/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: delay.c
 *
 * Code generated for Simulink model 'delay'.
 *
 * Model version                  : 1.24
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Thu Mar  5 19:39:13 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "delay.h"
#include <math.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include <string.h>
#include "delay_private.h"

/* Exported block parameters */
real_T Mix_rt = 0.5;                   /* Variable: Mix_rt
                                        * Referenced by:
                                        *   '<Root>/Gain4'
                                        *   '<Root>/Gain5'
                                        *   '<Root>/Gain6'
                                        *   '<Root>/Gain7'
                                        */

/* Block signals (default storage) */
B_delay_T delay_B;

/* Block states (default storage) */
DW_delay_T delay_DW;

/* Real-time model */
static RT_MODEL_delay_T delay_M_;
RT_MODEL_delay_T *const delay_M = &delay_M_;

/* Model step function */
void delay_step(void)
{
  real_T rtb_Gain[128];
  real_T rtb_Gain1[128];
  real_T D;
  real_T Di;
  real_T Dmax;
  real_T g;
  real_T q;
  real_T r1;
  int32_T i;

  /* S-Function (pisound_in): '<Root>/PiSound Input' */

  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = delay_M->childSfunctions[0];
    sfcnOutputs(rts,0);
  }

  for (i = 0; i < 128; i++) {
    /* Gain: '<Root>/Gain' incorporates:
     *  DataTypeConversion: '<Root>/Data Type Conversion'
     */
    rtb_Gain[i] = delay_P.Gain_Gain * (real_T)delay_B.PiSoundInput_o1[i];

    /* Gain: '<Root>/Gain1' incorporates:
     *  DataTypeConversion: '<Root>/Data Type Conversion2'
     */
    rtb_Gain1[i] = delay_P.Gain1_Gain * (real_T)delay_B.PiSoundInput_o2[i];
  }

  /* MATLABSystem: '<Root>/MATLAB System' incorporates:
   *  Constant: '<Root>/Constant'
   *  Constant: '<Root>/Constant1'
   *  Constant: '<Root>/Constant2'
   *  MATLAB Function: '<Root>/MATLAB Function'
   */
  if (delay_DW.obj.Mode != delay_P.MATLABSystem_Mode) {
    delay_DW.obj.Mode = delay_P.MATLABSystem_Mode;
  }

  if (delay_DW.obj.Delay != delay_P.MATLABSystem_Delay) {
    delay_DW.obj.Delay = delay_P.MATLABSystem_Delay;
  }

  if (delay_DW.obj.g != delay_P.MATLABSystem_g) {
    delay_DW.obj.g = delay_P.MATLABSystem_g;
  }

  /*  inL, inR, mode, time, repeat(g) */
  /*  Entradas de control (escalares) */
  g = fmax(-1.0, fmin(1.0, delay_P.Repeats_rt));

  /*  [-1,1] */
  if (delay_P.Mode_rt != 1) {
    if (g >= 1.0) {
      g = 0.999;
    }

    if (g <= -1.0) {
      g = -0.999;
    }
  }

  Dmax = delay_DW.obj.L - 2.0;
  for (i = 0; i < 128; i++) {
    /*  Retardo en muestras (fraccionario permitido) */
    D = delay_P.Time_rt * 48000.0;

    /*  clamp robusto */
    if (D < 1.0) {
      D = 1.0;
    }

    if (D > Dmax) {
      D = Dmax;
    }

    Di = floor(D);
    D -= Di;
    Di = delay_DW.obj.w - Di;
    if (delay_DW.obj.L == 0.0) {
      q = Di - 1.0;
      if (Di - 1.0 == 0.0) {
        q = delay_DW.obj.L;
      }
    } else if (rtIsNaN(Di - 1.0) || rtIsNaN(delay_DW.obj.L) || rtIsInf(Di - 1.0))
    {
      q = (rtNaN);
    } else if (rtIsInf(delay_DW.obj.L)) {
      if (delay_DW.obj.L > 0.0) {
        if (Di - 1.0 > 0.0) {
          q = Di - 1.0;
        } else if (Di - 1.0 < 0.0) {
          q = delay_DW.obj.L;
        } else {
          q = 0.0;
        }
      } else if (Di - 1.0 > 0.0) {
        q = delay_DW.obj.L;
      } else if (Di - 1.0 < 0.0) {
        q = Di - 1.0;
      } else {
        q = -0.0;
      }
    } else {
      if (delay_DW.obj.L > floor(delay_DW.obj.L)) {
        q = fabs((Di - 1.0) / delay_DW.obj.L);
        if (fabs(q - floor(q + 0.5)) > 2.2204460492503131E-16 * q) {
          q = fmod(Di - 1.0, delay_DW.obj.L);
        } else {
          q = 0.0;
        }
      } else {
        q = fmod(Di - 1.0, delay_DW.obj.L);
      }

      if (q == 0.0) {
        q = delay_DW.obj.L * 0.0;
      } else if (q < 0.0) {
        if (delay_DW.obj.L > 0.0) {
          q += delay_DW.obj.L;
        }
      } else if ((q > 0.0) && (delay_DW.obj.L < 0.0)) {
        q += delay_DW.obj.L;
      }
    }

    r1 = (q + 1.0) - 1.0;
    if ((q + 1.0) - 1.0 < 1.0) {
      r1 = delay_DW.obj.L;
    }

    switch (delay_P.Mode_rt) {
     case 0U:
      /*  pure delay */
      Di = delay_DW.obj.xBufL[(int32_T)(q + 1.0) - 1] * (1.0 - D) +
        delay_DW.obj.xBufL[(int32_T)r1 - 1] * D;
      D = delay_DW.obj.xBufR[(int32_T)(q + 1.0) - 1] * (1.0 - D) +
        delay_DW.obj.xBufR[(int32_T)r1 - 1] * D;
      break;

     case 1U:
      /*  comb FIR */
      Di = (delay_DW.obj.xBufL[(int32_T)(q + 1.0) - 1] * (1.0 - D) +
            delay_DW.obj.xBufL[(int32_T)r1 - 1] * D) * g + rtb_Gain[i];
      D = (delay_DW.obj.xBufR[(int32_T)(q + 1.0) - 1] * (1.0 - D) +
           delay_DW.obj.xBufR[(int32_T)r1 - 1] * D) * g + rtb_Gain1[i];
      break;

     case 2U:
      /*  comb IIR */
      Di = (delay_DW.obj.yBufL[(int32_T)(q + 1.0) - 1] * (1.0 - D) +
            delay_DW.obj.yBufL[(int32_T)r1 - 1] * D) * g + rtb_Gain[i];
      D = (delay_DW.obj.yBufR[(int32_T)(q + 1.0) - 1] * (1.0 - D) +
           delay_DW.obj.yBufR[(int32_T)r1 - 1] * D) * g + rtb_Gain1[i];
      break;

     case 3U:
      /*  all-pass */
      Di = ((delay_DW.obj.xBufL[(int32_T)(q + 1.0) - 1] * (1.0 - D) +
             delay_DW.obj.xBufL[(int32_T)r1 - 1] * D) + -g * rtb_Gain[i]) +
        (delay_DW.obj.yBufL[(int32_T)(q + 1.0) - 1] * (1.0 - D) +
         delay_DW.obj.yBufL[(int32_T)r1 - 1] * D) * g;
      D = ((delay_DW.obj.xBufR[(int32_T)(q + 1.0) - 1] * (1.0 - D) +
            delay_DW.obj.xBufR[(int32_T)r1 - 1] * D) + -g * rtb_Gain1[i]) +
        (delay_DW.obj.yBufR[(int32_T)(q + 1.0) - 1] * (1.0 - D) +
         delay_DW.obj.yBufR[(int32_T)r1 - 1] * D) * g;
      break;

     default:
      Di = rtb_Gain[i];
      D = rtb_Gain1[i];
      break;
    }

    r1 = rtb_Gain[i];
    delay_DW.obj.xBufL[(int32_T)delay_DW.obj.w - 1] = r1;
    q = rtb_Gain1[i];
    delay_DW.obj.xBufR[(int32_T)delay_DW.obj.w - 1] = q;
    delay_DW.obj.yBufL[(int32_T)delay_DW.obj.w - 1] = Di;
    delay_DW.obj.yBufR[(int32_T)delay_DW.obj.w - 1] = D;
    delay_DW.obj.w++;
    if (delay_DW.obj.w > delay_DW.obj.L) {
      delay_DW.obj.w = 1.0;
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion1' incorporates:
     *  Constant: '<Root>/Constant1'
     *  Gain: '<Root>/Gain2'
     *  Gain: '<Root>/Gain4'
     *  Gain: '<Root>/Gain5'
     *  MATLAB Function: '<Root>/MATLAB Function'
     *  Sum: '<Root>/Add'
     */
    Di = floor(((1.0 - Mix_rt) * r1 + Mix_rt * Di) * delay_P.Gain2_Gain);
    if (rtIsNaN(Di) || rtIsInf(Di)) {
      Di = 0.0;
    } else {
      Di = fmod(Di, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion1' */
    delay_B.DataTypeConversion1[i] = Di < 0.0 ? -(int32_T)(uint32_T)-Di :
      (int32_T)(uint32_T)Di;

    /* DataTypeConversion: '<Root>/Data Type Conversion3' incorporates:
     *  Gain: '<Root>/Gain3'
     *  Gain: '<Root>/Gain6'
     *  Gain: '<Root>/Gain7'
     *  Sum: '<Root>/Add1'
     */
    Di = floor(((1.0 - Mix_rt) * q + Mix_rt * D) * delay_P.Gain3_Gain);
    if (rtIsNaN(Di) || rtIsInf(Di)) {
      Di = 0.0;
    } else {
      Di = fmod(Di, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion3' */
    delay_B.DataTypeConversion3[i] = Di < 0.0 ? -(int32_T)(uint32_T)-Di :
      (int32_T)(uint32_T)Di;
  }

  /* End of MATLABSystem: '<Root>/MATLAB System' */

  /* S-Function (pisound_out): '<Root>/PiSound Output' */

  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = delay_M->childSfunctions[1];
    sfcnOutputs(rts,0);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  delay_M->Timing.t[0] =
    ((time_T)(++delay_M->Timing.clockTick0)) * delay_M->Timing.stepSize0;
}

/* Model initialize function */
void delay_initialize(void)
{
  /* Registration code */
  rtsiSetSolverName(&delay_M->solverInfo,"FixedStepDiscrete");
  delay_M->solverInfoPtr = (&delay_M->solverInfo);

  /* Initialize timing info */
  {
    int_T *mdlTsMap = delay_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    delay_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    delay_M->Timing.sampleTimes = (&delay_M->Timing.sampleTimesArray[0]);
    delay_M->Timing.offsetTimes = (&delay_M->Timing.offsetTimesArray[0]);

    /* task periods */
    delay_M->Timing.sampleTimes[0] = (0.0026666666666666666);

    /* task offsets */
    delay_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(delay_M, &delay_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = delay_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    delay_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(delay_M, -1);
  delay_M->Timing.stepSize0 = 0.0026666666666666666;
  delay_M->solverInfoPtr = (&delay_M->solverInfo);
  delay_M->Timing.stepSize = (0.0026666666666666666);
  rtsiSetFixedStepSize(&delay_M->solverInfo, 0.0026666666666666666);
  rtsiSetSolverMode(&delay_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* child S-Function registration */
  {
    RTWSfcnInfo *sfcnInfo = &delay_M->NonInlinedSFcns.sfcnInfo;
    delay_M->sfcnInfo = (sfcnInfo);
    rtssSetErrorStatusPtr(sfcnInfo, (&rtmGetErrorStatus(delay_M)));
    delay_M->Sizes.numSampTimes = (1);
    rtssSetNumRootSampTimesPtr(sfcnInfo, &delay_M->Sizes.numSampTimes);
    delay_M->NonInlinedSFcns.taskTimePtrs[0] = (&rtmGetTPtr(delay_M)[0]);
    rtssSetTPtrPtr(sfcnInfo,delay_M->NonInlinedSFcns.taskTimePtrs);
    rtssSetTStartPtr(sfcnInfo, &rtmGetTStart(delay_M));
    rtssSetTFinalPtr(sfcnInfo, &rtmGetTFinal(delay_M));
    rtssSetTimeOfLastOutputPtr(sfcnInfo, &rtmGetTimeOfLastOutput(delay_M));
    rtssSetStepSizePtr(sfcnInfo, &delay_M->Timing.stepSize);
    rtssSetStopRequestedPtr(sfcnInfo, &rtmGetStopRequested(delay_M));
    rtssSetDerivCacheNeedsResetPtr(sfcnInfo, &delay_M->derivCacheNeedsReset);
    rtssSetZCCacheNeedsResetPtr(sfcnInfo, &delay_M->zCCacheNeedsReset);
    rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(sfcnInfo,
      &delay_M->CTOutputIncnstWithState);
    rtssSetSampleHitsPtr(sfcnInfo, &delay_M->Timing.sampleHits);
    rtssSetPerTaskSampleHitsPtr(sfcnInfo, &delay_M->Timing.perTaskSampleHits);
    rtssSetSimModePtr(sfcnInfo, &delay_M->simMode);
    rtssSetSolverInfoPtr(sfcnInfo, &delay_M->solverInfoPtr);
  }

  delay_M->Sizes.numSFcns = (2);

  /* register each child */
  {
    (void) memset((void *)&delay_M->NonInlinedSFcns.childSFunctions[0], 0,
                  2*sizeof(SimStruct));
    delay_M->childSfunctions = (&delay_M->NonInlinedSFcns.childSFunctionPtrs[0]);
    delay_M->childSfunctions[0] = (&delay_M->NonInlinedSFcns.childSFunctions[0]);
    delay_M->childSfunctions[1] = (&delay_M->NonInlinedSFcns.childSFunctions[1]);

    /* Level2 S-Function Block: delay/<Root>/PiSound Input (pisound_in) */
    {
      SimStruct *rts = delay_M->childSfunctions[0];

      /* timing info */
      time_T *sfcnPeriod = delay_M->NonInlinedSFcns.Sfcn0.sfcnPeriod;
      time_T *sfcnOffset = delay_M->NonInlinedSFcns.Sfcn0.sfcnOffset;
      int_T *sfcnTsMap = delay_M->NonInlinedSFcns.Sfcn0.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &delay_M->NonInlinedSFcns.blkInfo2[0]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &delay_M->NonInlinedSFcns.inputOutputPortInfo2[0]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, delay_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &delay_M->NonInlinedSFcns.methods2[0]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &delay_M->NonInlinedSFcns.methods3[0]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &delay_M->NonInlinedSFcns.methods4[0]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &delay_M->NonInlinedSFcns.statesInfo2[0]);
        ssSetPeriodicStatesInfo(rts,
          &delay_M->NonInlinedSFcns.periodicStatesInfo[0]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &delay_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &delay_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &delay_M->NonInlinedSFcns.Sfcn0.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &delay_M->NonInlinedSFcns.Sfcn0.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 128);
          ssSetOutputPortSignal(rts, 0, ((int32_T *) delay_B.PiSoundInput_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 128);
          ssSetOutputPortSignal(rts, 1, ((int32_T *) delay_B.PiSoundInput_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "PiSound Input");
      ssSetPath(rts, "delay/PiSound Input");
      ssSetRTModel(rts,delay_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &delay_M->NonInlinedSFcns.Sfcn0.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)delay_P.PiSoundInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)delay_P.PiSoundInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)delay_P.PiSoundInput_P3_Size);
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

    /* Level2 S-Function Block: delay/<Root>/PiSound Output (pisound_out) */
    {
      SimStruct *rts = delay_M->childSfunctions[1];

      /* timing info */
      time_T *sfcnPeriod = delay_M->NonInlinedSFcns.Sfcn1.sfcnPeriod;
      time_T *sfcnOffset = delay_M->NonInlinedSFcns.Sfcn1.sfcnOffset;
      int_T *sfcnTsMap = delay_M->NonInlinedSFcns.Sfcn1.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &delay_M->NonInlinedSFcns.blkInfo2[1]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &delay_M->NonInlinedSFcns.inputOutputPortInfo2[1]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, delay_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &delay_M->NonInlinedSFcns.methods2[1]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &delay_M->NonInlinedSFcns.methods3[1]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &delay_M->NonInlinedSFcns.methods4[1]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &delay_M->NonInlinedSFcns.statesInfo2[1]);
        ssSetPeriodicStatesInfo(rts,
          &delay_M->NonInlinedSFcns.periodicStatesInfo[1]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts,
          &delay_M->NonInlinedSFcns.Sfcn1.inputPortInfo[0]);
        ssSetPortInfoForInputs(rts,
          &delay_M->NonInlinedSFcns.Sfcn1.inputPortInfo[0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &delay_M->NonInlinedSFcns.Sfcn1.inputPortUnits[0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &delay_M->NonInlinedSFcns.Sfcn1.inputPortCoSimAttribute[0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          int32_T const **sfcnUPtrs = (int32_T const **)
            &delay_M->NonInlinedSFcns.Sfcn1.UPtrs0;

          {
            int_T i1;
            const int32_T *u0 = delay_B.DataTypeConversion1;
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
            &delay_M->NonInlinedSFcns.Sfcn1.UPtrs1;

          {
            int_T i1;
            const int32_T *u1 = delay_B.DataTypeConversion3;
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
      ssSetPath(rts, "delay/PiSound Output");
      ssSetRTModel(rts,delay_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &delay_M->NonInlinedSFcns.Sfcn1.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)delay_P.PiSoundOutput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)delay_P.PiSoundOutput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)delay_P.PiSoundOutput_P3_Size);
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
  delay_DW.obj.Mode = delay_P.MATLABSystem_Mode;
  delay_DW.obj.Delay = delay_P.MATLABSystem_Delay;
  delay_DW.obj.g = delay_P.MATLABSystem_g;
  delay_DW.obj.isInitialized = 1;

  /*  inL, inR, mode, time, repeat(g) */
  /*  L constante para codegen (deriva de Nontunable Fs y MaxDelay) */
  delay_DW.obj.L = 96002.0;

  /* InitializeConditions for MATLABSystem: '<Root>/MATLAB System' */
  memset(&delay_DW.obj.xBufL[0], 0, 96002U * sizeof(real_T));
  memset(&delay_DW.obj.xBufR[0], 0, 96002U * sizeof(real_T));
  memset(&delay_DW.obj.yBufL[0], 0, 96002U * sizeof(real_T));
  memset(&delay_DW.obj.yBufR[0], 0, 96002U * sizeof(real_T));
  delay_DW.obj.w = 1.0;
}

/* Model terminate function */
void delay_terminate(void)
{
  /* Terminate for S-Function (pisound_in): '<Root>/PiSound Input' */
  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = delay_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (pisound_out): '<Root>/PiSound Output' */
  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = delay_M->childSfunctions[1];
    sfcnTerminate(rts);
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
