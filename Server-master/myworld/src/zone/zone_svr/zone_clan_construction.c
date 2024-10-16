/*
**  @file $RCSfile: zone_clan_construction.c,v $
**  general description of this module
**  $Id: zone_clan_construction.c,v 1.58 2013/10/24 08:16:59 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2013/10/24 08:16:59 $
**  @version $Revision: 1.58 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "proto_comm.h"
#include "proto_cs.h"
#include "role_db_meta.h"
#include "zone_clan.h"
#include "zone_clan_city.h"
#include "zone_clan_advance.h"
#include "zone_clan_construction.h"
#include "zone_clt.h"
#include "zone_svr.h"
#include "zone_relation.h"
#include "zone_oplog.h"
#include "zone_range.h"
#include "zone_player.h"
#include "zone_npc.h"
#include "zone_map.h"
#include "zone_package.h"
#include "zone_combat.h"
#include "zone_err.h"
#include "zone_zh_name.h"

extern int g_iOptype;
extern int g_iOpsubtype;

#define COND_DO_(cond, expr) (void)((cond) ? (expr) : 0)

const char *kClanBuildingName[] = {
    [CLAN_IDX_TECH_CENTER]       = LJY_CLAN_TECH_CENTER,
    [CLAN_IDX_COLLECTION_CENTER] = LJY_CLAN_COLLECTION_CENTER,
    [CLAN_IDX_FACTORY]           = LJY_CLAN_FACTORY,
    [CLAN_IDX_FARM]              = LJY_CLAN_FARM,
    [CLAN_IDX_WALL]              = LJY_CLAN_WALL,
    [CLAN_IDX_CITY]              = LJY_CLAN_CITY,
};

static const char *kClanBuilding2Tech[] = {
    [CLAN_IDX_TECH_CENTER]       = LJY_CLAN_TECH_CENTER1,
    [CLAN_IDX_COLLECTION_CENTER] = LJY_CLAN_COLLECTION_CENTER1,
    [CLAN_IDX_FACTORY]           = LJY_CLAN_FACTORY1,
    [CLAN_IDX_FARM]              = LJY_CLAN_FARM1,
    [CLAN_IDX_WALL]              = LJY_CLAN_WALL1,
    [CLAN_IDX_CITY]              = LJY_CLAN_CITY1,
};

// 删除全部公会城市额外NPC
static int clan_city_del_extra_npc(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan);

// 创建全部公会城市额外NPC
static int clan_city_new_extra_npc(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan);

// 检查公会资源是否够提升公会/城市/建筑等级
static int clan_upgrade_required_check(
	ZONESVRENV *pstEnv,
	ZoneClan *pstZoneClan,
	Player *pstPlayer,
	int fCheckScopedUTP,
	const char *pszAction,
	const CLANUPGRADEREQUIRED *pstRequired);

// 检查公会建筑的科技需求
static int clan_building_upgrade_required_check(
	ZONESVRENV *pstEnv,
	ZoneClan *pstZoneClan,
	Player *pstPlayer,
	int iBuildingIdx,
	const CLANCITYBUILDINGLEVELDEF *pstDef,
	int fOnlyCheckUsedTechPoint);

// 消耗提升公会/城市/建筑等级的公会资源
static int clan_upgrade_required_cost(
	ZONESVRENV *pstEnv,
	ZoneClan *pstZoneClan,
	Player *pstPlayer,
	const CLANUPGRADEREQUIRED *pstRequired);

// 公会建筑通知
static int clan_building_notify(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan,
                                const CLANCITYBUILDING *pstBuilding,
                                int iNotify);

// 移除公会城市里的障碍物
static int clan_city_remove_barrier(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                    int iDefID);

// 获取公会城市地图实例
static inline ZONEMAPINST *clan_city_map_inst(ZONESVRENV *pstEnv,
                                              ZoneClan *pstZoneClan)
{
	ZONEPWORLDINST *pstZonePworldInst = z_find_clan_city_inst(pstEnv,
	                                                          pstZoneClan);
	if (!pstZonePworldInst)
	{
		return NULL;
	}
	return z_find_map_inst(pstEnv->pstAppCtx, pstEnv,
	                       &pstZonePworldInst->astMapInstIdx[0]);
}


int map_entities_count_if(ZONESVRENV *pstEnv, const ZONEMAPINST *pstMapInst,
                          void *pvCookie, CountPred pfnPred);


int clan_upgrade(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, Player *pstPlayer)
{
	CLANLEVELDEF *pstNextDef = z_clan_level_def(pstEnv,
	                                    pstZoneClan->stClan.ClanProf.Level + 1);
	OPCLANDATA stData;

	if (pstZoneClan->stClan.ClanProf.Level >= CLAN_MAX_LEVEL)
	{
		// 等级已达上限
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN310,
		           pstZoneClan->stClan.ClanProf.Level);
		return -1;
	}

	if (!pstNextDef)
	{
		return -1;
	}

	if (pstZoneClan->stClan.ClanCity.Level < pstNextDef->RequiredCityLv)
	{
		// 公会城市等级不足
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN311,
		           pstNextDef->RequiredCityLv);
		return -1;
	}

	if (clan_upgrade_required_check(pstEnv, pstZoneClan, pstPlayer, 0,
	                                LJY_CLAN101, &pstNextDef->Required) < 0)
	{
		return -1;
	}
	
	if (clan_upgrade_required_cost(pstEnv, pstZoneClan, pstPlayer,
		                           &pstNextDef->Required) < 0)
	{
		return -1;
	}

	stData.ClanLevelCh.ClanLevelPre = pstZoneClan->stClan.ClanProf.Level;

	clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_LEVEL, 1, 0);

	stData.ClanLevelCh.ClanLevel = pstZoneClan->stClan.ClanProf.Level;

	clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_MONEY, pstNextDef->PresentClanMoney, 0);

	// 升级后的数值变化
	/*
	pstZoneClan->stClan.ClanAdvance.Storage.MaxMachineNum =
		pstNextDef->MaxMachineGrid;
	clan_property_notify(pstEnv, pstZoneClan,
						 CLAN_PROPERTY_MAX_MACHINE_GRID_NUM);

	pstZoneClan->stClan.ClanAdvance.Storage.MaxResourceNum =
		pstNextDef->MaxResourceGrid;
	clan_property_notify(pstEnv, pstZoneClan,
						 CLAN_PROPERTY_MAX_RESOURCE_GRID_NUM);
	*/
	++pstZoneClan->stZoneClanInfo.iListVersion;
	clan_set_dirty(pstZoneClan, CACHE_FLAG_IMM);

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN312,
	           pstZoneClan->stClan.ClanProf.Level);


	// 通知全公会
	player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD259,
	                          pstPlayer->stRoleData.RoleName,
	                          pstZoneClan->stClan.ClanProf.Level - 1,
	                          pstZoneClan->stClan.ClanProf.Level);

	clan_effect_broadcast(pstEnv, pstZoneClan, 0, pstNextDef->EffectString);

	player_clan_view_chg(pstEnv, pstZoneClan, CLAN_VIEW_CHG_MOD);

	z_role_clan_oplog(pstEnv, pstPlayer->stRoleData.RoleName, clan_id_warp(&pstZoneClan->stClan),
			  OP_CLAN_LEVEL_CH,&stData);

	

	//成长之路
	{
		ACHIEVEMENTCOND stCond;
		int i = 0;
		Player *pstClanPlayer = NULL;

		memset(&stCond,0,sizeof(stCond));
		
		stCond.Type = COND_CLAN_LEVEL;
		stCond.Val1 = pstZoneClan->stClan.ClanProf.Level;

		i = pstZoneClan->stClan.ClanProf.MemNum;
		while(i--)
		{
			if (!pstZoneClan->stClan.ClanCore.ClanMem[i].Online)
			{
				continue;
			}

			pstClanPlayer = z_name_player(pstEnv->pstAppCtx,
				pstEnv,
				pstZoneClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName);
			if (!pstClanPlayer)
			{
				continue;
			}

			z_player_achievement_cond(pstEnv,pstClanPlayer,&stCond);
			
		}
	}

	//公会升级后,技能有效等级恢复
	{
		int iNum = MAX_CLAN_SKILL_NUM;
		CLANSKILLINFO* pstInfo = NULL;
		int i = 0;
		int k = 0;
		CLANSKILLDEF* pstDef = NULL;
		int iChg = 0;
	
		
		pstInfo = &pstZoneClan->stClan.ClanAdvance.ClanSkillInfo;
		
		iNum = min(iNum,pstInfo->Num);


		for(i = 0; i < iNum; i++)
		{
			if(MEM_NUM_CLAN_SKILL_ID == pstInfo->ClanSkill[i].SkillID)
			{
				continue;
			}
			
			int iValidLevel = pstInfo->ClanSkill[i].ValidLevel;

			if (iValidLevel == pstInfo->ClanSkill[i].Level)
			{
				continue;
			}
			
			for( k = pstInfo->ClanSkill[i].ValidLevel; k <= pstInfo->ClanSkill[i].Level; k++)
			{
				pstDef = z_find_clan_skill_def(pstEnv,pstInfo->ClanSkill[i].SkillID ,k);
				if (!pstDef)
				{
					continue;
				}
				if (pstDef->ResearchNeedClanLevel > pstZoneClan->stClan.ClanProf.Level)
				{
					break;
				}
				else
				{					
					iValidLevel = k;
				}
			}
			
			if (iValidLevel != pstInfo->ClanSkill[i].ValidLevel)
			{
				iChg = 1;
				pstInfo->ClanSkill[i].ValidLevel = iValidLevel;
				clan_skill_valid_level_chg(pstEnv,pstZoneClan,&pstInfo->ClanSkill[i]);

				if(BUSINESS_CLAN_SKILL_ID == pstInfo->ClanSkill[i].SkillID)
				{
					//公会商运技能有效级别一改。缓存中的ClanBusinessExpMul 也要修改
					clan_skill_business_exp_jian_yan(pstEnv,pstZoneClan);
				}
			}
		}

		if (iChg)
		{
			clan_action_clan_skill_info(pstEnv,pstZoneClan);
		}
	}
	
	return 0;
}


