/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: dist3.c
 *
 * Code generated for Simulink model 'dist3'.
 *
 * Model version                  : 1.38
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Thu Feb 26 14:59:43 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "dist3.h"
#include "rtwtypes.h"
#include "dist3_types.h"
#include "dist3_private.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Block signals (default storage) */
B_dist3_T dist3_B;

/* Block states (default storage) */
DW_dist3_T dist3_DW;

/* Real-time model */
static RT_MODEL_dist3_T dist3_M_;
RT_MODEL_dist3_T *const dist3_M = &dist3_M_;

/* Forward declaration for local functions */
static void dist3_filter(real_T b[3], real_T a[3], const real_T x[128], const
  real_T zi[2], real_T y[128], real_T zf[2]);

/* Forward declaration for local functions */
static boolean_T dist3_isequal(const real_T varargin_1[257], const real_T
  varargin_2[257]);
static void dist3_filter_m(const real_T b[15], const real_T x[256], const real_T
  zi[14], real_T y[256], real_T zf[14]);
static void dist3_filter_mb(const real_T b[15], const real_T x[512], const
  real_T zi[14], real_T y[512], real_T zf[14]);
static real_T dist3_lut_ws_sys_shapeOne(const lut_ws_sys_dist3_T *obj, real_T x,
  uint8_T md, real_T drive, real_T b, real_T cheb3, real_T cheb5, const real_T
  xGrid[257], const real_T yGrid[257]);
static void dis_lut_ws_sys_processChannelOS(const lut_ws_sys_dist3_T *obj, const
  real_T x[128], uint8_T os, uint8_T md, real_T drive, real_T b, real_T cheb3,
  real_T cheb5, const real_T xGrid[257], const real_T yGrid[257], real_T zUp1[14],
  real_T zUp2[14], real_T zDn1[14], real_T zDn2[14], real_T y[128]);
static void dist3_filter(real_T b[3], real_T a[3], const real_T x[128], const
  real_T zi[2], real_T y[128], real_T zf[2])
{
  int32_T b_j;
  int32_T e_k;
  int32_T naxpy;

  /* Start for MATLABSystem: '<Root>/MATLAB System5' */
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

    /* Start for MATLABSystem: '<Root>/MATLAB System5' */
    if (128 - e_k <= 3) {
      naxpy = 128 - e_k;
    } else {
      naxpy = 3;
    }

    for (b_j = 0; b_j < naxpy; b_j++) {
      y_tmp = e_k + b_j;

      /* Start for MATLABSystem: '<Root>/MATLAB System5' */
      y[y_tmp] += x[e_k] * b[b_j];
    }

    /* Start for MATLABSystem: '<Root>/MATLAB System5' */
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
      /* Start for MATLABSystem: '<Root>/MATLAB System5' */
      zf[naxpy] += b[(naxpy - e_k) + 2] * x[e_k + 126];
    }
  }

  for (e_k = 0; e_k < 2; e_k++) {
    for (naxpy = 0; naxpy <= e_k; naxpy++) {
      /* Start for MATLABSystem: '<Root>/MATLAB System5' */
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
void dist3_MATLABSystem5_Init(DW_MATLABSystem5_dist3_T *localDW)
{
  /* Start for MATLABSystem: '<Root>/MATLAB System5' */
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

  /* InitializeConditions for MATLABSystem: '<Root>/MATLAB System5' */
  localDW->obj.zL[0] = 0.0;
  localDW->obj.zR[0] = 0.0;
  localDW->obj.zL[1] = 0.0;
  localDW->obj.zR[1] = 0.0;
}

/* Output and update for atomic system: */
void dist3_MATLABSystem5(const real_T rtu_0[128], const real_T rtu_1[128],
  uint8_T rtu_2, real_T rtu_3, real_T rtu_4, real_T rtu_5,
  B_MATLABSystem5_dist3_T *localB, DW_MATLABSystem5_dist3_T *localDW)
{
  real_T tmp_0[3];
  real_T tmp_1[3];
  real_T tmp[2];
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

  /* MATLABSystem: '<Root>/MATLAB System5' */
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
  for (t = 0; t < 2; t++) {
    tmp[t] = localDW->obj.zL[t];
  }

  for (t = 0; t < 3; t++) {
    tmp_0[t] = localDW->obj.b[t];
  }

  for (t = 0; t < 3; t++) {
    tmp_1[t] = localDW->obj.a[t];
  }

  /* MATLABSystem: '<Root>/MATLAB System5' */
  dist3_filter(tmp_0, tmp_1, rtu_0, tmp, localB->MATLABSystem5_o1,
               localDW->obj.zL);

  /* MATLABSystem: '<Root>/MATLAB System5' */
  for (t = 0; t < 2; t++) {
    tmp[t] = localDW->obj.zR[t];
  }

  for (t = 0; t < 3; t++) {
    tmp_0[t] = localDW->obj.b[t];
  }

  for (t = 0; t < 3; t++) {
    tmp_1[t] = localDW->obj.a[t];
  }

  /* MATLABSystem: '<Root>/MATLAB System5' */
  dist3_filter(tmp_0, tmp_1, rtu_1, tmp, localB->MATLABSystem5_o2,
               localDW->obj.zR);

  /* MATLABSystem: '<Root>/MATLAB System5' */
  localDW->zL[0] = localDW->obj.zL[0];
  localDW->zR[0] = localDW->obj.zR[0];
  localDW->zL[1] = localDW->obj.zL[1];
  localDW->zR[1] = localDW->obj.zR[1];
}

/* Termination for atomic system: */
void dist3_MATLABSystem5_Term(DW_MATLABSystem5_dist3_T *localDW)
{
  /* Terminate for MATLABSystem: '<Root>/MATLAB System5' */
  localDW->zL[0] = localDW->obj.zL[0];
  localDW->zR[0] = localDW->obj.zR[0];
  localDW->zL[1] = localDW->obj.zL[1];
  localDW->zR[1] = localDW->obj.zR[1];
}

static boolean_T dist3_isequal(const real_T varargin_1[257], const real_T
  varargin_2[257])
{
  int32_T b_k;
  boolean_T exitg1;
  boolean_T p;
  boolean_T p_0;
  p = false;

  /* Start for MATLABSystem: '<Root>/MATLAB System4' */
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

static void dist3_filter_m(const real_T b[15], const real_T x[256], const real_T
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
      /* Start for MATLABSystem: '<Root>/MATLAB System4' */
      zf[b_j] += b[(b_j - b_k) + 14] * x[b_k + 242];
    }
  }
}

static void dist3_filter_mb(const real_T b[15], const real_T x[512], const
  real_T zi[14], real_T y[512], real_T zf[14])
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
      /* Start for MATLABSystem: '<Root>/MATLAB System4' */
      zf[b_j] += b[(b_j - b_k) + 14] * x[b_k + 498];
    }
  }
}

