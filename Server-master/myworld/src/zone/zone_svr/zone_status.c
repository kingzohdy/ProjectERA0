/*
**  @file $RCSfile: zone_status.c,v $
**  general description of this module
**  $Id: zone_status.c,v 1.323 2014/07/01 07:14:43 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/07/01 07:14:43 $
**  @version $Revision: 1.323 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_status.h"
#include "zone_skill.h"
#include "zone_clt.h"
#include "zone_move.h"
#include "zone_attr.h"
#include "zone_combat.h"
#include "zone_team.h"
#include "zone_ai.h"
#include "zone_package.h"
#include "zone_pet.h"
#include "zone_player.h"
#include "zone_tmp.h"
#include "zone_npc.h"
#include "zone_map.h"
#include "zone_misc.h"
#include "zone_booty.h"
#include "zone_machine.h"
#include "zone_machine_view.h"
#include "zone_oplog.h"
#include "zone_clan_city.h"
#include "zone_clan.h"
#include "zone_err.h"

int iStatusSeq;

typedef struct tagDelStatus
{
	unsigned short unStatusID;
	unsigned short unLevel;
}DelStatus;

int z_player_apply_status2(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iStatus, int iStatusLevel, int iLast, int iNotify, int iApplyID,ZoneStatus** ppstZoneStatus);


STATUSDEF *z_find_status_zonestatus(ZONESVRENV* pstEnv, ZoneStatus *pstZoneStatus)
{
	STATUSDEF *pstStatusDef;
	STATUSDEF stStatusDef;
	int iStatusIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	if (pstZoneStatus->nDefIdx >= 0 && pstZoneStatus->nDefIdx < pstObj->iStatusDef)
	{
		pstStatusDef = (STATUSDEF *)pstObj->sStatusDef[pstZoneStatus->nDefIdx];
		if (pstStatusDef->StatusID == pstZoneStatus->unStatusID && pstStatusDef->Level == pstZoneStatus->bLevel)
		{
			return pstStatusDef;
		}
	}

	stStatusDef.StatusID = pstZoneStatus->unStatusID;
	stStatusDef.Level = pstZoneStatus->bLevel;
	iStatusIndex = bsearch_int(&stStatusDef, pstObj->sStatusDef, pstObj->iStatusDef, STATUS_DATA, &iEqu);
	if (iEqu)
	{
		pstZoneStatus->nDefIdx = iStatusIndex;
		return (STATUSDEF *)&pstObj->sStatusDef[iStatusIndex];
	}

	return NULL;
}

STATUSDEF *z_find_status_petstatus(ZONESVRENV* pstEnv, ZoneStatus *pstZoneStatus)
{
	STATUSDEF *pstStatusDef;
	STATUSDEF stStatusDef;
	int iStatusIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	if (pstZoneStatus->nDefIdx >= 0 && pstZoneStatus->nDefIdx < pstObj->iStatusDef)
	{
		pstStatusDef = (STATUSDEF *)pstObj->sStatusDef[pstZoneStatus->nDefIdx];
		if (pstStatusDef->StatusID == pstZoneStatus->unStatusID && pstStatusDef->Level == pstZoneStatus->bLevel)
		{
			return pstStatusDef;
		}
	}

	stStatusDef.StatusID = pstZoneStatus->unStatusID;
	stStatusDef.Level = pstZoneStatus->bLevel;
	iStatusIndex = bsearch_int(&stStatusDef, pstObj->sStatusDef, pstObj->iStatusDef, STATUS_DATA, &iEqu);
	if (iEqu)
	{
		pstZoneStatus->nDefIdx = iStatusIndex;
		return (STATUSDEF *)&pstObj->sStatusDef[iStatusIndex];
	}

	return NULL;
}

STATUSDEF *z_find_status(ZONESVRENV* pstEnv, int iStatusID, int iLevel)
{
	STATUSDEF stStatusDef;
	int iStatusIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stStatusDef.StatusID = iStatusID;
	stStatusDef.Level = iLevel;
	iStatusIndex = bsearch_int(&stStatusDef, pstObj->sStatusDef, pstObj->iStatusDef, STATUS_DATA, &iEqu);
	if (iEqu)
	{
		return (STATUSDEF *)&pstObj->sStatusDef[iStatusIndex];
	}

	return NULL;
}

int z_overlaytype_val(ZONESVRENV* pstEnv, STATUSDEF *pstStatusDef1, STATUSDEF *pstStatusDef2)
{
	int iEqu;
	int iIndex;
	OVERLAYTYPERELATIONDEF stOverlayTypeRelationDef;
	OVERLAYTYPERELATIONDEF *pstDef = NULL;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	int iRet;

	if (pstStatusDef1->StatusID == pstStatusDef2->StatusID)
	{
		iRet = OVERLAY_FLAG_SAME_REPLACE;
	}
	else
	{
		iRet = OVERLAY_FLAG_COEXIST;
	}

	stOverlayTypeRelationDef.OverlayType1 = pstStatusDef1->OverlayType;
	stOverlayTypeRelationDef.OverlayType2 = pstStatusDef2->OverlayType;

	iIndex = bsearch_int(&stOverlayTypeRelationDef, pstObj->sOverlayTypeRelationDef, pstObj->iOverlayTypeRelationDef, OVERLAYTYPE_RELATION_DATA, &iEqu);
	if (iEqu)
	{
		pstDef = (OVERLAYTYPERELATIONDEF *)pstObj->sOverlayTypeRelationDef[iIndex];
	}

	if (pstDef)
	{
		return pstDef->OverlayFlag;
	}

	return iRet;
}

STATUSOVERLAYDEF *z_find_status_overlay(ZONESVRENV* pstEnv, int iStatusID, int iLevel, int iOverlayNum)
{
	STATUSOVERLAYDEF stStatusOverDef;
	int iStatusOverlayIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stStatusOverDef.StatusID = iStatusID;
	stStatusOverDef.Level = iLevel;
	stStatusOverDef.Overlay = iOverlayNum;
	
	iStatusOverlayIndex = bsearch_int(&stStatusOverDef, pstObj->sStatusOverlayDef, pstObj->iStatusOverlayDef, STATUS_OVERLAY_DATA, &iEqu);
	if (iEqu)
	{
		return (STATUSOVERLAYDEF *)&pstObj->sStatusOverlayDef[iStatusOverlayIndex];
	}

	return NULL;
}

STATUSRESULT *z_get_status_result(STATUSDEF *pstStatusDef, int iResultID)
{
	int i;

	for (i=0; i<MAX_STATUS_RESULT; i++) 
	{
		if (0 == pstStatusDef->Result[i].ResultID)
		{
			break;
		}

		if (iResultID == pstStatusDef->Result[i].ResultID)
		{
			return pstStatusDef->Result+i;
		}
	}

	return NULL;
}

void z_init_zonestatus(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZoneStatus *pstZoneStatus, 
					STATUSDEF *pstStatusDef, int iLast, int iOverlay, int  iApplyID,ZoneAni *pstAni)
{
	STATUSOVERLAYDEF *pstStatusOverlay;
	STATUSRESULT *pstStatusResult = NULL;
	Player* pstPlayer = NULL;
	DYNATTRLIST *pstDyn = NULL;
	Monster* pstMon;

	memset(pstZoneStatus, 0, sizeof(*pstZoneStatus));

	pstZoneStatus->unStatusID = pstStatusDef->StatusID;
	pstZoneStatus->bLevel = pstStatusDef->Level;
	pstZoneStatus->unOverlay = iOverlay;

	pstZoneStatus->nOverlayMul = 0;
	if (iOverlay > 0)
	{
		pstStatusOverlay = z_find_status_overlay(pstEnv, pstStatusDef->StatusID, pstStatusDef->Level, iOverlay);
		if (pstStatusOverlay)
		{
			pstZoneStatus->nOverlayMul = pstStatusOverlay->Mul;
		}
	}

	pstZoneStatus->iLast = iLast;
	pstZoneStatus->stEnd.tv_sec = pstAppCtx->stCurr.tv_sec + iLast;
	pstZoneStatus->stEnd.tv_usec = pstAppCtx->stCurr.tv_usec;
	pstZoneStatus->nDefIdx = -1;
	pstZoneStatus->bType = pstStatusDef->Type;
	pstZoneStatus->bSubType = pstStatusDef->SubType;
	pstZoneStatus->unBindSkillID = pstStatusDef->BindSkill.SkillID;
	pstZoneStatus->bBindSkillLevel = pstStatusDef->BindSkill.Level;
	pstZoneStatus->bBindSkillTrig = pstStatusDef->BindSkill.Trig;
	pstZoneStatus->nRelationDefIdx = -1;
	pstZoneStatus->iApplyID = iApplyID;
	pstZoneStatus->tLastStatusCheck = pstAppCtx->stCurr.tv_sec;
	pstZoneStatus->iJumpedCount = 0;
	pstZoneStatus->iSeqID = iStatusSeq++;
	pstZoneStatus->iByAtk = pstStatusDef->ByAtkClear;
	
	//暂时按3秒跳一次数据
	if(pstStatusDef->StatusInterVal)
	{
		pstZoneStatus->iJumpNum = iLast/pstStatusDef->StatusInterVal;
	}
	else
	{
		pstZoneStatus->iJumpNum = iLast/3;
	}
	if(pstZoneStatus->iJumpNum<=0)
	{
		pstZoneStatus->iJumpNum = 1;
	}

	// 叠加状态值的初始化,不做修正处理
	memset(&pstZoneStatus->ResultStepDecSpd,0,sizeof(STATUSRESULT));
	memset(&pstZoneStatus->ResultStepDecHp,0,sizeof(STATUSRESULT));

	/*
	pstStatusResult = z_get_status_result(pstStatusDef, STATUS_RESULT_STEP_DEC_MSPD);
	if(NULL != pstStatusResult)
	{
		pstZoneStatus->ResultStepDecSpd.ResultID = pstStatusResult->ResultID;
		pstZoneStatus->ResultStepDecSpd.ResultVal1 = pstStatusResult->ResultVal1;
		pstZoneStatus->ResultStepDecSpd.ResultVal2 = pstStatusResult->ResultVal2;
		pstZoneStatus->ResultStepDecSpd.ResultVal3 = pstStatusResult->ResultVal3;
		pstZoneStatus->ResultStepDecSpd.ResultVal4 = pstStatusResult->ResultVal4;
	}
	pstStatusResult = z_get_status_result(pstStatusDef, STATUS_RESULT_STEP_DEC_HP);
	if(NULL != pstStatusResult)
	{
		pstZoneStatus->ResultStepDecHp.ResultID = pstStatusResult->ResultID;
		pstZoneStatus->ResultStepDecHp.ResultVal1 = pstStatusResult->ResultVal1;
		pstZoneStatus->ResultStepDecHp.ResultVal2 = pstStatusResult->ResultVal2;
		pstZoneStatus->ResultStepDecHp.ResultVal3 = pstStatusResult->ResultVal3;
		pstZoneStatus->ResultStepDecHp.ResultVal4 = pstStatusResult->ResultVal4;
	}*/

	pstStatusResult = z_get_status_result(pstStatusDef, STATUS_RESULT_ABSORB_HURT);
	if(pstAni && pstStatusResult)
	{
		switch(pstAni->iType)
		{
			case OBJ_PLAYER:
				pstPlayer = pstAni->stObj.pstPlayer;
				pstDyn = &pstPlayer->stOnline.stDyn;
				pstZoneStatus->iVal = pstDyn->stMidAttrList.uiHPMax*pstStatusResult->ResultVal1/100 +pstStatusResult->ResultVal2;
				break;
			case OBJ_MONSTER:
				pstMon= pstAni->stObj.pstMon;
				pstZoneStatus->iVal = pstMon->iMaxHP*pstStatusResult->ResultVal1/100 +pstStatusResult->ResultVal2;
				break;
			case OBJ_PET:
				break;
			default:
				return ;
				break;
		}
	}
	
	return;
}

void z_init_pet_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZoneStatus *pstZoneStatus, STATUSDEF *pstStatusDef, int iLast, int iOverlay)
{
	STATUSOVERLAYDEF *pstStatusOverlay;
	
	memset(pstZoneStatus, 0, sizeof(*pstZoneStatus));

	pstZoneStatus->unStatusID= pstStatusDef->StatusID;
	pstZoneStatus->bLevel= pstStatusDef->Level;
	pstZoneStatus->unOverlay= iOverlay;

	pstZoneStatus->nOverlayMul = 0;
	if (iOverlay > 0)
	{
		pstStatusOverlay = z_find_status_overlay(pstEnv, pstStatusDef->StatusID, pstStatusDef->Level, iOverlay);
		if (pstStatusOverlay)
		{
			pstZoneStatus->nOverlayMul = pstStatusOverlay->Mul;
		}
	}

	pstZoneStatus->iLast = iLast;
	pstZoneStatus->stEnd.tv_sec = pstAppCtx->stCurr.tv_sec + iLast;
	pstZoneStatus->stEnd.tv_usec = pstAppCtx->stCurr.tv_usec;
	pstZoneStatus->nDefIdx = -1;
	pstZoneStatus->bType = pstStatusDef->Type;
	pstZoneStatus->bSubType = pstStatusDef->SubType;
	pstZoneStatus->unBindSkillID = pstStatusDef->BindSkill.SkillID;
	pstZoneStatus->bBindSkillLevel = pstStatusDef->BindSkill.Level;
	pstZoneStatus->bBindSkillTrig = pstStatusDef->BindSkill.Trig;
	pstZoneStatus->nRelationDefIdx = -1;
	pstZoneStatus->iApplyID = 0;
	pstZoneStatus->tLastStatusCheck = pstAppCtx->stCurr.tv_sec;
	pstZoneStatus->iJumpedCount = 0;

	return;
}

int z_status_fill( TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSSTATUSINFO *pstCSStatusInfo)
{
	int i;
	STATUSINFO *pstStatusInfo = &pstPlayer->stRoleData.StatusInfo;

	UNUSED( pstAppCtx );
	UNUSED( pstEnv );
	
	pstCSStatusInfo->GoodStatusNum = 0;
	for (i=0; i<pstStatusInfo->GoodStatusNum; i++)
	{
		pstCSStatusInfo->GoodStatusList[i].StatusID = pstStatusInfo->GoodStatusList[i].StatusID;
		pstCSStatusInfo->GoodStatusList[i].Level = pstStatusInfo->GoodStatusList[i].Level;
		pstCSStatusInfo->GoodStatusList[i].Time = pstStatusInfo->GoodStatusList[i].Time;
		pstCSStatusInfo->GoodStatusList[i].Overlay = pstStatusInfo->GoodStatusList[i].Overlay;
		pstCSStatusInfo->GoodStatusNum++;	
	}

	pstCSStatusInfo->BadStatusNum = 0;
	for (i=0; i<pstStatusInfo->BadStatusNum; i++)
	{
		pstCSStatusInfo->BadStatusList[i].StatusID = pstStatusInfo->BadStatusList[i].StatusID;
		pstCSStatusInfo->BadStatusList[i].Level = pstStatusInfo->BadStatusList[i].Level;
		pstCSStatusInfo->BadStatusList[i].Time = pstStatusInfo->BadStatusList[i].Time;
		pstCSStatusInfo->BadStatusList[i].Overlay = pstStatusInfo->BadStatusList[i].Overlay;
		pstCSStatusInfo->BadStatusNum++;
	}

	pstCSStatusInfo->NoLimitStatusNum = 0;
	for (i=0; i<pstStatusInfo->NoLimitStatusNum; i++)
	{
		pstCSStatusInfo->NolimitStatusList[i].StatusID = pstStatusInfo->NolimitStatusList[i].StatusID;
		pstCSStatusInfo->NolimitStatusList[i].Level = pstStatusInfo->NolimitStatusList[i].Level;
		pstCSStatusInfo->NolimitStatusList[i].Time = pstStatusInfo->NolimitStatusList[i].Time;
		pstCSStatusInfo->NolimitStatusList[i].Overlay = pstStatusInfo->NolimitStatusList[i].Overlay;
		pstCSStatusInfo->NoLimitStatusNum++;
	}

	return 0;
}
// 计算下线状态时
int z_status_offline_calulate_time(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,Player *pstPlayer, ZoneStatus *pstZoneStatus, STATUSDEF *pstStatusDef, int iLast)
{	
	int iOffTime = 0;
	UNUSED(pstEnv);
	
	if( !(pstStatusDef->StatusFlag & STATUS_OFFLINE_CALCULATE_TIME) )
	{
		return 0;
	}
	iOffTime = pstAppCtx->stCurr.tv_sec - pstPlayer->stRoleData.LastLogout;
	if( iOffTime < 0 )
	{
		iOffTime = 0;
	}
	if( iOffTime >= iLast )
	{
		pstZoneStatus->stEnd.tv_sec = pstAppCtx->stCurr.tv_sec;
		pstZoneStatus->stEnd.tv_usec = pstAppCtx->stCurr.tv_usec;
	}
	else
	{
		pstZoneStatus->stEnd.tv_sec = pstAppCtx->stCurr.tv_sec + iLast - iOffTime;
		pstZoneStatus->stEnd.tv_usec = pstAppCtx->stCurr.tv_usec;	
	}
	
	return 0;	
}

// iPosFlag 从哪个状态恢复状态
// iPosFlag为1时  表示退出副本恢复状态
int z_status_online(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer , int iPosFlag)
{
	int i;
	STATUSDEF *pstStatusDef;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	STATUSINFO *pstStatusInfo ;//= &pstPlayer->stRoleData.StatusInfo;
	ZoneAni stAni;

	stAni.iType = OBJ_PLAYER;
	stAni.stObj.pstPlayer = pstPlayer;
	if (iPosFlag == 0)
	{
		pstStatusInfo = &pstPlayer->stRoleData.StatusInfo;
	}
	else
	{
		pstStatusInfo = &pstPlayer->stRoleData.MiscInfo.StatusInfo;
	}

	pstStatusList->bGoodStatusNum = 0;
	for (i=0; i<pstStatusInfo->GoodStatusNum; i++)
	{
		pstStatusList->astGoodStatus[i].unStatusID = pstStatusInfo->GoodStatusList[i].StatusID;
		pstStatusList->astGoodStatus[i].bLevel = pstStatusInfo->GoodStatusList[i].Level;
		pstStatusList->astGoodStatus[i].nDefIdx = -1;
		pstStatusDef = z_find_status_zonestatus(pstEnv, &pstStatusList->astGoodStatus[i]);
		if (!pstStatusDef)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "status %d level %d no def", pstStatusList->astGoodStatus[i].unStatusID, pstStatusList->astGoodStatus[i].bLevel );
			TV_CLONE(pstStatusList->astGoodStatus[i].stEnd , pstAppCtx->stCurr);
			continue;
		}

		if(pstStatusDef && 
			(pstStatusDef->StatusFlag & STATUS_LOGOUT_CLEAN) && 
			iPosFlag && 
			pstPlayer->stOnline.iCleanStatuFlag == 1)
		{
			continue;
		}

		if (pstStatusDef && 
			(pstStatusDef->StatusFlag & STATUS_CHG_MAP_CLEAN) &&
			0 == iPosFlag)
		{
			continue;
		}

		z_init_zonestatus(pstAppCtx, pstEnv, &pstStatusList->astGoodStatus[i], pstStatusDef, 
			pstStatusInfo->GoodStatusList[i].Time, pstStatusInfo->GoodStatusList[i].Overlay, 0,&stAni);
		z_status_offline_calulate_time(pstAppCtx, pstEnv,pstPlayer, &pstStatusList->astGoodStatus[i], pstStatusDef, pstStatusInfo->GoodStatusList[i].Time);
		pstStatusList->astGoodStatus[i].iVal = pstStatusInfo->GoodStatusList[i].Val;
		pstStatusList->bGoodStatusNum++;
	}

	pstStatusList->bBadStatusNum = 0;
	for (i=0; i<pstStatusInfo->BadStatusNum; i++)
	{
		pstStatusList->astBadStatus[i].unStatusID = pstStatusInfo->BadStatusList[i].StatusID;
		pstStatusList->astBadStatus[i].bLevel = pstStatusInfo->BadStatusList[i].Level;
		pstStatusList->astBadStatus[i].nDefIdx = -1;
		pstStatusDef = z_find_status_zonestatus(pstEnv, &pstStatusList->astBadStatus[i]);
		if (!pstStatusDef)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "status %d level %d no def", pstStatusList->astBadStatus[i].unStatusID, pstStatusList->astBadStatus[i].bLevel );
			TV_CLONE(pstStatusList->astBadStatus[i].stEnd , pstAppCtx->stCurr);
			continue;
		}

		if(pstStatusDef && 
			(pstStatusDef->StatusFlag & STATUS_LOGOUT_CLEAN) && 
			iPosFlag && 
			pstPlayer->stOnline.iCleanStatuFlag == 1)
		{
			continue;
		}

		if (pstStatusDef && 
			(pstStatusDef->StatusFlag & STATUS_CHG_MAP_CLEAN) &&
			0 == iPosFlag)
		{
			continue;
		}

		z_init_zonestatus(pstAppCtx, pstEnv, &pstStatusList->astBadStatus[i], pstStatusDef, pstStatusInfo->BadStatusList[i].Time, 
			pstStatusInfo->BadStatusList[i].Overlay, 0,&stAni);
		z_status_offline_calulate_time(pstAppCtx, pstEnv,pstPlayer, &pstStatusList->astBadStatus[i], pstStatusDef, pstStatusInfo->BadStatusList[i].Time);
		pstStatusList->astBadStatus[i].iVal = pstStatusInfo->BadStatusList[i].Val;
		pstStatusList->bBadStatusNum++;
	}

	pstStatusList->bNoLimitStatusNum = 0;
	for (i=0; i<pstStatusInfo->NoLimitStatusNum; i++)
	{
		pstStatusList->astNoLimitStatus[i].unStatusID = pstStatusInfo->NolimitStatusList[i].StatusID;
		pstStatusList->astNoLimitStatus[i].bLevel = pstStatusInfo->NolimitStatusList[i].Level;
		pstStatusList->astNoLimitStatus[i].nDefIdx = -1;
		pstStatusDef = z_find_status_zonestatus(pstEnv, &pstStatusList->astNoLimitStatus[i]);
		if (!pstStatusDef)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "status %d level %d no def", pstStatusList->astNoLimitStatus[i].unStatusID, pstStatusList->astNoLimitStatus[i].bLevel );
			TV_CLONE(pstStatusList->astNoLimitStatus[i].stEnd , pstAppCtx->stCurr);
			continue;
		}

		if(pstStatusDef && 
			(pstStatusDef->StatusFlag & STATUS_LOGOUT_CLEAN) && 
			iPosFlag && 
			pstPlayer->stOnline.iCleanStatuFlag == 1)
		{
			continue;
		}

		if (pstStatusDef && 
			(pstStatusDef->StatusFlag & STATUS_CHG_MAP_CLEAN) &&
			0 == iPosFlag)
		{
			continue;
		}

		z_init_zonestatus(pstAppCtx, pstEnv, &pstStatusList->astNoLimitStatus[i], pstStatusDef, pstStatusInfo->NolimitStatusList[i].Time,
			pstStatusInfo->NolimitStatusList[i].Overlay, 0,&stAni);
		z_status_offline_calulate_time(pstAppCtx, pstEnv,pstPlayer, &pstStatusList->astNoLimitStatus[i], pstStatusDef, pstStatusInfo->NolimitStatusList[i].Time);
		pstStatusList->astNoLimitStatus[i].iVal = pstStatusInfo->NolimitStatusList[i].Val;
		pstStatusList->bNoLimitStatusNum++;
	}

	if (z_player_find_status(pstPlayer, STATUS_ID_QIANXIN) && iPosFlag)
	{
		player_status_qianxin(pstEnv, pstPlayer, 1);
	}
	return 0;
}

int z_status_grab(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer )
{
	int i;
	ZoneStatus *pstZoneStatus;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	STATUSDEF* pstStatusDef;

	for (i=pstStatusList->bGoodStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
	
		if (pstStatusDef )
		{
			StatusResultMap* pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
			
			if (IS_STATUS_RESULTMAP_SET(STATUS_RESULT_LUCKY_ATTENDEE, pstResultMap->sMap))
			{
				pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_LUCK;
			}
		}
	}


	for (i=pstStatusList->bBadStatusNum - 1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
	
		if (pstStatusDef )
		{
			StatusResultMap* pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
			
			if (IS_STATUS_RESULTMAP_SET(STATUS_RESULT_LUCKY_ATTENDEE, pstResultMap->sMap))
			{
				pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_LUCK;
			}
		}
	}



	for (i=pstStatusList->bNoLimitStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
	
		if (pstStatusDef)
		{
			StatusResultMap* pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
			
			if (IS_STATUS_RESULTMAP_SET(STATUS_RESULT_LUCKY_ATTENDEE, pstResultMap->sMap))
			{
				pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_LUCK;
			}
		}
	}
	return 0;

	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_status_offline(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer )
{
	int i;
	int iFlag = 0;
	ZoneStatus *pstZoneStatus;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	STATUSDEF* pstStatusDef;

	for (i=pstStatusList->bGoodStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
	
		if (pstStatusDef && (pstStatusDef->StatusFlag & STATUS_LOGOUT_CLEAN))
		{
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astGoodStatus, i, &pstStatusList->bGoodStatusNum);
			iFlag = 1;
		}
	}


	for (i=pstStatusList->bBadStatusNum - 1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
	
		if (pstStatusDef && (pstStatusDef->StatusFlag & STATUS_LOGOUT_CLEAN))
		{
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astBadStatus, i, &pstStatusList->bBadStatusNum);

			iFlag = 1;
		}
	}



	for (i=pstStatusList->bNoLimitStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
	
		if (pstStatusDef && (pstStatusDef->StatusFlag & STATUS_LOGOUT_CLEAN))
		{
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astNoLimitStatus, i, &pstStatusList->bNoLimitStatusNum);

			iFlag = 1;
		}
	}

	if (iFlag)
	{

		z_player_send_status(pstAppCtx, pstEnv, pstPlayer);

		z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);	


	}

	return 0;
}

int z_status_safearea(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer )
{
	int i;
	int iFlag = 0;
	ZoneStatus *pstZoneStatus;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	STATUSDEF* pstStatusDef;

	for (i=pstStatusList->bGoodStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
	
		if (pstStatusDef && (pstStatusDef->StatusFlag & STATUS_SAFE_AREA_CLEAN))
		{
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astGoodStatus, i, &pstStatusList->bGoodStatusNum);
			iFlag = 1;
		}
	}


	for (i=pstStatusList->bBadStatusNum - 1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
	
		if (pstStatusDef && (pstStatusDef->StatusFlag & STATUS_SAFE_AREA_CLEAN))
		{
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astBadStatus, i, &pstStatusList->bBadStatusNum);

			iFlag = 1;
		}
	}



	for (i=pstStatusList->bNoLimitStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
	
		if (pstStatusDef && (pstStatusDef->StatusFlag & STATUS_SAFE_AREA_CLEAN))
		{
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astNoLimitStatus, i, &pstStatusList->bNoLimitStatusNum);

			iFlag = 1;
		}
	}

	if (iFlag)
	{

		z_player_send_status(pstAppCtx, pstEnv, pstPlayer);

		z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);	


	}

	return 0;
}



// iPosFlag 表示保存的位置  
// 为1时保存在misc中,进副本清除状态要用到
int z_status_roledata(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int iPosFlag)
{
	int i;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	STATUSINFO *pstStatusInfo ;//= &pstPlayer->stRoleData.StatusInfo;
	STATUSDEF *pstStatusDef;

	if (iPosFlag == 0)
	{
		pstStatusInfo = &pstPlayer->stRoleData.StatusInfo;
	}
	else
	{
		pstStatusInfo = &pstPlayer->stRoleData.MiscInfo.StatusInfo;
	}
	
	pstStatusInfo->GoodStatusNum = 0;
	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstStatusDef = z_find_status_zonestatus(pstEnv, &pstStatusList->astGoodStatus[i]);
		if(pstStatusDef && (pstStatusDef->StatusFlag & STATUS_LOGOUT_CLEAN) && iPosFlag == 0)
		{
			continue;
		}
		
		pstStatusInfo->GoodStatusList[pstStatusInfo->GoodStatusNum].StatusID = pstStatusList->astGoodStatus[i].unStatusID;
		pstStatusInfo->GoodStatusList[pstStatusInfo->GoodStatusNum].Level = pstStatusList->astGoodStatus[i].bLevel;
		if (pstStatusList->astGoodStatus[pstStatusInfo->GoodStatusNum].stEnd.tv_sec > pstAppCtx->stCurr.tv_sec)
		{
			pstStatusInfo->GoodStatusList[pstStatusInfo->GoodStatusNum].Time = pstStatusList->astGoodStatus[i].stEnd.tv_sec - pstAppCtx->stCurr.tv_sec;
		}
		else
		{
			pstStatusInfo->GoodStatusList[pstStatusInfo->GoodStatusNum].Time = 0;
		}
		pstStatusInfo->GoodStatusList[pstStatusInfo->GoodStatusNum].Overlay = pstStatusList->astGoodStatus[i].unOverlay;
		pstStatusInfo->GoodStatusList[pstStatusInfo->GoodStatusNum].Val = pstStatusList->astGoodStatus[i].iVal;
		pstStatusInfo->GoodStatusNum++;
	}

	pstStatusInfo->BadStatusNum = 0;
	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstStatusDef = z_find_status_zonestatus(pstEnv, &pstStatusList->astBadStatus[i]);
		if(pstStatusDef && (pstStatusDef->StatusFlag & STATUS_LOGOUT_CLEAN)&& iPosFlag == 0)
		{
			continue;
		}
		
		pstStatusInfo->BadStatusList[pstStatusInfo->BadStatusNum].StatusID = pstStatusList->astBadStatus[i].unStatusID;
		pstStatusInfo->BadStatusList[pstStatusInfo->BadStatusNum].Level = pstStatusList->astBadStatus[i].bLevel;
		if (pstStatusList->astBadStatus[pstStatusInfo->BadStatusNum].stEnd.tv_sec > pstAppCtx->stCurr.tv_sec)
		{
			pstStatusInfo->BadStatusList[pstStatusInfo->BadStatusNum].Time = pstStatusList->astBadStatus[i].stEnd.tv_sec - pstAppCtx->stCurr.tv_sec;
		}
		else
		{
			pstStatusInfo->BadStatusList[pstStatusInfo->BadStatusNum].Time = 0;
		}
		pstStatusInfo->BadStatusList[pstStatusInfo->BadStatusNum].Overlay = pstStatusList->astBadStatus[i].unOverlay;
		pstStatusInfo->BadStatusList[pstStatusInfo->BadStatusNum].Val = pstStatusList->astBadStatus[i].iVal;
		pstStatusInfo->BadStatusNum++;
	}

	pstStatusInfo->NoLimitStatusNum = 0;
	for (i=0; i<pstStatusList->bNoLimitStatusNum; i++)
	{
		pstStatusDef = z_find_status_zonestatus(pstEnv, &pstStatusList->astNoLimitStatus[i]);
		if(pstStatusDef && (pstStatusDef->StatusFlag & STATUS_LOGOUT_CLEAN)&& iPosFlag == 0)
		{
			continue;
		}
		
		pstStatusInfo->NolimitStatusList[pstStatusInfo->NoLimitStatusNum].StatusID = pstStatusList->astNoLimitStatus[i].unStatusID;
		pstStatusInfo->NolimitStatusList[pstStatusInfo->NoLimitStatusNum].Level = pstStatusList->astNoLimitStatus[i].bLevel;
		if (pstStatusList->astNoLimitStatus[pstStatusInfo->NoLimitStatusNum].stEnd.tv_sec > pstAppCtx->stCurr.tv_sec)
		{
			pstStatusInfo->NolimitStatusList[pstStatusInfo->NoLimitStatusNum].Time = pstStatusList->astNoLimitStatus[i].stEnd.tv_sec - pstAppCtx->stCurr.tv_sec;
		}
		else
		{
			pstStatusInfo->NolimitStatusList[pstStatusInfo->NoLimitStatusNum].Time = 0;
		}
		pstStatusInfo->NolimitStatusList[pstStatusInfo->NoLimitStatusNum].Overlay = pstStatusList->astNoLimitStatus[i].unOverlay;
		pstStatusInfo->NolimitStatusList[pstStatusInfo->NoLimitStatusNum].Val = pstStatusList->astNoLimitStatus[i].iVal;
		pstStatusInfo->NoLimitStatusNum++;
	}
	
	return 0;
}

int z_pet_status_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ROLEPET *pstRolePet, ZoneStatus *pstStatus, int iPos, unsigned char *pbNum)
{
	ZoneStatus *pstStatusPos = pstStatus + iPos;
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	UNUSED(pstRolePet);

	//防止越界
	if(*pbNum > 0)
	{
		(*pbNum)--;

		if (iPos < *pbNum)
		{
			memmove(pstStatusPos, pstStatusPos+1, (*pbNum-iPos)*sizeof(*pstStatus));
		}
	}

	return 0;
}

