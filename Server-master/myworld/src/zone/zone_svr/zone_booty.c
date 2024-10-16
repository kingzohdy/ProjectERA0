/*
**  @file $RCSfile: zone_booty.c,v $
**  general description of this module
**  $Id: zone_booty.c,v 1.83 2014/06/25 16:58:27 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2014/06/25 16:58:27 $
**  @version $Revision: 1.83 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_booty.h"
#include "zone_oplog.h"
#include "zone_package.h"
#include "zone_itemlimit.h"
#include "zone_npc.h"
#include "zone_player.h"
#include "zone_clt.h"
#include "zone_map.h"
#include "zone_move.h"
#include "zone_team.h"
#include "zone_status.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "role_db_meta.h"
#include "zone_respact.h"
#include "zone_pet.h"
#include "zone_attr.h"
#include "zone_fairy.h"
#include "zone_combat.h"
#include "zone_city.h"
#include "zone_misc.h"
#include "zone_ronglu.h"
#include "zone_err.h"
#include "zone_losttotem.h"
#include "zone_unreal.h"
#include "zone_ploy.h"

int iStatExp[MAX_TEAM_MEMBER+1] = { 0, 0, 5, 10, 15, 20, 25};

int is_share_exp_pworld(ZONESVRENV* pstEnv, Monster *pstMon);

MONDYNEXPLIMITDEF *z_find_mondynexplimit_def(ZONESVRENV *pstEnv, unsigned char ucLevel, int *pIidx)
{
	return (MONDYNEXPLIMITDEF *)z_find_def(pstEnv->pstZoneObj->sMonDynExpLimitDef, ucLevel, MONDYNEXPLIMIT_DATA, 
									pstEnv->pstZoneObj->iMonDynExpLimitDef, pIidx);
}

MONBOOTYDEF *z_find_mon_booty_def(ZONESVRENV *pstEnv, int iBootyID, int *pIidx)
{
	return (MONBOOTYDEF *)z_find_def(pstEnv->pstZoneObj->sMonBootyDef, iBootyID, MON_BOOTY_DEF_DATA, 
									pstEnv->pstZoneObj->iMonBootyDef, pIidx);
}

MONBOOTYGROUPDEF *z_find_mon_bootygroup_def(ZONESVRENV *pstEnv, int iGroupID, int *pIidx)
{
	return (MONBOOTYGROUPDEF *)z_find_def(pstEnv->pstZoneObj->sMonBootyGroupDef, iGroupID, MON_BOOTY_GROUP_DEF_DATA, 
									pstEnv->pstZoneObj->iMonBootyGroupDef, pIidx);
}

int is_mon_booty_owner(Monster *pstMon, Player *pstPlayer)
{
	if(pstMon->iFirstAtkerID <= 0)
	{
		return 0;
	}

	switch(pstMon->cFirstAtkerType)
	{
		case MON_BOOTY_OWNER_PLAYER:
			if(pstPlayer->iMemID == pstMon->iFirstAtkerID && 
				pstPlayer->stRoleData.RoleID == pstMon->ullFirstAtkerWid)
			{
				return 1;
			}
			break;
		case MON_BOOTY_OWNER_TEAM:
			{
				ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;

				if (pstMisc->TeamIdx == pstMon->iFirstAtkerID && pstMisc->TeamID ==  pstMon->ullFirstAtkerWid)
				{
					return 1;
				}
			}
			break;
		case MON_BOOTY_OWNER_CLAN:
			{
				CLANINFO *pstClanInfo = &pstPlayer->stRoleData.MiscInfo.ClanInfo;
				
				if(pstClanInfo->ClanId.GID > 0 &&
					pstMon->ullFirstAtkerWid == pstClanInfo->ClanId.GID)
				{
					return 1;
				}
			}
			break;
		default:
			break;
	}

	return 0;
}

int is_team_valid_mon_booty(ZONESVRENV* pstEnv , Monster *pstMon)
{
	ZONEIDX stIdx;
	ZONEPWORLD *pstPworld;
		
	if(pstMon->stMap.iID < PWORLD_MAPID_BASE)
	{	
		return 1;
	}

	stIdx.iID = GET_PWORLDID(pstMon->stMap.iID);
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (pstPworld  == NULL || !(pstPworld->stPworldDef.CtrlFlag & PWORLD_NO_TEAM))
	{
		return 1;
	}

	return 0;
}

int is_booty_grid_owner(Player *pstPlayer, DropItem *pstDropItem)
{
	if(pstDropItem->stObj.stBootyGrid.ullRoleID > 0)
	{	
		if(pstDropItem->stObj.stBootyGrid.ullRoleID != pstPlayer->stRoleData.RoleID)
		{
			return 0;
		}
	}
	else if(pstDropItem->stObj.stBootyGrid.ullTeamID > 0)
	{
		if(pstDropItem->stObj.stBootyGrid.ullTeamID > 0 &&
			pstDropItem->stObj.stBootyGrid.ullTeamID != pstPlayer->stRoleData.MiscInfo.TeamID)
		{
			return 0;
		}
	}
	else 
	{
		CLANINFO *pstClanInfo = &pstPlayer->stRoleData.MiscInfo.ClanInfo;
		
		if(pstDropItem->stObj.stBootyGrid.ullClanGID > 0 &&
			pstDropItem->stObj.stBootyGrid.ullClanGID != pstClanInfo->ClanId.GID)
		{
			return 0;
		}
	}
	
	return 1;
}

int mon_booty_first_action(ZONESVRENV* pstEnv, Monster *pstMon)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	if(is_camp_mon(pstMon))
	{
		return 0;
	}
	
	pstActionRes->ID = pstMon->iID;
	pstActionRes->ActionID = ACTION_MON_BOOTY_FIRST;
	pstActionRes->Data.MonBootyFirst.MemID = pstMon->iFirstAtkerID;
	pstActionRes->Data.MonBootyFirst.BootyOwnerType = pstMon->cFirstAtkerType;
	pstActionRes->Data.MonBootyFirst.BootyOwnerWid = pstMon->ullFirstAtkerWid;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_monview(pstEnv->pstAppCtx, pstEnv, pstMon, &stPkg, 0);
	
	return 0;
}

int mon_booty_owner_update(ZONESVRENV* pstEnv, Monster *pstMon, Player *pstPlayer)
{
	MONSTERDEF *pstMonDef;
	ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if(NULL == pstMonDef)
	{
		return -1;
	}
	
	if(pstMonDef->ClanBooty)
	{
		pstMon->cFirstAtkerType = MON_BOOTY_OWNER_CLAN;
		pstMon->iFirstAtkerID = pstMon->nOwnerFastID;
		pstMon->ullFirstAtkerWid = pstMon->ullBootyClanGID;
		pstMon->iFirstAtkerMemID = 0;
	}
	else if(pstTeam && is_valid_booty_team(pstEnv, pstPlayer))
	{
		pstMon->cFirstAtkerType = MON_BOOTY_OWNER_TEAM;
		pstMon->iFirstAtkerID = pstTeam->iTeamIdx;
		pstMon->ullFirstAtkerWid = pstTeam->stTeamInfo.TeamID;
		pstMon->iFirstAtkerMemID = pstPlayer->iMemID;
	}
	else
	{
		pstMon->cFirstAtkerType = MON_BOOTY_OWNER_PLAYER;
		pstMon->iFirstAtkerID = pstPlayer->iMemID;
		pstMon->ullFirstAtkerWid = pstPlayer->stRoleData.RoleID;
		pstMon->iFirstAtkerMemID = pstPlayer->iMemID;
	}

	pstMon->tLastAtkTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	mon_booty_first_action(pstEnv, pstMon);
	
	return 0;
}

int mon_booty_first_atker(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, Player *pstAtker)
{
	time_t tOldLastAtk;

	tOldLastAtk = pstMon->tLastAtkTime;

	if (0 != pstMon->szOwner[0] && !is_convoy_car(pstMon))
	{
		return 0;
	}

	if(is_city_build_mon(pstMon))
	{
		return 0;
	}

	if (0 == pstMon->iFirstAtkerID)
	{
		return mon_booty_owner_update(pstEnv, pstMon, pstAtker);
	}

	if (tOldLastAtk + FIRST_ATKER_TIME < pstAppCtx->stCurr.tv_sec)
	{
		return mon_booty_owner_update(pstEnv, pstMon, pstAtker);
	}

	if(pstMon->cFirstAtkerType == MON_BOOTY_OWNER_TEAM)
	{
		ZoneTeam *pstTeam =  z_idx_team(pstEnv, pstMon->iFirstAtkerID, pstMon->ullFirstAtkerWid);
		if(NULL == pstTeam)
		{
			return mon_booty_owner_update(pstEnv, pstMon, pstAtker);
		}
	}	

	if(is_mon_booty_owner(pstMon, pstAtker))
	{
		return pstMon->tLastAtkTime = pstAppCtx->stCurr.tv_sec;
	}

	return 0;
}

/*
int mon_press_min_no_owner_idx(ZONESVRENV * pstEnv, Monster *pstMon)
{
	int i;
	Player * pstPlayer;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;
	int iMinPress = pstTargetList->astPressEntrys[0].iPress;
	int iIdx = 0;
	int iNoOwnerIdx = -1;

	if (0 >= pstTargetList->PressNum)
	{
		return -1;
	}

	for (i=1; i<pstTargetList->PressNum; i++)
	{
		if (pstTargetList->astPressEntrys[i].iPress < iMinPress)
		{
			iMinPress = pstTargetList->astPressEntrys[i].iPress;
			iIdx = i;
		}

		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstTargetList->astPressEntrys[i].iTargetID);
		if( NULL == pstPlayer || !is_mon_booty_owner(pstMon, pstPlayer))
		{
			if(iNoOwnerIdx < 0 ||
				pstTargetList->astPressEntrys[i].iPress < pstTargetList->astPressEntrys[iNoOwnerIdx].iPress)
			{
				iNoOwnerIdx = i;
			}
		}
	}

	if(iNoOwnerIdx >= 0)
		return iNoOwnerIdx;

	return iIdx;
}

int mon_press_insert(ZONESVRENV * pstEnv, Monster *pstMon, int iTargetID, int iPress)
{
	int iIdx;
	Player * pstPlayer;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;

	if(AI_STAT_HOME == pstMon->bAiStat)
	{
		return -1;
	}
	
	if (0 <= mon_press_find(pstMon, iTargetID))
	{
		return -1;
	}

	if (pstTargetList->PressNum >= MAX_PRESS_ENTRY)
	{
		Player * pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, iTargetID);

		if(pstPlayer && is_mon_booty_owner(pstMon, pstPlayer))
		{
			iIdx = mon_press_min_idx(pstMon);
		}
		else
		{
			iIdx = mon_press_min_no_owner_idx(pstEnv, pstMon);	
		}

		pstTargetList->astPressEntrys[iIdx].iTargetID = iTargetID;
		pstTargetList->astPressEntrys[iIdx].iPress = iPress;
	}
	else
	{
		iIdx = pstTargetList->PressNum;
		pstTargetList->astPressEntrys[iIdx].iTargetID = iTargetID;
		pstTargetList->astPressEntrys[iIdx].iPress = iPress;
		pstTargetList->PressNum++;
	}

	return iIdx;
}

int mon_press_max(TAPPCTX *pstAppCtx, ZONESVRENV * pstEnv, Monster *pstMon, int *piIdx)
{
	int i;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;
	int iMaxPress = pstTargetList->astPressEntrys[0].iPress;
	int iIdx = 0;
	int iOwnerIdx = -1;
	Player *pstPlayer;

	if(piIdx)
	{
		*piIdx = -1;
	}

	if(pstTargetList->iMarkedID > 0) // 被标记的目标第一优先
	{
		if (pstTargetList->tMarkedEnd >= pstAppCtx->stCurr.tv_sec)
		{
			return pstTargetList->iMarkedID;
		}
		else
		{
			pstTargetList->iMarkedID = 0;
		}
	}
	else if(pstTargetList->iDefyID > 0) // 被嘲讽的目标第二优先
	{
		if (pstTargetList->tDefyEnd >= pstAppCtx->stCurr.tv_sec)
		{
			return pstTargetList->iDefyID;
		}
		else
		{
			pstTargetList->iDefyID = 0;
		}
	}

	if (0 >= pstTargetList->PressNum)
	{
		return -1;
	}

	for (i=1; i<pstTargetList->PressNum; i++) // 压力值最大的目标第三优先
	{
		if (pstTargetList->astPressEntrys[i].iPress > iMaxPress)
		{
			iMaxPress = pstTargetList->astPressEntrys[i].iPress;
			iIdx = i;
		}

		//压力表中战利品归属者优先打
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstTargetList->astPressEntrys[i].iTargetID);
		if(pstPlayer && is_mon_booty_owner(pstMon, pstPlayer))
		{
			if(iOwnerIdx < 0 ||
				pstTargetList->astPressEntrys[i].iPress > pstTargetList->astPressEntrys[iOwnerIdx].iPress)
			{
				iOwnerIdx = i;
			}
		}
	}

	if(iOwnerIdx >= 0)
		iIdx = iOwnerIdx;

	if (piIdx)
	{
		*piIdx = iIdx;
	}

	return pstTargetList->astPressEntrys[iIdx].iTargetID;
}*/

