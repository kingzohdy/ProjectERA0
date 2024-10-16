/*
**  @file $RCSfile: zone_city.c,v $
**  general description of this module
**  $Id: zone_city.c,v 1.146 2013/12/17 07:31:54 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2013/12/17 07:31:54 $
**  @version $Revision: 1.146 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_city.h"
#include "zone_map.h"
#include "zone_npc.h"
#include "zone_clan_advance.h"
#include "zone_move.h"
#include "zone_itemlimit.h"
#include "zone_clan.h"
#include "zone_machine.h"
#include "zone_clt.h"
#include "zone_oplog.h"
#include "zone_package.h"
#include "zone_strongpoint.h"
#include "zone_mail.h"
#include "zone_status.h"
#include "zone_attr.h"
#include "zone_player.h"
#include "zone_err.h"
#include "zone_clanboon.h"

int g_fWorldCitySiegeIn = 0;

int world_city_control_build_check(ZONESVRENV * pstEnv);

void city_build_info_born_set(CITYBUILDINFO *pstBuildInfo, int iIdx, int iLevel);
	
WORLDCITYBUILDDEF * z_find_world_city_build_def(ZONESVRENV *pstEnv, int iID)
{
	return (WORLDCITYBUILDDEF *)z_find_def(pstEnv->pstZoneObj->sWorldCityBuildDef, iID, WORLD_CITY_BUILD_DEF_DATA, 
									pstEnv->pstZoneObj->iWorldCityBuildDef , NULL);
}

CITYPRODUCTDEF * z_find_city_product_def(ZONESVRENV *pstEnv, int iID)
{
	return (CITYPRODUCTDEF *)z_find_def(pstEnv->pstZoneObj->sCityProductDef, iID, CITY_PRODUCT_DEF_DATA, 
									pstEnv->pstZoneObj->iCityProductDef, NULL);
}

WORLDCITYFIGHTAWARDDEF* z_find_world_city_fight_award_def(ZONESVRENV *pstEnv, int iID)
{
	return (WORLDCITYFIGHTAWARDDEF *)z_find_def(pstEnv->pstZoneObj->sWorldCityFightAwardDef, iID, WORLD_CITY_FIGHT_AWARD_DEF_DATA, 
									pstEnv->pstZoneObj->iWorldCityFightAwardDef, NULL);
}

//是否开启了攻城战功能
int is_open_fight_world_city(ZONESVRENV *pstEnv)
{
	if(!is_fini_global( pstEnv))
	{
		return 0;
	}

	if(pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity.CityCore.FightEnableStat == WORLD_CITY_OPEN)
	{
		return 1;
	}

	return 0;
}

//是否在攻城战时间内
int is_in_siege_city(ZONESVRENV *pstEnv)
{
	struct tm stNow;
	char sTmp[20];
	int iCurrDate;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;

	//GM控制开启城战
	if(g_fWorldCitySiegeIn && is_fini_global( pstEnv))
	{
		return 1;
	}
	
	if(!is_open_fight_world_city(pstEnv))
	{
		return 0;
	}

	if(pstWorldCity->CityCore.SiegeEndTime > 0 &&
		IsSameDay(pstWorldCity->CityCore.SiegeEndTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		return 0;
	}

	localtime_r(&pstEnv->pstAppCtx->stCurr.tv_sec, &stNow);

	//周六
	if(stNow.tm_wday != 6)
	{
		return 0;
	}

	//时间格式 xx小时xx分xx秒
	strftime(sTmp, sizeof(sTmp), "%H%M%S", &stNow);
	iCurrDate = atoi(sTmp);

	if(iCurrDate >= 200000 && 
		iCurrDate <= 213000)
	{
		return 1;
	}
	
	return 0;
}

//快速检查是否在攻城期间，只看状态不看时间
int is_in_siege_city_fast(ZONESVRENV *pstEnv)
{
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	
	if(pstWorldCity->CityCore.Stat == WORLD_CITY_STAT_FIGHT)
	{
		return 1;
	}

	return 0;
}

//是否在清场时间内
int is_in_clear_time(ZONESVRENV *pstEnv)
{
	struct tm stNow;
	char sTmp[20];
	int iCurrDate;

	//GM控制开启城战
	if(g_fWorldCitySiegeIn && is_fini_global( pstEnv))
	{
		return 0;
	}
	
	if(!is_open_fight_world_city(pstEnv))
	{
		return 0;
	}

	localtime_r(&pstEnv->pstAppCtx->stCurr.tv_sec, &stNow);

	//周六
	if(stNow.tm_wday != 6)
	{
		return 0;
	}

	//时间格式 xx小时xx分xx秒
	strftime(sTmp, sizeof(sTmp), "%H%M%S", &stNow);
	iCurrDate = atoi(sTmp);

	if(iCurrDate >= 190000 && 
		iCurrDate <= 200000)
	{
		return 1;
	}	
	
	return 0;
}

//城池建筑怪
int is_city_build_mon(Monster *pstMon)
{
	if(pstMon->iCityBuildDefID > 0)
	{
		return 1;
	}

	return 0;
}

//城池是否有名花有主了(被占领)
int is_has_master_world_city(ZONESVRENV *pstEnv)
{
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;

	if(pstWorldCity->CityCore.ClanGID > 0)
	{
		return 1;
	}

	return 0;
}

//是否是城池所有的公会
int is_world_city_clan(ZONESVRENV *pstEnv, ZoneClan *pstClan)
{
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;

	if(pstWorldCity->CityCore.ClanGID > 0 &&
		pstWorldCity->CityCore.ClanGID == pstClan->stClan.ClanProf.GID)
	{
		return 1;
	}

	return 0;
}

//是否是城池所有的公会人员
int is_world_city_owner(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;

	if(pstWorldCity->CityCore.ClanGID > 0 &&
		pstWorldCity->CityCore.ClanGID == pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID)
	{
		return 1;
	}

	return 0;
}

//是否是城主
int is_world_city_master(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	unsigned int uiClanPostion = player_clan_position(pstEnv, pstPlayer);

	if(!is_world_city_owner(pstEnv, pstPlayer))
	{
		return 0;
	}
	
	if(uiClanPostion == CLAN_POS_PRESIDENT)
	{
		return 1;
	}

	return 0;
}

//是不是同一周
 int is_same_week(time_t t1, time_t t2)
{
	int iNowWeekDay;
	int iStartWeekDay;
	struct tm stNow;
	struct tm stStart;
	time_t tNowTmp, tStartTmp;

	//凌晨6点切换天
	t1 -= 6*3600;
	t2 -= 6*3600;
	
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

int city_build_list_id_get(WORLDCITYBUILDDEF *pstBuildDef, int iIdx, int iLevel)
{
	if(iIdx >=0 && iIdx < MAX_CITY_BUILD_LIST &&
		iLevel >0 && iLevel <= MAX_CITY_BUILD_LEVEL)
	{
		return pstBuildDef->BuildList[iIdx].BuildID[iLevel-1];
	}

	return 0;
}

//建筑实际的MON定义ID
int city_build_id_get(ZONESVRENV *pstEnv, CITYBUILDINFO *pstBuildInfo, WORLDCITYBUILDDEF *pstBuildDef)
{
	if(pstBuildInfo->State == CITY_BUILD_STAT_ALIVE)
	{
		//WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;

		/*
		//门平常要开着。特殊处理
		if(pstBuildInfo->Type == CITY_BUILD_TYPE_DOOR_BUILD && 
			pstWorldCity->CityCore.Stat == WORLD_CITY_STAT_NORMAL)
		{
			return pstBuildDef->NormalDoorID[pstBuildInfo->BuildLevel-1];
		}
		*/
		
		if(pstBuildInfo->BuildIdx >=0 && pstBuildInfo->BuildIdx < MAX_CITY_BUILD_LIST &&
			pstBuildInfo->BuildLevel >0 && pstBuildInfo->BuildLevel <= MAX_CITY_BUILD_LEVEL)
		{
			return pstBuildDef->BuildList[pstBuildInfo->BuildIdx].BuildID[pstBuildInfo->BuildLevel-1];
		}
	}
	else if(pstBuildInfo->State == CITY_BUILD_STAT_DIE)
	{
		return pstBuildDef->DieBuildID;
	}
	UNUSED(pstEnv);
	return 0;
}

//清理掉这个建筑实例
int city_build_mon_clear(ZONESVRENV *pstEnv, CITYBUILDINFO *pstBuildInfo)
{
	Monster *pstMon;

	pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstBuildInfo->MemID);
	if(pstMon)
	{
		//如果是机械需要清理人员
		if(is_mon_machine(pstMon))
		{
			z_ride_machine_clear(pstEnv, pstMon);
		}
		
		z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
		z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);
	}

	pstBuildInfo->MemID = 0;
	return 0;
}

//获得建筑的状态
int city_build_state_get(WORLDCITYBUILD *pstWorldCityBuild, int iType)
{
	int i;
	
	for(i=0; i<pstWorldCityBuild->BuildNum; i++)
	{
		if(pstWorldCityBuild->BuildList[i].Type == iType)
		{
			return pstWorldCityBuild->BuildList[i].State;
		}
	}

	return CITY_BUILD_STAT_DIE;
}

int city_build_clear(ZONESVRENV *pstEnv, int iType)
{
	int i;
	WORLDCITYBUILD *pstWorldCityBuild = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity.CityBuild;

	for(i=0; i<pstWorldCityBuild->BuildNum; i++)
	{
		if(pstWorldCityBuild->BuildList[i].Type == iType)
		{
			city_build_mon_clear(pstEnv, &pstWorldCityBuild->BuildList[i]);
		}
	}
	
	return 0;
}

int city_build_control_num_get_byclan(ZONESVRENV *pstEnv, tdr_ulonglong ullGid)
{
	int i;
	int iCount = 0;
	Monster *pstMon;
	WORLDCITYBUILD *pstWorldCityBuild = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity.CityBuild;

	for(i=0; i<pstWorldCityBuild->BuildNum; i++)
	{
		if(pstWorldCityBuild->BuildList[i].Type != CITY_BUILD_TYPE_CONTROL)
		{
			continue;
		}

		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstWorldCityBuild->BuildList[i].MemID);
		if(NULL == pstMon)
		{
			continue;
		}

		if(pstMon->iPKType == MON_PK_TYPE_CLAN && 
			pstMon->ullPKID == ullGid)
		{
			iCount++;
		}
	}

	return iCount;
}

int city_build_buff_num_get_byclan(ZONESVRENV *pstEnv, tdr_ulonglong ullGid)
{
	int i;
	int iCount = 0;
	Monster *pstMon;
	WORLDCITYBUILD *pstWorldCityBuild = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity.CityBuild;

	for(i=0; i<pstWorldCityBuild->BuildNum; i++)
	{
		if(pstWorldCityBuild->BuildList[i].Type != CITY_BUILD_TYPE_BUFF ||
			pstWorldCityBuild->BuildList[i].State != CITY_BUILD_STAT_ALIVE)
		{
			continue;
		}

		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstWorldCityBuild->BuildList[i].MemID);
		if(NULL == pstMon)
		{
			continue;
		}

		if(pstMon->iPKType == MON_PK_TYPE_CLAN && 
			pstMon->ullPKID == ullGid)
		{
			iCount++;
		}
	}

	return iCount;
}

int city_build_refresh_clan_check(ZONESVRENV *pstEnv, CITYBUILDINFO *pstBuildInfo)
{
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	Monster *pstMon;
	
	if(pstBuildInfo->MemID == 0)
	{
		return 0;
	}
	
	pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstBuildInfo->MemID);
	if(NULL == pstMon)
	{
		pstBuildInfo->MemID = 0;
		return 0;
	}

	//易主了需要重新刷新
	if(pstMon->nOwnerFastID != (int)pstWorldCity->CityCore.ClanId)
	{
		city_build_mon_clear(pstEnv, pstBuildInfo);
	}
		
	return 0;
}

//攻城刷新建筑检查
int city_build_fight_refresh_check(ZONESVRENV *pstEnv, CITYBUILDINFO *pstBuildInfo)
{
	WORLDCITYBUILD *pstWorldCityBuild = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity.CityBuild;
	
	switch(pstBuildInfo->Type)
	{
		//阻挡只有在城门没打烂的时候刷新出来
		case CITY_BUILD_TYPE_DOOR_MASK:
			//强制刷新
			city_build_info_born_set(pstBuildInfo, 0, 1);
			
			if(CITY_BUILD_STAT_DIE == city_build_state_get(pstWorldCityBuild, CITY_BUILD_TYPE_DOOR_BUILD))
			{
				if(pstBuildInfo->MemID > 0)
				{
					city_build_mon_clear(pstEnv, pstBuildInfo);
				}
				return -1;
			}
			break;
		//阻挡只有在城门没打烂的时候刷新出来
		case CITY_BUILD_TYPE_DOOR2_MASK:
			//强制刷新
			city_build_info_born_set(pstBuildInfo, 0, 1);
			
			if(CITY_BUILD_STAT_DIE == city_build_state_get(pstWorldCityBuild, CITY_BUILD_TYPE_DOOR2))
			{
				if(pstBuildInfo->MemID > 0)
				{
					city_build_mon_clear(pstEnv, pstBuildInfo);
				}
				return -1;
			}
			break;
		//阻挡只有在城门没打烂的时候刷新出来
		case CITY_BUILD_TYPE_DOOR3_MASK:
			//强制刷新
			city_build_info_born_set(pstBuildInfo, 0, 1);
			
			if(CITY_BUILD_STAT_DIE == city_build_state_get(pstWorldCityBuild, CITY_BUILD_TYPE_DOOR3))
			{
				if(pstBuildInfo->MemID > 0)
				{
					city_build_mon_clear(pstEnv, pstBuildInfo);
				}
				return -1;
			}
			break;
		case CITY_BUILD_TYPE_CONTROL:
			if(pstBuildInfo->MemID > 0)
			{
				city_build_mon_clear(pstEnv, pstBuildInfo);
			}
			break;
			
		/*
		case CITY_BUILD_TYPE_DOOR_BUILD:
			//状态切换要更新下开关怪物
			if(pstBuildInfo->MemID > 0)
			{
				city_build_mon_clear(pstEnv, pstBuildInfo);
			}
			break;
		*/
		default:
			break;
	}
	
	return 0;
}

