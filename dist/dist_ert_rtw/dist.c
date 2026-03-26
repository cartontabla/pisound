/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: dist.c
 *
 * Code generated for Simulink model 'dist'.
 *
 * Model version                  : 1.41
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Fri Mar  6 12:08:05 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "dist.h"
#include "rtwtypes.h"
#include "dist_types.h"
#include <string.h>
#include <math.h>
#include "dist_private.h"
#include "rt_nonfinite.h"

/* Block signals (default storage) */
B_dist_T dist_B;

/* Block states (default storage) */
DW_dist_T dist_DW;

/* Real-time model */
static RT_MODEL_dist_T dist_M_;
RT_MODEL_dist_T *const dist_M = &dist_M_;

/* Forward declaration for local functions */
static boolean_T dist_isequal(const real_T varargin_1[257], const real_T
  varargin_2[257]);
static void dist_filter(const real_T b[15], const real_T x[256], const real_T
  zi[14], real_T y[256], real_T zf[14]);
static void dist_filter_k(const real_T b[15], const real_T x[512], const real_T
  zi[14], real_T y[512], real_T zf[14]);
static real_T dist_lut_ws_sys_shapeOne(const lut_ws_sys_dist_T *obj, real_T x,
  uint8_T md, real_T drive, real_T b, real_T cheb3, real_T cheb5, const real_T
  xGrid[257], const real_T yGrid[257]);
static void dis_lut_ws_sys_processChannelOS(const lut_ws_sys_dist_T *obj, const
  real_T x[128], uint8_T os, uint8_T md, real_T drive, real_T b, real_T cheb3,
  real_T cheb5, const real_T xGrid[257], const real_T yGrid[257], real_T zUp1[14],
  real_T zUp2[14], real_T zDn1[14], real_T zDn2[14], real_T y[128]);
static void dist_filter_kn(real_T b[3], real_T a[3], const real_T x[128], const
  real_T zi[2], real_T y[128], real_T zf[2]);
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

static boolean_T dist_isequal(const real_T varargin_1[257], const real_T
  varargin_2[257])
{
  int32_T b_k;
  boolean_T exitg1;
  boolean_T p;
  boolean_T p_0;
  p = false;

  /* Start for MATLABSystem: '<Root>/LUT-WS' */
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

static void dist_filter(const real_T b[15], const real_T x[256], const real_T
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
      /* Start for MATLABSystem: '<Root>/LUT-WS' */
      zf[b_j] += b[(b_j - b_k) + 14] * x[b_k + 242];
    }
  }
}

static void dist_filter_k(const real_T b[15], const real_T x[512], const real_T
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
      /* Start for MATLABSystem: '<Root>/LUT-WS' */
      zf[b_j] += b[(b_j - b_k) + 14] * x[b_k + 498];
    }
  }
}

static real_T dist_lut_ws_sys_shapeOne(const lut_ws_sys_dist_T *obj, real_T x,
  uint8_T md, real_T drive, real_T b, real_T cheb3, real_T cheb5, const real_T
  xGrid[257], const real_T yGrid[257])
{
  real_T y;

  /*  SHAPER (una muestra) */
  /*  pre: drive + bias */
  y = drive * x + b;

  /* Start for MATLABSystem: '<Root>/LUT-WS' */
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

  /* End of Start for MATLABSystem: '<Root>/LUT-WS' */
  return y;
}

static void dis_lut_ws_sys_processChannelOS(const lut_ws_sys_dist_T *obj, const
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

  /* Start for MATLABSystem: '<Root>/LUT-WS' */
  if (os == 0) {
    for (i = 0; i < 128; i++) {
      y[i] = dist_lut_ws_sys_shapeOne(obj, x[i], md, drive, b, cheb3, cheb5,
        xGrid, yGrid);
    }
  } else if (os == 1) {
    memset(&u[0], 0, sizeof(real_T) << 8U);
    for (i = 0; i < 128; i++) {
      u[i << 1] = x[i];
    }

    memcpy(&zUp1_0[0], &zUp1[0], 14U * sizeof(real_T));
    dist_filter(tmp, u, zUp1_0, b_y2, zUp1);

    /*  2N */
    for (i = 0; i < 256; i++) {
      y2[i] = dist_lut_ws_sys_shapeOne(obj, b_y2[i], md, drive, b, cheb3, cheb5,
        xGrid, yGrid);
    }

    memcpy(&zUp1_0[0], &zDn1[0], 14U * sizeof(real_T));
    dist_filter(tmp, y2, zUp1_0, u, zDn1);
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
    dist_filter(tmp, u, zUp1_0, b_y2, zUp1);

    /*  2N */
    memset(&u_0[0], 0, sizeof(real_T) << 9U);
    for (i = 0; i < 256; i++) {
      u_0[i << 1] = b_y2[i];
    }

    memcpy(&zUp1_0[0], &zUp2[0], 14U * sizeof(real_T));
    dist_filter_k(tmp, u_0, zUp1_0, b_y2_0, zUp2);

    /*  4N */
    for (i = 0; i < 512; i++) {
      y4[i] = dist_lut_ws_sys_shapeOne(obj, b_y2_0[i], md, drive, b, cheb3,
        cheb5, xGrid, yGrid);
    }

    memcpy(&zUp1_0[0], &zDn2[0], 14U * sizeof(real_T));
    dist_filter_k(tmp, y4, zUp1_0, u_0, zDn2);

    /*  2N */
    for (i = 0; i < 256; i++) {
      b_y2[i] = u_0[i << 1];
    }

    memcpy(&zUp1_0[0], &zDn1[0], 14U * sizeof(real_T));
    dist_filter(tmp, b_y2, zUp1_0, u, zDn1);
    for (i = 0; i < 128; i++) {
      y[i] = u[i << 1];
    }

    /*  N */
  }

  /* End of Start for MATLABSystem: '<Root>/LUT-WS' */
}

