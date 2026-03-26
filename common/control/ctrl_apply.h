#ifndef CTRL_APPLY_H
#define CTRL_APPLY_H

#include "ctrl_event.h"
#include "ctrl_map.h"
#include <stdint.h>
#include <stddef.h>

/*
 * ctrl_apply.h
 *
 * Applies control events to audio parameters with smooth ramping.
 * Handles click-free parameter transitions at audio rate.
 */

typedef struct {
    float       current_val;        /* Current parameter value */
    float       target_val;         /* Target value (from event) */
    float       ramp_rate;          /* Samples per unit (computed from ramp_time_ms) */
    uint32_t    ramp_samples_left;  /* Countdown to end of ramp */
    int         is_ramping;         /* 1 if currently ramping */
} ctrl_param_state_t;

typedef struct {
    ctrl_param_state_t *params;     /* Per-parameter state */
    size_t              param_count;
    
    uint32_t            sample_rate; /* Audio sample rate (Hz) */
    uint32_t            frame_size;  /* Audio frame size (samples) */
} ctrl_apply_ctx_t;

/* Public API */
int  ctrl_apply_init(ctrl_apply_ctx_t *ctx, size_t param_count, 
                     uint32_t sample_rate, uint32_t frame_size);
void ctrl_apply_free(ctrl_apply_ctx_t *ctx);

/* Apply a single event with ramping */
void ctrl_apply_event(ctrl_apply_ctx_t *ctx, const ctrl_event_t *evt,
                      const ctrl_mapping_t *mapping);

/* Get current value of a parameter (interpolated during ramp) */
float ctrl_apply_get(const ctrl_apply_ctx_t *ctx, uint32_t param_id);

/* Get target value (destination after ramp) */
float ctrl_apply_get_target(const ctrl_apply_ctx_t *ctx, uint32_t param_id);

/* Process one frame: advance ramps */
void ctrl_apply_process_frame(ctrl_apply_ctx_t *ctx);

/* Set parameter value directly (no ramping, use with caution) */
void ctrl_apply_set_immediate(ctrl_apply_ctx_t *ctx, uint32_t param_id, float val);

#endif /* CTRL_APPLY_H */
