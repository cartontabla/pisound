/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: eqgraph.c
 *
 * Code generated for Simulink model 'eqgraph'.
 *
 * Model version                  : 1.33
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Thu Mar  5 08:09:52 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "eqgraph.h"
#include "rtwtypes.h"
#include "eqgraph_private.h"
#include "rt_nonfinite.h"
#include <string.h>
#include <math.h>

/* Block signals (default storage) */
B_eqgraph_T eqgraph_B;

/* Block states (default storage) */
DW_eqgraph_T eqgraph_DW;

/* Real-time model */
static RT_MODEL_eqgraph_T eqgraph_M_;
RT_MODEL_eqgraph_T *const eqgraph_M = &eqgraph_M_;

/* Forward declaration for local functions */
static void eqgraph_filter(real_T b[3], real_T a[3], const real_T x[128], const
  real_T zi[2], real_T y[128], real_T zf[2]);
static void eqgraph_filter(real_T b[3], real_T a[3], const real_T x[128], const
  real_T zi[2], real_T y[128], real_T zf[2])
{
  int32_T b_j;
  int32_T e_k;
  int32_T naxpy;

  /* Start for MATLABSystem: '<Root>/MATLAB System' */
  if ((!rtIsInf(a[0])) && (!rtIsNaN(a[0])) && (!(a[0] == 0.0)) && (a[0] != 1.0))
  {
    b[0] /= a[0];
    b[1] /= a[0];
    b[2] /= a[0];
    a[1] /= a[0];
    a[2] /= a[0];
    a[0] = 1.0;
  }

  zf[0] = 0.0;
  y[0] = zi[0];
  zf[1] = 0.0;
  y[1] = zi[1];
  memset(&y[2], 0, 126U * sizeof(real_T));
  for (e_k = 0; e_k < 128; e_k++) {
    real_T as;
    int32_T y_tmp;

    /* Start for MATLABSystem: '<Root>/MATLAB System' */
    if (128 - e_k <= 3) {
      naxpy = 128 - e_k;
    } else {
      naxpy = 3;
    }

    for (b_j = 0; b_j < naxpy; b_j++) {
      y_tmp = e_k + b_j;

      /* Start for MATLABSystem: '<Root>/MATLAB System' */
      y[y_tmp] += x[e_k] * b[b_j];
    }

    /* Start for MATLABSystem: '<Root>/MATLAB System' */
    if (127 - e_k <= 2) {
      naxpy = 127 - e_k;
    } else {
      naxpy = 2;
    }

    as = -y[e_k];
    for (b_j = 0; b_j < naxpy; b_j++) {
      y_tmp = (b_j + e_k) + 1;
      y[y_tmp] += a[b_j + 1] * as;
    }
  }

  for (e_k = 0; e_k < 2; e_k++) {
    for (naxpy = 0; naxpy <= e_k; naxpy++) {
      /* Start for MATLABSystem: '<Root>/MATLAB System' */
      zf[naxpy] += b[(naxpy - e_k) + 2] * x[e_k + 126];
    }
  }

  for (e_k = 0; e_k < 2; e_k++) {
    for (naxpy = 0; naxpy <= e_k; naxpy++) {
      /* Start for MATLABSystem: '<Root>/MATLAB System' */
      zf[naxpy] += a[(naxpy - e_k) + 2] * -y[e_k + 126];
    }
  }
}

