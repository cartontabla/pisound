# `common/stream`

Contrato comun para transportar audio PCM entre la frontera `stream_in/out` del
modelo y el backend de streaming.

Fuente de verdad operativa:

- `common/stream/CANONICAL_RUNTIME.md`

Si otra nota antigua o helper experimental contradice ese archivo, manda
`CANONICAL_RUNTIME.md`.

Archivo legacy/prototipo movido fuera de la ruta viva:

- `common/stream/para-borrar/`

## Estado Resumido

La arquitectura real que queda en este repo es esta:

- Mac: bridge `audio-device <-> ETH` con `RAVENNAKIT`, usando `BlackHole`;
- Pi: `aes67-linux-daemon + RAVENNA-ALSA` exponiendo la tarjeta ALSA
  `RAVENNA`;
- modelo DSP: `stream_in/out` consumiendo ese backend ALSA sin tocar
  `pisound_in/out`.

Lo que ya esta comprobado:

- `build/ravennakit_bridge/pisound_ravennakit_bridge` compila en este Mac con
  `PortAudio + RAVENNAKIT`;
- el binario enumera `BlackHole 16ch`, `BlackHole 2ch` y `BlackHole 64ch`;
- en la Pi existe el servicio directo `pisound-stream-ravenna@.service` para
  correr los modelos sobre la tarjeta `RAVENNA`.
- el 2026-04-20 la Pi volvio a `PTP locked` con la topologia `ptp4l -S -i eth0 -i lo`;
- ese mismo dia `pisound-stream-ravenna@bypass2.service` quedo arrancando sobre
  `plughw:CARD=RAVENNA,DEV=0` con prioridades RT validas.
- la prueba DAW completa con `bypass2_app` ya queda limpia cuando el backend de
  la Pi opera en bloques logicos de `128` frames aunque ALSA exponga periodos
  fisicos de `48` frames.
- los proyectos mixtos que usan a la vez `pisound_*` y `stream_*` compilan
  ahora sobre `common/pisound_main.c`, sin tocar ni `custom_main.c` ni
  `stream_main.c`.
- `bypasscross` ya ha quedado validado como referencia híbrida:
  `pisound_in -> stream_out` y `stream_in -> pisound_out` funcionan.

Hallazgo de estabilidad mas importante hasta ahora:

- el fallo de `bypass2` no era un `xrun` clasico ni una perdida de lock en el
  Mac;
- el problema estaba en mezclar `48` frames fisicos del PCM `RAVENNA` con
  `128` frames logicos del modelo dentro de `stream_backend`;
- la mejora buena ha sido hacer que `stream_backend` lea y escriba en bloques
  logicos de `128`, dejando a ALSA resolver el detalle del periodo fisico.
- la ruta buena actual no depende de ninguna correccion activa de deriva en
  playback; el flujo valido queda fijado por el acoplamiento correcto entre
  bloques logicos `128` y periodos fisicos ALSA.

Para arrancar el bridge del Mac en primer plano con la topologia recomendada:

```bash
common/stream/run_ravennakit_bridge_mac.sh
```

Interfaz nueva recomendada para el host:

```bash
common/scripts/bridgectl.py start
common/scripts/bridgectl.py wait
common/scripts/bridgectl.py status
common/scripts/bridgectl.py logs 80
common/scripts/bridgectl.py stop
common/scripts/bridgectl.py gui
```

La GUI mínima nueva vive en:

- `common/scripts/ravennakit_bridge_web.py`
- lanzador corto: `common/scripts/open_ravennakit_bridge_app.sh`

Y se apoya en el controlador único:

- `common/scripts/bridgectl.py`

Los scripts bajo `common/stream/` siguen siendo el backend real; `bridgectl`
los encapsula para que el frontend no tenga que conocerlos uno a uno.

Para gestionarlo como runtime persistente en el host:

```bash
common/stream/start_ravennakit_bridge_mac.sh
common/stream/status_ravennakit_bridge_mac.sh
common/stream/logs_ravennakit_bridge_mac.sh
common/stream/wait_ravennakit_bridge_mac.sh
common/stream/stop_ravennakit_bridge_mac.sh
```

El launcher del Mac puede leer configuracion persistente desde:

```text
common/stream/ravennakit_bridge_mac.env
```

Hay una plantilla en:

```text
common/stream/ravennakit_bridge_mac.env.example
```

Para `bypass2`, el orden corto recomendado en el host es:

```bash
common/stream/start_ravennakit_bridge_mac.sh
common/stream/wait_ravennakit_bridge_mac.sh
```

Defaults del wrapper:

- `BlackHole 16ch` entrada en `1-2`;
- `BlackHole 16ch` salida en `3-4`;
- interfaz `en0`;
- destino multicast `239.69.0.2:5004`;
- `ptime = 1 ms`;
- `tx_timestamp_offset_frames = 480`;
- `rx_require_delay_frames = 1024`.

## Objetivo

Esta capa existe para aislar ALSA en el borde del sistema:

- ALSA habla con el adaptador o dispositivo de borde;
- ese borde habla con `common/stream`;
- `common/stream` habla con el backend.