// 计算组队契约的经验加成
void mon_booty_qiyue_exp_mul( ZONESVRENV * pstEnv, Player * pstPlayer, ZoneTeam * pstZoneTeam, MonExpMul *pstExpMul)
{
	QIYUERESULT stQiYueResult;

	if( !pstExpMul )
	{
		return;
	}

	pstExpMul->iQiYueExpPetMul = 0;
	pstExpMul->iQiYueExpPlayerMul = 0;
	
	memset( &stQiYueResult, 0, sizeof(stQiYueResult ) );

	// 获取经验加成的契约效果	
	stQiYueResult.QiYueResultID = QIYUE_RESULT_ID_EXP;
	player_team_checked_qiyue_result( pstEnv, pstPlayer, pstZoneTeam, &stQiYueResult );

	pstExpMul->iQiYueExpPlayerMul = stQiYueResult.ResultVal1;
}

int mon_dyn_exp_mul_get(ZONESVRENV* pstEnv, Player *pstPlayer, Monster* pstMon, MONSTERDEF *pstMonDef)
{
	MONDYNATTRDEF *pstMonDynAttr = NULL;
	ZONEPWORLDINST *pstPworldInst = NULL;
	ZONEMAPINST *pstMapInst;
	ZONEPWORLD* pstPworld = NULL;
	int iMul = 0;

	if(pstMonDef->DynAttr == 0)
	{
		return -1;
	}
	
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if (!pstMapInst)
	{
		return -1;
	}

	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if(!pstPworldInst || pstPworldInst->DynType == 0)
	{
		return -1;
	}

	pstPworld	= z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if( !pstPworld )
	{
		return -1;
	}
		
	pstMonDynAttr = z_find_mondyn(pstEnv, pstPlayer->stRoleData.Level);
	if (pstMonDynAttr &&  pstPworldInst->stPworldselect.PworldGrade < MAX_DYN_GRADE )
	{
		PWORLDDYNATTR *pstDyn = &pstPworld->stPworldDef.DynAttr;
		
		iMul =pstMonDynAttr->DynExpMul[pstPworldInst->stPworldselect.PworldGrade]*
			(1 + pstDyn->DynExpMul[pstPworldInst->stPworldselect.PworldGrade]/100.0);
	}

	return iMul;
}

void mon_booty_player_mul(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneTeam *pstTeam, Monster *pstMon, MONSTERDEF *pstMonDef, MonExpMul *pstExpMul,
								int *piMoneyMul, int *piItemRateMul)
{
	int iPlayerLevel = pstPlayer->stRoleData.Level;
	MONCOLORLEVEL *pstColorLevel;
	STATUSRESULT stStatusResult;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	MONSTERCOLORDEF *pstMonColorDef = NULL;
	ZONEPWORLDINST *pstZonePworldInst = NULL;

	pstExpMul->iNormalExpMul = 0;
	pstExpMul->iResultExpMul = 0;
	pstExpMul->fClanSkillExpMul = 0.0;

	//经验状态
	z_get_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_PLAYER_ADDEXP, &stStatusResult, 0);
	pstExpMul->iResultExpMul += stStatusResult.ResultVal1;
	z_get_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_QIXINXIELI, &stStatusResult, 0);

	//齐心协力在幻境不起效果
	pstZonePworldInst = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);
	if (pstZonePworldInst && pstZonePworldInst->stPworldIdx.iID == UNREAL_PWORLD_ID)
	{
		pstExpMul->iResultExpMul += 0;
	}
	else if(pstZonePworldInst && poly_is_mzt_pworld(pstZonePworldInst->stPworldIdx.iID))
	{
		//齐心协力在迷之塔不起效果
		pstExpMul->iResultExpMul += 0;
	}
	else
	{
		pstExpMul->iResultExpMul += stStatusResult.ResultVal1;
	}

	pstExpMul->iResultExpMul += pstPlayer->stOnline.stVipBarInfo.iVipBarExpMul;

	/*
	if (pstPlayer->stOnline.stFitOneline.unExpRate)
	{
		pstExpMul->iNormalExpMul += pstPlayer->stOnline.stFitOneline.unExpRate;
	}
	*/

	if (pstDyn->stHealChg.nArmExpMul)
	{
		pstExpMul->iResultExpMul += pstDyn->stHealChg.nArmExpMul;
	}

	if (pstDyn->iClanSkillExpMul)
	{
		//pstExpMul->iResultExpMul += pstDyn->iClanSkillExpMul;
		pstExpMul->fClanSkillExpMul = pstDyn->iClanSkillExpMul/100.0;
	}

	if (pstEnv->iSpanStrongExpMul && pstPlayer->stRoleData.Level >= 60)
	{
		pstExpMul->iResultExpMul  += 	pstEnv->iSpanStrongExpMul;
	}

	pstExpMul->iPetExpMul = pstDyn->stHealChg.unPetExpMul;

	//组队契约修正 
	if (pstTeam)
	{
		mon_booty_qiyue_exp_mul(pstEnv, pstPlayer, pstTeam, pstExpMul);
	}
	
	// 动态怪修正 &&  红白名
	if (pstMonDef->DynAttr) 
	{
		pstColorLevel = &pstMon->stMonDynAttr.ColorLevel;
		//pstExpMul->iMonDynExpMul = pstMon->stMonDynAttr.ExpMul;

		pstExpMul->iMonDynExpMul = mon_dyn_exp_mul_get(pstEnv, pstPlayer, pstMon, pstMonDef);
	}
	else
	{
		pstColorLevel = &pstMonDef->ColorLevel;
		pstExpMul->iMonDynExpMul = 0;
	}

	if (0 == pstColorLevel->BlackLevel)
	{
		return;
	}

	if (pstEnv->pstZoneObj->iMonsterColorDef)
	{
		pstMonColorDef = (MONSTERCOLORDEF *)pstEnv->pstZoneObj->sMonsterColorDef[0];

		if (iPlayerLevel < pstColorLevel->BlackLevel)
		{
			pstExpMul->iColorExpMul = (pstExpMul->iColorExpMul) * (pstMonColorDef->RedExpMul/100.0);
			*piMoneyMul = (*piMoneyMul) * (pstMonColorDef->RedMoneyMul/100.0);
			*piItemRateMul = (*piItemRateMul) * (pstMonColorDef->RedItemRate/100.0);
		}
		else if (iPlayerLevel < pstColorLevel->RedLevel)
		{
			pstExpMul->iColorExpMul = (pstExpMul->iColorExpMul) * (pstMonColorDef->OrangeExpMul/100.0);
			*piMoneyMul = (*piMoneyMul) * (pstMonColorDef->OrangeMoneyMul/100.0);
			*piItemRateMul = (*piItemRateMul) * (pstMonColorDef->OrangeItemRate/100.0);
		}
		else if (iPlayerLevel < pstColorLevel->WhiteLevel)
		{
			pstExpMul->iColorExpMul = (pstExpMul->iColorExpMul) * (pstMonColorDef->YellowExpMul/100.0);
			*piMoneyMul = (*piMoneyMul) * (pstMonColorDef->YellowMoneyMul/100.0);
			*piItemRateMul = (*piItemRateMul) * (pstMonColorDef->YellowItemRate/100.0);
		}
		else if (iPlayerLevel < pstColorLevel->GreenLevel)
		{
			pstExpMul->iColorExpMul = (pstExpMul->iColorExpMul) * (pstMonColorDef->WhiteExpMul/100.0);
			*piMoneyMul = (*piMoneyMul) * (pstMonColorDef->WhiteMoneyMul/100.0);
			*piItemRateMul = (*piItemRateMul) * (pstMonColorDef->WhiteItemRate/100.0);
		}
		else
		{
			pstExpMul->iColorExpMul = (pstExpMul->iColorExpMul) * (pstMonColorDef->GreenExpMul/100.0);
			*piMoneyMul = (*piMoneyMul) * (pstMonColorDef->GreenMoneyMul/100.0);
			*piItemRateMul = (*piItemRateMul) * (pstMonColorDef->GreenItemRate/100.0);
		}
	}
	
	return ;
}

void mon_booty_dynexp_player_limit(ZONESVRENV* pstEnv, MONSTERDEF *pstMonDef, Player *pstPlayer, unsigned int *puiExp)
{
	MONDYNEXPLIMITDEF *pstMonDynExpLimit;
	unsigned int uiExpLimit = 0;
	
	pstMonDynExpLimit = z_find_mondynexplimit_def(pstEnv, pstPlayer->stRoleData.Level, NULL);
	if (pstMonDynExpLimit)
	{
		switch(pstMonDef->SubType)
		{
			case MON_SUB_TYPE_MINI:
				
				uiExpLimit = pstMonDynExpLimit->MonMiniExpLimit;
				break;
			case MON_SUB_TYPE_WEAK:
				
				uiExpLimit = pstMonDynExpLimit->MonWeakExpLimit;
				break;
			case MON_SUB_TYPE_SOFT:

				uiExpLimit = pstMonDynExpLimit->MonSoftExpLimit;
				break;
			case MON_SUB_TYPE_NORM:

				uiExpLimit = pstMonDynExpLimit->MonNormalExpLimit;
				break;
			case MON_SUB_TYPE_HARD:

				uiExpLimit = pstMonDynExpLimit->MonHardExpLimit;
				break;
			case MON_SUB_TYPE_XI:

				uiExpLimit = pstMonDynExpLimit->MonXiExpLimit;
				break;
			case MON_SUB_TYPE_JIN:

				uiExpLimit = pstMonDynExpLimit->MonJinExpLimit;
				break;
			case MON_SUB_TYPE_BOSS:

				uiExpLimit = pstMonDynExpLimit->MonBossExpLimit;
				break;
			case MON_SUB_TYPE_JI:

				uiExpLimit = pstMonDynExpLimit->MonJiExpLimit;
				break;
			default:
				break;
		}
		if (*puiExp > uiExpLimit)
		{
			*puiExp = uiExpLimit;
		}
	}
	
	return ;
}

