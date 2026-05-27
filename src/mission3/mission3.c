#include "../core/plastic_game.h"
#include "../core/locale.h"

#define M3_MAX_PARTICLES  120
#define M3_MAX_FILTERS     20
#define M3_WAYPOINT_COUNT   6
#define M3_TOTAL_WAVES      3

typedef struct {
    Vector2 pos;
    float   speed;
    int     waypoint_target;
    bool    active;
    bool    captured;
    Color   color;
    float   size;
} Microplastic;

typedef struct {
    Vector2 pos;
    float   radius;
    bool    active;
} Filter;

typedef struct {
    Microplastic particles[M3_MAX_PARTICLES];
    Filter       filters[M3_MAX_FILTERS];
    int          filter_count;
    Vector2      waypoints[M3_WAYPOINT_COUNT];
    float        spawn_timer;
    float        spawn_interval;
    int          spawned;
    int          particles_per_wave;
    int          current_wave;
    int          total_waves;
    int          food_chain_hp;
    int          max_food_chain_hp;
    int          captured_count;
    bool         game_over;
    bool         wave_complete;
    float        wave_timer;
    bool         minigame_done;
    bool         trivia_phase;
} Mission3State;

static Mission3State m3;

static void Mission3_Init(void) {
    memset(&m3, 0, sizeof(m3));

    m3.waypoints[0] = (Vector2){ 30,  240};
    m3.waypoints[1] = (Vector2){220,  380};
    m3.waypoints[2] = (Vector2){460,  220};
    m3.waypoints[3] = (Vector2){680,  420};
    m3.waypoints[4] = (Vector2){880,  280};
    m3.waypoints[5] = (Vector2){1040, 340};

    m3.spawn_interval     = 1.1f;
    m3.particles_per_wave = 18;
    m3.total_waves        = M3_TOTAL_WAVES;
    m3.current_wave       = 0;
    m3.food_chain_hp      = 10;
    m3.max_food_chain_hp  = 10;

    TriviaManager_StartMission(MISSION_3_CONTAMINANTES);
}

static void Mission3_SpawnParticle(void) {
    for (int i = 0; i < M3_MAX_PARTICLES; i++) {
        if (!m3.particles[i].active) {
            m3.particles[i].active          = true;
            m3.particles[i].captured        = false;
            m3.particles[i].pos             = m3.waypoints[0];
            m3.particles[i].waypoint_target = 1;
            m3.particles[i].speed           = 65.0f + (float)(rand()%50);
            m3.particles[i].size            = 5.0f + (float)(rand()%6);
            unsigned char g = 120 + rand()%80;
            m3.particles[i].color = (Color){g, g, g+20, 220};
            return;
        }
    }
}

