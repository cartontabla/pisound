/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: leveller.c
 *
 * Code generated for Simulink model 'leveller'.
 *
 * Model version                  : 1.26
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Sat Apr  4 13:57:20 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "leveller.h"
#include <math.h>
#include <string.h>
#include "leveller_private.h"
#include "rt_nonfinite.h"
#include "rtwtypes.h"

const uint8_T leveller_U8GND = 0U;     /* uint8_T ground */
const uint32_T leveller_U32GND = 0U;   /* uint32_T ground */

/* Block signals (default storage) */
B_leveller_T leveller_B;

/* Block states (default storage) */
DW_leveller_T leveller_DW;

/* Real-time model */
static RT_MODEL_leveller_T leveller_M_;
RT_MODEL_leveller_T *const leveller_M = &leveller_M_;
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
void leveller_step(void)
{
  real_T in[128];
  real_T rtb_Gain[128];
  real_T rtb_Gain1[128];
  real_T rtb_VCA_o1[128];
  real_T varargin_1[128];
  real_T alpha;
  real_T e;
  real_T g;
  real_T gd2_;
  real_T gw1_;
  real_T gw2_;
  real_T tmp;
  int32_T i;

  /* S-Function (pisound_in): '<Root>/PiSound Input' */

  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = leveller_M->childSfunctions[0];
    sfcnOutputs(rts,0);
  }

  for (i = 0; i < 128; i++) {
    /* Gain: '<Root>/Gain' incorporates:
     *  DataTypeConversion: '<Root>/Data Type Conversion'
     */
    rtb_Gain[i] = leveller_P.Gain_Gain * (real_T)leveller_B.PiSoundInput_o1[i];

    /* Gain: '<Root>/Gain1' incorporates:
     *  DataTypeConversion: '<Root>/Data Type Conversion2'
     */
    rtb_Gain1[i] = leveller_P.Gain1_Gain * (real_T)leveller_B.PiSoundInput_o2[i];
  }

  /* MATLABSystem: '<Root>/ENV' incorporates:
   *  Constant: '<Root>/Constant'
   */
  if (leveller_DW.obj_n.ModeDefault != leveller_P.ENV_ModeDefault) {
    leveller_DW.obj_n.ModeDefault = leveller_P.ENV_ModeDefault;
  }

  if (leveller_DW.obj_n.Eps != leveller_P.ENV_Eps) {
    leveller_DW.obj_n.Eps = leveller_P.ENV_Eps;
  }

  if (leveller_DW.obj_n.TauRMS != leveller_P.ENV_TauRMS) {
    leveller_DW.obj_n.TauRMS = leveller_P.ENV_TauRMS;
  }

  /*  Estéreo enlazado por máximo */
  for (i = 0; i < 128; i++) {
    in[i] = fmax(fabs(rtb_Gain[i]), fabs(rtb_Gain1[i]));
  }

  if (leveller_P.Constant_Value == 1) {
    /*  -------- PEAK (instantáneo) -------- */
    for (i = 0; i < 128; i++) {
      varargin_1[i] = log10(in[i] + leveller_DW.obj_n.Eps) * 20.0;
    }
  } else if (leveller_P.Constant_Value == 2) {
    /*  -------- RMS exponencial (real) -------- */
    alpha = exp(-1.0 / (fmax(leveller_DW.obj_n.TauRMS, 1.0E-6) * 48000.0));
    for (i = 0; i < 128; i++) {
      e = in[i];
      e = (1.0 - alpha) * (e * e) + alpha * leveller_DW.obj_n.e_prev;
      leveller_DW.obj_n.e_prev = e;
      varargin_1[i] = log10(sqrt(e + leveller_DW.obj_n.Eps)) * 20.0;
    }
  } else {
    for (i = 0; i < 128; i++) {
      varargin_1[i] = log10(in[i] + leveller_DW.obj_n.Eps) * 20.0;
    }
  }

  /* MATLABSystem: '<Root>/GC' incorporates:
   *  Constant: '<Root>/Constant4'
   *  Constant: '<Root>/Constant5'
   *  MATLABSystem: '<Root>/ENV'
   */
  if (leveller_DW.obj_o.Mode != leveller_P.GC_Mode) {
    leveller_DW.obj_o.Mode = leveller_P.GC_Mode;
  }

  if (leveller_DW.obj_o.Offset != leveller_P.GC_Offset) {
    leveller_DW.obj_o.Offset = leveller_P.GC_Offset;
  }

  if (leveller_DW.obj_o.Threshold != leveller_P.GC_Threshold) {
    leveller_DW.obj_o.Threshold = leveller_P.GC_Threshold;
  }

  if (leveller_DW.obj_o.Slope != leveller_P.GC_Slope) {
    leveller_DW.obj_o.Slope = leveller_P.GC_Slope;
  }

  switch (leveller_DW.obj_o.Mode) {
   case 1U:
    /*  vocoder_env_db con threshold+slope */
    for (i = 0; i < 128; i++) {
      alpha = fmax(fmax(varargin_1[i] - leveller_DW.obj_o.Threshold, 0.0) *
                   leveller_DW.obj_o.Slope + leveller_P.Constant4_Value,
                   leveller_P.Constant4_Value);
      varargin_1[i] = alpha;
      rtb_VCA_o1[i] = fmin(alpha, leveller_P.Constant5_Value);
    }
    break;

   case 2U:
    /*  dB offset + clip */
    e = fmin(leveller_P.Constant4_Value, leveller_P.Constant5_Value);
    g = fmax(leveller_P.Constant4_Value, leveller_P.Constant5_Value);
    for (i = 0; i < 128; i++) {
      alpha = fmax(varargin_1[i] + leveller_DW.obj_o.Offset, e);
      varargin_1[i] = alpha;
      rtb_VCA_o1[i] = fmin(alpha, g);
    }
    break;

   case 3U:
    /*  leveller: G = ref_dB - nivel_actual */
    for (i = 0; i < 128; i++) {
      alpha = fmax(leveller_DW.obj_o.Offset - varargin_1[i],
                   leveller_P.Constant4_Value);
      varargin_1[i] = alpha;
      rtb_VCA_o1[i] = fmin(alpha, leveller_P.Constant5_Value);
    }
    break;

   default:
    memcpy(&rtb_VCA_o1[0], &varargin_1[0], sizeof(real_T) << 7U);
    break;
  }

  /* MATLABSystem: '<Root>/ARF' incorporates:
   *  Constant: '<Root>/Constant2'
   *  Constant: '<Root>/Constant3'
   *  MATLABSystem: '<Root>/GC'
   */
  /*  bypass del suavizado */
  /*  tiempos válidos */
  alpha = exp(-1.0 / (fmax(leveller_P.Constant3_Value, 1.0E-6) * 48000.0));
  e = exp(-1.0 / (fmax(leveller_P.Constant2_Value, 1.0E-6) * 48000.0));
  g = leveller_DW.obj_m.gPrev_dB;
  for (i = 0; i < 128; i++) {
    /* MATLABSystem: '<Root>/GC' */
    gd2_ = rtb_VCA_o1[i];
    if (gd2_ < g) {
      /*  más reducción de ganancia -> attack */
      g = (1.0 - alpha) * gd2_ + alpha * g;
    } else {
      /*  menos reducción de ganancia -> release */
      g = (1.0 - e) * gd2_ + e * g;
    }

    in[i] = g;
  }

  leveller_DW.obj_m.gPrev_dB = g;

  /* MATLABSystem: '<Root>/VCA' incorporates:
   *  Constant: '<Root>/Constant1'
   *  MATLABSystem: '<Root>/ARF'
   */
  /*  g es Nx1 */
  if (leveller_P.Constant1_Value != 0) {
    for (i = 0; i < 128; i++) {
      in[i] = rt_powd_snf(10.0, in[i] / 20.0);
    }

    /*  dB -> lineal, elemento a elemento */
  }

  /*  bp es logical */
  /*  elemento a elemento */
  for (i = 0; i < 128; i++) {
    e = in[i];
    rtb_VCA_o1[i] = rtb_Gain[i] * e;

    /* MATLABSystem: '<Root>/VCA' */
    in[i] = rtb_Gain1[i] * e;
  }

  /* End of MATLABSystem: '<Root>/VCA' */

  /* MATLABSystem: '<Root>/MIX' incorporates:
   *  Constant: '<Root>/Constant15'
   *  MATLABSystem: '<Root>/VCA'
   */
  if (leveller_DW.obj.SmoothTime != leveller_P.MIX_SmoothTime) {
    leveller_DW.obj.SmoothTime = leveller_P.MIX_SmoothTime;
  }

  if (leveller_DW.obj.IndepInDB != leveller_P.MIX_IndepInDB) {
    leveller_DW.obj.IndepInDB = leveller_P.MIX_IndepInDB;
  }

  /*  inL,inR,wetL,wetR,p1 (+p2 si Mode=2) */
  /*  Mode 3: crossfade por muestra (p1 puede ser [N x 1]) */
  /*  dummy */
  alpha = fmin(fmax(leveller_P.Mix_rt, 0.0), 1.0);
  e = leveller_DW.obj.a;
  g = leveller_DW.obj.gd1;
  gd2_ = leveller_DW.obj.gd2;
  gw1_ = leveller_DW.obj.gw1;
  gw2_ = leveller_DW.obj.gw2;

  /*  más robusto que obj.N */
  for (i = 0; i < 128; i++) {
    g += ((1.0 - alpha) - g) * e;
    gw1_ += (alpha - gw1_) * e;
    gd2_ += ((1.0 - alpha) - gd2_) * e;
    gw2_ += (alpha - gw2_) * e;
    varargin_1[i] = gd2_ * rtb_Gain1[i] + gw2_ * in[i];

    /* DataTypeConversion: '<Root>/Data Type Conversion1' incorporates:
     *  Gain: '<Root>/Gain2'
     *  MATLABSystem: '<Root>/VCA'
     */
    tmp = floor((g * rtb_Gain[i] + gw1_ * rtb_VCA_o1[i]) * leveller_P.Gain2_Gain);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion1' */
    leveller_B.DataTypeConversion1[i] = tmp < 0.0 ? -(int32_T)(uint32_T)-tmp :
      (int32_T)(uint32_T)tmp;
  }

  leveller_DW.obj.gd1 = g;
  leveller_DW.obj.gd2 = gd2_;
  leveller_DW.obj.gw1 = gw1_;
  leveller_DW.obj.gw2 = gw2_;

  /* S-Function (ctrl_in): '<Root>/S-Function' */

  /* Level2 S-Function Block: '<Root>/S-Function' (ctrl_in) */
  {
    SimStruct *rts = leveller_M->childSfunctions[1];
    sfcnOutputs(rts,0);
  }

  for (i = 0; i < 128; i++) {
    /* DataTypeConversion: '<Root>/Data Type Conversion3' incorporates:
     *  Gain: '<Root>/Gain3'
     *  MATLABSystem: '<Root>/MIX'
     */
    tmp = floor(leveller_P.Gain3_Gain * varargin_1[i]);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion3' */
    leveller_B.DataTypeConversion3[i] = tmp < 0.0 ? -(int32_T)(uint32_T)-tmp :
      (int32_T)(uint32_T)tmp;
  }

  /* S-Function (pisound_out): '<Root>/PiSound Output' */

  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = leveller_M->childSfunctions[2];
    sfcnOutputs(rts,0);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  leveller_M->Timing.t[0] =
    ((time_T)(++leveller_M->Timing.clockTick0)) * leveller_M->Timing.stepSize0;
}

