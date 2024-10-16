

#include "zone_unreal.h"
#include "zone_team.h"
#include "zone_map.h"
#include "zone_clt.h"
#include "zone_act_status.h"
#include "zone_move.h"
#include "zone_script.h"
#include "zone_package.h"
#include "zone_npc.h"
#include "zone_player.h"
#include "zone_machine.h"
#include "zone_err.h"
#include "zone_zh_name.h"
#include "zone_syncrelation.h"

extern int g_dartcar_allow;

#define UNREAL_BASE_EXP_LAYER 50

UNREALMAPDEF* z_find_unrealmapdef(ZONESVRENV* pstEnv, int iMapID)
{
	int iIndex;
	int iEqu;
	UNREALMAPDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	stDef.MapID = iMapID;
	iIndex = bsearch_int(&stDef, pstObj->sUnrealMapDef, pstObj->iUnrealMapDef, UNREAL_MAP_DEF_SIZE, &iEqu);
	if (iEqu)
	{
		return (UNREALMAPDEF*)pstObj->sUnrealMapDef[iIndex];
		
	}
	return NULL;	
}

UNREALDEF* z_find_unrealdef(ZONESVRENV* pstEnv, int iLayer)
{
	int iIndex;
	int iEqu;
	UNREALDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	stDef.Layer= iLayer;
	iIndex = bsearch_int(&stDef, pstObj->sUnrealDef, pstObj->iUnrealDef, UNREAL_DEF_SIZE, &iEqu);
	if (iEqu)
	{
		return (UNREALDEF*)pstObj->sUnrealDef[iIndex];
		
	}
	return NULL;	
}

int unreal_is_unvaild_layer(int iLayer)
{
	if (iLayer < 1 || iLayer >UNREAL_MAX_LAYER)
	{
		return -1;
	}
	return 0;
}

int unreal_get_rand_map(UNREALDEF *pstDef)
{
//	int iMapID = 0;
	int i;
	int iRand = 0;
	
	for (i=0; i<UNREAL_MAP_MAX; i++)
	{
		if (pstDef->RandMaps[i] <= 0)
		{
			break;
		}	

		iRand ++;
	}

	if (iRand <= 0) 
		return 0;
	return pstDef->RandMaps[RAND1(iRand)];
}

int unreal_new_map_inst(ZONESVRENV* pstEnv, PWORLDSELECT *pstSelect, int iPworldInstID)
{
	ZONEIDX stIdx;
	UNREALDEF *pstUnrealDef = z_find_unrealdef(pstEnv, pstSelect->Layer);
		
	if (!pstUnrealDef)
	{
		return -1;
	}

	int iRndMap = unreal_get_rand_map(pstUnrealDef);
	if (iRndMap <= 0)
		return -1;
	
	stIdx.iID = iRndMap;
	if (z_in_new_map_inst(pstEnv->pstAppCtx, pstEnv, iPworldInstID,pstSelect, &stIdx)<0)
	{
		return -1;
	}
	return iRndMap;
}


static int unreal_get_rand_pos(int iRandNum,const RESPOS *pastPosIn, int MaxIn, RESPOS *pastPosOut)
{
	int i;
	int iVaildPos=0;
	int iRndIdx;

	for (i=0; i<MaxIn; i++)
	{
		if (pastPosIn[i].X <= 0 || pastPosIn[i].Y <= 0)
		{
			break;
		}

		iVaildPos ++;
	}

	if (iVaildPos < iRandNum ||iRandNum <= 0 )
	{
		return -1;
	}

	iRndIdx = RAND1(iVaildPos);

	for (i=0; i<iRandNum; i++)
	{
		pastPosOut[i] = pastPosIn[iRndIdx++];
		iRndIdx %= iVaildPos;
	}
	return 0;
}

static int unreal_fill_info(ZONEPWORLDINST *pstZInst, int iMonID)
{	
	int i;
	
	pstZInst->stUnreal.iMonCount ++;  // 刷怪数量++
	for (i=0; i<UNREAL_MON_TYPE_COUNT; i++)
	{
		if (pstZInst->stUnreal.ID[i] == iMonID)
		{
			break;
		}
	}

	// 刷怪种类
	if (i>=UNREAL_MON_TYPE_COUNT && pstZInst->stUnreal.iMon < UNREAL_MON_TYPE_COUNT)
	{
		pstZInst->stUnreal.ID[pstZInst->stUnreal.iMon ] = iMonID;
		pstZInst->stUnreal.iMon ++;
	}
	return 0;
}



