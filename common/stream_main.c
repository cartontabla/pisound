/*
 * stream_main.c: Execution harness for stream_in/stream_out models.
 *
 * This variant now behaves more like the traditional pisound audio path:
 * RX, process, and TX are decoupled with a tiny double buffer so input and
 * output can run independently while the model still works block-by-block.
 */

#include <alsa/asoundlib.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <sys/resource.h>
#include <unistd.h>

#include "stream_audio.h"
#include "stream_direct_proto.h"
#include "stream/stream_backend.h"
#include "stream/stream_contract.h"

#ifdef ENABLE_CONTROL
#include "control/control.h"
#endif

#ifndef MODEL_FRAME_LENGTH
#define MODEL_FRAME_LENGTH 128
#endif

#ifndef ALSA_DEVICE
#define ALSA_DEVICE "aes67"
#endif

#ifndef MODEL_HEADER
#error "MODEL_HEADER not defined"
#endif
#include MODEL_HEADER

#ifndef PARAM_STRUCT_VAR
#define PARAM_TUNING_ENABLED 0
#else
#define PARAM_TUNING_ENABLED 1
extern PARAM_STRUCT_TYPE PARAM_STRUCT_VAR;
#endif

#define STREAM_BLOCK_SAMPLES (MODEL_FRAME_LENGTH * 2)
#define STREAM_SLOT_COUNT 2

float *g_stream_capture_l  = NULL;
float *g_stream_capture_r  = NULL;
float *g_stream_playback_l = NULL;
float *g_stream_playback_r = NULL;
int    g_stream_offset = 0;

static volatile sig_atomic_t keep_running = 1;

static void sigint_handler(int sig) { (void)sig; keep_running = 0; }

#if PARAM_TUNING_ENABLED
typedef void (*param_setter_t)(float v);
typedef struct { const char *label; param_setter_t set; } param_desc_t;

#define PARAM_SLOTS_DEFINE_SETTERS 1
#include "param_slots_20.h"
#undef PARAM_SLOTS_DEFINE_SETTERS

static const param_desc_t g_params[] = {
#define PARAM_SLOTS_DEFINE_TABLE 1
#include "param_slots_20.h"
#undef PARAM_SLOTS_DEFINE_TABLE
  { NULL, NULL }
};
#endif

static void print_commands(void)
{
#if PARAM_TUNING_ENABLED
  for (size_t i = 0; g_params[i].label != NULL; ++i) {
    printf("  %s=<val>\n", g_params[i].label);
  }
#endif
  printf("  q : Quit\n");
}

typedef struct {
  stream_direct_hdr_t hdr;
  float in_interleaved[STREAM_BLOCK_SAMPLES];
  float in_l[MODEL_FRAME_LENGTH];
  float in_r[MODEL_FRAME_LENGTH];
  int state;
  unsigned long generation;
} stream_rx_slot_t;

typedef struct {
  stream_direct_hdr_t hdr;
  float out_interleaved[STREAM_BLOCK_SAMPLES];
  float out_l[MODEL_FRAME_LENGTH];
  float out_r[MODEL_FRAME_LENGTH];
  int state;
  unsigned long generation;
} stream_tx_slot_t;

typedef struct {
  stream_backend_t backend;
  pthread_mutex_t lock;
  pthread_cond_t cond_rx;
  pthread_cond_t cond_tx;
  stream_rx_slot_t rx_slots[STREAM_SLOT_COUNT];
  stream_tx_slot_t tx_slots[STREAM_SLOT_COUNT];
  unsigned long rx_generation;
  unsigned long tx_generation;
  uint64_t rx_udp_packets;
  uint64_t tx_udp_packets;
  uint64_t last_rx_udp_generation;
  uint64_t last_tx_udp_generation;
  uint64_t timing_log_counter;
} stream_ctx_t;

enum {
  STREAM_SLOT_EMPTY = 0,
  STREAM_SLOT_READY = 1,
  STREAM_SLOT_IN_USE = 2,
};

#define STREAM_COND_WAIT_MS 100

static stream_ctx_t g_ctx;

typedef enum {
  STREAM_THREAD_PRIO_RX = 0,
  STREAM_THREAD_PRIO_MODEL = 1,
  STREAM_THREAD_PRIO_TX = 2,
} stream_thread_prio_kind_t;

static int getenv_int(const char *name, int fallback)
{
  const char *value = getenv(name);
  if (!value || value[0] == '\0') {
    return fallback;
  }
  return atoi(value);
}

