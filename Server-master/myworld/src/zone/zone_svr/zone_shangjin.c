/*
**  @file $RCSfile: zone_shangjin.c,v $
**  general description of this module
**  $Id: zone_shangjin.c,v 1.22 2014/04/24 04:09:49 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2014/04/24 04:09:49 $
**  @version $Revision: 1.22 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_task.h"
#include "zone_clt.h"
#include "zone_player.h"
#include "comm_misc.h"
#include "zone_package.h"
#include "zone_oplog.h"
#include "zone_shop.h"
#include "zone_err.h"

SJTASKDEF* z_sj_task_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	SJTASKDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.ID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sSJTaskDef, pstObj->iSJTaskDef, SJ_TASK_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (SJTASKDEF*)pstObj->sSJTaskDef[iIndex];	
	}
	return NULL;
}

int sj_task_pre(ZONESVRENV *pstEnv)
{
	int i;
	TASKDEF *pstDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	SJTaskFastList *pstSJTaskFastList = pstEnv->pstSJTaskFastList;

	memset(pstSJTaskFastList, 0, sizeof(*pstSJTaskFastList));
	for(i=0; i<pstObj->iTaskDef; i++)
	{
		pstDef= (TASKDEF*)pstObj->sTaskDef[i];
		if(pstDef->SJID > 0 && pstDef->SJID <= MAX_SJ_TASK_LIST)
		{
			pstSJTaskFastList->stOneList[pstDef->SJID-1].iTaskID[pstSJTaskFastList->stOneList[pstDef->SJID-1].iNum++] = pstDef->ID;
		}
	}
	
	return 0;
}

SJTASKONEINFO* player_sjtask_info_get(Player *pstPlayer, int iSJID)
{
	int i;
	SJTASKINFO *pstSJTaskInfo =&pstPlayer->stRoleData.MiscInfo.SJTaskInfo;
	
	for(i=0; i<pstSJTaskInfo->Num; i++)
	{
		if(pstSJTaskInfo->SJTaskOne[i].SJID == iSJID)
		{
			return &pstSJTaskInfo->SJTaskOne[i];
		}
	}

	if(pstSJTaskInfo->Num >= MAX_SJ_TASK_LIST)
	{
		return NULL;
	}
	
	memset(&pstSJTaskInfo->SJTaskOne[pstSJTaskInfo->Num], 0, sizeof(pstSJTaskInfo->SJTaskOne[0]));
	pstSJTaskInfo->SJTaskOne[pstSJTaskInfo->Num].SJID = iSJID;
	pstSJTaskInfo->Num++;
	
	return &pstSJTaskInfo->SJTaskOne[pstSJTaskInfo->Num-1];
}

int sj_task_trace_init(Player *pstPlayer, TASKRUNONE *pstTaskRunOne, TASKDEF *pstTaskDef)
{
	int i;
	TASKMONTRACEONE *pstTaskMonTraceOne;
	TASKITEMTRACEONE *pstTaskItemTraceOne;
	SJTASKONEINFO *pstSJTaskOneInfo;

	pstSJTaskOneInfo = player_sjtask_info_get(pstPlayer, pstTaskDef->SJID);
	if(!pstSJTaskOneInfo)
	{
		return -1;
	}
	
	if(pstTaskDef->Type == TASK_TYPE_SJ_MON)
	{
		for (i=0; i<pstTaskRunOne->TaskMonTrace.TaskMonTraceNum; i++)
		{
			pstTaskMonTraceOne = &pstTaskRunOne->TaskMonTrace.TaskKillMons[i];
			pstTaskMonTraceOne->MaxNum = pstSJTaskOneInfo->Num;
		}
	}
	else if(pstTaskDef->Type == TASK_TYPE_SJ_ITEM)
	{
		for (i=0; i<pstTaskRunOne->TaskItemTrace.TaskItemTraceNum; i++)
		{
			pstTaskItemTraceOne = &pstTaskRunOne->TaskItemTrace.TaskGetItems[i];
			pstTaskItemTraceOne->MaxNum = pstSJTaskOneInfo->Num;
		}
	}

	pstTaskRunOne->SJTaskStar = pstSJTaskOneInfo->Star;

	return 0;
}

int sj_task_rand_task(ZONESVRENV* pstEnv, SJTASKONEINFO *pstSJTaskOneInfo)
{
	int iIdx;
	int iTaskID;
	TASKDEF *pstTaskDef;
	SJTaskFastList *pstSJTaskFastList = pstEnv->pstSJTaskFastList;
	OneSJTaskList *pstOneSJTaskList = &pstSJTaskFastList->stOneList[pstSJTaskOneInfo->SJID-1];

	if(pstOneSJTaskList->iNum <= 0)
	{
		return -1;
	}

	iIdx = RAND1(pstOneSJTaskList->iNum);
	iTaskID  = pstOneSJTaskList->iTaskID[iIdx];
	
	pstTaskDef = task_def_find(pstEnv, iTaskID, NULL);
	if(!pstTaskDef)
	{
		return -1;
	}

	if(pstTaskDef->Type == TASK_TYPE_SJ_ITEM)
	{
		int iNumList[4] = {1, 5, 10, 20};

		iIdx = RAND1(4);
		pstSJTaskOneInfo->Num = iNumList[iIdx];
	}
	else if(pstTaskDef->Type == TASK_TYPE_SJ_MON)
	{
		int iNumList[4] = {50, 300, 500, 1000};

		iIdx = RAND1(4);
		pstSJTaskOneInfo->Num = iNumList[iIdx];
	}

	pstSJTaskOneInfo->TaskID = iTaskID;
	return 0;
}

int sj_task_rand_star(ZONESVRENV* pstEnv, SJTASKONEINFO *pstSJTaskOneInfo)
{
	int iStar = 0;
	int i, iRate, iRand;
	SJTASKDEF *pstSJTaskDef;
	
	pstSJTaskDef = z_sj_task_def(pstEnv, pstSJTaskOneInfo->SJID);
	if(!pstSJTaskDef)
	{
		return -1;
	}

	iRand = RAND1(100);
	iRate = 0;
	for(i=0; i<MAX_SJ_TASK_STAR; i++)
	{
		iRate += pstSJTaskDef->StarRate[i].Rate;
		if(iRand  < iRate)
		{
			iStar = i;
			break;
		}
	}

	pstSJTaskOneInfo->Star = iStar;
	return 0;
}


int player_sj_task_info(ZONESVRENV* pstEnv, Player *pstPlayer, SJTASKONEINFO *pstSJTaskOneInfo)
{
	CSPKG stPkg;
	CSSJTASKSVR *pstSJTaskSvr = &stPkg.Body.SJTaskSvr;

	pstSJTaskSvr->Type = SJ_TASK_SVR_TYPE_UI_INFO;
	pstSJTaskSvr->Data.TaskUIInfo.SJTaskInfo = *pstSJTaskOneInfo;
	Z_CSHEAD_INIT(&stPkg.Head, SJ_TASK_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}	

int player_sj_item_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSSJTASKSVR *pstSJTaskSvr = &stPkg.Body.SJTaskSvr;

	pstSJTaskSvr->Type = SJ_TASK_SVR_ITEM_CHG;
	pstSJTaskSvr->Data.TaskItem.Num = pstPlayer->stRoleData.MiscInfo.SJTaskInfo.GetItemNum;
	Z_CSHEAD_INIT(&stPkg.Head, SJ_TASK_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int player_sj_item_clear(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	if (!IsSameWeek(iCurr, pstPlayer->stRoleData.MiscInfo.SJTaskInfo.GetTime))
	{
		pstPlayer->stRoleData.MiscInfo.SJTaskInfo.GetTime = iCurr;
		pstPlayer->stRoleData.MiscInfo.SJTaskInfo.GetItemNum = 0;
	}

	player_sj_item_info(pstEnv, pstPlayer);
	return 0;
}

int player_sj_task_ui_close(ZONESVRENV* pstEnv, Player *pstPlayer, SJTASKONEINFO *pstSJTaskOneInfo)
{
	CSPKG stPkg;
	CSSJTASKSVR *pstSJTaskSvr = &stPkg.Body.SJTaskSvr;

	pstSJTaskSvr->Type = SJ_TASK_SVR_TYPE_CLOSE;
	pstSJTaskSvr->Data.TaskClose.SJID = pstSJTaskOneInfo->SJID;
	Z_CSHEAD_INIT(&stPkg.Head, SJ_TASK_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}	

int player_sjtask_open(ZONESVRENV* pstEnv, Player *pstPlayer, CSSJTASKOPEN *pstSJTaskOpen)
{
	ITEMDEF *pstItemDef;
	RESULT *pstResult;
	SJTASKONEINFO *pstSJTaskOneInfo;

	pstItemDef = z_find_itemdef(pstEnv, pstSJTaskOpen->ItemID);
	if(!pstItemDef)
	{
		return -1;
	}

	pstResult = z_item_get_result(pstItemDef, RESULT_SHANG_JI_TASK);
	if(!pstResult)
	{
		return -1;
	}

	if(0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstSJTaskOpen->ItemID, 1, -1,  NULL, PACKAGE_DEC_NONE))
	{
		return -1;
	}

	pstSJTaskOneInfo = player_sjtask_info_get(pstPlayer, pstResult->ResultVal1);
	if(!pstSJTaskOneInfo)
	{
		return -1;
	}

	if(pstSJTaskOneInfo->TaskID > 0)
	{	
		goto _final;
	}

	if(0 > sj_task_rand_task(pstEnv, pstSJTaskOneInfo))
	{
		return -1;
	}

	if(0 > sj_task_rand_star(pstEnv, pstSJTaskOneInfo))
	{
		return -1;
	}

_final:

	player_sj_task_info(pstEnv, pstPlayer, pstSJTaskOneInfo);;
	return 0;
}

int player_sjtask_refresh_task(ZONESVRENV* pstEnv, Player *pstPlayer, CSSJTASKREFRESHTASK *pstSJTaskRefresh)
{
	int iRet;
	SJTASKDEF *pstSJTaskDef;
	SJTASKONEINFO *pstSJTaskOneInfo;

	pstSJTaskOneInfo = player_sjtask_info_get(pstPlayer, pstSJTaskRefresh->SJID);
	if(!pstSJTaskOneInfo)
	{
		return -1;
	}

	pstSJTaskDef = z_sj_task_def(pstEnv, pstSJTaskOneInfo->SJID);
	if(!pstSJTaskDef)
	{
		return -1;
	}

	if (0 > package_dec_money_test(pstEnv, pstPlayer, pstSJTaskDef->RefreshTaskMoney.Type, pstSJTaskDef->RefreshTaskMoney.Val))
	{
		char szBuff[32];
		
		if (money_type_to_string(pstSJTaskDef->RefreshTaskMoney.Type, szBuff, sizeof(szBuff)) < 0 )
		{
			strcpy(szBuff, "金钱");
		}

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NOHUOBI);
		return -1;
	}

	if (0 > (iRet = package_dec_money(pstEnv, pstPlayer, pstSJTaskDef->RefreshTaskMoney.Type, pstSJTaskDef->RefreshTaskMoney.Val)))
	{
		return -1;
	}

	if (pstSJTaskDef->RefreshTaskMoney.Type == ATTR_ID_MONEY)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_SHANGJI, pstSJTaskDef->ID, 0, pstSJTaskDef->RefreshTaskMoney.Val, "赏金");
	}
	else if (pstSJTaskDef->RefreshTaskMoney.Type == ATTR_ID_BULL)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_SHANGJI, pstSJTaskDef->ID, pstSJTaskDef->RefreshTaskMoney.Val-iRet,iRet,  "赏金");
	}
	else if (pstSJTaskDef->RefreshTaskMoney.Type == ATTR_ID_GODCOIN)
	{
		godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
					pstPlayer->stRoleData.Level, GODCOIN_OPER_FLAG_SHANGJI,pstSJTaskDef->ID,0,0,
					pstSJTaskDef->RefreshTaskMoney.Val,"赏金");
	}

	if(0 > sj_task_rand_task(pstEnv, pstSJTaskOneInfo))
	{
		return -1;
	}
	
	player_sj_task_info(pstEnv, pstPlayer, pstSJTaskOneInfo);;
	return 0;
}

int player_sjtask_refresh_star(ZONESVRENV* pstEnv, Player *pstPlayer, CSSJTASKREFRESHSTAR *pstSJTaskRefresh)
{
	int iRet;
	SJTASKDEF *pstSJTaskDef;
	SJTASKONEINFO *pstSJTaskOneInfo;

	pstSJTaskOneInfo = player_sjtask_info_get(pstPlayer, pstSJTaskRefresh->SJID);
	if(!pstSJTaskOneInfo)
	{
		return -1;
	}

	if (pstSJTaskOneInfo->Star >= 4)
	{
		//z_sys_strf( pstEnv, pstPlayer, SYS_MIDDLE,"赏金令已经是最高星级了");
		return -1;
	}

	pstSJTaskDef = z_sj_task_def(pstEnv, pstSJTaskOneInfo->SJID);
	if(!pstSJTaskDef)
	{
		return -1;
	}

	if ( pstSJTaskRefresh->RefreshFlag == 0)
	{
		if (0 > package_dec_money_test(pstEnv, pstPlayer, pstSJTaskDef->RefreshStarMoney.Type, pstSJTaskDef->RefreshStarMoney.Val))
		{
			char szBuff[32];
			
			if (money_type_to_string(pstSJTaskDef->RefreshStarMoney.Type, szBuff, sizeof(szBuff)) < 0 )
			{
				strcpy(szBuff, "金钱");
			}

			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NOHUOBI);
			return -1;
		}

		if (0 > (iRet = package_dec_money(pstEnv, pstPlayer, pstSJTaskDef->RefreshStarMoney.Type, pstSJTaskDef->RefreshStarMoney.Val)))
		{
			return -1;
		}

		if (pstSJTaskDef->RefreshTaskMoney.Type == ATTR_ID_MONEY)
		{
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_SHANGJI, pstSJTaskDef->ID, 0, pstSJTaskDef->RefreshTaskMoney.Val, "赏金");
		}
		else if (pstSJTaskDef->RefreshTaskMoney.Type == ATTR_ID_BULL)
		{
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_SHANGJI, pstSJTaskDef->ID, pstSJTaskDef->RefreshTaskMoney.Val-iRet,iRet,  "赏金");
		}
		else if (pstSJTaskDef->RefreshTaskMoney.Type == ATTR_ID_GODCOIN)
		{
			godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
						pstPlayer->stRoleData.Level, GODCOIN_OPER_FLAG_SHANGJI,pstSJTaskDef->ID,0,0,
						pstSJTaskDef->RefreshTaskMoney.Val,"赏金");
		}
	}
	else
	{
		// 消耗道具
		if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, SJ_ITEM_ID, 1, -1, NULL,0))
		{
			return -1;
		}

		package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, SJ_ITEM_ID, 1, -1, NULL,0, 0);
		
		ITEMDEF *pstDef;
		pstDef = z_find_itemdef(pstEnv, SJ_ITEM_ID);
		if (pstDef)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_RONGYAN4, pstDef->Name,1);
		}
	}
	
	

	if(0 > sj_task_rand_star(pstEnv, pstSJTaskOneInfo))
	{
		return -1;
	}

	pstSJTaskOneInfo->Count++;
	if(pstSJTaskDef->MaxCostNum > 0 && pstSJTaskOneInfo->Count >= pstSJTaskDef->MaxCostNum)
	{
		pstSJTaskOneInfo->Star = MAX_SJ_TASK_STAR-1;
	}
	
	player_sj_task_info(pstEnv, pstPlayer, pstSJTaskOneInfo);	

	task_sjstar_trace(pstEnv, pstPlayer);
	return 0;
}

int player_sjtask_accpet(ZONESVRENV* pstEnv, Player *pstPlayer, CSSJTASKACCPET *pstSJTaskAccpet)
{
	ITEMDEF *pstItemDef;
	RESULT *pstResult;
	SJTASKONEINFO *pstSJTaskOneInfo;

	pstItemDef = z_find_itemdef(pstEnv, pstSJTaskAccpet->ItemID);
	if(!pstItemDef)
	{
		return -1;
	}

	pstResult = z_item_get_result(pstItemDef, RESULT_SHANG_JI_TASK);
	if(!pstResult)
	{
		return -1;
	}

	if(0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstSJTaskAccpet->ItemID, 1, -1,  NULL, PACKAGE_DEC_NONE))
	{
		return -1;
	}

	pstSJTaskOneInfo = player_sjtask_info_get(pstPlayer, pstResult->ResultVal1);
	if(!pstSJTaskOneInfo)
	{
		return -1;
	}

	if(pstSJTaskOneInfo->TaskID != pstSJTaskAccpet->TaskID ||
		pstSJTaskOneInfo->SJID != pstSJTaskAccpet->SJID)
	{
		return -1;
	}

	if( 0 > task_accept(pstEnv, pstPlayer, pstSJTaskOneInfo->TaskID))
	{
		return -1;
	}

	if(0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstSJTaskAccpet->ItemID, 1, -1,  NULL, 
			PACKAGE_DEC_NONE, IMPITEM_OPTYPE_SJ))
	{
		return -1;
	}
	
	pstSJTaskOneInfo->TaskID = 0;
	pstSJTaskOneInfo->Num = 0;
	pstSJTaskOneInfo->Star = 0;
	pstSJTaskOneInfo->Count = 0;
	player_sj_task_ui_close(pstEnv, pstPlayer, pstSJTaskOneInfo);
	return 0;
}

int player_shangjin_task(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSSJTASKCLT *pstSJTaskClt = &pstCsPkg->Body.SJTaskClt;

	switch (pstSJTaskClt->Type)
	{
	case SJ_TASK_CLT_TYPE_OPEN:
		return player_sjtask_open(pstEnv, pstPlayer, &pstSJTaskClt->Data.Open);
		break;
	case SJ_TASK_CLT_TYPE_REFRESH_TASK:
		return player_sjtask_refresh_task(pstEnv, pstPlayer, &pstSJTaskClt->Data.RefreshTask);
		break;
	case SJ_TASK_CLT_TYPE_REFRESH_STAR:
		return player_sjtask_refresh_star(pstEnv, pstPlayer, &pstSJTaskClt->Data.RefreshStar);
		break;
	case SJ_TASK_CLT_TYPE_ACCPET:
		return player_sjtask_accpet(pstEnv, pstPlayer, &pstSJTaskClt->Data.Accpet);
		break;
	default:
		return -1;
		break;
	
	}
	
	return 0;
}

