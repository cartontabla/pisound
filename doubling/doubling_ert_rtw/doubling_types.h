/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: doubling_types.h
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

#ifndef doubling_types_h_
#define doubling_types_h_
#include "rtwtypes.h"
#ifndef struct_tag_aOMAp9A5KTOGRyYUC88pSF
#define struct_tag_aOMAp9A5KTOGRyYUC88pSF

struct tag_aOMAp9A5KTOGRyYUC88pSF
{
  int32_T isInitialized;
  real_T SmoothTime;
  boolean_T IndepInDB;
  real_T gd1;
  real_T gd2;
  real_T gw1;
  real_T gw2;
  real_T a;
};

#endif                                 /* struct_tag_aOMAp9A5KTOGRyYUC88pSF */

#ifndef typedef_mix_sys_doubling_T
#define typedef_mix_sys_doubling_T

typedef struct tag_aOMAp9A5KTOGRyYUC88pSF mix_sys_doubling_T;

#endif                                 /* typedef_mix_sys_doubling_T */

#ifndef struct_tag_ixxSs1plL4L3ztYMyBWPeG
#define struct_tag_ixxSs1plL4L3ztYMyBWPeG

struct tag_ixxSs1plL4L3ztYMyBWPeG
{
  int32_T isInitialized;
  uint8_T Mode;
  real_T Delay;
  real_T g;
  real_T w;
  real_T L;
  real_T xBufL[7202];
  real_T xBufR[7202];
  real_T yBufL[7202];
  real_T yBufR[7202];
};

#endif                                 /* struct_tag_ixxSs1plL4L3ztYMyBWPeG */

#ifndef typedef_vcd_sys_doubling_T
#define typedef_vcd_sys_doubling_T

typedef struct tag_ixxSs1plL4L3ztYMyBWPeG vcd_sys_doubling_T;

#endif                                 /* typedef_vcd_sys_doubling_T */

#ifndef struct_tag_Z3LuZc3q26iyZDtciD4HdE
#define struct_tag_Z3LuZc3q26iyZDtciD4HdE

struct tag_Z3LuZc3q26iyZDtciD4HdE
{
  int32_T isInitialized;
  real_T Phase;
  real_T WaveTables[49152];
  real_T TablesSize[12];
};

#endif                                 /* struct_tag_Z3LuZc3q26iyZDtciD4HdE */

#ifndef typedef_vco_mono_sys_doubling_T
#define typedef_vco_mono_sys_doubling_T

typedef struct tag_Z3LuZc3q26iyZDtciD4HdE vco_mono_sys_doubling_T;

#endif                                 /* typedef_vco_mono_sys_doubling_T */

/* Forward declaration for rtModel */
typedef struct tag_RTM_doubling_T RT_MODEL_doubling_T;

#endif                                 /* doubling_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
