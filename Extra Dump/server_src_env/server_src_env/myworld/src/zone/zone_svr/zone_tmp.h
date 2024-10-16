
#ifndef ZONE_TMP_H
#define ZONE_TMP_H

#include "zone_svr.h"

#if defined(ZONE_ID_TRACKED)
int z_item_refresh_tracked(ZONESVRENV *pstEnv, DropItem *pstDropItem,
                           int iEnergeFlag, const char *pszFile, int iLine);

#define z_item_refresh(env, item, energe) \
	z_item_refresh_tracked(env, item, energe, __FILE__, __LINE__)
#else
int z_item_refresh(ZONESVRENV* pstEnv, DropItem *pstDropItem,int iEnergeFlag);
#endif

int tmp_item_create(ZONESVRENV* pstEnv,  Player *pstPlayer,int iItemID, ZONEIDX *pstMapIdx, RESPOS *pstPos, int iEnerge,int iRefresh, int iStay, int iDir,int iSkill_1_collectnum,int iSkill_2_collectnum,DropItem **pstItem);
//int z_item_refresh(ZONESVRENV* pstEnv, DropItem *pstDropItem,int iEnergeFlag);
int tmp_item_check(ZONESVRENV* pstEnv, Player *pstPlayer, int iOldMap);
int tmp_item_del(ZONESVRENV* pstEnv, Player *pstPlayer);
int tmp_item_add(ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstDropItem, ITEMDEF *pstItem);
int tmp_fill(ZONESVRENV* pstEnv , Player *pstPlayer, ZoneSkill *pstZoneSkill, Player *pstPlayerOther);
//DropFlag 1表换地图强制放下来
int tmp_item_drop(ZONESVRENV* pstEnv, Player *pstPlayer,int iDropFlag);
int tmp_tmpenerge_max_action(ZONESVRENV* pstEnv,Player *pstPlayer,int iTmpEnergeMax);
int tmp_apply_item(ZONESVRENV* pstEnv,Player *pstPlayer, int iItemID, int iStay);
int is_tmp_machine(ZONESVRENV* pstEnv,Player *pstPlayer);
#endif /* ZONE_TMP_H */
