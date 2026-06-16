#include "plastic_game.h"
#include "locale.h"

void Mission1_Run(void);
void Mission2_Run(void);
void Mission3_Run(void);
void Mission4_Run(void);
void Mission5_Run(void);
void Mission6_Run(void);
void Mission7_Run(void);
void Mission8_Run(void);

/* ──────────────────────────────────────────────────
   GLOBAL LOGO TEXTURE
   ────────────────────────────────────────────────── */
static Texture2D g_logo_texture;

/* ──────────────────────────────────────────────────
   LANGUAGE TOGGLE HELPER
   Call this INSIDE BeginDrawing() / EndDrawing().
   Returns true if the language was just changed
   (caller should reload questions & restart menu).
────────────────────────────────────────────────── */
static bool DrawLangButtons(void) {
    /* Two small pill buttons in the top-right corner */
    float bw = 54, bh = 34, gap = 6;
    float by = 14;
    float bx_en = SCREEN_W - bw - 14;
    float bx_es = bx_en - bw - gap;

    Rectangle r_es = { bx_es, by, bw, bh };
    Rectangle r_en = { bx_en, by, bw, bh };

    Color active_bg   = COL_UI_ACCENT;
    Color inactive_bg = COL_UI_PANEL;
    Color active_fg   = BLACK;
    Color inactive_fg = GRAY;

    bool changed = false;

    /* ES button */
    Color es_bg = (g_lang == LANG_ES) ? active_bg : inactive_bg;
    Color es_fg = (g_lang == LANG_ES) ? active_fg : inactive_fg;
    DrawRectangleRounded(r_es, 0.4f, 6, es_bg);
    DrawRectangleRoundedLines(r_es, 0.4f, 6, 1.5f,
                              (g_lang == LANG_ES) ? COL_UI_ACCENT : COL_UI_BORDER);
    {
        int tw = MeasureText(LOC(S_LANG_ES), 18);
        DrawText(LOC(S_LANG_ES),
                 (int)(r_es.x + r_es.width/2 - tw/2),
                 (int)(r_es.y + r_es.height/2 - 9), 18, es_fg);
    }
    {
        Vector2 m = GetMousePosition();
        if (CheckCollisionPointRec(m, r_es) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
                && g_lang != LANG_ES) {
            g_lang = LANG_ES;
            TriviaManager_LoadQuestions();
            changed = true;
        }
    }

    /* EN button */
    Color en_bg = (g_lang == LANG_EN) ? active_bg : inactive_bg;
    Color en_fg = (g_lang == LANG_EN) ? active_fg : inactive_fg;
    DrawRectangleRounded(r_en, 0.4f, 6, en_bg);
    DrawRectangleRoundedLines(r_en, 0.4f, 6, 1.5f,
                              (g_lang == LANG_EN) ? COL_UI_ACCENT : COL_UI_BORDER);
    {
        int tw = MeasureText(LOC(S_LANG_EN), 18);
        DrawText(LOC(S_LANG_EN),
                 (int)(r_en.x + r_en.width/2 - tw/2),
                 (int)(r_en.y + r_en.height/2 - 9), 18, en_fg);
    }
    {
        Vector2 m = GetMousePosition();
        if (CheckCollisionPointRec(m, r_en) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
                && g_lang != LANG_EN) {
            g_lang = LANG_EN;
            TriviaManager_LoadQuestions();
            changed = true;
        }
    }

    return changed;
}

