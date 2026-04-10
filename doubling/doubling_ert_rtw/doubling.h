/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: doubling.h
 *
 * Code generated for Simulink model 'doubling'.
 *
 * Model version                  : 1.9
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Fri Apr 10 16:11:12 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-A
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
#include "rt_nonfinite.h"
#include "math.h"
#endif                                 /* doubling_COMMON_INCLUDES_ */

#include "doubling_types.h"
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
  real_T sine[1024];
  real_T w4[1024];
  real_T w5[1024];
  real_T CtrlIn_o1;                    /* '<Root>/Ctrl In' */
  real_T CtrlIn_o2;                    /* '<Root>/Ctrl In' */
  real_T CtrlIn_o3;                    /* '<Root>/Ctrl In' */
  real_T CtrlIn_o4;                    /* '<Root>/Ctrl In' */
  int32_T PisoundIn_o1[128];           /* '<Root>/Pisound In' */
  int32_T PisoundIn_o2[128];           /* '<Root>/Pisound In' */
  int32_T DataTypeConversion1[128];    /* '<Root>/Data Type Conversion1' */
  int32_T DataTypeConversion3[128];    /* '<Root>/Data Type Conversion3' */
} B_doubling_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  vco_mono_sys_doubling_T obj;         /* '<Root>/LFO' */
  vcd_sys_doubling_T obj_l;            /* '<Root>/VCD' */
  mix_sys_doubling_T obj_e;            /* '<Root>/MIX' */
  uint32_T state_j[625];               /* '<Root>/LFO' */
} DW_doubling_T;

/* Constant parameters (default storage) */
typedef struct {
  /* Pooled Parameter (Expression: )
   * Referenced by:
   *   '<Root>/Ctrl In'
   *   '<Root>/Pisound In'
   *   '<Root>/ctrl_out'
   *   '<Root>/pisound_out'
   */
  real_T CtrlIn_P1_Size[2];

  /* Pooled Parameter (Expression: 48000)
   * Referenced by:
   *   '<Root>/Ctrl In'
   *   '<Root>/Pisound In'
   *   '<Root>/ctrl_out'
   *   '<Root>/pisound_out'
   */
  real_T pooled2;

  /* Pooled Parameter (Expression: )
   * Referenced by:
   *   '<Root>/Ctrl In'
   *   '<Root>/Pisound In'
   *   '<Root>/ctrl_out'
   *   '<Root>/pisound_out'
   */
  real_T CtrlIn_P2_Size[2];

  /* Pooled Parameter (Expression: 128)
   * Referenced by:
   *   '<Root>/Ctrl In'
   *   '<Root>/Pisound In'
   *   '<Root>/ctrl_out'
   *   '<Root>/pisound_out'
   */
  real_T pooled3;

  /* Pooled Parameter (Expression: )
   * Referenced by:
   *   '<Root>/Ctrl In'
   *   '<Root>/ctrl_out'
   */
  real_T CtrlIn_P3_Size[2];

  /* Pooled Parameter (Expression: 4)
   * Referenced by:
   *   '<Root>/Ctrl In'
   *   '<Root>/ctrl_out'
   */
  real_T pooled4;

  /* Pooled Parameter (Expression: )
   * Referenced by:
   *   '<Root>/Pisound In'
   *   '<Root>/pisound_out'
   */
  real_T PisoundIn_P3_Size[2];

  /* Pooled Parameter (Expression: 0)
   * Referenced by:
   *   '<Root>/Pisound In'
   *   '<Root>/pisound_out'
   */
  real_T pooled5;
} ConstP_doubling_T;

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

/* Block signals (default storage) */
extern B_doubling_T doubling_B;

/* Block states (default storage) */
extern DW_doubling_T doubling_DW;

/* Constant parameters (default storage) */
extern const ConstP_doubling_T doubling_ConstP;

/* Model entry point functions */
extern void doubling_initialize(void);
extern void doubling_step(void);
extern void doubling_terminate(void);

/* Real-time Model object */
extern RT_MODEL_doubling_T *const doubling_M;

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
