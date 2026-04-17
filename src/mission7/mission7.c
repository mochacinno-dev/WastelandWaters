#include "../core/plastic_game.h"

/* ──────────────────────────────────────────────────
   MISIÓN 7 — Exploración Naval: Gran Parche de Basura
────────────────────────────────────────────────── */

#define M7_MAX_DEBRIS   200
#define M7_PATCH_CX     700.0f
#define M7_PATCH_CY     500.0f
#define M7_PATCH_R      380.0f
#define M7_GOAL_PIECES   60
#define M7_WORLD_W      2000
#define M7_WORLD_H      1600

typedef struct {
    float x,y,vx,vy,rot,rot_speed;
} Boat;

typedef struct {
    float  x,y,size;
    Color  color;
    bool   active;
    float  bob_phase;
} Debris;

typedef struct {
    Boat     boat;
    Debris   debris[M7_MAX_DEBRIS];
    int      collected;
    int      goal;
    Camera2D camera;
    bool     mission_done;
    float    done_timer;
    float    world_scroll;
    bool     trivia_phase;
} Mission7State;

static Mission7State m7;

static void Mission7_SpawnDebris(void) {
    srand(42);
    for (int i=0;i<M7_MAX_DEBRIS;i++) {
        float angle=(float)(rand()%628)/100.0f;
        float r=M7_PATCH_R*sqrtf((float)(rand()%1000)/1000.0f);
        m7.debris[i].x=M7_PATCH_CX+cosf(angle)*r;
        m7.debris[i].y=M7_PATCH_CY+sinf(angle)*r;
        m7.debris[i].size=5.0f+(float)(rand()%9);
        m7.debris[i].active=true;
        m7.debris[i].bob_phase=(float)(rand()%628)/100.0f;
        int c=rand()%4;
        Color debris_colors[4]={{200,200,200,180},{220,180,100,180},{180,100,100,180},{100,180,180,180}};
        m7.debris[i].color=debris_colors[c];
    }
}

static void Mission7_Init(void) {
    memset(&m7,0,sizeof(m7));
    m7.boat.x=M7_PATCH_CX-500; m7.boat.y=M7_PATCH_CY;
    m7.goal=M7_GOAL_PIECES;
    /* La cámara ocupa GAME_W de la pantalla */
    m7.camera.offset=(Vector2){GAME_W/2.0f,SCREEN_H/2.0f};
    m7.camera.rotation=0; m7.camera.zoom=1.0f;
    Mission7_SpawnDebris();
    TriviaManager_StartMission(MISSION_7_ISLAS_PLASTICO);
}

static void Mission7_Update(float dt) {
    if (m7.trivia_phase) { TriviaManager_Update(dt); return; }
    m7.world_scroll+=dt;
    if (m7.mission_done) { m7.done_timer+=dt; if(m7.done_timer>2.5f){m7.trivia_phase=true;} return; }

    float thrust=0,turn=0;
    if (IsKeyDown(KEY_W)||IsKeyDown(KEY_UP))   thrust= 1.0f;
    if (IsKeyDown(KEY_S)||IsKeyDown(KEY_DOWN))  thrust=-0.5f;
    if (IsKeyDown(KEY_A)||IsKeyDown(KEY_LEFT))  turn  =-2.8f;
    if (IsKeyDown(KEY_D)||IsKeyDown(KEY_RIGHT)) turn  = 2.8f;

    m7.boat.rot+=turn*dt;
    float fx=cosf(m7.boat.rot)*thrust*200.0f*dt;
    float fy=sinf(m7.boat.rot)*thrust*200.0f*dt;
    m7.boat.vx=m7.boat.vx*0.92f+fx;
    m7.boat.vy=m7.boat.vy*0.92f+fy;
    m7.boat.x+=m7.boat.vx; m7.boat.y+=m7.boat.vy;
    if (m7.boat.x<0) m7.boat.x=0;
    if (m7.boat.x>M7_WORLD_W) m7.boat.x=M7_WORLD_W;
    if (m7.boat.y<0) m7.boat.y=0;
    if (m7.boat.y>M7_WORLD_H) m7.boat.y=M7_WORLD_H;

    m7.camera.target=(Vector2){m7.boat.x,m7.boat.y};

    float collect_r=32.0f;
    for (int i=0;i<M7_MAX_DEBRIS;i++) {
        if (!m7.debris[i].active) continue;
        float dx=m7.boat.x-m7.debris[i].x, dy=m7.boat.y-m7.debris[i].y;
        if (dx*dx+dy*dy<collect_r*collect_r) {
            m7.debris[i].active=false; m7.collected++;
            if (m7.collected>=m7.goal) m7.mission_done=true;
        }
    }
}

static void Mission7_DrawBoat(void) {
    float rot=m7.boat.rot, bx=m7.boat.x, by=m7.boat.y, size=22.0f;
    Vector2 tip  ={bx+cosf(rot)*size*1.6f, by+sinf(rot)*size*1.6f};
    Vector2 left ={bx+cosf(rot+2.4f)*size,  by+sinf(rot+2.4f)*size};
    Vector2 right={bx+cosf(rot-2.4f)*size,  by+sinf(rot-2.4f)*size};
    DrawTriangle(tip,right,left,WHITE);
    DrawTriangleLines(tip,right,left,COL_UI_ACCENT);
    DrawCircleV((Vector2){bx-cosf(rot)*size*2.2f,by-sinf(rot)*size*2.2f},8,ColorAlpha(WHITE,0.18f));
}

