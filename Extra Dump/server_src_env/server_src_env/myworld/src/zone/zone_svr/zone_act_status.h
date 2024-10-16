
#ifndef ZONE_ACT_STATUS_H
#define ZONE_ACT_STATUS_H

#include "zone_svr.h"

#define SET_PLAYER_NOMOVE(pstPlayer) ((pstPlayer)->stOnline.cMoving = 0, (pstPlayer)->stOnline.State &= ( ~CS_STAT_WALK))
#define SET_PLAYER_MOVE(pstPlayer) ((pstPlayer)->stOnline.cMoving = 1, (pstPlayer)->stOnline.State |= CS_STAT_WALK)

int z_player_act_test(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, int iAction);
int z_player_act_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, int iAction);
int z_act_status_init(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv);

#endif /* ZONE_ACT_STATUS_H */
