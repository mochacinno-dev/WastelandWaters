#ifndef LOCALE_H
#define LOCALE_H

/* ─────────────────────────────────────────────
   LANGUAGE SYSTEM
   Usage:  LOC(KEY_FOO)  returns const char*
   Toggle: g_lang = LANG_EN / LANG_ES
───────────────────────────────────────────── */

typedef enum { LANG_ES = 0, LANG_EN = 1 } Language;
extern Language g_lang;

/* ── String keys ────────────────────────────── */
typedef enum {

    /* ── Main Menu ── */
    S_GAME_TITLE,
    S_GAME_SUBTITLE,
    S_BTN_START_ALL,
    S_BTN_MENU_MAIN,
    S_SCORE_ACCUM,
    S_VERSION,
    S_LANG_ES,
    S_LANG_EN,

    /* ── Mission names (menu buttons) ── */
    S_M1_BTN, S_M2_BTN, S_M3_BTN, S_M4_BTN,
    S_M5_BTN, S_M6_BTN, S_M7_BTN, S_M8_BTN,

    /* ── Generic HUD ── */
    S_QUESTION_PROGRESS,   /* "Pregunta %d / %d" — use snprintf with this as fmt */
    S_SCORE_LABEL,
    S_STREAK_LABEL,
    S_CORRECT,
    S_INCORRECT,
    S_MISSION_COMPLETE,
    S_FINAL_SCORE,
    S_BTN_START,
    S_LOADING_TRIVIA,
    S_MISSION_LABEL,       /* "MISIÓN %d" */

    /* ── Results screen ── */
    S_GAME_COMPLETE,
    S_THANKS_MSG,
    S_REDUCE_MSG,
    S_PATCH_FACT,
    S_KNOWLEDGE_MSG,
    S_BTN_BACK_START,

    /* ── Mission 1 ── */
    S_M1_TITLE, S_M1_SUBTITLE, S_M1_INSTRUCTION,
    S_M1_PLACED, S_M1_CLASSIFY_DONE, S_M1_LOADING,
    S_M1_TRIVIA_TITLE,
    S_M1_INTRO_TITLE, S_M1_INTRO_TOPIC, S_M1_INTRO_BODY,

    /* ── Mission 2 ── */
    S_M2_TITLE, S_M2_INSTRUCTION,
    S_M2_BUILD_LABEL, S_M2_CHALLENGE_PROG, S_M2_MONOMERS_LABEL,
    S_M2_CHAIN_OK, S_M2_CHAIN_WRONG,
    S_M2_TRIVIA_TITLE,
    S_M2_INTRO_TITLE, S_M2_INTRO_TOPIC, S_M2_INTRO_BODY,

    /* ── Mission 3 ── */
    S_M3_TITLE, S_M3_INSTRUCTION,
    S_M3_FOOD_CHAIN_HP, S_M3_WAVE, S_M3_CAPTURED,
    S_M3_WAVE_DONE, S_M3_RIVER_OK, S_M3_RIVER_FAIL,
    S_M3_FILTER_LABEL, S_M3_LOADING_TRIVIA,
    S_M3_TRIVIA_TITLE,
    S_M3_INTRO_TITLE, S_M3_INTRO_TOPIC, S_M3_INTRO_BODY,

    /* ── Mission 4 ── */
    S_M4_TITLE,
    S_M4_ACT1_TITLE, S_M4_ACT2_TITLE, S_M4_ACT3_TITLE, S_M4_DONE_TITLE,
    S_M4_RANK_LABEL, S_M4_MAP_LABEL,
    S_M4_ACT3_HINT,
    S_M4_CORRECT_ASIA, S_M4_WRONG_REGION, S_M4_RANK_OK, S_M4_RIVER_OK,
    S_M4_TRIVIA_TITLE,
    S_M4_INTRO_TITLE, S_M4_INTRO_TOPIC, S_M4_INTRO_BODY,

    /* ── Mission 5 ── */
    S_M5_TITLE, S_M5_INSTRUCTION,
    S_M5_GYRES_HUD, S_M5_GOAL,
    S_M5_CORIOLIS_LINE1, S_M5_CORIOLIS_LINE2,
    S_M5_NORTH_LABEL, S_M5_SOUTH_LABEL, S_M5_EQUATOR,
    S_M5_GYRE_N, S_M5_GYRE_S,
    S_M5_TRIVIA_TITLE,
    S_M5_INTRO_TITLE, S_M5_INTRO_TOPIC, S_M5_INTRO_BODY,

    /* ── Mission 6 ── */
    S_M6_TITLE, S_M6_INSTRUCTION,
    S_M6_HUD, S_M6_REMAINING,
    S_M6_BTN_MECH, S_M6_BTN_CHEM, S_M6_BTN_WASTE,
    S_M6_GOAL_LABEL,
    S_M6_TRIVIA_TITLE,
    S_M6_INTRO_TITLE, S_M6_INTRO_TOPIC, S_M6_INTRO_BODY,

    /* ── Mission 7 ── */
    S_M7_TITLE, S_M7_INSTRUCTION,
    S_M7_COLLECTED, S_M7_PATCH_LABEL, S_M7_CLEANED,
    S_M7_TRIVIA_TITLE,
    S_M7_INTRO_TITLE, S_M7_INTRO_TOPIC, S_M7_INTRO_BODY,

    /* ── Mission 8 ── */
    S_M8_TITLE, S_M8_INSTRUCTION,
    S_M8_SELECT_ZONE, S_M8_TEMP_LABEL, S_M8_SALT_LABEL,
    S_M8_ICE_BTN, S_M8_ICE_NOTE, S_M8_AMOC_LABEL, S_M8_AMOC_COLLAPSED,
    S_M8_ICE_MSG, S_M8_STATUS_DEFAULT,
    S_M8_INTERACT_LABEL,
    S_M8_TRIVIA_TITLE,
    S_M8_INTRO_TITLE, S_M8_INTRO_TOPIC, S_M8_INTRO_BODY,

    /* ── Item definitions (mission 6) ── */
    S_M6_ITEM0_NAME, S_M6_ITEM0_REASON,
    S_M6_ITEM1_NAME, S_M6_ITEM1_REASON,
    S_M6_ITEM2_NAME, S_M6_ITEM2_REASON,
    S_M6_ITEM3_NAME, S_M6_ITEM3_REASON,
    S_M6_ITEM4_NAME, S_M6_ITEM4_REASON,
    S_M6_ITEM5_NAME, S_M6_ITEM5_REASON,
    S_M6_ITEM6_NAME, S_M6_ITEM6_REASON,
    S_M6_ITEM7_NAME, S_M6_ITEM7_REASON,
    S_M6_INCORRECT_FMT,   /* "Incorrect. Should be %s. %s" */
    S_M6_PROC_MECH, S_M6_PROC_CHEM, S_M6_PROC_WASTE,

    /* ── Mission 2 polymer challenges ── */
    S_M2_CH0_POLY, S_M2_CH0_MONO, S_M2_CH0_HINT,
    S_M2_CH1_POLY, S_M2_CH1_MONO, S_M2_CH1_HINT,
    S_M2_CH2_POLY, S_M2_CH2_MONO, S_M2_CH2_HINT,
    S_M2_CH3_POLY, S_M2_CH3_MONO, S_M2_CH3_HINT,
    S_M2_CH4_POLY, S_M2_CH4_MONO, S_M2_CH4_HINT,
    S_M2_CH5_POLY, S_M2_CH5_MONO, S_M2_CH5_HINT,

    /* ── Mission 2 monomer palette names ── */
    S_MONO_0, S_MONO_1, S_MONO_2, S_MONO_3,
    S_MONO_4, S_MONO_5, S_MONO_6, S_MONO_7,

    /* ── Mission 1 plastic item names ── */
    S_M1_ITEM0, S_M1_ITEM1, S_M1_ITEM2,
    S_M1_ITEM3, S_M1_ITEM4, S_M1_ITEM5,

    /* ── Mission 4 country names ── */
    S_M4_C0,S_M4_C1,S_M4_C2,S_M4_C3,S_M4_C4,
    S_M4_C5,S_M4_C6,S_M4_C7,S_M4_C8,S_M4_C9,
    S_M4_REGION_ASIA, S_M4_REGION_AFRICA, S_M4_REGION_LAM,

    /* ── Mission 4 river ── */
    S_M4_RIVER_NAME, S_M4_RIVER_HINT,
    S_M4_ACT3_CLICK_HINT,

    /* ── Mission 8 ocean zone names ── */
    S_M8_ZONE0, S_M8_ZONE1, S_M8_ZONE2, S_M8_ZONE3, S_M8_ZONE4,

    /* ── Mission 8 density labels ── */
    S_M8_TEMP_SHORT, S_M8_SALT_SHORT, S_M8_DENS_SHORT,

    /* must be last */
    S_COUNT
} StrKey;

const char *LOC(StrKey key);

#endif /* LOCALE_H */
