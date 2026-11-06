# LOTERÍA ZERO

**La tradición en tus manos.**

Aplicación nativa (`.fap`) para **Flipper Zero** que convierte el dispositivo en un **cantor digital** de la Lotería Mexicana tradicional de 54 cartas.

Identidad visual: tradición mexicana + estética retro + minimalismo monocromo Flipper.

---

## Una dedicatoria especial

# Para Antonella

La inspiración detrás de cada línea de código, cada idea y cada proyecto.

Porque las mejores creaciones nacen de aquello que más queremos.

---

## Características

- Catálogo completo de las **54 cartas** tradicionales
- **Rimas originales** al estilo de pregón familiar
- Ilustraciones **1-bit** originales optimizadas para 128×64
- Mezcla **Fisher-Yates** sin repeticiones
- Vista de carta en **orientación vertical** (gira el Flipper)
- Historial de cantadas / pendientes
- Estadísticas persistentes
- Modo automático (5 / 10 / 15 s o manual)
- Bienvenida dedicada (omitible tras la primera vez)
- Sección **Nuestra historia**

## Arquitectura

`cards.json` es la **única fuente de verdad** del catálogo. En compilación se genera código C estático (`src/cards_data.c`) para evitar parseo JSON en el dispositivo (RAM y dependencias).

| Módulo | Rol |
|--------|-----|
| `cards.json` + `tools/gen_cards.py` | Catálogo y validación |
| `tools/gen_card_art.py` | Ilustraciones 1-bit |
| `src/game.c` | Motor / shuffle / progreso |
| `src/renderer.c` | Dibujo de carta vertical |
| `src/storage.c` | Persistencia (`saved_struct`) |
| `src/scenes/*` | Navegación SceneManager |
| `src/loteria_zero.c` | Entry point FAP |

### Orientación de pantalla

Se eligió la **alternativa A híbrida**:

- Menús en horizontal nativo (128×64)
- Partida con `ViewOrientationVertical` (canvas lógico 64×128)

La pantalla física no cambia de tamaño: el usuario gira el Flipper (D-pad abajo). El firmware remapea los botones oficialmente.

## Estructura

```
loteria-zero/
├── application.fam
├── cards.json
├── loteria_zero.png
├── README.md
├── images/                 # 54 PNG 1-bit (+ iconos)
├── files/cards.json        # asset empaquetado en el FAP
├── src/
│   ├── loteria_zero.c/.h
│   ├── cards_data.c/.h     # generado
│   ├── game.c/.h
│   ├── renderer.c/.h
│   ├── storage.c/.h
│   └── scenes/
├── tools/
│   ├── gen_cards.py
│   └── gen_card_art.py
├── tests/
└── dist/loteria_zero.fap   # tras compilar
```

## Requisitos

- Python 3.8+
- [uFBT](https://github.com/flipperdevices/flipperzero-ufbt) (`pip install --upgrade ufbt`)
- Pillow (para regenerar arte): `pip install pillow`
- Firmware Flipper compatible con el SDK de uFBT (probado con **release 1.5.1 / API 88.2**)

## Compilación

```bash
# Instalar herramienta
python3 -m pip install --upgrade ufbt pillow

# Regenerar catálogo C e ilustraciones (si cambias cards.json)
python3 tools/gen_card_art.py
python3 tools/gen_cards.py --json cards.json --out-h src/cards_data.h --out-c src/cards_data.c --images images

# Compilar FAP
ufbt
```

El artefacto queda en:

`dist/loteria_zero.fap`

## Instalación en Flipper Zero

1. Conecta el Flipper por USB
2. `ufbt launch` (compila, sube y ejecuta), **o**
3. Copia `dist/loteria_zero.fap` a `SD Card/apps/Games/` con qFlipper
4. Abre **Apps → Games → Loteria Zero**

Si hay mismatch de API/SDK: `ufbt flash_usb` (actualiza firmware al canal del SDK).

## Controles

### Menús
| Botón | Acción |
|-------|--------|
| UP / DOWN | Navegar |
| OK | Seleccionar |
| BACK | Volver / salir |

### Partida (Flipper en vertical)
| Botón | Acción |
|-------|--------|
| OK | Cantar siguiente carta |
| LEFT | Ver carta anterior |
| RIGHT | Volver a la carta actual |
| UP | Pausar / reanudar |
| DOWN | Pausar modo auto |
| BACK | Guardar y menú |

## Configuración

- **Modo auto**: Manual, 5s, 10s, 15s
- **Omitir bienvenida**: tras la primera ejecución

## Catálogo

Las 54 cartas tradicionales (El Gallo … La Rana), cada una con:

- `id`, `name`, `image`, `rhyme`

Edita `cards.json` y regenera con los scripts de `tools/`.

## Pruebas

```bash
python3 -m unittest tests/test_catalog.py -v
```

Valida: 54 cartas, IDs únicos, rimas, ilustraciones, orden tradicional, shuffle sin duplicados y dedicación en el README.

## Limitaciones técnicas

1. Orientación vertical solo en capa fullscreen (restricción del GUI del firmware).
2. Fuentes del Flipper pueden no dibujar todos los glifos acentuados; el texto canónico completo vive en este README y en `Nuestra historia`.
3. No hay audio de voz: el “canto” es visual (nombre + rima) con vibración al revelar.

## Créditos

- Tradición: Lotería Mexicana popular
- Plataforma: Flipper Zero Official Firmware SDK / uFBT
- Proyecto: **Lotería Zero**

## Una dedicatoria especial

# Para Antonella

La inspiración detrás de cada línea de código, cada idea y cada proyecto.

Porque las mejores creaciones nacen de aquello que más queremos.