//正常刷出建筑检查
int city_build_normal_refresh_check(ZONESVRENV *pstEnv, CITYBUILDINFO *pstBuildInfo)
{	
	WORLDCITYBUILDDEF *pstBuildDef;
	//易主检查
	city_build_refresh_clan_check(pstEnv, pstBuildInfo);
	
	switch(pstBuildInfo->Type)
	{
		//城门阻挡平常不需要刷新出来
		case CITY_BUILD_TYPE_DOOR_MASK:
		case CITY_BUILD_TYPE_DOOR2_MASK:
		case CITY_BUILD_TYPE_DOOR3_MASK:
			if(pstBuildInfo->MemID > 0)
			{
				city_build_mon_clear(pstEnv, pstBuildInfo);
			}
			return -1;
			break;
		case CITY_BUILD_TYPE_CONTROL:
			if(pstBuildInfo->MemID > 0)
			{
				city_build_mon_clear(pstEnv, pstBuildInfo);
			}

			//非战争柱子都应该是满血复活的
			city_build_info_born_set(pstBuildInfo, 0, 1);
			break;
		/*
		case CITY_BUILD_TYPE_DOOR_BUILD:
			//状态切换要更新下开关怪物
			if(pstBuildInfo->MemID > 0)
			{
				city_build_mon_clear(pstEnv, pstBuildInfo);
			}
			break;
		*/
		case CITY_BUILD_TYPE_REVIVE:
			if(pstBuildInfo->MemID > 0)
			{
				city_build_mon_clear(pstEnv, pstBuildInfo);
			}
			break;
		case CITY_BUILD_TYPE_DOOR_BUILD:
		case CITY_BUILD_TYPE_DOOR2:
		case CITY_BUILD_TYPE_DOOR3:
		case CITY_BUILD_TYPE_DEF:
		case CITY_BUILD_TYPE_BUFF:
			if(pstBuildInfo->MemID > 0)
			{
				city_build_mon_clear(pstEnv, pstBuildInfo);
			}

			pstBuildDef = z_find_world_city_build_def(pstEnv, pstBuildInfo->DefID);
			if(pstBuildDef && pstBuildInfo->State == CITY_BUILD_STAT_DIE)
			{
				city_build_info_born_set(pstBuildInfo, pstBuildDef->DefaultIdx-1, 1);
			}
			
			break;
		default:

			
			break;
	}
	
	return 0;
}

int city_build_tmpclan_info_inset(TmpClanBuild *pastClanBuild, int *piNum, ZoneClan *pstClan)
{
	int i;

	for(i=0; i<*piNum; i++)
	{
		if(pastClanBuild[i].pstClan == pstClan)
		{
			pastClanBuild[i].iNum++;
			return 0;
		}
	}

	if(*piNum < 3)
	{
		pastClanBuild[*piNum].pstClan = pstClan;
		pastClanBuild[*piNum].iNum = 1;
		*piNum += 1;
	}

	return 0;
}

//获得占领控制中心最多的公会信息
ZoneClan* city_build_control_info_get(ZONESVRENV *pstEnv, int *piNum)
{
	int i, iNum, iCount;
	CITYBUILDINFO *pstBuildInfo;
	Monster *pstMon;
	ZoneClan *pstClan = NULL;
	TmpClanBuild astClanBuild[MAX_WORLD_CITY_CONTROL];
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	WORLDCITYBUILD *pstWorldCityBuild = &pstWorldCity->CityBuild;

	iCount = 0;
	iNum = 0;
	*piNum = 0;
	for(i=0; i<pstWorldCityBuild->BuildNum; i++)
	{
		pstBuildInfo = &pstWorldCityBuild->BuildList[i];

		if(pstBuildInfo->Type != CITY_BUILD_TYPE_CONTROL)
		{
			continue;
		}

		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstBuildInfo->MemID);
		if(NULL == pstMon)
		{
			continue;
		}

		pstClan = z_id_clan(pstEnv, pstMon->nOwnerFastID);
		if(NULL == pstClan)
		{
			continue;
		}

		city_build_tmpclan_info_inset(astClanBuild, &iNum, pstClan);
	}

	pstClan = NULL;
	for(i=0; i<iNum; i++)
	{
		if(astClanBuild[i].iNum > iCount)
		{
			pstClan = astClanBuild[i].pstClan;
			*piNum = iCount = astClanBuild[i].iNum;
		}
	}

	return pstClan;
}

void city_build_info_die_set(CITYBUILDINFO *pstBuildInfo)
{
	pstBuildInfo->State = CITY_BUILD_STAT_DIE;
	pstBuildInfo->BuildIdx = 0;
	pstBuildInfo->BuildLevel = 0;
	pstBuildInfo->MemID = 0;
	pstBuildInfo->HP = 10000;
	pstBuildInfo->Energy = 10000;
	pstBuildInfo->WarningEndTime = 0;
		
	return ;
}

void city_build_info_born_set(CITYBUILDINFO *pstBuildInfo, int iIdx, int iLevel)
{
	pstBuildInfo->State = CITY_BUILD_STAT_ALIVE;
	pstBuildInfo->BuildIdx = iIdx;
	pstBuildInfo->BuildLevel = iLevel;
	pstBuildInfo->MemID = 0;
	pstBuildInfo->HP = 10000;
	pstBuildInfo->Energy = 10000;
	pstBuildInfo->WarningEndTime = 0;
	
	return ;
}

//new一个普通公会建筑
Monster *new_city_build_normal_mon(ZONESVRENV *pstEnv, ZONEMAPINST *pstMapInst, 
		int iMonID, int iDir, RESPOS *pstPos, ZoneClan *pstClan, int iBuildDefID)
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
	//表示是建筑怪
	stMonster.iCityBuildDefID = iBuildDefID;
	if(pstClan)
	{
		mon_set_pkinfo(&stMonster, MON_PK_TYPE_CLAN, pstClan->stClan.ClanProf.GID);
		stMonster.nOwnerFastID = pstClan->stClan.ClanProf.Id;
	}
	else
	{
		mon_set_pkinfo(&stMonster, MON_PK_TYPE_CLAN, 0);
	}

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

//刷新出一个建筑
Monster *new_city_build_mon(ZONESVRENV *pstEnv, WORLDCITYBUILDDEF *pstBuildDef, CITYBUILDINFO *pstBuildInfo, ZoneClan *pstClan)
{
	int iMonDefID, iHP;
	Monster *pstMon = NULL, stMonster;
	MONSTERDEF *pstMonDef;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;

	if(pstBuildInfo->MemID > 0)
	{
		return NULL;
	}

	iMonDefID = city_build_id_get(pstEnv, pstBuildInfo, pstBuildDef);
	if(iMonDefID <= 0)
	{
		return NULL;
	}
	
	stIdx.iID = WORLD_CITY_MAPID;
	stIdx.iPos = -1;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return NULL;
	}

	stMonster.iDefIdx = iMonDefID;
	stMonster.iDefPos = -1;
	pstMonDef = z_get_mon_def(pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		return NULL;
	}

	if(pstMonDef->SubType == MON_SUB_TYPE_MACHINE)
	{
		MONMACHINEDEF *pstMachineDef;

		if(NULL == pstClan)
		{
			return NULL;
		}
		
		pstMon = new_mon_machine_dir(pstEnv, pstMapInst, iMonDefID, &pstBuildDef->Pos, 
			MACHINE_OWNER_CLAN, pstClan, pstBuildDef->Dir, 0, pstBuildDef->ID, 0);
		if(NULL == pstMon)
		{
			return NULL;
		}

		pstMachineDef = z_find_mon_machine_def(pstEnv, pstMon->iDefIdx);
		if(NULL == pstMachineDef)
		{
			return NULL;
		}
		
		z_machine_attr_chg(pstEnv, pstMon , 
			MON_MACHINE_ATTR_ENERGY, (tdr_ulonglong)pstBuildInfo->Energy*pstMachineDef->MaxEnergy/10000);
	}
	else
	{
		//if(pstBuildInfo->Type == CITY_BUILD_TYPE_REVIVE)
		//{
		//	pstMon = new_city_build_normal_mon(pstEnv, pstMapInst, 
		//		iMonDefID, pstBuildDef->Dir,&pstBuildDef->Pos, NULL, pstBuildInfo->DefID);
		//}

		//else
		{
			pstMon = new_city_build_normal_mon(pstEnv, pstMapInst, 
				iMonDefID, pstBuildDef->Dir,&pstBuildDef->Pos, pstClan, pstBuildInfo->DefID);
		}
	}

	if(NULL == pstMon)
	{
		return NULL;
	}

	iHP = (tdr_ulonglong )pstBuildInfo->HP*pstMonDef->HP/10000;
	if(pstMon->iHP != iHP)
	{
		pstMon->iHP = iHP;
		mon_hp_action(pstEnv, pstMon, pstMon->iHP, 0);
	}

	pstBuildInfo->MemID = pstMon->iID;
	return pstMon;
}

//城市当前坚持防守的次数0 (刚占领/ 未被占领也是0)，1 防守住1次。。。。
int world_city_fight_keep_num(ZONESVRENV *pstEnv)
{
	if(!is_fini_global(pstEnv))
	{
		return 0;
	}

	return pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity.CityCore.ClanKeepNum;
}

//城市当前应该刷新的类型(攻城、普通)
int world_city_refresh_stat_get(ZONESVRENV *pstEnv)
{
	if(is_in_siege_city(pstEnv))
	{
		return WORLD_CITY_STAT_FIGHT;
	}
	else if(is_in_clear_time(pstEnv))
	{
		return WORLD_CITY_STAT_CLEAR;
	}
	else
	{
		return WORLD_CITY_STAT_NORMAL;
	}

	return 0;
}

int world_city_capture_action(ZONESVRENV *pstEnv, ZoneClan *pstClan)
{
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	CSPKG stPkg;
	CSCITYSVR* pstCitySvr = &stPkg.Body.CitySvr;
	CSCITYBUILDCAPTURE *pstCSCapture;
	
	stIdx.iID = WORLD_CITY_MAPID;
	stIdx.iPos = -1;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstCSCapture = &pstCitySvr->Data.Capture;
	pstCitySvr->Type = CITY_SVR_BUILD_CAPTURE;
	memset(pstCSCapture, 0, sizeof(*pstCSCapture));
	
	if(pstClan)
	{
		pstCSCapture->CaptureEndTime = pstWorldCity->CityCore.CaptureEndTime;
		STRNCPY(pstCSCapture->ClanName, pstClan->stClan.ClanProf.Name, sizeof(pstCSCapture->ClanName));
	}

	Z_CSHEAD_INIT(&stPkg.Head, CITY_SVR);
	return map_broadcast(pstEnv->pstAppCtx, pstEnv, pstMapInst, &stPkg);
}

//城市建筑刷新
int world_city_build_refresh(ZONESVRENV *pstEnv)
{
	int i;
	ZoneClan *pstClan;
	Monster *pstMon;
	CITYBUILDINFO *pstBuildInfo;
	WORLDCITYBUILDDEF *pstBuildDef;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	WORLDCITYBUILD *pstWorldCityBuild = &pstWorldCity->CityBuild;

	if(pstWorldCity->CityCore.Stat == WORLD_CITY_STAT_FIGHT)
	{
		pstWorldCity->CityCore.CaptureEndTime = 0;
	}
	
	pstClan = z_id_clan(pstEnv, pstWorldCity->CityCore.ClanId);

	for(i=0; i<pstWorldCityBuild->BuildNum; i++)
	{
		pstBuildInfo = &pstWorldCityBuild->BuildList[i];
		pstBuildDef = z_find_world_city_build_def(pstEnv, pstBuildInfo->DefID);
		if(NULL == pstBuildDef)
		{
			continue;
		}
	
		if(pstWorldCity->CityCore.Stat == WORLD_CITY_STAT_FIGHT)
		{
			if(0 > city_build_fight_refresh_check(pstEnv, pstBuildInfo))
			{
				continue;
			}
		}
		else
		{
			if(0 > city_build_normal_refresh_check(pstEnv, pstBuildInfo))
			{
				continue;
			}
		}

		/*
		if(pstBuildInfo->Type == CITY_BUILD_TYPE_REVIVE)
		{
			new_city_build_mon(pstEnv, pstBuildDef, pstBuildInfo, NULL);
		}
		else
		*/
		//{
			pstMon = new_city_build_mon(pstEnv, pstBuildDef, pstBuildInfo, pstClan);
			if(pstMon && pstBuildInfo->Type == CITY_BUILD_TYPE_CONTROL)
			{
				if(is_in_siege_city_fast(pstEnv))
				{	
					z_mon_apply_status2(pstEnv->pstAppCtx, pstEnv, pstMon, 3192, 1, 86400, 1, 0);
				}
			}
		//}
	}

	map_world_city_info_broadcast(pstEnv);

	return 0;
}

//城市创建
int world_city_refresh(ZONESVRENV *pstEnv)
{
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	
	if(!is_fini_global( pstEnv))
	{
		return 0;
	}

	pstWorldCity->CityCore.Stat = world_city_refresh_stat_get(pstEnv);
	world_city_build_refresh(pstEnv);
	
	return 0;
}

//获得建筑的global信息
CITYBUILDINFO*  world_city_build_info_get(ZONESVRENV* pstEnv, int iID)
{
	int i;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	WORLDCITYBUILD *pstWorldCityBuild = &pstWorldCity->CityBuild;
	CITYBUILDINFO *pstBuildInfo;

	if(!is_fini_global(pstEnv))
	{
		return NULL;
	}

	for(i=0; i<pstWorldCityBuild->BuildNum; i++)
	{
		pstBuildInfo = &pstWorldCityBuild->BuildList[i];
		if(pstBuildInfo ->DefID == iID)
		{
			return pstBuildInfo;
		}
	}

	return NULL;
}

//当前存在的能源增幅器数量
int world_city_buff_build_alive_num_get(ZONESVRENV* pstEnv)
{
	int i;
	int iCount = 0;
	Monster *pstMon;
	WORLDCITYBUILD *pstWorldCityBuild = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity.CityBuild;

	for(i=0; i<pstWorldCityBuild->BuildNum; i++)
	{
		if(pstWorldCityBuild->BuildList[i].Type != CITY_BUILD_TYPE_BUFF ||
			pstWorldCityBuild->BuildList[i].State != CITY_BUILD_STAT_ALIVE)
		{
			continue;
		}

		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstWorldCityBuild->BuildList[i].MemID);
		if(NULL == pstMon)
		{
			continue;
		}

		iCount++;
	}

	return iCount;
}

