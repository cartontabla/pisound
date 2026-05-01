#ifndef PISOUND_STREAM_BACKEND_H
#define PISOUND_STREAM_BACKEND_H

#include <alsa/asoundlib.h>
#include <netinet/in.h>
#include <stdint.h>

#include "../stream_direct_proto.h"
#include "stream_contract.h"

#define STREAM_BACKEND_MAX_CHANNELS 128

typedef enum {
    STREAM_IO_BACKEND_ALSA = 0,
    STREAM_IO_BACKEND_UDP = 1,
    STREAM_IO_BACKEND_VIRTUAL_ALSA = 2,
} stream_io_backend_kind_t;

typedef struct {
    stream_io_backend_kind_t kind;
    char device[64];
    char capture_device[64];
    char playback_device[64];
    char rx_host[64];
    char tx_host[64];
    int rx_port;
    int tx_port;
    uint16_t capture_hw_channels;
    uint16_t playback_hw_channels;
    uint16_t capture_channel_map[STREAM_BACKEND_MAX_CHANNELS];
    uint16_t playback_channel_map[STREAM_BACKEND_MAX_CHANNELS];
    struct sockaddr_in tx_addr;
    stream_pcm_format_t format;
} stream_io_cfg_t;

typedef struct {
    stream_io_cfg_t cfg;
    snd_pcm_t *pcm_in;
    snd_pcm_t *pcm_out;
    uint32_t capture_period_frames;
    uint32_t playback_period_frames;
    uint32_t capture_buffer_frames;
    uint32_t playback_buffer_frames;
    void *alsa_in_scratch;
    void *alsa_out_scratch;
    size_t alsa_in_scratch_bytes;
    size_t alsa_out_scratch_bytes;
    float *alsa_in_float_scratch;
    float *alsa_out_float_scratch;
    size_t alsa_in_float_scratch_samples;
    size_t alsa_out_float_scratch_samples;
    float *alsa_capture_fifo;
    float *alsa_playback_fifo;
    uint32_t alsa_capture_fifo_frames;
    uint32_t alsa_playback_fifo_frames;
    uint32_t alsa_capture_fifo_capacity_frames;
    uint32_t alsa_playback_fifo_capacity_frames;
    int udp_in;
    int udp_out;
    stream_state_t state;
    uint64_t xruns;
    uint32_t delay_frames;
    uint64_t last_sequence;
    uint64_t rx_sequence;
    uint64_t rx_packets;
    uint64_t tx_packets;
    uint64_t last_rx_generation;
    uint64_t last_tx_generation;
    uint64_t timing_log_counter;
    uint32_t drift_target_delay_frames;
    uint64_t drift_corrections;
} stream_backend_t;

void stream_io_cfg_load_from_env(stream_io_cfg_t *cfg, const stream_pcm_format_t *default_format);
const char *stream_backend_kind_name(stream_io_backend_kind_t kind);
int stream_backend_open(stream_backend_t *backend);
int stream_backend_prepare(stream_backend_t *backend);
int stream_backend_start(stream_backend_t *backend);
int stream_backend_stop(stream_backend_t *backend);
int stream_backend_recover(stream_backend_t *backend);
void stream_backend_query_status(stream_backend_t *backend, stream_status_t *status);
void stream_backend_close(stream_backend_t *backend);
int stream_backend_read_frame(stream_backend_t *backend,
                              stream_direct_hdr_t *hdr,
                              float *interleaved,
                              size_t sample_count);
int stream_backend_write_frame(stream_backend_t *backend,
                               stream_direct_hdr_t *hdr,
                               const float *interleaved,
                               size_t sample_count);

#endif
