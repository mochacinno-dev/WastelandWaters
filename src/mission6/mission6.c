#include "../core/plastic_game.h"

/* ──────────────────────────────────────────────────
   MISIÓN 6 — Banda Transportadora de Reciclaje
   LÍMITE: 15 piezas de basura en total.
────────────────────────────────────────────────── */

#define M6_ITEM_DEF_COUNT   8
#define M6_MAX_ITEMS       15   /* limite de piezas */
#define M6_BAND_Y_TOP     300
#define M6_BAND_Y_BOT     390
#define M6_SPAWN_X       -170.0f
#define M6_FALL_X        1100.0f

typedef enum { PROCESS_MECANICO=0, PROCESS_QUIMICO, PROCESS_DESECHO, PROCESS_COUNT } RecycleProcess;

typedef struct {
    char           name[64];
    char           plastic_type[32];
    RecycleProcess correct_process;
    char           reason[256];
    Color          color;
} ConveyorItemDef;

typedef struct {
    ConveyorItemDef def;
    float           x;
    float           speed;
    bool            active;
    bool            classified;
    RecycleProcess  player_choice;
    bool            is_correct;
} ConveyorItem;

typedef struct {
    ConveyorItemDef defs[M6_ITEM_DEF_COUNT];
    ConveyorItem    item;
    bool            item_waiting;
    float           spawn_timer;
    float           spawn_interval;
    int             def_index;
    int             score;
    int             total_classified;
    int             correct_count;
    float           belt_anim;
    char            feedback_msg[256];
    Color           feedback_col;
    float           feedback_timer;
    bool            minigame_done;
    bool            trivia_phase;
} Mission6State;

static Mission6State m6;

static const ConveyorItemDef s_defs[M6_ITEM_DEF_COUNT] = {
    {"Botella de agua PET","PET",PROCESS_MECANICO,
     "El PET limpio se recicla mecanicamente para fabricar nuevas botellas o fibra textil.",COL_PLASTIC_YEL},
    {"Tuberia de PVC","PVC",PROCESS_DESECHO,
     "El PVC libera dioxinas al reciclarse mecanicamente; generalmente va a desecho.",RED},
    {"Bolsa LDPE","LDPE",PROCESS_MECANICO,
     "El LDPE puede reciclarse mecanicamente en bolsas y tuberias de baja presion.",GREEN},
    {"Envase de aceite HDPE","HDPE",PROCESS_MECANICO,
     "El HDPE limpio se recicla eficientemente para tuberias y muebles de exterior.",BLUE},
    {"Vaso de unicel EPS","PS",PROCESS_DESECHO,
     "El poliestireno expandido es dificil de reciclar y casi siempre termina en relleno.",ORANGE},
    {"Plastico mixto #7","Otro",PROCESS_QUIMICO,
     "Los plasticos #7 y mixtos requieren reciclaje quimico para despolimerizarse.",PURPLE},
    {"Botella PET negra","PET negro",PROCESS_DESECHO,
     "Los sensores opticos no detectan el negro; la mayoria de plantas lo envia a desecho.",DARKGRAY},
    {"Polipropileno limpio","PP",PROCESS_MECANICO,
     "El PP limpio (envases, tapas) se recicla mecanicamente con alta eficiencia.",VIOLET},
};

static void M6_SpawnItem(void) {
    m6.item.def     = m6.defs[m6.def_index % M6_ITEM_DEF_COUNT];
    m6.item.x       = M6_SPAWN_X;
    m6.item.speed   = 120.0f + (float)(rand()%60);
    m6.item.active  = true;
    m6.item.classified = false;
    m6.def_index++;
    m6.item_waiting = false;
}

static void Mission6_Init(void) {
    memset(&m6,0,sizeof(m6));
    memcpy(m6.defs,s_defs,sizeof(s_defs));
    m6.spawn_interval=0.4f;
    M6_SpawnItem();
    m6.spawn_interval=3.8f;
    TriviaManager_StartMission(MISSION_6_RECICLAJE);
}

static void M6_ClassifyItem(RecycleProcess choice) {
    if (!m6.item.active||m6.item.classified) return;
    m6.item.classified=true;
    m6.item.player_choice=choice;
    m6.item.is_correct=(choice==m6.item.def.correct_process);
    m6.total_classified++;
    if (m6.item.is_correct) {
        m6.score+=100; m6.correct_count++;
        strncpy(m6.feedback_msg,m6.item.def.reason,sizeof(m6.feedback_msg)-1);
        m6.feedback_col=COL_CORRECT;
    } else {
        const char *pn[PROCESS_COUNT]={"Mecanico","Quimico","Desecho"};
        char tmp[256];
        snprintf(tmp,sizeof(tmp),"Incorrecto. Era %s. %s",pn[m6.item.def.correct_process],m6.item.def.reason);
        strncpy(m6.feedback_msg,tmp,sizeof(m6.feedback_msg)-1);
        m6.feedback_col=COL_WRONG;
    }
    m6.feedback_timer=3.2f;
    m6.item.active=false;
    m6.spawn_timer=0.0f;

    /* comprobar limite */
    if (m6.total_classified>=M6_MAX_ITEMS) {
        m6.minigame_done=true;
        m6.trivia_phase=true;
    }
}

