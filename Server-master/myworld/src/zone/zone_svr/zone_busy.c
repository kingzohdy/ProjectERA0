
#include "zone_busy.h"
#include "zone_clt.h"
#include "zone_player.h"
#include "comm_misc.h"
#include "zone_package.h"
#include "zone_task.h"
#include "zone_map.h"
#include "zone_attr.h"
#include "zone_oplog.h"
#include "zone_shop.h"
#include "zone_clan.h"
#include "zone_respact.h"
#include "zone_misc.h"
#include "zone_vip.h"
#include "zone_err.h"

BUSYDEF * z_find_busy_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	BUSYDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.ID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sBusyDef, pstObj->iBusyDef, BUSY_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (BUSYDEF*)pstObj->sBusyDef[iIndex];
		
	}
	
	return NULL;
}

BUSYUSEDEF * z_find_busy_use_def(ZONESVRENV *pstEnv, int iNum)
{
	int iIndex;
	int iEqu;
	BUSYUSEDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.Num = iNum;
	iIndex = bsearch_int(&stDef, pstObj->sBusyUseDef, pstObj->iBusyUseDef, BUSY_USE_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (BUSYUSEDEF*)pstObj->sBusyUseDef[iIndex];
		
	}
	
	return NULL;
}

int player_busy_day_update(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	BUSYINFO *pstBusyInfo = &pstPlayer->stRoleData.MiscInfo.BusyInfo;

	if(!IsSameDay(pstBusyInfo->DayTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstBusyInfo->FiniNum = 0;
		pstBusyInfo->DayTime = pstEnv->pstAppCtx->stCurr.tv_sec;

		player_busy_info_action(pstEnv, pstPlayer);
	}

	return 0;
}

int player_busy_info_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSBUSYSVR *pstBusySvr = &stPkg.Body.BusySvr;
	BUSYINFO *pstBusyInfo = &pstPlayer->stRoleData.MiscInfo.BusyInfo;

	if(!IsSameDay(pstBusyInfo->DayTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstBusyInfo->FiniNum = 0;
		pstBusyInfo->DayTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	
	pstBusySvr->Cmd = BUSY_SVR_OP_DAY_NUM;
	pstBusySvr->Data.Info = *pstBusyInfo;
	Z_CSHEAD_INIT(&stPkg.Head, BUSY_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int player_busy_daynum_add(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	BUSYINFO *pstBusyInfo = &pstPlayer->stRoleData.MiscInfo.BusyInfo;
	
	player_busy_day_update(pstEnv, pstPlayer);
	pstBusyInfo->FiniNum++;

	player_busy_info_action(pstEnv, pstPlayer);
	return 0;
}

int player_busy_task_check(ZONESVRENV* pstEnv, Player *pstPlayer, BUSYDEF *pstBusyDef)
{
	int iRet = 0;
	TASKDEF * pstTaskDef;
	TASKRUNONE *pstTaskRunOne;

	pstTaskDef = task_def_find(pstEnv, pstBusyDef->TaskID, NULL);
	if(!pstTaskDef)
	{
		return -1;
	}

	pstTaskRunOne = task_run_find(pstPlayer, pstTaskDef->ID);
	if(pstTaskRunOne)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_BUSY201);
		return -1;
	}

	pstPlayer->stOnline.fBusyCheck = 1;
	iRet = task_accept_check(pstEnv, pstPlayer, pstTaskDef, 1, 0);
	pstPlayer->stOnline.fBusyCheck = 0;
	if(0 > iRet)
	{
		switch(-iRet)
		{
			case TAE_CLAN_LIMIT:
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_BUSY202);
				break;
			case TAE_FINI_PER_DAY:
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_BUSY203);
				break;
			case TAE_LIMIT_GROUP:
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_BUSY203);
				break;
			case TAE_MUTEX:
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_BUSY201);
				break;
			default:
				break;
		}
		
		return -1;
	}
	
	return 0;
}

