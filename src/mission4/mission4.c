#include "../core/plastic_game.h"
#include "../core/locale.h"

#define M4_COUNTRY_COUNT  10
#define M4_RIVER_POINTS    5
#define M4_MAP_X           30
#define M4_MAP_Y           80
#define M4_MAP_W          1020
#define M4_MAP_H           520

typedef struct {
    char    name[64];
    char    region[32];
    int     rank;
    float   norm_x, norm_y;
    Color   color;
    bool    selected;
} CountryPin;

typedef struct {
    Vector2 points[M4_RIVER_POINTS];
    char    name[64];
} RiverData;

typedef enum { M4_ACT_IDENTIFY=0, M4_ACT_RANKING, M4_ACT_RIVER, M4_ACT_DONE } M4Activity;

typedef struct {
    Texture2D map_tex;
    CountryPin  countries[M4_COUNTRY_COUNT];
    RiverData   yangtze;
    M4Activity  activity;
    int  rank_order[M4_COUNTRY_COUNT];
    int  dragging_rank_idx;
    float rank_drag_y;
    int  river_drawn;
    bool river_ok;
    bool activity_complete;
    float feedback_timer;
    char  feedback_msg[256];
    Color feedback_col;
    bool  minigame_done;
    bool  trivia_phase;
} Mission4State;

static Mission4State m4;

static void Mission4_BuildCountries(void) {
    /* Coordinates are language-independent; only names come from locale */
    struct {
        StrKey name_key;
        StrKey region_key;
        int rank;
        float nx, ny;
        Color col;
    } defs[M4_COUNTRY_COUNT] = {
        { S_M4_C0, S_M4_REGION_ASIA,   1, 0.795f, 0.355f, RED         },  // China
        { S_M4_C1, S_M4_REGION_ASIA,   2, 0.810f, 0.520f, ORANGE      },  // Indonesia
        { S_M4_C2, S_M4_REGION_ASIA,   3, 0.835f, 0.460f, YELLOW      },  // Filipinas
        { S_M4_C3, S_M4_REGION_ASIA,   4, 0.800f, 0.430f, COL_PLASTIC_YEL }, // Vietnam
        { S_M4_C4, S_M4_REGION_ASIA,   5, 0.745f, 0.490f, LIME        },  // Sri Lanka
        { S_M4_C5, S_M4_REGION_ASIA,   6, 0.795f, 0.445f, SKYBLUE     },  // Tailandia
        { S_M4_C6, S_M4_REGION_AFRICA, 7, 0.565f, 0.365f, PURPLE      },  // Egipto
        { S_M4_C7, S_M4_REGION_AFRICA, 8, 0.520f, 0.470f, VIOLET      },  // Nigeria
        { S_M4_C8, S_M4_REGION_LAM,    9, 0.340f, 0.590f, GREEN       },  // Brasil
        { S_M4_C9, S_M4_REGION_ASIA,  10, 0.720f, 0.415f, PINK        },  // India
    };
    for (int i = 0; i < M4_COUNTRY_COUNT; i++) {
        strncpy(m4.countries[i].name,   LOC(defs[i].name_key),   63);
        strncpy(m4.countries[i].region, LOC(defs[i].region_key), 31);
        m4.countries[i].rank   = defs[i].rank;
        m4.countries[i].norm_x = defs[i].nx;
        m4.countries[i].norm_y = defs[i].ny;
        m4.countries[i].color  = defs[i].col;
        m4.countries[i].selected = false;
    }
}

static void Mission4_Init(void) {
    memset(&m4, 0, sizeof(m4));

    m4.map_tex = LoadTexture("assets/world_map.png");

    Mission4_BuildCountries();

    m4.yangtze.points[0]=(Vector2){0.73f,0.36f};
    m4.yangtze.points[1]=(Vector2){0.75f,0.37f};
    m4.yangtze.points[2]=(Vector2){0.77f,0.37f};
    m4.yangtze.points[3]=(Vector2){0.78f,0.38f};
    m4.yangtze.points[4]=(Vector2){0.80f,0.38f};
    strncpy(m4.yangtze.name, LOC(S_M4_RIVER_NAME), 63);

    for (int i = 0; i < M4_COUNTRY_COUNT; i++) m4.rank_order[i] = i;
    m4.dragging_rank_idx = -1;
    m4.activity = M4_ACT_IDENTIFY;

    TriviaManager_StartMission(MISSION_4_PAISES);
}

static Vector2 NormToMap(float nx, float ny) {
    return (Vector2){M4_MAP_X + nx*M4_MAP_W, M4_MAP_Y + ny*M4_MAP_H};
}

