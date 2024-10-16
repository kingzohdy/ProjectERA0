#include "zone_pworld.h"
#include "zone_svr.h"
#include "zone_clt.h"
#include "zone_map.h"
#include "zone_battle.h"
#include "zone_script.h"
#include "zone_machine.h"
#include "zone_misc.h"
#include "zone_attr.h"
#include "zone_diffpworld.h"
#include "zone_package.h"
#include "zone_oplog.h"
#include "zone_err.h"
#include "zone_sparta.h"
#include "zone_dianfeng.h"
#include "zone_ploy.h"

extern ScriptEnv stSEnv;

typedef struct
{
	int iHuman;
	int iTitan;
	int iMonster;
}MachineWarCampInfo;

int machine_war_pworld_info(ZONESVRENV * pstEnv, ZONEPWORLD *pstPworld,
	ZONEPWORLDINST *pstPworldInst, Player *pstPlayer, int fEnd);

int is_human_player(Player *pstPlayer)
{
	if(pstPlayer->stOnline.stWarOL.cCampIdx == MACHINE_WAR_CAMP_HUMAN)
	{
		return 1;
	}
	
	return 0;
}

int is_titan_player(Player *pstPlayer)
{
	if(pstPlayer->stOnline.stWarOL.cCampIdx == MACHINE_WAR_CAMP_TITAN)
	{
		return 1;
	}
	
	return 0;
}

int is_monster_player(Player *pstPlayer)
{
	if(pstPlayer->stOnline.stWarOL.cCampIdx == MACHINE_WAR_CAMP_MONSTER)
	{
		return 1;
	}
	
	return 0;
}

static int get_min_campnum(ZONEPWORLDINST *pstZonePworldInst)
{
	int i,iIdx;
	int iNum;

	iIdx = 0;
	iNum = pstZonePworldInst->stPworldCamp.stCampInfo[0].iPlayerNum;
	
	for (i=1; i<pstZonePworldInst->stPworldCamp.iCampNum; i++)
	{
		if (pstZonePworldInst->stPworldCamp.stCampInfo[i].iPlayerNum< iNum)
		{
			iNum = pstZonePworldInst->stPworldCamp.stCampInfo[i].iPlayerNum;
			iIdx = i;
		}
	}

	return iIdx;		
}

int player_set_poly_info(ZONESVRENV * pstEnv, Player *pstPlayer, MapIndex *pstMapIndex,
	ZONEPWORLD *pstZonePworld, ZONEPWORLDINST *pstZonePworldInst, RESPOS *pstPos)
{
	int iCamp = 0, iPos = 0;

	switch(pstZonePworld->stPworldDef.PolyType)
	{
		case PWORLD_POLY_DOUBLE_MONSTER:
		case PWORLD_POLY_TANK:
			{	
				iCamp = get_min_campnum(pstZonePworldInst)+1;
				iPos = iCamp -1;
			}
			break;
		case PWORLD_POLY_MACHINE_WAR:
			{
				iCamp = MACHINE_WAR_CAMP_HUMAN;
				iPos = RAND1(2);
			}
			break;
		default:
			return 0;
			break;
	}

	memset (&pstPlayer->stOnline.stWarOL, 0, sizeof(pstPlayer->stOnline.stWarOL));

	//阵营统计信息
	if(iCamp > 0 && iCamp<= MAX_CAMP_NUM)
	{
		pstZonePworldInst->stPworldCamp.stCampInfo[iCamp-1].iCampLv += pstPlayer->stRoleData.Level;
		pstZonePworldInst->stPworldCamp.stCampInfo[iCamp-1].iPlayerNum++;
	}

	//出生点
	if(iPos >=0 && iPos<REVIVEPOS_NUM )
	{
		if (pstMapIndex->stMapDef.RevivePs[iPos].X > 0 && pstMapIndex->stMapDef.RevivePs[iPos].Y > 0)
		{
			*pstPos = pstMapIndex->stMapDef.RevivePs[iPos];
		}
	}	

	pstPlayer->stOnline.stWarOL.cCampIdx = iCamp;
	pstPlayer->stOnline.stWarOL.iLevel = pstPlayer->stRoleData.Level;
	pstPlayer->stOnline.stWarOL.cPloyType = pstZonePworld->stPworldDef.PolyType;
	pstPlayer->stOnline.stWarOL.tFightTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	return 0;
}

int player_pworld_camp_clear(Player *pstPlayer, ZONEPWORLDINST* pstPworldInst)
{
	if(pstPworldInst->stPworldCamp.iCampNum > 0 && pstPlayer->stOnline.stWarOL.cCampIdx > 0)
	{
		int iCamp =  pstPlayer->stOnline.stWarOL.cCampIdx - 1;
		PworldCamp *pstPworldCamp;

		pstPworldCamp = &pstPworldInst->stPworldCamp;
		
		pstPworldCamp->stCampInfo[iCamp].iPlayerNum--;
		if(pstPworldCamp->stCampInfo[iCamp].iPlayerNum < 0)
		{
			pstPworldCamp->stCampInfo[iCamp].iPlayerNum = 0;
		}
		
		pstPworldCamp->stCampInfo[iCamp].iCampLv -= pstPlayer->stOnline.stWarOL.iLevel;
		if(pstPworldCamp->stCampInfo[iCamp].iCampLv < 0)
		{
			pstPworldCamp->stCampInfo[iCamp].iCampLv = 0;
		}
	}

	return 0;
}

int z_player_leave_poly_pworld(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLDINST* pstPworldInst)
{
	ZONEPWORLD *pstPworld = NULL;
	
	if (!pstPworldInst)
	{
		return -1;
	}

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if (NULL == pstPworld)
	{
		return -1;
	}

	if(is_normal_battle_pworld(pstPworld) || 
		pstPworld->stPworldDef.PolyType == PWORLD_POLY_DOUBLE_MONSTER ||
		pstPworld->stPworldDef.PolyType == PWORLD_POLY_MACHINE_WAR ||
		pstPworld->stPworldDef.PolyType == PWORLD_POLY_TANK)
	{
		if(is_normal_battle_pworld(pstPworld))
		{
			//逃跑BUFF
			z_add_runaway_buff(pstEnv, pstPlayer);
		}

		if (pstPworld->stPworldDef.CampNum > 1)
		{
			player_pworld_camp_clear(pstPlayer, pstPworldInst);
		}

		player_xukong_energy_clear(pstEnv, pstPlayer);

		memset (&pstPlayer->stOnline.stWarOL, 0, sizeof(pstPlayer->stOnline.stWarOL));
	}
	else if(pstPworld->stPworldDef.PolyType == PWORLD_POLY_SPARTA)
	{
		player_leave_sparta(pstEnv, pstPworldInst, pstPlayer);
		memset (&pstPlayer->stOnline.stWarOL, 0, sizeof(pstPlayer->stOnline.stWarOL));
	}

	return 0;
}

