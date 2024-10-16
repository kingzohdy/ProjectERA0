/*
**  @file $RCSfile: zone_clan_city.c,v $
**  general description of this module
**  $Id: zone_clan_city.c,v 1.122 2014/03/04 07:45:22 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2014/03/04 07:45:22 $
**  @version $Revision: 1.122 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_clan_city.h"
#include "zone_clan_construction.h"
#include "zone_map.h"
#include "zone_clan.h"
#include "zone_script.h"
#include "zone_npc.h"
#include "zone_move.h"
#include "zone_clt.h"
#include "zone_city.h"
#include "zone_combat.h"
#include "zone_player.h"
#include "zone_package.h"
#include "zone_attr.h"
#include "zone_task.h"
#include "zone_err.h"

extern int g_iOptype;
extern int g_iOpsubtype;

//创建城市NPC
int z_city_npc_create(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan)
{
	int i;
	CLANCITYLEVELDEF *pstClanCityLevelDef;
	
	pstClanCityLevelDef = z_clan_city_level_def(pstEnv, pstZoneClan->stClan.ClanCity.Level);
	if (NULL == pstClanCityLevelDef)
	{
		return -1;
	}

	for (i = 0; i < CLAN_MAX_NPC_NUM;i++)
	{
		if (pstClanCityLevelDef->ExtraNpcs[i].NpcID == 0)
		{
			break;
		}
		
		clan_city_npc_create_one(pstEnv, pstZoneClan, &pstClanCityLevelDef->ExtraNpcs[i]);
	}
	
	return 0;
}


ZONEPWORLDINST *z_find_clan_city_inst(ZONESVRENV * pstEnv, ZoneClan *pstZoneClan)
{
	ZONEPWORLDINST *pstZonePworldInst;
	
	if (0 == pstZoneClan->stZoneClanInfo.ullCityPworldWID)
	{
		return NULL;
	}

	pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstZoneClan->stZoneClanInfo.iCityPworldInstID);
	if (NULL == pstZonePworldInst || 
		pstZonePworldInst->ullPworldWID != pstZoneClan->stZoneClanInfo.ullCityPworldWID ||
		pstZonePworldInst->stPworldTypeData.stClan.ullGID != pstZoneClan->stClan.ClanProf.GID)
	{
		pstZoneClan->stZoneClanInfo.ullCityPworldWID = 0;
		pstZoneClan->stZoneClanInfo.iCityPworldInstID = 0;
		return NULL;
	}

	return pstZonePworldInst;
}

ZONEPWORLDINST *z_find_clan_everyday_pworld_inst(ZONESVRENV * pstEnv, ZoneClan *pstZoneClan)
{
	ZONEPWORLDINST *pstZonePworldInst;
	
	if (0 == pstZoneClan->stZoneClanInfo.ullEveryDayPworldWID)
	{
		return NULL;
	}

	pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstZoneClan->stZoneClanInfo.iEveryDayPworldInstID);
	if (NULL == pstZonePworldInst || 
		pstZonePworldInst->ullPworldWID != pstZoneClan->stZoneClanInfo.ullEveryDayPworldWID ||
		pstZonePworldInst->stPworldTypeData.stClan.ullGID != pstZoneClan->stClan.ClanProf.GID)
	{
		pstZoneClan->stZoneClanInfo.ullEveryDayPworldWID = 0;
		pstZoneClan->stZoneClanInfo.iEveryDayPworldInstID = 0;
		return NULL;
	}

	return pstZonePworldInst;
}

ZONEPWORLDINST *z_find_clan_machine_pworld_inst(ZONESVRENV * pstEnv, ZoneClan *pstZoneClan)
{
	ZONEPWORLDINST *pstZonePworldInst;
	
	if (0 == pstZoneClan->stZoneClanInfo.ullMachinePworldWID)
	{
		return NULL;
	}

	pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstZoneClan->stZoneClanInfo.iMachinePworldInstID);
	if (NULL == pstZonePworldInst || 
		pstZonePworldInst->ullPworldWID != pstZoneClan->stZoneClanInfo.ullMachinePworldWID ||
		pstZonePworldInst->stPworldTypeData.stClan.ullGID != pstZoneClan->stClan.ClanProf.GID)
	{
		pstZoneClan->stZoneClanInfo.ullMachinePworldWID = 0;
		pstZoneClan->stZoneClanInfo.iMachinePworldInstID = 0;
		return NULL;
	}

	return pstZonePworldInst;
}

int z_player_enter_clan_city(ZONESVRENV * pstEnv, Player *pstPlayer, 
						ZONEPWORLDINST *pstZonePworldInst, ZONEPWORLD *pstZonePworld, RESPOS *pstPos,
						ZoneClan *pstZoneClan)
{
	int i;
	int iMapIndex;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	
	if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
	{
		pstPlayer->stRoleData.ExitMap = pstPlayer->stRoleData.Map;
		pstPlayer->stRoleData.ExitPos_x = pstPlayer->stRoleData.Pos.X;
		pstPlayer->stRoleData.ExitPos_y = pstPlayer->stRoleData.Pos.Y;
		player_exit_info_check(pstEnv, pstPlayer);
	}

	if (pstZonePworld->stPworldDef.PworldID == CLAN_CITY_PWORLD_ID)
	{		
		if (pstMiscInfo->PworldWID != pstZonePworldInst->ullPworldWID)
		{
			pstMiscInfo->PworldWIDPre = pstMiscInfo->PworldWID;
		}
	}
	else
	{
		pstMiscInfo->PworldWIDPre = 0;
	}

	pstMiscInfo->PworldInstID = pstZonePworldInst->iPworldInstID;
	pstMiscInfo->PworldWID = pstZonePworldInst->ullPworldWID;

	iMapIndex = (pstZonePworldInst->iPworldInstID & 0xffff)<<16 | (pstZonePworld->stPworldDef.InitMapID & 0xffff);

	pstPlayer->stOnline.stWeiXing.cWeiXingNum  = 0;

	if((pstPlayer->stOnline.State & (CS_STAT_RIDE_ON | CS_STAT_FLY )) && 
		pstPlayer->stOnline.stRideOnInfo.iPassengerNum > 0)
	{
		Player *pstTmpPlayer;

		for(i=0; i<pstPlayer->stOnline.stRideOnInfo.iPassengerNum; i++)
		{
			pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stRideOnInfo.PassengerID[i]);
			if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				continue;
			}

			//乘客下马
			z_ride_break(pstEnv->pstAppCtx, pstEnv, pstTmpPlayer);

			//如果同一个工会则工会
			if (pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID == pstTmpPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID)
			{
				clan_mem_enter_city(pstEnv, pstTmpPlayer, pstPos);
			}
		}

		//主人下马
		z_ride_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	player_clan_city_update(pstEnv,pstPlayer,pstZoneClan);
	
	return z_player_change_map(
		pstEnv->pstAppCtx,
		pstEnv,
		pstPlayer,
		iMapIndex,
		!pstPos ? &pstZonePworld->stPworldDef.InitMapPos : pstPos,
		!pstPos ? RAND1(180) : pstZonePworld->stPworldDef.InitMapDir,
		!pstPos ? 0 : pstZonePworld->stPworldDef.InitCameraDir);
}

int player_clan_goto_city(ZONESVRENV *pstEnv, Player *pstPlayer, RESPOS *pstPos)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer); 
	ZONEPWORLDINST *pstZonePworldInst = NULL;
	ZONEPWORLD *pstZonePworld = NULL;

	if (!pstZoneClan)
	{
		return -1;
	}

	pstZonePworldInst = z_find_clan_city_inst(pstEnv, pstZoneClan);
	if (!pstZonePworldInst)
	{
		return -1;
	}

	pstZonePworld = z_find_pworld(pstEnv->pstMapObj,&pstZonePworldInst->stPworldIdx);
	if (!pstZonePworld)
	{
		return -1;
	}

	return z_player_enter_clan_city(pstEnv, pstPlayer, pstZonePworldInst, pstZonePworld, pstPos, pstZoneClan);
}

Npc *z_find_clan_city_npc(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan, int iDefID, RESPOS *pstPos)
{
	int i, j;
	int iDyn;
	Npc* pstNpc;
	MapIndex *pstMapIdx;
	ZONEMAPINST* pstMapInst;
	AREADYN *pstAreaDyn;
	ZONEPWORLDINST *pstZonePworldInst;
	
	pstZonePworldInst = z_find_clan_city_inst(pstEnv, pstZoneClan);
	if (NULL == pstZonePworldInst)
	{
		return NULL;
	}

	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstZonePworldInst->astMapInstIdx[0]);
	if( NULL == pstMapInst )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_find_map_inst %d map inst fail", pstZonePworldInst->astMapInstIdx[0].iID);
		return NULL;
	}

	pstMapIdx	=	z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return NULL;
	}	

	//先在对应坐标区域找
	if(pstPos)
	{
		pstAreaDyn = z_pos_area_dyn(pstMapIdx, pstEnv->pstAreaDynPool, pstMapInst, pstPos);
		if (pstAreaDyn)
		{
			for(i=0; i<pstAreaDyn->iAnimate; i++)
			{
				if( i>=MAX_AREA_ANIMATE )
				{
					break;
				}

				pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstAreaDyn->aiAnimate[i]);
				if (pstNpc && pstNpc->stMon.iDefIdx == iDefID)
				{
					return pstNpc;
				}
			}
		}
	}

	//全地图找一遍
	for (i=0; i<pstMapInst->iAreaDyn; i++)
	{
		iDyn	 =	pstMapInst->aiAreaDyn[i];
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

		for(j=0; j<pstAreaDyn->iAnimate; j++)
		{
			if( j>=MAX_AREA_ANIMATE )
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d", pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
				break;
			}

			pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstAreaDyn->aiAnimate[j]);
			if (pstNpc && pstNpc->stMon.iDefIdx == iDefID)
			{
				return pstNpc;
			}
		}
	}

	return NULL;
}

int z_in_new_clan_pworld_npc(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, 
									CLANNPC*pstClanNpc,ZoneClan *pstZoneClan)
{
	Npc stNpc;
	Npc* pstFind = NULL;
	int iRet=0;
	MONSTERDEF *pstMonDef = NULL;
	ScriptIndex *pstScriptInd = NULL;

	memset(&stNpc, 0, sizeof(stNpc));

	stNpc.stMon.iDefIdx = pstClanNpc->NpcID;
	stNpc.stMon.stMap.iID = stNpc.stMon.iInitMap = pstMapInst->iMapInstID;
	stNpc.stMon.iInitMapPos = pstMapInst->iMapPos;
	stNpc.stMon.stMap.iPos = pstMapInst->iIndex;
	stNpc.stMon.cType = OBJ_NPC;
	stNpc.stMon.cStat = MONSTER_DIE;
	stNpc.stMon.bDirection = pstClanNpc->Dir;
	stNpc.stMon.tRefresh = pstAppCtx->stCurr.tv_sec;
	memcpy(&stNpc.stMon.stInitPos, &pstClanNpc->Pos, sizeof(stNpc.stMon.stInitPos));
	memcpy(&stNpc.stMon.stCurrPos, &pstClanNpc->Pos, sizeof(stNpc.stMon.stCurrPos));
	stNpc.stMon.cRefresh = 1;
	stNpc.stMon.iPatrolID = pstClanNpc->PatrolID;
	if (pstClanNpc->PKMode == 1)
	{
		mon_set_pkinfo(&stNpc.stMon, MON_PK_TYPE_CLAN, pstZoneClan->stClan.ClanProf.GID);	
		stNpc.stMon.nOwnerFastID = pstZoneClan->stClan.ClanProf.Id;
	}
		
	STRNCPY(stNpc.szScriptName, pstClanNpc->DynamicScript, sizeof(stNpc.szScriptName));
	trim(stNpc.szScriptName);

	if (stNpc.szScriptName[0] != 0)
	{
		pstScriptInd = get_script_ind(stNpc.szScriptName);
		if (!pstScriptInd)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "npc %d script %s cant be available", pstClanNpc->NpcID, pstClanNpc->DynamicScript);
			return -1;
		}
		memcpy(&stNpc.stScriptIdx, pstScriptInd, sizeof(stNpc.stScriptIdx));
	}

	pstFind	=	z_new_npc(pstAppCtx, pstEnv, &stNpc);
	if( !pstFind )
		return -1;

	pstMonDef = z_get_mon_def(pstEnv, &pstFind->stMon);
	if (!pstMonDef)
	{
		tmempool_free(pstEnv->pstNpcPool, pstFind->stMon.iMIndex);
		tlog_error(pstEnv->pstLogCat, 0, 0, "map npc %d no monster def", pstFind->stMon.iDefIdx);
		return -1;
	}

	z_check_mon_script(pstEnv, &stNpc.stMon, pstMonDef);
	pstFind->stMon.unRadius = pstMonDef->Radius;

	pstFind->stMon.cFirstBorn = 1;
	if (0 > z_refresh_mon(pstAppCtx, pstEnv, (Monster *)pstFind,NULL))
	{
		tmempool_free(pstEnv->pstNpcPool, pstFind->stMon.iMIndex);
		return -1;
	}
	pstFind->stMon.cFirstBorn = 0;
	
	pstFind->nSellListID = pstClanNpc->NpcSaleIdx;

	z_check_npc_script(pstEnv, pstFind);
		
	return iRet;
}

ZONEPWORLDINST* z_new_clan_pworld_inst(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, ZONEPWORLD * pstPworld, ZoneClan *pstZoneClan, int iTimeout)
{
	ZONEPWORLDINST*pstZonePworldInst;
	PWORLDSELECT stPworldSelect;

	bzero(&stPworldSelect, sizeof(stPworldSelect));

	pstZonePworldInst = z_in_new_pworld_inst(pstAppCtx, pstEnv, pstPworld, &stPworldSelect,  0, iTimeout);
	if (NULL == pstZonePworldInst)
	{
		return NULL;
	}

	pstZonePworldInst->iType = PWORLD_TYPE_CLAN;
	pstZonePworldInst->stPworldTypeData.stClan.iID = pstZoneClan->stClan.ClanProf.Id;
	pstZonePworldInst->stPworldTypeData.stClan.ullGID = pstZoneClan->stClan.ClanProf.GID;

	pstZoneClan->stZoneClanInfo.ullCityPworldWID =  pstZonePworldInst->ullPworldWID;
	pstZoneClan->stZoneClanInfo.iCityPworldInstID =  pstZonePworldInst->iPworldInstID;
	
	return pstZonePworldInst;
}

int z_new_clan_city(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan)
{
	int i;
	ZONEIDX stIdx;
	ZONEPWORLD *pstZonePworld;
	ZONEPWORLDINST *pstZonePworldInst;
	
	stIdx.iID = CLAN_CITY_PWORLD_ID;
	stIdx.iPos = -1;
	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (NULL == pstZonePworld)
	{
		return -1;
	}

	pstZonePworldInst = z_new_clan_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstZonePworld, pstZoneClan, 0);
	if (NULL == pstZonePworldInst)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_new_clan_pworld_inst fail");
		return -1;
	}
	
	if (clan_city_npc_create(pstEnv, pstZoneClan) < 0)
	{
		return -1;
	}

	for (i = CLAN_IDX_TECH_CENTER; i < CLAN_IDX_WALL; ++i)
	{
		int j;
		const CLANCITYBUILDING *pstBuilding =
			pstZoneClan->stClan.ClanCity.Buildings + i;
		for (j = 1; j < pstBuilding->Level + 1; ++j)
		{
			clan_building_remove_barrier(pstEnv, pstZoneClan, i, j);
		}
		if (pstBuilding->Pending)
		{
			clan_building_remove_barrier(pstEnv, pstZoneClan, i,
			                             pstBuilding->Level + 1);
		}
	}
	return 0;
}

int  clan_city_npc_create_one(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan, CLANNPC*pstClanNpc)
{
	ZONEPWORLDINST *pstZonePworldInst;
	ZONEMAPINST *pstMapInst;
	
	pstZonePworldInst = z_find_clan_city_inst(pstEnv, pstZoneClan);
	if (NULL == pstZonePworldInst)
	{
		return -1;
	}

	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstZonePworldInst->astMapInstIdx[0]);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	if (0 > z_in_new_clan_pworld_npc(pstEnv->pstAppCtx, pstEnv, pstMapInst, pstClanNpc,pstZoneClan))
	{
		return -1;
	}
	
	return 0;
}

int clan_city_npc_del_one(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan, int iDefID, RESPOS *pstPos)
{
	Npc* pstNpc;

	pstNpc = z_find_clan_city_npc(pstEnv, pstZoneClan, iDefID, pstPos);
	if (pstNpc)
	{
		z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, &pstNpc->stMon);
		z_free_monster(pstEnv->pstAppCtx, pstEnv, &pstNpc->stMon);
		return 0;
	}
	
	return -1;
}

int clan_city_npc_create(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan)
{
	//城市NPC 
	z_city_npc_create(pstEnv, pstZoneClan);

	//城市建筑
	//z_city_build_create(pstEnv, pstZoneClan);
	 	
	return 0;
}

int clan_city_create(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZoneClan *pstZoneClan;
	ZONEPWORLDINST *pstZonePworldInst;
	
	pstZoneClan = player_get_clan(pstEnv,  pstPlayer);
	if (NULL == pstZoneClan)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, 
			ZONEERR_CLAN71);
		return -1;
	}

	pstZonePworldInst = z_find_clan_city_inst(pstEnv, pstZoneClan);
	if (pstZonePworldInst)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, 
			ZONEERR_CLAN83);
		return -1;
	}
	
	return z_new_clan_city(pstEnv, pstZoneClan);
}

int clan_mem_enter_city_check(ZONESVRENV* pstEnv, Player *pstPlayer, RESPOS *pstPos)
{
	ZoneClan *pstZoneClan;
	ZONEPWORLD *pstZonePworld;
	ZONEPWORLDINST *pstZonePworldInst;

	if (INST_MODE_ZONE_SVR != pstEnv->iInstMode)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
		return 0;
	}
	
	if(0 > z_player_goto_check(pstEnv, pstPlayer, 0, NULL))
	{
		return -1;
	}
	
	pstZoneClan = player_get_clan(pstEnv,  pstPlayer);
	if (NULL == pstZoneClan)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, 
						pstPlayer, SYS_MIDDLE, ZONEERR_CLAN71);
		return -1;
	}
	
	pstZonePworldInst = z_find_clan_city_inst(pstEnv, pstZoneClan);
	if (NULL == pstZonePworldInst)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, 
						pstPlayer, SYS_MIDDLE, ZONEERR_CLAN84);
		return -1;
	}

	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
	if (NULL == pstZonePworld)
	{
		return -1;
	}

	return 0;
	UNUSED(pstPos);	//add by paraunused.pl
}

int clan_mem_enter_city(ZONESVRENV* pstEnv, Player *pstPlayer, RESPOS *pstPos)
{
	ZoneClan *pstZoneClan;
	ZONEPWORLD *pstZonePworld;
	ZONEPWORLDINST *pstZonePworldInst;
	
	pstZoneClan = player_get_clan(pstEnv,  pstPlayer);
	if (NULL == pstZoneClan)
	{
		return -1;
	}

	pstZonePworldInst = z_find_clan_city_inst(pstEnv, pstZoneClan);
	if (NULL == pstZonePworldInst)
	{
		return -1;
	}

	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
	if (NULL == pstZonePworld)
	{
		return -1;
	}
	
	return z_player_enter_clan_city(pstEnv, pstPlayer, pstZonePworldInst, pstZonePworld, pstPos,pstZoneClan);
}

int player_enter_clan_city_clt(ZONESVRENV* pstEnv, Player *pstPlayer, int iClanID,int iSpyTransID)
{
	ZoneClan *pstZoneClan;
	ZONEPWORLD *pstZonePworld;
	ZONEPWORLDINST *pstZonePworldInst;
	ZoneClan *pstSelf;
	RESPOS stPos;
	RESPOS* pstPos = NULL;
	CLANSPYTRANSDEF* pstSpyTransDef = NULL;
	int iRet = 0;

	if (iSpyTransID)
	{
		//GameEventInst *pstEventSpy = NULL;
		
		//pstEventSpy = z_get_game_event(pstEnv,SPY_EVENT_ID);

		//if (!pstEventSpy)
		//{	
			//z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "刺探活动未开启，不允许该操作");
			//return -1;
		//}
		
		pstSpyTransDef = z_find_clan_spy_trans_def(pstEnv,iSpyTransID);
		if (!pstSpyTransDef)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN86);
			return -1;
		}
		stPos.X = pstSpyTransDef->X;
		stPos.Y = pstSpyTransDef->Y;
		pstPos = &stPos;
	}
	else
	{		
		//if (check_npc_ui(pstEnv, pstPlayer, UI_TYPE_CLAN_SEARCH) < 0)
		//{
			//return -1;
		//}
	}

	if (player_is_zuolao(pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN87);
		return -1;
	}

	pstZoneClan = z_id_clan(pstEnv, iClanID);
	if (!pstZoneClan)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN88);
		return -1;
	}

	if (pstZoneClan->stZoneClanInfo.iDirty == CACHE_FLAG_INEXIST)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN88);
		return -1;
	}

	pstZonePworldInst = z_find_clan_city_inst(pstEnv, pstZoneClan);
	if (NULL == pstZonePworldInst)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN89);
		return -1;
	}

	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
	if (NULL == pstZonePworld)
	{
		return -1;
	}
	
	if (pstPos)
	{
		iRet = z_player_enter_clan_city(pstEnv, pstPlayer, pstZonePworldInst, pstZonePworld,pstPos,pstZoneClan);
	}
	else
	{
		iRet = z_player_enter_clan_city(pstEnv, pstPlayer, pstZonePworldInst, pstZonePworld,NULL,pstZoneClan);	
	}
	
	if (iRet == 0)
	{
		pstSelf = z_id_clan(pstEnv, pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id);
		if (!pstSelf )
		{
			player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
	                          						ZONEERR_CLANBROAD252, 
	                          						pstPlayer->stRoleData.RoleName);
			return 0;
		}

		if (pstSelf != pstZoneClan)
		{
			player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
	                          						ZONEERR_CLANBROAD253,
	                          						pstSelf->stClan.ClanProf.Name,
	                          						pstPlayer->stRoleData.RoleName);

			if ((pstPlayer->stOnline.cMiscFlag & MISC_FLAG_SPY) &&
				pstPlayer->stRoleData.MiscInfo.DailyInfo.SpyInfo.TargetClanID == 0 &&
				(pstZoneClan->stClan.ClanProf.Flag & CLAN_FLAG_SPY_AVAILABLE))
			{
				/*
				player_clan_msg_broadcast(pstEnv, pstSelf, SYS_CLAN,
	                          						"公会成员 %s 已经潜入 %s 公会,夺取情报去了!",	                          						
	                          						pstPlayer->stRoleData.RoleName,
	                          						pstZoneClan->stClan.ClanProf.Name);
	             */

				pstPlayer->stRoleData.MiscInfo.DailyInfo.SpyInfo.TargetClanID = pstZoneClan->stClan.ClanProf.Id;

				player_action_view_flag_chg(pstEnv,pstPlayer);
				
			}
			return 0;
		}
	}
	return 0;
}