/* ──────────────────────────────────────────────────
   RESULTS SCREEN
────────────────────────────────────────────────── */
static void ResultsScreen_Run(void) {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(COL_UI_BG);

        float t = (float)GetTime();
        for (int i = 0; i < 40; i++) {
            float x = (sinf(t * 0.3f + i * 2.3f) * 0.5f + 0.5f) * SCREEN_W;
            float y = (cosf(t * 0.2f + i * 1.7f) * 0.5f + 0.5f) * SCREEN_H;
            DrawCircle((int)x, (int)y, 3 + i%5, ColorAlpha(COL_OCEAN_LIGHT, 0.25f));
        }

        int tw;
        const char *titulo = LOC(S_GAME_COMPLETE);
        tw = MeasureText(titulo, 60);
        DrawText(titulo, SCREEN_W/2 - tw/2, 140, 60, COL_UI_ACCENT);

        char sc[64];
        snprintf(sc, sizeof(sc), LOC(S_FINAL_SCORE), g_trivia.score);
        tw = MeasureText(sc, 36);
        DrawText(sc, SCREEN_W/2 - tw/2, 230, 36, COL_PLASTIC_YEL);

        tw = MeasureText(LOC(S_THANKS_MSG), 24);
        DrawText(LOC(S_THANKS_MSG), SCREEN_W/2 - tw/2, 300, 24, WHITE);

        tw = MeasureText(LOC(S_REDUCE_MSG), 22);
        DrawText(LOC(S_REDUCE_MSG), SCREEN_W/2 - tw/2, 338, 22, LIGHTGRAY);

        tw = MeasureText(LOC(S_PATCH_FACT), 20);
        DrawText(LOC(S_PATCH_FACT), SCREEN_W/2 - tw/2, 390, 20,
                 ColorAlpha(COL_OCEAN_LIGHT, 0.9f));
        tw = MeasureText(LOC(S_KNOWLEDGE_MSG), 20);
        DrawText(LOC(S_KNOWLEDGE_MSG), SCREEN_W/2 - tw/2, 418, 20,
                 ColorAlpha(COL_OCEAN_LIGHT, 0.9f));

        if (UI_Button((Rectangle){SCREEN_W/2 - 140, 500, 280, 64},
                       LOC(S_BTN_BACK_START), COL_UI_ACCENT, BLACK)) {
            g_current_scene = SCENE_MAIN_MENU;
            TriviaManager_Init();
            EndDrawing();
            break;
        }

        DrawLangButtons();
        
        /* Draw logo texture in bottom-right corner - Results Screen */
        if (g_logo_texture.id > 0) {
            float logo_scale = 0.18f;  /* Ajustable: 0.15-0.25 */
            float logo_width = g_logo_texture.width * logo_scale;
            float logo_height = g_logo_texture.height * logo_scale;
            float logo_x = SCREEN_W - logo_width - 20;   /* 20px from right edge */
            float logo_y = SCREEN_H - logo_height - 20;  /* 20px from bottom edge */
            
            DrawTextureEx(g_logo_texture,
                          (Vector2){logo_x, logo_y},
                          0.0f, logo_scale, WHITE);
        }
        
        EndDrawing();
    }
}

/* ──────────────────────────────────────────────────
   MAIN MENU
────────────────────────────────────────────────── */
static float s_anim = 0.0f;