int double_monster_total(ZONESVRENV * pstEnv, ZONEPWORLDINST *pstPworldInst, int *piMonster1, int *piMonster2)
{
	int i;
	ZONEMAPINST *pstMapInst;

	*piMonster1 = 0;
	*piMonster2 = 0;
	
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPworldInst->astMapInstIdx[0]);
	if( NULL == pstMapInst )
	{
		return -1;
	}

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
			Monster* pstMonster = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstArea->aiAnimate[k]);
			
			if(NULL == pstMonster || MONSTER_DIE == pstMonster->cStat)
			{
				continue;
			}

			if(pstMonster->iDefIdx == 260100 || pstMonster->iDefIdx == 260102)
			{
				*piMonster1+=1;
			}
			else if(pstMonster->iDefIdx == 260101 || pstMonster->iDefIdx == 260103)
			{
				*piMonster2+=1;
			}
		}
	}

	return 0;
}

int double_monster_role_info(CSDOUBLEMONSTERROLEINFO *pstInfo, Player *pstPlayer, tdr_longlong llExp)
{
	memset(pstInfo, 0, sizeof(*pstInfo));
	
	pstInfo->CampIdx = pstPlayer->stOnline.stWarOL.cCampIdx;
	pstInfo->Career = pstPlayer->stRoleData.Career;
	pstInfo->MemID = pstPlayer->iMemID;
	
	STRNCPY(pstInfo->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstInfo->RoleName));
	pstInfo->ClanName[0] = 0;
	if (pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id > 0)
	{
		STRNCPY(pstInfo->ClanName, pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Name, sizeof(pstInfo->ClanName));
	}

	pstInfo->KillMonster = pstPlayer->stOnline.stWarOL.iKillNum;
	pstInfo->Exp = llExp;
	return 0;
}

static int double_monster_info_qsort_cmp(const void *p1, const void *p2)
{
	CSDOUBLEMONSTERROLEINFO *pInfo1 = (CSDOUBLEMONSTERROLEINFO*)p1;
	CSDOUBLEMONSTERROLEINFO *pInfo2 = (CSDOUBLEMONSTERROLEINFO*)p2;

	return pInfo2->KillMonster - pInfo1->KillMonster;
}

 int is_valid_money_award_player(int iIdx, int iMaxRole)
{
	if(iMaxRole == 1)
	{
		return 1;
	}
	else
	{
		if( ((iIdx+1)*100/iMaxRole) <= 60)
		{
			return 1;
		}
	}
		
	return 0;
}

int double_monster_pworld_info(ZONESVRENV * pstEnv, ZONEPWORLD *pstPworld,
	ZONEPWORLDINST *pstPworldInst, Player *pstPlayer, int fEnd)
{
	int i, iPlayer = 0;
	Player *pstPworldPlayer;
	Player *apstPlayer[MAX_PWORLD_MEMBER];
	tdr_longlong llExp = 0, llMoney;
	LEVELUP *pstLevelUP;
	CSPKG stPkg;
	CSPOLYPWORLDSVR *pstCSPolyPworldSvr = &stPkg.Body.PolyPworldSvr;
	CSDOUBLEMONSTERINFO *pstCSDoubleMonsterInfo;

	Z_CSHEAD_INIT(&stPkg.Head, POLY_PWORLD_SVR);
	pstCSPolyPworldSvr->SvrType = POLY_SVR_TYPE_INFO;
	pstCSPolyPworldSvr->SvrData.PolyPworldInfo.OpType = POLY_OP_DOUBLE_MONSTER;
	pstCSPolyPworldSvr->SvrData.PolyPworldInfo.End = fEnd;

	pstCSDoubleMonsterInfo = &pstCSPolyPworldSvr->SvrData.PolyPworldInfo.Data.DoubleMonster;
	memset(pstCSDoubleMonsterInfo, 0, sizeof(*pstCSDoubleMonsterInfo));

	//真正的开始时间
	pstCSDoubleMonsterInfo->StartTime = pstPworldInst->tStart + pstPworld->stPworldDef.ReadyTime;

	if(pstCSDoubleMonsterInfo->StartTime > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		pstCSDoubleMonsterInfo->MonsterCamp1 = 0;
		pstCSDoubleMonsterInfo->MonsterCamp2 = 0;
	}
	else
	{
		double_monster_total(pstEnv, pstPworldInst, 
			&pstCSDoubleMonsterInfo->MonsterCamp1, &pstCSDoubleMonsterInfo->MonsterCamp2);
	}

	if(fEnd)
	{
		pstCSPolyPworldSvr->SvrData.PolyPworldInfo.End = 1;
		pstCSDoubleMonsterInfo->VCamp = pstCSDoubleMonsterInfo->MonsterCamp1 > pstCSDoubleMonsterInfo->MonsterCamp2 ? 2:1;
	}

	if(pstPlayer)
	{
		double_monster_role_info(&pstCSDoubleMonsterInfo->MonsterEndInfo.RoleList[pstCSDoubleMonsterInfo->MonsterEndInfo.RoleNum], pstPlayer, 0);
		pstCSDoubleMonsterInfo->MonsterEndInfo.RoleNum++;
		
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	else
	{
		for (i=0; i<pstPworldInst->iPlayer; i++)
		{
			pstPworldPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i]);
			if (!pstPworldPlayer)
				continue;
			
			apstPlayer[iPlayer++] = pstPworldPlayer;
			
			double_monster_role_info(&pstCSDoubleMonsterInfo->MonsterEndInfo.RoleList[pstCSDoubleMonsterInfo->MonsterEndInfo.RoleNum], pstPworldPlayer, 0);
			pstCSDoubleMonsterInfo->MonsterEndInfo.RoleNum++;
		}

		if(pstCSPolyPworldSvr->SvrData.PolyPworldInfo.End)
		{
			qsort(&pstCSDoubleMonsterInfo->MonsterEndInfo.RoleList[0], pstCSDoubleMonsterInfo->MonsterEndInfo.RoleNum,
				sizeof(pstCSDoubleMonsterInfo->MonsterEndInfo.RoleList[0]), double_monster_info_qsort_cmp);

			for(i=0; i<pstCSDoubleMonsterInfo->MonsterEndInfo.RoleNum; i++)
			{
				pstPworldPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstCSDoubleMonsterInfo->MonsterEndInfo.RoleList[i].MemID);
				if(!pstPworldPlayer)
					continue;

				pstLevelUP = z_get_levelup(pstEnv, pstPworldPlayer->stRoleData.Level);
				if(pstLevelUP)
				{
					if(pstCSDoubleMonsterInfo->VCamp == pstPworldPlayer->stOnline.stWarOL.cCampIdx)
					{
						llExp = (tdr_ulonglong)100*pstLevelUP->ContributeExp/10000.0;
					}
					else
					{
						llExp = (tdr_ulonglong)80*pstLevelUP->ContributeExp/10000.0;
					}
					
					player_world_level_exp(pstEnv, pstPworldPlayer, &llExp);
					pstCSDoubleMonsterInfo->MonsterEndInfo.RoleList[i].Exp = llExp;
					
					player_add_exp(pstEnv, pstPworldPlayer, llExp, 0);
					package_add_money( pstEnv,  pstPworldPlayer, ATTR_ID_LIFT_BRAND, 100);
					

					if(is_valid_money_award_player(i, pstCSDoubleMonsterInfo->MonsterEndInfo.RoleNum))
					{
						llMoney = (tdr_ulonglong)100*pstLevelUP->ContributeMoney/10000.0;
						package_add_money(pstEnv, pstPworldPlayer, ATTR_ID_MONEY, llMoney);
						pstCSDoubleMonsterInfo->MonsterEndInfo.RoleList[i].Money = llMoney;

						z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_DOUBLE_MONSTER, 
							0, 0, llMoney, "光影之战银币奖励");
					}
				}
			}
		}

		if(iPlayer > 0)
		{
			z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer,iPlayer, &stPkg);
		}
	}
	
	return 0;
}