real_T rt_powd_snf(real_T u0, real_T u1)
{
  real_T y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else {
    real_T tmp;
    real_T tmp_0;
    tmp = fabs(u0);
    tmp_0 = fabs(u1);
    if (rtIsInf(u1)) {
      if (tmp == 1.0) {
        y = 1.0;
      } else if (tmp > 1.0) {
        if (u1 > 0.0) {
          y = (rtInf);
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = (rtInf);
      }
    } else if (tmp_0 == 0.0) {
      y = 1.0;
    } else if (tmp_0 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = (rtNaN);
    } else {
      y = pow(u0, u1);
    }
  }

  return y;
}

/* System initialize for atomic system: */
void eqgraph_MATLABSystem_Init(DW_MATLABSystem_eqgraph_T *localDW)
{
  /* Start for MATLABSystem: '<Root>/MATLAB System' */
  localDW->objisempty = true;
  localDW->obj.isInitialized = 1;

  /*  arranca en bypass */
  localDW->obj.b[0] = 1.0;
  localDW->obj.a[0] = 1.0;
  localDW->obj.b[1] = 0.0;
  localDW->obj.a[1] = 0.0;
  localDW->obj.b[2] = 0.0;
  localDW->obj.a[2] = 0.0;

  /*  fuerza recálculo en el primer frame */
  localDW->obj.lastType = MAX_uint8_T;
  localDW->obj.lastFreq = (rtNaN);
  localDW->obj.lastQ = (rtNaN);
  localDW->obj.lastGain = (rtNaN);

  /* InitializeConditions for MATLABSystem: '<Root>/MATLAB System' */
  localDW->obj.zL[0] = 0.0;
  localDW->obj.zR[0] = 0.0;
  localDW->obj.zL[1] = 0.0;
  localDW->obj.zR[1] = 0.0;
}

/* Output and update for atomic system: */
void eqgraph_MATLABSystem(const real_T rtu_0[128], const real_T rtu_1[128],
  uint8_T rtu_2, real_T rtu_3, real_T rtu_4, real_T rtu_5,
  B_MATLABSystem_eqgraph_T *localB, DW_MATLABSystem_eqgraph_T *localDW)
{
  real_T rtu_0_0[128];
  real_T tmp[3];
  real_T tmp_0[3];
  real_T b[2];
  real_T A;
  real_T G;
  real_T Q;
  real_T a0;
  real_T a0_tmp;
  real_T a0_tmp_0;
  real_T f;
  real_T p;
  real_T q;
  real_T sqrtA;
  real_T wc;
  int32_T t;

  /* MATLABSystem: '<Root>/MATLAB System' */
  /*  acepta double y castea */
  if ((rtu_2 != localDW->obj.lastType) || (rtu_3 != localDW->obj.lastFreq) ||
      (rtu_4 != localDW->obj.lastQ) || (rtu_5 != localDW->obj.lastGain)) {
    f = rtu_3;
    Q = rtu_4;
    G = rtu_5;
    t = rtu_2;
    if (rtIsInf(rtu_3) || rtIsNaN(rtu_3)) {
      f = 1000.0;
    }

    if (rtIsInf(rtu_4) || rtIsNaN(rtu_4)) {
      Q = 0.707;
    }

    if (rtIsInf(rtu_5) || rtIsNaN(rtu_5)) {
      G = 0.0;
    }

    /*  límites prácticos */
    if (f < 0.1) {
      f = 0.1;
    }

    if (f > 23952.0) {
      f = 23952.0;
    }

    if (Q < 0.001) {
      Q = 0.001;
    }

    wc = f / 48000.0 * 6.2831853071795862;
    if (rtu_2 < 1) {
      t = 1;
    } else if (rtu_2 > 7) {
      t = 7;
    }

    localDW->obj.b[0] = 1.0;
    localDW->obj.a[0] = 1.0;
    localDW->obj.b[1] = 0.0;
    localDW->obj.a[1] = 0.0;
    localDW->obj.b[2] = 0.0;
    localDW->obj.a[2] = 0.0;
    switch ((uint8_T)t) {
     case 1U:
      /*  lowpass */
      /* Cálculo de los coefifientes de un Filtro Paso-Bajo de 2º orden */
      /* Parámetos de entrada:  */
      /* wo = frecuencia de corte a -3dB (en radianes) */
      /* Parametros de salida: */
      /* Coeficientes [b,a] de un filtro IIR de segundo orden */
      p = cos(wc);
      G = 1.4142135623730951 * sin(wc);

      /* Normalización de coeficientes */
      /* fvtool(b,a); */
      f = (1.0 - p) / (G + 2.0);
      localDW->obj.b[0] = f;
      localDW->obj.b[1] = (1.0 - p) * 2.0 / (G + 2.0);
      localDW->obj.b[2] = f;
      localDW->obj.a[0] = (G + 2.0) / (G + 2.0);
      localDW->obj.a[1] = -4.0 * p / (G + 2.0);
      localDW->obj.a[2] = (2.0 - G) / (G + 2.0);
      break;

     case 2U:
      /*  highpass */
      /* Cálculo de los coefifientes de un Filtro Paso-Alto de 2º orden */
      /* Parámetos de entrada:  */
      /* wo = frecuencia de corte a -3dB (en radianes) */
      /* Parametros de salida: */
      /* Coeficientes [b,a] de un filtro IIR de segundo orden */
      p = cos(wc);

      /*  Coeficientes del ecualizador */
      G = 1.4142135623730951 * sin(wc);

      /* Normalización de coeficientes */
      /*  */
      /* fvtool(b,a); */
      f = (p + 1.0) / (G + 2.0);
      localDW->obj.b[0] = f;
      localDW->obj.b[1] = (p + 1.0) * -2.0 / (G + 2.0);
      localDW->obj.b[2] = f;
      localDW->obj.a[0] = (G + 2.0) / (G + 2.0);
      localDW->obj.a[1] = -4.0 * p / (G + 2.0);
      localDW->obj.a[2] = (2.0 - G) / (G + 2.0);
      break;

     case 3U:
      /*  bandpass (0 dB peak) */
      /* Cálculo de los coefifientes de un Filtro Paso-Banda 2º orden */
      /* Parámetos de entrada:  */
      /* wo = frecuencia central (en radianes) */
      /* Q = factor de calidad */
      /*  */
      /* Parametros de salida: */
      /* Coeficientes [b,a] de un filtro IIR de segundo orden */
      G = sin(wc) / Q;

      /*  <-- Q aquí */
      /*  Coeficientes del ecualizador */
      /* Normalización de coeficientes */
      /* fvtool(b,a); */
      localDW->obj.b[0] = G / (G + 2.0);
      localDW->obj.b[1] = 0.0 / (G + 2.0);
      localDW->obj.b[2] = -G / (G + 2.0);
      localDW->obj.a[0] = (G + 2.0) / (G + 2.0);
      localDW->obj.a[1] = -4.0 * cos(wc) / (G + 2.0);
      localDW->obj.a[2] = (2.0 - G) / (G + 2.0);
      break;

     case 4U:
      /*  bandreject / notch */
      /* Cálculo de los coefifientes de un Filtro Paso-Banda 2º orden */
      /* Parámetos de entrada:  */
      /* wo = frecuencia central (en radianes) */
      /* Q = factor de calidad */
      /*  */
      /* Parametros de salida: */
      /* Coeficientes [b,a] de un filtro IIR de segundo orden */
      /*  Coeficientes del ecualizador */
      G = sin(wc) / Q;

      /* Normalización de coeficientes */
      /* fvtool(b,a); */
      f = 2.0 / (G + 2.0);
      localDW->obj.b[0] = f;
      wc = -4.0 * cos(wc) / (G + 2.0);
      localDW->obj.b[1] = wc;
      localDW->obj.b[2] = f;
      localDW->obj.a[0] = (G + 2.0) / (G + 2.0);
      localDW->obj.a[1] = wc;
      localDW->obj.a[2] = (2.0 - G) / (G + 2.0);
      break;

     case 5U:
      /*  lowshelf (RBJ, usando Q como S) */
      /* Cálculo de los coefifientes de un Filtro Paso-Banda 2º orden */
      /* Parámetos de entrada:  */
      /* wo = frecuencia central (en radianes) */
      /* Q = factor de calidad */
      /*  */
      /* Parametros de salida: */
      /* Coeficientes [b,a] de un filtro IIR de segundo orden */
      A = rt_powd_snf(10.0, G / 40.0);
      sqrtA = sqrt(A);
      p = cos(wc);
      q = sin(wc);

      /*  Coeficientes del ecualizador */
      G = 1.0 / A;
      a0_tmp = (1.0 - G) * p + (G + 1.0);
      a0_tmp_0 = 1.4142135623730951 * q / sqrtA;
      a0 = a0_tmp + a0_tmp_0;

      /* Normalización de coeficientes */
      /* fvtool(b,a); */
      f = (1.0 - A) * p + (A + 1.0);
      wc = 1.4142135623730951 * sqrtA * q;
      localDW->obj.b[0] = (f + wc) / a0;
      localDW->obj.b[1] = ((A + 1.0) * p + (1.0 - A)) * -2.0 / a0;
      localDW->obj.b[2] = (f - wc) / a0;
      localDW->obj.a[0] = a0 / a0;
      localDW->obj.a[1] = ((G + 1.0) * p + (1.0 - G)) * -2.0 / a0;
      localDW->obj.a[2] = (a0_tmp - a0_tmp_0) / a0;
      break;

     case 6U:
      /*  highshelf (RBJ, usando Q como S) */
      /* Cálculo de los coefifientes de un Filtro Paso-Banda 2º orden */
      /* Parámetos de entrada:  */
      /* wo = frecuencia central (en radianes) */
      /* Q = factor de calidad */
      /*  */
      /* Parametros de salida: */
      /* Coeficientes [b,a] de un filtro IIR de segundo orden */
      A = rt_powd_snf(10.0, G / 40.0);
      sqrtA = sqrt(A);
      p = cos(wc);
      q = sin(wc);

      /*  Coeficientes del ecualizador */
      G = (1.0 - A) * p + (A + 1.0);
      a0_tmp = 1.4142135623730951 * sqrtA * q;
      a0 = G + a0_tmp;
      a0_tmp_0 = A * A;

      /* Normalización de coeficientes */
      /* fvtool(b,a); */
      f = 1.0 / A;
      wc = (1.0 - f) * p + (f + 1.0);
      sqrtA = 1.4142135623730951 * q / sqrtA;
      localDW->obj.b[0] = (wc + sqrtA) * a0_tmp_0 / a0;
      localDW->obj.b[1] = ((f + 1.0) * p + (1.0 - f)) * -2.0 * a0_tmp_0 / a0;
      localDW->obj.b[2] = (wc - sqrtA) * a0_tmp_0 / a0;
      localDW->obj.a[0] = a0 / a0;
      localDW->obj.a[1] = ((A + 1.0) * p + (1.0 - A)) * -2.0 / a0;
      localDW->obj.a[2] = (G - a0_tmp) / a0;
      break;

     case 7U:
      /*  peaking / centershelf */
      /* Cálculo de los coefifientes de un Filtro Paso-Banda 2º orden */
      /* Parámetos de entrada:  */
      /* wo = frecuencia central (en radianes) */
      /* Q = factor de calidad */
      /*  */
      /* Parametros de salida: */
      /* Coeficientes [b,a] de un filtro IIR de segundo orden */
      A = rt_powd_snf(10.0, G / 40.0);
      f = sin(wc) / (2.0 * Q);

      /*  Coeficientes del ecualizador */
      G = f / A;

      /* Normalización de coeficientes */
      /* fvtool(b,a); */
      f *= A;
      localDW->obj.b[0] = (f + 1.0) / (G + 1.0);
      wc = -2.0 * cos(wc) / (G + 1.0);
      localDW->obj.b[1] = wc;
      localDW->obj.b[2] = (1.0 - f) / (G + 1.0);
      localDW->obj.a[0] = (G + 1.0) / (G + 1.0);
      localDW->obj.a[1] = wc;
      localDW->obj.a[2] = (1.0 - G) / (G + 1.0);
      break;
    }

    localDW->obj.lastType = rtu_2;
    localDW->obj.lastFreq = rtu_3;
    localDW->obj.lastQ = Q;
    localDW->obj.lastGain = rtu_5;
  }

  /*  bp es logical */
  for (t = 0; t < 3; t++) {
    tmp[t] = localDW->obj.b[t];
  }

  for (t = 0; t < 3; t++) {
    tmp_0[t] = localDW->obj.a[t];
  }

  /* Start for MATLABSystem: '<Root>/MATLAB System' */
  memcpy(&rtu_0_0[0], &rtu_0[0], sizeof(real_T) << 7);

  /* MATLABSystem: '<Root>/MATLAB System' */
  eqgraph_filter(tmp, tmp_0, rtu_0_0, localDW->obj.zL, localB->MATLABSystem_o1,
                 b);
  localDW->obj.zL[0] = b[0];
  localDW->obj.zL[1] = b[1];
  for (t = 0; t < 3; t++) {
    tmp[t] = localDW->obj.b[t];
  }

  for (t = 0; t < 3; t++) {
    tmp_0[t] = localDW->obj.a[t];
  }

  /* Start for MATLABSystem: '<Root>/MATLAB System' */
  memcpy(&rtu_0_0[0], &rtu_1[0], sizeof(real_T) << 7);

  /* MATLABSystem: '<Root>/MATLAB System' */
  eqgraph_filter(tmp, tmp_0, rtu_0_0, localDW->obj.zR, localB->MATLABSystem_o2,
                 b);
  localDW->obj.zR[0] = b[0];
  localDW->zL[0] = localDW->obj.zL[0];
  localDW->zR[0] = b[0];
  localDW->obj.zR[1] = b[1];
  localDW->zL[1] = localDW->obj.zL[1];
  localDW->zR[1] = b[1];
}

/* Termination for atomic system: */
void eqgraph_MATLABSystem_Term(DW_MATLABSystem_eqgraph_T *localDW)
{
  /* Terminate for MATLABSystem: '<Root>/MATLAB System' */
  localDW->zL[0] = localDW->obj.zL[0];
  localDW->zR[0] = localDW->obj.zR[0];
  localDW->zL[1] = localDW->obj.zL[1];
  localDW->zR[1] = localDW->obj.zR[1];
}

/* Model step function */
void eqgraph_step(void)
{
  real_T rtb_Gain[128];
  real_T rtb_Gain1[128];
  real_T g;
  real_T tmp;
  int32_T i;

  /* S-Function (pisound_in): '<Root>/PiSound Input' */

  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = eqgraph_M->childSfunctions[0];
    sfcnOutputs(rts,0);
  }

  for (i = 0; i < 128; i++) {
    /* Gain: '<Root>/Gain' incorporates:
     *  DataTypeConversion: '<Root>/Data Type Conversion'
     */
    rtb_Gain[i] = eqgraph_P.Gain_Gain * (real_T)eqgraph_B.PiSoundInput_o1[i];

    /* Gain: '<Root>/Gain1' incorporates:
     *  DataTypeConversion: '<Root>/Data Type Conversion1'
     */
    rtb_Gain1[i] = eqgraph_P.Gain1_Gain * (real_T)eqgraph_B.PiSoundInput_o2[i];
  }

  /* Constant: '<Root>/Constant7' incorporates:
   *  Constant: '<Root>/Constant'
   *  Constant: '<Root>/Constant1'
   *  Constant: '<Root>/Constant10'
   *  Constant: '<Root>/Constant11'
   *  Constant: '<Root>/Constant12'
   *  Constant: '<Root>/Constant13'
   *  Constant: '<Root>/Constant14'
   *  Constant: '<Root>/Constant16'
   *  Constant: '<Root>/Constant17'
   *  Constant: '<Root>/Constant2'
   *  Constant: '<Root>/Constant3'
   *  Constant: '<Root>/Constant4'
   *  Constant: '<Root>/Constant5'
   *  Constant: '<Root>/Constant6'
   *  Constant: '<Root>/Constant9'
   */
  eqgraph_MATLABSystem(rtb_Gain, rtb_Gain1, eqgraph_P.Constant7_Value,
                       eqgraph_P.Constant1_Value, eqgraph_P.Constant_Value,
                       eqgraph_P.Gain1_rt, &eqgraph_B.MATLABSystem,
                       &eqgraph_DW.MATLABSystem);
  eqgraph_MATLABSystem(eqgraph_B.MATLABSystem.MATLABSystem_o1,
                       eqgraph_B.MATLABSystem.MATLABSystem_o2,
                       eqgraph_P.Constant7_Value, eqgraph_P.Constant2_Value,
                       eqgraph_P.Constant_Value, eqgraph_P.Gain2_rt,
                       &eqgraph_B.MATLABSystem1, &eqgraph_DW.MATLABSystem1);
  eqgraph_MATLABSystem(eqgraph_B.MATLABSystem1.MATLABSystem_o1,
                       eqgraph_B.MATLABSystem1.MATLABSystem_o2,
                       eqgraph_P.Constant7_Value, eqgraph_P.Constant3_Value,
                       eqgraph_P.Constant_Value, eqgraph_P.Gain3_rt,
                       &eqgraph_B.MATLABSystem2, &eqgraph_DW.MATLABSystem2);
  eqgraph_MATLABSystem(eqgraph_B.MATLABSystem2.MATLABSystem_o1,
                       eqgraph_B.MATLABSystem2.MATLABSystem_o2,
                       eqgraph_P.Constant7_Value, eqgraph_P.Constant4_Value,
                       eqgraph_P.Constant_Value, eqgraph_P.Gain4_rt,
                       &eqgraph_B.MATLABSystem3, &eqgraph_DW.MATLABSystem3);
  eqgraph_MATLABSystem(eqgraph_B.MATLABSystem3.MATLABSystem_o1,
                       eqgraph_B.MATLABSystem3.MATLABSystem_o2,
                       eqgraph_P.Constant7_Value, eqgraph_P.Constant5_Value,
                       eqgraph_P.Constant_Value, eqgraph_P.Gain5_rt,
                       &eqgraph_B.MATLABSystem4, &eqgraph_DW.MATLABSystem4);
  eqgraph_MATLABSystem(eqgraph_B.MATLABSystem4.MATLABSystem_o1,
                       eqgraph_B.MATLABSystem4.MATLABSystem_o2,
                       eqgraph_P.Constant7_Value, eqgraph_P.Constant6_Value,
                       eqgraph_P.Constant_Value, eqgraph_P.Gain6_rt,
                       &eqgraph_B.MATLABSystem5, &eqgraph_DW.MATLABSystem5);
  eqgraph_MATLABSystem(eqgraph_B.MATLABSystem5.MATLABSystem_o1,
                       eqgraph_B.MATLABSystem5.MATLABSystem_o2,
                       eqgraph_P.Constant7_Value, eqgraph_P.Constant16_Value,
                       eqgraph_P.Constant_Value, eqgraph_P.Gain7_rt,
                       &eqgraph_B.MATLABSystem7, &eqgraph_DW.MATLABSystem7);

  /* MATLABSystem: '<Root>/MATLAB System6' incorporates:
   *  Constant: '<Root>/Constant15'
   *  Constant: '<Root>/Constant8'
   */
  g = eqgraph_P.Gain_rt;

  /*  g es Nx1 */
  if (eqgraph_P.Constant15_Value != 0) {
    g = rt_powd_snf(10.0, eqgraph_P.Gain_rt / 20.0);

    /*  dB -> lineal, elemento a elemento */
  }

  /*  bp es logical */
  /*  elemento a elemento */
  for (i = 0; i < 128; i++) {
    /* DataTypeConversion: '<Root>/Data Type Conversion2' incorporates:
     *  Gain: '<Root>/Gain2'
     *  MATLABSystem: '<Root>/MATLAB System6'
     *  MATLABSystem: '<Root>/MATLAB System7'
     * */
    tmp = floor(eqgraph_B.MATLABSystem7.MATLABSystem_o1[i] * g *
                eqgraph_P.Gain2_Gain);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion2' */
    eqgraph_B.DataTypeConversion2[i] = tmp < 0.0 ? -(int32_T)(uint32_T)-tmp :
      (int32_T)(uint32_T)tmp;

    /* DataTypeConversion: '<Root>/Data Type Conversion3' incorporates:
     *  Gain: '<Root>/Gain3'
     *  MATLABSystem: '<Root>/MATLAB System6'
     *  MATLABSystem: '<Root>/MATLAB System7'
     * */
    tmp = floor(eqgraph_B.MATLABSystem7.MATLABSystem_o2[i] * g *
                eqgraph_P.Gain3_Gain);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion3' */
    eqgraph_B.DataTypeConversion3[i] = tmp < 0.0 ? -(int32_T)(uint32_T)-tmp :
      (int32_T)(uint32_T)tmp;
  }

  /* S-Function (pisound_out): '<Root>/PiSound Output' */

  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = eqgraph_M->childSfunctions[1];
    sfcnOutputs(rts,0);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  eqgraph_M->Timing.t[0] =
    ((time_T)(++eqgraph_M->Timing.clockTick0)) * eqgraph_M->Timing.stepSize0;
}

