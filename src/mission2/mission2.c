#include "../core/plastic_game.h"
#include "../core/locale.h"

#define M2_CHALLENGE_COUNT  6
#define M2_CHAIN_LEN        5
/* Paleta expandida: 5 copias del correcto + 3 distractores = 8 slots */
#define M2_MONOMER_PALETTE  8
#define M2_CORRECT_COPIES   5   /* cuántas copias del monómero correcto hay */
#define M2_DISTRACTOR_COUNT 3   /* cuántos distractores distintos hay */

typedef struct {
    char polymer_name[64];
    char monomer[32];       /* must match a palette entry exactly */
    char hint[256];
    Color color;
} PolymerChallenge;

typedef struct {
    char   monomer_name[32];
    Color  color;
    Rectangle rect;
    bool   dragging;
    Vector2 drag_offset;
    Vector2 origin;
    bool   used;
    bool   is_correct;  /* true si esta copia es del monómero correcto */
} Monomer;

typedef struct {
    char   content[32];
    Rectangle rect;
    bool   filled;
    bool   correct;
} ChainSlot;

typedef struct {
    PolymerChallenge challenges[M2_CHALLENGE_COUNT];
    int              current_challenge;
    ChainSlot  slots[M2_CHAIN_LEN];
    Monomer    palette[M2_MONOMER_PALETTE];
    bool    check_triggered;
    bool    chain_correct;
    float   feedback_timer;
    char    fun_fact[512];
    bool    show_fact;
    bool    minigame_done;
    bool    trivia_phase;
} Mission2State;

static Mission2State m2;

/* Colores fijos para los 3 distractores (índices 0-2 del array de distractores) */
static Color s_distractor_colors[3] = { RED, PURPLE, ORANGE };

/* Build challenges from locale strings */
static void Mission2_BuildChallenges(void) {
    struct { StrKey poly; StrKey mono; StrKey hint; Color col; } defs[M2_CHALLENGE_COUNT] = {
        { S_M2_CH0_POLY, S_M2_CH0_MONO, S_M2_CH0_HINT, COL_PLASTIC_YEL },
        { S_M2_CH1_POLY, S_M2_CH1_MONO, S_M2_CH1_HINT, BLUE             },
        { S_M2_CH2_POLY, S_M2_CH2_MONO, S_M2_CH2_HINT, ORANGE           },
        { S_M2_CH3_POLY, S_M2_CH3_MONO, S_M2_CH3_HINT, RED              },
        { S_M2_CH4_POLY, S_M2_CH4_MONO, S_M2_CH4_HINT, PURPLE           },
        { S_M2_CH5_POLY, S_M2_CH5_MONO, S_M2_CH5_HINT, GREEN            },
    };
    for (int i = 0; i < M2_CHALLENGE_COUNT; i++) {
        strncpy(m2.challenges[i].polymer_name, LOC(defs[i].poly), 63);
        strncpy(m2.challenges[i].monomer,      LOC(defs[i].mono), 31);
        strncpy(m2.challenges[i].hint,         LOC(defs[i].hint), 255);
        m2.challenges[i].color = defs[i].col;
    }
}

/*
 * Construye la paleta para el reto actual.
 *
 * Layout de los 8 slots:
 *   [0..4]  = 5 copias del monómero correcto  (color del polímero)
 *   [5..7]  = 3 distractores (monómeros de otros retos)
 *
 * Los distractores se eligen de los otros challenges para que tengan
 * sentido educativo y no sean completamente arbitrarios.
 */
