/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: dist3_types.h
 *
 * Code generated for Simulink model 'dist3'.
 *
 * Model version                  : 1.38
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Thu Feb 26 14:59:43 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef dist3_types_h_
#define dist3_types_h_
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

#ifndef typedef_vcf_sys_dist3_T
#define typedef_vcf_sys_dist3_T

typedef struct tag_RD0JYP5UEjIz8MuyvMepfD vcf_sys_dist3_T;

#endif                                 /* typedef_vcf_sys_dist3_T */

#ifndef struct_tag_ntBa79UdhDpc7EqZNC0xB
#define struct_tag_ntBa79UdhDpc7EqZNC0xB

struct tag_ntBa79UdhDpc7EqZNC0xB
{
  int32_T isInitialized;
};

#endif                                 /* struct_tag_ntBa79UdhDpc7EqZNC0xB */

#ifndef typedef_vca_sys_dist3_T
#define typedef_vca_sys_dist3_T

typedef struct tag_ntBa79UdhDpc7EqZNC0xB vca_sys_dist3_T;

#endif                                 /* typedef_vca_sys_dist3_T */

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

#ifndef typedef_mix_sys_dist3_T
#define typedef_mix_sys_dist3_T

typedef struct tag_aOMAp9A5KTOGRyYUC88pSF mix_sys_dist3_T;

#endif                                 /* typedef_mix_sys_dist3_T */

#ifndef struct_tag_jQS9WJPyorvTYWG4Nsu8ED
#define struct_tag_jQS9WJPyorvTYWG4Nsu8ED

struct tag_jQS9WJPyorvTYWG4Nsu8ED
{
  int32_T isInitialized;
  uint8_T ModeDefault;
  real_T AmountDefault;
  real_T AsymDefault;
  real_T Cheb3Default;
  real_T Cheb5Default;
  real_T XUser[257];
  real_T YUser[257];
  real_T MUser;
  real_T Eps;
  real_T zUp1L[14];
  real_T zUp1R[14];
  real_T zDn1L[14];
  real_T zDn1R[14];
  real_T zUp2L[14];
  real_T zUp2R[14];
  real_T zDn2L[14];
  real_T zDn2R[14];
};

#endif                                 /* struct_tag_jQS9WJPyorvTYWG4Nsu8ED */

#ifndef typedef_lut_ws_sys_dist3_T
#define typedef_lut_ws_sys_dist3_T

typedef struct tag_jQS9WJPyorvTYWG4Nsu8ED lut_ws_sys_dist3_T;

#endif                                 /* typedef_lut_ws_sys_dist3_T */

/* Parameters (default storage) */
typedef struct P_dist3_T_ P_dist3_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_dist3_T RT_MODEL_dist3_T;

#endif                                 /* dist3_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
