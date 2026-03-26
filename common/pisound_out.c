/*
 * pisound_out.c: S-Function for synchronous buffered audio output
 * Part of the "Modep-style" unified architecture
 *
 * CLEAN VERSION: AUDIO ONLY (no MIDI ports)
 */

#define S_FUNCTION_NAME pisound_out
#define S_FUNCTION_LEVEL 2

#ifdef MATLAB_MEX_FILE
#include <simstruc.h>
#else
#include "simstruc.h"
#endif

#if defined(__linux__) && !defined(MATLAB_MEX_FILE)
#include "unified_audio.h" /* Global pointers g_jack_out_l, etc. */
#endif

/* Parameters */
#define SAMPLE_RATE_PARAM(S) (ssGetSFcnParam(S, 0))
#define BUFFER_SIZE_PARAM(S) (ssGetSFcnParam(S, 1))

/* Constants */
#define DEFAULT_SAMPLE_RATE 48000.0
#define DEFAULT_BUFFER_SIZE 128
#define SCALE_INT32_TO_FLOAT (1.0f / 2147483648.0f) /* 2^31 */

static void mdlInitializeSizes(SimStruct *S) {
  ssSetNumSFcnParams(S, 3); /* Rate, Buffer, Backend (kept for compatibility) */
  if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S))
    return;

  ssSetNumContStates(S, 0);
  ssSetNumDiscStates(S, 0);

  /* AUDIO ONLY: 2 inputs (L/R) */
  if (!ssSetNumInputPorts(S, 2))
    return;

  /* Get Buffer Size from parameter */
  double buffer_size_param = mxGetScalar(BUFFER_SIZE_PARAM(S));
  int buffer_size = (int)buffer_size_param;
  if (buffer_size <= 0)
    buffer_size = DEFAULT_BUFFER_SIZE;

  /* Input 0: Audio Left */
  ssSetInputPortWidth(S, 0, buffer_size);
  ssSetInputPortDataType(S, 0, SS_INT32);
  ssSetInputPortDirectFeedThrough(S, 0, 1);

  /* Input 1: Audio Right */
  ssSetInputPortWidth(S, 1, buffer_size);
  ssSetInputPortDataType(S, 1, SS_INT32);
  ssSetInputPortDirectFeedThrough(S, 1, 1);

  if (!ssSetNumOutputPorts(S, 0))
    return;

  ssSetNumSampleTimes(S, 1);
  ssSetNumRWork(S, 0);
  ssSetNumIWork(S, 0);
  ssSetNumPWork(S, 0);
  ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
}

static void mdlInitializeSampleTimes(SimStruct *S) {
  double rate = mxGetScalar(SAMPLE_RATE_PARAM(S));
  double buffer_size = mxGetScalar(BUFFER_SIZE_PARAM(S));

  if (rate <= 0.0)
    rate = DEFAULT_SAMPLE_RATE;
  if (buffer_size <= 0.0)
    buffer_size = DEFAULT_BUFFER_SIZE;

  ssSetSampleTime(S, 0, buffer_size / rate);
  ssSetOffsetTime(S, 0, 0.0);
}

static void mdlStart(SimStruct *S) {
  /* No initialization needed - handled by custom_main.c */
  (void)S;
}

static void mdlOutputs(SimStruct *S, int_T tid) {
  (void)tid;

  int sig_size = ssGetInputPortWidth(S, 0);

  const int32_T *u0 = (const int32_T *)ssGetInputPortSignal(S, 0);
  const int32_T *u1 = (const int32_T *)ssGetInputPortSignal(S, 1);

#if defined(__linux__) && !defined(MATLAB_MEX_FILE)
  if (g_jack_out_l && g_jack_out_r) {
    for (int i = 0; i < sig_size; i++) {
      g_jack_out_l[g_frame_offset + i] = (float)u0[i] * SCALE_INT32_TO_FLOAT;
      g_jack_out_r[g_frame_offset + i] = (float)u1[i] * SCALE_INT32_TO_FLOAT;
    }
  }
#else
  /* MEX/sim: no hardware output */
  (void)u0; (void)u1;
#endif
}

static void mdlTerminate(SimStruct *S) { (void)S; }

#ifndef MATLAB_MEX_FILE
#ifdef __cplusplus
extern "C" {
#endif

void pisound_out(SimStruct *rts) {
  rts->mdlInitializeSizes = mdlInitializeSizes;
  rts->mdlInitializeSampleTimes = mdlInitializeSampleTimes;
  rts->mdlStart = mdlStart;
  rts->mdlOutputs = mdlOutputs;
  rts->mdlTerminate = mdlTerminate;

  mdlInitializeSizes(rts);
  mdlInitializeSampleTimes(rts);
}

#ifdef __cplusplus
}
#endif

#include "cg_sfun.h"
#else
#include "simulink.c"
#endif