/* Model initialize function */
void leveller_initialize(void)
{
  /* Registration code */
  rtsiSetSolverName(&leveller_M->solverInfo,"FixedStepDiscrete");
  leveller_M->solverInfoPtr = (&leveller_M->solverInfo);

  /* Initialize timing info */
  {
    int_T *mdlTsMap = leveller_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    leveller_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    leveller_M->Timing.sampleTimes = (&leveller_M->Timing.sampleTimesArray[0]);
    leveller_M->Timing.offsetTimes = (&leveller_M->Timing.offsetTimesArray[0]);

    /* task periods */
    leveller_M->Timing.sampleTimes[0] = (0.0026666666666666666);

    /* task offsets */
    leveller_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(leveller_M, &leveller_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = leveller_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    leveller_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(leveller_M, -1);
  leveller_M->Timing.stepSize0 = 0.0026666666666666666;
  leveller_M->solverInfoPtr = (&leveller_M->solverInfo);
  leveller_M->Timing.stepSize = (0.0026666666666666666);
  rtsiSetFixedStepSize(&leveller_M->solverInfo, 0.0026666666666666666);
  rtsiSetSolverMode(&leveller_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* child S-Function registration */
  {
    RTWSfcnInfo *sfcnInfo = &leveller_M->NonInlinedSFcns.sfcnInfo;
    leveller_M->sfcnInfo = (sfcnInfo);
    rtssSetErrorStatusPtr(sfcnInfo, (&rtmGetErrorStatus(leveller_M)));
    leveller_M->Sizes.numSampTimes = (1);
    rtssSetNumRootSampTimesPtr(sfcnInfo, &leveller_M->Sizes.numSampTimes);
    leveller_M->NonInlinedSFcns.taskTimePtrs[0] = (&rtmGetTPtr(leveller_M)[0]);
    rtssSetTPtrPtr(sfcnInfo,leveller_M->NonInlinedSFcns.taskTimePtrs);
    rtssSetTStartPtr(sfcnInfo, &rtmGetTStart(leveller_M));
    rtssSetTFinalPtr(sfcnInfo, &rtmGetTFinal(leveller_M));
    rtssSetTimeOfLastOutputPtr(sfcnInfo, &rtmGetTimeOfLastOutput(leveller_M));
    rtssSetStepSizePtr(sfcnInfo, &leveller_M->Timing.stepSize);
    rtssSetStopRequestedPtr(sfcnInfo, &rtmGetStopRequested(leveller_M));
    rtssSetDerivCacheNeedsResetPtr(sfcnInfo, &leveller_M->derivCacheNeedsReset);
    rtssSetZCCacheNeedsResetPtr(sfcnInfo, &leveller_M->zCCacheNeedsReset);
    rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(sfcnInfo,
      &leveller_M->CTOutputIncnstWithState);
    rtssSetSampleHitsPtr(sfcnInfo, &leveller_M->Timing.sampleHits);
    rtssSetPerTaskSampleHitsPtr(sfcnInfo, &leveller_M->Timing.perTaskSampleHits);
    rtssSetSimModePtr(sfcnInfo, &leveller_M->simMode);
    rtssSetSolverInfoPtr(sfcnInfo, &leveller_M->solverInfoPtr);
  }

  leveller_M->Sizes.numSFcns = (3);

  /* register each child */
  {
    (void) memset((void *)&leveller_M->NonInlinedSFcns.childSFunctions[0], 0,
                  3*sizeof(SimStruct));
    leveller_M->childSfunctions =
      (&leveller_M->NonInlinedSFcns.childSFunctionPtrs[0]);
    leveller_M->childSfunctions[0] =
      (&leveller_M->NonInlinedSFcns.childSFunctions[0]);
    leveller_M->childSfunctions[1] =
      (&leveller_M->NonInlinedSFcns.childSFunctions[1]);
    leveller_M->childSfunctions[2] =
      (&leveller_M->NonInlinedSFcns.childSFunctions[2]);

    /* Level2 S-Function Block: leveller/<Root>/PiSound Input (pisound_in) */
    {
      SimStruct *rts = leveller_M->childSfunctions[0];

      /* timing info */
      time_T *sfcnPeriod = leveller_M->NonInlinedSFcns.Sfcn0.sfcnPeriod;
      time_T *sfcnOffset = leveller_M->NonInlinedSFcns.Sfcn0.sfcnOffset;
      int_T *sfcnTsMap = leveller_M->NonInlinedSFcns.Sfcn0.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &leveller_M->NonInlinedSFcns.blkInfo2[0]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &leveller_M->NonInlinedSFcns.inputOutputPortInfo2[0]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, leveller_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &leveller_M->NonInlinedSFcns.methods2[0]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &leveller_M->NonInlinedSFcns.methods3[0]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &leveller_M->NonInlinedSFcns.methods4[0]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &leveller_M->NonInlinedSFcns.statesInfo2[0]);
        ssSetPeriodicStatesInfo(rts,
          &leveller_M->NonInlinedSFcns.periodicStatesInfo[0]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &leveller_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &leveller_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 4);
        _ssSetPortInfo2ForOutputUnits(rts,
          &leveller_M->NonInlinedSFcns.Sfcn0.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        ssSetOutputPortUnit(rts, 2, 0);
        ssSetOutputPortUnit(rts, 3, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &leveller_M->NonInlinedSFcns.Sfcn0.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 2, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 3, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 128);
          ssSetOutputPortSignal(rts, 0, ((int32_T *) leveller_B.PiSoundInput_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 128);
          ssSetOutputPortSignal(rts, 1, ((int32_T *) leveller_B.PiSoundInput_o2));
        }

        /* port 2 */
        {
          _ssSetOutputPortNumDimensions(rts, 2, 1);
          ssSetOutputPortWidthAsInt(rts, 2, 128);
          ssSetOutputPortSignal(rts, 2, ((uint8_T *) leveller_B.PiSoundInput_o3));
        }

        /* port 3 */
        {
          _ssSetOutputPortNumDimensions(rts, 3, 1);
          ssSetOutputPortWidthAsInt(rts, 3, 1);
          ssSetOutputPortSignal(rts, 3, ((uint32_T *)
            &leveller_B.PiSoundInput_o4));
        }
      }

      /* path info */
      ssSetModelName(rts, "PiSound Input");
      ssSetPath(rts, "leveller/PiSound Input");
      ssSetRTModel(rts,leveller_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &leveller_M->NonInlinedSFcns.Sfcn0.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)leveller_P.PiSoundInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)leveller_P.PiSoundInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)leveller_P.PiSoundInput_P3_Size);
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
      _ssSetOutputPortConnected(rts, 2, 0);
      _ssSetOutputPortConnected(rts, 3, 0);
      _ssSetOutputPortBeingMerged(rts, 0, 0);
      _ssSetOutputPortBeingMerged(rts, 1, 0);
      _ssSetOutputPortBeingMerged(rts, 2, 0);
      _ssSetOutputPortBeingMerged(rts, 3, 0);

      /* Update the BufferDstPort flags for each input port */
    }

    /* Level2 S-Function Block: leveller/<Root>/S-Function (ctrl_in) */
    {
      SimStruct *rts = leveller_M->childSfunctions[1];

      /* timing info */
      time_T *sfcnPeriod = leveller_M->NonInlinedSFcns.Sfcn1.sfcnPeriod;
      time_T *sfcnOffset = leveller_M->NonInlinedSFcns.Sfcn1.sfcnOffset;
      int_T *sfcnTsMap = leveller_M->NonInlinedSFcns.Sfcn1.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &leveller_M->NonInlinedSFcns.blkInfo2[1]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &leveller_M->NonInlinedSFcns.inputOutputPortInfo2[1]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, leveller_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &leveller_M->NonInlinedSFcns.methods2[1]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &leveller_M->NonInlinedSFcns.methods3[1]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &leveller_M->NonInlinedSFcns.methods4[1]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &leveller_M->NonInlinedSFcns.statesInfo2[1]);
        ssSetPeriodicStatesInfo(rts,
          &leveller_M->NonInlinedSFcns.periodicStatesInfo[1]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &leveller_M->NonInlinedSFcns.Sfcn1.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &leveller_M->NonInlinedSFcns.Sfcn1.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 6);
        _ssSetPortInfo2ForOutputUnits(rts,
          &leveller_M->NonInlinedSFcns.Sfcn1.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        ssSetOutputPortUnit(rts, 2, 0);
        ssSetOutputPortUnit(rts, 3, 0);
        ssSetOutputPortUnit(rts, 4, 0);
        ssSetOutputPortUnit(rts, 5, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &leveller_M->NonInlinedSFcns.Sfcn1.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 2, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 3, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 4, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 5, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 1);
          ssSetOutputPortSignal(rts, 0, ((real_T *) &leveller_B.SFunction_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 1);
          ssSetOutputPortSignal(rts, 1, ((real_T *) &leveller_B.SFunction_o2));
        }

        /* port 2 */
        {
          _ssSetOutputPortNumDimensions(rts, 2, 1);
          ssSetOutputPortWidthAsInt(rts, 2, 1);
          ssSetOutputPortSignal(rts, 2, ((real_T *) &leveller_B.SFunction_o3));
        }

        /* port 3 */
        {
          _ssSetOutputPortNumDimensions(rts, 3, 1);
          ssSetOutputPortWidthAsInt(rts, 3, 1);
          ssSetOutputPortSignal(rts, 3, ((real_T *) &leveller_B.SFunction_o4));
        }

        /* port 4 */
        {
          _ssSetOutputPortNumDimensions(rts, 4, 1);
          ssSetOutputPortWidthAsInt(rts, 4, 1);
          ssSetOutputPortSignal(rts, 4, ((real_T *) &leveller_B.SFunction_o5));
        }

        /* port 5 */
        {
          _ssSetOutputPortNumDimensions(rts, 5, 1);
          ssSetOutputPortWidthAsInt(rts, 5, 1);
          ssSetOutputPortSignal(rts, 5, ((real_T *) &leveller_B.SFunction_o6));
        }
      }

      /* path info */
      ssSetModelName(rts, "S-Function");
      ssSetPath(rts, "leveller/S-Function");
      ssSetRTModel(rts,leveller_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &leveller_M->NonInlinedSFcns.Sfcn1.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)leveller_P.SFunction_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)leveller_P.SFunction_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)leveller_P.SFunction_P3_Size);
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
      _ssSetOutputPortConnected(rts, 0, 0);
      _ssSetOutputPortConnected(rts, 1, 0);
      _ssSetOutputPortConnected(rts, 2, 0);
      _ssSetOutputPortConnected(rts, 3, 0);
      _ssSetOutputPortConnected(rts, 4, 0);
      _ssSetOutputPortConnected(rts, 5, 0);
      _ssSetOutputPortBeingMerged(rts, 0, 0);
      _ssSetOutputPortBeingMerged(rts, 1, 0);
      _ssSetOutputPortBeingMerged(rts, 2, 0);
      _ssSetOutputPortBeingMerged(rts, 3, 0);
      _ssSetOutputPortBeingMerged(rts, 4, 0);
      _ssSetOutputPortBeingMerged(rts, 5, 0);

      /* Update the BufferDstPort flags for each input port */
    }

    /* Level2 S-Function Block: leveller/<Root>/PiSound Output (pisound_out) */
    {
      SimStruct *rts = leveller_M->childSfunctions[2];

      /* timing info */
      time_T *sfcnPeriod = leveller_M->NonInlinedSFcns.Sfcn2.sfcnPeriod;
      time_T *sfcnOffset = leveller_M->NonInlinedSFcns.Sfcn2.sfcnOffset;
      int_T *sfcnTsMap = leveller_M->NonInlinedSFcns.Sfcn2.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &leveller_M->NonInlinedSFcns.blkInfo2[2]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &leveller_M->NonInlinedSFcns.inputOutputPortInfo2[2]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, leveller_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &leveller_M->NonInlinedSFcns.methods2[2]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &leveller_M->NonInlinedSFcns.methods3[2]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &leveller_M->NonInlinedSFcns.methods4[2]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &leveller_M->NonInlinedSFcns.statesInfo2[2]);
        ssSetPeriodicStatesInfo(rts,
          &leveller_M->NonInlinedSFcns.periodicStatesInfo[2]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 4);
        ssSetPortInfoForInputs(rts,
          &leveller_M->NonInlinedSFcns.Sfcn2.inputPortInfo[0]);
        ssSetPortInfoForInputs(rts,
          &leveller_M->NonInlinedSFcns.Sfcn2.inputPortInfo[0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &leveller_M->NonInlinedSFcns.Sfcn2.inputPortUnits[0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        ssSetInputPortUnit(rts, 2, 0);
        ssSetInputPortUnit(rts, 3, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &leveller_M->NonInlinedSFcns.Sfcn2.inputPortCoSimAttribute[0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);
        ssSetInputPortIsContinuousQuantity(rts, 2, 0);
        ssSetInputPortIsContinuousQuantity(rts, 3, 0);

        /* port 0 */
        {
          int32_T const **sfcnUPtrs = (int32_T const **)
            &leveller_M->NonInlinedSFcns.Sfcn2.UPtrs0;

          {
            int_T i1;
            const int32_T *u0 = leveller_B.DataTypeConversion1;
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
            &leveller_M->NonInlinedSFcns.Sfcn2.UPtrs1;

          {
            int_T i1;
            const int32_T *u1 = leveller_B.DataTypeConversion3;
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
            &leveller_M->NonInlinedSFcns.Sfcn2.UPtrs2;

          {
            int_T i1;
            for (i1=0; i1 < 128; i1++) {
              sfcnUPtrs[i1] = ((const uint8_T*) &leveller_U8GND);
            }
          }

          ssSetInputPortSignalPtrs(rts, 2, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 2, 1);
          ssSetInputPortWidthAsInt(rts, 2, 128);
        }

        /* port 3 */
        {
          uint32_T const **sfcnUPtrs = (uint32_T const **)
            &leveller_M->NonInlinedSFcns.Sfcn2.UPtrs3;
          sfcnUPtrs[0] = ((const uint32_T*) &leveller_U32GND);
          ssSetInputPortSignalPtrs(rts, 3, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 3, 1);
          ssSetInputPortWidthAsInt(rts, 3, 1);
        }
      }

      /* path info */
      ssSetModelName(rts, "PiSound Output");
      ssSetPath(rts, "leveller/PiSound Output");
      ssSetRTModel(rts,leveller_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &leveller_M->NonInlinedSFcns.Sfcn2.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)leveller_P.PiSoundOutput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)leveller_P.PiSoundOutput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)leveller_P.PiSoundOutput_P3_Size);
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
      _ssSetInputPortConnected(rts, 2, 0);
      _ssSetInputPortConnected(rts, 3, 0);

      /* Update the BufferDstPort flags for each input port */
      ssSetInputPortBufferDstPort(rts, 0, -1);
      ssSetInputPortBufferDstPort(rts, 1, -1);
      ssSetInputPortBufferDstPort(rts, 2, -1);
      ssSetInputPortBufferDstPort(rts, 3, -1);
    }
  }

  /* Start for MATLABSystem: '<Root>/ENV' */
  leveller_DW.obj_n.ModeDefault = leveller_P.ENV_ModeDefault;
  leveller_DW.obj_n.Eps = leveller_P.ENV_Eps;
  leveller_DW.obj_n.TauRMS = leveller_P.ENV_TauRMS;
  leveller_DW.obj_n.isInitialized = 1;

  /* InitializeConditions for MATLABSystem: '<Root>/ENV' */
  leveller_DW.obj_n.e_prev = 0.0;

  /* Start for MATLABSystem: '<Root>/GC' */
  leveller_DW.obj_o.Mode = leveller_P.GC_Mode;
  leveller_DW.obj_o.Offset = leveller_P.GC_Offset;
  leveller_DW.obj_o.Threshold = leveller_P.GC_Threshold;
  leveller_DW.obj_o.Slope = leveller_P.GC_Slope;
  leveller_DW.obj_o.isInitialized = 1;

  /* Start for MATLABSystem: '<Root>/ARF' */
  leveller_DW.obj_m.isInitialized = 1;

  /* InitializeConditions for MATLABSystem: '<Root>/ARF' */
  leveller_DW.obj_m.gPrev_dB = 0.0;

  /* Start for MATLABSystem: '<Root>/MIX' */
  leveller_DW.obj.SmoothTime = leveller_P.MIX_SmoothTime;
  leveller_DW.obj.IndepInDB = leveller_P.MIX_IndepInDB;
  leveller_DW.obj.isInitialized = 1;

  /*  inL,inR,wetL,wetR,p1 (+p2 si Mode=2) */
  if (leveller_DW.obj.SmoothTime <= 0.0) {
    leveller_DW.obj.a = 1.0;
  } else {
    leveller_DW.obj.a = 1.0 - exp(-1.0 / (48000.0 * leveller_DW.obj.SmoothTime));
  }

  /* InitializeConditions for MATLABSystem: '<Root>/MIX' */
  leveller_DW.obj.gd1 = 1.0;
  leveller_DW.obj.gd2 = 1.0;
  leveller_DW.obj.gw1 = 0.0;
  leveller_DW.obj.gw2 = 0.0;
}

/* Model terminate function */
void leveller_terminate(void)
{
  /* Terminate for S-Function (pisound_in): '<Root>/PiSound Input' */
  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = leveller_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (ctrl_in): '<Root>/S-Function' */
  /* Level2 S-Function Block: '<Root>/S-Function' (ctrl_in) */
  {
    SimStruct *rts = leveller_M->childSfunctions[1];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (pisound_out): '<Root>/PiSound Output' */
  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = leveller_M->childSfunctions[2];
    sfcnTerminate(rts);
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
