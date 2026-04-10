/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: leveller_types.h
 *
 * Code generated for Simulink model 'leveller'.
 *
 * Model version                  : 1.31
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Fri Apr 10 21:03:07 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef leveller_types_h_
#define leveller_types_h_
#include "rtwtypes.h"
#ifndef struct_tag_PQ3GGF1TFTTTDBfCOM3fcG
#define struct_tag_PQ3GGF1TFTTTDBfCOM3fcG

struct tag_PQ3GGF1TFTTTDBfCOM3fcG
{
  int32_T isInitialized;
  real_T gPrev_dB;
};

#endif                                 /* struct_tag_PQ3GGF1TFTTTDBfCOM3fcG */

#ifndef typedef_arf_sys_leveller_T
#define typedef_arf_sys_leveller_T

typedef struct tag_PQ3GGF1TFTTTDBfCOM3fcG arf_sys_leveller_T;

#endif                                 /* typedef_arf_sys_leveller_T */

#ifndef struct_tag_jr3P86wOJ34kDGHX5d0RFD
#define struct_tag_jr3P86wOJ34kDGHX5d0RFD

struct tag_jr3P86wOJ34kDGHX5d0RFD
{
  int32_T isInitialized;
  uint8_T ModeDefault;
  real_T Eps;
  real_T TauRMS;
  real_T e_prev;
};

#endif                                 /* struct_tag_jr3P86wOJ34kDGHX5d0RFD */

#ifndef typedef_env_sys_leveller_T
#define typedef_env_sys_leveller_T

typedef struct tag_jr3P86wOJ34kDGHX5d0RFD env_sys_leveller_T;

#endif                                 /* typedef_env_sys_leveller_T */

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

#ifndef typedef_map_sys_leveller_T
#define typedef_map_sys_leveller_T

typedef struct tag_bSPHRZHMnmk80Wi7XGJvKB map_sys_leveller_T;

#endif                                 /* typedef_map_sys_leveller_T */

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

#ifndef typedef_mix_sys_leveller_T
#define typedef_mix_sys_leveller_T

typedef struct tag_aOMAp9A5KTOGRyYUC88pSF mix_sys_leveller_T;

#endif                                 /* typedef_mix_sys_leveller_T */

#ifndef struct_tag_ntBa79UdhDpc7EqZNC0xB
#define struct_tag_ntBa79UdhDpc7EqZNC0xB

struct tag_ntBa79UdhDpc7EqZNC0xB
{
  int32_T isInitialized;
};

#endif                                 /* struct_tag_ntBa79UdhDpc7EqZNC0xB */

#ifndef typedef_vca_sys_leveller_T
#define typedef_vca_sys_leveller_T

typedef struct tag_ntBa79UdhDpc7EqZNC0xB vca_sys_leveller_T;

#endif                                 /* typedef_vca_sys_leveller_T */

/* Parameters (default storage) */
typedef struct P_leveller_T_ P_leveller_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_leveller_T RT_MODEL_leveller_T;

#endif                                 /* leveller_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
