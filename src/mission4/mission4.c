#include "../core/plastic_game.h"

/* ──────────────────────────────────────────────────
   MISIÓN 4 — Mapa Interactivo de Países Contaminantes
────────────────────────────────────────────────── */

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

static void Mission4_Init(void) {
    memset(&m4, 0, sizeof(m4));
    CountryPin cp[M4_COUNTRY_COUNT] = {
        {"China",      "Asia",   1, 0.77f,0.38f, RED,         false},
        {"Indonesia",  "Asia",   2, 0.80f,0.56f, ORANGE,      false},
        {"Filipinas",  "Asia",   3, 0.82f,0.50f, YELLOW,      false},
        {"Vietnam",    "Asia",   4, 0.79f,0.46f, COL_PLASTIC_YEL, false},
        {"Sri Lanka",  "Asia",   5, 0.72f,0.54f, LIME,        false},
        {"Tailandia",  "Asia",   6, 0.77f,0.48f, SKYBLUE,     false},
        {"Egipto",     "Africa", 7, 0.56f,0.40f, PURPLE,      false},
        {"Nigeria",    "Africa", 8, 0.50f,0.52f, VIOLET,      false},
        {"Brasil",     "LAm",    9, 0.34f,0.64f, GREEN,       false},
        {"India",      "Asia",  10, 0.70f,0.44f, PINK,        false},
    };
    memcpy(m4.countries, cp, sizeof(cp));
    m4.yangtze.points[0]=(Vector2){0.73f,0.36f};
    m4.yangtze.points[1]=(Vector2){0.75f,0.37f};
    m4.yangtze.points[2]=(Vector2){0.77f,0.37f};
    m4.yangtze.points[3]=(Vector2){0.78f,0.38f};
    m4.yangtze.points[4]=(Vector2){0.80f,0.38f};
    strncpy(m4.yangtze.name,"Rio Yangtse (China)",63);
    for (int i=0;i<M4_COUNTRY_COUNT;i++) m4.rank_order[i]=i;
    m4.dragging_rank_idx=-1;
    m4.activity=M4_ACT_IDENTIFY;
    TriviaManager_StartMission(MISSION_4_PAISES);
}

static Vector2 NormToMap(float nx, float ny) {
    return (Vector2){M4_MAP_X + nx*M4_MAP_W, M4_MAP_Y + ny*M4_MAP_H};
}

static void M4_Act1_Update(void) {
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return;
    Vector2 mouse = GetMousePosition();
    for (int i=0;i<M4_COUNTRY_COUNT;i++) {
        Vector2 p = NormToMap(m4.countries[i].norm_x, m4.countries[i].norm_y);
        float dx=mouse.x-p.x, dy=mouse.y-p.y;
        if (dx*dx+dy*dy < 24*24) {
            m4.countries[i].selected=true;
            if (strcmp(m4.countries[i].region,"Asia")==0) {
                strncpy(m4.feedback_msg,"Correcto! Asia aporta >60% de la contaminacion plastica oceanica.",sizeof(m4.feedback_msg)-1);
                m4.feedback_col=COL_CORRECT; m4.activity_complete=true; m4.feedback_timer=0;
            } else {
                strncpy(m4.feedback_msg,"Esa region contribuye, pero Asia es la mayor fuente. Intenta de nuevo.",sizeof(m4.feedback_msg)-1);
                m4.feedback_col=COL_WRONG; m4.activity_complete=false;
            }
            break;
        }
    }
}

static void M4_Act2_Update(float dt) {
    (void)dt;
    Vector2 mouse=GetMousePosition();
    float slot_h=46.0f, rx=60.0f, ry=170.0f;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (int i=0;i<M4_COUNTRY_COUNT;i++) {
            Rectangle r={rx,ry+i*slot_h,360,slot_h-4};
            if (CheckCollisionPointRec(mouse,r)) { m4.dragging_rank_idx=i; break; }
        }
    }
    if (m4.dragging_rank_idx>=0 && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        int new_pos=(int)((mouse.y-ry)/slot_h);
        if (new_pos<0) new_pos=0;
        if (new_pos>=M4_COUNTRY_COUNT) new_pos=M4_COUNTRY_COUNT-1;
        int old_pos=m4.dragging_rank_idx;
        if (new_pos!=old_pos) {
            int tmp=m4.rank_order[old_pos];
            int dir=(new_pos>old_pos)?1:-1;
            for (int i=old_pos;i!=new_pos;i+=dir) m4.rank_order[i]=m4.rank_order[i+dir];
            m4.rank_order[new_pos]=tmp;
        }
        m4.dragging_rank_idx=-1;
        bool ok=true;
        for (int i=0;i<M4_COUNTRY_COUNT;i++) if (m4.countries[m4.rank_order[i]].rank!=i+1) {ok=false;break;}
        if (ok) {
            strncpy(m4.feedback_msg,"Ranking correcto! China lidera, seguida de Indonesia y Filipinas.",sizeof(m4.feedback_msg)-1);
            m4.feedback_col=COL_CORRECT; m4.activity_complete=true; m4.feedback_timer=0;
        }
    }
}

