#ifndef ZONE_CLANBOON_H
#define ZONE_CLANBOON_H

#include "zone_svr.h"

int clan_boon_strong_chg_update(ZONESVRENV *pstEnv,int iMapID, ZoneClan *pstZoneClan, int iType);
int clan_boon_all_notify(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, Player *pstPlayer);
int clan_boon_assign_chg(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan);
int clan_boon_condition_chg(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan);
int clan_boon_get_online(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan);
int clan_boon_clear(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan);
int clan_boon_player_login(ZONESVRENV *pstEnv, Player *pstPlayer);
int clan_boon_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
#endif
