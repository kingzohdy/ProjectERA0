#ifndef ZONE_BAOXIANG_H
#define ZONE_BAOXIANG_H

#include "zone_svr.h"

BAOXIANGDEF* z_find_baoxiang_def(ZONESVRENV *pstEnv, int iID);

CARDBOXDEF* z_find_cardbox_def(ZONESVRENV *pstEnv, int iID);

int player_baoxiang_open(ZONESVRENV* pstEnv, Player *pstPlayer, CSBAOXIANGOPEN *pstReq);

int player_cardbox_close(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_cardbox_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

#endif	/*ZONE_BAOXIANG_H */