int player_busy_task_fini(ZONESVRENV* pstEnv, Player *pstPlayer, BUSYDEF *pstBusyDef)
{
	TASKDEF * pstTaskDef;
	TASKRUNONE *pstTaskRunOne;

	pstTaskDef = task_def_find(pstEnv, pstBusyDef->TaskID, NULL);
	if(!pstTaskDef)
	{
		return -1;
	}

	pstTaskRunOne = task_run_find(pstPlayer, pstTaskDef->ID);
	if(pstTaskRunOne)
	{
		return -1;
	}

	/*
	if(0 > task_accept_check(pstEnv, pstPlayer, pstTaskDef, 1, 0))
	{
		return -1;
	}
	*/

	if (pstTaskDef->TaskGetLimit.NumOneDay > 0)
	{
		task_add_oneday(pstEnv, pstPlayer, pstTaskDef->ID);
	}
	
	if (pstTaskDef->LimitGroupID > 0)
	{
		group_add_oneday(pstEnv, pstPlayer, pstTaskDef->LimitGroupID);
	}

	task_fini_trace(pstEnv, pstPlayer, pstTaskDef->ID);

	task_info_all(pstEnv, pstPlayer);
	
	return 0;
}

int player_busy_taskrecycle_check(ZONESVRENV* pstEnv, Player *pstPlayer, BUSYDEF *pstBusyDef)
{
	TASKRUNONE *pstTaskRunOne;
	TASKRECYCLEDEF * pstTaskRecycleDef;
	TASKRECYCLEONE *pstTaskRecycleOne;
	TASKCYCLEONEDAYINFO *pstCycOneDayInfo;

	pstTaskRecycleDef = taskcycle_def_find(pstEnv, pstBusyDef->TaskRecycleID, NULL);
	if(!pstTaskRecycleDef)
	{
		return -1;
	}

	pstTaskRecycleOne = taskcycle_run_find(pstPlayer, pstTaskRecycleDef->ID);
	if(pstTaskRecycleOne)
	{
		pstTaskRunOne = task_run_find(pstPlayer, pstTaskRecycleOne->TaskID);
		if(pstTaskRunOne || pstTaskRecycleOne->TaskRecycleIdx > 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
								ZONEERR_BUSY204, pstTaskRecycleDef->Name);
			return -1;
		}
	}

	pstCycOneDayInfo =  taskcycle_find_oneday(pstPlayer, pstTaskRecycleDef->ID);
	if (pstCycOneDayInfo && pstCycOneDayInfo->OneDayNum > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
							ZONEERR_BUSY205);
		return -1;
	}

	if (0 > taskcycle_accept_check(pstEnv, pstPlayer, pstTaskRecycleDef, 1))	
	{
		return -1;
	}

	return 0;
}

int player_busy_taskrecycle_fini(ZONESVRENV* pstEnv, Player *pstPlayer, BUSYDEF *pstBusyDef)
{
	TASKRUNONE *pstTaskRunOne;
	TASKRECYCLEDEF * pstTaskRecycleDef;
	TASKRECYCLEONE *pstTaskRecycleOne;
	TASKCYCLEONEDAYINFO *pstCycOneDayInfo;
	
	pstTaskRecycleDef = taskcycle_def_find(pstEnv, pstBusyDef->TaskRecycleID, NULL);
	if(!pstTaskRecycleDef)
	{
		return 0;
	}

	pstTaskRecycleOne = taskcycle_run_find(pstPlayer, pstTaskRecycleDef->ID);
	if(pstTaskRecycleOne)
	{
		pstTaskRunOne = task_run_find(pstPlayer, pstTaskRecycleOne->TaskID);
		if(pstTaskRunOne || pstTaskRecycleOne->TaskRecycleIdx > 0)
		{
			return 0;
		}
	}

	pstCycOneDayInfo =  taskcycle_find_oneday(pstPlayer, pstTaskRecycleDef->ID);
	if (pstCycOneDayInfo && pstCycOneDayInfo->OneDayNum > 0)
	{
		return 0;
	}

	if (0 > taskcycle_accept_check(pstEnv, pstPlayer, pstTaskRecycleDef, 0))	
	{
		return 0;
	}

	if (pstTaskRecycleDef->OneDayNum > 0 || pstTaskRecycleDef->WeekNum > 0)
	{
		taskcycle_add_oneday(pstEnv, pstPlayer, pstTaskRecycleDef->ID);
	}

	taskcycle_fini_trace(pstEnv, pstPlayer, pstTaskRecycleDef->ID);

	task_info_all(pstEnv, pstPlayer);

	return 0;
}

