/*
**  @file $RCSfile: zone_gay.c,v $
**  general description of this module
**  $Id: zone_gay.c,v 1.10 2013/05/14 09:27:15 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2013/05/14 09:27:15 $
**  @version $Revision: 1.10 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_clt.h"
#include "zone_svr.h"
#include "zone_team.h"
#include "zone_booty.h"
#include "zone_move.h"
#include "zone_attr.h"
#include "zone_package.h"
#include "zone_err.h"

int gay_invite_valid_player(Player *pstGay, Player *pstPlayer)
{
	if (NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus || 
		pstPlayer->stRoleData.Map != pstGay->stRoleData.Map)
	{
		return -1;
	}
	
	if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE && !z_pos_in_view(&pstGay->stRoleData.Pos, &pstPlayer->stRoleData.Pos))
	{
		return -1;
	}
	
	return 0;
}

int player_gay_invite(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	Player *apstPlayer[MAX_TEAM_MEMBER];
	int i, iNum = 0;
	Player *pstTmpPlayer;
	ZoneTeam *pstTeam;
	TEAMMEMBER *pstTeamMember;
	CSPKG stPkg;
	CSGAYSVR *pstGaySvr = &stPkg.Body.GaySvr;

	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(!pstTeam)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GAY1);
		return -1;
	}

	pstTeamMember = player_team_get_captain(pstTeam);
	// 找不到队长
	if (NULL == pstTeamMember )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GAY1);
		return -1;
	}

	// 申请的人不是队长
	if (0 != strcmp(pstTeamMember->MemberName, pstPlayer->stRoleData.RoleName))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GAY1);
		return -1;
	}

	if(0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, GAY_ITEM_ID1, 1, -1,  NULL, PACKAGE_DEC_NONE))
	{
		ITEMDEF *pstItemDef = z_find_itemdef(pstEnv, GAY_ITEM_ID1);
		if(pstItemDef)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GAY2, pstItemDef->Name);
		}
		return -1;
	}
	
	for(i = 0; i < pstTeam->stTeamInfo.MemberNum; i++)
	{
		pstTeamMember = &pstTeam->stTeamInfo.TeamMember[i];
		
		pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeamMember->MemberName);	
		if(!pstTmpPlayer)
		{
			continue;
		}

		if(pstPlayer == pstTmpPlayer)
		{
			continue;
		}

		if(0 > gay_invite_valid_player(pstPlayer, pstTmpPlayer))
		{
			continue;
		}

		apstPlayer[iNum++] = pstTmpPlayer;
	}
	
	if(iNum > 0 )
	{
		pstGaySvr->Type = GAY_SVR_INVITE;
		STRNCPY(pstGaySvr->Data.Invite.RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstGaySvr->Data.Invite.RoleName));
		Z_CSHEAD_INIT(&stPkg.Head, GAY_SVR);
		z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iNum, &stPkg);

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GAY3);
	}
	else
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GAY4);
	}

	return 0;
}

int player_gay_invite_reject(ZONESVRENV *pstEnv, Player *pstPlayer, CSGAYREJECT *pstCSReject)
{
	Player *pstGay;

	pstGay = z_name_player(pstEnv->pstAppCtx, pstEnv, pstCSReject->RoleName);
	if(!pstGay || PLAYER_STATUS_ROLE_LOGIN != pstGay->eStatus)
	{
		return -1;
	}

	if(pstCSReject->Flag)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstGay, SYS_MIDDLE, ZONEERR_GAY5, pstPlayer->stRoleData.RoleName);
	}
	else
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstGay, SYS_MIDDLE, ZONEERR_GAY6, pstPlayer->stRoleData.RoleName);
	}
	
	return 0;
}

int player_gay_invite_accept(ZONESVRENV *pstEnv, Player *pstPlayer, CSGAYACCEPT *pstCSAccept)
{
	Player *pstGay;
	ZoneTeam *pstTeam;
	TEAMMEMBER *pstTeamMember;

	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(!pstTeam)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GAY7);
		return -1;
	}

	pstTeamMember = player_team_get_captain(pstTeam);
	if (NULL == pstTeamMember )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GAY8);
		return -1;
	}

	if (0 != strcmp(pstTeamMember->MemberName, pstCSAccept->RoleName))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GAY8);
		return -1;
	}

	pstGay = z_name_player(pstEnv->pstAppCtx, pstEnv, pstCSAccept->RoleName);
	if(!pstGay || PLAYER_STATUS_ROLE_LOGIN != pstGay->eStatus)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GAY8);
		return -1;
	}

	if(IsSameDay(pstGay->stRoleData.MiscInfo.GayInfo.GayTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GAY8);
		return -1;
	}

	if(0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstGay, GAY_ITEM_ID1, 1, -1,  NULL, PACKAGE_DEC_NONE))
	{
		ITEMDEF *pstItemDef = z_find_itemdef(pstEnv, GAY_ITEM_ID1);
		if(pstItemDef)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstGay, SYS_MIDDLE, ZONEERR_GAY2, pstItemDef->Name);
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GAY9, 
				pstGay->stRoleData.RoleName, pstItemDef->Name);
		}
		return -1;
	}

	if(0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, GAY_ITEM_ID2, 1, -1,  NULL, PACKAGE_DEC_NONE))
	{
		ITEMDEF *pstItemDef = z_find_itemdef(pstEnv, GAY_ITEM_ID2);
		if(pstItemDef)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstGay, SYS_MIDDLE, ZONEERR_GAY9, 
				pstPlayer->stRoleData.RoleName, pstItemDef->Name);
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GAY2, pstItemDef->Name);
		}
		return -1;
	}

	if(0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstGay, GAY_ITEM_ID1, 1, -1,  NULL, PACKAGE_DEC_NONE, 0))
	{
		return -1;
	}

	if(0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, GAY_ITEM_ID2, 1, -1,  NULL, PACKAGE_DEC_NONE, 0))
	{
		return -1;
	}

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstGay, SYS_MIDDLE, ZONEERR_GAY10, pstPlayer->stRoleData.RoleName);
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GAY11, pstGay->stRoleData.RoleName);

	pstGay->stRoleData.MiscInfo.GayInfo.GayTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstGay->stRoleData.MiscInfo.GayInfo.GayRoleID = pstPlayer->stRoleData.RoleID;
	STRNCPY(pstGay->stRoleData.MiscInfo.GayInfo.GayRoleName, 
		pstPlayer->stRoleData.RoleName, 
		sizeof(pstGay->stRoleData.MiscInfo.GayInfo.GayRoleName));
	
	return 0;
}

//基友配对
int player_gay_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSGAYCLT *pstGayClt = &pstCsPkg->Body.GayClt;

	switch(pstGayClt->Type)
	{
		case GAY_CLT_ACCEPT:
			return player_gay_invite_accept(pstEnv, pstPlayer, &pstGayClt->Data.Accept);
			break;
		case GAY_CLT_REJECT:
			return player_gay_invite_reject(pstEnv, pstPlayer, &pstGayClt->Data.Reject);
			break;
		default:
			break;
	}
	return 0;
}

//基友奖励
int player_gay_award(ZONESVRENV* pstEnv, Player *pstPlayer, TASKRECYCLEDEF *pstTaskRecyleDef)
{
	int iExp;
	Player *pstOther;
	ZoneTeam *pstTeam;
	TEAMMEMBER *pstTeamMember;
	GAYINFO *pstGayInfo = &pstPlayer->stRoleData.MiscInfo.GayInfo;
	GAYDEF *pstGayDef = (GAYDEF *)&pstEnv->pstZoneObj->sGayDef[0];
	LEVELUP *pstLevelUP;

	if(!(pstTaskRecyleDef->CtrlFlag & CYC_CTRL_FLAG_GAY))
	{
		return -1;
	}

	if(!IsSameDay(pstGayInfo->GayTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		return -1;
	}
	
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(!pstTeam)
	{
		return -1;
	}

	pstOther = z_name_player(pstEnv->pstAppCtx, pstEnv, pstGayInfo->GayRoleName);
	if(!pstOther)
	{
		return -1;
	}

	pstTeamMember = player_team_get_captain(pstTeam);
	if (NULL == pstTeamMember )
	{
		return -1;
	}

	if(0 != strcmp(pstTeamMember->MemberName, pstPlayer->stRoleData.RoleName))
	{
		return -1;
	}
	
	if(!player_team_same(pstEnv, pstPlayer, pstOther))
	{
		return -1;
	}

	if(0 > gay_invite_valid_player(pstPlayer, pstOther))
	{
		return -1;
	}

	pstLevelUP = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	if (NULL == pstLevelUP)
	{
		return -1;
	}

	//配对发起者，攻方奖励
	iExp = pstGayDef->GayGongExp*(pstLevelUP->ContributeExp/10000.0);

	//攻的活力值高，奖励就多点
	if(pstPlayer->stRoleData.RoleDetail.GreenJade >= 5000)
	{
		iExp += pstGayDef->GayGongVigExp*(pstLevelUP->ContributeExp/10000.0);
	}
	player_add_exp(pstEnv, pstPlayer, iExp, 0);

	pstLevelUP = z_get_levelup(pstEnv, pstOther->stRoleData.Level);
	if (NULL == pstLevelUP)
	{
		return -1;
	}

	//受的奖励
	iExp = pstGayDef->GayShouExp*(pstLevelUP->ContributeExp/10000.0);
	player_add_exp(pstEnv, pstOther, iExp, 0);
	
	return 0;
}

//是不是一个基友配对的有效发起者
int is_gay_gong(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	GAYINFO *pstGayInfo = &pstPlayer->stRoleData.MiscInfo.GayInfo;

	if(IsSameDay(pstGayInfo->GayTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		return 1;
	}

	return 0;
}
