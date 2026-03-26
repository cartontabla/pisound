/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: reverb.c
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

#include "reverb.h"
#include "rtwtypes.h"
#include "reverb_private.h"
#include <string.h>
#include <math.h>
#include "rt_nonfinite.h"

/* Block signals (default storage) */
B_reverb_T reverb_B;

/* Block states (default storage) */
DW_reverb_T reverb_DW;

/* Real-time model */
static RT_MODEL_reverb_T reverb_M_;
RT_MODEL_reverb_T *const reverb_M = &reverb_M_;
real_T rt_powd_snf(real_T u0, real_T u1)
{
  real_T y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else {
    real_T tmp;
    real_T tmp_0;
    tmp = fabs(u0);
    tmp_0 = fabs(u1);
    if (rtIsInf(u1)) {
      if (tmp == 1.0) {
        y = 1.0;
      } else if (tmp > 1.0) {
        if (u1 > 0.0) {
          y = (rtInf);
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = (rtInf);
      }
    } else if (tmp_0 == 0.0) {
      y = 1.0;
    } else if (tmp_0 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = (rtNaN);
    } else {
      y = pow(u0, u1);
    }
  }

  return y;
}

/*
 * Output and update for atomic system:
 *    '<S1>/MATLAB Function'
 *    '<S2>/MATLAB Function'
 *    '<S3>/MATLAB Function'
 *    '<S6>/MATLAB Function'
 */
void reverb_MATLABFunction(real_T rtu_RT60, real_T rtu_D, real_T *rty_g)
{
  real_T RT60;
  RT60 = rtu_RT60;
  if (rtu_RT60 < 0.05) {
    RT60 = 0.05;
  }

  *rty_g = rt_powd_snf(10.0, -3.0 * rtu_D / RT60);
  if (*rty_g > 0.95) {
    *rty_g = 0.95;
  }

  if (*rty_g < 0.0) {
    *rty_g = 0.0;
  }
}

/*
 * Output and update for atomic system:
 *    '<S1>/MATLAB Function1'
 *    '<S2>/MATLAB Function1'
 *    '<S3>/MATLAB Function1'
 *    '<S6>/MATLAB Function1'
 */
void reverb_MATLABFunction1(real_T rtu_u, real_T rty_y[128])
{
  int32_T i;
  for (i = 0; i < 128; i++) {
    rty_y[i] = rtu_u;
  }
}

/* System initialize for atomic system: */
void reverb_MATLABSystem_Init(DW_MATLABSystem_reverb_T *localDW,
  P_MATLABSystem_reverb_T *localP)
{
  /* Start for MATLABSystem: '<S1>/MATLAB System' */
  localDW->objisempty = true;
  localDW->obj.Mode = localP->MATLABSystem_Mode;
  localDW->obj.Delay = localP->MATLABSystem_Delay;
  localDW->obj.g = localP->MATLABSystem_g;
  localDW->obj.isInitialized = 1;

  /*  inL, inR, mode, time, repeat(g) */
  /*  L constante para codegen (deriva de Nontunable Fs y MaxDelay) */
  localDW->obj.L = 96002.0;

  /* InitializeConditions for MATLABSystem: '<S1>/MATLAB System' */
  memset(&localDW->obj.xBufL[0], 0, 96002U * sizeof(real_T));
  memset(&localDW->obj.xBufR[0], 0, 96002U * sizeof(real_T));
  memset(&localDW->obj.yBufL[0], 0, 96002U * sizeof(real_T));
  memset(&localDW->obj.yBufR[0], 0, 96002U * sizeof(real_T));
  localDW->obj.w = 1.0;
  localDW->w = 1.0;
}

/* Output and update for atomic system: */
void reverb_MATLABSystem(const real_T rtu_0[128], const real_T rtu_1[128],
  uint8_T rtu_2, const real_T rtu_3[128], real_T rtu_4, B_MATLABSystem_reverb_T *
  localB, DW_MATLABSystem_reverb_T *localDW, P_MATLABSystem_reverb_T *localP)
{
  real_T Dmax;
  real_T g;
  int32_T b_n;

  /* MATLABSystem: '<S1>/MATLAB System' */
  if (localDW->obj.Mode != localP->MATLABSystem_Mode) {
    localDW->obj.Mode = localP->MATLABSystem_Mode;
  }

  if (localDW->obj.Delay != localP->MATLABSystem_Delay) {
    localDW->obj.Delay = localP->MATLABSystem_Delay;
  }

  if (localDW->obj.g != localP->MATLABSystem_g) {
    localDW->obj.g = localP->MATLABSystem_g;
  }

  /*  inL, inR, mode, time, repeat(g) */
  /*  Entradas de control (escalares) */
  g = fmax(-1.0, fmin(1.0, rtu_4));

  /*  [-1,1] */
  if (rtu_2 != 1) {
    if (g >= 1.0) {
      g = 0.999;
    }

    if (g <= -1.0) {
      g = -0.999;
    }
  }

  Dmax = localDW->obj.L - 2.0;
  for (b_n = 0; b_n < 128; b_n++) {
    real_T D;
    real_T Di;
    real_T r0;
    real_T r1;

    /*  Retardo en muestras (fraccionario permitido) */
    D = rtu_3[b_n] * 48000.0;

    /*  clamp robusto */
    if (D < 1.0) {
      D = 1.0;
    }

    if (D > Dmax) {
      D = Dmax;
    }

    Di = floor(D);
    D -= Di;
    r0 = localDW->obj.w - Di;
    if (localDW->obj.L == 0.0) {
      Di = r0 - 1.0;
      if (r0 - 1.0 == 0.0) {
        Di = localDW->obj.L;
      }
    } else if (rtIsNaN(r0 - 1.0) || rtIsNaN(localDW->obj.L) || rtIsInf(r0 - 1.0))
    {
      Di = (rtNaN);
    } else if (rtIsInf(localDW->obj.L)) {
      if (localDW->obj.L > 0.0) {
        if (r0 - 1.0 > 0.0) {
          Di = r0 - 1.0;
        } else if (r0 - 1.0 < 0.0) {
          Di = localDW->obj.L;
        } else {
          Di = 0.0;
        }
      } else if (r0 - 1.0 > 0.0) {
        Di = localDW->obj.L;
      } else if (r0 - 1.0 < 0.0) {
        Di = r0 - 1.0;
      } else {
        Di = -0.0;
      }
    } else {
      if (localDW->obj.L > floor(localDW->obj.L)) {
        Di = fabs((r0 - 1.0) / localDW->obj.L);
        if (fabs(Di - floor(Di + 0.5)) > 2.2204460492503131E-16 * Di) {
          Di = fmod(r0 - 1.0, localDW->obj.L);
        } else {
          Di = 0.0;
        }
      } else {
        Di = fmod(r0 - 1.0, localDW->obj.L);
      }

      if (Di == 0.0) {
        Di = localDW->obj.L * 0.0;
      } else if (Di < 0.0) {
        if (localDW->obj.L > 0.0) {
          Di += localDW->obj.L;
        }
      } else if ((Di > 0.0) && (localDW->obj.L < 0.0)) {
        Di += localDW->obj.L;
      }
    }

    r1 = (Di + 1.0) - 1.0;
    if ((Di + 1.0) - 1.0 < 1.0) {
      r1 = localDW->obj.L;
    }

    switch (rtu_2) {
     case 0U:
      /*  pure delay */
      r0 = localDW->obj.xBufL[(int32_T)(Di + 1.0) - 1] * (1.0 - D) +
        localDW->obj.xBufL[(int32_T)r1 - 1] * D;
      D = localDW->obj.xBufR[(int32_T)(Di + 1.0) - 1] * (1.0 - D) +
        localDW->obj.xBufR[(int32_T)r1 - 1] * D;
      break;

     case 1U:
      /*  comb FIR */
      r0 = (localDW->obj.xBufL[(int32_T)(Di + 1.0) - 1] * (1.0 - D) +
            localDW->obj.xBufL[(int32_T)r1 - 1] * D) * g + rtu_0[b_n];
      D = (localDW->obj.xBufR[(int32_T)(Di + 1.0) - 1] * (1.0 - D) +
           localDW->obj.xBufR[(int32_T)r1 - 1] * D) * g + rtu_1[b_n];
      break;

     case 2U:
      /*  comb IIR */
      r0 = (localDW->obj.yBufL[(int32_T)(Di + 1.0) - 1] * (1.0 - D) +
            localDW->obj.yBufL[(int32_T)r1 - 1] * D) * g + rtu_0[b_n];
      D = (localDW->obj.yBufR[(int32_T)(Di + 1.0) - 1] * (1.0 - D) +
           localDW->obj.yBufR[(int32_T)r1 - 1] * D) * g + rtu_1[b_n];
      break;

     case 3U:
      /*  all-pass */
      r0 = ((localDW->obj.xBufL[(int32_T)(Di + 1.0) - 1] * (1.0 - D) +
             localDW->obj.xBufL[(int32_T)r1 - 1] * D) + -g * rtu_0[b_n]) +
        (localDW->obj.yBufL[(int32_T)(Di + 1.0) - 1] * (1.0 - D) +
         localDW->obj.yBufL[(int32_T)r1 - 1] * D) * g;
      D = ((localDW->obj.xBufR[(int32_T)(Di + 1.0) - 1] * (1.0 - D) +
            localDW->obj.xBufR[(int32_T)r1 - 1] * D) + -g * rtu_1[b_n]) +
        (localDW->obj.yBufR[(int32_T)(Di + 1.0) - 1] * (1.0 - D) +
         localDW->obj.yBufR[(int32_T)r1 - 1] * D) * g;
      break;

     default:
      r0 = rtu_0[b_n];
      D = rtu_1[b_n];
      break;
    }

    localB->MATLABSystem_o1[b_n] = r0;
    localB->MATLABSystem_o2[b_n] = D;
    localDW->obj.xBufL[(int32_T)localDW->obj.w - 1] = rtu_0[b_n];
    localDW->obj.xBufR[(int32_T)localDW->obj.w - 1] = rtu_1[b_n];
    localDW->obj.yBufL[(int32_T)localDW->obj.w - 1] = r0;
    localDW->obj.yBufR[(int32_T)localDW->obj.w - 1] = D;
    localDW->obj.w++;
    if (localDW->obj.w > localDW->obj.L) {
      localDW->obj.w = 1.0;
    }
  }

  localDW->w = localDW->obj.w;

  /* End of MATLABSystem: '<S1>/MATLAB System' */
}

/* Termination for atomic system: */
void reverb_MATLABSystem_Term(DW_MATLABSystem_reverb_T *localDW)
{
  /* Terminate for MATLABSystem: '<S1>/MATLAB System' */
  localDW->w = localDW->obj.w;
}

/* System initialize for atomic system: */
void reverb_MATLABSystem_k_Init(DW_MATLABSystem_reverb_f_T *localDW,
  P_MATLABSystem_reverb_g_T *localP)
{
  /* Start for MATLABSystem: '<S4>/MATLAB System' */
  localDW->objisempty = true;
  localDW->obj.Mode = localP->MATLABSystem_Mode;
  localDW->obj.Delay = localP->MATLABSystem_Delay;
  localDW->obj.g = localP->MATLABSystem_g;
  localDW->obj.isInitialized = 1;

  /*  inL, inR, mode, time, repeat(g) */
  /*  L constante para codegen (deriva de Nontunable Fs y MaxDelay) */
  localDW->obj.L = 96002.0;

  /* InitializeConditions for MATLABSystem: '<S4>/MATLAB System' */
  memset(&localDW->obj.xBufL[0], 0, 96002U * sizeof(real_T));
  memset(&localDW->obj.xBufR[0], 0, 96002U * sizeof(real_T));
  memset(&localDW->obj.yBufL[0], 0, 96002U * sizeof(real_T));
  memset(&localDW->obj.yBufR[0], 0, 96002U * sizeof(real_T));
  localDW->obj.w = 1.0;
  localDW->w = 1.0;
}

/* Output and update for atomic system: */
void reverb_MATLABSystem_p(const real_T rtu_0[128], const real_T rtu_1[128],
  uint8_T rtu_2, real_T rtu_3, real_T rtu_4, B_MATLABSystem_reverb_c_T *localB,
  DW_MATLABSystem_reverb_f_T *localDW, P_MATLABSystem_reverb_g_T *localP)
{
  real_T Dmax;
  real_T g;
  int32_T b_n;

  /* MATLABSystem: '<S4>/MATLAB System' */
  if (localDW->obj.Mode != localP->MATLABSystem_Mode) {
    localDW->obj.Mode = localP->MATLABSystem_Mode;
  }

  if (localDW->obj.Delay != localP->MATLABSystem_Delay) {
    localDW->obj.Delay = localP->MATLABSystem_Delay;
  }

  if (localDW->obj.g != localP->MATLABSystem_g) {
    localDW->obj.g = localP->MATLABSystem_g;
  }

  /*  inL, inR, mode, time, repeat(g) */
  /*  Entradas de control (escalares) */
  g = fmax(-1.0, fmin(1.0, rtu_4));

  /*  [-1,1] */
  if (rtu_2 != 1) {
    if (g >= 1.0) {
      g = 0.999;
    }

    if (g <= -1.0) {
      g = -0.999;
    }
  }

  Dmax = localDW->obj.L - 2.0;
  for (b_n = 0; b_n < 128; b_n++) {
    real_T D;
    real_T Di;
    real_T r0;
    real_T r1;

    /*  Retardo en muestras (fraccionario permitido) */
    D = rtu_3 * 48000.0;

    /*  clamp robusto */
    if (D < 1.0) {
      D = 1.0;
    }

    if (D > Dmax) {
      D = Dmax;
    }

    Di = floor(D);
    D -= Di;
    r0 = localDW->obj.w - Di;
    if (localDW->obj.L == 0.0) {
      Di = r0 - 1.0;
      if (r0 - 1.0 == 0.0) {
        Di = localDW->obj.L;
      }
    } else if (rtIsNaN(r0 - 1.0) || rtIsNaN(localDW->obj.L) || rtIsInf(r0 - 1.0))
    {
      Di = (rtNaN);
    } else if (rtIsInf(localDW->obj.L)) {
      if (localDW->obj.L > 0.0) {
        if (r0 - 1.0 > 0.0) {
          Di = r0 - 1.0;
        } else if (r0 - 1.0 < 0.0) {
          Di = localDW->obj.L;
        } else {
          Di = 0.0;
        }
      } else if (r0 - 1.0 > 0.0) {
        Di = localDW->obj.L;
      } else if (r0 - 1.0 < 0.0) {
        Di = r0 - 1.0;
      } else {
        Di = -0.0;
      }
    } else {
      if (localDW->obj.L > floor(localDW->obj.L)) {
        Di = fabs((r0 - 1.0) / localDW->obj.L);
        if (fabs(Di - floor(Di + 0.5)) > 2.2204460492503131E-16 * Di) {
          Di = fmod(r0 - 1.0, localDW->obj.L);
        } else {
          Di = 0.0;
        }
      } else {
        Di = fmod(r0 - 1.0, localDW->obj.L);
      }

      if (Di == 0.0) {
        Di = localDW->obj.L * 0.0;
      } else if (Di < 0.0) {
        if (localDW->obj.L > 0.0) {
          Di += localDW->obj.L;
        }
      } else if ((Di > 0.0) && (localDW->obj.L < 0.0)) {
        Di += localDW->obj.L;
      }
    }

    r1 = (Di + 1.0) - 1.0;
    if ((Di + 1.0) - 1.0 < 1.0) {
      r1 = localDW->obj.L;
    }

    switch (rtu_2) {
     case 0U:
      /*  pure delay */
      r0 = localDW->obj.xBufL[(int32_T)(Di + 1.0) - 1] * (1.0 - D) +
        localDW->obj.xBufL[(int32_T)r1 - 1] * D;
      D = localDW->obj.xBufR[(int32_T)(Di + 1.0) - 1] * (1.0 - D) +
        localDW->obj.xBufR[(int32_T)r1 - 1] * D;
      break;

     case 1U:
      /*  comb FIR */
      r0 = (localDW->obj.xBufL[(int32_T)(Di + 1.0) - 1] * (1.0 - D) +
            localDW->obj.xBufL[(int32_T)r1 - 1] * D) * g + rtu_0[b_n];
      D = (localDW->obj.xBufR[(int32_T)(Di + 1.0) - 1] * (1.0 - D) +
           localDW->obj.xBufR[(int32_T)r1 - 1] * D) * g + rtu_1[b_n];
      break;

     case 2U:
      /*  comb IIR */
      r0 = (localDW->obj.yBufL[(int32_T)(Di + 1.0) - 1] * (1.0 - D) +
            localDW->obj.yBufL[(int32_T)r1 - 1] * D) * g + rtu_0[b_n];
      D = (localDW->obj.yBufR[(int32_T)(Di + 1.0) - 1] * (1.0 - D) +
           localDW->obj.yBufR[(int32_T)r1 - 1] * D) * g + rtu_1[b_n];
      break;

     case 3U:
      /*  all-pass */
      r0 = ((localDW->obj.xBufL[(int32_T)(Di + 1.0) - 1] * (1.0 - D) +
             localDW->obj.xBufL[(int32_T)r1 - 1] * D) + -g * rtu_0[b_n]) +
        (localDW->obj.yBufL[(int32_T)(Di + 1.0) - 1] * (1.0 - D) +
         localDW->obj.yBufL[(int32_T)r1 - 1] * D) * g;
      D = ((localDW->obj.xBufR[(int32_T)(Di + 1.0) - 1] * (1.0 - D) +
            localDW->obj.xBufR[(int32_T)r1 - 1] * D) + -g * rtu_1[b_n]) +
        (localDW->obj.yBufR[(int32_T)(Di + 1.0) - 1] * (1.0 - D) +
         localDW->obj.yBufR[(int32_T)r1 - 1] * D) * g;
      break;

     default:
      r0 = rtu_0[b_n];
      D = rtu_1[b_n];
      break;
    }

    localB->MATLABSystem_o1[b_n] = r0;
    localB->MATLABSystem_o2[b_n] = D;
    localDW->obj.xBufL[(int32_T)localDW->obj.w - 1] = rtu_0[b_n];
    localDW->obj.xBufR[(int32_T)localDW->obj.w - 1] = rtu_1[b_n];
    localDW->obj.yBufL[(int32_T)localDW->obj.w - 1] = r0;
    localDW->obj.yBufR[(int32_T)localDW->obj.w - 1] = D;
    localDW->obj.w++;
    if (localDW->obj.w > localDW->obj.L) {
      localDW->obj.w = 1.0;
    }
  }

  localDW->w = localDW->obj.w;

  /* End of MATLABSystem: '<S4>/MATLAB System' */
}

/* Termination for atomic system: */
void reverb_MATLABSystem_p_Term(DW_MATLABSystem_reverb_f_T *localDW)
{
  /* Terminate for MATLABSystem: '<S4>/MATLAB System' */
  localDW->w = localDW->obj.w;
}

/* Model step function */
void reverb_step(void)
{
  real_T rtb_Add3[128];
  real_T rtb_Gain[128];
  real_T rtb_Gain1[128];
  real_T rtb_Gain3[128];
  real_T a_;
  real_T gd1_;
  real_T gd2_;
  real_T gw1_;
  real_T gw2_;
  real_T rtb_g;
  real_T tmp;
  int32_T i;
  int32_T rtb_Gain3_0;

  /* S-Function (pisound_in): '<Root>/PiSound Input' */

  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = reverb_M->childSfunctions[0];
    sfcnOutputs(rts,0);
  }

  /* Gain: '<Root>/Gain' incorporates:
   *  DataTypeConversion: '<Root>/Data Type Conversion'
   */
  for (i = 0; i < 128; i++) {
    rtb_Gain[i] = reverb_P.Gain_Gain * (real_T)reverb_B.PiSoundInput_o1[i];
  }

  /* End of Gain: '<Root>/Gain' */

  /* MATLAB Function: '<S1>/MATLAB Function' incorporates:
   *  Constant: '<Root>/Constant2'
   *  Constant: '<S1>/Constant1'
   */
  reverb_MATLABFunction(reverb_P.RT60_rt, reverb_P.Time1_rt, &rtb_g);
  for (i = 0; i < 128; i++) {
    /* DataTypeConversion: '<Root>/Data Type Conversion2' */
    rtb_Gain3_0 = reverb_B.PiSoundInput_o2[i];
    rtb_Gain3[i] = rtb_Gain3_0;

    /* Gain: '<Root>/Gain1' */
    rtb_Gain1[i] = reverb_P.Gain1_Gain * (real_T)rtb_Gain3_0;
  }

  /* MATLAB Function: '<S1>/MATLAB Function1' incorporates:
   *  Constant: '<S1>/Constant1'
   */
  reverb_MATLABFunction1(reverb_P.Time1_rt, rtb_Gain3);

  /* Constant: '<S1>/Constant' */
  reverb_MATLABSystem(rtb_Gain, rtb_Gain1, reverb_P.Constant_Value, rtb_Gain3,
                      rtb_g, &reverb_B.MATLABSystem_p, &reverb_DW.MATLABSystem_p,
                      &reverb_P.MATLABSystem_p);

  /* MATLAB Function: '<S6>/MATLAB Function' incorporates:
   *  Constant: '<Root>/Constant2'
   *  Constant: '<S6>/Constant1'
   */
  reverb_MATLABFunction(reverb_P.RT60_rt, reverb_P.Time2_rt, &rtb_g);

  /* MATLAB Function: '<S6>/MATLAB Function1' incorporates:
   *  Constant: '<S6>/Constant1'
   */
  reverb_MATLABFunction1(reverb_P.Time2_rt, rtb_Gain3);

  /* Constant: '<S6>/Constant' */
  reverb_MATLABSystem(rtb_Gain, rtb_Gain1, reverb_P.Constant_Value_g, rtb_Gain3,
                      rtb_g, &reverb_B.MATLABSystem_pnaevv,
                      &reverb_DW.MATLABSystem_pnaevv,
                      &reverb_P.MATLABSystem_pnaevv);

  /* MATLAB Function: '<S2>/MATLAB Function' incorporates:
   *  Constant: '<Root>/Constant2'
   *  Constant: '<S2>/Constant1'
   */
  reverb_MATLABFunction(reverb_P.RT60_rt, reverb_P.Time3_rt, &rtb_g);

  /* MATLAB Function: '<S2>/MATLAB Function1' incorporates:
   *  Constant: '<S2>/Constant1'
   */
  reverb_MATLABFunction1(reverb_P.Time3_rt, rtb_Gain3);

  /* Constant: '<S2>/Constant' */
  reverb_MATLABSystem(rtb_Gain, rtb_Gain1, reverb_P.Constant_Value_a, rtb_Gain3,
                      rtb_g, &reverb_B.MATLABSystem_pn,
                      &reverb_DW.MATLABSystem_pn, &reverb_P.MATLABSystem_pn);

  /* MATLAB Function: '<S3>/MATLAB Function' incorporates:
   *  Constant: '<Root>/Constant2'
   *  Constant: '<S3>/Constant1'
   */
  reverb_MATLABFunction(reverb_P.RT60_rt, reverb_P.Time4_rt, &rtb_g);

  /* MATLAB Function: '<S3>/MATLAB Function1' incorporates:
   *  Constant: '<S3>/Constant1'
   */
  reverb_MATLABFunction1(reverb_P.Time4_rt, rtb_Gain3);

  /* Constant: '<S3>/Constant' */
  reverb_MATLABSystem(rtb_Gain, rtb_Gain1, reverb_P.Constant_Value_av, rtb_Gain3,
                      rtb_g, &reverb_B.MATLABSystem_pna,
                      &reverb_DW.MATLABSystem_pna, &reverb_P.MATLABSystem_pna);
  for (i = 0; i < 128; i++) {
    /* Sum: '<Root>/Add2' incorporates:
     *  Gain: '<S1>/Gain8'
     *  Gain: '<S2>/Gain8'
     *  Gain: '<S3>/Gain8'
     *  Gain: '<S6>/Gain8'
     *  MATLABSystem: '<S1>/MATLAB System'
     *  MATLABSystem: '<S2>/MATLAB System'
     *  MATLABSystem: '<S3>/MATLAB System'
     *  MATLABSystem: '<S6>/MATLAB System'
     */
    rtb_Gain3[i] = ((reverb_P.Gain8_Gain *
                     reverb_B.MATLABSystem_p.MATLABSystem_o1[i] +
                     reverb_P.Gain8_Gain_m *
                     reverb_B.MATLABSystem_pnaevv.MATLABSystem_o1[i]) +
                    reverb_P.Gain8_Gain_l *
                    reverb_B.MATLABSystem_pn.MATLABSystem_o1[i]) +
      reverb_P.Gain8_Gain_o * reverb_B.MATLABSystem_pna.MATLABSystem_o1[i];

    /* Sum: '<Root>/Add3' incorporates:
     *  Gain: '<S1>/Gain9'
     *  Gain: '<S2>/Gain9'
     *  Gain: '<S3>/Gain9'
     *  Gain: '<S6>/Gain9'
     *  MATLABSystem: '<S1>/MATLAB System'
     *  MATLABSystem: '<S2>/MATLAB System'
     *  MATLABSystem: '<S3>/MATLAB System'
     *  MATLABSystem: '<S6>/MATLAB System'
     */
    rtb_Add3[i] = ((reverb_P.Gain9_Gain *
                    reverb_B.MATLABSystem_p.MATLABSystem_o2[i] +
                    reverb_P.Gain9_Gain_p *
                    reverb_B.MATLABSystem_pnaevv.MATLABSystem_o2[i]) +
                   reverb_P.Gain9_Gain_j *
                   reverb_B.MATLABSystem_pn.MATLABSystem_o2[i]) +
      reverb_P.Gain9_Gain_a * reverb_B.MATLABSystem_pna.MATLABSystem_o2[i];
  }

  /* Constant: '<S4>/Constant' incorporates:
   *  Constant: '<S4>/Constant1'
   *  Constant: '<S4>/Constant2'
   */
  reverb_MATLABSystem_p(rtb_Gain3, rtb_Add3, reverb_P.Constant_Value_b,
                        reverb_P.Constant1_Value, reverb_P.Constant2_Value,
                        &reverb_B.MATLABSystem_pnae,
                        &reverb_DW.MATLABSystem_pnae,
                        &reverb_P.MATLABSystem_pnae);
  for (i = 0; i < 128; i++) {
    /* Gain: '<S4>/Gain8' incorporates:
     *  MATLABSystem: '<S4>/MATLAB System'
     */
    rtb_Gain3[i] = reverb_P.Gain8_Gain_c *
      reverb_B.MATLABSystem_pnae.MATLABSystem_o1[i];

    /* Gain: '<S4>/Gain9' incorporates:
     *  MATLABSystem: '<S4>/MATLAB System'
     */
    rtb_Add3[i] = reverb_P.Gain9_Gain_c *
      reverb_B.MATLABSystem_pnae.MATLABSystem_o2[i];
  }

  /* Constant: '<S5>/Constant' incorporates:
   *  Constant: '<S5>/Constant1'
   *  Constant: '<S5>/Constant2'
   */
  reverb_MATLABSystem_p(rtb_Gain3, rtb_Add3, reverb_P.Constant_Value_m,
                        reverb_P.Constant1_Value_e, reverb_P.Constant2_Value_e,
                        &reverb_B.MATLABSystem_pnaev,
                        &reverb_DW.MATLABSystem_pnaev,
                        &reverb_P.MATLABSystem_pnaev);
  for (i = 0; i < 128; i++) {
    /* Gain: '<S5>/Gain8' incorporates:
     *  MATLABSystem: '<S5>/MATLAB System'
     */
    rtb_Gain3[i] = reverb_P.Gain8_Gain_cn *
      reverb_B.MATLABSystem_pnaev.MATLABSystem_o1[i];

    /* Gain: '<S5>/Gain9' incorporates:
     *  MATLABSystem: '<S5>/MATLAB System'
     */
    rtb_Add3[i] = reverb_P.Gain9_Gain_js *
      reverb_B.MATLABSystem_pnaev.MATLABSystem_o2[i];
  }

  /* MATLABSystem: '<Root>/MATLAB System' incorporates:
   *  Constant: '<Root>/Constant1'
   */
  if (reverb_DW.obj.SmoothTime != reverb_P.MATLABSystem_SmoothTime) {
    reverb_DW.obj.SmoothTime = reverb_P.MATLABSystem_SmoothTime;
  }

  if (reverb_DW.obj.IndepInDB != reverb_P.MATLABSystem_IndepInDB) {
    reverb_DW.obj.IndepInDB = reverb_P.MATLABSystem_IndepInDB;
  }

  /*  inL,inR,wetL,wetR,p1 (+p2 si Mode=2) */
  /*  Mode 3: crossfade por muestra (p1 puede ser [N x 1]) */
  /*  dummy */
  rtb_g = fmin(fmax(reverb_P.Mix_rt, 0.0), 1.0);
  a_ = reverb_DW.obj.a;
  gd1_ = reverb_DW.obj.gd1;
  gd2_ = reverb_DW.obj.gd2;
  gw1_ = reverb_DW.obj.gw1;
  gw2_ = reverb_DW.obj.gw2;

  /*  más robusto que obj.N */
  for (i = 0; i < 128; i++) {
    gd1_ += ((1.0 - rtb_g) - gd1_) * a_;
    gw1_ += (rtb_g - gw1_) * a_;
    gd2_ += ((1.0 - rtb_g) - gd2_) * a_;
    gw2_ += (rtb_g - gw2_) * a_;

    /* DataTypeConversion: '<Root>/Data Type Conversion1' incorporates:
     *  Gain: '<Root>/Gain2'
     *  Gain: '<S5>/Gain8'
     */
    tmp = floor((gd1_ * rtb_Gain[i] + gw1_ * rtb_Gain3[i]) * reverb_P.Gain2_Gain);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion1' */
    reverb_B.DataTypeConversion1[i] = tmp < 0.0 ? -(int32_T)(uint32_T)-tmp :
      (int32_T)(uint32_T)tmp;

    /* DataTypeConversion: '<Root>/Data Type Conversion3' incorporates:
     *  Gain: '<Root>/Gain3'
     *  Gain: '<S5>/Gain9'
     */
    tmp = floor((gd2_ * rtb_Gain1[i] + gw2_ * rtb_Add3[i]) * reverb_P.Gain3_Gain);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion3' */
    reverb_B.DataTypeConversion3[i] = tmp < 0.0 ? -(int32_T)(uint32_T)-tmp :
      (int32_T)(uint32_T)tmp;
  }

  reverb_DW.obj.gd1 = gd1_;
  reverb_DW.obj.gd2 = gd2_;
  reverb_DW.obj.gw1 = gw1_;
  reverb_DW.obj.gw2 = gw2_;

  /* End of MATLABSystem: '<Root>/MATLAB System' */

  /* S-Function (pisound_out): '<Root>/PiSound Output' */

  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = reverb_M->childSfunctions[1];
    sfcnOutputs(rts,0);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  reverb_M->Timing.t[0] =
    ((time_T)(++reverb_M->Timing.clockTick0)) * reverb_M->Timing.stepSize0;
}