int player_enter_poly_pworld_info(ZONESVRENV * pstEnv, ZONEPWORLDINST *pstZonePworldInst, Player *pstPlayer)
{
	ZONEPWORLD *pstPworld = NULL;

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
	if (NULL == pstPworld)
	{
		return -1;
	}

	switch(pstPworld->stPworldDef.PolyType)
	{
		case PWORLD_POLY_DOUBLE_MONSTER:
			{	
				return double_monster_pworld_info(pstEnv, pstPworld, pstZonePworldInst, pstPlayer, 0);
			}
			break;
		case PWORLD_POLY_MACHINE_WAR:
			{	
				return machine_war_pworld_info(pstEnv, pstPworld, pstZonePworldInst, pstPlayer, 0);
			}
			break;
		case PWORLD_POLY_SPARTA:
			return sparta_fight_info_update(pstEnv, pstZonePworldInst, pstPlayer, 0, 0);
			break;
		case PWORLD_POLY_TANK:
			return ploy_tank_pworld_info(pstEnv, pstPworld,pstZonePworldInst, pstPlayer, 0);
			break;
		default:
			break;
	}

	return 0;
}

int double_monster_pworld_tick(ZONESVRENV * pstEnv, ZONEPWORLD *pstPworld, ZONEPWORLDINST *pstZonePworldInst, int iSec)
{
	int iMon1 = 0, iMon2 = 0;
	
	double_monster_total(pstEnv, pstZonePworldInst, &iMon1, &iMon2);

	//提前结束
	if(iMon1 >= 500 || iMon2 >= 500)
	{
		double_monster_pworld_info(pstEnv, pstPworld, pstZonePworldInst, NULL, 1);
		//停止副本刷怪等。
		z_pworld_over(pstEnv, pstZonePworldInst);
		pstZonePworldInst->tEnd = pstEnv->pstAppCtx->stCurr.tv_sec+10;
	}
	else
	{
		if(iSec % 5 == 0)
		{
			double_monster_pworld_info(pstEnv, pstPworld, pstZonePworldInst, NULL, 0);
		}
	}
	
	return 0;
}

int machine_war_change_camp(ZONESVRENV * pstEnv, Player *pstPlayer, ZONEPWORLDINST* pstPworldInst, int iCamp, int fLeader)
{
	player_pworld_camp_clear(pstPlayer, pstPworldInst);

	//阵营统计信息
	if(iCamp > 0 && iCamp<= MAX_CAMP_NUM)
	{
		pstPworldInst->stPworldCamp.stCampInfo[iCamp-1].iCampLv += pstPlayer->stRoleData.Level;
		pstPworldInst->stPworldCamp.stCampInfo[iCamp-1].iPlayerNum++;
	}

	pstPlayer->stOnline.stWarOL.cCampIdx = iCamp;
	pstPlayer->stOnline.stWarOL.fLeader = fLeader;

	return 0;
}

