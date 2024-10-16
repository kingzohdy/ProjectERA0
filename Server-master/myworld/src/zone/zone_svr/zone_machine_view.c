/*
**  @file $RCSfile: zone_machine_view.c,v $
**  general description of this module
**  $Id: zone_machine_view.c,v 1.27 2013/05/03 04:09:20 eric Exp $
**  @author $Author: eric $
**  @date $Date: 2013/05/03 04:09:20 $
**  @version $Revision: 1.27 $
*/
#include "zone_move.h"
#include "zone_map.h"
#include "zone_clt.h"
#include "tconnapi.h"
#include "zone_act_status.h"
#include "zone_npc.h"
#include "zone_ai.h"
#include "zone_attr.h"
#include "zone_team.h"
#include "zone_pet.h"
#include "zone_script.h"
#include "zone_clan.h"
#include "zone_status.h"
#include "zone_player.h"
#include "zone_fairy.h"
#include "mapmask.h"
#include "zone_package.h"
#include "zone_tmp.h"
#include "zone_skill.h"
#include "zone_machine.h"
#include "zone_combat.h"
#include "zone_machine.h"
#include "zone_machine_view.h"

int machine_count_view(ZONESVRENV* pstEnv, Monster *pstMachine)
{
	int i;
	int iIndex;
	char cType;
	TMEMBLOCK *pstMemBlock;
	Player *pstVPlayer;
	Monster *pstVMon;
	Npc *pstNpc;
		
	pstMachine->iEmon = pstMachine->iEplayer = 0;
	pstMachine->iFplayer = pstMachine->iImpMon = 0;

	for (i=VISIBLE_ANIMATE_NORM*6/7; i<pstMachine->iVPlayer; i++)
	{
		iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstMachine->aiVPlayer[i], &cType);

		if (iIndex < 0) continue;

		switch(cType)
		{
			case OBJ_PLAYER:
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
				if (!pstMemBlock->fValid)
					continue;

				pstVPlayer = (Player *)pstMemBlock->szData; 
				if (PLAYER_STATUS_ROLE_LOGIN != pstVPlayer->eStatus || pstVPlayer->stRoleData.Map != pstMachine->stMap.iID)
				{
					continue;
				}

				if (is_player_ride_on_machine(pstVPlayer) && pstMachine != machine_get_by_player(pstEnv, pstVPlayer))
				{
					continue;
				}

				if ( 0 == player_view_mon_must(pstEnv, pstVPlayer, pstMachine) || 
					(pstVPlayer->stOnline.State & CS_STAT_STALL))
				{
					continue;
				}

				if (is_mon_enemy_player(pstEnv, pstMachine, pstVPlayer))
				{
					pstMachine->iEplayer++;
				}
				else
				{
					pstMachine->iFplayer++;
				}
				
				break;
			case OBJ_MONSTER:
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
				if (!pstMemBlock->fValid)
					continue;
							
				pstVMon = (Monster *)pstMemBlock->szData;
				if (pstVMon->cMonMiscFlag & MON_MISC_FLAG_IMPVIEW)
				{
					pstMachine->iImpMon++;
				}
				else if (is_mon_enemy_mon(pstEnv, pstMachine, pstVMon))
				{
					pstMachine->iEmon++;
				}
				
				break;
			case OBJ_NPC:
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
				if (!pstMemBlock->fValid)
					continue;
				pstNpc = (Npc *)pstMemBlock->szData;
				pstVMon = &pstNpc->stMon;
				
				if (pstVMon->bMoveType == MOVE_TYPE_NO)
				{
					continue;
				}
				
				pstMachine->iImpMon++;
				break;
			default:
				break;
		}
	}
		
	return 0;
}

int machine_view_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMachine, Player *pstVPlayer)
{
	int i;
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSENTERVIEW *pstEnterView = &pstPkg->Body.EnterView;
	CSMOVEOTHER *pstMoveOther = &pstPkg->Body.MoveOther;
	CSWALK *pstWalk = &pstMoveOther->Data.Walk;
	Player *pstPlayerTmp;
	Player * apstPlayer[MAX_MON_MACHINE_PASSENGER];
	int iCount = 0;
	int iCurrPoint;

	if(pstVPlayer->stOnline.State & CS_STAT_AUTOFLY)
	{
		return 0;
	}

	z_make_player_view(pstAppCtx, pstEnv, pstVPlayer, pstEnterView);
	Z_CSHEAD_INIT(&pstPkg->Head, ENTER_VIEW);

	for (i=0; i<this_machine(pstEnv,pstMachine)->stMachineInfo.MaxPassenger; i++)
	{
		pstPlayerTmp = z_id_player(pstAppCtx, pstEnv, this_machine(pstEnv,pstMachine)->stMachineInfo.PassengerID[i]);
		if (pstPlayerTmp && pstPlayerTmp->eStatus == PLAYER_STATUS_ROLE_LOGIN && pstPlayerTmp != pstVPlayer)
		{
			if ((pstVPlayer->stOnline.State & CS_STAT_QIANXIN) && (0 == player_team_same(pstEnv, pstPlayerTmp, pstVPlayer)))
			{
				continue;
			}
			
			apstPlayer[iCount++] = pstPlayerTmp;
		}
	}

	z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iCount, pstPkg);
	
	if (pstVPlayer->stOnline.cMoving)
	{
		pstMoveOther->ID = pstVPlayer->iMemID;
		pstMoveOther->x = pstVPlayer->stRoleData.Pos.X;
		pstMoveOther->y = pstVPlayer->stRoleData.Pos.Y;
		
		pstMoveOther->MoveType = CS_MOVE_WALK;
		pstWalk->Total = pstVPlayer->stOnline.cFuturePoints;
		if (pstWalk->Total > CS_MAX_MOVEPOINTS)
		{
			pstWalk->Total = CS_MAX_MOVEPOINTS;
		}
		
		for (i = 0; i < pstWalk->Total; i++) {
			iCurrPoint = (pstVPlayer->stOnline.cCurrPoint + i) % RECORD_TURN;
			pstWalk->Points[i].x = pstVPlayer->stOnline.stTurnPoints[iCurrPoint].X;
			pstWalk->Points[i].y = pstVPlayer->stOnline.stTurnPoints[iCurrPoint].Y;
		}

		memcpy(&pstMoveOther->MspdInfo, &pstVPlayer->stOnline.stMspdInfo, sizeof(pstMoveOther->MspdInfo));

		Z_CSHEAD_INIT(&pstPkg->Head, MOVE_OTHER);
		z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iCount, pstPkg);
	}

	return 0;
}


