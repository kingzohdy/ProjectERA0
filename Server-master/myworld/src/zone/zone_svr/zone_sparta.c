#include "zone_sparta.h"
#include "zone_span.h"
#include "zone_team.h"
#include "zone_clt.h"
#include "zone_map.h"
#include "zone_player.h"
#include "zone_range.h"
#include "zone_package.h"
#include "zone_mail.h"
#include "zone_designation.h"
#include "zone_err.h"
#include "zone_span_strong.h"

int sparta_apply_team_clear(ZONESVRENV* pstEnv, SpartaQueueTeam *pstTeamInfo, int iIdx);

int sparta_wait_queue_ready_action(ZONESVRENV* pstEnv, SpartaWaitQueueInfo *pstWaitQueue);

int player_sparta_exit_apply_action(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_sparta_queue_clear_action(ZONESVRENV* pstEnv, Player *pstPlayer);


WEDEF* z_find_we_def(ZONESVRENV *pstEnv, int iFightVal)
{
	int iIndex;
	int iEqu;
	WEDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.FightVal = iFightVal;
	iIndex = bsearch_int(&stDef, pstObj->sWeDef, pstObj->iWeDef, WE_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (WEDEF*)pstObj->sWeDef[iIndex];
		
	}
	return NULL;
}

/*
SPARTADESIGNATIONDEF * z_find_sparta_designation_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	SPARTADESIGNATIONDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.ID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sSpartaDesignationDef, pstObj->iSpartaDesignationDef, SPARTA_DESIGNATION_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (SPARTADESIGNATIONDEF*)pstObj->sSpartaDesignationDef[iIndex];
		
	}
	return NULL;
}
*/

SPARTAAWARDDEF* z_find_sparta_award_def(ZONESVRENV *pstEnv, int iRange)
{
	int i;
	SPARTAAWARDDEF *pstDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	for(i=0; i<pstObj->iSpartaAwardDef; i++)
	{
		pstDef = (SPARTAAWARDDEF*)pstObj->sSpartaAwardDef[i];
		if(pstDef->RangMin <= iRange &&
			pstDef->RangMax >= iRange)
		{
			return pstDef;
		}
	}
	
	return NULL;
}

int is_in_add_sparta_glory_time(ZONESVRENV* pstEnv)
{
	GAMEEVENTDEF *pstEventDef;
	
	pstEventDef = z_find_event_def(pstEnv, SPARTA_ADD_GLORY_EVENTID, NULL);
	if(!pstEventDef)
	{
		return 0;
	}
		
	if( 0 > z_event_check_active_runing(pstEnv, pstEventDef))
	{
		return 0;
	}
	
	return 1;
}

int sparta_team_val_range_get(ZONESVRENV* pstEnv, SpartaQueueTeam *pstSpartaTeam)
{
	int iTmp;
	
	iTmp = pstEnv->pstAppCtx->stCurr.tv_sec - pstSpartaTeam->tApplyTime;
	iTmp = iTmp/120;
	
	return 200+iTmp*500;
}

int sparta_team_same_range_val(ZONESVRENV* pstEnv, SpartaQueueTeam *pstSpartaTeam1, SpartaQueueTeam *pstSpartaTeam2)
{
	int iRange = sparta_team_val_range_get(pstEnv, pstSpartaTeam1);

	if(pstSpartaTeam2->iTeamVal <= pstSpartaTeam1->iTeamVal+iRange &&
		pstSpartaTeam2->iTeamVal >= pstSpartaTeam1->iTeamVal-iRange)
	{
		return 1;
	}
	
	return 0;
}

int sparta_apply2wait_queue(ZONESVRENV* pstEnv, int iTeamIdx1, int iTeamIdx2)
{
	int i;
	SpartaWaitQueueInfo *pstSpartaWaitInfo;
	SpartaQueueTeam *pstSpartaApplyTeam;
	SpartaQueue *pstSpartaQueue = pstEnv->pstSpartaQueue;

	pstSpartaWaitInfo = &pstSpartaQueue->astWaitQueue[pstSpartaQueue->iWaitNum];

	pstSpartaWaitInfo->tStarTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	pstSpartaApplyTeam = &pstSpartaQueue->astApplyQueue[iTeamIdx1];
	pstSpartaWaitInfo->stSpartaTeam1.ullTeamID  = pstSpartaApplyTeam->ullTeamID;
	pstSpartaWaitInfo->stSpartaTeam1.iTeamVal = pstSpartaApplyTeam->iTeamVal;
	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		pstSpartaWaitInfo->stSpartaTeam1.astMem[i].iReady = 0;
		pstSpartaWaitInfo->stSpartaTeam1.astMem[i].iUin = pstSpartaApplyTeam->astMem[i].iUin;
		pstSpartaWaitInfo->stSpartaTeam1.astMem[i].ullRoleID = pstSpartaApplyTeam->astMem[i].ullRoleID;
		pstSpartaWaitInfo->stSpartaTeam1.astMem[i].iVal = pstSpartaApplyTeam->astMem[i].iVal;
		STRNCPY(pstSpartaWaitInfo->stSpartaTeam1.astMem[i].szRoleName, 
			pstSpartaApplyTeam->astMem[i].szRoleName, CCH(pstSpartaWaitInfo->stSpartaTeam1.astMem[i].szRoleName));
	}

	pstSpartaApplyTeam = &pstSpartaQueue->astApplyQueue[iTeamIdx2];
	pstSpartaWaitInfo->stSpartaTeam2.ullTeamID  = pstSpartaApplyTeam->ullTeamID;
	pstSpartaWaitInfo->stSpartaTeam2.iTeamVal = pstSpartaApplyTeam->iTeamVal;
	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		pstSpartaWaitInfo->stSpartaTeam2.astMem[i].iReady = 0;
		pstSpartaWaitInfo->stSpartaTeam2.astMem[i].iUin= pstSpartaApplyTeam->astMem[i].iUin;
		pstSpartaWaitInfo->stSpartaTeam2.astMem[i].ullRoleID = pstSpartaApplyTeam->astMem[i].ullRoleID;
		pstSpartaWaitInfo->stSpartaTeam2.astMem[i].iVal = pstSpartaApplyTeam->astMem[i].iVal;
		STRNCPY(pstSpartaWaitInfo->stSpartaTeam2.astMem[i].szRoleName, 
			pstSpartaApplyTeam->astMem[i].szRoleName, CCH(pstSpartaWaitInfo->stSpartaTeam2.astMem[i].szRoleName));
	}
	
	pstSpartaQueue->iWaitNum++;

	//iTeamIdx2 > iTeamIdx1
	pstSpartaQueue->iApplyNum--;
	if(iTeamIdx2 != pstSpartaQueue->iApplyNum)
	{
		memcpy(&pstSpartaQueue->astApplyQueue[iTeamIdx2], 
			&pstSpartaQueue->astApplyQueue[pstSpartaQueue->iApplyNum], sizeof(pstSpartaQueue->astApplyQueue[0]));
	}

	pstSpartaQueue->iApplyNum--;
	if(iTeamIdx1 != pstSpartaQueue->iApplyNum)
	{
		memcpy(&pstSpartaQueue->astApplyQueue[iTeamIdx1], 
			&pstSpartaQueue->astApplyQueue[pstSpartaQueue->iApplyNum], sizeof(pstSpartaQueue->astApplyQueue[0]));
	}

	sparta_wait_queue_ready_action(pstEnv, pstSpartaWaitInfo);
	return 0;
}

int sparta_apply_queue_scan(ZONESVRENV* pstEnv, int iStartIdx)
{
	int i;
	int iRet = 0;
	SpartaQueue *pstSpartaQueue = pstEnv->pstSpartaQueue;

	if(iStartIdx+1 >= pstSpartaQueue->iApplyNum)
	{
		return 0;
	}

	for(i=iStartIdx+1; i<pstSpartaQueue->iApplyNum; i++)
	{
		if(sparta_team_same_range_val(pstEnv, &pstSpartaQueue->astApplyQueue[iStartIdx], &pstSpartaQueue->astApplyQueue[i]))
		{
			iRet = 1;
			sparta_apply2wait_queue(pstEnv, iStartIdx, i);
			break;
		}
	}

	return iRet;
}

int sparta_apply_queue_tick(ZONESVRENV* pstEnv)
{
	int i;
	//int iCount = 0;
	GAMEEVENTDEF *pstEventDef;
	SpartaQueue *pstSpartaQueue = pstEnv->pstSpartaQueue;

	pstEventDef = z_find_event_def(pstEnv, SPARTA_RUN_EVENTID, NULL);
	if(!pstEventDef)
	{
		return -1;
	}
		
	if( 0 > z_event_check_active_runing(pstEnv, pstEventDef))
	{
		for(i=pstSpartaQueue->iApplyNum-1; i>=0; i--)
		{
			sparta_apply_team_clear(pstEnv, &pstSpartaQueue->astApplyQueue[i], i);
		}

		return -1;
	}

	for(i=0; i<pstSpartaQueue->iApplyNum; i++)
	{
		if(pstSpartaQueue->iWaitNum >= MAX_SPARTA_WAIT_QUEUE_TEAM)
		{
			break;
		}
		
		if(sparta_apply_queue_scan(pstEnv, i))
		{
			break;
			//iCount++;
			//i--;
		}

		//一次配对成功10组也差不多了
		//if(iCount >= 10)
		//{
		//	break;
		//}
	}

	return 0;
}