int machine_war_rand_create_leader(ZONESVRENV * pstEnv, ZONEPWORLDINST *pstPworldInst, int iCamp)
{
	int i, iChg = 0;
	Player *pstPlayer;
	MapIndex * pstMapIdx;
	ZONEMAPINST *pstMapInst;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPworldInst->astMapInstIdx[0]);
	if( NULL == pstMapInst )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_find_map_inst %d map inst fail", pstPworldInst->astMapInstIdx[0].iID);
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}	
	
	for (i=0; i<pstPworldInst->iPlayer; i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i]);
		if (!pstPlayer)
		{
			continue;
		}

		if(pstPlayer->stOnline.stWarOL.cCampIdx == iCamp)
		{
			iChg = 1;
			pstPlayer->stOnline.stWarOL.fLeader = 1;
			player_enter_map_add_machine(pstEnv, pstPlayer, pstMapIdx);
			break;
		}
	}

	if(iChg)
	{
		return 0;
	}

	for (i=0; i<pstPworldInst->iPlayer; i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i]);
		if (!pstPlayer)
		{
			continue;
		}

		if(pstPlayer->stOnline.stWarOL.cCampIdx != MACHINE_WAR_CAMP_HUMAN && 
			pstPlayer->stOnline.stWarOL.cCampIdx != iCamp &&
			pstPlayer->stOnline.stWarOL.fLeader == 0)
		{
			iChg = 1;
			machine_war_change_camp(pstEnv, pstPlayer, pstPworldInst, iCamp, 1);
			player_enter_map_add_machine(pstEnv, pstPlayer, pstMapIdx);
			break;
		}
	}

	if(iChg)
	{
		return 0;
	}

	for (i=0; i<pstPworldInst->iPlayer; i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i]);
		if (!pstPlayer)
		{
			continue;
		}

		if(pstPlayer->stOnline.stWarOL.cCampIdx == MACHINE_WAR_CAMP_HUMAN)
		{
			iChg = 1;
			machine_war_change_camp(pstEnv, pstPlayer, pstPworldInst, iCamp, 1);
			player_enter_map_add_machine(pstEnv, pstPlayer, pstMapIdx);
			break;
		}
	}
	
	return 0;
}

int machine_war_player_total(ZONEPWORLDINST *pstPworldInst, int *piHuman, int *piTitan, int *piMonster)
{
	*piHuman = pstPworldInst->stPworldCamp.stCampInfo[0].iPlayerNum;
	*piTitan = pstPworldInst->stPworldCamp.stCampInfo[1].iPlayerNum;
	*piMonster = pstPworldInst->stPworldCamp.stCampInfo[2].iPlayerNum;

	return 0;
}

int machine_war_pworld_info(ZONESVRENV * pstEnv, ZONEPWORLD *pstPworld,
	ZONEPWORLDINST *pstPworldInst, Player *pstPlayer, int fEnd)
{
	int i;
	Player *pstPworldPlayer;
	tdr_longlong llExp;
	LEVELUP *pstLevelUP;
	CSPKG stPkg;
	CSPOLYPWORLDSVR *pstCSPolyPworldSvr = &stPkg.Body.PolyPworldSvr;
	CSMACHINEWARINFO *pstCSMachineWarInfo;

	Z_CSHEAD_INIT(&stPkg.Head, POLY_PWORLD_SVR);
	pstCSPolyPworldSvr->SvrType = POLY_SVR_TYPE_INFO;
	pstCSPolyPworldSvr->SvrData.PolyPworldInfo.OpType = POLY_OP_MACHINE_WAR;
	pstCSPolyPworldSvr->SvrData.PolyPworldInfo.End = fEnd;

	pstCSMachineWarInfo = &pstCSPolyPworldSvr->SvrData.PolyPworldInfo.Data.MachineWar;
	pstCSMachineWarInfo->VCamp = 0;
	pstCSMachineWarInfo->EndExp = 0;
	pstCSMachineWarInfo->ToTalExp = 0;

	//真正的开始时间
	pstCSMachineWarInfo->StartTime = pstPworldInst->tStart + pstPworld->stPworldDef.ReadyTime;

	machine_war_player_total(pstPworldInst, 
		&pstCSMachineWarInfo->Human, &pstCSMachineWarInfo->Titan, &pstCSMachineWarInfo->Monster);

	if(fEnd)
	{
		pstCSPolyPworldSvr->SvrData.PolyPworldInfo.End = 1;
		if(pstCSMachineWarInfo->Human > 0)
		{
			pstCSMachineWarInfo->VCamp = MACHINE_WAR_CAMP_HUMAN;
		}
		else if (pstCSMachineWarInfo->Titan > pstCSMachineWarInfo->Monster)
		{
			pstCSMachineWarInfo->VCamp = MACHINE_WAR_CAMP_TITAN;
		}
		else
		{
			pstCSMachineWarInfo->VCamp = MACHINE_WAR_CAMP_MONSTER;
		}
	}

	if(pstPlayer)
	{
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	else
	{
		for (i=0; i<pstPworldInst->iPlayer; i++)
		{
			pstPworldPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i]);
			if (!pstPworldPlayer)
				continue;

			if(fEnd)
			{
				pstLevelUP = z_get_levelup(pstEnv, pstPworldPlayer->stRoleData.Level);
				if(pstLevelUP)
				{
					if(pstCSMachineWarInfo->VCamp == pstPworldPlayer->stOnline.stWarOL.cCampIdx)
					{
						llExp = (tdr_ulonglong)100*pstLevelUP->ContributeExp/10000.0;
					}
					else
					{
						llExp = (tdr_ulonglong)80*pstLevelUP->ContributeExp/10000.0;
					}

					player_world_level_exp(pstEnv, pstPworldPlayer, &llExp);
					player_add_exp(pstEnv, pstPworldPlayer, llExp, 0);
					package_add_money( pstEnv,  pstPworldPlayer, ATTR_ID_LIFT_BRAND, 100);
					pstPworldPlayer->stOnline.stWarOL.iTotalExp += llExp;

					pstCSMachineWarInfo->EndExp = llExp;
				}
			}

			pstCSMachineWarInfo->ToTalExp = pstPworldPlayer->stOnline.stWarOL.iTotalExp;
			pstCSMachineWarInfo->SelfCamp = pstPworldPlayer->stOnline.stWarOL.cCampIdx;
			z_cltmsg_send(pstEnv, pstPworldPlayer, &stPkg);
		}
	}
	
	return 0;
}

