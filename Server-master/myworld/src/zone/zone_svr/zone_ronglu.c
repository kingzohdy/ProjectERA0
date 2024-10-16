

#include "zone_ronglu.h"
#include "zone_task.h"
#include "zone_package.h"
#include "zone_map.h"
#include "zone_npc.h"
#include "zone_clt.h"
#include "zone_move.h"
#include "zone_err.h"

//#define RONGLU_CALL_TOTEM 1
//#define RONGLU_CALL_COLLECT 2
//#define RONGLU_COLLECT_END 3

//#define RONGLU_RESET_USE 123456
#define RONGLU_DIST 5000

RONGLUDEF* ronglu_def_find(ZONESVRENV* pstEnv, int iItemID)
{
	RONGLUDEF stDef;
	int iEqu, iIdx;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.ItemID = iItemID;

	iIdx = bsearch_int(&stDef, pstObj->sRongluDef, pstObj->iRongluDef, RONGLU_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (RONGLUDEF *)&pstObj->sRongluDef[iIdx];
	}
	return NULL;
}

RONGLUDEF* ronglu_get_rand(ZONESVRENV* pstEnv)
{
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	int i;
	int iRate;
	RONGLUDEF *pstDef= NULL;
	int iRateCount = 0;

	iRate = RAND1(10000);
	for (i=0; i<pstObj->iRongluDef; i++)
	{
		pstDef = (RONGLUDEF*)&pstObj->sRongluDef[i];	
		iRateCount += pstDef->Rate;
		if (iRate < iRateCount)
		{
			return pstDef;
		}
	}

	return NULL;
}

int ronglu_check_pre(ZONESVRENV* pstEnv)
{
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	int i;
	int iCount = 0;
	RONGLUDEF *pstDef= NULL;
	
	for (i=0; i<pstObj->iRongluDef; i++)
	{
		pstDef = (RONGLUDEF*)&pstObj->sRongluDef[i];	
		iCount += pstDef->Rate;
	}

	if (iCount != 10000)
	{
		printf("ronglu_check_pre is fail!\n");
		return -1;
	}
	
	return 0;
}

// 接任务掉的脚本
int ronglu_get_task(ZONESVRENV* pstEnv, Player *pstPlayer, int iTaskID)
{
	TASKDEF *pstDef;
	RONGLUDEF *pstRongluDef;
	ROLEGRID stRoleGrid;
	RONGLUINFO *pstRongluInfo = &pstPlayer->stRoleData.MiscInfo.RongluInfo;
	int iLvl = pstPlayer->stRoleData.Level;

	pstDef = task_def_find(pstEnv, iTaskID, NULL);
	if (!pstDef || pstDef->Type != TASK_TYPE_RONGLU)
	{
		return -1;
	}

	pstRongluDef = ronglu_get_rand(pstEnv);
	if (!pstRongluDef)
		return -1;

	if (0 == item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstRongluDef->ItemID) )
	{
		stRoleGrid.GridData.RoleItem.Num = 1;
		//stRoleGrid.GridData.RoleItem.InstVal1 = iTaskID;
	}
	else
	{
		return -1;
	}

	 // 接任务之前要test
	package_add(pstEnv->pstAppCtx , pstEnv , pstPlayer, &stRoleGrid, 1,0, 0);

	memset(pstRongluInfo, 0, sizeof(pstRongluInfo));
	pstRongluInfo->ItemID = pstRongluDef->ItemID;
	pstRongluInfo->TaskID = iTaskID;
	pstRongluInfo->Lvl  = iLvl/10;
	/*if (iLvl%10==0 && iLvl == pstEnv->pstConf->MaxLevel)
	{
		pstRongluInfo->Lvl  = (iLvl-1)/10;
	}*/
	
	return 0;
}