int clan_city_npc_del(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan)
{
	int i;
	CLANCITYLEVELDEF *pstClanCityLevelDef;
	//CLANCITYBUILDINGLEVELDEF *pstClanCityBuild;	//comment by paraunused.pl
	ZONEPWORLDINST *pstZonePworldInst;
	
	pstZonePworldInst = z_find_clan_city_inst(pstEnv, pstZoneClan);
	if (NULL == pstZonePworldInst)
	{
		return -1;
	}

	pstClanCityLevelDef = z_clan_city_level_def(pstEnv, pstZoneClan->stClan.ClanCity.Level);
	if (NULL == pstClanCityLevelDef)
	{
		return -1;
	}

	for (i = 0; i < CLAN_MAX_NPC_NUM;i++)
	{
		if (pstClanCityLevelDef->ExtraNpcs[i].NpcID == 0)
		{
			break;
		}
		
		clan_city_npc_del_one(pstEnv, pstZoneClan, pstClanCityLevelDef->ExtraNpcs[i].NpcID,
									&pstClanCityLevelDef->ExtraNpcs[i].Pos);
	}

	/*
	for (i = 0; i < CLAN_MAX_BUILDING_NUM; i++)
	{
		pstClanCityBuild = z_clan_city_building_level_def(pstEnv,
	                                                       pstZoneClan->stClan.ClanCity.Buildings[i].Level, i);
		if (NULL == pstClanCityBuild)
		{
			continue;
		}
		
		clan_city_npc_del_one(pstEnv, pstZoneClan, pstClanCityBuild->Dummy.NpcID,
									&pstClanCityBuild->Dummy.Pos);
	}
	*/

	return 0;
}

