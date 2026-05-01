# Patchbox Bootstrap

Bootstrap recomendado para un Patchbox nuevo donde la Pi entra al modelo por
`stream_in` y sale por `stream_out`.

## Objetivo

Dejar esta arquitectura:

```text
Mac/host audio -> RAVENNAKIT -> Ethernet -> aes67-linux-daemon -> ALSA RAVENNA -> stream_in -> modelo -> stream_out -> ALSA RAVENNA -> aes67-linux-daemon -> Ethernet
```

## Convenciones canonicas

- Stream del Mac hacia la Pi: `Pisound_In`
- Stream de la Pi hacia el Mac: `Pisound_Out`

En la Pi:

- `Pisound_In` termina entrando por `stream_in`
- `Pisound_Out` sale desde `stream_out`
- `pisound-aes67-bridge.service` no forma parte de esta ruta canónica
  `RAVENNA`; déjalo deshabilitado salvo pruebas explícitas del bridge directo.

## Despliegue base

Sincroniza el repo y ejecuta:

```bash
cd ~/pisound
sudo common/scripts/deploy_aes67.sh
```

Eso prepara los paquetes de sistema necesarios y deja instalados los servicios
del framework. Incluye compilador, ALSA, JACK, Python 3, DBus/GObject para BLE
MIDI, BlueZ, linuxptp y Avahi. Si estas reconfigurando una Pi sin instalar
paquetes, usa `--skip-apt`.

El despliegue instala:

- `pisound-aes67-runtime.target`
- `ptp4l-aes67.service`
- `phc2sys-aes67.service`
- `pisound-ravenna-http.service`
- `pisound-ravenna-rtsp.service`
- `pisound-aes67-sessions.service`
- `pisound-stream-ravenna@.service`
- `/etc/default/pisound-aes67`
- `/etc/default/pisound-aes67-sessions`

Si quieres validar o reparar requisitos sin hacer todo el despliegue:

```bash
sudo common/scripts/check_pi_requirements.sh --install \
  --project <proyecto> \
  --flags <proyecto>/<proyecto>_compile_flags.txt \
  --mode stream
```

Usa `--mode pisound` para proyectos JACK/Pisound puros y `--mode hybrid` para
proyectos que combinen Pisound, BLE MIDI y streaming.

## Configuracion persistente

Revisa estos archivos:

- `/etc/default/pisound-aes67`
- `/etc/default/pisound-aes67-sessions`

Valores esperados en sesiones:

```text
PISOUND_AES67_TX_NAME=Pisound_In
PISOUND_AES67_RX_NAME=Pisound_Out
PISOUND_AES67_PAYLOAD_TYPE=98
PISOUND_AES67_CHANNELS=2
PISOUND_AES67_MAX_SAMPLES_PER_PACKET=128
```

## Modo de ejecucion del proyecto DSP

Para la ruta final de streaming sobre la tarjeta `RAVENNA`, usa:

```bash
common/scripts/compile.sh <proyecto>
sudo common/scripts/start_ravenna_project.sh <proyecto>
```

Antes de invocar `gcc`, `compile.sh` ejecuta una comprobacion de requisitos en
la Pi segun los flags reales del proyecto. Si falta algo como `python3`,
`python3-dbus`, `linuxptp`, `libasound2-dev` o el runtime AES67, falla con una
accion concreta para instalarlo en vez de dejar un error opaco durante la
compilacion o la ejecucion.

Si quieres modo interactivo en primer plano, para salir con `q` o `Ctrl-C`:

```bash
common/scripts/compile.sh <proyecto>
sudo common/scripts/run_ravenna_project_foreground.sh <proyecto>
```

Para pararlo de forma simétrica:

```bash
sudo common/scripts/stop_ravenna_project.sh <proyecto>
```

Equivale a:

```bash
common/scripts/compile.sh <proyecto>
sudo common/scripts/start_ravenna_project.sh <proyecto>
```

## Verificacion minima

```bash
systemctl is-active aes67-daemon.service \
  pisound-aes67-runtime.target \
  ptp4l-aes67.service \
  phc2sys-aes67.service \
  pisound-ravenna-http.service \
  pisound-ravenna-rtsp.service \
  pisound-aes67-sessions.service
```

