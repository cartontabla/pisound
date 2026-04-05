/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: bypass2_private.h
 *
 * Code generated for Simulink model 'bypass2'.
 *
 * Model version                  : 1.6
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Sun Apr  5 11:08:17 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef bypass2_private_h_
#define bypass2_private_h_
#include "rtwtypes.h"
#include "bypass2_types.h"
#include "bypass2.h"

/* Private macros used by the generated code to access rtModel */
#ifndef rtmSetTFinal
#define rtmSetTFinal(rtm, val)         ((rtm)->Timing.tFinal = (val))
#endif

#ifndef rtmSetTPtr
#define rtmSetTPtr(rtm, val)           ((rtm)->Timing.t = (val))
#endif

extern void stream_in(SimStruct *rts);
extern void stream_out(SimStruct *rts);

#endif                                 /* bypass2_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
