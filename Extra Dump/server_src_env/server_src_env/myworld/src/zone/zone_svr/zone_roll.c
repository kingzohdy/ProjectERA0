/*
**  @file $RCSfile: zone_roll.c,v $
**  general description of this module
**  $Id: zone_roll.c,v 1.50 2014/02/20 03:49:08 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/02/20 03:49:08 $
**  @version $Revision: 1.50 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "mapmask.h"
#include "tconnapi.h"
#include "zone_status.h"
#include "zone_package.h"
#include "zone_clt.h"
#include "zone_move.h"
#include "zone_team.h"
#include "zone_player.h"
#include "zone_roll.h"
#include "zone_map.h"
#include "zone_npc.h"
#include "zone_oplog.h"
#include "zone_spansvr.h"
#include "zone_clan.h"
#include "zone_booty.h"
#include "zone_err.h"
#include "zone_zh_name.h"

int map_roll_booty_dropitem_update(ZONESVRENV* pstEnv, DropItem *pstDropItem)
{
	int i, j;
	SearchArea stSArea;
	AREADYN *pstArea;
	Player *pstPlayer;
	ZONEMAPINST* pstMapInst;
	int iPlayerNum = 0;
	Player *apstPlayer[MAX_PLAYER];

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstDropItem->stMap);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	
	if (z_pos_search_area(pstEnv->pstAppCtx, pstEnv, pstMapInst, &pstDropItem->stPos, &stSArea) < 0) 
	{
		return -1;
	}
		
	for (i = 0; i < stSArea.iNum; i++) 
	{
		pstArea = stSArea.pastArea[i];
		for (j = 0; j < pstArea->iAnimate; j++) 
		{
			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstArea->aiAnimate[j]);
			if (NULL == pstPlayer)
			{
				continue;
			}

			if (!z_pos_in_view(&pstPlayer->stRoleData.Pos, &pstDropItem->stPos))
			{
				continue;
			}

			if(0 == player_view_booty_valid(pstEnv, pstDropItem, pstPlayer))
			{
				continue;
			}

			apstPlayer[iPlayerNum++] = pstPlayer;
		}
	}

	if(iPlayerNum > 0)
	{
		CSPKG stPkg;
		CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

		pstActionRes->ID = pstDropItem->iID;
		pstActionRes->ActionID = ACTION_DROP_ITEM_UPDATE;

		pstActionRes->Data.DropItemUpdate.PickLimitTime = 0;
		if(pstDropItem->tPickLimitTime > pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			pstActionRes->Data.DropItemUpdate.PickLimitTime = 
				pstDropItem->tPickLimitTime -pstEnv->pstAppCtx->stCurr.tv_sec;
		}
		
		pstActionRes->Data.DropItemUpdate.OwnerRoleID = pstDropItem->stObj.stBootyGrid.ullRoleID;
		pstActionRes->Data.DropItemUpdate.Roll = pstDropItem->stObj.stBootyGrid.bRoll;

		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iPlayerNum, &stPkg);
	}

	return 0;
}

TeamBootyInfo* team_roll_booty_info_get(ZoneTeam *pstTeam, int iBootyID)
{
	int i;
	TeamBootyInfo *pstTeamBootyInfo;

	for(i = 0; i<pstTeam->iTeamBootyNum; i++)
	{
		pstTeamBootyInfo = &pstTeam->astTeamBooty[i];

		if(pstTeamBootyInfo->iBootyID == iBootyID)
		{
			return pstTeamBootyInfo;
		}
	}

	return NULL;
}

int team_roll_booty_wait_roller_del(TeamBootyInfo *pstTeamBootyInfo, Player *pstPlayer)
{
	int i, iRet = -1;
	TeamBootyRollInfo *pstRollInfo = &pstTeamBootyInfo->stBootyRollInfo;

	for(i=pstRollInfo->iWaitNum-1; i>=0; i--)
	{
		if(pstRollInfo->iWaitRoller[i] == pstPlayer->iMemID)
		{
			pstRollInfo->iWaitNum--;
			if(i != pstRollInfo->iWaitNum)
			{
				memcpy(&pstRollInfo->iWaitRoller[i], &pstRollInfo->iWaitRoller[pstRollInfo->iWaitNum], 
					sizeof(pstRollInfo->iWaitRoller[0]));
			}

			iRet = 0;
		}
	}

	return iRet;
}

// roll 提示信息，iRoll:  0 奖励提示信息,  >0 需求点数提示信息, -1 放弃需求
int team_roll_booty_notify(ZONESVRENV* pstEnv, ZoneTeam *pstTeam, DropItem *pstDropItem, char *pszName, int iRoll)
{
	CSPKG stPkg;
	ROLEGRID *pstRoleGrid = &pstDropItem->stObj.stBootyGrid.stGrid;
	ITEMDEF *pstItemDef;

	//需求
	if(iRoll > 0)
	{
		if(pstRoleGrid->Type == GRID_ITEM_ITEM)
		{
			z_fill_link_msg(&stPkg, SYS_TEAM, CHAT_LINK_ITEM, (void *)&pstRoleGrid->GridData.RoleItem,
				ZONEERR_LINK308, pszName, iRoll);
		}
		else
		{
			z_fill_link_msg(&stPkg, SYS_TEAM, CHAT_LINK_ARM, (void *)&pstRoleGrid->GridData.RoleArm,
				ZONEERR_LINK308, pszName, iRoll);
		}
	}
	//奖励提示
	else if(iRoll == 0)
	{
		if(pstRoleGrid->Type == GRID_ITEM_ITEM)
		{
			z_fill_link_msg(&stPkg, SYS_TEAM, CHAT_LINK_ITEM, (void *)&pstRoleGrid->GridData.RoleItem,
				ZONEERR_LINK309, pszName);

			pstItemDef = z_find_itemdef( pstEnv, pstRoleGrid->GridData.RoleItem.DefID);

			if (pstItemDef)
			{
				package_add_msg_show(pstEnv,pszName, pstItemDef,pstRoleGrid);
			}
		}
		else
		{
			z_fill_link_msg(&stPkg, SYS_TEAM, CHAT_LINK_ARM, (void *)&pstRoleGrid->GridData.RoleArm,
				ZONEERR_LINK309, pszName);

			pstItemDef = z_find_itemdef( pstEnv, pstRoleGrid->GridData.RoleArm.DefID);
			
			if (pstItemDef && pstRoleGrid->Type == GRID_ITEM_ARM && pstItemDef->Important >= 5)
			{
				CSPKG stPkgLink;
				z_fill_link_msg(&stPkgLink, SYS_SHOW, CHAT_LINK_ARM, 
								(void *)&pstRoleGrid->GridData.RoleArm, 
								ZONEERR_PLAYER804,pszName);

				if ( pstEnv->pstConf->CountryVer != COUNTRY_VER_KOREA)
				{
					z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkgLink);
				}
			}
		}
	}
	//放弃提示
	else
	{
		if(pstRoleGrid->Type == GRID_ITEM_ITEM)
		{
			z_fill_link_msg(&stPkg, SYS_TEAM, CHAT_LINK_ITEM, (void *)&pstRoleGrid->GridData.RoleItem,
				ZONEERR_LINK310, pszName);
		}
		else
		{
			z_fill_link_msg(&stPkg, SYS_TEAM, CHAT_LINK_ARM, (void *)&pstRoleGrid->GridData.RoleArm,
				ZONEERR_LINK310, pszName);
		}
	}

	player_team_broadcast(pstEnv, pstTeam, &stPkg);
	
	return 0;
}

int team_roll_booty_info_del(ZoneTeam *pstTeam, TeamBootyInfo *pstTeamBootyInfo)
{
	int i;
	
	for(i = pstTeam->iTeamBootyNum-1; i>=0; i--)
	{
		if(pstTeamBootyInfo->iBootyID != pstTeam->astTeamBooty[i].iBootyID)
		{
			continue;
		}

		pstTeam->iTeamBootyNum--;
		if(i != pstTeam->iTeamBootyNum)
		{
			memcpy(&pstTeam->astTeamBooty[i], &pstTeam->astTeamBooty[pstTeam->iTeamBootyNum], 
					sizeof(pstTeam->astTeamBooty[0]));
		}
	}
	
	return 0;
}

int team_roll_booty_valid_player(DropItem *pstRollItem, Player *pstPlayer)
{
	if (NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus || 
		pstPlayer->stRoleData.Map != pstRollItem->stMap.iID)
	{
		return -1;
	}
	
	if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE && !z_pos_in_view(&pstRollItem->stPos, &pstPlayer->stRoleData.Pos))
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

int team_roll_booty_data(ZONESVRENV* pstEnv, DropItem *pstRollItem, ZoneTeam *pstTeam, TeamBootyInfo *pstTeamBootyInfo)
{
	int i, iPlayerNum = 0;
	Player *pstPlayerTmp;
	Player *apstPlayer[MAX_RIDE_PASSENGER];
	CSPKG stPkg;
	CSROLLSVR *pstRollSvr = &stPkg.Body.RollSvr;
	TeamBootyRollInfo *pstBootyRollInfo = &pstTeamBootyInfo->stBootyRollInfo;
	
	memset(pstTeamBootyInfo, 0, sizeof(*pstTeamBootyInfo));
	pstTeamBootyInfo->iBootyID = pstRollItem->iID;
	pstTeamBootyInfo->tEndTime = pstEnv->pstAppCtx->stCurr.tv_sec + TEAM_ROLL_BOOTY_TIME;

	//筛选有效角色
	for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		pstPlayerTmp = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
		if (NULL == pstPlayerTmp || 0 > team_roll_booty_valid_player(pstRollItem, pstPlayerTmp))
		{
			continue;
		}

		if(pstBootyRollInfo->iWaitNum >= MAX_TEAM_MEMBER)
		{
			break;
		}
		
		pstBootyRollInfo->iWaitRoller[pstBootyRollInfo->iWaitNum] =  pstPlayerTmp->iMemID;
		pstBootyRollInfo->iWaitNum++;

		apstPlayer[iPlayerNum++] = pstPlayerTmp;
	}

	pstRollSvr->Type = ROLL_SVR_UI;
	pstRollSvr->Data.RollUIInfo.BootyID = pstRollItem->iID;
	pstRollSvr->Data.RollUIInfo.BootyInfo = pstRollItem->stObj.stBootyGrid.stGrid;
	pstRollSvr->Data.RollUIInfo.EndTime = TEAM_ROLL_BOOTY_TIME;

	Z_CSHEAD_INIT(&stPkg.Head, ROLL_SVR);
	z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iPlayerNum, &stPkg);

	
	return 0;
}

//拾取需要ROLL点的道具，开始roll点
int team_roll_booty_start(ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstRollItem)
{
	TeamBootyInfo *pstTeamBootyInfo;
	ZoneTeam *pstTeam;

	if(pstRollItem->tPickLimitTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return -1;
	}
	
	if(!is_wait_roll_drop_item(pstRollItem))
	{
		return -1;
	}

	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(NULL == pstTeam || pstTeam->iTeamBootyNum >= MAX_ROLL_TEAM_BOOTY)
	{
		//需要说点什么不?
		return -1;
	}
	
	pstTeamBootyInfo = &pstTeam->astTeamBooty[pstTeam->iTeamBootyNum];
	pstTeam->iTeamBootyNum++;

	team_roll_booty_data(pstEnv, pstRollItem, pstTeam, pstTeamBootyInfo);

	//更新物品时间等信息
	pstRollItem->tPickLimitTime = pstEnv->pstAppCtx->stCurr.tv_sec + TEAM_ROLL_BOOTY_TIME + 5;
	pstRollItem->iStayTime = DROP_ITEM_TIME;
	pstRollItem->stDropTime = pstEnv->pstAppCtx->stCurr;
	pstRollItem->stObj.stBootyGrid.bRoll = ITEM_ROLL_ROLLING;
	map_roll_booty_dropitem_update(pstEnv, pstRollItem);

	return 0;
}

//放身上
int team_roll_booty_assign_package(ZONESVRENV* pstEnv, ZoneTeam *pstTeam, TeamBootyInfo *pstTeamBootyInfo, 
								DropItem *pstDropItem)
{
	Player *pstPlayer;
	ROLEGRID *pstRoleGrid = &pstDropItem->stObj.stBootyGrid.stGrid;
	ROLEGRID stDummy;
	
	pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeamBootyInfo->stBootyRollInfo.szWinRoleName);
	if(NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus )
	{
		return -1;
	}
	
	if (package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRoleGrid, 1,0) < 0)
	{
		return -1;
	}

	// OP LOG
	memcpy(&stDummy, pstRoleGrid, sizeof(stDummy));
	
	package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRoleGrid, 1,0, IMPITEM_OPTYPE_PICK);

	z_role_pickitem_oplog( pstEnv, pstPlayer, &stDummy, 1, OP_ASSIGN);

	//Roll到了提示信息
	team_roll_booty_notify(pstEnv, pstTeam, pstDropItem, pstPlayer->stRoleData.RoleName, 0);

	//清理
	z_item_clean(pstEnv, pstDropItem);
	return 0;
}

//分赃
int team_roll_booty_assign(ZONESVRENV* pstEnv, ZoneTeam *pstTeam,
								DropItem *pstDropItem, TeamBootyInfo *pstTeamBootyInfo)
{
	int iPickLimitTime = 0;
	int iStayTime = 60;
	ZoneBootyGrid *pstBootyGrid = &pstDropItem->stObj.stBootyGrid;

	//归属权
	pstBootyGrid->ullRoleID = pstTeamBootyInfo->stBootyRollInfo.ullWinRoleID;
	
	if(pstBootyGrid->ullRoleID > 0)
	{
		iStayTime = DROP_ITEM_TIME;
		iPickLimitTime = DROP_ITEM_PICK_LIMIT_TIME;

		//先放背包里里面，放不下丢地上
		if( 0 == team_roll_booty_assign_package(pstEnv, pstTeam, pstTeamBootyInfo, pstDropItem))
		{
			return 0;
		}
	}
	
	//更新一下时间
	pstDropItem->tPickLimitTime = pstEnv->pstAppCtx->stCurr.tv_sec + iPickLimitTime;
	pstDropItem->iStayTime = iStayTime;
	pstDropItem->stDropTime = pstEnv->pstAppCtx->stCurr;
	pstDropItem->stObj.stBootyGrid.bRoll = ITEM_ROLL_NONE;
	map_roll_booty_dropitem_update(pstEnv, pstDropItem);
	
	//Roll到了提示信息
	if(pstBootyGrid->ullRoleID > 0)
	{
		team_roll_booty_notify(pstEnv, pstTeam, pstDropItem, pstTeamBootyInfo->stBootyRollInfo.szWinRoleName, 0);
	}
	
	return 0;
}

int team_booty_active_one(ZONESVRENV* pstEnv, ZoneTeam *pstTeam, TeamBootyInfo *pstTeamBootyInfo)
{
	DropItem *pstDropItem;

	pstDropItem = z_id_dropitem(pstEnv->pstAppCtx, pstEnv, pstTeamBootyInfo->iBootyID);
	if (NULL == pstDropItem)
	{
		team_roll_booty_info_del(pstTeam, pstTeamBootyInfo);
		return -1;
	}
	
	if(pstTeamBootyInfo->tEndTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		goto _final;
	}

	if(pstTeamBootyInfo->stBootyRollInfo.iWaitNum <= 0)
	{
		goto _final;
	}

	return 0;

_final:

	//分配战利品
	team_roll_booty_assign(pstEnv, pstTeam, pstDropItem, pstTeamBootyInfo);

	team_roll_booty_info_del(pstTeam, pstTeamBootyInfo);
	
	return 0;

}

int team_booty_active(ZONESVRENV* pstEnv, ZoneTeam *pstTeam)
{
	int i;
	TeamBootyInfo *pstTeamBootyInfo;
	
	for(i = pstTeam->iTeamBootyNum-1; i>=0; i--)
	{
		pstTeamBootyInfo = &pstTeam->astTeamBooty[i];

		//处理完的会删除
		team_booty_active_one(pstEnv, pstTeam, pstTeamBootyInfo);
	}

	return 0;
}

int team_booty_player_exit(ZONESVRENV* pstEnv, ZoneTeam *pstTeam, Player *pstPlayer)
{
	int i;
	TeamBootyInfo *pstTeamBootyInfo;
	
	for(i = pstTeam->iTeamBootyNum-1; i>=0; i--)
	{
		pstTeamBootyInfo = &pstTeam->astTeamBooty[i];
		
		team_roll_booty_wait_roller_del(pstTeamBootyInfo, pstPlayer);
	}
	UNUSED(pstEnv);
	return 0;
}

int team_booty_destroy(ZONESVRENV* pstEnv, ZoneTeam *pstTeam)
{
	int i;
	DropItem *pstDropItem;
	TeamBootyInfo *pstTeamBootyInfo;
	
	for(i = pstTeam->iTeamBootyNum-1; i>=0; i--)
	{
		pstTeamBootyInfo = &pstTeam->astTeamBooty[i];

		pstDropItem = z_id_dropitem(pstEnv->pstAppCtx, pstEnv, pstTeamBootyInfo->iBootyID);
		if (NULL == pstDropItem)
		{
			team_roll_booty_info_del(pstTeam, pstTeamBootyInfo);
			continue;
		}
		else
		{
			team_roll_booty_assign(pstEnv, pstTeam, pstDropItem, pstTeamBootyInfo);
			team_roll_booty_info_del(pstTeam, pstTeamBootyInfo);
		}	
	}

	return 0;
}

ClanBootyInfo* clan_roll_booty_info_get(ZoneClan *pstClan, int iBootyID)
{
	int i;
	ClanBootyInfo *pstClanBootyInfo;

	for(i = 0; i<pstClan->stZoneClanInfo.iClanBootyNum; i++)
	{
		pstClanBootyInfo = &pstClan->stZoneClanInfo.astClanBooty[i];

		if(pstClanBootyInfo->iBootyID == iBootyID)
		{
			return pstClanBootyInfo;
		}
	}

	return NULL;
}

int clan_roll_booty_info_del(ZoneClan *pstClan, ClanBootyInfo *pstClanBootyInfo)
{
	int i;
	
	for(i = pstClan->stZoneClanInfo.iClanBootyNum-1; i>=0; i--)
	{
		if(pstClanBootyInfo->iBootyID != pstClan->stZoneClanInfo.astClanBooty[i].iBootyID)
		{
			continue;
		}

		pstClan->stZoneClanInfo.iClanBootyNum--;
		if(i != pstClan->stZoneClanInfo.iClanBootyNum)
		{
			memcpy(&pstClan->stZoneClanInfo.astClanBooty[i], 
				&pstClan->stZoneClanInfo.astClanBooty[pstClan->stZoneClanInfo.iClanBootyNum], 
				sizeof(pstClan->stZoneClanInfo.astClanBooty[0]));
		}
	}
	
	return 0;
}

int clan_roll_booty_wait_roller_set(ClanBootyInfo *pstClanBootyInfo, Player *pstPlayer)
{
	int i;
	ClanBootyRollInfo *pstRollInfo = &pstClanBootyInfo->stBootyRollInfo;

	for(i=pstRollInfo->iWaitNum-1; i>=0; i--)
	{
		if(pstRollInfo->astClanBootyWaiterInfo[i].iID == pstPlayer->iMemID)
		{
			if(pstRollInfo->astClanBootyWaiterInfo[i].iRoll == 0)
			{
				pstRollInfo->astClanBootyWaiterInfo[i].iRoll = 1;
				return 0;
			}
			
			break;
		}
	}

	return -1;
}

int clan_roll_waiter_fini(ZoneClan *pstClan, ClanBootyInfo *pstClanBootyInfo)
{
	int i;

	for(i=0; i<pstClanBootyInfo->stBootyRollInfo.iWaitNum; i++)
	{
		if(pstClanBootyInfo->stBootyRollInfo.astClanBootyWaiterInfo[i].iRoll == 0)
		{
			return 0;
		}
	}
	
	return 1;
}

int clan_roll_waiter_broadcast(ZONESVRENV* pstEnv, ClanBootyInfo *pstClanBootyInfo, CSPKG *pstPkg)
{
	int i;
	Player *pstPlayer;
	Player *apstPlayer[MAX_TEAM_MEMBER];
	int iOnlineNum = 0;
	
	for (i=pstClanBootyInfo->stBootyRollInfo.iWaitNum-1; i>=0; i--)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstClanBootyInfo->stBootyRollInfo.astClanBootyWaiterInfo[i].iID);
		if (pstPlayer)
		{
			apstPlayer[iOnlineNum++] = pstPlayer;
		}
	}

	if (iOnlineNum > 0)
	{
		z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iOnlineNum, pstPkg);
	}
	
	return 0;
}

// roll 提示信息，iRoll:  0 奖励提示信息,  >0 需求点数提示信息, -1 放弃需求
int clan_roll_booty_notify(ZONESVRENV* pstEnv, ClanBootyInfo *pstClanBootyInfo, DropItem *pstDropItem, char *pszName, int iRoll)
{
	CSPKG stPkg;
	ROLEGRID *pstRoleGrid = &pstDropItem->stObj.stBootyGrid.stGrid;

	//需求
	if(iRoll > 0)
	{
		if(pstRoleGrid->Type == GRID_ITEM_ITEM)
		{
			z_fill_link_msg(&stPkg, SYS_CLAN, CHAT_LINK_ITEM, (void *)&pstRoleGrid->GridData.RoleItem,
					ZONEERR_LINK308, pszName, iRoll);
		}
		else
		{
			z_fill_link_msg(&stPkg, SYS_CLAN, CHAT_LINK_ARM, (void *)&pstRoleGrid->GridData.RoleArm,
					ZONEERR_LINK308, pszName, iRoll);
		}
	}
	//奖励提示
	else if(iRoll == 0)
	{
		if(pstRoleGrid->Type == GRID_ITEM_ITEM)
		{
			z_fill_link_msg(&stPkg, SYS_CLAN, CHAT_LINK_ITEM, (void *)&pstRoleGrid->GridData.RoleItem,
					ZONEERR_LINK309, pszName);
		}
		else
		{
			z_fill_link_msg(&stPkg, SYS_CLAN, CHAT_LINK_ARM, (void *)&pstRoleGrid->GridData.RoleArm,
					ZONEERR_LINK309, pszName);
		}
	}
	//放弃提示
	else
	{
		if(pstRoleGrid->Type == GRID_ITEM_ITEM)
		{
			z_fill_link_msg(&stPkg, SYS_CLAN, CHAT_LINK_ITEM, (void *)&pstRoleGrid->GridData.RoleItem,
					ZONEERR_LINK310, pszName);
		}
		else
		{
			z_fill_link_msg(&stPkg, SYS_CLAN, CHAT_LINK_ARM, (void *)&pstRoleGrid->GridData.RoleArm,
					ZONEERR_LINK310, pszName);
		}
	}

	clan_roll_waiter_broadcast(pstEnv, pstClanBootyInfo, &stPkg);
	
	return 0;
}

//放身上
int clan_roll_booty_assign_package(ZONESVRENV* pstEnv, ClanBootyInfo *pstClanBootyInfo, 
								DropItem *pstDropItem)
{
	Player *pstPlayer;
	ROLEGRID *pstRoleGrid = &pstDropItem->stObj.stBootyGrid.stGrid;
	ROLEGRID stDummy;
	
	pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstClanBootyInfo->stBootyRollInfo.szWinRoleName);
	if(NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus )
	{
		return -1;
	}
	
	if (package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRoleGrid, 1,0) < 0)
	{
		return -1;
	}

	// OP LOG
	memcpy(&stDummy, pstRoleGrid, sizeof(stDummy));
	
	package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRoleGrid, 1,0, IMPITEM_OPTYPE_PICK);

	z_role_pickitem_oplog( pstEnv, pstPlayer, &stDummy, 1, OP_ASSIGN);

	//Roll到了提示信息
	clan_roll_booty_notify(pstEnv, pstClanBootyInfo, pstDropItem, pstPlayer->stRoleData.RoleName, 0);

	//清理
	z_item_clean(pstEnv, pstDropItem);
	return 0;
}

//分赃
int clan_roll_booty_assign(ZONESVRENV* pstEnv, ZoneClan *pstClan,
								DropItem *pstDropItem, ClanBootyInfo *pstClanBootyInfo)
{
	int iPickLimitTime = DROP_ITEM_PICK_LIMIT_TIME;
	int iStayTime = DROP_ITEM_PICK_LIMIT_TIME;
	ZoneBootyGrid *pstBootyGrid = &pstDropItem->stObj.stBootyGrid;

	//归属权
	pstBootyGrid->ullRoleID = pstClanBootyInfo->stBootyRollInfo.ullWinRoleID;
	
	if(pstBootyGrid->ullRoleID > 0)
	{
		//先放背包里里面，放不下丢地上
		if( 0 == clan_roll_booty_assign_package(pstEnv, pstClanBootyInfo, pstDropItem))
		{
			return 0;
		}
	}
	
	//更新一下时间
	pstDropItem->tPickLimitTime = pstEnv->pstAppCtx->stCurr.tv_sec + iPickLimitTime+5;
	pstDropItem->iStayTime = iStayTime;
	pstDropItem->stDropTime = pstEnv->pstAppCtx->stCurr;
	pstDropItem->stObj.stBootyGrid.bRoll = ITEM_ROLL_NONE;
	map_roll_booty_dropitem_update(pstEnv, pstDropItem);
	
	//Roll到了提示信息
	if(pstBootyGrid->ullRoleID > 0)
	{
		clan_roll_booty_notify(pstEnv, pstClanBootyInfo, pstDropItem, pstClanBootyInfo->stBootyRollInfo.szWinRoleName, 0);
	}
	
	return 0;
}

int clan_booty_active_one(ZONESVRENV* pstEnv, ZoneClan *pstClan, ClanBootyInfo *pstClanBootyInfo)
{
	DropItem *pstDropItem;

	pstDropItem = z_id_dropitem(pstEnv->pstAppCtx, pstEnv, pstClanBootyInfo->iBootyID);
	if (NULL == pstDropItem)
	{
		clan_roll_booty_info_del(pstClan, pstClanBootyInfo);
		return -1;
	}
	
	if(pstClanBootyInfo->tEndTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		goto _final;
	}

	if(clan_roll_waiter_fini(pstClan, pstClanBootyInfo))
	{
		goto _final;
	}

	return 0;

_final:

	//分配战利品
	clan_roll_booty_assign(pstEnv, pstClan, pstDropItem, pstClanBootyInfo);

	clan_roll_booty_info_del(pstClan, pstClanBootyInfo);
	
	return 0;

}

int clan_booty_active(ZONESVRENV* pstEnv, ZoneClan *pstClan)
{
	int i;
	ClanBootyInfo *pstClanBootyInfo;
	
	for(i = pstClan->stZoneClanInfo.iClanBootyNum-1; i>=0; i--)
	{
		pstClanBootyInfo = &pstClan->stZoneClanInfo.astClanBooty[i];

		//处理完的会删除
		clan_booty_active_one(pstEnv, pstClan, pstClanBootyInfo);
	}

	return 0;
}

int clan_roll_booty_data(ZONESVRENV* pstEnv, DropItem *pstRollItem, ZoneClan *pstClan, ClanBootyInfo *pstClanBootyInfo)
{
	int i, j;
	Player *pstPlayerTmp;
	int iPlayerNum = 0;
	Player *apstPlayer[MAX_PLAYER];
	ZONEMAPINST *pstMapInst;
	SearchArea stSArea;
	AREADYN *pstArea;
	CSPKG stPkg;
	CSROLLSVR *pstRollSvr = &stPkg.Body.RollSvr;
	ClanBootyRollInfo *pstBootyRollInfo = &pstClanBootyInfo->stBootyRollInfo;

	memset(pstClanBootyInfo, 0, sizeof(*pstClanBootyInfo));
	pstClanBootyInfo->iBootyID = pstRollItem->iID;
	pstClanBootyInfo->tEndTime = pstEnv->pstAppCtx->stCurr.tv_sec + CLAN_ROLL_BOOTY_TIME;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstRollItem->stMap);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	
	if (z_pos_search_area(pstEnv->pstAppCtx, pstEnv, pstMapInst, &pstRollItem->stPos, &stSArea) < 0) 
	{
		return -1;
	}
		
	for (i = 0; i < stSArea.iNum; i++) 
	{
		pstArea = stSArea.pastArea[i];
		for (j = 0; j < pstArea->iAnimate; j++) 
		{
			pstPlayerTmp = z_id_player(pstEnv->pstAppCtx, pstEnv, pstArea->aiAnimate[j]);
			if (NULL == pstPlayerTmp || pstPlayerTmp->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				continue;
			}

			if (!z_pos_in_view(&pstPlayerTmp->stRoleData.Pos, &pstRollItem->stPos))
			{
				continue;
			}

			if(0 == is_booty_grid_owner(pstPlayerTmp, pstRollItem))
			{
				continue;
			}

			pstBootyRollInfo->astClanBootyWaiterInfo[pstBootyRollInfo->iWaitNum].iID =  pstPlayerTmp->iMemID;
			pstBootyRollInfo->astClanBootyWaiterInfo[pstBootyRollInfo->iWaitNum].iRoll =  0;
			pstBootyRollInfo->iWaitNum++;
		
			apstPlayer[iPlayerNum++] = pstPlayerTmp;
		}
	}

	pstRollSvr->Type = ROLL_SVR_UI;
	pstRollSvr->Data.RollUIInfo.BootyID = pstRollItem->iID;
	pstRollSvr->Data.RollUIInfo.BootyInfo = pstRollItem->stObj.stBootyGrid.stGrid;
	pstRollSvr->Data.RollUIInfo.EndTime = CLAN_ROLL_BOOTY_TIME;

	Z_CSHEAD_INIT(&stPkg.Head, ROLL_SVR);
	z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iPlayerNum, &stPkg);
	
	return 0;
}

//拾取需要ROLL点的道具，开始roll点
int clan_roll_booty_start(ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstRollItem)
{
	ClanBootyInfo *pstClanBootyInfo;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);

	if(!pstClan || pstClan->stZoneClanInfo.iClanBootyNum >= MAX_ROLL_CLAN_BOOTY)
	{
		return -1;
	}
	
	pstClanBootyInfo = &pstClan->stZoneClanInfo.astClanBooty[pstClan->stZoneClanInfo.iClanBootyNum];
	pstClan->stZoneClanInfo.iClanBootyNum++;

	clan_roll_booty_data(pstEnv, pstRollItem, pstClan, pstClanBootyInfo);

	//更新物品时间等信息
	pstRollItem->tPickLimitTime = pstEnv->pstAppCtx->stCurr.tv_sec + CLAN_ROLL_BOOTY_TIME + 5;
	pstRollItem->iStayTime = CLAN_ROLL_BOOTY_TIME+2;
	pstRollItem->stDropTime = pstEnv->pstAppCtx->stCurr;
	pstRollItem->stObj.stBootyGrid.bRoll = ITEM_ROLL_ROLLING;
	map_roll_booty_dropitem_update(pstEnv, pstRollItem);

	return 0;
}

int player_roll_booty_start(ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstRollItem)
{
	if(pstRollItem->tPickLimitTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return -1;
	}
	
	if(!is_wait_roll_drop_item(pstRollItem))
	{
		return -1;
	}

	if(pstRollItem->stObj.stBootyGrid.ullClanGID > 0)
	{
		clan_roll_booty_start(pstEnv, pstPlayer, pstRollItem);
	}
	else
	{
		team_roll_booty_start(pstEnv, pstPlayer, pstRollItem);
	}
	
	return 0;
}

int player_do_team_roll(Player *pstPlayer, TeamBootyInfo *pstTeamBootyInfo)
{
	int iRoll;
	TeamBootyRollInfo *pstRollInfo = &pstTeamBootyInfo->stBootyRollInfo;

	//Roll 1~100
	iRoll = RAND1(100) + 1;

	//记录最大的，但是同样大的不顶替，谁先谁得
	if(iRoll > pstRollInfo->iMaxRoll)
	{
		pstRollInfo->iMaxRoll = iRoll;
		pstRollInfo->ullWinRoleID = pstPlayer->stRoleData.RoleID;
		STRNCPY(pstRollInfo->szWinRoleName, pstPlayer->stRoleData.RoleName, CCH(pstRollInfo->szWinRoleName));
	}

	return iRoll;
}

int player_team_roll_need(ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstDropItem)
{
	int iRoll;
	ZoneTeam *pstTeam;
	TeamBootyInfo *pstTeamBootyInfo;

	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(NULL == pstTeam)
	{
		return -1;
	}

	pstTeamBootyInfo = team_roll_booty_info_get(pstTeam, pstDropItem->iID);
	if(NULL == pstTeamBootyInfo)
	{
		return -1;
	}

	//确保每人只Roll一次
	if(0 > team_roll_booty_wait_roller_del(pstTeamBootyInfo, pstPlayer))
	{
		return -1;
	}

	iRoll = player_do_team_roll(pstPlayer, pstTeamBootyInfo);
	//提示信息
	team_roll_booty_notify(pstEnv, pstTeam, pstDropItem, pstPlayer->stRoleData.RoleName, iRoll);

	//快速反应一下
	team_booty_active_one(pstEnv, pstTeam, pstTeamBootyInfo);
	
	return 0;
}

int player_do_clan_roll(Player *pstPlayer, ClanBootyInfo *pstClanBootyInfo)
{
	int iRoll;
	ClanBootyRollInfo *pstRollInfo = &pstClanBootyInfo->stBootyRollInfo;

	//Roll 1~100
	iRoll = RAND1(100) + 1;

	//记录最大的，但是同样大的不顶替，谁先谁得
	if(iRoll > pstRollInfo->iMaxRoll)
	{
		pstRollInfo->iMaxRoll = iRoll;
		pstRollInfo->ullWinRoleID = pstPlayer->stRoleData.RoleID;
		STRNCPY(pstRollInfo->szWinRoleName, pstPlayer->stRoleData.RoleName, CCH(pstRollInfo->szWinRoleName));
	}

	return iRoll;
}

int player_clan_roll_need(ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstDropItem)
{
	int iRoll;
	ZoneClan *pstClan;
	ClanBootyInfo *pstClanBootyInfo;

	pstClan = player_get_clan(pstEnv, pstPlayer);
	if(NULL == pstClan)
	{
		return -1;
	}

	pstClanBootyInfo = clan_roll_booty_info_get(pstClan, pstDropItem->iID);
	if(NULL == pstClanBootyInfo)
	{
		return -1;
	}

	if(0 > clan_roll_booty_wait_roller_set(pstClanBootyInfo, pstPlayer))
	{
		return -1;
	}

	iRoll = player_do_clan_roll(pstPlayer, pstClanBootyInfo);

	//提示信息
	clan_roll_booty_notify(pstEnv, pstClanBootyInfo, pstDropItem, pstPlayer->stRoleData.RoleName, iRoll);

	//快速反应一下
	clan_booty_active_one(pstEnv, pstClan, pstClanBootyInfo);
	
	return 0;
}

//需求 ROLL
int player_roll_need(ZONESVRENV* pstEnv, Player *pstPlayer, CSROLLCLT *pstRoleClt)
{
	DropItem *pstDropItem;

	pstDropItem = z_id_dropitem(pstEnv->pstAppCtx, pstEnv, pstRoleClt->BootyID);
	if (NULL == pstDropItem)
	{
		return -1;
	}

	if(pstDropItem->stObj.stBootyGrid.ullClanGID > 0)
	{
		player_clan_roll_need(pstEnv, pstPlayer,  pstDropItem);
	}
	else
	{
		player_team_roll_need(pstEnv, pstPlayer, pstDropItem);
	}
	
	return 0;
}

int player_team_roll_quit(ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstDropItem)
{
	ZoneTeam *pstTeam;
	TeamBootyInfo *pstTeamBootyInfo;

	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(NULL == pstTeam)
	{
		return -1;
	}
	
	pstTeamBootyInfo = team_roll_booty_info_get(pstTeam, pstDropItem->iID);
	if(NULL == pstTeamBootyInfo)
	{
		return -1;
	}

	//确保每人只Roll一次
	if(0 > team_roll_booty_wait_roller_del(pstTeamBootyInfo, pstPlayer))
	{
		return -1;
	}

	//提示信息
	team_roll_booty_notify(pstEnv, pstTeam, pstDropItem, pstPlayer->stRoleData.RoleName, -1);
	return 0;
}

int player_clan_roll_quit(ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstDropItem)
{
	ZoneClan *pstClan;
	ClanBootyInfo *pstClanBootyInfo;

	pstClan = player_get_clan(pstEnv, pstPlayer);
	if(NULL == pstClan)
	{
		return -1;
	}

	pstClanBootyInfo = clan_roll_booty_info_get(pstClan, pstDropItem->iID);
	if(NULL == pstClanBootyInfo)
	{
		return -1;
	}

	if(0 > clan_roll_booty_wait_roller_set(pstClanBootyInfo, pstPlayer))
	{
		return -1;
	}

	//提示信息
	clan_roll_booty_notify(pstEnv, pstClanBootyInfo, pstDropItem, pstPlayer->stRoleData.RoleName, -1);
	return 0;
}

int player_roll_quit(ZONESVRENV* pstEnv, Player *pstPlayer, int iBootyID)
{
	DropItem *pstDropItem;
	
	pstDropItem = z_id_dropitem(pstEnv->pstAppCtx, pstEnv, iBootyID);
	if (NULL == pstDropItem)
	{
		return -1;
	}

	if(pstDropItem->stObj.stBootyGrid.ullClanGID > 0)
	{
		player_clan_roll_quit(pstEnv, pstPlayer,  pstDropItem);
	}
	else
	{
		player_team_roll_quit(pstEnv, pstPlayer, pstDropItem);
	}
	
	return 0;
}

int player_roll_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
			TFRAMEHEAD	 *pstFrameHead, CSPKG *pstCsPkg)
{	
	CSROLLCLT *pstClt = &pstCsPkg->Body.RollClt;
	UNUSED(pstFrameHead);
	UNUSED(pstAppCtx);

	switch(pstClt->Roll)
	{
		case ROLL_CLT_NEED:
			player_roll_need(pstEnv, pstPlayer, pstClt);
			break;
		case ROLL_CLT_QUIT:
			player_roll_quit(pstEnv, pstPlayer, pstClt->BootyID);
			break;
		default:
			return -1;
			break;
	}

	return 0;
}

int is_wait_roll_drop_item(DropItem *pstDropItem)
{
	if(pstDropItem->cType != DROP_ITEM_GRID || 
		pstDropItem->stObj.stBootyGrid.bRoll != ITEM_ROLL_WAIT)
	{
		return 0;
	}

	return 1;
}

int is_rolling_drop_item(DropItem *pstDropItem)
{
	if(pstDropItem->cType != DROP_ITEM_GRID || 
		pstDropItem->stObj.stBootyGrid.bRoll != ITEM_ROLL_ROLLING)
	{
		return 0;
	}

	return 1;
}

int is_collect_drop_item(DropItem *pstDropItem)
{
	if(pstDropItem->cType != DROP_ITEM_GRID || 
		pstDropItem->stObj.stBootyGrid.bCollectTime  <= 0)
	{
		return 0;
	}

	return 1;
}

int booty_mode_to_string(int iTeamBootyMode, char* pszBuff, int iSize)
{
	switch(iTeamBootyMode)
	{
		case TEAM_BOOTY_RAND:
			STRNCPY(pszBuff, LJY_BOOTY_RAND,iSize);
			break;
		case TEAM_BOOTY_ROLL:
			STRNCPY(pszBuff, LJY_BOOTY_ROLL,iSize);
			break;
		case TEAM_BOOTY_ALL:
			STRNCPY(pszBuff, LJY_BOOTY_ALL,iSize);
			break;
		default:
			return -1;
			break;
	}

	return 0;
}

int player_team_chg_booty_mode_notify(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneTeam *pstTeam)
{
	char szMsg[256];
	
	switch(pstTeam->stTeamInfo.TeamBootyMode)
	{
		case TEAM_BOOTY_RAND:
			sprintf(szMsg, LJY_TEAM10,
				pstPlayer->stRoleData.RoleName);
			break;
		case TEAM_BOOTY_ROLL:
			sprintf(szMsg, LJY_TEAM11,
				pstPlayer->stRoleData.RoleName);
			break;
		case TEAM_BOOTY_ALL:
			sprintf(szMsg, LJY_TEAM12,
				pstPlayer->stRoleData.RoleName);
			break;
		default:
			return -1;
			break;
	}

	player_team_broadcast_tip(pstEnv, pstTeam, szMsg);

	return 0;
}

int player_team_chg_booty_mode(ZONESVRENV* pstEnv, Player *pstPlayer, CSTEAMCHGBOOTYMODE*pstChg)
{
	ZoneTeam *pstTeam;
	TEAMMEMBER *pstTeamMember;
	CSPKG stPkg;
	CSTEAMSVR *pstTeamSvr = &stPkg.Body.TeamSvr;

	if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ROLL201 );
		return -1;
	}
	
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(NULL == pstTeam)
	{
		return -1;
	}

	pstTeamMember = player_team_get_captain(pstTeam);
	if(NULL == pstTeamMember)
	{
		return -1;
	}

	if (0 != strcmp(pstTeamMember->MemberName, pstPlayer->stRoleData.RoleName))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ROLL202 );
		return -1;
	}

	if(pstChg->TeamBootyMode < TEAM_BOOTY_ROLL || 
		pstChg->TeamBootyMode > TEAM_BOOTY_ALL)
	{
		return -1;
	}

	pstTeam->stTeamInfo.TeamBootyMode = pstChg->TeamBootyMode;

	pstTeamSvr->OpType = TEAM_SVR_BOOTY_MODE;
	pstTeamSvr->TeamSvrData.BootyModeInfo.TeamBootyMode = pstTeam->stTeamInfo.TeamBootyMode;

	Z_CSHEAD_INIT(&stPkg.Head, TEAM_SVR);
	player_team_broadcast(pstEnv, pstTeam, &stPkg);

	player_team_chg_booty_mode_notify(pstEnv, pstPlayer, pstTeam);

	/*
	//跨服野外要同步队伍掉落模式
	if( INST_MODE_ZONE_SVR == pstEnv->iInstMode &&
		pstTeam->BattleBusID == pstEnv->iBattlePvpID)
	{
		team_shadow_sync_zone(pstEnv, pstTeam, PLAYER_STATUS_BATTLE_LOGIN_READY, pstEnv->iBattlePvpID);
	}
	*/
	
	return 0;
}

int team_booty_mode_chg(ZONESVRENV* pstEnv, ZoneTeam *pstTeam, int iMode)
{
	CSPKG stPkg;
	CSTEAMSVR *pstTeamSvr = &stPkg.Body.TeamSvr;

	pstTeam->stTeamInfo.TeamBootyMode = iMode;

	pstTeamSvr->OpType = TEAM_SVR_BOOTY_MODE;
	pstTeamSvr->TeamSvrData.BootyModeInfo.TeamBootyMode = pstTeam->stTeamInfo.TeamBootyMode;

	Z_CSHEAD_INIT(&stPkg.Head, TEAM_SVR);
	player_team_broadcast(pstEnv, pstTeam, &stPkg);

	return 0;
}

