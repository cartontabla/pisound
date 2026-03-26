/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: vocoder_data.c
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

#include "vocoder.h"

/* Block parameters (default storage) */
P_vocoder_T vocoder_P = {
  /* Variable: Attack_rt
   * Referenced by: '<S1>/Constant3'
   */
  0.05,

  /* Variable: Mix_rt
   * Referenced by: '<Root>/Constant15'
   */
  1.0,

  /* Variable: Release_rt
   * Referenced by: '<S1>/Constant2'
   */
  0.002,

  /* Expression: 0.01
   * Referenced by: '<Root>/MIX'
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
   * Referenced by: '<Root>/Gain1'
   */
  4.6566128752457969E-10,

  /* Expression: 160
   * Referenced by: '<Root>/Constant1'
   */
  160.0,

  /* Expression: 7000
   * Referenced by: '<Root>/Constant2'
   */
  7000.0,

  /* Expression: 1/(2^31-1)
   * Referenced by: '<Root>/Gain'
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
   * Referenced by: '<Root>/MIX'
   */
  false,

  /* Start of '<S1>/CoreSubsys' */
  {
    /* Expression: 1e-12
     * Referenced by: '<S3>/ENV'
     */
    1.0E-12,

    /* Expression: 0.02
     * Referenced by: '<S3>/ENV'
     */
    0.02,

    /* Expression: 22
     * Referenced by: '<S3>/MAP'
     */
    22.0,

    /* Expression: -45
     * Referenced by: '<S3>/MAP'
     */
    -45.0,

    /* Expression: 2
     * Referenced by: '<S3>/MAP'
     */
    2.0,

    /* Expression: -20
     * Referenced by: '<S1>/VCA'
     */
    -20.0,

    /* Expression: uint8(2)
     * Referenced by: '<S3>/ENV'
     */
    2U,

    /* Expression: uint8(1)
     * Referenced by: '<S3>/MAP'
     */
    1U,

    /* Expression: uint8(1)
     * Referenced by: '<S1>/VCA'
     */
    1U,

    /* Expression: uint8(0)
     * Referenced by: '<S1>/VCA'
     */
    0U,

    /* Computed Parameter: Constant_Value
     * Referenced by: '<S3>/Constant'
     */
    2U
  }
  /* End of '<S1>/CoreSubsys' */
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
