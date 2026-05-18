#include "core/plastic_game.h"

void Mission1_Run(void);
void Mission2_Run(void);
void Mission3_Run(void);
void Mission4_Run(void);
void Mission5_Run(void);
void Mission6_Run(void);
void Mission7_Run(void);
void Mission8_Run(void);

/* ──────────────────────────────────────────────────
   PANTALLA DE RESULTADOS
────────────────────────────────────────────────── */
static void ResultsScreen_Run(void) {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(COL_UI_BG);

        /* partículas de fondo */
        float t = (float)GetTime();
        for (int i = 0; i < 40; i++) {
            float x = (sinf(t * 0.3f + i * 2.3f) * 0.5f + 0.5f) * SCREEN_W;
            float y = (cosf(t * 0.2f + i * 1.7f) * 0.5f + 0.5f) * SCREEN_H;
            DrawCircle((int)x, (int)y, 3 + i%5, ColorAlpha(COL_OCEAN_LIGHT, 0.25f));
        }

        int tw;

        const char *titulo = "JUEGO COMPLETO";
        tw = MeasureText(titulo, 60);
        DrawText(titulo, SCREEN_W/2 - tw/2, 140, 60, COL_UI_ACCENT);

        char sc[64];
        snprintf(sc, sizeof(sc), "Puntaje final: %d", g_trivia.score);
        tw = MeasureText(sc, 36);
        DrawText(sc, SCREEN_W/2 - tw/2, 230, 36, COL_PLASTIC_YEL);

        const char *msg1 = "Gracias por aprender sobre la contaminacion plastica.";
        tw = MeasureText(msg1, 24);
        DrawText(msg1, SCREEN_W/2 - tw/2, 300, 24, WHITE);

        const char *msg2 = "Cada accion cuenta. Reduce, Reutiliza, Recicla.";
        tw = MeasureText(msg2, 22);
        DrawText(msg2, SCREEN_W/2 - tw/2, 338, 22, LIGHTGRAY);

        const char *msg3 = "El Gran Parche del Pacifico tiene mas de 1.6 millones de km2.";
        const char *msg4 = "Tu conocimiento es el primer paso para cambiarlo.";
        tw = MeasureText(msg3, 20);
        DrawText(msg3, SCREEN_W/2 - tw/2, 390, 20, ColorAlpha(COL_OCEAN_LIGHT, 0.9f));
        tw = MeasureText(msg4, 20);
        DrawText(msg4, SCREEN_W/2 - tw/2, 418, 20, ColorAlpha(COL_OCEAN_LIGHT, 0.9f));

        if (UI_Button((Rectangle){SCREEN_W/2 - 140, 500, 280, 64},
                       "Volver al inicio", COL_UI_ACCENT, BLACK)) {
            g_current_scene = SCENE_MAIN_MENU;
            TriviaManager_Init();
            break;
        }

        EndDrawing();
    }
}

/* ──────────────────────────────────────────────────
   MENÚ PRINCIPAL
────────────────────────────────────────────────── */
static float s_anim = 0.0f;

static void MainMenu_Run(void) {
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        s_anim += dt;

        BeginDrawing();
        ClearBackground(COL_UI_BG);

        /* fondo animado */
        for (int i = 0; i < 50; i++) {
            float x = (sinf(s_anim * 0.3f + i * 2.3f) * 0.5f + 0.5f) * SCREEN_W;
            float y = (cosf(s_anim * 0.2f + i * 1.7f) * 0.5f + 0.5f) * SCREEN_H;
            DrawCircle((int)x, (int)y, 3 + i%6, ColorAlpha(COL_OCEAN_LIGHT, 0.25f));
        }

        /* título */
        const char *title = "Wasteland Waters";
        int tw = MeasureText(title, 72);
        DrawText(title, SCREEN_W/2 - tw/2, 80, 72, COL_UI_ACCENT);

        const char *sub = "Un viaje educativo por la crisis de las islas de plastico";
        int sw = MeasureText(sub, 22);
        DrawText(sub, SCREEN_W/2 - sw/2, 168, 22, LIGHTGRAY);

        /* cuadrícula de misiones: 2 columnas × 4 filas */
        const char *mission_names[] = {
            "Mision 1 .. Tipos de Plastico",
            "Mision 2 .. Polimeros",
            "Mision 3 .. Contaminantes",
            "Mision 4 .. Paises Contaminantes",
            "Mision 5 .. Corrientes Superficiales",
            "Mision 6 .. Reciclaje",
            "Mision 7 .. Islas de Plastico",
            "Mision 8 .. Corrientes Oceanicas"
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

        /* botón inicio rápido */
        float iq_y = start_y + 4*(btn_h + gap_y) + 20;
        if (UI_Button((Rectangle){SCREEN_W/2 - 200, iq_y, 400, 68},
                       "INICIAR DESDE EL PRINCIPIO", COL_UI_ACCENT, BLACK)) {
            g_current_scene = SCENE_MISSION_1;
            EndDrawing();
            return;
        }

        /* puntaje */
        DrawText("Puntaje acumulado:", 28, SCREEN_H - 36, 18, GRAY);
        char pts[32];
        snprintf(pts, sizeof(pts), "%d", g_trivia.score);
        DrawText(pts, 220, SCREEN_H - 36, 18, COL_UI_ACCENT);

        DrawText("CoffeeShop Development  [v1.7.25]", SCREEN_W - 340, SCREEN_H - 26, 14, DARKGRAY);

        EndDrawing();
    }
}

/* ──────────────────────────────────────────────────
   MAIN
────────────────────────────────────────────────── */
int main(void) {
    InitWindow(SCREEN_W, SCREEN_H, "Wasteland Waters v1.7.25");
    SetTargetFPS(TARGET_FPS);
    srand((unsigned)time(NULL));

    TriviaManager_Init();
    g_current_scene = SCENE_MAIN_MENU;

    while (!WindowShouldClose()) {
        switch (g_current_scene) {
            case SCENE_MAIN_MENU:  MainMenu_Run();    break;
            case SCENE_MISSION_1:  Mission1_Run();    break;
            case SCENE_MISSION_2:  Mission2_Run();    break;
            case SCENE_MISSION_3:  Mission3_Run();    break;
            case SCENE_MISSION_4:  Mission4_Run();    break;
            case SCENE_MISSION_5:  Mission5_Run();    break;
            case SCENE_MISSION_6:  Mission6_Run();    break;
            case SCENE_MISSION_7:  Mission7_Run();    break;
            case SCENE_MISSION_8:  Mission8_Run();    break;
            case SCENE_RESULTS:    ResultsScreen_Run(); break;
            default:               g_current_scene = SCENE_MAIN_MENU; break;
        }
    }

    CloseWindow();
    return 0;
}