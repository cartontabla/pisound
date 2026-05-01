## Estado actual

Fuente de verdad operativa:

- `common/stream/CANONICAL_RUNTIME.md`

Archivo legacy/prototipo:

- `common/stream/para-borrar/`

- Nombres canonicos del enlace:
  - Mac -> Pi -> `Pisound_In`
  - Pi -> Mac -> `Pisound_Out`
- La Pi entra al modelo por `stream_in` y sale por `stream_out`.
- `aes67-linux-daemon + RAVENNA-ALSA` queda integrado como borde de
  red/driver ALSA de la Pi, no dentro del modelo.

## Prueba ya validada

- Tono de `1 kHz`, `3 s`, enviado desde el Mac con `RAVENNAKIT`.
- Captura en la Pi sobre `plughw:CARD=RAVENNA,DEV=0`.
- WAV devuelto por `scp` al Mac y reproducido localmente.
- En la segunda prueba el WAV capturado tuvo audio real:
  - `peak ~= 0.2`
  - `48000 Hz`
  - `2 canales`

## Hallazgo clave

- El problema no era Ethernet ni “nombre largo vs corto”.
- El bloqueo era el acoplamiento fragil del discovery al `session_name`.
- Tras fijar nombres canonicos y rehacer el `sink` desde SDP, desaparecieron:
  - `rtp_seq_id_error`
  - `rtp_ssrc_error`
  - `rtp_sac_error`
- En la rama `Mac -> Pi`, el bridge `RAVENNAKIT` mejora claramente cuando el
  emisor usa `ptime = 1 ms` / `framecount = 48` en vez del antiguo `128`
  frames. Esa pasa a ser la base buena actual del lado Mac.
- La mejor base actual para `Mac -> Pi` queda con:
  - `ptime = 1 ms`
  - `block-size = 128`
  - `tx_timestamp_offset_frames = 480`
  - el TX no arranca hasta que PTP esta calibrado
- El lado Mac ya tiene runtime operativo propio:
  - `run_ravennakit_bridge_mac.sh` para primer plano
  - `start_ravennakit_bridge_mac.sh` / `status_ravennakit_bridge_mac.sh` /
    `logs_ravennakit_bridge_mac.sh` /
    `wait_ravennakit_bridge_mac.sh` /
    `stop_ravennakit_bridge_mac.sh` para gestion persistente
  - `ravennakit_bridge_mac.env` como fichero local de configuracion del host
- En la rama `Pi -> Mac`, el receptor del bridge ahora:
  - no consume ni reproduce nada hasta que PTP este calibrado;
  - resetea el cursor RTP al cambiar el estado de calibracion;
  - y vuelve a alinearse contra `now - rx_require_delay_frames` una vez el
    reloj queda estable.
- `start/stop/status` del bridge Mac ya detectan y limpian instancias
  huérfanas para no dejar procesos fuera del `pid file`.
- La prueba DAW completa con `bypass2_app` ha quedado limpia.
- Hallazgo final del backend Pi:
  - no era un problema de `hw` vs `plughw`;
  - no era un `xrun` clasico (`xruns=0`);
  - no era una perdida de lock en el bridge Mac (`ptp=1 realigned=0`);
  - el problema era acoplar periodos ALSA fisicos de `48` con bloques logicos
    de `128` dentro de `stream_backend`;
  - la solucion buena ha sido leer y escribir en bloques logicos de `128`.
- Validacion final:
  - `bypass2_app` ha permanecido limpio durante muchos minutos;
  - se ha parado y relanzado y ha seguido limpio;
  - la rama queda consolidada como estable.
- Validacion híbrida:
  - `bypasscross` ya compila sobre `common/pisound_main.c`;
  - `pisound_in -> stream_out` funciona y llega al Mac/DAW;
  - `stream_in -> pisound_out` funciona y llega a la salida Pisound.

## Estado persistente recomendado en la Pi

- `aes67-daemon.service` activo
- `pisound-aes67-sessions.service` activo
- `/etc/default/pisound-aes67-sessions` con:
  - `PISOUND_AES67_TX_NAME=Pisound_In`
  - `PISOUND_AES67_RX_NAME=Pisound_Out`
- `pisound-stream-ravenna@<proyecto>.service` para correr modelos reales sobre
  `RAVENNA`
- `pisound-aes67-bridge.service` deshabilitado en la ruta canónica `RAVENNA`
  para no mezclar el bridge directo con `Pisound_Out`
- `pisound-aes67-runtime.target` agrupa el runtime obligatorio:
  `aes67-daemon`, `ptp4l`, `phc2sys`, `pisound-ravenna-http`,
  `pisound-ravenna-rtsp` y `pisound-aes67-sessions`

## Siguiente paso recomendado

- Dejar este hallazgo reflejado en cualquier proyecto `stream_in/out` que use
  `RAVENNA` a `48 kHz` y `N=128`.
- Mantener la secuencia canónica:
  `start_ravennakit_bridge_mac.sh -> wait_ravennakit_bridge_mac.sh -> ./xxx_app`
