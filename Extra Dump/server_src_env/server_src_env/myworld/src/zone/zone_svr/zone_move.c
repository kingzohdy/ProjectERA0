/*
**  @file $RCSfile: zone_move.c,v $
**  general description of this module
**  $Id: zone_move.c,v 1.426 2014/04/11 08:27:40 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/04/11 08:27:40 $
**  @version $Revision: 1.426 $
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
#include "zone_machine_view.h"
#include "zone_battle.h"
#include "zone_city.h"
#include "zone_strongpoint.h"
#include "zone_gm.h"
#include "zone_span.h"

extern ScriptEnv stSEnv;
extern int g_team_follow_flag;
extern int g_dartcar_allow;

#define MAX_TRUSTPOS_COUNT 10

int z_monster_move_notify_one(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, Player *pstVPlayer);

inline int z_reach_curr_turnpoint(Player *pstPlayer)
{
	memcpy(&pstPlayer->stRoleData.Pos, &pstPlayer->stOnline.stTurnPoints[pstPlayer->stOnline.cCurrPoint], sizeof(RESPOS));
	//if (++pstPlayer->stOnline.cPassPoints > MAX_PASS_POINT) pstPlayer->stOnline.cPassPoints = MAX_PASS_POINT;
	if (++pstPlayer->stOnline.cCurrPoint >= RECORD_TURN) pstPlayer->stOnline.cCurrPoint = 0;
	if (--pstPlayer->stOnline.cFuturePoints == 0) SET_PLAYER_NOMOVE(pstPlayer);
	return 0;
}

double z_move_on_path(Player *pstPlayer, double dMoveDist)
{
        RESPOS *pstNextPos;
        double dPointDist;
        double dHaveMoveDist = 0;
        RESPOS *pstPos = &pstPlayer->stRoleData.Pos;

        while (pstPlayer->stOnline.cMoving && dMoveDist > 0) {
                pstNextPos = &pstPlayer->stOnline.stTurnPoints[pstPlayer->stOnline.cCurrPoint];
                dPointDist = z_distance(pstNextPos, pstPos);

                if (dMoveDist >= dPointDist) {
                        dHaveMoveDist += dPointDist;
                        if (pstPlayer->stOnline.cWalkPath != pstPlayer->stOnline.cLastPath)
                        {
                                pstPlayer->stOnline.cWalkPath = pstPlayer->stOnline.cCurrPoint;
                        }
                        z_reach_curr_turnpoint(pstPlayer);
                } else {
                        if (dPointDist > 0)
                        {
                                pstPos->X = ((pstNextPos->X - pstPos->X) * dMoveDist + pstPos->X * dPointDist) / dPointDist;
                                pstPos->Y = ((pstNextPos->Y - pstPos->Y) * dMoveDist + pstPos->Y * dPointDist) / dPointDist;
                                dHaveMoveDist += dMoveDist;
                        }
                }
                dMoveDist -= dPointDist;
        }

        return dHaveMoveDist;
}

int z_id_mon_broadview(ZONESVRENV* pstEnv, Monster *pstMon)
{
	if (pstMon && (pstMon->cMonMiscFlag & MON_MISC_FLAG_MAPBROAD))
	{
		return 1;
	}
	
	return 0;
	UNUSED(pstEnv);
}

int obj_leave_view_send(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, Player *pstPlayer, int iID, int iObjType, int iNotifyFlag)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSLEAVEVIEW *pstLeaveView = &pstPkg->Body.LeaveView;
	int iSend = 0;
	Monster *pstMon = NULL;

	if (iObjType == OBJ_MONSTER)
	{
		pstMon = z_id_monster(pstAppCtx, pstEnv, iID);
		if (pstMon && is_mon_machine(pstMon))
		{
			obj_leave_machine_view_send(pstEnv, pstMon, pstMapInst, pstPlayer->iMemID);
			if(is_single_mon_machine(pstEnv, pstMon))
			{
				Player *pstDriver = machine_get_driver(pstEnv, pstMon);
				if(pstDriver && pstDriver != pstPlayer)
				{
					obj_leave_view_send(pstEnv->pstAppCtx, pstEnv, pstMapInst, pstPlayer, pstDriver->iMemID, OBJ_PLAYER, 0);
				}
			}
		}
	}
	else if (iObjType == OBJ_NPC)
	{
		pstMon = z_id_monster2(pstAppCtx, pstEnv, iID);
	}
	
	switch(iNotifyFlag)
	{
		case 0:
			if (1 == pstMapInst->bBroadcast && OBJ_ITEM != iObjType)
			{
				iSend = 0;
			}
			else if (pstMon && z_id_mon_broadview(pstEnv, pstMon))
			{
				iSend = 0;
			}
			else
			{
				iSend = 1;
			}
			break;
		case 1:
			iSend = 1;
			break;
		case 2:
			iSend = 0; 
			break;
		default:
			break;
	}

	if (iSend && PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus)
	{
		pstLeaveView->ID = iID;
		pstLeaveView->HomeFlag = 0;

		Z_CSHEAD_INIT(&pstPkg->Head, LEAVE_VIEW);
		
		z_cltmsg_send(pstEnv, pstPlayer, pstPkg);
	}
	
	return 0;
}

//iNotifyFlag:0 看braodcast，1 必发，2必不发
int z_obj_leave_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, Player *pstPlayer, int iAnimate, int iID, int iObjType, int iNotifyFlag)
{	
	int iDel;
	
	if (iAnimate) iDel = bdelete_int(&iID, &pstPlayer->stOnline.aiVAnimate, &pstPlayer->stOnline.iVAnimate, sizeof(int));
	else iDel = bdelete_int(&iID, &pstPlayer->stOnline.aiVInanimate, &pstPlayer->stOnline.iVInanimate, sizeof(int));

	if( !iDel)
	{
		/*tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player %d vanimal id %d leak?", 
					pstPlayer->stRoleData.Uin, iID);*/
		return -1;
	}

	obj_leave_view_send(pstAppCtx, pstEnv, pstMapInst, pstPlayer, iID, iObjType, iNotifyFlag);

	return 0;
}

int z_obj_leave_view_check(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, Player *pstPlayer, 
								char cType, int iIndex, int iID)
{
	Player *pstVPlayer;
	Monster *pstVMon;
	DropItem *pstDItem;
	TMEMBLOCK *pstMemBlock;
	int iNotify;

	if( iIndex<0 )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "iIndex %d ID %d", iIndex, iID);

		if (z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst, pstPlayer, 1, iID, OBJ_UNKNOW, 1) == 0) 
		{
			return 1;
		}
		else if (z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst, pstPlayer, 0, iID, OBJ_UNKNOW, 1) == 0)
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
				if (z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst,  pstPlayer, 1, iID, OBJ_PLAYER, 1) == 0) 
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
			pstVPlayer = (Player *)pstMemBlock->szData;

			/*
			//同一个骑乘上的就不离开视野了。
			if(z_player_same_ride(pstEnv, pstPlayer, pstVPlayer))
			{
				return 0;
			}
			*/

			if (pstVPlayer->stRoleData.Map == pstPlayer->stRoleData.Map && z_pos_in_view(&pstPlayer->stRoleData.Pos, &pstVPlayer->stRoleData.Pos)) return 0;

			z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst, pstVPlayer, 1, pstPlayer->iMemID, OBJ_PLAYER, 0);

			
			if (pstVPlayer->stOnline.State & CS_STAT_STALL)
			{
				iNotify = 2;
			}
			else
			{
				iNotify = 0;
			}

			if (z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst, pstPlayer, 1, iID, OBJ_PLAYER, iNotify) == 0) return 1;
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
				if (z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst, pstPlayer, 1, iID, cType, 1) == 0)
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}

			pstVMon = (Monster *)pstMemBlock->szData;

			if (/*OBJ_MONSTER == cType && */AI_STAT_HOME != pstVMon->bAiStat)
			{
				mon_press_update_player_in(pstEnv, pstVMon, pstPlayer);
			}

			if (pstVMon->stMap.iID ==  pstPlayer->stRoleData.Map && z_pos_in_view(&pstPlayer->stRoleData.Pos, &pstVMon->stCurrPos)) return 0;

			if(OBJ_MONSTER == cType)
			{
				player_boss_info_del(pstPlayer, pstVMon);
			}
			
			bdelete_int(&pstPlayer->iMemID, &pstVMon->aiVPlayer, &pstVMon->iVPlayer, sizeof(int));
			if (z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst, pstPlayer, 1, iID, cType, 0) == 0) return 1;
			break;
		case OBJ_ITEM:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstItemPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				if (z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst , pstPlayer, 0, iID, cType, 1) == 0)
					return 1;
				else
					return 0;
			}
			
			pstDItem = (DropItem *)pstMemBlock->szData;

			//所有者
			if(player_view_booty_valid(pstEnv, pstDItem, pstPlayer))
			{
				if (pstDItem->stMap.iID ==  pstPlayer->stRoleData.Map && 
					z_pos_in_view(&pstPlayer->stRoleData.Pos, &pstDItem->stPos)) 
				{
					return 0;
				}
			}
			
			if (z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst, pstPlayer, 0, iID, cType, 0) == 0) return 1;
			break;
	}
	return 0;
}


int  logic_area_action(ZONESVRENV* pstEnv, Player *pstPlayer, int iFlag, int iType)
{
	// 此函数flag和type失效  
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_LOGIC_AREA;

	pstActionRes->Data.LogicArea.Flag = iFlag;
	pstActionRes->Data.LogicArea.Type = pstPlayer->stOnline.cSecenOverLay;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);

	return 0;
	UNUSED(&iType);
}



int role_leave_logic_area_test(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	/*Player *pstTestPlayer;
	if (pstPlayer->stOnline.iTestMemID > 0)
	{
		pstTestPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.iTestMemID);
		if (pstTestPlayer)
		{
			pstTestPlayer->stOnline.iTestMemID = 0;
			role_test_action(pstEnv, pstTestPlayer, 1);
			z_sys_strf(	pstEnv, pstTestPlayer, SYS_TIP, 
						"玩家%s 离开切磋区域",
						pstPlayer->stRoleData.RoleName);
		}
		role_test_action(pstEnv, pstPlayer, 1);
		pstPlayer->stOnline.iTestMemID = 0;
	}*/
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}

int z_make_player_ride_view(ZONESVRENV* pstEnv, Player *pstVPlayer, CSPLAYER *pstCSPlayer)
{
	pstCSPlayer->RideItemID = 0;
	pstCSPlayer->RideLevel = 0;
	pstCSPlayer->RideView.OwnerID = 0;
	pstCSPlayer->RideView.PassengerNum = 0;
		
	if ( pstVPlayer->stOnline.State & (CS_STAT_RIDE_ON | CS_STAT_FLY | CS_STAT_HITCH_RIDE ))
	{
		pstCSPlayer->RideItemID = pstVPlayer->stOnline.RideItemID;
		pstCSPlayer->RideLevel = pstVPlayer->stOnline.RideLevel;
		pstCSPlayer->RideView.OwnerID = pstVPlayer->stOnline.stRideOnInfo.iOwnerID;

		if (pstVPlayer->stOnline.State & CS_STAT_HITCH_RIDE)
		{
			Player *pstOwner;

			pstOwner = z_id_player(pstEnv->pstAppCtx, pstEnv, pstVPlayer->stOnline.stRideOnInfo.iOwnerID);
			if (pstOwner)
			{
				pstCSPlayer->RideView.PassengerNum = pstOwner->stOnline.stRideOnInfo.iPassengerNum;
				memcpy(&pstCSPlayer->RideView.PassengerID[0], 
							&pstOwner->stOnline.stRideOnInfo.PassengerID[0],
							sizeof(pstCSPlayer->RideView.PassengerID));
			}
		}
		else
		{
			
			pstCSPlayer->RideView.PassengerNum = pstVPlayer->stOnline.stRideOnInfo.iPassengerNum;
			memcpy(&pstCSPlayer->RideView.PassengerID[0], 
						&pstVPlayer->stOnline.stRideOnInfo.PassengerID[0],
						sizeof(pstCSPlayer->RideView.PassengerID));
		}
	}

	return 0;
}

int z_make_player_prepare_view(ZONESVRENV* pstEnv, Player *pstVPlayer, CSPLAYER *pstCSPlayer)
{
	long long  llMs=0;
	struct timeval stDiff;				
	
	memset(&pstCSPlayer->PrepareInfo, 0, sizeof(pstCSPlayer->PrepareInfo));
	
	if(!(pstVPlayer->stOnline.State & CS_STAT_PREPARE))
	{
		return 0;
	}

	pstCSPlayer->PrepareInfoFlag = 1;
	pstCSPlayer->PrepareInfo.SkillID = pstVPlayer->stOnline.stAtkInfo.unSkillID;
	pstCSPlayer->PrepareInfo.Level = pstVPlayer->stOnline.stAtkInfo.unLevel;
	pstCSPlayer->PrepareInfo.AtkPos = pstVPlayer->stOnline.stAtkInfo.stAtkPos;

	TV_DIFF(stDiff, pstEnv->pstAppCtx->stCurr, pstVPlayer->stOnline.stAtkInfo.stPrepare);
	TV_TO_MS(llMs, stDiff);
	if (0 >= llMs)
	{
		return 0;
	}

	pstCSPlayer->PrepareInfo.PrepareSec = stDiff.tv_sec;
	pstCSPlayer->PrepareInfo.PrepareUSec = stDiff.tv_usec;
	
	return 0;
}

int z_make_player_machine_view(ZONESVRENV* pstEnv, Player *pstVPlayer, CSPLAYER *pstCSPlayer)
{
	memset(&pstCSPlayer->MachineInfo, 0, sizeof(pstCSPlayer->MachineInfo));
	
	//怪物机械
	if (pstVPlayer->stOnline.State & CS_STAT_RIDE_MON_MACHINE)
	{
		Monster *pstMon;
		
		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstVPlayer->stOnline.stMachineInfo.iMachineMonID);
		if(pstMon)
		{
			pstCSPlayer->MachineInfo.MachineID = pstMon->iID;
			pstCSPlayer->MachineInfo.MachineDefIdx = pstMon->iDefIdx;
			pstCSPlayer->MachineInfo.HP = pstMon->iHP;
			pstCSPlayer->MachineInfo.MaxHP = pstMon->iMaxHP;
		}
	}
	
	return 0;
}

