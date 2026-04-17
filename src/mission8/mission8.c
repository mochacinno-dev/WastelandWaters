#include "../core/plastic_game.h"

/* ──────────────────────────────────────────────────
   MISIÓN 8 — Circulación Termohalina
────────────────────────────────────────────────── */

#define M8_ZONE_COUNT      5
#define M8_MAX_PARTICLES  40
#define M8_PANEL_X        30
#define M8_PANEL_Y        80
#define M8_PANEL_W       720
#define M8_PANEL_H       520

typedef struct {
    char   name[32];
    float  temp;
    float  salinity;
    Rectangle rect;
    Color  color;
    bool   selected;
} OceanZone;

typedef struct {
    Vector2 pos;
    int     current_zone;
    float   t;
    float   speed;
    bool    active;
    Color   color;
} ThermoParticle;

typedef struct {
    OceanZone      zones[M8_ZONE_COUNT];
    ThermoParticle particles[M8_MAX_PARTICLES];
    int            selected_zone;
    float          slider_temp, slider_salt;
    bool           dragging_temp, dragging_salt;
    float          circulation_strength;
    bool           amoc_collapsed;
    float          ice_melt_timer;
    bool           show_ice_melt_effect;
    char           status_msg[256];
    Color          status_col;
    float          interact_timer;  /* tiempo de interaccion para desbloquear trivia */
    bool           minigame_done;
    bool           trivia_phase;
} Mission8State;

static Mission8State m8;

static void Mission8_InitZones(void) {
    const char *names[M8_ZONE_COUNT]={"Artico","Atlantico Norte","Tropical","Atlantico Sur","Antartico"};
    float temps[M8_ZONE_COUNT]={0.1f,0.3f,0.9f,0.6f,0.05f};
    float salts[M8_ZONE_COUNT]={0.7f,0.8f,0.6f,0.7f,0.75f};
    Color colors[M8_ZONE_COUNT]={SKYBLUE,BLUE,RED,ORANGE,DARKBLUE};
    float zw=130.0f, zh=M8_PANEL_H;
    for (int i=0;i<M8_ZONE_COUNT;i++) {
        strncpy(m8.zones[i].name,names[i],31);
        m8.zones[i].temp=temps[i]; m8.zones[i].salinity=salts[i];
        m8.zones[i].color=colors[i]; m8.zones[i].selected=false;
        m8.zones[i].rect=(Rectangle){M8_PANEL_X+i*(zw+10),M8_PANEL_Y,zw,zh};
    }
}

static float Zone_Density(OceanZone *z) {
    return z->salinity*0.6f+(1.0f-z->temp)*0.4f;
}

static float Compute_Circulation(void) {
    float d_arctic=Zone_Density(&m8.zones[0]);
    float d_tropical=Zone_Density(&m8.zones[2]);
    float gradient=d_arctic-d_tropical;
    if (gradient<0) gradient=0;
    return gradient/0.6f;
}

static void Mission8_InitParticles(void) {
    for (int i=0;i<M8_MAX_PARTICLES;i++) {
        m8.particles[i].active=true;
        m8.particles[i].current_zone=rand()%M8_ZONE_COUNT;
        m8.particles[i].t=(float)(rand()%100)/100.0f;
        m8.particles[i].speed=0.08f+(float)(rand()%50)/500.0f;
        m8.particles[i].color=ColorAlpha(WHITE,0.6f+(float)(rand()%30)/100.0f);
    }
}

static void Mission8_Init(void) {
    memset(&m8,0,sizeof(m8));
    Mission8_InitZones();
    Mission8_InitParticles();
    m8.selected_zone=-1;
    m8.slider_temp=m8.zones[0].temp;
    m8.slider_salt=m8.zones[0].salinity;
    m8.circulation_strength=Compute_Circulation();
    strncpy(m8.status_msg,"Selecciona una zona oceanica y ajusta sus parametros.",sizeof(m8.status_msg)-1);
    m8.status_col=WHITE;
    TriviaManager_StartMission(MISSION_8_CORRIENTES_OCEAN);
}