static void M4_Act3_Update(void) {
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return;
    Vector2 mouse=GetMousePosition();
    if (m4.river_drawn>=M4_RIVER_POINTS) return;
    Vector2 target=m4.yangtze.points[m4.river_drawn];
    Vector2 tp=NormToMap(target.x,target.y);
    float dx=mouse.x-tp.x, dy=mouse.y-tp.y;
    if (dx*dx+dy*dy<36*36) {
        m4.river_drawn++;
        if (m4.river_drawn==M4_RIVER_POINTS) {
            m4.river_ok=true;
            strncpy(m4.feedback_msg,"Correcto! El Yangtse descarga ~330.000 t de plastico al mar anualmente.",sizeof(m4.feedback_msg)-1);
            m4.feedback_col=COL_CORRECT; m4.activity_complete=true; m4.feedback_timer=0;
        }
    }
}

static void Mission4_Update(float dt) {
    if (m4.trivia_phase) { TriviaManager_Update(dt); return; }
    if (m4.activity_complete) {
        m4.feedback_timer+=dt;
        if (m4.feedback_timer>2.8f) {
            m4.activity=(M4Activity)(m4.activity+1);
            m4.activity_complete=false; m4.feedback_timer=0;
            if (m4.activity==M4_ACT_DONE) { m4.minigame_done=true; m4.trivia_phase=true; }
        }
        return;
    }
    switch(m4.activity) {
        case M4_ACT_IDENTIFY: M4_Act1_Update();   break;
        case M4_ACT_RANKING:  M4_Act2_Update(dt); break;
        case M4_ACT_RIVER:    M4_Act3_Update();   break;
        default: break;
    }
}

static void Mission4_DrawMap(void) {
    DrawRectangle(M4_MAP_X,M4_MAP_Y,M4_MAP_W,M4_MAP_H,COL_OCEAN);
    DrawRectangleLinesEx((Rectangle){M4_MAP_X,M4_MAP_Y,M4_MAP_W,M4_MAP_H},2,COL_UI_BORDER);
    DrawText("MAPA MUNDIAL",M4_MAP_X+6,M4_MAP_Y+6,14,COL_UI_BORDER);
    for (int i=0;i<M4_COUNTRY_COUNT;i++) {
        Vector2 p=NormToMap(m4.countries[i].norm_x,m4.countries[i].norm_y);
        Color c=m4.countries[i].selected?m4.countries[i].color:ColorAlpha(m4.countries[i].color,0.70f);
        DrawCircleV(p,12,c);
        DrawCircleLinesV(p,12,WHITE);
        DrawText(m4.countries[i].name,(int)(p.x+14),(int)(p.y-8),14,WHITE);
    }
}