int player_busy_pworld_check(ZONESVRENV* pstEnv, Player *pstPlayer, BUSYDEF *pstBusyDef)
{
	ZONEPWORLD *pstZonePworld;
	ZONEIDX stIdx;
	
	stIdx.iID = pstBusyDef->PworldID;
	stIdx.iPos = -1;

	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if(!pstZonePworld)
	{
		return -1;
	}

	if(GET_PWORLDID(pstPlayer->stRoleData.Map) == pstBusyDef->PworldID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_BUSY206);
		return -1;
	}

	if(z_check_pworld_enter_num(pstEnv, pstPlayer, &pstZonePworld->stPworldDef, 0))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_BUSY207);
		return -1;
	}

	return 0;
}	

int player_busy_pworld_fini(ZONESVRENV* pstEnv, Player *pstPlayer, BUSYDEF *pstBusyDef)
{
	ZONEPWORLD *pstZonePworld;
	ZONEIDX stIdx;
	ZONEPWORLDINST stPworldInst;
	
	stIdx.iID = pstBusyDef->PworldID;
	stIdx.iPos = -1;

	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if(!pstZonePworld)
	{
		return -1;
	}

	if(GET_PWORLDID(pstPlayer->stRoleData.Map) == pstBusyDef->PworldID)
	{
		return -1;
	}

	memset(&stPworldInst, 0, sizeof(stPworldInst));
	stPworldInst.stPworldIdx = stIdx;
	//填充一个不可能存在，但是又不能是0的值
	stPworldInst.ullPworldWID = 0xFFFFFFFFFFFFFFFFLL;

	player_insert_limit_pworld(pstEnv, pstPlayer, &stPworldInst, 0);
	return 0;
	UNUSED(pstBusyDef);
}	

int player_busy_haitan_check(ZONESVRENV* pstEnv, Player *pstPlayer, BUSYDEF *pstBusyDef)
{
	BEACHINFO *pstBeachInfo = &pstPlayer->stRoleData.MiscInfo.BeachInfo;

	if(pstPlayer->stRoleData.Map == BEACH_MAP_ID )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_BUSY208);
		return -1;
	}

	if (!IsSameDay(pstBeachInfo->EnterTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstBeachInfo->EnterTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstBeachInfo->Sec = 0;
		pstPlayer->stOnline.iBeachMapSec = 0;
	}

	if(pstBeachInfo->Sec > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_BUSY209);
		return -1;
	}
	
	return 0;

	UNUSED(pstBusyDef);
}

int player_busy_haitan_fini(ZONESVRENV* pstEnv, Player *pstPlayer, BUSYDEF *pstBusyDef)
{
	BEACHINFO *pstBeachInfo = &pstPlayer->stRoleData.MiscInfo.BeachInfo;

	pstBeachInfo->Sec = BEACH_EXP_HIGH;
	player_beach_info_action(pstEnv, pstPlayer);

	task_player_trace(pstEnv,pstPlayer,TASK_OBJ_BEACH_TIME_OVER);
	return 0;
}

