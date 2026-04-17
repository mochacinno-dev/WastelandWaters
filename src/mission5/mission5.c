#include "../core/plastic_game.h"

/* ──────────────────────────────────────────────────
   MISIÓN 5 — Simulación de Corrientes Superficiales
────────────────────────────────────────────────── */

#define M5_MAX_PARTICLES  80
#define M5_MAX_GYRES       6
#define M5_OCEAN_X        20
#define M5_OCEAN_Y        70
#define M5_OCEAN_W      1040
#define M5_OCEAN_H       760

typedef struct {
    Vector2 pos;
    float   angle;
    float   orbit_r;
    float   speed;
    bool    active;
    int     target_gyre;
    Color   color;
    float   size;
} PlasticParticle5;

typedef struct {
    Vector2 pos;
    float   pull_radius;
    float   rotation_speed;
    bool    active;
    bool    northern;
} OceanGyre;

typedef struct {
    PlasticParticle5 particles[M5_MAX_PARTICLES];
    OceanGyre        gyres[M5_MAX_GYRES];
    int              gyre_count;
    int              captured;
    float            info_timer;
    bool             show_coriolis_info;
    bool             minigame_done;
    bool             trivia_phase;
} Mission5State;

static Mission5State m5;

static void Mission5_SpawnParticles(void) {
    for (int i=0;i<M5_MAX_PARTICLES;i++) {
        m5.particles[i].active=true;
        m5.particles[i].pos.x=M5_OCEAN_X+20+(float)(rand()%(M5_OCEAN_W-40));
        m5.particles[i].pos.y=M5_OCEAN_Y+20+(float)(rand()%(M5_OCEAN_H-40));
        m5.particles[i].angle=(float)(rand()%628)/100.0f;
        m5.particles[i].orbit_r=999.0f;
        m5.particles[i].speed=0.5f+(float)(rand()%100)/200.0f;
        m5.particles[i].target_gyre=-1;
        m5.particles[i].size=4.0f+(float)(rand()%4);
        unsigned char rb=160+rand()%60;
        m5.particles[i].color=(Color){rb,rb,rb+20,200};
    }
}

static void Mission5_Init(void) {
    memset(&m5,0,sizeof(m5));
    Mission5_SpawnParticles();
    m5.show_coriolis_info=true;
    m5.info_timer=6.0f;
    TriviaManager_StartMission(MISSION_5_CORRIENTES_SUP);
}

