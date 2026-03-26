/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: dyn.c
 *
 * Code generated for Simulink model 'dyn'.
 *
 * Model version                  : 1.22
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Fri Mar  6 08:57:56 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "dyn.h"
#include <string.h>
#include <math.h>
#include "rt_nonfinite.h"
#include "dyn_private.h"
#include "rtwtypes.h"

/* Block signals (default storage) */
B_dyn_T dyn_B;

/* Block states (default storage) */
DW_dyn_T dyn_DW;

/* Real-time model */
static RT_MODEL_dyn_T dyn_M_;
RT_MODEL_dyn_T *const dyn_M = &dyn_M_;
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
void dyn_step(void)
{
  real_T rtb_Gain[128];
  real_T rtb_Gain1[128];
  real_T rtb_MATLABSystem_o1[128];
  real_T rtb_MATLABSystem_o2[128];
  real_T GdB;
  real_T alpha;
  real_T e;
  real_T gw2_;
  real_T lower;
  real_T tmp;
  real_T upper;
  int32_T i;
  boolean_T isGate;
  boolean_T isLimit;
  boolean_T isUp;

  /* S-Function (pisound_in): '<Root>/PiSound Input' */

  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = dyn_M->childSfunctions[0];
    sfcnOutputs(rts,0);
  }

  for (i = 0; i < 128; i++) {
    /* Gain: '<Root>/Gain' incorporates:
     *  DataTypeConversion: '<Root>/Data Type Conversion'
     */
    rtb_Gain[i] = dyn_P.Gain_Gain * (real_T)dyn_B.PiSoundInput_o1[i];

    /* Gain: '<Root>/Gain1' incorporates:
     *  DataTypeConversion: '<Root>/Data Type Conversion2'
     */
    rtb_Gain1[i] = dyn_P.Gain1_Gain * (real_T)dyn_B.PiSoundInput_o2[i];
  }

  /* MATLABSystem: '<Root>/MATLAB System1' incorporates:
   *  Constant: '<Root>/Constant'
   */
  if (dyn_DW.obj_n.ModeDefault != dyn_P.MATLABSystem1_ModeDefault) {
    dyn_DW.obj_n.ModeDefault = dyn_P.MATLABSystem1_ModeDefault;
  }

  if (dyn_DW.obj_n.Eps != dyn_P.MATLABSystem1_Eps) {
    dyn_DW.obj_n.Eps = dyn_P.MATLABSystem1_Eps;
  }

  if (dyn_DW.obj_n.TauRMS != dyn_P.MATLABSystem1_TauRMS) {
    dyn_DW.obj_n.TauRMS = dyn_P.MATLABSystem1_TauRMS;
  }

  memset(&rtb_MATLABSystem_o2[0], 0, sizeof(real_T) << 7U);
  if (dyn_P.Constant_Value == 1) {
    /*  -------- PEAK (instantáneo) -------- */
    for (i = 0; i < 128; i++) {
      rtb_MATLABSystem_o2[i] = log10(fmax(fabs(rtb_Gain[i]), fabs(rtb_Gain1[i]))
        + dyn_DW.obj_n.Eps) * 20.0;
    }
  } else if (dyn_P.Constant_Value == 2) {
    /*  -------- RMS (instantáneo por muestra) -------- */
    /*  RMS instantáneo por muestra: sqrt(max(x^2)) */
    for (i = 0; i < 128; i++) {
      e = rtb_Gain[i];
      GdB = rtb_Gain1[i];
      rtb_MATLABSystem_o2[i] = log10(sqrt(fmax(e * e, GdB * GdB) +
        dyn_DW.obj_n.Eps)) * 20.0;

      /*  ya tiene eps dentro */
    }
  } else if (dyn_P.Constant_Value == 3) {
    /*  -------- RMS exponencial (real) -------- */
    alpha = exp(-1.0 / (dyn_DW.obj_n.TauRMS * 48000.0));
    for (i = 0; i < 128; i++) {
      e = rtb_Gain[i];
      GdB = rtb_Gain1[i];
      e = (e * e + GdB * GdB) * 0.5 * (1.0 - alpha) + alpha *
        dyn_DW.obj_n.e_prev;
      dyn_DW.obj_n.e_prev = e;
      rtb_MATLABSystem_o2[i] = log10(sqrt(e + dyn_DW.obj_n.Eps)) * 20.0;
    }
  }

  /* MATLABSystem: '<Root>/MATLAB System2' incorporates:
   *  Constant: '<Root>/Constant4'
   *  Constant: '<Root>/Constant5'
   *  Constant: '<Root>/Constant6'
   *  Constant: '<Root>/Constant7'
   *  MATLABSystem: '<Root>/MATLAB System1'
   */
  if (dyn_DW.obj_o.Eps != dyn_P.MATLABSystem2_Eps) {
    dyn_DW.obj_o.Eps = dyn_P.MATLABSystem2_Eps;
  }

  if (dyn_DW.obj_o.Floor_dB != dyn_P.MATLABSystem2_Floor_dB) {
    dyn_DW.obj_o.Floor_dB = dyn_P.MATLABSystem2_Floor_dB;
  }

  e = dyn_P.Ratio_rt;
  alpha = fmax(dyn_P.Knee_rt, 0.0);

  /*  Signo define el lado activo (si r==0, se trata como 1:1 por seguridad) */
  if (dyn_P.Ratio_rt == 0.0) {
    e = 1.0;
  }

  isUp = (e > 0.0);

  /*  true: comp/limit ; false: exp/gate */
  e = fabs(e);

  /*  magnitud del ratio */
  /*  Casos especiales de infinito (ideal) */
  if (isUp && rtIsInf(e)) {
    isLimit = true;
  } else {
    isLimit = false;
  }

  /*  +Inf */
  if ((!isUp) && rtIsInf(e)) {
    isGate = true;
  } else {
    isGate = false;
  }

  /*  -Inf */
  /*  Normalización mínima (evitar |ratio|<1 que no tiene sentido aquí) */
  if ((!rtIsInf(e)) && (!rtIsNaN(e)) && (e < 1.0)) {
    e = 1.0;
  }

  /*  Procesar frame */
  for (i = 0; i < 128; i++) {
    if (alpha <= dyn_DW.obj_o.Eps) {
      /*  HARD KNEE, por tramos */
      if (isUp) {
        /*  -------- COMP/LIMIT: actúa por encima -------- */
        GdB = rtb_MATLABSystem_o2[i];
        if (GdB <= dyn_P.Threshold_rt) {
          lower = GdB;
        } else if (isLimit) {
          lower = dyn_P.Threshold_rt;

          /*  limitador ideal */
        } else {
          lower = (GdB - dyn_P.Threshold_rt) / e + dyn_P.Threshold_rt;

          /*  compresión */
        }
      } else {
        /*  -------- EXP/GATE: actúa por debajo -------- */
        GdB = rtb_MATLABSystem_o2[i];
        if (GdB >= dyn_P.Threshold_rt) {
          lower = GdB;
        } else if (isGate) {
          /*  gate ideal: equivale a imponer ganancia mínima (suelo) */
          /*  usando Lout = L + Floor  => GdB = Floor (antes de makeup) */
          lower = GdB + dyn_DW.obj_o.Floor_dB;
        } else {
          lower = (GdB - dyn_P.Threshold_rt) * e + dyn_P.Threshold_rt;

          /*  expansión descendente */
        }
      }
    } else {
      /*  SOFT KNEE (transición cuadrática), simétrica arriba/abajo */
      GdB = alpha / 2.0;
      lower = dyn_P.Threshold_rt - GdB;
      upper = GdB + dyn_P.Threshold_rt;
      if (isUp) {
        /*  -------- COMP/LIMIT: transición alrededor de T por arriba -------- */
        GdB = rtb_MATLABSystem_o2[i];
        if (GdB < lower) {
          lower = GdB;
        } else if (GdB > upper) {
          if (isLimit) {
            lower = dyn_P.Threshold_rt;
          } else {
            lower = (GdB - dyn_P.Threshold_rt) / e + dyn_P.Threshold_rt;
          }
        } else {
          /*  dentro del knee: x en [0,K] */
          lower = GdB - lower;
          if (isLimit) {
            /*  equivalente a 1/R -> 0 (clamp) en la fórmula */
            upper = 0.0;

            /*  1/R efectivo */
          } else {
            upper = 1.0 / e;

            /*  pendiente por encima */
          }

          /*  misma forma que tu compresor original: */
          /*  Lout = L + ((slopeTarget - 1) * x^2)/(2K) */
          lower = (upper - 1.0) * (lower * lower) / (2.0 * alpha) + GdB;
        }
      } else {
        /*  -------- EXP/GATE: transición alrededor de T por abajo -------- */
        GdB = rtb_MATLABSystem_o2[i];
        if (GdB > upper) {
          lower = GdB;
        } else if (GdB < lower) {
          if (isGate) {
            lower = GdB + dyn_DW.obj_o.Floor_dB;
          } else {
            lower = (GdB - dyn_P.Threshold_rt) * e + dyn_P.Threshold_rt;
          }
        } else {
          /*  dentro del knee (simétrico): x en [0,K] */
          lower = upper - GdB;
          if (isGate) {
            /*  tender a una ganancia mínima; aquí se aproxima con una transición suave */
            /*  hacia el suelo (la imposición final del suelo garantiza cierre). */
            upper = 0.0;

            /*  "pendiente efectiva" de salida vs entrada muy baja */
          } else {
            upper = e;

            /*  pendiente por debajo (expansión) */
          }

          /*  transición suave de pendiente 1 (arriba del umbral) a slopeTarget (abajo) */
          /*  forma cuadrática simétrica: */
          /*  Lout = L + ((slopeTarget - 1) * x^2)/(2K) */
          lower = (upper - 1.0) * (lower * lower) / (2.0 * alpha) + GdB;

          /*  si es gate, reforzar que no quede por encima de lo esperado */
          if (isGate) {
            /*  En gate, el objetivo es atenuar: asegurar que Lout no sube artificialmente */
            lower = fmin(lower, GdB);
          }
        }
      }
    }

    /*  Ganancia en dB (instantánea) + makeup */
    GdB = (lower - GdB) + dyn_P.Makeup_rt;

    /*  En puerta ideal (o muy agresiva) imponemos suelo de ganancia */
    if (isGate) {
      GdB = fmax(GdB, dyn_DW.obj_o.Floor_dB);
    }

    rtb_MATLABSystem_o1[i] = GdB;
  }

  /* MATLABSystem: '<Root>/MATLAB System3' incorporates:
   *  Constant: '<Root>/Constant2'
   *  Constant: '<Root>/Constant3'
   *  MATLABSystem: '<Root>/MATLAB System2'
   */
  if (dyn_DW.obj_m.AttackDefault != dyn_P.MATLABSystem3_AttackDefault) {
    dyn_DW.obj_m.AttackDefault = dyn_P.MATLABSystem3_AttackDefault;
  }

  if (dyn_DW.obj_m.ReleaseDefault != dyn_P.MATLABSystem3_ReleaseDefault) {
    dyn_DW.obj_m.ReleaseDefault = dyn_P.MATLABSystem3_ReleaseDefault;
  }

  /*  Evitar tiempos no válidos */
  alpha = exp(-1.0 / (fmax(dyn_P.Release_rt, 1.0E-6) * 48000.0));
  e = exp(-1.0 / (fmax(dyn_P.Attack_rt, 1.0E-6) * 48000.0));
  GdB = dyn_DW.obj_m.gPrev_dB;
  for (i = 0; i < 128; i++) {
    /* MATLABSystem: '<Root>/MATLAB System2' */
    lower = rtb_MATLABSystem_o1[i];
    if (lower < GdB) {
      /*  Entrando en compresión (ganancia baja): ataque */
      GdB = (1.0 - alpha) * lower + alpha * GdB;
    } else {
      /*  Saliendo de compresión (ganancia sube): release */
      GdB = (1.0 - e) * lower + e * GdB;
    }

    rtb_MATLABSystem_o2[i] = GdB;
  }

  dyn_DW.obj_m.gPrev_dB = GdB;

  /* MATLABSystem: '<Root>/MATLAB System' incorporates:
   *  Constant: '<Root>/Constant1'
   *  MATLABSystem: '<Root>/MATLAB System3'
   */
  /*  g es Nx1 */
  if (dyn_P.Constant1_Value != 0) {
    for (i = 0; i < 128; i++) {
      rtb_MATLABSystem_o2[i] = rt_powd_snf(10.0, rtb_MATLABSystem_o2[i] / 20.0);
    }

    /*  dB -> lineal, elemento a elemento */
  }

  /*  bp es logical */
  /*  elemento a elemento */
  for (i = 0; i < 128; i++) {
    GdB = rtb_MATLABSystem_o2[i];
    rtb_MATLABSystem_o1[i] = rtb_Gain[i] * GdB;

    /* MATLABSystem: '<Root>/MATLAB System' */
    rtb_MATLABSystem_o2[i] = rtb_Gain1[i] * GdB;
  }

  /* End of MATLABSystem: '<Root>/MATLAB System' */

  /* MATLABSystem: '<Root>/MATLAB System6' incorporates:
   *  Constant: '<Root>/Constant15'
   */
  if (dyn_DW.obj.SmoothTime != dyn_P.MATLABSystem6_SmoothTime) {
    dyn_DW.obj.SmoothTime = dyn_P.MATLABSystem6_SmoothTime;
  }

  if (dyn_DW.obj.IndepInDB != dyn_P.MATLABSystem6_IndepInDB) {
    dyn_DW.obj.IndepInDB = dyn_P.MATLABSystem6_IndepInDB;
  }

  /*  inL,inR,wetL,wetR,p1 (+p2 si Mode=2) */
  /*  Mode 3: crossfade por muestra (p1 puede ser [N x 1]) */
  /*  dummy */
  alpha = fmin(fmax(dyn_P.Mix_rt, 0.0), 1.0);
  e = dyn_DW.obj.a;
  GdB = dyn_DW.obj.gd1;
  lower = dyn_DW.obj.gd2;
  upper = dyn_DW.obj.gw1;
  gw2_ = dyn_DW.obj.gw2;

  /*  más robusto que obj.N */
  for (i = 0; i < 128; i++) {
    GdB += ((1.0 - alpha) - GdB) * e;
    upper += (alpha - upper) * e;
    lower += ((1.0 - alpha) - lower) * e;
    gw2_ += (alpha - gw2_) * e;

    /* DataTypeConversion: '<Root>/Data Type Conversion1' incorporates:
     *  Gain: '<Root>/Gain2'
     *  MATLABSystem: '<Root>/MATLAB System'
     */
    tmp = floor((GdB * rtb_Gain[i] + upper * rtb_MATLABSystem_o1[i]) *
                dyn_P.Gain2_Gain);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion1' */
    dyn_B.DataTypeConversion1[i] = tmp < 0.0 ? -(int32_T)(uint32_T)-tmp :
      (int32_T)(uint32_T)tmp;

    /* DataTypeConversion: '<Root>/Data Type Conversion3' incorporates:
     *  Gain: '<Root>/Gain3'
     *  MATLABSystem: '<Root>/MATLAB System'
     */
    tmp = floor((lower * rtb_Gain1[i] + gw2_ * rtb_MATLABSystem_o2[i]) *
                dyn_P.Gain3_Gain);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion3' */
    dyn_B.DataTypeConversion3[i] = tmp < 0.0 ? -(int32_T)(uint32_T)-tmp :
      (int32_T)(uint32_T)tmp;
  }

  dyn_DW.obj.gd1 = GdB;
  dyn_DW.obj.gd2 = lower;
  dyn_DW.obj.gw1 = upper;
  dyn_DW.obj.gw2 = gw2_;

  /* End of MATLABSystem: '<Root>/MATLAB System6' */

  /* S-Function (pisound_out): '<Root>/PiSound Output' */

  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = dyn_M->childSfunctions[1];
    sfcnOutputs(rts,0);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  dyn_M->Timing.t[0] =
    ((time_T)(++dyn_M->Timing.clockTick0)) * dyn_M->Timing.stepSize0;
}

