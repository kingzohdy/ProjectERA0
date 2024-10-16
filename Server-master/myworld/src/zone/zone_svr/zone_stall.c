#include "zone_stall.h"
#include "zone_clt.h"
#include "zone_package.h"
#include "zone_pet.h"
#include "zone_player.h"
#include "zone_move.h"
#include "zone_map.h"
#include "zone_db.h"
#include "libdirty.h"
#include "zone_err.h"
#include "zone_act_status.h"
#include "zone_oplog.h"
#include "zone_safelock.h"
#include "zone_misc.h"
#include "zone_machine.h"
#include "zone_err.h"
#include "zone_relation.h"


#define MIN_STALL_RADIUS  300
#define MIN_STALL_ROLE_LEVLE 20
//#define MAX_STALL_SEE_DIST 800
#define STALL_AD_ACTIVE 30

tdr_ulonglong stall_count_price(ZONESVRENV* pstEnv, Player *pstPlayer);
// 道具是否上架
int item_is_up(Player *pstPlayer, tdr_ulonglong ullWID)
{
	int i;
	STALLDATA *pstCSStallData = &pstPlayer->stOnline.stStallOnline.stStallData;

	for (i=0; i<pstCSStallData->ItemNum; i++)
	{
		if (pstCSStallData->ItemData[i].WID == ullWID)
		{
			return 1;
		}
	}
	return 0;
}

// 宠物是否上架
int pet_is_up(Player *pstPlayer, tdr_ulonglong ullWID)
{
	int i;
	STALLDATA *pstCSStallData = &pstPlayer->stOnline.stStallOnline.stStallData;

	for (i=0; i<pstCSStallData->PetNum; i++)
	{
		if (pstCSStallData->PetData[i].WID == ullWID)
		{
			return 1;
		}
	}
	return 0;
}

int stall_start_check_pos(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i,j;
	SearchArea stSArea;
	AREADYN *pstArea;
	Player *pstOther;
	Npc *pstNpc;
	RESPOS *pstPos = &pstPlayer->stRoleData.Pos;
	int iIndex;
	char cType;
	int iID;
	MapIndex* pstMapIdx;
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;

	stIdx.iID	 =	pstPlayer->stRoleData.Map;
	stIdx.iPos =	pstPlayer->stOnline.iMapPos;
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

	if (z_pos_search_area(pstEnv->pstAppCtx, pstEnv, pstMapInst, &pstPlayer->stRoleData.Pos, &stSArea) != 0)
	{
		return -1;
	}

	for(i = 0; i < stSArea.iNum; i++)
	{
		pstArea = stSArea.pastArea[i];

		for (j = 0; j < pstArea->iAnimate; j++)
		{
			iID = pstArea->aiAnimate[j];
			iIndex	=	z_id_indextype(pstEnv->pstAppCtx, pstEnv,iID, &cType);
			if( -1==iIndex )
				continue;

			switch(cType)
			{
				case OBJ_PLAYER:
					pstOther = z_id_player(pstEnv->pstAppCtx, pstEnv, iID);
					if (NULL == pstOther || PLAYER_STATUS_ROLE_LOGIN != pstOther->eStatus)
						continue;

					if ( (pstOther->stOnline.State & CS_STAT_STALL) && 
						pstOther->stRoleData.Map == pstPlayer->stRoleData.Map &&
						z_distance(&pstOther->stRoleData.Pos, pstPos) <= MIN_STALL_RADIUS)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL1);
						return -1;
					}

					break;
				case OBJ_NPC:
					pstNpc	=	z_id_npc(pstEnv->pstAppCtx, pstEnv, iID);
					if (NULL == pstNpc)
						continue;

					if ( 	pstNpc->stMon.iPatrolID == 0 &&
						pstNpc->stMon.iInitMap == pstPlayer->stRoleData.Map &&
						z_distance(&pstNpc->stMon.stCurrPos, pstPos) <= MIN_STALL_RADIUS)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL2);
						return -1;
					}
					break;
			
				default:
					break;
			}
		}
	}
	return 0;
}


