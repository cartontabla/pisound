/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: amp2.h
 *
 * Code generated for Simulink model 'amp2'.
 *
 * Model version                  : 1.15
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Fri May  1 09:27:40 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-A
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef amp2_h_
#define amp2_h_
#ifndef amp2_COMMON_INCLUDES_
#define amp2_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rt_nonfinite.h"
#include "math.h"
#endif                                 /* amp2_COMMON_INCLUDES_ */

#include "amp2_types.h"
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

/* Block signals for system '<S1>/CabHPF' */
typedef struct {
  real_T CabHPF_o1[128];               /* '<S1>/CabHPF' */
  real_T CabHPF_o2[128];               /* '<S1>/CabHPF' */
} B_CabHPF_amp2_T;

/* Block states (default storage) for system '<S1>/CabHPF' */
typedef struct {
  vcf_sys_amp2_T obj;                  /* '<S1>/CabHPF' */
  real_T zR[2];                        /* '<S1>/CabHPF' */
  real_T zL[2];                        /* '<S1>/CabHPF' */
  boolean_T objisempty;                /* '<S1>/CabHPF' */
} DW_CabHPF_amp2_T;

/* Block signals for system '<S3>/Stage2' */
typedef struct {
  real_T Stage2_o1[128];               /* '<S3>/Stage2' */
  real_T Stage2_o2[128];               /* '<S3>/Stage2' */
} B_Stage2_amp2_T;

/* Block states (default storage) for system '<S3>/Stage2' */
typedef struct {
  lut_ws_sys_amp2_T obj;               /* '<S3>/Stage2' */
  real_T zUp1L[14];                    /* '<S3>/Stage2' */
  real_T zDn2L[14];                    /* '<S3>/Stage2' */
  real_T zUp2R[14];                    /* '<S3>/Stage2' */
  real_T zUp1R[14];                    /* '<S3>/Stage2' */
  real_T zDn2R[14];                    /* '<S3>/Stage2' */
  real_T zDn1L[14];                    /* '<S3>/Stage2' */
  real_T zUp2L[14];                    /* '<S3>/Stage2' */
  real_T zDn1R[14];                    /* '<S3>/Stage2' */
  boolean_T objisempty;                /* '<S3>/Stage2' */
} DW_Stage2_amp2_T;

/* Block signals (default storage) */
typedef struct {
  real_T ControlInput_o1;              /* '<Root>/Control Input' */
  real_T ControlInput_o2;              /* '<Root>/Control Input' */
  real_T ControlInput_o3;              /* '<Root>/Control Input' */
  real_T ControlInput_o4;              /* '<Root>/Control Input' */
  real_T ControlInput_o5;              /* '<Root>/Control Input' */
  real_T ControlInput_o6;              /* '<Root>/Control Input' */
  real_T ControlInput_o7;              /* '<Root>/Control Input' */
  real_T ControlInput_o8;              /* '<Root>/Control Input' */
  int32_T StreamInput_o1[128];         /* '<Root>/Stream Input' */
  int32_T StreamInput_o2[128];         /* '<Root>/Stream Input' */
  int32_T DTC_L[128];                  /* '<Root>/DTC_L' */
  int32_T DTC_R[128];                  /* '<Root>/DTC_R' */
  B_CabHPF_amp2_T ToneTreble;          /* '<S1>/CabHPF' */
  B_CabHPF_amp2_T ToneMid;             /* '<S1>/CabHPF' */
  B_CabHPF_amp2_T ToneBass;            /* '<S1>/CabHPF' */
  B_Stage2_amp2_T Stage1;              /* '<S3>/Stage2' */
  B_CabHPF_amp2_T InputHPF;            /* '<S1>/CabHPF' */
  B_Stage2_amp2_T Stage2;              /* '<S3>/Stage2' */
  B_CabHPF_amp2_T IstagMid;            /* '<S1>/CabHPF' */
  B_CabHPF_amp2_T IstagHPF;            /* '<S1>/CabHPF' */
  B_CabHPF_amp2_T CabMid;              /* '<S1>/CabHPF' */
  B_CabHPF_amp2_T CabLPF;              /* '<S1>/CabHPF' */
  B_CabHPF_amp2_T CabHPF;              /* '<S1>/CabHPF' */
} B_amp2_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  lut_gc_sys_amp2_T obj;               /* '<S5>/SagGC' */
  mix_sys_amp2_T obj_l;                /* '<Root>/BypassMix' */
  env_sys_amp2_T obj_c;                /* '<S5>/SagENV' */
  arf_sys_amp2_T obj_m;                /* '<S5>/SagARF' */
  DW_CabHPF_amp2_T ToneTreble;         /* '<S1>/CabHPF' */
  DW_CabHPF_amp2_T ToneMid;            /* '<S1>/CabHPF' */
  DW_CabHPF_amp2_T ToneBass;           /* '<S1>/CabHPF' */
  DW_Stage2_amp2_T Stage1;             /* '<S3>/Stage2' */
  DW_CabHPF_amp2_T InputHPF;           /* '<S1>/CabHPF' */
  DW_Stage2_amp2_T Stage2;             /* '<S3>/Stage2' */
  DW_CabHPF_amp2_T IstagMid;           /* '<S1>/CabHPF' */
  DW_CabHPF_amp2_T IstagHPF;           /* '<S1>/CabHPF' */
  DW_CabHPF_amp2_T CabMid;             /* '<S1>/CabHPF' */
  DW_CabHPF_amp2_T CabLPF;             /* '<S1>/CabHPF' */
  DW_CabHPF_amp2_T CabHPF;             /* '<S1>/CabHPF' */
} DW_amp2_T;

