#ifndef PLASTIC_GAME_H
#define PLASTIC_GAME_H

#include "raylib.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define SCREEN_W   1600
#define SCREEN_H    900
#define TARGET_FPS  120
#define GAME_W     1080
#define TRIVIA_W    520

typedef enum {
    MISSION_NONE = -1,
    MISSION_1_TIPOS_PLASTICO = 0,
    MISSION_2_POLIMEROS,
    MISSION_3_CONTAMINANTES,
    MISSION_4_PAISES,
    MISSION_5_CORRIENTES_SUP,
    MISSION_6_RECICLAJE,
    MISSION_7_ISLAS_PLASTICO,
    MISSION_8_CORRIENTES_OCEAN,
    MISSION_COUNT
} MissionID;

#define MAX_QUESTIONS         50
#define QUESTIONS_PER_MISSION  6
#define MAX_OPTION_LEN       256
#define MAX_EXPLANATION_LEN  512

typedef struct {
    int         id;
    MissionID   mission;
    char        text[512];
    char        options[4][MAX_OPTION_LEN];
    int         correct_index;
    char        explanation[MAX_EXPLANATION_LEN];
} Question;

typedef enum {
    TRIVIA_IDLE,
    TRIVIA_SHOWING_QUESTION,
    TRIVIA_FEEDBACK,
    TRIVIA_MISSION_COMPLETE
} TriviaState;

typedef struct {
    Question    questions[MAX_QUESTIONS];
    int         total_questions;
    int         mission_indices[MISSION_COUNT][QUESTIONS_PER_MISSION];
    MissionID   current_mission;
    int         current_q_order[QUESTIONS_PER_MISSION];
    int         current_q_pos;
    int         selected_answer;
    bool        answer_correct;
    TriviaState state;
    float       feedback_timer;
    float       feedback_duration;
    int         score;
    int         streak;
    int         points_per_correct;
    int         streak_bonus;
    void (*on_answer_result)(bool correct, const char *explanation);
    void (*on_mission_complete)(void);
} TriviaManager;

extern TriviaManager g_trivia;

void TriviaManager_Init(void);
void TriviaManager_LoadQuestions(void);
void TriviaManager_StartMission(MissionID m);
void TriviaManager_SubmitAnswer(int option_index);
void TriviaManager_Update(float dt);
void TriviaManager_DrawPanel(Rectangle panel_rect);
const Question *TriviaManager_CurrentQuestion(void);

bool UI_Button(Rectangle rect, const char *label, Color bg, Color fg);
void UI_DrawWrappedText(const char *text, int x, int y, int width, int font_size, Color color);
void UI_DrawPanel(Rectangle r, Color bg, Color border, int radius);
bool ShowMissionIntro(int mission_number, const char *mission_title,
                      const char *topic_title, const char *body_text);

typedef enum {
    SCENE_MAIN_MENU = 0,
    SCENE_MISSION_1, SCENE_MISSION_2, SCENE_MISSION_3, SCENE_MISSION_4,
    SCENE_MISSION_5, SCENE_MISSION_6, SCENE_MISSION_7, SCENE_MISSION_8,
    SCENE_RESULTS
} SceneID;

extern SceneID g_current_scene;

#define COL_OCEAN       CLITERAL(Color){ 10,  90, 140, 255 }
#define COL_OCEAN_LIGHT CLITERAL(Color){ 30, 130, 180, 255 }
#define COL_PLASTIC_RED CLITERAL(Color){220,  60,  50, 255 }
#define COL_PLASTIC_GRN CLITERAL(Color){ 60, 180,  80, 255 }
#define COL_PLASTIC_YEL CLITERAL(Color){240, 200,  30, 255 }
#define COL_UI_BG       CLITERAL(Color){ 18,  18,  30, 255 }
#define COL_UI_PANEL    CLITERAL(Color){ 28,  28,  46, 255 }
#define COL_UI_BORDER   CLITERAL(Color){ 70,  70, 110, 255 }
#define COL_UI_ACCENT   CLITERAL(Color){ 80, 200, 255, 255 }
#define COL_CORRECT     CLITERAL(Color){ 50, 205, 100, 255 }
#define COL_WRONG       CLITERAL(Color){220,  60,  60, 255 }

#endif /* PLASTIC_GAME_H */
