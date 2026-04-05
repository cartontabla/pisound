#ifndef STREAM_AUDIO_H
#define STREAM_AUDIO_H

#include <stddef.h>

/* Global pointers to AES67 audio buffers.
 * Set by custom_main.c before calling model_step(),
 * filled/drained by the aes67-daemon interface. */
extern float *g_stream_in_l;
extern float *g_stream_in_r;
extern float *g_stream_out_l;
extern float *g_stream_out_r;

extern int g_stream_offset;  /* frame offset within the buffer (analogous to g_frame_offset) */

#endif