int unreal_refresh_map_inst(ZONESVRENV* pstEnv,ZONEPWORLDINST *pstZInst,ZONEMAPINST* pstMInst)
{
	UNREALDEF *pstUnrealDef;
	UNREALMAPDEF *pstUnrealMapDef;
	int iLayer;
	int iMapID;
	int i,j;
	MAPNPC stMapNpc;
	MAPMONSTER stMapMon;
	MapIndex* pstMapIndex;
	RESPOS astRanPos[UNREAL_NORMAL_MON_MAX_POS];
	int iPosIdx = 0;
	int iMon = 0;
	int iMonNum = 0;
//	int iMonHeap;
	int iHeap;
	int iRnd;
	int iGetPos = 0; //是否获取了坐标
	
	if (!pstZInst)
		return -1;

	iLayer = pstZInst->stUnreal.iCurrLayer;
	pstUnrealDef = z_find_unrealdef(pstEnv, iLayer);
	if (!pstUnrealDef)
	{
		return -1;
	}

	iMapID = pstZInst->astMapInstIdx[0].iID &0xffff;
	pstUnrealMapDef = z_find_unrealmapdef(pstEnv, iMapID);
	if (!pstUnrealMapDef)
	{
		return -1;
	}

	pstMapIndex=	z_mappos_index(pstEnv->pstMapObj, &pstMInst->iMapPos, pstMInst->iMapInstID);
	if( !pstMapIndex )
		return -1;

	// 刷幻境Npc
	for (i=0; i<UNREAL_NPC_MAX; i++)
	{
		if (pstUnrealDef->Npcs[i].ID <= 0)
		{
			break;
		}

		memset(&stMapNpc, 0, sizeof(stMapNpc));
		stMapNpc.MonsterID = pstUnrealDef->Npcs[i].ID;
		stMapNpc.Pos.X = pstUnrealMapDef->NpcPos[i].X;
		stMapNpc.Pos.Y= pstUnrealMapDef->NpcPos[i].Y;
		strncpy(stMapNpc.Script,pstUnrealDef->Npcs[i].Script,sizeof(stMapNpc.Script));
		
		if (z_in_new_pworld_npc(pstEnv->pstAppCtx, pstEnv, pstMapIndex, pstMInst, &stMapNpc,0) < 0)
		{
			return -1;
		}	
	}

	// 刷幻境普通monster
	if (pstUnrealDef->NormalNum > 0)
	{
		if (unreal_get_rand_pos(pstUnrealDef->NormalNum,pstUnrealMapDef->Npos,
							UNREAL_NORMAL_MON_MAX_POS, astRanPos) < 0)
		{
			return -1;
		}

		iGetPos = pstUnrealDef->NormalNum;

		// 幻境普通怪第一种方式
		/*for (i=0; i<UNREAL_NORMAL_MON_MAX; i++)
		{
			if (pstUnrealDef->NMons[i].ID <=0 || pstUnrealDef->NMons[i].Num <= 0)
			{
				break;
			}

			for (j=0; j<pstUnrealDef->NMons[i].Num;j++)
			{
				memset(&stMapMon, 0, sizeof(stMapMon));
				stMapMon.MonsterID = pstUnrealDef->NMons[i].ID;
				stMapMon.Pos = astRanPos[iPosIdx++];		// 保证位置随机
				iPosIdx %= pstUnrealDef->NormalNum;
				if (z_in_new_pworld_monster(pstEnv->pstAppCtx, pstEnv, pstMapIndex, pstMInst, &stMapMon) == 0)
				{
					unreal_fill_info(pstZInst, stMapMon.MonsterID);
				}	
			}
		}
		// 幻境普通怪第一种方式end
		*/

		// 幻境普通怪第二种方式
		iMon = 0;
		iMonNum = 0;
		for (i=0; i<UNREAL_NORMAL_MON_MAX; i++)
		{
			if (pstUnrealDef->NMons[i].ID <=0 || pstUnrealDef->NMons[i].Num <= 0)
			{
				break;
			}
			iMon ++ ;
			iMonNum += pstUnrealDef->NMons[i].Num;
		}

		// 每堆数量iHeap
		// pstUnrealDef->NormalNum为0的情况在前面排除了..
		iHeap = iMonNum/pstUnrealDef->NormalNum;
		for (i=0; i<pstUnrealDef->NormalNum; i++)
		{
			// 余数补给最后一堆
			if (i == pstUnrealDef->NormalNum -1)
			{
				iHeap += (iMonNum - iHeap*pstUnrealDef->NormalNum);
			}

			// 刷一堆
			for (j=0; j<iHeap; j++)
			{
				memset(&stMapMon, 0, sizeof(stMapMon));	
				stMapMon.MonsterID = pstUnrealDef->NMons[RAND1(iMon)].ID; // id随机
				stMapMon.Pos = astRanPos[i];
				if (z_in_new_pworld_monster(pstEnv->pstAppCtx, pstEnv, pstMapIndex, pstMInst, &stMapMon) == 0)
				{
					unreal_fill_info(pstZInst, stMapMon.MonsterID);
				}	
			}
		}
		// 幻境普通怪第二种方式 end
	}

	// 必定要刷普通怪
	if (!iGetPos)
	{
		return -1;
	}

	// 刷幻境精英怪
	if (pstUnrealDef->JingyingNum > 0)
	{	
		iPosIdx = RAND1(iGetPos);

		/*if (unreal_get_rand_pos(pstUnrealDef->JingyingNum,pstUnrealMapDef->Jpos,
						UNREAL_JINGYING_MON_MAX, astRanPos) < 0)
		{
			return -1;
		}*/
		
		iMon = 0;
		for (i=0; i<UNREAL_JINGYING_MON_MAX; i++)
		{
			if (pstUnrealDef->JMons[i].ID <=0 || pstUnrealDef->JMons[i].Num <= 0)
			{
				break;
			}
			iMon ++;
		}

		iRnd = RAND1(iMon);

		for (j=0; j<pstUnrealDef->JMons[iRnd].Num;j++)
		{
			memset(&stMapMon, 0, sizeof(stMapMon));
			stMapMon.MonsterID = pstUnrealDef->JMons[iRnd].ID;
			stMapMon.Pos = astRanPos[iPosIdx++];		
			iPosIdx %= iGetPos;
			if (z_in_new_pworld_monster(pstEnv->pstAppCtx, pstEnv, pstMapIndex, pstMInst, &stMapMon) == 0)
			{
				unreal_fill_info(pstZInst, stMapMon.MonsterID);
			}	
		}
	}


	if (pstUnrealDef->BossNum > 0)
	{	
		/*iPosIdx = 0;
		if (unreal_get_rand_pos(pstUnrealDef->BossNum,pstUnrealMapDef->Bpos,
							UNREAL_BOSS_MON_MAX, astRanPos) < 0)
		{
			return -1;
		}*/

		iMon = 0;
		for (i=0; i<UNREAL_BOSS_MON_MAX; i++)
		{
			if (pstUnrealDef->BMons[i].ID <=0 || pstUnrealDef->BMons[i].Num <= 0)
			{
				break;
			}
			iMon ++;
		}

		iRnd = RAND1(iMon);
		for (j=0; j<pstUnrealDef->BMons[iRnd].Num;j++)
		{
			memset(&stMapMon, 0, sizeof(stMapMon));
			stMapMon.MonsterID = pstUnrealDef->BMons[iRnd].ID;
			stMapMon.Pos = astRanPos[iPosIdx++];		
			iPosIdx %= iGetPos;
			if (z_in_new_pworld_monster(pstEnv->pstAppCtx, pstEnv, pstMapIndex, pstMInst, &stMapMon) == 0)
			{
				unreal_fill_info(pstZInst, stMapMon.MonsterID);
			}	
		}
	}
	
	return 0;
}

int unreal_info_insert( ZONESVRENV* pstEnv,int iLayer,int iInsertID)
{
	UnrealInfo *pstInfo = &pstEnv->stUnrealInfo;
	ZONEPWORLDINST *pstInst;
	UnrealPworld *pstUnrealPworld;
	
	pstUnrealPworld = &pstInfo->stPworld[iLayer-1];
	if (pstUnrealPworld->iNum > 0 && pstUnrealPworld->iPInstIDHead <=0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "unreal_info_insert:error! 1");
		return -1;
	}

	pstInst = z_find_pworld_inst(pstEnv->pstMapObj, iInsertID);
	if (!pstInst)
	{	
		tlog_error(pstEnv->pstLogCat, 0, 0, "unreal_info_insert:error! 2");
		return -1;
	}

	pstInst->iNextUnrealInstID = pstUnrealPworld->iPInstIDHead;
	pstUnrealPworld->iPInstIDHead = iInsertID;
	pstUnrealPworld->iNum ++;
	return 0;
}