Monster* ronglu_new_mon(ZONESVRENV* pstEnv, Player *pstPlayer , ZONEMAPINST *pstMapInst, 
								MapIndex *pstMapIdx, int iItemID, int iMonID, int iLeftTime, int iDynLvl, int iDynGrade)
{
	
	RONGLUINFO *pstRongluInfo = &pstPlayer->stRoleData.MiscInfo.RongluInfo;
	Monster *pstMon; 
	RONGLUDEF *pstRongluDef;
	int iGrade = pstRongluInfo->Lvl;
	Monster stMonster;
	MONSTERDEF *pstMonDef;
	Monster *pstFind;
	RESPOS stPos;
	MONDYNATTRDEF *pstMonDynAttr;
	

	pstRongluDef = ronglu_def_find(pstEnv, iItemID);
	if (!pstRongluDef || iGrade >= RONGLU_GRADE_MAX)
	{
		return NULL;
	}

	if (pstRongluDef->Grade[iGrade].MapID != (int)pstMapIdx->stMapDef.MapID &&
		pstRongluDef->Grade[iGrade].MapID1!= (int)pstMapIdx->stMapDef.MapID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_RONGLU1);
		/*MapIndex *pstMapIdx1 =  z_mappos_index(pstEnv->pstMapObj, NULL, pstRongluDef->Grade[iGrade].MapID);
		if (pstMapIdx1)
		{
			z_sys_strf(pstEnv,  pstPlayer, SYS_MIDDLE2, "请到 %s 使用此道具!",
					pstMapIdx1->stMapDef.Name);
		}*/
		return NULL;
	}

	// 自己的采集怪存在,不能招了
	pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstRongluInfo->ColletID);
	if (pstMon)
	{
		if (strcmp(pstMon->szOwner, pstPlayer->stRoleData.RoleName) == 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_RONGLU2);
			return NULL;
		}
	}

	if (pstRongluInfo->ItemID != iItemID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_RONGLU3);
		return NULL;
	}

	// 看看前面招的东东还在不在
	pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstRongluInfo->TutemID);
	if (pstMon)
	{
		if (strcmp(pstMon->szOwner, pstPlayer->stRoleData.RoleName) == 0)
		{
			z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
			z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);
			pstRongluInfo->TutemID = 0;
		}
	}


	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos=	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.tRefresh	=	pstEnv->pstAppCtx->stCurr.tv_sec;

	stMonster.iOwnerID = pstPlayer->iMemID;
	mon_set_pkinfo(&stMonster, MON_PK_TYPE_PLAYER, pstPlayer->iMemID);
	snprintf(stMonster.szOwner,sizeof(stMonster.szOwner),"%s",pstPlayer->stRoleData.RoleName);
	stMonster.szOwner[sizeof(stMonster.szOwner) - 1] = 0;
	
	pstMonDef = z_get_mon_def(pstEnv, &stMonster);
	if (NULL == pstMonDef || pstMonDef->SubType != MON_SUB_TYPE_RONGLU)
	{
		return NULL;
	}

	pstMonDynAttr = z_find_mondyn(pstEnv, iDynLvl);
	
	z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstPlayer->stRoleData.Pos, &pstPlayer->stRoleData.Pos, 400, 400, &stPos);

	stMonster.bDirection = RAND1(180);
	stMonster.stInitPos = stPos;
	pstFind =	z_new_monster(pstEnv->pstAppCtx, pstEnv, &stMonster, pstMonDef);
	if( !pstFind )
		return NULL;

	if (pstMonDynAttr && iDynGrade < MAX_DYN_GRADE)
	{
		pstFind->stMonDynAttr.Level = pstMonDynAttr->Level;
		pstFind->stMonDynAttr.HPMul = pstMonDynAttr->DynHPMul[iDynGrade];
		pstFind->stMonDynAttr.DynHurtMul = pstMonDynAttr->DynHurtMul[iDynGrade];
		pstFind->stMonDynAttr.IceHurtMul = pstMonDynAttr->IceHurtMul;
		pstFind->stMonDynAttr.PhsicHurtMul = pstMonDynAttr->PhsicHurtMul;
	}

	pstFind->iCommType = MON_UNION_TYPE_RONGLU;
	pstFind->stCommUnion.stRonglu.iPower = 0;
	pstFind->stCommUnion.stRonglu.iMaxPower = pstRongluDef->Grade[iGrade].KillNum;
	memcpy(pstFind->stCommUnion.stRonglu.MonIDs, pstRongluDef->Grade[iGrade].MonIDs, 
			sizeof(pstFind->stCommUnion.stRonglu.MonIDs));
	STRNCPY(pstFind->stCommUnion.stRonglu.OwnerName, 
			pstPlayer->stRoleData.RoleName, sizeof(pstFind->stCommUnion.stRonglu.OwnerName));

	pstFind->cFirstBorn = 1;
	if(iLeftTime > 0)
	{
		pstFind->tLifeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + iLeftTime;
	}
	if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstFind, NULL))
	{
		tmempool_free(pstEnv->pstMonsterPool, pstFind->iMIndex);
		return NULL;
	}

	pstRongluInfo->TutemID =pstFind->iID;
	pstFind->cRonglu = 1;
	pstFind->cFirstBorn = 0;
	
	return pstFind;
}


