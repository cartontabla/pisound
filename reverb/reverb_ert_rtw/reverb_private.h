/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: reverb_private.h
 *
 * Code generated for Simulink model 'reverb'.
 *
 * Model version                  : 1.32
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Thu Mar  5 19:41:23 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef reverb_private_h_
#define reverb_private_h_
#include "rtwtypes.h"
#include "reverb.h"
#include "reverb_types.h"

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
extern void reverb_MATLABFunction(real_T rtu_RT60, real_T rtu_D, real_T *rty_g);
extern void reverb_MATLABFunction1(real_T rtu_u, real_T rty_y[128]);
extern void reverb_MATLABSystem_Init(DW_MATLABSystem_reverb_T *localDW,
  P_MATLABSystem_reverb_T *localP);
extern void reverb_MATLABSystem(const real_T rtu_0[128], const real_T rtu_1[128],
  uint8_T rtu_2, const real_T rtu_3[128], real_T rtu_4, B_MATLABSystem_reverb_T *
  localB, DW_MATLABSystem_reverb_T *localDW, P_MATLABSystem_reverb_T *localP);
extern void reverb_MATLABSystem_k_Init(DW_MATLABSystem_reverb_f_T *localDW,
  P_MATLABSystem_reverb_g_T *localP);
extern void reverb_MATLABSystem_p(const real_T rtu_0[128], const real_T rtu_1
  [128], uint8_T rtu_2, real_T rtu_3, real_T rtu_4, B_MATLABSystem_reverb_c_T
  *localB, DW_MATLABSystem_reverb_f_T *localDW, P_MATLABSystem_reverb_g_T
  *localP);
extern void reverb_MATLABSystem_Term(DW_MATLABSystem_reverb_T *localDW);
extern void reverb_MATLABSystem_p_Term(DW_MATLABSystem_reverb_f_T *localDW);

#endif                                 /* reverb_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
