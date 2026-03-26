/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: reverb_data.c
 *
 * Code generated for Simulink model 'reverb'.
 *
 * Model version                  : 1.32
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Thu Mar  5 19:41:23 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "reverb.h"

/* Block parameters (default storage) */
P_reverb_T reverb_P = {
  /* Variable: Mix_rt
   * Referenced by: '<Root>/Constant1'
   */
  0.5,

  /* Variable: RT60_rt
   * Referenced by: '<Root>/Constant2'
   */
  3.0,

  /* Variable: Time1_rt
   * Referenced by: '<S1>/Constant1'
   */
  0.03,

  /* Variable: Time2_rt
   * Referenced by: '<S6>/Constant1'
   */
  0.035,

  /* Variable: Time3_rt
   * Referenced by: '<S2>/Constant1'
   */
  0.043,

  /* Variable: Time4_rt
   * Referenced by: '<S3>/Constant1'
   */
  0.04,

  /* Expression: 0.01
   * Referenced by: '<Root>/MATLAB System'
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

  /* Expression: 0.25
   * Referenced by: '<S1>/Gain8'
   */
  0.25,

  /* Expression: 0.25
   * Referenced by: '<S6>/Gain8'
   */
  0.25,

  /* Expression: 0.25
   * Referenced by: '<S2>/Gain8'
   */
  0.25,

  /* Expression: 0.25
   * Referenced by: '<S3>/Gain8'
   */
  0.25,

  /* Expression: 2
   * Referenced by: '<S4>/Constant1'
   */
  2.0,

  /* Expression: 0.7
   * Referenced by: '<S4>/Constant2'
   */
  0.7,

  /* Expression: 0.25
   * Referenced by: '<S1>/Gain9'
   */
  0.25,

  /* Expression: 0.25
   * Referenced by: '<S6>/Gain9'
   */
  0.25,

  /* Expression: 0.25
   * Referenced by: '<S2>/Gain9'
   */
  0.25,

  /* Expression: 0.25
   * Referenced by: '<S3>/Gain9'
   */
  0.25,

  /* Expression: 0.5
   * Referenced by: '<S4>/Gain8'
   */
  0.5,

  /* Expression: 6
   * Referenced by: '<S5>/Constant1'
   */
  6.0,

  /* Expression: 0.007
   * Referenced by: '<S5>/Constant2'
   */
  0.007,

  /* Expression: 0.5
   * Referenced by: '<S4>/Gain9'
   */
  0.5,

  /* Expression: 0.5
   * Referenced by: '<S5>/Gain8'
   */
  0.5,

  /* Expression: 0.5
   * Referenced by: '<S5>/Gain9'
   */
  0.5,

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
   * Referenced by: '<Root>/MATLAB System'
   */
  false,

  /* Computed Parameter: Constant_Value
   * Referenced by: '<S1>/Constant'
   */
  2U,

  /* Computed Parameter: Constant_Value_g
   * Referenced by: '<S6>/Constant'
   */
  2U,

  /* Computed Parameter: Constant_Value_a
   * Referenced by: '<S2>/Constant'
   */
  2U,

  /* Computed Parameter: Constant_Value_av
   * Referenced by: '<S3>/Constant'
   */
  2U,

  /* Computed Parameter: Constant_Value_b
   * Referenced by: '<S4>/Constant'
   */
  3U,

  /* Computed Parameter: Constant_Value_m
   * Referenced by: '<S5>/Constant'
   */
  3U,

  /* Start of '<S6>/MATLAB System' */
  {
    /* Expression: 0.1
     * Referenced by: '<S6>/MATLAB System'
     */
    0.1,

    /* Expression: 0.5
     * Referenced by: '<S6>/MATLAB System'
     */
    0.5,

    /* Expression: uint8(1)
     * Referenced by: '<S6>/MATLAB System'
     */
    1U
  }
  ,

  /* End of '<S6>/MATLAB System' */

  /* Start of '<S5>/MATLAB System' */
  {
    /* Expression: 0.1
     * Referenced by: '<S5>/MATLAB System'
     */
    0.1,

    /* Expression: 0.5
     * Referenced by: '<S5>/MATLAB System'
     */
    0.5,

    /* Expression: uint8(1)
     * Referenced by: '<S5>/MATLAB System'
     */
    1U
  }
  ,

  /* End of '<S5>/MATLAB System' */

  /* Start of '<S4>/MATLAB System' */
  {
    /* Expression: 0.1
     * Referenced by: '<S4>/MATLAB System'
     */
    0.1,

    /* Expression: 0.5
     * Referenced by: '<S4>/MATLAB System'
     */
    0.5,

    /* Expression: uint8(1)
     * Referenced by: '<S4>/MATLAB System'
     */
    1U
  }
  ,

  /* End of '<S4>/MATLAB System' */

  /* Start of '<S3>/MATLAB System' */
  {
    /* Expression: 0.1
     * Referenced by: '<S3>/MATLAB System'
     */
    0.1,

    /* Expression: 0.5
     * Referenced by: '<S3>/MATLAB System'
     */
    0.5,

    /* Expression: uint8(1)
     * Referenced by: '<S3>/MATLAB System'
     */
    1U
  }
  ,

  /* End of '<S3>/MATLAB System' */

  /* Start of '<S2>/MATLAB System' */
  {
    /* Expression: 0.1
     * Referenced by: '<S2>/MATLAB System'
     */
    0.1,

    /* Expression: 0.5
     * Referenced by: '<S2>/MATLAB System'
     */
    0.5,

    /* Expression: uint8(1)
     * Referenced by: '<S2>/MATLAB System'
     */
    1U
  }
  ,

  /* End of '<S2>/MATLAB System' */

  /* Start of '<S1>/MATLAB System' */
  {
    /* Expression: 0.1
     * Referenced by: '<S1>/MATLAB System'
     */
    0.1,

    /* Expression: 0.5
     * Referenced by: '<S1>/MATLAB System'
     */
    0.5,

    /* Expression: uint8(1)
     * Referenced by: '<S1>/MATLAB System'
     */
    1U
  }
  /* End of '<S1>/MATLAB System' */
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
