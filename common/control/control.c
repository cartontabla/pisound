/*
 * control.c - Control pipeline: MIDI RX → parse → map → ramp → ctrl_in
 *
 * Called each audio frame from custom_main.c via ENABLE_CONTROL hooks:
 *   control_process_rx()  — MIDI in  → g_ctrl_in_vals[]
 *   control_process_tx()  — g_ctrl_out_vals[] → MIDI/OSC out
 *
 * Compile-time configuration (set via -D flags in compile_flags.txt):
 *   CTRL_CONFIG_FILE    path to mapping config (e.g. "../common/control/ctrl_config_doubling.txt")
 *   CTRL_MIDI_PORT      ALSA port name or "client:port" (e.g. "Oxygen 25:0")
 *   CTRL_NUM_PARAMS     number of parameters (default 20)
 *   CTRL_SAMPLE_RATE    audio sample rate in Hz (default 48000)
 *   CTRL_FRAME_SIZE     audio frame size in samples (default 128)
 *   CTRL_TX_CONFIG_FILE path to OSC TX config file (default "ctrl_tx.conf")
 *                       Format: key=value, one per line
 *                         ip=192.168.0.31
 *                         port=9000
 */

#include "control.h"
#include "midi_io.h"
#include "midi_proto.h"
#include "ctrl_map.h"
#include "ctrl_apply.h"
#include "ctrl_event.h"
#include "ctrl_in.h"
#include "ctrl_out.h"

#ifdef ENABLE_OSC
#include "osc_proto.h"
#include "udp_io.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/* ---- Compile-time defaults ---------------------------------------- */

#ifndef CTRL_CONFIG_FILE
#define CTRL_CONFIG_FILE "ctrl_config.txt"
#endif

#ifndef CTRL_TX_CONFIG_FILE
#define CTRL_TX_CONFIG_FILE "ctrl_tx.conf"
#endif

#ifndef CTRL_MIDI_PORT
#define CTRL_MIDI_PORT ""
#endif

#ifndef CTRL_NUM_PARAMS
#define CTRL_NUM_PARAMS 20
#endif

#ifndef CTRL_OSC_TX_NUM
#define CTRL_OSC_TX_NUM 4
#endif

#ifndef CTRL_SAMPLE_RATE
#define CTRL_SAMPLE_RATE 48000
#endif

#ifndef CTRL_FRAME_SIZE
#define CTRL_FRAME_SIZE 128
#endif

#ifndef CONTROL_OSC_RX_IN_AUDIO_THREAD
#define CONTROL_OSC_RX_IN_AUDIO_THREAD 0
#endif

#ifndef CONTROL_OSC_TX_IN_AUDIO_THREAD
#define CONTROL_OSC_TX_IN_AUDIO_THREAD 0
#endif

static const char *g_tx_paths[CTRL_OSC_TX_NUM] = {
    "/doubling/out/delay_ms",
    "/doubling/out/depth",
    "/doubling/out/spread",
    "/doubling/out/bypass",
};

/* ---- OSC TX config (runtime) -------------------------------------- */

static int  g_osc_tx_port    = 9000;

/* ip entries accumulate; port applies to all */
static char  g_osc_tx_ips[8][64];
static int   g_osc_tx_nips = 0;

static void load_tx_config(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "control: no TX config '%s', using defaults\n", path);
        return;
    }
    char line[128];
    while (fgets(line, sizeof(line), f)) {
        char *eq = strchr(line, '=');
        if (!eq) continue;
        *eq = '\0';
        char *key = line;
        char *val = eq + 1;
        val[strcspn(val, "\r\n")] = '\0';
        if (strcmp(key, "ip") == 0 && g_osc_tx_nips < 8) {
            snprintf(g_osc_tx_ips[g_osc_tx_nips++], 64, "%s", val);
        } else if (strcmp(key, "port") == 0) {
            g_osc_tx_port = atoi(val);
        }
    }
    fclose(f);
}

/* ---- Module state -------------------------------------------------- */

