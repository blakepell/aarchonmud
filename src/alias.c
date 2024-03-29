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

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"

/* stuff to catch spammers --Bobble */
static CHAR_DATA *active_char = NULL;
void anti_spam_interpret( CHAR_DATA *ch, const char *argument )
{
    active_char = ch;
    interpret( ch, argument );
    active_char = NULL;
}

void punish_spam( void )
{
    if ( active_char == NULL || IS_NPC(active_char) )
	return;

    WAIT_STATE( active_char, 4 * PULSE_VIOLENCE );
}

/* does aliasing and other fun stuff */
void substitute_alias(DESCRIPTOR_DATA *d, const char *argument)
{
    CHAR_DATA *ch;
    char buf[MAX_STRING_LENGTH],prefix[MAX_INPUT_LENGTH],name[MAX_INPUT_LENGTH];
    const char *point;
    int alias;
    
    ch = d->character;
    
    /* charmed chars can't act on their own behalf */
    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master != NULL && !IS_IMMORTAL(ch) )
    {
	if ( ch->master->in_room == ch->in_room )
	{
	    char act_buf[MSL];
	    snprintf( act_buf, sizeof(act_buf), "You tell $N that you want to '%s'.", argument );
	    act( act_buf, ch, NULL, ch->master, TO_CHAR );
	    snprintf( act_buf, sizeof(act_buf), "$n looks like $e wants to '%s'.", argument );
	    act( act_buf, ch, NULL, ch->master, TO_VICT );
	    act( "$n looks at $N with adoring eyes.", ch, NULL, ch->master, TO_NOTVICT );
	}
	else
	    send_to_char( "Wait till your beloved master returns.\n\r", ch );
	WAIT_STATE( ch, PULSE_VIOLENCE );
	return;
    }

    /* check for prefix */
    if (ch->prefix[0] != '\0' && str_prefix("prefix",argument))
    {
        if (strlen(ch->prefix) + strlen(argument) > MAX_INPUT_LENGTH)
            send_to_char("Line too long, prefix not processed.\n\r",ch);
        else
        {
            snprintf( prefix, sizeof(prefix),"%s %s",ch->prefix,argument);
            argument = prefix;
        }
    }
    
    if (IS_NPC(ch) || ch->pcdata->alias[0] == NULL
        ||	!str_prefix("alias",argument) || !str_prefix("una",argument) 
        ||  !str_prefix("prefix",argument)) 
    {
        anti_spam_interpret(d->character,argument);
        return;
    }
    
    strlcpy(buf, argument, sizeof(buf));
    
    for (alias = 0; alias < MAX_ALIAS; alias++)	 /* go through the aliases */
    {
        if (ch->pcdata->alias[alias] == NULL)
            break;
        
        if (!str_prefix(ch->pcdata->alias[alias],argument))
        {
            point = one_argument(argument,name);
            if (!strcmp(ch->pcdata->alias[alias],name))
            {
                buf[0] = '\0';
                strlcat(buf,ch->pcdata->alias_sub[alias],sizeof(buf));
                if (point[0] != '\0')
                {                 
                    strlcat(buf," ",sizeof(buf));
                    strlcat(buf,point,sizeof(buf));
                }
                break;
            }
            if (strlen(buf) > MAX_INPUT_LENGTH)
            {
                send_to_char("Alias substitution too long. Truncated.\r\n",ch);
                buf[MAX_INPUT_LENGTH -1] = '\0';
            }
        }
    }

    anti_spam_interpret(d->character,buf);
}

DEF_DO_FUN(do_alia)
{
    send_to_char("I'm sorry, alias must be entered in full.\n\r",ch);
    return;
}

