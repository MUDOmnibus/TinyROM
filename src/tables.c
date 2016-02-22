/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik Strfeldt, Tom Madsen, and Katja Nyboe.    *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
 *  ROM 2.4 is copyright 1993-1998 Russ Taylor                             *
 *  ROM has been brought to you by the ROM consortium                      *
 *      Russ Taylor (rtaylor@hypercube.org)                                *
 *      Gabrielle Taylor (gtaylor@hypercube.org)                           *
 *      Brian Moore (zump@rom.org)                                         *
 *  By using this code, you have agreed to follow the terms of the         *
 *  ROM license, in the file Rom24/doc/rom.license                         *
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <time.h>
#include "merc.h"
#include "tables.h"

/* for clans */
const struct clan_type clan_table[MAX_CLAN] = {
    /*  name,       who entry,  death-transfer room,    independent */
    /* independent should be FALSE if is a real clan */
    {"", "", ROOM_VNUM_ALTAR, TRUE},
    {"loner", "[ Loner ] ", ROOM_VNUM_ALTAR, TRUE},
    {"rom", "[  ROM  ] ", ROOM_VNUM_ALTAR, FALSE}
};

/* for position */
const struct position_type position_table[] = {
    {"dead", "dead"},
    {"mortally wounded", "mort"},
    {"incapacitated", "incap"},
    {"stunned", "stun"},
    {"sleeping", "sleep"},
    {"resting", "rest"},
    {"sitting", "sit"},
    {"fighting", "fight"},
    {"standing", "stand"},
    {NULL, NULL}
};

/* for sex */
const struct sex_type sex_table[] = {
    {"none"},
    {"male"},
    {"female"},
    {"either"},
    {NULL}
};

/* for sizes */
const struct size_type size_table[] = {
    {"tiny"},
    {"small"},
    {"medium"},
    {"large"},
    {"huge",},
    {"giant"},
    {NULL}
};

/* various flag tables */
const struct flag_type act_flags[] = {
    {"npc", ACT_IS_NPC, FALSE},
    {"sentinel", ACT_SENTINEL, TRUE},
    {"stay_area", ACT_STAY_AREA, TRUE},
    {"wimpy", ACT_WIMPY, TRUE},
    {"pet", ACT_PET, TRUE},
    {"train", ACT_TRAIN, TRUE},
    {"practice", ACT_PRACTICE, TRUE},
    {"noalign", ACT_NOALIGN, TRUE},
    {"nopurge", ACT_NOPURGE, TRUE},
    {"outdoors", ACT_OUTDOORS, TRUE},
    {"indoors", ACT_INDOORS, TRUE},
    {"gain", ACT_GAIN, TRUE},
    {"update_always", ACT_UPDATE_ALWAYS, TRUE},
    {"changer", ACT_IS_CHANGER, TRUE},
    {NULL, 0, FALSE}
};

const struct flag_type plr_flags[] = {
    {"npc", A, FALSE},
    {"autoassist", C, FALSE},
    {"autoexit", D, FALSE},
    {"autoloot", E, FALSE},
    {"autosac", F, FALSE},
    {"autogold", G, FALSE},
    {"autosplit", H, FALSE},
    {"holylight", N, FALSE},
    {"can_loot", P, FALSE},
    {"nosummon", Q, FALSE},
    {"nofollow", R, FALSE},
    {"colour", T, FALSE},
    {"permit", U, TRUE},
    {"log", W, FALSE},
    {"deny", X, FALSE},
    {"freeze", Y, FALSE},
    {"thief", Z, FALSE},
    {"killer", aa, FALSE},
    {NULL, 0, 0}
};

const struct flag_type affect_flags[] = {
    {"blind", AFF_BLIND, TRUE},
    {"sneak", AFF_SNEAK, TRUE},
    {"hide", AFF_HIDE, TRUE},
    {"charm", AFF_CHARM, TRUE},
    {"flying", AFF_FLYING, TRUE},
    {"dark_vision", AFF_DARK_VISION, TRUE},
    {"swim", AFF_SWIM, TRUE},
    {NULL, 0, 0}
};

