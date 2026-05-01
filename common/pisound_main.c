/*
 * pisound_main.c: Hybrid execution harness for Simulink models that use both
 * Pisound/JACK S-Functions (pisound_in/out) and stream S-Functions
 * (stream_in/out) in the same model.
 */

#include <jack/jack.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "stream_audio.h"
#include "stream_direct_proto.h"
#include "stream/stream_backend.h"
#include "stream/stream_contract.h"
#include "unified_audio.h"

#ifdef ENABLE_CONTROL
#include "control/control.h"
#include "ctrl_in.h"
#endif

#ifndef MODEL_FRAME_LENGTH
#define MODEL_FRAME_LENGTH 128
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
#define STREAM_COND_WAIT_MS 100

jack_nframes_t g_nframes = 0;
jack_nframes_t g_frame_offset = 0;
float *g_jack_in_l = NULL;
float *g_jack_in_r = NULL;
float *g_jack_out_l = NULL;
float *g_jack_out_r = NULL;
void *g_jack_midi_in = NULL;
void *g_jack_midi_out = NULL;

float *g_stream_capture_l = NULL;
float *g_stream_capture_r = NULL;
float *g_stream_playback_l = NULL;
float *g_stream_playback_r = NULL;
int g_stream_offset = 0;

static volatile sig_atomic_t keep_running = 1;

static void sigint_handler(int sig)
{
  (void)sig;
  keep_running = 0;
}

#if PARAM_TUNING_ENABLED
typedef void (*param_setter_t)(float v);

typedef struct {
  const char *label;
  param_setter_t set;
} param_desc_t;

#define PARAM_SLOTS_DEFINE_SETTERS 1
#include "param_slots_20.h"
#undef PARAM_SLOTS_DEFINE_SETTERS

static const param_desc_t g_params[] = {
#define PARAM_SLOTS_DEFINE_TABLE 1
#include "param_slots_20.h"
#undef PARAM_SLOTS_DEFINE_TABLE
  { NULL, NULL }
};

static int has_label_ci(const char *s)
{
  for (size_t i = 0; g_params[i].label != NULL; ++i) {
    if (strcasecmp(g_params[i].label, s) == 0) return 1;
  }
  return 0;
}
#endif

static void print_commands(void)
{
  printf("Commands:\n");

#if PARAM_TUNING_ENABLED
  for (size_t i = 0; g_params[i].label != NULL; ++i) {
    printf("  %s=<val>\n", g_params[i].label);
  }

#ifdef PARAM_BYPASS_MEMBER
  if (!has_label_ci("Bypass")) {
    printf("  Bypass=<0/1>\n");
  }
#endif

  printf("  ?            : Show commands\n");
#endif

  printf("  q            : Quit\n");
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
} stream_ctx_t;

enum {
  STREAM_SLOT_EMPTY = 0,
  STREAM_SLOT_READY = 1,
  STREAM_SLOT_IN_USE = 2,
};

static stream_ctx_t g_ctx;

typedef enum {
  STREAM_THREAD_PRIO_RX = 0,
  STREAM_THREAD_PRIO_MODEL = 1,
  STREAM_THREAD_PRIO_TX = 2,
} stream_thread_prio_kind_t;

static int getenv_int_local(const char *name, int fallback)
{
  const char *value = getenv(name);
  if (!value || value[0] == '\0') {
    return fallback;
  }
  return atoi(value);
}