int ronglu_player_new_mon(ZONESVRENV* pstEnv, Player *pstPlayer ,int iItemID, 
								int iMonID, int iLeftTime, int iDynLvl, int iDynGrade)
{
	ZONEMAPINST *pstMInst;
	ZONEIDX stIdx;
	MapIndex *pstMapIdx;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = -1;
	pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (!pstMInst)
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMInst->iMapPos, pstMInst->iMapInstID);
	if (!pstMapIdx)
	{
		return -1;
	}
	
	ronglu_new_mon(pstEnv, pstPlayer , pstMInst, pstMapIdx, iItemID, iMonID, iLeftTime, iDynLvl, iDynGrade);

	return 0;
}


Monster* ronglu_new_collect_mon(ZONESVRENV* pstEnv, Player *pstPlayer , ZONEMAPINST *pstMapInst, 
								MapIndex *pstMapIdx, int iMonID, int iLeftTime)
{
//	int i;
	Monster stMonster;
	Monster *pstFind;
	MONSTERDEF *pstMonDef;
	Monster *pstMon;
	RONGLUINFO *pstRongluInfo = &pstPlayer->stRoleData.MiscInfo.RongluInfo;
	RESPOS stPos;
	
	pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstRongluInfo->TutemID);
	if (!pstMon || 
		pstMon->iCommType != MON_UNION_TYPE_RONGLU || 
		strcmp(pstMon->szOwner, pstPlayer->stRoleData.RoleName))
	{
		return NULL;
	}

	stPos = pstMon->stCurrPos;

	// 释放熔炉图腾柱
	z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
	z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);
	pstRongluInfo->TutemID = 0;
	
	memset(&stMonster, 0, sizeof(stMonster));
	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.stInitPos = stPos;
	stMonster.cRefresh	= 0;
	stMonster.bDirection = RAND1(180);
	stMonster.stCurrPos = stPos;
	STRNCPY(stMonster.szOwner, pstPlayer->stRoleData.RoleName, sizeof(stMonster.szOwner));
	
	pstMonDef = z_get_mon_def(pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		return NULL;
	}
	
	pstFind =	z_new_monster(pstEnv->pstAppCtx, pstEnv, &stMonster, pstMonDef);

	if( !pstFind )
	{
		return NULL;
	}

	pstFind->iPatrolID = 0;
	if (iLeftTime > 0)
	{
		pstFind->tLifeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + iLeftTime;
	}

	pstFind->cFirstBorn = 1;
	if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstFind,NULL))
	{
		tmempool_free(pstEnv->pstMonsterPool, pstFind->iMIndex);
		return NULL;
	}
	
	pstFind->cFirstBorn = 0;
	pstFind->cRonglu = 1;
	pstFind->cRefresh = 0;	// 不刷新
	pstRongluInfo->ColletID = pstFind->iID;

	return pstFind;
}


