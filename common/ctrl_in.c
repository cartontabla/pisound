/*
 * ctrl_in.c - S-Function: control parameter inputs to the Simulink model
 *
 * Provides N scalar double outputs to the model, one per controlled parameter.
 * Values are smoothed by ctrl_apply (ramping) before arriving here.
 *
 * On Linux/embedded (Pi): reads from g_ctrl_in_vals[], populated each frame
 *   by control_process_rx() via ctrl_in_set().
 * In MATLAB simulation (MEX): outputs zeros (model runs open-loop).
 *
 * S-Function dialog parameters:
 *   [0] sample_rate   (e.g. 48000)
 *   [1] buffer_size   (e.g. 128)   — used only for sample time calculation
 *   [2] num_params    (N outputs, must match ctrl_config_*.txt line count)
 */

#define S_FUNCTION_NAME  ctrl_in
#define S_FUNCTION_LEVEL 2

#ifdef MATLAB_MEX_FILE
#include <simstruc.h>
#else
#include "simstruc.h"
#endif

/* CTRL_IN_MAX_PARAMS must be visible in all build modes */
#ifndef CTRL_IN_MAX_PARAMS
#define CTRL_IN_MAX_PARAMS 20
#endif

#if defined(__linux__) && !defined(MATLAB_MEX_FILE)
#include "ctrl_in.h"
#endif

#define SAMPLE_RATE_PARAM(S)  (ssGetSFcnParam(S, 0))
#define BUFFER_SIZE_PARAM(S)  (ssGetSFcnParam(S, 1))
#define NUM_PARAMS_PARAM(S)   (ssGetSFcnParam(S, 2))

#define DEFAULT_SAMPLE_RATE  48000.0
#define DEFAULT_BUFFER_SIZE  128.0
#define DEFAULT_NUM_PARAMS   4

/* Cached at mdlInitializeSizes — ssGetNumOutputPorts not available in embedded simstruc */
static int s_num_params = DEFAULT_NUM_PARAMS;

/* ------------------------------------------------------------------ */
/* S-Function callbacks                                                 */
/* ------------------------------------------------------------------ */

static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 3);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) return;

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    if (!ssSetNumInputPorts(S, 0)) return;

    int n = (int)mxGetScalar(NUM_PARAMS_PARAM(S));
    if (n <= 0)                n = 1;
    if (n > CTRL_IN_MAX_PARAMS) n = CTRL_IN_MAX_PARAMS;
    s_num_params = n;

    if (!ssSetNumOutputPorts(S, n)) return;

    for (int i = 0; i < n; i++) {
        ssSetOutputPortWidth(S, i, 1);
        ssSetOutputPortDataType(S, i, SS_DOUBLE);
    }

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE | SS_OPTION_PLACE_ASAP);
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
    int n = s_num_params;

#if defined(__linux__) && !defined(MATLAB_MEX_FILE)
    for (int i = 0; i < n; i++) {
        real_T *y = (real_T *)ssGetOutputPortSignal(S, i);
        *y = (real_T)g_ctrl_in_vals[i];
    }
#else
    /* Simulation: open-loop, model uses default parameter values */
    for (int i = 0; i < n; i++) {
        real_T *y = (real_T *)ssGetOutputPortSignal(S, i);
        *y = 0.0;
    }
#endif
}

static void mdlTerminate(SimStruct *S) { (void)S; }

/* ------------------------------------------------------------------ */
/* Globals + API  (embedded binary only, not compiled into MEX)        */
/* ------------------------------------------------------------------ */

#if defined(__linux__) && !defined(MATLAB_MEX_FILE)

volatile float g_ctrl_in_vals[CTRL_IN_MAX_PARAMS] = {0};

void ctrl_in_set(unsigned int idx, float val)
{
    if (idx < CTRL_IN_MAX_PARAMS)
        g_ctrl_in_vals[idx] = val;
}

float ctrl_in_get(unsigned int idx)
{
    if (idx < CTRL_IN_MAX_PARAMS)
        return g_ctrl_in_vals[idx];
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

void ctrl_in(SimStruct *rts)
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