static midi_io_ctx_t     *g_midi       = NULL;
static midi_proto_ctx_t   g_midi_proto;
static ctrl_map_table_t   g_ctrl_map;
static ctrl_apply_ctx_t   g_ctrl_apply;
static ctrl_event_queue_t g_ctrl_queue;
static pthread_mutex_t    g_ctrl_queue_mu = PTHREAD_MUTEX_INITIALIZER;
static pthread_t          g_ctrl_rx_thread;
static pthread_t          g_ctrl_tx_thread;
static int                g_ctrl_threads_running = 0;
static int                g_ctrl_rx_thread_started = 0;
static int                g_ctrl_tx_thread_started = 0;

static void control_poll_inputs_once(void);
static void control_send_outputs_once(void);
static void *control_rx_thread_main(void *arg);
static void *control_tx_thread_main(void *arg);

/* -------------------------------------------------------------------- */

void control_init(void)
{
    ctrl_event_queue_init(&g_ctrl_queue);
    midi_proto_init(&g_midi_proto);
    ctrl_map_init(&g_ctrl_map, 32);
    ctrl_apply_init(&g_ctrl_apply, CTRL_NUM_PARAMS, CTRL_SAMPLE_RATE, CTRL_FRAME_SIZE);

    if (ctrl_map_load(&g_ctrl_map, CTRL_CONFIG_FILE) != 0) {
        fprintf(stderr, "control: cannot load '%s'\n", CTRL_CONFIG_FILE);
    } else {
        printf("control: loaded %zu mappings from '%s'\n",
               g_ctrl_map.count, CTRL_CONFIG_FILE);

        /* Initialize each param to its min_val so ctrl_in outputs are valid
         * at startup (avoids MAP bounds = 0, mix = 0, etc.) */
        uint8_t inited[64] = {0};
        for (size_t i = 0; i < g_ctrl_map.count; i++) {
            uint32_t pid = g_ctrl_map.entries[i].param_id;
            if (pid < g_ctrl_apply.param_count && pid < 64 && !inited[pid]) {
                float init_val = g_ctrl_map.entries[i].min_val;
                ctrl_apply_set_immediate(&g_ctrl_apply, pid, init_val);
                ctrl_in_set(pid, init_val);
                inited[pid] = 1;
            }
        }
    }

    if (midi_io_init(&g_midi, CTRL_MIDI_PORT) != 0) {
        fprintf(stderr, "control: midi_io_init failed\n");
        g_midi = NULL;
    }

#ifdef ENABLE_OSC
    load_tx_config(CTRL_TX_CONFIG_FILE);
    if (g_osc_tx_nips > 0) {
        osc_proto_init(g_osc_tx_ips[0], g_osc_tx_port);
        for (int i = 1; i < g_osc_tx_nips; i++) {
            udp_io_add_dest(g_osc_tx_ips[i], g_osc_tx_port);
        }
    }
    osc_server_start(9000);
#endif

    g_ctrl_threads_running = 1;
    if (pthread_create(&g_ctrl_rx_thread, NULL, control_rx_thread_main, NULL) == 0) {
        g_ctrl_rx_thread_started = 1;
    } else {
        fprintf(stderr, "control: failed to start RX thread\n");
        g_ctrl_threads_running = 0;
    }

    if (g_ctrl_threads_running &&
        pthread_create(&g_ctrl_tx_thread, NULL, control_tx_thread_main, NULL) == 0) {
        g_ctrl_tx_thread_started = 1;
    } else if (g_ctrl_threads_running) {
        fprintf(stderr, "control: failed to start TX thread\n");
    }
}

/* -------------------------------------------------------------------- */