El backend no deberia conocer detalles internos de ALSA. Solo deberia ver un
stream PCM con formato fijo, buffers por frames y un ciclo de vida pequeno.

En la misma linea, `stream_in` y `stream_out` tampoco deberian conocer el
transporte. Deben comportarse como frontera PCM del modelo:

- `stream_in` lee el periodo actual del lado `capture` del backend;
- `stream_out` escribe el periodo actual del lado `playback` del backend.

Si por debajo ese backend esta soportado por una tarjeta ALSA `RAVENNA`,
por `snd-aloop` o por un transporte de prueba, debe ser irrelevante para las
S-Functions.

Muy importante para la arquitectura del proyecto:

- `pisound_in` y `pisound_out` no se tocan;
- esas S-Functions pertenecen al camino analogico de `pisound`;
- `pisound_in` entrega audio digital ya capturado desde ese frente analogico;
- `pisound_out` lleva ese audio digital a la salida analogica;
- `stream_in` y `stream_out` son otra frontera distinta y no deben depender de
  `pisound_*`;
- si `RAVENNA/AES67` se usa en la Pi, debe quedar encapsulado dentro de
  `stream_in/out`, antes de `stream_in` y despues de `stream_out`.

En la implementacion actual, la parte de tiempo real vive en C:

- `common/stream/stream_contract.h`: contrato PCM comun;
- `common/stream/stream_backend.[ch]`: backend de I/O para `udp`, `alsa` y
  `virtual_alsa`, ahora con ciclo de vida explicito `open/prepare/start/stop`,
  `recover()` y `query_status()`;
- `common/stream_main.c`: harness del modelo sobre ese backend.

Y, del lado Simulink generado en la Pi, la frontera correcta es:

- `common/stream_in.c`
- `common/stream_out.c`

Esas dos S-Functions son homologas a `common/pisound_in.c` y
`common/pisound_out.c`:

- `pisound_in/out` pertenecen al camino analogico local de Pisound;
- `stream_in/out` pertenecen al camino de streaming y deben presentar al modelo
  el mismo tipo de flujo PCM bloque a bloque;
- cualquier proyecto que use `stream_in/out` debe poder compilarse sin tocar el
  script comun ni depender de flags escondidos en otro sitio.

## Compile Flags Por Proyecto

La regla para proyectos `stream` debe ser esta:

- toda opcion de compilacion especifica del proyecto vive en
  `<proyecto>/<proyecto>_compile_flags.txt`;
- `common/scripts/compile.sh` es el unico punto comun de compilacion;
- si un proyecto necesita `stream`, debe declararlo con `--stream` o con
  `-DSTREAM_MODE` dentro de ese flags file;
- no deberia haber flags de proyecto repartidos por servicios, wrappers o
  scripts ad hoc.
- `compile.sh` lee ese flags file y ejecuta un preflight de requisitos en la
  Pi antes de compilar; si falta `gcc`, `libasound2-dev`, JACK, Python/DBus
  para BLE, o el runtime AES67, falla con una accion concreta en vez de crear
  una app que luego no arranca.

Para preparar una Pi nueva:

```bash
cd ~/pisound
sudo common/scripts/prepare_project.sh <proyecto>
```

`prepare_project.sh` detecta el modo desde el flags file y desde el codigo
generado: `pisound`, `stream` o `hybrid`. Si el proyecto usa la ruta
`RAVENNA/ALSA`, ejecuta el despliegue AES67 del framework; despues llama al
preflight de requisitos con `--install`.

Variantes utiles:

```bash
common/scripts/prepare_project.sh --check-only <proyecto>
sudo common/scripts/prepare_project.sh --compile <proyecto>
sudo common/scripts/prepare_project.sh --no-deploy <proyecto>
```

Ejemplo minimo real para un proyecto `stream`:

```text
-DSTREAM_MODE
-DPORTABLE_WORDSIZES
-DSTREAM_BACKEND_DEFAULT_ALSA=1
```

Hay una plantilla base en:

- `common/stream/stream_project_compile_flags.example.txt`
- para arrancar el proyecto ya compilado sobre la ruta canónica:
  `common/scripts/start_ravenna_project.sh <proyecto>`
- o, si quieres quedarte en primer plano y salir con `q` o `Ctrl-C`:
  `common/scripts/run_ravenna_project_foreground.sh <proyecto>`
- para detenerlo:
  `common/scripts/stop_ravenna_project.sh <proyecto>`

Los dos modos usan el mismo `<proyecto>_app`:

- modo `service`: delega el ciclo de vida a `systemd`
- modo `foreground`: mantiene compilacion y runtime separados, y ejecuta el
  binario con el entorno `RAVENNA` correcto hasta `q` o `Ctrl-C`

En proyectos `stream` con backend ALSA por defecto, `compile.sh` genera ese
`<proyecto>_app` como wrapper del binario real, siguiendo la misma idea que en
el caso BLE: la app visible arranca la infraestructura que necesita y la suelta
al terminar.