//插入一个global建筑
int world_city_build_info_insert(ZONESVRENV* pstEnv, WORLDCITYBUILDDEF *pstBuildDef)
{
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	WORLDCITYBUILD *pstWorldCityBuild = &pstWorldCity->CityBuild;
	CITYBUILDINFO *pstBuildInfo;
	
	if(!is_fini_global(pstEnv))
	{
		return -1;
	}

	if(world_city_build_info_get(pstEnv, pstBuildDef->ID))
	{
		return -1;
	}

	if(pstWorldCityBuild->BuildNum >= MAX_WORLD_CITY_BUILD)
	{
		return -1;
	}

	pstBuildInfo = &pstWorldCityBuild->BuildList[pstWorldCityBuild->BuildNum++];

	memset(pstBuildInfo, 0, sizeof(*pstBuildInfo));
	pstBuildInfo->DefID = pstBuildDef->ID;
	pstBuildInfo->Type = pstBuildDef->Type;
	
	if(pstBuildDef->DefaultIdx > 0)
	{
		city_build_info_born_set(pstBuildInfo, pstBuildDef->DefaultIdx-1, 1);
	}
	else
	{
		city_build_info_die_set(pstBuildInfo);
	}
	
	return 0;
}

//建筑更新
int world_city_build_info_update(ZONESVRENV* pstEnv, WORLDCITYBUILDDEF *pstBuildDef)
{
	CITYBUILDINFO *pstBuildInfo;

	if(!is_fini_global(pstEnv))
	{
		return -1;
	}

	pstBuildInfo = world_city_build_info_get(pstEnv, pstBuildDef->ID);
	//没有就插入
	if(NULL == pstBuildInfo)
	{
		return world_city_build_info_insert(pstEnv, pstBuildDef);
	}

	pstBuildInfo->Type = pstBuildDef->Type;
	
	return 0;
}

//建筑死亡
int world_city_build_die(ZONESVRENV *pstEnv, Monster *pstMon, Player *pstKiller)
{
	CITYBUILDINFO *pstBuildInfo;
	WORLDCITYBUILDDEF *pstBuildDef;
	ZONEMAPINST * pstMapInst;
	MONSTERDEF *pstMonDef;

	if(!is_city_build_mon(pstMon))
	{
		return -1;
	}

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if(NULL == pstMonDef)
	{
		return -1;
	}
	
	pstBuildInfo = world_city_build_info_get(pstEnv, pstMon->iCityBuildDefID);
	if(NULL == pstBuildInfo)
	{
		return -1;
	}

	pstBuildDef = z_find_world_city_build_def(pstEnv, pstBuildInfo->DefID);
	if(NULL == pstBuildDef)
	{
		return -1;
	}

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	switch(pstBuildInfo->State)
	{
		//建筑->摧毁
		case CITY_BUILD_STAT_ALIVE:
			{
				if(pstBuildInfo->Type == CITY_BUILD_TYPE_REVIVE)
				{
					ZoneClan *pstClan = NULL;

					if(pstKiller)
					{
						pstClan = player_get_clan(pstEnv, pstKiller);
					}
						
					city_build_info_born_set(pstBuildInfo, 0, 1);
					if(pstClan && is_in_siege_city(pstEnv))
					{
						new_city_build_mon(pstEnv, pstBuildDef, pstBuildInfo, pstClan);
						z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE, 
							ZONEERR_BROADMAP301, pstClan->stClan.ClanProf.Name, pstMonDef->Name);
					}
					else
					{
						new_city_build_mon(pstEnv, pstBuildDef, pstBuildInfo, NULL);
					}
				}
				else
				{
					//可能没有公会。比如开始夺城池的时候
					ZoneClan *pstClan = z_id_clan(pstEnv, pstMon->nOwnerFastID);
					
					city_build_info_die_set(pstBuildInfo);
					new_city_build_mon(pstEnv, pstBuildDef, pstBuildInfo, pstClan);
					
					if(pstKiller)
					{
						if(pstBuildInfo->Type == CITY_BUILD_TYPE_CONTROL)
						{
							z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE, 
								ZONEERR_BROADMAP302, 
								pstKiller->stRoleData.RoleName, pstMonDef->Name);
						}
						else
						{
							z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE, 
								ZONEERR_BROADMAP303, 
								pstKiller->stRoleData.RoleName, pstMonDef->Name);
						}
						
					}
				}
				
				if(pstBuildInfo->Type == CITY_BUILD_TYPE_DOOR_BUILD)
				{
					//开门
					city_build_clear(pstEnv, CITY_BUILD_TYPE_DOOR_MASK);
				}
				else if(pstBuildInfo->Type == CITY_BUILD_TYPE_DOOR2)
				{
					//开门
					city_build_clear(pstEnv, CITY_BUILD_TYPE_DOOR2_MASK);
				}
				else if(pstBuildInfo->Type == CITY_BUILD_TYPE_DOOR3)
				{
					//开门
					city_build_clear(pstEnv, CITY_BUILD_TYPE_DOOR3_MASK);
				}

				if(pstBuildInfo->Type != CITY_BUILD_TYPE_CONTROL)
				{
					if(pstKiller)
					{
						ZoneClan *pstClan = player_get_clan(pstEnv, pstKiller);
						if(pstClan)
						{
							world_city_fight_range_update(pstEnv, pstClan, pstBuildInfo->Type);
						}
					}

					world_city_owner_fight_info_update(pstEnv);

				}
			}
			break;
		//摧毁的建筑-> 再死亡
		case CITY_BUILD_STAT_DIE:
			//控制中心
			if(pstBuildInfo->Type == CITY_BUILD_TYPE_CONTROL)
			{
				ZoneClan *pstClan = NULL;
				Monster *pstNew;

				city_build_info_born_set(pstBuildInfo, 0, 1);

				

				if(pstKiller)
				{
					pstClan = player_get_clan(pstEnv, pstKiller);
				}
			
				if(pstClan && is_in_siege_city(pstEnv))
				{	
					pstNew = new_city_build_mon(pstEnv, pstBuildDef, pstBuildInfo, pstClan);
					z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE, 
						ZONEERR_BROADMAP304, pstClan->stClan.ClanProf.Name, pstMonDef->Name);

					world_city_fight_range_update(pstEnv, pstClan, pstBuildInfo->Type);
				}
				else
				{
					pstNew = new_city_build_mon(pstEnv, pstBuildDef, pstBuildInfo, NULL);
				}

				world_city_owner_fight_info_update(pstEnv);

				if(is_in_siege_city_fast(pstEnv) && pstNew)
				{	
					z_mon_apply_status2(pstEnv->pstAppCtx, pstEnv, pstNew, 3192, 1, 86400, 1, 0);
				}

				//这里要看看是否都占领了倒计时
				//world_city_control_build_check(pstEnv);
			}
			else
			{
				//error 除了控制中心，其他的建筑不可以死亡再次死亡的。
			}
			break;
		default:
			break;
	}

	map_world_city_info_broadcast(pstEnv);

	return 0;
}

//更新建筑实例信息
int world_city_build_mon_update(ZONESVRENV *pstEnv, Monster *pstMon)
{
	CITYBUILDINFO *pstBuildInfo;

	if(!is_city_build_mon(pstMon))
	{
		return -1;
	}
	
	pstBuildInfo = world_city_build_info_get(pstEnv, pstMon->iCityBuildDefID);
	if(NULL == pstBuildInfo)
	{
		return -1;
	}

	if(pstMon->cStat == MONSTER_ALIVE)
	{
		pstBuildInfo->HP = (tdr_ulonglong )pstMon->iHP*10000/pstMon->iMaxHP;

		if(is_mon_machine(pstMon))
		{
			if(this_machine(pstEnv, pstMon)->stMachineInfo.MaxEnergy > 0)
			{
				pstBuildInfo->Energy = (tdr_ulonglong )this_machine(pstEnv, pstMon)->stMachineInfo.Energy*10000/
					this_machine(pstEnv, pstMon)->stMachineInfo.MaxEnergy;
			}
		}
	}

	return 0;
}

//10天后激活可进入地图
int world_city_set_enabletime(ZONESVRENV *pstEnv)
{
	struct tm stNow;
	time_t tTime;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;

	if(!is_fini_global( pstEnv))
	{
		return 0;
	}

	if(pstWorldCity->CityCore.FightEnableStat != WORLD_CITY_UNOPEN ||
		pstWorldCity->CityCore.FightEnableTime > 0)
	{
		return 0;
	}
	
	tTime = pstEnv->pstAppCtx->stCurr.tv_sec + 10*86400;
	localtime_r(&tTime, &stNow);
	
	//10天后20:00激活
	stNow.tm_hour = 20;
	stNow.tm_min = 0;
	stNow.tm_sec = 0;
	pstWorldCity->CityCore.FightEnableTime = mktime(&stNow);

	return 0;
}

//下周开启城战
int world_city_set_nextweek_open(ZONESVRENV *pstEnv)
{
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;

	if(!is_fini_global( pstEnv))
	{
		return 0;
	}

	if(pstWorldCity->CityCore.FightEnableStat != WORLD_CITY_UNOPEN)
	{
		return 0;
	}
	
	pstWorldCity->CityCore.FightEnableStat = WORLD_CITY_NEXT_WEEK_OPEN;
	pstWorldCity->CityCore.FightEnableTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	z_sys_strf_broadcast(pstEnv, SYS_SVR, 	ZONEERR_BROAD303);

	world_city_open_ui_action(pstEnv);
	return 0;
}

//是否可以进入攻城地图
int world_city_enter_check(ZONESVRENV *pstEnv)
{
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;

	if(!is_fini_global( pstEnv))
	{
		return -1;
	}

	if(pstWorldCity->CityCore.FightEnableStat != WORLD_CITY_NEXT_WEEK_OPEN &&
		pstWorldCity->CityCore.FightEnableStat != WORLD_CITY_OPEN)
	{
		return -1;
	}

	return 0;
}

