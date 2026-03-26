/*
 * ctrl_out.c - S-Function: monitoring/feedback outputs from the Simulink model
 *
 * Receives N scalar double inputs from the model and stores them in
 * g_ctrl_out_vals[] so control_process_tx() can read and forward them
 * via MIDI/OSC (e.g., parameter echo-back, metering, LED feedback).
 *
 * On Linux/embedded (Pi): writes inputs to g_ctrl_out_vals[] each frame.
 * In MATLAB simulation (MEX): no-op (inputs are discarded).
 *
 * S-Function dialog parameters:
 *   [0] sample_rate   (e.g. 48000)
 *   [1] buffer_size   (e.g. 128)   — used only for sample time calculation
 *   [2] num_mon       (N inputs, one per monitored signal)
 */

#define S_FUNCTION_NAME  ctrl_out
#define S_FUNCTION_LEVEL 2

#ifdef MATLAB_MEX_FILE
#include <simstruc.h>
#else
#include "simstruc.h"
#endif

/* CTRL_OUT_MAX_MON must be visible in all build modes */
#ifndef CTRL_OUT_MAX_MON
#define CTRL_OUT_MAX_MON 20
#endif

#if defined(__linux__) && !defined(MATLAB_MEX_FILE)
#include "ctrl_out.h"
#endif

#define SAMPLE_RATE_PARAM(S)  (ssGetSFcnParam(S, 0))
#define BUFFER_SIZE_PARAM(S)  (ssGetSFcnParam(S, 1))
#define NUM_MON_PARAM(S)      (ssGetSFcnParam(S, 2))

#define DEFAULT_SAMPLE_RATE  48000.0
#define DEFAULT_BUFFER_SIZE  128.0
#define DEFAULT_NUM_MON      4

/* Cached at mdlInitializeSizes — ssGetNumInputPorts not available in embedded simstruc */
static int s_num_mon = DEFAULT_NUM_MON;

/* ------------------------------------------------------------------ */
/* S-Function callbacks                                                 */
/* ------------------------------------------------------------------ */

static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 3);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) return;

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    int n = (int)mxGetScalar(NUM_MON_PARAM(S));
    if (n <= 0)               n = 1;
    if (n > CTRL_OUT_MAX_MON) n = CTRL_OUT_MAX_MON;
    s_num_mon = n;

    if (!ssSetNumInputPorts(S, n)) return;

    for (int i = 0; i < n; i++) {
        ssSetInputPortWidth(S, i, 1);
        ssSetInputPortDataType(S, i, SS_DOUBLE);
        ssSetInputPortDirectFeedThrough(S, i, 1);
    }

    if (!ssSetNumOutputPorts(S, 0)) return;

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
}

static void mdlInitializeSampleTimes(SimStruct *S)
{
    double rate = mxGetScalar(SAMPLE_RATE_PARAM(S));
    double buf  = mxGetScalar(BUFFER_SIZE_PARAM(S));
    if (rate <= 0.0) rate = DEFAULT_SAMPLE_RATE;
    if (buf  <= 0.0) buf  = DEFAULT_BUFFER_SIZE;
    ssSetSampleTime(S, 0, buf / rate);
    ssSetOffsetTime(S, 0, 0.0);
}

static void mdlStart(SimStruct *S) { (void)S; }

static void mdlOutputs(SimStruct *S, int_T tid)
{
    (void)tid;
    (void)S;

#if defined(__linux__) && !defined(MATLAB_MEX_FILE)
    int n = s_num_mon;
    for (int i = 0; i < n; i++) {
        const real_T *u = (const real_T *)ssGetInputPortSignal(S, i);
        g_ctrl_out_vals[i] = (float)(*u);
    }
#else
    (void)S;
#endif
}

static void mdlTerminate(SimStruct *S) { (void)S; }

/* ------------------------------------------------------------------ */
/* Globals + API  (embedded binary only, not compiled into MEX)        */
/* ------------------------------------------------------------------ */

#if defined(__linux__) && !defined(MATLAB_MEX_FILE)

volatile float g_ctrl_out_vals[CTRL_OUT_MAX_MON] = {0};

float ctrl_out_get(unsigned int idx)
{
    if (idx < CTRL_OUT_MAX_MON)
        return g_ctrl_out_vals[idx];
    return 0.0f;
}

#endif /* __linux__ && !MATLAB_MEX_FILE */

/* ------------------------------------------------------------------ */
/* S-Function registration (RTW embedded) / MEX entry point            */
/* ------------------------------------------------------------------ */

#ifndef MATLAB_MEX_FILE
#ifdef __cplusplus
extern "C" {
#endif

void ctrl_out(SimStruct *rts)
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
