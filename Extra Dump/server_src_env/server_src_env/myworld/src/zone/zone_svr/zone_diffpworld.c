/*
**  @file $RCSfile: zone_diffpworld.c,v $
**  general description of this module
**  $Id: zone_diffpworld.c,v 1.98 2014/02/21 07:54:09 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/02/21 07:54:09 $
**  @version $Revision: 1.98 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_diffpworld.h"
#include "zone_svr.h"
#include "zone_team.h"
#include "zone_map.h"
#include "zone_attr.h"
#include "zone_player.h"
#include "zone_package.h"
#include "zone_itemlimit.h"
#include "zone_relation.h"
#include "zone_clt.h"
#include "zone_autoteam.h"
#include "zone_mail.h"
#include "zone_clan.h"
#include "zone_move.h"
#include "zone_misc.h"
#include "zone_oplog.h"
#include "zone_spansvr.h"
#include "zone_autopworld.h"
#include "zone_span.h"
#include "zone_vip.h"
#include "zone_err.h"
#include "zone_syncrelation.h"
#include "zone_booty.h"

int g_aiCardIdxMap[MAX_PWORLD_VIP_AWARD_CARD];
int g_iCardRangeCurr;

int g_aiVipCardIdxMap[MAX_PWORLD_VIP_AWARD_CARD];
int g_iVipCardRangeCurr;

#define PWORLD_LONG_DAN_GU 211

int player_select_card(ZONESVRENV* pstEnv, Player *pstPlayer, int iCardIdx);

int player_select_vip_card(ZONESVRENV* pstEnv, Player *pstPlayer, int iCardIdx);

DIFFPWORLDDEF *z_find_diff_pworld_def(ZONESVRENV *pstEnv, int iDiffPworldID)
{
	return (DIFFPWORLDDEF *)z_find_def(pstEnv->pstZoneObj->sDiffPworldDef, iDiffPworldID, DIFF_PWORLD_DEF_DATA, 
									pstEnv->pstZoneObj->iDiffPworldDef , NULL);
}

PWORLDBOXDEF *z_find_pworld_box_def(ZONESVRENV *pstEnv, int iPworldID)
{
	return (PWORLDBOXDEF *)z_find_def(pstEnv->pstZoneObj->sPworldBoxDef, iPworldID, PWORLD_BOX_DEF_DATA, 
									pstEnv->pstZoneObj->iPworldBoxDef, NULL);
}

PWORLDBOOTYGROUPDEF* z_find_pworld_bootygroup_def(ZONESVRENV *pstEnv, int iPworldID, int iGroup)
{			
	PWORLDBOOTYGROUPDEF stDef;
	int iIndex = 0;
	int iEqu = 0;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.PworldID = iPworldID;
	stDef.GroupID = iGroup;

	iIndex = bsearch_int(&stDef, pstObj->sPworldBootyGroupDef, pstObj->iPworldBootyGroupDef, PWORLD_BOOTY_GROUP_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (PWORLDBOOTYGROUPDEF *)pstObj->sPworldBootyGroupDef[iIndex];
	}

	return NULL;
}

PWORLDAWARDMULDEF *z_find_pworld_award_mul_def(ZONESVRENV *pstEnv, int iPworldID, int iGrade)
{		
	PWORLDAWARDMULDEF stDef;
	int iIndex = 0;
	int iEqu = 0;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.PworldID = iPworldID;
	stDef.Grade = iGrade;

	iIndex = bsearch_int(&stDef, pstObj->sPworldAwardMulDef, pstObj->iPworldAwardMulDef, PWORLD_AWARD_MUL_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (PWORLDAWARDMULDEF *)pstObj->sPworldAwardMulDef[iIndex];
	}

	return NULL;
}

int is_diff_pworld_grade(ZONESVRENV *pstEnv, int iDiffPworldID, int iPworldID)
{
	int i;
	DIFFPWORLDDEF *pstDiffPworldDef = z_find_diff_pworld_def(pstEnv, iDiffPworldID);

	if(NULL == pstDiffPworldDef)
	{
		return 0;
	}

	for(i=0; i<MAX_DIFF_PWORLD_LIST; i++)
	{
		if(iPworldID == pstDiffPworldDef->PworldInfo[i].PworldID)
		{
			return 1;
		}
	}

	return 0;
}

int is_dyn_diff_pworld(ZONESVRENV *pstEnv, int iDiffPworldID)
{
	ZONEIDX stIdx;
	ZONEPWORLD *pstPworld;
	DIFFPWORLDDEF *pstDiffPworldDef = z_find_diff_pworld_def(pstEnv, iDiffPworldID);

	if(NULL == pstDiffPworldDef)
	{
		return 0;
	}
	stIdx.iID = pstDiffPworldDef->PworldInfo[0].PworldID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if(pstPworld && pstPworld->stPworldDef.DynType)
	{
		return 1;
	}

	return 0;
}

int team_diff_pworld_get(ZONESVRENV* pstEnv, ZoneTeam *pstTeam, int iDiffPworldID)
{
	int i;
	ZONEPWORLDINST *pstPworldInst;
	ZONEPWORLD *pstZonePworld;

	for(i=0; i<pstTeam->iPworldNum; i++)
	{
		pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstTeam->stPworldInfo[i].iPworldInstID);
		if(NULL == pstPworldInst)
			continue;

		pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
		if(NULL == pstZonePworld)
		{
			continue;
		}

		if(pstZonePworld->stPworldDef.DiffPworldID == iDiffPworldID)
		{
			return pstZonePworld->stPworldDef.PworldID;
		}
	}

	return 0;
}

int single_diff_pworld_get(ZONESVRENV* pstEnv, Player *pstPlayer, int iDiffPworldID)
{
	ZONEPWORLD *pstSinglePworld;
	ZONEPWORLDINST *pstZonePworldInst;
		
	pstZonePworldInst = z_find_pworld_inst_bysingle(pstEnv, pstPlayer);
	if(NULL == pstZonePworldInst )
	{
		return 0;
	}

	pstSinglePworld = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
	if(NULL == pstSinglePworld)
	{
		return 0;
	}

	if(iDiffPworldID == pstSinglePworld->stPworldDef.DiffPworldID)
	{
		return pstSinglePworld->stPworldDef.PworldID;
	}

	return 0;
}

int open_diff_pworld_ui(ZONESVRENV* pstEnv, Player *pstPlayer, int iDiffPworldID, int iNpcID)
{
	CSPKG stPkg;
	CSDIFFPWORLDSVR *pstSvr = &stPkg.Body.DiffPworldSvr;
	ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);
	
	pstSvr->Type= DIFF_PWORLD_SVR_UI;
	pstSvr->Data.UIInfo.DiffPworldID = iDiffPworldID;
	pstSvr->Data.UIInfo.DynLevel = 0;
	pstSvr->Data.UIInfo.TeamCreateID = 0;
	pstSvr->Data.UIInfo.NpcID = iNpcID;

	//队伍创建了之后只能进已经创建的
	if(pstTeam)
	{
		pstSvr->Data.UIInfo.TeamCreateID = team_diff_pworld_get(pstEnv, pstTeam, iDiffPworldID);
	}

	pstSvr->Data.UIInfo.DynLevel = diff_pworld_dyn_level_get(pstEnv, pstPlayer, iDiffPworldID);
	
	Z_CSHEAD_INIT(&stPkg.Head, DIFF_PWORLD_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int diff_pworld_team_tips_add(CSDIFFPWORLDTEAMTIPS *pstTeamTips, char *pszRoleName, int iErrno, ... )
{
	DIFFPWORLDTIPINFO *pstTipInfo;
	const char* pszErr;
	va_list marker;

	va_start( marker, iErrno );
	pszErr	=	ZONE_GET_ERR(iErrno, 0);

	if(pstTeamTips->Num >= MAX_TEAM_MEMBER)
	{
		return -1;
	}

	pstTipInfo = &pstTeamTips->Tips[pstTeamTips->Num];

	//角色名
	STRNCPY(pstTipInfo->RoleName, pszRoleName, CCH(pstTipInfo->RoleName));

	if( !pszErr )
	{
		snprintf(pstTipInfo->Msg, sizeof(pstTipInfo->Msg), "SysNo : %d", iErrno );
	}
	else
	{
		vsnprintf(pstTipInfo->Msg, sizeof(pstTipInfo->Msg), pszErr, marker);
	}

	pstTipInfo->Msg[sizeof(pstTipInfo->Msg) - 1] = 0;
	va_end(marker);

	pstTeamTips->Num++;
		
	return 0;
}

int diff_pworld_active_check(ZONESVRENV* pstEnv, Player *pstPlayer, int iDiffPworldID)
{
	ACTIVEHELPER *pstActiveHelper;
	DIFFPWORLDDEF *pstDiffPworldDef;
	
	pstDiffPworldDef = z_find_diff_pworld_def(pstEnv, iDiffPworldID);
	if(NULL == pstDiffPworldDef)
	{
		return -1;
	}

	pstActiveHelper = z_find_active_help_def(pstEnv, pstDiffPworldDef->ActiveID, NULL);
	if (NULL == pstActiveHelper)
	{
		return -1;
	}

	//检查时间过期，提示一下
	if (0 > autoteam_check_active_running(pstEnv, pstActiveHelper))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_PWORLD);
		return -1;
	}

	return 0;
}


int diff_pworld_team_check_tips(ZONESVRENV* pstEnv,ZoneTeam *pstTeam, 
									Player *pstPlayer,ZONEPWORLD *pstPworld)
{
	CSPKG stPkg;
	CSDIFFPWORLDSVR *pstSvr = &stPkg.Body.DiffPworldSvr;
	CSDIFFPWORLDTEAMTIPS *pstTeamTips = &pstSvr->Data.Tips;
	int i;
	Player *pstTmpPlayer;

	pstSvr->Type = DIFF_PWORLD_SVR_TIPS;
	pstTeamTips->DiffPworldID = pstPworld->stPworldDef.DiffPworldID;
	pstTeamTips->PworldID = pstPworld->stPworldDef.PworldID;
	pstTeamTips->Num = 0;
	
	for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
		if (NULL == pstTmpPlayer)
		{
			diff_pworld_team_tips_add(pstTeamTips, pstTeam->stTeamInfo.TeamMember[i].MemberName,
									ZONEERR_DIFF301, pstTeam->stTeamInfo.TeamMember[i].MemberName);
			continue;
		}

		if (0 > z_check_single_pworld(pstEnv, pstTmpPlayer, &pstPworld->stPworldDef, 0, pstTeamTips))
		{
			continue;
		}
	}

	if(pstTeamTips->Num > 0)
	{
		Z_CSHEAD_INIT(&stPkg.Head, DIFF_PWORLD_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

		return -1;
	}
	return 0;
}

int diff_pworld_team_check(ZONESVRENV* pstEnv, ZoneTeam *pstTeam, Player *pstPlayer, 
	ZONEPWORLD *pstPworld, CSDIFFPWORLDENTER *pstEnter)
{
	int iTeamPworldID = team_diff_pworld_get(pstEnv, pstTeam, pstEnter->DiffPworldID);
	ZONEPWORLDINST *pstPInst;
	
	//已经创建了就只能进创建的副本
	if(iTeamPworldID > 0 && pstEnter->PworldID != iTeamPworldID)
	{
		return -1;
	}

	//进已经开启的副本
	if(iTeamPworldID > 0)
	{
		return 0;
	}

	if (pstTeam->iAutoPworldID>0)
	{
		pstPInst = 	z_find_pworld_inst(pstEnv->pstMapObj, pstTeam->iAutoPworldID);
		if (pstPInst)
		{
			if (pstPInst->stPworldIdx.iID == (int)pstPworld->stPworldDef.PworldID)
			{
				return 0;
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PWORLD1);
				return -1;
			}
		}
	}

	//新建副本
	if (pstTeam->stTeamInfo.MemberNum < pstPworld->stPworldDef.AutoPworldNum)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PWORLD2, 
					pstPworld->stPworldDef.Name, pstPworld->stPworldDef.AutoPworldNum);
		return -1;
	}

	if(0 > diff_pworld_active_check(pstEnv, pstPlayer, pstEnter->DiffPworldID))
	{
		return -1;
	}

	//只有队长可以创建
	if(pstPlayer != player_team_get_captain_player(pstEnv, pstTeam))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PWORLD3);
		return -1;
	}

	/*
	if (pstTeam->stTeamInfo.MemberNum < pstPworld->stPworldDef.AutoPworldNum)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "进入%s 队伍人数不能少于%d人!", 
					pstPworld->stPworldDef.Name, pstPworld->stPworldDef.AutoPworldNum);
		return -1;
	}*/

	if(0 > z_check_single_pworld(pstEnv, pstPlayer, &pstPworld->stPworldDef, 0, NULL))
	{
		return -1;
	}

	if(pstEnter->EnterFlag)
	{
		return 0;
	}
	
	if (diff_pworld_team_check_tips(pstEnv,pstTeam, pstPlayer,pstPworld) < 0)
	{
		return -1;
	}

	return 0;
}