static void Mission3_Update(float dt) {
    if (m3.trivia_phase) { TriviaManager_Update(dt); return; }
    if (m3.game_over) {
        m3.wave_timer += dt;
        if (m3.wave_timer > 3.0f && !m3.minigame_done) {
            m3.minigame_done = true;
            m3.trivia_phase  = true;
        }
        return;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && m3.filter_count < M3_MAX_FILTERS) {
        Vector2 mouse = GetMousePosition();
        if (mouse.x < GAME_W - 10 && mouse.y > 60) {
            m3.filters[m3.filter_count].pos    = mouse;
            m3.filters[m3.filter_count].radius = 46.0f;
            m3.filters[m3.filter_count].active = true;
            m3.filter_count++;
        }
    }

    m3.spawn_timer += dt;
    if (m3.spawn_timer >= m3.spawn_interval &&
        m3.spawned < m3.particles_per_wave * (m3.current_wave + 1)) {
        Mission3_SpawnParticle();
        m3.spawned++;
        m3.spawn_timer = 0.0f;
    }

    int active_count = 0;
    for (int i = 0; i < M3_MAX_PARTICLES; i++) {
        Microplastic *p = &m3.particles[i];
        if (!p->active) continue;
        active_count++;
        if (p->captured) {
            p->size -= dt * 10.0f;
            if (p->size <= 0) p->active = false;
            continue;
        }
        bool caught = false;
        for (int f = 0; f < m3.filter_count; f++) {
            if (m3.filters[f].active) {
                float dx = p->pos.x - m3.filters[f].pos.x;
                float dy = p->pos.y - m3.filters[f].pos.y;
                if (dx*dx + dy*dy < m3.filters[f].radius * m3.filters[f].radius) {
                    p->captured = true;
                    m3.captured_count++;
                    caught = true;
                    break;
                }
            }
        }
        if (caught) continue;
        if (p->waypoint_target >= M3_WAYPOINT_COUNT) {
            p->active = false;
            m3.food_chain_hp--;
            if (m3.food_chain_hp <= 0) {
                m3.food_chain_hp = 0;
                m3.game_over     = true;
                m3.wave_timer    = 0;
            }
            continue;
        }
        Vector2 target = m3.waypoints[p->waypoint_target];
        float dx = target.x - p->pos.x, dy = target.y - p->pos.y;
        float dist = sqrtf(dx*dx + dy*dy);
        if (dist < 6.0f) p->waypoint_target++;
        else { p->pos.x += (dx/dist)*p->speed*dt; p->pos.y += (dy/dist)*p->speed*dt; }
    }

    if (!m3.wave_complete) {
        bool all_done = (m3.spawned >= m3.particles_per_wave*(m3.current_wave+1))
                        && (active_count == 0);
        if (all_done) { m3.wave_complete = true; m3.wave_timer = 0.0f; }
    } else {
        m3.wave_timer += dt;
        if (m3.wave_timer > 2.2f) {
            m3.current_wave++;
            m3.wave_complete = false;
            if (m3.current_wave >= m3.total_waves) {
                m3.game_over     = true;
                m3.minigame_done = true;
                m3.trivia_phase  = true;
            }
        }
    }
}

static void Mission3_DrawRiver(void) {
    for (int i = 0; i < M3_WAYPOINT_COUNT - 1; i++) {
        DrawLineEx(m3.waypoints[i], m3.waypoints[i+1], 60,
                   ColorAlpha(COL_OCEAN, 0.40f));
        DrawLineEx(m3.waypoints[i], m3.waypoints[i+1], 2.5f, COL_OCEAN_LIGHT);
    }
    Vector2 last = m3.waypoints[M3_WAYPOINT_COUNT-1];
    DrawRectangle((int)last.x, (int)last.y - 36, 110, 72, ColorAlpha(RED, 0.30f));
    /* "Food Chain" label — two lines to fit */
    const char *fc = LOC(S_M3_FOOD_CHAIN_HP);
    DrawText(fc, (int)last.x + 4, (int)last.y - 34, 15, RED);
}