int player_view_machine(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMachine, ZONEMAPINST* pstMapInst)
{
	short *piViewCount = NULL;
	short *piOtherViewCount = NULL;
	int iMaxVisibleAnimate = MAX_VISIBLE_ANIMATE;
	int iVisibleAnimateNorm = VISIBLE_ANIMATE_NORM;
	int iVisibleAnimateMust = VISIBLE_ANIMATE_MUST;
	int iVisibleAnimateEmon = VISIBLE_ANIMATE_EMON;
	int iVisibleAnimateEplayer = VISIBLE_ANIMATE_EPLAYER; 
	int iVisibleAnimateFplayer = VISIBLE_ANIMATE_FPLAYER;
	int iVisibleAnimateImpmon = VISIBLE_ANIMATE_IMPMON; 

	{
		if (z_pos_in_view_half1(&pstPlayer->stRoleData.Pos, &pstMachine->stCurrPos))  //小于等于16米
		{
			;
		}
		else if (z_pos_in_view_half2(&pstPlayer->stRoleData.Pos, &pstMachine->stCurrPos)) // 16米到20米
		{
			iVisibleAnimateNorm = VISIBLE_ANIMATE_NORM *6/7;
			iVisibleAnimateMust = VISIBLE_ANIMATE_MUST/2;
			iVisibleAnimateEmon = VISIBLE_ANIMATE_EMON/2;
			iVisibleAnimateEplayer = VISIBLE_ANIMATE_EPLAYER/2;
			iVisibleAnimateFplayer = VISIBLE_ANIMATE_FPLAYER/2;
			iVisibleAnimateImpmon = VISIBLE_ANIMATE_IMPMON/2;
			iMaxVisibleAnimate = (iVisibleAnimateNorm+ iVisibleAnimateMust+ iVisibleAnimateEmon+ iVisibleAnimateEplayer+ iVisibleAnimateFplayer+ iVisibleAnimateImpmon);
		}
		else //20米到24米
		{
			iVisibleAnimateNorm = VISIBLE_ANIMATE_NORM *3/7;
			iVisibleAnimateMust = VISIBLE_ANIMATE_MUST/4;
			iVisibleAnimateEmon = VISIBLE_ANIMATE_EMON/4;
			iVisibleAnimateEplayer = VISIBLE_ANIMATE_EPLAYER/4;
			iVisibleAnimateFplayer = VISIBLE_ANIMATE_FPLAYER/4;
			iVisibleAnimateImpmon = VISIBLE_ANIMATE_IMPMON/4;
			iMaxVisibleAnimate = (iVisibleAnimateNorm+ iVisibleAnimateMust+ iVisibleAnimateEmon+ iVisibleAnimateEplayer+ iVisibleAnimateFplayer+ iVisibleAnimateImpmon);
		}
	}
	
	if (pstPlayer->stOnline.iVAnimate >= iMaxVisibleAnimate || pstMachine->iVPlayer >= iMaxVisibleAnimate) return 0;

	if (pstPlayer->stOnline.iVAnimate >= (iMaxVisibleAnimate-iVisibleAnimateMust) || 
		pstMachine->iVPlayer >= (iMaxVisibleAnimate-iVisibleAnimateMust))
	{
		if (0 > player_view_mon_must(pstEnv, pstPlayer, pstMachine))
		{
			return 0;
		}
	}
	else if (pstPlayer->stOnline.iVAnimate >= iVisibleAnimateNorm || 
		pstMachine->iVPlayer >= iVisibleAnimateNorm)
	{
		if (0 == player_view_mon_must(pstEnv, pstPlayer, pstMachine))
		{
		}
		else
		{
			int iVal1 = is_player_enemy_mon(pstEnv, pstPlayer, pstMachine);
			int iVal2 = is_mon_enemy_player(pstEnv, pstMachine, pstPlayer);

			if (iVal1)
			{
				if (pstPlayer->stOnline.iEplayer >= iVisibleAnimateEplayer)
				{
					return 0;
				}

				piViewCount = &pstPlayer->stOnline.iEplayer;
			}
			else
			{
				if (pstPlayer->stOnline.iFplayer >= iVisibleAnimateFplayer)
				{
					return 0;
				}

				piViewCount = &pstPlayer->stOnline.iFplayer;
			}

			if (iVal2)
			{
				if (pstMachine->iEplayer >= iVisibleAnimateEplayer)
				{
					return 0;
				}

				piOtherViewCount = &pstMachine->iEplayer;
			}
			else
			{
				if (pstMachine->iFplayer >= iVisibleAnimateFplayer)
				{
					return 0;
				}

				piOtherViewCount = &pstMachine->iFplayer;
			}
		}
	}

	if (!binsert_int(&pstMachine->iID, &pstPlayer->stOnline.aiVAnimate, &pstPlayer->stOnline.iVAnimate, sizeof(int), 1)) return -2;
	if (!binsert_int(&pstPlayer->iMemID, &pstMachine->aiVAnimate, &pstMachine->iVPlayer, sizeof(int), 1)) {
		bdelete_int(&pstMachine->iID, &pstPlayer->stOnline.aiVAnimate, &pstPlayer->stOnline.iVAnimate, sizeof(int));
		return -3;
	}

	if (piViewCount && pstPlayer->stOnline.iVAnimate > iVisibleAnimateNorm)
	{
		(*piViewCount)++;
	}

	if (piOtherViewCount && pstMachine->iVPlayer > iVisibleAnimateNorm)
	{
		(*piOtherViewCount)++;
	}

	if (0 == pstMapInst->bBroadcast)
	{
		z_player_view_monster(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMachine);
		machine_view_player(pstEnv->pstAppCtx, pstEnv, pstMachine, pstPlayer);

		if(is_single_mon_machine(pstEnv, pstMachine))
		{
			Player *pstDriver = machine_get_driver(pstEnv, pstMachine);
			if(pstDriver && pstPlayer != pstDriver)
			{
				z_player_view_player(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstDriver);
			}
		}
	}
	
	return 0;
}

