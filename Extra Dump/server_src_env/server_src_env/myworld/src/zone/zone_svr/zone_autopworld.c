

#include "zone_autopworld.h"
#include "zone_team.h"
#include "zone_diffpworld.h"
#include "zone_clt.h"
#include "zone_map.h"
#include "zone_package.h"
#include "zone_combat.h"
#include "zone_err.h"
#include "zone_ploy.h"

int autopworld_invite_enter1(ZONESVRENV* pstEnv,ZoneTeam *pstTeam, int iPworldID)
{
	int i;
	CSPKG stPkg;
	AUTOPWORLDSVR *pstSvr= &stPkg.Body.AutoPworldSvr;
	Player *pstTmp;

	memset(pstSvr, 0, sizeof(*pstSvr));
	pstSvr->Cmd = AUTOPWORLD_SVR_OP_ENTER;
	pstSvr->Data.EnterTransmit.PworldID = iPworldID;
	pstSvr->Data.EnterTransmit.CancelFlag = 0;
	Z_CSHEAD_INIT(&stPkg.Head, AUTOPWORLD_SVR);
	
	for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		pstTmp = z_name_player(pstEnv->pstAppCtx,  pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
		if (!pstTmp || PLAYER_STATUS_ROLE_LOGIN != pstTmp->eStatus)
		{
			continue;
		}

		if (pstTeam->stTeamInfo.TeamMember[i].Position == TEAM_POSITION_CAPTAIN)
		{
			continue;
		}
		
		z_cltmsg_send(pstEnv, pstTmp, &stPkg);	
	}
	
	return 0;
}

// iCancel=0表示通知客户端弹出邀请面板 1-取消面板
int autopworld_invite_enter(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID, int iCancel, ZoneTeam *pstTeam, int iTip)
{
	int i;
	CSPKG stPkg;
	AUTOPWORLDSVR *pstSvr= &stPkg.Body.AutoPworldSvr;

	if (iPworldID <= 0)
		return 0;

	memset(pstSvr, 0, sizeof(*pstSvr));
	pstSvr->Cmd = AUTOPWORLD_SVR_OP_ENTER;
	pstSvr->Data.EnterTransmit.PworldID = iPworldID;
	pstSvr->Data.EnterTransmit.CancelFlag = iCancel;
	
	Z_CSHEAD_INIT(&stPkg.Head, AUTOPWORLD_SVR);
	if (iCancel)
	{
		player_team_broadcast(pstEnv, pstTeam, &stPkg);
		if (iTip)
		{
			ZONEPWORLD *pstPworld;
			ZONEIDX stIdx;
	
			stIdx.iID = pstTeam->iAutoPworldID;
			stIdx.iPos = -1;
			pstPworld =  z_find_pworld(pstEnv->pstMapObj, &stIdx);
			if (!pstPworld)
			{
				return 0;
			}	
			z_team_sys_strf(pstEnv, pstTeam, SYS_TEAM, ZONEERR_TEAM301,  pstPworld->stPworldDef.Name);
		}

		pstTeam->iAutoPworldID = 0;
		pstTeam->iAutoPworldStart = 0;	
	}
	else
	{
		pstSvr->Data.EnterTransmit.StartTM = pstTeam->iAutoPworldStart;
		for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
		{
			pstSvr->Data.EnterTransmit.TeamInfo[pstSvr->Data.EnterTransmit.TeamMemNum].MemID = 
												pstTeam->stTeamInfo.TeamMember[i].MemID;
			pstSvr->Data.EnterTransmit.TeamInfo[pstSvr->Data.EnterTransmit.TeamMemNum].Ready  = 
												pstTeam->stTeamInfo.TeamMember[i].EnterPworldFlag;
			pstSvr->Data.EnterTransmit.TeamMemNum ++;
			
		}
		if (pstPlayer)
		{
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);	
		}
		else
		{
			player_team_broadcast(pstEnv, pstTeam, &stPkg);
		}
	}
	return 0;
}