int stall_msg_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, int iCmd)
{
	CSPKG stPkg;
	CSSTALLSVR *pstStallSvr = &stPkg.Body.StallSvr;
	
	pstStallSvr->Type = iCmd;

	if (iCmd == SVR_STALL_START)
	{
		STRNCPY(pstStallSvr->StallCltData.StallStartSvr.StallName, pstPlayer->stOnline.stStallOnline.szStallName,
					sizeof(pstStallSvr->StallCltData.StallStartSvr.StallName));
	}
	else if(iCmd == SVR_STALL_CHANGE_NAME)
	{
		STRNCPY(pstStallSvr->StallCltData.StallChgNameSvr.StallName, pstPlayer->stOnline.stStallOnline.szStallName,
					sizeof(pstStallSvr->StallCltData.StallChgNameSvr.StallName));
	}

	else if(iCmd == SVR_STALL_END)
	{
		pstStallSvr->StallCltData.StallEndSvr.RoleID = pstPlayer->stRoleData.RoleID;
	}
	else if(SVR_STALL_GRID_NUM == iCmd)
	{
		pstStallSvr->StallCltData.StallGridNum = pstPlayer->stRoleData.MiscInfo.StallGridNum;
	}
	else
	{
		return -1;
	}

	Z_CSHEAD_INIT(&stPkg.Head, STALL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int stall_up_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, int iSucc,int iStallType, int iStallGrid)
{
	CSPKG stPkg;
	CSSTALLSVR *pstStallSvr = &stPkg.Body.StallSvr;	
	pstStallSvr->Type = SVR_STAL_UP_RES;
	pstStallSvr->StallCltData.StallUpRes.Succ = iSucc;
	pstStallSvr->StallCltData.StallUpRes.StallType = iStallType;
	pstStallSvr->StallCltData.StallUpRes.StallGridIdx = iStallGrid;
	Z_CSHEAD_INIT(&stPkg.Head, STALL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int stall_down_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, int iSucc,int iStallType, int iStallGrid)
{
	CSPKG stPkg;
	CSSTALLSVR *pstStallSvr = &stPkg.Body.StallSvr;	
	pstStallSvr->Type = SVR_STAL_DOWN_RES;
	pstStallSvr->StallCltData.StallDownRes.Succ = iSucc;
	pstStallSvr->StallCltData.StallDownRes.StallType = iStallType;
	pstStallSvr->StallCltData.StallDownRes.StallGridIdx = iStallGrid;
	Z_CSHEAD_INIT(&stPkg.Head, STALL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int stall_chg_admsg_res(ZONESVRENV* pstEnv, Player *pstPlayer, int iSucc)
{
	CSPKG stPkg;
	CSSTALLSVR *pstStallSvr = &stPkg.Body.StallSvr;	
	pstStallSvr->Type = SVR_STAL_CHG_ADMSG_RES;
	pstStallSvr->StallCltData.StallChgAdMsgRes.iSucc = iSucc;
	Z_CSHEAD_INIT(&stPkg.Head, STALL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int stall_see_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, STALLSEERES *pstStallSee)
{
	CSPKG stPkg;
	CSSTALLSVR *pstStallSvr = &stPkg.Body.StallSvr;

	 
	pstStallSvr->Type = SVR_STALL_SEE;
	memcpy (&pstStallSvr->StallCltData.StallSeeSvr, pstStallSee, sizeof(*pstStallSee));
	
	Z_CSHEAD_INIT(&stPkg.Head, STALL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}


int stall_action(ZONESVRENV* pstEnv, Player *pstPlayer, int iFlag)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstActionRes->ActionID = ACTION_STALL;
	pstActionRes->Data.Stall.Flag = iFlag;
	
	if(iFlag == 0)
	{
		STRNCPY(pstActionRes->Data.Stall.StallName, pstPlayer->stOnline.stStallOnline.szStallName, sizeof(pstActionRes->Data.Stall.StallName));
	}
	else
	{
		STRNCPY(pstActionRes->Data.Stall.StallName, pstPlayer->stRoleData.RoleName, sizeof(pstActionRes->Data.Stall.StallName));
	}
	
	pstActionRes->ID = pstPlayer->iMemID;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);

	pstActionRes->Data.Stall.InView = 1;
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);

	if (iFlag)
	{
		Player *apstPlayers[MAX_PLAYER];
		int iCount = 0;
		int i, iEqu;
		Player *pstPlayerOther = NULL;
		TMEMBLOCK *pstMemBlock;
		
		pstActionRes->Data.Stall.InView = 0;

		for (i=0; i<MAX_PLAYER; i++)
		{
			pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
			if (!pstMemBlock->fValid)
			{
				continue;
			}

			pstPlayerOther = (Player *)pstMemBlock->szData;
			if ( pstPlayerOther == pstPlayer || ((PLAYER_STATUS_ROLE_LOGIN != pstPlayerOther->eStatus)))
						
			{
				continue;
			}

			bsearch_int(&pstPlayerOther->iMemID, &pstPlayer->stOnline.aiVAnimate, 
				pstPlayer->stOnline.iVAnimate, sizeof(int), &iEqu);
			if (iEqu)
			{
				continue;
			}
			
			apstPlayers[iCount++] = pstPlayerOther;

			if (is_player_ride_on_machine(pstPlayerOther))
			{
				Monster *pstMachine;

				pstMachine = machine_get_by_player(pstEnv, pstPlayerOther);
				if (NULL != pstMachine)
				{
					bdelete_int(&pstPlayer->iMemID,  &pstMachine->aiVAnimate, &pstMachine->iVPlayer, sizeof(int));
				}
			}
			else
			{
				bdelete_int(&pstPlayer->iMemID,  &pstPlayerOther->stOnline.aiVAnimate, &pstPlayerOther->stOnline.iVAnimate, sizeof(int));
			}
			
		}

		if (iCount > 0)
		{
			z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers, iCount, &stPkg);
		}
	}

	if(iFlag)
	{
		wedding_dsn_action(pstEnv, pstPlayer);
	}
	return 0;
}


static int get_pack_grid(Player *pstPlayer, STALLITEM* pstItemData, ROLEGRID *pstGrid,int *piPos)
{
	//int iPos;
	ROLEPACKAGE *pstPack = &pstPlayer->stRoleData.Package;

	if (!pstGrid || !pstItemData || !pstPlayer ||!piPos )
	{
		return -1;
	}

	if (pstItemData->ListType == LIST_TYPE_SUNDRIES)
	{
		*piPos = package_grid_pos(pstPack->SundriesPackage.RoleGrids, pstPack->SundriesPackage.Num, pstItemData->GridIdx);
		if (*piPos < 0)
		{	
			return -1;
		}

		memcpy (pstGrid, &pstPack->SundriesPackage.RoleGrids[*piPos], sizeof(ROLEGRID));
		//pstGrid = &pstPack->SundriesPackage.RoleGrids[*piPos];
		
		
	}
	else if(pstItemData->ListType == LIST_TYPE_STUFF)
	{
		*piPos = package_itemgrid_pos(pstPack->StuffPackage.RoleItems, pstPack->StuffPackage.Num, pstItemData->GridIdx);
		if (*piPos < 0)
		{	
			return -1;
		}

		pstGrid->Type = GRID_ITEM_ITEM;
		memcpy(&pstGrid->GridData.RoleItem, &pstPack->StuffPackage.RoleItems[*piPos], sizeof(ROLEITEM));
	}
	else if(pstItemData->ListType == LIST_TYPE_TASK)
	{
		*piPos = package_itemgrid_pos(pstPack->TaskPackage.RoleItems, pstPack->TaskPackage.Num, pstItemData->GridIdx);
		if (*piPos < 0)
		{	
			return -1;
		}

		pstGrid->Type = GRID_ITEM_ITEM;
		memcpy(&pstGrid->GridData.RoleItem, &pstPack->TaskPackage.RoleItems[*piPos], sizeof(ROLEITEM));
	}
	else
	{
		return -1;
	}
	
	return 0;
}


static int get_pack_pet(Player *pstPlayer, STALLPET* pstPetData, ROLEPET* pstRolePet, int *piPos)
{
	int i;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;

	if (!pstPlayer || !pstPetData || !pstRolePet ||!piPos)
	{
		return -1;
	}
	
	for (i=0; i<pstPetPackage->Num; i++)
	{
		if (pstPetPackage->RolePets[i].WID == pstPetData->WID &&
			pstPetPackage->RolePets[i].PetID == pstPetData->DefID )
		{
			memcpy (pstRolePet, &pstPetPackage->RolePets[i], sizeof(ROLEPET));
			*piPos = i;
			return 0;
		}
	}
	return -1;
}


int stall_check_up_item(ZONESVRENV* pstEnv, Player *pstPlayer,STALLSEERES *pstStallSee,STALLITEM *pstItemData,int iCmd)
{
	tdr_ulonglong ullMoney;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	ITEMDEF *pstItemDef;
	int iPos;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	ROLEGRID *pstRoleGrid;
	ROLEITEM *pstRoleItem;
	
	ullMoney = pstItemData->Price;
	if (pstItemData->Num <= 0)
		return -1;
	
	if (pstItemData->Num * ullMoney > 0xFFFFFFFF)
		return -1;

	if (pstItemData->StallGridIdx >= pstMiscInfo->StallGridNum)
		return -1;

	pstItemDef = z_find_itemdef(pstEnv, pstItemData->DefID);
	if (!pstItemDef)
		return -1;
	
	if (pstItemData->ListType == LIST_TYPE_SUNDRIES)
	{
		iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, pstItemData->GridIdx);
		if (iPos < 0)
			return -1;
		
		pstRoleGrid = &pstPak->SundriesPackage.RoleGrids[iPos];
		if (pstRoleGrid->GridData.RoleItem.WID != pstItemData->WID)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "check_stall_data: SVR:WID(%llu) != CLT:WID(%llu), Cmd=%d",
											pstRoleGrid->GridData.RoleItem.WID, pstItemData->WID,iCmd);
			return -1;
		}

		if (package_current_item_check(pstEnv, pstPlayer, pstItemDef, &pstRoleGrid->GridData.RoleItem)  < 0)
			return -1;

		if (pstItemData->Num > pstRoleGrid->GridData.RoleItem.Num)
			return -1;

		if (pstStallSee)
		{
			pstStallSee->ItemEntity[pstStallSee->ItemNum].StallGridIdx = pstItemData->StallGridIdx;
			memcpy (&pstStallSee->ItemEntity[pstStallSee->ItemNum].RoleGrid, pstRoleGrid, sizeof(ROLEGRID));
			pstStallSee->ItemEntity[pstStallSee->ItemNum].Price = pstItemData->Price;
			pstStallSee->ItemEntity[pstStallSee->ItemNum].RoleGrid.GridData.RoleItem.Num = pstItemData->Num;
			pstStallSee->ItemEntity[pstStallSee->ItemNum].GridGID = pstItemData->GirdGID;
			pstStallSee->ItemNum ++;
		}

	}
	else if (pstItemData->ListType == LIST_TYPE_STUFF ||
			pstItemData->ListType == LIST_TYPE_TASK)
	{
		if (pstItemData->ListType == LIST_TYPE_STUFF)
		{
			iPos = package_itemgrid_pos(pstPak->StuffPackage.RoleItems, pstPak->StuffPackage.Num, pstItemData->GridIdx);
			if (iPos < 0)
				return -1;

			pstRoleItem = &pstPak->StuffPackage.RoleItems[iPos];
		}
		else
		{
			iPos = package_itemgrid_pos(pstPak->TaskPackage.RoleItems, pstPak->TaskPackage.Num, pstItemData->GridIdx);
			if (iPos < 0)
				return -1;

			pstRoleItem = &pstPak->TaskPackage.RoleItems[iPos];
		}

		if (package_current_item_check(pstEnv, pstPlayer, pstItemDef, pstRoleItem)  < 0)
			return -1;
		
		if (pstRoleItem->WID != pstItemData->WID)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "check_stall_data: pstItem->WID(%llu) != pstItemData->WID(%llu),Cmd=%d",
						pstRoleItem->WID, pstItemData->WID, iCmd);
			return -1;
		}

		if (pstItemData->Num > pstRoleItem->Num)
			return -1;

		if (pstStallSee)
		{
			pstStallSee->ItemEntity[pstStallSee->ItemNum].StallGridIdx = pstItemData->StallGridIdx;
			pstStallSee->ItemEntity[pstStallSee->ItemNum].RoleGrid.Type = GRID_ITEM_ITEM;
			memcpy (&pstStallSee->ItemEntity[pstStallSee->ItemNum].RoleGrid.GridData.RoleItem, pstRoleItem, sizeof(ROLEITEM));
			pstStallSee->ItemEntity[pstStallSee->ItemNum].Price = pstItemData->Price;
			pstStallSee->ItemEntity[pstStallSee->ItemNum].RoleGrid.GridData.RoleItem.Num = pstItemData->Num;
			pstStallSee->ItemEntity[pstStallSee->ItemNum].GridGID = pstItemData->GirdGID;
			pstStallSee->ItemNum ++;
		}
	}
	else
	{
		return -1;
	}
	return 0;
}

