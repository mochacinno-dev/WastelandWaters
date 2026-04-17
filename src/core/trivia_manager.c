#include "plastic_game.h"

TriviaManager g_trivia;
SceneID       g_current_scene = SCENE_MAIN_MENU;

/* ──────────────────────────────────────────────────
   BANCO DE 50 PREGUNTAS
────────────────────────────────────────────────── */
static Question s_question_bank[MAX_QUESTIONS] = {
/* ── MISIÓN 1 — Tipos de Plástico ── */
{0,MISSION_1_TIPOS_PLASTICO,
 "¿Cuál es el nombre del plástico más utilizado en la fabricación de botellas de agua?",
 {"PVC","HDPE","PET","PP"},2,
 "El PET (Polietileno Tereftalato) es el plástico #1 y el más usado en botellas de bebidas por su ligereza y transparencia."},

{1,MISSION_1_TIPOS_PLASTICO,
 "La sigla HDPE corresponde a:",
 {"Hidrocarburo de Densidad Polietileno Estable","Polietileno de Alta Densidad",
  "Polipropileno de Dureza Elevada","Polietileno de Hidrógeno Denso Expandido"},1,
 "HDPE = High-Density Polyethylene = Polietileno de Alta Densidad. Se usa en envases de detergente y tuberías."},

{2,MISSION_1_TIPOS_PLASTICO,
 "El poliestireno (unicel) lleva en el símbolo de reciclaje el número:",
 {"4","5","3","6"},3,
 "El poliestireno tiene el número 6 en la clasificación de plásticos. Su reciclaje es difícil y costoso."},

{3,MISSION_1_TIPOS_PLASTICO,
 "¿Cuál de los siguientes plásticos se usa comúnmente en tuberías de construcción?",
 {"PET","PP","PVC","LDPE"},2,
 "El PVC (Policloruro de Vinilo) es resistente y económico, ideal para tuberías, ventanas y suelos."},

{4,MISSION_1_TIPOS_PLASTICO,
 "Las bolsas de supermercado de baja densidad están fabricadas principalmente con:",
 {"PET","PVC","HDPE","LDPE"},3,
 "El LDPE (Low-Density Polyethylene, #4) es flexible y translúcido, perfecto para bolsas y films."},

{5,MISSION_1_TIPOS_PLASTICO,
 "El plástico número 7 es considerado el más problemático porque:",
 {"Es el más abundante en los océanos",
  "Agrupa plásticos difíciles de reciclar y puede contener BPA",
  "Solo se produce en países con poca regulación ambiental",
  "No puede ser identificado por ningún sistema actual"},1,
 "El #7 es la categoría 'otros'. Incluye policarbonato (con BPA), bioplásticos y mezclas difíciles de reciclar."},

/* ── MISIÓN 2 — Polímeros ── */
{6,MISSION_2_POLIMEROS,
 "Un polímero se define como:",
 {"Una molécula simple producida en laboratorios",
  "Un compuesto orgánico solo de fuentes petroquímicas",
  "Una macromolécula formada por la unión repetida de monómeros",
  "Un elemento de la tabla periódica con propiedades plásticas"},2,
 "Los polímeros son cadenas largas de monómeros repetidos. 'Poly' = muchos; 'meros' = partes."},

{7,MISSION_2_POLIMEROS,
 "El monómero que da origen al polietileno es:",
 {"Propileno","Estireno","Etileno (eteno)","Benceno"},2,
 "El polietileno se obtiene polimerizando etileno (CH2=CH2). Es el plástico más producido del mundo."},

{8,MISSION_2_POLIMEROS,
 "La principal diferencia entre un polímero sintético y uno natural es que:",
 {"Los naturales son siempre más resistentes",
  "Los sintéticos son producidos por el ser humano; los naturales existen en la naturaleza",
  "Los sintéticos se degradan más rápido",
  "Los naturales contienen más carbono"},1,
 "Los polímeros naturales (celulosa, seda, caucho) existen en la naturaleza. Los sintéticos (nylon, PET) son fabricados por el ser humano."},

{9,MISSION_2_POLIMEROS,
 "¿Cuál de los siguientes es un polímero natural?",
 {"Nylon","PVC","Poliestireno","Celulosa"},3,
 "La celulosa es el polímero natural más abundante; forma las paredes de las células vegetales."},

{10,MISSION_2_POLIMEROS,
 "Los plásticos son tan resistentes a la degradación principalmente porque:",
 {"Contienen metales pesados que repelen microorganismos",
  "Sus largas cadenas moleculares son difíciles de romper por microorganismos",
  "Se producen a temperaturas extremas que los vuelven inertes",
  "Absorben la humedad y se vuelven impermeables"},1,
 "Las cadenas de carbono de los plásticos son muy estables. Los microorganismos no poseen las enzimas necesarias para romperlas eficientemente."},

{11,MISSION_2_POLIMEROS,
 "La fotodegradación en los plásticos es el proceso por el cual:",
 {"El plástico se convierte en gas al contacto con la luz solar",
  "Los microorganismos descomponen el plástico usando energía lumínica",
  "La luz UV fragmenta el plástico en partículas más pequeñas sin eliminarlo",
  "El plástico absorbe la radiación y se recicla de forma natural"},2,
 "La fotodegradación produce microplásticos: el plástico se rompe en fragmentos cada vez más pequeños pero NO desaparece."},

{12,MISSION_2_POLIMEROS,
 "¿Cuánto tiempo puede tardar en degradarse una botella de PET en el océano?",
 {"Entre 10 y 50 años","Hasta 450 años","Aproximadamente 100 años","Entre 200 y 250 años"},1,
 "Una botella de PET puede tardar hasta 450 años en degradarse en el océano, liberando microplásticos durante todo ese tiempo."},

/* ── MISIÓN 3 — Contaminantes ── */
{13,MISSION_3_CONTAMINANTES,
 "Los microplásticos se definen como:",
 {"Plásticos biodegradables de tamaño reducido",
  "Fragmentos de plástico menores a 5 milímetros",
  "Partículas plásticas visibles que flotan en el océano",
  "Residuos generados exclusivamente por la industria textil"},1,
 "Los microplásticos son fragmentos < 5mm. Los nanoplásticos son < 1 micrómetro y aún más peligrosos."},

{14,MISSION_3_CONTAMINANTES,
 "Los microplásticos fabricados intencionalmente para cosméticos se conocen como:",
 {"Nanoplásticos industriales","Microplásticos secundarios",
  "Microplásticos primarios o microesferas","Pellets de procesamiento cosmético"},2,
 "Los microplásticos primarios (microbeads) se añaden deliberadamente a exfoliantes y cosméticos. Muchos países los han prohibido."},

{15,MISSION_3_CONTAMINANTES,
 "El BPA (Bisfenol A) puede afectar principalmente:",
 {"El sistema respiratorio al ser inhalado",
  "La resistencia de otros plásticos cercanos",
  "El sistema endocrino de los seres vivos",
  "La salinidad del agua al disolverse"},2,
 "El BPA es un disruptor endocrino: imita a los estrógenos y puede alterar el desarrollo hormonal en humanos y animales."},

{16,MISSION_3_CONTAMINANTES,
 "Respecto a microplásticos en el cuerpo humano, los estudios han encontrado que:",
 {"Solo se acumulan en el sistema digestivo",
  "Han sido detectados en sangre humana",
  "El cuerpo los elimina en menos de 72 horas",
  "Solo afectan a personas cerca de zonas costeras"},1,
 "En 2022, investigadores publicaron en Environment International la detección de microplásticos en sangre humana de donantes anónimos."},

{17,MISSION_3_CONTAMINANTES,
 "Los contaminantes persistentes orgánicos (POP) se relacionan con el plástico porque:",
 {"Son liberados solo durante el reciclaje",
  "Son sustancias tóxicas que el plástico absorbe y concentra en el océano",
  "Solo se encuentran en plásticos tipo 1 y 2",
  "Se producen cuando el plástico toca el agua salada"},1,
 "Los plásticos actúan como 'esponjas' de POPs como PCBs y DDT, concentrándolos miles de veces por encima del agua circundante."},

{18,MISSION_3_CONTAMINANTES,
 "El organismo marino especialmente vulnerable a confundir microplásticos con alimento es:",
 {"El tiburón blanco","El zooplancton",
  "El delfín nariz de botella","El coral de aguas profundas"},1,
 "El zooplancton, base de la cadena alimentaria marina, ingiere microplásticos creyendo que son fitoplancton, contaminando toda la red trófica."},

{19,MISSION_3_CONTAMINANTES,
 "Los microplásticos ingresan a la cadena alimentaria humana principalmente a través de:",
 {"La respiración de aire en zonas industriales costeras",
  "El consumo de peces, mariscos, agua y sal marina contaminados",
  "El contacto directo con arena de playas contaminadas",
  "El uso de utensilios plásticos de un solo uso"},1,
 "Mariscos, pescados, agua del grifo y sal marina son las principales vías de ingesta de microplásticos en humanos."},

/* ── MISIÓN 4 — Países ── */
{20,MISSION_4_PAISES,
 "El país que más plástico emite a los océanos en el mundo es:",
 {"China","India","Filipinas","Indonesia"},0,
 "China encabeza la lista de contaminación plástica oceánica, seguida de Indonesia, Filipinas y Vietnam, según estudios de Ocean Conservancy."},

{21,MISSION_4_PAISES,
 "La región del mundo que aporta la mayor proporción de plástico oceánico global es:",
 {"Europa Occidental","América del Norte","África Subsahariana","Asia, especialmente el Sudeste Asiático"},3,
 "El Sudeste Asiático representa más del 60% del plástico oceánico global, impulsado por alta densidad poblacional y sistemas de gestión de residuos limitados."},

{22,MISSION_4_PAISES,
 "¿Qué porcentaje aproximado de la basura plástica marina proviene de ríos?",
 {"30%","55%","80%","95%"},2,
 "Aproximadamente el 80% del plástico marino proviene de fuentes terrestres transportadas por ríos, según estudios publicados en Science (2015)."},

{23,MISSION_4_PAISES,
 "El río que más plástico descarga al océano en el mundo es:",
 {"El río Ganges, en India","El río Yangtsé, en China",
  "El río Mekong, en Vietnam","El río Amazonas, en Brasil"},1,
 "El río Yangtsé (Chang Jiang) descarga la mayor cantidad de plástico al océano, contribuyendo con ~0.3 millones de toneladas anuales."},

{24,MISSION_4_PAISES,
 "Respecto a la contribución de EE.UU. al Gran Parche, es correcto afirmar que:",
 {"No contribuye significativamente",
  "Es el segundo mayor contaminador oceánico",
  "Contribuye exportando residuos a países que los manejan deficientemente",
  "Solo contamina ríos internos"},2,
 "EE.UU. exporta millones de toneladas de residuos plásticos a países asiáticos donde parte termina en el océano, según investigaciones de The Last Beach Cleanup."},

{25,MISSION_4_PAISES,
 "El país de América Latina entre los mayores contaminadores plásticos oceánicos es:",
 {"México","Colombia","Argentina","Brasil"},3,
 "Brasil figura entre los 20 mayores contaminadores plásticos del océano a nivel mundial, principalmente a través de sus ríos costeros."},

/* ── MISIÓN 5 — Corrientes Superficiales ── */
{26,MISSION_5_CORRIENTES_SUP,
 "La fuerza principal responsable de las corrientes superficiales del océano es:",
 {"La gravedad lunar","El viento",
  "La diferencia de temperatura","La rotación del núcleo terrestre"},1,
 "El viento transfiere energía a la superficie oceánica por fricción, generando las corrientes superficiales en los primeros 100-200m del océano."},

{27,MISSION_5_CORRIENTES_SUP,
 "El efecto que desvía las corrientes oceánicas por la rotación de la Tierra se llama:",
 {"Efecto Doppler","Efecto Venturi","Efecto Coriolis","Efecto Bernoulli"},2,
 "El Efecto Coriolis, producido por la rotación terrestre, desvía las corrientes a la derecha en el hemisferio norte y a la izquierda en el sur."},

{28,MISSION_5_CORRIENTES_SUP,
 "En el hemisferio norte, el efecto Coriolis desvía las corrientes hacia:",
 {"El sur","La izquierda","El norte","La derecha"},3,
 "En el hemisferio norte las corrientes giran en sentido horario (hacia la derecha). En el sur, en sentido antihorario."},

{29,MISSION_5_CORRIENTES_SUP,
 "Un giro oceánico es:",
 {"Una corriente profunda de agua fría",
  "Un sistema circular de corrientes que giran alrededor de un centro de alta presión",
  "El movimiento vertical del agua por cambios de temperatura",
  "Una corriente generada por tormentas tropicales"},1,
 "Los giros oceánicos son grandes sistemas circulares de corrientes superficiales creados por el efecto Coriolis y los vientos alisios y del oeste."},

{30,MISSION_5_CORRIENTES_SUP,
 "¿Cuántos grandes giros oceánicos existen en los océanos del mundo?",
 {"3","7","5","9"},2,
 "Existen 5 grandes giros oceánicos: Pacífico Norte, Pacífico Sur, Atlántico Norte, Atlántico Sur e Índico."},

{31,MISSION_5_CORRIENTES_SUP,
 "La corriente superficial que baña las costas occidentales de México y California se llama:",
 {"Corriente del Golfo","Corriente de Humboldt",
  "Corriente de California","Corriente Ecuatorial Norte"},2,
 "La Corriente de California fluye de norte a sur a lo largo de la costa occidental de Norteamérica, transportando aguas frías del Pacífico Norte."},

/* ── MISIÓN 6 — Reciclaje y Desechos ── */
{32,MISSION_6_RECICLAJE,
 "Del total de plástico producido en la historia, el porcentaje reciclado es aproximadamente:",
 {"9%","25%","40%","60%"},0,
 "Solo el 9% del plástico producido ha sido reciclado. El 12% se ha incinerado y el 79% restante está en vertederos o en la naturaleza (Science Advances, 2017)."},

{33,MISSION_6_RECICLAJE,
 "El símbolo de las tres flechas en un producto plástico indica que:",
 {"El producto fue fabricado 100% reciclado",
  "Puede desecharse en cualquier contenedor",
  "Indica el tipo de plástico y si puede reciclarse, sin garantizarlo",
  "Cumple normas de biodegradabilidad"},2,
 "El símbolo de reciclaje con número identifica el tipo de resina. No garantiza que existan instalaciones para reciclarlo en tu área."},

{34,MISSION_6_RECICLAJE,
 "El reciclaje mecánico consiste en:",
 {"Descomponer el plástico en componentes químicos usando calor extremo",
  "Triturar y fundir plásticos para crear nuevos materiales sin alterar su composición química",
  "Convertir el plástico en combustible por pirólisis",
  "Separar plásticos por color y peso con máquinas"},1,
 "El reciclaje mecánico tritura, lava y funde el plástico. Es el más común pero degrada la calidad del material en cada ciclo."},

{35,MISSION_6_RECICLAJE,
 "El reciclaje químico se diferencia del mecánico porque:",
 {"Solo aplica a plásticos transparentes",
  "Requiere menos energía y produce menos residuos",
  "Descompone el plástico en sus componentes químicos básicos para crear nuevos materiales",
  "Es el único que aplica a plásticos contaminados con alimentos"},2,
 "El reciclaje químico (pirólisis, gasificación, depolimerización) rompe las cadenas moleculares, permitiendo obtener materias primas vírgenes de nuevo."},

{36,MISSION_6_RECICLAJE,
 "El principal problema del reciclaje del plástico PVC es que:",
 {"Es demasiado frágil para el triturado",
  "Solo puede reciclarse una vez",
  "Libera sustancias tóxicas y contamina otros plásticos reciclables",
  "Su color negro impide clasificarlo ópticamente"},2,
 "El PVC contiene aditivos de cloro y plastificantes que se liberan durante el reciclaje, contaminando el lote completo de plásticos mixtos."},

{37,MISSION_6_RECICLAJE,
 "La regla de las 3R establece que las acciones deben aplicarse en orden:",
 {"Reciclar, Reducir y Reutilizar",
  "Reutilizar, Reciclar y Reducir",
  "Reducir, Reciclar y Reutilizar",
  "Reducir, Reutilizar y Reciclar"},3,
 "El orden correcto es Reducir (prioritario), Reutilizar y finalmente Reciclar. Reducir el consumo es siempre la mejor opción ambiental."},

{38,MISSION_6_RECICLAJE,
 "La producción mundial de plástico al año es aproximadamente:",
 {"50 millones de toneladas","Más de 400 millones de toneladas",
  "150 millones de toneladas","Cerca de 800 millones de toneladas"},1,
 "En 2022 se produjeron más de 400 millones de toneladas de plástico globalmente. A este ritmo, se proyectan 1,000 millones de toneladas para 2050."},

/* ── MISIÓN 7 — Islas de Plástico ── */
{39,MISSION_7_ISLAS_PLASTICO,
 "El nombre oficial del mayor acumulamiento de basura plástica en el Pacífico es:",
 {"Zona Muerta del Pacífico Norte",
  "Archipiélago de Residuos del Pacífico",
  "Gran Parche de Basura del Pacífico",
  "Depósito Central de Residuos Marinos"},2,
 "El Great Pacific Garbage Patch (Gran Parche de Basura del Pacífico) es el mayor de los 5 parches oceánicos conocidos."},

{40,MISSION_7_ISLAS_PLASTICO,
 "¿Cuántos grandes parches de basura existen en los océanos del mundo?",
 {"2","3","7","5"},3,
 "Existen 5 grandes parches de basura oceánica, uno por cada giro oceánico principal: 2 en el Pacífico, 2 en el Atlántico y 1 en el Índico."},

{41,MISSION_7_ISLAS_PLASTICO,
 "En cuanto a su apariencia, el Gran Parche de Basura del Pacífico:",
 {"Es una isla sólida visible desde satélites",
  "Es principalmente una concentración de microplásticos suspendidos en el agua",
  "Forma una capa densa de plásticos enteros",
  "Solo es detectable con sonar en profundidades"},1,
 "El Gran Parche no es una isla sólida; es una 'sopa' de microplásticos y partículas suspendidas que hace el agua turbia y lechosa."},

{42,MISSION_7_ISLAS_PLASTICO,
 "La cantidad aproximada de plástico en el Gran Parche de Basura del Pacífico es:",
 {"Alrededor de 10,000 toneladas",
  "Más de 2 millones de toneladas",
  "Alrededor de 80,000 toneladas",
  "Cerca de 500,000 toneladas"},1,
 "Estudios de The Ocean Cleanup estiman más de 80,000 toneladas de plástico, aunque investigaciones más recientes sugieren que puede superar los 2 millones de toneladas."},

{43,MISSION_7_ISLAS_PLASTICO,
 "El Gran Parche de Basura del Pacífico fue descubierto por:",
 {"David Attenborough en 2001",
  "La NASA mediante satélites en 1989",
  "El oceanógrafo Charles Moore, en 1997",
  "Investigadores de Greenpeace en 2003"},2,
 "Charles Moore, navegante y oceanógrafo, descubrió el Gran Parche en 1997 mientras navegaba desde Hawaii a California en el velero Alguita."},

/* ── MISIÓN 8 — Corrientes Oceánicas ── */
{44,MISSION_8_CORRIENTES_OCEAN,
 "El sistema global de corrientes oceánicas profundas se conoce como:",
 {"Corriente Circumpolar Antártica",
  "La Cinta Transportadora Oceánica o Circulación Termohalina",
  "Sistema de Giros de Profundidad Global",
  "Corriente de Densidad Planetaria"},1,
 "La Circulación Termohalina o 'Cinta Transportadora Oceánica' mueve agua por todos los océanos en un ciclo que puede durar 1,000 años."},

{45,MISSION_8_CORRIENTES_OCEAN,
 "Las corrientes termohalinas están determinadas por:",
 {"La velocidad del viento y la presión atmosférica",
  "La temperatura y la salinidad del agua",
  "La profundidad y la composición del lecho marino",
  "La temperatura superficial y la radiación solar"},1,
 "'Termo' = temperatura; 'halino' = salinidad. Estas dos variables determinan la densidad del agua y, por tanto, si sube o baja en la columna de agua."},

{46,MISSION_8_CORRIENTES_OCEAN,
 "El agua fría y salada tiende a hundirse en el océano porque:",
 {"Contiene menos oxígeno disuelto",
  "La presión atmosférica la empuja",
  "Es más densa que el agua cálida y menos salada",
  "Los vientos polares la arrastran hacia las profundidades"},2,
 "La densidad del agua aumenta al bajar la temperatura y al subir la salinidad. El agua densa se hunde, impulsando la circulación termohalina."},

{47,MISSION_8_CORRIENTES_OCEAN,
 "La corriente oceánica responsable del clima templado de Europa Occidental es:",
 {"La Corriente de Humboldt","La Corriente de California",
  "La Corriente Circumpolar","La Corriente del Golfo"},3,
 "La Corriente del Golfo transporta agua cálida tropical hacia el norte, dando a Europa Occidental un clima hasta 10°C más cálido de lo que le correspondería por latitud."},

{48,MISSION_8_CORRIENTES_OCEAN,
 "El cambio climático afecta las corrientes oceánicas principalmente porque:",
 {"Aumenta la velocidad del viento global",
  "El deshielo polar agrega agua dulce que reduce la salinidad y puede debilitar la circulación termohalina",
  "Eleva la temperatura del océano de forma uniforme",
  "Genera más tormentas que interrumpen temporalmente los patrones"},1,
 "El agua de deshielo glaciar diluye la salinidad del Atlántico Norte, donde la corriente termohalina se 'hunde'. Esto puede debilitar o colapsar la AMOC."},

{49,MISSION_8_CORRIENTES_OCEAN,
 "Las corrientes oceánicas son clave para entender la acumulación de plástico porque:",
 {"Disuelven gradualmente el plástico al transportarlo",
  "Distribuyen el plástico homogéneamente por el fondo",
  "Transportan y concentran el plástico en los centros de los giros oceánicos",
  "Llevan el plástico a las costas para recolectarlo"},2,
 "Los giros oceánicos actúan como trampas: su centro tiene calma y baja presión, acumulando todo lo que flota, incluidos los plásticos."},
};

