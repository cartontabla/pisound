/*
 * pisound_in.c: S-Function for synchronous buffered audio input
 * Part of the "Modep-style" unified architecture
 */

#define S_FUNCTION_NAME pisound_in
#define S_FUNCTION_LEVEL 2

#ifdef MATLAB_MEX_FILE
/* Compilando MEX en Mac - usar headers reales de MATLAB */
#include <simstruc.h>
#else
/* Compilando para Pi - usar stubs locales */
#include "simstruc.h"
#endif

#include <string.h>

#if defined(__linux__) && !defined(MATLAB_MEX_FILE)
#include "unified_audio.h" /* Global pointers g_jack_in_l, etc. */
#endif

/* Parameters */
#define SAMPLE_RATE_PARAM(S) (ssGetSFcnParam(S, 0))
#define BUFFER_SIZE_PARAM(S) (ssGetSFcnParam(S, 1))

/* Constants */
#define DEFAULT_SAMPLE_RATE 48000.0
#define DEFAULT_BUFFER_SIZE 128
#define MIDI_BUF_SIZE 128

/* 24-bit audio scaling factors */
#define SCALE_INT32_TO_24BIT 2147483647.0f /* 2^31 - 1 */

static void mdlInitializeSizes(SimStruct *S) {
  ssSetNumSFcnParams(S, 3); /* Rate, Buffer, Backend */
  if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S))
    return;

  ssSetNumContStates(S, 0);
  ssSetNumDiscStates(S, 0);

  if (!ssSetNumInputPorts(S, 0))
    return;
  if (!ssSetNumOutputPorts(S, 4))
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

  /* Output 2: MIDI Bytes */
  ssSetOutputPortWidth(S, 2, MIDI_BUF_SIZE);
  ssSetOutputPortDataType(S, 2, SS_UINT8);

  /* Output 3: MIDI Count */
  ssSetOutputPortWidth(S, 3, 1);
  ssSetOutputPortDataType(S, 3, SS_UINT32);

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
  int buffer_size = ssGetOutputPortWidth(S, 0);

  int32_T *outL = (int32_T *)ssGetOutputPortSignal(S, 0);
  int32_T *outR = (int32_T *)ssGetOutputPortSignal(S, 1);
  uint8_T *midiMsg = (uint8_T *)ssGetOutputPortSignal(S, 2);
  uint32_T *midiCount = (uint32_T *)ssGetOutputPortSignal(S, 3);

#if defined(__linux__) && !defined(MATLAB_MEX_FILE)
  /* Synchronous read from global pointers set by custom_main.c */

  if (g_jack_in_l && g_jack_in_r) {
    for (int i = 0; i < buffer_size; i++) {
      /* Read from JACK buffer with offset, write to Simulink buffer without */
      float val_l = g_jack_in_l[g_frame_offset + i];
      float val_r = g_jack_in_r[g_frame_offset + i];

      if (val_l > 1.0f)
        val_l = 1.0f;
      if (val_l < -1.0f)
        val_l = -1.0f;
      if (val_r > 1.0f)
        val_r = 1.0f;
      if (val_r < -1.0f)
        val_r = -1.0f;

      outL[i] = (int32_T)(val_l * SCALE_INT32_TO_24BIT);
      outR[i] = (int32_T)(val_r * SCALE_INT32_TO_24BIT);
    }
  } else {
    /* Silence */
    memset(outL, 0, buffer_size * sizeof(int32_T));
    memset(outR, 0, buffer_size * sizeof(int32_T));
  }

  /* MIDI TODO: Implement global MIDI ringbuf if needed */
  *midiCount = 0;

#else
  /* Simulation fallback (Output Silence/Zeros) */
  memset(outL, 0, buffer_size * sizeof(int32_T));
  memset(outR, 0, buffer_size * sizeof(int32_T));
  *midiCount = 0;
#endif
}

static void mdlTerminate(SimStruct *S) { /* No cleanup needed */ }

#ifndef MATLAB_MEX_FILE
/* S-Function Glue Logic for Linked RTW/ERT Code (not needed for MEX) */
#ifdef __cplusplus
extern "C" {
#endif

void pisound_in(SimStruct *rts) {
  /* Register the S-function methods manually */
  rts->mdlInitializeSizes = mdlInitializeSizes;
  rts->mdlInitializeSampleTimes = mdlInitializeSampleTimes;
  rts->mdlStart = mdlStart;
  rts->mdlOutputs = mdlOutputs;
  rts->mdlTerminate = mdlTerminate;

  /* Call the initialization immediately as expected by registration */
  mdlInitializeSizes(rts);
  mdlInitializeSampleTimes(rts);
}

#ifdef __cplusplus
}
#endif

#include "cg_sfun.h"
#else
/* MEX compilation - use standard Simulink entry points */
#include "simulink.c"
#endif