int z_make_player_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstVPlayer, CSENTERVIEW *pstSvr)
{
	int i;
	CSROLESTATUS *pstCSStatus;
	ZoneStatus *pstZoneStatus;
	ZoneTeam *pstTeam;
	TEAMMEMBER *pstTeamMember;
	CSROLEPETVIEW *pstRolePetView;
	ZoneStatusList *pstZoneStatusList = &pstVPlayer->stOnline.stStatusList;
	ROLEBUDDY *pstBuddy = &pstVPlayer->stRoleData.Buddy;
	CSPLAYER *pstCSPlayer = &pstSvr->Object.Player;

	memset(pstCSPlayer, 0, sizeof(*pstCSPlayer));
	
	pstSvr->ID = pstVPlayer->iMemID;
	pstSvr->x = pstVPlayer->stRoleData.Pos.X;
	pstSvr->y = pstVPlayer->stRoleData.Pos.Y;
	pstSvr->Dir = pstVPlayer->stOnline.bDir;
	pstSvr->Type = OBJ_PLAYER;	

	//多人骑乘相关信息
	z_make_player_ride_view(pstEnv, pstVPlayer, pstCSPlayer);

	pstCSPlayer->Career = pstVPlayer->stRoleData.Career;
	pstCSPlayer->Gender = pstVPlayer->stRoleData.Gender;
	pstCSPlayer->Stat = pstVPlayer->stOnline.State;
	pstCSPlayer->Head = pstVPlayer->stRoleData.Head;
	pstCSPlayer->Face = pstVPlayer->stRoleData.Face;
	pstCSPlayer->Hair = pstVPlayer->stRoleData.Hair;
	pstCSPlayer->Level = pstVPlayer->stRoleData.Level;
	pstCSPlayer->ViewFlag = 0;
	pstCSPlayer->TeamID = 0;
	pstCSPlayer->ShamDead = pstVPlayer->stOnline.fShamDead;
	pstCSPlayer->VipFlag = pstVPlayer->stRoleData.MiscInfo.VipData.VipFlag;
	pstCSPlayer->SpartaFightVal = pstVPlayer->stRoleData.MiscInfo.SpartaMiscInfo.FightVal;

	//目标
	pstCSPlayer->SelectTargetID = pstVPlayer->stOnline.iSelectTarget;

	pstCSPlayer->XuKongEnergy = pstVPlayer->stOnline.stWarOL.iEnergyStat;
	
	// 角色当前的称号
	pstCSPlayer->DesignationID =
		pstVPlayer->stRoleData.MiscInfo.DesignationInfo.CurrentDesignationID;
	pstCSPlayer->MiscName[0] = 0;
	if (pstVPlayer->stOnline.State & CS_STAT_STALL)
	{
		STRNCPY(pstCSPlayer->MiscName, pstVPlayer->stOnline.stStallOnline.szStallName, sizeof(pstCSPlayer->MiscName));
	}
	else
	{
		if(pstBuddy->SpouseNum > 0)
		{
			char szText[64] = {0};
			char szSpouseName[ROLE_NAME_LEN];
			int type = pstBuddy->Spouse[pstBuddy->SpouseNum-1].IntimateColorType;
			
			STRNCPY(szSpouseName, pstBuddy->Spouse[pstBuddy->SpouseNum-1].RoleName, CCH(szSpouseName));
			battle_rolename_remove_prefix(szSpouseName);
		
			switch(type)
			{
				case INTIMATE_COLOR_SPOUSE:
					snprintf(szText, sizeof(szText),LJY_SYSMSG27,szSpouseName);
					break;
				case INTIMATE_COLOR_LOVERS:
					snprintf(szText, sizeof(szText),LJY_SYSMSG26,szSpouseName);
					break;
				default:
					szText[0] = 0;
					break;
			}
			
			if(szText[0])
			{
				STRNCPY(pstCSPlayer->MiscName, szText, sizeof(pstCSPlayer->MiscName));
			}
		}
	}
	
	STRNCPY(pstCSPlayer->Name, pstVPlayer->stRoleData.RoleName, sizeof(pstCSPlayer->Name));
		
	pstCSPlayer->StatusInfo.GoodStatusNum = pstZoneStatusList->bGoodStatusNum;
	for (i=0; i<pstZoneStatusList->bGoodStatusNum; i++)
	{
		pstCSStatus = &pstCSPlayer->StatusInfo.GoodStatusList[i];
		memset(pstCSStatus,0,sizeof(CSROLESTATUS));
		pstZoneStatus = &pstZoneStatusList->astGoodStatus[i];
		
		pstCSStatus->StatusID = pstZoneStatus->unStatusID;
		pstCSStatus->Level = pstZoneStatus->bLevel;
		pstCSStatus->ApplyRelation = pstZoneStatus->cApplyRelation;
		pstCSStatus->Overlay = pstZoneStatus->unOverlay;
		pstCSStatus->ApplyID = pstZoneStatus->iApplyID;
		SET_STATUS_TIME(pstCSStatus->Time, pstZoneStatus->stEnd, pstEnv->pstAppCtx->stCurr);
	}

	pstCSPlayer->StatusInfo.BadStatusNum = pstZoneStatusList->bBadStatusNum;
	for (i=0; i<pstZoneStatusList->bBadStatusNum; i++)
	{
		pstCSStatus = &pstCSPlayer->StatusInfo.BadStatusList[i];
		memset(pstCSStatus,0,sizeof(CSROLESTATUS));
		pstZoneStatus = &pstZoneStatusList->astBadStatus[i];

		pstCSStatus->StatusID = pstZoneStatus->unStatusID;
		pstCSStatus->Level = pstZoneStatus->bLevel;
		pstCSStatus->ApplyRelation = pstZoneStatus->cApplyRelation;
		pstCSStatus->Overlay = pstZoneStatus->unOverlay;
		pstCSStatus->ApplyID = pstZoneStatus->iApplyID;
		
		SET_STATUS_TIME(pstCSStatus->Time, pstZoneStatus->stEnd, pstEnv->pstAppCtx->stCurr);
	}

	pstCSPlayer->StatusInfo.NoLimitStatusNum = pstZoneStatusList->bNoLimitStatusNum;
	for (i=0; i<pstZoneStatusList->bNoLimitStatusNum; i++)
	{
		pstCSStatus = &pstCSPlayer->StatusInfo.NolimitStatusList[i];
		memset(pstCSStatus,0,sizeof(CSROLESTATUS));
		pstZoneStatus = &pstZoneStatusList->astNoLimitStatus[i];

		pstCSStatus->StatusID = pstZoneStatus->unStatusID;
		pstCSStatus->Level = pstZoneStatus->bLevel;
		pstCSStatus->ApplyRelation = pstZoneStatus->cApplyRelation;
		pstCSStatus->Overlay = pstZoneStatus->unOverlay;
		pstCSStatus->ApplyID = pstZoneStatus->iApplyID;
		
		SET_STATUS_TIME(pstCSStatus->Time, pstZoneStatus->stEnd, pstEnv->pstAppCtx->stCurr);
	}

	memcpy(&pstCSPlayer->Lock, &pstVPlayer->stOnline.stLock, sizeof(pstCSPlayer->Lock));
	memcpy(&pstCSPlayer->Avatar, &pstVPlayer->stRoleData.AvatarProf, sizeof(pstCSPlayer->Avatar));

	// 队伍信息
	pstTeam = player_team_get(pstEnv, pstVPlayer);
	if (pstTeam)
	{
		STRNCPY(pstCSPlayer->TeamName, pstTeam->stTeamInfo.TeamName, sizeof(pstCSPlayer->TeamName));
		pstCSPlayer->TeamNum = pstTeam->stTeamInfo.MemberNum;
		
		pstTeamMember = player_team_get_captain(pstTeam);
		if (pstTeamMember && 0 == strcmp(pstTeamMember->MemberName, pstVPlayer->stRoleData.RoleName))
		{
			pstCSPlayer->ViewFlag |= VIEW_FLAG_CAPTAIN;
		}

		pstCSPlayer->TeamID = pstTeam->stTeamInfo.TeamID;
	}
	else
	{
		pstCSPlayer->TeamName[0] = 0;
		pstCSPlayer->TeamNum = 0;
	}

	if (pstEnv->pstConf->CountryVer == COUNTRY_VER_KOREA)
	{
		if (	pstVPlayer->stRoleData.Perm > 0 ||
			(pstVPlayer->UinFlag & UIN_FLAG_GM_ALL) )
		{
			pstCSPlayer->ViewFlag |= VIEW_FLAG_GM;
		}
	}

	// 公会信息
	memset(&pstCSPlayer->ClanView, 0, sizeof(pstCSPlayer->ClanView));
	if(pstVPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID != 0ULL)
	{
		CLANINFO *pstClanInfo = &pstVPlayer->stRoleData.MiscInfo.ClanInfo;
		
		if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
		{
			pstCSPlayer->ClanView.Position = pstClanInfo->Position;
			pstCSPlayer->ClanView.Group = 0;
				
			STRNCPY(pstCSPlayer->ClanView.Name, pstClanInfo->ClanId.Name,
		        	CCH(pstCSPlayer->ClanView.Name));
			pstCSPlayer->ClanView.GID = pstClanInfo->ClanId.GID;
		}
		else
		{
			ZoneClan *pstClan = player_get_clan(pstEnv, pstVPlayer);
			if(pstClan)
			{
				CLANMEM *pstMem = clan_id_mem(pstClan, pstVPlayer->stRoleData.RoleID, 
					&pstVPlayer->stOnline.iClanMemIdx);
				if (pstMem)
				{
					pstCSPlayer->ClanView.Position = pstMem->Position;
					pstCSPlayer->ClanView.Group = pstMem->GroupID;
				}
				
				STRNCPY(pstCSPlayer->ClanView.Name, pstClan->stClan.ClanProf.Name,
			        	CCH(pstCSPlayer->ClanView.Name));
				pstCSPlayer->ClanView.GID = pstClan->stClan.ClanProf.GID;
			}
		}
	}

	if(pstVPlayer->stOnline.ClanItemID >0)
	{
		pstCSPlayer->TmpItem = pstVPlayer->stOnline.ClanItemID;
	}
	else
	{
		pstCSPlayer->TmpItem = pstVPlayer->stRoleData.MiscInfo.TmpSys.TmpItemID;
	}
	
	pstCSPlayer->HP = pstVPlayer->stRoleData.RoleDetail.HPCurr;
	pstCSPlayer->HPMax = pstVPlayer->stOnline.stDyn.stMidAttrList.uiHPMax;
	pstCSPlayer->MP = pstVPlayer->stRoleData.RoleDetail.MPCurr;
	pstCSPlayer->MPMax = pstVPlayer->stOnline.stDyn.stMidAttrList.uiMPMax;

	pstCSPlayer->AutoFlyPath = pstVPlayer->stRoleData.MiscInfo.AutoFlyPath;

	memset(&pstCSPlayer->PetView, 0, sizeof(pstCSPlayer->PetView));
	//宠物和精灵
	pstRolePetView = &pstCSPlayer->PetView;
	pet_view(pstEnv, pstVPlayer, pstRolePetView);

	// 新版精灵
	fairy_view(pstEnv, pstVPlayer, &pstCSPlayer->FairyView);

	// 逻辑区域
	pstCSPlayer->LogicArea = pstVPlayer->stOnline.cSecenOverLay;

	// 切磋
	pstCSPlayer->RoleTest = pstVPlayer->stOnline.stQiecuoInfo.iMemID;
	pstCSPlayer->QiecuoX = pstVPlayer->stOnline.stQiecuoInfo.stTestPos.X;
	pstCSPlayer->QiecuoY = pstVPlayer->stOnline.stQiecuoInfo.stTestPos.Y;

	//  阵营
	pstCSPlayer->CampIdx = pstVPlayer->stOnline.stWarOL.cCampIdx;

	// 安全区
	if (pstVPlayer->stOnline.cSecenOverLay & SCENEOVERLAY_SAFE)
	{
		pstCSPlayer->ViewFlag |= VIEW_FLAG_SAFE;
	}
	else if(pstVPlayer->stOnline.cSecenOverLay & SCENEOVERLAY_PK)
	{
		pstCSPlayer->ViewFlag |= VIEW_FLAG_PK;
	}

	pstCSPlayer->PkInfo.PkMode = pstVPlayer->stOnline.ePkMode;
	pstCSPlayer->PkInfo.PkVal = pstVPlayer->stRoleData.MiscInfo.PkInfo.PkVal;
	pstCSPlayer->PkInfo.PkTime = 0;
	if(pstVPlayer->stOnline.tPkEndTime > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		pstCSPlayer->PkInfo.PkTime = pstVPlayer->stOnline.tPkEndTime -pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	//机械
	z_make_player_machine_view(pstEnv, pstVPlayer, pstCSPlayer);

	//吟唱信息
	pstCSPlayer->PrepareInfoFlag = 0;
	z_make_player_prepare_view(pstEnv, pstVPlayer, pstCSPlayer);

	pstCSPlayer->LeaveState = pstVPlayer->stOnline.iLeaveStat;
	
	pstCSPlayer->MotionInfo = pstVPlayer->stOnline.stMotionInfo;

	pstCSPlayer->ItemView.PreItemID = pstVPlayer->stOnline.stItemUseInfo.iPreItemID;
	pstCSPlayer->ItemView.PreTargetID = pstVPlayer->stOnline.stItemUseInfo.iPreTargetID;
	
	set_show_flag(&pstCSPlayer->ShowFlag, pstVPlayer->stRoleData.MiscInfo.Flag);

	pstCSPlayer->HetiPetID = pstVPlayer->stOnline.stHeti.PetWID;

	//刺探活动
	if (pstVPlayer->stOnline.cMiscFlag & MISC_FLAG_SPY)
	{
		ZONEPWORLDINST *pstZonePworldInstSelf = NULL;
			
		pstZonePworldInstSelf = z_find_player_pworld_inst(pstEnv,pstVPlayer);	
		
		if (pstZonePworldInstSelf &&
			pstZonePworldInstSelf->iType == PWORLD_TYPE_CLAN &&
			pstZonePworldInstSelf->stPworldTypeData.stClan.iID ==
				pstVPlayer->stRoleData.MiscInfo.DailyInfo.SpyInfo.TargetClanID)
		{
			pstCSPlayer->ViewFlag |= VIEW_FLAG_SPY;
		}
	}

	if (pstVPlayer->stOnline.cMiscFlag & MISC_FLAG_ANTI_SPY)
	{
		ZONEPWORLDINST *pstZonePworldInstSelf = NULL;
			
		pstZonePworldInstSelf = z_find_player_pworld_inst(pstEnv,pstVPlayer);	
		
		if (pstZonePworldInstSelf &&
			pstZonePworldInstSelf->iType == PWORLD_TYPE_CLAN &&
			pstZonePworldInstSelf->stPworldTypeData.stClan.iID ==
				(int)pstVPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id)
		{
			pstCSPlayer->ViewFlag |= VIEW_FLAG_ANTI_SPY;
		}
	}

	// 刀疤
	player_make_daoba_view(pstEnv, pstCSPlayer , pstVPlayer);
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_player_move_notify_one(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstVPlayer)
{	
	int i ;
	unsigned int  iCurrPoint;
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSMOVEOTHER *pstMoveOther = &pstPkg->Body.MoveOther;
	CSWALK *pstWalk = &pstMoveOther->Data.Walk;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;
	
	UNUSED(pstAppCtx);
	
	pstMoveOther->ID = pstPlayer->iMemID;
	pstMoveOther->x = pstPlayer->stRoleData.Pos.X;
	pstMoveOther->y = pstPlayer->stRoleData.Pos.Y;
	
	pstMoveOther->MoveType = CS_MOVE_WALK;
	pstWalk->Total = pstPlayer->stOnline.cFuturePoints;
	if (pstWalk->Total > CS_MAX_MOVEPOINTS)
	{
		pstWalk->Total = CS_MAX_MOVEPOINTS;
	}

	if (pstPlayer->stOnline.cFuturePoints > RECORD_TURN)
	{
		tlog_error(pstEnv->pstLogCat, 0,0, "move notify %d player FuturePoints %d", 
			pstPlayer->stRoleData.Uin, pstPlayer->stOnline.cFuturePoints);
		return -1;
	}
	
	for (i = 0; i < pstWalk->Total; i++) {
		iCurrPoint = (pstPlayer->stOnline.cCurrPoint + i) % RECORD_TURN;
		pstWalk->Points[i].x = pstPlayer->stOnline.stTurnPoints[iCurrPoint].X;
		pstWalk->Points[i].y = pstPlayer->stOnline.stTurnPoints[iCurrPoint].Y;
	}

	memcpy(&pstMoveOther->MspdInfo, &pstPlayer->stOnline.stMspdInfo, sizeof(pstMoveOther->MspdInfo));

	if(pstRoleTmpSys->TmpItemID != 0 && 
              pstRoleTmpSys->PatrolID > 0 )
	{
		//玩家跑快些
		PATROLDEF *pstPatrolDef = NULL;
		pstPatrolDef = z_find_patroldef(pstEnv, pstRoleTmpSys->PatrolID, NULL);
		if (NULL != pstPatrolDef)
		{
			pstMoveOther->MspdInfo.NextNum = 1;
			if(pstMoveOther->MspdInfo.MoveSpdNext.MoveSpd)
			{
				pstMoveOther->MspdInfo.MoveSpdNext.MoveSpd *= (1+pstPatrolDef->CltSpeedMul/1000.0); 
			}
			else
			{
				pstMoveOther->MspdInfo.MoveSpdNext.MoveSpd = 
					pstMoveOther->MspdInfo.MoveSpdCurr * (1+pstPatrolDef->CltSpeedMul/1000.0);
			}
		}
	}
	
	Z_CSHEAD_INIT(&pstPkg->Head, MOVE_OTHER);
	z_cltmsg_send(pstEnv, pstVPlayer, pstPkg);
	
	return 0;

}

int z_player_view_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstVPlayer)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSENTERVIEW *pstEnterView = &pstPkg->Body.EnterView;
	int iRet = 0;
	if(pstVPlayer->stOnline.stWarOL.cCampIdx>0)
	{
		if((pstVPlayer->stOnline.State & CS_STAT_QIANXIN) && 
			!is_same_camp_player(pstPlayer, pstVPlayer))
		{
			return 0;
		}
	}
	else
	{
		if ((pstVPlayer->stOnline.State & CS_STAT_QIANXIN) && (0 == player_team_same(pstEnv, pstPlayer, pstVPlayer)))
		{
			return 0;
		}
	}

	if(pstVPlayer->stOnline.State & CS_STAT_AUTOFLY)
	{
		return 0;
	}

	z_make_player_view(pstAppCtx, pstEnv, pstVPlayer, pstEnterView);
	Z_CSHEAD_INIT(&pstPkg->Head, ENTER_VIEW);

	iRet = z_cltmsg_send(pstEnv, pstPlayer, pstPkg);

	//z_sys_strf( pstEnv,  pstPlayer, SYS_MIDDLE, "%sView%s  %d", pstPlayer->stRoleData.RoleName, pstVPlayer->stRoleData.RoleName, iRet);
	
	if (pstVPlayer->stOnline.cMoving)  z_player_move_notify_one(pstAppCtx, pstEnv, pstVPlayer, pstPlayer);

	/*
	if (is_player_ride_on_machine(pstPlayer))  
	{
		z_passenger_move_notify_one(pstEnv, pstVPlayer, pstPlayer);
	}*/
		
	return 0;
}

int z_check_proc(ZONESVRENV* pstEnv, Player *pstPlayer, MapIndex *pstMapIdx, RESPOS *pstPos, int fStartProc)
{
	int i;
	int iMapIndex;
	Area *pstArea;
	PROCEVENT *pstProcEvt;
	
	
	//上一次进入的逻辑区域	
	{
		ScriptIndex * pstIndex;
		int iChgMap= 0;
		int iCount = 0;
		int aiTmpProc[MAX_PLAYER_PROC_NUM];

		for(i = 0 ; i < pstPlayer->stOnline.iProcEvtNum;i++)
		{
			int iIdx = 0;
			if (i >= MAX_PLAYER_PROC_NUM)
			{
				break;
			}
			iIdx = pstPlayer->stOnline.aiProcEvtIdx[i] - 1;
		
			pstProcEvt = (PROCEVENT *)pstEnv->pstMapObj->sProcDef[iIdx];
			pstIndex = (ScriptIndex *)((char *)pstProcEvt + sizeof(*pstProcEvt));

			stSEnv.pstMonster = NULL;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPlayer = pstPlayer;
			stSEnv.pstPworldInst = NULL;
			stSEnv.pstKiller = NULL;
			stSEnv.pstMapinst = NULL;

			//enter map 时触发下 proc leave 
			if(pstProcEvt->MapID != pstMapIdx->stMapDef.MapID || fStartProc)
			{
				z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstProcEvt->Script, pstIndex, PROCEVT_SCRIPT_PROC_LEAVE);

				iChgMap = 1;
			}
			else
			{
				if(!IsPointInProcPoly(&pstProcEvt->ProcPoly, pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y))
				{
					z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstProcEvt->Script, pstIndex, PROCEVT_SCRIPT_PROC_LEAVE);
					
				}
				else
				{
					aiTmpProc[iCount] = iIdx + 1;
					iCount++;

					z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstProcEvt->Script, pstIndex, PROCEVT_SCRIPT_PROC_TIMER);

				}
			}
		}
		
		if (iChgMap)
		{
			pstPlayer->stOnline.iProcEvtNum = 0;
			memset(pstPlayer->stOnline.aiProcEvtIdx,0,sizeof(pstPlayer->stOnline.aiProcEvtIdx));
		}
		else
		{
			if (iCount != pstPlayer->stOnline.iProcEvtNum)
			{
				pstPlayer->stOnline.iProcEvtNum = iCount;
				if (iCount > 0 && iCount <= MAX_PLAYER_PROC_NUM) 
				{
					memcpy(pstPlayer->stOnline.aiProcEvtIdx,aiTmpProc,sizeof(pstPlayer->stOnline.aiProcEvtIdx[0])*iCount);
				}
			}
			
		}
	}


	pstArea = z_pos_area(pstEnv->pstMapObj, pstMapIdx, &pstPlayer->stRoleData.Pos);
	if (NULL == pstArea)
	{
		return -1;
	}

	if (0 == pstArea->stProc.iTotal)
	{
		return 0;
	}
	
	for (i=0; i<pstArea->stProc.iTotal; i++)
	{
		int k = 0;
		int iExist = 0;
		
		for (k = 0; k < pstPlayer->stOnline.iProcEvtNum;k++)
		{
			if (pstArea->stProc.aiIndex[i] == pstPlayer->stOnline.aiProcEvtIdx[k] -1)
			{
				iExist = 1;
				break;
			}
		}

		if (iExist)
		{
			continue;
		}
		
		pstProcEvt = (PROCEVENT *)pstEnv->pstMapObj->sProcDef[pstArea->stProc.aiIndex[i]];
		if (IsPointInProcPoly(&pstProcEvt->ProcPoly, pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y))
		{
			if (pstProcEvt->Script[0] != 0 && pstPlayer->stOnline.iProcEvtNum < MAX_PLAYER_PROC_NUM)
			{
				ScriptIndex * pstIndex;

					
				stSEnv.pstMonster = NULL;
				stSEnv.pstNpc = NULL;
				stSEnv.pstPlayer = pstPlayer;
				stSEnv.pstPworldInst = NULL;
				stSEnv.pstKiller = NULL;
				stSEnv.pstMapinst = NULL;

				pstIndex = (ScriptIndex *)((char *)pstProcEvt + sizeof(*pstProcEvt));
								
				if ( fStartProc ||(pstPos && !IsPointInProcPoly(&pstProcEvt->ProcPoly, pstPos->X, pstPos->Y)) )
				{
					z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstProcEvt->Script, pstIndex, PROCEVT_SCRIPT_PROC_START);
					pstPlayer->stOnline.aiProcEvtIdx[pstPlayer->stOnline.iProcEvtNum] = pstArea->stProc.aiIndex[i] +1;
					pstPlayer->stOnline.iProcEvtNum++;
				}

				continue;
			}

			if (pstProcEvt->LinkMapID >= PWORLD_MAPID_BASE)
			{
				iMapIndex = (pstPlayer->stRoleData.Map & 0xffff0000) | pstProcEvt->LinkMapID;
			}
			else
			{
				iMapIndex = pstProcEvt->LinkMapID;
			}

			if (iMapIndex != 0)
			{
				int iRet = 0;
				g_team_follow_flag = 5;
				g_dartcar_allow = 1;
				iRet = z_player_change_map(pstEnv->pstAppCtx, pstEnv, pstPlayer, iMapIndex, &pstProcEvt->LinkMapPos, pstProcEvt->LinkMapDir, pstProcEvt->CameraDir);
				g_team_follow_flag = 0;
				if (iRet == 0)
				{
					return 0;
				}
			}
			continue;
		}
	}
	
	return 0;
}

int z_obj_area_move(ZONESVRENV* pstEnv, MapIndex* pstMapIdx, ZONEMAPINST* pstMapInst, int iMemID, RESPOS *pstLastPos, int fStartProc)
{
	int iRet;
	RESPOS stNextPos;
	char cType;
	int iIndex;
	Monster *pstMon = NULL;
	Player *pstPlayer = NULL;
	MapIndex *pstMapIdx1;
	
	iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, iMemID, &cType);
	if( iIndex<0 )
	{
		return -1;
	}

	switch(cType)
	{
		case OBJ_PLAYER:
			pstPlayer	=	(Player *)tmempool_get(pstEnv->pstPlayerPool, iIndex);
			if (NULL == pstPlayer)
			{
				return -1;
			}
			stNextPos = pstPlayer->stRoleData.Pos;
			break;
		case OBJ_MONSTER:
			pstMon	=	(Monster *)tmempool_get(pstEnv->pstMonsterPool, iIndex);
			if (NULL == pstMon)
			{
				return -1;
			}
			stNextPos = pstMon->stCurrPos;
			break;
		case OBJ_NPC:
			pstMon	=	(Monster *)tmempool_get(pstEnv->pstNpcPool, iIndex);
			if (NULL == pstMon)
			{
				return -1;
			}
			stNextPos = pstMon->stCurrPos;
			break;
		default:
			return -1;
			break;
	}

	
	if( !z_in_same_area(pstMapIdx, pstEnv->pstAreaDynPool, pstMapInst, pstLastPos, &stNextPos) )
	{
		if (0 > (iRet = z_ani_leave_area(pstMapIdx, pstEnv->pstAreaDynPool, iMemID, pstLastPos, pstMapInst)))
		{
			if(pstPlayer && cType == OBJ_PLAYER)
			{
				tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player(%d) z_ani_leave_area ret %d", pstPlayer->stRoleData.Uin, iRet);
			}
			else if(pstMon && cType == OBJ_MONSTER)
			{
				tlog_error(pstEnv->pstLogCat, 0 , 0, "monster %d %d z_ani_leave_area ret %d", pstMon->iID, pstMon->iDefIdx, iRet);
			}
		}
		
		if (0 > (iRet = z_ani_enter_area(pstMapIdx, pstEnv->pstAreaDynPool, iMemID, &stNextPos, pstMapInst)))
		{
			if(pstPlayer && cType == OBJ_PLAYER)
			{
				tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player(%d) z_ani_enter_area ret %d", pstPlayer->stRoleData.Uin, iRet);
			}
			else if(pstMon && cType == OBJ_MONSTER)
			{
				tlog_error(pstEnv->pstLogCat, 0 , 0, "monster %d %d z_ani_enter_area ret %d", pstMon->iID, pstMon->iDefIdx, iRet);
			}
		}
	}

	if (pstMapIdx->stMapDef.ReferMapID > 0 && pstMapIdx->stMapDef.MapID < PWORLD_MAPID_BASE)
	{
		pstMapIdx1 = z_mappos_index(pstEnv->pstMapObj, &pstMapIdx->stMapDef.ReferMapIDPos, pstMapIdx->stMapDef.ReferMapID);
	}
	else
	{
		pstMapIdx1 = pstMapIdx;
	}

	//区域事件
	if(pstMapIdx1 && cType == OBJ_PLAYER && pstPlayer)
	{
		z_check_proc(pstEnv, pstPlayer, pstMapIdx1, pstLastPos, fStartProc);
	}

	return 0;
}