SpartaQueueTeam* sparta_apply_queue_get(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	SpartaQueue *pstSpartaQueue = pstEnv->pstSpartaQueue;
	SpartaQueueTeam *pstSpartaQueueTeam;

	for(i=0; i<pstSpartaQueue->iApplyNum; i++)
	{	
		pstSpartaQueueTeam = &pstSpartaQueue->astApplyQueue[i];
		if(pstSpartaQueueTeam->ullTeamID == pstPlayer->stRoleData.MiscInfo.TeamID)
		{
			return pstSpartaQueueTeam;
		}
	}

	return NULL;
}

int sparta_wait_team2pworld_team(PworldTypeSpartaTeam *pstPworldSpartaTeam, SpartaWaitQueueTeam *pstWaitTeam)
{
	int i;
	
	pstPworldSpartaTeam->ullTeamID = pstWaitTeam->ullTeamID;
	pstPworldSpartaTeam->iTeamVal = pstWaitTeam->iTeamVal;

	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		pstPworldSpartaTeam->astMem[i].iUin = pstWaitTeam->astMem[i].iUin;
		pstPworldSpartaTeam->astMem[i].ullRoleID = pstWaitTeam->astMem[i].ullRoleID;
		pstPworldSpartaTeam->astMem[i].iVal = pstWaitTeam->astMem[i].iVal;
		STRNCPY(pstPworldSpartaTeam->astMem[i].szRoleName, pstWaitTeam->astMem[i].szRoleName, 
			CCH(pstPworldSpartaTeam->astMem[i].szRoleName));
	}
	
	return 0;
}

ZONEPWORLDINST* sparta_new_team_pworld(ZONESVRENV * pstEnv, SpartaWaitQueueInfo *pstWaitTeamInfo)
{
	ZONEPWORLD * pstPworld;
	ZONEPWORLDINST*pstZonePworldInst;
	PWORLDSELECT stPworldSelect;


	pstPworld = z_find_pworld2(pstEnv, WORLD_MO_SHI_MAPID);	
	if(!pstPworld)
	{
		return NULL;
	}

	bzero(&stPworldSelect, sizeof(stPworldSelect));

	pstZonePworldInst = z_in_new_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstPworld, &stPworldSelect,  0, pstPworld->stPworldDef.Timeout*60);
	if (NULL == pstZonePworldInst)
	{
		return NULL;
	}

	if (is_same_real_week(pstEnv->pstZoneObj->stSpartaInfo.iTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstEnv->pstZoneObj->stSpartaInfo.iNum++;
	}
	else
	{
		pstEnv->pstZoneObj->stSpartaInfo.iNum = 1;
		pstEnv->pstZoneObj->stSpartaInfo.iTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	
	pstZonePworldInst->iType = PWORLD_TYPE_PLOY;
	sparta_wait_team2pworld_team(&pstZonePworldInst->stPworldTypeData.stSparta.stTeam1, &pstWaitTeamInfo->stSpartaTeam1);
	sparta_wait_team2pworld_team(&pstZonePworldInst->stPworldTypeData.stSparta.stTeam2, &pstWaitTeamInfo->stSpartaTeam2);
	
	return pstZonePworldInst;
}

int sparta_invite_team_clear(ZONESVRENV* pstEnv, SpartaWaitQueueInfo *pstWaitTeamInfo, int iIdx, int fClear)
{
	int i;
	Player *pstPlayer;
	SpartaQueue *pstSpartaQueue = pstEnv->pstSpartaQueue;

	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		pstPlayer = player_get_by_uin(pstEnv, pstWaitTeamInfo->stSpartaTeam1.astMem[i].iUin);
		if(!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
		{
			continue;
		}

		if(pstPlayer->stRoleData.RoleID != pstWaitTeamInfo->stSpartaTeam1.astMem[i].ullRoleID)
		{
			continue;
		}

		//TODO: 通知zone_svr
		player_sparta_queue_clear_action(pstEnv, pstPlayer);
		player_span_exit_pworld_queue_req(pstEnv, pstPlayer);

		if(fClear)
		{
			z_free_player(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}
		
	}


	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		pstPlayer = player_get_by_uin(pstEnv, pstWaitTeamInfo->stSpartaTeam2.astMem[i].iUin);
		if(!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
		{
			continue;
		}

		if(pstPlayer->stRoleData.RoleID != pstWaitTeamInfo->stSpartaTeam2.astMem[i].ullRoleID)
		{
			continue;
		}

		//TODO: 通知zone_svr
		player_sparta_queue_clear_action(pstEnv, pstPlayer);
		player_span_exit_pworld_queue_req(pstEnv, pstPlayer);

		if(fClear)
		{
			z_free_player(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}
	}

	pstSpartaQueue->iWaitNum--;
	if(iIdx != pstSpartaQueue->iWaitNum)
	{
		pstSpartaQueue->astWaitQueue[iIdx] = pstSpartaQueue->astWaitQueue[pstSpartaQueue->iWaitNum];
	}
	
	return 0;
}

int sparta_invite_team_pworld(ZONESVRENV* pstEnv, SpartaWaitQueueInfo *pstWaitTeamInfo, int iIdx)
{	
	int i;
	Player *pstPlayer;
	SSPKG stPkg;
	SSSPANSPARTAINVITE *pstSpartaInvite = &stPkg.Body.SpanSpartaInvite;
	ZONEPWORLDINST*pstZonePworldInst;
	
	pstZonePworldInst = sparta_new_team_pworld(pstEnv, pstWaitTeamInfo);
	if(!pstZonePworldInst)
	{
		return -1;
	}

	pstSpartaInvite->PworldInstID = pstZonePworldInst->iPworldInstID;
	pstSpartaInvite->PworldWID = pstZonePworldInst->ullPworldWID;

	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		pstPlayer = player_get_by_uin(pstEnv, pstWaitTeamInfo->stSpartaTeam1.astMem[i].iUin);
		if(!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
		{
			goto _error;
		}

		if(pstPlayer->stRoleData.RoleID != pstWaitTeamInfo->stSpartaTeam1.astMem[i].ullRoleID)
		{
			goto _error;
		}

		pstSpartaInvite->Uin = pstPlayer->stRoleData.Uin;
		pstSpartaInvite->RoleID =pstPlayer->stRoleData.RoleID;
		STRNCPY(pstSpartaInvite->RoleName, pstPlayer->stRoleData.RoleName, CCH(pstSpartaInvite->RoleName));

		Z_SSHEAD_INIT(&stPkg.Head, SPAN_SPARTA_INVITE);
		z_ssmsg_send(pstEnv, &stPkg, player_master_busid_get(pstEnv, pstPlayer));
	}

	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		pstPlayer = player_get_by_uin(pstEnv, pstWaitTeamInfo->stSpartaTeam2.astMem[i].iUin);
		if(!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
		{
			goto _error;
		}

		if(pstPlayer->stRoleData.RoleID != pstWaitTeamInfo->stSpartaTeam2.astMem[i].ullRoleID)
		{
			goto _error;
		}

		pstSpartaInvite->Uin = pstPlayer->stRoleData.Uin;
		pstSpartaInvite->RoleID = pstPlayer->stRoleData.RoleID;
		STRNCPY(pstSpartaInvite->RoleName, pstPlayer->stRoleData.RoleName, CCH(pstSpartaInvite->RoleName));

		Z_SSHEAD_INIT(&stPkg.Head, SPAN_SPARTA_INVITE);
		z_ssmsg_send(pstEnv, &stPkg, player_master_busid_get(pstEnv, pstPlayer));
	}

	sparta_invite_team_clear(pstEnv, pstWaitTeamInfo, iIdx, 0);
	return 0;
	
_error:
	sparta_invite_team_clear(pstEnv, pstWaitTeamInfo, iIdx, 1);
	return 0;
}

int sparta_wait_queue_check(ZONESVRENV* pstEnv, SpartaWaitQueueInfo *pstWaitQueueInfo, int iIdx)
{
	int i;
	SpartaWaitQueueTeamMem *pstMem;

	if(pstWaitQueueInfo->tStarTime+30 < pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		sparta_invite_team_pworld(pstEnv, pstWaitQueueInfo, iIdx);
		return 0;
	}

	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		pstMem = &pstWaitQueueInfo->stSpartaTeam1.astMem[i];
		if(pstMem->iReady == 0)
		{
			return -1;
		}
	}

	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		pstMem = &pstWaitQueueInfo->stSpartaTeam2.astMem[i];
		if(pstMem->iReady == 0)
		{
			return -1;
		}
	}

	sparta_invite_team_pworld(pstEnv, pstWaitQueueInfo, iIdx);
	return 0;
}

int sparta_wait_queue_tick(ZONESVRENV* pstEnv)
{
	int i;
	SpartaQueue *pstSpartaQueue = pstEnv->pstSpartaQueue;

	for(i=pstSpartaQueue->iWaitNum-1; i>=0; i--)
	{
		sparta_wait_queue_check(pstEnv, &pstSpartaQueue->astWaitQueue[i], i);
	}

	return 0;
}

int sparta_tick(ZONESVRENV* pstEnv)
{
	sparta_apply_queue_tick(pstEnv);

	sparta_wait_queue_tick(pstEnv);
	
	return 0;
}

SpartaWaitQueueTeamMem* sparta_wait_mem_get(SpartaWaitQueueInfo *pstWaitQueue, Player *pstPlayer)
{
	int i;
	SpartaWaitQueueTeamMem *pstMem;
	
	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		pstMem = &pstWaitQueue->stSpartaTeam1.astMem[i];
		if(pstMem->iUin == pstPlayer->stRoleData.Uin)
		{
			return pstMem;
		}
	}

	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		pstMem = &pstWaitQueue->stSpartaTeam2.astMem[i];
		if(pstMem->iUin == pstPlayer->stRoleData.Uin)
		{
			return pstMem;
		}
	}
	
	return NULL;
}

