# Guía: Compilar y Ejecutar Simple Gain en Raspberry Pi

## Paso 1: Transferir Archivos a Raspberry Pi

Desde tu Mac, transfiere los archivos necesarios:

```bash
# Usando scp (SSH)
scp pisound_in.c patch@raspberrypi.local:~/pisound/
scp pisound_out.c patch@raspberrypi.local:~/pisound/
scp simple_gain.c patch@raspberrypi.local:~/pisound/

# O usando una memoria USB, rsync, etc.
```

## Paso 2: Conectar a Raspberry Pi

```bash
ssh patch@raspberrypi.local
cd ~/pisound
```

## Paso 3: Instalar Dependencias (si no lo hiciste antes)

```bash
sudo apt-get update
sudo apt-get install libjack-jackd2-dev libasound2-dev build-essential
```

## Paso 4: Compilar el Programa

```bash
gcc -o simple_gain simple_gain.c pisound_in.c pisound_out.c \
    -ljack -lasound -lpthread -lm -lrt -O2
```

**Nota**: Los warnings sobre `simstruc.h` son normales - el código se compilará correctamente.

## Paso 5: Ejecutar el Efecto

### Con Gain por Defecto (0 dB - unity gain):
```bash
./simple_gain
```

### Con Gain Positivo (+6 dB - amplifica):
```bash
./simple_gain 6.0
```

### Con Gain Negativo (-3 dB - atenúa):
```bash
./simple_gain -3.0
```

## Paso 6: Conectar Puertos JACK (si usas JACK)

En otra terminal SSH:

```bash
# Ver puertos disponibles
jack_lsp

# Conectar entrada
jack_connect system:capture_1 pisound_simulink_in:input_L
jack_connect system:capture_2 pisound_simulink_in:input_R

# Conectar salida
jack_connect pisound_simulink_out:output_L system:playback_1
jack_connect pisound_simulink_out:output_R system:playback_2
```

### O usar qjackctl (GUI):
```bash
qjackctl &
```

## Paso 7: Probar

1. Conecta una fuente de audio a la entrada de PiSound
2. Conecta auriculares/altavoces a la salida de PiSound
3. Ejecuta `./simple_gain 3.0` (por ejemplo)
4. Deberías escuchar el audio amplificado por +3dB

## Paso 8: Detener el Programa

Presiona `Ctrl+C` para detener el programa limpiamente.

## Troubleshooting

### "Failed to open JACK client"
- Verifica que JACK esté corriendo: `jack_wait -c`
- O inicia JACK manualmente: `jackd -d alsa -dhw:pisound`

### "ALSA Error PCM Open"
- Verifica que PiSound esté conectado
- Comprueba con: `aplay -l` y `arecord -l`

### No se escucha audio
- Verifica conexiones JACK con `jack_lsp -c`
- Verifica volumen de ALSA con `alsamixer`

### CPU muy alto
- Intenta aumentar el buffer size en el código
- Verifica que no haya xruns con `jack_wait -w`

## Próximos Pasos

Una vez que este efecto simple funcione:

1. ✅ Probar con diferentes valores de gain
2. ✅ Monitorear latencia y CPU
3. ✅ Crear efectos más complejos (delay, reverb, etc.)
4. ✅ Integrar en Simulink
5. ✅ Convertir a plugin LV2 para MODEP

## Notas sobre el Código

El archivo `simple_gain.c` incluye:
- Conversión dB ↔ lineal
- Protección contra clipping
- Procesamiento de audio 24-bit (int32)
- Pass-through de MIDI
- Manejo de señales para cierre limpio

**IMPORTANTE**: Este es un ejemplo didáctico. El código actual NO llama directamente a las S-Functions pisound_in/out porque necesitarías integrarlas correctamente con la estructura de Simulink. El siguiente paso sería crear un modelo Simulink que use estos bloques.