//攻城战开启检查
int world_city_enable_check(ZONESVRENV *pstEnv, WORLDCITY *pstWorldCity)
{
	if(pstWorldCity->CityCore.FightEnableStat == WORLD_CITY_UNOPEN)
	{
		if(pstWorldCity->CityCore.FightEnableTime > 0 &&
			pstWorldCity->CityCore.FightEnableTime < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			z_sys_strf_broadcast(pstEnv, SYS_SVR,	ZONEERR_BROAD303);
			//world_city_add_bulletin(pstEnv, 30, 10, 
			//"通往时间城中控中心区域的通道已经开启，时间城城主争霸战将于下周六20：00正式开启");
			pstWorldCity->CityCore.FightEnableStat = WORLD_CITY_NEXT_WEEK_OPEN;
			pstWorldCity->CityCore.FightEnableTime = pstEnv->pstAppCtx->stCurr.tv_sec;

			world_city_open_ui_action(pstEnv);
		}
	}
	else if(pstWorldCity->CityCore.FightEnableStat == WORLD_CITY_NEXT_WEEK_OPEN)
	{
		if(!is_same_week(pstWorldCity->CityCore.FightEnableTime, pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			world_city_open(pstEnv);
		}
	}

	return 0;
}

int world_city_control_build_all_capture(ZONESVRENV *pstEnv, ZoneClan * pstClan)
{
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	
	pstWorldCity->CityCore.CaptureEndTime = pstEnv->pstAppCtx->stCurr.tv_sec + WORLD_CITY_FAST_WIN_TIME;
	//pstWorldCity->CityCore.CaptureEndTime = pstEnv->pstAppCtx->stCurr.tv_sec + 30;
	world_city_capture_action(pstEnv, pstClan);
	
	return 0;
}

int world_city_control_build_check(ZONESVRENV *pstEnv)
{
	int iCount = 0;
	ZoneClan *pstSiegeClan;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	
	pstSiegeClan = city_build_control_info_get(pstEnv, &iCount);
	if(NULL == pstSiegeClan)
	{
		goto _error;
	}

	if(pstSiegeClan->stClan.ClanProf.GID != pstWorldCity->CityCore.ClanGID && 
		iCount == 3)
	{
		if(pstWorldCity->CityCore.CaptureEndTime == 0)
		{
			world_city_control_build_all_capture(pstEnv, pstSiegeClan);
		}
	}
	else
	{
		goto _error;
	}

	return 0;

_error:
	
	if(pstWorldCity->CityCore.CaptureEndTime > 0)
	{
		pstWorldCity->CityCore.CaptureEndTime = 0;
		world_city_capture_action(pstEnv, NULL);
	}

	return 0;
}

int world_city_info_broadcast(ZONESVRENV* pstEnv)
{
	CSPKG stPkg;
	CSCITYSVR*pstCitySvr = &stPkg.Body.CitySvr;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;

	pstCitySvr->Type = CITY_SVR_CORE_INFO;
	pstCitySvr->Data.CoreInfo.ClanGID = pstWorldCity->CityCore.ClanGID;
	pstCitySvr->Data.CoreInfo.ClanId = pstWorldCity->CityCore.ClanId;
	pstCitySvr->Data.CoreInfo.KeepNum = pstWorldCity->CityCore.ClanKeepNum;
	pstCitySvr->Data.CoreInfo.Effigy = pstWorldCity->CityCore.Effigy;
	pstCitySvr->Data.CoreInfo.Stat = pstWorldCity->CityCore.Stat;
	pstCitySvr->Data.CoreInfo.FightEnableStat = pstWorldCity->CityCore.FightEnableStat;
	
	Z_CSHEAD_INIT(&stPkg.Head, CITY_SVR);
	return z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
}

int world_city_fight_begin_broadcast(ZONESVRENV* pstEnv)
{
	CSPKG stPkg;
	CSCITYSVR*pstCitySvr = &stPkg.Body.CitySvr;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;

	pstCitySvr->Type = CITY_SVR_FIGHT;
	pstCitySvr->Data.Fight.Stat= pstWorldCity->CityCore.Stat;
	
	Z_CSHEAD_INIT(&stPkg.Head, CITY_SVR);
	return z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
}

ZoneClan * world_city_clan_range_win_get(ZONESVRENV *pstEnv)
{
	ZoneClan *pstClan;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;

	if(pstWorldCity->ClanRange.Num <= 0)
	{
		return NULL;
	}

	if(pstWorldCity->ClanRange.ClanList[0].Total <= 0)
	{
		return NULL;
	}

	pstClan = z_clanid_clan(pstEnv, &pstWorldCity->ClanRange.ClanList[0].ClanId);
	if(!pstClan)
	{
		return NULL;
	}
	
	return pstClan;
}

//攻城战结束
int world_city_siege_end(ZONESVRENV *pstEnv, int fEndTime)
{	
	//int iCount = 0;
	//int iWinCount = MAX_WORLD_CITY_CONTROL-1;
	ZoneClan *pstClan;
	ZoneClan *pstSiegeClan;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	OPWORLDCITYINFO stPreWorldCityInfo;

	world_city_fight_range_svr_info(pstEnv, NULL, 1);
	
	//log
	stPreWorldCityInfo.ClanGID = pstWorldCity->CityCore.ClanGID;
	stPreWorldCityInfo.ClanId = pstWorldCity->CityCore.ClanId;
	stPreWorldCityInfo.ClanKeepNum = pstWorldCity->CityCore.ClanKeepNum;
	STRNCPY(stPreWorldCityInfo.ClanName, pstWorldCity->CityCore.ClanName, CCH(stPreWorldCityInfo.ClanName));
	
	pstClan = z_id_clan(pstEnv, pstWorldCity->CityCore.ClanId);
	//pstSiegeClan = city_build_control_info_get(pstEnv, &iCount);

	pstSiegeClan = world_city_clan_range_win_get(pstEnv);

	if(pstSiegeClan && pstSiegeClan != pstClan)
	{
		//易主
		STRNCPY(pstWorldCity->CityCore.ClanName, 
			pstSiegeClan->stClan.ClanProf.Name, sizeof(pstWorldCity->CityCore.ClanName));
		
		pstWorldCity->CityCore.ClanGID = pstSiegeClan->stClan.ClanProf.GID;
		pstWorldCity->CityCore.ClanId = pstSiegeClan->stClan.ClanProf.Id;
		pstWorldCity->CityCore.ClanBeginTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstWorldCity->CityCore.ClanKeepNum = 0;
		memset(&pstWorldCity->CityCore.Effigy, 0, sizeof(pstWorldCity->CityCore.Effigy));

		clan_map_resource_time_update(pstEnv->pstAppCtx->stCurr.tv_sec, 
			&pstSiegeClan->stClan.ClanCore.NextAddCityResource);
		
		if(fEndTime)
		{
			z_sys_strf_broadcast(pstEnv, SYS_SVR, 
				ZONEERR_BROAD304,
				pstSiegeClan->stClan.ClanProf.Name);
		}
		else
		{
			z_sys_strf_broadcast(pstEnv, SYS_SVR, 
				ZONEERR_BROAD305,
				pstSiegeClan->stClan.ClanProf.Name);
		}

		
		clan_boon_strong_chg_update(pstEnv,4404, pstSiegeClan, 2); 
		if (pstClan)
			clan_boon_strong_chg_update(pstEnv,4404, pstClan, 1);
	}
	else
	{
		if(NULL == pstClan)
		{
			z_sys_strf_broadcast(pstEnv, SYS_SVR, 	ZONEERR_BROAD306);
			goto _next;
		}

		//防守次数
		pstWorldCity->CityCore.ClanKeepNum++;
		clan_boon_strong_chg_update(pstEnv,4404, pstClan, 0);
	
		z_sys_strf_broadcast(pstEnv, SYS_SVR, 
				ZONEERR_BROAD307,
				pstClan->stClan.ClanProf.Name);
	}

	world_city_info_broadcast(pstEnv);

	dyn_active_status_ch(pstEnv,WORLD_CITY_ACTIVE_HELP_ID,ACTIVE_STATUS_END);

_next:
	
	pstWorldCity->CityCore.CaptureEndTime = 0;
	pstWorldCity->CityCore.SiegeEndTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstWorldCity->CityCore.Stat = WORLD_CITY_STAT_NORMAL;
	world_city_build_refresh(pstEnv);
	pstEnv->iWorldCityBuildSerial = 0;

	z_world_city_siege_oplog(pstEnv, &stPreWorldCityInfo);
	
	return 0;
}

int world_city_fight_tick(ZONESVRENV *pstEnv, unsigned int uiSec)
{
	//WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	
	//结束
	if(!is_in_siege_city(pstEnv))
	{
		world_city_siege_end(pstEnv, 1);
	}
	//战斗中检查
	else
	{
		//int iCount = 0;
		//ZoneClan *pstSiegeClan;

		if(uiSec % 10 == 0)
		{
			world_city_fight_range_svr_info(pstEnv, NULL, 0);
		}
		
		//world_city_control_build_check(pstEnv);
		/*
		if(pstWorldCity->CityCore.CaptureEndTime > 0 &&
			pstWorldCity->CityCore.CaptureEndTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			pstSiegeClan = city_build_control_info_get(pstEnv, &iCount);
			if(NULL == pstSiegeClan)
			{
				return 0;
			}

			if(pstSiegeClan->stClan.ClanProf.GID != pstWorldCity->CityCore.ClanGID && iCount == 3)
			{
				world_city_siege_end(pstEnv, 0);
			}
		}
		*/
	}

	return 0;
}

int world_city_normal_tick(ZONESVRENV *pstEnv)
{
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;

	if(pstWorldCity->CityCore.Stat == WORLD_CITY_STAT_NORMAL ||
		pstWorldCity->CityCore.Stat == WORLD_CITY_STAT_CLEAR)
	{
		if(is_in_siege_city(pstEnv))
		{
			//关门开搞
			pstWorldCity->CityCore.Stat = WORLD_CITY_STAT_FIGHT;
			world_city_build_refresh(pstEnv);

			//音效
			world_city_fight_begin_broadcast(pstEnv);
			//world_city_add_bulletin(pstEnv, 30, 10, "时间城城主争霸战正式开启！");
			z_sys_strf_broadcast(pstEnv, SYS_SVR, 
				ZONEERR_BROAD308);

			pstEnv->iWorldCityBuildSerial++;

			dyn_active_status_ch(pstEnv,WORLD_CITY_ACTIVE_HELP_ID,ACTIVE_STATUS_RUNNING);

			world_city_fight_range_init(pstEnv);
		}
		//只是方便清场的状态同等NORMAL
		else if(is_in_clear_time(pstEnv))
		{
			pstWorldCity->CityCore.Stat = WORLD_CITY_STAT_CLEAR;
		}
		else
		{
			pstWorldCity->CityCore.Stat = WORLD_CITY_STAT_NORMAL;
		}
	}
	
	return 0;
}

// 2秒TICK,  mapinst_active
int world_city_tick(ZONESVRENV *pstEnv, ZONEMAPINST *pstMapInst)
{
	static unsigned int uiActiveSec = 0;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;

	if(WORLD_CITY_MAPID != GET_MAPID(pstMapInst->iMapInstID))
	{
		return -1;
	}
	
	if(!is_fini_global( pstEnv))
	{
		return 0;
	}

	if(pstWorldCity->CityCore.FightEnableStat != WORLD_CITY_OPEN && g_fWorldCitySiegeIn == 0)
	{
		return world_city_enable_check(pstEnv, pstWorldCity);
	}

	if(pstWorldCity->CityCore.Stat == WORLD_CITY_STAT_FIGHT)
	{
		world_city_fight_tick(pstEnv, uiActiveSec);
	}
	else
	{
		world_city_normal_tick(pstEnv);
	}

	uiActiveSec += 2;
	return 0;
}

//设置城主雕像
int world_city_effigy_set(ZONESVRENV *pstEnv, Player *pstPlayer, int iMap, RESPOS *pstPos)
{
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	WORLDCITYEFFIGY *pstEffigy = &pstWorldCity->CityCore.Effigy;
	
	if(!is_fini_global( pstEnv))
	{
		return 0;
	}

	if(strcmp(pstEffigy->RoleName, pstPlayer->stRoleData.RoleName) != 0)
	{
		pstEffigy->Flower = 0;
		pstEffigy->Egg = 0;
		pstEffigy->StartTime = 0;
		pstEffigy->CurrStat = 0;
		pstEffigy->YesterdayFlower = 0;
		pstEffigy->YesterdayEgg= 0;
		pstEffigy->AwardTime = 0;
	}
	
	pstEffigy->RoleLevel = pstPlayer->stRoleData.Level;
	pstEffigy->RoleGender = pstPlayer->stRoleData.Gender;
	pstEffigy->RoleCareer = pstPlayer->stRoleData.Career;
	pstEffigy->RoleHair = pstPlayer->stRoleData.Hair;
	pstEffigy->RoleHead = pstPlayer->stRoleData.Head;
	pstEffigy->RoleFace = pstPlayer->stRoleData.Face;
	STRNCPY(pstEffigy->RoleName, pstPlayer->stRoleData.RoleName,
		      CCH(pstEffigy->RoleName));
	pstEffigy->RoleWear = pstPlayer->stRoleData.Wear;
	pstEffigy->RoleMiscFlag = pstPlayer->stRoleData.MiscInfo.Flag;
	pstEffigy->MapID = iMap;
	pstEffigy->Pos = *pstPos;
	
	return world_city_info_broadcast(pstEnv);
}

int all_player_world_city_info(ZONESVRENV * pstEnv)
{
	int i = 0;
	for (i=0; i<MAX_PLAYER; i++)
	{
		TMEMBLOCK* pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		Player * pstPlayer = NULL;
		
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (Player *)pstMemBlock->szData;		
		
		if(PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus ) 
		{
			player_world_city_info(pstEnv, pstPlayer);
		}
	}
	return 0;
}


//开放城池攻城
int world_city_open(ZONESVRENV *pstEnv)
{
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;

	if(!is_fini_global( pstEnv))
	{
		return 0;
	}
	
	pstWorldCity->CityCore.FightEnableStat = WORLD_CITY_OPEN;

	all_player_world_city_info(pstEnv);
		
	return 0;
}

//重置攻城战城池
int world_city_reset(ZONESVRENV *pstEnv)
{
	int i;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	WORLDCITYBUILD *pstWorldCityBuild = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity.CityBuild;

	for(i=0; i<pstWorldCityBuild->BuildNum; i++)
	{
		if(pstWorldCityBuild->BuildList[i].MemID > 0)
		{
			city_build_mon_clear(pstEnv, &pstWorldCityBuild->BuildList[i]);
		}
	}

	g_fWorldCitySiegeIn = 0;
	memset(pstWorldCity, 0, sizeof(*pstWorldCity));

	pstWorldCity->CityCore.Effigy.MapID = 1000;
	pstWorldCity->CityCore.Effigy.Pos.X = 26420;
	pstWorldCity->CityCore.Effigy.Pos.Y= 29945;
	
	//更新建筑
	global_world_city_update(pstEnv);

	//刷出建筑
	world_city_refresh(pstEnv);

	pstEnv->iWorldCityBuildSerial = 0;

	return 0;
}

//设置攻城强制开启、关闭
int world_city_siege_set(int fSiege)
{
	g_fWorldCitySiegeIn = fSiege;

	return 0;
}

int world_city_build_warning(ZONESVRENV *pstEnv, Monster *pstMon)
{
	ZONEMAPINST *pstMapInst;
	CITYBUILDINFO *pstBuildInfo;
	CSPKG stPkg;
	CSCITYSVR*pstCitySvr = &stPkg.Body.CitySvr;
	CSCITYBUILDWARNING *pstWarning = &pstCitySvr->Data.Warning;

	if(!is_city_build_mon(pstMon))
	{
		return 0;
	}
	
	pstBuildInfo = world_city_build_info_get(pstEnv, pstMon->iCityBuildDefID);
	if(NULL == pstBuildInfo || pstBuildInfo->Type == CITY_BUILD_TYPE_REVIVE)
	{
		return -1;
	}

	if(pstBuildInfo->WarningEndTime > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return 0;
	}
	pstBuildInfo->WarningEndTime = pstEnv->pstAppCtx->stCurr.tv_sec + 5;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstCitySvr->Type = CITY_SVR_BUILD_WARNING;
	pstWarning->DefID = pstBuildInfo->DefID;
	pstWarning->WarnEndTime = pstBuildInfo->WarningEndTime;
	pstWarning->ClanGID = pstMon->ullPKID;

	Z_CSHEAD_INIT(&stPkg.Head, CITY_SVR);
	return map_broadcast(pstEnv->pstAppCtx, pstEnv, pstMapInst, &stPkg);
}


//建造消耗检查
int world_city_build_cost_check(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneClan *pstClan, 
	CITYPRODUCTDEF *pstProductDef, int iMoney)
{
	int i = 0;
	CLANRESOURCE stResource = pstClan->stClan.ClanAdvance.Resource;

	if ((int)pstClan->stClan.ClanProf.Money < iMoney)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CITY201);
		return -1;
	}

	for(i=0; i<MAX_CITY_BUILD_MATERIAL; i++)
	{
		const CLANPRODUCTDEF *pstTestDef = NULL;

		if (!pstProductDef->BuildCost.Material[i].ID)
		{
			break;
		}
		
		pstTestDef = clan_product_def(pstEnv, pstProductDef->BuildCost.Material[i].ID);
		if (!pstTestDef)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CITY202);
			return -1;
		}

		if (clan_resource_dec_entries_in(pstEnv, &stResource,
		                                pstProductDef->BuildCost.Material[i].ID,
		                                pstProductDef->BuildCost.Material[i].Num,
		                                NULL) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CITY203,pstTestDef->Name);
			return -1;
		}
	}
	
	return 0;
}

//建造消耗
int world_city_build_cost(ZONESVRENV* pstEnv, ZoneClan *pstClan, 
	CITYPRODUCTDEF *pstProductDef, int iMoney)
{
	int i;
	int iRet = 0;
	
	clan_property_modify(pstEnv, pstClan, CLAN_PROPERTY_MONEY, iMoney, 1);
	
	//公会资源 pstProductDef->BuildCost.Material.ID
	for(i=0; i<MAX_CITY_BUILD_MATERIAL; i++)
	{
		if(0 > clan_resource_dec_entries( pstEnv, pstClan, 
			pstProductDef->BuildCost.Material[i].ID, pstProductDef->BuildCost.Material[i].Num))
		{
			iRet = -1;
		}
	}
	
	return iRet;;
}

//建造
int world_city_clt_create(ZONESVRENV* pstEnv, Player *pstPlayer, CSCITYBUILDCREATE *pstCSBuild)
{
	int iBuildMonDefID;
	CITYPRODUCTDEF *pstProductDef;
	CITYBUILDINFO *pstBuildInfo;
	WORLDCITYBUILDDEF *pstBuildDef;
	Monster *pstMon;
	MONSTERDEF *pstMonDef;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	if(NULL == pstClan)
	{
		return -1;
	}

	pstBuildDef = z_find_world_city_build_def(pstEnv, pstCSBuild->DefID);
	if(NULL == pstBuildDef)
	{
		return -1;
	}
	
	pstBuildInfo = world_city_build_info_get(pstEnv, pstCSBuild->DefID);
	if(NULL == pstBuildInfo)
	{
		goto _error;
	}

	//if(pstBuildInfo->State != CITY_BUILD_STAT_DIE)
	//{
	//	goto _error;
	//}
	
	//实际建造的怪物ID
	iBuildMonDefID = city_build_list_id_get(pstBuildDef, pstCSBuild->BuildListIdx, 1);
	if(iBuildMonDefID <=0)
	{
		return -1;
	}

	pstProductDef = z_find_city_product_def(pstEnv, iBuildMonDefID);
	if(NULL == pstProductDef)
	{
		return -1;
	}

	if(0 > world_city_build_cost_check(pstEnv, pstPlayer, pstClan, pstProductDef, pstProductDef->BuildCost.MakeClanMoney))
	{
		return -1;
	}
	world_city_build_cost(pstEnv, pstClan, pstProductDef, pstProductDef->BuildCost.MakeClanMoney);

	if(pstBuildInfo->MemID > 0)
	{
		city_build_mon_clear(pstEnv, pstBuildInfo);
	}

	city_build_info_born_set(pstBuildInfo, pstCSBuild->BuildListIdx, 1);
	pstMon = new_city_build_mon(pstEnv, pstBuildDef, pstBuildInfo, pstClan);
	if(NULL == pstMon)
	{
		return -1;
	}
	
	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if(pstMonDef)
	{
		player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD201,
	                          pstMonDef->Name);
	}
	                        
	return map_world_city_info_broadcast(pstEnv);
