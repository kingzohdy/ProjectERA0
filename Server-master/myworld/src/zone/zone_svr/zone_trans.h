
#ifndef ZONE_TRANS_H
#define ZONE_TRANS_H

#include "zone_svr.h"

int player_trans_select(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_trans_openlist(ZONESVRENV* pstEnv, Player *pstPlayer, Npc *pstNpc);

int player_trans_select_check(ZONESVRENV* pstEnv, Player *pstPlayer,CSTRANSSELECT *pstTransSelect , 
									int *piNeedMoney, RESPOS *pstPos, int *piMapID, int iTip);
int player_trans_check(ZONESVRENV* pstEnv, Player *pstPlayer, int iToMap, int iTip);

#endif /* ZONE_TRANS_H */
