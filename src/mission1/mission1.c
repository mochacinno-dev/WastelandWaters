#include "../core/plastic_game.h"

/* ──────────────────────────────────────────────────
   MISIÓN 1 — Clasificación de Plásticos
   Drag & drop hacia los contenedores correctos.
   Trivia aparece DESPUÉS de completar el minijuego.
────────────────────────────────────────────────── */

#define M1_ITEM_COUNT 6
#define M1_BIN_COUNT  6

typedef struct {
    char  name[64];
    char  type[16];
    int   number;
    Color color;
    Rectangle rect;
    bool  placed;
    bool  dragging;
    Vector2 drag_offset;
    Vector2 origin;
} PlasticItem;

typedef struct {
    char  accepted_type[16];
    Rectangle rect;
    Color color;
    bool  correct_anim;
    float anim_timer;
} RecycleBin;

typedef struct {
    PlasticItem items[M1_ITEM_COUNT];
    RecycleBin  bins[M1_BIN_COUNT];
    int         placed_count;
    bool        minigame_done;  /* minijuego completado → mostrar trivia */
    bool        trivia_phase;
} Mission1State;

static Mission1State m1;

static void Mission1_Init(void) {
    memset(&m1, 0, sizeof(m1));

    const char *names[M1_ITEM_COUNT]  = {"Botella de agua","Envase detergente",
                                          "Tuberia","Bolsa supermercado",
                                          "Tapa de yogurt","Vaso de unicel"};
    const char *types[M1_ITEM_COUNT]  = {"PET","HDPE","PVC","LDPE","PP","PS"};
    int nums[M1_ITEM_COUNT]           = {1,2,3,4,5,6};
    Color colors[M1_ITEM_COUNT]       = {COL_PLASTIC_YEL, BLUE, ORANGE,
                                          GREEN, PURPLE, RED};

    /* ítems en columna izquierda */
    float start_x = 40, start_y = 140;
    for (int i = 0; i < M1_ITEM_COUNT; i++) {
        strncpy(m1.items[i].name,  names[i], 63);
        strncpy(m1.items[i].type,  types[i], 15);
        m1.items[i].number = nums[i];
        m1.items[i].color  = colors[i];
        m1.items[i].origin = (Vector2){start_x, start_y + i * 110.0f};
        m1.items[i].rect   = (Rectangle){start_x, start_y + i*110.0f, 170, 88};
    }

    /* contenedores: 2 filas × 3 columnas dentro del área del minijuego (GAME_W = 1080) */
    float bin_x = 260, bin_y = 80;
    Color bin_colors[M1_BIN_COUNT] = {COL_PLASTIC_YEL, BLUE, ORANGE, GREEN, PURPLE, RED};
    for (int i = 0; i < M1_BIN_COUNT; i++) {
        strncpy(m1.bins[i].accepted_type, types[i], 15);
        m1.bins[i].rect  = (Rectangle){bin_x + (i%3)*260.0f,
                                        bin_y + (i/3)*240.0f,
                                        220, 150};
        m1.bins[i].color = bin_colors[i];
    }

    TriviaManager_StartMission(MISSION_1_TIPOS_PLASTICO);
}

/* ── helper: botón Back ── */
static bool DrawBackButton(void) {
    Rectangle r = {GAME_W - 200, SCREEN_H - 60, 180, 44};
    return UI_Button(r, "Menu Principal", COL_UI_PANEL, WHITE);
}

static void Mission1_Update(float dt) {
    if (m1.trivia_phase) {
        TriviaManager_Update(dt);
        return;
    }

    Vector2 mouse = GetMousePosition();

    for (int i = 0; i < M1_ITEM_COUNT; i++) {
        PlasticItem *item = &m1.items[i];
        if (item->placed) continue;

        if (!item->dragging) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                CheckCollisionPointRec(mouse, item->rect)) {
                item->dragging = true;
                item->drag_offset.x = mouse.x - item->rect.x;
                item->drag_offset.y = mouse.y - item->rect.y;
            }
        } else {
            item->rect.x = mouse.x - item->drag_offset.x;
            item->rect.y = mouse.y - item->drag_offset.y;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                item->dragging = false;
                bool snapped = false;
                for (int b = 0; b < M1_BIN_COUNT; b++) {
                    if (CheckCollisionRecs(item->rect, m1.bins[b].rect)) {
                        if (strcmp(item->type, m1.bins[b].accepted_type) == 0) {
                            item->placed = true;
                            item->rect   = m1.bins[b].rect;
                            m1.bins[b].correct_anim = true;
                            m1.bins[b].anim_timer   = 0.0f;
                            m1.placed_count++;
                            if (m1.placed_count == M1_ITEM_COUNT)
                                m1.minigame_done = true;
                        } else {
                            item->rect.x = item->origin.x;
                            item->rect.y = item->origin.y;
                        }
                        snapped = true;
                        break;
                    }
                }
                if (!snapped) {
                    item->rect.x = item->origin.x;
                    item->rect.y = item->origin.y;
                }
            }
        }
    }

    for (int b = 0; b < M1_BIN_COUNT; b++) {
        if (m1.bins[b].correct_anim) {
            m1.bins[b].anim_timer += dt;
            if (m1.bins[b].anim_timer > 0.6f) m1.bins[b].correct_anim = false;
        }
    }

    /* transición a trivia */
    if (m1.minigame_done) m1.trivia_phase = true;
}