int z_status_result_del_trans(ZONESVRENV* pstEnv,ZoneStatus* pstZoneStatus,STATUSRESULT* pstResult,int iKillerObjType,int iKillerMemID,ZONEMAPINST* pstMapInst,Player* pstExcept)
{
	int iTransed = 0;
	Player* pstKiller = NULL;
	//Monster* pstMonKiller = NULL;
	int iLast = 0;
	StatusResultMap* pstStatusMap = NULL;
	STATUSDEF* pstStatusDef = NULL;
	int iRand = 0;
	ZoneStatus* pstStatusTransed = NULL;
	int iDynPara = 0;
	int iMiscType = 0;

	if (!pstZoneStatus || !pstResult || !pstMapInst)
	{
		return -1;
	}

	pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
	if (!pstStatusDef)
	{
		return -1;
	}	

	iDynPara = pstZoneStatus->iDynPara;
	iMiscType = pstZoneStatus->MiscType;

	pstStatusMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
	
	iLast =pstZoneStatus->stEnd.tv_sec - pstEnv->pstAppCtx->stCurr.tv_sec;
	if (iLast <=0)
	{
		if(pstResult->ResultVal1 & STATUS_TRANS_MAP_NO_PROTECT_LUCKY)
		{
			//活动结束,清场
			pstEnv->iMiscFlag &= ~GAME_FLAG_LUCK;
			
			z_sys_strf_broadcast(pstEnv,SYS_CHAT, ZONEERR_BROAD343);
			lucky_open_ui_action(pstEnv,ACTIVE_GAME_SVR_TYPE_UI_CLOSE);
		}
		return 0;
	}
	
	if ((pstResult->ResultVal1 & STATUS_TRANS_KILLER) &&
		(iKillerObjType != OBJ_UNKNOW))
	{
		ZONEMAPINST* pstKillerMapInst = NULL;
		
		switch(iKillerObjType)
		{
		case OBJ_PLAYER:
			pstKiller = z_id_player(pstEnv->pstAppCtx,pstEnv,iKillerMemID);
			if(pstKiller && !(pstKiller->stOnline.State & CS_STAT_DEAD) &&
				 (pstKiller->stOnline.cMiscFlag & MISC_FLAG_LUCK) &&
				 !(pstKiller->stOnline.cSecenOverLay & SCENEOVERLAY_SAFE))
			{
				ZONEIDX stIdx;
				
				
				stIdx.iID = pstKiller->stRoleData.Map;
				stIdx.iPos = pstKiller->stOnline.iMapPos;
				
				pstKillerMapInst = z_find_map_inst(pstEnv->pstAppCtx,pstEnv, &stIdx);

				if (pstKillerMapInst == pstMapInst)
				{
					if ( 0 == z_player_apply_status2(pstEnv->pstAppCtx,pstEnv,pstKiller, 
						pstStatusDef->StatusID,pstStatusDef->Level,iLast,1,0,&pstStatusTransed))
					{
						iTransed = 1;						
					}
				}
			}

			break;
		/*	
		case OBJ_MONSTER:
			pstMonKiller = z_id_monster(pstEnv->pstAppCtx,pstEnv,iKillerMemID);
			if (pstMonKiller && (MONSTER_DIE != pstMonKiller->cStat))
			{
				pstKillerMapInst = z_find_map_inst(pstEnv->pstAppCtx,pstEnv, &pstMonKiller->stMap);

				if (pstKillerMapInst == pstMapInst)
				{
					if ( 0 == z_mon_apply_status(pstEnv->pstAppCtx,pstEnv,pstMonKiller, 
						pstStatusDef->StatusID,pstStatusDef->Level,iLast,1,0))
					{
						iTransed = 1;
					}
				}
			}
			break;
		*/	
		default:
			break;

		}
	}
	
	if ((pstResult->ResultVal1 & STATUS_TRANS_MAP_NO_PROTECT_LUCKY) &&
		(0 == iTransed) && pstMapInst->iPlayerCount > 0)
	{
		int i = 0;
		int iCount = 0;
		
		Player* pstValidPlayer = NULL;
		
		iRand = RAND1(pstMapInst->iPlayerCount);		
		
		for (i = 0; i < pstMapInst->iAreaDyn; i++) 
		{
			int iIndex	=	pstMapInst->aiAreaDyn[i];
			int k = 0;
			AREADYN* pstArea;

			if( iIndex<0)
			{
				continue;
			}

			pstArea	= (AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iIndex);
			if (NULL == pstArea)
			{
				continue;
			}
			
			for (k = 0; k < pstArea->iAnimate; k++) 
			{
				Player* pstMapPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstArea->aiAnimate[k]);
				
				if(NULL == pstMapPlayer || PLAYER_STATUS_ROLE_LOGIN != pstMapPlayer->eStatus ||
					(pstMapPlayer->stOnline.cSecenOverLay & SCENEOVERLAY_SAFE) ||
					!(pstMapPlayer->stOnline.cMiscFlag & MISC_FLAG_LUCK) ||
					(pstMapPlayer->stOnline.State & CS_STAT_DEAD) ||
					pstMapPlayer == pstExcept)
				{
					continue;
				}
				
				pstValidPlayer = pstMapPlayer;
				
				if (iCount == iRand)
				{
					break;
				}
				iCount++;
			}
		}

		//找到随机到的,或者是最后一个
		if (pstValidPlayer)
		{
			z_player_apply_status2(pstEnv->pstAppCtx,pstEnv,pstValidPlayer, 
						pstStatusDef->StatusID,pstStatusDef->Level,iLast,1,0,&pstStatusTransed);
			
		}
		else//找个怪物
		{
			/*
			int iMonID = get_map_rand_mon_memid(pstEnv,pstMapInst);
			int iRet = -1;
			if (iMonID)
			{
				Monster* pstMon = z_id_monster( pstEnv->pstAppCtx,pstEnv,iMonID);
				if (pstMon && (MONSTER_DIE != pstMon->cStat))
				{
					iRet = z_mon_apply_status(pstEnv->pstAppCtx,pstEnv,pstMon, pstStatusDef->StatusID,pstStatusDef->Level,iLast,1,0);
				}
			} 
			if (iRet != 0)//提前结束
			{
			
				//活动结束,清场
				pstEnv->iMiscFlag &= ~GAME_FLAG_LUCK;
			
			}*/
			
		}
	}
	
	if (pstStatusTransed)
	{
		pstStatusTransed->iDynPara = iDynPara;
		pstStatusTransed->MiscType = iMiscType;
	}
	else
	{
		//活动结束,
		pstEnv->iMiscFlag &= ~GAME_FLAG_LUCK;
	}

	return 0;
	
}


// 角色死亡清除状态，触发附加状态
int player_clean_state_touch_addstate(ZONESVRENV* pstEnv,Player *pstDiePlayer, STATUSDEF *pstStateDef)
{
	DieCleanOL *pstDieClean = &pstDiePlayer->stOnline.stDieClean;
	STATUSRESULT *pstResult;
	Player *pstKillPlayer;
	int iNum = 0;
	Player *pstTmp;
	int i;
	int iIndex;
	char cType;
	int iID;

	if (!pstDieClean->cDieClean || pstDieClean->iMemID <= 0)
	{
		return 0;
	}

	pstResult = z_get_status_result(pstStateDef, STATUS_RESULT_DIE_ADD_STATE);
	if (!pstResult)
	{
		return 0;
	}

	pstKillPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstDieClean->iMemID);
	if (!pstKillPlayer)
	{
		return 0;
	}

	for (i=0; i<pstDiePlayer->stOnline.iVAnimate; i++)
	{
		if (iNum >=pstResult->ResultVal1%1000 )
		{
			break;
		}
		iID = pstDiePlayer->stOnline.aiVAnimate[i];
		iIndex	=	z_id_indextype(pstEnv->pstAppCtx, pstEnv,iID, &cType);
		if( -1==iIndex )
			continue;

		switch(cType)
		{
			case OBJ_PLAYER:
				pstTmp = z_id_player(pstEnv->pstAppCtx, pstEnv, iID);
				if (NULL == pstTmp || PLAYER_STATUS_ROLE_LOGIN != pstTmp->eStatus)
					continue;
				if (pstTmp == pstKillPlayer || pstTmp == pstDiePlayer)
					continue;
				if (pstTmp->stRoleData.Map != pstKillPlayer->stRoleData.Map)
					continue;
				if (z_distance(&pstTmp->stRoleData.Pos, &pstDiePlayer->stRoleData.Pos) > pstResult->ResultVal1/1000*100)
				{
					continue;
				}

				if (!is_player_enemy_player(pstEnv, pstKillPlayer, pstTmp))
				{
					continue;
				}

				iNum++;
				z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstTmp, pstResult->ResultVal2,
										pstResult->ResultVal3, pstResult->ResultVal4, 1, 0);
				break;
		}
	}

	if (iNum > 0)
	{
		// 发个atk包 todo:
		CSPKG *pstPkgRes = &pstEnv->stCSRes;
		CSACTIONRES *pstActionRes = &pstPkgRes->Body.ActionRes;
		memset(pstActionRes,0,sizeof(CSACTIONRES));
		pstActionRes->ID = pstDiePlayer->iMemID;
		pstActionRes->ActionID = ACTION_HIT;
		pstActionRes->Data.Hit.SkillID = 2016;
		pstActionRes->Data.Hit.Level = 1;
		pstActionRes->Data.Hit.AtkPos = pstDiePlayer->stRoleData.Pos;
		pstActionRes->Data.Hit.SkillSeq = 0;
		pstActionRes->Data.Hit.RuneTransferedID = 2016;
		pstActionRes->Data.Hit.FromID = pstKillPlayer->iMemID;
		pstActionRes->Data.Hit.FromPos = pstDiePlayer->stRoleData.Pos;

		Z_CSHEAD_INIT(&pstPkgRes->Head, ACTION_RES);
		z_send_player_hit_res(pstEnv->pstAppCtx, pstEnv, pstKillPlayer, 
								&pstDiePlayer->stRoleData.Pos, pstPkgRes);
	}
	return 0;
}

int z_player_status_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ZoneStatus *pstStatus, int iPos, unsigned char *pbNum)
{
	int i;
	STATUSDEF *pstStatusDef;
	StatusResultMap *pstStatusMap;
	ZoneStatus *pstStatusPos = pstStatus + iPos;
	ZoneStatus stTmpStatus = *pstStatusPos;
	int iValue = pstStatusPos->iVal;
	int iApplyID = pstStatusPos->iApplyID;

	if (pstStatusPos->unBindSkillID > 0)
	{
		z_del_skill(pstAppCtx, pstEnv, pstPlayer, pstStatusPos->unBindSkillID, 1);
	}

	pstStatusDef = z_find_status_zonestatus(pstEnv, pstStatusPos);

	//回馈状态相关联的状态
	if (pstStatusPos->iFeedbackMemID && pstStatusPos->iFeedbackStatusID)
	{		
		Player* pstFeedbacker = z_id_player(pstAppCtx,pstEnv,pstStatusPos->iFeedbackMemID);
		
		pstStatusPos->iFeedbackMemID = 0;		
		
		if (pstFeedbacker && pstFeedbacker->eStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			z_player_del_status(pstAppCtx,pstEnv,pstFeedbacker,pstStatusPos->iFeedbackStatusID, 1, pstPlayer->iMemID);
		}
		pstStatusPos->iFeedbackStatusID = 0;
	}

	
	//回馈状态
	if (pstStatusDef)
	{
		StatusResultMap* pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(STATUS_RESULT_FEEDBACK, pstResultMap->sMap))
		{
			STATUSRESULT* pstFeedBack = z_get_status_result(pstStatusDef, STATUS_RESULT_FEEDBACK);

			if(pstFeedBack)
			{
				Player* pstApply = z_id_player(pstAppCtx,pstEnv,pstStatusPos->iApplyID);
				if (pstApply)
				{
					ZoneStatus * pstAdd = z_player_find_status(pstApply,pstFeedBack->ResultVal1);
					if (pstAdd && (pstAdd->iFeedbackMemID == pstPlayer->iMemID) && pstAdd->iFeedbackStatusID)
					{
						pstAdd->iFeedbackMemID = 0;
						pstAdd->iFeedbackStatusID = 0;
						
						z_player_del_status(pstAppCtx,pstEnv,pstApply,pstFeedBack->ResultVal1, 1, pstApply->iMemID);
					}
					
				}
			}
		}
	}

	//防止越界
	if(*pbNum > 0)
	{
		(*pbNum)--;

		if (iPos < *pbNum)
		{
			memmove(pstStatusPos, pstStatusPos+1, (*pbNum-iPos)*sizeof(*pstStatus));
		}
	}
	
	if (pstStatusDef)
	{
		pstStatusMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(STATUS_RESULT_MSPD, pstStatusMap->sMap) ||
			IS_STATUS_RESULTMAP_SET(STATUS_RESULT_STEP_DEC_MSPD, pstStatusMap->sMap) )
		{
			z_player_chk_mspd(pstAppCtx, pstEnv, pstPlayer);
		}

		//宏福活动
		{
			STATUSRESULT* pstResult;
			if (IS_STATUS_RESULTMAP_SET(STATUS_RESULT_LUCKY_ATTENDEE, pstStatusMap->sMap))
			{
				pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_LUCK;
			}
			pstResult = z_get_status_result(pstStatusDef, STATUS_RESULT_DEL_TRANS);
			if(pstResult)
			{
				if(pstResult->ResultVal1 & STATUS_TRANS_MAP_NO_PROTECT_LUCKY)
				{
					CSPKG stPkg;
					CSACTIONRES *pstAction = &stPkg.Body.ActionRes;
					ZONEIDX  stIdx;
					ZONEMAPINST * pstMapInst = NULL;
				

					memset(pstAction,0,sizeof(*pstAction));

					pstAction->ID = pstPlayer->iMemID;
					pstAction->ActionID = ACTION_MAP_POS;

					pstAction->Data.MapPos.ShowType = 0;

					Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);

					if (pstPlayer->stOnline.iOldChangeMapID)
					{
						stIdx.iID = pstPlayer->stOnline.iOldChangeMapID;
						stIdx.iPos = 0;
					}
					else
					{
						stIdx.iID = pstPlayer->stRoleData.Map;
						stIdx.iPos = pstPlayer->stOnline.iMapPos;
					}
					
					pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
					if (pstMapInst)
					{
						map_broadcast(pstEnv->pstAppCtx,pstEnv,pstMapInst,&stPkg);
					}

					pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_MAP_BROADCAST;
					player_broadcast_map_pos(pstEnv,pstPlayer,0);
				}
			}
		}

		if (pstStatusDef->StatusID == STATUS_ID_QIANXIN)
		{
			player_status_qianxin(pstEnv, pstPlayer, 0);
		}

		if (pstStatusDef->IsWushuang > 0) //无双状态
		{
			pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_WUSHUANG;
		}

		if(z_get_status_result(pstStatusDef, STATUS_RESULT_PET_ADD_ATK))
		{
			ROLEPET *pstRolePet = pet_get_camp(pstEnv, pstPlayer);
			if(pstRolePet)
			{
				pet_update_online(pstEnv, pstPlayer, pstRolePet);
			}
		}

		for (i=0; i<MAX_STATUS_RESULT; i++)
		{
			if (0 == pstStatusDef->Result[i].ResultID)
				break;

			switch(pstStatusDef->Result[i].ResultID)
			{
			case STATUS_RESULT_BEATBACK:
				z_del_skill(pstAppCtx, pstEnv, pstPlayer,
				            pstStatusDef->Result[i].ResultVal2, 0);
				break;
			case STATUS_RESULT_ADD_BIND_SKILL:
				z_del_skill(pstAppCtx, pstEnv, pstPlayer,
				            pstStatusDef->Result[i].ResultVal1, 0);
				break;
			// 如果状态效果中有禁止回蓝回血的效果,清除的时候需要去除MiscFlag标志
			case STATUS_RESULT_CANNOT_ADDHP:
				pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_CANNOT_ADDHP;
				break;

			case STATUS_RESULT_CANNOT_ADDMP:
				pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_CANNOT_ADDMP;
				break;

			case STATUS_RESULT_DAMAGE2HP:
				// 死了就不结算了,结算了也没用
				if(!(pstPlayer->stOnline.State & CS_STAT_DEAD) &&
				   pstStatusDef->Result[i].ResultVal1)
				{
					// 伤害转换HP技能一次性结算
					PlayerChagAttrVal stAttrChgVal;
					stAttrChgVal.AttrNum = 1;
					stAttrChgVal.AttrList[0].AttrID = ATTR_ID_HP;
					stAttrChgVal.AttrList[0].nFlag = 0;
					stAttrChgVal.AttrList[0].Value.HP = iValue;
					z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer,
					           &stAttrChgVal, 1);
				}
				break;

			case STATUS_RESULT_HATRED_MARK:
				// 怪物的标记结束了,unmark
				if(!(pstPlayer->stOnline.State & CS_STAT_DEAD) && iApplyID > 0)
				{
					AIEvt stAIEvt;
					Monster *pstMon = z_id_monster(pstAppCtx, pstEnv, iApplyID);
					if(!pstMon)
					{
						continue;
					}

					bzero(&stAIEvt, sizeof(stAIEvt));
					stAIEvt.iEvtID = AI_EVT_UNMARK;
					stAIEvt.stEvtDetail.stMark.stAtkee.iType = OBJ_PLAYER;
					stAIEvt.stEvtDetail.stMark.stAtkee.stObj.pstPlayer = 
						pstPlayer;
					mon_ai_stat(pstEnv, pstMon, &stAIEvt);
				}
				break;

			case STATUS_RESULT_REFLECTION:
				// TODO:
				break;
			case STATUS_RESULT_END_HPCHG:
				{
					int iOldHP = pstPlayer->stRoleData.RoleDetail.HPCurr;
					int iHP = pstPlayer->stRoleData.RoleDetail.HPCurr;

					iHP +=  pstStatusDef->Result[i].ResultVal1;
					if(iHP <= 0)
					{
						iHP = 1;
					}

					pstPlayer->stRoleData.RoleDetail.HPCurr = iHP;
					player_hp_action(pstEnv, pstPlayer, iOldHP, pstPlayer->stRoleData.RoleDetail.MPCurr, iApplyID);
				}
				break;
			case STATUS_RESULT_DEL_APPLY_STATUS:
				z_del_status_by_memid( pstAppCtx, pstEnv, &pstStatusDef->Result[i],iApplyID );
				break;			
			case STATUS_RESULT_ADD_BY_DEL:
				z_player_apply_status(pstAppCtx, pstEnv, pstPlayer, pstStatusDef->Result[i].ResultVal1,
					pstStatusDef->Result[i].ResultVal2,pstStatusDef->Result[i].ResultVal3,1, 0);
				break;
				
			case STATUS_RESULT_DEL_TRANS:
				{
					ZONEIDX stIdx;
					ZONEMAPINST * pstMapInst = NULL;

					if (pstPlayer->stOnline.iOldChangeMapID)
					{
						stIdx.iID = pstPlayer->stOnline.iOldChangeMapID;
						stIdx.iPos = 0;
					}
					else
					{
						stIdx.iID = pstPlayer->stRoleData.Map;
						stIdx.iPos = pstPlayer->stOnline.iMapPos;
					}
					
					pstMapInst = z_find_map_inst(pstEnv->pstAppCtx,pstEnv, &stIdx);
					if (pstMapInst)
					{
						z_status_result_del_trans(pstEnv, &stTmpStatus,&pstStatusDef->Result[i],
							pstPlayer->stOnline.iKillerObjType,pstPlayer->stOnline.iKillerMemID,pstMapInst,
							pstPlayer);
					}
				}
				break;
			case STATUS_RESULT_CLEAR_REFRESH_SKILL:
				{
					if(pstStatusDef->Result[i].ResultVal1 > 0)
					{
						player_skill_cool_clean(pstEnv,pstPlayer,pstStatusDef->Result[i].ResultVal1);
					}

					if(pstStatusDef->Result[i].ResultVal2 > 0)
					{
						player_skill_cool_clean(pstEnv,pstPlayer,pstStatusDef->Result[i].ResultVal2);
					}

					if(pstStatusDef->Result[i].ResultVal3 > 0 )
					{
						player_skill_cool_clean(pstEnv,pstPlayer,pstStatusDef->Result[i].ResultVal3);
					}

					if(pstStatusDef->Result[i].ResultVal4 > 0)
					{
						player_skill_cool_clean(pstEnv,pstPlayer,pstStatusDef->Result[i].ResultVal4);
					}
				}
				break;
			default:
				break;
			}
		}

		//上面有血量值的变化，最大 值更新后面一点
		if (IS_STATUS_RESULTMAP_SET(STATUS_RESULT_MAXHP, pstStatusMap->sMap) || 
			IS_STATUS_RESULTMAP_SET(STATUS_RESULT_MAXMP, pstStatusMap->sMap))
		{
			z_player_chk_maxmphp(pstAppCtx, pstEnv, pstPlayer);
		}

		if(pstStatusDef->IsCltShow == 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_STATUS202, pstStatusDef->Name);
		}

		player_clean_state_touch_addstate(pstEnv,pstPlayer, pstStatusDef);
	}

	return 0;
}

int z_pet_one_status_sec(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, ZoneStatus *pstZoneStatus)
{
	STATUSRESULT stResult;
	int iHPChg = 0;
	STATUSRESULT *pstResult;
	STATUSDEF *pstStatusDef = NULL;
	int iStatusInterval = 3;
	
	pstStatusDef = z_find_status(pstEnv, pstZoneStatus->unStatusID, pstZoneStatus->bLevel);
	if(!pstStatusDef)
	{
		return 0;
	}

	if (pstStatusDef->StatusInterVal > 0)
	{
		iStatusInterval = pstStatusDef->StatusInterVal;
	}

	if (pstAppCtx->stCurr.tv_sec - pstZoneStatus->tLastStatusCheck < iStatusInterval)
	{
		return 0;
	}

	pstZoneStatus->tLastStatusCheck = pstAppCtx->stCurr.tv_sec;

	if(0 == z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_HPADD, &stResult))
	{
		if( pstZoneStatus->iVal)
		{
			iHPChg += pstZoneStatus->iVal*(1+pstZoneStatus->nOverlayMul/100.0);
		}
		else
		{
			iHPChg +=  pstRolePet->MaxHP*(stResult.ResultVal1/100.0)+stResult.ResultVal2;
		}
	}

	pstResult = z_get_status_result(pstStatusDef, STATUS_RESULT_ADD_HP_FUNC_MUL);
	if(pstResult)
	{
		if( pstZoneStatus->iVal)
		{
			iHPChg += pstZoneStatus->iVal*(1+pstZoneStatus->nOverlayMul/100.0);
		}
		else if(pstZoneStatus->iJumpNum >0)
		{
			iHPChg +=  (pstResult->ResultVal2/pstZoneStatus->iJumpNum) * (1+pstZoneStatus->nOverlayMul/100.0);
		}
		pstZoneStatus->iJumpedCount++;
	}
	
	
	// 状态每3秒周期逐渐追加减血效果,ResultVal1表示乘修正,ResultVal2表示加修正
	if( 0 == z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_STEP_DEC_HP, &stResult) )
	{
		pstZoneStatus->ResultStepDecHp.ResultID = stResult.ResultID;
		pstZoneStatus->ResultStepDecHp.ResultVal1 += stResult.ResultVal1;
		pstZoneStatus->ResultStepDecHp.ResultVal2 += stResult.ResultVal2;
		pstZoneStatus->ResultStepDecHp.ResultVal3 += stResult.ResultVal3;
		pstZoneStatus->ResultStepDecHp.ResultVal4 += stResult.ResultVal4;

		iHPChg += pstZoneStatus->iVal* (1+pstZoneStatus->nOverlayMul/100.0)*(pstZoneStatus->ResultStepDecHp.ResultVal1/100.0)+
			              pstZoneStatus->ResultStepDecHp.ResultVal2;
	}
	

	if(iHPChg > 0)
	{
		pet_add_power(pstEnv, pstPlayer, pstRolePet, iHPChg);
	}
	
	return 0;
}

int z_pet_check_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet)
{
	int i;
	struct timeval stDiff;
	long long  llMs;
	ZoneStatus *pstZoneStatus;
	PetStatusInfo *pstStatusList;
	PetFightShiOnline *pstPetFightShiOnline;
	int fClear = 0;

	pstPetFightShiOnline = pet_find_fight_online(pstPlayer, pstRolePet);
	if(NULL == pstPetFightShiOnline)
	{
		return -1;
	}
	pstStatusList = &pstPetFightShiOnline->stStatusInfo;

	for (i=(int)pstStatusList->bGoodStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		z_pet_one_status_sec(pstAppCtx, pstEnv, pstPlayer, pstRolePet, pstZoneStatus);

		TV_DIFF(stDiff, pstZoneStatus->stEnd, pstAppCtx->stCurr);
		TV_TO_MS(llMs, stDiff);

		if (llMs < 0)
		{
			fClear = 1;
			z_pet_status_clear(pstAppCtx, pstEnv, pstRolePet, pstStatusList->astGoodStatus, i, &pstStatusList->bGoodStatusNum);
		}
	}

	if(fClear)
	{
		pet_update_online(pstEnv, pstPlayer, pstRolePet);
		pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);
	}

	return 0;
}

int z_player_status_chghp_die(ZONESVRENV* pstEnv, Player * pstPlayer, int iApplyID)
{
	Player *pstAtker;
	ZoneAni  stAni;

	stAni.iType = OBJ_UNKNOW;

	pstAtker = z_id_player(pstEnv->pstAppCtx, pstEnv, iApplyID);
	if(pstAtker)
	{
		stAni.iType = OBJ_PLAYER;
		stAni.stObj.pstPlayer = pstAtker;
	}

	return player_die(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stAni, 1);
}

int z_player_status_chg_hp_mp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int iHPChg,int iMPChg,int iApplyID)
{
	PlayerChagAttrVal stChgVal;
	int iOldHP = pstPlayer->stRoleData.RoleDetail.HPCurr;
	int iOldMP = pstPlayer->stRoleData.RoleDetail.MPCurr;
	int iHP = 0;
	AttrChgVal *pstVal = NULL;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	int fDead = 0;

	memset(&stChgVal,0,sizeof(stChgVal));
	
	if (0 > iHPChg) //不会因状态减血死亡
	{
		iHP = pstPlayer->stRoleData.RoleDetail.HPCurr;
		if (iHP > 0)
		{
			if ((iHP + iHPChg) < 0)
			{
				Player *pstApplyer;

				pstApplyer = z_id_player(pstAppCtx, pstEnv, iApplyID);

				//切磋的时候别弄死了
				if(pstApplyer && z_is_qiecuo(pstEnv, pstApplyer, pstPlayer))
				{
					iHPChg = -(iHP -1);
					
				}
				else
				{
					iHPChg = -iHP;
				}
			}
			
			pstVal = &stChgVal.AttrList[stChgVal.AttrNum];
			pstVal->AttrID = ATTR_ID_HP;
			pstVal->nFlag = 1;
			pstVal->Value.HP = -iHPChg;
			stChgVal.AttrNum++;
		}
	}
	else if (0 < iHPChg)
	{
		if (pstPlayer->stRoleData.RoleDetail.HPCurr < pstDyn->stMidAttrList.uiHPMax)
		{
			pstVal = &stChgVal.AttrList[stChgVal.AttrNum];
			pstVal->AttrID = ATTR_ID_HP;
			pstVal->nFlag = 0;
			pstVal->Value .HP = iHPChg;
			stChgVal.AttrNum++;
		}
	}
	
	if (0 > iMPChg)
	{
		if (pstPlayer->stRoleData.RoleDetail.MPCurr > 0)
		{
			pstVal = &stChgVal.AttrList[stChgVal.AttrNum];
			pstVal->AttrID = ATTR_ID_MP;
			pstVal->nFlag = 1;
			pstVal->Value.MP = -iMPChg;
			stChgVal.AttrNum++;
		}
		
	}
	else if (0 < iMPChg)
	{
		if (pstPlayer->stRoleData.RoleDetail.MPCurr < pstDyn->stMidAttrList.uiMPMax)
		{
			pstVal = &stChgVal.AttrList[stChgVal.AttrNum];
			pstVal->AttrID = ATTR_ID_MP;
			pstVal->nFlag = 0;
			pstVal->Value.MP = iMPChg;
			stChgVal.AttrNum++;
		}
	}

	if (stChgVal.AttrNum > 0)
	{
		z_attr_chg(pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);
	}

	if (iOldHP != (int)pstPlayer->stRoleData.RoleDetail.HPCurr ||
		iOldMP != (int)pstPlayer->stRoleData.RoleDetail.MPCurr)
	{
		player_hp_action(pstEnv, pstPlayer, iOldHP, iOldMP, iApplyID);

		if(pstPlayer->stRoleData.RoleDetail.HPCurr <= 0)
		{
			z_player_status_chghp_die(pstEnv, pstPlayer, iApplyID);
			fDead = 1;
		}
	}
	return fDead;
}

int z_mon_status_chghp_die(ZONESVRENV* pstEnv, Monster *pstMon, int iApplyID)
{
	MONSTERDEF * pstMonDef;
	Player *pstAtker;
	ZoneAni  stAni;

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (NULL == pstMonDef)
	{
		z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
		z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);
		return -1;
	}

	stAni.iType = OBJ_UNKNOW;

	pstAtker = z_id_player(pstEnv->pstAppCtx, pstEnv, iApplyID);
	if(pstAtker)
	{
		//争取一下战利品归属权
		mon_booty_first_atker(pstEnv->pstAppCtx, pstEnv, pstMon, pstAtker);

		stAni.iType = OBJ_PLAYER;
		stAni.stObj.pstPlayer = pstAtker;
	}
			
	return z_mon_die(pstEnv->pstAppCtx, pstEnv, pstMon, pstMonDef, &stAni, 1, 1);
}

int z_mon_status_chg_hp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, int iHPChg,int iApplyID)
{
	int iOldHP = 0;
	
	iOldHP = pstMon->iHP;

	pstMon->iHP += iHPChg;
	if (0 >= pstMon->iHP)
	{
		pstMon->iHP = 0;
	}

	if(iHPChg < 0)
	{
		ZoneAni stZoneAni;
		Player *pstAtker;

		pstAtker = z_id_player(pstEnv->pstAppCtx, pstEnv, iApplyID);
		if(pstAtker)
		{
			stZoneAni.iType = OBJ_PLAYER;
			stZoneAni.stObj.pstPlayer = pstAtker;
		
			mon_ai_addpress(pstEnv, pstMon, &stZoneAni, -iHPChg);
		}
	}

	if (pstMon->iHP != iOldHP)
	{
		mon_hp_action(pstEnv, pstMon, iOldHP, iApplyID);
		if(pstMon->iHP <= 0)
		{
			z_mon_status_chghp_die(pstEnv, pstMon, iApplyID);
		}
	}

	return 0;
	UNUSED(pstAppCtx);
}
//生命通道状态
int z_player_one_status_sec_chunnel(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ZoneStatus *pstZoneStatus)
{
	STATUSRESULT stResult;
	int iHPChg = 0;
	int iHPChg_apply = 0;
	int iIndex = 0;
	char cType = 0;
	TMEMBLOCK *pstMemBlock = NULL;
	Player *pstOther = NULL;
	Monster *pstMonOther = NULL;
	
	if( 0 >= pstZoneStatus->iApplyID ||
		0 > z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_HP_CHUNNEL_FUNC, &stResult) ||
		pstZoneStatus->iApplyID >= PET_ID_BASE )
	{
		return -1;
	}

	//宠物不能放这个出来
	if (pstZoneStatus->iApplyID >= PET_ID_BASE)
	{
		iIndex = z_id_indextype(pstAppCtx, pstEnv, pstZoneStatus->iApplyID%PET_ID_BASE, &cType);
	}
	else
	{
		iIndex = z_id_indextype(pstAppCtx, pstEnv, pstZoneStatus->iApplyID, &cType);
	}	
	
	if (iIndex < 0) 
	{
		return -1;
	}

	if (cType == OBJ_PLAYER)
	{
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
		if (!pstMemBlock->fValid)
		{
			return 0;
		}
		pstOther =  (Player *)pstMemBlock->szData;
		if (PLAYER_STATUS_ROLE_LOGIN != pstOther->eStatus || 
			(pstOther->stOnline.State & CS_STAT_DEAD) || 
			pstOther->stRoleData.RoleDetail.HPCurr == 0 ||
			(pstOther->stOnline.State & (CS_STAT_AUTOFLY | CS_STAT_QIANXIN)) ||
			pstOther == pstPlayer ||
			!z_pos_in_view_half(&pstPlayer->stRoleData.Pos, &pstOther->stRoleData.Pos) )
		{
			//把状态弄掉
			TV_CLONE(pstZoneStatus->stEnd, pstAppCtx->stCurr);
			pstZoneStatus->stEnd.tv_sec -= 1;
			return 0;
		}

		if(pstZoneStatus->cApplyRelation)
		{
			iHPChg = pstZoneStatus->iVal;
			iHPChg_apply = 	-iHPChg * (stResult.ResultVal2/100.0);
			//自己加血要做个修正
		iHPChg_apply = iHPChg_apply * (1 + pstOther->stOnline.stDyn.stHealChg.nAtkeeHealHPMul/100.0) + pstOther->stOnline.stDyn.stHealChg.unAtkeeHealHPAdd;
			
		}
		else
		{
			iHPChg = pstZoneStatus->iVal;
			iHPChg_apply = 	-iHPChg * (stResult.ResultVal1/100.0);
		}

		//最后一滴血不管了
		if( (iHPChg < 0 && pstPlayer->stRoleData.RoleDetail.HPCurr <=1) ||
			(iHPChg_apply < 0 && pstOther->stRoleData.RoleDetail.HPCurr <=1) )
		{
			return 0;
		}
		
		z_player_status_chg_hp_mp(pstAppCtx, pstEnv, pstPlayer,iHPChg,0,pstZoneStatus->iApplyID);
		z_player_status_chg_hp_mp(pstAppCtx, pstEnv, pstOther,iHPChg_apply,0,0);
		
	}
	else if (cType == OBJ_MONSTER)
	{
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
		if (!pstMemBlock->fValid)
		{
			return 0;
		}
		pstMonOther = (Monster *)pstMemBlock->szData;
		if (AI_STAT_HOME == pstMonOther->bAiStat ||
			pstMonOther->bSubType >= MON_SUB_TYPE_TRAP ||
			pstMonOther->cStat == MONSTER_DIE || 
			pstMonOther->iHP <= 0 ||
			//pstAppCtx->stCurr.tv_sec < pstMonOther->tBirthEnd ||
			!z_pos_in_view_half(&pstPlayer->stRoleData.Pos, &pstMonOther->stCurrPos))
		{
			//把状态弄掉
			TV_CLONE(pstZoneStatus->stEnd, pstAppCtx->stCurr);
			pstZoneStatus->stEnd.tv_sec -= 1;
			return 0;
		}
		
		if(pstZoneStatus->cApplyRelation)
		{
			iHPChg = pstZoneStatus->iVal;
			iHPChg_apply = 	-iHPChg * (stResult.ResultVal2/100.0);
		}
		else
		{
			iHPChg = pstZoneStatus->iVal;
			iHPChg_apply = 	-iHPChg * (stResult.ResultVal1/100.0);
		}

		//最后一滴血不管了
		if( (iHPChg < 0 && pstPlayer->stRoleData.RoleDetail.HPCurr <=1) ||
			(iHPChg_apply < 0 && pstMonOther->iHP<=0) )
		{
			return 0;
		}
		
		z_player_status_chg_hp_mp(pstAppCtx, pstEnv, pstPlayer,iHPChg,0,pstZoneStatus->iApplyID);
		z_mon_status_chg_hp(pstAppCtx, pstEnv, pstMonOther, iHPChg_apply,0);
	}
	
	return 0;
}

int z_mon_one_status_sec_chunnel(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, ZoneStatus *pstZoneStatus)
{
	STATUSRESULT stResult;
	int iHPChg = 0;
	int iHPChg_apply = 0;
	int iIndex = 0;
	char cType = 0;
	TMEMBLOCK *pstMemBlock = NULL;
	Player *pstOther = NULL;
	Monster *pstMonOther = NULL;
	
	if( 0 >= pstZoneStatus->iApplyID ||
		0 > z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_HP_CHUNNEL_FUNC, &stResult) ||
		pstZoneStatus->iApplyID >= PET_ID_BASE )
	{
		return -1;
	}

	//宠物不能放这个出来
	if (pstZoneStatus->iApplyID >= PET_ID_BASE)
	{
		iIndex = z_id_indextype(pstAppCtx, pstEnv, pstZoneStatus->iApplyID%PET_ID_BASE, &cType);
	}
	else
	{
		iIndex = z_id_indextype(pstAppCtx, pstEnv, pstZoneStatus->iApplyID, &cType);
	}	
	
	if (iIndex < 0) 
	{
		return -1;
	}

	if (cType == OBJ_PLAYER)
	{
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
		if (!pstMemBlock->fValid)
		{
			return -1;
		}
		pstOther =  (Player *)pstMemBlock->szData;
		if (PLAYER_STATUS_ROLE_LOGIN != pstOther->eStatus || 
			(pstOther->stOnline.State & CS_STAT_DEAD) || 
			pstOther->stRoleData.RoleDetail.HPCurr == 0 ||
			(pstOther->stOnline.State & (CS_STAT_AUTOFLY | CS_STAT_QIANXIN)) ||
			!z_pos_in_view_half(&pstMon->stCurrPos, &pstOther->stRoleData.Pos) )
		{
			//把状态弄掉
			TV_CLONE(pstZoneStatus->stEnd, pstAppCtx->stCurr);
			pstZoneStatus->stEnd.tv_sec -= 1;
			return 0;
		}

		if(pstZoneStatus->cApplyRelation)
		{
			iHPChg = pstZoneStatus->iVal;
			iHPChg_apply = 	-iHPChg * (stResult.ResultVal2/100.0);
			//自己加血要做个修正
			iHPChg_apply = iHPChg_apply * (1 + pstOther->stOnline.stDyn.stHealChg.nAtkeeHealHPMul/100.0) + pstOther->stOnline.stDyn.stHealChg.unAtkeeHealHPAdd;
		}
		else
		{
			iHPChg = pstZoneStatus->iVal;
			iHPChg_apply = 	-iHPChg * (stResult.ResultVal1/100.0);
		}

		//最后一滴血不管了
		if( (iHPChg < 0 && pstMon->iHP <=0) ||
			(iHPChg_apply < 0 && pstOther->stRoleData.RoleDetail.HPCurr <= 1) )
		{
			return 0;
		}
		
		z_mon_status_chg_hp(pstAppCtx, pstEnv, pstMon, iHPChg,pstZoneStatus->iApplyID);
		z_player_status_chg_hp_mp(pstAppCtx, pstEnv, pstOther,iHPChg_apply,0,0);
	}
	else if (cType == OBJ_MONSTER)
	{
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
		if (!pstMemBlock->fValid)
		{
			return 0;
		}
		pstMonOther = (Monster *)pstMemBlock->szData;
		if (AI_STAT_HOME == pstMonOther->bAiStat ||
			pstMonOther->bSubType >= MON_SUB_TYPE_TRAP ||
			pstMonOther->cStat == MONSTER_DIE || 
			pstMonOther->iHP <= 0 ||
			//pstAppCtx->stCurr.tv_sec < pstMonOther->tBirthEnd ||
			pstMonOther == pstMon ||
			!z_pos_in_view_half(&pstMon->stCurrPos, &pstMonOther->stCurrPos))
		{
			//把状态弄掉
			TV_CLONE(pstZoneStatus->stEnd, pstAppCtx->stCurr);
			pstZoneStatus->stEnd.tv_sec -= 1;
			return 0;
		}
		
		if(pstZoneStatus->cApplyRelation)
		{
			iHPChg = pstZoneStatus->iVal;
			iHPChg_apply = 	-iHPChg * (stResult.ResultVal2/100.0);
		}
		else
		{
			iHPChg = pstZoneStatus->iVal;
			iHPChg_apply = 	-iHPChg * (stResult.ResultVal1/100.0);
		}

		//最后一滴血不管了
		if( (iHPChg < 0 && pstMon->iHP <=0) ||
			(iHPChg_apply < 0 && pstMonOther->iHP <= 0) )
		{
			return 0;
		}
		
		z_mon_status_chg_hp(pstAppCtx, pstEnv, pstMon, iHPChg,pstZoneStatus->iApplyID);
		z_mon_status_chg_hp(pstAppCtx, pstEnv, pstMonOther,iHPChg_apply,0);
	}
	
	return 0;
}