int machine_view_machine(ZONESVRENV* pstEnv, Monster *pstMachine1, Monster *pstMachine2, ZONEMAPINST* pstMapInst)
{
	short *piViewCount = NULL;
	short *piOtherViewCount = NULL;
	int iMaxVisibleAnimate = MAX_VISIBLE_ANIMATE;
	int iVisibleAnimateNorm = VISIBLE_ANIMATE_NORM;
	int iVisibleAnimateMust = VISIBLE_ANIMATE_MUST;
	int iVisibleAnimateEmon = VISIBLE_ANIMATE_EMON;
	int iVisibleAnimateEplayer = VISIBLE_ANIMATE_EPLAYER; 
	int iVisibleAnimateFplayer = VISIBLE_ANIMATE_FPLAYER;
	int iVisibleAnimateImpmon = VISIBLE_ANIMATE_IMPMON;

	{
		if (z_pos_in_view_half1(&pstMachine1->stCurrPos, &pstMachine2->stCurrPos))  //小于等于16米
		{
			;
		}
		else if (z_pos_in_view_half2(&pstMachine1->stCurrPos, &pstMachine2->stCurrPos)) // 16米到20米
		{
			iVisibleAnimateNorm = VISIBLE_ANIMATE_NORM *6/7;
			iVisibleAnimateMust = VISIBLE_ANIMATE_MUST/2;
			iVisibleAnimateEmon = VISIBLE_ANIMATE_EMON/2;
			iVisibleAnimateEplayer = VISIBLE_ANIMATE_EPLAYER/2;
			iVisibleAnimateFplayer = VISIBLE_ANIMATE_FPLAYER/2;
			iVisibleAnimateImpmon = VISIBLE_ANIMATE_IMPMON/2;
			iMaxVisibleAnimate = (iVisibleAnimateNorm+ iVisibleAnimateMust+ iVisibleAnimateEmon+ iVisibleAnimateEplayer+ iVisibleAnimateFplayer+ iVisibleAnimateImpmon);
		}
		else //20米到24米
		{
			iVisibleAnimateNorm = VISIBLE_ANIMATE_NORM *3/7;
			iVisibleAnimateMust = VISIBLE_ANIMATE_MUST/4;
			iVisibleAnimateEmon = VISIBLE_ANIMATE_EMON/4;
			iVisibleAnimateEplayer = VISIBLE_ANIMATE_EPLAYER/4;
			iVisibleAnimateFplayer = VISIBLE_ANIMATE_FPLAYER/4;
			iVisibleAnimateImpmon = VISIBLE_ANIMATE_IMPMON/4;
			iMaxVisibleAnimate = (iVisibleAnimateNorm+ iVisibleAnimateMust+ iVisibleAnimateEmon+ iVisibleAnimateEplayer+ iVisibleAnimateFplayer+ iVisibleAnimateImpmon);
		}
	}
	
	if (pstMachine1->iVPlayer >= iMaxVisibleAnimate || pstMachine2->iVPlayer >= iMaxVisibleAnimate) return 0;

	if (pstMachine1->iVPlayer >= (iMaxVisibleAnimate-iVisibleAnimateMust) || 
		pstMachine2->iVPlayer >= (iMaxVisibleAnimate-iVisibleAnimateMust))
	{
		//if (0 > player_view_mon_must(pstEnv, pstPlayer, pstMachine))
		{
			return 0;
		}
	}
	else if (pstMachine1->iVPlayer >= iVisibleAnimateNorm || 
		pstMachine2->iVPlayer >= iVisibleAnimateNorm)
	{
		/*if (0 == player_view_mon_must(pstEnv, pstPlayer, pstMachine))
		{
		}
		else*/
		{
			int iVal1 = is_mon_enemy_mon(pstEnv, pstMachine1, pstMachine2);
			int iVal2 = is_mon_enemy_mon(pstEnv, pstMachine2, pstMachine1);

			if (iVal1)
			{
				if (pstMachine1->iEplayer >= iVisibleAnimateEplayer)
				{
					return 0;
				}

				piViewCount = &pstMachine1->iEplayer;
			}
			else
			{
				if (pstMachine1->iFplayer >= iVisibleAnimateFplayer)
				{
					return 0;
				}

				piViewCount = &pstMachine1->iFplayer;
			}

			if (iVal2)
			{
				if (pstMachine2->iEplayer >= iVisibleAnimateEplayer)
				{
					return 0;
				}

				piOtherViewCount = &pstMachine2->iEplayer;
			}
			else
			{
				if (pstMachine2->iFplayer >= iVisibleAnimateFplayer)
				{
					return 0;
				}

				piOtherViewCount = &pstMachine2->iFplayer;
			}
		}
	}

	if (!binsert_int(&pstMachine2->iID, &pstMachine1->aiVAnimate, &pstMachine1->iVPlayer, sizeof(int), 1)) return -2;
	if (!binsert_int(&pstMachine1->iID, &pstMachine2->aiVAnimate, &pstMachine2->iVPlayer, sizeof(int), 1)) {
		bdelete_int(&pstMachine2->iID, &pstMachine1->aiVAnimate, &pstMachine1->iVPlayer, sizeof(int));
		return -3;
	}

	if (piViewCount && pstMachine1->iVPlayer > iVisibleAnimateNorm)
	{
		(*piViewCount)++;
	}

	if (piOtherViewCount && pstMachine2->iVPlayer > iVisibleAnimateNorm)
	{
		(*piOtherViewCount)++;
	}

	if (0 == pstMapInst->bBroadcast)
	{
		machine_view_monster(pstEnv->pstAppCtx, pstEnv, pstMachine1, pstMachine2);
		machine_view_monster(pstEnv->pstAppCtx, pstEnv, pstMachine2, pstMachine1);
	}
	
	return 0;
}