int stall_check_up_pet(ZONESVRENV* pstEnv, Player *pstPlayer,STALLSEERES *pstStallSee,STALLPET *pstPetData)
{
	ROLEPET *pstRolePet;
	
	if (pstPetData->StallGridIdx >= STALL_MAX_PET)
		return -1;
	
	pstRolePet = z_find_role_pet(pstPlayer, pstPetData->WID, NULL);
	if (pstRolePet == NULL)
		return -1;

	if (pstEnv->pstZoneObj->iPetLoveLevelDef &&
		pstRolePet->LoveLevel >= pstEnv->pstZoneObj->iPetLoveLevelDef - 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL3);
		return -1;
	}

	if (pstRolePet->StatFlag & PET_STAT_FIGHT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL4);
		return -1;
	}

	if (pstStallSee)
	{
		pstStallSee->PetEntity[pstStallSee->PetNum].StallGridIdx = pstPetData->StallGridIdx;
		pstStallSee->PetEntity[pstStallSee->PetNum].Price = pstPetData->Price;
		memcpy (&pstStallSee->PetEntity[pstStallSee->PetNum].RolePet, pstRolePet, sizeof(*pstRolePet));
		pstStallSee->PetEntity[pstStallSee->PetNum].GridGID = pstPetData->GirdGID;
		pstStallSee->PetNum ++;
	}
	return 0;
}

int check_stall_data(ZONESVRENV* pstEnv, Player *pstPlayer, STALLSEERES *pstStallSee,int iCmd)
{
	int i,j;
	STALLDATA *pstCSStallData = &pstPlayer->stOnline.stStallOnline.stStallData;
	STALLITEM *pstItemData;
	STALLPET *pstPetData;
	int iRet = 0;

	if (	pstCSStallData->PetNum > STALL_MAX_PET ||
		pstCSStallData->ItemNum > STALL_MAX_ITEM)
	{
		return -1;
	}

	if (pstStallSee)
	{
		memset (pstStallSee, 0, sizeof(*pstStallSee));
		STRNCPY(pstStallSee->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstStallSee->RoleName));
		STRNCPY(pstStallSee->StallName, pstCSStallData->StallName, sizeof(pstStallSee->StallName));
	}

	for (i=0; i<pstCSStallData->ItemNum; i++)
	{
		pstItemData = &pstCSStallData->ItemData[i];
	
		for (j=i+1; j<pstCSStallData->ItemNum; j++)
		{
			if (pstItemData->WID == pstCSStallData->ItemData[j].WID)
				return -1;
		}

		if(pstStallSee)
		{
			iRet = stall_check_up_item(pstEnv, pstPlayer,pstStallSee,pstItemData,iCmd);
		}
		else
		{
			iRet = stall_check_up_item(pstEnv, pstPlayer,NULL,pstItemData,iCmd);
		}
		if ( iRet < 0)
		{
			return -1;
		}
		
	}
	
	for (i=0; i<pstCSStallData->PetNum; i++)
	{
		pstPetData = &pstCSStallData->PetData[i];

		for (j=i+1; j<pstCSStallData->PetNum; j++)
		{
			if (pstPetData->WID == pstCSStallData->PetData[j].WID)
			{
				return -1;
			}
		}

		if(pstStallSee)
		{
			iRet = stall_check_up_pet(pstEnv, pstPlayer,pstStallSee,pstPetData);
		}
		else
		{
			iRet = stall_check_up_pet(pstEnv, pstPlayer,NULL,pstPetData);
		}
		if ( iRet< 0)
		{
			return -1;
		}
	}
	return 0;
}