int autopworld_id_get(ZONESVRENV* pstEnv, ZoneTeam *pstTeam)
{
	ZONEPWORLDINST *pstPInst = z_find_pworld_inst(pstEnv->pstMapObj, pstTeam->iAutoPworldInstID);

	if (pstPInst)
	{
		return pstTeam->iAutoPworldInstID;
	}
	return 0;
}

int autopworld_check_one(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLD *pstZonePworld, 
							Player *pstCap, int iCreate)
{
	Player *pstTip = NULL;

	if (pstCap)
		pstTip = pstCap;
	else
		pstTip = pstPlayer;

	if (!pstTip)
		return -1;


	if (pstPlayer->stRoleData.Level < pstZonePworld->stPworldDef.MinLv)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTip,SYS_MIDDLE2, ZONEERR_PWORLD101,
					pstPlayer->stRoleData.RoleName);
		return -1;
	}

	if (pstZonePworld->stPworldDef.MaxLv > 0 &&
		pstPlayer->stRoleData.Level > pstZonePworld->stPworldDef.MaxLv)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstTip,SYS_MIDDLE2, ZONEERR_PWORLD102,
					pstPlayer->stRoleData.RoleName);
		return -1;
	}
	
	if (iCreate)
	{
		if (z_check_pworld_enter_num(pstEnv, pstPlayer, &pstZonePworld->stPworldDef, 0) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTip,SYS_MIDDLE2,  ZONEERR_PWORLD103,
						pstPlayer->stRoleData.RoleName, pstZonePworld->stPworldDef.Name);
			return -1;
		}
	}
	
	return 0;
}