int machine_view_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMachine, Monster *pstVMon)
{	
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSENTERVIEW *pstEnterView = &pstPkg->Body.EnterView;
	CSMOVEOTHER *pstMoveOther = &pstPkg->Body.MoveOther;
	CSWALK *pstWalk = &pstMoveOther->Data.Walk;
	CSMONSTER *pstCSMon;
	int i;
	Player *pstPlayerTmp;
	Player * apstPlayer[MAX_MON_MACHINE_PASSENGER];
	int iCount = 0;
	
	pstEnterView->ID = pstVMon->iID;
	pstEnterView->x = pstVMon->stCurrPos.X;
	pstEnterView->y = pstVMon->stCurrPos.Y;
	pstEnterView->Dir = pstVMon->bDirection;
	
	if (pstVMon->cType == OBJ_NPC)
	{
		pstEnterView->Type = OBJ_NPC;
		pstCSMon = &pstEnterView->Object.Monster;

		z_make_mon_view(pstAppCtx, pstEnv, pstCSMon, pstVMon);
	}
	else if (pstVMon->cType == OBJ_MONSTER)
	{
		pstEnterView->Type = OBJ_MONSTER;
		pstCSMon = &pstEnterView->Object.Monster;

		z_make_mon_view(pstAppCtx, pstEnv, pstCSMon, pstVMon);
	}
	else
		return -1;

	Z_CSHEAD_INIT(&pstPkg->Head, ENTER_VIEW);

	for (i=0; i<this_machine(pstEnv,pstMachine)->stMachineInfo.MaxPassenger; i++)
	{
		pstPlayerTmp = z_id_player(pstAppCtx, pstEnv, this_machine(pstEnv,pstMachine)->stMachineInfo.PassengerID[i]);
		if (pstPlayerTmp && pstPlayerTmp->eStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			if (pstVMon->bSubType == MON_SUB_TYPE_TRAP)
			{
				Player *pstOwner;

				pstOwner = z_id_player(pstAppCtx, pstEnv, pstVMon->iOwnerID);
				if (pstOwner && (pstOwner != pstPlayerTmp) && (!player_team_same(pstEnv, pstOwner, pstPlayerTmp)))
				{
					continue;
				}
			}
			
			apstPlayer[iCount++] = pstPlayerTmp;
		}
	}

	z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iCount, pstPkg);

	if(is_mon_machine(pstVMon))
	{
		int iCurrPoint;
		Player *pstRidePlayer = machine_get_driver(pstEnv, pstVMon); 
		if (pstRidePlayer && pstRidePlayer->stOnline.cMoving)
		{
			pstMoveOther->ID = pstVMon->iID;
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

			z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iCount, pstPkg);
		}

		if(pstRidePlayer && is_single_mon_machine(pstEnv, pstVMon))
		{
			z_make_player_view(pstAppCtx, pstEnv, pstRidePlayer, pstEnterView);

			Z_CSHEAD_INIT(&pstPkg->Head, ENTER_VIEW);
			z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iCount, pstPkg);
		}
	}

	else if (pstVMon->cMoving)
	{
		pstMoveOther->ID = pstVMon->iID;
		pstMoveOther->x = pstVMon->stCurrPos.X;
		pstMoveOther->y = pstVMon->stCurrPos.Y;
		
		pstMoveOther->MoveType = CS_MOVE_WALK;
		pstWalk->Total = 1;

		pstWalk->Points[0].x = pstVMon->stNextPos.X;
		pstWalk->Points[0].y = pstVMon->stNextPos.Y;

		memcpy(&pstMoveOther->MspdInfo, &pstVMon->stMspdInfo, sizeof(pstMoveOther->MspdInfo));
		
		Z_CSHEAD_INIT(&pstPkg->Head, MOVE_OTHER);

		z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iCount, pstPkg);
	}

	return 0;
}

int mon_norm_view_machine(ZONESVRENV* pstEnv, Monster *pstMon, Monster *pstMachine, ZONEMAPINST* pstMapInst)
{
	short *piViewCount = NULL;
	int iMaxVisibleAnimate = MAX_VISIBLE_ANIMATE;
	int iVisibleAnimateNorm = VISIBLE_ANIMATE_NORM;
	int iVisibleAnimateMust = VISIBLE_ANIMATE_MUST;
	int iVisibleAnimateEmon = VISIBLE_ANIMATE_EMON;
	int iVisibleAnimateEplayer = VISIBLE_ANIMATE_EPLAYER; 
	int iVisibleAnimateFplayer = VISIBLE_ANIMATE_FPLAYER;
	int iVisibleAnimateImpmon = VISIBLE_ANIMATE_IMPMON; 

	if(pstMon->stMap.iID != pstMachine->stMap.iID) return 0;

	if(pstMon->cMoving)
	{
		z_monster_move_curr(pstEnv->pstAppCtx, pstEnv, pstMon, 0);
		if(pstMon->stMap.iID != pstMachine->stMap.iID) return 0;
	}

	if(!z_pos_in_view(&pstMachine->stCurrPos, &pstMon->stCurrPos)) return 0;
	
	if (pstMachine->iVPlayer >= MAX_VISIBLE_ANIMATE)
	{
		return 0;
	}

	{
		if (z_pos_in_view_half1(&pstMachine->stCurrPos, &pstMon->stCurrPos))  //小于等于16米
		{
			;
		}
		else if (z_pos_in_view_half2(&pstMachine->stCurrPos, &pstMon->stCurrPos)) // 16米到20米
		{
			iVisibleAnimateNorm = VISIBLE_ANIMATE_NORM *6/7;
			iVisibleAnimateMust = VISIBLE_ANIMATE_MUST/2;
			iVisibleAnimateEmon = VISIBLE_ANIMATE_EMON/2;
			iVisibleAnimateEplayer = VISIBLE_ANIMATE_EPLAYER/2;
			iVisibleAnimateFplayer = VISIBLE_ANIMATE_FPLAYER/2;
			iVisibleAnimateImpmon = VISIBLE_ANIMATE_IMPMON/2;
			iMaxVisibleAnimate = (iVisibleAnimateNorm+ iVisibleAnimateMust+ iVisibleAnimateEmon+ iVisibleAnimateEplayer+ iVisibleAnimateFplayer+ iVisibleAnimateImpmon);
		}
		else //20米到24米
		{
			iVisibleAnimateNorm = VISIBLE_ANIMATE_NORM *3/7;
			iVisibleAnimateMust = VISIBLE_ANIMATE_MUST/4;
			iVisibleAnimateEmon = VISIBLE_ANIMATE_EMON/4;
			iVisibleAnimateEplayer = VISIBLE_ANIMATE_EPLAYER/4;
			iVisibleAnimateFplayer = VISIBLE_ANIMATE_FPLAYER/4;
			iVisibleAnimateImpmon = VISIBLE_ANIMATE_IMPMON/4;
			iMaxVisibleAnimate = (iVisibleAnimateNorm+ iVisibleAnimateMust+ iVisibleAnimateEmon+ iVisibleAnimateEplayer+ iVisibleAnimateFplayer+ iVisibleAnimateImpmon);
		}
	}

	if (pstMon->cMonMiscFlag & MON_MISC_FLAG_IMPVIEW)
	{
		if (pstMachine->iImpMon >= iVisibleAnimateImpmon)
		{
			return 0;
		}

		piViewCount = &pstMachine->iImpMon;
	}
	else
	{
		int iVal;

		iVal = is_mon_enemy_mon(pstEnv, pstMachine, pstMon);

		if (iVal)
		{
			if (pstMachine->iEmon >= iVisibleAnimateEmon)
			{
				return 0;
			}

			piViewCount = &pstMachine->iEmon;
		}
		else if (pstMon->iVPlayer >= iVisibleAnimateNorm)
		{
			return 0;
		}
	}
		
	if (pstMon->iVPlayer < MAX_MONSTER_VPLAYER)
	{
		if (pstMon->iVPlayer < MAX_MONSTER_VPLAYER_NORM)
		{
			binsert_int(&pstMachine->iID, &pstMon->aiVPlayer, &pstMon->iVPlayer, sizeof(int), 1);
		}
		else
		{
			if (!(pstMon->cMonMiscFlag & MON_MISC_FLAG_IMPVIEW))
			{
				return 0;
			}
		}
	}
					
	if(!binsert_int(&pstMon->iID, &pstMachine->aiVAnimate, &pstMachine->iVPlayer, sizeof(int), 1)) return -2;

	if (piViewCount && pstMachine->iVPlayer > iVisibleAnimateNorm)
	{
		(*piViewCount)++;
	}
				
	if (0 == pstMapInst->bBroadcast && 0 == (pstMon->cMonMiscFlag & MON_MISC_FLAG_MAPBROAD))
	{
		machine_view_monster(pstEnv->pstAppCtx, pstEnv, pstMachine, pstMon);
	}

	return 0;
}