static void Mission8_UpdateSlider(Rectangle track,float *value,bool *dragging,float mouse_x) {
    if (*dragging) {
        *value=(mouse_x-track.x)/track.width;
        if (*value<0)*value=0; if (*value>1)*value=1;
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) *dragging=false;
    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 m=GetMousePosition();
        if (CheckCollisionPointRec(m,track)) *dragging=true;
    }
}

static void Mission8_Update(float dt) {
    if (m8.trivia_phase) { TriviaManager_Update(dt); return; }

    Vector2 mouse=GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (int i=0;i<M8_ZONE_COUNT;i++) {
            if (CheckCollisionPointRec(mouse,m8.zones[i].rect)) {
                m8.selected_zone=i;
                m8.slider_temp=m8.zones[i].temp;
                m8.slider_salt=m8.zones[i].salinity;
                m8.interact_timer+=1.0f;
                break;
            }
        }
    }

    if (m8.selected_zone>=0) {
        Rectangle tr={800,260,200,20};
        Rectangle sr={800,330,200,20};
        Mission8_UpdateSlider(tr,&m8.slider_temp,&m8.dragging_temp,mouse.x);
        Mission8_UpdateSlider(sr,&m8.slider_salt,&m8.dragging_salt,mouse.x);
        if (m8.dragging_temp||m8.dragging_salt) m8.interact_timer+=dt;
        m8.zones[m8.selected_zone].temp=m8.slider_temp;
        m8.zones[m8.selected_zone].salinity=m8.slider_salt;
    }

    Rectangle ice_btn={800,390,200,50};
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&&CheckCollisionPointRec(mouse,ice_btn)) {
        m8.zones[0].salinity*=0.55f; m8.zones[4].salinity*=0.55f;
        m8.show_ice_melt_effect=true; m8.ice_melt_timer=3.0f;
        strncpy(m8.status_msg,"El deshielo reduce la salinidad polar! Esto puede colapsar la AMOC.",sizeof(m8.status_msg)-1);
        m8.status_col=COL_WRONG;
        m8.interact_timer+=3.0f;
    }

    float target_str=Compute_Circulation();
    m8.circulation_strength+=(target_str-m8.circulation_strength)*2.0f*dt;
    m8.amoc_collapsed=(m8.circulation_strength<0.15f);
    if (m8.show_ice_melt_effect) { m8.ice_melt_timer-=dt; if(m8.ice_melt_timer<=0) m8.show_ice_melt_effect=false; }

    /* desbloquear trivia tras ~10s de interaccion */
    m8.interact_timer+=dt*0.5f; /* tiempo normal tambien cuenta */
    if (!m8.minigame_done&&m8.interact_timer>=10.0f) {
        m8.minigame_done=true; m8.trivia_phase=true;
    }

    float speed_mult=m8.circulation_strength;
    for (int i=0;i<M8_MAX_PARTICLES;i++) {
        ThermoParticle *p=&m8.particles[i];
        if (!p->active) continue;
        p->t+=p->speed*speed_mult*dt*0.5f;
        if (p->t>=1.0f) { p->t=0.0f; p->current_zone=(p->current_zone+1)%M8_ZONE_COUNT; }
        int next=(p->current_zone+1)%M8_ZONE_COUNT;
        OceanZone *za=&m8.zones[p->current_zone], *zb=&m8.zones[next];
        float cx_a=za->rect.x+za->rect.width/2, cy_a=za->rect.y+za->rect.height*(1.0f-Zone_Density(za));
        float cx_b=zb->rect.x+zb->rect.width/2, cy_b=zb->rect.y+zb->rect.height*(1.0f-Zone_Density(zb));
        p->pos.x=cx_a+(cx_b-cx_a)*p->t;
        p->pos.y=cy_a+(cy_b-cy_a)*p->t;
        float temp=za->temp+(zb->temp-za->temp)*p->t;
        p->color=(Color){(unsigned char)(temp*220),(unsigned char)(50+(1-temp)*80),(unsigned char)((1-temp)*220),200};
    }
}

