/*
 * Stubs for pisound_in and pisound_out S-Functions
 * These replace the hardware I/O functions for LV2 plugin use
 */

#include "simstruc.h"
#include <string.h>

/* pisound_in stub */
void pisound_in(SimStruct *S) {
  /* Do nothing - LV2 wrapper will handle input */
}

/* pisound_out stub */
void pisound_out(SimStruct *S) {
  /* Do nothing - LV2 wrapper will handle output */
}
