#ifndef ZONE_GAY_H
#define ZONE_GAY_H

#include "zone_svr.h"

//�������
int player_gay_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_gay_invite(ZONESVRENV *pstEnv, Player *pstPlayer);

//���ѽ���
int player_gay_award(ZONESVRENV* pstEnv, Player *pstPlayer, TASKRECYCLEDEF *pstTaskRecyleDef);

//�ǲ���һ��������Ե���Ч������
int is_gay_gong(ZONESVRENV* pstEnv, Player *pstPlayer);

#endif