// iFlag---方便log查看错误出在啥地方
int unreal_info_delete( ZONESVRENV* pstEnv,int iLayer,int iDeleteID, int iFlag)
{
	UnrealInfo *pstInfo = &pstEnv->stUnrealInfo;
	ZONEPWORLDINST *pstInst;
	UnrealPworld *pstUnrealPworld;
	int iPworldInstID ;
	ZONEPWORLDINST *pstPreInst = NULL;
	int iFind = 0;

	if (unreal_is_unvaild_layer( iLayer) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "unreal_info_delete:error!5 %d", iLayer);
		return 0;
	}
	
	pstUnrealPworld = &pstInfo->stPworld[iLayer-1];
	if (pstUnrealPworld->iNum <= 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "%d unreal_info_delete:error!1 %d", iFlag, pstUnrealPworld->iNum);
		//return -1;
	}
	//else
	{
		iPworldInstID = pstUnrealPworld->iPInstIDHead;
		while(iPworldInstID)
		{
			pstInst = z_find_pworld_inst(pstEnv->pstMapObj, iPworldInstID);
			if (!pstInst)
			{	
				tlog_error(pstEnv->pstLogCat, 0, 0, "unreal_info_delete:error!2");
				return -1;	
			}

			if (iPworldInstID == iDeleteID)
			{
				iFind  =1;
				if (iPworldInstID == pstUnrealPworld->iPInstIDHead)
				{
					pstUnrealPworld->iPInstIDHead = pstInst->iNextUnrealInstID; 		// 头部
				}
				else
				{
					if (pstPreInst)
					{
						pstPreInst->iNextUnrealInstID = pstInst->iNextUnrealInstID;		
					}
					else
					{
						tlog_error(pstEnv->pstLogCat, 0, 0, "unreal_info_delete:error!3");
					}
				}

				break;
			}
			pstPreInst = pstInst;
			iPworldInstID = pstInst->iNextUnrealInstID;
		}

		if (!iFind)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "%d unreal_info_delete:error!4", iFlag);
			return 0;
		}
		pstUnrealPworld->iNum --;
		if (pstUnrealPworld->iNum < 0)
			pstUnrealPworld->iNum = 0;

		if (pstUnrealPworld->iNum == 0)
		{
			pstUnrealPworld->iPInstIDHead = 0;
		}
	}
	
	return 0;
}

int unreal_info_up(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, int iLayer,int iType)
{
	if (unreal_is_unvaild_layer(iLayer) < 0)
	{
		return -1;
	}

	// 直接插入
	if (	iType == UNREAL_ENTER_NPC || 
		iType == UNREAL_ENTER_BOSS)
	{
		unreal_info_insert( pstEnv,iLayer,pstPworldInst->iPworldInstID);
	}
	
	// 删除后在插入
	else if (iType == UNREAL_ENTER_NEXT)
	{
		if (iLayer <= 1)
			return -1;

		if (unreal_info_delete( pstEnv,iLayer-1,pstPworldInst->iPworldInstID, 2) < 0)
		{	
			return -1;
		}

		if (unreal_info_insert( pstEnv,iLayer,pstPworldInst->iPworldInstID) < 0)
		{
			return -1;
		}
	}
	return 0;
}


int team_trans_check_one(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if (pstPlayer->eStatus  != PLAYER_STATUS_ROLE_LOGIN)
	{
		return -1;	
	}
	
	if (pstPlayer->stOnline.State &CS_STAT_DEAD)
	{
		return -2;
	}

	if (pstPlayer->stOnline.State & CS_STAT_STALL)
	{
		return -4;
	}

	if (pstPlayer->stOnline.stExchgOnline.bExchgStat  > 0)
	{
		return -5;
	}

	if (pstPlayer->stOnline.stQiecuoInfo.iMemID > 0)
	{
		return -6;
	}

	if (player_is_zuolao( pstPlayer))
	{
		return -7;
	}

	if (player_is_in_dart_car(pstEnv,pstPlayer) )
	{
		if (!g_dartcar_allow)
			return -8;
	}

	return 0;
}

int team_trans_check_tip(ZONESVRENV* pstEnv, Player *pstPlayer, int iCap, int iRet, Player *pstTeamPlayer)
{
	switch(iRet)
	{
	case -1:
		if (iCap)
		{
			
		}
		else
		{
			if (pstTeamPlayer)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_UNREAL201,
							pstTeamPlayer->stRoleData.RoleName);
			}
		}
		break;
	case -2:
		if (iCap)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_UNREAL202);
		}
		else
		{
			if (pstTeamPlayer)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_UNREAL203,
							pstTeamPlayer->stRoleData.RoleName);
			}
		}
		break;
	case -3:
		if (iCap)
		{
			
		}
		else
		{
			if (pstTeamPlayer)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
							ZONEERR_UNREAL204,
							pstTeamPlayer->stRoleData.RoleName);
			}
		}
		break;
	case -4:
		if (iCap)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_UNREAL205);
		}
		else
		{
			if (pstTeamPlayer)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_UNREAL206,
							pstTeamPlayer->stRoleData.RoleName);
			}
			
		}
		break;
	case -5:
		if (iCap)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_UNREAL207);
		}
		else
		{
			if (pstTeamPlayer)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_UNREAL208,
							pstTeamPlayer->stRoleData.RoleName);
			}
			
		}
		break;
	case -6:
		if (iCap)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_UNREAL209);
		}
		else
		{
			if (pstTeamPlayer)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_UNREAL210,
							pstTeamPlayer->stRoleData.RoleName);
			}
			
		}
		break;
	case -7:
		if (iCap)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_UNREAL211);
		}
		else
		{
			if (pstTeamPlayer)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_UNREAL212,
							pstTeamPlayer->stRoleData.RoleName);
			}
			
		}
		break;
	case -8:
		if (iCap)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_UNREAL213);
		}
		else
		{
			if (pstTeamPlayer)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_UNREAL214,
							pstTeamPlayer->stRoleData.RoleName);
			}
			
		}
		break;
	default:
		break;
	}
	return 0;
}


int unreal_enter_check_team(ZONESVRENV* pstEnv, Player *pstPlayer,int iType, int iTip)
{
	ZoneTeam *pstTeam;
	TEAMMEMBER *pstMem;
	int i;
	int iVaildNum = 1;
	Player *pstTeamPlayer;
	int iRet;

	pstTeam = player_team_get(pstEnv, pstPlayer);
	if (!pstTeam )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_UNREAL215);
		return -1;
	}

	pstMem = player_team_get_captain(pstTeam);
	if (!pstMem)
	{
		return -1;
	}
	
	if (strcmp(pstMem->MemberName, pstPlayer->stRoleData.RoleName))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_UNREAL216);
		return -1;
	}

	if (pstPlayer->stRoleData.Level < 20)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_UNREAL217);
		return -1;	
	}

	if (	pstTeam->stTeamInfo.MemberNum < ENTER_UNREAL_MIN)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_UNREAL218, ENTER_UNREAL_MIN);
		return -1;
	}

	// 检查队长
	iRet = team_trans_check_one(pstEnv, pstPlayer);
	if (iRet < 0)
	{
		if (iTip)
		{
			team_trans_check_tip(pstEnv, pstPlayer,1, iRet, NULL);
		}
		return -1;
	}

	for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		pstTeamPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, 
									pstTeam->stTeamInfo.TeamMember[i].MemberName);

		// 掉线都不让进
		if (	!pstTeamPlayer )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_UNREAL219,
						pstTeam->stTeamInfo.TeamMember[i].MemberName);
			return -1;	
		}
		
		if (pstTeamPlayer == pstPlayer)
		{
			continue;
		}

		if (pstTeamPlayer->stRoleData.Map != pstPlayer->stRoleData.Map)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
							ZONEERR_UNREAL204,
							pstTeamPlayer->stRoleData.RoleName);
			
			if (	iType == UNREAL_ENTER_NPC ||
				iType == UNREAL_ENTER_GM)
			{
				return -1;
			}
			else
			{
				continue;	
			}
		}

		// 在同一地图 有不符合传送的
		iRet = team_trans_check_one(pstEnv, pstTeamPlayer);
		if (iRet < 0)
		{
			if (iTip)
			{
				team_trans_check_tip(pstEnv, pstPlayer,0, iRet, pstTeamPlayer);
			}
			return -1;
		}

		iVaildNum ++;
	}
	
	if (iVaildNum < ENTER_UNREAL_MIN)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_UNREAL220, ENTER_UNREAL_MIN);
		return -1;
	}

	return 0;
}

