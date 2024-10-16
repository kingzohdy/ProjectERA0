/*
**  @file $RCSfile: zone_machine.c,v $
**  general description of this module
**  $Id: zone_machine.c,v 1.272 2014/06/25 17:36:14 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/06/25 17:36:14 $
**  @version $Revision: 1.272 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_package.h"
#include "zone_clt.h"
#include "zone_db.h"
#include "zone_err.h"
#include "zone_map.h"
#include "zone_player.h"
#include "proto_comm.h"
#include "zone_combat.h"
#include "zone_attr.h"
#include "zone_tmp.h"
#include "zone_status.h"
#include "zone_pet.h"
#include "zone_ai.h"
#include "zone_skill.h"
#include "zone_act_status.h"
#include "zone_machine.h"
#include "zone_clan.h"
#include "zone_clan_advance.h"
#include "zone_machine_view.h"
#include "zone_itemlimit.h"
#include "zone_collect.h"
#include "zone_misc.h"
#include "zone_relation.h"


int player_ride_machine_clear(ZONESVRENV * pstEnv, Player * pstPlayer);

int machine_passenger_chg_attr(ZONESVRENV* pstEnv, Monster *pstMachineMon);

int machine_item_lock(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong ullWid);

int machine_item_release_lock(ZONESVRENV* pstEnv, Player *pstPlayer);

MONMACHINEDEF *z_find_mon_machine_def(ZONESVRENV *pstEnv, int  iMonID)
{
	int iIndex;
	int iEqu;
	MONMACHINEDEF stMachineDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stMachineDef.MonsterID = iMonID;
	iIndex = bsearch_int(&stMachineDef, pstObj->sMonMachineDef, pstObj->iMonMachineDef, MON_MACHINE_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (MONMACHINEDEF *)pstObj->sMonMachineDef[iIndex];
	}
	
	return NULL;	
}

DARTCARDEF * z_find_dartcar_def(ZONESVRENV *pstEnv, int  iLvl)
{
	int iIndex;
	int iEqu;
	DARTCARDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.Level = iLvl;
	iIndex = bsearch_int(&stDef, pstObj->sDartCarDef, pstObj->iDartCarDef, DART_CAR_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (DARTCARDEF *)pstObj->sDartCarDef[iIndex];
	}
	return NULL;
}

int z_sys_strf_machine_broadcast(ZONESVRENV *pstEnv, Monster *pstMachine, int iSysType, char *pszStr, ...)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	char szMsg[SYS_MSG_LEN];
	va_list valist;
	
	va_start( valist, pszStr );
	vsnprintf(szMsg, sizeof(szMsg), pszStr, valist);
	szMsg[SYS_MSG_LEN -1] = 0;
	va_end(valist);

	pstSysMsg->Type = iSysType;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
	STRNCPY(pstSysMsg->Msg, szMsg, sizeof(pstSysMsg->Msg));

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	machine_msg_broadcast_except(pstEnv, pstMachine, &stPkg, 0);

	return 0;
}


int z_sys_strf_machine_broadcast1(ZONESVRENV *pstEnv, Monster *pstMachine, int iSysType, int iErrno, ...)
{
	const char* pszErr;
	va_list marker;
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	
	va_start( marker, iErrno );
	pszErr	=	ZONE_GET_ERR(iErrno, 0);

	pstSysMsg->Type = iSysType;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
		
	if( !pszErr )
	{
		snprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), "SysNo : %d", iErrno );
	}
	else
	{
		vsnprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), pszErr, marker);
	}

	pstSysMsg->Msg[sizeof(pstSysMsg->Msg) - 1] = 0;
	va_end(marker);
	
	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	
	machine_msg_broadcast_except(pstEnv, pstMachine, &stPkg, 0);

	return 0;
}

int get_machine_attr_val(ZONESVRENV* pstEnv,Monster *pstMon, int iType)
{
	int iIdx;
	
	if(!is_mon_machine(pstMon))
	{
		return 0;
	}

	switch(iType)
	{
	case MON_MACHINE_ATTR_ENERGY:
		return this_machine(pstEnv,pstMon) ->stMachineInfo.Energy;
		break;
	case MON_MACHINE_ATTR_ORE:
		return this_machine(pstEnv,pstMon) ->stMachineInfo.Ore;
		break;
	case MON_MACHINE_ATTR_ORE1:
	case MON_MACHINE_ATTR_ORE2:
	case MON_MACHINE_ATTR_ORE3:
	case MON_MACHINE_ATTR_ORE4:
	case MON_MACHINE_ATTR_ORE5:
		iIdx = iType - MON_MACHINE_ATTR_ORE1;
		return this_machine(pstEnv,pstMon) ->stMachineInfo.Ores[iIdx].Ore;
		break;
	default:
		return 0;
		break;
	}
	
	return 0;
}

SKILLDEF * machine_get_normal_skill(ZONESVRENV* pstEnv, Monster *pstMon, int iSkillID, int iIdx)
{
	MonNormalSkill *pstMonNormalSkill;
	struct timeval stDiff;
	long long  llMs;

	if(iIdx < 0 || iIdx >= MAX_NORM_SKILL)
	{
		return NULL;
	}

	pstMonNormalSkill = pstMon->astNormalSkill + iIdx;
	if (iSkillID == pstMonNormalSkill->stSkill.SkillID)
	{
		TV_DIFF(stDiff, pstEnv->pstAppCtx->stCurr, pstMonNormalSkill->stCoolEnd);
		TV_TO_MS(llMs, stDiff);
		if (llMs + CLT_TIME_WAVE < 0)
		{
			return NULL;
		}
	
		return z_find_skill(pstEnv, pstMonNormalSkill->stSkill.SkillID, pstMonNormalSkill->stSkill.SkillLevel,
												&pstMonNormalSkill->iSkillIdx);
	}

	return NULL;
}

SKILLDEF* machine_get_atker_skill(ZONESVRENV* pstEnv, Monster *pstMon, int iSkillID, int iIdx)
{
	MonInsertSkill *pstMonInsertSkill;
	struct timeval stDiff;
	long long  llMs;

	if(iIdx < 0 || iIdx >= MAX_ATKER_SKILL)
	{
		return NULL;
	}

	pstMonInsertSkill = pstMon->astAtkerInsertSkill+ iIdx;
	if (iSkillID == pstMonInsertSkill->stSkill.SkillID)
	{
		TV_DIFF(stDiff, pstEnv->pstAppCtx->stCurr, pstMonInsertSkill->stCoolEnd);
		TV_TO_MS(llMs, stDiff);
		if (llMs + CLT_TIME_WAVE < 0)
		{
			return NULL;
		}
		
		return z_find_skill(pstEnv, pstMonInsertSkill->stSkill.SkillID, pstMonInsertSkill->stSkill.SkillLevel,
												&pstMonInsertSkill->iSkillIdx);
	}

	return NULL;
}

SKILLDEF* machine_get_atkee_skill(ZONESVRENV* pstEnv, Monster *pstMon, int iSkillID, int iIdx)
{
	MonInsertSkill *pstMonInsertSkill;
	struct timeval stDiff;
	long long  llMs;

	if(iIdx < 0 || iIdx >= MAX_ATKEE_SKILL)
	{
		return NULL;
	}

	pstMonInsertSkill = pstMon->astAtkeeInsertSkill + iIdx;
	if (iSkillID == pstMonInsertSkill->stSkill.SkillID)
	{
		TV_DIFF(stDiff, pstEnv->pstAppCtx->stCurr, pstMonInsertSkill->stCoolEnd);
		TV_TO_MS(llMs, stDiff);
		if (llMs + CLT_TIME_WAVE < 0)
		{
			return NULL;
		}
		
		return z_find_skill(pstEnv, pstMonInsertSkill->stSkill.SkillID, pstMonInsertSkill->stSkill.SkillLevel,
												&pstMonInsertSkill->iSkillIdx);
	}

	return NULL;
}

//获得驾驶机械的人
Player * machine_get_driver(ZONESVRENV* pstEnv, Monster *pstMonMachine)
{
	Player *pstPlayer;
	
	if(this_machine(pstEnv,pstMonMachine)->stMachineInfo.PassengerID[0] <= 0)
	{
		return NULL;
	}

	pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, this_machine(pstEnv,pstMonMachine)->stMachineInfo.PassengerID[0]);
	if(pstPlayer)
	{
		return pstPlayer;
	}

	return NULL;
}

int machine_get_free_seatidx(ZONESVRENV* pstEnv,Monster *pstMachineMon)
{
	int i;

	for(i=0; i<MAX_MON_MACHINE_PASSENGER; i++)
	{
		if(i >= this_machine(pstEnv,pstMachineMon)->stMachineInfo.MaxPassenger)
		{
			break;
		}

		if(this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[i] == 0)
		{
			return i;
		}
	}

	return -1;
}

int machine_get_player_seatidx(ZONESVRENV* pstEnv,Monster *pstMachineMon, Player *pstPlayer)
{
	int i;
	
	if(!(is_player_ride_on_machine(pstPlayer)))
	{
		return -1;
	}

	for(i=0; i<MAX_MON_MACHINE_PASSENGER; i++)
	{
		if(i >= this_machine(pstEnv,pstMachineMon)->stMachineInfo.MaxPassenger)
		{
			break;
		}

		if(this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[i] == pstPlayer->iMemID)
		{
			return i;
		}
	}

	return -1;
}

int machine_del_passenger(ZONESVRENV* pstEnv,Monster *pstMachineMon, int iMemID)
{
	int i;
	
	for(i=0; i<MAX_MON_MACHINE_PASSENGER; i++)
	{
		if(i >= this_machine(pstEnv,pstMachineMon)->stMachineInfo.MaxPassenger)
		{
			break;
		}

		if(this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[i] == iMemID)
		{
			this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[i] = 0;
			break;
		}
	}

	return 0;
}

int machine_passenger_chg_seat(ZONESVRENV* pstEnv, Monster *pstMachineMon, Player *pstPlayer, int iSeatIdx)
{
	//停下来在上
	z_player_stop(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, NULL);
	
	//清理技能
	machine_passenger_clear_skill(pstEnv, pstPlayer, pstMachineMon);
	machine_del_passenger(pstEnv,pstMachineMon, pstPlayer->iMemID);
	
	this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[iSeatIdx] = pstPlayer->iMemID;
	//获得技能
	machine_passenger_update_skill(pstEnv, pstPlayer, pstMachineMon);
	return 0;
}

int machine_add_passenger(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMachineMon, 
		int iSeatIdx, tdr_ulonglong ullItemWid)
{
	RESPOS stPos;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	ZONEMAPINST* pstMapInst;
	MapIndex *pstMapIdx;
	ZONEIDX stIdx;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if(NULL == pstMapInst)
	{
		return -1;
	}
		
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}
		
	//停下来在上
	z_player_stop(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, NULL);

	player_ride_on_machine_target_update(pstEnv, pstPlayer, pstMachineMon);

	//隐遁
	player_view_mon_delpress(pstEnv, pstPlayer);

	this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[iSeatIdx] = pstPlayer->iMemID;

	//位置更新
	stPos = pstPlayer->stRoleData.Pos;
	pstPlayer->stRoleData.Pos = pstMachineMon->stCurrPos;
	z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstPlayer->iMemID, &stPos, 0);
	
	pstPlayer->stOnline.State |= CS_STAT_RIDE_MON_MACHINE;
	pstPlayer->stOnline.stMachineInfo.iMachineMonID = pstMachineMon->iID;
	//锁定物品
	machine_item_lock(pstEnv, pstPlayer, ullItemWid);

	z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);

	machine_player_rideon_view(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMachineMon);
	
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_RIDE_ON_MACHINE;
	pstActionRes->Data.RideOnMachine.MachineID = pstMachineMon->iID;
	pstActionRes->Data.RideOnMachine.MachineDefID = pstMachineMon->iDefIdx;
	pstActionRes->Data.RideOnMachine.RolePos = pstPlayer->stRoleData.Pos;
	pstActionRes->Data.RideOnMachine.SeatIdx = iSeatIdx;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_monview(pstEnv->pstAppCtx, pstEnv, pstMachineMon, &stPkg, 0);

	//车被人搞变形
	machine_passenger_chg_attr(pstEnv, pstMachineMon);

	machine_pkval_update(pstEnv, pstMachineMon);

	if(this_machine(pstEnv,pstMachineMon)->stMachineInfo.FromItemWid > 0)
	{
		Player *pstDriver = machine_get_driver(pstEnv, pstMachineMon);
		if(pstDriver && pstDriver != pstPlayer)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstDriver, SYS_MIDDLE2, 
				ZONEERR_MACHINE19, pstPlayer->stRoleData.RoleName);
		}
	}

	//获得技能
	machine_passenger_update_skill(pstEnv, pstPlayer, pstMachineMon);

	machine_ride_on_pktime_update(pstEnv, pstPlayer, pstMachineMon);

	return 0;
}

Monster* machine_get_by_player(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	Monster *pstMachineMon;
	
	if(!is_player_ride_on_machine(pstPlayer))
	{
		return NULL;
	}

	pstMachineMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stMachineInfo.iMachineMonID);
	if(pstMachineMon == NULL)
	{
		return NULL;
	}

	return pstMachineMon;
}

int machine_limit_map_chek(ZONESVRENV* pstEnv, int iMachineID, int iMapID)
{
	MONMACHINEDEF *pstMachineDef;
	
	pstMachineDef = z_find_mon_machine_def(pstEnv, iMachineID);
	if(NULL == pstMachineDef)
	{
		return -1;
	}

	if(pstMachineDef->LimitMapID[0] > 0)
	{
		int i;
		int fInLimit = 0;
		
		for(i=0; i<MAX_MACHINE_LIMIT_MAP; i++)
		{
			if(iMapID == pstMachineDef->LimitMapID[i])
			{
				fInLimit = 1;
			}
		}

		if(!fInLimit)
		{
			return -1;
		}
	}
	
	return 0;
}

//机械切地图
int machine_chg_map(ZONESVRENV * pstEnv, Monster *pstMon, int iMapID, RESPOS *pstPos)
{
	int iRet;
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst = NULL;
	MapIndex *pstMapIdx;
	ZONEPWORLDINST *pstPworldInst;

	//if(pstMon->stMap.iID == iMapID)
	//{
	//	return 0;
	//}
	
	stIdx.iID	 =	iMapID;
	stIdx.iPos =	-1;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		goto _error;
	}
	
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx)
	{
		goto _error;
	}

	if (pstMapIdx->stMapDef.CtrlFlag & MAP_CTRL_NO_RIDE_MACHINE)
	{
		if (pstMon->iPatrolID <= 0)
		{
			goto _error;
		}
	}

	//不是玩家自己的机械不可切地图
	if (pstMon->cDartCarNotAtk  == 0)
	{
		MONMACHINEDEF *pstMachineDef = z_find_mon_machine_def(pstEnv, pstMon->iDefIdx);
		if(!pstMachineDef)
		{
			goto _error;
		}
	
		if(this_machine(pstEnv, pstMon)->stMachineInfo.FromItemWid == 0 &&
			pstMachineDef->LeaveLimit == 0)
		{
			goto _error;
		}
	}
	
	//骑乘机械进副本
	pstPworldInst	= z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if(pstPworldInst)
	{
		ZONEPWORLD *pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
		if (pstZonePworld && 
			(pstZonePworld->stPworldDef.CtrlFlag & PWORLD_NO_RIDE_MACHINE))
		{
			goto _error;
		}
	}

	if(0 > machine_limit_map_chek(pstEnv, pstMon->iDefIdx, iMapID))
	{
		goto _error;
	}
	
	/*
	//方便测试暂时不限制
	if(is_collect_machine(pstEnv, pstMon))
	{
		 if(!(pstMapIdx->stMapDef.CtrlFlag & MAP_CTRL_MON_COLLECT_MACHINE))
		 {
			goto _error;
		 }
	}
	else
	{
		 if(!(pstMapIdx->stMapDef.CtrlFlag & MAP_CTRL_MON_NORMAL_MACHINE))
		 {
			goto _error;
		 }
	}*/

	if (NULL == z_pos_area_dyn(pstMapIdx, pstEnv->pstAreaDynPool, pstMapInst, pstPos))
	{
		goto _error;
	}

	if (0 == z_valid_pos(pstEnv->pstMapObj, iMapID, &pstMapInst->iMapPos, pstPos))
	{
		goto _error;
	}

	z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);

	iRet = z_safe_monster_entermap(pstEnv->pstAppCtx, pstEnv, pstMon, iMapID, pstPos);
	if(iRet < 0)
	{
		goto _error;
	}

	player_mon_pk_mode_action(pstEnv, pstMon);
	return 0;
	
_error:
	
	//清理怪物上的人
	z_ride_machine_clear(pstEnv, pstMon);
	if(this_machine(pstEnv, pstMon)->stMachineInfo.FromItemWid > 0)
	{
		z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
		z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);	
	}
	
	return -1;
}

SKILLDEF *machine_set_atk_skill(ZONESVRENV * pstEnv, Player * pstPlayer, Monster *pstMachineMon, 
	CSSKILLATK *pstSkillAtk)
{
	int iSeatIdx;
	int iMonSkillIdx = 0, iSkillType = 0;
	SKILLDEF *pstSkillDef;
	MonAtkInfo *pstMonAtkInfo = NULL;

	pstMonAtkInfo = &pstMachineMon->stMonAtkInfo;
	
	iSeatIdx = machine_get_player_seatidx(pstEnv,pstMachineMon, pstPlayer);
	if(iSeatIdx < 0)
	{
		return NULL;
	}

	pstSkillDef = NULL;
	switch(iSeatIdx)
	{
		case 0:
			{
				
				pstSkillDef = machine_get_normal_skill(pstEnv, pstMachineMon, pstSkillAtk->SkillID, 0);
				if(pstSkillDef)
				{
					iMonSkillIdx = 0;
					iSkillType = MON_SKILL_TYPE_NORMAL;
					break;
				}

				pstSkillDef = machine_get_normal_skill(pstEnv, pstMachineMon, pstSkillAtk->SkillID, 1);
				if(pstSkillDef)
				{
					iMonSkillIdx = 1;
					iSkillType = MON_SKILL_TYPE_NORMAL;
					break;
				}

				if(is_single_mon_machine(pstEnv, pstMachineMon))
				{
					pstSkillDef = machine_get_normal_skill(pstEnv, pstMachineMon, pstSkillAtk->SkillID, 2);
					if(pstSkillDef)
					{
						iMonSkillIdx = 2;
						iSkillType = MON_SKILL_TYPE_NORMAL;
						break;
					}

					pstSkillDef = machine_get_normal_skill(pstEnv, pstMachineMon, pstSkillAtk->SkillID, 3);
					if(pstSkillDef)
					{
						iMonSkillIdx = 3;
						iSkillType = MON_SKILL_TYPE_NORMAL;
						break;
					}
				}
			}
			break;
		case 1:
			{
				pstSkillDef = machine_get_normal_skill(pstEnv, pstMachineMon, pstSkillAtk->SkillID, 2);
				if(pstSkillDef)
				{
					iMonSkillIdx = 2;
					iSkillType = MON_SKILL_TYPE_NORMAL;
					break;
				}

				pstSkillDef = machine_get_normal_skill(pstEnv, pstMachineMon, pstSkillAtk->SkillID, 3);
				if(pstSkillDef)
				{
					iMonSkillIdx = 3;
					iSkillType = MON_SKILL_TYPE_NORMAL;
					break;
				}
			}
			break;
		case 2:
			{
				pstSkillDef = machine_get_atker_skill(pstEnv, pstMachineMon, pstSkillAtk->SkillID, 0);
				if(pstSkillDef)
				{
					iMonSkillIdx = 0;
					iSkillType = MON_SKILL_TYPE_INSERT;
					break;
				}

				pstSkillDef = machine_get_atker_skill(pstEnv, pstMachineMon, pstSkillAtk->SkillID, 1);
				if(pstSkillDef)
				{
					iMonSkillIdx = 1;
					iSkillType = MON_SKILL_TYPE_INSERT;
					break;
				}
			}
			break;
		case 3:
			{
				pstSkillDef = machine_get_atker_skill(pstEnv, pstMachineMon, pstSkillAtk->SkillID, 2);
				if(pstSkillDef)
				{
					iMonSkillIdx = 2;
					iSkillType = MON_SKILL_TYPE_INSERT;
					break;
				}

				pstSkillDef = machine_get_atker_skill(pstEnv, pstMachineMon, pstSkillAtk->SkillID, 3);
				if(pstSkillDef)
				{
					iMonSkillIdx = 3;
					iSkillType = MON_SKILL_TYPE_INSERT;
					break;
				}
			}
			break;
		case 4:
			{
				pstSkillDef = machine_get_atkee_skill(pstEnv, pstMachineMon, pstSkillAtk->SkillID, 0);
				if(pstSkillDef)
				{
					iMonSkillIdx = 0;
					iSkillType = MON_SKILL_TYPE_ATKEE;
					break;
				}

				pstSkillDef = machine_get_atkee_skill(pstEnv, pstMachineMon, pstSkillAtk->SkillID, 1);
				if(pstSkillDef)
				{
					iMonSkillIdx = 1;
					iSkillType = MON_SKILL_TYPE_ATKEE;
					break;
				}
			}
			break;
		case 5:
			{
				pstSkillDef = machine_get_atkee_skill(pstEnv, pstMachineMon, pstSkillAtk->SkillID, 2);
				if(pstSkillDef)
				{
					iMonSkillIdx = 2;
					iSkillType = MON_SKILL_TYPE_ATKEE;
					break;
				}

				pstSkillDef = machine_get_atkee_skill(pstEnv, pstMachineMon, pstSkillAtk->SkillID, 3);
				if(pstSkillDef)
				{
					iMonSkillIdx = 3;
					iSkillType = MON_SKILL_TYPE_ATKEE;
					break;
				}
			}
			break;
		default:
			return NULL;
			break;

	}

	if(NULL == pstSkillDef)
	{
		return NULL;
	}
	
	pstMonAtkInfo->unSkillID = pstSkillDef->SkillID;
	pstMonAtkInfo->bSkillLevel = pstSkillDef->Level;
	pstMonAtkInfo->iSkillIdx = 0;
	pstMonAtkInfo->bMonSkillIdx = iMonSkillIdx;
	pstMonAtkInfo->bSkillType = iSkillType;
	pstMonAtkInfo->stAtkPos = pstSkillAtk->AtkPos;
	
	return pstSkillDef;
}

