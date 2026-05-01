/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: bypasscross_private.h
 *
 * Code generated for Simulink model 'bypasscross'.
 *
 * Model version                  : 1.7
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Fri Apr 24 18:14:16 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef bypasscross_private_h_
#define bypasscross_private_h_
#include "rtwtypes.h"
#include "bypasscross_types.h"
#include "bypasscross.h"

/* Private macros used by the generated code to access rtModel */
#ifndef rtmSetTFinal
#define rtmSetTFinal(rtm, val)         ((rtm)->Timing.tFinal = (val))
#endif

#ifndef rtmSetTPtr
#define rtmSetTPtr(rtm, val)           ((rtm)->Timing.t = (val))
#endif

extern void pisound_in(SimStruct *rts);
extern void stream_in(SimStruct *rts);
extern void stream_out(SimStruct *rts);
extern void pisound_out(SimStruct *rts);

#endif                                 /* bypasscross_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