const struct flag_type off_flags[] = {
    {"area_attack", OFF_AREA_ATTACK, TRUE},
    {"assist_all", ASSIST_ALL, TRUE},
    {"assist_align", ASSIST_ALIGN, TRUE},
    {"assist_race", ASSIST_RACE, TRUE},
    {"assist_players", ASSIST_PLAYERS, TRUE},
    {"assist_vnum", ASSIST_VNUM, TRUE},
    {NULL, 0, 0}
};

const struct flag_type imm_flags[] = {
    {"summon", IMM_SUMMON, TRUE},
    {"charm", IMM_CHARM, TRUE},
    {"magic", IMM_MAGIC, TRUE},
    {"weapon", IMM_WEAPON, TRUE},
    {"bash", IMM_BASH, TRUE},
    {"pierce", IMM_PIERCE, TRUE},
    {"slash", IMM_SLASH, TRUE},
    {"fire", IMM_FIRE, TRUE},
    {"cold", IMM_COLD, TRUE},
    {"lightning", IMM_LIGHTNING, TRUE},
    {"acid", IMM_ACID, TRUE},
    {NULL, 0, 0}
};

const struct flag_type form_flags[] = {
    {"edible", FORM_EDIBLE, TRUE},
    {"instant_decay", FORM_INSTANT_DECAY, TRUE},
    {NULL, 0, 0}
};

const struct flag_type part_flags[] = {
    {"head", PART_HEAD, TRUE},
    {"arms", PART_ARMS, TRUE},
    {"legs", PART_LEGS, TRUE},
    {"heart", PART_HEART, TRUE},
    {"brains", PART_BRAINS, TRUE},
    {"guts", PART_GUTS, TRUE},
    {NULL, 0, 0}
};

const struct flag_type comm_flags[] = {
    {"quiet", COMM_QUIET, TRUE},
    {"deaf", COMM_DEAF, TRUE},
    {"nowiz", COMM_NOWIZ, TRUE},
    {"noclangossip", COMM_NOAUCTION, TRUE},
    {"nogossip", COMM_NOGOSSIP, TRUE},
    {"noquestion", COMM_NOQUESTION, TRUE},
    {"nomusic", COMM_NOMUSIC, TRUE},
    {"noclan", COMM_NOCLAN, TRUE},
    {"noquote", COMM_NOQUOTE, TRUE},
    {"shoutsoff", COMM_SHOUTSOFF, TRUE},
    {"compact", COMM_COMPACT, TRUE},
    {"brief", COMM_BRIEF, TRUE},
    {"prompt", COMM_PROMPT, TRUE},
    {"combine", COMM_COMBINE, TRUE},
    {"telnet_ga", COMM_TELNET_GA, TRUE},
    {"show_affects", COMM_SHOW_AFFECTS, TRUE},
    {"nograts", COMM_NOGRATS, TRUE},
    {"noemote", COMM_NOEMOTE, FALSE},
    {"noshout", COMM_NOSHOUT, FALSE},
    {"notell", COMM_NOTELL, FALSE},
    {"nochannels", COMM_NOCHANNELS, FALSE},
    {"snoop_proof", COMM_SNOOP_PROOF, FALSE},
    {"afk", COMM_AFK, TRUE},
    {NULL, 0, 0}
};

const struct flag_type mprog_flags[] = {
    {"act", TRIG_ACT, TRUE},
    {"bribe", TRIG_BRIBE, TRUE},
    {"death", TRIG_DEATH, TRUE},
    {"entry", TRIG_ENTRY, TRUE},
    {"fight", TRIG_FIGHT, TRUE},
    {"give", TRIG_GIVE, TRUE},
    {"greet", TRIG_GREET, TRUE},
    {"grall", TRIG_GRALL, TRUE},
    {"kill", TRIG_KILL, TRUE},
    {"hpcnt", TRIG_HPCNT, TRUE},
    {"random", TRIG_RANDOM, TRUE},
    {"speech", TRIG_SPEECH, TRUE},
    {"exit", TRIG_EXIT, TRUE},
    {"exall", TRIG_EXALL, TRUE},
    {"delay", TRIG_DELAY, TRUE},
    {NULL, 0, TRUE}
};

const struct flag_type area_flags[] = {
    {"none", AREA_NONE, FALSE},
    {"changed", AREA_CHANGED, TRUE},
    {"added", AREA_ADDED, TRUE},
    {"loading", AREA_LOADING, FALSE},
    {NULL, 0, 0}
};