int passenger_add_one_skill(ZONESVRENV * pstEnv, Player * pstPlayer, int iSkillID, int iSkillLV, 
	struct timeval *pstCoolEnd)
{
	ZoneSkill *pstZoneSkill;
	SKILLDEF *pstSkillDef;

	pstSkillDef = z_find_skill(pstEnv, iSkillID, iSkillLV, NULL);
	if(!pstSkillDef || pstSkillDef->BindSkill != BINDSKILL_TYPE_MACHINE)
	{
		return -1;
	}
	
	if (0 > z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, iSkillID, iSkillLV, 0))
	{
		return -1;
	}

	pstZoneSkill = z_get_player_zoneskill(pstPlayer, iSkillID);
	if (NULL == pstZoneSkill)
	{
		return -1;
	}

	pstZoneSkill->stSkillData.CoolLeft = 0;
	pstZoneSkill->stSkillData.CollectNum = 0;
	pstZoneSkill->stCoolEnd = *pstCoolEnd;
	if (pstZoneSkill->stCoolEnd.tv_sec > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		struct timeval stTmp;
		long long  llMs=0;
		memset(&stTmp,0,sizeof(stTmp));
		TV_DIFF(stTmp, pstZoneSkill->stCoolEnd, pstEnv->pstAppCtx->stCurr);
		TV_TO_MS(llMs, stTmp);
		
		pstZoneSkill->stSkillData.CoolLeft = llMs/100;
	}
	
	z_skill_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstZoneSkill);

	return 0;
}

int machine_passenger_clear_skill(ZONESVRENV * pstEnv, Player * pstPlayer, Monster *pstMachineMon)
{
	int iSeatIdx;
	
	iSeatIdx = machine_get_player_seatidx(pstEnv,pstMachineMon, pstPlayer);
	if(iSeatIdx < 0)
	{
		return -1;
	}
	
	switch(iSeatIdx)
	{
		case 0:
			{
				MonNormalSkill *pstMonNormalSkill;

				pstMonNormalSkill = &pstMachineMon->astNormalSkill[0];
				z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMonNormalSkill->stSkill.SkillID, 1);
			
				pstMonNormalSkill = &pstMachineMon->astNormalSkill[1];
				z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMonNormalSkill->stSkill.SkillID, 1);

				if(is_single_mon_machine(pstEnv, pstMachineMon))
				{
					pstMonNormalSkill = &pstMachineMon->astNormalSkill[2];
					z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMonNormalSkill->stSkill.SkillID, 1);

					pstMonNormalSkill = &pstMachineMon->astNormalSkill[3];
					z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMonNormalSkill->stSkill.SkillID, 1);
				}
			}
			break;
		case 1:
			{
				MonNormalSkill *pstMonNormalSkill;

				pstMonNormalSkill = &pstMachineMon->astNormalSkill[2];
				z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMonNormalSkill->stSkill.SkillID, 1);

				pstMonNormalSkill = &pstMachineMon->astNormalSkill[3];
				z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMonNormalSkill->stSkill.SkillID, 1);
			}
			break;
		case 2:
			{
				MonInsertSkill *pstMonInsertSkill;

				pstMonInsertSkill = &pstMachineMon->astAtkerInsertSkill[0];
				z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMonInsertSkill->stSkill.SkillID, 1);

				pstMonInsertSkill = &pstMachineMon->astAtkerInsertSkill[1];
				z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMonInsertSkill->stSkill.SkillID, 1);
			}
			break;
		case 3:
			{
				MonInsertSkill *pstMonInsertSkill;

				pstMonInsertSkill = &pstMachineMon->astAtkerInsertSkill[2];
				z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMonInsertSkill->stSkill.SkillID, 1);

				pstMonInsertSkill = &pstMachineMon->astAtkerInsertSkill[3];
				z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMonInsertSkill->stSkill.SkillID, 1);
			}
			break;
		case 4:
			{
				MonInsertSkill *pstMonInsertSkill;

				pstMonInsertSkill = &pstMachineMon->astAtkeeInsertSkill[0];
				z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMonInsertSkill->stSkill.SkillID, 1);

				pstMonInsertSkill = &pstMachineMon->astAtkeeInsertSkill[1];
				z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMonInsertSkill->stSkill.SkillID, 1);
			}
			break;
		case 5:
			{
				MonInsertSkill *pstMonInsertSkill;

				pstMonInsertSkill = &pstMachineMon->astAtkeeInsertSkill[2];
				z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMonInsertSkill->stSkill.SkillID, 1);

				pstMonInsertSkill =& pstMachineMon->astAtkeeInsertSkill[3];
				z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMonInsertSkill->stSkill.SkillID, 1);
			}
			break;
		default:
			return -1;
			break;

	}
	
	return 0;;
}

int machine_passenger_update_skill(ZONESVRENV * pstEnv, Player * pstPlayer, Monster *pstMachineMon)
{
	int iSeatIdx;
	
	iSeatIdx = machine_get_player_seatidx(pstEnv,pstMachineMon, pstPlayer);
	if(iSeatIdx < 0)
	{
		return -1;
	}
	
	switch(iSeatIdx)
	{
		case 0:
			{
				MonNormalSkill *pstMonNormalSkill;

				pstMonNormalSkill = &pstMachineMon->astNormalSkill[0];
				passenger_add_one_skill(pstEnv, pstPlayer, pstMonNormalSkill->stSkill.SkillID, 
					pstMonNormalSkill->stSkill.SkillLevel, &pstMonNormalSkill->stCoolEnd);

				pstMonNormalSkill = &pstMachineMon->astNormalSkill[1];
				passenger_add_one_skill(pstEnv, pstPlayer, pstMonNormalSkill->stSkill.SkillID, 
					pstMonNormalSkill->stSkill.SkillLevel, &pstMonNormalSkill->stCoolEnd);

				if(is_single_mon_machine(pstEnv, pstMachineMon))
				{
					pstMonNormalSkill = &pstMachineMon->astNormalSkill[2];
					passenger_add_one_skill(pstEnv, pstPlayer, pstMonNormalSkill->stSkill.SkillID, 
						pstMonNormalSkill->stSkill.SkillLevel, &pstMonNormalSkill->stCoolEnd);

					pstMonNormalSkill = &pstMachineMon->astNormalSkill[3];
					passenger_add_one_skill(pstEnv, pstPlayer, pstMonNormalSkill->stSkill.SkillID, 
						pstMonNormalSkill->stSkill.SkillLevel, &pstMonNormalSkill->stCoolEnd);
				}
			}
			break;
		case 1:
			{
				MonNormalSkill *pstMonNormalSkill;

				pstMonNormalSkill = &pstMachineMon->astNormalSkill[2];
				passenger_add_one_skill(pstEnv, pstPlayer, pstMonNormalSkill->stSkill.SkillID, 
					pstMonNormalSkill->stSkill.SkillLevel, &pstMonNormalSkill->stCoolEnd);

				pstMonNormalSkill = &pstMachineMon->astNormalSkill[3];
				passenger_add_one_skill(pstEnv, pstPlayer, pstMonNormalSkill->stSkill.SkillID, 
					pstMonNormalSkill->stSkill.SkillLevel, &pstMonNormalSkill->stCoolEnd);
			}
			break;
		case 2:
			{
				MonInsertSkill *pstMonInsertSkill;

				pstMonInsertSkill = &pstMachineMon->astAtkerInsertSkill[0];
				passenger_add_one_skill(pstEnv, pstPlayer, pstMonInsertSkill->stSkill.SkillID, 
					pstMonInsertSkill->stSkill.SkillLevel, &pstMonInsertSkill->stCoolEnd);

				pstMonInsertSkill = &pstMachineMon->astAtkerInsertSkill[1];
				passenger_add_one_skill(pstEnv, pstPlayer, pstMonInsertSkill->stSkill.SkillID, 
					pstMonInsertSkill->stSkill.SkillLevel, &pstMonInsertSkill->stCoolEnd);
			}
			break;
		case 3:
			{
				MonInsertSkill *pstMonInsertSkill;

				pstMonInsertSkill = &pstMachineMon->astAtkerInsertSkill[2];
				passenger_add_one_skill(pstEnv, pstPlayer, pstMonInsertSkill->stSkill.SkillID, 
					pstMonInsertSkill->stSkill.SkillLevel, &pstMonInsertSkill->stCoolEnd);

				pstMonInsertSkill = &pstMachineMon->astAtkerInsertSkill[3];
				passenger_add_one_skill(pstEnv, pstPlayer, pstMonInsertSkill->stSkill.SkillID, 
					pstMonInsertSkill->stSkill.SkillLevel, &pstMonInsertSkill->stCoolEnd);
			}
			break;
		case 4:
			{
				MonInsertSkill *pstMonInsertSkill;

				pstMonInsertSkill = &pstMachineMon->astAtkeeInsertSkill[0];
				passenger_add_one_skill(pstEnv, pstPlayer, pstMonInsertSkill->stSkill.SkillID, 
					pstMonInsertSkill->stSkill.SkillLevel, &pstMonInsertSkill->stCoolEnd);

				pstMonInsertSkill = &pstMachineMon->astAtkeeInsertSkill[1];
				passenger_add_one_skill(pstEnv, pstPlayer, pstMonInsertSkill->stSkill.SkillID, 
					pstMonInsertSkill->stSkill.SkillLevel, &pstMonInsertSkill->stCoolEnd);
			}
			break;
		case 5:
			{
				MonInsertSkill *pstMonInsertSkill;

				pstMonInsertSkill = &pstMachineMon->astAtkeeInsertSkill[2];
				passenger_add_one_skill(pstEnv, pstPlayer, pstMonInsertSkill->stSkill.SkillID, 
					pstMonInsertSkill->stSkill.SkillLevel, &pstMonInsertSkill->stCoolEnd);

				pstMonInsertSkill = &pstMachineMon->astAtkeeInsertSkill[3];
				passenger_add_one_skill(pstEnv, pstPlayer, pstMonInsertSkill->stSkill.SkillID, 
					pstMonInsertSkill->stSkill.SkillLevel, &pstMonInsertSkill->stCoolEnd);
			}
			break;
		default:
			return -1;
			break;

	}
	
	return 0;;
}

int machine_passenger_skill_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i = 0;
	SKILLDEF *pstSkillDef = NULL;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;
	
	for (i=pstZoneSkillList->nSkillNum-1; i>=0; i--)
	{
		pstSkillDef = z_find_skill_zoneskill(pstEnv, &pstZoneSkillList->astZoneSkills[i]);
		if( pstSkillDef && pstSkillDef->BindSkill == BINDSKILL_TYPE_MACHINE)
		{
			if(!is_player_ride_on_machine(pstPlayer))
			{
				z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstSkillDef->SkillID, 1);
			}
		}
	}
	return 0;
}

int machine_get_passenger_info(ZONESVRENV* pstEnv, Monster *pstMachineMon, CSACTIONMACHINEPASSENGERINFO *pstPassengerInfo)
{
	int i;
	Player *pstTmpPlayer;
	ZoneClan *pstClan;
	
	pstPassengerInfo->MaxPassenger = this_machine(pstEnv,pstMachineMon)->stMachineInfo.MaxPassenger;
	
	for(i=0; i<MAX_MON_MACHINE_PASSENGER; i++)
	{
		if(i >= this_machine(pstEnv,pstMachineMon)->stMachineInfo.MaxPassenger)
		{
			break;
		}

		if(this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[i] <=0)
		{
			continue;
		}

		pstPassengerInfo->Passengers[i].PassengerID = this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[i];

		pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[i]);
		if(pstTmpPlayer && PLAYER_STATUS_ROLE_LOGIN == pstTmpPlayer->eStatus)
		{
			pstPassengerInfo->Passengers[i].Level = pstTmpPlayer->stRoleData.Level;
			pstPassengerInfo->Passengers[i].Career = pstTmpPlayer->stRoleData.Career;
			pstPassengerInfo->Passengers[i].Head = pstTmpPlayer->stRoleData.Head;
			pstPassengerInfo->Passengers[i].Hair = pstTmpPlayer->stRoleData.Hair;
			pstPassengerInfo->Passengers[i].Face = pstTmpPlayer->stRoleData.Face;
			pstPassengerInfo->Passengers[i].Gender = pstTmpPlayer->stRoleData.Gender;

			STRNCPY(pstPassengerInfo->Passengers[i].RoleName, 
					pstTmpPlayer->stRoleData.RoleName, CCH(pstPassengerInfo->Passengers[i].RoleName));

			pstClan = player_get_clan(pstEnv, pstTmpPlayer);
			if (pstClan)
			{
				STRNCPY(pstPassengerInfo->Passengers[i].ClanName, 
						pstClan->stClan.ClanProf.Name, CCH(pstPassengerInfo->Passengers[i].RoleName));
			}
		}
	}

	return 0;
}

int machine_passenger_info(ZONESVRENV* pstEnv, Monster *pstMachineMon)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	CSACTIONMACHINEPASSENGERINFO *pstPassengerInfo = &pstActionRes->Data.PassengerInfo;

	pstActionRes->ID = pstMachineMon->iID;
	pstActionRes->ActionID = ACTION_MACHINE_PASSENGER_INFO;

	memset(pstPassengerInfo, 0, sizeof(*pstPassengerInfo));
	machine_get_passenger_info(pstEnv, pstMachineMon, pstPassengerInfo);
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	//machine_msg_broadcast(pstEnv, pstMachineMon, &stPkg);
	z_sendpkg_monview(pstEnv->pstAppCtx, pstEnv, pstMachineMon, &stPkg, 0);

	return 0;
}

int machine_passenger_ui(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMachineMon)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	CSACTIONMACHINEPASSENGERINFO *pstPassengerInfo = &pstActionRes->Data.PassengerInfo;

	pstActionRes->ID = pstMachineMon->iID;
	pstActionRes->ActionID = ACTION_MACHINE_PASSENGER_INFO;

	memset(pstPassengerInfo, 0, sizeof(*pstPassengerInfo));
	machine_get_passenger_info(pstEnv, pstMachineMon, pstPassengerInfo);

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int machine_ride_check(ZONESVRENV* pstEnv,Monster *pstMachine, Player *pstPlayer)
{
	switch(this_machine(pstEnv,pstMachine)->stMachineInfo.OwnerType)
	{
		case MACHINE_OWNER_SELF:
			if(this_machine(pstEnv,pstMachine)->stMachineInfo.OwnerID != pstPlayer->stRoleData.RoleID)
			{
				Player *pstOwner = machine_get_driver(pstEnv, pstMachine);
				if(NULL == pstOwner)
				{
					return -1;
				}

				if(0 == player_team_same(pstEnv, pstPlayer, pstOwner))
				{
					return -1;
				}
			}
			break;
		case MACHINE_OWNER_CLAN:
			if(this_machine(pstEnv,pstMachine)->stMachineInfo.OwnerID != pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID)
			{
				return -1;
			}
			break;
		case MACHINE_OWNER_SPOUSE:
			{
				if (this_machine(pstEnv,pstMachine)->stMachineInfo.OwnerID == pstPlayer->stRoleData.RoleID)
				{
					return 0;	
				}

				if (pstPlayer->stRoleData.Buddy.SpouseNum > 0)
				{
					if (	pstPlayer->stRoleData.Buddy.Spouse[0].RoleID == 
						this_machine(pstEnv,pstMachine)->stMachineInfo.OwnerID)
					{
						return 0;
					}
				}
				return -1;
			}	
		default:
			return 0;
			break;
	}
	return 0;
}

CSATTRIDVAL *player_get_base_attr(Player *pstPlayer, int iAttrID)
{
	int i;
	CSPLAYERATTRVAL *pstAttrList = &pstPlayer->stOnline.stBaseAttrList;
		
	for(i=0; i<pstAttrList->AttrNum; i++)
	{
		if(pstAttrList->AttrList[i].AttrID == iAttrID)
		{
			return &pstAttrList->AttrList[i].Value;
		}
	}
	
	return NULL;
}

int z_get_player_base_hpmax(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	unsigned int iHP;
	HEALCHG *pstHealChg = &pstPlayer->stOnline.stDyn.stHealChg;
	
	iHP = z_dyn_attr_translate(pstEnv, pstPlayer, BASE2ATTR_TYPE_HP);

	iHP += get_player_level_attr(pstEnv, pstPlayer, BASE2ATTR_TYPE_HP);
	
	iHP = (iHP + pstHealChg->iHPMaxBase)*(1+(pstHealChg->nHPMaxMul)/100.0)+pstHealChg->nHPMaxAdd;
	
	if (iHP > MAX_HP)
	{
		iHP = MAX_HP;
	}
	else if (iHP <= 0)
	{
		iHP = 1;
	}
	
	return iHP;
}

int machine_passenger_get_attr(ZONESVRENV* pstEnv, Player *pstPlayer, int iAttrType)
{
	int iValue1 = 0, iValue2 = 0;
	CSATTRIDVAL *pstValue;
	
	switch(iAttrType)
	{
		case MACHINE_DYNATTR_TYPE_ATK:
			pstValue = player_get_base_attr(pstPlayer, ATTR_ID_PHSIC_ATK);
			if(pstValue)
			{
				iValue1 = pstValue->PhsicAtk;
			}

			pstValue = player_get_base_attr(pstPlayer, ATTR_ID_ICE_ATK);
			if(pstValue)
			{
				iValue2 = pstValue->IceAtk;
			}
			break;
		case MACHINE_DYNATTR_TYPE_HP:
			iValue1 = z_get_player_base_hpmax(pstEnv, pstPlayer);
			break;
		case MACHINE_DYNATTR_TYPE_ICEDEF:
			pstValue = player_get_base_attr(pstPlayer, ATTR_ID_ICE_DEF);
			if(pstValue)
			{
				iValue1 = pstValue->IceDef;
			}
			break;
		case MACHINE_DYNATTR_TYPE_PHSICDEF:
			pstValue = player_get_base_attr(pstPlayer, ATTR_ID_PHSIC_DEF);
			if(pstValue)
			{
				iValue1 = pstValue->PhsicDef;
			}
			break;
		default:
			break;
	}
	
	return iValue1 > iValue2 ? iValue1 : iValue2;
}

int machine_cacl_passenger_attrmul(ZONESVRENV* pstEnv, Monster *pstMachineMon, Player *pstPlayer)
{	
	int iVal;
	int iLevel = pstPlayer->stRoleData.Level;
	int iMonLevel = pstMachineMon->stMonDynAttr.Level;
	int iAtk = machine_passenger_get_attr(pstEnv, pstPlayer, MACHINE_DYNATTR_TYPE_ATK);
	int iIceDef = machine_passenger_get_attr(pstEnv, pstPlayer, MACHINE_DYNATTR_TYPE_ICEDEF);
	int iPhsicDef = machine_passenger_get_attr(pstEnv, pstPlayer, MACHINE_DYNATTR_TYPE_PHSICDEF);
	int iHP = machine_passenger_get_attr(pstEnv, pstPlayer, MACHINE_DYNATTR_TYPE_HP);
	
	iVal =  (6*iAtk+2*iIceDef+2*iPhsicDef+iHP)/(iLevel + 10);
	iVal = iVal/1300.0*(1.0 -abs(iLevel -iMonLevel)/2.0/(iLevel + iMonLevel))*80;
	
	return iVal;
}

int machine_cacl_dynattr(ZONESVRENV* pstEnv, Monster *pstMachineMon)
{
	int i;
	Player *pstPlayer;
	MONMACHINEDEF *pstMachineDef;

	pstMachineDef = z_find_mon_machine_def(pstEnv, pstMachineMon->iDefIdx);
	if(!pstMachineDef)
	{
		return -1;
	}

	this_machine(pstEnv, pstMachineMon)->iDynAttrMul = 0;

	if(is_mon_biaoche(pstMachineMon)  || pstMachineDef->LeaveLimit == 1)
	{
		return 0;
	}
	
	for(i=0; i<MAX_MON_MACHINE_PASSENGER; i++)
	{
		if(i >= this_machine(pstEnv,pstMachineMon)->stMachineInfo.MaxPassenger)
		{
			break;
		}

		if(this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[i] <=0)
		{
			continue;
		}

		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[i]);
		if(pstPlayer && PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus)
		{
			this_machine(pstEnv, pstMachineMon)->iDynAttrMul += 
				machine_cacl_passenger_attrmul(pstEnv, pstMachineMon, pstPlayer);
		}
	}

	return 0;
}