static void Mission2_BuildPalette(void) {
    int cur = m2.current_challenge;
    const PolymerChallenge *ch = &m2.challenges[cur];

    /* Recoger 3 distractores: monómeros de otros challenges */
    const char *distractor_names[M2_DISTRACTOR_COUNT];
    int d_count = 0;
    for (int i = 0; i < M2_CHALLENGE_COUNT && d_count < M2_DISTRACTOR_COUNT; i++) {
        if (i == cur) continue;
        distractor_names[d_count++] = m2.challenges[i].monomer;
    }

    /* Calcular posiciones en dos filas de 4 para que quepan en GAME_W */
    float btn_w = 170.0f, btn_h = 64.0f;
    float gap_x = 18.0f, gap_y = 10.0f;
    float row_total = 4 * btn_w + 3 * gap_x;
    float start_x = (GAME_W - row_total) / 2.0f;
    float row1_y = SCREEN_H - 158.0f;
    float row2_y = row1_y + btn_h + gap_y;

    for (int i = 0; i < M2_MONOMER_PALETTE; i++) {
        int col = i % 4;
        int row = i / 4;
        float px = start_x + col * (btn_w + gap_x);
        float py = (row == 0) ? row1_y : row2_y;

        m2.palette[i].origin = (Vector2){px, py};
        m2.palette[i].rect   = (Rectangle){px, py, btn_w, btn_h};
        m2.palette[i].dragging = false;
        m2.palette[i].used     = false;

        if (i < M2_CORRECT_COPIES) {
            /* Copias del monómero correcto */
            strncpy(m2.palette[i].monomer_name, ch->monomer, 31);
            m2.palette[i].color      = ch->color;
            m2.palette[i].is_correct = true;
        } else {
            /* Distractores */
            int di = i - M2_CORRECT_COPIES;
            strncpy(m2.palette[i].monomer_name, distractor_names[di], 31);
            m2.palette[i].color      = s_distractor_colors[di];
            m2.palette[i].is_correct = false;
        }
    }
}

static void Mission2_BuildSlots(void) {
    float slot_w = 158.0f, slot_h = 86.0f;
    float gap    = 16.0f;
    float total  = M2_CHAIN_LEN * slot_w + (M2_CHAIN_LEN - 1) * gap;
    float start_x = (GAME_W - total) / 2.0f;
    float start_y = 290.0f;
    for (int i = 0; i < M2_CHAIN_LEN; i++) {
        m2.slots[i].filled  = false;
        m2.slots[i].correct = false;
        m2.slots[i].content[0] = '\0';
        m2.slots[i].rect = (Rectangle){
            start_x + i * (slot_w + gap), start_y, slot_w, slot_h
        };
    }
}

static void Mission2_LoadChallenge(int idx) {
    m2.current_challenge = idx;
    m2.check_triggered  = false;
    m2.chain_correct    = false;
    m2.feedback_timer   = 0.0f;
    m2.show_fact        = false;
    Mission2_BuildSlots();
    Mission2_BuildPalette();
}

static void Mission2_Init(void) {
    memset(&m2, 0, sizeof(m2));
    Mission2_BuildChallenges();
    Mission2_LoadChallenge(0);
    TriviaManager_StartMission(MISSION_2_POLIMEROS);
}

static void Mission2_CheckChain(void) {
    const char *correct = m2.challenges[m2.current_challenge].monomer;
    bool all_filled = true;
    bool all_correct = true;

    for (int i = 0; i < M2_CHAIN_LEN; i++) {
        if (!m2.slots[i].filled) {
            all_filled = false;
            break;
        }
        bool slot_ok = (strcmp(m2.slots[i].content, correct) == 0);
        m2.slots[i].correct = slot_ok;
        if (!slot_ok) all_correct = false;
    }

    if (all_filled) {
        m2.check_triggered = true;
        m2.chain_correct   = all_correct;
        m2.feedback_timer  = 0.0f;
        strncpy(m2.fun_fact, m2.challenges[m2.current_challenge].hint,
                sizeof(m2.fun_fact) - 1);
        m2.show_fact = true;
    }
}