int z_player_one_status_sec(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ZoneStatus *pstZoneStatus)
{
	STATUSRESULT stResult;
	STATUSRESULT *pstResult = NULL;
	int iHPChg = 0;
	int iMPChg = 0;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	STATUSDEF *pstStatusDef = NULL;
	int iStatusInterval = 3;
	int fDead = 0;

	pstStatusDef = z_find_status(pstEnv, pstZoneStatus->unStatusID, pstZoneStatus->bLevel);
	if(!pstStatusDef)
	{
		return 0;
	}

	if (pstStatusDef->StatusInterVal > 0)
	{
		iStatusInterval = pstStatusDef->StatusInterVal;
	}

	if (pstAppCtx->stCurr.tv_sec - pstZoneStatus->tLastStatusCheck < iStatusInterval)
	{
		return 0;
	}

	pstZoneStatus->tLastStatusCheck = pstAppCtx->stCurr.tv_sec;

	//默认生命通道没有其他效果
	if(0 == z_player_one_status_sec_chunnel(pstAppCtx, pstEnv, pstPlayer, pstZoneStatus))
	{
		//这里如果会死就要修改一下
		return 0;
	}


	if(pstPlayer->stOnline.cMoving && 0 == z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_DOT_ON_WALK, &stResult))
	{
		if( pstZoneStatus->iVal)
		{
			iHPChg += pstZoneStatus->iVal*(1+pstZoneStatus->nOverlayMul/100.0);
		}
	}
	
	if(0 == z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_HPADD, &stResult))
	{
		if( pstZoneStatus->iVal)
		{
			iHPChg += pstZoneStatus->iVal*(1+pstZoneStatus->nOverlayMul/100.0);
		}
		else
		{
			iHPChg +=  pstDyn->stMidAttrList.uiHPMax*(stResult.ResultVal1/100.0)+stResult.ResultVal2;
		}
	}
	pstResult = z_get_status_result(pstStatusDef, STATUS_RESULT_ADD_HP_FUNC_MUL);
	if(pstResult)
	{
		if( pstZoneStatus->iVal)
		{
			iHPChg += pstZoneStatus->iVal*(1+pstZoneStatus->nOverlayMul/100.0);
		}
		else if(pstZoneStatus->iJumpNum >0)
		{
			iHPChg +=  (pstResult->ResultVal2/pstZoneStatus->iJumpNum) * (1+pstZoneStatus->nOverlayMul/100.0);
		}
		pstZoneStatus->iJumpedCount++;
	}
	

	if(0 == z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_MPADD, &stResult))
	{
		iMPChg +=  pstDyn->stMidAttrList.uiMPMax*(stResult.ResultVal1/100.0)+ stResult.ResultVal2;
	}
       
	//debuff
	// 状态每3秒周期逐渐追加减速效果,ResultVal1表示乘修正,ResultVal2表示加修正
	if(0 == z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_STEP_DEC_MSPD, &stResult) )
	{
		pstZoneStatus->ResultStepDecSpd.ResultID = stResult.ResultID;
		pstZoneStatus->ResultStepDecSpd.ResultVal1 += stResult.ResultVal1;
		pstZoneStatus->ResultStepDecSpd.ResultVal2 += stResult.ResultVal2;
		pstZoneStatus->ResultStepDecSpd.ResultVal3 += stResult.ResultVal3;
		pstZoneStatus->ResultStepDecSpd.ResultVal4 += stResult.ResultVal4;
		
		z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	
	// 状态每3秒周期逐渐追加减血效果,ResultVal1表示乘修正,ResultVal2表示加修正
	if( 0 == z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_STEP_DEC_HP, &stResult) )
	{
		pstZoneStatus->ResultStepDecHp.ResultID = stResult.ResultID;
		pstZoneStatus->ResultStepDecHp.ResultVal1 += stResult.ResultVal1;
		pstZoneStatus->ResultStepDecHp.ResultVal2 += stResult.ResultVal2;
		pstZoneStatus->ResultStepDecHp.ResultVal3 += stResult.ResultVal3;
		pstZoneStatus->ResultStepDecHp.ResultVal4 += stResult.ResultVal4;

		iHPChg += pstZoneStatus->iVal* (1+pstZoneStatus->nOverlayMul/100.0)*(pstZoneStatus->ResultStepDecHp.ResultVal1/100.0)+
			              pstZoneStatus->ResultStepDecHp.ResultVal2;
	}
	
	if(0 == z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_HPDEC, &stResult))
	{
		if(pstZoneStatus->iVal)
		{
			iHPChg += pstZoneStatus->iVal* (1+pstZoneStatus->nOverlayMul/100.0);
		}
		else
		{
			//iHPChg += (pstZoneStatus->iVal + stResult.ResultVal2);
			iHPChg +=  pstDyn->stMidAttrList.uiHPMax*(stResult.ResultVal1/100.0)+stResult.ResultVal2;
		}
	}
	pstResult = z_get_status_result(pstStatusDef, STATUS_RESULT_DEC_HP_FUNC_MUL);
	if(pstResult)
	{
		if( pstZoneStatus->iVal)
		{
			iHPChg += pstZoneStatus->iVal*(1+pstZoneStatus->nOverlayMul/100.0);
		}
		else if(pstZoneStatus->iJumpNum >0)
		{
			iHPChg +=  (pstResult->ResultVal2/pstZoneStatus->iJumpNum) * (1+pstZoneStatus->nOverlayMul/100.0);
		}
		pstZoneStatus->iJumpedCount++;
	}
	

	if(0 == z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_MPDEC, &stResult))
	{
		iMPChg += pstDyn->stMidAttrList.uiMPMax*(stResult.ResultVal1/100.0)+stResult.ResultVal2;
	}

	if(0 == z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_DANCE_ADD_EXP, &stResult))
	{
		LEVELUP* pstLevel = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
		if (pstLevel)
		{
			player_add_exp(pstEnv, pstPlayer, pstLevel->ContributeExp*stResult.ResultVal1/100000.0, 0);
		}
	}

	z_player_status_chuanran_by_team(pstAppCtx, pstEnv, pstPlayer,pstZoneStatus);

	z_player_status_chuanran_by_player(pstAppCtx, pstEnv, pstPlayer,pstZoneStatus);

	if(0 == z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_ADD_ATTR, &stResult))
	{
		int iExp = stResult.ResultVal2;
		int iMoney = stResult.ResultVal2;
		int iGold = stResult.ResultVal2;
		LEVELUP* pstLevel = NULL;
		
		if (pstZoneStatus->MiscType & ZONESTATUS_MISCTYPE_DYN)
		{
			pstLevel = z_get_levelup(pstEnv,pstZoneStatus->iDynPara);
			if (pstLevel)
			{
				iExp = pstLevel->ContributeExp / 4800;
				iMoney = pstLevel->ContributeMoney / 1600;
				iGold = pstZoneStatus->iDynPara / 10;
				
			}
		}
		switch(stResult.ResultVal1)
		{
		case 1://经验
			if (iExp > 0)
			{
				long long llValue = iExp;
				z_player_addexp_test(pstEnv,pstPlayer, &llValue);

				if (llValue > 0)
				{
					PlayerChagAttrVal stChgVal;
					AttrChgVal *pstVal;
				

					stChgVal.AttrNum = 0;					
				
					pstVal = &stChgVal.AttrList[stChgVal.AttrNum++];
					pstVal->AttrID = ATTR_ID_EXP;
					pstVal->nFlag = 0;
					pstVal->Value.Exp = llValue;
					
					if (stChgVal.AttrNum > 0)
					{
						z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 2);
					}
					
					z_lucky_oplog(pstEnv,pstPlayer,OP_LUCKY_EXP,llValue);
				}
			}
			
			break;
			
		case 2://银币
			if (0 ==  package_add_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, iMoney))
			{
				package_add_money(pstEnv, pstPlayer, ATTR_ID_MONEY, iMoney);
				
				z_lucky_oplog(pstEnv,pstPlayer,OP_LUCKY_MONEY,iMoney);
				z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_LUCKY, pstStatusDef->StatusID, 0, iMoney, "BUFF争夺");
			}			
			break;
			
		case 3://金券
			if (iGold > 0)
			{
				PlayerChagAttrVal stChgVal;
				AttrChgVal *pstVal;
				

				stChgVal.AttrNum = 0;					
			
				pstVal = &stChgVal.AttrList[stChgVal.AttrNum++];
				pstVal->AttrID = ATTR_ID_BINDGODCOIN;
				pstVal->nFlag = 0;
				pstVal->Value.Money = iGold;
				
				if (stChgVal.AttrNum > 0)
				{
					z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 2);
				}
				
				z_lucky_oplog(pstEnv,pstPlayer,OP_LUCKY_GOLD,iGold);
				z_bindgold_create_oplog(pstEnv, pstPlayer, BINDGOLD_OPTYPE_LUCKYGOLD, pstStatusDef->StatusID,
								iGold, "BUFF争夺");
			}
			break;
		}
	}

	//等级差补偿经验BUFF
	if(pstZoneStatus->unStatusID == WORLD_LEVEL_EXP_BUFF_ID)
	{
		player_level_buff_exp_add(pstEnv, pstPlayer);
	}

	
	fDead = z_player_status_chg_hp_mp(pstAppCtx, pstEnv, pstPlayer,iHPChg,iMPChg,pstZoneStatus->iApplyID);

	return fDead;
}

//随机删除一个debuff的状态
int z_player_del_random_one_debuff(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,SKILLRESULT *pstResult,CSONEHIT *pstOneHit)
{
	unsigned int uiRandom = 0;
	ZoneStatus *pstZoneStatus;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	int i=0;
	STATUSDEF *pstStatusDef = NULL;

	for( i=0;i<pstResult->SkillResultVal1;i++)
	{
		if (0 == pstStatusList->bBadStatusNum)
		{
			break;
		}
		
		uiRandom = RAND1(pstStatusList->bBadStatusNum);
		if(uiRandom >= MAX_BAD_STATUS)
		{
			break;
		}
		pstZoneStatus = pstStatusList->astBadStatus+uiRandom;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if(pstStatusDef && (pstStatusDef->StatusFlag & STATUS_NO_CAN_CLEAR))
		{
			continue;
		}

		z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astBadStatus, uiRandom, &pstStatusList->bBadStatusNum);
	}
	
	z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);
	
	//z_player_send_status(pstAppCtx, pstEnv, pstPlayer);
	pstOneHit->StatusFlag = 1;
	//z_get_player_csstatusinfo(pstEnv->pstAppCtx, &pstOneHit->Status, &pstPlayer->stOnline.stStatusList);


	return 0;
}

int z_player_check_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	struct timeval stDiff;
	long long  llMs;
	int iFlag = 0;
	STATUSDEF *pstStatusDef;
	ZoneStatus *pstZoneStatus;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	tdr_longlong llSeq = 0xfffffffffLL;
	int iOldDead;
	
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_STATUS_TRACE;
	//改变地图要清理状态
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_STATUS_CHGMAP_CLEAN;
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_AUTO_PICK_ITEM;
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_SKILL;

	for (i=(int)pstStatusList->bGoodStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		if (llSeq == pstZoneStatus->iSeqID)
		{
			continue;
		}

		llSeq = pstZoneStatus->iSeqID;

		iOldDead = CS_STAT_DEAD & pstPlayer->stOnline.State;
		z_player_one_status_sec(pstAppCtx, pstEnv, pstPlayer, pstZoneStatus);
		if (0 == iOldDead && (CS_STAT_DEAD & pstPlayer->stOnline.State))
		{
			return 0;
		}

		//有绑定技能的良性状态，且技能发动条件0， 则服务器每秒主动发送此技能
		if (pstZoneStatus->unBindSkillID > 0 && pstZoneStatus->bBindSkillTrig == STATUS_TRIG_NONE)
		{
			player_atk_by_svr(pstAppCtx, pstEnv, pstPlayer, pstZoneStatus->unBindSkillID, pstZoneStatus->iApplyID);

			if(llSeq != pstZoneStatus->iSeqID)
			{
				continue;
			}
		}

		TV_DIFF(stDiff, pstZoneStatus->stEnd, pstAppCtx->stCurr);
		TV_TO_MS(llMs, stDiff);
		
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);

		if (llMs < 0)
		{
			iFlag = 1;
			//处理状态消失触发技能
			if ((pstZoneStatus->bBindSkillTrig & STATUS_TRIG_END))
			{
				if (pstStatusDef && 0 == (pstStatusDef->StatusFlag & STATUS_BEATK_CLEAN) &&
					2 != pstStatusDef->DeadClear)
				{
					if (0 > z_status_bind_skill_trig_end(pstEnv,pstZoneStatus,&pstPlayer->stRoleData.Pos))
					{
						player_atk_by_svr(pstAppCtx, pstEnv, pstPlayer, pstZoneStatus->unBindSkillID, pstZoneStatus->iApplyID);
					}
				}
			}
		
			z_player_del_status_seq(pstAppCtx, pstEnv, pstPlayer, 0, llSeq);
		}
		else
		{
			if(pstStatusDef)
			{
				if( (pstStatusDef->StatusFlag & STATUS_CHG_MAP_CLEAN))
				{
					pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_STATUS_CHGMAP_CLEAN;
				}
				if( pstStatusDef->StatusID == AUTO_PICK_ITEM_STATUS_ID)
				{
					pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_AUTO_PICK_ITEM;
				}
				
				if (pstStatusDef->StatusFlag & STATUS_BEATK_CLEAN)
				{
					pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_STATUS_TRACE;
				}

				if (pstStatusDef->BindSkill.SkillID && (pstStatusDef->BindSkill.Trig & STATUS_TRIG_BEATK))
				{
					pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_STATUS_TRACE;
				}

				/*
				if (pstStatusDef->StatusFlag & STATUS_HEAVYHIT_CLEAN)
				{
					pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_STATUS_TRACE;
				}*/

				if (pstStatusDef->StatusFlag & STATUS_SKILL_CLEAN)
				{
					pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_SKILL;
				}
			}
			
		}
	}

	for (i=(int)pstStatusList->bBadStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;

		if (llSeq == pstZoneStatus->iSeqID)
		{
			continue;
		}

		llSeq = pstZoneStatus->iSeqID;

		iOldDead = CS_STAT_DEAD & pstPlayer->stOnline.State;
		z_player_one_status_sec(pstAppCtx, pstEnv, pstPlayer, pstZoneStatus);
		if (0 == iOldDead && (CS_STAT_DEAD & pstPlayer->stOnline.State))
		{
			return 0;
		}

		if (pstZoneStatus->unBindSkillID > 0 && pstZoneStatus->bBindSkillTrig == STATUS_TRIG_NONE)
		{
			player_atk_by_svr(pstAppCtx, pstEnv, pstPlayer, pstZoneStatus->unBindSkillID, pstZoneStatus->iApplyID);

			if(llSeq != pstZoneStatus->iSeqID)
			{
				continue;
			}
		}

		TV_DIFF(stDiff, pstZoneStatus->stEnd, pstAppCtx->stCurr);
		TV_TO_MS(llMs, stDiff);

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);

		if (llMs < 0)
		{
			iFlag = 1;
			//处理状态消失触发技能
			if ((pstZoneStatus->bBindSkillTrig & STATUS_TRIG_END))
			{
				if (pstStatusDef && 0 == (pstStatusDef->StatusFlag & STATUS_BEATK_CLEAN) &&
					2 != pstStatusDef->DeadClear)
				{
					if (0 > z_status_bind_skill_trig_end(pstEnv,pstZoneStatus,&pstPlayer->stRoleData.Pos))
					{
						player_atk_by_svr(pstAppCtx, pstEnv, pstPlayer, pstZoneStatus->unBindSkillID, pstZoneStatus->iApplyID);
					}
				}
			}
			z_player_del_status_seq(pstAppCtx, pstEnv, pstPlayer, 0, llSeq);
		}
		else
		{
			if( pstStatusDef )
			{
				//改变地图要清理状态
				if( (pstStatusDef->StatusFlag & STATUS_CHG_MAP_CLEAN) )
				{
					pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_STATUS_CHGMAP_CLEAN;
				}
				if( pstStatusDef->StatusID == AUTO_PICK_ITEM_STATUS_ID)
				{
					pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_AUTO_PICK_ITEM;
				}

			
				if (pstStatusDef->StatusFlag & STATUS_BEATK_CLEAN)
				{
					pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_STATUS_TRACE;
				}

				if (pstStatusDef->BindSkill.SkillID && (pstStatusDef->BindSkill.Trig & STATUS_TRIG_BEATK))
				{
					pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_STATUS_TRACE;
				}

				/*
				if (pstStatusDef->StatusFlag & STATUS_HEAVYHIT_CLEAN)
				{
					pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_STATUS_TRACE;
				}*/

				if (pstStatusDef->StatusFlag & STATUS_SKILL_CLEAN)
				{
					pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_SKILL;
				}
			}
		}
	}

	for (i=(int)pstStatusList->bNoLimitStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;

		if (llSeq == pstZoneStatus->iSeqID)
		{
			continue;
		}

		llSeq = pstZoneStatus->iSeqID;

		iOldDead = CS_STAT_DEAD & pstPlayer->stOnline.State;
		z_player_one_status_sec(pstAppCtx, pstEnv, pstPlayer, pstZoneStatus);
		if (0 == iOldDead && (CS_STAT_DEAD & pstPlayer->stOnline.State))
		{
			return 0;
		}
		
		TV_DIFF(stDiff, pstZoneStatus->stEnd, pstAppCtx->stCurr);
		TV_TO_MS(llMs, stDiff);
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);

		if (llMs < 0)
		{
			iFlag = 1;
			//处理状态消失触发技能
			if ((pstZoneStatus->bBindSkillTrig & STATUS_TRIG_END))
			{
				if (pstStatusDef && 0 == (pstStatusDef->StatusFlag & STATUS_BEATK_CLEAN) &&
					2 != pstStatusDef->DeadClear)
				{
					if (0 > z_status_bind_skill_trig_end(pstEnv,pstZoneStatus,&pstPlayer->stRoleData.Pos))
					{
						player_atk_by_svr(pstAppCtx, pstEnv, pstPlayer, pstZoneStatus->unBindSkillID, pstZoneStatus->iApplyID);
					}
				}
			}
			z_player_del_status_seq(pstAppCtx, pstEnv, pstPlayer, 0, llSeq);
		}
		else
		{
			if(pstStatusDef)
			{
				if( (pstStatusDef->StatusFlag & STATUS_CHG_MAP_CLEAN))
				{
					pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_STATUS_CHGMAP_CLEAN;
				}
				if( pstStatusDef->StatusID == AUTO_PICK_ITEM_STATUS_ID)
				{
					pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_AUTO_PICK_ITEM;
				}
				
				if (pstStatusDef->StatusFlag & STATUS_BEATK_CLEAN)
				{
					pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_STATUS_TRACE;
				}

				if (pstStatusDef->BindSkill.SkillID && (pstStatusDef->BindSkill.Trig & STATUS_TRIG_BEATK))
				{
					pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_STATUS_TRACE;
				}

				/*
				if (pstStatusDef->StatusFlag & STATUS_HEAVYHIT_CLEAN)
				{
					pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_STATUS_TRACE;
				}*/

				if (pstStatusDef->StatusFlag & STATUS_SKILL_CLEAN)
				{
					pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_SKILL;
				}
			}

		
		}
	}

	if (iFlag)
	{
		z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);

		z_player_send_status(pstAppCtx, pstEnv, pstPlayer);
	}

	return 0;
}

// 清理匹配的状态
int Player_clean_matching_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int StatusFlag,long long MiscFlag)
{
	int i;
	int iFlag = 0;
	STATUSDEF *pstStatusDef;
	ZoneStatus *pstZoneStatus;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	
	for (i=(int)pstStatusList->bGoodStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if(pstStatusDef && (pstStatusDef->StatusFlag & StatusFlag))
		{
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astGoodStatus, i, &pstStatusList->bGoodStatusNum);
			iFlag = 1;
		}
	}

	for (i=(int)pstStatusList->bBadStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
				
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if(pstStatusDef && (pstStatusDef->StatusFlag & StatusFlag))
		{
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astBadStatus, i, &pstStatusList->bBadStatusNum);
			iFlag = 1;
		}
	}

	for (i=(int)pstStatusList->bNoLimitStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if(pstStatusDef && (pstStatusDef->StatusFlag & StatusFlag))
		{
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astNoLimitStatus, i, &pstStatusList->bNoLimitStatusNum);
			iFlag = 1;
		}
	}

	pstPlayer->stOnline.cMiscFlag &= ~MiscFlag;

	if (iFlag)
	{
		z_player_send_status(pstAppCtx, pstEnv, pstPlayer);

		z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);
	}
	
	return 0;
}


int player_status_clean_all(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	STATUSDEF *pstStatusDef;
	ZoneStatus *pstZoneStatus;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	
	for (i=(int)pstStatusList->bGoodStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if(pstStatusDef)
		{
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astGoodStatus, i, &pstStatusList->bGoodStatusNum);
		}
	}

	for (i=(int)pstStatusList->bBadStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
				
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if(pstStatusDef )
		{
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astBadStatus, i, &pstStatusList->bBadStatusNum);
		}
	}

	for (i=(int)pstStatusList->bNoLimitStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if(pstStatusDef )
		{			
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astNoLimitStatus, i, &pstStatusList->bNoLimitStatusNum);
		}
	}

	z_player_send_status(pstAppCtx, pstEnv, pstPlayer);
	z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);
	return 0;
}

int player_status_beatk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSONEHIT *pstOneHit)
{
	//这个MiscFlag是共用的，在tick里面清
	//return Player_clean_matching_status(pstAppCtx, pstEnv, pstPlayer, STATUS_BEATK_CLEAN, 0LL);

	int i;
	int iFlag = 0;
	int iSeq;
	STATUSDEF *pstStatusDef;
	ZoneStatus *pstZoneStatus;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	STATUSRESULT *pstStatusResult;
	
	for (i=(int)pstStatusList->bGoodStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		iSeq = pstZoneStatus->iSeqID;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if(pstStatusDef)
		{
			if (pstStatusDef->StatusFlag & STATUS_BEATK_CLEAN)
			{
				pstStatusResult = z_get_status_result(pstStatusDef, STATUS_RESULT_BEATK_CLR_RATE);
				if (!(pstStatusResult && pstStatusResult->ResultVal1*100 <= RAND1(10000)))
				{
					if ((pstZoneStatus->bBindSkillTrig & STATUS_TRIG_END) &&
						pstZoneStatus->unBindSkillID > 0)
					{
						if (0 > z_status_bind_skill_trig_end(pstEnv,pstZoneStatus,&pstPlayer->stRoleData.Pos))
						{
							player_atk_by_svr(pstAppCtx, pstEnv, pstPlayer, pstZoneStatus->unBindSkillID, pstZoneStatus->iApplyID);
						}
					}
					
					z_player_del_status_seq(pstAppCtx, pstEnv, pstPlayer, 0, iSeq);
					iFlag = 1;
				}
			}
			else if (pstZoneStatus->unBindSkillID > 0 && (pstZoneStatus->bBindSkillTrig & STATUS_TRIG_BEATK))
			{
				player_atk_by_svr(pstAppCtx, pstEnv, pstPlayer, pstZoneStatus->unBindSkillID, 0);
			}
		}
	}

	for (i=(int)pstStatusList->bBadStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
		iSeq = pstZoneStatus->iSeqID;
				
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if(pstStatusDef)
		{
			if (pstStatusDef->StatusFlag & STATUS_BEATK_CLEAN)
			{
				pstStatusResult = z_get_status_result(pstStatusDef, STATUS_RESULT_BEATK_CLR_RATE);
				if (!(pstStatusResult && pstStatusResult->ResultVal1*100 <= RAND1(10000)))
				{
					if ((pstZoneStatus->bBindSkillTrig & STATUS_TRIG_END) &&
						pstZoneStatus->unBindSkillID > 0)
					{
						if (0 > z_status_bind_skill_trig_end(pstEnv,pstZoneStatus,&pstPlayer->stRoleData.Pos))
						{
							player_atk_by_svr(pstAppCtx, pstEnv, pstPlayer, pstZoneStatus->unBindSkillID, pstZoneStatus->iApplyID);
						}
					}
					z_player_del_status_seq(pstAppCtx, pstEnv, pstPlayer, 0, iSeq);
					iFlag = 1;
				}
			}
			else if (pstZoneStatus->unBindSkillID > 0 && (pstZoneStatus->bBindSkillTrig & STATUS_TRIG_BEATK))
			{
				player_atk_by_svr(pstAppCtx, pstEnv, pstPlayer, pstZoneStatus->unBindSkillID, 0);
			}
		}
	}

	for (i=(int)pstStatusList->bNoLimitStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;
		iSeq = pstZoneStatus->iSeqID;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if(pstStatusDef )
		{
			if (pstStatusDef->StatusFlag & STATUS_BEATK_CLEAN)
			{
				pstStatusResult = z_get_status_result(pstStatusDef, STATUS_RESULT_BEATK_CLR_RATE);
				if (!(pstStatusResult && pstStatusResult->ResultVal1*100 <= RAND1(10000)))
				{
					if ((pstZoneStatus->bBindSkillTrig & STATUS_TRIG_END) &&
						pstZoneStatus->unBindSkillID > 0)
					{
						if (0 > z_status_bind_skill_trig_end(pstEnv,pstZoneStatus,&pstPlayer->stRoleData.Pos))
						{
							player_atk_by_svr(pstAppCtx, pstEnv, pstPlayer, pstZoneStatus->unBindSkillID, pstZoneStatus->iApplyID);
						}
					}
					
					z_player_del_status_seq(pstAppCtx, pstEnv, pstPlayer, 0, iSeq);
					iFlag = 1;
				}
			}
			else if (pstZoneStatus->unBindSkillID > 0 && (pstZoneStatus->bBindSkillTrig & STATUS_TRIG_BEATK))
			{
				player_atk_by_svr(pstAppCtx, pstEnv, pstPlayer, pstZoneStatus->unBindSkillID, 0);
			}
		}
	}

	if (iFlag)
	{
		pstOneHit->StatusFlag = 1;

		z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);
	}

	return 0;
}

/*
int player_status_heavyhit_clean(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	//这个MiscFlag是共用的，在tick里面清
	return Player_clean_matching_status(pstAppCtx, pstEnv, pstPlayer, STATUS_HEAVYHIT_CLEAN, 0LL);
}*/

int player_status_skill_clean(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	return Player_clean_matching_status(pstAppCtx, pstEnv, pstPlayer, STATUS_SKILL_CLEAN, MISC_FLAG_SKILL);
}

int z_del_status_by_memid(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, STATUSRESULT *pstResult,int iApplyID )
{
	int  iIndex = 0;
	char cType = 0;
	TMEMBLOCK *pstMemBlock = NULL;
	Player *pstPlayer = NULL;
	Monster *pstMon = NULL;

	if(!iApplyID ||
		pstResult->ResultID != STATUS_RESULT_DEL_APPLY_STATUS)
	{
		return -1;
	}
	
	iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, iApplyID, &cType);
	if (iIndex >= 0 )
	{
		if(cType == OBJ_PLAYER)
		{
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				return -1;
			}
			
			pstPlayer	=	(Player *)pstMemBlock->szData;
			if (pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
			{
				
				z_player_del_status(pstAppCtx, pstEnv, pstPlayer, pstResult->ResultVal1, 1, 0);
			}
			
		}
		else if( cType == OBJ_MONSTER )
		{
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				return -1;
			}

			pstMon = (Monster *)pstMemBlock->szData;
			
			z_mon_del_status(pstAppCtx, pstEnv, pstMon, pstResult->ResultVal1, 0);
		}
	}
	return 0;
}
int z_mon_status_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, ZoneStatus *pstStatus, int iPos, unsigned char *pbNum)
{
	int i = 0;
	STATUSDEF *pstStatusDef;
	StatusResultMap *pstStatusMap;
	ZoneStatus *pstStatusPos = pstStatus + iPos;
	const int iValue = pstStatusPos->iVal; 
	const int iApplyID = pstStatusPos->iApplyID;
	
	pstStatusDef = z_find_status_zonestatus(pstEnv, pstStatusPos);

	//防止越界
	if(*pbNum > 0)
	{
		(*pbNum)--;

		if (iPos < *pbNum)
		{
			memmove(pstStatus+iPos, pstStatus+iPos+1, (*pbNum-iPos)*sizeof(*pstStatus));
		}
	}

	if (pstStatusDef)
	{
		pstStatusMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(STATUS_RESULT_MSPD, pstStatusMap->sMap) ||
			IS_STATUS_RESULTMAP_SET(STATUS_RESULT_STEP_DEC_MSPD, pstStatusMap->sMap) )
		{
			z_mon_chk_mspd(pstAppCtx, pstEnv, pstMon, 1);
		}
		
		z_mon_chg_status_skill(pstEnv, pstMon,pstStatusDef,0);
		
		// 一些要在状态结束时处理的动作
		for(i = 0; i < MAX_STATUS_RESULT; i++)
		{
			if (0 == pstStatusDef->Result[i].ResultID)
				break;

			switch(pstStatusDef->Result[i].ResultID)
			{
			case STATUS_RESULT_DAMAGE2HP: // 伤害转换HP状态结算
				if(pstStatusDef->Result[i].ResultVal1)
				{
					pstMon->iHP += iValue;
					if(pstMon->iHP > pstMon->iMaxHP)
					{
						pstMon->iHP = pstMon->iMaxHP; 
					}
				}
				break;

			case STATUS_RESULT_HATRED_MARK: // 标记状态结束
				if(iApplyID)
				{
					AIEvt stAIEvt;
					Monster *pstMon = z_id_monster(pstAppCtx, pstEnv, iApplyID);
					if(!pstMon)
					{
						continue;
					}

					bzero(&stAIEvt, sizeof(stAIEvt));
					stAIEvt.iEvtID = AI_EVT_UNMARK;
					stAIEvt.stEvtDetail.stMark.stAtkee.iType = OBJ_MONSTER;
					stAIEvt.stEvtDetail.stMark.stAtkee.stObj.pstMon = pstMon;
					mon_ai_stat(pstEnv, pstMon, &stAIEvt);
				}
				break;
			case STATUS_RESULT_DEL_APPLY_STATUS:
				z_del_status_by_memid( pstAppCtx, pstEnv, &pstStatusDef->Result[i],iApplyID );
				break;
			case STATUS_RESULT_ADD_BY_DEL:
				z_mon_apply_status(pstAppCtx, pstEnv, pstMon, pstStatusDef->Result[i].ResultVal1,
					pstStatusDef->Result[i].ResultVal2,pstStatusDef->Result[i].ResultVal3,1, 0);
				break;		

			case STATUS_RESULT_DEL_TRANS:
				{
					ZONEMAPINST * pstMapInst = NULL;
					
					pstMapInst = z_find_map_inst(pstEnv->pstAppCtx,pstEnv, &pstMon->stMap);
					if (pstMapInst)
					{
						z_status_result_del_trans(pstEnv,pstStatusPos,&pstStatusDef->Result[i],
							pstMon->iKillerObjType,pstMon->iKillerMemID,pstMapInst,NULL);
					}
					
					if (pstStatusDef->Result[i].ResultVal1 & STATUS_TRANS_MAP_NO_PROTECT_LUCKY)
					{
						pstMon->cMonMiscFlag &= ~MON_MISC_FLAG_MAP_POS;
						mon_map_pos(pstEnv,pstMon,0);
					}
				}
				break;
				
			default:
				break;
			}
		}
	}

	
	return 0;
}

int mon_status_beatk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon)
{
	int i;
	STATUSDEF *pstStatusDef;
	ZoneStatus *pstZoneStatus;
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;
	STATUSRESULT *pstStatusResult;
	
	for (i=(int)pstStatusList->bGoodStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if(pstStatusDef && (pstStatusDef->StatusFlag & STATUS_BEATK_CLEAN))
		{
			pstStatusResult = z_get_status_result(pstStatusDef, STATUS_RESULT_BEATK_CLR_RATE);
			if (!(pstStatusResult && pstStatusResult->ResultVal1*100 <= RAND1(10000)))
			{
				z_mon_status_clear(pstAppCtx, pstEnv, pstMon, pstStatusList->astGoodStatus, i, &pstStatusList->bGoodStatusNum);
			}
		}
	}

	for (i=(int)pstStatusList->bBadStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
			
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if(pstStatusDef && (pstStatusDef->StatusFlag & STATUS_BEATK_CLEAN))
		{
			pstStatusResult = z_get_status_result(pstStatusDef, STATUS_RESULT_BEATK_CLR_RATE);
			if (!(pstStatusResult && pstStatusResult->ResultVal1*100 <= RAND1(10000)))
			{
				z_mon_status_clear(pstAppCtx, pstEnv, pstMon, pstStatusList->astBadStatus, i, &pstStatusList->bBadStatusNum);
			}
		}
	}

	pstMon->cMonMiscFlag &= ~MON_MISC_FLAG_STATUS_TRACE;
	z_mon_send_status(pstAppCtx, pstEnv, pstMon);
	
	return 0;
}	