static void Mission1_DrawMinigame(void) {
    DrawText("MISION 1 — Clasifica los Plasticos", 20, 14, 26, COL_UI_ACCENT);
    DrawText("Arrastra cada plastico a su contenedor correcto", 20, 48, 20, LIGHTGRAY);

    /* contenedores */
    for (int b = 0; b < M1_BIN_COUNT; b++) {
        RecycleBin *bin = &m1.bins[b];
        Color c = bin->correct_anim
                    ? ColorAlpha(COL_CORRECT, 0.35f + 0.65f*(1.0f - bin->anim_timer/0.6f))
                    : ColorAlpha(bin->color, 0.22f);
        DrawRectangleRec(bin->rect, c);
        DrawRectangleLinesEx(bin->rect, 2.5f, bin->color);
        int lw = MeasureText(bin->accepted_type, 24);
        DrawText(bin->accepted_type,
                 (int)(bin->rect.x + bin->rect.width/2 - lw/2),
                 (int)(bin->rect.y + 10), 24, bin->color);
        char num_str[4];
        snprintf(num_str, sizeof(num_str), "#%d", b+1);
        int nw = MeasureText(num_str, 36);
        DrawText(num_str,
                 (int)(bin->rect.x + bin->rect.width/2 - nw/2),
                 (int)(bin->rect.y + 46), 36, WHITE);
    }

    /* ítems */
    for (int i = 0; i < M1_ITEM_COUNT; i++) {
        PlasticItem *item = &m1.items[i];
        if (item->placed) continue;
        Color c = ColorAlpha(item->color, item->dragging ? 0.95f : 0.72f);
        DrawRectangleRec(item->rect, c);
        DrawRectangleLinesEx(item->rect, item->dragging ? 3.5f : 2.0f, item->color);
        DrawText(item->name,  (int)(item->rect.x+8), (int)(item->rect.y+8), 16, WHITE);
        char nb[8];
        snprintf(nb, sizeof(nb), "#%d", item->number);
        DrawText(nb, (int)(item->rect.x+8), (int)(item->rect.y+32), 28, WHITE);
    }

    /* estado */
    char prog[40];
    snprintf(prog, sizeof(prog), "Colocados: %d / %d", m1.placed_count, M1_ITEM_COUNT);
    DrawText(prog, 20, SCREEN_H - 40, 22, LIGHTGRAY);

    if (m1.minigame_done) {
        DrawRectangle(0, 0, GAME_W, SCREEN_H, ColorAlpha(BLACK, 0.55f));
        const char *msg = "Clasificacion completa!";
        int mw = MeasureText(msg, 42);
        DrawText(msg, GAME_W/2 - mw/2, SCREEN_H/2 - 50, 42, COL_CORRECT);
        const char *msg2 = "Cargando preguntas...";
        int m2w = MeasureText(msg2, 24);
        DrawText(msg2, GAME_W/2 - m2w/2, SCREEN_H/2 + 14, 24, WHITE);
    }
}

static void Mission1_DrawTrivia(void) {
    /* fondo de instrucción */
    DrawRectangle(0, 0, GAME_W, SCREEN_H, COL_UI_BG);
    const char *inst = "MISION 1 — Trivia: Tipos de Plastico";
    int iw = MeasureText(inst, 28);
    DrawText(inst, GAME_W/2 - iw/2, 20, 28, COL_UI_ACCENT);

    /* panel de trivia centrado en el área del juego */
    TriviaManager_DrawPanel((Rectangle){(GAME_W - 820)/2.0f, 70, 820, SCREEN_H - 80});
}

static void Mission1_Draw(void) {
    ClearBackground(COL_UI_BG);

    if (!m1.trivia_phase) {
        Mission1_DrawMinigame();
        DrawBackButton();
    } else {
        Mission1_DrawTrivia();
    }
}

void Mission1_Run(void) {
    /* pantalla de introducción */
    bool go = ShowMissionIntro(1,
        "Tipos de Plastico",
        "La Gran Mancha de Plastico del Pacifico",
        "El Gran Parche de Basura del Pacifico tiene mas de 1.6 millones de km2 de extension "
        "y contiene mas de 80.000 toneladas de plastico. Pero, ¿sabias que no todo el plastico "
        "es igual? Existen 7 categorias principales de plasticos, cada una con propiedades y "
        "riesgos diferentes. Los tipos 1 (PET) y 2 (HDPE) son los mas reciclables; el tipo 6 "
        "(poliestireno, unicel) casi nunca se recicla y termina fragmentandose en microplasticos "
        "en el oceano. Aprende a identificarlos: el primer paso para reducir la contaminacion "
        "es saber exactamente que estas descartando.");
    if (!go) { g_current_scene = SCENE_MAIN_MENU; return; }

    Mission1_Init();
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Mission1_Update(dt);
        BeginDrawing();
        Mission1_Draw();
        EndDrawing();
        if (g_trivia.state == TRIVIA_MISSION_COMPLETE) break;
        /* botón menú (solo en fase minijuego) */
        if (!m1.trivia_phase && DrawBackButton()) {
            g_current_scene = SCENE_MAIN_MENU;
            return;
        }
    }
    g_current_scene = SCENE_MISSION_2;
}