En macOS, las cabezas `A` y `B` del bridge AES67 deben seguir la misma idea:
dos procesos independientes, simetricos, con I/O por periodo fijo y una
maquina de estados pequena (`open -> prepare -> start -> running -> recover ->
stop`), en lugar de compartir un buffer interno entre captura y reproduccion.

Para el receptor macOS conviene separar dos modos:

- `receive + analyze`: verificar que `aes67_in(mac)` escucha, parsea RTP/L24 y
  mide `seq_gap/peak/rms` sin depender de CoreAudio;
- `receive + playout`: solo despues, conectar esa recepcion validada a la
  salida de audio real.

En esta fase existen helpers nuevos para ese aislamiento:

- `common/stream/rx_headless_mac.js`: receptor headless centrado en
  `receive + analyze`, con opcion de reenviar PCM float32 por UDP local;
- `common/stream/rtp_forward_mac.py`: receptor RTP L24 en Python que reenvia
  bloques PCM float32 por UDP local;
- `common/stream/pcm_playout_mac.py`: playout CoreAudio en proceso separado,
  alimentado por PCM float32 local;
- `common/stream/pcm_capture_mac.py`: captura/analisador simple para verificar
  lo que llega a una pareja de canales del dispositivo.
- `common/stream/prove_b_path.py`: orquestador de prueba local que levanta
  `receive -> forward -> playout -> capture` en un solo comando.

Tambien queda preparado un nuevo punto de partida portable para sustituir ese
bridge Python especifico de macOS:

- `common/stream/ravennakit_bridge/`

Ese directorio define una arquitectura C++ separando:

- `IDuplexAudioDriver`: audio local del host;
- `IAoipEngine`: red `RAVENNA/AES67`;
- `BridgeConfig`: configuracion comun CLI;
- `main.cpp`: ensamblado del bridge.

La intencion es que el lado Mac use `BlackHole`, pero que la misma base pueda
reutilizarse en Windows o Linux cambiando solo el dispositivo de audio local.

## Interfaz de red AES67

La ruta de audio del bridge no esta fijada a una NIC concreta, pero la
sincronizacion PTP del sistema si necesita saber que interfaz cableada usar.

En esta carpeta, el despliegue instala `/etc/default/pisound-aes67` con:

```text
AES67_IFACE=eth0
```

Si la Raspberry o la distro nombran la interfaz de otra forma, por ejemplo
`end0` o `enp1s0`, hay que cambiar ese valor y reiniciar:

```text
systemctl restart ptp4l-aes67 phc2sys-aes67 \
  pisound-ravenna-http pisound-ravenna-rtsp pisound-aes67-sessions
```

El problema no suele estar en el modelo generado, sino en haber dejado
`ptp4l/phc2sys` atados a `eth0` cuando la NIC real tiene otro nombre o expone
otro PHC.

## Alcance

Incluye:

- descripcion del formato PCM;
- negociacion minima de capacidades;
- estados del stream;
- lectura y escritura por frames;
- timestamps monotonicos;
- manejo de `xrun`.

No incluye:

- mixers;
- controles ALSA completos;
- MIDI;
- transporte de red;
- enumeracion del sistema.

## Modelo

Un stream es un flujo PCM en una sola direccion:

- `playback`: un cliente ALSA escribe audio y el backend lo consume;
- `capture`: el backend produce audio y el cliente ALSA lo lee.

Toda la API externa usa `frames` como unidad canonica. No bytes.

## Tipos base

```text
enum StreamDirection {
  Playback,
  Capture,
}

enum SampleFormat {
  S16LE,
  S24LE,
  S32LE,
  Float32LE,
}

struct PcmFormat {
  direction: StreamDirection,
  sample_rate_hz: u32,
  channels: u16,
  sample_format: SampleFormat,
  frames_per_period: u32,
  period_count: u16,
}

struct BufferMeta {
  frame_count: u32,
  pts_monotonic_ns: u64,
  sequence: u64,
}
```

Restricciones recomendadas:

- `sample_rate_hz > 0`
- `channels > 0`
- `frames_per_period > 0`
- `period_count >= 2`

## Capacidades

El adaptador ALSA debe poder anunciar una superficie pequena y estable:

```text
struct StreamCapabilities {
  directions: [Playback | Capture],
  sample_formats: [SampleFormat],
  sample_rates_hz: [u32] | Range,
  channel_counts: [u16] | Range,
  min_frames_per_period: u32,
  max_frames_per_period: u32,
  min_period_count: u16,
  max_period_count: u16,
}
```

Minimo viable sugerido:

- `Playback`;
- `48000 Hz`;
- `2` canales;
- `S16LE`;
- `256` frames por periodo;
- `4` periodos.

## Ciclo de vida

```text
Closed -> Open -> Prepared -> Running -> Draining -> Stopped -> Closed
```

Operaciones minimas:

```text
open(format) -> handle
prepare(handle)
start(handle)
stop(handle)
drain(handle)
close(handle)

write(handle, frames, meta)
read(handle, max_frames)
query_status(handle)
recover(handle, error)
```

Semantica:

- el formato no cambia tras `open`;
- `prepare` reserva e inicializa buffers;
- `start` activa I/O;
- `drain` aplica a `playback`;
- `recover` intenta salir de `xrun` sin reconstruir todo;
- si cambia el formato, se cierra y se vuelve a abrir.

## Semantica de I/O

### `write`

Para `playback`:

- acepta hasta `n` frames;
- puede aceptar menos;
- puede devolver `WouldBlock`;
- puede devolver `XRun`;
- no debe bloquear indefinidamente.

### `read`

Para `capture`:

- devuelve hasta `n` frames;
- puede devolver menos;
- puede devolver `WouldBlock`;
- incluye `BufferMeta`.

## Estado observable

```text
struct StreamStatus {
  state: StreamState,
  queued_frames: u32,
  available_frames: u32,
  xruns: u64,
  delay_frames: u32,
  last_sequence: u64,
}
```

`delay_frames` es la latencia visible entre el extremo ALSA y el backend.

## Errores

```text
enum StreamError {
  InvalidFormat,
  UnsupportedFormat,
  BadState,
  WouldBlock,
  XRun,
  DeviceLost,
  BackendDisconnected,
  Internal,
}
```

Regla practica:

- `WouldBlock` no es fatal;
- `XRun` intenta `recover`;
- `BackendDisconnected` obliga a cerrar;
- `BadState` suele indicar bug de integracion.

## Mapeo a ALSA virtual

El driver ALSA virtual deberia traducir asi:

- `hw_params` -> `PcmFormat`;
- `prepare` -> `prepare()`;
- `trigger START` -> `start()`;
- `trigger STOP` -> `stop()`;
- `pointer` -> frames confirmados por backend;
- `copy` o `mmap` -> `write()` / `read()`.

Para la primera iteracion, la ruta de copias es suficiente. `mmap` puede venir
despues.

## Backends actuales

`stream_main` ya puede arrancar sobre tres variantes:

- `STREAM_BACKEND=virtual_alsa`: usa el dispositivo ALSA virtual configurado en
  `STREAM_CAPTURE_DEVICE` y `STREAM_PLAYBACK_DEVICE`, usando por defecto
  `capture_aes67_app` y `playback_aes67_app`;
- `STREAM_BACKEND=alsa`: usa un dispositivo ALSA explicito, pensado para pruebas
  o hardware directo, por ejemplo la tarjeta `RAVENNA`;
- `STREAM_BACKEND=udp`: usa el enlace directo por UDP localhost con
  `aes67_bridge`, sin pasar por el dispositivo ALSA virtual.

Hay dos despliegues distintos y no deben mezclarse:

- `virtual_alsa`: mantiene el bridge antiguo sobre `snd-aloop`;
- `alsa` directo con `STREAM_DEVICE=RAVENNA`: conecta `stream_in/out`
  directamente con la tarjeta AES67 abierta por `aes67-daemon`.

Para la migracion objetivo de la Pi, el despliegue correcto es el segundo.

## Integracion en Patchbox

Si queremos que esto "ya exista" en Patchbox como dispositivo virtual del
sistema, la opcion razonable no es escribir un driver de kernel nuevo. La opcion
razonable es integrar este backend como servicio del sistema sobre ALSA
loopback:

- `snd-aloop` expone el PCM virtual;
- `/etc/asound.conf` publica `pcm.aes67`;
- `pisound_aes67_bridge` conecta ese PCM virtual con AES67;
- `pisound-stream-app@.service` arranca los modelos sobre
  `STREAM_BACKEND=virtual_alsa`.

Eso hace que, para el resto del sistema, el backend aparezca ya como un
dispositivo ALSA virtual estable. Es mucho mas mantenible que un modulo de
kernel y encaja mejor con Patchbox.

## Smoke test minimo: `bypass2_app`

`bypass2_app` es la prueba minima de esta arquitectura y debe seguir siendolo:

- entra por `stream_in`;
- sale por `stream_out`;
- no toca `pisound_in`;
- no toca `pisound_out`;
- no necesita logica DSP adicional, porque su funcion es solo hacer de corto
  entre entrada y salida.

Si `bypass2_app` funciona sobre `RAVENNA`, el contrato de `stream_in/out`
queda validado para el resto de modelos que compartan esa frontera.

## Latencia inicial

Configuracion sugerida para arrancar:

- `48000 Hz`;
- `2` canales;
- `256` frames;
- `4` periodos.

Latencia de buffer aproximada:

```text
256 * 4 / 48000 = 21.33 ms
```

## Orden de implementacion

1. fijar esta spec;
2. definir tipos concretos en `common/stream`;
3. crear un backend in-memory con ring buffer y tests;
4. adaptar ALSA virtual a esta API;
5. medir `xrun`, latencia y drift.

## Minimo viable real

Para desbloquear el driver ALSA virtual basta con:

- solo `playback`;
- solo `48000 Hz`;
- solo estereo;
- solo `S16LE`;
- ring buffer por frames;
- `open/prepare/start/stop/close`;
- `write/query_status/recover`.