static real_T dist3_lut_ws_sys_shapeOne(const lut_ws_sys_dist3_T *obj, real_T x,
  uint8_T md, real_T drive, real_T b, real_T cheb3, real_T cheb5, const real_T
  xGrid[257], const real_T yGrid[257])
{
  real_T y;

  /*  SHAPER (una muestra) */
  /*  pre: drive + bias */
  y = drive * x + b;

  /* Start for MATLABSystem: '<Root>/MATLAB System4' */
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

  /* End of Start for MATLABSystem: '<Root>/MATLAB System4' */
  return y;
}

static void dis_lut_ws_sys_processChannelOS(const lut_ws_sys_dist3_T *obj, const
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

  /* Start for MATLABSystem: '<Root>/MATLAB System4' */
  if (os == 0) {
    for (i = 0; i < 128; i++) {
      y[i] = dist3_lut_ws_sys_shapeOne(obj, x[i], md, drive, b, cheb3, cheb5,
        xGrid, yGrid);
    }
  } else if (os == 1) {
    memset(&u[0], 0, sizeof(real_T) << 8U);
    for (i = 0; i < 128; i++) {
      u[i << 1] = x[i];
    }

    memcpy(&zUp1_0[0], &zUp1[0], 14U * sizeof(real_T));
    dist3_filter_m(tmp, u, zUp1_0, b_y2, zUp1);

    /*  2N */
    for (i = 0; i < 256; i++) {
      y2[i] = dist3_lut_ws_sys_shapeOne(obj, b_y2[i], md, drive, b, cheb3, cheb5,
        xGrid, yGrid);
    }

    memcpy(&zUp1_0[0], &zDn1[0], 14U * sizeof(real_T));
    dist3_filter_m(tmp, y2, zUp1_0, u, zDn1);
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
    dist3_filter_m(tmp, u, zUp1_0, b_y2, zUp1);

    /*  2N */
    memset(&u_0[0], 0, sizeof(real_T) << 9U);
    for (i = 0; i < 256; i++) {
      u_0[i << 1] = b_y2[i];
    }

    memcpy(&zUp1_0[0], &zUp2[0], 14U * sizeof(real_T));
    dist3_filter_mb(tmp, u_0, zUp1_0, b_y2_0, zUp2);

    /*  4N */
    for (i = 0; i < 512; i++) {
      y4[i] = dist3_lut_ws_sys_shapeOne(obj, b_y2_0[i], md, drive, b, cheb3,
        cheb5, xGrid, yGrid);
    }

    memcpy(&zUp1_0[0], &zDn2[0], 14U * sizeof(real_T));
    dist3_filter_mb(tmp, y4, zUp1_0, u_0, zDn2);

    /*  2N */
    for (i = 0; i < 256; i++) {
      b_y2[i] = u_0[i << 1];
    }

    memcpy(&zUp1_0[0], &zDn1[0], 14U * sizeof(real_T));
    dist3_filter_m(tmp, b_y2, zUp1_0, u, zDn1);
    for (i = 0; i < 128; i++) {
      y[i] = u[i << 1];
    }

    /*  N */
  }

  /* End of Start for MATLABSystem: '<Root>/MATLAB System4' */
}

