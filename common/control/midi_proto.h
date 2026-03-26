#ifndef MIDI_PROTO_H
#define MIDI_PROTO_H

#include "ctrl_event.h"
#include <stdint.h>
#include <stddef.h>

/*
 * midi_proto.h
 *
 * MIDI protocol parsing.
 * Converts raw MIDI bytes to unified control events.
 * 
 * Supported messages:
 * - Control Change (CC): param change
 * - Note On/Off: toggle/scene
 * - Pitch Bend: hi-res param change
 */

typedef struct {
    uint8_t status;     /* MIDI status byte (channel + message type) */
    uint8_t channel;    /* MIDI channel (0..15) */
    uint8_t data1;      /* First data byte (CC number, note, etc.) */
    uint8_t data2;      /* Second data byte (value, velocity, etc.) */
} midi_msg_t;

typedef struct {
    uint8_t  rx_buffer[1024];
    uint16_t rx_head;
    uint16_t rx_tail;
    uint8_t  running_status;
} midi_proto_ctx_t;

/* Initialize MIDI protocol parser */
int midi_proto_init(midi_proto_ctx_t *ctx);

/* Push raw MIDI bytes into the parser */
int midi_proto_push_bytes(midi_proto_ctx_t *ctx, const uint8_t *data, size_t len);

/* Extract next complete MIDI message */
int midi_proto_get_message(midi_proto_ctx_t *ctx, midi_msg_t *out);

/* Convert MIDI message to control event */
int midi_proto_to_event(const midi_msg_t *msg, ctrl_event_t *out);

/* Debug: print MIDI message */
void midi_proto_print_message(const midi_msg_t *msg);

#endif /* MIDI_PROTO_H */