La regla de diseno es simple: `common/stream` debe reducir ALSA a un contrato
pequeno, determinista y portable para que el backend pueda comportarse como un
dispositivo virtual sin contaminarse con detalles del borde.

## Estado historico en la Pi

Nota historica, no arquitectura objetivo vigente. Comprobado en
`patch@patchbox.local` el 2026-04-17:

- la Pi no esta usando hoy un stack `aes67_daemon + MergingALSA`;
- no aparecen paquetes ni servicios `MergingALSA` ni `aes67_daemon`;
- el camino desplegado en ese momento era el bridge propio:
  `pisound-aes67-bridge.service` + `ptp4l-aes67.service` +
  `phc2sys-aes67.service`;
- las apps del modelo corren con `pisound-stream-app@.service` sobre
  `STREAM_BACKEND=virtual_alsa`;
- PipeWire ve `snd-aloop` como nodos ALSA (`alsa_output.platform-snd_aloop...`
  y `alsa_input.platform-snd_aloop...`).

Esto importa porque `stream_in` y `stream_out` si existen en el codigo comun,
pero en la Pi quedan embebidos dentro de cada `*_app`; no son un daemon AES67
del sistema por si solos.

## Candidato open source real detectado

En esa misma Pi si esta instalado `pipewire-aes67`:

- binario: `/usr/bin/pipewire-aes67`
- config base: `/usr/share/pipewire/pipewire-aes67.conf`

Esa configuracion ya contempla:

- seleccion de PHC por interfaz (`clock.interface = "eth0"`);
- anuncios SAP;
- sink/source AES67 en red;
- integracion con PipeWire en modo open source.

A dia de hoy no aparece activado ningun servicio especifico de
`pipewire-aes67`, asi que el candidato open source real para migrar desde el
bridge propio no es `MergingALSA`, sino `pipewire-aes67`.

## Migracion hacia `pipewire-aes67`

La ruta de migracion propuesta mantiene `stream_in` y `stream_out` sin cambios:

- las apps DSP siguen usando `STREAM_BACKEND=virtual_alsa`;
- `snd-aloop` sigue siendo la frontera ALSA local;
- `pipewire-aes67` sustituye solo la parte de red y reloj que hoy hace
  `pisound_aes67_bridge`.

Mapeo de senal:

- TX app -> `playback_aes67_app` -> `hw:Loopback,0,0`
- PipeWire ve eso como
  `alsa_input.platform-snd_aloop.0.analog-stereo:capture_FL/FR`
- el helper enlaza esos puertos a `rtp-sink:send_CH1/CH2`
- RX AES67 crea un `Audio/Source` en PipeWire
- el helper enlaza ese source a
  `alsa_output.platform-snd_aloop.0.analog-stereo:playback_FL/FR`
- la app lee de `capture_aes67_app` -> `hw:Loopback,1,1`

Archivos de migracion anadidos en esta carpeta:

- `pipewire-aes67-pisound.conf.example`
- `pisound-pipewire-aes67.service`
- `pisound-pipewire-aes67-link.service`
- `pisound-pipewire-aes67-link.sh`
- `99-pisound-ptp-permissions.rules.example`
- `setup_pipewire_aes67.sh`

Hallazgo importante en `patchbox.local`:

- `pipewire-aes67` crea `rtp-sink` correctamente;
- en la sesion de `patch` no pudo abrir `/dev/ptp0` por permisos;
- por eso la migracion necesita una regla udev o permisos equivalentes sobre
  los dispositivos `ptp*` antes de apagar el bridge propio.

Esta migracion aun no esta desplegada en la Pi. El repo solo queda preparado
con la configuracion, los servicios y el helper de enlaces para continuarla.

## Bloqueo real encontrado al desplegar

Probado en `patch@patchbox.local` el 2026-04-18:

- los permisos sobre `/dev/ptp0` se pudieron corregir con udev;
- `pipewire-aes67` arranca y publica `rtp-sink` en la variante basica;
- pero la build instalada de PipeWire no puede crear nodos ALSA directos dentro
  de `pipewire-aes67`.

Error observado al intentar usar adapters ALSA en la propia config:

```text
no library for api.alsa.pcm.source: No such file or directory
can't create object from factory adapter
failed to create context: No such file or directory
```

Consecuencia practica:

- la migracion no pudo completarse usando `pipewire-aes67` + adapters ALSA
  directos en esta imagen actual de Patchbox;
- para no dejar la Pi rota, se deshabilitaron los servicios
  `pisound-pipewire-aes67*` y se restauro `pisound-aes67-bridge.service`.

Asi que el siguiente paso tecnico ya no es solo configuracion:

- o se instala una build de PipeWire que incluya soporte `api.alsa.pcm.*`
  accesible desde `pipewire-aes67`;
- o se busca otra estrategia open source para el puente ALSA <-> AES67.

## Ruta `aes67-daemon + RAVENNA-ALSA`

Probado tambien en `patch@patchbox.local` el 2026-04-18:

- `aes67-linux-daemon` si compila en esta Patchbox;
- el modulo del driver `RAVENNA-ALSA` compila contra el kernel activo
  `6.12.75+rpt-rpi-2712`;