int mon_view_machine(ZONESVRENV* pstEnv, Monster *pstMon, Monster *pstMachine, ZONEMAPINST* pstMapInst)
{
	if (is_mon_machine(pstMon))
	{
		machine_view_machine(pstEnv, pstMon, pstMachine, pstMapInst);
	}
	else
	{
		mon_norm_view_machine(pstEnv, pstMon, pstMachine, pstMapInst);
	}
	
	return 0;
}

int machine_msg_broadcast_except(ZONESVRENV* pstEnv, Monster *pstMachine, CSPKG *pstPkg, int iID)
{
	int i;
	Player *pstPlayerTmp;
	Player * apstPlayer[MAX_MON_MACHINE_PASSENGER];
	int iCount = 0;

	for (i=0; i<this_machine(pstEnv,pstMachine)->stMachineInfo.MaxPassenger; i++)
	{
		pstPlayerTmp = z_id_player(pstEnv->pstAppCtx, pstEnv, this_machine(pstEnv,pstMachine)->stMachineInfo.PassengerID[i]);
		if (pstPlayerTmp && pstPlayerTmp->eStatus == PLAYER_STATUS_ROLE_LOGIN && pstPlayerTmp->iMemID != iID)
		{
			apstPlayer[iCount++] = pstPlayerTmp;
		}
	}

	z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iCount, pstPkg);
	
	return 0;
}

int machine_msg_broadcast(ZONESVRENV* pstEnv, Monster *pstMachine, CSPKG *pstPkg)
{
	return machine_msg_broadcast_except(pstEnv, pstMachine, pstPkg, 0);
}

int obj_leave_machine_view_send(ZONESVRENV* pstEnv, Monster *pstMachine, ZONEMAPINST* pstMapInst, int iID)
{
	Monster *pstMon;
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSLEAVEVIEW *pstLeaveView = &pstPkg->Body.LeaveView;

	if (pstMapInst->bBroadcast)
	{
		return 0;
	}

	pstMon = z_id_monster2(pstEnv->pstAppCtx, pstEnv, iID);
	if(pstMon && (pstMon->cMonMiscFlag & MON_MISC_FLAG_MAPBROAD))
	{
		return 0;
	}

	Z_CSHEAD_INIT(&pstPkg->Head, LEAVE_VIEW);

	if(pstMon && is_single_mon_machine(pstEnv, pstMon))
	{
		Player *pstDriver = machine_get_driver(pstEnv, pstMon);
		if(pstDriver)
		{
			pstLeaveView->ID = pstDriver->iMemID;
			pstLeaveView->HomeFlag = 0;
			machine_msg_broadcast_except(pstEnv, pstMachine, pstPkg, pstDriver->iMemID);
		}
	}

	pstLeaveView->ID = iID;
	pstLeaveView->HomeFlag = 0;
	return machine_msg_broadcast_except(pstEnv, pstMachine, pstPkg, iID);
}

int obj_leave_machine_view(ZONESVRENV* pstEnv, Monster *pstMachine, ZONEMAPINST* pstMapInst, int iID, char cObjType)
{
	int iDel;
	Monster *pstMon;
	Player *pstPlayer;
	
	iDel = bdelete_int(&iID, &pstMachine->aiVAnimate, &pstMachine->iVPlayer, sizeof(int));
	
	if( !iDel)
	{
		return -1;
	}

	switch (cObjType)
	{
		case OBJ_MONSTER:
			pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, iID);
			if (pstMon && is_mon_machine(pstMon))
			{
				bdelete_int(&pstMachine->iID, &pstMon->aiVPlayer, &pstMon->iVPlayer, sizeof(int));
				obj_leave_machine_view_send(pstEnv, pstMon, pstMapInst, pstMachine->iID);
			}

			obj_leave_machine_view_send(pstEnv, pstMachine, pstMapInst, iID);

			break;
		case OBJ_PLAYER:
			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, iID);
			if (pstPlayer && pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
			{
				z_obj_leave_view(pstEnv->pstAppCtx, pstEnv, pstMapInst, pstPlayer, 1, pstMachine->iID, OBJ_MONSTER, 0);
				if(is_single_mon_machine(pstEnv, pstMachine))
				{
					Player *pstDriver = machine_get_driver(pstEnv, pstMachine);
					if(pstDriver && pstDriver != pstPlayer)
					{
						obj_leave_view_send(pstEnv->pstAppCtx, pstEnv, pstMapInst, pstPlayer, pstDriver->iMemID, OBJ_PLAYER, 0);
					}
				}
			}
			break;
		default:
			obj_leave_machine_view_send(pstEnv, pstMachine, pstMapInst, iID);
			break;
	}

	return 0;
}

