/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: phaser_data.c
 *
 * Code generated for Simulink model 'phaser'.
 *
 * Model version                  : 1.29
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Fri Mar  6 08:42:11 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "phaser.h"

/* Block parameters (default storage) */
P_phaser_T phaser_P = {
  /* Variable: Depth_rt
   * Referenced by: '<Root>/Gain4'
   */
  0.3,

  /* Variable: Dry_rt
   * Referenced by: '<Root>/Constant8'
   */
  0.7,

  /* Variable: Feedback_rt
   * Referenced by:
   *   '<Root>/Gain8'
   *   '<Root>/Gain9'
   */
  0.5,

  /* Variable: Freq_rt
   * Referenced by: '<Root>/Constant1'
   */
  1.0,

  /* Variable: Repeats_rt
   * Referenced by: '<Root>/Constant2'
   */
  0.9,

  /* Variable: Time_rt
   * Referenced by: '<Root>/Constant7'
   */
  0.5,

  /* Variable: Wet_rt
   * Referenced by: '<Root>/Constant9'
   */
  0.7,

  /* Expression: 0.01
   * Referenced by: '<Root>/MATLAB System2'
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

  /* Expression: 0.1
   * Referenced by: '<Root>/Constant11'
   */
  0.1,

  /* Expression: 0.707
   * Referenced by: '<Root>/Constant12'
   */
  0.707,

  /* Expression: 0
   * Referenced by: '<Root>/Constant13'
   */
  0.0,

  /* Expression: 1
   * Referenced by: '<Root>/Constant14'
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<Root>/Constant3'
   */
  1.0,

  /* Expression: 0
   * Referenced by: '<Root>/Constant4'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<Root>/Unit Delay1'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<Root>/Unit Delay'
   */
  0.0,

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
   * Referenced by: '<Root>/MATLAB System2'
   */
  false,

  /* Computed Parameter: Constant_Value
   * Referenced by: '<Root>/Constant'
   */
  3U,

  /* Computed Parameter: Constant10_Value
   * Referenced by: '<Root>/Constant10'
   */
  1U,

  /* Computed Parameter: Constant5_Value
   * Referenced by: '<Root>/Constant5'
   */
  1U,

  /* Computed Parameter: Constant6_Value
   * Referenced by: '<Root>/Constant6'
   */
  0U,

  /* Start of '<S1>/MATLAB System3' */
  {
    /* Expression: 0.1
     * Referenced by: '<S1>/MATLAB System3'
     */
    0.1,

    /* Expression: 0.5
     * Referenced by: '<S1>/MATLAB System3'
     */
    0.5,

    /* Expression: uint8(1)
     * Referenced by: '<S1>/MATLAB System3'
     */
    1U
  }
  ,

  /* End of '<S1>/MATLAB System3' */

  /* Start of '<S1>/MATLAB System2' */
  {
    /* Expression: 0.1
     * Referenced by: '<S1>/MATLAB System2'
     */
    0.1,

    /* Expression: 0.5
     * Referenced by: '<S1>/MATLAB System2'
     */
    0.5,

    /* Expression: uint8(1)
     * Referenced by: '<S1>/MATLAB System2'
     */
    1U
  }
  ,

  /* End of '<S1>/MATLAB System2' */

  /* Start of '<S1>/MATLAB System1' */
  {
    /* Expression: 0.1
     * Referenced by: '<S1>/MATLAB System1'
     */
    0.1,

    /* Expression: 0.5
     * Referenced by: '<S1>/MATLAB System1'
     */
    0.5,

    /* Expression: uint8(1)
     * Referenced by: '<S1>/MATLAB System1'
     */
    1U
  }
  ,

  /* End of '<S1>/MATLAB System1' */

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
