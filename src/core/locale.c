#include "locale.h"

Language g_lang = LANG_ES;

/* ──────────────────────────────────────────────────
   STRING TABLES
   Index must match StrKey enum order exactly.
────────────────────────────────────────────────── */

static const char *s_strings[2][S_COUNT] = {

/* ═══════════════════════════════════════════════
   [0] SPANISH
═══════════════════════════════════════════════ */
{
/* S_GAME_TITLE      */ "Wasteland Waters",
/* S_GAME_SUBTITLE   */ "Un viaje educativo por la crisis de las islas de plastico",
/* S_BTN_START_ALL   */ "INICIAR DESDE EL PRINCIPIO",
/* S_BTN_MENU_MAIN   */ "Menu Principal",
/* S_SCORE_ACCUM     */ "Puntaje acumulado:",
/* S_VERSION         */ "CoffeeShop Development  [v2.0.1]",
/* S_LANG_ES         */ "ES",
/* S_LANG_EN         */ "EN",

/* mission buttons */
/* S_M1_BTN */ "Mision 1 .. Tipos de Plastico",
/* S_M2_BTN */ "Mision 2 .. Polimeros",
/* S_M3_BTN */ "Mision 3 .. Contaminantes",
/* S_M4_BTN */ "Mision 4 .. Paises Contaminantes",
/* S_M5_BTN */ "Mision 5 .. Corrientes Superficiales",
/* S_M6_BTN */ "Mision 6 .. Reciclaje",
/* S_M7_BTN */ "Mision 7 .. Islas de Plastico",
/* S_M8_BTN */ "Mision 8 .. Corrientes Oceanicas",

/* generic HUD */
/* S_QUESTION_PROGRESS */ "Pregunta  %d / %d",
/* S_SCORE_LABEL       */ "Puntaje: %d",
/* S_STREAK_LABEL      */ "Racha x%d!",
/* S_CORRECT           */ "¡Correcto!",
/* S_INCORRECT         */ "Incorrecto",
/* S_MISSION_COMPLETE  */ "¡Misión completa!",
/* S_FINAL_SCORE       */ "Puntaje final: %d",
/* S_BTN_START         */ "  Comenzar  ",
/* S_LOADING_TRIVIA    */ "Cargando preguntas...",
/* S_MISSION_LABEL     */ "MISIÓN %d",

/* results */
/* S_GAME_COMPLETE  */ "JUEGO COMPLETO",
/* S_THANKS_MSG     */ "Gracias por aprender sobre la contaminacion plastica.",
/* S_REDUCE_MSG     */ "Cada accion cuenta. Reduce, Reutiliza, Recicla.",
/* S_PATCH_FACT     */ "El Gran Parche del Pacifico tiene mas de 1.6 millones de km2.",
/* S_KNOWLEDGE_MSG  */ "Tu conocimiento es el primer paso para cambiarlo.",
/* S_BTN_BACK_START */ "Volver al inicio",

/* mission 1 */
/* S_M1_TITLE       */ "MISION 1 — Clasifica los Plasticos",
/* S_M1_SUBTITLE    */ "Arrastra cada plastico a su contenedor correcto",
/* S_M1_INSTRUCTION */ "Arrastra cada plastico a su contenedor correcto",
/* S_M1_PLACED      */ "Colocados: %d / %d",
/* S_M1_CLASSIFY_DONE */ "Clasificacion completa!",
/* S_M1_LOADING     */ "Cargando preguntas...",
/* S_M1_TRIVIA_TITLE */ "MISION 1 — Trivia: Tipos de Plastico",
/* S_M1_INTRO_TITLE */ "Tipos de Plastico",
/* S_M1_INTRO_TOPIC */ "La Gran Mancha de Plastico del Pacifico",
/* S_M1_INTRO_BODY  */
    "El Gran Parche de Basura del Pacifico tiene mas de 1.6 millones de km2 de extension "
    "y contiene mas de 80.000 toneladas de plastico. Pero, ¿sabias que no todo el plastico "
    "es igual? Existen 7 categorias principales de plasticos, cada una con propiedades y "
    "riesgos diferentes. Los tipos 1 (PET) y 2 (HDPE) son los mas reciclables; el tipo 6 "
    "(poliestireno, unicel) casi nunca se recicla y termina fragmentandose en microplasticos "
    "en el oceano. Aprende a identificarlos: el primer paso para reducir la contaminacion "
    "es saber exactamente que estas descartando.",

/* mission 2 */
/* S_M2_TITLE        */ "MISION 2 — Cadena de Polimeros",
/* S_M2_INSTRUCTION  */ "Arrastra el monomero correcto a cada casillero de la cadena",
/* S_M2_BUILD_LABEL  */ "Construye la cadena del polimero:",
/* S_M2_CHALLENGE_PROG */ "Reto %d de %d",
/* S_M2_MONOMERS_LABEL */ "Monomeros disponibles:",
/* S_M2_CHAIN_OK     */ "Cadena correcta!",
/* S_M2_CHAIN_WRONG  */ "Incorrecto, intentalo de nuevo",
/* S_M2_TRIVIA_TITLE */ "MISION 2 — Trivia: Polimeros",
/* S_M2_INTRO_TITLE  */ "Cadena de Polimeros",
/* S_M2_INTRO_TOPIC  */ "El plastico: una cadena interminable en el oceano",
/* S_M2_INTRO_BODY   */
    "Los plasticos son polimeros: largas cadenas de moleculas repetidas llamadas monomeros. "
    "Esta estructura molecular es precisamente lo que los hace tan duraderos... y tan "
    "peligrosos. Una botella de PET en el Gran Parche del Pacifico puede tardar hasta "
    "450 anos en degradarse, pero nunca desaparece del todo: se fragmenta en microplasticos "
    "que persisten indefinidamente. El Gran Parche contiene aproximadamente 1.8 billones "
    "de fragmentos plasticos. Entender la quimica del plastico nos ayuda a tomar mejores "
    "decisiones sobre cuales plasticos usar y cuales evitar.",

/* mission 3 */
/* S_M3_TITLE        */ "MISION 3 — Bloqueo de Microplasticos",
/* S_M3_INSTRUCTION  */ "Haz clic para colocar filtros y detener los microplasticos",
/* S_M3_FOOD_CHAIN_HP */ "Vida cadena alimentaria:",
/* S_M3_WAVE         */ "Ola: %d / %d",
/* S_M3_CAPTURED     */ "Atrapados: %d",
/* S_M3_WAVE_DONE    */ "Ola superada! Preparate...",
/* S_M3_RIVER_OK     */ "Rio protegido!",
/* S_M3_RIVER_FAIL   */ "Cadena alimentaria contaminada!",
/* S_M3_FILTER_LABEL */ "FILTRO",
/* S_M3_LOADING_TRIVIA */ "Cargando trivia...",
/* S_M3_TRIVIA_TITLE */ "MISION 3 — Trivia: Contaminantes Plasticos",
/* S_M3_INTRO_TITLE  */ "Contaminantes Plasticos",
/* S_M3_INTRO_TOPIC  */ "Microplasticos: el enemigo invisible del oceano",
/* S_M3_INTRO_BODY   */
    "El Gran Parche del Pacifico no es una isla solida de plastico. En realidad, es una "
    "densa 'sopa' de microplasticos: fragmentos menores a 5 mm que flotan suspendidos "
    "en el agua. La luz UV del sol fragmenta los plasticos grandes en millones de particulas "
    "invisibles al ojo humano. El zooplancton las confunde con alimento, y asi los "
    "microplasticos entran a la cadena alimentaria marina. Se han encontrado en tortugas, "
    "aves marinas, ballenas y peces que consumimos. Algunos estudios incluso los han "
    "detectado en la sangre humana. Detenerlos en la fuente es la unica solucion efectiva.",

/* mission 4 */
/* S_M4_TITLE         */ "MISION 4 — Mapa Interactivo de Paises",
/* S_M4_ACT1_TITLE    */ "Actividad 1: Que region contamina mas? Haz clic en la region",
/* S_M4_ACT2_TITLE    */ "Actividad 2: Ordena los paises por cantidad de plastico (arrastra)",
/* S_M4_ACT3_TITLE    */ "Actividad 3: Traza la ruta del rio mas contaminante (Yangtse)",
/* S_M4_DONE_TITLE    */ "Mision 4 completa!",
/* S_M4_RANK_LABEL    */ "Arrastra para ordenar:",
/* S_M4_MAP_LABEL     */ "MAPA MUNDIAL",
/* S_M4_ACT3_HINT     */ "Haz clic en los puntos naranjas en orden",
/* S_M4_CORRECT_ASIA  */ "Correcto! Asia aporta >60% de la contaminacion plastica oceanica.",
/* S_M4_WRONG_REGION  */ "Esa region contribuye, pero Asia es la mayor fuente. Intenta de nuevo.",
/* S_M4_RANK_OK       */ "Ranking correcto! China lidera, seguida de Indonesia y Filipinas.",
/* S_M4_RIVER_OK      */ "Correcto! El Yangtse descarga ~330.000 t de plastico al mar anualmente.",
/* S_M4_TRIVIA_TITLE  */ "MISION 4 — Trivia: Paises Contaminantes",
/* S_M4_INTRO_TITLE   */ "Paises Contaminantes",
/* S_M4_INTRO_TOPIC   */ "¿Quien envia el plastico al Gran Parche?",
/* S_M4_INTRO_BODY    */
    "El Gran Parche del Pacifico Norte se forma principalmente con desechos provenientes "
    "de las costas de Asia Oriental. China, Indonesia, Filipinas, Vietnam y Sri Lanka "
    "concentran el 60% de la contaminacion plastica oceanica mundial. El rio Yangtse, "
    "en China, es el mayor transportador de plastico al mar, arrojando hasta 330.000 "
    "toneladas anuales. Estas cantidades masivas llegan al oceano porque en muchas "
    "regiones la gestion de residuos es insuficiente. Sin embargo, paises desarrollados "
    "como EE.UU. tambien contribuyen exportando millones de toneladas de residuos "
    "plasticos a naciones con menor capacidad de gestion.",

/* mission 5 */
/* S_M5_TITLE          */ "MISION 5 — Corrientes Superficiales",
/* S_M5_INSTRUCTION    */ "Haz clic para colocar giros oceanicos y atrapar el plastico",
/* S_M5_GYRES_HUD      */ "Giros: %d/%d   Plastico atrapado: %d/%d",
/* S_M5_GOAL           */ "Meta: capturar el 60% del plastico",
/* S_M5_CORIOLIS_LINE1 */ "Efecto Coriolis: los giros giran en sentido horario",
/* S_M5_CORIOLIS_LINE2 */ "en el hemisferio norte y antihorario en el sur.",
/* S_M5_NORTH_LABEL    */ "H. Norte (giro horario)",
/* S_M5_SOUTH_LABEL    */ "H. Sur (giro antihorario)",
/* S_M5_EQUATOR        */ "Ecuador",
/* S_M5_GYRE_N         */ "Giro N",
/* S_M5_GYRE_S         */ "Giro S",
/* S_M5_TRIVIA_TITLE   */ "MISION 5 — Trivia: Corrientes Superficiales",
/* S_M5_INTRO_TITLE    */ "Corrientes Superficiales",
/* S_M5_INTRO_TOPIC    */ "Como llega el plastico al Gran Parche?",
/* S_M5_INTRO_BODY     */
    "El Gran Parche del Pacifico Norte existe gracias al Giro del Pacifico Norte, un "
    "sistema circular de corrientes oceanicas creado por el Efecto Coriolis y los vientos "
    "dominantes. Este giro atrapa todo lo que flota en su interior, concentrando el "
    "plastico en una zona de baja presion en su centro. El plastico puede tardar anos "
    "en llegar desde la costa hasta el parche, viajando miles de kilometros impulsado "
    "por las corrientes. Existen 5 grandes parches de basura en el mundo, uno por cada "
    "giro oceanico principal. Comprender estas corrientes es clave para predecir donde "
    "se acumula el plastico y como interceptarlo.",

/* mission 6 */
/* S_M6_TITLE        */ "MISION 6 — Banda Transportadora de Reciclaje",
/* S_M6_INSTRUCTION  */ "Como se recicla este plastico? Decide antes de que caiga.",
/* S_M6_HUD          */ "Clasificados: %d/%d   Correctos: %d   Puntaje: %d",
/* S_M6_REMAINING    */ "Piezas restantes: %d",
/* S_M6_BTN_MECH     */ "MECANICO",
/* S_M6_BTN_CHEM     */ "QUIMICO",
/* S_M6_BTN_WASTE    */ "DESECHO",
/* S_M6_GOAL_LABEL   */ "Meta: 15 piezas",
/* S_M6_TRIVIA_TITLE */ "MISION 6 — Trivia: Reciclaje y Desechos",
/* S_M6_INTRO_TITLE  */ "Reciclaje y Desechos",
/* S_M6_INTRO_TOPIC  */ "El reciclaje no es la solucion magica al Gran Parche",
/* S_M6_INTRO_BODY   */
    "Solo el 9% del plastico producido en la historia ha sido reciclado. El 79% restante "
    "esta en vertederos o en el medio ambiente, incluyendo el Gran Parche del Pacifico. "
    "El reciclaje tiene limitaciones reales: el PVC contamina todo un lote de reciclaje, "
    "el poliestireno raramente se recicla, y muchos plasticos negros son invisibles para "
    "los sensores de clasificacion. La verdadera solucion empieza por reducir el consumo "
    "desde la fuente. En este minijuego clasificaras 15 piezas de plastico segun el "
    "proceso de reciclaje correcto: mecanico, quimico o desecho.",

/* mission 7 */
/* S_M7_TITLE        */ "MISION 7 — Gran Parche de Basura",
/* S_M7_INSTRUCTION  */ "WASD / Flechas para navegar. Recoge el plastico.",
/* S_M7_COLLECTED    */ "Recogido: %d / %d piezas",
/* S_M7_PATCH_LABEL  */ "GRAN PARCHE",
/* S_M7_CLEANED      */ "Parche limpiado!",
/* S_M7_TRIVIA_TITLE */ "MISION 7 — Trivia: Islas de Plastico",
/* S_M7_INTRO_TITLE  */ "Islas de Plastico",
/* S_M7_INTRO_TOPIC  */ "El Gran Parche del Pacifico: lo que los ojos no ven",
/* S_M7_INTRO_BODY   */
    "El Gran Parche de Basura del Pacifico (Great Pacific Garbage Patch) fue descubierto "
    "en 1997 por el navegante Charles Moore. Con mas de 1.6 millones de km2, es tres "
    "veces el tamano de Francia. Sin embargo, no es una isla solida: es una 'sopa' "
    "de microplasticos suspendidos en el agua que la hace turbia y lechosa. "
    "Se estima que contiene mas de 80.000 toneladas de plastico y mas de 1.8 billones "
    "de fragmentos. Es imposible de limpiar completamente con la tecnologia actual, "
    "pero organizaciones como The Ocean Cleanup trabajan para reducirlo. "
    "En este minijuego navegaras por el parche recogiendo residuos.",

/* mission 8 */
/* S_M8_TITLE           */ "MISION 8 — Circulacion Termohalina",
/* S_M8_INSTRUCTION     */ "Ajusta temperatura y salinidad de las zonas oceanicas",
/* S_M8_SELECT_ZONE     */ "Selecciona una zona",
/* S_M8_TEMP_LABEL      */ "Temperatura",
/* S_M8_SALT_LABEL      */ "Salinidad",
/* S_M8_ICE_BTN         */ "Simular deshielo polar",
/* S_M8_ICE_NOTE        */ "(reduce salinidad artica)",
/* S_M8_AMOC_LABEL      */ "Fuerza AMOC:",
/* S_M8_AMOC_COLLAPSED  */ "AMOC COLAPSADA!",
/* S_M8_ICE_MSG         */ "El deshielo reduce la salinidad polar! Esto puede colapsar la AMOC.",
/* S_M8_STATUS_DEFAULT  */ "Selecciona una zona oceanica y ajusta sus parametros.",
/* S_M8_INTERACT_LABEL  */ "Interactua para continuar:",
/* S_M8_TRIVIA_TITLE    */ "MISION 8 — Trivia: Corrientes Oceanicas",
/* S_M8_INTRO_TITLE     */ "Corrientes Oceanicas",
/* S_M8_INTRO_TOPIC     */ "La cinta transportadora que distribuye el plastico",
/* S_M8_INTRO_BODY      */
    "La Circulacion Termohalina, llamada 'cinta transportadora oceanica', mueve agua "
    "por todos los oceanos en un ciclo que puede durar hasta 1.000 anos. Funciona "
    "gracias a las diferencias de temperatura y salinidad: el agua fria y salada del "
    "Artico se hunde y arrastra toda la masa oceanica. Esta circulacion distribuye "
    "calor, oxigeno y nutrientes por el planeta, regulando el clima global. El cambio "
    "climatico amenaza este sistema: el deshielo polar agrega agua dulce que reduce "
    "la salinidad y puede debilitar o colapsar la AMOC (Atlantic Meridional Overturning "
    "Circulation), con consecuencias catastroficas para el clima europeo y la "
    "distribucion del plastico en los oceanos. Explora y experimenta con los parametros.",

/* M6 item names & reasons */
/* S_M6_ITEM0_NAME   */ "Botella de agua PET",
/* S_M6_ITEM0_REASON */ "El PET limpio se recicla mecanicamente para fabricar nuevas botellas o fibra textil.",
/* S_M6_ITEM1_NAME   */ "Tuberia de PVC",
/* S_M6_ITEM1_REASON */ "El PVC libera dioxinas al reciclarse mecanicamente; generalmente va a desecho.",
/* S_M6_ITEM2_NAME   */ "Bolsa LDPE",
/* S_M6_ITEM2_REASON */ "El LDPE puede reciclarse mecanicamente en bolsas y tuberias de baja presion.",
/* S_M6_ITEM3_NAME   */ "Envase de aceite HDPE",
/* S_M6_ITEM3_REASON */ "El HDPE limpio se recicla eficientemente para tuberias y muebles de exterior.",
/* S_M6_ITEM4_NAME   */ "Vaso de unicel EPS",
/* S_M6_ITEM4_REASON */ "El poliestireno expandido es dificil de reciclar y casi siempre termina en relleno.",
/* S_M6_ITEM5_NAME   */ "Plastico mixto #7",
/* S_M6_ITEM5_REASON */ "Los plasticos #7 y mixtos requieren reciclaje quimico para despolimerizarse.",
/* S_M6_ITEM6_NAME   */ "Botella PET negra",
/* S_M6_ITEM6_REASON */ "Los sensores opticos no detectan el negro; la mayoria de plantas lo envia a desecho.",
/* S_M6_ITEM7_NAME   */ "Polipropileno limpio",
/* S_M6_ITEM7_REASON */ "El PP limpio (envases, tapas) se recicla mecanicamente con alta eficiencia.",
/* S_M6_INCORRECT_FMT */ "Incorrecto. Era %s. %s",
/* S_M6_PROC_MECH    */ "Mecanico",
/* S_M6_PROC_CHEM    */ "Quimico",
/* S_M6_PROC_WASTE   */ "Desecho",

/* M2 challenges */
/* S_M2_CH0_POLY */ "Polietileno (PE)",
/* S_M2_CH0_MONO */ "Etileno",
/* S_M2_CH0_HINT */ "Monomero: Etileno (C2H4). El polietileno es el plastico mas producido del mundo.",
/* S_M2_CH1_POLY */ "Polipropileno (PP)",
/* S_M2_CH1_MONO */ "Propileno",
/* S_M2_CH1_HINT */ "Monomero: Propileno (C3H6). Usado en tapas, envases y fibras textiles.",
/* S_M2_CH2_POLY */ "Poliestireno (PS)",
/* S_M2_CH2_MONO */ "Estireno",
/* S_M2_CH2_HINT */ "Monomero: Estireno. El unicel (EPS) es poliestireno expandido.",
/* S_M2_CH3_POLY */ "PVC",
/* S_M2_CH3_MONO */ "Cloruro de vinilo",
/* S_M2_CH3_HINT */ "Monomero: Cloruro de vinilo. El Cl en su cadena lo hace dificil de reciclar.",
/* S_M2_CH4_POLY */ "Nylon (PA6)",
/* S_M2_CH4_MONO */ "Caprolactama",
/* S_M2_CH4_HINT */ "Monomero: Caprolactama. Es un polimero sintetico de alto rendimiento.",
/* S_M2_CH5_POLY */ "Celulosa (natural)",
/* S_M2_CH5_MONO */ "Glucosa",
/* S_M2_CH5_HINT */ "Monomero: Glucosa. La celulosa es el polimero natural mas abundante en la Tierra.",

/* M2 monomer palette */
/* S_MONO_0 */ "Etileno",
/* S_MONO_1 */ "Propileno",
/* S_MONO_2 */ "Estireno",
/* S_MONO_3 */ "Cloruro de vinilo",
/* S_MONO_4 */ "Caprolactama",
/* S_MONO_5 */ "Glucosa",
/* S_MONO_6 */ "Benceno",
/* S_MONO_7 */ "Acetileno",

/* M1 item names */
/* S_M1_ITEM0 */ "Botella de agua",
/* S_M1_ITEM1 */ "Envase detergente",
/* S_M1_ITEM2 */ "Tuberia",
/* S_M1_ITEM3 */ "Bolsa supermercado",
/* S_M1_ITEM4 */ "Tapa de yogurt",
/* S_M1_ITEM5 */ "Vaso de unicel",

/* M4 country names */
/* S_M4_C0 */ "China",
/* S_M4_C1 */ "Indonesia",
/* S_M4_C2 */ "Filipinas",
/* S_M4_C3 */ "Vietnam",
/* S_M4_C4 */ "Sri Lanka",
/* S_M4_C5 */ "Tailandia",
/* S_M4_C6 */ "Egipto",
/* S_M4_C7 */ "Nigeria",
/* S_M4_C8 */ "Brasil",
/* S_M4_C9 */ "India",
/* S_M4_REGION_ASIA   */ "Asia",
/* S_M4_REGION_AFRICA */ "Africa",
/* S_M4_REGION_LAM    */ "LAm",

/* M4 river */
/* S_M4_RIVER_NAME     */ "Rio Yangtse (China)",
/* S_M4_RIVER_OK (dup) already defined above; this slot is river label */
/* S_M4_RIVER_HINT     */ "El Yangtse descarga ~330.000 t de plastico al mar anualmente.",
/* S_M4_ACT3_CLICK_HINT */ "Haz clic en los puntos naranjas en orden",

/* M8 zone names */
/* S_M8_ZONE0 */ "Artico",
/* S_M8_ZONE1 */ "Atlantico Norte",
/* S_M8_ZONE2 */ "Tropical",
/* S_M8_ZONE3 */ "Atlantico Sur",
/* S_M8_ZONE4 */ "Antartico",

/* M8 short labels */
/* S_M8_TEMP_SHORT */ "T:%.0f%%",
/* S_M8_SALT_SHORT */ "S:%.0f%%",
/* S_M8_DENS_SHORT */ "D:%.2f",

/* S_BTN_NEXT */ "Siguiente",

/* S_REPAIR_S4*/ "Clic derecho para reparar"
},

/* ═══════════════════════════════════════════════
   [1] ENGLISH
═══════════════════════════════════════════════ */
{
/* S_GAME_TITLE      */ "Wasteland Waters",
/* S_GAME_SUBTITLE   */ "An educational journey through the plastic island crisis",
/* S_BTN_START_ALL   */ "START FROM THE BEGINNING",
/* S_BTN_MENU_MAIN   */ "Main Menu",
/* S_SCORE_ACCUM     */ "Accumulated score:",
/* S_VERSION         */ "CoffeeShop Development  [v2.0.1]",
/* S_LANG_ES         */ "ES",
/* S_LANG_EN         */ "EN",

/* mission buttons */
/* S_M1_BTN */ "Mission 1 .. Types of Plastic",
/* S_M2_BTN */ "Mission 2 .. Polymers",
/* S_M3_BTN */ "Mission 3 .. Contaminants",
/* S_M4_BTN */ "Mission 4 .. Polluting Countries",
/* S_M5_BTN */ "Mission 5 .. Surface Currents",
/* S_M6_BTN */ "Mission 6 .. Recycling",
/* S_M7_BTN */ "Mission 7 .. Plastic Islands",
/* S_M8_BTN */ "Mission 8 .. Ocean Currents",

/* generic HUD */
/* S_QUESTION_PROGRESS */ "Question  %d / %d",
/* S_SCORE_LABEL       */ "Score: %d",
/* S_STREAK_LABEL      */ "Streak x%d!",
/* S_CORRECT           */ "Correct!",
/* S_INCORRECT         */ "Incorrect",
/* S_MISSION_COMPLETE  */ "Mission complete!",
/* S_FINAL_SCORE       */ "Final score: %d",
/* S_BTN_START         */ "  Start  ",
/* S_LOADING_TRIVIA    */ "Loading questions...",
/* S_MISSION_LABEL     */ "MISSION %d",

/* results */
/* S_GAME_COMPLETE  */ "GAME COMPLETE",
/* S_THANKS_MSG     */ "Thank you for learning about plastic pollution.",
/* S_REDUCE_MSG     */ "Every action counts. Reduce, Reuse, Recycle.",
/* S_PATCH_FACT     */ "The Great Pacific Patch covers more than 1.6 million km2.",
/* S_KNOWLEDGE_MSG  */ "Your knowledge is the first step toward changing it.",
/* S_BTN_BACK_START */ "Back to start",

/* mission 1 */
/* S_M1_TITLE        */ "MISSION 1 — Classify the Plastics",
/* S_M1_SUBTITLE     */ "Drag each plastic to its correct bin",
/* S_M1_INSTRUCTION  */ "Drag each plastic to its correct bin",
/* S_M1_PLACED       */ "Placed: %d / %d",
/* S_M1_CLASSIFY_DONE */ "Classification complete!",
/* S_M1_LOADING       */ "Loading questions...",
/* S_M1_TRIVIA_TITLE  */ "MISSION 1 — Trivia: Types of Plastic",
/* S_M1_INTRO_TITLE   */ "Types of Plastic",
/* S_M1_INTRO_TOPIC   */ "The Great Pacific Garbage Patch",
/* S_M1_INTRO_BODY    */
    "The Great Pacific Garbage Patch covers more than 1.6 million km2 "
    "and contains over 80,000 tonnes of plastic. But did you know that not all plastic "
    "is the same? There are 7 main plastic categories, each with different properties and "
    "risks. Types 1 (PET) and 2 (HDPE) are the most recyclable; type 6 "
    "(polystyrene, foam) is rarely recycled and ends up fragmenting into microplastics "
    "in the ocean. Learn to identify them: the first step to reducing pollution "
    "is knowing exactly what you are throwing away.",

/* mission 2 */
/* S_M2_TITLE        */ "MISSION 2 — Polymer Chain",
/* S_M2_INSTRUCTION  */ "Drag the correct monomer into each slot of the chain",
/* S_M2_BUILD_LABEL  */ "Build the polymer chain:",
/* S_M2_CHALLENGE_PROG */ "Challenge %d of %d",
/* S_M2_MONOMERS_LABEL */ "Available monomers:",
/* S_M2_CHAIN_OK     */ "Correct chain!",
/* S_M2_CHAIN_WRONG  */ "Incorrect, try again",
/* S_M2_TRIVIA_TITLE */ "MISSION 2 — Trivia: Polymers",
/* S_M2_INTRO_TITLE  */ "Polymer Chain",
/* S_M2_INTRO_TOPIC  */ "Plastic: an endless chain in the ocean",
/* S_M2_INTRO_BODY   */
    "Plastics are polymers: long chains of repeated molecules called monomers. "
    "This molecular structure is precisely what makes them so durable—and so "
    "dangerous. A PET bottle in the Great Pacific Patch can take up to "
    "450 years to degrade, but never fully disappears: it breaks into microplastics "
    "that persist indefinitely. The Patch contains roughly 1.8 trillion "
    "plastic fragments. Understanding plastic chemistry helps us make better "
    "decisions about which plastics to use and which to avoid.",

/* mission 3 */
/* S_M3_TITLE        */ "MISSION 3 — Microplastic Blockade",
/* S_M3_INSTRUCTION  */ "Click to place filters and stop the microplastics",
/* S_M3_FOOD_CHAIN_HP */ "Food chain health:",
/* S_M3_WAVE         */ "Wave: %d / %d",
/* S_M3_CAPTURED     */ "Trapped: %d",
/* S_M3_WAVE_DONE    */ "Wave cleared! Get ready...",
/* S_M3_RIVER_OK     */ "River protected!",
/* S_M3_RIVER_FAIL   */ "Food chain contaminated!",
/* S_M3_FILTER_LABEL */ "FILTER",
/* S_M3_LOADING_TRIVIA */ "Loading trivia...",
/* S_M3_TRIVIA_TITLE */ "MISSION 3 — Trivia: Plastic Contaminants",
/* S_M3_INTRO_TITLE  */ "Plastic Contaminants",
/* S_M3_INTRO_TOPIC  */ "Microplastics: the ocean's invisible enemy",
/* S_M3_INTRO_BODY   */
    "The Great Pacific Patch is not a solid plastic island. In reality, it is a "
    "dense 'soup' of microplastics—fragments smaller than 5 mm suspended "
    "in the water. Solar UV light breaks large plastics into millions of particles "
    "invisible to the naked eye. Zooplankton mistakes them for food, allowing "
    "microplastics to enter the marine food chain. They have been found in turtles, "
    "seabirds, whales, and fish we eat. Some studies have even detected them "
    "in human blood. Stopping them at the source is the only effective solution.",

/* mission 4 */
/* S_M4_TITLE         */ "MISSION 4 — Interactive Country Map",
/* S_M4_ACT1_TITLE    */ "Activity 1: Which region pollutes the most? Click on the region",
/* S_M4_ACT2_TITLE    */ "Activity 2: Rank countries by plastic output (drag to reorder)",
/* S_M4_ACT3_TITLE    */ "Activity 3: Trace the route of the most polluting river (Yangtze)",
/* S_M4_DONE_TITLE    */ "Mission 4 complete!",
/* S_M4_RANK_LABEL    */ "Drag to rank:",
/* S_M4_MAP_LABEL     */ "WORLD MAP",
/* S_M4_ACT3_HINT     */ "Click the orange dots in order",
/* S_M4_CORRECT_ASIA  */ "Correct! Asia contributes >60% of the world's oceanic plastic pollution.",
/* S_M4_WRONG_REGION  */ "That region contributes, but Asia is the largest source. Try again.",
/* S_M4_RANK_OK       */ "Ranking correct! China leads, followed by Indonesia and the Philippines.",
/* S_M4_RIVER_OK      */ "Correct! The Yangtze discharges ~330,000 t of plastic into the sea yearly.",
/* S_M4_TRIVIA_TITLE  */ "MISSION 4 — Trivia: Polluting Countries",
/* S_M4_INTRO_TITLE   */ "Polluting Countries",
/* S_M4_INTRO_TOPIC   */ "Who is sending plastic to the Great Patch?",
/* S_M4_INTRO_BODY    */
    "The North Pacific Garbage Patch forms mainly from waste originating "
    "along the coasts of East Asia. China, Indonesia, the Philippines, Vietnam, and "
    "Sri Lanka account for 60% of the world's oceanic plastic pollution. The Yangtze "
    "River in China is the largest transporter of plastic to the sea, releasing up to "
    "330,000 tonnes annually. These massive amounts reach the ocean because waste "
    "management in many regions is insufficient. However, developed nations "
    "like the USA also contribute by exporting millions of tonnes of plastic waste "
    "to countries with less management capacity.",

/* mission 5 */
/* S_M5_TITLE          */ "MISSION 5 — Surface Currents",
/* S_M5_INSTRUCTION    */ "Click to place ocean gyres and trap the plastic",
/* S_M5_GYRES_HUD      */ "Gyres: %d/%d   Plastic trapped: %d/%d",
/* S_M5_GOAL           */ "Goal: capture 60% of the plastic",
/* S_M5_CORIOLIS_LINE1 */ "Coriolis effect: gyres rotate clockwise",
/* S_M5_CORIOLIS_LINE2 */ "in the northern hemisphere, counterclockwise in the south.",
/* S_M5_NORTH_LABEL    */ "N. Hemisphere (clockwise)",
/* S_M5_SOUTH_LABEL    */ "S. Hemisphere (counterclockwise)",
/* S_M5_EQUATOR        */ "Equator",
/* S_M5_GYRE_N         */ "Gyre N",
/* S_M5_GYRE_S         */ "Gyre S",
/* S_M5_TRIVIA_TITLE   */ "MISSION 5 — Trivia: Surface Currents",
/* S_M5_INTRO_TITLE    */ "Surface Currents",
/* S_M5_INTRO_TOPIC    */ "How does plastic reach the Great Patch?",
/* S_M5_INTRO_BODY     */
    "The North Pacific Garbage Patch exists because of the North Pacific Gyre, a "
    "circular system of ocean currents created by the Coriolis Effect and dominant winds. "
    "This gyre traps everything floating inside it, concentrating plastic in a "
    "low-pressure zone at its center. Plastic can take years to travel from shore "
    "to the patch, carried thousands of kilometers by currents. There are 5 major "
    "garbage patches in the world, one for each principal ocean gyre. Understanding "
    "these currents is key to predicting where plastic accumulates and how to intercept it.",

/* mission 6 */
/* S_M6_TITLE        */ "MISSION 6 — Recycling Conveyor Belt",
/* S_M6_INSTRUCTION  */ "How is this plastic recycled? Decide before it falls off.",
/* S_M6_HUD          */ "Classified: %d/%d   Correct: %d   Score: %d",
/* S_M6_REMAINING    */ "Remaining pieces: %d",
/* S_M6_BTN_MECH     */ "MECHANICAL",
/* S_M6_BTN_CHEM     */ "CHEMICAL",
/* S_M6_BTN_WASTE    */ "WASTE",
/* S_M6_GOAL_LABEL   */ "Goal: 15 pieces",
/* S_M6_TRIVIA_TITLE */ "MISSION 6 — Trivia: Recycling & Waste",
/* S_M6_INTRO_TITLE  */ "Recycling & Waste",
/* S_M6_INTRO_TOPIC  */ "Recycling is not the magic solution to the Great Patch",
/* S_M6_INTRO_BODY   */
    "Only 9% of all plastic ever produced has been recycled. The remaining 79% "
    "sits in landfills or in the environment, including the Great Pacific Patch. "
    "Recycling has real limitations: PVC contaminates an entire recycling batch, "
    "polystyrene is rarely recycled, and many black plastics are invisible to optical "
    "sorting sensors. The real solution starts with reducing consumption at the source. "
    "In this minigame you will classify 15 plastic items by the correct recycling "
    "process: mechanical, chemical, or waste.",

/* mission 7 */
/* S_M7_TITLE        */ "MISSION 7 — Great Garbage Patch",
/* S_M7_INSTRUCTION  */ "WASD / Arrow keys to navigate. Collect the plastic.",
/* S_M7_COLLECTED    */ "Collected: %d / %d pieces",
/* S_M7_PATCH_LABEL  */ "GREAT PATCH",
/* S_M7_CLEANED      */ "Patch cleaned!",
/* S_M7_TRIVIA_TITLE */ "MISSION 7 — Trivia: Plastic Islands",
/* S_M7_INTRO_TITLE  */ "Plastic Islands",
/* S_M7_INTRO_TOPIC  */ "The Great Pacific Patch: what the eyes cannot see",
/* S_M7_INTRO_BODY   */
    "The Great Pacific Garbage Patch was discovered in 1997 by sailor and oceanographer "
    "Charles Moore. Spanning more than 1.6 million km2, it is three times the size of "
    "France. Yet it is not a solid island: it is a 'soup' of microplastics suspended "
    "in the water, making it cloudy and milky. It is estimated to contain over 80,000 "
    "tonnes of plastic and more than 1.8 trillion fragments. It cannot be fully cleaned "
    "with current technology, but organizations like The Ocean Cleanup are working to "
    "reduce it. In this minigame you will navigate the patch collecting debris.",

/* mission 8 */
/* S_M8_TITLE           */ "MISSION 8 — Thermohaline Circulation",
/* S_M8_INSTRUCTION     */ "Adjust the temperature and salinity of each ocean zone",
/* S_M8_SELECT_ZONE     */ "Select a zone",
/* S_M8_TEMP_LABEL      */ "Temperature",
/* S_M8_SALT_LABEL      */ "Salinity",
/* S_M8_ICE_BTN         */ "Simulate polar melt",
/* S_M8_ICE_NOTE        */ "(reduces arctic salinity)",
/* S_M8_AMOC_LABEL      */ "AMOC strength:",
/* S_M8_AMOC_COLLAPSED  */ "AMOC COLLAPSED!",
/* S_M8_ICE_MSG         */ "Melting reduces polar salinity! This can collapse the AMOC.",
/* S_M8_STATUS_DEFAULT  */ "Select an ocean zone and adjust its parameters.",
/* S_M8_INTERACT_LABEL  */ "Interact to continue:",
/* S_M8_TRIVIA_TITLE    */ "MISSION 8 — Trivia: Ocean Currents",
/* S_M8_INTRO_TITLE     */ "Ocean Currents",
/* S_M8_INTRO_TOPIC     */ "The conveyor belt that distributes plastic across the globe",
/* S_M8_INTRO_BODY      */
    "The Thermohaline Circulation, known as the 'ocean conveyor belt', moves water "
    "through all the oceans in a cycle that can last up to 1,000 years. It is driven "
    "by differences in temperature and salinity: cold, salty Arctic water sinks and "
    "drags the entire ocean mass with it. This circulation distributes heat, oxygen, "
    "and nutrients across the planet, regulating the global climate. Climate change "
    "threatens this system: polar ice melt adds fresh water that reduces salinity and "
    "could weaken or collapse the AMOC (Atlantic Meridional Overturning Circulation), "
    "with catastrophic consequences for European climate and the distribution of "
    "plastic across the oceans. Explore and experiment with the parameters.",

/* M6 item names & reasons */
/* S_M6_ITEM0_NAME   */ "PET water bottle",
/* S_M6_ITEM0_REASON */ "Clean PET is mechanically recycled to make new bottles or textile fiber.",
/* S_M6_ITEM1_NAME   */ "PVC pipe",
/* S_M6_ITEM1_REASON */ "PVC releases dioxins during mechanical recycling and usually goes to waste.",
/* S_M6_ITEM2_NAME   */ "LDPE bag",
/* S_M6_ITEM2_REASON */ "LDPE can be mechanically recycled into bags and low-pressure pipes.",
/* S_M6_ITEM3_NAME   */ "HDPE oil container",
/* S_M6_ITEM3_REASON */ "Clean HDPE is efficiently recycled into pipes and outdoor furniture.",
/* S_M6_ITEM4_NAME   */ "Foam EPS cup",
/* S_M6_ITEM4_REASON */ "Expanded polystyrene is hard to recycle and almost always ends up in landfill.",
/* S_M6_ITEM5_NAME   */ "Mixed plastic #7",
/* S_M6_ITEM5_REASON */ "#7 and mixed plastics require chemical recycling for depolymerization.",
/* S_M6_ITEM6_NAME   */ "Black PET bottle",
/* S_M6_ITEM6_REASON */ "Optical sensors cannot detect black; most plants route it to waste.",
/* S_M6_ITEM7_NAME   */ "Clean polypropylene",
/* S_M6_ITEM7_REASON */ "Clean PP (containers, caps) is mechanically recycled with high efficiency.",
/* S_M6_INCORRECT_FMT */ "Incorrect. Should be %s. %s",
/* S_M6_PROC_MECH    */ "Mechanical",
/* S_M6_PROC_CHEM    */ "Chemical",
/* S_M6_PROC_WASTE   */ "Waste",

/* M2 challenges */
/* S_M2_CH0_POLY */ "Polyethylene (PE)",
/* S_M2_CH0_MONO */ "Ethylene",
/* S_M2_CH0_HINT */ "Monomer: Ethylene (C2H4). Polyethylene is the world's most produced plastic.",
/* S_M2_CH1_POLY */ "Polypropylene (PP)",
/* S_M2_CH1_MONO */ "Propylene",
/* S_M2_CH1_HINT */ "Monomer: Propylene (C3H6). Used in caps, containers, and textile fibers.",
/* S_M2_CH2_POLY */ "Polystyrene (PS)",
/* S_M2_CH2_MONO */ "Styrene",
/* S_M2_CH2_HINT */ "Monomer: Styrene. Foam (EPS) is expanded polystyrene.",
/* S_M2_CH3_POLY */ "PVC",
/* S_M2_CH3_MONO */ "Vinyl chloride",
/* S_M2_CH3_HINT */ "Monomer: Vinyl chloride. The Cl in its chain makes it hard to recycle.",
/* S_M2_CH4_POLY */ "Nylon (PA6)",
/* S_M2_CH4_MONO */ "Caprolactam",
/* S_M2_CH4_HINT */ "Monomer: Caprolactam. A high-performance synthetic polymer.",
/* S_M2_CH5_POLY */ "Cellulose (natural)",
/* S_M2_CH5_MONO */ "Glucose",
/* S_M2_CH5_HINT */ "Monomer: Glucose. Cellulose is the most abundant natural polymer on Earth.",

/* M2 monomer palette */
/* S_MONO_0 */ "Ethylene",
/* S_MONO_1 */ "Propylene",
/* S_MONO_2 */ "Styrene",
/* S_MONO_3 */ "Vinyl chloride",
/* S_MONO_4 */ "Caprolactam",
/* S_MONO_5 */ "Glucose",
/* S_MONO_6 */ "Benzene",
/* S_MONO_7 */ "Acetylene",

/* M1 item names */
/* S_M1_ITEM0 */ "Water bottle",
/* S_M1_ITEM1 */ "Detergent container",
/* S_M1_ITEM2 */ "Pipe",
/* S_M1_ITEM3 */ "Shopping bag",
/* S_M1_ITEM4 */ "Yogurt lid",
/* S_M1_ITEM5 */ "Foam cup",

/* M4 country names */
/* S_M4_C0 */ "China",
/* S_M4_C1 */ "Indonesia",
/* S_M4_C2 */ "Philippines",
/* S_M4_C3 */ "Vietnam",
/* S_M4_C4 */ "Sri Lanka",
/* S_M4_C5 */ "Thailand",
/* S_M4_C6 */ "Egypt",
/* S_M4_C7 */ "Nigeria",
/* S_M4_C8 */ "Brazil",
/* S_M4_C9 */ "India",
/* S_M4_REGION_ASIA   */ "Asia",
/* S_M4_REGION_AFRICA */ "Africa",
/* S_M4_REGION_LAM    */ "LAm",

/* M4 river */
/* S_M4_RIVER_NAME     */ "Yangtze River (China)",
/* S_M4_RIVER_HINT     */ "The Yangtze discharges ~330,000 t of plastic into the sea yearly.",
/* S_M4_ACT3_CLICK_HINT */ "Click the orange dots in order",

/* M8 zone names */
/* S_M8_ZONE0 */ "Arctic",
/* S_M8_ZONE1 */ "N. Atlantic",
/* S_M8_ZONE2 */ "Tropical",
/* S_M8_ZONE3 */ "S. Atlantic",
/* S_M8_ZONE4 */ "Antarctic",

/* M8 short labels */
/* S_M8_TEMP_SHORT */ "T:%.0f%%",
/* S_M8_SALT_SHORT */ "S:%.0f%%",
/* S_M8_DENS_SHORT */ "D:%.2f",

/* S_BTN_NEXT */ "Next",

/* S_REPAIR_S4*/ "Right click to repair"
},
}; /* end s_strings */

const char *LOC(StrKey key) {
    if (key < 0 || key >= S_COUNT) return "???";
    return s_strings[(int)g_lang][key];
}