// 上架
int stall_up(ZONESVRENV* pstEnv, Player *pstPlayer, STALLUPREQ *pstReq)
{
	int i;
	STALLITEM *pstStallItem;                       	
    	STALLPET *pstStallPet; 
	STALLDATA *pstCSStallData = &pstPlayer->stOnline.stStallOnline.stStallData;
	int iStallGrid ;
	STALLSEERES stStallSee;

	memset(&stStallSee,0, sizeof(stStallSee));
	
	if (pstReq->Type == STALL_TYPE_ITEM)
	{
		iStallGrid = pstReq->StallUpData.StallItem.StallGridIdx;
		pstStallItem = &pstReq->StallUpData.StallItem;
		pstStallItem->GirdGID = z_get_gid(pstEnv->pstAppCtx, pstEnv);
		
		if (pstCSStallData->ItemNum >= pstPlayer->stRoleData.MiscInfo.StallGridNum)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL23);
			goto error;
		}
		
		for (i=0; i<pstCSStallData->ItemNum; i++)
		{
			if (pstStallItem->WID == pstCSStallData->ItemData[i].WID)
			{
				goto error;
			}
		}

		if (	pstPlayer->stRoleData.RoleDetail.Money+ 
			stall_count_price(pstEnv, pstPlayer) +
			pstReq->StallUpData.StallItem.Num*pstReq->StallUpData.StallItem.Price > MAX_MONEY)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL24);
			goto error;
		}

		if (stall_check_up_item( pstEnv, pstPlayer,&stStallSee,pstStallItem,CLT_STALL_UP) < 0)
		{
			goto error;
		}

		memcpy(&pstCSStallData->ItemData[(int)pstCSStallData->ItemNum], pstStallItem, sizeof(*pstStallItem));
		pstCSStallData->ItemNum ++;

	}
	else if(pstReq->Type == STALL_TYPE_PET)
	{
		iStallGrid = pstReq->StallUpData.StallPet.StallGridIdx;
		pstStallPet = &pstReq->StallUpData.StallPet;
		pstStallPet->GirdGID = z_get_gid(pstEnv->pstAppCtx, pstEnv);
		
		if (pstCSStallData->PetNum >= STALL_MAX_PET)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL23);
			goto error;
		}
		
		for (i=0; i<pstCSStallData->PetNum; i++)
		{
			if (pstStallPet->WID == pstCSStallData->PetData[i].WID)
			{
				goto error;
			}
		}

		if (	pstPlayer->stRoleData.RoleDetail.Money+ 
			stall_count_price(pstEnv, pstPlayer) +
			pstReq->StallUpData.StallPet.Price > MAX_MONEY)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL24);
			goto error;
		}
		
		if (stall_check_up_pet( pstEnv, pstPlayer,&stStallSee,pstStallPet) < 0)
		{
			goto error;
		}

		memcpy(&pstCSStallData->PetData[(int)pstCSStallData->PetNum], pstStallPet, sizeof(*pstStallPet));
		pstCSStallData->PetNum ++;
	}
	else
	{
		goto error;
	}
	
	stall_up_to_clt(pstEnv, pstPlayer,1,pstReq->Type, iStallGrid);
	z_role_stall_oplog(pstEnv, pstPlayer,&stStallSee,OPLOG_STALL_FLAG_UP);
	return 0;
	
error:
	stall_up_to_clt(pstEnv, pstPlayer,0,pstReq->Type, iStallGrid);
	return -1;
}

// 下架
int stall_down(ZONESVRENV* pstEnv, Player *pstPlayer, STALLDOWNREQ *pstReq)
{
	int i;
	STALLDATA *pstCSStallData = &pstPlayer->stOnline.stStallOnline.stStallData;
	STALLSEERES stStallSee;
	int iPos = -1;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	ROLEPET *pstRolePet;
	
	memset(&stStallSee,0, sizeof(stStallSee));

	if (pstReq->StallType == STALL_TYPE_ITEM)
	{
		for (i=0; i<pstCSStallData->ItemNum; i++)
		{
			if (pstCSStallData->ItemData[i].StallGridIdx == pstReq->StallGridIdx &&
				pstCSStallData->ItemData[i].WID == pstReq->WID)
			{
				break;
			}
		}

		if ( i >= pstCSStallData->ItemNum)
		{
			goto error;
		}

		iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, pstCSStallData->ItemData[i].GridIdx);
		if (iPos < 0)
			goto error;

		stStallSee.ItemNum = 1;
		stStallSee.ItemEntity[0].GridGID = pstCSStallData->ItemData[i].GirdGID;
		stStallSee.ItemEntity[0].Price = pstCSStallData->ItemData[i].Price;
		stStallSee.ItemEntity[0].StallGridIdx = pstCSStallData->ItemData[i].StallGridIdx;
		stStallSee.ItemEntity[0].RoleGrid = pstPak->SundriesPackage.RoleGrids[iPos];

		pstCSStallData->ItemNum --;
		if (i != pstCSStallData->ItemNum)
		{
			memcpy(&pstCSStallData->ItemData[i], &pstCSStallData->ItemData[pstCSStallData->ItemNum],
					sizeof(pstCSStallData->ItemData[0]));
		}
	}
	else if(pstReq->StallType == STALL_TYPE_PET)
	{
		for (i=0; i<pstCSStallData->PetNum; i++)
		{
			if (pstCSStallData->PetData[i].StallGridIdx == pstReq->StallGridIdx &&
				pstCSStallData->PetData[i].WID == pstReq->WID)
			{
				break;
			}
		}

		if (i >= pstCSStallData->PetNum)
		{
			goto error;
		}

		if (pstPlayer->stOnline.stStallOnline.stStallData.PetNum >= STALL_MAX_PET)
		{
			goto error;
		}
	
		pstRolePet = z_find_role_pet(pstPlayer, pstCSStallData->PetData[i].WID, NULL);
		if (pstRolePet == NULL)
			goto error;

		stStallSee.PetNum = 1;
		stStallSee.PetEntity[0].GridGID = pstCSStallData->PetData[i].GirdGID;
		stStallSee.PetEntity[0].Price = pstCSStallData->PetData[i].Price;
		stStallSee.PetEntity[0].StallGridIdx = pstCSStallData->PetData[i].StallGridIdx;
		stStallSee.PetEntity[0].RolePet = *pstRolePet;
		
		pstCSStallData->PetNum --;
		if (i != pstCSStallData->PetNum)
		{
			memcpy(&pstCSStallData->PetData[i], &pstCSStallData->PetData[pstCSStallData->PetNum],
					sizeof(pstCSStallData->PetData[0]));
		}
	}
	else
	{
		goto error;
	}
	
	stall_down_to_clt(pstEnv, pstPlayer, 1,pstReq->StallType, pstReq->StallGridIdx);
	z_role_stall_oplog(pstEnv, pstPlayer,&stStallSee,OPLOG_STALL_FLAG_DOWN);
	return 0;

