/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: dyn_data.c
 *
 * Code generated for Simulink model 'dyn'.
 *
 * Model version                  : 1.22
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Fri Mar  6 08:57:56 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "dyn.h"

/* Block parameters (default storage) */
P_dyn_T dyn_P = {
  /* Variable: Attack_rt
   * Referenced by: '<Root>/Constant2'
   */
  0.02,

  /* Variable: Knee_rt
   * Referenced by: '<Root>/Constant5'
   */
  3.0,

  /* Variable: Makeup_rt
   * Referenced by: '<Root>/Constant6'
   */
  4.0,

  /* Variable: Mix_rt
   * Referenced by: '<Root>/Constant15'
   */
  0.9,

  /* Variable: Ratio_rt
   * Referenced by: '<Root>/Constant7'
   */
  3.0,

  /* Variable: Release_rt
   * Referenced by: '<Root>/Constant3'
   */
  0.06,

  /* Variable: Threshold_rt
   * Referenced by: '<Root>/Constant4'
   */
  -24.0,

  /* Expression: 1e-12
   * Referenced by: '<Root>/MATLAB System1'
   */
  1.0E-12,

  /* Expression: 0.02
   * Referenced by: '<Root>/MATLAB System1'
   */
  0.02,

  /* Expression: 1e-12
   * Referenced by: '<Root>/MATLAB System2'
   */
  1.0E-12,

  /* Expression: -80
   * Referenced by: '<Root>/MATLAB System2'
   */
  -80.0,

  /* Expression: 0.010
   * Referenced by: '<Root>/MATLAB System3'
   */
  0.01,

  /* Expression: 0.100
   * Referenced by: '<Root>/MATLAB System3'
   */
  0.1,

  /* Expression: 0.01
   * Referenced by: '<Root>/MATLAB System6'
   */
  0.01,

  /* Computed Parameter: PiSoundInput_P1_Size
   * Referenced by: '<Root>/PiSound Input'
   */
  { 1.0, 1.0 },

  /* Expression: 48000
   * Referenced by: '<Root>/PiSound Input'
   */
  48000.0,

  /* Computed Parameter: PiSoundInput_P2_Size
   * Referenced by: '<Root>/PiSound Input'
   */
  { 1.0, 1.0 },

  /* Expression: 128
   * Referenced by: '<Root>/PiSound Input'
   */
  128.0,

  /* Computed Parameter: PiSoundInput_P3_Size
   * Referenced by: '<Root>/PiSound Input'
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<Root>/PiSound Input'
   */
  0.0,

  /* Expression: 1/(2^31-1)
   * Referenced by: '<Root>/Gain'
   */
  4.6566128752457969E-10,

  /* Expression: 1/(2^31-1)
   * Referenced by: '<Root>/Gain1'
   */
  4.6566128752457969E-10,

  /* Expression: 2^31-1
   * Referenced by: '<Root>/Gain2'
   */
  2.147483647E+9,

  /* Expression: 2^31-1
   * Referenced by: '<Root>/Gain3'
   */
  2.147483647E+9,

  /* Computed Parameter: PiSoundOutput_P1_Size
   * Referenced by: '<Root>/PiSound Output'
   */
  { 1.0, 1.0 },

  /* Expression: 48000
   * Referenced by: '<Root>/PiSound Output'
   */
  48000.0,

  /* Computed Parameter: PiSoundOutput_P2_Size
   * Referenced by: '<Root>/PiSound Output'
   */
  { 1.0, 1.0 },

  /* Expression: 128
   * Referenced by: '<Root>/PiSound Output'
   */
  128.0,

  /* Computed Parameter: PiSoundOutput_P3_Size
   * Referenced by: '<Root>/PiSound Output'
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<Root>/PiSound Output'
   */
  0.0,

  /* Expression: false
   * Referenced by: '<Root>/MATLAB System6'
   */
  false,

  /* Expression: uint8( 1 )
   * Referenced by: '<Root>/MATLAB System1'
   */
  1U,

  /* Computed Parameter: Constant_Value
   * Referenced by: '<Root>/Constant'
   */
  3U,

  /* Computed Parameter: Constant1_Value
   * Referenced by: '<Root>/Constant1'
   */
  1U
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