static void Mission2_Update(float dt) {
    if (m2.trivia_phase) { TriviaManager_Update(dt); return; }

    Vector2 mouse = GetMousePosition();

    if (m2.check_triggered) {
        m2.feedback_timer += dt;

        if (m2.chain_correct && m2.feedback_timer > 2.5f) {
            int next = m2.current_challenge + 1;
            if (next < M2_CHALLENGE_COUNT) {
                Mission2_BuildChallenges();   /* re-read locale in case lang changed */
                Mission2_LoadChallenge(next);
            } else {
                m2.minigame_done = true;
                m2.trivia_phase  = true;
            }
        } else if (!m2.chain_correct && m2.feedback_timer > 2.0f) {
            /* Respuesta incorrecta: limpiar slots y devolver monómeros */
            for (int i = 0; i < M2_CHAIN_LEN; i++) {
                m2.slots[i].filled  = false;
                m2.slots[i].correct = false;
                m2.slots[i].content[0] = '\0';
            }
            for (int i = 0; i < M2_MONOMER_PALETTE; i++) {
                if (m2.palette[i].used) {
                    m2.palette[i].used       = false;
                    m2.palette[i].rect.x     = m2.palette[i].origin.x;
                    m2.palette[i].rect.y     = m2.palette[i].origin.y;
                }
            }
            m2.check_triggered = false;
        }
        return;
    }

    /* Drag & drop de monómeros */
    for (int i = 0; i < M2_MONOMER_PALETTE; i++) {
        Monomer *mon = &m2.palette[i];
        if (mon->used) continue;

        if (!mon->dragging) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                CheckCollisionPointRec(mouse, mon->rect)) {
                mon->dragging = true;
                mon->drag_offset.x = mouse.x - mon->rect.x;
                mon->drag_offset.y = mouse.y - mon->rect.y;
            }
        } else {
            mon->rect.x = mouse.x - mon->drag_offset.x;
            mon->rect.y = mouse.y - mon->drag_offset.y;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                mon->dragging = false;
                bool dropped = false;

                for (int s = 0; s < M2_CHAIN_LEN; s++) {
                    if (!m2.slots[s].filled &&
                        CheckCollisionRecs(mon->rect, m2.slots[s].rect)) {
                        strncpy(m2.slots[s].content, mon->monomer_name, 31);
                        m2.slots[s].filled = true;
                        mon->used  = true;
                        dropped    = true;
                        Mission2_CheckChain();
                        break;
                    }
                }

                if (!dropped) {
                    /* Devolver al origen */
                    mon->rect.x = mon->origin.x;
                    mon->rect.y = mon->origin.y;
                }
            }
        }
    }
}