int ronglu_player_new_collect_mon(ZONESVRENV* pstEnv, Player *pstPlayer, int iMonID, int iLeftTime)
{
	ZONEMAPINST *pstMInst;
	ZONEIDX stIdx;
	MapIndex *pstMapIdx;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = -1;
	pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (!pstMInst)
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMInst->iMapPos, pstMInst->iMapInstID);
	if (!pstMapIdx)
	{
		return -1;
	}

	ronglu_new_collect_mon(pstEnv, pstPlayer , pstMInst, pstMapIdx, iMonID, iLeftTime);
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_RONGLU4);
	return 0;
}


int ronglu_kill_mon_booty(ZONESVRENV* pstEnv, Player *pstPlayer,  Monster *pstKillMon)
{
	RONGLUINFO *pstRongluInfo = &pstPlayer->stRoleData.MiscInfo.RongluInfo;
	int iMonID = pstKillMon->iDefIdx;
	int iGrade = pstRongluInfo->Lvl;

	Monster *pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstRongluInfo->TutemID);
	MonRonglu *pstRonglu;
	int i;
	RONGLUDEF *pstRongluDef;

	if (	!pstMon || 
		pstMon->iCommType != MON_UNION_TYPE_RONGLU || 
		strcmp(pstMon->szOwner, pstPlayer->stRoleData.RoleName))
		return 0;
	
	pstRongluDef = ronglu_def_find(pstEnv, pstRongluInfo->ItemID);
	if (!pstRongluDef || iGrade >= RONGLU_GRADE_MAX)
	{
		return 0;
	}

	if (pstKillMon->stMap.iID != pstMon->stMap.iID)
	{
		return 0;
	}

	if (z_distance(&pstMon->stCurrPos, &pstKillMon->stCurrPos) > RONGLU_DIST)
	{
		return 0;
	}
	
	pstRonglu = &pstMon->stCommUnion.stRonglu;

	if (pstRonglu->iPower < pstRonglu->iMaxPower)
	{
		for (i=0; i<RONGLU_KILL_MON_MAX; i++)
		{
			if (pstRonglu->MonIDs[i] != 0 && pstRonglu->MonIDs[i] == iMonID )
			{
				pstRonglu->iPower ++;		
				// 通知客户端,怪物视野广播
				ronglu_killnum_chg(pstEnv, pstPlayer, pstRonglu->iPower, pstMon, pstKillMon, 0);
				break;
			}
		}
	}
	return 0;
}

int ronglu_is_full(Monster *pstMon)
{
	if (pstMon->stCommUnion.stRonglu.iPower >= pstMon->stCommUnion.stRonglu.iMaxPower)
	{
		return 1;
	}
	return 0;
}

int ronglu_get_task_id(Player *pstPlayer)
{
	return pstPlayer->stRoleData.MiscInfo.RongluInfo.TaskID;
}

int ronglu_get_item_id(Player *pstPlayer)
{
	return pstPlayer->stRoleData.MiscInfo.RongluInfo.ItemID;
}


