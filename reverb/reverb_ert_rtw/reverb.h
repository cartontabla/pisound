/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: reverb.h
 *
 * Code generated for Simulink model 'reverb'.
 *
 * Model version                  : 1.32
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Thu Mar  5 19:41:23 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef reverb_h_
#define reverb_h_
#ifndef reverb_COMMON_INCLUDES_
#define reverb_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rt_nonfinite.h"
#include "math.h"
#endif                                 /* reverb_COMMON_INCLUDES_ */

#include "reverb_types.h"
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

/* Block signals for system '<S1>/MATLAB System' */
typedef struct {
  real_T MATLABSystem_o1[128];         /* '<S1>/MATLAB System' */
  real_T MATLABSystem_o2[128];         /* '<S1>/MATLAB System' */
} B_MATLABSystem_reverb_T;

/* Block states (default storage) for system '<S1>/MATLAB System' */
typedef struct {
  vcd_sys_reverb_T obj;                /* '<S1>/MATLAB System' */
  real_T w;                            /* '<S1>/MATLAB System' */
  boolean_T objisempty;                /* '<S1>/MATLAB System' */
} DW_MATLABSystem_reverb_T;

/* Block signals for system '<S4>/MATLAB System' */
typedef struct {
  real_T MATLABSystem_o1[128];         /* '<S4>/MATLAB System' */
  real_T MATLABSystem_o2[128];         /* '<S4>/MATLAB System' */
} B_MATLABSystem_reverb_c_T;

/* Block states (default storage) for system '<S4>/MATLAB System' */
typedef struct {
  vcd_sys_reverb_T obj;                /* '<S4>/MATLAB System' */
  real_T w;                            /* '<S4>/MATLAB System' */
  boolean_T objisempty;                /* '<S4>/MATLAB System' */
} DW_MATLABSystem_reverb_f_T;

/* Block signals (default storage) */
typedef struct {
  int32_T PiSoundInput_o1[128];        /* '<Root>/PiSound Input' */
  int32_T PiSoundInput_o2[128];        /* '<Root>/PiSound Input' */
  int32_T DataTypeConversion1[128];    /* '<Root>/Data Type Conversion1' */
  int32_T DataTypeConversion3[128];    /* '<Root>/Data Type Conversion3' */
  B_MATLABSystem_reverb_T MATLABSystem_pnaevv;/* '<S1>/MATLAB System' */
  B_MATLABSystem_reverb_c_T MATLABSystem_pnaev;/* '<S4>/MATLAB System' */
  B_MATLABSystem_reverb_c_T MATLABSystem_pnae;/* '<S4>/MATLAB System' */
  B_MATLABSystem_reverb_T MATLABSystem_pna;/* '<S1>/MATLAB System' */
  B_MATLABSystem_reverb_T MATLABSystem_pn;/* '<S1>/MATLAB System' */
  B_MATLABSystem_reverb_T MATLABSystem_p;/* '<S1>/MATLAB System' */
} B_reverb_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  mix_sys_reverb_T obj;                /* '<Root>/MATLAB System' */
  DW_MATLABSystem_reverb_T MATLABSystem_pnaevv;/* '<S1>/MATLAB System' */
  DW_MATLABSystem_reverb_f_T MATLABSystem_pnaev;/* '<S4>/MATLAB System' */
  DW_MATLABSystem_reverb_f_T MATLABSystem_pnae;/* '<S4>/MATLAB System' */
  DW_MATLABSystem_reverb_T MATLABSystem_pna;/* '<S1>/MATLAB System' */
  DW_MATLABSystem_reverb_T MATLABSystem_pn;/* '<S1>/MATLAB System' */
  DW_MATLABSystem_reverb_T MATLABSystem_p;/* '<S1>/MATLAB System' */
} DW_reverb_T;

/* Parameters for system: '<S1>/MATLAB System' */
struct P_MATLABSystem_reverb_T_ {
  real_T MATLABSystem_Delay;           /* Expression: 0.1
                                        * Referenced by: '<S1>/MATLAB System'
                                        */
  real_T MATLABSystem_g;               /* Expression: 0.5
                                        * Referenced by: '<S1>/MATLAB System'
                                        */
  uint8_T MATLABSystem_Mode;           /* Expression: uint8(1)
                                        * Referenced by: '<S1>/MATLAB System'
                                        */
};

/* Parameters for system: '<S4>/MATLAB System' */
struct P_MATLABSystem_reverb_g_T_ {
  real_T MATLABSystem_Delay;           /* Expression: 0.1
                                        * Referenced by: '<S4>/MATLAB System'
                                        */
  real_T MATLABSystem_g;               /* Expression: 0.5
                                        * Referenced by: '<S4>/MATLAB System'
                                        */
  uint8_T MATLABSystem_Mode;           /* Expression: uint8(1)
                                        * Referenced by: '<S4>/MATLAB System'
                                        */
};