int z_mon_one_status_sec(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, ZoneStatus *pstZoneStatus)
{
	STATUSRESULT stResult;
	STATUSRESULT *pstResult = NULL;
	STATUSDEF *pstStatusDef;
	int iHPChg = 0;
	//int iOldHP = 0;
	int iStatusInterval = 3;
	int iJumpFlag = 0;
	int fDecHP = 0;

	pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
	if(NULL == pstStatusDef)
	{
		return -1;
	}

	if (pstStatusDef->StatusInterVal > 0)
	{
		iStatusInterval = pstStatusDef->StatusInterVal;
	}

	if (pstAppCtx->stCurr.tv_sec - pstZoneStatus->tLastStatusCheck < iStatusInterval)
	{
		return 0;
	}

	pstZoneStatus->tLastStatusCheck = pstAppCtx->stCurr.tv_sec;

	//默认生命通道没有其他效果
	if( 0 == z_mon_one_status_sec_chunnel(pstAppCtx, pstEnv, pstMon, pstZoneStatus))
	{
		return 0;
	}

	if(pstMon->cMoving && 0 == z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_DOT_ON_WALK, &stResult))
	{
		if( pstZoneStatus->iVal)
		{
			iHPChg += pstZoneStatus->iVal*(1+pstZoneStatus->nOverlayMul/100.0);
		}
	}

	// 状态每3秒周期逐渐追加减速效果,ResultVal1表示乘修正,ResultVal2表示加修正
	if(0 == z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_STEP_DEC_MSPD, &stResult) )
	{
		pstZoneStatus->ResultStepDecSpd.ResultID = stResult.ResultID;
		pstZoneStatus->ResultStepDecSpd.ResultVal1 += stResult.ResultVal1;
		pstZoneStatus->ResultStepDecSpd.ResultVal2 += stResult.ResultVal2;
		pstZoneStatus->ResultStepDecSpd.ResultVal3 += stResult.ResultVal3;
		pstZoneStatus->ResultStepDecSpd.ResultVal4 += stResult.ResultVal4;

		z_mon_chk_mspd(pstAppCtx, pstEnv, pstMon, 1);
	}

	
	// 状态每3秒周期逐渐追加减血效果,ResultVal1表示乘修正,ResultVal2表示加修正
	if( 0 == z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_STEP_DEC_HP, &stResult) )
	{
		pstZoneStatus->ResultStepDecHp.ResultID = stResult.ResultID;
		pstZoneStatus->ResultStepDecHp.ResultVal1 += stResult.ResultVal1;
		pstZoneStatus->ResultStepDecHp.ResultVal2 += stResult.ResultVal2;
		pstZoneStatus->ResultStepDecHp.ResultVal3 += stResult.ResultVal3;
		pstZoneStatus->ResultStepDecHp.ResultVal4 += stResult.ResultVal4;

		iHPChg += pstZoneStatus->iVal* (1+pstZoneStatus->nOverlayMul/100.0)*(pstZoneStatus->ResultStepDecHp.ResultVal1/100.0)+
			              pstZoneStatus->ResultStepDecHp.ResultVal2;
	}
	
	// 检查有没有禁止回血的Status
	if(!z_get_status_result(pstStatusDef, STATUS_RESULT_CANNOT_ADDHP))
	{
		if(0 == z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_HPADD, &stResult))
		{
			if( pstZoneStatus->iVal )
			{
				iHPChg += pstZoneStatus->iVal * (1+pstZoneStatus->nOverlayMul/100.0);
			}
			else
			{
				iHPChg += pstMon->iMaxHP*(stResult.ResultVal1/100.0)+stResult.ResultVal2;
			}
		}
	}

	pstResult = z_get_status_result(pstStatusDef, STATUS_RESULT_ADD_HP_FUNC_MUL);
	if(pstResult)
	{
		if( pstZoneStatus->iVal)
		{
			iHPChg += pstZoneStatus->iVal*(1+pstZoneStatus->nOverlayMul/100.0);
		}
		else if(pstZoneStatus->iJumpNum >0)
		{
			iHPChg +=  (pstResult->ResultVal2/pstZoneStatus->iJumpNum) * (1+pstZoneStatus->nOverlayMul/100.0);
		}
		pstZoneStatus->iJumpedCount++;
		iJumpFlag = 1;
	}
	
	if(0 == z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_HPDEC, &stResult))
	{
		if(pstZoneStatus->iVal)
		{
			iHPChg += pstZoneStatus->iVal * (1+pstZoneStatus->nOverlayMul/100.0);
		}
		else
		{
			//iHPChg += (pstZoneStatus->iVal+stResult.ResultVal2);
			iHPChg +=  pstMon->iMaxHP*(stResult.ResultVal1/100.0)+stResult.ResultVal2;
		}

		fDecHP = 1;
	}
	pstResult = z_get_status_result(pstStatusDef, STATUS_RESULT_DEC_HP_FUNC_MUL);
	if(pstResult)
	{
		if( pstZoneStatus->iVal)
		{
			iHPChg += pstZoneStatus->iVal*(1+pstZoneStatus->nOverlayMul/100.0);
		}
		else if(pstZoneStatus->iJumpNum >0)
		{
			iHPChg +=  (pstResult->ResultVal2/pstZoneStatus->iJumpNum) * (1+pstZoneStatus->nOverlayMul/100.0);
		}
		if(iJumpFlag == 0)
		{
			pstZoneStatus->iJumpedCount++;
			iJumpFlag = 1;
		}

		fDecHP = 1;
	}


	if( fDecHP && 0 <= z_mon_dec_one_hp_by_hit(pstEnv,pstMon) )
	{
		iHPChg = -1;
	}

	z_mon_status_chg_hp(pstAppCtx, pstEnv, pstMon, iHPChg,pstZoneStatus->iApplyID);

	z_mon_status_chuanran_by_monster( pstAppCtx, pstEnv, pstMon, pstZoneStatus);
	return 0;
}

//随机删除一个debuff的状态
int z_mon_del_random_one_debuff(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon,SKILLRESULT *pstResult,CSONEHIT *pstOneHit)
{
	unsigned int uiRandom = 0;
	ZoneStatus *pstZoneStatus;
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;
	int i=0;
	STATUSDEF *pstStatusDef = NULL;

	for(i=0;i<pstResult->SkillResultVal1;i++)
	{
		if (0 == pstStatusList->bBadStatusNum)
		{
			break;
		}
			
		uiRandom = RAND1((int)pstStatusList->bBadStatusNum);
		if(uiRandom >= MAX_BAD_STATUS)
		{
			break;
		}
		pstZoneStatus = pstStatusList->astBadStatus+uiRandom;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if(pstStatusDef && (pstStatusDef->StatusFlag & STATUS_NO_CAN_CLEAR))
		{
			continue;
		}
		
		z_mon_status_clear(pstAppCtx, pstEnv, pstMon, pstStatusList->astBadStatus, uiRandom, &pstStatusList->bBadStatusNum);
	}

	//z_mon_send_status(pstAppCtx, pstEnv, pstMon);
	pstOneHit->StatusFlag = 1;
	//z_get_mon_csstatusinfo(pstEnv->pstAppCtx, &pstOneHit->Status, &pstMon->stStatusList);
	
	return 0;
}
int mon_status_clean_debuff(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon)
{
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;
	int i = 0;
	int iFlag = 0;
	
	for (i=(int)pstStatusList->bBadStatusNum-1;i>=0; i--)
	{
		iFlag++;
		z_mon_status_clear(pstAppCtx, pstEnv, pstMon, pstStatusList->astBadStatus, i, &pstStatusList->bBadStatusNum);
	}

	if (iFlag)
	{
		z_mon_send_status(pstEnv->pstAppCtx, pstEnv, pstMon);
	}
	return 0;
}


//状态结束触发技能,如果是人的话就用人的技能
int z_status_bind_skill_trig_end(ZONESVRENV* pstEnv,ZoneStatus *pstZoneStatus,RESPOS *pstAtkPos)
{
	/*
	Player *pstPlayer;
	char cType = 0;
	int iIndex;		
	TMEMBLOCK *pstMemBlock = NULL;

	if (!(pstZoneStatus->bBindSkillTrig & STATUS_TRIG_END) || 
		pstZoneStatus->iApplyID <=0 )
	{
		return -1;
	}
	
	iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstZoneStatus->iApplyID, &cType);
	if (iIndex >= 0 && cType == OBJ_PLAYER)
	{
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
		if (pstMemBlock->fValid)
		{
			pstPlayer	=	(Player *)pstMemBlock->szData;
			if(pstPlayer &&
			   pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
			{
				player_atk_by_add_skill(pstEnv->pstAppCtx, pstEnv,pstPlayer,
						 pstZoneStatus->unBindSkillID, pstZoneStatus->bBindSkillLevel,pstAtkPos);
				return 0;
			}
		}
	}
	*/

	return -1;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstZoneStatus);	//add by paraunused.pl
	UNUSED(pstAtkPos);	//add by paraunused.pl
}

int z_check_mon_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon)
{
	int i;
	int iFlag = 0;
	struct timeval stDiff;
	long long  llMs;
	STATUSDEF *pstStatusDef;
	ZoneStatus *pstZoneStatus;
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;
	tdr_longlong  llSeqID = 0xfffffffffLL;

	pstMon->cMonMiscFlag &= ~MON_MISC_FLAG_STATUS_TRACE;

	for (i=(int)pstStatusList->bGoodStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
				
		//buff满的情况一下加了两个buff，会有问题
		if (llSeqID == pstZoneStatus->iSeqID)
		{
			continue;
		}
		
		llSeqID = pstZoneStatus->iSeqID;

		z_mon_one_status_sec(pstAppCtx, pstEnv, pstMon, pstZoneStatus);
		if(pstMon->iHP == 0)
		{
			return 0;
		}

		//有绑定技能的良性状态，且技能发动条件0， 则服务器每秒主动发送此技能
		if (pstZoneStatus->unBindSkillID > 0 && pstZoneStatus->bBindSkillTrig == STATUS_TRIG_NONE)
		{
			mon_svr_atk(pstEnv, pstMon, pstZoneStatus->unBindSkillID, pstZoneStatus->bBindSkillLevel, pstZoneStatus->iApplyID);

			//防止出错
			if(llSeqID != pstZoneStatus->iSeqID)
			{
				continue;
			}
		}

		TV_DIFF(stDiff, pstZoneStatus->stEnd, pstAppCtx->stCurr);
		TV_TO_MS(llMs, stDiff);

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);

		if (llMs < 0)
		{
			iFlag = 1;
			//处理状态结束触发技能
			if (pstZoneStatus->bBindSkillTrig & STATUS_TRIG_END)
			{
				if (pstStatusDef && 0 == (pstStatusDef->StatusFlag & STATUS_BEATK_CLEAN) &&
					2 != pstStatusDef->DeadClear)
				{
					if (0 > z_status_bind_skill_trig_end(pstEnv,pstZoneStatus,&pstMon->stCurrPos))
					{
						mon_svr_atk(pstEnv, pstMon, pstZoneStatus->unBindSkillID, pstZoneStatus->bBindSkillLevel, pstZoneStatus->iApplyID);
					}
				}
			}

			z_mon_del_status_seq(pstAppCtx, pstEnv, pstMon, llSeqID);
		}
		else
		{
			if(pstStatusDef)
			{
				if (pstStatusDef->StatusFlag & STATUS_BEATK_CLEAN)
				{
					pstMon->cMonMiscFlag |= MON_MISC_FLAG_STATUS_TRACE;
				}

				if (pstStatusDef->BindSkill.SkillID && (pstStatusDef->BindSkill.Trig & STATUS_TRIG_BEATK))
				{
					pstMon->cMonMiscFlag |= MON_MISC_FLAG_STATUS_TRACE;
				}
			}
		}
	}

	for (i=(int)pstStatusList->bBadStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;

		if (llSeqID == pstZoneStatus->iSeqID)
		{
			continue;
		}
		
		llSeqID = pstZoneStatus->iSeqID;

		z_mon_one_status_sec(pstAppCtx, pstEnv, pstMon, pstZoneStatus);
		if(pstMon->iHP == 0)
		{
			return 0;
		}

		TV_DIFF(stDiff, pstZoneStatus->stEnd, pstAppCtx->stCurr);
		TV_TO_MS(llMs, stDiff);

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);

		if (llMs < 0)
		{
			iFlag = 1;
			//处理状态结束触发技能
			if (pstZoneStatus->bBindSkillTrig & STATUS_TRIG_END)
			{
				if (pstStatusDef && 0 == (pstStatusDef->StatusFlag & STATUS_BEATK_CLEAN) &&
					2 != pstStatusDef->DeadClear)
				{
					if (0 > z_status_bind_skill_trig_end(pstEnv,pstZoneStatus,&pstMon->stCurrPos))
					{
						mon_svr_atk(pstEnv, pstMon, pstZoneStatus->unBindSkillID, pstZoneStatus->bBindSkillLevel, pstZoneStatus->iApplyID);
					}
				}
			}

			z_mon_del_status_seq(pstAppCtx, pstEnv, pstMon, llSeqID);
		}
		else
		{
			if(pstStatusDef)
			{
				if (pstStatusDef->StatusFlag & STATUS_BEATK_CLEAN)
				{
					pstMon->cMonMiscFlag |= MON_MISC_FLAG_STATUS_TRACE;
				}

				if (pstStatusDef->BindSkill.SkillID && (pstStatusDef->BindSkill.Trig & STATUS_TRIG_BEATK))
				{
					pstMon->cMonMiscFlag |= MON_MISC_FLAG_STATUS_TRACE;
				}
			}
		}
	}

	if (iFlag)
	{
		z_mon_send_status(pstAppCtx, pstEnv, pstMon);
	}
	
	return 0;
}	

int player_die_status_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	ZoneStatus *pstZoneStatus;
	STATUSDEF *pstStatusDef;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	int iFlag = 0;
	int iSeq;

	for (i=(int)pstStatusList->bGoodStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		iSeq = pstZoneStatus->iSeqID;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (pstStatusDef && 0 == pstStatusDef->DeadClear)
		{
			continue;
		}
		iFlag = 1;

		if (2 == pstStatusDef->DeadClear && (pstZoneStatus->bBindSkillTrig & STATUS_TRIG_END) &&
					pstZoneStatus->unBindSkillID > 0)
		{
			if (0 > z_status_bind_skill_trig_end(pstEnv,pstZoneStatus,&pstPlayer->stRoleData.Pos))
			{
				player_atk_by_svr(pstAppCtx, pstEnv, pstPlayer, pstZoneStatus->unBindSkillID, pstZoneStatus->iApplyID);
			}
		}
		
		z_player_del_status_seq(pstAppCtx, pstEnv, pstPlayer, 0, iSeq);
	}

	for (i=(int)pstStatusList->bBadStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
				
		iSeq = pstZoneStatus->iSeqID;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (pstStatusDef && 0 == pstStatusDef->DeadClear)
		{
			continue;
		}
		iFlag = 1;

		if (2 == pstStatusDef->DeadClear && (pstZoneStatus->bBindSkillTrig & STATUS_TRIG_END) &&
					pstZoneStatus->unBindSkillID > 0)
		{
			if (0 > z_status_bind_skill_trig_end(pstEnv,pstZoneStatus,&pstPlayer->stRoleData.Pos))
			{
				player_atk_by_svr(pstAppCtx, pstEnv, pstPlayer, pstZoneStatus->unBindSkillID, pstZoneStatus->iApplyID);
			}
		}
		
		z_player_del_status_seq(pstAppCtx, pstEnv, pstPlayer, 0, iSeq);
	}

	for (i=(int)pstStatusList->bNoLimitStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;

		iSeq = pstZoneStatus->iSeqID;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (pstStatusDef && 0 == pstStatusDef->DeadClear)
		{
			continue;
		}
		iFlag = 1;

		if (2 == pstStatusDef->DeadClear && (pstZoneStatus->bBindSkillTrig & STATUS_TRIG_END) &&
					pstZoneStatus->unBindSkillID > 0)
		{
			if (0 > z_status_bind_skill_trig_end(pstEnv,pstZoneStatus,&pstPlayer->stRoleData.Pos))
			{
				player_atk_by_svr(pstAppCtx, pstEnv, pstPlayer, pstZoneStatus->unBindSkillID, pstZoneStatus->iApplyID);
			}
		}
		
		z_player_del_status_seq(pstAppCtx, pstEnv, pstPlayer, 0, iSeq);
	}

	if (iFlag)
	{
		z_player_send_status(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);
	}

	return 0;
}

int player_status_clean_debuff(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	int iFlag = 0;
	
	for (i=(int)pstStatusList->bBadStatusNum-1; i>=0; i--)	
	{
		iFlag++;
		z_player_status_clear(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstStatusList->astBadStatus, i, &pstStatusList->bBadStatusNum);	
	}

	if (iFlag)
	{
		z_player_send_status(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	return 0;
}

int player_status_clean(ZONESVRENV* pstEnv, Player *pstPlayer, SKILLRESULT *pstResult, unsigned char ucSubType)
{
	int i;
	ZoneStatus *pstZoneStatus;
	STATUSDEF *pstStatusDef;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	int iFlag = 0;
	
	for (i=(int)pstStatusList->bBadStatusNum-1; i>=0; i--)	
	{	
		pstZoneStatus = pstStatusList->astBadStatus+i;
				
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (pstStatusDef && pstStatusDef->SubType != ucSubType)
		{
			continue;
		}

		iFlag++;
		z_player_status_clear(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstStatusList->astBadStatus, i, &pstStatusList->bBadStatusNum);

		if (iFlag >= pstResult->SkillResultVal1)
		{
			break;
		}
	}

	if (iFlag)
	{
		z_player_send_status(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	return 0;
}

int mon_status_clean(ZONESVRENV* pstEnv, Monster *pstMon, SKILLRESULT *pstResult, unsigned char ucSubType)
{
	int i;
	ZoneStatus *pstZoneStatus;
	STATUSDEF *pstStatusDef;
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;
	int iFlag = 0;

	for (i=(int)pstStatusList->bBadStatusNum-1; i>=0; i--)	
	{	
		pstZoneStatus = pstStatusList->astBadStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (pstStatusDef && pstStatusDef->SubType != ucSubType)
		{
			continue;
		}
		
		iFlag++;
		z_mon_status_clear(pstEnv->pstAppCtx, pstEnv, pstMon, pstStatusList->astBadStatus, i, &pstStatusList->bBadStatusNum);

		if (iFlag >= pstResult->SkillResultVal1)
		{
			break;
		}
	}

	if (iFlag)
	{
		z_mon_send_status(pstEnv->pstAppCtx, pstEnv, pstMon);
	}

	return 0;
}

// 清理所有怪物的状态
int z_mon_status_ai_home_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon)
{
	int i;
	ZoneStatus *pstZoneStatus;
	STATUSDEF *pstStatusDef;
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;
	int iFlag = 0;

	for (i=(int)pstStatusList->bGoodStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (!pstStatusDef )
		{
			continue;
		}
		if( !(pstStatusDef->StatusFlag&STATUS_AI_HOME_CLEAN) ) 
		{
			continue;
		}

		iFlag = 1;
		z_mon_status_clear(pstAppCtx, pstEnv, pstMon, pstStatusList->astGoodStatus, i, &pstStatusList->bGoodStatusNum);
	}

	for (i=(int)pstStatusList->bBadStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (!pstStatusDef )
		{
			continue;
		}
		if( !(pstStatusDef->StatusFlag&STATUS_AI_HOME_CLEAN) ) 
		{
			continue;
		}
		iFlag = 1;
		z_mon_status_clear(pstAppCtx, pstEnv, pstMon, pstStatusList->astBadStatus, i, &pstStatusList->bBadStatusNum);
	}

	if (iFlag)
	{
		z_mon_send_status(pstAppCtx, pstEnv, pstMon);
	}
	return 0;
}
	
int mon_die_status_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon)
{
	int i;
	ZoneStatus *pstZoneStatus;
	STATUSDEF *pstStatusDef;
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;
	int iFlag = 0;
	int iSeq;

	for (i=(int)pstStatusList->bGoodStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		iSeq = pstZoneStatus->iSeqID;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (pstStatusDef && 0 == pstStatusDef->DeadClear)
		{
			continue;
		}

		iFlag = 1;

		if (2 == pstStatusDef->DeadClear && (pstStatusDef->BindSkill.Trig & STATUS_TRIG_END) &&
					pstStatusDef->BindSkill.SkillID > 0)
		{
			if(0 > z_status_bind_skill_trig_end(pstEnv,pstZoneStatus,&pstMon->stCurrPos))
			{
				mon_svr_atk(pstEnv, pstMon, pstZoneStatus->unBindSkillID, pstZoneStatus->bBindSkillLevel, pstZoneStatus->iApplyID);
			}
		}
		
		z_mon_del_status_seq(pstAppCtx, pstEnv, pstMon, iSeq);
	}

	for (i=(int)pstStatusList->bBadStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;

		iSeq = pstZoneStatus->iSeqID;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (pstStatusDef && 0 == pstStatusDef->DeadClear)
		{
			continue;
		}

		iFlag = 1;

		if (2 == pstStatusDef->DeadClear && (pstStatusDef->BindSkill.Trig & STATUS_TRIG_END) &&
					pstStatusDef->BindSkill.SkillID > 0)
		{
			if(0 > z_status_bind_skill_trig_end(pstEnv,pstZoneStatus,&pstMon->stCurrPos))
			{
				mon_svr_atk(pstEnv, pstMon, pstZoneStatus->unBindSkillID, pstZoneStatus->bBindSkillLevel, pstZoneStatus->iApplyID);
			}
		}
		
		z_mon_del_status_seq(pstAppCtx, pstEnv, pstMon, iSeq);
	}

	if (iFlag)
	{
		z_mon_send_status(pstAppCtx, pstEnv, pstMon);
	}
	
	return 0;
}

// 退出战斗时清除状态
int mon_out_fight_status_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,
                               Monster *pstMon)
{
	int i;
	ZoneStatus *pstZoneStatus = NULL;
	STATUSDEF *pstStatusDef = NULL;
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;
	int iFlag = 0;

	i = pstStatusList->bGoodStatusNum;
	while(i--)	
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (pstStatusDef && pstStatusDef->OutFightKeep) // 部分又要保留
		{
			continue;
		}

		iFlag = 1;
		z_mon_status_clear(pstAppCtx, pstEnv, pstMon,
		                   pstStatusList->astGoodStatus, i,
		                   &pstStatusList->bGoodStatusNum);
	}

	i = pstStatusList->bBadStatusNum;
	while(i--)	
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (pstStatusDef && pstStatusDef->OutFightKeep) // 部分又要保留
		{
			continue;
		}

		iFlag = 1;
		z_mon_status_clear(pstAppCtx, pstEnv, pstMon,
		                   pstStatusList->astBadStatus, i,
		                   &pstStatusList->bBadStatusNum);
	}

	if(iFlag)
	{
		z_mon_send_status(pstAppCtx, pstEnv, pstMon);
	}
	
	return 0;
}

ZoneStatus  *z_find_bindskill_status(ZONESVRENV* pstEnv, Player *pstPlayer, unsigned short unSkillID)
{
	int i;
	ZoneStatus *pstZoneStatus;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	UNUSED(pstEnv);

	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		if (pstZoneStatus->unBindSkillID == unSkillID)
			return pstZoneStatus;
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
				
		if (pstZoneStatus->unBindSkillID == unSkillID)
			return pstZoneStatus;
	}

	for (i=0; i<pstStatusList->bNoLimitStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;

		if (pstZoneStatus->unBindSkillID == unSkillID)
			return pstZoneStatus;
	}

	return NULL;
}

int z_get_pet_status_result(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, int iResultID, STATUSRESULT *pstResult)
{
	int i;
	STATUSDEF *pstStatusDef;
	StatusResultMap *pstResultMap;
	STATUSRESULT *pstStatusResult;
	PetStatusInfo *pstStatusList ;
	ZoneStatus *pstZoneStatus;
	PetFightShiOnline *pstPetFightShiOnline;

	memset(pstResult, 0, sizeof(*pstResult));
	pstPetFightShiOnline = pet_find_fight_online(pstPlayer, pstRolePet);
	if(NULL == pstPetFightShiOnline)
	{
		return -1;
	}
	
	pstStatusList = &pstPetFightShiOnline->stStatusInfo;
	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		pstStatusDef = z_find_status_petstatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			pstStatusResult = &pstStatusDef->Result[(unsigned)pstResultMap->sMapIdx[iResultID]];
			pstResult->ResultID = iResultID;
			pstResult->ResultVal1 += pstStatusResult->ResultVal1*(1+pstZoneStatus->nOverlayMul/100.0);
			pstResult->ResultVal2 += pstStatusResult->ResultVal2*(1+pstZoneStatus->nOverlayMul/100.0);
			pstResult->ResultVal3 += pstStatusResult->ResultVal3*(1+pstZoneStatus->nOverlayMul/100.0);
			pstResult->ResultVal4 += pstStatusResult->ResultVal4*(1+pstZoneStatus->nOverlayMul/100.0);
			
			return 0;
		}
	}

	return -1;
}

STATUSRESULT* player_status_result(ZONESVRENV* pstEnv, Player *pstPlayer,
                                   int iResultID)
{
	ZoneStatus *pstZoneStatus = z_is_player_status_result(
		pstEnv,
		pstPlayer,
		iResultID);
	if(pstZoneStatus)
	{
		STATUSDEF *pstStatusDef = z_find_status(
			pstEnv,
			pstZoneStatus->unStatusID,
			pstZoneStatus->bLevel);
		if(pstStatusDef)
		{
			return z_get_status_result(pstStatusDef, iResultID);
		}
	}
	return NULL;
}

STATUSRESULT* mon_status_result(ZONESVRENV* pstEnv, Monster *pstMon,
                                   int iResultID)
{
	ZoneStatus *pstZoneStatus = z_is_mon_status_result(
		pstEnv,
		pstMon,
		iResultID);
	if(pstZoneStatus)
	{
		STATUSDEF *pstStatusDef = z_find_status(
			pstEnv,
			pstZoneStatus->unStatusID,
			pstZoneStatus->bLevel);
		if(pstStatusDef)
		{
			return z_get_status_result(pstStatusDef, iResultID);
		}
	}
	return NULL;
}

int z_get_status_info_result(ZONESVRENV* pstEnv, ZoneStatus *pstZoneStatus, int iResultID, STATUSRESULT *pstResult)
{
	STATUSRESULT *pstStatusResult;
	STATUSDEF *pstStatusDef;
	
	memset(pstResult, 0, sizeof(*pstResult));

	pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
	if(NULL == pstStatusDef)
	{
		return -1;
	}
	
	pstStatusResult = z_get_status_result(pstStatusDef, iResultID);
	if(NULL == pstStatusResult)
	{
		return -1;
	}
	
	pstResult->ResultID = iResultID;
	pstResult->ResultVal1 += pstStatusResult->ResultVal1*(1+pstZoneStatus->nOverlayMul/100.0);
	pstResult->ResultVal2 += pstStatusResult->ResultVal2*(1+pstZoneStatus->nOverlayMul/100.0);
	pstResult->ResultVal3 += pstStatusResult->ResultVal3*(1+pstZoneStatus->nOverlayMul/100.0);
	pstResult->ResultVal4 += pstStatusResult->ResultVal4*(1+pstZoneStatus->nOverlayMul/100.0);
	
	return 0;
}
//通过resultid把玩家的状态句柄取出来
ZoneStatus *z_get_player_zonestatus_by_resultid(ZONESVRENV* pstEnv, Player *pstPlayer, int iResultID)
{
	int i;
//	int iVal = 0;
	ZoneStatus *pstZoneStatus = NULL;
	STATUSDEF *pstStatusDef;
	StatusResultMap *pstResultMap;
	STATUSRESULT *pstStatusResult;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;

	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			pstStatusResult = &pstStatusDef->Result[(unsigned)pstResultMap->sMapIdx[iResultID]];
			if(pstStatusResult->ResultID == iResultID)
			{
				return pstZoneStatus;
			}
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
				
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			pstStatusResult = &pstStatusDef->Result[(unsigned)pstResultMap->sMapIdx[iResultID]];
			if( pstStatusResult->ResultID == iResultID )
			{
				return pstZoneStatus;
			}
		}
	}

	for (i=0; i<pstStatusList->bNoLimitStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			pstStatusResult = &pstStatusDef->Result[(unsigned)pstResultMap->sMapIdx[iResultID]];
			if( pstStatusResult->ResultID == iResultID )
			{
				return pstZoneStatus;
			}
		}
	}
	
	return NULL;
}

int z_get_player_status_result(ZONESVRENV* pstEnv, Player *pstPlayer, int iResultID, STATUSRESULT *pstResult, int iUnique)
{
	int i;
	int iVal = 0;
	ZoneStatus *pstZoneStatus;
	STATUSDEF *pstStatusDef;
	StatusResultMap *pstResultMap;
	STATUSRESULT *pstStatusResult;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;

	memset(pstResult, 0, sizeof(*pstResult));
	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			pstStatusResult = &pstStatusDef->Result[(unsigned)pstResultMap->sMapIdx[iResultID]];
			pstResult->ResultID = iResultID;
			if( iResultID == STATUS_RESULT_STEP_DEC_MSPD )
			{
				pstResult->ResultVal1 += pstZoneStatus->ResultStepDecSpd.ResultVal1;
				pstResult->ResultVal2 += pstZoneStatus->ResultStepDecSpd.ResultVal2;
				pstResult->ResultVal3 += pstZoneStatus->ResultStepDecSpd.ResultVal3;
				pstResult->ResultVal4 += pstZoneStatus->ResultStepDecSpd.ResultVal4;
			}
			else if (iResultID == STATUS_RESULT_DEL_BY_HEAVYHIT)
			{
				pstResult->ResultVal1 = pstStatusResult->ResultVal1;
			}
			else
			{
				pstResult->ResultVal1 += pstStatusResult->ResultVal1*(1+pstZoneStatus->nOverlayMul/100.0);
				pstResult->ResultVal2 += pstStatusResult->ResultVal2*(1+pstZoneStatus->nOverlayMul/100.0);
				pstResult->ResultVal3 += pstStatusResult->ResultVal3*(1+pstZoneStatus->nOverlayMul/100.0);
				pstResult->ResultVal4 += pstStatusResult->ResultVal4*(1+pstZoneStatus->nOverlayMul/100.0);
			}
			iVal += pstZoneStatus->iVal;

			if (iUnique) return iVal;
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
				
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			pstStatusResult = &pstStatusDef->Result[(unsigned)pstResultMap->sMapIdx[iResultID]];
			pstResult->ResultID = iResultID;
			pstResult->ResultID = iResultID;
			if( iResultID == STATUS_RESULT_STEP_DEC_MSPD )
			{
				pstResult->ResultVal1 += pstZoneStatus->ResultStepDecSpd.ResultVal1;
				pstResult->ResultVal2 += pstZoneStatus->ResultStepDecSpd.ResultVal2;
				pstResult->ResultVal3 += pstZoneStatus->ResultStepDecSpd.ResultVal3;
				pstResult->ResultVal4 += pstZoneStatus->ResultStepDecSpd.ResultVal4;
			}
			else if (iResultID == STATUS_RESULT_DEL_BY_HEAVYHIT)
			{
				pstResult->ResultVal1 = pstStatusResult->ResultVal1;
			}
			else
			{
				pstResult->ResultVal1 += pstStatusResult->ResultVal1*(1+pstZoneStatus->nOverlayMul/100.0);
				pstResult->ResultVal2 += pstStatusResult->ResultVal2*(1+pstZoneStatus->nOverlayMul/100.0);
				pstResult->ResultVal3 += pstStatusResult->ResultVal3*(1+pstZoneStatus->nOverlayMul/100.0);
				pstResult->ResultVal4 += pstStatusResult->ResultVal4*(1+pstZoneStatus->nOverlayMul/100.0);
			}
			iVal += pstZoneStatus->iVal;

			if (iUnique) return iVal;
		}
	}

	for (i=0; i<pstStatusList->bNoLimitStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			pstStatusResult = &pstStatusDef->Result[(unsigned)pstResultMap->sMapIdx[iResultID]];
			pstResult->ResultID = iResultID;
			pstResult->ResultID = iResultID;
			if( iResultID == STATUS_RESULT_STEP_DEC_MSPD )
			{
				pstResult->ResultVal1 += pstZoneStatus->ResultStepDecSpd.ResultVal1;
				pstResult->ResultVal2 += pstZoneStatus->ResultStepDecSpd.ResultVal2;
				pstResult->ResultVal3 += pstZoneStatus->ResultStepDecSpd.ResultVal3;
				pstResult->ResultVal4 += pstZoneStatus->ResultStepDecSpd.ResultVal4;
			}
			else if (iResultID == STATUS_RESULT_DEL_BY_HEAVYHIT)
			{
				pstResult->ResultVal1 = pstStatusResult->ResultVal1;
			}
			else
			{
				pstResult->ResultVal1 += pstStatusResult->ResultVal1*(1+pstZoneStatus->nOverlayMul/100.0);
				pstResult->ResultVal2 += pstStatusResult->ResultVal2*(1+pstZoneStatus->nOverlayMul/100.0);
				pstResult->ResultVal3 += pstStatusResult->ResultVal3*(1+pstZoneStatus->nOverlayMul/100.0);
				pstResult->ResultVal4 += pstStatusResult->ResultVal4*(1+pstZoneStatus->nOverlayMul/100.0);
			}
			iVal += pstZoneStatus->iVal;

			if (iUnique) return iVal;
		}
	}

	return iVal;
}

int z_get_player_status_result_min(ZONESVRENV* pstEnv, Player *pstPlayer, int iResultID, int *piVal)
{
	int i;
	int iTmpVal = *piVal;
	int iTmp;
	STATUSRESULT stStatusResultMin;
	STATUSRESULT stStatusResultNormal;
	STATUSRESULT stStatusResultTmp;
	ZoneStatus *pstZoneStatus;
	STATUSDEF *pstStatusDef;
	StatusResultMap *pstResultMap;
	STATUSRESULT *pstStatusResult;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;

	memset(&stStatusResultMin, 0, sizeof(stStatusResultMin));
	memset(&stStatusResultNormal, 0, sizeof(stStatusResultNormal));

	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			pstStatusResult = &pstStatusDef->Result[(unsigned)pstResultMap->sMapIdx[iResultID]];
			if (pstStatusResult->ResultVal1 < 0 || pstStatusResult->ResultVal2 < 0)
			{
				stStatusResultTmp.ResultVal1 = pstStatusResult->ResultVal1*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultTmp.ResultVal2 = pstStatusResult->ResultVal2*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultTmp.ResultVal3 = pstStatusResult->ResultVal3*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultTmp.ResultVal4 = pstStatusResult->ResultVal4*(1+pstZoneStatus->nOverlayMul/100.0);
				
				iTmp = (*piVal) * (1 + stStatusResultTmp.ResultVal1/100.0) + stStatusResultTmp.ResultVal2;	
				if (iTmp < iTmpVal)
				{
					iTmpVal = iTmp;
					memcpy(&stStatusResultMin, &stStatusResultTmp, sizeof(stStatusResultMin));
				}
			}
			else
			{
				stStatusResultNormal.ResultVal1 += pstStatusResult->ResultVal1*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultNormal.ResultVal2 += pstStatusResult->ResultVal2*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultNormal.ResultVal3 += pstStatusResult->ResultVal3*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultNormal.ResultVal4 += pstStatusResult->ResultVal4*(1+pstZoneStatus->nOverlayMul/100.0);
			}
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
				
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			pstStatusResult = &pstStatusDef->Result[(unsigned)pstResultMap->sMapIdx[iResultID]];
			if (pstStatusResult->ResultVal1 < 0 || pstStatusResult->ResultVal2 < 0)
			{
				stStatusResultTmp.ResultVal1 = pstStatusResult->ResultVal1*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultTmp.ResultVal2 = pstStatusResult->ResultVal2*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultTmp.ResultVal3 = pstStatusResult->ResultVal3*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultTmp.ResultVal4 = pstStatusResult->ResultVal4*(1+pstZoneStatus->nOverlayMul/100.0);
				
				iTmp = (*piVal) * (1 + stStatusResultTmp.ResultVal1/100.0) + stStatusResultTmp.ResultVal2;	
				if (iTmp < iTmpVal)
				{
					iTmpVal = iTmp;
					memcpy(&stStatusResultMin, &stStatusResultTmp, sizeof(stStatusResultMin));
				}
			}
			else
			{
				stStatusResultNormal.ResultVal1 += pstStatusResult->ResultVal1*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultNormal.ResultVal2 += pstStatusResult->ResultVal2*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultNormal.ResultVal3 += pstStatusResult->ResultVal3*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultNormal.ResultVal4 += pstStatusResult->ResultVal4*(1+pstZoneStatus->nOverlayMul/100.0);
			}
		}
	}

	for (i=0; i<pstStatusList->bNoLimitStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			pstStatusResult = &pstStatusDef->Result[(unsigned)pstResultMap->sMapIdx[iResultID]];
			if (pstStatusResult->ResultVal1 < 0 || pstStatusResult->ResultVal2 < 0)
			{
				stStatusResultTmp.ResultVal1 = pstStatusResult->ResultVal1*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultTmp.ResultVal2 = pstStatusResult->ResultVal2*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultTmp.ResultVal3 = pstStatusResult->ResultVal3*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultTmp.ResultVal4 = pstStatusResult->ResultVal4*(1+pstZoneStatus->nOverlayMul/100.0);
				
				iTmp = (*piVal) * (1 + stStatusResultTmp.ResultVal1/100.0) + stStatusResultTmp.ResultVal2;	
				if (iTmp < iTmpVal)
				{
					iTmpVal = iTmp;
					memcpy(&stStatusResultMin, &stStatusResultTmp, sizeof(stStatusResultMin));
				}
			}
			else
			{
				stStatusResultNormal.ResultVal1 += pstStatusResult->ResultVal1*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultNormal.ResultVal2 += pstStatusResult->ResultVal2*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultNormal.ResultVal3 += pstStatusResult->ResultVal3*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultNormal.ResultVal4 += pstStatusResult->ResultVal4*(1+pstZoneStatus->nOverlayMul/100.0);
			}
		}
	}

	*piVal = (*piVal) * (1 + (stStatusResultNormal.ResultVal1+stStatusResultMin.ResultVal1)/100.0) + 
				(stStatusResultNormal.ResultVal2+stStatusResultMin.ResultVal2);
	
	return 0;
}