_error:

	player_open_world_city_ui_info(pstEnv, pstPlayer, 0);
	return 0;
}

//升级建筑
int world_city_clt_levelup(ZONESVRENV* pstEnv, Player *pstPlayer, CSCITYBUILDLEVELUP *pstCSBuild)
{
	int iBuildMonDefID, iCurrMaxLevel = 0;
	CITYPRODUCTDEF *pstProductDef;
	CITYBUILDINFO *pstBuildInfo;
	WORLDCITYBUILDDEF *pstBuildDef;
	Monster *pstMon;
	MONSTERDEF *pstMonDef;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	if(NULL == pstClan)
	{
		return -1;
	}

	pstBuildDef = z_find_world_city_build_def(pstEnv, pstCSBuild->DefID);
	if(NULL == pstBuildDef)
	{
		return -1;
	}
	
	pstBuildInfo = world_city_build_info_get(pstEnv, pstCSBuild->DefID);
	if(NULL == pstBuildInfo)
	{
		goto _error;
	}

	if(pstBuildInfo->State != CITY_BUILD_STAT_ALIVE)
	{
		goto _error;
	}

	iCurrMaxLevel = get_curr_max_level(pstEnv);
	if(iCurrMaxLevel <= 79 && pstBuildInfo->BuildLevel+1 == 2)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
			ZONEERR_CITY204);
		return -1;
	}

	if(iCurrMaxLevel <= 89 && pstBuildInfo->BuildLevel+1 == 3)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE, 
			ZONEERR_CITY205);
		return -1;
	}

	if(pstBuildInfo->BuildLevel >= MAX_CITY_BUILD_LEVEL)
	{
		//todo: msg
		return -1;
	}
	//实际建造的怪物ID
	iBuildMonDefID = city_build_list_id_get(pstBuildDef, pstBuildInfo->BuildIdx, pstBuildInfo->BuildLevel+1);
	if(iBuildMonDefID <=0)
	{
		return -1;
	}

	pstProductDef = z_find_city_product_def(pstEnv, iBuildMonDefID);
	if(NULL == pstProductDef)
	{
		return -1;
	}
	
	if(0 > world_city_build_cost_check(pstEnv, pstPlayer, pstClan, pstProductDef, pstProductDef->BuildCost.MakeClanMoney))
	{
		return -1;
	}
	world_city_build_cost(pstEnv, pstClan, pstProductDef, pstProductDef->BuildCost.MakeClanMoney);

	if(pstBuildInfo->MemID > 0)
	{
		city_build_mon_clear(pstEnv, pstBuildInfo);
	}
	
	city_build_info_born_set(pstBuildInfo, pstBuildInfo->BuildIdx, pstBuildInfo->BuildLevel+1);
	pstMon = new_city_build_mon(pstEnv, pstBuildDef, pstBuildInfo, pstClan);
	if(NULL == pstMon)
	{
		return -1;
	}
	
	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if(pstMonDef)
	{
		player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD202,
	                          pstMonDef->Name);
	}

	return map_world_city_info_broadcast(pstEnv);
_error:

	player_open_world_city_ui_info(pstEnv, pstPlayer, 0);
	return 0;
}

//修理建筑(HP\能量)
int world_city_clt_repair(ZONESVRENV* pstEnv, Player *pstPlayer, int fEnergy,CSCITYBUILDLEVELUP *pstCSBuild)
{
	int iBuildMonDefID;
	Monster *pstMon;
	CITYPRODUCTDEF *pstProductDef;
	CITYBUILDINFO *pstBuildInfo;
	WORLDCITYBUILDDEF *pstBuildDef;
	MONSTERDEF *pstMonDef;
	int iCostMoney = 0;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	if(NULL == pstClan)
	{
		return -1;
	}

	pstBuildDef = z_find_world_city_build_def(pstEnv, pstCSBuild->DefID);
	if(NULL == pstBuildDef)
	{
		return -1;
	}
	
	pstBuildInfo = world_city_build_info_get(pstEnv, pstCSBuild->DefID);
	if(NULL == pstBuildInfo || pstBuildInfo->State == CITY_BUILD_STAT_DIE)
	{
		goto _error;
	}

	iBuildMonDefID = city_build_list_id_get(pstBuildDef, pstBuildInfo->BuildIdx, pstBuildInfo->BuildLevel);
	if(iBuildMonDefID <=0)
	{
		return -1;
	}

	pstProductDef = z_find_city_product_def(pstEnv, iBuildMonDefID);
	if(NULL == pstProductDef)
	{
		return -1;
	}
	
	pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstBuildInfo->MemID);
	if(NULL == pstMon)
	{
		goto _error;
	}
	
	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if(NULL == pstMonDef)
	{
		goto _error;
	}

	if(fEnergy)
	{
		int iEnergy;
		int iCurr, iMax;
		
		if(!is_mon_machine(pstMon) ||
			pstBuildInfo->Energy >= 10000)
		{
			return -1;
		}

		iCurr = this_machine(pstEnv, pstMon)->stMachineInfo.Energy;
		iMax = this_machine(pstEnv, pstMon)->stMachineInfo.MaxEnergy;

		iCostMoney = (tdr_ulonglong)pstProductDef->BuildCost.EnergyClanMoney - iCurr*1.0*pstProductDef->BuildCost.EnergyClanMoney/iMax;
		if ((int)pstClan->stClan.ClanProf.Money < iCostMoney)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CITY206);
			return -1;
		}
		
		clan_property_modify(pstEnv, pstClan, CLAN_PROPERTY_MONEY, iCostMoney, 1);
		pstBuildInfo->Energy = 10000;
		iEnergy = this_machine(pstEnv, pstMon)->stMachineInfo.MaxEnergy - this_machine(pstEnv, pstMon)->stMachineInfo.Energy;
		z_machine_attr_chg(pstEnv, pstMon, MON_MACHINE_ATTR_ENERGY, iEnergy);

		player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD203,
	                          pstMonDef->Name);
	}
	else
	{
		if(pstBuildInfo->HP >= 10000)
		{
			return -1;
		}
		
		iCostMoney = (tdr_ulonglong)pstProductDef->BuildCost.RepairClanMoney -pstMon->iHP*1.0*pstProductDef->BuildCost.RepairClanMoney/pstMon->iMaxHP;
		if ((int)pstClan->stClan.ClanProf.Money < iCostMoney)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CITY207);
			return -1;
		}

		clan_property_modify(pstEnv, pstClan, CLAN_PROPERTY_MONEY, iCostMoney, 1);
		pstBuildInfo->HP = 10000;
		pstMon->iHP = pstMon->iMaxHP;
		mon_hp_action(pstEnv, pstMon, pstMon->iHP, 0);

		player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD204,
	                          pstMonDef->Name);
	}

	//更新地图信息
	return map_world_city_info_broadcast(pstEnv);
_error:

	player_open_world_city_ui_info(pstEnv, pstPlayer, 0);
	return 0;
}

//销毁建筑
int world_city_clt_del(ZONESVRENV* pstEnv, Player *pstPlayer, CSCITYBUILDLEVELUP *pstCSBuild)
{
	WORLDCITYBUILDDEF *pstBuildDef;
	CITYBUILDINFO *pstBuildInfo;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	if(NULL == pstClan)
	{
		return -1;
	}

	pstBuildDef = z_find_world_city_build_def(pstEnv, pstCSBuild->DefID);
	if(NULL == pstBuildDef)
	{
		return -1;
	}
	
	pstBuildInfo = world_city_build_info_get(pstEnv, pstCSBuild->DefID);
	if(NULL == pstBuildInfo)
	{
		goto _error;
	}

	if(pstBuildInfo->State == CITY_BUILD_STAT_DIE)
	{
		goto _error;
	}

	if(pstBuildInfo->MemID > 0)
	{
		Monster *pstDelMon;

		pstDelMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstBuildInfo->MemID);
		if(pstDelMon)
		{
			MONSTERDEF *pstDelMonDef = z_get_mon_def(pstEnv, pstDelMon);
			if(pstDelMonDef)
			{
				player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD205,
	                          pstDelMonDef->Name);
			}
		}
		
		city_build_mon_clear(pstEnv, pstBuildInfo);
	}

	city_build_info_die_set(pstBuildInfo);
	new_city_build_mon(pstEnv, pstBuildDef, pstBuildInfo, pstClan);
	
	return map_world_city_info_broadcast(pstEnv);
_error:

	player_open_world_city_ui_info(pstEnv, pstPlayer, 0);
	return 0;
}

int world_city_map_pos_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i, j, iIndex;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	AREADYN *pstArea;
	Player *pstTmpPlayer;
	CSPKG stPkg;
	CSCITYSVR *pstCitySvr = &stPkg.Body.CitySvr;
	CSCITYMAPPOSINFO *pstMapPosInfo = &pstCitySvr->Data.MapPosInfo;
	ZoneClan *pstMyClan = player_get_clan(pstEnv, pstPlayer);
	if(!pstMyClan)
	{
		return -1;
	}
	
	if(	WORLD_CITY_MAPID != GET_MAPID(pstPlayer->stRoleData.Map) &&
		!strong_player_is_in_pworld(pstEnv, pstPlayer))
	{
		return -1;
	}

	if(pstPlayer->stOnline.tCityCltCD > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return -1;
	}
	pstPlayer->stOnline.tCityCltCD = pstEnv->pstAppCtx->stCurr.tv_sec + 3;
	
	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstMapPosInfo->MemNum = 0;
	for (i = 0; i < pstMapInst->iAreaDyn; i++) 
	{
		iIndex	=	pstMapInst->aiAreaDyn[i];

		if( iIndex<0)
		{
			continue;
		}

		pstArea	=	(AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iIndex);
		if (NULL == pstArea)
		{
			continue;
		}
		
		for (j = 0; j < pstArea->iAnimate; j++) 
		{
			pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstArea->aiAnimate[j]);
			if(NULL == pstTmpPlayer || PLAYER_STATUS_ROLE_LOGIN != pstTmpPlayer->eStatus)
			{
				continue;
			}

			if(0 > player_check_same_clan(pstEnv, pstPlayer, pstTmpPlayer))
			{
				continue;
			}

			pstMapPosInfo->Members[pstMapPosInfo->MemNum].MemID = pstTmpPlayer->iMemID;
			pstMapPosInfo->Members[pstMapPosInfo->MemNum].Pos = pstTmpPlayer->stRoleData.Pos;
			pstMapPosInfo->MemNum++;

			if(pstMapPosInfo->MemNum >= CLAN_MAX_MEM)
			{
				goto _final;
			}
		}
	}

_final:
	
	pstCitySvr->Type = CITY_SVR_MAP_MEM_POS;
	Z_CSHEAD_INIT(&stPkg.Head, CITY_SVR);
	return z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
}

int world_city_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
			TFRAMEHEAD	 *pstFrameHead, CSPKG *pstCsPkg)
{
	CSCITYCLT *pstCityClt = &pstCsPkg->Body.CityClt;
	unsigned int uiClanPostion = player_clan_position(pstEnv, pstPlayer);

	if(pstCityClt->Type != CITY_CLT_MAP_MEM_POS)
	{
		//非战争期间才可以
		if(is_in_siege_city_fast(pstEnv))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
				ZONEERR_CITY208);
			return 0;
		}
		
		if(0 > check_npc_ui(pstEnv, pstPlayer, UI_TYPE_WORLD_CITY))
		{
			return -1;
		}

		//只有所有公会能操作
		if(!is_world_city_owner(pstEnv, pstPlayer))
		{
			return -1;
		}

		//只有会长，副会长可以
		if(uiClanPostion != CLAN_POS_PRESIDENT  &&
			uiClanPostion != CLAN_POS_VICE_PRESIDENT)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
				ZONEERR_CITY209);
			return -1;
		}
	}

	switch(pstCityClt->Type)
	{
		case CITY_CLT_BUILD_CREATE:
			return world_city_clt_create(pstEnv, pstPlayer, &pstCityClt->Data.Create);
			break;
		case CITY_CLT_BUILD_LEVELUP:
			return world_city_clt_levelup(pstEnv, pstPlayer, &pstCityClt->Data.LevelUP);
			break;
		case CITY_CLT_BUILD_REPAIR:
			return world_city_clt_repair(pstEnv, pstPlayer, 0, &pstCityClt->Data.Repair);
			break;
		case CITY_CLT_BUILD_ADD_ENERGY:
			return world_city_clt_repair(pstEnv, pstPlayer, 0, &pstCityClt->Data.Repair);
			break;
		case CITY_CLT_BUILD_DEL:
			return world_city_clt_del(pstEnv, pstPlayer, &pstCityClt->Data.Del);
			break;
		case CITY_CLT_MAP_MEM_POS:
			return world_city_map_pos_info(pstEnv, pstPlayer);
			break;
		default:
			break;
	}

	return 0;
	UNUSED(pstAppCtx);
	UNUSED(pstFrameHead);
}

int global_world_city_update(ZONESVRENV *pstEnv)
{
	int i;
	CITYBUILDINFO *pstBuildInfo;
	WORLDCITYBUILDDEF *pstBuildDef;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	WORLDCITYBUILD *pstWorldCityBuild = &pstWorldCity->CityBuild;
	
	if(!is_fini_global( pstEnv))
	{
		return 0;
	}

	for(i=pstWorldCityBuild->BuildNum-1; i>=0; i--)
	{
		pstBuildInfo = &pstWorldCityBuild->BuildList[i];
		pstBuildDef = z_find_world_city_build_def(pstEnv, pstBuildInfo->DefID);
		if(NULL == pstBuildDef)
		{
			pstWorldCityBuild->BuildNum--;
			if(i != pstWorldCityBuild->BuildNum)
			{
				city_build_mon_clear(pstEnv, pstBuildInfo);
				pstWorldCityBuild->BuildList[i] = pstWorldCityBuild->BuildList[pstWorldCityBuild->BuildNum];
			}
		
			continue;
		}
	}

	//更新表格数据
	for(i = 0; i<pstEnv->pstZoneObj->iWorldCityBuildDef; i++)
	{
		pstBuildDef = (WORLDCITYBUILDDEF *)pstEnv->pstZoneObj->sWorldCityBuildDef[i];

		world_city_build_info_update(pstEnv, pstBuildDef);
	}

	return 0;
}