static void Mission7_DrawMinigame(void) {
    /* mundo con cámara */
    BeginMode2D(m7.camera);
    float t=m7.world_scroll;
    for (int tx=0;tx<M7_WORLD_W;tx+=100)
        for (int ty=0;ty<M7_WORLD_H;ty+=100) {
            float wave=sinf(t+tx*0.05f+ty*0.04f)*0.03f;
            DrawRectangle(tx,ty,100,100,ColorAlpha(COL_OCEAN,0.6f+wave));
        }
    DrawRectangleLinesEx((Rectangle){0,0,M7_WORLD_W,M7_WORLD_H},5,COL_UI_BORDER);
    DrawCircleV((Vector2){M7_PATCH_CX,M7_PATCH_CY},M7_PATCH_R,ColorAlpha(COL_PLASTIC_YEL,0.07f));
    DrawCircleLinesV((Vector2){M7_PATCH_CX,M7_PATCH_CY},M7_PATCH_R,ColorAlpha(COL_PLASTIC_YEL,0.30f));
    DrawText("GRAN PARCHE",(int)(M7_PATCH_CX-70),(int)(M7_PATCH_CY-M7_PATCH_R-20),18,ColorAlpha(COL_PLASTIC_YEL,0.60f));
    for (int i=0;i<M7_MAX_DEBRIS;i++) {
        if (!m7.debris[i].active) continue;
        float bob=sinf(t*1.5f+m7.debris[i].bob_phase)*3.5f;
        DrawCircleV((Vector2){m7.debris[i].x,m7.debris[i].y+bob},m7.debris[i].size,m7.debris[i].color);
    }
    Mission7_DrawBoat();
    EndMode2D();

    /* HUD encima */
    DrawRectangle(0,0,GAME_W,64,ColorAlpha(COL_UI_BG,0.88f));
    DrawText("MISION 7 — Gran Parche de Basura",20,12,26,COL_UI_ACCENT);
    DrawText("WASD / Flechas para navegar. Recoge el plastico.",20,44,18,LIGHTGRAY);

    float pct=(float)m7.collected/(float)m7.goal; if(pct>1)pct=1;
    DrawRectangle(20,SCREEN_H-44,500,18,COL_UI_BORDER);
    DrawRectangle(20,SCREEN_H-44,(int)(500*pct),18,COL_PLASTIC_GRN);
    char prog[64]; snprintf(prog,sizeof(prog),"Recogido: %d / %d piezas",m7.collected,m7.goal);
    DrawText(prog,20,SCREEN_H-68,20,WHITE);

    if (m7.mission_done) {
        DrawRectangle(0,0,GAME_W,SCREEN_H,ColorAlpha(BLACK,0.55f));
        const char *msg="Parche limpiado!";
        int mw=MeasureText(msg,48); DrawText(msg,GAME_W/2-mw/2,SCREEN_H/2-40,48,COL_CORRECT);
        DrawText("Cargando trivia...",GAME_W/2-120,SCREEN_H/2+24,24,LIGHTGRAY);
    }
}

static void Mission7_DrawTrivia(void) {
    DrawRectangle(0,0,GAME_W,SCREEN_H,COL_UI_BG);
    const char *inst="MISION 7 — Trivia: Islas de Plastico";
    int iw=MeasureText(inst,28); DrawText(inst,GAME_W/2-iw/2,20,28,COL_UI_ACCENT);
    TriviaManager_DrawPanel((Rectangle){(GAME_W-820)/2.0f,70,820,SCREEN_H-80});
}

static void Mission7_Draw(void) {
    ClearBackground(COL_UI_BG);
    if (!m7.trivia_phase) {
        Mission7_DrawMinigame();
        Rectangle back={GAME_W-200,SCREEN_H-68,180,44};
        UI_Button(back,"Menu Principal",COL_UI_PANEL,WHITE);
    } else { Mission7_DrawTrivia(); }
}

void Mission7_Run(void) {
    bool go=ShowMissionIntro(7,
        "Islas de Plastico",
        "El Gran Parche del Pacifico: lo que los ojos no ven",
        "El Gran Parche de Basura del Pacifico (Great Pacific Garbage Patch) fue descubierto "
        "en 1997 por el navegante Charles Moore. Con mas de 1.6 millones de km2, es tres "
        "veces el tamano de Francia. Sin embargo, no es una isla solida: es una 'sopa' "
        "de microplasticos suspendidos en el agua que la hace turbia y lechosa. "
        "Se estima que contiene mas de 80.000 toneladas de plastico y mas de 1.8 billones "
        "de fragmentos. Es imposible de limpiar completamente con la tecnologia actual, "
        "pero organizaciones como The Ocean Cleanup trabajan para reducirlo. "
        "En este minijuego navegaras por el parche recogiendo residuos.");
    if (!go) { g_current_scene=SCENE_MAIN_MENU; return; }
    Mission7_Init();
    while (!WindowShouldClose()) {
        float dt=GetFrameTime();
        Mission7_Update(dt);
        BeginDrawing(); Mission7_Draw(); EndDrawing();
        if (g_trivia.state==TRIVIA_MISSION_COMPLETE) break;
        if (!m7.trivia_phase) {
            Rectangle back={GAME_W-200,SCREEN_H-68,180,44};
            Vector2 mouse=GetMousePosition();
            if (CheckCollisionPointRec(mouse,back)&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { g_current_scene=SCENE_MAIN_MENU; return; }
        }
    }
    g_current_scene=SCENE_MISSION_8;
}