# Guía Práctica 5: Verificación de Filtros Universales (VCF)

En esta práctica verificaremos que nuestra "pieza de LEGO" fundamental (el objeto `filter_core_sys`) funciona correctamente en sus 7 modos de operación.

A diferencia del Trémolo (que tenía parámetros fijos), aquí usaremos la nueva **App Universal** de consola, que nos permite cambiar parámetros en tiempo real escribiendo comandos.

---

## 🛠️ 1. Preparación del Entorno

### A. Estandarización del Proyecto
Asegúrate de que tu proyecto cumple con el nuevo estándar "Universal":
1.  Tu carpeta se llama `filters` (plural).
2.  Tu modelo se llama `filters.slx`.
3.  Has generado el código C (`slbuild('filters')`).
4.  Tienes el archivo `filters_compile_flags.txt` en la carpeta `common` (o en tu carpeta de proyecto, si el script lo busca ahí) definiendo tus parámetros.

### B. Compilación en la Raspberry Pi
1.  Transfiere las carpetas `common` (actualizada) y `filters` a la Pi.
2.  Accede y compila:

```bash
cd pisound/filters
../common/compile.sh filters
```

Si todo va bien, verás un mensaje: `Build SUCCESS: ./filters_app`

---

## 🎛️ 2. Uso de la Consola Universal

Ejecuta la aplicación:
```bash
./filters_app
```

Verás que el audio empieza a sonar. Ahora puedes controlar el filtro escribiendo comandos y pulsando **Enter**.

### Comandos Disponibles
| Comando | Descripción | Ejemplo |
| :--- | :--- | :--- |
| **Type** | Selecciona el tipo de filtro (1-7) | `Type=1` (Pone LowPass) |
| **Fc** | Frecuencia de corte/centro (Hz) | `Fc=500` |
| **Q** | Factor de calidad / Resonancia | `Q=2.0` |
| **Gain** | Ganancia en dB (solo para EQs) | `Gain=6.0` |
| **q** | Salir del programa | `q` |

*(Nota: Los nombres Type, Fc, Q, Gain dependen de cómo hayas configurado tu archivo `_compile_flags.txt`. Si no funcionan, revisa ese archivo).*

---

## 🧪 3. Ejercicios de Verificación

Realiza estos ejercicios para asegurar que tu código matemático funciona.

### 🔊 Fase 1: Ruido Blanco (White Noise)
Usa un generador de ruido blanco en la entrada (o un archivo mp3 de ruido).

#### Ejercicio 1: El Barrido LowPass
Comprobamos que el filtro realmente corta agudos.
1.  Escribe: `Type=1` (LowPass)
2.  Escribe: `Q=0.707` (Plano)
3.  Escribe: `Fc=10000` (El ruido suena casi completo)
4.  Baja la frecuencia: `Fc=1000` -> `Fc=500` -> `Fc=100`

**Resultado:** El sonido debe volverse oscuro y apagado progresivamente.

#### Ejercicio 2: La Resonancia
Comprobamos que la Q funciona.
1.  Pon `Fc=1000`.
2.  Sube la Q: `Q=4.0` o `Q=8.0`.

**Resultado:** Deberías oír un "silbido" o tono afinado en 1000 Hz sumado al ruido. Si mueves `Fc` ahora, el silbido debería moverse (efecto sintetizador clásico).

#### Ejercicio 3: BandPass y BandReject
1.  **BandPass (`Type=3`):** Con `Fc=1000`. Solo debe sonar una banda estrecha de ruido ("viento"). Lo grave y agudo desaparece.
2.  **BandReject (`Type=4`):** Con `Fc=1000` y `Q=5`. Debe sonar TODO el espectro EXCEPTO el tono de 1kHz. Si haces un barrido de Fc (`Fc=200`...`Fc=5000`) deberías oír un efecto "Phaser" o "Jet".

---

### 🎵 Fase 2: Música (Ecualización)
Pon una canción que conozcas bien. Vamos a probar los filtros de EQ (Tipos 5, 6, 7).

#### Ejercicio 4: Low Shelf (Graves)
1.  Escribe: `Type=5` (LowShelf).
2.  Configura: `Fc=200`, `Q=0.707`.
3.  Prueba:
    *   `Gain=0` (Sonido normal).
    *   `Gain=10` (Refuerzo masivo de graves/bombo).
    *   `Gain=-10` (Sonido delgado, sin fuerza).

#### Ejercicio 5: Central Shelf / Peak (Medios)
1.  Escribe: `Type=6` (CentralShelf/Peak).
2.  Configura: `Fc=1000`, `Q=1.0`.
3.  Prueba:
    *   `Gain=6` (Voz nasal, efecto teléfono antiguo).
    *   `Q=5` (Resonancia muy molesta en la voz).

#### Ejercicio 6: High Shelf (Agudos)
1.  Escribe: `Type=7` (HighShelf).
2.  Configura: `Fc=5000`.
3.  Prueba:
    *   `Gain=6` (Sonido brillante, "Aire").
    *   `Gain=-20` (Sonido opaco, "debajo del agua" pero distinto al LowPass).

---

## ✅ Criterio de Éxito
Si has podido distinguir auditivamente todos estos cambios, tu bloque `filters` está **perfectamente implementado** y listo para ser usado en proyectos más complejos.