static void Mission4_DrawMinigame(void) {
    const char *titles[]={
        "Actividad 1: Que region contamina mas? Haz clic en la region",
        "Actividad 2: Ordena los paises por cantidad de plastico (arrastra)",
        "Actividad 3: Traza la ruta del rio mas contaminante (Yangtse)",
        "Mision 4 completa!"
    };
    DrawText("MISION 4 — Mapa Interactivo de Paises",20,14,26,COL_UI_ACCENT);
    DrawText(titles[m4.activity<M4_ACT_DONE?m4.activity:M4_ACT_DONE],20,48,20,LIGHTGRAY);
    Mission4_DrawMap();

    if (m4.activity==M4_ACT_RANKING) {
        float rx=60,ry=170;
        DrawText("Arrastra para ordenar:",(int)rx,(int)ry-22,16,WHITE);
        for (int i=0;i<M4_COUNTRY_COUNT;i++) {
            CountryPin *cp=&m4.countries[m4.rank_order[i]];
            Rectangle slot={rx,ry+i*46.0f,360,42};
            DrawRectangleRec(slot,ColorAlpha(cp->color,0.28f));
            DrawRectangleLinesEx(slot,1.5f,cp->color);
            char label[80]; snprintf(label,sizeof(label),"#%d  %s",i+1,cp->name);
            DrawText(label,(int)(rx+10),(int)(ry+i*46+10),18,WHITE);
        }
    }
    if (m4.activity==M4_ACT_RIVER) {
        DrawText("Haz clic en los puntos naranjas en orden",20,620,16,ORANGE);
        for (int i=0;i<M4_RIVER_POINTS;i++) {
            Vector2 p=NormToMap(m4.yangtze.points[i].x,m4.yangtze.points[i].y);
            Color c=i<m4.river_drawn?COL_CORRECT:ORANGE;
            DrawCircleV(p,10,c);
            char num[4]; snprintf(num,sizeof(num),"%d",i+1);
            DrawText(num,(int)(p.x-5),(int)(p.y-10),16,BLACK);
        }
        for (int i=0;i<m4.river_drawn-1;i++) {
            Vector2 a=NormToMap(m4.yangtze.points[i].x,m4.yangtze.points[i].y);
            Vector2 b=NormToMap(m4.yangtze.points[i+1].x,m4.yangtze.points[i+1].y);
            DrawLineEx(a,b,3.5f,COL_CORRECT);
        }
    }
    if (m4.feedback_msg[0])
        DrawText(m4.feedback_msg,M4_MAP_X,M4_MAP_Y+M4_MAP_H+12,18,m4.feedback_col);
}

static void Mission4_DrawTrivia(void) {
    DrawRectangle(0,0,GAME_W,SCREEN_H,COL_UI_BG);
    const char *inst="MISION 4 — Trivia: Paises Contaminantes";
    int iw=MeasureText(inst,28); DrawText(inst,GAME_W/2-iw/2,20,28,COL_UI_ACCENT);
    TriviaManager_DrawPanel((Rectangle){(GAME_W-820)/2.0f,70,820,SCREEN_H-80});
}

static void Mission4_Draw(void) {
    ClearBackground(COL_UI_BG);
    if (!m4.trivia_phase) {
        Mission4_DrawMinigame();
        Rectangle back={GAME_W-200,SCREEN_H-60,180,44};
        UI_Button(back,"Menu Principal",COL_UI_PANEL,WHITE);
    } else { Mission4_DrawTrivia(); }
}

void Mission4_Run(void) {
    bool go=ShowMissionIntro(4,
        "Paises Contaminantes",
        "¿Quien envia el plastico al Gran Parche?",
        "El Gran Parche del Pacifico Norte se forma principalmente con desechos provenientes "
        "de las costas de Asia Oriental. China, Indonesia, Filipinas, Vietnam y Sri Lanka "
        "concentran el 60% de la contaminacion plastica oceanica mundial. El rio Yangtse, "
        "en China, es el mayor transportador de plastico al mar, arrojando hasta 330.000 "
        "toneladas anuales. Estas cantidades masivas llegan al oceano porque en muchas "
        "regiones la gestion de residuos es insuficiente. Sin embargo, paises desarrollados "
        "como EE.UU. tambien contribuyen exportando millones de toneladas de residuos "
        "plasticos a naciones con menor capacidad de gestion.");
    if (!go) { g_current_scene=SCENE_MAIN_MENU; return; }
    Mission4_Init();
    while (!WindowShouldClose()) {
        float dt=GetFrameTime();
        Mission4_Update(dt);
        BeginDrawing(); Mission4_Draw(); EndDrawing();
        if (g_trivia.state==TRIVIA_MISSION_COMPLETE) break;
        if (!m4.trivia_phase) {
            Rectangle back={GAME_W-200,SCREEN_H-60,180,44};
            Vector2 mouse=GetMousePosition();
            if (CheckCollisionPointRec(mouse,back)&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { g_current_scene=SCENE_MAIN_MENU; return; }
        }
    }
    g_current_scene=SCENE_MISSION_5;
}