#include "osc_proto.h"
#include "udp_io.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>

static char buffer[256];
static int osc_ready = 0;

static int osc_pad(int n)
{
    return (n + 3) & ~3;
}

void osc_proto_init(const char *ip, int port)
{
    //printf("OSC INIT ip=%s port=%d\n", ip, port);
    fflush(stdout);
    udp_io_init(ip, port);
    osc_ready = 1;
	//udp_io_send("INIT\n", 5);
}

void osc_send_float(const char *addr, float v)
{
    int pos = 0;

    int L = (int)strlen(addr) + 1;
    memcpy(buffer + pos, addr, L);
    pos += osc_pad(L);

    memcpy(buffer + pos, ",f", 3);
    pos += osc_pad(3);

    union { float f; uint32_t i; } u;
    u.f = v;

    uint32_t be =
        ((u.i >> 24) & 0xff) |
        ((u.i >> 8)  & 0xff00) |
        ((u.i << 8)  & 0xff0000) |
        ((u.i << 24) & 0xff000000);

    memcpy(buffer + pos, &be, 4);
    pos += 4;

    udp_io_send(buffer, pos);
}

void osc_proto_send(const char *path, const double *data, int n)
{
    int len;

    if (!osc_ready) return;
    if (path == NULL) return;
    if (data == NULL) return;
    if (n <= 0) return;

    len = snprintf(buffer, sizeof(buffer), "%s=%.6f\n", path, data[0]);
    if (len <= 0) return;
    if (len >= (int)sizeof(buffer)) len = (int)sizeof(buffer) - 1;

    udp_io_send(buffer, len);
} 

int osc_proto_recv(double *data, int maxn)
{
    (void)data;
    (void)maxn;
    return 0;
}

void osc_server_start(int port)
{
    udp_io_bind(port);
}

void osc_server_stop(void)
{
    udp_io_close();
}

int osc_proto_recv_msg(char *path, size_t path_max, float *value)
{
    static uint8_t buf[256];
    int n = udp_io_recv(buf, (int)sizeof(buf) - 1);
    if (n <= 0) return -1;   /* no data */

    int pos = 0;

    /* 1. Address pattern — must start with '/' */
    if (buf[0] != '/') return -2;
    int addr_len = 0;
    while (pos + addr_len < n && buf[pos + addr_len] != '\0') addr_len++;
    if (pos + addr_len >= n) return -2;

    size_t copy = (size_t)addr_len < path_max - 1 ? (size_t)addr_len : path_max - 1;
    memcpy(path, buf + pos, copy);
    path[copy] = '\0';
    pos += osc_pad(addr_len + 1);

    /* 2. Type tag — must be ",f..." */
    if (pos + 4 > n) return -2;
    if (buf[pos] != ',') return -2;
    if (buf[pos + 1] != 'f') return -3;   /* only float supported */
    int tt_len = 0;
    while (pos + tt_len < n && buf[pos + tt_len] != '\0') tt_len++;
    pos += osc_pad(tt_len + 1);

    /* 3. Float argument (big-endian) */
    if (pos + 4 > n) return -2;
    uint32_t be;
    memcpy(&be, buf + pos, 4);
    uint32_t le = ((be >> 24) & 0x000000ffu) |
                  ((be >>  8) & 0x0000ff00u) |
                  ((be <<  8) & 0x00ff0000u) |
                  ((be << 24) & 0xff000000u);
    union { float f; uint32_t i; } u;
    u.i = le;
    *value = u.f;

    return 0;
}

void osc_proto_init4(uint8_t a, uint8_t b, uint8_t c, uint8_t d, int port)
{
    char ip[32];
    snprintf(ip, sizeof(ip), "%u.%u.%u.%u", a, b, c, d);
    osc_proto_init(ip, port);
}