int player_busy_run_task_fini(ZONESVRENV* pstEnv, Player *pstPlayer, BUSYDEF *pstBusyDef)
{
	int i;
	TASKDEF * pstTaskDef;
	TASKRUNONE *pstTaskRunOne;

	for(i=0; i<MAX_BUSY_FINI_TASK; i++)
	{
		if(pstBusyDef->FiniTaskID[i] == 0)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstBusyDef->FiniTaskID[i], NULL);
		if(!pstTaskDef)
		{
			continue;
		}

		pstTaskRunOne = task_run_find(pstPlayer, pstTaskDef->ID);
		if(!pstTaskRunOne || pstTaskRunOne->TaskStat == TASK_STAT_FINI)
		{
			continue;
		}

		pstTaskRunOne->TaskStat = TASK_STAT_FINI;
		task_chg(pstEnv, pstPlayer, pstTaskRunOne);
	}

	return 0;
}

int player_busy_award_check(ZONESVRENV* pstEnv, Player *pstPlayer, BUSYDEF *pstBusyDef, CSBUSYFINI *pstCSBusyFini)
{
	tdr_longlong ullExp;
	int iItemNum, iFiniNum, iAwardMoney;
	int iCostMoney;
	LEVELUP *pstLevelUP;
	ROLEGRID stRoleGrid[MAX_BUSY_AWARD_ITEM+1];
	BUSYUSEDEF *pstBusyUseDef;
	BUSYINFO *pstBusyInfo = &pstPlayer->stRoleData.MiscInfo.BusyInfo;
	int i;
	int GridNum = 0;

	pstLevelUP = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	if (!pstLevelUP)
 	{
 		return 0;
 	}

	iCostMoney = pstBusyDef->CostMoney.MoneyNum;
	iItemNum = pstBusyDef->CostItemNum;

	iAwardMoney = pstBusyDef->AwardMoney.MoneyNum;
	iAwardMoney += pstBusyDef->AwardMoneyMul/10000.0*pstLevelUP->ContributeMoney;
	
	ullExp = pstBusyDef->Exp;
	ullExp += pstBusyDef->ExpMul/10000.0*pstLevelUP->ContributeExp;
	if(pstCSBusyFini->UseItem == 1)
	{
		ullExp += pstBusyDef->ExpMul15/10000.0*pstLevelUP->ContributeExp;
		iCostMoney += pstBusyDef->CostMoney15;
	}
	else if (pstCSBusyFini->UseItem == 2)
	{
		ullExp += pstBusyDef->ExpMul20/10000.0*pstLevelUP->ContributeExp;
		iCostMoney += pstBusyDef->CostMoney20;
	}
	

	if(ullExp > 0)
	{
		//经验值奖励
		player_world_level_exp(pstEnv, pstPlayer, &ullExp);
		z_player_addexp_test(pstEnv, pstPlayer, &ullExp);
		
		if(ullExp <= 0)
		{
			return -1;
		}
	}

	iFiniNum = pstBusyInfo->FiniNum+1;
	iFiniNum = iFiniNum > pstEnv->pstZoneObj->iBusyUseDef ? pstEnv->pstZoneObj->iBusyUseDef : iFiniNum;
	pstBusyUseDef = z_find_busy_use_def(pstEnv, iFiniNum);
	if(pstBusyUseDef)
	{
		iCostMoney += pstBusyUseDef->MoneyVal;
		iItemNum += pstBusyUseDef->ItemNum;
	}

	if (NULL == star_get_result(pstEnv, pstPlayer, STAR_RESULT_BUSY))
	{
		if(pstCSBusyFini->Type == BUSY_FINI_ITEM)
		{
			if(pstCSBusyFini->UseItem > 0)
			{
				return -1;
			}
			
			//消耗道具
			if(0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, BUSY_ITEM_ID, iItemNum, -1, NULL, PACKAGE_DEC_NONE))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BUSY210);
				return -1;
			}
		}
		else
		{
			//消耗金币
			if(0 > package_dec_money_test(pstEnv, pstPlayer, pstBusyDef->CostMoney.MoneyID, iCostMoney))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOHUOBI);
				return -1;
			}
		}
	}

	if(iAwardMoney > 0)
	{
		//奖励绑银
		if(0 > package_add_money_test(pstEnv, pstPlayer, pstBusyDef->AwardMoney.MoneyID, iAwardMoney))
		{
			return -1;
		}
	}

	//奖励物品
	if(pstBusyDef->AwardItem.ItemID > 0)
	{
		if(0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid[GridNum], pstBusyDef->AwardItem.ItemID))
		{
			return -1;
		}

		stRoleGrid[GridNum].GridData.RoleItem.Num = pstBusyDef->AwardItem.ItemNum;
		GridNum++;
	}

	//也是奖励物品
	for(i=0;i<MAX_BUSY_AWARD_ITEM;i++)
	{
		if(pstBusyDef->EXAwardItem[i].ItemID > 0 )
		{
			if(0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid[GridNum], pstBusyDef->EXAwardItem[i].ItemID))
			{
				return -1;
			}

			stRoleGrid[GridNum].GridData.RoleItem.Num = pstBusyDef->EXAwardItem[i].ItemNum;
			GridNum++;
		}
	}

	if(GridNum > 0)
	{
		if(0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, stRoleGrid, GridNum, 0))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
			return -1;
		}
	}
	return 0;
}