int clan_city_upgrade(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                      Player *pstPlayer, const char *pszName)
{
	int fNew = 0;
	CLANCITY *pstCity = &pstZoneClan->stClan.ClanCity;
	const CLANLEVELDEF *pstDef = z_clan_level_def(pstEnv,
	                                        pstZoneClan->stClan.ClanProf.Level);
	const CLANCITYLEVELDEF *pstNextDef = z_clan_city_level_def(pstEnv,
	                                                        pstCity->Level + 1);
	OPCLANDATA stData;

	if (!pstDef || !pstNextDef)
	{
		return -1;
	}
	if (pstCity->Level >= pstDef->LvLimited[CLAN_IDX_CITY])
	{
		if (pstDef->LvLimited[CLAN_IDX_CITY] > 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
			          ZONEERR_CLAN313 ,
			           pstZoneClan->stClan.ClanProf.Level,
			           pstDef->LvLimited[CLAN_IDX_CITY]);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_TIP,
			           ZONEERR_CLAN314,
			           pstZoneClan->stClan.ClanProf.Level);
		}
		return -1;
	}

	fNew = (pstCity->Level == 0) ? 1 : 0; // 从零级开始升级说明实际是申请公会城市
	if (clan_upgrade_required_check(pstEnv, pstZoneClan, pstPlayer, 0,
	                                fNew ? LJY_CLAN102 : LJY_CLAN103,
		                            &pstNextDef->Required) < 0)
	{
		return -1;
	}
	
	if(!fNew)
	{
		// 删除额外的NPC
		clan_city_del_extra_npc(pstEnv, pstZoneClan);
	}

	if(fNew)
	{
		if(pszName && pszName[0])
		{
			STRNCPY(pstCity->Name, pszName, CCH(pstCity->Name));
		}
		if(z_new_clan_city(pstEnv, pstZoneClan) < 0) // 申请鸟城市，创建一个先
		{
			pstZoneClan->stZoneClanInfo.ullCityPworldWID = 0ULL;
			pstZoneClan->stZoneClanInfo.iCityPworldInstID = 0;
			
			tlog_error(pstEnv->pstLogCat, 0, 0,	"new clan city fail");
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN315);
			return -1;
		}
	}
	
	if (clan_upgrade_required_cost(pstEnv, pstZoneClan, pstPlayer,
		                           &pstNextDef->Required) < 0)
	{
		return -1;
	}

	stData.ClanCityLevelCh.CityLevelPre = pstCity->Level;
	
	clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_CITY_LEVEL, 1, 0);
	
	stData.ClanCityLevelCh.CityLevel = pstCity->Level;

	clan_city_new_extra_npc(pstEnv, pstZoneClan);
	
	// 通知全公会
	if(fNew)
	{
		//player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
		                          //"%s 申请了公会城市",
		                          //pstPlayer->stRoleData.RoleName);
	}
	else
	{
		
		player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
		                          ZONEERR_CLANBROAD260,
		                          pstPlayer->stRoleData.RoleName,
		                          pstCity->Level - 1,
		                          pstCity->Level);
	}
	clan_effect_broadcast(pstEnv, pstZoneClan, 0, pstNextDef->EffectString);

	z_role_clan_oplog(pstEnv, pstPlayer->stRoleData.RoleName, clan_id_warp(&pstZoneClan->stClan),
		  OP_CLAN_CITY_LEVEL_CH,&stData);

	clan_city_chg(pstEnv,pstZoneClan);
	
	return 0;
}