int unreal_enter_check(ZONESVRENV* pstEnv, Player *pstPlayer,int iLayer,int iType)
{
	UNREALMISC *pstUnreal = &pstPlayer->stRoleData.MiscInfo.Unreal;
	int iNextlayer;
	UNREALDEF *pstUnrealDef;

	if (unreal_enter_check_team(pstEnv, pstPlayer, iType, 1) < 0)
	{
		return -1;
	}

	if (iType == UNREAL_ENTER_NPC)
	{
		if (iLayer %10 != 1)
		{
			return -1;
		}

		// 大于10的肯定是保存过的,判断有没有保存
		if (iLayer > 10)
		{
			int iTmp = iLayer/10;
			iTmp = pow(2.0,iTmp-1);
			if (!(pstUnreal->Save&iTmp))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_UNREAL221,iLayer);
				return -1;
			}
		}

		iNextlayer = iLayer;
	}
	else if (iType == UNREAL_ENTER_NEXT)
	{
		ZONEPWORLDINST *pstZonePworldInst;
		
		pstZonePworldInst = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);
		if (NULL == pstZonePworldInst ||pstZonePworldInst->stPworldIdx.iID != UNREAL_PWORLD_ID)
		{
			return -1;
		}

		if (unreal_enter_next_check_kill(pstZonePworldInst) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_UNREAL222, ENTER_NEXT_KILL );
			return -1;
		}

		iNextlayer = pstZonePworldInst->stUnreal.iCurrLayer +1;
	}
	else if (iType == UNREAL_ENTER_BOSS)
	{
		ZONEMAPINST *pstMapInst;
		ZONEIDX stIdx;
		MapIndex *pstMapIdx;
	
		stIdx.iID = pstPlayer->stRoleData.Map;
		pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
		if (NULL == pstMapInst)
			return -1;

		pstMapIdx	=	z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
		if(!pstMapIdx || pstMapIdx->stMapDef.UnrealLayer <= 0)
		{
			return -1;
		}

		iNextlayer = pstMapIdx->stMapDef.UnrealLayer +1;
	}
	else if (iType == UNREAL_ENTER_GM)
	{
		iNextlayer = iLayer;
	}
	else
	{
		return -1;
	}

	if (iNextlayer < 1 )
	{
		return -1;
	}

	if (iNextlayer > UNREAL_MAX_LAYER)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_UNREAL223);
		return -1;
	}

	pstUnrealDef = z_find_unrealdef( pstEnv, iNextlayer);
	if (!pstUnrealDef)
	{
		return -1;
	}

	return iNextlayer;
}