Comprueba ademas:

```bash
curl -fsS http://127.0.0.1:8090/api/ptp/status
curl -fsS http://127.0.0.1:8090/api/streams
```

La tarjeta debe existir como ALSA:

```bash
arecord -l
arecord -L | grep -E 'RAVENNA|plughw:CARD=RAVENNA|dsnoop:CARD=RAVENNA'
```

## Prueba funcional recomendada

Desde el Mac:

- levantar `common/stream/start_ravennakit_bridge_mac.sh`
- comprobar `common/stream/status_ravennakit_bridge_mac.sh`
- emitir un tono de `1000 Hz` durante `3 s` a `BlackHole 16ch` canales `1-2`

En la Pi:

- grabar `plughw:CARD=RAVENNA,DEV=0`

Si la captura tiene señal real, la integracion `RAVENNAKIT <-> aes67-linux-daemon + ALSA`
esta operativa y ya se puede pasar a validar el modelo real con
`stream_in/out`.

## Flags del proyecto

Para cualquier proyecto `xxx` que use `stream_in.c` o `stream_out.c`, el
archivo `xxx_compile_flags.txt` debe ser la unica fuente de verdad para el
backend de compilacion.

Para la ruta canonica sobre `RAVENNA`, lo esperado es:

```text
-DSTREAM_MODE
-DPORTABLE_WORDSIZES
-DSTREAM_BACKEND_DEFAULT_ALSA=1
```

El backend `UDP` queda solo para pruebas auxiliares y ya no debe ser el
default de proyectos como `bypass2`.

## Ejecucion directa del binario

Para proyectos `stream` con backend ALSA por defecto, `compile.sh` deja el
`<proyecto>_app` como wrapper del binario real, igual que ocurre con otros
casos como BLE. Ese wrapper:

- levanta `pisound-aes67-runtime.target` al entrar si hace falta
- exporta el entorno `STREAM_*` canonico de `RAVENNA`
- tumba el runtime al salir si fue esa misma instancia quien lo encendio

Ejemplo interactivo limpio:

```bash
common/scripts/compile.sh bypass2
./bypass2_app
```

## Runtime del Mac

El backend del host no deberia quedar como una orden manual dificil de recordar.
La operativa recomendada es:

```bash
common/stream/start_ravennakit_bridge_mac.sh
common/stream/wait_ravennakit_bridge_mac.sh
common/stream/status_ravennakit_bridge_mac.sh
common/stream/logs_ravennakit_bridge_mac.sh
common/stream/stop_ravennakit_bridge_mac.sh
```

Y para la prueba de oro con `bypass2`:

1. En Mac:
   `common/stream/start_ravennakit_bridge_mac.sh`
   `common/stream/wait_ravennakit_bridge_mac.sh`
2. En la Pi:
   `./bypass2_app`
3. En la DAW:
   enviar por `BlackHole 1-2` y escuchar por `BlackHole 3-4`

## Hallazgo clave de estabilidad

En la Pi, para `bypass2` y cualquier otro proyecto `stream_in/out` con
`MODEL_FRAME_LENGTH=128`, la estabilidad no dependio de cambiar `hw` por
`plughw` ni de perseguir `xruns`.

La mejora real vino de esto:

- el PCM `RAVENNA` expone periodos fisicos de `48` frames;
- el modelo trabaja en bloques logicos de `128` frames;
- `stream_backend` debe leer y escribir en bloques logicos de `128`, dejando a
  ALSA resolver el detalle del periodo fisico.

Si el backend vuelve a trocear internamente a `48`, puede aparecer audio roto
en `bypass2` aunque:

- el bridge Mac siga `ptp=1`;
- `realigned=0`;
- `xruns=0`.

Si hace falta fijar parametros del host, copia:

```text
common/stream/ravennakit_bridge_mac.env.example
```

a:

```text
common/stream/ravennakit_bridge_mac.env
```

y ajusta ahi `NETWORK_INTERFACE`, `TX/RX` puertos, sesiones y offsets.