static int getenv_flag(const char *name)
{
  const char *value = getenv(name);
  if (!value || value[0] == '\0') {
    return 0;
  }
  return !(strcmp(value, "0") == 0 ||
           strcasecmp(value, "false") == 0 ||
           strcasecmp(value, "no") == 0 ||
           strcasecmp(value, "off") == 0);
}

static int stream_main_verbose(void)
{
  return getenv_flag("STREAM_VERBOSE");
}

static float block_peak_abs(const float *samples, size_t count)
{
  float peak = 0.0f;
  size_t i;

  if (!samples) {
    return 0.0f;
  }

  for (i = 0; i < count; ++i) {
    float sample = samples[i];
    if (sample < 0.0f) {
      sample = -sample;
    }
    if (sample > peak) {
      peak = sample;
    }
  }

  return peak;
}

static int lock_process_memory(void)
{
  if (mlockall(MCL_CURRENT | MCL_FUTURE) != 0) {
    fprintf(stderr, "stream_main: mlockall failed: %s\n", strerror(errno));
    return -1;
  }
  return 0;
}

static int realtime_priority_for(stream_thread_prio_kind_t kind)
{
  int max_prio = sched_get_priority_max(SCHED_FIFO);
  int min_prio = sched_get_priority_min(SCHED_FIFO);
  struct rlimit rl;
  int limit_prio = max_prio;
  int fallback;
  int prio;
  const char *env_name;

  if (max_prio < 0 || min_prio < 0) {
    return -1;
  }
  if (getrlimit(RLIMIT_RTPRIO, &rl) == 0) {
    if (rl.rlim_cur != RLIM_INFINITY && (int)rl.rlim_cur < limit_prio) {
      limit_prio = (int)rl.rlim_cur;
    }
  }

  switch (kind) {
    case STREAM_THREAD_PRIO_RX:
      env_name = "STREAM_RT_PRIO_RX";
      fallback = limit_prio - 2;
      break;
    case STREAM_THREAD_PRIO_MODEL:
      env_name = "STREAM_RT_PRIO_MODEL";
      fallback = limit_prio - 1;
      break;
    case STREAM_THREAD_PRIO_TX:
    default:
      env_name = "STREAM_RT_PRIO_TX";
      fallback = limit_prio - 3;
      break;
  }

  if (fallback < min_prio) {
    fallback = min_prio;
  }
  prio = getenv_int(env_name, fallback);
  if (prio < min_prio) {
    prio = min_prio;
  } else if (prio > limit_prio) {
    prio = limit_prio;
  }
  return prio;
}

static void request_realtime_thread(const char *name, stream_thread_prio_kind_t kind)
{
  struct sched_param param;
  int rc;
  int prio = realtime_priority_for(kind);

  if (prio < 0) {
    fprintf(stderr, "stream_main: cannot query RT priority for %s thread\n", name);
    return;
  }

  memset(&param, 0, sizeof(param));
  param.sched_priority = prio;
  rc = pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
  if (rc != 0) {
    fprintf(stderr,
            "stream_main: unable to enable SCHED_FIFO for %s thread at prio %d: %s\n",
            name, prio, strerror(rc));
    return;
  }

  if (stream_main_verbose()) {
    fprintf(stderr, "stream_main: %s thread running SCHED_FIFO prio=%d\n", name, prio);
  }
}

static uint64_t monotonic_us(void)
{
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint64_t)ts.tv_sec * 1000000ULL + (uint64_t)ts.tv_nsec / 1000ULL;
}

static int stream_read_frame(stream_ctx_t *ctx, stream_rx_slot_t *slot)
{
  return stream_backend_read_frame(&ctx->backend,
                                   &slot->hdr,
                                   slot->in_interleaved,
                                   STREAM_BLOCK_SAMPLES);
}

static int stream_write_frame(stream_ctx_t *ctx, stream_tx_slot_t *slot)
{
  return stream_backend_write_frame(&ctx->backend,
                                    &slot->hdr,
                                    slot->out_interleaved,
                                    STREAM_BLOCK_SAMPLES);
}

static void deinterleave_slot(stream_rx_slot_t *slot)
{
  int i;
  for (i = 0; i < MODEL_FRAME_LENGTH; i++) {
    slot->in_l[i] = slot->in_interleaved[2 * i];
    slot->in_r[i] = slot->in_interleaved[2 * i + 1];
  }
}

static void interleave_slot(stream_tx_slot_t *slot)
{
  int i;
  for (i = 0; i < MODEL_FRAME_LENGTH; i++) {
    slot->out_interleaved[2 * i] = slot->out_l[i];
    slot->out_interleaved[2 * i + 1] = slot->out_r[i];
  }
}

