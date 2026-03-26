/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: tremolo2_types.h
 *
 * Code generated for Simulink model 'tremolo2'.
 *
 * Model version                  : 1.18
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Sat Feb  7 10:39:11 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef tremolo2_types_h_
#define tremolo2_types_h_
#include "rtwtypes.h"
#ifndef struct_tag_sqLSXqaXaxUmZt4JndiypG
#define struct_tag_sqLSXqaXaxUmZt4JndiypG

struct tag_sqLSXqaXaxUmZt4JndiypG
{
  int32_T isInitialized;
  real_T Gain;
  boolean_T Bypass;
};

#endif                                 /* struct_tag_sqLSXqaXaxUmZt4JndiypG */

#ifndef typedef_amp_sys_tremolo2_T
#define typedef_amp_sys_tremolo2_T

typedef struct tag_sqLSXqaXaxUmZt4JndiypG amp_sys_tremolo2_T;

#endif                                 /* typedef_amp_sys_tremolo2_T */

#ifndef struct_tag_NdjiXRAt8kSNECJBA01N3G
#define struct_tag_NdjiXRAt8kSNECJBA01N3G

struct tag_NdjiXRAt8kSNECJBA01N3G
{
  int32_T isInitialized;
  real_T Rate;
  real_T Depth;
  real_T StereoPhase;
  uint8_T WaveSel;
  real_T Phase;
};

#endif                                 /* struct_tag_NdjiXRAt8kSNECJBA01N3G */

#ifndef typedef_lfo_sys_tremolo2_T
#define typedef_lfo_sys_tremolo2_T

typedef struct tag_NdjiXRAt8kSNECJBA01N3G lfo_sys_tremolo2_T;

#endif                                 /* typedef_lfo_sys_tremolo2_T */

/* Parameters (default storage) */
typedef struct P_tremolo2_T_ P_tremolo2_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_tremolo2_T RT_MODEL_tremolo2_T;

#endif                                 /* tremolo2_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