int player_busy_award(ZONESVRENV* pstEnv, Player *pstPlayer, BUSYDEF *pstBusyDef, CSBUSYFINI *pstCSBusyFini)
{
	tdr_longlong ullExp;
	int iFiniNum;
	int iCostMoney, iItemNum, iAwardMoney;
	LEVELUP *pstLevelUP;
	ROLEGRID stRoleGrid;
	BUSYUSEDEF *pstBusyUseDef;
	BUSYINFO *pstBusyInfo = &pstPlayer->stRoleData.MiscInfo.BusyInfo;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	int i;

	pstLevelUP = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	if (!pstLevelUP)
 	{
 		return 0;
 	}

	iCostMoney = pstBusyDef->CostMoney.MoneyNum;
	iItemNum = pstBusyDef->CostItemNum;
		
	ullExp = pstBusyDef->Exp;
	ullExp += pstBusyDef->ExpMul/10000.0*pstLevelUP->ContributeExp;

	iAwardMoney = pstBusyDef->AwardMoney.MoneyNum;
	iAwardMoney += pstBusyDef->AwardMoneyMul/10000.0*pstLevelUP->ContributeMoney;
	if(pstCSBusyFini->UseItem == 1)
	{
		ullExp += pstBusyDef->ExpMul15/10000.0*pstLevelUP->ContributeExp;
		iCostMoney += pstBusyDef->CostMoney15;
	}
	else if (pstCSBusyFini->UseItem == 2)
	{
		ullExp += pstBusyDef->ExpMul20/10000.0*pstLevelUP->ContributeExp;
		iCostMoney += pstBusyDef->CostMoney20;
	}

	iFiniNum = pstBusyInfo->FiniNum+1;
	iFiniNum = iFiniNum > pstEnv->pstZoneObj->iBusyUseDef ? pstEnv->pstZoneObj->iBusyUseDef : iFiniNum;
	pstBusyUseDef = z_find_busy_use_def(pstEnv, iFiniNum);
	if(pstBusyUseDef)
	{
		iCostMoney += pstBusyUseDef->MoneyVal;
		iItemNum += pstBusyUseDef->ItemNum;
	}

	if(ullExp > 0)
	{	
		//经验值奖励
		player_world_level_exp(pstEnv, pstPlayer, &ullExp);
		player_add_exp(pstEnv, pstPlayer, ullExp, 0);
	}

	if (NULL == star_get_result(pstEnv, pstPlayer, STAR_RESULT_BUSY))
	{
		if(pstCSBusyFini->Type == BUSY_FINI_ITEM)
		{
			if(pstCSBusyFini->UseItem > 0)
			{
				return -1;
			}
			
			if(0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, BUSY_ITEM_ID, iItemNum, -1, NULL, 
				PACKAGE_DEC_NONE, IMPITEM_OPTYPE_BUSY))
			{
				return -1;
			}
		}
		else
		{
			//消耗金币
			if(0 > package_dec_money(pstEnv, pstPlayer, pstBusyDef->CostMoney.MoneyID, iCostMoney))
			{
				return -1;
			}

			godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
						pstPlayer->stRoleData.Level, GODCOIN_OPER_FLAG_BUSY, pstBusyDef->ID, 0, 0,
						iCostMoney,"我很忙");
		}
	}

	if(iAwardMoney > 0)
	{
		//奖励绑银
		if(0 > package_add_money(pstEnv, pstPlayer, pstBusyDef->AwardMoney.MoneyID, iAwardMoney))
		{
			return -1;
		}
	}

	if (pstBusyDef->AwardMoney.MoneyID == ATTR_ID_BULL)
	{
		z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_BUSY, 0,  
			pstBusyDef->AwardMoney.MoneyNum, 0, "我很忙");
	}
	else if (pstBusyDef->AwardMoney.MoneyID == ATTR_ID_MONEY)
	{
		z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_BUSY, 0,  0, 
			pstBusyDef->AwardMoney.MoneyNum, "我很忙");
	}

	//奖励物品
	if(pstBusyDef->AwardItem.ItemID > 0)
	{
		if(0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstBusyDef->AwardItem.ItemID))
		{
			return -1;
		}

		stRoleGrid.GridData.RoleItem.Num = pstBusyDef->AwardItem.ItemNum;

		if(pstBusyDef->AwardItem.Bind)
		{
			stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
		}

		if(0 > package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1, 0, IMPITEM_OPTYPE_BUSY))
		{
			return -1;
		}
	}

	if(pstBusyDef->Respact.ID)
	{
		player_respact_inc(pstEnv, pstPlayer, pstBusyDef->Respact.ID,
		                   pstBusyDef->Respact.Value, 0);
	}

	if(pstClan)
	{
		if(pstBusyDef->ClanMoney > 0)
		{
			clan_resource_inc(pstEnv, pstClan, pstPlayer, CLAN_MONEY,
			                  pstBusyDef->ClanMoney);
		}
		
		if(pstBusyDef->ClanOwnedContribute)
		{
			clan_resource_inc(pstEnv, pstClan, pstPlayer, CLAN_OWNED_CONTRIBUT,
			                  pstBusyDef->ClanOwnedContribute);
		}

		// 公会安定度
		if (pstBusyDef->ClanAnDing > 0)
		{
			clan_resource_inc(pstEnv, pstClan, pstPlayer, CLAN_STABLE_VALUE,
			                  pstBusyDef->ClanAnDing);
		}
	}

	//也是奖励物品
	for(i=0;i<MAX_BUSY_AWARD_ITEM;i++)
	{
		if(pstBusyDef->EXAwardItem[i].ItemID > 0 )
		{
			if(0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstBusyDef->EXAwardItem[i].ItemID))
			{
				return -1;
			}

			stRoleGrid.GridData.RoleItem.Num = pstBusyDef->EXAwardItem[i].ItemNum;

			if(pstBusyDef->EXAwardItem[i].Bind)
			{
				stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
			}

			if(0 > package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1, 0, IMPITEM_OPTYPE_BUSY))
			{
				return -1;
			}
		}
	}
	
	return 0;
}

