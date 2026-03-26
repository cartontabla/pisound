/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: dist_private.h
 *
 * Code generated for Simulink model 'dist'.
 *
 * Model version                  : 1.41
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Fri Mar  6 12:08:05 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef dist_private_h_
#define dist_private_h_
#include "rtwtypes.h"
#include "dist_types.h"
#include "dist.h"

/* Private macros used by the generated code to access rtModel */
#ifndef rtmSetTFinal
#define rtmSetTFinal(rtm, val)         ((rtm)->Timing.tFinal = (val))
#endif

#ifndef rtmSetTPtr
#define rtmSetTPtr(rtm, val)           ((rtm)->Timing.t = (val))
#endif

extern real_T rt_powd_snf(real_T u0, real_T u1);
extern real_T rt_roundd_snf(real_T u);
extern void pisound_in(SimStruct *rts);
extern void pisound_out(SimStruct *rts);

#endif                                 /* dist_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