int unreal_cacl_down_time(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst)
{
	UNREALDEF *pstUnrealDef = z_find_unrealdef(pstEnv, pstPworldInst->stUnreal.iCurrLayer);

	pstPworldInst->iDownTm = 0;
	if (pstUnrealDef && pstUnrealDef->TimeLimit > 0)
	{
		pstPworldInst->iDownTm = pstUnrealDef->TimeLimit *60 + pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	return 0;
}

int unreal_layer_info_notify(ZONESVRENV* pstEnv, Player* pstPlayer, ZONEPWORLDINST *pstPworldInst)
{
	CSPKG stPkg;
	UNREALSVR *pstUnrealSvr = &stPkg.Body.UnrealSvr;

	pstUnrealSvr->Cmd = UNREAL_SVR_OP_MON_INFO;
	pstUnrealSvr->Data.Mon.Layer = pstPworldInst->stUnreal.iCurrLayer;
	pstUnrealSvr->Data.Mon.MonCount = pstPworldInst->stUnreal.iMonCount;
	pstUnrealSvr->Data.Mon.MonKill = pstPworldInst->stUnreal.iKillNum;

	Z_CSHEAD_INIT(&stPkg.Head, UNREAL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int unreal_enter_next_check_kill(ZONEPWORLDINST *pstPworldInst)
{
	UNREAL *pstUnreal = &pstPworldInst->stUnreal;

	float f = 100.0*pstUnreal->iKillNum/pstUnreal->iMonCount;

	if (f < ENTER_NEXT_KILL)
		return -1;
	return 0;
}


int unreal_layer_kill_mon_notify(ZONESVRENV* pstEnv, Player* pstPlayer, ZONEPWORLDINST *pstPworldInst)
{
	ZONEMAPINST *pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPworldInst->astMapInstIdx[0]);
	if (pstMInst)
	{
		if (unreal_enter_next_check_kill(pstPworldInst) == 0)
		{
			z_sys_strf_map_broadcast(pstEnv,pstMInst, SYS_MIDDLE, 
						ZONEERR_BROADMAP324,
						pstPworldInst->stUnreal.iKillNum,ENTER_NEXT_KILL);
			return 0;
		}
		z_sys_strf_map_broadcast(pstEnv,pstMInst, SYS_MIDDLE, ZONEERR_BROADMAP325,
								pstPworldInst->stUnreal.iKillNum,pstPworldInst->stUnreal.iMonCount);
	}
	
	return 0;
}


int unreal_enter_next(ZONESVRENV* pstEnv, ZONEPWORLD *pstPworld,ZONEPWORLDINST *pstPworldInst,PWORLDSELECT *pstSelect)
{
	int i;
	MapIndex* pstIdx;
	Player *pstOtherPlayer;
	int iMapIndex;
	RESPOS stPos;
	int iPlayer = 0;
	int aiPlayerID[MAX_PWORLD_MEMBER];
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSENTERMAP *pstEnterMap = &pstPkg->Body.EnterMap;
	int iRandMap;
	UNREALDEF *pstUnrealDef;
	UNREALMAPDEF *pstUnrealMapDef;
	
	pstUnrealDef = z_find_unrealdef(pstEnv, pstPworldInst->stUnreal.iCurrLayer +1);
	if (!pstUnrealDef)
	{
		return -1;
	}

	// 离开地图
	for (i=pstPworldInst->iPlayer-1; i>=0; i--)
       {
	   	pstOtherPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i]);
		if (pstOtherPlayer)
		{	
			aiPlayerID[iPlayer++] = pstOtherPlayer->iMemID;

			SET_PLAYER_NOMOVE(pstOtherPlayer);
			z_player_move_stop(pstEnv->pstAppCtx, pstEnv, pstOtherPlayer);
			z_player_leave_map(pstEnv->pstAppCtx, pstEnv, pstOtherPlayer, 1);
		}
	}

	// 释放地图实例
	while(pstPworldInst->iMapInst>0)
	{
		z_in_destroy_map_inst(pstEnv->pstAppCtx, pstEnv, pstPworld, pstPworldInst, 
								pstPworldInst->astMapInstIdx + pstPworldInst->iMapInst - 1);
		pstPworldInst->iMapInst--;
	}

	if ((pstPworldInst->cPworldScriptFlag & PWORLD_SCRIPT_PROC_START_MAP))
	{
		pstPworldInst->bDelayScript |= PWORLD_SCRIPT_PROC_START_MAP;
	}

	pstPworldInst->iPlayer = 0;
	memset(pstPworldInst->sScriptVar, 0, sizeof(pstPworldInst->sScriptVar));
	pstPworldInst->iMonHpTotal = 0;
	pstPworldInst->iMapCurr = 0;
	pstPworldInst->bOver = 0;
	pstPworldInst->iMapInst = 0;
	memset (&pstPworldInst->stUnreal, 0, sizeof(pstPworldInst->stUnreal));
	pstPworldInst->stUnreal.iCurrLayer = pstUnrealDef->Layer;
	// 重新加载地图
	iRandMap = unreal_new_map_inst( pstEnv, pstSelect, pstPworldInst->iPworldInstID);
	if (iRandMap < 0)
	{
		return -1;
	}

	pstUnrealMapDef = z_find_unrealmapdef(pstEnv, iRandMap);
	if (!pstUnrealMapDef)
	{
		return -1;
	}

	// 副本池索引|MapID
	iMapIndex = (pstPworldInst->iPworldInstID & 0xffff)<<16 | iRandMap;

	// 进入点
	stPos = pstUnrealMapDef->EnterPos;		
	
	pstIdx	= z_mappos_index(pstEnv->pstMapObj, NULL, iMapIndex);
	if( NULL == pstIdx )
	{
		return 0;
	}

	// 进入下一层更新链表
	// 先删除在插入
	unreal_info_up(pstEnv, pstPworldInst, pstUnrealDef->Layer,UNREAL_ENTER_NEXT);
	unreal_cacl_down_time(pstEnv, pstPworldInst);

	for (i=0; i<iPlayer; i++)
	{
		pstOtherPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, aiPlayerID[i]);
		if (pstOtherPlayer)
		{
			pstEnterMap->Map = iRandMap;
			pstEnterMap->RealMap = iRandMap;

			pstEnterMap->x = stPos.X;
			pstEnterMap->y = stPos.Y;

			pstOtherPlayer->stOnline.iPosSeq += 1000;
			pstEnterMap->InitMoveSeq = pstOtherPlayer->stOnline.iPosSeq;
			pstEnterMap->Dir = pstOtherPlayer->stOnline.bDir;
			pstEnterMap->Effect = pstIdx->stMapDef.EffectID;
			pstEnterMap->CameraDir = pstPworld->stPworldDef.InitCameraDir;
			
			pstEnterMap->PworldInfoFlag = 1;
			pstEnterMap->PworldInfo.SelectGrade = pstPworldInst->stPworldselect.PworldGrade;
			
			pstEnterMap->PworldInfo.LeftTime = pstPworldInst->tEnd - pstEnv->pstAppCtx->stCurr.tv_sec;
				
			if (pstPworldInst->iDownTm > 0)
			{
				pstEnterMap->PworldInfo.LeftTime = pstPworldInst->iDownTm - pstEnv->pstAppCtx->stCurr.tv_sec;
				if (0 > pstEnterMap->PworldInfo.LeftTime)
				{
					pstEnterMap->PworldInfo.LeftTime = 0;
				}
			}
			
			pstEnterMap->PworldInfo.PworldInstID = pstPworldInst->iPworldInstID;

			Z_CSHEAD_INIT(&pstPkg->Head, ENTER_MAP);
			z_cltmsg_send(pstEnv, pstOtherPlayer, pstPkg);

			unreal_layer_info_notify(pstEnv, pstOtherPlayer,pstPworldInst);

			z_player_entermap(pstEnv->pstAppCtx, pstEnv, pstOtherPlayer, iMapIndex, &stPos, 1);

			if (pstPworldInst->stUnreal.iCurrLayer %10 ==  1)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOtherPlayer, SYS_MIDDLE, 
							ZONEERR_UNREAL224, 
							pstPworldInst->stUnreal.iCurrLayer, pstPworldInst->stUnreal.iCurrLayer);
			}

			
		}
	}

	for (i=0; i<iPlayer; i++)
	{
		pstOtherPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, aiPlayerID[i]);
		if (pstOtherPlayer)
		{
			//师徒声望奖励
			player_pworld_award_schoolval(pstEnv, pstOtherPlayer);
		}
	}
	return 0;
}


int unreal_team_enter_pworld(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLD *pstPworld, 
									ZONEPWORLDINST *pstPInst,ZoneTeam *pstTeam)
{
	int i;
	Player *pstTeamPlayer;
	int iVaildNum = 1;
	Player *astpPlayer[MAX_TEAM_MEMBER];
	int iEnterType = pstPlayer->stOnline.stUnrealOL.iEnterFlag;
	int iRet;
	
	if (0 > z_check_single_pworld(pstEnv, pstPlayer, &pstPworld->stPworldDef, 0, NULL))
	{
		return -1;
	}
	astpPlayer[0] = pstPlayer;
	
	for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		pstTeamPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, 
							pstTeam->stTeamInfo.TeamMember[i].MemberName);
		if (!pstTeamPlayer )
		{
			return -1;
		}

		if ( pstTeamPlayer == pstPlayer)
		{
			continue;
		}

		if (	pstTeamPlayer->stRoleData.Map != pstPlayer->stRoleData.Map )
		{
			if (	iEnterType == UNREAL_ENTER_NPC ||
				iEnterType == UNREAL_ENTER_GM)
			{
				return -1;	
			}
			else
			{
				continue;
			}
		}

		iRet = team_trans_check_one(pstEnv, pstTeamPlayer);
		if (iRet < 0)
		{
			return -1;
		}

		if (0 > z_check_single_pworld(pstEnv, pstTeamPlayer, &pstPworld->stPworldDef, 0, NULL))
		{
			return -1;
		}
		
		astpPlayer[iVaildNum] = pstTeamPlayer;
		iVaildNum ++;
	}

	if (	iVaildNum < ENTER_UNREAL_MIN )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE, ZONEERR_UNREAL220, ENTER_UNREAL_MIN);
		return -1;
	}

	for (i=0; i<iVaildNum; i++)
	{
		z_player_enter_pworld_map(pstEnv, astpPlayer[i], pstPInst, pstPworld);
	}
	return 0;
}