error:	
	stall_down_to_clt(pstEnv, pstPlayer, 0,pstReq->StallType, pstReq->StallGridIdx);
	return -1;
}

int stall_start(ZONESVRENV* pstEnv, Player *pstPlayer, STALLSTARTREQ *pstStallStartClt)
{
	int i;
	STALLSEERES stStallSee;
	StallOnline *pstStallOnline = &pstPlayer->stOnline.stStallOnline;
//	int iNum = pstStallStartClt->CSStallData.ItemNum + pstStallStartClt->CSStallData.PetNum;
	STALLDATA *pstCSStallData = &pstPlayer->stOnline.stStallOnline.stStallData;

	if (player_check_safelock(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}

	player_trace_act_break_prepare(pstEnv, pstPlayer, BRK_PRE_ACT_STALL);

	if (pstPlayer->stRoleData.Level < MIN_STALL_ROLE_LEVLE)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL5);
		return -1;
	}

	if (pstPlayer->stOnline.stQiecuoInfo.iReqID > 0 ||pstPlayer->stOnline.stQiecuoInfo.iMemID > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL6);
		return -1;
	}

	if (pstPlayer->stOnline.State & CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL7);
		return -1;
	}

	if (pstPlayer->stOnline.State & CS_STAT_STALL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL8);
		return -1;
	}
	
	if (0 == z_player_act_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, RES_ACTION_STALL))
	{
		return -1;
	}

	if ((CheckDirty(pstStallStartClt->CSStallData.StallName , 0x1f) & 0x1f ) )
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_DIRTY);
		return -1;
	}
	if ((CheckDirty(pstStallStartClt->CSStallData.StallAdMsg , 0x0f) & 0x0f ) )
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_DIRTY);
		return -1;
	}
	
	if ( stall_start_check_pos(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}

	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	}

	if (pstPlayer->stOnline.cMoving)
	{
		z_player_stop(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, NULL);
	
	}

	memcpy(&pstPlayer->stOnline.stStallOnline.stStallData, &pstStallStartClt->CSStallData,sizeof(pstStallStartClt->CSStallData));

	// 给每个格子分配一个全局唯一ID
	for (i=0; i<pstCSStallData->ItemNum; i++)
	{
		pstCSStallData->ItemData[i].GirdGID = z_get_gid(pstEnv->pstAppCtx, pstEnv);
	}

	for (i=0; i<pstCSStallData->PetNum; i++)
	{
		pstCSStallData->PetData[i].GirdGID = z_get_gid(pstEnv->pstAppCtx, pstEnv);
	}

	if (pstPlayer->stRoleData.RoleDetail.Money+ stall_count_price(pstEnv, pstPlayer) > MAX_MONEY)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL24);
		return -1;
	}
	
	if (check_stall_data(pstEnv, pstPlayer, &stStallSee,CLT_STALL_START) < 0)
	{
		return -1;
	}
	
	pstStallOnline->tStallActiveTime = 0;
	pstPlayer->stOnline.State |= CS_STAT_STALL;
	pstStallOnline->ullStallWID = z_get_gid(pstEnv->pstAppCtx, pstEnv);
	STRNCPY(pstStallOnline->szStallName, pstStallStartClt->CSStallData.StallName, sizeof(pstStallOnline->szStallName));
 
	stall_action(pstEnv, pstPlayer, 0);
	z_role_stall_oplog(pstEnv, pstPlayer,&stStallSee,OPLOG_STALL_FLAG_START);
	return 0;
}


int stall_chg_name(ZONESVRENV* pstEnv, Player *pstPlayer, STALLCHGNAMEREQ *pstStallChgNameClt)
{
	StallOnline *pstStallOnline = &pstPlayer->stOnline.stStallOnline;


	if (!(pstPlayer->stOnline.State & CS_STAT_STALL))
	{
		return -1;
	}

	if ( (CheckDirty(pstStallChgNameClt->StallName , 0x1f) & 0x1f ) )
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_DIRTY);
		return -1;
	}

	if (strcmp(pstStallOnline->szStallName, pstStallChgNameClt->StallName) == 0)
	{
		return -1;
	}
	
	STRNCPY(pstStallOnline->szStallName, pstStallChgNameClt->StallName, sizeof(pstStallOnline->szStallName));	
	
	stall_action(pstEnv, pstPlayer, 0);

	return 0;
}

int stall_chg_admsg(ZONESVRENV* pstEnv, Player *pstPlayer, STALLCHGNAMEREQ *pstStallChgAdMsgClt)
{
	StallOnline *pstStallOnline = &pstPlayer->stOnline.stStallOnline;


	if (!(pstPlayer->stOnline.State & CS_STAT_STALL))
	{
		goto error;
	}

	if ( (CheckDirty(pstStallChgAdMsgClt->StallName , 0x0f) & 0x0f ) )
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_DIRTY);
		goto error;
	}

	if (strcmp(pstStallOnline->stStallData.StallAdMsg, pstStallChgAdMsgClt->StallName) == 0)
	{
		goto error;
	}
	
	STRNCPY(pstStallOnline->stStallData.StallAdMsg, pstStallChgAdMsgClt->StallName, sizeof(pstStallOnline->stStallData.StallAdMsg));	
	stall_chg_admsg_res(pstEnv, pstPlayer, 1);
	return 0;

error:
	stall_chg_admsg_res(pstEnv, pstPlayer, 0);
	return 0;
}