int ronglu_one_chg(ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID, int iTaskID)
{
	CSPKG stPkg;
	RONGLUSVR*pstSvr = &stPkg.Body.RongluSvr;
	
	pstSvr->Cmd = RONGLU_SVR_OP_ONE;
	pstSvr->Data.One.ItemID =iItemID;
	pstSvr->Data.One.TaskID = iTaskID;
	pstSvr->Data.One.LvlGrade = pstPlayer->stRoleData.MiscInfo.RongluInfo.Lvl;
	
	Z_CSHEAD_INIT(&stPkg.Head, RONGLU_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int ronglu_player_login(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	RONGLUINFO *pstRongluInfo = &pstPlayer->stRoleData.MiscInfo.RongluInfo;
	
	if (pstRongluInfo->ItemID >0 && pstRongluInfo->TaskID > 0)
	{
		ronglu_one_chg(pstEnv, pstPlayer, pstRongluInfo->ItemID, pstRongluInfo->TaskID);
	}
	return 0;
}

int ronglu_killnum_chg(ZONESVRENV* pstEnv, Player *pstPlayer, int iNum, 
						Monster *pstRonglu,Monster *pstMon, int iType)
{
	CSPKG stPkg;
	RONGLUSVR*pstSvr = &stPkg.Body.RongluSvr;
	
	pstSvr->Cmd = RONGLU_SVR_OP_KILL_NUM_CHG;
	pstSvr->Data.Kill.Num = iNum;
	pstSvr->Data.Kill.MonMemID = pstRonglu->iID;
	pstSvr->Data.Kill.KillMonMemID = pstMon->iID;
	pstSvr->Data.Kill.Type = iType;
	
	Z_CSHEAD_INIT(&stPkg.Head, RONGLU_SVR);
	z_sendpkg_mon_area_view(pstEnv->pstAppCtx, pstEnv, pstMon, &stPkg);
	return 0;
}


int ronglu_start_res(ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID)
{
	CSPKG stPkg;
	RONGLUSVR*pstSvr = &stPkg.Body.RongluSvr;
	
	pstSvr->Cmd = RONGLU_SVR_OP_START_RES;
	pstSvr->Data.Start.ItemID = iItemID;
	
	Z_CSHEAD_INIT(&stPkg.Head, RONGLU_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int ronglu_task_fini(ZONESVRENV* pstEnv, Player *pstPlayer, TASKDEF *pstTaskDef)
{
	RONGLUINFO *pstRongluInfo = &pstPlayer->stRoleData.MiscInfo.RongluInfo;
	Monster *pstMon;
	
	if (pstTaskDef->Type != TASK_TYPE_RONGLU)
	{
		return 0;
	}

	package_dec(pstEnv->pstAppCtx,pstEnv,pstPlayer,pstRongluInfo->ItemID,1,LIST_TYPE_SUNDRIES,NULL,0,0);

	if (pstRongluInfo->TutemID > 0)
	{
		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstRongluInfo->TutemID);
		if (pstMon)
		{
			z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
			z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);	
		}
	}

	if (pstRongluInfo->ColletID > 0)
	{
		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstRongluInfo->ColletID);
		if (pstMon)
		{
			z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
			z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);	
		}	
	}

	memset(pstRongluInfo, 0, sizeof(*pstRongluInfo));
	return 0;
}

