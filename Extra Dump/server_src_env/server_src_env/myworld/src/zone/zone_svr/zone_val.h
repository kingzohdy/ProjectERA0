#ifndef ZONE_VAL_H
#define ZONE_VAL_H

#include "zone_svr.h"

int val_fight(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSVALFIGHTCLT *pstFightClt);

int val_watch(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSVALWATCHCLT *pstCSWatch);

int val_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
			TFRAMEHEAD	 *pstFrameHead, CSPKG *pstCsPkg);

#endif /* ZONE_VAL_H */

