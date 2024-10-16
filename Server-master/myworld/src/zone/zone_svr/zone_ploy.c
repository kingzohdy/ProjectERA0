
#include "zone_ploy.h"
#include "zone_clt.h"
#include "zone_map.h"
#include "zone_err.h"
#include "zone_machine.h"
#include "zone_npc.h"
#include "zone_pworld.h"
#include "zone_attr.h"
#include "zone_package.h"
#include "zone_oplog.h"
#include "zone_misc.h"
#include "zone_team.h"
#include "zone_status.h"


static short gszMztPworldID[]={321,322,323,324,325,326,327,328,329};


int ploy_gc_join_condtion(ZONESVRENV* pstEnv,Player *pstPlayer, void* p)
{
	UNUSED(pstEnv);
	UNUSED(p);
	
	if (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
	{
		return 0;
	}

	if (pstPlayer->stRoleData.Level < GC_PLOY_LVL)
	{
		return 0;
	}
	return 1;
}

// 勇闯古城活动
int ploy_gc_ui_info(ZONESVRENV* pstEnv, Player *pstPlayer, int iType, int iStartTime)
{
	CSPKG stPkg;
	CSACTIVEGAMESVR *pstActiveGameSvr = &stPkg.Body.ActiveGameSvr;
	CSACTIVEGAMEUIINFO *pstActiveUIInfo = &pstActiveGameSvr->Data.UIInfo;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	//memset(pstActiveGameSvr,0,sizeof(*pstActiveGameSvr));
	
	pstActiveGameSvr->Type = iType;
	pstActiveUIInfo->UIType = ACTIVE_UI_TYPE_GUCHENG;
	pstActiveUIInfo->Data.GuCheng.Invaild = iStartTime+GC_PLOY_TIME;
	Z_CSHEAD_INIT(&stPkg.Head, ACTIVE_GAME_SVR);

	if (pstPlayer)
	{
		if (pstPlayer->stRoleData.Level < GC_PLOY_LVL)
		{
			return 0;
		}
			
		if (!IsSameDay(iCurr, pstPlayer->stRoleData.MiscInfo.GCPloyInfo.Time))
		{
			pstPlayer->stRoleData.MiscInfo.GCPloyInfo.GetFlag = 0;
			pstPlayer->stRoleData.MiscInfo.GCPloyInfo.Time = iCurr;
		}
		pstActiveUIInfo->Data.GuCheng.Award = pstPlayer->stRoleData.MiscInfo.GCPloyInfo.GetFlag;
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);	
	}
	else
	{
		z_cltmsg_broadcast_condition(pstEnv, &stPkg, ploy_gc_join_condtion);	
	}
	return 0;
}

int ploy_gc_player_login(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	GameEventInst stEventInst, *pstEvent;
	
	stEventInst.iEventID = GC_EVENT_ID;
	pstEvent = sht_find(pstEnv->pstShtEvent, &stEventInst, EventHashCmp, EventHashCode);
	if (!pstEvent)
	{
		return 0;
	}
	ploy_gc_ui_info( pstEnv, pstPlayer, ACTIVE_GAME_SVR_TYPE_UI_INFO,pstEvent->tEventStart);
	return 0;
}

// 活动结束清场
int ploy_gc_end_player_clear(ZONESVRENV* pstEnv)
{
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst = NULL;
	MapIndex* pstMapIdx = NULL;
	int i,j;
	AREADYN *pstArea;
	Player *pstPlayer;
	int iIndex;
//	RESPOS stPos;


	stIdx.iID	=	LOST_MAP_ID_LAYER7;
	stIdx.iPos	=	-1;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst)
		return -1;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (	NULL == pstMapIdx || 
		pstMapIdx->stMapDef.RevivePs[0].X <= 0 || 
		pstMapIdx->stMapDef.RevivePs[0].Y <= 0 ||
		pstMapIdx->stMapDef.ReviveMap <= 0)
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
		
		for (j = pstArea->iAnimate-1; j >=0; j--) 
		{
			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstArea->aiAnimate[j]);
			if(NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
			{
				continue;
			}

			z_player_change_map(pstEnv->pstAppCtx,  pstEnv, pstPlayer, pstMapIdx->stMapDef.ReviveMap, 
									&pstMapIdx->stMapDef.RevivePs[0], RAND1(180), 0);
		}
	}
	return 0;
}