//从DB拉回的数据需要清理掉记录的建筑内存ID，防止误删除
int global_city_build_online_clear(ZONESVRENV *pstEnv)
{
	int i;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	WORLDCITYBUILD *pstWorldCityBuild = &pstWorldCity->CityBuild;

	for(i=0; i<pstWorldCityBuild->BuildNum; i++)
	{
		pstWorldCityBuild->BuildList[i].MemID = 0;
	}

	return 0;
}

int global_world_city_init(ZONESVRENV *pstEnv)
{
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	
	if(!is_fini_global( pstEnv))
	{
		return 0;
	}

	if(pstWorldCity->CityCore.Effigy.MapID == 0)
	{
		pstWorldCity->CityCore.Effigy.MapID = 1000;
		pstWorldCity->CityCore.Effigy.Pos.X = 26420;
		pstWorldCity->CityCore.Effigy.Pos.Y= 29945;
	}
	
	//清理关机残留MEMID
	global_city_build_online_clear(pstEnv);

	//更新建筑
	global_world_city_update(pstEnv);

	//刷出建筑
	world_city_refresh(pstEnv);

	return 0;
}

int map_one_city_build_info(ZONESVRENV *pstEnv, CITYBUILDINFO *pstBuildInfo, CSCITYBUILDINFO *pstCSBuildInfo)
{
	Monster *pstMon;

	memset(pstCSBuildInfo, 0, sizeof(*pstCSBuildInfo));
	pstCSBuildInfo->State = pstBuildInfo->State;
	pstCSBuildInfo->DefID = pstBuildInfo->DefID;
	pstCSBuildInfo->BuildIdx = pstBuildInfo->BuildIdx;
	pstCSBuildInfo->BuildLevel = pstBuildInfo->BuildLevel;
	pstCSBuildInfo->WarnEndTime = pstBuildInfo->WarningEndTime;
	pstCSBuildInfo->Type = pstBuildInfo->Type;
	
	pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstBuildInfo->MemID);
	if(NULL == pstMon)
	{
		return 0;
	}

	pstCSBuildInfo->MemID = pstMon->iID;
	pstCSBuildInfo->HP = pstMon->iHP;
	pstCSBuildInfo->MaxHP = pstMon->iMaxHP;

	if(is_mon_machine(pstMon))
	{
		pstCSBuildInfo->Energy = this_machine(pstEnv, pstMon)->stMachineInfo.Energy;
		pstCSBuildInfo->MaxEnergy = this_machine(pstEnv, pstMon)->stMachineInfo.MaxEnergy;
	}

	if(pstMon->nOwnerFastID > 0)
	{
		ZoneClan *pstClan = z_id_clan(pstEnv, pstMon->nOwnerFastID);
		if(pstClan)
		{
			pstCSBuildInfo->ClanGID = pstClan->stClan.ClanProf.GID;
			STRNCPY(pstCSBuildInfo->ClanName, 
				pstClan->stClan.ClanProf.Name, sizeof(pstCSBuildInfo->ClanName));
		}
	}

	return 0;
	
}

int map_world_city_build_info_get(ZONESVRENV *pstEnv, CSCITYALLBUILD *pstAllBuild)
{
	int i;
	CITYBUILDINFO *pstBuildInfo;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	WORLDCITYBUILD *pstWorldCityBuild = &pstWorldCity->CityBuild;

	memset(&pstAllBuild->CaptureInfo, 0, sizeof(pstAllBuild->CaptureInfo));
	pstAllBuild->BuildNum = 0;
	pstAllBuild->Stat = pstWorldCity->CityCore.Stat;
	
	if(pstWorldCity->CityCore.CaptureEndTime > 0)
	{
		int iCount = 0;
		ZoneClan *pstSiegeClan = city_build_control_info_get(pstEnv, &iCount);
		//防范一下
		if(pstSiegeClan && iCount == 3)
		{
			STRNCPY(pstAllBuild->CaptureInfo.ClanName, 
				pstSiegeClan->stClan.ClanProf.Name, sizeof(pstAllBuild->CaptureInfo.ClanName));
			pstAllBuild->CaptureInfo.CaptureEndTime = pstWorldCity->CityCore.CaptureEndTime;
		}
	}

	for(i=0; i<pstWorldCityBuild->BuildNum; i++)
	{
		pstBuildInfo = &pstWorldCityBuild->BuildList[i];
		map_one_city_build_info(pstEnv, pstBuildInfo, &pstAllBuild->BuildList[pstAllBuild->BuildNum]);
		pstAllBuild->BuildNum++;
	}

	return 0;
}

int map_world_city_info_broadcast(ZONESVRENV *pstEnv)
{
	CSPKG stPkg;
	CSCITYSVR*pstCitySvr = &stPkg.Body.CitySvr;
	CSCITYALLBUILD *pstAllBuild = &pstCitySvr->Data.MapInfo;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	
	stIdx.iID = WORLD_CITY_MAPID;
	stIdx.iPos = -1;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstCitySvr->Type = CITY_SVR_MAP_INFO;
	map_world_city_build_info_get(pstEnv, pstAllBuild);

	Z_CSHEAD_INIT(&stPkg.Head, CITY_SVR);
	return map_broadcast(pstEnv->pstAppCtx, pstEnv, pstMapInst, &stPkg);
}

int player_enter_world_city_info(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	CSPKG stPkg;
	CSCITYSVR*pstCitySvr = &stPkg.Body.CitySvr;
	CSCITYALLBUILD *pstAllBuild = &pstCitySvr->Data.MapInfo;

	if(WORLD_CITY_MAPID != GET_MAPID(pstPlayer->stRoleData.Map))
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

	pstCitySvr->Type = CITY_SVR_MAP_INFO;
	map_world_city_build_info_get(pstEnv, pstAllBuild);

	Z_CSHEAD_INIT(&stPkg.Head, CITY_SVR);
	return z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
}

int player_open_world_city_ui_info(ZONESVRENV *pstEnv, Player *pstPlayer, int iNpcID)
{
	int i;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	CITYBUILDINFO *pstBuildInfo;
	WORLDCITYBUILD *pstWorldCityBuild = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity.CityBuild;
	CSPKG stPkg;
	CSCITYSVR*pstCitySvr = &stPkg.Body.CitySvr;
	CSCITYBUILDUI *pstBuildUIInfo = &pstCitySvr->Data.UIInfo;
	
	if(WORLD_CITY_MAPID != GET_MAPID(pstPlayer->stRoleData.Map))
	{
		return -1;
	}

	if(!is_world_city_owner(pstEnv, pstPlayer))
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

	pstCitySvr->Type = CITY_SVR_BUILD_UI_INFO;
	pstBuildUIInfo->NpcID = iNpcID;
	pstBuildUIInfo->BuildNum = 0;
	for(i=0; i<pstWorldCityBuild->BuildNum; i++)
	{
		pstBuildInfo = &pstWorldCityBuild->BuildList[i];
		map_one_city_build_info(pstEnv, pstBuildInfo, &pstBuildUIInfo->BuildList[pstBuildUIInfo->BuildNum]);
		pstBuildUIInfo->BuildNum++;
	}

	Z_CSHEAD_INIT(&stPkg.Head, CITY_SVR);
	return z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
}

void player_exit_info_check(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int iEnterMap = 0;
	MapIndex *pstMapIdx;
	
	if(WORLD_CITY_MAPID != GET_MAPID(pstPlayer->stRoleData.ExitMap))
	{
		return ;
	}

	//回到主城复活点
	iEnterMap = BASE_CITY_MAPID;
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, BASE_CITY_MAPID);
	if (NULL == pstMapIdx)
	{
		return ;
	}

	pstPlayer->stRoleData.ExitMap = BASE_CITY_MAPID;
	pstPlayer->stRoleData.ExitPos_x = pstMapIdx->stMapDef.RevivePs[0].X;
	pstPlayer->stRoleData.ExitPos_y= pstMapIdx->stMapDef.RevivePs[0].Y;
	
	return ;
}

int world_city_master_notify(ZONESVRENV* pstEnv, Player *pstPlayer, int fIn)
{
	if(!is_world_city_master(pstEnv, pstPlayer))
	{
		return 0;
	}
	
	if(fIn)
	{
		z_sys_strf_broadcast(pstEnv, SYS_CHAT, ZONEERR_BROAD309,
			pstPlayer->stRoleData.RoleName);
	}
	else
	{
		z_sys_strf_broadcast(pstEnv, SYS_CHAT, ZONEERR_BROAD310,
			pstPlayer->stRoleData.RoleName);
	}

	return 0;
}

int player_world_city_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSCITYSVR*pstCitySvr = &stPkg.Body.CitySvr;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;

	if(!is_fini_global( pstEnv))
	{
		return 0;
	}
	
	pstCitySvr->Type = CITY_SVR_CORE_INFO;
	pstCitySvr->Data.CoreInfo.ClanGID = pstWorldCity->CityCore.ClanGID;
	pstCitySvr->Data.CoreInfo.ClanId = pstWorldCity->CityCore.ClanId;
	pstCitySvr->Data.CoreInfo.KeepNum = pstWorldCity->CityCore.ClanKeepNum;
	pstCitySvr->Data.CoreInfo.Effigy = pstWorldCity->CityCore.Effigy;
	pstCitySvr->Data.CoreInfo.FightEnableStat = pstWorldCity->CityCore.FightEnableStat;
	pstCitySvr->Data.CoreInfo.Stat = pstWorldCity->CityCore.Stat;
	
	Z_CSHEAD_INIT(&stPkg.Head, CITY_SVR);
	return z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
}


int world_city_add_bulletin(ZONESVRENV* pstEnv,int iActiveTime,int iRollInterval, char *pszStr, ...)
{
	SSBULLETIN stReq;
	char szMsg[SYS_MSG_LEN];
	va_list valist;
	
	memset(&stReq,0,sizeof(stReq));	
	
	va_start( valist, pszStr );
	vsnprintf(szMsg, sizeof(szMsg), pszStr, valist);
	szMsg[SYS_MSG_LEN -1] = 0;
	va_end(valist);
	
	stReq.ActiveTime = iActiveTime-10;
	stReq.Core.ID = z_gen_bulletin_id();
	stReq.Core.RollInterval = iRollInterval;
	STRNCPY(stReq.Core.Text, szMsg, CCH(stReq.Core.Text));

	if(z_add_bulletin(pstEnv, &stReq) < 0)
	{
		return -1;
	}

	return 0;
}

int world_city_revive_pos_get(ZONESVRENV* pstEnv, Player *pstPlayer, MapIndex *pstMapIdx, int *piMapIdx, RESPOS *pstPos)
{
	int i, iDist = 0, iTmpDist = 0;
	CITYBUILDINFO *pstBuildInfo;
	WORLDCITYBUILDDEF *pstBuildDef;
	Monster *pstMon;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	WORLDCITYBUILD *pstWorldCityBuild = &pstWorldCity->CityBuild;
	
	if(is_world_city_owner(pstEnv, pstPlayer))
	{
		*piMapIdx = pstMapIdx->stMapDef.ReviveMap;
		*pstPos = pstMapIdx->stMapDef.RevivePs[0];

		iDist  = 9999999;
	}
	else
	{
		*piMapIdx = pstMapIdx->stMapDef.ReviveMap;
		*pstPos = pstMapIdx->stMapDef.RevivePs[1];	

		iDist  = 9999999;
	}

	for(i=pstWorldCityBuild->BuildNum-1; i>=0; i--)
	{
		pstBuildInfo = &pstWorldCityBuild->BuildList[i];
		if(pstBuildInfo->Type != CITY_BUILD_TYPE_REVIVE ||
			pstBuildInfo->State != CITY_BUILD_STAT_ALIVE)
		{
			continue;
		}

		pstBuildDef = z_find_world_city_build_def(pstEnv, pstBuildInfo->DefID);
		if(NULL == pstBuildDef)
		{
			continue;
		}

		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstBuildInfo->MemID);
		if(NULL == pstMon)
		{
			continue;
		}

		if(pstMon->iPKType == MON_PK_TYPE_CLAN &&
			pstMon->ullPKID > 0 &&
			pstMon->ullPKID == pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID)
		{
			iTmpDist  = z_distance(&pstPlayer->stRoleData.Pos, &pstMon->stCurrPos);
			if(iTmpDist < iDist)
			{
				*pstPos = pstBuildDef->RevivePos;	
				iDist  = z_distance(&pstPlayer->stRoleData.Pos, pstPos);
			}
		}
	}

	return 0;
}

int world_city_open_ui_action(ZONESVRENV* pstEnv)
{
	CSPKG stPkg;
	CSACTIVEGAMESVR *pstActiveGameSvr = &stPkg.Body.ActiveGameSvr;
	CSACTIVEGAMEUIINFO *pstActiveUIInfo = &pstActiveGameSvr->Data.UIInfo;

	pstActiveGameSvr->Type = ACTIVE_GAME_SVR_TYPE_UI_INFO;
	pstActiveUIInfo->UIType = ACTIVE_UI_TYPE_WORLD_CITY_OPEN;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTIVE_GAME_SVR);
	return z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
}

int world_city_player_kill_msg(ZONESVRENV* pstEnv, Player *pstKiller, ZONEMAPINST *pstMapInst)
{

	if (pstKiller->stOnline.stCityFightOL.iContinueKill == 100)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE,ZONEERR_BROADMAP305, 
					pstKiller->stRoleData.RoleName, pstKiller->stOnline.stCityFightOL.iContinueKill);	
		z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstKiller, 3190, 
									3, 3600, 1, 0);
	}
	else if (pstKiller->stOnline.stCityFightOL.iContinueKill == 50)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE,ZONEERR_BROADMAP306, 
					pstKiller->stRoleData.RoleName, pstKiller->stOnline.stCityFightOL.iContinueKill);	

		z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstKiller, 3190, 
									2, 3600, 1, 0);
	}
	else if (pstKiller->stOnline.stCityFightOL.iContinueKill == 20)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE,ZONEERR_BROADMAP307, 
					pstKiller->stRoleData.RoleName, pstKiller->stOnline.stCityFightOL.iContinueKill);	
		z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstKiller, 3190, 
									1, 3600, 1, 0);
	}
	else if(pstKiller->stOnline.stCityFightOL.iContinueKill == 10)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE,ZONEERR_BROADMAP308, 
					pstKiller->stRoleData.RoleName, pstKiller->stOnline.stCityFightOL.iContinueKill);	
	}
	else if (pstKiller->stOnline.stCityFightOL.iContinueKill == 5)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE,ZONEERR_BROADMAP309, 
					pstKiller->stRoleData.RoleName, pstKiller->stOnline.stCityFightOL.iContinueKill);
	}
	else if (pstKiller->stOnline.stCityFightOL.iContinueKill == 3)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE,ZONEERR_BROADMAP310, 
					pstKiller->stRoleData.RoleName, pstKiller->stOnline.stCityFightOL.iContinueKill);
	}
	else
	{
		return 0;
	}
	
	return 0;
}

