/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: dyn.h
 *
 * Code generated for Simulink model 'dyn'.
 *
 * Model version                  : 1.22
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Fri Mar  6 08:57:56 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef dyn_h_
#define dyn_h_
#ifndef dyn_COMMON_INCLUDES_
#define dyn_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rt_nonfinite.h"
#include "math.h"
#endif                                 /* dyn_COMMON_INCLUDES_ */

#include "dyn_types.h"
#include "rtGetInf.h"
#include "rtGetNaN.h"
#include <string.h>
#include <stddef.h>

/* Macros for accessing real-time model data structure */
#ifndef rtmGetFinalTime
#define rtmGetFinalTime(rtm)           ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetSampleHitArray
#define rtmGetSampleHitArray(rtm)      ((rtm)->Timing.sampleHitArray)
#endif

#ifndef rtmGetStepSize
#define rtmGetStepSize(rtm)            ((rtm)->Timing.stepSize)
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
  int32_T PiSoundInput_o1[128];        /* '<Root>/PiSound Input' */
  int32_T PiSoundInput_o2[128];        /* '<Root>/PiSound Input' */
  int32_T DataTypeConversion1[128];    /* '<Root>/Data Type Conversion1' */
  int32_T DataTypeConversion3[128];    /* '<Root>/Data Type Conversion3' */
} B_dyn_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  mix_sys_dyn_T obj;                   /* '<Root>/MATLAB System6' */
  env_sys_dyn_T obj_n;                 /* '<Root>/MATLAB System1' */
  arf_sys_dyn_T obj_m;                 /* '<Root>/MATLAB System3' */
  gc_sys_dyn_T obj_o;                  /* '<Root>/MATLAB System2' */
} DW_dyn_T;

