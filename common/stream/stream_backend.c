#include "stream_backend.h"

#include <arpa/inet.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define STREAM_BACKEND_DEVICE_DEFAULT "aes67"
#define STREAM_BACKEND_CAPTURE_DEVICE_DEFAULT "capture_aes67_app"
#define STREAM_BACKEND_PLAYBACK_DEVICE_DEFAULT "playback_aes67_app"
#define STREAM_RX_HOST_DEFAULT "127.0.0.1"
#define STREAM_TX_HOST_DEFAULT "127.0.0.1"
#ifndef STREAM_RX_PORT_DEFAULT
#define STREAM_RX_PORT_DEFAULT 15010
#endif
#ifndef STREAM_TX_PORT_DEFAULT
#define STREAM_TX_PORT_DEFAULT 15011
#endif

static void stream_backend_reset_runtime(stream_backend_t *backend)
{
    backend->xruns = 0;
    backend->delay_frames = 0;
    backend->drift_target_delay_frames = 0;
    backend->drift_corrections = 0;
    backend->last_sequence = 0;
    backend->rx_sequence = 0;
    backend->rx_packets = 0;
    backend->tx_packets = 0;
    backend->last_rx_generation = 0;
    backend->last_tx_generation = 0;
    backend->timing_log_counter = 0;
}

static int getenv_int(const char *name, int fallback)
{
    const char *value = getenv(name);

    if (!value || value[0] == '\0') {
        return fallback;
    }
    return atoi(value);
}

static int getenv_flag(const char *name)
{
    const char *value = getenv(name);

    if (!value || value[0] == '\0') {
        return 0;
    }
    return !(strcmp(value, "0") == 0 ||
             strcasecmp(value, "false") == 0 ||
             strcasecmp(value, "no") == 0 ||
             strcasecmp(value, "off") == 0);
}

static int stream_backend_verbose(void)
{
    return getenv_flag("STREAM_VERBOSE");
}

static int stream_backend_debug_drift(void)
{
    return getenv_flag("STREAM_DEBUG_DRIFT") || getenv_flag("STREAM_DEBUG_STATUS");
}

static stream_sample_format_t getenv_sample_format(const char *name,
                                                   stream_sample_format_t fallback)
{
    const char *value = getenv(name);

    if (!value || value[0] == '\0') {
        return fallback;
    }
    if (strcasecmp(value, "s16") == 0 || strcasecmp(value, "s16le") == 0) {
        return STREAM_SAMPLE_S16LE;
    }
    if (strcasecmp(value, "s24") == 0 || strcasecmp(value, "s24le") == 0 ||
        strcasecmp(value, "s24_3le") == 0) {
        return STREAM_SAMPLE_S24LE;
    }
    if (strcasecmp(value, "s32") == 0 || strcasecmp(value, "s32le") == 0) {
        return STREAM_SAMPLE_S32LE;
    }
    if (strcasecmp(value, "f32") == 0 || strcasecmp(value, "float") == 0 ||
        strcasecmp(value, "float32") == 0 || strcasecmp(value, "float32le") == 0) {
        return STREAM_SAMPLE_FLOAT32LE;
    }
    fprintf(stderr,
            "stream_backend: unknown STREAM_SAMPLE_FORMAT '%s', using %s\n",
            value,
            stream_sample_format_name(fallback));
    return fallback;
}

static uint64_t monotonic_us(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000ULL + (uint64_t)ts.tv_nsec / 1000ULL;
}

static snd_pcm_format_t to_alsa_format(stream_sample_format_t sample_format)
{
    switch (sample_format) {
        case STREAM_SAMPLE_S16LE:
            return SND_PCM_FORMAT_S16_LE;
        case STREAM_SAMPLE_S24LE:
            return SND_PCM_FORMAT_S24_3LE;
        case STREAM_SAMPLE_S32LE:
            return SND_PCM_FORMAT_S32_LE;
        case STREAM_SAMPLE_FLOAT32LE:
        default:
            return SND_PCM_FORMAT_FLOAT_LE;
    }
}

static int parse_channel_map(const char *value,
                             uint16_t *dst,
                             uint16_t logical_channels,
                             uint16_t hw_channels)
{
    char buffer[512];
    char *token;
    char *saveptr = NULL;
    unsigned int count = 0;

    if (!value || value[0] == '\0') {
        for (count = 0; count < logical_channels; ++count) {
            dst[count] = (uint16_t)count;
        }
        return 0;
    }

    snprintf(buffer, sizeof(buffer), "%s", value);
    token = strtok_r(buffer, ",", &saveptr);
    while (token && count < logical_channels) {
        int mapped = atoi(token);
        if (mapped < 0 || mapped >= (int)hw_channels) {
            return -1;
        }
        dst[count++] = (uint16_t)mapped;
        token = strtok_r(NULL, ",", &saveptr);
    }

    if (count != logical_channels || token != NULL) {
        return -1;
    }
    return 0;
}

static size_t stream_bytes_per_period_for_layout(const stream_pcm_format_t *format,
                                                 uint16_t hw_channels)
{
    stream_pcm_format_t hw_format = *format;
    hw_format.channels = hw_channels;
    return stream_bytes_per_period(&hw_format);
}

static size_t stream_samples_per_period_for_layout(const stream_pcm_format_t *format,
                                                   uint16_t hw_channels)
{
    return (size_t)format->frames_per_period * (size_t)hw_channels;
}

