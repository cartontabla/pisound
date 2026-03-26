#include "udp_io.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>

static int udp_sock    = -1;   /* TX socket */
static int udp_rx_sock = -1;   /* RX socket */
static struct sockaddr_in udp_addr;
static int udp_ready = 0;

void udp_io_init(const char *ip, int port)
{
    if (udp_ready) {
        return;
    }

    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock < 0) {
        perror("socket");
        return;
    }

    memset(&udp_addr, 0, sizeof(udp_addr));
    udp_addr.sin_family = AF_INET;
    udp_addr.sin_port = htons(port);
    udp_addr.sin_addr.s_addr = inet_addr(ip);

    udp_ready = 1;

    printf("UDP INIT ip=%s port=%d sock=%d\n", ip, port, udp_sock);
}

void udp_io_send(const void *data, int len)
{
	//printf("UDP SEND len=%d\n", len);
	fflush(stdout);
	
    if (!udp_ready || udp_sock < 0) {
        return;
    }

    sendto(udp_sock, data, len, 0,
           (struct sockaddr *)&udp_addr, sizeof(udp_addr));
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