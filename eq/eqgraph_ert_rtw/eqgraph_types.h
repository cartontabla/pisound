/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: eqgraph_types.h
 *
 * Code generated for Simulink model 'eqgraph'.
 *
 * Model version                  : 1.33
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Thu Mar  5 08:09:52 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef eqgraph_types_h_
#define eqgraph_types_h_
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

#ifndef typedef_vcf_sys_eqgraph_T
#define typedef_vcf_sys_eqgraph_T

typedef struct tag_RD0JYP5UEjIz8MuyvMepfD vcf_sys_eqgraph_T;

#endif                                 /* typedef_vcf_sys_eqgraph_T */

#ifndef struct_tag_ntBa79UdhDpc7EqZNC0xB
#define struct_tag_ntBa79UdhDpc7EqZNC0xB

struct tag_ntBa79UdhDpc7EqZNC0xB
{
  int32_T isInitialized;
};

#endif                                 /* struct_tag_ntBa79UdhDpc7EqZNC0xB */

#ifndef typedef_vca_sys_eqgraph_T
#define typedef_vca_sys_eqgraph_T

typedef struct tag_ntBa79UdhDpc7EqZNC0xB vca_sys_eqgraph_T;

#endif                                 /* typedef_vca_sys_eqgraph_T */

/* Parameters (default storage) */
typedef struct P_eqgraph_T_ P_eqgraph_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_eqgraph_T RT_MODEL_eqgraph_T;

#endif                                 /* eqgraph_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