/* Model initialize function */
void dyn_initialize(void)
{
  /* Registration code */
  rtsiSetSolverName(&dyn_M->solverInfo,"FixedStepDiscrete");
  dyn_M->solverInfoPtr = (&dyn_M->solverInfo);

  /* Initialize timing info */
  {
    int_T *mdlTsMap = dyn_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    dyn_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    dyn_M->Timing.sampleTimes = (&dyn_M->Timing.sampleTimesArray[0]);
    dyn_M->Timing.offsetTimes = (&dyn_M->Timing.offsetTimesArray[0]);

    /* task periods */
    dyn_M->Timing.sampleTimes[0] = (0.0026666666666666666);

    /* task offsets */
    dyn_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(dyn_M, &dyn_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = dyn_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    dyn_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(dyn_M, -1);
  dyn_M->Timing.stepSize0 = 0.0026666666666666666;
  dyn_M->solverInfoPtr = (&dyn_M->solverInfo);
  dyn_M->Timing.stepSize = (0.0026666666666666666);
  rtsiSetFixedStepSize(&dyn_M->solverInfo, 0.0026666666666666666);
  rtsiSetSolverMode(&dyn_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* child S-Function registration */
  {
    RTWSfcnInfo *sfcnInfo = &dyn_M->NonInlinedSFcns.sfcnInfo;
    dyn_M->sfcnInfo = (sfcnInfo);
    rtssSetErrorStatusPtr(sfcnInfo, (&rtmGetErrorStatus(dyn_M)));
    dyn_M->Sizes.numSampTimes = (1);
    rtssSetNumRootSampTimesPtr(sfcnInfo, &dyn_M->Sizes.numSampTimes);
    dyn_M->NonInlinedSFcns.taskTimePtrs[0] = (&rtmGetTPtr(dyn_M)[0]);
    rtssSetTPtrPtr(sfcnInfo,dyn_M->NonInlinedSFcns.taskTimePtrs);
    rtssSetTStartPtr(sfcnInfo, &rtmGetTStart(dyn_M));
    rtssSetTFinalPtr(sfcnInfo, &rtmGetTFinal(dyn_M));
    rtssSetTimeOfLastOutputPtr(sfcnInfo, &rtmGetTimeOfLastOutput(dyn_M));
    rtssSetStepSizePtr(sfcnInfo, &dyn_M->Timing.stepSize);
    rtssSetStopRequestedPtr(sfcnInfo, &rtmGetStopRequested(dyn_M));
    rtssSetDerivCacheNeedsResetPtr(sfcnInfo, &dyn_M->derivCacheNeedsReset);
    rtssSetZCCacheNeedsResetPtr(sfcnInfo, &dyn_M->zCCacheNeedsReset);
    rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(sfcnInfo,
      &dyn_M->CTOutputIncnstWithState);
    rtssSetSampleHitsPtr(sfcnInfo, &dyn_M->Timing.sampleHits);
    rtssSetPerTaskSampleHitsPtr(sfcnInfo, &dyn_M->Timing.perTaskSampleHits);
    rtssSetSimModePtr(sfcnInfo, &dyn_M->simMode);
    rtssSetSolverInfoPtr(sfcnInfo, &dyn_M->solverInfoPtr);
  }

  dyn_M->Sizes.numSFcns = (2);

  /* register each child */
  {
    (void) memset((void *)&dyn_M->NonInlinedSFcns.childSFunctions[0], 0,
                  2*sizeof(SimStruct));
    dyn_M->childSfunctions = (&dyn_M->NonInlinedSFcns.childSFunctionPtrs[0]);
    dyn_M->childSfunctions[0] = (&dyn_M->NonInlinedSFcns.childSFunctions[0]);
    dyn_M->childSfunctions[1] = (&dyn_M->NonInlinedSFcns.childSFunctions[1]);

    /* Level2 S-Function Block: dyn/<Root>/PiSound Input (pisound_in) */
    {
      SimStruct *rts = dyn_M->childSfunctions[0];

      /* timing info */
      time_T *sfcnPeriod = dyn_M->NonInlinedSFcns.Sfcn0.sfcnPeriod;
      time_T *sfcnOffset = dyn_M->NonInlinedSFcns.Sfcn0.sfcnOffset;
      int_T *sfcnTsMap = dyn_M->NonInlinedSFcns.Sfcn0.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &dyn_M->NonInlinedSFcns.blkInfo2[0]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &dyn_M->NonInlinedSFcns.inputOutputPortInfo2[0]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, dyn_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &dyn_M->NonInlinedSFcns.methods2[0]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &dyn_M->NonInlinedSFcns.methods3[0]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &dyn_M->NonInlinedSFcns.methods4[0]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &dyn_M->NonInlinedSFcns.statesInfo2[0]);
        ssSetPeriodicStatesInfo(rts, &dyn_M->NonInlinedSFcns.periodicStatesInfo
          [0]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &dyn_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &dyn_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &dyn_M->NonInlinedSFcns.Sfcn0.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &dyn_M->NonInlinedSFcns.Sfcn0.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 128);
          ssSetOutputPortSignal(rts, 0, ((int32_T *) dyn_B.PiSoundInput_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 128);
          ssSetOutputPortSignal(rts, 1, ((int32_T *) dyn_B.PiSoundInput_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "PiSound Input");
      ssSetPath(rts, "dyn/PiSound Input");
      ssSetRTModel(rts,dyn_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **) &dyn_M->NonInlinedSFcns.Sfcn0.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)dyn_P.PiSoundInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)dyn_P.PiSoundInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)dyn_P.PiSoundInput_P3_Size);
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

    /* Level2 S-Function Block: dyn/<Root>/PiSound Output (pisound_out) */
    {
      SimStruct *rts = dyn_M->childSfunctions[1];

      /* timing info */
      time_T *sfcnPeriod = dyn_M->NonInlinedSFcns.Sfcn1.sfcnPeriod;
      time_T *sfcnOffset = dyn_M->NonInlinedSFcns.Sfcn1.sfcnOffset;
      int_T *sfcnTsMap = dyn_M->NonInlinedSFcns.Sfcn1.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &dyn_M->NonInlinedSFcns.blkInfo2[1]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &dyn_M->NonInlinedSFcns.inputOutputPortInfo2[1]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, dyn_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &dyn_M->NonInlinedSFcns.methods2[1]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &dyn_M->NonInlinedSFcns.methods3[1]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &dyn_M->NonInlinedSFcns.methods4[1]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &dyn_M->NonInlinedSFcns.statesInfo2[1]);
        ssSetPeriodicStatesInfo(rts, &dyn_M->NonInlinedSFcns.periodicStatesInfo
          [1]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts, &dyn_M->NonInlinedSFcns.Sfcn1.inputPortInfo
          [0]);
        ssSetPortInfoForInputs(rts, &dyn_M->NonInlinedSFcns.Sfcn1.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &dyn_M->NonInlinedSFcns.Sfcn1.inputPortUnits[0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &dyn_M->NonInlinedSFcns.Sfcn1.inputPortCoSimAttribute[0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          int32_T const **sfcnUPtrs = (int32_T const **)
            &dyn_M->NonInlinedSFcns.Sfcn1.UPtrs0;

          {
            int_T i1;
            const int32_T *u0 = dyn_B.DataTypeConversion1;
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
            &dyn_M->NonInlinedSFcns.Sfcn1.UPtrs1;

          {
            int_T i1;
            const int32_T *u1 = dyn_B.DataTypeConversion3;
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
      ssSetPath(rts, "dyn/PiSound Output");
      ssSetRTModel(rts,dyn_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **) &dyn_M->NonInlinedSFcns.Sfcn1.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)dyn_P.PiSoundOutput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)dyn_P.PiSoundOutput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)dyn_P.PiSoundOutput_P3_Size);
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

  /* Start for MATLABSystem: '<Root>/MATLAB System1' */
  dyn_DW.obj_n.ModeDefault = dyn_P.MATLABSystem1_ModeDefault;
  dyn_DW.obj_n.Eps = dyn_P.MATLABSystem1_Eps;
  dyn_DW.obj_n.TauRMS = dyn_P.MATLABSystem1_TauRMS;
  dyn_DW.obj_n.isInitialized = 1;

  /* InitializeConditions for MATLABSystem: '<Root>/MATLAB System1' */
  dyn_DW.obj_n.e_prev = 0.0;

  /* Start for MATLABSystem: '<Root>/MATLAB System2' */
  dyn_DW.obj_o.Eps = dyn_P.MATLABSystem2_Eps;
  dyn_DW.obj_o.Floor_dB = dyn_P.MATLABSystem2_Floor_dB;
  dyn_DW.obj_o.isInitialized = 1;

  /* Start for MATLABSystem: '<Root>/MATLAB System3' */
  /*  permite +/-Inf */
  dyn_DW.obj_m.AttackDefault = dyn_P.MATLABSystem3_AttackDefault;
  dyn_DW.obj_m.ReleaseDefault = dyn_P.MATLABSystem3_ReleaseDefault;
  dyn_DW.obj_m.isInitialized = 1;

  /* InitializeConditions for MATLABSystem: '<Root>/MATLAB System3' */
  /*  ganancia unitaria inicial (0 dB) */
  dyn_DW.obj_m.gPrev_dB = 0.0;

  /* Start for MATLABSystem: '<Root>/MATLAB System6' */
  dyn_DW.obj.SmoothTime = dyn_P.MATLABSystem6_SmoothTime;
  dyn_DW.obj.IndepInDB = dyn_P.MATLABSystem6_IndepInDB;
  dyn_DW.obj.isInitialized = 1;

  /*  inL,inR,wetL,wetR,p1 (+p2 si Mode=2) */
  if (dyn_DW.obj.SmoothTime <= 0.0) {
    dyn_DW.obj.a = 1.0;
  } else {
    dyn_DW.obj.a = 1.0 - exp(-1.0 / (48000.0 * dyn_DW.obj.SmoothTime));
  }

  /* InitializeConditions for MATLABSystem: '<Root>/MATLAB System6' */
  dyn_DW.obj.gd1 = 1.0;
  dyn_DW.obj.gd2 = 1.0;
  dyn_DW.obj.gw1 = 0.0;
  dyn_DW.obj.gw2 = 0.0;
}

/* Model terminate function */
void dyn_terminate(void)
{
  /* Terminate for S-Function (pisound_in): '<Root>/PiSound Input' */
  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = dyn_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (pisound_out): '<Root>/PiSound Output' */
  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = dyn_M->childSfunctions[1];
    sfcnTerminate(rts);
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
