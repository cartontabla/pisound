/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: rmod_private.h
 *
 * Code generated for Simulink model 'rmod'.
 *
 * Model version                  : 1.62
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Sat Apr 11 11:25:51 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef rmod_private_h_
#define rmod_private_h_
#include "rtwtypes.h"
#include "rmod_types.h"
#include "rmod.h"

/* Private macros used by the generated code to access rtModel */
#ifndef rtmSetTFinal
#define rtmSetTFinal(rtm, val)         ((rtm)->Timing.tFinal = (val))
#endif

#ifndef rtmSetTPtr
#define rtmSetTPtr(rtm, val)           ((rtm)->Timing.t = (val))
#endif

extern real_T rt_powd_snf(real_T u0, real_T u1);
extern void ctrl_in(SimStruct *rts);
extern void pisound_in(SimStruct *rts);
extern void pisound_out(SimStruct *rts);

#endif                                 /* rmod_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