static stream_rx_slot_t *find_slot_for_rx(stream_ctx_t *ctx)
{
  int i;
  for (i = 0; i < STREAM_SLOT_COUNT; i++) {
    if (ctx->rx_slots[i].state == STREAM_SLOT_EMPTY) {
      return &ctx->rx_slots[i];
    }
  }
  return NULL;
}

static stream_rx_slot_t *find_oldest_input_ready(stream_ctx_t *ctx)
{
  stream_rx_slot_t *best = NULL;
  int i;
  for (i = 0; i < STREAM_SLOT_COUNT; i++) {
    if (ctx->rx_slots[i].state != STREAM_SLOT_READY) {
      continue;
    }
    if (!best || ctx->rx_slots[i].generation < best->generation) {
      best = &ctx->rx_slots[i];
    }
  }
  return best;
}

static stream_tx_slot_t *find_slot_for_tx_write(stream_ctx_t *ctx)
{
  int i;
  for (i = 0; i < STREAM_SLOT_COUNT; i++) {
    if (ctx->tx_slots[i].state == STREAM_SLOT_EMPTY) {
      return &ctx->tx_slots[i];
    }
  }
  return NULL;
}

static stream_tx_slot_t *find_oldest_output_ready(stream_ctx_t *ctx)
{
  stream_tx_slot_t *best = NULL;
  int i;
  for (i = 0; i < STREAM_SLOT_COUNT; i++) {
    if (ctx->tx_slots[i].state != STREAM_SLOT_READY) {
      continue;
    }
    if (!best || ctx->tx_slots[i].generation < best->generation) {
      best = &ctx->tx_slots[i];
    }
  }
  return best;
}

static void *rx_thread_main(void *arg)
{
  stream_ctx_t *ctx = (stream_ctx_t *)arg;
  request_realtime_thread("rx", STREAM_THREAD_PRIO_RX);

  while (keep_running) {
    stream_rx_slot_t *slot;
    int rc;

    pthread_mutex_lock(&ctx->lock);
    while (keep_running && (slot = find_slot_for_rx(ctx)) == NULL) {
      struct timespec ts;
      clock_gettime(CLOCK_REALTIME, &ts);
      ts.tv_nsec += STREAM_COND_WAIT_MS * 1000000L;
      ts.tv_sec += ts.tv_nsec / 1000000000L;
      ts.tv_nsec %= 1000000000L;
      pthread_cond_timedwait(&ctx->cond_rx, &ctx->lock, &ts);
    }
    if (!keep_running) {
      pthread_mutex_unlock(&ctx->lock);
      break;
    }
    pthread_mutex_unlock(&ctx->lock);

    rc = stream_read_frame(ctx, slot);
    if (rc < 0) {
      keep_running = 0;
      pthread_cond_broadcast(&ctx->cond_rx);
      pthread_cond_broadcast(&ctx->cond_tx);
      break;
    }
    if (rc > 0) {
      continue;
    }

    deinterleave_slot(slot);

    pthread_mutex_lock(&ctx->lock);
    slot->generation = ++ctx->rx_generation;
    slot->state = STREAM_SLOT_READY;
    pthread_cond_broadcast(&ctx->cond_tx);
    pthread_mutex_unlock(&ctx->lock);
  }

  return NULL;
}

static void *tx_thread_main(void *arg)
{
  stream_ctx_t *ctx = (stream_ctx_t *)arg;
  request_realtime_thread("tx", STREAM_THREAD_PRIO_TX);

  while (keep_running) {
    stream_tx_slot_t *slot;
    int rc;

    pthread_mutex_lock(&ctx->lock);
    while (keep_running && (slot = find_oldest_output_ready(ctx)) == NULL) {
      struct timespec ts;
      clock_gettime(CLOCK_REALTIME, &ts);
      ts.tv_nsec += STREAM_COND_WAIT_MS * 1000000L;
      ts.tv_sec += ts.tv_nsec / 1000000000L;
      ts.tv_nsec %= 1000000000L;
      pthread_cond_timedwait(&ctx->cond_tx, &ctx->lock, &ts);
    }
    if (!keep_running) {
      pthread_mutex_unlock(&ctx->lock);
      break;
    }
    slot->state = STREAM_SLOT_IN_USE;
    pthread_mutex_unlock(&ctx->lock);

    rc = stream_write_frame(ctx, slot);
    if (rc < 0) {
      keep_running = 0;
      pthread_cond_broadcast(&ctx->cond_rx);
      pthread_cond_broadcast(&ctx->cond_tx);
      break;
    }

    pthread_mutex_lock(&ctx->lock);
    slot->state = STREAM_SLOT_EMPTY;
    pthread_cond_broadcast(&ctx->cond_rx);
    pthread_mutex_unlock(&ctx->lock);
  }

  return NULL;
}