static void Mission5_Update(float dt) {
    if (m5.trivia_phase) { TriviaManager_Update(dt); return; }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && m5.gyre_count<M5_MAX_GYRES) {
        Vector2 mouse=GetMousePosition();
        if (mouse.x>M5_OCEAN_X && mouse.x<M5_OCEAN_X+M5_OCEAN_W &&
            mouse.y>M5_OCEAN_Y && mouse.y<M5_OCEAN_Y+M5_OCEAN_H) {
            OceanGyre *g=&m5.gyres[m5.gyre_count];
            g->pos=mouse;
            g->pull_radius=140.0f;
            g->northern=(mouse.y < M5_OCEAN_Y+M5_OCEAN_H/2);
            g->rotation_speed=g->northern?1.3f:-1.3f;
            g->active=true;
            m5.gyre_count++;
        }
    }

    for (int i=0;i<M5_MAX_PARTICLES;i++) {
        PlasticParticle5 *p=&m5.particles[i];
        if (!p->active) continue;
        float min_dist=9999; int closest=-1;
        for (int g=0;g<m5.gyre_count;g++) {
            if (!m5.gyres[g].active) continue;
            float dx=p->pos.x-m5.gyres[g].pos.x, dy=p->pos.y-m5.gyres[g].pos.y;
            float d=sqrtf(dx*dx+dy*dy);
            if (d<m5.gyres[g].pull_radius && d<min_dist) { min_dist=d; closest=g; }
        }
        if (closest>=0) {
            OceanGyre *g=&m5.gyres[closest];
            float dx=p->pos.x-g->pos.x, dy=p->pos.y-g->pos.y;
            float dist=sqrtf(dx*dx+dy*dy);
            float angle=atan2f(dy,dx);
            float new_angle=angle+g->rotation_speed*dt;
            float new_r=dist-22.0f*dt;
            if (new_r<5.0f) { p->pos.x=g->pos.x; p->pos.y=g->pos.y; p->color=ColorAlpha(COL_PLASTIC_RED,0.6f); m5.captured++; continue; }
            p->pos.x=g->pos.x+cosf(new_angle)*new_r;
            p->pos.y=g->pos.y+sinf(new_angle)*new_r;
        } else {
            p->pos.x+=sinf(p->angle)*9.0f*dt;
            p->pos.y+=cosf(p->angle*1.3f)*6.0f*dt;
            p->angle+=0.3f*dt;
            if (p->pos.x<M5_OCEAN_X) p->pos.x=M5_OCEAN_X;
            if (p->pos.x>M5_OCEAN_X+M5_OCEAN_W) p->pos.x=M5_OCEAN_X+M5_OCEAN_W;
            if (p->pos.y<M5_OCEAN_Y) p->pos.y=M5_OCEAN_Y;
            if (p->pos.y>M5_OCEAN_Y+M5_OCEAN_H) p->pos.y=M5_OCEAN_Y+M5_OCEAN_H;
        }
    }

    /* minijuego completado cuando se captura >60% de las particulas */
    if (!m5.minigame_done && m5.captured >= (int)(M5_MAX_PARTICLES*0.6f)) {
        m5.minigame_done=true;
        m5.trivia_phase=true;
    }

    if (m5.show_coriolis_info) { m5.info_timer-=dt; if (m5.info_timer<=0) m5.show_coriolis_info=false; }
}

static void Mission5_DrawMinigame(void) {
    DrawText("MISION 5 — Corrientes Superficiales",20,14,26,COL_UI_ACCENT);
    DrawText("Haz clic para colocar giros oceanicos y atrapar el plastico",20,48,20,LIGHTGRAY);

    DrawRectangle(M5_OCEAN_X,M5_OCEAN_Y,M5_OCEAN_W,M5_OCEAN_H,COL_OCEAN);
    DrawRectangleLinesEx((Rectangle){M5_OCEAN_X,M5_OCEAN_Y,M5_OCEAN_W,M5_OCEAN_H},2,COL_UI_BORDER);

    int eq_y=M5_OCEAN_Y+M5_OCEAN_H/2;
    DrawLineEx((Vector2){M5_OCEAN_X,(float)eq_y},(Vector2){M5_OCEAN_X+M5_OCEAN_W,(float)eq_y},1.5f,ColorAlpha(WHITE,0.30f));
    DrawText("Ecuador",M5_OCEAN_X+6,eq_y-18,14,ColorAlpha(WHITE,0.50f));
    DrawText("H. Norte (giro horario)",M5_OCEAN_X+6,M5_OCEAN_Y+6,14,ColorAlpha(WHITE,0.50f));
    DrawText("H. Sur (giro antihorario)",M5_OCEAN_X+6,M5_OCEAN_Y+M5_OCEAN_H-22,14,ColorAlpha(WHITE,0.50f));

    for (int g=0;g<m5.gyre_count;g++) {
        OceanGyre *gy=&m5.gyres[g];
        Color gc=gy->northern?ColorAlpha(SKYBLUE,0.22f):ColorAlpha(LIME,0.22f);
        DrawCircleV(gy->pos,gy->pull_radius,gc);
        DrawCircleLinesV(gy->pos,gy->pull_radius,gy->northern?SKYBLUE:LIME);
        DrawText(gy->northern?"Giro N":"Giro S",(int)(gy->pos.x-28),(int)(gy->pos.y-10),16,WHITE);
    }
    for (int i=0;i<M5_MAX_PARTICLES;i++) {
        PlasticParticle5 *p=&m5.particles[i];
        if (!p->active) continue;
        DrawCircleV(p->pos,p->size,p->color);
    }

    char hud[80];
    snprintf(hud,sizeof(hud),"Giros: %d/%d   Plastico atrapado: %d/%d",
             m5.gyre_count,M5_MAX_GYRES,m5.captured,M5_MAX_PARTICLES);
    DrawText(hud,M5_OCEAN_X,M5_OCEAN_Y+M5_OCEAN_H+10,18,LIGHTGRAY);

    /* barra de progreso */
    float pct=(float)m5.captured/M5_MAX_PARTICLES; if(pct>1)pct=1;
    DrawRectangle(M5_OCEAN_X,M5_OCEAN_Y+M5_OCEAN_H+38,M5_OCEAN_W,14,COL_UI_BORDER);
    DrawRectangle(M5_OCEAN_X,M5_OCEAN_Y+M5_OCEAN_H+38,(int)(M5_OCEAN_W*pct),14,COL_PLASTIC_GRN);
    DrawText("Meta: capturar el 60% del plastico",M5_OCEAN_X,M5_OCEAN_Y+M5_OCEAN_H+58,16,GRAY);

    if (m5.show_coriolis_info) {
        DrawRectangle(M5_OCEAN_X,M5_OCEAN_Y+34,500,72,ColorAlpha(BLACK,0.72f));
        DrawText("Efecto Coriolis: los giros giran en sentido horario",M5_OCEAN_X+8,M5_OCEAN_Y+42,16,COL_UI_ACCENT);
        DrawText("en el hemisferio norte y antihorario en el sur.",M5_OCEAN_X+8,M5_OCEAN_Y+64,16,LIGHTGRAY);
    }
}