- el modulo carga bien y expone una tarjeta ALSA nueva:
  `card 2: RAVENNA [Merging RAVENNA]`;
- `aes67-daemon` queda instalado en `/usr/local/bin/aes67-daemon`;
- la configuracion activa vive en `/etc/daemon.conf`;
- el servicio `aes67-daemon.service` queda instalado y el WebUI responde en
  `http://patchbox.local:8090`.

Correccion importante encontrada el 2026-04-21:

- en esta instalacion concreta, `aes67-daemon.service` quedaba colgado en
  `activating` cuando la unidad estaba declarada como `Type=notify`;
- eso bloqueaba por dependencia a `pisound-aes67-sessions.service` y a
  `pisound-stream-ravenna@.service`;
- para dejar la Pi instalable y repetible, la unidad debe quedar como
  `Type=simple`.

En esta carpeta queda tambien el helper:

- `common/stream/fix_pi_aes67_daemon_service.sh`

Uso en la Pi:

```bash
sudo /home/patch/pisound/common/stream/fix_pi_aes67_daemon_service.sh
```

Despues de aplicarlo, el estado esperado es:

```bash
systemctl show aes67-daemon.service -p Type -p ActiveState -p SubState
```

y debe devolver algo equivalente a:

```text
Type=simple
ActiveState=active
SubState=running
```

Configuracion desplegada en la Pi:

- `interface_name = "eth0"`
- `sample_rate = 48000`
- `ptp_domain = 0`
- `rtp_port = 5004`
- `http_port = 8090`
- `status_file = "/etc/status.json"`
- `ptp_status_script = "/usr/local/share/aes67-daemon/scripts/ptp_status.sh"`

Esto corrige una hipotesis anterior: `pipewire-aes67` no es el unico candidato
open source real en esta Pi. `aes67-daemon + RAVENNA-ALSA` tambien es viable y
ya esta desplegado para pruebas.

## Bloqueo PTP real en `eth0`

El daemon y el driver quedan arriba, pero el estado PTP sigue:

```json
{ "status": "unlocked", "gmid": "00-00-00-00-00-00-00-00" }
```

La causa encontrada no es solo de configuracion local. El propio proyecto
documenta que:

- el driver actua como esclavo PTP;
- necesita ver un master PTP compatible en red;
- `ptp4l` desactiva `IP_MULTICAST_LOOP`, asi que un master levantado en la
  misma maquina sobre `eth0` no se reinyecta localmente al driver;
- la unica excepcion mencionada por el propio proyecto es usar `lo` en el test
  autocontenido.

Consecuencia practica:

- para una prueba aislada en una sola Pi, `ptp4l` local sobre `eth0` no basta
  para que `RAVENNA-ALSA` llegue a `locked`;
- para uso real en AES67 sobre red, hace falta un grandmaster PTP externo o un
  dispositivo AES67 que anuncie ese reloj en la LAN;
- mientras no exista ese master externo, `aes67-daemon` puede arrancar, pero no
  llegara a streaming util sobre `eth0`.

## Estado operativo observado despues del despliegue

Nota historica de migracion. Tras esas pruebas, en `patchbox.local` quedaban
visibles dos caminos:

- el camino antiguo con `pisound-aes67-bridge.service`, usado entonces como
  fallback operativo;
- el camino nuevo con `aes67-daemon.service` y la tarjeta ALSA `RAVENNA`,
  desplegado pero pendiente de un master PTP real para completar la migracion.

La arquitectura objetivo actual ya no depende de ese bridge directo: el
framework prepara `aes67-daemon.service`, `RAVENNA` ALSA y
`pisound-stream-ravenna@.service`.

## Servicio directo para `RAVENNA`

En esta carpeta tambien queda preparado el servicio:

- `pisound-stream-ravenna@.service`

Este servicio arranca un modelo directamente sobre la tarjeta ALSA `RAVENNA`
usando formato nativo entero y conversion explicita en `stream_backend`:

```text
STREAM_BACKEND=alsa
STREAM_SAMPLE_FORMAT=s32le
STREAM_DEVICE=hw:CARD=RAVENNA,DEV=0
STREAM_CAPTURE_DEVICE=hw:CARD=RAVENNA,DEV=0
STREAM_PLAYBACK_DEVICE=hw:CARD=RAVENNA,DEV=0
```

Su intencion es probar exactamente el camino:

```text
AES67/RAVENNA -> stream_in -> modelo -> stream_out -> AES67/RAVENNA
```

sin pasar por `snd-aloop`, sin pasar por `pisound_*` y sin depender del bridge
propio.

Correccion operativa importante para no perder tiempo:

- `pisound-stream-app@.service` pertenece al camino legacy `udp/direct`;
- `pisound-stream-ravenna@.service` es el servicio correcto para la ruta final
  `aes67-daemon + ravenna-alsa`;
- no deben quedar los dos activos para el mismo proyecto al mismo tiempo.

En la Pi, para un proyecto como `bypass2`, la combinacion correcta es:

