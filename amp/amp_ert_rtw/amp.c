/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: amp.c
 *
 * Code generated for Simulink model 'amp'.
 *
 * Model version                  : 1.10
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Tue Mar  3 20:32:49 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "amp.h"
#include <math.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "amp_private.h"

/* Block signals (default storage) */
B_amp_T amp_B;

/* Block states (default storage) */
DW_amp_T amp_DW;

/* Real-time model */
static RT_MODEL_amp_T amp_M_;
RT_MODEL_amp_T *const amp_M = &amp_M_;

/* Model step function */
void amp_step(void)
{
  real_T rtb_MATLABSystem_o1_0[128];
  real_T rtb_MATLABSystem_o2_0[128];
  real_T tmp;
  int32_T i;

  /* S-Function (pisound_in): '<Root>/PiSound Input' */

  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = amp_M->childSfunctions[0];
    sfcnOutputs(rts,0);
  }

  /* MATLABSystem: '<Root>/MATLAB System' incorporates:
   *  DataTypeConversion: '<Root>/Data Type Conversion'
   *  DataTypeConversion: '<Root>/Data Type Conversion2'
   *  Gain: '<Root>/Gain'
   *  Gain: '<Root>/Gain1'
   */
  if (amp_DW.obj.Gain != amp_P.Gain_rt) {
    amp_DW.obj.Gain = amp_P.Gain_rt;
  }

  if (amp_DW.obj.Bypass != amp_P.Bypass_rt) {
    amp_DW.obj.Bypass = amp_P.Bypass_rt;
  }

  /*  Implement algorithm  */
  if (amp_DW.obj.Bypass) {
    for (i = 0; i < 128; i++) {
      rtb_MATLABSystem_o1_0[i] = amp_P.Gain_Gain * (real_T)
        amp_B.PiSoundInput_o1[i];
      rtb_MATLABSystem_o2_0[i] = amp_P.Gain1_Gain * (real_T)
        amp_B.PiSoundInput_o2[i];
    }
  } else {
    for (i = 0; i < 128; i++) {
      rtb_MATLABSystem_o1_0[i] = amp_P.Gain_Gain * (real_T)
        amp_B.PiSoundInput_o1[i] * amp_DW.obj.Gain;
      rtb_MATLABSystem_o2_0[i] = amp_P.Gain1_Gain * (real_T)
        amp_B.PiSoundInput_o2[i] * amp_DW.obj.Gain;
    }
  }

  for (i = 0; i < 128; i++) {
    /* DataTypeConversion: '<Root>/Data Type Conversion1' incorporates:
     *  Gain: '<Root>/Gain2'
     *  MATLABSystem: '<Root>/MATLAB System'
     */
    tmp = floor(amp_P.Gain2_Gain * rtb_MATLABSystem_o1_0[i]);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion1' */
    amp_B.DataTypeConversion1[i] = tmp < 0.0 ? -(int32_T)(uint32_T)-tmp :
      (int32_T)(uint32_T)tmp;

    /* DataTypeConversion: '<Root>/Data Type Conversion3' incorporates:
     *  Gain: '<Root>/Gain3'
     *  MATLABSystem: '<Root>/MATLAB System'
     */
    tmp = floor(amp_P.Gain3_Gain * rtb_MATLABSystem_o2_0[i]);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion3' */
    amp_B.DataTypeConversion3[i] = tmp < 0.0 ? -(int32_T)(uint32_T)-tmp :
      (int32_T)(uint32_T)tmp;
  }

  /* S-Function (pisound_out): '<Root>/PiSound Output' */

  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = amp_M->childSfunctions[1];
    sfcnOutputs(rts,0);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  amp_M->Timing.t[0] =
    ((time_T)(++amp_M->Timing.clockTick0)) * amp_M->Timing.stepSize0;
}

