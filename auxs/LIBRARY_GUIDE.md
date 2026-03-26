# Guía para Crear la Biblioteca de Simulink

## Pasos para Crear tu Biblioteca PiSound

### 1. Abrir MATLAB

Navega al directorio de tu proyecto:
```matlab
cd '/Users/linogarciamorales/Library/Mobile Documents/com~apple~CloudDocs/asignaturas/Ingeniería de Audio III/pisound'
```

### 2. Ejecutar el Script

Ejecuta el script de creación de biblioteca:
```matlab
create_pisound_library
```

### 3. Resultado

El script creará automáticamente:
- ✅ **pisound_library.slx** - Biblioteca de Simulink
- ✅ **PiSound Input** - Bloque con máscara personalizada
- ✅ **PiSound Output** - Bloque con máscara personalizada

## Características de los Bloques

### PiSound Input

**Interfaz Gráfica (Máscara):**
- Sample Rate: Campo de texto (default: 48000)
- Buffer Size: Campo de texto (default: 256)
- Latency: Campo de texto (default: 5.33)
- Backend: Menú desplegable (Auto/ALSA/JACK)

**Salidas:**
- Audio L (int32)
- Audio R (int32)
- MIDI (uint8[128])
- Count (uint32)

### PiSound Output

**Interfaz Gráfica (Máscara):**
- Backend: Menú desplegable (Auto/ALSA/JACK)

**Entradas:**
- Audio L (int32)
- Audio R (int32)
- MIDI (uint8[128])
- Count (uint32)

## Usar los Bloques en tu Modelo

### Método 1: Arrastrar y Soltar

1. Abre tu modelo Simulink
2. Abre la biblioteca: `open_system('pisound_library')`
3. Arrastra los bloques a tu modelo

### Método 2: Comando MATLAB

```matlab
% Añadir PiSound Input a tu modelo
add_block('pisound_library/PiSound Input', 'tu_modelo/PiSound Input');

% Añadir PiSound Output a tu modelo
add_block('pisound_library/PiSound Output', 'tu_modelo/PiSound Output');
```

### Método 3: Library Browser

1. Abre Library Browser en Simulink
2. Busca "pisound_library"
3. Arrastra los bloques a tu modelo

## Configurar los Bloques

### Configuración Típica

**PiSound Input:**
1. Doble clic en el bloque
2. Sample Rate: `48000`
3. Buffer Size: `256`
4. Latency: `5.33`
5. Backend: `Auto (JACK preferred)`

**PiSound Output:**
1. Doble clic en el bloque
2. Backend: `Auto (JACK preferred)` (debe coincidir con Input)

### Configuraciones Predefinidas

**Baja Latencia:**
```
Sample Rate: 48000
Buffer Size: 128
Latency: 2.67
Backend: Auto
```

**Calidad Alta:**
```
Sample Rate: 96000
Buffer Size: 512
Latency: 5.33
Backend: Auto
```

## Ejemplo Completo de Modelo

```matlab
% Crear nuevo modelo
model_name = 'pisound_test';
new_system(model_name);
open_system(model_name);

% Añadir bloques PiSound
add_block('pisound_library/PiSound Input', [model_name '/PiSound In']);
add_block('pisound_library/PiSound Output', [model_name '/PiSound Out']);

% Posicionar bloques
set_param([model_name '/PiSound In'], 'Position', [30, 30, 130, 130]);
set_param([model_name '/PiSound Out'], 'Position', [400, 30, 500, 130]);

% Conectar audio directamente (passthrough)
add_line(model_name, 'PiSound In/1', 'PiSound Out/1');  % Audio L
add_line(model_name, 'PiSound In/2', 'PiSound Out/2');  % Audio R
add_line(model_name, 'PiSound In/3', 'PiSound Out/3');  % MIDI
add_line(model_name, 'PiSound In/4', 'PiSound Out/4');  % MIDI Count

% Guardar modelo
save_system(model_name);
```

## Personalizar la Biblioteca

Si quieres modificar la biblioteca después de crearla:

```matlab
% Abrir biblioteca para edición
open_system('pisound_library');
set_param('pisound_library', 'Lock', 'off');

% Hacer cambios...

% Guardar y bloquear
save_system('pisound_library');
set_param('pisound_library', 'Lock', 'on');
```

## Compartir la Biblioteca

Para compartir tu biblioteca con otros:

1. Copia estos archivos:
   - `pisound_library.slx`
   - `pisound_in.mexmaci64` (o `.mexmaca64`)
   - `pisound_out.mexmaci64` (o `.mexmaca64`)
   - `PARAMETERS_GUIDE.md`
   - `build_instructions.md`

2. Los usuarios simplemente abren `pisound_library.slx` en Simulink

## Solución de Problemas

### "S-Function 'pisound_in' no encontrada"

- Asegúrate de que los archivos `.mexmaci64` están en el mismo directorio
- O añade el directorio al path de MATLAB:
  ```matlab
  addpath('/ruta/a/pisound');
  ```

### "Error al abrir biblioteca"

- Verifica que ejecutaste `create_pisound_library` primero
- Comprueba que la compilación MEX fue exitosa

### "Los bloques no aparecen en Library Browser"

- Ejecuta: `sl_refresh_customizations`
- Reinicia MATLAB si es necesario

## Siguiente Paso

Una vez creada la biblioteca, puedes:
1. ✅ Usar los bloques en cualquier modelo Simulink
2. ✅ Diseñar tu procesamiento de audio
3. ✅ Generar código C con Simulink Coder
4. ✅ Compilar en Raspberry Pi
