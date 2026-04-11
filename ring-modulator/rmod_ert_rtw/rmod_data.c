/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: rmod_data.c
 *
 * Code generated for Simulink model 'rmod'.
 *
 * Model version                  : 1.62
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Sat Apr 11 11:25:51 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "rmod.h"

/* Block parameters (default storage) */
P_rmod_T rmod_P = {
  /* Variable: WaveSel_rt
   * Referenced by: '<Root>/Constant10'
   */
  1U,

  /* Expression: 0.01
   * Referenced by: '<Root>/MIX'
   */
  0.01,

  /* Computed Parameter: ControlInput_P1_Size
   * Referenced by: '<Root>/Control Input'
   */
  { 1.0, 1.0 },

  /* Expression: 48000
   * Referenced by: '<Root>/Control Input'
   */
  48000.0,

  /* Computed Parameter: ControlInput_P2_Size
   * Referenced by: '<Root>/Control Input'
   */
  { 1.0, 1.0 },

  /* Expression: 128
   * Referenced by: '<Root>/Control Input'
   */
  128.0,

  /* Computed Parameter: ControlInput_P3_Size
   * Referenced by: '<Root>/Control Input'
   */
  { 1.0, 1.0 },

  /* Expression: 4
   * Referenced by: '<Root>/Control Input'
   */
  4.0,

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

  /* Expression: 1
   * Referenced by: '<Root>/Constant8'
   */
  1.0,

  /* Expression: 0
   * Referenced by: '<Root>/Constant11'
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

  /* Computed Parameter: Constant1_Value
   * Referenced by: '<Root>/Constant1'
   */
  0U,

  /* Computed Parameter: Constant12_Value
   * Referenced by: '<Root>/Constant12'
   */
  0U
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
