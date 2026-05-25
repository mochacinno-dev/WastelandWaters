# PlasticGame — Guía de implementación en C + Raylib

## Índice

1. [Estructura del proyecto](#1-estructura-del-proyecto)
2. [Requisitos previos](#2-requisitos-previos)
3. [Instalar Raylib](#3-instalar-raylib)
4. [Compilar y ejecutar](#4-compilar-y-ejecutar)
5. [Arquitectura: cómo se conectan los archivos](#5-arquitectura-cómo-se-conectan-los-archivos)
6. [Sistema Core — Trivia Engine](#6-sistema-core--trivia-engine)
7. [Misión 1 — Clasificación de Plásticos](#7-misión-1--clasificación-de-plásticos)
8. [Misión 2 — Cadena de Polímeros](#8-misión-2--cadena-de-polímeros)
9. [Misión 3 — Bloqueo de Microplásticos](#9-misión-3--bloqueo-de-microplásticos)
10. [Misión 4 — Mapa Interactivo de Países](#10-misión-4--mapa-interactivo-de-países)
11. [Misión 5 — Simulación de Corrientes](#11-misión-5--simulación-de-corrientes)
12. [Misión 6 — Banda Transportadora](#12-misión-6--banda-transportadora)
13. [Misión 7 — Exploración Naval](#13-misión-7--exploración-naval)
14. [Misión 8 — Circulación Termohalina](#14-misión-8--circulación-termohalina)
15. [Flujo general de una misión](#15-flujo-general-de-una-misión)
16. [Referencia de la API interna](#16-referencia-de-la-api-interna)
17. [Arte y sonido](#17-arte-y-sonido)
18. [Errores comunes](#18-errores-comunes)
19. [Personalizar y extender](#19-personalizar-y-extender)

---

## 1. Estructura del proyecto

``` bash
PlasticGame/
├── Makefile
├── README.md
└── src/
    ├── main.c                    ← Menú principal + bucle de escenas
    ├── core/
    │   ├── plastic_game.h        ← Tipos compartidos, colores, declaraciones
    │   └── trivia_manager.c      ← Motor de trivia + las 50 preguntas + UI
    ├── mission1/
    │   └── mission1.c            ← Drag & drop de plásticos a contenedores
    ├── mission2/
    │   └── mission2.c            ← Construcción de cadenas de polímeros
    ├── mission3/
    │   └── mission3.c            ← Tower defense: filtros contra microplásticos
    ├── mission4/
    │   └── mission4.c            ← Mapa interactivo de países contaminantes
    ├── mission5/
    │   └── mission5.c            ← Simulación de giros oceánicos y partículas
    ├── mission6/
    │   └── mission6.c            ← Banda transportadora de reciclaje
    ├── mission7/
    │   └── mission7.c            ← Barco recolector en el Gran Parche
    └── mission8/
        └── mission8.c            ← Sliders de temperatura/salinidad
```

A diferencia de Unity, **no hay Inspector ni ScriptableObjects**. Toda la configuración (datos de preguntas, parámetros de juego, colores) está en código, lo que hace el proyecto completamente portátil: un `make` y listo.

---

## 2. Requisitos previos

| Herramienta | Versión mínima | Notas |
|-------------|----------------|-------|
| GCC o Clang | cualquiera moderna | En Windows: MinGW-w64 |
| Raylib | 4.5 o superior | Ver sección 3 |
| Make | cualquiera | En Windows: `mingw32-make` |
| Sistema operativo | Linux / macOS / Windows | Probado en Arch Linux |

No se requiere ninguna dependencia adicional. Raylib incluye todo lo necesario (OpenGL, audio, entrada).

---

## 3. Instalar Raylib

### Arch Linux

```bash
# Opción A — paquete del sistema (más fácil)
sudo pacman -S raylib
```

### macOS

```bash
# Con Homebrew
brew install raylib
```

### Windows (MinGW-w64)

1. Descarga el instalador de Raylib desde [raylib.com](https://www.raylib.com)  
   o usa el paquete `w64devkit` que ya incluye Raylib.
2. Ajusta las rutas en el `Makefile`:

   ```makefile
   RAYLIB_INC = C:/raylib/include
   RAYLIB_LIB = C:/raylib/lib
   ```

### Verificar la instalación

```bash
echo '#include "raylib.h"
int main(){ InitWindow(200,200,"test"); CloseWindow(); }' > test.c
gcc test.c -lraylib -lGL -lm -o test && ./test
```

Si se abre una ventana en blanco, Raylib está correctamente instalado.

---

## 4. Compilar y ejecutar

```bash
# Clonar o descomprimir el proyecto
cd WastelandWaters

# Compilar todo (primera vez)
make

# Ejecutar directamente
make run

# Limpiar artefactos
make clean
```

El ejecutable queda en `build/WastelandWaters` (Linux/macOS) o `build/WastelandWaters.exe` (Windows).

### Compilar en modo Debug

```bash
make CFLAGS="-std=c99 -Wall -g -O0"
```

---

## 5. Arquitectura: cómo se conectan los archivos

``` bash
main.c
│
├── TriviaManager_Init()          ← inicializa el singleton global g_trivia
│
└── bucle principal
      │
      ├── SCENE_MAIN_MENU  → MainMenu_Run()
      ├── SCENE_MISSION_1  → Mission1_Run()
      ├── ...              → MissionX_Run()
      └── SCENE_RESULTS    → ResultsScreen_Run()

Cada MissionX_Run():
  ├── llama a TriviaManager_StartMission(MISSION_X)
  ├── tiene su propio bucle while(!WindowShouldClose())
  ├── llama a TriviaManager_Update(dt) en cada frame
  └── llama a TriviaManager_DrawPanel(rect) en cada frame
```

### Variables globales (definidas en `trivia_manager.c`)

```c
TriviaManager g_trivia;        // singleton del motor de trivia
SceneID       g_current_scene; // escena activa
```

Todas las misiones acceden a estas variables directamente a través del header `plastic_game.h`.

---

## 6. Sistema Core — Trivia Engine

### Archivo: `src/core/plastic_game.h` + `src/core/trivia_manager.c`

El motor de trivia es **completamente independiente** de cada minijuego. Funciona como un sistema de eventos: recibe respuestas del jugador y gestiona puntaje, racha y feedback.

### Ciclo de vida

``` bash
TriviaManager_Init()
      │
TriviaManager_StartMission(MissionID m)
      │   mezcla las 6 preguntas de la misión en orden aleatorio
      │
[por frame] TriviaManager_Update(dt)
      │   avanza el timer de feedback, cambia de pregunta
      │
[por frame] TriviaManager_DrawPanel(rect)
      │   dibuja pregunta, botones A/B/C/D, feedback, puntuación
      │   también captura los clics del jugador directamente
      │
[cuando el jugador hace clic] → TriviaManager_SubmitAnswer(index)
                                 calcula puntaje, activa feedback
      │
[después de 6 preguntas] g_trivia.state == TRIVIA_MISSION_COMPLETE
```

### Callbacks opcionales

Puedes conectar funciones propias para reaccionar a eventos de trivia:

```c
// En el Start() de tu misión:
g_trivia.on_answer_result = mi_funcion_feedback;
g_trivia.on_mission_complete = mi_funcion_siguiente_escena;

// Implementaciones:
void mi_funcion_feedback(bool correct, const char *explanation) {
    // reproducir sonido, mostrar efecto visual, etc.
    PlaySound(correct ? sfx_correct : sfx_wrong);
}

void mi_funcion_siguiente_escena(void) {
    g_current_scene = SCENE_MISSION_2;
}
```

### Estructura `Question`

```c
typedef struct {
    int         id;
    MissionID   mission;
    char        text[512];
    char        options[4][256];   // A, B, C, D
    int         correct_index;     // 0-3
    char        explanation[512];  // se muestra en el feedback
} Question;
```

### Posición del panel de trivia

`TriviaManager_DrawPanel()` recibe un `Rectangle` que indica dónde dibujar. Por convención, los minijuegos usan `{780, 0, 500, 720}` (el 40% derecho de la pantalla). Puedes cambiarlo a cualquier zona:

```c
// Panel pequeño en la esquina inferior
TriviaManager_DrawPanel((Rectangle){0, 500, 780, 220});

// Panel a pantalla completa (solo trivia, sin minijuego)
TriviaManager_DrawPanel((Rectangle){0, 0, 1280, 720});
```

---

## 7. Misión 1 — Clasificación de Plásticos

**Archivo:** `src/mission1/mission1.c`

### Mecánica

El jugador arrastra íconos de objetos plásticos (botella, tubería, bolsa...) a contenedores etiquetados con el tipo de plástico correcto (PET, HDPE, PVC, LDPE, PP, PS).

### Cómo funciona el drag & drop en Raylib

No hay sistema de física 2D ni Canvas como en Unity. El drag & drop se implementa manualmente:

```c
// Detectar inicio del arrastre
if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
    CheckCollisionPointRec(GetMousePosition(), item.rect)) {
    item.dragging = true;
    item.drag_offset = { mouse.x - item.rect.x, mouse.y - item.rect.y };
}

// Mover el ítem mientras se arrastra
if (item.dragging) {
    item.rect.x = mouse.x - item.drag_offset.x;
    item.rect.y = mouse.y - item.drag_offset.y;
}

// Soltar: verificar intersección con contenedor
if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && item.dragging) {
    item.dragging = false;
    if (CheckCollisionRecs(item.rect, bin.rect)) {
        // ¡clasificado!
    } else {
        // volver al origen
        item.rect = item.origin_rect;
    }
}
```

### Añadir más plásticos

En `mission1.c`, edita los arrays `names`, `types`, `nums` y `colors` al inicio de `Mission1_Init()`. El sistema acepta hasta `M1_ITEM_COUNT` ítems (definido en la cabecera de la función).

### Añadir sprites

Cuando tengas sprites, usa:

```c
Texture2D tex = LoadTexture("assets/pet_bottle.png");
DrawTextureEx(tex, (Vector2){item.rect.x, item.rect.y}, 0, scale, WHITE);
```

Carga las texturas en `Mission1_Init()` y libéralas con `UnloadTexture()` al terminar.

---

## 8. Misión 2 — Cadena de Polímeros

**Archivo:** `src/mission2/mission2.c`

### Mecánica del Nivel

Se muestran casilleros vacíos en fila (la cadena del polímero) y una paleta de monómeros en la parte inferior. El jugador arrastra el monómero correcto a los casilleros para completar la cadena.

### Añadir o editar retos

Los retos están en el array `s_challenges` al inicio del archivo:

```c
static const PolymerChallenge s_challenges[M2_CHALLENGE_COUNT] = {
    {
        "Polietileno (PE)",   // nombre del polímero
        "Etileno",            // monómero correcto (debe coincidir EXACTAMENTE
                              // con el nombre en s_all_monomers)
        "Dato curioso: ...",  // texto educativo
        COL_PLASTIC_YEL       // color del tema
    },
    // ...
};
```

El array `s_all_monomers` contiene todos los monómeros disponibles como distractores. Si añades un monómero nuevo al reto, agrégalo también aquí.

### Longitud de la cadena

`M2_CHAIN_LEN` (definido al inicio del archivo) controla cuántos casilleros debe rellenar el jugador. Por defecto es 5 (la cadena clásica –[M]–[M]–[M]–[M]–[M]–).

---

## 9. Misión 3 — Bloqueo de Microplásticos

**Archivo:** `src/mission3/mission3.c`

### Mecánica de la Misión

Tower defense simplificado. El jugador hace clic en el área del río para colocar filtros circulares. Las partículas de microplástico viajan por los waypoints hacia la "cadena alimentaria"; si un filtro las intersecta, quedan atrapadas.

### Ajustar la dificultad

```c
m3.spawn_interval     = 1.2f;   // segundos entre partículas (menor = más difícil)
m3.particles_per_wave = 15;     // partículas por ola
m3.total_waves        = 3;      // número de olas
m3.food_chain_hp      = 10;     // "vidas" de la cadena alimentaria
```

### Cambiar la ruta del río

Los waypoints definen el camino de las partículas. Edítalos en `Mission3_Init()`:

```c
m3.waypoints[0] = (Vector2){  20, 200 };
m3.waypoints[1] = (Vector2){ 200, 300 };
// ... añade más waypoints para una ruta más compleja
```

Puedes añadir hasta `M3_WAYPOINT_COUNT` waypoints (5 por defecto).

---

## 10. Misión 4 — Mapa Interactivo de Países

**Archivo:** `src/mission4/mission4.c`

### Mecánica (3 actividades secuenciales)

1. **Identificar región**: el jugador hace clic en un país asiático para confirmar que Asia es la mayor fuente de contaminación.
2. **Ranking**: el jugador arrastra las filas para ordenar los países de mayor a menor contaminador.
3. **Trazar río**: el jugador hace clic en los puntos naranjas del Yangtsé en orden.

### Ajustar posiciones de los pines en el mapa

Las posiciones son **normalizadas** (0.0 a 1.0), donde `(0,0)` es la esquina superior izquierda del área del mapa (`M4_MAP_X`, `M4_MAP_Y`). Si cambias la imagen del mapa, recalcula estas coordenadas:

```c
CountryPin cp[] = {
    {"China", "Asia", 1, 0.77f, 0.38f, RED, false},
    //                    ^^^^  ^^^^
    //                    norm_x  norm_y  (0-1)
};
```

Para convertir coordenadas geográficas reales a normalizadas en un mapa Mercator 2:1:

```bash
norm_x = (longitud + 180) / 360
norm_y = 1 - (latitud + 90) / 180   (invertido porque Y crece hacia abajo)
```

---

## 11. Misión 5 — Simulación de Corrientes

**Archivo:** `src/mission5/mission5.c`

### Mecánica

El jugador hace clic en el océano para colocar giros oceánicos. Las partículas de plástico orbitán en espiral hacia el centro del giro más cercano, simulando cómo el efecto Coriolis concentra el plástico.

### Dirección de giro (efecto Coriolis)

```c
g->northern = (mouse.y < M5_OCEAN_Y + M5_OCEAN_H / 2);
g->rotation_speed = g->northern ? 1.2f : -1.2f;
// positivo = horario (H. Norte), negativo = antihorario (H. Sur)
```

### Ajustar el comportamiento de las partículas

```c
// En Mission5_Update(), dentro del bloque "mover en espiral":
float new_r = dist - 20.0f * dt;  // velocidad de espiral hacia adentro
// Si quieres espirales más lentas: 20.0f → 8.0f
// Si quieres que el giro sea más rápido: 0.8f → 2.0f en rotation_speed
```

---

## 12. Misión 6 — Banda Transportadora

**Archivo:** `src/mission6/mission6.c`

### Mecánica

Un objeto plástico viaja de izquierda a derecha en una banda. Antes de que caiga por el extremo derecho, el jugador presiona uno de tres botones: **Mecánico**, **Químico** o **Desecho**.

### Añadir nuevos ítems

Edita el array `s_defs` al inicio del archivo:

```c
static const ConveyorItemDef s_defs[] = {
    {
        "Botella de agua PET",    // nombre visible
        "PET",                    // tipo de plástico
        PROCESS_MECANICO,         // proceso correcto
        "Explicación breve...",   // se muestra en el feedback
        COL_PLASTIC_YEL           // color del ítem
    },
    // ...
};
```

### Ajustar la velocidad y dificultad

```c
m6.spawn_interval = 3.5f;     // segundos entre ítems (menor = más difícil)
m6.item.speed = 100.0f + ...  // píxeles por segundo de movimiento
```

La barra de urgencia debajo de la banda muestra visualmente cuánto tiempo queda: verde → amarillo → rojo.

---

## 13. Misión 7 — Exploración Naval

**Archivo:** `src/mission7/mission7.c`

### Mecánica

Vista top-down. El jugador controla un barco con **WASD** o las **flechas** del teclado y debe recoger 60 fragmentos de plástico flotantes dentro del Gran Parche.

### Controles del barco

```
W / ↑   — avanzar (thrust +1)
S / ↓   — reversa (thrust -0.5)
A / ←   — girar a la izquierda
D / →   — girar a la derecha
```

El barco usa física simple con inercia y fricción (velocity × 0.92 por frame).

### Ajustar distribución del plástico

```c
#define M7_PATCH_CX  400.0f   // centro X del parche en el mundo
#define M7_PATCH_CY  350.0f   // centro Y
#define M7_PATCH_R   300.0f   // radio del parche
#define M7_GOAL_PIECES 60     // meta de recolección
```

La función `Mission7_SpawnDebris()` usa `sqrt(rand)` para concentrar más debris en el centro, simulando la acumulación real del parche.

### Cámara que sigue al barco

Se usa `Camera2D` de Raylib:

```c
m7.camera.target = (Vector2){m7.boat.x, m7.boat.y};
m7.camera.offset = (Vector2){SCREEN_W * 0.6f / 2, SCREEN_H / 2.0f};
m7.camera.zoom   = 1.0f;

BeginMode2D(m7.camera);
    // dibujar mundo
EndMode2D();
// dibujar HUD FUERA de BeginMode2D (coordenadas de pantalla)
```

---

## 14. Misión 8 — Circulación Termohalina

**Archivo:** `src/mission8/mission8.c`

### Mecánica

El jugador hace clic en una de las 5 zonas oceánicas para seleccionarla y luego mueve los sliders de temperatura y salinidad. Las partículas termohalinas fluyen a través de las zonas con velocidad proporcional a la fuerza de la circulación. Un botón "Simular deshielo polar" reduce la salinidad del Ártico y Antártico, debilitando la AMOC.

### Cálculo de densidad y circulación

```c
// mayor densidad = agua fría y salada = se hunde
float density = z->salinity * 0.6f + (1.0f - z->temp) * 0.4f;

// fuerza de la circulación = gradiente Ártico - Tropical
float gradient = density_arctic - density_tropical;
circulation_strength = gradient / 0.6f;  // normalizado 0-1
```

### Implementar sliders manuales en Raylib

Raylib no tiene widgets de UI nativos. Los sliders se implementan con un `Rectangle` como pista y detección de clic/arrastre:

```c
void DrawSlider(Rectangle track, float value, Color fill) {
    DrawRectangleRec(track, DARKGRAY);
    DrawRectangle(track.x, track.y, track.width * value, track.height, fill);
    // thumb (círculo en la posición del valor)
    float tx = track.x + track.width * value;
    DrawCircle(tx, track.y + track.height/2, 9, WHITE);
}
```

---

## 15. Flujo general de una misión

``` bash
MissionX_Run()
│
├── MissionX_Init()
│     ├── Inicializar estado local
│     └── TriviaManager_StartMission(MISSION_X)
│           └── mezcla las 6 preguntas de la misión
│
└── while (!WindowShouldClose())
      │
      ├── dt = GetFrameTime()
      │
      ├── MissionX_Update(dt)
      │     ├── lógica del minijuego (física, colisiones, estados)
      │     └── TriviaManager_Update(dt)
      │           └── avanza timer de feedback, cambia pregunta
      │
      ├── BeginDrawing()
      │     ClearBackground(COL_UI_BG)
      │     │
      │     ├── MissionX_Draw()
      │     │     └── dibuja el minijuego
      │     │
      │     └── TriviaManager_DrawPanel(rect)
      │           ├── dibuja pregunta y botones A/B/C/D
      │           └── captura clics → TriviaManager_SubmitAnswer()
      │
      └── EndDrawing()
            │
            └── if (g_trivia.state == TRIVIA_MISSION_COMPLETE) break
                  → sale del while → g_current_scene = SCENE_MISSION_X+1
```

La misión puede terminar por dos razones:

- **Trivia completada**: `g_trivia.state == TRIVIA_MISSION_COMPLETE` (las 6 preguntas respondidas)
- **Minijuego completado**: el minijuego tiene su propia condición de victoria y puede cambiar `g_current_scene` directamente

En la implementación actual, el bucle termina cuando se completa la trivia, independientemente del estado del minijuego. Esto te da flexibilidad: el jugador puede completar el minijuego antes o después de la trivia.

---

## 16. Referencia de la API interna

### `TriviaManager` (en `plastic_game.h` + `trivia_manager.c`)

| Función | Descripción |
|---|---|
| `TriviaManager_Init()` | Inicializa el singleton, carga las 50 preguntas. Llamar una vez al inicio. |
| `TriviaManager_StartMission(MissionID m)` | Inicia la trivia para la misión indicada. Mezcla sus 6 preguntas. |
| `TriviaManager_Update(float dt)` | Avanza el timer de feedback. Llamar cada frame. |
| `TriviaManager_DrawPanel(Rectangle r)` | Dibuja el panel completo. Captura clics. Llamar cada frame en BeginDrawing(). |
| `TriviaManager_SubmitAnswer(int idx)` | Envía una respuesta (0-3). Normalmente llamado internamente por DrawPanel. |
| `TriviaManager_CurrentQuestion()` | Devuelve puntero a la pregunta actual o NULL. |

### `UI_*` (en `trivia_manager.c`)

| Función | Descripción |
|---|---|
| `UI_Button(rect, label, bg, fg)` | Dibuja un botón y devuelve `true` si se hizo clic. |
| `UI_DrawWrappedText(text, x, y, w, size, color)` | Dibuja texto con salto de línea automático. |
| `UI_DrawPanel(r, bg, border, radius)` | Dibuja un panel redondeado. |

### Colores del juego (en `plastic_game.h`)

```c
COL_OCEAN         // azul oscuro del océano
COL_OCEAN_LIGHT   // azul más claro para efectos
COL_PLASTIC_RED   // rojo de plástico contaminante
COL_PLASTIC_GRN   // verde de reciclaje correcto
COL_PLASTIC_YEL   // amarillo de plástico degradado
COL_UI_BG         // fondo oscuro de la UI
COL_UI_PANEL      // panel de UI ligeramente más claro
COL_UI_BORDER     // bordes de paneles
COL_UI_ACCENT     // azul brillante para acentos
COL_CORRECT       // verde de respuesta correcta
COL_WRONG         // rojo de respuesta incorrecta
```

---

## 17. Arte y sonido

### Sprites (opcionales — el juego funciona sin ellos)

El juego actual usa formas primitivas (rectángulos, círculos, triángulos). Para añadir sprites:

```c
// Cargar (en _Init())
Texture2D tex_bottle = LoadTexture("assets/pet_bottle.png");

// Dibujar (en _Draw())
DrawTextureEx(tex_bottle,
              (Vector2){item.rect.x, item.rect.y},
              0.0f,     // rotación
              0.5f,     // escala
              WHITE);

// Liberar (al final del bucle o en un Unload())
UnloadTexture(tex_bottle);
```

### Sprites recomendados por misión

| Misión | Necesitas |
|---|---|
| 1 | 6 íconos de objetos plásticos + 6 íconos de contenedores |
| 2 | 8 círculos de color con etiqueta (los actuales ya funcionan) |
| 3 | Fondo de río + ícono de microplástico + ícono de filtro |
| 4 | Imagen de mapa mundial (PNG, proporción 2:1) |
| 5 | Fondo de océano con textura de agua |
| 6 | Fondo de banda transportadora + 8 íconos de objetos |
| 7 | Sprite de barco top-down + sprites de fragmentos |
| 8 | Fondos para las 5 zonas oceánicas |

### Sonido

```c
// En main() después de InitWindow():
InitAudioDevice();

// Cargar efectos:
Sound sfx_correct = LoadSound("assets/correct.wav");
Sound sfx_wrong   = LoadSound("assets/wrong.wav");
Music bgm         = LoadMusicStream("assets/ocean_ambient.ogg");

// Reproducir:
PlaySound(sfx_correct);
PlayMusicStream(bgm);
UpdateMusicStream(bgm); // llamar cada frame

// Liberar:
UnloadSound(sfx_correct);
UnloadMusicStream(bgm);
CloseAudioDevice();
```

### Fuentes personalizadas

```c
Font font = LoadFont("assets/mi_fuente.ttf");
DrawTextEx(font, "Texto bonito", (Vector2){x, y}, font_size, spacing, WHITE);
UnloadFont(font);
```

---

## 18. Errores comunes

**`undefined reference to 'TriviaManager_Init'`**  
→ Asegúrate de que `src/core/trivia_manager.c` está en la lista `SRCS` del `Makefile`.

**La ventana abre y se cierra inmediatamente**  
→ Raylib no encontró la librería en tiempo de ejecución. En Linux: `sudo ldconfig`. En macOS: verifica que `brew install raylib` se completó sin errores.

**El drag & drop no funciona**  
→ Verifica que estás llamando `GetMousePosition()` fuera de `BeginMode2D()` para coordenadas de pantalla, o dentro si usas coordenadas de mundo. En las misiones 1, 2, 3, 4 y 6 todo es espacio de pantalla; en la 7 el barco usa `Camera2D`.

**Las partículas de la Misión 5 no orbitan**  
→ El giro no está dentro del área del océano definida. Revisa que el clic está entre `M5_OCEAN_X` y `M5_OCEAN_X + M5_OCEAN_W`.

**El mapa de la Misión 4 no muestra los pines en el lugar correcto**  
→ Los pines usan coordenadas normalizadas. Edita `norm_x` y `norm_y` en el array `cp[]` dentro de `Mission4_Init()`. Recuerda que `(0,0)` es la esquina superior izquierda del área del mapa.

**El barco de la Misión 7 no responde a los controles**  
→ Verifica que el foco de la ventana está en el juego. En Linux con gestores de ventanas tiling, a veces hay que hacer clic una vez en la ventana.

**`TriviaManager_DrawPanel` dibuja fuera de pantalla**  
→ El `Rectangle` que le pasas está fuera de `{0, 0, SCREEN_W, SCREEN_H}`. La resolución por defecto es 1280×720, definida en `plastic_game.h`.

**Compilación en Windows: `undefined reference to WinMain`**  
→ Asegúrate de que `CFLAGS_EXTRA = -mwindows` está en el `Makefile` para la plataforma Windows.

---

## 19. Personalizar y extender

### Cambiar la resolución

En `plastic_game.h`:

```c
#define SCREEN_W  1920
#define SCREEN_H  1080
```

Luego ajusta las posiciones hardcodeadas en cada misión (busca valores como `780`, `500`, `760`).

### Añadir más preguntas a una misión

En `trivia_manager.c`, el banco de preguntas `s_question_bank` tiene 50 entradas (6 por misión + algunas de las primeras). Para añadir más:

1. Aumenta `QUESTIONS_PER_MISSION` en `plastic_game.h` (de 6 a 8, por ejemplo).
2. Añade nuevas entradas al array, con el `MissionID` correcto.
3. El sistema las mezclará y seleccionará automáticamente.

### Añadir una nueva misión

1. Crea `src/missionN/missionN.c` con una función `MissionN_Run(void)`.
2. Añade `MISSION_N` a la enumeración `MissionID` en `plastic_game.h`.
3. Añade `SCENE_MISSION_N` a `SceneID`.
4. Añade el `case` en el `switch` de `main.c`.
5. Añade las preguntas al banco en `trivia_manager.c` con `mission = MISSION_N`.
6. Añade `src/missionN/missionN.c` a `SRCS` en el `Makefile`.

### Guardar progreso (persistencia)

Raylib no tiene sistema de guardado. Usa la librería estándar de C:

```c
void SaveProgress(void) {
    FILE *f = fopen("save.dat", "wb");
    if (f) {
        fwrite(&g_trivia.score, sizeof(int), 1, f);
        fwrite(&g_current_scene, sizeof(SceneID), 1, f);
        fclose(f);
    }
}

void LoadProgress(void) {
    FILE *f = fopen("save.dat", "rb");
    if (f) {
        fread(&g_trivia.score, sizeof(int), 1, f);
        fread(&g_current_scene, sizeof(SceneID), 1, f);
        fclose(f);
    }
}
```