int z_player_move_curr_bytime(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int  iSearch, struct timeval *pstTime, int iTimeIgnore)
{
	ZONEMAPINST* pstMapInst;
	int iMoveSpeed ;
	long long  llMs = 0;
	double dMoveDist;
	long long  llMs1=0, llMs2=0;
	int iMoveSpeed1;
	RESPOS stPos, stSearchPos;
	MapIndex *pstMapIdx;
	double dHaveMoveDist1;
	double dHaveMoveDist2;
	int iUsedUs;
	int iSec;
	ZONEIDX stIdx;
	struct timeval stDiff;
	struct timeval stTmp;
	CSMSPDINFO *pstMspdInfo = &pstPlayer->stOnline.stMspdInfo;
	RESPOS *pstPos = &pstPlayer->stRoleData.Pos;
//	int iSpd;

	tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
				"before move Uin %d FuturePoints %d LastPathPoints %d WalkPath %d LastPath %d "
				"CurrPoint  %d currx %d curry %d, movetime(%d,%d), movespd %d, next movespd %d",
				pstPlayer->stRoleData.Uin,  pstPlayer->stOnline.cFuturePoints, pstPlayer->stOnline.cLastPathPoints,
				pstPlayer->stOnline.cWalkPath, pstPlayer->stOnline.cLastPath, 
				pstPlayer->stOnline.cCurrPoint, pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y,
				pstPlayer->stOnline.stLastMoveTv.tv_sec, pstPlayer->stOnline.stLastMoveTv.tv_usec,
				pstMspdInfo->MoveSpdCurr, pstMspdInfo->MoveSpdNext.MoveSpd);

	memset(&stTmp,0,sizeof(stTmp));
	
	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos	=	pstPlayer->stOnline.iMapPos;
	pstMapInst	=	z_find_map_inst(pstAppCtx, pstEnv,  &stIdx);
	if(!pstMapInst)
		return -1;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx) return -1;

	if (!pstPlayer->stOnline.cFuturePoints) {
		SET_PLAYER_NOMOVE(pstPlayer);
		if (iSearch && pstPlayer->stOnline.cDoSearch) z_player_search_view(pstAppCtx, pstEnv,  pstMapInst, &pstPlayer->stOnline.stLastSearchPos, pstPlayer);
		return 0;
	}

	TV_DIFF(stDiff, (*pstTime), pstPlayer->stOnline.stLastMoveTv);
	TV_TO_MS(llMs, stDiff);

	if (llMs < 0)
		return 0;
	else if (llMs < iTimeIgnore) {
		if (iSearch && pstPlayer->stOnline.cDoSearch) z_player_search_view(pstAppCtx, pstEnv,  pstMapInst, &pstPlayer->stOnline.stLastSearchPos, pstPlayer);
		return 0;
	}

	memcpy(&stPos, pstPos, sizeof(stPos));

	llMs1 = 0;
	iMoveSpeed1 = 0;
	iMoveSpeed = 0;
	if (pstMspdInfo->NextNum > 0)
	{
		stTmp.tv_sec = pstMspdInfo->MoveSpdNext.SvrTime.BenchTimeSec;
		stTmp.tv_usec = pstMspdInfo->MoveSpdNext.SvrTime.BenchTimeUSec;
		TV_DIFF(stDiff, stTmp, pstPlayer->stOnline.stLastMoveTv);
		TV_TO_MS(llMs2, stDiff);
		if (llMs2 <= 0)
		{
			iMoveSpeed = pstMspdInfo->MoveSpdNext.MoveSpd;
			pstMspdInfo->NextNum = 0;
			pstMspdInfo->MoveSpdCurr = iMoveSpeed;
		}
		else
		{
			llMs1 = llMs - llMs2;
			if (llMs1 >= 0)
			{
				llMs = llMs2;
				iMoveSpeed = pstMspdInfo->MoveSpdCurr;
				iMoveSpeed1 =  pstMspdInfo->MoveSpdNext.MoveSpd;
				pstMspdInfo->NextNum = 0;
				pstMspdInfo->MoveSpdCurr = iMoveSpeed1;
			}
			else
			{
				iMoveSpeed = pstMspdInfo->MoveSpdCurr;
			}
		}
	}
	else
	{
		iMoveSpeed = pstMspdInfo->MoveSpdCurr;
	}

	if (iMoveSpeed <= 0 && iMoveSpeed1 <= 0)
	{
		SET_PLAYER_NOMOVE(pstPlayer);
		pstPlayer->stOnline.stLastSpeed0 = (*pstTime);
		return 0;
	}

	if (llMs > 0 && iMoveSpeed > 0)
	{
		dMoveDist = ( iMoveSpeed * llMs) / 1000;
		dHaveMoveDist1 = z_move_on_path(pstPlayer, dMoveDist);
		dHaveMoveDist2 = dHaveMoveDist1;
		iUsedUs = dHaveMoveDist1*1000000 / iMoveSpeed;
		pstPlayer->stOnline.stLastMoveTv.tv_usec += iUsedUs;
		iSec = pstPlayer->stOnline.stLastMoveTv.tv_usec / (1000000) ;
		pstPlayer->stOnline.stLastMoveTv.tv_sec += iSec;
		pstPlayer->stOnline.stLastMoveTv.tv_usec -= iSec*1000000 ;
	}

	if (llMs1 > 0 && iMoveSpeed1 > 0)
	{
		dMoveDist = ( iMoveSpeed1 * llMs1) / 1000;
		dHaveMoveDist1 = z_move_on_path(pstPlayer, dMoveDist);
		dHaveMoveDist2 += dHaveMoveDist1;
		iUsedUs = dHaveMoveDist1*1000000 / iMoveSpeed1;
		pstPlayer->stOnline.stLastMoveTv.tv_usec += iUsedUs;
		iSec = pstPlayer->stOnline.stLastMoveTv.tv_usec / (1000000) ;
		pstPlayer->stOnline.stLastMoveTv.tv_sec += iSec;
		pstPlayer->stOnline.stLastMoveTv.tv_usec -= iSec*1000000 ;
	}

	tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
				"after move Uin  %d dist %f FuturePoints %d LastPathPoints %d WalkPath %d LastPath %d "
				"CurrPoint  %d currx %d curry %d, movetime(%d,%d), movespd %d, next movespd %d",
				pstPlayer->stRoleData.Uin,  dHaveMoveDist2, pstPlayer->stOnline.cFuturePoints, pstPlayer->stOnline.cLastPathPoints,
				pstPlayer->stOnline.cWalkPath, pstPlayer->stOnline.cLastPath, 
				pstPlayer->stOnline.cCurrPoint, pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y,
				pstPlayer->stOnline.stLastMoveTv.tv_sec, pstPlayer->stOnline.stLastMoveTv.tv_usec,
				pstMspdInfo->MoveSpdCurr, pstMspdInfo->MoveSpdNext.MoveSpd);

	z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstPlayer->iMemID, &stPos, 0);

/*
	if (pstMapIdx && !z_in_same_area(pstMapIdx, pstEnv->pstAreaDynPool, pstMapInst, &stPos, pstPos)) {
		if (0 > (iRet = z_ani_leave_area(pstMapIdx, pstEnv->pstAreaDynPool, pstPlayer->iMemID, &stPos, pstMapInst)))
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player(%d) z_ani_leave_area ret %d",  pstPlayer->stRoleData.Uin, iRet);
		}
		if (0 > (iRet = z_ani_enter_area(pstMapIdx, pstEnv->pstAreaDynPool, pstPlayer->iMemID, &pstPlayer->stRoleData.Pos, pstMapInst)))
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player(%d) z_ani_enter_area ret %d",  pstPlayer->stRoleData.Uin, iRet);
		}
	}
*/
	
	if (pstMapIdx && z_pos_search_pos(pstMapIdx, pstPos, &stSearchPos)) {
		if (!z_same_pos(&stSearchPos, &pstPlayer->stOnline.stLastSearchPos)) {
			if (iSearch) z_player_search_view(pstAppCtx, pstEnv,  pstMapInst, &stSearchPos, pstPlayer);
			else {
				pstPlayer->stOnline.cDoSearch = 1;
				memcpy(&pstPlayer->stOnline.stLastSearchPos, &stSearchPos, sizeof(RESPOS));
			}
		}
	}

	z_mon_machine_move(pstEnv, pstPlayer);

	return 0;
}

int z_player_move_curr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iSearch)
{
	struct timeval stCurr;

	stCurr = pstAppCtx->stCurr;

	z_player_move_curr_bytime(pstAppCtx, pstEnv, pstPlayer, iSearch, &stCurr, 20);

	return 0;
}

int z_monster_move_curr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, int iSearch)
{
	long long llMs=0; 
	int iMoveDist, iPointDist;
	RESPOS stPos, stSearchPos;
	ZONEMAPINST* pstMapInst;
	MapIndex *pstMapIdx;
	int iMoveSpeed;
	struct timeval stDiff;
	long long  llMs1, llMs2;
	int iMoveSpeed1;
	struct timeval stTmp;
	CSMSPDINFO *pstMspdInfo = &pstMon->stMspdInfo;
	int i;

	memset(&stTmp,0,sizeof(stTmp));
	pstMapInst	=	z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
	if(!pstMapInst)
		return -1;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx) return -1;

	TV_DIFF(stDiff, pstAppCtx->stCurr, pstMon->stLastMoveTv);
	TV_TO_MS(llMs, stDiff);
	if (llMs < 50) {
		if (iSearch && pstMon->cDoSearch) z_mon_search_view(pstAppCtx, pstEnv, pstMapInst, &pstMon->stLastSearchPos, pstMon);
		return 0;
	}

	llMs1 = 0;
	iMoveSpeed1 = 0;
	iMoveSpeed = 0;
	
	if (pstMspdInfo->NextNum > 0)
	{
		stTmp.tv_sec = pstMspdInfo->MoveSpdNext.SvrTime.BenchTimeSec;
		stTmp.tv_usec = pstMspdInfo->MoveSpdNext.SvrTime.BenchTimeUSec;
		TV_DIFF(stDiff, stTmp, pstMon->stLastMoveTv);
		TV_TO_MS(llMs2, stDiff);
		if (llMs2 <= 0)
		{
			iMoveSpeed = pstMspdInfo->MoveSpdNext.MoveSpd;
			pstMspdInfo->NextNum = 0;
			pstMspdInfo->MoveSpdCurr = iMoveSpeed;
		}
		else
		{
			llMs1 = llMs - llMs2;
			if (llMs1 >= 0)
			{
				llMs = llMs2;
				iMoveSpeed = pstMspdInfo->MoveSpdCurr;
				iMoveSpeed1 =  pstMspdInfo->MoveSpdNext.MoveSpd;
				pstMspdInfo->NextNum = 0;
				pstMspdInfo->MoveSpdCurr = iMoveSpeed1;
			}
			else
			{
				iMoveSpeed = pstMspdInfo->MoveSpdCurr;
			}
		}
	}
	else
	{
		iMoveSpeed = pstMspdInfo->MoveSpdCurr;
	}

	if (iMoveSpeed <= 0 && iMoveSpeed1 <= 0)
	{
		pstMon->cMoving = 0;
		return 0;
	}

	memcpy(&stPos, &pstMon->stCurrPos, sizeof(RESPOS));

	iMoveDist = (iMoveSpeed * llMs)/1000 + (iMoveSpeed1 * llMs1)/1000;

	iPointDist = z_distance(&pstMon->stNextPos, &pstMon->stCurrPos);

	/*移动Begin: 修改坐标&& 移动格子*/
	if (iMoveDist >= iPointDist)
	{
		pstMon->stCurrPos = pstMon->stNextPos;
		pstMon->cMoving = 0;
	}
	else if( iPointDist )
	{
		pstMon->stCurrPos.X += (pstMon->stNextPos.X - pstMon->stCurrPos.X) * iMoveDist / iPointDist;
		pstMon->stCurrPos.Y += (pstMon->stNextPos.Y - pstMon->stCurrPos.Y) * iMoveDist / iPointDist;
	}
	z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstMon->iID, &stPos, 0);
	/* 移动End */
		
	if (pstMon->iPatrolID > 0 && is_mon_machine(pstMon))
	{
		Player *pstPlayer;

		z_monster_move_notify(pstAppCtx,  pstEnv, pstMon, CS_MOVE_WALK);
	
		for(i=0; i<MAX_MON_MACHINE_PASSENGER; i++)
		{
			if(i >= this_machine(pstEnv,pstMon)->stMachineInfo.MaxPassenger)
			{
				break;
			}

			if(this_machine(pstEnv,pstMon)->stMachineInfo.PassengerID[i] <= 0)
			{
				continue;	
			}

			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, this_machine(pstEnv,pstMon)->stMachineInfo.PassengerID[i]);
			if(pstPlayer && PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus)
			{
				stPos = pstMon->stCurrPos;
				z_passenger_update_pos(pstEnv, pstMapIdx, pstMapInst, pstPlayer, &stPos);
				z_monster_move_notify_one(pstAppCtx,  pstEnv, pstMon, pstPlayer);
			}
		}
	}


	TV_CLONE(pstMon->stLastMoveTv, pstAppCtx->stCurr);
	if (pstMapIdx && z_pos_search_pos(pstMapIdx, &pstMon->stCurrPos, &stSearchPos)) {
		if (!z_same_pos(&stSearchPos, &pstMon->stLastSearchPos)) {
			if (iSearch) z_mon_search_view(pstAppCtx, pstEnv, pstMapInst, &stSearchPos, pstMon);
			else {
				pstMon->cDoSearch = 1;
				memcpy(&pstMon->stLastSearchPos, &stSearchPos, sizeof(RESPOS));
			}
		}
	}

	if(pstMon->bMoveType == MOVE_TYPE_FORCED)
	{
		if(pstMon->stCurrPos.X == pstMon->stForcedPos.X &&
		   pstMon->stCurrPos.Y == pstMon->stForcedPos.Y)
		{
			// 强制移动到指定位置后，回复怪的原来移动类型
			const MONSTERDEF *pstMonDef = z_get_mon_def(pstEnv, pstMon);
			if(pstMonDef)
			{
				pstMon->bMoveType = pstMonDef->MoveType;

				//执行脚本强制移动到终点的事件
				if (pstMon->cMonScriptFlag & MON_SCRIPT_PROC_FORCE_MOVE_END_MAP)
				{
					char szName[128];				
		
					stSEnv.pstMonster = pstMon;
					stSEnv.pstNpc = NULL;
					stSEnv.pstPworldInst = NULL;
					stSEnv.pstPlayer = NULL;
					stSEnv.pstKiller = NULL;
					stSEnv.pstMapinst = NULL;
					
					snprintf(szName, sizeof(szName), "monster%d.mac", pstMonDef->MonsterID);
					szName[sizeof(szName) - 1] = 0;
					
					z_exec_script(pstEnv->pstAppCtx, pstEnv, NULL, szName, &pstMon->stScriptIdx, MON_SCRIPT_PROC_FORCE_MOVE_END);
				}
			}
			
			if( MON_SUB_TYPE_SKILL_DESTORY == pstMon->bSubType ||
				MON_SUB_TYPE_SKILL_NODESTORY == pstMon->bSubType)
			{
				z_mon_destroy(pstEnv, pstMon);
			}
		}
	}
	
	return 0;
}

int z_monster_move_bytime(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, struct timeval *pstCltTime)
{
	long long  llMs=0;
	struct timeval stDiff;
	struct timeval stTmp;

	TV_DIFF(stDiff, (*pstCltTime), pstMon->stLastMoveTv);
	TV_TO_MS(llMs, stDiff);

	if (llMs < 50)
	{
		return 0;
	}

	stTmp = pstAppCtx->stCurr;

	TV_DIFF(stDiff, (*pstCltTime), stTmp);
	TV_TO_MS(llMs, stDiff);
	
	if (abs(llMs) < 800)
	{
		pstAppCtx->stCurr = *pstCltTime;
	}

	z_monster_move_curr(pstAppCtx, pstEnv, pstMon, 0);

	pstAppCtx->stCurr = stTmp;
	
	return 0;
}

int z_monster_move_notify_one(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, Player *pstVPlayer)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSMOVEOTHER *pstMoveOther = &pstPkg->Body.MoveOther;
	CSWALK *pstWalk = &pstMoveOther->Data.Walk;
	UNUSED(pstAppCtx);
	
	pstMoveOther->ID = pstMon->iID;
	pstMoveOther->x = pstMon->stCurrPos.X;
	pstMoveOther->y = pstMon->stCurrPos.Y;
	
	pstMoveOther->MoveType = CS_MOVE_WALK;
	pstWalk->Total = 1;

	pstWalk->Points[0].x = pstMon->stNextPos.X;
	pstWalk->Points[0].y = pstMon->stNextPos.Y;

	memcpy(&pstMoveOther->MspdInfo, &pstMon->stMspdInfo, sizeof(pstMoveOther->MspdInfo));
	
	Z_CSHEAD_INIT(&pstPkg->Head, MOVE_OTHER);

	z_cltmsg_send(pstEnv, pstVPlayer, pstPkg);

	return 0;
}