int obj_leave_machine_view_check(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, Monster *pstMachine, 
								char cType, int iIndex, int iID)
{
	Player *pstVPlayer;
	Monster *pstVMon;
	TMEMBLOCK *pstMemBlock;

	if( iIndex<0 )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "iIndex %d ID %d", iIndex, iID);

		if (obj_leave_machine_view(pstEnv, pstMachine, pstMapInst, iID, OBJ_UNKNOW) == 0) 
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	switch (cType) {
		case OBJ_PLAYER:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				if (obj_leave_machine_view(pstEnv, pstMachine, pstMapInst, iID, cType) == 0) 
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
			pstVPlayer = (Player *)pstMemBlock->szData;
				
			if (pstVPlayer->stRoleData.Map == pstMachine->stMap.iID && z_pos_in_view(&pstMachine->stCurrPos, &pstVPlayer->stRoleData.Pos)) return 0;
			z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst, pstVPlayer, 1, pstMachine->iID, OBJ_MONSTER, 0);
			if (bdelete_int(&iID, &pstMachine->aiVAnimate, &pstMachine->iVPlayer, sizeof(int))) return 1;
			break;
		case OBJ_NPC:
		case OBJ_MONSTER:
			if (cType == OBJ_NPC)
			{
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
			}
			else
			{
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
			}
			if (!pstMemBlock->fValid)
			{
				if (obj_leave_machine_view(pstEnv, pstMachine, pstMapInst, iID, cType) == 0)
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}

			pstVMon = (Monster *)pstMemBlock->szData;

			if (OBJ_MONSTER == cType && AI_STAT_HOME != pstVMon->bAiStat)
			{
				mon_press_update_mon_in(pstEnv, pstVMon, pstMachine);
			}

			if (pstVMon->stMap.iID ==  pstMachine->stMap.iID && z_pos_in_view(&pstMachine->stCurrPos, &pstVMon->stCurrPos)) return 0;

			bdelete_int(&pstMachine->iID, &pstVMon->aiVPlayer, &pstVMon->iVPlayer, sizeof(int));
			if (obj_leave_machine_view(pstEnv, pstMachine, pstMapInst, iID, cType) == 0) return 1;
			break;
	}
	
	return 0;
}

int npc_view_machine_check(ZONESVRENV* pstEnv, Monster *pstNpcMon, Monster *pstMachine)
{
	Player* pstPlayer = NULL;

	pstPlayer = machine_get_driver(pstEnv,pstMachine);

	if(pstNpcMon->cType != OBJ_NPC)
	{
		return 0;
	}
	
	if (0 == pstNpcMon->cOwnerIDType && pstNpcMon->iOwnerID > 0)
	{
		if (!pstPlayer)
		{
			return -1;
		}
		
		if (pstNpcMon->iOwnerID != pstPlayer->iMemID)
		{
			return -1;
		}
	}

	if (1 == pstNpcMon->cOwnerIDType )
	{
		if (!pstPlayer)
		{
			return -1;
		}
		if (pstNpcMon->iOwnerID != pstPlayer->iMemID)
		{
			ZoneTeam *pstZoneTeam = player_team_get(pstEnv, pstPlayer);
			if (NULL == pstZoneTeam || pstZoneTeam->stTeamInfo.TeamID != pstNpcMon->ullOwnerApplyID)
			{
				return -1;
			}
		}
	}

	return 0;
}

