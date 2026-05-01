# Canonical AES67/RAVENNA Runtime

Este archivo es la fuente de verdad operativa para la ruta que ya funciona.
Si hay contradiccion con notas antiguas o prototipos bajo `common/stream`,
manda este documento.

Estado de limpieza:

- todo lo legacy/prototipo retirado de la ruta viva se ha movido a
  `common/stream/para-borrar/`;
- ese subdirectorio ya no forma parte del funcionamiento canónico.

## Objetivo

Dejar funcionando de forma estable esta ruta:

```text
DAW -> BlackHole 1-2 -> RAVENNAKIT bridge (Mac) -> ETH ->
aes67-linux-daemon + RAVENNA-ALSA (Pi) -> stream_in ->
modelo -> stream_out -> aes67-linux-daemon + RAVENNA-ALSA ->
ETH -> RAVENNAKIT bridge (Mac) -> BlackHole 3-4
```

## Nombres canonicos

- Mac -> Pi: `Pisound_In`
- Pi -> Mac: `Pisound_Out`

## Pieza canónica por lado

### Mac

Bridge real:

- `build/ravennakit_bridge/pisound_ravennakit_bridge`

Wrappers operativos:

- `common/stream/run_ravennakit_bridge_mac.sh`
- `common/stream/start_ravennakit_bridge_mac.sh`
- `common/stream/wait_ravennakit_bridge_mac.sh`
- `common/stream/status_ravennakit_bridge_mac.sh`
- `common/stream/logs_ravennakit_bridge_mac.sh`
- `common/stream/stop_ravennakit_bridge_mac.sh`

Controlador único del host:

- `common/scripts/bridgectl.py`

Frontend mínimo de escritorio:

- `common/scripts/ravennakit_bridge_web.py`
- `common/scripts/open_ravennakit_bridge_app.sh`

Wrappers equivalentes bajo `common/scripts`:

- `common/scripts/start_ravennakit_bridge_host.sh`
- `common/scripts/wait_ravennakit_bridge_host.sh`
- `common/scripts/status_ravennakit_bridge_host.sh`
- `common/scripts/logs_ravennakit_bridge_host.sh`
- `common/scripts/stop_ravennakit_bridge_host.sh`

Configuracion persistente del host:

- `common/stream/ravennakit_bridge_mac.env`
- plantilla: `common/stream/ravennakit_bridge_mac.env.example`

### Pi

Runtime AES67/RAVENNA:

- `aes67-daemon.service`
- `pisound-aes67-runtime.target`
- `pisound-aes67-sessions.service`
- `pisound-ravenna-http.service`
- `pisound-ravenna-rtsp.service`

Proyecto DSP sobre `stream_in/out`:

- `common/scripts/compile.sh <project>`
- `./<project>_app`

Servicio canónico de proyecto:

- `pisound-stream-ravenna@<project>.service`

No canónico para esta ruta:

- `pisound-aes67-bridge.service`
- `pisound-stream-app@.service`

## Parametros que deben quedar así

### Mac bridge

- audio input device: `BlackHole 16ch`
- audio output device: `BlackHole 16ch`
- input channels: `1-2`
- output channels: `3-4`
- network interface: `en0`
- tx destination: `239.69.0.2:5004`
- rx port: `5004`
- sample rate: `48000`
- block size: `128`
- channels: `2`
- payload type: `98`
- tx session: `Pisound_In`
- rx session: `Pisound_Out`
- `ptime = 1 ms`
- `tx_timestamp_offset_frames = 480`
- `rx_require_delay_frames = 1024`

### Pi runtime

- sample rate del stream: `48000`
- canales: `2`
- payload type: `98`
- `PISOUND_AES67_TX_NAME=Pisound_In`
- `PISOUND_AES67_RX_NAME=Pisound_Out`

### Pi model / stream backend

- `MODEL_FRAME_LENGTH = 128`
- backend ALSA por defecto
- dispositivo `RAVENNA` para capture/playback
- la app visible debe ser `./<project>_app`

## Hallazgo clave que NO se debe perder

El fallo importante de `bypass2` no era:

- ni un `xrun` clásico;
- ni `hw` vs `plughw`;
- ni pérdida de lock del bridge Mac;
- ni PTP cayéndose en caliente.

El problema real era este:

- ALSA `RAVENNA` expone periodos físicos de `48` frames;
- el modelo trabaja en bloques lógicos de `128` frames;
- si `stream_backend` trocea y acopla internamente al periodo físico de `48`,
  el bypass simultáneo puede sonar roto aunque:
  - `xruns=0`
  - `ptp=1`
  - `realigned=0`
  - `in_peak == out_peak`

La solución buena ha sido:

- hacer que `stream_backend` lea y escriba en bloques lógicos de `128`;
- dejar a ALSA resolver por debajo el detalle del periodo físico de `48`.