/*int ronglu_reset(ZONESVRENV* pstEnv, Player *pstPlayer, RONGLURESETCLT*pstReset)
{
	int iPos;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	ROLEITEM *pstItem;
	ITEMDEF *pstItemDef;
	RONGLUINFO *pstRongluInfo = &pstPlayer->stRoleData.MiscInfo.RongluInfo;
	TASKRUNONE *pstTaskOne;
	int iTaskID ;
	RONGLUDEF *pstRongluDef;
	ROLEGRID stRoleGrid;

	if (pstRongluInfo->ColletID > 0 || pstRongluInfo->TutemID > 0)
	{
		return -1;
	}
	
	iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, pstReset->GridIdx);
	if (0 > iPos)
	{
		return -1;
	}

	pstItem = &pstPak->SundriesPackage.RoleGrids[iPos].GridData.RoleItem;

	pstItemDef = z_find_itemdef(pstEnv, pstItem->DefID);
	if (!pstItemDef)
	{
		return -1;
	}

	iTaskID = pstRongluInfo->TaskID;

	pstTaskOne = task_run_find(pstPlayer, iTaskID);
	if (!pstTaskOne)
	{
		return -1;
	}

	if (pstTaskOne->TaskStat == TASK_STAT_FINI)
	{	
		z_sys_strf(pstEnv,pstPlayer,SYS_MIDDLE2,"任务已经完成!");
		return -1;
	}

	pstRongluDef =  ronglu_get_rand(pstEnv);
	if (!pstRongluDef)
	{		
		return -1;
	}	

	if (package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer,RONGLU_RESET_USE,1, 
							LIST_TYPE_SUNDRIES,NULL, 0) < 0)
	{
		ITEMDEF *pstItemDef1 = z_find_itemdef(pstEnv, RONGLU_RESET_USE);
		if (pstItemDef1)
		{
			z_sys_strf(pstEnv,pstPlayer,SYS_MIDDLE2,"缺少道具:%s", pstItemDef1->Name);
		}
		return -1;
	}

	if (package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstItemDef->ItemID,1, 
							LIST_TYPE_SUNDRIES,&iPos, 0) < 0)
	{
		return -1;
	}

	if (0 != item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstRongluDef->ItemID) )
	{
		return -1;
	}
	else
	{
		stRoleGrid.GridData.RoleItem.Num = 1;
		//stRoleGrid.GridData.RoleItem.InstVal1 = iTaskID;
	}

	if (package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstItemDef->ItemID,1,LIST_TYPE_SUNDRIES,&iPos, 0, 0) != 0)
	{
		return -1;
	}
	package_add(pstEnv->pstAppCtx , pstEnv , pstPlayer, &stRoleGrid, 1,0, 0);

	pstRongluInfo->ItemID = pstItemDef->ItemID;

	// 通知客户端熔炉变了
	ronglu_one_chg(pstEnv, pstPlayer, pstRongluDef->ItemID, iTaskID);
	return 0;
}*/

int ronglu_mon_is_exist(ZONESVRENV* pstEnv,Player *pstPlayer, int iMonID)
{
	Monster *pstMon;

	if (iMonID <= 0)
		return 0;

	pstMon = z_id_monster(pstEnv->pstAppCtx,  pstEnv, iMonID);
	if (!pstMon)
		return 0;

	if (strcmp(pstMon->szOwner, pstPlayer->stRoleData.RoleName) == 0)
	{
		return 1;
	}
	return 0;
}

int ronglu_reset_start(ZONESVRENV* pstEnv, Player *pstPlayer,  RESETSTARTCLT *pstStart)
{
	int iPos = -1;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	ROLEITEM *pstItem;
	ITEMDEF *pstItemDef;
	RONGLUINFO *pstRongluInfo = &pstPlayer->stRoleData.MiscInfo.RongluInfo;
	TASKRUNONE *pstTaskOne;
	int iTaskID ;
	RONGLUDEF *pstRongluDef;
	int iItemID = 0;
	int i;

	if (ronglu_mon_is_exist(pstEnv,pstPlayer, pstRongluInfo->ColletID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_RONGLU5);
		goto end;
	}

	if (ronglu_mon_is_exist(pstEnv,pstPlayer, pstRongluInfo->TutemID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_RONGLU6);
		goto end;
	}

	for (i=0; i<pstPak->SundriesPackage.Num; i++)
	{
		if (pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem.DefID == pstRongluInfo->ItemID)
		{
			iPos	 = i;
			break;
		}
	}

	if (iPos < 0)
		goto end;

	pstItem = &pstPak->SundriesPackage.RoleGrids[iPos].GridData.RoleItem;

	pstItemDef = z_find_itemdef(pstEnv, pstItem->DefID);
	if (!pstItemDef || pstItemDef->Result[0].ResultID != RESULT_RONGLU )
	{
		goto end;
	}

	pstRongluDef = ronglu_def_find( pstEnv,pstItemDef->ItemID);
	if (!pstRongluDef || pstRongluDef->Pinzhi == 4)
	{
		//z_sys_strf(pstEnv,pstPlayer,SYS_MIDDLE,"熔炉当前已是最高等级,不需要重铸!");
		goto end;
	}

	iTaskID = pstRongluInfo->TaskID;

	pstTaskOne = task_run_find(pstPlayer, iTaskID);
	if (!pstTaskOne)
	{
		goto end;
	}

	if (pstTaskOne->TaskStat == TASK_STAT_FINI)
	{	
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_RONGLU201);
		goto end;
	}

	pstRongluDef =  ronglu_get_rand(pstEnv);
	if (!pstRongluDef )
	{		
		goto end;
	}	

	if (package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer,RONGLU_RESET_USE,1, 
							LIST_TYPE_SUNDRIES,NULL, 0) < 0)
	{
		ITEMDEF *pstItemDef1 = z_find_itemdef(pstEnv, RONGLU_RESET_USE);
		if (pstItemDef1)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2,ZONEERR_RONGLU202, pstItemDef1->Name);
		}
		goto end;
	}

	if (package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstItemDef->ItemID,1, 
							LIST_TYPE_SUNDRIES,&iPos, 0) < 0)
	{
		goto end;
	}

	if (pstRongluInfo->RongluID > 0)
	{
		iItemID  = pstRongluInfo->RongluID;
	}
	else
	{
		iItemID  = pstRongluDef->ItemID;
	}
	pstRongluInfo->RongluID = iItemID;
	pstPlayer->stOnline.bRongluReset = 1;
	
