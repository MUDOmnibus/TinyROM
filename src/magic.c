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
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"
#include "magic.h"
#include "recycle.h"

/*
 * Local functions.
 */
void say_spell args ((CHAR_DATA * ch, int sn));

/* imported functions */
bool remove_obj args ((CHAR_DATA * ch, int iWear, bool fReplace));
void wear_obj args ((CHAR_DATA * ch, OBJ_DATA * obj, bool fReplace));



/*
 * Lookup a skill by name.
 */
int skill_lookup (const char *name)
{
    int sn;

    for (sn = 0; sn < MAX_SKILL; sn++)
    {
        if (skill_table[sn].name == NULL)
            break;
        if (LOWER (name[0]) == LOWER (skill_table[sn].name[0])
            && !str_prefix (name, skill_table[sn].name))
            return sn;
    }

    return -1;
}

int find_spell (CHAR_DATA * ch, const char *name)
{
    /* finds a spell the character can cast if possible */
    int sn, found = -1;

    if (IS_NPC (ch))
        return skill_lookup (name);

    for (sn = 0; sn < MAX_SKILL; sn++)
    {
        if (skill_table[sn].name == NULL)
            break;
        if (LOWER (name[0]) == LOWER (skill_table[sn].name[0])
            && !str_prefix (name, skill_table[sn].name))
        {
            if (found == -1)
                found = sn;
            if (ch->level >= skill_table[sn].skill_level[ch->class]
                && ch->pcdata->learned[sn] > 0)
                return sn;
        }
    }
    return found;
}



/*
 * Lookup a skill by slot number.
 * Used for object loading.
 */
int slot_lookup (int slot)
{
    extern bool fBootDb;
    int sn;

    if (slot <= 0)
        return -1;

    for (sn = 0; sn < MAX_SKILL; sn++)
    {
        if (slot == skill_table[sn].slot)
            return sn;
    }

    if (fBootDb)
    {
        bug ("Slot_lookup: bad slot %d.", slot);
        abort ();
    }

    return -1;
}



/*
 * Utter mystical words for an sn.
 */
void say_spell (CHAR_DATA * ch, int sn)
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    CHAR_DATA *rch;
    char *pName;
    int iSyl;
    int length;

    struct syl_type {
        char *old;
        char *new;
    };

    static const struct syl_type syl_table[] = {
        {" ", " "},
        {"ar", "abra"},
        {"au", "kada"},
        {"bless", "fido"},
        {"blind", "nose"},
        {"bur", "mosa"},
        {"cu", "judi"},
        {"de", "oculo"},
        {"en", "unso"},
        {"light", "dies"},
        {"lo", "hi"},
        {"mor", "zak"},
        {"move", "sido"},
        {"ness", "lacri"},
        {"ning", "illa"},
        {"per", "duda"},
        {"ra", "gru"},
        {"fresh", "ima"},
        {"re", "candus"},
        {"son", "sabru"},
        {"tect", "infra"},
        {"tri", "cula"},
        {"ven", "nofo"},
        {"a", "a"}, {"b", "b"}, {"c", "q"}, {"d", "e"},
        {"e", "z"}, {"f", "y"}, {"g", "o"}, {"h", "p"},
        {"i", "u"}, {"j", "y"}, {"k", "t"}, {"l", "r"},
        {"m", "w"}, {"n", "i"}, {"o", "a"}, {"p", "s"},
        {"q", "d"}, {"r", "f"}, {"s", "g"}, {"t", "h"},
        {"u", "j"}, {"v", "z"}, {"w", "x"}, {"x", "n"},
        {"y", "l"}, {"z", "k"},
        {"", ""}
    };

    buf[0] = '\0';
    for (pName = skill_table[sn].name; *pName != '\0'; pName += length)
    {
        for (iSyl = 0; (length = strlen (syl_table[iSyl].old)) != 0; iSyl++)
        {
            if (!str_prefix (syl_table[iSyl].old, pName))
            {
                strcat (buf, syl_table[iSyl].new);
                break;
            }
        }

        if (length == 0)
            length = 1;
    }

    sprintf (buf2, "$n utters the words, '%s'.", buf);
    sprintf (buf, "$n utters the words, '%s'.", skill_table[sn].name);

    for (rch = ch->in_room->people; rch; rch = rch->next_in_room)
    {
        if (rch != ch)
            act ((!IS_NPC (rch) && ch->class == rch->class) ? buf : buf2,
                 ch, NULL, rch, TO_VICT);
    }

    return;
}