static void dist_filter_kn(real_T b[3], real_T a[3], const real_T x[128], const
  real_T zi[2], real_T y[128], real_T zf[2])
{
  int32_T b_j;
  int32_T e_k;
  int32_T naxpy;

  /* Start for MATLABSystem: '<Root>/VCF' */
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

    /* Start for MATLABSystem: '<Root>/VCF' */
    if (128 - e_k <= 3) {
      naxpy = 128 - e_k;
    } else {
      naxpy = 3;
    }

    for (b_j = 0; b_j < naxpy; b_j++) {
      y_tmp = e_k + b_j;

      /* Start for MATLABSystem: '<Root>/VCF' */
      y[y_tmp] += x[e_k] * b[b_j];
    }

    /* Start for MATLABSystem: '<Root>/VCF' */
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
      /* Start for MATLABSystem: '<Root>/VCF' */
      zf[naxpy] += b[(naxpy - e_k) + 2] * x[e_k + 126];
    }
  }

  for (e_k = 0; e_k < 2; e_k++) {
    for (naxpy = 0; naxpy <= e_k; naxpy++) {
      /* Start for MATLABSystem: '<Root>/VCF' */
      zf[naxpy] += a[(naxpy - e_k) + 2] * -y[e_k + 126];
    }
  }
}