SpartaWaitQueueInfo* sparta_wait_queue_get(ZONESVRENV* pstEnv, Player *pstPlayer, int *piIdx)
{
	int i;
	SpartaQueue *pstSpartaQueue = pstEnv->pstSpartaQueue;

	for(i=0; i<pstSpartaQueue->iWaitNum; i++)
	{
		if(pstSpartaQueue->astWaitQueue[i].stSpartaTeam1.ullTeamID == pstPlayer->stRoleData.MiscInfo.TeamID ||
			pstSpartaQueue->astWaitQueue[i].stSpartaTeam2.ullTeamID == pstPlayer->stRoleData.MiscInfo.TeamID )
		{
			if(piIdx)
			{
				*piIdx = i;
			}
			
			return &pstSpartaQueue->astWaitQueue[i];
		}
	}

	return NULL;
}

int sparta_wait_queue_ready_action(ZONESVRENV* pstEnv, SpartaWaitQueueInfo *pstWaitQueue)
{
	int i, iCount = 0;
	Player *apstPlayer[MAX_SPARTA_ROLE], *pstPlayer;
	SpartaWaitQueueTeamMem *pstMem;
	CSPKG stPkg;
	CSSPARTASVR *pstSpartaSvr = &stPkg.Body.SpartaSvr;
	CSSPARTAREADYUI *pstUIInfo = &pstSpartaSvr->Data.ReadyUI;

	memset(pstUIInfo, 0, sizeof(pstUIInfo));
	
	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		pstMem = &pstWaitQueue->stSpartaTeam1.astMem[i];
		
		pstUIInfo->RoleInfo[i].Ready = pstMem->iReady;
		STRNCPY(pstUIInfo->RoleInfo[i].RoleName, pstMem->szRoleName, CCH(pstUIInfo->RoleInfo[0].RoleName));

		pstPlayer = player_get_by_uin(pstEnv, pstMem->iUin);
		if(pstPlayer)
		{
			apstPlayer[iCount++] = pstPlayer;
		}
	}

	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		pstMem = &pstWaitQueue->stSpartaTeam2.astMem[i];
		
		pstUIInfo->RoleInfo[MAX_SPARTA_TEAM_MEM+i].Ready = pstMem->iReady;
		STRNCPY(pstUIInfo->RoleInfo[MAX_SPARTA_TEAM_MEM+i].RoleName, pstMem->szRoleName, CCH(pstUIInfo->RoleInfo[0].RoleName));

		pstPlayer = player_get_by_uin(pstEnv, pstMem->iUin);
		if(pstPlayer)
		{
			apstPlayer[iCount++] = pstPlayer;
		}
	}

	pstSpartaSvr->Type = SPARTA_SVR_READY_UI;
	Z_CSHEAD_INIT(&stPkg.Head, SPARTA_SVR);
	z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iCount, &stPkg);
	
	return 0;
}

int player_sparta_match_val_get(Player *pstPlayer)
{
	int iVal = 0;
	SPARTAMISCINFO *pstSpartaInfo = &pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo;

	iVal += pstSpartaInfo->FightVal;

	if(pstSpartaInfo->Victory > 30)
	{
		int iRate = player_sparta_victory_rate(pstPlayer);
		if(iRate > 70)
		{
			iVal += 50;
		}
		else if(iRate > 50)
		{
			iVal += 30;
		}
	}

	if(pstSpartaInfo->VictoryCurr > 1)
	{
		iVal = iVal + (pstSpartaInfo->VictoryCurr-1)*10;
	}

	if(pstSpartaInfo->DefeatCurr > 1)
	{
		iVal = iVal - (pstSpartaInfo->Defeat-1)*10;
	}
		
	return iVal;
}

int sparta_team_match_val_get(ZONESVRENV* pstEnv, ZoneTeam *pstTeam)
{
	int i, iVal = 0, iMaxVal = 0, iSum = 0;
	Player *pstTmpPlayer;

	for(i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
		if(!pstTmpPlayer)
		{
			return 0;
		}

		iVal  = player_sparta_match_val_get(pstTmpPlayer);
		if(iVal > iMaxVal)
		{
			iMaxVal = iVal;
		}

		iSum += iVal;
	}

	iSum = iSum/3;
	iMaxVal = iMaxVal-100;

	if(iMaxVal > iSum)
	{
		return iMaxVal;
	}
		
	return iSum;
}

int player_sparta_apply_req(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	Player *pstTmpPlayer;
	Player *pstCaptain;
	ZoneTeam *pstTeam;
	SpanOL *pstSpanOnline;
	SSPKG stPkg;
	SSSPANSPARTAAPPLYREQ *pstSpartaApplyReq = &stPkg.Body.SpanSpartaApplyReq;
	GAMEEVENTDEF *pstEventDef;

	if(INST_MODE_ZONE_SVR != pstEnv->iInstMode ||
				pstEnv->pstConf->SpartaFlag == 0)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPARTA301);
		return -1;
	}
	
	pstEventDef = z_find_event_def(pstEnv, SPARTA_RUN_EVENTID, NULL);
	if(!pstEventDef)
	{
		
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPARTA302);
		return -1;
	}
		
	if( 0 > z_event_check_active_runing(pstEnv, pstEventDef))
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPARTA302);
		return -1;
	}

	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(!pstTeam)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPARTA303);
		return -1;
	}

	pstCaptain = player_team_get_captain_player(pstEnv, pstTeam);
	if(!pstCaptain || pstCaptain != pstPlayer)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPARTA304);
		return -1;
	}

	if(pstTeam->stTeamInfo.MemberNum != MAX_SPARTA_TEAM_MEM)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPARTA305);
		return -1;
	}

	pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
	if(pstSpanOnline->stApply.iBattleApply == SPAN_APPLY_ING)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPARTA306);
		return -1;
	}

	if(pstSpanOnline->stApply.iSpartaApply == SPAN_APPLY_ING)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPARTA307);
		return -1;
	}

	if (pstPlayer->stOnline.stPworldQueue.iPworldID > 0 &&
		pstPlayer->stOnline.stPworldQueue.iQueueType > 0)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPARTA306);
		return -1;
	}

	if(player_is_zuolao(pstPlayer) ||
		(pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE && CLAN_CITY_MAP_ID != GET_MAPID(pstPlayer->stRoleData.Map)))
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPARTA308);
		return -1;
	}

	for(i = 0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
		if(!pstTmpPlayer || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
				ZONEERR_SPARTA309, 
				pstTeam->stTeamInfo.TeamMember[i].MemberName);
			return -1;
		}

		if(pstTmpPlayer->stRoleData.Level < 60)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPARTA310, pstTmpPlayer->stRoleData.RoleName);
			return -1;
		}

		pstSpanOnline = &pstTmpPlayer->stOnline.stSpanOnline;
		if(pstSpanOnline->stApply.iBattleApply == SPAN_APPLY_ING)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPARTA311,
				pstTmpPlayer->stRoleData.RoleName);
			return -1;
		}

		if (pstTmpPlayer->stOnline.stPworldQueue.iPworldID > 0 &&
			pstTmpPlayer->stOnline.stPworldQueue.iQueueType > 0)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPARTA311,
				pstTmpPlayer->stRoleData.RoleName);
			return -1;
		}

		if(player_is_zuolao(pstTmpPlayer) ||
			(pstTmpPlayer->stRoleData.Map >= PWORLD_MAPID_BASE && CLAN_CITY_MAP_ID != GET_MAPID(pstTmpPlayer->stRoleData.Map)))
		{
			z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPARTA312, pstTmpPlayer->stRoleData.RoleName);
			return -1;
		}

		//末世巅峰准备地图和决赛副本中不能报名末世荣耀
		if (pstTmpPlayer->stRoleData.Map == SHZ_READY_MAP_ID || pstTmpPlayer->stRoleData.Map == ZHZ_READY_MAP_ID ||
			pstTmpPlayer->stRoleData.Map == JNZ_READY_MAP_ID || pstTmpPlayer->stRoleData.Map == LHS_READY_MAP_ID ||
			pstTmpPlayer->stRoleData.Map == LSZ_READY_MAP_ID || pstTmpPlayer->stRoleData.Map == HMZ_READY_MAP_ID ||
			pstTmpPlayer->stRoleData.Map == SYZ_READY_MAP_ID || pstTmpPlayer->stRoleData.Map == YLS_READY_MAP_ID ||
			(GET_PWORLDID(pstTmpPlayer->stRoleData.Map) == DIANFENG_PWORLD_ID))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SPARTA313,pstTmpPlayer->stRoleData.RoleName);
			return -1;
		}
	}

	memset(pstSpartaApplyReq, 0, sizeof(*pstSpartaApplyReq));
	pstSpartaApplyReq->TeamID = pstTeam->stTeamInfo.TeamID;
	pstSpartaApplyReq->FightVal = sparta_team_match_val_get(pstEnv, pstTeam);
	
	for(i = 0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
		if(!pstTmpPlayer || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			return -1;
		}
		
		if(0 > player_span_apply_req(pstEnv, pstTmpPlayer, SPAN_DST_TYPE_SPARTA))
		{
			return -1;
		}

		pstSpartaApplyReq->SpartaMem[i].Uin = pstTmpPlayer->stRoleData.Uin;
		pstSpartaApplyReq->SpartaMem[i].RoleID = pstTmpPlayer->stRoleData.RoleID;
		pstSpartaApplyReq->SpartaMem[i].Val = pstTmpPlayer->stRoleData.MiscInfo.SpartaMiscInfo.FightVal;
		STRNCPY(pstSpartaApplyReq->SpartaMem[i].RoleName, pstTmpPlayer->stRoleData.RoleName, CCH(pstSpartaApplyReq->SpartaMem[i].RoleName));
	}

	Z_SSHEAD_INIT(&stPkg.Head, SPAN_SPARTA_APPLY_REQ);
	z_ssmsg_send(pstEnv, &stPkg, pstEnv->iBattleSpartaID);
	
	
	return 0;
}

