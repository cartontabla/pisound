/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: vocoder_types.h
 *
 * Code generated for Simulink model 'vocoder'.
 *
 * Model version                  : 1.106
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Thu Mar 26 10:08:42 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef vocoder_types_h_
#define vocoder_types_h_
#include "rtwtypes.h"
#ifndef struct_tag_PQ3GGF1TFTTTDBfCOM3fcG
#define struct_tag_PQ3GGF1TFTTTDBfCOM3fcG

struct tag_PQ3GGF1TFTTTDBfCOM3fcG
{
  int32_T isInitialized;
  real_T gPrev_dB;
};

#endif                                 /* struct_tag_PQ3GGF1TFTTTDBfCOM3fcG */

#ifndef typedef_arf_sys_vocoder_T
#define typedef_arf_sys_vocoder_T

typedef struct tag_PQ3GGF1TFTTTDBfCOM3fcG arf_sys_vocoder_T;

#endif                                 /* typedef_arf_sys_vocoder_T */

#ifndef struct_tag_jkcCMuUeaGVtEREfG39NjE
#define struct_tag_jkcCMuUeaGVtEREfG39NjE

struct tag_jkcCMuUeaGVtEREfG39NjE
{
  int32_T isInitialized;
  uint8_T ModeDefault;
  real_T Eps;
  real_T TauRMS;
  real_T e_prev;
};

#endif                                 /* struct_tag_jkcCMuUeaGVtEREfG39NjE */

#ifndef typedef_env_mono_sys_vocoder_T
#define typedef_env_mono_sys_vocoder_T

typedef struct tag_jkcCMuUeaGVtEREfG39NjE env_mono_sys_vocoder_T;

#endif                                 /* typedef_env_mono_sys_vocoder_T */

#ifndef struct_tag_bSPHRZHMnmk80Wi7XGJvKB
#define struct_tag_bSPHRZHMnmk80Wi7XGJvKB

struct tag_bSPHRZHMnmk80Wi7XGJvKB
{
  int32_T isInitialized;
  uint8_T Mode;
  real_T Offset;
  real_T Threshold;
  real_T Slope;
};

#endif                                 /* struct_tag_bSPHRZHMnmk80Wi7XGJvKB */

#ifndef typedef_map_sys_vocoder_T
#define typedef_map_sys_vocoder_T

typedef struct tag_bSPHRZHMnmk80Wi7XGJvKB map_sys_vocoder_T;

#endif                                 /* typedef_map_sys_vocoder_T */

#ifndef struct_tag_QXr3RZQr8JiiqQXgsjGkmF
#define struct_tag_QXr3RZQr8JiiqQXgsjGkmF

struct tag_QXr3RZQr8JiiqQXgsjGkmF
{
  int32_T isInitialized;
  uint8_T Mode;
  uint8_T ForceGainEnable;
  real_T ForceGainValue;
};

#endif                                 /* struct_tag_QXr3RZQr8JiiqQXgsjGkmF */

#ifndef typedef_vca_mono_sys_vocoder_T
#define typedef_vca_mono_sys_vocoder_T

typedef struct tag_QXr3RZQr8JiiqQXgsjGkmF vca_mono_sys_vocoder_T;

#endif                                 /* typedef_vca_mono_sys_vocoder_T */

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

#ifndef typedef_mix_sys_vocoder_T
#define typedef_mix_sys_vocoder_T

typedef struct tag_aOMAp9A5KTOGRyYUC88pSF mix_sys_vocoder_T;

#endif                                 /* typedef_mix_sys_vocoder_T */

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

#ifndef typedef_vcf_sys_vocoder_T
#define typedef_vcf_sys_vocoder_T

typedef struct tag_RD0JYP5UEjIz8MuyvMepfD vcf_sys_vocoder_T;

#endif                                 /* typedef_vcf_sys_vocoder_T */

/* Parameters for system: '<Root>/Filter Bank' */
typedef struct P_CoreSubsys_vocoder_T_ P_CoreSubsys_vocoder_T;

/* Parameters (default storage) */
typedef struct P_vocoder_T_ P_vocoder_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_vocoder_T RT_MODEL_vocoder_T;

#endif                                 /* vocoder_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
