/*
 * stream_in.c: S-Function for generic streamed audio input.
 *
 * The design goal is ALSA-like and transport-agnostic:
 * stream_in reads the current capture period prepared by the backend.
 * That backend may be backed by a virtual ALSA PCM, a RAVENNA/AES67 ALSA
 * device, or a direct test transport, but stream_in should not care.
 *
 * In MATLAB simulation (MEX): outputs silence.
 * Interface remains identical to pisound_in: 2 outputs, int32, frame-based.
 */

#define S_FUNCTION_NAME  stream_in
#define S_FUNCTION_LEVEL 2

#ifdef MATLAB_MEX_FILE
#include <simstruc.h>
#else
#include "simstruc.h"
#endif

#include <string.h>

#if defined(__linux__) && !defined(MATLAB_MEX_FILE)
#include "stream_audio.h"
#endif

/* Parameters */
#define SAMPLE_RATE_PARAM(S) (ssGetSFcnParam(S, 0))
#define BUFFER_SIZE_PARAM(S) (ssGetSFcnParam(S, 1))

/* Constants */
#define DEFAULT_SAMPLE_RATE  48000.0
#define DEFAULT_BUFFER_SIZE  128
#define SCALE_FLOAT_TO_INT32 2147483647.0f  /* 2^31 - 1 */

static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 3);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) return;

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    if (!ssSetNumInputPorts(S, 0)) return;
    if (!ssSetNumOutputPorts(S, 2)) return;

    double buffer_size_param = mxGetScalar(BUFFER_SIZE_PARAM(S));
    int buffer_size = (int)buffer_size_param;
    if (buffer_size <= 0) buffer_size = DEFAULT_BUFFER_SIZE;

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

    int buffer_size = ssGetOutputPortWidth(S, 0);
    int32_T *outL   = (int32_T *)ssGetOutputPortSignal(S, 0);
    int32_T *outR   = (int32_T *)ssGetOutputPortSignal(S, 1);

#if defined(__linux__) && !defined(MATLAB_MEX_FILE)
    if (g_stream_capture_l && g_stream_capture_r) {
        for (int i = 0; i < buffer_size; i++) {
            float vl = g_stream_capture_l[g_stream_offset + i];
            float vr = g_stream_capture_r[g_stream_offset + i];

            if (vl >  1.0f) vl =  1.0f;
            if (vl < -1.0f) vl = -1.0f;
            if (vr >  1.0f) vr =  1.0f;
            if (vr < -1.0f) vr = -1.0f;

            outL[i] = (int32_T)(vl * SCALE_FLOAT_TO_INT32);
            outR[i] = (int32_T)(vr * SCALE_FLOAT_TO_INT32);
        }
    } else {
        memset(outL, 0, buffer_size * sizeof(int32_T));
        memset(outR, 0, buffer_size * sizeof(int32_T));
    }
#else
    /* Simulation: silence */
    memset(outL, 0, buffer_size * sizeof(int32_T));
    memset(outR, 0, buffer_size * sizeof(int32_T));
#endif
}

static void mdlTerminate(SimStruct *S) { (void)S; }

#ifndef MATLAB_MEX_FILE
#ifdef __cplusplus
extern "C" {
#endif

void stream_in(SimStruct *rts)
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
