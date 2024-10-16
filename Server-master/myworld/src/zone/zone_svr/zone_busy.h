#ifndef ZONE_BUSY_H
#define ZONE_BUSY_H

#include "zone_svr.h"

int player_busy_info_action(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_busy_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
			TFRAMEHEAD	 *pstFrameHead, CSPKG *pstCsPkg);

int player_busy_ui(ZONESVRENV* pstEnv, Player *pstPlayer, int iID, int iNpcID);

int player_busy_day_update(ZONESVRENV* pstEnv, Player *pstPlayer);

#endif /* ZONE_BUSY_H */
