#include "midi_io.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#ifdef __linux__
#include <alsa/asoundlib.h>
#endif

/*
 * midi_io_posix.c (Linux/Pi with ALSA)
 *
 * Platform-specific MIDI I/O implementation.
 * Uses ALSA sequencer on Linux for DIN/USB/BLE MIDI.
 */

struct midi_io_ctx {
#ifdef __linux__
    snd_seq_t *seq;
    int my_port;
    int remote_port;
    int remote_client;
#else
    int fd;
#endif
    char port_name[256];
};

#ifdef __linux__

static int _alsa_init(midi_io_ctx_t *ctx, const char *port_name)
{
    int err;
    
    /* Open ALSA sequencer */
    err = snd_seq_open(&ctx->seq, "default", SND_SEQ_OPEN_DUPLEX, 0);
    if (err < 0) {
        fprintf(stderr, "midi_io: Cannot open sequencer: %s\n", snd_strerror(err));
        return -1;
    }
    
    /* Set client name */
    snd_seq_set_client_name(ctx->seq, "pisound-control");
    
    /* Create input port */
    ctx->my_port = snd_seq_create_simple_port(ctx->seq, "in",
                                              SND_SEQ_PORT_CAP_WRITE |
                                              SND_SEQ_PORT_CAP_SUBS_WRITE,
                                              SND_SEQ_PORT_TYPE_APPLICATION);
    if (ctx->my_port < 0) {
        fprintf(stderr, "midi_io: Cannot create port\n");
        snd_seq_close(ctx->seq);
        return -1;
    }
    
    /* If port_name specified, try to connect to it.
     * snd_seq_parse_address() accepts both "client:port" numeric and
     * named forms (e.g. "Oxygen 25:0"), so no manual parsing needed. */
    if (port_name && strlen(port_name) > 0) {
        snd_seq_addr_t addr;
        err = snd_seq_parse_address(ctx->seq, &addr, port_name);
        if (err < 0) {
            fprintf(stderr, "midi_io: Cannot parse address '%s': %s\n",
                    port_name, snd_strerror(err));
            /* Non-fatal: port created, connect manually with aconnect */
        } else {
            err = snd_seq_connect_from(ctx->seq, ctx->my_port,
                                       addr.client, addr.port);
            if (err < 0) {
                fprintf(stderr, "midi_io: Cannot connect to '%s': %s\n",
                        port_name, snd_strerror(err));
            } else {
                printf("midi_io: Connected to '%s' (%d:%d)\n",
                       port_name, addr.client, addr.port);
            }
        }
    }

    /* Set non-blocking mode (correct ALSA API) */
    snd_seq_nonblock(ctx->seq, 1);
    
    strncpy(ctx->port_name, port_name ? port_name : "default", sizeof(ctx->port_name)-1);
    
    return 0;
}

int midi_io_init(midi_io_ctx_t **ctx_out, const char *port_name)
{
    if (!ctx_out) return -1;
    
    midi_io_ctx_t *ctx = (midi_io_ctx_t *)malloc(sizeof(midi_io_ctx_t));
    if (!ctx) return -1;
    
    memset(ctx, 0, sizeof(*ctx));
    
    if (_alsa_init(ctx, port_name) != 0) {
        free(ctx);
        return -1;
    }
    
    *ctx_out = ctx;
    return 0;
}

void midi_io_close(midi_io_ctx_t *ctx)
{
    if (!ctx) return;
    
#ifdef __linux__
    if (ctx->seq) {
        snd_seq_close(ctx->seq);
    }
#endif
    
    free(ctx);
}

int midi_io_send(midi_io_ctx_t *ctx, const uint8_t *data, size_t len)
{
    if (!ctx || !data || len == 0) return -1;
    
#ifdef __linux__
    snd_seq_event_t ev;
    snd_seq_ev_clear(&ev);
    snd_seq_ev_set_source(&ev, ctx->my_port);
    snd_seq_ev_set_subs(&ev);
    snd_seq_ev_set_direct(&ev);
    
    /* For simplicity, send as SysEx */
    snd_seq_ev_set_sysex(&ev, len, (void *)data);
    
    return snd_seq_event_output_direct(ctx->seq, &ev);
#else
    /* Stub for non-Linux */
    return (int)len;
#endif
}