int mon_booty_assign_egg(ZONESVRENV* pstEnv, MONSTERDEF *pstMonDef, Monster *pstMon, Player *pstPlayer)
{
	ZONEIDX stMapIdx;
	Monster stMonster;
	MapIndex *pstMapIdx;
	ZONEMAPINST *pstMapInst = NULL;
	MONSTERDEF *pstCollectMonDef;
	Monster *pstFind;
	RESPOS stPos;
	
	stMapIdx.iID = pstPlayer->stRoleData.Map;
	stMapIdx.iPos = pstPlayer->stOnline.iMapPos;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stMapIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = pstMonDef->Booty.BootyPetEgg;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos=	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.tRefresh	=	pstEnv->pstAppCtx->stCurr.tv_sec;
	if (pstPlayer)
	{
		ZoneTeam *pstZoneTeam;
		
		stMonster.iOwnerID = pstPlayer->iMemID;

		pstZoneTeam = player_team_get(pstEnv, pstPlayer);
		if(pstZoneTeam)
		{
			stMonster.stTeamInfo.iTeamIdx = pstZoneTeam->iTeamIdx;
			stMonster.stTeamInfo.ullTeamID = pstZoneTeam->stTeamInfo.TeamID;
		}
	}

	pstCollectMonDef = z_get_mon_def(pstEnv, &stMonster);
	if (NULL == pstCollectMonDef)
	{
		return -1;
	}

	stMonster.bDirection = RAND1(180);
	z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstMon->stCurrPos, &pstMon->stCurrPos, 
						200, 50, &stPos);
	stMonster.stInitPos = stPos;

	pstFind =	z_new_monster(pstEnv->pstAppCtx, pstEnv, &stMonster, pstCollectMonDef);
	if( !pstFind )
		return -1;

	pstFind->cFirstBorn = 1;
	pstFind->tLifeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + 180;
	if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstFind,NULL))
	{
		tmempool_free(pstEnv->pstMonsterPool, pstFind->iMIndex);
		return -1;
	}
	
	pstFind->cFirstBorn = 0;	
	mon_life_action(pstEnv, pstFind);
	
	return 0;
}

int mon_booty_item_valid_player(ZONESVRENV* pstEnv, MONBOOTYITEM *pstBootyItem, Player *pstPlayer, int iRateMul)
{
	if(pstBootyItem->TeamBootyType == MON_BOOTY_TEAM_TYPE_SINGLE)
	{	
		if ( pstBootyItem->ItemID >= ITEM_NUM_TASK_MIN && pstBootyItem->ItemID <= ITEM_NUM_TASK_MAX) //任务物品
		{
			//任务物品不受红白名修正
			iRateMul = 100;
		}
		
		//红白名修正
		if(iRateMul < 100 && (int)RAND1(100) >= iRateMul)
		{
			return -1;
		}
	}
	
	if (pstBootyItem->MinLevel > 0 && pstBootyItem->MinLevel > pstPlayer->stRoleData.Level) 
	{
		return -1;
	}

	if (pstBootyItem->MaxLevel >0 && pstBootyItem->MaxLevel < pstPlayer->stRoleData.Level)
	{
		return -1;
	}

	if ( (CAREER_NON != pstBootyItem->Career && (pstPlayer->stRoleData.Career&0x0F) != (pstBootyItem->Career&0x0F)) ||
		 (CAREER_NON != (pstBootyItem->Career&0xF0) && (pstPlayer->stRoleData.Career&0xF0) != (pstBootyItem->Career&0xF0)) )
	{
		return -1;
	}

	if (0 != pstBootyItem->TaskID1)
	{
		if (0 > mon_booty_check_task(pstEnv, pstBootyItem, pstPlayer, 0))
		{
			return -1;
		}
	}

	//防沉迷
	if (pstPlayer->bAasIncomeLimt > 0)
	{
		//沉迷期
		if(pstPlayer->bAasIncomeLimt >= 100)
		{
			return -1;
		}
		//疲劳期
		else if(pstPlayer->bAasIncomeLimt >= RAND1(100))
		{
			return -1;
		}
	}
	
	return 0;
}

int mon_booty_valid_player(Monster *pstMon, Player *pstPlayer)
{
	if (NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus || 
		pstPlayer->stRoleData.Map != pstMon->stMap.iID)
	{
		return -1;
	}
	
	if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE && !z_pos_in_view(&pstMon->stCurrPos, &pstPlayer->stRoleData.Pos))
	{
		return -1;
	}

	//有指定状态不分
	if(z_player_find_status(pstPlayer, ENTER_PWORLD_SAFE_STATUSID))
	{
		return -1;
	}
	
	return 0;
}

int mon_booty_assign_resource(ZONESVRENV* pstEnv, unsigned int uiExp, Player *pstPlayer)
{
	PlayerChagAttrVal stChgVal;
	AttrChgVal *pstVal;
	int iRet;
	tdr_longlong iExp = uiExp*(1 -pstPlayer->bAasIncomeLimt/100.0);

	stChgVal.AttrNum = 0;
	if(iExp > 0)
	{	
		/*
		if(GET_MAPID(pstPlayer->stRoleData.Map) == 21100 &&
			pstPlayer->stRoleData.Level >= 65)
		{
			return 0;
		}
		*/
		
		player_world_level_exp(pstEnv, pstPlayer, &iExp);
		player_use_offline_exp(pstEnv,pstPlayer,&iExp);	

		if(GET_MAPID(pstPlayer->stRoleData.Map) == 21100 &&
			pstPlayer->stRoleData.Level >= 65)
		{
			iExp = iExp / 10.0;
		}

		iRet = z_player_addexp_test(pstEnv,pstPlayer, &iExp);

		if(iExp <= 0)
		{
			/*
			if(iRet == -2)
			{
				z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
						"您当前存储的经验值已经达到上限,请提升等级,否则无法继续获取经验");
			}*/

			if (pstPlayer->stOnline.iUsedOfflineExp > 0)
			{
				z_player_awardexp_change(pstEnv,pstPlayer,pstPlayer->stOnline.iUsedOfflineExp, TRUE);
				pstPlayer->stOnline.iUsedOfflineExp = 0;
			}

			pstPlayer->stOnline.iAddWorldLevelExp = 0;

			return 0;
		}
		else
		{
			pstVal = &stChgVal.AttrList[stChgVal.AttrNum++];
			pstVal->AttrID = ATTR_ID_EXP;
			pstVal->nFlag = 0;
			pstVal->Value.Exp = iExp;
		}
	}

	//防沉迷提示比较蛋疼
	if(pstPlayer->bAasIncomeLimt > 0)
	{
		pstPlayer->stOnline.fAasMsgNotify = 1;
	}
	
	if (stChgVal.AttrNum > 0)
	{
		z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 2);
	}

	pstPlayer->stOnline.fAasMsgNotify = 0;
	
	return 0;
}

int mon_booty_check_task(ZONESVRENV* pstEnv, MONBOOTYITEM *pstBootyItem,  Player *pstPlayer, int fCollect)
{
	int i;
	TASKRUNONE *pstTaskRunOne1;
	TASKRUNONE *pstTaskRunOne2 = NULL;
	TASKDEF *pstTaskDef = NULL;
	int iNum = 0;
	
	pstTaskRunOne1 = task_run_find(pstPlayer, pstBootyItem->TaskID1);
	if (0 != pstBootyItem->TaskID2)
	{
		pstTaskRunOne2 = task_run_find(pstPlayer, pstBootyItem->TaskID2);
	}

	if ((NULL == pstTaskRunOne1 || pstTaskRunOne1->TaskStat != TASK_STAT_RUN ) && 
		(NULL == pstTaskRunOne2 || pstTaskRunOne2->TaskStat != TASK_STAT_RUN)) 
	{
		return -1;
	}

	if (pstTaskRunOne1)
	{
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne1->TaskID, &pstTaskRunOne1->TaskIdx);
		if (NULL == pstTaskDef)
		{
			return -1;
		}

		for (i=0; i<MAX_TASK_ITEM; i++)
		{
			if (pstTaskDef->GetItem[i].ItemID == 0 || pstTaskDef->GetItem[i].Num == 0)
			{
				break;
			}

			if (pstTaskDef->GetItem[i].ItemID == pstBootyItem->ItemID)
			{
				iNum += pstTaskDef->GetItem[i].Num;
				break;
			}
		}
	}
	
	if (pstTaskRunOne2)
	{
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne2->TaskID, &pstTaskRunOne2->TaskIdx);
		if (NULL == pstTaskDef)
		{
			return -1;
		}

		for (i=0; i<MAX_TASK_ITEM; i++)
		{
			if (pstTaskDef->GetItem[i].ItemID == 0 || pstTaskDef->GetItem[i].Num == 0)
			{
				break;
			}

			if (pstTaskDef->GetItem[i].ItemID == pstBootyItem->ItemID)
			{
				iNum += pstTaskDef->GetItem[i].Num;
				break;
			}
		}
	}

	//杀怪掉落，任务限制，不要求是任务需求物品
	if(!fCollect && iNum == 0)
	{
		return 0;
	}
	
	if (0 == package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstBootyItem->ItemID, iNum , -1, NULL,PACKAGE_DEC_NONE))
	{
		return -1;
	}

	return 0;
}
	
int booty_assign_pak_pick(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEGRID *pstRoleGrid,
			Monster *pstMon)
{
	int iNum;
	ROLEGRID stTmpGrid;

	iNum = pstRoleGrid->GridData.RoleItem.Num;
	memcpy(&stTmpGrid, pstRoleGrid, sizeof(stTmpGrid));
	if (package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRoleGrid, 1,0) < 0)
	{
		return -1;
	}
	package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRoleGrid, 1,0, IMPITEM_OPTYPE_PICK);

	stTmpGrid.GridData.RoleItem.Num = iNum - pstRoleGrid->GridData.RoleItem.Num;

	if (stTmpGrid.GridData.RoleItem.Num > 0)
	{
		z_role_booty_drop_oplog(pstEnv->pstAppCtx,pstEnv,
					pstPlayer->stRoleData.Map,&pstPlayer->stRoleData.Pos,&stTmpGrid, ITEM_DROP_TYPE_PACKAGE, pstMon->iID);
				
		z_role_pickitem_oplog( pstEnv, pstPlayer, &stTmpGrid, 1, OP_ASSIGN );
	}

	if (pstRoleGrid->GridData.RoleItem.Num > 0)
	{
		return -1;
	}

	return 0;
}