int obj_enter_machine_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, Monster *pstMachine, char cType, int iIndex)
{
	Player *pstVPlayer;
	Monster *pstVMon;
	TMEMBLOCK *pstMemBlock;
	Npc *pstVNpc;
	short *piViewCount = NULL;
	int iRet;
			
	if( iIndex<0 )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "iIndex %d", iIndex);
		return -1;
	}

	switch (cType) {
		case OBJ_PLAYER:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
			if (!pstMemBlock->fValid)
				return -1;

			pstVPlayer = (Player *)pstMemBlock->szData;
			if (PLAYER_STATUS_ROLE_LOGIN != pstVPlayer->eStatus || pstVPlayer->stRoleData.Map != pstMachine->stMap.iID) return 0;

			if (is_player_ride_on_machine(pstVPlayer) && pstMachine != machine_get_by_player(pstEnv, pstVPlayer))
			{
				return 0;
			}
			
			if (pstVPlayer->stOnline.cMoving)
			{
				z_player_move_curr(pstAppCtx, pstEnv, pstVPlayer, 0);
				if(pstVPlayer->stRoleData.Map != pstMachine->stMap.iID)
					return 0;
			}

			if (pstVPlayer->stOnline.State & CS_STAT_STALL)
			{
				if (!z_pos_in_view_half(&pstMachine->stCurrPos, &pstVPlayer->stRoleData.Pos)) return 0;
				
				if (pstMachine->cDoSearch == 99) //相同位置搜过
				{
					return 0;
				}

				iRet = 0;
				if (pstMachine->iVPlayer >= MAX_VISIBLE_ANIMATE-VISIBLE_ANIMATE_MUST-VISIBLE_ANIMATE_IMPMON)
				{
					return 0;
				}

				binsert_int(&pstVPlayer->iMemID, &pstMachine->aiVAnimate, &pstMachine->iVPlayer, sizeof(int), 1);
				
				if (pstVPlayer->stOnline.iVAnimate < VISIBLE_ANIMATE_NORM)
				{
					iRet = binsert_int(&pstMachine->iID, &pstVPlayer->stOnline.aiVAnimate, &pstVPlayer->stOnline.iVAnimate, sizeof(int), 1);
				}
				
				if (0 == pstMapInst->bBroadcast)
				{
					machine_view_player(pstEnv->pstAppCtx, pstEnv, pstMachine, pstVPlayer);

					if (iRet)
					{
						z_player_view_monster(pstEnv->pstAppCtx, pstEnv, pstVPlayer, pstMachine);
						if(is_single_mon_machine(pstEnv, pstMachine))
						{
							Player *pstDriver = machine_get_driver(pstEnv, pstMachine);
							if(pstDriver && pstVPlayer != pstDriver)
							{
								z_player_view_player(pstEnv->pstAppCtx, pstEnv, pstVPlayer, pstDriver);
							}
						}
					}
				}

				return 0;
			}
			
			if (!z_pos_in_view(&pstMachine->stCurrPos, &pstVPlayer->stRoleData.Pos)) return 0;

			player_view_machine(pstEnv, pstVPlayer, pstMachine, pstMapInst);

			break;

		case OBJ_MONSTER:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
			if (!pstMemBlock->fValid)
				return -1;
						
			pstVMon = (Monster *)pstMemBlock->szData;

			if(pstVMon->stMap.iID != pstMachine->stMap.iID) return 0;

			if(pstVMon->cMoving)
			{
				z_monster_move_curr(pstAppCtx, pstEnv, pstVMon, 0);
				if(pstVMon->stMap.iID != pstMachine->stMap.iID) return 0;
			}

			if(!z_pos_in_view(&pstMachine->stCurrPos, &pstVMon->stCurrPos)) return 0;

			mon_view_machine(pstEnv, pstVMon, pstMachine, pstMapInst);
			
			mon_press_update_mon_in(pstEnv, pstVMon, pstMachine);

			break;

		case OBJ_NPC:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
			if (!pstMemBlock->fValid)
				return -1;
			
			pstVNpc = (Npc *)pstMemBlock->szData;
			pstVMon = &pstVNpc->stMon;

			if (npc_view_machine_check(pstEnv,pstVMon,pstMachine) < 0)
			{
				return -1;
			}
			
			if(pstVMon->stMap.iID != pstMachine->stMap.iID) return 0;

			if(pstVMon->cMoving)
			{
				z_monster_move_curr(pstAppCtx, pstEnv, pstVMon, 0);
				if(pstVMon->stMap.iID != pstMachine->stMap.iID) return 0;
			}

			if(!z_pos_in_view(&pstMachine->stCurrPos, &pstVMon->stCurrPos)) return 0;

			if (pstVMon->bMoveType == MOVE_TYPE_NO)
			{
				if (pstMachine->cDoSearch == 99) //相同位置搜过
				{
					return 0;
				}
				
				if (pstVMon->iVPlayer < MAX_MONSTER_VPLAYER)
				{
					binsert_int(&pstMachine->iID, &pstVMon->aiVPlayer, &pstVMon->iVPlayer, sizeof(int), 1);
				}
				
				if (pstMachine->iVPlayer < VISIBLE_ANIMATE_NORM)
				{
					binsert_int(&pstVMon->iID, &pstMachine->aiVAnimate, &pstMachine->iVPlayer, sizeof(int), 1);
				}
				
				if (0 == pstMapInst->bBroadcast && 0 == (pstVMon->cMonMiscFlag & MON_MISC_FLAG_MAPBROAD))
				{
					machine_view_monster(pstAppCtx, pstEnv, pstMachine, pstVMon);
				}
				
				return 0;
			}

			if (pstVMon->iVPlayer >= MAX_VISIBLE_ANIMATE)
			{
				return 0;
			}

			if (pstMachine->iImpMon >= VISIBLE_ANIMATE_IMPMON)
			{
				return 0;
			}

			piViewCount = &pstMachine->iImpMon;
			
			if (pstVMon->iVPlayer < MAX_MONSTER_VPLAYER)
			{
				binsert_int(&pstMachine->iID, &pstVMon->aiVPlayer, &pstVMon->iVPlayer, sizeof(int), 1);
			}

			if (pstMachine->iVPlayer < MAX_VISIBLE_ANIMATE)
			{
				if(!binsert_int(&pstVMon->iID, &pstMachine->aiVAnimate, &pstMachine->iVPlayer, sizeof(int), 1))
				{
					return -2;
				}
				else
				{
					if (piViewCount && pstMachine->iVPlayer > VISIBLE_ANIMATE_NORM)
					{
						(*piViewCount)++;
					}
				}
			}

			if (0 == pstMapInst->bBroadcast && 0 == (pstVMon->cMonMiscFlag & MON_MISC_FLAG_MAPBROAD))
			{
				machine_view_monster(pstAppCtx, pstEnv, pstMachine, pstVMon);
			}

			break;
		
		default:
			return 0;
	}

	return 0;
}


int machine_search_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMachine, ZONEMAPINST* pstMapInst, RESPOS *pstPos)
{
	int i, j, iIndex;
	SearchArea stSArea;
	AREADYN *pstArea;
	char cType;
	int iTotal;
	int iID;
	int iEqu;

	iTotal	=	0;

	pstMachine->cDoSearch = 0;
	if (z_pos_search_area(pstAppCtx, pstEnv,  pstMapInst, pstPos, &stSArea) < 0) 
		return -1;

	if (pstMachine->stLastSearchPos.X == pstPos->X && 
		pstMachine->stLastSearchPos.Y == pstPos->Y)
	{
		pstMachine->cDoSearch = 99;
	}
		
	memcpy(&pstMachine->stLastSearchPos, pstPos, sizeof(RESPOS));
	TV_CLONE(pstMachine->stLastSearchTv, pstAppCtx->stCurr);
	
	i = 0;
	while (i < pstMachine->iVPlayer) {
		iID = pstMachine->aiVAnimate[i];
		iIndex = z_id_indextype(pstAppCtx, pstEnv, iID, &cType);
		if (iIndex < 0) {
			if (obj_leave_machine_view(pstEnv, pstMachine, pstMapInst, iID, OBJ_UNKNOW) != 0) i++;
		}
		else if (obj_leave_machine_view_check(pstAppCtx, pstEnv, pstMapInst, pstMachine, cType, iIndex, iID) == 0) i++;
	}
	
	if (pstMachine->iVPlayer <= VISIBLE_ANIMATE_NORM *6/7)
	{
		pstMachine->iEmon = pstMachine->iEplayer = 0;
		pstMachine->iFplayer = pstMachine->iImpMon = 0;
	}
	else if (pstMachine->iVPlayer < MAX_VISIBLE_ANIMATE)
	{
		machine_count_view(pstEnv, pstMachine);
	}
	
	for (i = 0; i < stSArea.iNum; i++) {
		pstArea = stSArea.pastArea[i];
		
		for (j = 0; j < pstArea->iAnimate; j++) {
			if (pstArea->aiAnimate[j] == pstMachine->iID) continue;
			bsearch_int(&pstArea->aiAnimate[j], pstMachine->aiVAnimate, pstMachine->iVPlayer, sizeof(int), &iEqu);
			if (iEqu)
			{
				continue;
			}
					
			iIndex = z_id_indextype(pstAppCtx, pstEnv, pstArea->aiAnimate[j], &cType);
			if (iIndex < 0) continue;

			obj_enter_machine_view(pstAppCtx, pstEnv, pstMapInst ,pstMachine, cType, iIndex);
		}
	}

	pstMachine->cDoSearch = 0;

	return 0;
}