//车被人搞变形了
int machine_passenger_chg_attr(ZONESVRENV* pstEnv, Monster *pstMachineMon)
{
	int iChgHP = 0;
	int iMul = 0;
	int iOldMaxHP = pstMachineMon->iMaxHP;
	MONSTERDEF *pstMonDef;

	//计算修正系数
	machine_cacl_dynattr(pstEnv, pstMachineMon);
	
	pstMonDef = z_get_mon_def(pstEnv, pstMachineMon);
	if(NULL == pstMonDef)
	{
		return -1;
	}

	if (pstMonDef->IsJicheng)
	{
		return 0;
	}
	
	iMul = this_machine(pstEnv, pstMachineMon)->iDynAttrMul;
	
	pstMachineMon->iMaxHP = pstMonDef->HP*(1+iMul/100.0);

	if(iOldMaxHP != pstMachineMon->iMaxHP)
	{
		iChgHP = (tdr_ulonglong )pstMachineMon->iHP*100.0/iOldMaxHP;
		iChgHP = (tdr_ulonglong )iChgHP*pstMachineMon->iMaxHP/100.0;

		pstMachineMon->iHP = iChgHP;
		if(pstMachineMon->iHP <=0)
		{
			pstMachineMon->iHP = 1;
		}

		mon_hp_action(pstEnv, pstMachineMon, pstMachineMon->iHP, 0);
	}
		
	return 0;
}

//是不是怪物机械
int is_mon_machine(Monster *pstMachineMon)
{
	if(pstMachineMon->bSubType == MON_SUB_TYPE_MACHINE)
	{
		return 1;
	}
	
	return 0;
}

//是不是镖车
int is_mon_biaoche(Monster *pstMon)
{
	if(!is_mon_machine(pstMon))
	{
		return 0;
	}

	if(pstMon->cDartCarNotAtk > 0)
	{
		return 1;
	}
	
	return 0;
}

int is_single_mon_machine(ZONESVRENV* pstEnv, Monster *pstMachineMon)
{
	if(!is_mon_machine(pstMachineMon))
	{
		return 0;
	}

	//目前仅仅单人
	if(this_machine(pstEnv, pstMachineMon)->stMachineInfo.MaxPassenger != 1)
	{
		return 0;
	}

	return 1;
}

//是不是采集车
int is_collect_machine(ZONESVRENV* pstEnv,Monster *pstMachineMon)
{
	if(pstMachineMon->bSubType != MON_SUB_TYPE_MACHINE)
	{
		return 0;
	}
	
	if(this_machine(pstEnv,pstMachineMon)->stMachineInfo.CollcetVal > 0)
	{
		return 1;
	}
	
	return 0;
}

//座位是否是空着的
int is_seat_empty_machine(ZONESVRENV* pstEnv,Monster *pstMachineMon, int iIdx)
{
	if(iIdx >= MAX_MON_MACHINE_PASSENGER ||
		iIdx >= this_machine(pstEnv,pstMachineMon)->stMachineInfo.MaxPassenger)
	{
		return 1;
	}
	
	if(this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[iIdx] > 0)
	{
		return 0;
	}

	return 1;
}

//是不是空的怪物机械
int  is_empty_machine(ZONESVRENV* pstEnv,Monster *pstMachineMon)
{
	int i;
	
	//目前仅单人乘坐
	if(pstMachineMon->bSubType != MON_SUB_TYPE_MACHINE)
	{
		return 0;
	}

	for(i=0; i<MAX_MON_MACHINE_PASSENGER; i++)
	{
		if(i >= this_machine(pstEnv,pstMachineMon)->stMachineInfo.MaxPassenger)
		{
			break;
		}

		if(this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[i]> 0)
		{
			return 0;
		}
	}

	return 1;
}

int  is_clan_machine(ZONESVRENV* pstEnv,Monster *pstMachineMon)
{
	if(is_mon_machine(pstMachineMon))
	{
		return 0;
	}

	if(this_machine(pstEnv, pstMachineMon)->stMachineInfo.OwnerType != MACHINE_OWNER_CLAN)
	{
		return 0;
	}

	return 1;
}

int is_player_ride_on_machine(Player *pstPlayer)
{
	if(CS_STAT_RIDE_MON_MACHINE & pstPlayer->stOnline.State)
	{
		return 1;
	}

	return 0;
}

int is_player_driver_machine(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	Monster *pstMachine;
	Player *pstDriver;
	
	pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	if(NULL == pstMachine)
	{
		return 0;
	}

	pstDriver = machine_get_driver(pstEnv, pstMachine); 
	if(NULL == pstDriver || pstDriver != pstPlayer)
	{
		return 0;
	}

	return 1;
}

int is_player_normal_passenger_machine(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	Monster *pstMachine;
	Player *pstDriver;
	
	pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	if(NULL == pstMachine)
	{
		return 0;
	}

	pstDriver = machine_get_driver(pstEnv, pstMachine); 
	if(pstDriver && pstDriver == pstPlayer)
	{
		return 0;
	}

	return 1;
}

int is_together_ride_machine(Player *pstPlayer1, Player *pstPlayer2)
{
	if(!is_player_ride_on_machine(pstPlayer1))
	{
		return 0;
	}

	if(!is_player_ride_on_machine(pstPlayer2))
	{
		return 0;
	}

	if(pstPlayer1->stOnline.stMachineInfo.iMachineMonID != pstPlayer2->stOnline.stMachineInfo.iMachineMonID)
	{
		return 0;
	}

	return 1;	
}

int is_player_use_item_machine(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMachine)
{
	if(this_machine(pstEnv, pstMachine)->stMachineInfo.FromItemWid > 0 &&
		this_machine(pstEnv, pstMachine)->stMachineInfo.FromItemWid == pstPlayer->stOnline.stMachineInfo.ullLockItemWID)
	{
		return 1;
	}

	return 0;
}
//进入视野移动通知
int z_passenger_move_notify_one(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstVPlayer)
{	
	int i ;
	unsigned int  iCurrPoint;
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSMOVEOTHER *pstMoveOther = &pstPkg->Body.MoveOther;
	CSWALK *pstWalk = &pstMoveOther->Data.Walk;
	Player *pstRidePlayer;
	Monster *pstMachine;

	pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	if(NULL == pstMachine)
	{
		return 0;
	}

	pstRidePlayer = machine_get_driver(pstEnv, pstMachine); 
	if(NULL == pstRidePlayer || pstRidePlayer == pstPlayer)
	{
		return 0;
	}

	if(!pstRidePlayer->stOnline.cMoving)
	{
		return 0;
	}
	
	pstMoveOther->ID = pstPlayer->iMemID;
	pstMoveOther->x = pstRidePlayer->stRoleData.Pos.X;
	pstMoveOther->y = pstRidePlayer->stRoleData.Pos.Y;
	
	pstMoveOther->MoveType = CS_MOVE_WALK;
	pstWalk->Total = pstRidePlayer->stOnline.cFuturePoints;
	if (pstWalk->Total > CS_MAX_MOVEPOINTS)
	{
		pstWalk->Total = CS_MAX_MOVEPOINTS;
	}
	
	for (i = 0; i < pstWalk->Total; i++) {
		iCurrPoint = (pstRidePlayer->stOnline.cCurrPoint + i) % RECORD_TURN;
		pstWalk->Points[i].x = pstRidePlayer->stOnline.stTurnPoints[iCurrPoint].X;
		pstWalk->Points[i].y = pstRidePlayer->stOnline.stTurnPoints[iCurrPoint].Y;
	}

	memcpy(&pstMoveOther->MspdInfo, &pstRidePlayer->stOnline.stMspdInfo, sizeof(pstMoveOther->MspdInfo));

	Z_CSHEAD_INIT(&pstPkg->Head, MOVE_OTHER);

	z_cltmsg_send(pstEnv, pstVPlayer, pstPkg);
	
	return 0;
}

//进入视野移动通知
int z_machine_move_notify_one(ZONESVRENV* pstEnv, Monster *pstMonMachine, Player *pstVPlayer)
{	
	int i ;
	unsigned int  iCurrPoint;
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSMOVEOTHER *pstMoveOther = &pstPkg->Body.MoveOther;
	CSWALK *pstWalk = &pstMoveOther->Data.Walk;
	Player *pstRidePlayer = machine_get_driver(pstEnv, pstMonMachine); 

	if(NULL == pstRidePlayer || !pstRidePlayer->stOnline.cMoving)
	{
		return 0;
	}
	
	pstMoveOther->ID = pstMonMachine->iID;
	pstMoveOther->x = pstRidePlayer->stRoleData.Pos.X;
	pstMoveOther->y = pstRidePlayer->stRoleData.Pos.Y;
	
	pstMoveOther->MoveType = CS_MOVE_WALK;
	pstWalk->Total = pstRidePlayer->stOnline.cFuturePoints;
	if (pstWalk->Total > CS_MAX_MOVEPOINTS)
	{
		pstWalk->Total = CS_MAX_MOVEPOINTS;
	}
	
	for (i = 0; i < pstWalk->Total; i++) {
		iCurrPoint = (pstRidePlayer->stOnline.cCurrPoint + i) % RECORD_TURN;
		pstWalk->Points[i].x = pstRidePlayer->stOnline.stTurnPoints[iCurrPoint].X;
		pstWalk->Points[i].y = pstRidePlayer->stOnline.stTurnPoints[iCurrPoint].Y;
	}

	memcpy(&pstMoveOther->MspdInfo, &pstRidePlayer->stOnline.stMspdInfo, sizeof(pstMoveOther->MspdInfo));

	Z_CSHEAD_INIT(&pstPkg->Head, MOVE_OTHER);

	z_cltmsg_send(pstEnv, pstVPlayer, pstPkg);
	
	return 0;
}

int z_passenger_update_pos(ZONESVRENV* pstEnv, MapIndex *pstMapIdx, ZONEMAPINST* pstMapInst, 
	Player *pstPlayer, RESPOS *pstPos)
{
	RESPOS stPos, stSearchPos;

	stPos = pstPlayer->stRoleData.Pos;
	pstPlayer->stRoleData.Pos = *pstPos;
	
	z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstPlayer->iMemID, &stPos, 0);
	
	if (pstMapIdx && z_pos_search_pos(pstMapIdx, pstPos, &stSearchPos)) 
	{
		if (!z_same_pos(&stSearchPos, &pstPlayer->stOnline.stLastSearchPos)) 
		{
			z_player_search_view(pstEnv->pstAppCtx, pstEnv,  pstMapInst, &stSearchPos, pstPlayer);
		}
	}

	return 0;
}

int z_machine_update_pos(ZONESVRENV* pstEnv, MapIndex *pstMapIdx, ZONEMAPINST* pstMapInst, 
	Monster *pstMachineMon, RESPOS *pstPos)
{
	int i;
	RESPOS stPos, stSearchPos;
	Player *pstPlayer;
	
	stPos = pstMachineMon->stCurrPos;
	
	pstMachineMon->stCurrPos = *pstPos;
	z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstMachineMon->iID, &stPos, 0);

	TV_CLONE(pstMachineMon->stLastMoveTv, pstEnv->pstAppCtx->stCurr);
	if (pstMapIdx && z_pos_search_pos(pstMapIdx, &pstMachineMon->stCurrPos, &stSearchPos)) 
	{
		if (!z_same_pos(&stSearchPos, &pstMachineMon->stLastSearchPos)) 
		{
			z_mon_search_view(pstEnv->pstAppCtx, pstEnv, pstMapInst, &stSearchPos, pstMachineMon);
		}
	}

	//除了驾驶员之外的乘客
	for(i=1; i<MAX_MON_MACHINE_PASSENGER; i++)
	{
		if(i >= this_machine(pstEnv,pstMachineMon)->stMachineInfo.MaxPassenger)
		{
			break;
		}

		if(this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[i] <= 0)
		{
			continue;	
		}

		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[i]);
		if(pstPlayer && PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus)
		{
			z_passenger_update_pos(pstEnv, pstMapIdx, pstMapInst, pstPlayer, pstPos);
		}
	}

	return 0;
}

//更新怪物位置
int z_mon_machine_move(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	Monster *pstMachineMon;
	ZONEMAPINST* pstMapInst;
	MapIndex *pstMapIdx;
	ZONEIDX stIdx;
	RESPOS stPos;
	
	if(!(CS_STAT_RIDE_MON_MACHINE & pstPlayer->stOnline.State))
	{
		return 0;
	}

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if(!pstMapInst)
		return -1;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx) return -1;
	
	pstMachineMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stMachineInfo.iMachineMonID);
	if(pstMachineMon == NULL)
	{
		return -1;
	}

	stPos = pstPlayer->stRoleData.Pos;
	
	z_machine_update_pos(pstEnv, pstMapIdx, pstMapInst, pstMachineMon, &stPos);

	return 0;
}

//上车
int z_ride_on_machine(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMachineMon, int iSeatIdx, tdr_ulonglong ullItemWid)
{
	if(iSeatIdx < 0 || iSeatIdx >= MAX_MON_MACHINE_PASSENGER)
	{
		return -1;
	}

	/*
	//清除临时道具
	if(pstRoleTmpSys->TmpItemID)
	{
		tmp_item_drop(pstEnv, pstPlayer,1);
	}

	player_drop_clan_tem(pstEnv,pstPlayer);*/

	if (pstPlayer->stOnline.stPetOnline.bFightShiNum > 0)
	{
		if (!is_pet_heti(pstPlayer))
		{
			z_player_backup_pet(pstEnv, pstPlayer);
		}
	}

	//切换位置
	if(machine_get_player_seatidx(pstEnv,pstMachineMon, pstPlayer) >= 0)
	{
		machine_passenger_chg_seat(pstEnv, pstMachineMon, pstPlayer, iSeatIdx);
	}
	//新来的
	else
	{
		if (0 > check_replace_skill(pstEnv, pstPlayer, REPLACE_SKILL_OP_MACHINE))
		{
			return -1;
		}
		machine_add_passenger(pstEnv, pstPlayer, pstMachineMon, iSeatIdx, ullItemWid);
	}

	this_machine(pstEnv,pstMachineMon)->iNoIdle = 1;

	//todo: 通知位置面板变换
	machine_passenger_info(pstEnv, pstMachineMon);

	if (pstMachineMon->cDartCarNotAtk == 1)
	{
		player_pkval_chg_outpress(pstEnv,pstPlayer);
	}
	return 0;
}

int z_ride_machine_die(ZONESVRENV * pstEnv, Monster *pstMachineMon, ZoneAni * pstAtker)
{
	int i;
	Player *pstPlayer;
	MONMACHINEDEF *pstMachineDef;

	pstMachineDef = z_find_mon_machine_def(pstEnv, pstMachineMon->iDefIdx);
	if(!pstMachineDef || !pstMachineDef->PlayerDie)
	{
		z_ride_machine_clear(pstEnv, pstMachineMon);
		return 0;
	}
	
	for(i=0; i<MAX_MON_MACHINE_PASSENGER; i++)
	{
		if(i >= this_machine(pstEnv,pstMachineMon)->stMachineInfo.MaxPassenger)
		{
			break;
		}

		if(this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[i] == 0)
		{
			continue;
		}

		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[i]);
		if(pstPlayer)
		{			
			
			player_ride_machine_clear(pstEnv, pstPlayer);
			player_die(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstAtker, 1);
		}
	}
	
	memset(this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID, 0, sizeof(this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID));
	
	return 0;
}

int z_ride_machine_clear(ZONESVRENV * pstEnv, Monster *pstMachineMon)
{
	int i;
	Player *pstPlayer;
	Player *pstMachinePlayer = NULL;
	MONMACHINEDEF *pstMachineDef;

	for(i=0; i<MAX_MON_MACHINE_PASSENGER; i++)
	{
		if(i >= this_machine(pstEnv,pstMachineMon)->stMachineInfo.MaxPassenger)
		{
			break;
		}

		if(this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[i] == 0)
		{
			continue;
		}

		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[i]);
		if(pstPlayer)
		{			
			//假设是所有者
			if(i == 0)
			{
				pstMachinePlayer = pstPlayer;
			}
			
			player_ride_machine_clear(pstEnv, pstPlayer);
		}
	}
	memset(this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID, 0, sizeof(this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID));

	//车被人搞变形
	machine_passenger_chg_attr(pstEnv, pstMachineMon);

	if(pstMachineMon->cStat != MONSTER_DIE)
	{
		pstMachineDef = z_find_mon_machine_def(pstEnv, pstMachineMon->iDefIdx);
		if(pstMachineDef)
		{
			int i;
			int iType;
			int iNum;

			for(i=0; i<MAX_MON_MACHINE_ORE; i++)
			{
				iType = machine_ore_idx2type(i);
				if(iType < 0)
				{
					break;
				}

				iNum = get_machine_attr_val(pstEnv, pstMachineMon, iType);
				if(iNum <= 0)
				{
					continue;
				}

				if(pstMachinePlayer && pstMachinePlayer->eStatus == PLAYER_STATUS_ROLE_LOGOUT_REQ)
				{
					ROLEGRID stRoleGrid;
					
					iNum = ceil(iNum *0.3);
					if(0 == item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstMachineDef->Ore[i].OreItemID))
					{
						stRoleGrid.GridData.RoleItem.Num = iNum;
						package_add(pstEnv->pstAppCtx, pstEnv, pstMachinePlayer, &stRoleGrid, 1, 0, IMPITEM_OPTYPE_CLAN_RESOURCE_MACHINE);
					} 
				}
				else
				{
					iNum = ceil(iNum *0.5);
					//掉落
					booty_discrete_drop_item(pstEnv, pstMachineMon, 
							pstMachineDef->Ore[i].OreItemID, iNum);
				}
				
			}
		}
	}
	
	return 0;
}

int machine_ore_idx2type(int iIdx)
{
	switch(iIdx)
	{
		case 0:
			return MON_MACHINE_ATTR_ORE1;
			break;
		case 1:
			return MON_MACHINE_ATTR_ORE2;
			break;
		case 2:
			return MON_MACHINE_ATTR_ORE3;
			break;
		case 3:
			return MON_MACHINE_ATTR_ORE4;
			break;
		case 4:
			return MON_MACHINE_ATTR_ORE5;
			break;
		default:
			break;
	}

	return -1;
}

int machine_ores_sum(ZONESVRENV * pstEnv, Monster *pstMon)
{
	int i;
	int iTotal = 0;
	
	for(i=0; i<MAX_MON_MACHINE_ORE; i++)
	{
		iTotal += this_machine(pstEnv, pstMon)->stMachineInfo.Ores[i].Ore;
	}

	return iTotal;
}

int z_machine_attr_chg(ZONESVRENV * pstEnv, Monster *pstMon, int iType, int iVal)
{
	int iCurrVal = 0;
	int iOldVal;
	int iIdx, iTotalOre;
	MONMACHINEINFO *pstMachineInfo;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	if(!is_mon_machine(pstMon) || iVal == 0)
	{
		return 0;
	}

	pstMachineInfo = &this_machine(pstEnv, pstMon)->stMachineInfo;

	switch(iType)
	{
		case MON_MACHINE_ATTR_ENERGY:
			iOldVal = this_machine(pstEnv,pstMon) ->stMachineInfo.Energy;
			
			if (this_machine(pstEnv,pstMon) ->stMachineInfo.Energy + iVal >= this_machine(pstEnv,pstMon) ->stMachineInfo.MaxEnergy)
			{
				this_machine(pstEnv,pstMon) ->stMachineInfo.Energy = this_machine(pstEnv,pstMon) ->stMachineInfo.MaxEnergy;
			}
			else
			{
				this_machine(pstEnv,pstMon) ->stMachineInfo.Energy += iVal;
			}

			if(this_machine(pstEnv,pstMon) ->stMachineInfo.Energy <= 0)
			{
				this_machine(pstEnv,pstMon) ->stMachineInfo.Energy = 0;
				z_mon_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstMon, 1);
				z_sys_strf_machine_broadcast1(pstEnv, pstMon, SYS_MIDDLE2, ZONEERR_RONGYAN15);
			//	z_sys_strf_machine_broadcast(pstEnv, pstMon, SYS_MIDDLE2, "机械能量消耗殆尽，请尽快补充能量");
			}
			else
			{
				if(iOldVal == 0)
				{
					z_mon_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstMon, 1);
				}
			}

			iCurrVal = this_machine(pstEnv,pstMon) ->stMachineInfo.Energy;
			break;
		case MON_MACHINE_ATTR_ORE:
			if (this_machine(pstEnv,pstMon) ->stMachineInfo.Ore + iVal >= this_machine(pstEnv,pstMon) ->stMachineInfo.MaxPackOre)
			{
				this_machine(pstEnv,pstMon) ->stMachineInfo.Ore = this_machine(pstEnv,pstMon) ->stMachineInfo.MaxPackOre;
			}
			else
			{
				this_machine(pstEnv,pstMon) ->stMachineInfo.Ore += iVal;
			}

			if(this_machine(pstEnv,pstMon) ->stMachineInfo.Ore < 0)
			{
				this_machine(pstEnv,pstMon) ->stMachineInfo.Ore = 0;
			}
			
			iCurrVal = this_machine(pstEnv,pstMon) ->stMachineInfo.Ore;
			break;
		case MON_MACHINE_ATTR_ORE1:
		case MON_MACHINE_ATTR_ORE2:
		case MON_MACHINE_ATTR_ORE3:
		case MON_MACHINE_ATTR_ORE4:
		case MON_MACHINE_ATTR_ORE5:
			iIdx = iType - MON_MACHINE_ATTR_ORE1;
			iTotalOre = machine_ores_sum(pstEnv, pstMon);
			if (iTotalOre + iVal >= pstMachineInfo->MaxPackOre)
			{
				pstMachineInfo->Ores[iIdx].Ore += pstMachineInfo->MaxPackOre -iTotalOre;
			}
			else
			{
				pstMachineInfo->Ores[iIdx].Ore += iVal;
			}

			if(pstMachineInfo->Ores[iIdx].Ore < 0)
			{
				pstMachineInfo->Ores[iIdx].Ore = 0;
			}
			
			iCurrVal = pstMachineInfo->Ores[iIdx].Ore;
			break;
		default:
			return -1;
			break;
	}

	pstActionRes->ID = pstMon->iID;
	pstActionRes->ActionID = ACTION_MACHINE_ATTR_CHG;
	pstActionRes->Data.MachineAttrChg.AttrType = iType;
	pstActionRes->Data.MachineAttrChg.CurrVal = iCurrVal;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_monview(pstEnv->pstAppCtx, pstEnv, pstMon, &stPkg, 0);
	
	return 0;
}

