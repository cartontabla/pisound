/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: vocoder.c
 *
 * Code generated for Simulink model 'vocoder'.
 *
 * Model version                  : 1.106
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Thu Mar 26 10:08:42 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "vocoder.h"
#include "rtwtypes.h"
#include <math.h>
#include "rt_nonfinite.h"
#include "vocoder_private.h"
#include <string.h>

/* Block signals (default storage) */
B_vocoder_T vocoder_B;

/* Block states (default storage) */
DW_vocoder_T vocoder_DW;

/* Real-time model */
static RT_MODEL_vocoder_T vocoder_M_;
RT_MODEL_vocoder_T *const vocoder_M = &vocoder_M_;

/* Forward declaration for local functions */
static real_T vocoder_interp_lin_table(real_T x, const real_T xp[32], const
  real_T yp[32]);
static void vocoder_filter(real_T b[3], real_T a[3], const real_T x[128], const
  real_T zi[2], real_T y[128], real_T zf[2]);

/* Function for MATLAB Function: '<Root>/MATLAB Function' */
static real_T vocoder_interp_lin_table(real_T x, const real_T xp[32], const
  real_T yp[32])
{
  real_T y;
  if (x <= xp[0]) {
    y = yp[0];
  } else if (x >= xp[31]) {
    y = yp[31];
  } else {
    int32_T i;
    boolean_T exitg1;
    y = yp[0];
    i = 0;
    exitg1 = false;
    while ((!exitg1) && (i < 31)) {
      if (x >= xp[i]) {
        real_T tmp;
        tmp = xp[i + 1];
        if (x <= tmp) {
          y = (x - xp[i]) / (tmp - xp[i]) * (yp[i + 1] - yp[i]) + yp[i];
          exitg1 = true;
        } else {
          i++;
        }
      } else {
        i++;
      }
    }
  }

  return y;
}