int clan_city_npc_bind_selllist(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan , int iDefID, int iSellLIst)
{
	Npc* pstNpc;

	pstNpc = z_find_clan_city_npc(pstEnv, pstZoneClan, iDefID, NULL);
	if (pstNpc)
	{
		pstNpc->nSellListID = iSellLIst;
		return 0;
	}
	
	return -1;
}

int clan_destroy_city(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan)
{
	ZONEPWORLDINST *pstZonePworldInst;
	
	pstZonePworldInst = z_find_clan_city_inst(pstEnv, pstZoneClan);
	if (NULL == pstZonePworldInst)
	{
		return -1;
	}

	if(0 > z_in_destroy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstZonePworldInst)) 
	{
		return -1;
	}

	pstZoneClan->stZoneClanInfo.ullCityPworldWID = 0;
	pstZoneClan->stZoneClanInfo.iCityPworldInstID = 0;
	
	return 0;
}

int player_everyday_pworld_ui_info(ZONESVRENV* pstEnv, Player *pstPlayer, CSPKG *pstPkg)
{
	ZoneClan *pstZoneClan;
	ZONEPWORLDINST *pstPworldInst;
	CLANLEVELDEF *pstClanLevel;
	LEVELUP *pstLevelUp;
	CSACTIVEGAMESVR *pstActiveGameSvr = &pstPkg->Body.ActiveGameSvr;
	CSACTIVEGAMEUIINFO *pstActiveUIInfo = &pstActiveGameSvr->Data.UIInfo;

	pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	if(NULL == pstZoneClan)
	{
		return -1;
	}

	pstClanLevel = z_clan_level_def(pstEnv, pstZoneClan->stClan.ClanProf.Level);
	if(NULL == pstClanLevel)
	{
		return -1;
	}
	pstLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	if(NULL == pstLevelUp)
	{
		return -1;
	}
	pstPworldInst = z_find_clan_everyday_pworld_inst(pstEnv, pstZoneClan);
	if(NULL == pstPworldInst)
	{
		
		return -1;
	}
	
	pstActiveGameSvr->Type = ACTIVE_GAME_SVR_TYPE_UI_INFO;
	pstActiveUIInfo->UIType = ACTIVE_UI_TYPE_CLAN_PWORLD;
	
	memset(&pstActiveUIInfo->Data.ClanPworld, 0, sizeof(pstActiveUIInfo->Data.ClanPworld));
	pstActiveUIInfo->Data.ClanPworld.Val = pstPlayer->stRoleData.MiscInfo.ClanInfo.EveryDayPworldVal;
	pstActiveUIInfo->Data.ClanPworld.ClanVal =  pstZoneClan->stZoneClanInfo.iEveryDayPworldVal;
	pstActiveUIInfo->Data.ClanPworld.ClanMaxVal = pstZoneClan->stZoneClanInfo.iMaxEveryDayPworldVal;
	pstActiveUIInfo->Data.ClanPworld.EndTime = pstPworldInst->tEnd;
	pstActiveUIInfo->Data.ClanPworld.BossLife = pstZoneClan->stZoneClanInfo.iBossLife;

	if(pstPlayer->stRoleData.MiscInfo.ClanInfo.EveryDayPworldVal > 0)
	{
		double iRate;
		if(pstPlayer->stRoleData.MiscInfo.ClanInfo.EveryDayPworldVal < 51)
		{
			iRate = 10/(510-pstPlayer->stRoleData.MiscInfo.ClanInfo.EveryDayPworldVal*10.0);
		}
		else
		{
			iRate = 0.1;
		}
		
		pstActiveUIInfo->Data.ClanPworld.ClanMoney = iRate*pstClanLevel->EveryDayPworldClanMoney;
		/*
		pstActiveUIInfo->Data.ClanPworld.ClanContri =  iRate*pstClanLevel->EveryDayPworldContri/100;
		pstActiveUIInfo->Data.ClanPworld.PlayerMoney = iRate*CLAN_EVERYDAY_PWORLD_PLAYER_AWARD/100;
		*/
		pstActiveUIInfo->Data.ClanPworld.PlayerExp = (iRate*pstLevelUp->ContributeExp)/200;
	}
	
	Z_CSHEAD_INIT(&pstPkg->Head, ACTIVE_GAME_SVR);

	player_dyn_active_helper_info(pstEnv,pstPlayer,CLAN_BANISH_ACTIVE_HELP_ID,ACTIVE_STATUS_RUNNING);

	return 0;
}

