/*
 * ctrl_out.h - Control output S-Function interface (generic)
 *
 * Shared array between the ctrl_out S-Function block inside the Simulink
 * model and the control pipeline (control.c) for TX feedback.
 *
 * Flow:
 *   [Simulink model signals]
 *          ↓
 *   [ctrl_out S-Function] → g_ctrl_out_vals[]
 *                                  ↓
 *                       control_process_tx() → ctrl_out_get(idx) → MIDI/OSC out
 *
 * Typical use: metering, parameter echo-back to controller LEDs/displays.
 * For the doubling cobaya: not strictly required yet (TX is future).
 */

#ifndef CTRL_OUT_H
#define CTRL_OUT_H

#ifdef __cplusplus
extern "C" {
#endif

#define CTRL_OUT_MAX_MON 20

/* Shared buffer: written by ctrl_out S-Function, read by control_process_tx() */
extern volatile float g_ctrl_out_vals[CTRL_OUT_MAX_MON];

/* Called by control_process_tx() to read model monitoring outputs */
float ctrl_out_get(unsigned int idx);

#ifdef __cplusplus
}
#endif

#endif /* CTRL_OUT_H */
