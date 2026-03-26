/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: pitch.h
 *
 * Code generated for Simulink model 'pitch'.
 *
 * Model version                  : 1.43
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Fri Mar  6 13:38:43 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef pitch_h_
#define pitch_h_
#ifndef pitch_COMMON_INCLUDES_
#define pitch_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rt_nonfinite.h"
#include "math.h"
#endif                                 /* pitch_COMMON_INCLUDES_ */

#include "pitch_types.h"
#include "rtGetNaN.h"
#include "rtGetInf.h"
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

/* Block signals for system '<Root>/VCD1' */
typedef struct {
  real_T VCD1_o1[128];                 /* '<Root>/VCD1' */
  real_T VCD1_o2[128];                 /* '<Root>/VCD1' */
} B_VCD1_pitch_T;

/* Block states (default storage) for system '<Root>/VCD1' */
typedef struct {
  vcd_sys_pitch_T obj;                 /* '<Root>/VCD1' */
  real_T w;                            /* '<Root>/VCD1' */
  boolean_T objisempty;                /* '<Root>/VCD1' */
} DW_VCD1_pitch_T;

/* Block signals (default storage) */
typedef struct {
  real_T sine[1024];
  real_T w4[1024];
  real_T w5[1024];
  int32_T PiSoundInput_o1[128];        /* '<Root>/PiSound Input' */
  int32_T PiSoundInput_o2[128];        /* '<Root>/PiSound Input' */
  int32_T DataTypeConversion1[128];    /* '<Root>/Data Type Conversion1' */
  int32_T DataTypeConversion3[128];    /* '<Root>/Data Type Conversion3' */
  B_VCD1_pitch_T VCD2;                 /* '<Root>/VCD1' */
  B_VCD1_pitch_T VCD1;                 /* '<Root>/VCD1' */
} B_pitch_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  vco_mono_sys_pitch_T obj;            /* '<Root>/VCO' */
  mix_sys_pitch_T obj_j;               /* '<Root>/MIX' */
  real_T gd1;                          /* '<Root>/MIX' */
  real_T gd2;                          /* '<Root>/MIX' */
  real_T gw1;                          /* '<Root>/MIX' */
  real_T gw2;                          /* '<Root>/MIX' */
  uint32_T state_i[625];               /* '<Root>/VCO' */
  DW_VCD1_pitch_T VCD2;                /* '<Root>/VCD1' */
  DW_VCD1_pitch_T VCD1;                /* '<Root>/VCD1' */
} DW_pitch_T;

/* Parameters for system: '<Root>/VCD1' */
struct P_VCD1_pitch_T_ {
  real_T VCD1_Delay;                   /* Expression: 0.1
                                        * Referenced by: '<Root>/VCD1'
                                        */
  real_T VCD1_g;                       /* Expression: 0.5
                                        * Referenced by: '<Root>/VCD1'
                                        */
  uint8_T VCD1_Mode;                   /* Expression: uint8(1)
                                        * Referenced by: '<Root>/VCD1'
                                        */
};

/* Parameters (default storage) */
struct P_pitch_T_ {
  real_T Semitone_rt;                  /* Variable: Semitone_rt
                                        * Referenced by: '<Root>/Constant1'
                                        */
  real_T MIX_SmoothTime;               /* Expression: 0.01
                                        * Referenced by: '<Root>/MIX'
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
  real_T Constant2_Value;              /* Expression: 0
                                        * Referenced by: '<Root>/Constant2'
                                        */
  real_T Gain1_Gain;                   /* Expression: 1/(2^31-1)
                                        * Referenced by: '<Root>/Gain1'
                                        */
  real_T Constant7_Value;              /* Expression: 0.005
                                        * Referenced by: '<Root>/Constant7'
                                        */
  real_T Constant10_Value;             /* Expression: 0.25
                                        * Referenced by: '<Root>/Constant10'
                                        */
  real_T Constant3_Value;              /* Expression: 1
                                        * Referenced by: '<Root>/Constant3'
                                        */
  real_T Constant4_Value;              /* Expression: 0
                                        * Referenced by: '<Root>/Constant4'
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
  boolean_T MIX_IndepInDB;             /* Expression: false
                                        * Referenced by: '<Root>/MIX'
                                        */
  uint8_T Constant_Value;              /* Computed Parameter: Constant_Value
                                        * Referenced by: '<Root>/Constant'
                                        */
  uint8_T Constant5_Value;             /* Computed Parameter: Constant5_Value
                                        * Referenced by: '<Root>/Constant5'
                                        */
  uint8_T Constant6_Value;             /* Computed Parameter: Constant6_Value
                                        * Referenced by: '<Root>/Constant6'
                                        */
  P_VCD1_pitch_T VCD2;                 /* '<Root>/VCD1' */
  P_VCD1_pitch_T VCD1;                 /* '<Root>/VCD1' */
};

/* Real-time Model Data Structure */
struct tag_RTM_pitch_T {
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
extern P_pitch_T pitch_P;

/* Block signals (default storage) */
extern B_pitch_T pitch_B;

/* Block states (default storage) */
extern DW_pitch_T pitch_DW;

/* Model entry point functions */
extern void pitch_initialize(void);
extern void pitch_step(void);
extern void pitch_terminate(void);

/* Real-time Model object */
extern RT_MODEL_pitch_T *const pitch_M;

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
 * '<Root>' : 'pitch'
 * '<S1>'   : 'pitch/MATLAB Function'
 * '<S2>'   : 'pitch/MATLAB Function2'
 */
#endif                                 /* pitch_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