int clan_city_building_upgrade(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan,
	                           Player *pstPlayer, int iBuildingIdx)
{
	CLANCITYBUILDING *pstBuilding = NULL;
	CLANCITYBUILDINGLEVELDEF *pstNextDef = NULL;
	const CLANLEVELDEF *pstDef = z_clan_level_def(pstEnv,
	                                        pstZoneClan->stClan.ClanProf.Level);

	if (!pstDef)
	{
		return -1;
	}

	if (iBuildingIdx < CLAN_IDX_TECH_CENTER || iBuildingIdx > CLAN_IDX_WALL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN316);
		return -1;
	}
	pstBuilding = pstZoneClan->stClan.ClanCity.Buildings + iBuildingIdx;

	if (pstBuilding->Pending)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN317);
		return -1;
	}

	if (pstBuilding->Level >= pstDef->LvLimited[iBuildingIdx])
	{
		if (pstDef->LvLimited[iBuildingIdx] > 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
			           ZONEERR_CLAN318,
			           pstZoneClan->stClan.ClanProf.Level,
			           pstDef->LvLimited[iBuildingIdx]);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
			           ZONEERR_CLAN319,
			           pstZoneClan->stClan.ClanProf.Level);
		}
		return -1;
	}

	if (!(pstNextDef = z_clan_city_building_level_def(pstEnv,
		                                              pstBuilding->Level + 1,
		                                              iBuildingIdx)))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN320);
		return -1;
	}

	if (clan_building_upgrade_required_check(pstEnv, pstZoneClan, pstPlayer,
	                                         iBuildingIdx, pstNextDef, 0) < 0)
	{
		return -1;
	}

	if (clan_upgrade_required_check(pstEnv, pstZoneClan, pstPlayer, 1,
	                                !pstBuilding->Level ? LJY_CLAN104 : LJY_CLAN105,
		                            &pstNextDef->Required) < 0)
	{
		return -1;
	}

	if (clan_upgrade_required_cost(pstEnv, pstZoneClan, pstPlayer,
		                           &pstNextDef->Required) < 0)
	{
		return -1;
	}

	// 移除障碍物, 如果存在的话
	clan_building_remove_barrier(pstEnv, pstZoneClan, iBuildingIdx,
	                             pstBuilding->Level + 1);
		                                              
	pstBuilding->Pending = 1;
	pstBuilding->BeginTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstBuilding->EndTime = pstBuilding->BeginTime + pstNextDef->TimeCost;
	clan_set_dirty_notify(pstZoneClan, CACHE_FLAG_IMM);

	// 通知客户端: 开始建造建筑
	clan_building_notify(pstEnv, pstZoneClan, pstBuilding, CLAN_BUILDING_BEGIN);

	if (!pstBuilding->Level)
	{
		char szTime[256];
		memset(szTime,0,sizeof(256));
		
		player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
		                          ZONEERR_CLANBROAD261,
		                          pstPlayer->stRoleData.RoleName,
		                          kClanBuildingName[iBuildingIdx]
		                          ,time_text_zh(pstNextDef->TimeCost,szTime));
	}
	else
	{
		char szTime[256];
		memset(szTime,0,sizeof(256));
		
		player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
		                          ZONEERR_CLANBROAD262,
		                          pstPlayer->stRoleData.RoleName,
		                          kClanBuildingName[iBuildingIdx],
		                          pstBuilding->Level,time_text_zh(pstNextDef->TimeCost,szTime));
	}

	clan_city_chg(pstEnv,pstZoneClan);
	
	return 0;
}


