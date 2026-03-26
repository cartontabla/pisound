# Guía Práctica 1: Creación de un Entorno de Audio en Tiempo Real con Raspberry Pi y Simulink

Esta guía te llevará paso a paso desde cero absoluto hasta tener un sistema de procesamiento de audio en tiempo real funcionando. Crearemos un proyecto "Bypass" (lo que entra, sale) diseñado en Simulink y ejecutado en una Raspberry Pi con tarjeta Pisound.

## 🎯 Objetivos de la Práctica
1.  Configurar el sistema operativo **Patchbox OS** en la Raspberry Pi.
2.  Entender el flujo de trabajo **Host-Target** (Diseño en PC/Mac -> Ejecución en RPi).
3.  Implementar un modelo básico de "Bypass" de audio usando **S-Functions**.
4.  Compilar y ejecutar el código generado en la Raspberry Pi.

---

## 🛠️ Requisitos Previos

### Hardware
*   **Raspberry Pi 5** (o 4/3B+).
*   **Pisound**: Tarjeta de sonido HAT para Raspberry Pi.
*   **Micro SD**: Mínimo 16GB.
*   **Fuente de alimentación** para RPi.
*   **Cable Ethernet** o conectividad WiFi.

### Software
*   **MATLAB & Simulink** (con Simulink Coder/Embedded Coder instalado).
*   **Balena Etcher**: Para flashear la tarjeta SD.
*   **Patchbox OS Image**: Descargada desde [blokas.io](https://blokas.io/patchbox-os/).

### 🔌 Conexionado Físico Recomendado
La tarjeta Pisound tiene niveles de línea. Aunque puede mover auriculares, lo ideal es usarla conectada a un sistema de amplificación.

**Opción A: Laboratorio / Estudio (Recomendado)**
1.  **Entrada**: Tu fuente de sonido (Móvil/PC) $\rightarrow$ Adaptador Jack Estéreo 6.35mm.
2.  **Salida**: Cables **Jack Estéreo a Doble Jack Mono** $\rightarrow$ Mesa de mezclas o Altavoces Activos.

**Opción B: Práctica Rápida (Auriculares)**
Si no tienes altavoces a mano, puedes conectar auriculares directamente, pero ten en cuenta que la salida está diseñada para "Línea" (Line Level), por lo que el volumen o la respuesta en graves podría no ser óptima en auriculares de alta impedancia.
*   **Adaptador necesario**: Jack 6.35mm Macho $\rightarrow$ Mini-jack 3.5mm Hembra.

*Nota: Usa el potenciómetro de OUTPUT en la Pisound para ajustar el nivel.*

---

## PASO 1: Preparación de la Raspberry Pi (Patchbox OS)

El sistema operativo es el "suelo" donde pisará nuestro plugin. Usaremos Patchbox OS, optimizado para audio.

1.  **Flashear la SD**:
    *   Abre **Balena Etcher**.
    *   Selecciona la imagen de Patchbox OS descargada.
    *   Selecciona tu tarjeta SD y dale a "Flash".

2.  **Primer Inicio (Setup Wizard)**:
    *   Inserta la SD en la Pi y enciéndela.
    *   Conéctate a la red WiFi que crea la Pi (`Patchbox` / password `blokaslabs`) O conéctala por cable Ethernet a tu router.
    *   Abre una terminal en tu ordenador y conecta por SSH: 
        `ssh patch@patchbox.local` (password: `blokas`)
    *   Se iniciará un asistente (Wizard). **PASOS CLAVE**:
        *   **Boot to Console**: Sí (Ahorra recursos gráficos).
        *   **Pisound Hat**: Selecciona "Yes" para activar los drivers.
        *   **Jack Service**: Instalarlo/Activarlo. Configura:
            *   Driver: `ALSA`
            *   Device: `hw:Pisound`
            *   Sample Rate: `48000`
            *   Buffer Size: `128` (Menor latencia) o `256` (Más seguro).
            *   Period: `2`.

3.  **Instalar Compiladores y Librerías**:
    Necesitamos herramientas para "construir" nuestro plugin en la Pi. Ejecuta en la terminal de la Pi:
    ```bash
    sudo apt update
    sudo apt install build-essential git cmake libjack-jackd2-dev libasound2-dev
    ```

---

## PASO 2: Estructura del Proyecto y Directorio `common`

Para mantener el orden y facilitar la reutilización, organizaremos nuestros archivos en una estructura jerárquica. Es fundamental entender esto antes de empezar.

### La Estructura de Directorios
Tu carpeta de trabajo `pisound` debería verse así:

```text
pisound/
├── common/           <-- ARCHIVOS COMUNES (La caja de herramientas)
│   ├── pisound_in.c
│   ├── pisound_out.c
│   ├── pisound_library.slx
│   ├── simstruc.h    <-- Headers necesarios para compilar sin MATLAB
│   └── ...
└── bypass1/          <-- NUESTRO PROYECTO
    ├── bypass.slx
    └── ...
```

### ¿Qué hay en `common` y para qué sirve?
Este directorio contiene todo lo necesario para que tus modelos de Simulink cobren vida en la Raspberry Pi sin tener que instalar MATLAB en ella.

1.  **`pisound_in.c` / `pisound_out.c` (Interfases de Audio)**:
    Son los puentes. `pisound_in.c` recoge el audio de la tarjeta y lo empaqueta para que el código C generado por Simulink lo entienda. `pisound_out.c` hace lo inverso.
    
    > **Nota sobre los puertos**:
    > Verás que estos bloques tienen 4 conexiones:
    > *   **1 y 2 (Audio L/R)**: Las señales de audio izquierda y derecha.
    > *   **3 (MIDI Bytes)**: Un vector que contiene mensajes MIDI entrantes (notas, control, reloj).
    > *   **4 (MIDI Count)**: Indica cuántos mensajes MIDI válidos han llegado en ese instante.
    > *Aunque en este proyecto "Bypass" no los usemos, ¡la estructura está lista para procesar sintetizadores o efectos controlados por teclado!*

2.  **`simstruc.h`, `fixedpoint.h`, etc. (Headers de Simulink)**:
    El código C que genera Simulink (`bypass.c`) hace referencia a funciones internas de MATLAB. Normalmente necesitarías copiar gigabytes de la instalación de MATLAB a la Pi.
    *   **El Truco**: Hemos puesto versiones ligeras o "dummy" de estos archivos en `common`. Así engañamos al compilador de la Pi para que crea que tiene MATLAB instalado. ¡Ahorra mucho espacio y configuración!

3.  **`pisound_library.slx`**:
    Una librería de Simulink que ya contiene los bloques `pisound_in` y `pisound_out` pre-configurados. ¡Úsala para arrastrar y soltar en tus diseños!

---

## PASO 3: Diseño en MATLAB/Simulink (En tu ordenador Host)

1.  **Configurar el Path**:
    En MATLAB, añade la carpeta `common` al path para que encuentre los archivos S-Function y la librería.
    ```matlab
    addpath('../common');
    savepath; % Opcional, para guardar el cambio
    ```

2.  **Generar las S-Functions (MEX)**:
    Aunque el código C ya está en `common`, tu Mac necesita compilarlos para poder **simular** en Simulink (aunque no haya audio real en el Mac).
    ```matlab
    mex -I../common ../common/pisound_in.c
    mex -I../common ../common/pisound_out.c
    ```

3.  **Crear el Modelo (Automáticamente)**:
    En lugar de arrastrar bloques manualmente, ejecutaremos un script que ensambla el modelo y configura todos los parámetros de compilación por ti.
    
    *   Asegúrate de estar en la carpeta `pisound/bypass1`.
    *   Ejecuta en la consola de MATLAB:
        ```matlab
        create_bypass
        ```
    
    > **🧐 ¿Qué hace este script?**
    > Para los curiosos, aquí está el código que crea el modelo. Nota cómo configura `Solver`, `SystemTargetFile` y conecta los puertos automáticamente:
    > ```matlab
    > function create_bypass()
    > model = 'bypass';
    > if bdIsLoaded(model)
    >     close_system(model, 0);
    > end
    > new_system(model);
    > open_system(model);
    > % Añadir bloques
    > add_block('pisound_library/PiSound Input',[model '/pisound_in']);
    > add_block('pisound_library/PiSound Output',[model '/pisound_out']);
    > % Conectar puertos (1:Left, 2:Right, 3:MidiBytes, 4:MidiCount)
    > add_line(model,'pisound_in/1','pisound_out/1');
    > add_line(model,'pisound_in/2','pisound_out/2');
    > add_line(model,'pisound_in/3','pisound_out/3');
    > add_line(model,'pisound_in/4','pisound_out/4');
    > % Configuración para Code Generation
    > set_param(model,'SolverType','Fixed-step');
    > set_param(model,'FixedStep','auto');
    > set_param(model,'Solver','FixedStepDiscrete');
    > set_param(model,'SystemTargetFile','ert.tlc'); % Embedded Coder
    > set_param(model,'TargetLang','C');
    > set_param(model,'GenerateSampleERTMain','on');
    > save_system(model);
    > end
    > ```

    *   Esto abrirá `bypass.slx` con los bloques `pisound_in` y `pisound_out` ya conectados y configurados.

    > **💡 Tip Pro: Parámetros en Tiempo Real**
    > Por defecto, Simulink "congela" (inlines) los parámetros numéricos para optimizar. Si en futuros proyectos quieres cambiar ganancias o switches en tiempo real desde la Pi, ejecuta esto antes de compilar:
    > ```matlab
    > set_param('bypass', 'DefaultParameterBehavior', ...
    >           'Tunable');
    > slbuild('bypass');
    > ```
    > Esto hará que el código C exponga los parámetros en memoria RAM.

    ![Diagrama de Bloques Simulink - Bypass](/Users/linogarciamorales/.gemini/antigravity/brain/d1a55b89-b314-45af-84a0-0220d79e15a8/bypass_simulink_diagram_1765964190727.png)
    *Figura 1: El resultado será este esquema. El script conecta automáticamente las entradas y salidas.*

4.  **Generar Código C**:
    Igual que antes: `Ctrl+B`. Se generará la carpeta `bypass_ert_rtw` dentro de `bypass1`.

---

## PASO 4: Compilación y Ejecución (En Raspberry Pi Target)

Ahora movemos nuestro proyecto a la Pi. Asumiremos que copias toda la carpeta `pisound` (con `common` y `bypass1`) a la Pi, manteniendo la estructura.

1.  **Transferencia (De Mac $\rightarrow$ RPi)**:
    Hay dos tipos de transferencia según el momento:
    
    *   **A) Primera vez (SETUP)**: Necesitas copiar toda la estructura base, incluyendo la carpeta `common` que no cambia comúnmente.
        ```bash
        # Desde la terminal de tu Mac, estando en la carpeta raíz 'pisound':
        scp -r common patch@patchbox.local:~/pisound/
        scp -r bypass1 patch@patchbox.local:~/pisound/
        ```
    
    *   **B) En cada cambio (ITERACIÓN)**: Si modificas tu modelo en Simulink y regeneras código, **SOLO** necesitas actualizar la carpeta `_ert_rtw`. No hace falta volver a copiar `common` ni los archivos `.c` de la interfaz.
        ```bash
        # Solo actualiza el código generado (más rápido)
        scp -r bypass1/bypass_ert_rtw/* patch@patchbox.local:~/pisound/bypass1/bypass_ert_rtw/
        ```
        *Tip: Esto ahorra mucho tiempo en el ciclo de desarrollo.*

