#ifndef ZONE_EXCHG_H
#define ZONE_EXCHG_H

#include "zone_svr.h"


int player_exchg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int exchg_logout(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_exchg_cancel_in(ZONESVRENV* pstEnv, Player *pstPlayer,Player *pstOther);
#endif