int diff_pworld_enter(ZONESVRENV* pstEnv, Player *pstPlayer, CSDIFFPWORLDENTER *pstEnter)
{
	ZONEIDX stIdx;
	//Npc *pstNpc;
	ZONEPWORLD *pstPworld;
	PWORLDSELECT stPwolrdSelect;
	ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);
	int iPInstID;

	if(!is_diff_pworld_grade(pstEnv, pstEnter->DiffPworldID, pstEnter->PworldID))
	{
		return -1;
	}

	/*pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstEnter->NpcID);
	if (NULL == pstNpc)
	{
		return -1;
	}

	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	}

	if (0 > z_check_op_dist(pstEnv, pstPlayer, pstNpc->stMon.stMap.iID, &pstNpc->stMon.stCurrPos, OP_DIST+pstNpc->stMon.unRadius))
	{
		return -1;
	}*/

	stIdx.iID = pstEnter->PworldID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if(NULL == pstPworld)
	{
		return -1;
	}

	if(pstTeam)
	{
		if(0 > diff_pworld_team_check(pstEnv, pstTeam, pstPlayer, pstPworld, pstEnter))
		{
			return -1;
		}
	}
	else
	{
		int iSinglePworldID = single_diff_pworld_get(pstEnv, pstPlayer, pstEnter->DiffPworldID);

		if(iSinglePworldID > 0)
		{
			if(iSinglePworldID != pstEnter->PworldID)
				return -1;
		}
		else
		{
			//创建副本
			if (pstPworld->stPworldDef.AutoPworldNum > 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PWORLD2, 
					pstPworld->stPworldDef.Name, pstPworld->stPworldDef.AutoPworldNum);
				return -1;
			}
			
			if(0 > diff_pworld_active_check(pstEnv, pstPlayer, pstEnter->DiffPworldID))
			{
				return -1;
			}
		}
	}

	memset(&stPwolrdSelect, 0, sizeof(stPwolrdSelect));

	iPInstID = z_player_enter_pworld(pstEnv, pstPlayer, pstEnter->PworldID, &stPwolrdSelect, 0);
	if (pstTeam && iPInstID > 0)
	{
		pstTeam->iAutoPworldID = iPInstID;
		//autopworld_invite_enter_trans(pstEnv, pstPlayer, pstTeam, &pstPworld->stPworldDef);
	}
	return 0;
}

int diff_pworld_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
			TFRAMEHEAD	 *pstFrameHead, CSPKG *pstCsPkg)
{	
	CSDIFFPWORLDCLT *pstClt = &pstCsPkg->Body.DiffPworldClt;
	AUTOPWORLDENTERREQ stReq;
	UNUSED(pstFrameHead);
	UNUSED(pstAppCtx);
	

	switch(pstClt->Type)
	{
		case DIFF_PWORLD_CLT_ENTER:
			if(!is_my_master_svr(pstEnv->pstAppCtx->iId, pstPlayer))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,  ZONEERR_BATTLE_SVR_OP);
				return -1;
			}
			//diff_pworld_enter(pstEnv, pstPlayer, &pstClt->Data.Enter);
			
			stReq.DiffPworldID = pstClt->Data.Enter.DiffPworldID;
			stReq.PworldID = pstClt->Data.Enter.PworldID;
			
			autopworld_clt_enter(pstEnv, pstPlayer, &stReq, pstClt->Data.Enter.EnterFlag);
			break;
		case DIFF_PWORLD_CLT_SELECT_CARD:
			return  player_select_card(pstEnv, pstPlayer, pstClt->Data.Select.GridIdx);
		case DIFF_PWORLD_CLT_SELECT_VIP_CARD:
			return  player_select_vip_card(pstEnv, pstPlayer, pstClt->Data.VipSelect.GridIdx);
		default:
			return -1;
			break;
	}

	return 0;
}

int team_pworld_level_get(ZONESVRENV* pstEnv, ZoneTeam *pstTeam, ZONEPWORLD *pstPworld)
{
	int iMinLevel = 0;
	int iLevel = 0;

	if(pstPworld->stPworldDef.DiffPworldID > 0 || pstPworld->stPworldDef.DynType > 0)
	{
		z_get_team_avg_level(pstEnv->pstAppCtx, pstEnv, pstTeam, &iLevel);
		z_get_team_min_level(pstEnv->pstAppCtx, pstEnv, pstTeam, &iMinLevel);
		//取2者值小的那个
		if(iMinLevel+10 < iLevel)
		{
			iLevel = iMinLevel+10;
		}

		if(iLevel < pstPworld->stPworldDef.MinLv)
		{
			iLevel = pstPworld->stPworldDef.MinLv;
		}
	}
	//副本都准备个等级以后也许用得到
	else
	{
		iLevel = pstPworld->stPworldDef.MinLv;
	}

	return iLevel;
}

int single_pworld_level_get(Player *pstPlayer, ZONEPWORLD *pstPworld)
{
	int iLevel = 0;
	
	if(pstPworld->stPworldDef.DiffPworldID > 0 || pstPworld->stPworldDef.DynType > 0)
	{
		iLevel = pstPlayer->stRoleData.Level;
		if(iLevel < pstPworld->stPworldDef.MinLv)
		{
			iLevel = pstPworld->stPworldDef.MinLv;
		}
	}
	//副本都准备个等级以后也许用得到
	else
	{
		iLevel = pstPworld->stPworldDef.MinLv;
	}

	return iLevel;
}

int diff_pworld_dyn_level_get(ZONESVRENV* pstEnv, Player *pstPlayer, int iDiffPworldID)
{
	ZONEIDX stIdx;
	ZONEPWORLD *pstPworld;
	ZoneTeam *pstTeam;
	DIFFPWORLDDEF *pstDiffPworldDef = z_find_diff_pworld_def(pstEnv, iDiffPworldID);

	if(NULL == pstDiffPworldDef)
	{
		return 0;
	}
	
	stIdx.iID = pstDiffPworldDef->PworldInfo[0].PworldID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if(NULL == pstPworld || pstPworld->stPworldDef.DynType == 0)
	{
		return 0;
	}

	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(pstTeam)
	{
		return team_pworld_level_get(pstEnv, pstTeam, pstPworld);
	}
	else
	{
		return single_pworld_level_get(pstPlayer, pstPworld);
	}

	return 0;
}

int diff_pworld_grade_get(DIFFPWORLDDEF *pstDiffPworldDef, int iPworldID)
{
	int i;

	for(i = 0; i<MAX_DIFF_PWORLD_LIST; i++)
	{
		if(pstDiffPworldDef->PworldInfo[i].PworldID == iPworldID)
		{
			return i;
		}
	}

	return -1;
}

DIFFPWORLDLIST* diff_pworld_info_get(DIFFPWORLDDEF *pstDiffPworldDef, int iPworldID)
{
	int i;

	for(i = 0; i<MAX_DIFF_PWORLD_LIST; i++)
	{
		if(pstDiffPworldDef->PworldInfo[i].PworldID == iPworldID)
		{
			return &pstDiffPworldDef->PworldInfo[i];
		}
	}

	return NULL;
}

int pworld_select_waiter_add(Player *pstPlayer, ZONEPWORLDINST *pstPworldInst)
{
	if(pstPworldInst->stAwardInfo.iWaitSelectNum >= MAX_TEAM_MEMBER)
	{
		return -1;
	}

	pstPworldInst->stAwardInfo.aiWaiter[pstPworldInst->stAwardInfo.iWaitSelectNum] = pstPlayer->iMemID;
	pstPworldInst->stAwardInfo.iWaitSelectNum++;

	return 0;
}

int pworld_select_waiter_del(ZONEPWORLDINST *pstPworldInst, int iMemID)
{
	int i;
	for(i = pstPworldInst->stAwardInfo.iWaitSelectNum-1; i>=0; i--)
	{
		if(pstPworldInst->stAwardInfo.aiWaiter[i] == iMemID)
		{
			pstPworldInst->stAwardInfo.iWaitSelectNum--;
			if(i != pstPworldInst->stAwardInfo.iWaitSelectNum)
			{
				memcpy(&pstPworldInst->stAwardInfo.aiWaiter[i], 
					&pstPworldInst->stAwardInfo.aiWaiter[pstPworldInst->stAwardInfo.iWaitSelectNum], 
					sizeof(pstPworldInst->stAwardInfo.aiWaiter[0]));
			}
		}
	}

	return 0;
}

int pworld_select_vip_waiter_add(Player *pstPlayer, ZONEPWORLDINST *pstPworldInst)
{		
	if(pstPworldInst->stAwardInfo.iVipWaitSelectNum >= MAX_TEAM_MEMBER)
	{
		return -1;
	}

	pstPworldInst->stAwardInfo.aiVipWaiter[pstPworldInst->stAwardInfo.iVipWaitSelectNum] = pstPlayer->iMemID;
	pstPworldInst->stAwardInfo.iVipWaitSelectNum++;

	return 0;
}