int player_status_consume( ZONESVRENV* pstEnv,PlayerChagAttrVal *pstChgVal,Player *pstPlayer)
{
	STATUSDEF *pstStatusDef;
	//AttrChgVal *pstVal;
	//ROLEDETAIL *pstRoleDetail = &pstPlayer->stRoleData.RoleDetail; 
	int i;
	ZoneStatus *pstZoneStatus;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	UNUSED(pstChgVal);

	for (i=pstStatusList->bGoodStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		pstStatusDef =  z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (pstStatusDef)
		{
			if (pstStatusDef->ConsumeStatusType == CONSUME_STATUS_SPD)
			{
				/*if (pstRoleDetail->SpeMovePower <=  pstStatusDef->ConsumeStatusValue)
				{
					z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstStatusDef->StatusID, 1, 0);
					z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);
				}
				
				pstVal = &pstChgVal->AttrList[pstChgVal->AttrNum];
				pstVal->AttrID = ATTR_ID_SPEMOVE;
				pstVal->nFlag = 1;
				pstVal->Value.SpeMovePower = pstStatusDef->ConsumeStatusValue;
				pstChgVal->AttrNum++;*/
			}
			else 
			{

			}
		}
		
	}

	for (i=pstStatusList->bBadStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
				
		pstStatusDef =  z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (pstStatusDef)
		{
			if (pstStatusDef->ConsumeStatusType == CONSUME_STATUS_SPD)
			{
				/*if (pstRoleDetail->SpeMovePower <= pstStatusDef->ConsumeStatusValue)
				{
					z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstStatusDef->StatusID, 1, 0);
					z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);
				}
				
				pstVal = &pstChgVal->AttrList[pstChgVal->AttrNum];
				pstVal->AttrID = ATTR_ID_SPEMOVE;
				pstVal->nFlag = 1;
				pstVal->Value.SpeMovePower = pstStatusDef->ConsumeStatusValue;
				pstChgVal->AttrNum++;*/
			}
			else 
			{

			}
		}
	}

	for (i=pstStatusList->bNoLimitStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;

		pstStatusDef =  z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (pstStatusDef)
		{
			if (pstStatusDef->ConsumeStatusType == CONSUME_STATUS_SPD)
			{
				/*if (pstRoleDetail->SpeMovePower <= pstStatusDef->ConsumeStatusValue)
				{
					z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstStatusDef->StatusID, 1, 0);
					z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);
				}
				
				pstVal = &pstChgVal->AttrList[pstChgVal->AttrNum];
				pstVal->AttrID = ATTR_ID_SPEMOVE;
				pstVal->nFlag = 1;
				pstVal->Value.SpeMovePower = pstStatusDef->ConsumeStatusValue;
				pstChgVal->AttrNum++;*/
			}
			else 
			{

			}
		}		
	}
	return 0;
}

int z_player_byatk_clear_status(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	ZoneStatus *pstZoneStatus;
	STATUSDEF *pstStatusDef;

	for (i=pstStatusList->bGoodStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;

		if (pstStatusDef->ByAtkClear <= 0)
			continue;

		pstZoneStatus->iByAtk --;
		if (pstZoneStatus->iByAtk <= 0 )
		{
			z_player_del_status(pstEnv->pstAppCtx,pstEnv,pstPlayer,pstStatusDef->StatusID, 1, 0);
		}
	}

	for (i=pstStatusList->bBadStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;

		if (pstStatusDef->ByAtkClear <= 0)
			continue;

		pstZoneStatus->iByAtk --;
		if (pstZoneStatus->iByAtk <= 0 )
		{
			z_player_del_status(pstEnv->pstAppCtx,pstEnv,pstPlayer,pstStatusDef->StatusID, 1, 0);
		}
	}

	for (i=pstStatusList->bNoLimitStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;

		if (pstStatusDef->ByAtkClear <= 0)
			continue;

		pstZoneStatus->iByAtk --;
		if (pstZoneStatus->iByAtk <= 0 )
		{
			z_player_del_status(pstEnv->pstAppCtx,pstEnv,pstPlayer,pstStatusDef->StatusID, 1, 0);
		}
	}
	return 0;
}

ZoneStatus* z_is_player_status_result(ZONESVRENV* pstEnv, Player *pstPlayer, int iResultID)
{
	int i;
	ZoneStatus *pstZoneStatus;
	STATUSDEF *pstStatusDef;
	StatusResultMap *pstResultMap;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;

	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			return pstZoneStatus;
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
				
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			return pstZoneStatus;
		}
	}

	for (i=0; i<pstStatusList->bNoLimitStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			return pstZoneStatus;
		}
	}

	return NULL;
}

int  z_player_has_wushuang_status(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	ZoneStatus *pstZoneStatus;
	STATUSDEF *pstStatusDef;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;

	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		
		if(pstStatusDef->IsWushuang > 0)
		{
			return 1;
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		
		if(pstStatusDef->IsWushuang > 0)
		{
			return 1;
		}
	}

	for (i=0; i<pstStatusList->bNoLimitStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		
		if(pstStatusDef->IsWushuang > 0)
		{
			return 1;
		}
	}

	return 0;
}

ZoneStatus *z_get_mon_zonestatus_by_resultid(ZONESVRENV* pstEnv, Monster *pstMon, int iResultID)
{
	int i;
	ZoneStatus *pstZoneStatus = NULL;
	STATUSDEF *pstStatusDef;
	StatusResultMap *pstResultMap;
	STATUSRESULT *pstStatusResult;
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;


	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			pstStatusResult = &pstStatusDef->Result[(unsigned)pstResultMap->sMapIdx[iResultID]];
			if( pstStatusResult->ResultID == iResultID )
			{
				return pstZoneStatus;
			}
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			pstStatusResult = &pstStatusDef->Result[(unsigned)pstResultMap->sMapIdx[iResultID]];
			if( pstStatusResult->ResultID == iResultID )
			{
				return pstZoneStatus;
			}
		}
	}
	
	return NULL;
}

int z_get_mon_status_result(ZONESVRENV* pstEnv, Monster *pstMon, int iResultID, STATUSRESULT *pstResult)
{
	int i;
	int iVal = 0;
	ZoneStatus *pstZoneStatus;
	STATUSDEF *pstStatusDef;
	StatusResultMap *pstResultMap;
	STATUSRESULT *pstStatusResult;
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;

	memset(pstResult, 0, sizeof(*pstResult));

	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			pstStatusResult = &pstStatusDef->Result[(unsigned)pstResultMap->sMapIdx[iResultID]];
			pstResult->ResultID = pstStatusResult->ResultID;
			if( pstResult->ResultID == STATUS_RESULT_STEP_DEC_MSPD )
			{
				pstResult->ResultVal1 += pstZoneStatus->ResultStepDecSpd.ResultVal1;
				pstResult->ResultVal2 += pstZoneStatus->ResultStepDecSpd.ResultVal2;
				pstResult->ResultVal3 += pstZoneStatus->ResultStepDecSpd.ResultVal3;
				pstResult->ResultVal4 += pstZoneStatus->ResultStepDecSpd.ResultVal4;
			}
			else
			{
				pstResult->ResultVal1 += pstStatusResult->ResultVal1*(1+pstZoneStatus->nOverlayMul/100.0);
				pstResult->ResultVal2 += pstStatusResult->ResultVal2*(1+pstZoneStatus->nOverlayMul/100.0);
				pstResult->ResultVal3 += pstStatusResult->ResultVal3*(1+pstZoneStatus->nOverlayMul/100.0);
				pstResult->ResultVal4 += pstStatusResult->ResultVal4*(1+pstZoneStatus->nOverlayMul/100.0);
			}
		
			iVal += pstZoneStatus->iVal;
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			pstStatusResult = &pstStatusDef->Result[(unsigned)pstResultMap->sMapIdx[iResultID]];
			pstResult->ResultID = pstStatusResult->ResultID;
			if( pstResult->ResultID == STATUS_RESULT_STEP_DEC_MSPD )
			{
				pstResult->ResultVal1 += pstZoneStatus->ResultStepDecSpd.ResultVal1;
				pstResult->ResultVal2 += pstZoneStatus->ResultStepDecSpd.ResultVal2;
				pstResult->ResultVal3 += pstZoneStatus->ResultStepDecSpd.ResultVal3;
				pstResult->ResultVal4 += pstZoneStatus->ResultStepDecSpd.ResultVal4;
			}
			else
			{
				pstResult->ResultVal1 += pstStatusResult->ResultVal1*(1+pstZoneStatus->nOverlayMul/100.0);
				pstResult->ResultVal2 += pstStatusResult->ResultVal2*(1+pstZoneStatus->nOverlayMul/100.0);
				pstResult->ResultVal3 += pstStatusResult->ResultVal3*(1+pstZoneStatus->nOverlayMul/100.0);
				pstResult->ResultVal4 += pstStatusResult->ResultVal4*(1+pstZoneStatus->nOverlayMul/100.0);
			}
			
			iVal += pstZoneStatus->iVal;
		}
	}
	
	return iVal;
}

int z_get_mon_status_result_min(ZONESVRENV* pstEnv, Monster *pstMon, int iResultID, int *piVal)
{
	int i;
	int iTmpVal = *piVal;
	int iTmp;
	STATUSRESULT stStatusResultMin;
	STATUSRESULT stStatusResultNormal;
	STATUSRESULT stStatusResultTmp;
	ZoneStatus *pstZoneStatus;
	STATUSDEF *pstStatusDef;
	StatusResultMap *pstResultMap;
	STATUSRESULT *pstStatusResult;
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;

	memset(&stStatusResultMin, 0, sizeof(stStatusResultMin));
	memset(&stStatusResultNormal, 0, sizeof(stStatusResultNormal));

	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			pstStatusResult = &pstStatusDef->Result[(unsigned)pstResultMap->sMapIdx[iResultID]];
			if (pstStatusResult->ResultVal1 < 0 || pstStatusResult->ResultVal2 < 0)
			{
				stStatusResultTmp.ResultVal1 = pstStatusResult->ResultVal1*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultTmp.ResultVal2 = pstStatusResult->ResultVal2*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultTmp.ResultVal3 = pstStatusResult->ResultVal3*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultTmp.ResultVal4 = pstStatusResult->ResultVal4*(1+pstZoneStatus->nOverlayMul/100.0);
				
				iTmp = (*piVal) * (1 + stStatusResultTmp.ResultVal1/100.0) + stStatusResultTmp.ResultVal2;	
				if (iTmp < iTmpVal)
				{
					iTmpVal = iTmp;
					memcpy(&stStatusResultMin, &stStatusResultTmp, sizeof(stStatusResultMin));
				}
			}
			else
			{
				stStatusResultNormal.ResultVal1 += pstStatusResult->ResultVal1*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultNormal.ResultVal2 += pstStatusResult->ResultVal2*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultNormal.ResultVal3 += pstStatusResult->ResultVal3*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultNormal.ResultVal4 += pstStatusResult->ResultVal4*(1+pstZoneStatus->nOverlayMul/100.0);
			}
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			pstStatusResult = &pstStatusDef->Result[(unsigned)pstResultMap->sMapIdx[iResultID]];
			if (pstStatusResult->ResultVal1 < 0 || pstStatusResult->ResultVal2 < 0)
			{
				stStatusResultTmp.ResultVal1 = pstStatusResult->ResultVal1*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultTmp.ResultVal2 = pstStatusResult->ResultVal2*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultTmp.ResultVal3 = pstStatusResult->ResultVal3*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultTmp.ResultVal4 = pstStatusResult->ResultVal4*(1+pstZoneStatus->nOverlayMul/100.0);
				
				iTmp = (*piVal) * (1 + stStatusResultTmp.ResultVal1/100.0) + stStatusResultTmp.ResultVal2;	
				if (iTmp < iTmpVal)
				{
					iTmpVal = iTmp;
					memcpy(&stStatusResultMin, &stStatusResultTmp, sizeof(stStatusResultMin));
				}
			}
			else
			{
				stStatusResultNormal.ResultVal1 += pstStatusResult->ResultVal1*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultNormal.ResultVal2 += pstStatusResult->ResultVal2*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultNormal.ResultVal3 += pstStatusResult->ResultVal3*(1+pstZoneStatus->nOverlayMul/100.0);
				stStatusResultNormal.ResultVal4 += pstStatusResult->ResultVal4*(1+pstZoneStatus->nOverlayMul/100.0);
			}
		}
	}

	*piVal = (*piVal) * (1 + (stStatusResultNormal.ResultVal1+stStatusResultMin.ResultVal1)/100.0) + 
				(stStatusResultNormal.ResultVal2+stStatusResultMin.ResultVal2);
		
	return 0;
}


ZoneStatus* z_is_mon_status_result(ZONESVRENV* pstEnv, Monster *pstMon, int iResultID)
{
	int i;
	ZoneStatus *pstZoneStatus;
	STATUSDEF *pstStatusDef;
	StatusResultMap *pstResultMap;
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;

	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			return pstZoneStatus;
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(iResultID, pstResultMap->sMap))
		{
			return pstZoneStatus;
		}
	}
	
	return NULL;
}

int player_status_qianxin(ZONESVRENV* pstEnv, Player *pstPlayer, int iFlag)
{
	int i, iIndex;
	Monster *pstMon;
	ZoneTeam *pstZoneTeam;
	Player *pstPlayerOther;
	TMEMBLOCK *pstMemBlock;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst = NULL;
	char cType;
	CSPKG stPkg;
	int iPlayerView = 0;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	CSLEAVEVIEW *pstLeaveView = &stPkg.Body.LeaveView;

	pstZoneTeam = player_team_get(pstEnv, pstPlayer);

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if(!pstMapInst)
	{
		return 0;
	}

	//通知自身和队友
	pstActionRes->ActionID = ACTION_QIANXIN;
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->Data.QianXin.Flag = iFlag;
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);

	if (pstZoneTeam)
	{
		player_team_broadcast(pstEnv, pstZoneTeam, &stPkg);
	}
	else
	{
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	if (iFlag)
	{
		pstPlayer->stOnline.State |= CS_STAT_QIANXIN;
		pstLeaveView->ID = pstPlayer->iMemID;
		pstLeaveView->HomeFlag = 0;
		Z_CSHEAD_INIT(&stPkg.Head, LEAVE_VIEW);

		//清除HitTarget，使侍卫不再攻击
		pstPlayer->stOnline.iHitTarget = 0;
	}
	else
	{
		pstPlayer->stOnline.State &= ~CS_STAT_QIANXIN;
	}

	if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE)
	{
		if (pstMapInst && pstMapInst->bBroadcast)
		{
			ZONEPWORLDINST *pstPworldInst;
			pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
			if (pstPworldInst)
			{
				iPlayerView = 1;

				for (i = 0; i < pstPworldInst->iPlayer; i++) 
				{
					if (i >= (int)(sizeof(pstPworldInst->aiPlayerID)/sizeof(pstPworldInst->aiPlayerID[0])))
					{
						break;
					}
						
					pstPlayerOther = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i]);
					
					if (NULL == pstPlayerOther || pstPlayerOther == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayerOther->eStatus || pstPlayerOther->stRoleData.Map != pstMapInst->iMapInstID)
					{
						continue;
					}

					if ((pstZoneTeam &&  
						 player_team_get_member(pstZoneTeam, pstPlayerOther->stRoleData.RoleName, NULL) && 
						 (pstPlayer->stOnline.stWarOL.cCampIdx<=0) ) ||
						(pstPlayer->stOnline.stWarOL.cCampIdx > 0 && pstPlayer->stOnline.stWarOL.cCampIdx == pstPlayerOther->stOnline.stWarOL.cCampIdx))
					{
						continue;
					}
					
					if (iFlag)
					{
						z_cltmsg_send(pstEnv, pstPlayerOther, &stPkg);
					}
					else
					{
						z_player_view_player(pstEnv->pstAppCtx, pstEnv, pstPlayerOther, pstPlayer);
					}
				}
			}
		}
	}
	
	//处理压力表以及角色视野
	for (i=0; i<pstPlayer->stOnline.iVAnimate; i++)
	{
		iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.aiVAnimate[i], &cType);
		if (iIndex < 0)
		{
			continue;
		}

		switch (cType)
		{
			case OBJ_PLAYER:
				if (iPlayerView)
				{
					break;
				}
				
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					break;
				}

				pstPlayerOther = (Player *)pstMemBlock->szData;
				if ((pstZoneTeam &&  
					 player_team_get_member(pstZoneTeam, pstPlayerOther->stRoleData.RoleName, NULL) && 
					 (pstPlayer->stOnline.stWarOL.cCampIdx<=0) ) ||
					 (pstPlayer->stOnline.stWarOL.cCampIdx > 0 && pstPlayer->stOnline.stWarOL.cCampIdx == pstPlayerOther->stOnline.stWarOL.cCampIdx))
				{
					break;
				}

				if (iFlag)
				{
					z_cltmsg_send(pstEnv, pstPlayerOther, &stPkg);
				}
				else
				{
					z_player_view_player(pstEnv->pstAppCtx, pstEnv, pstPlayerOther, pstPlayer);
				}
				
				break;
			case OBJ_MONSTER:
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					break;
				}

				pstMon = (Monster *)pstMemBlock->szData;
				
				if (iFlag)
				{
					if(is_mon_machine(pstMon))
					{
						obj_leave_machine_view_send(pstEnv, pstMon, pstMapInst, pstPlayer->iMemID);
					}
					//mon_press_delete_player(pstEnv, pstMon, pstPlayer);
				}
				else
				{
					mon_press_update_player_in(pstEnv, pstMon, pstPlayer);
					if(is_mon_machine(pstMon))
					{
						machine_view_player(pstEnv->pstAppCtx, pstEnv, pstMon, pstPlayer);
					}
				}
				break;
			default:
				break;
		}
	}

	if(iFlag)
	{
		player_view_mon_delpress(pstEnv, pstPlayer);
	}
	
	return 0;
}

int z_player_add_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, STATUSDEF *pstStatusDef, int iLast, int  iApplyID,ZoneStatus** ppstZoneStatus)
{
	int i;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	ZoneStatus *pstZoneStatus;
	ZoneAni stAni;

	stAni.iType = OBJ_PLAYER;
	stAni.stObj.pstPlayer = pstPlayer;
	
	if (STATUS_TYPE_GOOD == pstStatusDef->Type)
	{
		if (pstStatusList->bGoodStatusNum >= MAX_GOOD_STATUS)
		{
			memmove(pstStatusList->astGoodStatus, pstStatusList->astGoodStatus+1, sizeof(pstStatusList->astGoodStatus[0])*(MAX_GOOD_STATUS-1));
			pstZoneStatus = pstStatusList->astGoodStatus+(MAX_GOOD_STATUS-1);
			pstStatusList->bGoodStatusNum = MAX_GOOD_STATUS;
		}
		else
		{
			pstZoneStatus = pstStatusList->astGoodStatus+pstStatusList->bGoodStatusNum;
			pstStatusList->bGoodStatusNum++;
		}

		if (pstStatusDef->StatusID == DIE_REMIT_BUFFID)
		{
			// 这个buff借用result中时间字段
			z_init_zonestatus(pstAppCtx, pstEnv, pstZoneStatus, pstStatusDef, iLast%10000, 0, iApplyID,&stAni);
			pstZoneStatus->unOverlay = iLast/10000;
		}
		else
		{
			z_init_zonestatus(pstAppCtx, pstEnv, pstZoneStatus, pstStatusDef, iLast, 0, iApplyID,&stAni);
		}
	}
	else if (STATUS_TYPE_BAD == pstStatusDef->Type)
	{
		if (pstStatusList->bBadStatusNum >= MAX_BAD_STATUS)
		{
			memmove(pstStatusList->astBadStatus, pstStatusList->astBadStatus+1, sizeof(pstStatusList->astBadStatus[0])*(MAX_BAD_STATUS-1));
			pstZoneStatus = pstStatusList->astBadStatus+(MAX_BAD_STATUS-1);
			pstStatusList->bBadStatusNum = MAX_BAD_STATUS;
		}
		else
		{
			pstZoneStatus = pstStatusList->astBadStatus+pstStatusList->bBadStatusNum;
			pstStatusList->bBadStatusNum++;
		}

		if (pstStatusDef->StatusID == DIE_REMIT_BUFFID)
		{
			// 这个buff借用result中时间字段
			z_init_zonestatus(pstAppCtx, pstEnv, pstZoneStatus, pstStatusDef, iLast%10000, 0, iApplyID,&stAni);
			pstZoneStatus->unOverlay = iLast/10000;
		}
		else
		{
			z_init_zonestatus(pstAppCtx, pstEnv, pstZoneStatus, pstStatusDef, iLast, 0, iApplyID,&stAni);
		}
	}
	else
	{
		if (pstStatusList->bNoLimitStatusNum >= MAX_NOLIMIT_STATUS)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "pstStatusList->bNoLimitStatusNum %d", pstStatusList->bNoLimitStatusNum);
			return -1;
		}

		pstZoneStatus = pstStatusList->astNoLimitStatus+pstStatusList->bNoLimitStatusNum;
		pstStatusList->bNoLimitStatusNum++;

		if (pstStatusDef->StatusID == DIE_REMIT_BUFFID)
		{
			z_init_zonestatus(pstAppCtx, pstEnv, pstZoneStatus, pstStatusDef, iLast%10000, 0, iApplyID,&stAni);
			pstZoneStatus->unOverlay = iLast/10000;
		}
		else
		{
			z_init_zonestatus(pstAppCtx, pstEnv, pstZoneStatus, pstStatusDef, iLast, 0, iApplyID,&stAni);
		}
	}

	if (ppstZoneStatus )
	{
		(*ppstZoneStatus) = pstZoneStatus;
	}

	if (pstStatusDef->BindSkill.SkillID > 0) //状态有绑定技能
	{
		SKILLDEF *pstSkillDef;

		pstSkillDef = z_find_skill(pstEnv, pstStatusDef->BindSkill.SkillID, pstStatusDef->BindSkill.Level, NULL);
		if(pstSkillDef)
		{
			int iNotify = 0;
			
			if (pstSkillDef->BindSkill == BINDSKILL_TYPE_STATUS)
			{
				iNotify = 1;
			}
			
			z_add_skill(pstAppCtx, pstEnv, pstPlayer, NULL, pstStatusDef->BindSkill.SkillID, pstStatusDef->BindSkill.Level, iNotify);

			//处理状态开始触发技能
			if (pstStatusDef->BindSkill.Trig & STATUS_TRIG_START)
			{
				player_atk_by_svr(pstAppCtx, pstEnv, pstPlayer, pstStatusDef->BindSkill.SkillID, pstZoneStatus->iApplyID);
			}
		}
	}

	if (STATUS_ID_QIANXIN == pstStatusDef->StatusID)
	{
		player_status_qianxin(pstEnv, pstPlayer, 1);
	}

	for (i=0; i<MAX_STATUS_RESULT; i++)
	{
		if (0 == pstStatusDef->Result[i].ResultID)
			break;

		if (STATUS_RESULT_BEATBACK == pstStatusDef->Result[i].ResultID)
		{
			SKILLDEF *pstSkillDef;

			pstSkillDef = z_find_skill(pstEnv, pstStatusDef->Result[i].ResultVal2, pstStatusDef->Result[i].ResultVal1, NULL);

			if (pstSkillDef && pstSkillDef->BeatBack)
			{
				z_add_skill(pstAppCtx, pstEnv, pstPlayer, NULL, pstStatusDef->Result[i].ResultVal2, pstStatusDef->Result[i].ResultVal1, 0);
			}
		}

		if(STATUS_RESULT_ADD_BIND_SKILL == pstStatusDef->Result[i].ResultID)
		{
			SKILLDEF *pstSkillDef;
			pstSkillDef = z_find_skill(pstEnv, pstStatusDef->Result[i].ResultVal1, pstStatusDef->Result[i].ResultVal2, NULL);
			if (pstSkillDef)
			{
				z_add_skill(pstAppCtx, pstEnv, pstPlayer, NULL, pstStatusDef->Result[i].ResultVal1, pstStatusDef->Result[i].ResultVal2, 0);
			}
		}
		
	}

	if(pstStatusDef->StatusFlag & STATUS_BEATK_CLEAN)
	{
		pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_STATUS_TRACE;
	}

	if (pstStatusDef->BindSkill.SkillID && (pstStatusDef->BindSkill.Trig & STATUS_TRIG_BEATK))
	{
		pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_STATUS_TRACE;
	}

	if (pstStatusDef->StatusFlag & STATUS_SKILL_CLEAN)
	{
		pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_SKILL;
	}

	/*
	if (pstStatusDef->StatusFlag & STATUS_HEAVYHIT_CLEAN)
	{
		pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_STATUS_TRACE;
	}*/

	//改变地图要清理状态
	if(pstStatusDef->StatusFlag & STATUS_CHG_MAP_CLEAN)
	{
		pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_STATUS_CHGMAP_CLEAN;
	}

	//被控制状态攻击
	if (is_ctrl_status(pstStatusDef))
	{
		player_trace_act_break_prepare(pstEnv, pstPlayer, BRK_PRE_ACT_BEATK_CTRLSTATUS);
	}

	if(pstStatusDef->IsCltShow == 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_STATUS203, pstStatusDef->Name);
	}

	if(z_get_status_result(pstStatusDef, STATUS_RESULT_PET_ADD_ATK))
	{
		ROLEPET *pstRolePet = pet_get_camp(pstEnv, pstPlayer);
		if(pstRolePet)
		{
			pet_update_online(pstEnv, pstPlayer, pstRolePet);
		}
	}

	task_player_trace(pstEnv, pstPlayer, TASK_OBJ_STATUS_OVERLAY);
	return 0;
}

int z_pet_add_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, STATUSDEF *pstStatusDef, int iLast)
{
	ZoneStatus *pstZoneStatus;
	PetStatusInfo *pstStatusList;
	PetFightShiOnline *pstPetFightShiOnline;

	pstPetFightShiOnline = pet_find_fight_online(pstPlayer, pstRolePet);
	if(NULL == pstPetFightShiOnline)
	{
		return -1;
	}
	pstStatusList = &pstPetFightShiOnline->stStatusInfo;
	
	if (STATUS_TYPE_GOOD == pstStatusDef->Type)
	{
		if (pstStatusList->bGoodStatusNum>= MAX_PET_GOOD_STATUS)
		{
			memmove(pstStatusList->astGoodStatus, pstStatusList->astGoodStatus+1, sizeof(pstStatusList->astGoodStatus[0])*(MAX_PET_GOOD_STATUS-1));
			pstZoneStatus = pstStatusList->astGoodStatus+(MAX_PET_GOOD_STATUS-1);
			z_init_pet_status(pstAppCtx, pstEnv, pstZoneStatus, pstStatusDef, iLast, 0);
			pstStatusList->bGoodStatusNum= MAX_PET_GOOD_STATUS;
		}
		else
		{
			pstZoneStatus = pstStatusList->astGoodStatus+pstStatusList->bGoodStatusNum;
			z_init_pet_status(pstAppCtx, pstEnv, pstZoneStatus, pstStatusDef, iLast, 0);
			pstStatusList->bGoodStatusNum++;
		}
	}
	
	return 0;
}
// iFlag 1是加状态的时候，0是减状态的时候
int z_mon_chg_status_skill(ZONESVRENV* pstEnv, Monster* pstMon,STATUSDEF *pstStatusDef,int iFlag)
{
	MONSTERSTATESKILLDEF *pstMonStateSkillDef = NULL;
	STATUSRESULT *pstStatusResult = NULL;
	
	pstStatusResult = z_get_status_result(pstStatusDef, STATUS_RESULT_MON_ADD_SKILL);
	if(!pstStatusResult)
	{
		return -1;
	}
	if(!pstStatusResult->ResultVal2)
	{
		return -1;
	}
	
	if(iFlag == 0 && pstStatusResult->ResultVal1 == 0)
	{
		//把技能刷到出生时
		mon_refresh_skill(pstEnv->pstAppCtx, pstEnv, pstMon);
	}
	else if(iFlag == 1)
	{
		pstMonStateSkillDef = z_find_monsterstateskill_def(pstEnv, pstStatusResult->ResultVal2, NULL);
		if(!pstMonStateSkillDef)
		{
			return -1;
		}
			
		//刷状态技能
		mon_refresh_state_skill( pstEnv, pstMon,pstMonStateSkillDef,1);
	}

	return 0;
}
int z_mon_add_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, STATUSDEF *pstStatusDef, int iLast, int  iApplyID)
{
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;
	ZoneAni stAni;
	
	stAni.iType = OBJ_MONSTER;
	stAni.stObj.pstMon= pstMon;
	
	if (STATUS_TYPE_GOOD == pstStatusDef->Type || STATUS_TYPE_OTHER == pstStatusDef->Type)
	{
		if (pstStatusList->bGoodStatusNum >= MAX_MON_GOOD_STATUS)
		{
			memmove(pstStatusList->astGoodStatus, pstStatusList->astGoodStatus+1, sizeof(pstStatusList->astGoodStatus[0])*(MAX_MON_GOOD_STATUS-1));
			z_init_zonestatus(pstAppCtx, pstEnv, pstStatusList->astGoodStatus+(MAX_MON_GOOD_STATUS-1), pstStatusDef, iLast, 0, iApplyID,&stAni);
			pstStatusList->bGoodStatusNum = MAX_MON_GOOD_STATUS;
		}
		else
		{
			z_init_zonestatus(pstAppCtx, pstEnv, pstStatusList->astGoodStatus+pstStatusList->bGoodStatusNum, pstStatusDef, iLast, 0, iApplyID,&stAni);
			pstStatusList->bGoodStatusNum++;
		}
	}
	else if (STATUS_TYPE_BAD == pstStatusDef->Type)
	{
		if (pstStatusList->bBadStatusNum >= MAX_BAD_STATUS)
		{
			memmove(pstStatusList->astBadStatus, pstStatusList->astBadStatus+1, sizeof(pstStatusList->astBadStatus[0])*(MAX_BAD_STATUS-1));
			z_init_zonestatus(pstAppCtx, pstEnv, pstStatusList->astBadStatus+(MAX_BAD_STATUS-1), pstStatusDef, iLast, 0, iApplyID,&stAni);
			pstStatusList->bBadStatusNum = MAX_BAD_STATUS;
		}
		else
		{
			z_init_zonestatus(pstAppCtx, pstEnv, pstStatusList->astBadStatus+pstStatusList->bBadStatusNum, pstStatusDef, iLast, 0, iApplyID,&stAni);
			pstStatusList->bBadStatusNum++;
		}
	}
	else
	{
		return -1;
	}

	if(pstStatusDef->StatusFlag & STATUS_BEATK_CLEAN)
	{
		pstMon->cMonMiscFlag |= MON_MISC_FLAG_STATUS_TRACE;
	}

	if (pstStatusDef->BindSkill.SkillID && (pstStatusDef->BindSkill.Trig & STATUS_TRIG_BEATK))
	{
		pstMon->cMonMiscFlag |= MON_MISC_FLAG_STATUS_TRACE;
	}

	//处理状态开始触发技能
	if (pstStatusDef->BindSkill.SkillID && (pstStatusDef->BindSkill.Trig & STATUS_TRIG_START))
	{
		mon_svr_atk(pstEnv, pstMon, pstStatusDef->BindSkill.SkillID, pstStatusDef->BindSkill.Level, iApplyID);
	}

	z_mon_chg_status_skill(pstEnv, pstMon,pstStatusDef,1);


	//被控制状态攻击
	if (is_ctrl_status(pstStatusDef) && is_mon_machine(pstMon))
	{
		Player *pstPlayer = machine_get_driver(pstEnv, pstMon);
		if(pstPlayer)
		{
			player_trace_act_break_prepare(pstEnv, pstPlayer, BRK_PRE_ACT_BEATK_CTRLSTATUS);
		}
	}
	
	return 0;
}

int z_player_del_wushuang_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{

	int i;
	ZoneStatus *pstZoneStatus;
	STATUSDEF *pstStatusDef;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	int iFlag = 0;

	for (i=(int)pstStatusList->bGoodStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (pstStatusDef && pstStatusDef->IsWushuang > 0)
		{
			iFlag = 1;
			//z_player_del_status(pstAppCtx, pstEnv, pstPlayer, pstStatusDef->StatusID, 1);
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astGoodStatus, i, &pstStatusList->bGoodStatusNum);
		}
	}

	for (i=(int)pstStatusList->bBadStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
				
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (pstStatusDef && pstStatusDef->IsWushuang > 0)
		{
			iFlag = 1;
			//z_player_del_status(pstAppCtx, pstEnv, pstPlayer, pstStatusDef->StatusID, 1);
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astBadStatus, i, &pstStatusList->bBadStatusNum);
		}
	}

	for (i=(int)pstStatusList->bNoLimitStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		
		if (pstStatusDef && pstStatusDef->IsWushuang > 0)
		{
			iFlag = 1;
			//z_player_del_status(pstAppCtx, pstEnv, pstPlayer, pstStatusDef->StatusID, 1);
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astNoLimitStatus, i, &pstStatusList->bNoLimitStatusNum);
		}
	
	}

	if (iFlag)
	{
		z_player_send_status(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);
	}

	return 0;
}

int z_del_apply_unique_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	SKILLDEF *pstSkillDef;
	SKILLRESULT *pstSkillResult;
	ZoneSkill *pstZoneSkill;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;

	for (i=0; i<pstZoneSkillList->nSkillNum; i++)
	{
		pstZoneSkill = &pstZoneSkillList->astZoneSkills[i];
		if (pstZoneSkill->iTagAniID)
		{
			pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill);
			if (!pstSkillDef)
			{
				continue;
			}
			
			pstSkillResult  = z_skill_get_result(pstSkillDef, RESULT_UNIQUE_STATUS);
			if (pstSkillResult)
			{
				z_del_unique_status(pstAppCtx, pstEnv, pstPlayer, pstZoneSkill->iTagAniID, pstSkillResult->SkillResultVal2);
			}	
		}
	}

	return 0;
}

