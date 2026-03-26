/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: wah_data.c
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

#include "wah.h"

/* Block parameters (default storage) */
P_wah_T wah_P = {
  /* Variable: Depth_rt
   * Referenced by: '<Root>/Constant1'
   */
  0.0,

  /* Variable: FilterGain_rt
   * Referenced by: '<Root>/Constant3'
   */
  0.0,

  /* Variable: Fmax_rt
   * Referenced by: '<Root>/Constant5'
   */
  3000.0,

  /* Variable: Fmin_rt
   * Referenced by: '<Root>/Constant4'
   */
  300.0,

  /* Variable: Gain_rt
   * Referenced by: '<Root>/Constant7'
   */
  0.0,

  /* Variable: Q_rt
   * Referenced by: '<Root>/Constant2'
   */
  0.707,

  /* Variable: Rate_rt
   * Referenced by: '<Root>/Constant9'
   */
  1.0,

  /* Variable: iPhase_rt
   * Referenced by: '<Root>/Constant11'
   */
  0.0,

  /* Variable: Type_rt
   * Referenced by: '<Root>/Constant'
   */
  1U,

  /* Variable: WaveSel_rt
   * Referenced by: '<Root>/Constant10'
   */
  1U,

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

  /* Expression: 1
   * Referenced by: '<Root>/Constant8'
   */
  1.0,

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

  /* Computed Parameter: Constant12_Value
   * Referenced by: '<Root>/Constant12'
   */
  0U,

  /* Computed Parameter: Constant6_Value
   * Referenced by: '<Root>/Constant6'
   */
  1U
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