int player_sparta_apply_team_clear(ZONESVRENV* pstEnv, SSSPANSPARTAAPPLYREQ *pstReq)
{
	int i;
	Player *pstPlayer;

	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		pstPlayer = player_get_by_uin(pstEnv, pstReq->SpartaMem[i].Uin);
		if(!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
		{
			continue;
		}

		if(pstPlayer->stRoleData.RoleID != pstReq->SpartaMem[i].RoleID)
		{
			continue;
		}

		player_sparta_exit_apply_action(pstEnv, pstPlayer);
		//TODO: zone_svr 清理状态
		player_span_exit_pworld_queue_req(pstEnv, pstPlayer);
		z_free_player(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}
	
	return 0;
}

//报名竞技场
int player_sparta_apply(ZONESVRENV* pstEnv, SSSPANSPARTAAPPLYREQ *pstReq)
{
	int i, iCount = 0;
	Player *pstPlayer, *apstPlayer[MAX_SPARTA_ROLE];
	SpartaQueueTeam *pstSpartaTeam;
	SpartaQueue *pstSpartaQueue = pstEnv->pstSpartaQueue;
	CSPKG stPkg;
	CSSPARTASVR *pstSpartaSvr = &stPkg.Body.SpartaSvr;

	if(pstSpartaQueue->iApplyNum >= MAX_SPARTA_APPLY_QUEUE_TEAM)
	{
		goto _clean;
	}

	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		pstPlayer = player_get_by_uin(pstEnv, pstReq->SpartaMem[i].Uin);
		if(!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
		{
			goto _clean;
		}

		if(pstPlayer->stRoleData.RoleID != pstReq->SpartaMem[i].RoleID)
		{
			goto _clean;
		}

		apstPlayer[iCount++] = pstPlayer;
	}

	pstSpartaTeam = &pstSpartaQueue->astApplyQueue[pstSpartaQueue->iApplyNum];

	pstSpartaTeam->ullTeamID = pstReq->TeamID;
	pstSpartaTeam->iTeamVal = pstReq->FightVal;
	pstSpartaTeam->tApplyTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		pstSpartaTeam->astMem[i].iUin = pstReq->SpartaMem[i].Uin;
		pstSpartaTeam->astMem[i].ullRoleID = pstReq->SpartaMem[i].RoleID;
		pstSpartaTeam->astMem[i].iVal = pstReq->SpartaMem[i].Val;
		STRNCPY(pstSpartaTeam->astMem[i].szRoleName, 
			pstReq->SpartaMem[i].RoleName, CCH(pstSpartaTeam->astMem[i].szRoleName));
	}

	pstSpartaQueue->iApplyNum++;

	//TODO: 下发给客户端
	pstSpartaSvr->Type = SPARTA_SVR_APPLY_INFO;
	pstSpartaSvr->Data.Apply.Succ = 1;

	Z_CSHEAD_INIT(&stPkg.Head, SPARTA_SVR);
	z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iCount, &stPkg);
	
	return 0;
	
_clean:
	player_sparta_apply_team_clear(pstEnv, pstReq);
	return 0;
}

int span_sparta_invite(ZONESVRENV* pstEnv, SSSPANSPARTAINVITE *pstReq)
{
	Player *pstPlayer;
	SSSPANDSTDATA stDstData;
	ZoneTeam *pstTeam;

	pstPlayer = player_get_by_uin(pstEnv, pstReq->Uin);
	if(!pstPlayer || pstPlayer->stRoleData.RoleID != pstReq->RoleID)
	{
		return 0;
	}

	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(!pstTeam || pstTeam->stTeamInfo.MemberNum != MAX_SPARTA_TEAM_MEM)
	{
		return 0;
	}

	stDstData.DstType = SPAN_DST_TYPE_SPARTA;
	stDstData.Data.Sparta.TeamID = pstTeam->stTeamInfo.TeamID;
	stDstData.Data.Sparta.PworldWID = pstReq->PworldWID;
	stDstData.Data.Sparta.PworldInstID = pstReq->PworldInstID;
	
				
	return player_span_hold_req(pstEnv, pstPlayer, &stDstData);
}

int sparta_player_pos_get(ZONEPWORLDINST *pstZonePworldInst, MapIndex *pstMapIdx, Player *pstPlayer, RESPOS *pstPos)
{
	PworldTypeSparta *pstPworldSparta = &pstZonePworldInst->stPworldTypeData.stSparta;
	
	if(pstPlayer->stRoleData.MiscInfo.TeamID == pstPworldSparta->stTeam1.ullTeamID)
	{
		*pstPos = pstMapIdx->stMapDef.RevivePs[0];
	}
	else
	{
		*pstPos = pstMapIdx->stMapDef.RevivePs[1];
	}
	
	return 0;
}

int player_set_sparta_info(ZONESVRENV * pstEnv, Player *pstPlayer, MapIndex *pstMapIndex,
	ZONEPWORLD *pstZonePworld, ZONEPWORLDINST *pstZonePworldInst, RESPOS *pstPos)
{
	int i, iCamp = 0;
	PworldTypeSparta *pstPworldSparta = &pstZonePworldInst->stPworldTypeData.stSparta;

	if(pstZonePworld->stPworldDef.PolyType != PWORLD_POLY_SPARTA)
	{
		return -1;
	}

	memset (&pstPlayer->stOnline.stWarOL, 0, sizeof(pstPlayer->stOnline.stWarOL));

	if(pstPlayer->stRoleData.MiscInfo.TeamID == pstPworldSparta->stTeam1.ullTeamID)
	{
		iCamp = 1;
		*pstPos = pstMapIndex->stMapDef.RevivePs[0];
	}
	else if(pstPlayer->stRoleData.MiscInfo.TeamID == pstPworldSparta->stTeam2.ullTeamID)
	{
		iCamp = 2;
		*pstPos = pstMapIndex->stMapDef.RevivePs[1];
	}
	else
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "RoleName:%s, WorldID:%d, TeamID:%llu, Team1ID:%llu, Team2ID:%llu",
					pstPlayer->stRoleData.RoleName, pstPlayer->stRoleData.WorldID,
					pstPlayer->stRoleData.MiscInfo.TeamID, pstPworldSparta->stTeam1.ullTeamID,
					pstPworldSparta->stTeam2.ullTeamID);

		for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
		{
			if(pstPworldSparta->stTeam1.astMem[i].ullRoleID == pstPlayer->stRoleData.RoleID)
			{
				iCamp = 1;
				*pstPos = pstMapIndex->stMapDef.RevivePs[0];
				break;
			}
		}

		if(iCamp == 0)
		{
			for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
			{
				if(pstPworldSparta->stTeam2.astMem[i].ullRoleID == pstPlayer->stRoleData.RoleID)
				{
					iCamp = 2;
					*pstPos = pstMapIndex->stMapDef.RevivePs[1];
					break;
				}
			}
		}

		if(iCamp == 0)
		{
			iCamp = 1;
			*pstPos = pstMapIndex->stMapDef.RevivePs[0];
			
			tlog_error(pstEnv->pstLogCat, 0, 0, "RoleName:%s, WorldID:%d, RoleID:%llu, Not Find",
					pstPlayer->stRoleData.RoleName, pstPlayer->stRoleData.WorldID,
					pstPlayer->stRoleData.RoleID);
		}
	}

	/*
	//阵营统计信息
	if(iCamp > 0 && iCamp<= MAX_CAMP_NUM)
	{
		pstZonePworldInst->stPworldCamp.stCampInfo[iCamp-1].iCampLv += pstPlayer->stRoleData.Level;
		pstZonePworldInst->stPworldCamp.stCampInfo[iCamp-1].iPlayerNum++;
	}
	*/

	pstPlayer->stOnline.stWarOL.cCampIdx = iCamp;
	pstPlayer->stOnline.stWarOL.cPloyType = pstZonePworld->stPworldDef.PolyType;
	pstPlayer->stOnline.stWarOL.iLevel = pstPlayer->stRoleData.Level;
	pstPlayer->stOnline.stWarOL.tFightTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	return 0;
}