static void Mission5_DrawTrivia(void) {
    DrawRectangle(0,0,GAME_W,SCREEN_H,COL_UI_BG);
    const char *inst="MISION 5 — Trivia: Corrientes Superficiales";
    int iw=MeasureText(inst,28); DrawText(inst,GAME_W/2-iw/2,20,28,COL_UI_ACCENT);
    TriviaManager_DrawPanel((Rectangle){(GAME_W-820)/2.0f,70,820,SCREEN_H-80});
}

static void Mission5_Draw(void) {
    ClearBackground(COL_UI_BG);
    if (!m5.trivia_phase) {
        Mission5_DrawMinigame();
        Rectangle back={GAME_W-200,SCREEN_H-60,180,44};
        UI_Button(back,"Menu Principal",COL_UI_PANEL,WHITE);
    } else { Mission5_DrawTrivia(); }
}

void Mission5_Run(void) {
    bool go=ShowMissionIntro(5,
        "Corrientes Superficiales",
        "Como llega el plastico al Gran Parche?",
        "El Gran Parche del Pacifico Norte existe gracias al Giro del Pacifico Norte, un "
        "sistema circular de corrientes oceanicas creado por el Efecto Coriolis y los vientos "
        "dominantes. Este giro atrapa todo lo que flota en su interior, concentrando el "
        "plastico en una zona de baja presion en su centro. El plastico puede tardar anos "
        "en llegar desde la costa hasta el parche, viajando miles de kilometros impulsado "
        "por las corrientes. Existen 5 grandes parches de basura en el mundo, uno por cada "
        "giro oceanico principal. Comprender estas corrientes es clave para predecir donde "
        "se acumula el plastico y como interceptarlo.");
    if (!go) { g_current_scene=SCENE_MAIN_MENU; return; }
    Mission5_Init();
    while (!WindowShouldClose()) {
        float dt=GetFrameTime();
        Mission5_Update(dt);
        BeginDrawing(); Mission5_Draw(); EndDrawing();
        if (g_trivia.state==TRIVIA_MISSION_COMPLETE) break;
        if (!m5.trivia_phase) {
            Rectangle back={GAME_W-200,SCREEN_H-60,180,44};
            Vector2 mouse=GetMousePosition();
            if (CheckCollisionPointRec(mouse,back)&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { g_current_scene=SCENE_MAIN_MENU; return; }
        }
    }
    g_current_scene=SCENE_MISSION_6;
}