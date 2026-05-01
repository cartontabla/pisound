#ifndef PISOUND_STREAM_CONTRACT_H
#define PISOUND_STREAM_CONTRACT_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    STREAM_DIRECTION_PLAYBACK = 0,
    STREAM_DIRECTION_CAPTURE = 1,
} stream_direction_t;

typedef enum {
    STREAM_SAMPLE_S16LE = 0,
    STREAM_SAMPLE_S24LE = 1,
    STREAM_SAMPLE_S32LE = 2,
    STREAM_SAMPLE_FLOAT32LE = 3,
} stream_sample_format_t;

typedef enum {
    STREAM_STATE_CLOSED = 0,
    STREAM_STATE_OPEN = 1,
    STREAM_STATE_PREPARED = 2,
    STREAM_STATE_RUNNING = 3,
    STREAM_STATE_DRAINING = 4,
    STREAM_STATE_STOPPED = 5,
} stream_state_t;

typedef struct {
    stream_direction_t direction;
    uint32_t sample_rate_hz;
    uint16_t channels;
    stream_sample_format_t sample_format;
    uint32_t frames_per_period;
    uint16_t period_count;
} stream_pcm_format_t;

typedef struct {
    uint32_t frame_count;
    uint64_t pts_monotonic_ns;
    uint64_t sequence;
} stream_buffer_meta_t;

typedef struct {
    stream_state_t state;
    uint32_t queued_frames;
    uint32_t available_frames;
    uint64_t xruns;
    uint32_t delay_frames;
    uint64_t last_sequence;
} stream_status_t;

int stream_pcm_format_validate(const stream_pcm_format_t *fmt, char *err, size_t err_size);
size_t stream_bytes_per_sample(stream_sample_format_t sample_format);
size_t stream_bytes_per_frame(const stream_pcm_format_t *fmt);
size_t stream_bytes_per_period(const stream_pcm_format_t *fmt);
const char *stream_direction_name(stream_direction_t direction);
const char *stream_sample_format_name(stream_sample_format_t sample_format);
const char *stream_state_name(stream_state_t state);

#endif
