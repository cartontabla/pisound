# Guía Práctica 3: Añadiendo una Interfaz Gráfica (UI)

Hasta ahora, nuestro plugin LV2 tiene una "cara" genérica: una lista de faders y botones que el host (Reaper, Ardour, Jalv) dibuja automáticamente. Pero para una experiencia profesional, necesitamos nuestra propia interfaz.

En esta guía exploraremos cómo funciona el sistema de UI en LV2 y cómo usar **MODGUI**, el estándar visual para el ecosistema Raspberry Pi / MODEP.

---

## 🎨 1. Arquitectura: Separando Cerebro y Rostro

Lo más importante que hay que entender es que en LV2, **el Procesamiento de Audio (DSP) y la Interfaz Gráfica (UI) están completamente separados**.

*   **El DSP (Código C)**: Corre en un hilo de "Tiempo Real" con prioridad máxima. No puede esperar a que se dibuje un botón o se cargue una imagen. Si se detiene un microsegundo más de la cuenta, el audio "cruje" (xruns).
*   **La UI (Gráficos)**: Corre en el hilo principal del sistema operativo.

### ¿Cómo se comunican?
A través de **Puertos de Control**.
1.  Cuando mueves un fader en la UI, esta envía un mensaje al host.
2.  El host actualiza el valor en la memoria del plugin DSP.
3.  El plugin lee el nuevo valor en el siguiente ciclo de audio.

---

## 🖥️ 2. Tipos de UI en LV2

Existen varias formas de crear interfaces, pero para nuestro entorno nos interesan dos:

### A. UIs Nativas (GTK, Qt, X11)
Son programas clásicos de escritorio compilados en C/C++.
*   **Ventajas**: Máximo rendimiento y control.
*   **Desventajas**: Muy complejas de programar. Difíciles de portar (una UI hecha para Linux X11 no abre en Mac fácilmente sin librerías extra).

### B. MODGUI (Web Technologies) 🌟 *Recomendada*
Es el estándar usado por **MOD Devices** y **Patchbox OS (MODEP)**.
*   **Tecnología**: HTML5, CSS3 y JavaScript.
*   **Ventajas**:
    *   Diseñas como si fuera una página web.
    *   **Portabilidad total**: La interfaz se ve en el navegador web de tu ordenador cuando te conectas a la Raspberry Pi.
    *   Personalización visual infinita con CSS.

---

---

## 🛠️ 3. Implementando MODGUI: El "Kit de Construcción"

Una duda común es: **¿De dónde salen estos diseños? ¿Dónde están los archivos CSS?**

La respuesta es que **viven dentro de Patchbox OS**. El sistema (MOD host) inyecta automáticamente los estilos cuando carga tu plugin. Por eso, si abres tu archivo `index.html` en tu ordenador, se verá "roto" o vacío. **Solo se ve bien cuando está cargado en la Raspberry Pi**.

El flujo de trabajo es "ciego" pero seguro: confía en que si usas estas clases (`mod-knob`), aparecerán correctamente en la Pi.

### 3.1 Flujo de Trabajo Recomendado
1.  **Edita en PC**: Copia y pega los bloques HTML en tu archivo.
2.  **Sube a la Pi**: Copia la carpeta.
3.  **Visualiza en el Navegador**: Conéctate a `http://patchbox.local` y carga tu plugin. Ahí verás el resultado final.

> **Nota**: Existe una herramienta llamada `MODSDK` para previsualizar en el PC, pero requiere instalar Docker y es compleja para este curso. El método de "Subir y Refrescar" es más directo.

### 3.2 Estructura de Directorios

Dentro de tu plugin compilado (`bypass2.lv2`), debes crear una carpeta `modgui`:

```text
bypass2.lv2/
├── bypass2.ttl
├── ...
└── modgui/          <-- NUEVA CARPETA
    ├── index.html   (Tu lienzo)
    ├── style.css    (Ajustes de posición)
    ├── script.js    (Comportamiento opcional)
    └── icon.png     (Miniatura para el menú)
```

### 3.3 El ejemplo más Simple

Para no complicarnos, usaremos una plantilla básica. Copia este código en tu archivo `index.html`:

```html
<div class="mod-pedal mod-pedal-fender">
    <div class="mod-control-group clearfix">
        <!-- Un Knob para la Ganancia -->
        <div class="mod-knob" title="Gain">
            <div class="mod-knob-image" mod-role="input-control-port" mod-port-symbol="gain"></div>
            <span class="mod-knob-title">Gain</span>
        </div>

        <!-- Un Switch para el Bypass -->
        <div class="mod-switch" title="Bypass">
            <div class="mod-switch-image" mod-role="input-control-port" mod-port-symbol="bypass"></div>
            <span class="mod-switch-title">Bypass</span>
        </div>
    </div>
</div>
```