/*
 * Compute a saving throw.
 * Negative apply's make saving throw better.
 */
bool saves_spell (int level, CHAR_DATA * victim, int dam_type)
{
    int save;

    save = 50 + (victim->level - level) * 5 - victim->saving_throw * 2;

    switch (check_immune (victim, dam_type))
    {
        case IS_IMMUNE:
            return TRUE;
        case IS_RESISTANT:
            save += 2;
            break;
        case IS_VULNERABLE:
            save -= 2;
            break;
    }

    if (!IS_NPC (victim) && class_table[victim->class].fMana)
        save = 9 * save / 10;
    save = URANGE (5, save, 95);
    return number_percent () < save;
}

/* RT save for dispels */

bool saves_dispel (int dis_level, int spell_level, int duration)
{
    int save;

    if (duration == -1)
        spell_level += 5;
    /* very hard to dispel permanent effects */

    save = 50 + (spell_level - dis_level) * 5;
    save = URANGE (5, save, 95);
    return number_percent () < save;
}

/* co-routine for dispel magic and cancellation */

bool check_dispel (int dis_level, CHAR_DATA * victim, int sn)
{
    AFFECT_DATA *af;

    if (is_affected (victim, sn))
    {
        for (af = victim->affected; af != NULL; af = af->next)
        {
            if (af->type == sn)
            {
                if (!saves_dispel (dis_level, af->level, af->duration))
                {
                    affect_strip (victim, sn);
                    if (skill_table[sn].msg_off)
                    {
                        send_to_char (skill_table[sn].msg_off, victim);
                        send_to_char ("\n\r", victim);
                    }
                    return TRUE;
                }
                else
                    af->level--;
            }
        }
    }
    return FALSE;
}

/* for finding mana costs -- temporary version */
int mana_cost (CHAR_DATA * ch, int min_mana, int level)
{
    if (ch->level + 2 == level)
        return 1000;
    return UMAX (min_mana, (100 / (2 + ch->level - level)));
}



/*
 * The kludgy global is for spells who want more stuff from command line.
 */
char *target_name;

