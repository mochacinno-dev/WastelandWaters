#include "../core/plastic_game.h"

/* ──────────────────────────────────────────────────
   MISIÓN 2 — Cadena de Polímeros
────────────────────────────────────────────────── */

#define M2_CHALLENGE_COUNT  6
#define M2_CHAIN_LEN        5
#define M2_MONOMER_PALETTE  8

typedef struct {
    char polymer_name[64];
    char monomer[32];
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

static const PolymerChallenge s_challenges[M2_CHALLENGE_COUNT] = {
    {"Polietileno (PE)",  "Etileno",
     "Monomero: Etileno (C2H4). El polietileno es el plastico mas producido del mundo.",
     COL_PLASTIC_YEL},
    {"Polipropileno (PP)","Propileno",
     "Monomero: Propileno (C3H6). Usado en tapas, envases y fibras textiles.", BLUE},
    {"Poliestireno (PS)", "Estireno",
     "Monomero: Estireno. El unicel (EPS) es poliestireno expandido.", ORANGE},
    {"PVC",               "Cloruro de vinilo",
     "Monomero: Cloruro de vinilo. El Cl en su cadena lo hace dificil de reciclar.", RED},
    {"Nylon (PA6)",       "Caprolactama",
     "Monomero: Caprolactama. Es un polimero sintetico de alto rendimiento.", PURPLE},
    {"Celulosa (natural)","Glucosa",
     "Monomero: Glucosa. La celulosa es el polimero natural mas abundante en la Tierra.", GREEN},
};

static const char *s_all_monomers[M2_MONOMER_PALETTE] = {
    "Etileno","Propileno","Estireno","Cloruro de vinilo",
    "Caprolactama","Glucosa","Benceno","Acetileno"
};
static Color s_monomer_colors[M2_MONOMER_PALETTE] = {
    COL_PLASTIC_YEL, BLUE, ORANGE, RED, PURPLE, GREEN, SKYBLUE, PINK
};

static void Mission2_BuildPalette(void) {
    for (int i = 0; i < M2_MONOMER_PALETTE; i++) {
        strncpy(m2.palette[i].monomer_name, s_all_monomers[i], 31);
        m2.palette[i].color  = s_monomer_colors[i];
        float px = 30.0f + i * 120.0f;
        float py = SCREEN_H - 130.0f;
        m2.palette[i].origin = (Vector2){px, py};
        m2.palette[i].rect   = (Rectangle){px, py, 108, 72};
        m2.palette[i].dragging = false;
        m2.palette[i].used     = false;
    }
}

static void Mission2_BuildSlots(void) {
    float slot_w = 140.0f, slot_h = 90.0f;
    float start_x = (GAME_W - M2_CHAIN_LEN * (slot_w + 14)) / 2.0f;
    float start_y = 300.0f;
    for (int i = 0; i < M2_CHAIN_LEN; i++) {
        m2.slots[i].filled  = false;
        m2.slots[i].correct = false;
        m2.slots[i].content[0] = '\0';
        m2.slots[i].rect = (Rectangle){
            start_x + i * (slot_w + 14), start_y, slot_w, slot_h
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
    memcpy(m2.challenges, s_challenges, sizeof(s_challenges));
    Mission2_LoadChallenge(0);
    TriviaManager_StartMission(MISSION_2_POLIMEROS);
}

static void Mission2_CheckChain(void) {
    const char *correct = m2.challenges[m2.current_challenge].monomer;
    bool all_filled = true, all_correct = true;
    for (int i = 0; i < M2_CHAIN_LEN; i++) {
        if (!m2.slots[i].filled) { all_filled = false; break; }
        if (strcmp(m2.slots[i].content, correct) != 0) all_correct = false;
        m2.slots[i].correct = (strcmp(m2.slots[i].content, correct) == 0);
    }
    if (all_filled) {
        m2.check_triggered = true;
        m2.chain_correct   = all_correct;
        m2.feedback_timer  = 0.0f;
        strncpy(m2.fun_fact, m2.challenges[m2.current_challenge].hint, sizeof(m2.fun_fact)-1);
        m2.show_fact = true;
    }
}

static void Mission2_Update(float dt) {
    if (m2.trivia_phase) { TriviaManager_Update(dt); return; }

    Vector2 mouse = GetMousePosition();

    if (m2.check_triggered) {
        m2.feedback_timer += dt;
        if (m2.feedback_timer > 2.5f && m2.chain_correct) {
            int next = m2.current_challenge + 1;
            if (next < M2_CHALLENGE_COUNT)
                Mission2_LoadChallenge(next);
            else {
                m2.minigame_done = true;
                m2.trivia_phase  = true;
            }
        } else if (m2.feedback_timer > 2.0f && !m2.chain_correct) {
            for (int i = 0; i < M2_CHAIN_LEN; i++) { m2.slots[i].filled = false; m2.slots[i].content[0] = '\0'; }
            for (int i = 0; i < M2_MONOMER_PALETTE; i++) m2.palette[i].used = false;
            m2.check_triggered = false;
        }
        return;
    }

    for (int i = 0; i < M2_MONOMER_PALETTE; i++) {
        Monomer *mon = &m2.palette[i];
        if (mon->used) continue;
        if (!mon->dragging) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, mon->rect)) {
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
                    if (!m2.slots[s].filled && CheckCollisionRecs(mon->rect, m2.slots[s].rect)) {
                        strncpy(m2.slots[s].content, mon->monomer_name, 31);
                        m2.slots[s].filled = true;
                        mon->used = true;
                        dropped = true;
                        Mission2_CheckChain();
                        break;
                    }
                }
                if (!dropped) { mon->rect.x = mon->origin.x; mon->rect.y = mon->origin.y; }
            }
        }
    }
}

