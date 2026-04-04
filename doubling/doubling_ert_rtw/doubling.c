/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: doubling.c
 *
 * Code generated for Simulink model 'doubling'.
 *
 * Model version                  : 1.8
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Sat Mar 28 11:36:35 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-A (64-bit)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "doubling.h"
#include <math.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "doubling_private.h"

/* Block signals (default storage) */
B_doubling_T doubling_B;

/* Block states (default storage) */
DW_doubling_T doubling_DW;

/* Real-time model */
static RT_MODEL_doubling_T doubling_M_;
RT_MODEL_doubling_T *const doubling_M = &doubling_M_;

/* Model step function */
void doubling_step(void)
{
  real_T rtb_Product[128];
  real_T Delay_L_DSTATE;
  real_T tmp;
  real_T tmp_0;
  int32_T i;

  /* S-Function (ctrl_in): '<Root>/S-Function' */

  /* Level2 S-Function Block: '<Root>/S-Function' (ctrl_in) */
  {
    SimStruct *rts = doubling_M->childSfunctions[0];
    sfcnOutputs(rts,0);
  }

  /* Product: '<Root>/Product' incorporates:
   *  UnitDelay: '<Root>/Delay_L'
   */
  for (i = 0; i < 128; i++) {
    rtb_Product[i] = doubling_B.SFunction_o2 * doubling_DW.Delay_L_DSTATE[i];
  }

  /* End of Product: '<Root>/Product' */

  /* S-Function (pisound_in): '<Root>/pisound_in' */

  /* Level2 S-Function Block: '<Root>/pisound_in' (pisound_in) */
  {
    SimStruct *rts = doubling_M->childSfunctions[1];
    sfcnOutputs(rts,0);
  }

  /* Gain: '<Root>/Gain_R_Spread' */
  tmp = doubling_P.Spread / 2.0 + 1.0;
  for (i = 0; i < 128; i++) {
    /* Gain: '<Root>/Gain_In_L' incorporates:
     *  UnitDelay: '<Root>/Delay_L'
     */
    Delay_L_DSTATE = (real_T)doubling_P.Gain_In_L_Gain * 2.1684043449710089E-19 *
      (real_T)doubling_B.pisound_in_o1[i];
    doubling_DW.Delay_L_DSTATE[i] = Delay_L_DSTATE;

    /* Sum: '<Root>/Mix' */
    Delay_L_DSTATE += rtb_Product[i];
    rtb_Product[i] = Delay_L_DSTATE;

    /* DataTypeConversion: '<Root>/DTC_Out_L' incorporates:
     *  Gain: '<Root>/Gain_Out_L'
     *  Product: '<Root>/Product1'
     */
    tmp_0 = floor(doubling_B.SFunction_o3 * Delay_L_DSTATE *
                  doubling_P.Gain_Out_L_Gain);
    if (rtIsNaN(tmp_0) || rtIsInf(tmp_0)) {
      tmp_0 = 0.0;
    } else {
      tmp_0 = fmod(tmp_0, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/DTC_Out_L' */
    doubling_B.DTC_Out_L[i] = tmp_0 < 0.0 ? -(int32_T)(uint32_T)-tmp_0 :
      (int32_T)(uint32_T)tmp_0;

    /* DataTypeConversion: '<Root>/DTC_Out_R' incorporates:
     *  Gain: '<Root>/Gain_Out_R'
     *  Gain: '<Root>/Gain_R_Spread'
     */
    tmp_0 = floor(tmp * Delay_L_DSTATE * doubling_P.Gain_Out_R_Gain);
    if (rtIsNaN(tmp_0) || rtIsInf(tmp_0)) {
      tmp_0 = 0.0;
    } else {
      tmp_0 = fmod(tmp_0, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/DTC_Out_R' */
    doubling_B.DTC_Out_R[i] = tmp_0 < 0.0 ? -(int32_T)(uint32_T)-tmp_0 :
      (int32_T)(uint32_T)tmp_0;
  }

  /* S-Function (ctrl_out): '<Root>/S-Function1' */

  /* Level2 S-Function Block: '<Root>/S-Function1' (ctrl_out) */
  {
    SimStruct *rts = doubling_M->childSfunctions[2];
    sfcnOutputs(rts,0);
  }

  /* S-Function (pisound_out): '<Root>/pisound_out' */

  /* Level2 S-Function Block: '<Root>/pisound_out' (pisound_out) */
  {
    SimStruct *rts = doubling_M->childSfunctions[3];
    sfcnOutputs(rts,0);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  doubling_M->Timing.t[0] =
    ((time_T)(++doubling_M->Timing.clockTick0)) * doubling_M->Timing.stepSize0;
}

/* Model initialize function */
void doubling_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));
  rtsiSetSolverName(&doubling_M->solverInfo,"FixedStepDiscrete");
  doubling_M->solverInfoPtr = (&doubling_M->solverInfo);

  /* Initialize timing info */
  {
    int_T *mdlTsMap = doubling_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    doubling_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    doubling_M->Timing.sampleTimes = (&doubling_M->Timing.sampleTimesArray[0]);
    doubling_M->Timing.offsetTimes = (&doubling_M->Timing.offsetTimesArray[0]);

    /* task periods */
    doubling_M->Timing.sampleTimes[0] = (0.0026666666666666666);

    /* task offsets */
    doubling_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(doubling_M, &doubling_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = doubling_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    doubling_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(doubling_M, -1);
  doubling_M->Timing.stepSize0 = 0.0026666666666666666;
  doubling_M->solverInfoPtr = (&doubling_M->solverInfo);
  doubling_M->Timing.stepSize = (0.0026666666666666666);
  rtsiSetFixedStepSize(&doubling_M->solverInfo, 0.0026666666666666666);
  rtsiSetSolverMode(&doubling_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* child S-Function registration */
  {
    RTWSfcnInfo *sfcnInfo = &doubling_M->NonInlinedSFcns.sfcnInfo;
    doubling_M->sfcnInfo = (sfcnInfo);
    rtssSetErrorStatusPtr(sfcnInfo, (&rtmGetErrorStatus(doubling_M)));
    doubling_M->Sizes.numSampTimes = (1);
    rtssSetNumRootSampTimesPtr(sfcnInfo, &doubling_M->Sizes.numSampTimes);
    doubling_M->NonInlinedSFcns.taskTimePtrs[0] = (&rtmGetTPtr(doubling_M)[0]);
    rtssSetTPtrPtr(sfcnInfo,doubling_M->NonInlinedSFcns.taskTimePtrs);
    rtssSetTStartPtr(sfcnInfo, &rtmGetTStart(doubling_M));
    rtssSetTFinalPtr(sfcnInfo, &rtmGetTFinal(doubling_M));
    rtssSetTimeOfLastOutputPtr(sfcnInfo, &rtmGetTimeOfLastOutput(doubling_M));
    rtssSetStepSizePtr(sfcnInfo, &doubling_M->Timing.stepSize);
    rtssSetStopRequestedPtr(sfcnInfo, &rtmGetStopRequested(doubling_M));
    rtssSetDerivCacheNeedsResetPtr(sfcnInfo, &doubling_M->derivCacheNeedsReset);
    rtssSetZCCacheNeedsResetPtr(sfcnInfo, &doubling_M->zCCacheNeedsReset);
    rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(sfcnInfo,
      &doubling_M->CTOutputIncnstWithState);
    rtssSetSampleHitsPtr(sfcnInfo, &doubling_M->Timing.sampleHits);
    rtssSetPerTaskSampleHitsPtr(sfcnInfo, &doubling_M->Timing.perTaskSampleHits);
    rtssSetSimModePtr(sfcnInfo, &doubling_M->simMode);
    rtssSetSolverInfoPtr(sfcnInfo, &doubling_M->solverInfoPtr);
  }

  doubling_M->Sizes.numSFcns = (4);

  /* register each child */
  {
    (void) memset((void *)&doubling_M->NonInlinedSFcns.childSFunctions[0], 0,
                  4*sizeof(SimStruct));
    doubling_M->childSfunctions =
      (&doubling_M->NonInlinedSFcns.childSFunctionPtrs[0]);
    doubling_M->childSfunctions[0] =
      (&doubling_M->NonInlinedSFcns.childSFunctions[0]);
    doubling_M->childSfunctions[1] =
      (&doubling_M->NonInlinedSFcns.childSFunctions[1]);
    doubling_M->childSfunctions[2] =
      (&doubling_M->NonInlinedSFcns.childSFunctions[2]);
    doubling_M->childSfunctions[3] =
      (&doubling_M->NonInlinedSFcns.childSFunctions[3]);

    /* Level2 S-Function Block: doubling/<Root>/S-Function (ctrl_in) */
    {
      SimStruct *rts = doubling_M->childSfunctions[0];

      /* timing info */
      time_T *sfcnPeriod = doubling_M->NonInlinedSFcns.Sfcn0.sfcnPeriod;
      time_T *sfcnOffset = doubling_M->NonInlinedSFcns.Sfcn0.sfcnOffset;
      int_T *sfcnTsMap = doubling_M->NonInlinedSFcns.Sfcn0.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &doubling_M->NonInlinedSFcns.blkInfo2[0]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &doubling_M->NonInlinedSFcns.inputOutputPortInfo2[0]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, doubling_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &doubling_M->NonInlinedSFcns.methods2[0]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &doubling_M->NonInlinedSFcns.methods3[0]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &doubling_M->NonInlinedSFcns.methods4[0]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &doubling_M->NonInlinedSFcns.statesInfo2[0]);
        ssSetPeriodicStatesInfo(rts,
          &doubling_M->NonInlinedSFcns.periodicStatesInfo[0]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &doubling_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &doubling_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 4);
        _ssSetPortInfo2ForOutputUnits(rts,
          &doubling_M->NonInlinedSFcns.Sfcn0.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        ssSetOutputPortUnit(rts, 2, 0);
        ssSetOutputPortUnit(rts, 3, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &doubling_M->NonInlinedSFcns.Sfcn0.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 2, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 3, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 1);
          ssSetOutputPortSignal(rts, 0, ((real_T *) &doubling_B.SFunction_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 1);
          ssSetOutputPortSignal(rts, 1, ((real_T *) &doubling_B.SFunction_o2));
        }

        /* port 2 */
        {
          _ssSetOutputPortNumDimensions(rts, 2, 1);
          ssSetOutputPortWidthAsInt(rts, 2, 1);
          ssSetOutputPortSignal(rts, 2, ((real_T *) &doubling_B.SFunction_o3));
        }

        /* port 3 */
        {
          _ssSetOutputPortNumDimensions(rts, 3, 1);
          ssSetOutputPortWidthAsInt(rts, 3, 1);
          ssSetOutputPortSignal(rts, 3, ((real_T *) &doubling_B.SFunction_o4));
        }
      }

      /* path info */
      ssSetModelName(rts, "S-Function");
      ssSetPath(rts, "doubling/S-Function");
      ssSetRTModel(rts,doubling_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &doubling_M->NonInlinedSFcns.Sfcn0.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)doubling_P.SFunction_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)doubling_P.SFunction_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)doubling_P.SFunction_P3_Size);
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

    /* Level2 S-Function Block: doubling/<Root>/pisound_in (pisound_in) */
    {
      SimStruct *rts = doubling_M->childSfunctions[1];

      /* timing info */
      time_T *sfcnPeriod = doubling_M->NonInlinedSFcns.Sfcn1.sfcnPeriod;
      time_T *sfcnOffset = doubling_M->NonInlinedSFcns.Sfcn1.sfcnOffset;
      int_T *sfcnTsMap = doubling_M->NonInlinedSFcns.Sfcn1.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &doubling_M->NonInlinedSFcns.blkInfo2[1]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &doubling_M->NonInlinedSFcns.inputOutputPortInfo2[1]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, doubling_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &doubling_M->NonInlinedSFcns.methods2[1]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &doubling_M->NonInlinedSFcns.methods3[1]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &doubling_M->NonInlinedSFcns.methods4[1]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &doubling_M->NonInlinedSFcns.statesInfo2[1]);
        ssSetPeriodicStatesInfo(rts,
          &doubling_M->NonInlinedSFcns.periodicStatesInfo[1]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &doubling_M->NonInlinedSFcns.Sfcn1.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &doubling_M->NonInlinedSFcns.Sfcn1.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &doubling_M->NonInlinedSFcns.Sfcn1.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &doubling_M->NonInlinedSFcns.Sfcn1.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 128);
          ssSetOutputPortSignal(rts, 0, ((int32_T *) doubling_B.pisound_in_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 128);
          ssSetOutputPortSignal(rts, 1, ((int32_T *) doubling_B.pisound_in_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "pisound_in");
      ssSetPath(rts, "doubling/pisound_in");
      ssSetRTModel(rts,doubling_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &doubling_M->NonInlinedSFcns.Sfcn1.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)doubling_P.pisound_in_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)doubling_P.pisound_in_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)doubling_P.pisound_in_P3_Size);
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

    /* Level2 S-Function Block: doubling/<Root>/S-Function1 (ctrl_out) */
    {
      SimStruct *rts = doubling_M->childSfunctions[2];

      /* timing info */
      time_T *sfcnPeriod = doubling_M->NonInlinedSFcns.Sfcn2.sfcnPeriod;
      time_T *sfcnOffset = doubling_M->NonInlinedSFcns.Sfcn2.sfcnOffset;
      int_T *sfcnTsMap = doubling_M->NonInlinedSFcns.Sfcn2.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &doubling_M->NonInlinedSFcns.blkInfo2[2]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &doubling_M->NonInlinedSFcns.inputOutputPortInfo2[2]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, doubling_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &doubling_M->NonInlinedSFcns.methods2[2]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &doubling_M->NonInlinedSFcns.methods3[2]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &doubling_M->NonInlinedSFcns.methods4[2]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &doubling_M->NonInlinedSFcns.statesInfo2[2]);
        ssSetPeriodicStatesInfo(rts,
          &doubling_M->NonInlinedSFcns.periodicStatesInfo[2]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 4);
        ssSetPortInfoForInputs(rts,
          &doubling_M->NonInlinedSFcns.Sfcn2.inputPortInfo[0]);
        ssSetPortInfoForInputs(rts,
          &doubling_M->NonInlinedSFcns.Sfcn2.inputPortInfo[0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &doubling_M->NonInlinedSFcns.Sfcn2.inputPortUnits[0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        ssSetInputPortUnit(rts, 2, 0);
        ssSetInputPortUnit(rts, 3, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &doubling_M->NonInlinedSFcns.Sfcn2.inputPortCoSimAttribute[0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);
        ssSetInputPortIsContinuousQuantity(rts, 2, 0);
        ssSetInputPortIsContinuousQuantity(rts, 3, 0);

        /* port 0 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &doubling_M->NonInlinedSFcns.Sfcn2.UPtrs0;
          sfcnUPtrs[0] = &doubling_B.SFunction_o1;
          ssSetInputPortSignalPtrs(rts, 0, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidthAsInt(rts, 0, 1);
        }

        /* port 1 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &doubling_M->NonInlinedSFcns.Sfcn2.UPtrs1;
          sfcnUPtrs[0] = &doubling_B.SFunction_o2;
          ssSetInputPortSignalPtrs(rts, 1, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 1, 1);
          ssSetInputPortWidthAsInt(rts, 1, 1);
        }

        /* port 2 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &doubling_M->NonInlinedSFcns.Sfcn2.UPtrs2;
          sfcnUPtrs[0] = &doubling_B.SFunction_o3;
          ssSetInputPortSignalPtrs(rts, 2, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 2, 1);
          ssSetInputPortWidthAsInt(rts, 2, 1);
        }

        /* port 3 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &doubling_M->NonInlinedSFcns.Sfcn2.UPtrs3;
          sfcnUPtrs[0] = &doubling_B.SFunction_o4;
          ssSetInputPortSignalPtrs(rts, 3, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 3, 1);
          ssSetInputPortWidthAsInt(rts, 3, 1);
        }
      }

      /* path info */
      ssSetModelName(rts, "S-Function1");
      ssSetPath(rts, "doubling/S-Function1");
      ssSetRTModel(rts,doubling_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &doubling_M->NonInlinedSFcns.Sfcn2.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)doubling_P.SFunction1_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)doubling_P.SFunction1_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)doubling_P.SFunction1_P3_Size);
      }

      /* registration */
      ctrl_out(rts);
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

    /* Level2 S-Function Block: doubling/<Root>/pisound_out (pisound_out) */
    {
      SimStruct *rts = doubling_M->childSfunctions[3];

      /* timing info */
      time_T *sfcnPeriod = doubling_M->NonInlinedSFcns.Sfcn3.sfcnPeriod;
      time_T *sfcnOffset = doubling_M->NonInlinedSFcns.Sfcn3.sfcnOffset;
      int_T *sfcnTsMap = doubling_M->NonInlinedSFcns.Sfcn3.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &doubling_M->NonInlinedSFcns.blkInfo2[3]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &doubling_M->NonInlinedSFcns.inputOutputPortInfo2[3]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, doubling_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &doubling_M->NonInlinedSFcns.methods2[3]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &doubling_M->NonInlinedSFcns.methods3[3]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &doubling_M->NonInlinedSFcns.methods4[3]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &doubling_M->NonInlinedSFcns.statesInfo2[3]);
        ssSetPeriodicStatesInfo(rts,
          &doubling_M->NonInlinedSFcns.periodicStatesInfo[3]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts,
          &doubling_M->NonInlinedSFcns.Sfcn3.inputPortInfo[0]);
        ssSetPortInfoForInputs(rts,
          &doubling_M->NonInlinedSFcns.Sfcn3.inputPortInfo[0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &doubling_M->NonInlinedSFcns.Sfcn3.inputPortUnits[0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &doubling_M->NonInlinedSFcns.Sfcn3.inputPortCoSimAttribute[0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          int32_T const **sfcnUPtrs = (int32_T const **)
            &doubling_M->NonInlinedSFcns.Sfcn3.UPtrs0;

          {
            int_T i1;
            const int32_T *u0 = doubling_B.DTC_Out_L;
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
            &doubling_M->NonInlinedSFcns.Sfcn3.UPtrs1;

          {
            int_T i1;
            const int32_T *u1 = doubling_B.DTC_Out_R;
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
      ssSetModelName(rts, "pisound_out");
      ssSetPath(rts, "doubling/pisound_out");
      ssSetRTModel(rts,doubling_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &doubling_M->NonInlinedSFcns.Sfcn3.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)doubling_P.pisound_out_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)doubling_P.pisound_out_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)doubling_P.pisound_out_P3_Size);
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
    int32_T i;

    /* InitializeConditions for UnitDelay: '<Root>/Delay_L' */
    for (i = 0; i < 128; i++) {
      doubling_DW.Delay_L_DSTATE[i] = doubling_P.Delay_L_InitialCondition;
    }

    /* End of InitializeConditions for UnitDelay: '<Root>/Delay_L' */
  }
}

/* Model terminate function */
void doubling_terminate(void)
{
  /* Terminate for S-Function (ctrl_in): '<Root>/S-Function' */
  /* Level2 S-Function Block: '<Root>/S-Function' (ctrl_in) */
  {
    SimStruct *rts = doubling_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (pisound_in): '<Root>/pisound_in' */
  /* Level2 S-Function Block: '<Root>/pisound_in' (pisound_in) */
  {
    SimStruct *rts = doubling_M->childSfunctions[1];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (ctrl_out): '<Root>/S-Function1' */
  /* Level2 S-Function Block: '<Root>/S-Function1' (ctrl_out) */
  {
    SimStruct *rts = doubling_M->childSfunctions[2];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (pisound_out): '<Root>/pisound_out' */
  /* Level2 S-Function Block: '<Root>/pisound_out' (pisound_out) */
  {
    SimStruct *rts = doubling_M->childSfunctions[3];
    sfcnTerminate(rts);
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