```bash
sudo systemctl disable --now pisound-stream-app@bypass2.service
sudo systemctl enable --now pisound-stream-ravenna@bypass2.service
```

## Siguiente arquitectura objetivo

La arquitectura objetivo queda asi:

- Pi:
  `stream_in/out` sobre `aes67-linux-daemon + RAVENNA-ALSA`;
- Mac, Windows o Linux host:
  bridge local `audio-device <-> ETH` construido sobre `RAVENNAKIT`;
- Mac especificamente:
  `BlackHole 1-2 -> ETH` y `ETH -> BlackHole 3-4`.

Razon de esta decision:

- `RAVENNAKIT` documenta `RavennaNode` como abstraccion recomendada para montar
  un nodo virtual;
- la documentacion publica muestra integracion por `CMake`;
- los ejemplos de emisor y receptor usan `PortAudio`, lo que ayuda a no atar
  el bridge al backend de audio de un solo sistema operativo.

Trabajo en curso en este repo:

- `common/stream/ravennakit_bridge/` ya separa `PortAudio` del motor AoIP;
- admite una etapa `loopback` para validar `BlackHole 1-2 -> BlackHole 3-4`
  sin red;
- deja preparada la forma de las sesiones `TX/RX` y el adapter de
  `RAVENNAKIT` para cerrar despues el puente completo con la Pi.

## Estado real tras integrar `RAVENNAKIT`

Lo que ya quedo comprobado de punta a punta es esto:

- el bridge nuevo del host compila y enlaza de verdad con `RAVENNAKIT`;
- en el Mac ya emite un stream `RAVENNA/AES67` real por `en0`;
- la Pi con `aes67-daemon + RAVENNA-ALSA` ya descubre ese stream por
  `mDNS/RTSP`;
- la Pi llega a recibir RTP en un `sink` real del daemon.

En otras palabras: el transporte `Mac -> ETH -> Pi` ya no es teorico; ya esta
vivo a nivel de anuncio y de paquetes RTP.

El bloqueo que queda es de reloj PTP:

- el `sink` de la Pi muestra `receiving_rtp_packet=true`;
- pero el estado PTP del driver sigue en `unlocked`;
- mientras eso ocurra, el driver deja el audio mutado y no hay flujo util para
  `stream_in`.

### Hallazgo clave sobre `RAVENNAKIT`

La implementacion PTP del SDK vendorized nace en modo `slave_only`.

Pruebas concretas:

- en `third_party/ravennakit/include/ravennakit/ptp/ptp_error.hpp` existe el
  error `only_slave_supported`;
- en `third_party/ravennakit/src/ravennakit/ptp/ptp_instance.cpp` la instancia
  se construye con `default_ds_(true)`;
- ese `true` inicializa `DefaultDs::slave_only = true`.

Consecuencia practica:

- con solo `Pi + Mac`, esta build de `RAVENNAKIT` no esta asumiendo el papel de
  grandmaster PTP;
- cuando se apaga `ptp4l` en la Pi, el Mac pasa a `listening`, no a `master`;
- por eso `aes67-daemon` en la Pi sigue `unlocked` aunque el RTP ya llegue.

### Conclusiones actuales

- `RAVENNAKIT` si nos vale para el bridge `audio-device <-> ETH` del host;
- `aes67-linux-daemon + RAVENNA-ALSA` si nos vale para exponer `RAVENNA`
  dentro de `stream_in/out` en la Pi;
- lo que sigue faltando para una topologia `solo Pi + Mac` es un grandmaster
  PTP real o una version/parche del lado Mac que permita `master`.

En ese momento, la Pi se dejo restaurada al estado operativo anterior:

- `ptp4l-aes67.service` activo;
- `phc2sys-aes67.service` activo;
- `pisound-aes67-bridge.service` activo.

## Avance real con la Pi como grandmaster

Despues de seguir probando la hipotesis "la Pi manda el reloj y el Mac sigue",
ya hay una receta que si mueve el sistema:

- `ptp4l` en la Pi anunciando por `eth0` y por `lo` al mismo tiempo;
- `aes67-daemon` configurado con `interface_name = "eth0,lo"`;
- `aes67-daemon + RAVENNA-ALSA` siguiendo ese reloj local;
- el bridge del Mac con `RAVENNAKIT` funcionando como follower.

Con esa combinacion se comprobo lo siguiente en la Pi:

- `/api/ptp/status` pasa a `locked`;
- el GMID queda en la MAC de la propia Pi;
- `arecord -D dsnoop:CARD=RAVENNA,DEV=0 -f S32_LE ...` captura de verdad;
- `bypass2_app` arranca sobre `plughw:CARD=RAVENNA,DEV=0` sin error;
- mientras `bypass2_app` corre, salen paquetes RTP desde la Pi hacia
  `239.69.0.2:5004`.

En otras palabras:

```text
Mac bridge -> AES67/RAVENNA -> Pi stream_in -> bypass2_app -> stream_out -> AES67/RAVENNA
```

ya esta vivo a nivel de reloj, captura ALSA y emision RTP de retorno.

