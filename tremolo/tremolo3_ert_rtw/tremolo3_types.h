/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: tremolo3_types.h
 *
 * Code generated for Simulink model 'tremolo3'.
 *
 * Model version                  : 1.53
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Wed Mar  4 14:45:10 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef tremolo3_types_h_
#define tremolo3_types_h_
#include "rtwtypes.h"
#ifndef struct_tag_ntBa79UdhDpc7EqZNC0xB
#define struct_tag_ntBa79UdhDpc7EqZNC0xB

struct tag_ntBa79UdhDpc7EqZNC0xB
{
  int32_T isInitialized;
};

#endif                                 /* struct_tag_ntBa79UdhDpc7EqZNC0xB */

#ifndef typedef_vca_sys_tremolo3_T
#define typedef_vca_sys_tremolo3_T

typedef struct tag_ntBa79UdhDpc7EqZNC0xB vca_sys_tremolo3_T;

#endif                                 /* typedef_vca_sys_tremolo3_T */

#ifndef struct_tag_ml4f7J7g4jNinCEHPR39EH
#define struct_tag_ml4f7J7g4jNinCEHPR39EH

struct tag_ml4f7J7g4jNinCEHPR39EH
{
  real_T Phase;
  real_T WaveTables[40960];
  real_T TablesSize[10];
};

#endif                                 /* struct_tag_ml4f7J7g4jNinCEHPR39EH */

#ifndef typedef_vco_sys_tremolo3_T
#define typedef_vco_sys_tremolo3_T

typedef struct tag_ml4f7J7g4jNinCEHPR39EH vco_sys_tremolo3_T;

#endif                                 /* typedef_vco_sys_tremolo3_T */

/* Parameters (default storage) */
typedef struct P_tremolo3_T_ P_tremolo3_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_tremolo3_T RT_MODEL_tremolo3_T;

#endif                                 /* tremolo3_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