static void Mission8_DrawSlider(Rectangle track,float value,const char *label,Color fill) {
    DrawRectangleRec(track,COL_UI_BORDER);
    DrawRectangle((int)track.x,(int)track.y,(int)(track.width*value),(int)track.height,fill);
    DrawRectangleLinesEx(track,1.5f,WHITE);
    float tx=track.x+track.width*value;
    DrawCircle((int)tx,(int)(track.y+track.height/2),11,WHITE);
    DrawText(label,(int)track.x,(int)(track.y-22),16,LIGHTGRAY);
    char pct[16]; snprintf(pct,sizeof(pct),"%.0f%%",value*100);
    DrawText(pct,(int)(track.x+track.width+8),(int)track.y,16,WHITE);
}

static void Mission8_DrawMinigame(void) {
    DrawText("MISION 8 — Circulacion Termohalina",20,14,26,COL_UI_ACCENT);
    DrawText("Ajusta temperatura y salinidad de las zonas oceanicas",20,48,20,LIGHTGRAY);

    for (int i=0;i<M8_ZONE_COUNT;i++) {
        OceanZone *z=&m8.zones[i];
        float density=Zone_Density(z);
        DrawRectangleRec(z->rect,ColorAlpha(z->color,0.45f));
        DrawRectangleLinesEx(z->rect,m8.selected_zone==i?3.5f:1.5f,
                              m8.selected_zone==i?COL_UI_ACCENT:COL_UI_BORDER);
        int bar_h=(int)(z->rect.height*density);
        DrawRectangle((int)z->rect.x+5,(int)(z->rect.y+z->rect.height-bar_h),(int)z->rect.width-10,bar_h,ColorAlpha(DARKBLUE,0.48f));
        DrawText(z->name,(int)(z->rect.x+5),(int)(z->rect.y+8),15,WHITE);
        char ti[16],si[16],di[16];
        snprintf(ti,sizeof(ti),"T:%.0f%%",z->temp*100);
        snprintf(si,sizeof(si),"S:%.0f%%",z->salinity*100);
        snprintf(di,sizeof(di),"D:%.2f",density);
        DrawText(ti,(int)(z->rect.x+5),(int)(z->rect.y+30),14,COL_PLASTIC_YEL);
        DrawText(si,(int)(z->rect.x+5),(int)(z->rect.y+48),14,SKYBLUE);
        DrawText(di,(int)(z->rect.x+5),(int)(z->rect.y+66),14,LIGHTGRAY);
        if (i<M8_ZONE_COUNT-1) {
            float ax=z->rect.x+z->rect.width+3, ay=z->rect.y+z->rect.height/2;
            float str=m8.circulation_strength;
            Color ac=str>0.5f?COL_CORRECT:(str>0.2f?COL_PLASTIC_YEL:COL_WRONG);
            DrawLineEx((Vector2){ax,ay},(Vector2){ax+8,ay},3*str+1,ac);
        }
    }

    /* partículas */
    for (int i=0;i<M8_MAX_PARTICLES;i++) {
        ThermoParticle *p=&m8.particles[i];
        if (!p->active) continue;
        DrawCircleV(p->pos,5,p->color);
    }

    /* panel control derecho */
    Rectangle panel={780,160,260,SCREEN_H-200};
    DrawRectangleRec(panel,COL_UI_PANEL);
    DrawRectangleLinesEx(panel,1.5f,COL_UI_BORDER);

    if (m8.selected_zone>=0) {
        DrawText(m8.zones[m8.selected_zone].name,792,170,20,COL_UI_ACCENT);
        Mission8_DrawSlider((Rectangle){800,260,200,20},m8.slider_temp,"Temperatura",COL_WRONG);
        Mission8_DrawSlider((Rectangle){800,330,200,20},m8.slider_salt,"Salinidad",SKYBLUE);
    } else {
        DrawText("Selecciona una zona",792,180,17,GRAY);
    }

    Rectangle ice_btn={800,390,200,50};
    if (UI_Button(ice_btn,"Simular deshielo polar",ColorAlpha(SKYBLUE,0.72f),BLACK)) {}
    DrawText("(reduce salinidad artica)",792,446,13,GRAY);

    DrawText("Fuerza AMOC:",792,510,18,WHITE);
    DrawRectangle(792,534,200,18,COL_UI_BORDER);
    Color sc=m8.amoc_collapsed?COL_WRONG:(m8.circulation_strength>0.5f?COL_CORRECT:COL_PLASTIC_YEL);
    DrawRectangle(792,534,(int)(200*m8.circulation_strength),18,sc);
    if (m8.amoc_collapsed) DrawText("AMOC COLAPSADA!",792,558,16,COL_WRONG);

    if (m8.show_ice_melt_effect) {
        DrawRectangle(M8_PANEL_X,M8_PANEL_Y,32,M8_PANEL_H,ColorAlpha(SKYBLUE,0.40f*(m8.ice_melt_timer/3.0f)));
        DrawRectangle((int)(M8_PANEL_X+(M8_ZONE_COUNT-1)*140.0f),M8_PANEL_Y,32,M8_PANEL_H,ColorAlpha(SKYBLUE,0.40f*(m8.ice_melt_timer/3.0f)));
    }

    UI_DrawWrappedText(m8.status_msg,M8_PANEL_X,M8_PANEL_Y+M8_PANEL_H+14,760,17,m8.status_col);

    /* barra de progreso de interaccion */
    float pct=m8.interact_timer/10.0f; if(pct>1)pct=1;
    DrawText("Interactua para continuar:",M8_PANEL_X,SCREEN_H-50,16,GRAY);
    DrawRectangle(M8_PANEL_X+240,SCREEN_H-50,400,16,COL_UI_BORDER);
    DrawRectangle(M8_PANEL_X+240,SCREEN_H-50,(int)(400*pct),16,COL_UI_ACCENT);
}

