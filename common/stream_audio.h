#ifndef STREAM_AUDIO_H
#define STREAM_AUDIO_H

#include <stddef.h>

/*
 * Global pointers to the current PCM period exchanged with the backend.
 *
 * The intended model is ALSA-like and transport-agnostic:
 * - capture_* is what the backend delivers to the model and stream_in reads
 * - playback_* is what the model produces and stream_out writes
 *
 * Whether the backend is backed by a loopback PCM, a RAVENNA/AES67 ALSA
 * device, or a direct test transport is deliberately hidden from the
 * S-Functions.
 */
extern float *g_stream_capture_l;
extern float *g_stream_capture_r;
extern float *g_stream_playback_l;
extern float *g_stream_playback_r;

extern int g_stream_offset;  /* frame offset within the buffer (analogous to g_frame_offset) */

/* Compatibility aliases used by older code while the naming migrates. */
#define g_stream_in_l     g_stream_capture_l
#define g_stream_in_r     g_stream_capture_r
#define g_stream_out_l    g_stream_playback_l
#define g_stream_out_r    g_stream_playback_r

#endif
