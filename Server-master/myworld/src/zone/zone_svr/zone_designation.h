#ifndef ZONE_DESIGNATION_H
#define ZONE_DESIGNATION_H


#define ADD_DESIG_NUM_LIMIT -1
#define ADD_DESIG_EXIST -2
#define ADD_DESIG_NO_DEF -3

int g_iDesigVar;

#define GET_DESIG_CHG_CAREER 1 			// תְ��ȡ�ƺ�
#define GET_DESIG_MON_KILL 2 				//������ɱ��N��
#define GET_DESIG_KILL_DUDUNIAO 3 			// ��ɱ5000ֻ����
#define GET_DESIF_ENTER_JAIN_NUM 4 		//�������100��
#define GET_DESIG_USE_FOOD 5 				// ����ʳ��5000��
#define GET_DESIG_FRIEND_NUM 6 			// ���������ﵽ60
#define GET_DESIG_CUIDI_PAOYOU 7 			// �ʹ޵ٴ���100��

#define GET_DESIG_EXP_99999	9			// exp���5��������99999
#define GET_DESIG_RIDE_STREN 10			// ǿ������ﵽ50��
#define GET_DESIG_WATCH 11					// �鿴/���鿴100��


// ��ѯ�ƺŶ���
const DESIGNATIONDEF* z_designation_def(ZONESVRENV* pstEnv, unsigned int iID);

// ���ͽ�ɫ���гƺ��б�
int player_designation_info(ZONESVRENV* pstEnv, Player *pstPlayer);

// �ƺ������Ұ�㲥��Ϣ
int player_designation_action(ZONESVRENV* pstEnv, Player *pstPlayer);

// ���ӽ�ɫ�ĳƺ�
int player_designation_add(ZONESVRENV* pstEnv, Player *pstPlayer,
                           unsigned int iID);

// ���ý�ɫ�ĳƺ�
int player_designation_set(ZONESVRENV* pstEnv, Player *pstPlayer,
                           unsigned int iID);

// ���ҽ�ɫ���еĳƺ�
DESIGNATIONENTRY* player_find_designation(ZONESVRENV* pstEnv, Player *pstPlayer,
                                          unsigned int iID);

// ɾ��һ����ɫ���еĳƺ�
int player_designation_del(ZONESVRENV* pstEnv, Player *pstPlayer,
                           unsigned int iID);

// ����ͻ��˷��͹����Ĺ��ڽ�ɫ�ƺŵĲ�����
int player_designation(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                       TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_designation_add_in(ZONESVRENV* pstEnv, Player *pstPlayer,
                           unsigned int iID);


int player_des_del_by_time(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_desig_get(ZONESVRENV* pstEnv, Player *pstPlayer, int iType);


#endif //  ZONE_DESIGNATION_H
