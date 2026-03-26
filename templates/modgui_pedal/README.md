# Guía de Implementación MODGUI (Estilo Pedal)

Esta carpeta contiene una plantilla para dotar a tus plugins LV2 de una interfaz gráfica visual (tipo "Pedal de Guitarra BOSS") compatible con sistemas como MODEP o Patchbox OS.

## 1. Entendiendo los Archivos
Antes de copiar nada, es importante entender para qué sirve cada pieza del puzzle:

### `modgui.ttl` (El Cerebro)
Es el archivo de definición. Su trabajo es **conectar** el mundo del código (puertos LV2) con el mundo visual (imágenes y HTML).
*   Define qué imagen usar de fondo.
*   Define qué control gráfico (rueda, interruptor) controla qué parámetro del plugin.

### `modgui/index.html` (El Esqueleto)
Es la estructura de la interfaz. En lugar de dibujar los controles con código, esta plantilla usa una **imagen de fondo** (tu diseño) y coloca encima controles invisibles "interactivos" en posiciones específicas.

### `modgui/theme.css` (El Estilo)
Define la apariencia estética. En esta plantilla es muy simple: solo le dice al sistema que el fondo sea transparente para que se vea tu imagen personalizada.

### `modgui/screenshot.png` y `thumbnail.png` (La Piel)
Son las imágenes que tú debes crear. `screenshot.png` es lo que se ve en la pantalla cuando usas el plugin.

---

## 2. Instrucciones de Uso Rápido

### Paso A: Copiar la Plantilla
Copia el archivo `modgui.ttl` y la carpeta `modgui` dentro del directorio `LV2` de tu proyecto.
Tu estructura debería quedar así:
```
miprojecto/
  └── LV2/
      ├── miprojecto.ttl
      ├── manifest.ttl
      ├── modgui.ttl       <-- NUEVO
      └── modgui/          <-- NUEVA CARPETA
            ├── index.html
            └── theme.css
```

### Paso B: Crear tus Imágenes
Necesitas diseñar dos imágenes y guardarlas en `modgui/`:

1.  **`screenshot.png`**: Esta será la cara de tu pedal.
    *   **Tamaño obligatorio**: 200px (ancho) x 380px (alto).
    *   **Diseño**: Incluye en la imagen el color del pedal, el logo, los textos ("Gain", "Volume") y el pad de goma inferior.
    *   **IMPORTANTE**: Debes diseñar sabiendo dónde aparecerán los controles interactivos:
        *   **Potenciómetro**: Su centro estará exactamente al **22.5%** de altura (pixel 85 aprox).
        *   **Interruptor**: Su centro estará al **74%** de altura (pixel 280 aprox).

2.  **`thumbnail.png`**: El icono para la librería.
    *   **Tamaño**: 280px x 280px.
    *   Pon tu pedal (escalado) centrado en un fondo transparente.

### Paso C: Editar `modgui.ttl`
Abre el archivo y modifica las líneas marcadas con comentarios:
1.  Cambia la URI (`<http://pisound.org/plugins/TU_PLUGIN>`) por la de tu plugin.
2.  En la sección `modgui:port`: asegúrate de que los `lv2:symbol` ("gain", "bypass") coinciden con los nombres de puerto que pusiste en tu archivo `.ttl` principal. Si tu volumen se llama "volume" en vez de "gain", cámbialo aquí.

### Paso D: Vincular y Compilar
1.  Abre `manifest.ttl` y añade: `rdfs:seeAlso <modgui.ttl> .`
2.  Abre `Makefile.lv2` y añade `modgui.ttl` y la carpeta `modgui/` a la lista de archivos que se copian en `install`.
3.  Despliega a la Raspberry Pi (`scp` y `make install`).

---

## 3. Personalización Avanzada (Tuning)

¿Quieres añadir más controles o moverlos? Tienes que tocar dos sitios:

1.  **En `modgui.ttl`**:
    Añade un nuevo bloque `modgui:port` para el nuevo control.
    ```ttl
    , [
        lv2:index 6 ;
        lv2:symbol "tone" ;
        modgui:name "Tone" ;
    ]
    ```

2.  **En `modgui/index.html`**:
    Añade un nuevo `div` para el control y asígnale una clase (ej: `.knob-tone`).
    ```html
    <div class="knob-tone">
        <div class="mod-knob" mod-role="input-control-port" mod-port-symbol="tone"></div>
    </div>
    ```

3.  **En el HTML (Estilos CSS)**:
    Busca la sección `<style>` y define la posición de tu nueva clase.
    ```css
    .knob-tone {
        position: absolute;
        transform: translate(-50%, -50%);
        left: 50%;       /* O la posición horizontal que quieras */
        top: 40%;        /* La altura que quieras */
        width: 64px; height: 64px;
    }
    ```
