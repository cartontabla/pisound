#ifndef CONTROL_H
#define CONTROL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void control_init(void);
void control_audio_tick(void);
void control_shutdown(void);

/* Set a parameter by slot index (same as ctrl_config order).
 * Updates ctrl_apply + ctrl_in immediately so the ctrl_in→param bridge
 * reflects the new value and doesn't override a text-input change. */
void control_set_param(uint32_t param_id, float val);

#ifdef __cplusplus
}
#endif

#endif
