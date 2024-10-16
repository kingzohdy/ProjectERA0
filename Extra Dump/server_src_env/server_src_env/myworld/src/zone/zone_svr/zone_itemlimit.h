#ifndef ZONE_ITEMLIMIT_H
#define ZONE_ITEMLIMIT_H

#include "zone_svr.h"


#define WORLD_LIMIT_RATE	1000000


int is_fini_global(ZONESVRENV * pstEnv);
	
//限制物品产出记录下来
int world_item_limit_add(ZONESVRENV* pstEnv, int iItemID, int iNum);

//物品产出限制检查
int world_item_limit_check(ZONESVRENV* pstEnv, int iItemID);



//掉落这个组的时候要处理的事情(命中、未命中概率都需要执行)
int world_booty_group_limit_add(ZONESVRENV* pstEnv, int iGroupID, int fSucc);

//获得限制组的修正值
int world_booty_group_limit_val_get(ZONESVRENV* pstEnv, int iGroupID);


//怪物死亡记录
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

//tick 更新
int world_limit_tick(ZONESVRENV* pstEnv);

#endif	/*ZONE_ITEMLIMIT_H */