// 设置某层领取状态
// 返回值-1表示参数错误  
int ploy_gc_set_award(ZONESVRENV* pstEnv, Player *pstPlayer, int iLayer)
{
	GCPLOYINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.GCPloyInfo;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;

	if (iLayer < 1 || iLayer > 7)
	{
		return -1;
	}
	
	if (!IsSameDay(iCurr, pstInfo->Time))
	{
		pstInfo->Time = iCurr;
		pstInfo->GetFlag = 0;
	}
	
	pstInfo->GetFlag |=(0x1<<(iLayer-1));	

	// 这里只是借用此函数通知客户端领取状态
	ploy_gc_player_login(pstEnv, pstPlayer);
	return 0;
}


// 脚本获取某层领取状态
int ploy_gc_get_award(ZONESVRENV* pstEnv, Player *pstPlayer, int iLayer)
{
	GCPLOYINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.GCPloyInfo;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;

	if (iLayer < 1 || iLayer > 7)
	{
		return -1;
	}

	if (IsSameDay(iCurr, pstInfo->Time))
	{
		if ((pstInfo->GetFlag>>(iLayer-1)) &0x1)
		{
			return -2;
		}
	}
	
	return 0;
}


//-----------------------------坦克大战----------------------------


int ploy_tank_role_info(CSTANKROLEINFO *pstInfo, Player *pstPlayer, tdr_longlong llExp)
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

static int ploy_tank_info_qsort_cmp(const void *p1, const void *p2)
{
	CSTANKROLEINFO *pInfo1 = (CSTANKROLEINFO*)p1;
	CSTANKROLEINFO *pInfo2 = (CSTANKROLEINFO*)p2;

	return pInfo2->KillMonster - pInfo1->KillMonster;
}

