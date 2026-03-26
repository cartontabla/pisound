/* fake_tmwtypes.h - Minimal types for Raspberry Pi standalone compilation */
#ifndef TMWTYPES_H
#define TMWTYPES_H

#include <limits.h>

/* Primary types */
typedef float real32_T;
typedef double real64_T;
typedef double real_T;
typedef double time_T;
typedef unsigned char boolean_T;
typedef int int_T;
typedef unsigned int uint_T;
typedef unsigned long ulong_T;
typedef char char_T;
typedef unsigned char uchar_T;
typedef char byte_T;

/* Int types */
typedef signed char int8_T;
typedef unsigned char uint8_T;
typedef short int16_T;
typedef unsigned short uint16_T;
typedef int int32_T;
typedef unsigned int uint32_T;

/* Complex Types */
typedef struct {
  real32_T re;
  real32_T im;
} creal32_T;

typedef struct {
  real64_T re;
  real64_T im;
} creal64_T;

typedef struct {
  real_T re;
  real_T im;
} creal_T;

/* Limits */
#define MAX_int8_T ((int8_T)(127))
#define MIN_int8_T ((int8_T)(-128))
#define MAX_uint8_T ((uint8_T)(255))
#define MIN_uint8_T ((uint8_T)(0))
#define MAX_int16_T ((int16_T)(32767))
#define MIN_int16_T ((int16_T)(-32768))
#define MAX_uint16_T ((uint16_T)(65535))
#define MIN_uint16_T ((uint16_T)(0))
#define MAX_int32_T ((int32_T)(2147483647))
#define MIN_int32_T ((int32_T)(-2147483647 - 1))
#define MAX_uint32_T ((uint32_T)(0xFFFFFFFFU))
#define MIN_uint32_T ((uint32_T)(0))

#endif