2.  **Compilación**:
    Vamos a la carpeta del código generado en la Pi:
    ```bash
    cd ~/pisound/bypass1/bypass_ert_rtw
    ```

    Usaremos `gcc` para enlazar:
    *   El código de tu modelo (`bypass.c`)
    *   El código de interfaz (`../common/pisound_in.c`)
    *   Los headers necesarios (`-I../common`)

    comando:
    \begin{fullwidth}
    \begin{verbatim}
    gcc -o bypass_app \
        bypass2.c \
        bypass2_data.c \
        rt_nonfinite.c \
        ../../common/custom_main.c \
        ../../common/pisound_in.c \
        ../../common/pisound_out.c \
        -I. -I.. -I../../common \
        -DMODEL_HEADER="\"bypass2.h\"" \
        -DMODEL_INITIALIZE=bypass2_initialize \
        -DMODEL_STEP=bypass2_step \
        -DMODEL_TERMINATE=bypass2_terminate \
        -DPARAM_STRUCT_TYPE=P_bypass2_T \
        -DPARAM_STRUCT_VAR=bypass2_P \
        -DPARAM_GAIN_MEMBER=MATLABSystem_Gain \
        -DPARAM_BYPASS_MEMBER=MATLABSystem_Bypass \
        -DPARAM_FS_MEMBER=MATLABSystem_Fs \
        -ljack -lasound -lpthread -lm -lrt -O3
    \end{verbatim}
    \end{fullwidth}
    
    *Nota: Estamos usando `custom_main.c` como el "cerebro" que conecta JACK con nuestro modelo (`bypass`). Los parámetros `-DMODEL_...` le dicen a ese cerebro qué funciones específicas debe llamar.*