static int lock_process_memory(void)
{
  if (mlockall(MCL_CURRENT | MCL_FUTURE) != 0) {
    fprintf(stderr, "pisound_main: mlockall failed: %s\n", strerror(errno));
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
  prio = getenv_int_local(env_name, fallback);
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
    fprintf(stderr, "pisound_main: cannot query RT priority for %s thread\n", name);
    return;
  }

  memset(&param, 0, sizeof(param));
  param.sched_priority = prio;
  rc = pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
  if (rc != 0) {
    fprintf(stderr,
            "pisound_main: unable to enable SCHED_FIFO for %s thread at prio %d: %s\n",
            name, prio, strerror(rc));
    return;
  }
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
  request_realtime_thread("hybrid-rx", STREAM_THREAD_PRIO_RX);

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

    rc = stream_backend_read_frame(&ctx->backend,
                                   &slot->hdr,
                                   slot->in_interleaved,
                                   STREAM_BLOCK_SAMPLES);
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
  request_realtime_thread("hybrid-tx", STREAM_THREAD_PRIO_TX);

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

    rc = stream_backend_write_frame(&ctx->backend,
                                    &slot->hdr,
                                    slot->out_interleaved,
                                    STREAM_BLOCK_SAMPLES);
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

static int process(jack_nframes_t nframes, void *arg)
{
  jack_port_t *in_l = (jack_port_t *)((void **)arg)[0];
  jack_port_t *in_r = (jack_port_t *)((void **)arg)[1];
  jack_port_t *out_l = (jack_port_t *)((void **)arg)[2];
  jack_port_t *out_r = (jack_port_t *)((void **)arg)[3];
  float *inL = (float *)jack_port_get_buffer(in_l, nframes);
  float *inR = (float *)jack_port_get_buffer(in_r, nframes);
  float *outL = (float *)jack_port_get_buffer(out_l, nframes);
  float *outR = (float *)jack_port_get_buffer(out_r, nframes);
  float null_capture_l[MODEL_FRAME_LENGTH] = {0};
  float null_capture_r[MODEL_FRAME_LENGTH] = {0};
  float null_playback_l[MODEL_FRAME_LENGTH] = {0};
  float null_playback_r[MODEL_FRAME_LENGTH] = {0};

  if (nframes % MODEL_FRAME_LENGTH != 0) {
    for (jack_nframes_t i = 0; i < nframes; ++i) {
      outL[i] = 0.0f;
      outR[i] = 0.0f;
    }
    return 0;
  }

  g_jack_in_l = inL;
  g_jack_in_r = inR;
  g_jack_out_l = outL;
  g_jack_out_r = outR;

  for (jack_nframes_t off = 0; off < nframes; off += MODEL_FRAME_LENGTH) {
    stream_rx_slot_t *rx_slot = NULL;
    stream_tx_slot_t *tx_slot = NULL;

    pthread_mutex_lock(&g_ctx.lock);
    rx_slot = find_oldest_input_ready(&g_ctx);
    if (rx_slot != NULL) {
      rx_slot->state = STREAM_SLOT_IN_USE;
    }
    tx_slot = find_slot_for_tx_write(&g_ctx);
    if (tx_slot != NULL) {
      tx_slot->state = STREAM_SLOT_IN_USE;
    }
    pthread_mutex_unlock(&g_ctx.lock);

    g_nframes = MODEL_FRAME_LENGTH;
    g_frame_offset = off;

    g_stream_capture_l = rx_slot ? rx_slot->in_l : null_capture_l;
    g_stream_capture_r = rx_slot ? rx_slot->in_r : null_capture_r;
    g_stream_playback_l = tx_slot ? tx_slot->out_l : null_playback_l;
    g_stream_playback_r = tx_slot ? tx_slot->out_r : null_playback_r;
    g_stream_offset = 0;

    if (tx_slot) {
      memset(tx_slot->out_l, 0, sizeof(tx_slot->out_l));
      memset(tx_slot->out_r, 0, sizeof(tx_slot->out_r));
    }

#ifdef ENABLE_CONTROL
    control_audio_tick();
#if PARAM_TUNING_ENABLED
  #ifdef PARAM_P1_CTRL_INDEX
    set_p1(ctrl_in_get(PARAM_P1_CTRL_INDEX));
  #endif
  #ifdef PARAM_P2_CTRL_INDEX
    set_p2(ctrl_in_get(PARAM_P2_CTRL_INDEX));
  #endif
  #ifdef PARAM_P3_CTRL_INDEX
    set_p3(ctrl_in_get(PARAM_P3_CTRL_INDEX));
  #endif
  #ifdef PARAM_P4_CTRL_INDEX
    set_p4(ctrl_in_get(PARAM_P4_CTRL_INDEX));
  #endif
#endif
#endif

    MODEL_STEP();

    pthread_mutex_lock(&g_ctx.lock);
    if (rx_slot) {
      rx_slot->state = STREAM_SLOT_EMPTY;
    }
    if (tx_slot) {
      interleave_slot(tx_slot);
      if (rx_slot) {
        tx_slot->hdr = rx_slot->hdr;
      } else {
        memset(&tx_slot->hdr, 0, sizeof(tx_slot->hdr));
        tx_slot->hdr.magic = STREAM_DIRECT_MAGIC;
        tx_slot->hdr.frames = MODEL_FRAME_LENGTH;
      }
      tx_slot->generation = ++g_ctx.tx_generation;
      tx_slot->state = STREAM_SLOT_READY;
      pthread_cond_broadcast(&g_ctx.cond_tx);
    }
    pthread_cond_broadcast(&g_ctx.cond_rx);
    pthread_mutex_unlock(&g_ctx.lock);
  }

  return 0;
}

int main(int argc, char **argv)
{
  unsigned int rate = 48000;
  stream_pcm_format_t default_format;
  pthread_t rx_thread;
  pthread_t tx_thread;
  int rx_thread_started = 0;
  int tx_thread_started = 0;
  jack_client_t *client;
  jack_status_t status;
  jack_port_t *ports[4];

  (void)argc;
  (void)argv;

  setvbuf(stdout, NULL, _IOLBF, 0);
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
#ifdef MODEL_NAME_STRING
  printf("Model: %s\n", MODEL_NAME_STRING);
#endif
  printf("Model Initialized.\n");

  client = jack_client_open("pisound_model", JackNullOption, &status);
  if (!client) {
    fprintf(stderr, "jack_client_open() failed, status = 0x%2.0x\n", status);
    stream_backend_stop(&g_ctx.backend);
    stream_backend_close(&g_ctx.backend);
    return 1;
  }

  ports[0] = jack_port_register(client, "input_L", JACK_DEFAULT_AUDIO_TYPE,
                                JackPortIsInput, 0);
  ports[1] = jack_port_register(client, "input_R", JACK_DEFAULT_AUDIO_TYPE,
                                JackPortIsInput, 0);
  ports[2] = jack_port_register(client, "output_L", JACK_DEFAULT_AUDIO_TYPE,
                                JackPortIsOutput, 0);
  ports[3] = jack_port_register(client, "output_R", JACK_DEFAULT_AUDIO_TYPE,
                                JackPortIsOutput, 0);

  if (!ports[0] || !ports[1] || !ports[2] || !ports[3]) {
    jack_client_close(client);
    stream_backend_stop(&g_ctx.backend);
    stream_backend_close(&g_ctx.backend);
    return 1;
  }

  jack_set_process_callback(client, process, ports);

  rate = jack_get_sample_rate(client);

#ifdef ENABLE_CONTROL
  control_init();
#endif

  if (pthread_create(&rx_thread, NULL, rx_thread_main, &g_ctx) != 0) {
    fprintf(stderr, "pisound_main: cannot create stream RX thread\n");
    jack_client_close(client);
    stream_backend_stop(&g_ctx.backend);
    stream_backend_close(&g_ctx.backend);
    return 1;
  }
  rx_thread_started = 1;

  if (pthread_create(&tx_thread, NULL, tx_thread_main, &g_ctx) != 0) {
    fprintf(stderr, "pisound_main: cannot create stream TX thread\n");
    keep_running = 0;
    pthread_cond_broadcast(&g_ctx.cond_rx);
    pthread_join(rx_thread, NULL);
    jack_client_close(client);
    stream_backend_stop(&g_ctx.backend);
    stream_backend_close(&g_ctx.backend);
    return 1;
  }
  tx_thread_started = 1;

  if (jack_activate(client)) {
    keep_running = 0;
    pthread_cond_broadcast(&g_ctx.cond_rx);
    pthread_cond_broadcast(&g_ctx.cond_tx);
    pthread_join(rx_thread, NULL);
    pthread_join(tx_thread, NULL);
    jack_client_close(client);
    stream_backend_stop(&g_ctx.backend);
    stream_backend_close(&g_ctx.backend);
    return 1;
  }

  jack_connect(client, "system:capture_1", jack_port_name(ports[0]));
  jack_connect(client, "system:capture_2", jack_port_name(ports[1]));
  jack_connect(client, jack_port_name(ports[2]), "system:playback_1");
  jack_connect(client, jack_port_name(ports[3]), "system:playback_2");

#if PARAM_TUNING_ENABLED && defined(PARAM_FS_MEMBER)
  PARAM_STRUCT_VAR.PARAM_FS_MEMBER = (double)rate;
  printf("Injected Fs = %u Hz from JACK\n", rate);
#endif

  printf("Running. Press Ctrl+C to quit.\n");
  print_commands();

  signal(SIGINT, sigint_handler);
  signal(SIGTERM, sigint_handler);

  {
    fd_set readfds;
    int stdin_active = 1;
    while (keep_running) {
      struct timeval tv;
      tv.tv_sec = 0;
      tv.tv_usec = 100000;

      FD_ZERO(&readfds);
      if (stdin_active) {
        FD_SET(STDIN_FILENO, &readfds);
      }

      if (select(STDIN_FILENO + 1, stdin_active ? &readfds : NULL, NULL, NULL, &tv) > 0 &&
          stdin_active && FD_ISSET(STDIN_FILENO, &readfds)) {
        char line[64];
        if (!fgets(line, sizeof(line), stdin)) {
          stdin_active = 0;
          continue;
        }

        if (line[0] == 'q') {
          keep_running = 0;
          continue;
        }

#if PARAM_TUNING_ENABLED
        if (line[0] == '?') {
          print_commands();
          continue;
        }

        {
          float val;
          int handled = 0;

          for (size_t i = 0; g_params[i].label != NULL; ++i) {
            size_t L = strlen(g_params[i].label);
            if (strncasecmp(line, g_params[i].label, L) == 0 && line[L] == '=') {
              if (sscanf(line + L + 1, "%f", &val) == 1) {
                g_params[i].set(val);
                printf("%s set to %.4f\n", g_params[i].label, val);
#ifdef ENABLE_CONTROL
                switch (i) {
                  #ifdef PARAM_P1_CTRL_INDEX
                  case 0: control_set_param(PARAM_P1_CTRL_INDEX, val); break;
                  #endif
                  #ifdef PARAM_P2_CTRL_INDEX
                  case 1: control_set_param(PARAM_P2_CTRL_INDEX, val); break;
                  #endif
                  #ifdef PARAM_P3_CTRL_INDEX
                  case 2: control_set_param(PARAM_P3_CTRL_INDEX, val); break;
                  #endif
                  #ifdef PARAM_P4_CTRL_INDEX
                  case 3: control_set_param(PARAM_P4_CTRL_INDEX, val); break;
                  #endif
                  default: break;
                }
#endif
              }
              handled = 1;
              break;
            }
          }
          if (handled) continue;
        }

#ifdef PARAM_BYPASS_MEMBER
        {
          float val;
          if (strncmp(line, "Bypass=", 7) == 0) {
            if (sscanf(line + 7, "%f", &val) == 1) {
              int ival = (int)val;
              PARAM_STRUCT_VAR.PARAM_BYPASS_MEMBER = (ival != 0);
              printf("Bypass set to %d\n", (ival != 0));
            }
            continue;
          }
        }
#endif
#endif
      }
    }
  }

  keep_running = 0;
  pthread_cond_broadcast(&g_ctx.cond_rx);
  pthread_cond_broadcast(&g_ctx.cond_tx);
  if (tx_thread_started) {
    pthread_join(tx_thread, NULL);
  }
  if (rx_thread_started) {
    pthread_join(rx_thread, NULL);
  }

#ifdef ENABLE_CONTROL
  control_shutdown();
#endif

  jack_client_close(client);
  stream_backend_stop(&g_ctx.backend);
  stream_backend_close(&g_ctx.backend);
  pthread_mutex_destroy(&g_ctx.lock);
  pthread_cond_destroy(&g_ctx.cond_rx);
  pthread_cond_destroy(&g_ctx.cond_tx);

  MODEL_TERMINATE();
  return 0;
}
