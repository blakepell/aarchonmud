/***************************************************************************
*  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
*  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
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
*   ROM 2.4 is copyright 1993-1996 Russ Taylor             *
*   ROM has been brought to you by the ROM consortium          *
*       Russ Taylor (rtaylor@efn.org)                  *
*       Gabrielle Taylor                           *
*       Brian Moore (zump@rom.org)                     *
*   By using this code, you have agreed to follow the terms of the     *
*   ROM license, in the file Rom24/doc/rom.license             *
***************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

static const char *const distance[4]=
{
    "right here.", "nearby to the %s.", "not far %s.", "off in the distance %s."
};

static void scan_list           args((ROOM_INDEX_DATA *scan_room, CHAR_DATA *ch,
                              sh_int depth, sh_int door));
static void scan_char           args((CHAR_DATA *victim, CHAR_DATA *ch,
                              sh_int depth, sh_int door));

void do_stare(CHAR_DATA *ch)
{
    ROOM_INDEX_DATA *scan_room;
    EXIT_DATA *pExit;
    sh_int door, depth;
    int chance;
    
    
    if (IS_TAG(ch))
    {
        send_to_char("You cannot stare while playing Freeze Tag.\n\r",ch);
        return;
    }
    
    if ((chance = get_skill(ch, gsn_thousand_yard_stare)) != 0)
    {
        if (number_percent () < chance)
        {
            check_improve(ch, gsn_thousand_yard_stare, TRUE, 4);
            act("$n stares off into the distance.", ch, NULL, NULL, TO_ROOM);
            send_to_char("Staring into the distance, you see:\n\r", ch);
            scan_room = ch->in_room;
            scan_list(ch->in_room, ch, 0, -1);
            for (door=0;door<MAX_DIR;door++)
            {
                scan_room = ch->in_room;
                for (depth = 1; depth < 4; depth++)
                {
                    if ((pExit = scan_room->exit[door]) != NULL)
                    {
                        scan_room = pExit->u1.to_room;
                        scan_list(pExit->u1.to_room, ch, depth, door);
                    }
                }
            }
            return;
        } else 
        {
            send_to_char("You can't see into the distance.\n\r", ch);
            check_improve(ch, gsn_thousand_yard_stare, FALSE, 4);
            return;
        }  
    } else
				{
        send_to_char("You don't have the 1000 yard stare.\n\r", ch);
        return;
				}
    return;
}

DEF_DO_FUN(do_scan)
{
    char arg1[MAX_INPUT_LENGTH], buf[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *scan_room;
    EXIT_DATA *pExit;
    sh_int door, depth;

    
    if (IS_TAG(ch))
    {
        send_to_char("You cannot scan while playing Freeze Tag.\n\r",ch);
        return;
    }

    argument = one_argument(argument, arg1);
    
    if (arg1[0] == '\0')
    {
        act("$n looks all around.", ch, NULL, NULL, TO_ROOM);
        send_to_char("Looking around you see:\n\r", ch);
        scan_list(ch->in_room, ch, 0, -1);
        
        for (door=0;door<MAX_DIR;door++)
        {
            if ((pExit = ch->in_room->exit[door]) != NULL
                    && !IS_SET(pExit->exit_info, EX_DORMANT) )
                scan_list(pExit->u1.to_room, ch, 1, door);
        }
        return;
    }
    else if (!str_cmp(arg1, "n") || !str_cmp(arg1, "north")) door = DIR_NORTH;
    else if (!str_cmp(arg1, "e") || !str_cmp(arg1, "east"))  door = DIR_EAST;
    else if (!str_cmp(arg1, "s") || !str_cmp(arg1, "south")) door = DIR_SOUTH;
    else if (!str_cmp(arg1, "w") || !str_cmp(arg1, "west"))  door = DIR_WEST;
    else if (!str_cmp(arg1, "u") || !str_cmp(arg1, "up" ))   door = DIR_UP;
    else if (!str_cmp(arg1, "d") || !str_cmp(arg1, "down"))  door = DIR_DOWN;
    else if (!str_cmp(arg1, "ne") || !str_cmp(arg1, "northeast")) door=DIR_NORTHEAST;
    else if (!str_cmp(arg1, "se") || !str_cmp(arg1, "southeast")) door=DIR_SOUTHEAST;
    else if (!str_cmp(arg1, "sw") || !str_cmp(arg1, "southwest")) door=DIR_SOUTHWEST;
    else if (!str_cmp(arg1, "nw") || !str_cmp(arg1, "northwest")) door=DIR_NORTHWEST;
    else { send_to_char("Which way do you want to scan?\n\r", ch); return; }
    
    act("You peer intently $T.", ch, NULL, dir_name[door], TO_CHAR);
    act("$n peers intently $T.", ch, NULL, dir_name[door], TO_ROOM);
    snprintf( buf, sizeof(buf), "Looking %s you see:\n\r", dir_name[door]);
    
    scan_room = ch->in_room;
    
    for (depth = 1; depth < 4; depth++)
    {
        pExit=scan_room->exit[door];
        if (!pExit || IS_SET(pExit->exit_info, EX_DORMANT) )
            break;
        scan_room = pExit->u1.to_room;
        scan_list(pExit->u1.to_room, ch, depth, door);
    }
    return;
}

static void scan_list(ROOM_INDEX_DATA *scan_room, CHAR_DATA *ch, sh_int depth,
               sh_int door)
{
    CHAR_DATA *rch;
    
    if (scan_room == NULL) return;
    for (rch=scan_room->people; rch != NULL; rch=rch->next_in_room)
    {
        if (rch == ch) 
            continue;
        if (!IS_NPC(rch) && rch->invis_level > get_trust(ch)) 
            continue;
        if (check_see(ch, rch)) 
            scan_char(rch, ch, depth, door);
    }
    return;
}

static void scan_char(CHAR_DATA *victim, CHAR_DATA *ch, sh_int depth, sh_int door)
{
    char buf[MAX_INPUT_LENGTH], buf2[MAX_INPUT_LENGTH];
    
    buf[0] = '\0';
    
    strlcat(buf, PERS(victim, ch), sizeof(buf));
    strlcat(buf, ", ", sizeof(buf));
    snprintf( buf2, sizeof(buf2), distance[depth], dir_name[door]);
    strlcat(buf, buf2, sizeof(buf));
    strlcat(buf, "\n\r", sizeof(buf));
    
    send_to_char(buf, ch);
    return;
}