static void Mission6_Update(float dt) {
    if (m6.trivia_phase) { TriviaManager_Update(dt); return; }

    m6.belt_anim+=dt*70.0f;

    if (m6.item.active&&!m6.item.classified) {
        m6.item.x+=m6.item.speed*dt;
        if (m6.item.x>M6_FALL_X) M6_ClassifyItem(PROCESS_DESECHO);
    }
    if (m6.feedback_timer>0) {
        m6.feedback_timer-=dt;
        if (m6.feedback_timer<=0) m6.feedback_msg[0]='\0';
    }
    if (!m6.item.active && !m6.minigame_done) {
        m6.spawn_timer+=dt;
        if (m6.spawn_timer>=m6.spawn_interval) { M6_SpawnItem(); m6.spawn_timer=0; }
    }

    /* botones clasificación */
    const char *btn_labels[PROCESS_COUNT]={"MECANICO","QUIMICO","DESECHO"};
    Color btn_colors[PROCESS_COUNT]={COL_PLASTIC_GRN,COL_UI_ACCENT,COL_WRONG};
    float btn_w=200,btn_h=66;
    float btn_y=SCREEN_H-90.0f;
    float total_w=PROCESS_COUNT*btn_w+(PROCESS_COUNT-1)*30.0f;
    float start_x=(GAME_W-total_w)/2.0f;
    for (int i=0;i<PROCESS_COUNT;i++) {
        Rectangle r={start_x+i*(btn_w+30),btn_y,btn_w,btn_h};
        if (UI_Button(r,btn_labels[i],btn_colors[i],BLACK)) M6_ClassifyItem((RecycleProcess)i);
    }
}

static void Mission6_DrawBand(void) {
    DrawRectangle(0,M6_BAND_Y_TOP-24,GAME_W,106,DARKGRAY);
    int offset=(int)m6.belt_anim%36;
    for (int x=-36+offset;x<GAME_W+36;x+=36)
        DrawLine(x,M6_BAND_Y_TOP-24,x+36,M6_BAND_Y_BOT+24,ColorAlpha(BLACK,0.28f));
    DrawRectangle(0,M6_BAND_Y_TOP-26,GAME_W,8,GRAY);
    DrawRectangle(0,M6_BAND_Y_BOT+18,GAME_W,8,GRAY);
    DrawCircle(50,(M6_BAND_Y_TOP+M6_BAND_Y_BOT)/2,28,DARKGRAY);
    DrawCircleLinesV((Vector2){50,(M6_BAND_Y_TOP+M6_BAND_Y_BOT)/2.0f},28,GRAY);
    DrawCircle(GAME_W-50,(M6_BAND_Y_TOP+M6_BAND_Y_BOT)/2,28,DARKGRAY);
    DrawCircleLinesV((Vector2){GAME_W-50,(M6_BAND_Y_TOP+M6_BAND_Y_BOT)/2.0f},28,GRAY);
}

static void Mission6_DrawItem(void) {
    if (!m6.item.active) return;
    float iy=(M6_BAND_Y_TOP+M6_BAND_Y_BOT)/2.0f-38;
    Rectangle r={m6.item.x,iy,150,76};
    DrawRectangleRec(r,ColorAlpha(m6.item.def.color,0.82f));
    DrawRectangleLinesEx(r,2.5f,m6.item.def.color);
    UI_DrawWrappedText(m6.item.def.name,(int)(m6.item.x+6),(int)(iy+6),140,16,BLACK);
    DrawText(m6.item.def.plastic_type,(int)(m6.item.x+6),(int)(iy+50),18,WHITE);
}

