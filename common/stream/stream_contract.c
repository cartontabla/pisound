#include "stream_contract.h"

#include <stdio.h>

int stream_pcm_format_validate(const stream_pcm_format_t *fmt, char *err, size_t err_size)
{
    if (!fmt) {
        if (err && err_size) {
            snprintf(err, err_size, "null format");
        }
        return -1;
    }
    if (fmt->sample_rate_hz == 0U) {
        if (err && err_size) {
            snprintf(err, err_size, "sample_rate_hz must be > 0");
        }
        return -1;
    }
    if (fmt->channels == 0U) {
        if (err && err_size) {
            snprintf(err, err_size, "channels must be > 0");
        }
        return -1;
    }
    if (fmt->frames_per_period == 0U) {
        if (err && err_size) {
            snprintf(err, err_size, "frames_per_period must be > 0");
        }
        return -1;
    }
    if (fmt->period_count < 2U) {
        if (err && err_size) {
            snprintf(err, err_size, "period_count must be >= 2");
        }
        return -1;
    }
    if (stream_bytes_per_sample(fmt->sample_format) == 0U) {
        if (err && err_size) {
            snprintf(err, err_size, "unsupported sample_format");
        }
        return -1;
    }
    return 0;
}

size_t stream_bytes_per_sample(stream_sample_format_t sample_format)
{
    switch (sample_format) {
        case STREAM_SAMPLE_S16LE:
            return 2U;
        case STREAM_SAMPLE_S24LE:
            return 3U;
        case STREAM_SAMPLE_S32LE:
        case STREAM_SAMPLE_FLOAT32LE:
            return 4U;
        default:
            return 0U;
    }
}

size_t stream_bytes_per_frame(const stream_pcm_format_t *fmt)
{
    if (!fmt) {
        return 0U;
    }
    return (size_t)fmt->channels * stream_bytes_per_sample(fmt->sample_format);
}

size_t stream_bytes_per_period(const stream_pcm_format_t *fmt)
{
    if (!fmt) {
        return 0U;
    }
    return (size_t)fmt->frames_per_period * stream_bytes_per_frame(fmt);
}

const char *stream_direction_name(stream_direction_t direction)
{
    switch (direction) {
        case STREAM_DIRECTION_PLAYBACK:
            return "playback";
        case STREAM_DIRECTION_CAPTURE:
            return "capture";
        default:
            return "unknown";
    }
}

const char *stream_sample_format_name(stream_sample_format_t sample_format)
{
    switch (sample_format) {
        case STREAM_SAMPLE_S16LE:
            return "s16le";
        case STREAM_SAMPLE_S24LE:
            return "s24le";
        case STREAM_SAMPLE_S32LE:
            return "s32le";
        case STREAM_SAMPLE_FLOAT32LE:
            return "float32le";
        default:
            return "unknown";
    }
}

const char *stream_state_name(stream_state_t state)
{
    switch (state) {
        case STREAM_STATE_CLOSED:
            return "closed";
        case STREAM_STATE_OPEN:
            return "open";
        case STREAM_STATE_PREPARED:
            return "prepared";
        case STREAM_STATE_RUNNING:
            return "running";
        case STREAM_STATE_DRAINING:
            return "draining";
        case STREAM_STATE_STOPPED:
            return "stopped";
        default:
            return "unknown";
    }
}