int player_del_status_by_idx(ZONESVRENV* pstEnv, Player *pstPlayer,
                             int iStatusID, int iIdx)
{
	int fChanged = 0;
	ZoneStatus *pstZoneStatus = NULL;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;

	if(iIdx < 0 || iIdx >= pstStatusList->bGoodStatusNum)
	{
		goto next_1;
	}

	pstZoneStatus = pstStatusList->astGoodStatus + iIdx;
	if(pstZoneStatus->unStatusID == iStatusID)
	{
		z_player_status_clear(pstEnv->pstAppCtx,
		                      pstEnv,
		                      pstPlayer,
		                      pstStatusList->astGoodStatus,
		                      iIdx,
		                      &pstStatusList->bGoodStatusNum);
		fChanged = 1;
		goto final;
	}

next_1:
	if(iIdx < 0 || iIdx >= pstStatusList->bNoLimitStatusNum)
	{
		goto next_2;
	}

	pstZoneStatus = pstStatusList->astNoLimitStatus + iIdx;
	if(pstZoneStatus->unStatusID == iStatusID)
	{
		z_player_status_clear(pstEnv->pstAppCtx,
		                      pstEnv,
		                      pstPlayer,
		                      pstStatusList->astNoLimitStatus,
		                      iIdx,
		                      &pstStatusList->bNoLimitStatusNum);
		fChanged = 1;
		goto final;
	}

next_2:
	// Others :

final:
	if(fChanged)
	{
		z_player_send_status(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	//取消了自动寻路对应的buff
	{
		AutoMove* pstAutoMove = &pstPlayer->stOnline.stAutoMoveInfo;
		if (iStatusID == pstAutoMove->iStatusID && pstAutoMove->iMapID)
		{
			if(pstPlayer->stOnline.cMiscFlag & MISC_FLAG_TMPITEM_PATROL_TRACE)
			{
				task_player_trace(pstEnv, pstPlayer, TASK_OBJ_TMPITEM_PATROL_END);
			}

			if (pstAutoMove->iTmpItemID != 0)
			{
				//弄到脚本里去删除了
				tmp_item_del(pstEnv,pstPlayer);
				pstAutoMove->iTmpItemID = 0;
			}

			pstAutoMove->iMapID = 0;

			{
				CSPKG stPkg;
				CSACTIONRES *pstAction = &stPkg.Body.ActionRes;

				memset(pstAction,0,sizeof(*pstAction));

				pstAction->ID = pstPlayer->iMemID;
				pstAction->ActionID = ACTION_CLIENT_CMD;
				
				pstAction->Data.ClientCmd.CmdType = CLIENT_CMD_AUTO_MOVE;
				pstAction->Data.ClientCmd.CmdData.AutoMove.MapID = pstAutoMove->iMapID;
				pstAction->Data.ClientCmd.CmdData.AutoMove.X = pstAutoMove->iX;
				pstAction->Data.ClientCmd.CmdData.AutoMove.Y = pstAutoMove->iY;
				pstAction->Data.ClientCmd.CmdData.AutoMove.Stop = 1;

				Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
				z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
			}

			memset(pstAutoMove,0,sizeof(*pstAutoMove));

			z_player_stop(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, NULL);
		}
	}
	
	return 0;
}

int z_player_del_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iStatusID, int iNotify, int iApplyID)
{
	int i;
	int iFlag = 0;
	ZoneStatus *pstZoneStatus;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;

	for (i=pstStatusList->bGoodStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		if (iApplyID != 0 && pstZoneStatus->iApplyID!= iApplyID)
		{
			continue;
		}

		if (pstZoneStatus->unStatusID == iStatusID)
		{
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astGoodStatus, i, &pstStatusList->bGoodStatusNum);
			iFlag = 1;
		}
	}

	if (iFlag)
	{
		if (iNotify)
		{
			z_player_send_status(pstAppCtx, pstEnv, pstPlayer);
		}

		z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);


		return 0;
	}

	for (i=pstStatusList->bBadStatusNum - 1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;

		if (iApplyID != 0 && pstZoneStatus->iApplyID != iApplyID)
		{
			continue;
		}
				
		if (pstZoneStatus->unStatusID == iStatusID)
		{
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astBadStatus, i, &pstStatusList->bBadStatusNum);

			iFlag = 1;
		}
	}

	if (iFlag)
	{
		if (iNotify)
		{
			z_player_send_status(pstAppCtx, pstEnv, pstPlayer);
		}

		z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);

	
		return 0;
	}

	for (i=pstStatusList->bNoLimitStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;

		if (iApplyID != 0 && pstZoneStatus->iApplyID != iApplyID)
		{
			continue;
		}

		if (pstZoneStatus->unStatusID == iStatusID)
		{
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astNoLimitStatus, i, &pstStatusList->bNoLimitStatusNum);

			iFlag = 1;
		}
	}

	if (iFlag)
	{
		if (iNotify)
		{
			z_player_send_status(pstAppCtx, pstEnv, pstPlayer);
		}

		z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);

	

		return 0;
	}

	return -1;
}

int z_player_del_status_level(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iStatusID, int iLevel, int iNotify, int iApplyID)
{
	int i;
	int iFlag = 0;
	ZoneStatus *pstZoneStatus;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;

	for (i=pstStatusList->bGoodStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		if (iApplyID != 0 && pstZoneStatus->iApplyID!= iApplyID)
		{
			continue;
		}

		if (pstZoneStatus->unStatusID == iStatusID && pstZoneStatus->bLevel == iLevel)
		{
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astGoodStatus, i, &pstStatusList->bGoodStatusNum);
			iFlag = 1;
		}
	}

	if (iFlag)
	{
		if (iNotify)
		{
			z_player_send_status(pstAppCtx, pstEnv, pstPlayer);
		}

		z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);

		return 0;
	}

	for (i=pstStatusList->bBadStatusNum - 1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;

		if (iApplyID != 0 && pstZoneStatus->iApplyID != iApplyID)
		{
			continue;
		}
				
		if (pstZoneStatus->unStatusID == iStatusID && pstZoneStatus->bLevel == iLevel)
		{
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astBadStatus, i, &pstStatusList->bBadStatusNum);

			iFlag = 1;
		}
	}

	if (iFlag)
	{
		if (iNotify)
		{
			z_player_send_status(pstAppCtx, pstEnv, pstPlayer);
		}

		z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);

		return 0;
	}

	for (i=pstStatusList->bNoLimitStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;

		if (iApplyID != 0 && pstZoneStatus->iApplyID != iApplyID)
		{
			continue;
		}

		if (pstZoneStatus->unStatusID == iStatusID && pstZoneStatus->bLevel == iLevel)
		{
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astNoLimitStatus, i, &pstStatusList->bNoLimitStatusNum);

			iFlag = 1;
		}
	}

	if (iFlag)
	{
		if (iNotify)
		{
			z_player_send_status(pstAppCtx, pstEnv, pstPlayer);
		}

		z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);

		return 0;
	}

	return -1;
}

int z_player_del_status_seq(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iNotify, int iSeq)
{
	int i;
	int iFlag = 0;
	ZoneStatus *pstZoneStatus;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;

	for (i=pstStatusList->bGoodStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		if (pstZoneStatus->iSeqID != iSeq)
		{
			continue;
		}

		{
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astGoodStatus, i, &pstStatusList->bGoodStatusNum);
			iFlag = 1;
			break;
		}
	}

	if (iFlag)
	{
		/*
		if (iNotify)
		{
			z_player_send_status(pstAppCtx, pstEnv, pstPlayer);
		}

		z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);*/

		return 0;
	}

	for (i=pstStatusList->bBadStatusNum - 1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;

		if (pstZoneStatus->iSeqID != iSeq)
		{
			continue;
		}
		
		{
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astBadStatus, i, &pstStatusList->bBadStatusNum);

			iFlag = 1;
			break;
		}
	}

	if (iFlag)
	{
		/*
		if (iNotify)
		{
			z_player_send_status(pstAppCtx, pstEnv, pstPlayer);
		}

		z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);
		*/
	
		return 0;
	}

	for (i=pstStatusList->bNoLimitStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;

		if (pstZoneStatus->iSeqID != iSeq)
		{
			continue;
		}
		
		{
			z_player_status_clear(pstAppCtx, pstEnv, pstPlayer, pstStatusList->astNoLimitStatus, i, &pstStatusList->bNoLimitStatusNum);

			iFlag = 1;
			break;
		}
	}

	if (iFlag)
	{
		/*
		if (iNotify)
		{
			z_player_send_status(pstAppCtx, pstEnv, pstPlayer);
		}

		z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);
		*/

		return 0;
	}

	return -1;
	UNUSED(iNotify);	//add by paraunused.pl
}


int z_pet_del_status_level(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
								ROLEPET *pstRolePet, int iStatusID, int iLevel, int iNotify)
{
	int i;
	int iFlag = 0;
	ZoneStatus *pstZoneStatus;
	PetStatusInfo *pstStatusList;
	PetFightShiOnline *pstPetFightShiOnline;

	pstPetFightShiOnline = pet_find_fight_online(pstPlayer, pstRolePet);
	if(NULL == pstPetFightShiOnline)
	{
		return -1;
	}
	pstStatusList = &pstPetFightShiOnline->stStatusInfo;

	for (i=pstStatusList->bGoodStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		if (pstZoneStatus->unStatusID == iStatusID && pstZoneStatus->bLevel == iLevel)
		{
			z_pet_status_clear(pstAppCtx, pstEnv, pstRolePet,  pstStatusList->astGoodStatus, i, &pstStatusList->bGoodStatusNum);
			iFlag = 1;
		}
	}

	if (iFlag)
	{
		if (iNotify)
		{
			//z_player_send_status(pstAppCtx, pstEnv, pstPlayer);
		}

		return 0;
	}

	return -1;
}


int z_mon_del_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, int iStatusID, int iApplyID)
{
	int i;
	int iFlag = 0;
	ZoneStatus *pstZoneStatus;
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;

	for (i=pstStatusList->bGoodStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		if (iApplyID != 0 && pstZoneStatus->iApplyID != iApplyID)
		{
			continue;
		}

		if (pstZoneStatus->unStatusID == iStatusID)
		{
			z_mon_status_clear(pstAppCtx, pstEnv, pstMon, pstStatusList->astGoodStatus, i, &pstStatusList->bGoodStatusNum);
			iFlag = 1;
		}
	}

	if (iFlag)
	{
		return 0;
	}
	
	for (i=pstStatusList->bBadStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;

		if (iApplyID != 0 && pstZoneStatus->iApplyID != iApplyID)
		{
			continue;
		}
		
		if (pstZoneStatus->unStatusID == iStatusID)
		{
			z_mon_status_clear(pstAppCtx, pstEnv, pstMon, pstStatusList->astBadStatus, i, &pstStatusList->bBadStatusNum);
			iFlag = 1;
		}
	}

	if (iFlag)
	{
		return 0;
	}

	return -1;
}

int z_mon_del_status_level(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, int iStatusID, int iLevel, int iApplyID)
{
	int i;
	int iFlag = 0;
	ZoneStatus *pstZoneStatus;
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;

	for (i=pstStatusList->bGoodStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		if (iApplyID != 0 && pstZoneStatus->iApplyID != iApplyID)
		{
			continue;
		}

		if (pstZoneStatus->unStatusID == iStatusID && pstZoneStatus->bLevel == iLevel)
		{
			z_mon_status_clear(pstAppCtx, pstEnv, pstMon, pstStatusList->astGoodStatus, i, &pstStatusList->bGoodStatusNum);
			iFlag = 1;
		}
	}

	if (iFlag)
	{
		return 0;
	}
	
	for (i=pstStatusList->bBadStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;

		if (iApplyID != 0 && pstZoneStatus->iApplyID != iApplyID)
		{
			continue;
		}
		
		if (pstZoneStatus->unStatusID == iStatusID && pstZoneStatus->bLevel == iLevel)
		{
			z_mon_status_clear(pstAppCtx, pstEnv, pstMon, pstStatusList->astBadStatus, i, &pstStatusList->bBadStatusNum);
			iFlag = 1;
		}
	}

	if (iFlag)
	{
		return 0;
	}

	return -1;
}

int z_mon_del_status_seq(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, int iSeq)
{
	int i;
	int iFlag = 0;
	ZoneStatus *pstZoneStatus;
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;

	for (i=pstStatusList->bGoodStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		if (iSeq != pstZoneStatus->iSeqID)
		{
			continue;
		}

		{
			z_mon_status_clear(pstAppCtx, pstEnv, pstMon, pstStatusList->astGoodStatus, i, &pstStatusList->bGoodStatusNum);
			iFlag = 1;
			break;
		}
	}

	if (iFlag)
	{
		return 0;
	}
	
	for (i=pstStatusList->bBadStatusNum-1; i>=0; i--)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;

		if (iSeq != pstZoneStatus->iSeqID)
		{
			continue;
		}
		
		{
			z_mon_status_clear(pstAppCtx, pstEnv, pstMon, pstStatusList->astBadStatus, i, &pstStatusList->bBadStatusNum);
			iFlag = 1;
			break;
		}
	}

	if (iFlag)
	{
		return 0;
	}

	return -1;
}
	

void z_get_player_csstatusinfo(TAPPCTX* pstAppCtx, CSSTATUSINFO *pstCSStatusInfo, ZoneStatusList *pstStatusList)
{
	int i;
	struct timeval stDiff;
	long long llMs=0;
	
	pstCSStatusInfo->GoodStatusNum = pstStatusList->bGoodStatusNum;
	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstCSStatusInfo->GoodStatusList[i].Level = pstStatusList->astGoodStatus[i].bLevel;
		pstCSStatusInfo->GoodStatusList[i].StatusID = pstStatusList->astGoodStatus[i].unStatusID;
		pstCSStatusInfo->GoodStatusList[i].Overlay = pstStatusList->astGoodStatus[i].unOverlay;
		pstCSStatusInfo->GoodStatusList[i].ApplyID= pstStatusList->astGoodStatus[i].iApplyID;
		pstCSStatusInfo->GoodStatusList[i].ApplyRelation= pstStatusList->astGoodStatus[i].cApplyRelation;
		{
			TV_DIFF(stDiff, pstStatusList->astGoodStatus[i].stEnd, pstAppCtx->stCurr);
			TV_TO_MS(llMs, stDiff);
			if (llMs > 0)
			{
				pstCSStatusInfo->GoodStatusList[i].Time = llMs/1000;
			}
			else
			{
				pstCSStatusInfo->GoodStatusList[i].Time = 0;
			}
		}
	}

	pstCSStatusInfo->BadStatusNum = pstStatusList->bBadStatusNum;
	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{		
		pstCSStatusInfo->BadStatusList[i].Level = pstStatusList->astBadStatus[i].bLevel;
		pstCSStatusInfo->BadStatusList[i].StatusID = pstStatusList->astBadStatus[i].unStatusID;
		pstCSStatusInfo->BadStatusList[i].Overlay = pstStatusList->astBadStatus[i].unOverlay;
		pstCSStatusInfo->BadStatusList[i].ApplyID= pstStatusList->astBadStatus[i].iApplyID;
		pstCSStatusInfo->BadStatusList[i].ApplyRelation= pstStatusList->astBadStatus[i].cApplyRelation;
		{
			TV_DIFF(stDiff, pstStatusList->astBadStatus[i].stEnd, pstAppCtx->stCurr);
			TV_TO_MS(llMs, stDiff);
			if (llMs > 0)
			{
				pstCSStatusInfo->BadStatusList[i].Time = llMs/1000;
			}
			else
			{
				pstCSStatusInfo->BadStatusList[i].Time = 0;
			}
		}
	}

	pstCSStatusInfo->NoLimitStatusNum = pstStatusList->bNoLimitStatusNum;
	for (i=0; i<pstStatusList->bNoLimitStatusNum; i++)
	{
		pstCSStatusInfo->NolimitStatusList[i].Level = pstStatusList->astNoLimitStatus[i].bLevel;
		pstCSStatusInfo->NolimitStatusList[i].StatusID = pstStatusList->astNoLimitStatus[i].unStatusID;
		pstCSStatusInfo->NolimitStatusList[i].Overlay = pstStatusList->astNoLimitStatus[i].unOverlay;
		pstCSStatusInfo->NolimitStatusList[i].ApplyID= pstStatusList->astNoLimitStatus[i].iApplyID;
		pstCSStatusInfo->NolimitStatusList[i].ApplyRelation= pstStatusList->astNoLimitStatus[i].cApplyRelation;
		 
		{
			TV_DIFF(stDiff, pstStatusList->astNoLimitStatus[i].stEnd, pstAppCtx->stCurr);
			TV_TO_MS(llMs, stDiff);
			if (llMs > 0)
			{
				pstCSStatusInfo->NolimitStatusList[i].Time = llMs/1000;
			}
			else
			{
				pstCSStatusInfo->NolimitStatusList[i].Time = 0;
			}
		}
	}

	return;
}

int z_player_send_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	CSSTATUSINFO *pstCSStatusInfo = &pstActionRes->Data.Status;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_STATUS;

	z_get_player_csstatusinfo(pstAppCtx, pstCSStatusInfo, pstStatusList);
		
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);

	return 0;
}

void z_get_mon_csstatusinfo(TAPPCTX* pstAppCtx, CSSTATUSINFO *pstCSStatusInfo, ZoneMonStatusList *pstStatusList)
{
	int i;
	struct timeval stDiff;
	long long  llMs=0;
	
	pstCSStatusInfo->GoodStatusNum = pstStatusList->bGoodStatusNum;
	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstCSStatusInfo->GoodStatusList[i].Level = pstStatusList->astGoodStatus[i].bLevel;
		pstCSStatusInfo->GoodStatusList[i].StatusID = pstStatusList->astGoodStatus[i].unStatusID;
		pstCSStatusInfo->GoodStatusList[i].Overlay = pstStatusList->astGoodStatus[i].unOverlay;
		pstCSStatusInfo->GoodStatusList[i].ApplyID = pstStatusList->astGoodStatus[i].iApplyID;
		pstCSStatusInfo->GoodStatusList[i].ApplyRelation = pstStatusList->astGoodStatus[i].cApplyRelation;
		
		{
			TV_DIFF(stDiff, pstStatusList->astGoodStatus[i].stEnd, pstAppCtx->stCurr);
			TV_TO_MS(llMs, stDiff);
			if (llMs > 0)
			{
				pstCSStatusInfo->GoodStatusList[i].Time = llMs/1000;
			}
			else
			{
				pstCSStatusInfo->GoodStatusList[i].Time = 0;
			}
		}
	}

	pstCSStatusInfo->BadStatusNum = pstStatusList->bBadStatusNum;
	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstCSStatusInfo->BadStatusList[i].Level = pstStatusList->astBadStatus[i].bLevel;
		pstCSStatusInfo->BadStatusList[i].StatusID = pstStatusList->astBadStatus[i].unStatusID;
		pstCSStatusInfo->BadStatusList[i].Overlay = pstStatusList->astBadStatus[i].unOverlay;
		pstCSStatusInfo->BadStatusList[i].ApplyID= pstStatusList->astBadStatus[i].iApplyID;
		pstCSStatusInfo->BadStatusList[i].ApplyRelation= pstStatusList->astBadStatus[i].cApplyRelation;
		
		{
			TV_DIFF(stDiff, pstStatusList->astBadStatus[i].stEnd, pstAppCtx->stCurr);
			TV_TO_MS(llMs, stDiff);
			if (llMs > 0)
			{
				pstCSStatusInfo->BadStatusList[i].Time = llMs/1000;
			}
			else
			{
				pstCSStatusInfo->BadStatusList[i].Time = 0;
			}
		}
	}

	pstCSStatusInfo->NoLimitStatusNum = 0;
	
	return;
}

void z_get_pet_csstatusinfo(TAPPCTX* pstAppCtx, CSSTATUSINFO *pstCSStatusInfo, PetStatusInfo *pstStatusList)
{
	int i;
	long long  llMs;
	struct timeval stDiff;
	
	pstCSStatusInfo->GoodStatusNum = pstStatusList->bGoodStatusNum;
	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstCSStatusInfo->GoodStatusList[i].Level = pstStatusList->astGoodStatus[i].bLevel;
		pstCSStatusInfo->GoodStatusList[i].StatusID = pstStatusList->astGoodStatus[i].unStatusID;
		pstCSStatusInfo->GoodStatusList[i].Overlay = pstStatusList->astGoodStatus[i].unOverlay;
		pstCSStatusInfo->GoodStatusList[i].ApplyID= pstStatusList->astGoodStatus[i].iApplyID;
		pstCSStatusInfo->GoodStatusList[i].ApplyRelation= pstStatusList->astGoodStatus[i].cApplyRelation;
		
		TV_DIFF(stDiff, pstStatusList->astGoodStatus[i].stEnd, pstAppCtx->stCurr);
		TV_TO_MS(llMs, stDiff);
		if (llMs > 0)
		{
			pstCSStatusInfo->GoodStatusList[i].Time = llMs/1000;
		}
		else
		{
			pstCSStatusInfo->GoodStatusList[i].Time = 0;
		}
	}

	pstCSStatusInfo->BadStatusNum = 0;
	pstCSStatusInfo->NoLimitStatusNum = 0;
	
	return;
}

int z_mon_send_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	CSSTATUSINFO *pstCSStatusInfo = &pstActionRes->Data.Status;
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;

	memset(pstCSStatusInfo,0,sizeof(CSSTATUSINFO));
	
	pstActionRes->ID = pstMon->iID;
	pstActionRes->ActionID = ACTION_STATUS;
		
	z_get_mon_csstatusinfo(pstAppCtx, pstCSStatusInfo, pstStatusList);	

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_monview(pstAppCtx, pstEnv, pstMon, &stPkg, 0);

	return 0;
}

int z_mon_apply_status2(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, int iStatus, int iStatusLevel, int iLast, int iNotify, int  iApplyID)
{
	int i;
	STATUSDEF *pstStatusDef;
	STATUSDEF *pstStatusDefCurr;
	ZoneStatus *pstZoneStatus = NULL;
	int iRelationValue;
	DelStatus astDelStatus[MAX_MON_GOOD_STATUS+MAX_BAD_STATUS+MAX_NOLIMIT_STATUS];
	int iDelStatusNum = 0;
	int iMerged = 0;
	int iMergedIdxType = -1;
	int iMergedIdx = -1;
	int iMergedStatusID = -1;
	int iMergedOverlay = 0;
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;
	ZoneAni stAni;

	stAni.iType = OBJ_MONSTER;
	stAni.stObj.pstMon= pstMon;

	pstStatusDef = z_find_status(pstEnv, iStatus, iStatusLevel);
	if (NULL == pstStatusDef)
	{
		return -1;
	}

	if(z_get_status_result(pstStatusDef, STATUS_RESULT_IMMUNITY_DEBUFF))
	{
		mon_status_clean_debuff(pstAppCtx, pstEnv, pstMon);
	}

	if( z_get_mon_zonestatus_by_resultid(pstEnv,pstMon,STATUS_RESULT_IMMUNITY_DEBUFF) &&
		STATUS_TYPE_BAD == pstStatusDef->Type )
	{
		return -1;	
	}
		
	
	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		pstStatusDefCurr = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDefCurr)
		{
			continue;
		}

		if(pstStatusDefCurr->StatusID == STATUS_ID_HIDE_IGNORE &&
			(pstStatusDef->StatusFlag & STATUS_CAN_IGNORE))
		{
			return -1;
		}

		if(pstStatusDef->StatusID == STATUS_ID_HIDE_IGNORE)
		{
			if(pstStatusDefCurr->StatusFlag & STATUS_CAN_IGNORE)
			{
				astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
				astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
				iDelStatusNum++;
			}
		}
		else
		{
			iRelationValue = z_overlaytype_val(pstEnv, pstStatusDef, pstStatusDefCurr);
			switch(iRelationValue)
			{
				case OVERLAY_FLAG_LEVEL_REJECT:
					if (pstStatusDefCurr->Level > pstStatusDef->Level)
					{
						return -1;
					}
					break;
					
				case OVERLAY_FLAG_SAME_REJECT:
					if (pstStatusDef == pstStatusDefCurr)
					{
						return -1;
					}
					break;

				case OVERLAY_FLAG_SAME_OVERLAY:
					if (pstStatusDef == pstStatusDefCurr)
					{
						if (iMerged)
						{
							continue;
						}

						iMergedIdxType = 0;
						iMergedIdx = i;
						iMergedStatusID = iStatus;
						if (pstZoneStatus->unOverlay < pstStatusDef->OverlayLimit)
						{
							iMergedOverlay = 1;
						}

						iMerged = 1;
					}
					break;

				case OVERLAY_FLAG_SAME_ADDTIME:
					if (pstStatusDef == pstStatusDefCurr)
					{
						if (iMerged)
						{
							continue;
						}

						iMergedIdxType = 0;
						iMergedIdx = i;
						iMergedStatusID = iStatus;
						
						if (pstZoneStatus->stEnd.tv_sec > pstEnv->pstAppCtx->stCurr.tv_sec)
						{
							iLast += (pstZoneStatus->stEnd.tv_sec - pstEnv->pstAppCtx->stCurr.tv_sec);
						}
						
						iMerged = 1;
					}
					break;	

				case OVERLAY_FLAG_SAME_REPLACE:
					if (pstStatusDef == pstStatusDefCurr)
					{
						if (iMerged)
						{
							continue;
						}

						iMergedIdxType = 0;
						iMergedIdx = i;
						iMergedStatusID = iStatus;

						iMerged = 1;
					}
					break;

				case OVERLAY_FLAG_REJECT:
					return -1;
					break;

				case OVERLAY_FLAG_REPLACE:
					if (pstStatusDef->StatusID == pstStatusDefCurr->StatusID)
					{
						if (iMerged)
						{
							continue;
						}

						iMergedIdxType = 0;
						iMergedIdx = i;
						iMergedStatusID = iStatus;

						iMerged = 1;
					}
					break;

				case OVERLAY_FLAG_CLEAR:
					astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
					astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
					iDelStatusNum++;
					break;

				case OVERLAY_FLAG_LEVEL_CLEAR:
					if (pstStatusDef->Level >= pstStatusDefCurr->Level)
					{
						astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
						astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
						iDelStatusNum++;
					}
					break;

				case OVERLAY_FLAG_LEVEL_REJECT_CLEAR:
					if (pstStatusDefCurr->Level > pstStatusDef->Level)
					{
						return -1;
					}
					else
					{
						astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
						astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
						iDelStatusNum++;
					}
					
					break;	
				
				default:
					break;
			}
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
		pstStatusDefCurr = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDefCurr)
		{
			continue;
		}

		if(pstStatusDefCurr->StatusID == STATUS_ID_HIDE_IGNORE &&
			(pstStatusDef->StatusFlag & STATUS_CAN_IGNORE))
		{
			return -1;
		}

		if(pstStatusDef->StatusID == STATUS_ID_HIDE_IGNORE)
		{
			if(pstStatusDefCurr->StatusFlag & STATUS_CAN_IGNORE)
			{
				astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
				astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
				iDelStatusNum++;
			}
		}
		else
		{
			iRelationValue = z_overlaytype_val(pstEnv, pstStatusDef, pstStatusDefCurr);
			switch(iRelationValue)
			{
				case OVERLAY_FLAG_LEVEL_REJECT:
					if (pstStatusDefCurr->Level > pstStatusDef->Level)
					{
						return -1;
					}
					break;
					
				case OVERLAY_FLAG_SAME_REJECT:
					if (pstStatusDef == pstStatusDefCurr)
					{
						return -1;
					}
					break;

				case OVERLAY_FLAG_SAME_OVERLAY:
					if (pstStatusDef == pstStatusDefCurr)
					{
						if (iMerged)
						{
							continue;
						}

						iMergedIdxType = 1;
						iMergedIdx = i;
						iMergedStatusID = iStatus;
						if (pstZoneStatus->unOverlay < pstStatusDef->OverlayLimit)
						{
							iMergedOverlay = 1;
						}

						iMerged = 1;
					}
					break;

				case OVERLAY_FLAG_SAME_ADDTIME:
					if (pstStatusDef == pstStatusDefCurr)
					{
						if (iMerged)
						{
							continue;
						}

						iMergedIdxType = 1;
						iMergedIdx = i;
						iMergedStatusID = iStatus;
						if (pstZoneStatus->stEnd.tv_sec > pstEnv->pstAppCtx->stCurr.tv_sec)
						{
							iLast += (pstZoneStatus->stEnd.tv_sec - pstEnv->pstAppCtx->stCurr.tv_sec);
						}

						iMerged = 1;
					}
					break;	

				case OVERLAY_FLAG_SAME_REPLACE:
					if (pstStatusDef == pstStatusDefCurr)
					{
						if (iMerged)
						{
							continue;
						}

						iMergedIdxType = 1;
						iMergedIdx = i;
						iMergedStatusID = iStatus;

						iMerged = 1;
					}
					break;

				case OVERLAY_FLAG_REJECT:
					return -1;
					break;

				case OVERLAY_FLAG_REPLACE:
					if (pstStatusDef->StatusID == pstStatusDefCurr->StatusID)
					{
						if (iMerged)
						{
							continue;
						}

						iMergedIdxType = 1;
						iMergedIdx = i;
						iMergedStatusID = iStatus;

						iMerged = 1;
					}
					break;

				case OVERLAY_FLAG_CLEAR:
					astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
					astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
					iDelStatusNum++;
					break;

				case OVERLAY_FLAG_LEVEL_CLEAR:
					if (pstStatusDef->Level >= pstStatusDefCurr->Level)
					{
						astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
						astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
						iDelStatusNum++;
					}
					break;

				case OVERLAY_FLAG_LEVEL_REJECT_CLEAR:
					if (pstStatusDefCurr->Level > pstStatusDef->Level)
					{
						return -1;
					}
					else
					{
						astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
						astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
						iDelStatusNum++;
					}
					
					break;	
				
				default:
					break;
			}
		}
	}

	for (i=0; i<iDelStatusNum; i++)
	{
		z_mon_del_status_level(pstAppCtx, pstEnv, pstMon, astDelStatus[i].unStatusID, astDelStatus[i].unLevel, 0);
	}

	if (iMerged)
	{
		switch(iMergedIdxType)
		{
			case 0:
				pstZoneStatus = pstStatusList->astGoodStatus+iMergedIdx;
				break;
			case 1:
				pstZoneStatus = pstStatusList->astBadStatus+iMergedIdx;
				break;
			default:
				pstZoneStatus = NULL;
				break;
		}

		if (pstZoneStatus)
		{
			if (pstZoneStatus->unStatusID != iMergedStatusID || pstZoneStatus->bLevel != iStatusLevel)
			{
				pstStatusDef = z_find_status(pstEnv, iMergedStatusID, iStatusLevel);
				if (NULL == pstStatusDef)
				{
					return -1;
				}

				z_init_zonestatus(pstAppCtx, pstEnv, pstZoneStatus, pstStatusDef, iLast, pstZoneStatus->unOverlay+iMergedOverlay, iApplyID,&stAni);
				z_mon_chg_status_skill(pstEnv, pstMon,pstStatusDef,1);
			}
			else
			{
				STATUSOVERLAYDEF *pstStatusOverlay;
				
				pstZoneStatus->unOverlay += iMergedOverlay;
				if (pstZoneStatus->unOverlay > 0)
				{
					pstStatusOverlay = z_find_status_overlay(pstEnv, pstStatusDef->StatusID, pstStatusDef->Level, pstZoneStatus->unOverlay);
					if (pstStatusOverlay)
					{
						pstZoneStatus->nOverlayMul = pstStatusOverlay->Mul;
						
					}
					pstZoneStatus->iJumpedCount = 0;
				}
				pstZoneStatus->MiscType &= ~ZONESTATUS_MISCTYPE_CHUANRAN_FLAG;
			
				pstZoneStatus->stEnd.tv_sec = pstAppCtx->stCurr.tv_sec + iLast;
				pstZoneStatus->stEnd.tv_usec = pstAppCtx->stCurr.tv_usec;
				if (iApplyID > 0)
				{
					pstZoneStatus->iApplyID = iApplyID;
				}
			}
		}
	}
	else
	{
		z_mon_add_status(pstAppCtx, pstEnv, pstMon, pstStatusDef, iLast, iApplyID);	
	}

	z_mon_chk_mspd(pstAppCtx, pstEnv, pstMon, 1);

	if (iNotify)
	{
		z_mon_send_status(pstAppCtx, pstEnv, pstMon);
	}

	if (pstZoneStatus && 
		pstZoneStatus->unOverlay > 0)
	{	
		STATUSRESULT *pstStatusResultOverlay;
		pstStatusResultOverlay = z_get_status_result(pstStatusDef, STATUS_RESULT_ADD_STATUS_BY_OVERLAY);
		if( pstStatusResultOverlay &&
			(pstZoneStatus->unOverlay+1) >= pstStatusResultOverlay->ResultVal1 &&
			pstStatusResultOverlay->ResultVal2 != iStatus )
		{
			STATUSDEF *pstStatusDefTmp = z_find_status(pstEnv, pstStatusResultOverlay->ResultVal2, pstStatusResultOverlay->ResultVal3);
			if (!pstStatusDef)
			{
				return -1;
			}
			if(z_get_status_result(pstStatusDefTmp, STATUS_RESULT_ADD_STATUS_BY_OVERLAY))
			{
				//防止策划填了嵌套的逻辑
				return -1;
			}
			z_mon_apply_status(pstAppCtx, pstEnv, pstMon, pstStatusResultOverlay->ResultVal2, 
				pstStatusResultOverlay->ResultVal3, pstStatusResultOverlay->ResultVal4, iNotify, iApplyID);
		}
	}

	//宏福活动
	{
		STATUSRESULT* pstResult;
		StatusResultMap* pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));
		if (IS_STATUS_RESULTMAP_SET(STATUS_RESULT_DEL_TRANS, pstResultMap->sMap))
		{
			ZONEIDX stIdx;
			MapIndex* pstMapIndex;
			MONSTERDEF* pstMonDef;

			stIdx = pstMon->stMap;

			pstMapIndex =	z_mappos_index(pstEnv->pstMapObj, &stIdx.iPos, stIdx.iID);
			pstMonDef = z_get_mon_def(pstEnv,pstMon);
			pstResult = z_get_status_result( pstStatusDef, STATUS_RESULT_DEL_TRANS);
 			if(pstResult &&
 				pstMapIndex && pstMonDef)
			{
				if(pstResult->ResultVal1 & STATUS_TRANS_MAP_NO_PROTECT_LUCKY)
				{
					z_sys_strf_broadcast(pstEnv,SYS_CHAT, ZONEERR_BROAD344,
						pstMapIndex->stMapDef.Name,pstMonDef->Name);
					
					pstMon->cMonMiscFlag |= MON_MISC_FLAG_MAP_POS;
					pstEnv->iLuckyTipCount = 1;
					
					mon_map_pos(pstEnv,pstMon,1);
				}
			}
		}
	}
	return 0;
}
int z_mon_apply_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, int iStatus, int iStatusLevel, int iLast, int iNotify, int  iApplyID)
{
	MONSTERDEF *pstMonDef;

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (pstMonDef && (MON_MISC_TYPE_HIT_ONE_HP & pstMonDef->MiscType))
	{
		if (iApplyID)
		{
			Player *pstPlayer;

			pstPlayer = z_id_player(pstAppCtx, pstEnv, iApplyID);
			if (pstPlayer && pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
									ZONEERR_STATUS201);
			}
		}
		
		return -1;
	}

	z_mon_apply_status2(pstAppCtx, pstEnv, pstMon, iStatus, iStatusLevel, iLast, iNotify, iApplyID);
	return 0;
}