static void Mission8_DrawTrivia(void) {
    DrawRectangle(0,0,GAME_W,SCREEN_H,COL_UI_BG);
    const char *inst="MISION 8 — Trivia: Corrientes Oceanicas";
    int iw=MeasureText(inst,28); DrawText(inst,GAME_W/2-iw/2,20,28,COL_UI_ACCENT);
    TriviaManager_DrawPanel((Rectangle){(GAME_W-820)/2.0f,70,820,SCREEN_H-80});
}

static void Mission8_Draw(void) {
    ClearBackground(COL_UI_BG);
    if (!m8.trivia_phase) {
        Mission8_DrawMinigame();
        Rectangle back={GAME_W-200,SCREEN_H-60,180,44};
        UI_Button(back,"Menu Principal",COL_UI_PANEL,WHITE);
    } else { Mission8_DrawTrivia(); }
}

void Mission8_Run(void) {
    bool go=ShowMissionIntro(8,
        "Corrientes Oceanicas",
        "La cinta transportadora que distribuye el plastico",
        "La Circulacion Termohalina, llamada 'cinta transportadora oceanica', mueve agua "
        "por todos los oceanos en un ciclo que puede durar hasta 1.000 anos. Funciona "
        "gracias a las diferencias de temperatura y salinidad: el agua fria y salada del "
        "Artico se hunde y arrastra toda la masa oceanica. Esta circulacion distribuye "
        "calor, oxigeno y nutrientes por el planeta, regulando el clima global. El cambio "
        "climatico amenaza este sistema: el deshielo polar agrega agua dulce que reduce "
        "la salinidad y puede debilitar o colapsar la AMOC (Atlantic Meridional Overturning "
        "Circulation), con consecuencias catastróficas para el clima europeo y la "
        "distribucion del plastico en los oceanos. Explora y experimenta con los parametros.");
    if (!go) { g_current_scene=SCENE_MAIN_MENU; return; }
    Mission8_Init();
    while (!WindowShouldClose()) {
        float dt=GetFrameTime();
        Mission8_Update(dt);
        BeginDrawing(); Mission8_Draw(); EndDrawing();
        if (g_trivia.state==TRIVIA_MISSION_COMPLETE) break;
        if (!m8.trivia_phase) {
            Rectangle back={GAME_W-200,SCREEN_H-60,180,44};
            Vector2 mouse=GetMousePosition();
            if (CheckCollisionPointRec(mouse,back)&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { g_current_scene=SCENE_MAIN_MENU; return; }
        }
    }
    g_current_scene=SCENE_RESULTS;
}