/* ──────────────────────────────────────────────────
   DRAW
────────────────────────────────────────────────── */
static void Mission2_DrawMinigame(void) {
    DrawText(LOC(S_M2_TITLE), 20, 14, 26, COL_UI_ACCENT);
    DrawText(LOC(S_M2_INSTRUCTION), 20, 48, 20, LIGHTGRAY);

    const PolymerChallenge *ch = &m2.challenges[m2.current_challenge];
    DrawText(LOC(S_M2_BUILD_LABEL), 20, 86, 20, WHITE);
    DrawText(ch->polymer_name, 20, 112, 32, ch->color);

    char prog[32];
    snprintf(prog, sizeof(prog), LOC(S_M2_CHALLENGE_PROG),
             m2.current_challenge + 1, M2_CHALLENGE_COUNT);
    DrawText(prog, 20, 154, 18, GRAY);

    /* ── Chain slots ── */
    for (int i = 0; i < M2_CHAIN_LEN; i++) {
        ChainSlot *s = &m2.slots[i];
        Color border = COL_UI_BORDER;
        Color bg     = COL_UI_PANEL;

        if (s->filled) {
            border = m2.check_triggered
                        ? (s->correct ? COL_CORRECT : COL_WRONG)
                        : ch->color;
            bg = ColorAlpha(border, 0.20f);
        }

        DrawRectangleRec(s->rect, bg);
        DrawRectangleLinesEx(s->rect, 2.5f, border);

        if (s->filled) {
            UI_DrawWrappedText(s->content,
                               (int)(s->rect.x + 6), (int)(s->rect.y + 10),
                               (int)(s->rect.width - 8), 16, WHITE);
        } else {
            int qw = MeasureText("?", 28);
            DrawText("?",
                     (int)(s->rect.x + s->rect.width / 2 - qw / 2),
                     (int)(s->rect.y + s->rect.height / 2 - 14),
                     28, GRAY);
        }

        /* Flecha entre slots */
        if (i < M2_CHAIN_LEN - 1) {
            DrawText("->",
                     (int)(s->rect.x + s->rect.width + 3),
                     (int)(s->rect.y + s->rect.height / 2 - 12),
                     20, GRAY);
        }
    }

    /* ── Separador y leyenda de paleta ── */
    DrawLine(0, SCREEN_H - 168, GAME_W, SCREEN_H - 168, COL_UI_BORDER);
    DrawText(LOC(S_M2_MONOMERS_LABEL), 20, SCREEN_H - 160, 18, LIGHTGRAY);

    /* Indicador de cuántas copias correctas restan */
    int remaining_correct = 0;
    for (int i = 0; i < M2_MONOMER_PALETTE; i++)
        if (!m2.palette[i].used && m2.palette[i].is_correct) remaining_correct++;

    char copies_msg[48];
    snprintf(copies_msg, sizeof(copies_msg), "[%d/%d]",
             remaining_correct, M2_CORRECT_COPIES);
    DrawText(copies_msg, GAME_W - 80, SCREEN_H - 160, 16, ch->color);

    /* ── Paleta de monómeros ── */
    for (int i = 0; i < M2_MONOMER_PALETTE; i++) {
        Monomer *mon = &m2.palette[i];
        if (mon->used) continue;

        Color c = ColorAlpha(mon->color, mon->dragging ? 0.95f : 0.72f);
        DrawRectangleRec(mon->rect, c);
        DrawRectangleLinesEx(mon->rect,
                             mon->dragging ? 3.0f : 1.5f,
                             mon->is_correct
                                ? ColorAlpha(WHITE, 0.55f) /* borde sutil en correctos */
                                : mon->color);

        UI_DrawWrappedText(mon->monomer_name,
                           (int)(mon->rect.x + 5), (int)(mon->rect.y + 8),
                           (int)(mon->rect.width - 6), 14, WHITE);
    }

    /* ── Feedback tras verificar ── */
    if (m2.check_triggered) {
        const char *msg = m2.chain_correct ? LOC(S_M2_CHAIN_OK) : LOC(S_M2_CHAIN_WRONG);
        Color msg_col   = m2.chain_correct ? COL_CORRECT : COL_WRONG;
        DrawText(msg, 20, 196, 22, msg_col);
        if (m2.show_fact)
            UI_DrawWrappedText(m2.fun_fact, 20, 226, GAME_W - 40, 16, LIGHTGRAY);
    }
}

static void Mission2_DrawTrivia(void) {
    DrawRectangle(0, 0, GAME_W, SCREEN_H, COL_UI_BG);
    int iw = MeasureText(LOC(S_M2_TRIVIA_TITLE), 28);
    DrawText(LOC(S_M2_TRIVIA_TITLE), GAME_W / 2 - iw / 2, 20, 28, COL_UI_ACCENT);
    TriviaManager_DrawPanel((Rectangle){(GAME_W - 820) / 2.0f, 70, 820, SCREEN_H - 80});
}

static void Mission2_Draw(void) {
    ClearBackground(COL_UI_BG);
    if (!m2.trivia_phase) {
        Mission2_DrawMinigame();
        Rectangle back = {GAME_W - 200, SCREEN_H - 60, 180, 44};
        UI_Button(back, LOC(S_BTN_MENU_MAIN), COL_UI_PANEL, WHITE);
    } else {
        Mission2_DrawTrivia();
    }
}

void Mission2_Run(void) {
    bool go = ShowMissionIntro(2,
        LOC(S_M2_INTRO_TITLE),
        LOC(S_M2_INTRO_TOPIC),
        LOC(S_M2_INTRO_BODY));
    if (!go) { g_current_scene = SCENE_MAIN_MENU; return; }

    Mission2_Init();
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Mission2_Update(dt);
        BeginDrawing();
        Mission2_Draw();
        EndDrawing();

        if (g_trivia.state == TRIVIA_MISSION_COMPLETE) break;

        if (!m2.trivia_phase) {
            Rectangle back = {GAME_W - 200, SCREEN_H - 60, 180, 44};
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, back) &&
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                g_current_scene = SCENE_MAIN_MENU;
                return;
            }
        }
    }
    g_current_scene = SCENE_MISSION_3;
}