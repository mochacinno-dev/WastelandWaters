#include "plastic_game.h"
#include "locale.h"

TriviaManager g_trivia;
SceneID       g_current_scene = SCENE_MAIN_MENU;

/* ──────────────────────────────────────────────────
   QUESTION BANKS  (Spanish + English)
────────────────────────────────────────────────── */
static Question s_question_bank_es[MAX_QUESTIONS] = {
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
/* ── MISIÓN 6 — Reciclaje ── */
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

/* ── ENGLISH QUESTION BANK ── */
static Question s_question_bank_en[MAX_QUESTIONS] = {
/* ── MISSION 1 — Types of Plastic ── */
{0,MISSION_1_TIPOS_PLASTICO,
 "What is the name of the plastic most commonly used to make water bottles?",
 {"PVC","HDPE","PET","PP"},2,
 "PET (Polyethylene Terephthalate) is plastic #1 and the most widely used for drink bottles thanks to its lightness and transparency."},
{1,MISSION_1_TIPOS_PLASTICO,
 "The acronym HDPE stands for:",
 {"Hydro-Dense Polymer Ethylene","High-Density Polyethylene",
  "Hard-Durability Polypropylene Ester","Hydrogen-Dense Polyethylene Expanded"},1,
 "HDPE = High-Density Polyethylene. It is used in detergent containers, pipes, and outdoor furniture."},
{2,MISSION_1_TIPOS_PLASTICO,
 "Polystyrene (foam) carries which number on the recycling symbol?",
 {"4","5","3","6"},3,
 "Polystyrene is resin code 6. It is difficult and costly to recycle and rarely accepted by curbside programs."},
{3,MISSION_1_TIPOS_PLASTICO,
 "Which of the following plastics is commonly used in construction pipes?",
 {"PET","PP","PVC","LDPE"},2,
 "PVC (Polyvinyl Chloride) is durable and inexpensive, ideal for pipes, window frames, and flooring."},
{4,MISSION_1_TIPOS_PLASTICO,
 "Low-density grocery bags are primarily made from:",
 {"PET","PVC","HDPE","LDPE"},3,
 "LDPE (Low-Density Polyethylene, #4) is flexible and translucent, perfect for bags and plastic films."},
{5,MISSION_1_TIPOS_PLASTICO,
 "Plastic number 7 is considered the most problematic because:",
 {"It is the most abundant plastic in the oceans",
  "It groups hard-to-recycle plastics and may contain BPA",
  "It is only produced in countries with little environmental regulation",
  "No current system can identify it"},1,
 "Category #7 is 'other'. It includes polycarbonate (which may contain BPA), bioplastics, and hard-to-recycle blends."},
/* ── MISSION 2 — Polymers ── */
{6,MISSION_2_POLIMEROS,
 "A polymer is defined as:",
 {"A simple molecule produced in a laboratory",
  "An organic compound derived solely from petrochemical sources",
  "A macromolecule formed by the repeated bonding of monomers",
  "An element in the periodic table with plastic-like properties"},2,
 "Polymers are long chains of repeated monomers. 'Poly' = many; 'meros' = parts."},
{7,MISSION_2_POLIMEROS,
 "The monomer that produces polyethylene is:",
 {"Propylene","Styrene","Ethylene (ethene)","Benzene"},2,
 "Polyethylene is made by polymerizing ethylene (CH2=CH2). It is the world's most produced plastic."},
{8,MISSION_2_POLIMEROS,
 "The main difference between a synthetic polymer and a natural one is:",
 {"Natural polymers are always stronger",
  "Synthetic ones are made by humans; natural ones exist in nature",
  "Synthetic ones degrade faster",
  "Natural ones contain more carbon"},1,
 "Natural polymers (cellulose, silk, rubber) occur in nature. Synthetic ones (nylon, PET) are manufactured by humans."},
{9,MISSION_2_POLIMEROS,
 "Which of the following is a natural polymer?",
 {"Nylon","PVC","Polystyrene","Cellulose"},3,
 "Cellulose is the most abundant natural polymer; it forms the walls of plant cells."},
{10,MISSION_2_POLIMEROS,
 "Plastics are so resistant to degradation mainly because:",
 {"They contain heavy metals that repel microorganisms",
  "Their long molecular chains are difficult for microorganisms to break down",
  "They are produced at extreme temperatures that make them inert",
  "They absorb moisture and become waterproof"},1,
 "The carbon chains of plastics are very stable. Microorganisms lack the enzymes to break them down efficiently."},
{11,MISSION_2_POLIMEROS,
 "Photodegradation in plastics is the process by which:",
 {"Plastic turns into gas when exposed to sunlight",
  "Microorganisms decompose plastic using light energy",
  "UV light fragments plastic into smaller particles without eliminating it",
  "Plastic absorbs radiation and naturally recycles itself"},2,
 "Photodegradation produces microplastics: plastic breaks into ever-smaller fragments but does NOT disappear."},
{12,MISSION_2_POLIMEROS,
 "How long can a PET bottle take to degrade in the ocean?",
 {"Between 10 and 50 years","Up to 450 years","About 100 years","Between 200 and 250 years"},1,
 "A PET bottle can take up to 450 years to degrade in the ocean, releasing microplastics throughout that entire time."},
/* ── MISSION 3 — Contaminants ── */
{13,MISSION_3_CONTAMINANTES,
 "Microplastics are defined as:",
 {"Biodegradable plastics of reduced size",
  "Plastic fragments smaller than 5 millimeters",
  "Visible plastic particles floating in the ocean",
  "Waste generated exclusively by the textile industry"},1,
 "Microplastics are fragments < 5 mm. Nanoplastics are < 1 micrometer and even more dangerous."},
{14,MISSION_3_CONTAMINANTES,
 "Microplastics intentionally manufactured for use in cosmetics are known as:",
 {"Industrial nanoplastics","Secondary microplastics",
  "Primary microplastics or microbeads","Cosmetic processing pellets"},2,
 "Primary microplastics (microbeads) are deliberately added to exfoliants and cosmetics. Many countries have now banned them."},
{15,MISSION_3_CONTAMINANTES,
 "BPA (Bisphenol A) primarily affects:",
 {"The respiratory system when inhaled",
  "The structural integrity of nearby plastics",
  "The endocrine system of living organisms",
  "Water salinity when it dissolves"},2,
 "BPA is an endocrine disruptor: it mimics estrogens and can alter hormonal development in humans and animals."},
{16,MISSION_3_CONTAMINANTES,
 "Regarding microplastics in the human body, studies have found that:",
 {"They accumulate only in the digestive system",
  "They have been detected in human blood",
  "The body eliminates them within 72 hours",
  "They only affect people living near coastal areas"},1,
 "In 2022, researchers published in Environment International the detection of microplastics in blood samples from anonymous donors."},
{17,MISSION_3_CONTAMINANTES,
 "Persistent organic pollutants (POPs) are linked to plastic because:",
 {"They are released only during recycling",
  "They are toxic substances that plastic absorbs and concentrates in the ocean",
  "They are found only in type 1 and type 2 plastics",
  "They are produced when plastic contacts saltwater"},1,
 "Plastics act as 'sponges' for POPs such as PCBs and DDT, concentrating them thousands of times above surrounding water levels."},
{18,MISSION_3_CONTAMINANTES,
 "The marine organism most vulnerable to mistaking microplastics for food is:",
 {"The great white shark","Zooplankton",
  "The bottlenose dolphin","Deep-sea coral"},1,
 "Zooplankton, the base of the marine food chain, ingests microplastics thinking they are phytoplankton, contaminating the entire food web."},
{19,MISSION_3_CONTAMINANTES,
 "Microplastics enter the human food chain mainly through:",
 {"Breathing air in coastal industrial areas",
  "Consuming contaminated fish, shellfish, water, and sea salt",
  "Direct contact with polluted beach sand",
  "Using single-use plastic utensils"},1,
 "Shellfish, fish, tap water, and sea salt are the primary routes of microplastic ingestion in humans."},
/* ── MISSION 4 — Countries ── */
{20,MISSION_4_PAISES,
 "The country that emits the most plastic into the oceans is:",
 {"China","India","Philippines","Indonesia"},0,
 "China tops the list of oceanic plastic pollution, followed by Indonesia, the Philippines, and Vietnam, according to Ocean Conservancy research."},
{21,MISSION_4_PAISES,
 "The world region that contributes the largest share of global oceanic plastic is:",
 {"Western Europe","North America","Sub-Saharan Africa","Asia, especially Southeast Asia"},3,
 "Southeast Asia accounts for more than 60% of global oceanic plastic, driven by high population density and limited waste management systems."},
{22,MISSION_4_PAISES,
 "What approximate percentage of marine plastic waste comes from rivers?",
 {"30%","55%","80%","95%"},2,
 "Approximately 80% of marine plastic comes from land-based sources transported by rivers, according to studies published in Science (2015)."},
{23,MISSION_4_PAISES,
 "The river that discharges the most plastic into the ocean is:",
 {"The Ganges, India","The Yangtze, China",
  "The Mekong, Vietnam","The Amazon, Brazil"},1,
 "The Yangtze River (Chang Jiang) discharges the most plastic into the ocean, contributing ~0.3 million tonnes annually."},
{24,MISSION_4_PAISES,
 "Regarding the USA's contribution to the Great Patch, which statement is correct?",
 {"It does not contribute significantly",
  "It is the second-largest oceanic polluter",
  "It contributes by exporting waste to countries with poor management",
  "It only pollutes internal rivers"},2,
 "The USA exports millions of tonnes of plastic waste to Asian countries where some ends up in the ocean, per The Last Beach Cleanup research."},
{25,MISSION_4_PAISES,
 "The Latin American country among the largest oceanic plastic polluters is:",
 {"Mexico","Colombia","Argentina","Brazil"},3,
 "Brazil ranks among the top 20 oceanic plastic polluters worldwide, primarily through its coastal rivers."},
/* ── MISSION 5 — Surface Currents ── */
{26,MISSION_5_CORRIENTES_SUP,
 "The main force responsible for ocean surface currents is:",
 {"Lunar gravity","Wind",
  "Temperature differences","Rotation of the Earth's core"},1,
 "Wind transfers energy to the ocean surface through friction, generating surface currents in the top 100–200 m of the ocean."},
{27,MISSION_5_CORRIENTES_SUP,
 "The effect that deflects ocean currents due to Earth's rotation is called:",
 {"The Doppler Effect","The Venturi Effect","The Coriolis Effect","The Bernoulli Effect"},2,
 "The Coriolis Effect, caused by Earth's rotation, deflects currents to the right in the northern hemisphere and to the left in the southern hemisphere."},
{28,MISSION_5_CORRIENTES_SUP,
 "In the northern hemisphere, the Coriolis effect deflects currents toward:",
 {"The south","The left","The north","The right"},3,
 "In the northern hemisphere, currents rotate clockwise (to the right). In the southern hemisphere, they rotate counterclockwise."},
{29,MISSION_5_CORRIENTES_SUP,
 "An ocean gyre is:",
 {"A deep cold-water current",
  "A circular system of currents rotating around a high-pressure center",
  "The vertical movement of water due to temperature changes",
  "A current generated by tropical storms"},1,
 "Ocean gyres are large circular systems of surface currents created by the Coriolis effect and trade and westerly winds."},
{30,MISSION_5_CORRIENTES_SUP,
 "How many major ocean gyres exist in the world's oceans?",
 {"3","7","5","9"},2,
 "There are 5 major ocean gyres: North Pacific, South Pacific, North Atlantic, South Atlantic, and Indian Ocean."},
{31,MISSION_5_CORRIENTES_SUP,
 "The surface current that runs along the western coasts of Mexico and California is called:",
 {"The Gulf Stream","The Humboldt Current",
  "The California Current","The North Equatorial Current"},2,
 "The California Current flows south along North America's western coast, carrying cold water from the North Pacific."},
/* ── MISSION 6 — Recycling ── */
{32,MISSION_6_RECICLAJE,
 "Of all plastic ever produced, the percentage that has been recycled is approximately:",
 {"9%","25%","40%","60%"},0,
 "Only 9% of all plastic ever produced has been recycled. 12% has been incinerated and the remaining 79% is in landfills or the environment (Science Advances, 2017)."},
{33,MISSION_6_RECICLAJE,
 "The three-arrow symbol on a plastic product indicates that:",
 {"The product was made from 100% recycled material",
  "It can be disposed of in any container",
  "It identifies the plastic type and whether it may be recyclable, without guaranteeing it",
  "It meets biodegradability standards"},2,
 "The numbered recycling symbol identifies the resin type. It does not guarantee that recycling facilities exist in your area."},
{34,MISSION_6_RECICLAJE,
 "Mechanical recycling consists of:",
 {"Breaking plastic down into chemical components using extreme heat",
  "Shredding and melting plastics to create new materials without altering their chemical composition",
  "Converting plastic into fuel via pyrolysis",
  "Sorting plastics by color and weight with machines"},1,
 "Mechanical recycling shreds, washes, and melts plastic. It is the most common method but degrades material quality with each cycle."},
{35,MISSION_6_RECICLAJE,
 "Chemical recycling differs from mechanical recycling because it:",
 {"Only applies to transparent plastics",
  "Requires less energy and produces less waste",
  "Breaks plastic down into basic chemical components to create new materials",
  "Is the only method that works on food-contaminated plastics"},2,
 "Chemical recycling (pyrolysis, gasification, depolymerization) breaks molecular chains, allowing virgin raw materials to be recovered."},
{36,MISSION_6_RECICLAJE,
 "The main problem with recycling PVC plastic is that:",
 {"It is too fragile for shredding",
  "It can only be recycled once",
  "It releases toxic substances and contaminates other recyclable plastics",
  "Its black color prevents optical sorting"},2,
 "PVC contains chlorine additives and plasticizers that are released during recycling, contaminating the entire mixed-plastic batch."},
{37,MISSION_6_RECICLAJE,
 "The 3Rs rule states that actions should be applied in this order:",
 {"Recycle, Reduce, and Reuse",
  "Reuse, Recycle, and Reduce",
  "Reduce, Recycle, and Reuse",
  "Reduce, Reuse, and Recycle"},3,
 "The correct order is Reduce (highest priority), Reuse, and finally Recycle. Reducing consumption is always the best environmental choice."},
{38,MISSION_6_RECICLAJE,
 "Annual global plastic production is approximately:",
 {"50 million tonnes","More than 400 million tonnes",
  "150 million tonnes","Around 800 million tonnes"},1,
 "In 2022, more than 400 million tonnes of plastic were produced globally. At this rate, 1 billion tonnes are projected by 2050."},
/* ── MISSION 7 — Plastic Islands ── */
{39,MISSION_7_ISLAS_PLASTICO,
 "The official name of the largest accumulation of plastic waste in the Pacific is:",
 {"North Pacific Dead Zone",
  "Pacific Waste Archipelago",
  "Great Pacific Garbage Patch",
  "Central Marine Waste Depot"},2,
 "The Great Pacific Garbage Patch is the largest of the 5 known oceanic garbage patches."},
{40,MISSION_7_ISLAS_PLASTICO,
 "How many major garbage patches exist in the world's oceans?",
 {"2","3","7","5"},3,
 "There are 5 major oceanic garbage patches, one per principal ocean gyre: 2 in the Pacific, 2 in the Atlantic, and 1 in the Indian Ocean."},
{41,MISSION_7_ISLAS_PLASTICO,
 "In terms of appearance, the Great Pacific Garbage Patch:",
 {"Is a solid island visible from satellites",
  "Is mainly a concentration of microplastics suspended in the water",
  "Forms a dense layer of whole plastic items",
  "Is only detectable by sonar at depth"},1,
 "The Great Patch is not a solid island; it is a 'soup' of microplastics and suspended particles that makes the water murky and milky."},
{42,MISSION_7_ISLAS_PLASTICO,
 "The approximate amount of plastic in the Great Pacific Garbage Patch is:",
 {"Around 10,000 tonnes",
  "More than 2 million tonnes",
  "Around 80,000 tonnes",
  "Around 500,000 tonnes"},1,
 "The Ocean Cleanup estimates over 80,000 tonnes, though more recent research suggests it may exceed 2 million tonnes."},
{43,MISSION_7_ISLAS_PLASTICO,
 "The Great Pacific Garbage Patch was discovered by:",
 {"David Attenborough in 2001",
  "NASA via satellite in 1989",
  "Oceanographer Charles Moore in 1997",
  "Greenpeace researchers in 2003"},2,
 "Charles Moore, sailor and oceanographer, discovered the Great Patch in 1997 while sailing from Hawaii to California aboard the vessel Alguita."},
/* ── MISSION 8 — Ocean Currents ── */
{44,MISSION_8_CORRIENTES_OCEAN,
 "The global system of deep ocean currents is known as:",
 {"The Antarctic Circumpolar Current",
  "The Ocean Conveyor Belt or Thermohaline Circulation",
  "The Global Deep Gyre System",
  "The Planetary Density Current"},1,
 "The Thermohaline Circulation, or 'Ocean Conveyor Belt', moves water through all oceans in a cycle that can last 1,000 years."},
{45,MISSION_8_CORRIENTES_OCEAN,
 "Thermohaline currents are driven by:",
 {"Wind speed and atmospheric pressure",
  "Water temperature and salinity",
  "Seafloor depth and composition",
  "Surface temperature and solar radiation"},1,
 "'Thermo' = temperature; 'haline' = salinity. These two variables determine water density and therefore whether it sinks or rises."},
{46,MISSION_8_CORRIENTES_OCEAN,
 "Cold, salty water tends to sink in the ocean because:",
 {"It contains less dissolved oxygen",
  "Atmospheric pressure pushes it down",
  "It is denser than warm, less salty water",
  "Polar winds drag it into the depths"},2,
 "Water density increases as temperature drops and salinity rises. Dense water sinks, driving thermohaline circulation."},
{47,MISSION_8_CORRIENTES_OCEAN,
 "The ocean current responsible for Western Europe's mild climate is:",
 {"The Humboldt Current","The California Current",
  "The Circumpolar Current","The Gulf Stream"},3,
 "The Gulf Stream carries warm tropical water northward, giving Western Europe a climate up to 10°C warmer than its latitude would otherwise allow."},
{48,MISSION_8_CORRIENTES_OCEAN,
 "Climate change affects ocean currents primarily because:",
 {"It increases global wind speeds",
  "Polar ice melt adds fresh water that reduces salinity and may weaken thermohaline circulation",
  "It raises ocean temperature uniformly",
  "It generates more storms that temporarily disrupt current patterns"},1,
 "Glacial meltwater dilutes salinity in the North Atlantic, where the thermohaline current 'sinks'. This can weaken or collapse the AMOC."},
{49,MISSION_8_CORRIENTES_OCEAN,
 "Ocean currents are key to understanding plastic accumulation because:",
 {"They gradually dissolve plastic as they transport it",
  "They distribute plastic evenly across the seafloor",
  "They transport and concentrate plastic in the centers of ocean gyres",
  "They carry plastic to coastlines for collection"},2,
 "Ocean gyres act as traps: their low-pressure centers are calm and accumulate everything that floats, including plastics."},
};

/* ──────────────────────────────────────────────────
   INIT & LOAD
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
    Question *bank = (g_lang == LANG_EN) ? s_question_bank_en : s_question_bank_es;
    int count = MAX_QUESTIONS;
    memcpy(g_trivia.questions, bank, count * sizeof(Question));
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
   TRIVIA PANEL DRAW
────────────────────────────────────────────────── */
void TriviaManager_DrawPanel(Rectangle r) {
    const Question *q = TriviaManager_CurrentQuestion();

    DrawRectangleRec(r, COL_UI_PANEL);
    DrawRectangleLinesEx(r, 2, COL_UI_BORDER);

    float pad = 24.0f;
    float cx  = r.x + pad;
    float cy  = r.y + pad;
    float pw  = r.width - 2*pad;

    /* progress */
    char prog[40];
    snprintf(prog, sizeof(prog), LOC(S_QUESTION_PROGRESS),
             g_trivia.current_q_pos + 1, QUESTIONS_PER_MISSION);
    DrawText(prog, (int)cx, (int)cy, 20, COL_UI_BORDER);
    cy += 30;

    /* score */
    char sc[40];
    snprintf(sc, sizeof(sc), LOC(S_SCORE_LABEL), g_trivia.score);
    DrawText(sc, (int)(r.x + r.width - pad - 160), (int)(r.y + pad), 20, COL_UI_ACCENT);

    /* streak */
    if (g_trivia.streak >= 2) {
        char st[32];
        snprintf(st, sizeof(st), LOC(S_STREAK_LABEL), g_trivia.streak);
        DrawText(st, (int)(r.x + r.width - pad - 130), (int)(r.y + pad + 28), 17, COL_PLASTIC_YEL);
    }

    cy += 14;

    if (!q) {
        if (g_trivia.state == TRIVIA_MISSION_COMPLETE) {
            DrawText(LOC(S_MISSION_COMPLETE), (int)cx, (int)(cy + 80), 28, COL_CORRECT);
            char final_sc[64];
            snprintf(final_sc, sizeof(final_sc), LOC(S_FINAL_SCORE), g_trivia.score);
            DrawText(final_sc, (int)cx, (int)(cy + 120), 22, WHITE);
        }
        return;
    }

    /* question text */
    UI_DrawWrappedText(q->text, (int)cx, (int)cy, (int)pw, 20, WHITE);
    cy += 130;

    /* answer buttons */
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
        DrawText(g_trivia.answer_correct ? LOC(S_CORRECT) : LOC(S_INCORRECT),
                 (int)cx, (int)cy, 24, fbcol);
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
   MISSION INTRO SCREEN
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

        for (int i = 0; i < 30; i++) {
            float x = (sinf(anim * 0.25f + i * 1.9f) * 0.5f + 0.5f) * SCREEN_W;
            float y = (cosf(anim * 0.18f + i * 2.3f) * 0.5f + 0.5f) * SCREEN_H;
            DrawCircle((int)x, (int)y, 4 + i%6, ColorAlpha(COL_OCEAN_LIGHT, 0.18f));
        }

        int cw = 920, ch = 560;
        int cx = (SCREEN_W - cw) / 2;
        int cy = (SCREEN_H - ch) / 2;
        DrawRectangleRounded((Rectangle){(float)cx,(float)cy,(float)cw,(float)ch},
                              0.05f, 12, COL_UI_PANEL);
        DrawRectangleRoundedLines((Rectangle){(float)cx,(float)cy,(float)cw,(float)ch},
                           0.05f, 12, 2.0f, COL_UI_BORDER);

        char mn[32];
        snprintf(mn, sizeof(mn), LOC(S_MISSION_LABEL), mission_number);
        int mnw = MeasureText(mn, 20);
        DrawText(mn, cx + (cw - mnw)/2, cy + 28, 20, COL_UI_BORDER);

        int tw = MeasureText(mission_title, 36);
        DrawText(mission_title, cx + (cw - tw)/2, cy + 58, 36, COL_UI_ACCENT);

        DrawLineEx((Vector2){(float)(cx+40),(float)(cy+108)},
                   (Vector2){(float)(cx+cw-40),(float)(cy+108)}, 2, COL_UI_BORDER);

        DrawText(topic_title, cx + 40, cy + 122, 22, COL_PLASTIC_YEL);
        UI_DrawWrappedText(body_text, cx + 40, cy + 158, cw - 80, 19, LIGHTGRAY);

        Rectangle btn_start = {(float)(cx + cw/2 - 220), (float)(cy + ch - 90), 200, 56};
        Rectangle btn_menu  = {(float)(cx + cw/2 + 20),  (float)(cy + ch - 90), 200, 56};

        if (UI_Button(btn_start, LOC(S_BTN_START), COL_UI_ACCENT, BLACK)) {
            EndDrawing();
            return true;
        }
        if (UI_Button(btn_menu, LOC(S_BTN_MENU_MAIN), COL_UI_PANEL, WHITE)) {
            EndDrawing();
            return false;
        }

        EndDrawing();
    }
    return false;
}

/* ──────────────────────────────────────────────────
   UI UTILITIES
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
    DrawRectangleRoundedLines(r, (float)radius / 100.0f, 8, 2.0f, border);
}
