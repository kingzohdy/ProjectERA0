#ifndef ZONE_BOOTY_H
#define ZONE_BOOTY_H

#include "zone_svr.h"

int mon_booty_first_atker(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, Player *pstAtker);

int mon_booty_first_action(ZONESVRENV* pstEnv, Monster *pstMon);

int is_mon_booty_owner(Monster *pstMon, Player *pstPlayer);

int mon_booty_valid_player(Monster *pstMon, Player *pstPlayer);

int mon_booty_check_task(ZONESVRENV* pstEnv, MONBOOTYITEM *pstBootyItem,  Player *pstPlayer, int fCollect);

int mon_booty_group_assign(ZONESVRENV* pstEnv, Monster *pstMon, ADDMONBOOTYGROUP *pstMonBooty, ZoneTeam *pstTeam);

int mon_booty_assign(ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef, Player *pstKiller);

void mon_die_add_xp(ZONESVRENV* pstEnv, Monster* pstMon, MONSTERDEF *pstMonDef, ZoneAni *pstAtker);

int mon_booty_owner_tick(ZONESVRENV* pstEnv, Monster *pstMon);

int mon_target_action(ZONESVRENV* pstEnv, Monster *pstMon);

int is_booty_grid_owner(Player *pstPlayer, DropItem *pstDropItem);

int dropitem_get_stay_time(ZONESVRENV* pstEnv, DropItem *pstDropItem);

int dropitem_get_pick_limit_time(ZONESVRENV* pstEnv, DropItem *pstDropItem);

int mon_booty_assign_resource(ZONESVRENV* pstEnv, unsigned int uiExp, Player *pstPlayer);

int check_team_booty_roll(ZONESVRENV* pstEnv, Player *pstPlayer,ZoneTeam *pstZoneTeam);

int mon_die_team_add_buff(ZONESVRENV* pstEnv, Monster *pstMon,int iBuffID, int iSec);

#endif /* ZONE_BOOTY_H */
