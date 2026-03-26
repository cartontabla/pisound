#ifndef UNIFIED_AUDIO_H
#define UNIFIED_AUDIO_H

#include <jack/jack.h>
#include <stddef.h>

/* Global pointers to JACK audio buffers */
/* These are set by custom_main.c before calling model_step() */
extern float *g_jack_in_l;
extern float *g_jack_in_r;
extern float *g_jack_out_l;
extern float *g_jack_out_r;

/* MIDI globals if needed */
extern void *g_jack_midi_in;
extern void *g_jack_midi_out;
extern jack_nframes_t g_nframes;
extern jack_nframes_t g_frame_offset;   /* offset dentro del buffer JACK (0, 128, ...) */

#endif

