#ifndef UDP_IO_H
#define UDP_IO_H

void udp_io_init(const char *ip, int port);        /* TX: set first destination */
void udp_io_add_dest(const char *ip, int port);    /* TX: add extra destination */
void udp_io_send(const void *data, int len);       /* TX: send to all destinations */
void udp_io_close(void);

int  udp_io_bind(int port);                   /* RX: bind local port (non-blocking) */
int  udp_io_recv(void *buf, int maxlen);      /* RX: returns bytes read, -1 if none */

#endif