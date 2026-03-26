/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: tremolo2_private.h
 *
 * Code generated for Simulink model 'tremolo2'.
 *
 * Model version                  : 1.18
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Sat Feb  7 10:39:11 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef tremolo2_private_h_
#define tremolo2_private_h_
#include "rtwtypes.h"
#include "tremolo2_types.h"
#include "tremolo2.h"

/* Private macros used by the generated code to access rtModel */
#ifndef rtmSetTFinal
#define rtmSetTFinal(rtm, val)         ((rtm)->Timing.tFinal = (val))
#endif

#ifndef rtmSetTPtr
#define rtmSetTPtr(rtm, val)           ((rtm)->Timing.t = (val))
#endif

extern void pisound_in(SimStruct *rts);
extern void pisound_out(SimStruct *rts);

#endif                                 /* tremolo2_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
