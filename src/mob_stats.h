#ifndef MOB_STATS_H
#define MOB_STATS_H

int level_base_hp( int level );
int mob_base_hp( MOB_INDEX_DATA *pMobIndex, int level );
int mob_base_mana( MOB_INDEX_DATA *pMobIndex, int level );
int mob_base_move( MOB_INDEX_DATA *pMobIndex, int level );
int mob_base_hitroll( MOB_INDEX_DATA *pMobIndex, int level );
int level_base_damroll( int level );
int mob_base_damroll( MOB_INDEX_DATA *pMobIndex, int level );
int level_base_damage( int level );
int mob_base_damage( MOB_INDEX_DATA *pMobIndex, int level );
int mob_base_ac( MOB_INDEX_DATA *pMobIndex, int level );
int mob_base_saves( MOB_INDEX_DATA *pMobIndex, int level );
long level_base_wealth( int level );
long mob_base_wealth( MOB_INDEX_DATA *pMobIndex );
int level_base_attacks( int level );
int mob_base_attacks( MOB_INDEX_DATA *pMobIndex, int level );


#endif // MOB_STATS_H