//收回机械
int mon_machine_back(ZONESVRENV * pstEnv, Monster *pstMachine)
{
	//人员下车
	z_ride_machine_clear(pstEnv, pstMachine);


	//地图上拿掉
	z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMachine);
	z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMachine);

	return 0;
}



int player_reset_dart_car(ZONESVRENV * pstEnv, Player *pstPlayer)
{
	Monster *pstMon;
	int iLeftTime;
	Npc *pstNpc;
	int iMonMemID = pstPlayer->stRoleData.MiscInfo.DartCarInfo.MonMemID;
	if (iMonMemID <= 0)
	{
		return -1;
	}

	pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, iMonMemID);
	if (!pstMon )
		return -1;

	if (pstMon->cDartCarNotAtk != 2)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE20);
		return -1;
	}

	pstNpc = z_id_npc(pstEnv->pstAppCtx,  pstEnv,pstPlayer->stOnline.iDialogID);
	if (!pstNpc)
	{
		return -1;
	}

	if (z_distance(&pstMon->stCurrPos, &pstNpc->stMon.stCurrPos) > 700)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_MACHINE21);
		return -1;
	}

	iLeftTime = 60*60;
	
	// 消耗道具检查不知道ID, todo
	if (package_dec(pstEnv->pstAppCtx,pstEnv,pstPlayer, RESET_DART_CAR_ITEM, 1, 
		LIST_TYPE_SUNDRIES, NULL, PACKAGE_DEC_NONE, IMPITEM_OPTYPE_DART) < 0)
	{
		ITEMDEF *pstDef = z_find_itemdef(pstEnv,RESET_DART_CAR_ITEM);
		if (pstDef)
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE22, pstDef->Name);
		return -1;
	}

	// 干掉以前的
	z_ride_machine_clear(pstEnv, pstMon);
	z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
	z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);
		
	return new_mon_dart_car_by_task(pstEnv, pstMon, pstPlayer,DART_CAR_REFRESH_TYPE_RAND, iLeftTime);
}


int player_resume_dart_car_in(ZONESVRENV * pstEnv, Player *pstPlayer, int iMonID)
{
	Monster stMon;
	MONSTERDEF *pstMonDef;
	int iType;
	DARTCARDEF *pstDef;
	int iLvl = (pstPlayer->stRoleData.Level -1)/10;
	int i;
	Monster *pstMon;
	ZONEMAPINST *pstMapInst;
	ZONEIDX stIdx;
	RESPOS stPos;
	int iAwardNum = 0;

	stMon.iDefIdx = iMonID;
	stMon.iDefPos = -1;

	pstMonDef = z_get_mon_def( pstEnv, &stMon);
	if (	!pstMonDef || 
		pstMonDef->DartCarType <= DART_CAR_TYPE_NONE ||
		pstMonDef->DartCarType >= DART_CAR_TYPE_POLAN)
	{
		return -1;
	}

	iType = pstMonDef->DartCarType;

	pstDef = z_find_dartcar_def(pstEnv, iLvl);
	if (!pstDef)
		return -1;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = -1;
	stPos = pstPlayer->stRoleData.Pos;
	
	
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	for (i=0; i<pstDef->CarNum; i++)
	{
		stMon.iDefIdx = pstDef->CarOne[i].MonID;
		stMon.iDefPos = -1;
		pstMonDef = z_get_mon_def( pstEnv, &stMon);
		if (	!pstMonDef )
		{
			return -1;
		}

		if (pstMonDef->DartCarType == iType)
		{
			if (!(pstMon = new_mon_dart_car(pstEnv,pstMapInst, iMonID, &stPos, pstPlayer, 60*60)))
			{
				return -1;
			}

			if (pstMonDef->DartCarType == DART_CAR_TYPE_GOLD)
			{
				CSPKG stPkg;
				CSPKG *pstPkg = &stPkg;
				CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;

				pstSysMsg->Type = SYS_TIP;
				pstSysMsg->Times = 0;
				pstSysMsg->Interval = 0;
				pstSysMsg->LinkLen = 0;
				snprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), 
						LJY_SYSMSG5,
						pstPlayer->stRoleData.RoleName);
			
				Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
				z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
				iAwardNum = 4;
			}
			else if (pstMonDef->DartCarType ==DART_CAR_TYPE_PURPLE)
			{
				iAwardNum = 2;
			}
			else if (pstMonDef->DartCarType ==DART_CAR_TYPE_BLUE)
			{
				iAwardNum = 1;
			}
				
			pstPlayer->stRoleData.MiscInfo.DartCarInfo.MonMemID = pstMon->iID;
			pstPlayer->stRoleData.MiscInfo.DartCarInfo.AwardRate = pstDef->CarOne[i].AwardRate;
			pstPlayer->stRoleData.MiscInfo.DartCarInfo.AwardNum = iAwardNum;

			player_ride_machine(pstEnv, pstPlayer, pstMon->iID, 0);
			return pstMon->iID;
	
		}
	}
	
	return -1;
}

// 破烂货车恢复到之前的镖车
int player_resume_dart_car(ZONESVRENV * pstEnv, Player *pstPlayer, int iUseItemNum)
{
	Monster *pstMon;
//	Npc *pstNpc;
	int iMonMemID = pstPlayer->stRoleData.MiscInfo.DartCarInfo.MonMemID;
	if (iMonMemID <= 0)
	{
		return -1;
	}

	pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, iMonMemID);
	if (!pstMon )
		return -1;

	
	if (pstMon->cDartCarNotAtk != 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE20);
		return -1;
	}

	if (package_dec_test(pstEnv->pstAppCtx,pstEnv,pstPlayer, RESET_DART_CAR_ITEM, iUseItemNum, 
		LIST_TYPE_SUNDRIES, NULL, PACKAGE_DEC_NONE) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2,
							ZONEERR_MACHINE23);	
		return -1;
	}
	
	package_dec(pstEnv->pstAppCtx,pstEnv,pstPlayer, RESET_DART_CAR_ITEM, iUseItemNum, 
		LIST_TYPE_SUNDRIES, NULL, PACKAGE_DEC_NONE, IMPITEM_OPTYPE_DART);
	
	// 干掉以前的烂镖车
	z_ride_machine_clear(pstEnv, pstMon);
	z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
	z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);

	return player_resume_dart_car_in(pstEnv, pstPlayer,
			pstPlayer->stRoleData.MiscInfo.DartCarInfo.PreID);
}


// 商运镖车被干死,掉落银币
int dart_car_die_drop_money(ZONESVRENV * pstEnv,Player *pstPlayer)
{
	TASKRUNONE *pstOne;
	TASKDEF *pstTaskDef;
	ZoneBootyGrid stBootyGrid;
	int iDropMoney;
	
	pstOne = player_get_dart_car_task(pstEnv,pstPlayer);
	if (!pstOne)
		return -1;

	pstTaskDef =  task_def_find(pstEnv, pstOne->TaskID, NULL);
	if (!pstTaskDef)
		return -1;

	iDropMoney = pstTaskDef->ConsumeMoney.Val/2;
	if ( iDropMoney <= 0)
		return 0;
		
	memset(&stBootyGrid, 0, sizeof(stBootyGrid));
	stBootyGrid.stGrid.GridData.RoleItem.DefID = BOOTY_MONEY_ID;
	stBootyGrid.stGrid.GridData.RoleItem.Flag = iDropMoney;
	stBootyGrid.bViewType = BOOTY_DROP_VIEW_ALL;
	return dropitem_create(pstEnv, &stBootyGrid, &pstPlayer->stRoleData.Pos, pstPlayer->stRoleData.Map, 0);
}

//被打死了
int mon_machine_die(ZONESVRENV * pstEnv, Monster *pstMachine, ZoneAni * pstAtker)
{
	//ZoneClan *pstZoneClan = mon_owner_clan(pstEnv, pstMachine);
	MONMACHINEDEF *pstMachineDef;
	Player *pstPlayer;
	int iPreID = pstMachine->iDefIdx;
	
	if(!is_mon_machine(pstMachine))
	{
		return 0;
	}

	/*
	//从仓库删除
	if (pstZoneClan)
	{
		clan_machine_on_destroy(pstEnv, pstZoneClan, pstMachine);
	}
	*/

	pstMachineDef = z_find_mon_machine_def(pstEnv, pstMachine->iDefIdx);
	if(pstMachineDef)
	{
		int i;
		int iType;

		for(i=0; i<MAX_MON_MACHINE_ORE; i++)
		{
			iType = machine_ore_idx2type(i);
			if(iType < 0)
			{
				break;
			}
			
			//掉落
			booty_discrete_drop_item(pstEnv, pstMachine, 
					pstMachineDef->Ore[i].OreItemID, get_machine_attr_val(pstEnv,pstMachine, iType));
		}
	}

	z_ride_machine_die(pstEnv, pstMachine, pstAtker);

	// 可以打的商运镖车死了,刷一个破烂镖车
	if (	pstMachine->cDartCarNotAtk == 2 &&
		pstMachine->tLifeEnd > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
								this_machine(pstEnv,pstMachine)->stMachineInfo.OwnerName);
		if (!pstPlayer)
			return 0;

		clan_dart_die(pstEnv, pstPlayer, pstAtker, pstMachine);
		// 掉落银币todo:
		//dart_car_die_drop_money( pstEnv,pstPlayer);

		if (new_mon_dart_car_by_task(pstEnv, pstMachine, pstPlayer, DART_CAR_REFRESH_TYPE_POLAN, 
						pstMachine->tLifeEnd - pstEnv->pstAppCtx->stCurr.tv_sec) >= 0)
		{
			// 记一下镖车被打烂时的ID，做恢复用.
			pstPlayer->stRoleData.MiscInfo.DartCarInfo.PreID = iPreID;	
		}
	}
	return 0;
}

//出生怪物机械属性，所有的new_mon都会调用
int mon_machine_data(ZONESVRENV * pstEnv, Monster *pstMachine)
{
	MONMACHINEDEF *pstMachineDef;
	
	if(!is_mon_machine(pstMachine))
	{
		return 0;
	}
	
	pstMachineDef = z_find_mon_machine_def(pstEnv, pstMachine->iDefIdx);
	if(pstMachineDef)
	{
		//燃料上限
		this_machine(pstEnv,pstMachine)->stMachineInfo.MaxEnergy = pstMachineDef->MaxEnergy;

		//矿石携带上限
		this_machine(pstEnv,pstMachine)->stMachineInfo.MaxPackOre = pstMachineDef->MaxPackOre;

		//采集时间
		this_machine(pstEnv,pstMachine)->stMachineInfo.CollcetTime = pstMachineDef->CollcetTime;

		//一次采集多少个
		this_machine(pstEnv,pstMachine)->stMachineInfo.CollcetVal = pstMachineDef->CollcetVal;

		this_machine(pstEnv,pstMachine)->stMachineInfo.MaxPassenger = pstMachineDef->PassengerNum;
		this_machine(pstEnv,pstMachine)->iNoIdle = 0;
	}

	return 0;
}

static int mon_machine_owner_info(ZONESVRENV * pstEnv, MONSTERDEF *pstMonDef, Monster *pstMon, 
	int iOwnerType, const void *pstOwnerData)
{
	//所有权的信息
	this_machine(pstEnv,pstMon)->stMachineInfo.OwnerType = iOwnerType;
	this_machine(pstEnv,pstMon)->stMachineInfo.OwnerName[0] = 0;
	this_machine(pstEnv,pstMon)->stMachineInfo.OwnerID = 0;
	
	if(iOwnerType == MACHINE_OWNER_CLAN)
	{
		ZoneClan *pstZoneClan = (ZoneClan *)pstOwnerData;
		
		this_machine(pstEnv,pstMon)->stMachineInfo.OwnerID =  pstZoneClan->stClan.ClanProf.GID;
		STRNCPY(this_machine(pstEnv,pstMon)->stMachineInfo.OwnerName, 
			pstMonDef->Name, CCH(this_machine(pstEnv,pstMon)->stMachineInfo.OwnerName));

		mon_set_pkinfo(pstMon, MON_PK_TYPE_CLAN, pstZoneClan->stClan.ClanProf.GID);
		pstMon->nOwnerFastID = pstZoneClan->stClan.ClanProf.Id;
	}
	else if(iOwnerType == MACHINE_OWNER_SELF)
	{
		Player *pstPlayer = (Player *)pstOwnerData;

		this_machine(pstEnv,pstMon)->stMachineInfo.OwnerID = pstPlayer->stRoleData.RoleID;
		STRNCPY(this_machine(pstEnv,pstMon)->stMachineInfo.OwnerName, 
			pstPlayer->stRoleData.RoleName, CCH(this_machine(pstEnv,pstMon)->stMachineInfo.OwnerName));


		mon_set_pkinfo(pstMon, MON_PK_TYPE_PLAYER, pstPlayer->iMemID);

		pstMon->iCampIdx = pstPlayer->stOnline.stWarOL.cCampIdx;
	}
	else if(iOwnerType == MACHINE_OWNER_SPOUSE)
	{
		Player *pstPlayer = (Player *)pstOwnerData;

		this_machine(pstEnv,pstMon)->stMachineInfo.OwnerID = pstPlayer->stRoleData.RoleID;
		STRNCPY(this_machine(pstEnv,pstMon)->stMachineInfo.OwnerName, 
			pstPlayer->stRoleData.RoleName, CCH(this_machine(pstEnv,pstMon)->stMachineInfo.OwnerName));
		mon_set_pkinfo(pstMon, MON_PK_TYPE_PROTECT, 0);
	}
	else
	{
		mon_set_pkinfo(pstMon, MON_PK_TYPE_PROTECT, 0);
	}

	return 0;
}

Monster* new_mon_machine_dir(ZONESVRENV * pstEnv, ZONEMAPINST *pstMapInst, 
	int iMonID, RESPOS *pstPos, int iOwnerType, const void *pstOwnerData, unsigned char bDir, tdr_ulonglong ullWid,
	int iBuildDefID, int iLeftTime)
{
	Monster stMonster;
	MapIndex *pstMapIdx;
	Monster *pstMon;
	MONSTERDEF *pstMonDef;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return NULL;
	}

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.stInitPos = *pstPos;
	stMonster.bDirection = bDir;
	stMonster.stCurrPos = stMonster.stInitPos;
	stMonster.iCityBuildDefID = iBuildDefID;

	pstMonDef = z_get_mon_def(pstEnv, &stMonster);
	if (NULL == pstMonDef || pstMonDef->SubType != MON_SUB_TYPE_MACHINE)
	{
		return NULL;
	}

	//所有权
	mon_machine_owner_info(pstEnv, pstMonDef, &stMonster, iOwnerType, pstOwnerData);

	pstMon =	z_new_monster(pstEnv->pstAppCtx, pstEnv, &stMonster, pstMonDef);
	if( !pstMon )
	{
		return NULL;
	}

	pstMon->iCommType = MON_UNION_TYPE_MACHINE;

	//机械属性
	mon_machine_data(pstEnv, pstMon);
	this_machine(pstEnv, pstMon)->stMachineInfo.FromItemWid = ullWid;

	// 商运镖车属性--start--
	if (iLeftTime)
		pstMon->tLifeEnd = pstEnv->pstAppCtx->stCurr.tv_sec+iLeftTime;

	if (pstMonDef->DartCarType > DART_CAR_TYPE_NONE)
	{
		if (pstMonDef->DartCarType == DART_CAR_TYPE_POLAN)
			pstMon->cDartCarNotAtk = 1; 						// 不可以攻击
		else
			pstMon->cDartCarNotAtk = 2; 
	}
	// 商运镖车属性--end--
	
	pstMon->cFirstBorn = 1;
	if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstMon,NULL))
	{
		return NULL;
	}
	pstMon->cFirstBorn = 0;
		
	return pstMon;
}

//召唤一个机械(空燃料，空弹药。需要填充)
Monster* new_mon_machine(ZONESVRENV * pstEnv, ZONEMAPINST *pstMapInst, 
	int iMonID, RESPOS *pstPos, int iOwnerType, const void *pstOwnerData, tdr_ulonglong ullWid)
{
	unsigned char bDir = RAND1(180);
		
	return new_mon_machine_dir(pstEnv,pstMapInst,iMonID,pstPos,iOwnerType,pstOwnerData,bDir, ullWid, 0,0);
}

Monster* new_mon_machine_dir_patrol(ZONESVRENV * pstEnv, ZONEMAPINST *pstMapInst, 
	int iMonID, RESPOS *pstPos, int iOwnerType, const void *pstOwnerData, unsigned char bDir, tdr_ulonglong ullWid,
	int iBuildDefID, int iLeftTime, int iPatrol)
{
	Monster stMonster;
	MapIndex *pstMapIdx;
	Monster *pstMon;
	MONSTERDEF *pstMonDef;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return NULL;
	}

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.stInitPos = *pstPos;
	stMonster.bDirection = bDir;
	stMonster.stCurrPos = stMonster.stInitPos;
	stMonster.iCityBuildDefID = iBuildDefID;

	pstMonDef = z_get_mon_def(pstEnv, &stMonster);
	if (NULL == pstMonDef || pstMonDef->SubType != MON_SUB_TYPE_MACHINE)
	{
		return NULL;
	}

	//所有权
	mon_machine_owner_info(pstEnv, pstMonDef, &stMonster, iOwnerType, pstOwnerData);

	pstMon =	z_new_monster(pstEnv->pstAppCtx, pstEnv, &stMonster, pstMonDef);
	if( !pstMon )
	{
		return NULL;
	}

	pstMon->iCommType = MON_UNION_TYPE_MACHINE;

	//机械属性
	mon_machine_data(pstEnv, pstMon);
	this_machine(pstEnv, pstMon)->stMachineInfo.FromItemWid = ullWid;

	//modify lzk  start
	z_machine_attr_chg(pstEnv, pstMon, MON_MACHINE_ATTR_ENERGY, this_machine(pstEnv, pstMon)->stMachineInfo.MaxEnergy);
	//end

	// 商运镖车属性--start--
	if (iLeftTime)
		pstMon->tLifeEnd = pstEnv->pstAppCtx->stCurr.tv_sec+iLeftTime;

	if (pstMonDef->DartCarType > DART_CAR_TYPE_NONE)
	{
		if (pstMonDef->DartCarType == DART_CAR_TYPE_POLAN)
			pstMon->cDartCarNotAtk = 1; 						// 不可以攻击
		else
			pstMon->cDartCarNotAtk = 2; 
	}
	// 商运镖车属性--end--
	
	pstMon->cFirstBorn = 1;
	pstMon->iPatrolID = iPatrol;
	pstMon->bMoveType = MOVE_TYPE_PATROL;
	if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstMon,NULL))
	{
		return NULL;
	}
	pstMon->cFirstBorn = 0;
		
	return pstMon;
}


// 刷一个商运镖车
Monster* new_mon_dart_car(ZONESVRENV * pstEnv, ZONEMAPINST *pstMapInst, 
								int iMonID, RESPOS *pstPos, Player *pstPlayer,int iLeftTime)
{

	
	unsigned char bDir = RAND1(180);
	Monster *pstMon;
	//MONSTERDEF *pstMonDef;
	MONMACHINEDEF *pstMachineDef;
	
	pstMon = new_mon_machine_dir(pstEnv,pstMapInst,iMonID,pstPos,
									MACHINE_OWNER_SELF, (void *)pstPlayer,bDir, 0, 0, iLeftTime);

	if (NULL == pstMon)
	{
		return NULL;
	}
	
	pstMachineDef = z_find_mon_machine_def(pstEnv, pstMon->iDefIdx);

	if (pstMachineDef)
	{
		z_machine_attr_chg(pstEnv, pstMon , MON_MACHINE_ATTR_ENERGY, pstMachineDef->MaxEnergy);
	}
	
	return pstMon;
}