int autopworld_clt_enter(ZONESVRENV* pstEnv, Player *pstPlayer, AUTOPWORLDENTERREQ *pstReq, int iEnterFlag)
{
	ZONEPWORLD *pstZonePworld;
	ZONEIDX stIdx;
	ZoneTeam *pstTeam;
	PWORLDSELECT stPwolrdSelect;
	int i;
	ZONEPWORLDINST *pstPInst;
	Player *pstCap;
	int iCreate = 0;
	Player *pstTmp;
	int iPinstID;
	ZONEPWORLD *pstZPworld;

	//pstTeam = player_team_get(pstEnv, pstPlayer);
	
	if (!poly_is_mzt_pworld(pstReq->PworldID))
	{
		if(!is_diff_pworld_grade(pstEnv, pstReq->DiffPworldID, pstReq->PworldID))
		{
			return -1;
		}
	}
	
	stIdx.iID = pstReq->PworldID;
	stIdx.iPos = -1;

	pstZonePworld =  z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (!pstZonePworld)
	{
		return -1;
	}

	if (z_player_goto_check(pstEnv, pstPlayer, 0, NULL) < 0)
	{
		return -1;	
	}

	if (player_is_in_logout_to_revivepoint_map( pstEnv,  pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_DIANFENG31);
		return -1;
	}

	pstTeam = player_team_get(pstEnv, pstPlayer);
	if (pstTeam)
	{
		ZONEPWORLD *pstTmpPworld = pstZonePworld;
		ZONEIDX stTmp;

		pstPInst = z_find_pworld_inst(pstEnv->pstMapObj, pstTeam->iAutoPworldInstID);
		if(pstPInst  && pstPInst->stPworldIdx.iID != pstReq->PworldID)
		{
			//if (!poly_is_mzt_pworld(pstReq->PworldID))
			{
				pstZPworld = z_find_pworld(pstEnv->pstMapObj, &pstPInst->stPworldIdx);
				if (!pstZPworld)
					return -1;
				
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PWORLD104, 
								pstZPworld->stPworldDef.Name);
				return -1;
			}
		}

		pstCap = player_team_get_captain_player(pstEnv,pstTeam);
		if (!pstCap)
			return -1;

		if (!pstPInst)
		{
			iCreate = 1;

			if (poly_is_mzt_pworld(pstReq->PworldID))
			{
				stTmp.iID = ploy_mzt_get_pworldid(pstEnv, pstTeam);
				stTmp.iPos = -1;

				pstTmpPworld =  z_find_pworld(pstEnv->pstMapObj, &stTmp);
				if (!pstTmpPworld)
				{
					z_team_sys_strf(pstEnv, pstTeam, SYS_MIDDLE, ZONEERR_TEAM319);
					return -1;
				}
			}
		}	

		if (autopworld_check_one( pstEnv, pstPlayer, pstTmpPworld, NULL,iCreate) < 0)
		{
			return -1;
		}

		if (iCreate )
		{
			if (pstCap == pstPlayer)
			{
				ZONEPWORLD *pstTmpPworld;
				ZONEIDX stTmp;

				if (pstPlayer->stOnline.State & CS_STAT_FIGHT)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PWORLD105);
					return -1;
				}
				
				if (pstZonePworld->stPworldDef.AutoPworldNum > 0)
				{
					if (pstTeam->stTeamInfo.MemberNum < pstZonePworld->stPworldDef.AutoPworldNum)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PWORLD106, 
									pstZonePworld->stPworldDef.Name, pstZonePworld->stPworldDef.AutoPworldNum);
						return -1;
					}

					if (poly_is_mzt_pworld(pstReq->PworldID))
					{
						stTmp.iID = ploy_mzt_get_pworldid(pstEnv, pstTeam);
						stTmp.iPos = -1;

						pstTmpPworld =  z_find_pworld(pstEnv->pstMapObj, &stTmp);
						if (!pstTmpPworld)
						{
							return -1;
						}
					}
					else
					{
						pstTmpPworld = pstZonePworld;
					}

					for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
					{
						pstTeam->stTeamInfo.TeamMember[i].EnterPworldFlag = 0;
						pstTmp = z_name_player(pstEnv->pstAppCtx,  pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
						if (!pstTmp || PLAYER_STATUS_ROLE_LOGIN != pstTmp->eStatus)
						{
							z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PWORLD107, 
										pstTeam->stTeamInfo.TeamMember[i].MemberName);
							return -1;
						}

						if (pstTmp == pstCap)
						{
							pstTeam->stTeamInfo.TeamMember[i].EnterPworldFlag = 1;
						}

						if (autopworld_check_one( pstEnv, pstTmp, pstTmpPworld, pstCap,iCreate) < 0)
						{
							return -1;
						}
					}

					pstTeam->iAutoPworldID = pstReq->PworldID;
					pstTeam->iAutoPworldStart = pstEnv->pstAppCtx->stCurr.tv_sec;

					if (poly_is_mzt_pworld(pstTeam->iAutoPworldID))
					{
						int iMztPworldID = ploy_mzt_get_pworldid(pstEnv, pstTeam);
						if (iMztPworldID > 0)
						{
							pstTeam->iAutoPworldID = iMztPworldID;	
						}
						else
						{
							return -1;
						}
						poly_mzt_team_layer_notify(pstEnv,  pstTeam);
					}

					autopworld_invite_enter(pstEnv,NULL, pstZonePworld->stPworldDef.PworldID, 0, pstTeam,0);

				}
				else
				{
					if (!iEnterFlag)
					{
						if (diff_pworld_team_check_tips( pstEnv,pstTeam, pstPlayer,pstZonePworld) < 0)
						{
							return -1;
						}
					}

					memset(&stPwolrdSelect, 0, sizeof(stPwolrdSelect));
					if ((iPinstID = z_player_enter_pworld(pstEnv, pstPlayer,pstReq->PworldID, &stPwolrdSelect, 0)) < 0)
					{
						return -1;
					}

					pstTeam->iAutoPworldInstID = iPinstID;
					autopworld_invite_enter(pstEnv, NULL,pstZonePworld->stPworldDef.PworldID, 1, pstTeam,0);
					autopworld_invite_enter1(pstEnv,pstTeam, pstReq->PworldID);
				}
				
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PWORLD108);
				return -1;
			}
		}
		else
		{
			if (pstPlayer->stOnline.State & CS_STAT_FIGHT)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PWORLD105);
				return -1;
			}

			if (poly_is_mzt_pworld(pstReq->PworldID))
			{
				int iLayer = poly_mzt_get_team_layer(pstEnv,pstTeam) ;
				if (iLayer < pstPlayer->stRoleData.MiscInfo.MztInfo.Layer)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_RONGYAN14,
							pstPlayer->stRoleData.MiscInfo.MztInfo.Layer, iLayer);
					return -1;
				}
			}
			
			memset(&stPwolrdSelect, 0, sizeof(stPwolrdSelect));
			z_player_enter_pworld(pstEnv, pstPlayer,pstReq->PworldID, &stPwolrdSelect, 0);
		}
		
	}
	else
	{
		if (pstZonePworld->stPworldDef.AutoPworldNum > 1)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PWORLD106, 
						pstZonePworld->stPworldDef.Name, pstZonePworld->stPworldDef.AutoPworldNum);
			return -1;
		}

		if (autopworld_check_one( pstEnv, pstPlayer, pstZonePworld, NULL,1) < 0)
		{
			return -1;
		}
		
		if (pstPlayer->stOnline.State & CS_STAT_FIGHT)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PWORLD105);
			return -1;
		}

		memset(&stPwolrdSelect, 0, sizeof(stPwolrdSelect));
		if (z_player_enter_pworld(pstEnv, pstPlayer,pstReq->PworldID, &stPwolrdSelect, 0) < 0)
		{
			return -1;
		}
		
	}
	return 0;
}

