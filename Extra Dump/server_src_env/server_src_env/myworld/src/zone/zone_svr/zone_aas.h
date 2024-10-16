#ifndef ZONE_AAS_H
#define ZONE_AAS_H

#include "zone_svr.h"

//通知客户端身份补填成功
int player_add_id_card_succ(ZONESVRENV* pstEnv, Player *pstPlayer);

//客户端弹出身份证补填
int player_aas_id_card_notify(ZONESVRENV* pstEnv, Player *pstPlayer);

//登录防沉迷检查
int player_acc_login_aas_check(ZONESVRENV* pstEnv, Player *pstPlayer, char *pstBirthDay);

int aas_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
			TFRAMEHEAD	 *pstFrameHead, CSPKG *pstCsPkg);

#endif /* ZONE_AAS_H */
