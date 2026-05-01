# `ravennakit_bridge`

Bridge portable pensado para:

- `macOS`: `BlackHole <-> ETH`
- `Windows`: cualquier dispositivo PortAudio-compatible ` <-> ETH`
- `Linux`: cualquier dispositivo PortAudio-compatible ` <-> ETH`

La idea es separar dos responsabilidades:

- audio local del sistema operativo;
- transporte AoIP `RAVENNA/AES67` sobre `RAVENNAKIT`.

## Objetivo

Este bridge no toca `pisound_in/out` ni la ruta analogica de la Pi.

Su papel es solo exponer un borde de red portable para una DAW o para cualquier
otro host:

```text
audio local del host -> bridge portable -> ETH/AES67 -> Pi
Pi -> ETH/AES67 -> bridge portable -> audio local del host
```

En macOS el mapeo deseado es:

```text
BlackHole 1-2 -> TX AES67
RX AES67 -> BlackHole 3-4
```

## Estado Actual

En este repo ya existe una build real del bridge en:

```text
build/ravennakit_bridge/pisound_ravennakit_bridge
```

Y en este Mac ya se ha comprobado que:

- el binario compila con `PortAudio + RAVENNAKIT`;
- `--list-devices` enumera `BlackHole 16ch`, `BlackHole 2ch` y
  `BlackHole 64ch`;
- el wrapper recomendado para arrancarlo es:

```text
common/stream/run_ravennakit_bridge_mac.sh
```

- para dejarlo como backend gestionable en macOS:

```text
common/stream/start_ravennakit_bridge_mac.sh
common/stream/status_ravennakit_bridge_mac.sh
common/stream/logs_ravennakit_bridge_mac.sh
common/stream/wait_ravennakit_bridge_mac.sh
common/stream/stop_ravennakit_bridge_mac.sh
```

Ese wrapper usa por defecto:

- entrada `BlackHole 16ch` canales `1-2`;
- salida `BlackHole 16ch` canales `3-4`;
- interfaz `en0`;
- destino multicast `239.69.0.2:5004`.
- `rx_require_delay_frames = 1024`;
- `tx_timestamp_offset_frames = 480`.

Y puede cargar overrides persistentes desde:

```text
common/stream/ravennakit_bridge_mac.env
```

## Decision de arquitectura

La implementacion queda dividida en dos capas:

1. `IDuplexAudioDriver`
   Gestiona captura y reproduccion local.
   La primera opcion prevista es `PortAudio`, porque aparece tambien en los
   ejemplos publicos de `RAVENNAKIT` y es portable entre macOS, Windows y
   Linux.

2. `IAoipEngine`
   Gestiona reloj, descubrimiento y streams de red.
   La primera opcion prevista es `RAVENNAKIT`, usando `rav::RavennaNode` como
   abstraccion de mas alto nivel.

Con esto el bridge queda pensado para Mac, pero no amarrado a CoreAudio ni a
BlackHole.

## Estado de este directorio

Este directorio deja preparada una base portable:

- parser comun de configuracion CLI;
- interfaces abstractas para audio local y AoIP;
- `main.cpp` que valida configuracion y ensambla ambas capas;
- un driver `PortAudio` listo para integrarse cuando el build se configure con
  `PISOUND_RAVENNAKIT_BRIDGE_ENABLE_PORTAUDIO=ON`;
- un backend `loopback` para probar el puente local por etapas sin depender
  todavia de red ni de `RAVENNAKIT`;
- un backend `ravennakit` ya enlazado contra `rav::RavennaNode`, con sender y
  receiver creados desde el propio bridge.

El SDK de `RAVENNAKIT` queda integrado por CMake como subdirectorio del
proyecto cuando se activa la opcion correspondiente.

## Estado actual del audio local

La capa `IDuplexAudioDriver` ya tiene una implementacion real basada en
`PortAudio`:

- abre captura y reproduccion en el mismo callback duplex;
- busca dispositivos por nombre exacto o por subcadena;
- usa `float32` interleaved;
- remapea canales por software, para poder pedir por ejemplo:
  - entrada `1-2`
  - salida `3-4`
  aunque el dispositivo exponga mas canales.

