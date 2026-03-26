/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: osc_types.h
 *
 * Code generated for Simulink model 'osc'.
 *
 * Model version                  : 1.30
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Wed Mar  4 14:05:05 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef osc_types_h_
#define osc_types_h_
#include "rtwtypes.h"
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

#ifndef typedef_osc_sys_osc_T
#define typedef_osc_sys_osc_T

typedef struct tag_xLqANSPUkF7HJqvUhElWE osc_sys_osc_T;

#endif                                 /* typedef_osc_sys_osc_T */

/* Parameters (default storage) */
typedef struct P_osc_T_ P_osc_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_osc_T RT_MODEL_osc_T;

#endif                                 /* osc_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