int booty_assign_player_item(ZONESVRENV* pstEnv, Monster *pstMon, MONBOOTYITEM *pstBootyItem, Player *pstPlayer)
{
	ROLEGRID stRoleGrid;
	ZoneBootyGrid stBootyGrid;
	ITEMDEF  *pstItemDef;
	MONSTERDEF *pstMonDef;
	ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);
	
	if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstBootyItem->ItemID))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "item create %d fail", pstBootyItem->ItemID);
		return -1;
	}

	if (GRID_ITEM_ITEM == stRoleGrid.Type)
	{
		stRoleGrid.GridData.RoleItem.Num = pstBootyItem->Num;
	}

	//超过全局限制就不掉落了
	if(0 > world_item_limit_check(pstEnv, pstBootyItem->ItemID))
	{
		return -1;
	}

	//记录下全局限制的掉落
	world_item_limit_add(pstEnv, pstBootyItem->ItemID, pstBootyItem->Num);

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if(NULL == pstMonDef)
	{
		return -1;
	}

	memset(&stBootyGrid, 0, sizeof(stBootyGrid));
	
	stBootyGrid.ullRoleID = pstPlayer->stRoleData.RoleID;
	stBootyGrid.bViewType = pstMonDef->BootyViewType;
	stBootyGrid.bType = pstMonDef->BootyDropType;
	if(pstTeam)
	{
		stBootyGrid.ullTeamID = pstTeam->stTeamInfo.TeamID;
	}
	
	pstItemDef = z_get_itemdef_roleitem(pstEnv, &stRoleGrid.GridData.RoleItem);
	if(pstItemDef && pstItemDef->DropSelfView)
	{
		stBootyGrid.bViewType = BOOTY_DROP_VIEW_SELF;
	}

	if (pstBootyItem->DirectPakage || (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_AUTO_PICK_ITEM))
	{
		if(0 > booty_assign_pak_pick(pstEnv, pstPlayer, &stRoleGrid, pstMon) &&
			pstBootyItem->DirectPakage)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);

			memcpy(&stBootyGrid.stGrid, &stRoleGrid, sizeof(stBootyGrid.stGrid));
		}
	}
	else
	{
		memcpy(&stBootyGrid.stGrid, &stRoleGrid, sizeof(stBootyGrid.stGrid));
	}

	if(stBootyGrid.stGrid.GridData.RoleItem.Num > 0)
	{
		dropitem_create(pstEnv, &stBootyGrid, &pstMon->stCurrPos, pstMon->stMap.iID, pstMon->iID);
	}
	
	return 0;
}

int booty_group_assign_player(ZONESVRENV* pstEnv, Monster * pstMon, MONBOOTYGROUP *pstGroup,
	ROLEGRID *pstRoleGrid, Player *pstPlayer)
{
	MONSTERDEF *pstMonDef;
	ITEMDEF  *pstItemDef;
	ZoneBootyGrid stBootyGrid;
	ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if(NULL == pstMonDef)
	{
		return -1;
	}

	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstRoleGrid->GridData.RoleItem);
	if(NULL == pstItemDef)
	{
		return -1;
	}

	memset(&stBootyGrid, 0, sizeof(stBootyGrid));
	
	stBootyGrid.ullRoleID = pstPlayer->stRoleData.RoleID;
	stBootyGrid.bType = pstMonDef->BootyDropType;
	stBootyGrid.bViewType = BOOTY_DROP_VIEW_ALL;
	
	if(pstTeam)
	{
		stBootyGrid.ullTeamID = pstTeam->stTeamInfo.TeamID;
	}
	
	if(pstItemDef->DropSelfView)
	{
		stBootyGrid.bViewType = BOOTY_DROP_VIEW_SELF;
	}

	if (pstGroup->DirectPakage || (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_AUTO_PICK_ITEM))
	{
		if(0 > booty_assign_pak_pick(pstEnv, pstPlayer, pstRoleGrid, pstMon) && pstGroup->DirectPakage)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);

			memcpy(&stBootyGrid.stGrid, pstRoleGrid, sizeof(stBootyGrid.stGrid));
		}
	}
	else
	{
		memcpy(&stBootyGrid.stGrid, pstRoleGrid, sizeof(stBootyGrid.stGrid));
	}

	if(stBootyGrid.stGrid.GridData.RoleItem.Num > 0)
	{
		dropitem_create(pstEnv, &stBootyGrid, &pstMon->stCurrPos, pstMon->stMap.iID, pstMon->iID);
	}

	return 0;
}

int booty_group_assign_by_clan(ZONESVRENV* pstEnv, Monster * pstMon, 
		ROLEGRID *pstRoleGrid, tdr_ulonglong ullClanGID)
{
	MONSTERDEF *pstMonDef;
	ITEMDEF  *pstItemDef;
	ZoneBootyGrid stBootyGrid;

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if(NULL == pstMonDef)
	{
		return -1;
	}

	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstRoleGrid->GridData.RoleItem);
	if(NULL == pstItemDef)
	{
		return -1;
	}

	memset(&stBootyGrid, 0, sizeof(stBootyGrid));
	
	stBootyGrid.ullClanGID = ullClanGID;
	stBootyGrid.bType = pstMonDef->BootyDropType;
	stBootyGrid.bViewType = BOOTY_DROP_VIEW_ALL;
	stBootyGrid.bRoll = ITEM_ROLL_WAIT;

	memcpy(&stBootyGrid.stGrid, pstRoleGrid, sizeof(stBootyGrid.stGrid));

	if(stBootyGrid.stGrid.GridData.RoleItem.Num > 0)
	{
		dropitem_create(pstEnv, &stBootyGrid, &pstMon->stCurrPos, pstMon->stMap.iID, pstMon->iID);
	}

	return 0;
}

int booty_group_assign_by_id(ZONESVRENV* pstEnv, Monster * pstMon, 
		ROLEGRID *pstRoleGrid, tdr_ulonglong ullRoleID, tdr_ulonglong ullTeamID)
{
	MONSTERDEF *pstMonDef;
	ITEMDEF  *pstItemDef;
	ZoneBootyGrid stBootyGrid;

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if(NULL == pstMonDef)
	{
		return -1;
	}

	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstRoleGrid->GridData.RoleItem);
	if(NULL == pstItemDef)
	{
		return -1;
	}

	memset(&stBootyGrid, 0, sizeof(stBootyGrid));
	
	stBootyGrid.ullRoleID = ullRoleID;
	stBootyGrid.bType = pstMonDef->BootyDropType;
	stBootyGrid.bViewType = BOOTY_DROP_VIEW_ALL;
	stBootyGrid.ullTeamID = ullTeamID;
	
	if(pstItemDef->DropSelfView)
	{
		stBootyGrid.bViewType = BOOTY_DROP_VIEW_SELF;
	}

	memcpy(&stBootyGrid.stGrid, pstRoleGrid, sizeof(stBootyGrid.stGrid));

	if(stBootyGrid.stGrid.GridData.RoleItem.Num > 0)
	{
		dropitem_create(pstEnv, &stBootyGrid, &pstMon->stCurrPos, pstMon->stMap.iID, pstMon->iID);
	}

	return 0;
}

int booty_group_assign_team_item(ZONESVRENV* pstEnv, Monster * pstMon, ROLEGRID *pstRoleGrid,
		ZoneTeam *pstTeam, int iRoll)
{
	MONSTERDEF *pstMonDef;
	ITEMDEF  *pstItemDef;
	ZoneBootyGrid stBootyGrid;

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if(NULL == pstMonDef)
	{
		return -1;
	}

	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstRoleGrid->GridData.RoleItem);
	if(NULL == pstItemDef)
	{
		return -1;
	}

	memset(&stBootyGrid, 0, sizeof(stBootyGrid));

	memcpy(&stBootyGrid.stGrid, pstRoleGrid, sizeof(stBootyGrid.stGrid));
	stBootyGrid.bType = pstMonDef->BootyDropType;
	stBootyGrid.bViewType = BOOTY_DROP_VIEW_ALL;
	stBootyGrid.ullTeamID = pstTeam->stTeamInfo.TeamID;
	if(iRoll)
	{
		stBootyGrid.bRoll = ITEM_ROLL_WAIT;
	}
	
	if(pstItemDef->DropSelfView)
	{
		stBootyGrid.bViewType = BOOTY_DROP_VIEW_SELF;
	}

	dropitem_create(pstEnv, &stBootyGrid, &pstMon->stCurrPos, pstMon->stMap.iID, pstMon->iID);

	return 0;
}

int booty_group_assign_team(ZONESVRENV* pstEnv, Monster * pstMon, MONBOOTYGROUP *pstGroup, 
	ROLEGRID *pstRoleGrid, ZoneTeam *pstTeam, Player **apstPlayer, int iPlayerNum)
{
	int iTeamBootyMode;
	ITEMDEF *pstItemDef;
	
	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstRoleGrid->GridData.RoleItem);
	if(NULL == pstItemDef)
	{
		return -1;
	}

	iTeamBootyMode = pstTeam->stTeamInfo.TeamBootyMode;

	switch(iTeamBootyMode)
	{
		case TEAM_BOOTY_RAND:
			if(iPlayerNum <= 0)
			{
				return booty_group_assign_team_item(pstEnv, pstMon, pstRoleGrid, pstTeam, 0);
			}
			else
			{
				return booty_group_assign_player(pstEnv, pstMon, pstGroup, pstRoleGrid, apstPlayer[(int)RAND1(iPlayerNum)]);
			}
			
			break;
		case TEAM_BOOTY_ROLL:
			if(pstItemDef->Important >= 2 || (ITEM_COLOR_BLUE & pstItemDef->CtrlFlag) ||
				(ITEM_COLOR_PURPLE & pstItemDef->CtrlFlag) || (ITEM_COLOR_PINK & pstItemDef->CtrlFlag))
			{
				return booty_group_assign_team_item(pstEnv, pstMon, pstRoleGrid, pstTeam, 1);
			}
			else
			{
				if(iPlayerNum <= 0)
				{
					return booty_group_assign_team_item(pstEnv, pstMon, pstRoleGrid, pstTeam, 0);
				}
				else
				{
					return booty_group_assign_player(pstEnv, pstMon, pstGroup, pstRoleGrid, apstPlayer[(int)RAND1(iPlayerNum)]);
				}
			}
			break;
		case TEAM_BOOTY_ALL:
			return booty_group_assign_team_item(pstEnv, pstMon, pstRoleGrid, pstTeam, 0);
			break;
		default:
			break;
	}

	return 0;
}

int booty_group_assign_collect_item(ZONESVRENV* pstEnv, Monster * pstMon,
	ROLEGRID *pstRoleGrid, int iCollectTime)
{
	MONSTERDEF *pstMonDef;
	ZoneBootyGrid stBootyGrid;

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if(NULL == pstMonDef)
	{
		return -1;
	}

	memset(&stBootyGrid, 0, sizeof(stBootyGrid));

	stBootyGrid.ullRoleID = 0;
	stBootyGrid.bType = pstMonDef->BootyDropType;
	stBootyGrid.bViewType = BOOTY_DROP_VIEW_ALL;
	stBootyGrid.bCollectTime = iCollectTime;
	memcpy(&stBootyGrid.stGrid, pstRoleGrid, sizeof(stBootyGrid.stGrid));

	dropitem_create(pstEnv, &stBootyGrid, &pstMon->stCurrPos, pstMon->stMap.iID, pstMon->iID);

	return 0;
}

