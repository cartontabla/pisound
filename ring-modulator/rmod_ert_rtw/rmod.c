/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: rmod.c
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
#include "rtwtypes.h"
#include "rmod_private.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Block signals (default storage) */
B_rmod_T rmod_B;

/* Block states (default storage) */
DW_rmod_T rmod_DW;

/* Real-time model */
static RT_MODEL_rmod_T rmod_M_;
RT_MODEL_rmod_T *const rmod_M = &rmod_M_;

/* Forward declaration for local functions */
static real_T rmod_wt_read_lin(const real_T tbl[4096], real_T L, real_T ph);
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

static real_T rmod_wt_read_lin(const real_T tbl[4096], real_T L, real_T ph)
{
  real_T idx;
  real_T k;
  real_T p1;
  real_T p2;
  if (ph >= 1.0) {
    /* Start for MATLABSystem: '<Root>/VCO' */
    ph -= floor(ph);
  }

  idx = ph * L;

  /* Start for MATLABSystem: '<Root>/VCO' */
  k = floor(idx);
  p1 = k + 1.0;

  /* Start for MATLABSystem: '<Root>/VCO' */
  if (k + 1.0 > L) {
    p1 = 1.0;
  }

  p2 = p1 + 1.0;

  /* Start for MATLABSystem: '<Root>/VCO' */
  if (p1 + 1.0 > L) {
    p2 = 1.0;
  }

  p1 = tbl[(int32_T)p1 - 1];

  /* Start for MATLABSystem: '<Root>/VCO' */
  return (tbl[(int32_T)p2 - 1] - p1) * (idx - k) + p1;
}

/* Model step function */
void rmod_step(void)
{
  real_T rtb_Gain[128];
  real_T rtb_Gain1[128];
  real_T rtb_VCA2_o1[128];
  real_T rtb_gain[128];
  real_T L;
  real_T g;
  real_T inc;
  real_T ph;
  real_T q;
  real_T tmp;
  real_T vL;
  int32_T i;
  uint8_T ws;
  boolean_T m;

  /* S-Function (ctrl_in): '<Root>/Control Input' */

  /* Level2 S-Function Block: '<Root>/Control Input' (ctrl_in) */
  {
    SimStruct *rts = rmod_M->childSfunctions[0];
    sfcnOutputs(rts,0);
  }

  /* S-Function (pisound_in): '<Root>/PiSound Input' */

  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = rmod_M->childSfunctions[1];
    sfcnOutputs(rts,0);
  }

  for (i = 0; i < 128; i++) {
    /* Gain: '<Root>/Gain' incorporates:
     *  DataTypeConversion: '<Root>/Data Type Conversion'
     */
    rtb_Gain[i] = rmod_P.Gain_Gain * (real_T)rmod_B.PiSoundInput_o1[i];

    /* Gain: '<Root>/Gain1' incorporates:
     *  DataTypeConversion: '<Root>/Data Type Conversion1'
     */
    rtb_Gain1[i] = rmod_P.Gain1_Gain * (real_T)rmod_B.PiSoundInput_o2[i];
  }

  /* MATLABSystem: '<Root>/VCA1' incorporates:
   *  Constant: '<Root>/Constant1'
   */
  g = rmod_B.ControlInput_o3;

  /*  g es Nx1 */
  if (rmod_P.Constant1_Value != 0) {
    g = rt_powd_snf(10.0, rmod_B.ControlInput_o3 / 20.0);

    /*  dB -> lineal, elemento a elemento */
  }

  /* MATLABSystem: '<Root>/VCO' incorporates:
   *  Constant: '<Root>/Constant10'
   *  Constant: '<Root>/Constant11'
   *  Constant: '<Root>/Constant12'
   */
  /*  bp es logical */
  /*  elemento a elemento */
  /*  --- I/O */
  /*  --- WaveSel seguro (NO double->uint8) */
  ws = rmod_P.WaveSel_rt;
  if (rmod_P.WaveSel_rt < 1) {
    ws = 1U;
  } else if (rmod_P.WaveSel_rt > 10) {
    ws = 10U;
  }

  L = rmod_DW.obj.TablesSize[ws - 1];
  if (L < 2.0) {
    L = 2.0;
  }

  inc = rmod_B.ControlInput_o2 / 48000.0;
  ph = rmod_DW.obj.Phase;
  if (rtIsNaN(rmod_P.Constant11_Value) || rtIsInf(rmod_P.Constant11_Value)) {
    q = (rtNaN);
  } else {
    q = fabs(rmod_P.Constant11_Value / 6.2831853071795862);
    if (fabs(q - floor(q + 0.5)) > 2.2204460492503131E-16 * q) {
      q = fmod(rmod_P.Constant11_Value, 6.2831853071795862);
    } else {
      q = 0.0;
    }

    if (q == 0.0) {
      q = 0.0;
    } else if (q < 0.0) {
      q += 6.2831853071795862;
    }
  }

  q /= 6.2831853071795862;

  /*  cycles */
  m = (rmod_P.Constant12_Value != 0);
  for (i = 0; i < 128; i++) {
    vL = rmod_wt_read_lin(&rmod_DW.obj.WaveTables[(ws - 1) << 12], L, ph + q);
    if (m) {
      /*  unipolar 0..1 */
      vL = (vL + 1.0) * 0.5;
    } else {
      /*  bipolar */
      /*  v = v */
    }

    ph += inc;
    if (ph >= 1.0) {
      ph--;
    }

    /* MATLAB Function: '<Root>/MATLAB Function' incorporates:
     *  Constant: '<Root>/Constant8'
     */
    rtb_gain[i] = rmod_P.Constant8_Value * vL * rmod_B.ControlInput_o1;
  }

  rmod_DW.obj.Phase = ph;

  /* End of MATLABSystem: '<Root>/VCO' */

  /* MATLABSystem: '<Root>/VCA2' incorporates:
   *  Constant: '<Root>/Constant1'
   *  MATLABSystem: '<Root>/VCA1'
   * */
  /*  g es Nx1 */
  if (rmod_P.Constant1_Value != 0) {
    for (i = 0; i < 128; i++) {
      rtb_gain[i] = rt_powd_snf(10.0, rtb_gain[i] / 20.0);
    }

    /*  dB -> lineal, elemento a elemento */
  }

  /*  bp es logical */
  /*  elemento a elemento */
  for (i = 0; i < 128; i++) {
    L = rtb_gain[i];
    rtb_VCA2_o1[i] = rtb_Gain[i] * g * L;

    /* MATLABSystem: '<Root>/VCA2' incorporates:
     *  MATLABSystem: '<Root>/VCA1'
     * */
    rtb_gain[i] = rtb_Gain1[i] * g * L;
  }

  /* End of MATLABSystem: '<Root>/VCA2' */

  /* MATLABSystem: '<Root>/MIX' */
  if (rmod_DW.obj_a.SmoothTime != rmod_P.MIX_SmoothTime) {
    rmod_DW.obj_a.SmoothTime = rmod_P.MIX_SmoothTime;
  }

  if (rmod_DW.obj_a.IndepInDB != rmod_P.MIX_IndepInDB) {
    rmod_DW.obj_a.IndepInDB = rmod_P.MIX_IndepInDB;
  }

  /*  inL,inR,wetL,wetR,p1 (+p2 si Mode=2) */
  /*  Mode 3: crossfade por muestra (p1 puede ser [N x 1]) */
  /*  dummy */
  g = fmin(fmax(rmod_B.ControlInput_o4, 0.0), 1.0);
  L = rmod_DW.obj_a.a;
  inc = rmod_DW.obj_a.gd1;
  ph = rmod_DW.obj_a.gd2;
  q = rmod_DW.obj_a.gw1;
  vL = rmod_DW.obj_a.gw2;

  /*  más robusto que obj.N */
  for (i = 0; i < 128; i++) {
    inc += ((1.0 - g) - inc) * L;
    q += (g - q) * L;
    ph += ((1.0 - g) - ph) * L;
    vL += (g - vL) * L;

    /* DataTypeConversion: '<Root>/Data Type Conversion2' incorporates:
     *  Gain: '<Root>/Gain2'
     *  MATLABSystem: '<Root>/VCA2'
     */
    tmp = floor((inc * rtb_Gain[i] + q * rtb_VCA2_o1[i]) * rmod_P.Gain2_Gain);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion2' */
    rmod_B.DataTypeConversion2[i] = tmp < 0.0 ? -(int32_T)(uint32_T)-tmp :
      (int32_T)(uint32_T)tmp;

    /* DataTypeConversion: '<Root>/Data Type Conversion3' incorporates:
     *  Gain: '<Root>/Gain3'
     *  MATLABSystem: '<Root>/VCA2'
     */
    tmp = floor((ph * rtb_Gain1[i] + vL * rtb_gain[i]) * rmod_P.Gain3_Gain);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion3' */
    rmod_B.DataTypeConversion3[i] = tmp < 0.0 ? -(int32_T)(uint32_T)-tmp :
      (int32_T)(uint32_T)tmp;
  }

  rmod_DW.obj_a.gd1 = inc;
  rmod_DW.obj_a.gd2 = ph;
  rmod_DW.obj_a.gw1 = q;
  rmod_DW.obj_a.gw2 = vL;

  /* End of MATLABSystem: '<Root>/MIX' */

  /* S-Function (pisound_out): '<Root>/PiSound Output' */

  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = rmod_M->childSfunctions[2];
    sfcnOutputs(rts,0);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  rmod_M->Timing.t[0] =
    ((time_T)(++rmod_M->Timing.clockTick0)) * rmod_M->Timing.stepSize0;
}