void stream_io_cfg_load_from_env(stream_io_cfg_t *cfg, const stream_pcm_format_t *default_format)
{
    const char *backend = getenv("STREAM_BACKEND");
    const char *device = getenv("STREAM_DEVICE");
    const char *capture_device = getenv("STREAM_CAPTURE_DEVICE");
    const char *playback_device = getenv("STREAM_PLAYBACK_DEVICE");
    const char *rx_host = getenv("STREAM_RX_HOST");
    const char *tx_host = getenv("STREAM_TX_HOST");
    const char *capture_map = getenv("STREAM_CAPTURE_MAP");
    const char *playback_map = getenv("STREAM_PLAYBACK_MAP");

    memset(cfg, 0, sizeof(*cfg));
    cfg->format = *default_format;
    cfg->format.sample_format =
        getenv_sample_format("STREAM_SAMPLE_FORMAT", cfg->format.sample_format);
    if (backend && backend[0] != '\0') {
        if (strcasecmp(backend, "udp") == 0) {
            cfg->kind = STREAM_IO_BACKEND_UDP;
        } else if (strcasecmp(backend, "virtual_alsa") == 0 ||
                   strcasecmp(backend, "virtual-alsa") == 0) {
            cfg->kind = STREAM_IO_BACKEND_VIRTUAL_ALSA;
        } else {
            cfg->kind = STREAM_IO_BACKEND_ALSA;
        }
    } else {
#if defined(STREAM_BACKEND_DEFAULT_UDP) && STREAM_BACKEND_DEFAULT_UDP
        cfg->kind = STREAM_IO_BACKEND_UDP;
#elif defined(STREAM_BACKEND_DEFAULT_ALSA) && STREAM_BACKEND_DEFAULT_ALSA
        cfg->kind = STREAM_IO_BACKEND_ALSA;
#else
        cfg->kind = STREAM_IO_BACKEND_VIRTUAL_ALSA;
#endif
    }
    snprintf(cfg->device, sizeof(cfg->device), "%s", device ? device : STREAM_BACKEND_DEVICE_DEFAULT);
    snprintf(cfg->capture_device,
             sizeof(cfg->capture_device),
             "%s",
             capture_device ? capture_device :
             (cfg->kind == STREAM_IO_BACKEND_VIRTUAL_ALSA
                  ? STREAM_BACKEND_CAPTURE_DEVICE_DEFAULT
                  : cfg->device));
    snprintf(cfg->playback_device,
             sizeof(cfg->playback_device),
             "%s",
             playback_device ? playback_device :
             (cfg->kind == STREAM_IO_BACKEND_VIRTUAL_ALSA
                  ? STREAM_BACKEND_PLAYBACK_DEVICE_DEFAULT
                  : cfg->device));
    snprintf(cfg->rx_host, sizeof(cfg->rx_host), "%s", rx_host ? rx_host : STREAM_RX_HOST_DEFAULT);
    snprintf(cfg->tx_host, sizeof(cfg->tx_host), "%s", tx_host ? tx_host : STREAM_TX_HOST_DEFAULT);
    cfg->rx_port = getenv_int("STREAM_RX_PORT", STREAM_RX_PORT_DEFAULT);
    cfg->tx_port = getenv_int("STREAM_TX_PORT", STREAM_TX_PORT_DEFAULT);
    cfg->capture_hw_channels =
        (uint16_t)getenv_int("STREAM_CAPTURE_HW_CHANNELS", cfg->format.channels);
    cfg->playback_hw_channels =
        (uint16_t)getenv_int("STREAM_PLAYBACK_HW_CHANNELS", cfg->format.channels);
    if (cfg->capture_hw_channels == 0) {
        cfg->capture_hw_channels = cfg->format.channels;
    }
    if (cfg->playback_hw_channels == 0) {
        cfg->playback_hw_channels = cfg->format.channels;
    }
    if (parse_channel_map(capture_map,
                          cfg->capture_channel_map,
                          cfg->format.channels,
                          cfg->capture_hw_channels) != 0) {
        fprintf(stderr,
                "stream_backend: invalid STREAM_CAPTURE_MAP '%s', using identity map\n",
                capture_map ? capture_map : "");
        parse_channel_map(NULL,
                          cfg->capture_channel_map,
                          cfg->format.channels,
                          cfg->capture_hw_channels);
    }
    if (parse_channel_map(playback_map,
                          cfg->playback_channel_map,
                          cfg->format.channels,
                          cfg->playback_hw_channels) != 0) {
        fprintf(stderr,
                "stream_backend: invalid STREAM_PLAYBACK_MAP '%s', using identity map\n",
                playback_map ? playback_map : "");
        parse_channel_map(NULL,
                          cfg->playback_channel_map,
                          cfg->format.channels,
                          cfg->playback_hw_channels);
    }
}

static float clamp_unit(float sample)
{
    if (sample > 1.0f) {
        return 1.0f;
    }
    if (sample < -1.0f) {
        return -1.0f;
    }
    return sample;
}

static void stream_backend_apply_playback_drift_guard(stream_backend_t *backend,
                                                      uint32_t logical_frames)
{
    (void)backend;
    (void)logical_frames;
}

static void alsa_interleaved_to_float(const stream_pcm_format_t *format,
                                      const void *src,
                                      float *dst,
                                      size_t sample_count)
{
    size_t i;

    switch (format->sample_format) {
        case STREAM_SAMPLE_S16LE: {
            const int16_t *samples = (const int16_t *)src;
            for (i = 0; i < sample_count; ++i) {
                dst[i] = (float)samples[i] / 32768.0f;
            }
            break;
        }
        case STREAM_SAMPLE_S24LE: {
            const uint8_t *samples = (const uint8_t *)src;
            for (i = 0; i < sample_count; ++i) {
                int32_t value =
                    ((int32_t)samples[i * 3 + 0]) |
                    ((int32_t)samples[i * 3 + 1] << 8) |
                    ((int32_t)samples[i * 3 + 2] << 16);
                if (value & 0x00800000) {
                    value |= ~0x00ffffff;
                }
                dst[i] = (float)value / 8388608.0f;
            }
            break;
        }
        case STREAM_SAMPLE_S32LE: {
            const int32_t *samples = (const int32_t *)src;
            for (i = 0; i < sample_count; ++i) {
                dst[i] = (float)samples[i] / 2147483648.0f;
            }
            break;
        }
        case STREAM_SAMPLE_FLOAT32LE:
        default:
            memcpy(dst, src, sample_count * sizeof(float));
            break;
    }
}

