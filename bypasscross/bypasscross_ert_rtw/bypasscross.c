/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: bypasscross.c
 *
 * Code generated for Simulink model 'bypasscross'.
 *
 * Model version                  : 1.7
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Fri Apr 24 18:14:16 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "bypasscross.h"
#include "bypasscross_private.h"

/* Block signals (default storage) */
B_bypasscross_T bypasscross_B;

/* Real-time model */
static RT_MODEL_bypasscross_T bypasscross_M_;
RT_MODEL_bypasscross_T *const bypasscross_M = &bypasscross_M_;

/* Model step function */
void bypasscross_step(void)
{
  /* S-Function (pisound_in): '<Root>/Pisound Input' */

  /* Level2 S-Function Block: '<Root>/Pisound Input' (pisound_in) */
  {
    SimStruct *rts = bypasscross_M->childSfunctions[0];
    sfcnOutputs(rts,0);
  }

  /* S-Function (stream_in): '<Root>/Stream Input' */

  /* Level2 S-Function Block: '<Root>/Stream Input' (stream_in) */
  {
    SimStruct *rts = bypasscross_M->childSfunctions[1];
    sfcnOutputs(rts,0);
  }

  /* S-Function (stream_out): '<Root>/Stream Output' */

  /* Level2 S-Function Block: '<Root>/Stream Output' (stream_out) */
  {
    SimStruct *rts = bypasscross_M->childSfunctions[2];
    sfcnOutputs(rts,0);
  }

  /* S-Function (pisound_out): '<Root>/Stream Output1' */

  /* Level2 S-Function Block: '<Root>/Stream Output1' (pisound_out) */
  {
    SimStruct *rts = bypasscross_M->childSfunctions[3];
    sfcnOutputs(rts,0);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  bypasscross_M->Timing.t[0] =
    ((time_T)(++bypasscross_M->Timing.clockTick0)) *
    bypasscross_M->Timing.stepSize0;
}

/* Model initialize function */
void bypasscross_initialize(void)
{
  /* Registration code */
  rtsiSetSolverName(&bypasscross_M->solverInfo,"FixedStepDiscrete");
  bypasscross_M->solverInfoPtr = (&bypasscross_M->solverInfo);

  /* Initialize timing info */
  {
    int_T *mdlTsMap = bypasscross_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    bypasscross_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    bypasscross_M->Timing.sampleTimes = (&bypasscross_M->
      Timing.sampleTimesArray[0]);
    bypasscross_M->Timing.offsetTimes = (&bypasscross_M->
      Timing.offsetTimesArray[0]);

    /* task periods */
    bypasscross_M->Timing.sampleTimes[0] = (0.0026666666666666666);

    /* task offsets */
    bypasscross_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(bypasscross_M, &bypasscross_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = bypasscross_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    bypasscross_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(bypasscross_M, -1);
  bypasscross_M->Timing.stepSize0 = 0.0026666666666666666;
  bypasscross_M->solverInfoPtr = (&bypasscross_M->solverInfo);
  bypasscross_M->Timing.stepSize = (0.0026666666666666666);
  rtsiSetFixedStepSize(&bypasscross_M->solverInfo, 0.0026666666666666666);
  rtsiSetSolverMode(&bypasscross_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* child S-Function registration */
  {
    RTWSfcnInfo *sfcnInfo = &bypasscross_M->NonInlinedSFcns.sfcnInfo;
    bypasscross_M->sfcnInfo = (sfcnInfo);
    rtssSetErrorStatusPtr(sfcnInfo, (&rtmGetErrorStatus(bypasscross_M)));
    bypasscross_M->Sizes.numSampTimes = (1);
    rtssSetNumRootSampTimesPtr(sfcnInfo, &bypasscross_M->Sizes.numSampTimes);
    bypasscross_M->NonInlinedSFcns.taskTimePtrs[0] = (&rtmGetTPtr(bypasscross_M)
      [0]);
    rtssSetTPtrPtr(sfcnInfo,bypasscross_M->NonInlinedSFcns.taskTimePtrs);
    rtssSetTStartPtr(sfcnInfo, &rtmGetTStart(bypasscross_M));
    rtssSetTFinalPtr(sfcnInfo, &rtmGetTFinal(bypasscross_M));
    rtssSetTimeOfLastOutputPtr(sfcnInfo, &rtmGetTimeOfLastOutput(bypasscross_M));
    rtssSetStepSizePtr(sfcnInfo, &bypasscross_M->Timing.stepSize);
    rtssSetStopRequestedPtr(sfcnInfo, &rtmGetStopRequested(bypasscross_M));
    rtssSetDerivCacheNeedsResetPtr(sfcnInfo,
      &bypasscross_M->derivCacheNeedsReset);
    rtssSetZCCacheNeedsResetPtr(sfcnInfo, &bypasscross_M->zCCacheNeedsReset);
    rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(sfcnInfo,
      &bypasscross_M->CTOutputIncnstWithState);
    rtssSetSampleHitsPtr(sfcnInfo, &bypasscross_M->Timing.sampleHits);
    rtssSetPerTaskSampleHitsPtr(sfcnInfo,
      &bypasscross_M->Timing.perTaskSampleHits);
    rtssSetSimModePtr(sfcnInfo, &bypasscross_M->simMode);
    rtssSetSolverInfoPtr(sfcnInfo, &bypasscross_M->solverInfoPtr);
  }

  bypasscross_M->Sizes.numSFcns = (4);

  /* register each child */
  {
    (void) memset((void *)&bypasscross_M->NonInlinedSFcns.childSFunctions[0], 0,
                  4*sizeof(SimStruct));
    bypasscross_M->childSfunctions =
      (&bypasscross_M->NonInlinedSFcns.childSFunctionPtrs[0]);
    bypasscross_M->childSfunctions[0] =
      (&bypasscross_M->NonInlinedSFcns.childSFunctions[0]);
    bypasscross_M->childSfunctions[1] =
      (&bypasscross_M->NonInlinedSFcns.childSFunctions[1]);
    bypasscross_M->childSfunctions[2] =
      (&bypasscross_M->NonInlinedSFcns.childSFunctions[2]);
    bypasscross_M->childSfunctions[3] =
      (&bypasscross_M->NonInlinedSFcns.childSFunctions[3]);

    /* Level2 S-Function Block: bypasscross/<Root>/Pisound Input (pisound_in) */
    {
      SimStruct *rts = bypasscross_M->childSfunctions[0];

      /* timing info */
      time_T *sfcnPeriod = bypasscross_M->NonInlinedSFcns.Sfcn0.sfcnPeriod;
      time_T *sfcnOffset = bypasscross_M->NonInlinedSFcns.Sfcn0.sfcnOffset;
      int_T *sfcnTsMap = bypasscross_M->NonInlinedSFcns.Sfcn0.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &bypasscross_M->NonInlinedSFcns.blkInfo2[0]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &bypasscross_M->NonInlinedSFcns.inputOutputPortInfo2[0]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, bypasscross_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &bypasscross_M->NonInlinedSFcns.methods2[0]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &bypasscross_M->NonInlinedSFcns.methods3[0]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &bypasscross_M->NonInlinedSFcns.methods4[0]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &bypasscross_M->NonInlinedSFcns.statesInfo2[0]);
        ssSetPeriodicStatesInfo(rts,
          &bypasscross_M->NonInlinedSFcns.periodicStatesInfo[0]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &bypasscross_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &bypasscross_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &bypasscross_M->NonInlinedSFcns.Sfcn0.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &bypasscross_M->NonInlinedSFcns.Sfcn0.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 128);
          ssSetOutputPortSignal(rts, 0, ((int32_T *)
            bypasscross_B.PisoundInput_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 128);
          ssSetOutputPortSignal(rts, 1, ((int32_T *)
            bypasscross_B.PisoundInput_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "Pisound Input");
      ssSetPath(rts, "bypasscross/Pisound Input");
      ssSetRTModel(rts,bypasscross_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &bypasscross_M->NonInlinedSFcns.Sfcn0.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)bypasscross_ConstP.PisoundInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)bypasscross_ConstP.PisoundInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)bypasscross_ConstP.PisoundInput_P3_Size);
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

    /* Level2 S-Function Block: bypasscross/<Root>/Stream Input (stream_in) */
    {
      SimStruct *rts = bypasscross_M->childSfunctions[1];

      /* timing info */
      time_T *sfcnPeriod = bypasscross_M->NonInlinedSFcns.Sfcn1.sfcnPeriod;
      time_T *sfcnOffset = bypasscross_M->NonInlinedSFcns.Sfcn1.sfcnOffset;
      int_T *sfcnTsMap = bypasscross_M->NonInlinedSFcns.Sfcn1.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &bypasscross_M->NonInlinedSFcns.blkInfo2[1]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &bypasscross_M->NonInlinedSFcns.inputOutputPortInfo2[1]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, bypasscross_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &bypasscross_M->NonInlinedSFcns.methods2[1]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &bypasscross_M->NonInlinedSFcns.methods3[1]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &bypasscross_M->NonInlinedSFcns.methods4[1]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &bypasscross_M->NonInlinedSFcns.statesInfo2[1]);
        ssSetPeriodicStatesInfo(rts,
          &bypasscross_M->NonInlinedSFcns.periodicStatesInfo[1]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &bypasscross_M->NonInlinedSFcns.Sfcn1.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &bypasscross_M->NonInlinedSFcns.Sfcn1.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &bypasscross_M->NonInlinedSFcns.Sfcn1.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &bypasscross_M->NonInlinedSFcns.Sfcn1.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 128);
          ssSetOutputPortSignal(rts, 0, ((int32_T *)
            bypasscross_B.StreamInput_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 128);
          ssSetOutputPortSignal(rts, 1, ((int32_T *)
            bypasscross_B.StreamInput_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "Stream Input");
      ssSetPath(rts, "bypasscross/Stream Input");
      ssSetRTModel(rts,bypasscross_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &bypasscross_M->NonInlinedSFcns.Sfcn1.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)bypasscross_ConstP.PisoundInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)bypasscross_ConstP.PisoundInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)bypasscross_ConstP.PisoundInput_P3_Size);
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

    /* Level2 S-Function Block: bypasscross/<Root>/Stream Output (stream_out) */
    {
      SimStruct *rts = bypasscross_M->childSfunctions[2];

      /* timing info */
      time_T *sfcnPeriod = bypasscross_M->NonInlinedSFcns.Sfcn2.sfcnPeriod;
      time_T *sfcnOffset = bypasscross_M->NonInlinedSFcns.Sfcn2.sfcnOffset;
      int_T *sfcnTsMap = bypasscross_M->NonInlinedSFcns.Sfcn2.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &bypasscross_M->NonInlinedSFcns.blkInfo2[2]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &bypasscross_M->NonInlinedSFcns.inputOutputPortInfo2[2]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, bypasscross_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &bypasscross_M->NonInlinedSFcns.methods2[2]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &bypasscross_M->NonInlinedSFcns.methods3[2]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &bypasscross_M->NonInlinedSFcns.methods4[2]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &bypasscross_M->NonInlinedSFcns.statesInfo2[2]);
        ssSetPeriodicStatesInfo(rts,
          &bypasscross_M->NonInlinedSFcns.periodicStatesInfo[2]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts,
          &bypasscross_M->NonInlinedSFcns.Sfcn2.inputPortInfo[0]);
        ssSetPortInfoForInputs(rts,
          &bypasscross_M->NonInlinedSFcns.Sfcn2.inputPortInfo[0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &bypasscross_M->NonInlinedSFcns.Sfcn2.inputPortUnits[0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &bypasscross_M->NonInlinedSFcns.Sfcn2.inputPortCoSimAttribute[0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          int32_T const **sfcnUPtrs = (int32_T const **)
            &bypasscross_M->NonInlinedSFcns.Sfcn2.UPtrs0;

          {
            int_T i1;
            const int32_T *u0 = bypasscross_B.PisoundInput_o1;
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
            &bypasscross_M->NonInlinedSFcns.Sfcn2.UPtrs1;

          {
            int_T i1;
            const int32_T *u1 = bypasscross_B.PisoundInput_o2;
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
      ssSetModelName(rts, "Stream Output");
      ssSetPath(rts, "bypasscross/Stream Output");
      ssSetRTModel(rts,bypasscross_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &bypasscross_M->NonInlinedSFcns.Sfcn2.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)bypasscross_ConstP.PisoundInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)bypasscross_ConstP.PisoundInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)bypasscross_ConstP.PisoundInput_P3_Size);
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

    /* Level2 S-Function Block: bypasscross/<Root>/Stream Output1 (pisound_out) */
    {
      SimStruct *rts = bypasscross_M->childSfunctions[3];

      /* timing info */
      time_T *sfcnPeriod = bypasscross_M->NonInlinedSFcns.Sfcn3.sfcnPeriod;
      time_T *sfcnOffset = bypasscross_M->NonInlinedSFcns.Sfcn3.sfcnOffset;
      int_T *sfcnTsMap = bypasscross_M->NonInlinedSFcns.Sfcn3.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &bypasscross_M->NonInlinedSFcns.blkInfo2[3]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &bypasscross_M->NonInlinedSFcns.inputOutputPortInfo2[3]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, bypasscross_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &bypasscross_M->NonInlinedSFcns.methods2[3]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &bypasscross_M->NonInlinedSFcns.methods3[3]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &bypasscross_M->NonInlinedSFcns.methods4[3]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &bypasscross_M->NonInlinedSFcns.statesInfo2[3]);
        ssSetPeriodicStatesInfo(rts,
          &bypasscross_M->NonInlinedSFcns.periodicStatesInfo[3]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts,
          &bypasscross_M->NonInlinedSFcns.Sfcn3.inputPortInfo[0]);
        ssSetPortInfoForInputs(rts,
          &bypasscross_M->NonInlinedSFcns.Sfcn3.inputPortInfo[0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &bypasscross_M->NonInlinedSFcns.Sfcn3.inputPortUnits[0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &bypasscross_M->NonInlinedSFcns.Sfcn3.inputPortCoSimAttribute[0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          int32_T const **sfcnUPtrs = (int32_T const **)
            &bypasscross_M->NonInlinedSFcns.Sfcn3.UPtrs0;

          {
            int_T i1;
            const int32_T *u0 = bypasscross_B.StreamInput_o1;
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
            &bypasscross_M->NonInlinedSFcns.Sfcn3.UPtrs1;

          {
            int_T i1;
            const int32_T *u1 = bypasscross_B.StreamInput_o2;
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
      ssSetModelName(rts, "Stream Output1");
      ssSetPath(rts, "bypasscross/Stream Output1");
      ssSetRTModel(rts,bypasscross_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &bypasscross_M->NonInlinedSFcns.Sfcn3.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)bypasscross_ConstP.PisoundInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)bypasscross_ConstP.PisoundInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)bypasscross_ConstP.PisoundInput_P3_Size);
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
}

/* Model terminate function */
void bypasscross_terminate(void)
{
  /* Terminate for S-Function (pisound_in): '<Root>/Pisound Input' */
  /* Level2 S-Function Block: '<Root>/Pisound Input' (pisound_in) */
  {
    SimStruct *rts = bypasscross_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (stream_in): '<Root>/Stream Input' */
  /* Level2 S-Function Block: '<Root>/Stream Input' (stream_in) */
  {
    SimStruct *rts = bypasscross_M->childSfunctions[1];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (stream_out): '<Root>/Stream Output' */
  /* Level2 S-Function Block: '<Root>/Stream Output' (stream_out) */
  {
    SimStruct *rts = bypasscross_M->childSfunctions[2];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (pisound_out): '<Root>/Stream Output1' */
  /* Level2 S-Function Block: '<Root>/Stream Output1' (pisound_out) */
  {
    SimStruct *rts = bypasscross_M->childSfunctions[3];
    sfcnTerminate(rts);
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