Ese remapeo software evita atarnos a APIs especificas de CoreAudio o ASIO y
mantiene la misma idea en macOS, Windows y Linux.

Para activarlo en CMake:

```text
cmake -S common/stream/ravennakit_bridge -B build/ravennakit_bridge \
  -DPISOUND_RAVENNAKIT_BRIDGE_ENABLE_PORTAUDIO=ON
```

La utilidad tambien admite:

```text
pisound_ravennakit_bridge --list-devices
```

para enumerar los dispositivos vistos por `PortAudio` en el host actual.

## Estado actual del motor AoIP

La capa `IAoipEngine` ahora tiene tres modos:

- `stub`
  falla de forma explicita y sirve para no confundir una build sin backend AoIP
  real;
- `loopback`
  devuelve a la salida exactamente lo que entra por captura, y permite validar
  la parte local del bridge;
- `ravennakit`
  es el objetivo final para `RAVENNA/AES67`.

La integracion de `RAVENNAKIT` sigue pensada para esta forma de build:

```text
cmake -S common/stream/ravennakit_bridge -B build/ravennakit_bridge \
  -DPISOUND_RAVENNAKIT_BRIDGE_ENABLE_PORTAUDIO=ON \
  -DPISOUND_RAVENNAKIT_BRIDGE_ENABLE_RAVENNAKIT=ON \
  -DPISOUND_RAVENNAKIT_SOURCE_DIR=/ruta/al/sdk/ravennakit
```

Con esa opcion activa:

- el proyecto hace `add_subdirectory()` del arbol de `RAVENNAKIT`;
- enlaza contra el target `ravennakit`;
- usa una clase `RavennaKitAoipEngine`.

Esa clase ya deja implementados:

- `rav::do_system_checks()`;
- `rav::RavennaNode`;
- configuracion de interfaz de red;
- `sender` TX con sesion, `payload type`, `ttl` y destino;
- `receiver` RX con sesion y `delay` requerido.

El bloqueo principal ya no es "enchufar la API", sino validar reloj PTP y flujo
real extremo a extremo contra la Pi.

## Prueba por etapas

Sin `RAVENNAKIT` todavia montado, ya se puede validar la parte local del host
con:

```text
pisound_ravennakit_bridge \
  --input-device "BlackHole 16ch" \
  --output-device "BlackHole 16ch" \
  --input-start-channel 1 \
  --output-start-channel 3 \
  --network-interface en0 \
  --aoip-backend loopback
```

Ese modo comprueba:

- apertura duplex del dispositivo local;
- remapeo `1-2 -> 3-4`;
- callback duplex y cola intermedia del bridge.

No prueba red todavia, pero si nos deja verificar el esqueleto completo del
host antes de meter `RAVENNAKIT`.

## Comando objetivo

El comando real recomendado en este repo queda asi:

```text
common/stream/run_ravennakit_bridge_mac.sh
```

Si se quiere como backend persistente del host, la secuencia operativa es:

```text
common/stream/start_ravennakit_bridge_mac.sh
common/stream/wait_ravennakit_bridge_mac.sh
common/stream/status_ravennakit_bridge_mac.sh
common/stream/stop_ravennakit_bridge_mac.sh
```

Y su equivalente expandido es:

```text
pisound_ravennakit_bridge \
  --audio-backend portaudio \
  --aoip-backend ravennakit \
  --input-device "BlackHole 16ch" \
  --input-start-channel 1 \
  --output-device "BlackHole 16ch" \
  --output-start-channel 3 \
  --network-interface en0 \
  --tx-destination 239.69.0.2 \
  --tx-port 5004 \
  --rx-port 5004 \
  --sample-rate 48000 \
  --block-size 128 \
  --channels 2 \
  --payload-type 98 \
  --session-name pisound \
  --tx-session-name Pisound_In \
  --rx-session-name Pisound_Out \
  --rx-require-delay-frames 1024 \
  --tx-timestamp-offset-frames 480 \
  --verbose
```

En Windows o Linux solo deberia cambiar el nombre del dispositivo local y la
interfaz de red.