static void Mission2_DrawMinigame(void) {
    DrawText("MISION 2 — Cadena de Polimeros", 20, 14, 26, COL_UI_ACCENT);
    DrawText("Arrastra el monomero correcto a cada casillero de la cadena", 20, 48, 20, LIGHTGRAY);

    const PolymerChallenge *ch = &m2.challenges[m2.current_challenge];
    DrawText("Construye la cadena del polimero:", 20, 86, 20, WHITE);
    DrawText(ch->polymer_name, 20, 112, 32, ch->color);
    char prog[32];
    snprintf(prog, sizeof(prog), "Reto %d de %d", m2.current_challenge+1, M2_CHALLENGE_COUNT);
    DrawText(prog, 20, 154, 18, GRAY);

    /* cadena */
    for (int i = 0; i < M2_CHAIN_LEN; i++) {
        ChainSlot *s = &m2.slots[i];
        Color border = COL_UI_BORDER;
        Color bg     = COL_UI_PANEL;
        if (s->filled) {
            border = m2.check_triggered ? (s->correct ? COL_CORRECT : COL_WRONG) : ch->color;
            bg = ColorAlpha(border, 0.2f);
        }
        DrawRectangleRec(s->rect, bg);
        DrawRectangleLinesEx(s->rect, 2.5f, border);
        if (s->filled)
            UI_DrawWrappedText(s->content, (int)(s->rect.x+6), (int)(s->rect.y+10), (int)(s->rect.width-8), 16, WHITE);
        else {
            int qw = MeasureText("?", 28);
            DrawText("?", (int)(s->rect.x + s->rect.width/2 - qw/2),
                     (int)(s->rect.y + s->rect.height/2 - 14), 28, GRAY);
        }
        if (i < M2_CHAIN_LEN - 1) {
            float ax = s->rect.x + s->rect.width + 3;
            float ay = s->rect.y + s->rect.height/2;
            DrawText("->", (int)(ax), (int)(ay - 12), 20, GRAY);
        }
    }

    DrawLine(0, SCREEN_H - 160, GAME_W, SCREEN_H - 160, COL_UI_BORDER);
    DrawText("Monomeros disponibles:", 20, SCREEN_H - 152, 18, LIGHTGRAY);

    for (int i = 0; i < M2_MONOMER_PALETTE; i++) {
        Monomer *mon = &m2.palette[i];
        if (mon->used) continue;
        Color c = ColorAlpha(mon->color, mon->dragging ? 0.95f : 0.72f);
        DrawRectangleRec(mon->rect, c);
        DrawRectangleLinesEx(mon->rect, mon->dragging ? 3.0f : 1.5f, mon->color);
        UI_DrawWrappedText(mon->monomer_name, (int)(mon->rect.x+5), (int)(mon->rect.y+8), (int)(mon->rect.width-6), 14, WHITE);
    }

    if (m2.check_triggered) {
        const char *msg = m2.chain_correct ? "Cadena correcta!" : "Incorrecto, intentalo de nuevo";
        DrawText(msg, 20, 196, 22, m2.chain_correct ? COL_CORRECT : COL_WRONG);
        if (m2.show_fact)
            UI_DrawWrappedText(m2.fun_fact, 20, 226, GAME_W - 40, 16, LIGHTGRAY);
    }
}

static void Mission2_DrawTrivia(void) {
    DrawRectangle(0, 0, GAME_W, SCREEN_H, COL_UI_BG);
    const char *inst = "MISION 2 — Trivia: Polimeros";
    int iw = MeasureText(inst, 28);
    DrawText(inst, GAME_W/2 - iw/2, 20, 28, COL_UI_ACCENT);
    TriviaManager_DrawPanel((Rectangle){(GAME_W - 820)/2.0f, 70, 820, SCREEN_H - 80});
}

static void Mission2_Draw(void) {
    ClearBackground(COL_UI_BG);
    if (!m2.trivia_phase) {
        Mission2_DrawMinigame();
        Rectangle back = {GAME_W - 200, SCREEN_H - 60, 180, 44};
        UI_Button(back, "Menu Principal", COL_UI_PANEL, WHITE);
    } else {
        Mission2_DrawTrivia();
    }
}

void Mission2_Run(void) {
    bool go = ShowMissionIntro(2,
        "Cadena de Polimeros",
        "El plastico: una cadena interminable en el oceano",
        "Los plasticos son polimeros: largas cadenas de moleculas repetidas llamadas monomeros. "
        "Esta estructura molecular es precisamente lo que los hace tan duraderos... y tan "
        "peligrosos. Una botella de PET en el Gran Parche del Pacifico puede tardar hasta "
        "450 anos en degradarse, pero nunca desaparece del todo: se fragmenta en microplasticos "
        "que persisten indefinidamente. El Gran Parche contiene aproximadamente 1.8 billones "
        "de fragmentos plasticos. Entender la quimica del plastico nos ayuda a tomar mejores "
        "decisiones sobre cuales plasticos usar y cuales evitar.");
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
            if (CheckCollisionPointRec(mouse, back) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                g_current_scene = SCENE_MAIN_MENU; return;
            }
        }
    }
    g_current_scene = SCENE_MISSION_3;
}