static void Mission3_DrawMinigame(void) {
    DrawText(LOC(S_M3_TITLE), 20, 14, 26, COL_UI_ACCENT);
    DrawText(LOC(S_M3_INSTRUCTION), 20, 48, 20, LIGHTGRAY);

    Mission3_DrawRiver();

    for (int f = 0; f < m3.filter_count; f++) {
        DrawCircleV(m3.filters[f].pos, m3.filters[f].radius,
                    ColorAlpha(COL_PLASTIC_GRN, 0.28f));
        DrawCircleLinesV(m3.filters[f].pos, m3.filters[f].radius, COL_PLASTIC_GRN);
        DrawText(LOC(S_M3_FILTER_LABEL),
                 (int)(m3.filters[f].pos.x-26), (int)(m3.filters[f].pos.y-9),
                 14, COL_PLASTIC_GRN);
    }
    for (int i = 0; i < M3_MAX_PARTICLES; i++) {
        Microplastic *p = &m3.particles[i];
        if (!p->active) continue;
        DrawCircleV(p->pos, p->size, p->captured ? COL_CORRECT : p->color);
    }

    /* HUD */
    DrawText(LOC(S_M3_FOOD_CHAIN_HP), 20, SCREEN_H - 64, 18, WHITE);
    for (int h = 0; h < m3.max_food_chain_hp; h++) {
        Color hc = h < m3.food_chain_hp ? COL_CORRECT : DARKGRAY;
        DrawRectangle(300 + h*28, SCREEN_H - 62, 22, 20, hc);
    }
    char wave_str[40];
    snprintf(wave_str, sizeof(wave_str), LOC(S_M3_WAVE),
             m3.current_wave+1, m3.total_waves);
    DrawText(wave_str, 20, SCREEN_H - 38, 18, GRAY);
    char cap_str[40];
    snprintf(cap_str, sizeof(cap_str), LOC(S_M3_CAPTURED), m3.captured_count);
    DrawText(cap_str, 240, SCREEN_H - 38, 18, COL_PLASTIC_GRN);

    if (m3.wave_complete && m3.current_wave < m3.total_waves)
        DrawText(LOC(S_M3_WAVE_DONE), 20, 480, 26, COL_CORRECT);

    if (m3.game_over) {
        DrawRectangle(0, 0, GAME_W, SCREEN_H, ColorAlpha(BLACK, 0.55f));
        bool victory = (m3.current_wave >= m3.total_waves && m3.food_chain_hp > 0);
        const char *msg = victory ? LOC(S_M3_RIVER_OK) : LOC(S_M3_RIVER_FAIL);
        int mw = MeasureText(msg, 40);
        DrawText(msg, GAME_W/2 - mw/2, SCREEN_H/2 - 30, 40,
                 victory ? COL_CORRECT : COL_WRONG);
        int lw = MeasureText(LOC(S_M3_LOADING_TRIVIA), 24);
        DrawText(LOC(S_M3_LOADING_TRIVIA), GAME_W/2 - lw/2, SCREEN_H/2 + 30, 24, LIGHTGRAY);
    }
}

static void Mission3_DrawTrivia(void) {
    DrawRectangle(0, 0, GAME_W, SCREEN_H, COL_UI_BG);
    int iw = MeasureText(LOC(S_M3_TRIVIA_TITLE), 28);
    DrawText(LOC(S_M3_TRIVIA_TITLE), GAME_W/2 - iw/2, 20, 28, COL_UI_ACCENT);
    TriviaManager_DrawPanel((Rectangle){(GAME_W-820)/2.0f, 70, 820, SCREEN_H-80});
}

static void Mission3_Draw(void) {
    ClearBackground(COL_UI_BG);
    if (!m3.trivia_phase) {
        Mission3_DrawMinigame();
        Rectangle back = {GAME_W - 200, SCREEN_H - 60, 180, 44};
        UI_Button(back, LOC(S_BTN_MENU_MAIN), COL_UI_PANEL, WHITE);
    } else {
        Mission3_DrawTrivia();
    }
}

void Mission3_Run(void) {
    bool go = ShowMissionIntro(3,
        LOC(S_M3_INTRO_TITLE),
        LOC(S_M3_INTRO_TOPIC),
        LOC(S_M3_INTRO_BODY));
    if (!go) { g_current_scene = SCENE_MAIN_MENU; return; }

    Mission3_Init();
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Mission3_Update(dt);
        BeginDrawing();
        Mission3_Draw();
        EndDrawing();
        if (g_trivia.state == TRIVIA_MISSION_COMPLETE) break;
        if (!m3.trivia_phase) {
            Rectangle back = {GAME_W - 200, SCREEN_H - 60, 180, 44};
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, back) &&
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                g_current_scene = SCENE_MAIN_MENU;
                return;
            }
        }
    }
    g_current_scene = SCENE_MISSION_4;
}
