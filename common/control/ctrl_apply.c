#include "ctrl_apply.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
 * ctrl_apply.c
 *
 * Parameter ramping implementation.
 * Click-free transitions using linear or exponential interpolation.
 */

int ctrl_apply_init(ctrl_apply_ctx_t *ctx, size_t param_count,
                    uint32_t sample_rate, uint32_t frame_size)
{
    if (!ctx || param_count == 0) return -1;
    
    ctx->params = (ctrl_param_state_t *)malloc(param_count * sizeof(ctrl_param_state_t));
    if (!ctx->params) return -1;
    
    memset(ctx->params, 0, param_count * sizeof(ctrl_param_state_t));
    
    ctx->param_count = param_count;
    ctx->sample_rate = sample_rate;
    ctx->frame_size = frame_size;
    
    return 0;
}

void ctrl_apply_free(ctrl_apply_ctx_t *ctx)
{
    if (!ctx) return;
    if (ctx->params) {
        free(ctx->params);
    }
    memset(ctx, 0, sizeof(*ctx));
}

static float _linear_ramp(float current, float target, float progress)
{
    /* progress: 0.0 (start) to 1.0 (end) */
    return current + (target - current) * progress;
}

void ctrl_apply_event(ctrl_apply_ctx_t *ctx, const ctrl_event_t *evt,
                      const ctrl_mapping_t *mapping)
{
    if (!ctx || !evt || !mapping) return;
    if (evt->param_id >= ctx->param_count) return;
    
    ctrl_param_state_t *p = &ctx->params[evt->param_id];
    
    /* Scale event value [0..1] to parameter range [min..max] */
    float target_val = mapping->min_val + evt->value * (mapping->max_val - mapping->min_val);
    
    /* Handle mode */
    if (evt->mode == CTRL_MODE_TOGGLE) {
        target_val = (evt->value > 0.5f) ? mapping->max_val : mapping->min_val;
    }
    
    /* Clamp — handles both normal (min<max) and inverted (min>max) ranges */
    float lo = mapping->min_val < mapping->max_val ? mapping->min_val : mapping->max_val;
    float hi = mapping->min_val < mapping->max_val ? mapping->max_val : mapping->min_val;
    if (target_val < lo) target_val = lo;
    if (target_val > hi) target_val = hi;
    
    p->target_val = target_val;
    
    /* Compute ramp duration in samples */
    uint16_t ramp_ms = mapping->ramp_time_ms;
    if (ramp_ms == 0) {
        /* No ramping: apply immediately */
        p->current_val = target_val;
        p->is_ramping = 0;
        p->ramp_samples_left = 0;
    } else {
        /* Compute ramp rate */
        uint32_t ramp_samples = (ctx->sample_rate * ramp_ms) / 1000;
        if (ramp_samples == 0) ramp_samples = 1;
        
        p->ramp_samples_left = ramp_samples;
        p->ramp_rate = 1.0f / (float)ramp_samples;
        p->is_ramping = 1;
    }
}

float ctrl_apply_get(const ctrl_apply_ctx_t *ctx, uint32_t param_id)
{
    if (!ctx || param_id >= ctx->param_count) return 0.0f;
    return ctx->params[param_id].current_val;
}

float ctrl_apply_get_target(const ctrl_apply_ctx_t *ctx, uint32_t param_id)
{
    if (!ctx || param_id >= ctx->param_count) return 0.0f;
    return ctx->params[param_id].target_val;
}

void ctrl_apply_process_frame(ctrl_apply_ctx_t *ctx)
{
    if (!ctx) return;
    
    for (size_t i = 0; i < ctx->param_count; i++) {
        ctrl_param_state_t *p = &ctx->params[i];
        
        if (!p->is_ramping) continue;
        
        if (p->ramp_samples_left > ctx->frame_size) {
            /* Ramp continues through this frame */
            float progress = p->ramp_rate * (float)(ctx->frame_size);
            p->current_val = _linear_ramp(p->current_val, p->target_val, progress);
            p->ramp_samples_left -= ctx->frame_size;
        } else {
            /* Ramp ends within this frame */
            p->current_val = p->target_val;
            p->is_ramping = 0;
            p->ramp_samples_left = 0;
        }
    }
}

void ctrl_apply_set_immediate(ctrl_apply_ctx_t *ctx, uint32_t param_id, float val)
{
    if (!ctx || param_id >= ctx->param_count) return;
    
    ctx->params[param_id].current_val = val;
    ctx->params[param_id].target_val = val;
    ctx->params[param_id].is_ramping = 0;
}