int pworld_select_vip_waiter_del(ZONEPWORLDINST *pstPworldInst, int iMemID)
{
	int i;
	for(i = pstPworldInst->stAwardInfo.iVipWaitSelectNum-1; i>=0; i--)
	{
		if(pstPworldInst->stAwardInfo.aiVipWaiter[i] == iMemID)
		{
			pstPworldInst->stAwardInfo.iVipWaitSelectNum--;
			if(i != pstPworldInst->stAwardInfo.iVipWaitSelectNum)
			{
				memcpy(&pstPworldInst->stAwardInfo.aiVipWaiter[i], 
					&pstPworldInst->stAwardInfo.aiVipWaiter[pstPworldInst->stAwardInfo.iVipWaitSelectNum], 
					sizeof(pstPworldInst->stAwardInfo.aiVipWaiter[0]));
			}
		}
	}

	return 0;
}

int is_vip_wait_select_card_player(Player *pstPlayer, ZONEPWORLDINST *pstPworldInst)
{
	int i;

	for(i = 0; i< pstPworldInst->stAwardInfo.iVipWaitSelectNum; i++)
	{
		if(pstPworldInst->stAwardInfo.aiVipWaiter[i] == pstPlayer->iMemID)
		{
			return 1;
		}
	}

	return 0;
}

int is_wait_select_card_player(Player *pstPlayer, ZONEPWORLDINST *pstPworldInst)
{
	int i;

	for(i = 0; i< pstPworldInst->stAwardInfo.iWaitSelectNum; i++)
	{
		if(pstPworldInst->stAwardInfo.aiWaiter[i] == pstPlayer->iMemID)
		{
			return 1;
		}
	}

	return 0;
}

int is_free_pworld_vip_award_card(ZONEPWORLDINST *pstPworldInst, int iGridIdx)
{
	int i;

	for(i = 0; i< pstPworldInst->stAwardInfo.iVipCardNum; i++)
	{
		if(pstPworldInst->stAwardInfo.VipAwardCards[i].iGridIdx == iGridIdx &&
			pstPworldInst->stAwardInfo.VipAwardCards[i].szOwner[0] != 0)
		{
			return 0;
		}
	}

	return 1;
}

int is_free_pworld_award_card(ZONEPWORLDINST *pstPworldInst, int iGridIdx)
{
	int i;

	for(i = 0; i< pstPworldInst->stAwardInfo.iCardNum; i++)
	{
		if(pstPworldInst->stAwardInfo.AwardCards[i].iGridIdx == iGridIdx &&
			pstPworldInst->stAwardInfo.AwardCards[i].szOwner[0] != 0)
		{
			return 0;
		}
	}

	return 1;
}

int pworld_card_msg(ZONESVRENV* pstEnv, Player *pstPlayer, const char *pszStr)
{
	CSPKG stPkg;
	CSDIFFPWORLDSVR *pstDiffPworldSvr = &stPkg.Body.DiffPworldSvr;
	CSDIFFPWORLDCARDMSG* pstMsgInfo;

	pstMsgInfo = &pstDiffPworldSvr->Data.CardMsg;
	STRNCPY(pstMsgInfo->Msg, pszStr, CCH(pstMsgInfo->Msg));
	pstDiffPworldSvr->Type = DIFF_PWORLD_SVR_CARD_MSG;

	Z_CSHEAD_INIT(&stPkg.Head, DIFF_PWORLD_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int pworld_cards_rand_init(ZONEPWORLDINST *pstPworldInst)
{
	int i, j;

	g_iCardRangeCurr = MAX_PWORLD_AWARD_CARD;
	for(i=0; i<g_iCardRangeCurr; i++)
	{
		g_aiCardIdxMap[i] = i;
	}

	for(i=g_iCardRangeCurr -1; i>=0; i--)
	{
		for(j=pstPworldInst->stAwardInfo.iCardNum-1; j>=0; j--)
		{
			if(pstPworldInst->stAwardInfo.AwardCards[j].szOwner[0] != 0 &&
				pstPworldInst->stAwardInfo.AwardCards[j].iGridIdx == g_aiCardIdxMap[i])
			{
				g_iCardRangeCurr--;
				if(g_iCardRangeCurr != i)
				{
					 g_aiCardIdxMap[i] = g_aiCardIdxMap[g_iCardRangeCurr];
				}
			}
		}
	}

	return 0;
}

int pworld_cards_rand_idx_get()
{
	int iRand, iGridIdx;

	if(g_iCardRangeCurr <=0)
		return -1;

	iRand = RAND1(g_iCardRangeCurr);
	iGridIdx = g_aiCardIdxMap[iRand];

	g_iCardRangeCurr--;
	if(iRand != g_iCardRangeCurr)
	{
		g_aiCardIdxMap[iRand] = g_aiCardIdxMap[g_iCardRangeCurr];
	}
	
	return iGridIdx;
}

int pworld_vip_cards_rand_init(ZONEPWORLDINST *pstPworldInst)
{
	int i, j;

	g_iVipCardRangeCurr = MAX_PWORLD_VIP_AWARD_CARD;
	for(i=0; i<g_iVipCardRangeCurr; i++)
	{
		g_aiVipCardIdxMap[i] = i;
	}

	for(i=g_iVipCardRangeCurr-1; i>=0; i--)
	{
		for(j=pstPworldInst->stAwardInfo.iVipCardNum-1; j>=0; j--)
		{
			if(pstPworldInst->stAwardInfo.VipAwardCards[j].szOwner[0] != 0 &&
				pstPworldInst->stAwardInfo.VipAwardCards[j].iGridIdx == g_aiVipCardIdxMap[i])
			{
				g_iVipCardRangeCurr--;
				if(g_iVipCardRangeCurr != i)
				{
					 g_aiVipCardIdxMap[i] = g_aiVipCardIdxMap[g_iVipCardRangeCurr];
				}	
				break;
			}
		}
	}

	return 0;
}

int pworld_vip_cards_rand_idx_get()
{
	int iRand, iGridIdx;

	if(g_iVipCardRangeCurr <=0)
		return -1;

	iRand = RAND1(g_iVipCardRangeCurr);
	iGridIdx = g_aiVipCardIdxMap[iRand];

	g_iVipCardRangeCurr--;
	if(iRand != g_iVipCardRangeCurr)
	{
		g_aiVipCardIdxMap[iRand] = g_aiVipCardIdxMap[g_iVipCardRangeCurr];
	}
	
	return iGridIdx;
}

int pworld_vip_award_cards_end(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst)
{
	int i;
	ZONEMAPINST *pstMapInst;
	PworldAwardCard *pstPworldCardInfo;
	CSPKG stPkg;
	CSDIFFPWORLDSVR *pstDiffPworldSvr = &stPkg.Body.DiffPworldSvr;
	CSDIFFPWORLDUPDATEVIPCARD *pstCards = &pstDiffPworldSvr->Data.VipCards;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPworldInst->astMapInstIdx[0]);
	if (!pstMapInst)
	{
		return -1;
	}

	pstPworldInst->stAwardInfo.iVipStat = PWORLD_AWARD_END;
	pworld_vip_cards_rand_init(pstPworldInst);

	pstDiffPworldSvr->Type = DIFF_PWORLD_SVR_UPDATE_VIP_CARD;
	pstCards->CardNum = 0;
	for(i=0; i<pstPworldInst->stAwardInfo.iVipCardNum; i++)
	{
		pstPworldCardInfo = &pstPworldInst->stAwardInfo.VipAwardCards[i];
		
		if(pstPworldCardInfo->szOwner[0] == 0)
		{
			 pstPworldCardInfo->iGridIdx = pworld_vip_cards_rand_idx_get();
		}

		memcpy(&pstCards->CardsInfo[pstCards->CardNum].Grid, &pstPworldCardInfo->stGrid, sizeof(pstCards->CardsInfo[0].Grid));
		STRNCPY(pstCards->CardsInfo[pstCards->CardNum].RoleName, pstPworldCardInfo->szOwner, 
			CCH(pstCards->CardsInfo[0].RoleName));
		
		pstCards->CardsInfo[pstCards->CardNum].GridIdx = pstPworldCardInfo->iGridIdx;
		pstCards->CardNum++;
	}

	Z_CSHEAD_INIT(&stPkg.Head, DIFF_PWORLD_SVR);
	pworld_map_broadcast(pstEnv->pstAppCtx, pstEnv, pstPworldInst, pstMapInst, 0, &stPkg);

	return 0;
}

int pworld_award_cards_end(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst)
{
	int i;
	ZONEMAPINST *pstMapInst;
	PworldAwardCard *pstPworldCardInfo;
	CSPKG stPkg;
	CSDIFFPWORLDSVR *pstDiffPworldSvr = &stPkg.Body.DiffPworldSvr;
	CSDIFFPWORLDUPDATECARD *pstCards = &pstDiffPworldSvr->Data.Cards;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPworldInst->astMapInstIdx[0]);
	if (!pstMapInst)
	{
		return -1;
	}

	pstPworldInst->stAwardInfo.iStat = PWORLD_AWARD_END;
	pworld_cards_rand_init(pstPworldInst);

	pstDiffPworldSvr->Type = DIFF_PWORLD_SVR_UPDATE_CARD;
	pstCards->CardNum = 0;
	for(i=0; i<pstPworldInst->stAwardInfo.iCardNum; i++)
	{
		pstPworldCardInfo = &pstPworldInst->stAwardInfo.AwardCards[i];
		
		if(pstPworldCardInfo->szOwner[0] == 0)
		{
			 pstPworldCardInfo->iGridIdx = pworld_cards_rand_idx_get();
		}

		memcpy(&pstCards->CardsInfo[pstCards->CardNum].Grid, &pstPworldCardInfo->stGrid, sizeof(pstCards->CardsInfo[0].Grid));
		STRNCPY(pstCards->CardsInfo[pstCards->CardNum].RoleName, pstPworldCardInfo->szOwner, 
			CCH(pstCards->CardsInfo[0].RoleName));
		
		pstCards->CardsInfo[pstCards->CardNum].GridIdx = pstPworldCardInfo->iGridIdx;
		pstCards->CardNum++;
	}

	Z_CSHEAD_INIT(&stPkg.Head, DIFF_PWORLD_SVR);
	pworld_map_broadcast(pstEnv->pstAppCtx, pstEnv, pstPworldInst, pstMapInst, 0, &stPkg);

	return 0;
}

