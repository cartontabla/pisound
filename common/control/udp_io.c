#include "udp_io.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define UDP_MAX_DESTS 8

static int udp_sock                          = -1;
static int udp_rx_sock                       = -1;
static struct sockaddr_in udp_dests[UDP_MAX_DESTS];
static int udp_ndests                        = 0;
static int udp_ready                         = 0;

static void _add_dest(const char *ip, int port)
{
    if (udp_ndests >= UDP_MAX_DESTS) {
        fprintf(stderr, "udp_io: max destinations (%d) reached\n", UDP_MAX_DESTS);
        return;
    }
    struct sockaddr_in *a = &udp_dests[udp_ndests++];
    memset(a, 0, sizeof(*a));
    a->sin_family      = AF_INET;
    a->sin_port        = htons((uint16_t)port);
    a->sin_addr.s_addr = inet_addr(ip);
    printf("UDP dest[%d] ip=%s port=%d\n", udp_ndests - 1, ip, port);
}

void udp_io_init(const char *ip, int port)
{
    if (udp_ready) return;

    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock < 0) { perror("socket"); return; }

    udp_ready = 1;
    printf("UDP INIT ip=%s port=%d sock=%d\n", ip, port, udp_sock);
    _add_dest(ip, port);
}

void udp_io_add_dest(const char *ip, int port)
{
    if (!udp_ready) {
        fprintf(stderr, "udp_io: call udp_io_init before udp_io_add_dest\n");
        return;
    }
    _add_dest(ip, port);
}

void udp_io_send(const void *data, int len)
{
    if (!udp_ready || udp_sock < 0) return;

    for (int i = 0; i < udp_ndests; i++) {
        sendto(udp_sock, data, len, 0,
               (struct sockaddr *)&udp_dests[i], sizeof(udp_dests[i]));
    }
}

void udp_io_close(void)
{
    if (udp_sock >= 0) {
        close(udp_sock);
        udp_sock = -1;
    }
    if (udp_rx_sock >= 0) {
        close(udp_rx_sock);
        udp_rx_sock = -1;
    }
    udp_ready = 0;
}

int udp_io_bind(int port)
{
    if (udp_rx_sock >= 0) return 0;   /* already bound */

    udp_rx_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_rx_sock < 0) { perror("udp_io_bind socket"); return -1; }

    /* non-blocking */
    int flags = fcntl(udp_rx_sock, F_GETFL, 0);
    fcntl(udp_rx_sock, F_SETFL, flags | O_NONBLOCK);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons((uint16_t)port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(udp_rx_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("udp_io_bind bind");
        close(udp_rx_sock);
        udp_rx_sock = -1;
        return -1;
    }

    printf("udp_io: listening on UDP port %d\n", port);
    return 0;
}

int udp_io_recv(void *buf, int maxlen)
{
    if (udp_rx_sock < 0) return -1;
    ssize_t n = recv(udp_rx_sock, buf, (size_t)maxlen, 0);
    return (int)n;   /* -1 = EAGAIN (no data), >0 = bytes */
}