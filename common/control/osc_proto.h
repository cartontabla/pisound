#ifndef OSC_PROTO_H
#define OSC_PROTO_H

#include <stdint.h>
#include <stddef.h>

void osc_proto_init(const char *ip, int port);
void osc_proto_init4(uint8_t a, uint8_t b, uint8_t c, uint8_t d, int port);
void osc_send_float(const char *addr, float v);
void osc_proto_send(const char *path, const double *data, int n);
int osc_proto_recv(double *data, int maxn);
void osc_server_start(int port);
void osc_server_stop(void);

/* Parse one incoming OSC packet.
 * Fills path and value from the first float argument.
 * Values are expected in [0,1] (normalized), same convention as MIDI.
 * Returns 0 on success, negative on error/no data. */
int osc_proto_recv_msg(char *path, size_t path_max, float *value);

#endif