Eso es vital para cualquier proyecto `stream_in/out` que use esta ruta a:

- `48 kHz`
- `2 canales`
- `N = 128`

## Orden canónico de uso

### Arranque en Mac

```bash
common/scripts/bridgectl.py start
common/scripts/bridgectl.py wait
common/scripts/bridgectl.py status
```

o bien, si prefieres abrir la app:

```bash
common/scripts/bridgectl.py gui
```

### Arranque en la Pi

Separar compilación y ejecución:

```bash
common/scripts/compile.sh bypass2
cd ~/pisound/bypass2
./bypass2_app
```

### DAW

- enviar por `BlackHole 1-2`
- escuchar solo por `BlackHole 3-4`

## Qué mirar si algo falla

### Mac

```bash
common/scripts/bridgectl.py status
common/scripts/bridgectl.py logs 120
```

Señales sanas del bridge:

- `Ready: PTP calibrated`
- `ptp=1`
- `realigned=0`
- ausencia de `miss=1`

### Pi

`bypass2_app` debe arrancar silencioso por defecto. Solo usa trazas si quieres diagnosticar algo:

```bash
STREAM_VERBOSE=1 ./bypass2_app
STREAM_DEBUG_STATUS=1 STREAM_DEBUG_PEAKS=1 ./bypass2_app
STREAM_DEBUG_DRIFT=1 ./bypass2_app
```

Uso recomendado para diagnóstico fino:

```bash
STREAM_DEBUG_STATUS=1 STREAM_DEBUG_PEAKS=1 ./bypass2_app
```

Qué sería sano:

- `xruns=0`
- `queued/delay` estables
- `in_peak` y `out_peak` coherentes

Qué ya quedó validado:

- `bypass2_app` puede sonar limpio durante muchos minutos;
- parar y volver a arrancar `bypass2_app` mantiene el flujo limpio;
- la estabilidad buena ya no depende de “acertar” el arranque por azar.
- `bypasscross_app` compila y corre como proyecto híbrido;
- `pisound_in -> stream_out` llega al Mac/DAW;
- `stream_in -> pisound_out` llega a la salida Pisound.

## Archivos que forman la ruta viva

### Canónicos

- `common/stream/CANONICAL_RUNTIME.md`
- `common/stream/README.md`
- `common/stream/SESSION.md`
- `common/scripts/bridgectl.py`
- `common/scripts/ravennakit_bridge_app.py`
- `common/pisound_main.c` para proyectos híbridos `pisound_* + stream_*`
- `common/stream/PATCHBOX_BOOTSTRAP.md`
- `common/stream/ravennakit_bridge/`
- `common/stream/stream_backend.c`
- `common/stream/stream_backend.h`
- `common/stream/stream_contract.c`
- `common/stream/stream_contract.h`
- `common/stream/run_ravennakit_bridge_mac.sh`
- `common/stream/start_ravennakit_bridge_mac.sh`
- `common/stream/wait_ravennakit_bridge_mac.sh`
- `common/stream/status_ravennakit_bridge_mac.sh`
- `common/stream/logs_ravennakit_bridge_mac.sh`
- `common/stream/stop_ravennakit_bridge_mac.sh`
- `common/stream/ravennakit_bridge_mac.env.example`
- `common/stream/pisound-aes67-runtime.target`
- `common/stream/pisound-aes67-sessions.service`
- `common/stream/pisound-ravenna-rtsp.service`
- `common/stream/pisound-stream-ravenna@.service`
- `common/stream/pisound_aes67_sessions.py`
- `common/stream/pisound_ravenna_rtsp_server.py`

### Soporte / despliegue

- `common/stream/.runtime/` (generado en ejecución)
- `common/stream/ptp4l-aes67.conf`
- `common/stream/ptp4l-aes67.service`
- `common/stream/ptp4l-aes67.sh`
- `common/stream/phc2sys-aes67.service`
- `common/stream/phc2sys-aes67.sh`
- `common/stream/pisound-aes67.env.example`
- `common/stream/pisound-aes67-sessions.env.example`
- `common/stream/pisound-ravenna-rtsp.service.xml`
- `common/stream/pisound-ravenna.service.xml`
- `common/stream/ravenna-http.service`
- `common/stream/ravenna-index.html`
- `common/stream/stream_project_compile_flags.example.txt`
- `common/stream/stream_project_ravenna.env.example`
- `common/stream/stream_contract.py`
- `common/stream/99-pisound-ptp-permissions.rules.example`
- `common/stream/setup_pi.sh`
- `common/stream/fix_pi_aes67_daemon_service.sh`

### Legacy / prototipo / no canónico

Todo lo archivado bajo:

- `common/stream/para-borrar/`

ya no forma parte de la ruta viva validada y no debe considerarse necesario
para que el sistema funcione.