//不大好确定倒计时暂时时间久一点
int pworld_award_tick(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst)
{
	PworldAwardInfo *pstAwardInfo = &pstPworldInst->stAwardInfo;
	
	if(pstAwardInfo->tAwardTime == 0 || pstEnv->pstAppCtx->stCurr.tv_sec < pstAwardInfo->tAwardTime + PWORLD_PLAYER_SELECT_CARD_TIME+10)
	{
		return -1;
	}

	if(pstAwardInfo->iStat == PWORLD_AWARD_ASSIGN)
	{
		if(pstAwardInfo->iWaitSelectNum > 0)
		{
			int i;
			Player *pstWaitPlayer;
			
			pworld_cards_rand_init(pstPworldInst);
		
			for(i=pstAwardInfo->iWaitSelectNum-1; i>=0; i--)
			{
				pstWaitPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAwardInfo->aiWaiter[i]);
				if(pstWaitPlayer && pstWaitPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
				{
					 player_select_card(pstEnv, pstWaitPlayer, pworld_cards_rand_idx_get());
				}

				pworld_select_waiter_del(pstPworldInst, pstAwardInfo->aiWaiter[i]);
			}
		}
		
		pworld_award_cards_end(pstEnv, pstPworldInst);
	}

	if(pstAwardInfo->iVipStat == PWORLD_AWARD_ASSIGN)
	{
		if(pstAwardInfo->iVipWaitSelectNum > 0)
		{
			int i;
			Player *pstWaitPlayer;
			
			pworld_vip_cards_rand_init(pstPworldInst);
		
			for(i=pstAwardInfo->iVipWaitSelectNum-1; i>=0; i--)
			{
				pstWaitPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAwardInfo->aiVipWaiter[i]);
				if(pstWaitPlayer && pstWaitPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
				{
					 player_select_vip_card(pstEnv, pstWaitPlayer, pworld_vip_cards_rand_idx_get());
				}
				else
				{
					pworld_select_vip_waiter_del(pstPworldInst, pstAwardInfo->aiVipWaiter[i]);
				}
			}
		}
			
		pworld_vip_award_cards_end(pstEnv, pstPworldInst);
	}

	
	
	
	return 0;
}

int pworld_booty_group_item_create(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, int iItemID, int iNum)
{
	ROLEGRID *pstRoleGrid;

	if(pstPworldInst->stAwardInfo.iCardNum >= MAX_PWORLD_AWARD_CARD)
	{
		return -1;
	}
	
	pstRoleGrid = &pstPworldInst->stAwardInfo.AwardCards[pstPworldInst->stAwardInfo.iCardNum].stGrid;
	if (0 > item_create(pstEnv->pstAppCtx, pstEnv, pstRoleGrid, iItemID))
	{
		return -1;
	}
	
	if (pstRoleGrid->Type == GRID_ITEM_ITEM)
	{
		pstRoleGrid->GridData.RoleItem.Num = iNum;
	}

	//强制绑定
	pstRoleGrid->GridData.RoleItem.InstFlag |= INST_ITEM_BIND;

	//超过全局限制 只能看拿不到
	if(0 > world_item_limit_check(pstEnv, iItemID))
	{
		//分配概率的时候 -1视为分配0概率
		pstPworldInst->stAwardInfo.AwardCards[pstPworldInst->stAwardInfo.iCardNum].iRate = -1;
	}
	else
	{
		pstPworldInst->stAwardInfo.AwardCards[pstPworldInst->stAwardInfo.iCardNum].iRate = 0;
		
		//记录下全局限制的掉落
		world_item_limit_add(pstEnv, iItemID, iNum);
	}

	pstPworldInst->stAwardInfo.iCardNum++;
	return 0;
}

int pworld_booty_group_item_get(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, int iGroup, int iNum)
{
	int i;
	tdr_ulonglong iBase, iRand, iRate;
	int iCreateNum;
	char sMap[MAX_PWORLD_BOOTY_GROUP_ITEM];
	PWORLDBOOTYGROUPDEF *pstBooyGroupDef;

	//动态副本10级一个段
	if(pstPworldInst->DynType)
	{
		if(pstPworldInst->bPlayerMeanLevel > 20)
		{
			int iTmp = (pstPworldInst->bPlayerMeanLevel - 20)/10;
			
			iGroup = iGroup + iTmp*10;
		}
	}

	pstBooyGroupDef = z_find_pworld_bootygroup_def(pstEnv, pstPworldInst->stPworldIdx.iID, iGroup);
	if(NULL == pstBooyGroupDef)
	{
		return 0;
	}
	
	memset(sMap, 0, sizeof(sMap));

	iBase = 1000000;
	iCreateNum = iNum;
	while(iCreateNum-- && iBase > 0)
	{
		iRate = 0;
		iRand = RAND1(1000000);
		for(i = 0; i<MAX_PWORLD_BOOTY_GROUP_ITEM; i++)
		{
			if(iBase <= 0)
			{
				break;
			}

			if(sMap[i])
			{
				continue;
			}
			
			iRate += pstBooyGroupDef->BootyItem[i].Rate;
			if(iRand < iRate*1000000/iBase)
			{
				//不能制造出来，也算浪费了一个配额了
				iBase -= pstBooyGroupDef->BootyItem[i].Rate;
				sMap[i] = 1;
				
				pworld_booty_group_item_create(pstEnv, pstPworldInst, 
					pstBooyGroupDef->BootyItem[i].ItemID, pstBooyGroupDef->BootyItem[i].Num);
				break;
			}	
		}
	}
	
	return 0;
}

int pworld_vip_booty_group_item_create(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, int iItemID, int iNum)
{
	ROLEGRID *pstRoleGrid;

	if(pstPworldInst->stAwardInfo.iVipCardNum >= MAX_PWORLD_VIP_AWARD_CARD)
	{
		return -1;
	}
	
	pstRoleGrid = &pstPworldInst->stAwardInfo.VipAwardCards[pstPworldInst->stAwardInfo.iVipCardNum].stGrid;
	if (0 > item_create(pstEnv->pstAppCtx, pstEnv, pstRoleGrid, iItemID))
	{
		return -1;
	}
	
	if (pstRoleGrid->Type == GRID_ITEM_ITEM)
	{
		pstRoleGrid->GridData.RoleItem.Num = iNum;
	}

	//强制绑定
	pstRoleGrid->GridData.RoleItem.InstFlag |= INST_ITEM_BIND;

	//超过全局限制 只能看拿不到
	if(0 > world_item_limit_check(pstEnv, iItemID))
	{
		//分配概率的时候 -1视为分配0概率
		pstPworldInst->stAwardInfo.VipAwardCards[pstPworldInst->stAwardInfo.iVipCardNum].iRate = -1;
	}
	else
	{
		pstPworldInst->stAwardInfo.VipAwardCards[pstPworldInst->stAwardInfo.iVipCardNum].iRate = 0;
		
		//记录下全局限制的掉落
		world_item_limit_add(pstEnv, iItemID, iNum);
	}

	pstPworldInst->stAwardInfo.iVipCardNum++;
	return 0;
}

int pworld_vip_booty_group_item_get(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, int iGroup, int iNum)
{
	int i;
	tdr_ulonglong iBase, iRand, iRate;
	int iCreateNum;
	char sMap[MAX_PWORLD_BOOTY_GROUP_ITEM];
	PWORLDBOOTYGROUPDEF *pstBooyGroupDef;

	//动态副本10级一个段
	if(pstPworldInst->DynType)
	{
		if(pstPworldInst->bPlayerMeanLevel > 20)
		{
			int iTmp = (pstPworldInst->bPlayerMeanLevel - 20)/10;
			
			iGroup = iGroup + iTmp*10;
		}
	}

	pstBooyGroupDef = z_find_pworld_bootygroup_def(pstEnv, pstPworldInst->stPworldIdx.iID, iGroup);
	if(NULL == pstBooyGroupDef)
	{
		return 0;
	}
	
	memset(sMap, 0, sizeof(sMap));

	iBase = 1000000;
	iCreateNum = iNum;
	while(iCreateNum-- && iBase > 0)
	{
		iRate = 0;
		iRand = RAND1(1000000);
		for(i = 0; i<MAX_PWORLD_BOOTY_GROUP_ITEM; i++)
		{
			if(iBase <= 0)
			{
				break;
			}

			if(sMap[i])
			{
				continue;
			}
			
			iRate += pstBooyGroupDef->BootyItem[i].Rate;
			if(iRand < iRate*1000000/iBase)
			{
				//不能制造出来，也算浪费了一个配额了
				iBase -= pstBooyGroupDef->BootyItem[i].Rate;
				sMap[i] = 1;
				
				pworld_vip_booty_group_item_create(pstEnv, pstPworldInst, 
					pstBooyGroupDef->BootyItem[i].ItemID, pstBooyGroupDef->BootyItem[i].Num);
				break;
			}	
		}
	}
	
	return 0;
}

int pworld_vip_booty_create(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, PWORLDAWARDMULDEF *pstPworldAwardMulDef)
{
	int i, j, iNum, iGridNum;
	int iRate;
	PWORLDBOOTYGROUPLIST *pstBootyInfo;
	PworldAwardInfo *pstPworldAwardInfo = &pstPworldInst->stAwardInfo;
	
	iGridNum = 0;
	for(i=0; i<MAX_PWORLD_BOOTY_GROUP; i++)
	{
		pstBootyInfo = &pstPworldAwardMulDef->VipBootyGroup[i];

		//最后一个补全
		if(i == MAX_PWORLD_BOOTY_GROUP - 1)
		{
			iNum = MAX_PWORLD_VIP_AWARD_CARD - pstPworldAwardInfo->iVipCardNum;
		}
		else
		{
			if(pstBootyInfo->MaxNum > pstBootyInfo->MinNum)
			{
				iNum = pstBootyInfo->MinNum + RAND1(pstBootyInfo->MaxNum - pstBootyInfo->MinNum +1);
			}
			else
			{
				iNum = pstBootyInfo->MinNum;
			}
		}
		
		if(iNum <= 0)
		{
			//没有产出的组要去掉占用的概率部分
			pstPworldAwardInfo->iVipCardDecRate += pstBootyInfo->PlayerRate;
			continue;
		}
			
		iGridNum = pstPworldAwardInfo->iVipCardNum;
		//提取物品
		pworld_vip_booty_group_item_get(pstEnv, pstPworldInst, i+1, iNum);
		if(pstPworldAwardInfo->iVipCardNum <= iGridNum)
		{
			//没有产出的组要去掉占用的概率部分
			pstPworldAwardInfo->iVipCardDecRate += pstBootyInfo->PlayerRate;
			continue;
		}

		//给提取出来的物品分配概率
		iRate = 0;
		for(j= iGridNum; j<pstPworldAwardInfo->iVipCardNum; j++)
		{
			//最后一个补全概率
			if(j == pstPworldAwardInfo->iVipCardNum-1)
			{
				//最后一个是只看不能拿的剔除掉概率
				if(pstPworldAwardInfo->VipAwardCards[j].iRate == -1)
				{
					//剔除这个物品所占有的概率
					pstPworldAwardInfo->iVipCardDecRate += pstBootyInfo->PlayerRate - iRate;
					pstPworldAwardInfo->VipAwardCards[j].iRate = 0;
				}
				else
				{
					pstPworldAwardInfo->VipAwardCards[j].iRate = pstBootyInfo->PlayerRate - iRate;
				}
			}
			else
			{
				if(pstPworldAwardInfo->VipAwardCards[j].iRate != -1)
				{
					pstPworldAwardInfo->VipAwardCards[j].iRate = pstBootyInfo->PlayerRate/(pstPworldAwardInfo->iVipCardNum - iGridNum);
					iRate += pstPworldAwardInfo->VipAwardCards[j].iRate;
				}
			}
		}
	}

	if(pstPworldAwardInfo->iVipCardNum < MAX_PWORLD_VIP_AWARD_CARD)
	{
		//TODO: log
		return -1;
	}
	
	return 0;
}

