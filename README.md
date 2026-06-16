# Wasteland Waters

> **Un viaje educativo por la crisis de las islas de plástico** / *An educational journey through the plastic island crisis*

Juego educativo desarrollado en **C + Raylib** sobre la contaminación plástica oceánica, con foco en la Gran Mancha del Pacífico. Incluye 8 misiones interactivas con mecánicas distintas y un motor de trivia bilingual (ES/EN) con 50 preguntas.

---

## Contenido

- [Misiones](#misiones)
- [Arquitectura del código](#arquitectura-del-código)
- [Compilar en Arch Linux](#compilar-en-arch-linux)
- [Compilar para Windows (cross-compile)](#compilar-para-windows-desde-linux)
- [Compilar para WebAssembly](#compilar-para-webassembly)
- [Controles](#controles)
- [Estado del proyecto](#estado-del-proyecto)

---

## Misiones

| # | Título | Mecánica |
|---|--------|----------|
| 1 | **Tipos de Plástico** | Drag & drop: clasifica objetos en contenedores PET / HDPE / PVC / LDPE / PP / PS |
| 2 | **Cadena de Polímeros** | Arrastra el monómero correcto a los 5 casilleros de la cadena (6 retos, 8 distractores) |
| 3 | **Bloqueo de Microplásticos** | Tower defense: coloca filtros para interceptar partículas en 3 oleadas |
| 4 | **Países Contaminantes** | Mapa interactivo: identifica región → ordena ranking → traza el río Yangtsé |
| 5 | **Corrientes Superficiales** | Coloca giros oceánicos con efecto Coriolis para atrapar el 60% del plástico |
| 6 | **Reciclaje** | Banda transportadora: clasifica 15 piezas (mecánico / químico / desecho) antes de que caigan |
| 7 | **Islas de Plástico** | Top-down: navega con WASD y recoge 60 fragmentos en el Gran Parche |
| 8 | **Circulación Termohalina** | Sliders de temperatura/salinidad en 5 zonas oceánicas; simula el colapso de la AMOC |

Cada misión incluye una **pantalla de introducción** con contexto educativo real y concluye con **6 preguntas de trivia** aleatorias del banco de la misión. El puntaje es acumulativo entre las 8 misiones y se muestran rachas.

---

## Arquitectura del código

```
WastelandWaters/
├── Makefile
├── build_web.sh              ← Script para compilar a WebAssembly (Emscripten)
├── devguide.md               ← Guía técnica detallada para desarrolladores
└── src/
    ├── core/
    │   ├── plastic_game.h    ← Tipos, colores, constantes, declaraciones globales
    │   ├── main.c            ← Menú principal, bucle de escenas, toggle de idioma
    │   ├── trivia_manager.c  ← Motor de trivia (50 preguntas ES+EN), panel de UI, intro
    │   ├── locale.h          ← Enumeración de 200+ claves de cadenas
    │   └── locale.c          ← Tablas de cadenas ES e EN
    ├── mission1/mission1.c
    ├── mission2/mission2.c
    ├── mission3/mission3.c
    ├── mission4/mission4.c
    ├── mission5/mission5.c
    ├── mission6/mission6.c
    ├── mission7/mission7.c
    └── mission8/mission8.c
```

Las misiones son completamente independientes entre sí. Cada una expone una sola función pública `MissionX_Run()` y se comunica con el resto del juego únicamente a través de `g_current_scene` y `g_trivia` (singletons definidos en `trivia_manager.c`).

---

## Compilar en Arch Linux

### 1. Dependencias

```bash
sudo pacman -S --needed gcc make raylib
```

> Si `raylib` no está en los repos oficiales de tu mirror, está disponible en AUR:
>
> ```bash
> yay -S raylib
> ```

### 2. Compilar y ejecutar

```bash
git clone https://github.com/mochacinno-dev/WastelandWaters.git
cd WastelandWaters

make          # compila todo en build/WastelandWaters
make run      # compila y ejecuta directamente
make clean    # elimina la carpeta build/
```

El ejecutable queda en `build/WastelandWaters`.

### 3. Compilar en modo debug

```bash
make CFLAGS="-std=c99 -Wall -Wextra -g -O0"
```

Luego puedes adjuntar `gdb` o usar la configuración incluida en `.vscode/launch.json` con la extensión *C/C++ Runner*.

---

## Compilar para Windows desde Linux

Requiere el cross-compiler MinGW-w64 y las binarias de Raylib para Windows:

```bash
# Instalar MinGW-w64
sudo pacman -S --needed mingw-w64-gcc

# Descargar Raylib para Windows (solo la primera vez)
mkdir -p /tmp/raylib-win
curl -L https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_win64_mingw-w64.zip \
     -o /tmp/raylib-win/raylib.zip
unzip /tmp/raylib-win/raylib.zip -d /tmp/raylib-win/

# Compilar
make win
```

El resultado es `build/win/WastelandWaters.exe`. Para probarlo en Linux:

```bash
sudo pacman -S --needed wine
wine build/win/WastelandWaters.exe
```

No se necesitan DLLs adicionales (las dependencias se enlazan estáticamente con `-static`).

---

## Compilar para WebAssembly

El script `build_web.sh` automatiza todo el proceso: descarga Raylib, lo compila para WASM y genera el bundle listo para servir.

### Dependencias

```bash
# Emscripten
sudo pacman -S --needed emscripten
source /etc/profile.d/emscripten.sh   # o abre una terminal nueva
```

### Uso

```bash
chmod +x build_web.sh

./build_web.sh            # genera build/web/WastelandWaters.html
./build_web.sh --serve    # también lanza un servidor local en :8080
```

El flag `--serve` inicia `python3 -m http.server 8080`. Abre luego:

```
http://localhost:8080/WastelandWaters.html
```

> Los navegadores bloquean `SharedArrayBuffer` en `file://`, por eso se necesita el servidor HTTP incluso para probar localmente.

El primer `./build_web.sh` tarda varios minutos porque compila Raylib desde fuentes para WASM. Las siguientes ejecuciones reutilizan la librería ya compilada en `/tmp/raylib-wasm/`.

---

## Controles

| Acción | Tecla |
|--------|-------|
| Navegar el menú | Clic |
| Drag & drop (misiones 1, 2, 4, 6) | Clic sostenido + arrastrar |
| Mover el barco (misión 7) | `W A S D` o flechas |
| Colocar objeto (misiones 3, 5) | Clic izquierdo |
| Mover sliders (misión 8) | Clic sostenido + arrastrar |
| Responder trivia | Clic en la opción |
| Cambiar idioma (ES ↔ EN) | Botones en esquina superior derecha |
| Volver al menú | Botón "Menú Principal" |

---

## Estado del proyecto

### Completado

- [x] Motor de trivia con 50 preguntas bilingüe (ES/EN)
- [x] Sistema de idiomas (`LOC()`) para todos los textos del juego
- [x] Pantallas de introducción por misión con texto educativo real
- [x] 8 misiones con mecánicas distintas
- [x] Sistema de puntaje acumulativo y rachas
- [x] Compilación para Linux, Windows (cross-compile MinGW) y WebAssembly
- [x] Soporte para resolución 1600×900 con área de juego 1080px + trivia 520px
- [x] Mejora de fuente, colores, accesibilidad y tiempo de feedback
- [x] Pantallas de explicación educativa (intro por misión)
- [x] Corrección ortográfica (RAE)
- [x] Trailer del juego
- [x] Pantalla de presentación y tutorial interactivo
- [x] Sprites / ilustraciones (el juego funciona sin ellos con formas primitivas)

### Pendiente

- [ ] Manual general de uso (PDF)
- [ ] Música y efectos de sonido (`InitAudioDevice` ya está reservado)
- [ ] Pantalla de créditos y licencia

---

## Guía de desarrollo

Para añadir preguntas, nuevas misiones, sprites o sonido, consulta **[devguide.md](devguide.md)**, que incluye:

- Referencia completa de la API interna (`TriviaManager`, `UI_*`, callbacks)
- Cómo implementar drag & drop, cámaras 2D y sliders en Raylib
- Paleta de colores del juego (`COL_OCEAN`, `COL_PLASTIC_*`, etc.)
- Errores comunes y sus soluciones
- Instrucciones para agregar una misión nueva desde cero

---

## Créditos

Desarrollado por **Lovebound Games** como herramienta educativa sobre contaminación plástica oceánica, con énfasis en la Gran Mancha de Plástico del Pacífico Norte.

Construido con [Raylib](https://www.raylib.com/) — una biblioteca de videojuegos simple y fácil de usar, escrita en C.