int ploy_tank_pworld_info(ZONESVRENV * pstEnv, ZONEPWORLD *pstPworld,
	ZONEPWORLDINST *pstPworldInst, Player *pstPlayer, int fEnd)
{
	int i, iPlayer = 0;
	Player *pstPworldPlayer;
	Player *apstPlayer[MAX_PWORLD_MEMBER];
	tdr_longlong llExp = 0, llMoney;
	LEVELUP *pstLevelUP;
	CSPKG stPkg;
	CSPOLYPWORLDSVR *pstCSPolyPworldSvr = &stPkg.Body.PolyPworldSvr;
	CSTANKINFO *pstCSTankInfo;

	Z_CSHEAD_INIT(&stPkg.Head, POLY_PWORLD_SVR);
	pstCSPolyPworldSvr->SvrType = POLY_SVR_TYPE_INFO;

	pstCSPolyPworldSvr->SvrData.PolyPworldInfo.OpType = POLY_OP_TANK;
	pstCSPolyPworldSvr->SvrData.PolyPworldInfo.End = fEnd;

	pstCSTankInfo = &pstCSPolyPworldSvr->SvrData.PolyPworldInfo.Data.TankInfo;
	memset(pstCSTankInfo, 0, sizeof(*pstCSTankInfo));

	//第一方阵容标示为1 ， 第二方阵容标示为2
	pstCSTankInfo->StartTime = pstPworldInst->tStart + pstPworld->stPworldDef.ReadyTime;
	pstCSTankInfo->KillCamp1 = pstPworldInst->stPworldCamp.stCampInfo[0].iKillNum; 	
	pstCSTankInfo->KillCamp2 = pstPworldInst->stPworldCamp.stCampInfo[1].iKillNum;	

	if (fEnd)
	{
		if (pstCSTankInfo->KillCamp1 >pstCSTankInfo->KillCamp2)
		{
			pstCSTankInfo->VCamp = 1;
		}
		else if (pstCSTankInfo->KillCamp1 < pstCSTankInfo->KillCamp2)
		{
			pstCSTankInfo->VCamp = 2;	
		}	
	}

	if(pstPlayer)
	{
		ploy_tank_role_info(&pstCSTankInfo->RoleList[0],pstPlayer,  llExp);
		pstCSTankInfo->RoleNum++;
		
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

			ploy_tank_role_info(&pstCSTankInfo->RoleList[pstCSTankInfo->RoleNum],pstPworldPlayer,  llExp);
			pstCSTankInfo->RoleNum++;
		}

		if(fEnd)
		{
			qsort(&pstCSTankInfo->RoleList[0], pstCSTankInfo->RoleNum,sizeof(pstCSTankInfo->RoleList[0]), ploy_tank_info_qsort_cmp);

			for(i=0; i<pstCSTankInfo->RoleNum; i++)
			{
				pstPworldPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstCSTankInfo->RoleList[i].MemID);
				if(!pstPworldPlayer)
					continue;

				pstLevelUP = z_get_levelup(pstEnv, pstPworldPlayer->stRoleData.Level);
				if(pstLevelUP)
				{
					if(pstCSTankInfo->VCamp == pstPworldPlayer->stOnline.stWarOL.cCampIdx)
					{
						llExp = (tdr_ulonglong)100*pstLevelUP->ContributeExp/10000.0;
					}
					else
					{
						llExp = (tdr_ulonglong)80*pstLevelUP->ContributeExp/10000.0;
					}
					
					player_world_level_exp(pstEnv, pstPworldPlayer, &llExp);
					pstCSTankInfo->RoleList[i].Exp = llExp;
					
					player_add_exp(pstEnv, pstPworldPlayer, llExp, 0);
					package_add_money( pstEnv,  pstPworldPlayer, ATTR_ID_ZHENGYI, 100);
					

					if(is_valid_money_award_player(i, pstCSTankInfo->RoleNum))
					{
						llMoney = (tdr_ulonglong)100*pstLevelUP->ContributeMoney/10000.0;
						package_add_money(pstEnv, pstPworldPlayer, ATTR_ID_MONEY, llMoney);
						pstCSTankInfo->RoleList[i].Money= llMoney;

						z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_TANK, 0, 0, llMoney, "坦克大战银币奖励");
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

int ploy_tank_pworld_tick(ZONESVRENV * pstEnv, ZONEPWORLD *pstPworld, ZONEPWORLDINST *pstZonePworldInst, int iSec)
{
	//提前结束
	if(	pstZonePworldInst->stPworldCamp.stCampInfo[0].iKillNum >= TANK_WIN_NUM|| 
		pstZonePworldInst->stPworldCamp.stCampInfo[1].iKillNum >= TANK_WIN_NUM)
	{
		ploy_tank_pworld_info(pstEnv, pstPworld,pstZonePworldInst, NULL, 1);
		//停止副本刷怪等。
		z_pworld_over(pstEnv, pstZonePworldInst);
		pstZonePworldInst->tEnd = pstEnv->pstAppCtx->stCurr.tv_sec+10;
	}
	else
	{
		if(iSec % 5 == 0)
		{
			ploy_tank_pworld_info(pstEnv, pstPworld,pstZonePworldInst, NULL, 0);
		}
	}
	
	return 0;
}

int poly_tank_pworld_mon_die(ZONESVRENV * pstEnv,  ZONEPWORLDINST *pstZonePworldInst,  Player *pstKiller)
{
	int iCamp = pstKiller->stOnline.stWarOL.cCampIdx;

	if (iCamp <=0 || iCamp > MAX_CAMP_NUM)
		return 0;
	
	pstKiller->stOnline.stWarOL.iKillNum ++;
	pstKiller->stOnline.stWarOL.iContinueKill ++;
	
	pstZonePworldInst->stPworldCamp.stCampInfo[iCamp-1].iKillNum++;

	
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	
	pstMiscSvr->PlayerMiscType = CONTINUE_KILL_TANK;
	pstMiscSvr->PlayerMiscSvrData.ContinueKill.Continue = pstKiller->stOnline.stWarOL.iContinueKill;
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstKiller, &stPkg);
	return 0;
}

int poly_tank_pworld_player_die(ZONESVRENV * pstEnv, Player *pstPlayer, ZoneAni *pstAtker)
{
	Player* pstPlayerKiller = NULL;

	pstPlayer->stOnline.stWarOL.iContinueKill = 0;
	if(pstAtker->iType == OBJ_MONSTER && is_mon_machine(pstAtker->stObj.pstMon))
	{
		pstPlayerKiller = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAtker->stObj.pstMon->iSkillAtkerID);
	}
	else
	{
		pstPlayerKiller = get_atker_player(pstEnv,  pstAtker);
	}

	if (pstPlayerKiller)
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_TANK1,pstPlayerKiller->stRoleData.RoleName);
	return 0;
}

int player_is_in_tank_pworld(ZONESVRENV * pstEnv, Player *pstPlayer)
{
	ZONEPWORLD *pstPworld = NULL;

	pstPworld = z_find_pworld2(pstEnv,pstPlayer->stRoleData.Map);
	if (NULL == pstPworld || pstPworld->stPworldDef.PolyType != PWORLD_POLY_TANK)
	{
		return 0;
	}
	return 1;	
}