static void float_to_alsa_interleaved(const stream_pcm_format_t *format,
                                      const float *src,
                                      void *dst,
                                      size_t sample_count)
{
    size_t i;

    switch (format->sample_format) {
        case STREAM_SAMPLE_S16LE: {
            int16_t *samples = (int16_t *)dst;
            for (i = 0; i < sample_count; ++i) {
                float clamped = clamp_unit(src[i]);
                long value = lroundf(clamped * 32767.0f);
                if (value < -32768L) {
                    value = -32768L;
                } else if (value > 32767L) {
                    value = 32767L;
                }
                samples[i] = (int16_t)value;
            }
            break;
        }
        case STREAM_SAMPLE_S24LE: {
            uint8_t *samples = (uint8_t *)dst;
            for (i = 0; i < sample_count; ++i) {
                float clamped = clamp_unit(src[i]);
                long value = lroundf(clamped * 8388607.0f);
                if (value < -8388608L) {
                    value = -8388608L;
                } else if (value > 8388607L) {
                    value = 8388607L;
                }
                samples[i * 3 + 0] = (uint8_t)(value & 0xff);
                samples[i * 3 + 1] = (uint8_t)((value >> 8) & 0xff);
                samples[i * 3 + 2] = (uint8_t)((value >> 16) & 0xff);
            }
            break;
        }
        case STREAM_SAMPLE_S32LE: {
            int32_t *samples = (int32_t *)dst;
            for (i = 0; i < sample_count; ++i) {
                float clamped = clamp_unit(src[i]);
                long long value = llroundf(clamped * 2147483647.0f);
                if (value < -2147483648LL) {
                    value = -2147483648LL;
                } else if (value > 2147483647LL) {
                    value = 2147483647LL;
                }
                samples[i] = (int32_t)value;
            }
            break;
        }
        case STREAM_SAMPLE_FLOAT32LE:
        default:
            memcpy(dst, src, sample_count * sizeof(float));
            break;
    }
}

static int stream_backend_alloc_alsa_scratch(stream_backend_t *backend)
{
    size_t in_period_bytes;
    size_t out_period_bytes;
    size_t in_period_samples;
    size_t out_period_samples;
    uint32_t logical_frames;
    uint32_t in_period_frames;
    uint32_t out_period_frames;
    uint32_t capture_fifo_frames;
    uint32_t playback_fifo_frames;
    stream_pcm_format_t in_period_format;
    stream_pcm_format_t out_period_format;

    if (!backend || backend->cfg.kind == STREAM_IO_BACKEND_UDP) {
        return 0;
    }

    logical_frames = backend->cfg.format.frames_per_period;
    in_period_frames = backend->capture_period_frames
                           ? backend->capture_period_frames
                           : backend->cfg.format.frames_per_period;
    out_period_frames = backend->playback_period_frames
                            ? backend->playback_period_frames
                            : backend->cfg.format.frames_per_period;
    if (in_period_frames < logical_frames) {
        in_period_frames = logical_frames;
    }
    if (out_period_frames < logical_frames) {
        out_period_frames = logical_frames;
    }
    in_period_format = backend->cfg.format;
    out_period_format = backend->cfg.format;
    in_period_format.frames_per_period = in_period_frames;
    out_period_format.frames_per_period = out_period_frames;

    in_period_bytes = stream_bytes_per_period_for_layout(&in_period_format,
                                                         backend->cfg.capture_hw_channels);
    out_period_bytes = stream_bytes_per_period_for_layout(&out_period_format,
                                                          backend->cfg.playback_hw_channels);
    in_period_samples = stream_samples_per_period_for_layout(&in_period_format,
                                                             backend->cfg.capture_hw_channels);
    out_period_samples = stream_samples_per_period_for_layout(&out_period_format,
                                                              backend->cfg.playback_hw_channels);
    capture_fifo_frames = logical_frames +
                          (backend->capture_buffer_frames
                               ? backend->capture_buffer_frames
                               : in_period_frames);
    playback_fifo_frames = logical_frames +
                           (backend->playback_buffer_frames
                                ? backend->playback_buffer_frames
                                : out_period_frames);

    backend->alsa_in_float_scratch = calloc(in_period_samples, sizeof(float));
    backend->alsa_out_float_scratch = calloc(out_period_samples, sizeof(float));
    backend->alsa_capture_fifo =
        calloc((size_t)capture_fifo_frames * backend->cfg.format.channels, sizeof(float));
    backend->alsa_playback_fifo =
        calloc((size_t)playback_fifo_frames * backend->cfg.format.channels, sizeof(float));
    if (!backend->alsa_in_float_scratch ||
        !backend->alsa_out_float_scratch ||
        !backend->alsa_capture_fifo ||
        !backend->alsa_playback_fifo) {
        fprintf(stderr, "stream_backend: unable to allocate ALSA float scratch buffers\n");
        free(backend->alsa_in_float_scratch);
        free(backend->alsa_out_float_scratch);
        free(backend->alsa_capture_fifo);
        free(backend->alsa_playback_fifo);
        backend->alsa_in_float_scratch = NULL;
        backend->alsa_out_float_scratch = NULL;
        backend->alsa_capture_fifo = NULL;
        backend->alsa_playback_fifo = NULL;
        return -1;
    }
    backend->alsa_in_float_scratch_samples = in_period_samples;
    backend->alsa_out_float_scratch_samples = out_period_samples;
    backend->alsa_capture_fifo_capacity_frames = capture_fifo_frames;
    backend->alsa_playback_fifo_capacity_frames = playback_fifo_frames;
    backend->alsa_capture_fifo_frames = 0;
    backend->alsa_playback_fifo_frames = 0;

    if (backend->cfg.format.sample_format != STREAM_SAMPLE_FLOAT32LE) {
        backend->alsa_in_scratch = calloc(1, in_period_bytes);
        backend->alsa_out_scratch = calloc(1, out_period_bytes);
        if (!backend->alsa_in_scratch || !backend->alsa_out_scratch) {
            fprintf(stderr,
                    "stream_backend: unable to allocate ALSA scratch buffers (%zu/%zu bytes)\n",
                    in_period_bytes,
                    out_period_bytes);
            free(backend->alsa_in_scratch);
            free(backend->alsa_out_scratch);
            backend->alsa_in_scratch = NULL;
            backend->alsa_out_scratch = NULL;
            free(backend->alsa_in_float_scratch);
            free(backend->alsa_out_float_scratch);
            free(backend->alsa_capture_fifo);
            free(backend->alsa_playback_fifo);
            backend->alsa_in_float_scratch = NULL;
            backend->alsa_out_float_scratch = NULL;
            backend->alsa_capture_fifo = NULL;
            backend->alsa_playback_fifo = NULL;
            return -1;
        }
        backend->alsa_in_scratch_bytes = in_period_bytes;
        backend->alsa_out_scratch_bytes = out_period_bytes;
    }
    return 0;
}

