/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: pitch_types.h
 *
 * Code generated for Simulink model 'pitch'.
 *
 * Model version                  : 1.43
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Fri Mar  6 13:38:43 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef pitch_types_h_
#define pitch_types_h_
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

#ifndef typedef_vcd_sys_pitch_T
#define typedef_vcd_sys_pitch_T

typedef struct tag_IYHrcOhzPNWR90aMOyW4RD vcd_sys_pitch_T;

#endif                                 /* typedef_vcd_sys_pitch_T */

#ifndef struct_tag_t9RTy2PPtN6gtlroAtWrTF
#define struct_tag_t9RTy2PPtN6gtlroAtWrTF

struct tag_t9RTy2PPtN6gtlroAtWrTF
{
  int32_T isInitialized;
  real_T SmoothTime;
  boolean_T IndepInDB;
};

#endif                                 /* struct_tag_t9RTy2PPtN6gtlroAtWrTF */

#ifndef typedef_mix_sys_pitch_T
#define typedef_mix_sys_pitch_T

typedef struct tag_t9RTy2PPtN6gtlroAtWrTF mix_sys_pitch_T;

#endif                                 /* typedef_mix_sys_pitch_T */

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

#ifndef typedef_vco_mono_sys_pitch_T
#define typedef_vco_mono_sys_pitch_T

typedef struct tag_Z3LuZc3q26iyZDtciD4HdE vco_mono_sys_pitch_T;

#endif                                 /* typedef_vco_mono_sys_pitch_T */

/* Parameters for system: '<Root>/VCD1' */
typedef struct P_VCD1_pitch_T_ P_VCD1_pitch_T;

/* Parameters (default storage) */
typedef struct P_pitch_T_ P_pitch_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_pitch_T RT_MODEL_pitch_T;

#endif                                 /* pitch_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
