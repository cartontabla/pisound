/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: tremolo2.c
 *
 * Code generated for Simulink model 'tremolo2'.
 *
 * Model version                  : 1.18
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Sat Feb  7 10:39:11 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "tremolo2.h"
#include "tremolo2_private.h"

/* Block signals (default storage) */
B_tremolo2_T tremolo2_B;

/* Block states (default storage) */
DW_tremolo2_T tremolo2_DW;

/* Real-time model */
static RT_MODEL_tremolo2_T tremolo2_M_;
RT_MODEL_tremolo2_T *const tremolo2_M = &tremolo2_M_;

/* Model step function */
void tremolo2_step(void)
{
  /* S-Function (pisound_in): '<Root>/pisound_in' */

  /* Level2 S-Function Block: '<Root>/pisound_in' (pisound_in) */
  {
    SimStruct *rts = tremolo2_M->childSfunctions[0];
    sfcnOutputs(rts,0);
  }

  /* MATLABSystem: '<Root>/MATLAB System1' */
  /*  tabla seleccionada (igual que osc_sys) */
  /* out-of-bounds matrix access would cause program termination and was eliminated */

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  tremolo2_M->Timing.t[0] =
    ((time_T)(++tremolo2_M->Timing.clockTick0)) * tremolo2_M->Timing.stepSize0;
}