//活动UI信息
int player_clan_everyday_pworld_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;

	if(0 > player_everyday_pworld_ui_info(pstEnv, pstPlayer, &stPkg))
	{
		return -1;
	}
	
	return z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
}

//个人积分变化
int player_clan_everyday_pworld_val_chg(ZONESVRENV* pstEnv, Player *pstPlayer, int iChg)
{
	pstPlayer->stRoleData.MiscInfo.ClanInfo.EveryDayPworldVal += iChg;
	if(pstPlayer->stRoleData.MiscInfo.ClanInfo.EveryDayPworldVal < 0)
	{
		pstPlayer->stRoleData.MiscInfo.ClanInfo.EveryDayPworldVal = 0;
	}

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
		ZONEERR_CLAN90, 
		iChg, pstPlayer->stRoleData.MiscInfo.ClanInfo.EveryDayPworldVal);

	return player_clan_everyday_pworld_action(pstEnv, pstPlayer);
}

int player_clan_everyday_pworld_award(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iVal;
	int iClanMoney = 0;
	double iRate;
	tdr_longlong iExp;
	ZoneClan *pstZoneClan;
	ZONEPWORLDINST *pstPworldInst;
	CLANLEVELDEF *pstClanLevel;
	LEVELUP *pstLevelUp;
	PlayerChagAttrVal stAttr;
	pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	if(NULL == pstZoneClan)
	{
		return -1;
	}

	pstClanLevel = z_clan_level_def(pstEnv, pstZoneClan->stClan.ClanProf.Level);
	if(NULL == pstClanLevel)
	{
		return -1;
	}
	pstLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	pstPworldInst = z_find_clan_everyday_pworld_inst(pstEnv, pstZoneClan);
	if(NULL == pstPworldInst)
	{
		return -1;
	}

	if(pstPlayer->stRoleData.MiscInfo.ClanInfo.EveryDayPworldVal <= 0)
	{
		return -1;
	}

	if(pstPlayer->stRoleData.MiscInfo.ClanInfo.EveryDayPworldVal < 51)
	{
		iRate = 10/(510-pstPlayer->stRoleData.MiscInfo.ClanInfo.EveryDayPworldVal*10.0);
	}
	else
	{
		iRate = 0.1;
	}

	/*
	clan_resource_inc(pstEnv, pstZoneClan, pstPlayer, CLAN_OWNED_CONTRIBUT,
			iRate*pstClanLevel->EveryDayPworldContri/100);
	iMoney = iRate*CLAN_EVERYDAY_PWORLD_PLAYER_AWARD/100;
	
	if (0 > package_add_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, iMoney))
	{
		return -1;
	}
	package_add_money(pstEnv, pstPlayer, ATTR_ID_MONEY, iMoney);
	*/

	iClanMoney = (int)(iRate*pstClanLevel->EveryDayPworldClanMoney);
	g_iOptype = OP_OPTYPE_DAYPWORLD;
	g_iOpsubtype = pstPworldInst->stPworldIdx.iID;
	clan_resource_inc(pstEnv, pstZoneClan, pstPlayer, CLAN_MONEY,iClanMoney);
	g_iOptype = 0;
	g_iOpsubtype = 0;
	
	iExp = (int)((iRate*pstLevelUp->ContributeExp)/200);
	z_player_addexp_test(pstEnv, pstPlayer, &iExp);
	if (iExp <= 0)
	{
		goto _final;
	}

	stAttr.AttrList[0].AttrID = ATTR_ID_EXP;
	stAttr.AttrList[0].Value.Exp= iExp;
	stAttr.AttrList[0].nFlag = 0;
	stAttr.AttrNum = 1;
	z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stAttr, 1);

