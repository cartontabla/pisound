# PiSound

Repositorio de ejemplos y runtime para el libro **Procesamiento de audio digital en tiempo real**.

Este proyecto agrupa modelos de Simulink, codigo C generado, utilidades comunes y scripts para compilar y ejecutar procesadores de audio en Raspberry Pi con Patchbox OS y PiSound. Algunos proyectos usan JACK/PiSound y otros pueden usar el modo de streaming AES67/RAVENNA.

## Primer comando de uso

Desde la raiz del repositorio, en la Raspberry Pi/Patchbox:

```bash
sudo common/scripts/prepare_project.sh --compile amp2
```

Este comando comprueba los requisitos del proyecto `amp2`, instala dependencias si faltan, despliega el soporte AES67/RAVENNA cuando el proyecto lo necesita y compila el ejecutable `amp2/amp2_app`.

Para comprobar requisitos sin instalar ni compilar:

```bash
common/scripts/prepare_project.sh --check-only amp2
```

## Requisitos

- Raspberry Pi con Patchbox OS o un sistema Linux equivalente.
- PiSound configurado si se trabaja en modo JACK/PiSound.
- Codigo C generado por Simulink Coder dentro de cada carpeta `<proyecto>/<proyecto>_ert_rtw/`.
- Archivo de configuracion de compilacion por proyecto: `<proyecto>/<proyecto>_compile_flags.txt`.
- Herramientas de compilacion C: `gcc` y `build-essential`.
- Librerias de audio:
  - `libjack-jackd2-dev` y `jackd2` para modo JACK/PiSound.
  - `libasound2-dev` y `alsa-utils` para ALSA.
- Para proyectos AES67/RAVENNA: `linuxptp`, `systemd` y los servicios desplegados con `common/scripts/deploy_aes67.sh`.
- Para control MIDI/BLE/OSC, las dependencias adicionales se infieren desde las banderas del archivo `<proyecto>_compile_flags.txt`.

Los scripts de preparacion y compilacion usan el archivo de flags de cada proyecto como fuente principal para decidir que dependencias y modo de ejecucion necesita.

## Estructura de carpetas

- `amp/`, `amp2/`, `bypass/`, `delay/`, `dist/`, `doubling/`, `dyn/`, `eq/`, `filters/`, `leveller/`, `osc/`, `phaser/`, `pitch/`, `reverb/`, `ring-modulator/`, `tremolo/`, `vocoder/`, `wah/`: proyectos de audio. Normalmente contienen el modelo `.slx`, el codigo generado `<proyecto>_ert_rtw/` y el archivo `<proyecto>_compile_flags.txt`.
- `common/`: runtime compartido para entrada/salida de audio, control, streaming, cabeceras comunes y scripts de compilacion/despliegue.
- `common/scripts/`: utilidades principales (`prepare_project.sh`, `compile.sh`, `check_pi_requirements.sh`, scripts JACK y scripts AES67/RAVENNA).
- `common/control/`: soporte de control MIDI, OSC, UDP y BLE MIDI.
- `common/stream/`: runtime y servicios para streaming AES67/RAVENNA.
- `codes/`: scripts MATLAB de pruebas, diseno, diagnostico y generacion.
- `templates/`: plantillas de codigo, imagenes y GUI para nuevos proyectos.
- `signals/`: senales de audio de prueba.
- `docs/`, `auxs/`, `measurements/`: documentacion auxiliar, guias, mediciones y material de referencia.
- `touchosc/`: plantilla de control TouchOSC.
- `gain2x2/` y carpetas `lv2/`: ejemplos y recursos para plugins LV2/MOD.

## Flujo basico

1. Generar o actualizar el codigo C del modelo desde Simulink.
2. Revisar el archivo `<proyecto>/<proyecto>_compile_flags.txt`.
3. Preparar y compilar en la Raspberry Pi:

```bash
sudo common/scripts/prepare_project.sh --compile <proyecto>
```

4. Ejecutar el binario generado:

```bash
./<proyecto>/<proyecto>_app
```

Ejemplo:

```bash
./amp2/amp2_app
```
