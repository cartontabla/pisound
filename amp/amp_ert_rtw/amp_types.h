/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: amp_types.h
 *
 * Code generated for Simulink model 'amp'.
 *
 * Model version                  : 1.10
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Tue Mar  3 20:32:49 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef amp_types_h_
#define amp_types_h_
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

#ifndef typedef_amp_sys_amp_T
#define typedef_amp_sys_amp_T

typedef struct tag_sqLSXqaXaxUmZt4JndiypG amp_sys_amp_T;

#endif                                 /* typedef_amp_sys_amp_T */

/* Parameters (default storage) */
typedef struct P_amp_T_ P_amp_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_amp_T RT_MODEL_amp_T;

#endif                                 /* amp_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
