/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: amp2.c
 *
 * Code generated for Simulink model 'amp2'.
 *
 * Model version                  : 1.15
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Fri May  1 09:27:40 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-A
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "amp2.h"
#include "rtwtypes.h"
#include "amp2_types.h"
#include "amp2_private.h"
#include "rt_nonfinite.h"
#include <string.h>
#include <math.h>

/* Block signals (default storage) */
B_amp2_T amp2_B;

/* Block states (default storage) */
DW_amp2_T amp2_DW;

/* Real-time model */
static RT_MODEL_amp2_T amp2_M_;
RT_MODEL_amp2_T *const amp2_M = &amp2_M_;

/* Forward declaration for local functions */
static void amp2_filter(real_T b[3], real_T a[3], const real_T x[128], const
  real_T zi[2], real_T y[128], real_T zf[2]);

/* Forward declaration for local functions */
static boolean_T amp2_isequal(const real_T varargin_1[257], const real_T
  varargin_2[257]);
static real_T amp2_lut_ws_sys_shapeOne(const lut_ws_sys_amp2_T *obj, real_T x,
  uint8_T md, real_T drive, real_T b, real_T cheb3, real_T cheb5, const real_T
  xGrid[257], const real_T yGrid[257]);
static void amp2_filter_o(const real_T b[15], const real_T x[256], const real_T
  zi[14], real_T y[256], real_T zf[14]);
static void amp2_filter_o0(const real_T b[15], const real_T x[512], const real_T
  zi[14], real_T y[512], real_T zf[14]);
static void amp_lut_ws_sys_processChannelOS(const lut_ws_sys_amp2_T *obj, const
  real_T x[128], uint8_T os, uint8_T md, real_T drive, real_T b, real_T cheb3,
  real_T cheb5, const real_T xGrid[257], const real_T yGrid[257], real_T zUp1[14],
  real_T zUp2[14], real_T zDn1[14], real_T zDn2[14], real_T y[128]);

/* Forward declaration for local functions */
static boolean_T amp2_isequal_n(const real_T varargin_1[257], const real_T
  varargin_2[257]);
