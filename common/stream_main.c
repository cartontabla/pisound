/*
 * stream_main.c: Execution harness for Simulink models over AES67 (ALSA backend)
 * Part of the pisound framework — stream variant.
 *
 * Replaces custom_main.c when the model uses stream_in/stream_out instead of
 * pisound_in/pisound_out. The model code is identical; only the audio I/O
 * backend changes: ALSA reads/writes the PCM device exposed by aes67-daemon.
 *
 * aes67-daemon exposes a standard ALSA PCM device (e.g. "aes67") that any
 * ALSA-capable application can open. This harness reads one frame from that
 * device, calls MODEL_STEP(), then writes the result back.
 *
 * Usage (compile.sh passes -DSTREAM_MAIN to select this file):
 *   compile.sh <project> --stream
 *
 * Compile-time defines (same as custom_main.c):
 *   MODEL_HEADER, MODEL_INITIALIZE, MODEL_STEP, MODEL_TERMINATE
 *   MODEL_FRAME_LENGTH  (default 128)
 *   ALSA_DEVICE         (default "aes67")
 */

#include <alsa/asoundlib.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/select.h>
#include <unistd.h>

#include "stream_audio.h"

#ifdef ENABLE_CONTROL
#include "control/control.h"
#endif

#ifndef MODEL_FRAME_LENGTH
#define MODEL_FRAME_LENGTH 128
#endif

#ifndef ALSA_DEVICE
#define ALSA_DEVICE "aes67"
#endif

#ifndef MODEL_HEADER
#error "MODEL_HEADER not defined"
#endif
#include MODEL_HEADER

/* Parameter tuning (same mechanism as custom_main.c) */
#ifndef PARAM_STRUCT_VAR
#define PARAM_TUNING_ENABLED 0
#else
#define PARAM_TUNING_ENABLED 1
extern PARAM_STRUCT_TYPE PARAM_STRUCT_VAR;
#endif

/* Global stream buffer pointers (declared extern in stream_audio.h) */
float *g_stream_in_l  = NULL;
float *g_stream_in_r  = NULL;
float *g_stream_out_l = NULL;
float *g_stream_out_r = NULL;
int    g_stream_offset = 0;

static int keep_running = 1;

static void sigint_handler(int sig) { (void)sig; keep_running = 0; }

#if PARAM_TUNING_ENABLED
typedef void (*param_setter_t)(float v);
typedef struct { const char *label; param_setter_t set; } param_desc_t;

#define PARAM_SLOTS_DEFINE_SETTERS 1
#include "param_slots_20.h"
#undef PARAM_SLOTS_DEFINE_SETTERS

static const param_desc_t g_params[] = {
#define PARAM_SLOTS_DEFINE_TABLE 1
#include "param_slots_20.h"
#undef PARAM_SLOTS_DEFINE_TABLE
  { NULL, NULL }
};
#endif

static void print_commands(void) {
#if PARAM_TUNING_ENABLED
  for (size_t i = 0; g_params[i].label != NULL; ++i)
    printf("  %s=<val>\n", g_params[i].label);
#endif
  printf("  q : Quit\n");
}

/* ------------------------------------------------------------------ */
/* ALSA helpers                                                         */
/* ------------------------------------------------------------------ */

static snd_pcm_t *g_pcm_in  = NULL;
static snd_pcm_t *g_pcm_out = NULL;