void do_cast (CHAR_DATA * ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    void *vo;
    int mana;
    int sn;
    int target;

    /*
     * Switched NPC's can cast spells, but others can't.
     */
    if (IS_NPC (ch) && ch->desc == NULL)
        return;

    target_name = one_argument (argument, arg1);
    one_argument (target_name, arg2);

    if (arg1[0] == '\0')
    {
        send_to_char ("Cast which what where?\n\r", ch);
        return;
    }

    if ((sn = find_spell (ch, arg1)) < 1
        || skill_table[sn].spell_fun == spell_null || (!IS_NPC (ch)
                                                       && (ch->level <
                                                           skill_table
                                                           [sn].skill_level
                                                           [ch->class]
                                                           || ch->
                                                           pcdata->learned[sn]
                                                           == 0)))
    {
        send_to_char ("You don't know any spells of that name.\n\r", ch);
        return;
    }

    if (ch->position < skill_table[sn].minimum_position)
    {
        send_to_char ("You can't concentrate enough.\n\r", ch);
        return;
    }

    if (ch->level + 2 == skill_table[sn].skill_level[ch->class])
        mana = 50;
    else
        mana = UMAX (skill_table[sn].min_mana,
                     100 / (2 + ch->level -
                            skill_table[sn].skill_level[ch->class]));

    /*
     * Locate targets.
     */
    victim = NULL;
    obj = NULL;
    vo = NULL;
    target = TARGET_NONE;

    switch (skill_table[sn].target)
    {
        default:
            bug ("Do_cast: bad target for sn %d.", sn);
            return;

        case TAR_IGNORE:
            break;

        case TAR_CHAR_OFFENSIVE:
            if (arg2[0] == '\0')
            {
                if ((victim = ch->fighting) == NULL)
                {
                    send_to_char ("Cast the spell on whom?\n\r", ch);
                    return;
                }
            }
            else
            {
                if ((victim = get_char_room (ch, target_name)) == NULL)
                {
                    send_to_char ("They aren't here.\n\r", ch);
                    return;
                }
            }
/*
        if ( ch == victim )
        {
            send_to_char( "You can't do that to yourself.\n\r", ch );
            return;
        }
*/


            if (!IS_NPC (ch))
            {

                if (is_safe (ch, victim) && victim != ch)
                {
                    send_to_char ("Not on that target.\n\r", ch);
                    return;
                }
                check_killer (ch, victim);
            }

            if (IS_AFFECTED (ch, AFF_CHARM) && ch->master == victim)
            {
                send_to_char ("You can't do that on your own follower.\n\r",
                              ch);
                return;
            }

            vo = (void *) victim;
            target = TARGET_CHAR;
            break;

        case TAR_CHAR_DEFENSIVE:
            if (arg2[0] == '\0')
            {
                victim = ch;
            }
            else
            {
                if ((victim = get_char_room (ch, target_name)) == NULL)
                {
                    send_to_char ("They aren't here.\n\r", ch);
                    return;
                }
            }

            vo = (void *) victim;
            target = TARGET_CHAR;
            break;

        case TAR_CHAR_SELF:
            if (arg2[0] != '\0' && !is_name (target_name, ch->name))
            {
                send_to_char ("You cannot cast this spell on another.\n\r",
                              ch);
                return;
            }

            vo = (void *) ch;
            target = TARGET_CHAR;
            break;

        case TAR_OBJ_INV:
            if (arg2[0] == '\0')
            {
                send_to_char ("What should the spell be cast upon?\n\r", ch);
                return;
            }

            if ((obj = get_obj_carry (ch, target_name, ch)) == NULL)
            {
                send_to_char ("You are not carrying that.\n\r", ch);
                return;
            }

            vo = (void *) obj;
            target = TARGET_OBJ;
            break;

        case TAR_OBJ_CHAR_OFF:
            if (arg2[0] == '\0')
            {
                if ((victim = ch->fighting) == NULL)
                {
                    send_to_char ("Cast the spell on whom or what?\n\r", ch);
                    return;
                }

                target = TARGET_CHAR;
            }
            else if ((victim = get_char_room (ch, target_name)) != NULL)
            {
                target = TARGET_CHAR;
            }

            if (target == TARGET_CHAR)
            {                    /* check the sanity of the attack */
                if (is_safe_spell (ch, victim, FALSE) && victim != ch)
                {
                    send_to_char ("Not on that target.\n\r", ch);
                    return;
                }

                if (IS_AFFECTED (ch, AFF_CHARM) && ch->master == victim)
                {
                    send_to_char
                        ("You can't do that on your own follower.\n\r", ch);
                    return;
                }

                if (!IS_NPC (ch))
                    check_killer (ch, victim);

                vo = (void *) victim;
            }
            else if ((obj = get_obj_here (ch, target_name)) != NULL)
            {
                vo = (void *) obj;
                target = TARGET_OBJ;
            }
            else
            {
                send_to_char ("You don't see that here.\n\r", ch);
                return;
            }
            break;

        case TAR_OBJ_CHAR_DEF:
            if (arg2[0] == '\0')
            {
                vo = (void *) ch;
                target = TARGET_CHAR;
            }
            else if ((victim = get_char_room (ch, target_name)) != NULL)
            {
                vo = (void *) victim;
                target = TARGET_CHAR;
            }
            else if ((obj = get_obj_carry (ch, target_name, ch)) != NULL)
            {
                vo = (void *) obj;
                target = TARGET_OBJ;
            }
            else
            {
                send_to_char ("You don't see that here.\n\r", ch);
                return;
            }
            break;
    }

    if (!IS_NPC (ch) && ch->mana < mana)
    {
        send_to_char ("You don't have enough mana.\n\r", ch);
        return;
    }

    if (str_cmp (skill_table[sn].name, "ventriloquate"))
        say_spell (ch, sn);

    WAIT_STATE (ch, skill_table[sn].beats);

    if (number_percent () > get_skill (ch, sn))
    {
        send_to_char ("You lost your concentration.\n\r", ch);
        check_improve (ch, sn, FALSE, 1);
        ch->mana -= mana / 2;
    }
    else
    {
        ch->mana -= mana;
        if (IS_NPC (ch) || class_table[ch->class].fMana)
            /* class has spells */
            (*skill_table[sn].spell_fun) (sn, ch->level, ch, vo, target);
        else
            (*skill_table[sn].spell_fun) (sn, 3 * ch->level / 4, ch, vo, target);
        check_improve (ch, sn, TRUE, 1);
    }

    if ((skill_table[sn].target == TAR_CHAR_OFFENSIVE
         || (skill_table[sn].target == TAR_OBJ_CHAR_OFF
             && target == TARGET_CHAR)) && victim != ch
        && victim->master != ch)
    {
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;

        for (vch = ch->in_room->people; vch; vch = vch_next)
        {
            vch_next = vch->next_in_room;
            if (victim == vch && victim->fighting == NULL)
            {
                check_killer (victim, ch);
                multi_hit (victim, ch, TYPE_UNDEFINED);
                break;
            }
        }
    }

    return;
}