/* Model step function */
void dist3_step(void)
{
  real_T xGrid[257];
  real_T yGrid[257];
  real_T rtb_Gain[128];
  real_T rtb_Gain1[128];
  real_T rtb_MATLABSystem2_o1[128];
  real_T rtb_MATLABSystem2_o2[128];
  real_T c[14];
  real_T d[14];
  real_T e[14];
  real_T f[14];
  real_T a_;
  real_T b;
  real_T g;
  real_T gd1_;
  real_T gd2_;
  real_T gw1_;
  real_T gw2_;
  int32_T Mu;
  int32_T b_k;
  int32_T b_n;
  int32_T i;

  /* S-Function (pisound_in): '<Root>/pisound_in' */

  /* Level2 S-Function Block: '<Root>/pisound_in' (pisound_in) */
  {
    SimStruct *rts = dist3_M->childSfunctions[0];
    sfcnOutputs(rts,0);
  }

  for (i = 0; i < 128; i++) {
    /* Gain: '<Root>/Gain' incorporates:
     *  DataTypeConversion: '<Root>/Data Type Conversion'
     */
    rtb_Gain[i] = dist3_P.Gain_Gain * (real_T)dist3_B.pisound_in_o1[i];

    /* Gain: '<Root>/Gain1' incorporates:
     *  DataTypeConversion: '<Root>/Data Type Conversion2'
     */
    rtb_Gain1[i] = dist3_P.Gain1_Gain * (real_T)dist3_B.pisound_in_o2[i];
  }

  /* MATLABSystem: '<Root>/MATLAB System2' incorporates:
   *  Constant: '<Root>/Constant2'
   *  Constant: '<Root>/Constant4'
   */
  g = dist3_P.Gain_rt;

  /*  g es Nx1 */
  if (dist3_P.Constant4_Value != 0) {
    g = rt_powd_snf(10.0, (real_T)dist3_P.Gain_rt / 20.0);

    /*  dB -> lineal, elemento a elemento */
  }

  /*  bp es logical */
  /*  elemento a elemento */
  for (i = 0; i < 128; i++) {
    rtb_MATLABSystem2_o1[i] = rtb_Gain1[i] * g;
    rtb_MATLABSystem2_o2[i] = rtb_Gain[i] * g;
  }

  /* End of MATLABSystem: '<Root>/MATLAB System2' */

  /* Constant: '<Root>/Constant17' incorporates:
   *  Constant: '<Root>/Constant16'
   *  Constant: '<Root>/Constant18'
   *  Constant: '<Root>/Constant19'
   */
  dist3_MATLABSystem5(rtb_MATLABSystem2_o1, rtb_MATLABSystem2_o2,
                      dist3_P.Constant17_Value, dist3_P.Constant19_Value,
                      dist3_P.Constant16_Value, dist3_P.Constant18_Value,
                      &dist3_B.MATLABSystem7, &dist3_DW.MATLABSystem7);

  /* MATLABSystem: '<Root>/MATLAB System4' incorporates:
   *  Constant: '<Root>/Constant5'
   *  Constant: '<Root>/Constant6'
   *  Constant: '<Root>/Constant9'
   */
  if (dist3_DW.obj.ModeDefault != dist3_P.MATLABSystem4_ModeDefault) {
    dist3_DW.obj.ModeDefault = dist3_P.MATLABSystem4_ModeDefault;
  }

  if (dist3_DW.obj.AmountDefault != dist3_P.MATLABSystem4_AmountDefault) {
    dist3_DW.obj.AmountDefault = dist3_P.MATLABSystem4_AmountDefault;
  }

  if (dist3_DW.obj.AsymDefault != dist3_P.MATLABSystem4_AsymDefault) {
    dist3_DW.obj.AsymDefault = dist3_P.MATLABSystem4_AsymDefault;
  }

  if (dist3_DW.obj.Cheb3Default != dist3_P.MATLABSystem4_Cheb3Default) {
    dist3_DW.obj.Cheb3Default = dist3_P.MATLABSystem4_Cheb3Default;
  }

  if (dist3_DW.obj.Cheb5Default != dist3_P.MATLABSystem4_Cheb5Default) {
    dist3_DW.obj.Cheb5Default = dist3_P.MATLABSystem4_Cheb5Default;
  }

  if (!dist3_isequal(dist3_DW.obj.XUser, dist3_P.MATLABSystem4_XUser)) {
    memcpy(&dist3_DW.obj.XUser[0], &dist3_P.MATLABSystem4_XUser[0], 257U *
           sizeof(real_T));
  }

  if (!dist3_isequal(dist3_DW.obj.YUser, dist3_P.MATLABSystem4_YUser)) {
    memcpy(&dist3_DW.obj.YUser[0], &dist3_P.MATLABSystem4_YUser[0], 257U *
           sizeof(real_T));
  }

  if (dist3_DW.obj.MUser != dist3_P.MATLABSystem4_MUser) {
    dist3_DW.obj.MUser = dist3_P.MATLABSystem4_MUser;
  }

  if (dist3_DW.obj.Eps != dist3_P.MATLABSystem4_Eps) {
    dist3_DW.obj.Eps = dist3_P.MATLABSystem4_Eps;
  }

  /*  inL,inR,mode,amount,asym,cheb3,cheb5 */
  /*  Nloc = size(inL,1); */
  /*  outL = zeros(Nloc,1); */
  /*  outR = zeros(Nloc,1); */
  /*  bp es logical */
  /*  --- defaults --- */
  b_n = (int32_T)fmin(fmax(dist3_P.Shaper_rt, 1.0), 6.0);

  /*  por ejemplo */
  if (dist3_P.Asym_rt < -0.5) {
    b = -0.5;
  } else if (dist3_P.Asym_rt > 0.5) {
    b = 0.5;
  } else {
    b = dist3_P.Asym_rt;
  }

  /*  drive interno (lineal) */
  if (dist3_P.Amount_rt < 0.0) {
    g = 0.0;
  } else if (dist3_P.Amount_rt > 60.0) {
    g = 60.0;
  } else {
    g = dist3_P.Amount_rt;
  }

  g = rt_powd_snf(10.0, g / 20.0);

  /*  preparar LUT usuario (si aplica) */
  /*  (tamaño fijo M, relleno seguro) */
  memset(&xGrid[0], 0, 257U * sizeof(real_T));
  memset(&yGrid[0], 0, 257U * sizeof(real_T));
  if (b_n == 6) {
    i = (int32_T)rt_roundd_snf(fmin(fmax(dist3_DW.obj.MUser, 1.0), 257.0));
    memcpy(&xGrid[0], &dist3_DW.obj.XUser[0], (uint32_T)((i - 1) + 1) * sizeof
           (real_T));
    memcpy(&yGrid[0], &dist3_DW.obj.YUser[0], (uint32_T)((i - 1) + 1) * sizeof
           (real_T));
    Mu = i + 1;
    for (b_k = Mu; b_k < 258; b_k++) {
      xGrid[b_k - 1] = xGrid[i - 1];
      yGrid[b_k - 1] = yGrid[i - 1];
    }
  }

  /* Start for MATLABSystem: '<Root>/MATLAB System4' incorporates:
   *  Constant: '<Root>/Constant10'
   *  Constant: '<Root>/Constant11'
   *  MATLABSystem: '<Root>/MATLAB System7'
   */
  memcpy(&c[0], &dist3_DW.obj.zUp1L[0], 14U * sizeof(real_T));
  memcpy(&d[0], &dist3_DW.obj.zUp2L[0], 14U * sizeof(real_T));
  memcpy(&e[0], &dist3_DW.obj.zDn1L[0], 14U * sizeof(real_T));
  memcpy(&f[0], &dist3_DW.obj.zDn2L[0], 14U * sizeof(real_T));
  dis_lut_ws_sys_processChannelOS(&dist3_DW.obj,
    dist3_B.MATLABSystem7.MATLABSystem5_o1, 2, (uint8_T)b_n, g, b,
    dist3_P.Constant10_Value, dist3_P.Constant11_Value, xGrid, yGrid, c, d, e, f,
    rtb_MATLABSystem2_o1);
  for (i = 0; i < 14; i++) {
    /* MATLABSystem: '<Root>/MATLAB System4' */
    dist3_DW.obj.zUp1L[i] = c[i];
    dist3_DW.obj.zUp2L[i] = d[i];
    dist3_DW.obj.zDn1L[i] = e[i];
    dist3_DW.obj.zDn2L[i] = f[i];

    /* Start for MATLABSystem: '<Root>/MATLAB System4' */
    c[i] = dist3_DW.obj.zUp1R[i];
    d[i] = dist3_DW.obj.zUp2R[i];
    e[i] = dist3_DW.obj.zDn1R[i];
    f[i] = dist3_DW.obj.zDn2R[i];
  }

  /* Start for MATLABSystem: '<Root>/MATLAB System4' incorporates:
   *  Constant: '<Root>/Constant10'
   *  Constant: '<Root>/Constant11'
   *  MATLABSystem: '<Root>/MATLAB System7'
   */
  dis_lut_ws_sys_processChannelOS(&dist3_DW.obj,
    dist3_B.MATLABSystem7.MATLABSystem5_o2, 2, (uint8_T)b_n, g, b,
    dist3_P.Constant10_Value, dist3_P.Constant11_Value, xGrid, yGrid, c, d, e, f,
    rtb_MATLABSystem2_o2);

  /* MATLABSystem: '<Root>/MATLAB System4' */
  memcpy(&dist3_DW.obj.zUp1R[0], &c[0], 14U * sizeof(real_T));
  memcpy(&dist3_DW.obj.zUp2R[0], &d[0], 14U * sizeof(real_T));
  memcpy(&dist3_DW.obj.zDn1R[0], &e[0], 14U * sizeof(real_T));
  memcpy(&dist3_DW.obj.zDn2R[0], &f[0], 14U * sizeof(real_T));

  /* MATLABSystem: '<Root>/MATLAB System' incorporates:
   *  Constant: '<Root>/Constant1'
   *  Constant: '<Root>/Constant8'
   *  MATLABSystem: '<Root>/MATLAB System4'
   */
  g = dist3_P.Level_rt;

  /*  g es Nx1 */
  if (dist3_P.Constant1_Value != 0) {
    g = rt_powd_snf(10.0, dist3_P.Level_rt / 20.0);

    /*  dB -> lineal, elemento a elemento */
  }

  /*  bp es logical */
  /*  elemento a elemento */
  for (b_n = 0; b_n < 128; b_n++) {
    rtb_MATLABSystem2_o1[b_n] *= g;
    rtb_MATLABSystem2_o2[b_n] *= g;
  }

  /* End of MATLABSystem: '<Root>/MATLAB System' */

  /* Constant: '<Root>/Constant13' incorporates:
   *  Constant: '<Root>/Constant12'
   *  Constant: '<Root>/Constant14'
   *  Constant: '<Root>/Constant7'
   */
  dist3_MATLABSystem5(rtb_MATLABSystem2_o1, rtb_MATLABSystem2_o2,
                      dist3_P.Constant13_Value, dist3_P.Tone_rt,
                      dist3_P.Constant12_Value, dist3_P.Constant14_Value,
                      &dist3_B.MATLABSystem5, &dist3_DW.MATLABSystem5);

  /* MATLABSystem: '<Root>/MATLAB System6' incorporates:
   *  Constant: '<Root>/Constant15'
   */
  if (dist3_DW.obj_e.SmoothTime != dist3_P.MATLABSystem6_SmoothTime) {
    dist3_DW.obj_e.SmoothTime = dist3_P.MATLABSystem6_SmoothTime;
  }

  if (dist3_DW.obj_e.IndepInDB != dist3_P.MATLABSystem6_IndepInDB) {
    dist3_DW.obj_e.IndepInDB = dist3_P.MATLABSystem6_IndepInDB;
  }

  /*  inL,inR,wetL,wetR,p1 (+p2 si Mode=2) */
  /*  dummy */
  b = dist3_P.Mix_rt;
  if (dist3_P.Mix_rt < 0.0) {
    b = 0.0;
  } else if (dist3_P.Mix_rt > 1.0) {
    b = 1.0;
  }

  a_ = dist3_DW.obj_e.a;
  gd1_ = dist3_DW.obj_e.gd1;
  gd2_ = dist3_DW.obj_e.gd2;
  gw1_ = dist3_DW.obj_e.gw1;
  gw2_ = dist3_DW.obj_e.gw2;

  /*  más robusto que obj.N */
  for (b_n = 0; b_n < 128; b_n++) {
    gd1_ += ((1.0 - b) - gd1_) * a_;
    gw1_ += (b - gw1_) * a_;
    gd2_ += ((1.0 - b) - gd2_) * a_;
    gw2_ += (b - gw2_) * a_;

    /* DataTypeConversion: '<Root>/Data Type Conversion1' incorporates:
     *  Gain: '<Root>/Gain2'
     *  MATLABSystem: '<Root>/MATLAB System5'
     */
    g = floor((gd1_ * rtb_Gain[b_n] + gw1_ *
               dist3_B.MATLABSystem5.MATLABSystem5_o1[b_n]) * dist3_P.Gain2_Gain);
    if (rtIsNaN(g) || rtIsInf(g)) {
      g = 0.0;
    } else {
      g = fmod(g, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion1' */
    dist3_B.DataTypeConversion1[b_n] = g < 0.0 ? -(int32_T)(uint32_T)-g :
      (int32_T)(uint32_T)g;

    /* DataTypeConversion: '<Root>/Data Type Conversion3' incorporates:
     *  Gain: '<Root>/Gain3'
     *  MATLABSystem: '<Root>/MATLAB System5'
     */
    g = floor((gd2_ * rtb_Gain1[b_n] + gw2_ *
               dist3_B.MATLABSystem5.MATLABSystem5_o2[b_n]) * dist3_P.Gain3_Gain);
    if (rtIsNaN(g) || rtIsInf(g)) {
      g = 0.0;
    } else {
      g = fmod(g, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion3' */
    dist3_B.DataTypeConversion3[b_n] = g < 0.0 ? -(int32_T)(uint32_T)-g :
      (int32_T)(uint32_T)g;
  }

  dist3_DW.obj_e.gd1 = gd1_;
  dist3_DW.obj_e.gd2 = gd2_;
  dist3_DW.obj_e.gw1 = gw1_;
  dist3_DW.obj_e.gw2 = gw2_;

  /* End of MATLABSystem: '<Root>/MATLAB System6' */

  /* S-Function (pisound_out): '<Root>/pisound_out' */

  /* Level2 S-Function Block: '<Root>/pisound_out' (pisound_out) */
  {
    SimStruct *rts = dist3_M->childSfunctions[1];
    sfcnOutputs(rts,0);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  dist3_M->Timing.t[0] =
    ((time_T)(++dist3_M->Timing.clockTick0)) * dist3_M->Timing.stepSize0;
}

/* Model initialize function */
void dist3_initialize(void)
{
  /* Registration code */
  rtsiSetSolverName(&dist3_M->solverInfo,"FixedStepDiscrete");
  dist3_M->solverInfoPtr = (&dist3_M->solverInfo);

  /* Initialize timing info */
  {
    int_T *mdlTsMap = dist3_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    dist3_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    dist3_M->Timing.sampleTimes = (&dist3_M->Timing.sampleTimesArray[0]);
    dist3_M->Timing.offsetTimes = (&dist3_M->Timing.offsetTimesArray[0]);

    /* task periods */
    dist3_M->Timing.sampleTimes[0] = (0.0026666666666666666);

    /* task offsets */
    dist3_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(dist3_M, &dist3_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = dist3_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    dist3_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(dist3_M, -1);
  dist3_M->Timing.stepSize0 = 0.0026666666666666666;
  dist3_M->solverInfoPtr = (&dist3_M->solverInfo);
  dist3_M->Timing.stepSize = (0.0026666666666666666);
  rtsiSetFixedStepSize(&dist3_M->solverInfo, 0.0026666666666666666);
  rtsiSetSolverMode(&dist3_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* child S-Function registration */
  {
    RTWSfcnInfo *sfcnInfo = &dist3_M->NonInlinedSFcns.sfcnInfo;
    dist3_M->sfcnInfo = (sfcnInfo);
    rtssSetErrorStatusPtr(sfcnInfo, (&rtmGetErrorStatus(dist3_M)));
    dist3_M->Sizes.numSampTimes = (1);
    rtssSetNumRootSampTimesPtr(sfcnInfo, &dist3_M->Sizes.numSampTimes);
    dist3_M->NonInlinedSFcns.taskTimePtrs[0] = (&rtmGetTPtr(dist3_M)[0]);
    rtssSetTPtrPtr(sfcnInfo,dist3_M->NonInlinedSFcns.taskTimePtrs);
    rtssSetTStartPtr(sfcnInfo, &rtmGetTStart(dist3_M));
    rtssSetTFinalPtr(sfcnInfo, &rtmGetTFinal(dist3_M));
    rtssSetTimeOfLastOutputPtr(sfcnInfo, &rtmGetTimeOfLastOutput(dist3_M));
    rtssSetStepSizePtr(sfcnInfo, &dist3_M->Timing.stepSize);
    rtssSetStopRequestedPtr(sfcnInfo, &rtmGetStopRequested(dist3_M));
    rtssSetDerivCacheNeedsResetPtr(sfcnInfo, &dist3_M->derivCacheNeedsReset);
    rtssSetZCCacheNeedsResetPtr(sfcnInfo, &dist3_M->zCCacheNeedsReset);
    rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(sfcnInfo,
      &dist3_M->CTOutputIncnstWithState);
    rtssSetSampleHitsPtr(sfcnInfo, &dist3_M->Timing.sampleHits);
    rtssSetPerTaskSampleHitsPtr(sfcnInfo, &dist3_M->Timing.perTaskSampleHits);
    rtssSetSimModePtr(sfcnInfo, &dist3_M->simMode);
    rtssSetSolverInfoPtr(sfcnInfo, &dist3_M->solverInfoPtr);
  }

  dist3_M->Sizes.numSFcns = (2);

  /* register each child */
  {
    (void) memset((void *)&dist3_M->NonInlinedSFcns.childSFunctions[0], 0,
                  2*sizeof(SimStruct));
    dist3_M->childSfunctions = (&dist3_M->NonInlinedSFcns.childSFunctionPtrs[0]);
    dist3_M->childSfunctions[0] = (&dist3_M->NonInlinedSFcns.childSFunctions[0]);
    dist3_M->childSfunctions[1] = (&dist3_M->NonInlinedSFcns.childSFunctions[1]);

    /* Level2 S-Function Block: dist3/<Root>/pisound_in (pisound_in) */
    {
      SimStruct *rts = dist3_M->childSfunctions[0];

      /* timing info */
      time_T *sfcnPeriod = dist3_M->NonInlinedSFcns.Sfcn0.sfcnPeriod;
      time_T *sfcnOffset = dist3_M->NonInlinedSFcns.Sfcn0.sfcnOffset;
      int_T *sfcnTsMap = dist3_M->NonInlinedSFcns.Sfcn0.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &dist3_M->NonInlinedSFcns.blkInfo2[0]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &dist3_M->NonInlinedSFcns.inputOutputPortInfo2[0]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, dist3_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &dist3_M->NonInlinedSFcns.methods2[0]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &dist3_M->NonInlinedSFcns.methods3[0]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &dist3_M->NonInlinedSFcns.methods4[0]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &dist3_M->NonInlinedSFcns.statesInfo2[0]);
        ssSetPeriodicStatesInfo(rts,
          &dist3_M->NonInlinedSFcns.periodicStatesInfo[0]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &dist3_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &dist3_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 4);
        _ssSetPortInfo2ForOutputUnits(rts,
          &dist3_M->NonInlinedSFcns.Sfcn0.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        ssSetOutputPortUnit(rts, 2, 0);
        ssSetOutputPortUnit(rts, 3, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &dist3_M->NonInlinedSFcns.Sfcn0.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 2, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 3, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 128);
          ssSetOutputPortSignal(rts, 0, ((int32_T *) dist3_B.pisound_in_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 128);
          ssSetOutputPortSignal(rts, 1, ((int32_T *) dist3_B.pisound_in_o2));
        }

        /* port 2 */
        {
          _ssSetOutputPortNumDimensions(rts, 2, 1);
          ssSetOutputPortWidthAsInt(rts, 2, 128);
          ssSetOutputPortSignal(rts, 2, ((uint8_T *) dist3_B.pisound_in_o3));
        }

        /* port 3 */
        {
          _ssSetOutputPortNumDimensions(rts, 3, 1);
          ssSetOutputPortWidthAsInt(rts, 3, 1);
          ssSetOutputPortSignal(rts, 3, ((uint32_T *) &dist3_B.pisound_in_o4));
        }
      }

      /* path info */
      ssSetModelName(rts, "pisound_in");
      ssSetPath(rts, "dist3/pisound_in");
      ssSetRTModel(rts,dist3_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &dist3_M->NonInlinedSFcns.Sfcn0.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)dist3_P.pisound_in_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)dist3_P.pisound_in_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)dist3_P.pisound_in_P3_Size);
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
      _ssSetOutputPortConnected(rts, 2, 1);
      _ssSetOutputPortConnected(rts, 3, 1);
      _ssSetOutputPortBeingMerged(rts, 0, 0);
      _ssSetOutputPortBeingMerged(rts, 1, 0);
      _ssSetOutputPortBeingMerged(rts, 2, 0);
      _ssSetOutputPortBeingMerged(rts, 3, 0);

      /* Update the BufferDstPort flags for each input port */
    }

    /* Level2 S-Function Block: dist3/<Root>/pisound_out (pisound_out) */
    {
      SimStruct *rts = dist3_M->childSfunctions[1];

      /* timing info */
      time_T *sfcnPeriod = dist3_M->NonInlinedSFcns.Sfcn1.sfcnPeriod;
      time_T *sfcnOffset = dist3_M->NonInlinedSFcns.Sfcn1.sfcnOffset;
      int_T *sfcnTsMap = dist3_M->NonInlinedSFcns.Sfcn1.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &dist3_M->NonInlinedSFcns.blkInfo2[1]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &dist3_M->NonInlinedSFcns.inputOutputPortInfo2[1]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, dist3_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &dist3_M->NonInlinedSFcns.methods2[1]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &dist3_M->NonInlinedSFcns.methods3[1]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &dist3_M->NonInlinedSFcns.methods4[1]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &dist3_M->NonInlinedSFcns.statesInfo2[1]);
        ssSetPeriodicStatesInfo(rts,
          &dist3_M->NonInlinedSFcns.periodicStatesInfo[1]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 4);
        ssSetPortInfoForInputs(rts,
          &dist3_M->NonInlinedSFcns.Sfcn1.inputPortInfo[0]);
        ssSetPortInfoForInputs(rts,
          &dist3_M->NonInlinedSFcns.Sfcn1.inputPortInfo[0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &dist3_M->NonInlinedSFcns.Sfcn1.inputPortUnits[0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        ssSetInputPortUnit(rts, 2, 0);
        ssSetInputPortUnit(rts, 3, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &dist3_M->NonInlinedSFcns.Sfcn1.inputPortCoSimAttribute[0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);
        ssSetInputPortIsContinuousQuantity(rts, 2, 0);
        ssSetInputPortIsContinuousQuantity(rts, 3, 0);

        /* port 0 */
        {
          int32_T const **sfcnUPtrs = (int32_T const **)
            &dist3_M->NonInlinedSFcns.Sfcn1.UPtrs0;

          {
            int_T i1;
            const int32_T *u0 = dist3_B.DataTypeConversion1;
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
            &dist3_M->NonInlinedSFcns.Sfcn1.UPtrs1;

          {
            int_T i1;
            const int32_T *u1 = dist3_B.DataTypeConversion3;
            for (i1=0; i1 < 128; i1++) {
              sfcnUPtrs[i1] = &u1[i1];
            }
          }

          ssSetInputPortSignalPtrs(rts, 1, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 1, 1);
          ssSetInputPortWidthAsInt(rts, 1, 128);
        }

        /* port 2 */
        {
          uint8_T const **sfcnUPtrs = (uint8_T const **)
            &dist3_M->NonInlinedSFcns.Sfcn1.UPtrs2;

          {
            int_T i1;
            const uint8_T *u2 = dist3_B.pisound_in_o3;
            for (i1=0; i1 < 128; i1++) {
              sfcnUPtrs[i1] = &u2[i1];
            }
          }

          ssSetInputPortSignalPtrs(rts, 2, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 2, 1);
          ssSetInputPortWidthAsInt(rts, 2, 128);
        }

        /* port 3 */
        {
          uint32_T const **sfcnUPtrs = (uint32_T const **)
            &dist3_M->NonInlinedSFcns.Sfcn1.UPtrs3;
          sfcnUPtrs[0] = &dist3_B.pisound_in_o4;
          ssSetInputPortSignalPtrs(rts, 3, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 3, 1);
          ssSetInputPortWidthAsInt(rts, 3, 1);
        }
      }

      /* path info */
      ssSetModelName(rts, "pisound_out");
      ssSetPath(rts, "dist3/pisound_out");
      ssSetRTModel(rts,dist3_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &dist3_M->NonInlinedSFcns.Sfcn1.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)dist3_P.pisound_out_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)dist3_P.pisound_out_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)dist3_P.pisound_out_P3_Size);
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
      _ssSetInputPortConnected(rts, 2, 1);
      _ssSetInputPortConnected(rts, 3, 1);

      /* Update the BufferDstPort flags for each input port */
      ssSetInputPortBufferDstPort(rts, 0, -1);
      ssSetInputPortBufferDstPort(rts, 1, -1);
      ssSetInputPortBufferDstPort(rts, 2, -1);
      ssSetInputPortBufferDstPort(rts, 3, -1);
    }
  }

  dist3_MATLABSystem5_Init(&dist3_DW.MATLABSystem7);

  /* Start for MATLABSystem: '<Root>/MATLAB System4' */
  dist3_DW.obj.ModeDefault = dist3_P.MATLABSystem4_ModeDefault;
  dist3_DW.obj.AmountDefault = dist3_P.MATLABSystem4_AmountDefault;
  dist3_DW.obj.AsymDefault = dist3_P.MATLABSystem4_AsymDefault;
  dist3_DW.obj.Cheb3Default = dist3_P.MATLABSystem4_Cheb3Default;
  dist3_DW.obj.Cheb5Default = dist3_P.MATLABSystem4_Cheb5Default;
  memcpy(&dist3_DW.obj.XUser[0], &dist3_P.MATLABSystem4_XUser[0], 257U * sizeof
         (real_T));
  memcpy(&dist3_DW.obj.YUser[0], &dist3_P.MATLABSystem4_YUser[0], 257U * sizeof
         (real_T));
  dist3_DW.obj.MUser = dist3_P.MATLABSystem4_MUser;
  dist3_DW.obj.Eps = dist3_P.MATLABSystem4_Eps;
  dist3_DW.obj.isInitialized = 1;

  /* InitializeConditions for MATLABSystem: '<Root>/MATLAB System4' */
  /*  Validaciones */
  memset(&dist3_DW.obj.zUp1L[0], 0, 14U * sizeof(real_T));
  memset(&dist3_DW.obj.zUp1R[0], 0, 14U * sizeof(real_T));
  memset(&dist3_DW.obj.zDn1L[0], 0, 14U * sizeof(real_T));
  memset(&dist3_DW.obj.zDn1R[0], 0, 14U * sizeof(real_T));
  memset(&dist3_DW.obj.zUp2L[0], 0, 14U * sizeof(real_T));
  memset(&dist3_DW.obj.zUp2R[0], 0, 14U * sizeof(real_T));
  memset(&dist3_DW.obj.zDn2L[0], 0, 14U * sizeof(real_T));
  memset(&dist3_DW.obj.zDn2R[0], 0, 14U * sizeof(real_T));
  dist3_MATLABSystem5_Init(&dist3_DW.MATLABSystem5);

  /* Start for MATLABSystem: '<Root>/MATLAB System6' */
  dist3_DW.obj_e.SmoothTime = dist3_P.MATLABSystem6_SmoothTime;
  dist3_DW.obj_e.IndepInDB = dist3_P.MATLABSystem6_IndepInDB;
  dist3_DW.obj_e.isInitialized = 1;

  /*  inL,inR,wetL,wetR,p1 (+p2 si Mode=2) */
  if (dist3_DW.obj_e.SmoothTime <= 0.0) {
    dist3_DW.obj_e.a = 1.0;
  } else {
    dist3_DW.obj_e.a = 1.0 - exp(-1.0 / (48000.0 * dist3_DW.obj_e.SmoothTime));
  }

  /* InitializeConditions for MATLABSystem: '<Root>/MATLAB System6' */
  dist3_DW.obj_e.gd1 = 1.0;
  dist3_DW.obj_e.gd2 = 1.0;
  dist3_DW.obj_e.gw1 = 0.0;
  dist3_DW.obj_e.gw2 = 0.0;
}

/* Model terminate function */
void dist3_terminate(void)
{
  /* Terminate for S-Function (pisound_in): '<Root>/pisound_in' */
  /* Level2 S-Function Block: '<Root>/pisound_in' (pisound_in) */
  {
    SimStruct *rts = dist3_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  dist3_MATLABSystem5_Term(&dist3_DW.MATLABSystem7);
  dist3_MATLABSystem5_Term(&dist3_DW.MATLABSystem5);

  /* Terminate for S-Function (pisound_out): '<Root>/pisound_out' */
  /* Level2 S-Function Block: '<Root>/pisound_out' (pisound_out) */
  {
    SimStruct *rts = dist3_M->childSfunctions[1];
    sfcnTerminate(rts);
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
