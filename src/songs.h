/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,    *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                     *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael      *
 *  Chastain, Michael Quan, and Mitchell Tse.                  *
 *                                     *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                           *
 *                                     *
 *  Much time and thought has gone into this software and you are      *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                          *
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
#ifndef SONGS_H
#define SONGS_H

DECLARE_DO_FUN( do_sing     );
DECLARE_DO_FUN( do_wail     );
DECLARE_DO_FUN( do_riff     );

//DECLARE_DO_FUN( do_fox      );
//DECLARE_DO_FUN( do_bear     );
//DECLARE_DO_FUN( do_cat      );


//void apply_bard_song_affect_to_group(CHAR_DATA *ch);
void check_bard_song( CHAR_DATA *ch, bool deduct_cost );
void check_bard_song_group( CHAR_DATA *ch );
//void remove_bard_song_group( CHAR_DATA *ch );
void remove_passive_bard_song( CHAR_DATA *ch );
//void add_deadly_dance_attacks(CHAR_DATA *ch, CHAR_DATA *victim, int gsn, int damtype);
//void add_deadly_dance_attacks_with_one_hit(CHAR_DATA *ch, CHAR_DATA *victim, int gsn);
int song_cost( CHAR_DATA *ch, int song );
int get_lunge_chance( CHAR_DATA *ch );
int get_instrument_skill( CHAR_DATA *ch );
bool is_song( int sn );
void song_heal( CHAR_DATA *ch );
CHAR_DATA* get_singer( CHAR_DATA *ch, int song );

#endif // SONGS_H