int z_make_mon_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, CSMONSTER *pstCSMon, Monster *pstVMon)
{	
	int i;
	CSSTATUSINFO *pstCSStatusInfo;
	ZoneMonStatusList *pstZoneStatusList;
	CSROLESTATUS *pstCSRoleStatus;
	ZoneStatus *pstZoneStatus;
	SKILLDEF *pstSkillDef;
	long long  llMs=0;
	struct timeval stDiff;
	MonAtkInfo *pstMonAtkInfo = &pstVMon->stMonAtkInfo;
	UNUSED(pstEnv);

	memset(pstCSMon, 0, sizeof(*pstCSMon));
	
	pstCSMon->Index = pstVMon->iDefIdx;

	if (pstVMon->tBirthEnd > pstAppCtx->stCurr.tv_sec)
	{
		pstCSMon->BirthTimeLeft = pstVMon->tBirthEnd - pstAppCtx->stCurr.tv_sec;
	}
	else
	{
		pstCSMon->BirthTimeLeft = 0;
	}

	//存活时间
	if (pstVMon->tLifeEnd >=  pstAppCtx->stCurr.tv_sec)
	{
		pstCSMon->LifeEndTime = pstVMon->tLifeEnd - pstAppCtx->stCurr.tv_sec;
	}
	else
	{
		pstCSMon->LifeEndTime = -1;
	}
	
	if (MONSTER_DIE == pstVMon->cStat)
	{
		pstCSMon->Stat = CS_STAT_DEAD;
	}
	else
	{
		pstCSMon->Stat = 0;
	}

	pstCSMon->HP = pstVMon->iHP;
	pstCSMon->HPMax = pstVMon->iMaxHP;
	pstCSMon->OwnerID = pstVMon->iOwnerID;
	

	if(is_camp_mon(pstVMon))
	{
		pstCSMon->BootyOwner = 0;
		pstCSMon->BootyOwnerType = 0;
		pstCSMon->BootyOwnerWid = 0;
	}
	else
	{
		pstCSMon->BootyOwner = pstVMon->iFirstAtkerID;
		pstCSMon->BootyOwnerType = pstVMon->cFirstAtkerType;
		pstCSMon->BootyOwnerWid = pstVMon->ullFirstAtkerWid;
	}

	pstCSMon->PatrolID = 0;
	if(pstVMon->bMoveType == MOVE_TYPE_PATROL)
	{
		pstCSMon->PatrolID = pstVMon->iPatrolID;
	}

	pstCSMon->HitTargetID = pstVMon->stAtkTargets.iTargetCur;

	//阵营信息
	pstCSMon->CampIdx = pstVMon->iCampIdx;

	pstCSMon->CityBuildFlag = 0;
	pstCSMon->TopName[0] = 0;
	if(is_city_build_mon(pstVMon))
	{
		ZoneClan *pstClan = !pstVMon->nOwnerFastID ? NULL : z_id_clan(pstEnv, pstVMon->nOwnerFastID);

		pstCSMon->CityBuildFlag = pstVMon->iCityBuildDefID;
		if(pstClan)
		{
			STRNCPY(pstCSMon->TopName, pstClan->stClan.ClanProf.Name, sizeof(pstCSMon->TopName));
		}
	}

	pstCSMon->MonPkFlag = 0;
	if( pstVMon->iPKType != MON_PK_TYPE_NONE)
	{
		pstCSMon->MonPkFlag = 1;
		player_mon_pkmode_make_view( pstAppCtx, pstEnv, pstVMon, &pstCSMon->MonPkModeChg);
	}
	
	pstCSMon->MachineFlag = 0;
	if(is_mon_machine(pstVMon))
	{
		pstCSMon->MachineFlag = 1;
		pstCSMon->MachineInfo = this_machine(pstEnv,pstVMon)->stMachineInfo;
	}

	pstCSMon->FactoryFlag = 0;
	if(is_mon_factory(pstVMon))
	{
		ZoneClan *pstClan = z_id_clan(pstEnv, pstVMon->stCommUnion.stFactory.iClanID);
		if(pstClan)
		{
			STRNCPY(pstCSMon->TopName, pstClan->stClan.ClanProf.Name, sizeof(pstCSMon->TopName));
		}
		
		pstCSMon->FactoryFlag = 1;
		pstCSMon->FactoryInfo.BuildTime = 0;
		pstCSMon->FactoryInfo.ClanGID = 0ULL;
		if(pstVMon->stCommUnion.stFactory.tBuildTime > pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			pstCSMon->FactoryInfo.BuildTime = pstVMon->stCommUnion.stFactory.tBuildTime -pstEnv->pstAppCtx->stCurr.tv_sec;
		}
		
	}

	//文字密码
	if(pstVMon->iCommType == MON_UNION_TYPE_WORD_PWD)
	{
		WORDPWDDEF *pstWordDef = z_find_word_pwd_def(pstEnv, pstVMon->stCommUnion.stWordPwd.iLine);
		int iWordNameNum = 0;
		if(pstWordDef != NULL)
		{
			iWordNameNum = pstVMon->stCommUnion.stWordPwd.iIndex;
			STRNCPY(pstCSMon->TopName, pstWordDef->WordMonInfo[iWordNameNum-1].Name, sizeof(pstCSMon->TopName));
		}
	}

	pstCSStatusInfo = &pstCSMon->StatusInfo;
	pstZoneStatusList = &pstVMon->stStatusList;
	
	pstCSStatusInfo->GoodStatusNum = pstZoneStatusList->bGoodStatusNum;
	for (i=0; i<pstCSStatusInfo->GoodStatusNum; i++)
	{
		pstCSRoleStatus = &pstCSStatusInfo->GoodStatusList[i];
		memset(pstCSRoleStatus,0,sizeof(CSROLESTATUS));
		pstZoneStatus = &pstZoneStatusList->astGoodStatus[i];

		pstCSRoleStatus->Level = pstZoneStatus->bLevel;
		pstCSRoleStatus->StatusID = pstZoneStatus->unStatusID;
		pstCSRoleStatus->Overlay = pstZoneStatus->unOverlay;
		pstCSRoleStatus->ApplyID = pstZoneStatus->iApplyID;
		pstCSRoleStatus->ApplyRelation = pstZoneStatus->cApplyRelation;
		if (pstZoneStatus->stEnd.tv_sec-pstAppCtx->stCurr.tv_sec <= 0)
		{
			pstCSRoleStatus->Time = 0;
		}
		else
		{
			pstCSRoleStatus->Time = pstZoneStatus->stEnd.tv_sec-pstAppCtx->stCurr.tv_sec;
		}
	}

	pstCSStatusInfo->BadStatusNum = pstZoneStatusList->bBadStatusNum;
	for (i=0; i<pstCSStatusInfo->BadStatusNum; i++)
	{
		pstCSRoleStatus = &pstCSStatusInfo->BadStatusList[i];
		memset(pstCSRoleStatus,0,sizeof(CSROLESTATUS));
		pstZoneStatus = &pstZoneStatusList->astBadStatus[i];

		pstCSRoleStatus->Level = pstZoneStatus->bLevel;
		pstCSRoleStatus->StatusID = pstZoneStatus->unStatusID;
		pstCSRoleStatus->Overlay = pstZoneStatus->unOverlay;
		pstCSRoleStatus->ApplyID = pstZoneStatus->iApplyID;
		pstCSRoleStatus->ApplyRelation = pstZoneStatus->cApplyRelation;
		if(pstZoneStatus->stEnd.tv_sec-pstAppCtx->stCurr.tv_sec <= 0)
		{
			pstCSRoleStatus->Time = 0;
		}
		else
		{
			pstCSRoleStatus->Time = pstZoneStatus->stEnd.tv_sec-pstAppCtx->stCurr.tv_sec;
		}
	}

	pstCSStatusInfo->NoLimitStatusNum = 0;
	
	pstCSMon->Level = pstVMon->stMonDynAttr.Level;	
	STRNCPY(pstCSMon->OwnerName, pstVMon->szOwner, sizeof(pstCSMon->OwnerName));
	if (pstVMon->cDartCarNotAtk > 0)
	{
		STRNCPY(pstCSMon->OwnerName, pstCSMon->MachineInfo.OwnerName, sizeof(pstCSMon->OwnerName));
	}

	if(	pstVMon->bSubType == MON_SUB_TYPE_TEAM_CAR || 
		pstVMon->bSubType == MON_SUB_TYPE_TEAM_SAFECAR ||
		pstVMon->bSubType == MON_SUB_TYPE_COLLECT)
	{
		pstCSMon->OwnerTeamID = pstVMon->stTeamInfo.ullTeamID;
	}
	else
	{
		pstCSMon->OwnerTeamID = 0;
	} 

	//以下是吟唱攻击数据
	pstCSMon->PrepareAtkFlag = 0;
	pstSkillDef = z_find_skill(pstEnv, pstMonAtkInfo->unSkillID, pstMonAtkInfo->bSkillLevel, &pstMonAtkInfo->iSkillIdx);
	if (pstSkillDef && pstSkillDef->SkillType == SKILL_TYPE_PREPARE_ATK )
	{
		TV_DIFF(stDiff, pstVMon->stMonAtkInfo.stAtkStatEnd, pstEnv->pstAppCtx->stCurr);
		TV_TO_MS(llMs, stDiff);
		if (0 >= llMs)
		{
			return 0;
		}
		pstCSMon->PrepareAtkFlag = 1;
		//pstCSMon->MonPrepareAtkInfo.sec = pstEnv->pstAppCtx->stCurr.tv_sec;
		//pstCSMon->MonPrepareAtkInfo.usec = pstEnv->pstAppCtx->stCurr.tv_usec;
		pstCSMon->MonPrepareAtkInfo.sec = stDiff.tv_sec;
		pstCSMon->MonPrepareAtkInfo.usec = stDiff.tv_usec;		
		pstCSMon->MonPrepareAtkInfo.SkillID = pstMonAtkInfo->unSkillID;
		pstCSMon->MonPrepareAtkInfo.Level = pstMonAtkInfo->bSkillLevel;
		pstCSMon->MonPrepareAtkInfo.AtkPos = pstMonAtkInfo->stAtkPos;		
	}

	strong_make_banner_view( pstEnv, pstCSMon, pstVMon);

	if(	pstVMon->bSubType == MON_SUB_TYPE_RONGLU)
	{
		pstCSMon->Rongluinfo.Num = pstVMon->stCommUnion.stRonglu.iPower;
		pstCSMon->Rongluinfo.Max = pstVMon->stCommUnion.stRonglu.iMaxPower;
	}
	else if (pstVMon->bSubType == MON_SUB_TYPE_LOST_TOTEM)
	{
		pstCSMon->LostToteminfo.Num = pstVMon->stCommUnion.stLostTotem.iPower;
		pstCSMon->LostToteminfo.Max = pstVMon->stCommUnion.stLostTotem.iMaxPower;
	}
	return 0;
}

int z_make_npc_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, CSNPC *pstCSNpc, Monster *pstVMon)
{
	pstCSNpc->Index = pstVMon->iDefIdx;
	pstCSNpc->PatrolID = 0;
	if(pstVMon->bMoveType == MOVE_TYPE_PATROL)
	{
		pstCSNpc->PatrolID = pstVMon->iPatrolID;
	}

	pstCSNpc->CampIdx = pstVMon->iCampIdx;

	pstCSNpc->NpcPkFlag = 0;
	if( pstVMon->iPKType != MON_PK_TYPE_NONE)
	{
		pstCSNpc->NpcPkFlag = 1;
		player_mon_pkmode_make_view( pstAppCtx, pstEnv, pstVMon, &pstCSNpc->PKModeChg);
	}
	
	return 0;
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	
}
 
int z_player_view_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstVMon)
{	
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSENTERVIEW *pstEnterView = &pstPkg->Body.EnterView;
	//CSNPC *pstCSNpc ;	//comment by paraunused.pl
	CSMONSTER *pstCSMon;
	UNUSED(pstEnv);
	
	if (pstVMon->bSubType == MON_SUB_TYPE_TRAP)
	{
		Player *pstOwner;

		pstOwner = z_id_player(pstAppCtx, pstEnv, pstVMon->iOwnerID);
		if (pstOwner && (pstOwner != pstPlayer) && (!player_team_same(pstEnv, pstOwner, pstPlayer)))
		{
			return 0;
		}
	}
		
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

	z_cltmsg_send(pstEnv, pstPlayer, pstPkg);

      /*monster view player
	if (pstVMon->iVPlayer < MAX_MONSTER_VPLAYER && binsert_int(&pstPlayer->iMemID, &pstVMon->aiVPlayer, &pstVMon->iVPlayer, sizeof(int), 1)) 
	{
		
	}*/

	if(is_mon_machine(pstVMon))
	{
		z_machine_move_notify_one(pstEnv, pstVMon, pstPlayer);
	}
	else if (pstVMon->cMoving)
	{
		z_monster_move_notify_one(pstAppCtx, pstEnv, pstVMon, pstPlayer);
	}

	return 0;
}

int z_player_view_item(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstDItem)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSENTERVIEW *pstEnterView = &pstPkg->Body.EnterView;
	CSDROPITEM *pstCSItem = &pstEnterView->Object.Item;

	memset(pstCSItem,0,sizeof(CSDROPITEM));
	pstEnterView->ID = pstDItem->iID;
	pstEnterView->x = pstDItem->stPos.X;
	pstEnterView->y = pstDItem->stPos.Y;
	pstEnterView->Dir = pstDItem->bDir;

	pstEnterView->Type = OBJ_ITEM;
	pstCSItem->Index = pstDItem->iDefIdx;
	pstCSItem->FromID = pstDItem->iFromID;
	
	if(DROP_ITEM_GRID == pstDItem->cType )
	{
		pstCSItem->RoleID = pstDItem->stObj.stBootyGrid.ullRoleID;
		pstCSItem->TeamID = pstDItem->stObj.stBootyGrid.ullTeamID;
		pstCSItem->DropType = pstDItem->stObj.stBootyGrid.bType;
		pstCSItem->ShowFlag = pstDItem->stObj.stBootyGrid.szShowFlag;
		pstCSItem->ClanGID = pstDItem->stObj.stBootyGrid.ullClanGID;
		if(is_money_booty_grid(&pstDItem->stObj.stBootyGrid.stGrid))
		{
			pstCSItem->ItemNum = pstDItem->stObj.stBootyGrid.stGrid.GridData.RoleItem.Flag;
		}
		else
		{
			pstCSItem->ItemNum = pstDItem->stObj.stBootyGrid.stGrid.GridData.RoleItem.Num;
		}

		if(pstDItem->tPickLimitTime > pstAppCtx->stCurr.tv_sec)
		{
			pstCSItem->PickLimitTime = pstDItem->tPickLimitTime -pstAppCtx->stCurr.tv_sec;
		}

		pstCSItem->Roll = pstDItem->stObj.stBootyGrid.bRoll;
		pstCSItem->CollectTime = pstDItem->stObj.stBootyGrid.bCollectTime;
		pstCSItem->ArmStar = 0;
		if(pstDItem->stObj.stBootyGrid.stGrid.Type == GRID_ITEM_ARM)
		{
			pstCSItem->ArmStar = pstDItem->stObj.stBootyGrid.stGrid.GridData.RoleArm.StarLv;
		}
	}
	
	if(DROP_ITEM_TMP == pstDItem->cType )
	{	//临时道具吧属主下发下
		STRNCPY(pstCSItem->OwnerName,pstDItem->stObj.stTmpItem.OwnerName,CCH(pstCSItem->OwnerName));
		pstCSItem->TmpEnerge = pstDItem->stObj.stTmpItem.iEnerge;
	}
	
	if ((pstDItem->stDropTime.tv_sec == pstAppCtx->stCurr.tv_sec) && (pstAppCtx->stCurr.tv_usec - pstDItem->stDropTime.tv_usec < 200))
	{
		pstCSItem->Flag = 1;
	}
	else
	{
		pstCSItem->Flag = 0;
	}

	pstCSItem->DropItemType = pstDItem->cType;

	Z_CSHEAD_INIT(&pstPkg->Head, ENTER_VIEW);

	z_cltmsg_send(pstEnv, pstPlayer, pstPkg);

	return 0;
}

int player_view_player_must(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstVPlayer)
{	
	ROLEMISCINFO *pstRoleMiscInfo1 = &pstPlayer->stRoleData.MiscInfo;
	ROLEMISCINFO *pstRoleMiscInfo2 = &pstVPlayer->stRoleData.MiscInfo;
	UNUSED(pstEnv);

	//同队伍应优先
	if (pstRoleMiscInfo1->TeamID != 0 && pstRoleMiscInfo1->TeamID == pstRoleMiscInfo2->TeamID)
	{
		return 0;
	}

	//同一个怪物机械上
	if(is_together_ride_machine(pstPlayer, pstVPlayer))
	{
		return 0;
	}

	//携带乘客的骑乘相互优先
	if(z_player_same_ride(pstEnv, pstPlayer, pstVPlayer))
	{
		return 0;
	}

	//切磋对手
	if(pstPlayer->iMemID ==pstVPlayer->stOnline.stQiecuoInfo.iMemID)
	{
		return 0;
	}

	return -1;
}

int player_view_mon_must(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstVMon)
{	
	UNUSED(pstEnv);
	if (pstVMon->iOwnerID ==  pstPlayer->iMemID)
	{
		return 0;
	}

	if (pstVMon == machine_get_by_player(pstEnv, pstPlayer))
	{
		return 0;
	}
	
	return -1;
}