int pworld_booty_create(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, int iValGrade)
{
	int i, j, iNum, iGridNum;
	int iRate;
	PWORLDAWARDMULDEF *pstPworldAwardMulDef;
	PWORLDBOOTYGROUPLIST *pstBootyInfo;
	PworldAwardInfo *pstPworldAwardInfo = &pstPworldInst->stAwardInfo;

	pstPworldAwardMulDef = z_find_pworld_award_mul_def(pstEnv, 
								pstPworldInst->stPworldIdx.iID, iValGrade);
	if(NULL == pstPworldAwardMulDef)
	{
		return -1;
	}

	iGridNum = 0;
	for(i=0; i<MAX_PWORLD_BOOTY_GROUP; i++)
	{
		pstBootyInfo = &pstPworldAwardMulDef->BootyGroup[i];

		//最后一个补全
		if(i == MAX_PWORLD_BOOTY_GROUP - 1)
		{
			iNum = MAX_PWORLD_AWARD_CARD - pstPworldAwardInfo->iCardNum;
		}
		else
		{
			if(pstBootyInfo->MaxNum > pstBootyInfo->MinNum)
			{
				iNum = pstBootyInfo->MinNum + RAND1(pstBootyInfo->MaxNum - pstBootyInfo->MinNum +1);
			}
			else
			{
				iNum = pstBootyInfo->MinNum;
			}
		}
		
		if(iNum <= 0)
		{
			//没有产出的组要去掉占用的概率部分
			pstPworldAwardInfo->iCardDecRate += pstBootyInfo->PlayerRate;
			continue;
		}
			

		iGridNum = pstPworldAwardInfo->iCardNum;
		//提取物品
		pworld_booty_group_item_get(pstEnv, pstPworldInst, i+1, iNum);
		if(pstPworldAwardInfo->iCardNum <= iGridNum)
		{
			//没有产出的组要去掉占用的概率部分
			pstPworldAwardInfo->iCardDecRate += pstBootyInfo->PlayerRate;
			continue;
		}

		//给提取出来的物品分配概率
		iRate = 0;
		for(j= iGridNum; j<pstPworldAwardInfo->iCardNum; j++)
		{
			//最后一个补全概率
			if(j == pstPworldAwardInfo->iCardNum-1)
			{
				//最后一个是只看不能拿的剔除掉概率
				if(pstPworldAwardInfo->AwardCards[j].iRate == -1)
				{
					//剔除这个物品所占有的概率
					pstPworldAwardInfo->iCardDecRate += pstBootyInfo->PlayerRate - iRate;
					pstPworldAwardInfo->AwardCards[j].iRate = 0;
				}
				else
				{
					pstPworldAwardInfo->AwardCards[j].iRate = pstBootyInfo->PlayerRate - iRate;
				}
			}
			else
			{
				if(pstPworldAwardInfo->AwardCards[j].iRate != -1)
				{
					pstPworldAwardInfo->AwardCards[j].iRate = pstBootyInfo->PlayerRate/(pstPworldAwardInfo->iCardNum - iGridNum);
					iRate += pstPworldAwardInfo->AwardCards[j].iRate;
				}
			}
		}
	}

	if(pstPworldAwardInfo->iCardNum < MAX_PWORLD_AWARD_CARD)
	{
		//TODO: log
		return -1;
	}

	if(0 > pworld_vip_booty_create(pstEnv, pstPworldInst, pstPworldAwardMulDef))
	{
		return -1;
	}

	return 0;
	
}

//计算副本评价得分
int pworld_award_cacl_val(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, 
	DIFFPWORLDLIST*pstDiffPworldInfo, CSDIFFPWORLDAWARDUI *pstUIInfo)
{
	int iDieNum;
	
	//普通怪物
	pstUIInfo->NormalNum = pstPworldInst->stAwardInfo.iNormalNum;
	pstUIInfo->NormalVal = pstPworldInst->stAwardInfo.iNormalNum*1;
	
	//精英怪物
	pstUIInfo->JingNum = pstPworldInst->stAwardInfo.iJingNum;
	pstUIInfo->JingVal = pstPworldInst->stAwardInfo.iJingNum*5;
	
	//BOSS
	pstUIInfo->BossNum = pstPworldInst->stAwardInfo.iBossNum;
	pstUIInfo->BossVal = pstPworldInst->stAwardInfo.iBossNum*20;

	iDieNum = pstUIInfo->DieNum = pstPworldInst->stAwardInfo.iDieNum;
	//死亡超过20次后的不在扣分
	if(iDieNum > 20)
		iDieNum = 20;

	//死亡扣分
	pstUIInfo->DieVal = iDieNum*3;

	//时间得分
	pstUIInfo->TimeSec = pstEnv->pstAppCtx->stCurr.tv_sec - pstPworldInst->tStart;
	pstUIInfo->TimeVal = pstDiffPworldInfo->BaseTimeVal - pstUIInfo->TimeSec;
	if(pstUIInfo->TimeVal < 0)
	{
		pstUIInfo->TimeVal = 0;
	}

	//	合计得分为【击杀得分】+【时间得分】-【死亡扣分】。
	pstUIInfo->TotalVal = pstUIInfo->NormalVal + pstUIInfo->JingVal + pstUIInfo->BossVal 
			-pstUIInfo->DieVal+pstUIInfo->TimeVal;

	if(pstUIInfo->TotalVal <= 0)
		pstUIInfo->TotalVal = 1;

	return pstUIInfo->TotalVal;
}

//分数对应评价等级
int pworld_award_val2type(int iVal, int iBaseVal, int iGrade, int fDyn)
{
	int iValRate;

	if(iBaseVal <=0)
		return PWORLD_AWARD_VAL_D;

	iValRate = iVal*100.0/iBaseVal;

	if(iValRate >= 90)
	{
		if(!fDyn)
		{
			if(iGrade <= DIFF_PWORLD_GRADE_C)
			{
				return PWORLD_AWARD_VAL_B;
			}
			else if(iGrade == DIFF_PWORLD_GRADE_B)
			{
				return PWORLD_AWARD_VAL_A;
			}
		}
		
		return PWORLD_AWARD_VAL_S;
	}
	else if(iValRate >= 80)
	{
		if(!fDyn)
		{
			if(iGrade <= DIFF_PWORLD_GRADE_C)
			{
				return PWORLD_AWARD_VAL_B;
			}
		}
		
		return PWORLD_AWARD_VAL_A;
	}
	else if(iValRate >= 70)
	{
		return PWORLD_AWARD_VAL_B;
	}
	else if(iValRate >= 60)
	{
		return PWORLD_AWARD_VAL_C;
	}
	else
	{
		return PWORLD_AWARD_VAL_D;
	}

	return PWORLD_AWARD_VAL_D;
}

int pworld_player_add_exp(ZONESVRENV* pstEnv, Player *pstPlayer, int iExp)
{
	PlayerChagAttrVal stChgVal;
	AttrChgVal *pstVal;

	stChgVal.AttrNum = 0;
	if(iExp > 0)
	{
		pstVal = &stChgVal.AttrList[stChgVal.AttrNum++];
		pstVal->AttrID = ATTR_ID_EXP;
		pstVal->nFlag = 0;
		pstVal->Value.Exp = iExp;
	}

	pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_EXP_NOLEVEL_LIMIT;
	if (stChgVal.AttrNum > 0)
	{
		z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);
	}
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_EXP_NOLEVEL_LIMIT;
	
	return 0;
}

static int has_friend_player_in_pworld(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, Player *pstPlayer)
{
	int i;
	Player *pstTmpPlayer;
	
	for(i=0; i<pstPworldInst->iPlayer; i++)
	{
		if(pstPworldInst->aiPlayerID[i] == pstPlayer->iMemID)
		{
			continue;
		}
		
		pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i]);
		if(NULL == pstTmpPlayer || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}

		if(online_info_find_friend_byID(&pstPlayer->stRoleData.Buddy, pstTmpPlayer->stRoleData.RoleID) >=0)
		{
			return 1;
		}
	}

	return 0;
}

static int has_clan_player_in_pworld(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, Player *pstPlayer)
{
	int i;
	Player *pstTmpPlayer;
	ZoneClan *pstMyClan = player_get_clan(pstEnv, pstPlayer);
	ZoneClan *pstOtherClan;

	if(NULL == pstMyClan)
	{
		return 0;
	}
	
	for(i=0; i<pstPworldInst->iPlayer; i++)
	{
		if(pstPworldInst->aiPlayerID[i] == pstPlayer->iMemID)
		{
			continue;
		}
		
		pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i]);
		if(NULL == pstTmpPlayer || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}

		pstOtherClan = player_get_clan(pstEnv, pstTmpPlayer);
		if(pstOtherClan && pstOtherClan == pstMyClan)
		{
			return 1;
		}
	}
	
	return 0;
}

static int pworld_award_other_exp(ZONESVRENV* pstEnv, ZONEPWORLD *pstPworld, ZONEPWORLDINST *pstPworldInst, Player *pstPlayer,
	DIFFPWORLDDEF *pstDiffPworldDef, CSDIFFPWORLDAWARDUI *pstUIInfo)
{
	int iDiffGrade;
	tdr_longlong llExp;
	PWORLDAWARDMULDEF *pstPworldAwardMulDef;

	pstPworldAwardMulDef = z_find_pworld_award_mul_def(pstEnv, pstPworldInst->stPworldIdx.iID, pstUIInfo->ValGrade);
	if(NULL == pstPworldAwardMulDef)
	{
		return -1;
	}

	pstUIInfo->GradeExp = 0;
	//通关副本后，评价等级在A级或以上时
	if(pstUIInfo->ValGrade >= PWORLD_AWARD_VAL_A)
	{
		//奖励经验=副本通关奖励经验*评价等级对应系数
		pstUIInfo->GradeExp = pstUIInfo->Exp*pstPworldAwardMulDef->AddExpMul/100.0;
	}

	pstUIInfo->DiffExp = 0;
	//难度奖励，非动态副本
	if(pstPworldInst->DynType == 0)
	{
		iDiffGrade = diff_pworld_grade_get(pstDiffPworldDef, pstPworldInst->stPworldIdx.iID);
		if(iDiffGrade >= DIFF_PWORLD_GRADE_B && iDiffGrade < MAX_DIFF_PWORLD_LIST)
		{
			//副本难度：奖励经验=副本通关奖励经验*副本难度对应系数
			pstUIInfo->DiffExp = pstUIInfo->Exp*pstDiffPworldDef->PworldInfo[iDiffGrade].AwardExpMul/100.0;
		}
	}

	pstUIInfo->FriendExp = 0;
	//好友奖励
	if(has_friend_player_in_pworld(pstEnv, pstPworldInst, pstPlayer))
	{
		//好友组队：奖励经验=副本通关奖励经验*2%；
		pstUIInfo->FriendExp = pstUIInfo->Exp*2/100.0;
	}

	pstUIInfo->ClanExp = 0;
	//公会奖励
	if(has_clan_player_in_pworld(pstEnv, pstPworldInst, pstPlayer))
	{
		//公会组队：奖励经验=副本通关奖励经验*2%
		pstUIInfo->ClanExp = pstUIInfo->Exp*2/100.0;
	}

	//世界补偿经验
	llExp = pstUIInfo->Exp;
	player_world_level_exp(pstEnv, pstPlayer, &llExp);
	pstUIInfo->Exp = llExp;

	pstUIInfo->OffExp = 0;
	//补偿
	if(pstPworld->stPworldDef.OffExpMul > 0)
	{
		pstUIInfo->OffExp = player_pworld_award_offexp(pstEnv, pstPlayer, pstUIInfo->Exp*6);
	}
	
	return 0;
}