### Configuracion de prueba que funciono

En la Pi:

```bash
sudo systemctl stop pisound-aes67-runtime.target \
  ptp4l-aes67 phc2sys-aes67 \
  pisound-ravenna-http pisound-ravenna-rtsp pisound-aes67-sessions
sudo ptp4l -m -A -4 -S -i eth0 -i lo \
  --dscp_event=46 \
  --dscp_general=34 \
  --domainNumber=0 \
  --logAnnounceInterval=1 \
  --logSyncInterval=-3 \
  --logMinDelayReqInterval=0 \
  --logMinPdelayReqInterval=0 \
  --announceReceiptTimeout=3 \
  --priority1=128 \
  --priority2=128
```

Y en `/etc/daemon.conf`:

```json
"interface_name": "eth0,lo"
```

Notas practicas:

- con `interface_name = "eth0,lo"` el daemon ya se une a `224.0.1.129` tanto en
  `192.168.0.31` como en `127.0.0.1`;
- para captura compartida en pruebas manuales, `dsnoop:CARD=RAVENNA,DEV=0`
  funciono con formato `S32_LE`;
- para las apps del proyecto, el nombre ALSA que si abrio fue
  `plughw:CARD=RAVENNA,DEV=0`, no `RAVENNA` a secas.

### Cambios en los scripts del repo

El wrapper `common/stream/ptp4l-aes67.sh` ahora admite:

- `AES67_PTP_EXTRA_IFACES`
- `AES67_PTP_SOFTWARE_TS`

para poder expresar desde entorno una topologia `eth0 + lo` sin reescribir la
unidad systemd.

### Restauracion automatica de sesiones en la Pi

Para que `aes67-daemon` no vuelva a quedarse con `sources: []` y `sinks: []`
tras un reinicio, esta carpeta incluye tambien:

- `common/stream/pisound_aes67_sessions.py`
- `common/stream/pisound-aes67-sessions.service`
- `common/stream/pisound-aes67-sessions.env.example`

Ese `oneshot` reconstruye en la Pi:

- `source 0` como `Pisound_Out`, que es el stream publicado por la Pi para lo
  que sale de `stream_out`;
- `sink 0` tomando el SDP remoto del stream `Pisound_In`, que es el stream del
  Mac consumido por `stream_in`.

La decision importante es que ya no depende del puerto RTSP efimero del bridge
del Mac. En vez de guardar una URL fija, pregunta a
`GET /api/browse/sources/all`, toma el SDP anunciado y recrea el `sink` con
`use_sdp=true`. Tampoco depende ya de que el nombre visible coincida de forma
fragil: normaliza el nombre remoto y tambien valida el `s=` del SDP.

Ademas, la unidad queda ligada a `aes67-daemon.service` con `PartOf=`, asi que
si el daemon se reinicia:

- la Pi recompone sola `source/sink`;
- el `sink` vuelve a `framecount:128`;
- y los flags de `sink/status/0` vuelven a quedar limpios cuando el bridge del
  Mac reaparece.

La configuracion persistente de esos nombres y parametros vive ahora en:

- `/etc/default/pisound-aes67-sessions`

con plantilla en:

- `common/stream/pisound-aes67-sessions.env.example`

## Arquitectura Final

La integracion correcta de `aes67-linux-daemon + RAVENNA-ALSA` en la
arquitectura es esta:

- En el Mac, `RAVENNAKIT` hace de bridge entre `BlackHole` y Ethernet.
- En la Pi, `aes67-linux-daemon` hace de gestor de sesiones AES67/RAVENNA y
  programa la tarjeta `Merging RAVENNA`.
- Esa tarjeta `RAVENNA` aparece como PCM ALSA local.
- `stream_in` lee de `STREAM_CAPTURE_DEVICE=hw:CARD=RAVENNA,DEV=0`.
- `stream_out` escribe en `STREAM_PLAYBACK_DEVICE=hw:CARD=RAVENNA,DEV=0`.

En otras palabras:

```text
Mac audio device -> RAVENNAKIT -> Ethernet -> aes67-linux-daemon -> ALSA RAVENNA -> stream_in -> modelo -> stream_out -> ALSA RAVENNA -> aes67-linux-daemon -> Ethernet
```

La consecuencia practica importante es que `aes67-linux-daemon` no debe entrar
en el codigo del modelo ni en `stream_in/out`. Su sitio arquitectonico es el
borde de red/driver de la Pi, antes de `stream_in` y despues de `stream_out`.

## Estado Persistente Que Conviene Dejar En La Pi

Para retomar trabajo sin perder tiempo, el estado que conviene dejar aplicado es:

- `aes67-daemon.service` corregido a `Type=simple`;
- `pisound-aes67-runtime.target` enabled;
- `pisound-aes67-sessions.service` enabled;
- `pisound-stream-ravenna@<proyecto>.service` enabled;

Comprobacion rapida:

```bash
systemctl is-enabled aes67-daemon.service \
  pisound-aes67-runtime.target \
  pisound-aes67-sessions.service \
  pisound-stream-ravenna@bypass2.service
```