Esto creará un pedal plateado (estilo Fender) con un potenciómetro y un interruptor. ¡Sencillo y efectivo!

> **🧐 ¿Y la posición?**
> Los controles aparecerán en pantalla **en el mismo orden que en el HTML** (de izquierda a derecha).
> *   Si quieres cambiar su orden: corta y pega las líneas de código `<div>...</div>` para cambiarlas de sitio.
> *   Si quieres coordenadas exactas (ej. "el botón más arriba"): necesitas usar CSS en el archivo `style.css`.

### 3.4 Conectando todo (`manifest.ttl`)

Para que el sistema sepa que existe esta UI bonita, añade estas líneas al final de tu archivo `bypass2.ttl`:

```turtle
@prefix modgui: <http://moddevices.com/ns/modgui#> .

<http://pisound.org/plugins/bypass2>
    modgui:gui [
        modgui:resourcesDirectory <modgui> ;
        modgui:iconTemplate <modgui/icon.html> ;
        modgui:stylesheet <modgui/style.css> ;
        modgui:screenshot <modgui/screenshot.png> ;
        modgui:thumbnail <modgui/thumbnail.png> ;
    ] .
```

---

## 🚀 Resumen
Con estos pasos básicos ya tienes tu plugin funcionando con una interfaz profesional. Si quieres profundizar, añadir logos, cambiar colores o usar otros componentes, consulta el **Apéndice A**.

---

<div style="page-break-before:always"></div>

# APÉNDICE A: Kit de Construcción Avanzado

Aquí tienes los detalles técnicos para personalizar al máximo tu plugin.

## A.1 Catálogo de Componentes Visuales

Copia y pega estos bloques dentro de tu `div` principal en el HTML.

**Potenciómetros (Knobs)**
```html
<div class="mod-knob" title="Gain">
    <!-- 'mod-port-symbol' debe coincidir EXACTAMENTE con tu archivo .ttl -->
    <div class="mod-knob-image" mod-role="input-control-port" mod-port-symbol="gain"></div>
    <span class="mod-knob-title">Gain</span>
</div>
```

**Listas Desplegables**
```html
<div class="mod-enumerated" title="Waveform" mod-role="input-control-port" mod-port-symbol="Waveform">
    <div mod-role="input-control-value" mod-port-symbol="Waveform" class="mod-enumerated-selected"></div>
    <div class="mod-enumerated-list">
        <div mod-role="enumeration-option" mod-port-value="0">Seno</div>
        <div mod-role="enumeration-option" mod-port-value="1">Sierra</div>
    </div>
</div>
```

## A.2 Personalización (Colores y Logos)

Para cambiar el aspecto, editaremos el archivo `style.css`.

**1. Cambiar el Fondo (Color o Imagen)**
Añade una clase propia al div principal en el HTML (`<div class="mod-pedal mi-estilo">`) y define en el CSS:

```css
.mi-estilo {
    /* Opción A: Color sólido */
    background-color: #ff5733 !important;

    **Opción B: Imagen de Fondo (Skin)**
    Esta es la opción profesional. Úsala si buscas texturas realistas: **metal oxidado, pintura desgastada ("relic"), madera** o diseños complejos.
    1.  Consigue una imagen cuadrada (ej: 500x500px) con esa textura (busca "grunge green metal texture" en Google).
    2.  Guárdala como `fondo.png` en la carpeta `modgui`.
    3.  Usa este CSS:
    ```css
    .mod-pedal-custom {
        background-image: url('fondo.png') !important;
        background-size: cover; 
        background-repeat: no-repeat;
        /* Y si quieres redondear bordes: */
        border-radius: 10px;
        box-shadow: inset 0 0 20px rgba(0,0,0,0.5); /* Sombra interior para realismo */
    }
    ```

**2. Añadir un Logo**
Simplemente usa HTML estándar para poner tu imagen:
```html
<div class="mi-logo">
    <img src="/resources/bypass2.lv2/modgui/logo.png" width="100">
</div>
```

## A.3 Diseño Avanzado: Filas y Columnas

Si tienes muchos controles, querrás organizarlos en filas (ej: una fila de knobs arriba y los switches abajo).

El truco es crear varios bloques `mod-control-group`. Cada bloque es una "fila" nueva.

```html
<div class="mod-pedal mod-pedal-fender">
    
    <!-- FILA 1: Botones de arriba -->
    <div class="mod-control-group clearfix">
        <div class="mod-knob" title="Gain"> ... </div>
        <div class="mod-knob" title="Tone"> ... </div>
    </div>

    <!-- FILA 2: Interruptores abajo -->
    <div class="mod-control-group clearfix">
        <div class="mod-switch" title="Bypass"> ... </div>
    </div>

</div>
```
*Cada `<div>` con la clase `mod-control-group` se apilará debajo del anterior.*