/* ──────────────────────────────────────────────────
   INIT Y CARGA
────────────────────────────────────────────────── */
void TriviaManager_Init(void) {
    memset(&g_trivia, 0, sizeof(g_trivia));
    g_trivia.feedback_duration = 3.5f;
    g_trivia.points_per_correct = 100;
    g_trivia.streak_bonus       = 50;
    g_trivia.state              = TRIVIA_IDLE;
    g_trivia.selected_answer    = -1;
    g_trivia.current_mission    = MISSION_NONE;
    TriviaManager_LoadQuestions();
}

void TriviaManager_LoadQuestions(void) {
    int count = sizeof(s_question_bank) / sizeof(s_question_bank[0]);
    if (count > MAX_QUESTIONS) count = MAX_QUESTIONS;
    memcpy(g_trivia.questions, s_question_bank, count * sizeof(Question));
    g_trivia.total_questions = count;

    int mission_cursors[MISSION_COUNT] = {0};
    for (int i = 0; i < count; i++) {
        MissionID m = g_trivia.questions[i].mission;
        if (m >= 0 && m < MISSION_COUNT) {
            int pos = mission_cursors[m];
            if (pos < QUESTIONS_PER_MISSION) {
                g_trivia.mission_indices[m][pos] = i;
                mission_cursors[m]++;
            }
        }
    }
}

