/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: eqgraph_private.h
 *
 * Code generated for Simulink model 'eqgraph'.
 *
 * Model version                  : 1.33
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Thu Mar  5 08:09:52 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef eqgraph_private_h_
#define eqgraph_private_h_
#include "rtwtypes.h"
#include "eqgraph.h"
#include "eqgraph_types.h"

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
extern void eqgraph_MATLABSystem_Init(DW_MATLABSystem_eqgraph_T *localDW);
extern void eqgraph_MATLABSystem(const real_T rtu_0[128], const real_T rtu_1[128],
  uint8_T rtu_2, real_T rtu_3, real_T rtu_4, real_T rtu_5,
  B_MATLABSystem_eqgraph_T *localB, DW_MATLABSystem_eqgraph_T *localDW);
extern void eqgraph_MATLABSystem_Term(DW_MATLABSystem_eqgraph_T *localDW);

#endif                                 /* eqgraph_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