int world_city_player_die(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneAni *pstAtker)
{
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	Player *pstKiller = get_atker_player(pstEnv,  pstAtker);

	if(WORLD_CITY_MAPID != GET_MAPID(pstPlayer->stRoleData.Map))
	{
		return -1;
	}

	if(!is_in_siege_city_fast(pstEnv))
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

	if(pstKiller && WORLD_CITY_MAPID == GET_MAPID(pstKiller->stRoleData.Map))
	{
		ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);

		if(pstClan)
		{
			unsigned int uiClanPostion = player_clan_position(pstEnv, pstPlayer);
			if(uiClanPostion == CLAN_POS_PRESIDENT  ||
				uiClanPostion == CLAN_POS_VICE_PRESIDENT)
			{
				z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE, 
								ZONEERR_BROADMAP311, 
								pstKiller->stRoleData.RoleName, pstClan->stClan.ClanProf.Name,
								kClanPosName[uiClanPostion], pstPlayer->stRoleData.RoleName);
			}
		}
		
		pstKiller->stOnline.stCityFightOL.iContinueKill++;
		world_city_kill_info(pstEnv, pstKiller);
		world_city_player_kill_msg(pstEnv, pstKiller, pstMapInst);

		if (pstPlayer->stOnline.stCityFightOL.iContinueKill > 2)
		{
			z_sys_strf_map_broadcast(pstEnv, pstMapInst,SYS_MIDDLE,ZONEERR_BROADMAP312, 
					pstKiller->stRoleData.RoleName, pstPlayer->stRoleData.RoleName,
					pstPlayer->stOnline.stCityFightOL.iContinueKill);
		}
	}

	pstPlayer->stOnline.stCityFightOL.iContinueKill = 0;
	world_city_kill_info(pstEnv, pstPlayer);

	return 0;
}

 int get_world_city_clan_pos(ZONESVRENV *pstEnv, Player *pstPlayer)
 {
       unsigned int uiClanPostion = player_clan_position(pstEnv, pstPlayer);
       int iPos = 0;

      if(is_world_city_owner(pstEnv, pstPlayer))
       {
              iPos = 1;
              if(uiClanPostion == CLAN_POS_PRESIDENT)
              {
                      iPos = 2;
               }
      }

       return iPos;
 }

 int world_city_enter_time_update(ZONESVRENV *pstEnv)
 {
	int i, j, iIndex;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	AREADYN *pstArea;
	Player *pstTmpPlayer;

	stIdx.iID = WORLD_CITY_MAPID;
	stIdx.iPos = -1;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	for (i = 0; i < pstMapInst->iAreaDyn; i++) 
	{
		iIndex	=	pstMapInst->aiAreaDyn[i];
		if( iIndex<0)
		{
			continue;
		}

		pstArea	=	(AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iIndex);
		if (NULL == pstArea)
		{
			continue;
		}
		
		for (j = 0; j < pstArea->iAnimate; j++) 
		{
			pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstArea->aiAnimate[j]);
			if(NULL == pstTmpPlayer)
			{
				continue;
			}

			pstTmpPlayer->stRoleData.MiscInfo.EnterCityTime = pstEnv->pstAppCtx->stCurr.tv_sec;
			pstTmpPlayer->stRoleData.MiscInfo.CityFightTime = 0;
		}
	}
	
	return 0;
 }

int world_city_fight_range_insert_clan(ZONESVRENV *pstEnv, ZoneClan *pstClan)
{
	int i;
	WORLDCITYCLANFIGHTINFO* pstClanFightInfo;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	
	for(i=0; i<pstWorldCity->ClanRange.Num; i++)
	{
		pstClanFightInfo = &pstWorldCity->ClanRange.ClanList[i];
		if(pstClanFightInfo->ClanId.GID == pstClan->stClan.ClanProf.GID)
		{
			return 0;
		}
	}

	if(pstWorldCity->ClanRange.Num >= MAX_WORLD_CITY_CLAN_RANGE)
	{
		return -1;
	}

	pstClanFightInfo = &pstWorldCity->ClanRange.ClanList[pstWorldCity->ClanRange.Num];
	pstClanFightInfo->ClanId = *clan_id_warp(&pstClan->stClan);
	pstWorldCity->ClanRange.Num++;

	return 0;
}

int world_city_fight_range_init(ZONESVRENV *pstEnv)
{
	int i;
	ZoneClan *pstClan;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;

	memset(&pstWorldCity->ClanRange, 0, sizeof(pstWorldCity->ClanRange));

	pstClan = z_id_clan(pstEnv, pstWorldCity->CityCore.ClanId);
	if(pstClan)
	{
		world_city_fight_range_insert_clan(pstEnv, pstClan);
	}
	
	for(i=0; i<pstInfo->Num; i++)
	{
		pstClan = z_id_clan(pstEnv, pstInfo->StrongPoints[i].OwerID);
		if(!pstClan)
		{
			continue;
		}

		world_city_fight_range_insert_clan(pstEnv, pstClan);
	}

	//先填充下防守方的数据
	world_city_owner_fight_info_update(pstEnv);

	world_city_enter_time_update(pstEnv);

	world_city_fight_range_svr_info(pstEnv, NULL, 0);
	
	return 0;
}

WORLDCITYCLANFIGHTINFO* clan_fight_range_info_get(ZONESVRENV *pstEnv, ZoneClan *pstClan)
{
	int i;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;

	for(i=0; i<pstWorldCity->ClanRange.Num; i++)
	{
		if(pstClan->stClan.ClanProf.GID == pstWorldCity->ClanRange.ClanList[i].ClanId.GID)
		{
			return &pstWorldCity->ClanRange.ClanList[i];
		}
	}
	
	return NULL;
}

int world_city_fight_range_val_cacl(WORLDCITYCLANFIGHTINFO* pstClanFightInfo)
{
	pstClanFightInfo->Total = pstClanFightInfo->BuffBuild*20 +
				pstClanFightInfo->ControlBuild*150 +
				pstClanFightInfo->DefBuild*25+
				pstClanFightInfo->DoorBuild*30+
				pstClanFightInfo->ReviveBuild*60;
	return 0;
}

int world_city_one_clan_range_update(ZONESVRENV* pstEnv, WORLDCITYCLANFIGHTINFO* pstClanFightInfo)
{
	int i;
	Monster *pstMon;
	CITYBUILDINFO *pstBuildInfo;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	WORLDCITYBUILD *pstWorldCityBuild = &pstWorldCity->CityBuild;

	pstClanFightInfo->ReviveBuild = 0;
	pstClanFightInfo->ControlBuild = 0;
	for(i=pstWorldCityBuild->BuildNum-1; i>=0; i--)
	{
		pstBuildInfo = &pstWorldCityBuild->BuildList[i];
		//if(pstBuildInfo->State != CITY_BUILD_STAT_ALIVE)
		//{
		//	continue;
		//}

		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstWorldCityBuild->BuildList[i].MemID);
		if(NULL == pstMon || pstMon->iPKType != MON_PK_TYPE_CLAN)
		{
			continue;
		}

		if(pstMon->ullPKID != pstClanFightInfo->ClanId.GID)
		{
			continue;
		}

		switch(pstBuildInfo->Type)
		{
			case CITY_BUILD_TYPE_REVIVE:
				pstClanFightInfo->ReviveBuild++;
				break;
			case CITY_BUILD_TYPE_CONTROL:
				pstClanFightInfo->ControlBuild++;
				break;
			default:
				break;
		}
	}

	world_city_fight_range_val_cacl(pstClanFightInfo);
	return 0;
}

int world_city_owner_range_update(ZONESVRENV *pstEnv)
{
	int i;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;

	for(i=0; i<pstWorldCity->ClanRange.Num; i++)
	{
		world_city_one_clan_range_update(pstEnv, &pstWorldCity->ClanRange.ClanList[i]);
	}

	return 0;
}

int world_city_fight_range_update(ZONESVRENV *pstEnv, ZoneClan *pstClan, int iBuildType)
{
	WORLDCITYCLANFIGHTINFO* pstClanFightInfo = clan_fight_range_info_get(pstEnv, pstClan);

	if(!pstClanFightInfo)
	{
		return 0;
	}

	pstClanFightInfo->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	switch(iBuildType)
	{
		case CITY_BUILD_TYPE_DOOR_BUILD:
		case CITY_BUILD_TYPE_DOOR2:
		case CITY_BUILD_TYPE_DOOR3:
			pstClanFightInfo->DoorBuild++;
			break;
		case CITY_BUILD_TYPE_CONTROL:
			//pstClanFightInfo->ControlBuild++;
			return world_city_owner_range_update(pstEnv);
			break;
		case CITY_BUILD_TYPE_DEF:
			pstClanFightInfo->DefBuild++;
			break;
		case CITY_BUILD_TYPE_BUFF:
			pstClanFightInfo->BuffBuild++;
			break;
		case CITY_BUILD_TYPE_REVIVE:
			//pstClanFightInfo->ReviveBuild++;
			return world_city_owner_range_update(pstEnv);
			break;
		default:
			break;
	}

	world_city_fight_range_val_cacl(pstClanFightInfo);

	//world_city_owner_fight_info_update(pstEnv);
	
	return 0;
}

static int clan_fight_qsort_cmp(const void *p1, const void *p2)
{
	WORLDCITYCLANFIGHTINFO *pInfo1 = (WORLDCITYCLANFIGHTINFO*)p1;
	WORLDCITYCLANFIGHTINFO *pInfo2 = (WORLDCITYCLANFIGHTINFO*)p2;

	if(pInfo2->Total !=  pInfo1->Total)
	{
		return pInfo2->Total - pInfo1->Total;
	}
	else
	{
		return pInfo1->Time - pInfo2->Time;
	}

	return pInfo2->Total - pInfo1->Total;
}

int player_world_city_clan_range_get(ZONESVRENV* pstEnv, Player * pstPlayer, CSCITYCLANFIGHTRANGE *pstFightRange)
{
	int i;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	if(!pstClan)
	{
		return 0;
	}

	for(i=0; i<pstFightRange->Num; i++)
	{	
		if(pstFightRange->ClanList[i].ClanId.GID == pstClan->stClan.ClanProf.GID)
		{
			return i+1;
		}
	}

	return 0;
}

int world_city_clan_mem_award(ZONESVRENV* pstEnv, CSPKG *pstPkg)
{
	int iTop;
	int i, j, iIndex;
	tdr_ulonglong ullExp;
	LEVELUP *pstLevelUP;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	ROLEGRID stRoleGrid;
	AREADYN *pstArea;
	Player *pstTmpPlayer;
	WORLDCITYFIGHTAWARDDEF *pstAwardDef;
	CSCITYCLANFIGHTRANGE *pstFightRange = &pstPkg->Body.CitySvr.Data.ClanFightRange;
	
	stIdx.iID = WORLD_CITY_MAPID;
	stIdx.iPos = -1;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	for (i = 0; i < pstMapInst->iAreaDyn; i++) 
	{
		iIndex	=	pstMapInst->aiAreaDyn[i];
		if( iIndex<0)
		{
			continue;
		}

		pstArea	=	(AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iIndex);
		if (NULL == pstArea)
		{
			continue;
		}
		
		for (j = 0; j < pstArea->iAnimate; j++) 
		{
			pstFightRange->Money = 0;
			pstFightRange->MoneyType = 0;
			pstFightRange->SelfClanTop = 0;
			
			pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstArea->aiAnimate[j]);
			if(NULL == pstTmpPlayer || PLAYER_STATUS_ROLE_LOGIN != pstTmpPlayer->eStatus)
			{
				continue;
			}
			pstFightRange->EnterTime = pstTmpPlayer->stRoleData.MiscInfo.CityFightTime;

			iTop = player_world_city_clan_range_get(pstEnv, pstTmpPlayer, pstFightRange);
			if(iTop <= 0)
			{
				z_cltmsg_send(pstEnv, pstTmpPlayer, pstPkg);
				continue;
			}
			pstFightRange->SelfClanTop = iTop;

			//TODO: 符合条件
			if(pstTmpPlayer->stRoleData.MiscInfo.CityFightTime < 1500 &&
				pstFightRange->ClanList[iTop-1].Total <= 100)
			{
				z_cltmsg_send(pstEnv, pstTmpPlayer, pstPkg);
				continue;
			}

			pstAwardDef = z_find_world_city_fight_award_def(pstEnv, iTop);
			if(!pstAwardDef)
			{
				z_cltmsg_send(pstEnv, pstTmpPlayer, pstPkg);
				continue;
			}

			//经验值奖励
			pstLevelUP = z_get_levelup(pstEnv, pstTmpPlayer->stRoleData.Level);
			if (pstLevelUP)
		 	{
				ullExp = pstAwardDef->Exp/10000.0*pstLevelUP->ContributeExp;

				pstTmpPlayer->stOnline.cMiscFlag |= MISC_FLAG_EXP_NOLEVEL_LIMIT;
				player_add_exp(pstEnv, pstTmpPlayer, ullExp, 0);
				pstTmpPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_EXP_NOLEVEL_LIMIT;
				
		 	}

			pstFightRange->Money = pstAwardDef->Money;
			pstFightRange->MoneyType = pstAwardDef->MoneyType;

			package_add_money(pstEnv, pstTmpPlayer, pstAwardDef->MoneyType, pstAwardDef->Money);
			if (ATTR_ID_BULL == pstAwardDef->MoneyType)
			{
				z_money_create_oplog(pstEnv, pstTmpPlayer, MONEY_OPTYPE_CITYFIGHT, 0,  pstAwardDef->Money, 
						0, "城战");
			}
			else if (ATTR_ID_MONEY == pstAwardDef->MoneyType)
			{
				z_money_create_oplog(pstEnv, pstTmpPlayer, MONEY_OPTYPE_CITYFIGHT, 0,  0, 
						pstAwardDef->Money, "城战");
			}

			if(pstAwardDef->AwardItemID > 0 &&
				0 == item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstAwardDef->AwardItemID))
			{
				char szMsg[256];

				sprintf(szMsg, 
					LJY_MAIL5, 
					pstTmpPlayer->stRoleData.RoleName, iTop);
			
				player_sysmail_fast_send(pstEnv, pstTmpPlayer->stRoleData.RoleName, 
					LJY_MAIL6, 
					szMsg, 
					&stRoleGrid, 1, 0,NULL, 0);
			}
			
			z_cltmsg_send(pstEnv, pstTmpPlayer, pstPkg);
		}
	}
	
	return 0;
}

