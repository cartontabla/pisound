/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: filters.c
 *
 * Code generated for Simulink model 'filters'.
 *
 * Model version                  : 1.21
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Wed Mar  4 16:01:03 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "filters.h"
#include "filters_types.h"
#include "rtwtypes.h"
#include <math.h>
#include "rt_nonfinite.h"
#include "filters_private.h"
#include <string.h>

/* Block signals (default storage) */
B_filters_T filters_B;

/* Block states (default storage) */
DW_filters_T filters_DW;

/* Real-time model */
static RT_MODEL_filters_T filters_M_;
RT_MODEL_filters_T *const filters_M = &filters_M_;

/* Forward declaration for local functions */
static void filter_filters_sys_updateCoeffs(filters_sys_filters_T *obj);
static void filters_filter(real_T b[3], real_T a[3], const real_T x[128], const
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

static void filter_filters_sys_updateCoeffs(filters_sys_filters_T *obj)
{
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

  /*  Sanitiza parámetros (evita NaN/inf y zonas problemáticas) */
  f = obj->Freq;
  Q = obj->Q;
  G = obj->Gain;
  t = obj->FilterType;

  /* Start for MATLABSystem: '<Root>/MATLAB System' */
  if (rtIsInf(obj->Freq) || rtIsNaN(obj->Freq)) {
    f = 1000.0;
  }

  if (rtIsInf(obj->Q) || rtIsNaN(obj->Q)) {
    Q = 0.707;
  }

  if (rtIsInf(obj->Gain) || rtIsNaN(obj->Gain)) {
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

  /* Start for MATLABSystem: '<Root>/MATLAB System' */
  /*  Pre-cálculos */
  wc = f / 48000.0 * 6.2831853071795862;

  /*  Coefs (sin normalizar) */
  obj->b[0] = 1.0;
  obj->a[0] = 1.0;
  obj->b[1] = 0.0;
  obj->a[1] = 0.0;
  obj->b[2] = 0.0;
  obj->a[2] = 0.0;
  if (obj->FilterType < 1) {
    t = 1;
  } else if (obj->FilterType > 7) {
    t = 7;
  }

  switch ((uint8_T)t) {
   case 1U:
    /* Start for MATLABSystem: '<Root>/MATLAB System' */
    /*  lowpass */
    /* Cálculo de los coefifientes de un Filtro Paso-Bajo de 2º orden */
    /* Parámetos de entrada:  */
    /* wo = frecuencia de corte a -3dB (en radianes) */
    /* Parametros de salida: */
    /* Coeficientes [b,a] de un filtro IIR de segundo orden */
    p = cos(wc);
    Q = 1.4142135623730951 * sin(wc);

    /* Normalización de coeficientes */
    /* fvtool(b,a); */
    f = (1.0 - p) / (Q + 2.0);
    obj->b[0] = f;
    obj->b[1] = (1.0 - p) * 2.0 / (Q + 2.0);
    obj->b[2] = f;

    /* Start for MATLABSystem: '<Root>/MATLAB System' */
    obj->a[0] = (Q + 2.0) / (Q + 2.0);
    obj->a[1] = -4.0 * p / (Q + 2.0);
    obj->a[2] = (2.0 - Q) / (Q + 2.0);
    break;

   case 2U:
    /* Start for MATLABSystem: '<Root>/MATLAB System' */
    /*  highpass */
    /* Cálculo de los coefifientes de un Filtro Paso-Alto de 2º orden */
    /* Parámetos de entrada:  */
    /* wo = frecuencia de corte a -3dB (en radianes) */
    /* Parametros de salida: */
    /* Coeficientes [b,a] de un filtro IIR de segundo orden */
    p = cos(wc);

    /*  Coeficientes del ecualizador */
    Q = 1.4142135623730951 * sin(wc);

    /* Normalización de coeficientes */
    /*  */
    /* fvtool(b,a); */
    f = (p + 1.0) / (Q + 2.0);
    obj->b[0] = f;
    obj->b[1] = (p + 1.0) * -2.0 / (Q + 2.0);
    obj->b[2] = f;

    /* Start for MATLABSystem: '<Root>/MATLAB System' */
    obj->a[0] = (Q + 2.0) / (Q + 2.0);
    obj->a[1] = -4.0 * p / (Q + 2.0);
    obj->a[2] = (2.0 - Q) / (Q + 2.0);
    break;

   case 3U:
    /* Start for MATLABSystem: '<Root>/MATLAB System' */
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
    obj->b[0] = G / (G + 2.0);
    obj->b[1] = 0.0 / (G + 2.0);
    obj->b[2] = -G / (G + 2.0);
    obj->a[0] = (G + 2.0) / (G + 2.0);
    obj->a[1] = -4.0 * cos(wc) / (G + 2.0);
    obj->a[2] = (2.0 - G) / (G + 2.0);
    break;

   case 4U:
    /* Start for MATLABSystem: '<Root>/MATLAB System' */
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
    obj->b[0] = f;
    wc = -4.0 * cos(wc) / (G + 2.0);
    obj->b[1] = wc;
    obj->b[2] = f;

    /* Start for MATLABSystem: '<Root>/MATLAB System' */
    obj->a[0] = (G + 2.0) / (G + 2.0);
    obj->a[1] = wc;

    /* Start for MATLABSystem: '<Root>/MATLAB System' */
    obj->a[2] = (2.0 - G) / (G + 2.0);
    break;

   case 5U:
    /* Start for MATLABSystem: '<Root>/MATLAB System' */
    /*  lowshelf (RBJ, S=1) */
    /* Cálculo de los coefifientes de un Filtro Paso-Banda 2º orden */
    /* Parámetos de entrada:  */
    /* wo = frecuencia central (en radianes) */
    /* Q = factor de calidad */
    /*  */
    /* Parametros de salida: */
    /* Coeficientes [b,a] de un filtro IIR de segundo orden */
    G = rt_powd_snf(10.0, G / 40.0);
    sqrtA = sqrt(G);
    p = cos(wc);
    q = sin(wc);

    /*  Coeficientes del ecualizador */
    Q = 1.0 / G;

    /* Start for MATLABSystem: '<Root>/MATLAB System' */
    a0_tmp = (1.0 - Q) * p + (Q + 1.0);
    a0_tmp_0 = 1.4142135623730951 * q / sqrtA;
    a0 = a0_tmp + a0_tmp_0;

    /* Start for MATLABSystem: '<Root>/MATLAB System' */
    /* Normalización de coeficientes */
    /* fvtool(b,a); */
    f = (1.0 - G) * p + (G + 1.0);
    wc = 1.4142135623730951 * sqrtA * q;
    obj->b[0] = (f + wc) / a0;
    obj->b[1] = ((G + 1.0) * p + (1.0 - G)) * -2.0 / a0;
    obj->b[2] = (f - wc) / a0;
    obj->a[0] = a0 / a0;
    obj->a[1] = ((Q + 1.0) * p + (1.0 - Q)) * -2.0 / a0;
    obj->a[2] = (a0_tmp - a0_tmp_0) / a0;
    break;

   case 6U:
    /* Start for MATLABSystem: '<Root>/MATLAB System' */
    /*  highshelf (RBJ, S=1) */
    /* Cálculo de los coefifientes de un Filtro Paso-Banda 2º orden */
    /* Parámetos de entrada:  */
    /* wo = frecuencia central (en radianes) */
    /* Q = factor de calidad */
    /*  */
    /* Parametros de salida: */
    /* Coeficientes [b,a] de un filtro IIR de segundo orden */
    G = rt_powd_snf(10.0, G / 40.0);
    sqrtA = sqrt(G);
    p = cos(wc);
    q = sin(wc);

    /*  Coeficientes del ecualizador */
    Q = (1.0 - G) * p + (G + 1.0);
    a0_tmp = 1.4142135623730951 * sqrtA * q;
    a0 = Q + a0_tmp;

    /* Start for MATLABSystem: '<Root>/MATLAB System' */
    a0_tmp_0 = G * G;

    /* Normalización de coeficientes */
    /* fvtool(b,a); */
    f = 1.0 / G;
    wc = (1.0 - f) * p + (f + 1.0);
    sqrtA = 1.4142135623730951 * q / sqrtA;
    obj->b[0] = (wc + sqrtA) * a0_tmp_0 / a0;
    obj->b[1] = ((f + 1.0) * p + (1.0 - f)) * -2.0 * a0_tmp_0 / a0;
    obj->b[2] = (wc - sqrtA) * a0_tmp_0 / a0;
    obj->a[0] = a0 / a0;
    obj->a[1] = ((G + 1.0) * p + (1.0 - G)) * -2.0 / a0;
    obj->a[2] = (Q - a0_tmp) / a0;
    break;

   case 7U:
    /* Start for MATLABSystem: '<Root>/MATLAB System' */
    /*  centershelf / peaking (RBJ) */
    /* Cálculo de los coefifientes de un Filtro Paso-Banda 2º orden */
    /* Parámetos de entrada:  */
    /* wo = frecuencia central (en radianes) */
    /* Q = factor de calidad */
    /*  */
    /* Parametros de salida: */
    /* Coeficientes [b,a] de un filtro IIR de segundo orden */
    G = rt_powd_snf(10.0, G / 40.0);
    f = sin(wc) / (2.0 * Q);

    /*  Coeficientes del ecualizador */
    Q = f / G;

    /* Normalización de coeficientes */
    /* fvtool(b,a); */
    f *= G;
    obj->b[0] = (f + 1.0) / (Q + 1.0);
    wc = -2.0 * cos(wc) / (Q + 1.0);
    obj->b[1] = wc;
    obj->b[2] = (1.0 - f) / (Q + 1.0);
    obj->a[0] = (Q + 1.0) / (Q + 1.0);
    obj->a[1] = wc;

    /* Start for MATLABSystem: '<Root>/MATLAB System' */
    obj->a[2] = (1.0 - Q) / (Q + 1.0);
    break;
  }

  /*  Guarda "last" */
  obj->lastFilterType = obj->FilterType;
  obj->lastFreq = obj->Freq;
  obj->lastQ = obj->Q;
  obj->lastGain = obj->Gain;
}

static void filters_filter(real_T b[3], real_T a[3], const real_T x[128], const
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

/* Model step function */
void filters_step(void)
{
  real_T rtb_MATLABSystem_o1_0[128];
  real_T tmp[128];
  real_T tmp_2[3];
  real_T tmp_3[3];
  real_T tmp_1[2];
  real_T tmp_0;
  int32_T i;

  /* S-Function (pisound_in): '<Root>/PiSound Input' */

  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = filters_M->childSfunctions[0];
    sfcnOutputs(rts,0);
  }

  /* MATLABSystem: '<Root>/MATLAB System' */
  if (filters_DW.obj.FilterType != filters_P.FilterType_rt) {
    filters_DW.obj.FilterType = filters_P.FilterType_rt;
  }

  if (filters_DW.obj.Freq != filters_P.Freq_rt) {
    filters_DW.obj.Freq = filters_P.Freq_rt;
  }

  if (filters_DW.obj.Q != filters_P.Q_rt) {
    filters_DW.obj.Q = filters_P.Q_rt;
  }

  if (filters_DW.obj.Gain != filters_P.Gain_rt) {
    filters_DW.obj.Gain = filters_P.Gain_rt;
  }

  /*  Recalcular coefs si cambió algún parámetro */
  if ((filters_DW.obj.FilterType != filters_DW.obj.lastFilterType) ||
      (filters_DW.obj.Freq != filters_DW.obj.lastFreq) || (filters_DW.obj.Q !=
       filters_DW.obj.lastQ) || (filters_DW.obj.Gain != filters_DW.obj.lastGain))
  {
    filter_filters_sys_updateCoeffs(&filters_DW.obj);
  }

  /* Gain: '<Root>/Gain' incorporates:
   *  DataTypeConversion: '<Root>/Data Type Conversion'
   */
  /*  ---- Canal L (idx=1) */
  for (i = 0; i < 128; i++) {
    tmp[i] = filters_P.Gain_Gain * (real_T)filters_B.PiSoundInput_o1[i];
  }

  /* End of Gain: '<Root>/Gain' */

  /* MATLABSystem: '<Root>/MATLAB System' */
  for (i = 0; i < 2; i++) {
    tmp_1[i] = filters_DW.obj.zL[i];
  }

  for (i = 0; i < 3; i++) {
    tmp_2[i] = filters_DW.obj.b[i];
  }

  for (i = 0; i < 3; i++) {
    tmp_3[i] = filters_DW.obj.a[i];
  }

  filters_filter(tmp_2, tmp_3, tmp, tmp_1, rtb_MATLABSystem_o1_0,
                 filters_DW.obj.zL);

  /*  ---- Canal R (idx=2) */
  for (i = 0; i < 128; i++) {
    /* Gain: '<Root>/Gain1' incorporates:
     *  DataTypeConversion: '<Root>/Data Type Conversion1'
     */
    tmp[i] = filters_P.Gain1_Gain * (real_T)filters_B.PiSoundInput_o2[i];

    /* DataTypeConversion: '<Root>/Data Type Conversion2' incorporates:
     *  Gain: '<Root>/Gain2'
     *  MATLABSystem: '<Root>/MATLAB System'
     */
    tmp_0 = floor(filters_P.Gain2_Gain * rtb_MATLABSystem_o1_0[i]);
    if (rtIsNaN(tmp_0) || rtIsInf(tmp_0)) {
      tmp_0 = 0.0;
    } else {
      tmp_0 = fmod(tmp_0, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion2' */
    filters_B.DataTypeConversion2[i] = tmp_0 < 0.0 ? -(int32_T)(uint32_T)-tmp_0 :
      (int32_T)(uint32_T)tmp_0;
  }

  /* MATLABSystem: '<Root>/MATLAB System' */
  for (i = 0; i < 2; i++) {
    tmp_1[i] = filters_DW.obj.zR[i];
  }

  for (i = 0; i < 3; i++) {
    tmp_2[i] = filters_DW.obj.b[i];
  }

  for (i = 0; i < 3; i++) {
    tmp_3[i] = filters_DW.obj.a[i];
  }

  filters_filter(tmp_2, tmp_3, tmp, tmp_1, rtb_MATLABSystem_o1_0,
                 filters_DW.obj.zR);
  for (i = 0; i < 128; i++) {
    /* DataTypeConversion: '<Root>/Data Type Conversion3' incorporates:
     *  Gain: '<Root>/Gain3'
     *  MATLABSystem: '<Root>/MATLAB System'
     */
    tmp_0 = floor(filters_P.Gain3_Gain * rtb_MATLABSystem_o1_0[i]);
    if (rtIsNaN(tmp_0) || rtIsInf(tmp_0)) {
      tmp_0 = 0.0;
    } else {
      tmp_0 = fmod(tmp_0, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion3' */
    filters_B.DataTypeConversion3[i] = tmp_0 < 0.0 ? -(int32_T)(uint32_T)-tmp_0 :
      (int32_T)(uint32_T)tmp_0;
  }

  /* S-Function (pisound_out): '<Root>/PiSound Output' */

  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = filters_M->childSfunctions[1];
    sfcnOutputs(rts,0);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  filters_M->Timing.t[0] =
    ((time_T)(++filters_M->Timing.clockTick0)) * filters_M->Timing.stepSize0;
}

/* Model initialize function */
void filters_initialize(void)
{
  /* Registration code */
  rtsiSetSolverName(&filters_M->solverInfo,"FixedStepDiscrete");
  filters_M->solverInfoPtr = (&filters_M->solverInfo);

  /* Initialize timing info */
  {
    int_T *mdlTsMap = filters_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    filters_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    filters_M->Timing.sampleTimes = (&filters_M->Timing.sampleTimesArray[0]);
    filters_M->Timing.offsetTimes = (&filters_M->Timing.offsetTimesArray[0]);

    /* task periods */
    filters_M->Timing.sampleTimes[0] = (0.0026666666666666666);

    /* task offsets */
    filters_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(filters_M, &filters_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = filters_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    filters_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(filters_M, -1);
  filters_M->Timing.stepSize0 = 0.0026666666666666666;
  filters_M->solverInfoPtr = (&filters_M->solverInfo);
  filters_M->Timing.stepSize = (0.0026666666666666666);
  rtsiSetFixedStepSize(&filters_M->solverInfo, 0.0026666666666666666);
  rtsiSetSolverMode(&filters_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* child S-Function registration */
  {
    RTWSfcnInfo *sfcnInfo = &filters_M->NonInlinedSFcns.sfcnInfo;
    filters_M->sfcnInfo = (sfcnInfo);
    rtssSetErrorStatusPtr(sfcnInfo, (&rtmGetErrorStatus(filters_M)));
    filters_M->Sizes.numSampTimes = (1);
    rtssSetNumRootSampTimesPtr(sfcnInfo, &filters_M->Sizes.numSampTimes);
    filters_M->NonInlinedSFcns.taskTimePtrs[0] = (&rtmGetTPtr(filters_M)[0]);
    rtssSetTPtrPtr(sfcnInfo,filters_M->NonInlinedSFcns.taskTimePtrs);
    rtssSetTStartPtr(sfcnInfo, &rtmGetTStart(filters_M));
    rtssSetTFinalPtr(sfcnInfo, &rtmGetTFinal(filters_M));
    rtssSetTimeOfLastOutputPtr(sfcnInfo, &rtmGetTimeOfLastOutput(filters_M));
    rtssSetStepSizePtr(sfcnInfo, &filters_M->Timing.stepSize);
    rtssSetStopRequestedPtr(sfcnInfo, &rtmGetStopRequested(filters_M));
    rtssSetDerivCacheNeedsResetPtr(sfcnInfo, &filters_M->derivCacheNeedsReset);
    rtssSetZCCacheNeedsResetPtr(sfcnInfo, &filters_M->zCCacheNeedsReset);
    rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(sfcnInfo,
      &filters_M->CTOutputIncnstWithState);
    rtssSetSampleHitsPtr(sfcnInfo, &filters_M->Timing.sampleHits);
    rtssSetPerTaskSampleHitsPtr(sfcnInfo, &filters_M->Timing.perTaskSampleHits);
    rtssSetSimModePtr(sfcnInfo, &filters_M->simMode);
    rtssSetSolverInfoPtr(sfcnInfo, &filters_M->solverInfoPtr);
  }

  filters_M->Sizes.numSFcns = (2);

  /* register each child */
  {
    (void) memset((void *)&filters_M->NonInlinedSFcns.childSFunctions[0], 0,
                  2*sizeof(SimStruct));
    filters_M->childSfunctions = (&filters_M->
      NonInlinedSFcns.childSFunctionPtrs[0]);
    filters_M->childSfunctions[0] = (&filters_M->
      NonInlinedSFcns.childSFunctions[0]);
    filters_M->childSfunctions[1] = (&filters_M->
      NonInlinedSFcns.childSFunctions[1]);

    /* Level2 S-Function Block: filters/<Root>/PiSound Input (pisound_in) */
    {
      SimStruct *rts = filters_M->childSfunctions[0];

      /* timing info */
      time_T *sfcnPeriod = filters_M->NonInlinedSFcns.Sfcn0.sfcnPeriod;
      time_T *sfcnOffset = filters_M->NonInlinedSFcns.Sfcn0.sfcnOffset;
      int_T *sfcnTsMap = filters_M->NonInlinedSFcns.Sfcn0.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &filters_M->NonInlinedSFcns.blkInfo2[0]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &filters_M->NonInlinedSFcns.inputOutputPortInfo2[0]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, filters_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &filters_M->NonInlinedSFcns.methods2[0]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &filters_M->NonInlinedSFcns.methods3[0]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &filters_M->NonInlinedSFcns.methods4[0]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &filters_M->NonInlinedSFcns.statesInfo2[0]);
        ssSetPeriodicStatesInfo(rts,
          &filters_M->NonInlinedSFcns.periodicStatesInfo[0]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &filters_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &filters_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &filters_M->NonInlinedSFcns.Sfcn0.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &filters_M->NonInlinedSFcns.Sfcn0.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 128);
          ssSetOutputPortSignal(rts, 0, ((int32_T *) filters_B.PiSoundInput_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 128);
          ssSetOutputPortSignal(rts, 1, ((int32_T *) filters_B.PiSoundInput_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "PiSound Input");
      ssSetPath(rts, "filters/PiSound Input");
      ssSetRTModel(rts,filters_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &filters_M->NonInlinedSFcns.Sfcn0.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)filters_P.PiSoundInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)filters_P.PiSoundInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)filters_P.PiSoundInput_P3_Size);
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

    /* Level2 S-Function Block: filters/<Root>/PiSound Output (pisound_out) */
    {
      SimStruct *rts = filters_M->childSfunctions[1];

      /* timing info */
      time_T *sfcnPeriod = filters_M->NonInlinedSFcns.Sfcn1.sfcnPeriod;
      time_T *sfcnOffset = filters_M->NonInlinedSFcns.Sfcn1.sfcnOffset;
      int_T *sfcnTsMap = filters_M->NonInlinedSFcns.Sfcn1.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &filters_M->NonInlinedSFcns.blkInfo2[1]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &filters_M->NonInlinedSFcns.inputOutputPortInfo2[1]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, filters_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &filters_M->NonInlinedSFcns.methods2[1]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &filters_M->NonInlinedSFcns.methods3[1]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &filters_M->NonInlinedSFcns.methods4[1]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &filters_M->NonInlinedSFcns.statesInfo2[1]);
        ssSetPeriodicStatesInfo(rts,
          &filters_M->NonInlinedSFcns.periodicStatesInfo[1]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts,
          &filters_M->NonInlinedSFcns.Sfcn1.inputPortInfo[0]);
        ssSetPortInfoForInputs(rts,
          &filters_M->NonInlinedSFcns.Sfcn1.inputPortInfo[0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &filters_M->NonInlinedSFcns.Sfcn1.inputPortUnits[0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &filters_M->NonInlinedSFcns.Sfcn1.inputPortCoSimAttribute[0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          int32_T const **sfcnUPtrs = (int32_T const **)
            &filters_M->NonInlinedSFcns.Sfcn1.UPtrs0;

          {
            int_T i1;
            const int32_T *u0 = filters_B.DataTypeConversion2;
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
            &filters_M->NonInlinedSFcns.Sfcn1.UPtrs1;

          {
            int_T i1;
            const int32_T *u1 = filters_B.DataTypeConversion3;
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
      ssSetPath(rts, "filters/PiSound Output");
      ssSetRTModel(rts,filters_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &filters_M->NonInlinedSFcns.Sfcn1.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)filters_P.PiSoundOutput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)filters_P.PiSoundOutput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)filters_P.PiSoundOutput_P3_Size);
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

  /* Start for MATLABSystem: '<Root>/MATLAB System' */
  filters_DW.obj.FilterType = filters_P.FilterType_rt;
  filters_DW.obj.Freq = filters_P.Freq_rt;
  filters_DW.obj.Q = filters_P.Q_rt;
  filters_DW.obj.Gain = filters_P.Gain_rt;
  filters_DW.obj.isInitialized = 1;
  filters_DW.obj.lastFilterType = 0U;
  filters_DW.obj.lastFreq = (rtNaN);
  filters_DW.obj.lastQ = (rtNaN);
  filters_DW.obj.lastGain = (rtNaN);
  filter_filters_sys_updateCoeffs(&filters_DW.obj);

  /* InitializeConditions for MATLABSystem: '<Root>/MATLAB System' */
  /*  inicializa coefs */
  filters_DW.obj.zL[0] = 0.0;
  filters_DW.obj.zR[0] = 0.0;
  filters_DW.obj.zL[1] = 0.0;
  filters_DW.obj.zR[1] = 0.0;
}

/* Model terminate function */
void filters_terminate(void)
{
  /* Terminate for S-Function (pisound_in): '<Root>/PiSound Input' */
  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = filters_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (pisound_out): '<Root>/PiSound Output' */
  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = filters_M->childSfunctions[1];
    sfcnTerminate(rts);
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
