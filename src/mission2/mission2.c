#include "../core/plastic_game.h"
#include "../core/locale.h"

#define M2_CHALLENGE_COUNT  6
#define M2_CHAIN_LEN        5
#define M2_MONOMER_PALETTE  8

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

static Color s_monomer_colors[M2_MONOMER_PALETTE] = {
    COL_PLASTIC_YEL, BLUE, ORANGE, RED, PURPLE, GREEN, SKYBLUE, PINK
};

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

static void Mission2_BuildPalette(void) {
    StrKey mono_keys[M2_MONOMER_PALETTE] = {
        S_MONO_0, S_MONO_1, S_MONO_2, S_MONO_3,
        S_MONO_4, S_MONO_5, S_MONO_6, S_MONO_7
    };
    for (int i = 0; i < M2_MONOMER_PALETTE; i++) {
        strncpy(m2.palette[i].monomer_name, LOC(mono_keys[i]), 31);
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
    Mission2_BuildChallenges();
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
            if (next < M2_CHALLENGE_COUNT) {
                Mission2_BuildChallenges();   /* re-read locale in case lang changed */
                Mission2_LoadChallenge(next);
            } else {
                m2.minigame_done = true;
                m2.trivia_phase  = true;
            }
        } else if (m2.feedback_timer > 2.0f && !m2.chain_correct) {
            for (int i = 0; i < M2_CHAIN_LEN; i++) {
                m2.slots[i].filled = false;
                m2.slots[i].content[0] = '\0';
            }
            for (int i = 0; i < M2_MONOMER_PALETTE; i++) m2.palette[i].used = false;
            m2.check_triggered = false;
        }
        return;
    }

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
                        mon->used = true;
                        dropped = true;
                        Mission2_CheckChain();
                        break;
                    }
                }
                if (!dropped) {
                    mon->rect.x = mon->origin.x;
                    mon->rect.y = mon->origin.y;
                }
            }
        }
    }
}

static void Mission2_DrawMinigame(void) {
    DrawText(LOC(S_M2_TITLE), 20, 14, 26, COL_UI_ACCENT);
    DrawText(LOC(S_M2_INSTRUCTION), 20, 48, 20, LIGHTGRAY);

    const PolymerChallenge *ch = &m2.challenges[m2.current_challenge];
    DrawText(LOC(S_M2_BUILD_LABEL), 20, 86, 20, WHITE);
    DrawText(ch->polymer_name, 20, 112, 32, ch->color);

    char prog[32];
    snprintf(prog, sizeof(prog), LOC(S_M2_CHALLENGE_PROG),
             m2.current_challenge+1, M2_CHALLENGE_COUNT);
    DrawText(prog, 20, 154, 18, GRAY);

    /* chain slots */
    for (int i = 0; i < M2_CHAIN_LEN; i++) {
        ChainSlot *s = &m2.slots[i];
        Color border = COL_UI_BORDER;
        Color bg     = COL_UI_PANEL;
        if (s->filled) {
            border = m2.check_triggered
                        ? (s->correct ? COL_CORRECT : COL_WRONG)
                        : ch->color;
            bg = ColorAlpha(border, 0.2f);
        }
        DrawRectangleRec(s->rect, bg);
        DrawRectangleLinesEx(s->rect, 2.5f, border);
        if (s->filled)
            UI_DrawWrappedText(s->content,
                               (int)(s->rect.x+6), (int)(s->rect.y+10),
                               (int)(s->rect.width-8), 16, WHITE);
        else {
            int qw = MeasureText("?", 28);
            DrawText("?", (int)(s->rect.x + s->rect.width/2 - qw/2),
                     (int)(s->rect.y + s->rect.height/2 - 14), 28, GRAY);
        }
        if (i < M2_CHAIN_LEN - 1)
            DrawText("->", (int)(s->rect.x + s->rect.width + 3),
                     (int)(s->rect.y + s->rect.height/2 - 12), 20, GRAY);
    }

    DrawLine(0, SCREEN_H - 160, GAME_W, SCREEN_H - 160, COL_UI_BORDER);
    DrawText(LOC(S_M2_MONOMERS_LABEL), 20, SCREEN_H - 152, 18, LIGHTGRAY);

    /* palette */
    for (int i = 0; i < M2_MONOMER_PALETTE; i++) {
        Monomer *mon = &m2.palette[i];
        if (mon->used) continue;
        Color c = ColorAlpha(mon->color, mon->dragging ? 0.95f : 0.72f);
        DrawRectangleRec(mon->rect, c);
        DrawRectangleLinesEx(mon->rect, mon->dragging ? 3.0f : 1.5f, mon->color);
        UI_DrawWrappedText(mon->monomer_name,
                           (int)(mon->rect.x+5), (int)(mon->rect.y+8),
                           (int)(mon->rect.width-6), 14, WHITE);
    }

    if (m2.check_triggered) {
        const char *msg = m2.chain_correct ? LOC(S_M2_CHAIN_OK) : LOC(S_M2_CHAIN_WRONG);
        DrawText(msg, 20, 196, 22, m2.chain_correct ? COL_CORRECT : COL_WRONG);
        if (m2.show_fact)
            UI_DrawWrappedText(m2.fun_fact, 20, 226, GAME_W - 40, 16, LIGHTGRAY);
    }
}

static void Mission2_DrawTrivia(void) {
    DrawRectangle(0, 0, GAME_W, SCREEN_H, COL_UI_BG);
    int iw = MeasureText(LOC(S_M2_TRIVIA_TITLE), 28);
    DrawText(LOC(S_M2_TRIVIA_TITLE), GAME_W/2 - iw/2, 20, 28, COL_UI_ACCENT);
    TriviaManager_DrawPanel((Rectangle){(GAME_W - 820)/2.0f, 70, 820, SCREEN_H - 80});
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