_final:	
	
	iVal = pstPlayer->stRoleData.MiscInfo.ClanInfo.EveryDayPworldVal;
	if(iVal == 50)
	{
		task_clan_everyday_trace(pstEnv,pstPlayer);
		pstPlayer->stRoleData.MiscInfo.DailyInfo.ClanEveryDayPworldScore.Value++;
	 	pstPlayer->stRoleData.MiscInfo.DailyInfo.ClanEveryDayPworldScore.Time 
	 		= pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	
	pstPlayer->stRoleData.MiscInfo.ClanInfo.EveryDayPworldVal  = 0;
	player_clan_everyday_pworld_action(pstEnv, pstPlayer);
	clan_everyday_pworld_val(pstEnv, pstZoneClan, pstClanLevel, iVal);
	return 0;
}

//公会在线人员广播各自需要的协议
int clan_broadcast_do_pkg(ZONESVRENV* pstEnv, ZoneClan *pstClan, int (*pfnPred)(ZONESVRENV *, Player *, CSPKG *))
{
	int i;
	int iRet = 0;
	Player *pstPlayer = NULL;
	CSPKG stPkg;
	
	for(i = 0; i < pstClan->stClan.ClanProf.MemNum; ++i)
	{
		if (!pstClan->stClan.ClanCore.ClanMem[i].Online)
		{
			continue;
		}

		pstPlayer = z_name_player(
			pstEnv->pstAppCtx,
			pstEnv,
			pstClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName);
		if (!pstPlayer)
		{
			continue;
		}

		iRet = (*pfnPred)(pstEnv, pstPlayer, &stPkg);
		if(iRet < 0)
		{
			continue;
		}

		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	return 0;
}

int clan_active_ui_info_clan_spy(ZONESVRENV * pstEnv,ZoneClan* pstClan,int iType)
{
	CSPKG stPkg;
	CSACTIVEGAMESVR *pstActiveGameSvr = &stPkg.Body.ActiveGameSvr;
	CSACTIVEGAMEUIINFO *pstActiveUIInfo = &pstActiveGameSvr->Data.UIInfo;
	
	memset(pstActiveGameSvr,0,sizeof(*pstActiveGameSvr));
	
	pstActiveGameSvr->Type = iType;
	pstActiveUIInfo->UIType = ACTIVE_UI_TYPE_CLAN_SPY;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTIVE_GAME_SVR);
	clan_broadcast_if(pstEnv, pstClan, &stPkg, ClanEveryBody);

	return 0;
}

int clan_pworld_wave_countdown_action(ZONESVRENV* pstEnv,ZoneClan* pstZoneClan,ZONEPWORLDINST *pstPworldInst)
{
	int i= 0;
	Player* pstPlayer = NULL;

	i = pstZoneClan->stClan.ClanProf.MemNum;
	while(i--)
	{
		if (!pstZoneClan->stClan.ClanCore.ClanMem[i].Online)
		{
			continue;
		}

		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
			(const char*)&pstZoneClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName);
		if (!pstPlayer)
		{
			continue;
		}
		player_action_clan_machine_pworld(pstEnv,pstPlayer,pstPworldInst);
	}
	
	return 0;
}


int clan_active_ui_info_clan_machine_pworld(ZONESVRENV * pstEnv,ZoneClan* pstClan,int iType,
														ZONEPWORLDINST*pstZonePworldInst, int iWaveInfo)
{
	CSPKG stPkg;
	CSACTIVEGAMESVR *pstActiveGameSvr = &stPkg.Body.ActiveGameSvr;
	CSACTIVEGAMEUIINFO *pstActiveUIInfo = &pstActiveGameSvr->Data.UIInfo;
	ZONEPWORLD *pstPworld;
	int i, iCount = 0;
	Player *pstPlayer = NULL;
	
	memset(pstActiveGameSvr,0,sizeof(*pstActiveGameSvr));
	
	pstActiveGameSvr->Type = iType;
	pstActiveUIInfo->UIType = ACTIVE_UI_TYPE_CLAN_MACHINE_PWORLD;
	pstActiveUIInfo->Data.ClanMachine.ClanID = pstClan->stClan.ClanProf.Id;
	pstActiveUIInfo->Data.ClanMachine.Wave = pstZonePworldInst->stWaveRound.iCurrRound+1;
	Z_CSHEAD_INIT(&stPkg.Head, ACTIVE_GAME_SVR);

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
	if(NULL == pstPworld)
	{
		return -1;
	}

	if (!pstZonePworldInst->cShareShuNiu)
	{
		
		Player *apstPlayers[CLAN_MAX_MEM];

		for (i=0; i<pstClan->stClan.ClanProf.MemNum; i++)
		{
			pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName);
			if (!pstPlayer)
			{
				continue;
			}

			// 今天已经参加过了
			if (iType == ACTIVE_GAME_SVR_TYPE_UI_INFO)
			{
				if (!iWaveInfo)
				{
					if (IsSameDay(pstPlayer->stRoleData.MiscInfo.ShuNiuInfo.Time, pstEnv->pstAppCtx->stCurr.tv_sec))
					{
						continue;
					}
				}
			}
			apstPlayers[iCount++] = pstPlayer;
		}

		if (iCount > 0)
		{		
			z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
			                                iCount, &stPkg);
		}

		if (iType == ACTIVE_GAME_SVR_TYPE_UI_CLOSE)
		{
			player_clan_msg_broadcast(pstEnv,pstClan,SYS_CLAN,ZONEERR_CLANBROAD254);
		}
	}
	else
	{
		Player *apstPlayers[MAX_PLAYER];
		TMEMBLOCK *pstMemBlock;

		pstActiveUIInfo->Data.ClanMachine.Flag = 1;
		for (i=0; i<MAX_PLAYER; i++)
		{
			pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
			if (!pstMemBlock->fValid)
			{
				continue;
			}

			pstPlayer = (Player *)pstMemBlock->szData;
			if ( PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
			{
				continue;
			}

			if (iType == ACTIVE_GAME_SVR_TYPE_UI_INFO)
			{
				if (!player_get_clan(pstEnv, pstPlayer))
				{
					continue;
				}

				if (!iWaveInfo)
				{
					if (IsSameDay(pstPlayer->stRoleData.MiscInfo.ShuNiuInfo.Time, pstEnv->pstAppCtx->stCurr.tv_sec))
					{
						continue;
					}
				}
			}
			
			apstPlayers[iCount++] = pstPlayer;	
		}

		if (iCount > 0)
		{
			z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
			                                iCount, &stPkg);
		}	

		if (iType == ACTIVE_GAME_SVR_TYPE_UI_CLOSE)
		{
			z_sys_strf_broadcast(pstEnv, SYS_SVR, ZONEERR_STRONG241, pstClan->stClan.ClanProf.Name);
			player_clan_msg_broadcast(pstEnv,pstClan,SYS_CLAN,ZONEERR_STRONG241, pstClan->stClan.ClanProf.Name);
		}
		else if (iType == ACTIVE_GAME_SVR_TYPE_UI_INFO)
		{
			if (!iWaveInfo)
				z_sys_strf_broadcast(pstEnv, SYS_SVR, ZONEERR_STRONG240, pstClan->stClan.ClanProf.Name);
		}
	}
	
	return 0;
}