/* Model initialize function */
void eqgraph_initialize(void)
{
  /* Registration code */
  rtsiSetSolverName(&eqgraph_M->solverInfo,"FixedStepDiscrete");
  eqgraph_M->solverInfoPtr = (&eqgraph_M->solverInfo);

  /* Initialize timing info */
  {
    int_T *mdlTsMap = eqgraph_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    eqgraph_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    eqgraph_M->Timing.sampleTimes = (&eqgraph_M->Timing.sampleTimesArray[0]);
    eqgraph_M->Timing.offsetTimes = (&eqgraph_M->Timing.offsetTimesArray[0]);

    /* task periods */
    eqgraph_M->Timing.sampleTimes[0] = (0.0026666666666666666);

    /* task offsets */
    eqgraph_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(eqgraph_M, &eqgraph_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = eqgraph_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    eqgraph_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(eqgraph_M, -1);
  eqgraph_M->Timing.stepSize0 = 0.0026666666666666666;
  eqgraph_M->solverInfoPtr = (&eqgraph_M->solverInfo);
  eqgraph_M->Timing.stepSize = (0.0026666666666666666);
  rtsiSetFixedStepSize(&eqgraph_M->solverInfo, 0.0026666666666666666);
  rtsiSetSolverMode(&eqgraph_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* child S-Function registration */
  {
    RTWSfcnInfo *sfcnInfo = &eqgraph_M->NonInlinedSFcns.sfcnInfo;
    eqgraph_M->sfcnInfo = (sfcnInfo);
    rtssSetErrorStatusPtr(sfcnInfo, (&rtmGetErrorStatus(eqgraph_M)));
    eqgraph_M->Sizes.numSampTimes = (1);
    rtssSetNumRootSampTimesPtr(sfcnInfo, &eqgraph_M->Sizes.numSampTimes);
    eqgraph_M->NonInlinedSFcns.taskTimePtrs[0] = (&rtmGetTPtr(eqgraph_M)[0]);
    rtssSetTPtrPtr(sfcnInfo,eqgraph_M->NonInlinedSFcns.taskTimePtrs);
    rtssSetTStartPtr(sfcnInfo, &rtmGetTStart(eqgraph_M));
    rtssSetTFinalPtr(sfcnInfo, &rtmGetTFinal(eqgraph_M));
    rtssSetTimeOfLastOutputPtr(sfcnInfo, &rtmGetTimeOfLastOutput(eqgraph_M));
    rtssSetStepSizePtr(sfcnInfo, &eqgraph_M->Timing.stepSize);
    rtssSetStopRequestedPtr(sfcnInfo, &rtmGetStopRequested(eqgraph_M));
    rtssSetDerivCacheNeedsResetPtr(sfcnInfo, &eqgraph_M->derivCacheNeedsReset);
    rtssSetZCCacheNeedsResetPtr(sfcnInfo, &eqgraph_M->zCCacheNeedsReset);
    rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(sfcnInfo,
      &eqgraph_M->CTOutputIncnstWithState);
    rtssSetSampleHitsPtr(sfcnInfo, &eqgraph_M->Timing.sampleHits);
    rtssSetPerTaskSampleHitsPtr(sfcnInfo, &eqgraph_M->Timing.perTaskSampleHits);
    rtssSetSimModePtr(sfcnInfo, &eqgraph_M->simMode);
    rtssSetSolverInfoPtr(sfcnInfo, &eqgraph_M->solverInfoPtr);
  }

  eqgraph_M->Sizes.numSFcns = (2);

  /* register each child */
  {
    (void) memset((void *)&eqgraph_M->NonInlinedSFcns.childSFunctions[0], 0,
                  2*sizeof(SimStruct));
    eqgraph_M->childSfunctions = (&eqgraph_M->
      NonInlinedSFcns.childSFunctionPtrs[0]);
    eqgraph_M->childSfunctions[0] = (&eqgraph_M->
      NonInlinedSFcns.childSFunctions[0]);
    eqgraph_M->childSfunctions[1] = (&eqgraph_M->
      NonInlinedSFcns.childSFunctions[1]);

    /* Level2 S-Function Block: eqgraph/<Root>/PiSound Input (pisound_in) */
    {
      SimStruct *rts = eqgraph_M->childSfunctions[0];

      /* timing info */
      time_T *sfcnPeriod = eqgraph_M->NonInlinedSFcns.Sfcn0.sfcnPeriod;
      time_T *sfcnOffset = eqgraph_M->NonInlinedSFcns.Sfcn0.sfcnOffset;
      int_T *sfcnTsMap = eqgraph_M->NonInlinedSFcns.Sfcn0.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &eqgraph_M->NonInlinedSFcns.blkInfo2[0]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &eqgraph_M->NonInlinedSFcns.inputOutputPortInfo2[0]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, eqgraph_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &eqgraph_M->NonInlinedSFcns.methods2[0]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &eqgraph_M->NonInlinedSFcns.methods3[0]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &eqgraph_M->NonInlinedSFcns.methods4[0]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &eqgraph_M->NonInlinedSFcns.statesInfo2[0]);
        ssSetPeriodicStatesInfo(rts,
          &eqgraph_M->NonInlinedSFcns.periodicStatesInfo[0]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &eqgraph_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &eqgraph_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &eqgraph_M->NonInlinedSFcns.Sfcn0.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &eqgraph_M->NonInlinedSFcns.Sfcn0.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 128);
          ssSetOutputPortSignal(rts, 0, ((int32_T *) eqgraph_B.PiSoundInput_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 128);
          ssSetOutputPortSignal(rts, 1, ((int32_T *) eqgraph_B.PiSoundInput_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "PiSound Input");
      ssSetPath(rts, "eqgraph/PiSound Input");
      ssSetRTModel(rts,eqgraph_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &eqgraph_M->NonInlinedSFcns.Sfcn0.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)eqgraph_P.PiSoundInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)eqgraph_P.PiSoundInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)eqgraph_P.PiSoundInput_P3_Size);
      }

      /* registration */
      pisound_in(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 0.0026666666666666666);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 0;

      /* set compiled values of dynamic vector attributes */
      ssSetNumNonsampledZCsAsInt(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetOutputPortConnected(rts, 0, 1);
      _ssSetOutputPortConnected(rts, 1, 1);
      _ssSetOutputPortBeingMerged(rts, 0, 0);
      _ssSetOutputPortBeingMerged(rts, 1, 0);

      /* Update the BufferDstPort flags for each input port */
    }

    /* Level2 S-Function Block: eqgraph/<Root>/PiSound Output (pisound_out) */
    {
      SimStruct *rts = eqgraph_M->childSfunctions[1];

      /* timing info */
      time_T *sfcnPeriod = eqgraph_M->NonInlinedSFcns.Sfcn1.sfcnPeriod;
      time_T *sfcnOffset = eqgraph_M->NonInlinedSFcns.Sfcn1.sfcnOffset;
      int_T *sfcnTsMap = eqgraph_M->NonInlinedSFcns.Sfcn1.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &eqgraph_M->NonInlinedSFcns.blkInfo2[1]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &eqgraph_M->NonInlinedSFcns.inputOutputPortInfo2[1]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, eqgraph_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &eqgraph_M->NonInlinedSFcns.methods2[1]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &eqgraph_M->NonInlinedSFcns.methods3[1]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &eqgraph_M->NonInlinedSFcns.methods4[1]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &eqgraph_M->NonInlinedSFcns.statesInfo2[1]);
        ssSetPeriodicStatesInfo(rts,
          &eqgraph_M->NonInlinedSFcns.periodicStatesInfo[1]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts,
          &eqgraph_M->NonInlinedSFcns.Sfcn1.inputPortInfo[0]);
        ssSetPortInfoForInputs(rts,
          &eqgraph_M->NonInlinedSFcns.Sfcn1.inputPortInfo[0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &eqgraph_M->NonInlinedSFcns.Sfcn1.inputPortUnits[0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &eqgraph_M->NonInlinedSFcns.Sfcn1.inputPortCoSimAttribute[0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          int32_T const **sfcnUPtrs = (int32_T const **)
            &eqgraph_M->NonInlinedSFcns.Sfcn1.UPtrs0;

          {
            int_T i1;
            const int32_T *u0 = eqgraph_B.DataTypeConversion2;
            for (i1=0; i1 < 128; i1++) {
              sfcnUPtrs[i1] = &u0[i1];
            }
          }

          ssSetInputPortSignalPtrs(rts, 0, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidthAsInt(rts, 0, 128);
        }

        /* port 1 */
        {
          int32_T const **sfcnUPtrs = (int32_T const **)
            &eqgraph_M->NonInlinedSFcns.Sfcn1.UPtrs1;

          {
            int_T i1;
            const int32_T *u1 = eqgraph_B.DataTypeConversion3;
            for (i1=0; i1 < 128; i1++) {
              sfcnUPtrs[i1] = &u1[i1];
            }
          }

          ssSetInputPortSignalPtrs(rts, 1, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 1, 1);
          ssSetInputPortWidthAsInt(rts, 1, 128);
        }
      }

      /* path info */
      ssSetModelName(rts, "PiSound Output");
      ssSetPath(rts, "eqgraph/PiSound Output");
      ssSetRTModel(rts,eqgraph_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &eqgraph_M->NonInlinedSFcns.Sfcn1.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)eqgraph_P.PiSoundOutput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)eqgraph_P.PiSoundOutput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)eqgraph_P.PiSoundOutput_P3_Size);
      }

      /* registration */
      pisound_out(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 0.0026666666666666666);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 0;

      /* set compiled values of dynamic vector attributes */
      ssSetNumNonsampledZCsAsInt(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetInputPortConnected(rts, 0, 1);
      _ssSetInputPortConnected(rts, 1, 1);

      /* Update the BufferDstPort flags for each input port */
      ssSetInputPortBufferDstPort(rts, 0, -1);
      ssSetInputPortBufferDstPort(rts, 1, -1);
    }
  }

  eqgraph_MATLABSystem_Init(&eqgraph_DW.MATLABSystem);
  eqgraph_MATLABSystem_Init(&eqgraph_DW.MATLABSystem1);
  eqgraph_MATLABSystem_Init(&eqgraph_DW.MATLABSystem2);
  eqgraph_MATLABSystem_Init(&eqgraph_DW.MATLABSystem3);
  eqgraph_MATLABSystem_Init(&eqgraph_DW.MATLABSystem4);
  eqgraph_MATLABSystem_Init(&eqgraph_DW.MATLABSystem5);
  eqgraph_MATLABSystem_Init(&eqgraph_DW.MATLABSystem7);
}

/* Model terminate function */
void eqgraph_terminate(void)
{
  /* Terminate for S-Function (pisound_in): '<Root>/PiSound Input' */
  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = eqgraph_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  eqgraph_MATLABSystem_Term(&eqgraph_DW.MATLABSystem);
  eqgraph_MATLABSystem_Term(&eqgraph_DW.MATLABSystem1);
  eqgraph_MATLABSystem_Term(&eqgraph_DW.MATLABSystem2);
  eqgraph_MATLABSystem_Term(&eqgraph_DW.MATLABSystem3);
  eqgraph_MATLABSystem_Term(&eqgraph_DW.MATLABSystem4);
  eqgraph_MATLABSystem_Term(&eqgraph_DW.MATLABSystem5);
  eqgraph_MATLABSystem_Term(&eqgraph_DW.MATLABSystem7);

  /* Terminate for S-Function (pisound_out): '<Root>/PiSound Output' */
  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = eqgraph_M->childSfunctions[1];
    sfcnTerminate(rts);
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