const struct flag_type sex_flags[] = {
    {"male", SEX_MALE, TRUE},
    {"female", SEX_FEMALE, TRUE},
    {"neutral", SEX_NEUTRAL, TRUE},
    {"random", 3, TRUE},        /* ROM */
    {"none", SEX_NEUTRAL, TRUE},
    {NULL, 0, 0}
};



const struct flag_type exit_flags[] = {
    {"door", EX_ISDOOR, TRUE},
    {"closed", EX_CLOSED, TRUE},
    {"locked", EX_LOCKED, TRUE},
    {"pickproof", EX_PICKPROOF, TRUE},
    {"noclose", EX_NOCLOSE, TRUE},
    {"nolock", EX_NOLOCK, TRUE},
    {NULL, 0, 0}
};



const struct flag_type door_resets[] = {
    {"open and unlocked", 0, TRUE},
    {"closed and unlocked", 1, TRUE},
    {"closed and locked", 2, TRUE},
    {NULL, 0, 0}
};



const struct flag_type room_flags[] = {
    {"dark", ROOM_DARK, TRUE},
    {"no_mob", ROOM_NO_MOB, TRUE},
    {"indoors", ROOM_INDOORS, TRUE},
    {"private", ROOM_PRIVATE, TRUE},
    {"safe", ROOM_SAFE, TRUE},
    {"solitary", ROOM_SOLITARY, TRUE},
    {"pet_shop", ROOM_PET_SHOP, TRUE},
    {"no_recall", ROOM_NO_RECALL, TRUE},
    {"imp_only", ROOM_IMP_ONLY, TRUE},
    {"gods_only", ROOM_GODS_ONLY, TRUE},
    {"heroes_only", ROOM_HEROES_ONLY, TRUE},
    {"newbies_only", ROOM_NEWBIES_ONLY, TRUE},
    {"nowhere", ROOM_NOWHERE, TRUE},
    {NULL, 0, 0}
};



const struct flag_type sector_flags[] = {
    {"inside", SECT_INSIDE, TRUE},
    {"city", SECT_CITY, TRUE},
    {"field", SECT_FIELD, TRUE},
    {"forest", SECT_FOREST, TRUE},
    {"hills", SECT_HILLS, TRUE},
    {"mountain", SECT_MOUNTAIN, TRUE},
    {"swim", SECT_WATER_SWIM, TRUE},
    {"noswim", SECT_WATER_NOSWIM, TRUE},
    {"unused", SECT_UNUSED, TRUE},
    {"air", SECT_AIR, TRUE},
    {"desert", SECT_DESERT, TRUE},
    {NULL, 0, 0}
};


const struct flag_type type_flags[] = {
    {"light", ITEM_LIGHT, TRUE},
    {"weapon", ITEM_WEAPON, TRUE},
    {"armor", ITEM_ARMOR, TRUE},
    {"trash", ITEM_TRASH, TRUE},
    {"clothing", ITEM_CLOTHING, TRUE},
    {"furniture", ITEM_FURNITURE, TRUE},
    {"container", ITEM_CONTAINER, TRUE},
    {"drinkcontainer", ITEM_DRINK_CON, TRUE},
    {"food", ITEM_FOOD, TRUE},
    {"money", ITEM_MONEY, TRUE},
    {"npccorpse", ITEM_CORPSE_NPC, TRUE},
    {"pc corpse", ITEM_CORPSE_PC, FALSE},
    {"fountain", ITEM_FOUNTAIN, TRUE},
    {"portal", ITEM_PORTAL, TRUE},
    {NULL, 0, 0}
};


const struct flag_type extra_flags[] = {
    {"nodrop", ITEM_NODROP, TRUE},
    {"noremove", ITEM_NOREMOVE, TRUE},
    {"inventory", ITEM_INVENTORY, TRUE},
    {"nopurge", ITEM_NOPURGE, TRUE},
    {"rotdeath", ITEM_ROT_DEATH, TRUE},
    {"visdeath", ITEM_VIS_DEATH, TRUE},
    {"meltdrop", ITEM_MELT_DROP, TRUE},
    {"sellextract", ITEM_SELL_EXTRACT, TRUE},
    {NULL, 0, 0}
};