static void shuffle_ints(int *arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
    }
}

void TriviaManager_StartMission(MissionID m) {
    if (m < 0 || m >= MISSION_COUNT) return;
    g_trivia.current_mission = m;
    g_trivia.current_q_pos   = 0;
    g_trivia.selected_answer = -1;
    g_trivia.state           = TRIVIA_SHOWING_QUESTION;

    for (int i = 0; i < QUESTIONS_PER_MISSION; i++)
        g_trivia.current_q_order[i] = g_trivia.mission_indices[m][i];
    shuffle_ints(g_trivia.current_q_order, QUESTIONS_PER_MISSION);
}

const Question *TriviaManager_CurrentQuestion(void) {
    if (g_trivia.state == TRIVIA_IDLE || g_trivia.current_mission == MISSION_NONE)
        return NULL;
    if (g_trivia.current_q_pos >= QUESTIONS_PER_MISSION) return NULL;
    int idx = g_trivia.current_q_order[g_trivia.current_q_pos];
    return &g_trivia.questions[idx];
}

void TriviaManager_SubmitAnswer(int option_index) {
    if (g_trivia.state != TRIVIA_SHOWING_QUESTION) return;
    const Question *q = TriviaManager_CurrentQuestion();
    if (!q) return;

    g_trivia.selected_answer = option_index;
    g_trivia.answer_correct  = (option_index == q->correct_index);
    g_trivia.state           = TRIVIA_FEEDBACK;
    g_trivia.feedback_timer  = 0.0f;

    if (g_trivia.answer_correct) {
        g_trivia.streak++;
        g_trivia.score += g_trivia.points_per_correct;
        if (g_trivia.streak > 0 && g_trivia.streak % 3 == 0)
            g_trivia.score += g_trivia.streak_bonus;
    } else {
        g_trivia.streak = 0;
    }

    if (g_trivia.on_answer_result)
        g_trivia.on_answer_result(g_trivia.answer_correct, q->explanation);
}