int  booty_group_assign_item(ZONESVRENV* pstEnv, Monster * pstMon, ADDMONBOOTYGROUP *pstMonBooty, 
	MONBOOTYGROUP *pstGroup, ZoneTeam *pstTeam, Player **apstPlayer, int iPlayerNum)
{
	int iValidItem;
	int i, iIdx;
	MONBOOTYGROUPDEF *pstMonBootyGroupDef;
	ROLEGRID stRoleGrid;

	pstMonBootyGroupDef = z_find_mon_bootygroup_def(pstEnv, pstGroup->GroupID, NULL);
	if(pstMonBootyGroupDef == NULL)
	{
		return -1;
	}

	iValidItem = 0;
	for(i = 0; i<MAX_MON_BOOTY_GROUP_ITEM; i++)
	{
		if(pstMonBootyGroupDef->Items[i].ItemID == 0)
		{
			break;
		}

		iValidItem ++;
	}

	iIdx = RAND1(iValidItem);
	if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstMonBootyGroupDef->Items[iIdx].ItemID))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "item create %d fail", pstMonBootyGroupDef->Items[iIdx].ItemID);
		return -1;
	}

	if (GRID_ITEM_ITEM == stRoleGrid.Type)
	{
		stRoleGrid.GridData.RoleItem.Num = pstMonBootyGroupDef->Items[iIdx].Num;
	}

	if(pstMonBootyGroupDef->Bind)
	{
		stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
	}

	//超过全局限制就不掉落了
	if(0 > world_item_limit_check(pstEnv, pstMonBootyGroupDef->Items[iIdx].ItemID))
	{
		return -1;
	}

	//记录下全局限制的掉落
	world_item_limit_add(pstEnv, pstMonBootyGroupDef->Items[iIdx].ItemID, pstMonBootyGroupDef->Items[iIdx].Num);

	//采集类物品
	if(pstMonBooty->CollectTime > 0)
	{
		return booty_group_assign_collect_item(pstEnv, pstMon, &stRoleGrid, pstMonBooty->CollectTime);
	}

	if(pstMonBooty->Flag & BOOTY_FLAG_COLLECTER_ASSIGN)
	{
		if(iPlayerNum <= 0)
		{
			return -1;
			
		}
		
		//采集标记的只会带入采集者进来
		return booty_group_assign_player(pstEnv, pstMon, pstGroup, 
							&stRoleGrid, apstPlayer[(int)RAND1(iPlayerNum)]);
	}
	else
	{
		if(pstTeam)
		{
			booty_group_assign_team(pstEnv, pstMon, pstGroup, 
								&stRoleGrid, pstTeam, apstPlayer, iPlayerNum);
		}
		else
		{
			if(iPlayerNum > 0)
			{
				booty_group_assign_player(pstEnv, pstMon, pstGroup, 
								&stRoleGrid, apstPlayer[(int)RAND1(iPlayerNum)]);
			}
			else
			{
				tdr_ulonglong ullRoleID = 0;
				tdr_ulonglong ullTeamID = 0;
				tdr_ulonglong ullClanGID = 0;

				if(pstMon->cFirstAtkerType == MON_BOOTY_OWNER_PLAYER)
				{
					ullRoleID = pstMon->ullFirstAtkerWid;
					booty_group_assign_by_id(pstEnv, pstMon, &stRoleGrid, ullRoleID, ullTeamID);
				}
				else if(pstMon->cFirstAtkerType == MON_BOOTY_OWNER_TEAM)
				{
					ullTeamID = pstMon->ullFirstAtkerWid;
					booty_group_assign_by_id(pstEnv, pstMon, &stRoleGrid, ullRoleID, ullTeamID);
				}
				else if(pstMon->cFirstAtkerType == MON_BOOTY_OWNER_CLAN)
				{
					ullClanGID = pstMon->ullFirstAtkerWid;
					booty_group_assign_by_clan(pstEnv, pstMon, &stRoleGrid, ullClanGID);
				}
				
				
			}
		}
	}
	
	return 0;
}

int get_mon_in_pworld_grade(ZONESVRENV* pstEnv, Monster *pstMon)
{	
	ZONEPWORLDINST *pstPworldInst;
	ZONEPWORLD *pstPworld;
	ZONEMAPINST *pstMapInst; 

	if(pstMon->stMap.iID < PWORLD_MAPID_BASE)
	{
		return -1;
	}
	
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if(pstMapInst == NULL)
	{
		return -1;
	}
	
	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (!pstPworldInst)
	{
		return -1;
	}

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if (!pstPworld || pstPworld->stPworldDef.DynType == 0 ||
		pstPworldInst->stPworldselect.PworldGrade >= MAX_DYN_GRADE)
	{
		return -1;
	}

	return pstPworldInst->stPworldselect.PworldGrade;
}

int mon_booty_group_assign(ZONESVRENV* pstEnv, Monster *pstMon, ADDMONBOOTYGROUP *pstMonBooty, ZoneTeam *pstTeam)
{
	int i, iRet;
	int iRnd, iLimitNum;
	MONBOOTYDEF *pstMonBootyDef;
	int iRate;
	Player *apstPlayer[MAX_TEAM_MEMBER]; 
	Player *pstPlayerTmp;
	int iPlayerNum;
	int iSucc = 0;
	
	int iSumExRateMul = 0;//额外的掉率修正总和


	pstMonBootyDef = z_find_mon_booty_def(pstEnv, pstMonBooty->GroupID, NULL);
	if(pstMonBootyDef == NULL)
	{
		return 0;
	}

	iPlayerNum = 0;
	if(pstTeam)
	{
		for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
		{
			pstPlayerTmp = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
			if (NULL == pstPlayerTmp || 0 > mon_booty_valid_player(pstMon, pstPlayerTmp))
			{
				continue;
			}
			if (pstPlayerTmp->iMemID == pstMon->iKillerMemID)
			{				
				iSumExRateMul = pstPlayerTmp->stOnline.stVipBarInfo.iVipBarDropMul;
			}
			
			apstPlayer[iPlayerNum] = pstPlayerTmp;


			iPlayerNum++;
			
		}

	}
	else
	{
		if(pstMon->cFirstAtkerType == MON_BOOTY_OWNER_PLAYER)
		{
			pstPlayerTmp = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->iFirstAtkerID);
			if (pstPlayerTmp && pstPlayerTmp->stRoleData.RoleID == pstMon->ullFirstAtkerWid)
			{
				iSumExRateMul = pstPlayerTmp->stOnline.stVipBarInfo.iVipBarDropMul;
				apstPlayer[iPlayerNum] = pstPlayerTmp;
				
				
				iPlayerNum++;
			}
		}
		else if(pstMon->cFirstAtkerType == MON_BOOTY_OWNER_CLAN)
		{
			iPlayerNum = 0;
		}
	}

	

	iLimitNum = 0;
	iRate = 0;
	iRnd = RAND1(1000000);
	for(i = 0; i<MAX_BOOTY_GROUP; i++)
	{
		iSucc = 0;
		
		if (pstMonBootyDef->BootyGroup[i].Rate == 0)
		{
			break;
		}

		//最多命中组数限定
		if(pstMonBootyDef->LimitGroupNum != 0 && iLimitNum >= pstMonBootyDef->LimitGroupNum)
		{
			break;
		}

		//游戏事件限制掉落，用于圣诞节活动之类的特殊掉落
		if(pstMonBootyDef->BootyGroup[i].EventLimit > 0)
		{
			GameEventInst stEventInst, *pstEvent;
			
			stEventInst.iEventID = pstMonBootyDef->BootyGroup[i].EventLimit;
			pstEvent = sht_find(pstEnv->pstShtEvent, &stEventInst, EventHashCmp, EventHashCode);
			if(NULL == pstEvent)
			{
				continue;
			}
		}

		//动态副本难度限定
		if(pstMonBootyDef->BootyGroup[i].PworldGrade > 0 )
		{
			iRet = get_mon_in_pworld_grade(pstEnv, pstMon);
			if(iRet < 0 || iRet +1 != pstMonBootyDef->BootyGroup[i].PworldGrade)
			{
				continue;
			}
		}

		if(pstMonBootyDef->RateFlag == 0)
		{
			iRate = pstMonBootyDef->BootyGroup[i].Rate;
			//全局控制掉落组概率
			iRate +=	world_booty_group_limit_val_get(pstEnv, pstMonBootyDef->BootyGroup[i].GroupID);

			//独立概率
			iRnd = RAND1(1000000);
			if(iRnd < iRate +  (int)(iRate * (iSumExRateMul / 100.0)))
			{
				iSucc = 1;
			}
			
		}
		else
		{
			int iLimitRate = 0;

			iRate += pstMonBootyDef->BootyGroup[i].Rate;
			if(iRnd >= iRate +  (int)(iRate * (iSumExRateMul / 100.0)))
			{
				continue;
			}

			//如果有减少概率的修正，在计算下是否掉。不掉就什么都不掉了
			iLimitRate = world_booty_group_limit_val_get(pstEnv, pstMonBootyDef->BootyGroup[i].GroupID);
			if(iLimitRate < 0 && RAND1(1000000) < (-iLimitRate))
			{
				break;
			}

			iSucc = 1;
		}
		
		if(iSucc)
		{
			//记录掉落控制
			world_booty_group_limit_add(pstEnv, pstMonBootyDef->BootyGroup[i].GroupID, 1);
			
			iLimitNum ++;			
		
			if(pstTeam)
			{
				booty_group_assign_item(pstEnv, pstMon, pstMonBooty,
				&pstMonBootyDef->BootyGroup[i], pstTeam, apstPlayer, iPlayerNum);
			}
			else
			{
				if (pstMon->cRonglu)
				{	
					Player *pstPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pstMon->szOwner);
					if (pstPlayer)
					{
						apstPlayer[0] = pstPlayer;
						iPlayerNum = 1;
						booty_group_assign_item(pstEnv, pstMon, pstMonBooty,
						&pstMonBootyDef->BootyGroup[i], NULL, apstPlayer, iPlayerNum);	
					}
				}
				else
				{
					booty_group_assign_item(pstEnv, pstMon, pstMonBooty,
						&pstMonBootyDef->BootyGroup[i], NULL, apstPlayer, iPlayerNum);
				}
			}

			if(pstMonBootyDef->RateFlag)
				break;
		}
		else
		{
			//记录掉落控制
			world_booty_group_limit_add(pstEnv, pstMonBootyDef->BootyGroup[i].GroupID, 0);
		}
	}
	
	return 0;
}

int mon_booty_add_item(ZONESVRENV* pstEnv, Monster *pstMon, MONBOOTYITEM *pstBootyItem, Player **apstPlayer, MonRateMul *apstRateMul, int iPlayerNum)
{
	int i;
	int iRate = 0;
	int iRateMul = 100;
	int iRet, iValidPlayerNum;
	Player *apstBootyPlayer[MAX_TEAM_MEMBER];
	
	if ( pstBootyItem->ItemID >= ITEM_NUM_TASK_MIN && pstBootyItem->ItemID <= ITEM_NUM_TASK_MAX) //任务物品
	{
		//任务物品不受红白名修正
		iRateMul = 100;
	}
	else
	{
		//大家都会获得的物品采用概率最小的那个
		if(pstBootyItem->TeamBootyType == MON_BOOTY_TEAM_TYPE_ALL)
		{
			for(i=0; i<iPlayerNum; i++)
			{
				if(iRateMul > apstRateMul[i].iItemRateMul)
				{
					iRateMul = apstRateMul[i].iItemRateMul;
				}
			}
		}
	}

	/*
	人物装备等相关修正
	iRate = pstBootyItem->Rate + get_player_rate_mul(pstOwner, pstBootyItem->ItemID);
	*/

	iRate = pstBootyItem->Rate;
	if (iRate < 10000 && (int)(RAND1(1000000)) >= iRate*iRateMul)
	{
		return -1;
	}

	iValidPlayerNum = 0;
	//过滤玩家
	for(i = 0; i<iPlayerNum; i++)
	{
		iRet = mon_booty_item_valid_player(pstEnv, pstBootyItem, apstPlayer[i], apstRateMul[i].iItemRateMul);
		if(iRet == 0 && iValidPlayerNum < MAX_TEAM_MEMBER)
		{
			apstBootyPlayer[iValidPlayerNum++] = apstPlayer[i];
		}
	}

	if(iValidPlayerNum <= 0)
	{
		return -1;
	}

	//符合条件的人人都会获得
	if(pstBootyItem->TeamBootyType == MON_BOOTY_TEAM_TYPE_ALL)
	{
		for(i=0; i<iValidPlayerNum; i++)
		{
			booty_assign_player_item(pstEnv, pstMon, pstBootyItem, apstBootyPlayer[i]);
		}
	}
	else
	{
		booty_assign_player_item(pstEnv, pstMon, pstBootyItem, apstBootyPlayer[(int)RAND1(iValidPlayerNum)]);
	}

	return 0;
}