// 玩家查看摊位
int stall_see(ZONESVRENV* pstEnv, Player *pstPlayer, STALLSEEREQ *pstStallSeeClt)
{
	Player *pstStallOwner;
	STALLSEERES stStallSee;


	if (pstPlayer->stOnline.State & CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL9);
		return -1;
	}

	/*
	if (pstPlayer->stOnline.State & CS_STAT_FIGHT)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "战斗状态下无法查看摊位");
		return -1;
	}
	*/

	pstStallOwner = z_id_player(pstEnv->pstAppCtx,pstEnv,  pstStallSeeClt->MemID);
	if (!pstStallOwner || !(pstStallOwner->stOnline.State & CS_STAT_STALL) ||PLAYER_STATUS_ROLE_LOGIN != pstStallOwner->eStatus)
	{
		return -1;
	}

	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	}

	if (0 > z_check_op_dist(pstEnv, pstPlayer, pstStallOwner->stRoleData.Map, &pstStallOwner->stRoleData.Pos, MAX_STALL_SEE_DIST+100))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL10);
		return -1;
	}

	if (check_stall_data(pstEnv, pstStallOwner, &stStallSee,CLT_STALL_SEE)< 0 )
	{
		return -1;
	}

	// 保存打开摊位的WID
	pstPlayer->stOnline.stStallOnline.ullStallOtherWID = pstStallOwner->stOnline.stStallOnline.ullStallWID;
	
	STRNCPY(stStallSee.RoleName, pstStallOwner->stRoleData.RoleName, sizeof(stStallSee.RoleName));
	STRNCPY(stStallSee.StallName, pstStallOwner->stOnline.stStallOnline.szStallName, sizeof(stStallSee.StallName));

	stall_see_to_clt(pstEnv, pstPlayer, &stStallSee);
	
	return 0;
}

STALLITEM* stall_get_onlineinfo_by_item(Player *pstPlayer, STALLBUYREQ *pstStallBuyClt)
{
	STALLITEM *pstItemData;
	int i;
	StallOnline *pstStallOnline = &pstPlayer->stOnline.stStallOnline;

	if (pstStallBuyClt->StallType == STALL_TYPE_PET)
	{
		return NULL;
	}

	for (i=0; i<pstStallOnline->stStallData.ItemNum; i++)
	{
		pstItemData = &pstStallOnline->stStallData.ItemData[i];
	
		if (	pstItemData->WID == pstStallBuyClt->WID &&
			pstItemData->StallGridIdx == pstStallBuyClt->StallGridIdx )
		{
			return pstItemData;
		}
	}
	return NULL;
}



STALLPET* stall_get_onlineinfo_by_pet(Player *pstPlayer, STALLBUYREQ *pstStallBuyClt)
{
	STALLPET *pstPetData;
	int i;
	StallOnline *pstStallOnline = &pstPlayer->stOnline.stStallOnline;

	if (pstStallBuyClt->StallType == STALL_TYPE_ITEM)
	{
		return NULL;
	}
	
	for (i=0; i<pstStallOnline->stStallData.PetNum; i++)
	{
		pstPetData = &pstStallOnline->stStallData.PetData[i];

		if (pstPetData->StallGridIdx == pstStallBuyClt->StallGridIdx &&
			pstPetData->WID == pstStallBuyClt->WID)
		{
			return pstPetData;
		}
	}
	return NULL;
}


int stall_update_online(Player *pstPlayer, tdr_ulonglong WID, int iNum, int iStallType)
{
	int i;
	StallOnline *pstStallOnline = &pstPlayer->stOnline.stStallOnline;
	STALLITEM *pstStallItem;
	STALLPET   *pstStallPet;

	if (iStallType == STALL_TYPE_ITEM)		// item
	{
		for(i=pstStallOnline->stStallData.ItemNum-1; i>=0; i--)
		{
			pstStallItem = &pstStallOnline->stStallData.ItemData[i];
			if (pstStallItem->WID == WID )
			{
				if (iNum > 0)
				{
					pstStallItem->Num = iNum;
				}
				else
				{
					pstStallOnline->stStallData.ItemNum --;
					if (pstStallOnline->stStallData.ItemNum != i)
					{
						int iIdx = pstStallOnline->stStallData.ItemNum;
						memcpy(pstStallItem, &pstStallOnline->stStallData.ItemData[iIdx], sizeof(*pstStallItem));
					}
				}
				
				break;
			}
		}

		if (i<0)
		{
			return -1;
		}
	}
	else if(iStallType == STALL_TYPE_PET) 	// pet
	{
		for (i=pstStallOnline->stStallData.PetNum-1; i>=0; i--)
		{
			pstStallPet = &pstStallOnline->stStallData.PetData[i];
			if (pstStallPet->WID == WID)
			{
				pstStallOnline->stStallData.PetNum --;
				if (pstStallOnline->stStallData.PetNum != i)
				{
					int iIdx = pstStallOnline->stStallData.PetNum;
					memcpy(pstStallPet, &pstStallOnline->stStallData.PetData[iIdx], sizeof(*pstStallPet));
				}
				break;
			}
		}

		if (i<0)
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}

	return 0;
}

int stall_buy_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, STALLINFOCHG *pstStallInfoChg)
{
	CSPKG stPkg;
	CSSTALLSVR *pstStallSvr = &stPkg.Body.StallSvr;

	pstStallSvr->Type = SVR_STALL_BUY;
	memcpy (&pstStallSvr->StallCltData.StallBuySvr.StallInfoBuy, pstStallInfoChg, sizeof(*pstStallInfoChg));
	
	Z_CSHEAD_INIT(&stPkg.Head, STALL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);	

	return 0;
}

tdr_ulonglong stall_count_price(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	tdr_ulonglong ullPrice = 0;
	int i;
	STALLDATA *pstCSStallData = &pstPlayer->stOnline.stStallOnline.stStallData;

	for (i=0; i<pstCSStallData->ItemNum; i++)
	{
		ullPrice += pstCSStallData->ItemData[i].Price* pstCSStallData->ItemData[i].Num;
	}

	for (i=0; i<pstCSStallData->PetNum; i++)
	{
		ullPrice += pstCSStallData->PetData[i].Price;
	}
	UNUSED(pstEnv);
	return ullPrice;
}