void TriviaManager_Update(float dt) {
    if (g_trivia.state == TRIVIA_FEEDBACK) {
        g_trivia.feedback_timer += dt;
        if (g_trivia.feedback_timer >= g_trivia.feedback_duration) {
            g_trivia.current_q_pos++;
            g_trivia.selected_answer = -1;
            if (g_trivia.current_q_pos >= QUESTIONS_PER_MISSION) {
                g_trivia.state = TRIVIA_MISSION_COMPLETE;
                if (g_trivia.on_mission_complete)
                    g_trivia.on_mission_complete();
            } else {
                g_trivia.state = TRIVIA_SHOWING_QUESTION;
            }
        }
    }
}

/* ──────────────────────────────────────────────────
   DIBUJO DEL PANEL DE TRIVIA  (pantalla completa)
   Se invoca sólo DESPUÉS de que el minijuego termina.
────────────────────────────────────────────────── */
void TriviaManager_DrawPanel(Rectangle r) {
    const Question *q = TriviaManager_CurrentQuestion();

    DrawRectangleRec(r, COL_UI_PANEL);
    DrawRectangleLinesEx(r, 2, COL_UI_BORDER);

    float pad = 24.0f;
    float cx  = r.x + pad;
    float cy  = r.y + pad;
    float pw  = r.width - 2*pad;

    /* progreso */
    char prog[40];
    snprintf(prog, sizeof(prog), "Pregunta  %d / %d",
             g_trivia.current_q_pos + 1, QUESTIONS_PER_MISSION);
    DrawText(prog, (int)cx, (int)cy, 20, COL_UI_BORDER);
    cy += 30;

    /* puntuación */
    char sc[40];
    snprintf(sc, sizeof(sc), "Puntaje: %d", g_trivia.score);
    DrawText(sc, (int)(r.x + r.width - pad - 160), (int)(r.y + pad), 20, COL_UI_ACCENT);

    /* racha */
    if (g_trivia.streak >= 2) {
        char st[32];
        snprintf(st, sizeof(st), "Racha x%d!", g_trivia.streak);
        DrawText(st, (int)(r.x + r.width - pad - 130), (int)(r.y + pad + 28), 17, COL_PLASTIC_YEL);
    }

    cy += 14;

    if (!q) {
        if (g_trivia.state == TRIVIA_MISSION_COMPLETE) {
            DrawText("¡Misión completa!", (int)cx, (int)(cy + 80), 28, COL_CORRECT);
            char final_sc[64];
            snprintf(final_sc, sizeof(final_sc), "Puntaje final: %d", g_trivia.score);
            DrawText(final_sc, (int)cx, (int)(cy + 120), 22, WHITE);
        }
        return;
    }

    /* texto de la pregunta */
    UI_DrawWrappedText(q->text, (int)cx, (int)cy, (int)pw, 20, WHITE);
    cy += 130;

    /* botones de respuesta */
    const char *labels[4] = {"A","B","C","D"};
    for (int i = 0; i < 4; i++) {
        Rectangle btn = { cx, cy, pw, 64 };

        Color bg     = COL_UI_BG;
        Color border = COL_UI_BORDER;
        Color fg     = WHITE;

        if (g_trivia.state == TRIVIA_FEEDBACK) {
            if (i == q->correct_index) { bg = COL_CORRECT; border = COL_CORRECT; fg = BLACK; }
            else if (i == g_trivia.selected_answer) { bg = COL_WRONG; border = COL_WRONG; }
        }

        /* hover highlight */
        if (g_trivia.state == TRIVIA_SHOWING_QUESTION) {
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, btn)) {
                bg = ColorAlpha(COL_UI_ACCENT, 0.15f);
                border = COL_UI_ACCENT;
            }
        }

        DrawRectangleRec(btn, bg);
        DrawRectangleLinesEx(btn, 2.0f, border);

        char label[MAX_OPTION_LEN + 6];
        snprintf(label, sizeof(label), "%s)  %s", labels[i], q->options[i]);
        UI_DrawWrappedText(label, (int)(cx + 12), (int)(cy + 10), (int)(pw - 16), 18, fg);

        if (g_trivia.state == TRIVIA_SHOWING_QUESTION) {
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                TriviaManager_SubmitAnswer(i);
        }

        cy += 74;
    }

    /* feedback */
    if (g_trivia.state == TRIVIA_FEEDBACK) {
        cy += 14;
        Color fbcol = g_trivia.answer_correct ? COL_CORRECT : COL_WRONG;
        const char *verdict = g_trivia.answer_correct ? "¡Correcto!" : "Incorrecto";
        DrawText(verdict, (int)cx, (int)cy, 24, fbcol);
        cy += 34;
        UI_DrawWrappedText(q->explanation, (int)cx, (int)cy, (int)pw, 16, LIGHTGRAY);

        float t = g_trivia.feedback_timer / g_trivia.feedback_duration;
        Rectangle bar_bg = { cx, r.y + r.height - 14, pw, 8 };
        Rectangle bar_fg = { cx, r.y + r.height - 14, pw * (1.0f - t), 8 };
        DrawRectangleRec(bar_bg, COL_UI_BORDER);
        DrawRectangleRec(bar_fg, fbcol);
    }
}

