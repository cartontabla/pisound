/*
 * custom_main.c: Generic execution harness for Simulink models on Pisound
 * (JACK) Supports synchronous execution and runtime parameter tuning via
 * macros.
 */

#include <jack/jack.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>   /* strcasecmp/strncasecmp */
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

#include "unified_audio.h"

#ifdef ENABLE_CONTROL
#include "control/control.h"
#endif

#ifndef MODEL_FRAME_LENGTH
#define MODEL_FRAME_LENGTH 128
#endif

jack_nframes_t g_nframes = 0;
jack_nframes_t g_frame_offset = 0;

#ifndef MODEL_HEADER
#error "MODEL_HEADER no definido: revisa compile.sh (o flags). No se permite fallback a bypass."
#endif
#include MODEL_HEADER

/* Parameter Tuning Macros - Defaults do nothing */
#ifndef PARAM_STRUCT_VAR
#define PARAM_TUNING_ENABLED 0
#else
#define PARAM_TUNING_ENABLED 1
extern PARAM_STRUCT_TYPE PARAM_STRUCT_VAR;
#endif

/* Global audio pointers */
float *g_jack_in_l = NULL;
float *g_jack_in_r = NULL;
float *g_jack_out_l = NULL;
float *g_jack_out_r = NULL;
void *g_jack_midi_in = NULL;
void *g_jack_midi_out = NULL;

static int keep_running = 1;

static void sigint_handler(int sig) {
  (void)sig;
  keep_running = 0;
}

#if PARAM_TUNING_ENABLED
typedef void (*param_setter_t)(float v);

typedef struct {
  const char* label;
  param_setter_t set;
} param_desc_t;

/* 1) Genera setters (set_p1..set_p20) */
#define PARAM_SLOTS_DEFINE_SETTERS 1
#include "param_slots_20.h"
#undef PARAM_SLOTS_DEFINE_SETTERS

/* 2) Genera la tabla {label, setter} */
static const param_desc_t g_params[] = {
#define PARAM_SLOTS_DEFINE_TABLE 1
#include "param_slots_20.h"
#undef PARAM_SLOTS_DEFINE_TABLE
  { NULL, NULL }
};

/* Utilidad: ¿existe ya un label (case-insensitive) en la tabla? */
static int has_label_ci(const char* s) {
  for (size_t i = 0; g_params[i].label != NULL; ++i) {
    if (strcasecmp(g_params[i].label, s) == 0) return 1;
  }
  return 0;
}
#endif /* PARAM_TUNING_ENABLED */

static void print_commands(void) {
  printf("Commands:\n");

#if PARAM_TUNING_ENABLED
  for (size_t i = 0; g_params[i].label != NULL; ++i) {
    printf("  %s=<val>\n", g_params[i].label);
  }

#ifdef PARAM_BYPASS_MEMBER
  /* Evita duplicar ayuda si "Bypass" ya está expuesto como genérico */
  if (!has_label_ci("Bypass")) {
    printf("  Bypass=<0/1>\n");
  }
#endif

  printf("  ?            : Show commands\n");
#endif

  printf("  q            : Quit\n");
}

int process(jack_nframes_t nframes, void *arg) {
  jack_port_t *in_l = (jack_port_t *)((void **)arg)[0];
  jack_port_t *in_r = (jack_port_t *)((void **)arg)[1];
  jack_port_t *out_l = (jack_port_t *)((void **)arg)[2];
  jack_port_t *out_r = (jack_port_t *)((void **)arg)[3];

  float *inL = (float *)jack_port_get_buffer(in_l, nframes);
  float *inR = (float *)jack_port_get_buffer(in_r, nframes);
  float *outL = (float *)jack_port_get_buffer(out_l, nframes);
  float *outR = (float *)jack_port_get_buffer(out_r, nframes);

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
      g_nframes = MODEL_FRAME_LENGTH;
      g_frame_offset = off;

#ifdef ENABLE_CONTROL
    control_process_rx();
#endif

    MODEL_STEP();

#ifdef ENABLE_CONTROL
    control_process_tx();
#endif
  }
  return 0;
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  jack_client_t *client;
  jack_status_t status;
  jack_port_t *ports[4];

  MODEL_INITIALIZE();
#ifdef MODEL_NAME_STRING
  printf("Model: %s\n", MODEL_NAME_STRING);
#endif
  printf("Model Initialized.\n");

  client = jack_client_open("pisound_model", JackNullOption, &status);
  if (!client) {
    fprintf(stderr, "jack_client_open() failed, status = 0x%2.0x\n", status);
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

  if (!ports[0] || !ports[1] || !ports[2] || !ports[3])
    return 1;

  jack_set_process_callback(client, process, ports);

  if (jack_activate(client))
    return 1;

  jack_connect(client, "system:capture_1", jack_port_name(ports[0]));
  jack_connect(client, "system:capture_2", jack_port_name(ports[1]));
  jack_connect(client, jack_port_name(ports[2]), "system:playback_1");
  jack_connect(client, jack_port_name(ports[3]), "system:playback_2");

  jack_nframes_t jack_buf = jack_get_buffer_size(client);
  printf("JACK buffer size = %u\n", (unsigned)jack_buf);

  jack_nframes_t jack_sr = jack_get_sample_rate(client);

#ifdef ENABLE_CONTROL
  control_init();
#endif
    
#if PARAM_TUNING_ENABLED && defined(PARAM_FS_MEMBER)
  jack_nframes_t jack_sr = jack_get_sample_rate(client);
  PARAM_STRUCT_VAR.PARAM_FS_MEMBER = (double)jack_sr;
  printf("Injected Fs = %u Hz from JACK\n", (unsigned)jack_sr);
#endif

  printf("Running. Press Ctrl+C to quit.\n");

#if PARAM_TUNING_ENABLED
  printf("Interactive Controls:\n");
#endif
  print_commands();

  signal(SIGINT, sigint_handler);

  fd_set readfds;
  while (keep_running) {
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;

    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    int ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
    if (ret > 0 && FD_ISSET(STDIN_FILENO, &readfds)) {
      char line[64];
      if (fgets(line, sizeof(line), stdin)) {
        if (line[0] == 'q') {
          keep_running = 0;
          continue;
        }

#if PARAM_TUNING_ENABLED
        if (line[0] == '?') {
          print_commands();
          continue;
        }

        /* -------- Parser único: recorre la tabla -------- */
        {
          float val;
          int handled = 0;

          for (size_t i = 0; g_params[i].label != NULL; ++i) {
            size_t L = strlen(g_params[i].label);
            if (strncasecmp(line, g_params[i].label, L) == 0 && line[L] == '=') {
              if (sscanf(line + L + 1, "%f", &val) == 1) {
                g_params[i].set(val);
                printf("%s set to %.4f\n", g_params[i].label, val);
              }
              handled = 1;
              break;
            }
          }
          if (handled) continue;
        }

        /* Legacy special: Bypass=0/1 */
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

#endif /* PARAM_TUNING_ENABLED */
      }
    }
  }

#ifdef ENABLE_CONTROL
  control_shutdown();
#endif
  
  jack_client_close(client);
  MODEL_TERMINATE();
  return 0;
}