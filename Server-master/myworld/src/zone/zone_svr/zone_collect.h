
#ifndef ZONE_COLLECT_H
#define ZONE_COLLECT_H

#include "zone_svr.h"


int player_collect(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_collect_break(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMonCollectee);

int player_collect_award(ZONESVRENV* pstEnv, Player *pstPlayer);

int map_refresh_ore(ZONESVRENV* pstEnv, MapIndex* pstMapIndex, ZONEMAPINST* pstMapInst);

int map_inst_ore_del(ZONEMAPINST* pstMapInst, Monster *pstMon);

int player_map_ore_info(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_map_bigore_info(ZONESVRENV* pstEnv, Player *pstPlayer);

int world_big_ore_info_broadcast(ZONESVRENV *pstEnv);

int player_collect_end(ZONESVRENV* pstEnv, Player *pstPlayer);

int  map_ore_if_fix_mon_rand(ZONESVRENV* pstEnv,MapIndex* pstMapIndex,int iMonID);




#endif	/*ZONE_COLLECT_H */
