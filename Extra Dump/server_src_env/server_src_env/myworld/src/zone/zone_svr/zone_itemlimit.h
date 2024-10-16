#ifndef ZONE_ITEMLIMIT_H
#define ZONE_ITEMLIMIT_H

#include "zone_svr.h"


#define WORLD_LIMIT_RATE	1000000


int is_fini_global(ZONESVRENV * pstEnv);
	
//������Ʒ������¼����
int world_item_limit_add(ZONESVRENV* pstEnv, int iItemID, int iNum);

//��Ʒ�������Ƽ��
int world_item_limit_check(ZONESVRENV* pstEnv, int iItemID);



//����������ʱ��Ҫ���������(���С�δ���и��ʶ���Ҫִ��)
int world_booty_group_limit_add(ZONESVRENV* pstEnv, int iGroupID, int fSucc);

//��������������ֵ
int world_booty_group_limit_val_get(ZONESVRENV* pstEnv, int iGroupID);


//����������¼
int world_mon_event_die(ZONESVRENV* pstEnv, Monster *pstMon);

int world_baoxiang_add(ZONESVRENV* pstEnv, int iID);

int global_booty_group_limit_update(ZONESVRENV* pstEnv);

int global_item_limit_update(ZONESVRENV* pstEnv);

int global_mon_event_update(ZONESVRENV* pstEnv);

int global_world_limit_update(ZONESVRENV* pstEnv);

int global_baoxiang_update(ZONESVRENV* pstEnv);

int world_cardbox_add(ZONESVRENV* pstEnv, int iID, int iVal);

int world_cardbox_check(ZONESVRENV* pstEnv, int iID);

int world_cardbox_clear(ZONESVRENV* pstEnv, int iID);

int global_cardbox_update(ZONESVRENV* pstEnv);

//tick ����
int world_limit_tick(ZONESVRENV* pstEnv);

#endif	/*ZONE_ITEMLIMIT_H */