int machine_war_update_leader(ZONESVRENV * pstEnv, ZONEPWORLDINST *pstPworldInst)
{
	int i, iCount = 0, iTitanLeader = 0, iMonsterLeader = 0;
	Player *pstPlayer, *apstPlayer[MAX_PWORLD_MEMBER];;
	CSPKG stPkg;
	CSPOLYPWORLDSVR *pstPolySvr = &stPkg.Body.PolyPworldSvr;
	
	for (i=0; i<pstPworldInst->iPlayer; i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i]);
		if (!pstPlayer)
		{
			continue;
		}

		apstPlayer[iCount++] = pstPlayer;
			
		if(pstPlayer->stOnline.stWarOL.cCampIdx == MACHINE_WAR_CAMP_TITAN &&
			pstPlayer->stOnline.stWarOL.fLeader  == 1)
		{
			iTitanLeader = 1;
			pstPolySvr->SvrData.BossPosition.Titan = pstPlayer->stRoleData.Pos;
		}

		if(pstPlayer->stOnline.stWarOL.cCampIdx == MACHINE_WAR_CAMP_MONSTER &&
			pstPlayer->stOnline.stWarOL.fLeader  == 1)
		{
			iMonsterLeader = 1;
			pstPolySvr->SvrData.BossPosition.Machine = pstPlayer->stRoleData.Pos;
		}
	}

	if(iTitanLeader == 0)
	{
		machine_war_rand_create_leader(pstEnv, pstPworldInst, MACHINE_WAR_CAMP_TITAN);
	}

	if(iMonsterLeader == 0)
	{
		machine_war_rand_create_leader(pstEnv, pstPworldInst, MACHINE_WAR_CAMP_MONSTER);
	}

	if(iTitanLeader && iMonsterLeader)
	{
		pstPolySvr->SvrType = POLY_SVR_TYPE_BOSS_POSITION;
		Z_CSHEAD_INIT(&stPkg.Head, POLY_PWORLD_SVR);
		z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iCount, &stPkg);
	}

	return 0;
}

int machine_war_human_tick(ZONESVRENV * pstEnv, ZONEPWORLD *pstPworld, ZONEPWORLDINST *pstPworldInst)
{
	int i;
	int iHuman = pstPworldInst->stPworldCamp.stCampInfo[0].iPlayerNum;
	Player *pstPlayer;
	tdr_longlong llExp;
	LEVELUP *pstLevelUP;
	
	if(pstPworldInst->tStart + pstPworld->stPworldDef.ReadyTime + 10 > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return 0;
	}

	for (i=0; i<pstPworldInst->iPlayer; i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i]);
		if (!pstPlayer)
			continue;

		if(pstPlayer->stOnline.stWarOL.cCampIdx != MACHINE_WAR_CAMP_HUMAN)
		{
			continue;
		}

		if(pstPlayer->stOnline.stWarOL.iAwardTime == 0 ||
			pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.stWarOL.iAwardTime >= 10)
		{
			pstPlayer->stOnline.stWarOL.iAwardTime = pstEnv->pstAppCtx->stCurr.tv_sec;

			pstLevelUP = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
			if(pstLevelUP)
			{
				llExp = (tdr_ulonglong )pstLevelUP->ContributeExp*5/100000*(160-iHuman)/55;
				player_add_exp(pstEnv, pstPlayer, llExp, 0);
				pstPlayer->stOnline.stWarOL.iTotalExp += llExp;
			}
		}
	}

	return 0;
}

int machine_war_pworld_tick(ZONESVRENV * pstEnv, ZONEPWORLD *pstPworld, ZONEPWORLDINST *pstPworldInst, int iSec)
{
	if(pstPworldInst->bDelayScript & PWORLD_SCRIPT_PROC_REAL_START_MAP)
	{
		return 0;
	}
	
	machine_war_update_leader(pstEnv, pstPworldInst);
	machine_war_human_tick(pstEnv, pstPworld, pstPworldInst);
		
	if(iSec % 5 == 0)
	{
		machine_war_pworld_info(pstEnv, pstPworld, pstPworldInst, NULL, 0);
	}
	
	return 0;
}

int poly_machine_war_real_start(ZONESVRENV * pstEnv, ZONEPWORLD *pstPworld, ZONEPWORLDINST *pstPworldInst)
{
	int i, iPlayer, iIdx;
	int fLeader;
	int aiPlayerID[MAX_PWORLD_MEMBER];
	MachineWarCampInfo sWarCamp;
	Player *pstPlayer;
	MapIndex * pstMapIdx;
	ZONEMAPINST *pstMapInst;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPworldInst->astMapInstIdx[0]);
	if( NULL == pstMapInst )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_find_map_inst %d map inst fail", pstPworldInst->astMapInstIdx[0].iID);
		goto _error;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		goto _error;
	}	
	
	if(pstPworldInst->iPlayer < 10)
	{
		//todo: 人数不足关闭副本
		goto _error;
	}

	sWarCamp.iTitan = pstPworldInst->iPlayer*0.1;
	if(sWarCamp.iTitan < 1)
	{
		goto _error;
	}

	sWarCamp.iMonster = pstPworldInst->iPlayer*0.1;
	if(sWarCamp.iMonster< 1)
	{
		goto _error;
	}

	sWarCamp.iHuman = pstPworldInst->iPlayer - (sWarCamp.iTitan + sWarCamp.iMonster);
	if(sWarCamp.iHuman< 1)
	{
		goto _error;
	}

	iPlayer = pstPworldInst->iPlayer;
	memcpy(&aiPlayerID[0], &pstPworldInst->aiPlayerID[0], sizeof(aiPlayerID[0])*iPlayer);

	for(i=0; i<sWarCamp.iTitan; i++)
	{
		fLeader = 0;
		if(i == 0)
		{
			fLeader = 1;
		}
		iIdx = RAND1(iPlayer);
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, aiPlayerID[iIdx]);
		if(pstPlayer)
		{
			machine_war_change_camp(pstEnv, pstPlayer, pstPworldInst, MACHINE_WAR_CAMP_TITAN, fLeader);
			player_enter_map_add_machine(pstEnv, pstPlayer, pstMapIdx);
			z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE, 
				ZONEERR_BROADMAP313);
		}

		iPlayer--;
		if(iPlayer != iIdx)
		{
			aiPlayerID[iIdx] = aiPlayerID[iPlayer];
		}
	}

	for(i=0; i<sWarCamp.iMonster; i++)
	{
		fLeader = 0;
		if(i == 0)
		{
			fLeader = 1;
		}
		iIdx = RAND1(iPlayer);
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, aiPlayerID[iIdx]);
		if(pstPlayer)
		{
			machine_war_change_camp(pstEnv, pstPlayer, pstPworldInst, MACHINE_WAR_CAMP_MONSTER, fLeader);
			player_enter_map_add_machine(pstEnv, pstPlayer, pstMapIdx);
			z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE, 
				ZONEERR_BROADMAP314);
		}

		iPlayer--;
		if(iPlayer != iIdx)
		{
			aiPlayerID[iIdx] = aiPlayerID[iPlayer];
		}
	}

	z_sys_strf_camp_broadcast(pstEnv, pstMapInst, MACHINE_WAR_CAMP_HUMAN, SYS_MIDDLE2, 
		ZONEERR_BROADCAMPMAP301);
	z_sys_strf_camp_broadcast(pstEnv, pstMapInst, MACHINE_WAR_CAMP_TITAN, SYS_MIDDLE2, 
		ZONEERR_BROADCAMPMAP302);
	z_sys_strf_camp_broadcast(pstEnv, pstMapInst, MACHINE_WAR_CAMP_MONSTER, SYS_MIDDLE2, 
		ZONEERR_BROADCAMPMAP303);
	
	return 0;