int z_player_apply_status2(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iStatus, int iStatusLevel, int iLast, int iNotify, int iApplyID,ZoneStatus** ppstZoneStatus)
{
	int i;
	STATUSDEF *pstStatusDef;
	STATUSDEF *pstStatusDefCurr;
	ZoneStatus *pstZoneStatus = NULL;
	int iRelationValue;
	DelStatus astDelStatus[MAX_GOOD_STATUS+MAX_BAD_STATUS+MAX_NOLIMIT_STATUS];
	int iDelStatusNum = 0;
	int iMerged = 0;
	int iMergedIdxType = -1;
	int iMergedIdx = -1;
	int iMergedStatusID = -1;
	int iMergedOverlay = 0;
	int iOldHP =0 ;
	int iOldMP = 0;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	ZoneAni stAni;
	StatusResultMap* pstResultMap = NULL;

	stAni.iType = OBJ_PLAYER;
	stAni.stObj.pstPlayer = pstPlayer;

	pstStatusDef = z_find_status(pstEnv, iStatus, iStatusLevel);
	if (NULL == pstStatusDef)
	{
		return -1;
	}

	if(z_get_status_result(pstStatusDef, STATUS_RESULT_CLAN_CITY_BUFF))
	{
		ZoneClan *pstClan = player_get_clan(pstEnv,pstPlayer);
		STATUSDEF *pstTmpDef = NULL;
		if (pstClan && pstClan->stClan.ClanCore.CityStatusID && 
			pstClan->stClan.ClanCore.CityStatusLevel &&
			pstClan->stClan.ClanCore.CityStatusTime)
		{
			pstTmpDef = z_find_status(pstEnv, pstClan->stClan.ClanCore.CityStatusID,
				pstClan->stClan.ClanCore.CityStatusLevel);
		}
		if (pstTmpDef && pstClan)
		{
			pstStatusDef = pstTmpDef;
			iLast = pstClan->stClan.ClanCore.CityStatusTime;
		}
	}

	if (NULL == pstStatusDef)
	{
		return -1;
	}
		
	pstResultMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));

	if(z_get_status_result(pstStatusDef, STATUS_RESULT_IMMUNITY_DEBUFF))
	{
		player_status_clean_debuff(pstEnv, pstPlayer);
	}
	if(z_get_status_result(pstStatusDef, STATUS_RESULT_MAXHP))
	{
		iOldHP = pstPlayer->stRoleData.RoleDetail.HPCurr;
	}
	if(z_get_status_result(pstStatusDef, STATUS_RESULT_MAXMP))
	{
		iOldMP = pstPlayer->stRoleData.RoleDetail.MPCurr;
	}


	if( z_get_player_zonestatus_by_resultid(pstEnv,pstPlayer,STATUS_RESULT_IMMUNITY_DEBUFF) &&
		STATUS_TYPE_BAD == pstStatusDef->Type )
	{
		return -1;	
	}
	
	if (IS_STATUS_RESULTMAP_SET(STATUS_RESULT_CLAN_CITY, pstResultMap->sMap) && 
		! is_in_my_clan_city(pstEnv,pstPlayer))
	{
		return -1;
	}
	
	if(pstStatusDef->IsWushuang > 0 && pstPlayer->stRoleData.MiscInfo.TmpSys.TmpItemID > 0)
	{
		tmp_item_drop(pstEnv, pstPlayer,1);
	}
	
	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		pstStatusDefCurr = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDefCurr)
		{
			continue;
		}

		if(pstStatusDefCurr->StatusID == STATUS_ID_HIDE_IGNORE &&
			(pstStatusDef->StatusFlag & STATUS_CAN_IGNORE))
		{
			return -1;
		}

		if(pstStatusDef->StatusID == STATUS_ID_HIDE_IGNORE)
		{
			if(pstStatusDefCurr->StatusFlag & STATUS_CAN_IGNORE)
			{
				astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
				astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
				iDelStatusNum++;
			}
		}
		else
		{
			iRelationValue = z_overlaytype_val(pstEnv, pstStatusDef, pstStatusDefCurr);
			
			switch(iRelationValue)
			{
				case OVERLAY_FLAG_LEVEL_REJECT:
					if (pstStatusDefCurr->Level > pstStatusDef->Level)
					{
						return -1;
					}
					break;
					
				case OVERLAY_FLAG_SAME_REJECT:
					if (pstStatusDef == pstStatusDefCurr)
					{
						return -1;
					}
					break;

				case OVERLAY_FLAG_SAME_OVERLAY:
					if (pstStatusDef == pstStatusDefCurr)
					{
						if (iMerged)
						{
							continue;
						}

						iMergedIdxType = 0;
						iMergedIdx = i;
						iMergedStatusID = iStatus;
						if (pstZoneStatus->unOverlay < pstStatusDef->OverlayLimit)
						{
							iMergedOverlay = 1;
						}

						iMerged = 1;
					}
					break;

				case OVERLAY_FLAG_SAME_ADDTIME:
					if (pstStatusDef == pstStatusDefCurr)
					{
						if (iMerged)
						{
							continue;
						}

						iMergedIdxType = 0;
						iMergedIdx = i;
						iMergedStatusID = iStatus;

						if (pstZoneStatus->stEnd.tv_sec > pstEnv->pstAppCtx->stCurr.tv_sec)
						{
							iLast += (pstZoneStatus->stEnd.tv_sec - pstEnv->pstAppCtx->stCurr.tv_sec);
						}
						
						iMerged = 1;
					}
					break;	

				case OVERLAY_FLAG_SAME_REPLACE:
					if (pstStatusDef == pstStatusDefCurr)
					{
						if (iMerged)
						{
							continue;
						}

						iMergedIdxType = 0;
						iMergedIdx = i;
						iMergedStatusID = iStatus;

						iMerged = 1;
					}
					break;

				case OVERLAY_FLAG_REJECT:
					return -1;
					break;

				case OVERLAY_FLAG_REPLACE:
					if (pstStatusDef->StatusID == pstStatusDefCurr->StatusID)
					{
						if (iMerged)
						{
							continue;
						}

						iMergedIdxType = 0;
						iMergedIdx = i;
						iMergedStatusID = iStatus;

						iMerged = 1;
					}
					break;

				case OVERLAY_FLAG_CLEAR:
					astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
					astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
					iDelStatusNum++;
					break;

				case OVERLAY_FLAG_LEVEL_CLEAR:
					if (pstStatusDef->Level >= pstStatusDefCurr->Level)
					{
						astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
						astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
						iDelStatusNum++;
					}
					break;

				case OVERLAY_FLAG_LEVEL_REJECT_CLEAR:
					if (pstStatusDefCurr->Level > pstStatusDef->Level)
					{
						return -1;
					}
					else
					{
						astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
						astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
						iDelStatusNum++;
					}
					
					break;	
				
				default:
					break;
			}
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
		pstStatusDefCurr = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDefCurr)
		{
			continue;
		}

		if(pstStatusDefCurr->StatusID == STATUS_ID_HIDE_IGNORE &&
			(pstStatusDef->StatusFlag & STATUS_CAN_IGNORE))
		{
			return -1;
		}

		if(pstStatusDef->StatusID == STATUS_ID_HIDE_IGNORE)
		{
			if(pstStatusDefCurr->StatusFlag & STATUS_CAN_IGNORE)
			{
				astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
				astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
				iDelStatusNum++;
			}
		}
		else
		{
			iRelationValue = z_overlaytype_val(pstEnv, pstStatusDef, pstStatusDefCurr);
			
			switch(iRelationValue)
			{
				case OVERLAY_FLAG_LEVEL_REJECT:
					if (pstStatusDefCurr->Level > pstStatusDef->Level)
					{
						return -1;
					}
					break;
					
				case OVERLAY_FLAG_SAME_REJECT:
					if (pstStatusDef == pstStatusDefCurr)
					{
						return -1;
					}
					break;

				case OVERLAY_FLAG_SAME_OVERLAY:
					if (pstStatusDef == pstStatusDefCurr)
					{
						if (iMerged)
						{
							continue;
						}

						iMergedIdxType = 1;
						iMergedIdx = i;
						iMergedStatusID = iStatus;
						if (pstZoneStatus->unOverlay < pstStatusDef->OverlayLimit)
						{
							iMergedOverlay = 1;
						}

						iMerged = 1;
					}
					break;

				case OVERLAY_FLAG_SAME_ADDTIME:
					if (pstStatusDef == pstStatusDefCurr)
					{
						if (iMerged)
						{
							continue;
						}

						iMergedIdxType = 1;
						iMergedIdx = i;
						iMergedStatusID = iStatus;

						if (pstZoneStatus->stEnd.tv_sec > pstEnv->pstAppCtx->stCurr.tv_sec)
						{
							iLast += (pstZoneStatus->stEnd.tv_sec - pstEnv->pstAppCtx->stCurr.tv_sec);
						}
						
						iMerged = 1;
					}
					break;
		

				case OVERLAY_FLAG_SAME_REPLACE:
					if (pstStatusDef == pstStatusDefCurr)
					{
						if (iMerged)
						{
							continue;
						}

						iMergedIdxType = 1;
						iMergedIdx = i;
						iMergedStatusID = iStatus;

						iMerged = 1;
					}
					break;

				case OVERLAY_FLAG_REJECT:
					return -1;
					break;

				case OVERLAY_FLAG_REPLACE:
					if (pstStatusDef->StatusID == pstStatusDefCurr->StatusID)
					{
						if (iMerged)
						{
							continue;
						}

						iMergedIdxType = 1;
						iMergedIdx = i;
						iMergedStatusID = iStatus;

						iMerged = 1;
					}
					break;

				case OVERLAY_FLAG_CLEAR:
					astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
					astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
					iDelStatusNum++;
					break;

				case OVERLAY_FLAG_LEVEL_CLEAR:
					if (pstStatusDef->Level >= pstStatusDefCurr->Level)
					{
						astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
						astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
						iDelStatusNum++;
					}
					break;

				case OVERLAY_FLAG_LEVEL_REJECT_CLEAR:
					if (pstStatusDefCurr->Level > pstStatusDef->Level)
					{
						return -1;
					}
					else
					{
						astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
						astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
						iDelStatusNum++;
					}
					
					break;
				
				default:
					break;
			}
		}
	}

	for (i=0; i<pstStatusList->bNoLimitStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;

		pstStatusDefCurr = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDefCurr)
		{
			continue;
		}

		if(pstStatusDefCurr->StatusID == STATUS_ID_HIDE_IGNORE &&
			(pstStatusDef->StatusFlag & STATUS_CAN_IGNORE))
		{
			return -1;
		}

		if(pstStatusDef->StatusID == STATUS_ID_HIDE_IGNORE)
		{
			if(pstStatusDefCurr->StatusFlag & STATUS_CAN_IGNORE)
			{
				astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
				astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
				iDelStatusNum++;
			}
		}
		else
		{
			iRelationValue = z_overlaytype_val(pstEnv, pstStatusDef, pstStatusDefCurr);
			
			switch(iRelationValue)
			{
				case OVERLAY_FLAG_LEVEL_REJECT:
					if (pstStatusDefCurr->Level > pstStatusDef->Level)
					{
						return -1;
					}
					break;
					
				case OVERLAY_FLAG_SAME_REJECT:
					if (pstStatusDef == pstStatusDefCurr)
					{
						return -1;
					}
					break;

				case OVERLAY_FLAG_SAME_OVERLAY:
					if (pstStatusDef == pstStatusDefCurr)
					{
						if (iMerged)
						{
							continue;
						}

						iMergedIdxType = 2;
						iMergedIdx = i;
						iMergedStatusID = iStatus;
						if (pstZoneStatus->unOverlay < pstStatusDef->OverlayLimit)
						{
							iMergedOverlay = 1;
						}

						iMerged = 1;
					}
					break;

				case OVERLAY_FLAG_SAME_ADDTIME:
					if (pstStatusDef == pstStatusDefCurr)
					{
						if (iMerged)
						{
							continue;
						}

						iMergedIdxType = 2;
						iMergedIdx = i;
						iMergedStatusID = iStatus;

						if (pstZoneStatus->stEnd.tv_sec > pstEnv->pstAppCtx->stCurr.tv_sec)
						{
							iLast += (pstZoneStatus->stEnd.tv_sec - pstEnv->pstAppCtx->stCurr.tv_sec);
						}
						
						iMerged = 1;
					}
					break;	

				case OVERLAY_FLAG_SAME_REPLACE:
					if (pstStatusDef == pstStatusDefCurr)
					{
						if (iMerged)
						{
							continue;
						}

						iMergedIdxType = 2;
						iMergedIdx = i;
						iMergedStatusID = iStatus;

						iMerged = 1;
					}
					break;

				case OVERLAY_FLAG_REJECT:
					return -1;
					break;

				case OVERLAY_FLAG_REPLACE:
					if (pstStatusDef->StatusID == pstStatusDefCurr->StatusID)
					{
						if (iMerged)
						{
							continue;
						}

						iMergedIdxType = 2;
						iMergedIdx = i;
						iMergedStatusID = iStatus;

						iMerged = 1;
					}
					break;

				case OVERLAY_FLAG_CLEAR:
					astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
					astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
					iDelStatusNum++;
					break;

				case OVERLAY_FLAG_LEVEL_CLEAR:
					if (pstStatusDef->Level >= pstStatusDefCurr->Level)
					{
						astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
						astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
						iDelStatusNum++;
					}
					break;

				case OVERLAY_FLAG_LEVEL_REJECT_CLEAR:
					if (pstStatusDefCurr->Level > pstStatusDef->Level)
					{
						return -1;
					}
					else
					{
						astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
						astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
						iDelStatusNum++;
					}
					
					break;	
				
				default:
					break;
			}
		}
	}

	for (i=0; i<iDelStatusNum; i++)
	{
		z_player_del_status_level(pstAppCtx, pstEnv, pstPlayer, astDelStatus[i].unStatusID, astDelStatus[i].unLevel, 0, 0);
	}

	//状态回馈

	if (IS_STATUS_RESULTMAP_SET(STATUS_RESULT_FEEDBACK, pstResultMap->sMap))
	{
		STATUSRESULT* pstFeedBack = z_get_status_result(pstStatusDef, STATUS_RESULT_FEEDBACK);

		if(pstFeedBack)
		{
			Player* pstApply = z_id_player(pstAppCtx,pstEnv,iApplyID);
			if (pstApply)
			{
				ZoneStatus * pstAdd = NULL;

				z_player_apply_status2(pstAppCtx,pstEnv,pstApply,pstFeedBack->ResultVal1,pstFeedBack->ResultVal2,pstFeedBack->ResultVal3,1,iApplyID,&pstAdd);

				if (pstAdd)
				{
					pstAdd->iFeedbackMemID = pstPlayer->iMemID;
					pstAdd->iFeedbackStatusID = pstStatusDef->StatusID;
				}
				else
				{
					return -1;
				}
			}
		}
	}




	if (iMerged)
	{
		switch(iMergedIdxType)
		{
			case 0:
				pstZoneStatus = pstStatusList->astGoodStatus+iMergedIdx;
				break;
			case 1:
				pstZoneStatus = pstStatusList->astBadStatus+iMergedIdx;
				break;
			case 2:
				pstZoneStatus = pstStatusList->astNoLimitStatus+iMergedIdx;
				break;
			default:
				pstZoneStatus = NULL;
				break;
		}

		if (pstZoneStatus)
		{
			if (pstZoneStatus->unStatusID != iMergedStatusID || pstZoneStatus->bLevel != iStatusLevel)
			{
				pstStatusDef = z_find_status(pstEnv, iMergedStatusID, iStatusLevel);
				if (NULL == pstStatusDef)
				{
					return -1;
				}

				z_init_zonestatus(pstAppCtx, pstEnv, pstZoneStatus, pstStatusDef, iLast, pstZoneStatus->unOverlay+iMergedOverlay, iApplyID,&stAni);
			}
			else
			{
				STATUSOVERLAYDEF *pstStatusOverlay;
				
				pstZoneStatus->unOverlay += iMergedOverlay;
				if (pstZoneStatus->unOverlay > 0)
				{
					pstStatusOverlay = z_find_status_overlay(pstEnv, pstStatusDef->StatusID, pstStatusDef->Level, pstZoneStatus->unOverlay);
					if (pstStatusOverlay)
					{
						pstZoneStatus->nOverlayMul = pstStatusOverlay->Mul;
					}
					pstZoneStatus->iJumpedCount = 0;
				}
				pstZoneStatus->MiscType &= ~ZONESTATUS_MISCTYPE_CHUANRAN_FLAG;
				
				pstZoneStatus->stEnd.tv_sec = pstAppCtx->stCurr.tv_sec + iLast;
				pstZoneStatus->stEnd.tv_usec = pstAppCtx->stCurr.tv_usec;
				if (iApplyID > 0)
				{
					pstZoneStatus->iApplyID = iApplyID;
				}

				task_player_trace(pstEnv, pstPlayer, TASK_OBJ_STATUS_OVERLAY);
			}
		}
	}
	else
	{
		z_player_add_status(pstAppCtx, pstEnv, pstPlayer, pstStatusDef, iLast, iApplyID,ppstZoneStatus);	
	}

	

	// 如果状态有禁止回血回蓝的效果，在MiscFlag里设置一个标志
	pstStatusDef = z_find_status(pstEnv, iStatus, iStatusLevel);
	if(!pstStatusDef)
	{
		return -1;
	}
	if(z_get_status_result(pstStatusDef, STATUS_RESULT_CANNOT_ADDHP))
	{
		pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_CANNOT_ADDHP;
	}
	if(z_get_status_result(pstStatusDef, STATUS_RESULT_CANNOT_ADDMP))
	{
		pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_CANNOT_ADDMP;
	}

	z_player_chk_mspd(pstAppCtx, pstEnv, pstPlayer);

	if(z_get_status_result(pstStatusDef, STATUS_RESULT_MAXHP))
	{
		pstPlayer->stRoleData.RoleDetail.HPCurr = iOldHP;
	}
	if(z_get_status_result(pstStatusDef, STATUS_RESULT_MAXMP))
	{
	    pstPlayer->stRoleData.RoleDetail.MPCurr = iOldMP;
	}
	z_player_chk_maxmphp(pstAppCtx, pstEnv, pstPlayer);

	if (iNotify)
	{
		z_player_send_status(pstAppCtx, pstEnv, pstPlayer);
	}

	z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);

	if (pstZoneStatus && 
		pstZoneStatus->unOverlay > 0)
	{	
		STATUSRESULT *pstStatusResultOverlay;
		pstStatusResultOverlay = z_get_status_result(pstStatusDef, STATUS_RESULT_ADD_STATUS_BY_OVERLAY);
		if( pstStatusResultOverlay &&
			(pstZoneStatus->unOverlay+1) >= pstStatusResultOverlay->ResultVal1 &&
			pstStatusResultOverlay->ResultVal2 != iStatus )
		{
			STATUSDEF *pstStatusDefTmp = z_find_status(pstEnv, pstStatusResultOverlay->ResultVal2, pstStatusResultOverlay->ResultVal3);
			if (!pstStatusDef)
			{
				return -1;
			}
			if(z_get_status_result(pstStatusDefTmp, STATUS_RESULT_ADD_STATUS_BY_OVERLAY))
			{
				//防止策划填了嵌套的逻辑
				return -1;
			}
			z_player_apply_status(pstAppCtx, pstEnv, pstPlayer,pstStatusResultOverlay->ResultVal2, 
				pstStatusResultOverlay->ResultVal3, pstStatusResultOverlay->ResultVal4, iNotify, iApplyID);
		}
	}	
	
	//宏福活动
	{
		STATUSRESULT* pstResult;
		if (IS_STATUS_RESULTMAP_SET(STATUS_RESULT_LUCKY_ATTENDEE, pstResultMap->sMap))
		{
			pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_LUCK;
		}
		
		pstResult = z_get_status_result(pstStatusDef, STATUS_RESULT_DEL_TRANS);
		if(pstResult)
		{
			if(pstResult->ResultVal1 & STATUS_TRANS_MAP_NO_PROTECT_LUCKY)
			{
				pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_MAP_BROADCAST;
				player_broadcast_map_pos(pstEnv,pstPlayer,1);
				if (pstStatusDef)
				{
					
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_CHAT, ZONEERR_STATUS204,pstStatusDef->Name);
				}
				
				{
					//宏福buff携带者提示				
								
					pstEnv->iLuckyTipCount = 0;
				}

				//下机械
				player_ride_machine_brk(pstEnv,pstPlayer,1);
			}
		}
	}
	return 0;
}


int z_player_apply_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iStatus, int iStatusLevel, int iLast, int iNotify, int iApplyID)
{
	
	return z_player_apply_status2(pstAppCtx,pstEnv,pstPlayer,iStatus,iStatusLevel,iLast,iNotify,iApplyID, NULL);
}

int z_player_apply_status_test(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iStatus, int iStatusLevel)
{
	int i;
	STATUSDEF *pstStatusDef;
	STATUSDEF *pstStatusDefCurr;
	ZoneStatus *pstZoneStatus = NULL;
	int iRelationValue;
	DelStatus astDelStatus[MAX_GOOD_STATUS+MAX_BAD_STATUS+MAX_NOLIMIT_STATUS];
	int iDelStatusNum = 0;
	int iMerged = 0;
	int iMergedIdxType = -1;
	int iMergedIdx = -1;
	int iMergedStatusID = -1;
	int iMergedOverlay = 0;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	
	pstStatusDef = z_find_status(pstEnv, iStatus, iStatusLevel);
	if (NULL == pstStatusDef)
	{
		return -1;
	}

	if( z_get_player_zonestatus_by_resultid(pstEnv,pstPlayer,STATUS_RESULT_IMMUNITY_DEBUFF) &&
		STATUS_TYPE_BAD == pstStatusDef->Type )
	{
		return -1;	
	}
	
	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		pstStatusDefCurr = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDefCurr)
		{
			continue;
		}

		iRelationValue = z_overlaytype_val(pstEnv, pstStatusDef, pstStatusDefCurr);
		switch(iRelationValue)
		{
			case OVERLAY_FLAG_LEVEL_REJECT:
				if (pstStatusDefCurr->Level > pstStatusDef->Level)
				{
					return -1;
				}
				break;
				
			case OVERLAY_FLAG_SAME_REJECT:
				if (pstStatusDef == pstStatusDefCurr)
				{
					return -1;
				}
				break;

			case OVERLAY_FLAG_SAME_OVERLAY:
				if (pstStatusDef == pstStatusDefCurr)
				{
					if (iMerged)
					{
						continue;
					}

					iMergedIdxType = 0;
					iMergedIdx = i;
					iMergedStatusID = iStatus;
					if (pstZoneStatus->unOverlay < pstStatusDef->OverlayLimit)
					{
						iMergedOverlay = 1;
					}

					iMerged = 1;
				}
				break;

			case OVERLAY_FLAG_SAME_ADDTIME:
				if (pstStatusDef == pstStatusDefCurr)
				{
					if (iMerged)
					{
						continue;
					}

					iMergedIdxType = 0;
					iMergedIdx = i;
					iMergedStatusID = iStatus;

					iMerged = 1;
				}
				break;	

			case OVERLAY_FLAG_SAME_REPLACE:
				if (pstStatusDef == pstStatusDefCurr)
				{
					if (iMerged)
					{
						continue;
					}

					iMergedIdxType = 0;
					iMergedIdx = i;
					iMergedStatusID = iStatus;

					iMerged = 1;
				}
				break;

			case OVERLAY_FLAG_REJECT:
				return -1;
				break;

			case OVERLAY_FLAG_REPLACE:
				if (pstStatusDef->StatusID == pstStatusDefCurr->StatusID)
				{
					if (iMerged)
					{
						continue;
					}

					iMergedIdxType = 0;
					iMergedIdx = i;
					iMergedStatusID = iStatus;

					iMerged = 1;
				}
				break;

			case OVERLAY_FLAG_CLEAR:
				astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
				astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
				iDelStatusNum++;
				break;

			case OVERLAY_FLAG_LEVEL_CLEAR:
				if (pstStatusDef->Level >= pstStatusDefCurr->Level)
				{
					astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
					astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
					iDelStatusNum++;
				}
				break;

			case OVERLAY_FLAG_LEVEL_REJECT_CLEAR:
				if (pstStatusDefCurr->Level > pstStatusDef->Level)
				{
					return -1;
				}
				else
				{
					astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
					astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
					iDelStatusNum++;
				}
				
				break;	
			
			default:
				break;
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
		pstStatusDefCurr = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDefCurr)
		{
			continue;
		}

		iRelationValue = z_overlaytype_val(pstEnv, pstStatusDef, pstStatusDefCurr);
		
		switch(iRelationValue)
		{
			case OVERLAY_FLAG_LEVEL_REJECT:
				if (pstStatusDefCurr->Level > pstStatusDef->Level)
				{
					return -1;
				}
				break;
				
			case OVERLAY_FLAG_SAME_REJECT:
				if (pstStatusDef == pstStatusDefCurr)
				{
					return -1;
				}
				break;

			case OVERLAY_FLAG_SAME_OVERLAY:
				if (pstStatusDef == pstStatusDefCurr)
				{
					if (iMerged)
					{
						continue;
					}

					iMergedIdxType = 1;
					iMergedIdx = i;
					iMergedStatusID = iStatus;
					if (pstZoneStatus->unOverlay < pstStatusDef->OverlayLimit)
					{
						iMergedOverlay = 1;
					}

					iMerged = 1;
				}
				break;

			case OVERLAY_FLAG_SAME_ADDTIME:
				if (pstStatusDef == pstStatusDefCurr)
				{
					if (iMerged)
					{
						continue;
					}

					iMergedIdxType = 1;
					iMergedIdx = i;
					iMergedStatusID = iStatus;

					iMerged = 1;
				}
				break;
	

			case OVERLAY_FLAG_SAME_REPLACE:
				if (pstStatusDef == pstStatusDefCurr)
				{
					if (iMerged)
					{
						continue;
					}

					iMergedIdxType = 1;
					iMergedIdx = i;
					iMergedStatusID = iStatus;

					iMerged = 1;
				}
				break;

			case OVERLAY_FLAG_REJECT:
				return -1;
				break;

			case OVERLAY_FLAG_REPLACE:
				if (pstStatusDef->StatusID == pstStatusDefCurr->StatusID)
				{
					if (iMerged)
					{
						continue;
					}

					iMergedIdxType = 1;
					iMergedIdx = i;
					iMergedStatusID = iStatus;

					iMerged = 1;
				}
				break;

			case OVERLAY_FLAG_CLEAR:
				astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
				astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
				iDelStatusNum++;
				break;

			case OVERLAY_FLAG_LEVEL_CLEAR:
				if (pstStatusDef->Level >= pstStatusDefCurr->Level)
				{
					astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
					astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
					iDelStatusNum++;
				}
				break;

			case OVERLAY_FLAG_LEVEL_REJECT_CLEAR:
				if (pstStatusDefCurr->Level > pstStatusDef->Level)
				{
					return -1;
				}
				else
				{
					astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
					astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
					iDelStatusNum++;
				}
				
				break;
			
			default:
				break;
		}
	}

	for (i=0; i<pstStatusList->bNoLimitStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;

		pstStatusDefCurr = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDefCurr)
		{
			continue;
		}

		iRelationValue = z_overlaytype_val(pstEnv, pstStatusDef, pstStatusDefCurr);
		
		switch(iRelationValue)
		{
			case OVERLAY_FLAG_LEVEL_REJECT:
				if (pstStatusDefCurr->Level > pstStatusDef->Level)
				{
					return -1;
				}
				break;
				
			case OVERLAY_FLAG_SAME_REJECT:
				if (pstStatusDef == pstStatusDefCurr)
				{
					return -1;
				}
				break;

			case OVERLAY_FLAG_SAME_OVERLAY:
				if (pstStatusDef == pstStatusDefCurr)
				{
					if (iMerged)
					{
						continue;
					}

					iMergedIdxType = 2;
					iMergedIdx = i;
					iMergedStatusID = iStatus;
					if (pstZoneStatus->unOverlay < pstStatusDef->OverlayLimit)
					{
						iMergedOverlay = 1;
					}

					iMerged = 1;
				}
				break;

			case OVERLAY_FLAG_SAME_ADDTIME:
				if (pstStatusDef == pstStatusDefCurr)
				{
					if (iMerged)
					{
						continue;
					}

					iMergedIdxType = 2;
					iMergedIdx = i;
					iMergedStatusID = iStatus;

					iMerged = 1;
				}
				break;	

			case OVERLAY_FLAG_SAME_REPLACE:
				if (pstStatusDef == pstStatusDefCurr)
				{
					if (iMerged)
					{
						continue;
					}

					iMergedIdxType = 2;
					iMergedIdx = i;
					iMergedStatusID = iStatus;

					iMerged = 1;
				}
				break;

			case OVERLAY_FLAG_REJECT:
				return -1;
				break;

			case OVERLAY_FLAG_REPLACE:
				if (pstStatusDef->StatusID == pstStatusDefCurr->StatusID)
				{
					if (iMerged)
					{
						continue;
					}

					iMergedIdxType = 2;
					iMergedIdx = i;
					iMergedStatusID = iStatus;

					iMerged = 1;
				}
				break;

			case OVERLAY_FLAG_CLEAR:
				astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
				astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
				iDelStatusNum++;
				break;

			case OVERLAY_FLAG_LEVEL_CLEAR:
				if (pstStatusDef->Level >= pstStatusDefCurr->Level)
				{
					astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
					astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
					iDelStatusNum++;
				}
				break;

			case OVERLAY_FLAG_LEVEL_REJECT_CLEAR:
				if (pstStatusDefCurr->Level > pstStatusDef->Level)
				{
					return -1;
				}
				else
				{
					astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
					astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
					iDelStatusNum++;
				}
				
				break;	
			
			default:
				break;
		}
	}

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}


int z_pet_apply_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, int iStatus, int iStatusLevel, int iLast, int iNotify)
{
	int i;
	STATUSDEF *pstStatusDef;
	STATUSDEF *pstStatusDefCurr;
	int iRelationValue;
	DelStatus astDelStatus[MAX_GOOD_STATUS+MAX_BAD_STATUS+MAX_NOLIMIT_STATUS];
	int iDelStatusNum = 0;
	int iMerged = 0;
	int iMergedIdxType = -1;
	int iMergedIdx = -1;
	int iMergedStatusID = -1;
	int iMergedOverlay = 0;
	PetFightShiOnline *pstPetFightShiOnline;
	PetStatusInfo *pstStatusList;
	ZoneStatus *pstZoneStatus = NULL;

	pstPetFightShiOnline = pet_find_fight_online(pstPlayer, pstRolePet);
	if(NULL == pstPetFightShiOnline)
	{
		return -1;
	}
	pstStatusList = &pstPetFightShiOnline->stStatusInfo;

	pstStatusDef = z_find_status(pstEnv, iStatus, iStatusLevel);
	if (NULL == pstStatusDef)
	{
		return -1;
	}
	
	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
					
		pstStatusDefCurr = z_find_status_petstatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDefCurr)
		{
			continue;
		}

		iRelationValue = z_overlaytype_val(pstEnv, pstStatusDef, pstStatusDefCurr);
		
		switch(iRelationValue)
		{
			case OVERLAY_FLAG_LEVEL_REJECT:
				if (pstStatusDefCurr->Level > pstStatusDef->Level)
				{
					return -1;
				}
				break;
				
			case OVERLAY_FLAG_SAME_REJECT:
				if (pstStatusDef == pstStatusDefCurr)
				{
					return -1;
				}
				break;

			case OVERLAY_FLAG_SAME_OVERLAY:
				if (pstStatusDef == pstStatusDefCurr)
				{
					if (iMerged)
					{
						continue;
					}

					iMergedIdxType = 0;
					iMergedIdx = i;
					iMergedStatusID = iStatus;
					if (pstZoneStatus->unOverlay< pstStatusDef->OverlayLimit)
					{
						iMergedOverlay = 1;
					}

					iMerged = 1;
				}
				break;

			case OVERLAY_FLAG_SAME_REPLACE:
				if (pstStatusDef == pstStatusDefCurr)
				{
					if (iMerged)
					{
						continue;
					}

					iMergedIdxType = 0;
					iMergedIdx = i;
					iMergedStatusID = iStatus;

					iMerged = 1;
				}
				break;

			case OVERLAY_FLAG_REJECT:
				return -1;
				break;

			case OVERLAY_FLAG_REPLACE:
				if (pstStatusDef->StatusID == pstStatusDefCurr->StatusID)
				{
					if (iMerged)
					{
						continue;
					}

					iMergedIdxType = 0;
					iMergedIdx = i;
					iMergedStatusID = iStatus;

					iMerged = 1;
				}
				break;

			case OVERLAY_FLAG_CLEAR:
				astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
				astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
				iDelStatusNum++;
				break;

			case OVERLAY_FLAG_LEVEL_CLEAR:
				if (pstStatusDef->Level >= pstStatusDefCurr->Level)
				{
					astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
					astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
					iDelStatusNum++;
				}
				break;

			case OVERLAY_FLAG_LEVEL_REJECT_CLEAR:
				if (pstStatusDefCurr->Level > pstStatusDef->Level)
				{
					return -1;
				}
				else
				{
					astDelStatus[iDelStatusNum].unStatusID = pstZoneStatus->unStatusID;
					astDelStatus[iDelStatusNum].unLevel = pstZoneStatus->bLevel;
					iDelStatusNum++;
				}
				
				break;	
			
			default:
				break;
		}
	}
	
	for (i=0; i<iDelStatusNum; i++)
	{
		z_pet_del_status_level(pstAppCtx, pstEnv, pstPlayer, pstRolePet, astDelStatus[i].unStatusID, astDelStatus[i].unLevel, 0);
	}

	if (iMerged)
	{
		switch(iMergedIdxType)
		{
			case 0:
				pstZoneStatus = pstStatusList->astGoodStatus+iMergedIdx;
				break;
			case 1:
				pstZoneStatus = pstStatusList->astGoodStatus+iMergedIdx;
				break;
			case 2:
				pstZoneStatus = pstStatusList->astGoodStatus+iMergedIdx;
				break;
			default:
				pstZoneStatus = NULL;
				break;
		}

		if (pstZoneStatus)
		{
			if (pstZoneStatus->unStatusID!= iMergedStatusID || pstZoneStatus->bLevel!= iStatusLevel)
			{
				pstStatusDef = z_find_status(pstEnv, iMergedStatusID, iStatusLevel);
				if (NULL == pstStatusDef)
				{
					return -1;
				}
				
				z_init_pet_status(pstAppCtx, pstEnv, pstZoneStatus, pstStatusDef, iLast, pstZoneStatus->unOverlay+iMergedOverlay);
			}
			else
			{
				STATUSOVERLAYDEF *pstStatusOverlay;
				
				pstZoneStatus->unOverlay += iMergedOverlay;
				if (pstZoneStatus->unOverlay > 0)
				{
					pstStatusOverlay = z_find_status_overlay(pstEnv, pstStatusDef->StatusID, pstStatusDef->Level, pstZoneStatus->unOverlay);
					if (pstStatusOverlay)
					{
						pstZoneStatus->nOverlayMul = pstStatusOverlay->Mul;
					}
					pstZoneStatus->iJumpedCount = 0;
				}
				
				pstZoneStatus->stEnd.tv_sec = pstAppCtx->stCurr.tv_sec + iLast;
				pstZoneStatus->stEnd.tv_usec = pstAppCtx->stCurr.tv_usec;
			}
		}
	}
	else
	{
		z_pet_add_status(pstAppCtx, pstEnv, pstPlayer, pstRolePet, pstStatusDef, iLast);	
	}

	if (iNotify)
	{
		//z_player_send_status(pstAppCtx, pstEnv, pstPlayer);
	}

	pet_update_online(pstEnv, pstPlayer, pstRolePet);
	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);

	if (pstZoneStatus && 
		pstZoneStatus->unOverlay > 0)
	{	
		STATUSRESULT *pstStatusResultOverlay;
		pstStatusResultOverlay = z_get_status_result(pstStatusDef, STATUS_RESULT_ADD_STATUS_BY_OVERLAY);
		if( pstStatusResultOverlay &&
			(pstZoneStatus->unOverlay+1) >= pstStatusResultOverlay->ResultVal1 &&
			pstStatusResultOverlay->ResultVal2 != iStatus )
		{
			STATUSDEF *pstStatusDefTmp = z_find_status(pstEnv, pstStatusResultOverlay->ResultVal2, pstStatusResultOverlay->ResultVal3);
			if (!pstStatusDef)
			{
				return -1;
			}
			if(z_get_status_result(pstStatusDefTmp, STATUS_RESULT_ADD_STATUS_BY_OVERLAY))
			{
				//防止策划填了嵌套的逻辑
				return -1;
			}
			z_pet_apply_status(pstAppCtx, pstEnv, pstPlayer, pstRolePet, pstStatusResultOverlay->ResultVal2, 
				pstStatusResultOverlay->ResultVal3, pstStatusResultOverlay->ResultVal4, iNotify);
		}
	}
	
	return 0;
}