int player_view_item_imp(ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstDropItem)
{	
	UNUSED(pstEnv);
	
	if(pstDropItem->cType == DROP_ITEM_GRID)
	{
		if(pstDropItem->stObj.stBootyGrid.ullRoleID == pstPlayer->stRoleData.RoleID)
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
	
	return -1;
}

int player_count_view(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	int iIndex;
	char cType;
	TMEMBLOCK *pstMemBlock;
	Player *pstVPlayer;
	Monster *pstVMon;
	Npc *pstNpc;
		
	pstPlayer->stOnline.iEmon = pstPlayer->stOnline.iEplayer = 0;
	pstPlayer->stOnline.iFplayer = pstPlayer->stOnline.iImpMon = 0;

	for (i=VISIBLE_ANIMATE_NORM * 6/7; i<pstPlayer->stOnline.iVAnimate; i++)
	{
		iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.aiVAnimate[i], &cType);

		if (iIndex < 0) continue;

		switch(cType)
		{
			case OBJ_PLAYER:
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
				if (!pstMemBlock->fValid)
					continue;

				pstVPlayer = (Player *)pstMemBlock->szData; 
				if (PLAYER_STATUS_ROLE_LOGIN != pstVPlayer->eStatus || pstVPlayer->stRoleData.Map != pstPlayer->stRoleData.Map)
				{
					continue;
				}

				if (is_player_ride_on_machine(pstVPlayer) || 0 == player_view_player_must(pstEnv, pstPlayer, pstVPlayer) || 
					(pstVPlayer->stOnline.State & CS_STAT_STALL))
				{
					continue;
				}
				
				if (is_player_enemy_player(pstEnv, pstPlayer, pstVPlayer))
				{
					pstPlayer->stOnline.iEplayer++;
				}
				else
				{
					pstPlayer->stOnline.iFplayer++;
				}
				
				break;
			case OBJ_MONSTER:
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
				if (!pstMemBlock->fValid)
					continue;
							
				pstVMon = (Monster *)pstMemBlock->szData;
				if (pstVMon->cMonMiscFlag & MON_MISC_FLAG_IMPVIEW)
				{
					pstPlayer->stOnline.iImpMon++;
				}
				else if (is_player_enemy_mon(pstEnv, pstPlayer, pstVMon))
				{
					pstPlayer->stOnline.iEmon++;
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
				
				pstPlayer->stOnline.iImpMon++;
				break;
			default:
				break;
		}
	}
		
	return 0;
}

int z_obj_enter_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, Player *pstPlayer, char cType, int iIndex)
{
	Player *pstVPlayer;
	Monster *pstVMon;
	DropItem *pstDItem;
	TMEMBLOCK *pstMemBlock;
	Npc *pstVNpc;
	int iMust;
	int iRet;
	short *piViewCount = NULL;
	short *piOtherViewCount = NULL;
	int iMaxVisibleAnimate = MAX_VISIBLE_ANIMATE;
	int iVisibleAnimateNorm = VISIBLE_ANIMATE_NORM;
	int iVisibleAnimateMust = VISIBLE_ANIMATE_MUST;
	int iVisibleAnimateEmon = VISIBLE_ANIMATE_EMON;
	int iVisibleAnimateEplayer = VISIBLE_ANIMATE_EPLAYER; 
	int iVisibleAnimateFplayer = VISIBLE_ANIMATE_FPLAYER;
	int iVisibleAnimateImpmon = VISIBLE_ANIMATE_IMPMON; 
			
	if( iIndex<0 )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "iIndex %d", iIndex);
		return -1;
	}

	if (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus ||
		(is_player_ride_on_machine(pstPlayer) && cType != OBJ_ITEM))
	{
		return 0;
	}

	switch (cType) {
		case OBJ_PLAYER:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
			if (!pstMemBlock->fValid)
				return -1;

			pstVPlayer = (Player *)pstMemBlock->szData;
			if (PLAYER_STATUS_ROLE_LOGIN != pstVPlayer->eStatus || pstVPlayer->stRoleData.Map != pstPlayer->stRoleData.Map) return 0;

			if (is_player_ride_on_machine(pstVPlayer))
			{
				return 0;
			}
			
			if (pstVPlayer->stOnline.cMoving)
			{
				z_player_move_curr(pstAppCtx, pstEnv, pstVPlayer, 0);
				if(pstVPlayer->stRoleData.Map != pstPlayer->stRoleData.Map)
					return 0;
			}

			if (pstVPlayer->stOnline.State & CS_STAT_STALL)
			{
				if (!z_pos_in_view_half(&pstPlayer->stRoleData.Pos, &pstVPlayer->stRoleData.Pos)) return 0;
				
				if (pstPlayer->stOnline.cDoSearch == 99) //相同位置搜过
				{
					return 0;
				}

				iRet = 0;
				if (pstPlayer->stOnline.iVAnimate >= MAX_VISIBLE_ANIMATE-VISIBLE_ANIMATE_MUST-VISIBLE_ANIMATE_IMPMON)
				{
					return 0;
				}

				binsert_int(&pstVPlayer->iMemID, &pstPlayer->stOnline.aiVAnimate, &pstPlayer->stOnline.iVAnimate, sizeof(int), 1);
				
				if (pstVPlayer->stOnline.iVAnimate < VISIBLE_ANIMATE_NORM)
				{
					iRet = binsert_int(&pstPlayer->iMemID, &pstVPlayer->stOnline.aiVAnimate, &pstVPlayer->stOnline.iVAnimate, sizeof(int), 1);
				}
				
				if (0 == pstMapInst->bBroadcast)
				{
					z_player_view_player(pstAppCtx, pstEnv, pstPlayer, pstVPlayer);
					if (iRet)
					{
						z_player_view_player(pstAppCtx, pstEnv, pstVPlayer, pstPlayer);
					}
				}

				return 0;
			}
			
			if (!z_pos_in_view(&pstPlayer->stRoleData.Pos, &pstVPlayer->stRoleData.Pos)) return 0;

			//简单的分圈外圈EPlayer FPlayer 计算不精确
			{
				if (z_pos_in_view_half1(&pstPlayer->stRoleData.Pos, &pstVPlayer->stRoleData.Pos))  //小于等于16米
				{
					;
				}
				else if (z_pos_in_view_half2(&pstPlayer->stRoleData.Pos, &pstVPlayer->stRoleData.Pos)) // 16米到20米
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

			if (pstPlayer->stOnline.iVAnimate >= iMaxVisibleAnimate || pstVPlayer->stOnline.iVAnimate >= iMaxVisibleAnimate) return 0;

			if (pstPlayer->stOnline.iVAnimate >= (iMaxVisibleAnimate-iVisibleAnimateMust) || 
				pstVPlayer->stOnline.iVAnimate >= iMaxVisibleAnimate-iVisibleAnimateMust)
			{
				if (0 > player_view_player_must(pstEnv, pstPlayer, pstVPlayer))
				{
					return 0;
				}
			}
			else if (pstPlayer->stOnline.iVAnimate >= iVisibleAnimateNorm || 
				pstVPlayer->stOnline.iVAnimate >= iVisibleAnimateNorm)
			{
				if (0 == player_view_player_must(pstEnv, pstPlayer, pstVPlayer))
				{
				}
				else
				{
					int iVal1 = is_player_enemy_player(pstEnv, pstPlayer, pstVPlayer);
					int iVal2 = is_player_enemy_player(pstEnv, pstVPlayer, pstPlayer);

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
						if (pstVPlayer->stOnline.iEplayer >= iVisibleAnimateEplayer)
						{
							return 0;
						}

						piOtherViewCount = &pstVPlayer->stOnline.iEplayer;
					}
					else
					{
						if (pstVPlayer->stOnline.iFplayer >= iVisibleAnimateFplayer)
						{
							return 0;
						}

						piOtherViewCount = &pstVPlayer->stOnline.iFplayer;
					}
				}
			}

			if (!binsert_int(&pstVPlayer->iMemID, &pstPlayer->stOnline.aiVAnimate, &pstPlayer->stOnline.iVAnimate, sizeof(int), 1)) return -2;
			if (!binsert_int(&pstPlayer->iMemID, &pstVPlayer->stOnline.aiVAnimate, &pstVPlayer->stOnline.iVAnimate, sizeof(int), 1)) {
				bdelete_int(&pstVPlayer->iMemID, &pstPlayer->stOnline.aiVAnimate, &pstPlayer->stOnline.iVAnimate, sizeof(int));
				return -3;
			}

			if (piViewCount && pstPlayer->stOnline.iVAnimate > iVisibleAnimateNorm)
			{
				(*piViewCount)++;
			}

			if (piOtherViewCount && pstVPlayer->stOnline.iVAnimate > iVisibleAnimateNorm)
			{
				(*piOtherViewCount)++;
			}

			if (0 == pstMapInst->bBroadcast)
			{
				z_player_view_player(pstAppCtx, pstEnv, pstPlayer, pstVPlayer);
				z_player_view_player(pstAppCtx, pstEnv, pstVPlayer, pstPlayer);
			}

			break;

		case OBJ_MONSTER:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
			if (!pstMemBlock->fValid)
				return -1;
						
			pstVMon = (Monster *)pstMemBlock->szData;

			if(pstVMon->stMap.iID != pstPlayer->stRoleData.Map) return 0;

			if(pstVMon->cMoving)
			{
				z_monster_move_curr(pstAppCtx, pstEnv, pstVMon, 0);
				if(pstVMon->stMap.iID != pstPlayer->stRoleData.Map) return 0;
			}

			if(!z_pos_in_view(&pstPlayer->stRoleData.Pos, &pstVMon->stCurrPos)) return 0;

			if (is_mon_machine(pstVMon))
			{
				return player_view_machine(pstEnv, pstPlayer, pstVMon, pstMapInst);
			}

			if (pstPlayer->stOnline.iVAnimate >= MAX_VISIBLE_ANIMATE)
			{
				return 0;
			}

			//简单的分圈外圈
			{
				if (z_pos_in_view_half1(&pstPlayer->stRoleData.Pos, &pstVMon->stCurrPos))  //小于等于16米
				{
					;
				}
				else if (z_pos_in_view_half2(&pstPlayer->stRoleData.Pos, &pstVMon->stCurrPos)) // 16米到20米
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

			if (0 > player_view_mon_must(pstEnv, pstPlayer, pstVMon))
			{
				iMust = 0;
			}
			else
			{
				iMust = 1;
			}

			if (0 == iMust)
			{
				if (pstVMon->cMonMiscFlag & MON_MISC_FLAG_IMPVIEW)
				{
					if (pstPlayer->stOnline.iImpMon >= iVisibleAnimateImpmon)
					{
						return 0;
					}

					piViewCount = &pstPlayer->stOnline.iImpMon;
				}
				else
				{
					int iVal;

					iVal = is_player_enemy_mon(pstEnv, pstPlayer, pstVMon);

					if (iVal)
					{
						if (pstPlayer->stOnline.iEmon >= iVisibleAnimateEmon)
						{
							return 0;
						}

						piViewCount = &pstPlayer->stOnline.iEmon;
					}
					else if (pstVMon->iVPlayer >= iVisibleAnimateNorm)
					{
						return 0;
					}
				}
			}
			
			if (pstVMon->iVPlayer < MAX_MONSTER_VPLAYER)
			{
				if (pstVMon->iVPlayer < MAX_MONSTER_VPLAYER_NORM || iMust)
				{
					binsert_int(&pstPlayer->iMemID, &pstVMon->aiVPlayer, &pstVMon->iVPlayer, sizeof(int), 1);
				}
				else
				{
					if (!(pstVMon->cMonMiscFlag & MON_MISC_FLAG_IMPVIEW))
					{
						return 0;
					}
				}
			}
							
			if(!binsert_int(&pstVMon->iID, &pstPlayer->stOnline.aiVAnimate, &pstPlayer->stOnline.iVAnimate, sizeof(int), 1)) return -2;

			if (piViewCount && pstPlayer->stOnline.iVAnimate > iVisibleAnimateNorm)
			{
				(*piViewCount)++;
			}
						
			if (0 == pstMapInst->bBroadcast && 0 == (pstVMon->cMonMiscFlag & MON_MISC_FLAG_MAPBROAD))
			{
				z_player_view_monster(pstAppCtx, pstEnv, pstPlayer, pstVMon);
			}

			mon_press_update_player_in(pstEnv, pstVMon, pstPlayer);

			break;

		case OBJ_NPC:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
			if (!pstMemBlock->fValid)
				return -1;
			
			pstVNpc = (Npc *)pstMemBlock->szData;
			pstVMon = &pstVNpc->stMon;

			if (0 == pstVMon->cOwnerIDType && pstVMon->iOwnerID > 0)
			{
				if (pstVMon->iOwnerID != pstPlayer->iMemID)
				{
					return -1;
				}
			}

			if (1 == pstVMon->cOwnerIDType && pstVMon->iOwnerID != pstPlayer->iMemID)
			{
				ZoneTeam *pstZoneTeam = player_team_get(pstEnv, pstPlayer);
				if (NULL == pstZoneTeam || pstZoneTeam->stTeamInfo.TeamID != pstVMon->ullOwnerApplyID)
				{
					return -1;
				}
			}
			
			if(pstVMon->stMap.iID != pstPlayer->stRoleData.Map) return 0;

			if(pstVMon->cMoving)
			{
				z_monster_move_curr(pstAppCtx, pstEnv, pstVMon, 0);
				if(pstVMon->stMap.iID != pstPlayer->stRoleData.Map) return 0;
			}

			if(!z_pos_in_view(&pstPlayer->stRoleData.Pos, &pstVMon->stCurrPos)) return 0;

			if (pstVMon->bMoveType == MOVE_TYPE_NO)
			{
				if (pstPlayer->stOnline.cDoSearch == 99 && 0 == pstPlayer->stOnline.cMoving) //相同位置搜过
				{
					return 0;
				}
				
				if (pstVMon->iVPlayer < MAX_MONSTER_VPLAYER)
				{
					binsert_int(&pstPlayer->iMemID, &pstVMon->aiVPlayer, &pstVMon->iVPlayer, sizeof(int), 1);
				}
				
				if (pstPlayer->stOnline.iVAnimate < VISIBLE_ANIMATE_NORM)
				{
					binsert_int(&pstVMon->iID, &pstPlayer->stOnline.aiVAnimate, &pstPlayer->stOnline.iVAnimate, sizeof(int), 1);
				}
				
				if (0 == pstMapInst->bBroadcast && 0 == (pstVMon->cMonMiscFlag & MON_MISC_FLAG_MAPBROAD))
				{
					z_player_view_monster(pstAppCtx, pstEnv, pstPlayer, pstVMon);
				}
				
				return 0;
			}
			
			if (pstPlayer->stOnline.iVAnimate >= MAX_VISIBLE_ANIMATE)
			{
				return 0;
			}

			if (0 > player_view_mon_must(pstEnv, pstPlayer, pstVMon))
			{
				iMust = 0;
			}
			else
			{
				iMust = 1;
			}

			if (0 == iMust)
			{
				if (pstPlayer->stOnline.iImpMon >= VISIBLE_ANIMATE_IMPMON)
				{
					return 0;
				}

				piViewCount = &pstPlayer->stOnline.iImpMon;
			}
			

			if (pstVMon->iVPlayer < MAX_MONSTER_VPLAYER)
			{
				binsert_int(&pstPlayer->iMemID, &pstVMon->aiVPlayer, &pstVMon->iVPlayer, sizeof(int), 1);
			}

			if (pstPlayer->stOnline.iVAnimate < MAX_VISIBLE_ANIMATE)
			{
				if(!binsert_int(&pstVMon->iID, &pstPlayer->stOnline.aiVAnimate, &pstPlayer->stOnline.iVAnimate, sizeof(int), 1))
				{
					return -2;
				}
				else
				{
					if (piViewCount && pstPlayer->stOnline.iVAnimate > VISIBLE_ANIMATE_NORM)
					{
						(*piViewCount)++;
					}
				}
			}
			
			if (0 == pstMapInst->bBroadcast && 0 == (pstVMon->cMonMiscFlag & MON_MISC_FLAG_MAPBROAD))
			{
				z_player_view_monster(pstAppCtx, pstEnv, pstPlayer, pstVMon);

				
				//if (	pstVMon->bAtkType == ATK_TYPE_INITIATIVE && 
					//pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal >= PLAYER_PK_FRENZY)
				{
					mon_press_update_player_in(pstEnv, pstVMon, pstPlayer);
				}
			}

			break;

		case OBJ_ITEM:
			if (pstPlayer->stOnline.iVInanimate >= MAX_VISIBLE_INANIMATE) return 0;

			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstItemPool, iIndex);
			if (!pstMemBlock->fValid)
				return -1;
			
			pstDItem = (DropItem *)pstMemBlock->szData;

			if (pstPlayer->stOnline.iVInanimate >= MAX_VISIBLE_INANIMATE_NORM &&
				0 > player_view_item_imp(pstEnv, pstPlayer, pstDItem))
			{
				return 0;
			}
			
			if(0 == player_view_booty_valid(pstEnv, pstDItem, pstPlayer))
			{
				return 0;
			}
			
			if (pstDItem->stMap.iID != pstPlayer->stRoleData.Map) return 0;
			if (!z_pos_in_view(&pstPlayer->stRoleData.Pos, &pstDItem->stPos)) return 0;
			if (!binsert_int(&pstDItem->iID, &pstPlayer->stOnline.aiVInanimate, &pstPlayer->stOnline.iVInanimate, sizeof(int), 1)) return 0;
			z_player_view_item(pstAppCtx, pstEnv, pstPlayer, pstDItem);
			
			break;
		default:
			return 0;
	}

	return 0;
}


int z_player_search_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, RESPOS *pstPos,
							Player *pstPlayer)
{
	int i, j, iIndex;
	SearchArea stSArea;
	AREADYN *pstArea;
	char cType;
	int iTotal;
	int iID;
	int iEqu;

	iTotal	=	0;

	pstPlayer->stOnline.cDoSearch = 0;
	if (0 > z_pos_search_area(pstAppCtx, pstEnv, pstMapInst, pstPos, &stSArea))
		return -1;

	if (pstPlayer->stOnline.stLastSearchPos.X == pstPos->X && 
		pstPlayer->stOnline.stLastSearchPos.Y == pstPos->Y)
	{
		pstPlayer->stOnline.cDoSearch = 99;
	}
	
	memcpy(&pstPlayer->stOnline.stLastSearchPos, pstPos, sizeof(pstPlayer->stOnline.stLastSearchPos));
	memcpy(&pstPlayer->stOnline.stLastSearchTv, &pstAppCtx->stCurr, sizeof(pstPlayer->stOnline.stLastSearchTv));

	if (!is_player_ride_on_machine(pstPlayer))
	{
		i = 0;
		while (i < pstPlayer->stOnline.iVAnimate) {
			iID = pstPlayer->stOnline.aiVAnimate[i];
			iIndex = z_id_indextype(pstAppCtx, pstEnv, iID, &cType);
			if (iIndex < 0) {
				if (z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst, pstPlayer, 1, iID, OBJ_UNKNOW, 1) != 0) i++;
			}
			else if (z_obj_leave_view_check(pstAppCtx, pstEnv, pstMapInst, pstPlayer, cType, iIndex, iID) == 0) i++;
		}
	}

	for (i = 0; i < pstPlayer->stOnline.iVInanimate; ) {
		iID = pstPlayer->stOnline.aiVInanimate[i];
		iIndex = z_id_indextype(pstAppCtx, pstEnv, iID, &cType);
		if (iIndex < 0) {
			if (z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst, pstPlayer, 0, iID, OBJ_UNKNOW, 1) != 0) i++;
		}
		else if (z_obj_leave_view_check(pstAppCtx, pstEnv, pstMapInst, pstPlayer, cType, iIndex, iID) == 0) i++;
	}

	if (!is_player_ride_on_machine(pstPlayer))
	{
		if (pstPlayer->stOnline.iVAnimate <= VISIBLE_ANIMATE_NORM * 6/7)
		{
			pstPlayer->stOnline.iEmon = pstPlayer->stOnline.iEplayer = 0;
			pstPlayer->stOnline.iFplayer = pstPlayer->stOnline.iImpMon = 0;
		}
		else if (pstPlayer->stOnline.iVAnimate < MAX_VISIBLE_ANIMATE)
		{
			player_count_view(pstEnv, pstPlayer);
		}
	}
	
	for (i = 0; i < stSArea.iNum; i++) {
		pstArea = stSArea.pastArea[i];

		if (!is_player_ride_on_machine(pstPlayer))
		{
			for (j = 0; j < pstArea->iAnimate; j++) {
				if (pstArea->aiAnimate[j] == pstPlayer->iMemID) continue;
				bsearch_int(&pstArea->aiAnimate[j], pstPlayer->stOnline.aiVAnimate, pstPlayer->stOnline.iVAnimate, sizeof(int), &iEqu);
				if (iEqu)
				{
					continue;
				}
						
				iIndex = z_id_indextype(pstAppCtx, pstEnv, pstArea->aiAnimate[j], &cType);
				if (iIndex < 0) continue;

				z_obj_enter_view(pstAppCtx, pstEnv, pstMapInst ,pstPlayer, cType, iIndex);
			}
		}
		
		for (j = 0; j < pstArea->iInanimate; j++) {
			bsearch_int(&pstArea->aiInanimate[j], &pstPlayer->stOnline.aiVInanimate, pstPlayer->stOnline.iVInanimate, sizeof(int), &iEqu);
			if (iEqu)
			{
				continue;
			}
			iIndex = z_id_indextype(pstAppCtx, pstEnv, pstArea->aiInanimate[j], &cType);
			if (iIndex < 0) continue;
			z_obj_enter_view(pstAppCtx, pstEnv, pstMapInst, pstPlayer, cType, iIndex);
		}
	}

	pstPlayer->stOnline.cDoSearch = 0;
	return 0;
}

int z_item_search_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, RESPOS *pstPos, DropItem *pstDropItem)
{
	int i, j, iIndex;
	char cType;
	SearchArea stSArea;
	AREADYN *pstArea;
	Player *pstPlayer;
	TMEMBLOCK *pstMemBlock;

	if (z_pos_search_area(pstAppCtx, pstEnv, pstMapInst, pstPos, &stSArea) < 0) return -1;

	for (i = 0; i < stSArea.iNum; i++) 
	{
		pstArea = stSArea.pastArea[i];
		for (j = 0; j < pstArea->iAnimate; j++) 
		{
			iIndex = z_id_indextype(pstAppCtx, pstEnv, pstArea->aiAnimate[j], &cType);
			if (iIndex < 0) continue;

			switch (cType)
			{
				case OBJ_PLAYER:
					pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
					if (!pstMemBlock->fValid) continue;
					pstPlayer = (Player *)pstMemBlock->szData;
					
					z_obj_enter_view(pstAppCtx, pstEnv, pstMapInst, pstPlayer, OBJ_ITEM, pstDropItem->iMIndex);
					break;
				default:
					break;
			}
		}
	}
	
	return 0;
}
int z_player_store_remain_patrol(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i = 0;
	unsigned char cCurrPoint,cFuturePoints;
	REMAINPATROL  *pstRemainPatrol;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;
	UNUSED(pstEnv);
	
	if(pstRoleTmpSys->TmpItemID == 0 || 
		pstRoleTmpSys->PatrolID == 0 ||
		pstPlayer->stOnline.cFuturePoints ==0 || 
		pstPlayer->stOnline.cPatrolFlag != 1)
	{		
		return -1;
	}
	pstPlayer->stOnline.cPatrolFlag  = 2;
	
	// 还有剩余的巡逻路没走完
	memset(&pstPlayer->stOnline.stRemainPatrol,0,sizeof(pstPlayer->stOnline.stRemainPatrol));
	pstRemainPatrol = &pstPlayer->stOnline.stRemainPatrol;	

	//把第一个点先空留下来,
	pstRemainPatrol->cFuturePoints++;
	
	cCurrPoint = pstPlayer->stOnline.cCurrPoint;
	cFuturePoints = pstPlayer->stOnline.cFuturePoints;
	for( i=0; i<pstPlayer->stOnline.cFuturePoints && i<RECORD_TURN; i++)
	{
		memcpy(&pstRemainPatrol->stTurnPoints[pstRemainPatrol->cFuturePoints], 
				&pstPlayer->stOnline.stTurnPoints[cCurrPoint], 
				sizeof(RESPOS));
		pstRemainPatrol->cFuturePoints++;
		
		if (++cCurrPoint >= RECORD_TURN) cCurrPoint = 0;
		if (--cFuturePoints == 0) break;
	}
	
	return 0;
			
}

int z_player_move_stop(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{	
	if( pstPlayer->stOnline.cMoving )
		z_player_move_curr(pstAppCtx, pstEnv, pstPlayer, 1);

	z_player_store_remain_patrol(pstEnv, pstPlayer);
	
	pstPlayer->stOnline.cLastPathPoints = 0;
	pstPlayer->stOnline.cWalkPath = pstPlayer->stOnline.cLastPath = pstPlayer->stOnline.cCurrPoint;

	pstPlayer->stOnline.cFuturePoints = 0;
	SET_PLAYER_NOMOVE(pstPlayer);
	return 0;
}

int z_sendpkg_playerview(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iSelf, CSPKG *pstPkg, int iImportant)
{
	Player *pstVPlayer;
	Monster *pstVMon;
	int i, iIndex;
	char cType;
	TMEMBLOCK *pstMemBlock;
	TDRDATA stNet;
	int iRet;
	int iCount = 0;
	Player *apstPlayers[MAX_PLAYER];

	if (iSelf) z_cltmsg_send(pstEnv, pstPlayer, pstPkg);

	if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE)
	{
		ZONEIDX stIdx;
		ZONEMAPINST *pstMapInst;

		stIdx.iID = pstPlayer->stRoleData.Map;
		stIdx.iPos = pstPlayer->stOnline.iMapPos;
		pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
		if (pstMapInst && pstMapInst->bBroadcast)
		{
			ZONEPWORLDINST *pstPworldInst;
			pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
			if (pstPworldInst)
			{
				return pworld_map_broadcast_except(pstAppCtx, pstEnv, pstPworldInst, pstMapInst, iImportant, 
												pstPkg, pstPlayer);
			}
		}
	}

	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = &pstEnv->szSendBuffer[0];
	iRet = net_cs_pack(pstEnv->pstNetHan, &stNet, pstPkg, 0);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_sendpkg_playerview cmd=%d cs pack fail ret=%d",
					pstPkg->Head.Cmd, iRet);
		return -1;
	}

	for (i = 0; i < pstPlayer->stOnline.iVAnimate; i++) {
		iIndex = z_id_indextype(pstAppCtx, pstEnv, pstPlayer->stOnline.aiVAnimate[i], &cType);
		if (iIndex < 0) continue;

		switch (cType)
		{
			case OBJ_PLAYER:
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					continue;
				}
				pstVPlayer = (Player *)pstMemBlock->szData;
				
				if (PLAYER_STATUS_ROLE_LOGIN != pstVPlayer->eStatus || pstVPlayer->stOnline.bConnLost )
				{
					continue;
				}

				if ((pstPlayer->stOnline.State & CS_STAT_QIANXIN) && (0 == player_team_same(pstEnv, pstPlayer, pstVPlayer)))
				{
					continue;
				}
				if((pstPlayer->stOnline.State & CS_STAT_QIANXIN) && 
					pstPlayer->stOnline.stWarOL.cCampIdx>0 &&
					!is_same_camp_player(pstPlayer, pstVPlayer))
				{
					continue;
				}

				if (!iImportant && pstPlayer->stOnline.iVAnimate > VISIBLE_ANIMATE_VALVE
					&& !z_pos_in_view_half(&pstPlayer->stRoleData.Pos, &pstVPlayer->stRoleData.Pos)) continue;
				

				apstPlayers[iCount++] = pstVPlayer;
				break;
			case OBJ_MONSTER:
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					continue;
				}
				pstVMon = (Monster *)pstMemBlock->szData;

				if (is_mon_machine(pstVMon))
				{
					machine_msg_broadcast_except(pstEnv, pstVMon, pstPkg, pstPlayer->iMemID);
				}
				break;
		}
	}

	if (iCount > 0)
	{
		return z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
		                                iCount, pstPkg);
	}
	return 0;
}