// 怪物死亡给有效范围内队伍玩家加buff
int mon_die_team_add_buff(ZONESVRENV* pstEnv, Monster *pstMon,int iBuffID, int iSec)
{
	ZoneTeam *pstTeam;
	int i;
	Player *pstPlayerTmp;
	int iValidTeamNum;
	Player *apstPlayer[MAX_TEAM_MEMBER];
		
	if (0 == pstMon->szOwner[0] || is_convoy_car(pstMon))
	{
		if(pstMon->cFirstAtkerType == MON_BOOTY_OWNER_TEAM)
		{
			pstTeam = z_idx_team(pstEnv, pstMon->iFirstAtkerID, pstMon->ullFirstAtkerWid);
			if (!pstTeam)
			{
				return 0;
			}

			for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
			{
				pstPlayerTmp = z_name_player(pstEnv->pstAppCtx, pstEnv, 
											pstTeam->stTeamInfo.TeamMember[i].MemberName);
				if (NULL == pstPlayerTmp || 0 > mon_booty_valid_player(pstMon, pstPlayerTmp))
				{
					continue;
				}
				
				apstPlayer[iValidTeamNum++] = pstPlayerTmp;
			}
		}
		else
		{
			return 0;
		}
	}

	if (iValidTeamNum > 0 && iValidTeamNum <= MAX_TEAM_MEMBER)
	{
		for (i=0; i<iValidTeamNum; i++)
		{
			z_player_apply_status(pstEnv->pstAppCtx, pstEnv, apstPlayer[i], iBuffID,iValidTeamNum, iSec, 1, 0);
		}
	}

	return 0;
}

int mon_booty_team_zhengyi(ZONESVRENV* pstEnv, MONSTERDEF *pstMonDef, Player **pastPlayer, int iValidNum)
{
	int i;
	int iMul;
	unsigned int iZhengyi;

	if (iValidNum >= 6)
	{
		iMul = 25;
	}
	else if (iValidNum >= 5)
	{
		iMul = 15;
	}
	else if (iValidNum >= 4)
	{
		iMul = 10;
	}
	else if (iValidNum >= 3)
	{
		iMul = 6;
	}
	else if (iValidNum >= 2)
	{
		iMul = 5;
	}
	else
	{
		iMul = 0;
	}

	iZhengyi = pstMonDef->Zhengyi * (1 + iMul/100.0);

	for (i = 0; i < iValidNum; i++)
	{
		package_add_money(pstEnv, pastPlayer[i], ATTR_ID_ZHENGYI, iZhengyi);
	}
	
	return 0;
}

int mon_booty_assign_team(ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef, ZoneTeam *pstTeam)
{
	int i;
	//static const int iStatExp[MAX_TEAM_MEMBER+1] = { 0, 0, 15, 30, 45, 60, 75};
	Player *apstPlayer[MAX_TEAM_MEMBER];
	MonRateMul astRateMul[MAX_TEAM_MEMBER];
	MonRateMul *pstRateMul;
	int iValidTeamNum = 0;
	Player *pstPlayerTmp;
	MONBOOTYMONEY *pstBootyMoney;
	MONBOOTYITEM *pstBootyItem;
	unsigned int uiExpTotal = 0;
	unsigned int uiPetExpTotal = 0; 
	
	for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		pstPlayerTmp = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
		if (NULL == pstPlayerTmp || 0 > mon_booty_valid_player(pstMon, pstPlayerTmp))
		{
			continue;
		}
		
		if (MISC_FLAG_TRACE & pstPlayerTmp->stOnline.cMiscFlag)
		{
			task_trace_mon(pstEnv, pstPlayerTmp, pstMon);
		}
		
		apstPlayer[iValidTeamNum] = pstPlayerTmp;
		pstRateMul = &astRateMul[iValidTeamNum];
		memset(pstRateMul, 0, sizeof(*pstRateMul));
		pstRateMul->ExpMul.iColorExpMul = 100;
		pstRateMul->iItemRateMul = 100;
		pstRateMul->iMoneyMul = 100;
		
		mon_booty_player_mul(pstEnv, pstPlayerTmp, pstTeam, pstMon, pstMonDef, &pstRateMul->ExpMul, &pstRateMul->iMoneyMul , &pstRateMul->iItemRateMul);	

		iValidTeamNum++;

		ronglu_kill_mon_booty(pstEnv,pstPlayerTmp,pstMon);
		lostmap_mon_die(pstEnv,pstPlayerTmp,pstMonDef, pstMon);
	}

	//分正义点
	if (pstMonDef->Zhengyi > 0)
	{
		mon_booty_team_zhengyi(pstEnv, pstMonDef, apstPlayer, iValidTeamNum);
	}
	
	if (0 >= iValidTeamNum)
	{
		return -1;
	}

	if(!is_share_exp_pworld(pstEnv, pstMon))
	{
		for (i=0; i<iValidTeamNum; i++)
		{
			if(apstPlayer[i]->stOnline.State & CS_STAT_DEAD)
			{
				continue;
			}

			uiExpTotal = pstMonDef->Booty.Exp*(astRateMul[i].ExpMul.iColorExpMul/100.0)*(100 + astRateMul[i].ExpMul.iMonDynExpMul)/100.0;
			uiExpTotal += unreal_add_award_exp(pstEnv,apstPlayer[i]);
			if (uiExpTotal < 1 && pstMonDef->Booty.Exp)
		    	{
		            uiExpTotal = 1;
		    	}

			astRateMul[i].ExpMul.iResultExpMul += astRateMul[i].ExpMul.iNormalExpMul +
													pstEnv->pstConf->ExpMul +
													astRateMul[i].ExpMul.iQiYueExpPlayerMul +
													iStatExp[iValidTeamNum];

			uiPetExpTotal = pstMonDef->Booty.Exp*((100 + astRateMul[i].ExpMul.iMonDynExpMul)/100.0/iValidTeamNum*(100 + (astRateMul[i].ExpMul.iPetExpMul))/100.0);
			if(uiPetExpTotal < 1 && pstMonDef->Booty.Exp)
			{
				 uiPetExpTotal = 1;
			}
			
			mon_booty_dynexp_player_limit(pstEnv, pstMonDef, apstPlayer[i], &uiExpTotal);

			apstPlayer[i]->stOnline.stVipBarInfo.iVipBarEffectExp = (int)uiExpTotal * (apstPlayer[i]->stOnline.stVipBarInfo.iVipBarExpMul /100.0);

			//原来的
			//mon_booty_assign_resource(pstEnv, uiExpTotal*((100+ astRateMul[i].ExpMul.iResultExpMul)/100.0), apstPlayer[i]);
			//lzk   公会技能经验修正改变了算法
			
			mon_booty_assign_resource(pstEnv, uiExpTotal*((100+ astRateMul[i].ExpMul.iResultExpMul+astRateMul[i].ExpMul.fClanSkillExpMul)/100.0), apstPlayer[i]);
			mon_booty_assign_pet_resource(pstEnv, pstMonDef, apstPlayer[i], uiPetExpTotal, iStatExp[iValidTeamNum]+astRateMul[i].ExpMul.iQiYueExpPetMul);

			// 获得声望
			if(pstMonDef->Booty.Respact.ID)
			{
				player_respact_inc(pstEnv, apstPlayer[i],
								pstMonDef->Booty.Respact.ID,
								pstMonDef->Booty.Respact.Value, 1);
			}
		}
	}
	
	for (i=0; i<MAX_MONBOOTY_MONEY; i++)
	{
		pstBootyMoney = &pstMonDef->Booty.Moneys[i];	

		if (ATTR_ID_MONEY != pstBootyMoney->MoneyAttrID && ATTR_ID_BULL != pstBootyMoney->MoneyAttrID)
		{
			break;
		}

		if ((int)(RAND1(10000)) >= pstBootyMoney->Rate * 100.0)
		{
			continue;
		}
		
		booty_money_drop(pstEnv, apstPlayer[(int) RAND1(iValidTeamNum)], pstMon, 
				pstBootyMoney->MoneyAttrID, pstBootyMoney->MoneyVal);
	}

	for (i=0; i<MAX_MONBOOTY_ITEM; i++)
	{
		pstBootyItem = pstMonDef->Booty.Items + i;
		if (0 == pstBootyItem->ItemID)
		{
			break;
		}

		mon_booty_add_item(pstEnv, pstMon, pstBootyItem, apstPlayer, astRateMul, iValidTeamNum);
	}

	/*
	//获得宠物蛋 
	iRnd = RAND1(10000);
	if (iRnd < (pstMonDef->Booty.BootyPetEggRate)*100)
	{
		mon_booty_assign_egg(pstEnv, pstMonDef, pstMon, apstPlayer[(int) RAND1(iValidTeamNum)]);
	}
	*/
	
	return 0;
}

int mon_booty_assign_single(ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef, Player *pstPlayer)
{
	int i;
	MONBOOTYITEM *pstBootyItem;
	MONBOOTYMONEY *pstBootyMoney;
	MonExpMul stExpMul;
	int iMoneyMul = 100;
	int iItemRateMul = 100;
	unsigned int uiExpTotal = 0;
	unsigned int uiPetExpTotal = 0; 
	int iExtraRate = 0;
	MonRateMul stRateMul;

	memset(&stExpMul, 0, sizeof(stExpMul));
	stExpMul.iColorExpMul = 100;

	if (MISC_FLAG_TRACE & pstPlayer->stOnline.cMiscFlag)
	{
		//task_trace_mon(pstEnv, pstPlayer, pstMon->iDefIdx);
		task_trace_mon(pstEnv, pstPlayer, pstMon);
	}

	ronglu_kill_mon_booty(pstEnv,pstPlayer,pstMon);
	lostmap_mon_die(pstEnv,pstPlayer,pstMonDef, pstMon);
	
	mon_booty_player_mul(pstEnv, pstPlayer, NULL, pstMon, pstMonDef, &stExpMul, &iMoneyMul, &iItemRateMul);
	iExtraRate = pstPlayer->stOnline.stDyn.stHealChg.ucBullRate;

	if(!is_share_exp_pworld(pstEnv, pstMon))
	{
		uiExpTotal = pstMonDef->Booty.Exp*(stExpMul.iColorExpMul/100.0);
		uiExpTotal = uiExpTotal*((100 + stExpMul.iMonDynExpMul)/100.0);
		uiExpTotal += unreal_add_award_exp(pstEnv,pstPlayer);
		stExpMul.iResultExpMul += stExpMul.iNormalExpMul  + pstEnv->pstConf->ExpMul ;
		uiPetExpTotal = uiExpTotal*((100 + stExpMul.iPetExpMul)/100.0);
		mon_booty_dynexp_player_limit(pstEnv, pstMonDef, pstPlayer, &uiExpTotal);

		pstPlayer->stOnline.stVipBarInfo.iVipBarEffectExp = (int)uiExpTotal * (pstPlayer->stOnline.stVipBarInfo.iVipBarExpMul /100.0);
		//原来的
		//mon_booty_assign_resource(pstEnv, uiExpTotal*((100+ stExpMul.iResultExpMul)/100.0), pstPlayer);
		//lzk   公会技能经验修正算法改变了
		mon_booty_assign_resource(pstEnv, uiExpTotal*((100+ stExpMul.iResultExpMul+stExpMul.fClanSkillExpMul)/100.0), pstPlayer);
		mon_booty_assign_pet_resource(pstEnv, pstMonDef, pstPlayer, uiPetExpTotal, 0);
	}

	//金钱
	for (i=0; i<MAX_MONBOOTY_MONEY; i++)
	{
		pstBootyMoney = &pstMonDef->Booty.Moneys[i];	

		if (ATTR_ID_MONEY != pstBootyMoney->MoneyAttrID && ATTR_ID_BULL != pstBootyMoney->MoneyAttrID)
		{
			break;
		}

		if ((int)(RAND1(10000)) >= pstBootyMoney->Rate * 100.0)
		{
			continue;
		}

		//单人获得金钱的时候防沉迷
		booty_money_drop(pstEnv, pstPlayer, pstMon, 
				pstBootyMoney->MoneyAttrID, pstBootyMoney->MoneyVal*(1 - pstPlayer->bAasIncomeLimt/100.0));
	}
	
	memset(&stRateMul, 0, sizeof(stRateMul));
	stRateMul.iItemRateMul = iItemRateMul;
		
	for (i=0; i<MAX_MONBOOTY_ITEM; i++)
	{
		pstBootyItem = pstMonDef->Booty.Items + i;
		if (0 == pstBootyItem->ItemID)
		{
			break;
		}

		mon_booty_add_item(pstEnv, pstMon, pstBootyItem, &pstPlayer, &stRateMul, 1);
	}

	/*
	//获得宠物蛋
	iRnd = RAND1(10000);
	if (iRnd < (pstMonDef->Booty.BootyPetEggRate+ pstPlayer->stOnline.stDyn.stHealChg.ucEggRate)*100)
	{
		mon_booty_assign_egg(pstEnv, pstMonDef, pstMon, pstPlayer);
	}
	*/

	// 获得声望
	if(pstMonDef->Booty.Respact.ID)
	{
		player_respact_inc(pstEnv, pstPlayer, pstMonDef->Booty.Respact.ID,
					pstMonDef->Booty.Respact.Value, 1);
	}

	if (pstMonDef->Zhengyi > 0)
	{
		package_add_money(pstEnv, pstPlayer, ATTR_ID_ZHENGYI, pstMonDef->Zhengyi);
	}
	
	return 0;
}