real_T rt_roundd_snf(real_T u)
{
  real_T y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

static void amp2_filter(real_T b[3], real_T a[3], const real_T x[128], const
  real_T zi[2], real_T y[128], real_T zf[2])
{
  int32_T b_j;
  int32_T e_k;
  int32_T naxpy;

  /* Start for MATLABSystem: '<S1>/CabHPF' */
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

    /* Start for MATLABSystem: '<S1>/CabHPF' */
    if (128 - e_k <= 3) {
      naxpy = 128 - e_k;
    } else {
      naxpy = 3;
    }

    for (b_j = 0; b_j < naxpy; b_j++) {
      y_tmp = e_k + b_j;

      /* Start for MATLABSystem: '<S1>/CabHPF' */
      y[y_tmp] += x[e_k] * b[b_j];
    }

    /* Start for MATLABSystem: '<S1>/CabHPF' */
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
      /* Start for MATLABSystem: '<S1>/CabHPF' */
      zf[naxpy] += b[(naxpy - e_k) + 2] * x[e_k + 126];
    }
  }

  for (e_k = 0; e_k < 2; e_k++) {
    for (naxpy = 0; naxpy <= e_k; naxpy++) {
      /* Start for MATLABSystem: '<S1>/CabHPF' */
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
void amp2_CabHPF_Init(DW_CabHPF_amp2_T *localDW)
{
  /* Start for MATLABSystem: '<S1>/CabHPF' */
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

  /* InitializeConditions for MATLABSystem: '<S1>/CabHPF' */
  localDW->obj.zL[0] = 0.0;
  localDW->obj.zR[0] = 0.0;
  localDW->obj.zL[1] = 0.0;
  localDW->obj.zR[1] = 0.0;
}

/* Output and update for atomic system: */
void amp2_CabHPF(const real_T rtu_0[128], const real_T rtu_1[128], real_T rtu_2,
                 real_T rtu_3, real_T rtu_4, real_T rtu_5, B_CabHPF_amp2_T
                 *localB, DW_CabHPF_amp2_T *localDW)
{
  real_T rtu_0_0[128];
  real_T tmp_0[3];
  real_T tmp_1[3];
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
  uint8_T tmp;

  /* MATLABSystem: '<S1>/CabHPF' */
  G = rt_roundd_snf(rtu_2);
  if (G < 256.0) {
    if (G >= 0.0) {
      tmp = (uint8_T)G;
    } else {
      tmp = 0U;
    }
  } else {
    tmp = MAX_uint8_T;
  }

  /*  acepta double y castea */
  if ((tmp != localDW->obj.lastType) || (rtu_3 != localDW->obj.lastFreq) ||
      (rtu_4 != localDW->obj.lastQ) || (rtu_5 != localDW->obj.lastGain)) {
    f = rtu_3;
    Q = rtu_4;
    G = rtu_5;
    t = tmp;
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
    if (tmp < 1) {
      t = 1;
    } else if (tmp > 7) {
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
      f = 1.4142135623730951 * sin(wc);

      /* Normalización de coeficientes */
      /* fvtool(b,a); */
      G = (1.0 - p) / (f + 2.0);
      localDW->obj.b[0] = G;
      localDW->obj.b[1] = (1.0 - p) * 2.0 / (f + 2.0);
      localDW->obj.b[2] = G;
      localDW->obj.a[0] = (f + 2.0) / (f + 2.0);
      localDW->obj.a[1] = -4.0 * p / (f + 2.0);
      localDW->obj.a[2] = (2.0 - f) / (f + 2.0);
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
      f = 1.4142135623730951 * sin(wc);

      /* Normalización de coeficientes */
      /*  */
      /* fvtool(b,a); */
      G = (p + 1.0) / (f + 2.0);
      localDW->obj.b[0] = G;
      localDW->obj.b[1] = (p + 1.0) * -2.0 / (f + 2.0);
      localDW->obj.b[2] = G;
      localDW->obj.a[0] = (f + 2.0) / (f + 2.0);
      localDW->obj.a[1] = -4.0 * p / (f + 2.0);
      localDW->obj.a[2] = (2.0 - f) / (f + 2.0);
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
      f = sin(wc) / Q;

      /*  <-- Q aquí */
      /*  Coeficientes del ecualizador */
      /* Normalización de coeficientes */
      /* fvtool(b,a); */
      localDW->obj.b[0] = f / (f + 2.0);
      localDW->obj.b[1] = 0.0 / (f + 2.0);
      localDW->obj.b[2] = -f / (f + 2.0);
      localDW->obj.a[0] = (f + 2.0) / (f + 2.0);
      localDW->obj.a[1] = -4.0 * cos(wc) / (f + 2.0);
      localDW->obj.a[2] = (2.0 - f) / (f + 2.0);
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
      f = sin(wc) / Q;

      /* Normalización de coeficientes */
      /* fvtool(b,a); */
      G = 2.0 / (f + 2.0);
      localDW->obj.b[0] = G;
      wc = -4.0 * cos(wc) / (f + 2.0);
      localDW->obj.b[1] = wc;
      localDW->obj.b[2] = G;
      localDW->obj.a[0] = (f + 2.0) / (f + 2.0);
      localDW->obj.a[1] = wc;
      localDW->obj.a[2] = (2.0 - f) / (f + 2.0);
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
      f = 1.0 / A;
      a0_tmp = (1.0 - f) * p + (f + 1.0);
      a0_tmp_0 = 1.4142135623730951 * q / sqrtA;
      a0 = a0_tmp + a0_tmp_0;

      /* Normalización de coeficientes */
      /* fvtool(b,a); */
      G = (1.0 - A) * p + (A + 1.0);
      wc = 1.4142135623730951 * sqrtA * q;
      localDW->obj.b[0] = (G + wc) / a0;
      localDW->obj.b[1] = ((A + 1.0) * p + (1.0 - A)) * -2.0 / a0;
      localDW->obj.b[2] = (G - wc) / a0;
      localDW->obj.a[0] = a0 / a0;
      localDW->obj.a[1] = ((f + 1.0) * p + (1.0 - f)) * -2.0 / a0;
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
      f = (1.0 - A) * p + (A + 1.0);
      a0_tmp = 1.4142135623730951 * sqrtA * q;
      a0 = f + a0_tmp;
      a0_tmp_0 = A * A;

      /* Normalización de coeficientes */
      /* fvtool(b,a); */
      G = 1.0 / A;
      wc = (1.0 - G) * p + (G + 1.0);
      sqrtA = 1.4142135623730951 * q / sqrtA;
      localDW->obj.b[0] = (wc + sqrtA) * a0_tmp_0 / a0;
      localDW->obj.b[1] = ((G + 1.0) * p + (1.0 - G)) * -2.0 * a0_tmp_0 / a0;
      localDW->obj.b[2] = (wc - sqrtA) * a0_tmp_0 / a0;
      localDW->obj.a[0] = a0 / a0;
      localDW->obj.a[1] = ((A + 1.0) * p + (1.0 - A)) * -2.0 / a0;
      localDW->obj.a[2] = (f - a0_tmp) / a0;
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
      G = sin(wc) / (2.0 * Q);

      /*  Coeficientes del ecualizador */
      f = G / A;

      /* Normalización de coeficientes */
      /* fvtool(b,a); */
      G *= A;
      localDW->obj.b[0] = (G + 1.0) / (f + 1.0);
      wc = -2.0 * cos(wc) / (f + 1.0);
      localDW->obj.b[1] = wc;
      localDW->obj.b[2] = (1.0 - G) / (f + 1.0);
      localDW->obj.a[0] = (f + 1.0) / (f + 1.0);
      localDW->obj.a[1] = wc;
      localDW->obj.a[2] = (1.0 - f) / (f + 1.0);
      break;
    }

    localDW->obj.lastType = tmp;
    localDW->obj.lastFreq = rtu_3;
    localDW->obj.lastQ = Q;
    localDW->obj.lastGain = rtu_5;
  }

  /*  bp es logical */
  for (t = 0; t < 3; t++) {
    tmp_0[t] = localDW->obj.b[t];
  }

  for (t = 0; t < 3; t++) {
    tmp_1[t] = localDW->obj.a[t];
  }

  /* Start for MATLABSystem: '<S1>/CabHPF' */
  memcpy(&rtu_0_0[0], &rtu_0[0], sizeof(real_T) << 7);

  /* MATLABSystem: '<S1>/CabHPF' */
  amp2_filter(tmp_0, tmp_1, rtu_0_0, localDW->obj.zL, localB->CabHPF_o1, b);
  localDW->obj.zL[0] = b[0];
  localDW->obj.zL[1] = b[1];
  for (t = 0; t < 3; t++) {
    tmp_0[t] = localDW->obj.b[t];
  }

  for (t = 0; t < 3; t++) {
    tmp_1[t] = localDW->obj.a[t];
  }

  /* Start for MATLABSystem: '<S1>/CabHPF' */
  memcpy(&rtu_0_0[0], &rtu_1[0], sizeof(real_T) << 7);

  /* MATLABSystem: '<S1>/CabHPF' */
  amp2_filter(tmp_0, tmp_1, rtu_0_0, localDW->obj.zR, localB->CabHPF_o2, b);
  localDW->obj.zR[0] = b[0];
  localDW->zR[0] = b[0];
  localDW->zL[0] = localDW->obj.zL[0];
  localDW->obj.zR[1] = b[1];
  localDW->zR[1] = b[1];
  localDW->zL[1] = localDW->obj.zL[1];
}

/* Termination for atomic system: */
void amp2_CabHPF_Term(DW_CabHPF_amp2_T *localDW)
{
  /* Terminate for MATLABSystem: '<S1>/CabHPF' */
  localDW->zR[0] = localDW->obj.zR[0];
  localDW->zL[0] = localDW->obj.zL[0];
  localDW->zR[1] = localDW->obj.zR[1];
  localDW->zL[1] = localDW->obj.zL[1];
}

static boolean_T amp2_isequal(const real_T varargin_1[257], const real_T
  varargin_2[257])
{
  int32_T b_k;
  boolean_T exitg1;
  boolean_T p;
  boolean_T p_0;
  p = false;

  /* Start for MATLABSystem: '<S3>/Stage2' */
  p_0 = true;
  b_k = 0;
  exitg1 = false;
  while ((!exitg1) && (b_k < 257)) {
    if (!(varargin_1[b_k] == varargin_2[b_k])) {
      p_0 = false;
      exitg1 = true;
    } else {
      b_k++;
    }
  }

  if (p_0) {
    p = true;
  }

  return p;
}

static real_T amp2_lut_ws_sys_shapeOne(const lut_ws_sys_amp2_T *obj, real_T x,
  uint8_T md, real_T drive, real_T b, real_T cheb3, real_T cheb5, const real_T
  xGrid[257], const real_T yGrid[257])
{
  real_T y;

  /*  SHAPER (una muestra) */
  /*  pre: drive + bias */
  y = drive * x + b;

  /* Start for MATLABSystem: '<S3>/Stage2' */
  if (rtIsInf(y) || rtIsNaN(y)) {
    y = 0.0;
  }

  /*  limitar a rango razonable antes de funciones no lineales */
  /* u = clamp(obj, u, -1.0, 1.0); */
  if (md == 1) {
    /*  ---- Hard clip simétrico ---- */
    if (y < -1.0) {
      y = -1.0;
    } else if (y > 1.0) {
      y = 1.0;
    }
  } else if (md == 2) {
    /*  ---- Soft clip polinómico (tipo 3er orden, continuo) ---- */
    if (y < -2.0) {
      y = -2.0;
    } else if (y > 2.0) {
      y = 2.0;
    }

    /*  y = u - u^3/3 para |u|<=1  (suave, sin discontinuidad) */
    /*  (si quieres más agresivo, usa otro polinomio) */
    y -= y * y * y / 3.0;
  } else if (md == 3) {
    /*  ---- Tanh (soft saturación) ---- */
    /*  Normalizamos ligeramente para que tanh(1) no sea demasiado bajo */
    y = tanh(2.0 * y) / 0.9640275800758169;
  } else if (md == 4) {
    /*  ---- Asym clip (bias ya aplicado) + hard clip ---- */
    /*  Asimetría sale de b; aquí solo clippeamos. */
    /* y = clamp(obj, u, -1.0, 1.0); */
    /*  recorta antes en negativo */
    if (y < -0.6) {
      y = -0.6;
    } else if (y > 1.0) {
      y = 1.0;
    }

    /*  renormaliza */
  } else if (md == 5) {
    real_T den;
    real_T u2;

    /*  ---- Chebyshev controlado (T1 + a3*T3 + a5*T5) ---- */
    /*  Para x en [-1,1]: */
    /*  T1=u */
    /*  T3=4u^3-3u */
    /*  T5=16u^5-20u^3+5u */
    if (y < -1.0) {
      y = -1.0;
    } else if (y > 1.0) {
      y = 1.0;
    }

    u2 = y * y;
    den = u2 * y;
    y = ((den * u2 * 16.0 - 20.0 * den) + 5.0 * y) * cheb5 + ((4.0 * den - 3.0 *
      y) * cheb3 + y);

    /*  re-limit (Chebyshev puede exceder) */
    if (y < -1.0) {
      y = -1.0;
    } else if (y > 1.0) {
      y = 1.0;
    }

    /*  ---- User LUT: y = interp(u; XUser,YUser) ---- */
    /*  Interpolación lineal LUT (saturación) */
  } else if (y <= xGrid[0]) {
    y = yGrid[0];
  } else if (y >= xGrid[256]) {
    y = yGrid[256];
  } else {
    real_T den;
    real_T u2;
    int32_T b_i;
    int32_T k;
    boolean_T exitg1;
    k = 1;
    b_i = 0;
    exitg1 = false;
    while ((!exitg1) && (b_i < 256)) {
      if (y < xGrid[b_i + 1]) {
        k = b_i + 1;
        exitg1 = true;
      } else {
        b_i++;
      }
    }

    u2 = xGrid[k - 1];
    den = xGrid[k] - u2;
    if ((den <= obj->Eps) || (rtIsInf(den) || rtIsNaN(den))) {
      y = yGrid[k - 1];
    } else {
      real_T y_tmp;
      y_tmp = yGrid[k - 1];
      y = (y - u2) / den * (yGrid[k] - y_tmp) + y_tmp;
    }
  }

  /*  post: opcionalmente quitar bias DC introducido (simple, estático) */
  /*  En audio real, quitar DC es mejor con HPF muy suave; aquí hacemos corrección estática: */
  /* y = y - b; */
  if (rtIsInf(y) || rtIsNaN(y)) {
    y = 0.0;
  }

  /*  limitar salida final */
  if (y < -1.0) {
    y = -1.0;
  } else if (y > 1.0) {
    y = 1.0;
  }

  /* End of Start for MATLABSystem: '<S3>/Stage2' */
  return y;
}

static void amp2_filter_o(const real_T b[15], const real_T x[256], const real_T
  zi[14], real_T y[256], real_T zf[14])
{
  int32_T b_j;
  int32_T b_k;
  for (b_k = 0; b_k < 14; b_k++) {
    zf[b_k] = 0.0;
    y[b_k] = zi[b_k];
  }

  memset(&y[14], 0, 242U * sizeof(real_T));
  for (b_k = 0; b_k < 15; b_k++) {
    for (b_j = b_k + 1; b_j < 257; b_j++) {
      y[b_j - 1] += x[(b_j - b_k) - 1] * b[b_k];
    }
  }

  for (b_k = 0; b_k < 14; b_k++) {
    for (b_j = 0; b_j <= b_k; b_j++) {
      /* Start for MATLABSystem: '<S3>/Stage2' */
      zf[b_j] += b[(b_j - b_k) + 14] * x[b_k + 242];
    }
  }
}

static void amp2_filter_o0(const real_T b[15], const real_T x[512], const real_T
  zi[14], real_T y[512], real_T zf[14])
{
  int32_T b_j;
  int32_T b_k;
  for (b_k = 0; b_k < 14; b_k++) {
    zf[b_k] = 0.0;
    y[b_k] = zi[b_k];
  }

  memset(&y[14], 0, 498U * sizeof(real_T));
  for (b_k = 0; b_k < 15; b_k++) {
    for (b_j = b_k + 1; b_j < 513; b_j++) {
      y[b_j - 1] += x[(b_j - b_k) - 1] * b[b_k];
    }
  }

  for (b_k = 0; b_k < 14; b_k++) {
    for (b_j = 0; b_j <= b_k; b_j++) {
      /* Start for MATLABSystem: '<S3>/Stage2' */
      zf[b_j] += b[(b_j - b_k) + 14] * x[b_k + 498];
    }
  }
}

static void amp_lut_ws_sys_processChannelOS(const lut_ws_sys_amp2_T *obj, const
  real_T x[128], uint8_T os, uint8_T md, real_T drive, real_T b, real_T cheb3,
  real_T cheb5, const real_T xGrid[257], const real_T yGrid[257], real_T zUp1[14],
  real_T zUp2[14], real_T zDn1[14], real_T zDn2[14], real_T y[128])
{
  real_T b_y2_0[512];
  real_T u_0[512];
  real_T y4[512];
  real_T b_y2[256];
  real_T u[256];
  real_T y2[256];
  int32_T i;
  static const real_T tmp[15] = { -0.000106360599, 0.0, 0.00695210045, 0.0,
    -0.0516884606, 0.0, 0.294825487, 0.500034468, 0.294825487, 0.0,
    -0.0516884606, 0.0, 0.00695210045, 0.0, -0.000106360599 };

  real_T zUp1_0[14];

  /* Start for MATLABSystem: '<S3>/Stage2' */
  if (os == 0) {
    for (i = 0; i < 128; i++) {
      y[i] = amp2_lut_ws_sys_shapeOne(obj, x[i], md, drive, b, cheb3, cheb5,
        xGrid, yGrid);
    }
  } else if (os == 1) {
    memset(&u[0], 0, sizeof(real_T) << 8U);
    for (i = 0; i < 128; i++) {
      u[i << 1] = x[i];
    }

    memcpy(&zUp1_0[0], &zUp1[0], 14U * sizeof(real_T));
    amp2_filter_o(tmp, u, zUp1_0, b_y2, zUp1);

    /*  2N */
    for (i = 0; i < 256; i++) {
      y2[i] = amp2_lut_ws_sys_shapeOne(obj, b_y2[i], md, drive, b, cheb3, cheb5,
        xGrid, yGrid);
    }

    memcpy(&zUp1_0[0], &zDn1[0], 14U * sizeof(real_T));
    amp2_filter_o(tmp, y2, zUp1_0, u, zDn1);
    for (i = 0; i < 128; i++) {
      y[i] = u[i << 1];
    }

    /*  N */
  } else {
    memset(&u[0], 0, sizeof(real_T) << 8U);
    for (i = 0; i < 128; i++) {
      u[i << 1] = x[i];
    }

    memcpy(&zUp1_0[0], &zUp1[0], 14U * sizeof(real_T));
    amp2_filter_o(tmp, u, zUp1_0, b_y2, zUp1);

    /*  2N */
    memset(&u_0[0], 0, sizeof(real_T) << 9U);
    for (i = 0; i < 256; i++) {
      u_0[i << 1] = b_y2[i];
    }

    memcpy(&zUp1_0[0], &zUp2[0], 14U * sizeof(real_T));
    amp2_filter_o0(tmp, u_0, zUp1_0, b_y2_0, zUp2);

    /*  4N */
    for (i = 0; i < 512; i++) {
      y4[i] = amp2_lut_ws_sys_shapeOne(obj, b_y2_0[i], md, drive, b, cheb3,
        cheb5, xGrid, yGrid);
    }

    memcpy(&zUp1_0[0], &zDn2[0], 14U * sizeof(real_T));
    amp2_filter_o0(tmp, y4, zUp1_0, u_0, zDn2);

    /*  2N */
    for (i = 0; i < 256; i++) {
      b_y2[i] = u_0[i << 1];
    }

    memcpy(&zUp1_0[0], &zDn1[0], 14U * sizeof(real_T));
    amp2_filter_o(tmp, b_y2, zUp1_0, u, zDn1);
    for (i = 0; i < 128; i++) {
      y[i] = u[i << 1];
    }

    /*  N */
  }

  /* End of Start for MATLABSystem: '<S3>/Stage2' */
}

/* System initialize for atomic system: */
void amp2_Stage2_Init(DW_Stage2_amp2_T *localDW)
{
  /* Start for MATLABSystem: '<S3>/Stage2' */
  localDW->objisempty = true;
  localDW->obj.ModeDefault = 3U;
  localDW->obj.AmountDefault = 12.0;
  localDW->obj.AsymDefault = 0.0;
  localDW->obj.Cheb3Default = 1.0;
  localDW->obj.Cheb5Default = 0.0;
  memcpy(&localDW->obj.XUser[0], &amp2_ConstP.pooled5[0], 257U * sizeof(real_T));
  memcpy(&localDW->obj.YUser[0], &amp2_ConstP.pooled5[0], 257U * sizeof(real_T));
  localDW->obj.MUser = 257.0;
  localDW->obj.Eps = 1.0E-12;
  localDW->obj.isInitialized = 1;

  /* InitializeConditions for MATLABSystem: '<S3>/Stage2' */
  /*  Validaciones */
  memset(&localDW->obj.zUp1L[0], 0, 14U * sizeof(real_T));
  memset(&localDW->obj.zUp1R[0], 0, 14U * sizeof(real_T));
  memset(&localDW->obj.zDn1L[0], 0, 14U * sizeof(real_T));
  memset(&localDW->obj.zDn1R[0], 0, 14U * sizeof(real_T));
  memset(&localDW->obj.zUp2L[0], 0, 14U * sizeof(real_T));
  memset(&localDW->obj.zUp2R[0], 0, 14U * sizeof(real_T));
  memset(&localDW->obj.zDn2L[0], 0, 14U * sizeof(real_T));
  memset(&localDW->obj.zDn2R[0], 0, 14U * sizeof(real_T));
}

/* Output and update for atomic system: */
void amp2_Stage2(const real_T rtu_0[128], const real_T rtu_1[128], real_T rtu_2,
                 real_T rtu_3, real_T rtu_4, real_T rtu_5, real_T rtu_6,
                 B_Stage2_amp2_T *localB, DW_Stage2_amp2_T *localDW)
{
  real_T xGrid[257];
  real_T yGrid[257];
  real_T c[14];
  real_T d[14];
  real_T e[14];
  real_T f[14];
  real_T b;
  real_T drive;
  real_T obj_zDn1R;
  real_T obj_zDn2R;
  int32_T i;
  int32_T tmp;

  /* MATLABSystem: '<S3>/Stage2' */
  if (localDW->obj.ModeDefault != 3) {
    localDW->obj.ModeDefault = 3U;
  }

  if (localDW->obj.AmountDefault != 12.0) {
    localDW->obj.AmountDefault = 12.0;
  }

  if (localDW->obj.AsymDefault != 0.0) {
    localDW->obj.AsymDefault = 0.0;
  }

  if (localDW->obj.Cheb3Default != 1.0) {
    localDW->obj.Cheb3Default = 1.0;
  }

  if (localDW->obj.Cheb5Default != 0.0) {
    localDW->obj.Cheb5Default = 0.0;
  }

  if (!amp2_isequal(localDW->obj.XUser, amp2_ConstP.pooled5)) {
    memcpy(&localDW->obj.XUser[0], &amp2_ConstP.pooled5[0], 257U * sizeof(real_T));
  }

  if (!amp2_isequal(localDW->obj.YUser, amp2_ConstP.pooled5)) {
    memcpy(&localDW->obj.YUser[0], &amp2_ConstP.pooled5[0], 257U * sizeof(real_T));
  }

  if (localDW->obj.MUser != 257.0) {
    localDW->obj.MUser = 257.0;
  }

  if (localDW->obj.Eps != 1.0E-12) {
    localDW->obj.Eps = 1.0E-12;
  }

  /*  inL,inR,mode,amount,asym,cheb3,cheb5 */
  /*  Nloc = size(inL,1); */
  /*  outL = zeros(Nloc,1); */
  /*  outR = zeros(Nloc,1); */
  /*  bp es logical */
  /*  --- defaults --- */
  tmp = (int32_T)fmin(fmax(rt_roundd_snf(rtu_2), 1.0), 6.0);

  /*  por ejemplo */
  if (rtu_4 < -0.5) {
    b = -0.5;
  } else if (rtu_4 > 0.5) {
    b = 0.5;
  } else {
    b = rtu_4;
  }

  /*  drive interno (lineal) */
  if (rtu_3 < 0.0) {
    drive = 0.0;
  } else if (rtu_3 > 60.0) {
    drive = 60.0;
  } else {
    drive = rtu_3;
  }

  drive = rt_powd_snf(10.0, drive / 20.0);

  /*  preparar LUT usuario (si aplica) */
  /*  (tamaño fijo M, relleno seguro) */
  memset(&xGrid[0], 0, 257U * sizeof(real_T));
  memset(&yGrid[0], 0, 257U * sizeof(real_T));
  if (tmp == 6) {
    memcpy(&xGrid[0], &localDW->obj.XUser[0], 257U * sizeof(real_T));
    memcpy(&yGrid[0], &localDW->obj.YUser[0], 257U * sizeof(real_T));
  }

  /* Start for MATLABSystem: '<S3>/Stage2' */
  memcpy(&c[0], &localDW->obj.zUp1L[0], 14U * sizeof(real_T));
  memcpy(&d[0], &localDW->obj.zUp2L[0], 14U * sizeof(real_T));
  memcpy(&e[0], &localDW->obj.zDn1L[0], 14U * sizeof(real_T));
  memcpy(&f[0], &localDW->obj.zDn2L[0], 14U * sizeof(real_T));
  amp_lut_ws_sys_processChannelOS(&localDW->obj, rtu_0, 0, (uint8_T)tmp, drive,
    b, rtu_5, rtu_6, xGrid, yGrid, c, d, e, f, localB->Stage2_o1);
  for (i = 0; i < 14; i++) {
    /* MATLABSystem: '<S3>/Stage2' */
    localDW->obj.zUp1L[i] = c[i];
    localDW->obj.zUp2L[i] = d[i];
    localDW->obj.zDn1L[i] = e[i];
    localDW->obj.zDn2L[i] = f[i];

    /* Start for MATLABSystem: '<S3>/Stage2' */
    c[i] = localDW->obj.zUp1R[i];
    d[i] = localDW->obj.zUp2R[i];
    e[i] = localDW->obj.zDn1R[i];
    f[i] = localDW->obj.zDn2R[i];
  }

  /* Start for MATLABSystem: '<S3>/Stage2' */
  amp_lut_ws_sys_processChannelOS(&localDW->obj, rtu_1, 0, (uint8_T)tmp, drive,
    b, rtu_5, rtu_6, xGrid, yGrid, c, d, e, f, localB->Stage2_o2);

  /* MATLABSystem: '<S3>/Stage2' */
  for (i = 0; i < 14; i++) {
    b = c[i];
    localDW->obj.zUp1R[i] = b;
    drive = d[i];
    localDW->obj.zUp2R[i] = drive;
    obj_zDn1R = e[i];
    localDW->obj.zDn1R[i] = obj_zDn1R;
    obj_zDn2R = f[i];
    localDW->obj.zDn2R[i] = obj_zDn2R;
    localDW->zUp1L[i] = localDW->obj.zUp1L[i];
    localDW->zDn2L[i] = localDW->obj.zDn2L[i];
    localDW->zUp2R[i] = drive;
    localDW->zUp1R[i] = b;
    localDW->zDn2R[i] = obj_zDn2R;
    localDW->zDn1L[i] = localDW->obj.zDn1L[i];
    localDW->zUp2L[i] = localDW->obj.zUp2L[i];
    localDW->zDn1R[i] = obj_zDn1R;
  }
}

/* Termination for atomic system: */
void amp2_Stage2_Term(DW_Stage2_amp2_T *localDW)
{
  /* Terminate for MATLABSystem: '<S3>/Stage2' */
  memcpy(&localDW->zUp1L[0], &localDW->obj.zUp1L[0], 14U * sizeof(real_T));
  memcpy(&localDW->zDn2L[0], &localDW->obj.zDn2L[0], 14U * sizeof(real_T));
  memcpy(&localDW->zUp2R[0], &localDW->obj.zUp2R[0], 14U * sizeof(real_T));
  memcpy(&localDW->zUp1R[0], &localDW->obj.zUp1R[0], 14U * sizeof(real_T));
  memcpy(&localDW->zDn2R[0], &localDW->obj.zDn2R[0], 14U * sizeof(real_T));
  memcpy(&localDW->zDn1L[0], &localDW->obj.zDn1L[0], 14U * sizeof(real_T));
  memcpy(&localDW->zUp2L[0], &localDW->obj.zUp2L[0], 14U * sizeof(real_T));
  memcpy(&localDW->zDn1R[0], &localDW->obj.zDn1R[0], 14U * sizeof(real_T));
}

static boolean_T amp2_isequal_n(const real_T varargin_1[257], const real_T
  varargin_2[257])
{
  int32_T b_k;
  boolean_T exitg1;
  boolean_T p;
  boolean_T p_0;
  p = false;

  /* Start for MATLABSystem: '<S5>/SagGC' */
  p_0 = true;
  b_k = 0;
  exitg1 = false;
  while ((!exitg1) && (b_k < 257)) {
    if (!(varargin_1[b_k] == varargin_2[b_k])) {
      p_0 = false;
      exitg1 = true;
    } else {
      b_k++;
    }
  }

  if (p_0) {
    p = true;
  }

  return p;
}

/* Model step function */
void amp2_step(void)
{
  real_T xGrid[257];
  real_T yGrid[257];
  real_T in[128];
  real_T rtb_NormL[128];
  real_T rtb_NormR[128];
  real_T rtb_SagVCA_o1[128];
  real_T a_;
  real_T den;
  real_T g;
  real_T gd1_;
  real_T gd2_;
  real_T gw1_;
  real_T gw2_;
  int32_T b_i;
  int32_T i;
  int32_T k;
  boolean_T exitg1;

  /* S-Function (ctrl_in): '<Root>/Control Input' */

  /* Level2 S-Function Block: '<Root>/Control Input' (ctrl_in) */
  {
    SimStruct *rts = amp2_M->childSfunctions[0];
    sfcnOutputs(rts,0);
  }

  /* S-Function (stream_in): '<Root>/Stream Input' */

  /* Level2 S-Function Block: '<Root>/Stream Input' (stream_in) */
  {
    SimStruct *rts = amp2_M->childSfunctions[1];
    sfcnOutputs(rts,0);
  }

  for (i = 0; i < 128; i++) {
    /* Gain: '<Root>/NormL' */
    rtb_NormL[i] = 4.6565999992607965E-10 * (real_T)amp2_B.StreamInput_o1[i];

    /* Gain: '<Root>/NormR' */
    rtb_NormR[i] = 4.6565999992607965E-10 * (real_T)amp2_B.StreamInput_o2[i];
  }

  /* Constant: '<S4>/InHPF_type' incorporates:
   *  Constant: '<S4>/InHPF_Q'
   *  Constant: '<S4>/InHPF_freq'
   *  Constant: '<S4>/InHPF_gain'
   */
  amp2_CabHPF(rtb_NormL, rtb_NormR, 2.0, 80.0, 0.7, 0.0, &amp2_B.InputHPF,
              &amp2_DW.InputHPF);

  /* Constant: '<S4>/St1_mode' incorporates:
   *  Constant: '<S4>/St1_cheb3'
   *  Constant: '<S4>/St1_cheb5'
   */
  amp2_Stage2(amp2_B.InputHPF.CabHPF_o1, amp2_B.InputHPF.CabHPF_o2, 3.0,
              amp2_B.ControlInput_o1, amp2_B.ControlInput_o2, 0.0, 0.0,
              &amp2_B.Stage1, &amp2_DW.Stage1);

  /* Constant: '<S2>/IsHPF_type' incorporates:
   *  Constant: '<S2>/IsHPF_Q'
   *  Constant: '<S2>/IsHPF_freq'
   *  Constant: '<S2>/IsHPF_gain'
   */
  amp2_CabHPF(amp2_B.Stage1.Stage2_o1, amp2_B.Stage1.Stage2_o2, 2.0, 100.0, 0.7,
              0.0, &amp2_B.IstagHPF, &amp2_DW.IstagHPF);

  /* Constant: '<S2>/IsMid_type' incorporates:
   *  Constant: '<S2>/IsMid_Q'
   *  Constant: '<S2>/IsMid_freq'
   *  Constant: '<S2>/IsMid_gain'
   */
  amp2_CabHPF(amp2_B.IstagHPF.CabHPF_o1, amp2_B.IstagHPF.CabHPF_o2, 7.0, 1000.0,
              1.2, 3.0, &amp2_B.IstagMid, &amp2_DW.IstagMid);

  /* Constant: '<S3>/St2_mode' incorporates:
   *  Constant: '<S3>/St2_amount'
   *  Constant: '<S3>/St2_asym'
   *  Constant: '<S3>/St2_cheb3'
   *  Constant: '<S3>/St2_cheb5'
   */
  amp2_Stage2(amp2_B.IstagMid.CabHPF_o1, amp2_B.IstagMid.CabHPF_o2, 4.0, 6.0,
              0.15, 0.0, 0.0, &amp2_B.Stage2, &amp2_DW.Stage2);

  /* MATLABSystem: '<S5>/SagENV' incorporates:
   *  MATLABSystem: '<S3>/Stage2'
   */
  if (amp2_DW.obj_c.ModeDefault != 1) {
    amp2_DW.obj_c.ModeDefault = 1U;
  }

  if (amp2_DW.obj_c.Eps != 1.0E-12) {
    amp2_DW.obj_c.Eps = 1.0E-12;
  }

  if (amp2_DW.obj_c.TauRMS != 0.02) {
    amp2_DW.obj_c.TauRMS = 0.02;
  }

  /*  Estéreo enlazado por máximo */
  /*  -------- PEAK (instantáneo) -------- */
  for (i = 0; i < 128; i++) {
    rtb_SagVCA_o1[i] = log10(fmax(fabs(amp2_B.Stage2.Stage2_o1[i]), fabs
      (amp2_B.Stage2.Stage2_o2[i])) + 1.0E-12) * 20.0;
  }

  /* MATLABSystem: '<S5>/SagGC' incorporates:
   *  Constant: '<S5>/SagGC_thr'
   */
  if (amp2_DW.obj.Xmin != -80.0) {
    amp2_DW.obj.Xmin = -80.0;
  }

  if (amp2_DW.obj.Xmax != 10.0) {
    amp2_DW.obj.Xmax = 10.0;
  }

  if (amp2_DW.obj.Floor_dB != -80.0) {
    amp2_DW.obj.Floor_dB = -80.0;
  }

  if (!amp2_isequal_n(amp2_DW.obj.XUser, amp2_ConstP.SagGC_XUser)) {
    memcpy(&amp2_DW.obj.XUser[0], &amp2_ConstP.SagGC_XUser[0], 257U * sizeof
           (real_T));
  }

  if (!amp2_isequal_n(amp2_DW.obj.YUser, amp2_ConstP.SagGC_YUser)) {
    memset(&amp2_DW.obj.YUser[0], 0, 257U * sizeof(real_T));
  }

  if (amp2_DW.obj.MUser != 257.0) {
    amp2_DW.obj.MUser = 257.0;
  }

  /*  --- parámetros --- */
  /*  --- construir LUT (xGrid, yGrid) --- */
  /*  xGrid: [1 x M] */
  xGrid[256] = 10.0;
  xGrid[0] = -80.0;
  for (i = 0; i < 255; i++) {
    xGrid[i + 1] = ((real_T)i + 1.0) * 0.3515625 - 80.0;
  }

  /*  ===== MODO 1: Compresor descendente (hard knee) ===== */
  /*  Lout = x si x<=T ; Lout = T + (x-T)/R si x>T */
  for (i = 0; i < 257; i++) {
    den = xGrid[i];
    if (den <= -12.0) {
      g = den;
    } else {
      g = (den - -12.0) / 4.0 - 12.0;
    }

    yGrid[i] = g - den;

    /*  GdB0 */
  }

  /* MATLABSystem: '<S5>/SagARF' */
  /*  --- aplicar LUT por interpolación lineal a Lin_dB --- */
  /*  bypass del suavizado */
  /*  tiempos válidos */
  g = amp2_DW.obj_m.gPrev_dB;

  /*  g es Nx1 */
  /*  dB -> lineal, elemento a elemento */
  /*  bp es logical */
  /*  elemento a elemento */
  for (i = 0; i < 128; i++) {
    /* MATLABSystem: '<S5>/SagENV' incorporates:
     *  MATLABSystem: '<S5>/SagGC'
     * */
    /*  ---------- Interpolación lineal con saturación ---------- */
    /*  Saturación fuera de rango */
    den = rtb_SagVCA_o1[i];

    /* MATLABSystem: '<S5>/SagGC' incorporates:
     *  MATLABSystem: '<S5>/SagENV'
     */
    if (den <= xGrid[0]) {
      den = yGrid[0];
    } else if (den >= xGrid[256]) {
      den = yGrid[256];
    } else {
      /*  Búsqueda lineal (M pequeño; codegen-friendly) */
      k = 0;
      b_i = 0;
      exitg1 = false;
      while ((!exitg1) && (b_i < 256)) {
        if (rtb_SagVCA_o1[i] < xGrid[b_i + 1]) {
          k = b_i;
          exitg1 = true;
        } else {
          b_i++;
        }
      }

      /*  Evitar división por cero si la LUT de usuario  */
      /*  no es estrictamente creciente */
      den = xGrid[k + 1] - xGrid[k];
      if (den <= 0.0) {
        den = yGrid[k];
      } else {
        den = (yGrid[k + 1] - yGrid[k]) * ((rtb_SagVCA_o1[i] - xGrid[k]) / den)
          + yGrid[k];
      }
    }

    /* MATLABSystem: '<S5>/SagARF' incorporates:
     *  MATLABSystem: '<S5>/SagGC'
     */
    /*  ganancia en dB */
    /*  + makeup */
    if (den < g) {
      /*  más reducción de ganancia -> attack */
      g = 0.99958342012683377 * g + 0.000416579873166234 * den;
    } else {
      /*  menos reducción de ganancia -> release */
      g = 0.99993055796676522 * g + 6.9442033234778933E-5 * den;
    }

    /* MATLABSystem: '<S5>/SagVCA' incorporates:
     *  MATLABSystem: '<S3>/Stage2'
     *  MATLABSystem: '<S5>/SagARF'
     *  Product: '<S5>/SagScale'
     */
    den = rt_powd_snf(10.0, g * amp2_B.ControlInput_o3 / 20.0);
    rtb_SagVCA_o1[i] = amp2_B.Stage2.Stage2_o1[i] * den;
    in[i] = amp2_B.Stage2.Stage2_o2[i] * den;
  }

  /* MATLABSystem: '<S5>/SagARF' */
  amp2_DW.obj_m.gPrev_dB = g;

  /* Constant: '<S6>/Bass_type' incorporates:
   *  Constant: '<S6>/Bass_Q'
   *  Constant: '<S6>/Bass_freq'
   */
  amp2_CabHPF(rtb_SagVCA_o1, in, 5.0, 120.0, 0.7, amp2_B.ControlInput_o4,
              &amp2_B.ToneBass, &amp2_DW.ToneBass);

  /* Constant: '<S6>/Mid_type' incorporates:
   *  Constant: '<S6>/Mid_Q'
   *  Constant: '<S6>/Mid_freq'
   */
  amp2_CabHPF(amp2_B.ToneBass.CabHPF_o1, amp2_B.ToneBass.CabHPF_o2, 7.0, 800.0,
              1.5, amp2_B.ControlInput_o5, &amp2_B.ToneMid, &amp2_DW.ToneMid);

  /* Constant: '<S6>/Treble_type' incorporates:
   *  Constant: '<S6>/Treble_Q'
   *  Constant: '<S6>/Treble_freq'
   */
  amp2_CabHPF(amp2_B.ToneMid.CabHPF_o1, amp2_B.ToneMid.CabHPF_o2, 6.0, 4000.0,
              0.7, amp2_B.ControlInput_o6, &amp2_B.ToneTreble,
              &amp2_DW.ToneTreble);

  /* Constant: '<S1>/CabHPF_type' incorporates:
   *  Constant: '<S1>/CabHPF_Q'
   *  Constant: '<S1>/CabHPF_freq'
   *  Constant: '<S1>/CabHPF_gain'
   */
  amp2_CabHPF(amp2_B.ToneTreble.CabHPF_o1, amp2_B.ToneTreble.CabHPF_o2, 2.0,
              80.0, 0.7, 0.0, &amp2_B.CabHPF, &amp2_DW.CabHPF);

  /* Constant: '<S1>/CabLPF_type' incorporates:
   *  Constant: '<S1>/CabLPF_Q'
   *  Constant: '<S1>/CabLPF_freq'
   *  Constant: '<S1>/CabLPF_gain'
   */
  amp2_CabHPF(amp2_B.CabHPF.CabHPF_o1, amp2_B.CabHPF.CabHPF_o2, 1.0, 5000.0, 0.7,
              0.0, &amp2_B.CabLPF, &amp2_DW.CabLPF);

  /* Constant: '<S1>/CabMid_type' incorporates:
   *  Constant: '<S1>/CabMid_Q'
   *  Constant: '<S1>/CabMid_freq'
   *  Constant: '<S1>/CabMid_gain'
   */
  amp2_CabHPF(amp2_B.CabLPF.CabHPF_o1, amp2_B.CabLPF.CabHPF_o2, 7.0, 800.0, 2.0,
              -4.0, &amp2_B.CabMid, &amp2_DW.CabMid);
  for (i = 0; i < 128; i++) {
    /* Gain: '<Root>/MasterL' incorporates:
     *  MATLABSystem: '<S1>/CabMid'
     */
    in[i] = 0.8 * amp2_B.CabMid.CabHPF_o1[i];

    /* Gain: '<Root>/MasterR' incorporates:
     *  MATLABSystem: '<S1>/CabMid'
     */
    rtb_SagVCA_o1[i] = 0.8 * amp2_B.CabMid.CabHPF_o2[i];
  }

  /* MATLABSystem: '<Root>/BypassMix' */
  if (amp2_DW.obj_l.SmoothTime != 0.02) {
    amp2_DW.obj_l.SmoothTime = 0.02;
  }

  if (amp2_DW.obj_l.IndepInDB) {
    amp2_DW.obj_l.IndepInDB = false;
  }

  /*  inL,inR,wetL,wetR,p1 (+p2 si Mode=2) */
  /*  Mode 3: crossfade por muestra (p1 puede ser [N x 1]) */
  /*  dummy */
  den = fmin(fmax(amp2_B.ControlInput_o8, 0.0), 1.0);
  a_ = amp2_DW.obj_l.a;
  gd1_ = amp2_DW.obj_l.gd1;
  gd2_ = amp2_DW.obj_l.gd2;
  gw1_ = amp2_DW.obj_l.gw1;
  gw2_ = amp2_DW.obj_l.gw2;

  /*  más robusto que obj.N */
  for (i = 0; i < 128; i++) {
    gd1_ += ((1.0 - den) - gd1_) * a_;
    gw1_ += (den - gw1_) * a_;
    gd2_ += ((1.0 - den) - gd2_) * a_;
    gw2_ += (den - gw2_) * a_;

    /* DataTypeConversion: '<Root>/DTC_L' incorporates:
     *  Gain: '<Root>/MasterL'
     *  Gain: '<Root>/ScaleL'
     */
    g = floor((gd1_ * rtb_NormL[i] + gw1_ * in[i]) * 2.147483647E+9);
    if (rtIsNaN(g) || rtIsInf(g)) {
      g = 0.0;
    } else {
      g = fmod(g, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/DTC_L' */
    amp2_B.DTC_L[i] = g < 0.0 ? -(int32_T)(uint32_T)-g : (int32_T)(uint32_T)g;

    /* DataTypeConversion: '<Root>/DTC_R' incorporates:
     *  Gain: '<Root>/MasterR'
     *  Gain: '<Root>/ScaleR'
     */
    g = floor((gd2_ * rtb_NormR[i] + gw2_ * rtb_SagVCA_o1[i]) * 2.147483647E+9);
    if (rtIsNaN(g) || rtIsInf(g)) {
      g = 0.0;
    } else {
      g = fmod(g, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/DTC_R' */
    amp2_B.DTC_R[i] = g < 0.0 ? -(int32_T)(uint32_T)-g : (int32_T)(uint32_T)g;
  }

  amp2_DW.obj_l.gd1 = gd1_;
  amp2_DW.obj_l.gd2 = gd2_;
  amp2_DW.obj_l.gw1 = gw1_;
  amp2_DW.obj_l.gw2 = gw2_;

  /* End of MATLABSystem: '<Root>/BypassMix' */

  /* S-Function (stream_out): '<Root>/Stream Ouputt' */

  /* Level2 S-Function Block: '<Root>/Stream Ouputt' (stream_out) */
  {
    SimStruct *rts = amp2_M->childSfunctions[2];
    sfcnOutputs(rts,0);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  amp2_M->Timing.t[0] =
    ((time_T)(++amp2_M->Timing.clockTick0)) * amp2_M->Timing.stepSize0;
}

/* Model initialize function */
void amp2_initialize(void)
{
  /* Registration code */
  rtsiSetSolverName(&amp2_M->solverInfo,"FixedStepDiscrete");
  amp2_M->solverInfoPtr = (&amp2_M->solverInfo);

  /* Initialize timing info */
  {
    int_T *mdlTsMap = amp2_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    amp2_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    amp2_M->Timing.sampleTimes = (&amp2_M->Timing.sampleTimesArray[0]);
    amp2_M->Timing.offsetTimes = (&amp2_M->Timing.offsetTimesArray[0]);

    /* task periods */
    amp2_M->Timing.sampleTimes[0] = (0.0026666666666666666);

    /* task offsets */
    amp2_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(amp2_M, &amp2_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = amp2_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    amp2_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(amp2_M, -1);
  amp2_M->Timing.stepSize0 = 0.0026666666666666666;
  amp2_M->solverInfoPtr = (&amp2_M->solverInfo);
  amp2_M->Timing.stepSize = (0.0026666666666666666);
  rtsiSetFixedStepSize(&amp2_M->solverInfo, 0.0026666666666666666);
  rtsiSetSolverMode(&amp2_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* child S-Function registration */
  {
    RTWSfcnInfo *sfcnInfo = &amp2_M->NonInlinedSFcns.sfcnInfo;
    amp2_M->sfcnInfo = (sfcnInfo);
    rtssSetErrorStatusPtr(sfcnInfo, (&rtmGetErrorStatus(amp2_M)));
    amp2_M->Sizes.numSampTimes = (1);
    rtssSetNumRootSampTimesPtr(sfcnInfo, &amp2_M->Sizes.numSampTimes);
    amp2_M->NonInlinedSFcns.taskTimePtrs[0] = (&rtmGetTPtr(amp2_M)[0]);
    rtssSetTPtrPtr(sfcnInfo,amp2_M->NonInlinedSFcns.taskTimePtrs);
    rtssSetTStartPtr(sfcnInfo, &rtmGetTStart(amp2_M));
    rtssSetTFinalPtr(sfcnInfo, &rtmGetTFinal(amp2_M));
    rtssSetTimeOfLastOutputPtr(sfcnInfo, &rtmGetTimeOfLastOutput(amp2_M));
    rtssSetStepSizePtr(sfcnInfo, &amp2_M->Timing.stepSize);
    rtssSetStopRequestedPtr(sfcnInfo, &rtmGetStopRequested(amp2_M));
    rtssSetDerivCacheNeedsResetPtr(sfcnInfo, &amp2_M->derivCacheNeedsReset);
    rtssSetZCCacheNeedsResetPtr(sfcnInfo, &amp2_M->zCCacheNeedsReset);
    rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(sfcnInfo,
      &amp2_M->CTOutputIncnstWithState);
    rtssSetSampleHitsPtr(sfcnInfo, &amp2_M->Timing.sampleHits);
    rtssSetPerTaskSampleHitsPtr(sfcnInfo, &amp2_M->Timing.perTaskSampleHits);
    rtssSetSimModePtr(sfcnInfo, &amp2_M->simMode);
    rtssSetSolverInfoPtr(sfcnInfo, &amp2_M->solverInfoPtr);
  }

  amp2_M->Sizes.numSFcns = (3);

  /* register each child */
  {
    (void) memset((void *)&amp2_M->NonInlinedSFcns.childSFunctions[0], 0,
                  3*sizeof(SimStruct));
    amp2_M->childSfunctions = (&amp2_M->NonInlinedSFcns.childSFunctionPtrs[0]);
    amp2_M->childSfunctions[0] = (&amp2_M->NonInlinedSFcns.childSFunctions[0]);
    amp2_M->childSfunctions[1] = (&amp2_M->NonInlinedSFcns.childSFunctions[1]);
    amp2_M->childSfunctions[2] = (&amp2_M->NonInlinedSFcns.childSFunctions[2]);

    /* Level2 S-Function Block: amp2/<Root>/Control Input (ctrl_in) */
    {
      SimStruct *rts = amp2_M->childSfunctions[0];

      /* timing info */
      time_T *sfcnPeriod = amp2_M->NonInlinedSFcns.Sfcn0.sfcnPeriod;
      time_T *sfcnOffset = amp2_M->NonInlinedSFcns.Sfcn0.sfcnOffset;
      int_T *sfcnTsMap = amp2_M->NonInlinedSFcns.Sfcn0.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &amp2_M->NonInlinedSFcns.blkInfo2[0]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &amp2_M->NonInlinedSFcns.inputOutputPortInfo2[0]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, amp2_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &amp2_M->NonInlinedSFcns.methods2[0]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &amp2_M->NonInlinedSFcns.methods3[0]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &amp2_M->NonInlinedSFcns.methods4[0]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &amp2_M->NonInlinedSFcns.statesInfo2[0]);
        ssSetPeriodicStatesInfo(rts, &amp2_M->
          NonInlinedSFcns.periodicStatesInfo[0]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &amp2_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &amp2_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 8);
        _ssSetPortInfo2ForOutputUnits(rts,
          &amp2_M->NonInlinedSFcns.Sfcn0.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        ssSetOutputPortUnit(rts, 2, 0);
        ssSetOutputPortUnit(rts, 3, 0);
        ssSetOutputPortUnit(rts, 4, 0);
        ssSetOutputPortUnit(rts, 5, 0);
        ssSetOutputPortUnit(rts, 6, 0);
        ssSetOutputPortUnit(rts, 7, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &amp2_M->NonInlinedSFcns.Sfcn0.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 2, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 3, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 4, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 5, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 6, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 7, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 1);
          ssSetOutputPortSignal(rts, 0, ((real_T *) &amp2_B.ControlInput_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 1);
          ssSetOutputPortSignal(rts, 1, ((real_T *) &amp2_B.ControlInput_o2));
        }

        /* port 2 */
        {
          _ssSetOutputPortNumDimensions(rts, 2, 1);
          ssSetOutputPortWidthAsInt(rts, 2, 1);
          ssSetOutputPortSignal(rts, 2, ((real_T *) &amp2_B.ControlInput_o3));
        }

        /* port 3 */
        {
          _ssSetOutputPortNumDimensions(rts, 3, 1);
          ssSetOutputPortWidthAsInt(rts, 3, 1);
          ssSetOutputPortSignal(rts, 3, ((real_T *) &amp2_B.ControlInput_o4));
        }

        /* port 4 */
        {
          _ssSetOutputPortNumDimensions(rts, 4, 1);
          ssSetOutputPortWidthAsInt(rts, 4, 1);
          ssSetOutputPortSignal(rts, 4, ((real_T *) &amp2_B.ControlInput_o5));
        }

        /* port 5 */
        {
          _ssSetOutputPortNumDimensions(rts, 5, 1);
          ssSetOutputPortWidthAsInt(rts, 5, 1);
          ssSetOutputPortSignal(rts, 5, ((real_T *) &amp2_B.ControlInput_o6));
        }

        /* port 6 */
        {
          _ssSetOutputPortNumDimensions(rts, 6, 1);
          ssSetOutputPortWidthAsInt(rts, 6, 1);
          ssSetOutputPortSignal(rts, 6, ((real_T *) &amp2_B.ControlInput_o7));
        }

        /* port 7 */
        {
          _ssSetOutputPortNumDimensions(rts, 7, 1);
          ssSetOutputPortWidthAsInt(rts, 7, 1);
          ssSetOutputPortSignal(rts, 7, ((real_T *) &amp2_B.ControlInput_o8));
        }
      }

      /* path info */
      ssSetModelName(rts, "Control Input");
      ssSetPath(rts, "amp2/Control Input");
      ssSetRTModel(rts,amp2_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &amp2_M->NonInlinedSFcns.Sfcn0.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)amp2_ConstP.ControlInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)amp2_ConstP.ControlInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)amp2_ConstP.ControlInput_P3_Size);
      }

      /* registration */
      ctrl_in(rts);
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
      _ssSetOutputPortConnected(rts, 2, 1);
      _ssSetOutputPortConnected(rts, 3, 1);
      _ssSetOutputPortConnected(rts, 4, 1);
      _ssSetOutputPortConnected(rts, 5, 1);
      _ssSetOutputPortConnected(rts, 6, 0);
      _ssSetOutputPortConnected(rts, 7, 1);
      _ssSetOutputPortBeingMerged(rts, 0, 0);
      _ssSetOutputPortBeingMerged(rts, 1, 0);
      _ssSetOutputPortBeingMerged(rts, 2, 0);
      _ssSetOutputPortBeingMerged(rts, 3, 0);
      _ssSetOutputPortBeingMerged(rts, 4, 0);
      _ssSetOutputPortBeingMerged(rts, 5, 0);
      _ssSetOutputPortBeingMerged(rts, 6, 0);
      _ssSetOutputPortBeingMerged(rts, 7, 0);

      /* Update the BufferDstPort flags for each input port */
    }

    /* Level2 S-Function Block: amp2/<Root>/Stream Input (stream_in) */
    {
      SimStruct *rts = amp2_M->childSfunctions[1];

      /* timing info */
      time_T *sfcnPeriod = amp2_M->NonInlinedSFcns.Sfcn1.sfcnPeriod;
      time_T *sfcnOffset = amp2_M->NonInlinedSFcns.Sfcn1.sfcnOffset;
      int_T *sfcnTsMap = amp2_M->NonInlinedSFcns.Sfcn1.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &amp2_M->NonInlinedSFcns.blkInfo2[1]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &amp2_M->NonInlinedSFcns.inputOutputPortInfo2[1]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, amp2_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &amp2_M->NonInlinedSFcns.methods2[1]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &amp2_M->NonInlinedSFcns.methods3[1]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &amp2_M->NonInlinedSFcns.methods4[1]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &amp2_M->NonInlinedSFcns.statesInfo2[1]);
        ssSetPeriodicStatesInfo(rts, &amp2_M->
          NonInlinedSFcns.periodicStatesInfo[1]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &amp2_M->NonInlinedSFcns.Sfcn1.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &amp2_M->NonInlinedSFcns.Sfcn1.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &amp2_M->NonInlinedSFcns.Sfcn1.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &amp2_M->NonInlinedSFcns.Sfcn1.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 128);
          ssSetOutputPortSignal(rts, 0, ((int32_T *) amp2_B.StreamInput_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 128);
          ssSetOutputPortSignal(rts, 1, ((int32_T *) amp2_B.StreamInput_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "Stream Input");
      ssSetPath(rts, "amp2/Stream Input");
      ssSetRTModel(rts,amp2_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &amp2_M->NonInlinedSFcns.Sfcn1.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)amp2_ConstP.ControlInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)amp2_ConstP.ControlInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)amp2_ConstP.StreamInput_P3_Size);
      }

      /* registration */
      stream_in(rts);
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

    /* Level2 S-Function Block: amp2/<Root>/Stream Ouputt (stream_out) */
    {
      SimStruct *rts = amp2_M->childSfunctions[2];

      /* timing info */
      time_T *sfcnPeriod = amp2_M->NonInlinedSFcns.Sfcn2.sfcnPeriod;
      time_T *sfcnOffset = amp2_M->NonInlinedSFcns.Sfcn2.sfcnOffset;
      int_T *sfcnTsMap = amp2_M->NonInlinedSFcns.Sfcn2.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &amp2_M->NonInlinedSFcns.blkInfo2[2]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &amp2_M->NonInlinedSFcns.inputOutputPortInfo2[2]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, amp2_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &amp2_M->NonInlinedSFcns.methods2[2]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &amp2_M->NonInlinedSFcns.methods3[2]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &amp2_M->NonInlinedSFcns.methods4[2]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &amp2_M->NonInlinedSFcns.statesInfo2[2]);
        ssSetPeriodicStatesInfo(rts, &amp2_M->
          NonInlinedSFcns.periodicStatesInfo[2]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts, &amp2_M->
          NonInlinedSFcns.Sfcn2.inputPortInfo[0]);
        ssSetPortInfoForInputs(rts, &amp2_M->
          NonInlinedSFcns.Sfcn2.inputPortInfo[0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &amp2_M->NonInlinedSFcns.Sfcn2.inputPortUnits[0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &amp2_M->NonInlinedSFcns.Sfcn2.inputPortCoSimAttribute[0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          int32_T const **sfcnUPtrs = (int32_T const **)
            &amp2_M->NonInlinedSFcns.Sfcn2.UPtrs0;

          {
            int_T i1;
            const int32_T *u0 = amp2_B.DTC_L;
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
            &amp2_M->NonInlinedSFcns.Sfcn2.UPtrs1;

          {
            int_T i1;
            const int32_T *u1 = amp2_B.DTC_R;
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
      ssSetModelName(rts, "Stream Ouputt");
      ssSetPath(rts, "amp2/Stream Ouputt");
      ssSetRTModel(rts,amp2_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &amp2_M->NonInlinedSFcns.Sfcn2.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)amp2_ConstP.ControlInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)amp2_ConstP.ControlInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)amp2_ConstP.StreamInput_P3_Size);
      }

      /* registration */
      stream_out(rts);
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

  {
    int32_T i;
    amp2_CabHPF_Init(&amp2_DW.InputHPF);
    amp2_Stage2_Init(&amp2_DW.Stage1);
    amp2_CabHPF_Init(&amp2_DW.IstagHPF);
    amp2_CabHPF_Init(&amp2_DW.IstagMid);
    amp2_Stage2_Init(&amp2_DW.Stage2);

    /* Start for MATLABSystem: '<S5>/SagENV' */
    amp2_DW.obj_c.ModeDefault = 1U;
    amp2_DW.obj_c.Eps = 1.0E-12;
    amp2_DW.obj_c.TauRMS = 0.02;
    amp2_DW.obj_c.isInitialized = 1;

    /* InitializeConditions for MATLABSystem: '<S5>/SagENV' */
    amp2_DW.obj_c.e_prev = 0.0;

    /* Start for MATLABSystem: '<S5>/SagGC' */
    amp2_DW.obj.Xmin = -80.0;
    amp2_DW.obj.Xmax = 10.0;
    amp2_DW.obj.Floor_dB = -80.0;
    for (i = 0; i < 257; i++) {
      amp2_DW.obj.XUser[i] = amp2_ConstP.SagGC_XUser[i];
      amp2_DW.obj.YUser[i] = 0.0;
    }

    amp2_DW.obj.MUser = 257.0;
    amp2_DW.obj.isInitialized = 1;

    /* End of Start for MATLABSystem: '<S5>/SagGC' */

    /* Start for MATLABSystem: '<S5>/SagARF' */
    /*  Sin estado */
    amp2_DW.obj_m.isInitialized = 1;

    /* InitializeConditions for MATLABSystem: '<S5>/SagARF' */
    amp2_DW.obj_m.gPrev_dB = 0.0;
    amp2_CabHPF_Init(&amp2_DW.ToneBass);
    amp2_CabHPF_Init(&amp2_DW.ToneMid);
    amp2_CabHPF_Init(&amp2_DW.ToneTreble);
    amp2_CabHPF_Init(&amp2_DW.CabHPF);
    amp2_CabHPF_Init(&amp2_DW.CabLPF);
    amp2_CabHPF_Init(&amp2_DW.CabMid);

    /* Start for MATLABSystem: '<Root>/BypassMix' */
    amp2_DW.obj_l.SmoothTime = 0.02;
    amp2_DW.obj_l.IndepInDB = false;
    amp2_DW.obj_l.isInitialized = 1;

    /*  inL,inR,wetL,wetR,p1 (+p2 si Mode=2) */
    amp2_DW.obj_l.a = 0.0010411243202754772;

    /* InitializeConditions for MATLABSystem: '<Root>/BypassMix' */
    amp2_DW.obj_l.gd1 = 1.0;
    amp2_DW.obj_l.gd2 = 1.0;
    amp2_DW.obj_l.gw1 = 0.0;
    amp2_DW.obj_l.gw2 = 0.0;
  }
}

/* Model terminate function */
void amp2_terminate(void)
{
  /* Terminate for S-Function (ctrl_in): '<Root>/Control Input' */
  /* Level2 S-Function Block: '<Root>/Control Input' (ctrl_in) */
  {
    SimStruct *rts = amp2_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (stream_in): '<Root>/Stream Input' */
  /* Level2 S-Function Block: '<Root>/Stream Input' (stream_in) */
  {
    SimStruct *rts = amp2_M->childSfunctions[1];
    sfcnTerminate(rts);
  }

  amp2_CabHPF_Term(&amp2_DW.InputHPF);
  amp2_Stage2_Term(&amp2_DW.Stage1);
  amp2_CabHPF_Term(&amp2_DW.IstagHPF);
  amp2_CabHPF_Term(&amp2_DW.IstagMid);
  amp2_Stage2_Term(&amp2_DW.Stage2);
  amp2_CabHPF_Term(&amp2_DW.ToneBass);
  amp2_CabHPF_Term(&amp2_DW.ToneMid);
  amp2_CabHPF_Term(&amp2_DW.ToneTreble);
  amp2_CabHPF_Term(&amp2_DW.CabHPF);
  amp2_CabHPF_Term(&amp2_DW.CabLPF);
  amp2_CabHPF_Term(&amp2_DW.CabMid);

  /* Terminate for S-Function (stream_out): '<Root>/Stream Ouputt' */
  /* Level2 S-Function Block: '<Root>/Stream Ouputt' (stream_out) */
  {
    SimStruct *rts = amp2_M->childSfunctions[2];
    sfcnTerminate(rts);
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