/* ──────────────────────────────────────────────────
   PANTALLA DE INTRODUCCIÓN DE MISIÓN
   Devuelve true  → jugador presionó "Comenzar"
   Devuelve false → jugador presionó "Menú Principal"
────────────────────────────────────────────────── */
bool ShowMissionIntro(int mission_number,
                      const char *mission_title,
                      const char *topic_title,
                      const char *body_text)
{
    float anim = 0.0f;
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        anim += dt;

        BeginDrawing();
        ClearBackground(COL_UI_BG);

        /* fondo animado */
        for (int i = 0; i < 30; i++) {
            float x = (sinf(anim * 0.25f + i * 1.9f) * 0.5f + 0.5f) * SCREEN_W;
            float y = (cosf(anim * 0.18f + i * 2.3f) * 0.5f + 0.5f) * SCREEN_H;
            DrawCircle((int)x, (int)y, 4 + i%6, ColorAlpha(COL_OCEAN_LIGHT, 0.18f));
        }

        /* tarjeta central */
        int cw = 920, ch = 560;
        int cx = (SCREEN_W - cw) / 2;
        int cy = (SCREEN_H - ch) / 2;
        DrawRectangleRounded((Rectangle){(float)cx,(float)cy,(float)cw,(float)ch},
                              0.05f, 12, COL_UI_PANEL);
        DrawRectangleRoundedLines((Rectangle){(float)cx,(float)cy,(float)cw,(float)ch},
                                   0.05f, 12, COL_UI_BORDER);

        /* número de misión */
        char mn[32];
        snprintf(mn, sizeof(mn), "MISIÓN %d", mission_number);
        int mnw = MeasureText(mn, 20);
        DrawText(mn, cx + (cw - mnw)/2, cy + 28, 20, COL_UI_BORDER);

        /* título de la misión */
        int tw = MeasureText(mission_title, 36);
        DrawText(mission_title, cx + (cw - tw)/2, cy + 58, 36, COL_UI_ACCENT);

        /* línea separadora */
        DrawLineEx((Vector2){(float)(cx+40),(float)(cy+108)},
                   (Vector2){(float)(cx+cw-40),(float)(cy+108)}, 2, COL_UI_BORDER);

        /* título del tema */
        DrawText(topic_title, cx + 40, cy + 122, 22, COL_PLASTIC_YEL);

        /* cuerpo */
        UI_DrawWrappedText(body_text, cx + 40, cy + 158, cw - 80, 19, LIGHTGRAY);

        /* botones */
        Rectangle btn_start = {(float)(cx + cw/2 - 220), (float)(cy + ch - 90), 200, 56};
        Rectangle btn_menu  = {(float)(cx + cw/2 + 20),  (float)(cy + ch - 90), 200, 56};

        if (UI_Button(btn_start, "  Comenzar  ", COL_UI_ACCENT, BLACK)) {
            EndDrawing();
            return true;
        }
        if (UI_Button(btn_menu, "Menu Principal", COL_UI_PANEL, WHITE)) {
            EndDrawing();
            return false;
        }

        EndDrawing();
    }
    return false;
}

