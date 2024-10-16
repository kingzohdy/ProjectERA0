/*
**  @file $RCSfile: zone_itemlimit.c,v $
**  general description of this module
**  $Id: zone_itemlimit.c,v 1.12 2013/07/08 03:22:50 eric Exp $
**  @author $Author: eric $
**  @date $Date: 2013/07/08 03:22:50 $
**  @version $Revision: 1.12 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_itemlimit.h"
#include "zone_script.h"
#include "zone_baoxiang.h"

extern ScriptEnv stSEnv;

WORLDITEMLIMITDEF *z_find_item_limit_def(ZONESVRENV* pstEnv, int iItemID)
{
	WORLDITEMLIMITDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.ItemID = iItemID;
	iIndex = bsearch_int(&stDef, pstObj->sWorldItemLimitDef, pstObj->iWorldItemLimitDef, WORLD_ITEM_LIMIT_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (WORLDITEMLIMITDEF *)&pstObj->sWorldItemLimitDef[iIndex];
	}
	return NULL;
}

WORLDBOOTYGROUPLIMITDEF *z_find_booty_group_limit_def(ZONESVRENV* pstEnv, int iGroupID)
{
	WORLDBOOTYGROUPLIMITDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.GroupID = iGroupID;
	iIndex = bsearch_int(&stDef, pstObj->sWorldBootyGroupLimitDef, pstObj->iWorldBootyGroupLimitDef, WORLD_BOOTY_GROUP_LIMIT_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (WORLDBOOTYGROUPLIMITDEF *)&pstObj->sWorldBootyGroupLimitDef[iIndex];
	}
	return NULL;
}

MONEVENTDEF *z_find_mon_event_def(ZONESVRENV* pstEnv, int iID)
{
	MONEVENTDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.ID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sMonEventDef, pstObj->iMonEventDef, MON_EVENT_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (MONEVENTDEF *)&pstObj->sMonEventDef[iIndex];
	}
	return NULL;
}


int is_fini_global(ZONESVRENV* pstEnv)
{
	if(pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat == GLOBAL_STAT_FINI)
	{
		return 1;
	}

	return 0;
}

void world_item_limit_cycletime_update(ZONESVRENV* pstEnv, WORLDITEMLIMIT* pstItemLimit)
{
	int iEndTime = pstItemLimit->StartTime + pstItemLimit->CycleTime*3600;

	if(pstItemLimit->CycleTime <= 0)
	{
		return ;
	}
	
	if(iEndTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		pstItemLimit->Num = 0;
		pstItemLimit->StartTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	return ;
}

//获取物品限制信息( 已更新有效期限)
WORLDITEMLIMIT*  world_item_limit_get(ZONESVRENV* pstEnv, int iItemID)
{
	int i;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;
	WORLDITEMLIMIT *pstItemLimit;

	if(!is_fini_global(pstEnv))
	{
		return NULL;
	}

	for(i=0; i<pstWorldLimit->ItemNum; i++)
	{
		pstItemLimit = &pstWorldLimit->ItemLimitInfo[i];
		if(pstItemLimit ->ItemID == iItemID)
		{
			//顺便更新下周期
			world_item_limit_cycletime_update(pstEnv, pstItemLimit);
		
			return pstItemLimit;
		}
	}

	return NULL;
}

int world_item_limit_insert(ZONESVRENV* pstEnv, WORLDITEMLIMITDEF *pstItemLimitDef)
{
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;
	WORLDITEMLIMIT *pstItemLimit;

	if(!is_fini_global(pstEnv))
	{
		return -1;
	}

	if(world_item_limit_get(pstEnv, pstItemLimitDef->ItemID))
	{
		return -1;
	}

	if(pstWorldLimit->ItemNum >= MAX_WORLD_ITEM_LIMIT)
	{
		return -1;
	}

	pstItemLimit = &pstWorldLimit->ItemLimitInfo[pstWorldLimit->ItemNum++];

	memset(pstItemLimit, 0, sizeof(*pstItemLimit));
	pstItemLimit->ItemID = pstItemLimitDef->ItemID;
	pstItemLimit->MaxNum = pstItemLimitDef->Num;
	pstItemLimit->CycleTime = pstItemLimitDef->CycleTime;
	pstItemLimit->StartTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	return 0;
}

int world_item_limit_update(ZONESVRENV* pstEnv, WORLDITEMLIMITDEF *pstItemLimitDef)
{
	WORLDITEMLIMIT *pstItemLimit;

	if(!is_fini_global(pstEnv))
	{
		return -1;
	}

	pstItemLimit = world_item_limit_get(pstEnv, pstItemLimitDef->ItemID);
	//没有就插入
	if(NULL == pstItemLimit)
	{
		return world_item_limit_insert(pstEnv, pstItemLimitDef);
	}
	
	pstItemLimit->ItemID = pstItemLimitDef->ItemID;
	pstItemLimit->MaxNum = pstItemLimitDef->Num;
	pstItemLimit->CycleTime = pstItemLimitDef->CycleTime;

	//顺便更新下周期
	world_item_limit_cycletime_update(pstEnv, pstItemLimit);
	
	return 0;
}

int world_item_limit_add(ZONESVRENV* pstEnv, int iItemID, int iNum)
{
	WORLDITEMLIMIT *pstItemLimit;
	
	pstItemLimit = world_item_limit_get(pstEnv, iItemID);
	if(NULL == pstItemLimit)
	{
		return -1;
	}

	//一直加吧。调整了MAXNUM也能限制住
	pstItemLimit->Num += iNum;
	if(pstItemLimit->Num >= pstItemLimit->MaxNum)
	{
		char szBuff[128];
			
		snprintf(szBuff, CCH(szBuff), "WorldItemLimit ItemID=%d, Num=%d", 
				iItemID, pstItemLimit->Num);
		z_game_notify(pstEnv, szBuff, CCH(szBuff));
	}

	return 0;
}

int world_item_limit_check(ZONESVRENV* pstEnv, int iItemID)
{
	WORLDITEMLIMIT *pstItemLimit;
	
	pstItemLimit = world_item_limit_get(pstEnv, iItemID);
	if(NULL == pstItemLimit)
	{
		return 0;
	}

	if(pstItemLimit->Num >= pstItemLimit->MaxNum)
	{
		char szBuff[128];
			
		snprintf(szBuff, CCH(szBuff), "WorldItemLimit check ItemID=%d, Num=%d", 
				iItemID, pstItemLimit->Num);
		z_game_notify(pstEnv, szBuff, CCH(szBuff));
		
		return -1;
	}

	return 0;
}

void world_booty_group_limit_cycletime_update(ZONESVRENV* pstEnv, WORLDBOOTYGROUPLIMIT* pstGroupLimit)
{
	int iEndTime = pstGroupLimit->StartTime + pstGroupLimit->CycleTime*3600;

	if(pstGroupLimit->CycleTime <= 0)
	{
		return ;
	}

	if(iEndTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		pstGroupLimit->Val = 0;
		pstGroupLimit->StartTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	return ;
}

//获取怪物战利品组限制信息( 已更新有效期限)
WORLDBOOTYGROUPLIMIT*  world_booty_group_limit_get(ZONESVRENV* pstEnv, int iGroupID)
{
	int i;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;
	WORLDBOOTYGROUPLIMIT *pstBootyGroupLimit;

	if(!is_fini_global(pstEnv))
	{
		return NULL;
	}

	for(i=0; i<pstWorldLimit->GroupNum; i++)
	{
		pstBootyGroupLimit = &pstWorldLimit->GroupLimitInfo[i];
		if(pstBootyGroupLimit ->GroupID == iGroupID)
		{
			//顺便更新下周期
			world_booty_group_limit_cycletime_update(pstEnv, pstBootyGroupLimit);
		
			return pstBootyGroupLimit;
		}
	}

	return NULL;
}

int world_booty_group_limit_insert(ZONESVRENV* pstEnv, WORLDBOOTYGROUPLIMITDEF *pstBootyGroupLimitDef)
{
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;
	WORLDBOOTYGROUPLIMIT *pstBootyGroupLimit;

	if(!is_fini_global(pstEnv))
	{
		return -1;
	}

	if(world_booty_group_limit_get(pstEnv, pstBootyGroupLimitDef->GroupID))
	{
		return -1;
	}

	if(pstWorldLimit->GroupNum >= MAX_WORLD_BOOTY_GROUP_LIMIT)
	{
		return -1;
	}

	pstBootyGroupLimit = &pstWorldLimit->GroupLimitInfo[pstWorldLimit->GroupNum++];

	memset(pstBootyGroupLimit, 0, sizeof(*pstBootyGroupLimit));
	pstBootyGroupLimit->GroupID = pstBootyGroupLimitDef->GroupID;
	pstBootyGroupLimit->Step = pstBootyGroupLimitDef->Step;
	pstBootyGroupLimit->CycleTime = pstBootyGroupLimitDef->CycleTime;
	pstBootyGroupLimit->StartTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	return 0;
}

int world_booty_group_limit_update(ZONESVRENV* pstEnv, WORLDBOOTYGROUPLIMITDEF *pstBootyGroupLimitDef)
{
	WORLDBOOTYGROUPLIMIT *pstBootyGroupLimit;

	if(!is_fini_global(pstEnv))
	{
		return -1;
	}

	pstBootyGroupLimit = world_booty_group_limit_get(pstEnv, pstBootyGroupLimitDef->GroupID);
	//没有就插入
	if(NULL == pstBootyGroupLimit)
	{
		return world_booty_group_limit_insert(pstEnv, pstBootyGroupLimitDef);
	}
	
	pstBootyGroupLimit->GroupID = pstBootyGroupLimitDef->GroupID;
	pstBootyGroupLimit->Step = pstBootyGroupLimitDef->Step;
	pstBootyGroupLimit->CycleTime = pstBootyGroupLimitDef->CycleTime;

	//顺便更新下周期
	world_booty_group_limit_cycletime_update(pstEnv, pstBootyGroupLimit);
	
	return 0;
}

//掉落这个组的时候要处理的事情(命中、未命中概率都需要执行)
int world_booty_group_limit_add(ZONESVRENV* pstEnv, int iGroupID, int fSucc)
{
	WORLDBOOTYGROUPLIMIT *pstBootyGroupLimit;
	
	pstBootyGroupLimit = world_booty_group_limit_get(pstEnv, iGroupID);
	if(NULL == pstBootyGroupLimit)
	{
		return -1;
	}

	//成功掉落这个组
	if(fSucc)
	{
		//递增的掉了就归零
		if(pstBootyGroupLimit->Step > 0)
		{
			
			pstBootyGroupLimit->Val = 0;
		}
		//递减的掉落了就要减概率
		else
		{
			pstBootyGroupLimit->Val += pstBootyGroupLimit->Step;
			if(pstBootyGroupLimit->Val < -WORLD_LIMIT_RATE)
			{
				pstBootyGroupLimit->Val = -WORLD_LIMIT_RATE;
			}
		}
	}
	//未命中这个战利品组
	else
	{
		//递增的未命中需要增加概率
		if(pstBootyGroupLimit->Step > 0)
		{
			pstBootyGroupLimit->Val += pstBootyGroupLimit->Step;
			if(pstBootyGroupLimit->Val > WORLD_LIMIT_RATE)
			{
				pstBootyGroupLimit->Val = pstBootyGroupLimit->Val;
			}
		}
	}

	return 0;
}

//获得限制组的修正值
int world_booty_group_limit_val_get(ZONESVRENV* pstEnv, int iGroupID)
{
	WORLDBOOTYGROUPLIMIT *pstBootyGroupLimit;
	
	pstBootyGroupLimit = world_booty_group_limit_get(pstEnv, iGroupID);
	if(NULL == pstBootyGroupLimit)
	{
		return 0;
	}

	return pstBootyGroupLimit->Val;
}

WORLDMONEVENT*  world_mon_event_get(ZONESVRENV* pstEnv, int iID)
{
	int i;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;
	WORLDMONEVENT *pstMonEventInfo;

	if(!is_fini_global(pstEnv))
	{
		return NULL;
	}

	for(i=0; i<pstWorldLimit->MonEventNum; i++)
	{
		pstMonEventInfo = &pstWorldLimit->MonEventInfo[i];
		if(pstMonEventInfo ->ID == iID)
		{
			return pstMonEventInfo;
		}
	}

	return NULL;
}

int world_mon_event_insert(ZONESVRENV* pstEnv, MONEVENTDEF *pstMonEventDef)
{
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;
	WORLDMONEVENT *pstMonEventInfo;

	if(!is_fini_global(pstEnv))
	{
		return -1;
	}

	if(world_mon_event_get(pstEnv, pstMonEventDef->ID))
	{
		return -1;
	}

	if(pstWorldLimit->MonEventNum >= MAX_WORLD_MON_EVENT)
	{
		return -1;
	}

	pstMonEventInfo = &pstWorldLimit->MonEventInfo[pstWorldLimit->MonEventNum++];

	memset(pstMonEventInfo, 0, sizeof(*pstMonEventInfo));
	pstMonEventInfo->ID = pstMonEventDef->ID;
	pstMonEventInfo->MaxNum = pstMonEventDef->Num;
	
	memcpy(&pstMonEventInfo->MonID[0], &pstMonEventDef->MonID[0], 
			MAX_EVENT_MON_ID*sizeof(pstMonEventDef->MonID[0]));
	
	STRNCPY(pstMonEventInfo->EventScriptName, pstMonEventDef->EventScriptName, 
			CCH(pstMonEventInfo->EventScriptName));

	return 0;
}

int world_mon_event_update(ZONESVRENV* pstEnv, MONEVENTDEF *pstMonEventDef)
{
	WORLDMONEVENT *pstMonEventInfo;

	if(!is_fini_global(pstEnv))
	{
		return -1;
	}

	pstMonEventInfo = world_mon_event_get(pstEnv, pstMonEventDef->ID);
	//没有就插入
	if(NULL == pstMonEventInfo)
	{
		return world_mon_event_insert(pstEnv, pstMonEventDef);
	}
	
	pstMonEventInfo->ID = pstMonEventDef->ID;
	pstMonEventInfo->MaxNum = pstMonEventDef->Num;
	memcpy(&pstMonEventInfo->MonID[0], &pstMonEventDef->MonID[0], 
			MAX_EVENT_MON_ID*sizeof(pstMonEventDef->MonID[0]));
	
	STRNCPY(pstMonEventInfo->EventScriptName, pstMonEventDef->EventScriptName, 
			CCH(pstMonEventInfo->EventScriptName));
	
	return 0;
}

int is_event_mon(WORLDMONEVENT *pstMonEvenInfo, Monster *pstMon)
{
	int i;

	for(i=0; i<MAX_EVENT_MON_ID; i++)
	{
		if( pstMonEvenInfo->MonID[i] == pstMon->iDefIdx)
		{
			return 1;
		}
	}

	return 0;
}

int mon_event_exec_script(ZONESVRENV* pstEnv, WORLDMONEVENT *pstMonEventInfo, Monster *pstMon, int iProc)
{
	ScriptIndex *pstScriptInd;

	if(pstMonEventInfo->EventScriptName[0] == 0)
	{
		return -1;
	}
			
	pstScriptInd = get_script_ind(pstMonEventInfo->EventScriptName);
	if (!pstScriptInd)
	{
		return -1;
	}

	stSEnv.pstMonster = pstMon;
	stSEnv.pstNpc = NULL;
	stSEnv.pstPlayer = NULL;
	stSEnv.pstPworldInst = NULL;
	stSEnv.pstKiller = NULL;
	stSEnv.pstMapinst = NULL;
				
	z_exec_script(pstEnv->pstAppCtx, pstEnv, NULL, pstMonEventInfo->EventScriptName, pstScriptInd, iProc);

	return 0;
}

int world_mon_event_die(ZONESVRENV* pstEnv, Monster *pstMon)
{
	int i;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;
	WORLDMONEVENT *pstMonEventInfo;

	for(i=pstWorldLimit->MonEventNum-1; i>=0; i--)
	{
		pstMonEventInfo = &pstWorldLimit->MonEventInfo[i];

		if(!is_event_mon(pstMonEventInfo, pstMon))
		{
			continue;
		}

		//死亡数量达到要求执行脚本
		pstMonEventInfo->Num++;
		if(pstMonEventInfo->Num >= pstMonEventInfo->MaxNum)
		{
			pstMonEventInfo->Num = 0;
			mon_event_exec_script(pstEnv, pstMonEventInfo, pstMon, WORLD_MON_EVENT_SCRIPT_PROC_DIE);
		}
	}
	
	return 0;
}

WORLDBAOXIANG*  world_baoxiang_get(ZONESVRENV* pstEnv, int iID)
{
	int i;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;
	WORLDBAOXIANG *pstBaoXiangInfo;

	if(!is_fini_global(pstEnv))
	{
		return NULL;
	}

	for(i=0; i<pstWorldLimit->BaoXiangNum; i++)
	{
		pstBaoXiangInfo = &pstWorldLimit->BaoXiangLimit[i];
		if(pstBaoXiangInfo ->ID == iID)
		{
			return pstBaoXiangInfo;
		}
	}

	return NULL;
}

int world_baoxiang_insert(ZONESVRENV* pstEnv, BAOXIANGDEF *pstBaoXiangDef)
{
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;
	WORLDBAOXIANG *pstBaoXiangInfo;

	if(!is_fini_global(pstEnv))
	{
		return -1;
	}

	if(world_baoxiang_get(pstEnv, pstBaoXiangDef->ID))
	{
		return -1;
	}

	if(pstWorldLimit->BaoXiangNum >= MAX_WORLD_BAOXIANG_LIMIT)
	{
		return -1;
	}

	if(pstBaoXiangDef->HideItemID <= 0 || pstBaoXiangDef->HideCount <= 0)
	{
		return -1;
	}

	pstBaoXiangInfo = &pstWorldLimit->BaoXiangLimit[pstWorldLimit->BaoXiangNum++];

	memset(pstBaoXiangInfo, 0, sizeof(*pstBaoXiangInfo));
	pstBaoXiangInfo->ID = pstBaoXiangDef->ID;
	pstBaoXiangInfo->MaxNum = pstBaoXiangDef->HideCount;

	return 0;
}

int world_baoxiang_update(ZONESVRENV* pstEnv, BAOXIANGDEF *pstBaoXiangDef)
{
	WORLDBAOXIANG *pstBaoXiangInfo;

	if(!is_fini_global(pstEnv))
	{
		return -1;
	}

	pstBaoXiangInfo = world_baoxiang_get(pstEnv, pstBaoXiangDef->ID);
	//没有就插入
	if(NULL == pstBaoXiangInfo)
	{
		return world_baoxiang_insert(pstEnv, pstBaoXiangDef);
	}
	
	pstBaoXiangInfo->ID = pstBaoXiangDef->ID;
	pstBaoXiangInfo->MaxNum = pstBaoXiangDef->HideCount;
	return 0;
}

int world_baoxiang_add(ZONESVRENV* pstEnv, int iID)
{
	WORLDBAOXIANG *pstBaoXiangInfo;
	
	pstBaoXiangInfo = world_baoxiang_get(pstEnv, iID);
	if(NULL == pstBaoXiangInfo)
	{
		return -1;
	}

	pstBaoXiangInfo->Num++;
	if(pstBaoXiangInfo->Num >= pstBaoXiangInfo->MaxNum)
	{
		pstBaoXiangInfo->Num = 0;
		return 1;
	}
	
	return 0;
}

int world_limit_tick(ZONESVRENV* pstEnv)
{
	static int iSec = 0;
	int i;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;

	iSec++;
	//每小时检查一次
	if(iSec % 3600 != 0)
	{
		return 0;
	}

	if(!is_fini_global(pstEnv))
	{
		return 0;
	}

	for(i=0; i<pstWorldLimit->ItemNum; i++)
	{
		world_item_limit_cycletime_update(pstEnv, &pstWorldLimit->ItemLimitInfo[i]);
	}

	for(i=0; i<pstWorldLimit->GroupNum; i++)
	{
		world_booty_group_limit_cycletime_update(pstEnv, &pstWorldLimit->GroupLimitInfo[i]);
	}

	return 0;
}

WORLDCARDBOX*  world_cardbox_get(ZONESVRENV* pstEnv, int iID)
{
	int i;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;
	WORLDCARDBOX *pstCardBoxInfo;

	if(!is_fini_global(pstEnv))
	{
		return NULL;
	}

	for(i=0; i<pstWorldLimit->CardBoxNum; i++)
	{
		pstCardBoxInfo = &pstWorldLimit->CardBoxLimit[i];
		if(pstCardBoxInfo ->ID == iID)
		{
			return pstCardBoxInfo;
		}
	}

	return NULL;
}

int world_cardbox_insert(ZONESVRENV* pstEnv, CARDBOXDEF *pstCardBoxDef)
{
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;
	WORLDCARDBOX *pstCardBoxInfo;

	if(!is_fini_global(pstEnv))
	{
		return -1;
	}

	if(world_cardbox_get(pstEnv, pstCardBoxDef->ID))
	{
		return -1;
	}

	if(pstWorldLimit->BaoXiangNum >= MAX_WORLD_CARDBOX_LIMIT)
	{
		return -1;
	}

	if(pstCardBoxDef->LimitID <= 0 || pstCardBoxDef->LimitVal<= 0)
	{
		return -1;
	}

	pstCardBoxInfo = &pstWorldLimit->CardBoxLimit[pstWorldLimit->CardBoxNum++];

	memset(pstCardBoxInfo, 0, sizeof(*pstCardBoxInfo));
	pstCardBoxInfo->ID = pstCardBoxDef->ID;
	pstCardBoxInfo->MaxVal = pstCardBoxDef->LimitVal;

	return 0;
}

int world_cardbox_update(ZONESVRENV* pstEnv, CARDBOXDEF *pstCardBoxDef)
{
	WORLDCARDBOX *pstCardBoxInfo;

	if(!is_fini_global(pstEnv))
	{
		return -1;
	}

	pstCardBoxInfo = world_cardbox_get(pstEnv, pstCardBoxDef->ID);
	//没有就插入
	if(NULL == pstCardBoxInfo)
	{
		return world_cardbox_insert(pstEnv, pstCardBoxDef);
	}
	
	pstCardBoxInfo->ID = pstCardBoxDef->ID;
	pstCardBoxInfo->MaxVal = pstCardBoxDef->LimitVal;
	return 0;
}

int world_cardbox_add(ZONESVRENV* pstEnv, int iID, int iVal)
{
	WORLDCARDBOX *pstCardBoxInfo;
	
	pstCardBoxInfo = world_cardbox_get(pstEnv, iID);
	if(NULL == pstCardBoxInfo)
	{
		return -1;
	}

	pstCardBoxInfo->Val += iVal;
	return 0;
}

int world_cardbox_check(ZONESVRENV* pstEnv, int iID)
{
	WORLDCARDBOX *pstCardBoxInfo;
	
	pstCardBoxInfo = world_cardbox_get(pstEnv, iID);
	if(NULL == pstCardBoxInfo)
	{
		return 0;
	}

	if(pstCardBoxInfo->Val >= pstCardBoxInfo->MaxVal)
	{
		return 1;
	}
	return 0;
}

int world_cardbox_clear(ZONESVRENV* pstEnv, int iID)
{
	WORLDCARDBOX *pstCardBoxInfo;
	
	pstCardBoxInfo = world_cardbox_get(pstEnv, iID);
	if(NULL == pstCardBoxInfo)
	{
		return -1;
	}

	pstCardBoxInfo->Val = 0;
	return 0;
}

int global_booty_group_limit_update(ZONESVRENV* pstEnv)
{
	int i;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;
	WORLDBOOTYGROUPLIMIT *pstBootyGroupLimit;
	WORLDBOOTYGROUPLIMITDEF *pstBootyGroupLimitDef;
	
	if(!is_fini_global(pstEnv))
	{
		return 0;
	}

	//清理已取消的限制
	for(i=pstWorldLimit->GroupNum-1; i>=0; i--)
	{
		pstBootyGroupLimit = &pstWorldLimit->GroupLimitInfo[i];

		pstBootyGroupLimitDef = z_find_booty_group_limit_def(pstEnv, pstBootyGroupLimit ->GroupID);
		if(NULL == pstBootyGroupLimitDef)
		{
			pstWorldLimit->GroupNum--;
			if(i != pstWorldLimit->GroupNum)
			{
				pstWorldLimit->GroupLimitInfo[i] = pstWorldLimit->GroupLimitInfo[pstWorldLimit->GroupNum];
			}
		
			continue;
		}

		world_booty_group_limit_update(pstEnv, pstBootyGroupLimitDef);
	}

	//更新表格数据
	for(i = 0; i<pstEnv->pstZoneObj->iWorldBootyGroupLimitDef; i++)
	{
		pstBootyGroupLimitDef = (WORLDBOOTYGROUPLIMITDEF *)pstEnv->pstZoneObj->sWorldBootyGroupLimitDef[i];

		world_booty_group_limit_update(pstEnv, pstBootyGroupLimitDef);
	}

	return 0;
}

int global_item_limit_update(ZONESVRENV* pstEnv)
{
	int i;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;
	WORLDITEMLIMIT *pstItemLimit;
	WORLDITEMLIMITDEF *pstItemLimitDef;

	if(!is_fini_global(pstEnv))
	{
		return 0;
	}

	//清理已取消的限制
	for(i=pstWorldLimit->ItemNum-1; i>=0; i--)
	{
		pstItemLimit = &pstWorldLimit->ItemLimitInfo[i];

		pstItemLimitDef = z_find_item_limit_def(pstEnv, pstItemLimit->ItemID);
		if(NULL == pstItemLimitDef)
		{
			pstWorldLimit->ItemNum--;
			if(i != pstWorldLimit->ItemNum)
			{
				pstWorldLimit->ItemLimitInfo[i] = pstWorldLimit->ItemLimitInfo[pstWorldLimit->ItemNum];
			}

			continue;
		}

		world_item_limit_update(pstEnv, pstItemLimitDef);
	}

	//更新表格数据
	for(i = 0; i<pstEnv->pstZoneObj->iWorldItemLimitDef; i++)
	{
		pstItemLimitDef = (WORLDITEMLIMITDEF *)pstEnv->pstZoneObj->sWorldItemLimitDef[i];

		world_item_limit_update(pstEnv, pstItemLimitDef);
	}

	return 0;
}

int global_mon_event_update(ZONESVRENV* pstEnv)
{
	int i;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;
	WORLDMONEVENT *pstMonEventInfo;
	MONEVENTDEF  *pstMonEventDef;

	if(!is_fini_global(pstEnv))
	{
		return 0;
	}

	//清理已取消的限制
	for(i=pstWorldLimit->MonEventNum-1; i>=0; i--)
	{
		pstMonEventInfo = &pstWorldLimit->MonEventInfo[i];

		pstMonEventDef = z_find_mon_event_def(pstEnv, pstMonEventInfo->ID);
		if(NULL == pstMonEventDef)
		{
			pstWorldLimit->MonEventNum--;
			if(i != pstWorldLimit->MonEventNum)
			{
				pstWorldLimit->MonEventInfo[i] = pstWorldLimit->MonEventInfo[pstWorldLimit->MonEventNum];
			}

			continue;
		}

		world_mon_event_update(pstEnv, pstMonEventDef);
	}

	//更新表格数据
	for(i = 0; i<pstEnv->pstZoneObj->iMonEventDef; i++)
	{
		pstMonEventDef = (MONEVENTDEF *)pstEnv->pstZoneObj->sMonEventDef[i];

		world_mon_event_update(pstEnv, pstMonEventDef);
	}

	return 0;
}

int global_baoxiang_update(ZONESVRENV* pstEnv)
{
	int i;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;
	WORLDBAOXIANG *pstBaoXiangInfo;
	BAOXIANGDEF  *pstBaoXiangDef;

	if(!is_fini_global(pstEnv))
	{
		return 0;
	}

	//清理已取消的限制
	for(i=pstWorldLimit->BaoXiangNum-1; i>=0; i--)
	{
		pstBaoXiangInfo = &pstWorldLimit->BaoXiangLimit[i];

		pstBaoXiangDef = z_find_baoxiang_def(pstEnv, pstBaoXiangInfo->ID);
		if(NULL == pstBaoXiangDef)
		{
			pstWorldLimit->BaoXiangNum--;
			if(i != pstWorldLimit->BaoXiangNum)
			{
				pstWorldLimit->BaoXiangLimit[i] = pstWorldLimit->BaoXiangLimit[pstWorldLimit->BaoXiangNum];
			}

			continue;
		}

		world_baoxiang_update(pstEnv, pstBaoXiangDef);
	}

	//更新表格数据
	for(i = 0; i<pstEnv->pstZoneObj->iBaoXiangDef; i++)
	{
		pstBaoXiangDef = (BAOXIANGDEF *)pstEnv->pstZoneObj->sBaoXiangDef[i];

		world_baoxiang_update(pstEnv, pstBaoXiangDef);
	}

	return 0;
}

int global_cardbox_update(ZONESVRENV* pstEnv)
{
	int i;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;
	WORLDCARDBOX *pstCardBoxInfo;
	CARDBOXDEF  *pstCardBoxDef;

	if(!is_fini_global(pstEnv))
	{
		return 0;
	}

	//清理已取消的限制
	for(i=pstWorldLimit->CardBoxNum-1; i>=0; i--)
	{
		pstCardBoxInfo = &pstWorldLimit->CardBoxLimit[i];

		pstCardBoxDef = z_find_cardbox_def(pstEnv, pstCardBoxInfo->ID);
		if(NULL == pstCardBoxDef)
		{
			pstWorldLimit->CardBoxNum--;
			if(i != pstWorldLimit->CardBoxNum)
			{
				pstWorldLimit->CardBoxLimit[i] = pstWorldLimit->CardBoxLimit[pstWorldLimit->CardBoxNum];
			}

			continue;
		}

		world_cardbox_update(pstEnv, pstCardBoxDef);
	}

	//更新表格数据
	for(i = 0; i<pstEnv->pstZoneObj->iCardBoxDef; i++)
	{
		pstCardBoxDef = (CARDBOXDEF *)pstEnv->pstZoneObj->sCardBoxDef[i];

		world_cardbox_update(pstEnv, pstCardBoxDef);
	}

	return 0;
}

int global_world_limit_update(ZONESVRENV* pstEnv)
{
	if(!is_fini_global(pstEnv))
	{
		return 0;
	}

	global_item_limit_update(pstEnv);

	global_booty_group_limit_update(pstEnv);

	global_mon_event_update(pstEnv);

	global_baoxiang_update(pstEnv);

	global_cardbox_update(pstEnv);
	
	return 0;
}
