#ifndef FAKE_SIMSTRUC_H
#define FAKE_SIMSTRUC_H

#include "rtwtypes.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- 1. Missing Types Definitions --- */

typedef enum {
  SS_SIMMODE_NORMAL,
  SS_SIMMODE_SIZES_CALL_ONLY,
  SS_SIMMODE_RTWGEN,
  SS_SIMMODE_EXTERNAL
} SS_SimMode;

typedef struct {
  int dummy;
} RTWSolverInfo;
typedef struct {
  int dummy;
} RTWSfcnInfo;
typedef struct {
  int dummy;
} mxArray;
typedef struct {
  int dummy;
} ssPeriodicStatesInfo;

struct _ssBlkInfo2 {
  int dummy;
};
struct _ssSFcnModelMethods2 {
  int dummy;
};
struct _ssSFcnModelMethods3 {
  int dummy;
};
struct _ssSFcnModelMethods4 {
  int dummy;
};
struct _ssStatesInfo2 {
  int dummy;
};
struct _ssPortInfo2 {
  int dummy;
};
struct _ssPortOutputs {
  int dummy;
};
struct _ssOutPortUnit {
  int dummy;
};
struct _ssOutPortCoSimAttribute {
  int dummy;
};
struct _ssDWorkRecord {
  int dummy;
};
struct _ssDWorkAuxRecord {
  int dummy;
};
struct _ssPortInputs {
  int dummy;
};
struct _ssInPortUnit {
  int dummy;
};
struct _ssInPortCoSimAttribute {
  int dummy;
};

/* --- 2. SimStruct Definition with IO and Function Pointers --- */

typedef void *InputPtrsType;
typedef real_T *InputRealPtrsType;

/* Forward declaration */
struct SimStruct_tag;

/* Function Pointer Types for S-Function Methods */
typedef void (*MdlInitializeSizesFcn)(struct SimStruct_tag *S);
typedef void (*MdlInitializeSampleTimesFcn)(struct SimStruct_tag *S);
typedef void (*MdlStartFcn)(struct SimStruct_tag *S);
typedef void (*MdlOutputsFcn)(struct SimStruct_tag *S, int tid);
typedef void (*MdlTerminateFcn)(struct SimStruct_tag *S);

typedef struct SimStruct_tag {
  /* IO Pointers */
  void *inputs[8];
  void *outputs[8];

  /* S-Function parameters (set by ssSetSFcnParam, read by ssGetSFcnParam) */
  void *sfcn_params[8];

  /* Work Vectors */
  void *PWork[16];
  int IWork[16];

  /* Methods Registration */
  MdlInitializeSizesFcn mdlInitializeSizes;
  MdlInitializeSampleTimesFcn mdlInitializeSampleTimes;
  MdlStartFcn mdlStart;
  MdlOutputsFcn mdlOutputs;
  MdlTerminateFcn mdlTerminate;

  /* Dummy fields */
  const char *path;
  const char *modelName;
  void *userData;
} SimStruct;

/* --- 3. Constants & Macros --- */

#define SS_INTEGER 2
#define SS_INT32 3
#define SS_DOUBLE 0
#define SS_POINTER 4
#define SS_UINT8 5
#define DYNAMICALLY_SIZED -1
#define COMPLEX_NO 0
#define COMPLEX_YES 1

/* Solver Macros */
#define rtsiSetSolverName(s, n)
#define rtsiSetFixedStepSize(s, f)
#define rtsiSetSolverMode(s, m)
#define SOLVER_MODE_SINGLETASKING 0

/* RTW / Internal Macros */
#define rtssSetErrorStatusPtr(info, ptr)
#define rtssSetNumRootSampTimesPtr(info, ptr)
#define rtssSetTPtrPtr(info, ptr)
#define rtssSetTStartPtr(info, ptr)
#define rtssSetTFinalPtr(info, ptr)
#define rtssSetTimeOfLastOutputPtr(info, ptr)
#define rtssSetStepSizePtr(info, ptr)
#define rtssSetStopRequestedPtr(info, ptr)
#define rtssSetDerivCacheNeedsResetPtr(info, ptr)
#define rtssSetZCCacheNeedsResetPtr(info, ptr)
#define rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(info, ptr)
#define rtssSetSampleHitsPtr(info, ptr)
#define rtssSetPerTaskSampleHitsPtr(info, ptr)
#define rtssSetSimModePtr(info, ptr)
#define rtssSetSolverInfoPtr(info, ptr)

/* S-Function Setup Macros */
#define ssSetSampleTimePtr(S, ptr)
#define ssSetOffsetTimePtr(S, ptr)
#define ssSetSampleTimeTaskIDPtr(S, ptr)
#define ssSetBlkInfo2Ptr(S, ptr)
#define _ssSetBlkInfo2PortInfo2Ptr(S, ptr)
#define ssSetRTWSfcnInfo(S, info)
#define ssSetModelMethods2(S, ptr)
#define ssSetModelMethods3(S, ptr)
#define ssSetModelMethods4(S, ptr)
#define ssSetStatesInfo2(S, ptr)
#define ssSetPeriodicStatesInfo(S, ptr)
#define ssSetPortInfoForOutputs(S, ptr)
#define _ssSetNumOutputPorts(S, n) ((void)(S), (void)(n))
#define _ssSetPortInfo2ForOutputUnits(S, ptr)
#define ssSetOutputPortUnit(S, p, u)
#define _ssSetPortInfo2ForOutputCoSimAttribute(S, ptr)
#define ssSetOutputPortIsContinuousQuantity(S, p, v)
#define _ssSetOutputPortNumDimensions(S, p, n)
#define ssSetOutputPortWidthAsInt(S, p, w)
#define ssSetModelName(S, name)
#define ssSetPath(S, path)
#define ssSetRTModel(S, m)
#define ssSetParentSS(S, p)
#define ssSetRootSS(S, r)
#define ssSetVersion(S, v)
#define SIMSTRUCT_VERSION_LEVEL2 2
#define ssSetSFcnParamsCount(S, n)
#define ssSetSFcnParamsPtr(S, ptr) ((void)(S), (void)(ptr))
#define ssSetSFcnParam(S, i, v)    ((S)->sfcn_params[i] = (void *)(v))
#define ssSetNumSFcnParams(S, n)

