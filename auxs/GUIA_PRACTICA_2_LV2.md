# Guía Práctica 2: Convirtiendo Simulink en un Plugin LV2

En la guía anterior creamos una aplicación "Standalone" (un ejecutable `bypass_app`) que toma el control del audio. Pero, ¿y si queremos usar nuestro algoritmo dentro de un DAW, o en un pedalboard virtual como MODEP?

Aquí es donde entra **LV2** (LADSPA Version 2), el estándar de plugins de audio abierto para Linux. En esta práctica, encapsularemos nuestro modelo `bypass2` dentro de un contenedor LV2.

---

## 📂 1. La Estructura del Plugin LV2

Dentro de tu carpeta `bypass2`, encontrarás un subdirectorio llamado `LV2`. Este contiene los archivos necesarios para definir el plugin ante el sistema.

```text
bypass2/
├── bypass2.slx         (Tu modelo Simulink)
├── bypass2_ert_rtw/    (Código C generado por Simulink)
└── LV2/                (Archivos del Wrapper LV2)
    ├── bypass2.ttl     <- Definición del plugin (Puertos, Nombre)
    ├── manifest.ttl    <- DNI del plugin (URI -> Binario)
    ├── bypass2_lv2.c   <- El código C que une LV2 con Simulink
    └── Makefile.lv2    <- Receta de compilación
```

### 1.1 `manifest.ttl`
Es el archivo de índice. Le dice al sistema: "Existe un plugin con este URI y sus datos están en `bypass2.ttl`".

### 1.2 `bypass2.ttl`
Define la "cara" del plugin:
*   **Nombre**: "Bypass2 (Simulink)".
*   **Puertos de Audio**: Entrada L/R, Salida L/R.
*   **Parámetros de Control**:
    *   `Gain` (Ganancia): De 0.0 a 2.0.
    *   `Bypass` (Switch): 0 (Off) o 1 (On).

> **Importante**: Los índices de los puertos en este archivo (index 0, 1, 2...) deben coincidir EXACTAMENTE con el `enum` en el código C.

### 1.3 `bypass2_lv2.c` (El Wrapper)
Este archivo C es el "traductor". Simulink no sabe qué es LV2, y LV2 no sabe qué es Simulink.
*   En `instantiate()`: Inicializa el modelo (`bypass2_initialize()`).
*   En `run()`: Se ejecuta cada buffer de audio.
    1.  Copia los valores de los controles LV2 a las variables globales del modelo Simulink (`bypass2_DW.obj.Gain`, etc.).
    2.  Procesa el audio sample a sample invocando la lógica del modelo.

---

## 🛠️ 2. Compilación en la Raspberry Pi

Asumiendo que ya has transferido los archivos a la Pi (carpeta `pisound/bypass2`), el proceso es muy sencillo gracias al `Makefile.lv2`.

1.  Conecta por SSH a tu Raspberry Pi.
2.  Navega al directorio LV2:
    ```bash
    cd ~/pisound/bypass2/LV2
    ```
3.  **Compilar**:
    ```bash
    make -f Makefile.lv2
    ```
    *Esto generará una carpeta `bypass2.lv2` con el binario compilado `bypass2.so` y los archivos TTL.*

4.  **Instalar**:
    ```bash
    make -f Makefile.lv2 install
    ```
    *Esto copiará la carpeta generada a `~/.lv2/`, donde el sistema busca los plugins de usuario.*

---

## 🎛️ 3. Probando el Plugin (Jalv)

No necesitas un DAW complejo para verificar si funciona. Usaremos `jalv`, un host ligero de LV2 para terminal.

1.  **Lanzar el plugin**:
    ```bash
    jalv.gtk3 http://pisound.org/plugins/bypass2
    ```
    *   Si usas la versión `gtk3`, se abrirá una ventanita gráfica genérica con los faders de Gain y el checkbox de Bypass.
    *   Si usas `jalv` (sólo texto), verás la salida en terminal.

2.  **Conectar Audio**:
    Al igual que en la práctica anterior, usa `jack_lsp` o un gestor gráfico (si tienes VNC) para conectar las entradas y salidas de la Pisound a tu plugin `jalv`.

    ```bash
    # Ejemplo manual (si no se auto-conecta)
    jack_connect system:capture_1 bypass2:Audio_In_L
    jack_connect bypass2:Audio_Out_L system:playback_1
    ```

3.  **¡Prueba de Sonido!**
    *   Reproduce audio.
    *   Mueve el fader de **Gain** en la ventana de Jalv. Deberías escuchar el cambio de volumen.
    *   Activa el **Bypass**. La señal debería pasar sin cambios (y sin ganancia).

---

## 🌍 Nota: ¿Funciona en Mac y Windows?

Es común preguntarse: *¿Si LV2 es un estándar asociado a Linux, puedo usar este plugin en mi Mac o PC?*

La respuesta tiene dos matices:

1.  **El Estándar (Sí)**: LV2 es multiplataforma. DAWs como **Reaper**, **Ardour** o **Audacity** soportan plugins LV2 en macOS y Windows.
2.  **El Plugin Compilado (No)**: El archivo binario `.so` que acabamos de crear en la Raspberry Pi es específico para su procesador (ARM) y su sistema operativo (Linux). No funcionará si lo copias directamente a tu ordenador.

Para usar tu plugin "Bypass" en tu Mac o Windows, tendrías que **recompilar** este mismo código C en tu ordenador. Al ser código C estándar, ¡es totalmente posible!

---

## 🚀 Siguientes Pasos
Ahora que tienes tu modelo como plugin LV2, podrías cargarlo en **MODEP** (la pedalera virtual de Patchbox OS) y usarlo junto a efectos de guitarra, reverbs y sintetizadores profesionales, todo controlado desde una interfaz web.
