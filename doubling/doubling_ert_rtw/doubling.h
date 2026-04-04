/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: doubling.h
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

#ifndef doubling_h_
#define doubling_h_
#ifndef doubling_COMMON_INCLUDES_
#define doubling_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#endif                                 /* doubling_COMMON_INCLUDES_ */

#include "doubling_types.h"
#include <string.h>
#include <stddef.h>
#include "rt_nonfinite.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm) ((rtm)->CTOutputIncnstWithState)
#endif

#ifndef rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm, val) ((rtm)->CTOutputIncnstWithState = (val))
#endif

#ifndef rtmGetDerivCacheNeedsReset
#define rtmGetDerivCacheNeedsReset(rtm) ((rtm)->derivCacheNeedsReset)
#endif

#ifndef rtmSetDerivCacheNeedsReset
#define rtmSetDerivCacheNeedsReset(rtm, val) ((rtm)->derivCacheNeedsReset = (val))
#endif

#ifndef rtmGetFinalTime
#define rtmGetFinalTime(rtm)           ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetSampleHitArray
#define rtmGetSampleHitArray(rtm)      ((rtm)->Timing.sampleHitArray)
#endif

#ifndef rtmGetStepSize
#define rtmGetStepSize(rtm)            ((rtm)->Timing.stepSize)
#endif

#ifndef rtmGetZCCacheNeedsReset
#define rtmGetZCCacheNeedsReset(rtm)   ((rtm)->zCCacheNeedsReset)
#endif

#ifndef rtmSetZCCacheNeedsReset
#define rtmSetZCCacheNeedsReset(rtm, val) ((rtm)->zCCacheNeedsReset = (val))
#endif

#ifndef rtmGet_TimeOfLastOutput
#define rtmGet_TimeOfLastOutput(rtm)   ((rtm)->Timing.timeOfLastOutput)
#endif

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTFinal
#define rtmGetTFinal(rtm)              ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

#ifndef rtmGetTStart
#define rtmGetTStart(rtm)              ((rtm)->Timing.tStart)
#endif

#ifndef rtmGetTimeOfLastOutput
#define rtmGetTimeOfLastOutput(rtm)    ((rtm)->Timing.timeOfLastOutput)
#endif

/* Block signals (default storage) */
typedef struct {
  real_T SFunction_o1;                 /* '<Root>/S-Function' */
  real_T SFunction_o2;                 /* '<Root>/S-Function' */
  real_T SFunction_o3;                 /* '<Root>/S-Function' */
  real_T SFunction_o4;                 /* '<Root>/S-Function' */
  int32_T pisound_in_o1[128];          /* '<Root>/pisound_in' */
  int32_T pisound_in_o2[128];          /* '<Root>/pisound_in' */
  int32_T DTC_Out_L[128];              /* '<Root>/DTC_Out_L' */
  int32_T DTC_Out_R[128];              /* '<Root>/DTC_Out_R' */
} B_doubling_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T Delay_L_DSTATE[128];          /* '<Root>/Delay_L' */
} DW_doubling_T;