static int alsa_open(const char *device, snd_pcm_stream_t dir,
                     snd_pcm_t **pcm, unsigned int rate, int frames)
{
    int err;
    snd_pcm_hw_params_t *hw;

    err = snd_pcm_open(pcm, device, dir, 0);
    if (err < 0) {
        fprintf(stderr, "stream_main: cannot open ALSA device '%s' (%s): %s\n",
                device, dir == SND_PCM_STREAM_CAPTURE ? "capture" : "playback",
                snd_strerror(err));
        return err;
    }

    snd_pcm_hw_params_alloca(&hw);
    snd_pcm_hw_params_any(*pcm, hw);
    snd_pcm_hw_params_set_access(*pcm, hw, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(*pcm, hw, SND_PCM_FORMAT_FLOAT_LE);
    snd_pcm_hw_params_set_channels(*pcm, hw, 2);
    snd_pcm_hw_params_set_rate(*pcm, hw, rate, 0);
    snd_pcm_hw_params_set_period_size(*pcm, hw, frames, 0);

    err = snd_pcm_hw_params(*pcm, hw);
    if (err < 0) {
        fprintf(stderr, "stream_main: snd_pcm_hw_params failed: %s\n",
                snd_strerror(err));
        return err;
    }
    return 0;
}

/* ------------------------------------------------------------------ */
/* Main loop                                                            */
/* ------------------------------------------------------------------ */

int main(int argc, char **argv)
{
    (void)argc; (void)argv;

    const int   N    = MODEL_FRAME_LENGTH;
    const char *dev  = ALSA_DEVICE;
    unsigned int rate = 48000;

    /* Interleaved capture/playback buffers: [L0,R0, L1,R1, ...] */
    float *buf_in  = calloc(N * 2, sizeof(float));
    float *buf_out = calloc(N * 2, sizeof(float));

    /* Deinterleaved buffers exposed to stream_in / stream_out */
    float *in_l  = calloc(N, sizeof(float));
    float *in_r  = calloc(N, sizeof(float));
    float *out_l = calloc(N, sizeof(float));
    float *out_r = calloc(N, sizeof(float));

    if (!buf_in || !buf_out || !in_l || !in_r || !out_l || !out_r) {
        fprintf(stderr, "stream_main: out of memory\n");
        return 1;
    }

    /* Point globals at deinterleaved buffers */
    g_stream_in_l  = in_l;
    g_stream_in_r  = in_r;
    g_stream_out_l = out_l;
    g_stream_out_r = out_r;
    g_stream_offset = 0;

    if (alsa_open(dev, SND_PCM_STREAM_CAPTURE,  &g_pcm_in,  rate, N) < 0) return 1;
    if (alsa_open(dev, SND_PCM_STREAM_PLAYBACK, &g_pcm_out, rate, N) < 0) return 1;

    MODEL_INITIALIZE();
    printf("stream_main: model initialized, device='%s', N=%d, Fs=%u\n",
           dev, N, rate);

#ifdef ENABLE_CONTROL
    control_init();
#endif

    signal(SIGINT, sigint_handler);
    print_commands();

    while (keep_running) {

        /* --- Read from aes67-daemon (interleaved float stereo) --- */
        int err = snd_pcm_readi(g_pcm_in, buf_in, N);
        if (err == -EPIPE) {
            snd_pcm_prepare(g_pcm_in);
            continue;
        } else if (err < 0) {
            fprintf(stderr, "stream_main: read error: %s\n", snd_strerror(err));
            break;
        }

        /* Deinterleave */
        for (int i = 0; i < N; i++) {
            in_l[i] = buf_in[2*i];
            in_r[i] = buf_in[2*i + 1];
        }

        /* --- Process --- */
#ifdef ENABLE_CONTROL
        control_audio_tick();
#endif

        MODEL_STEP();

        /* Interleave output */
        for (int i = 0; i < N; i++) {
            buf_out[2*i]     = out_l[i];
            buf_out[2*i + 1] = out_r[i];
        }

        /* --- Write to aes67-daemon --- */
        err = snd_pcm_writei(g_pcm_out, buf_out, N);
        if (err == -EPIPE) {
            snd_pcm_prepare(g_pcm_out);
        } else if (err < 0) {
            fprintf(stderr, "stream_main: write error: %s\n", snd_strerror(err));
            break;
        }

        /* --- Non-blocking stdin for parameter tuning --- */
        {
            fd_set fds;
            struct timeval tv = {0, 0};
            FD_ZERO(&fds);
            FD_SET(STDIN_FILENO, &fds);
            if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0) {
                char line[64];
                if (fgets(line, sizeof(line), stdin)) {
                    if (line[0] == 'q') { keep_running = 0; continue; }
#if PARAM_TUNING_ENABLED
                    for (size_t i = 0; g_params[i].label != NULL; ++i) {
                        size_t L = strlen(g_params[i].label);
                        if (strncasecmp(line, g_params[i].label, L) == 0
                            && line[L] == '=') {
                            float val;
                            if (sscanf(line + L + 1, "%f", &val) == 1) {
                                g_params[i].set(val);
                                printf("%s = %.4f\n", g_params[i].label, val);
                            }
                            break;
                        }
                    }
#endif
                }
            }
        }
    }

#ifdef ENABLE_CONTROL
    control_shutdown();
#endif

    snd_pcm_close(g_pcm_in);
    snd_pcm_close(g_pcm_out);
    MODEL_TERMINATE();

    free(buf_in); free(buf_out);
    free(in_l); free(in_r);
    free(out_l); free(out_r);

    return 0;
}
