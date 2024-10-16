#ifndef ZONE_BACK_H
#define ZONE_BACK_H

#include "zone_svr.h"

int player_back_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

#endif /* ZONE_BACK_H */