int player_clan_city_info(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSCLANSVR *pstClanSvr = &stPkg.Body.ClanSvr;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);

	if(!pstZoneClan)
	{
		return -1;
	}

	pstClanSvr->Operation = CLAN_SVR_CITY_INFO;
	pstClanSvr->ClanSvrData.ClanCityInfo = pstZoneClan->stClan.ClanCity;

	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_clan_enter_city(ZONESVRENV* pstEnv, Player *pstPlayer,
                           RESPOS *pstPos)
{
	ZoneClan *pstZoneClan = NULL;

	pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstZoneClan)
	{
		COND_DO_(pstPlayer, z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_SVR,
		                               ZONEERR_CLAN321));
		return -1;
	}

	if (!pstZoneClan->stClan.ClanCity.Level)
	{
		COND_DO_(pstPlayer, z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR,
		                               ZONEERR_CLAN322));
		return -1;
	}

	if (!pstZoneClan->stZoneClanInfo.ullCityPworldWID ||
		!pstZoneClan->stZoneClanInfo.iCityPworldInstID)
	{
		COND_DO_(pstPlayer, z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_SVR,
		                              ZONEERR_CLAN323));
		return -1;
	}

	if (clan_mem_enter_city(pstEnv, pstPlayer, pstPos) < 0)
	{
		COND_DO_(pstPlayer, z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR,
		                               ZONEERR_CLAN324));
		return -1;
	}

	return 0;
}


int player_clan_leave_city(ZONESVRENV *pstEnv, Player *pstPlayer,
                           const ZoneClan *pstZoneClan)
{
	const ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;

	if (!pstZoneClan)
	{
		return -1;
	}

	if (!pstZoneClan->stClan.ClanCity.Level)
	{
		return -1;
	}

	if (!pstZoneClan->stZoneClanInfo.ullCityPworldWID ||
		!pstZoneClan->stZoneClanInfo.iCityPworldInstID)
	{
		return -1;
	}

	if (pstZoneClan->stZoneClanInfo.ullCityPworldWID !=
		pstMiscInfo->PworldWID ||
		pstZoneClan->stZoneClanInfo.iCityPworldInstID !=
		pstMiscInfo->PworldInstID)
	{
		return -1;
	}

	return player_pworld_exit_chgmap(pstEnv, pstPlayer);
}