int clan_dny_active_helper_info(ZONESVRENV * pstEnv,ZoneClan* pstClan,int iID,int iStatus)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;

	memset(pstAction,0,sizeof(*pstAction));

	pstAction->ID = 0;
	pstAction->ActionID = ACTION_DYN_ACTIVE_INFO;
	
	pstAction->Data.DynActiveInfo.ActiveHelpID = iID;
	pstAction->Data.DynActiveInfo.Stauts = iStatus;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	
	clan_broadcast_if(pstEnv, pstClan, &stPkg, ClanEveryBody);

	if (iID == CLAN_TOUCH_ACTIVE_HELP_ID)
	{
		CSACTIVEGAMESVR *pstActiveGameSvr = &stPkg.Body.ActiveGameSvr;
		CSACTIVEGAMEUIINFO *pstActiveUIInfo = &pstActiveGameSvr->Data.UIInfo;

		int iType = ACTIVE_GAME_SVR_TYPE_UI_CLOSE;
		
		memset(pstActiveGameSvr,0,sizeof(*pstActiveGameSvr));

		

		if (iStatus == ACTIVE_STATUS_RUNNING)
		{
			iType = ACTIVE_GAME_SVR_TYPE_UI_INFO;
		}
		
		pstActiveGameSvr->Type = iType;
		pstActiveUIInfo->UIType = ACTIVE_UI_TYPE_CLAN_TOUCH;
		
		Z_CSHEAD_INIT(&stPkg.Head, ACTIVE_GAME_SVR);
		clan_broadcast_if(pstEnv, pstClan, &stPkg, ClanEveryBody);
	}
	return 0;
}


//关闭公会活动UI
int clan_everyday_pworld_close_ui(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan)
{
	CSPKG stPkg;
	CSACTIVEGAMESVR *pstActiveGameSvr = &stPkg.Body.ActiveGameSvr;
	CSACTIVEGAMEUICLOSE *pstActiveUIClose = &pstActiveGameSvr->Data.UIClose;

	pstActiveGameSvr->Type = ACTIVE_GAME_SVR_TYPE_UI_CLOSE;
	pstActiveUIClose->UIType = ACTIVE_UI_TYPE_CLAN_PWORLD;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTIVE_GAME_SVR);
	clan_broadcast_if(pstEnv, pstZoneClan, &stPkg, ClanEveryBody);

	clan_dny_active_helper_info(pstEnv,pstZoneClan,CLAN_BANISH_ACTIVE_HELP_ID,ACTIVE_STATUS_END);
	return 0;
}

ZONEPWORLDINST* z_new_clan_everyday_pworld_inst(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, ZONEPWORLD * pstPworld, ZoneClan *pstZoneClan, int iTimeout)
{
	ZONEPWORLDINST*pstZonePworldInst;
	PWORLDSELECT stPworldSelect;
	CLANLEVELDEF *pstClanLevel;

	bzero(&stPworldSelect, sizeof(stPworldSelect));

	pstZonePworldInst = z_in_new_pworld_inst(pstAppCtx, pstEnv, pstPworld, &stPworldSelect,  0, iTimeout);
	if (NULL == pstZonePworldInst)
	{
		return NULL;
	}

	pstZonePworldInst->iType = PWORLD_TYPE_CLAN;
	pstZonePworldInst->stPworldTypeData.stClan.iID = pstZoneClan->stClan.ClanProf.Id;
	pstZonePworldInst->stPworldTypeData.stClan.ullGID = pstZoneClan->stClan.ClanProf.GID;

	//一天只能开启一次，记录下时间
	pstZoneClan->stClan.ClanInner.EveryDayPworldTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	pstZoneClan->stZoneClanInfo.ullEveryDayPworldWID =  pstZonePworldInst->ullPworldWID;
	pstZoneClan->stZoneClanInfo.iEveryDayPworldInstID =  pstZonePworldInst->iPworldInstID;
	pstZoneClan->stZoneClanInfo.iEveryDayPworldVal = 0;
	pstZoneClan->stZoneClanInfo.iMaxEveryDayPworldVal = 0;
	pstZoneClan->stZoneClanInfo.iBossLife = 0;
	
	pstClanLevel = z_clan_level_def(pstEnv, pstZoneClan->stClan.ClanProf.Level);
	if(pstClanLevel)
	{
		pstZoneClan->stZoneClanInfo.iMaxEveryDayPworldVal = pstClanLevel->MaxEveryDayPworldVal;
	}
	
	//通知大家活动开启了
	clan_broadcast_do_pkg(pstEnv, pstZoneClan, player_everyday_pworld_ui_info);
	
	return pstZonePworldInst;
}

ZONEPWORLDINST* z_new_clan_machine_pworld_inst(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, ZONEPWORLD * pstPworld, 
													ZoneClan *pstZoneClan, int iTimeout,int iShare)
{
	ZONEPWORLDINST*pstZonePworldInst;
	PWORLDSELECT stPworldSelect;

	bzero(&stPworldSelect, sizeof(stPworldSelect));

	pstZonePworldInst = z_in_new_pworld_inst(pstAppCtx, pstEnv, pstPworld, &stPworldSelect,  0, iTimeout);
	if (NULL == pstZonePworldInst)
	{
		return NULL;
	}

	pstZonePworldInst->iType = PWORLD_TYPE_PLOY;
	pstZonePworldInst->stPworldTypeData.stClan.iID = pstZoneClan->stClan.ClanProf.Id;
	pstZonePworldInst->stPworldTypeData.stClan.ullGID = pstZoneClan->stClan.ClanProf.GID;
	pstZonePworldInst->cShareShuNiu = iShare;

	//一天只能开启一次，一周只能开两次,记录下时间
	if (!IsSameWeek(pstEnv->pstAppCtx->stCurr.tv_sec,
				pstZoneClan->stClan.ClanCore.MachinePworldLastOpenTime) )
	{
		pstZoneClan->stClan.ClanCore.MachinePworldWeekTimes = 0;
	}
		
	pstZoneClan->stClan.ClanCore.MachinePworldLastOpenTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstZoneClan->stClan.ClanCore.MachinePworldWeekTimes++;

	pstZoneClan->stZoneClanInfo.ullMachinePworldWID =  pstZonePworldInst->ullPworldWID;
	pstZoneClan->stZoneClanInfo.iMachinePworldInstID =  pstZonePworldInst->iPworldInstID;
	pstZoneClan->stZoneClanInfo.iMachinePworldVal = 0;

	//通知大家活动开启了
	clan_active_ui_info_clan_machine_pworld(pstEnv, pstZoneClan,
				ACTIVE_GAME_SVR_TYPE_UI_INFO, pstZonePworldInst, 0);
	if (iShare)
	{
		pstEnv->iShuNiuClanID = pstZoneClan->stClan.ClanProf.Id;		
	}
		
	return pstZonePworldInst;
}