_error:
	z_pworld_over(pstEnv, pstPworldInst);
	pstPworldInst->tEnd = pstEnv->pstAppCtx->stCurr.tv_sec;
	z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE2, ZONEERR_BROADMAP315);
	return 0;
}

int poly_pworld_real_start(ZONESVRENV * pstEnv, ZONEPWORLD *pstPworld, ZONEPWORLDINST *pstPworldInst)
{
	switch(pstPworld->stPworldDef.PolyType)
	{
		case PWORLD_POLY_DOUBLE_MONSTER:
			break;
		case PWORLD_POLY_MACHINE_WAR:
			return poly_machine_war_real_start(pstEnv, pstPworld, pstPworldInst);
			break;
		case PWORLD_POLY_TANK:

			break;
		default:
			break;
	}
	
	return 0;
}

int poly_pworld_end(ZONESVRENV * pstEnv, ZONEPWORLDINST *pstZonePworldInst)
{
	ZONEPWORLD *pstPworld = NULL;

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
	if (NULL == pstPworld)
	{
		return -1;
	}

	switch(pstPworld->stPworldDef.PolyType)
	{
		case PWORLD_POLY_DOUBLE_MONSTER:
			{	
				double_monster_pworld_info(pstEnv, pstPworld, pstZonePworldInst, NULL, 1);

				//停止副本刷怪等。
				z_pworld_over(pstEnv, pstZonePworldInst);
				pstZonePworldInst->tEnd = pstEnv->pstAppCtx->stCurr.tv_sec+10;
			}
			break;
		case PWORLD_POLY_MACHINE_WAR:
			{
				machine_war_pworld_info(pstEnv, pstPworld, pstZonePworldInst, NULL, 1);
				
				//停止副本刷怪等。
				z_pworld_over(pstEnv, pstZonePworldInst);
				pstZonePworldInst->tEnd = pstEnv->pstAppCtx->stCurr.tv_sec+10;
			}
			break;
		case PWORLD_POLY_TANK:
			{
				ploy_tank_pworld_info(pstEnv, pstPworld,pstZonePworldInst, NULL, 1);
				z_pworld_over(pstEnv, pstZonePworldInst);
				pstZonePworldInst->tEnd = pstEnv->pstAppCtx->stCurr.tv_sec+10;
			}
			break;
		default:
			break;
	}

	switch(pstPworld->stPworldDef.PworldID)
	{
		case PWORLD_LIUYI:
			{	
				pworld_liuyi_player_exp(pstEnv, pstZonePworldInst, 1, 1);
				pstZonePworldInst->tEnd = pstEnv->pstAppCtx->stCurr.tv_sec+10;
			}
			break;
		case PWORLD_LIUYI2:
			{	
				pworld_liuyi_player_exp(pstEnv, pstZonePworldInst, 2, 1);
				pstZonePworldInst->tEnd = pstEnv->pstAppCtx->stCurr.tv_sec+10;
			}
			break;
		default:
			break;
	}

	
	return 0;
}

int poly_pworld_tick(ZONESVRENV * pstEnv, ZONEPWORLDINST *pstPworldInst, int iSec)
{
	ZONEPWORLD *pstPworld = NULL;

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if (NULL == pstPworld)
	{
		return -1;
	}

	//准备时间结束进入的脚本事件
	if(pstPworld->stPworldDef.ReadyTime + pstPworldInst->tStart <= pstEnv->pstAppCtx->stCurr.tv_sec &&
		pstPworldInst->bDelayScript & PWORLD_SCRIPT_PROC_REAL_START_MAP)
	{
		char szName[128];

		poly_pworld_real_start(pstEnv, pstPworld, pstPworldInst);
		
		pstPworldInst->bDelayScript &= ~PWORLD_SCRIPT_PROC_REAL_START_MAP;
			
		stSEnv.pstMonster = NULL;
		stSEnv.pstNpc = NULL;
		stSEnv.pstPlayer = NULL;
		stSEnv.pstPworldInst = pstPworldInst;
		stSEnv.pstKiller = NULL;
		stSEnv.pstMapinst = NULL;

		snprintf(szName, sizeof(szName), "pworld%d.mac", pstPworldInst->stPworldIdx.iID);
		szName[sizeof(szName) - 1] = 0;
		
		z_exec_script(pstEnv->pstAppCtx, pstEnv, NULL, szName, &pstPworldInst->stScriptIdx, PWORLD_SCRIPT_PROC_REAL_START);
	}

	if(pstPworldInst->bOver)
	{
		return 0;
	}

	switch(pstPworld->stPworldDef.PolyType)
	{
		case PWORLD_POLY_DOUBLE_MONSTER:
			double_monster_pworld_tick(pstEnv, pstPworld, pstPworldInst, iSec);
			break;
		case PWORLD_POLY_MACHINE_WAR:
			machine_war_pworld_tick(pstEnv, pstPworld, pstPworldInst, iSec);
			break;
		case PWORLD_POLY_SPARTA:
			sparta_pworld_tick(pstEnv, pstPworld, pstPworldInst, iSec);
			break;
		case PWORLD_POLY_BATTLE:
			battle_pworld_tick(pstEnv, pstPworld, pstPworldInst, iSec);
			break;
		case PWORLD_POLY_DIANFENG:
			dianfeng_pworld_tick(pstEnv, pstPworld, pstPworldInst, iSec);
			break;
		case PWORLD_POLY_TANK:
			ploy_tank_pworld_tick( pstEnv, pstPworld, pstPworldInst, iSec);
			break;
		default:
			break;
	}
	
	return 0;	
}

