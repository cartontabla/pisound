/*
 * stream_out.c: S-Function for generic streamed audio output.
 *
 * The design goal is ALSA-like and transport-agnostic:
 * stream_out writes the current playback period that the backend will consume.
 * The backend may in turn be a virtual ALSA PCM, a RAVENNA/AES67 ALSA device,
 * or a direct test transport, but stream_out should only see a PCM period.
 *
 * In MATLAB simulation (MEX): samples are discarded.
 * Interface remains identical to pisound_out: 2 inputs, int32, frame-based.
 */

#define S_FUNCTION_NAME  stream_out
#define S_FUNCTION_LEVEL 2

#ifdef MATLAB_MEX_FILE
#include <simstruc.h>
#else
#include "simstruc.h"
#endif

#if defined(__linux__) && !defined(MATLAB_MEX_FILE)
#include "stream_audio.h"
#endif

/* Parameters */
#define SAMPLE_RATE_PARAM(S) (ssGetSFcnParam(S, 0))
#define BUFFER_SIZE_PARAM(S) (ssGetSFcnParam(S, 1))

/* Constants */
#define DEFAULT_SAMPLE_RATE  48000.0
#define DEFAULT_BUFFER_SIZE  128
#define SCALE_INT32_TO_FLOAT (1.0f / 2147483648.0f)  /* 1 / 2^31 */

static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 3);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) return;

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    if (!ssSetNumInputPorts(S, 2)) return;

    double buffer_size_param = mxGetScalar(BUFFER_SIZE_PARAM(S));
    int buffer_size = (int)buffer_size_param;
    if (buffer_size <= 0) buffer_size = DEFAULT_BUFFER_SIZE;

    /* Input 0: Audio Left */
    ssSetInputPortWidth(S, 0, buffer_size);
    ssSetInputPortDataType(S, 0, SS_INT32);
    ssSetInputPortDirectFeedThrough(S, 0, 1);

    /* Input 1: Audio Right */
    ssSetInputPortWidth(S, 1, buffer_size);
    ssSetInputPortDataType(S, 1, SS_INT32);
    ssSetInputPortDirectFeedThrough(S, 1, 1);

    if (!ssSetNumOutputPorts(S, 0)) return;

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
}

static void mdlInitializeSampleTimes(SimStruct *S)
{
    double rate        = mxGetScalar(SAMPLE_RATE_PARAM(S));
    double buffer_size = mxGetScalar(BUFFER_SIZE_PARAM(S));

    if (rate        <= 0.0) rate        = DEFAULT_SAMPLE_RATE;
    if (buffer_size <= 0.0) buffer_size = DEFAULT_BUFFER_SIZE;

    ssSetSampleTime(S, 0, buffer_size / rate);
    ssSetOffsetTime(S, 0, 0.0);
}

static void mdlStart(SimStruct *S) { (void)S; }

static void mdlOutputs(SimStruct *S, int_T tid)
{
    (void)tid;

    int sig_size        = ssGetInputPortWidth(S, 0);
    const int32_T *inL  = (const int32_T *)ssGetInputPortSignal(S, 0);
    const int32_T *inR  = (const int32_T *)ssGetInputPortSignal(S, 1);

#if defined(__linux__) && !defined(MATLAB_MEX_FILE)
    if (g_stream_playback_l && g_stream_playback_r) {
        for (int i = 0; i < sig_size; i++) {
            g_stream_playback_l[g_stream_offset + i] = (float)inL[i] * SCALE_INT32_TO_FLOAT;
            g_stream_playback_r[g_stream_offset + i] = (float)inR[i] * SCALE_INT32_TO_FLOAT;
        }
    }
#else
    /* Simulation: discard */
    (void)inL; (void)inR;
#endif
}

static void mdlTerminate(SimStruct *S) { (void)S; }

#ifndef MATLAB_MEX_FILE
#ifdef __cplusplus
extern "C" {
#endif

void stream_out(SimStruct *rts)
{
    rts->mdlInitializeSizes       = mdlInitializeSizes;
    rts->mdlInitializeSampleTimes = mdlInitializeSampleTimes;
    rts->mdlStart                 = mdlStart;
    rts->mdlOutputs               = mdlOutputs;
    rts->mdlTerminate             = mdlTerminate;

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
