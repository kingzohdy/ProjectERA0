#ifndef ZONE_GAY_H
#define ZONE_GAY_H

#include "zone_svr.h"

//基友配对
int player_gay_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_gay_invite(ZONESVRENV *pstEnv, Player *pstPlayer);

//基友奖励
int player_gay_award(ZONESVRENV* pstEnv, Player *pstPlayer, TASKRECYCLEDEF *pstTaskRecyleDef);

//是不是一个基友配对的有效发起者
int is_gay_gong(ZONESVRENV* pstEnv, Player *pstPlayer);

#endif