int stall_buy(ZONESVRENV* pstEnv, Player *pstPlayer, STALLBUYREQ *pstStallBuyClt)
{
	Player *pstStallOwner;
	ROLEPACKAGE *pstMyPack = &pstPlayer->stRoleData.Package;
	STALLITEM *pstItemData;
	STALLPET *pstPetData;
	ROLEGRID stGrid;
	ROLEPET stRolePet;
	tdr_ulonglong iMoney = 0, iDecMoney;
	int iPos;
	CSPKG stPkg;
	CSSTALLSVR *pstStallSvr = &stPkg.Body.StallSvr;
	int iNum;
	int iListType;

	if (pstStallBuyClt->Num <= 0)
	{
		return -1;
	}

	pstStallOwner = z_name_player(pstEnv->pstAppCtx, pstEnv, pstStallBuyClt->RoleName);
	if (!pstStallOwner )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL11);
		return -1;
	}

	if (!(pstStallOwner->stOnline.State & CS_STAT_STALL))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL12);
		return -1;
	}

	if (player_check_safelock(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}

	if (	pstStallOwner->eStatus != PLAYER_STATUS_ROLE_LOGIN ||
		pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		return -1;
	}

	if (check_stall_data(pstEnv, pstStallOwner, NULL,CLT_STALL_BUY) < 0)
	{
		return -1;
	}

	if (pstPlayer->stOnline.stStallOnline.ullStallOtherWID != pstStallOwner->stOnline.stStallOnline.ullStallWID)
	{
		return -1;
	}

	memset(pstStallSvr, 0, sizeof(*pstStallSvr));
	
	if (pstStallBuyClt->StallType == STALL_TYPE_ITEM)			// item
	{
		pstItemData = stall_get_onlineinfo_by_item(pstStallOwner, pstStallBuyClt);
		if (!pstItemData)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL13);
			return -1;
		}

		if (pstStallBuyClt->GirdGID != pstItemData->GirdGID)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL14);
			return -1;
		}

		iListType = pstItemData->ListType;
		iMoney = pstItemData->Price;
		iMoney *= pstStallBuyClt->Num;
		if (iMoney > 0xFFFFFFFF)
		{
			return -1;
		}

		if (get_pack_grid(pstStallOwner, pstItemData, &stGrid, &iPos) < 0)
		{
			return -1;
		}

		stGrid.GridData.RoleItem.Num = pstStallBuyClt->Num;

		if (pstItemData->Num < (int)pstStallBuyClt->Num)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL15);
			return -1;
		}
		
		if (stGrid.GridData.RoleItem.WID != pstStallBuyClt->WID)
		{
			return -1;
		}

		if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, iMoney))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOMONEY);
			return -1;
		}

		if (package_add_money_test(pstEnv, pstStallOwner, ATTR_ID_MONEY, iMoney) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_STALL16, MONEY_TYPE_MONEY_NAME);
			return -1;
		}

		if (package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stGrid, 1,0) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
			return -1;
		}

		// 摊主更新在线信息
		iNum = pstItemData->Num;
		if (stall_update_online(pstStallOwner, pstStallBuyClt->WID, pstItemData->Num -pstStallBuyClt->Num, STALL_TYPE_ITEM) < 0)
		{
			return -1;
		}

		
		package_dec(pstEnv->pstAppCtx, pstEnv, pstStallOwner, stGrid.GridData.RoleItem.DefID, pstStallBuyClt->Num, 
			iListType, &iPos,PACKAGE_DEC_NONE, 0);

		iDecMoney = iMoney*STALL_TAX_RATE/100.0;
		package_add_money(pstEnv, pstStallOwner, ATTR_ID_MONEY, iMoney-iDecMoney);

		z_role_stall_buy_oplog(pstEnv, pstPlayer, pstStallOwner, &stGrid, NULL, STALL_TYPE_ITEM,iMoney);
		z_money_consume_oplog(pstEnv, pstStallOwner, MONEY_OPTYPE_STALL_TEX, 0, 0, iDecMoney, "摆摊交易税");

		/*if (stGrid.Type == GRID_ITEM_ARM)
		{
			 stGrid.GridData.RoleArm.KeYinAddAttr.AddAttrID = 0;
			 stGrid.GridData.RoleArm.KeYinAddAttr.Val = 0;
			 stGrid.GridData.RoleArm.KeYinEnd = 0;
		}*/
		package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stGrid, 1,0,0);
		package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, iMoney);

		pstStallSvr->StallCltData.StallInfoChg.Num = iNum -pstStallBuyClt->Num;
		pstStallSvr->StallCltData.StallInfoChg.WID = pstStallBuyClt->WID;
		pstStallSvr->StallCltData.StallInfoChg.StallType = STALL_TYPE_ITEM;
		pstStallSvr->StallCltData.StallInfoChg.StallGridIdx = pstStallBuyClt->StallGridIdx;

	}
	else if (pstStallBuyClt->StallType == STALL_TYPE_PET)		
	{
		PETDEF* pstPetDef = NULL;
		pstPetData = stall_get_onlineinfo_by_pet(pstStallOwner, pstStallBuyClt);
		if (!pstPetData)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL17);
			return -1;
		}

		iMoney = pstPetData->Price;

		if (pstPetData->GirdGID != pstStallBuyClt->GirdGID)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL18);
			return -1;
		}

		iListType = pstPetData->ListType;
		if(pstMyPack->PetPackage.MaxNum- pstMyPack->PetPackage.Num <= 0 )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL19);
			return -1;
		}
		
		if (get_pack_pet(pstStallOwner, pstPetData, &stRolePet,  &iPos) <0)
		{
			return -1;
		}

		pstPetDef = z_find_pet_def(pstEnv,stRolePet.PetID,NULL);

		if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, iMoney))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOMONEY);
			return -1;
		}

		if (package_add_money_test(pstEnv, pstStallOwner, ATTR_ID_MONEY, iMoney) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL16, MONEY_TYPE_MONEY_NAME);
			return -1;
		}

		// 摊主更新在线信息
		if (stall_update_online(pstStallOwner, pstStallBuyClt->WID, 0, STALL_TYPE_PET) < 0)
		{
			return -1;
		}

		//删宠物
		pet_del(pstEnv, pstStallOwner, stRolePet.WID, 0, NULL);

		iDecMoney = iMoney*STALL_TAX_RATE/100.0;
		package_add_money(pstEnv, pstStallOwner, ATTR_ID_MONEY, iMoney-iDecMoney);
		z_money_consume_oplog(pstEnv, pstStallOwner, MONEY_OPTYPE_STALL_TEX, 0, 0, iDecMoney, "摆摊交易税");

		if (pstPetDef)
		{
			pet_love_level_up_in(pstEnv,pstPlayer,&stRolePet,1, pstPetDef,0);
		}
		
		//加宠物
		memcpy(&pstMyPack->PetPackage.RolePets[pstMyPack->PetPackage.Num], &stRolePet, sizeof(ROLEPET));
		pstMyPack->PetPackage.Num++;
		pet_update_to_clt(pstEnv, pstPlayer,  &stRolePet);
		
		package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, iMoney);

		pstStallSvr->StallCltData.StallInfoChg.Num = 0;
		pstStallSvr->StallCltData.StallInfoChg.WID = pstStallBuyClt->WID;
		pstStallSvr->StallCltData.StallInfoChg.StallType = STALL_TYPE_PET;
		pstStallSvr->StallCltData.StallInfoChg.StallGridIdx = pstStallBuyClt->StallGridIdx;

		z_role_stall_buy_oplog(pstEnv, pstPlayer, pstStallOwner, NULL, &stRolePet, STALL_TYPE_PET ,iMoney);

			//成长之路
		{
			ACHIEVEMENTCOND stCond;
			
		
			memset(&stCond,0,sizeof(stCond));
			
			stCond.Type = COND_PET_GET;
			stCond.Val1 = 0;
		
			z_player_achievement_cond(pstEnv,pstPlayer,&stCond);

			if (pstPetDef && pstPetDef->Type == PET_TYPE_ABERRANCE)
			{
				stCond.Val1 = 1;
		
				z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
			}

			if (stRolePet.PetGenius == PET_GENIUS_S)
			{
				stCond.Val1 = 2;
		
				z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
			}
			
			
		}

	}
	else
	{
		return -1;
	}

	strncpy(pstStallSvr->StallCltData.StallInfoChg.BuyName, pstPlayer->stRoleData.RoleName, 
			sizeof(pstStallSvr->StallCltData.StallInfoChg.BuyName));

	// 通知购买者更新摊位界面
	stall_buy_to_clt(pstEnv, pstPlayer, &pstStallSvr->StallCltData.StallInfoChg);

	// 通知摊主更新摊位界面
	pstStallSvr->Type = SVR_STALL_INFO_CHG;
	Z_CSHEAD_INIT(&stPkg.Head, STALL_SVR);
	z_cltmsg_send(pstEnv, pstStallOwner, &stPkg);

	return 0;
}