int z_player_move_notify(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,char cMoveType)
{
	int i;
	int iCurrPoint;
	CSWALK *pstCSWalk;
	CSTURN *pstCSTurn;
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSMOVEOTHER *pstMoveOther = &pstPkg->Body.MoveOther;
	
	pstMoveOther->ID = pstPlayer->iMemID;
	pstMoveOther->x = pstPlayer->stRoleData.Pos.X;
	pstMoveOther->y = pstPlayer->stRoleData.Pos.Y;
	
	pstMoveOther->MoveType = cMoveType;
	if (CS_MOVE_TURN == cMoveType)
	{
		pstCSTurn = &pstMoveOther->Data.Turn;

		pstCSTurn->Dir = pstPlayer->stOnline.bDir;
	}
	else
	{
		pstCSWalk = &pstMoveOther->Data.Walk;

		pstCSWalk->Total = pstPlayer->stOnline.cFuturePoints;
		if (pstCSWalk->Total > CS_MAX_MOVEPOINTS)
		{
			pstCSWalk->Total = CS_MAX_MOVEPOINTS;
		}
	
		for (i = 0; i < pstCSWalk->Total; i++) {
			iCurrPoint = (pstPlayer->stOnline.cCurrPoint + i) % RECORD_TURN;
			pstCSWalk->Points[i].x = pstPlayer->stOnline.stTurnPoints[iCurrPoint].X;
			pstCSWalk->Points[i].y = pstPlayer->stOnline.stTurnPoints[iCurrPoint].Y;
		}
	}

	memcpy(&pstMoveOther->MspdInfo, &pstPlayer->stOnline.stMspdInfo, sizeof(pstMoveOther->MspdInfo));

	Z_CSHEAD_INIT(&pstPkg->Head, MOVE_OTHER);

	if (is_player_driver_machine(pstEnv, pstPlayer))
	{
		Monster *pstMon;

		pstMon = z_id_monster(pstAppCtx, pstEnv, pstPlayer->stOnline.stMachineInfo.iMachineMonID);
		if(pstMon == NULL)
		{
			return 0;
		}
		
		pstMoveOther->ID = pstPlayer->stOnline.stMachineInfo.iMachineMonID;
		
		Z_CSHEAD_INIT(&pstPkg->Head, MOVE_OTHER);
		z_sendpkg_monview(pstAppCtx, pstEnv, pstMon, pstPkg, 0);
	}
	else
	{
		z_sendpkg_playerview(pstAppCtx, pstEnv, pstPlayer, 0, pstPkg, 0);
	}
		
	return 0;
}

int z_player_stop(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iBench, struct timeval *pstCltTime)
{	
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSMOVESTOP *pstMoveStop = &pstPkg->Body.MoveStop;
	UNUSED(pstCltTime);
	
	z_player_move_stop(pstAppCtx, pstEnv, pstPlayer);

	pstPlayer->stOnline.iPosSeq += 1000;
	pstMoveStop->MoveSeq = pstPlayer->stOnline.iPosSeq;
	pstMoveStop->x = pstPlayer->stRoleData.Pos.X;
	pstMoveStop->y = pstPlayer->stRoleData.Pos.Y;
	pstMoveStop->Dir = pstPlayer->stOnline.bDir;
	pstMoveStop->BenchTimeMs = 0;
	
	pstPlayer->stOnline.stLastMoveTv = pstAppCtx->stCurr;

	if (iBench)
	{
		pstMoveStop->BenchTimeMs = z_bench_time(pstPlayer);
	}

	tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "stop svr::x,y %d,%d ", pstMoveStop->x, pstMoveStop->y);

	Z_CSHEAD_INIT(&pstPkg->Head, MOVE_STOP);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkg);
	
	z_player_move_notify(pstAppCtx, pstEnv, pstPlayer, CS_MOVE_TURN);
	
	return 0;
}

int z_trust_clt_pos(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						int x, int y, struct timeval *pstCltTime, int iNoChgPos)
{
	char cPassC;
	int cNewPath;
	int iDist;
	int iDiff;
	int iWalkDist;
	RESPOS stPos;
	RESPOS stTmpPos;
	ZONEMAPINST* pstMapInst;
	MapIndex* pstMapIdx;
	ZONEIDX stIdx;
		
	if( pstPlayer->stOnline.cMoving && z_compare_time(&pstPlayer->stOnline.stLastMoveTv, pstCltTime)>0)
		z_player_move_curr_bytime(pstAppCtx, pstEnv, pstPlayer, 0, pstCltTime, 0);

	if (pstPlayer->stRoleData.Pos.X == x && pstPlayer->stRoleData.Pos.Y == y)
	{
		if (0 == iNoChgPos)
		{
			goto _end;
		}
		
		return 0;
	}

	stPos.X = x;
	stPos.Y = y;

	if (pstPlayer->stOnline.cLastPathPoints) 
	{
		cPassC = z_point_on_recordpath(x, y, pstPlayer->stOnline.cLastPath, pstPlayer->stOnline.cLastPathPoints, pstPlayer->stOnline.stTurnPoints, pstPlayer->stOnline.cFuturePoints);
		if (!cPassC)
		{
			return -1;
		}
		else
		{
			cNewPath = (pstPlayer->stOnline.cLastPath + cPassC) % RECORD_TURN;

			if (pstPlayer->stOnline.cWalkPath != pstPlayer->stOnline.cLastPath)
			{
				iWalkDist = 0;
			}
			else
			{
				stTmpPos = pstPlayer->stOnline.stTurnPoints[pstPlayer->stOnline.cCurrPoint];
				pstPlayer->stOnline.stTurnPoints[pstPlayer->stOnline.cCurrPoint] = pstPlayer->stRoleData.Pos;
					
				iWalkDist = z_turn_point_dist(pstPlayer->stOnline.stTurnPoints, pstPlayer->stOnline.cLastPath, 
											pstPlayer->stOnline.cCurrPoint);
				
				pstPlayer->stOnline.stTurnPoints[pstPlayer->stOnline.cCurrPoint] = stTmpPos;
			}

			if (iNoChgPos)
			{
				stTmpPos = pstPlayer->stOnline.stTurnPoints[cNewPath];
			}
			pstPlayer->stOnline.stTurnPoints[cNewPath].X = x;
			pstPlayer->stOnline.stTurnPoints[cNewPath].Y = y;
			iDist = z_turn_point_dist(pstPlayer->stOnline.stTurnPoints, pstPlayer->stOnline.cLastPath, cNewPath);
			if (iNoChgPos)
			{
				pstPlayer->stOnline.stTurnPoints[cNewPath] = stTmpPos;
			}
						
			iDiff	= iWalkDist - iDist;

			if (iNoChgPos)
			{
				if (iDiff < -300)
				{
					return -1;
				}
			}
			else
			{
				if (iDiff < -100)
				{
					if (pstPlayer->stOnline.bTrustPosCount < 255)
					{
						pstPlayer->stOnline.bTrustPosCount++;
					}
					return -1;
				}
				else if (iDiff < -25)
				{
					if (pstPlayer->stOnline.bTrustPosCount > 0)
					{
						pstPlayer->stOnline.bTrustPosCount--;
					}
				}
			}
		}
	} 
	else 
	{
		iDist = z_distance(&pstPlayer->stRoleData.Pos, &stPos);
		if (iNoChgPos)
		{
			if (iDist > 300)
			{
				if (pstPlayer->stOnline.bTrustPosCount < 254)
				{
					pstPlayer->stOnline.bTrustPosCount += 2;
				}
				return -1;
			}
			else if (iDist > 20)
			{
				if (pstPlayer->stOnline.bTrustPosCount < 255)
				{
					pstPlayer->stOnline.bTrustPosCount++;
				}
			}
			else
			{
				if (pstPlayer->stOnline.bTrustPosCount > 0)
				{
					pstPlayer->stOnline.bTrustPosCount--;
				}
			}
		}
		else
		{
			if (iDist > 100)
			{
				if (pstPlayer->stOnline.bTrustPosCount < 254)
				{
					pstPlayer->stOnline.bTrustPosCount += 2;
				}
				return -1;
			}
			else if (iDist > 20)
			{
				if (pstPlayer->stOnline.bTrustPosCount < 255)
				{
					pstPlayer->stOnline.bTrustPosCount++;
				}
			}
			else
			{
				if (pstPlayer->stOnline.bTrustPosCount > 0)
				{
					pstPlayer->stOnline.bTrustPosCount--;
				}
			}
		}
	}

	if (pstPlayer->stOnline.bTrustPosCount > MAX_TRUSTPOS_COUNT)
	{
		//惩罚2秒
		//pstPlayer->stOnline.stLastCltTime.tv_sec += 2;
		//return -1;
	}

	if (iNoChgPos)
	{
		return 0;
	}
	
	z_player_store_remain_patrol(pstEnv, pstPlayer);

	memcpy(&stPos, &pstPlayer->stRoleData.Pos, sizeof(stPos));

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst  =   z_find_map_inst(pstAppCtx, pstEnv,  &stIdx);
	if(!pstMapInst)
		return -1;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if(!pstMapIdx)
		return -1;

	pstPlayer->stRoleData.Pos.X	=	x;
	pstPlayer->stRoleData.Pos.Y	=	y;

	z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstPlayer->iMemID, &stPos, 0);

_end:
	pstPlayer->stOnline.cLastPathPoints = 0;
	pstPlayer->stOnline.cWalkPath = pstPlayer->stOnline.cLastPath = pstPlayer->stOnline.cCurrPoint;
	pstPlayer->stOnline.cFuturePoints = 0;
	SET_PLAYER_NOMOVE(pstPlayer);

	z_mon_machine_move(pstEnv, pstPlayer);

	return 0;
	
}

int z_player_check_stop(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSMOVEC *pstMove, struct timeval *pstCltTime)
{
	//int iStopTimeMin;
	//int iStopTimeMax;
	//ZONE_SVRCONF *pstConf = pstEnv->pstConf;

	//iStopTimeMin	=	pstConf->StopTimeMin;
	//iStopTimeMax	=	pstConf->StopTimeMax;

	/*if (pstPlayer->stOnline.stLastCltStopTime.tv_sec != 0 && z_compare_time(&pstPlayer->stOnline.stLastCltStopTime, pstCltTime) < iStopTimeMax )
	{
		return z_player_stop(pstAppCtx, pstEnv, pstPlayer, 1, pstCltTime);
	}*/

	if (pstPlayer->stOnline.stLastCltStopTime.tv_sec != 0 && z_compare_time(&pstPlayer->stOnline.stLastCltStopTime, pstCltTime) < 20 )
	{
		return z_player_stop(pstAppCtx, pstEnv, pstPlayer, 1, pstCltTime);
	}

	pstPlayer->stOnline.stLastCltStopTime.tv_sec = pstCltTime->tv_sec;
	pstPlayer->stOnline.stLastCltStopTime.tv_usec = pstCltTime->tv_usec;

	
	if (0 > z_trust_clt_pos(pstAppCtx, pstEnv, pstPlayer, pstMove->x, pstMove->y, pstCltTime, 0))
	{
		return z_player_stop(pstAppCtx, pstEnv, pstPlayer, 1, pstCltTime);
	}

	pstPlayer->stOnline.bDir = pstMove->Data.Turn.Dir;

	z_player_move_notify(pstAppCtx, pstEnv, pstPlayer, CS_MOVE_TURN);

	return 0;
}

int z_add_path(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSMOVEC *pstMove, char cPassC)
{
	int cAdd, cNewPath;
	int i, iDist, iMoveDist, iWalkDist;
	RESPOS stPos, stSearchPos, stTmpPos;
	ZONEMAPINST* pstMapInst;
	MapIndex* pstMapIdx;
	ZONEIDX stIdx;
	int iOldFuturePoints = pstPlayer->stOnline.cFuturePoints;
	int j;

	if (pstPlayer->stOnline.cLastPathPoints) 
	{
		if (pstPlayer->stOnline.cWalkPath != pstPlayer->stOnline.cLastPath)
		{
			iWalkDist = 0;
		}
		else
		{
			stTmpPos = pstPlayer->stOnline.stTurnPoints[pstPlayer->stOnline.cCurrPoint];
			pstPlayer->stOnline.stTurnPoints[pstPlayer->stOnline.cCurrPoint] = pstPlayer->stRoleData.Pos;
				
			iWalkDist = z_turn_point_dist(pstPlayer->stOnline.stTurnPoints, pstPlayer->stOnline.cLastPath, 
										pstPlayer->stOnline.cCurrPoint);
			
			pstPlayer->stOnline.stTurnPoints[pstPlayer->stOnline.cCurrPoint] = stTmpPos;
		}
	}
	else
	{
		iWalkDist =  0;
	}
		
	memcpy(&stPos, &pstPlayer->stRoleData.Pos, sizeof(stPos));

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst  =   z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if(!pstMapInst)
		return -1;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if(!pstMapIdx)
		return -1;

	cNewPath = cAdd = (pstPlayer->stOnline.cLastPath + cPassC) % RECORD_TURN;
	pstPlayer->stOnline.stTurnPoints[cAdd].X = pstMove->x;
	pstPlayer->stOnline.stTurnPoints[cAdd].Y = pstMove->y;
	cAdd = (cAdd + 1)  % RECORD_TURN;
	
	iDist = z_turn_point_dist(pstPlayer->stOnline.stTurnPoints, pstPlayer->stOnline.cLastPath, cNewPath);
	
	iMoveDist = 0;
	
	if(pstPlayer->stOnline.cWalkPath == pstPlayer->stOnline.cLastPath && iWalkDist >= iDist) 
	{
		pstPlayer->stOnline.cWalkPath = cNewPath;
		iMoveDist = iWalkDist - iDist;
		pstPlayer->stOnline.cFuturePoints = 0;
		memcpy(&pstPlayer->stRoleData.Pos, &pstPlayer->stOnline.stTurnPoints[cNewPath], sizeof(RESPOS));
		pstPlayer->stOnline.cCurrPoint = cAdd;
	}
	else
	{
		pstPlayer->stOnline.cFuturePoints = pstPlayer->stOnline.cFuturePoints - pstPlayer->stOnline.cLastPathPoints + cPassC;
	}

	for(i = 0; i < pstMove->Data.Walk.Total; i++)
	{
		pstPlayer->stOnline.stTurnPoints[cAdd].X = pstMove->Data.Walk.Points[i].x;
		pstPlayer->stOnline.stTurnPoints[cAdd].Y = pstMove->Data.Walk.Points[i].y;
		cAdd = (cAdd + 1)  % RECORD_TURN;
		if (pstPlayer->stOnline.cFuturePoints > MAX_FUTURE_POINT)
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
				"stop %d player FuturePoints %d oldfuturepoints %d LastPathPoints %d PassC %d WalkPath %d LastPath %d NewPath %d iDist %d "
				"CurrPoint  %d moving %d movex %d movey %d currx %d curry %d",
				pstPlayer->stRoleData.Uin, pstPlayer->stOnline.cFuturePoints, iOldFuturePoints, pstPlayer->stOnline.cLastPathPoints, cPassC,
				pstPlayer->stOnline.cWalkPath, pstPlayer->stOnline.cLastPath, cNewPath, iDist, 
				pstPlayer->stOnline.cCurrPoint, pstPlayer->stOnline.cMoving, pstMove->x, pstMove->y, 
				pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y);

			for (j=0; j<=pstPlayer->stOnline.cLastPathPoints; j++)
			{
				tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
					"(%d, %d)", 
					pstPlayer->stOnline.stTurnPoints[(pstPlayer->stOnline.cLastPath+j)%RECORD_TURN].X,
					pstPlayer->stOnline.stTurnPoints[(pstPlayer->stOnline.cLastPath+j)%RECORD_TURN].Y);
			}
			//外面stop
			return -1;
		}
		else
		{
			pstPlayer->stOnline.cFuturePoints++;
		}
	}

	pstPlayer->stOnline.cLastPath = cNewPath;
	pstPlayer->stOnline.cLastPathPoints = pstMove->Data.Walk.Total;
	if(pstPlayer->stOnline.cFuturePoints)
	{
		if(!pstPlayer->stOnline.cMoving)
		{
			SET_PLAYER_MOVE(pstPlayer);
		}

		if (iMoveDist > 0)
		{
			z_move_on_path(pstPlayer, iMoveDist);
		}
	}

	z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstPlayer->iMemID, &stPos, 0);

	if(pstMapIdx && z_pos_search_pos(pstMapIdx, &pstPlayer->stRoleData.Pos, &stSearchPos))
	{
		if(!z_same_pos(&stSearchPos, &pstPlayer->stOnline.stLastSearchPos))
			z_player_search_view(pstAppCtx, pstEnv,  pstMapInst, &stSearchPos, pstPlayer);
	}

	return 0;
}

int z_mon_search_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, RESPOS *pstPos, Monster* pstMon)
{
	int i, j, iIndex;
	SearchArea stSArea;
	AREADYN *pstArea;
	char cType;
	Player *pstVPlayer;
	Monster *pstVMon;
	int iTotal;
	TMEMBLOCK *pstMemBlock;
	int iEqu;

	if (is_mon_machine(pstMon))
	{
		return machine_search_view(pstAppCtx, pstEnv, pstMon, pstMapInst, pstPos);
	}
	
	iTotal	=	0;

	pstMon->cDoSearch = 0;
	if (z_pos_search_area(pstAppCtx, pstEnv,  pstMapInst, pstPos, &stSArea) < 0) 
		return -1;
		
	memcpy(&pstMon->stLastSearchPos, pstPos, sizeof(RESPOS));
	TV_CLONE(pstMon->stLastSearchTv, pstAppCtx->stCurr);

	for (i = 0; i < pstMon->iVPlayer; ) {
		iIndex = z_id_indextype(pstAppCtx, pstEnv, pstMon->aiVPlayer[i], &cType);
		if (iIndex < 0)
		{
			mon_press_delete(pstMon, pstMon->aiVPlayer[i]);

			if (bdelete_int(&pstMon->aiVPlayer[i], &pstMon->aiVPlayer, &pstMon->iVPlayer, sizeof(int)) == 0) i++;

			continue;
		}

		if (cType == OBJ_PLAYER)
		{
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				mon_press_delete(pstMon, pstMon->aiVPlayer[i]);
				
				if (bdelete_int(&pstMon->aiVPlayer[i], &pstMon->aiVPlayer, &pstMon->iVPlayer, sizeof(int)) == 0) i++;
				continue;
			}

			pstVPlayer = (Player *)pstMemBlock->szData;

			if (!z_pos_in_view(&pstMon->stCurrPos, &pstVPlayer->stRoleData.Pos) ) {
				if (bdelete_int(&pstVPlayer->iMemID, &pstMon->aiVPlayer, &pstMon->iVPlayer, sizeof(int)) != 0)
				{
					z_obj_leave_view(pstAppCtx, pstEnv,  pstMapInst, pstVPlayer, 1, pstMon->iID, pstMon->cType, 0);
					continue;
				}
			}
		}
		else
		{
			if (cType == OBJ_MONSTER)
			{
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
				if (pstMemBlock->fValid)
				{
					pstVMon = (Monster *)pstMemBlock->szData;
					if(is_mon_machine(pstVMon))
					{
						 if (!z_pos_in_view(&pstMon->stCurrPos, &pstVMon->stCurrPos))
						 {
						 	if (bdelete_int(&pstVMon->iID, &pstMon->aiVPlayer, &pstMon->iVPlayer, sizeof(int)) != 0)
							{
								obj_leave_machine_view(pstEnv, pstVMon, pstMapInst, pstMon->iID, pstMon->cType);
								continue;
							}
						 }
						 else
						 {
						 	i++;
							continue;
						 }
					}
				}
			}
			
			if (bdelete_int(&pstMon->aiVPlayer[i], &pstMon->aiVPlayer, &pstMon->iVPlayer, sizeof(int)) == 0) i++;
			continue;
		}

		i++;
	}

	for (i = 0; i < stSArea.iNum; i++) 
	{
		pstArea = stSArea.pastArea[i];
		for (j = 0; j < pstArea->iAnimate; j++)
             {
			iIndex = z_id_indextype(pstAppCtx, pstEnv,  pstArea->aiAnimate[j], &cType);

			if (iIndex < 0) continue;

			if (cType == OBJ_PLAYER)
			{
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					continue;
				}
				
				pstVPlayer = (Player *)pstMemBlock->szData;
				
				bsearch_int(&pstArea->aiAnimate[j], &pstMon->aiVPlayer, pstMon->iVPlayer, sizeof(int), &iEqu);
				if (!iEqu)
				{
					z_obj_enter_view(pstAppCtx, pstEnv,  pstMapInst, pstVPlayer, pstMon->cType, pstMon->iMIndex);
				}
				else
				{
					mon_press_update_player_in(pstEnv, pstMon, pstVPlayer);
				}
			}
			else if (cType == OBJ_MONSTER)
			{	
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
                if (!pstMemBlock->fValid)
                {
                        continue;
                }

                pstVMon = (Monster *)pstMemBlock->szData;
				
				if (is_mon_machine(pstVMon))
				{
					if (npc_view_machine_check(pstEnv,pstMon,pstVMon) < 0)
					{
						continue;
					}
					
					mon_norm_view_machine(pstEnv, pstMon, pstVMon, pstMapInst);
				}

				mon_press_update_mon_in(pstEnv, pstMon, pstVMon);
			}
		}
       }

	return 0;
}