/* Model initialize function */
void rmod_initialize(void)
{
  /* Registration code */
  rtsiSetSolverName(&rmod_M->solverInfo,"FixedStepDiscrete");
  rmod_M->solverInfoPtr = (&rmod_M->solverInfo);

  /* Initialize timing info */
  {
    int_T *mdlTsMap = rmod_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    rmod_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    rmod_M->Timing.sampleTimes = (&rmod_M->Timing.sampleTimesArray[0]);
    rmod_M->Timing.offsetTimes = (&rmod_M->Timing.offsetTimesArray[0]);

    /* task periods */
    rmod_M->Timing.sampleTimes[0] = (0.0026666666666666666);

    /* task offsets */
    rmod_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(rmod_M, &rmod_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = rmod_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    rmod_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(rmod_M, -1);
  rmod_M->Timing.stepSize0 = 0.0026666666666666666;
  rmod_M->solverInfoPtr = (&rmod_M->solverInfo);
  rmod_M->Timing.stepSize = (0.0026666666666666666);
  rtsiSetFixedStepSize(&rmod_M->solverInfo, 0.0026666666666666666);
  rtsiSetSolverMode(&rmod_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* child S-Function registration */
  {
    RTWSfcnInfo *sfcnInfo = &rmod_M->NonInlinedSFcns.sfcnInfo;
    rmod_M->sfcnInfo = (sfcnInfo);
    rtssSetErrorStatusPtr(sfcnInfo, (&rtmGetErrorStatus(rmod_M)));
    rmod_M->Sizes.numSampTimes = (1);
    rtssSetNumRootSampTimesPtr(sfcnInfo, &rmod_M->Sizes.numSampTimes);
    rmod_M->NonInlinedSFcns.taskTimePtrs[0] = (&rtmGetTPtr(rmod_M)[0]);
    rtssSetTPtrPtr(sfcnInfo,rmod_M->NonInlinedSFcns.taskTimePtrs);
    rtssSetTStartPtr(sfcnInfo, &rtmGetTStart(rmod_M));
    rtssSetTFinalPtr(sfcnInfo, &rtmGetTFinal(rmod_M));
    rtssSetTimeOfLastOutputPtr(sfcnInfo, &rtmGetTimeOfLastOutput(rmod_M));
    rtssSetStepSizePtr(sfcnInfo, &rmod_M->Timing.stepSize);
    rtssSetStopRequestedPtr(sfcnInfo, &rtmGetStopRequested(rmod_M));
    rtssSetDerivCacheNeedsResetPtr(sfcnInfo, &rmod_M->derivCacheNeedsReset);
    rtssSetZCCacheNeedsResetPtr(sfcnInfo, &rmod_M->zCCacheNeedsReset);
    rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(sfcnInfo,
      &rmod_M->CTOutputIncnstWithState);
    rtssSetSampleHitsPtr(sfcnInfo, &rmod_M->Timing.sampleHits);
    rtssSetPerTaskSampleHitsPtr(sfcnInfo, &rmod_M->Timing.perTaskSampleHits);
    rtssSetSimModePtr(sfcnInfo, &rmod_M->simMode);
    rtssSetSolverInfoPtr(sfcnInfo, &rmod_M->solverInfoPtr);
  }

  rmod_M->Sizes.numSFcns = (3);

  /* register each child */
  {
    (void) memset((void *)&rmod_M->NonInlinedSFcns.childSFunctions[0], 0,
                  3*sizeof(SimStruct));
    rmod_M->childSfunctions = (&rmod_M->NonInlinedSFcns.childSFunctionPtrs[0]);
    rmod_M->childSfunctions[0] = (&rmod_M->NonInlinedSFcns.childSFunctions[0]);
    rmod_M->childSfunctions[1] = (&rmod_M->NonInlinedSFcns.childSFunctions[1]);
    rmod_M->childSfunctions[2] = (&rmod_M->NonInlinedSFcns.childSFunctions[2]);

    /* Level2 S-Function Block: rmod/<Root>/Control Input (ctrl_in) */
    {
      SimStruct *rts = rmod_M->childSfunctions[0];

      /* timing info */
      time_T *sfcnPeriod = rmod_M->NonInlinedSFcns.Sfcn0.sfcnPeriod;
      time_T *sfcnOffset = rmod_M->NonInlinedSFcns.Sfcn0.sfcnOffset;
      int_T *sfcnTsMap = rmod_M->NonInlinedSFcns.Sfcn0.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &rmod_M->NonInlinedSFcns.blkInfo2[0]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &rmod_M->NonInlinedSFcns.inputOutputPortInfo2[0]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, rmod_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &rmod_M->NonInlinedSFcns.methods2[0]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &rmod_M->NonInlinedSFcns.methods3[0]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &rmod_M->NonInlinedSFcns.methods4[0]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &rmod_M->NonInlinedSFcns.statesInfo2[0]);
        ssSetPeriodicStatesInfo(rts, &rmod_M->
          NonInlinedSFcns.periodicStatesInfo[0]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &rmod_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &rmod_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 4);
        _ssSetPortInfo2ForOutputUnits(rts,
          &rmod_M->NonInlinedSFcns.Sfcn0.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        ssSetOutputPortUnit(rts, 2, 0);
        ssSetOutputPortUnit(rts, 3, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &rmod_M->NonInlinedSFcns.Sfcn0.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 2, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 3, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 1);
          ssSetOutputPortSignal(rts, 0, ((real_T *) &rmod_B.ControlInput_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 1);
          ssSetOutputPortSignal(rts, 1, ((real_T *) &rmod_B.ControlInput_o2));
        }

        /* port 2 */
        {
          _ssSetOutputPortNumDimensions(rts, 2, 1);
          ssSetOutputPortWidthAsInt(rts, 2, 1);
          ssSetOutputPortSignal(rts, 2, ((real_T *) &rmod_B.ControlInput_o3));
        }

        /* port 3 */
        {
          _ssSetOutputPortNumDimensions(rts, 3, 1);
          ssSetOutputPortWidthAsInt(rts, 3, 1);
          ssSetOutputPortSignal(rts, 3, ((real_T *) &rmod_B.ControlInput_o4));
        }
      }

      /* path info */
      ssSetModelName(rts, "Control Input");
      ssSetPath(rts, "rmod/Control Input");
      ssSetRTModel(rts,rmod_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &rmod_M->NonInlinedSFcns.Sfcn0.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)rmod_P.ControlInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)rmod_P.ControlInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)rmod_P.ControlInput_P3_Size);
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
      _ssSetOutputPortBeingMerged(rts, 0, 0);
      _ssSetOutputPortBeingMerged(rts, 1, 0);
      _ssSetOutputPortBeingMerged(rts, 2, 0);
      _ssSetOutputPortBeingMerged(rts, 3, 0);

      /* Update the BufferDstPort flags for each input port */
    }

    /* Level2 S-Function Block: rmod/<Root>/PiSound Input (pisound_in) */
    {
      SimStruct *rts = rmod_M->childSfunctions[1];

      /* timing info */
      time_T *sfcnPeriod = rmod_M->NonInlinedSFcns.Sfcn1.sfcnPeriod;
      time_T *sfcnOffset = rmod_M->NonInlinedSFcns.Sfcn1.sfcnOffset;
      int_T *sfcnTsMap = rmod_M->NonInlinedSFcns.Sfcn1.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &rmod_M->NonInlinedSFcns.blkInfo2[1]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &rmod_M->NonInlinedSFcns.inputOutputPortInfo2[1]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, rmod_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &rmod_M->NonInlinedSFcns.methods2[1]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &rmod_M->NonInlinedSFcns.methods3[1]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &rmod_M->NonInlinedSFcns.methods4[1]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &rmod_M->NonInlinedSFcns.statesInfo2[1]);
        ssSetPeriodicStatesInfo(rts, &rmod_M->
          NonInlinedSFcns.periodicStatesInfo[1]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &rmod_M->NonInlinedSFcns.Sfcn1.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &rmod_M->NonInlinedSFcns.Sfcn1.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &rmod_M->NonInlinedSFcns.Sfcn1.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &rmod_M->NonInlinedSFcns.Sfcn1.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 128);
          ssSetOutputPortSignal(rts, 0, ((int32_T *) rmod_B.PiSoundInput_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 128);
          ssSetOutputPortSignal(rts, 1, ((int32_T *) rmod_B.PiSoundInput_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "PiSound Input");
      ssSetPath(rts, "rmod/PiSound Input");
      ssSetRTModel(rts,rmod_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &rmod_M->NonInlinedSFcns.Sfcn1.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)rmod_P.PiSoundInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)rmod_P.PiSoundInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)rmod_P.PiSoundInput_P3_Size);
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

    /* Level2 S-Function Block: rmod/<Root>/PiSound Output (pisound_out) */
    {
      SimStruct *rts = rmod_M->childSfunctions[2];

      /* timing info */
      time_T *sfcnPeriod = rmod_M->NonInlinedSFcns.Sfcn2.sfcnPeriod;
      time_T *sfcnOffset = rmod_M->NonInlinedSFcns.Sfcn2.sfcnOffset;
      int_T *sfcnTsMap = rmod_M->NonInlinedSFcns.Sfcn2.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &rmod_M->NonInlinedSFcns.blkInfo2[2]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &rmod_M->NonInlinedSFcns.inputOutputPortInfo2[2]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, rmod_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &rmod_M->NonInlinedSFcns.methods2[2]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &rmod_M->NonInlinedSFcns.methods3[2]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &rmod_M->NonInlinedSFcns.methods4[2]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &rmod_M->NonInlinedSFcns.statesInfo2[2]);
        ssSetPeriodicStatesInfo(rts, &rmod_M->
          NonInlinedSFcns.periodicStatesInfo[2]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts, &rmod_M->
          NonInlinedSFcns.Sfcn2.inputPortInfo[0]);
        ssSetPortInfoForInputs(rts, &rmod_M->
          NonInlinedSFcns.Sfcn2.inputPortInfo[0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &rmod_M->NonInlinedSFcns.Sfcn2.inputPortUnits[0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &rmod_M->NonInlinedSFcns.Sfcn2.inputPortCoSimAttribute[0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          int32_T const **sfcnUPtrs = (int32_T const **)
            &rmod_M->NonInlinedSFcns.Sfcn2.UPtrs0;

          {
            int_T i1;
            const int32_T *u0 = rmod_B.DataTypeConversion2;
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
            &rmod_M->NonInlinedSFcns.Sfcn2.UPtrs1;

          {
            int_T i1;
            const int32_T *u1 = rmod_B.DataTypeConversion3;
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
      ssSetPath(rts, "rmod/PiSound Output");
      ssSetRTModel(rts,rmod_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &rmod_M->NonInlinedSFcns.Sfcn2.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)rmod_P.PiSoundOutput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)rmod_P.PiSoundOutput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)rmod_P.PiSoundOutput_P3_Size);
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
    real_T a;
    real_T a_0;
    int32_T b_k;
    int32_T b_k_0;
    static const int8_T tmp[1024] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

    static const real_T tmp_0[1024] = { -1.0, -0.99609375, -0.9921875,
      -0.98828125, -0.984375, -0.98046875, -0.9765625, -0.97265625, -0.96875,
      -0.96484375, -0.9609375, -0.95703125, -0.953125, -0.94921875, -0.9453125,
      -0.94140625, -0.9375, -0.93359375, -0.9296875, -0.92578125, -0.921875,
      -0.91796875, -0.9140625, -0.91015625, -0.90625, -0.90234375, -0.8984375,
      -0.89453125, -0.890625, -0.88671875, -0.8828125, -0.87890625, -0.875,
      -0.87109375, -0.8671875, -0.86328125, -0.859375, -0.85546875, -0.8515625,
      -0.84765625, -0.84375, -0.83984375, -0.8359375, -0.83203125, -0.828125,
      -0.82421875, -0.8203125, -0.81640625, -0.8125, -0.80859375, -0.8046875,
      -0.80078125, -0.796875, -0.79296875, -0.7890625, -0.78515625, -0.78125,
      -0.77734375, -0.7734375, -0.76953125, -0.765625, -0.76171875, -0.7578125,
      -0.75390625, -0.75, -0.74609375, -0.7421875, -0.73828125, -0.734375,
      -0.73046875, -0.7265625, -0.72265625, -0.71875, -0.71484375, -0.7109375,
      -0.70703125, -0.703125, -0.69921875, -0.6953125, -0.69140625, -0.6875,
      -0.68359375, -0.6796875, -0.67578125, -0.671875, -0.66796875, -0.6640625,
      -0.66015625, -0.65625, -0.65234375, -0.6484375, -0.64453125, -0.640625,
      -0.63671875, -0.6328125, -0.62890625, -0.625, -0.62109375, -0.6171875,
      -0.61328125, -0.609375, -0.60546875, -0.6015625, -0.59765625, -0.59375,
      -0.58984375, -0.5859375, -0.58203125, -0.578125, -0.57421875, -0.5703125,
      -0.56640625, -0.5625, -0.55859375, -0.5546875, -0.55078125, -0.546875,
      -0.54296875, -0.5390625, -0.53515625, -0.53125, -0.52734375, -0.5234375,
      -0.51953125, -0.515625, -0.51171875, -0.5078125, -0.50390625, -0.5,
      -0.49609375, -0.4921875, -0.48828125, -0.484375, -0.48046875, -0.4765625,
      -0.47265625, -0.46875, -0.46484375, -0.4609375, -0.45703125, -0.453125,
      -0.44921875, -0.4453125, -0.44140625, -0.4375, -0.43359375, -0.4296875,
      -0.42578125, -0.421875, -0.41796875, -0.4140625, -0.41015625, -0.40625,
      -0.40234375, -0.3984375, -0.39453125, -0.390625, -0.38671875, -0.3828125,
      -0.37890625, -0.375, -0.37109375, -0.3671875, -0.36328125, -0.359375,
      -0.35546875, -0.3515625, -0.34765625, -0.34375, -0.33984375, -0.3359375,
      -0.33203125, -0.328125, -0.32421875, -0.3203125, -0.31640625, -0.3125,
      -0.30859375, -0.3046875, -0.30078125, -0.296875, -0.29296875, -0.2890625,
      -0.28515625, -0.28125, -0.27734375, -0.2734375, -0.26953125, -0.265625,
      -0.26171875, -0.2578125, -0.25390625, -0.25, -0.24609375, -0.2421875,
      -0.23828125, -0.234375, -0.23046875, -0.2265625, -0.22265625, -0.21875,
      -0.21484375, -0.2109375, -0.20703125, -0.203125, -0.19921875, -0.1953125,
      -0.19140625, -0.1875, -0.18359375, -0.1796875, -0.17578125, -0.171875,
      -0.16796875, -0.1640625, -0.16015625, -0.15625, -0.15234375, -0.1484375,
      -0.14453125, -0.140625, -0.13671875, -0.1328125, -0.12890625, -0.125,
      -0.12109375, -0.1171875, -0.11328125, -0.109375, -0.10546875, -0.1015625,
      -0.09765625, -0.09375, -0.08984375, -0.0859375, -0.08203125, -0.078125,
      -0.07421875, -0.0703125, -0.06640625, -0.0625, -0.05859375, -0.0546875,
      -0.05078125, -0.046875, -0.04296875, -0.0390625, -0.03515625, -0.03125,
      -0.02734375, -0.0234375, -0.01953125, -0.015625, -0.01171875, -0.0078125,
      -0.00390625, 0.0, 0.00390625, 0.0078125, 0.01171875, 0.015625, 0.01953125,
      0.0234375, 0.02734375, 0.03125, 0.03515625, 0.0390625, 0.04296875,
      0.046875, 0.05078125, 0.0546875, 0.05859375, 0.0625, 0.06640625, 0.0703125,
      0.07421875, 0.078125, 0.08203125, 0.0859375, 0.08984375, 0.09375,
      0.09765625, 0.1015625, 0.10546875, 0.109375, 0.11328125, 0.1171875,
      0.12109375, 0.125, 0.12890625, 0.1328125, 0.13671875, 0.140625, 0.14453125,
      0.1484375, 0.15234375, 0.15625, 0.16015625, 0.1640625, 0.16796875,
      0.171875, 0.17578125, 0.1796875, 0.18359375, 0.1875, 0.19140625, 0.1953125,
      0.19921875, 0.203125, 0.20703125, 0.2109375, 0.21484375, 0.21875,
      0.22265625, 0.2265625, 0.23046875, 0.234375, 0.23828125, 0.2421875,
      0.24609375, 0.25, 0.25390625, 0.2578125, 0.26171875, 0.265625, 0.26953125,
      0.2734375, 0.27734375, 0.28125, 0.28515625, 0.2890625, 0.29296875,
      0.296875, 0.30078125, 0.3046875, 0.30859375, 0.3125, 0.31640625, 0.3203125,
      0.32421875, 0.328125, 0.33203125, 0.3359375, 0.33984375, 0.34375,
      0.34765625, 0.3515625, 0.35546875, 0.359375, 0.36328125, 0.3671875,
      0.37109375, 0.375, 0.37890625, 0.3828125, 0.38671875, 0.390625, 0.39453125,
      0.3984375, 0.40234375, 0.40625, 0.41015625, 0.4140625, 0.41796875,
      0.421875, 0.42578125, 0.4296875, 0.43359375, 0.4375, 0.44140625, 0.4453125,
      0.44921875, 0.453125, 0.45703125, 0.4609375, 0.46484375, 0.46875,
      0.47265625, 0.4765625, 0.48046875, 0.484375, 0.48828125, 0.4921875,
      0.49609375, 0.5, 0.50390625, 0.5078125, 0.51171875, 0.515625, 0.51953125,
      0.5234375, 0.52734375, 0.53125, 0.53515625, 0.5390625, 0.54296875,
      0.546875, 0.55078125, 0.5546875, 0.55859375, 0.5625, 0.56640625, 0.5703125,
      0.57421875, 0.578125, 0.58203125, 0.5859375, 0.58984375, 0.59375,
      0.59765625, 0.6015625, 0.60546875, 0.609375, 0.61328125, 0.6171875,
      0.62109375, 0.625, 0.62890625, 0.6328125, 0.63671875, 0.640625, 0.64453125,
      0.6484375, 0.65234375, 0.65625, 0.66015625, 0.6640625, 0.66796875,
      0.671875, 0.67578125, 0.6796875, 0.68359375, 0.6875, 0.69140625, 0.6953125,
      0.69921875, 0.703125, 0.70703125, 0.7109375, 0.71484375, 0.71875,
      0.72265625, 0.7265625, 0.73046875, 0.734375, 0.73828125, 0.7421875,
      0.74609375, 0.75, 0.75390625, 0.7578125, 0.76171875, 0.765625, 0.76953125,
      0.7734375, 0.77734375, 0.78125, 0.78515625, 0.7890625, 0.79296875,
      0.796875, 0.80078125, 0.8046875, 0.80859375, 0.8125, 0.81640625, 0.8203125,
      0.82421875, 0.828125, 0.83203125, 0.8359375, 0.83984375, 0.84375,
      0.84765625, 0.8515625, 0.85546875, 0.859375, 0.86328125, 0.8671875,
      0.87109375, 0.875, 0.87890625, 0.8828125, 0.88671875, 0.890625, 0.89453125,
      0.8984375, 0.90234375, 0.90625, 0.91015625, 0.9140625, 0.91796875,
      0.921875, 0.92578125, 0.9296875, 0.93359375, 0.9375, 0.94140625, 0.9453125,
      0.94921875, 0.953125, 0.95703125, 0.9609375, 0.96484375, 0.96875,
      0.97265625, 0.9765625, 0.98046875, 0.984375, 0.98828125, 0.9921875,
      0.99609375, 1.0, 0.99609375, 0.9921875, 0.98828125, 0.984375, 0.98046875,
      0.9765625, 0.97265625, 0.96875, 0.96484375, 0.9609375, 0.95703125,
      0.953125, 0.94921875, 0.9453125, 0.94140625, 0.9375, 0.93359375, 0.9296875,
      0.92578125, 0.921875, 0.91796875, 0.9140625, 0.91015625, 0.90625,
      0.90234375, 0.8984375, 0.89453125, 0.890625, 0.88671875, 0.8828125,
      0.87890625, 0.875, 0.87109375, 0.8671875, 0.86328125, 0.859375, 0.85546875,
      0.8515625, 0.84765625, 0.84375, 0.83984375, 0.8359375, 0.83203125,
      0.828125, 0.82421875, 0.8203125, 0.81640625, 0.8125, 0.80859375, 0.8046875,
      0.80078125, 0.796875, 0.79296875, 0.7890625, 0.78515625, 0.78125,
      0.77734375, 0.7734375, 0.76953125, 0.765625, 0.76171875, 0.7578125,
      0.75390625, 0.75, 0.74609375, 0.7421875, 0.73828125, 0.734375, 0.73046875,
      0.7265625, 0.72265625, 0.71875, 0.71484375, 0.7109375, 0.70703125,
      0.703125, 0.69921875, 0.6953125, 0.69140625, 0.6875, 0.68359375, 0.6796875,
      0.67578125, 0.671875, 0.66796875, 0.6640625, 0.66015625, 0.65625,
      0.65234375, 0.6484375, 0.64453125, 0.640625, 0.63671875, 0.6328125,
      0.62890625, 0.625, 0.62109375, 0.6171875, 0.61328125, 0.609375, 0.60546875,
      0.6015625, 0.59765625, 0.59375, 0.58984375, 0.5859375, 0.58203125,
      0.578125, 0.57421875, 0.5703125, 0.56640625, 0.5625, 0.55859375, 0.5546875,
      0.55078125, 0.546875, 0.54296875, 0.5390625, 0.53515625, 0.53125,
      0.52734375, 0.5234375, 0.51953125, 0.515625, 0.51171875, 0.5078125,
      0.50390625, 0.5, 0.49609375, 0.4921875, 0.48828125, 0.484375, 0.48046875,
      0.4765625, 0.47265625, 0.46875, 0.46484375, 0.4609375, 0.45703125,
      0.453125, 0.44921875, 0.4453125, 0.44140625, 0.4375, 0.43359375, 0.4296875,
      0.42578125, 0.421875, 0.41796875, 0.4140625, 0.41015625, 0.40625,
      0.40234375, 0.3984375, 0.39453125, 0.390625, 0.38671875, 0.3828125,
      0.37890625, 0.375, 0.37109375, 0.3671875, 0.36328125, 0.359375, 0.35546875,
      0.3515625, 0.34765625, 0.34375, 0.33984375, 0.3359375, 0.33203125,
      0.328125, 0.32421875, 0.3203125, 0.31640625, 0.3125, 0.30859375, 0.3046875,
      0.30078125, 0.296875, 0.29296875, 0.2890625, 0.28515625, 0.28125,
      0.27734375, 0.2734375, 0.26953125, 0.265625, 0.26171875, 0.2578125,
      0.25390625, 0.25, 0.24609375, 0.2421875, 0.23828125, 0.234375, 0.23046875,
      0.2265625, 0.22265625, 0.21875, 0.21484375, 0.2109375, 0.20703125,
      0.203125, 0.19921875, 0.1953125, 0.19140625, 0.1875, 0.18359375, 0.1796875,
      0.17578125, 0.171875, 0.16796875, 0.1640625, 0.16015625, 0.15625,
      0.15234375, 0.1484375, 0.14453125, 0.140625, 0.13671875, 0.1328125,
      0.12890625, 0.125, 0.12109375, 0.1171875, 0.11328125, 0.109375, 0.10546875,
      0.1015625, 0.09765625, 0.09375, 0.08984375, 0.0859375, 0.08203125,
      0.078125, 0.07421875, 0.0703125, 0.06640625, 0.0625, 0.05859375, 0.0546875,
      0.05078125, 0.046875, 0.04296875, 0.0390625, 0.03515625, 0.03125,
      0.02734375, 0.0234375, 0.01953125, 0.015625, 0.01171875, 0.0078125,
      0.00390625, 0.0, -0.00390625, -0.0078125, -0.01171875, -0.015625,
      -0.01953125, -0.0234375, -0.02734375, -0.03125, -0.03515625, -0.0390625,
      -0.04296875, -0.046875, -0.05078125, -0.0546875, -0.05859375, -0.0625,
      -0.06640625, -0.0703125, -0.07421875, -0.078125, -0.08203125, -0.0859375,
      -0.08984375, -0.09375, -0.09765625, -0.1015625, -0.10546875, -0.109375,
      -0.11328125, -0.1171875, -0.12109375, -0.125, -0.12890625, -0.1328125,
      -0.13671875, -0.140625, -0.14453125, -0.1484375, -0.15234375, -0.15625,
      -0.16015625, -0.1640625, -0.16796875, -0.171875, -0.17578125, -0.1796875,
      -0.18359375, -0.1875, -0.19140625, -0.1953125, -0.19921875, -0.203125,
      -0.20703125, -0.2109375, -0.21484375, -0.21875, -0.22265625, -0.2265625,
      -0.23046875, -0.234375, -0.23828125, -0.2421875, -0.24609375, -0.25,
      -0.25390625, -0.2578125, -0.26171875, -0.265625, -0.26953125, -0.2734375,
      -0.27734375, -0.28125, -0.28515625, -0.2890625, -0.29296875, -0.296875,
      -0.30078125, -0.3046875, -0.30859375, -0.3125, -0.31640625, -0.3203125,
      -0.32421875, -0.328125, -0.33203125, -0.3359375, -0.33984375, -0.34375,
      -0.34765625, -0.3515625, -0.35546875, -0.359375, -0.36328125, -0.3671875,
      -0.37109375, -0.375, -0.37890625, -0.3828125, -0.38671875, -0.390625,
      -0.39453125, -0.3984375, -0.40234375, -0.40625, -0.41015625, -0.4140625,
      -0.41796875, -0.421875, -0.42578125, -0.4296875, -0.43359375, -0.4375,
      -0.44140625, -0.4453125, -0.44921875, -0.453125, -0.45703125, -0.4609375,
      -0.46484375, -0.46875, -0.47265625, -0.4765625, -0.48046875, -0.484375,
      -0.48828125, -0.4921875, -0.49609375, -0.5, -0.50390625, -0.5078125,
      -0.51171875, -0.515625, -0.51953125, -0.5234375, -0.52734375, -0.53125,
      -0.53515625, -0.5390625, -0.54296875, -0.546875, -0.55078125, -0.5546875,
      -0.55859375, -0.5625, -0.56640625, -0.5703125, -0.57421875, -0.578125,
      -0.58203125, -0.5859375, -0.58984375, -0.59375, -0.59765625, -0.6015625,
      -0.60546875, -0.609375, -0.61328125, -0.6171875, -0.62109375, -0.625,
      -0.62890625, -0.6328125, -0.63671875, -0.640625, -0.64453125, -0.6484375,
      -0.65234375, -0.65625, -0.66015625, -0.6640625, -0.66796875, -0.671875,
      -0.67578125, -0.6796875, -0.68359375, -0.6875, -0.69140625, -0.6953125,
      -0.69921875, -0.703125, -0.70703125, -0.7109375, -0.71484375, -0.71875,
      -0.72265625, -0.7265625, -0.73046875, -0.734375, -0.73828125, -0.7421875,
      -0.74609375, -0.75, -0.75390625, -0.7578125, -0.76171875, -0.765625,
      -0.76953125, -0.7734375, -0.77734375, -0.78125, -0.78515625, -0.7890625,
      -0.79296875, -0.796875, -0.80078125, -0.8046875, -0.80859375, -0.8125,
      -0.81640625, -0.8203125, -0.82421875, -0.828125, -0.83203125, -0.8359375,
      -0.83984375, -0.84375, -0.84765625, -0.8515625, -0.85546875, -0.859375,
      -0.86328125, -0.8671875, -0.87109375, -0.875, -0.87890625, -0.8828125,
      -0.88671875, -0.890625, -0.89453125, -0.8984375, -0.90234375, -0.90625,
      -0.91015625, -0.9140625, -0.91796875, -0.921875, -0.92578125, -0.9296875,
      -0.93359375, -0.9375, -0.94140625, -0.9453125, -0.94921875, -0.953125,
      -0.95703125, -0.9609375, -0.96484375, -0.96875, -0.97265625, -0.9765625,
      -0.98046875, -0.984375, -0.98828125, -0.9921875, -0.99609375 };

    static const real_T tmp_1[1024] = { 0.0, 0.03067418743179556,
      0.061315817409201, 0.091892394944719208, 0.12237154980431976,
      0.15272109843409762, 0.18290910534886382, 0.21290394380667088,
      0.242674355596131, 0.27218950976692768, 0.30141906013813091,
      0.33033320142378025, 0.35890272382067806, 0.3870990659094,
      0.41489436572616256, 0.442261509870342, 0.46917418052008608,
      0.49560690023656007, 0.52153507444587988, 0.54693503149666467,
      0.57178406020034345, 0.59606044477083808, 0.61974349708995957,
      0.64281358623475959, 0.66525216521312014, 0.68704179486400041,
      0.70816616488893624, 0.72861011199156422, 0.74835963511206871,
      0.7674019077534795, 0.78572528740664782, 0.80331932209043411,
      0.820174754033136, 0.8362835205304141, 0.85163875202390338,
      0.86623476745329575, 0.88006706694291548, 0.89313232189164116,
      0.90542836254244585, 0.91695416311479216, 0.927709824589616,
      0.9376965552426425, 0.94691664902728179, 0.95537346191333994,
      0.96307138629224143, 0.97001582356338312, 0.97621315501962791,
      0.98167071115279314, 0.98639673950229245, 0.99040037117186674,
      0.99369158614057862, 0.996281177494972, 0.99818071470951208,
      0.99940250610214143, 0.999959560591032, 0.99986554887738965,
      0.99913476417750891, 0.99778208262518941, 0.99582292346314383,
      0.99327320913916983, 0.99014932541964518, 0.98646808162937494,
      0.98224667112298081, 0.97750263208891719, 0.97225380878284484,
      0.966518313282522, 0.96031448785161044, 0.95366086799486716,
      0.94657614628212949, 0.93907913701333234, 0.93118874179153244,
      0.92292391606560786, 0.9143036366989471, 0.90534687061508368,
      0.89607254456589247, 0.886499516062647, 0.87664654550498755,
      0.86653226953767226, 0.85617517565989409, 0.84559357810698155,
      0.83480559501944562, 0.823829126909641, 0.812681836431753,
      0.8013811294564428, 0.78994413744727776, 0.77838770113205435,
      0.766728355458298, 0.7549823158186, 0.743165465528037, 0.731293344532708,
      0.71938113932543724, 0.70744367404190789, 0.69549540270793264,
      0.683550402606224, 0.67162236872889058, 0.65972460927997012,
      0.64787004219060318, 0.63607119260793676, 0.62434019131755758,
      0.61268877405814137, 0.60112828168608, 0.58966966114712838,
      0.57832346721154981, 0.56709986492885067, 0.55600863275797052,
      0.54505916632873819, 0.53426048279045846, 0.5236212257037326,
      0.51314967043195181, 0.50285372998937183, 0.49274096130326794,
      0.482818571848348, 0.47309342661239157, 0.46357205535296925,
      0.45426066010603433, 0.44516512290822868, 0.43629101369582807,
      0.42764359834442356, 0.41922784681462671, 0.41104844137036139,
      0.40310978483757459, 0.39541600887254663, 0.38797098221031856,
      0.38077831886513147, 0.37384138625616975, 0.36716331323329277,
      0.36074699797885096, 0.3545951157630971, 0.34871012653210387,
      0.34309428230851957, 0.33774963438687083, 0.33267804030652837,
      0.32788117058680538, 0.32336051521003595, 0.31911738983980037,
      0.31515294176280029, 0.3114681555441749, 0.30806385838733635,
      0.3049407251906382, 0.30209928329444846, 0.29953991691336657,
      0.29726287124953449, 0.29526825628411629, 0.2935560502451709,
      0.29212610275121803, 0.29097813763088509, 0.29011175542006168,
      0.28952643553900304, 0.28922153815282742, 0.28919630571979282,
      0.28944986423271174, 0.28998122415973943, 0.29078928109167512,
      0.29187281610377575, 0.29323049584090694, 0.294860872335663,
      0.29676238256987481, 0.29893334779067082, 0.30137197259298537,
      0.30407634378111265, 0.30704442902257029, 0.31027407530818296,
      0.31376300723292055, 0.317508825112594, 0.3215090029520925,
      0.32576088628135086, 0.33026168987574023, 0.33500849537804173,
      0.33999824883957419, 0.34522775819845453, 0.3506936907133219,
      0.35639257037116673, 0.36232077528819551, 0.36847453512289186,
      0.37484992852063664, 0.38144288060939546, 0.38824916056609005,
      0.39526437927334, 0.40248398708625044, 0.40990327172890351,
      0.41751735634011206, 0.4253211976878466, 0.43330958457155527,
      0.44147713643134323, 0.44981830218265678, 0.45832735929476853,
      0.46699841313090912, 0.47582539656742823, 0.48480206990879987,
      0.49392202111469752, 0.50317866635468034, 0.51256525090531546,
      0.52207485040376667, 0.53170037247102819, 0.54143455871707591,
      0.55126998713924646, 0.56119907492411591, 0.57121408166208421,
      0.5813071129827343, 0.59147012461783866, 0.6016949268976779,
      0.61197318968501568, 0.62229644774979387, 0.63265610658621385,
      0.64304344867248286, 0.65344964017207963, 0.66386573807390981,
      0.67428269776725458, 0.68469138104591032, 0.69508256453438144,
      0.70544694852748391, 0.715775166233167, 0.72605779340683718,
      0.73628535836394948, 0.74644835235611484, 0.75653724029449154,
      0.7665424718027547, 0.77645449258051491, 0.78626375605665733,
      0.79596073531072475, 0.80553593523916234, 0.81497990494201111,
      0.82428325030443561, 0.83343664674637585, 0.84243085211256308,
      0.85125671967418015, 0.85990521121257324, 0.86836741015462959,
      0.876634534728749, 0.8846979511097377, 0.89254918652045967,
      0.90017994225769971, 0.90758210660941285, 0.91474776763036447,
      0.92166922574312415, 0.92833900613143183, 0.93474987089314487,
      0.94089483092026849, 0.94676715747399676, 0.95236039342322354,
      0.95766836411563927, 0.96268518785130075, 0.96740528592943964,
      0.971823392240281, 0.97593456237473564, 0.97973418222604791,
      0.98321797605878791, 0.98638201402198278, 0.98922271908468007,
      0.99173687337382332, 0.99392162389597771, 0.99577448762618814,
      0.99729335594905189, 0.998476498438955, 0.99932256596833791,
      0.99983059313481859, 1.0, 0.99983059313481859, 0.999322565968338,
      0.998476498438955, 0.99729335594905189, 0.99577448762618814,
      0.99392162389597771, 0.99173687337382332, 0.98922271908468018,
      0.98638201402198278, 0.983217976058788, 0.97973418222604791,
      0.97593456237473564, 0.97182339224028114, 0.96740528592943975,
      0.96268518785130086, 0.95766836411563938, 0.95236039342322354,
      0.94676715747399687, 0.9408948309202686, 0.934749870893145,
      0.92833900613143194, 0.92166922574312427, 0.91474776763036458,
      0.907582106609413, 0.90017994225769982, 0.89254918652045978,
      0.88469795110973781, 0.87663453472874919, 0.86836741015462959,
      0.85990521121257335, 0.85125671967418048, 0.84243085211256319,
      0.83343664674637608, 0.82428325030443572, 0.81497990494201122,
      0.80553593523916245, 0.79596073531072487, 0.78626375605665766,
      0.77645449258051513, 0.766542471802755, 0.75653724029449188,
      0.74644835235611506, 0.73628535836394948, 0.7260577934068374,
      0.71577516623316717, 0.70544694852748413, 0.69508256453438155,
      0.68469138104591054, 0.67428269776725469, 0.66386573807390969,
      0.65344964017207985, 0.643043448672483, 0.63265610658621374,
      0.6222964477497942, 0.61197318968501579, 0.6016949268976779,
      0.59147012461783877, 0.58130711298273419, 0.57121408166208465,
      0.56119907492411625, 0.55126998713924691, 0.54143455871707613,
      0.53170037247102808, 0.52207485040376689, 0.51256525090531579,
      0.50317866635468045, 0.49392202111469757, 0.48480206990879993,
      0.47582539656742834, 0.46699841313090956, 0.45832735929476864,
      0.44981830218265745, 0.44147713643134351, 0.433309584571556,
      0.42532119768784665, 0.41751735634011194, 0.40990327172890356,
      0.40248398708625016, 0.39526437927334029, 0.38824916056609016,
      0.38144288060939557, 0.374849928520637, 0.36847453512289235,
      0.36232077528819562, 0.35639257037116706, 0.35069369071332196,
      0.34522775819845442, 0.33999824883957408, 0.33500849537804162,
      0.33026168987574012, 0.32576088628135075, 0.321509002952093,
      0.31750882511259426, 0.31376300723292089, 0.31027407530818329,
      0.30704442902257018, 0.30407634378111253, 0.30137197259298504,
      0.29893334779067066, 0.29676238256987492, 0.2948608723356631,
      0.29323049584090682, 0.29187281610377558, 0.290789281091675,
      0.28998122415973954, 0.28944986423271185, 0.28919630571979249,
      0.28922153815282708, 0.28952643553900337, 0.29011175542006157,
      0.2909781376308852, 0.29212610275121809, 0.29355605024517095,
      0.29526825628411618, 0.29726287124953432, 0.29953991691336646,
      0.30209928329444813, 0.30494072519063808, 0.30806385838733608,
      0.31146815554417517, 0.31515294176279995, 0.31911738983980026,
      0.32336051521003562, 0.32788117058680527, 0.3326780403065282,
      0.33774963438687094, 0.34309428230851929, 0.34871012653210376,
      0.35459511576309721, 0.36074699797885107, 0.36716331323329288,
      0.37384138625616964, 0.38077831886513153, 0.38797098221031845,
      0.39541600887254674, 0.40310978483757404, 0.41104844137036112,
      0.41922784681462621, 0.42764359834442328, 0.43629101369582834,
      0.44516512290822835, 0.45426066010603439, 0.46357205535296875,
      0.47309342661239145, 0.48281857184834748, 0.49274096130326805,
      0.50285372998937117, 0.51314967043195148, 0.52362122570373215,
      0.53426048279045835, 0.54505916632873841, 0.55600863275797019,
      0.56709986492885056, 0.57832346721154948, 0.58966966114712827,
      0.60112828168607957, 0.61268877405814148, 0.62434019131755714,
      0.63607119260793654, 0.64787004219060362, 0.65972460927997,
      0.6716223687288907, 0.68355040260622379, 0.69549540270793275,
      0.70744367404190756, 0.71938113932543724, 0.73129334453270745,
      0.74316546552803686, 0.75498231581859931, 0.76672835545829787,
      0.77838770113205469, 0.78994413744727754, 0.8013811294564428,
      0.81268183643175251, 0.82382912690964094, 0.83480559501944518,
      0.84559357810698155, 0.85617517565989376, 0.866532269537672,
      0.87664654550498711, 0.88649951606264676, 0.89607254456589269,
      0.90534687061508334, 0.9143036366989471, 0.92292391606560764,
      0.93118874179153244, 0.939079137013332, 0.94657614628212949,
      0.95366086799486682, 0.96031448785161033, 0.96651831328252169,
      0.97225380878284473, 0.9775026320889173, 0.9822466711229807,
      0.98646808162937494, 0.990149325419645, 0.99327320913916983,
      0.99582292346314372, 0.99778208262518941, 0.99913476417750891,
      0.99986554887738965, 0.999959560591032, 0.99940250610214143,
      0.998180714709512, 0.996281177494972, 0.99369158614057851,
      0.99040037117186686, 0.98639673950229245, 0.98167071115279336,
      0.976213155019628, 0.97001582356338334, 0.96307138629224154,
      0.95537346191334038, 0.946916649027282, 0.93769655524264239,
      0.92770982458961626, 0.916954163114792, 0.9054283625424463,
      0.89313232189164127, 0.880067066942916, 0.866234767453296,
      0.85163875202390416, 0.83628352053041421, 0.820174754033137,
      0.80331932209043455, 0.78572528740664771, 0.76740190775348,
      0.7483596351120686, 0.72861011199156522, 0.708166164888936,
      0.68704179486400141, 0.66525216521312047, 0.6428135862347607,
      0.61974349708996013, 0.59606044477083775, 0.57178406020034422,
      0.54693503149666456, 0.52153507444588065, 0.49560690023655996,
      0.46917418052008697, 0.44226150987034207, 0.41489436572616373,
      0.38709906590940035, 0.35890272382067956, 0.33033320142378092,
      0.30141906013813041, 0.27218950976692852, 0.2426743555961306,
      0.21290394380667171, 0.18290910534886365, 0.15272109843409873,
      0.12237154980431997, 0.091892394944720623, 0.061315817409201542,
      0.0306741874317972, 5.66553889764798E-16, -0.030674187431796066,
      -0.061315817409199967, -0.0918923949447195, -0.12237154980431884,
      -0.15272109843409762, -0.18290910534886254, -0.2129039438066706,
      -0.24267435559612952, -0.27218950976692746, -0.30141906013812936,
      -0.33033320142377981, -0.3589027238206785, -0.3870990659093993,
      -0.41489436572616267, -0.442261509870341, -0.46917418052008597,
      -0.49560690023655896, -0.52153507444587965, -0.54693503149666367,
      -0.57178406020034322, -0.59606044477083686, -0.61974349708995891,
      -0.64281358623475993, -0.66525216521311958, -0.6870417948640003,
      -0.70816616488893525, -0.72861011199156411, -0.74835963511206782,
      -0.76740190775347961, -0.78572528740664671, -0.80331932209043388,
      -0.82017475403313633, -0.83628352053041366, -0.85163875202390349,
      -0.86623476745329542, -0.88006706694291548, -0.89313232189164071,
      -0.90542836254244574, -0.91695416311479183, -0.92770982458961582,
      -0.93769655524264217, -0.94691664902728157, -0.95537346191334,
      -0.96307138629224132, -0.97001582356338312, -0.97621315501962769,
      -0.98167071115279314, -0.98639673950229234, -0.99040037117186663,
      -0.9936915861405784, -0.99628117749497191, -0.998180714709512,
      -0.99940250610214143, -0.999959560591032, -0.99986554887738965,
      -0.99913476417750891, -0.99778208262518941, -0.99582292346314383,
      -0.99327320913916983, -0.99014932541964518, -0.986468081629375,
      -0.98224667112298092, -0.97750263208891752, -0.972253808782845,
      -0.96651831328252191, -0.96031448785161067, -0.953660867994867,
      -0.9465761462821296, -0.93907913701333245, -0.93118874179153266,
      -0.922923916065608, -0.91430363669894743, -0.90534687061508379,
      -0.89607254456589291, -0.886499516062647, -0.87664654550498766,
      -0.86653226953767271, -0.85617517565989409, -0.8455935781069821,
      -0.8348055950194454, -0.82382912690964161, -0.81268183643175307,
      -0.80138112945644335, -0.78994413744727809, -0.778387701132055,
      -0.766728355458298, -0.75498231581860009, -0.74316546552803708,
      -0.731293344532708, -0.7193811393254379, -0.707443674041908,
      -0.69549540270793342, -0.68355040260622446, -0.67162236872889136,
      -0.65972460927997023, -0.647870042190604, -0.63607119260793687,
      -0.62434019131755769, -0.61268877405814215, -0.60112828168607968,
      -0.58966966114712849, -0.57832346721155026, -0.56709986492885078,
      -0.55600863275797063, -0.545059166328739, -0.53426048279045857,
      -0.52362122570373271, -0.51314967043195192, -0.50285372998937117,
      -0.49274096130326844, -0.4828185718483477, -0.47309342661239207,
      -0.46357205535296897, -0.45426066010603483, -0.44516512290822879,
      -0.43629101369582857, -0.42764359834442367, -0.41922784681462683,
      -0.41104844137036156, -0.4031097848375747, -0.39541600887254719,
      -0.38797098221031867, -0.38077831886513158, -0.37384138625616947,
      -0.36716331323329332, -0.36074699797885107, -0.35459511576309766,
      -0.348710126532104, -0.34309428230851974, -0.33774963438687094,
      -0.33267804030652848, -0.3278811705868051, -0.32336051521003562,
      -0.31911738983980048, -0.3151529417628004, -0.31146815554417584,
      -0.30806385838733652, -0.3049407251906387, -0.30209928329444774,
      -0.29953991691336668, -0.2972628712495346, -0.2952682562841164,
      -0.293556050245171, -0.2921261027512177, -0.2909781376308852,
      -0.29011175542006179, -0.28952643553900315, -0.28922153815282753,
      -0.28919630571979293, -0.28944986423271185, -0.28998122415973909,
      -0.29078928109167529, -0.29187281610377586, -0.29323049584090666,
      -0.2948608723356631, -0.29676238256987492, -0.29893334779067049,
      -0.30137197259298465, -0.30407634378111192, -0.30704442902256956,
      -0.31027407530818307, -0.31376300723292067, -0.31750882511259493,
      -0.32150900295209262, -0.32576088628135014, -0.33026168987573995,
      -0.33500849537804139, -0.3399982488395743, -0.34522775819845425,
      -0.35069369071332118, -0.35639257037116728, -0.36232077528819523,
      -0.36847453512289113, -0.37484992852063675, -0.38144288060939557,
      -0.38824916056608932, -0.39526437927333929, -0.40248398708625055,
      -0.40990327172890278, -0.41751735634011178, -0.42532119768784588,
      -0.43330958457155538, -0.44147713643134295, -0.44981830218265612,
      -0.45832735929476903, -0.46699841313090962, -0.47582539656742756,
      -0.4848020699087992, -0.49392202111469763, -0.50317866635468012,
      -0.51256525090531491, -0.522074850403766, -0.5317003724710283,
      -0.54143455871707524, -0.55126998713924658, -0.5611990749241168,
      -0.571214081662085, -0.58130711298273363, -0.59147012461783777,
      -0.6016949268976779, -0.611973189685015, -0.6222964477497932,
      -0.632656106586214, -0.643043448672483, -0.65344964017207974,
      -0.66386573807390925, -0.67428269776725469, -0.68469138104591032,
      -0.69508256453438089, -0.7054469485274828, -0.71577516623316728,
      -0.726057793406837, -0.73628535836394893, -0.74644835235611551,
      -0.75653724029449165, -0.76654247180275481, -0.77645449258051447,
      -0.786263756056658, -0.79596073531072453, -0.80553593523916189,
      -0.81497990494201045, -0.82428325030443561, -0.83343664674637541,
      -0.84243085211256263, -0.85125671967418071, -0.85990521121257324,
      -0.86836741015462915, -0.87663453472874842, -0.88469795110973792,
      -0.89254918652045934, -0.90017994225769915, -0.90758210660941208,
      -0.91474776763036469, -0.92166922574312382, -0.92833900613143161,
      -0.9347498708931451, -0.9408948309202686, -0.94676715747399676,
      -0.95236039342322321, -0.9576683641156396, -0.96268518785130075,
      -0.96740528592943931, -0.97182339224028069, -0.97593456237473575,
      -0.9797341822260478, -0.9832179760587878, -0.98638201402198278,
      -0.98922271908468018, -0.99173687337382321, -0.9939216238959776,
      -0.99577448762618814, -0.99729335594905189, -0.99847649843895492,
      -0.99932256596833791, -0.99983059313481859, -1.0, -0.99983059313481859,
      -0.99932256596833791, -0.998476498438955, -0.997293355949052,
      -0.99577448762618825, -0.99392162389597771, -0.99173687337382344,
      -0.98922271908468029, -0.98638201402198311, -0.98321797605878791,
      -0.97973418222604791, -0.97593456237473586, -0.9718233922402808,
      -0.96740528592943975, -0.962685187851301, -0.95766836411564,
      -0.95236039342322343, -0.94676715747399687, -0.94089483092026882,
      -0.93474987089314554, -0.92833900613143183, -0.92166922574312438,
      -0.91474776763036481, -0.90758210660941274, -0.90017994225769971,
      -0.89254918652046, -0.88469795110973859, -0.87663453472874908,
      -0.86836741015462982, -0.859905211212574, -0.85125671967418137,
      -0.84243085211256286, -0.83343664674637619, -0.82428325030443639,
      -0.81497990494201067, -0.80553593523916223, -0.79596073531072487,
      -0.78626375605665833, -0.7764544925805148, -0.76654247180275514,
      -0.75653724029449254, -0.74644835235611584, -0.73628535836394926,
      -0.72605779340683763, -0.71577516623316761, -0.70544694852748369,
      -0.69508256453438122, -0.68469138104591076, -0.6742826977672558,
      -0.66386573807390958, -0.65344964017208007, -0.64304344867248342,
      -0.63265610658621474, -0.62229644774979431, -0.61197318968501613,
      -0.60169492689767834, -0.59147012461783821, -0.58130711298273408,
      -0.57121408166208509, -0.56119907492411725, -0.55126998713924691,
      -0.54143455871707635, -0.53170037247102875, -0.52207485040376644,
      -0.51256525090531568, -0.50317866635468056, -0.49392202111469807,
      -0.48480206990879965, -0.475825396567428, -0.46699841313090967,
      -0.45832735929476953, -0.44981830218265695, -0.4414771364313434,
      -0.43330958457155588, -0.42532119768784638, -0.41751735634011228,
      -0.40990327172890367, -0.40248398708625105, -0.39526437927333979,
      -0.38824916056609021, -0.38144288060939607, -0.3748499285206377,
      -0.368474535122892, -0.36232077528819567, -0.35639257037116734,
      -0.35069369071332168, -0.34522775819845469, -0.33999824883957397,
      -0.33500849537804189, -0.33026168987574, -0.32576088628135064,
      -0.32150900295209311, -0.31750882511259459, -0.31376300723292078,
      -0.31027407530818357, -0.30704442902257006, -0.30407634378111242,
      -0.30137197259298515, -0.298933347790671, -0.29676238256987497,
      -0.29486087233566277, -0.2932304958409071, -0.29187281610377547,
      -0.2907892810916749, -0.28998122415973959, -0.28944986423271191,
      -0.2891963057197926, -0.2892215381528272, -0.2895264355390032,
      -0.29011175542006146, -0.29097813763088487, -0.2921261027512182,
      -0.29355605024517067, -0.29526825628411607, -0.29726287124953421,
      -0.29953991691336634, -0.30209928329444824, -0.304940725190638,
      -0.30806385838733613, -0.31146815554417506, -0.31515294176280007,
      -0.31911738983980015, -0.32336051521003611, -0.32788117058680516,
      -0.33267804030652809, -0.33774963438687061, -0.34309428230851935,
      -0.34871012653210365, -0.35459511576309649, -0.36074699797885157,
      -0.36716331323329293, -0.37384138625616953, -0.3807783188651308,
      -0.38797098221031873, -0.3954160088725468, -0.40310978483757393,
      -0.4110484413703604, -0.41922784681462649, -0.42764359834442295,
      -0.4362910136958274, -0.44516512290822885, -0.4542606601060345,
      -0.46357205535296864, -0.47309342661239057, -0.48281857184834814,
      -0.49274096130326811, -0.50285372998937083, -0.51314967043195048,
      -0.52362122570373282, -0.53426048279045824, -0.54505916632873719,
      -0.55600863275797063, -0.56709986492885045, -0.57832346721154926,
      -0.58966966114712716, -0.60112828168608012, -0.61268877405814126,
      -0.62434019131755669, -0.63607119260793521, -0.6478700421906034,
      -0.6597246092799699, -0.67162236872888981, -0.68355040260622424,
      -0.69549540270793253, -0.70744367404190711, -0.71938113932543613,
      -0.73129334453270778, -0.74316546552803653, -0.75498231581859931,
      -0.76672835545829665, -0.7783877011320548, -0.78994413744727732,
      -0.80138112945644158, -0.81268183643175285, -0.82382912690964039,
      -0.83480559501944473, -0.84559357810698044, -0.85617517565989443,
      -0.86653226953767171, -0.876646545504987, -0.886499516062646,
      -0.89607254456589236, -0.90534687061508323, -0.91430363669894621,
      -0.9229239160656082, -0.93118874179153222, -0.939079137013332,
      -0.94657614628212894, -0.95366086799486716, -0.96031448785161033,
      -0.96651831328252158, -0.97225380878284429, -0.97750263208891708,
      -0.98224667112298059, -0.98646808162937449, -0.99014932541964507,
      -0.99327320913916972, -0.99582292346314372, -0.99778208262518919,
      -0.99913476417750891, -0.99986554887738965, -0.999959560591032,
      -0.99940250610214143, -0.998180714709512, -0.996281177494972,
      -0.99369158614057884, -0.99040037117186674, -0.98639673950229267,
      -0.98167071115279358, -0.97621315501962858, -0.9700158235633829,
      -0.96307138629224165, -0.95537346191334049, -0.94691664902728168,
      -0.93769655524264262, -0.92770982458961671, -0.916954163114793,
      -0.90542836254244552, -0.89313232189164138, -0.88006706694291659,
      -0.86623476745329731, -0.85163875202390327, -0.83628352053041477,
      -0.82017475403313711, -0.80331932209043355, -0.785725287406648,
      -0.76740190775348038, -0.74835963511207049, -0.728610111991564,
      -0.7081661648889368, -0.68704179486400185, -0.66525216521312225,
      -0.64281358623475982, -0.61974349708996057, -0.59606044477084008,
      -0.57178406020034278, -0.54693503149666467, -0.52153507444588154,
      -0.49560690023656279, -0.46917418052008553, -0.44226150987034296,
      -0.41489436572616423, -0.38709906590940252, -0.35890272382067839,
      -0.33033320142378103, -0.30141906013813352, -0.27218950976692696,
      -0.2426743555961316, -0.21290394380667227, -0.1829091053488664,
      -0.15272109843409709, -0.12237154980432052, -0.091892394944721192,
      -0.061315817409204325, -0.03067418743179599 };

    static const real_T tmp_2[1024] = { 0.0, 0.0040906667695745341,
      0.0081816415167966956, 0.012273232051180256, 0.016365745846030646,
      0.020459489870489182, 0.024554770421755343, 0.028651892957546322,
      0.03275116192885303, 0.036852880613051621, 0.040957350947429451,
      0.045064873363184384, 0.049175746619955989, 0.053290267640947013,
      0.057408731348693683, 0.061531430501542583, 0.065658655530892021,
      0.069790694379255461, 0.073927832339204233, 0.0780703518932465,
      0.082218532554699172, 0.086372650709608881, 0.090532979459778326,
      0.094699788466953108, 0.098873343798224766, 0.10305390777270405,
      0.10724173880951965, 0.11143709127719559, 0.11564021534446078,
      0.11985135683254411, 0.12407075706900679, 0.1282986527431646,
      0.13253527576315072, 0.13678085311467031, 0.14103560672149709,
      0.14529975330776135, 0.14957350426207841, 0.15385706550356618,
      0.15815063734979923, 0.16245441438674671, 0.16676858534074035,
      0.17109333295251847, 0.17542883385339061, 0.17977525844356787,
      0.18413277077270127, 0.1885015284226724, 0.19288168239267703,
      0.19727337698664407, 0.20167674970302907, 0.20609193112702387,
      0.21051904482521877, 0.21495820724275702, 0.21940952760301857,
      0.22387310780986805, 0.22834904235250453, 0.23283741821294526,
      0.23733831477617903, 0.24185180374302048, 0.24637794904569779,
      0.2509168067662047, 0.25546842505744616, 0.26003284406720756,
      0.26461009586497414, 0.26920020437162984, 0.27380318529205844,
      0.27841904605067541, 0.28304778572991285, 0.28768939501167951,
      0.29234385612182096, 0.29701114277759771, 0.30169122013820354,
      0.3063840447583423, 0.3110895645448799, 0.3158077187165913,
      0.32053843776701468, 0.32528164343043048, 0.33003724865097755,
      0.33480515755491941, 0.33958526542607165, 0.34437745868440278,
      0.34918161486781479, 0.35399760261711477, 0.35882528166418343,
      0.36366450282334589, 0.36851510798595166, 0.37337693011816658,
      0.37824979326197905, 0.38313351253942368, 0.38802789416002215,
      0.39293273543144119, 0.39784782477336506, 0.40277294173458206,
      0.40770785701327988, 0.41265233248054373, 0.41760612120705437,
      0.42256896749297695, 0.42754060690103279, 0.43252076629274522,
      0.43750916386784872, 0.44250550920685189, 0.44750950331673822,
      0.45252083867979481, 0.45753919930555281, 0.46256426078582291,
      0.46759569035281218, 0.4726331469403009, 0.47767628124786382,
      0.48272473580811354, 0.487778145056947, 0.49283613540677207,
      0.49789832532269135, 0.50296432540162, 0.5080337384543121,
      0.51310615959027028, 0.51818117630550986, 0.523258368573153,
      0.52833730893681974, 0.53341756260679041, 0.5384986875589054,
      0.543580234636172, 0.54866174765304632, 0.55374276350235518,
      0.55882281226482411, 0.563901417321177, 0.56897809546677081,
      0.57405235702872714, 0.5791237059855241, 0.584191640089008,
      0.58925565098878951, 0.59431522435897721, 0.59936984002721261,
      0.6044189721059634, 0.60946208912603, 0.61449865417222516,
      0.6195281250211796, 0.62454995428122984, 0.6295635895343431,
      0.63456847348003009, 0.63956404408120315, 0.64454973471192778,
      0.64952497430701939, 0.65448918751344076, 0.65944179484344423,
      0.66438221282941157, 0.66930985418034228, 0.67422412793993436,
      0.67912443964620917, 0.68401019149262732, 0.68888078249063989,
      0.69373560863362582, 0.69857406306215675, 0.70339553623053863,
      0.70819941607457271, 0.71298508818048134, 0.7177519359549428,
      0.72249934079618006, 0.72722668226604359, 0.731933338263036,
      0.7366186851962172, 0.74128209815993529, 0.7459229511093225,
      0.75054061703650266, 0.75513446814744645, 0.75970387603941936,
      0.76424821187896341, 0.768766846580351, 0.77325915098445641,
      0.77772449603797844, 0.78216225297296471, 0.786571793486567,
      0.79095248992097777, 0.79530371544348111, 0.79962484422656133,
      0.80391525162801092, 0.80817431437097342, 0.8124014107238664,
      0.816595920680122, 0.820757226137685, 0.82488471107820993,
      0.82897776174589832, 0.83303576682591418, 0.83705811762232019,
      0.84104420823547577, 0.84499343573883623, 0.84890520035509509,
      0.85277890563161207, 0.85661395861506662, 0.86040977002527941,
      0.86416575442814414, 0.86788133040761217, 0.87155592073667254,
      0.87518895254726958, 0.87877985749910359, 0.882328071947256,
      0.88583303710858408, 0.88929419922683106, 0.8927110097363945,
      0.896082925424699, 0.89940940859312057, 0.902689927216406,
      0.90592395510053747, 0.90911097203898839, 0.912250463967315,
      0.91534192311604068, 0.91838484816177068, 0.92137874437649714,
      0.924323123775038, 0.92721750526056379, 0.930061414768162,
      0.93285438540639165, 0.93559595759678293, 0.93828567921123118,
      0.94092310570724214, 0.943507800260984, 0.94603933389809924,
      0.94851728562223458, 0.95094124254124524, 0.95331079999103185,
      0.95562556165696833, 0.95788513969288058, 0.9600891548375351,
      0.96223723652860027, 0.96432902301404155, 0.96636416146091253,
      0.96834230806150667, 0.9702631281368338, 0.972126296237387,
      0.97393149624116548, 0.97567842144892214, 0.9773667746766016,
      0.97899626834494136, 0.98056662456620247, 0.98207757522800287,
      0.98352886207422474, 0.9849202367829698, 0.98625146104153372,
      0.987522306618379, 0.98873255543207739, 0.98988199961720313,
      0.99097044158715153, 0.99199769409386451, 0.99296358028444176,
      0.99386793375461746, 0.9947105985990895, 0.99549142945867664,
      0.99621029156429575, 0.996867060777738, 0.99746162362923674,
      0.99799387735180645, 0.99846372991234988, 0.99887110003951685,
      0.999215917248307, 0.99949812186141151, 0.99971766502727932,
      0.99987450873491157, 0.99996862582536961, 1.0, 0.99996862582536961,
      0.99987450873491157, 0.99971766502727932, 0.99949812186141151,
      0.999215917248307, 0.99887110003951685, 0.99846372991234988,
      0.99799387735180645, 0.99746162362923674, 0.996867060777738,
      0.99621029156429575, 0.99549142945867664, 0.99471059859908972,
      0.99386793375461746, 0.99296358028444176, 0.99199769409386462,
      0.99097044158715153, 0.98988199961720313, 0.98873255543207739,
      0.987522306618379, 0.98625146104153372, 0.9849202367829698,
      0.98352886207422474, 0.98207757522800287, 0.98056662456620247,
      0.97899626834494136, 0.9773667746766016, 0.97567842144892214,
      0.97393149624116548, 0.972126296237387, 0.9702631281368338,
      0.96834230806150667, 0.96636416146091253, 0.96432902301404155,
      0.96223723652860027, 0.9600891548375351, 0.95788513969288058,
      0.95562556165696844, 0.95331079999103185, 0.95094124254124524,
      0.94851728562223458, 0.94603933389809924, 0.943507800260984,
      0.94092310570724214, 0.93828567921123118, 0.935595957596783,
      0.93285438540639165, 0.930061414768162, 0.92721750526056379,
      0.92432312377503811, 0.92137874437649714, 0.91838484816177068,
      0.91534192311604079, 0.91225046396731513, 0.90911097203898839,
      0.90592395510053747, 0.902689927216406, 0.89940940859312057,
      0.89608292542469925, 0.8927110097363945, 0.88929419922683106,
      0.88583303710858408, 0.882328071947256, 0.87877985749910359,
      0.87518895254726969, 0.87155592073667254, 0.86788133040761217,
      0.86416575442814414, 0.86040977002527941, 0.85661395861506673,
      0.85277890563161207, 0.84890520035509509, 0.84499343573883623,
      0.8410442082354761, 0.8370581176223203, 0.833035766825914,
      0.82897776174589843, 0.82488471107820993, 0.82075722613768531,
      0.81659592068012221, 0.81240141072386651, 0.80817431437097342,
      0.80391525162801114, 0.79962484422656155, 0.79530371544348133,
      0.790952489920978, 0.78657179348656692, 0.78216225297296471,
      0.77772449603797844, 0.77325915098445641, 0.76876684658035133,
      0.76424821187896363, 0.75970387603941936, 0.75513446814744667,
      0.75054061703650277, 0.74592295110932261, 0.74128209815993529,
      0.73661868519621709, 0.731933338263036, 0.72722668226604359,
      0.72249934079618028, 0.7177519359549428, 0.71298508818048167,
      0.70819941607457293, 0.703395536230539, 0.69857406306215675,
      0.69373560863362582, 0.68888078249064, 0.68401019149262721,
      0.6791244396462095, 0.67422412793993436, 0.66930985418034261,
      0.66438221282941157, 0.65944179484344445, 0.654489187513441,
      0.64952497430701961, 0.64454973471192778, 0.63956404408120315,
      0.6345684734800302, 0.62956358953434288, 0.62454995428123006,
      0.6195281250211796, 0.61449865417222527, 0.60946208912603,
      0.60441897210596351, 0.59936984002721272, 0.59431522435897743,
      0.58925565098878963, 0.58419164008900792, 0.5791237059855241,
      0.57405235702872714, 0.568978095466771, 0.563901417321177,
      0.55882281226482433, 0.55374276350235518, 0.54866174765304665,
      0.54358023463617211, 0.53849868755890562, 0.53341756260679052,
      0.52833730893681963, 0.52325836857315311, 0.51818117630550986,
      0.51310615959027039, 0.5080337384543121, 0.50296432540162017,
      0.49789832532269135, 0.49283613540677235, 0.48777814505694717,
      0.48272473580811376, 0.47767628124786382, 0.47263314694030084,
      0.46759569035281229, 0.46256426078582291, 0.45753919930555292,
      0.45252083867979481, 0.44750950331673839, 0.44250550920685189,
      0.437509163867849, 0.43252076629274527, 0.42754060690103279,
      0.42256896749297712, 0.41760612120705431, 0.41265233248054378,
      0.40770785701327988, 0.40277294173458228, 0.39784782477336506,
      0.3929327354314413, 0.38802789416002231, 0.38313351253942396,
      0.37824979326197922, 0.37337693011816647, 0.36851510798595183,
      0.36366450282334578, 0.35882528166418354, 0.35399760261711477,
      0.3491816148678149, 0.34437745868440278, 0.33958526542607187,
      0.33480515755491941, 0.33003724865097789, 0.32528164343043059,
      0.32053843776701463, 0.31580771871659141, 0.31108956454487985,
      0.30638404475834241, 0.30169122013820354, 0.29701114277759788,
      0.292343856121821, 0.28768939501167973, 0.2830477857299129,
      0.27841904605067569, 0.27380318529205849, 0.26920020437162978,
      0.26461009586497436, 0.2600328440672075, 0.25546842505744632,
      0.2509168067662047, 0.24637794904569799, 0.24185180374302051,
      0.23733831477617925, 0.23283741821294535, 0.22834904235250478,
      0.22387310780986816, 0.21940952760301849, 0.21495820724275716,
      0.21051904482521874, 0.20609193112702404, 0.2016767497030291,
      0.19727337698664424, 0.19288168239267711, 0.18850152842267262,
      0.18413277077270132, 0.17977525844356809, 0.1754288338533907,
      0.17109333295251838, 0.16676858534074049, 0.16245441438674671,
      0.15815063734979939, 0.15385706550356618, 0.14957350426207858,
      0.14529975330776138, 0.14103560672149731, 0.13678085311467039,
      0.13253527576315097, 0.12829865274316474, 0.12407075706900676,
      0.11985135683254423, 0.11564021534446078, 0.11143709127719574,
      0.10724173880951966, 0.10305390777270422, 0.098873343798224808,
      0.094699788466953358, 0.0905329794597784, 0.086372650709608825,
      0.08221853255469927, 0.078070351893246473, 0.073927832339204344,
      0.069790694379255461, 0.065658655530892174, 0.061531430501542604,
      0.057408731348693863, 0.053290267640947055, 0.0491757466199562,
      0.045064873363184467, 0.040957350947429388, 0.036852880613051718,
      0.032751161928853, 0.028651892957546457, 0.024554770421755343,
      0.020459489870489341, 0.01636574584603067, 0.012273232051180444,
      0.00818164151679675, 0.00409066676957475, 8.1643119943156876E-17,
      -0.004090666769574587, -0.0081816415167965863, -0.012273232051180282,
      -0.016365745846030507, -0.020459489870489175, -0.024554770421755176,
      -0.028651892957546294, -0.032751161928852836, -0.036852880613051558,
      -0.040957350947429229, -0.0450648733631843, -0.04917574661995603,
      -0.053290267640946888, -0.0574087313486937, -0.061531430501542445,
      -0.065658655530892021, -0.06979069437925528, -0.073927832339204191,
      -0.0780703518932463, -0.0822185325546991, -0.086372650709608659,
      -0.090532979459778243, -0.094699788466953164, -0.098873343798224655,
      -0.10305390777270405, -0.1072417388095195, -0.11143709127719557,
      -0.1156402153444606, -0.11985135683254407, -0.1240707570690066,
      -0.12829865274316454, -0.1325352757631508, -0.1367808531146702,
      -0.14103560672149712, -0.14529975330776121, -0.14957350426207841,
      -0.153857065503566, -0.1581506373497992, -0.16245441438674654,
      -0.16676858534074032, -0.17109333295251825, -0.17542883385339056,
      -0.17977525844356793, -0.18413277077270118, -0.18850152842267245,
      -0.19288168239267692, -0.19727337698664402, -0.20167674970302887,
      -0.20609193112702387, -0.21051904482521858, -0.21495820724275697,
      -0.21940952760301832, -0.223873107809868, -0.22834904235250456,
      -0.23283741821294515, -0.23733831477617903, -0.24185180374302032,
      -0.24637794904569779, -0.25091680676620448, -0.25546842505744616,
      -0.26003284406720734, -0.26461009586497414, -0.26920020437162956,
      -0.27380318529205833, -0.27841904605067552, -0.28304778572991268,
      -0.28768939501167956, -0.29234385612182084, -0.29701114277759771,
      -0.30169122013820338, -0.30638404475834219, -0.31108956454487968,
      -0.31580771871659125, -0.32053843776701446, -0.32528164343043037,
      -0.33003724865097767, -0.3348051575549193, -0.33958526542607176,
      -0.34437745868440262, -0.34918161486781479, -0.3539976026171146,
      -0.35882528166418332, -0.36366450282334561, -0.36851510798595161,
      -0.37337693011816636, -0.378249793261979, -0.38313351253942374,
      -0.38802789416002209, -0.39293273543144114, -0.39784782477336483,
      -0.40277294173458206, -0.40770785701327972, -0.41265233248054373,
      -0.41760612120705409, -0.4225689674929769, -0.42754060690103257,
      -0.43252076629274505, -0.43750916386784877, -0.44250550920685178,
      -0.44750950331673822, -0.45252083867979459, -0.45753919930555281,
      -0.46256426078582269, -0.46759569035281207, -0.47263314694030062,
      -0.47767628124786365, -0.48272473580811354, -0.48777814505694694,
      -0.49283613540677212, -0.49789832532269118, -0.50296432540162,
      -0.50803373845431188, -0.51310615959027028, -0.51818117630550975,
      -0.52325836857315289, -0.52833730893681952, -0.5334175626067903,
      -0.5384986875589054, -0.54358023463617189, -0.54866174765304654,
      -0.55374276350235518, -0.55882281226482411, -0.56390141732117682,
      -0.56897809546677081, -0.574052357028727, -0.579123705985524,
      -0.58419164008900781, -0.58925565098878951, -0.59431522435897721,
      -0.5993698400272125, -0.6044189721059634, -0.60946208912602973,
      -0.61449865417222516, -0.61952812502117938, -0.62454995428122984,
      -0.62956358953434277, -0.63456847348003, -0.63956404408120293,
      -0.64454973471192767, -0.649524974307019, -0.6544891875134411,
      -0.65944179484344423, -0.66438221282941146, -0.66930985418034206,
      -0.67422412793993447, -0.67912443964620917, -0.684010191492627,
      -0.68888078249063944, -0.69373560863362582, -0.69857406306215664,
      -0.70339553623053852, -0.708199416074573, -0.71298508818048134,
      -0.71775193595494269, -0.72249934079617972, -0.7272266822660437,
      -0.731933338263036, -0.736618685196217, -0.74128209815993473,
      -0.7459229511093225, -0.75054061703650243, -0.75513446814744611,
      -0.75970387603941969, -0.76424821187896352, -0.768766846580351,
      -0.773259150984456, -0.77772449603797844, -0.78216225297296449,
      -0.78657179348656669, -0.79095248992097744, -0.79530371544348111,
      -0.79962484422656133, -0.8039152516280107, -0.80817431437097353,
      -0.8124014107238664, -0.816595920680122, -0.82075722613768465,
      -0.82488471107821015, -0.82897776174589832, -0.833035766825914,
      -0.83705811762231985, -0.84104420823547577, -0.84499343573883612,
      -0.84890520035509465, -0.85277890563161218, -0.85661395861506662,
      -0.86040977002527919, -0.86416575442814381, -0.86788133040761217,
      -0.87155592073667243, -0.87518895254726947, -0.87877985749910337,
      -0.882328071947256, -0.885833037108584, -0.889294199226831,
      -0.89271100973639461, -0.89608292542469925, -0.89940940859312046,
      -0.90268992721640551, -0.90592395510053747, -0.909110972038988,
      -0.91225046396731491, -0.91534192311604079, -0.91838484816177068,
      -0.92137874437649714, -0.924323123775038, -0.92721750526056412,
      -0.93006141476816184, -0.93285438540639154, -0.93559595759678282,
      -0.93828567921123118, -0.94092310570724214, -0.94350780026098391,
      -0.94603933389809935, -0.94851728562223458, -0.95094124254124524,
      -0.95331079999103174, -0.95562556165696844, -0.95788513969288058,
      -0.96008915483753487, -0.96223723652860016, -0.96432902301404155,
      -0.96636416146091253, -0.96834230806150645, -0.970263128136834,
      -0.972126296237387, -0.97393149624116548, -0.97567842144892192,
      -0.9773667746766016, -0.97899626834494125, -0.98056662456620247,
      -0.98207757522800276, -0.98352886207422474, -0.9849202367829698,
      -0.98625146104153372, -0.98752230661837925, -0.98873255543207739,
      -0.98988199961720313, -0.99097044158715153, -0.99199769409386462,
      -0.99296358028444176, -0.99386793375461746, -0.9947105985990895,
      -0.99549142945867664, -0.99621029156429575, -0.996867060777738,
      -0.99746162362923674, -0.99799387735180645, -0.99846372991234988,
      -0.99887110003951651, -0.999215917248307, -0.99949812186141151,
      -0.99971766502727932, -0.99987450873491157, -0.99996862582536961, -1.0,
      -0.99996862582536961, -0.99987450873491157, -0.99971766502727932,
      -0.99949812186141151, -0.999215917248307, -0.99887110003951685,
      -0.99846372991234988, -0.99799387735180645, -0.99746162362923674,
      -0.996867060777738, -0.99621029156429575, -0.99549142945867664,
      -0.9947105985990895, -0.99386793375461746, -0.99296358028444176,
      -0.99199769409386462, -0.99097044158715153, -0.98988199961720313,
      -0.9887325554320775, -0.98752230661837925, -0.98625146104153372,
      -0.9849202367829698, -0.98352886207422474, -0.98207757522800276,
      -0.98056662456620247, -0.97899626834494136, -0.97736677467660182,
      -0.97567842144892214, -0.97393149624116571, -0.97212629623738722,
      -0.970263128136834, -0.96834230806150667, -0.96636416146091253,
      -0.96432902301404189, -0.96223723652860016, -0.9600891548375351,
      -0.9578851396928808, -0.95562556165696855, -0.95331079999103185,
      -0.95094124254124524, -0.9485172856222347, -0.94603933389809958,
      -0.943507800260984, -0.94092310570724225, -0.93828567921123129,
      -0.93559595759678293, -0.93285438540639165, -0.930061414768162,
      -0.92721750526056423, -0.924323123775038, -0.92137874437649725,
      -0.918384848161771, -0.915341923116041, -0.912250463967315,
      -0.90911097203898839, -0.9059239551005378, -0.90268992721640584,
      -0.89940940859312057, -0.89608292542469936, -0.89271100973639472,
      -0.88929419922683106, -0.88583303710858408, -0.88232807194725626,
      -0.87877985749910348, -0.87518895254726958, -0.87155592073667265,
      -0.86788133040761239, -0.86416575442814392, -0.86040977002527941,
      -0.85661395861506673, -0.85277890563161229, -0.84890520035509509,
      -0.84499343573883623, -0.8410442082354761, -0.83705811762232007,
      -0.83303576682591418, -0.82897776174589843, -0.82488471107821015,
      -0.82075722613768487, -0.81659592068012221, -0.81240141072386685,
      -0.80817431437097387, -0.80391525162801092, -0.79962484422656155,
      -0.79530371544348144, -0.79095248992097755, -0.786571793486567,
      -0.78216225297296482, -0.77772449603797877, -0.7732591509844563,
      -0.76876684658035144, -0.76424821187896363, -0.75970387603942,
      -0.75513446814744645, -0.75054061703650277, -0.74592295110932272,
      -0.741282098159935, -0.7366186851962172, -0.7319333382630363,
      -0.72722668226604392, -0.72249934079618006, -0.717751935954943,
      -0.71298508818048167, -0.70819941607457337, -0.70339553623053863,
      -0.698574063062157, -0.693735608633626, -0.68888078249063966,
      -0.68401019149262732, -0.67912443964620961, -0.6742241279399348,
      -0.66930985418034228, -0.66438221282941179, -0.65944179484344456,
      -0.65448918751344143, -0.64952497430701939, -0.64454973471192789,
      -0.63956404408120371, -0.63456847348003, -0.6295635895343431,
      -0.62454995428123006, -0.61952812502117993, -0.61449865417222493,
      -0.60946208912603006, -0.60441897210596363, -0.599369840027213,
      -0.59431522435897721, -0.58925565098878974, -0.58419164008900848,
      -0.57912370598552387, -0.57405235702872737, -0.56897809546677125,
      -0.56390141732117749, -0.55882281226482411, -0.55374276350235541,
      -0.54866174765304676, -0.54358023463617255, -0.5384986875589054,
      -0.53341756260679063, -0.52833730893682007, -0.52325836857315289,
      -0.51818117630551, -0.51310615959027051, -0.50803373845431254,
      -0.50296432540162, -0.49789832532269146, -0.49283613540677235,
      -0.48777814505694761, -0.48272473580811354, -0.47767628124786388,
      -0.47263314694030129, -0.46759569035281207, -0.46256426078582297,
      -0.45753919930555303, -0.45252083867979526, -0.44750950331673817,
      -0.44250550920685194, -0.43750916386784916, -0.43252076629274572,
      -0.42754060690103279, -0.42256896749297718, -0.41760612120705476,
      -0.41265233248054356, -0.40770785701328, -0.40277294173458245,
      -0.39784782477336544, -0.39293273543144114, -0.38802789416002237,
      -0.383133512539424, -0.37824979326197955, -0.37337693011816664,
      -0.36851510798595188, -0.36366450282334623, -0.35882528166418326,
      -0.35399760261711483, -0.349181614867815, -0.34437745868440323,
      -0.33958526542607159, -0.33480515755491957, -0.33003724865097794,
      -0.32528164343043103, -0.32053843776701468, -0.31580771871659152,
      -0.31108956454488029, -0.30638404475834219, -0.30169122013820365,
      -0.29701114277759794, -0.29234385612182145, -0.28768939501167951,
      -0.283047785729913, -0.27841904605067574, -0.27380318529205888,
      -0.26920020437162984, -0.26461009586497447, -0.26003284406720795,
      -0.2554684250574461, -0.25091680676620481, -0.24637794904569807,
      -0.24185180374302087, -0.23733831477617898, -0.23283741821294543,
      -0.22834904235250486, -0.22387310780986791, -0.21940952760301857,
      -0.21495820724275724, -0.21051904482521913, -0.20609193112702384,
      -0.20167674970302918, -0.19727337698664429, -0.19288168239267747,
      -0.18850152842267234, -0.18413277077270143, -0.1797752584435682,
      -0.1754288338533905, -0.1710933329525185, -0.16676858534074057,
      -0.1624544143867471, -0.15815063734979914, -0.15385706550356629,
      -0.14957350426207866, -0.14529975330776176, -0.14103560672149709,
      -0.13678085311467045, -0.13253527576315105, -0.12829865274316449,
      -0.12407075706900683, -0.11985135683254432, -0.11564021534446117,
      -0.11143709127719552, -0.10724173880951976, -0.10305390777270429,
      -0.098873343798225211, -0.094699788466953108, -0.090532979459778493,
      -0.0863726507096092, -0.082218532554699061, -0.078070351893246556,
      -0.073927832339204427, -0.069790694379255822, -0.065658655530891966,
      -0.061531430501542687, -0.057408731348693953, -0.053290267640947436,
      -0.049175746619955975, -0.045064873363184543, -0.04095735094742977,
      -0.03685288061305151, -0.032751161928853086, -0.028651892957546537,
      -0.024554770421755717, -0.020459489870489126, -0.01636574584603075,
      -0.012273232051180527, -0.0081816415167971276, -0.0040906667695745349 };

    static const real_T tmp_3[1024] = { 0.0, 0.024555080660630939,
      0.049079685308301577, 0.073543491153235233, 0.097916480212640031,
      0.1221690876457996, 0.14627234529039596, 0.17019801893684458,
      0.19391873798944334, 0.21740811629729673, 0.24064086309071767,
      0.26359288312613927, 0.28624136532028382, 0.3085648593380782,
      0.33054333978428679, 0.35215825783191768, 0.37339258029731309,
      0.394230816339015, 0.4146590321120443, 0.43466485384870035,
      0.45423745995952741, 0.47336756285239745, 0.49204738125301273,
      0.51027060387633216, 0.52803234534577248, 0.54532909528625662,
      0.562158661529359, 0.57852010836532153, 0.59441369075918515,
      0.60984078541845166, 0.62480381955941477, 0.6393061981704169,
      0.65335223051465707, 0.66694705655455444, 0.680096573915689,
      0.69280736594255632, 0.70508663133209659, 0.71694211576540656,
      0.72838204589423317, 0.73941506597760465, 0.7500501774059618,
      0.760296681295933, 0.770164124288808, 0.77966224764004588,
      0.78880093964594267, 0.79759019141686238, 0.80604005597419115,
      0.81416061062021228, 0.82196192250628053, 0.82945401730472579,
      0.836646850873586, 0.84355028379028685, 0.85017405862041606,
      0.85652777978052508, 0.8626208958491034, 0.86846268417723,
      0.87406223764963942, 0.87942845344777931, 0.88457002366863768,
      0.88949542765645018, 0.89421292590867552, 0.8987305554226267,
      0.9030561263547352, 0.907197219870435, 0.91116118706896143,
      0.91495514887384621, 0.918585996786463, 0.92206039440655563,
      0.9253847796301824, 0.92856536744189333, 0.93160815322416268,
      0.93451891651309793, 0.93730322513520414, 0.93996643966548432,
      0.94251371815238061, 0.94495002106000514, 0.94728011638276144,
      0.949508584891828, 0.9516398254770515, 0.95367806055160131,
      0.95562734149026463, 0.957491554075524, 0.95927442392857254,
      0.96097952190519187, 0.96261026943895689, 0.9641699438165573,
      0.96566168337214187, 0.9670884925895179, 0.96845324710279,
      0.96975869858759656, 0.97100747953652777, 0.97220210791359318,
      0.973344991683748, 0.97443843321451351, 0.97548463354764114,
      0.97648569653957074, 0.97744363287015412, 0.9783603639197338,
      0.97923772551522092, 0.98007747154628844, 0.98088127745320586,
      0.98165074358820059, 0.98238739845252132, 0.983092701811638,
      0.98376804769121906, 0.98441476725669641, 0.98503413157936637,
      0.98562735429208126, 0.98619559413766267, 0.98673995741322562,
      0.98726150031363191, 0.9877612311773134, 0.98824011263769562,
      0.98869906368344485, 0.98913896163073145, 0.98956064401066235,
      0.98996491037499179, 0.99035252402316576, 0.99072421365368857,
      0.99108067494274388, 0.991422572052922, 0.99175053907484112,
      0.992065181404365, 0.99236707705805072, 0.99265677792937179,
      0.99293481098818936, 0.99320167942585869, 0.99345786374827993,
      0.99370382281912406, 0.993939994855379, 0.994166798377293,
      0.99438463311470626, 0.99459388087169109, 0.99479490635134526,
      0.99498805794251355, 0.99517366847013711, 0.99535205591086628,
      0.99552352407550415, 0.99568836325978294, 0.99584685086491265,
      0.995999251989283, 0.9961458199926353, 0.99628679703397427,
      0.99642241458442127, 0.99655289391617208, 0.99667844656866134,
      0.99679927479299024, 0.99691557197563052, 0.997027523042365,
      0.9971353048433903, 0.99723908652045823, 0.99733902985689771,
      0.99743528961131778, 0.99752801383575751, 0.997617344179012,
      0.99770341617582992, 0.99778635952264927, 0.99786629834050145,
      0.99794335142568913, 0.99801763248881414, 0.99808925038270235,
      0.99815830931975025, 0.998224909079191, 0.99828914520475542,
      0.99835110919318037, 0.99841088867399586, 0.99846856758100255,
      0.99852422631583, 0.99857794190395066, 0.9986297881435019,
      0.99867983574725838, 0.99872815247807323, 0.99877480327809876,
      0.99881985039207588, 0.99886335348497335, 0.99890536975424027,
      0.9989459540369261, 0.99898515891190687, 0.99902303479744847,
      0.99905963004432374, 0.99909499102469246, 0.9991291622169407,
      0.9991621862866692, 0.99919410416400867, 0.99922495511743492,
      0.99925477682424457, 0.99928360543784633, 0.99931147565201761,
      0.99933842076226365, 0.99936447272441609, 0.99938966221059489,
      0.999414018662659, 0.99943757034325764, 0.99946034438459452,
      0.999482366835008, 0.99950366270346935, 0.99952425600209038,
      0.9995441697867361, 0.99956342619582284, 0.99958204648738835,
      0.99960005107451022, 0.9996174595591466, 0.99963429076447086,
      0.99965056276576825, 0.999666292919957, 0.99968149789379757,
      0.99969619369084484, 0.99971039567720421, 0.99972411860613764,
      0.99973737664157658, 0.999750183380584, 0.99976255187481622,
      0.999774494651025, 0.99978602373064018, 0.9997971506484773,
      0.99980788647060137, 0.99981824181138723, 0.9998282268498091,
      0.99983785134498993, 0.99984712465104431, 0.99985605573124148,
      0.999864653171518, 0.99987292519336657, 0.9998808796661246,
      0.99988852411868834, 0.99989586575067324, 0.99990291144304522,
      0.99990966776823975, 0.99991614099979087, 0.99992233712148748,
      0.99992826183607386, 0.99993392057351327, 0.99993931849882722,
      0.99994446051952779, 0.99994935129265561, 0.99995399523143769,
      0.9999583965115767, 0.999962559077185, 0.99996648664637144,
      0.9999701827164964, 0.99997365056909882, 0.99997689327450989,
      0.99997991369615824, 0.99998271449457654, 0.99998529813111547,
      0.99998766687137342, 0.99998982278834747, 0.9999917677653114,
      0.99999350349842586, 0.999995031499086, 0.99999635309601065,
      0.99999746943707712, 0.99999838149090381, 0.9999990900481851,
      0.99999959572278041, 0.99999989895255847, 1.0, 0.99999989895255847,
      0.99999959572278041, 0.9999990900481851, 0.99999838149090381,
      0.99999746943707712, 0.99999635309601065, 0.999995031499086,
      0.99999350349842586, 0.9999917677653114, 0.99998982278834747,
      0.99998766687137342, 0.99998529813111547, 0.99998271449457654,
      0.99997991369615824, 0.99997689327450989, 0.99997365056909882,
      0.9999701827164964, 0.99996648664637144, 0.999962559077185,
      0.9999583965115767, 0.99995399523143769, 0.99994935129265561,
      0.99994446051952779, 0.99993931849882722, 0.99993392057351327,
      0.99992826183607386, 0.99992233712148748, 0.99991614099979087,
      0.99990966776823975, 0.99990291144304522, 0.99989586575067324,
      0.99988852411868834, 0.9998808796661246, 0.99987292519336657,
      0.999864653171518, 0.99985605573124148, 0.99984712465104431,
      0.99983785134498993, 0.9998282268498091, 0.99981824181138723,
      0.99980788647060137, 0.9997971506484773, 0.99978602373064018,
      0.999774494651025, 0.99976255187481622, 0.999750183380584,
      0.99973737664157658, 0.99972411860613764, 0.99971039567720421,
      0.99969619369084484, 0.99968149789379757, 0.999666292919957,
      0.99965056276576825, 0.99963429076447086, 0.9996174595591466,
      0.99960005107451022, 0.99958204648738835, 0.99956342619582284,
      0.9995441697867361, 0.99952425600209038, 0.99950366270346935,
      0.999482366835008, 0.99946034438459452, 0.99943757034325764,
      0.999414018662659, 0.99938966221059489, 0.99936447272441609,
      0.99933842076226365, 0.99931147565201761, 0.99928360543784633,
      0.99925477682424457, 0.99922495511743492, 0.99919410416400867,
      0.9991621862866692, 0.9991291622169407, 0.99909499102469246,
      0.99905963004432374, 0.99902303479744847, 0.99898515891190687,
      0.9989459540369261, 0.99890536975424027, 0.99886335348497335,
      0.99881985039207588, 0.99877480327809876, 0.99872815247807323,
      0.99867983574725838, 0.9986297881435019, 0.99857794190395066,
      0.99852422631583, 0.99846856758100255, 0.99841088867399586,
      0.99835110919318037, 0.99828914520475542, 0.998224909079191,
      0.99815830931975025, 0.99808925038270235, 0.99801763248881414,
      0.99794335142568913, 0.99786629834050145, 0.99778635952264927,
      0.99770341617582992, 0.997617344179012, 0.99752801383575751,
      0.99743528961131778, 0.99733902985689771, 0.99723908652045823,
      0.9971353048433903, 0.997027523042365, 0.99691557197563052,
      0.99679927479299024, 0.99667844656866134, 0.99655289391617208,
      0.99642241458442127, 0.99628679703397427, 0.9961458199926353,
      0.995999251989283, 0.99584685086491265, 0.99568836325978294,
      0.99552352407550426, 0.99535205591086628, 0.99517366847013711,
      0.99498805794251355, 0.99479490635134526, 0.99459388087169109,
      0.99438463311470626, 0.994166798377293, 0.993939994855379,
      0.99370382281912406, 0.99345786374827993, 0.99320167942585869,
      0.99293481098818936, 0.9926567779293719, 0.99236707705805072,
      0.992065181404365, 0.99175053907484112, 0.991422572052922,
      0.99108067494274388, 0.99072421365368868, 0.99035252402316576,
      0.98996491037499179, 0.98956064401066235, 0.98913896163073145,
      0.98869906368344485, 0.98824011263769562, 0.9877612311773134,
      0.98726150031363191, 0.98673995741322562, 0.98619559413766278,
      0.98562735429208126, 0.98503413157936637, 0.98441476725669641,
      0.98376804769121906, 0.983092701811638, 0.98238739845252132,
      0.98165074358820059, 0.98088127745320586, 0.98007747154628844,
      0.97923772551522092, 0.9783603639197338, 0.97744363287015412,
      0.97648569653957074, 0.975484633547641, 0.97443843321451351,
      0.973344991683748, 0.97220210791359329, 0.97100747953652777,
      0.96975869858759667, 0.96845324710279013, 0.967088492589518,
      0.96566168337214187, 0.9641699438165573, 0.96261026943895689,
      0.96097952190519176, 0.95927442392857265, 0.957491554075524,
      0.95562734149026463, 0.95367806055160131, 0.95163982547705162,
      0.949508584891828, 0.94728011638276155, 0.94495002106000514,
      0.94251371815238061, 0.93996643966548443, 0.93730322513520414,
      0.93451891651309793, 0.93160815322416268, 0.92856536744189344,
      0.9253847796301824, 0.92206039440655585, 0.918585996786463,
      0.91495514887384644, 0.91116118706896154, 0.90719721987043489,
      0.90305612635473531, 0.8987305554226267, 0.89421292590867563,
      0.88949542765645018, 0.88457002366863791, 0.87942845344777942,
      0.87406223764963964, 0.86846268417723016, 0.86262089584910373,
      0.8565277797805253, 0.850174058620416, 0.84355028379028707,
      0.83664685087358592, 0.829454017304726, 0.82196192250628064,
      0.8141606106202125, 0.80604005597419137, 0.79759019141686283,
      0.78880093964594278, 0.77966224764004644, 0.77016412428880821,
      0.76029668129593286, 0.75005017740596214, 0.73941506597760465,
      0.7283820458942335, 0.71694211576540656, 0.70508663133209715,
      0.69280736594255632, 0.68009657391568956, 0.66694705655455466,
      0.65335223051465807, 0.63930619817041723, 0.62480381955941466,
      0.60984078541845221, 0.59441369075918515, 0.57852010836532208,
      0.562158661529359, 0.54532909528625717, 0.52803234534577259,
      0.51027060387633327, 0.49204738125301306, 0.47336756285239717,
      0.45423745995952791, 0.43466485384870013, 0.41465903211204491,
      0.394230816339015, 0.37339258029731381, 0.35215825783191784,
      0.33054333978428774, 0.30856485933807848, 0.28624136532028488,
      0.26359288312613977, 0.24064086309071733, 0.21740811629729734,
      0.19391873798944315, 0.17019801893684541, 0.14627234529039596,
      0.12216908764580053, 0.09791648021264017, 0.073543491153236357,
      0.0490796853083019, 0.024555080660632233, 4.9018748853540808E-16,
      -0.024555080660631251, -0.049079685308300931, -0.0735434911532354,
      -0.0979164802126392, -0.12216908764579955, -0.14627234529039496,
      -0.17019801893684447, -0.19391873798944223, -0.21740811629729637,
      -0.24064086309071644, -0.26359288312613882, -0.28624136532028405,
      -0.30856485933807754, -0.3305433397842869, -0.352158257831917,
      -0.37339258029731309, -0.39423081633901413, -0.41465903211204408,
      -0.43466485384869946, -0.45423745995952719, -0.47336756285239634,
      -0.49204738125301234, -0.5102706038763325, -0.52803234534577215,
      -0.54532909528625662, -0.56215866152935834, -0.57852010836532153,
      -0.59441369075918449, -0.60984078541845155, -0.62480381955941411,
      -0.63930619817041667, -0.65335223051465741, -0.66694705655455411,
      -0.680096573915689, -0.692807365942556, -0.70508663133209659,
      -0.716942115765406, -0.72838204589423317, -0.73941506597760431,
      -0.7500501774059618, -0.76029668129593253, -0.77016412428880776,
      -0.7796622476400461, -0.78880093964594244, -0.79759019141686249,
      -0.80604005597419093, -0.81416061062021217, -0.82196192250628031,
      -0.82945401730472579, -0.8366468508735857, -0.84355028379028685,
      -0.85017405862041573, -0.856527779780525, -0.8626208958491034,
      -0.86846268417722994, -0.87406223764963942, -0.8794284534477792,
      -0.88457002366863768, -0.88949542765645, -0.89421292590867552,
      -0.89873055542262648, -0.90305612635473509, -0.90719721987043467,
      -0.91116118706896143, -0.91495514887384632, -0.91858599678646291,
      -0.92206039440655574, -0.92538477963018229, -0.92856536744189333,
      -0.93160815322416257, -0.93451891651309782, -0.937303225135204,
      -0.93996643966548432, -0.9425137181523805, -0.944950021060005,
      -0.94728011638276144, -0.949508584891828, -0.9516398254770515,
      -0.9536780605516012, -0.95562734149026463, -0.95749155407552389,
      -0.95927442392857254, -0.96097952190519176, -0.96261026943895678,
      -0.9641699438165573, -0.96566168337214187, -0.9670884925895179,
      -0.96845324710279, -0.96975869858759656, -0.97100747953652777,
      -0.97220210791359318, -0.97334499168374788, -0.97443843321451351,
      -0.975484633547641, -0.97648569653957074, -0.977443632870154,
      -0.97836036391973369, -0.97923772551522092, -0.98007747154628833,
      -0.98088127745320586, -0.98165074358820048, -0.98238739845252132,
      -0.983092701811638, -0.98376804769121906, -0.98441476725669641,
      -0.98503413157936637, -0.98562735429208126, -0.98619559413766267,
      -0.98673995741322562, -0.98726150031363191, -0.9877612311773134,
      -0.98824011263769562, -0.98869906368344485, -0.98913896163073145,
      -0.98956064401066235, -0.98996491037499179, -0.99035252402316576,
      -0.99072421365368857, -0.99108067494274388, -0.991422572052922,
      -0.99175053907484112, -0.992065181404365, -0.99236707705805072,
      -0.99265677792937179, -0.99293481098818936, -0.99320167942585869,
      -0.99345786374827993, -0.99370382281912406, -0.993939994855379,
      -0.994166798377293, -0.99438463311470626, -0.99459388087169109,
      -0.99479490635134526, -0.99498805794251355, -0.99517366847013711,
      -0.99535205591086628, -0.99552352407550415, -0.99568836325978294,
      -0.99584685086491265, -0.995999251989283, -0.9961458199926353,
      -0.99628679703397427, -0.99642241458442127, -0.99655289391617208,
      -0.99667844656866134, -0.99679927479299024, -0.99691557197563052,
      -0.997027523042365, -0.9971353048433903, -0.99723908652045823,
      -0.99733902985689771, -0.99743528961131778, -0.99752801383575751,
      -0.997617344179012, -0.99770341617582992, -0.99778635952264927,
      -0.99786629834050145, -0.99794335142568913, -0.99801763248881414,
      -0.99808925038270235, -0.99815830931975025, -0.998224909079191,
      -0.99828914520475542, -0.99835110919318037, -0.99841088867399586,
      -0.99846856758100255, -0.99852422631583, -0.99857794190395066,
      -0.9986297881435019, -0.99867983574725838, -0.99872815247807323,
      -0.99877480327809876, -0.99881985039207588, -0.99886335348497335,
      -0.99890536975424027, -0.9989459540369261, -0.99898515891190687,
      -0.99902303479744847, -0.99905963004432374, -0.99909499102469246,
      -0.9991291622169407, -0.9991621862866692, -0.99919410416400867,
      -0.99922495511743492, -0.99925477682424457, -0.99928360543784633,
      -0.99931147565201761, -0.99933842076226365, -0.99936447272441609,
      -0.99938966221059489, -0.999414018662659, -0.99943757034325764,
      -0.99946034438459452, -0.999482366835008, -0.99950366270346935,
      -0.99952425600209038, -0.9995441697867361, -0.99956342619582284,
      -0.99958204648738835, -0.99960005107451022, -0.9996174595591466,
      -0.99963429076447086, -0.99965056276576825, -0.999666292919957,
      -0.99968149789379757, -0.99969619369084484, -0.99971039567720421,
      -0.99972411860613764, -0.99973737664157658, -0.999750183380584,
      -0.99976255187481622, -0.999774494651025, -0.99978602373064018,
      -0.9997971506484773, -0.99980788647060137, -0.99981824181138723,
      -0.9998282268498091, -0.99983785134498993, -0.99984712465104431,
      -0.99985605573124148, -0.999864653171518, -0.99987292519336657,
      -0.9998808796661246, -0.99988852411868834, -0.99989586575067324,
      -0.99990291144304522, -0.99990966776823975, -0.99991614099979087,
      -0.99992233712148748, -0.99992826183607386, -0.99993392057351327,
      -0.99993931849882722, -0.99994446051952779, -0.99994935129265561,
      -0.99995399523143769, -0.9999583965115767, -0.999962559077185,
      -0.99996648664637144, -0.9999701827164964, -0.99997365056909882,
      -0.99997689327450989, -0.99997991369615824, -0.99998271449457654,
      -0.99998529813111547, -0.99998766687137342, -0.99998982278834747,
      -0.9999917677653114, -0.99999350349842586, -0.999995031499086,
      -0.99999635309601065, -0.99999746943707712, -0.99999838149090381,
      -0.9999990900481851, -0.99999959572278041, -0.99999989895255847, -1.0,
      -0.99999989895255847, -0.99999959572278041, -0.9999990900481851,
      -0.99999838149090381, -0.99999746943707712, -0.99999635309601065,
      -0.999995031499086, -0.99999350349842586, -0.9999917677653114,
      -0.99998982278834747, -0.99998766687137342, -0.99998529813111547,
      -0.99998271449457654, -0.99997991369615824, -0.99997689327450989,
      -0.99997365056909882, -0.9999701827164964, -0.99996648664637144,
      -0.999962559077185, -0.9999583965115767, -0.99995399523143769,
      -0.99994935129265561, -0.99994446051952779, -0.99993931849882722,
      -0.99993392057351327, -0.99992826183607386, -0.99992233712148748,
      -0.99991614099979087, -0.99990966776823975, -0.99990291144304522,
      -0.99989586575067324, -0.99988852411868834, -0.9998808796661246,
      -0.99987292519336657, -0.999864653171518, -0.99985605573124148,
      -0.99984712465104431, -0.99983785134498993, -0.9998282268498091,
      -0.99981824181138723, -0.99980788647060137, -0.9997971506484773,
      -0.99978602373064018, -0.999774494651025, -0.99976255187481622,
      -0.999750183380584, -0.99973737664157658, -0.99972411860613764,
      -0.99971039567720421, -0.99969619369084484, -0.99968149789379757,
      -0.999666292919957, -0.99965056276576825, -0.99963429076447086,
      -0.9996174595591466, -0.99960005107451022, -0.99958204648738835,
      -0.99956342619582284, -0.9995441697867361, -0.99952425600209038,
      -0.99950366270346935, -0.999482366835008, -0.99946034438459452,
      -0.99943757034325764, -0.999414018662659, -0.99938966221059489,
      -0.99936447272441609, -0.99933842076226365, -0.99931147565201761,
      -0.99928360543784633, -0.99925477682424457, -0.99922495511743492,
      -0.99919410416400867, -0.9991621862866692, -0.9991291622169407,
      -0.99909499102469246, -0.99905963004432374, -0.99902303479744847,
      -0.99898515891190687, -0.9989459540369261, -0.99890536975424027,
      -0.99886335348497335, -0.99881985039207588, -0.99877480327809876,
      -0.99872815247807323, -0.99867983574725838, -0.9986297881435019,
      -0.99857794190395066, -0.99852422631583, -0.99846856758100255,
      -0.99841088867399586, -0.99835110919318037, -0.99828914520475542,
      -0.998224909079191, -0.99815830931975025, -0.99808925038270235,
      -0.99801763248881414, -0.99794335142568913, -0.99786629834050145,
      -0.99778635952264927, -0.99770341617582992, -0.997617344179012,
      -0.99752801383575751, -0.99743528961131778, -0.99733902985689771,
      -0.99723908652045823, -0.99713530484339041, -0.997027523042365,
      -0.99691557197563052, -0.99679927479299024, -0.99667844656866134,
      -0.99655289391617208, -0.99642241458442127, -0.99628679703397427,
      -0.9961458199926353, -0.995999251989283, -0.99584685086491276,
      -0.99568836325978294, -0.99552352407550415, -0.99535205591086628,
      -0.99517366847013711, -0.99498805794251355, -0.99479490635134526,
      -0.99459388087169109, -0.99438463311470637, -0.994166798377293,
      -0.993939994855379, -0.99370382281912406, -0.99345786374827993,
      -0.99320167942585869, -0.99293481098818936, -0.9926567779293719,
      -0.99236707705805072, -0.992065181404365, -0.99175053907484112,
      -0.991422572052922, -0.99108067494274388, -0.99072421365368857,
      -0.99035252402316587, -0.98996491037499179, -0.98956064401066235,
      -0.98913896163073145, -0.98869906368344485, -0.98824011263769573,
      -0.9877612311773134, -0.98726150031363191, -0.98673995741322562,
      -0.98619559413766278, -0.98562735429208126, -0.98503413157936637,
      -0.98441476725669652, -0.98376804769121906, -0.983092701811638,
      -0.98238739845252132, -0.98165074358820059, -0.98088127745320586,
      -0.98007747154628844, -0.97923772551522092, -0.9783603639197338,
      -0.97744363287015412, -0.97648569653957074, -0.97548463354764114,
      -0.97443843321451351, -0.973344991683748, -0.97220210791359329,
      -0.97100747953652788, -0.96975869858759656, -0.96845324710279013,
      -0.967088492589518, -0.96566168337214209, -0.96416994381655741,
      -0.96261026943895689, -0.960979521905192, -0.95927442392857254,
      -0.957491554075524, -0.95562734149026474, -0.95367806055160154,
      -0.9516398254770515, -0.9495085848918281, -0.94728011638276166,
      -0.94495002106000536, -0.94251371815238061, -0.93996643966548443,
      -0.93730322513520437, -0.93451891651309782, -0.93160815322416268,
      -0.92856536744189344, -0.92538477963018273, -0.92206039440655563,
      -0.91858599678646313, -0.91495514887384655, -0.91116118706896188,
      -0.907197219870435, -0.90305612635473542, -0.89873055542262714,
      -0.8942129259086754, -0.88949542765645029, -0.884570023668638,
      -0.87942845344777987, -0.87406223764963931, -0.86846268417723016,
      -0.86262089584910384, -0.85652777978052486, -0.85017405862041606,
      -0.84355028379028718, -0.83664685087358659, -0.82945401730472568,
      -0.82196192250628075, -0.81416061062021272, -0.806040055974192,
      -0.79759019141686238, -0.78880093964594289, -0.77966224764004655,
      -0.77016412428880765, -0.7602966812959332, -0.75005017740596225,
      -0.73941506597760565, -0.72838204589423283, -0.71694211576540667,
      -0.70508663133209748, -0.69280736594255754, -0.6800965739156889,
      -0.666947056554555, -0.65335223051465829, -0.63930619817041656,
      -0.624803819559415, -0.60984078541845255, -0.5944136907591866,
      -0.57852010836532131, -0.56215866152935923, -0.5453290952862575,
      -0.52803234534577426, -0.51027060387633216, -0.49204738125301339,
      -0.47336756285239895, -0.45423745995952697, -0.43466485384870057,
      -0.41465903211204536, -0.39423081633901685, -0.37339258029731281,
      -0.35215825783191823, -0.33054333978428818, -0.30856485933808042,
      -0.28624136532028377, -0.26359288312614015, -0.24064086309071947,
      -0.21740811629729612, -0.19391873798944362, -0.17019801893684586,
      -0.14627234529039812, -0.12216908764579926, -0.097916480212640655,
      -0.073543491153236856, -0.049079685308304165, -0.024555080660630946 };

    /* Start for MATLABSystem: '<Root>/VCO' */
    /*  --- I/O */
    rmod_DW.obj.Phase = 0.0;
    memset(&rmod_DW.obj.WaveTables[0], 0, 40960U * sizeof(real_T));
    memset(&rmod_DW.obj.TablesSize[0], 0, 10U * sizeof(real_T));
    for (b_k = 0; b_k < 1024; b_k++) {
      rmod_B.sine[b_k] = sin(0.0061359231515425647 * (real_T)b_k);
    }

    for (b_k = 0; b_k < 5; b_k++) {
      rmod_DW.obj.TablesSize[b_k] = 1024.0;
    }

    /*  WTc: [L x 5] en [-1,1], un ciclo, periodicidad implícita */
    /*  ciclos [0,1) */
    /*  1) FM suave (rico pero continuo) */
    /*  2) Sine + distorsión cúbica (armónicos impares) */
    /*  3) Soft-square (tanh), menos “dientes” que un umbral duro */
    /*  4) Saw aditiva con roll-off exponencial (reduce aspereza) */
    for (b_k = 0; b_k < 1024; b_k++) {
      rmod_DW.obj.WaveTables[b_k] = rmod_B.sine[b_k];
      rmod_DW.obj.WaveTables[b_k + 4096] = tmp[b_k];
      rmod_DW.obj.WaveTables[b_k + 8192] = tmp_0[b_k];
      rmod_DW.obj.WaveTables[b_k + 12288] = 0.001953125 * (real_T)b_k - 1.0;
      rmod_DW.obj.WaveTables[b_k + 16384] = -0.001953125 * (real_T)b_k + 1.0;
      rmod_B.w4[b_k] = 0.0;
    }

    for (b_k = 0; b_k < 64; b_k++) {
      a = ((real_T)b_k + 1.0) * 6.2831853071795862;
      a_0 = 1.0 / ((real_T)b_k + 1.0) * exp(-((real_T)b_k + 1.0) / 20.0);
      for (b_k_0 = 0; b_k_0 < 1024; b_k_0++) {
        rmod_B.w4[b_k_0] += sin(0.0009765625 * (real_T)b_k_0 * a) * a_0;
      }
    }

    /*  5) “Organ” (impares), roll-off 1/k */
    memset(&rmod_B.w5[0], 0, sizeof(real_T) << 10U);
    for (b_k_0 = 0; b_k_0 < 32; b_k_0++) {
      b_k = (b_k_0 << 1) + 1;
      a = 6.2831853071795862 * (real_T)b_k;
      a_0 = 1.0 / (real_T)b_k;
      for (b_k = 0; b_k < 1024; b_k++) {
        rmod_B.w5[b_k] += sin(0.0009765625 * (real_T)b_k * a) * a_0;
      }
    }

    for (b_k = 0; b_k < 1024; b_k++) {
      rmod_B.sine[b_k] = fabs(rmod_B.w4[b_k]);
    }

    a = rmod_B.sine[0];
    for (b_k = 0; b_k < 1023; b_k++) {
      a_0 = rmod_B.sine[b_k + 1];
      if (a < a_0) {
        a = a_0;
      }
    }

    for (b_k = 0; b_k < 1024; b_k++) {
      if (a > 0.0) {
        rmod_B.w4[b_k] /= a;
      }

      rmod_B.sine[b_k] = fabs(rmod_B.w5[b_k]);
    }

    a = rmod_B.sine[0];
    for (b_k = 0; b_k < 1023; b_k++) {
      a_0 = rmod_B.sine[b_k + 1];
      if (a < a_0) {
        a = a_0;
      }
    }

    /*  [L x 5] */
    for (b_k = 0; b_k < 1024; b_k++) {
      if (a > 0.0) {
        rmod_B.w5[b_k] /= a;
      }

      rmod_DW.obj.WaveTables[b_k + 20480] = tmp_1[b_k];
      rmod_DW.obj.WaveTables[b_k + 24576] = tmp_2[b_k];
      rmod_DW.obj.WaveTables[b_k + 28672] = tmp_3[b_k];
      rmod_DW.obj.WaveTables[b_k + 32768] = rmod_B.w4[b_k];
      rmod_DW.obj.WaveTables[b_k + 36864] = rmod_B.w5[b_k];
    }

    for (b_k = 0; b_k < 5; b_k++) {
      rmod_DW.obj.TablesSize[b_k + 5] = 1024.0;
    }

    /* End of Start for MATLABSystem: '<Root>/VCO' */

    /* Start for MATLABSystem: '<Root>/MIX' */
    rmod_DW.obj_a.SmoothTime = rmod_P.MIX_SmoothTime;
    rmod_DW.obj_a.IndepInDB = rmod_P.MIX_IndepInDB;
    rmod_DW.obj_a.isInitialized = 1;

    /*  inL,inR,wetL,wetR,p1 (+p2 si Mode=2) */
    if (rmod_DW.obj_a.SmoothTime <= 0.0) {
      rmod_DW.obj_a.a = 1.0;
    } else {
      rmod_DW.obj_a.a = 1.0 - exp(-1.0 / (48000.0 * rmod_DW.obj_a.SmoothTime));
    }

    /* InitializeConditions for MATLABSystem: '<Root>/MIX' */
    rmod_DW.obj_a.gd1 = 1.0;
    rmod_DW.obj_a.gd2 = 1.0;
    rmod_DW.obj_a.gw1 = 0.0;
    rmod_DW.obj_a.gw2 = 0.0;
  }
}

/* Model terminate function */
void rmod_terminate(void)
{
  /* Terminate for S-Function (ctrl_in): '<Root>/Control Input' */
  /* Level2 S-Function Block: '<Root>/Control Input' (ctrl_in) */
  {
    SimStruct *rts = rmod_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (pisound_in): '<Root>/PiSound Input' */
  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = rmod_M->childSfunctions[1];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (pisound_out): '<Root>/PiSound Output' */
  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = rmod_M->childSfunctions[2];
    sfcnTerminate(rts);
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