static void M4_Act1_Update(void) {
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return;
    Vector2 mouse = GetMousePosition();
    for (int i = 0; i < M4_COUNTRY_COUNT; i++) {
        Vector2 p = NormToMap(m4.countries[i].norm_x, m4.countries[i].norm_y);
        float dx=mouse.x-p.x, dy=mouse.y-p.y;
        if (dx*dx+dy*dy < 24*24) {
            m4.countries[i].selected = true;
            if (strcmp(m4.countries[i].region, LOC(S_M4_REGION_ASIA)) == 0) {
                strncpy(m4.feedback_msg, LOC(S_M4_CORRECT_ASIA),
                        sizeof(m4.feedback_msg)-1);
                m4.feedback_col       = COL_CORRECT;
                m4.activity_complete  = true;
                m4.feedback_timer     = 0;
            } else {
                strncpy(m4.feedback_msg, LOC(S_M4_WRONG_REGION),
                        sizeof(m4.feedback_msg)-1);
                m4.feedback_col       = COL_WRONG;
                m4.activity_complete  = false;
            }
            break;
        }
    }
}

static void M4_Act2_Update(float dt) {
    (void)dt;
    Vector2 mouse = GetMousePosition();
    float slot_h = 46.0f, rx = 60.0f, ry = 170.0f;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (int i = 0; i < M4_COUNTRY_COUNT; i++) {
            Rectangle r={rx, ry+i*slot_h, 360, slot_h-4};
            if (CheckCollisionPointRec(mouse, r)) {
                m4.dragging_rank_idx = i;
                break;
            }
        }
    }
    if (m4.dragging_rank_idx >= 0 && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        int new_pos = (int)((mouse.y - ry) / slot_h);
        if (new_pos < 0) new_pos = 0;
        if (new_pos >= M4_COUNTRY_COUNT) new_pos = M4_COUNTRY_COUNT - 1;
        int old_pos = m4.dragging_rank_idx;
        if (new_pos != old_pos) {
            int tmp = m4.rank_order[old_pos];
            int dir = (new_pos > old_pos) ? 1 : -1;
            for (int i = old_pos; i != new_pos; i += dir) m4.rank_order[i] = m4.rank_order[i+dir];
            m4.rank_order[new_pos] = tmp;
        }
        m4.dragging_rank_idx = -1;
        bool ok = true;
        for (int i = 0; i < M4_COUNTRY_COUNT; i++)
            if (m4.countries[m4.rank_order[i]].rank != i+1) { ok=false; break; }
        if (ok) {
            strncpy(m4.feedback_msg, LOC(S_M4_RANK_OK), sizeof(m4.feedback_msg)-1);
            m4.feedback_col      = COL_CORRECT;
            m4.activity_complete = true;
            m4.feedback_timer    = 0;
        }
    }
}

static void M4_Act3_Update(void) {
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return;
    Vector2 mouse = GetMousePosition();
    if (m4.river_drawn >= M4_RIVER_POINTS) return;
    Vector2 target = m4.yangtze.points[m4.river_drawn];
    Vector2 tp = NormToMap(target.x, target.y);
    float dx=mouse.x-tp.x, dy=mouse.y-tp.y;
    if (dx*dx+dy*dy < 36*36) {
        m4.river_drawn++;
        if (m4.river_drawn == M4_RIVER_POINTS) {
            m4.river_ok = true;
            strncpy(m4.feedback_msg, LOC(S_M4_RIVER_OK), sizeof(m4.feedback_msg)-1);
            m4.feedback_col      = COL_CORRECT;
            m4.activity_complete = true;
            m4.feedback_timer    = 0;
        }
    }
}

static void Mission4_Update(float dt) {
    if (m4.trivia_phase) { TriviaManager_Update(dt); return; }
    if (m4.activity_complete) {
        m4.feedback_timer += dt;
        if (m4.feedback_timer > 2.8f) {
            m4.activity = (M4Activity)(m4.activity + 1);
            m4.activity_complete = false;
            m4.feedback_timer    = 0;
            if (m4.activity == M4_ACT_DONE) {
                m4.minigame_done = true;
                m4.trivia_phase  = true;
            }
        }
        return;
    }
    switch (m4.activity) {
        case M4_ACT_IDENTIFY: M4_Act1_Update();   break;
        case M4_ACT_RANKING:  M4_Act2_Update(dt); break;
        case M4_ACT_RIVER:    M4_Act3_Update();   break;
        default: break;
    }
}

static void Mission4_DrawMap(void) {
    DrawRectangle(M4_MAP_X, M4_MAP_Y, M4_MAP_W, M4_MAP_H, COL_OCEAN);
    // Dibujar el mapa real escalado al área definida
    if (m4.map_tex.id > 0) {
        Rectangle src  = { 0, 0,
                           (float)m4.map_tex.width,
                           (float)m4.map_tex.height };
        Rectangle dest = { M4_MAP_X, M4_MAP_Y, M4_MAP_W, M4_MAP_H };
        DrawTexturePro(m4.map_tex, src, dest,
                       (Vector2){0, 0}, 0.0f, WHITE);
    }

    DrawRectangleLinesEx(
        (Rectangle){M4_MAP_X, M4_MAP_Y, M4_MAP_W, M4_MAP_H},
        2, COL_UI_BORDER);
    DrawText(LOC(S_M4_MAP_LABEL), M4_MAP_X+6, M4_MAP_Y+6, 14, COL_UI_BORDER);

    for (int i = 0; i < M4_COUNTRY_COUNT; i++) {
        Vector2 p = NormToMap(m4.countries[i].norm_x, m4.countries[i].norm_y);
        Color c = m4.countries[i].selected
                    ? m4.countries[i].color
                    : ColorAlpha(m4.countries[i].color, 0.70f);
        DrawCircleV(p, 12, c);
        DrawCircleLinesV(p, 12, WHITE);
        DrawText(m4.countries[i].name, (int)(p.x+14), (int)(p.y-8), 14, BLACK);
    }
}