/* Parameters (default storage) */
struct P_dyn_T_ {
  real_T Attack_rt;                    /* Variable: Attack_rt
                                        * Referenced by: '<Root>/Constant2'
                                        */
  real_T Knee_rt;                      /* Variable: Knee_rt
                                        * Referenced by: '<Root>/Constant5'
                                        */
  real_T Makeup_rt;                    /* Variable: Makeup_rt
                                        * Referenced by: '<Root>/Constant6'
                                        */
  real_T Mix_rt;                       /* Variable: Mix_rt
                                        * Referenced by: '<Root>/Constant15'
                                        */
  real_T Ratio_rt;                     /* Variable: Ratio_rt
                                        * Referenced by: '<Root>/Constant7'
                                        */
  real_T Release_rt;                   /* Variable: Release_rt
                                        * Referenced by: '<Root>/Constant3'
                                        */
  real_T Threshold_rt;                 /* Variable: Threshold_rt
                                        * Referenced by: '<Root>/Constant4'
                                        */
  real_T MATLABSystem1_Eps;            /* Expression: 1e-12
                                        * Referenced by: '<Root>/MATLAB System1'
                                        */
  real_T MATLABSystem1_TauRMS;         /* Expression: 0.02
                                        * Referenced by: '<Root>/MATLAB System1'
                                        */
  real_T MATLABSystem2_Eps;            /* Expression: 1e-12
                                        * Referenced by: '<Root>/MATLAB System2'
                                        */
  real_T MATLABSystem2_Floor_dB;       /* Expression: -80
                                        * Referenced by: '<Root>/MATLAB System2'
                                        */
  real_T MATLABSystem3_AttackDefault;  /* Expression: 0.010
                                        * Referenced by: '<Root>/MATLAB System3'
                                        */
  real_T MATLABSystem3_ReleaseDefault; /* Expression: 0.100
                                        * Referenced by: '<Root>/MATLAB System3'
                                        */
  real_T MATLABSystem6_SmoothTime;     /* Expression: 0.01
                                        * Referenced by: '<Root>/MATLAB System6'
                                        */
  real_T PiSoundInput_P1_Size[2];    /* Computed Parameter: PiSoundInput_P1_Size
                                      * Referenced by: '<Root>/PiSound Input'
                                      */
  real_T PiSoundInput_P1;              /* Expression: 48000
                                        * Referenced by: '<Root>/PiSound Input'
                                        */
  real_T PiSoundInput_P2_Size[2];    /* Computed Parameter: PiSoundInput_P2_Size
                                      * Referenced by: '<Root>/PiSound Input'
                                      */
  real_T PiSoundInput_P2;              /* Expression: 128
                                        * Referenced by: '<Root>/PiSound Input'
                                        */
  real_T PiSoundInput_P3_Size[2];    /* Computed Parameter: PiSoundInput_P3_Size
                                      * Referenced by: '<Root>/PiSound Input'
                                      */
  real_T PiSoundInput_P3;              /* Expression: 0
                                        * Referenced by: '<Root>/PiSound Input'
                                        */
  real_T Gain_Gain;                    /* Expression: 1/(2^31-1)
                                        * Referenced by: '<Root>/Gain'
                                        */
  real_T Gain1_Gain;                   /* Expression: 1/(2^31-1)
                                        * Referenced by: '<Root>/Gain1'
                                        */
  real_T Gain2_Gain;                   /* Expression: 2^31-1
                                        * Referenced by: '<Root>/Gain2'
                                        */
  real_T Gain3_Gain;                   /* Expression: 2^31-1
                                        * Referenced by: '<Root>/Gain3'
                                        */
  real_T PiSoundOutput_P1_Size[2];  /* Computed Parameter: PiSoundOutput_P1_Size
                                     * Referenced by: '<Root>/PiSound Output'
                                     */
  real_T PiSoundOutput_P1;             /* Expression: 48000
                                        * Referenced by: '<Root>/PiSound Output'
                                        */
  real_T PiSoundOutput_P2_Size[2];  /* Computed Parameter: PiSoundOutput_P2_Size
                                     * Referenced by: '<Root>/PiSound Output'
                                     */
  real_T PiSoundOutput_P2;             /* Expression: 128
                                        * Referenced by: '<Root>/PiSound Output'
                                        */
  real_T PiSoundOutput_P3_Size[2];  /* Computed Parameter: PiSoundOutput_P3_Size
                                     * Referenced by: '<Root>/PiSound Output'
                                     */
  real_T PiSoundOutput_P3;             /* Expression: 0
                                        * Referenced by: '<Root>/PiSound Output'
                                        */
  boolean_T MATLABSystem6_IndepInDB;   /* Expression: false
                                        * Referenced by: '<Root>/MATLAB System6'
                                        */
  uint8_T MATLABSystem1_ModeDefault;   /* Expression: uint8( 1 )
                                        * Referenced by: '<Root>/MATLAB System1'
                                        */
  uint8_T Constant_Value;              /* Computed Parameter: Constant_Value
                                        * Referenced by: '<Root>/Constant'
                                        */
  uint8_T Constant1_Value;             /* Computed Parameter: Constant1_Value
                                        * Referenced by: '<Root>/Constant1'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_dyn_T {
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
    SimStruct childSFunctions[2];
    SimStruct *childSFunctionPtrs[2];
    struct _ssBlkInfo2 blkInfo2[2];
    struct _ssSFcnModelMethods2 methods2[2];
    struct _ssSFcnModelMethods3 methods3[2];
    struct _ssSFcnModelMethods4 methods4[2];
    struct _ssStatesInfo2 statesInfo2[2];
    ssPeriodicStatesInfo periodicStatesInfo[2];
    struct _ssPortInfo2 inputOutputPortInfo2[2];
    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      struct _ssPortOutputs outputPortInfo[2];
      struct _ssOutPortUnit outputPortUnits[2];
      struct _ssOutPortCoSimAttribute outputPortCoSimAttribute[2];
      uint_T attribs[3];
      mxArray *params[3];
    } Sfcn0;

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
    } Sfcn1;
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
extern P_dyn_T dyn_P;

/* Block signals (default storage) */
extern B_dyn_T dyn_B;

/* Block states (default storage) */
extern DW_dyn_T dyn_DW;

/* Model entry point functions */
extern void dyn_initialize(void);
extern void dyn_step(void);
extern void dyn_terminate(void);

/* Real-time Model object */
extern RT_MODEL_dyn_T *const dyn_M;

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
 * '<Root>' : 'dyn'
 */
#endif                                 /* dyn_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