int player_busy_fini(ZONESVRENV* pstEnv, Player *pstPlayer, CSBUSYFINI *pstCSBusyFini)
{
	CSPKG stPkg;
	CSBUSYSVR *pstBusySvr = &stPkg.Body.BusySvr;
	BUSYDEF *pstBusyDef;

	player_busy_day_update(pstEnv, pstPlayer);

	pstBusyDef = z_find_busy_def(pstEnv, pstCSBusyFini->ID);
	if(!pstBusyDef)
	{
		return 0;
	}

	if(pstBusyDef->Type == BUSY_TYPE_TASK)
	{
		if(0 > player_busy_task_check(pstEnv, pstPlayer, pstBusyDef))
		{
			return -1;
		}
	}
	else if(pstBusyDef->Type == BUSY_TYPE_TASK_RECYCLE)
	{
		if(0 > player_busy_taskrecycle_check(pstEnv, pstPlayer, pstBusyDef))
		{
			return -1;
		}
	}
	else if(pstBusyDef->Type == BUSY_TYPE_PWORLD)
	{
		if(0 > player_busy_pworld_check(pstEnv, pstPlayer, pstBusyDef))
		{
			return -1;
		}
	}
	else if(pstBusyDef->Type == BUSY_TYPE_HAITAN)
	{
		if(0 > player_busy_haitan_check(pstEnv, pstPlayer, pstBusyDef))
		{
			return -1;
		}
	}

	if(0 > player_busy_award_check(pstEnv, pstPlayer, pstBusyDef, pstCSBusyFini))
	{
		return -1;
	}

	if(pstBusyDef->Type == BUSY_TYPE_TASK)
	{
		if(0 > player_busy_task_fini(pstEnv, pstPlayer, pstBusyDef))
		{
			return -1;
		}
	}
	else if(pstBusyDef->Type == BUSY_TYPE_TASK_RECYCLE)
	{
		if(0 > player_busy_taskrecycle_fini(pstEnv, pstPlayer, pstBusyDef))
		{
			return -1;
		}
	}
	else if(pstBusyDef->Type == BUSY_TYPE_PWORLD)
	{
		if(0 > player_busy_pworld_fini(pstEnv, pstPlayer, pstBusyDef))
		{
			return -1;
		}
	}
	else if(pstBusyDef->Type == BUSY_TYPE_HAITAN)
	{
		if(0 > player_busy_haitan_fini(pstEnv, pstPlayer, pstBusyDef))
		{
			return -1;
		}
	}

	player_busy_run_task_fini(pstEnv, pstPlayer, pstBusyDef);
	player_busy_award(pstEnv, pstPlayer, pstBusyDef, pstCSBusyFini);

	player_busy_daynum_add(pstEnv, pstPlayer);

	pstBusySvr->Cmd = BUSY_SVR_OP_FINI_ACTION;
	pstBusySvr->Data.FiniAction.ID = pstBusyDef->ID;
	Z_CSHEAD_INIT(&stPkg.Head, BUSY_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_BUSY211, pstBusyDef->Name);
	return 0;
}

int player_busy_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
			TFRAMEHEAD	 *pstFrameHead, CSPKG *pstCsPkg)
{
	CSBUSYCLT *pstBusyClt = &pstCsPkg->Body.BusyClt;

	switch(pstBusyClt->Cmd)
	{
		case BUSY_CLT_OP_FINI:
			player_busy_fini(pstEnv, pstPlayer, &pstBusyClt->Data.Fini);
			break;
		default:
			break;
	}
	return 0;
}

int player_busy_ui(ZONESVRENV* pstEnv, Player *pstPlayer, int iID, int iNpcID)
{
	CSPKG stPkg;
	CSBUSYSVR *pstBusySvr = &stPkg.Body.BusySvr;

	pstBusySvr->Cmd = BUSY_SVR_OP_UI;
	pstBusySvr->Data.BusyUI.ID  = iID;
	pstBusySvr->Data.BusyUI.NpcID  = iNpcID;
	
	Z_CSHEAD_INIT(&stPkg.Head, BUSY_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}
