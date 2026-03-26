/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: filters_types.h
 *
 * Code generated for Simulink model 'filters'.
 *
 * Model version                  : 1.21
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Wed Mar  4 16:01:03 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef filters_types_h_
#define filters_types_h_
#include "rtwtypes.h"
#ifndef struct_tag_jQR5G4PJLSOgIv7k7gAEi
#define struct_tag_jQR5G4PJLSOgIv7k7gAEi

struct tag_jQR5G4PJLSOgIv7k7gAEi
{
  int32_T isInitialized;
  uint8_T FilterType;
  real_T Freq;
  real_T Q;
  real_T Gain;
  real_T zL[2];
  real_T zR[2];
  real_T b[3];
  real_T a[3];
  uint8_T lastFilterType;
  real_T lastFreq;
  real_T lastQ;
  real_T lastGain;
};

#endif                                 /* struct_tag_jQR5G4PJLSOgIv7k7gAEi */

#ifndef typedef_filters_sys_filters_T
#define typedef_filters_sys_filters_T

typedef struct tag_jQR5G4PJLSOgIv7k7gAEi filters_sys_filters_T;

#endif                                 /* typedef_filters_sys_filters_T */

/* Parameters (default storage) */
typedef struct P_filters_T_ P_filters_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_filters_T RT_MODEL_filters_T;

#endif                                 /* filters_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