/*int autopworld_clt_enter(ZONESVRENV* pstEnv, Player *pstPlayer, AUTOPWORLDENTERREQ *pstReq)
{
	ZoneTeam *pstTeam;
	ZONEPWORLD *pstZonePworld;
	ZONEIDX stIdx;
	PWORLDSELECT stPwolrdSelect;
	int iPinstID = 0;
	ZONEPWORLDINST *pstPInst;
	Player *pstCap;
	char szName[32];
	int iCreate = 0;

	stIdx.iID = pstReq->PworldID;
	stIdx.iPos = -1;

	pstZonePworld =  z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (!pstZonePworld)
	{
		return -1;
	}

	if (z_player_goto_check(pstEnv, pstPlayer, 0, NULL) < 0)
	{
		return -1;	
	}
	
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if (pstTeam)
	{	
		pstPInst = z_find_pworld_inst(pstEnv->pstMapObj, pstTeam->iAutoPworldID);
		if(pstPInst  && pstPInst->stPworldIdx.iID != pstReq->PworldID)
		{
			z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "传送失败!", szName);
			return -1;
		}

		pstCap = player_team_get_captain_player(pstEnv,pstTeam);
		if (!pstCap)
			return -1;

		if (!pstPInst)
		{
			iCreate = 1;
		}

		if (autopworld_check_one( pstEnv, pstPlayer, pstZonePworld, NULL,iCreate) < 0)
		{
			return -1;
		}
		
		// 创建副本要消耗货币，要检查队员进入次数
		if (iCreate )
		{
			if (pstCap == pstPlayer)
			{
				if (0 > package_dec_money_test(pstEnv, pstPlayer, pstZonePworld->stPworldDef.AutoPworldUseType, 
						pstZonePworld->stPworldDef.AutoPworldUseVal))
				{
					money_type_to_string(pstZonePworld->stPworldDef.AutoPworldUseType, szName, sizeof(szName));
					z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "%s不足,开启传送失败!", szName);
					return -1;
				}

				if (pstTeam->stTeamInfo.MemberNum < pstZonePworld->stPworldDef.AutoPworldNum)
				{
					z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "进入%s 队伍人数不能少于%d人!", 
								pstZonePworld->stPworldDef.Name, pstZonePworld->stPworldDef.AutoPworldNum);
					return -1;
				}

				if (diff_pworld_team_check_tips( pstEnv,pstTeam, pstPlayer,pstZonePworld) < 0)
				{
					return -1;
				}
			}
			else
			{
				z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "没有自动传送的副本!");
				return -1;
			}
		}

		memset(&stPwolrdSelect, 0, sizeof(stPwolrdSelect));
		if ((iPinstID = z_player_enter_pworld(pstEnv, pstPlayer,pstReq->PworldID, &stPwolrdSelect, 0)) < 0)
		{
			return -1;
		}

		// 不是队长就不转发了
		if (pstCap != pstPlayer)
		{
			return 0;
		}

		// 创建副本
		if(iCreate)
		{
			package_dec_money(pstEnv, pstPlayer, pstZonePworld->stPworldDef.AutoPworldUseType, 
								pstZonePworld->stPworldDef.AutoPworldUseVal);

			pstTeam->iAutoPworldID = iPinstID;
			autopworld_invite_enter_trans(pstEnv, pstPlayer, pstTeam, &pstZonePworld->stPworldDef);
		}
	}

	else
	{
		if (pstZonePworld->stPworldDef.AutoPworldNum > 0)
		{
			z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "进入%s 队伍人数不能少于%d人!", 
						pstZonePworld->stPworldDef.Name, pstZonePworld->stPworldDef.AutoPworldNum);
			return -1;
		}
		
		if (autopworld_check_one( pstEnv, pstPlayer, pstZonePworld, NULL,1) < 0)
		{
			return -1;
		}
		
		if (0 > package_dec_money_test(pstEnv, pstPlayer, pstZonePworld->stPworldDef.AutoPworldUseType, 
					pstZonePworld->stPworldDef.AutoPworldUseVal))
		{
			money_type_to_string(pstZonePworld->stPworldDef.AutoPworldUseType, szName, sizeof(szName));
			z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "%s不足,开启传送失败", szName);
			return -1;
		}

		memset(&stPwolrdSelect, 0, sizeof(stPwolrdSelect));
		z_player_enter_pworld(pstEnv, pstPlayer,pstReq->PworldID, &stPwolrdSelect, 0);
		package_dec_money(pstEnv, pstPlayer, pstZonePworld->stPworldDef.AutoPworldUseType, 
							pstZonePworld->stPworldDef.AutoPworldUseVal);

	}

	return 0;

}*/