int mon_boss_assign(ZONESVRENV* pstEnv, Monster *pstMon)
{
	int i;
	Player *pstPlayer;
	STATUSRESULT stStatusResult;
	int iExpMul;
	int iExp;
	SearchArea stSArea;
	ZONEMAPINST *pstMapInst;

	if(pstMon->bSubType != MON_SUB_TYPE_BOSS)
	{
		return 0;
	}

	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	
	if(0 == z_pos_search_area(pstEnv->pstAppCtx, pstEnv, pstMapInst, &pstMon->stCurrPos, &stSArea))
	{
		int j;
		AREADYN *pstArea;

		for(i = 0; i < stSArea.iNum; i++)
		{
			pstArea = stSArea.pastArea[i];
			for (j = 0; j < pstArea->iAnimate; j++)
			{
				
				pstPlayer	=	z_id_player(pstEnv->pstAppCtx, pstEnv, pstArea->aiAnimate[j]);
				if (NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
				{
					continue;
				}

				if(is_mon_booty_owner(pstMon, pstPlayer))
				{
					continue;
				}
				
				if(!is_in_player_boss_info(pstPlayer, pstMon))
				{
					continue;
				}

				//任务追踪
				if ((MISC_FLAG_TRACE & pstPlayer->stOnline.cMiscFlag) && 
					0 == mon_trace_valid_player(pstMon, pstPlayer))
				{
					task_trace_mon(pstEnv, pstPlayer, pstMon);
				}

				z_get_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_PLAYER_ADDEXP, &stStatusResult, 0);

				iExpMul = pstEnv->pstConf->ExpMul + stStatusResult.ResultVal1;
				iExp = (pstMon->stMonDynAttr.Level*10)*(1+iExpMul/100.0);
				
				//经验
				mon_booty_assign_resource(pstEnv, iExp, pstPlayer);

				//清理掉
				player_boss_info_del(pstPlayer, pstMon);
			}
		}
	}

	return 0;
}

int mon_camp_task(ZONESVRENV* pstEnv, Monster *pstMon)
{
	int i;
	Player *pstPlayer;
	Player *pstFirstPlayer =  NULL;
	SearchArea stSArea;
	ZONEMAPINST *pstMapInst;

	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	if (pstMon->iFirstAtkerMemID != 0)
	{
		pstFirstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->iFirstAtkerMemID);
		if (pstFirstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			pstFirstPlayer = NULL;
		}
	}
	
	if(0 == z_pos_search_area(pstEnv->pstAppCtx, pstEnv, pstMapInst, &pstMon->stCurrPos, &stSArea))
	{
		int j;
		AREADYN *pstArea;

		for(i = 0; i < stSArea.iNum; i++)
		{
			pstArea = stSArea.pastArea[i];
			for (j = 0; j < pstArea->iAnimate; j++)
			{
				
				pstPlayer	=	z_id_player(pstEnv->pstAppCtx, pstEnv, pstArea->aiAnimate[j]);
				if (NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
				{
					continue;
				}

				if (pstFirstPlayer && pstFirstPlayer->stOnline.stWarOL.cCampIdx != pstPlayer->stOnline.stWarOL.cCampIdx)
				{
					continue;
				}

				if(!is_in_player_boss_info(pstPlayer, pstMon))
				{
					continue;
				}

				//任务追踪
				if ((MISC_FLAG_TRACE & pstPlayer->stOnline.cMiscFlag) && 
					0 == mon_trace_valid_player(pstMon, pstPlayer))
				{
					task_trace_mon(pstEnv, pstPlayer, pstMon);
				}

				//清理掉
				player_boss_info_del(pstPlayer, pstMon);
			}
		}
	}

	return 0;
}


int mon_booty_task_assign(ZONESVRENV* pstEnv, Monster *pstMon, Player *pstKiller)
{
	int i;
	int fIn = 0;
	Player *pstPlayer;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;
	MONSTERDEF *pstMonDef;

	if (0 != pstMon->szOwner[0])
	{
		return 0;
	}

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (NULL == pstMonDef)
	{
		return 0;
	}

	if (MON_MISC_TYPE_CAMP_TASK & pstMonDef->MiscType)
	{
		return mon_camp_task(pstEnv, pstMon);
	}

	if(pstMon->bSubType == MON_SUB_TYPE_BOSS)
	{
		return mon_boss_assign(pstEnv, pstMon);
	}

	for(i=0; i<pstTargetList->PressNum; i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstTargetList->astPressEntrys[i].iTargetID);
		if(!pstPlayer)
		{
			continue;
		}

		if(pstTargetList->astPressEntrys[i].iPress <= 0)
		{
			continue;
		}

		if(is_mon_booty_owner(pstMon, pstPlayer))
		{
			continue;
		}

		if(pstKiller == pstPlayer)
		{
			fIn = 1;
		}

		//任务追踪
		if ((MISC_FLAG_TRACE & pstPlayer->stOnline.cMiscFlag) && 
			0 == mon_trace_valid_player(pstMon, pstPlayer))
		{
			task_trace_mon(pstEnv, pstPlayer, pstMon);
		}
	}

	if(pstKiller && 
		fIn == 0 && 
		!is_mon_booty_owner(pstMon, pstKiller))
	{
		//任务追踪
		if ((MISC_FLAG_TRACE & pstKiller->stOnline.cMiscFlag) && 
			0 == mon_trace_valid_player(pstMon, pstKiller))
		{
			task_trace_mon(pstEnv, pstKiller, pstMon);
		}
	}
	
	return 0;
}

int is_share_exp_pworld(ZONESVRENV* pstEnv, Monster *pstMon)
{
	ZONEPWORLDINST *pstPworldInst;
	ZONEPWORLD *pstPworld;
	ZONEMAPINST *pstMapInst;
	
	if(pstMon->stMap.iID < PWORLD_MAPID_BASE)
	{
		return 0;
	}
	
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if(pstMapInst == NULL)
	{
		return 0;
	}
	
	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (!pstPworldInst)
	{
		return 0;
	}

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if(!pstPworld || !(pstPworld->stPworldDef.CtrlFlag & PWORLD_SHARE_EXP))
	{
		return 0;
	}

	return 1;
}

int mon_booty_pworld_exp_assign(ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef)
{
	ZONEPWORLDINST *pstPworldInst;
	ZONEPWORLD *pstPworld;
	ZONEMAPINST *pstMapInst;
	MonExpMul stExpMul;
	int iMoneyMul = 100;
	int iItemRateMul = 100;
	unsigned int uiExpTotal = 0;
	unsigned int uiPetExpTotal = 0; 
	int i, iExtraRate = 0;
	Player *pstPlayer;
	float f = 1.0f;

	if(pstMon->stMap.iID < PWORLD_MAPID_BASE)
	{
		return -1;
	}
	
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if(pstMapInst == NULL)
	{
		return -1;
	}
	
	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (!pstPworldInst)
	{
		return -1;
	}

	if (pstPworldInst->cShareShuNiu)
	{
		f = 1.1f;
	}

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if (!pstPworld || !(pstPworld->stPworldDef.CtrlFlag & PWORLD_SHARE_EXP))
	{
		return -1;
	}

	for(i=0; i<pstPworldInst->iPlayer; i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i]);
		if(!pstPlayer)
		{
			continue;
		}

		if(pstPlayer->stOnline.State & CS_STAT_DEAD)
		{
			continue;
		}
		
		memset(&stExpMul, 0, sizeof(stExpMul));
		stExpMul.iColorExpMul = 100;
		iMoneyMul = 100;
		iItemRateMul = 100;
		
		mon_booty_player_mul(pstEnv, pstPlayer, NULL, pstMon, pstMonDef, &stExpMul, &iMoneyMul, &iItemRateMul);
		iExtraRate = pstPlayer->stOnline.stDyn.stHealChg.ucBullRate;
		uiExpTotal = pstMonDef->Booty.Exp*(stExpMul.iColorExpMul/100.0);
		uiExpTotal = uiExpTotal*((100 + stExpMul.iMonDynExpMul)/100.0);
		stExpMul.iResultExpMul += stExpMul.iNormalExpMul  + pstEnv->pstConf->ExpMul ;
		uiPetExpTotal = uiExpTotal*((100 + stExpMul.iPetExpMul)/100.0);
		mon_booty_dynexp_player_limit(pstEnv, pstMonDef, pstPlayer, &uiExpTotal);
		pstPlayer->stOnline.stVipBarInfo.iVipBarEffectExp = (int)uiExpTotal * (pstPlayer->stOnline.stVipBarInfo.iVipBarExpMul /100.0);

		//原来的
		//mon_booty_assign_resource(pstEnv, uiExpTotal*((100+ stExpMul.iResultExpMul)/100.0)*f, pstPlayer);
		//lzk   公会技能经验修正算法改变了
		mon_booty_assign_resource(pstEnv, uiExpTotal*((100+ stExpMul.iResultExpMul+stExpMul.fClanSkillExpMul)/100.0)*f, pstPlayer);
		mon_booty_assign_pet_resource(pstEnv, pstMonDef, pstPlayer, uiPetExpTotal, 0);
	}
	
	return 0;
}

