/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: delay_types.h
 *
 * Code generated for Simulink model 'delay'.
 *
 * Model version                  : 1.24
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Thu Mar  5 19:39:13 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef delay_types_h_
#define delay_types_h_
#include "rtwtypes.h"
#ifndef struct_tag_IYHrcOhzPNWR90aMOyW4RD
#define struct_tag_IYHrcOhzPNWR90aMOyW4RD

struct tag_IYHrcOhzPNWR90aMOyW4RD
{
  int32_T isInitialized;
  uint8_T Mode;
  real_T Delay;
  real_T g;
  real_T w;
  real_T L;
  real_T xBufL[96002];
  real_T xBufR[96002];
  real_T yBufL[96002];
  real_T yBufR[96002];
};

#endif                                 /* struct_tag_IYHrcOhzPNWR90aMOyW4RD */

#ifndef typedef_vcd_sys_delay_T
#define typedef_vcd_sys_delay_T

typedef struct tag_IYHrcOhzPNWR90aMOyW4RD vcd_sys_delay_T;

#endif                                 /* typedef_vcd_sys_delay_T */

/* Parameters (default storage) */
typedef struct P_delay_T_ P_delay_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_delay_T RT_MODEL_delay_T;

#endif                                 /* delay_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