static int pworld_award_exp_money(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst,
	DIFFPWORLDLIST*pstDiffPworldInfo, CSDIFFPWORLDAWARDUI *pstUIInfo)
{
	PWORLDAWARDMULDEF *pstPworldAwardMulDef;

	pstPworldAwardMulDef = z_find_pworld_award_mul_def(pstEnv, pstPworldInst->stPworldIdx.iID, pstUIInfo->ValGrade);
	if(NULL == pstPworldAwardMulDef)
	{
		return -1;
	}
	
	//动态副本
	if(pstPworldInst->DynType)
	{
		double iExp;
		double iMoney;
		int iValMul;
		LEVELUP *pstLevelDef = z_get_levelup(pstEnv, pstPworldInst->bPlayerMeanLevel);
		if(NULL == pstLevelDef)
		{
			return -1;
		}

		iExp = (longlong )pstDiffPworldInfo->BaseExp*pstLevelDef->ContributeExp/10000.0;
		iMoney = (longlong )pstDiffPworldInfo->BaseMoney*pstLevelDef->ContributeMoney/10000.0;

		iValMul = pstUIInfo->TotalVal*100/pstDiffPworldInfo->BaseVal;
		if(iValMul > 100)
		{
			iValMul = 100;
		}

		//奖励经验 = 基准奖励经验（读取数据库表，动态取）*（合计得分/副本基准分）*评价等级对应系数。
		pstUIInfo->Exp = iExp*iValMul*pstPworldAwardMulDef->ExpMul/100.0/100.0;

		//奖励金钱 = 基准奖励金钱（读取数据库表，动态取）*（合计得分/副本基准分）*评价等级对应系数。
		pstUIInfo->Money = iMoney*iValMul*pstPworldAwardMulDef->MoneyMul/100.0/100.0;
	}
	else
	{
		int iValMul;
		
		iValMul = pstUIInfo->TotalVal*100/pstDiffPworldInfo->BaseVal;
		if(iValMul > 100)
		{
			iValMul = 100;
		}
		
		//奖励经验 = 基准奖励经验*（合计得分/副本基准分）*评价等级对应系数。
		pstUIInfo->Exp = (longlong )pstDiffPworldInfo->BaseExp*iValMul*pstPworldAwardMulDef->ExpMul/100.0/100.0;

		//奖励金钱 = 基准奖励金钱*（合计得分/副本基准分）*评价等级对应系数。
		pstUIInfo->Money = (longlong )pstDiffPworldInfo->BaseMoney*iValMul*pstPworldAwardMulDef->MoneyMul/100.0/100.0;
	}

	if(pstUIInfo->Exp < 0)
		pstUIInfo->Exp = 0;

	if(pstUIInfo->Money < 0)
		pstUIInfo->Money = 0;
	return 0;
}

int pworld_award_card2cs(CSDIFFPWORLDAWARDUI *pstUIInfo, ZONEPWORLDINST *pstPworldInst)
{
	int i;
	DIFFPWORLDCARDINFO *pstUICardInfo;

	pworld_cards_rand_init(pstPworldInst);
	for(i=0; i<pstPworldInst->stAwardInfo.iCardNum; i++)
	{
		if(pstUIInfo->CardNum >= MAX_PWORLD_AWARD_CARD)
		{
			return 0;
		}
		
		pstUICardInfo = &pstUIInfo->CardsInfo[pstUIInfo->CardNum];
		pstUICardInfo->Grid = pstPworldInst->stAwardInfo.AwardCards[i].stGrid;
		pstUICardInfo->GridIdx = pworld_cards_rand_idx_get();
		STRNCPY(pstUICardInfo->RoleName, pstPworldInst->stAwardInfo.AwardCards[i].szOwner, CCH(pstUICardInfo->RoleName));

		pstUIInfo->CardNum++;
	}

	pworld_vip_cards_rand_init(pstPworldInst);
	for(i=0; i<pstPworldInst->stAwardInfo.iVipCardNum; i++)
	{
		if(pstUIInfo->VipCardNum >= MAX_PWORLD_VIP_AWARD_CARD)
		{
			return 0;
		}
		
		pstUICardInfo = &pstUIInfo->VipCardsInfo[pstUIInfo->VipCardNum];
		pstUICardInfo->Grid = pstPworldInst->stAwardInfo.VipAwardCards[i].stGrid;
		pstUICardInfo->GridIdx = pworld_vip_cards_rand_idx_get();
		STRNCPY(pstUICardInfo->RoleName, pstPworldInst->stAwardInfo.VipAwardCards[i].szOwner, CCH(pstUICardInfo->RoleName));

		pstUIInfo->VipCardNum++;
	}

	return 0;
}

int pworld_award_ui_comm_calc(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, DIFFPWORLDDEF *pstDiffPworldDef,
		DIFFPWORLDLIST*pstDiffPworldInfo, CSDIFFPWORLDAWARDUI *pstUIInfo)
{
	int iDiffGrade = diff_pworld_grade_get(pstDiffPworldDef, pstPworldInst->stPworldIdx.iID);
	
	//评分
	pworld_award_cacl_val(pstEnv, pstPworldInst, pstDiffPworldInfo, pstUIInfo);

	//评价S- D
	pstUIInfo->ValGrade = pworld_award_val2type(pstUIInfo->TotalVal, pstDiffPworldInfo->BaseVal, iDiffGrade, pstPworldInst->DynType);

	//评价经验金钱
	if(0 > pworld_award_exp_money(pstEnv, pstPworldInst, pstDiffPworldInfo, pstUIInfo))
	{
		// TODO: log
		return -1;
	}

	//产生物品
	if(0 > pworld_booty_create(pstEnv, pstPworldInst, pstUIInfo->ValGrade))
	{
		// TODO: log
		return -1;
	}

	pworld_award_card2cs(pstUIInfo, pstPworldInst);

	return 0;
}

//打开副本奖励界面
int pworld_award_open_ui(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, ZONEPWORLD *pstPworld)
{
	int i;
	tdr_longlong iCommExp, iExp = 0;
	Player * pstPlayer;
	DIFFPWORLDDEF *pstDiffPworldDef;
	DIFFPWORLDLIST*pstDiffPworldInfo;
	CSPKG stPkg;
	CSDIFFPWORLDSVR *pstDiffPworldSvr = &stPkg.Body.DiffPworldSvr;
	CSDIFFPWORLDAWARDUI *pstUIInfo = &pstDiffPworldSvr->Data.AwardUI;

	pstDiffPworldDef = z_find_diff_pworld_def(pstEnv, pstPworld->stPworldDef.DiffPworldID);
	if(NULL == pstDiffPworldDef)
	{
		return -1;
	}

	pstDiffPworldInfo = diff_pworld_info_get(pstDiffPworldDef, pstPworld->stPworldDef.PworldID);
	if(NULL == pstDiffPworldInfo)
	{
		return -1;
	}

	memset(pstUIInfo, 0, sizeof(*pstUIInfo));

	pstUIInfo->DiffPworldID = pstPworld->stPworldDef.DiffPworldID;
	pstUIInfo->PworldID = pstPworld->stPworldDef.PworldID;
	
	//公共部分
	if(0 > pworld_award_ui_comm_calc(pstEnv, pstPworldInst, pstDiffPworldDef, pstDiffPworldInfo, pstUIInfo))
	{
		return -1;
	}

	//副本奖励日志流水号
	z_oplog_event_begin(pstEnv);
	pstPworldInst->stAwardInfo.ullLogWID = z_get_logwid(pstEnv);

	iCommExp = pstUIInfo->Exp;
	for(i=0; i<pstPworldInst->iPlayer; i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i]);
		if(NULL == pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}

		//还原公共经验部分
		pstUIInfo->Exp = iCommExp;

		//防止不计算副本次数
		pstPlayer->stRoleData.MiscInfo.InPworldSafeTime = 0;

		//给予抽牌的权利
		pworld_select_waiter_add(pstPlayer, pstPworldInst);

		//VIP抽牌的权利
		if (star_get_result(pstEnv, pstPlayer,STAR_RESULT_PWORLD_CARD))
		{
			pworld_select_vip_waiter_add(pstPlayer, pstPworldInst);
		}

		//额外个人奖励部分
		pworld_award_other_exp(pstEnv, pstPworld, pstPworldInst, pstPlayer, pstDiffPworldDef, pstUIInfo);

		//趁这个时候把奖励 金钱、经验给予玩家
		if (pstUIInfo->Money > 0)
		{
			package_add_money(pstEnv, pstPlayer, MONEY_TYPE_BULL, pstUIInfo->Money);
			z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_PWORLD, 0,  pstUIInfo->Money, 0, "副本奖励");
		}

		if(pstPlayer->stRoleData.Level >= 65 && pstPworld->stPworldDef.PworldID == PWORLD_LONG_DAN_GU)
		{
			/*
			pstUIInfo->Exp = 0;
			pstUIInfo->OffExp = 0;
			pstUIInfo->GradeExp = 0;
			pstUIInfo->DiffExp = 0;
			pstUIInfo->FriendExp = 0;
			pstUIInfo->ClanExp = 0;
			*/

			//原有的经验的10%
			pstUIInfo->Exp = pstUIInfo->Exp / 10.0;
			pstUIInfo->OffExp = pstUIInfo->OffExp / 10.0;
			pstUIInfo->GradeExp = pstUIInfo->GradeExp / 10.0;
			pstUIInfo->DiffExp = pstUIInfo->DiffExp / 10.0;
			pstUIInfo->FriendExp = pstUIInfo->FriendExp / 10.0;
			pstUIInfo->ClanExp = pstUIInfo->ClanExp / 10.0;

			iExp = pstUIInfo->Exp+pstUIInfo->OffExp+pstUIInfo->GradeExp+pstUIInfo->DiffExp+pstUIInfo->FriendExp+pstUIInfo->ClanExp;
			pworld_player_add_exp(pstEnv, pstPlayer, iExp);
		}
		else
		{
			iExp = pstUIInfo->Exp+pstUIInfo->OffExp+pstUIInfo->GradeExp+pstUIInfo->DiffExp+pstUIInfo->FriendExp+pstUIInfo->ClanExp;
			pworld_player_add_exp(pstEnv, pstPlayer, iExp);
		}
		
		//人人显示的自己部分都不同的
		pstDiffPworldSvr->Type = DIFF_PWORLD_SVR_AWARD_UI;
		Z_CSHEAD_INIT(&stPkg.Head, DIFF_PWORLD_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

		//师徒声望奖励
		player_pworld_award_schoolval(pstEnv, pstPlayer);

		//加亲密度
		friend_team_in_pworld_chg_buddyval(pstEnv, pstPworld,pstPworldInst, pstPlayer);

		//完成每日副本任务
		task_pworld_fini_trace(pstEnv,pstPlayer,pstPworld->stPworldDef.PworldID);
		
		z_role_pworld_award_oplog(pstEnv, pstPlayer, pstPworld->stPworldDef.PworldID, iExp, pstUIInfo->Money);
	}

	pstPworldInst->stAwardInfo.tAwardTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstPworldInst->stAwardInfo.iStat = PWORLD_AWARD_ASSIGN;
	pstPworldInst->stAwardInfo.iVipStat = PWORLD_AWARD_ASSIGN;
	return 0;
}