int sparta_end_fight_val(ZONESVRENV* pstEnv, tdr_ulonglong ullTeamID, PworldTypeSparta *pstPworldSparta, int iVCamp)
{
	WEDEF *pstWeDef;
	int iSelfCamp = 0, iWe = 0;
	int iFightVal = abs(pstPworldSparta->stTeam1.iTeamVal - pstPworldSparta->stTeam2.iTeamVal);

	if(ullTeamID == pstPworldSparta->stTeam1.ullTeamID)
	{
		iSelfCamp = 1;
		if(pstPworldSparta->stTeam1.iTeamVal < pstPworldSparta->stTeam2.iTeamVal)
		{
			iWe = 1;
		}
	}
	else
	{
		iSelfCamp = 2;
		if(pstPworldSparta->stTeam2.iTeamVal < pstPworldSparta->stTeam1.iTeamVal)
		{
			iWe = 1;
		}
	}

	if(iFightVal >= MAX_SPARTA_WE)
	{
		if(iSelfCamp ==  iVCamp)
		{
			return 0;
		}
		else
		{
			iFightVal = MAX_SPARTA_WE-1;
		}
	}

	iFightVal = iFightVal/10*10;
	pstWeDef = z_find_we_def(pstEnv, iFightVal);
	if(!pstWeDef)
	{
		return 0;
	}

	if(iWe)
	{
		iWe = 1000 -pstWeDef->We;
	}
	else
	{
		iWe = pstWeDef->We;
	}

	
	if(iVCamp == iSelfCamp)
	{
		return 32*(1000-iWe)/1000;
	}
	else
	{
		return 32*(0-iWe)/1000;
	}
	
	return 0;
}

int player_sparta_end_award(ZONESVRENV* pstEnv, Player *pstPlayer , PworldTypeSparta *pstPworldSparta, int iVCamp)
{
	int iFightVal = sparta_end_fight_val(pstEnv, pstPlayer->stRoleData.MiscInfo.TeamID, pstPworldSparta, iVCamp);
	
	if(pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.FightVal +  iFightVal > MAX_SPARTA_FIGHT_VAL)
	{
		iFightVal = MAX_SPARTA_FIGHT_VAL - pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.FightVal;
	}

	if(pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.FightVal + iFightVal < MIN_SPARTA_FIGHT_VAL)
	{
		iFightVal = MIN_SPARTA_FIGHT_VAL- pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.FightVal;
	}
	pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.FightVal += iFightVal;
	
	if(pstPlayer->stOnline.stWarOL.cCampIdx == iVCamp)
	{
		char szName[ROLE_NAME_LEN];
		int iBusID = player_master_busid_get(pstEnv, pstPlayer);
	
		STRNCPY(szName, pstPlayer->stRoleData.RoleName, CCH(szName));
		battle_rolename_remove_prefix(szName);
		
		pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.Victory += 1;
		pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.VictoryCurr += 1;
		pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.DefeatCurr = 0;

		if(pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.VictoryCurr == 20)
		{
			z_sys_strf_span_broadcast(pstEnv, iBusID, SYS_MIDDLE, 
				"恭喜%s末世荣耀殿堂中连胜20场", pstPlayer->stRoleData.RoleName);
		}
		else if(pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.VictoryCurr == 50)
		{
			z_sys_strf_span_broadcast(pstEnv, iBusID, SYS_MIDDLE, 
				"恭喜%s已经进入了统治模式，在末世荣耀殿堂中连续50场无一败绩", pstPlayer->stRoleData.RoleName);
		}
		else if(pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.VictoryCurr == 100)
		{
			z_sys_strf_span_broadcast(pstEnv, iBusID, SYS_MIDDLE, 
				"恭喜%s深深体会到了高手的寂寞，末世荣耀殿堂已经100场连胜", pstPlayer->stRoleData.RoleName);
		}
		else if(pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.VictoryCurr == 300)
		{
			z_sys_strf_span_broadcast(pstEnv, iBusID, SYS_MIDDLE, 
				"恭喜%s已经连胜300场，独孤求败，谁能击败我？", pstPlayer->stRoleData.RoleName);
		}

		task_player_trace(pstEnv, pstPlayer, TASK_OBJ_SPARTA_WIN);
	}
	else if(iVCamp == 0)
	{
		//平局啥都没
	}
	else
	{
		pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.Defeat += 1;
		pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.DefeatCurr += 1;
		pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.VictoryCurr = 0;
	}

	pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.Total++;

	// 记录周参与
	if ( is_same_real_week(pstEnv->pstAppCtx->stCurr.tv_sec, pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.WeekTime))
	{
		pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.TotalWeek ++;
	}
	else
	{
		pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.WeekTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.TotalWeek =1;
	}

	task_player_sparta_fini_trace(pstEnv, pstPlayer);
	player_sparta_info(pstEnv, pstPlayer);			
	return iFightVal;
}

int sparta_leave_role_chg_fightval_get(ZONESVRENV* pstEnv, PworldTypeSparta *pstPworldSparta, 
	PworldTypeSpartaTeamMem *pstMemInfo, tdr_ulonglong ullTeamID)
{
	int iFightVal;

	//用一个错误的值取必输
	iFightVal = sparta_end_fight_val(pstEnv, ullTeamID, pstPworldSparta, 3)-20;

	if(pstMemInfo->iVal + iFightVal > MAX_SPARTA_FIGHT_VAL)
	{
		iFightVal = MAX_SPARTA_FIGHT_VAL - pstMemInfo->iVal;
	}

	if(pstMemInfo->iVal + iFightVal < MIN_SPARTA_FIGHT_VAL)
	{
		iFightVal = MIN_SPARTA_FIGHT_VAL - pstMemInfo->iVal;
	}

	return iFightVal;
}

int player_sparta_runaway_fightval_get(ZONESVRENV* pstEnv, Player *pstPlayer, PworldTypeSparta *pstPworldSparta)
{
	int iFightVal;

	//用一个错误的值取必输
	iFightVal = sparta_end_fight_val(pstEnv, pstPlayer->stRoleData.MiscInfo.TeamID, pstPworldSparta, 3)-20;
	if(pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.FightVal +  iFightVal > MAX_SPARTA_FIGHT_VAL)
	{
		iFightVal = MAX_SPARTA_FIGHT_VAL - pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.FightVal;
	}

	if(pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.FightVal + iFightVal < MIN_SPARTA_FIGHT_VAL)
	{
		iFightVal = MIN_SPARTA_FIGHT_VAL- pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.FightVal;
	}

	return iFightVal;
}

int player_sparta_runaway_info_chg(ZONESVRENV* pstEnv, Player *pstPlayer, PworldTypeSparta *pstPworldSparta)
{
	int iVal = player_sparta_runaway_fightval_get(pstEnv, pstPlayer, pstPworldSparta);
	
	pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.FightVal += iVal;
	pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.RunAway++;
	pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.VictoryCurr = 0;
	pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.DefeatCurr = 0;
	pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.Total++;

	player_sparta_info(pstEnv, pstPlayer);			
	return 0;
}

int sparta_fight_award_glory_get(ZONESVRENV* pstEnv, Player *pstPlayer, int iVCamp)
{
	int iAdd = 0;
	SPARTAMISCINFO *pstSpartaInfo = &pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo;

	if(!IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstSpartaInfo->DayTime))
	{
		pstSpartaInfo->DayTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstSpartaInfo->DayGlory = 0;
	}

	if(pstPlayer->stOnline.stWarOL.cCampIdx == iVCamp)
	{
		iAdd = 40;
	}
	else
	{
		iAdd = 20;
	}

	if(pstSpartaInfo->DayGlory + iAdd > 300)
	{
		iAdd = 300 - pstSpartaInfo->DayGlory;
	}

	pstSpartaInfo->DayGlory += iAdd;
	return iAdd;
}