int clan_construction_tick(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	int i = 0;
	const int kCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	CLANCITY *pstCity = &pstZoneClan->stClan.ClanCity;
	OPCLANDATA stData;

	for(i = CLAN_IDX_TECH_CENTER; i < CLAN_IDX_WALL; ++i)
	{
		CLANCITYBUILDING *pstBuilding = pstCity->Buildings + i;

		if (pstBuilding->Pending && pstBuilding->EndTime < kCurr)
		{
			const CLANCITYBUILDINGLEVELDEF *pstDef =
				z_clan_city_building_level_def(pstEnv, ++pstBuilding->Level, i);
			if (!pstDef)
			{
				--pstBuilding->Level;
				continue;
			}
			stData.ClanBuildingCh.ClanBuildingType = i;
			stData.ClanBuildingCh.ClanBuildingLevelPre = pstBuilding->Level - 1;
			stData.ClanBuildingCh.ClanBuildingLevel = pstBuilding->Level;
			
			pstBuilding->Pending = 0;
			pstBuilding->BeginTime = 0;
			pstBuilding->EndTime = 0;
			clan_set_dirty_notify(pstZoneClan, CACHE_FLAG_IMM);

			// 通知客户端: 建造完成
			clan_building_notify(pstEnv, pstZoneClan, pstBuilding,
			                     CLAN_BUILDING_COMPLETED);

			if (pstBuilding->Level <= 1)
			{
				player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
				                          ZONEERR_CLANBROAD263, kClanBuildingName[i]);
			}
			else
			{
				player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
				                          ZONEERR_CLANBROAD264,
				                          kClanBuildingName[i],
				                          pstBuilding->Level);
			}
			clan_effect_broadcast(pstEnv, pstZoneClan, 0, pstDef->EffectString);

			z_role_clan_oplog(pstEnv, NULL, clan_id_warp(&pstZoneClan->stClan),
				OP_CLAN_BUILDING_LEVEL_CH,&stData);

			clan_city_chg(pstEnv,pstZoneClan);
		}
	}
	return 0;
}

int player_clan_upgrade(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);

	clan_upgrade(pstEnv, pstZoneClan, pstPlayer);
	return 0;
}


int player_clan_city_upgrade(ZONESVRENV *pstEnv, Player *pstPlayer, 
                             const char *pszClanCityName)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);

	/*
	if(check_npc_ui(pstEnv, pstPlayer, UI_TYPE_UPGRADE_CLAN_CITY) < 0)
	{
		return -1;
	}
	*/

	clan_city_upgrade(pstEnv, pstZoneClan, pstPlayer, pszClanCityName);
	return 0;
}

int player_clan_city_update(ZONESVRENV *pstEnv, Player *pstPlayer,ZoneClan *pstZoneClan)
{
	
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;
	
	
	memset(pstAction,0,sizeof(*pstAction));
	
	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_CLAN_CITY_UPDATE;


	memcpy(pstZoneClan->stClan.ClanCity.Name,pstZoneClan->stClan.ClanProf.Name,sizeof(pstZoneClan->stClan.ClanCity.Name));

	pstZoneClan->stClan.ClanCity.Name[CLAN_MAX_CITY_NAME_LEN - 1] = 0;
	
	pstAction->Data.ClanCityUpdate.ClanID = pstZoneClan->stClan.ClanProf.Id;
	pstAction->Data.ClanCityUpdate.ClanCity = pstZoneClan->stClan.ClanCity;
	pstAction->Data.ClanCityUpdate.PlayerClanID = pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int clan_city_chg(ZONESVRENV *pstEnv,ZoneClan *pstZoneClan)
{
	ZONEPWORLDINST *pstZonePworldInst = z_find_clan_city_inst(pstEnv, pstZoneClan);
	int i = 0;

	if (NULL == pstZonePworldInst)
	{
		return -1;
	}

	for(i = 0; i < pstZonePworldInst->iPlayer; i++)
	{
		Player* pstPlayer = z_id_player(pstEnv->pstAppCtx,pstEnv,pstZonePworldInst->aiPlayerID[i]);
		if (pstPlayer)
		{
			player_clan_city_update(pstEnv,pstPlayer,pstZoneClan);
		}
	}
	
	
	return 0;
}

int player_clan_city_building_upgrade(ZONESVRENV *pstEnv, Player *pstPlayer,
                                      int iBuildingID)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);

	//if(check_npc_ui(pstEnv, pstPlayer, iBuildingID) < 0)
	//{
	//	return -1;
	//}

	clan_city_building_upgrade(pstEnv, pstZoneClan, pstPlayer, iBuildingID);
	return 0;
}


