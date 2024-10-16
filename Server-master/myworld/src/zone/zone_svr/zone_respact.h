#ifndef ZONE_RESPACT_H
#define ZONE_RESPACT_H

const RESPACTPARTYDEF* z_respact_party_def(ZONESVRENV *pstEnv, unsigned int nID);

const RESPACTPHASEDEF* z_respact_phase_def(ZONESVRENV *pstEnv,
                                           unsigned int nPhase);

// ��������ֱֵ�ӵõ���ǰ�������׶ζ���
const RESPACTPHASEDEF* z_respact_phase_refer_value(ZONESVRENV *pstEnv,
                                                   unsigned int nRespactValue);

// ���ҽ�ɫ��������Ϣ
RESPACTENTRY* player_find_respact(ZONESVRENV *pstEnv, Player *pstPlayer,
                                  unsigned int nID);

// ��¼ʱ���͸��ͻ��˵�������Ϣ����
int player_respact_info(ZONESVRENV *pstEnv, Player *pstPlayer);

// ��ɫ�������仯��֪ͨ
int player_respact_chg(ZONESVRENV *pstEnv, Player *pstPlayer, unsigned int nID);

// ��ɫ����������
int player_respact_inc(ZONESVRENV *pstEnv, Player *pstPlayer, unsigned int nID,
                       unsigned int nValue, int fInCombat);

// ����ֵ���
int player_respact_value_check(ZONESVRENV *pstEnv, Player *pstPlayer,
                               const RESPACTENTRY *pstEntry);

// ɾ��һ����ɫ������Ϣ
int player_respact_del(ZONESVRENV *pstEnv, Player *pstPlayer, unsigned int nID);

#endif // ZONE_RESPACT_H