int autopworld_clt_ui(ZONESVRENV* pstEnv, Player *pstPlayer, AUTOPWORLDUIREQ *pstReq)
{
	CSPKG stPkg;
	AUTOPWORLDSVR *pstSvr= &stPkg.Body.AutoPworldSvr;
	ZoneTeam *pstTeam;
	ZONEPWORLD *pstZonePworld;
	ZONEPWORLDINST *pstPInst;

	pstSvr->Cmd = AUTOPWORLD_SVR_OP_UI;

	memset(&pstSvr->Data.UI, 0, sizeof(pstSvr->Data.UI));
	pstSvr->Data.UI.EffectFlag = pstReq->Invalid;
	
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if (pstTeam)
	{
		pstPInst = z_find_pworld_inst(pstEnv->pstMapObj, pstTeam->iAutoPworldInstID);
		if (pstPInst)
		{
			pstZonePworld =  z_find_pworld(pstEnv->pstMapObj, &pstPInst->stPworldIdx);
			if (!pstZonePworld)
			{
				return -1;
			}

			pstSvr->Data.UI.PworldID = pstPInst->stPworldIdx.iID;
			pstSvr->Data.UI.Level = team_pworld_level_get(pstEnv, pstTeam, pstZonePworld);

			if (pstPInst->iPlayer <=0)
			{
				pstSvr->Data.UI.PworldID = 0;	
			}
		}
	}

	Z_CSHEAD_INIT(&stPkg.Head, AUTOPWORLD_SVR);

	if (pstTeam && pstReq->Invalid)
	{
		player_team_broadcast(pstEnv, pstTeam,&stPkg);
	}
	else
	{
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);	
	}
	

	return 0;

}