end:
	ronglu_start_res(pstEnv, pstPlayer, iItemID);
	return 0;
}


int ronglu_reset_end(ZONESVRENV* pstEnv, Player *pstPlayer,  RESETENDCLT *pstEnd)
{
	int iPos = -1;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	ROLEITEM *pstItem;
	ITEMDEF *pstItemDef;
	RONGLUINFO *pstRongluInfo = &pstPlayer->stRoleData.MiscInfo.RongluInfo;
	TASKRUNONE *pstTaskOne;
	int iTaskID ;
	ROLEGRID stRoleGrid;
	int i;

	pstPlayer->stOnline.bRongluReset = 0;
	if (ronglu_mon_is_exist(pstEnv,pstPlayer, pstRongluInfo->ColletID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_RONGLU203);
		return -1;
	}

	if (ronglu_mon_is_exist(pstEnv,pstPlayer, pstRongluInfo->TutemID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_RONGLU204);
		return -1;
	}
	
	if (pstEnd->RongluID != pstRongluInfo->RongluID)
	{
		return -1;
	}

	for (i=0; i<pstPak->SundriesPackage.Num; i++)
	{
		if (pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem.DefID == pstRongluInfo->ItemID)
		{
			iPos	 = i;
			break;
		}
	}

	if (iPos < 0)
		return -1;

	pstItem = &pstPak->SundriesPackage.RoleGrids[iPos].GridData.RoleItem;

	pstItemDef = z_find_itemdef(pstEnv, pstItem->DefID);
	if (!pstItemDef || pstItemDef->Result[0].ResultID != RESULT_RONGLU)
	{
		return -1;
	}

	iTaskID = pstRongluInfo->TaskID;

	pstTaskOne = task_run_find(pstPlayer, iTaskID);
	if (!pstTaskOne)
	{
		return -1;
	}

	if (pstTaskOne->TaskStat == TASK_STAT_FINI)
	{	
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_RONGLU201);
		return -1;
	}

	if (package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer,RONGLU_RESET_USE,1, 
							LIST_TYPE_SUNDRIES,NULL, 0) < 0)
	{
		ITEMDEF *pstItemDef1 = z_find_itemdef(pstEnv, RONGLU_RESET_USE);
		if (pstItemDef1)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2,ZONEERR_RONGLU202, pstItemDef1->Name);
		}
		return -1;
	}

	if (package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstItemDef->ItemID,1, 
							LIST_TYPE_SUNDRIES,&iPos, 0) < 0)
	{
		return -1;
	}

	if (0 != item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstRongluInfo->RongluID) )
	{
		return -1;
	}
	else
	{
		stRoleGrid.GridData.RoleItem.Num = 1;
	}

	if (package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstItemDef->ItemID,1,LIST_TYPE_SUNDRIES,&iPos, 0, 0) < 0)
	{
		return -1;
	}

	if (package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,RONGLU_RESET_USE,1,LIST_TYPE_SUNDRIES,NULL, 0, 0) < 0)
	{
		return -1;
	}

	package_add(pstEnv->pstAppCtx , pstEnv , pstPlayer, &stRoleGrid, 1,0, 0);

	// 通知客户端熔炉变了
	pstRongluInfo->ItemID = pstRongluInfo->RongluID; // 记一下熔炉道具ID
	ronglu_one_chg(pstEnv, pstPlayer, pstRongluInfo->RongluID, iTaskID);
	pstRongluInfo->RongluID = 0;	// 重铸时随机的熔炉ID清零
	
	return 0;

	
}


