#ifndef ZONE_SHANGJIN_H
#define ZONE_SHANGJIN_H

#include "zone_svr.h"

int sj_task_pre(ZONESVRENV *pstEnv);

SJTASKDEF* z_sj_task_def(ZONESVRENV *pstEnv, int iID);

int sj_task_trace_init(Player *pstPlayer, TASKRUNONE *pstTaskRunOne, TASKDEF *pstTaskDef);

int player_shangjin_task(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int player_sj_item_info(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_sj_item_clear(ZONESVRENV* pstEnv, Player *pstPlayer);
#endif /* ZONE_SHANGJIN_H */

