#ifndef ZONE_STALL_H
#define ZONE_STALL_H

#include "zone_svr.h"

int player_stall(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int stall_msg_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, int iCmd);
int player_stall_clear( ZONESVRENV* pstEnv, Player *pstPlayer );
int z_active_player_stall(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int item_is_up(Player *pstPlayer, tdr_ulonglong ullWID);
int pet_is_up(Player *pstPlayer, tdr_ulonglong ullWID);
int stall_action(ZONESVRENV* pstEnv, Player *pstPlayer, int iFlag);
#endif