ZoneStatus * z_player_find_status(Player *pstPlayer, int iStatusID)
{
	int i;
	ZoneStatus *pstZoneStatus;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	
	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		
		if (pstZoneStatus->unStatusID == iStatusID)
		{
			return pstZoneStatus;
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;

		if (pstZoneStatus->unStatusID == iStatusID)
		{
			return pstZoneStatus;
		}		
	}

	for (i=0; i<pstStatusList->bNoLimitStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;

		if (pstZoneStatus->unStatusID == iStatusID)
		{
			return pstZoneStatus;
		}
	}

	return NULL;
}
ZoneStatus * z_player_find_status_by_level(Player *pstPlayer, int iStatusID,char cLevel)
{
	int i;
	ZoneStatus *pstZoneStatus;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	
	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		
		if (pstZoneStatus->unStatusID == iStatusID &&
			pstZoneStatus->bLevel ==cLevel )
		{
			return pstZoneStatus;
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;

		if (pstZoneStatus->unStatusID == iStatusID &&
			pstZoneStatus->bLevel ==cLevel )
		{
			return pstZoneStatus;
		}		
	}

	for (i=0; i<pstStatusList->bNoLimitStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;

		if (pstZoneStatus->unStatusID == iStatusID &&
			pstZoneStatus->bLevel ==cLevel )
		{
			return pstZoneStatus;
		}
	}

	return NULL;
}

ZoneStatus *z_mon_find_status_by_level(Monster *pstMon, int iStatusID,char cLevel)
{
	int i;
	ZoneStatus *pstZoneStatus;
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;
	
	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		
		if (pstZoneStatus->unStatusID == iStatusID &&
			pstZoneStatus->bLevel == cLevel)
		{
			return pstZoneStatus;
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;

		if (pstZoneStatus->unStatusID == iStatusID &&
			pstZoneStatus->bLevel ==cLevel)
		{
			return pstZoneStatus;
		}		
	}

	return NULL;
}
ZoneStatus *z_mon_find_status(Monster *pstMon, int iStatusID)
{
	int i;
	ZoneStatus *pstZoneStatus;
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;
	
	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		
		if (pstZoneStatus->unStatusID == iStatusID)
		{
			return pstZoneStatus;
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;

		if (pstZoneStatus->unStatusID == iStatusID)
		{
			return pstZoneStatus;
		}		
	}

	return NULL;
}

int player_cancel_status(TAPPCTX* pstAppCtx,
                         ZONESVRENV* pstEnv,
                         Player *pstPlayer, 
                         TFRAMEHEAD *pstFrameHead,
                         CSPKG *pstCsPkg)
{
	CSCANCELSTATUS *pstCancelInfo = &pstCsPkg->Body.CancelStatus;
	STATUSDEF *pstStatusDef = NULL;
	int iDeltaTime = 0;
	const ZoneStatus *pstZoneStatus = NULL;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	
	UNUSED(pstAppCtx);
	UNUSED(pstFrameHead);
	

	if(pstCancelInfo->Index < 0 || pstCancelInfo->Index >= pstStatusList->bGoodStatusNum)
	{
		return -1;
	}

	pstZoneStatus = pstStatusList->astGoodStatus + pstCancelInfo->Index;
	if(pstZoneStatus->unStatusID != pstCancelInfo->StatusBeCanceledID)
	{
		return -1;
	}

	pstStatusDef = z_find_status(pstEnv, pstCancelInfo->StatusBeCanceledID, pstZoneStatus->bLevel);
	if(!pstStatusDef || !pstStatusDef->CanCancel)
	{
		return -1; // Status不能被取消
	}

	pstZoneStatus = z_player_find_status(pstPlayer,
	                                     pstCancelInfo->StatusBeCanceledID);
	if(!pstZoneStatus)
	{
		return -1; // 没有这个Status
	}

	iDeltaTime = (pstZoneStatus->stEnd.tv_sec - pstEnv->pstAppCtx->stCurr.tv_sec)
	             - pstCancelInfo->RemainTime;
	iDeltaTime = iDeltaTime < 0 ? -iDeltaTime : iDeltaTime;

	if(iDeltaTime > 5)
	{
		return -1; // 误差过大
	}

	player_del_status_by_idx(pstEnv, pstPlayer,
	                         pstCancelInfo->StatusBeCanceledID,
	                         pstCancelInfo->Index);

	
	return 0;
}

ZoneStatus* z_pet_find_status(ZONESVRENV* pstEnv,  Player *pstPlayer, ROLEPET *pstRolePet,int iStatusID)
{
	int i = 0;
	STATUSDEF *pstStatusDef;
	PetFightShiOnline *pstPetFightShiOnline;
	PetStatusInfo *pstStatusList;
	ZoneStatus *pstZoneStatus;

	pstPetFightShiOnline = pet_find_fight_online(pstPlayer, pstRolePet);
	if(NULL == pstPetFightShiOnline)
	{
		return NULL;
	}
	pstStatusList = &pstPetFightShiOnline->stStatusInfo;

	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		pstStatusDef = z_find_status_petstatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;

		if( pstStatusDef->StatusID == iStatusID )
		{
			return pstZoneStatus;
		}
	}

	return NULL;
}
//暴击删除指定的状态
int z_del_result_by_heavyhit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,Player *pstPlayer,Monster *pstMon,int iHeavyHit)
{
	STATUSRESULT stResult;

	memset(&stResult,0,sizeof(stResult));

	if(!iHeavyHit)
	{
		return -1;
	}
	
	if(pstPlayer)
	{
		z_get_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_DEL_BY_HEAVYHIT, &stResult, 0);
		if(stResult.ResultID == STATUS_RESULT_DEL_BY_HEAVYHIT)
		{
			z_player_del_status(pstAppCtx, pstEnv, pstPlayer, stResult.ResultVal1, 1, 0);
		}
	}

	if(pstMon)
	{
		z_get_mon_status_result(pstEnv, pstMon, STATUS_RESULT_DEL_BY_HEAVYHIT, &stResult);
		if(stResult.ResultID == STATUS_RESULT_DEL_BY_HEAVYHIT)
		{
			z_mon_del_status(pstAppCtx, pstEnv, pstMon, stResult.ResultVal1, 0);
		}
	}

	return 0;
}
//吞噬debuf
int z_status_megre_debuff(ZONESVRENV* pstEnv,ZoneAni *pstZoneAni,SKILLRESULT *pstResult,CSONEHIT *pstOneHit)
{
	ZoneStatus *pstZoneStatus = NULL;
	STATUSDEF *pstStatusDef = NULL;
	struct timeval stDiff;
	long long  llMs;
	int iDecHp = 0;
	STATUSRESULT stResult;
	STATUSRESULT *pstStatusResult = NULL;
		
	if( pstZoneAni->iType == OBJ_MONSTER )
	{
		memset(&stResult,0,sizeof(stResult));
		/*if(pstResult->SkillResultVal2 > 0)
		{
			pstZoneStatus = z_mon_find_status_by_level(pstZoneAni->stObj.pstMon, 
				pstResult->SkillResultVal1,pstResult->SkillResultVal2);
		}
		else
		{*/
			pstZoneStatus = z_mon_find_status(pstZoneAni->stObj.pstMon, pstResult->SkillResultVal1);
		//}
		if(!pstZoneStatus)
		{
			return -1;
		}
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if(!pstStatusDef)
		{
			return -1;
		}
		TV_DIFF(stDiff, pstZoneStatus->stEnd, pstEnv->pstAppCtx->stCurr);
		TV_TO_MS(llMs, stDiff);

		//转换成伤害次数				
		if(0 == z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_HPDEC, &stResult))
		{
			if(pstZoneStatus->iVal)
			{
				iDecHp = pstZoneStatus->iVal * (1+pstZoneStatus->nOverlayMul/100.0);
			}
			else
			{
				iDecHp +=  pstZoneAni->stObj.pstMon->iMaxHP*(stResult.ResultVal1/100.0)+stResult.ResultVal2;
			}

			llMs = llMs/(1000.0*3)+1;
			iDecHp = iDecHp* llMs;
		}
		else 
		{
			pstStatusResult = z_get_status_result(pstStatusDef, STATUS_RESULT_DEC_HP_FUNC_MUL);
			if(pstStatusResult)
			{
				if( pstZoneStatus->iVal)
				{
					iDecHp = pstZoneStatus->iVal*(1+pstZoneStatus->nOverlayMul/100.0);
				}
				else if(pstZoneStatus->iJumpNum >0)
				{
					iDecHp =  (pstStatusResult->ResultVal2/pstZoneStatus->iJumpNum) * (1+pstZoneStatus->nOverlayMul/100.0);
				}
				if(pstZoneStatus->iJumpNum > pstZoneStatus->iJumpedCount)
				{
					iDecHp = iDecHp * (pstZoneStatus->iJumpNum - pstZoneStatus->iJumpedCount);
				}
			}
		}

		iDecHp = iDecHp * (1+pstResult->SkillResultVal2/100.0);
		
		//z_mon_status_chg_hp(pstEnv->pstAppCtx, pstEnv, pstZoneAni->stObj.pstMon, iDecHp,pstZoneStatus->iApplyID);
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
		if(iDecHp<0)
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = -iDecHp;
		}
		else
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iDecHp;
		}
		pstOneHit->TotalHurt++;
		if (0==z_mon_del_status(pstEnv->pstAppCtx,  pstEnv, pstZoneAni->stObj.pstMon, pstResult->SkillResultVal1, pstZoneStatus->iApplyID))
		{
			pstOneHit->StatusFlag = 1;
			//z_get_mon_csstatusinfo(pstEnv->pstAppCtx, &pstOneHit->Status, &pstZoneAni->stObj.pstMon->stStatusList);
		}
		
	}
	else if(pstZoneAni->iType == OBJ_PLAYER)
	{
		memset(&stResult,0,sizeof(stResult));
		pstZoneStatus = z_player_find_status(pstZoneAni->stObj.pstPlayer, pstResult->SkillResultVal1);
		if(!pstZoneStatus)
		{
			return -1;
		}
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if(!pstStatusDef)
		{
			return -1;
		}
		TV_DIFF(stDiff, pstZoneStatus->stEnd, pstEnv->pstAppCtx->stCurr);
		TV_TO_MS(llMs, stDiff);

		//转换成伤害次数				
		if(0 == z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_HPDEC, &stResult))
		{
			if(pstZoneStatus->iVal)
			{
				iDecHp = pstZoneStatus->iVal * (1+pstZoneStatus->nOverlayMul/100.0);
			}
			else
			{
				iDecHp +=  pstZoneAni->stObj.pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax*
								(stResult.ResultVal1/100.0)+stResult.ResultVal2;
			}
			llMs = llMs/(1000.0*3)+1;
			iDecHp = iDecHp* llMs;
		}
		else 
		{
			pstStatusResult = z_get_status_result(pstStatusDef, STATUS_RESULT_DEC_HP_FUNC_MUL);
			if(pstStatusResult)
			{
				if( pstZoneStatus->iVal)
				{
					iDecHp = pstZoneStatus->iVal*(1+pstZoneStatus->nOverlayMul/100.0);
				}
				else if(pstZoneStatus->iJumpNum >0)
				{
					iDecHp =  (pstStatusResult->ResultVal2/pstZoneStatus->iJumpNum) * (1+pstZoneStatus->nOverlayMul/100.0);
				}
				if(pstZoneStatus->iJumpNum > pstZoneStatus->iJumpedCount)
				{
					iDecHp = iDecHp * (pstZoneStatus->iJumpNum - pstZoneStatus->iJumpedCount);
				}
			}
		}
		
		iDecHp = iDecHp * (1+pstResult->SkillResultVal2/100.0);
		
		//z_player_status_chg_hp_mp(pstEnv->pstAppCtx, pstEnv, pstZoneAni->stObj.pstPlayer, iDecHp,0,pstZoneStatus->iApplyID);
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
		if(iDecHp<0)
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = -iDecHp;
		}
		else
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iDecHp;
		}
		pstOneHit->TotalHurt++;
		z_player_del_status(pstEnv->pstAppCtx,  pstEnv, pstZoneAni->stObj.pstPlayer, pstResult->SkillResultVal1, 0,pstZoneStatus->iApplyID);

		pstOneHit->StatusFlag = 1;
		//z_get_player_csstatusinfo(pstEnv->pstAppCtx, &pstOneHit->Status, &pstZoneAni->stObj.pstPlayer->stOnline.stStatusList);
	}

	return iDecHp;
}
//
int z_status_mul_skill_hurt( ZONESVRENV* pstEnv, ZoneStatus *pstZoneStatus,int *iHurt,int *iHurtMul)
{
	STATUSRESULT *pstStatusResult = NULL;
	STATUSDEF *pstStatusDef = NULL;
	
	pstStatusDef = z_find_status(pstEnv,pstZoneStatus->unStatusID, pstZoneStatus->bLevel);
	if(!pstStatusDef)
	{
		return -1;
	}
	
	pstStatusResult = z_get_status_result(pstStatusDef, STATUS_RESULT_MUL_SKILL_HURT);
	if(!pstStatusResult)
	{
		return -1;
	}
	
	*iHurt = (*iHurt) * (1+pstStatusResult->ResultVal1/100.0) + pstStatusResult->ResultVal2;

	*iHurtMul += pstStatusResult->ResultVal3;
	return 0;
}

//被队友的debuff传染了
int z_player_status_chuanran_by_team(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,ZoneStatus *pstZoneStatus)
{
	ZoneTeam *pstZoneTeam = NULL;
	//ZoneStatus *pstZoneStatus = NULL;
	STATUSRESULT stResult;
	int i = 0;
	Player *pstMember = NULL;
	int itv_sec = 0;
	ZoneStatus *pstZoneStatusTmp = NULL;
	
	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if(!pstZoneTeam)
	{
		return -1;
	}
	
	memset(&stResult,0,sizeof(stResult));
	if( (pstZoneStatus->MiscType&ZONESTATUS_MISCTYPE_CHUANRAN_FLAG) ||
		0 > z_get_status_info_result(pstEnv, pstZoneStatus, STATUS_RESULT_CHUANRAN_BY_TEAM, &stResult) ||
		!stResult.ResultID)
	{
		return -1;
	}

	pstZoneStatus->MiscType |= ZONESTATUS_MISCTYPE_CHUANRAN_FLAG;
	//判断下距离
	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstMember = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (!pstMember || pstMember == pstPlayer)
		{
			continue;
		}

		/*if(z_player_find_status_by_level(pstMember, pstZoneStatus->unStatusID, pstZoneStatus->bLevel))
		{
			continue;
		}*/
		
		if( z_distance(&pstPlayer->stRoleData.Pos, &pstMember->stRoleData.Pos) > stResult.ResultVal1 )
		{
			continue;
		}

		itv_sec = pstZoneStatus->stEnd.tv_sec - pstAppCtx->stCurr.tv_sec;
		if(itv_sec <= 1 )
		{
			continue;
		}
		
		//把状态传染给你
		z_player_apply_status(pstAppCtx, pstEnv, pstMember, 
								pstZoneStatus->unStatusID, pstZoneStatus->bLevel, itv_sec, 1, 0 );
		pstZoneStatusTmp = z_player_find_status_by_level(pstMember, pstZoneStatus->unStatusID, pstZoneStatus->bLevel);
		if (pstZoneStatusTmp)
		{
			pstZoneStatusTmp->iVal = pstZoneStatus->iVal;
			pstZoneStatusTmp->MiscType |= ZONESTATUS_MISCTYPE_CHUANRAN_FLAG;
		}
	}
	
	return 0;
}
//人的状态传染给其他附进的人
int z_player_status_chuanran_by_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,ZoneStatus *pstZoneStatus)
{
	STATUSRESULT *pstStatusResult = NULL;
	int i = 0;
	int j = 0;
	int itv_sec = 0;

	ZONEMAPINST* pstMapInst;
	MapIndex *pstMapIdx;
	ZONEIDX stIdx;
	SearchArea stSArea;
	AREADYN *pstArea;
	int iIndex  = 0;
	char cType  = 0;
	TMEMBLOCK *pstMemBlock = NULL;
	Player *pstVPlayer = NULL;
	SKILLDEF *pstVSkillDef = NULL;
	int iDistance = 0;
	int iOffset = 0;
	int iMemID = 0;
	ZoneStatus *pstZoneStatusTmp = NULL;
	Player *pstAppPlayer = NULL;
	Monster *pstAppMon = NULL;
	STATUSDEF *pstStatusDef;
	
	int iCount = 0;
	int iMax = 100;
	int iData = 100;
	char sDataSqort[iMax][iData];
	

	memset(sDataSqort[0], 0, iMax*iData);

	pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
	if(!pstStatusDef)
	{
		return -1;
	}
	pstStatusResult = z_get_status_result(pstStatusDef, STATUS_RESULT_CHUANRAN);
	if( (pstZoneStatus->MiscType&ZONESTATUS_MISCTYPE_CHUANRAN_FLAG) ||
		!pstStatusResult ||
		!pstStatusResult->ResultID)
	{
		return -1;
	}

	
	iIndex = z_id_indextype(pstAppCtx, pstEnv,  pstZoneStatus->iApplyID, &cType);
	if (iIndex < 0) 
	{
		return -1;
	}
	if (cType == OBJ_PLAYER)
	{
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
		if (!pstMemBlock->fValid)
		{
			return -1;
		}
		pstAppPlayer =  (Player *)pstMemBlock->szData;
		if (PLAYER_STATUS_ROLE_LOGIN != pstAppPlayer->eStatus)
		{
			return -1;
		}
	}
	else if (cType == OBJ_MONSTER)
	{	
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
        if (!pstMemBlock->fValid)
        {
            return -1;
        }
        pstAppMon = (Monster *)pstMemBlock->szData;
	}

				
	itv_sec = pstZoneStatus->stEnd.tv_sec - pstAppCtx->stCurr.tv_sec;
	if(itv_sec <= 0 )
	{
		return -1;
	}

	
	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst	=	z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if(!pstMapInst)
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx) 
	{
		return -1;
	}

	if (z_pos_search_area(pstAppCtx, pstEnv,  pstMapInst, &pstPlayer->stRoleData.Pos, &stSArea) < 0) 
		return -1;

	pstZoneStatus->MiscType |= ZONESTATUS_MISCTYPE_CHUANRAN_FLAG;
	
	for (i = 0; i < stSArea.iNum; i++) 
	{
		if(iCount >= iMax)
		{
			break;
		}
		
		pstArea = stSArea.pastArea[i];
		for (j = 0; j < pstArea->iAnimate; j++)
		{
			iIndex = z_id_indextype(pstAppCtx, pstEnv,  pstArea->aiAnimate[j], &cType);
			if (iIndex < 0) continue;

			if (cType == OBJ_PLAYER)
			{	
				ZoneAni stAni;
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					continue;
				}
				
                		pstVPlayer = (Player *)pstMemBlock->szData;
				iDistance = z_distance(&pstVPlayer->stRoleData.Pos, &pstPlayer->stRoleData.Pos);
				if( pstVPlayer == pstPlayer ||
					pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN ||
					iDistance > pstStatusResult->ResultVal2 )
				{
					continue;
				}
				
				if( pstAppPlayer )
				{
					pstVSkillDef = z_get_player_normalskilldef(pstEnv,pstVPlayer);
					if(!pstVSkillDef)
					{
						continue;
					}
					memset(&stAni,0,sizeof(stAni));
					stAni.iType = OBJ_PLAYER;
					stAni.stObj.pstPlayer = pstVPlayer;
					if(!z_player_filter_relation(pstAppCtx, pstEnv, pstAppPlayer, &stAni, pstVSkillDef))
					{
						continue;
					}
				}
				else if(pstAppMon)
				{
					pstVSkillDef = z_find_skill(pstEnv, pstAppMon->astNormalSkill[0].stSkill.SkillID, pstAppMon->astNormalSkill[0].stSkill.SkillLevel,
												&pstAppMon->astNormalSkill[0].iSkillIdx);
					if(!pstVSkillDef)
					{
						continue;
					}
					memset(&stAni,0,sizeof(stAni));
					stAni.iType = OBJ_PLAYER;
					stAni.stObj.pstPlayer = pstVPlayer;
					if(!z_mon_filter_relation(pstAppCtx, pstEnv, pstAppMon, &stAni, pstVSkillDef))
					{
						continue;
					}
				}
				else
				{
					continue;
				}
					
				iOffset = 0;
				memcpy(sDataSqort[iCount]+iOffset,&iDistance,sizeof(iDistance));
				iOffset += sizeof(iDistance);
				memcpy(sDataSqort[iCount]+iOffset,&pstVPlayer->iMemID,sizeof(pstVPlayer->iMemID));
				iOffset += sizeof(pstVPlayer->iMemID);
				iCount++;
				if(iCount >= iMax)
				{
					break;
				}
				
			}
		}
    }

	// 按距离做下排序处理
	qsort(sDataSqort[0], iCount, iData, trl_comp_func);	

	for(i=0;i<iCount && i<pstStatusResult->ResultVal1;i++)
	{
		memcpy(&iMemID,sDataSqort[i]+sizeof(int),sizeof(int));
		iIndex = z_id_indextype(pstAppCtx, pstEnv,  iMemID, &cType);

		if (iIndex < 0) continue;

		if (cType == OBJ_PLAYER)
		{	
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
			if(!pstMemBlock->fValid)
			{
				continue;
			}
			
			pstVPlayer = (Player *)pstMemBlock->szData;
			z_player_apply_status(pstAppCtx, pstEnv, pstVPlayer, 
							pstZoneStatus->unStatusID, pstZoneStatus->bLevel, itv_sec, 1, pstZoneStatus->iApplyID);
			pstZoneStatusTmp = z_player_find_status_by_level(pstVPlayer, pstZoneStatus->unStatusID, pstZoneStatus->bLevel);
			if (pstZoneStatusTmp)
			{
				pstZoneStatusTmp->iVal = pstZoneStatus->iVal;
				//只传一次
				pstZoneStatusTmp->MiscType |= ZONESTATUS_MISCTYPE_CHUANRAN_FLAG;
			}
		}
	}

	return 0;
}

//怪物把状态传染给其他附件的怪
int z_mon_status_chuanran_by_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon,ZoneStatus *pstZoneStatus)
{
	STATUSRESULT *pstStatusResult = NULL;
	int i = 0;
	int j = 0;
	int itv_sec = 0;

	ZONEMAPINST* pstMapInst;
	MapIndex *pstMapIdx;
	SearchArea stSArea;
	AREADYN *pstArea;
	int iIndex  = 0;
	char cType  = 0;
	TMEMBLOCK *pstMemBlock = NULL;
	Monster *pstVMon = NULL;
	int iDistance = 0;
	int iOffset = 0;
	int iMemID = 0;
	ZoneStatus *pstZoneStatusTmp = NULL;
	Player *pstAppPlayer = NULL;
	Monster *pstAppMon = NULL;
	MONSTERDEF *pstVMonDef = NULL;
	STATUSDEF *pstStatusDef;
	
	int iCount = 0;
	int iMax = 100;
	int iData = 100;
	char sDataSqort[iMax][iData];
	

	memset(sDataSqort[0], 0, iMax*iData);

	pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
	if(!pstStatusDef)
	{
		return -1;
	}
	pstStatusResult = z_get_status_result(pstStatusDef, STATUS_RESULT_CHUANRAN);
	if( (pstZoneStatus->MiscType&ZONESTATUS_MISCTYPE_CHUANRAN_FLAG) ||
		!pstStatusResult ||
		!pstStatusResult->ResultID)
	{
		return -1;
	}

	
	iIndex = z_id_indextype(pstAppCtx, pstEnv,  pstZoneStatus->iApplyID, &cType);
	if (iIndex < 0) 
	{
		return -1;
	}
	if (cType == OBJ_PLAYER)
	{
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
		if (!pstMemBlock->fValid)
		{
			return -1;
		}
		pstAppPlayer =  (Player *)pstMemBlock->szData;
		if (PLAYER_STATUS_ROLE_LOGIN != pstAppPlayer->eStatus)
		{
			return -1;
		}
	}
	else if (cType == OBJ_MONSTER)
	{	
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
		if (!pstMemBlock->fValid)
		{
			return -1;
		}
		pstAppMon = (Monster *)pstMemBlock->szData;
	}

				
	itv_sec = pstZoneStatus->stEnd.tv_sec - pstAppCtx->stCurr.tv_sec;
	if(itv_sec <= 0 )
	{
		return -1;
	}

	pstMapInst	=	z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
	if(!pstMapInst)
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx) 
	{
		return -1;
	}

	if (z_pos_search_area(pstAppCtx, pstEnv,  pstMapInst, &pstMon->stCurrPos, &stSArea) < 0) 
		return -1;

	pstZoneStatus->MiscType |= ZONESTATUS_MISCTYPE_CHUANRAN_FLAG;
	
	for (i = 0; i < stSArea.iNum; i++) 
	{
		if(iCount >= iMax)
		{
			break;
		}
		
		pstArea = stSArea.pastArea[i];
		for (j = 0; j < pstArea->iAnimate; j++)
		{
			iIndex = z_id_indextype(pstAppCtx, pstEnv,  pstArea->aiAnimate[j], &cType);

			if (iIndex < 0) continue;

			if (cType == OBJ_MONSTER)
			{	
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					continue;
				}
				
				pstVMon = (Monster *)pstMemBlock->szData;
				iDistance = z_distance(&pstVMon->stCurrPos, &pstMon->stCurrPos);
				if( pstMon == pstVMon ||
					pstVMon->bSubType > MON_SUB_TYPE_JI ||
					iDistance > pstStatusResult->ResultVal2 )
				{
					continue;
				}
				
				iOffset = 0;
				memcpy(sDataSqort[iCount]+iOffset,&iDistance,sizeof(iDistance));
				iOffset += sizeof(iDistance);
				memcpy(sDataSqort[iCount]+iOffset,&pstVMon->iID,sizeof(pstVMon->iID));
				iOffset += sizeof(pstVMon->iID);
				iCount++;
				if(iCount >= iMax)
				{
					break;
				}
				
			}
		}
    }

	// 按距离做下排序处理
	qsort(sDataSqort[0], iCount, iData, trl_comp_func);	

	for(i=0;i<iCount && i<pstStatusResult->ResultVal1;i++)
	{
		memcpy(&iMemID,sDataSqort[i]+sizeof(int),sizeof(int));
		iIndex = z_id_indextype(pstAppCtx, pstEnv,  iMemID, &cType);

		if (iIndex < 0) continue;

		if (cType == OBJ_MONSTER)
		{	
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
            if (!pstMemBlock->fValid)
            {
                continue;
            }
            pstVMon = (Monster *)pstMemBlock->szData;
			pstVMonDef = z_get_mon_def(pstEnv, pstVMon);
			if (NULL == pstVMonDef)
			{
				continue;
			}
			z_mon_apply_status(pstAppCtx, pstEnv, pstVMon, 
							pstZoneStatus->unStatusID, pstZoneStatus->bLevel, itv_sec, 1, pstZoneStatus->iApplyID);
			pstZoneStatusTmp = z_mon_find_status_by_level(pstVMon, pstZoneStatus->unStatusID, pstZoneStatus->bLevel);
			if (pstZoneStatusTmp)
			{
				pstZoneStatusTmp->iVal = pstZoneStatus->iVal;
				//只传一次
				pstZoneStatusTmp->MiscType |= ZONESTATUS_MISCTYPE_CHUANRAN_FLAG;

				if ( pstAppPlayer &&
					0 > mon_press_find(pstVMon, pstAppPlayer->iMemID) &&
					z_distance(&pstAppPlayer->stRoleData.Pos, &pstVMon->stCurrPos) <= (int)pstVMonDef->TraceDist)
				{
					mon_press_insert_player(pstEnv, pstVMon, pstVMonDef, pstAppPlayer);
				}
				else if( pstAppMon &&
					  	 0 > mon_press_find(pstVMon, pstAppMon->iID) )
				{
					mon_press_insert_mon( pstEnv, pstVMon, pstAppMon);
				}
			}
		}
	}

	return 0;
}

int is_ctrl_status(STATUSDEF *pstStatusDef)
{
	int i;

	for(i=0; i<MAX_STATUS_RESULT; i++)
	{
		if(pstStatusDef->Result[i].ResultID == STATUS_RESULT_NOITEM)
		{
			return 1;
		}

		if(pstStatusDef->Result[i].ResultID == STATUS_RESULT_NOSKILL)
		{
			return 1;
		}

		if(pstStatusDef->Result[i].ResultID == STATUS_RESULT_RANDOM_RUN)
		{
			return 1;
		}
	}
	
	return 0;
}

int z_status_player_refresh_time_by_hit(ZONESVRENV* pstEnv, SKILLDEF *pstSkillDef,Player *pstPlayer)
{
	SKILLRESULT *pstSkillResult;
	ZoneStatus *pstZoneStatus;
	pstSkillResult = z_get_skill_result(pstSkillDef, RESULT_REFRESH_STATUS_TIME);
	if(!pstSkillResult)
	{
		return -1;
	}

	pstZoneStatus = z_player_find_status(pstPlayer,pstSkillResult->SkillResultVal1);
	if(!pstZoneStatus)
	{
		return -1;
	}

	pstZoneStatus->stEnd.tv_sec = pstEnv->pstAppCtx->stCurr.tv_sec + pstZoneStatus->iLast;
	pstZoneStatus->stEnd.tv_usec = pstEnv->pstAppCtx->stCurr.tv_usec;


	z_player_send_status(pstEnv->pstAppCtx,  pstEnv, pstPlayer);
	return 0;
}

int z_status_monster_refresh_time_by_hit(ZONESVRENV* pstEnv, SKILLDEF *pstSkillDef,Monster *pstMon)
{
	SKILLRESULT *pstSkillResult;
	ZoneStatus *pstZoneStatus;
	pstSkillResult = z_get_skill_result(pstSkillDef, RESULT_REFRESH_STATUS_TIME);
	if(!pstSkillResult)
	{
		return -1;
	}

	pstZoneStatus = z_mon_find_status(pstMon, pstSkillResult->SkillResultVal1);
	if(!pstZoneStatus)
	{
		return -1;
	}

	pstZoneStatus->stEnd.tv_sec = pstEnv->pstAppCtx->stCurr.tv_sec + pstZoneStatus->iLast;
	pstZoneStatus->stEnd.tv_usec = pstEnv->pstAppCtx->stCurr.tv_usec;

	z_mon_send_status(pstEnv->pstAppCtx, pstEnv, pstMon);

	return 0;
}

int z_status_pet_refresh_time_by_hit(ZONESVRENV* pstEnv, SKILLDEF *pstSkillDef,Player *pstPlayer, ROLEPET *pstRolePet)
{
	SKILLRESULT *pstSkillResult;
	ZoneStatus *pstZoneStatus;
	pstSkillResult = z_get_skill_result(pstSkillDef, RESULT_REFRESH_STATUS_TIME);
	if(!pstSkillResult)
	{
		return -1;
	}

	pstZoneStatus = z_pet_find_status(pstEnv, pstPlayer,pstRolePet, pstSkillResult->SkillResultVal1);
	if(!pstZoneStatus)
	{
		return -1;
	}

	pstZoneStatus->stEnd.tv_sec = pstEnv->pstAppCtx->stCurr.tv_sec + pstZoneStatus->iLast;
	pstZoneStatus->stEnd.tv_usec = pstEnv->pstAppCtx->stCurr.tv_usec;

	pet_update_online(pstEnv, pstPlayer, pstRolePet);
	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);
	return 0;
}

//把减血的生命通道状态放到坏状态的内存去
int z_player_check_hp_chunnel_status( ZONESVRENV* pstEnv,Player *pstPlayer,ZoneStatus stZoneStatus)
{
	STATUSDEF *pstStatusDef;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;

	pstStatusDef = z_find_status( pstEnv, stZoneStatus.unStatusID, stZoneStatus.bLevel);
	if(!pstStatusDef)
	{
		return -1;
	}
	if( stZoneStatus.cApplyRelation==0 ||
		0 >= stZoneStatus.iApplyID ||
		!z_get_status_result(pstStatusDef, STATUS_RESULT_HP_CHUNNEL_FUNC) ||
		stZoneStatus.iApplyID >= PET_ID_BASE )
	{
		return -1;
	}

	if(pstStatusList->bBadStatusNum >= MAX_BAD_STATUS)
	{
		return -1;
	}
	//把好状态放到坏状态里来
	if(stZoneStatus.iVal < 0)
	{
		z_player_del_status_level(pstEnv->pstAppCtx, pstEnv, pstPlayer, stZoneStatus.unStatusID, stZoneStatus.bLevel, 0, 0);
	}
	if(!z_get_player_zonestatus_by_resultid(pstEnv,pstPlayer,STATUS_RESULT_IMMUNITY_DEBUFF) )
	{
		pstStatusList->astBadStatus[pstStatusList->bBadStatusNum] = stZoneStatus;
		pstStatusList->bBadStatusNum++;
	}
	
	return 0;
}

int z_mon_check_hp_chunnel_status( ZONESVRENV* pstEnv,Monster *pstMon,ZoneStatus stZoneStatus)
{
	STATUSDEF *pstStatusDef;
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;

	pstStatusDef = z_find_status( pstEnv, stZoneStatus.unStatusID, stZoneStatus.bLevel);
	if(!pstStatusDef)
	{
		return -1;
	}
	if( stZoneStatus.cApplyRelation==0 ||
		0 >= stZoneStatus.iApplyID ||
		!z_get_status_result(pstStatusDef, STATUS_RESULT_HP_CHUNNEL_FUNC) ||
		stZoneStatus.iApplyID >= PET_ID_BASE )
	{
		return -1;
	}
	
	if(pstStatusList->bBadStatusNum >= MAX_BAD_STATUS)
	{
		return -1;
	}
	//把好状态放到坏状态里来
	if(stZoneStatus.iVal < 0)
	{
		z_mon_del_status_level(pstEnv->pstAppCtx, pstEnv, pstMon, stZoneStatus.unStatusID, stZoneStatus.bLevel, 0);
	}
	
	if(!z_get_mon_zonestatus_by_resultid(pstEnv,pstMon,STATUS_RESULT_IMMUNITY_DEBUFF))
	{
		pstStatusList->astBadStatus[pstStatusList->bBadStatusNum] = stZoneStatus;
		pstStatusList->bBadStatusNum++;
	}
	return 0;
}

int player_has_bianshen_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	ZoneStatus *pstZoneStatus;
	STATUSDEF *pstStatusDef;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;

	for (i=(int)pstStatusList->bGoodStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (pstStatusDef && pstStatusDef->ModeID > 0)
		{
			return 1;
		}
	}

	for (i=(int)pstStatusList->bBadStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
				
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (pstStatusDef && pstStatusDef->ModeID > 0)
		{
			return 1;
		}
	}

	for (i=(int)pstStatusList->bNoLimitStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (pstStatusDef && pstStatusDef->ModeID > 0)
		{
			return 1;
		}
	}

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}
