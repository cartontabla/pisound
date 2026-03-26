#ifndef MIDI_IO_H
#define MIDI_IO_H

#include <stdint.h>
#include <stddef.h>

/*
 * midi_io.h
 *
 * Platform-agnostic MIDI I/O interface.
 * Abstracts DIN/USB/BLE transport to simple send/recv functions.
 * 
 * Platform implementations: midi_io_posix.c, midi_io_win32.c, etc.
 */

typedef struct midi_io_ctx midi_io_ctx_t;

/* Initialize MIDI I/O subsystem */
int midi_io_init(midi_io_ctx_t **ctx, const char *port_name);

/* Close and cleanup */
void midi_io_close(midi_io_ctx_t *ctx);

/* Send raw MIDI bytes */
int midi_io_send(midi_io_ctx_t *ctx, const uint8_t *data, size_t len);

/* Receive raw MIDI bytes (non-blocking) */
int midi_io_recv(midi_io_ctx_t *ctx, uint8_t *data, size_t maxlen);

/* List available MIDI ports */
int midi_io_list_ports(char **port_names, size_t max_ports);

#endif /* MIDI_IO_H */