const struct flag_type wear_flags[] = {
    {"take", ITEM_TAKE, TRUE},
    {"relic", ITEM_WEAR_RELIC, TRUE},
    {"head", ITEM_WEAR_HEAD, TRUE},
    {"clothes", ITEM_WEAR_CLOTHES, TRUE},
    {"armor", ITEM_WEAR_ARMOR, TRUE},
    {"shield", ITEM_WEAR_SHIELD, TRUE},
    {"wield", ITEM_WIELD, TRUE},
    {"hold", ITEM_HOLD, TRUE},
/*    {   "twohands",            ITEM_TWO_HANDS,         TRUE    }, */
    {NULL, 0, 0}
};

/*
 * Used when adding an affect to tell where it goes.
 * See addaffect and delaffect in act_olc.c
 */
const struct flag_type apply_flags[] = {
    {"none", APPLY_NONE, TRUE},
    {"strength", APPLY_STR, TRUE},
    {"dexterity", APPLY_DEX, TRUE},
    {"intelligence", APPLY_INT, TRUE},
    {"wisdom", APPLY_WIS, TRUE},
    {"constitution", APPLY_CON, TRUE},
    {"sex", APPLY_SEX, TRUE},
    {"class", APPLY_CLASS, TRUE},
    {"level", APPLY_LEVEL, TRUE},
    {"age", APPLY_AGE, TRUE},
    {"height", APPLY_HEIGHT, TRUE},
    {"weight", APPLY_WEIGHT, TRUE},
    {"mana", APPLY_MANA, TRUE},
    {"hp", APPLY_HIT, TRUE},
    {"move", APPLY_MOVE, TRUE},
    {"gold", APPLY_GOLD, TRUE},
    {"experience", APPLY_EXP, TRUE},
    {"ac", APPLY_AC, TRUE},
    {"hitroll", APPLY_HITROLL, TRUE},
    {"damroll", APPLY_DAMROLL, TRUE},
    {"saves", APPLY_SAVES, TRUE},
    {"spell_affect", APPLY_SPELL_AFFECT, TRUE},
    {NULL, 0, 0}
};



/*
 * What is seen.
 */
const struct flag_type wear_loc_strings[] = {
    {"in the inventory", WEAR_NONE, TRUE},
    {"as a relic (1)", WEAR_RELIC_1, TRUE},
    {"as a relic (2)", WEAR_RELIC_2, TRUE},
    {"over the head", WEAR_HEAD, TRUE},
    {"on the body", WEAR_CLOTHES, TRUE},
    {"as armor", WEAR_ARMOR, TRUE},
    {"as a shield", WEAR_SHIELD, TRUE},
    {"wielded", WEAR_WIELD, TRUE},
    {"held in the hands", WEAR_HOLD, TRUE},
    {NULL, 0, 0}
};


const struct flag_type wear_loc_flags[] = {
    {"none", WEAR_NONE, TRUE},
    {"relic1", WEAR_RELIC_1, TRUE},
    {"relic2", WEAR_RELIC_2, TRUE},
    {"head", WEAR_HEAD, TRUE},
    {"clothes", WEAR_CLOTHES, TRUE},
    {"armor", WEAR_ARMOR, TRUE},
    {"shield", WEAR_SHIELD, TRUE},
    {"wielded", WEAR_WIELD, TRUE},
    {"hold", WEAR_HOLD, TRUE},
    {NULL, 0, 0}
};

const struct flag_type container_flags[] = {
    {"closeable", 1, TRUE},
    {"pickproof", 2, TRUE},
    {"closed", 4, TRUE},
    {"locked", 8, TRUE},
    {"puton", 16, TRUE},
    {NULL, 0, 0}
};

/*****************************************************************************
                      ROM - specific tables:
 ****************************************************************************/




const struct flag_type ac_type[] = {
    {"pierce", AC_PIERCE, TRUE},
    {"bash", AC_BASH, TRUE},
    {"slash", AC_SLASH, TRUE},
    {"exotic", AC_EXOTIC, TRUE},
    {NULL, 0, 0}
};


