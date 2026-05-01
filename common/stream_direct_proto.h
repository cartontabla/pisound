#ifndef STREAM_DIRECT_PROTO_H
#define STREAM_DIRECT_PROTO_H

#include <stdint.h>

#define STREAM_DIRECT_MAGIC 0x5354524dU

typedef struct {
  uint32_t magic;
  uint32_t frames;
  uint64_t generation;
  uint64_t bridge_rx_us;
  uint64_t app_rx_us;
  uint64_t app_tx_us;
} stream_direct_hdr_t;

#endif