int ronglu_item_chg_gold(ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID)
{
	RONGLUINFO *pstRongluInfo = &pstPlayer->stRoleData.MiscInfo.RongluInfo;
	RONGLUDEF *pstDef;

	if (pstPlayer->stOnline.bRongluReset)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE, ZONEERR_RONGLU205);
		return -1;
	}
	
	pstDef = ronglu_def_find(pstEnv, iItemID);
	if (!pstDef)
	{
		return -1;
	}

	if (pstDef->Pinzhi != 4)
	{
		return -1;
	}

	if (pstRongluInfo->ItemID == iItemID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE, ZONEERR_RONGLU206);
		return -1;
	}

	if (package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstRongluInfo->ItemID,1, LIST_TYPE_SUNDRIES,NULL, 0) < 0)
	{
		return -1;
	}
	package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstRongluInfo->ItemID,1,LIST_TYPE_SUNDRIES,NULL, 0, 0);
	package_add_by_id(pstEnv, pstPlayer, iItemID, 1, 1);

	pstRongluInfo->ItemID = iItemID;
	ronglu_one_chg(pstEnv, pstPlayer, iItemID, ronglu_get_task_id(pstPlayer));
	return 0;
}

int ronglu_reset_break(ZONESVRENV* pstEnv, Player *pstPlayer,  RESETBREAKCLT *pstBreak)
{
	pstPlayer->stOnline.bRongluReset = 0;
	return 0;
}

int ronglu_clt_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	RONGLUCLT*pstClt = &pstCsPkg->Body.RongluClt;

	if (pstClt->Cmd == RONGLU_CLT_OP_RESET_START)
	{
		ronglu_reset_start(pstEnv, pstPlayer, &pstClt->Data.Start);
	}
	else if (pstClt->Cmd == RONGLU_CLT_OP_RESET_END)
	{
		ronglu_reset_end(pstEnv, pstPlayer, &pstClt->Data.End);
	}
	else if (pstClt->Cmd == RONGLU_CLT_OP_RESET_BREAK)
	{
		ronglu_reset_break(pstEnv, pstPlayer,  &pstClt->Data.Break);	
	}
	return 0;
}


int ronglu_player_login_check(ZONESVRENV *pstEnv, Player *pstPlayer)
{	
	Monster *pstMonster; 
	int iID = pstPlayer->stRoleData.MiscInfo.RongluInfo.TutemID;

	if (iID <= 0) return 0;
	
	pstMonster = z_id_monster(pstEnv->pstAppCtx,pstEnv, iID);
	if (	!pstMonster || 
		pstMonster->iCommType != MON_UNION_TYPE_RONGLU || 
		strcmp(pstMonster->szOwner, pstPlayer->stRoleData.RoleName))
	{
		pstPlayer->stRoleData.MiscInfo.RongluInfo.TutemID = 0;
		return 0;
	}
	
	mon_set_pkinfo(pstMonster, MON_PK_TYPE_PLAYER, pstPlayer->iMemID);
	return 0;
}