// iImportant == 99 狂沙提示信息广播
int pworld_map_broadcast_except(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, 
										ZONEMAPINST *pstMapInst, int iImportant, CSPKG *pstPkg, Player *pstPlayerExcept)
{	
	int i, iIndex;
	int iRet;
	char cType;
	TMEMBLOCK *pstMemBlock;
	Player *pstPlayer;
	TDRDATA stNet;
	int iCount = 0;
	Player *apstPlayers[MAX_PLAYER];
	UNUSED(&iImportant);
	
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = &pstEnv->szSendBuffer[0];
	iRet = net_cs_pack(pstEnv->pstNetHan, &stNet, pstPkg, 0);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "pworld_broadcast cmd=%d cs pack fail ret=%d",
					pstPkg->Head.Cmd, iRet);
		return -1;
	}

	for (i = 0; i < pstPworldInst->iPlayer; i++) {
		if (i >= (int)(sizeof(pstPworldInst->aiPlayerID)/sizeof(pstPworldInst->aiPlayerID[0])))
		{
			break;
		}
			
		iIndex = z_id_indextype(pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i], &cType);
		if (iIndex < 0 || cType != OBJ_PLAYER) continue;
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
		if (!pstMemBlock->fValid)
		{
			continue;
		}
		pstPlayer = (Player *)pstMemBlock->szData;
		
		if (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus  || pstPlayer->stRoleData.Map != pstMapInst->iMapInstID)
		{
			continue;
		}

		if (pstPlayerExcept == pstPlayer)
		{
			if (iImportant != 99 )
			{
				continue;
			}
		}
	
		apstPlayers[iCount++] = pstPlayer;
	}

	if (iCount > 0)
	{
		return z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
		                                iCount, pstPkg);
	}
	return 0;
}

int pworld_map_broadcast(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, 
								ZONEMAPINST *pstMapInst, int iImportant, CSPKG *pstPkg)
{	
	return pworld_map_broadcast_except(pstAppCtx, pstEnv, pstPworldInst, pstMapInst, iImportant, pstPkg, NULL);
}

int z_sendpkg_monview_except(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, 
		CSPKG *pstPkg, int iImportant, Player *pstPlayer)
{
	Player *pstVPlayer;
	Monster *pstVMon;
	int i, iIndex;
	char cType;
	TMEMBLOCK *pstMemBlock;
	TDRDATA stNet;
	int iRet;
	ZONEMAPINST *pstMapInst;
	int iCount = 0;
	Player *apstPlayers[MAX_PLAYER];
	MonAtkInfo *pststMonAtkInfo = &pstMon->stMonAtkInfo;
	SKILLDEF* pstSkillDef = NULL;

	if (pstMon->cMonMiscFlag & (MON_MISC_FLAG_IMPVIEW|MON_MISC_FLAG_MAPBROAD) || is_mon_machine(pstMon))
	{
		return z_sendpkg_mon_area_view_except(pstAppCtx, pstEnv, pstMon, pstPkg, pstPlayer);
	}

	if (pstMon->stMap.iID >= PWORLD_MAPID_BASE)
	{
		pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
		if (pstMapInst && pstMapInst->bBroadcast)
		{
			ZONEPWORLDINST *pstPworldInst;
			pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
			if (pstPworldInst)
			{
				return pworld_map_broadcast_except(pstAppCtx, pstEnv, pstPworldInst, 
								pstMapInst, iImportant, pstPkg, pstPlayer);
			}
		}
	}

	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = &pstEnv->szSendBuffer[0];
	iRet = net_cs_pack(pstEnv->pstNetHan, &stNet, pstPkg, 0);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_sendpkg_monview cmd=%d cs pack fail ret=%d",
					pstPkg->Head.Cmd, iRet);
		return -1;
	}

	for (i = 0; i < pstMon->iVPlayer; i++) {
		iIndex = z_id_indextype(pstAppCtx, pstEnv, pstMon->aiVPlayer[i], &cType);
		if (iIndex < 0) continue;

		switch(cType)
		{
			case OBJ_PLAYER:
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					continue;
				}
				pstVPlayer = (Player *)pstMemBlock->szData;
				if(pstVPlayer == pstPlayer)
				{
					continue;
				}
				
				if (PLAYER_STATUS_ROLE_LOGIN != pstVPlayer->eStatus)
				{
					continue;
				}

				apstPlayers[iCount++] = pstVPlayer;
			break;

			case OBJ_MONSTER:
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					continue;
				}
				pstVMon = (Monster *)pstMemBlock->szData;

				if (is_mon_machine(pstVMon))
				{
					machine_msg_broadcast(pstEnv, pstVMon, pstPkg);
				}
				
				break;
		}
	}

	pstSkillDef = z_find_skill(pstEnv, pststMonAtkInfo->unSkillID, pststMonAtkInfo->bSkillLevel, &pststMonAtkInfo->iSkillIdx);
	if(pstSkillDef&&pstSkillDef->JiqiChgFlag==2 && 
		pstPkg->Head.Cmd == ACTION_RES &&
		pstPkg->Body.ActionRes.ActionID == ACTION_HIT)
	{
		pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
		if (!pstMapInst)
		{
			return z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
		                                iCount, pstPkg);
		}
		z_sendpkg_atkposview(pstAppCtx, pstEnv, pstMapInst, &pststMonAtkInfo->stAtkPos,pstPkg,apstPlayers, iCount);
	}
	else if (iCount > 0)
	{
		return z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
		                                iCount, pstPkg);
	}

	return 0;
}

int z_sendpkg_monview(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, CSPKG *pstPkg, int iImportant)
{
	z_sendpkg_monview_except(pstAppCtx, pstEnv, pstMon, pstPkg, iImportant, NULL);
	return 0;
}

//以打击点为准
int z_sendpkg_atkposview(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst, 
							RESPOS *pstAtkPos,CSPKG *pstPkg,Player **apstPlayerView, int iNum)
{
	int i,j;
	Player *pstPlayerTmp;
	Player *apstPlayerHit[MAX_SKILL_TARGET];
	int iPlayerHit = 0;
	Player *apstPlayerNotify[VISIBLE_ANIMATE_VALVE + MAX_SKILL_TARGET];
	int iPlayerNotify = 0;
	CSHITS *pstHits = &pstPkg->Body.ActionRes.Data.Hit;
	CSONEHIT *pstOneHit;
	SearchArea stSArea;
	int iPlayerNotifyTmp = 0;

	if( !(pstPkg->Head.Cmd == ACTION_RES &&
	    pstPkg->Body.ActionRes.ActionID == ACTION_HIT))
	{
		return -1;
	}
		
	for (i=0; i<pstHits->TotalHit; i++)
	{
		pstOneHit = pstHits->Hits+i;
		pstPlayerTmp = z_id_player(pstAppCtx, pstEnv, pstOneHit->ID);
		if (pstPlayerTmp)
		{
			apstPlayerHit[iPlayerHit++] = pstPlayerTmp;
			pstPlayerTmp->stOnline.cMiscFlag &= ~MISC_FLAG_TAG;
		}
	}
	
	//以打击点为准
	if(0 == z_pos_search_area(pstAppCtx, pstEnv, pstMapInst, pstAtkPos, &stSArea))
	{
		int j;
		AREADYN *pstArea;
		
		for(i = 0; i < stSArea.iNum; i++)
		{
			if (iPlayerNotify >= VISIBLE_ANIMATE_VALVE)
			{
				break;
			}
			
			pstArea = stSArea.pastArea[i];

			for (j = 0; j < pstArea->iAnimate; j++)
			{
				if (iPlayerNotify >= VISIBLE_ANIMATE_VALVE)
				{
					break;
				}
				
				pstPlayerTmp = z_id_player(pstAppCtx, pstEnv, pstArea->aiAnimate[j]);
				if (NULL == pstPlayerTmp || PLAYER_STATUS_ROLE_LOGIN != pstPlayerTmp->eStatus)
				{
					continue;
				}
 
				if (pstArea->iAnimate > VISIBLE_ANIMATE_VALVE && 
					!z_pos_in_view_half(pstAtkPos, &pstPlayerTmp->stRoleData.Pos))
				{
					continue;
				}

				apstPlayerNotify[iPlayerNotify++] = pstPlayerTmp;
				pstPlayerTmp->stOnline.cMiscFlag |= MISC_FLAG_TAG;
			}
		}
	}

	//剔除重复的
	iPlayerNotifyTmp = iPlayerNotify;
	for (i=0; i<iPlayerHit; i++)
	{
		pstPlayerTmp = apstPlayerHit[i];
		if (0 == (pstPlayerTmp->stOnline.cMiscFlag & MISC_FLAG_TAG))
		{
			for(j=0;j<iPlayerNotifyTmp;j++)
			{
				if(apstPlayerNotify[j] == pstPlayerTmp)
				{
					break;
				}
			}
			if(j<iPlayerNotifyTmp)
			{
				apstPlayerNotify[iPlayerNotify++] = pstPlayerTmp;
			}
		}
	}

	iPlayerNotifyTmp = iPlayerNotify;
	for (i=0; i<iNum; i++)
	{
		pstPlayerTmp = apstPlayerView[i];
		if (0 == (pstPlayerTmp->stOnline.cMiscFlag & MISC_FLAG_TAG))
		{
			for(j=0;j<iPlayerNotifyTmp;j++)
			{
				if(apstPlayerNotify[j] == pstPlayerTmp)
				{
					break;
				}
			}
			if(j<iPlayerNotifyTmp)
			{
				apstPlayerNotify[iPlayerNotify++] = pstPlayerTmp;
			}
		}
	}

	z_cltmsg_limit_broadcast(pstAppCtx, pstEnv, apstPlayerNotify, iPlayerNotify, pstPkg);

	return 0;
}

int z_monster_move_notify(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster  *pstMon, char cMoveType)
{
	CSWALK *pstCSWalk;
	CSTURN *pstCSTurn;
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSMOVEOTHER *pstMoveOther = &pstPkg->Body.MoveOther;
	
	pstMoveOther->ID = pstMon->iID;
	pstMoveOther->x = pstMon->stCurrPos.X;
	pstMoveOther->y = pstMon->stCurrPos.Y;
	
	pstMoveOther->MoveType = cMoveType;
	if (CS_MOVE_TURN == cMoveType)
	{
		pstCSTurn = &pstMoveOther->Data.Turn;

		pstCSTurn->Dir = pstMon->bDirection;
	}
	else
	{
		pstCSWalk = &pstMoveOther->Data.Walk;

		pstCSWalk->Total = 1;

		pstCSWalk->Points[0].x = pstMon->stNextPos.X;
		pstCSWalk->Points[0].y = pstMon->stNextPos.Y;
	}

	memcpy(&pstMoveOther->MspdInfo, &pstMon->stMspdInfo, sizeof(pstMoveOther->MspdInfo));

	Z_CSHEAD_INIT(&pstPkg->Head, MOVE_OTHER);

	z_sendpkg_monview(pstAppCtx, pstEnv, pstMon, pstPkg, 0);
		
	return 0;
}

int z_mon_stop(ZONESVRENV* pstEnv, Monster *pstMon)
{

	if (pstMon->cMoving)
	{
		z_monster_move_curr(pstEnv->pstAppCtx, pstEnv, pstMon, 0);
	}

	pstMon->stNextPos = pstMon->stCurrPos;
	pstMon->cMoving = 0;

	z_monster_move_notify(pstEnv->pstAppCtx, pstEnv, pstMon, CS_MOVE_TURN);

	return 0;
}

int z_player_chgpos(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						RESPOS *pstPos)
{
	RESPOS stPos;
	ZONEMAPINST* pstMapInst;
	MapIndex* pstMapIdx;
	ZONEIDX stIdx;
	
	memcpy(&stPos, &pstPlayer->stRoleData.Pos, sizeof(stPos));

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst  =   z_find_map_inst(pstAppCtx, pstEnv,  &stIdx);
	if(!pstMapInst)
		return -1;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if(!pstMapIdx)
		return -1;

	pstPlayer->stRoleData.Pos.X	=	pstPos->X;
	pstPlayer->stRoleData.Pos.Y	=	pstPos->Y;

	z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstPlayer->iMemID, &stPos, 0);

/*
	if(!z_in_same_area(pstMapIdx, pstEnv->pstAreaDynPool, pstMapInst, &stPos, &pstPlayer->stRoleData.Pos))
	{
		if (0 > (iRet = z_ani_leave_area(pstMapIdx, pstEnv->pstAreaDynPool, pstPlayer->iMemID, &stPos, pstMapInst)))
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player(%d) z_ani_leave_area ret %d", pstPlayer->stRoleData.Uin, iRet);
		}
		if (0 > (iRet = z_ani_enter_area(pstMapIdx, pstEnv->pstAreaDynPool, pstPlayer->iMemID, &pstPlayer->stRoleData.Pos, pstMapInst)))
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player(%d) z_ani_enter_area ret %d", pstPlayer->stRoleData.Uin, iRet);
		}
	}
*/

	z_player_stop(pstAppCtx, pstEnv, pstPlayer, 1, NULL);	

	return 0;
}

int z_mon_chgpos(ZONESVRENV* pstEnv, Monster *pstMon, RESPOS *pstPos)
{
	RESPOS stPos;
	ZONEMAPINST* pstMapInst;
	MapIndex* pstMapIdx;
	
	memcpy(&stPos, &pstMon->stCurrPos, sizeof(stPos));

	pstMapInst  =   z_find_map_inst(pstEnv->pstAppCtx, pstEnv,  &pstMon->stMap);
	if(!pstMapInst)
		return -1;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if(!pstMapIdx)
		return -1;

	pstMon->cMoving = 0;

	pstMon->stCurrPos = *pstPos;

	z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstMon->iID, &stPos, 0);

	z_mon_stop(pstEnv, pstMon);	

	z_mon_search_view(pstEnv->pstAppCtx, pstEnv, pstMapInst, &pstMon->stCurrPos, pstMon);

	return 0;
}


//怪物逃跑	iDist 奔跑距离<= 怪物的当前速度，尽量往AtkPos 的反方向
int z_mon_run_away(ZONESVRENV* pstEnv, Monster*pstMon, RESPOS *pstAtkPos, int iDist, int iAiStat)
{
	int i, iRet;
	int iDir, iMoveDist;
	RESPOS stNextPos;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	TMAPMASK *pstMask;
	MONSTERDEF *pstMonsterDef;

	if(MOVE_TYPE_NO == pstMon->bMoveType)
	{
		return 0;
	}

	if (MONSTER_DIE == pstMon->cStat)
	{
		return 0;
	}

	if (iDist > 0 && pstAtkPos && z_distance(pstAtkPos, &pstMon->stCurrPos) > iDist)
	{
		return 0;
	}

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	pstMapIdx =  z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx) return -1;

	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);

	pstMonsterDef = z_get_mon_def(pstEnv, pstMon);
	if (NULL == pstMonsterDef)
	{
		return -1;
	}

	pstMon->bAiStat = iAiStat;
	z_mon_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstMon, 0);

	if( iDist == 0  || iDist > z_get_mon_spd(pstEnv->pstAppCtx, pstEnv, pstMon, pstMonsterDef))
	{
		iMoveDist = 0.45*z_get_mon_spd(pstEnv->pstAppCtx, pstEnv, pstMon, pstMonsterDef);
	}
	else
	{
		iMoveDist = iDist;
	}

	if (pstAtkPos)
	{
		iDir	=	FaceAngleBetween(pstAtkPos->X, pstAtkPos->Y,  pstMon->stCurrPos.X,  pstMon->stCurrPos.Y);
	}
	else
	{
		iDir  = (int)RAND1(90);
	}

	for (i=0; i<3; i++)
	{
		//iMoveDist = iMoveDist >> i;
		stNextPos.X = (int)(pstMon->stCurrPos.X + cosf(iDir*PI/180.0f)* iMoveDist);		
		stNextPos.Y = (int)(pstMon->stCurrPos.Y - sinf(iDir*PI/180.0f)* iMoveDist);	
		if (IsPixelLineThrough(pstMask, pstMon->stCurrPos.X, pstMon->stCurrPos.Y, 
		 							stNextPos.X, stNextPos.Y, maskStop) &&
		 	dyn_msk_check(pstEnv, pstMapInst, pstMask, pstMon->stCurrPos.X, 
		 			pstMon->stCurrPos.Y, stNextPos.X, stNextPos.Y,z_get_monsterdef_misctype(pstEnv,pstMon), DYN_MSK_STOP) &&						
		 	z_pos_area(pstEnv->pstMapObj, pstMapIdx, &stNextPos)) 
		{
			break;
		}

		iDir  += (int)RAND1(90);
	}

	if (i == 3 || (int)pstMonsterDef->TraceDist < z_distance(&pstMon->stInitPos, &stNextPos))
	{
		if ( 0 > z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstMon->stCurrPos, &pstMon->stCurrPos, iMoveDist, 100, &stNextPos))
		{
			return -1;
		}
	}

	pstMon->stNextPos = stNextPos;
      	iRet = mon_have_movespd(pstMon);
	if (0 == iRet) //冰冻
	{
		return 0;
	}
	else
	{
		pstMon->stMspdInfo.MoveSpdCurr = iRet;
		pstMon->stMspdInfo.NextNum = 0;
	}
	
	pstMon->cMoving = 1;
	pstMon->tLastTrace = pstEnv->pstAppCtx->stCurr.tv_sec;
	z_monster_move_notify(pstEnv->pstAppCtx, pstEnv, pstMon, CS_MOVE_WALK);
	TV_CLONE(pstMon->stLastMoveTv, pstEnv->pstAppCtx->stCurr);

	return 0;
}

int z_svr_mon_move( ZONESVRENV* pstEnv, Monster*pstMon)
{
	ZoneStatus *pstStutas;
	Player *pstApplyPlayer = NULL;

	pstStutas = z_is_mon_status_result(pstEnv, pstMon, STATUS_RESULT_RANDOM_RUN);
	if (pstStutas && pstStutas->iApplyID && pstStutas->bCount < 3)
	{
		pstStutas->bCount++;
		pstApplyPlayer	=	z_id_player(pstEnv->pstAppCtx, pstEnv, pstStutas->iApplyID);
	}

	if (pstMon->stMonAtkInfo.cAtkStat == CS_STAT_PREPARE)
	{
		z_mon_skill_break(pstEnv, pstMon);
	}
	pstMon->stMonAtkInfo.cAtkStat = 0;
	
	if(pstApplyPlayer && pstApplyPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
	{
		if (pstApplyPlayer->stOnline.cMoving)
		{
			z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstApplyPlayer, 0);
		}
		
		return z_mon_run_away(pstEnv, pstMon, &pstApplyPlayer->stRoleData.Pos, 0, AI_STAT_MOVE);
	}
	else
	{
		return z_mon_run_away(pstEnv, pstMon, NULL, 0, AI_STAT_MOVE);
	}

	return 0;
}

