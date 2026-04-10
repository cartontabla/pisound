/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: leveller_data.c
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

#include "leveller.h"

/* Block parameters (default storage) */
P_leveller_T leveller_P = {
  /* Expression: 1e-12
   * Referenced by: '<Root>/ENV'
   */
  1.0E-12,

  /* Expression: 0.02
   * Referenced by: '<Root>/ENV'
   */
  0.02,

  /* Expression: 40
   * Referenced by: '<Root>/MAP'
   */
  40.0,

  /* Expression: -45
   * Referenced by: '<Root>/MAP'
   */
  -45.0,

  /* Expression: 1
   * Referenced by: '<Root>/MAP'
   */
  1.0,

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

  /* Expression: 1/(2^31-1)
   * Referenced by: '<Root>/Gain1'
   */
  4.6566128752457969E-10,

  /* Computed Parameter: SFunction_P1_Size
   * Referenced by: '<Root>/S-Function'
   */
  { 1.0, 1.0 },

  /* Expression: 48000
   * Referenced by: '<Root>/S-Function'
   */
  48000.0,

  /* Computed Parameter: SFunction_P2_Size
   * Referenced by: '<Root>/S-Function'
   */
  { 1.0, 1.0 },

  /* Expression: 128
   * Referenced by: '<Root>/S-Function'
   */
  128.0,

  /* Computed Parameter: SFunction_P3_Size
   * Referenced by: '<Root>/S-Function'
   */
  { 1.0, 1.0 },

  /* Expression: 6
   * Referenced by: '<Root>/S-Function'
   */
  6.0,

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

  /* Expression: uint8( 1 )
   * Referenced by: '<Root>/ENV'
   */
  1U,

  /* Expression: uint8(3)
   * Referenced by: '<Root>/MAP'
   */
  3U,

  /* Computed Parameter: Constant_Value
   * Referenced by: '<Root>/Constant'
   */
  2U,

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
