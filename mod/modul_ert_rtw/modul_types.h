/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: modul_types.h
 *
 * Code generated for Simulink model 'modul'.
 *
 * Model version                  : 1.30
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Thu Mar  5 21:03:46 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef modul_types_h_
#define modul_types_h_
#include "rtwtypes.h"
#ifndef struct_tag_IYHrcOhzPNWR90aMOyW4RD
#define struct_tag_IYHrcOhzPNWR90aMOyW4RD

struct tag_IYHrcOhzPNWR90aMOyW4RD
{
  int32_T isInitialized;
  uint8_T Mode;
  real_T Delay;
  real_T g;
  real_T w;
  real_T L;
  real_T xBufL[96002];
  real_T xBufR[96002];
  real_T yBufL[96002];
  real_T yBufR[96002];
};

#endif                                 /* struct_tag_IYHrcOhzPNWR90aMOyW4RD */

#ifndef typedef_vcd_sys_modul_T
#define typedef_vcd_sys_modul_T

typedef struct tag_IYHrcOhzPNWR90aMOyW4RD vcd_sys_modul_T;

#endif                                 /* typedef_vcd_sys_modul_T */

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

#ifndef typedef_mix_sys_modul_T
#define typedef_mix_sys_modul_T

typedef struct tag_aOMAp9A5KTOGRyYUC88pSF mix_sys_modul_T;

#endif                                 /* typedef_mix_sys_modul_T */

#ifndef struct_tag_Z3LuZc3q26iyZDtciD4HdE
#define struct_tag_Z3LuZc3q26iyZDtciD4HdE

struct tag_Z3LuZc3q26iyZDtciD4HdE
{
  int32_T isInitialized;
  real_T Phase;
  real_T WaveTables[49152];
  real_T TablesSize[12];
};

#endif                                 /* struct_tag_Z3LuZc3q26iyZDtciD4HdE */

#ifndef typedef_vco_mono_sys_modul_T
#define typedef_vco_mono_sys_modul_T

typedef struct tag_Z3LuZc3q26iyZDtciD4HdE vco_mono_sys_modul_T;

#endif                                 /* typedef_vco_mono_sys_modul_T */

#ifndef struct_tag_kF0BkpWSBdBfLXK7ywl8DD
#define struct_tag_kF0BkpWSBdBfLXK7ywl8DD

struct tag_kF0BkpWSBdBfLXK7ywl8DD
{
  real_T z[2];
  real_T b[3];
  real_T a[3];
  uint8_T lastType;
  real_T lastFreq;
  real_T lastQ;
  real_T lastGain;
};

#endif                                 /* struct_tag_kF0BkpWSBdBfLXK7ywl8DD */

#ifndef typedef_vcf_mono_sys_modul_T
#define typedef_vcf_mono_sys_modul_T

typedef struct tag_kF0BkpWSBdBfLXK7ywl8DD vcf_mono_sys_modul_T;

#endif                                 /* typedef_vcf_mono_sys_modul_T */

/* Parameters (default storage) */
typedef struct P_modul_T_ P_modul_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_modul_T RT_MODEL_modul_T;

#endif                                 /* modul_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
