/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: phaser_private.h
 *
 * Code generated for Simulink model 'phaser'.
 *
 * Model version                  : 1.29
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Fri Mar  6 08:42:11 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef phaser_private_h_
#define phaser_private_h_
#include "rtwtypes.h"
#include "phaser.h"
#include "phaser_types.h"

/* Private macros used by the generated code to access rtModel */
#ifndef rtmSetTFinal
#define rtmSetTFinal(rtm, val)         ((rtm)->Timing.tFinal = (val))
#endif

#ifndef rtmSetTPtr
#define rtmSetTPtr(rtm, val)           ((rtm)->Timing.t = (val))
#endif

extern real_T rt_powd_snf(real_T u0, real_T u1);
extern void pisound_in(SimStruct *rts);
extern void pisound_out(SimStruct *rts);
extern void phaser_MATLABSystem_Init(DW_MATLABSystem_phaser_T *localDW,
  P_MATLABSystem_phaser_T *localP);
extern void phaser_MATLABSystem(const real_T rtu_0[128], const real_T rtu_1[128],
  uint8_T rtu_2, const real_T rtu_3[128], real_T rtu_4, B_MATLABSystem_phaser_T *
  localB, DW_MATLABSystem_phaser_T *localDW, P_MATLABSystem_phaser_T *localP);
extern void phaser_MATLABSystem_Term(DW_MATLABSystem_phaser_T *localDW);

#endif                                 /* phaser_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