static void vocoder_filter(real_T b[3], real_T a[3], const real_T x[128], const
  real_T zi[2], real_T y[128], real_T zf[2])
{
  int32_T b_j;
  int32_T e_k;
  int32_T naxpy;

  /* Start for MATLABSystem: '<S1>/VCF' */
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

    /* Start for MATLABSystem: '<S1>/VCF' */
    if (128 - e_k <= 3) {
      naxpy = 128 - e_k;
    } else {
      naxpy = 3;
    }

    for (b_j = 0; b_j < naxpy; b_j++) {
      y_tmp = e_k + b_j;

      /* Start for MATLABSystem: '<S1>/VCF' */
      y[y_tmp] += x[e_k] * b[b_j];
    }

    /* Start for MATLABSystem: '<S1>/VCF' */
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
      /* Start for MATLABSystem: '<S1>/VCF' */
      zf[naxpy] += b[(naxpy - e_k) + 2] * x[e_k + 126];
    }
  }

  for (e_k = 0; e_k < 2; e_k++) {
    for (naxpy = 0; naxpy <= e_k; naxpy++) {
      /* Start for MATLABSystem: '<S1>/VCF' */
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

/* Model step function */
void vocoder_step(void)
{
  real_T rtb_Gain[128];
  real_T rtb_Gain1[128];
  real_T rtb_MIX_o1_0[128];
  real_T rtb_MIX_o2_0[128];
  real_T rtb_VCF_o1_0[128];
  real_T tmp[32];
  real_T tmp_0[32];
  real_T tmp_1[32];
  real_T tmp_2[32];
  real_T FcRefScaled[16];
  real_T rtb_Q[16];
  real_T rtb_freq[16];
  real_T rtb_gain[16];
  real_T rtb_outMax[16];
  real_T rtb_outMin[16];
  real_T fk;
  real_T fmin_hz;
  real_T gw1_;
  real_T gw2_;
  real_T logOutMax;
  real_T logOutMin;
  real_T lx1;
  real_T rtb_Gain1_0;
  real_T rtb_Gain_0;
  int32_T ForEach_itr;
  int32_T i;
  uint8_T rtb_type[16];
  uint8_T rtb_type_0;
  static const int16_T b[16] = { 50, 68, 91, 123, 166, 224, 302, 407, 548, 738,
    995, 1340, 1800, 2420, 3250, 4360 };

  static const real_T yRef[16] = { 0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 1.0, 1.0, 1.5,
    1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5 };

  real_T tmp_4[3];
  real_T tmp_5[3];
  real_T tmp_3[2];
  boolean_T exitg1;

  /* S-Function (pisound_in): '<Root>/PiSound Input' */

  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = vocoder_M->childSfunctions[0];
    sfcnOutputs(rts,0);
  }

  /* Gain: '<Root>/Gain1' incorporates:
   *  DataTypeConversion: '<Root>/Data Type Conversion2'
   */
  for (i = 0; i < 128; i++) {
    rtb_Gain1[i] = vocoder_P.Gain1_Gain * (real_T)vocoder_B.PiSoundInput_o2[i];
  }

  /* End of Gain: '<Root>/Gain1' */

  /* MATLAB Function: '<Root>/MATLAB Function' incorporates:
   *  Constant: '<Root>/Constant1'
   *  Constant: '<Root>/Constant2'
   */
  fmin_hz = vocoder_P.Constant1_Value;
  logOutMax = vocoder_P.Constant2_Value;
  if (vocoder_P.Constant1_Value <= 0.0) {
    fmin_hz = 50.0;
  }

  if (vocoder_P.Constant2_Value <= fmin_hz) {
    logOutMax = 4360.0;
  }

  logOutMin = log(fmin_hz);
  logOutMax = log(logOutMax);
  for (i = 0; i < 16; i++) {
    FcRefScaled[i] = exp((log(b[i]) - 3.912023005428146) / 4.4682043309149329 *
                         (logOutMax - logOutMin) + logOutMin);
  }

  for (i = 0; i < 16; i++) {
    fk = exp((((real_T)i + 1.0) - 1.0) / 15.0 * (logOutMax - logOutMin) +
             logOutMin);
    rtb_type[i] = 3U;
    rtb_freq[i] = fk;
    rtb_Q[i] = 7.0;
    if (fk <= FcRefScaled[0]) {
      rtb_gain[i] = 0.0;
    } else if (fk >= FcRefScaled[15]) {
      rtb_gain[i] = 4.5;
    } else {
      rtb_gain[i] = 0.0;
      ForEach_itr = 0;
      exitg1 = false;
      while ((!exitg1) && (ForEach_itr < 15)) {
        if (fk >= FcRefScaled[ForEach_itr]) {
          fmin_hz = FcRefScaled[ForEach_itr + 1];
          if (fk <= fmin_hz) {
            lx1 = log(FcRefScaled[ForEach_itr]);
            rtb_gain[i] = (log(fk) - lx1) / (log(fmin_hz) - lx1) *
              (yRef[ForEach_itr + 1] - yRef[ForEach_itr]) + yRef[ForEach_itr];
            exitg1 = true;
          } else {
            ForEach_itr++;
          }
        } else {
          ForEach_itr++;
        }
      }
    }
  }

  rtb_type[15] = 2U;
  rtb_freq[15] /= sqrt(exp((logOutMax - logOutMin) / 15.0));
  rtb_Q[15] = 0.70710678118654746;
  rtb_gain[15] += 2.0;
  logOutMax = fmax(rtb_freq[0], 1.0);
  fmin_hz = log(logOutMax);
  logOutMax = log(fmax(rtb_freq[15], logOutMax + 1.0E-12)) - fmin_hz;
  if (fabs(logOutMax) < 1.0E-12) {
    for (i = 0; i < 16; i++) {
      FcRefScaled[i] = (((real_T)i + 1.0) - 1.0) / 15.0;
    }
  } else {
    for (i = 0; i < 16; i++) {
      FcRefScaled[i] = (log(fmax(rtb_freq[i], 1.0)) - fmin_hz) / logOutMax;
    }
  }

  for (i = 0; i < 32; i++) {
    fmin_hz = 0.032258064516129031 * (real_T)i;
    tmp[i] = fmin_hz;
    tmp_0[i] = -60.0;
    tmp_1[i] = fmin_hz;
    tmp_2[i] = 0.0;
  }

  for (i = 0; i < 16; i++) {
    fmin_hz = FcRefScaled[i];
    rtb_outMin[i] = vocoder_interp_lin_table(fmin_hz, tmp, tmp_0);
    rtb_outMax[i] = vocoder_interp_lin_table(fmin_hz, tmp_1, tmp_2);
  }

  /* End of MATLAB Function: '<Root>/MATLAB Function' */

  /* Gain: '<Root>/Gain' incorporates:
   *  DataTypeConversion: '<Root>/Data Type Conversion'
   */
  for (i = 0; i < 128; i++) {
    rtb_Gain[i] = vocoder_P.Gain_Gain * (real_T)vocoder_B.PiSoundInput_o1[i];
  }

  /* End of Gain: '<Root>/Gain' */

  /* Outputs for Iterator SubSystem: '<Root>/Filter Bank' incorporates:
   *  ForEach: '<S1>/For Each'
   */
  for (ForEach_itr = 0; ForEach_itr < 16; ForEach_itr++) {
    /* ForEachSliceSelector generated from: '<S1>/type' */
    /*  acepta double y castea */
    rtb_type_0 = rtb_type[ForEach_itr];

    /* ForEachSliceSelector generated from: '<S1>/freq' */
    logOutMax = rtb_freq[ForEach_itr];

    /* ForEachSliceSelector generated from: '<S1>/Q' */
    logOutMin = rtb_Q[ForEach_itr];

    /* ForEachSliceSelector generated from: '<S1>/gain' */
    fk = rtb_gain[ForEach_itr];

    /* MATLABSystem: '<S1>/VCF' incorporates:
     *  ForEachSliceSelector generated from: '<S1>/Q'
     *  ForEachSliceSelector generated from: '<S1>/freq'
     *  ForEachSliceSelector generated from: '<S1>/gain'
     *  ForEachSliceSelector generated from: '<S1>/type'
     */
    if ((rtb_type_0 != vocoder_DW.CoreSubsys[ForEach_itr].obj.lastType) ||
        (logOutMax != vocoder_DW.CoreSubsys[ForEach_itr].obj.lastFreq) ||
        (logOutMin != vocoder_DW.CoreSubsys[ForEach_itr].obj.lastQ) || (fk !=
         vocoder_DW.CoreSubsys[ForEach_itr].obj.lastGain)) {
      fmin_hz = logOutMax;
      if (rtIsInf(logOutMax) || rtIsNaN(logOutMax)) {
        fmin_hz = 1000.0;
      }

      /*  límites prácticos */
      if (fmin_hz < 0.1) {
        fmin_hz = 0.1;
      }

      if (fmin_hz > 23952.0) {
        fmin_hz = 23952.0;
      }

      fmin_hz = fmin_hz / 48000.0 * 6.2831853071795862;
      if (rtb_type_0 == 2) {
        /*  highpass */
        /* Cálculo de los coefifientes de un Filtro Paso-Alto de 2º orden */
        /* Parámetos de entrada:  */
        /* wo = frecuencia de corte a -3dB (en radianes) */
        /* Parametros de salida: */
        /* Coeficientes [b,a] de un filtro IIR de segundo orden */
        lx1 = cos(fmin_hz);

        /*  Coeficientes del ecualizador */
        gw1_ = 1.4142135623730951 * sin(fmin_hz);

        /* Normalización de coeficientes */
        /*  */
        /* fvtool(b,a); */
        fmin_hz = (lx1 + 1.0) / (gw1_ + 2.0);
        vocoder_DW.CoreSubsys[ForEach_itr].obj.b[0] = fmin_hz;
        vocoder_DW.CoreSubsys[ForEach_itr].obj.b[1] = (lx1 + 1.0) * -2.0 / (gw1_
          + 2.0);
        vocoder_DW.CoreSubsys[ForEach_itr].obj.b[2] = fmin_hz;
        vocoder_DW.CoreSubsys[ForEach_itr].obj.a[0] = (gw1_ + 2.0) / (gw1_ + 2.0);
        vocoder_DW.CoreSubsys[ForEach_itr].obj.a[1] = -4.0 * lx1 / (gw1_ + 2.0);
        vocoder_DW.CoreSubsys[ForEach_itr].obj.a[2] = (2.0 - gw1_) / (gw1_ + 2.0);
      } else {
        /*  bandpass (0 dB peak) */
        /* Cálculo de los coefifientes de un Filtro Paso-Banda 2º orden */
        /* Parámetos de entrada:  */
        /* wo = frecuencia central (en radianes) */
        /* Q = factor de calidad */
        /*  */
        /* Parametros de salida: */
        /* Coeficientes [b,a] de un filtro IIR de segundo orden */
        lx1 = sin(fmin_hz) / logOutMin;

        /*  <-- Q aquí */
        /*  Coeficientes del ecualizador */
        /* Normalización de coeficientes */
        /* fvtool(b,a); */
        vocoder_DW.CoreSubsys[ForEach_itr].obj.b[0] = lx1 / (lx1 + 2.0);
        vocoder_DW.CoreSubsys[ForEach_itr].obj.b[1] = 0.0 / (lx1 + 2.0);
        vocoder_DW.CoreSubsys[ForEach_itr].obj.b[2] = -lx1 / (lx1 + 2.0);
        vocoder_DW.CoreSubsys[ForEach_itr].obj.a[0] = (lx1 + 2.0) / (lx1 + 2.0);
        vocoder_DW.CoreSubsys[ForEach_itr].obj.a[1] = -4.0 * cos(fmin_hz) / (lx1
          + 2.0);
        vocoder_DW.CoreSubsys[ForEach_itr].obj.a[2] = (2.0 - lx1) / (lx1 + 2.0);
      }

      vocoder_DW.CoreSubsys[ForEach_itr].obj.lastType = rtb_type_0;
      vocoder_DW.CoreSubsys[ForEach_itr].obj.lastFreq = logOutMax;
      vocoder_DW.CoreSubsys[ForEach_itr].obj.lastQ = logOutMin;
      vocoder_DW.CoreSubsys[ForEach_itr].obj.lastGain = fk;
    }

    /*  bp es logical */
    for (i = 0; i < 2; i++) {
      tmp_3[i] = vocoder_DW.CoreSubsys[ForEach_itr].obj.zL[i];
    }

    for (i = 0; i < 3; i++) {
      tmp_4[i] = vocoder_DW.CoreSubsys[ForEach_itr].obj.b[i];
    }

    for (i = 0; i < 3; i++) {
      tmp_5[i] = vocoder_DW.CoreSubsys[ForEach_itr].obj.a[i];
    }

    vocoder_filter(tmp_4, tmp_5, rtb_Gain, tmp_3, rtb_VCF_o1_0,
                   vocoder_DW.CoreSubsys[ForEach_itr].obj.zL);
    for (i = 0; i < 2; i++) {
      tmp_3[i] = vocoder_DW.CoreSubsys[ForEach_itr].obj.zR[i];
    }

    for (i = 0; i < 3; i++) {
      tmp_4[i] = vocoder_DW.CoreSubsys[ForEach_itr].obj.b[i];
    }

    for (i = 0; i < 3; i++) {
      tmp_5[i] = vocoder_DW.CoreSubsys[ForEach_itr].obj.a[i];
    }

    vocoder_filter(tmp_4, tmp_5, rtb_Gain1, tmp_3, rtb_MIX_o1_0,
                   vocoder_DW.CoreSubsys[ForEach_itr].obj.zR);

    /* MATLABSystem: '<S3>/ENV' incorporates:
     *  Constant: '<S3>/Constant'
     *  MATLABSystem: '<S1>/VCF'
     */
    if (vocoder_DW.CoreSubsys[ForEach_itr].obj_d.ModeDefault !=
        vocoder_P.CoreSubsys.ENV_ModeDefault) {
      vocoder_DW.CoreSubsys[ForEach_itr].obj_d.ModeDefault =
        vocoder_P.CoreSubsys.ENV_ModeDefault;
    }

    if (vocoder_DW.CoreSubsys[ForEach_itr].obj_d.Eps !=
        vocoder_P.CoreSubsys.ENV_Eps) {
      vocoder_DW.CoreSubsys[ForEach_itr].obj_d.Eps =
        vocoder_P.CoreSubsys.ENV_Eps;
    }

    if (vocoder_DW.CoreSubsys[ForEach_itr].obj_d.TauRMS !=
        vocoder_P.CoreSubsys.ENV_TauRMS) {
      vocoder_DW.CoreSubsys[ForEach_itr].obj_d.TauRMS =
        vocoder_P.CoreSubsys.ENV_TauRMS;
    }

    if (vocoder_P.CoreSubsys.Constant_Value == 1) {
      /*  -------- PEAK (instantáneo) -------- */
      for (i = 0; i < 128; i++) {
        rtb_MIX_o2_0[i] = log10(fabs(rtb_VCF_o1_0[i]) +
          vocoder_DW.CoreSubsys[ForEach_itr].obj_d.Eps) * 20.0;
      }
    } else if (vocoder_P.CoreSubsys.Constant_Value == 2) {
      /*  -------- RMS exponencial (real) -------- */
      fmin_hz = exp(-1.0 / (fmax(vocoder_DW.CoreSubsys[ForEach_itr].obj_d.TauRMS,
        1.0E-6) * 48000.0));
      for (i = 0; i < 128; i++) {
        /* MATLABSystem: '<S1>/VCF' */
        logOutMax = rtb_VCF_o1_0[i];
        logOutMax = (1.0 - fmin_hz) * (logOutMax * logOutMax) + fmin_hz *
          vocoder_DW.CoreSubsys[ForEach_itr].obj_d.e_prev;
        vocoder_DW.CoreSubsys[ForEach_itr].obj_d.e_prev = logOutMax;
        rtb_MIX_o2_0[i] = log10(sqrt(logOutMax +
          vocoder_DW.CoreSubsys[ForEach_itr].obj_d.Eps)) * 20.0;
      }
    } else {
      for (i = 0; i < 128; i++) {
        rtb_MIX_o2_0[i] = log10(fabs(rtb_VCF_o1_0[i]) +
          vocoder_DW.CoreSubsys[ForEach_itr].obj_d.Eps) * 20.0;
      }
    }

    /* MATLABSystem: '<S3>/MAP' incorporates:
     *  ForEachSliceSelector generated from: '<S1>/outMax'
     *  ForEachSliceSelector generated from: '<S1>/outMin'
     *  MATLABSystem: '<S3>/ENV'
     */
    if (vocoder_DW.CoreSubsys[ForEach_itr].obj_e.Mode !=
        vocoder_P.CoreSubsys.MAP_Mode) {
      vocoder_DW.CoreSubsys[ForEach_itr].obj_e.Mode =
        vocoder_P.CoreSubsys.MAP_Mode;
    }

    if (vocoder_DW.CoreSubsys[ForEach_itr].obj_e.Offset !=
        vocoder_P.CoreSubsys.MAP_Offset) {
      vocoder_DW.CoreSubsys[ForEach_itr].obj_e.Offset =
        vocoder_P.CoreSubsys.MAP_Offset;
    }

    if (vocoder_DW.CoreSubsys[ForEach_itr].obj_e.Threshold !=
        vocoder_P.CoreSubsys.MAP_Threshold) {
      vocoder_DW.CoreSubsys[ForEach_itr].obj_e.Threshold =
        vocoder_P.CoreSubsys.MAP_Threshold;
    }

    if (vocoder_DW.CoreSubsys[ForEach_itr].obj_e.Slope !=
        vocoder_P.CoreSubsys.MAP_Slope) {
      vocoder_DW.CoreSubsys[ForEach_itr].obj_e.Slope =
        vocoder_P.CoreSubsys.MAP_Slope;
    }

    switch (vocoder_DW.CoreSubsys[ForEach_itr].obj_e.Mode) {
     case 1U:
      /* ForEachSliceSelector generated from: '<S1>/outMin' */
      /*  vocoder_env_db con threshold+slope */
      logOutMax = rtb_outMin[ForEach_itr];
      for (i = 0; i < 128; i++) {
        rtb_MIX_o2_0[i] = fmin(fmax(fmax(rtb_MIX_o2_0[i] -
          vocoder_DW.CoreSubsys[ForEach_itr].obj_e.Threshold, 0.0) *
          vocoder_DW.CoreSubsys[ForEach_itr].obj_e.Slope + logOutMax,
          rtb_outMin[ForEach_itr]), rtb_outMax[ForEach_itr]);
      }
      break;

     case 2U:
      /* ForEachSliceSelector generated from: '<S1>/outMin' */
      /*  dB offset + clip */
      logOutMax = rtb_outMin[ForEach_itr];

      /* ForEachSliceSelector generated from: '<S1>/outMax' */
      logOutMin = rtb_outMax[ForEach_itr];
      fmin_hz = fmin(logOutMax, logOutMin);
      logOutMax = fmax(logOutMax, logOutMin);
      for (i = 0; i < 128; i++) {
        rtb_MIX_o2_0[i] = fmin(fmax(rtb_MIX_o2_0[i] +
          vocoder_DW.CoreSubsys[ForEach_itr].obj_e.Offset, fmin_hz), logOutMax);
      }
      break;
    }

    /* MATLABSystem: '<S3>/ARF' incorporates:
     *  Constant: '<S1>/Constant2'
     *  Constant: '<S1>/Constant3'
     *  MATLABSystem: '<S3>/MAP'
     */
    /*  bypass del suavizado */
    /*  tiempos válidos */
    fmin_hz = exp(-1.0 / (fmax(vocoder_P.Attack_rt, 1.0E-6) * 48000.0));
    logOutMax = exp(-1.0 / (fmax(vocoder_P.Release_rt, 1.0E-6) * 48000.0));
    logOutMin = vocoder_DW.CoreSubsys[ForEach_itr].obj_f.gPrev_dB;
    for (i = 0; i < 128; i++) {
      /* MATLABSystem: '<S3>/MAP' */
      fk = rtb_MIX_o2_0[i];
      if (fk < logOutMin) {
        /*  más reducción de ganancia -> attack */
        logOutMin = (1.0 - fmin_hz) * fk + fmin_hz * logOutMin;
      } else {
        /*  menos reducción de ganancia -> release */
        logOutMin = (1.0 - logOutMax) * fk + logOutMax * logOutMin;
      }

      rtb_VCF_o1_0[i] = logOutMin;
    }

    vocoder_DW.CoreSubsys[ForEach_itr].obj_f.gPrev_dB = logOutMin;

    /* End of MATLABSystem: '<S3>/ARF' */

    /* MATLABSystem: '<S1>/VCA' */
    if (vocoder_DW.CoreSubsys[ForEach_itr].obj_dx.Mode !=
        vocoder_P.CoreSubsys.VCA_Mode) {
      vocoder_DW.CoreSubsys[ForEach_itr].obj_dx.Mode =
        vocoder_P.CoreSubsys.VCA_Mode;
    }

    if (vocoder_DW.CoreSubsys[ForEach_itr].obj_dx.ForceGainEnable !=
        vocoder_P.CoreSubsys.VCA_ForceGainEnable) {
      vocoder_DW.CoreSubsys[ForEach_itr].obj_dx.ForceGainEnable =
        vocoder_P.CoreSubsys.VCA_ForceGainEnable;
    }

    if (vocoder_DW.CoreSubsys[ForEach_itr].obj_dx.ForceGainValue !=
        vocoder_P.CoreSubsys.VCA_ForceGainValue) {
      vocoder_DW.CoreSubsys[ForEach_itr].obj_dx.ForceGainValue =
        vocoder_P.CoreSubsys.VCA_ForceGainValue;
    }

    /*  ------------------------- */
    /*  INTERFAZ */
    /*  ------------------------- */
    /*  bp es logical */
    if (vocoder_DW.CoreSubsys[ForEach_itr].obj_dx.ForceGainEnable != 0) {
      for (i = 0; i < 128; i++) {
        rtb_VCF_o1_0[i] = vocoder_DW.CoreSubsys[ForEach_itr].
          obj_dx.ForceGainValue;
      }
    }

    /*  --- convertir si está en dB --- */
    if (vocoder_DW.CoreSubsys[ForEach_itr].obj_dx.Mode == 1) {
      for (i = 0; i < 128; i++) {
        rtb_VCF_o1_0[i] = rt_powd_snf(10.0, rtb_VCF_o1_0[i] / 20.0);
      }
    }

    /* ForEachSliceAssignment generated from: '<S1>/audio' incorporates:
     *  MATLABSystem: '<S1>/VCA'
     *  MATLABSystem: '<S1>/VCF'
     */
    for (i = 0; i < 128; i++) {
      vocoder_B.ImpAsg_InsertedFor_audio_at[i + (ForEach_itr << 7)] =
        rtb_VCF_o1_0[i] * rtb_MIX_o1_0[i];
    }

    /* End of ForEachSliceAssignment generated from: '<S1>/audio' */
  }

  /* End of Outputs for SubSystem: '<Root>/Filter Bank' */

  /* Sum: '<Root>/Sum of Elements' incorporates:
   *  ForEachSliceAssignment generated from: '<S1>/audio'
   */
  for (i = 0; i < 128; i++) {
    fmin_hz = -0.0;
    for (ForEach_itr = 0; ForEach_itr < 16; ForEach_itr++) {
      fmin_hz += vocoder_B.ImpAsg_InsertedFor_audio_at[(ForEach_itr << 7) + i];
    }

    rtb_Gain[i] = fmin_hz;
  }

  /* MATLABSystem: '<Root>/MIX' incorporates:
   *  Constant: '<Root>/Constant15'
   */
  if (vocoder_DW.obj.SmoothTime != vocoder_P.MIX_SmoothTime) {
    vocoder_DW.obj.SmoothTime = vocoder_P.MIX_SmoothTime;
  }

  if (vocoder_DW.obj.IndepInDB != vocoder_P.MIX_IndepInDB) {
    vocoder_DW.obj.IndepInDB = vocoder_P.MIX_IndepInDB;
  }

  /*  inL,inR,wetL,wetR,p1 (+p2 si Mode=2) */
  /*  Mode 3: crossfade por muestra (p1 puede ser [N x 1]) */
  /*  dummy */
  logOutMax = fmin(fmax(vocoder_P.Mix_rt, 0.0), 1.0);
  logOutMin = vocoder_DW.obj.a;
  fk = vocoder_DW.obj.gd1;
  lx1 = vocoder_DW.obj.gd2;
  gw1_ = vocoder_DW.obj.gw1;
  gw2_ = vocoder_DW.obj.gw2;

  /*  más robusto que obj.N */
  for (i = 0; i < 128; i++) {
    fk += ((1.0 - logOutMax) - fk) * logOutMin;
    gw1_ += (logOutMax - gw1_) * logOutMin;
    rtb_Gain1_0 = rtb_Gain1[i];

    /* Sum: '<Root>/Sum of Elements' */
    rtb_Gain_0 = rtb_Gain[i];
    lx1 += ((1.0 - logOutMax) - lx1) * logOutMin;
    gw2_ += (logOutMax - gw2_) * logOutMin;

    /* DataTypeConversion: '<Root>/Data Type Conversion1' incorporates:
     *  Gain: '<Root>/Gain2'
     *  Sum: '<Root>/Sum of Elements'
     */
    fmin_hz = floor((fk * rtb_Gain1_0 + gw1_ * rtb_Gain_0) *
                    vocoder_P.Gain2_Gain);
    if (rtIsNaN(fmin_hz) || rtIsInf(fmin_hz)) {
      fmin_hz = 0.0;
    } else {
      fmin_hz = fmod(fmin_hz, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion1' */
    vocoder_B.DataTypeConversion1[i] = fmin_hz < 0.0 ? -(int32_T)(uint32_T)
      -fmin_hz : (int32_T)(uint32_T)fmin_hz;

    /* DataTypeConversion: '<Root>/Data Type Conversion3' incorporates:
     *  Gain: '<Root>/Gain3'
     *  Sum: '<Root>/Sum of Elements'
     */
    fmin_hz = floor((lx1 * rtb_Gain1_0 + gw2_ * rtb_Gain_0) *
                    vocoder_P.Gain3_Gain);
    if (rtIsNaN(fmin_hz) || rtIsInf(fmin_hz)) {
      fmin_hz = 0.0;
    } else {
      fmin_hz = fmod(fmin_hz, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion3' */
    vocoder_B.DataTypeConversion3[i] = fmin_hz < 0.0 ? -(int32_T)(uint32_T)
      -fmin_hz : (int32_T)(uint32_T)fmin_hz;
  }

  vocoder_DW.obj.gd1 = fk;
  vocoder_DW.obj.gd2 = lx1;
  vocoder_DW.obj.gw1 = gw1_;
  vocoder_DW.obj.gw2 = gw2_;

  /* End of MATLABSystem: '<Root>/MIX' */

  /* S-Function (pisound_out): '<Root>/PiSound Output' */

  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = vocoder_M->childSfunctions[1];
    sfcnOutputs(rts,0);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  vocoder_M->Timing.t[0] =
    ((time_T)(++vocoder_M->Timing.clockTick0)) * vocoder_M->Timing.stepSize0;
}

/* Model initialize function */
void vocoder_initialize(void)
{
  /* Registration code */
  rtsiSetSolverName(&vocoder_M->solverInfo,"FixedStepDiscrete");
  vocoder_M->solverInfoPtr = (&vocoder_M->solverInfo);

  /* Initialize timing info */
  {
    int_T *mdlTsMap = vocoder_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    vocoder_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    vocoder_M->Timing.sampleTimes = (&vocoder_M->Timing.sampleTimesArray[0]);
    vocoder_M->Timing.offsetTimes = (&vocoder_M->Timing.offsetTimesArray[0]);

    /* task periods */
    vocoder_M->Timing.sampleTimes[0] = (0.0026666666666666666);

    /* task offsets */
    vocoder_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(vocoder_M, &vocoder_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = vocoder_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    vocoder_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(vocoder_M, -1);
  vocoder_M->Timing.stepSize0 = 0.0026666666666666666;
  vocoder_M->solverInfoPtr = (&vocoder_M->solverInfo);
  vocoder_M->Timing.stepSize = (0.0026666666666666666);
  rtsiSetFixedStepSize(&vocoder_M->solverInfo, 0.0026666666666666666);
  rtsiSetSolverMode(&vocoder_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* child S-Function registration */
  {
    RTWSfcnInfo *sfcnInfo = &vocoder_M->NonInlinedSFcns.sfcnInfo;
    vocoder_M->sfcnInfo = (sfcnInfo);
    rtssSetErrorStatusPtr(sfcnInfo, (&rtmGetErrorStatus(vocoder_M)));
    vocoder_M->Sizes.numSampTimes = (1);
    rtssSetNumRootSampTimesPtr(sfcnInfo, &vocoder_M->Sizes.numSampTimes);
    vocoder_M->NonInlinedSFcns.taskTimePtrs[0] = (&rtmGetTPtr(vocoder_M)[0]);
    rtssSetTPtrPtr(sfcnInfo,vocoder_M->NonInlinedSFcns.taskTimePtrs);
    rtssSetTStartPtr(sfcnInfo, &rtmGetTStart(vocoder_M));
    rtssSetTFinalPtr(sfcnInfo, &rtmGetTFinal(vocoder_M));
    rtssSetTimeOfLastOutputPtr(sfcnInfo, &rtmGetTimeOfLastOutput(vocoder_M));
    rtssSetStepSizePtr(sfcnInfo, &vocoder_M->Timing.stepSize);
    rtssSetStopRequestedPtr(sfcnInfo, &rtmGetStopRequested(vocoder_M));
    rtssSetDerivCacheNeedsResetPtr(sfcnInfo, &vocoder_M->derivCacheNeedsReset);
    rtssSetZCCacheNeedsResetPtr(sfcnInfo, &vocoder_M->zCCacheNeedsReset);
    rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(sfcnInfo,
      &vocoder_M->CTOutputIncnstWithState);
    rtssSetSampleHitsPtr(sfcnInfo, &vocoder_M->Timing.sampleHits);
    rtssSetPerTaskSampleHitsPtr(sfcnInfo, &vocoder_M->Timing.perTaskSampleHits);
    rtssSetSimModePtr(sfcnInfo, &vocoder_M->simMode);
    rtssSetSolverInfoPtr(sfcnInfo, &vocoder_M->solverInfoPtr);
  }

  vocoder_M->Sizes.numSFcns = (2);

  /* register each child */
  {
    (void) memset((void *)&vocoder_M->NonInlinedSFcns.childSFunctions[0], 0,
                  2*sizeof(SimStruct));
    vocoder_M->childSfunctions = (&vocoder_M->
      NonInlinedSFcns.childSFunctionPtrs[0]);
    vocoder_M->childSfunctions[0] = (&vocoder_M->
      NonInlinedSFcns.childSFunctions[0]);
    vocoder_M->childSfunctions[1] = (&vocoder_M->
      NonInlinedSFcns.childSFunctions[1]);

    /* Level2 S-Function Block: vocoder/<Root>/PiSound Input (pisound_in) */
    {
      SimStruct *rts = vocoder_M->childSfunctions[0];

      /* timing info */
      time_T *sfcnPeriod = vocoder_M->NonInlinedSFcns.Sfcn0.sfcnPeriod;
      time_T *sfcnOffset = vocoder_M->NonInlinedSFcns.Sfcn0.sfcnOffset;
      int_T *sfcnTsMap = vocoder_M->NonInlinedSFcns.Sfcn0.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &vocoder_M->NonInlinedSFcns.blkInfo2[0]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &vocoder_M->NonInlinedSFcns.inputOutputPortInfo2[0]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, vocoder_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &vocoder_M->NonInlinedSFcns.methods2[0]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &vocoder_M->NonInlinedSFcns.methods3[0]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &vocoder_M->NonInlinedSFcns.methods4[0]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &vocoder_M->NonInlinedSFcns.statesInfo2[0]);
        ssSetPeriodicStatesInfo(rts,
          &vocoder_M->NonInlinedSFcns.periodicStatesInfo[0]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &vocoder_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &vocoder_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &vocoder_M->NonInlinedSFcns.Sfcn0.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &vocoder_M->NonInlinedSFcns.Sfcn0.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 128);
          ssSetOutputPortSignal(rts, 0, ((int32_T *) vocoder_B.PiSoundInput_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 128);
          ssSetOutputPortSignal(rts, 1, ((int32_T *) vocoder_B.PiSoundInput_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "PiSound Input");
      ssSetPath(rts, "vocoder/PiSound Input");
      ssSetRTModel(rts,vocoder_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &vocoder_M->NonInlinedSFcns.Sfcn0.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)vocoder_P.PiSoundInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)vocoder_P.PiSoundInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)vocoder_P.PiSoundInput_P3_Size);
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

    /* Level2 S-Function Block: vocoder/<Root>/PiSound Output (pisound_out) */
    {
      SimStruct *rts = vocoder_M->childSfunctions[1];

      /* timing info */
      time_T *sfcnPeriod = vocoder_M->NonInlinedSFcns.Sfcn1.sfcnPeriod;
      time_T *sfcnOffset = vocoder_M->NonInlinedSFcns.Sfcn1.sfcnOffset;
      int_T *sfcnTsMap = vocoder_M->NonInlinedSFcns.Sfcn1.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &vocoder_M->NonInlinedSFcns.blkInfo2[1]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &vocoder_M->NonInlinedSFcns.inputOutputPortInfo2[1]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, vocoder_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &vocoder_M->NonInlinedSFcns.methods2[1]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &vocoder_M->NonInlinedSFcns.methods3[1]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &vocoder_M->NonInlinedSFcns.methods4[1]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &vocoder_M->NonInlinedSFcns.statesInfo2[1]);
        ssSetPeriodicStatesInfo(rts,
          &vocoder_M->NonInlinedSFcns.periodicStatesInfo[1]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts,
          &vocoder_M->NonInlinedSFcns.Sfcn1.inputPortInfo[0]);
        ssSetPortInfoForInputs(rts,
          &vocoder_M->NonInlinedSFcns.Sfcn1.inputPortInfo[0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &vocoder_M->NonInlinedSFcns.Sfcn1.inputPortUnits[0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &vocoder_M->NonInlinedSFcns.Sfcn1.inputPortCoSimAttribute[0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          int32_T const **sfcnUPtrs = (int32_T const **)
            &vocoder_M->NonInlinedSFcns.Sfcn1.UPtrs0;

          {
            int_T i1;
            const int32_T *u0 = vocoder_B.DataTypeConversion1;
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
            &vocoder_M->NonInlinedSFcns.Sfcn1.UPtrs1;

          {
            int_T i1;
            const int32_T *u1 = vocoder_B.DataTypeConversion3;
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
      ssSetPath(rts, "vocoder/PiSound Output");
      ssSetRTModel(rts,vocoder_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &vocoder_M->NonInlinedSFcns.Sfcn1.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)vocoder_P.PiSoundOutput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)vocoder_P.PiSoundOutput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)vocoder_P.PiSoundOutput_P3_Size);
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

  {
    int32_T ForEach_itr;

    /* SystemInitialize for Iterator SubSystem: '<Root>/Filter Bank' */
    for (ForEach_itr = 0; ForEach_itr < 16; ForEach_itr++) {
      /* Start for MATLABSystem: '<S1>/VCF' */
      vocoder_DW.CoreSubsys[ForEach_itr].obj.isInitialized = 1;

      /*  arranca en bypass */
      vocoder_DW.CoreSubsys[ForEach_itr].obj.b[0] = 1.0;
      vocoder_DW.CoreSubsys[ForEach_itr].obj.a[0] = 1.0;
      vocoder_DW.CoreSubsys[ForEach_itr].obj.b[1] = 0.0;
      vocoder_DW.CoreSubsys[ForEach_itr].obj.a[1] = 0.0;
      vocoder_DW.CoreSubsys[ForEach_itr].obj.b[2] = 0.0;
      vocoder_DW.CoreSubsys[ForEach_itr].obj.a[2] = 0.0;

      /*  fuerza recálculo en el primer frame */
      vocoder_DW.CoreSubsys[ForEach_itr].obj.lastType = MAX_uint8_T;
      vocoder_DW.CoreSubsys[ForEach_itr].obj.lastFreq = (rtNaN);
      vocoder_DW.CoreSubsys[ForEach_itr].obj.lastQ = (rtNaN);
      vocoder_DW.CoreSubsys[ForEach_itr].obj.lastGain = (rtNaN);

      /* InitializeConditions for MATLABSystem: '<S1>/VCF' */
      vocoder_DW.CoreSubsys[ForEach_itr].obj.zL[0] = 0.0;
      vocoder_DW.CoreSubsys[ForEach_itr].obj.zR[0] = 0.0;
      vocoder_DW.CoreSubsys[ForEach_itr].obj.zL[1] = 0.0;
      vocoder_DW.CoreSubsys[ForEach_itr].obj.zR[1] = 0.0;

      /* Start for MATLABSystem: '<S3>/ENV' */
      vocoder_DW.CoreSubsys[ForEach_itr].obj_d.ModeDefault =
        vocoder_P.CoreSubsys.ENV_ModeDefault;
      vocoder_DW.CoreSubsys[ForEach_itr].obj_d.Eps =
        vocoder_P.CoreSubsys.ENV_Eps;
      vocoder_DW.CoreSubsys[ForEach_itr].obj_d.TauRMS =
        vocoder_P.CoreSubsys.ENV_TauRMS;
      vocoder_DW.CoreSubsys[ForEach_itr].obj_d.isInitialized = 1;

      /* InitializeConditions for MATLABSystem: '<S3>/ENV' */
      vocoder_DW.CoreSubsys[ForEach_itr].obj_d.e_prev = 0.0;

      /* Start for MATLABSystem: '<S3>/MAP' */
      vocoder_DW.CoreSubsys[ForEach_itr].obj_e.Mode =
        vocoder_P.CoreSubsys.MAP_Mode;
      vocoder_DW.CoreSubsys[ForEach_itr].obj_e.Offset =
        vocoder_P.CoreSubsys.MAP_Offset;
      vocoder_DW.CoreSubsys[ForEach_itr].obj_e.Threshold =
        vocoder_P.CoreSubsys.MAP_Threshold;
      vocoder_DW.CoreSubsys[ForEach_itr].obj_e.Slope =
        vocoder_P.CoreSubsys.MAP_Slope;
      vocoder_DW.CoreSubsys[ForEach_itr].obj_e.isInitialized = 1;

      /* Start for MATLABSystem: '<S3>/ARF' */
      vocoder_DW.CoreSubsys[ForEach_itr].obj_f.isInitialized = 1;

      /* InitializeConditions for MATLABSystem: '<S3>/ARF' */
      vocoder_DW.CoreSubsys[ForEach_itr].obj_f.gPrev_dB = 0.0;

      /* Start for MATLABSystem: '<S1>/VCA' */
      vocoder_DW.CoreSubsys[ForEach_itr].obj_dx.Mode =
        vocoder_P.CoreSubsys.VCA_Mode;
      vocoder_DW.CoreSubsys[ForEach_itr].obj_dx.ForceGainEnable =
        vocoder_P.CoreSubsys.VCA_ForceGainEnable;
      vocoder_DW.CoreSubsys[ForEach_itr].obj_dx.ForceGainValue =
        vocoder_P.CoreSubsys.VCA_ForceGainValue;
      vocoder_DW.CoreSubsys[ForEach_itr].obj_dx.isInitialized = 1;

      /*  ------------------------- */
      /*  INTERFAZ */
      /*  ------------------------- */
      /*  INTERFAZ */
      /*  ------------------------- */
      /*  INTERFAZ */
      /*  ------------------------- */
      /*  INTERFAZ */
      /*  ------------------------- */
    }

    /* End of SystemInitialize for SubSystem: '<Root>/Filter Bank' */

    /* Start for MATLABSystem: '<Root>/MIX' */
    vocoder_DW.obj.SmoothTime = vocoder_P.MIX_SmoothTime;
    vocoder_DW.obj.IndepInDB = vocoder_P.MIX_IndepInDB;
    vocoder_DW.obj.isInitialized = 1;

    /*  inL,inR,wetL,wetR,p1 (+p2 si Mode=2) */
    if (vocoder_DW.obj.SmoothTime <= 0.0) {
      vocoder_DW.obj.a = 1.0;
    } else {
      vocoder_DW.obj.a = 1.0 - exp(-1.0 / (48000.0 * vocoder_DW.obj.SmoothTime));
    }

    /* InitializeConditions for MATLABSystem: '<Root>/MIX' */
    vocoder_DW.obj.gd1 = 1.0;
    vocoder_DW.obj.gd2 = 1.0;
    vocoder_DW.obj.gw1 = 0.0;
    vocoder_DW.obj.gw2 = 0.0;
  }
}

/* Model terminate function */
void vocoder_terminate(void)
{
  /* Terminate for S-Function (pisound_in): '<Root>/PiSound Input' */
  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = vocoder_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (pisound_out): '<Root>/PiSound Output' */
  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = vocoder_M->childSfunctions[1];
    sfcnTerminate(rts);
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