int z_svr_player_move( ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stCsPkg;
	CSMOVEC *pstMove = &stCsPkg.Body.MoveReq;
	TFRAMEHEAD stFrameHead;
	int iDir;	
	int i;
	int iTmpSeq;
	RESPOS stNextPos;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	TMAPMASK *pstMask;
	ZoneStatus *pstStutas;
	Player *pstApplyPlayer = NULL;
	Monster *pstApplyMon = NULL;
	struct timeval stTmpLastCltMoveTime;
	TMEMBLOCK *pstMemBlock;
	int iIndex;
	char cType;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst) return -1;
	pstMapIdx =  z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx) return -1;

	iDir  =  (int)RAND1(90);

	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);
	
	pstStutas = z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_RANDOM_RUN);
	if (pstStutas && pstStutas->iApplyID && pstStutas->bCount < 3)
	{
		pstStutas->bCount++;

		iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstStutas->iApplyID, &cType);
		if (iIndex >= 0)
		{
			switch(cType)
			{
				case OBJ_PLAYER:
					pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
					if (pstMemBlock->fValid)
					{
						pstApplyPlayer = (Player *)pstMemBlock->szData;
						if (pstApplyPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
						{
							if (pstApplyPlayer->stOnline.cMoving)
							{
								z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstApplyPlayer, 0);
							}
							iDir	=	FaceAngleBetween(pstApplyPlayer->stRoleData.Pos.X, pstApplyPlayer->stRoleData.Pos.Y,  pstPlayer->stRoleData.Pos.X,  pstPlayer->stRoleData.Pos.Y);
						}
					}
					break;
				case OBJ_MONSTER:
					pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
					if (pstMemBlock->fValid)
					{
						pstApplyMon = (Monster *)pstMemBlock->szData;
						
						if (pstApplyMon->cMoving)
						{
							z_monster_move_curr(pstEnv->pstAppCtx, pstEnv, pstApplyMon, 0);
						}
						iDir	=	FaceAngleBetween(pstApplyMon->stCurrPos.X, pstApplyMon->stCurrPos.Y,  pstPlayer->stRoleData.Pos.X,  pstPlayer->stRoleData.Pos.Y);
					}
					break;
				default:
					break;
			}
		}
	}

	for(i = 3; i>0; i--)
	{
		stNextPos.X = (int)(pstPlayer->stRoleData.Pos.X + cosf(iDir*PI/180.0f)* 0.8*z_get_player_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer));		
		stNextPos.Y = (int)(pstPlayer->stRoleData.Pos.Y - sinf(iDir*PI/180.0f)* 0.8*z_get_player_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer));

		if (MAX_RANDOM_RUN_DIST < z_distance(&pstPlayer->stRoleData.Pos, &stNextPos))
		{
			continue;
		}
		
		if (IsPixelLineThrough(pstMask, pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y, 
		 							stNextPos.X, stNextPos.Y, maskStop) &&
		 	dyn_msk_check(pstEnv, pstMapInst, pstMask, pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y, 
		 							stNextPos.X, stNextPos.Y, 0,DYN_MSK_STOP) &&						
		 	z_pos_area(pstEnv->pstMapObj, pstMapIdx, &stNextPos)) 
		{
			break;
		}
		 
		 iDir  =  (int)RAND1(90);
	}

	if (i <=0)
	{
		return -1;
	}
		
	iTmpSeq = pstPlayer->stOnline.iPosSeq;
	stTmpLastCltMoveTime = pstPlayer->stOnline.stLastCltMoveTime; 
	
	pstMove->MoveSeq = pstPlayer->stOnline.iPosSeq;
	pstMove->SvrTime.BenchTimeSec = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstMove->SvrTime.BenchTimeUSec =  pstEnv->pstAppCtx->stCurr.tv_usec;
	pstPlayer->stOnline.stLastCltMoveTime.tv_sec = 0;
	pstPlayer->stOnline.stLastCltMoveTime.tv_usec = 0;

	stFrameHead.TimeStamp.TimeVal.sec = pstMove->SvrTime.BenchTimeSec ;
	stFrameHead.TimeStamp.TimeVal.usec =pstMove->SvrTime.BenchTimeUSec;

	pstMove->MoveType = CS_MOVE_WALK;
	pstMove->Data.Walk.Total = 1;
	pstMove->x = pstPlayer->stRoleData.Pos.X;
	pstMove->y = pstPlayer->stRoleData.Pos.Y;
	pstMove->Data.Walk.Points[0].x = stNextPos.X;
	pstMove->Data.Walk.Points[0].y = stNextPos.Y;

	z_player_move(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stFrameHead, &stCsPkg);

	pstPlayer->stOnline.stLastCltMoveTime = stTmpLastCltMoveTime;
	pstPlayer->stOnline.iPosSeq = iTmpSeq;

	if(pstPlayer->stOnline.cMoving)
	{
		z_player_move_notify_one(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstPlayer);
	}
	
	return 0;
}

// 制定路径id,给玩家,让玩家走起来
int z_svr_player_move_by_patroldef( ZONESVRENV* pstEnv, Player *pstPlayer, RESPOS *pstPatrolPos, unsigned int uiNum)
{
	CSPKG stCsPkg;
	CSMOVEC *pstMove = &stCsPkg.Body.MoveReq;
	TFRAMEHEAD stFrameHead;
	unsigned int i;
	int iTmpSeq;
	ZONEIDX stIdx;
	MapIndex *pstMapIdx;
	TMAPMASK *pstMask;
	struct timeval stTmpLastCltMoveTime;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;


	memset(pstMove,0,sizeof(CSMOVEC));
	
	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;

	pstMapIdx =  z_map_inst_index(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (!pstMapIdx) return -1;

	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);
		
	iTmpSeq = pstPlayer->stOnline.iPosSeq;
	stTmpLastCltMoveTime = pstPlayer->stOnline.stLastCltMoveTime; 
	
	pstMove->MoveSeq = pstPlayer->stOnline.iPosSeq;
	pstMove->SvrTime.BenchTimeSec = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstMove->SvrTime.BenchTimeUSec =  pstEnv->pstAppCtx->stCurr.tv_usec;
	pstPlayer->stOnline.stLastCltMoveTime.tv_sec = 0;
	pstPlayer->stOnline.stLastCltMoveTime.tv_usec = 0;

	stFrameHead.TimeStamp.TimeVal.sec = pstMove->SvrTime.BenchTimeSec ;
	stFrameHead.TimeStamp.TimeVal.usec =pstMove->SvrTime.BenchTimeUSec;

	pstMove->MoveType = CS_MOVE_WALK;
	
	pstMove->x = pstPlayer->stRoleData.Pos.X;
	pstMove->y = pstPlayer->stRoleData.Pos.Y;

	pstMove->Data.Walk.Total = 0;
	for(i = 0; i<uiNum && i < CS_MAX_MOVEPOINTS; i++)
	{
		if(pstPatrolPos[i].X == 0 || pstPatrolPos[i].Y == 0)
		{
			break;
		}
		pstMove->Data.Walk.Points[i].x = pstPatrolPos[i].X;
		pstMove->Data.Walk.Points[i].y = pstPatrolPos[i].Y;
		pstMove->Data.Walk.Total++;
	}

	//保留下删除的点坐标
	if(pstMove->Data.Walk.Total > 0)
	{
		pstRoleTmpSys->DelPos.X = pstMove->Data.Walk.Points[pstMove->Data.Walk.Total-1].x;
		pstRoleTmpSys->DelPos.Y = pstMove->Data.Walk.Points[pstMove->Data.Walk.Total-1].y;
	}

	z_player_move(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stFrameHead, &stCsPkg);

	pstPlayer->stOnline.stLastCltMoveTime = stTmpLastCltMoveTime;
	pstPlayer->stOnline.iPosSeq = iTmpSeq;
	pstPlayer->stOnline.cPatrolFlag = 1;
	if(pstPlayer->stOnline.cMoving)
	{
		z_player_move_notify_one(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstPlayer);
	}
	
	return 0;
}
// 巡逻结束了,如果是有临时道具的要把临时道具删除
int z_player_stop_by_patrol_end( ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;

	// 严格判断到终点才删除
	if(pstRoleTmpSys->TmpItemID != 0 && 
		pstRoleTmpSys->PatrolID > 0 &&
		pstRoleTmpSys->DelPos.X == pstPlayer->stRoleData.Pos.X &&
		pstRoleTmpSys->DelPos.Y == pstPlayer->stRoleData.Pos.Y)
	{
		if(pstPlayer->stOnline.cMiscFlag & MISC_FLAG_TMPITEM_PATROL_TRACE)
		{
			task_player_trace(pstEnv, pstPlayer, TASK_OBJ_TMPITEM_PATROL_END);
		}
		//弄到脚本里去删除了
		tmp_item_del(pstEnv,pstPlayer);
	}
	else if(pstRoleTmpSys->TmpItemID != 0 && 
				pstRoleTmpSys->PatrolID > 0 &&
				!(pstPlayer->stOnline.State & CS_STAT_PREPARE) &&
				(pstPlayer->stOnline.stLastCltTime.tv_sec < pstEnv->pstAppCtx->stCurr.tv_sec)  )
	{
		// 如果没到终点就停下来了(比如发技能),就继续走
		memcpy(&pstPlayer->stOnline.stRemainPatrol.stTurnPoints[0], 
					&pstPlayer->stRoleData.Pos, 
					sizeof(RESPOS));	
		z_svr_player_move_by_patroldef( pstEnv, pstPlayer,pstPlayer->stOnline.stRemainPatrol.stTurnPoints,
								pstPlayer->stOnline.stRemainPatrol.cFuturePoints);
	}
	
	
	

	return 0;
}

//匀变速直线运动的速度与时间关系的公式 V=V0+at 
//t是秒
int z_move_get_v_spd(int v0,int a,float t)
{
	float V = 0;
	V = v0 + a*t;

	return (int)V;
}

//匀变速直线运动的位移与时间关系的公式s=(v0+v)*t/2=v0*t+1/2*at^2
//0 = ax^2 + bx + c
//x = [-b ± sqrt( b^2 - 4ac )] / (2a)
//1/2*at^2 + v0*t - x = 0
float z_move_get_t_time(int v0,int a,int s)
{
	float t = 0.000;

	t = (-v0 + sqrt(v0*v0 - 4*( 1/2.0*a)*(- s) ) )/(2*(1/2.0*a));
	if( t > 0)
	{
		return t;
	}

	t = (-v0 - sqrt(v0*v0 - 4*( 1/2.0*a)*(- s) ) )/(2*(1/2.0*a));
	if( t > 0)
	{
		return t;
	}

	return 0;
}

// 匀变速直线运动的速度与时间关系的公式：v=v0+at
// 匀变速直线运动的位移与时间关系的公式：x=(v0+v)*t/2=v0*t+1/2*at^2　
int z_move_get_x_distance(int v0,int a,int v)
{
	float t = 0.0;
	int x = 0;

	//把时间算出来
	t =  (v - v0)*1.0/a;

	x = v0*t + 1/2.0*a*t*t;
	
	return x;
}


/*区域视图,主要用来处理npc的下发问题,npc是无视野的,并且所有人可见
int z_player_area_view_imp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, Player *pstPlayer,
							RESPOS *pstCurrPos,RESPOS *pstNextPos)
{
	SearchArea 	stSArea_curr;
	SearchArea 	stSArea_next;
	SearchArea 	stSArea_leave;
	SearchArea 	stSArea_enter;
	AREADYN 	*pstArea = NULL;
	MapIndex 	*pstMapIdx = NULL;
	int  		i = 0;
	int  		j = 0;
	int  		iIndex = 0;
	char 		cType = 0;
	TMEMBLOCK 	*pstMemBlock = NULL;
	Monster 	*pstMon = NULL;
	CSPKG stPkg;
	CSPKG 		*pstPkg = &stPkg;
	CSLEAVEVIEW *pstLeaveView = &pstPkg->Body.LeaveView;

	memset(&stSArea_curr,0,sizeof(stSArea_curr));
	memset(&stSArea_next,0,sizeof(stSArea_next));
	memset(&stSArea_leave,0,sizeof(stSArea_leave));
	memset(&stSArea_enter,0,sizeof(stSArea_enter));

	pstMapIdx =  z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}
	
	if (pstMapInst->iMapInstID != pstPlayer->stRoleData.Map)
	{
		return -1;
	}
	
	if (pstCurrPos && pstCurrPos->X >0 &&		
		0 > z_pos_search_area(pstAppCtx, pstEnv, pstMapInst, pstCurrPos, &stSArea_curr))
	{
		return -1;
	}
	if (0 > z_pos_search_area(pstAppCtx, pstEnv, pstMapInst, pstNextPos, &stSArea_next))
	{
		return -1;
	}
	//tlog_error(pstEnv->pstLogCat, 0, 0, "mapid %d,MapInst %d,iNum %d, pastArea %d", pstMapIdx->stMapDef.MapID, pstMapInst,
	//				stSArea_next.iNum,stSArea_next.pastArea);

	//离开的区域
	for( i=0; i<stSArea_curr.iNum; i++)
	{
		for( j=0; j<stSArea_next.iNum;j++)
		{
			if(stSArea_curr.pastArea[i] == stSArea_next.pastArea[j])
			{
				//这个区域是上个区域里的不管了
				break;
			}
		}
		
		if(j>=stSArea_next.iNum)
		{
			//离开的区域
			stSArea_leave.pastArea[stSArea_leave.iNum++] = stSArea_curr.pastArea[i];
		}
	}

	//进入的区域
	for( i=0; i<stSArea_next.iNum; i++)
	{
		for( j=0; j<stSArea_curr.iNum;j++)
		{
			if(stSArea_next.pastArea[i] == stSArea_curr.pastArea[j])
			{
				//这个区域是上个区域里的不管了
				break;
			}
		}
		
		if(j>=stSArea_curr.iNum)
		{
			//离开的区域
			stSArea_enter.pastArea[stSArea_enter.iNum++] = stSArea_next.pastArea[i];
		}
	}

	//npc 离开视野
	for (i = 0; i < stSArea_leave.iNum; i++) 
	{
		pstArea = stSArea_leave.pastArea[i];
		for (j = 0; j < pstArea->iAnimate; j++)
        {
			iIndex = z_id_indextype(pstAppCtx, pstEnv,  pstArea->aiAnimate[j], &cType);

			if (iIndex < 0) continue;

			if (cType == OBJ_NPC ||
				cType == OBJ_MONSTER)
			{	
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
                if(!pstMemBlock->fValid)
                {
                	continue;
                }
                pstMon = (Monster *)pstMemBlock->szData;	
				if( !(pstMon->cMonMiscFlag & MON_MISC_FLAG_IMPVIEW) ||
					pstMon->iInitMap != pstPlayer->stRoleData.Map)
				{
					continue;
				}
				
				//离开视野
				pstLeaveView->ID = pstMon->iID;
				pstLeaveView->HomeFlag = 0;
				Z_CSHEAD_INIT(&pstPkg->Head, LEAVE_VIEW);
				z_cltmsg_send(pstEnv, pstPlayer, pstPkg);
							
			}
		}
	}

	//npc 进入视野
	for (i = 0; i < stSArea_enter.iNum; i++) 
	{
		pstArea = stSArea_enter.pastArea[i];
		for (j = 0; j < pstArea->iAnimate; j++)
        {
			iIndex = z_id_indextype(pstAppCtx, pstEnv,  pstArea->aiAnimate[j], &cType);

			if (iIndex < 0) continue;

			if (cType == OBJ_NPC ||
				cType == OBJ_MONSTER)
			{	
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
                if (!pstMemBlock->fValid)
                {
                        continue;
                }
                pstMon = (Monster *)pstMemBlock->szData;	
				if( !(pstMon->cMonMiscFlag & MON_MISC_FLAG_IMPVIEW) ||
					pstMon->iInitMap != pstPlayer->stRoleData.Map )
				{
					continue;
				}
				
				z_player_view_monster(pstAppCtx, pstEnv, pstPlayer, pstMon);							
			}
		}
	}	
	return 0;
}*/

/*区域视图,主要用来处理npc的下发问题,npc是无视野的,并且所有人可见
int z_monster_area_view_imp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, Monster *pstMon,
							RESPOS *pstCurrPos,RESPOS *pstNextPos)
{
	SearchArea 	stSArea_curr;
	SearchArea 	stSArea_next;
	SearchArea 	stSArea_leave;
	SearchArea 	stSArea_enter;
	AREADYN 	*pstArea = NULL;
	MapIndex 	*pstMapIdx = NULL;
	int  		i = 0;
	int  		j = 0;
	int  		iIndex = 0;
	char 		cType = 0;
	TMEMBLOCK 	*pstMemBlock = NULL;
	Player		*pstVPlayer = NULL;	
	CSPKG 		stPkg;
	CSPKG 		*pstPkg = &stPkg;
	CSLEAVEVIEW *pstLeaveView = &pstPkg->Body.LeaveView;

	memset(&stSArea_curr,0,sizeof(stSArea_curr));
	memset(&stSArea_next,0,sizeof(stSArea_next));
	memset(&stSArea_leave,0,sizeof(stSArea_leave));
	memset(&stSArea_enter,0,sizeof(stSArea_enter));

	if( !(pstMon->cMonMiscFlag & MON_MISC_FLAG_IMPVIEW))
	{
		return -1;
	}

	pstMapIdx =  z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}
	
	if (pstMapInst->iMapInstID != pstMon->iInitMap)
	{
		return -1;
	}
	
	if (pstCurrPos && pstCurrPos->X >0 &&		
		0 > z_pos_search_area(pstAppCtx, pstEnv, pstMapInst, pstCurrPos, &stSArea_curr))
	{
		return -1;
	}
	if (0 > z_pos_search_area(pstAppCtx, pstEnv, pstMapInst, pstNextPos, &stSArea_next))
	{
		return -1;
	}

	//离开的区域
	for( i=0; i<stSArea_curr.iNum; i++)
	{
		for( j=0; j<stSArea_next.iNum;j++)
		{
			if(stSArea_curr.pastArea[i] == stSArea_next.pastArea[j])
			{
				//这个区域是上个区域里的不管了
				break;
			}
		}
		
		if(j>=stSArea_next.iNum)
		{
			//离开的区域
			stSArea_leave.pastArea[stSArea_leave.iNum++] = stSArea_curr.pastArea[i];
		}
	}

	//进入的区域
	for( i=0; i<stSArea_next.iNum; i++)
	{
		for( j=0; j<stSArea_curr.iNum;j++)
		{
			if(stSArea_next.pastArea[i] == stSArea_curr.pastArea[j])
			{
				//这个区域是上个区域里的不管了
				break;
			}
		}
		
		if(j>=stSArea_curr.iNum)
		{
			//离开的区域
			stSArea_enter.pastArea[stSArea_enter.iNum++] = stSArea_next.pastArea[i];
		}
	}

	//npc 离开视野
	for (i = 0; i < stSArea_leave.iNum; i++) 
	{
		pstArea = stSArea_leave.pastArea[i];
		for (j = 0; j < pstArea->iAnimate; j++)
        {
			iIndex = z_id_indextype(pstAppCtx, pstEnv,  pstArea->aiAnimate[j], &cType);

			if (iIndex < 0) continue;

			if (cType == OBJ_PLAYER)
			{	
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					continue;
				}
				pstVPlayer = (Player *)pstMemBlock->szData;
				if (PLAYER_STATUS_ROLE_LOGIN != pstVPlayer->eStatus ||
					pstVPlayer->stOnline.bConnLost || 
					pstVPlayer->ucRoleLogoutReason == ROLE_LOGOUT_GRAB ||
					pstMon->iInitMap != pstVPlayer->stRoleData.Map)
				{
					continue;
				}
			
				//离开视野
				pstLeaveView->ID = pstMon->iID;
				pstLeaveView->HomeFlag = 0;
				Z_CSHEAD_INIT(&pstPkg->Head, LEAVE_VIEW);
				z_cltmsg_send(pstEnv, pstVPlayer, pstPkg);
							
			}
		}
	}

	//npc 进入视野
	for (i = 0; i < stSArea_enter.iNum; i++) 
	{
		pstArea = stSArea_enter.pastArea[i];
		for (j = 0; j < pstArea->iAnimate; j++)
        {
			iIndex = z_id_indextype(pstAppCtx, pstEnv,  pstArea->aiAnimate[j], &cType);

			if (iIndex < 0) continue;

			if (cType == OBJ_PLAYER)
			{	
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					continue;
				}
				
				pstVPlayer = (Player *)pstMemBlock->szData;		
				if( pstMon->iInitMap != pstVPlayer->stRoleData.Map)
				{
					continue;
				}
					
				z_player_view_monster(pstAppCtx, pstEnv, pstVPlayer, pstMon);							
			}
		}
	}	
	return 0;
}*/