int player_award_card(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLDINST *pstPworldInst, ZONEMAPINST* pstMapInst,
		PworldAwardCard *pstPworldCard, int iCardIdx)
{
	CSPKG stPkg;
	CSDIFFPWORLDSVR *pstDiffPworldSvr = &stPkg.Body.DiffPworldSvr;
	CSDIFFPWORLDUPDATECARD *pstCards = &pstDiffPworldSvr->Data.Cards;
	ZONEPWORLD *pstPworld;

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if (!pstPworld)
	{
		return 0;
	}

	STRNCPY(pstPworldCard->szOwner, pstPlayer->stRoleData.RoleName, CCH(pstPworldCard->szOwner));
	pstPworldCard->iGridIdx = iCardIdx;
	//不能再被抽取了
	pstPworldCard->iRate = 0;

	//删除抽牌的权利
	pworld_select_waiter_del(pstPworldInst, pstPlayer->iMemID);

	pstDiffPworldSvr->Type = DIFF_PWORLD_SVR_UPDATE_CARD;
	pstCards->CardNum = 1;
	STRNCPY(pstCards->CardsInfo[0].RoleName, pstPlayer->stRoleData.RoleName, CCH(pstCards->CardsInfo[0].RoleName));
	pstCards->CardsInfo[0].GridIdx = pstPworldCard->iGridIdx;
	memcpy(&pstCards->CardsInfo[0].Grid, &pstPworldCard->stGrid, sizeof(pstCards->CardsInfo[0].Grid));

	Z_CSHEAD_INIT(&stPkg.Head, DIFF_PWORLD_SVR);
	pworld_map_broadcast(pstEnv->pstAppCtx, pstEnv, pstPworldInst, pstMapInst, 0, &stPkg);

	//副本奖励日志流水号
	z_set_logwid(pstEnv, pstPworldInst->stAwardInfo.ullLogWID);

	if(pstPworldCard->stGrid.GridData.RoleItem.DefID == PWORLD_CARD_EXP_ITEMID)
	{
		pworld_player_add_exp(pstEnv, pstPlayer, pstPworldCard->stGrid.GridData.RoleItem.Num);

		z_role_pworld_card_oplog(pstEnv, pstPlayer, pstPworldInst->stPworldIdx.iID, 
			pstPworldCard->stGrid.GridData.RoleItem.Num, 0, NULL);
	}
	else if(pstPworldCard->stGrid.GridData.RoleItem.DefID == PWORLD_CARD_MONEY_ITEMID)
	{
		package_add_money(pstEnv, pstPlayer, MONEY_TYPE_BULL, pstPworldCard->stGrid.GridData.RoleItem.Num);
		z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_PWORLD_CARD, 0,  pstPworldCard->stGrid.GridData.RoleItem.Num, 0, "副本翻牌奖励");

		z_role_pworld_card_oplog(pstEnv, pstPlayer, pstPworldInst->stPworldIdx.iID, 
			0, pstPworldCard->stGrid.GridData.RoleItem.Num, NULL);
	}
	else
	{
		ROLEGRID stOpGrid;

		stOpGrid = pstPworldCard->stGrid;
		if(0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &pstPworldCard->stGrid, 1, 0))
		{
			char szMsg[128];

			sprintf(szMsg, LJY_MAIL7, pstPworld->stPworldDef.Name);
			pworld_card_msg(pstEnv, pstPlayer, LJY_PWORLD101);
			player_sysmail_fast_send(pstEnv, pstPlayer->stRoleData.RoleName, 
					szMsg, 
					LJY_MAIL8, 
					&pstPworldCard->stGrid, 1, 0,NULL, 0);
		}
		else
		{
			 package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &pstPworldCard->stGrid, 1, 0, IMPITEM_OPTYPE_CARD);
		}

		z_role_pworld_card_oplog(pstEnv, pstPlayer, pstPworldInst->stPworldIdx.iID, 0,0, &stOpGrid);
	}
	
	
	return 0;
}

int player_award_vip_card(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLDINST *pstPworldInst, ZONEMAPINST* pstMapInst,
		PworldAwardCard *pstPworldCard, int iCardIdx)
{
	CSPKG stPkg;
	CSDIFFPWORLDSVR *pstDiffPworldSvr = &stPkg.Body.DiffPworldSvr;
	CSDIFFPWORLDUPDATEVIPCARD *pstCards = &pstDiffPworldSvr->Data.VipCards;
	ZONEPWORLD *pstPworld;

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if (!pstPworld)
	{
		return 0;
	}

	STRNCPY(pstPworldCard->szOwner, pstPlayer->stRoleData.RoleName, CCH(pstPworldCard->szOwner));
	pstPworldCard->iGridIdx = iCardIdx;
	//不能再被抽取了
	pstPworldCard->iRate = 0;

	//删除抽牌的权利
	pworld_select_vip_waiter_del(pstPworldInst, pstPlayer->iMemID);

	pstDiffPworldSvr->Type = DIFF_PWORLD_SVR_UPDATE_VIP_CARD;
	pstCards->CardNum = 1;
	STRNCPY(pstCards->CardsInfo[0].RoleName, pstPlayer->stRoleData.RoleName, CCH(pstCards->CardsInfo[0].RoleName));
	pstCards->CardsInfo[0].GridIdx = pstPworldCard->iGridIdx;
	memcpy(&pstCards->CardsInfo[0].Grid, &pstPworldCard->stGrid, sizeof(pstCards->CardsInfo[0].Grid));

	Z_CSHEAD_INIT(&stPkg.Head, DIFF_PWORLD_SVR);
	pworld_map_broadcast(pstEnv->pstAppCtx, pstEnv, pstPworldInst, pstMapInst, 0, &stPkg);

	//副本奖励日志流水号
	z_set_logwid(pstEnv, pstPworldInst->stAwardInfo.ullLogWID);

	if(pstPworldCard->stGrid.GridData.RoleItem.DefID == PWORLD_CARD_EXP_ITEMID)
	{
		pworld_player_add_exp(pstEnv, pstPlayer, pstPworldCard->stGrid.GridData.RoleItem.Num);

		z_role_pworld_card_oplog(pstEnv, pstPlayer, pstPworldInst->stPworldIdx.iID, 
			pstPworldCard->stGrid.GridData.RoleItem.Num, 0, NULL);
	}
	else if(pstPworldCard->stGrid.GridData.RoleItem.DefID == PWORLD_CARD_MONEY_ITEMID)
	{
		package_add_money(pstEnv, pstPlayer, MONEY_TYPE_BULL, pstPworldCard->stGrid.GridData.RoleItem.Num);
		z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_PWORLD_CARD, 0,  pstPworldCard->stGrid.GridData.RoleItem.Num, 0, "副本翻牌奖励");

		z_role_pworld_card_oplog(pstEnv, pstPlayer, pstPworldInst->stPworldIdx.iID, 
			0, pstPworldCard->stGrid.GridData.RoleItem.Num, NULL);
	}
	else
	{
		ROLEGRID stOpGrid;

		stOpGrid = pstPworldCard->stGrid;
		if(0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &pstPworldCard->stGrid, 1, 0))
		{
			char szMsg[128];

			sprintf(szMsg, LJY_MAIL7, pstPworld->stPworldDef.Name);
			pworld_card_msg(pstEnv, pstPlayer, LJY_PWORLD101);
			player_sysmail_fast_send(pstEnv, pstPlayer->stRoleData.RoleName, 
					szMsg, 
					LJY_MAIL8, 
					&pstPworldCard->stGrid, 1, 0,NULL, 0);
		}
		else
		{
			 package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &pstPworldCard->stGrid, 1, 0, IMPITEM_OPTYPE_CARD);
		}

		z_role_pworld_card_oplog(pstEnv, pstPlayer, pstPworldInst->stPworldIdx.iID, 0,0, &stOpGrid);
	}
	
	return 0;
}

