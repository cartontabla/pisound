/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: rmod_types.h
 *
 * Code generated for Simulink model 'rmod'.
 *
 * Model version                  : 1.62
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Sat Apr 11 11:25:51 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef rmod_types_h_
#define rmod_types_h_
#include "rtwtypes.h"
#ifndef struct_tag_aOMAp9A5KTOGRyYUC88pSF
#define struct_tag_aOMAp9A5KTOGRyYUC88pSF

struct tag_aOMAp9A5KTOGRyYUC88pSF
{
  int32_T isInitialized;
  real_T SmoothTime;
  boolean_T IndepInDB;
  real_T gd1;
  real_T gd2;
  real_T gw1;
  real_T gw2;
  real_T a;
};

#endif                                 /* struct_tag_aOMAp9A5KTOGRyYUC88pSF */

#ifndef typedef_mix_sys_rmod_T
#define typedef_mix_sys_rmod_T

typedef struct tag_aOMAp9A5KTOGRyYUC88pSF mix_sys_rmod_T;

#endif                                 /* typedef_mix_sys_rmod_T */

#ifndef struct_tag_ntBa79UdhDpc7EqZNC0xB
#define struct_tag_ntBa79UdhDpc7EqZNC0xB

struct tag_ntBa79UdhDpc7EqZNC0xB
{
  int32_T isInitialized;
};

#endif                                 /* struct_tag_ntBa79UdhDpc7EqZNC0xB */

#ifndef typedef_vca_sys_rmod_T
#define typedef_vca_sys_rmod_T

typedef struct tag_ntBa79UdhDpc7EqZNC0xB vca_sys_rmod_T;

#endif                                 /* typedef_vca_sys_rmod_T */

#ifndef struct_tag_ml4f7J7g4jNinCEHPR39EH
#define struct_tag_ml4f7J7g4jNinCEHPR39EH

struct tag_ml4f7J7g4jNinCEHPR39EH
{
  real_T Phase;
  real_T WaveTables[40960];
  real_T TablesSize[10];
};

#endif                                 /* struct_tag_ml4f7J7g4jNinCEHPR39EH */

#ifndef typedef_vco_sys_rmod_T
#define typedef_vco_sys_rmod_T

typedef struct tag_ml4f7J7g4jNinCEHPR39EH vco_sys_rmod_T;

#endif                                 /* typedef_vco_sys_rmod_T */

/* Parameters (default storage) */
typedef struct P_rmod_T_ P_rmod_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_rmod_T RT_MODEL_rmod_T;

#endif                                 /* rmod_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
