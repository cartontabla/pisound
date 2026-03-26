# Crear Modelo Simulink para Efecto de Ganancia

## Método 1: Manual en Simulink (Recomendado)

### Paso 1: Crear Nuevo Modelo

1. Abre MATLAB
2. Crea nuevo modelo: `Ctrl+N` o `File → New → Model`
3. Guarda como: `gain_effect.slx`

### Paso 2: Añadir Bloques

#### A partir de tu Biblioteca:
```matlab
% Abrir biblioteca
open_system('pisound_library')
```

Arrastra estos bloques a tu modelo:
1. **PiSound Input** (de pisound_library)
2. **Gain** (de Simulink > Math Operations)
3. **PiSound Output** (de pisound_library)

#### O por comandos:
```matlab
% Añadir PiSound Input
add_block('pisound_library/PiSound Input', 'gain_effect/PiSound In');

% Añadir Gain (stereo - necesitas 2)
add_block('simulink/Math Operations/Gain', 'gain_effect/Gain L');
add_block('simulink/Math Operations/Gain', 'gain_effect/Gain R');

% Añadir PiSound Output
add_block('pisound_library/PiSound Output', 'gain_effect/PiSound Out');
```

### Paso 3: Configurar Bloques

#### PiSound Input:
- Doble clic → S-function parameters
- Valor: `48000, 256, 5.33, 0`

#### Gain (ambos canales):
- Gain value: `10^(6/20)` (para +6dB)
- O simplemente: `2` (para +6dB aprox)
- Multiplication: `Element-wise`
- Sample time: `inherit`

#### PiSound Output:
- Doble clic → S-function parameters
- Valor: `0`

### Paso 4: Conectar Bloques

```
PiSound In (port 1 - Audio L) → Gain L → PiSound Out (port 1)
PiSound In (port 2 - Audio R) → Gain R → PiSound Out (port 2)
PiSound In (port 3 - MIDI) → PiSound Out (port 3)
PiSound In (port 4 - MIDI Count) → PiSound Out (port 4)
```

### Paso 5: Configurar Modelo para Code Generation

```matlab
% Abrir configuración
set_param('gain_effect', 'SolverType', 'Fixed-step');
set_param('gain_effect', 'FixedStep', '256/48000');
```

O manualmente:
1. `Ctrl+E` → Configuration Parameters
2. **Solver**:
   - Type: `Fixed-step`
   - Solver: `discrete (no continuous states)`
   - Fixed-step size: `256/48000` (o `auto`)

3. **Code Generation**:
   - System target file: `ert.tlc`
   - Language: `C`

4. **Hardware Implementation**:
   - Device vendor: `ARM Compatible`
   - Device type: `ARM Cortex-A`

### Paso 6: Generar Código

```matlab
slbuild('gain_effect')
```

Esto genera carpeta `gain_effect_ert_rtw/` con código C.

---

## Método 2: Script Automático

Copia este script en MATLAB y ejecútalo:

```matlab
function create_gain_effect_model()
% Creates a simple gain effect model with PiSound I/O

model_name = 'gain_effect';

% Create new model
if bdIsLoaded(model_name)
    close_system(model_name, 0);
end
new_system(model_name);
open_system(model_name);

% Add PiSound Input
h_in = add_block('pisound_library/PiSound Input', [model_name '/PiSound In']);
set_param(h_in, 'Position', [30, 30, 130, 180]);

% Add Gain blocks for stereo
h_gain_l = add_block('simulink/Math Operations/Gain', [model_name '/Gain L']);
set_param(h_gain_l, 'Gain', '2'); % +6dB approximately
set_param(h_gain_l, 'Position', [200, 50, 230, 80]);

h_gain_r = add_block('simulink/Math Operations/Gain', [model_name '/Gain R']);
set_param(h_gain_r, 'Gain', '2'); % +6dB approximately
set_param(h_gain_r, 'Position', [200, 120, 230, 150]);

% Add PiSound Output
h_out = add_block('pisound_library/PiSound Output', [model_name '/PiSound Out']);
set_param(h_out, 'Position', [300, 30, 400, 180]);

% Connect blocks
add_line(model_name, 'PiSound In/1', 'Gain L/1'); % Audio L
add_line(model_name, 'PiSound In/2', 'Gain R/1'); % Audio R
add_line(model_name, 'Gain L/1', 'PiSound Out/1'); % Audio L out
add_line(model_name, 'Gain R/1', 'PiSound Out/2'); % Audio R out
add_line(model_name, 'PiSound In/3', 'PiSound Out/3'); % MIDI pass-through
add_line(model_name, 'PiSound In/4', 'PiSound Out/4'); % MIDI count pass-through

% Configure solver
set_param(model_name, 'SolverType', 'Fixed-step');
set_param(model_name, 'FixedStep', '256/48000');
set_param(model_name, 'Solver', 'FixedStepDiscrete');

% Configure code generation
set_param(model_name, 'SystemTargetFile', 'ert.tlc');
set_param(model_name, 'GenCodeOnly', 'off');

% Save
save_system(model_name);
fprintf('Model created: %s.slx\n', model_name);
fprintf('To generate code: slbuild(''%s'')\n', model_name);
end
```

Luego ejecuta:
```matlab
create_gain_effect_model
```

---

## Método 3: Creación Rápida con Comandos

Si solo quieres probar rápido:

```matlab
% Crear modelo
new_system('quick_gain');
open_system('quick_gain');

% Añadir bloques esenciales
add_block('pisound_library/PiSound Input','quick_gain/In');
add_block('simulink/Math Operations/Gain','quick_gain/GL');
add_block('simulink/Math Operations/Gain','quick_gain/GR');
add_block('pisound_library/PiSound Output','quick_gain/Out');

% Configurar gains
set_param('quick_gain/GL','Gain','2');
set_param('quick_gain/GR','Gain','2');

% Conectar
add_line('quick_gain','In/1','GL/1');
add_line('quick_gain','In/2','GR/1');
add_line('quick_gain','GL/1','Out/1');
add_line('quick_gain','GR/1','Out/2');
add_line('quick_gain','In/3','Out/3');
add_line('quick_gain','In/4','Out/4');

% Configurar solver
set_param('quick_gain','SolverType','Fixed-step','FixedStep','auto');

% Guardar
save_system('quick_gain');
```

---

## Verificación

Antes de generar código, verifica:
- [ ] Modelo compila sin errores (`Ctrl+D`)
- [ ] No hay warnings de data type mismatch
- [ ] Todas las conexiones son correctas
- [ ] Sample times están configurados

## Generar Código

```matlab
slbuild('gain_effect')
```

Código generado aparecerá en: `gain_effect_ert_rtw/`

## Siguiente Paso

Transferir y compilar en Raspberry Pi (ver `SIMPLE_GAIN_GUIDE.md`).
