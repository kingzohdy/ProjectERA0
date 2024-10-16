#ifndef ZONE_LOSTTOTEM_H
#define ZONE_LOSTTOTEM_H

#include "zone_svr.h"

ROLEITEM *player_find_item_by_totem(ZONESVRENV* pstEnv,Player *pstPlayer);
Monster* player_find_lost_totem(ZONESVRENV* pstEnv,Player *pstPlayer);
int lostmap_mon_die(ZONESVRENV* pstEnv,Player *pstPlayer, MONSTERDEF *pstMonDef, Monster *pstMon);
int losttotem_use(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEITEM *pstRoleItem, ITEMDEF *pstItemDef);
int losttotem_use_check(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEITEM *pstRoleItem, ITEMDEF *pstItemDef);
int losttotem_free(ZONESVRENV* pstEnv,Player *pstPlayer);
int losttotem_player_login_check(ZONESVRENV *pstEnv, Player *pstPlayer);
int player_is_in_lostmap(Player *pstPlayer);
int losttotem_pkg_move_limit( ZONESVRENV *pstEnv, Player *pstPlayer, int iSrcList, int iDstList, ROLEITEM *pstRoleItem);
int player_losttotem_active_notify(ZONESVRENV* pstEnv, Player* pstPlayer, int iActive, tdr_ulonglong ullWid);
int losttotem_leave_map(ZONESVRENV* pstEnv, Monster *pstMon);
#endif	