/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: reverb_types.h
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

#ifndef reverb_types_h_
#define reverb_types_h_
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

#ifndef typedef_vcd_sys_reverb_T
#define typedef_vcd_sys_reverb_T

typedef struct tag_IYHrcOhzPNWR90aMOyW4RD vcd_sys_reverb_T;

#endif                                 /* typedef_vcd_sys_reverb_T */

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

#ifndef typedef_mix_sys_reverb_T
#define typedef_mix_sys_reverb_T

typedef struct tag_aOMAp9A5KTOGRyYUC88pSF mix_sys_reverb_T;

#endif                                 /* typedef_mix_sys_reverb_T */

/* Parameters for system: '<S1>/MATLAB System' */
typedef struct P_MATLABSystem_reverb_T_ P_MATLABSystem_reverb_T;

/* Parameters for system: '<S4>/MATLAB System' */
typedef struct P_MATLABSystem_reverb_g_T_ P_MATLABSystem_reverb_g_T;

/* Parameters (default storage) */
typedef struct P_reverb_T_ P_reverb_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_reverb_T RT_MODEL_reverb_T;

#endif                                 /* reverb_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
