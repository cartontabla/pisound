/*
 * control.c - Control pipeline: MIDI RX → parse → map → ramp → ctrl_in
 *
 * Called each audio frame from custom_main.c via ENABLE_CONTROL hooks:
 *   control_process_rx()  — MIDI in  → g_ctrl_in_vals[]
 *   control_process_tx()  — g_ctrl_out_vals[] → MIDI/OSC out (future)
 *
 * Compile-time configuration (set via -D flags in compile_flags.txt):
 *   CTRL_CONFIG_FILE   path to mapping config (e.g. "../common/control/ctrl_config_doubling.txt")
 *   CTRL_MIDI_PORT     ALSA port name or "client:port" (e.g. "Oxygen 25:0")
 *                      Empty string "" → port created but not auto-connected;
 *                      connect manually: aconnect <oxygen_port> pisound-control:in
 *   CTRL_NUM_PARAMS    number of parameters (default 20)
 *   CTRL_SAMPLE_RATE   audio sample rate in Hz (default 48000)
 *   CTRL_FRAME_SIZE    audio frame size in samples (default 128)
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
#endif

#include <stdio.h>

/* ---- Compile-time defaults ---------------------------------------- */

#ifndef CTRL_CONFIG_FILE
#define CTRL_CONFIG_FILE "ctrl_config.txt"
#endif

#ifndef CTRL_MIDI_PORT
#define CTRL_MIDI_PORT ""
#endif

#ifndef CTRL_NUM_PARAMS
#define CTRL_NUM_PARAMS 20
#endif

#ifndef CTRL_SAMPLE_RATE
#define CTRL_SAMPLE_RATE 48000
#endif

#ifndef CTRL_FRAME_SIZE
#define CTRL_FRAME_SIZE 128
#endif

/* ---- Module state -------------------------------------------------- */

static midi_io_ctx_t     *g_midi       = NULL;
static midi_proto_ctx_t   g_midi_proto;
static ctrl_map_table_t   g_ctrl_map;
static ctrl_apply_ctx_t   g_ctrl_apply;
static ctrl_event_queue_t g_ctrl_queue;

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
    }

    if (midi_io_init(&g_midi, CTRL_MIDI_PORT) != 0) {
        fprintf(stderr, "control: midi_io_init failed\n");
        g_midi = NULL;
    }

#ifdef ENABLE_OSC
    osc_server_start(9000);
#endif
}

/* -------------------------------------------------------------------- */

void control_process_rx(void)
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
                    ctrl_event_enqueue(&g_ctrl_queue, &evt);
                }
            }
        }
    }

#ifdef ENABLE_OSC
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
                ctrl_event_enqueue(&g_ctrl_queue, &evt);
            }
        }
    }
#endif

    /* 3. Apply queued events to ctrl_apply (sets targets + ramp params) */
    ctrl_event_t evt;
    while (ctrl_event_dequeue(&g_ctrl_queue, &evt) == 0) {
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

void control_process_tx(void)
{
    /* TX: read g_ctrl_out_vals[] and send back via MIDI/OSC — future work */
    (void)ctrl_out_get;
}

/* -------------------------------------------------------------------- */

void control_shutdown(void)
{
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