static void control_poll_inputs_once(void)
{
    /* 1. Drain MIDI bytes from ALSA into proto parser */
    if (g_midi) {
        uint8_t buf[64];
        int n;
        while ((n = midi_io_recv(g_midi, buf, sizeof(buf))) > 0) {
            midi_proto_push_bytes(&g_midi_proto, buf, (size_t)n);
        }

        /* 2. Parse MIDI messages → look up mapping → enqueue events */
        midi_msg_t msg;
        int r;
        while ((r = midi_proto_get_message(&g_midi_proto, &msg)) != -2) {
            if (r != 0) continue;
            uint8_t msg_type = msg.status & 0xF0;
            if (msg_type == 0xB0) {
                ctrl_mapping_t *mapping = NULL;
                if (ctrl_map_by_midi_cc(&g_ctrl_map, msg.data1, &mapping) == 0) {
                    ctrl_event_t evt;
                    midi_proto_to_event(&msg, &evt);
                    evt.param_id = mapping->param_id;
                    pthread_mutex_lock(&g_ctrl_queue_mu);
                    ctrl_event_enqueue(&g_ctrl_queue, &evt);
                    pthread_mutex_unlock(&g_ctrl_queue_mu);
                }
            }
        }
    }

#if defined(ENABLE_OSC) && CONTROL_OSC_RX_IN_AUDIO_THREAD
    /* 2b. Drain incoming OSC packets → look up mapping → enqueue events */
    {
        char osc_path[64];
        float osc_val;
        while (osc_proto_recv_msg(osc_path, sizeof(osc_path), &osc_val) == 0) {
            ctrl_mapping_t *mapping = NULL;
            if (ctrl_map_by_osc_path(&g_ctrl_map, osc_path, &mapping) == 0) {
                ctrl_event_t evt;
                evt.param_id = mapping->param_id;
                evt.value    = osc_val;   /* sender uses [0,1] normalized */
                evt.mode     = CTRL_MODE_ABSOLUTE;
                pthread_mutex_lock(&g_ctrl_queue_mu);
                ctrl_event_enqueue(&g_ctrl_queue, &evt);
                pthread_mutex_unlock(&g_ctrl_queue_mu);
            }
        }
    }
#endif
}

void control_audio_tick(void)
{
    /* 3. Apply queued events to ctrl_apply (sets targets + ramp params) */
    ctrl_event_t evt;
    for (;;) {
        int has_evt;
        pthread_mutex_lock(&g_ctrl_queue_mu);
        has_evt = (ctrl_event_dequeue(&g_ctrl_queue, &evt) == 0);
        pthread_mutex_unlock(&g_ctrl_queue_mu);
        if (!has_evt) {
            break;
        }

        if (evt.param_id < (uint32_t)g_ctrl_map.count) {
            ctrl_apply_event(&g_ctrl_apply, &evt,
                             &g_ctrl_map.entries[evt.param_id]);
        }
    }

    /* 4. Advance ramps by one frame */
    ctrl_apply_process_frame(&g_ctrl_apply);

    /* 5. Write smooth values → g_ctrl_in_vals[] (read by ctrl_in S-Function) */
    for (unsigned int i = 0; i < (unsigned int)g_ctrl_map.count
                             && i < CTRL_IN_MAX_PARAMS; i++) {
        ctrl_in_set(i, ctrl_apply_get(&g_ctrl_apply, i));
    }
}

/* -------------------------------------------------------------------- */

static void control_send_outputs_once(void)
{
#ifdef ENABLE_OSC
    /* Throttle to ~30 Hz (128/48000 ≈ 2.67ms per frame, every 12 frames ≈ 32ms) */
    static int s_frame = 0;
    if (++s_frame < 12) return;
    s_frame = 0;

    for (int i = 0; i < CTRL_OSC_TX_NUM; i++) {
        osc_send_float(g_tx_paths[i], ctrl_out_get((unsigned int)i));
    }
#endif
}

static void *control_rx_thread_main(void *arg)
{
    (void)arg;

    while (g_ctrl_threads_running) {
        control_poll_inputs_once();
        usleep(1000);
    }

    return NULL;
}

static void *control_tx_thread_main(void *arg)
{
    (void)arg;

    while (g_ctrl_threads_running) {
        control_send_outputs_once();
        usleep(2667);
    }

    return NULL;
}

/* -------------------------------------------------------------------- */

void control_set_param(uint32_t param_id, float val)
{
    ctrl_apply_set_immediate(&g_ctrl_apply, param_id, val);
    ctrl_in_set(param_id, val);
}

/* -------------------------------------------------------------------- */

void control_shutdown(void)
{
    g_ctrl_threads_running = 0;

    if (g_ctrl_rx_thread_started) {
        pthread_join(g_ctrl_rx_thread, NULL);
        g_ctrl_rx_thread_started = 0;
    }
    if (g_ctrl_tx_thread_started) {
        pthread_join(g_ctrl_tx_thread, NULL);
        g_ctrl_tx_thread_started = 0;
    }

#ifdef ENABLE_OSC
    osc_server_stop();
#endif

    if (g_midi) {
        midi_io_close(g_midi);
        g_midi = NULL;
    }

    ctrl_apply_free(&g_ctrl_apply);
    ctrl_map_free(&g_ctrl_map);
}