int player_select_card(ZONESVRENV* pstEnv, Player *pstPlayer, int iCardIdx)
{
	int i;
	tdr_ulonglong iBaseRate, iRand, iRate;
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst = NULL;
	ZONEPWORLDINST *pstPworldInst = NULL;
	CSPKG stPkg;
	CSDIFFPWORLDSVR *pstDiffPworldSvr = &stPkg.Body.DiffPworldSvr;
	CSDIFFPWORLDSELECTRES *pstRes = &pstDiffPworldSvr->Data.Select;
	
	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos	=	pstPlayer->stOnline.iMapPos;
	
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		goto _error;
	}
		
	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (NULL == pstPworldInst || pstPworldInst->stAwardInfo.iStat != PWORLD_AWARD_ASSIGN)
	{
		goto _error;
	}

	if(!is_wait_select_card_player(pstPlayer, pstPworldInst))
	{
		goto _error;
	}

	if(!is_free_pworld_award_card(pstPworldInst, iCardIdx))
	{
		goto _error;
	}

	iRate = 0;
	iRand = RAND1(1000000);
	iBaseRate = 1000000 - pstPworldInst->stAwardInfo.iCardDecRate;
	if(iBaseRate <= 0)
	{
		goto _error;
	}

	for(i = 0; i<pstPworldInst->stAwardInfo.iCardNum; i++)
	{
		
		iRate += pstPworldInst->stAwardInfo.AwardCards[i].iRate;
		if(iRand < iRate*1000000/iBaseRate)
		{
			//调整概率归一化
			pstPworldInst->stAwardInfo.iCardDecRate += pstPworldInst->stAwardInfo.AwardCards[i].iRate;
			
			player_award_card(pstEnv, pstPlayer, pstPworldInst, 
					pstMapInst, &pstPworldInst->stAwardInfo.AwardCards[i], iCardIdx);
			
			//这个物品不在会命中
			pstPworldInst->stAwardInfo.AwardCards[i].iRate = 0;
			break;
		}
	}

	pstRes->Succ = 1;
	pstDiffPworldSvr->Type = DIFF_PWORLD_SVR_SELECT;
	Z_CSHEAD_INIT(&stPkg.Head, DIFF_PWORLD_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	if(pstPworldInst->stAwardInfo.iWaitSelectNum <= 0)
	{
		pworld_award_cards_end(pstEnv, pstPworldInst);
	}

	if(pstPworldInst->stAwardInfo.iVipWaitSelectNum <= 0)
	{
		pworld_vip_award_cards_end(pstEnv, pstPworldInst);
	}

	return 0;
	
_error:
	
	pstRes->Succ = 0;
	pstDiffPworldSvr->Type = DIFF_PWORLD_SVR_SELECT;
	Z_CSHEAD_INIT(&stPkg.Head, DIFF_PWORLD_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int player_select_vip_card(ZONESVRENV* pstEnv, Player *pstPlayer, int iCardIdx)
{
	int i;
	tdr_ulonglong iBaseRate, iRand, iRate;
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst = NULL;
	ZONEPWORLDINST *pstPworldInst = NULL;
	CSPKG stPkg;
	CSDIFFPWORLDSVR *pstDiffPworldSvr = &stPkg.Body.DiffPworldSvr;
	CSDIFFPWORLDSELECTRES *pstRes = &pstDiffPworldSvr->Data.Select;
	
	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos	=	pstPlayer->stOnline.iMapPos;
	
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		goto _error;
	}
		
	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (NULL == pstPworldInst || pstPworldInst->stAwardInfo.iVipStat != PWORLD_AWARD_ASSIGN)
	{
		goto _error;
	}

	if(!is_vip_wait_select_card_player(pstPlayer, pstPworldInst))
	{
		goto _error;
	}

	if(!is_free_pworld_vip_award_card(pstPworldInst, iCardIdx))
	{
		goto _error;
	}

	iRate = 0;
	iRand = RAND1(1000000);
	iBaseRate = 1000000 - pstPworldInst->stAwardInfo.iVipCardDecRate;
	if(iBaseRate <= 0)
	{
		goto _error;
	}

	for(i = 0; i<pstPworldInst->stAwardInfo.iVipCardNum; i++)
	{
		
		iRate += pstPworldInst->stAwardInfo.VipAwardCards[i].iRate;
		if(iRand < iRate*1000000/iBaseRate)
		{
			//调整概率归一化
			pstPworldInst->stAwardInfo.iVipCardDecRate += pstPworldInst->stAwardInfo.VipAwardCards[i].iRate;
			
			player_award_vip_card(pstEnv, pstPlayer, pstPworldInst, 
					pstMapInst, &pstPworldInst->stAwardInfo.VipAwardCards[i], iCardIdx);
			
			//这个物品不在会命中
			pstPworldInst->stAwardInfo.VipAwardCards[i].iRate = 0;
			break;
		}
	}

	pstRes->Succ = 1;
	pstDiffPworldSvr->Type = DIFF_PWORLD_SVR_SELECT_VIP;
	Z_CSHEAD_INIT(&stPkg.Head, DIFF_PWORLD_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	if(pstPworldInst->stAwardInfo.iVipWaitSelectNum <= 0)
	{
		pworld_vip_award_cards_end(pstEnv, pstPworldInst);
	}

	return 0;
	
_error:
	
	pstRes->Succ = 0;
	pstDiffPworldSvr->Type = DIFF_PWORLD_SVR_SELECT_VIP;
	Z_CSHEAD_INIT(&stPkg.Head, DIFF_PWORLD_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}


int pworld_mon_count(ZONEPWORLDINST *pstPworldInst, Monster *pstMon)
{
	switch(pstMon->bSubType)
	{
		case MON_SUB_TYPE_MINI:
		case MON_SUB_TYPE_SOFT:
		case MON_SUB_TYPE_WEAK:
		case MON_SUB_TYPE_NORM:
			pstPworldInst->stAwardInfo.iNormalNum++;
			break;
		case MON_SUB_TYPE_JIN:
		case MON_SUB_TYPE_HARD:
			pstPworldInst->stAwardInfo.iJingNum++;
			break;
		//case MON_SUB_TYPE_JI:
		case MON_SUB_TYPE_XI:
		case MON_SUB_TYPE_BOSS:
			pstPworldInst->stAwardInfo.iBossNum++;
			break;
		default:
			break;
	}

	return 0;
}

int pworld_val_trace_mon_die(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, Monster *pstMon)
{
	MONSTERDEF *pstMonDef;

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if(NULL == pstMonDef)
	{
		return -1;
	}

	if(pstMonDef->MiscType & MON_MISC_TYPE_NO_PWORLD_VAL)
	{
		return -1;
	}
	
	pworld_mon_count(pstPworldInst, pstMon);
	
	// 对连锁怪物的处理
 	if(pstMon->iLinkedNext >= 0 && pstMon->iLinkedPrev >= 0)
 	{
		Monster *pstLinkedMon = NULL;

		for(pstLinkedMon = tmempool_get(pstEnv->pstMonsterPool,
		                                pstMon->iLinkedNext);
		   pstLinkedMon != pstMon;
		   pstLinkedMon = tmempool_get(pstEnv->pstMonsterPool,
		                                pstLinkedMon->iLinkedNext))
		{
			if (!pstLinkedMon )
			{
				break;
			}

			pworld_mon_count(pstPworldInst, pstLinkedMon);
		}
	}

	return 0;
}

int pworld_val_trace_player_die(ZONEPWORLDINST *pstPworldInst)
{
	pstPworldInst->stAwardInfo.iDieNum++;
	return 0;
}

int player_is_in_diffpworld(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iMap = pstPlayer->stRoleData.Map;
	ZONEPWORLD *pstPworld;

	pstPworld = z_find_pworld2(pstEnv, iMap);
	if (!pstPworld)
		return 0;
	if (pstPworld->stPworldDef.DiffPworldID <= 0)
		return 0;
	
	return 1;
}

int player_team_create_syn_diffpworld(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneTeam *pstTeam)
{
	ZONEPWORLDINST *pstZInst;
	if (player_is_in_diffpworld(pstEnv, pstPlayer))
	{
		pstZInst =  z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);
		if (pstZInst)
			pstTeam->iAutoPworldInstID = pstZInst->iPworldInstID;
	}
	return 0;
}

int pworld_box_start(ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst)
{
	int i;
	Player *pstPlayer;
	PWORLDBOXDEF *pstPworldBoxDef;
	ZONEPWORLDINST *pstPworldInst;
	MapIndex *pstMapIndex;
	MAPNPC stMapNpc;

	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (NULL == pstPworldInst)
	{
		return -1;
	}

	if(pstPworldInst->stPworldBoxInfo.iPlayerNum > 0)
	{
		return -1;
	}
		
	pstPworldBoxDef = z_find_pworld_box_def(pstEnv, pstPworldInst->stPworldIdx.iID);
	if(!pstPworldBoxDef)
	{
		return -1;
	}

	if(RAND1(100) >= pstPworldBoxDef->Rate)
	{
		return -1;
	}

	pstMapIndex = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIndex)
	{
	        return -1;
	}

	memset(&stMapNpc, 0, sizeof(stMapNpc));
	stMapNpc.MonsterID = pstPworldBoxDef->NpcID;
	stMapNpc.Pos = pstPworldBoxDef->Pos;
	STRNCPY(stMapNpc.Script, pstPworldBoxDef->Script, sizeof(stMapNpc.Script));

	if (0 > z_in_new_pworld_npc(pstEnv->pstAppCtx, pstEnv, pstMapIndex, pstMapInst, &stMapNpc, 0))
	{
		return -1;;
	}

	for(i=0; i<pstPworldInst->iPlayer; i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i]);
		if(!pstPlayer)
		{
			continue;
		}

		pstPworldInst->stPworldBoxInfo.astBoxPlayer[pstPworldInst->stPworldBoxInfo.iPlayerNum].iMemID = pstPlayer->iMemID;
		pstPworldInst->stPworldBoxInfo.astBoxPlayer[pstPworldInst->stPworldBoxInfo.iPlayerNum].iOpen = 0;
		pstPworldInst->stPworldBoxInfo.iPlayerNum++;
	}
	
	return 0;
}

int pworld_box_award(ZONESVRENV* pstEnv, Player *pstPlayer, Npc *pstNpc, ZONEMAPINST *pstMapInst)
{
	int i;
	ADDMONBOOTYGROUP stMonBooty;
	PWORLDBOXDEF *pstPworldBoxDef;
	ZONEPWORLDINST *pstPworldInst;
	PworldBoxInfo *pstPworldBoxInfo;
	PworldBoxPlayerInfo *pstPworldBoxPlayerInfo;

	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (NULL == pstPworldInst)
	{
		return -1;
	}

	pstPworldBoxDef = z_find_pworld_box_def(pstEnv, pstPworldInst->stPworldIdx.iID);
	if(!pstPworldBoxDef)
	{
		return -1;
	}

	pstPworldBoxPlayerInfo = NULL;
	pstPworldBoxInfo = &pstPworldInst->stPworldBoxInfo;
	for(i=0; i<pstPworldBoxInfo->iPlayerNum; i++)
	{
		if(pstPworldBoxInfo->astBoxPlayer[i].iMemID == pstPlayer->iMemID)
		{
			pstPworldBoxPlayerInfo = &pstPworldBoxInfo->astBoxPlayer[i];
			break;
		}
	}

	if(!pstPworldBoxPlayerInfo)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_DIFF_301);
		return -1;
	}

	if(pstPworldBoxPlayerInfo->iOpen)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_DIFF_301);
		return -1;
	}

	pstNpc->stMon.cFirstAtkerType = MON_BOOTY_OWNER_PLAYER;
	pstNpc->stMon.iFirstAtkerID = pstPlayer->iMemID;
	pstNpc->stMon.ullFirstAtkerWid = pstPlayer->stRoleData.RoleID;
	pstNpc->stMon.iFirstAtkerMemID = pstPlayer->iMemID;

	for(i=0; i<PWORLD_BOX_GROUP_NUM; i++)
	{
		if(pstPworldBoxDef->Group[i].GroupID == 0)
		{
			continue;
		}

		if(pstPlayer->stRoleData.Level >= pstPworldBoxDef->Group[i].MinLV &&
			pstPlayer->stRoleData.Level <= pstPworldBoxDef->Group[i].MaxLV)
		{
			stMonBooty.GroupID = pstPworldBoxDef->Group[i].GroupID;
			stMonBooty.CollectTime = 0;
			stMonBooty.Flag = 0;
			
			mon_booty_group_assign(pstEnv, &pstNpc->stMon,
					&stMonBooty, NULL);
			break;
		}
	}

	pstPworldBoxPlayerInfo->iOpen = 1;
	pstNpc->stMon.cFirstAtkerType = 0;
	pstNpc->stMon.iFirstAtkerID = 0;
	pstNpc->stMon.ullFirstAtkerWid = 0;
	pstNpc->stMon.iFirstAtkerMemID = 0;
	
	return 0;
}