/* Model initialize function */
void amp_initialize(void)
{
  /* Registration code */
  rtsiSetSolverName(&amp_M->solverInfo,"FixedStepDiscrete");
  amp_M->solverInfoPtr = (&amp_M->solverInfo);

  /* Initialize timing info */
  {
    int_T *mdlTsMap = amp_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    amp_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    amp_M->Timing.sampleTimes = (&amp_M->Timing.sampleTimesArray[0]);
    amp_M->Timing.offsetTimes = (&amp_M->Timing.offsetTimesArray[0]);

    /* task periods */
    amp_M->Timing.sampleTimes[0] = (0.0026666666666666666);

    /* task offsets */
    amp_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(amp_M, &amp_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = amp_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    amp_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(amp_M, -1);
  amp_M->Timing.stepSize0 = 0.0026666666666666666;
  amp_M->solverInfoPtr = (&amp_M->solverInfo);
  amp_M->Timing.stepSize = (0.0026666666666666666);
  rtsiSetFixedStepSize(&amp_M->solverInfo, 0.0026666666666666666);
  rtsiSetSolverMode(&amp_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* child S-Function registration */
  {
    RTWSfcnInfo *sfcnInfo = &amp_M->NonInlinedSFcns.sfcnInfo;
    amp_M->sfcnInfo = (sfcnInfo);
    rtssSetErrorStatusPtr(sfcnInfo, (&rtmGetErrorStatus(amp_M)));
    amp_M->Sizes.numSampTimes = (1);
    rtssSetNumRootSampTimesPtr(sfcnInfo, &amp_M->Sizes.numSampTimes);
    amp_M->NonInlinedSFcns.taskTimePtrs[0] = (&rtmGetTPtr(amp_M)[0]);
    rtssSetTPtrPtr(sfcnInfo,amp_M->NonInlinedSFcns.taskTimePtrs);
    rtssSetTStartPtr(sfcnInfo, &rtmGetTStart(amp_M));
    rtssSetTFinalPtr(sfcnInfo, &rtmGetTFinal(amp_M));
    rtssSetTimeOfLastOutputPtr(sfcnInfo, &rtmGetTimeOfLastOutput(amp_M));
    rtssSetStepSizePtr(sfcnInfo, &amp_M->Timing.stepSize);
    rtssSetStopRequestedPtr(sfcnInfo, &rtmGetStopRequested(amp_M));
    rtssSetDerivCacheNeedsResetPtr(sfcnInfo, &amp_M->derivCacheNeedsReset);
    rtssSetZCCacheNeedsResetPtr(sfcnInfo, &amp_M->zCCacheNeedsReset);
    rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(sfcnInfo,
      &amp_M->CTOutputIncnstWithState);
    rtssSetSampleHitsPtr(sfcnInfo, &amp_M->Timing.sampleHits);
    rtssSetPerTaskSampleHitsPtr(sfcnInfo, &amp_M->Timing.perTaskSampleHits);
    rtssSetSimModePtr(sfcnInfo, &amp_M->simMode);
    rtssSetSolverInfoPtr(sfcnInfo, &amp_M->solverInfoPtr);
  }

  amp_M->Sizes.numSFcns = (2);

  /* register each child */
  {
    (void) memset((void *)&amp_M->NonInlinedSFcns.childSFunctions[0], 0,
                  2*sizeof(SimStruct));
    amp_M->childSfunctions = (&amp_M->NonInlinedSFcns.childSFunctionPtrs[0]);
    amp_M->childSfunctions[0] = (&amp_M->NonInlinedSFcns.childSFunctions[0]);
    amp_M->childSfunctions[1] = (&amp_M->NonInlinedSFcns.childSFunctions[1]);

    /* Level2 S-Function Block: amp/<Root>/PiSound Input (pisound_in) */
    {
      SimStruct *rts = amp_M->childSfunctions[0];

      /* timing info */
      time_T *sfcnPeriod = amp_M->NonInlinedSFcns.Sfcn0.sfcnPeriod;
      time_T *sfcnOffset = amp_M->NonInlinedSFcns.Sfcn0.sfcnOffset;
      int_T *sfcnTsMap = amp_M->NonInlinedSFcns.Sfcn0.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &amp_M->NonInlinedSFcns.blkInfo2[0]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &amp_M->NonInlinedSFcns.inputOutputPortInfo2[0]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, amp_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &amp_M->NonInlinedSFcns.methods2[0]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &amp_M->NonInlinedSFcns.methods3[0]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &amp_M->NonInlinedSFcns.methods4[0]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &amp_M->NonInlinedSFcns.statesInfo2[0]);
        ssSetPeriodicStatesInfo(rts, &amp_M->NonInlinedSFcns.periodicStatesInfo
          [0]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &amp_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &amp_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &amp_M->NonInlinedSFcns.Sfcn0.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &amp_M->NonInlinedSFcns.Sfcn0.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 128);
          ssSetOutputPortSignal(rts, 0, ((int32_T *) amp_B.PiSoundInput_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 128);
          ssSetOutputPortSignal(rts, 1, ((int32_T *) amp_B.PiSoundInput_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "PiSound Input");
      ssSetPath(rts, "amp/PiSound Input");
      ssSetRTModel(rts,amp_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **) &amp_M->NonInlinedSFcns.Sfcn0.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)amp_P.PiSoundInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)amp_P.PiSoundInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)amp_P.PiSoundInput_P3_Size);
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

    /* Level2 S-Function Block: amp/<Root>/PiSound Output (pisound_out) */
    {
      SimStruct *rts = amp_M->childSfunctions[1];

      /* timing info */
      time_T *sfcnPeriod = amp_M->NonInlinedSFcns.Sfcn1.sfcnPeriod;
      time_T *sfcnOffset = amp_M->NonInlinedSFcns.Sfcn1.sfcnOffset;
      int_T *sfcnTsMap = amp_M->NonInlinedSFcns.Sfcn1.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &amp_M->NonInlinedSFcns.blkInfo2[1]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &amp_M->NonInlinedSFcns.inputOutputPortInfo2[1]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, amp_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &amp_M->NonInlinedSFcns.methods2[1]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &amp_M->NonInlinedSFcns.methods3[1]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &amp_M->NonInlinedSFcns.methods4[1]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &amp_M->NonInlinedSFcns.statesInfo2[1]);
        ssSetPeriodicStatesInfo(rts, &amp_M->NonInlinedSFcns.periodicStatesInfo
          [1]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts, &amp_M->NonInlinedSFcns.Sfcn1.inputPortInfo
          [0]);
        ssSetPortInfoForInputs(rts, &amp_M->NonInlinedSFcns.Sfcn1.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &amp_M->NonInlinedSFcns.Sfcn1.inputPortUnits[0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &amp_M->NonInlinedSFcns.Sfcn1.inputPortCoSimAttribute[0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          int32_T const **sfcnUPtrs = (int32_T const **)
            &amp_M->NonInlinedSFcns.Sfcn1.UPtrs0;

          {
            int_T i1;
            const int32_T *u0 = amp_B.DataTypeConversion1;
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
            &amp_M->NonInlinedSFcns.Sfcn1.UPtrs1;

          {
            int_T i1;
            const int32_T *u1 = amp_B.DataTypeConversion3;
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
      ssSetPath(rts, "amp/PiSound Output");
      ssSetRTModel(rts,amp_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **) &amp_M->NonInlinedSFcns.Sfcn1.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)amp_P.PiSoundOutput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)amp_P.PiSoundOutput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)amp_P.PiSoundOutput_P3_Size);
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
  amp_DW.obj.isInitialized = 1;

  /*  Perform one-time calculations  */
  amp_DW.obj.Gain = 1.0;
  amp_DW.obj.Bypass = false;
}

/* Model terminate function */
void amp_terminate(void)
{
  /* Terminate for S-Function (pisound_in): '<Root>/PiSound Input' */
  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = amp_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (pisound_out): '<Root>/PiSound Output' */
  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = amp_M->childSfunctions[1];
    sfcnTerminate(rts);
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
