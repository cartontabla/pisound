/*
 * ctrl_in.h - Control input S-Function interface (generic)
 *
 * Shared array between the control pipeline (control.c) and the
 * ctrl_in S-Function block inside the Simulink model.
 *
 * Flow:
 *   control_process_rx() → ctrl_in_set(idx, val) → g_ctrl_in_vals[]
 *                                                         ↓
 *                                              [ctrl_in S-Function]
 *                                                         ↓
 *                                              [Simulink model outputs]
 *
 * Slot index matches the param_id assigned by ctrl_map_load()
 * (order of lines in the config file, 0-based).
 */

#ifndef CTRL_IN_H
#define CTRL_IN_H

#ifdef __cplusplus
extern "C" {
#endif

#define CTRL_IN_MAX_PARAMS 20

/* Shared buffer: written by control.c, read by ctrl_in S-Function */
extern volatile float g_ctrl_in_vals[CTRL_IN_MAX_PARAMS];

/* Called once per audio frame by control_process_rx() */
void  ctrl_in_set(unsigned int idx, float val);

/* Optional: read back current smooth value (debug/metering) */
float ctrl_in_get(unsigned int idx);

#ifdef __cplusplus
}
#endif

#endif /* CTRL_IN_H */