static void MainMenu_Run(void) {
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        s_anim += dt;

        BeginDrawing();
        ClearBackground(COL_UI_BG);

        /* animated background */
        for (int i = 0; i < 50; i++) {
            float x = (sinf(s_anim * 0.3f + i * 2.3f) * 0.5f + 0.5f) * SCREEN_W;
            float y = (cosf(s_anim * 0.2f + i * 1.7f) * 0.5f + 0.5f) * SCREEN_H;
            DrawCircle((int)x, (int)y, 3 + i%6, ColorAlpha(COL_OCEAN_LIGHT, 0.25f));
        }

        /* title */
        int tw = MeasureText(LOC(S_GAME_TITLE), 72);
        DrawText(LOC(S_GAME_TITLE), SCREEN_W/2 - tw/2, 80, 72, COL_UI_ACCENT);

        int sw = MeasureText(LOC(S_GAME_SUBTITLE), 22);
        DrawText(LOC(S_GAME_SUBTITLE), SCREEN_W/2 - sw/2, 168, 22, LIGHTGRAY);

        /* mission grid: 2 cols × 4 rows */
        const char *mission_names[8] = {
            LOC(S_M1_BTN), LOC(S_M2_BTN), LOC(S_M3_BTN), LOC(S_M4_BTN),
            LOC(S_M5_BTN), LOC(S_M6_BTN), LOC(S_M7_BTN), LOC(S_M8_BTN)
        };

        float btn_w = 560, btn_h = 62;
        float gap_x = 40, gap_y = 14;
        float total_w = 2*btn_w + gap_x;
        float start_x = (SCREEN_W - total_w) / 2.0f;
        float start_y = 220.0f;

        for (int i = 0; i < 8; i++) {
            int col = i % 2;
            int row = i / 2;
            Rectangle r = {
                start_x + col * (btn_w + gap_x),
                start_y + row * (btn_h + gap_y),
                btn_w, btn_h
            };
            if (UI_Button(r, mission_names[i], COL_UI_PANEL, WHITE)) {
                g_current_scene = (SceneID)(SCENE_MISSION_1 + i);
                EndDrawing();
                return;
            }
        }

        /* quick-start button */
        float iq_y = start_y + 4*(btn_h + gap_y) + 20;
        if (UI_Button((Rectangle){SCREEN_W/2 - 200, iq_y, 400, 68},
                       LOC(S_BTN_START_ALL), COL_UI_ACCENT, BLACK)) {
            g_current_scene = SCENE_MISSION_1;
            EndDrawing();
            return;
        }

        /* score */
        DrawText(LOC(S_SCORE_ACCUM), 28, SCREEN_H - 36, 18, GRAY);
        char pts[32];
        snprintf(pts, sizeof(pts), "%d", g_trivia.score);
        DrawText(pts, 220, SCREEN_H - 36, 18, COL_UI_ACCENT);

        /* Draw logo texture in bottom-right corner - Main Menu */
        if (g_logo_texture.id > 0) {
            float logo_scale = 0.20f;  /* Ajustable: 0.15-0.25 */
            float logo_width = g_logo_texture.width * logo_scale;
            float logo_height = g_logo_texture.height * logo_scale;
            float logo_x = SCREEN_W - logo_width - 20;   /* 20px from right edge */
            float logo_y = SCREEN_H - logo_height - 20;  /* 20px from bottom edge */
            
            DrawTextureEx(g_logo_texture,
                          (Vector2){logo_x, logo_y},
                          0.0f, logo_scale, WHITE);
        }

        /* language buttons — restart menu loop on change */
        if (DrawLangButtons()) {
            EndDrawing();
            return;   /* re-enters MainMenu_Run with new strings */
        }

        EndDrawing();
    }
}

/* ──────────────────────────────────────────────────
   MAIN
────────────────────────────────────────────────── */
int main(void) {
    InitWindow(SCREEN_W, SCREEN_H, "Wasteland Waters v2.0.1");
    SetTargetFPS(TARGET_FPS);
    srand((unsigned)time(NULL));

    /* Load the logo texture */
    g_logo_texture = LoadTexture("assets/lovebound.png");

    TriviaManager_Init();
    g_current_scene = SCENE_MAIN_MENU;

    while (!WindowShouldClose()) {
        switch (g_current_scene) {
            case SCENE_MAIN_MENU:  MainMenu_Run();      break;
            case SCENE_MISSION_1:  Mission1_Run();      break;
            case SCENE_MISSION_2:  Mission2_Run();      break;
            case SCENE_MISSION_3:  Mission3_Run();      break;
            case SCENE_MISSION_4:  Mission4_Run();      break;
            case SCENE_MISSION_5:  Mission5_Run();      break;
            case SCENE_MISSION_6:  Mission6_Run();      break;
            case SCENE_MISSION_7:  Mission7_Run();      break;
            case SCENE_MISSION_8:  Mission8_Run();      break;
            case SCENE_RESULTS:    ResultsScreen_Run(); break;
            default:               g_current_scene = SCENE_MAIN_MENU; break;
        }
    }

    /* Unload the logo texture before closing */
    if (g_logo_texture.id > 0) {
        UnloadTexture(g_logo_texture);
    }

    CloseWindow();
    return 0;
}