static void Mission4_DrawMinigame(void) {
    const char *act_titles[4] = {
        LOC(S_M4_ACT1_TITLE), LOC(S_M4_ACT2_TITLE),
        LOC(S_M4_ACT3_TITLE), LOC(S_M4_DONE_TITLE)
    };
    DrawText(LOC(S_M4_TITLE), 20, 14, 26, COL_UI_ACCENT);
    DrawText(act_titles[m4.activity < M4_ACT_DONE ? m4.activity : M4_ACT_DONE],
             20, 48, 20, LIGHTGRAY);
    Mission4_DrawMap();

    if (m4.activity == M4_ACT_RANKING) {
        float rx = 60, ry = 170;
        DrawText(LOC(S_M4_RANK_LABEL), (int)rx, (int)ry-22, 16, WHITE);
        for (int i = 0; i < M4_COUNTRY_COUNT; i++) {
            CountryPin *cp = &m4.countries[m4.rank_order[i]];
            Rectangle slot = {rx, ry+i*46.0f, 360, 42};
            DrawRectangleRec(slot, ColorAlpha(cp->color, 0.28f));
            DrawRectangleLinesEx(slot, 1.5f, cp->color);
            char label[80];
            snprintf(label, sizeof(label), "#%d  %s", i+1, cp->name);
            DrawText(label, (int)(rx+10), (int)(ry+i*46+10), 18, BLACK);
        }
    }
    if (m4.activity == M4_ACT_RIVER) {
        DrawText(LOC(S_M4_ACT3_CLICK_HINT), 20, 620, 16, ORANGE);
        for (int i = 0; i < M4_RIVER_POINTS; i++) {
            Vector2 p = NormToMap(m4.yangtze.points[i].x, m4.yangtze.points[i].y);
            Color c = i < m4.river_drawn ? COL_CORRECT : ORANGE;
            DrawCircleV(p, 10, c);
            char num[4]; snprintf(num, sizeof(num), "%d", i+1);
            DrawText(num, (int)(p.x-5), (int)(p.y-10), 16, BLACK);
        }
        for (int i = 0; i < m4.river_drawn - 1; i++) {
            Vector2 a = NormToMap(m4.yangtze.points[i].x, m4.yangtze.points[i].y);
            Vector2 b = NormToMap(m4.yangtze.points[i+1].x, m4.yangtze.points[i+1].y);
            DrawLineEx(a, b, 3.5f, COL_CORRECT);
        }
    }
    if (m4.feedback_msg[0])
        DrawText(m4.feedback_msg, M4_MAP_X, M4_MAP_Y+M4_MAP_H+12, 18, m4.feedback_col);
}

static void Mission4_DrawTrivia(void) {
    DrawRectangle(0, 0, GAME_W, SCREEN_H, COL_UI_BG);
    int iw = MeasureText(LOC(S_M4_TRIVIA_TITLE), 28);
    DrawText(LOC(S_M4_TRIVIA_TITLE), GAME_W/2-iw/2, 20, 28, COL_UI_ACCENT);
    TriviaManager_DrawPanel((Rectangle){(GAME_W-820)/2.0f, 70, 820, SCREEN_H-80});
}

static void Mission4_Draw(void) {
    ClearBackground(COL_UI_BG);
    if (!m4.trivia_phase) {
        Mission4_DrawMinigame();
        Rectangle back = {GAME_W-200, SCREEN_H-60, 180, 44};
        UI_Button(back, LOC(S_BTN_MENU_MAIN), COL_UI_PANEL, WHITE);
    } else {
        Mission4_DrawTrivia();
    }
}

void Mission4_Run(void) {
    bool go = ShowMissionIntro(4,
        LOC(S_M4_INTRO_TITLE),
        LOC(S_M4_INTRO_TOPIC),
        LOC(S_M4_INTRO_BODY));
    if (!go) { g_current_scene = SCENE_MAIN_MENU; return; }

    Mission4_Init();
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Mission4_Update(dt);
        BeginDrawing();
        Mission4_Draw();
        EndDrawing();
        if (g_trivia.state == TRIVIA_MISSION_COMPLETE) break;
        if (!m4.trivia_phase) {
            Rectangle back = {GAME_W-200, SCREEN_H-60, 180, 44};
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, back) &&
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                g_current_scene = SCENE_MAIN_MENU;
                return;
            }
        }
    }
    UnloadTexture(m4.map_tex);

    g_current_scene = SCENE_MISSION_5;
}
