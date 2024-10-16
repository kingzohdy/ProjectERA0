/*
**  @file $RCSfile: zone_levfeng.c,v $
**  general description of this module
**  $Id: zone_levfeng.c,v 1.8 2013/10/30 03:23:59 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2013/10/30 03:23:59 $
**  @version $Revision: 1.8 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_levfeng.h"
#include "zone_clt.h"

int gs_iFindLevFeng = 0;

LEVELFENG *level_feng_def_find(ZONESVRENV* pstEnv, int iLevel)
{
	LEVELFENG stDef;
	int iEqu, iIdx;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Level = iLevel;

	iIdx = bsearch_int(&stDef, pstObj->sLevelFengDef, pstObj->iLevelFengDef, LEVEL_FENG_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (LEVELFENG *)&pstObj->sLevelFengDef[iIdx];
	}
	
	return NULL;
}

int level_feng_notify(ZONESVRENV *pstEnv, int iChg)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;
	GLOBAL *pstGlobal = &pstEnv->pstGlobalObj->stGlobal;

	//全服广播
	pstAction->ID = 0;
	pstAction->ActionID = ACTION_LEVEL_FENG;
	pstAction->Data.LevelFeng.LevelFeng = pstGlobal->LevFeng;

	if (pstGlobal->LevFeng.LevelFeng > 0)
	{
		pstAction->Data.LevelFeng.LevelFengChg = iChg;
	}
	else
	{
		pstAction->Data.LevelFeng.LevelFengChg = 0;
	}

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);

	return 0;
}

int level_feng_levelup(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	LEVELFENG *pstLevelFeng;
	GLOBAL *pstGlobal = &pstEnv->pstGlobalObj->stGlobal;
	
	if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat != GLOBAL_FINI ||
		pstEnv->pstConf->NoLevelFeng)
	{
		return -1;
	}

	if (pstGlobal->LevFeng.LevelFeng <= 0)
	{
		return 0;
	}

	if (pstPlayer->stRoleData.Level >= pstGlobal->LevFeng.LevelFeng)
	{
		pstLevelFeng = level_feng_def_find(pstEnv, pstGlobal->LevFeng.LevelFeng);
		if (NULL == pstLevelFeng)
		{
			return -1;
		}

		if (pstGlobal->Misc.NewWorldFlags.FirstStartupTime + pstLevelFeng->LevelFengMin*3600 < pstGlobal->LevFeng.LevelFengEnd)
		{
			pstGlobal->LevFeng.LevelFengEnd -= pstLevelFeng->LevelFengTimeDec;
			if (pstGlobal->LevFeng.LevelFengEnd < pstGlobal->Misc.NewWorldFlags.FirstStartupTime + pstLevelFeng->LevelFengMin*3600)
			{
				pstGlobal->LevFeng.LevelFengEnd = pstGlobal->Misc.NewWorldFlags.FirstStartupTime + pstLevelFeng->LevelFengMin*3600;
			}

			if (pstEnv->pstAppCtx->stCurr.tv_sec <= pstGlobal->LevFeng.LevelFengEnd)
			{
				level_feng_notify(pstEnv, 0);
			}
			else
			{
				level_feng_update(pstEnv);
			}
		}
	}
	
	return 0;
}

int level_feng_find_next(ZONESVRENV *pstEnv)
{
	int i;
	LEVELFENG *pstLevelFeng;
	
	GLOBAL *pstGlobal = &pstEnv->pstGlobalObj->stGlobal;
	
	if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat != GLOBAL_FINI)
	{
		return -1;
	}

	for (i=0; i<pstEnv->pstZoneObj->iLevelFengDef; i++)
	{
		pstLevelFeng = (LEVELFENG *)&pstEnv->pstZoneObj->sLevelFengDef[i];
		if (pstLevelFeng->Level <= pstGlobal->LevFeng.LevelFeng || pstLevelFeng->Level > pstEnv->pstConf->MaxLevel)
		{
			continue;
		}

		if (pstEnv->pstAppCtx->stCurr.tv_sec < pstGlobal->Misc.NewWorldFlags.FirstStartupTime + pstLevelFeng->LevelFengMax*3600)	
		{
			break;
		}
	}

	if (i >= pstEnv->pstZoneObj->iLevelFengDef)
	{
		pstGlobal->LevFeng.LevelFeng = 0;
		pstGlobal->LevFeng.LevelFengEnd = 0;
		gs_iFindLevFeng = 1;
	}
	else
	{
		pstGlobal->LevFeng.LevelFeng = pstLevelFeng->Level;
		pstGlobal->LevFeng.LevelFengEnd = pstGlobal->Misc.NewWorldFlags.FirstStartupTime + pstLevelFeng->LevelFengMax*3600;
		pstEnv->pstGlobalObj->stGlobalObjInfo.iDirty = 2;
	}
	
	return 0;
}

// 获取当前封印等级
int level_get_curr_fy(ZONESVRENV *pstEnv)
{
	GLOBAL *pstGlobal = &pstEnv->pstGlobalObj->stGlobal;

	if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat != GLOBAL_FINI || pstEnv->pstConf->NoLevelFeng)
	{
		return -1;
	}

	if (pstGlobal->LevFeng.LevelFeng > 0)
	{
		return pstGlobal->LevFeng.LevelFeng;
	}
	else
	{
		return pstEnv->pstConf->MaxLevel;
	}
}

//与时间变化相关的update
int level_feng_update(ZONESVRENV *pstEnv)
{
	int iOldLevFeng;
		
	GLOBAL *pstGlobal = &pstEnv->pstGlobalObj->stGlobal;
	
	if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat != GLOBAL_FINI || pstEnv->pstConf->NoLevelFeng)
	{
		return -1;
	}

	if (gs_iFindLevFeng)
	{
		return 0;
	}

	if (pstEnv->pstAppCtx->stCurr.tv_sec <= pstGlobal->LevFeng.LevelFengEnd)
	{
		return 0;
	}

	iOldLevFeng = pstGlobal->LevFeng.LevelFeng;

	if (0 > level_feng_find_next(pstEnv))
	{
		return -1;
	}

	if (iOldLevFeng > 0 && iOldLevFeng < pstGlobal->LevFeng.LevelFeng)
	{
		pstGlobal->Misc.LevFengBefore = iOldLevFeng;
	}

	level_feng_notify(pstEnv, 1);
	
	return 0;
}

int level_feng_clear(ZONESVRENV *pstEnv)
{
	GLOBAL *pstGlobal = &pstEnv->pstGlobalObj->stGlobal;
	
	if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat != GLOBAL_FINI)
	{
		return -1;
	}

	pstGlobal->LevFeng.LevelFeng = 0;
	pstGlobal->LevFeng.LevelFengEnd = 0;
	gs_iFindLevFeng = 0;
	pstGlobal->Misc.LevFengBefore = 0;

	level_feng_update(pstEnv);

	return 0;
}

int level_feng_get_expdec(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i;
	int iExpCount;
	LEVELUP *pstLevelUp;
	LEVELFENG *pstLevFeng;
	GLOBAL *pstGlobal = &pstEnv->pstGlobalObj->stGlobal;
	
	if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat != GLOBAL_FINI || pstEnv->pstConf->NoLevelFeng)
	{
		return 100;
	}

	if (pstPlayer->stRoleData.Level > pstGlobal->LevFeng.LevelFeng)
	{
		return 0;
	}

	if (pstGlobal->LevFeng.LevelFeng <= 0 || pstPlayer->stRoleData.Level != pstGlobal->LevFeng.LevelFeng)
	{
		return 100;
	}

	pstLevFeng = level_feng_def_find(pstEnv, pstGlobal->LevFeng.LevelFeng);
	if (NULL == pstLevFeng)
	{
		return 100;
	}

	pstLevelUp = z_get_levelup(pstEnv, pstGlobal->LevFeng.LevelFeng);
	if (NULL == pstLevelUp || 0 == pstLevelUp->LevelNextExp)
	{
		return 100;
	}

	iExpCount = pstPlayer->stRoleData.RoleDetail.Exp/pstLevelUp->LevelNextExp + 1;
	if (pstPlayer->stRoleData.RoleDetail.Exp == (long long)pstLevelUp->LevelNextExp*(iExpCount-1))
	{
		iExpCount = iExpCount-1;
	}

	for (i=0; i<MAX_LEV_FENG_EXPDEC; i++)
	{
		if (0 == pstLevFeng->ExpRateDec[i].ExpCount)
		{
			break;
		}

		if (pstLevFeng->ExpRateDec[i].ExpCount == iExpCount)
		{
			return pstLevFeng->ExpRateDec[i].ExpRate;
		}
	}

	if (i>= MAX_LEV_FENG_EXPDEC)
	{
		if (pstLevFeng->ExpRateDec[MAX_LEV_FENG_EXPDEC-1].ExpCount < iExpCount)
		{
			return pstLevFeng->ExpRateDec[MAX_LEV_FENG_EXPDEC-1].ExpRate;
		}
	}
	else if (i > 0 && pstLevFeng->ExpRateDec[i-1].ExpCount < iExpCount)
	{
		return pstLevFeng->ExpRateDec[i-1].ExpRate;
	}
	
	return 100;
}