const char *stream_backend_kind_name(stream_io_backend_kind_t kind)
{
    switch (kind) {
        case STREAM_IO_BACKEND_ALSA:
            return "alsa";
        case STREAM_IO_BACKEND_UDP:
            return "udp";
        case STREAM_IO_BACKEND_VIRTUAL_ALSA:
            return "virtual_alsa";
        default:
            return "unknown";
    }
}

static int alsa_open_one(const stream_io_cfg_t *cfg,
                         snd_pcm_stream_t dir,
                         uint16_t hw_channels,
                         snd_pcm_t **pcm,
                         uint32_t *actual_period_frames,
                         uint32_t *actual_buffer_frames)
{
    int err;
    snd_pcm_hw_params_t *hw;
    snd_pcm_sw_params_t *sw;
    snd_pcm_uframes_t period_size;
    snd_pcm_uframes_t buffer_size;
    const char *device =
        (dir == SND_PCM_STREAM_CAPTURE) ? cfg->capture_device : cfg->playback_device;

    err = snd_pcm_open(pcm, device, dir, 0);
    if (err < 0) {
        fprintf(stderr,
                "stream_backend: cannot open %s device '%s': %s\n",
                dir == SND_PCM_STREAM_CAPTURE ? "capture" : "playback",
                device,
                snd_strerror(err));
        return err;
    }

    snd_pcm_hw_params_alloca(&hw);
    snd_pcm_sw_params_alloca(&sw);
    snd_pcm_hw_params_any(*pcm, hw);
    snd_pcm_hw_params_set_access(*pcm, hw, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(*pcm, hw, to_alsa_format(cfg->format.sample_format));
    snd_pcm_hw_params_set_channels(*pcm, hw, hw_channels);
    snd_pcm_hw_params_set_rate(*pcm, hw, cfg->format.sample_rate_hz, 0);

    period_size = cfg->format.frames_per_period;
    buffer_size = cfg->format.frames_per_period * cfg->format.period_count;
    snd_pcm_hw_params_set_period_size_near(*pcm, hw, &period_size, 0);
    snd_pcm_hw_params_set_buffer_size_near(*pcm, hw, &buffer_size);

    err = snd_pcm_hw_params(*pcm, hw);
    if (err < 0) {
        fprintf(stderr, "stream_backend: hw_params failed on '%s': %s\n", device, snd_strerror(err));
        snd_pcm_close(*pcm);
        *pcm = NULL;
        return err;
    }

    snd_pcm_hw_params_get_period_size(hw, &period_size, 0);
    snd_pcm_hw_params_get_buffer_size(hw, &buffer_size);

    snd_pcm_sw_params_current(*pcm, sw);
    snd_pcm_sw_params_set_avail_min(*pcm, sw, period_size);
    if (dir == SND_PCM_STREAM_PLAYBACK) {
        snd_pcm_uframes_t start_threshold =
            (buffer_size > period_size) ? (buffer_size - period_size) : period_size;
        snd_pcm_sw_params_set_start_threshold(*pcm, sw, start_threshold);
    } else {
        snd_pcm_sw_params_set_start_threshold(*pcm, sw, 1);
    }
    err = snd_pcm_sw_params(*pcm, sw);
    if (err < 0) {
        fprintf(stderr, "stream_backend: sw_params failed on '%s': %s\n", device, snd_strerror(err));
        snd_pcm_close(*pcm);
        *pcm = NULL;
        return err;
    }

    snd_pcm_prepare(*pcm);
    if (actual_period_frames) {
        *actual_period_frames = (uint32_t)period_size;
    }
    if (actual_buffer_frames) {
        *actual_buffer_frames = (uint32_t)buffer_size;
    }
    return 0;
}

static int udp_open(stream_backend_t *backend)
{
    struct sockaddr_in rx_addr;
    struct timeval tv;
    int reuse = 1;
    int block_bytes =
        (int)(sizeof(stream_direct_hdr_t) + stream_bytes_per_period(&backend->cfg.format));

    backend->udp_in = socket(AF_INET, SOCK_DGRAM, 0);
    backend->udp_out = socket(AF_INET, SOCK_DGRAM, 0);
    if (backend->udp_in < 0 || backend->udp_out < 0) {
        perror("stream_backend: socket");
        return -1;
    }

    setsockopt(backend->udp_in, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    setsockopt(backend->udp_in, SOL_SOCKET, SO_RCVBUF, &block_bytes, sizeof(block_bytes));
    setsockopt(backend->udp_out, SOL_SOCKET, SO_SNDBUF, &block_bytes, sizeof(block_bytes));

    memset(&rx_addr, 0, sizeof(rx_addr));
    rx_addr.sin_family = AF_INET;
    rx_addr.sin_port = htons((uint16_t)backend->cfg.rx_port);
    if (inet_pton(AF_INET, backend->cfg.rx_host, &rx_addr.sin_addr) != 1) {
        fprintf(stderr, "stream_backend: invalid STREAM_RX_HOST '%s'\n", backend->cfg.rx_host);
        return -1;
    }
    if (bind(backend->udp_in, (struct sockaddr *)&rx_addr, sizeof(rx_addr)) < 0) {
        fprintf(stderr,
                "stream_backend: cannot bind UDP RX %s:%d: %s\n",
                backend->cfg.rx_host,
                backend->cfg.rx_port,
                strerror(errno));
        return -1;
    }

    memset(&backend->cfg.tx_addr, 0, sizeof(backend->cfg.tx_addr));
    backend->cfg.tx_addr.sin_family = AF_INET;
    backend->cfg.tx_addr.sin_port = htons((uint16_t)backend->cfg.tx_port);
    if (inet_pton(AF_INET, backend->cfg.tx_host, &backend->cfg.tx_addr.sin_addr) != 1) {
        fprintf(stderr, "stream_backend: invalid STREAM_TX_HOST '%s'\n", backend->cfg.tx_host);
        return -1;
    }

    tv.tv_sec = 0;
    tv.tv_usec = 500000;
    setsockopt(backend->udp_in, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    return 0;
}

int stream_backend_open(stream_backend_t *backend)
{
    char errbuf[128];
    stream_io_cfg_t cfg = backend->cfg;

    memset(backend, 0, sizeof(*backend));
    backend->cfg = cfg;
    backend->udp_in = -1;
    backend->udp_out = -1;
    backend->state = STREAM_STATE_CLOSED;
    stream_backend_reset_runtime(backend);

    if (stream_pcm_format_validate(&backend->cfg.format, errbuf, sizeof(errbuf)) != 0) {
        fprintf(stderr, "stream_backend: invalid format: %s\n", errbuf);
        return -1;
    }
    if (backend->cfg.capture_hw_channels < backend->cfg.format.channels ||
        backend->cfg.playback_hw_channels < backend->cfg.format.channels ||
        backend->cfg.capture_hw_channels > STREAM_BACKEND_MAX_CHANNELS ||
        backend->cfg.playback_hw_channels > STREAM_BACKEND_MAX_CHANNELS) {
        fprintf(stderr,
                "stream_backend: invalid hw channel layout capture=%u playback=%u logical=%u\n",
                backend->cfg.capture_hw_channels,
                backend->cfg.playback_hw_channels,
                backend->cfg.format.channels);
        return -1;
    }

    if (backend->cfg.kind == STREAM_IO_BACKEND_UDP) {
        if (udp_open(backend) < 0) {
            return -1;
        }
        backend->state = STREAM_STATE_OPEN;
        return 0;
    }

    if (alsa_open_one(&backend->cfg,
                      SND_PCM_STREAM_CAPTURE,
                      backend->cfg.capture_hw_channels,
                      &backend->pcm_in,
                      &backend->capture_period_frames,
                      &backend->capture_buffer_frames) < 0) {
        return -1;
    }
    if (alsa_open_one(&backend->cfg,
                      SND_PCM_STREAM_PLAYBACK,
                      backend->cfg.playback_hw_channels,
                      &backend->pcm_out,
                      &backend->playback_period_frames,
                      &backend->playback_buffer_frames) < 0) {
        snd_pcm_close(backend->pcm_in);
        backend->pcm_in = NULL;
        return -1;
    }
    if (stream_backend_alloc_alsa_scratch(backend) < 0) {
        snd_pcm_close(backend->pcm_in);
        snd_pcm_close(backend->pcm_out);
        backend->pcm_in = NULL;
        backend->pcm_out = NULL;
        return -1;
    }
    if (stream_backend_verbose()) {
        fprintf(stderr,
                "stream_backend: ALSA capture period=%u buffer=%u playback period=%u buffer=%u logical_frames=%u\n",
                backend->capture_period_frames,
                backend->capture_buffer_frames,
                backend->playback_period_frames,
                backend->playback_buffer_frames,
                backend->cfg.format.frames_per_period);
    }
    backend->state = STREAM_STATE_OPEN;
    return 0;
}

int stream_backend_prepare(stream_backend_t *backend)
{
    if (!backend) {
        return -1;
    }
    if (backend->state != STREAM_STATE_OPEN &&
        backend->state != STREAM_STATE_STOPPED &&
        backend->state != STREAM_STATE_PREPARED) {
        fprintf(stderr,
                "stream_backend: cannot prepare from state '%s'\n",
                stream_state_name(backend->state));
        return -1;
    }

    if (backend->cfg.kind != STREAM_IO_BACKEND_UDP) {
        if (backend->pcm_in && snd_pcm_prepare(backend->pcm_in) < 0) {
            fprintf(stderr, "stream_backend: prepare failed on capture PCM\n");
            return -1;
        }
        if (backend->pcm_out && snd_pcm_prepare(backend->pcm_out) < 0) {
            fprintf(stderr, "stream_backend: prepare failed on playback PCM\n");
            return -1;
        }
    }

    backend->delay_frames = 0;
    backend->state = STREAM_STATE_PREPARED;
    return 0;
}

int stream_backend_start(stream_backend_t *backend)
{
    if (!backend) {
        return -1;
    }
    if (backend->state != STREAM_STATE_PREPARED) {
        fprintf(stderr,
                "stream_backend: cannot start from state '%s'\n",
                stream_state_name(backend->state));
        return -1;
    }
    backend->state = STREAM_STATE_RUNNING;
    return 0;
}

int stream_backend_stop(stream_backend_t *backend)
{
    if (!backend) {
        return -1;
    }
    if (backend->state != STREAM_STATE_PREPARED &&
        backend->state != STREAM_STATE_RUNNING &&
        backend->state != STREAM_STATE_DRAINING) {
        fprintf(stderr,
                "stream_backend: cannot stop from state '%s'\n",
                stream_state_name(backend->state));
        return -1;
    }

    if (backend->cfg.kind != STREAM_IO_BACKEND_UDP) {
        if (backend->pcm_in) {
            snd_pcm_drop(backend->pcm_in);
        }
        if (backend->pcm_out) {
            snd_pcm_drop(backend->pcm_out);
        }
    }

    backend->delay_frames = 0;
    backend->state = STREAM_STATE_STOPPED;
    return 0;
}

int stream_backend_recover(stream_backend_t *backend)
{
    if (!backend) {
        return -1;
    }
    if (backend->state != STREAM_STATE_RUNNING &&
        backend->state != STREAM_STATE_DRAINING &&
        backend->state != STREAM_STATE_STOPPED &&
        backend->state != STREAM_STATE_PREPARED) {
        fprintf(stderr,
                "stream_backend: cannot recover from state '%s'\n",
                stream_state_name(backend->state));
        return -1;
    }
    if (backend->cfg.kind != STREAM_IO_BACKEND_UDP) {
        if (backend->pcm_in && snd_pcm_prepare(backend->pcm_in) < 0) {
            fprintf(stderr, "stream_backend: recover failed on capture PCM\n");
            return -1;
        }
        if (backend->pcm_out && snd_pcm_prepare(backend->pcm_out) < 0) {
            fprintf(stderr, "stream_backend: recover failed on playback PCM\n");
            return -1;
        }
    }
    backend->delay_frames = 0;
    backend->state = STREAM_STATE_PREPARED;
    return 0;
}

void stream_backend_query_status(stream_backend_t *backend, stream_status_t *status)
{
    snd_pcm_sframes_t delay = 0;
    uint32_t capacity_frames;

    if (!backend || !status) {
        return;
    }

    if (backend->cfg.kind != STREAM_IO_BACKEND_UDP &&
        backend->pcm_out &&
        snd_pcm_delay(backend->pcm_out, &delay) == 0 &&
        delay > 0) {
        backend->delay_frames = (uint32_t)delay;
    }

    memset(status, 0, sizeof(*status));
    capacity_frames = backend->playback_buffer_frames
                          ? backend->playback_buffer_frames
                          : (backend->cfg.format.frames_per_period *
                             backend->cfg.format.period_count);
    status->state = backend->state;
    status->queued_frames = backend->delay_frames;
    status->available_frames =
        (backend->delay_frames < capacity_frames)
            ? (capacity_frames - backend->delay_frames)
            : 0;
    status->xruns = backend->xruns;
    status->delay_frames = backend->delay_frames;
    status->last_sequence = backend->last_sequence;
}

void stream_backend_close(stream_backend_t *backend)
{
    free(backend->alsa_in_scratch);
    free(backend->alsa_out_scratch);
    free(backend->alsa_in_float_scratch);
    free(backend->alsa_out_float_scratch);
    free(backend->alsa_capture_fifo);
    free(backend->alsa_playback_fifo);
    backend->alsa_in_scratch = NULL;
    backend->alsa_out_scratch = NULL;
    backend->alsa_in_float_scratch = NULL;
    backend->alsa_out_float_scratch = NULL;
    backend->alsa_capture_fifo = NULL;
    backend->alsa_playback_fifo = NULL;
    backend->alsa_in_scratch_bytes = 0;
    backend->alsa_out_scratch_bytes = 0;
    backend->alsa_in_float_scratch_samples = 0;
    backend->alsa_out_float_scratch_samples = 0;
    backend->alsa_capture_fifo_frames = 0;
    backend->alsa_playback_fifo_frames = 0;
    backend->alsa_capture_fifo_capacity_frames = 0;
    backend->alsa_playback_fifo_capacity_frames = 0;
    if (backend->pcm_in) {
        snd_pcm_close(backend->pcm_in);
        backend->pcm_in = NULL;
    }
    if (backend->pcm_out) {
        snd_pcm_close(backend->pcm_out);
        backend->pcm_out = NULL;
    }
    if (backend->udp_in >= 0) {
        close(backend->udp_in);
        backend->udp_in = -1;
    }
    if (backend->udp_out >= 0) {
        close(backend->udp_out);
        backend->udp_out = -1;
    }
    backend->state = STREAM_STATE_CLOSED;
    backend->delay_frames = 0;
}

int stream_backend_read_frame(stream_backend_t *backend,
                              stream_direct_hdr_t *hdr,
                              float *interleaved,
                              size_t sample_count)
{
    uint32_t logical_frames;

    if (backend->state != STREAM_STATE_RUNNING) {
        fprintf(stderr,
                "stream_backend: cannot read from state '%s'\n",
                stream_state_name(backend->state));
        return -1;
    }

    if (backend->cfg.kind == STREAM_IO_BACKEND_UDP) {
        size_t packet_bytes = sizeof(stream_direct_hdr_t) + sample_count * sizeof(float);
        uint8_t packet_buf[sizeof(stream_direct_hdr_t) + 2048];
        stream_direct_hdr_t *packet_hdr;
        float *packet_samples;
        ssize_t got;

        if (packet_bytes > sizeof(packet_buf)) {
            fprintf(stderr, "stream_backend: UDP packet too large (%zu bytes)\n", packet_bytes);
            return -1;
        }
        got = recv(backend->udp_in, packet_buf, packet_bytes, 0);

        if (got < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
                return 1;
            }
            fprintf(stderr, "stream_backend: UDP read error: %s\n", strerror(errno));
            return -1;
        }
        if ((size_t)got != packet_bytes) {
            return 1;
        }
        packet_hdr = (stream_direct_hdr_t *)packet_buf;
        packet_samples = (float *)(packet_buf + sizeof(stream_direct_hdr_t));
        if (packet_hdr->magic != STREAM_DIRECT_MAGIC ||
            packet_hdr->frames != backend->cfg.format.frames_per_period) {
            return 1;
        }
        *hdr = *packet_hdr;
        hdr->app_rx_us = monotonic_us();
        memcpy(interleaved, packet_samples, sample_count * sizeof(float));
        backend->rx_packets++;
        backend->last_sequence = packet_hdr->generation;
        if (backend->last_rx_generation &&
            packet_hdr->generation <= backend->last_rx_generation) {
            fprintf(stderr,
                    "stream_backend: UDP RX generation repeated/out-of-order prev=%llu curr=%llu\n",
                    (unsigned long long)backend->last_rx_generation,
                    (unsigned long long)packet_hdr->generation);
        }
        backend->last_rx_generation = packet_hdr->generation;
        return 0;
    }

    logical_frames = (uint32_t)(sample_count / backend->cfg.format.channels);
    {
        uint32_t io_frames = logical_frames;
        while (backend->alsa_capture_fifo_frames < logical_frames) {
            uint32_t frames_this_read = logical_frames - backend->alsa_capture_fifo_frames;
            float *hw_interleaved = backend->alsa_in_float_scratch;
            void *pcm_target =
                (backend->cfg.format.sample_format == STREAM_SAMPLE_FLOAT32LE)
                    ? (void *)hw_interleaved
                    : backend->alsa_in_scratch;
            size_t hw_sample_count;
            int err;
            uint32_t frame;
            uint16_t ch;

            hw_sample_count = (size_t)frames_this_read * backend->cfg.capture_hw_channels;
            err = snd_pcm_readi(backend->pcm_in, pcm_target, frames_this_read);
            if (err < 0) {
                int recovered = snd_pcm_recover(backend->pcm_in, err, 1);
                if (recovered == 0) {
                    backend->xruns++;
                    backend->state = STREAM_STATE_RUNNING;
                    fprintf(stderr,
                            "stream_backend: capture recover ok after ALSA error: %s (xruns=%llu)\n",
                            snd_strerror(err),
                            (unsigned long long)backend->xruns);
                    return 1;
                }
            }
            if (err < 0) {
                fprintf(stderr, "stream_backend: read error: %s\n", snd_strerror(err));
                return -1;
            }
            if (err == 0) {
                return 1;
            }
            frames_this_read = (uint32_t)err;
            if (backend->alsa_capture_fifo_frames + frames_this_read >
                backend->alsa_capture_fifo_capacity_frames) {
                fprintf(stderr, "stream_backend: capture FIFO overflow\n");
                return -1;
            }
            hw_sample_count = (size_t)frames_this_read * backend->cfg.capture_hw_channels;
            if (backend->cfg.format.sample_format != STREAM_SAMPLE_FLOAT32LE) {
                alsa_interleaved_to_float(&backend->cfg.format,
                                          backend->alsa_in_scratch,
                                          hw_interleaved,
                                          hw_sample_count);
            }
            for (frame = 0; frame < frames_this_read; ++frame) {
                for (ch = 0; ch < backend->cfg.format.channels; ++ch) {
                    backend->alsa_capture_fifo
                        [(backend->alsa_capture_fifo_frames + frame) *
                             backend->cfg.format.channels +
                         ch] =
                        hw_interleaved[frame * backend->cfg.capture_hw_channels +
                                       backend->cfg.capture_channel_map[ch]];
                }
            }
            backend->alsa_capture_fifo_frames += frames_this_read;
        }
        memcpy(interleaved,
               backend->alsa_capture_fifo,
               (size_t)logical_frames * backend->cfg.format.channels * sizeof(float));
        backend->alsa_capture_fifo_frames -= logical_frames;
        if (backend->alsa_capture_fifo_frames > 0) {
            memmove(backend->alsa_capture_fifo,
                    backend->alsa_capture_fifo +
                        ((size_t)logical_frames * backend->cfg.format.channels),
                    (size_t)backend->alsa_capture_fifo_frames *
                        backend->cfg.format.channels *
                        sizeof(float));
        }
    }

    memset(hdr, 0, sizeof(*hdr));
    hdr->magic = STREAM_DIRECT_MAGIC;
    hdr->frames = backend->cfg.format.frames_per_period;
    hdr->generation = ++backend->rx_sequence;
    hdr->app_rx_us = monotonic_us();
    backend->last_sequence = hdr->generation;
    return 0;
}

int stream_backend_write_frame(stream_backend_t *backend,
                               stream_direct_hdr_t *hdr,
                               const float *interleaved,
                               size_t sample_count)
{
    uint32_t logical_frames;

    if (backend->state != STREAM_STATE_RUNNING) {
        fprintf(stderr,
                "stream_backend: cannot write from state '%s'\n",
                stream_state_name(backend->state));
        return -1;
    }

    if (backend->cfg.kind == STREAM_IO_BACKEND_UDP) {
        size_t packet_bytes = sizeof(stream_direct_hdr_t) + sample_count * sizeof(float);
        uint8_t packet_buf[sizeof(stream_direct_hdr_t) + 2048];
        stream_direct_hdr_t *packet_hdr;
        float *packet_samples;
        ssize_t sent;

        if (packet_bytes > sizeof(packet_buf)) {
            fprintf(stderr, "stream_backend: UDP packet too large (%zu bytes)\n", packet_bytes);
            return -1;
        }
        packet_hdr = (stream_direct_hdr_t *)packet_buf;
        packet_samples = (float *)(packet_buf + sizeof(stream_direct_hdr_t));
        *packet_hdr = *hdr;
        packet_hdr->magic = STREAM_DIRECT_MAGIC;
        packet_hdr->frames = backend->cfg.format.frames_per_period;
        packet_hdr->app_tx_us = monotonic_us();
        memcpy(packet_samples, interleaved, sample_count * sizeof(float));
        sent = sendto(backend->udp_out,
                      packet_buf,
                      packet_bytes,
                      0,
                      (struct sockaddr *)&backend->cfg.tx_addr,
                      sizeof(backend->cfg.tx_addr));
        if (sent < 0) {
            fprintf(stderr, "stream_backend: UDP write error: %s\n", strerror(errno));
            return -1;
        }
        backend->tx_packets++;
        backend->last_sequence = packet_hdr->generation;
        if (backend->last_tx_generation &&
            packet_hdr->generation <= backend->last_tx_generation) {
            fprintf(stderr,
                    "stream_backend: UDP TX generation repeated/out-of-order prev=%llu curr=%llu\n",
                    (unsigned long long)backend->last_tx_generation,
                    (unsigned long long)packet_hdr->generation);
        }
        backend->last_tx_generation = packet_hdr->generation;
        if (++backend->timing_log_counter % 256 == 0 &&
            packet_hdr->bridge_rx_us != 0 &&
            packet_hdr->app_rx_us != 0 &&
            packet_hdr->app_tx_us != 0) {
            fprintf(stderr,
                    "stream_backend: gen=%llu bridge_to_app=%.3fms model=%.3fms total_app=%.3fms\n",
                    (unsigned long long)packet_hdr->generation,
                    (packet_hdr->app_rx_us - packet_hdr->bridge_rx_us) / 1000.0,
                    (packet_hdr->app_tx_us - packet_hdr->app_rx_us) / 1000.0,
                    (packet_hdr->app_tx_us - packet_hdr->bridge_rx_us) / 1000.0);
        }
        return 0;
    }

    logical_frames = (uint32_t)(sample_count / backend->cfg.format.channels);
    {
        uint32_t io_frames = logical_frames;
        if (backend->alsa_playback_fifo_frames + logical_frames >
            backend->alsa_playback_fifo_capacity_frames) {
            fprintf(stderr, "stream_backend: playback FIFO overflow\n");
            return -1;
        }
        memcpy(backend->alsa_playback_fifo +
                   ((size_t)backend->alsa_playback_fifo_frames *
                    backend->cfg.format.channels),
               interleaved,
               (size_t)logical_frames * backend->cfg.format.channels * sizeof(float));
        backend->alsa_playback_fifo_frames += logical_frames;

        stream_backend_apply_playback_drift_guard(backend, logical_frames);

        while (backend->alsa_playback_fifo_frames >= io_frames) {
            const void *pcm_source = interleaved;
            float *hw_interleaved = backend->alsa_out_float_scratch;
            uint32_t frames_this_write = io_frames;
            size_t hw_sample_count;
            uint32_t frame;
            uint16_t ch;
            int err;

            hw_sample_count = (size_t)frames_this_write * backend->cfg.playback_hw_channels;
            memset(hw_interleaved, 0, hw_sample_count * sizeof(float));
            for (frame = 0; frame < frames_this_write; ++frame) {
                for (ch = 0; ch < backend->cfg.format.channels; ++ch) {
                    hw_interleaved[frame * backend->cfg.playback_hw_channels +
                                   backend->cfg.playback_channel_map[ch]] =
                        backend->alsa_playback_fifo
                            [frame * backend->cfg.format.channels + ch];
                }
            }
            if (backend->cfg.format.sample_format != STREAM_SAMPLE_FLOAT32LE) {
                float_to_alsa_interleaved(&backend->cfg.format,
                                          hw_interleaved,
                                          backend->alsa_out_scratch,
                                          hw_sample_count);
                pcm_source = backend->alsa_out_scratch;
            } else {
                pcm_source = hw_interleaved;
            }
            err = snd_pcm_writei(backend->pcm_out, pcm_source, frames_this_write);
            if (err < 0) {
                int recovered = snd_pcm_recover(backend->pcm_out, err, 1);
                if (recovered == 0) {
                    backend->xruns++;
                    backend->state = STREAM_STATE_RUNNING;
                    fprintf(stderr,
                            "stream_backend: playback recover ok after ALSA error: %s (xruns=%llu)\n",
                            snd_strerror(err),
                            (unsigned long long)backend->xruns);
                    return 1;
                }
            }
            if (err < 0) {
                fprintf(stderr, "stream_backend: write error: %s\n", snd_strerror(err));
                return -1;
            }
            if (err == 0) {
                return 1;
            }
            if ((uint32_t)err > backend->alsa_playback_fifo_frames) {
                backend->alsa_playback_fifo_frames = 0;
            } else {
                backend->alsa_playback_fifo_frames -= (uint32_t)err;
            }
            if (backend->alsa_playback_fifo_frames > 0) {
                memmove(backend->alsa_playback_fifo,
                        backend->alsa_playback_fifo +
                            ((size_t)err * backend->cfg.format.channels),
                        (size_t)backend->alsa_playback_fifo_frames *
                            backend->cfg.format.channels *
                            sizeof(float));
            }
        }
    }
    backend->last_sequence = hdr ? hdr->generation : backend->last_sequence;
    return 0;
}
