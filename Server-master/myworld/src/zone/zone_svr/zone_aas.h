#ifndef ZONE_AAS_H
#define ZONE_AAS_H

#include "zone_svr.h"

//֪ͨ�ͻ�����ݲ���ɹ�
int player_add_id_card_succ(ZONESVRENV* pstEnv, Player *pstPlayer);

//�ͻ��˵������֤����
int player_aas_id_card_notify(ZONESVRENV* pstEnv, Player *pstPlayer);

//��¼�����Լ��
int player_acc_login_aas_check(ZONESVRENV* pstEnv, Player *pstPlayer, char *pstBirthDay);

int aas_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
			TFRAMEHEAD	 *pstFrameHead, CSPKG *pstCsPkg);

#endif /* ZONE_AAS_H */