/* Work Vector Macros */
#define ssGetPWork(S) ((S)->PWork)
#define ssSetPWork(S, ptr) ((void)(S), (void)(ptr))
#define ssGetIWork(S) ((S)->IWork)
#define ssSetIWork(S, ptr) ((void)(S), (void)(ptr))
#define ssSetNumPWork(S, n) 1
#define ssSetNumIWork(S, n) 1

#define ssSetSFcnDWork(S, ptr) ((void)(S), (void)(ptr))
#define ssSetSFcnDWorkAux(S, ptr) ((void)(S), (void)(ptr))
#define ssSetNumDWorkAsInt(S, n) ((void)(S), (void)(n))
#define ssSetDWorkWidthAsInt(S, i, w)
#define ssSetDWorkDataType(S, i, t)
#define ssSetDWorkComplexSignal(S, i, c)
#define ssSetDWork(S, i, ptr)
#define ssSetNumNonsampledZCsAsInt(S, n)
#define _ssSetOutputPortConnected(S, p, v)
#define _ssSetOutputPortBeingMerged(S, p, v)

/* Input Macros */
#define _ssSetNumInputPorts(S, n) ((void)(S), (void)(n))
#define ssSetPortInfoForInputs(S, ptr)
#define _ssSetPortInfo2ForInputUnits(S, ptr)
#define ssSetInputPortUnit(S, p, u)
#define _ssSetPortInfo2ForInputCoSimAttribute(S, ptr)
#define ssSetInputPortIsContinuousQuantity(S, p, v)
#define _ssSetInputPortNumDimensions(S, p, n)
#define ssSetInputPortWidthAsInt(S, p, w)
#define _ssSetInputPortConnected(S, p, v)
#define ssSetInputPortBufferDstPort(S, p, dst)

/* Execution Macros - Routing to function pointers */
#define sfcnInitializeSizes(S)                                                 \
  if ((S)->mdlInitializeSizes)                                                 \
  (S)->mdlInitializeSizes(S)
#define sfcnInitializeSampleTimes(S)
#define sfcnStart(S)                                                           \
  if ((S)->mdlStart)                                                           \
  (S)->mdlStart(S)
#define sfcnOutputs(S, tid)                                                    \
  if ((S)->mdlOutputs)                                                         \
  (S)->mdlOutputs(S, tid)
#define sfcnTerminate(S)                                                       \
  if ((S)->mdlTerminate)                                                       \
  (S)->mdlTerminate(S)

/* Standard S-Function Macros */
#define ssGetNumSFcnParams(S) 0
#define ssGetSFcnParamsCount(S) 0
#define ssSetNumContStates(S, n) 1
#define ssSetNumDiscStates(S, n) 1
#define ssSetNumInputPorts(S, n) 1
#define ssSetNumOutputPorts(S, n)                                              \
  1 /* Port width - Force 128 for Pisound/MODEP compatibility */
#define ssGetInputPortWidth(S, port) 128
/* Ensure no conflict or implicit decl */
#ifndef ssGetOutputPortWidth
#define ssGetOutputPortWidth(S, port) 128
#endif
#define ssSetInputPortWidth(S, p, w) 1
#define ssSetInputPortDataType(S, p, t)
#define ssSetInputPortDirectFeedThrough(S, p, d)
#define ssSetOutputPortWidth(S, p, w) 1
#define ssSetOutputPortDataType(S, p, t)

#define ssSetNumSampleTimes(S, n) 1
#define ssSetNumRWork(S, n) 1
#define ssSetNumModes(S, n) 1
#define ssSetNumNonsampledZCs(S, n) 1
#define ssSetOptions(S, n)
#define ssSetSampleTime(S, i, t)
#define ssSetOffsetTime(S, i, t)

#define mxGetPr(p) ((double *)0)
#define ssGetSFcnParam(S, i) ((S)->sfcn_params[i])
/* ERT layout: param pointer → [size[0], size[1], value] as real_T[3].
 * The generated code sets params to &SFunction_PX_Size = {1.0,1.0},
 * immediately followed by SFunction_PX (the scalar value) in the struct. */
#define mxGetScalar(p) ((p) ? ((const real_T *)(p))[2] : 0.0)

#define ssGetErrorStatus(S) ((const char *)0)
#define ssSetErrorStatus(S, msg)

/* --- 4. Critical IO Wiring & Access Macros --- */

#define ssSetOutputPortSignal(S, p, ptr) ((S)->outputs[p] = (void *)(ptr))
#define ssSetInputPortSignalPtrs(S, p, ptr) ((S)->inputs[p] = (void *)(ptr))

#define ssGetOutputPortSignal(S, p) ((S)->outputs[p])
#define ssGetInputPortSignalPtrs(S, p) ((const void **)(S)->inputs[p])
#define ssGetInputPortRealSignalPtrs(S, p) ((const real_T **)(S)->inputs[p])

// Added strictly for pisound_out.c which seems to use this form
#define ssGetInputPortSignal(S, p)                                             \
  ((const void *)((const void **)(S)->inputs[p])[0])
// #define ssGetInputPortWidth(S, p) 128 (Already defined earlier)

#endif