int  autopworld_effect(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	AUTOPWORLDUIREQ stReq;
	stReq.Invalid = 0;

	autopworld_clt_ui(pstEnv,pstPlayer, &stReq);
	return 0;
}

int autopworld_one_set_ready(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneTeam *pstTeam)
{
	int i;
	Player *pstTmp;

	for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		pstTmp = z_name_player(pstEnv->pstAppCtx,pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
		if (!pstTmp || PLAYER_STATUS_ROLE_LOGIN != pstTmp->eStatus)
		{
			return -1;
		}

		if (pstTmp == pstPlayer)
		{
			pstTeam->stTeamInfo.TeamMember[i].EnterPworldFlag = 1;	
			return 0;
		}
	}

	return -1;
}

int autopworld_check_is_ready(ZONESVRENV* pstEnv, ZoneTeam *pstTeam)
{
	int i;
//	Player *pstTmp;

	for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		if (!pstTeam->stTeamInfo.TeamMember[i].EnterPworldFlag)
		{		
			return -1;
		}
	}

	return 0;
}

int autopworld_clt_agree(ZONESVRENV* pstEnv, Player *pstPlayer,  AUTOPWORLDAGREE *pstAgree)
{
	ZoneTeam *pstTeam;
	ZONEPWORLD *pstZonePworld;
	ZONEIDX stIdx;
	int i;
	Player *pstCap;
	Player *pstTmp;
	int iCreate = 0;
	PWORLDSELECT stPwolrdSelect;
	Player *ppstPlayer[MAX_TEAM_MEMBER-1];
	int iNum = 0;
	int iInstID;
	char szName[32];
		
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if (!pstTeam)
	{
		return -1;
	}

	if (!pstTeam->iAutoPworldID)
	{
		return -1;
	}

	pstCap = player_team_get_captain_player(pstEnv,pstTeam);
	if (!pstCap)
		return -1;

	if (pstCap == pstPlayer)
		iCreate = 1;

	stIdx.iID = pstTeam->iAutoPworldID;
	stIdx.iPos = -1;
	pstZonePworld =  z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (!pstZonePworld)
	{
		return -1;
	}

	strncpy(szName, pstZonePworld->stPworldDef.Name, sizeof(szName));
	/*if (poly_is_mzt_pworld(pstTeam->iAutoPworldID))
	{
		ZONEPWORLD *pstTmp;
		ZONEIDX stTmpIdx;
		int iTmpPworldID = ploy_mzt_get_pworldid(pstEnv, pstTeam);

		stTmpIdx.iID = iTmpPworldID;
		stTmpIdx.iPos = -1;
		pstTmp = z_find_pworld(pstEnv->pstMapObj, &stTmpIdx);
		if (pstTmp)
		{
			strncpy(szName, pstTmp->stPworldDef.Name, sizeof(szName));
		}
	}*/
	
	if (pstAgree->Agree)
	{
		if (!iCreate)
		{
			if (autopworld_one_set_ready(pstEnv, pstPlayer,pstTeam) < 0)
			{
				goto final;
			}

			// 广播准备情况
			autopworld_invite_enter(pstEnv, NULL,pstZonePworld->stPworldDef.PworldID, 0, pstTeam,0);
			
			z_team_sys_strf(pstEnv, pstTeam, SYS_TEAM, ZONEERR_TEAM302, pstPlayer->stRoleData.RoleName, szName);
			return 0;
		}
	}
	else
	{
		z_team_sys_strf(pstEnv, pstTeam, SYS_TEAM, ZONEERR_TEAM303, pstPlayer->stRoleData.RoleName, szName);

		// 通知队伍取消面板
		goto final;
	}

	
	if (iCreate)
	{
		if (autopworld_check_is_ready(pstEnv, pstTeam) < 0)
		{
			return 0;
		}

		memset(&stPwolrdSelect, 0, sizeof(stPwolrdSelect));	

		// 先检查每个人是不是真的能进
		for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
		{
			pstTmp = z_name_player(pstEnv->pstAppCtx,pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
			if (!pstTmp || PLAYER_STATUS_ROLE_LOGIN != pstTmp->eStatus)
			{
				z_team_sys_strf(pstEnv, pstTeam, SYS_TEAM, ZONEERR_TEAM304, 
								pstTeam->stTeamInfo.TeamMember[i].MemberName);
				goto final;
			}

			if (z_player_goto_check(pstEnv, pstTmp, 0, NULL) < 0)
			{
				z_team_sys_strf(pstEnv, pstTeam, SYS_TEAM, ZONEERR_TEAM305, 
								pstTeam->stTeamInfo.TeamMember[i].MemberName);
				goto final;
			}

			if (player_is_in_logout_to_revivepoint_map( pstEnv,  pstTmp))
			{
				z_team_sys_strf(pstEnv, pstTeam, SYS_TEAM, ZONEERR_DIANFENG32, 
								pstTeam->stTeamInfo.TeamMember[i].MemberName);
				return -1;
			}

			if(0 > z_check_single_pworld(pstEnv, pstTmp, &pstZonePworld->stPworldDef, 0, NULL))
			{
				goto final;
			}

			if (pstCap != pstTmp)
			{
				ppstPlayer[iNum] = pstTmp;	
				iNum++;
			}
		}

		if (iNum <= 0)
		{
			goto final;
		}

		// 队长创建副本
		if ((iInstID = z_player_enter_pworld(pstEnv, pstCap,pstZonePworld->stPworldDef.PworldID, &stPwolrdSelect, 0)) < 0)
		{
			goto final;
		}

		pstTeam->iAutoPworldInstID = iInstID;

		// 队员最后进
		for (i=0; i<iNum; i++)
		{
			z_player_enter_pworld(pstEnv, ppstPlayer[i],pstZonePworld->stPworldDef.PworldID, &stPwolrdSelect, 0);	
		}

		pstTeam->iAutoPworldID = 0;
		pstTeam->iAutoPworldStart = 0;
	}

	autopworld_invite_enter(pstEnv, NULL,pstZonePworld->stPworldDef.PworldID, 1, pstTeam,0);
	return 0;

final:
	autopworld_invite_enter(pstEnv, NULL,pstZonePworld->stPworldDef.PworldID, 1, pstTeam,1);
	return 0;
}


int autopworld_team_tick(ZONESVRENV* pstEnv, ZoneTeam *pstTeam)
{

	if (pstTeam->iAutoPworldStart <= 0 ||
		pstTeam->iAutoPworldID <= 0)
	{
		return 0;
	}

	if (pstEnv->pstAppCtx->stCurr.tv_sec > pstTeam->iAutoPworldStart+60)
	{
		autopworld_invite_enter(pstEnv, NULL, pstTeam->iAutoPworldID, 1, pstTeam, 1);	
	}
	return 0;
}

int autopworld_clt_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	AUTOPWORLDCLT *pstClt = &pstCsPkg->Body.AutoPworldClt;

	if(INST_MODE_ZONE_SVR != pstEnv->iInstMode )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR1);
		return -1;
	}

	if (pstClt->Cmd == AUTOPWORLD_CLT_OP_ENTER)
	{
		autopworld_clt_enter(pstEnv, pstPlayer, &pstClt->Data.Enter, 0);
	}
	else if (pstClt->Cmd == AUTOPWORLD_CLT_OP_UI)
	{
		autopworld_clt_ui(pstEnv, pstPlayer, &pstClt->Data.UI);
	}
	else if (pstClt->Cmd == AUTOPWORLD_CLT_OP_AGREE)
	{
		autopworld_clt_agree(pstEnv, pstPlayer,   &pstClt->Data.Agree);
	}
	else
	{
		return 0;
	}

	return 0;
}




