static void Mission6_DrawMinigame(void) {
    DrawText("MISION 6 — Banda Transportadora de Reciclaje",20,14,26,COL_UI_ACCENT);
    DrawText("Como se recicla este plastico? Decide antes de que caiga.",20,48,20,LIGHTGRAY);

    Mission6_DrawBand();
    Mission6_DrawItem();

    /* contadores */
    char hud[80];
    snprintf(hud,sizeof(hud),"Clasificados: %d/%d   Correctos: %d   Puntaje: %d",
             m6.total_classified,M6_MAX_ITEMS,m6.correct_count,m6.score);
    DrawText(hud,20,260,20,WHITE);

    /* barra de urgencia */
    if (m6.item.active) {
        float pct=(m6.item.x-M6_SPAWN_X)/(M6_FALL_X-M6_SPAWN_X);
        DrawRectangle(0,M6_BAND_Y_BOT+28,GAME_W,12,COL_UI_BORDER);
        Color uc=pct<0.5f?COL_CORRECT:(pct<0.8f?COL_PLASTIC_YEL:COL_WRONG);
        DrawRectangle(0,M6_BAND_Y_BOT+28,(int)(GAME_W*pct),12,uc);
    }

    /* progreso de ítems */
    DrawRectangle(20,M6_BAND_Y_BOT+48,GAME_W-40,10,COL_UI_BORDER);
    DrawRectangle(20,M6_BAND_Y_BOT+48,(int)((GAME_W-40)*(float)m6.total_classified/M6_MAX_ITEMS),10,COL_UI_ACCENT);
    char prog[40]; snprintf(prog,sizeof(prog),"Piezas restantes: %d",M6_MAX_ITEMS-m6.total_classified);
    DrawText(prog,20,M6_BAND_Y_BOT+64,16,GRAY);

    /* feedback */
    if (m6.feedback_msg[0]) {
        Rectangle fb={10,M6_BAND_Y_BOT+84,GAME_W-20,80};
        DrawRectangleRec(fb,ColorAlpha(BLACK,0.72f));
        UI_DrawWrappedText(m6.feedback_msg,18,M6_BAND_Y_BOT+92,GAME_W-36,16,m6.feedback_col);
    }

    /* botones */
    const char *btn_labels[PROCESS_COUNT]={"MECANICO","QUIMICO","DESECHO"};
    Color btn_colors[PROCESS_COUNT]={COL_PLASTIC_GRN,COL_UI_ACCENT,COL_WRONG};
    float btn_w=200,btn_h=66;
    float btn_y=SCREEN_H-90.0f;
    float total_w=PROCESS_COUNT*btn_w+(PROCESS_COUNT-1)*30.0f;
    float start_x=(GAME_W-total_w)/2.0f;
    for (int i=0;i<PROCESS_COUNT;i++) {
        Rectangle r={start_x+i*(btn_w+30),btn_y,btn_w,btn_h};
        DrawRectangleRec(r,ColorAlpha(btn_colors[i],0.82f));
        DrawRectangleLinesEx(r,2.5f,btn_colors[i]);
        int tw=MeasureText(btn_labels[i],22);
        DrawText(btn_labels[i],(int)(r.x+r.width/2-tw/2),(int)(r.y+r.height/2-11),22,BLACK);
    }
}

static void Mission6_DrawTrivia(void) {
    DrawRectangle(0,0,GAME_W,SCREEN_H,COL_UI_BG);
    const char *inst="MISION 6 — Trivia: Reciclaje y Desechos";
    int iw=MeasureText(inst,28); DrawText(inst,GAME_W/2-iw/2,20,28,COL_UI_ACCENT);
    TriviaManager_DrawPanel((Rectangle){(GAME_W-820)/2.0f,70,820,SCREEN_H-80});
}

static void Mission6_Draw(void) {
    ClearBackground(COL_UI_BG);
    if (!m6.trivia_phase) {
        Mission6_DrawMinigame();
        Rectangle back={GAME_W-200,SCREEN_H-90,180,44};
        UI_Button(back,"Menu Principal",COL_UI_PANEL,WHITE);
    } else { Mission6_DrawTrivia(); }
}

void Mission6_Run(void) {
    bool go=ShowMissionIntro(6,
        "Reciclaje y Desechos",
        "El reciclaje no es la solucion magica al Gran Parche",
        "Solo el 9% del plastico producido en la historia ha sido reciclado. El 79% restante "
        "esta en vertederos o en el medio ambiente, incluyendo el Gran Parche del Pacifico. "
        "El reciclaje tiene limitaciones reales: el PVC contamina todo un lote de reciclaje, "
        "el poliestireno raramente se recicla, y muchos plasticos negros son invisibles para "
        "los sensores de clasificacion. La verdadera solucion empieza por reducir el consumo "
        "desde la fuente. En este minijuego clasificaras 15 piezas de plastico segun el "
        "proceso de reciclaje correcto: mecanico, quimico o desecho.");
    if (!go) { g_current_scene=SCENE_MAIN_MENU; return; }
    Mission6_Init();
    while (!WindowShouldClose()) {
        float dt=GetFrameTime();
        Mission6_Update(dt);
        BeginDrawing(); Mission6_Draw(); EndDrawing();
        if (g_trivia.state==TRIVIA_MISSION_COMPLETE) break;
        if (!m6.trivia_phase) {
            Rectangle back={GAME_W-200,SCREEN_H-90,180,44};
            Vector2 mouse=GetMousePosition();
            if (CheckCollisionPointRec(mouse,back)&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { g_current_scene=SCENE_MAIN_MENU; return; }
        }
    }
    g_current_scene=SCENE_MISSION_7;
}