int poly_pworld_player_die(ZONESVRENV * pstEnv, Player *pstPlayer, ZoneAni *pstAtker)
{
	ZONEPWORLDINST *pstZonePworldInst;
	ZONEMAPINST *pstMapInst;
	ZONEIDX stIdx;
	tdr_longlong llExp;
	LEVELUP *pstLevelUP;
	ZONEPWORLD *pstPworld = NULL;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (NULL == pstZonePworldInst)
	{
		return -1;
	}
	
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
	if (NULL == pstPworld)
	{
		return -1;
	}

	switch(pstPworld->stPworldDef.PolyType)
	{
		case PWORLD_POLY_DOUBLE_MONSTER:
			
			return 0;
			break;
		case PWORLD_POLY_TANK:
			poly_tank_pworld_player_die( pstEnv, pstPlayer, pstAtker);
			break;
		case PWORLD_POLY_MACHINE_WAR:
			{
				Monster *pstMachine;
				Player *pstAtkerPlayer;
				int fLeader;
				
				if(pstAtker->iType != OBJ_MONSTER || !is_mon_machine(pstAtker->stObj.pstMon))
				{
					return 0;
				}

				fLeader = pstPlayer->stOnline.stWarOL.fLeader;

				pstMachine = pstAtker->stObj.pstMon;
				pstAtkerPlayer = machine_get_driver(pstEnv, pstMachine);
				if(!pstAtkerPlayer || is_human_player(pstAtkerPlayer))
				{
					return 0;
				}

				pstLevelUP = z_get_levelup(pstEnv, pstAtkerPlayer->stRoleData.Level);
				if(pstLevelUP)
				{
					llExp = pstLevelUP->ContributeExp/10000;
					player_add_exp(pstEnv, pstAtkerPlayer, llExp, 0);
					pstAtkerPlayer->stOnline.stWarOL.iTotalExp += llExp;
				}

				machine_war_change_camp(pstEnv, pstPlayer, 
					pstZonePworldInst, pstAtkerPlayer->stOnline.stWarOL.cCampIdx, 0);

				if(fLeader)
				{
					machine_war_update_leader(pstEnv, pstZonePworldInst);
				}

				if(pstPlayer->stOnline.stWarOL.cCampIdx == MACHINE_WAR_CAMP_TITAN)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
						ZONEERR_PWORLD201);
				}
				else if(pstPlayer->stOnline.stWarOL.cCampIdx == MACHINE_WAR_CAMP_MONSTER)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
						ZONEERR_PWORLD202);
				}
			}
			break;
		case PWORLD_POLY_DIANFENG:
			dianfeng_player_pworld_die_dispose(pstEnv,pstPlayer);
			break;
		default:
			break;
	}

	return 0;
}

int poly_pworld_mon_die(ZONESVRENV * pstEnv, Monster *pstMon, Player *pstPlayer)
{
	ZONEPWORLDINST *pstZonePworldInst;
	ZONEMAPINST *pstMapInst;
	ZONEIDX stIdx;
	ZONEPWORLD *pstPworld = NULL;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (NULL == pstZonePworldInst)
	{
		return -1;
	}
	
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
	if (NULL == pstPworld)
	{
		return -1;
	}

	switch(pstPworld->stPworldDef.PolyType)
	{
		case PWORLD_POLY_DOUBLE_MONSTER:
			//复用一下这个值
			pstPlayer->stOnline.stWarOL.iKillNum++;
			return 0;
			break;
		case PWORLD_POLY_MACHINE_WAR:
			return -1;
			break;
		case PWORLD_POLY_TANK:
			 poly_tank_pworld_mon_die( pstEnv,  pstZonePworldInst,  pstPlayer);
			break;
		default:
			break;
	}

	return 0;
}

int plaer_map_machine_id_get(MapIndex* pstMapIdx, Player *pstPlayer)
{
	if(pstPlayer->stOnline.stWarOL.cCampIdx < 0 && 
		pstPlayer->stOnline.stWarOL.cCampIdx > MAP_MACHINE_CAMP_NUM)
	{
		return -1;
	}

	if(pstPlayer->stOnline.stWarOL.fLeader)
	{
		if(pstPlayer->stOnline.stWarOL.cCampIdx == MACHINE_WAR_CAMP_TITAN)
		{
			return 1463004;
		}
		else if(pstPlayer->stOnline.stWarOL.cCampIdx == MACHINE_WAR_CAMP_MONSTER)
		{
			return 1463002;
		}
	}

	return pstMapIdx->stMapDef.MapMachine[pstPlayer->stOnline.stWarOL.cCampIdx-1];
}


int player_enter_map_add_machine(ZONESVRENV * pstEnv, Player *pstPlayer, MapIndex* pstMapIdx)
{
	int iMachineID;
	Monster *pstMachine;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if(!pstMapInst )
	{
		return -1;
	}
		
	if(pstMapIdx->stMapDef.MapMachine[0] == 0)
	{
		return -1;
	}

	iMachineID = plaer_map_machine_id_get(pstMapIdx, pstPlayer);
	if(iMachineID < 0)
	{
		return -1;
	}
	
	if(is_player_ride_on_machine(pstPlayer))
	{
		Monster *pstMachine = machine_get_by_player(pstEnv, pstPlayer);
		//同地图传送的时候不清理机械
		if(pstMachine && pstMachine->iDefIdx == iMachineID)
		{
			return 0;
		}
		else
		{
			player_ride_machine_brk(pstEnv, pstPlayer, 1);
		}
	}

	//产出一个空燃料空弹药的机械
	pstMachine = new_mon_machine(pstEnv, pstMapInst, iMachineID, &pstPlayer->stRoleData.Pos, MACHINE_OWNER_SELF, (void *)pstPlayer, 0);
	if(pstMachine == NULL)
	{
		return -1;
	}
	
	z_machine_attr_chg(pstEnv, pstMachine, MON_MACHINE_ATTR_ENERGY, this_machine(pstEnv, pstMachine)->stMachineInfo.MaxEnergy);
	
	//乘坐
	z_ride_on_machine(pstEnv, pstPlayer, pstMachine, machine_get_free_seatidx(pstEnv,pstMachine), 0);

	return 0;
}