/* Model initialize function */
void reverb_initialize(void)
{
  /* Registration code */
  rtsiSetSolverName(&reverb_M->solverInfo,"FixedStepDiscrete");
  reverb_M->solverInfoPtr = (&reverb_M->solverInfo);

  /* Initialize timing info */
  {
    int_T *mdlTsMap = reverb_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    reverb_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    reverb_M->Timing.sampleTimes = (&reverb_M->Timing.sampleTimesArray[0]);
    reverb_M->Timing.offsetTimes = (&reverb_M->Timing.offsetTimesArray[0]);

    /* task periods */
    reverb_M->Timing.sampleTimes[0] = (0.0026666666666666666);

    /* task offsets */
    reverb_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(reverb_M, &reverb_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = reverb_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    reverb_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(reverb_M, -1);
  reverb_M->Timing.stepSize0 = 0.0026666666666666666;
  reverb_M->solverInfoPtr = (&reverb_M->solverInfo);
  reverb_M->Timing.stepSize = (0.0026666666666666666);
  rtsiSetFixedStepSize(&reverb_M->solverInfo, 0.0026666666666666666);
  rtsiSetSolverMode(&reverb_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* child S-Function registration */
  {
    RTWSfcnInfo *sfcnInfo = &reverb_M->NonInlinedSFcns.sfcnInfo;
    reverb_M->sfcnInfo = (sfcnInfo);
    rtssSetErrorStatusPtr(sfcnInfo, (&rtmGetErrorStatus(reverb_M)));
    reverb_M->Sizes.numSampTimes = (1);
    rtssSetNumRootSampTimesPtr(sfcnInfo, &reverb_M->Sizes.numSampTimes);
    reverb_M->NonInlinedSFcns.taskTimePtrs[0] = (&rtmGetTPtr(reverb_M)[0]);
    rtssSetTPtrPtr(sfcnInfo,reverb_M->NonInlinedSFcns.taskTimePtrs);
    rtssSetTStartPtr(sfcnInfo, &rtmGetTStart(reverb_M));
    rtssSetTFinalPtr(sfcnInfo, &rtmGetTFinal(reverb_M));
    rtssSetTimeOfLastOutputPtr(sfcnInfo, &rtmGetTimeOfLastOutput(reverb_M));
    rtssSetStepSizePtr(sfcnInfo, &reverb_M->Timing.stepSize);
    rtssSetStopRequestedPtr(sfcnInfo, &rtmGetStopRequested(reverb_M));
    rtssSetDerivCacheNeedsResetPtr(sfcnInfo, &reverb_M->derivCacheNeedsReset);
    rtssSetZCCacheNeedsResetPtr(sfcnInfo, &reverb_M->zCCacheNeedsReset);
    rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(sfcnInfo,
      &reverb_M->CTOutputIncnstWithState);
    rtssSetSampleHitsPtr(sfcnInfo, &reverb_M->Timing.sampleHits);
    rtssSetPerTaskSampleHitsPtr(sfcnInfo, &reverb_M->Timing.perTaskSampleHits);
    rtssSetSimModePtr(sfcnInfo, &reverb_M->simMode);
    rtssSetSolverInfoPtr(sfcnInfo, &reverb_M->solverInfoPtr);
  }

  reverb_M->Sizes.numSFcns = (2);

  /* register each child */
  {
    (void) memset((void *)&reverb_M->NonInlinedSFcns.childSFunctions[0], 0,
                  2*sizeof(SimStruct));
    reverb_M->childSfunctions = (&reverb_M->NonInlinedSFcns.childSFunctionPtrs[0]);
    reverb_M->childSfunctions[0] = (&reverb_M->NonInlinedSFcns.childSFunctions[0]);
    reverb_M->childSfunctions[1] = (&reverb_M->NonInlinedSFcns.childSFunctions[1]);

    /* Level2 S-Function Block: reverb/<Root>/PiSound Input (pisound_in) */
    {
      SimStruct *rts = reverb_M->childSfunctions[0];

      /* timing info */
      time_T *sfcnPeriod = reverb_M->NonInlinedSFcns.Sfcn0.sfcnPeriod;
      time_T *sfcnOffset = reverb_M->NonInlinedSFcns.Sfcn0.sfcnOffset;
      int_T *sfcnTsMap = reverb_M->NonInlinedSFcns.Sfcn0.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &reverb_M->NonInlinedSFcns.blkInfo2[0]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &reverb_M->NonInlinedSFcns.inputOutputPortInfo2[0]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, reverb_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &reverb_M->NonInlinedSFcns.methods2[0]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &reverb_M->NonInlinedSFcns.methods3[0]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &reverb_M->NonInlinedSFcns.methods4[0]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &reverb_M->NonInlinedSFcns.statesInfo2[0]);
        ssSetPeriodicStatesInfo(rts,
          &reverb_M->NonInlinedSFcns.periodicStatesInfo[0]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &reverb_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        ssSetPortInfoForOutputs(rts,
          &reverb_M->NonInlinedSFcns.Sfcn0.outputPortInfo[0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &reverb_M->NonInlinedSFcns.Sfcn0.outputPortUnits[0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &reverb_M->NonInlinedSFcns.Sfcn0.outputPortCoSimAttribute[0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidthAsInt(rts, 0, 128);
          ssSetOutputPortSignal(rts, 0, ((int32_T *) reverb_B.PiSoundInput_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidthAsInt(rts, 1, 128);
          ssSetOutputPortSignal(rts, 1, ((int32_T *) reverb_B.PiSoundInput_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "PiSound Input");
      ssSetPath(rts, "reverb/PiSound Input");
      ssSetRTModel(rts,reverb_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &reverb_M->NonInlinedSFcns.Sfcn0.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)reverb_P.PiSoundInput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)reverb_P.PiSoundInput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)reverb_P.PiSoundInput_P3_Size);
      }

      /* registration */
      pisound_in(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 0.0026666666666666666);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 0;

      /* set compiled values of dynamic vector attributes */
      ssSetNumNonsampledZCsAsInt(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetOutputPortConnected(rts, 0, 1);
      _ssSetOutputPortConnected(rts, 1, 1);
      _ssSetOutputPortBeingMerged(rts, 0, 0);
      _ssSetOutputPortBeingMerged(rts, 1, 0);

      /* Update the BufferDstPort flags for each input port */
    }

    /* Level2 S-Function Block: reverb/<Root>/PiSound Output (pisound_out) */
    {
      SimStruct *rts = reverb_M->childSfunctions[1];

      /* timing info */
      time_T *sfcnPeriod = reverb_M->NonInlinedSFcns.Sfcn1.sfcnPeriod;
      time_T *sfcnOffset = reverb_M->NonInlinedSFcns.Sfcn1.sfcnOffset;
      int_T *sfcnTsMap = reverb_M->NonInlinedSFcns.Sfcn1.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts, &reverb_M->NonInlinedSFcns.blkInfo2[1]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &reverb_M->NonInlinedSFcns.inputOutputPortInfo2[1]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, reverb_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts, &reverb_M->NonInlinedSFcns.methods2[1]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts, &reverb_M->NonInlinedSFcns.methods3[1]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts, &reverb_M->NonInlinedSFcns.methods4[1]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts, &reverb_M->NonInlinedSFcns.statesInfo2[1]);
        ssSetPeriodicStatesInfo(rts,
          &reverb_M->NonInlinedSFcns.periodicStatesInfo[1]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts,
          &reverb_M->NonInlinedSFcns.Sfcn1.inputPortInfo[0]);
        ssSetPortInfoForInputs(rts,
          &reverb_M->NonInlinedSFcns.Sfcn1.inputPortInfo[0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &reverb_M->NonInlinedSFcns.Sfcn1.inputPortUnits[0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &reverb_M->NonInlinedSFcns.Sfcn1.inputPortCoSimAttribute[0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          int32_T const **sfcnUPtrs = (int32_T const **)
            &reverb_M->NonInlinedSFcns.Sfcn1.UPtrs0;

          {
            int_T i1;
            const int32_T *u0 = reverb_B.DataTypeConversion1;
            for (i1=0; i1 < 128; i1++) {
              sfcnUPtrs[i1] = &u0[i1];
            }
          }

          ssSetInputPortSignalPtrs(rts, 0, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidthAsInt(rts, 0, 128);
        }

        /* port 1 */
        {
          int32_T const **sfcnUPtrs = (int32_T const **)
            &reverb_M->NonInlinedSFcns.Sfcn1.UPtrs1;

          {
            int_T i1;
            const int32_T *u1 = reverb_B.DataTypeConversion3;
            for (i1=0; i1 < 128; i1++) {
              sfcnUPtrs[i1] = &u1[i1];
            }
          }

          ssSetInputPortSignalPtrs(rts, 1, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 1, 1);
          ssSetInputPortWidthAsInt(rts, 1, 128);
        }
      }

      /* path info */
      ssSetModelName(rts, "PiSound Output");
      ssSetPath(rts, "reverb/PiSound Output");
      ssSetRTModel(rts,reverb_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &reverb_M->NonInlinedSFcns.Sfcn1.params;
        ssSetSFcnParamsCount(rts, 3);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)reverb_P.PiSoundOutput_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)reverb_P.PiSoundOutput_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)reverb_P.PiSoundOutput_P3_Size);
      }

      /* registration */
      pisound_out(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 0.0026666666666666666);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 0;

      /* set compiled values of dynamic vector attributes */
      ssSetNumNonsampledZCsAsInt(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetInputPortConnected(rts, 0, 1);
      _ssSetInputPortConnected(rts, 1, 1);

      /* Update the BufferDstPort flags for each input port */
      ssSetInputPortBufferDstPort(rts, 0, -1);
      ssSetInputPortBufferDstPort(rts, 1, -1);
    }
  }

  reverb_MATLABSystem_Init(&reverb_DW.MATLABSystem_p, &reverb_P.MATLABSystem_p);
  reverb_MATLABSystem_Init(&reverb_DW.MATLABSystem_pnaevv,
    &reverb_P.MATLABSystem_pnaevv);
  reverb_MATLABSystem_Init(&reverb_DW.MATLABSystem_pn, &reverb_P.MATLABSystem_pn);
  reverb_MATLABSystem_Init(&reverb_DW.MATLABSystem_pna,
    &reverb_P.MATLABSystem_pna);
  reverb_MATLABSystem_k_Init(&reverb_DW.MATLABSystem_pnae,
    &reverb_P.MATLABSystem_pnae);
  reverb_MATLABSystem_k_Init(&reverb_DW.MATLABSystem_pnaev,
    &reverb_P.MATLABSystem_pnaev);

  /* Start for MATLABSystem: '<Root>/MATLAB System' */
  reverb_DW.obj.SmoothTime = reverb_P.MATLABSystem_SmoothTime;
  reverb_DW.obj.IndepInDB = reverb_P.MATLABSystem_IndepInDB;
  reverb_DW.obj.isInitialized = 1;

  /*  inL,inR,wetL,wetR,p1 (+p2 si Mode=2) */
  if (reverb_DW.obj.SmoothTime <= 0.0) {
    reverb_DW.obj.a = 1.0;
  } else {
    reverb_DW.obj.a = 1.0 - exp(-1.0 / (48000.0 * reverb_DW.obj.SmoothTime));
  }

  /* InitializeConditions for MATLABSystem: '<Root>/MATLAB System' */
  reverb_DW.obj.gd1 = 1.0;
  reverb_DW.obj.gd2 = 1.0;
  reverb_DW.obj.gw1 = 0.0;
  reverb_DW.obj.gw2 = 0.0;
}

/* Model terminate function */
void reverb_terminate(void)
{
  /* Terminate for S-Function (pisound_in): '<Root>/PiSound Input' */
  /* Level2 S-Function Block: '<Root>/PiSound Input' (pisound_in) */
  {
    SimStruct *rts = reverb_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  reverb_MATLABSystem_Term(&reverb_DW.MATLABSystem_p);
  reverb_MATLABSystem_Term(&reverb_DW.MATLABSystem_pnaevv);
  reverb_MATLABSystem_Term(&reverb_DW.MATLABSystem_pn);
  reverb_MATLABSystem_Term(&reverb_DW.MATLABSystem_pna);
  reverb_MATLABSystem_p_Term(&reverb_DW.MATLABSystem_pnae);
  reverb_MATLABSystem_p_Term(&reverb_DW.MATLABSystem_pnaev);

  /* Terminate for S-Function (pisound_out): '<Root>/PiSound Output' */
  /* Level2 S-Function Block: '<Root>/PiSound Output' (pisound_out) */
  {
    SimStruct *rts = reverb_M->childSfunctions[1];
    sfcnTerminate(rts);
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