int sparta_pworld_tick(ZONESVRENV * pstEnv, ZONEPWORLD *pstPworld, ZONEPWORLDINST *pstPworldInst, int iSec)
{	
	int i, iCount, iVCamp = 0;
	Player *pstPlayer;
	PworldTypeSparta *pstPworldSparta = &pstPworldInst->stPworldTypeData.stSparta;
	
	if(iSec % 5 != 0)
	{
		return 0;
	}

	iCount = 0;
	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		pstPlayer = player_get_by_uin(pstEnv, pstPworldSparta->stTeam1.astMem[i].iUin);
		if(!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			iCount++;
		}
	}

	if(iCount == MAX_SPARTA_TEAM_MEM)
	{
		iVCamp = 2;
		goto _error;
	}

	iCount = 0;
	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		pstPlayer = player_get_by_uin(pstEnv, pstPworldSparta->stTeam2.astMem[i].iUin);
		if(!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			iCount++;
		}
	}

	if(iCount == MAX_SPARTA_TEAM_MEM)
	{
		iVCamp = 1;
		goto _error;
	}

	return 0;

_error:
	sparta_fight_info_update(pstEnv, pstPworldInst, NULL, 1, iVCamp);
	return 0;
}

int sparta_fight_info_update(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, Player *pstEnterPlayer, int fEnd, int iVCamp)
{
	int i, fGlory = 0, iCount =0, iFightVal = 0;
	Player *pstPlayer, *apstPlayer[MAX_SPARTA_ROLE];
	PworldTypeSparta *pstPworldSparta = &pstPworldInst->stPworldTypeData.stSparta;
	CSPKG stPkg;
	CSSPARTASVR *pstSpartaSvr = &stPkg.Body.SpartaSvr;
	CSSPARTAFIGHTINFO *pstFightInfo = &pstSpartaSvr->Data.FightInfo;

	if(pstPworldInst->bOver)
	{
		return -1;
	}
	
	memset(pstFightInfo, 0, sizeof(*pstFightInfo));

	pstFightInfo->Team1ID = pstPworldSparta->stTeam1.ullTeamID;
	pstFightInfo->Team1Kill = pstPworldSparta->iTeam1Kill;
	pstFightInfo->Team2ID = pstPworldSparta->stTeam2.ullTeamID;
	pstFightInfo->Team2Kill = pstPworldSparta->iTeam2Kill;

	if(fEnd)
	{
		pstFightInfo->End = 1;
		if(iVCamp > 0)
		{
			pstFightInfo->VCamp = iVCamp;
		}
		else if(pstFightInfo->Team1Kill > pstFightInfo->Team2Kill)
		{
			pstFightInfo->VCamp = 1;
		}
		else if(pstFightInfo->Team2Kill > pstFightInfo->Team1Kill)
		{
			pstFightInfo->VCamp = 2;
		}
		else
		{
			pstFightInfo->VCamp = 0;
		}
	}
	else if(pstPworldSparta->iTeam1Kill >= SPARTA_WIN_KILL_NUM||
		pstPworldSparta->iTeam2Kill >= SPARTA_WIN_KILL_NUM)
	{
		pstFightInfo->End = 1;
		if(pstPworldSparta->iTeam1Kill >= SPARTA_WIN_KILL_NUM)
		{
			pstFightInfo->VCamp = 1;
		}
		else if(pstPworldSparta->iTeam2Kill >= SPARTA_WIN_KILL_NUM)
		{
			pstFightInfo->VCamp = 2;
		}
		else
		{
			pstFightInfo->VCamp = 0;
		}
	}

	if(pstFightInfo->End == 1)
	{
		//停止副本刷怪等。
		z_pworld_over(pstEnv, pstPworldInst);
		pstPworldInst->tEnd = pstEnv->pstAppCtx->stCurr.tv_sec+7;

		if(is_in_add_sparta_glory_time(pstEnv))
		{
			fGlory = 1;
		}
	}

	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		STRNCPY(pstFightInfo->EndInfo.RoleInfo[i].RoleName, 
			pstPworldSparta->stTeam1.astMem[i].szRoleName, CCH(pstFightInfo->EndInfo.RoleInfo[0].RoleName));
		
		pstFightInfo->EndInfo.RoleInfo[i].Camp = 1;
		pstFightInfo->EndInfo.RoleInfo[i].FightVal = pstPworldSparta->stTeam1.astMem[i].iVal;
		
		pstPlayer = player_get_by_uin(pstEnv, pstPworldSparta->stTeam1.astMem[i].iUin);
		if(!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			if(pstFightInfo->End)
			{
				pstFightInfo->EndInfo.RoleInfo[i].Leave = 1;
				pstFightInfo->EndInfo.RoleInfo[i].ChgFightVal = sparta_leave_role_chg_fightval_get(pstEnv, pstPworldSparta, 
					&pstPworldSparta->stTeam1.astMem[i], pstPworldSparta->stTeam1.ullTeamID);
			}
			continue;
		}
		else
		{
			apstPlayer[iCount++] = pstPlayer;
			if(pstFightInfo->End)
			{
				iFightVal = player_sparta_end_award(pstEnv, pstPlayer, pstPworldSparta, pstFightInfo->VCamp);
				pstFightInfo->EndInfo.RoleInfo[i].ChgFightVal = iFightVal;
				if(fGlory)
				{	
					pstFightInfo->EndInfo.RoleInfo[i].SpartaGlory = sparta_fight_award_glory_get(pstEnv, pstPlayer, pstFightInfo->VCamp);
					if(pstFightInfo->EndInfo.RoleInfo[i].SpartaGlory > 0)
					{
						package_add_money(pstEnv, pstPlayer, ATTR_ID_SPARTA_GLORY, pstFightInfo->EndInfo.RoleInfo[i].SpartaGlory);
					}
				}
			}
		}
	}

	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		STRNCPY(pstFightInfo->EndInfo.RoleInfo[MAX_SPARTA_TEAM_MEM+i].RoleName, 
			pstPworldSparta->stTeam2.astMem[i].szRoleName, CCH(pstFightInfo->EndInfo.RoleInfo[0].RoleName));
		
		pstFightInfo->EndInfo.RoleInfo[MAX_SPARTA_TEAM_MEM+i].Camp = 2;
		pstFightInfo->EndInfo.RoleInfo[MAX_SPARTA_TEAM_MEM+i].FightVal = pstPworldSparta->stTeam2.astMem[i].iVal;
		
		pstPlayer = player_get_by_uin(pstEnv, pstPworldSparta->stTeam2.astMem[i].iUin);
		if(!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			if(pstFightInfo->End)
			{
				pstFightInfo->EndInfo.RoleInfo[MAX_SPARTA_TEAM_MEM+i].Leave = 1;
				pstFightInfo->EndInfo.RoleInfo[MAX_SPARTA_TEAM_MEM+i].ChgFightVal = sparta_leave_role_chg_fightval_get(pstEnv, pstPworldSparta, 
					&pstPworldSparta->stTeam2.astMem[i], pstPworldSparta->stTeam2.ullTeamID);
			}
			continue;
		}
		else
		{
			if(pstFightInfo->End)
			{
				iFightVal = player_sparta_end_award(pstEnv, pstPlayer, pstPworldSparta, pstFightInfo->VCamp);
				pstFightInfo->EndInfo.RoleInfo[MAX_SPARTA_TEAM_MEM+i].ChgFightVal = iFightVal;

				if(fGlory)
				{
					pstFightInfo->EndInfo.RoleInfo[MAX_SPARTA_TEAM_MEM+i].SpartaGlory = sparta_fight_award_glory_get(pstEnv, pstPlayer, pstFightInfo->VCamp);
					if(pstFightInfo->EndInfo.RoleInfo[MAX_SPARTA_TEAM_MEM+i].SpartaGlory > 0)
					{
						package_add_money(pstEnv, pstPlayer, ATTR_ID_SPARTA_GLORY, pstFightInfo->EndInfo.RoleInfo[MAX_SPARTA_TEAM_MEM+i].SpartaGlory);
					}
				}
			}
			apstPlayer[iCount++] = pstPlayer;
		}
	}

	pstSpartaSvr->Type = SPARTA_SVR_FIGHT_INFO;
	Z_CSHEAD_INIT(&stPkg.Head, SPARTA_SVR);

	if(pstEnterPlayer && pstFightInfo->End == 0)
	{
		z_cltmsg_send(pstEnv, pstEnterPlayer, &stPkg);
	}
	else
	{
		z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iCount, &stPkg);
	}
	
	
	return 0;
}

