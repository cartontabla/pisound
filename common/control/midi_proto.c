#include "midi_proto.h"
#include <string.h>
#include <stdio.h>

/*
 * midi_proto.c
 *
 * MIDI protocol parser.
 * Handles running status, multi-byte messages, and SysEx (basic).
 */

int midi_proto_init(midi_proto_ctx_t *ctx)
{
    if (!ctx) return -1;
    memset(ctx, 0, sizeof(*ctx));
    return 0;
}

int midi_proto_push_bytes(midi_proto_ctx_t *ctx, const uint8_t *data, size_t len)
{
    if (!ctx || !data) return -1;
    
    for (size_t i = 0; i < len; i++) {
        uint16_t next_head = (ctx->rx_head + 1) % (sizeof(ctx->rx_buffer));
        if (next_head == ctx->rx_tail) {
            /* Buffer full, drop byte */
            return -2;
        }
        
        ctx->rx_buffer[ctx->rx_head] = data[i];
        ctx->rx_head = next_head;
    }
    
    return 0;
}

static int _is_status_byte(uint8_t b)
{
    return (b & 0x80) != 0;
}

static int _is_data_byte(uint8_t b)
{
    return (b & 0x80) == 0;
}

int midi_proto_get_message(midi_proto_ctx_t *ctx, midi_msg_t *out)
{
    if (!ctx || !out) return -1;
    
    memset(out, 0, sizeof(*out));
    
    /* Need at least 1 byte to start */
    if (ctx->rx_head == ctx->rx_tail) {
        return -2; /* No data */
    }
    
    uint8_t b0 = ctx->rx_buffer[ctx->rx_tail];
    
    /* System Exclusive or Real-Time message - skip for now */
    if (b0 == 0xF0 || b0 >= 0xF8) {
        ctx->rx_tail = (ctx->rx_tail + 1) % sizeof(ctx->rx_buffer);
        return -3; /* Unsupported message */
    }
    
    uint8_t status;
    
    if (_is_status_byte(b0)) {
        /* New status byte */
        status = b0;
        ctx->running_status = status;
        ctx->rx_tail = (ctx->rx_tail + 1) % sizeof(ctx->rx_buffer);
    } else {
        /* Continuation (running status) */
        if (ctx->running_status == 0) {
            /* Error: data byte without status */
            ctx->rx_tail = (ctx->rx_tail + 1) % sizeof(ctx->rx_buffer);
            return -3;
        }
        status = ctx->running_status;
    }
    
    out->status = status;
    out->channel = status & 0x0F;
    
    uint8_t msg_type = status & 0xF0;
    
    /* Determine how many data bytes we need */
    int data_bytes = 0;
    if (msg_type == 0x80 || msg_type == 0x90 || msg_type == 0xA0 ||
        msg_type == 0xB0 || msg_type == 0xE0) {
        data_bytes = 2;
    } else if (msg_type == 0xC0 || msg_type == 0xD0) {
        data_bytes = 1;
    } else {
        return -3; /* Unknown message */
    }
    
    /* Check if we have enough data bytes */
    size_t available = 0;
    uint16_t pos = ctx->rx_tail;
    while (pos != ctx->rx_head && !_is_status_byte(ctx->rx_buffer[pos])) {
        available++;
        pos = (pos + 1) % sizeof(ctx->rx_buffer);
    }
    
    if (available < (size_t)data_bytes) {
        return -2; /* Need more data */
    }
    
    /* Extract data bytes */
    out->data1 = ctx->rx_buffer[ctx->rx_tail];
    ctx->rx_tail = (ctx->rx_tail + 1) % sizeof(ctx->rx_buffer);
    
    if (data_bytes >= 2) {
        out->data2 = ctx->rx_buffer[ctx->rx_tail];
        ctx->rx_tail = (ctx->rx_tail + 1) % sizeof(ctx->rx_buffer);
    }
    
    return 0; /* Message complete */
}

int midi_proto_to_event(const midi_msg_t *msg, ctrl_event_t *out)
{
    if (!msg || !out) return -1;
    
    memset(out, 0, sizeof(*out));
    
    uint8_t msg_type = msg->status & 0xF0;
    
    if (msg_type == 0xB0) {
        /* Control Change */
        out->param_id = msg->data1;  /* CC number as parameter ID */
        out->value = msg->data2 / 127.0f;  /* Normalize [0..127] to [0..1] */
        out->mode = CTRL_MODE_ABSOLUTE;
        return 0;
    } else if (msg_type == 0x90) {
        /* Note On */
        out->param_id = msg->data1 + 128;  /* Offset to avoid CC collision */
        out->value = msg->data2 / 127.0f;  /* Velocity */
        out->mode = CTRL_MODE_TOGGLE;
        return 0;
    } else if (msg_type == 0x80) {
        /* Note Off */
        out->param_id = msg->data1 + 128;
        out->value = 0.0f;
        out->mode = CTRL_MODE_TOGGLE;
        return 0;
    } else if (msg_type == 0xE0) {
        /* Pitch Bend */
        int bend = ((msg->data2 & 0x7F) << 7) | (msg->data1 & 0x7F);
        bend -= 0x2000;  /* Center at 0 */
        out->param_id = 256;  /* Special param ID for pitch bend */
        out->value = (bend / 8192.0f + 1.0f) / 2.0f;  /* Normalize to [0..1] */
        out->mode = CTRL_MODE_ABSOLUTE;
        return 0;
    }
    
    return -1; /* Unsupported message */
}

void midi_proto_print_message(const midi_msg_t *msg)
{
    if (!msg) return;
    
    uint8_t msg_type = msg->status & 0xF0;
    
    switch (msg_type) {
        case 0x80:
            printf("NOTE OFF   Ch%d Note=%d Vel=%d\n", msg->channel, msg->data1, msg->data2);
            break;
        case 0x90:
            printf("NOTE ON    Ch%d Note=%d Vel=%d\n", msg->channel, msg->data1, msg->data2);
            break;
        case 0xB0:
            printf("CC         Ch%d CC=%d Val=%d\n", msg->channel, msg->data1, msg->data2);
            break;
        case 0xE0:
            printf("PITCHBEND  Ch%d Bend=%d\n", msg->channel, 
                   ((msg->data2 & 0x7F) << 7) | (msg->data1 & 0x7F));
            break;
        default:
            printf("UNKNOWN    Status=0x%02x\n", msg->status);
    }
}
