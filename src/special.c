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
#include <time.h>
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

/*
 * The following special functions are available for mobiles.
 */

DECLARE_SPEC_FUN (spec_janitor);


/* the function table */
const struct spec_type spec_table[] = {
    {"spec_janitor", spec_janitor},
    {NULL, NULL}
};

/*
 * Given a name, return the appropriate spec fun.
 */
SPEC_FUN *spec_lookup (const char *name)
{
    int i;

    for (i = 0; spec_table[i].name != NULL; i++)
    {
        if (LOWER (name[0]) == LOWER (spec_table[i].name[0])
            && !str_prefix (name, spec_table[i].name))
            return spec_table[i].function;
    }

    return 0;
}

char *spec_name (SPEC_FUN * function)
{
    int i;

    for (i = 0; spec_table[i].function != NULL; i++)
    {
        if (function == spec_table[i].function)
            return spec_table[i].name;
    }

    return NULL;
}


bool spec_janitor (CHAR_DATA * ch)
{
    return FALSE;
}