CARONE *z_find_carone1(DARTCARDEF *pstDef)
{
	int i;
	int iCountRate = 0;
	int iRate;

	if (pstDef->CarNum > MAX_DART_CAR)
		return NULL;

	for (i=0; i<pstDef->CarNum; i++)
	{
		iCountRate += pstDef->CarOne[i].GetTaskCreateRate;
	}

	if (iCountRate <= 0)
		return NULL;
	
	iRate = RAND1(iCountRate); 

	iCountRate = 0;
	for (i=0; i<pstDef->CarNum; i++)
	{
		iCountRate += pstDef->CarOne[i].GetTaskCreateRate;
		if (iRate < iCountRate)
			return &pstDef->CarOne[i];
	}
	return NULL;
}

CARONE *z_find_carone(DARTCARDEF *pstDef)
{
	int i;
	int iCountRate = 0;
	int iRate;

	if (pstDef->CarNum > MAX_DART_CAR)
		return NULL;

	for (i=0; i<pstDef->CarNum; i++)
	{
		iCountRate += pstDef->CarOne[i].CreateRate;
	}

	if (iCountRate <= 0)
		return NULL;
	
	iRate = RAND1(iCountRate); 

	iCountRate = 0;
	for (i=0; i<pstDef->CarNum; i++)
	{
		iCountRate += pstDef->CarOne[i].CreateRate;
		if (iRate < iCountRate)
			return &pstDef->CarOne[i];
	}
	
	return NULL;
}

Monster* mon_biaoche_get_by_player(ZONESVRENV * pstEnv,Player *pstPlayer)
{
	Monster *pstMon;	
	
	if (pstPlayer->stRoleData.MiscInfo.DartCarInfo.MonMemID <= 0)
	{
		return NULL;
	}

	pstMon = z_id_monster(pstEnv->pstAppCtx,pstEnv, pstPlayer->stRoleData.MiscInfo.DartCarInfo.MonMemID);
	if (!pstMon || !is_mon_biaoche(pstMon))
	{
		return NULL;
	}

	if(this_machine(pstEnv, pstMon)->stMachineInfo.OwnerType != MACHINE_OWNER_SELF ||
		this_machine(pstEnv, pstMon)->stMachineInfo.OwnerID != pstPlayer->stRoleData.RoleID)
	{
		return NULL;
	}

	return pstMon;
}

int player_is_in_dart_car(ZONESVRENV * pstEnv,Player *pstPlayer)
{
	Monster *pstMon;
	
	if (!is_player_ride_on_machine(pstPlayer))
	{
		return 0;
	}

	pstMon = machine_get_by_player(pstEnv, pstPlayer);
	if(!pstMon || !is_mon_biaoche(pstMon))
	{
		return 0;
	}
	
	return 1;
}

int player_dartcar_dayinfo_update(ZONESVRENV * pstEnv, Player *pstPlayer)
{
	DARTCARINFO *pstDartInfo = &pstPlayer->stRoleData.MiscInfo.DartCarInfo;
	
	if(IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstDartInfo->DayTime))
	{
		return 0;
	}

	pstDartInfo->DayTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstDartInfo->CostNum = 0;
	pstDartInfo->DayCostNum = 0;
	pstDartInfo->RandCostNum = 0;
	return 0;
}

int get_rand_costnum(int iCostVal, int iMinMul, int iMaxMul)
{
	int iMinVal, iMaxVal;
	int iVal;

	iMinVal = iCostVal*(100+iMinMul)/100.0;
	iMaxVal = iCostVal*(100+iMaxMul)/100.0;

	if (iMaxVal > iMinVal)
	{
		iVal = iMinVal + RAND1(iMaxVal-iMinVal+1);
	}
	else
	{
		iVal = iMaxVal + RAND1(iMinVal-iMaxVal+1);
	}

	return iVal;
}

//消耗一个换车合同获得镖车
CARONE * player_dart_car_rand_get(ZONESVRENV * pstEnv, Player *pstPlayer, DARTCARDEF *pstDef)
{
	CARONE *pstOne = NULL;
	DARTCARINFO *pstDartInfo = &pstPlayer->stRoleData.MiscInfo.DartCarInfo;
	
	player_dartcar_dayinfo_update(pstEnv, pstPlayer);

	if(pstDef->CostNum > 0 && pstDartInfo->RandCostNum == 0)
	{
		pstDartInfo->RandCostNum = get_rand_costnum(pstDef->CostNum, -30, 30);
	}

	if(pstDef->MaxCostNum > 0 &&
		pstDartInfo->DayCostNum >= pstDef->MaxCostNum)
	{
		pstDartInfo->DayCostNum = 0;
		pstOne = &pstDef->CarOne[4];
		goto _final;
	}

	if(pstDartInfo->RandCostNum > 0 &&
		pstDartInfo->CostNum >= pstDartInfo->RandCostNum)
	{
		pstDartInfo->CostNum = 0;
		if(RAND1(100) < pstDef->MaxDartRate)
		{
			pstOne = &pstDef->CarOne[4];
			goto _final;
		}
		else
		{
			pstOne = &pstDef->CarOne[3];
			goto _final;
		}
	}
	
	pstOne = z_find_carone(pstDef);

_final:

	pstDartInfo->CostNum++;
	pstDartInfo->DayCostNum++;
	
	return pstOne;
}

// iType等于DART_CAR_REFRESH_TYPE
int new_mon_dart_car_by_task(ZONESVRENV * pstEnv, Monster *pstMachine, Player *pstPlayer, int iType, int iLeftTime)
{
	DARTCARDEF *pstDef;
	int iLvl = (pstPlayer->stRoleData.Level -1)/10;
	CARONE *pstOne;
	ZONEMAPINST *pstMapInst;
	ZONEIDX stIdx;
	int iMonID;
	Monster *pstMon;
	MONSTERDEF *pstMonDef;
	int iRate = 0;
	int i;
	RESPOS stPos;
	int iAwardNum = 0;

	if(pstMachine)
	{
		stIdx = pstMachine->stMap;
		stPos = pstMachine->stCurrPos;
	}
	else
	{
		stIdx.iID = pstPlayer->stRoleData.Map;
		stIdx.iPos = -1;
		stPos = pstPlayer->stRoleData.Pos;
	}
	
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstDef = z_find_dartcar_def(pstEnv, iLvl);
	if (!pstDef)
		return -1;

	iMonID = DART_CAR_POLAN;
	if (iType != DART_CAR_REFRESH_TYPE_POLAN)
	{
		
		if (iType == DART_CAR_REFRESH_TYPE_NORMAL)
		{
			iMonID = pstDef->NewID;
			for (i=0; i<pstDef->CarNum; i++)
			{
				if (pstDef->CarOne[i].MonID == iMonID)
				{
					iRate = pstDef->CarOne[i].AwardRate;
					break;
				}
			}
		}
		else if (iType == DART_CAR_REFRESH_TYPE_GETTASK_RAND)
		{
			pstOne = z_find_carone1(pstDef);
			if (!pstOne)
				return -1;
			iMonID = pstOne->MonID;
			iRate = pstOne->AwardRate;
		}
		else
		{
			pstOne = player_dart_car_rand_get(pstEnv, pstPlayer, pstDef);
			if (!pstOne)
				return -1;
			iMonID = pstOne->MonID;
			iRate = pstOne->AwardRate;
		}
	}
	else
	{
		iRate = pstPlayer->stRoleData.MiscInfo.DartCarInfo.AwardRate*0.6;
	}

	if (iMonID <= 0 || iRate <= 0)
		return -1;
	
	if (!(pstMon = new_mon_dart_car(pstEnv,pstMapInst, iMonID, &stPos, pstPlayer, iLeftTime)))
	{
		return -1;
	}

	if (iType != DART_CAR_REFRESH_TYPE_POLAN)	// 随机车
	{
		pstMonDef = z_get_mon_def( pstEnv,  pstMon);
		if(pstMonDef  )
		{
			if (pstMonDef->DartCarType == DART_CAR_TYPE_GOLD)
			{
				CSPKG stPkg;
				CSPKG *pstPkg = &stPkg;
				CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;

				pstSysMsg->Type = SYS_SVR;
				pstSysMsg->Times = 0;
				pstSysMsg->Interval = 0;
				pstSysMsg->LinkLen = 0;
				snprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), 
						LJY_SYSMSG6,
						pstPlayer->stRoleData.RoleName);
			
				Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
				z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);

				iAwardNum = 4;
			}
			else if (pstMonDef->DartCarType ==DART_CAR_TYPE_PURPLE)
			{
				iAwardNum = 2;
			}
			else if (pstMonDef->DartCarType ==DART_CAR_TYPE_BLUE)
			{
				iAwardNum = 1;
			}
		}
	}
		
	// 记一下商运镖车的memID和倍率
	pstPlayer->stRoleData.MiscInfo.DartCarInfo.MonMemID = pstMon->iID;
	pstPlayer->stRoleData.MiscInfo.DartCarInfo.AwardRate = iRate;
	pstPlayer->stRoleData.MiscInfo.DartCarInfo.AwardNum = iAwardNum;

	if (iType != DART_CAR_REFRESH_TYPE_POLAN)
		player_ride_machine(pstEnv, pstPlayer, pstMon->iID, 0);
	
	return pstMon->iID;
}


/*
int player_machine_passenger_info(ZONESVRENV* pstEnv, Monster *pstMachineMon, Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstActionRes->ID = pstMachineMon->iID;
	pstActionRes->ActionID = ACTION_MACHINE_PASSENGER_INFO;
	pstActionRes->Data.PassengerInfo.MaxPassenger = this_machine(pstEnv,pstMachineMon)->stMachineInfo.MaxPassenger;

	memcpy(pstActionRes->Data.PassengerInfo.PassengerID, this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID, 
		sizeof(pstActionRes->Data.PassengerInfo.PassengerID));

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}
*/

int player_ride_machine(ZONESVRENV* pstEnv, Player *pstPlayer,  int iMachineID, int iSeatIdx)
{	
	int iChg = 0;
	Monster *pstMachineMon;
	MapIndex *pstMapIdx;

	if(iSeatIdx < 0 || iSeatIdx >= MAX_MON_MACHINE_PASSENGER)
	{
		return -1;
	}

	if (pstPlayer->stOnline.State &  CS_STAT_DEAD)
	{
		return -1;
	}

	if(is_in_qiecuo(pstEnv, pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_MACHINE1);
		return -1;
	}

	/*
	if(pstPetOnline->bFightShiNum > 0)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, "宠物出征状态下,不可乘坐机械");
		return -1;
	}*/
	
	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOSKILL))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_MACHINE1);
		return -1;
	}

	if (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_MAP_BROADCAST)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE2);
		return -1;
	}
	
	if (0 == z_player_act_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, RES_ACTION_RIDE_MACHINE))
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, pstPlayer->stRoleData.Map);
	if (!pstMapIdx)
	{
		return -1;
	}
	
	if (pstMapIdx->stMapDef.CtrlFlag & MAP_CTRL_NO_RIDE_MACHINE)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE3);
		return -1;
	}

	if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE) //在副本中
	{
		ZONEPWORLD *pstPworld;
		ZONEIDX stIdx;

		stIdx.iID = GET_PWORLDID(pstPlayer->stRoleData.Map);
		stIdx.iPos = -1;
		
		pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);	
		if (pstPworld && 
			(pstPworld->stPworldDef.CtrlFlag & PWORLD_NO_RIDE_MACHINE))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE3);
			return -1;
		}
	}

	if(is_player_ride_on_machine(pstPlayer) )
	{
		if(pstPlayer->stOnline.stMachineInfo.iMachineMonID != iMachineID)
		{
			return -1;
		}
		else
		{
			iChg = 1;
		}
	}

	pstMachineMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, iMachineID);
	if(pstMachineMon == NULL || pstMachineMon->cStat == MONSTER_DIE)
	{
		return -1;
	}

	if(0 > machine_limit_map_chek(pstEnv, pstMachineMon->iDefIdx, GET_MAPID(pstPlayer->stRoleData.Map)))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE4);
		return -1;
	}
	
	//位置检查
	if(!is_seat_empty_machine(pstEnv,pstMachineMon, iSeatIdx))
	{
		machine_passenger_ui(pstEnv, pstPlayer, pstMachineMon);

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_MACHINE5);
		return -1;
	}

	if(iChg == 0)
	{
		if(0 > machine_ride_check(pstEnv,pstMachineMon, pstPlayer))
		{
			return -1;
		}
		
		if (pstPlayer->stOnline.cMoving)
		{
			z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
		}

		//距离
		if(z_distance(&pstMachineMon->stCurrPos, &pstPlayer->stRoleData.Pos) > 500)
		{
			return -1;
		}
	}
	else
	{
		//物品召唤的不可以换座位
		if(this_machine(pstEnv, pstMachineMon)->stMachineInfo.FromItemWid > 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_MACHINE6);
			return -1;
		}
	}

	return z_ride_on_machine(pstEnv, pstPlayer, pstMachineMon, iSeatIdx, 0);
}

int player_ride_machine_clear(ZONESVRENV * pstEnv, Player * pstPlayer)
{
	Monster *pstMachineMon;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	
	//停下来
	z_player_stop(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, NULL);

	player_world_city_debuff_clear(pstEnv,pstPlayer);

	if(pstPlayer->stOnline.stMachineInfo.ullLockItemWID > 0)
	{	
		machine_item_update(pstEnv, pstPlayer);
	}

	//解锁物品
	machine_item_release_lock(pstEnv, pstPlayer);
	
	pstPlayer->stOnline.State &= ~CS_STAT_RIDE_MON_MACHINE;
	z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);

	pstMachineMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stMachineInfo.iMachineMonID);
	if(NULL == pstMachineMon)
	{
		return -1;
	}
	
	//驾驶员
	if(machine_get_driver(pstEnv, pstMachineMon) == pstPlayer)
	{
		//更新下怪物位置
		z_mon_stop(pstEnv, pstMachineMon);
	}
	
	//清理乘客
	machine_del_passenger(pstEnv,pstMachineMon, pstPlayer->iMemID);

	machine_passenger_skill_check(pstEnv, pstPlayer);

	//车被人搞变形
	machine_passenger_chg_attr(pstEnv, pstMachineMon);
	machine_pkval_update(pstEnv, pstMachineMon);

	//面板更新
	machine_passenger_info(pstEnv, pstMachineMon);

	player_ride_off_machine_target_update(pstEnv, pstPlayer, pstMachineMon);

	machine_player_rideoff_view(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMachineMon);
	
	memset(&pstPlayer->stOnline.stMachineInfo, 0, sizeof(pstPlayer->stOnline.stMachineInfo));

	z_player_restore_pet_nowmap(pstEnv, pstPlayer);

	//更新乘客列表
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_LEAVE_MACHINE;
	pstActionRes->Data.LeaveMachine.RolePos = pstPlayer->stRoleData.Pos;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_monview(pstEnv->pstAppCtx, pstEnv, pstMachineMon, &stPkg, 0);
	
	//且地图的时候怪物视野里已经没有自己了
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int player_machine_leave_pre_brk(ZONESVRENV * pstEnv, Player * pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstPlayer->stOnline.stMachineInfo.tLeavePreTime = 0;
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_MACHINE_LEAVE_PRE_BRK;
	pstActionRes->Data.MachineLeaveBrk.MachineID = 0;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int player_machine_leave_pre(ZONESVRENV * pstEnv, Player * pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	Monster *pstMachine;
	
	if(!is_player_ride_on_machine(pstPlayer))
	{
		return player_machine_leave_pre_brk(pstEnv, pstPlayer);
	}

	pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	if(NULL == pstMachine)
	{
		return player_machine_leave_pre_brk(pstEnv, pstPlayer);
	}
	
	pstPlayer->stOnline.stMachineInfo.tLeavePreTime = pstEnv->pstAppCtx->stCurr.tv_sec + MACHINE_LEAVE_PRE_TIME;
	
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_MACHINE_LEAVE_PRE;
	pstActionRes->Data.MachineLeavePre.MachineID = pstMachine->iID;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}
int player_ride_machine_brk(ZONESVRENV * pstEnv, Player * pstPlayer, int iMust)
{
	int i;
	Player *pstTmp;
	MONSTERDEF *pstMonDef;
	Monster *pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	if(NULL == pstMachine)
	{
		return 0;
	}

	if(is_player_use_item_machine(pstEnv, pstPlayer, pstMachine))
	{
		if(iMust == 0)
		{
			if(pstPlayer->stOnline.stMachineInfo.tLeavePreTime == 0 ||
				pstPlayer->stOnline.stMachineInfo.tLeavePreTime > pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				return player_machine_leave_pre_brk(pstEnv, pstPlayer);
			}
		}

		pstPlayer->stOnline.stMachineInfo.tLeavePreTime = 0;
		//清理人员
		z_ride_machine_clear(pstEnv, pstMachine);
			
		//地图上拿掉
		z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMachine);
		z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMachine);	
	}
	else
	{
		MONMACHINEDEF *pstMachineDef = z_find_mon_machine_def(pstEnv, pstMachine->iDefIdx);

		
		
		player_ride_machine_clear(pstEnv, pstPlayer);

		if(pstMachineDef && pstMachineDef->LeaveLimit)
		{
			if (	MACHINE_OWNER_SPOUSE == pstMachine->stCommUnion.stMachine.stMachineInfo.OwnerType)
			{
				pstMonDef = z_get_mon_def( pstEnv,  pstMachine);
				if (!pstMonDef)
					return -1;
			
				if (this_machine(pstEnv,pstMachine)->stMachineInfo.PassengerID[0] == 0)
				{
					for(i=1; i<MAX_MON_MACHINE_PASSENGER; i++)
					{
						if(i >= this_machine(pstEnv,pstMachine)->stMachineInfo.MaxPassenger)
						{
							break;
						}

						if(this_machine(pstEnv,pstMachine)->stMachineInfo.PassengerID[i] > 0)
						{
							pstTmp = z_id_player(pstEnv->pstAppCtx, pstEnv,
										this_machine(pstEnv,pstMachine)->stMachineInfo.PassengerID[i] );
							if (!pstTmp  || PLAYER_STATUS_ROLE_LOGIN != pstTmp->eStatus)
							{
								continue;
							}
							this_machine(pstEnv,pstMachine)->stMachineInfo.PassengerID[0] = 
								this_machine(pstEnv,pstMachine)->stMachineInfo.PassengerID[i];
							this_machine(pstEnv,pstMachine)->stMachineInfo.PassengerID[i] = 0;

							mon_machine_owner_info(pstEnv, pstMonDef, pstMachine, MACHINE_OWNER_SPOUSE, pstTmp);

							machine_passenger_info(pstEnv, pstMachine);

							pstPlayer->stOnline.stMachineInfo.iMachineMonID = pstMachine->iID;
							break;
						}
					}
				}
				return 0;
			}
			//清理人员
			z_ride_machine_clear(pstEnv, pstMachine);
			
			//地图上拿掉
			z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMachine);
			z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMachine);
		}
	}
	
	return 0;
}


int action_ballistic_notify(ZONESVRENV * pstEnv, Player * pstPlayer, Monster *pstMon, CSSKILLATK *pstSkillAtk)
{
	MonAtkInfo *pstMonAtkInfo;
	pstMonAtkInfo = &pstMon->stMonAtkInfo;
	
	CSPKG *pstPkgRes = &pstEnv->stCSRes;
	CSACTIONRES *pstActionRes = &pstPkgRes->Body.ActionRes;

	pstActionRes->ActionID = ACTION_BALLISTIC_ATK;
	pstActionRes->ID = pstMon->iID;
	pstActionRes->Data.BallisticAtk.SkillID = pstMonAtkInfo->unSkillID;
	pstActionRes->Data.BallisticAtk.Level =  pstMonAtkInfo->bSkillLevel;
	pstActionRes->Data.BallisticAtk.RuneTransferedID = pstMonAtkInfo->unSkillID;
	pstActionRes->Data.BallisticAtk.SkillSeq =pstSkillAtk->SkillSeq;
	pstActionRes->Data.BallisticAtk.AtkPos = pstMonAtkInfo->stAtkPos;
	pstActionRes->Data.BallisticAtk.TargetNum = pstSkillAtk->TargetNum;

	memcpy(&pstActionRes->Data.BallisticAtk.TargetIDs[0], &pstSkillAtk->TargetIDs[0], 
		sizeof(pstSkillAtk->TargetIDs[0])*pstSkillAtk->TargetNum);
	pstActionRes->Data.BallisticAtk.SelfPosChgFlag = 0;

	Z_CSHEAD_INIT(&pstPkgRes->Head, ACTION_RES);
	//z_send_player_hit_res(pstEnv->pstAppCtx, pstEnv, pstPlayer, &pstMonAtkInfo->stAtkPos, pstPkgRes);
	z_sendpkg_mon_area_view(pstEnv->pstAppCtx, pstEnv, pstMon, pstPkgRes);
	return 0;
}