const struct flag_type size_flags[] = {
    {"tiny", SIZE_TINY, TRUE},
    {"small", SIZE_SMALL, TRUE},
    {"medium", SIZE_MEDIUM, TRUE},
    {"large", SIZE_LARGE, TRUE},
    {"huge", SIZE_HUGE, TRUE},
    {"giant", SIZE_GIANT, TRUE},
    {NULL, 0, 0},
};


const struct flag_type weapon_class[] = {
    {"exotic", WEAPON_EXOTIC, TRUE},
    {"sword", WEAPON_SWORD, TRUE},
    {NULL, 0, 0}
};


const struct flag_type weapon_type2[] = {
    {"twohands", WEAPON_TWO_HANDS, TRUE},
    {NULL, 0, 0}
};

const struct flag_type res_flags[] = {
    {"summon", RES_SUMMON, TRUE},
    {"charm", RES_CHARM, TRUE},
    {"magic", RES_MAGIC, TRUE},
    {"weapon", RES_WEAPON, TRUE},
    {"bash", RES_BASH, TRUE},
    {"pierce", RES_PIERCE, TRUE},
    {"slash", RES_SLASH, TRUE},
    {"fire", RES_FIRE, TRUE},
    {"cold", RES_COLD, TRUE},
    {"lightning", RES_LIGHTNING, TRUE},
    {"acid", RES_ACID, TRUE},
    {NULL, 0, 0}
};


const struct flag_type vuln_flags[] = {
    {"summon", VULN_SUMMON, TRUE},
    {"charm", VULN_CHARM, TRUE},
    {"magic", VULN_MAGIC, TRUE},
    {"weapon", VULN_WEAPON, TRUE},
    {"bash", VULN_BASH, TRUE},
    {"pierce", VULN_PIERCE, TRUE},
    {"slash", VULN_SLASH, TRUE},
    {"fire", VULN_FIRE, TRUE},
    {"cold", VULN_COLD, TRUE},
    {"lightning", VULN_LIGHTNING, TRUE},
    {"acid", VULN_ACID, TRUE},
    {NULL, 0, 0}
};

const struct flag_type position_flags[] = {
    {"dead", POS_DEAD, FALSE},
    {"mortal", POS_MORTAL, FALSE},
    {"incap", POS_INCAP, FALSE},
    {"stunned", POS_STUNNED, FALSE},
    {"sleeping", POS_SLEEPING, TRUE},
    {"resting", POS_RESTING, TRUE},
    {"sitting", POS_SITTING, TRUE},
    {"fighting", POS_FIGHTING, FALSE},
    {"standing", POS_STANDING, TRUE},
    {NULL, 0, 0}
};

const struct flag_type portal_flags[] = {
    {"normal_exit", GATE_NORMAL_EXIT, TRUE},
    {"go_with", GATE_GOWITH, TRUE},
    {NULL, 0, 0}
};

const struct flag_type furniture_flags[] = {
    {"stand_at", STAND_AT, TRUE},
    {"stand_on", STAND_ON, TRUE},
    {"stand_in", STAND_IN, TRUE},
    {"sit_at", SIT_AT, TRUE},
    {"sit_on", SIT_ON, TRUE},
    {"sit_in", SIT_IN, TRUE},
    {"rest_at", REST_AT, TRUE},
    {"rest_on", REST_ON, TRUE},
    {"rest_in", REST_IN, TRUE},
    {"sleep_at", SLEEP_AT, TRUE},
    {"sleep_on", SLEEP_ON, TRUE},
    {"sleep_in", SLEEP_IN, TRUE},
    {NULL, 0, 0}
};

const struct flag_type apply_types[] = {
    {"affects", TO_AFFECTS, TRUE},
    {"object", TO_OBJECT, TRUE},
    {"immune", TO_IMMUNE, TRUE},
    {"resist", TO_RESIST, TRUE},
    {"vuln", TO_VULN, TRUE},
    {"weapon", TO_WEAPON, TRUE},
    {NULL, 0, TRUE}
};

const struct bit_type bitvector_type[] = {
    {affect_flags, "affect"},
    {apply_flags, "apply"},
    {imm_flags, "imm"},
    {res_flags, "res"},
    {vuln_flags, "vuln"},
    {weapon_type2, "weapon"}
};
