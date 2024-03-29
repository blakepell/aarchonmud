/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 ***************************************************************************/
 
/***************************************************************************
*	ROM 2.4 is copyright 1993-1996 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@efn.org)				   *
*	    Gabrielle Taylor						   *
*	    Brian Moore (zump@rom.org)					   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Rom24/doc/rom.license			   *
***************************************************************************/
#ifndef LOOKUP_H
#define LOOKUP_H

int   clan_lookup	args( (const char *name) );
int   clan_rank_lookup args ( (sh_int clan, const char *name) );
int   position_lookup	args( (const char *name) );
int   sex_lookup	args( (const char *name) );
int   size_lookup	args( (const char *name) );
int   flag_lookup	args( (const char *, const struct flag_type *) );
int   penalty_table_lookup args( (const char *name) );
int   stance_lookup args( (const char *name) );
const char *name_lookup args( (const int bit, const struct flag_type *) );
int index_lookup( const int bit, const struct flag_type *flag_table );
int subclass_lookup (const char *name);


#endif