//乘车攻击
int player_machine_atk(ZONESVRENV * pstEnv, Player * pstPlayer, CSSKILLATK *pstSkillAtk)
{
	struct timeval stDiff;
	struct timeval stCltTime;
	//long long  llMs;
	SKILLDEF *pstSkillDef;
	Monster *pstMachineMon;
	MonAtkInfo *pstMonAtkInfo;
	int iUseEnergy;
	ZoneObjList stObjList;
	MONSTERDEF *pstMonDef;
	unsigned short SkillID;

	memset(&stObjList,0,sizeof(stObjList));
	memset(&stDiff,0,sizeof(stDiff));
	if(!(CS_STAT_RIDE_MON_MACHINE & pstPlayer->stOnline.State))
	{
		return 0;
	}

	pstPlayer->stOnline.State &= ~CS_STAT_PREPARE;
	pstPlayer->stOnline.State &= ~CS_STAT_MOVE_PREPARE;
	
	stCltTime.tv_sec = pstSkillAtk->SvrTime.BenchTimeSec;
       stCltTime.tv_usec = pstSkillAtk->SvrTime.BenchTimeUSec;
	
	pstMachineMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stMachineInfo.iMachineMonID);
	if(NULL == pstMachineMon)
	{
		return -1;
	}
	pstMonDef = z_get_mon_def(pstEnv, pstMachineMon);
	if (!pstMonDef)
	{
		return -1;
	}

	pstMonAtkInfo = &pstMachineMon->stMonAtkInfo;

	//这里检查并且设置了怪物释放技能的信息
	pstSkillDef = machine_set_atk_skill(pstEnv, pstPlayer, pstMachineMon, pstSkillAtk);
	if (NULL == pstSkillDef)
	{
		return -1;
	}

	if(pstSkillDef->JiqiChgFlag == 2 &&
		pstPlayer->stOnline.cSecenOverLay > 0)
	{
		return -1;
	}

	if(pstPlayer->stRoleData.Level < pstSkillDef->Learn.LevelMin)
	{
		return -1;
	}

	iUseEnergy = pstSkillDef->EnergyUse;
	if(iUseEnergy > get_machine_attr_val(pstEnv,pstMachineMon, MON_MACHINE_ATTR_ENERGY))
	{
		return -1;
	}

	if(pstSkillDef->UseItemID > 0)
	{
		if(0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstSkillDef->UseItemID, 1, -1, NULL,PACKAGE_DEC_NONE))
		{
			return -1;
		}
	}

	//角色坐标的处理
	if (0 > z_trust_clt_pos(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstSkillAtk->RolePos.X, pstSkillAtk->RolePos.Y, &stCltTime, 1))
	{
		//z_player_stop(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stCltTime);
		//return -1;

		tlog_info(pstEnv->pstLogCat, 0, 0, "check combat pos player uin %d name %s svrpos(%d,%d), atk pos(%d, %d)", 
			pstPlayer->stRoleData.Uin, 	pstPlayer->stRoleData.RoleName, pstPlayer->stRoleData.Pos.X,
			pstPlayer->stRoleData.Pos.Y, pstSkillAtk->RolePos.X, pstSkillAtk->RolePos.Y);

		pstSkillAtk->RolePos = pstPlayer->stRoleData.Pos;
	}

	//cd
	mon_skill_begin_cool(pstEnv, pstMachineMon);

	if(pstSkillDef->JiqiChgFlag == 2)
	{
		if (pstSkillDef->AtkType != SKILL_ATK_TYPE_POS)
		{
			//目标默认自己
			if(0>z_mon_get_atk_list_by_atkpos(pstEnv->pstAppCtx, pstEnv, pstMachineMon,pstSkillDef,&pstMonAtkInfo->stAtkPos,&stObjList))
			{
				return -1;
			}
		}
	}
	else if (0 > z_player_atklist(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstSkillDef,
		pstSkillAtk, &stObjList, &stCltTime))
	{
		return -1;
	}

	if(pstSkillDef->UseItemID > 0)
	{
		if(0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstSkillDef->UseItemID, 1, -1, 
			NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_SKILL))
		{
			return -1;
		}
	}

	pstPlayer->stOnline.stAtkInfo.iSkillSeq = pstSkillAtk->SkillSeq;
	SkillID = pstSkillDef->SkillID;

	if (pstSkillDef->JiqiChgFlag == 2 && pstSkillDef->AtkType == SKILL_ATK_TYPE_POS)
	{
		pstMonAtkInfo->iSkillSeq = pstSkillAtk->SkillSeq;
		action_ballistic_notify(pstEnv, pstPlayer, pstMachineMon, pstSkillAtk);
	}
	else
	{
		pstMachineMon->iSkillAtkerID = pstPlayer->iMemID;
		mon_atk_end(pstEnv, pstMachineMon, pstSkillDef, &stObjList);
		
		pstMachineMon->iSkillAtkerID = 0;
	}
	
	z_machine_attr_chg(pstEnv, pstMachineMon, MON_MACHINE_ATTR_ENERGY, -iUseEnergy);

	wedding_machine_skill_effect(pstEnv, pstPlayer,pstMachineMon,SkillID);
	
	return 0;
}

int player_machine_ballistic_hit(ZONESVRENV * pstEnv, Player * pstPlayer, CSSKILLATK *pstSkillAtk)
{
	SKILLDEF *pstSkillDef;
	Monster *pstMachineMon;
	MonAtkInfo *pstMonAtkInfo;
	ZoneObjList stObjList;
	MONSTERDEF *pstMonDef;

	pstMachineMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stMachineInfo.iMachineMonID);
	if(NULL == pstMachineMon)
	{
		return -1;
	}
	pstMonDef = z_get_mon_def(pstEnv, pstMachineMon);
	if (!pstMonDef)
	{
		return -1;
	}

	pstMonAtkInfo = &pstMachineMon->stMonAtkInfo;

	//这里检查并且设置了怪物释放技能的信息
	pstSkillDef = machine_set_atk_skill(pstEnv, pstPlayer, pstMachineMon, pstSkillAtk);
	if (NULL == pstSkillDef)
	{
		return -1;
	}

	if (!(pstSkillDef->JiqiChgFlag == 2 && pstSkillDef->AtkType == SKILL_ATK_TYPE_POS))
	{
		return -1;
	}

	if (pstSkillAtk->SkillSeq == 0 || pstSkillAtk->SkillSeq != pstMonAtkInfo->iSkillSeq)
	{
		return -1;
	}

	if(0>z_mon_get_atk_list_by_atkpos(pstEnv->pstAppCtx, pstEnv, pstMachineMon,pstSkillDef,&pstMonAtkInfo->stAtkPos,&stObjList))
	{
		return -1;
	}

	pstMachineMon->iSkillAtkerID = pstPlayer->iMemID;
	mon_atk_end(pstEnv, pstMachineMon, pstSkillDef, &stObjList);
		
	pstMachineMon->iSkillAtkerID = 0;
	
	return 0;
}

int player_new_machine_and_ride(ZONESVRENV *pstEnv, Player *pstPlayer,
                                int iMonMachineID, int iEnergy, RESPOS *pstPos)
{
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	Monster *pstMachine;
	MapIndex *pstMapIdx;

	if(is_player_ride_on_machine(pstPlayer))
	{
		return -1;
	}

	if (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_MAP_BROADCAST)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE2);
		return -1;
	}

	if(is_in_qiecuo(pstEnv, pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,ZONEERR_MACHINE1 );
		return -1;
	}

	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOSKILL))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_MACHINE7);
		return -1;
	}
	
	if (0 == z_player_act_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, RES_ACTION_RIDE_MACHINE))
	{
		return -1;
	}

	if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE) //在副本中
	{
		ZONEPWORLD *pstPworld;
		ZONEIDX stIdx;

		stIdx.iID = GET_PWORLDID(pstPlayer->stRoleData.Map);
		stIdx.iPos = -1;
		
		pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);	
		if (pstPworld && 
			(pstPworld->stPworldDef.CtrlFlag & PWORLD_NO_RIDE_MACHINE))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE4);
			return -1;
		}
	}

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx)
	{
		return -1;
	}
	
	if (pstMapIdx->stMapDef.CtrlFlag & MAP_CTRL_NO_RIDE_MACHINE)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE3);
		return -1;
	}

	//产出一个空燃料空弹药的机械
	pstMachine = new_mon_machine(pstEnv, pstMapInst, iMonMachineID, pstPos, MACHINE_OWNER_SPOUSE, pstPlayer, 0);
	if(pstMachine == NULL)
	{
		return -1;
	}

	//燃料
	z_machine_attr_chg(pstEnv, pstMachine, MON_MACHINE_ATTR_ENERGY, iEnergy);

	//乘坐
	z_ride_on_machine(pstEnv, pstPlayer, pstMachine, machine_get_free_seatidx(pstEnv,pstMachine), 0);
	
	return pstMachine->iID;
}

/*
int player_machine_back(ZONESVRENV* pstEnv, Player *pstPlayer,  int iMachineID)
{
	Monster *pstMachineMon;
	ZoneClan *pstZoneClan;

	pstMachineMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, iMachineID);
	if(pstMachineMon == NULL)
	{
		return -1;
	}

	//暂时单人
	if(!is_empty_machine(pstMachineMon))
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, "机械上有人,不可回收");
		return -1;
	}
	
	pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	if (NULL == pstZoneClan || pstZoneClan->stClan.ClanProf.GID != pstMachineMon->stMachineInfo.OwnerID)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, "你无权回收此机械");
		return -1;
	}

	//公会仓库状态更新(保存血量、燃料值)
	clan_machine_on_callback(pstEnv, pstZoneClan, pstMachineMon);

	//地图上拿掉
	z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMachineMon);
	z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMachineMon);

	return 0;
}*/

//踢人
int player_machine_kick(ZONESVRENV* pstEnv, Player *pstPlayer,  int iPassengerID)
{
	Player *pstKickPlayer;
	Monster *pstMachineMon;
	
	if(!is_player_ride_on_machine(pstPlayer) ||
		pstPlayer->stOnline.stMachineInfo.ullLockItemWID == 0)
	{
		return -1;
	}

	pstMachineMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stMachineInfo.iMachineMonID);
	if(pstMachineMon == NULL)
	{
		return -1;
	}
	
	if (pstPlayer != machine_get_driver(pstEnv, pstMachineMon))
	{
		return -1;
	}

	pstKickPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, iPassengerID);
	if(NULL == pstKickPlayer)
	{
		return -1;
	}

	if(0 > machine_get_player_seatidx(pstEnv,pstMachineMon, pstKickPlayer))
	{
		return -1;
	}
	
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstKickPlayer, SYS_MIDDLE2, 
		ZONEERR_MACHINE24, pstPlayer->stRoleData.RoleName);

	return player_ride_machine_brk(pstEnv, pstKickPlayer, 0);
}

int player_machine_passenger_ui(ZONESVRENV* pstEnv, Player *pstPlayer,  int iMachineID)
{
	Monster *pstMachineMon;
	
	pstMachineMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, iMachineID);
	if(pstMachineMon == NULL)
	{
		return -1;
	}

	if(!is_mon_machine(pstMachineMon))
	{
		return -1;
	}

	machine_passenger_ui(pstEnv, pstPlayer, pstMachineMon);
	
	return 0;
}

int player_machine_invite_accpet(ZONESVRENV* pstEnv, Player *pstPlayer, CSMACHINEINVITEACCPET *pstInviteAccpet)
{
	int iSeatIdx;
	Player *pstRidePlayer;
	Monster *pstMachine;

	pstRidePlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstInviteAccpet->MachineOwnerID);
	if (pstRidePlayer == NULL || pstRidePlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		//对方不在线
		return -1;
	}

	if(!is_player_driver_machine(pstEnv, pstRidePlayer))
	{
		return -1;
	}

	//不在一个地图
	if (pstRidePlayer->stRoleData.Map != pstPlayer->stRoleData.Map)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE8);
		return -1;
	}

	//已经搭车的人不管
	if(is_player_ride_on_machine(pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_MACHINE9);
		return -1;
	}

	pstMachine = machine_get_by_player(pstEnv, pstRidePlayer);
	if(NULL == pstMachine)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_MACHINE10);
		return -1;
	}
	
	iSeatIdx = machine_get_free_seatidx(pstEnv,pstMachine);
	//满人
	if(iSeatIdx < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE11);
		return -1;
	}

	if (pstRidePlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstRidePlayer, 0);
	}
	
	//上马距离
	if(z_distance(&pstRidePlayer->stRoleData.Pos, &pstPlayer->stRoleData.Pos) > MAX_HITCH_RIDE_AREA)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE8);
		return -1;
	}

	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOSKILL))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_MACHINE1);
		return -1;
	}

	if (0 == z_player_act_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, RES_ACTION_RIDE_MACHINE))
	{
		return -1;
	}

	z_ride_on_machine(pstEnv, pstPlayer, pstMachine, iSeatIdx, 0);

	return 0;
}


int player_machine_invite(ZONESVRENV* pstEnv, Player *pstPlayer, CSMACHINEINVITE *pstInvite)
{
	Player *pstTmpPlayer;
	Monster *pstMachine;
	CSPKG stPkg;
	CSMACHINESVR *pstMachineSvr = &stPkg.Body.MachineSvr;

	//自己都没上马
	if(!is_player_driver_machine(pstEnv, pstPlayer) ||
		pstPlayer->stOnline.stMachineInfo.ullLockItemWID == 0)
	{
		return -1;
	}

	pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	if(NULL == pstMachine)
	{
		return -1;
	}

	//单人的邀请不了别人
	if(this_machine(pstEnv, pstMachine)->stMachineInfo.MaxPassenger <= 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE12);
		return -1;
	}

	if(0 > machine_get_free_seatidx(pstEnv, pstMachine))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE13);
		return -1;
	}

	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	}

	pstTmpPlayer  = z_id_player(pstEnv->pstAppCtx, pstEnv, pstInvite->MachineInviteID);
	if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		//对方不在线
		return -1;
	}

	//已经搭车的人不管
	if(is_player_ride_on_machine(pstTmpPlayer))
	{
		return -1;
	}
	
	if (pstTmpPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstTmpPlayer, 0);
	}

	if (pstTmpPlayer->stRoleData.Map != pstPlayer->stRoleData.Map)
	{
		//不在一个地图
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE14);
		return -1;
	}

	//距离太远邀请不了
	if(z_distance(&pstTmpPlayer->stRoleData.Pos, &pstPlayer->stRoleData.Pos) > MAX_HITCH_RIDE_AREA)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE14);
		return -1;
	}

	pstMachineSvr->SvrOp = MACHINE_SVR_INVITE_INFO;
	pstMachineSvr->Data.Invite.MachineInviteID = pstPlayer->iMemID;
	STRNCPY(pstMachineSvr->Data.Invite.RoleName, pstPlayer->stRoleData.RoleName, CCH(pstMachineSvr->Data.Invite.RoleName));

	Z_CSHEAD_INIT(&stPkg.Head, MACHINE_SVR);
	z_cltmsg_send(pstEnv, pstTmpPlayer, &stPkg);
	
	return 0;
}

int player_machine_invite_refuse(ZONESVRENV* pstEnv, Player *pstPlayer, CSMACHINEINVITEREFUSE *pstInviteRefuse)
{
	Player *pstTmpPlayer;

	pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstInviteRefuse->MachineOwnerID);
	if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		//对方不在线
		return -1;
	}

	if (pstInviteRefuse->RefuseType == MACHINE_REFUSE_CANCEL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTmpPlayer, SYS_MIDDLE2, 
							ZONEERR_MACHINE15, pstPlayer->stRoleData.RoleName);
	}
	else
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTmpPlayer, SYS_MIDDLE2, 
							ZONEERR_MACHINE16, pstPlayer->stRoleData.RoleName);
	}
	
	return 0;
}

int player_machine_clt_leave(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	Player *pstDriver;
	MONMACHINEDEF *pstMachineDef;
	Monster *pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	
	if(!pstMachine)
	{
		return 0;
	}

	pstMachineDef = z_find_mon_machine_def(pstEnv, pstMachine->iDefIdx);
	if(!pstMachineDef || pstMachineDef->LeaveLimit)
	{
		return 0;
	}

	if(is_player_use_item_machine(pstEnv, pstPlayer, pstMachine))
	{
		if(pstPlayer->stOnline.stMachineInfo.tLeavePreTime == 0 ||
				pstPlayer->stOnline.stMachineInfo.tLeavePreTime > pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			return player_machine_leave_pre_brk(pstEnv, pstPlayer);
		}

		pstPlayer->stOnline.stMachineInfo.tLeavePreTime = 0;
		//清理人员
		z_ride_machine_clear(pstEnv, pstMachine);
			
		//地图上拿掉
		z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMachine);
		z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMachine);	
	}
	else
	{
		player_ride_machine_clear(pstEnv, pstPlayer);
		
		pstDriver = machine_get_driver(pstEnv, pstMachine);
		if(pstDriver && pstDriver != pstPlayer)
		{
			if(this_machine(pstEnv, pstMachine)->stMachineInfo.FromItemWid > 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstDriver, SYS_MIDDLE2, 
					ZONEERR_MACHINE25, pstPlayer->stRoleData.RoleName);
			}
		}
	}
	
	return 0;
}

int player_machine_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
			TFRAMEHEAD	 *pstFrameHead, CSPKG *pstCsPkg)
{	
	CSRIDEMACHINECLT *pstClt = &pstCsPkg->Body.RideMachineClt;
	UNUSED(pstFrameHead);
	UNUSED(pstAppCtx);

	switch(pstClt->CltOp)
	{
		case RIDE_MACHINE_CLT_RIDE:
			return player_ride_machine(pstEnv, pstPlayer, pstClt->Data.Ride.MachineID, pstClt->Data.Ride.SitIdx);
			break;
		case RIDE_MACHINE_CLT_LEAVE_PRE:
			return player_machine_leave_pre(pstEnv, pstPlayer);
			break;
		case RIDE_MACHINE_CLT_LEAVE_PRE_BRK:
			return player_machine_leave_pre_brk(pstEnv, pstPlayer);
			break;
		case RIDE_MACHINE_CLT_LEAVE:
			return player_machine_clt_leave(pstEnv, pstPlayer);
			break;
		case RIDE_MACHINE_CLT_KICK:
			return player_machine_kick(pstEnv, pstPlayer, pstClt->Data.Kick.KickID);
			break;
		case RIDE_MACHINE_CLT_PASSENGER_UI:
			return player_machine_passenger_ui(pstEnv, pstPlayer, pstClt->Data.Passenger.MachineID);
			break;
		case RIDE_MACHINE_CLT_INVITE:
			return player_machine_invite(pstEnv, pstPlayer, &pstClt->Data.Invite);
			break;
		case RIDE_MACHINE_CLT_INVITE_ACCPET:
			return player_machine_invite_accpet(pstEnv, pstPlayer, &pstClt->Data.InviteAccpet);
			break;
		case RIDE_MACHINE_CLT_INVITE_REFUSE:
			return player_machine_invite_refuse(pstEnv, pstPlayer, &pstClt->Data.InviteRefuse);
			break;
		default:
			return -1;
			break;
	}

	return 0;
}

int machine_set_fight(ZONESVRENV* pstEnv, Monster *pstMachineMon)
{
	int i;
	Player *pstPlayer;
	
	for(i=0; i<MAX_MON_MACHINE_PASSENGER; i++)
	{
		if(i >= this_machine(pstEnv,pstMachineMon)->stMachineInfo.MaxPassenger)
		{
			break;
		}

		if(this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[i] <=0)
		{
			continue;
		}

		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[i]);
		if(pstPlayer && PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus)
		{
			SET_PLAYER_FIGHT(pstEnv, pstPlayer, pstEnv->pstAppCtx->stCurr.tv_sec, FIGHT_STAT_DUR);
		}
	}

	return 0;
}

int package_use_machine_item_check(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEITEM *pstRoleItem,
	ITEMDEF *pstItemDef, RESULT *pstResult)
{
	MONSTERDEF *pstMonDef;
	Monster stMon;
	MapIndex *pstMapIdx;

	if(pstRoleItem->InstVal1 <= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_MACHINE26);
		return -1;
	}

	if(is_in_qiecuo(pstEnv, pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_MACHINE17);
		return -1;
	}

	/*
	if(pstPetOnline->bFightShiNum > 0)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, "宠物出征状态下,不可乘坐机械");
		return -1;
	}*/
	
	//可叠加的物品不可以用做怪物机械物品
	if(pstItemDef->MaxPile > 1)
	{
		return -1;
	}

	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOSKILL))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_MACHINE18);
		return -1;
	}
	
	if (0 == z_player_act_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, RES_ACTION_RIDE_MACHINE))
	{
		return -1;
	}

	if (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_MAP_BROADCAST)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE2);
		return -1;
	}

	if(is_player_ride_on_machine(pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE2);
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, pstPlayer->stRoleData.Map);
	if (!pstMapIdx)
	{
		return -1;
	}
	
	if (pstMapIdx->stMapDef.CtrlFlag & MAP_CTRL_NO_RIDE_MACHINE)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE3);
		return -1;
	}

	if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE) //在副本中
	{
		ZONEPWORLD *pstPworld;
		ZONEIDX stIdx;

		stIdx.iID = GET_PWORLDID(pstPlayer->stRoleData.Map);
		stIdx.iPos = -1;
		
		pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);	
		if (pstPworld && 
			(pstPworld->stPworldDef.CtrlFlag & PWORLD_NO_RIDE_MACHINE))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE3);
			return -1;
		}
	}

	stMon.iDefIdx = pstResult->ResultVal2;
	stMon.iDefPos = -1;
	pstMonDef = z_get_mon_def(pstEnv, &stMon);
	if(NULL == pstMonDef)
	{
		return -1;
	}

	if(0 > machine_limit_map_chek(pstEnv, pstResult->ResultVal2, GET_MAPID(pstPlayer->stRoleData.Map)))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE4);
		return -1;
	}

	if (0 > check_replace_skill(pstEnv, pstPlayer, REPLACE_SKILL_OP_MACHINE))
	{
		return -1;
	}

	return 0;
}