/* Parameters (default storage) */
struct P_reverb_T_ {
  real_T Mix_rt;                       /* Variable: Mix_rt
                                        * Referenced by: '<Root>/Constant1'
                                        */
  real_T RT60_rt;                      /* Variable: RT60_rt
                                        * Referenced by: '<Root>/Constant2'
                                        */
  real_T Time1_rt;                     /* Variable: Time1_rt
                                        * Referenced by: '<S1>/Constant1'
                                        */
  real_T Time2_rt;                     /* Variable: Time2_rt
                                        * Referenced by: '<S6>/Constant1'
                                        */
  real_T Time3_rt;                     /* Variable: Time3_rt
                                        * Referenced by: '<S2>/Constant1'
                                        */
  real_T Time4_rt;                     /* Variable: Time4_rt
                                        * Referenced by: '<S3>/Constant1'
                                        */
  real_T MATLABSystem_SmoothTime;      /* Expression: 0.01
                                        * Referenced by: '<Root>/MATLAB System'
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
  real_T Gain8_Gain;                   /* Expression: 0.25
                                        * Referenced by: '<S1>/Gain8'
                                        */
  real_T Gain8_Gain_m;                 /* Expression: 0.25
                                        * Referenced by: '<S6>/Gain8'
                                        */
  real_T Gain8_Gain_l;                 /* Expression: 0.25
                                        * Referenced by: '<S2>/Gain8'
                                        */
  real_T Gain8_Gain_o;                 /* Expression: 0.25
                                        * Referenced by: '<S3>/Gain8'
                                        */
  real_T Constant1_Value;              /* Expression: 2
                                        * Referenced by: '<S4>/Constant1'
                                        */
  real_T Constant2_Value;              /* Expression: 0.7
                                        * Referenced by: '<S4>/Constant2'
                                        */
  real_T Gain9_Gain;                   /* Expression: 0.25
                                        * Referenced by: '<S1>/Gain9'
                                        */
  real_T Gain9_Gain_p;                 /* Expression: 0.25
                                        * Referenced by: '<S6>/Gain9'
                                        */
  real_T Gain9_Gain_j;                 /* Expression: 0.25
                                        * Referenced by: '<S2>/Gain9'
                                        */
  real_T Gain9_Gain_a;                 /* Expression: 0.25
                                        * Referenced by: '<S3>/Gain9'
                                        */
  real_T Gain8_Gain_c;                 /* Expression: 0.5
                                        * Referenced by: '<S4>/Gain8'
                                        */
  real_T Constant1_Value_e;            /* Expression: 6
                                        * Referenced by: '<S5>/Constant1'
                                        */
  real_T Constant2_Value_e;            /* Expression: 0.007
                                        * Referenced by: '<S5>/Constant2'
                                        */
  real_T Gain9_Gain_c;                 /* Expression: 0.5
                                        * Referenced by: '<S4>/Gain9'
                                        */
  real_T Gain8_Gain_cn;                /* Expression: 0.5
                                        * Referenced by: '<S5>/Gain8'
                                        */
  real_T Gain9_Gain_js;                /* Expression: 0.5
                                        * Referenced by: '<S5>/Gain9'
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
  boolean_T MATLABSystem_IndepInDB;    /* Expression: false
                                        * Referenced by: '<Root>/MATLAB System'
                                        */
  uint8_T Constant_Value;              /* Computed Parameter: Constant_Value
                                        * Referenced by: '<S1>/Constant'
                                        */
  uint8_T Constant_Value_g;            /* Computed Parameter: Constant_Value_g
                                        * Referenced by: '<S6>/Constant'
                                        */
  uint8_T Constant_Value_a;            /* Computed Parameter: Constant_Value_a
                                        * Referenced by: '<S2>/Constant'
                                        */
  uint8_T Constant_Value_av;           /* Computed Parameter: Constant_Value_av
                                        * Referenced by: '<S3>/Constant'
                                        */
  uint8_T Constant_Value_b;            /* Computed Parameter: Constant_Value_b
                                        * Referenced by: '<S4>/Constant'
                                        */
  uint8_T Constant_Value_m;            /* Computed Parameter: Constant_Value_m
                                        * Referenced by: '<S5>/Constant'
                                        */
  P_MATLABSystem_reverb_T MATLABSystem_pnaevv;/* '<S1>/MATLAB System' */
  P_MATLABSystem_reverb_g_T MATLABSystem_pnaev;/* '<S4>/MATLAB System' */
  P_MATLABSystem_reverb_g_T MATLABSystem_pnae;/* '<S4>/MATLAB System' */
  P_MATLABSystem_reverb_T MATLABSystem_pna;/* '<S1>/MATLAB System' */
  P_MATLABSystem_reverb_T MATLABSystem_pn;/* '<S1>/MATLAB System' */
  P_MATLABSystem_reverb_T MATLABSystem_p;/* '<S1>/MATLAB System' */
};

/* Real-time Model Data Structure */
struct tag_RTM_reverb_T {
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
extern P_reverb_T reverb_P;

/* Block signals (default storage) */
extern B_reverb_T reverb_B;

/* Block states (default storage) */
extern DW_reverb_T reverb_DW;

/* Model entry point functions */
extern void reverb_initialize(void);
extern void reverb_step(void);
extern void reverb_terminate(void);

/* Real-time Model object */
extern RT_MODEL_reverb_T *const reverb_M;

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
 * '<Root>' : 'reverb'
 * '<S1>'   : 'reverb/Subsystem'
 * '<S2>'   : 'reverb/Subsystem1'
 * '<S3>'   : 'reverb/Subsystem2'
 * '<S4>'   : 'reverb/Subsystem4'
 * '<S5>'   : 'reverb/Subsystem5'
 * '<S6>'   : 'reverb/Subsystem6'
 * '<S7>'   : 'reverb/Subsystem/MATLAB Function'
 * '<S8>'   : 'reverb/Subsystem/MATLAB Function1'
 * '<S9>'   : 'reverb/Subsystem1/MATLAB Function'
 * '<S10>'  : 'reverb/Subsystem1/MATLAB Function1'
 * '<S11>'  : 'reverb/Subsystem2/MATLAB Function'
 * '<S12>'  : 'reverb/Subsystem2/MATLAB Function1'
 * '<S13>'  : 'reverb/Subsystem6/MATLAB Function'
 * '<S14>'  : 'reverb/Subsystem6/MATLAB Function1'
 */
#endif                                 /* reverb_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