int midi_io_recv(midi_io_ctx_t *ctx, uint8_t *data, size_t maxlen)
{
    if (!ctx || !data || maxlen == 0) return -1;
    
#ifdef __linux__
    snd_seq_event_t *ev = NULL;
    int n = snd_seq_event_input(ctx->seq, &ev);
    
    if (n == -ENOSPC || n == 0) {
        return -2; /* No data */
    }
    if (n < 0) {
        return -1; /* Error */
    }
    
    if (!ev) return -2;
    
    /* Extract MIDI bytes from event */
    size_t copy_len = 0;
    
    if (ev->type == SND_SEQ_EVENT_NOTEON ||
        ev->type == SND_SEQ_EVENT_NOTEOFF) {
        /* Note event: convert to 3 MIDI bytes */
        uint8_t status = (ev->type == SND_SEQ_EVENT_NOTEON) ? 0x90 : 0x80;
        status |= (ev->data.note.channel & 0x0F);
        
        if (copy_len + 3 <= maxlen) {
            data[copy_len++] = status;
            data[copy_len++] = ev->data.note.note & 0x7F;
            data[copy_len++] = ev->data.note.velocity & 0x7F;
        }
    } else if (ev->type == SND_SEQ_EVENT_CONTROLLER) {
        /* CC event */
        uint8_t status = 0xB0 | (ev->data.control.channel & 0x0F);
        
        if (copy_len + 3 <= maxlen) {
            data[copy_len++] = status;
            data[copy_len++] = ev->data.control.param & 0x7F;
            data[copy_len++] = ev->data.control.value & 0x7F;
        }
    } else if (ev->type == SND_SEQ_EVENT_PITCHBEND) {
        /* Pitch bend */
        uint8_t status = 0xE0 | (ev->data.control.channel & 0x0F);
        int bend = ev->data.control.value;  /* -8192..8191 */
        
        if (copy_len + 3 <= maxlen) {
            data[copy_len++] = status;
            data[copy_len++] = bend & 0x7F;
            data[copy_len++] = (bend >> 7) & 0x7F;
        }
    }
    
    return (int)copy_len;
#else
    return -1; /* Not implemented */
#endif
}

int midi_io_list_ports(char **port_names, size_t max_ports)
{
#ifdef __linux__
    snd_seq_t *seq;
    int err = snd_seq_open(&seq, "default", SND_SEQ_OPEN_DUPLEX, 0);
    if (err < 0) return -1;
    
    snd_seq_client_info_t *cinfo;
    snd_seq_port_info_t *pinfo;
    int count = 0;
    
    snd_seq_client_info_alloca(&cinfo);
    snd_seq_port_info_alloca(&pinfo);
    
    snd_seq_client_info_set_client(cinfo, -1);
    
    while (snd_seq_query_next_client(seq, cinfo) >= 0) {
        int client = snd_seq_client_info_get_client(cinfo);
        const char *cname = snd_seq_client_info_get_name(cinfo);
        
        snd_seq_port_info_set_client(pinfo, client);
        snd_seq_port_info_set_port(pinfo, -1);
        
        while (snd_seq_query_next_port(seq, pinfo) >= 0) {
            int port = snd_seq_port_info_get_port(pinfo);
            const char *pname = snd_seq_port_info_get_name(pinfo);
            
            if (count < (int)max_ports && port_names) {
                size_t len = strlen(cname) + strlen(pname) + 10;
                port_names[count] = (char *)malloc(len);
                if (port_names[count]) {
                    snprintf(port_names[count], len, "%d:%d (%s:%s)",
                            client, port, cname, pname);
                    count++;
                }
            }
        }
    }
    
    snd_seq_close(seq);
    return count;
#else
    return 0;
#endif
}

#else /* Non-Linux stub */

int midi_io_init(midi_io_ctx_t **ctx_out, const char *port_name)
{
    (void)port_name;
    midi_io_ctx_t *ctx = (midi_io_ctx_t *)malloc(sizeof(midi_io_ctx_t));
    *ctx_out = ctx;
    return 0;
}

void midi_io_close(midi_io_ctx_t *ctx)
{
    free(ctx);
}

int midi_io_send(midi_io_ctx_t *ctx, const uint8_t *data, size_t len)
{
    (void)ctx;
    (void)data;
    return (int)len;
}

int midi_io_recv(midi_io_ctx_t *ctx, uint8_t *data, size_t maxlen)
{
    (void)ctx;
    (void)data;
    (void)maxlen;
    return -2;
}

int midi_io_list_ports(char **port_names, size_t max_ports)
{
    (void)port_names;
    (void)max_ports;
    return 0;
}

#endif