/*
 * Cast spells at targets using a magical object.
 */
void obj_cast_spell (int sn, int level, CHAR_DATA * ch, CHAR_DATA * victim,
                     OBJ_DATA * obj)
{
    void *vo;
    int target = TARGET_NONE;

    if (sn <= 0)
        return;

    if (sn >= MAX_SKILL || skill_table[sn].spell_fun == 0)
    {
        bug ("Obj_cast_spell: bad sn %d.", sn);
        return;
    }

    switch (skill_table[sn].target)
    {
        default:
            bug ("Obj_cast_spell: bad target for sn %d.", sn);
            return;

        case TAR_IGNORE:
            vo = NULL;
            break;

        case TAR_CHAR_OFFENSIVE:
            if (victim == NULL)
                victim = ch->fighting;
            if (victim == NULL)
            {
                send_to_char ("You can't do that.\n\r", ch);
                return;
            }
            if (is_safe (ch, victim) && ch != victim)
            {
                send_to_char ("Something isn't right...\n\r", ch);
                return;
            }
            vo = (void *) victim;
            target = TARGET_CHAR;
            break;

        case TAR_CHAR_DEFENSIVE:
        case TAR_CHAR_SELF:
            if (victim == NULL)
                victim = ch;
            vo = (void *) victim;
            target = TARGET_CHAR;
            break;

        case TAR_OBJ_INV:
            if (obj == NULL)
            {
                send_to_char ("You can't do that.\n\r", ch);
                return;
            }
            vo = (void *) obj;
            target = TARGET_OBJ;
            break;

        case TAR_OBJ_CHAR_OFF:
            if (victim == NULL && obj == NULL)
            {
                if (ch->fighting != NULL)
                    victim = ch->fighting;
                else
                {
                    send_to_char ("You can't do that.\n\r", ch);
                    return;
                }
            }

            if (victim != NULL)
            {
                if (is_safe_spell (ch, victim, FALSE) && ch != victim)
                {
                    send_to_char ("Somehting isn't right...\n\r", ch);
                    return;
                }

                vo = (void *) victim;
                target = TARGET_CHAR;
            }
            else
            {
                vo = (void *) obj;
                target = TARGET_OBJ;
            }
            break;


        case TAR_OBJ_CHAR_DEF:
            if (victim == NULL && obj == NULL)
            {
                vo = (void *) ch;
                target = TARGET_CHAR;
            }
            else if (victim != NULL)
            {
                vo = (void *) victim;
                target = TARGET_CHAR;
            }
            else
            {
                vo = (void *) obj;
                target = TARGET_OBJ;
            }

            break;
    }

    target_name = "";
    (*skill_table[sn].spell_fun) (sn, level, ch, vo, target);



    if ((skill_table[sn].target == TAR_CHAR_OFFENSIVE
         || (skill_table[sn].target == TAR_OBJ_CHAR_OFF
             && target == TARGET_CHAR)) && victim != ch
        && victim->master != ch)
    {
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;

        for (vch = ch->in_room->people; vch; vch = vch_next)
        {
            vch_next = vch->next_in_room;
            if (victim == vch && victim->fighting == NULL)
            {
                check_killer (victim, ch);
                multi_hit (victim, ch, TYPE_UNDEFINED);
                break;
            }
        }
    }

    return;
}



/*
 * Spell functions.
 */
void spell_acid_blast (int sn, int level, CHAR_DATA * ch, void *vo,
                       int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice (level, 12);
    if (saves_spell (level, victim, DAM_ACID))
        dam /= 2;
    damage (ch, victim, dam, sn, DAM_ACID, TRUE);
    return;
}


void spell_null (int sn, int level, CHAR_DATA * ch, void *vo, int target)
{
    send_to_char ("That's not a spell!\n\r", ch);
    return;
}