int package_use_machine_item(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, CSPACKAGEUSE *pstUse)
{
	int iPos;
	ITEMDEF *pstItemDef;
	ROLEITEM *pstRoleItem;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	Monster *pstMachine;
	RESPOS stPos;
	ZONEIDX stIdx;

	if (0 > package_get_item(pstEnv, pstPlayer, pstUse->SrcListType, 
							pstUse->SrcGridIdx, &pstItemDef, &pstRoleItem, &iPos))
	{
		return -1;
	}

	if(pstRoleItem->InstVal1 <= 0)
	{
		return -1;
	}

	if (0 > check_replace_skill(pstEnv, pstPlayer, REPLACE_SKILL_OP_MACHINE))
	{
		return -1;
	}

	if(is_player_ride_on_machine(pstPlayer))
	{
		return -1;
	}

	if(is_in_qiecuo(pstEnv, pstPlayer))
	{
		return -1;
	}

	if (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_MAP_BROADCAST)
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, pstPlayer->stRoleData.Map);
	if (!pstMapIdx)
	{
		return -1;
	}
	
	if (pstMapIdx->stMapDef.CtrlFlag & MAP_CTRL_NO_RIDE_MACHINE)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE3);
		return -1;
	}

	if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE) //在副本中
	{
		ZONEPWORLD *pstPworld;
		ZONEIDX stIdx;

		stIdx.iID = GET_PWORLDID(pstPlayer->stRoleData.Map);
		stIdx.iPos = -1;
		
		pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);	
		if (pstPworld && 
			(pstPworld->stPworldDef.CtrlFlag & PWORLD_NO_RIDE_MACHINE))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE3);
			return -1;
		}
	}

	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOSKILL))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_MACHINE7);
		return -1;
	}
	
	if (0 == z_player_act_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, RES_ACTION_RIDE_MACHINE))
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

	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	}
	stPos = pstPlayer->stRoleData.Pos;

	//产出一个空燃料空弹药的机械
	pstMachine = new_mon_machine(pstEnv, pstMapInst, pstResult->ResultVal2, &stPos, MACHINE_OWNER_SELF, pstPlayer, pstRoleItem->WID);

	if(pstMachine == NULL)
	{
		return -1;
	}

	//HP
	if(pstMachine->iHP != pstRoleItem->InstVal1)
	{
		pstMachine->iHP = pstRoleItem->InstVal1;
		mon_hp_action(pstEnv, pstMachine, pstMachine->iHP, 0);
	}
	
	//燃料
	z_machine_attr_chg(pstEnv, pstMachine, MON_MACHINE_ATTR_ENERGY, pstRoleItem->InstVal2);

	//乘坐
	z_ride_on_machine(pstEnv, pstPlayer, pstMachine, machine_get_free_seatidx(pstEnv,pstMachine), pstRoleItem->WID);

	return 0;
}

ROLEITEM *package_find_item_by_wid(Player *pstPlayer, tdr_ulonglong ullWID, int *piListType, int *piPos)
{
	int i;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;

	for(i=0; i<pstPak->SundriesPackage.Num; i++)
	{
		if(pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem.WID == ullWID)
		{
			*piPos = i;
			*piListType = LIST_TYPE_SUNDRIES;
			return &pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem;
		}
	}

	for(i=0; i<pstPak->StuffPackage.Num; i++)
	{
		if(pstPak->StuffPackage.RoleItems[i].WID == ullWID)
		{
			*piPos = i;
			*piListType = LIST_TYPE_STUFF;
			return &pstPak->StuffPackage.RoleItems[i];
		}
	}

	for(i=0; i<pstPak->TaskPackage.Num; i++)
	{
		if(pstPak->TaskPackage.RoleItems[i].WID == ullWID)
		{
			*piPos = i;
			*piListType = LIST_TYPE_TASK;
			return &pstPak->TaskPackage.RoleItems[i];
		}
	}

	return NULL;
}

int machine_item_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_MACHINE_ITEM_LOCK;
	pstActionRes->Data.MachineItemLock.LockItemWid = pstPlayer->stOnline.stMachineInfo.ullLockItemWID;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int machine_item_lock(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong ullWid)
{
	pstPlayer->stOnline.stMachineInfo.ullLockItemWID = ullWid;
	
	if(pstPlayer->stOnline.stMachineInfo.ullLockItemWID > 0)
	{
		machine_item_action(pstEnv, pstPlayer);
	}
	
	return 0;
}

int machine_item_release_lock(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if(pstPlayer->stOnline.stMachineInfo.ullLockItemWID == 0)
	{
		return 0;
	}
	
	pstPlayer->stOnline.stMachineInfo.ullLockItemWID = 0;
	return machine_item_action(pstEnv, pstPlayer);
}

int machine_item_update(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iListType = 0;
	int iPos = 0;
	Monster *pstMachine;
	ROLEITEM *pstMachineItem;
	MONMACHINEDEF *pstMachineDef;
	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstPackageChg = &pstPackageInfo->PackageInfoData.PackageChg;
	
	if(pstPlayer->stOnline.stMachineInfo.ullLockItemWID == 0)
	{
		return 0;
	}

	pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	if(NULL == pstMachine)
	{
		return 0;
	}

	pstMachineDef = z_find_mon_machine_def(pstEnv, pstMachine->iDefIdx);
	if(NULL == pstMachineDef)
	{
		return 0;
	}

	pstMachineItem = package_find_item_by_wid(pstPlayer, 
			pstPlayer->stOnline.stMachineInfo.ullLockItemWID, &iListType, &iPos);
	if(NULL == pstMachineItem)
	{
		return 0;
	}

	if(pstMachine->cStat == MONSTER_DIE)
	{
		pstMachineItem->InstVal1 = 0;
		pstMachineItem->InstVal2 = 0;

		if(!(pstMachineDef->CtrlFlag & MACHINE_CTRL_DIE_NO_DEL))
		{
			package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMachineItem->DefID, 
				1, iListType, &iPos, PACKAGE_DEC_NONE, 0);
		}
	}
	else
	{
		MONSTERDEF *pstMonDef;
		
		pstMonDef = z_get_mon_def(pstEnv, pstMachine);
		if(NULL == pstMonDef)
		{
			return -1;
		}
		
		pstMachineItem->InstVal1 = pstMachine->iHP*1.0/pstMachine->iMaxHP*pstMonDef->HP;
		pstMachineItem->InstVal2 = get_machine_attr_val(pstEnv, pstMachine, MON_MACHINE_ATTR_ENERGY);
	}

	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstPackageChg->ArmChg.ChgNum = 0;
	pstPackageChg->ItemChg.ChgNum = 1;
	
	pstPackageChg->ItemChg.Items[0].ListType = iListType;
	memcpy(&pstPackageChg->ItemChg.Items[0].Item, 
		pstMachineItem, sizeof(pstPackageChg->ItemChg.Items[0].Item));

	Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int machine_get_ore_idx_byid(MONMACHINEDEF *pstMachineDef, int iID)
{
	int i;
	
	for(i=0; i<MAX_MON_MACHINE_ORE; i++)
	{
		if(pstMachineDef->Ore[i].OreItemID== iID)
		{
			return i;
		}
	}

	return -1;
}

int machine_ore_clear(ZONESVRENV* pstEnv, Monster *pstMachine)
{
	z_machine_attr_chg(pstEnv, pstMachine, 
		MON_MACHINE_ATTR_ORE1, -get_machine_attr_val(pstEnv, pstMachine, MON_MACHINE_ATTR_ORE1));
	z_machine_attr_chg(pstEnv, pstMachine, 
		MON_MACHINE_ATTR_ORE2, -get_machine_attr_val(pstEnv, pstMachine, MON_MACHINE_ATTR_ORE2));
	z_machine_attr_chg(pstEnv, pstMachine, 
		MON_MACHINE_ATTR_ORE3, -get_machine_attr_val(pstEnv, pstMachine, MON_MACHINE_ATTR_ORE3));
	z_machine_attr_chg(pstEnv, pstMachine, 
		MON_MACHINE_ATTR_ORE4, -get_machine_attr_val(pstEnv, pstMachine, MON_MACHINE_ATTR_ORE4));
	z_machine_attr_chg(pstEnv, pstMachine, 
		MON_MACHINE_ATTR_ORE5, -get_machine_attr_val(pstEnv, pstMachine, MON_MACHINE_ATTR_ORE5));

	return 0;
}

int player_machine_commit_ore(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEGRID *pastRoleGrid, int iNum)
{
	MONMACHINEDEF *pstMachineDef;
	Monster *pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	
	if(NULL == pstMachine)
	{
		return -1;
	}

	pstMachineDef = z_find_mon_machine_def(pstEnv, pstMachine->iDefIdx);
	if(NULL == pstMachineDef)
	{
		return -1;
	}

	if(0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pastRoleGrid, iNum, 0))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
		return -1;
	}

	machine_ore_clear(pstEnv, pstMachine);

	return  package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, pastRoleGrid, iNum, 0, IMPITEM_OPTYPE_CLAN_RESOURCE_MACHINE);
}

int player_machine_commit_ore_check(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEGRID *pastRoleGrid, int *piNum)
{
	int i;
	MONMACHINEINFO *pstMachineInfo;
	MONMACHINEDEF *pstMachineDef;
	Monster *pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	
	if(NULL == pstMachine)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE27);
		return -1;
	}

	pstMachineDef = z_find_mon_machine_def(pstEnv, pstMachine->iDefIdx);
	if(NULL == pstMachineDef)
	{
		return -1;
	}

	pstMachineInfo = &this_machine(pstEnv, pstMachine)->stMachineInfo;

	*piNum = 0;
	for(i=0; i<MAX_MON_MACHINE_ORE; i++)
	{
		if(pstMachineInfo->Ores[i].Ore <= 0)
		{
			continue;
		}

		if(0 > item_create(pstEnv->pstAppCtx, pstEnv, &pastRoleGrid[*piNum], pstMachineDef->Ore[i].OreItemID))
		{
			return -1;
		}

		if(pastRoleGrid[*piNum].Type == GRID_ITEM_ITEM)
		{
			pastRoleGrid[*piNum].GridData.RoleItem.Num = pstMachineInfo->Ores[i].Ore;
		}

		*piNum += 1;
	}

	if(*piNum <= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE27);
		return -1;
	}

	if(0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pastRoleGrid, *piNum, 0))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
		return -1;
	}

	return  0;
}


int machine_item_create(ZONESVRENV* pstEnv, RESULT *pstResult, ROLEITEM *pstRoleItem)
{
	Monster stMon;
	MONSTERDEF *pstMonDef;
	MONMACHINEDEF *pstMachineDef;

	pstMachineDef = z_find_mon_machine_def(pstEnv, pstResult->ResultVal2);
	if(NULL == pstMachineDef)
	{
		return -1;
	}

	stMon.iDefIdx = pstResult->ResultVal2;
	stMon.iDefPos = -1;
	pstMonDef = z_get_mon_def(pstEnv, &stMon);
	if(NULL == pstMonDef)
	{
		return -1;
	}

	pstRoleItem->InstVal1 = pstMonDef->HP;
	pstRoleItem->InstVal2 = pstMachineDef->MaxEnergy;
	
	return 0;
}

int is_mon_big_ore(int iOreDefID)
{
	if(iOreDefID == MON_COLLECT_BIG_ORE_ITEMID)
	{
		return 1;
	}
	
	return 0;
}

int is_mon_big_oil(int iOreDefID)
{
	if(iOreDefID == MON_COLLECT_BIG_OIL_ITEMID)
	{
		return 1;
	}
	
	return 0;
}

int is_mon_factory(Monster *pstMon)
{
	if(pstMon->bSubType == MON_SUB_TYPE_CLAN_FACTORY)
	{
		return 1;
	}
	
	return 0;
}

WORLDOREINFO *global_world_ore_get_byid(ZONESVRENV* pstEnv, int iDefID)
{
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;
	
	if(is_mon_big_ore(iDefID))
	{
		return  &pstWorldLimit->Ores[0];
	}
	else if(is_mon_big_oil(iDefID))
	{
		return &pstWorldLimit->Ores[1];
	}
	else
	{
		return NULL;
	}

	return NULL;
}

OREINFO *world_ore_get_by_factory(ZONESVRENV* pstEnv, int iFactoryID)
{
	int i;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;

	for(i=0; i<MAX_WORLD_ORE; i++)
	{
		if(pstWorldLimit->Ores[i].Factory.DefID == iFactoryID)
		{
			return &pstWorldLimit->Ores[i].Ore;
		}
	}
	
	return NULL;
}

FACTORYINFO *world_factory_get_by_memid(ZONESVRENV* pstEnv, int iID)
{
	int i;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;

	for(i=0; i<MAX_WORLD_ORE; i++)
	{
		if(pstWorldLimit->Ores[i].Factory.MemID== iID)
		{
			return &pstWorldLimit->Ores[i].Factory;
		}
	}
	
	return NULL;
}

OREINFO *world_ore_get_by_memid(ZONESVRENV* pstEnv, int iID)
{
	int i;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;

	for(i=0; i<MAX_WORLD_ORE; i++)
	{
		if(pstWorldLimit->Ores[i].Ore.MemID== iID)
		{
			return &pstWorldLimit->Ores[i].Ore;
		}
	}
	
	return NULL;
}

int mon_factory_del(ZONESVRENV * pstEnv, Monster *pstMon)
{
	//地图上拿掉
	z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
	z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);

	return 0;
}

int global_world_ore_del(ZONESVRENV* pstEnv, Monster *pstMon)
{
	int i;
	Monster *pstFactoryMon;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;

	for(i=MAX_WORLD_ORE-1; i>=0; i--)
	{
		if(pstWorldLimit->Ores[i].Ore.MemID== pstMon->iID)
		{
			pstFactoryMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstWorldLimit->Ores[i].Factory.MemID);
			if(pstFactoryMon && pstFactoryMon->cStat != MONSTER_DIE)
			{
				mon_factory_del(pstEnv, pstFactoryMon);
			}
			
			memset(&pstWorldLimit->Ores[i], 0, sizeof(pstWorldLimit->Ores[i]));
			//48小时CD
			pstWorldLimit->Ores[i].CoolDownTime = pstEnv->pstAppCtx->stCurr.tv_sec + WORLD_BIG_ORE_REFRESH_TIME;
		}
	}

	world_big_ore_info_broadcast(pstEnv);
	return 0;
}


//矿石更新
int mon_ore_update(ZONESVRENV* pstEnv, Monster *pstMon)
{
	OREINFO *pstOreInfo;

	if(!is_mon_big_ore(pstMon->iDefIdx) && !is_mon_big_oil(pstMon->iDefIdx))
	{
		return -1;
	}

	pstOreInfo = world_ore_get_by_memid(pstEnv, pstMon->iID);
	if(NULL == pstOreInfo || pstOreInfo->DefID != pstMon->iDefIdx)
	{
		return -1;
	}

	if(pstMon->cStat == MONSTER_DIE)
	{
		global_world_ore_del(pstEnv, pstMon);
	}
	else
	{
		pstOreInfo->HP = pstMon->iHP;
		pstOreInfo->Pos = pstMon->stCurrPos;
	}

	return 0;
}

//工厂更新
int mon_factory_update(ZONESVRENV* pstEnv, Monster *pstMon)
{
	FACTORYINFO *pstFactoryInfo;

	if(!is_mon_factory(pstMon))
	{
		return -1;
	}

	pstFactoryInfo = world_factory_get_by_memid(pstEnv, pstMon->iID);
	if(NULL == pstFactoryInfo || pstFactoryInfo->DefID != pstMon->iDefIdx)
	{
		return -1;
	}

	if(pstMon->cStat == MONSTER_DIE)
	{
		memset(pstFactoryInfo, 0, sizeof(*pstFactoryInfo));
	}
	else
	{
		pstFactoryInfo->HP = pstMon->iHP;
		pstFactoryInfo->Pos = pstMon->stCurrPos;
		pstFactoryInfo->BuildTime = 0;
		if(pstMon->stCommUnion.stFactory.tBuildTime > pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			pstFactoryInfo->BuildTime = pstMon->stCommUnion.stFactory.tBuildTime - pstEnv->pstAppCtx->stCurr.tv_sec;
		}
	}

	return 0;
}

int mon_clan_factory_del(ZONESVRENV* pstEnv, tdr_ulonglong ullClanGID)
{
	int i;
	Monster *pstFactoryMon;
	FACTORYINFO *pstFactoryInfo;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;

	for(i=0; i<MAX_WORLD_ORE; i++)
	{
		pstFactoryInfo = &pstWorldLimit->Ores[i].Factory;
		if(pstFactoryInfo->ClanGID == ullClanGID)
		{
			pstFactoryMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstFactoryInfo->MemID);
			if(pstFactoryMon && pstFactoryMon->cStat != MONSTER_DIE)
			{
				mon_factory_del(pstEnv, pstFactoryMon);
			}

			memset(pstFactoryInfo, 0, sizeof(*pstFactoryInfo));
		}
	}

	return 0;
}

int mon_factory_tick(ZONESVRENV* pstEnv, Monster *pstMon)
{
	int i;
	OREINFO *pstOreInfo;
	ROLEGRID stRoleGrid;
	Monster *pstOreMon;
	MONSTERDEF *pstMonDef;
	MONBOOTYITEM *pstBootyItem;
	ZoneClan * pstZoneClan;

	if(!is_mon_factory(pstMon))
	{
		return -1;
	}

	mon_factory_update(pstEnv, pstMon);
	if(pstMon->stCommUnion.stFactory.tBuildTime > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return 0;
	}

	pstOreInfo = world_ore_get_by_factory(pstEnv, pstMon->iDefIdx);
	if(NULL == pstOreInfo)
	{
		return -1;
	}

	pstOreMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstOreInfo->MemID);
	if(NULL == pstOreMon || pstOreMon->cStat == MONSTER_DIE)
	{
		return -1;
	}

	pstMonDef = z_get_mon_def(pstEnv, pstOreMon);
	if(NULL == pstMonDef)
	{
		return -1;
	}

	pstZoneClan = z_id_clan(pstEnv, pstMon->stCommUnion.stFactory.iClanID);
	if(NULL == pstZoneClan)
	{
		return -1;
	}

	if(pstMon->stCommUnion.stFactory.tBuildTime > 0)
	{
		pstMon->stCommUnion.stFactory.tBuildTime = 0;
		z_sys_strf_broadcast(pstEnv, SYS_TIP, 
			ZONEERR_BROAD325, 
			pstZoneClan->stClan.ClanProf.Name, pstMonDef->Name);
	}
	else
	{
		if(pstMon->stCommUnion.stFactory.tNotify < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			CSPKG stPkg;
			POSTIONLINK stPostionLink;

			stPostionLink.MapID = GET_MAPID(pstMon->stMap.iID);
			stPostionLink.X = pstMon->stCurrPos.X;
			stPostionLink.Y = pstMon->stCurrPos.Y;
	
			z_fill_link_msg(&stPkg, SYS_TIP, CHAT_LINK_POSTION, (void *)&stPostionLink, ZONEERR_LINK304,
				pstZoneClan->stClan.ClanProf.Name, pstMonDef->Name);
			z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
			pstMon->stCommUnion.stFactory.tNotify = pstEnv->pstAppCtx->stCurr.tv_sec+3600;
			//pstMon->stCommUnion.stFactory.tNotify = pstEnv->pstAppCtx->stCurr.tv_sec+60;
		}
	}

	for (i=0; i<MAX_MONBOOTY_ITEM; i++)
	{
		pstBootyItem = pstMonDef->Booty.Items + i;
		if (0 == pstBootyItem->ItemID)
		{
			break;
		}

		if (0 != pstBootyItem->TaskID1)
		{
			continue;
		}

		if ((int)(RAND1(10000)) >= (int)pstBootyItem->Rate)
		{
			continue;
		}

		if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstBootyItem->ItemID))
		{
			return -1;
		}

		if (GRID_ITEM_ITEM == stRoleGrid.Type)
		{
			stRoleGrid.GridData.RoleItem.Num = pstBootyItem->Num;
		}

		clan_commit_item_byid(pstEnv,pstZoneClan, stRoleGrid.GridData.RoleItem.DefID, stRoleGrid.GridData.RoleItem.Num);
	}

	pstOreMon->iHP -= 1;
	if(pstOreMon->iHP <= 0)
	{
		ZoneAni stAni;
			
		pstMon->cFirstAtkerType = 0;
		pstMon->ullFirstAtkerWid = 0;
		pstMon->iFirstAtkerID = 0; //没有战利品
		pstMon->iFirstAtkerMemID = 0;
		pstMon->szOwner[0] = 0;
		stAni.iType = OBJ_UNKNOW;
		z_mon_die(pstEnv->pstAppCtx, pstEnv, pstOreMon, pstMonDef, &stAni, 1, 1);
	}
	else
	{
		mon_hp_action(pstEnv, pstOreMon, pstOreMon->iHP, 0);
	}
	
	return 0;
}

