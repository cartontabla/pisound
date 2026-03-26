/*
 * pisound_in.c: S-Function for synchronous buffered audio input
 * Part of the "Modep-style" unified architecture
 *
 * CLEAN VERSION: AUDIO ONLY (no MIDI ports)
 */

#define S_FUNCTION_NAME pisound_in
#define S_FUNCTION_LEVEL 2

#ifdef MATLAB_MEX_FILE
#include <simstruc.h>
#else
#include "simstruc.h"
#endif

#include <string.h>

#if defined(__linux__) && !defined(MATLAB_MEX_FILE)
#include "unified_audio.h" /* Global pointers g_jack_in_l, g_jack_in_r, g_frame_offset */
#endif

/* Parameters */
#define SAMPLE_RATE_PARAM(S) (ssGetSFcnParam(S, 0))
#define BUFFER_SIZE_PARAM(S) (ssGetSFcnParam(S, 1))

/* Constants */
#define DEFAULT_SAMPLE_RATE 48000.0
#define DEFAULT_BUFFER_SIZE 128

/* 24-bit audio scaling factors */
#define SCALE_INT32_TO_24BIT 2147483647.0f /* 2^31 - 1 */

static void mdlInitializeSizes(SimStruct *S) {
  ssSetNumSFcnParams(S, 3); /* Rate, Buffer, Backend (kept for compatibility) */
  if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S))
    return;

  ssSetNumContStates(S, 0);
  ssSetNumDiscStates(S, 0);

  if (!ssSetNumInputPorts(S, 0))
    return;

  /* AUDIO ONLY: 2 outputs (L/R) */
  if (!ssSetNumOutputPorts(S, 2))
    return;

  /* Get Buffer Size parameter */
  double buffer_size_param = mxGetScalar(BUFFER_SIZE_PARAM(S));
  int buffer_size = (int)buffer_size_param;
  if (buffer_size <= 0)
    buffer_size = DEFAULT_BUFFER_SIZE;

  /* Output 0: Audio Left */
  ssSetOutputPortWidth(S, 0, buffer_size);
  ssSetOutputPortDataType(S, 0, SS_INT32);

  /* Output 1: Audio Right */
  ssSetOutputPortWidth(S, 1, buffer_size);
  ssSetOutputPortDataType(S, 1, SS_INT32);

  ssSetNumSampleTimes(S, 1);
  ssSetNumRWork(S, 0);
  ssSetNumIWork(S, 0);
  ssSetNumPWork(S, 0);
  ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE | SS_OPTION_PLACE_ASAP);
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
}

static void mdlOutputs(SimStruct *S, int_T tid) {
  (void)tid;

  int buffer_size = ssGetOutputPortWidth(S, 0);

  int32_T *outL = (int32_T *)ssGetOutputPortSignal(S, 0);
  int32_T *outR = (int32_T *)ssGetOutputPortSignal(S, 1);

#if defined(__linux__) && !defined(MATLAB_MEX_FILE)
  /* Synchronous read from global pointers set by custom_main.c */
  if (g_jack_in_l && g_jack_in_r) {
    for (int i = 0; i < buffer_size; i++) {
      float val_l = g_jack_in_l[g_frame_offset + i];
      float val_r = g_jack_in_r[g_frame_offset + i];

      if (val_l > 1.0f)  val_l = 1.0f;
      if (val_l < -1.0f) val_l = -1.0f;
      if (val_r > 1.0f)  val_r = 1.0f;
      if (val_r < -1.0f) val_r = -1.0f;

      outL[i] = (int32_T)(val_l * SCALE_INT32_TO_24BIT);
      outR[i] = (int32_T)(val_r * SCALE_INT32_TO_24BIT);
    }
  } else {
    /* Silence */
    memset(outL, 0, buffer_size * sizeof(int32_T));
    memset(outR, 0, buffer_size * sizeof(int32_T));
  }
#else
  /* Simulation fallback (Output Silence/Zeros) */
  memset(outL, 0, buffer_size * sizeof(int32_T));
  memset(outR, 0, buffer_size * sizeof(int32_T));
#endif
}

static void mdlTerminate(SimStruct *S) { (void)S; }

#ifndef MATLAB_MEX_FILE
#ifdef __cplusplus
extern "C" {
#endif

void pisound_in(SimStruct *rts) {
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