// iType=0通过npc进入创建副本 ,1-副本内进入下一层2-boss地图进入创建副本
int unreal_enter(ZONESVRENV* pstEnv, Player *pstPlayer,int iLayer,int iType)
{
	int iEnterlayer;
	PWORLDSELECT stSelect;
	ZONEPWORLD *pstPworld;
	ZONEIDX stIdx;
	
	if ((iEnterlayer = unreal_enter_check(pstEnv, pstPlayer,iLayer,iType)) == -1)
	{
		return -1;
	}

	stIdx.iID = UNREAL_PWORLD_ID;
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (!pstPworld)
	{
		return -1;
	}

	memset(&stSelect, 0, sizeof(stSelect));
	stSelect.Layer = iEnterlayer;
	pstPlayer->stOnline.stUnrealOL.iEnterFlag = iType;

	// 创建副本进入
	if (	iType == UNREAL_ENTER_NPC || 
		iType == UNREAL_ENTER_BOSS || 
		iType == UNREAL_ENTER_GM)
	{
		z_player_enter_pworld(pstEnv, pstPlayer, UNREAL_PWORLD_ID, &stSelect, 0);
		unreal_exp_layer_chg(pstEnv,pstPlayer, 0);
	}
	// 副本实例存在
	else
	{	
		ZONEPWORLDINST *pstPInst = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);
		if ( pstPInst )
		{
			unreal_enter_next(pstEnv, pstPworld,pstPInst,&stSelect);
			unreal_exp_layer_chg(pstEnv,pstPlayer, 0);
		}
		return -1;
	}
	return 0;
}

// iEnterMap=1 进入地图  0-离开地图
int  unreal_up_boss_info(ZONESVRENV* pstEnv, Monster *pstMon,int iEnterMap)
{
	MONSTERDEF *pstDef;
	UNREALBOSSINFOS *pstBoss= &pstEnv->stUnrealInfo.stBossInfo;
	int i;
	MapIndex *pstMapIndex;
	int iPos = -1;
	char szName[32];
	
	pstDef = z_get_mon_def( pstEnv,  pstMon);
	if (!pstDef || pstDef->BossType <= 0 )
	{
		return 0;
	}

	pstMapIndex = z_mappos_index(pstEnv->pstMapObj, &iPos, pstMon->stMap.iID);
	if (!pstMapIndex)
	{
		return 0;
	}

	if (pstMon->iDefIdx == 206863)
	{
		strncpy(szName, LJY_UNREAL1, sizeof(szName));
	}
	else if(pstMon->iDefIdx == 206864)
	{
		strncpy(szName, LJY_UNREAL2, sizeof(szName));
	}
	else if (pstMon->iDefIdx == 206865)
	{
		strncpy(szName, LJY_UNREAL3, sizeof(szName));
	}
	else if (pstMon->iDefIdx == 206866)
	{
		strncpy(szName, LJY_UNREAL4, sizeof(szName));
	}
	else if (pstMon->iDefIdx == 206867)
	{
		strncpy(szName, LJY_UNREAL5, sizeof(szName));
	}
	else if (pstMon->iDefIdx == 206868)
	{
		strncpy(szName, LJY_UNREAL6, sizeof(szName));
	}
	else
	{
		return 0;
	}
	
	if (iEnterMap)
	{
		/*z_sys_strf_broadcast(pstEnv, SYS_SVR, 
	"携带大量珍宝的[%s-%s]已经降临在[%s],然后呢,你们都懂的!", 
	szName, pstDef->Name,pstMapIndex->stMapDef.Name);*/
	}
	else
	{
		/*z_sys_strf_broadcast(pstEnv, SYS_SVR, 
	"降临在[%s]的[%s-%s],已经被勇士们击败,笼罩在[%s]的邪恶力量已经消散!", 
			pstMapIndex->stMapDef.Name,szName, pstDef->Name, pstMapIndex->stMapDef.Name);*/
	}

	// 先实现功能,以后在优化
	for ( i=0; i<pstBoss->Num; i++)
	{
		if (pstBoss->BossInfo[i].ID == (int)pstDef->MonsterID)
		{
			break;
		}	
	}
	
	if (iEnterMap)
	{
		if (	i >= pstBoss->Num && 
			pstBoss->Num < UNREAL_MAX_BOSS)
		{
			pstBoss->BossInfo[pstBoss->Num].ID = pstDef->MonsterID;
			pstBoss->BossInfo[pstBoss->Num].MapID = pstMon->stMap.iID;
			pstBoss->BossInfo[pstBoss->Num].Pos = pstMon->stCurrPos;
			pstBoss->Num ++;
		}
	}
	else
	{
		if ( i < pstBoss->Num )
		{
			pstBoss->Num --;
			if ( i != pstBoss->Num )
			{
				pstBoss->BossInfo[i] = pstBoss->BossInfo[pstBoss->Num];
			}
		}
	}
	
	return 0;
}


// 脚本用
int unreal_watch_boss_info(ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID)
{
	CSPKG stPkg;
	UNREALSVR *pstUnrealSvr = &stPkg.Body.UnrealSvr;

	pstUnrealSvr->Cmd = UNREAL_SVR_OP_BOSS_INFO;
	pstUnrealSvr->Data.Boss = pstEnv->stUnrealInfo.stBossInfo;
	pstUnrealSvr->Data.Boss.NpcID = iNpcID;
	
	Z_CSHEAD_INIT(&stPkg.Head, UNREAL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int unreal_save_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	UNREALSVR *pstUnrealSvr = &stPkg.Body.UnrealSvr;

	// 80层的以后不开放
	// 有的玩家已经保存了80存以后的记录
	// 以后开放110层后,下面这句话要删除
	if (UNREAL_MAX_LAYER == 80)
	{
		pstPlayer->stRoleData.MiscInfo.Unreal.Save &= ~(128|256|512);
	}
	
	pstUnrealSvr->Cmd = UNREAL_SVR_OP_SAVE_INFO;
	pstUnrealSvr->Data.Save.Save= pstPlayer->stRoleData.MiscInfo.Unreal.Save;
	
	Z_CSHEAD_INIT(&stPkg.Head, UNREAL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

// 脚本用
int unreal_save(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZONEPWORLDINST *pstPInst;
	int iLayer;
	int iBit;
	UNREALMISC *pstUnreal = &pstPlayer->stRoleData.MiscInfo.Unreal;

	pstPInst = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);
	if (!pstPInst)
	{
		return -1;
	}

	iLayer = pstPInst->stUnreal.iCurrLayer;
	if (pstPInst->stPworldIdx.iID != UNREAL_PWORLD_ID )
	{
		return -1;
	}

	if ((iLayer %10) != 1 || iLayer <=10)
	{
		return -1;
	}

	iBit = iLayer/10;
	iBit = pow(2.0,iBit-1);
	
	if (pstUnreal->Save & iBit)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_UNREAL225);
		return -1;
	}

	pstUnreal->Save |= iBit;
	z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_UNREAL226);
	task_player_trace(pstEnv, pstPlayer, TASK_OBJ_UNREAL_SAVE);

	unreal_save_info(pstEnv, pstPlayer);
	return 0;
	
}