int clan_everyday_pworld_stat_get(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan)
{
	if(IsSameDay(pstZoneClan->stClan.ClanInner.EveryDayPworldTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		ZONEPWORLDINST *pstPworldInst = z_find_clan_everyday_pworld_inst(pstEnv, pstZoneClan);
		if(pstPworldInst)
		{
			return EVERYDAY_PWORLD_OPEN;
		}
		else
		{
			return EVERYDAY_PWORLD_CLOSE;
		}
	}
	else
	{
		return EVERYDAY_PWORLD_UNOPEN;
	}

	return EVERYDAY_PWORLD_UNOPEN;
}

//创建公会积分活动副本
int clan_everyday_pworld_create(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan)
{	
	ZONEIDX stIdx;
	ZONEPWORLD * pstPworld;
	
	if(EVERYDAY_PWORLD_UNOPEN != clan_everyday_pworld_stat_get(pstEnv, pstZoneClan))
	{
		return 0;
	}

	stIdx.iID = CLAN_EVERYDAY_PWORLD_ID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if(NULL == pstPworld)
	{
		return -1;
	}

	z_new_clan_everyday_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstPworld, 
									pstZoneClan, pstPworld->stPworldDef.Timeout*60);
	return 0;
}

//进入公会积分活动副本
int clan_everyday_pworld_enter(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZoneClan *pstZoneClan;
	ZONEPWORLD * pstPworld;
	ZONEPWORLDINST *pstPworldInst;

	pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	if(NULL == pstZoneClan)
	{
		return -1;
	}

	pstPworldInst = z_find_clan_everyday_pworld_inst(pstEnv, pstZoneClan);
	if(NULL == pstPworldInst)
	{
		return -1;
	}

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if(NULL == pstPworld)
	{
		return -1;
	}
	
	if(0 == z_player_enter_pworld_by_inst(pstEnv, pstPlayer, pstPworld, pstPworldInst))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_CLAN85);
	}
	return 0;
}

//进入公会守护枢纽副本
int clan_machine_pworld_enter(ZONESVRENV* pstEnv, Player *pstPlayer, int iClanID)
{
	ZoneClan *pstZoneClan;
	ZONEPWORLD * pstPworld;
	ZONEPWORLDINST *pstPworldInst;
	ZoneClan *pstSelfClan;

	pstSelfClan = player_get_clan(pstEnv, pstPlayer);
	if(NULL == pstSelfClan)
	{
		return -1;
	}

	pstZoneClan = z_id_clan( pstEnv,iClanID);
	if(NULL == pstZoneClan)
	{
		return -1;
	}

	pstPworldInst = z_find_clan_machine_pworld_inst(pstEnv, pstZoneClan);
	if(NULL == pstPworldInst || pstPworldInst->bOver)
	{
		return -1;
	}

	if (IsSameDay(pstPlayer->stRoleData.MiscInfo.ShuNiuInfo.Time, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		if (pstPlayer->stRoleData.MiscInfo.ShuNiuInfo.Wid != pstPworldInst->ullPworldWID)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STRONG244);
			return -1;
		}
	}

	if (pstSelfClan != pstZoneClan)
	{
		if (!pstPworldInst->cShareShuNiu)
		{
			return -1;
		}
	}
	
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if(NULL == pstPworld)
	{
		return -1;
	}
	
	if(0 == z_player_enter_pworld_by_inst(pstEnv, pstPlayer, pstPworld, pstPworldInst))
	{
		player_action_clan_machine_pworld(pstEnv,pstPlayer,pstPworldInst);
	}
	return 0;
}

//创建公会守护枢纽副本
int clan_machine_pworld_create(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan, int iShare)
{	
	ZONEIDX stIdx;
	ZONEPWORLD * pstPworld;
	ZoneClan *pstOpenClan;
	ZONEPWORLDINST *pstPInst;
	
	// 一个服只能有一个联合模式开启
	if (iShare && pstEnv->iShuNiuClanID > 0)
	{
		pstOpenClan = z_id_clan( pstEnv, pstEnv->iShuNiuClanID);
		if (pstOpenClan)
		{
			pstPInst = z_find_clan_machine_pworld_inst(pstEnv, pstOpenClan);
			if (pstPInst && !pstPInst->bOver)
			{
				return -3;
			}
		}
	}
	
	if(IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec,
				pstZoneClan->stClan.ClanCore.MachinePworldLastOpenTime))
	{
		return -1;
	}

	if(IsSameWeek(pstEnv->pstAppCtx->stCurr.tv_sec,
				pstZoneClan->stClan.ClanBrief.CreateTime))
	{
		return -2;
	}

	if (IsSameWeek(pstEnv->pstAppCtx->stCurr.tv_sec,
				pstZoneClan->stClan.ClanCore.MachinePworldLastOpenTime) &&
		pstZoneClan->stClan.ClanCore.MachinePworldWeekTimes >= CLAN_MACHINE_PWORLD_WEEK_MAX)
	{
		return -1;
	}

	stIdx.iID = CLAN_MACHINE_PWORLD_ID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if(NULL == pstPworld)
	{
		return -1;
	}

	z_new_clan_machine_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstPworld, 
									pstZoneClan, pstPworld->stPworldDef.Timeout*60,iShare);
	return 0;
}

Monster *new_normal_mon(ZONESVRENV *pstEnv, ZONEMAPINST *pstMapInst, 
		int iMonID, int iDir, RESPOS *pstPos)
{
	Monster *pstMon, stMonster;
	MONSTERDEF *pstMonDef;
	
	memset(&stMonster, 0, sizeof(stMonster));
	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.stInitPos = *pstPos;
	stMonster.bDirection = iDir;
	stMonster.stCurrPos = stMonster.stInitPos;

	pstMonDef = z_get_mon_def(pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		return NULL;
	}

	pstMon =	z_new_monster(pstEnv->pstAppCtx, pstEnv, &stMonster, pstMonDef);
	if( !pstMon )
	{
		return NULL;
	}

	pstMon->cFirstBorn = 1;
	if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstMon, NULL))
	{
		tmempool_free(pstEnv->pstMonsterPool, pstMon->iMIndex);
		return NULL;
	}
	pstMon->cFirstBorn = 0;

	return pstMon;
}


int clan_everyday_pworld_award(ZONESVRENV* pstEnv, ZoneClan *pstClan, CLANLEVELDEF *pstClanLevel)
{
	int i;
	RESPOS stPos;
	Player *pstPlayer = NULL;
	ZONEMAPINST * pstMapInst;
	ZONEPWORLDINST *pstZonePworldInst;
	
	for(i = 0; i < pstClan->stClan.ClanProf.MemNum; ++i)
	{
		if (!pstClan->stClan.ClanCore.ClanMem[i].Online)
		{
			continue;
		}

		pstPlayer = z_name_player(
			pstEnv->pstAppCtx,
			pstEnv,
			pstClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName);
		if (!pstPlayer)
		{
			continue;
		}

		if(GET_PWORLDID(pstPlayer->stRoleData.Map) != CLAN_EVERYDAY_PWORLD_ID)
		{
			continue;
		}

		g_iOptype = OP_OPTYPE_DAYPWORLD;
		clan_resource_inc(pstEnv, pstClan, pstPlayer, CLAN_OWNED_CONTRIBUT,
			pstClanLevel->ClanValContri);
		g_iOptype = 0;
		
	}

	player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN, 
		ZONEERR_CLANBROAD255,
		CLAN_EVERYDAY_PWORLD_MAP_ID, 16700, 19000, pstClanLevel->ClanValContri);

	pstZonePworldInst = z_find_clan_everyday_pworld_inst(pstEnv, pstClan);
	if(NULL == pstZonePworldInst)
	{
		return -1;
	}

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstZonePworldInst->astMapInstIdx[0]);
	if(pstMapInst)
	{
		stPos.X = 16700;
		stPos.Y = 19000;
	
		new_normal_mon(pstEnv, pstMapInst, 284275, 0, &stPos);
		pstClan->stZoneClanInfo.iBossLife = 1;
	}

	return 0;
}