int stall_end(ZONESVRENV* pstEnv, Player *pstPlayer, STALLENDREQ *pstStallEndClt)
{
	StallOnline *pstStallOnline = &pstPlayer->stOnline.stStallOnline;
	UNUSED(pstStallEndClt);

	pstPlayer->stOnline.State &= ~CS_STAT_STALL;

	pstStallOnline->ullStallWID = 0;
	pstStallOnline->szStallName[0] = 0;
	memset (&pstStallOnline->stStallData, 0, sizeof(pstStallOnline->stStallData));

	stall_action(pstEnv, pstPlayer, 1);
	z_role_stall_oplog(pstEnv, pstPlayer,NULL,OPLOG_STALL_FLAG_END);
	return 0;
}

int stall_add_grid(ZONESVRENV* pstEnv, Player *pstPlayer, STALLADDGRIDREQ *pstStallAddGridClt)
{
	
	ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;
	UNUSED(pstStallAddGridClt);

	if (pstPlayer->stOnline.State & CS_STAT_STALL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL20);
		return -1;
	}

	if (pstMisc->StallGridNum >= STALL_MAX_ITEM)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL21);
		return -1;
	}

	if (package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, STALL_GRID_ADD_ITEMID, 1, -1, NULL,PACKAGE_DEC_NONE) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STALL22);
		return -1;
	}

	pstMisc->StallGridNum = STALL_MAX_ITEM;

	package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, STALL_GRID_ADD_ITEMID, 1, -1, NULL,PACKAGE_DEC_NONE, 0);

	stall_msg_to_clt(pstEnv, pstPlayer, SVR_STALL_GRID_NUM);
	return 0;
}

int player_stall(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSSTALLCLT *pstStallClt =  &pstCsPkg->Body.StallClt;
	UNUSED(pstAppCtx);
	UNUSED(pstFrameHead);

	if( pstEnv->iInstMode != INST_MODE_ZONE_SVR )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
		return 0;
	}

	 switch(pstStallClt->Type)
	 {
	case CLT_STALL_START:
		return stall_start(pstEnv, pstPlayer, &pstStallClt->StallCltData.StallStartClt);
		break;
		
	case CLT_STALL_END:
		return stall_end(pstEnv, pstPlayer, &pstStallClt->StallCltData.StallEndClt);
		break;
		
	case CLT_STALL_CHANGE_NAME:
		return stall_chg_name(pstEnv, pstPlayer, &pstStallClt->StallCltData.StallChgNameClt);
		break;

	case CLT_STALL_SEE:
		return stall_see(pstEnv, pstPlayer,  &pstStallClt->StallCltData.StallSeeClt);
		break;

	case CLT_STALL_BUY:
		return stall_buy(pstEnv, pstPlayer, &pstStallClt->StallCltData.StallBuyClt);
		break;

	case CLT_STALL_ADD_GRID:
		return stall_add_grid(pstEnv, pstPlayer, &pstStallClt->StallCltData.StallAddGridClt);
		break;
	case CLT_STALL_CHANGE_ADMSG:
		return stall_chg_admsg(pstEnv, pstPlayer, &pstStallClt->StallCltData.StallChgAdMsgClt);
		break;	
	case CLT_STALL_UP:
		return stall_up(pstEnv, pstPlayer, &pstStallClt->StallCltData.StallUpClt);
		break;

	case CLT_STALL_DOWN:
		return stall_down(pstEnv, pstPlayer, &pstStallClt->StallCltData.StallDownClt);
		break;
	default:
		return -1;
		break;
	 }

	return 0;
}

int player_stall_clear( ZONESVRENV* pstEnv, Player *pstPlayer )
{
	StallOnline *pstStallOnline = &pstPlayer->stOnline.stStallOnline;

	pstPlayer->stOnline.State &= ~CS_STAT_STALL;

	pstStallOnline->ullStallWID = 0;
	pstStallOnline->szStallName[0] = 0;
	memset (&pstStallOnline->stStallData, 0, sizeof(pstStallOnline->stStallData));
	
	return stall_action( pstEnv, pstPlayer, 1 );
}

int z_active_player_stall(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSSTALLSVR *pstStallSvr = &stPkg.Body.StallSvr;
	StallOnline *pstStallOnline = &pstPlayer->stOnline.stStallOnline;
		
	if ( !(pstPlayer->stOnline.State & CS_STAT_STALL)  || 
		((pstStallOnline->stStallData.ItemNum ==0)  && (pstStallOnline->stStallData.PetNum == 0)) )
	{
		return 0;
	}

	if (pstAppCtx->stCurr.tv_sec - pstStallOnline->tStallActiveTime < STALL_AD_ACTIVE)
	{
		return 0;
	}

	pstStallOnline->tStallActiveTime = pstAppCtx->stCurr.tv_sec;

       
	if ( strlen(pstStallOnline->stStallData.StallAdMsg) == 0)
	{
		return 0;
	}
	// 下发用户广告时间

	pstStallSvr->Type = SVR_STAL_SHOW_ADMSG;
	pstStallSvr->StallCltData.StallShowAdMsgSvr.ShowTime = STALL_SHOW_ADMSG_TIME;
	memcpy(pstStallSvr->StallCltData.StallShowAdMsgSvr.StallShowAdMsg,
			pstStallOnline->stStallData.StallAdMsg,STALL_MAX_NAME_LEN);
	pstStallSvr->StallCltData.StallShowAdMsgSvr.WID = pstPlayer->iMemID;
	
	Z_CSHEAD_INIT(&stPkg.Head, STALL_SVR);
	//z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	return 0;
}