/* ──────────────────────────────────────────────────
   UTILIDADES DE UI
────────────────────────────────────────────────── */
bool UI_Button(Rectangle rect, const char *label, Color bg, Color fg) {
    Vector2 mouse = GetMousePosition();
    bool hovered  = CheckCollisionPointRec(mouse, rect);
    bool pressed  = hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    Color draw_bg = hovered ? Fade(bg, 0.80f) : bg;
    DrawRectangleRec(rect, draw_bg);
    DrawRectangleLinesEx(rect, 2, hovered ? COL_UI_ACCENT : COL_UI_BORDER);

    int tw = MeasureText(label, 20);
    DrawText(label,
             (int)(rect.x + rect.width/2 - tw/2),
             (int)(rect.y + rect.height/2 - 10),
             20, fg);
    return pressed;
}

void UI_DrawWrappedText(const char *text, int x, int y, int width, int font_size, Color color) {
    char buf[4096];
    strncpy(buf, text, sizeof(buf)-1);
    buf[sizeof(buf)-1] = '\0';

    char line[1024] = "";
    char word[256];
    int  cy = y;
    int  i  = 0, len = (int)strlen(buf);

    while (i <= len) {
        int wi = 0;
        while (i <= len && buf[i] != ' ' && buf[i] != '\0') word[wi++] = buf[i++];
        word[wi] = '\0';
        i++;

        char test[1024];
        snprintf(test, sizeof(test), "%s%s%s",
                 line, (line[0] ? " " : ""), word);

        if (MeasureText(test, font_size) > width && line[0]) {
            DrawText(line, x, cy, font_size, color);
            cy += font_size + 5;
            snprintf(line, sizeof(line), "%s", word);
        } else {
            snprintf(line, sizeof(line), "%s", test);
        }
    }
    if (line[0]) DrawText(line, x, cy, font_size, color);
}

void UI_DrawPanel(Rectangle r, Color bg, Color border, int radius) {
    DrawRectangleRounded(r, (float)radius / 100.0f, 8, bg);
    DrawRectangleRoundedLines(r, (float)radius / 100.0f, 8, border);
}