int global_world_ore_add(ZONESVRENV * pstEnv, Monster *pstMon)
{
	WORLDOREINFO *pstWorldOreInfo;

	if(!is_fini_global(pstEnv))
	{
		return -1;
	}
	
	pstWorldOreInfo = global_world_ore_get_byid(pstEnv, pstMon->iDefIdx);
	if(NULL == pstWorldOreInfo)
	{
		return -1;
	}

	if(pstWorldOreInfo->Ore.DefID > 0)
	{
		return -1;
	}

	pstWorldOreInfo->Ore.DefID = pstMon->iDefIdx;
	pstWorldOreInfo->Ore.MemID = pstMon->iID;
	pstWorldOreInfo->Ore.MapID = GET_MAPID(pstMon->stMap.iID);
	pstWorldOreInfo->Ore.HP = pstMon->iHP;
	pstWorldOreInfo->Ore.Pos = pstMon->stCurrPos;
	memset(&pstWorldOreInfo->Factory, 0, sizeof(pstWorldOreInfo->Factory));

	world_big_ore_info_broadcast(pstEnv);
	return 0;
}

int global_world_factory_add(ZONESVRENV * pstEnv, Monster *pstOreMon, Monster *pstFactoryMon)
{
	WORLDOREINFO *pstWorldOreInfo;

	if(!is_fini_global(pstEnv))
	{
		return -1;
	}
	
	if(!is_mon_factory(pstFactoryMon))
	{
		return 0;
	}

	pstWorldOreInfo = global_world_ore_get_byid(pstEnv, pstOreMon->iDefIdx);
	if(NULL == pstWorldOreInfo)
	{
		return -1;
	}

	if(pstWorldOreInfo->Factory.DefID > 0)
	{
		return -1;
	}

	pstWorldOreInfo->Factory.DefID = pstFactoryMon->iDefIdx;
	pstWorldOreInfo->Factory.MemID = pstFactoryMon->iID;
	pstWorldOreInfo->Factory.MapID = GET_MAPID(pstFactoryMon->stMap.iID);
	pstWorldOreInfo->Factory.HP = pstFactoryMon->iHP;
	pstWorldOreInfo->Factory.Pos = pstFactoryMon->stCurrPos;
	pstWorldOreInfo->Factory.ClanId = pstFactoryMon->stCommUnion.stFactory.iClanID;
	pstWorldOreInfo->Factory.ClanGID = pstFactoryMon->stCommUnion.stFactory.ullClanGID;
	pstWorldOreInfo->Factory.BuildTime = 0;
	if(pstFactoryMon->stCommUnion.stFactory.tBuildTime > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		pstWorldOreInfo->Factory.BuildTime = pstFactoryMon->stCommUnion.stFactory.tBuildTime - pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	return 0;
}

Monster* global_new_ore_mon(ZONESVRENV * pstEnv, WORLDOREINFO *pstWorldOreInfo)
{
	Monster *pstMon, stMonster;
	MONSTERDEF *pstMonDef;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	
	stIdx.iID = pstWorldOreInfo->Ore.MapID;
	stIdx.iPos = -1;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return NULL;
	}

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = pstWorldOreInfo->Ore.DefID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.stInitPos = pstWorldOreInfo->Ore.Pos;
	stMonster.bDirection = RAND1(180);
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
	if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstMon,NULL))
	{
		tmempool_free(pstEnv->pstMonsterPool, pstMon->iMIndex);
		return NULL;
	}
	pstMon->cFirstBorn = 0;

	if(pstMon->iHP != pstWorldOreInfo->Ore.HP)
	{
		pstMon->iHP = pstWorldOreInfo->Ore.HP;
		mon_hp_action(pstEnv, pstMon, pstMon->iHP, 0);
	}
	
	return pstMon;
}

Monster* global_new_factory_mon(ZONESVRENV * pstEnv, WORLDOREINFO *pstWorldOreInfo)
{
	Monster *pstMon, stMonster;
	MONSTERDEF *pstMonDef;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	
	stIdx.iID = pstWorldOreInfo->Factory.MapID;
	stIdx.iPos = -1;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return NULL;
	}

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = pstWorldOreInfo->Factory.DefID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.stInitPos = pstWorldOreInfo->Factory.Pos;
	stMonster.bDirection = RAND1(180);
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

	mon_set_pkinfo(pstMon, MON_PK_TYPE_CLAN, pstWorldOreInfo->Factory.ClanGID);
	pstMon->nOwnerFastID = pstWorldOreInfo->Factory.ClanId;
	
	pstMon->iCommType = MON_UNION_TYPE_FACTORY;
	pstMon->stCommUnion.stFactory.iClanID = pstWorldOreInfo->Factory.ClanId;
	pstMon->stCommUnion.stFactory.ullClanGID = pstWorldOreInfo->Factory.ClanGID;
	pstMon->stCommUnion.stFactory.tBuildTime = pstEnv->pstAppCtx->stCurr.tv_sec + pstWorldOreInfo->Factory.BuildTime;

	pstMon->cFirstBorn = 1;
	if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstMon,NULL))
	{
		tmempool_free(pstEnv->pstMonsterPool, pstMon->iMIndex);
		return NULL;
	}
	pstMon->cFirstBorn = 0;

	if(pstMon->iHP != pstWorldOreInfo->Factory.HP)
	{
		pstMon->iHP = pstWorldOreInfo->Factory.HP;
		mon_hp_action(pstEnv, pstMon, pstMon->iHP, 0);

	}
	return pstMon;
}

int global_world_ore_refresh(ZONESVRENV* pstEnv)
{
	int i;
	Monster *pstOreMon, *pstFactoryMon;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;

	if(!is_fini_global(pstEnv))
	{
		return -1;
	}
	
	for(i=MAX_WORLD_ORE-1; i>=0; i--)
	{
		
		pstOreMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstWorldLimit->Ores[i].Ore.MemID);
		if(pstOreMon && pstWorldLimit->Ores[i].Ore.DefID == pstOreMon->iDefIdx)
		{
			continue;
		}

		//刷新矿
		pstOreMon = global_new_ore_mon(pstEnv, &pstWorldLimit->Ores[i]);
		if(pstOreMon)
		{
			pstWorldLimit->Ores[i].Ore.MemID = pstOreMon->iID;
		}

		pstFactoryMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstWorldLimit->Ores[i].Factory.MemID);
		if(pstFactoryMon && pstWorldLimit->Ores[i].Ore.DefID == pstFactoryMon->iDefIdx)
		{
			continue;
		}

		//刷新工厂
		pstFactoryMon = global_new_factory_mon(pstEnv, &pstWorldLimit->Ores[i]);
		if(pstFactoryMon)
		{
			pstWorldLimit->Ores[i].Factory.MemID = pstFactoryMon->iID;
		}
		
	}

	return 0;
}

int global_world_ore_new_check(ZONESVRENV * pstEnv, int iOreDefID)
{
	WORLDOREINFO *pstWorldOreInfo;
	
	if(!is_fini_global(pstEnv))
	{
		return -1;
	}

	pstWorldOreInfo = global_world_ore_get_byid(pstEnv, iOreDefID);
	if(NULL == pstWorldOreInfo)
	{
		return -1;
	}
	
	if(pstWorldOreInfo->Ore.DefID > 0)
	{
		return -1;
	}

	if(pstWorldOreInfo->CoolDownTime > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return -1;
	}

	return 0;
}

Monster* new_mon_factory(ZONESVRENV * pstEnv, Player *pstPlayer,
	Monster *pstOreMon, int iFactoryID, int iRadius)
{
	Monster *pstMon, stMonster;
	MapIndex *pstMapIdx;
	MONSTERDEF *pstMonDef;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	RESPOS stPos;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	if(NULL == pstClan)
	{
		return NULL;
	}
	
	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return NULL;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return NULL;
	}

	z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstOreMon->stCurrPos,
		&pstOreMon->stCurrPos, iRadius, 300, &stPos);

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iFactoryID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.stInitPos = stPos;
	stMonster.bDirection = RAND1(180);
	stMonster.stCurrPos = stMonster.stInitPos;

	pstMonDef = z_get_mon_def(pstEnv, &stMonster);
	if (NULL == pstMonDef || pstMonDef->SubType != MON_SUB_TYPE_CLAN_FACTORY)
	{
		return NULL;
	}

	pstMon =	z_new_monster(pstEnv->pstAppCtx, pstEnv, &stMonster, pstMonDef);
	if( !pstMon )
	{
		return NULL;
	}

	mon_set_pkinfo(pstMon, MON_PK_TYPE_CLAN, pstClan->stClan.ClanProf.GID);
	pstMon->nOwnerFastID = pstClan->stClan.ClanProf.Id;
	
	pstMon->iCommType = MON_UNION_TYPE_FACTORY;
	pstMon->stCommUnion.stFactory.iClanID = pstClan->stClan.ClanProf.Id;
	pstMon->stCommUnion.stFactory.ullClanGID = pstClan->stClan.ClanProf.GID;
	pstMon->stCommUnion.stFactory.tBuildTime = pstEnv->pstAppCtx->stCurr.tv_sec + MON_FACTORY_BUILD_TIME;
	//pstMon->stCommUnion.stFactory.tBuildTime = pstEnv->pstAppCtx->stCurr.tv_sec + 60;
	pstMon->stCommUnion.stFactory.tNotify = 0;
	pstMon->cFirstBorn = 1;
	if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstMon,NULL))
	{
		tmempool_free(pstEnv->pstMonsterPool, pstMon->iMIndex);
		return NULL;
	}
	pstMon->cFirstBorn = 0;

	global_world_factory_add(pstEnv, pstOreMon, pstMon);

	z_sys_strf_broadcast(pstEnv, SYS_TIP, 
		ZONEERR_BROAD327, 
		pstClan->stClan.ClanProf.Name, pstMonDef->Name);
		
	return pstMon;
}

int is_in_factory_build_time(ZONESVRENV *pstEnv)
{
	struct tm stNow;
	char sTmp[20];
	int iCurrDate;

	localtime_r(&pstEnv->pstAppCtx->stCurr.tv_sec, &stNow);

	//时间格式 xx小时xx分xx秒
	strftime(sTmp, sizeof(sTmp), "%H%M%S", &stNow);
	iCurrDate = atoi(sTmp);

	if(iCurrDate >= 100000 && 
		iCurrDate <= 235959)
	{
		return 1;
	}
	
	return 0;
}

int package_use_factory_item_check(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, int iOreMemID)
{
	WORLDOREINFO *pstWorldOreInfo;
	MONSTERDEF *pstMonDef;
	Monster stMon;
	Monster *pstOreMon;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	if(NULL == pstClan)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE28);
		return -1;
	}

	stMon.iDefIdx = pstResult->ResultVal2;
	stMon.iDefPos = -1;
	pstMonDef = z_get_mon_def(pstEnv, &stMon);
	if(NULL == pstMonDef || pstMonDef->SubType != MON_SUB_TYPE_CLAN_FACTORY)
	{
		return -1;
	}

	pstOreMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, iOreMemID);
	if(NULL == pstOreMon)
	{
		return -1;
	}

	pstWorldOreInfo = global_world_ore_get_byid(pstEnv, pstOreMon->iDefIdx);
	if(NULL == pstWorldOreInfo)
	{
		return -1;
	}
	
	if(pstWorldOreInfo->Factory.DefID > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_MACHINE29,
			pstMonDef->Name);
		return -1;
	}

	if(!is_in_factory_build_time(pstEnv))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_MACHINE30);
		return -1;
	}

	return 0;
}

int package_use_factory_item(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, int iOreMemID)
{
	Monster *pstOreMon;
	WORLDOREINFO *pstWorldOreInfo;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	if(NULL == pstClan)
	{
		return -1;
	}
	
	pstOreMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, iOreMemID);
	if(NULL == pstOreMon)
	{
		return -1;
	}

	pstWorldOreInfo = global_world_ore_get_byid(pstEnv, pstOreMon->iDefIdx);
	if(NULL == pstWorldOreInfo)
	{
		return -1;
	}
	
	if(pstWorldOreInfo->Factory.DefID > 0)
	{
		return -1;
	}
	
	new_mon_factory(pstEnv, pstPlayer, pstOreMon, pstResult->ResultVal2, pstResult->ResultVal1);
	return 0;
}

int package_use_machine_energy_check(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult)
{
	Monster *pstMachine;

	pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	if(NULL == pstMachine)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE31);
		return -1;
	}

	if(pstMachine->iDefIdx == 1460090)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,ZONEERR_WEDDING75);
		return -1;
	}

	if(this_machine(pstEnv, pstMachine)->stMachineInfo.Energy >= this_machine(pstEnv, pstMachine)->stMachineInfo.MaxEnergy)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE32);
		return -1;
	}

	return 0;
	UNUSED(pstResult);
}

int package_use_machine_energy(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult)
{
	int iEnergy;
	Monster *pstMachine;

	pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	if(NULL == pstMachine)
	{
		return -1;
	}

	iEnergy = pstResult->ResultVal1 + this_machine(pstEnv, pstMachine)->stMachineInfo.MaxEnergy*pstResult->ResultVal2/100.0;
	z_machine_attr_chg(pstEnv, pstMachine, MON_MACHINE_ATTR_ENERGY, iEnergy);
	return 0;
	
}


int package_use_machine_hp_check(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult)
{
	Monster *pstMachine;

	pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	if(NULL == pstMachine)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE31);
		return -1;
	}

	if(pstMachine->iHP >= pstMachine->iMaxHP)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE33);
		return -1;
	}

	return 0;
	UNUSED(pstResult);
}

int package_use_machine_status_check(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult)
{
	Monster *pstMachine;

	pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	if(NULL == pstMachine)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MACHINE31);
		return -1;
	}

	return 0;
	UNUSED(pstResult);
}

int package_use_machine_status(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult)
{
	Monster *pstMachine;

	pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	if(NULL == pstMachine)
	{
		return -1;
	}

	z_mon_apply_status(pstEnv->pstAppCtx, pstEnv, pstMachine, pstResult->ResultVal2, 
						pstResult->ResultVal3, pstResult->ResultVal4, 1, 0);
	
	return 0;
}

int package_use_machine_hp(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult)
{
	Monster *pstMachine;
	int iOldHP;

	pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	if(NULL == pstMachine)
	{
		return -1;
	}

	iOldHP = pstMachine->iHP;
	pstMachine->iHP += pstResult->ResultVal1 + pstMachine->iMaxHP*pstResult->ResultVal2/100.0;
	if(pstMachine->iHP > pstMachine->iMaxHP)
	{
		pstMachine->iHP = pstMachine->iMaxHP;
	}

	mon_hp_action(pstEnv, pstMachine, iOldHP, 0);
	return 0;
}

int machine_pk_val_action(ZONESVRENV* pstEnv, Monster *pstMachineMon)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	if(!is_mon_machine(pstMachineMon))
	{
		return 0;
	}
	
	pstActionRes->ID = pstMachineMon->iID;
	pstActionRes->ActionID = ACTION_PK_VAL_CHG;
	pstActionRes->Data.PkValChg.PKVal = this_machine(pstEnv,pstMachineMon)->stMachineInfo.PKVal;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_monview(pstEnv->pstAppCtx, pstEnv, pstMachineMon, &stPkg, 0);
	
	return 0;
}

int machine_pk_time_action(ZONESVRENV* pstEnv, Monster *pstMachineMon)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	if(!is_mon_machine(pstMachineMon))
	{
		return 0;
	}
	
	pstActionRes->ID = pstMachineMon->iID;
	pstActionRes->ActionID = ACTION_PK_TIME;
	pstActionRes->Data.PKTime.PkTime = 0;

	if(this_machine(pstEnv,pstMachineMon)->stMachineInfo.PkTime > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		pstActionRes->Data.PKTime.PkTime = this_machine(pstEnv,pstMachineMon)->stMachineInfo.PkTime - pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_monview(pstEnv->pstAppCtx, pstEnv, pstMachineMon, &stPkg, 0);

	return 0;
}

int machine_get_max_passenger_pkval(ZONESVRENV* pstEnv, Monster *pstMachineMon)
{
	int i;
	int iPkVal = 0;
	Player *pstTmpPlayer;
	
	for(i=0; i<MAX_MON_MACHINE_PASSENGER; i++)
	{
		if(i >= this_machine(pstEnv,pstMachineMon)->stMachineInfo.MaxPassenger)
		{
			break;
		}

		if(this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[i] <=0)
		{
			continue;
		}

		pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, this_machine(pstEnv,pstMachineMon)->stMachineInfo.PassengerID[i]);
		if(pstTmpPlayer && PLAYER_STATUS_ROLE_LOGIN == pstTmpPlayer->eStatus)
		{
			if(pstTmpPlayer->stRoleData.MiscInfo.PkInfo.PkVal > iPkVal)
			{
				iPkVal = pstTmpPlayer->stRoleData.MiscInfo.PkInfo.PkVal;
			}
		}
	}

	return iPkVal;
}

int machine_pkval_update(ZONESVRENV* pstEnv, Monster *pstMachineMon)
{
	int iPkVal = machine_get_max_passenger_pkval(pstEnv, pstMachineMon);

	if(this_machine(pstEnv,pstMachineMon)->stMachineInfo.PKVal != iPkVal)
	{
		this_machine(pstEnv,pstMachineMon)->stMachineInfo.PKVal = iPkVal;
		machine_pk_val_action(pstEnv, pstMachineMon);
	}

	return 0;
}

int player_ride_on_machine_target_update(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMachine)
{
	int i;
	Monster *pstMon;
	
	for (i=pstPlayer->stOnline.iVAnimate-1; i>=0; i--)
	{
		pstMon = z_id_monster2(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.aiVAnimate[i]);
		if (NULL == pstMon || pstMon->bSubType >= MON_SUB_TYPE_TMP || pstMon->cStat != MONSTER_ALIVE )
		{
			continue;
		}

		if (0 > mon_press_find(pstMon, pstPlayer->iMemID))
		{
			continue;
		}

		mon_press_insert_mon(pstEnv, pstMon, pstMachine);
	}

	return 0;
}

int player_ride_off_machine_target_update(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMachine)
{
	int i;
	Monster *pstMon;
	MONSTERDEF *pstMonDef;

	for (i=0; i<pstMachine->iVPlayer; i++)
	{
		pstMon = z_id_monster2(pstEnv->pstAppCtx, pstEnv, pstMachine->aiVAnimate[i]);
		if (NULL == pstMon || pstMon->bSubType >= MON_SUB_TYPE_TMP || pstMon->cStat != MONSTER_ALIVE )
		{
			continue;
		}

		if (0 > mon_press_find(pstMon, pstMachine->iID))
		{
			continue;
		}

		pstMonDef = z_get_mon_def(pstEnv, pstMon);
		if (NULL == pstMonDef)
		{
			continue;
		}

		mon_press_insert_player(pstEnv, pstMon, pstMonDef, pstPlayer);
	}

	return 0;
}

int machine_ride_on_pktime_update(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMachine)
{
	if(!is_in_pk_time_player(pstEnv, pstPlayer))
	{
		return 1;
	}

	if(pstMachine->iPKType != MON_PK_TYPE_PLAYER)
	{
		return 0;
	}

	this_machine(pstEnv, pstMachine)->stMachineInfo.PkTime = pstPlayer->stOnline.tPkEndTime;
	return machine_pk_time_action(pstEnv, pstMachine);
}

int mon_factory_warning_byhit(ZONESVRENV *pstEnv, Monster *pstMon)
{	
	MONSTERDEF * pstMonsterDef;
	ZoneClan * pstZoneClan;
	int iMul;
	
	pstMonsterDef = z_get_mon_def(pstEnv, pstMon);
	if(NULL == pstMonsterDef)
	{
		return -1;
	}
	
	if(!is_mon_factory(pstMon))
	{
		return -1;
	}
	
	pstZoneClan = z_id_clan(pstEnv, pstMon->stCommUnion.stFactory.iClanID);
	if(NULL == pstZoneClan)
	{
		return -1;
	}

	iMul = pstMon->iHP*100/pstMon->iMaxHP;
	if(iMul <= 20 && pstMon->iWarningMul != 20)
	{
		pstMon->iWarningMul = 20;
		z_sys_strf_broadcast(pstEnv, SYS_SVR, 
			ZONEERR_BROAD328,
			pstZoneClan->stClan.ClanProf.Name, pstMonsterDef->Name, 
			GET_MAPID(pstMon->stMap.iID), pstMon->stCurrPos.X, pstMon->stCurrPos.Y);
	}
	
	if(!pstMon->iWarningTime || ( pstMon->iWarningTime + 120 <= pstEnv->pstAppCtx->stCurr.tv_sec ))
	{
		pstMon->iWarningTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN, 
			ZONEERR_CLANBROAD265,
			pstMonsterDef->Name, GET_MAPID(pstMon->stMap.iID), pstMon->stCurrPos.X, pstMon->stCurrPos.Y);
	}
	return 0;

}


int machine_dart_car_login(ZONESVRENV *pstEnv, Player *pstPlayer)
{	
	Monster *pstMonster; 
	int iID = pstPlayer->stRoleData.MiscInfo.DartCarInfo.MonMemID;

	if (iID <= 0) return 0;
	
	pstMonster = z_id_monster(pstEnv->pstAppCtx,pstEnv, iID);
	if (!pstMonster || !is_mon_biaoche(pstMonster))
	{
		return 0;
	}

	if(this_machine(pstEnv, pstMonster)->stMachineInfo.OwnerType != MACHINE_OWNER_SELF ||
		this_machine(pstEnv, pstMonster)->stMachineInfo.OwnerID != pstPlayer->stRoleData.RoleID)
	{
		return 0;
	}

	mon_set_pkinfo(pstMonster, MON_PK_TYPE_PLAYER, pstPlayer->iMemID);
	return 0;
}

