# Guía de Compilación: Modelo Bypass en Raspberry Pi

Esta guía explica cómo transferir el código generado desde MATLAB y compilarlo en tu Raspberry Pi 5 con PiSound.

## 1. Archivos Necesarios

Necesitas transferir TODOS estos archivos a una sola carpeta en la Raspberry Pi (ej: `~/pisound/bypass`):

**De la carpeta `bypass_ert_rtw/`:**
- `bypass.c`
- `bypass.h`
- `bypass_data.c`
- `bypass_private.h`
- `bypass_types.h`
- `ert_main.c`
- `rtwtypes.h`
- `rt_nonfinite.c`
- `rt_nonfinite.h`

**De la carpeta raíz (`/Users/linogarciamorales/.../pisound/`):**
- `pisound_in.c`
- `pisound_out.c`
- `simstruc.h` (Si no tienes este archivo, necesitas obtenerlo de MATLAB)

> **Nota sobre simstruc.h**: Este archivo es parte de MATLAB. Normalmente necesitamos una versión "standalone" o "fake" para compilar fuera de MATLAB. He creado un `fake_simstruc.h` simplificado abajo.

## 2. Crear `fake_simstruc.h` (si no tienes el original)

En la Raspberry Pi, crea este archivo para satisfacer las dependencias:

```c
/* fake_simstruc.h - Minimal definitions to compile S-Functions standalone */
#ifndef FAKE_SIMSTRUC_H
#define FAKE_SIMSTRUC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Data Types */
typedef float real_T;
typedef double time_T;
typedef unsigned char boolean_T;
typedef int int_T;
typedef unsigned int uint_T;
typedef unsigned long ulong_T;
typedef char char_T;
typedef unsigned char uint8_T;
typedef int int32_T;
typedef unsigned int uint32_T;

/* Constants */
#define SS_INT32  0
#define SS_UINT8  1
#define SS_UINT32 2
#define DYNAMICALLY_SIZED -1

/* SimStruct definition (dummy) */
typedef struct SimStruct_tag {
    int dummy;
} SimStruct;

/* Macros to bypass S-Function checks */
#define ssSetNumSFcnParams(S, n)
#define ssGetNumSFcnParams(S) 0
#define ssGetSFcnParamsCount(S) 0
#define ssSetNumContStates(S, n)
#define ssSetNumDiscStates(S, n)
#define ssSetNumInputPorts(S, n) 1
#define ssSetNumOutputPorts(S, n) 1
#define ssSetInputPortWidth(S, p, w)
#define ssSetInputPortDataType(S, p, t)
#define ssSetInputPortDirectFeedThrough(S, p, d)
#define ssSetOutputPortWidth(S, p, w)
#define ssSetOutputPortDataType(S, p, t)
#define ssSetNumSampleTimes(S, n)
#define ssSetNumRWork(S, n)
#define ssSetNumIWork(S, n)
#define ssSetNumPWork(S, n)
#define ssSetNumModes(S, n)
#define ssSetNumNonsampledZCs(S, n)
#define ssSetOptions(S, n)
#define ssSetSampleTime(S, i, t)
#define ssSetOffsetTime(S, i, t)

/* Parameter Access (dummy) */
#define mxGetPr(p) ((double*)0)
#define ssGetSFcnParam(S, i) ((void*)0)

/* Work/Port Access Macros - THESE NEED TO MAP TO GLOBAL VARIABLES IN MAIN */
/* Note: In a real integration, we'd need to link these to the model's I/O */
/* For this bypass test, we might need to modify pisound_in.c/out.c slightly */

#endif
```

## 3. Transferencia (Comandos desde Mac)

```bash
# Crear directorio remoto
ssh patch@192.168.0.31 "mkdir -p ~/pisound/bypass"

# Transferir código generado
scp bypass_ert_rtw/*.c bypass_ert_rtw/*.h patch@192.168.0.31:~/pisound/bypass/

# Transferir S-Functions
scp pisound_in.c pisound_out.c patch@192.168.0.31:~/pisound/bypass/
```

## 4. Compilación en Raspberry Pi

Conéctate por SSH:
```bash
ssh patch@192.168.0.31
cd ~/pisound/bypass
```

Compila usando GCC:
```bash
gcc -o bypass \
    bypass.c \
    bypass_data.c \
    ert_main.c \
    rt_nonfinite.c \
    pisound_in.c \
    pisound_out.c \
    -I. \
    -DMODEL=bypass \
    -DNUMST=1 \
    -DNCSTATES=0 \
    -DHAVESTDIO \
    -DTERMFCN=1 \
    -DONESTEPFCN=1 \
    -DMAT_FILE=0 \
    -DMULTI_INSTANCE_CODE=0 \
    -DINTEGER_CODE=0 \
    -DMT=0 \
    -DCLASSIC_INTERFACE=0 \
    -DALLOCATIONFCN=0 \
    -ljack -lasound -lpthread -lm -lrt -O2
```

## 5. Ejecutar

```bash
./bypass
```

## ⚠️ Puntos Críticos

1. **`simstruc.h`**: Las S-Functions `pisound_in/out.c` incluyen `simstruc.h`. Como no tenemos MATLAB en la Pi, necesitamos el `fake_simstruc.h` (y renombrarlo a `simstruc.h`) O modificar los .c para no incluirlo si definimos una macro.
2. **Conexión de señales**: El código generado por Simulink (`bypass.c`) llama a las S-Functions. Necesitamos asegurarnos de que la interfaz entre `ert_main.c` y las S-Functions sea correcta.

---

### Solución Recomendada para `simstruc.h`

En la Raspberry Pi:
```bash
# Crear fake simstruc
nano simstruc.h
# (Pegar el contenido de arriba)
```