/* Model step function */
void dist_step(void)
{
  real_T xGrid[257];
  real_T yGrid[257];
  real_T rtb_ARF_0[128];
  real_T rtb_ENV_o1_0[128];
  real_T rtb_Gain[128];
  real_T rtb_Gain1[128];
  real_T rtb_Gain1_0[128];
  real_T rtb_Gain_0[128];
  real_T c[14];
  real_T d[14];
  real_T e_0[14];
  real_T f[14];
  real_T tmp_0[3];
  real_T tmp_1[3];
  real_T tmp[2];
  real_T A;
  real_T a0;
  real_T a0_tmp;
  real_T a0_tmp_0;
  real_T alpha;
  real_T alphaR;
  real_T e;
  real_T f_0;
  real_T p;
  real_T q;
  real_T sqrtA;
  real_T wc;
  int32_T Mu;
  int32_T b_k;
  int32_T i;
  int32_T t;

  /* S-Function (pisound_in): '<Root>/PiSound Input' */

  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = dist_M->childSfunctions[0];
    sfcnOutputs(rts,0);
  }

  for (i = 0; i < 128; i++) {
    /* Gain: '<Root>/Gain' incorporates:
     *  DataTypeConversion: '<Root>/Data Type Conversion'
     */
    rtb_Gain[i] = dist_P.Gain_Gain * (real_T)dist_B.PiSoundInput_o1[i];

    /* Gain: '<Root>/Gain1' incorporates:
     *  DataTypeConversion: '<Root>/Data Type Conversion2'
     */
    rtb_Gain1[i] = dist_P.Gain1_Gain * (real_T)dist_B.PiSoundInput_o2[i];
  }

  /* MATLABSystem: '<Root>/ENV' incorporates:
   *  Constant: '<Root>/Constant'
   */
  if (dist_DW.obj_n.ModeDefault != dist_P.ENV_ModeDefault) {
    dist_DW.obj_n.ModeDefault = dist_P.ENV_ModeDefault;
  }

  if (dist_DW.obj_n.Eps != dist_P.ENV_Eps) {
    dist_DW.obj_n.Eps = dist_P.ENV_Eps;
  }

  if (dist_DW.obj_n.TauRMS != dist_P.ENV_TauRMS) {
    dist_DW.obj_n.TauRMS = dist_P.ENV_TauRMS;
  }

  memset(&rtb_ENV_o1_0[0], 0, sizeof(real_T) << 7U);
  if (dist_P.Constant_Value == 1) {
    /*  -------- PEAK (instantáneo) -------- */
    for (t = 0; t < 128; t++) {
      rtb_ENV_o1_0[t] = log10(fmax(fabs(rtb_Gain[t]), fabs(rtb_Gain1[t])) +
        dist_DW.obj_n.Eps) * 20.0;
    }
  } else if (dist_P.Constant_Value == 2) {
    /*  -------- RMS (instantáneo por muestra) -------- */
    /*  RMS instantáneo por muestra: sqrt(max(x^2)) */
    for (t = 0; t < 128; t++) {
      e = rtb_Gain[t];
      alphaR = rtb_Gain1[t];
      rtb_ENV_o1_0[t] = log10(sqrt(fmax(e * e, alphaR * alphaR) +
        dist_DW.obj_n.Eps)) * 20.0;

      /*  ya tiene eps dentro */
    }
  } else if (dist_P.Constant_Value == 3) {
    /*  -------- RMS exponencial (real) -------- */
    alpha = exp(-1.0 / (dist_DW.obj_n.TauRMS * 48000.0));
    for (t = 0; t < 128; t++) {
      e = rtb_Gain[t];
      alphaR = rtb_Gain1[t];
      e = (e * e + alphaR * alphaR) * 0.5 * (1.0 - alpha) + alpha *
        dist_DW.obj_n.e_prev;
      dist_DW.obj_n.e_prev = e;
      rtb_ENV_o1_0[t] = log10(sqrt(e + dist_DW.obj_n.Eps)) * 20.0;
    }
  }

  /* MATLABSystem: '<Root>/ARF' incorporates:
   *  Constant: '<Root>/Constant2'
   *  Constant: '<Root>/Constant3'
   *  MATLABSystem: '<Root>/ENV'
   */
  if (dist_DW.obj_m.AttackDefault != dist_P.ARF_AttackDefault) {
    dist_DW.obj_m.AttackDefault = dist_P.ARF_AttackDefault;
  }

  if (dist_DW.obj_m.ReleaseDefault != dist_P.ARF_ReleaseDefault) {
    dist_DW.obj_m.ReleaseDefault = dist_P.ARF_ReleaseDefault;
  }

  /*  Evitar tiempos no válidos */
  alpha = exp(-1.0 / (fmax(dist_P.Attack_rt, 1.0E-6) * 48000.0));
  alphaR = exp(-1.0 / (fmax(dist_P.Release_rt, 1.0E-6) * 48000.0));
  e = dist_DW.obj_m.gPrev_dB;
  for (t = 0; t < 128; t++) {
    /* MATLABSystem: '<Root>/ENV' */
    f_0 = rtb_ENV_o1_0[t];
    if (f_0 < e) {
      /*  Entrando en compresión (ganancia baja): ataque */
      e = (1.0 - alpha) * f_0 + alpha * e;
    } else {
      /*  Saliendo de compresión (ganancia sube): release */
      e = (1.0 - alphaR) * f_0 + alphaR * e;
    }

    rtb_ARF_0[t] = e;
  }

  dist_DW.obj_m.gPrev_dB = e;

  /* MATLABSystem: '<Root>/VCA1 (Pre-Gain)' incorporates:
   *  Constant: '<Root>/Constant4'
   *  MATLABSystem: '<Root>/ARF'
   */
  /*  g es Nx1 */
  if (dist_P.Constant4_Value != 0) {
    for (b_k = 0; b_k < 128; b_k++) {
      rtb_ARF_0[b_k] = rt_powd_snf(10.0, rtb_ARF_0[b_k] / 20.0);
    }

    /*  dB -> lineal, elemento a elemento */
  }

  /* MATLABSystem: '<Root>/LUT-WS' incorporates:
   *  Constant: '<Root>/Constant5'
   *  Constant: '<Root>/Constant6'
   *  Constant: '<Root>/Constant9'
   */
  /*  bp es logical */
  /*  elemento a elemento */
  if (dist_DW.obj.ModeDefault != dist_P.LUTWS_ModeDefault) {
    dist_DW.obj.ModeDefault = dist_P.LUTWS_ModeDefault;
  }

  if (dist_DW.obj.AmountDefault != dist_P.LUTWS_AmountDefault) {
    dist_DW.obj.AmountDefault = dist_P.LUTWS_AmountDefault;
  }

  if (dist_DW.obj.AsymDefault != dist_P.LUTWS_AsymDefault) {
    dist_DW.obj.AsymDefault = dist_P.LUTWS_AsymDefault;
  }

  if (dist_DW.obj.Cheb3Default != dist_P.LUTWS_Cheb3Default) {
    dist_DW.obj.Cheb3Default = dist_P.LUTWS_Cheb3Default;
  }

  if (dist_DW.obj.Cheb5Default != dist_P.LUTWS_Cheb5Default) {
    dist_DW.obj.Cheb5Default = dist_P.LUTWS_Cheb5Default;
  }

  if (!dist_isequal(dist_DW.obj.XUser, dist_P.LUTWS_XUser)) {
    memcpy(&dist_DW.obj.XUser[0], &dist_P.LUTWS_XUser[0], 257U * sizeof(real_T));
  }

  if (!dist_isequal(dist_DW.obj.YUser, dist_P.LUTWS_YUser)) {
    memcpy(&dist_DW.obj.YUser[0], &dist_P.LUTWS_YUser[0], 257U * sizeof(real_T));
  }

  if (dist_DW.obj.MUser != dist_P.LUTWS_MUser) {
    dist_DW.obj.MUser = dist_P.LUTWS_MUser;
  }

  if (dist_DW.obj.Eps != dist_P.LUTWS_Eps) {
    dist_DW.obj.Eps = dist_P.LUTWS_Eps;
  }

  /*  inL,inR,mode,amount,asym,cheb3,cheb5 */
  /*  Nloc = size(inL,1); */
  /*  outL = zeros(Nloc,1); */
  /*  outR = zeros(Nloc,1); */
  /*  bp es logical */
  /*  --- defaults --- */
  t = (int32_T)fmin(fmax(dist_P.Shaper_rt, 1.0), 6.0);

  /*  por ejemplo */
  if (dist_P.Asym_rt < -0.5) {
    e = -0.5;
  } else if (dist_P.Asym_rt > 0.5) {
    e = 0.5;
  } else {
    e = dist_P.Asym_rt;
  }

  /*  drive interno (lineal) */
  if (dist_P.Amount_rt < 0.0) {
    alpha = 0.0;
  } else if (dist_P.Amount_rt > 60.0) {
    alpha = 60.0;
  } else {
    alpha = dist_P.Amount_rt;
  }

  alpha = rt_powd_snf(10.0, alpha / 20.0);

  /*  preparar LUT usuario (si aplica) */
  /*  (tamaño fijo M, relleno seguro) */
  memset(&xGrid[0], 0, 257U * sizeof(real_T));
  memset(&yGrid[0], 0, 257U * sizeof(real_T));
  if (t == 6) {
    i = (int32_T)rt_roundd_snf(fmin(fmax(dist_DW.obj.MUser, 1.0), 257.0));
    memcpy(&xGrid[0], &dist_DW.obj.XUser[0], (uint32_T)((i - 1) + 1) * sizeof
           (real_T));
    memcpy(&yGrid[0], &dist_DW.obj.YUser[0], (uint32_T)((i - 1) + 1) * sizeof
           (real_T));
    b_k = i + 1;
    for (Mu = b_k; Mu < 258; Mu++) {
      xGrid[Mu - 1] = xGrid[i - 1];
      yGrid[Mu - 1] = yGrid[i - 1];
    }
  }

  /* Start for MATLABSystem: '<Root>/LUT-WS' */
  memcpy(&c[0], &dist_DW.obj.zUp1L[0], 14U * sizeof(real_T));
  memcpy(&d[0], &dist_DW.obj.zUp2L[0], 14U * sizeof(real_T));
  memcpy(&e_0[0], &dist_DW.obj.zDn1L[0], 14U * sizeof(real_T));
  memcpy(&f[0], &dist_DW.obj.zDn2L[0], 14U * sizeof(real_T));
  for (i = 0; i < 128; i++) {
    /* Start for MATLABSystem: '<Root>/VCA1 (Pre-Gain)' */
    rtb_Gain1_0[i] = rtb_Gain1[i] * rtb_ARF_0[i];
  }

  /* Start for MATLABSystem: '<Root>/LUT-WS' incorporates:
   *  Constant: '<Root>/Constant10'
   *  Constant: '<Root>/Constant11'
   */
  dis_lut_ws_sys_processChannelOS(&dist_DW.obj, rtb_Gain1_0, 1, (uint8_T)t,
    alpha, e, dist_P.Constant10_Value, dist_P.Constant11_Value, xGrid, yGrid, c,
    d, e_0, f, rtb_ENV_o1_0);
  for (i = 0; i < 14; i++) {
    /* MATLABSystem: '<Root>/LUT-WS' */
    dist_DW.obj.zUp1L[i] = c[i];
    dist_DW.obj.zUp2L[i] = d[i];
    dist_DW.obj.zDn1L[i] = e_0[i];
    dist_DW.obj.zDn2L[i] = f[i];

    /* Start for MATLABSystem: '<Root>/LUT-WS' */
    c[i] = dist_DW.obj.zUp1R[i];
    d[i] = dist_DW.obj.zUp2R[i];
    e_0[i] = dist_DW.obj.zDn1R[i];
    f[i] = dist_DW.obj.zDn2R[i];
  }

  for (i = 0; i < 128; i++) {
    /* Start for MATLABSystem: '<Root>/VCA1 (Pre-Gain)' */
    rtb_Gain_0[i] = rtb_Gain[i] * rtb_ARF_0[i];
  }

  /* Start for MATLABSystem: '<Root>/LUT-WS' incorporates:
   *  Constant: '<Root>/Constant10'
   *  Constant: '<Root>/Constant11'
   */
  dis_lut_ws_sys_processChannelOS(&dist_DW.obj, rtb_Gain_0, 1, (uint8_T)t, alpha,
    e, dist_P.Constant10_Value, dist_P.Constant11_Value, xGrid, yGrid, c, d, e_0,
    f, rtb_Gain1_0);

  /* MATLABSystem: '<Root>/LUT-WS' */
  memcpy(&dist_DW.obj.zUp1R[0], &c[0], 14U * sizeof(real_T));
  memcpy(&dist_DW.obj.zUp2R[0], &d[0], 14U * sizeof(real_T));
  memcpy(&dist_DW.obj.zDn1R[0], &e_0[0], 14U * sizeof(real_T));
  memcpy(&dist_DW.obj.zDn2R[0], &f[0], 14U * sizeof(real_T));

  /* MATLABSystem: '<Root>/VCA2 (Post-Gain)' incorporates:
   *  Constant: '<Root>/Constant1'
   *  Constant: '<Root>/Constant8'
   */
  e = dist_P.Level_rt;

  /*  g es Nx1 */
  if (dist_P.Constant1_Value != 0) {
    e = rt_powd_snf(10.0, dist_P.Level_rt / 20.0);

    /*  dB -> lineal, elemento a elemento */
  }

  /* MATLABSystem: '<Root>/VCF' incorporates:
   *  Constant: '<Root>/Constant12'
   *  Constant: '<Root>/Constant13'
   *  Constant: '<Root>/Constant14'
   *  Constant: '<Root>/Constant7'
   */
  /*  bp es logical */
  /*  elemento a elemento */
  /*  acepta double y castea */
  if ((dist_P.Constant13_Value != dist_DW.obj_d.lastType) || (dist_P.Tone_rt !=
       dist_DW.obj_d.lastFreq) || (dist_P.Constant12_Value !=
       dist_DW.obj_d.lastQ) || (dist_P.Constant14_Value !=
       dist_DW.obj_d.lastGain)) {
    f_0 = dist_P.Tone_rt;
    alphaR = dist_P.Constant12_Value;
    alpha = dist_P.Constant14_Value;
    t = dist_P.Constant13_Value;
    if (rtIsInf(dist_P.Tone_rt) || rtIsNaN(dist_P.Tone_rt)) {
      f_0 = 1000.0;
    }

    if (rtIsInf(dist_P.Constant12_Value) || rtIsNaN(dist_P.Constant12_Value)) {
      alphaR = 0.707;
    }

    if (rtIsInf(dist_P.Constant14_Value) || rtIsNaN(dist_P.Constant14_Value)) {
      alpha = 0.0;
    }

    /*  límites prácticos */
    if (f_0 < 0.1) {
      f_0 = 0.1;
    }

    if (f_0 > 23952.0) {
      f_0 = 23952.0;
    }

    if (alphaR < 0.001) {
      alphaR = 0.001;
    }

    wc = f_0 / 48000.0 * 6.2831853071795862;
    if (dist_P.Constant13_Value < 1) {
      t = 1;
    } else if (dist_P.Constant13_Value > 7) {
      t = 7;
    }

    dist_DW.obj_d.b[0] = 1.0;
    dist_DW.obj_d.a[0] = 1.0;
    dist_DW.obj_d.b[1] = 0.0;
    dist_DW.obj_d.a[1] = 0.0;
    dist_DW.obj_d.b[2] = 0.0;
    dist_DW.obj_d.a[2] = 0.0;
    switch ((uint8_T)t) {
     case 1U:
      /*  lowpass */
      /* Cálculo de los coefifientes de un Filtro Paso-Bajo de 2º orden */
      /* Parámetos de entrada:  */
      /* wo = frecuencia de corte a -3dB (en radianes) */
      /* Parametros de salida: */
      /* Coeficientes [b,a] de un filtro IIR de segundo orden */
      p = cos(wc);
      f_0 = 1.4142135623730951 * sin(wc);

      /* Normalización de coeficientes */
      /* fvtool(b,a); */
      alpha = (1.0 - p) / (f_0 + 2.0);
      dist_DW.obj_d.b[0] = alpha;
      dist_DW.obj_d.b[1] = (1.0 - p) * 2.0 / (f_0 + 2.0);
      dist_DW.obj_d.b[2] = alpha;
      dist_DW.obj_d.a[0] = (f_0 + 2.0) / (f_0 + 2.0);
      dist_DW.obj_d.a[1] = -4.0 * p / (f_0 + 2.0);
      dist_DW.obj_d.a[2] = (2.0 - f_0) / (f_0 + 2.0);
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
      f_0 = 1.4142135623730951 * sin(wc);

      /* Normalización de coeficientes */
      /*  */
      /* fvtool(b,a); */
      alpha = (p + 1.0) / (f_0 + 2.0);
      dist_DW.obj_d.b[0] = alpha;
      dist_DW.obj_d.b[1] = (p + 1.0) * -2.0 / (f_0 + 2.0);
      dist_DW.obj_d.b[2] = alpha;
      dist_DW.obj_d.a[0] = (f_0 + 2.0) / (f_0 + 2.0);
      dist_DW.obj_d.a[1] = -4.0 * p / (f_0 + 2.0);
      dist_DW.obj_d.a[2] = (2.0 - f_0) / (f_0 + 2.0);
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
      f_0 = sin(wc) / alphaR;

      /*  <-- Q aquí */
      /*  Coeficientes del ecualizador */
      /* Normalización de coeficientes */
      /* fvtool(b,a); */
      dist_DW.obj_d.b[0] = f_0 / (f_0 + 2.0);
      dist_DW.obj_d.b[1] = 0.0 / (f_0 + 2.0);
      dist_DW.obj_d.b[2] = -f_0 / (f_0 + 2.0);
      dist_DW.obj_d.a[0] = (f_0 + 2.0) / (f_0 + 2.0);
      dist_DW.obj_d.a[1] = -4.0 * cos(wc) / (f_0 + 2.0);
      dist_DW.obj_d.a[2] = (2.0 - f_0) / (f_0 + 2.0);
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
      f_0 = sin(wc) / alphaR;

      /* Normalización de coeficientes */
      /* fvtool(b,a); */
      alpha = 2.0 / (f_0 + 2.0);
      dist_DW.obj_d.b[0] = alpha;
      wc = -4.0 * cos(wc) / (f_0 + 2.0);
      dist_DW.obj_d.b[1] = wc;
      dist_DW.obj_d.b[2] = alpha;
      dist_DW.obj_d.a[0] = (f_0 + 2.0) / (f_0 + 2.0);
      dist_DW.obj_d.a[1] = wc;
      dist_DW.obj_d.a[2] = (2.0 - f_0) / (f_0 + 2.0);
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
      A = rt_powd_snf(10.0, alpha / 40.0);
      sqrtA = sqrt(A);
      p = cos(wc);
      q = sin(wc);

      /*  Coeficientes del ecualizador */
      f_0 = 1.0 / A;
      a0_tmp = (1.0 - f_0) * p + (f_0 + 1.0);
      a0_tmp_0 = 1.4142135623730951 * q / sqrtA;
      a0 = a0_tmp + a0_tmp_0;

      /* Normalización de coeficientes */
      /* fvtool(b,a); */
      alpha = (1.0 - A) * p + (A + 1.0);
      wc = 1.4142135623730951 * sqrtA * q;
      dist_DW.obj_d.b[0] = (alpha + wc) / a0;
      dist_DW.obj_d.b[1] = ((A + 1.0) * p + (1.0 - A)) * -2.0 / a0;
      dist_DW.obj_d.b[2] = (alpha - wc) / a0;
      dist_DW.obj_d.a[0] = a0 / a0;
      dist_DW.obj_d.a[1] = ((f_0 + 1.0) * p + (1.0 - f_0)) * -2.0 / a0;
      dist_DW.obj_d.a[2] = (a0_tmp - a0_tmp_0) / a0;
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
      A = rt_powd_snf(10.0, alpha / 40.0);
      sqrtA = sqrt(A);
      p = cos(wc);
      q = sin(wc);

      /*  Coeficientes del ecualizador */
      f_0 = (1.0 - A) * p + (A + 1.0);
      a0_tmp = 1.4142135623730951 * sqrtA * q;
      a0 = f_0 + a0_tmp;
      a0_tmp_0 = A * A;

      /* Normalización de coeficientes */
      /* fvtool(b,a); */
      alpha = 1.0 / A;
      wc = (1.0 - alpha) * p + (alpha + 1.0);
      sqrtA = 1.4142135623730951 * q / sqrtA;
      dist_DW.obj_d.b[0] = (wc + sqrtA) * a0_tmp_0 / a0;
      dist_DW.obj_d.b[1] = ((alpha + 1.0) * p + (1.0 - alpha)) * -2.0 * a0_tmp_0
        / a0;
      dist_DW.obj_d.b[2] = (wc - sqrtA) * a0_tmp_0 / a0;
      dist_DW.obj_d.a[0] = a0 / a0;
      dist_DW.obj_d.a[1] = ((A + 1.0) * p + (1.0 - A)) * -2.0 / a0;
      dist_DW.obj_d.a[2] = (f_0 - a0_tmp) / a0;
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
      A = rt_powd_snf(10.0, alpha / 40.0);
      alpha = sin(wc) / (2.0 * alphaR);

      /*  Coeficientes del ecualizador */
      f_0 = alpha / A;

      /* Normalización de coeficientes */
      /* fvtool(b,a); */
      alpha *= A;
      dist_DW.obj_d.b[0] = (alpha + 1.0) / (f_0 + 1.0);
      wc = -2.0 * cos(wc) / (f_0 + 1.0);
      dist_DW.obj_d.b[1] = wc;
      dist_DW.obj_d.b[2] = (1.0 - alpha) / (f_0 + 1.0);
      dist_DW.obj_d.a[0] = (f_0 + 1.0) / (f_0 + 1.0);
      dist_DW.obj_d.a[1] = wc;
      dist_DW.obj_d.a[2] = (1.0 - f_0) / (f_0 + 1.0);
      break;
    }

    dist_DW.obj_d.lastType = dist_P.Constant13_Value;
    dist_DW.obj_d.lastFreq = dist_P.Tone_rt;
    dist_DW.obj_d.lastQ = alphaR;
    dist_DW.obj_d.lastGain = dist_P.Constant14_Value;
  }

  /*  bp es logical */
  for (t = 0; t < 128; t++) {
    /* Start for MATLABSystem: '<Root>/VCA2 (Post-Gain)' incorporates:
     *  MATLABSystem: '<Root>/LUT-WS'
     */
    rtb_Gain_0[t] = rtb_ENV_o1_0[t] * e;
  }

  /* MATLABSystem: '<Root>/VCF' */
  for (i = 0; i < 2; i++) {
    tmp[i] = dist_DW.obj_d.zL[i];
  }

  for (i = 0; i < 3; i++) {
    tmp_0[i] = dist_DW.obj_d.b[i];
  }

  for (i = 0; i < 3; i++) {
    tmp_1[i] = dist_DW.obj_d.a[i];
  }

  dist_filter_kn(tmp_0, tmp_1, rtb_Gain_0, tmp, rtb_ARF_0, dist_DW.obj_d.zL);
  for (t = 0; t < 128; t++) {
    /* Start for MATLABSystem: '<Root>/VCA2 (Post-Gain)' incorporates:
     *  MATLABSystem: '<Root>/LUT-WS'
     */
    rtb_Gain_0[t] = rtb_Gain1_0[t] * e;
  }

  /* MATLABSystem: '<Root>/VCF' */
  for (i = 0; i < 2; i++) {
    tmp[i] = dist_DW.obj_d.zR[i];
  }

  for (i = 0; i < 3; i++) {
    tmp_0[i] = dist_DW.obj_d.b[i];
  }

  for (i = 0; i < 3; i++) {
    tmp_1[i] = dist_DW.obj_d.a[i];
  }

  dist_filter_kn(tmp_0, tmp_1, rtb_Gain_0, tmp, rtb_ENV_o1_0, dist_DW.obj_d.zR);

  /* MATLABSystem: '<Root>/MIX' incorporates:
   *  Constant: '<Root>/Constant15'
   */
  if (dist_DW.obj_e.SmoothTime != dist_P.MIX_SmoothTime) {
    dist_DW.obj_e.SmoothTime = dist_P.MIX_SmoothTime;
  }

  if (dist_DW.obj_e.IndepInDB != dist_P.MIX_IndepInDB) {
    dist_DW.obj_e.IndepInDB = dist_P.MIX_IndepInDB;
  }

  /*  inL,inR,wetL,wetR,p1 (+p2 si Mode=2) */
  /*  Mode 3: crossfade por muestra (p1 puede ser [N x 1]) */
  /*  dummy */
  e = fmin(fmax(dist_P.Mix_rt, 0.0), 1.0);
  alphaR = dist_DW.obj_e.a;
  f_0 = dist_DW.obj_e.gd1;
  A = dist_DW.obj_e.gd2;
  p = dist_DW.obj_e.gw1;
  wc = dist_DW.obj_e.gw2;

  /*  más robusto que obj.N */
  for (t = 0; t < 128; t++) {
    f_0 += ((1.0 - e) - f_0) * alphaR;
    p += (e - p) * alphaR;
    A += ((1.0 - e) - A) * alphaR;
    wc += (e - wc) * alphaR;

    /* DataTypeConversion: '<Root>/Data Type Conversion1' incorporates:
     *  Gain: '<Root>/Gain2'
     *  MATLABSystem: '<Root>/VCF'
     */
    alpha = floor((f_0 * rtb_Gain[t] + p * rtb_ARF_0[t]) * dist_P.Gain2_Gain);
    if (rtIsNaN(alpha) || rtIsInf(alpha)) {
      alpha = 0.0;
    } else {
      alpha = fmod(alpha, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion1' */
    dist_B.DataTypeConversion1[t] = alpha < 0.0 ? -(int32_T)(uint32_T)-alpha :
      (int32_T)(uint32_T)alpha;

    /* DataTypeConversion: '<Root>/Data Type Conversion3' incorporates:
     *  Gain: '<Root>/Gain3'
     *  MATLABSystem: '<Root>/VCF'
     */
    alpha = floor((A * rtb_Gain1[t] + wc * rtb_ENV_o1_0[t]) * dist_P.Gain3_Gain);
    if (rtIsNaN(alpha) || rtIsInf(alpha)) {
      alpha = 0.0;
    } else {
      alpha = fmod(alpha, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion3' */
    dist_B.DataTypeConversion3[t] = alpha < 0.0 ? -(int32_T)(uint32_T)-alpha :
      (int32_T)(uint32_T)alpha;
  }

  dist_DW.obj_e.gd1 = f_0;
  dist_DW.obj_e.gd2 = A;
  dist_DW.obj_e.gw1 = p;
  dist_DW.obj_e.gw2 = wc;

  /* End of MATLABSystem: '<Root>/MIX' */

  /* S-Function (pisound_out): '<Root>/PiSound Output' */

  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = dist_M->childSfunctions[1];
    sfcnOutputs(rts,0);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  dist_M->Timing.t[0] =
    ((time_T)(++dist_M->Timing.clockTick0)) * dist_M->Timing.stepSize0;
}

/* Model initialize function */
void dist_initialize(void)
{
  /* Registration code */
  rtsiSetSolverName(&dist_M->solverInfo,"FixedStepDiscrete");
  dist_M->solverInfoPtr = (&dist_M->solverInfo);

  /* Initialize timing info */
  {
    int_T *mdlTsMap = dist_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    dist_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    dist_M->Timing.sampleTimes = (&dist_M->Timing.sampleTimesArray[0]);
    dist_M->Timing.offsetTimes = (&dist_M->Timing.offsetTimesArray[0]);

    /* task periods */
    dist_M->Timing.sampleTimes[0] = (0.0026666666666666666);

    /* task offsets */
    dist_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(dist_M, &dist_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = dist_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    dist_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(dist_M, -1);
  dist_M->Timing.stepSize0 = 0.0026666666666666666;
  dist_M->solverInfoPtr = (&dist_M->solverInfo);
  dist_M->Timing.stepSize = (0.0026666666666666666);
  rtsiSetFixedStepSize(&dist_M->solverInfo, 0.0026666666666666666);
  rtsiSetSolverMode(&dist_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* child S-Function registration */
  {
    RTWSfcnInfo *sfcnInfo = &dist_M->NonInlinedSFcns.sfcnInfo;
    dist_M->sfcnInfo = (sfcnInfo);
    rtssSetErrorStatusPtr(sfcnInfo, (&rtmGetErrorStatus(dist_M)));
    dist_M->Sizes.numSampTimes = (1);
    rtssSetNumRootSampTimesPtr(sfcnInfo, &dist_M->Sizes.numSampTimes);
    dist_M->NonInlinedSFcns.taskTimePtrs[0] = (&rtmGetTPtr(dist_M)[0]);
    rtssSetTPtrPtr(sfcnInfo,dist_M->NonInlinedSFcns.taskTimePtrs);
    rtssSetTStartPtr(sfcnInfo, &rtmGetTStart(dist_M));
    rtssSetTFinalPtr(sfcnInfo, &rtmGetTFinal(dist_M));
    rtssSetTimeOfLastOutputPtr(sfcnInfo, &rtmGetTimeOfLastOutput(dist_M));
    rtssSetStepSizePtr(sfcnInfo, &dist_M->Timing.stepSize);
    rtssSetStopRequestedPtr(sfcnInfo, &rtmGetStopRequested(dist_M));
    rtssSetDerivCacheNeedsResetPtr(sfcnInfo, &dist_M->derivCacheNeedsReset);
    rtssSetZCCacheNeedsResetPtr(sfcnInfo, &dist_M->zCCacheNeedsReset);
    rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(sfcnInfo,
      &dist_M->CTOutputIncnstWithState);
    rtssSetSampleHitsPtr(sfcnInfo, &dist_M->Timing.sampleHits);
    rtssSetPerTaskSampleHitsPtr(sfcnInfo, &dist_M->Timing.perTaskSampleHits);
    rtssSetSimModePtr(sfcnInfo, &dist_M->simMode);
    rtssSetSolverInfoPtr(sfcnInfo, &dist_M->solverInfoPtr);
  }

  dist_M->Sizes.numSFcns = (2);

  /* register each child */
  {
    (void) memset((void *)&dist_M->NonInlinedSFcns.childSFunctions[0], 0,
                  2*sizeof(SimStruct));
    dist_M->childSfunctions = (&dist_M->NonInlinedSFcns.childSFunctionPtrs[0]);
    dist_M->childSfunctions[0] = (&dist_M->NonInlinedSFcns.childSFunctions[0]);
    dist_M->childSfunctions[1] = (&dist_M->NonInlinedSFcns.childSFunctions[1]);

    /* Level2 S-Function Block: dist/<Root>/PiSound Input (pisound_in) */
    {
      SimStruct *rts = dist_M->childSfunctions[0];

      /* timing info */
      time_T *sfcnPeriod = dist_M->NonInlinedSFcns.Sfcn0.sfcnPeriod;
      time_T *sfcnOffset = dist_M->NonInlinedSFcns.Sfcn0.sfcnOffset;
      int_T *sfcnTsMap = dist_M->NonInlinedSFcns.Sfcn0.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &dist_M->NonInlinedSFcns.blkInfo2[0]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &dist_M->NonInlinedSFcns.inputOutputPortInfo2[0]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, dist_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &dist_M->NonInlinedSFcns.methods2[0]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &dist_M->NonInlinedSFcns.methods3[0]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &dist_M->NonInlinedSFcns.methods4[0]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &dist_M->NonInlinedSFcns.statesInfo2[0]);
        ssSetPeriodicStatesInfo(rts, &dist_M->
          NonInlinedSFcns.periodicStatesInfo[0]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &dist_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &dist_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &dist_M->NonInlinedSFcns.Sfcn0.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &dist_M->NonInlinedSFcns.Sfcn0.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 128);
          ssSetOutputPortSignal(rts, 0, ((int32_T *) dist_B.PiSoundInput_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 128);
          ssSetOutputPortSignal(rts, 1, ((int32_T *) dist_B.PiSoundInput_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "PiSound Input");
      ssSetPath(rts, "dist/PiSound Input");
      ssSetRTModel(rts,dist_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &dist_M->NonInlinedSFcns.Sfcn0.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)dist_P.PiSoundInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)dist_P.PiSoundInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)dist_P.PiSoundInput_P3_Size);
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

    /* Level2 S-Function Block: dist/<Root>/PiSound Output (pisound_out) */
    {
      SimStruct *rts = dist_M->childSfunctions[1];

      /* timing info */
      time_T *sfcnPeriod = dist_M->NonInlinedSFcns.Sfcn1.sfcnPeriod;
      time_T *sfcnOffset = dist_M->NonInlinedSFcns.Sfcn1.sfcnOffset;
      int_T *sfcnTsMap = dist_M->NonInlinedSFcns.Sfcn1.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &dist_M->NonInlinedSFcns.blkInfo2[1]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &dist_M->NonInlinedSFcns.inputOutputPortInfo2[1]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, dist_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &dist_M->NonInlinedSFcns.methods2[1]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &dist_M->NonInlinedSFcns.methods3[1]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &dist_M->NonInlinedSFcns.methods4[1]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &dist_M->NonInlinedSFcns.statesInfo2[1]);
        ssSetPeriodicStatesInfo(rts, &dist_M->
          NonInlinedSFcns.periodicStatesInfo[1]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts, &dist_M->
          NonInlinedSFcns.Sfcn1.inputPortInfo[0]);
        ssSetPortInfoForInputs(rts, &dist_M->
          NonInlinedSFcns.Sfcn1.inputPortInfo[0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &dist_M->NonInlinedSFcns.Sfcn1.inputPortUnits[0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &dist_M->NonInlinedSFcns.Sfcn1.inputPortCoSimAttribute[0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          int32_T const **sfcnUPtrs = (int32_T const **)
            &dist_M->NonInlinedSFcns.Sfcn1.UPtrs0;

          {
            int_T i1;
            const int32_T *u0 = dist_B.DataTypeConversion1;
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
            &dist_M->NonInlinedSFcns.Sfcn1.UPtrs1;

          {
            int_T i1;
            const int32_T *u1 = dist_B.DataTypeConversion3;
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
      ssSetPath(rts, "dist/PiSound Output");
      ssSetRTModel(rts,dist_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &dist_M->NonInlinedSFcns.Sfcn1.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)dist_P.PiSoundOutput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)dist_P.PiSoundOutput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)dist_P.PiSoundOutput_P3_Size);
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

  /* Start for MATLABSystem: '<Root>/ENV' */
  dist_DW.obj_n.ModeDefault = dist_P.ENV_ModeDefault;
  dist_DW.obj_n.Eps = dist_P.ENV_Eps;
  dist_DW.obj_n.TauRMS = dist_P.ENV_TauRMS;
  dist_DW.obj_n.isInitialized = 1;

  /* InitializeConditions for MATLABSystem: '<Root>/ENV' */
  dist_DW.obj_n.e_prev = 0.0;

  /* Start for MATLABSystem: '<Root>/ARF' */
  dist_DW.obj_m.AttackDefault = dist_P.ARF_AttackDefault;
  dist_DW.obj_m.ReleaseDefault = dist_P.ARF_ReleaseDefault;
  dist_DW.obj_m.isInitialized = 1;

  /* InitializeConditions for MATLABSystem: '<Root>/ARF' */
  /*  ganancia unitaria inicial (0 dB) */
  dist_DW.obj_m.gPrev_dB = 0.0;

  /* Start for MATLABSystem: '<Root>/LUT-WS' */
  dist_DW.obj.ModeDefault = dist_P.LUTWS_ModeDefault;
  dist_DW.obj.AmountDefault = dist_P.LUTWS_AmountDefault;
  dist_DW.obj.AsymDefault = dist_P.LUTWS_AsymDefault;
  dist_DW.obj.Cheb3Default = dist_P.LUTWS_Cheb3Default;
  dist_DW.obj.Cheb5Default = dist_P.LUTWS_Cheb5Default;
  memcpy(&dist_DW.obj.XUser[0], &dist_P.LUTWS_XUser[0], 257U * sizeof(real_T));
  memcpy(&dist_DW.obj.YUser[0], &dist_P.LUTWS_YUser[0], 257U * sizeof(real_T));
  dist_DW.obj.MUser = dist_P.LUTWS_MUser;
  dist_DW.obj.Eps = dist_P.LUTWS_Eps;
  dist_DW.obj.isInitialized = 1;

  /* InitializeConditions for MATLABSystem: '<Root>/LUT-WS' */
  /*  Validaciones */
  memset(&dist_DW.obj.zUp1L[0], 0, 14U * sizeof(real_T));
  memset(&dist_DW.obj.zUp1R[0], 0, 14U * sizeof(real_T));
  memset(&dist_DW.obj.zDn1L[0], 0, 14U * sizeof(real_T));
  memset(&dist_DW.obj.zDn1R[0], 0, 14U * sizeof(real_T));
  memset(&dist_DW.obj.zUp2L[0], 0, 14U * sizeof(real_T));
  memset(&dist_DW.obj.zUp2R[0], 0, 14U * sizeof(real_T));
  memset(&dist_DW.obj.zDn2L[0], 0, 14U * sizeof(real_T));
  memset(&dist_DW.obj.zDn2R[0], 0, 14U * sizeof(real_T));

  /* Start for MATLABSystem: '<Root>/VCF' */
  dist_DW.obj_d.isInitialized = 1;

  /*  arranca en bypass */
  dist_DW.obj_d.b[0] = 1.0;
  dist_DW.obj_d.a[0] = 1.0;
  dist_DW.obj_d.b[1] = 0.0;
  dist_DW.obj_d.a[1] = 0.0;
  dist_DW.obj_d.b[2] = 0.0;
  dist_DW.obj_d.a[2] = 0.0;

  /*  fuerza recálculo en el primer frame */
  dist_DW.obj_d.lastType = MAX_uint8_T;
  dist_DW.obj_d.lastFreq = (rtNaN);
  dist_DW.obj_d.lastQ = (rtNaN);
  dist_DW.obj_d.lastGain = (rtNaN);

  /* InitializeConditions for MATLABSystem: '<Root>/VCF' */
  dist_DW.obj_d.zL[0] = 0.0;
  dist_DW.obj_d.zR[0] = 0.0;
  dist_DW.obj_d.zL[1] = 0.0;
  dist_DW.obj_d.zR[1] = 0.0;

  /* Start for MATLABSystem: '<Root>/MIX' */
  dist_DW.obj_e.SmoothTime = dist_P.MIX_SmoothTime;
  dist_DW.obj_e.IndepInDB = dist_P.MIX_IndepInDB;
  dist_DW.obj_e.isInitialized = 1;

  /*  inL,inR,wetL,wetR,p1 (+p2 si Mode=2) */
  if (dist_DW.obj_e.SmoothTime <= 0.0) {
    dist_DW.obj_e.a = 1.0;
  } else {
    dist_DW.obj_e.a = 1.0 - exp(-1.0 / (48000.0 * dist_DW.obj_e.SmoothTime));
  }

  /* InitializeConditions for MATLABSystem: '<Root>/MIX' */
  dist_DW.obj_e.gd1 = 1.0;
  dist_DW.obj_e.gd2 = 1.0;
  dist_DW.obj_e.gw1 = 0.0;
  dist_DW.obj_e.gw2 = 0.0;
}

/* Model terminate function */
void dist_terminate(void)
{
  /* Terminate for S-Function (pisound_in): '<Root>/PiSound Input' */
  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = dist_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (pisound_out): '<Root>/PiSound Output' */
  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = dist_M->childSfunctions[1];
    sfcnTerminate(rts);
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
