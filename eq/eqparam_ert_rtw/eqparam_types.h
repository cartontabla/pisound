/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: eqparam_types.h
 *
 * Code generated for Simulink model 'eqparam'.
 *
 * Model version                  : 1.34
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Thu Mar  5 08:31:01 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef eqparam_types_h_
#define eqparam_types_h_
#include "rtwtypes.h"
#ifndef struct_tag_RD0JYP5UEjIz8MuyvMepfD
#define struct_tag_RD0JYP5UEjIz8MuyvMepfD

struct tag_RD0JYP5UEjIz8MuyvMepfD
{
  int32_T isInitialized;
  real_T zL[2];
  real_T zR[2];
  real_T b[3];
  real_T a[3];
  uint8_T lastType;
  real_T lastFreq;
  real_T lastQ;
  real_T lastGain;
};

#endif                                 /* struct_tag_RD0JYP5UEjIz8MuyvMepfD */

#ifndef typedef_vcf_sys_eqparam_T
#define typedef_vcf_sys_eqparam_T

typedef struct tag_RD0JYP5UEjIz8MuyvMepfD vcf_sys_eqparam_T;

#endif                                 /* typedef_vcf_sys_eqparam_T */

#ifndef struct_tag_ntBa79UdhDpc7EqZNC0xB
#define struct_tag_ntBa79UdhDpc7EqZNC0xB

struct tag_ntBa79UdhDpc7EqZNC0xB
{
  int32_T isInitialized;
};

#endif                                 /* struct_tag_ntBa79UdhDpc7EqZNC0xB */

#ifndef typedef_vca_sys_eqparam_T
#define typedef_vca_sys_eqparam_T

typedef struct tag_ntBa79UdhDpc7EqZNC0xB vca_sys_eqparam_T;

#endif                                 /* typedef_vca_sys_eqparam_T */

/* Parameters (default storage) */
typedef struct P_eqparam_T_ P_eqparam_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_eqparam_T RT_MODEL_eqparam_T;

#endif                                 /* eqparam_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