static void poll_stdin_commands(void)
{
  fd_set fds;
  struct timeval tv = {0, 0};

  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) <= 0) {
    return;
  }

  if (FD_ISSET(STDIN_FILENO, &fds)) {
    char line[64];
    if (!fgets(line, sizeof(line), stdin)) {
      return;
    }
    if (line[0] == 'q') {
      keep_running = 0;
      return;
    }
#if PARAM_TUNING_ENABLED
    for (size_t i = 0; g_params[i].label != NULL; ++i) {
      size_t L = strlen(g_params[i].label);
      if (strncasecmp(line, g_params[i].label, L) == 0 && line[L] == '=') {
        float val;
        if (sscanf(line + L + 1, "%f", &val) == 1) {
          g_params[i].set(val);
          printf("%s = %.4f\n", g_params[i].label, val);
        }
        break;
      }
    }
#endif
  }
}

int main(int argc, char **argv)
{
  unsigned int rate = 48000;
  stream_pcm_format_t default_format;
  pthread_t rx_thread;
  pthread_t tx_thread;
  int rx_thread_started = 0;
  int tx_thread_started = 0;
  const int debug_peaks = getenv_flag("STREAM_DEBUG_PEAKS");
  const int debug_status = getenv_flag("STREAM_DEBUG_STATUS");
  const int verbose = stream_main_verbose();
  unsigned long debug_counter = 0;

  (void)argc;
  (void)argv;

  memset(&g_ctx, 0, sizeof(g_ctx));
  pthread_mutex_init(&g_ctx.lock, NULL);
  pthread_cond_init(&g_ctx.cond_rx, NULL);
  pthread_cond_init(&g_ctx.cond_tx, NULL);

  lock_process_memory();
  default_format.direction = STREAM_DIRECTION_PLAYBACK;
  default_format.sample_rate_hz = rate;
  default_format.channels = 2;
  default_format.sample_format = STREAM_SAMPLE_FLOAT32LE;
  default_format.frames_per_period = MODEL_FRAME_LENGTH;
  default_format.period_count = 4;
  stream_io_cfg_load_from_env(&g_ctx.backend.cfg, &default_format);
  if (stream_backend_open(&g_ctx.backend) < 0) {
    return 1;
  }
  if (stream_backend_prepare(&g_ctx.backend) < 0) {
    stream_backend_close(&g_ctx.backend);
    return 1;
  }
  if (stream_backend_start(&g_ctx.backend) < 0) {
    stream_backend_close(&g_ctx.backend);
    return 1;
  }

  MODEL_INITIALIZE();
  if (verbose) {
    if (g_ctx.backend.cfg.kind == STREAM_IO_BACKEND_UDP) {
      printf("stream_main: model initialized, backend='%s', rx=%s:%d tx=%s:%d, N=%d, Fs=%u\n",
             stream_backend_kind_name(g_ctx.backend.cfg.kind),
             g_ctx.backend.cfg.rx_host, g_ctx.backend.cfg.rx_port,
             g_ctx.backend.cfg.tx_host, g_ctx.backend.cfg.tx_port,
             MODEL_FRAME_LENGTH, rate);
    } else {
      printf("stream_main: model initialized, backend='%s', capture='%s', playback='%s', N=%d, Fs=%u\n",
             stream_backend_kind_name(g_ctx.backend.cfg.kind),
             g_ctx.backend.cfg.capture_device,
             g_ctx.backend.cfg.playback_device,
             MODEL_FRAME_LENGTH, rate);
    }
  }

#ifdef ENABLE_CONTROL
  control_init();
#endif

  signal(SIGINT, sigint_handler);
  signal(SIGTERM, sigint_handler);
  print_commands();
  request_realtime_thread("model", STREAM_THREAD_PRIO_MODEL);

  if (pthread_create(&rx_thread, NULL, rx_thread_main, &g_ctx) != 0) {
    fprintf(stderr, "stream_main: cannot create RX thread\n");
    keep_running = 0;
  } else {
    rx_thread_started = 1;
  }
  if (keep_running && pthread_create(&tx_thread, NULL, tx_thread_main, &g_ctx) != 0) {
    fprintf(stderr, "stream_main: cannot create TX thread\n");
    keep_running = 0;
  } else if (keep_running) {
    tx_thread_started = 1;
  }

  while (keep_running) {
    stream_rx_slot_t *rx_slot;
    stream_tx_slot_t *tx_slot;

    pthread_mutex_lock(&g_ctx.lock);
    while (keep_running && (rx_slot = find_oldest_input_ready(&g_ctx)) == NULL) {
      struct timespec ts;
      clock_gettime(CLOCK_REALTIME, &ts);
      ts.tv_nsec += STREAM_COND_WAIT_MS * 1000000L;
      ts.tv_sec += ts.tv_nsec / 1000000000L;
      ts.tv_nsec %= 1000000000L;
      pthread_cond_timedwait(&g_ctx.cond_tx, &g_ctx.lock, &ts);
    }
    if (!keep_running) {
      pthread_mutex_unlock(&g_ctx.lock);
      break;
    }
    rx_slot->state = STREAM_SLOT_IN_USE;
    tx_slot = find_slot_for_tx_write(&g_ctx);
    if (tx_slot == NULL) {
      tx_slot = find_oldest_output_ready(&g_ctx);
      if (tx_slot != NULL) {
        tx_slot->state = STREAM_SLOT_IN_USE;
      }
    }
    pthread_mutex_unlock(&g_ctx.lock);

    if (tx_slot == NULL) {
      continue;
    }

    g_stream_capture_l = rx_slot->in_l;
    g_stream_capture_r = rx_slot->in_r;
    g_stream_playback_l = tx_slot->out_l;
    g_stream_playback_r = tx_slot->out_r;
    g_stream_offset = 0;

#ifdef ENABLE_CONTROL
    control_audio_tick();
#endif
    MODEL_STEP();

    interleave_slot(tx_slot);
    tx_slot->hdr = rx_slot->hdr;

    if (debug_peaks || debug_status) {
      ++debug_counter;
    }

    if (debug_peaks) {
      if ((debug_counter % 128UL) == 0UL) {
        fprintf(stderr,
                "stream_main: gen=%lu in_peak_l=%.6f in_peak_r=%.6f out_peak_l=%.6f out_peak_r=%.6f\n",
                rx_slot->generation,
                block_peak_abs(rx_slot->in_l, MODEL_FRAME_LENGTH),
                block_peak_abs(rx_slot->in_r, MODEL_FRAME_LENGTH),
                block_peak_abs(tx_slot->out_l, MODEL_FRAME_LENGTH),
                block_peak_abs(tx_slot->out_r, MODEL_FRAME_LENGTH));
      }
    }

    if (debug_status) {
      if ((debug_counter % 128UL) == 0UL) {
        stream_status_t status;
        memset(&status, 0, sizeof(status));
        stream_backend_query_status(&g_ctx.backend, &status);
        fprintf(stderr,
                "stream_main: backend state=%d queued=%u avail=%u delay=%u xruns=%llu last_seq=%llu\n",
                (int)status.state,
                status.queued_frames,
                status.available_frames,
                status.delay_frames,
                (unsigned long long)status.xruns,
                (unsigned long long)status.last_sequence);
      }
    }

    pthread_mutex_lock(&g_ctx.lock);
    rx_slot->state = STREAM_SLOT_EMPTY;
    tx_slot->generation = rx_slot->generation;
    tx_slot->state = STREAM_SLOT_READY;
    g_ctx.tx_generation = tx_slot->generation;
    pthread_cond_broadcast(&g_ctx.cond_tx);
    pthread_cond_broadcast(&g_ctx.cond_rx);
    pthread_mutex_unlock(&g_ctx.lock);

    poll_stdin_commands();
  }

  keep_running = 0;
  pthread_cond_broadcast(&g_ctx.cond_rx);
  pthread_cond_broadcast(&g_ctx.cond_tx);
  if (rx_thread_started) {
    pthread_join(rx_thread, NULL);
  }
  if (tx_thread_started) {
    pthread_join(tx_thread, NULL);
  }

#ifdef ENABLE_CONTROL
  control_shutdown();
#endif

  stream_backend_stop(&g_ctx.backend);
  stream_backend_close(&g_ctx.backend);
  pthread_mutex_destroy(&g_ctx.lock);
  pthread_cond_destroy(&g_ctx.cond_rx);
  pthread_cond_destroy(&g_ctx.cond_tx);

  MODEL_TERMINATE();
  return 0;
}