int player_clan_info(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	CSCLANINFO *pstInfo = &stPkg.Body.ClanSvr.ClanSvrData.ClanInfo;

	pstInfo->ProfInfo = pstClan->stClan.ClanProf;
	pstInfo->PresidentID = pstClan->stClan.ClanBrief.Leader;
	STRNCPY(pstInfo->BulletinText, pstClan->stClan.ClanInner.GongGao,
	        CCH(pstInfo->BulletinText));
	pstInfo->BulletinTextLastEditTime =
		pstClan->stClan.ClanInner.GongGaoLastWrite;

	stPkg.Body.ClanSvr.Operation = CLAN_SVR_INFO;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


static int clan_upgrade_required_check(
	ZONESVRENV *pstEnv,
	ZoneClan *pstZoneClan,
	Player *pstPlayer,
	int fCheckScopedUTP,
	const char *pszAction,
	const CLANUPGRADEREQUIRED *pstRequired)
{
	int i = 0;
	CLANRESOURCE stFakeResource = pstZoneClan->stClan.ClanAdvance.Resource;
	
	if ((int)pstZoneClan->stClan.ClanProf.StableValue <
		pstRequired->StableValue)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN331,
		           pstRequired->StableValue -
		           pstZoneClan->stClan.ClanProf.StableValue,
		           pszAction);
		return -1;
	}

	if ((int)pstZoneClan->stClan.ClanProf.Money < pstRequired->Money)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN325,
		           pstRequired->Money - pstZoneClan->stClan.ClanProf.Money,
		           pszAction);
		return -1;
	}

	if (!fCheckScopedUTP &&
		pstZoneClan->stClan.ClanAdvance.UsedTechPoint <
		pstRequired->UsedTechPoint)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN326,
		           pstRequired->UsedTechPoint -
		           pstZoneClan->stClan.ClanAdvance.UsedTechPoint,
		           pszAction);
		return -1;
	}

	for(i = 0; i < (int)CCH(pstRequired->Material) &&
		pstRequired->Material[i].ID; ++i)
	{
		if (clan_resource_dec_entries_in(pstEnv, &stFakeResource,
			                               pstRequired->Material[i].ID,
			                               pstRequired->Material[i].Num,NULL) < 0)
		{
			const CLANPRODUCTDEF *pstDef = clan_product_def(pstEnv,
			                                       pstRequired->Material[i].ID);
			if (pstDef)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP,
				          ZONEERR_CLAN327, pstDef->Name, pszAction);
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
				           ZONEERR_CLAN328, pszAction);
			}
			return -1;
		}
	}
	return 0;
}

int clan_tech_used_tech_point(ZONESVRENV *pstEnv, int iCategroy, int iLevel)
{
	int i = 0, iPoint = 0;

	for(i = 1; i < iLevel + 1; ++i)
	{
		const CLANTECHDEF *pstDef = clan_tech_def(pstEnv, iCategroy, i);
		if (!pstDef)
		{
			iPoint = 0;
			break;
		}
		iPoint += pstDef->NeedTechPointCost;
	}
	return iPoint;
}

int clan_building_used_tech_point(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                  int iBuildingIdx)
{
	int iPoint = 0;
	const CLANTECH *pstTech = &pstZoneClan->stClan.ClanAdvance.Tech;

	switch(iBuildingIdx)
	{
	#define _PUT_TECH(category) \
    	iPoint += clan_tech_used_tech_point(pstEnv, category, \
    	                                    pstTech->TechLevel[category])

    case CLAN_IDX_COLLECTION_CENTER:
    	_PUT_TECH(CLAN_TECH_CATEGORY_COLLECTOR_ATTRIBUTES);
    	_PUT_TECH(CLAN_TECH_CATEGORY_COLLECTOR_MAKING);
    	_PUT_TECH(CLAN_TECH_CATEGORY_COLLECTOR_ALTERATION);
    	break;

    case CLAN_IDX_FACTORY:
		_PUT_TECH(CLAN_TECH_CATEGORY_COLLECTOR_ATTRIBUTES);
		_PUT_TECH(CLAN_TECH_CATEGORY_COLLECTOR_MAKING);
		_PUT_TECH(CLAN_TECH_CATEGORY_COLLECTOR_ALTERATION);
		_PUT_TECH(CLAN_TECH_CATEGORY_MACHINE_MAKING);
		_PUT_TECH(CLAN_TECH_CATEGORY_TANK_MAKING);
		_PUT_TECH(CLAN_TECH_CATEGORY_SPANNER);
		_PUT_TECH(CLAN_TECH_CATEGORY_GRENADE);
		_PUT_TECH(CLAN_TECH_CATEGORY_BAZOOKA);
		_PUT_TECH(CLAN_TECH_CATEGORY_FUEL_MACHINE);
		_PUT_TECH(CLAN_TECH_CATEGORY_CLAN_RIDE);
		_PUT_TECH(CLAN_TECH_CATEGORY_MACHINE_MON);
		_PUT_TECH(CLAN_TECH_CATEGORY_TANK);
		_PUT_TECH(CLAN_TECH_CATEGORY_MISSILE);
		_PUT_TECH(CLAN_TECH_CATEGORY_BOMB);
		_PUT_TECH(CLAN_TECH_CATEGORY_MINE);
		_PUT_TECH(CLAN_TECH_CATEGORY_ANTI_MINE_MACHINE);
		_PUT_TECH(CLAN_TECH_CATEGORY_ANTI_MINE_PKG);
		_PUT_TECH(CLAN_TECH_CATEGORY_BIG_MACHINE);
		_PUT_TECH(CLAN_TECH_CATEGORY_COLLECTOR_FUEL);
		_PUT_TECH(CLAN_TECH_CATEGORY_RESOURCE_CAPTURE);
		_PUT_TECH(CLAN_TECH_CATEGORY_SPY);
		_PUT_TECH(CLAN_TECH_CATEGORY_RADAR);
    	break;

    case CLAN_IDX_FARM:
    	_PUT_TECH(CLAN_TECH_CATEGORY_PET);
    	_PUT_TECH(CLAN_TECH_CATEGORY_PET_ALTERATION);
    	break;

    default:
    	break;
    #undef _PUT_TECH
	}
	return iPoint;
}