int unreal_team_info_fill(ZONESVRENV* pstEnv,ZONEPWORLDINST *pstPInst,UNREALTEAMONE *pstTeamInfoOne)
{
	int i;
	ZoneTeam *pstTeam;
	Player *pstPlayer;
	TEAMMEMBER *pstMem;

	for (i=0; i<pstPInst->iPlayer; i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx,  pstEnv, pstPInst->aiPlayerID[i]);
		if (!pstPlayer)
		{
			continue;
		}
		
		pstTeam = player_team_get(pstEnv, pstPlayer);	
		if (!pstTeam)
		{
			continue;
		}

		pstMem = player_team_get_captain(pstTeam);
		if (!pstMem)
		{
			continue;;
		}

		pstTeamInfoOne->Layer = pstPInst->stUnreal.iCurrLayer;
		pstTeamInfoOne->Level = pstMem->Level;
		pstTeamInfoOne->Num = pstTeam->stTeamInfo.MemberNum;
		strncpy(pstTeamInfoOne->CaptainName, pstMem->MemberName, 
				sizeof(pstTeamInfoOne->CaptainName));
		break;
		
	}

	// 没有写入过队伍信息
	if ( i >= pstPInst->iPlayer)	
	{
		return -1;
	}
	return 0;
}

int unreal_team_info_make(ZONESVRENV* pstEnv,UnrealPworld *pstUnrealPworld,UNREALSVR *pstUnrealSvr)
{
	int iPInstID;
	ZONEPWORLDINST *pstPInst;
	
	iPInstID = pstUnrealPworld->iPInstIDHead;
	while(iPInstID && pstUnrealSvr->Data.Team.TeamNum < UNREAL_TEAM_INFO_MAX)
	{
		pstPInst = z_find_pworld_inst(pstEnv->pstMapObj, iPInstID);
		if (!pstPInst ||pstPInst->stPworldIdx.iID != UNREAL_PWORLD_ID)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "unreal_team_info_make:error! ");
			return -1;	
		}
		iPInstID = pstPInst->iNextUnrealInstID;
		if(unreal_team_info_fill(pstEnv,pstPInst,
							&pstUnrealSvr->Data.Team.TeamInfos[pstUnrealSvr->Data.Team.TeamNum]) == 0)
		{
			pstUnrealSvr->Data.Team.TeamNum++;
		}
	} 

	return 0;
}

int unreal_team_info(ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID)
{
	CSPKG stPkg;
	UNREALSVR *pstUnrealSvr = &stPkg.Body.UnrealSvr;
	int iCount = 0;
	int i;
	int iLayer;

	for (i=0; i<UNREAL_MAX_LAYER; i++)
	{
		iCount += pstEnv->stUnrealInfo.stPworld[i].iNum;
	}

	if (iCount <= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2,ZONEERR_UNREAL227);
		return -1;
	}

	memset(&pstUnrealSvr->Data.Team, 0, sizeof(pstUnrealSvr->Data.Team));
	pstUnrealSvr->Data.Team.NpcID = iNpcID;
	pstUnrealSvr->Cmd = UNREAL_SVR_OP_TEAM_INFO;
	
	if (iCount <= UNREAL_TEAM_INFO_MAX)
	{
		for (i=0; i<UNREAL_MAX_LAYER; i++)
		{
			unreal_team_info_make(pstEnv, &pstEnv->stUnrealInfo.stPworld[i], pstUnrealSvr);
		}
	}
	else
	{
		iLayer = pstPlayer->stRoleData.Level -20;	 // 优先找自己等级匹配的层
		if(iLayer < 1)
		{
			iLayer = 1;
		}
		else if (iLayer > UNREAL_MAX_LAYER)
		{
			iLayer = UNREAL_MAX_LAYER;
		}

		 // 优先找自己等级以及以上的层
		for (i=iLayer-1; i<UNREAL_MAX_LAYER; i++)
		{
			unreal_team_info_make(pstEnv, &pstEnv->stUnrealInfo.stPworld[i], pstUnrealSvr);
		}

		// 没找到10个继续找自己等级一下的
		if (pstUnrealSvr->Data.Team.TeamNum < UNREAL_TEAM_INFO_MAX)
		{
			for (i=iLayer-2; i>=0; i--)
			{
				unreal_team_info_make(pstEnv, &pstEnv->stUnrealInfo.stPworld[i], pstUnrealSvr);
			}
		}
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, UNREAL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int unreal_get_enter_pos(ZONESVRENV* pstEnv,ZONEPWORLDINST *pstPInst, int *piMapIdx, RESPOS *pstPos)
{
	UNREALMAPDEF *pstDef;
	
	*piMapIdx = pstPInst->astMapInstIdx[0].iID;

	pstDef = z_find_unrealmapdef(pstEnv, (*piMapIdx) & 0xffff);
	if( !pstDef )
		return -1;

	*pstPos = pstDef->EnterPos;
	return 0;
}

int unreal_up_kill(ZONESVRENV* pstEnv,  Player *pstPlayer,int iKillID)
{
	ZONEPWORLDINST *pstPInst;
	int i;
	
	pstPInst = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);
	if (NULL == pstPInst )
	{
		return 0;
	}

	for (i=0; i<pstPInst->stUnreal.iMon; i++)
	{
		if (pstPInst->stUnreal.ID[i] == iKillID)
		{
			pstPInst->stUnreal.iKillNum ++;
			if (pstPInst->stUnreal.iKillNum > pstPInst->stUnreal.iMonCount)
			{
				pstPInst->stUnreal.iKillNum = pstPInst->stUnreal.iMonCount;
				tlog_error(pstEnv->pstLogCat, 0, 0, "unreal_up_kill:error!");
			}

			// 不发协议了 改成地图广播
			//unreal_layer_info_notify(pstEnv, pstPlayer, pstPInst);

			unreal_layer_kill_mon_notify(pstEnv, pstPlayer, pstPInst);
			return 0;
		}
	}
	return 0;
}

