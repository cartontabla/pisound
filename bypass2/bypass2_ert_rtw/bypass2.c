/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: bypass2.c
 *
 * Code generated for Simulink model 'bypass2'.
 *
 * Model version                  : 1.6
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Sun Apr  5 11:08:17 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "bypass2.h"
#include "bypass2_private.h"

/* Block signals (default storage) */
B_bypass2_T bypass2_B;

/* Real-time model */
static RT_MODEL_bypass2_T bypass2_M_;
RT_MODEL_bypass2_T *const bypass2_M = &bypass2_M_;

/* Model step function */
void bypass2_step(void)
{
  /* S-Function (stream_in): '<Root>/Stream Input' */

  /* Level2 S-Function Block: '<Root>/Stream Input' (stream_in) */
  {
    SimStruct *rts = bypass2_M->childSfunctions[0];
    sfcnOutputs(rts,0);
  }

  /* S-Function (stream_out): '<Root>/Stream Output' */

  /* Level2 S-Function Block: '<Root>/Stream Output' (stream_out) */
  {
    SimStruct *rts = bypass2_M->childSfunctions[1];
    sfcnOutputs(rts,0);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  bypass2_M->Timing.t[0] =
    ((time_T)(++bypass2_M->Timing.clockTick0)) * bypass2_M->Timing.stepSize0;
}

/* Model initialize function */
void bypass2_initialize(void)
{
  /* Registration code */
  rtsiSetSolverName(&bypass2_M->solverInfo,"FixedStepDiscrete");
  bypass2_M->solverInfoPtr = (&bypass2_M->solverInfo);

  /* Initialize timing info */
  {
    int_T *mdlTsMap = bypass2_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    bypass2_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    bypass2_M->Timing.sampleTimes = (&bypass2_M->Timing.sampleTimesArray[0]);
    bypass2_M->Timing.offsetTimes = (&bypass2_M->Timing.offsetTimesArray[0]);

    /* task periods */
    bypass2_M->Timing.sampleTimes[0] = (0.0026666666666666666);

    /* task offsets */
    bypass2_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(bypass2_M, &bypass2_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = bypass2_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    bypass2_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(bypass2_M, -1);
  bypass2_M->Timing.stepSize0 = 0.0026666666666666666;
  bypass2_M->solverInfoPtr = (&bypass2_M->solverInfo);
  bypass2_M->Timing.stepSize = (0.0026666666666666666);
  rtsiSetFixedStepSize(&bypass2_M->solverInfo, 0.0026666666666666666);
  rtsiSetSolverMode(&bypass2_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* child S-Function registration */
  {
    RTWSfcnInfo *sfcnInfo = &bypass2_M->NonInlinedSFcns.sfcnInfo;
    bypass2_M->sfcnInfo = (sfcnInfo);
    rtssSetErrorStatusPtr(sfcnInfo, (&rtmGetErrorStatus(bypass2_M)));
    bypass2_M->Sizes.numSampTimes = (1);
    rtssSetNumRootSampTimesPtr(sfcnInfo, &bypass2_M->Sizes.numSampTimes);
    bypass2_M->NonInlinedSFcns.taskTimePtrs[0] = (&rtmGetTPtr(bypass2_M)[0]);
    rtssSetTPtrPtr(sfcnInfo,bypass2_M->NonInlinedSFcns.taskTimePtrs);
    rtssSetTStartPtr(sfcnInfo, &rtmGetTStart(bypass2_M));
    rtssSetTFinalPtr(sfcnInfo, &rtmGetTFinal(bypass2_M));
    rtssSetTimeOfLastOutputPtr(sfcnInfo, &rtmGetTimeOfLastOutput(bypass2_M));
    rtssSetStepSizePtr(sfcnInfo, &bypass2_M->Timing.stepSize);
    rtssSetStopRequestedPtr(sfcnInfo, &rtmGetStopRequested(bypass2_M));
    rtssSetDerivCacheNeedsResetPtr(sfcnInfo, &bypass2_M->derivCacheNeedsReset);
    rtssSetZCCacheNeedsResetPtr(sfcnInfo, &bypass2_M->zCCacheNeedsReset);
    rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(sfcnInfo,
      &bypass2_M->CTOutputIncnstWithState);
    rtssSetSampleHitsPtr(sfcnInfo, &bypass2_M->Timing.sampleHits);
    rtssSetPerTaskSampleHitsPtr(sfcnInfo, &bypass2_M->Timing.perTaskSampleHits);
    rtssSetSimModePtr(sfcnInfo, &bypass2_M->simMode);
    rtssSetSolverInfoPtr(sfcnInfo, &bypass2_M->solverInfoPtr);
  }

  bypass2_M->Sizes.numSFcns = (2);

  /* register each child */
  {
    (void) memset((void *)&bypass2_M->NonInlinedSFcns.childSFunctions[0], 0,
                  2*sizeof(SimStruct));
    bypass2_M->childSfunctions = (&bypass2_M->
      NonInlinedSFcns.childSFunctionPtrs[0]);
    bypass2_M->childSfunctions[0] = (&bypass2_M->
      NonInlinedSFcns.childSFunctions[0]);
    bypass2_M->childSfunctions[1] = (&bypass2_M->
      NonInlinedSFcns.childSFunctions[1]);

    /* Level2 S-Function Block: bypass2/<Root>/Stream Input (stream_in) */
    {
      SimStruct *rts = bypass2_M->childSfunctions[0];

      /* timing info */
      time_T *sfcnPeriod = bypass2_M->NonInlinedSFcns.Sfcn0.sfcnPeriod;
      time_T *sfcnOffset = bypass2_M->NonInlinedSFcns.Sfcn0.sfcnOffset;
      int_T *sfcnTsMap = bypass2_M->NonInlinedSFcns.Sfcn0.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &bypass2_M->NonInlinedSFcns.blkInfo2[0]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &bypass2_M->NonInlinedSFcns.inputOutputPortInfo2[0]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, bypass2_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &bypass2_M->NonInlinedSFcns.methods2[0]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &bypass2_M->NonInlinedSFcns.methods3[0]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &bypass2_M->NonInlinedSFcns.methods4[0]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &bypass2_M->NonInlinedSFcns.statesInfo2[0]);
        ssSetPeriodicStatesInfo(rts,
          &bypass2_M->NonInlinedSFcns.periodicStatesInfo[0]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &bypass2_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &bypass2_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &bypass2_M->NonInlinedSFcns.Sfcn0.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &bypass2_M->NonInlinedSFcns.Sfcn0.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 128);
          ssSetOutputPortSignal(rts, 0, ((int32_T *) bypass2_B.StreamInput_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 128);
          ssSetOutputPortSignal(rts, 1, ((int32_T *) bypass2_B.StreamInput_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "Stream Input");
      ssSetPath(rts, "bypass2/Stream Input");
      ssSetRTModel(rts,bypass2_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &bypass2_M->NonInlinedSFcns.Sfcn0.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)bypass2_ConstP.StreamInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)bypass2_ConstP.StreamInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)bypass2_ConstP.StreamInput_P3_Size);
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

    /* Level2 S-Function Block: bypass2/<Root>/Stream Output (stream_out) */
    {
      SimStruct *rts = bypass2_M->childSfunctions[1];

      /* timing info */
      time_T *sfcnPeriod = bypass2_M->NonInlinedSFcns.Sfcn1.sfcnPeriod;
      time_T *sfcnOffset = bypass2_M->NonInlinedSFcns.Sfcn1.sfcnOffset;
      int_T *sfcnTsMap = bypass2_M->NonInlinedSFcns.Sfcn1.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &bypass2_M->NonInlinedSFcns.blkInfo2[1]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &bypass2_M->NonInlinedSFcns.inputOutputPortInfo2[1]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, bypass2_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &bypass2_M->NonInlinedSFcns.methods2[1]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &bypass2_M->NonInlinedSFcns.methods3[1]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &bypass2_M->NonInlinedSFcns.methods4[1]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &bypass2_M->NonInlinedSFcns.statesInfo2[1]);
        ssSetPeriodicStatesInfo(rts,
          &bypass2_M->NonInlinedSFcns.periodicStatesInfo[1]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts,
          &bypass2_M->NonInlinedSFcns.Sfcn1.inputPortInfo[0]);
        ssSetPortInfoForInputs(rts,
          &bypass2_M->NonInlinedSFcns.Sfcn1.inputPortInfo[0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &bypass2_M->NonInlinedSFcns.Sfcn1.inputPortUnits[0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &bypass2_M->NonInlinedSFcns.Sfcn1.inputPortCoSimAttribute[0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          int32_T const **sfcnUPtrs = (int32_T const **)
            &bypass2_M->NonInlinedSFcns.Sfcn1.UPtrs0;

          {
            int_T i1;
            const int32_T *u0 = bypass2_B.StreamInput_o1;
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
            &bypass2_M->NonInlinedSFcns.Sfcn1.UPtrs1;

          {
            int_T i1;
            const int32_T *u1 = bypass2_B.StreamInput_o2;
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
      ssSetPath(rts, "bypass2/Stream Output");
      ssSetRTModel(rts,bypass2_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &bypass2_M->NonInlinedSFcns.Sfcn1.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)bypass2_ConstP.StreamInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)bypass2_ConstP.StreamInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)bypass2_ConstP.StreamInput_P3_Size);
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
}

/* Model terminate function */
void bypass2_terminate(void)
{
  /* Terminate for S-Function (stream_in): '<Root>/Stream Input' */
  /* Level2 S-Function Block: '<Root>/Stream Input' (stream_in) */
  {
    SimStruct *rts = bypass2_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (stream_out): '<Root>/Stream Output' */
  /* Level2 S-Function Block: '<Root>/Stream Output' (stream_out) */
  {
    SimStruct *rts = bypass2_M->childSfunctions[1];
    sfcnTerminate(rts);
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