//公会积分变化
int clan_everyday_pworld_val(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan, CLANLEVELDEF *pstClanLevel, int iVal)
{
	if(pstZoneClan->stZoneClanInfo.iEveryDayPworldVal >= pstZoneClan->stZoneClanInfo.iMaxEveryDayPworldVal)
	{
		return 0;
	}
	
	pstZoneClan->stZoneClanInfo.iEveryDayPworldVal += iVal;
	if(pstZoneClan->stZoneClanInfo.iEveryDayPworldVal >= pstZoneClan->stZoneClanInfo.iMaxEveryDayPworldVal)
	{
		pstZoneClan->stZoneClanInfo.iEveryDayPworldVal = pstZoneClan->stZoneClanInfo.iMaxEveryDayPworldVal;

		clan_everyday_pworld_award(pstEnv, pstZoneClan, pstClanLevel);
	}
	
	clan_broadcast_do_pkg(pstEnv, pstZoneClan, player_everyday_pworld_ui_info);

	return 0;
}

//销毁公会积分活动副本
int clan_destroy_everyday_pworld(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan)
{
	ZONEPWORLDINST *pstZonePworldInst;
	
	pstZonePworldInst = z_find_clan_everyday_pworld_inst(pstEnv, pstZoneClan);
	if(NULL == pstZonePworldInst)
	{
		return -1;
	}

	if(0 > z_in_destroy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstZonePworldInst)) 
	{
		return -1;
	}
	
	return 0;
}

/*	-lxk
	功能:判断角色是否在自己的公会城市里
*/
int is_in_my_clan_city(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZoneClan *pstZoneClan;
	ZONEPWORLDINST *pstZonePworldInst;
	ZONEMAPINST *pstMapInst;
	ZONEIDX stMapIdx;
	
	pstZoneClan = player_get_clan(pstEnv,  pstPlayer);
	if (NULL == pstZoneClan)
	{
		return 0;
	}

	pstZonePworldInst = z_find_clan_city_inst(pstEnv, pstZoneClan);
	if (NULL == pstZonePworldInst)
	{
		return 0;
	}

	stMapIdx.iID = pstPlayer->stRoleData.Map;
	stMapIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stMapIdx);
	if(NULL == pstMapInst)
	{
		return 0;
	}
	
	//if(pstPlayer->stRoleData.Map == pstZonePworld.iMap && pstPlayer->stRoleData.MiscInfo.PworldInstID = pstZonePworldInst->iPworldInstID)
	//if(pstPlayer->stRoleData.MiscInfo.PworldWID == pstZonePworldInst->ullPworldWID)
	if(pstMapInst->iPworldInstID == pstZonePworldInst->iPworldInstID)
	{
		return 1;
	}
	return 0;
}

int player_exit_clan_clear_everydaypworld(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan, Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIVEGAMESVR *pstActiveGameSvr = &stPkg.Body.ActiveGameSvr;
	CSACTIVEGAMEUICLOSE *pstActiveUIClose = &pstActiveGameSvr->Data.UIClose;

	if (NULL == pstZoneClan)
	{
		return 0;
	}

	if(EVERYDAY_PWORLD_OPEN != clan_everyday_pworld_stat_get(pstEnv, pstZoneClan))
	{
		return 0;
	}
	
	if(GET_PWORLDID(pstPlayer->stRoleData.Map) == CLAN_EVERYDAY_PWORLD_ID)
	{
		player_pworld_exit_chgmap(pstEnv, pstPlayer);
	}

	pstActiveGameSvr->Type = ACTIVE_GAME_SVR_TYPE_UI_CLOSE;
	pstActiveUIClose->UIType = ACTIVE_UI_TYPE_CLAN_PWORLD;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTIVE_GAME_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int active_game_award(ZONESVRENV* pstEnv, Player *pstPlayer, CSACTIVEGAMEAWARD *pstAward)
{
	switch(pstAward->UIType)
	{
		case ACTIVE_UI_TYPE_CLAN_PWORLD:
			return player_clan_everyday_pworld_award(pstEnv, pstPlayer);
			break;
		default:
			break;
	}

	return 0;
}

int active_game_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
			TFRAMEHEAD	 *pstFrameHead, CSPKG *pstCsPkg)
{
	CSACTIVEGAMECLT *pstActiveGameClt = &pstCsPkg->Body.ActiveGameClt;
	
	switch(pstActiveGameClt->Type)
	{
		case ACTIVE_GAME_CLT_TYPE_AWARD:
			return active_game_award(pstEnv, pstPlayer, &pstActiveGameClt->Data.Award);
			break;
		default:
			break;
	}

	return 0;
	UNUSED(pstAppCtx);
	UNUSED(pstFrameHead);
}
/*	-lxk
	功能:公会建筑受到攻击对公会成员发出警报
*/
int clan_city_build_warning_byhit(ZONESVRENV *pstEnv, Monster *pstMon)
{	
	MONSTERDEF * pstMonsterDef;
	ZoneClan * pstZoneClan;
	pstMonsterDef = z_get_mon_def(pstEnv, pstMon);
	if(NULL == pstMonsterDef)
	{
		return -1;
	}
	if(pstMonsterDef->MiscType != MON_MISC_TYPE_CLAN_BUILD)
	{
		return -1;
	}	
	pstZoneClan = z_id_clan(pstEnv, pstMon->nOwnerFastID);
	if(NULL == pstZoneClan)
	{
		return -1;
	}
	if(!pstMon->iWarningTime || ( pstMon->iWarningTime + 60 <= pstEnv->pstAppCtx->stCurr.tv_sec ))
	{
		pstMon->iWarningTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN, 
			ZONEERR_CLANBROAD256,
			pstMonsterDef->Name,GET_MAPID(pstMon->stMap.iID),pstMon->stCurrPos.X,pstMon->stCurrPos.Y);
	}
	return 0;

}

int clan_city_build_warning_die(ZONESVRENV *pstEnv, Monster *pstMon,Player * pstKiller)
{	
	MONSTERDEF * pstMonsterDef;
	ZoneClan * pstZoneClan;
	ZoneClan * pstZoneClanKiller;
	if(NULL == pstKiller)
	{
		return -1;
	}
	pstMonsterDef = z_get_mon_def(pstEnv, pstMon);
	if(NULL == pstMonsterDef)
	{
		return -1;
	}
	if(pstMonsterDef->MiscType != MON_MISC_TYPE_CLAN_BUILD)
	{
		return -1;
	}	
	pstZoneClan = z_id_clan(pstEnv, pstMon->nOwnerFastID);
	if(NULL == pstZoneClan)
	{
		return -1;
	}
	pstZoneClanKiller = player_get_clan(pstEnv, pstKiller);
	if(NULL == pstZoneClanKiller)
	{
		player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN, 
								  ZONEERR_CLANBROAD257,
								  pstMonsterDef->Name,pstKiller->stRoleData.RoleName);
	}
	else
	{
		player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN, 
								  ZONEERR_CLANBROAD258,
								  pstMonsterDef->Name,pstZoneClanKiller->stClan.ClanProf.Name,pstKiller->stRoleData.RoleName);
	}
	return 0;

}

//公会城市传送卷检查
int item_back_to_clan_city_check(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	ZONEIDX stIdx;
	MapIndex * pstMapIdx = NULL;
	ZONEMAPINST *pstMapInst = NULL; 

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx,pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	if(pstMapIdx->stMapDef.CtrlFlag & MAP_CTRL_NO_USER_CLAN_SUMMON)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN294);
		return -1;
	}

	if(GET_PWORLDID(pstPlayer->stRoleData.Map) == DIANFENG_PWORLD_ID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_DIANFENG2);
		return -1;
	}

	return 0;
}