int sparta_player_die(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstAtkPlayer)
{
	ZONEPWORLDINST *pstZonePworldInst;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	ZONEIDX stIdx;
	PworldTypeSparta *pstPworldSparta;
	ZONEPWORLD * pstPworld;
	//RESPOS stPos;
	
	if(!is_battle_sparta_svr(pstEnv))
	{
		return -1;
	}

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (NULL == pstZonePworldInst || pstZonePworldInst->bOver)
	{
		return -1;
	}

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
	if (!pstPworld)
	{
		return -1;
	}

	if (strong_is_span(pstPworld))
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	pstPworldSparta = &pstZonePworldInst->stPworldTypeData.stSparta;

	if(pstPlayer->stRoleData.MiscInfo.TeamID == pstPworldSparta->stTeam1.ullTeamID)
	{
		pstPworldSparta->iTeam2Kill++;
	}
	else
	{
		pstPworldSparta->iTeam1Kill++;
	}

/*
	//自动复活
	player_revive_svr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	
	sparta_player_pos_get(pstZonePworldInst, pstMapIdx, pstPlayer, &stPos);
	z_player_change_map(pstEnv->pstAppCtx,  pstEnv, pstPlayer, pstPlayer->stRoleData.Map, &stPos, RAND1(180), 0);

	z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_TIP, "当前比分%d :%d", 
		pstPworldSparta->iTeam1Kill, pstPworldSparta->iTeam2Kill);
*/

/*
	if(pstPworldSparta->iTeam1Kill >= SPARTA_WIN_KILL_NUM ||
		pstPworldSparta->iTeam2Kill >= SPARTA_WIN_KILL_NUM)
	{
		//end 
		z_in_destroy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstZonePworldInst);
	}
*/

	if(pstAtkPlayer)
	{
		char szName[ROLE_NAME_LEN], szAtkerName[ROLE_NAME_LEN];
	
		STRNCPY(szName, pstPlayer->stRoleData.RoleName, CCH(szName));
		battle_rolename_remove_prefix(szName);

		STRNCPY(szAtkerName, pstAtkPlayer->stRoleData.RoleName, CCH(szAtkerName));
		battle_rolename_remove_prefix(szAtkerName);
		
		z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE2, ZONEERR_BROADMAP326, 
			szAtkerName, szName);
	}

	sparta_fight_info_update(pstEnv, pstZonePworldInst, NULL, 0, 0);
	return 0;
}