int world_city_owner_fight_info_update(ZONESVRENV* pstEnv)
{
	int i;
	Monster *pstMon;
	ZoneClan *pstClan;
	CITYBUILDINFO *pstBuildInfo;
	WORLDCITYCLANFIGHTINFO* pstClanFightInfo;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	WORLDCITYBUILD *pstWorldCityBuild = &pstWorldCity->CityBuild;

	pstClan = z_id_clan(pstEnv, pstWorldCity->CityCore.ClanId);
	if(!pstClan)
	{
		return 0;
	}
	pstClanFightInfo = clan_fight_range_info_get(pstEnv, pstClan);
	if(!pstClanFightInfo)
	{
		return 0;
	}

	pstClanFightInfo->BuffBuild = 0;
	pstClanFightInfo->DefBuild = 0;
	pstClanFightInfo->DoorBuild = 0;
	pstClanFightInfo->ReviveBuild = 0;
	pstClanFightInfo->ControlBuild = 0;
	for(i=pstWorldCityBuild->BuildNum-1; i>=0; i--)
	{
		pstBuildInfo = &pstWorldCityBuild->BuildList[i];
		if(pstBuildInfo->State != CITY_BUILD_STAT_ALIVE)
		{
			continue;
		}

		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstWorldCityBuild->BuildList[i].MemID);
		if(NULL == pstMon || pstMon->iPKType != MON_PK_TYPE_CLAN)
		{
			continue;
		}

		if(pstMon->ullPKID != pstClanFightInfo->ClanId.GID)
		{
			continue;
		}

		switch(pstBuildInfo->Type)
		{
			case CITY_BUILD_TYPE_DEF:
				pstClanFightInfo->DefBuild++;
				break;
			case CITY_BUILD_TYPE_BUFF:
				pstClanFightInfo->BuffBuild++;
				break;
			case CITY_BUILD_TYPE_DOOR_BUILD:
			case CITY_BUILD_TYPE_DOOR2:
			case CITY_BUILD_TYPE_DOOR3:
				pstClanFightInfo->DoorBuild++;
				break;
			case CITY_BUILD_TYPE_CONTROL:
				pstClanFightInfo->ControlBuild++;
				break;
			case CITY_BUILD_TYPE_REVIVE:
				pstClanFightInfo->ReviveBuild++;
				break;
			default:
				break;
		}
	}

	world_city_fight_range_val_cacl(pstClanFightInfo);
	return 0;
}

int world_city_fight_range_svr_info(ZONESVRENV* pstEnv, Player *pstPlayer, int fEnd)
{
	CSCITYCLANFIGHTINFO *pstClanFightInfo;
	int i, j, iIndex;
	AREADYN *pstArea;
	Player *pstTmpPlayer;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	CSPKG stPkg;
	CSCITYSVR *pstCitySvr =&stPkg.Body.CitySvr;
	CSCITYCLANFIGHTRANGE *pstFightRange = &pstCitySvr->Data.ClanFightRange;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;

	stIdx.iID = WORLD_CITY_MAPID;
	stIdx.iPos = -1;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	
	memset(pstFightRange, 0, sizeof(*pstFightRange));

	if(fEnd)
	{
		qsort(&pstWorldCity->ClanRange.ClanList[0], pstWorldCity->ClanRange.Num,
			sizeof(pstWorldCity->ClanRange.ClanList[0]), clan_fight_qsort_cmp);

		// log
		for (i=0; i<pstWorldCity->ClanRange.Num; i++)
		{
			z_city_fini_oplog(pstEnv, &pstWorldCity->ClanRange.ClanList[i], i);
		}
	}
	
	pstFightRange->Num = 0;
	for(i=0; i<pstWorldCity->ClanRange.Num; i++)
	{
		pstClanFightInfo = &pstFightRange->ClanList[pstFightRange->Num];

		pstClanFightInfo->ClanId = pstWorldCity->ClanRange.ClanList[i].ClanId;
		pstClanFightInfo->ReviveBuild = pstWorldCity->ClanRange.ClanList[i].ReviveBuild;
		pstClanFightInfo->ControlBuild = pstWorldCity->ClanRange.ClanList[i].ControlBuild;
		pstClanFightInfo->BuffBuild = pstWorldCity->ClanRange.ClanList[i].BuffBuild;
		pstClanFightInfo->DoorBuild= pstWorldCity->ClanRange.ClanList[i].DoorBuild;
		pstClanFightInfo->DefBuild = pstWorldCity->ClanRange.ClanList[i].DefBuild;
		pstClanFightInfo->Total = pstWorldCity->ClanRange.ClanList[i].Total;
		pstClanFightInfo->Time = pstWorldCity->ClanRange.ClanList[i].Time;
		pstFightRange->Num++;
	}

	pstFightRange->End = fEnd;
	pstCitySvr->Type = CITY_SVR_FIGHT_RANGE;
	Z_CSHEAD_INIT(&stPkg.Head, CITY_SVR);
	
	if(pstFightRange->End)
	{
		//qsort(&pstFightRange->ClanList[0], pstFightRange->Num,
		//	sizeof(pstFightRange->ClanList[0]), clan_fight_qsort_cmp);

		return world_city_clan_mem_award(pstEnv, &stPkg);
	}
	else
	{
		if(pstPlayer)
		{
			pstFightRange->EnterTime = pstPlayer->stRoleData.MiscInfo.CityFightTime;
			return z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		}
		else
		{
			for (i = 0; i < pstMapInst->iAreaDyn; i++) 
			{
				iIndex	=	pstMapInst->aiAreaDyn[i];
				if( iIndex<0)
				{
					continue;
				}

				pstArea	=	(AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iIndex);
				if (NULL == pstArea)
				{
					continue;
				}
				
				for (j = 0; j < pstArea->iAnimate; j++) 
				{
					pstFightRange->EnterTime = 0;
					
					pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstArea->aiAnimate[j]);
					if(NULL == pstTmpPlayer || PLAYER_STATUS_ROLE_LOGIN != pstTmpPlayer->eStatus)
					{
						continue;
					}

					pstFightRange->EnterTime = pstTmpPlayer->stRoleData.MiscInfo.CityFightTime;
					z_cltmsg_send(pstEnv, pstTmpPlayer, &stPkg);
				}

			}
			
			//return map_broadcast(pstEnv->pstAppCtx, pstEnv, pstMapInst, &stPkg);
		}
	}
	
	return 0;
}

int world_city_kill_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSCITYSVR *pstCitySvr =&stPkg.Body.CitySvr;

	pstCitySvr->Data.KillInfo.ContinueKill = pstPlayer->stOnline.stCityFightOL.iContinueKill;
	pstCitySvr->Type = CITY_SVR_KILL_INFO;
	Z_CSHEAD_INIT(&stPkg.Head, CITY_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int world_city_effigy_flower_update(ZONESVRENV* pstEnv)
{
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	WORLDCITYEFFIGY *pstEffigy = &pstWorldCity->CityCore.Effigy;

	if(!IsSameDay(pstEffigy->StartTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstEffigy->YesterdayFlower = pstEffigy->Flower;
		pstEffigy->YesterdayEgg = pstEffigy->Egg;
		pstEffigy->Flower = 0;
		pstEffigy->Egg = 0;
		pstEffigy->StartTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	
	return 0;
}

int world_city_effigy_msg(ZONESVRENV* pstEnv, int fFlower)
{
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	WORLDCITYEFFIGY *pstEffigy = &pstWorldCity->CityCore.Effigy;

	if(fFlower)
	{
		switch(pstEffigy->Flower)
		{
			case 50:
				z_sys_strf_broadcast(pstEnv, SYS_SVR, 
					ZONEERR_BROAD311, pstEffigy->RoleName);
				break;
			case 100:
				z_sys_strf_broadcast(pstEnv, SYS_SVR, 
					ZONEERR_BROAD312, pstEffigy->RoleName);
				break;
			case 200:
				z_sys_strf_broadcast(pstEnv, SYS_SVR, 
					ZONEERR_BROAD313, pstEffigy->RoleName);
				break;
			case 400:
				z_sys_strf_broadcast(pstEnv, SYS_SVR, 
					ZONEERR_BROAD314, pstEffigy->RoleName);
				break;
			case 500:
				z_sys_strf_broadcast(pstEnv, SYS_SVR, 
					ZONEERR_BROAD315, pstEffigy->RoleName);
				break;
			case 1000:
				z_sys_strf_broadcast(pstEnv, SYS_SVR, 
					ZONEERR_BROAD316, pstEffigy->RoleName);
				break;
			case 2000:
				z_sys_strf_broadcast(pstEnv, SYS_SVR, 
					ZONEERR_BROAD317, pstEffigy->RoleName);
				break;
			case 3000:
				z_sys_strf_broadcast(pstEnv, SYS_SVR, 
					ZONEERR_BROAD318, pstEffigy->RoleName);
				break;
		}
	}
	else
	{
		switch(pstEffigy->Egg)
		{
			case 100:
				z_sys_strf_broadcast(pstEnv, SYS_SVR, 
					ZONEERR_BROAD319, pstEffigy->RoleName);
				break;
			case 300:
				z_sys_strf_broadcast(pstEnv, SYS_SVR, 
					ZONEERR_BROAD320, pstEffigy->RoleName);
				break;
			case 800:
				z_sys_strf_broadcast(pstEnv, SYS_SVR, 
					ZONEERR_BROAD321, pstEffigy->RoleName);
				break;
			case 1000:
				z_sys_strf_broadcast(pstEnv, SYS_SVR, 
					ZONEERR_BROAD322, pstEffigy->RoleName);
				break;
			case 2000:
				z_sys_strf_broadcast(pstEnv, SYS_SVR, 
					ZONEERR_BROAD323, pstEffigy->RoleName);
				break;
		}
	}
	
	return 0;
}

int player_worship_effigy_test(ZONESVRENV * pstEnv, Player * pstPlayer, int fFlower)
{
	tdr_longlong ullExp;
	LEVELUP *pstLevelUP;
	CLANMEM *pstMem;
	ZoneClan *pstClan;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;

	if(IsSameDay(pstPlayer->stRoleData.MiscInfo.WorshipInfo.WorshipTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CITY210);
		return -1;
	}

	pstClan = z_id_clan(pstEnv, pstWorldCity->CityCore.ClanId);
	if(!pstClan)
	{	
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CITY211);
		return -1;
	}

	if(pstWorldCity->CityCore.Effigy.RoleName[0] == 0 )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CITY212);
		return -1;
	}

	pstMem = clan_name_mem(pstClan, pstWorldCity->CityCore.Effigy.RoleName, NULL);
	if(!pstMem)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CITY212);
		return -1;
	}

	if(pstMem->Position != CLAN_POS_PRESIDENT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CITY212);
		return -1;
	}

	pstLevelUP = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	if (NULL == pstLevelUP)
	{
		return -1;
	}

	ullExp = pstLevelUP->ContributeExp*(100/10000.0);
	z_player_addexp_test(pstEnv, pstPlayer, &ullExp);
	if(0 >= ullExp)
	{
		return -1;
	}
	
	return 0;
}

//膜拜城主雕像
int player_worship_effigy(ZONESVRENV* pstEnv, Player *pstPlayer, int fFlower)
{
	tdr_longlong ullExp;
	LEVELUP *pstLevelUP;
	CLANMEM *pstMem;
	ZoneClan *pstClan;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	WORLDCITYEFFIGY *pstEffigy = &pstWorldCity->CityCore.Effigy;

	if(IsSameDay(pstPlayer->stRoleData.MiscInfo.WorshipInfo.WorshipTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CITY210);
		return -1;
	}

	pstClan = z_id_clan(pstEnv, pstWorldCity->CityCore.ClanId);
	if(!pstClan)
	{	
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CITY211);
		return -1;
	}

	if(pstWorldCity->CityCore.Effigy.RoleName[0] == 0 )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CITY212);
		return -1;
	}

	pstMem = clan_name_mem(pstClan, pstWorldCity->CityCore.Effigy.RoleName, NULL);
	if(!pstMem)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CITY212);
		return -1;
	}

	if(pstMem->Position != CLAN_POS_PRESIDENT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE,ZONEERR_CITY212);
		return -1;
	}

	pstLevelUP = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	if (NULL == pstLevelUP)
	{
		return -1;
	}

	ullExp = pstLevelUP->ContributeExp*(100/10000.0);
	z_player_addexp_test(pstEnv, pstPlayer, &ullExp);
	if(0 >= ullExp)
	{
		return -1;
	}

	world_city_effigy_flower_update(pstEnv);
	if(fFlower)
	{
		pstEffigy->Flower++;
		pstEffigy->CurrStat = EFFIGY_STAT_FLOWER;

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CITY213);
	}
	else
	{
		pstEffigy->Egg++;
		pstEffigy->CurrStat = EFFIGY_STAT_EGG;

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CITY214);
	}
	
	player_add_exp(pstEnv, pstPlayer, ullExp, 0);

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_WORSHIP_EFFIGY;
	pstActionRes->Data.WorshipEggify.Flower = fFlower;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);

	world_city_effigy_msg(pstEnv, fFlower);

	world_city_info_broadcast(pstEnv);
	
	//TODO:奖励、每日次数
	pstPlayer->stRoleData.MiscInfo.WorshipInfo.WorshipTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	return 0;
}

int player_effigy_award(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iMoney = 0;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	WORLDCITYEFFIGY *pstEffigy = &pstWorldCity->CityCore.Effigy;

	if(!is_world_city_master(pstEnv, pstPlayer))
	{
		return -1;
	}

	if(pstEffigy->RoleName[0] == 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CITY215);
		return -1;
	}

	if(strcmp(pstEffigy->RoleName, pstPlayer->stRoleData.RoleName) != 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CITY216);
		return -1;
	}

	if(IsSameDay(pstEffigy->AwardTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CITY217);
		return -1;
	}

	world_city_effigy_flower_update(pstEnv);
	
	iMoney = (pstEffigy->YesterdayFlower + pstEffigy->YesterdayEgg)*100;

	if(0 > package_add_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, iMoney))
	{
		return -1;
	}

	package_add_money(pstEnv, pstPlayer, ATTR_ID_MONEY, iMoney);
	z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_EFFIGY, 0,  0, 
			iMoney, "雕像");
	
	pstEffigy->AwardTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
		ZONEERR_CITY218, iMoney);
	return 0;
}