/* Constant parameters (default storage) */
typedef struct {
  /* Pooled Parameter (Expression: linspace( -1, 1, 257 ))
   * Referenced by:
   *   '<S3>/Stage2'
   *   '<S4>/Stage1'
   */
  real_T pooled5[257];

  /* Expression: linspace( -80, 10, 257 )
   * Referenced by: '<S5>/SagGC'
   */
  real_T SagGC_XUser[257];

  /* Expression: zeros( 1, 257 )
   * Referenced by: '<S5>/SagGC'
   */
  real_T SagGC_YUser[257];

  /* Pooled Parameter (Expression: )
   * Referenced by:
   *   '<Root>/Control Input'
   *   '<Root>/Stream Input'
   *   '<Root>/Stream Ouputt'
   */
  real_T ControlInput_P1_Size[2];

  /* Pooled Parameter (Expression: 48000)
   * Referenced by:
   *   '<Root>/Control Input'
   *   '<Root>/Stream Input'
   *   '<Root>/Stream Ouputt'
   */
  real_T pooled17;

  /* Pooled Parameter (Expression: )
   * Referenced by:
   *   '<Root>/Control Input'
   *   '<Root>/Stream Input'
   *   '<Root>/Stream Ouputt'
   */
  real_T ControlInput_P2_Size[2];

  /* Pooled Parameter (Expression: 128)
   * Referenced by:
   *   '<Root>/Control Input'
   *   '<Root>/Stream Input'
   *   '<Root>/Stream Ouputt'
   */
  real_T pooled18;

  /* Computed Parameter: ControlInput_P3_Size
   * Referenced by: '<Root>/Control Input'
   */
  real_T ControlInput_P3_Size[2];

  /* Expression: 8
   * Referenced by: '<Root>/Control Input'
   */
  real_T ControlInput_P3;

  /* Pooled Parameter (Expression: )
   * Referenced by:
   *   '<Root>/Stream Input'
   *   '<Root>/Stream Ouputt'
   */
  real_T StreamInput_P3_Size[2];

  /* Pooled Parameter (Expression: 0)
   * Referenced by:
   *   '<Root>/Stream Input'
   *   '<Root>/Stream Ouputt'
   */
  real_T pooled19;
} ConstP_amp2_T;

/* Real-time Model Data Structure */
struct tag_RTM_amp2_T {
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
    SimStruct childSFunctions[3];
    SimStruct *childSFunctionPtrs[3];
    struct _ssBlkInfo2 blkInfo2[3];
    struct _ssSFcnModelMethods2 methods2[3];
    struct _ssSFcnModelMethods3 methods3[3];
    struct _ssSFcnModelMethods4 methods4[3];
    struct _ssStatesInfo2 statesInfo2[3];
    ssPeriodicStatesInfo periodicStatesInfo[3];
    struct _ssPortInfo2 inputOutputPortInfo2[3];
    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      struct _ssPortOutputs outputPortInfo[8];
      struct _ssOutPortUnit outputPortUnits[8];
      struct _ssOutPortCoSimAttribute outputPortCoSimAttribute[8];
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
      struct _ssPortInputs inputPortInfo[2];
      struct _ssInPortUnit inputPortUnits[2];
      struct _ssInPortCoSimAttribute inputPortCoSimAttribute[2];
      real_T const *UPtrs0[128];
      real_T const *UPtrs1[128];
      uint_T attribs[3];
      mxArray *params[3];
    } Sfcn2;
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
extern B_amp2_T amp2_B;

/* Block states (default storage) */
extern DW_amp2_T amp2_DW;

/* Constant parameters (default storage) */
extern const ConstP_amp2_T amp2_ConstP;

/* Model entry point functions */
extern void amp2_initialize(void);
extern void amp2_step(void);
extern void amp2_terminate(void);

/* Real-time Model object */
extern RT_MODEL_amp2_T *const amp2_M;

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
 * '<Root>' : 'amp2'
 * '<S1>'   : 'amp2/CabVoicing'
 * '<S2>'   : 'amp2/Interstage'
 * '<S3>'   : 'amp2/PowerStage'
 * '<S4>'   : 'amp2/Preamp'
 * '<S5>'   : 'amp2/Sag'
 * '<S6>'   : 'amp2/ToneStack'
 */
#endif                                 /* amp2_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
