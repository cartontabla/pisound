/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: tremolo_types.h
 *
 * Code generated for Simulink model 'tremolo'.
 *
 * Model version                  : 1.18
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Wed Mar  4 14:22:38 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef tremolo_types_h_
#define tremolo_types_h_
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

#ifndef typedef_amp_sys_tremolo_T
#define typedef_amp_sys_tremolo_T

typedef struct tag_sqLSXqaXaxUmZt4JndiypG amp_sys_tremolo_T;

#endif                                 /* typedef_amp_sys_tremolo_T */

#ifndef struct_tag_xLqANSPUkF7HJqvUhElWE
#define struct_tag_xLqANSPUkF7HJqvUhElWE

struct tag_xLqANSPUkF7HJqvUhElWE
{
  int32_T isInitialized;
  real_T A;
  real_T F;
  real_T iPhase;
  uint8_T WaveSel;
  real_T Phase;
  real_T WaveTables[40960];
  real_T TablesSize[10];
};

#endif                                 /* struct_tag_xLqANSPUkF7HJqvUhElWE */

#ifndef typedef_osc_sys_tremolo_T
#define typedef_osc_sys_tremolo_T

typedef struct tag_xLqANSPUkF7HJqvUhElWE osc_sys_tremolo_T;

#endif                                 /* typedef_osc_sys_tremolo_T */

/* Parameters (default storage) */
typedef struct P_tremolo_T_ P_tremolo_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_tremolo_T RT_MODEL_tremolo_T;

#endif                                 /* tremolo_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