int player_leave_map_machine_clear(ZONESVRENV * pstEnv, MapIndex *pstMapIndex, Player *pstPlayer)
{
	if(pstMapIndex->stMapDef.MapMachine[0] == 0)
	{
		return -1;
	}

	if(is_player_ride_on_machine(pstPlayer))
	{
		player_ride_machine_brk(pstEnv, pstPlayer, 1);
	}
	
	return 0;
}

int machine_war_camp_broadcast_except(ZONESVRENV *pstEnv, ZONEMAPINST* pstMapInst, int iCampIdx, CSPKG *pstPkg, Player *pstExcept)
{
	int i = 0, iCount = 0;
	Player *apstPlayers[MAX_PWORLD_MEMBER];
	ZONEPWORLDINST* pstPworldInst = NULL;

	pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if(NULL == pstPworldInst)
		return -1;

	bzero(apstPlayers, sizeof(apstPlayers));
	i = pstPworldInst->iPlayer;
	while(i--)
	{
		Player *pstElement = z_id_player(pstEnv->pstAppCtx, pstEnv,
		                                 pstPworldInst->aiPlayerID[i]);
		if(NULL == pstElement || pstElement == pstExcept)
		{
			continue;
		}

		if(pstElement->stOnline.stWarOL.cCampIdx == iCampIdx)
		{
			apstPlayers[iCount++] = pstElement;
		}
	}

	if(iCount > 0)
	{
		if(z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
		                            iCount, pstPkg) < 0)
		{
			return -1;
		}
	}
	return iCount;
}

int machine_war_camp_broadcast(ZONESVRENV *pstEnv, ZONEMAPINST * pstMapInst, int iCampIdx, CSPKG *pstPkg)
{
	return machine_war_camp_broadcast_except(pstEnv, pstMapInst, iCampIdx, pstPkg, NULL);
}

int z_sys_strf_camp_broadcast(ZONESVRENV *pstEnv, ZONEMAPINST * pstMapInst, int iCampIdx, int iSysType, int iErrno, ...)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	const char* pszErr;
	va_list marker;

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
	machine_war_camp_broadcast(pstEnv, pstMapInst, iCampIdx, &stPkg);

	return 0;
}

int pworld_liuyi_monster_total(ZONESVRENV * pstEnv, ZONEPWORLDINST *pstPworldInst, int *piMonster1, int *piMonster2, int *piMonster3)
{
	int i;
	ZONEMAPINST *pstMapInst;

	*piMonster1 = 0;
	*piMonster2 = 0;
	*piMonster3 = 0;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPworldInst->astMapInstIdx[0]);
	if( NULL == pstMapInst )
	{
		return -1;
	}

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
			Monster* pstMonster = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstArea->aiAnimate[k]);
			
			if(NULL == pstMonster || MONSTER_DIE == pstMonster->cStat)
			{
				continue;
			}

			if(pstMonster->iDefIdx == 230052)
			{
				*piMonster1+=1;
			}
			else if(pstMonster->iDefIdx == 230053)
			{
				*piMonster2+=1;
			}
			else if(pstMonster->iDefIdx == 230054)
			{
				*piMonster3+=1;
			}
		}
	}

	return 0;
}

long long  pworld_liuyi_player_exp(ZONESVRENV * pstEnv,ZONEPWORLDINST *pstPworldInst,int type, int fEnd)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;

	Player *pstPworldPlayer;
	LEVELUP *pstLevelUP;
	tdr_longlong llExp = 0;
	char szMsg[256];
	int i,isSucc=0;		
	int monster1,monster2,monster3;
	
	pstSysMsg->Type = SYS_POP_WINDOW;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
	
	pworld_liuyi_monster_total(pstEnv, pstPworldInst, 
			&monster1, &monster2,&monster3);

	if(monster1 > 0 && monster2 == 0 && monster3 == 0)
	{
		isSucc = 1;
	}
	else if(monster2 > 0 && monster1 == 0 && monster3 == 0)
	{
		isSucc = 1;
	}
	else if(monster3 > 0 && monster1 == 0 && monster2 == 0)
	{
		isSucc = 1;
	}

	if(fEnd)
	{
		pstPworldInst->bOver= 1;
	}
	else if(!isSucc)
	{
		return 0;
	}
	for (i=0; i< pstPworldInst->iPlayer; i++)
	{
		pstPworldPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i]);
		if (!pstPworldPlayer)
			continue;
		
		pstLevelUP = z_get_levelup(pstEnv, pstPworldPlayer->stRoleData.Level);
		if(pstLevelUP)
		{
			if(isSucc)
			{
				if(type == 1)
				{
					llExp = (tdr_ulonglong)100*pstLevelUP->ContributeExp/10000.0;
				}
				else
				{
					llExp = (tdr_ulonglong)200*pstLevelUP->ContributeExp/10000.0;
				}
				snprintf(szMsg,sizeof(szMsg),LJY_SYSMSG22,llExp);	
			}
			else
			{
				if(type == 1)
				{
					llExp = (tdr_ulonglong)50*pstLevelUP->ContributeExp/10000.0;
				}
				else
				{
					llExp = (tdr_ulonglong)10*pstLevelUP->ContributeExp/10000.0;
				}
				snprintf(szMsg,sizeof(szMsg),LJY_SYSMSG23,llExp);	
			}
			
			player_add_exp(stSEnv.pstEnv, pstPworldPlayer, llExp, 0);

			if(isSucc)
			{
				task_liuyi_fini_trace(pstEnv, pstPworldPlayer);
			}
			//弹出提示
			STRNCPY(pstSysMsg->Msg, szMsg, sizeof(szMsg));
			
			Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
			z_cltmsg_send(pstEnv, pstPworldPlayer, pstPkg);
			
			
		}
	}
	//停止副本刷怪等。
	z_pworld_over(pstEnv, pstPworldInst);
	return llExp;
}

