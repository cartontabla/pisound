/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: doubling_private.h
 *
 * Code generated for Simulink model 'doubling'.
 *
 * Model version                  : 1.9
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Fri Apr 10 16:11:12 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-A
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef doubling_private_h_
#define doubling_private_h_
#include "rtwtypes.h"
#include "doubling_types.h"
#include "doubling.h"

/* Private macros used by the generated code to access rtModel */
#ifndef rtmSetTFinal
#define rtmSetTFinal(rtm, val)         ((rtm)->Timing.tFinal = (val))
#endif

#ifndef rtmSetTPtr
#define rtmSetTPtr(rtm, val)           ((rtm)->Timing.t = (val))
#endif

extern void ctrl_in(SimStruct *rts);
extern void pisound_in(SimStruct *rts);
extern void ctrl_out(SimStruct *rts);
extern void pisound_out(SimStruct *rts);

#endif                                 /* doubling_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