int sparta_apply_team_clear(ZONESVRENV* pstEnv, SpartaQueueTeam *pstTeamInfo, int iIdx)
{
	int i;
	Player *pstPlayer;
	SpartaQueue *pstSpartaQueue = pstEnv->pstSpartaQueue;

	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		pstPlayer = player_get_by_uin(pstEnv, pstTeamInfo->astMem[i].iUin);
		if(!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
		{
			continue;
		}

		if(pstPlayer->stRoleData.RoleID != pstTeamInfo->astMem[i].ullRoleID)
		{
			continue;
		}

		//
		player_sparta_queue_clear_action(pstEnv, pstPlayer);
		player_span_exit_pworld_queue_req(pstEnv, pstPlayer);
		z_free_player(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	pstSpartaQueue->iApplyNum--;
	if(iIdx != pstSpartaQueue->iApplyNum)
	{
		pstSpartaQueue->astApplyQueue[iIdx] = pstSpartaQueue->astApplyQueue[pstSpartaQueue->iApplyNum];
	}
	
	return 0;
}

SpartaQueueTeam* sparta_apply_team_get(ZONESVRENV* pstEnv, tdr_ulonglong ullTeamID, int *piIdx)
{
	int i;
	SpartaQueue *pstSpartaQueue = pstEnv->pstSpartaQueue;

	for(i=0; i<pstSpartaQueue->iApplyNum; i++)
	{
		if(pstSpartaQueue->astApplyQueue[i].ullTeamID == ullTeamID)
		{
			if(piIdx)
			{
				*piIdx = i;
			}

			return &pstSpartaQueue->astApplyQueue[i];
		}
	}
	
	return NULL;
}

int sparta_role_logout_clean(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iIdx = -1;
	SpartaQueueTeam *pstSpartaTeam;
	SpartaWaitQueueInfo *pstSpartaWaitTeam;
	ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);

	pstSpartaTeam = sparta_apply_team_get(pstEnv, pstPlayer->stRoleData.MiscInfo.TeamID, &iIdx);
	if(pstSpartaTeam)
	{
		if(pstTeam)
		{
			char szName[ROLE_NAME_LEN];
	
			STRNCPY(szName, pstPlayer->stRoleData.RoleName, CCH(szName));
			battle_rolename_remove_prefix(szName);
		
			z_team_sys_strf2(pstEnv, pstTeam, SYS_MIDDLE2, 
				"%s已离开末世荣耀殿堂列队，请重新报名",
				szName);
		}
		
		sparta_apply_team_clear(pstEnv, pstSpartaTeam, iIdx);
		goto _final;
	}

	pstSpartaWaitTeam = sparta_wait_queue_get(pstEnv, pstPlayer, &iIdx);
	if(pstSpartaWaitTeam)
	{
		if(pstTeam)
		{
			char szName[ROLE_NAME_LEN];
	
			STRNCPY(szName, pstPlayer->stRoleData.RoleName, CCH(szName));
			battle_rolename_remove_prefix(szName);
			
			z_team_sys_strf2(pstEnv, pstTeam, SYS_MIDDLE2, 
				"%s已离开末世荣耀殿堂列队，请重新报名",
				szName);
		}
			
		sparta_invite_team_clear(pstEnv, pstSpartaWaitTeam, iIdx, 1);
		goto _final;
	}

	z_free_player(pstEnv->pstAppCtx, pstEnv, pstPlayer);
_final:
	return 0;
}

int player_sparta_ready(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iIdx;
	SpartaWaitQueueInfo *pstWaitQueue;
	SpartaWaitQueueTeamMem *pstMem;
	
	pstWaitQueue = sparta_wait_queue_get(pstEnv, pstPlayer, &iIdx);
	if(!pstWaitQueue)
	{
		return -1;
	}

	pstMem = sparta_wait_mem_get(pstWaitQueue, pstPlayer);
	if(!pstMem)
	{
		return -1;
	}

	if(pstMem->iReady)
	{
		goto _final;
	}

	pstMem->iReady = 1;

	//TODO: fast tick check
	if(0 == sparta_wait_queue_check(pstEnv, pstWaitQueue, iIdx))
	{
		return 0;
	}

_final:
	sparta_wait_queue_ready_action(pstEnv, pstWaitQueue);
	return 0;
}

int player_sparta_exit_apply_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSSPARTASVR *pstSpartaSvr = &stPkg.Body.SpartaSvr;

	pstSpartaSvr->Type = SPARTA_SVR_APPLY_INFO;
	pstSpartaSvr->Data.Apply.Succ = 0;
	Z_CSHEAD_INIT(&stPkg.Head, SPARTA_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_sparta_queue_clear_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSSPARTASVR *pstSpartaSvr = &stPkg.Body.SpartaSvr;

	pstSpartaSvr->Type = SPARTA_SVR_EXIT_QUEUE;
	pstSpartaSvr->Data.ExitQueue = 1;
	Z_CSHEAD_INIT(&stPkg.Head, SPARTA_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_sparta_exit_sparta_apply(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	SpanOL *pstSpanOnline;

	pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
	if(pstSpanOnline->stApply.iSpartaApply != SPAN_APPLY_ING)
	{
		return -1;
	}

	player_span_apply_queue_clean(pstEnv, pstPlayer);
	return 0;
}

int player_sparta_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSSPARTACLT *pstSpartaClt = &pstCsPkg->Body.SpartaClt;
	
	switch(pstSpartaClt->Type)
	{
		case SPARTA_CLT_READY:
			if(INST_MODE_ZONE_SVR == pstEnv->iInstMode)
			{	
				return zone_proxy_sparta_request(pstEnv, pstPlayer, pstCsPkg);
			}	
			else
			{
				return player_sparta_ready(pstEnv, pstPlayer);
			}
			break;
		case SPARTA_CLT_APPLY:
			return player_sparta_apply_req(pstEnv, pstPlayer);
			break;
		case SPARTA_CLT_EXIT_APPLY:
			return player_sparta_exit_sparta_apply(pstEnv, pstPlayer);
			break;
		default:
			break;
	}

	UNUSED(pstAppCtx);
	UNUSED(pstFrameHead);
	return 0;
}

int player_sparta_team_chg_clean(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if(pstPlayer->stOnline.stSpanOnline.stApply.iSpartaApply == SPAN_APPLY_ING)
	{
		player_span_role_clean(pstEnv, pstPlayer->stRoleData.Uin, pstPlayer->stRoleData.RoleID, pstEnv->iBattleSpartaID);
	}
	
	return 0;
}

PworldTypeSpartaTeamMem* sparta_pworld_mem_info_get(PworldTypeSparta *pstPworldSparta, Player *pstPlayer)
{
	int i;

	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		if(pstPworldSparta->stTeam1.astMem[i].ullRoleID == pstPlayer->stRoleData.RoleID)
		{
			return &pstPworldSparta->stTeam1.astMem[i];
		}
	}

	for(i=0; i<MAX_SPARTA_TEAM_MEM; i++)
	{
		if(pstPworldSparta->stTeam2.astMem[i].ullRoleID == pstPlayer->stRoleData.RoleID)
		{
			return &pstPworldSparta->stTeam2.astMem[i];
		}
	}
	
	return 0;
}

int player_leave_sparta(ZONESVRENV* pstEnv, ZONEPWORLDINST* pstPworldInst, Player *pstPlayer)
{
	PworldTypeSparta *pstPworldSparta = &pstPworldInst->stPworldTypeData.stSparta;
	
	if(!is_battle_sparta_svr(pstEnv))
	{
		return 0;
	}

	if(pstPworldInst->bOver)
	{
		return 0;
	}

	player_sparta_runaway_info_chg(pstEnv, pstPlayer, pstPworldSparta);
	return 0;
}

int player_sparta_victory_rate(Player *pstPlayer)
{
	int iSum = pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.Victory+ 
		pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.Defeat + pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.RunAway;

	if(iSum <= 0)
	{
		return 0;
	}

	return pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.Victory*100/iSum;
}

int player_sparta_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSSPARTASVR *pstSpartaSvr = &stPkg.Body.SpartaSvr;

	pstSpartaSvr->Type = SPARTA_SVR_ROLE_SPARTA_INFO;
	pstSpartaSvr->Data.SprataInfo.FightVal = pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.FightVal;
	pstSpartaSvr->Data.SprataInfo.Victory = pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.Victory;
	pstSpartaSvr->Data.SprataInfo.RunAway = pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.RunAway;
	pstSpartaSvr->Data.SprataInfo.Defeat = pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.Defeat;
	
	Z_CSHEAD_INIT(&stPkg.Head, SPARTA_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

//竞技场奖励
int sparta_player_award(ZONESVRENV* pstEnv, char *pszName, int iRange)
{
	int i, iGridNum = 0;
	char szMsg[1024];
	ROLEGRID astGrid[MAX_SPARTA_AWARD_ITEM];
	SPARTAAWARDDEF *pstAwardDef;

	pstAwardDef = z_find_sparta_award_def(pstEnv, iRange);
	if(!pstAwardDef)
	{
		return -1;
	}

	for(i=0; i<=MAX_SPARTA_AWARD_ITEM-1; i++)
	{
		if(pstAwardDef->AwardItem[i].ItemID == 0)
		{
			continue;
		}

		if(0 > item_create(pstEnv->pstAppCtx, pstEnv, &astGrid[iGridNum], pstAwardDef->AwardItem[i].ItemID))
		{
			continue;
		}

		if(pstAwardDef->AwardItem[i].Bind)
		{
			astGrid[iGridNum].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
		}

		if(astGrid[iGridNum].Type == GRID_ITEM_ITEM)
		{
			astGrid[iGridNum].GridData.RoleItem.Num = pstAwardDef->AwardItem[i].Num;
		}

		iGridNum++;
	}

	if(iGridNum <= 0)
	{
		return 0;
	}

	snprintf(szMsg, sizeof(szMsg), 
			LJY_MAIL52, 
			pszName, iRange);
	player_sysmail_fast_send(pstEnv, pszName, LJY_MAIL53, szMsg, astGrid, iGridNum, 0, NULL, 0);
	return 0;
}

 int is_same_real_week(time_t t1, time_t t2)
{
	int iNowWeekDay;
	int iStartWeekDay;
	struct tm stNow;
	struct tm stStart;
	time_t tNowTmp, tStartTmp;
	
	localtime_r(&t2 , &stNow);
	localtime_r(&t1, &stStart);

	iStartWeekDay = stStart.tm_wday;
	if ( stStart.tm_wday == 0 )
	{
		iStartWeekDay = 7;
	}
	//一周的开始那一天
	tStartTmp = t1 - (iStartWeekDay-1)*24*60*60;

	iNowWeekDay = stNow.tm_wday;
	if ( stNow.tm_wday == 0 )
	{
		iNowWeekDay = 7;
	}
	//一周的开始那一天
	tNowTmp = t2  - (iNowWeekDay-1)*24*60*60;
	
	//同一天则同一个星期内
	tNowTmp += 6*3600;
	tStartTmp += 6*3600;
	
	if (IsSameDay(tNowTmp, tStartTmp))
	{
		return 1;
	}

	return 0;
}

int sparta_login_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	SPARTAMISCINFO *pstSpartaInfo = &pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	int iTmp = iCurr;
	int iWeek = 0;
	int i;
	RangeOneMem * pstOne;

	if (pstPlayer->stRoleData.LastLogout <= 0)
		goto jump;

	pstOne = range_find_onemem(pstEnv,RANGE_MAIN_SPARTA, RANGE_MAIN_SPARTA_SUB_FIGHT);
	if (!pstOne)
	{
		goto jump;
	}

	for (i=0; i<pstOne->stLastRange.iNum; i++)
	{
		if (pstOne->stLastRange.astRangeEntry[i].stRange.RangeData.GID == pstPlayer->stRoleData.RoleID)
		{
			pstSpartaInfo->FightVal = pstOne->stLastRange.astRangeEntry[i].stRange.RangeData.RangeVal;
			break;
		}
	}

	if (i < pstOne->stLastRange.iNum)
	{
		goto jump;
	}

	while(iTmp > (int)pstPlayer->stRoleData.LastLogout )
	{
		if (!is_same_real_week(iTmp, pstPlayer->stRoleData.LastLogout))
		{
			iWeek ++;
		}
		else
		{
			break;
		}

		iTmp -= 7*24*60*60;
	}

	if (iWeek > 0)
	{
		if (pstSpartaInfo->TotalWeek > 10)
		{
			iWeek --;
		}
	}

	if (pstSpartaInfo->FightVal > INIT_SPARTA_FIGHT_VAL+iWeek*100)
	{
		pstSpartaInfo->FightVal -= iWeek*100;
	}
	else
	{
		pstSpartaInfo->FightVal = INIT_SPARTA_FIGHT_VAL;
	}

jump:
	if ( !is_same_real_week(iCurr, pstSpartaInfo->WeekTime))
	{
		pstSpartaInfo->WeekTime = iCurr;
		pstSpartaInfo->TotalWeek = 0;
	}

	role_login_sparta(pstEnv, pstPlayer);
	return 0;
}

int sparta_range_check(ZONESVRENV* pstEnv)
{
	int i;
	TOPRANGE *pstRange;
	RangeOne *pstRangeOne;
	RangeOneMem *pstRangeOneMem;
	GLOBAL *pstGlobal = &pstEnv->pstGlobalObj->stGlobal;
	
	pstRangeOneMem = range_find_onemem(pstEnv, RANGE_MAIN_SPARTA, RANGE_MAIN_SPARTA_SUB_FIGHT);
	if (NULL == pstRangeOneMem)
	{
		return -1;
	}
	
	pstRangeOne = &pstRangeOneMem->stLastRange;

	if(pstRangeOne->iNum <=0)
	{
		return -1;
	}

	if(pstGlobal->Misc.GlobalSparta.AwardTime == 0)
	{
		pstGlobal->Misc.GlobalSparta.AwardTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		return 0;
	}

	if(is_same_real_week(pstGlobal->Misc.GlobalSparta.AwardTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		return 0;
	}

	for (i=0; i<pstRangeOne->iNum; i++)
	{
		pstRange = &pstRangeOne->astRangeEntry[i].stRange;	
		sparta_player_award(pstEnv, pstRange->RangeData.FirstName, i+1);
	}

	pstGlobal->Misc.GlobalSparta.AwardTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	return 0;
}

int role_login_sparta(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if(pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.FightVal == 0)
	{
		pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.FightVal = INIT_SPARTA_FIGHT_VAL;
	}

	player_sparta_info(pstEnv, pstPlayer);
	return 0;
}
/*
int sparta_designation_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	DESIGNATIONENTRY *pstDsn;
	SPARTADESIGNATIONDEF *pstSpartaDesignationDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	SPARTAMISCINFO *pstSpartaInfo = &pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo;
	ROLEDESIGNATIONS *pstInfo = &pstPlayer->stRoleData.MiscInfo.DesignationInfo;

	for(i=pstInfo->Num-1; i>=0; i--)
	{
		pstSpartaDesignationDef = z_find_sparta_designation_def(pstEnv, pstInfo->Designations[i].ID);
		if(!pstSpartaDesignationDef)
		{
			continue;
		}

		if(pstSpartaDesignationDef->FightVal > pstSpartaInfo->FightVal)
		{
			player_designation_del(pstEnv, pstPlayer, pstInfo->Designations[i].ID);
		}
	}

	for(i=0; i<pstObj->iSpartaDesignationDef; i++)
	{
		pstSpartaDesignationDef = (SPARTADESIGNATIONDEF *)pstObj->sSpartaDesignationDef[i];

		if(pstSpartaDesignationDef->FightVal > pstSpartaInfo->FightVal)
		{
			continue;
		}

		pstDsn = player_find_designation(pstEnv, pstPlayer, pstSpartaDesignationDef->ID);
		if(pstDsn)
		{
			continue;
		}

		player_designation_add(pstEnv,pstPlayer, pstSpartaDesignationDef->ID);
	}
	
	return 0;
}
*/

int player_sparta_runaway_clear(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	SPARTAMISCINFO *pstSpartaInfo = &pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo;

	pstSpartaInfo->RunAway = 0;
	player_sparta_info(pstEnv, pstPlayer);
	return 0;
}

int player_sparta_all_clear(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	SPARTAMISCINFO *pstSpartaInfo = &pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo;

	pstSpartaInfo->RunAway = 0;
	pstSpartaInfo->Victory = 0;
	pstSpartaInfo->Defeat = 0;
	pstSpartaInfo->Total = 0;
	player_sparta_info(pstEnv, pstPlayer);
	return 0;
}

