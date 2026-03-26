/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: bypass.c
 *
 * Code generated for Simulink model 'bypass'.
 *
 * Model version                  : 1.1
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Tue Mar  3 15:30:42 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "bypass.h"
#include "bypass_private.h"

/* Block signals (default storage) */
B_bypass_T bypass_B;

/* Real-time model */
static RT_MODEL_bypass_T bypass_M_;
RT_MODEL_bypass_T *const bypass_M = &bypass_M_;

/* Model step function */
void bypass_step(void)
{
  /* S-Function (pisound_in): '<Root>/PiSound Input' */

  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = bypass_M->childSfunctions[0];
    sfcnOutputs(rts,0);
  }

  /* S-Function (pisound_out): '<Root>/PiSound Output' */

  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = bypass_M->childSfunctions[1];
    sfcnOutputs(rts,0);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  bypass_M->Timing.t[0] =
    ((time_T)(++bypass_M->Timing.clockTick0)) * bypass_M->Timing.stepSize0;
}

/* Model initialize function */
void bypass_initialize(void)
{
  /* Registration code */
  rtsiSetSolverName(&bypass_M->solverInfo,"FixedStepDiscrete");
  bypass_M->solverInfoPtr = (&bypass_M->solverInfo);

  /* Initialize timing info */
  {
    int_T *mdlTsMap = bypass_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    bypass_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    bypass_M->Timing.sampleTimes = (&bypass_M->Timing.sampleTimesArray[0]);
    bypass_M->Timing.offsetTimes = (&bypass_M->Timing.offsetTimesArray[0]);

    /* task periods */
    bypass_M->Timing.sampleTimes[0] = (0.0026666666666666666);

    /* task offsets */
    bypass_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(bypass_M, &bypass_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = bypass_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    bypass_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(bypass_M, -1);
  bypass_M->Timing.stepSize0 = 0.0026666666666666666;
  bypass_M->solverInfoPtr = (&bypass_M->solverInfo);
  bypass_M->Timing.stepSize = (0.0026666666666666666);
  rtsiSetFixedStepSize(&bypass_M->solverInfo, 0.0026666666666666666);
  rtsiSetSolverMode(&bypass_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* child S-Function registration */
  {
    RTWSfcnInfo *sfcnInfo = &bypass_M->NonInlinedSFcns.sfcnInfo;
    bypass_M->sfcnInfo = (sfcnInfo);
    rtssSetErrorStatusPtr(sfcnInfo, (&rtmGetErrorStatus(bypass_M)));
    bypass_M->Sizes.numSampTimes = (1);
    rtssSetNumRootSampTimesPtr(sfcnInfo, &bypass_M->Sizes.numSampTimes);
    bypass_M->NonInlinedSFcns.taskTimePtrs[0] = (&rtmGetTPtr(bypass_M)[0]);
    rtssSetTPtrPtr(sfcnInfo,bypass_M->NonInlinedSFcns.taskTimePtrs);
    rtssSetTStartPtr(sfcnInfo, &rtmGetTStart(bypass_M));
    rtssSetTFinalPtr(sfcnInfo, &rtmGetTFinal(bypass_M));
    rtssSetTimeOfLastOutputPtr(sfcnInfo, &rtmGetTimeOfLastOutput(bypass_M));
    rtssSetStepSizePtr(sfcnInfo, &bypass_M->Timing.stepSize);
    rtssSetStopRequestedPtr(sfcnInfo, &rtmGetStopRequested(bypass_M));
    rtssSetDerivCacheNeedsResetPtr(sfcnInfo, &bypass_M->derivCacheNeedsReset);
    rtssSetZCCacheNeedsResetPtr(sfcnInfo, &bypass_M->zCCacheNeedsReset);
    rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(sfcnInfo,
      &bypass_M->CTOutputIncnstWithState);
    rtssSetSampleHitsPtr(sfcnInfo, &bypass_M->Timing.sampleHits);
    rtssSetPerTaskSampleHitsPtr(sfcnInfo, &bypass_M->Timing.perTaskSampleHits);
    rtssSetSimModePtr(sfcnInfo, &bypass_M->simMode);
    rtssSetSolverInfoPtr(sfcnInfo, &bypass_M->solverInfoPtr);
  }

  bypass_M->Sizes.numSFcns = (2);

  /* register each child */
  {
    (void) memset((void *)&bypass_M->NonInlinedSFcns.childSFunctions[0], 0,
                  2*sizeof(SimStruct));
    bypass_M->childSfunctions = (&bypass_M->NonInlinedSFcns.childSFunctionPtrs[0]);
    bypass_M->childSfunctions[0] = (&bypass_M->NonInlinedSFcns.childSFunctions[0]);
    bypass_M->childSfunctions[1] = (&bypass_M->NonInlinedSFcns.childSFunctions[1]);

    /* Level2 S-Function Block: bypass/<Root>/PiSound Input (pisound_in) */
    {
      SimStruct *rts = bypass_M->childSfunctions[0];

      /* timing info */
      time_T *sfcnPeriod = bypass_M->NonInlinedSFcns.Sfcn0.sfcnPeriod;
      time_T *sfcnOffset = bypass_M->NonInlinedSFcns.Sfcn0.sfcnOffset;
      int_T *sfcnTsMap = bypass_M->NonInlinedSFcns.Sfcn0.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &bypass_M->NonInlinedSFcns.blkInfo2[0]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &bypass_M->NonInlinedSFcns.inputOutputPortInfo2[0]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, bypass_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &bypass_M->NonInlinedSFcns.methods2[0]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &bypass_M->NonInlinedSFcns.methods3[0]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &bypass_M->NonInlinedSFcns.methods4[0]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &bypass_M->NonInlinedSFcns.statesInfo2[0]);
        ssSetPeriodicStatesInfo(rts,
          &bypass_M->NonInlinedSFcns.periodicStatesInfo[0]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &bypass_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &bypass_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &bypass_M->NonInlinedSFcns.Sfcn0.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &bypass_M->NonInlinedSFcns.Sfcn0.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 128);
          ssSetOutputPortSignal(rts, 0, ((int32_T *) bypass_B.PiSoundInput_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 128);
          ssSetOutputPortSignal(rts, 1, ((int32_T *) bypass_B.PiSoundInput_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "PiSound Input");
      ssSetPath(rts, "bypass/PiSound Input");
      ssSetRTModel(rts,bypass_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &bypass_M->NonInlinedSFcns.Sfcn0.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)bypass_ConstP.PiSoundInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)bypass_ConstP.PiSoundInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)bypass_ConstP.PiSoundInput_P3_Size);
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

    /* Level2 S-Function Block: bypass/<Root>/PiSound Output (pisound_out) */
    {
      SimStruct *rts = bypass_M->childSfunctions[1];

      /* timing info */
      time_T *sfcnPeriod = bypass_M->NonInlinedSFcns.Sfcn1.sfcnPeriod;
      time_T *sfcnOffset = bypass_M->NonInlinedSFcns.Sfcn1.sfcnOffset;
      int_T *sfcnTsMap = bypass_M->NonInlinedSFcns.Sfcn1.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &bypass_M->NonInlinedSFcns.blkInfo2[1]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &bypass_M->NonInlinedSFcns.inputOutputPortInfo2[1]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, bypass_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &bypass_M->NonInlinedSFcns.methods2[1]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &bypass_M->NonInlinedSFcns.methods3[1]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &bypass_M->NonInlinedSFcns.methods4[1]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &bypass_M->NonInlinedSFcns.statesInfo2[1]);
        ssSetPeriodicStatesInfo(rts,
          &bypass_M->NonInlinedSFcns.periodicStatesInfo[1]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts,
          &bypass_M->NonInlinedSFcns.Sfcn1.inputPortInfo[0]);
        ssSetPortInfoForInputs(rts,
          &bypass_M->NonInlinedSFcns.Sfcn1.inputPortInfo[0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &bypass_M->NonInlinedSFcns.Sfcn1.inputPortUnits[0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &bypass_M->NonInlinedSFcns.Sfcn1.inputPortCoSimAttribute[0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          int32_T const **sfcnUPtrs = (int32_T const **)
            &bypass_M->NonInlinedSFcns.Sfcn1.UPtrs0;

          {
            int_T i1;
            const int32_T *u0 = bypass_B.PiSoundInput_o1;
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
            &bypass_M->NonInlinedSFcns.Sfcn1.UPtrs1;

          {
            int_T i1;
            const int32_T *u1 = bypass_B.PiSoundInput_o2;
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
      ssSetPath(rts, "bypass/PiSound Output");
      ssSetRTModel(rts,bypass_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &bypass_M->NonInlinedSFcns.Sfcn1.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)bypass_ConstP.PiSoundInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)bypass_ConstP.PiSoundInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)bypass_ConstP.PiSoundInput_P3_Size);
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
void bypass_terminate(void)
{
  /* Terminate for S-Function (pisound_in): '<Root>/PiSound Input' */
  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = bypass_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (pisound_out): '<Root>/PiSound Output' */
  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = bypass_M->childSfunctions[1];
    sfcnTerminate(rts);
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