int is_unreal_boss_map(ZONESVRENV* pstEnv, int iMapID)
{
	MapIndex *pstMapIdx; 

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, iMapID);
	if (pstMapIdx && pstMapIdx->stMapDef.UnrealLayer > 0)
	{
		return 1;
	}
	return 0;
}

int unreal_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	UNREALCLT *pstUnrealClt = &pstCsPkg->Body.UnrealClt;
	UNUSED(pstFrameHead);
	UNUSED(pstAppCtx);

	if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode  || 
		 pstPlayer->stOnline.eStatusAssistant&PLAYER_STATUS_BATTLE_LOGIN_READY)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2 ,ZONEERR_BATTLE_SVR_OP);
		return -1;
	}

	if (pstUnrealClt->Cmd == UNREAL_CLT_OP_ENTER )
	{
		if (check_npc_ui(pstEnv, pstPlayer, UI_TYPE_UNREAL) < 0)
		{
			return -1;
		}
		return  unreal_enter(pstEnv, pstPlayer,pstUnrealClt->Data.Enter.Layer,UNREAL_ENTER_NPC);
	}
	return 0;
}

/*	-lxk
	功能:判断角色所在队伍的队长是否在彩虹幻境中,并返回队长角色
*/
Player *is_captain_in_unreal(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZoneTeam *pstTeam;
	TEAMMEMBER *pstCaptain;
	Player *pstCaptainPlayer = NULL;
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(pstTeam == NULL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_UNREAL215);
		return NULL;
	}

	pstCaptain = player_team_get_captain(pstTeam);
	if(pstCaptain == NULL)
	{
		return NULL;
	}
	pstCaptainPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,pstCaptain->MemberName);
	if(pstCaptainPlayer == NULL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_UNREAL228);
		return NULL;
	}
	//判断队长是不是角色本人
	if(strcmp(pstCaptain->MemberName, pstPlayer->stRoleData.RoleName) != 0)
	{
		ZONEPWORLDINST *pstZonePworldInst;
		pstZonePworldInst = z_find_pworldinst2(pstEnv, pstCaptainPlayer->stRoleData.Map);
		if (NULL == pstZonePworldInst )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_UNREAL229);
			return NULL;
		}

		if (pstZonePworldInst->stPworldIdx.iID != UNREAL_PWORLD_ID)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_UNREAL229);
			return NULL;
		}
	}
	else
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_UNREAL230);
		return 0;
	}
	return pstCaptainPlayer;
}

/*	-lxk
	功能:获取角色当前所在层
*/
int get_player_unreal_layer(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	ZONEPWORLDINST *pstZonePworldInst;
	pstZonePworldInst = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);
	if (NULL == pstZonePworldInst )
	{
		return -1;
	}
	//是否在幻境中
	if (pstZonePworldInst->stPworldIdx.iID != UNREAL_PWORLD_ID)
	{
		return -1;
	}
	return pstZonePworldInst->stUnreal.iCurrLayer;
}

int unreal_enter_boss_check_level(ZONESVRENV *pstEnv,Player *pstPlayer, ZONEPWORLDINST *pstPInst)
{
	int iCurr = pstPInst->stUnreal.iCurrLayer;
	int iLevel;

	if (iCurr % 10 != 0)
	{
		return -1;
	}

	if (iCurr < 110)
	{
		iLevel = 	iCurr+10;
	}
	else if (iCurr == 110)
	{
		iLevel = 110;
	}
	else
	{
		return -1;
	}

	if (pstPlayer->stRoleData.Level < iLevel)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer,SYS_MIDDLE2, 
				ZONEERR_UNREAL231,iLevel);
		return -1;
	}
	
	return 0;
}

int unreal_check_exp_layer(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	UNREALMISC *pstMisc = &pstPlayer->stRoleData.MiscInfo.Unreal;

	if (!IsSameWeek(pstMisc->Time, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstMisc->Time =  pstEnv->pstAppCtx->stCurr.tv_sec;
		if (pstMisc->ExpLayer < 0)
		{
			pstMisc->ExpLayer = 0;	
		}
		pstMisc->ExpLayer = pstMisc->ExpLayer/2+UNREAL_BASE_EXP_LAYER;
		if (pstMisc->ExpLayer  > 100)
		{
			pstMisc->ExpLayer  = 100;
		}
	}
	return 0;
}

int unreal_exp_layer_notify(ZONESVRENV *pstEnv,Player *pstPlayer, int iSingle)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	ZoneTeam *pstTeam;
	int i;
	Player *pstTmp;
	UNREALMISC *pstMisc;

	pstMiscSvr->PlayerMiscType = PLAYER_UNREAL_EXP_LAYER_NOTIFY;

	if (iSingle)
	{
		pstMisc = &pstPlayer->stRoleData.MiscInfo.Unreal;
		if (pstMisc->ExpLayer >= 0)
		{
			pstMisc->ExpLayer --;
		}
		
		pstMiscSvr->PlayerMiscSvrData.UnrealExpLayerInfo.Layer = pstMisc->ExpLayer;
		if (pstMiscSvr->PlayerMiscSvrData.UnrealExpLayerInfo.Layer < 0)
			pstMiscSvr->PlayerMiscSvrData.UnrealExpLayerInfo.Layer = 0;	

		Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		return 0;
	}
	
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if (!pstTeam)
	{
		return 0;
	}

	for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		pstTmp = z_name_player(pstEnv->pstAppCtx,  pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstTmp)
		{
			pstMisc = &pstTmp->stRoleData.MiscInfo.Unreal;
			if (pstMisc->ExpLayer >= 0)
			{
				pstMisc->ExpLayer --;
			}
			pstMiscSvr->PlayerMiscSvrData.UnrealExpLayerInfo.Layer =pstMisc->ExpLayer;
			if (pstMiscSvr->PlayerMiscSvrData.UnrealExpLayerInfo.Layer < 0)
				pstMiscSvr->PlayerMiscSvrData.UnrealExpLayerInfo.Layer = 0;	

			Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
			z_cltmsg_send(pstEnv, pstTmp, &stPkg);
		}
	}

	return 0;
}

int unreal_exp_layer_chg(ZONESVRENV *pstEnv,Player *pstPlayer, int iSingle)
{
	
	// 通知客户端
	unreal_exp_layer_notify(pstEnv,pstPlayer,iSingle);
	return 0;
}

int unreal_add_award_exp(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	ZONEPWORLDINST *pstZonePworldInst;
	pstZonePworldInst = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);
	LEVELUP *pstLevelUP;
	
	if (	pstZonePworldInst && 
		pstZonePworldInst->stPworldIdx.iID == UNREAL_PWORLD_ID && 
		pstPlayer->stRoleData.MiscInfo.Unreal.ExpLayer >= 0)
	{
		pstLevelUP = (LEVELUP *)pstEnv->pstZoneObj->sLevelUpDef[pstPlayer->stRoleData.Level-1];
		return  pstLevelUP->ContributeExp/20000;
	}
		
	return 0;
}