//应该上去之前调用
int machine_player_rideon_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMachine)
{
	char cType;
	int i, iIndex;
	Player *pstVPlayer;
	Monster *pstVMon = NULL;
	TMEMBLOCK *pstMemBlock;
	ZONEMAPINST *pstMapInst;
	ZONEIDX stIdx;

	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos	=	pstPlayer->stOnline.iMapPos;

	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst)
		return -1;
	
	cType = OBJ_UNKNOW;
	for (i = 0; i < pstPlayer->stOnline.iVAnimate;) {
		iIndex = z_id_indextype(pstAppCtx, pstEnv, pstPlayer->stOnline.aiVAnimate[i], &cType);
		if (iIndex >= 0) {
			switch (cType) {
				case OBJ_NPC:
				case OBJ_MONSTER:
					if (cType == OBJ_NPC) 
					{
						pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
					}
					else
					{
						pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
					}
					if (!pstMemBlock->fValid)
					{
						break;
					}

					pstVMon = (Monster *)pstMemBlock->szData;
					if (pstVMon == pstMachine)
					{
						break;
					}
					
					if (OBJ_MONSTER == cType)
					{
						mon_press_delete_player(pstEnv, pstVMon, pstPlayer);
					}
										
					bdelete_int(&pstPlayer->iMemID, &pstVMon->aiVPlayer, &pstVMon->iVPlayer, sizeof(int));
					break;
				case OBJ_PLAYER:
					pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
					if (!pstMemBlock->fValid)
					{
						break;
					}
					
					pstVPlayer = (Player *)pstMemBlock->szData;
					z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst, pstVPlayer, 1, pstPlayer->iMemID, OBJ_PLAYER, 0);
					break;
			}
		}

		if (pstVMon != pstMachine)
		{
			if (z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst, pstPlayer, 1, pstPlayer->stOnline.aiVAnimate[i], cType, 0) == 0) continue;
		}
		
		i++;
	}

	for (i=0; i<pstMachine->iVPlayer; i++)
	{
		iIndex = z_id_indextype(pstAppCtx, pstEnv, pstMachine->aiVAnimate[i], &cType);
		if (iIndex >= 0) {
			switch (cType) {
				case OBJ_NPC:
				case OBJ_MONSTER:
					if (cType == OBJ_NPC) 
					{
						pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
					}
					else
					{
						pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
					}
					if (!pstMemBlock->fValid)
					{
						break;
					}

					pstVMon = (Monster *)pstMemBlock->szData;

					z_player_view_monster(pstAppCtx, pstEnv, pstPlayer, pstVMon);
					if(is_mon_machine(pstVMon) && is_single_mon_machine(pstEnv, pstMachine))
					{
						machine_view_player(pstAppCtx, pstEnv, pstVMon, pstPlayer);
					}
					
					if(is_single_mon_machine(pstEnv, pstVMon))
					{
						Player *pstDriver = machine_get_driver(pstEnv, pstVMon);
						if(pstDriver && pstPlayer != pstDriver)
						{
							z_player_view_player(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstDriver);
						}
					}
					break;
				case OBJ_PLAYER:
					pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
					if (!pstMemBlock->fValid)
					{
						break;
					}
					
					pstVPlayer = (Player *)pstMemBlock->szData;

					if (pstPlayer != pstVPlayer)
					{
						z_player_view_player(pstAppCtx, pstEnv, pstPlayer, pstVPlayer);
						
						if(is_single_mon_machine(pstEnv, pstMachine))
						{
							z_player_view_player(pstAppCtx, pstEnv, pstVPlayer, pstPlayer);
						}
					}
					break;
			}
		}
	}
	
	return 0;
}

//下来之后调
int machine_player_rideoff_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMachine)
{
	char cType;
	int i, iIndex;
	ZONEMAPINST *pstMapInst;
	ZONEIDX stIdx;

	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos	=	pstPlayer->stOnline.iMapPos;
	
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst)
		return -1;

	for (i=0; i<pstMachine->iVPlayer; i++)
	{
		iIndex = z_id_indextype(pstAppCtx, pstEnv, pstMachine->aiVAnimate[i], &cType);
		if (iIndex >= 0) 
		{
			if(cType == OBJ_PLAYER )
			{
				Player *pstVPlayer = (Player *)tmempool_get(pstEnv->pstPlayerPool, iIndex);
				if(NULL == pstVPlayer || pstPlayer == pstVPlayer)
				{
					continue;
				}

				if(is_single_mon_machine(pstEnv, pstMachine))
				{
					obj_leave_view_send(pstAppCtx, pstEnv, pstMapInst, pstVPlayer, pstPlayer->iMemID, OBJ_PLAYER, 0);
				}
			}
			else if(cType == OBJ_MONSTER)
			{
				Monster  *pstMon = (Monster *)tmempool_get(pstEnv->pstMonsterPool, iIndex);
				if(NULL == pstMon || pstMon == pstMachine)
				{
					continue;
				}
			}

			obj_leave_view_send(pstAppCtx, pstEnv, pstMapInst, pstPlayer, pstMachine->aiVAnimate[i], cType, 0);
		}
	}

	map_mon_broadcast(pstEnv, pstPlayer, pstMapInst);
		
	pstPlayer->stOnline.iVAnimate = 0;
	pstPlayer->stOnline.aiVAnimate[pstPlayer->stOnline.iVAnimate++] = pstMachine->iID;
	memset(&pstPlayer->stOnline.stLastSearchPos, 0, sizeof(pstPlayer->stOnline.stLastSearchPos));
	z_player_search_view(pstAppCtx, pstEnv, pstMapInst, &pstPlayer->stRoleData.Pos, pstPlayer);
	
	return 0;
}