int clan_building_upgrade_usedtech_check(
	ZONESVRENV *pstEnv,
    ZoneClan *pstZoneClan,
	Player *pstPlayer,
	int iBuildingIdx)
{
	const CLANCITYBUILDING *pstBuilding = pstZoneClan->stClan.ClanCity.Buildings
	                                    + iBuildingIdx;
	const CLANCITYBUILDINGLEVELDEF *pstNextDef = z_clan_city_building_level_def(
		pstEnv, pstBuilding->Level + 1, iBuildingIdx);

	if (!pstNextDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN329,
		           kClanBuildingName[iBuildingIdx]);
		return -1;
	}

	return clan_building_upgrade_required_check(pstEnv, pstZoneClan, pstPlayer,
	                                            iBuildingIdx, pstNextDef, 1);
}

static int clan_building_upgrade_required_check(
	ZONESVRENV *pstEnv,
	ZoneClan *pstZoneClan,
	Player *pstPlayer,
	int iBuildingIdx,
	const CLANCITYBUILDINGLEVELDEF *pstDef,
	int fOnlyCheckUsedTechPoint)
{
	int iPoint = 0;
	const CLANUPGRADEREQUIRED *pstRequired = &pstDef->Required;
	const CLANCITYBUILDING *pstBuilding = pstZoneClan->stClan.ClanCity.Buildings
	                                    + iBuildingIdx;

	if (!pstRequired->UsedTechPoint)
	{
		goto no_tech_point_check;
	}
	switch(iBuildingIdx)
	{
    case CLAN_IDX_COLLECTION_CENTER:
    case CLAN_IDX_FACTORY:
    case CLAN_IDX_FARM:
    	iPoint = clan_building_used_tech_point(pstEnv, pstZoneClan,
    	                                       iBuildingIdx);
		if (pstBuilding->Level > 0)
		{
			if (iPoint < pstRequired->UsedTechPoint)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
				           ZONEERR_CLAN330,
				           pstRequired->UsedTechPoint - iPoint,
				           kClanBuilding2Tech[iBuildingIdx],
				           kClanBuildingName[iBuildingIdx]);
				return -1;
			}
		}
		else
		{
			if (iPoint < pstRequired->UsedTechPoint)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP,
				           ZONEERR_CLAN333,
				           kClanBuilding2Tech[iBuildingIdx],
				           kClanBuildingName[iBuildingIdx]);
				return -1;
			}	
		}
    	break;

    case CLAN_IDX_TECH_CENTER:
    case CLAN_IDX_WALL:
    case CLAN_IDX_CITY:
    	break;

    default:
    	return -1;
    	break;
	}

	if (fOnlyCheckUsedTechPoint)
	{
		return 0;
	}

no_tech_point_check:
	if (pstZoneClan->stClan.ClanCity.Level < pstDef->RequiredCityLv)
	{
		if (pstBuilding->Level > 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP,
		           ZONEERR_CLAN334,
		           pstDef->RequiredCityLv,
		           kClanBuildingName[iBuildingIdx]);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP,
		           ZONEERR_CLAN335,
		           pstDef->RequiredCityLv,
		           kClanBuildingName[iBuildingIdx]);
		}
		
		return -1;
	}
	return 0;
}

static int clan_upgrade_required_cost(
	ZONESVRENV *pstEnv,
	ZoneClan *pstZoneClan,
	Player *pstPlayer,
	const CLANUPGRADEREQUIRED *pstRequired)
{
	int i = 0;

	UNUSED(pstPlayer);

	g_iOptype = OP_OPTYPE_UPGRADE;
	if (clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_MONEY,
	                         pstRequired->Money, 1) < 0)
	{
		g_iOptype = 0;
		return -1;
	}

	for(i = 0; i < (int)CCH(pstRequired->Material) &&
		pstRequired->Material[i].ID; ++i)
	{
		if (clan_resource_dec_entries(pstEnv, pstZoneClan, 
			                         pstRequired->Material[i].ID,
			                         pstRequired->Material[i].Num) < 0)
		{
			g_iOptype = 0;
			return -1;
		}
	}
	g_iOptype = 0;
	
	return 0;
}

static int clan_city_del_extra_npc(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan)
{
	int i = 0;
	CLANCITYLEVELDEF *pstDef = z_clan_city_level_def(pstEnv,
		                                    pstZoneClan->stClan.ClanCity.Level);
	if(!pstDef)
	{
		return -1;
	}
	for(i = 0; i < (int)CCH(pstDef->ExtraNpcs) && pstDef->ExtraNpcs[i].NpcID;
		++i)
	{
		clan_city_npc_del_one(pstEnv, pstZoneClan, pstDef->ExtraNpcs[i].NpcID,
			                  &pstDef->ExtraNpcs[i].Pos);
	}
	return 0;
}

static int clan_city_new_extra_npc(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan)
{
	int i = 0;
	CLANCITYLEVELDEF *pstDef = z_clan_city_level_def(pstEnv,
		                                    pstZoneClan->stClan.ClanCity.Level);
	if(!pstDef)
	{
		return -1;
	}
	for(i = 0; i < (int)CCH(pstDef->ExtraNpcs) && pstDef->ExtraNpcs[i].NpcID;
		++i)
	{
		clan_city_npc_create_one(pstEnv, pstZoneClan, &pstDef->ExtraNpcs[i]);
	}
	return 0;
}