/* Model initialize function */
void tremolo2_initialize(void)
{
  /* Registration code */
  rtsiSetSolverName(&tremolo2_M->solverInfo,"FixedStepDiscrete");
  tremolo2_M->solverInfoPtr = (&tremolo2_M->solverInfo);

  /* Initialize timing info */
  {
    int_T *mdlTsMap = tremolo2_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    tremolo2_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    tremolo2_M->Timing.sampleTimes = (&tremolo2_M->Timing.sampleTimesArray[0]);
    tremolo2_M->Timing.offsetTimes = (&tremolo2_M->Timing.offsetTimesArray[0]);

    /* task periods */
    tremolo2_M->Timing.sampleTimes[0] = (0.0026666666666666666);

    /* task offsets */
    tremolo2_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(tremolo2_M, &tremolo2_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = tremolo2_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    tremolo2_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(tremolo2_M, -1);
  tremolo2_M->Timing.stepSize0 = 0.0026666666666666666;
  tremolo2_M->solverInfoPtr = (&tremolo2_M->solverInfo);
  tremolo2_M->Timing.stepSize = (0.0026666666666666666);
  rtsiSetFixedStepSize(&tremolo2_M->solverInfo, 0.0026666666666666666);
  rtsiSetSolverMode(&tremolo2_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* child S-Function registration */
  {
    RTWSfcnInfo *sfcnInfo = &tremolo2_M->NonInlinedSFcns.sfcnInfo;
    tremolo2_M->sfcnInfo = (sfcnInfo);
    rtssSetErrorStatusPtr(sfcnInfo, (&rtmGetErrorStatus(tremolo2_M)));
    tremolo2_M->Sizes.numSampTimes = (1);
    rtssSetNumRootSampTimesPtr(sfcnInfo, &tremolo2_M->Sizes.numSampTimes);
    tremolo2_M->NonInlinedSFcns.taskTimePtrs[0] = (&rtmGetTPtr(tremolo2_M)[0]);
    rtssSetTPtrPtr(sfcnInfo,tremolo2_M->NonInlinedSFcns.taskTimePtrs);
    rtssSetTStartPtr(sfcnInfo, &rtmGetTStart(tremolo2_M));
    rtssSetTFinalPtr(sfcnInfo, &rtmGetTFinal(tremolo2_M));
    rtssSetTimeOfLastOutputPtr(sfcnInfo, &rtmGetTimeOfLastOutput(tremolo2_M));
    rtssSetStepSizePtr(sfcnInfo, &tremolo2_M->Timing.stepSize);
    rtssSetStopRequestedPtr(sfcnInfo, &rtmGetStopRequested(tremolo2_M));
    rtssSetDerivCacheNeedsResetPtr(sfcnInfo, &tremolo2_M->derivCacheNeedsReset);
    rtssSetZCCacheNeedsResetPtr(sfcnInfo, &tremolo2_M->zCCacheNeedsReset);
    rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(sfcnInfo,
      &tremolo2_M->CTOutputIncnstWithState);
    rtssSetSampleHitsPtr(sfcnInfo, &tremolo2_M->Timing.sampleHits);
    rtssSetPerTaskSampleHitsPtr(sfcnInfo, &tremolo2_M->Timing.perTaskSampleHits);
    rtssSetSimModePtr(sfcnInfo, &tremolo2_M->simMode);
    rtssSetSolverInfoPtr(sfcnInfo, &tremolo2_M->solverInfoPtr);
  }

  tremolo2_M->Sizes.numSFcns = (2);

  /* register each child */
  {
    (void) memset((void *)&tremolo2_M->NonInlinedSFcns.childSFunctions[0], 0,
                  2*sizeof(SimStruct));
    tremolo2_M->childSfunctions =
      (&tremolo2_M->NonInlinedSFcns.childSFunctionPtrs[0]);
    tremolo2_M->childSfunctions[0] =
      (&tremolo2_M->NonInlinedSFcns.childSFunctions[0]);
    tremolo2_M->childSfunctions[1] =
      (&tremolo2_M->NonInlinedSFcns.childSFunctions[1]);

    /* Level2 S-Function Block: tremolo2/<Root>/pisound_in (pisound_in) */
    {
      SimStruct *rts = tremolo2_M->childSfunctions[0];

      /* timing info */
      time_T *sfcnPeriod = tremolo2_M->NonInlinedSFcns.Sfcn0.sfcnPeriod;
      time_T *sfcnOffset = tremolo2_M->NonInlinedSFcns.Sfcn0.sfcnOffset;
      int_T *sfcnTsMap = tremolo2_M->NonInlinedSFcns.Sfcn0.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &tremolo2_M->NonInlinedSFcns.blkInfo2[0]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &tremolo2_M->NonInlinedSFcns.inputOutputPortInfo2[0]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, tremolo2_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &tremolo2_M->NonInlinedSFcns.methods2[0]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &tremolo2_M->NonInlinedSFcns.methods3[0]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &tremolo2_M->NonInlinedSFcns.methods4[0]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &tremolo2_M->NonInlinedSFcns.statesInfo2[0]);
        ssSetPeriodicStatesInfo(rts,
          &tremolo2_M->NonInlinedSFcns.periodicStatesInfo[0]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &tremolo2_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &tremolo2_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 4);
        _ssSetPortInfo2ForOutputUnits(rts,
          &tremolo2_M->NonInlinedSFcns.Sfcn0.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        ssSetOutputPortUnit(rts, 2, 0);
        ssSetOutputPortUnit(rts, 3, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &tremolo2_M->NonInlinedSFcns.Sfcn0.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 2, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 3, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 128);
          ssSetOutputPortSignal(rts, 0, ((int32_T *) tremolo2_B.pisound_in_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 128);
          ssSetOutputPortSignal(rts, 1, ((int32_T *) tremolo2_B.pisound_in_o2));
        }

        /* port 2 */
        {
          _ssSetOutputPortNumDimensions(rts, 2, 1);
          ssSetOutputPortWidthAsInt(rts, 2, 128);
          ssSetOutputPortSignal(rts, 2, ((uint8_T *) tremolo2_B.pisound_in_o3));
        }

        /* port 3 */
        {
          _ssSetOutputPortNumDimensions(rts, 3, 1);
          ssSetOutputPortWidthAsInt(rts, 3, 1);
          ssSetOutputPortSignal(rts, 3, ((uint32_T *) &tremolo2_B.pisound_in_o4));
        }
      }

      /* path info */
      ssSetModelName(rts, "pisound_in");
      ssSetPath(rts, "tremolo2/pisound_in");
      ssSetRTModel(rts,tremolo2_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &tremolo2_M->NonInlinedSFcns.Sfcn0.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)tremolo2_P.pisound_in_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)tremolo2_P.pisound_in_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)tremolo2_P.pisound_in_P3_Size);
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
      _ssSetOutputPortConnected(rts, 2, 1);
      _ssSetOutputPortConnected(rts, 3, 1);
      _ssSetOutputPortBeingMerged(rts, 0, 0);
      _ssSetOutputPortBeingMerged(rts, 1, 0);
      _ssSetOutputPortBeingMerged(rts, 2, 0);
      _ssSetOutputPortBeingMerged(rts, 3, 0);

      /* Update the BufferDstPort flags for each input port */
    }

    /* Level2 S-Function Block: tremolo2/<Root>/pisound_out (pisound_out) */
    {
      SimStruct *rts = tremolo2_M->childSfunctions[1];

      /* timing info */
      time_T *sfcnPeriod = tremolo2_M->NonInlinedSFcns.Sfcn1.sfcnPeriod;
      time_T *sfcnOffset = tremolo2_M->NonInlinedSFcns.Sfcn1.sfcnOffset;
      int_T *sfcnTsMap = tremolo2_M->NonInlinedSFcns.Sfcn1.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &tremolo2_M->NonInlinedSFcns.blkInfo2[1]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &tremolo2_M->NonInlinedSFcns.inputOutputPortInfo2[1]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, tremolo2_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &tremolo2_M->NonInlinedSFcns.methods2[1]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &tremolo2_M->NonInlinedSFcns.methods3[1]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &tremolo2_M->NonInlinedSFcns.methods4[1]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &tremolo2_M->NonInlinedSFcns.statesInfo2[1]);
        ssSetPeriodicStatesInfo(rts,
          &tremolo2_M->NonInlinedSFcns.periodicStatesInfo[1]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 4);
        ssSetPortInfoForInputs(rts,
          &tremolo2_M->NonInlinedSFcns.Sfcn1.inputPortInfo[0]);
        ssSetPortInfoForInputs(rts,
          &tremolo2_M->NonInlinedSFcns.Sfcn1.inputPortInfo[0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &tremolo2_M->NonInlinedSFcns.Sfcn1.inputPortUnits[0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        ssSetInputPortUnit(rts, 2, 0);
        ssSetInputPortUnit(rts, 3, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &tremolo2_M->NonInlinedSFcns.Sfcn1.inputPortCoSimAttribute[0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);
        ssSetInputPortIsContinuousQuantity(rts, 2, 0);
        ssSetInputPortIsContinuousQuantity(rts, 3, 0);

        /* port 0 */
        {
          int32_T const **sfcnUPtrs = (int32_T const **)
            &tremolo2_M->NonInlinedSFcns.Sfcn1.UPtrs0;

          {
            int_T i1;
            const int32_T *u0 = tremolo2_B.DataTypeConversion1;
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
            &tremolo2_M->NonInlinedSFcns.Sfcn1.UPtrs1;

          {
            int_T i1;
            const int32_T *u1 = tremolo2_B.DataTypeConversion3;
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
            &tremolo2_M->NonInlinedSFcns.Sfcn1.UPtrs2;

          {
            int_T i1;
            const uint8_T *u2 = tremolo2_B.pisound_in_o3;
            for (i1=0; i1 < 128; i1++) {
              sfcnUPtrs[i1] = &u2[i1];
            }
          }

          ssSetInputPortSignalPtrs(rts, 2, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 2, 1);
          ssSetInputPortWidthAsInt(rts, 2, 128);
        }

        /* port 3 */
        {
          uint32_T const **sfcnUPtrs = (uint32_T const **)
            &tremolo2_M->NonInlinedSFcns.Sfcn1.UPtrs3;
          sfcnUPtrs[0] = &tremolo2_B.pisound_in_o4;
          ssSetInputPortSignalPtrs(rts, 3, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 3, 1);
          ssSetInputPortWidthAsInt(rts, 3, 1);
        }
      }

      /* path info */
      ssSetModelName(rts, "pisound_out");
      ssSetPath(rts, "tremolo2/pisound_out");
      ssSetRTModel(rts,tremolo2_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &tremolo2_M->NonInlinedSFcns.Sfcn1.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)tremolo2_P.pisound_out_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)tremolo2_P.pisound_out_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)tremolo2_P.pisound_out_P3_Size);
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
      _ssSetInputPortConnected(rts, 2, 1);
      _ssSetInputPortConnected(rts, 3, 1);

      /* Update the BufferDstPort flags for each input port */
      ssSetInputPortBufferDstPort(rts, 0, -1);
      ssSetInputPortBufferDstPort(rts, 1, -1);
      ssSetInputPortBufferDstPort(rts, 2, -1);
      ssSetInputPortBufferDstPort(rts, 3, -1);
    }
  }

  /* Start for MATLABSystem: '<Root>/MATLAB System1' */
  tremolo2_DW.obj.Rate = tremolo2_P.MATLABSystem1_Rate;
  tremolo2_DW.obj.Depth = tremolo2_P.MATLABSystem1_Depth;
  tremolo2_DW.obj.StereoPhase = tremolo2_P.MATLABSystem1_StereoPhase;
  tremolo2_DW.obj.WaveSel = tremolo2_P.MATLABSystem1_WaveSel;
  tremolo2_DW.obj.isInitialized = 1;
  tremolo2_DW.obj.Phase = 0.0;

  /* Start for MATLABSystem: '<Root>/MATLAB System' */
  /*  (aquí copias tu init de tablas de osc_sys: built-in + custom) */
  /*  obj.WaveTables, obj.TablesSize ... */
  tremolo2_DW.obj_i.isInitialized = 1;

  /*  Perform one-time calculations  */
  tremolo2_DW.obj_i.Gain = 1.0;
  tremolo2_DW.obj_i.Bypass = false;
}

/* Model terminate function */
void tremolo2_terminate(void)
{
  /* Terminate for S-Function (pisound_in): '<Root>/pisound_in' */
  /* Level2 S-Function Block: '<Root>/pisound_in' (pisound_in) */
  {
    SimStruct *rts = tremolo2_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (pisound_out): '<Root>/pisound_out' */
  /* Level2 S-Function Block: '<Root>/pisound_out' (pisound_out) */
  {
    SimStruct *rts = tremolo2_M->childSfunctions[1];
    sfcnTerminate(rts);
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