int poly_tank_pworld_tip1(ZONESVRENV * pstEnv, Player *pstPlayer, int iDie)
{
	ZONEMAPINST *pstMapInst;
	ZONEIDX stIdx;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	if (iDie >= 15)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMapInst,SYS_MIDDLE,ZONEERR_TANK5);	
	}
	else if (iDie >= 8)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMapInst,SYS_MIDDLE,ZONEERR_TANK4,pstPlayer->stRoleData.RoleName, iDie);	
	}
	else if (iDie >= 4)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMapInst,SYS_MIDDLE,ZONEERR_TANK3,pstPlayer->stRoleData.RoleName, iDie);	
	}
	else if (iDie >= 2)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMapInst,SYS_MIDDLE,ZONEERR_TANK2,pstPlayer->stRoleData.RoleName, iDie);	
	}
	
	return 0;
}



//--------------------------------mzt------------------------------------------
int ploy_mzt_get_pworldid(ZONESVRENV * pstEnv, ZoneTeam *pstTeam)
{
	int iMax = 0;
	int i;
	Player *pstTeamPlayer;

	for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		pstTeamPlayer = z_name_player(pstEnv->pstAppCtx,  pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstTeamPlayer && pstTeamPlayer->stRoleData.MiscInfo.MztInfo.Layer > iMax)
		{
			iMax = pstTeamPlayer->stRoleData.MiscInfo.MztInfo.Layer;
		}
	}

	if (iMax >= MZT_MAX_LAYER)
	{
		return 0;
	}
	
	return gszMztPworldID[iMax];
}


int poly_mzt_team_layer_notify(ZONESVRENV * pstEnv,  ZoneTeam *pstTeam)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	MZTLAYERINFO *pstInfo = &pstMiscSvr->PlayerMiscSvrData.MztLayerInfo;
	int i;
	Player *pstTeamPlayer;
		
	pstMiscSvr->PlayerMiscType = PLAYER_TEAM_MZT_LAYER_NOTIFY;
	pstInfo->Num = 0;

	for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		pstTeamPlayer = z_name_player(pstEnv->pstAppCtx,  pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstTeamPlayer)
		{
			pstInfo->Ones[pstInfo->Num].Layer =  pstTeamPlayer->stRoleData.MiscInfo.MztInfo.Layer;
			strncpy(pstInfo->Ones[pstInfo->Num].Name, pstTeamPlayer->stRoleData.RoleName,
					sizeof(pstInfo->Ones[pstInfo->Num].Name));
			pstInfo->Num++;
		}
	}

	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	if (pstInfo->Num > 0)
	{
		player_team_broadcast(pstEnv, pstTeam, &stPkg);
	}
	return 0;
}


int poly_is_mzt_pworld(int iPworldID)
{
	int i;
	int iSize = (int)(sizeof(gszMztPworldID)/sizeof(gszMztPworldID[0]));

	for (i=0; i<iSize; i++)
	{
		if (gszMztPworldID[i] == iPworldID)
			return 1;
	}
	return 0;
}

int poly_mzt_get_layer(int iPworldID)
{
	int i;
	int iSize = (int)(sizeof(gszMztPworldID)/sizeof(gszMztPworldID[0]));

	for (i=0; i<iSize; i++)
	{
		if (gszMztPworldID[i] == iPworldID)
			return i+1;
	}

	return 0;
}

int poly_mzt_del_status(ZONESVRENV* pstEnv, Player *pstPlayer,int iOldMap,int iStatusID)
{
	//迷之塔 去除特定坏buf
	if (poly_is_mzt_pworld(GET_PWORLDID(iOldMap)))
	{
		ZoneStatus *pstStutas = NULL;
		pstStutas = z_player_find_status(pstPlayer, iStatusID);
		if (pstStutas)
		{
			z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, iStatusID, 1, 0);
		}
	}
	
	return 0;
}

int poly_mzt_get_team_layer(ZONESVRENV* pstEnv,ZoneTeam *pstTeam)
{
//	int iLayer = 0;
	ZONEPWORLDINST *pstPworldInst;
	
	pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstTeam->iAutoPworldInstID);
	if(NULL == pstPworldInst)
		return 0;
	return  poly_mzt_get_layer(pstPworldInst->stPworldIdx.iID);
}

