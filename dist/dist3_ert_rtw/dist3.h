/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: dist3.h
 *
 * Code generated for Simulink model 'dist3'.
 *
 * Model version                  : 1.38
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Thu Feb 26 14:59:43 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef dist3_h_
#define dist3_h_
#ifndef dist3_COMMON_INCLUDES_
#define dist3_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rt_nonfinite.h"
#include "math.h"
#endif                                 /* dist3_COMMON_INCLUDES_ */

#include "dist3_types.h"
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

/* Block signals for system '<Root>/MATLAB System5' */
typedef struct {
  real_T MATLABSystem5_o1[128];        /* '<Root>/MATLAB System5' */
  real_T MATLABSystem5_o2[128];        /* '<Root>/MATLAB System5' */
} B_MATLABSystem5_dist3_T;

/* Block states (default storage) for system '<Root>/MATLAB System5' */
typedef struct {
  vcf_sys_dist3_T obj;                 /* '<Root>/MATLAB System5' */
  real_T zL[2];                        /* '<Root>/MATLAB System5' */
  real_T zR[2];                        /* '<Root>/MATLAB System5' */
  boolean_T objisempty;                /* '<Root>/MATLAB System5' */
} DW_MATLABSystem5_dist3_T;

/* Block signals (default storage) */
typedef struct {
  uint32_T pisound_in_o4;              /* '<Root>/pisound_in' */
  int32_T pisound_in_o1[128];          /* '<Root>/pisound_in' */
  int32_T pisound_in_o2[128];          /* '<Root>/pisound_in' */
  int32_T DataTypeConversion1[128];    /* '<Root>/Data Type Conversion1' */
  int32_T DataTypeConversion3[128];    /* '<Root>/Data Type Conversion3' */
  uint8_T pisound_in_o3[128];          /* '<Root>/pisound_in' */
  B_MATLABSystem5_dist3_T MATLABSystem7;/* '<Root>/MATLAB System5' */
  B_MATLABSystem5_dist3_T MATLABSystem5;/* '<Root>/MATLAB System5' */
} B_dist3_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  lut_ws_sys_dist3_T obj;              /* '<Root>/MATLAB System4' */
  mix_sys_dist3_T obj_e;               /* '<Root>/MATLAB System6' */
  DW_MATLABSystem5_dist3_T MATLABSystem7;/* '<Root>/MATLAB System5' */
  DW_MATLABSystem5_dist3_T MATLABSystem5;/* '<Root>/MATLAB System5' */
} DW_dist3_T;