static int clan_building_notify(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan,
                                const CLANCITYBUILDING *pstBuilding,
                                int iNotify)
{
	CSPKG stPkg;
	CSCLANBUILDINGNOTIFY *pstData =
		&stPkg.Body.ClanSvr.ClanSvrData.ClanBuildingNotify;
	CLANCITY *pstCity = &pstZoneClan->stClan.ClanCity;

	switch(iNotify)
	{
	case CLAN_BUILDING_BEGIN:
		pstData->Data.Begin.Idx = pstBuilding - pstCity->Buildings;
		pstData->Data.Begin.BeginTime = pstBuilding->BeginTime;
		pstData->Data.Begin.EndTime = pstBuilding->EndTime;
		break;

	case CLAN_BUILDING_COMPLETED:
		pstData->Data.Completed = pstBuilding - pstCity->Buildings;
		break;

	default:
		return -1;
		break;
	}
	pstData->What = iNotify;

	stPkg.Body.ClanSvr.Operation = CLAN_SVR_BUILDING_NOTIFY;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	player_clan_broadcast(pstEnv, pstZoneClan, &stPkg, 0);
	return 0;
}

typedef struct tagDestroyCookie
{
	int *aiIDs;
	int iMax;
	int iDefID;
}DestroyCookie;

static int WillDestroyDefID(ZONESVRENV *pstEnv, int iID, void *pvCookie,
                            int *piCount)
{
	DestroyCookie *pstCookie = pvCookie;

	Monster *pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, iID);
	if(pstMon && pstMon->iDefIdx == pstCookie->iDefID)
	{
		pstCookie->aiIDs[(*piCount)] = pstMon->iID;
		++(*piCount);
	}
	return 0;
}

static int clan_city_remove_barrier(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                    int iDefID)
{
	int i, iRet, aiIDs[MAX_AREA_ANIMATE];
	DestroyCookie stCookie = {
		.aiIDs = aiIDs,
		.iMax = CCH(aiIDs),
		.iDefID = iDefID,
	};
	const ZONEMAPINST *pstMapInst = clan_city_map_inst(pstEnv, pstZoneClan);
	if(!pstMapInst)
	{
		return -1;
	}

	iRet = map_entities_count_if(pstEnv, pstMapInst, &stCookie,
	                             WillDestroyDefID);
	if(iRet < 0)
	{
		return -1;
	}

	for(i = 0; i < iRet; ++i)
	{
		Monster *pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, aiIDs[i]);
		if(!pstMon)
		{
			continue;
		}
		z_mon_destroy(pstEnv, pstMon);
	}

	return 0;
}

// 移除建筑的障碍物
int clan_building_remove_barrier(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                 int iIdx, int iLevel)
{
	const CLANCITYBUILDINGLEVELDEF *pstDef = 
		z_clan_city_building_level_def(pstEnv, iLevel, iIdx);
	if (!pstDef)
	{
		return -1;
	}
	return !pstDef->BarrierDefID ? 0 :
		clan_city_remove_barrier(pstEnv, pstZoneClan, pstDef->BarrierDefID);
}

int map_entities_count_if(ZONESVRENV *pstEnv, const ZONEMAPINST *pstMapInst,
                          void *pvCookie, CountPred pfnPred)
{
	int i = 0, iCount = 0;

	for(i = 0; i < pstMapInst->iAreaDyn; ++i)
	{
		const AREADYN *pstAreaDyn = NULL;
		int j = 0;
		int iDyn = pstMapInst->aiAreaDyn[i];
		if(iDyn < 0)
		{
			continue;
		}

		pstAreaDyn = tmempool_get(pstEnv->pstAreaDynPool, iDyn);
		if (!pstAreaDyn)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		for(j = 0; j < pstAreaDyn->iAnimate; ++j)
		{
			if(j >= MAX_AREA_ANIMATE)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d",
				           pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
				return -1;
			}

			if(pfnPred(pstEnv, pstAreaDyn->aiAnimate[j], pvCookie, &iCount) < 0)
			{
				return -1;
			}
		}
	}
	return iCount;
}

int clan_effect_broadcast(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iID,
                          const char *pszEffect)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	extern int add_str_replace(char *, int, char *,int);

	if (!pszEffect || !pszEffect[0])
	{
		return 0;
	}

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	pstActionRes->ID = iID;
	pstActionRes->ActionID = ACTION_EFFECT;

	add_str_replace(pstActionRes->Data.Effect.EffectStr,
	                sizeof(pstActionRes->Data.Effect.EffectStr) - 1,
	                (char *)pszEffect,0);
	pstActionRes->Data.Effect.EffectStr[
		strlen(pstActionRes->Data.Effect.EffectStr) - 1] = 0;
	return player_clan_broadcast(pstEnv, pstZoneClan, &stPkg, 0);
}
