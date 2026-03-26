/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: pitch_data.c
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

#include "pitch.h"

/* Block parameters (default storage) */
P_pitch_T pitch_P = {
  /* Variable: Semitone_rt
   * Referenced by: '<Root>/Constant1'
   */
  4.0,

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
   * Referenced by: '<Root>/Gain'
   */
  4.6566128752457969E-10,

  /* Expression: 0
   * Referenced by: '<Root>/Constant2'
   */
  0.0,

  /* Expression: 1/(2^31-1)
   * Referenced by: '<Root>/Gain1'
   */
  4.6566128752457969E-10,

  /* Expression: 0.005
   * Referenced by: '<Root>/Constant7'
   */
  0.005,

  /* Expression: 0.25
   * Referenced by: '<Root>/Constant10'
   */
  0.25,

  /* Expression: 1
   * Referenced by: '<Root>/Constant3'
   */
  1.0,

  /* Expression: 0
   * Referenced by: '<Root>/Constant4'
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
   * Referenced by: '<Root>/MIX'
   */
  false,

  /* Computed Parameter: Constant_Value
   * Referenced by: '<Root>/Constant'
   */
  0U,

  /* Computed Parameter: Constant5_Value
   * Referenced by: '<Root>/Constant5'
   */
  4U,

  /* Computed Parameter: Constant6_Value
   * Referenced by: '<Root>/Constant6'
   */
  1U,

  /* Start of '<Root>/VCD2' */
  {
    /* Expression: 0.1
     * Referenced by: '<Root>/VCD2'
     */
    0.1,

    /* Expression: 0.5
     * Referenced by: '<Root>/VCD2'
     */
    0.5,

    /* Expression: uint8(1)
     * Referenced by: '<Root>/VCD2'
     */
    1U
  }
  ,

  /* End of '<Root>/VCD2' */

  /* Start of '<Root>/VCD1' */
  {
    /* Expression: 0.1
     * Referenced by: '<Root>/VCD1'
     */
    0.1,

    /* Expression: 0.5
     * Referenced by: '<Root>/VCD1'
     */
    0.5,

    /* Expression: uint8(1)
     * Referenced by: '<Root>/VCD1'
     */
    1U
  }
  /* End of '<Root>/VCD1' */
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
