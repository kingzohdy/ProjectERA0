#ifndef ZONE_BOSS_H
#define ZONE_BOSS_H

#include "zone_svr.h"

int boss_data_init(ZONESVRENV* pstEnv);
int boss_fresh_tick(ZONESVRENV* pstEnv);
int boss_die(ZONESVRENV* pstEnv,Monster* pstMon, MONSTERDEF *pstMonDef);
int boss_info_req(ZONESVRENV* pstEnv, Player *pstPlayer, CSWORLDBOSSINFOREQ *pstReq);
int if_span_pvp_map(ZONESVRENV *pstEnv,int MapID);
int span_map_world_boss_data_sync(ZONESVRENV *pstEnv,SSWORLDBOSSDATASYNC *pstWorldBossData);
int span_map_boss_data_send(ZONESVRENV *pstEnv,int MapID,WORLDBOSSONE *pstOne);
WORLDBOSSONE *boss_one_find(WORLDBOSS *pstWorldBoss, int iMonID);

#endif 