3.  **¡Ejecución!**:
    ```bash
    ./bypass_app
    ```
    El programa se quedará "colgado" (running). Eso es bueno.

4.  **Conexión Virtual (Patchbay)**:
4.  **Verificar Conexiones y Evitar Conflictos**:
    Nuestra aplicación (`bypass_app`) se conecta automáticamente a las entradas y salidas de la Pisound al arrancar.

    **¡Importante!**: Es posible que el sistema tenga activo un servicio de monitorización (MODEP/mod-monitor) que conecta la entrada directamente a la salida. Esto provoca que **escuches la señal limpia mezclada con tu procesado** (o silencio si inviertes fase).

    Para evitarlo, abre otra terminal y verifica:
    ```bash
    # Ver conexiones
    jack_lsp -c
    ```
    
    Si ves que `system:playback_1` está conectado a `mod-monitor:out_1` (además de a tu `pisound_model`), **DESCONÉCTALO**:
    ```bash
    jack_disconnect "mod-monitor:out_1" "system:playback_1"
    jack_disconnect "mod-monitor:out_2" "system:playback_2"
    ```
    *Así garantizas que solo escuchas lo que sale de tu programa Simulink.*


¡Listo! Si hablas por el micro o reproduces audio por la entrada, debería salir por la salida. Has creado tu primer procesador de audio digital embebido.