/* Parameters (default storage) */
struct P_dist3_T_ {
  real_T Amount_rt;                    /* Variable: Amount_rt
                                        * Referenced by: '<Root>/Constant5'
                                        */
  real_T Asym_rt;                      /* Variable: Asym_rt
                                        * Referenced by: '<Root>/Constant6'
                                        */
  real_T Level_rt;                     /* Variable: Level_rt
                                        * Referenced by: '<Root>/Constant8'
                                        */
  real_T Mix_rt;                       /* Variable: Mix_rt
                                        * Referenced by: '<Root>/Constant15'
                                        */
  real_T Tone_rt;                      /* Variable: Tone_rt
                                        * Referenced by: '<Root>/Constant7'
                                        */
  uint8_T Gain_rt;                     /* Variable: Gain_rt
                                        * Referenced by: '<Root>/Constant2'
                                        */
  uint8_T Shaper_rt;                   /* Variable: Shaper_rt
                                        * Referenced by: '<Root>/Constant9'
                                        */
  real_T MATLABSystem4_AmountDefault;  /* Expression: 12.0
                                        * Referenced by: '<Root>/MATLAB System4'
                                        */
  real_T MATLABSystem4_AsymDefault;    /* Expression: 0.0
                                        * Referenced by: '<Root>/MATLAB System4'
                                        */
  real_T MATLABSystem4_Cheb3Default;   /* Expression: 1.0
                                        * Referenced by: '<Root>/MATLAB System4'
                                        */
  real_T MATLABSystem4_Cheb5Default;   /* Expression: 0.0
                                        * Referenced by: '<Root>/MATLAB System4'
                                        */
  real_T MATLABSystem4_XUser[257];     /* Expression: linspace( -1, 1, 257 )
                                        * Referenced by: '<Root>/MATLAB System4'
                                        */
  real_T MATLABSystem4_YUser[257];     /* Expression: linspace( -1, 1, 257 )
                                        * Referenced by: '<Root>/MATLAB System4'
                                        */
  real_T MATLABSystem4_MUser;          /* Expression: 257
                                        * Referenced by: '<Root>/MATLAB System4'
                                        */
  real_T MATLABSystem4_Eps;            /* Expression: 1e-12
                                        * Referenced by: '<Root>/MATLAB System4'
                                        */
  real_T MATLABSystem6_SmoothTime;     /* Expression: 0.01
                                        * Referenced by: '<Root>/MATLAB System6'
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
  real_T Gain_Gain;                    /* Expression: 1/(2^31-1)
                                        * Referenced by: '<Root>/Gain'
                                        */
  real_T Gain1_Gain;                   /* Expression: 1/(2^31-1)
                                        * Referenced by: '<Root>/Gain1'
                                        */
  real_T Constant19_Value;             /* Expression: 180
                                        * Referenced by: '<Root>/Constant19'
                                        */
  real_T Constant16_Value;             /* Expression: 0.707
                                        * Referenced by: '<Root>/Constant16'
                                        */
  real_T Constant18_Value;             /* Expression: 0
                                        * Referenced by: '<Root>/Constant18'
                                        */
  real_T Constant10_Value;             /* Expression: 0.5
                                        * Referenced by: '<Root>/Constant10'
                                        */
  real_T Constant11_Value;             /* Expression: 0
                                        * Referenced by: '<Root>/Constant11'
                                        */
  real_T Constant12_Value;             /* Expression: 0.707
                                        * Referenced by: '<Root>/Constant12'
                                        */
  real_T Constant14_Value;             /* Expression: 3
                                        * Referenced by: '<Root>/Constant14'
                                        */
  real_T Gain2_Gain;                   /* Expression: 2^31-1
                                        * Referenced by: '<Root>/Gain2'
                                        */
  real_T Gain3_Gain;                   /* Expression: 2^31-1
                                        * Referenced by: '<Root>/Gain3'
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
  boolean_T MATLABSystem6_IndepInDB;   /* Expression: false
                                        * Referenced by: '<Root>/MATLAB System6'
                                        */
  uint8_T MATLABSystem4_ModeDefault;   /* Expression: uint8( 3 )
                                        * Referenced by: '<Root>/MATLAB System4'
                                        */
  uint8_T Constant4_Value;             /* Computed Parameter: Constant4_Value
                                        * Referenced by: '<Root>/Constant4'
                                        */
  uint8_T Constant17_Value;            /* Computed Parameter: Constant17_Value
                                        * Referenced by: '<Root>/Constant17'
                                        */
  uint8_T Constant1_Value;             /* Computed Parameter: Constant1_Value
                                        * Referenced by: '<Root>/Constant1'
                                        */
  uint8_T Constant13_Value;            /* Computed Parameter: Constant13_Value
                                        * Referenced by: '<Root>/Constant13'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_dist3_T {
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
      struct _ssPortInputs inputPortInfo[4];
      struct _ssInPortUnit inputPortUnits[4];
      struct _ssInPortCoSimAttribute inputPortCoSimAttribute[4];
      real_T const *UPtrs0[128];
      real_T const *UPtrs1[128];
      real_T const *UPtrs2[128];
      real_T const *UPtrs3[1];
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
extern P_dist3_T dist3_P;

/* Block signals (default storage) */
extern B_dist3_T dist3_B;

/* Block states (default storage) */
extern DW_dist3_T dist3_DW;

/* Model entry point functions */
extern void dist3_initialize(void);
extern void dist3_step(void);
extern void dist3_terminate(void);

/* Real-time Model object */
extern RT_MODEL_dist3_T *const dist3_M;

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
 * '<Root>' : 'dist3'
 */
#endif                                 /* dist3_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
