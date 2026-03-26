# Guía Práctica 4: Medición y Calibración del Sistema

Antes de confiar en nuestro sistema para procesamiento de audio crítico, debemos responder una pregunta: **¿Cuánta calidad perdemos en el camino?**

En esta práctica usaremos MATLAB como **Analizador de Audio** para medir la Distorsión (THD), Ruido (SNR) y Respuesta en Frecuencia de nuestra Raspberry Pi + Pisound.

---

## 🔌 1. Setup de Medición

Usaremos una configuración de bucle cerrado (Loopback). Necesitas una interfaz de audio en tu ordenador (Host) con al menos una entrada y una salida de línea.

### Conexiones Físicas
1.  **PC Salida (L)** $\rightarrow$ **Pisound Entrada (L)**
2.  **Pisound Salida (L)** $\rightarrow$ **PC Entrada (L)**

> **⚠️ Precaución**: Empieza con los volúmenes BAJOS para evitar dañar los equipos.

---

## 🎯 2. Procedimiento de Calibración (Unity Gain)

El objetivo es conseguir **Ganancia Unitaria**: que 1 Voltio que sale del PC vuelva como 1 Voltio, compensando las ganancias de los potenciómetros.

### Paso A: Preparar la Raspberry Pi
Debemos asegurarnos de que el audio pasa limpio a través de ella. Ejecuta nuestro plugin `bypass` (o la app standalone) en la Pi.
*   Asegúrate de que la ganancia digital del modelo sea 1.0 (0 dB).
*   Asegúrate de que el switch de "Bypass" esté activo (o ganancia 1.0).

### Paso B: Calibración con MATLAB
Ejecuta el siguiente script en tu ordenador (Host). Este script emite un tono y te ayuda a ajustar los niveles.

```matlab
% calibracion_setup.m
% Genera un tono de 1kHz a -3 dBFS para ajustar niveles

fs = 48000;
duration = 10; % Segundos
f = 1000;

% Generar señal
t = 0:1/fs:duration;
sig = 0.707 * sin(2*pi*f*t); % -3 dBFS aprox (0.707 amplitud)

% Crear objetos de audio (Ajusta 'Device' según tu tarjeta)
devWriter = audioDeviceWriter('SampleRate',fs);
devReader = audioDeviceReader('SampleRate',fs, 'SamplesPerFrame', 2048);

disp('--- INICIANDO CALIBRACION ---');
disp('1. Ajusta GAIN en Pisound (Entrada) hasta que el LED de clip parpadee rojo y bájalo un pelín.');
disp('2. Ajusta VOLUME en Pisound (Salida) hasta que el valor RMS sea cercano a 0.5 (-6 dBFS).');
disp('   (Pulsa Ctrl+C para parar)');

setup(devWriter, sig.');
setup(devReader);

% Bucle de medida en tiempo real
tic;
while toc < duration*5 % 50 segundos máx
    % Reproducir
    devWriter(sig(1:2048).'); 
    
    % Grabar retorno
    rec = devReader();
    
    % Calcular nivel RMS
    nivel = rms(rec(:,1)); % Solo canal 1
    nivel_db = 20*log10(nivel + eps);
    
    fprintf('Nivel Retorno: %.3f ( %.1f dBFS ) \n', nivel, nivel_db);
    pause(0.1);
end
```

**Objetivo del ajuste**:
1.  **Entrada Pisound**: Sube hasta que sature (LED rojo) y baja un poco. Queremos maximizar la relación señal-ruido de entrada.
2.  **Salida Pisound**: Ajusta para que en la consola de MATLAB veas aproximadamente **-3.0 dBFS** (o el mismo valor que estás enviando).

---

## 📊 3. Midiendo la Calidad

Una vez calibrado (¡No toques los potenciómetros!), lanzamos las medidas.

### A. Medida de THD+N (Distorsión + Ruido)
Mide cuánto se "ensucia" una onda senoidal pura.

```matlab
% measure_thd.m
disp('Midiendo THD...');
fs = 48000;
t = 0:1/fs:5;
test_sig = sin(2*pi*1000*t); % Tono puro 1kHz

% Reproducir y Grabar simultáneamente
rec_sig = player_recorder_function(test_sig, fs); % (Función helper, ver abajo)

% Calcular THD
thd_value = thd(rec_sig, fs);
thd_percent = 10^(thd_value/20) * 100;

fprintf('THD medida: %.2f dB (%.4f %%)\n', thd_value, thd_percent);
% Visualizar espectro
figure; thd(rec_sig, fs); title('Espectro de THD de Pisound');
```

*(Nota: En un entorno real necesitarás usar `audioplayer` y `audiorecorder` o la toolbox de audio para sincronizar, pero la lógica es esta).*

### B. Respuesta en Frecuencia
¿Colorea el sonido? ¿Pierde graves o agudos?

```matlab
% measure_fr.m
% Generar Sweep Logarítmico (20Hz - 20kHz)
t = 0:1/fs:5;
sweep = chirp(t, 20, 5, 20000, 'logarithmic');

% Grabar paso por el sistema
resp = player_recorder_function(sweep, fs);

% Analizar
figure;
tfestimate(sweep, resp, [], [], [], fs);
title('Función de Transferencia (Respuesta en Frecuencia)');
```

---

## 📝 Resultados Esperados
La Pisound es una tarjeta de calidad (192kHz/24bit). Deberías obtener:
*   **THD+N**: < -90 dB (comúnmente alrededor de 0.005% o menos si la ganancia no está forzada).
*   **Respuesta**: Muy plana, quizás una ligera caída en graves (< 40Hz) si hay acoplos AC, y plana hasta >20kHz.

Si ves mucha distorsión (> 1%), revisa tus niveles: **probablemente estás saturando la entrada o la salida**. Vuelve a calibrar bajando los volúmenes.