/* Parameters (default storage) */
struct P_doubling_T_ {
  real_T Spread;                       /* Variable: Spread
                                        * Referenced by: '<Root>/Gain_R_Spread'
                                        */
  real_T Delay_L_InitialCondition;     /* Expression: 0
                                        * Referenced by: '<Root>/Delay_L'
                                        */
  real_T SFunction_P1_Size[2];         /* Computed Parameter: SFunction_P1_Size
                                        * Referenced by: '<Root>/S-Function'
                                        */
  real_T SFunction_P1;                 /* Expression: 48000
                                        * Referenced by: '<Root>/S-Function'
                                        */
  real_T SFunction_P2_Size[2];         /* Computed Parameter: SFunction_P2_Size
                                        * Referenced by: '<Root>/S-Function'
                                        */
  real_T SFunction_P2;                 /* Expression: 128
                                        * Referenced by: '<Root>/S-Function'
                                        */
  real_T SFunction_P3_Size[2];         /* Computed Parameter: SFunction_P3_Size
                                        * Referenced by: '<Root>/S-Function'
                                        */
  real_T SFunction_P3;                 /* Expression: 4
                                        * Referenced by: '<Root>/S-Function'
                                        */
  real_T pisound_in_P1_Size[2];        /* Computed Parameter: pisound_in_P1_Size
                                        * Referenced by: '<Root>/pisound_in'
                                        */
  real_T pisound_in_P1;                /* Expression: 48000
                                        * Referenced by: '<Root>/pisound_in'
                                        */
  real_T pisound_in_P2_Size[2];        /* Computed Parameter: pisound_in_P2_Size
                                        * Referenced by: '<Root>/pisound_in'
                                        */
  real_T pisound_in_P2;                /* Expression: 128
                                        * Referenced by: '<Root>/pisound_in'
                                        */
  real_T pisound_in_P3_Size[2];        /* Computed Parameter: pisound_in_P3_Size
                                        * Referenced by: '<Root>/pisound_in'
                                        */
  real_T pisound_in_P3;                /* Expression: 0
                                        * Referenced by: '<Root>/pisound_in'
                                        */
  real_T Gain_Out_L_Gain;              /* Expression: 2147483647
                                        * Referenced by: '<Root>/Gain_Out_L'
                                        */
  real_T Gain_Out_R_Gain;              /* Expression: 2147483647
                                        * Referenced by: '<Root>/Gain_Out_R'
                                        */
  real_T SFunction1_P1_Size[2];        /* Computed Parameter: SFunction1_P1_Size
                                        * Referenced by: '<Root>/S-Function1'
                                        */
  real_T SFunction1_P1;                /* Expression: 48000
                                        * Referenced by: '<Root>/S-Function1'
                                        */
  real_T SFunction1_P2_Size[2];        /* Computed Parameter: SFunction1_P2_Size
                                        * Referenced by: '<Root>/S-Function1'
                                        */
  real_T SFunction1_P2;                /* Expression: 128
                                        * Referenced by: '<Root>/S-Function1'
                                        */
  real_T SFunction1_P3_Size[2];        /* Computed Parameter: SFunction1_P3_Size
                                        * Referenced by: '<Root>/S-Function1'
                                        */
  real_T SFunction1_P3;                /* Expression: 4
                                        * Referenced by: '<Root>/S-Function1'
                                        */
  real_T pisound_out_P1_Size[2];      /* Computed Parameter: pisound_out_P1_Size
                                       * Referenced by: '<Root>/pisound_out'
                                       */
  real_T pisound_out_P1;               /* Expression: 48000
                                        * Referenced by: '<Root>/pisound_out'
                                        */
  real_T pisound_out_P2_Size[2];      /* Computed Parameter: pisound_out_P2_Size
                                       * Referenced by: '<Root>/pisound_out'
                                       */
  real_T pisound_out_P2;               /* Expression: 128
                                        * Referenced by: '<Root>/pisound_out'
                                        */
  real_T pisound_out_P3_Size[2];      /* Computed Parameter: pisound_out_P3_Size
                                       * Referenced by: '<Root>/pisound_out'
                                       */
  real_T pisound_out_P3;               /* Expression: 0
                                        * Referenced by: '<Root>/pisound_out'
                                        */
  int32_T Gain_In_L_Gain;              /* Computed Parameter: Gain_In_L_Gain
                                        * Referenced by: '<Root>/Gain_In_L'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_doubling_T {
  struct SimStruct_tag * *childSfunctions;
  const char_T *errorStatus;
  SS_SimMode simMode;
  RTWSolverInfo solverInfo;
  RTWSolverInfo *solverInfoPtr;
  void *sfcnInfo;

  /*
   * NonInlinedSFcns:
   * The following substructure contains information regarding
   * non-inlined s-functions used in the model.
   */
  struct {
    RTWSfcnInfo sfcnInfo;
    time_T *taskTimePtrs[1];
    SimStruct childSFunctions[4];
    SimStruct *childSFunctionPtrs[4];
    struct _ssBlkInfo2 blkInfo2[4];
    struct _ssSFcnModelMethods2 methods2[4];
    struct _ssSFcnModelMethods3 methods3[4];
    struct _ssSFcnModelMethods4 methods4[4];
    struct _ssStatesInfo2 statesInfo2[4];
    ssPeriodicStatesInfo periodicStatesInfo[4];
    struct _ssPortInfo2 inputOutputPortInfo2[4];
    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      struct _ssPortOutputs outputPortInfo[4];
      struct _ssOutPortUnit outputPortUnits[4];
      struct _ssOutPortCoSimAttribute outputPortCoSimAttribute[4];
      uint_T attribs[3];
      mxArray *params[3];
    } Sfcn0;

    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      struct _ssPortOutputs outputPortInfo[2];
      struct _ssOutPortUnit outputPortUnits[2];
      struct _ssOutPortCoSimAttribute outputPortCoSimAttribute[2];
      uint_T attribs[3];
      mxArray *params[3];
    } Sfcn1;

    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      struct _ssPortInputs inputPortInfo[4];
      struct _ssInPortUnit inputPortUnits[4];
      struct _ssInPortCoSimAttribute inputPortCoSimAttribute[4];
      real_T const *UPtrs0[1];
      real_T const *UPtrs1[1];
      real_T const *UPtrs2[1];
      real_T const *UPtrs3[1];
      uint_T attribs[3];
      mxArray *params[3];
    } Sfcn2;

    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      struct _ssPortInputs inputPortInfo[2];
      struct _ssInPortUnit inputPortUnits[2];
      struct _ssInPortCoSimAttribute inputPortCoSimAttribute[2];
      real_T const *UPtrs0[128];
      real_T const *UPtrs1[128];
      uint_T attribs[3];
      mxArray *params[3];
    } Sfcn3;
  } NonInlinedSFcns;

  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    uint32_T options;
    int_T numContStates;
    int_T numU;
    int_T numY;
    int_T numSampTimes;
    int_T numBlocks;
    int_T numBlockIO;
    int_T numBlockPrms;
    int_T numDwork;
    int_T numSFcnPrms;
    int_T numSFcns;
    int_T numIports;
    int_T numOports;
    int_T numNonSampZCs;
    int_T sysDirFeedThru;
    int_T rtwGenSfcn;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    time_T stepSize;
    uint32_T clockTick0;
    time_T stepSize0;
    time_T tStart;
    time_T tFinal;
    time_T timeOfLastOutput;
    boolean_T stopRequestedFlag;
    time_T *sampleTimes;
    time_T *offsetTimes;
    int_T *sampleTimeTaskIDPtr;
    int_T *sampleHits;
    int_T *perTaskSampleHits;
    time_T *t;
    time_T sampleTimesArray[1];
    time_T offsetTimesArray[1];
    int_T sampleTimeTaskIDArray[1];
    int_T sampleHitArray[1];
    int_T perTaskSampleHitsArray[1];
    time_T tArray[1];
  } Timing;
};

/* Block parameters (default storage) */
extern P_doubling_T doubling_P;

/* Block signals (default storage) */
extern B_doubling_T doubling_B;

/* Block states (default storage) */
extern DW_doubling_T doubling_DW;

/* Model entry point functions */
extern void doubling_initialize(void);
extern void doubling_step(void);
extern void doubling_terminate(void);

/* Real-time Model object */
extern RT_MODEL_doubling_T *const doubling_M;
extern volatile boolean_T stopRequested;
extern volatile boolean_T runModel;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<Root>/Gain_In_R' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'doubling'
 */
#endif                                 /* doubling_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