int mon_booty_assign(ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef, Player *pstKiller)
{
	ZoneTeam *pstTeam = NULL;
	Player *pstPlayer = NULL;
	int i;

	if (0 == pstMon->iFirstAtkerID && 0 == pstMon->szOwner[0])
	{
		return -1;
	}

	if (0 == pstMon->iFirstAtkerID &&  is_convoy_car(pstMon))
	{
		return -1;
	}

	if (0 == pstMon->szOwner[0] || is_convoy_car(pstMon))
	{
		if(pstMon->cFirstAtkerType == MON_BOOTY_OWNER_PLAYER)
		{
			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->iFirstAtkerID);
		}
		else if (pstMon->cFirstAtkerType == MON_BOOTY_OWNER_TEAM)
		{
			pstTeam = z_idx_team(pstEnv, pstMon->iFirstAtkerID, pstMon->ullFirstAtkerWid);
		}
	}
	else
	{
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstMon->szOwner);
	}

	if(pstPlayer && is_valid_booty_team(pstEnv, pstPlayer))
	{
		pstTeam = player_team_get(pstEnv, pstPlayer);
	}

	mon_booty_task_assign(pstEnv, pstMon, pstKiller);

	//初始化掉落位置
	rand_position_init();
	
	if(pstTeam)
	{
		mon_booty_assign_team(pstEnv, pstMon, pstMonDef, pstTeam);
	}
	else if(pstPlayer)
	{
		
		mon_booty_assign_single(pstEnv, pstMon, pstMonDef, pstPlayer);
	}

	mon_booty_pworld_exp_assign(pstEnv, pstMon, pstMonDef);
	
	//掉落包单拿出来
	for (i=0; i<MAX_MON_BOOTY_GROUP_LIST; i++)
	{
		if(pstTeam)
		{
			mon_booty_group_assign(pstEnv, pstMon,
					&pstMonDef->BootyGroup[i], pstTeam);
		}
		else
		{
			mon_booty_group_assign(pstEnv, pstMon,
					&pstMonDef->BootyGroup[i], NULL);
		}
	}

	return 0;
}

int mon_booty_owner_tick(ZONESVRENV* pstEnv, Monster *pstMon)
{
	Player *pstPlayer;
	ZoneTeam *pstTeam;
	MONSTERDEF *pstMonDef;

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if(NULL == pstMonDef)
	{
		return -1;
	}

	//目前的公会所属掉落的怪物不会清除所有信息
	if(pstMonDef->ClanBooty)
	{
		return 0;
	}

	//超时清理归属
	if (pstMon->iFirstAtkerID > 0 &&
		pstMon->tLastAtkTime + FIRST_ATKER_TIME < pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		goto _clear;
	}
	
	//只用检查所有人转移向队伍
	if(pstMon->cFirstAtkerType == MON_BOOTY_OWNER_PLAYER)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->iFirstAtkerID);
		if(NULL == pstPlayer)
		{
			return 0;
		}

		//转移给队伍
		pstTeam = player_team_get(pstEnv, pstPlayer);
		if(pstTeam && is_valid_booty_team(pstEnv, pstPlayer))
		{
			mon_booty_owner_update(pstEnv, pstMon, pstPlayer);
		}
	}
	else if(pstMon->cFirstAtkerType == MON_BOOTY_OWNER_TEAM)
	{
		pstTeam = z_idx_team(pstEnv, pstMon->iFirstAtkerID, pstMon->ullFirstAtkerWid);
		if(NULL == pstTeam)
		{
			goto _clear;
		}
	}

	return 0;

_clear:

	pstMon->iFirstAtkerID = 0;
	pstMon->cFirstAtkerType = 0;
	pstMon->ullFirstAtkerWid = 0;
	pstMon->iFirstAtkerMemID = 0;
	mon_booty_first_action(pstEnv, pstMon);
	
	return 0;
}

int mon_target_action(ZONESVRENV* pstEnv, Monster *pstMon)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	
	pstActionRes->ID = pstMon->iID;
	pstActionRes->ActionID = ACTION_TARGET;
	pstActionRes->Data.Target.TargetID = pstMon->stAtkTargets.iTargetCur;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_monview(pstEnv->pstAppCtx, pstEnv, pstMon, &stPkg, 0);
	
	return 0;
}

static int mon_die_xp_aasign(ZONESVRENV* pstEnv,Player *pstPlayer,int iTeam,Monster* pstMon,int iXp)
{
	CSPKG stPkg;
	CSACTIONRES *pstActRes = &stPkg.Body.ActionRes;
	ZoneTeam *pstZoneTeam;
	MONSTERDEF *pstMonDef = NULL;

	pstActRes->ID = pstPlayer->iMemID;
	pstActRes->ActionID = ACTION_GET_XP;
	pstActRes->Data.GetXp.ObjType = OBJ_MONSTER;
	pstActRes->Data.GetXp.ID = pstMon->iID;
	pstActRes->Data.GetXp.Xp = iXp;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);

	if (iTeam)
	{
		pstZoneTeam = player_team_get(pstEnv, pstPlayer);
		if(pstZoneTeam)
		{
			player_team_broadcast(pstEnv, pstZoneTeam, &stPkg);
		}
	}
	else
	{
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}	

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if(pstMonDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, 
			ZONEERR_BOOTY201, pstMonDef->Name, iXp);
	}

	player_add_xp(pstEnv, pstPlayer, iXp, 0, 0);

	return 0;
}

void mon_die_add_xp(ZONESVRENV* pstEnv, Monster* pstMon, MONSTERDEF *pstMonDef, ZoneAni *pstAtker)
{
	Player *pstPlayer = NULL;
	ROLEDATA *pstRoleData;
	ROLEFAIRY *pstRoleFairy = NULL;
	FAIRYDEF *pstFairyDef = NULL;
	int iRand;
	int iXp;
	unsigned int uiMaxXp ;
	ZoneTeam *pstTeam;
	Player *pstTeamPlayer;
	int i;
	
	pstPlayer = get_atker_player(pstEnv, pstAtker);
	if (!pstPlayer)
		return ;
	
	// 没精灵出征不加
	if (pstPlayer->stOnline.stFairyOnline.ullCampWID <= 0)
		return ;

	pstRoleFairy = get_pack_fairy(pstPlayer, pstPlayer->stOnline.stFairyOnline.ullCampWID);
	if (!pstRoleFairy)
		return ;

	pstFairyDef = faity_find(pstEnv, pstRoleFairy->FairyID, pstRoleFairy->FairyLevel);
	if (!pstFairyDef)
		return ;

	iRand =  (int)(RAND1(10000));
	if (iRand < pstFairyDef->GetXpRate *100)
	{
		// 获得XP = 怪物类型对应xp值 * (25 - ABS（怪物等级 - 玩家等级） / 25
		int iLvDiff = abs(pstMon->stMonDynAttr.Level-pstPlayer->stRoleData.Level);

		iXp = 	1.0*pstMonDef->BootyXp*(25-iLvDiff)/25;
		if (iXp < 0)
			iXp = 0;
		
		pstTeam = player_team_get(pstEnv, pstPlayer);
		if (!pstTeam)
		{
			pstRoleData = &pstPlayer->stRoleData;
			uiMaxXp = pstPlayer->stOnline.stDyn.stMidAttrList.uiXpMax;
			//宠物天赋,精灵xp
			pet_native_skill_result(pstEnv,pstPlayer,RESULT_NATIVE_XP_ADD_MAP, &iXp,NULL);
			if (	iXp > 0 &&
				pstRoleData->RoleDetail.XP < uiMaxXp &&
				!(pstPlayer->stOnline.cMiscFlag&MISC_FLAG_WUSHUANG))
			{
				mon_die_xp_aasign(pstEnv,pstPlayer,0,pstMon,iXp);
			}
		}
		else
		{
			for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
			{
				int iAddXp;
				pstTeamPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
				if (!pstTeamPlayer )
					continue;
				if (pstTeamPlayer->stOnline.stFairyOnline.ullCampWID <= 0)
					continue;

				if(0 > mon_booty_valid_player(pstMon, pstTeamPlayer))
				{
					continue;
				}

				pstRoleData = &pstTeamPlayer->stRoleData;
				uiMaxXp = pstTeamPlayer->stOnline.stDyn.stMidAttrList.uiXpMax;
				
				iAddXp = (1+iStatExp[pstTeam->stTeamInfo.MemberNum]/100.0)*(iXp*1.0f/pstTeam->stTeamInfo.MemberNum);
				//宠物天赋,精灵xp
				pet_native_skill_result(pstEnv,pstTeamPlayer,RESULT_NATIVE_XP_ADD_MAP, &iAddXp,NULL);
				if (	iAddXp > 0 &&
					pstRoleData->RoleDetail.XP < uiMaxXp &&
					!(pstTeamPlayer->stOnline.cMiscFlag&MISC_FLAG_WUSHUANG))
				{
					mon_die_xp_aasign(pstEnv,pstTeamPlayer,1,pstMon,iAddXp);
				}
				
			}
		}
		
	}
	return ;
}

int dropitem_get_stay_time(ZONESVRENV* pstEnv, DropItem *pstDropItem)
{
	ITEMDEF *pstItemDef;
	int iTime = 0;

	pstItemDef = z_find_itemdef(pstEnv, pstDropItem->iDefIdx);
	if (NULL == pstItemDef)
	{
		return -1;
	}

	switch(pstItemDef->Important)
	{
		case 0:
			iTime = 40;
			break;
		case 1:
			iTime = 70;
			break;
		case 2:
			iTime = 120;
			break;
		case 3:
			iTime = 150;
			break;
		case 4:
			iTime = 180;
			break;
		default:
			iTime = 180;
			break;
	}

	return iTime;
}

int dropitem_get_pick_limit_time(ZONESVRENV* pstEnv, DropItem *pstDropItem)
{
	ITEMDEF *pstItemDef;
	int iTime = 0;

	pstItemDef = z_find_itemdef(pstEnv, pstDropItem->iDefIdx);
	if (NULL == pstItemDef)
	{
		return -1;
	}

	switch(pstItemDef->Important)
	{
		case 0:
			iTime = 30;
			break;
		case 1:
			iTime = 60;
			break;
		case 2:
			iTime = 90;
			break;
		case 3:
			iTime = 120;
			break;
		case 4:
			iTime = 150;
			break;
		default:
			iTime = 150;
			break;
	}

	return iTime;
}

int check_team_booty_roll(ZONESVRENV* pstEnv, Player *pstPlayer,ZoneTeam *pstZoneTeam)
{
	int j,iDyn,k,iID, iIndex;
	char cType;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	AREADYN* pstAreaDyn;
	DropItem *pstDropItem;

	
	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst )
	{
		return 0;
	}

	for (j=0; j<pstMapInst->iAreaDyn; j++)
	{
		iDyn	=	pstMapInst->aiAreaDyn[j];

		if( iDyn<0)
		{
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		for(k=0; k<pstAreaDyn->iInanimate; k++)
		{
			if( k>=MAX_AREA_ANIMATE )
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "map %d dyn %d iInanimate=%d", pstMapInst->iMapInstID, iDyn, pstAreaDyn->iInanimate);
				break;
			}

			iID	=	pstAreaDyn->aiInanimate[k];
			iIndex	=	z_id_indextype(pstEnv->pstAppCtx, pstEnv, iID, &cType);

			if( -1==iIndex )
			{
				continue;
			}

			switch(cType)
			{
				case OBJ_ITEM:
					pstDropItem = (DropItem *)tmempool_get(pstEnv->pstItemPool, iIndex);
					if (!pstDropItem)
					{
						break;
					}
					if(pstDropItem->stObj.stBootyGrid.bRoll > ITEM_ROLL_NONE &&
						pstDropItem->stObj.stBootyGrid.ullTeamID == pstZoneTeam->stTeamInfo.TeamID)
					{
						return 1;
					}
					break;
				default:
					break;
			}
		}
	}
			
	return 0;
}