DEF_DO_FUN(do_alias)
{
    CHAR_DATA *rch;
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    int pos;
    
    argument = smash_tilde_cc(argument);
    
    rch = ch;
    
    if (IS_NPC(rch))
        return;
    
    argument = one_argument(argument,arg);
    
    
    if (arg[0] == '\0')
    {
        
        if (rch->pcdata->alias[0] == NULL)
        {
            send_to_char("You have no aliases defined.\n\r",ch);
            return;
        }
        send_to_char("Your current aliases are:\n\r",ch);
        
        for (pos = 0; pos < MAX_ALIAS; pos++)
        {
            if (rch->pcdata->alias[pos] == NULL
                ||	rch->pcdata->alias_sub[pos] == NULL)
                break;
            
            snprintf( buf, sizeof(buf),"    %s:  %s\n\r",rch->pcdata->alias[pos],
                rch->pcdata->alias_sub[pos]);
            send_to_char(buf,ch);
        }
        snprintf( buf, sizeof(buf),"(%d/%d aliases used)\n\r",pos,MAX_ALIAS);
            send_to_char(buf,ch);
        return;
    }
    
    if (!str_prefix("una",arg) || !str_cmp("alias",arg))
    {
        send_to_char("Sorry, that word is reserved.\n\r",ch);
        return;
    }
    
    if (argument[0] == '\0')
    {
        for (pos = 0; pos < MAX_ALIAS; pos++)
        {
            if (rch->pcdata->alias[pos] == NULL
                ||	rch->pcdata->alias_sub[pos] == NULL)
                break;
            
            if (!str_cmp(arg,rch->pcdata->alias[pos]))
            {
                snprintf( buf, sizeof(buf),"%s aliases to '%s'.\n\r",rch->pcdata->alias[pos],
                    rch->pcdata->alias_sub[pos]);
                send_to_char(buf,ch);
                return;
            }
        }
        
        send_to_char("That alias is not defined.\n\r",ch);
        return;
    }
    
    if (!str_prefix(argument,"delete") || !str_prefix(argument,"prefix"))
    {
        send_to_char("That shall not be done!\n\r",ch);
        return;
    }
    
    for (pos = 0; pos < MAX_ALIAS; pos++)
    {
        if (rch->pcdata->alias[pos] == NULL)
            break;
        
        if (!str_cmp(arg,rch->pcdata->alias[pos])) /* redefine an alias */
        {
            free_string(rch->pcdata->alias_sub[pos]);
            rch->pcdata->alias_sub[pos] = str_dup(argument);
            snprintf( buf, sizeof(buf),"%s is now realiased to '%s'.\n\r",arg,argument);
            send_to_char(buf,ch);
            return;
        }
    }
    
    if (pos >= MAX_ALIAS)
    {
        send_to_char("Sorry, you have reached the alias limit.\n\r",ch);
        return;
    }
    
    /* make a new alias */
    rch->pcdata->alias[pos]		= str_dup(arg);
    rch->pcdata->alias_sub[pos]	= str_dup(argument);
    snprintf( buf, sizeof(buf),"%s is now aliased to '%s'.\n\r",arg,argument);
    send_to_char(buf,ch);
}


DEF_DO_FUN(do_unalias)
{
    CHAR_DATA *rch;
    char arg[MAX_INPUT_LENGTH];
    int pos;
    bool found = FALSE;
    
    rch = ch;
    
    if (IS_NPC(rch))
        return;
    
    argument = one_argument(argument,arg);
    
    if (arg[0] == '\0')
    {
        send_to_char("Unalias what?\n\r",ch);
        return;
    }
    
    for (pos = 0; pos < MAX_ALIAS; pos++)
    {
        if (rch->pcdata->alias[pos] == NULL)
            break;
        
        if (found)
        {
            rch->pcdata->alias[pos-1]		= rch->pcdata->alias[pos];
            rch->pcdata->alias_sub[pos-1]	= rch->pcdata->alias_sub[pos];
            rch->pcdata->alias[pos]		= NULL;
            rch->pcdata->alias_sub[pos]		= NULL;
            continue;
        }
        
        if(!strcmp(arg,rch->pcdata->alias[pos]))
        {
            send_to_char("Alias removed.\n\r",ch);
            free_string(rch->pcdata->alias[pos]);
            free_string(rch->pcdata->alias_sub[pos]);
            rch->pcdata->alias[pos] = NULL;
            rch->pcdata->alias_sub[pos] = NULL;
            found = TRUE;
        }
    }
    
    if (!found)
        send_to_char("No alias of that name to remove.\n\r",ch);
}
