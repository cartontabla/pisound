/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: wah_types.h
 *
 * Code generated for Simulink model 'wah'.
 *
 * Model version                  : 1.54
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Wed Mar  4 20:01:26 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef wah_types_h_
#define wah_types_h_
#include "rtwtypes.h"
#ifndef struct_tag_ntBa79UdhDpc7EqZNC0xB
#define struct_tag_ntBa79UdhDpc7EqZNC0xB

struct tag_ntBa79UdhDpc7EqZNC0xB
{
  int32_T isInitialized;
};

#endif                                 /* struct_tag_ntBa79UdhDpc7EqZNC0xB */

#ifndef typedef_vca_sys_wah_T
#define typedef_vca_sys_wah_T

typedef struct tag_ntBa79UdhDpc7EqZNC0xB vca_sys_wah_T;

#endif                                 /* typedef_vca_sys_wah_T */

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

#ifndef typedef_vcf_sys_wah_T
#define typedef_vcf_sys_wah_T

typedef struct tag_RD0JYP5UEjIz8MuyvMepfD vcf_sys_wah_T;

#endif                                 /* typedef_vcf_sys_wah_T */

#ifndef struct_tag_ml4f7J7g4jNinCEHPR39EH
#define struct_tag_ml4f7J7g4jNinCEHPR39EH

struct tag_ml4f7J7g4jNinCEHPR39EH
{
  real_T Phase;
  real_T WaveTables[40960];
  real_T TablesSize[10];
};

#endif                                 /* struct_tag_ml4f7J7g4jNinCEHPR39EH */

#ifndef typedef_vco_sys_wah_T
#define typedef_vco_sys_wah_T

typedef struct tag_ml4f7J7g4jNinCEHPR39EH vco_sys_wah_T;

#endif                                 /* typedef_vco_sys_wah_T */

/* Parameters (default storage) */
typedef struct P_wah_T_ P_wah_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_wah_T RT_MODEL_wah_T;

#endif                                 /* wah_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
