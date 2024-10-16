//#include "zone_svr.h"
#include "zone_exchg.h"
#include "zone_clt.h"
#include "zone_package.h"
#include "zone_pet.h"
#include "zone_player.h"
#include "zone_move.h"
#include "zone_relation.h"
#include "zone_task.h"
#include "zone_act_status.h"
#include "zone_oplog.h"
#include "zone_safelock.h"
#include "zone_range.h"
#include "zone_span.h"
#include "zone_err.h"

/*
	<macrosgroup name="EXCHG_TYPE">
		<macro name="INVITE" value="0" desc="邀请" />
		<macro name="REJECT" value="1" desc="拒绝" />
		<macro name="ACCEPT" value="2" desc="接受" />
		<macro name="CANCEL" value="3" desc="取消" />
		<macro name="LOCK" value="4" desc="交易锁定" />
		<macro name="DEAL" value="5" desc="交易确认" />
		<macro name="START" value="6" desc="开始交易" />
		<macro name="END" value="7" desc="结束交易" />
	</macrosgroup>
*/


static int exchg_msg_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstOther, int iCmd)
{
	CSPKG stPkg;
	CSEXCHGSVR *pstExchgSvr = &stPkg.Body.ExchgSvr;

	pstExchgSvr->type = iCmd;
	strncpy(pstExchgSvr->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstExchgSvr->RoleName));

	pstExchgSvr->ExchgEntiy.Money = 0;
	pstExchgSvr->ExchgEntiy.ItemNum = 0;
	pstExchgSvr->ExchgEntiy.PetNum = 0;

	Z_CSHEAD_INIT(&stPkg.Head, EXCHG_SVR);
	z_cltmsg_send(pstEnv, pstOther, &stPkg);

	return 0;
}


static int exchg_lock_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstOther, EXCHGENTIY *pstExchgEntiy)
{
	CSPKG stPkg;
	CSEXCHGSVR *pstExchgSvr = &stPkg.Body.ExchgSvr;

	pstExchgSvr->type = LOCK;
	strncpy(pstExchgSvr->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstExchgSvr->RoleName));
	memcpy(&pstExchgSvr->ExchgEntiy, pstExchgEntiy, sizeof(pstExchgSvr->ExchgEntiy));

	Z_CSHEAD_INIT(&stPkg.Head, EXCHG_SVR);
	z_cltmsg_send(pstEnv, pstOther, &stPkg);
	return 0;
}


static int exchg_data_check(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstOther, EXCHGENTIY *pstExchgEntiy)
{
	int i,j;
	EXCHGDATA *pstExchgData = &pstPlayer->stOnline.stExchgOnline.stExchgData;
	ITEMDEF *pstItemDef;
	ROLEPET *pstRolePet;
	EXCHGITEM *pstExchgItem;
	EXCHGPET *pstPetData;
	ROLEGRID *pstGrid;
	ROLEITEM *pstItem;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	ROLEPACKAGE *pstPakOther = &pstOther->stRoleData.Package;
	ROLEBUDDYMEMBER *pstRoleBuddyMember = NULL;	
	ROLEBUDDY *pstRoleBuddy = &pstPlayer->stRoleData.Buddy;
	int iPos;

	pstRoleBuddyMember = relation_buddy_online_find(pstRoleBuddy,  pstOther->stRoleData.RoleID);
	
	if (pstExchgData->Money < 0)
	{
		return -1;
	}
	
	if (pstExchgData->Money > 0)
	{
		if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, pstExchgData->Money))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG29, MONEY_TYPE_MONEY_NAME);
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE2, ZONEERR_EXG30, MONEY_TYPE_MONEY_NAME);
			return -1;
		}

		if (package_add_money_test(pstEnv, pstOther, ATTR_ID_MONEY, pstExchgData->Money) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG31, MONEY_TYPE_MONEY_NAME);
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE2, ZONEERR_EXG32, MONEY_TYPE_MONEY_NAME);
			return -1;
		}
	}
	
	memset (pstExchgEntiy, 0, sizeof(*pstExchgEntiy));

	pstExchgEntiy->Money = pstExchgData->Money;

	if(pstExchgData->ItemNum> EXCHG_MAX_ITEM_NUM ||
		pstExchgData->PetNum > EXCHG_MAX_PET_NUM)
	{
		return -1;
	}

	for (i=0; i<pstExchgData->ItemNum; i++)
	{
		pstExchgItem = &pstExchgData->ItemData[i];

		for (j=i+1; j<pstExchgData->ItemNum; j++)
		{
			if (pstExchgItem->WID == pstExchgData->ItemData[j].WID || 
				(pstExchgItem->ListType == pstExchgData->ItemData[j].ListType && pstExchgItem->GridIdx == pstExchgData->ItemData[j].GridIdx)
				|| pstExchgItem->ExchgGridIdx == pstExchgData->ItemData[j].ExchgGridIdx)
			{
				return -1;
			}
		}
		
		if (pstExchgItem->Num <= 0)
		{
			return -1;
		}

		if (pstExchgItem->ExchgGridIdx >= EXCHG_MAX_ITEM_NUM)
		{
			return -1;
		}

		pstItemDef = z_find_itemdef(pstEnv, pstExchgItem->DefID);
		if (!pstItemDef )
		{
			return -1;
		}

		if (pstExchgItem->ListType == LIST_TYPE_SUNDRIES)
		{
			iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, pstExchgItem->GridIdx);
			if (iPos < 0)
			{	
				return -1;
			}
			
			pstGrid = &pstPak->SundriesPackage.RoleGrids[iPos];
			if (pstGrid->GridData.RoleItem.WID != pstExchgItem->WID)
			{
				return -1;
			}

			if ( 0 > package_current_item_check(pstEnv, pstPlayer, pstItemDef, &pstGrid->GridData.RoleItem))
			{
				return -1;
			}

			if (pstExchgItem->Num > pstGrid->GridData.RoleItem.Num)
			{
				return -1;
			}

			pstExchgEntiy->ItemEntiy[pstExchgEntiy->ItemNum].ExchgGridIdx = pstExchgItem->ExchgGridIdx;
			memcpy(&pstExchgEntiy->ItemEntiy[pstExchgEntiy->ItemNum].RoleGrid, pstGrid,sizeof(ROLEGRID));
			pstExchgEntiy->ItemEntiy[pstExchgEntiy->ItemNum].RoleGrid.GridData.RoleItem.Num = pstExchgItem->Num;
			pstExchgEntiy->ItemNum ++;

		}
		else if (pstExchgItem->ListType == LIST_TYPE_STUFF ||
				pstExchgItem->ListType == LIST_TYPE_TASK)
		{
			if (pstExchgItem->ListType == LIST_TYPE_STUFF)
			{
				iPos = package_itemgrid_pos(pstPak->StuffPackage.RoleItems, pstPak->StuffPackage.Num, pstExchgItem->GridIdx);
				if (iPos < 0)
				{	
					return -1;
				}

				pstItem = &pstPak->StuffPackage.RoleItems[iPos];
			}
			else if(pstExchgItem->ListType == LIST_TYPE_TASK)
			{
				iPos = package_itemgrid_pos(pstPak->TaskPackage.RoleItems, pstPak->TaskPackage.Num, pstExchgItem->GridIdx);
				if (iPos < 0)
				{	
					return -1;
				}

				pstItem = &pstPak->TaskPackage.RoleItems[iPos];
			}
			else
			{
				return -1;
			}
			
			if (pstItem->WID != pstExchgItem->WID)
			{
				return -1;
			}

			if ( 0 > package_current_item_check(pstEnv, pstPlayer, pstItemDef, pstItem))
			{
				return -1;
			}

			if (pstExchgItem->Num > pstItem->Num)
			{
				return -1;
			}
			
			pstExchgEntiy->ItemEntiy[pstExchgEntiy->ItemNum].ExchgGridIdx = pstExchgItem->ExchgGridIdx;
			pstExchgEntiy->ItemEntiy[pstExchgEntiy->ItemNum].RoleGrid.Type = GRID_ITEM_ITEM;
			memcpy(&pstExchgEntiy->ItemEntiy[pstExchgEntiy->ItemNum].RoleGrid.GridData.RoleItem, pstItem, sizeof(ROLEITEM));
			pstExchgEntiy->ItemEntiy[pstExchgEntiy->ItemNum].RoleGrid.GridData.RoleItem.Num = pstExchgItem->Num;
			pstExchgEntiy->ItemNum ++;
		}
		
		else
		{
			return -1;
		}
	}

	
	for (i=0; i<pstExchgData->PetNum; i++)
	{
		pstPetData = &pstExchgData->PetData[i];

		for (j=i+1; j<pstExchgData->PetNum; j++)
		{
			if (pstPetData->WID == pstExchgData->PetData[j].WID || 
				pstPetData->ExchgGridIdx == pstExchgData->PetData[j].ExchgGridIdx)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "exchg pet wid err  rolename=%s wid=%llu", 
					pstPlayer->stRoleData.RoleName, pstPetData->WID);
				return -1;
			}
		}

		for (j=0; j<pstPakOther->PetPackage.Num; j++)
		{
			if (pstPakOther->PetPackage.RolePets[j].WID == pstPetData->WID)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "exchg pet wid err  rolename=%s wid=%llu  -> rolename=%s", 
					pstPlayer->stRoleData.RoleName, pstPetData->WID, pstOther->stRoleData.RoleName);
				return -1;
			}
		}

		if (pstPetData->ExchgGridIdx >= EXCHG_MAX_PET_NUM)
		{
			return -1;
		}
		
		for (j=0; j<pstPak->PetPackage.Num; j++)
		{
			if (pstPak->PetPackage.RolePets[j].WID == pstPetData->WID)
			{
				break;
			}
		}

		if (j == pstPak->PetPackage.Num)
		{
			return -1;
		}
		pstRolePet = &pstPak->PetPackage.RolePets[j];

		if (pstEnv->pstZoneObj->iPetLoveLevelDef &&
			pstRolePet->LoveLevel >= pstEnv->pstZoneObj->iPetLoveLevelDef - 1)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG1);
			return -1;
		}

		if (pstRolePet->StatFlag & PET_STAT_FIGHT)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG2);
			return -1;
		}

		pstExchgEntiy->PetEntiy[pstExchgEntiy->PetNum].ExchgGridIdx = pstPetData->ExchgGridIdx;
		pstExchgEntiy->PetEntiy[pstExchgEntiy->PetNum].PetType = LIST_TYPE_SHI;
		memcpy(&pstExchgEntiy->PetEntiy[pstExchgEntiy->PetNum].RolePet, pstRolePet, sizeof(*pstRolePet));
		pstExchgEntiy->PetNum ++;
	}	
	
	return 0;
}

static int exchg_pack_dec_add_test(ZONESVRENV* pstEnv, Player *pstPlayer, EXCHGENTIY *pstExchgEntiy)
{
	ROLEPACKAGE stPak;
	int i,j;
	ExchgOnline *pstExchgOnline = &pstPlayer->stOnline.stExchgOnline;
	EXCHGITEM *pstExchgItem;
	EXCHGPET *pstExchgPet;
	ROLEPET *pstRolePet;
	int iPos = -1;
	ROLEPACKAGECHG stRolePakChg;
	unsigned short  iNum;
	ITEMDEF *pstItemDef;

	Player *pstOther;
	int iMemID = pstPlayer->stOnline.stExchgOnline.iMemID;

	if (iMemID <= 0)
	{
		return -1;
	}

	pstOther = z_id_player(pstEnv->pstAppCtx, pstEnv, iMemID);
	if (!pstOther)
	{
		return -1;
	}

	memcpy(&stPak,  &pstPlayer->stRoleData.Package, sizeof(ROLEPACKAGE));
	stRolePakChg.ItemChg.ChgNum = 0;
	stRolePakChg.ArmChg.ChgNum = 0;
	
	for (i=0; i<pstExchgOnline->stExchgData.ItemNum; i++)
	{	
		pstExchgItem = &pstExchgOnline->stExchgData.ItemData[i];
		iNum = pstExchgItem->Num;
		if (pstExchgItem->ListType == LIST_TYPE_SUNDRIES)
		{
			iPos = -1;
			for (j=stPak.SundriesPackage.Num -1; j>=0; j--)
			{
				if (pstExchgItem->DefID == stPak.SundriesPackage.RoleGrids[j].GridData.RoleItem.DefID &&
					pstExchgItem->WID == stPak.SundriesPackage.RoleGrids[j].GridData.RoleItem.WID &&
					pstExchgItem->GridIdx == stPak.SundriesPackage.RoleGrids[j].GridData.RoleItem.GridIdx)
				{
					iPos = j;
					break;
				}
			}

			if (iPos < 0)
			{
				return -1;
			}

			if (stPak.SundriesPackage.RoleGrids[j].GridData.RoleItem.Num < iNum)
			{
				return -1;
			}

			if (package_dec_in_grid(pstExchgItem->DefID, stPak.SundriesPackage.RoleGrids, &stPak.SundriesPackage.Num,
                                LIST_TYPE_SUNDRIES, &iNum, &iPos, &stRolePakChg,PACKAGE_DEC_NONE)< 0)
            {
                    return -1;
            }

		}
		else if(pstExchgItem->ListType == LIST_TYPE_TASK)
		{
			iPos = -1;
			for (j=stPak.TaskPackage.Num-1; j>=0; j--)
			{
				if (pstExchgItem->DefID == stPak.TaskPackage.RoleItems[j].DefID &&
					pstExchgItem->WID == stPak.TaskPackage.RoleItems[j].WID &&
					pstExchgItem->GridIdx == stPak.TaskPackage.RoleItems[j].GridIdx)
				{
					iPos = j;
					break;
				}
			}

			if (iPos < 0)
			{
				return -1;
			}

			if (stPak.TaskPackage.RoleItems[j].Num < iNum)
			{
				return -1;
			}

			if (package_dec_in_item(pstExchgItem->DefID, stPak.TaskPackage.RoleItems, &stPak.TaskPackage.Num,
                                  LIST_TYPE_TASK, &iNum, &iPos, &stRolePakChg,PACKAGE_DEC_NONE)< 0)
            {
                    return -1;
            }

		}
		else
		{
			return -1;
		}

	}

	
	for (i=0; i<pstExchgOnline->stExchgData.PetNum; i++)
	{
		pstExchgPet = &pstExchgOnline->stExchgData.PetData[i];
		
		for (j=stPak.PetPackage.Num-1;j>=0; j--)
		{
			if (stPak.PetPackage.RolePets[j].PetID == pstExchgPet->DefID &&
				stPak.PetPackage.RolePets[j].WID == pstExchgPet->WID )
			{
				break;
			}
		}

		if (j < 0)
			return -1;
		
		pstRolePet = &stPak.PetPackage.RolePets[j];

            	stPak.PetPackage.Num--;
            	if ( j != stPak.PetPackage.Num)
            	{
                    memcpy(pstRolePet, &stPak.PetPackage.RolePets[stPak.PetPackage.Num], sizeof(ROLEPET));
            	}
	}

	stRolePakChg.ItemChg.ChgNum = 0;
	stRolePakChg.ArmChg.ChgNum = 0;
	for(i=0; i<pstExchgEntiy->ItemNum; i++)
	{
		ROLEGRID stGrid;
		pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstExchgEntiy->ItemEntiy[i].RoleGrid.GridData.RoleItem);
		if(pstItemDef ==NULL)
		{
			return -1;
		}

		memcpy (&stGrid, &pstExchgEntiy->ItemEntiy[i].RoleGrid, sizeof(stGrid));
		if (0 > package_add_in(pstEnv->pstAppCtx, pstEnv, NULL, &stPak, &stGrid, &stRolePakChg))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG3);
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE2, ZONEERR_EXG4);
			return -1;
		}
	}

	if (stPak.PetPackage.MaxNum - stPak.PetPackage.Num< pstExchgEntiy->PetNum)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG5);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE2, ZONEERR_EXG6);
		return -1;
	}
	
	return 0;
}
static int exchg_pack_dec(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	ExchgOnline *pstExchgOnline = &pstPlayer->stOnline.stExchgOnline;
	EXCHGITEM *pstExchgItem;
	EXCHGPET *pstExchgPet;
	ROLEPET *pstRolePet;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	int iPos ;

	package_dec_money(pstEnv,pstPlayer, ATTR_ID_MONEY, pstExchgOnline->stExchgData.Money);
	
	for (i=0; i<pstExchgOnline->stExchgData.ItemNum; i++)
	{	
		iPos = -1;
		pstExchgItem = &pstExchgOnline->stExchgData.ItemData[i];
		if (pstExchgItem->ListType == LIST_TYPE_SUNDRIES)
		{
			iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, pstExchgItem->GridIdx);
		}
		else if (pstExchgItem->ListType == LIST_TYPE_TASK)
		{
			iPos = package_itemgrid_pos(pstPak->TaskPackage.RoleItems,pstPak->TaskPackage.Num, pstExchgItem->GridIdx);
		}

		if (iPos < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "exchg exchg_pack_dec error!");
			return -1;
		}
		
		package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstExchgItem->DefID, pstExchgItem->Num, pstExchgItem->ListType, 
			&iPos,PACKAGE_DEC_NONE, 0);
	}

	for (i=pstExchgOnline->stExchgData.PetNum-1; i>=0; i--)
	{
		pstExchgPet = &pstExchgOnline->stExchgData.PetData[i];
		pstRolePet = z_find_role_pet(pstPlayer, pstExchgPet->WID, NULL);
		if (pstRolePet)
		{
			pet_del(pstEnv, pstPlayer, pstRolePet ->WID, 0, NULL);	
		}
	}
	return 0;
}

static int exchg_pack_add(ZONESVRENV* pstEnv, Player *pstPlayer, EXCHGENTIY *pstExchgEntiy)
{
	int i;
	ROLEGRID *pstGrid;

	PETPACKAGE *pstPetPack  = &pstPlayer->stRoleData.Package.PetPackage;
	ROLEPET *pstRolePet;
	PETDEF* pstPetDef;

	if (pstExchgEntiy->Money != 0)
	{
		tdr_ulonglong ullDecMony = pstExchgEntiy->Money*1.0*EXCHG_TAX_RATE/100.0;

		pstExchgEntiy->Money -= ullDecMony;
		package_add_money(pstEnv, pstPlayer, ATTR_ID_MONEY, pstExchgEntiy->Money);

		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_EXCHG_TEX, 0, 0, ullDecMony, "交易税");
	}
		
	for (i=0; i<pstExchgEntiy->ItemNum; i++)
	{
		pstGrid = &pstExchgEntiy->ItemEntiy[i].RoleGrid;
		/*if (pstGrid->Type == GRID_ITEM_ARM)
		{
			 pstGrid->GridData.RoleArm.KeYinAddAttr.AddAttrID = 0;
			 pstGrid->GridData.RoleArm.KeYinAddAttr.Val = 0;
			 pstGrid->GridData.RoleArm.KeYinEnd = 0;
		}*/
		package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer,  pstGrid, 1,0, 0);
	}

	for (i=0; i<pstExchgEntiy->PetNum; i++)
	{
		pstRolePet = &pstExchgEntiy->PetEntiy[i].RolePet;
		pstPetDef = z_find_pet_def(pstEnv,pstRolePet->PetID,NULL);
		
		if (pstPetDef)
		{
			pet_love_level_up_in(pstEnv,pstPlayer,pstRolePet,1, pstPetDef,0);
		}
		
		memcpy(&pstPetPack->RolePets[pstPetPack->Num], pstRolePet, sizeof(ROLEPET));
		pstPetPack->Num++;
		
		pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);

		if (pstPetDef)
		{
			range_pet_set(pstEnv, pstPlayer, pstRolePet, pstPetDef, RANGE_MAIN_PET_SUB_FIGHT,
						pet_fight_val(pstEnv, pstRolePet, pstPetDef));
		}

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

			if (pstRolePet && pstRolePet->PetGenius == PET_GENIUS_S)
			{
				stCond.Val1 = 2;
		
				z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
			}
		}
		
	}
	
	return 0;
}

int exchg_condition_check(ZONESVRENV* pstEnv, Player *pstPlayer,Player *pstOther)
{
	if (0 == z_player_act_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, RES_ACTION_EXCHG))
	{
		return -1;
	}

	if (0 == z_player_act_status(pstEnv->pstAppCtx, pstEnv, pstOther, RES_ACTION_EXCHG))
	{
		return -1;
	}

	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	}

	if (pstOther->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstOther, 0);
	}

	if (0 > z_check_op_dist(pstEnv, pstPlayer, pstOther->stRoleData.Map, &pstOther->stRoleData.Pos, EXCHG_MAX_DIST + 100))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG7);
		return -1;
	}
	
	return 0;
}
int player_exchg_invite(ZONESVRENV* pstEnv, Player *pstPlayer, CSEXCHGCLT *pstExchgClt)
{

	Player *pstOther;

	pstOther = z_name_player(pstEnv->pstAppCtx,  pstEnv, pstExchgClt->RoleName);
	if (!pstOther)
	{	
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG8);
		return -1;
	}

	if (player_check_safelock(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}

	if (player_check_grabsafe(pstEnv, pstOther) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
				ZONEERR_EXG9, GRAB_SAFE_TIME);
		return -1;
	}
	
	if (player_check_safelock_in(pstEnv, pstOther) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG10);
		return -1;
	}

	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		if(!is_together_world(pstEnv, pstPlayer, pstOther))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG11);
			return -1;
		}
	}
	
	if (pstPlayer == pstOther)
	{
		return -1;
	}

	if (pstOther->stOnline.bWaitRoleLogout)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG12);
		return -1;
	}

	if (pstPlayer->stOnline.tRoleLogin + LOGIN_NOT_EXCHG_TIME > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG13, pstPlayer->stOnline.tRoleLogin + LOGIN_NOT_EXCHG_TIME - pstEnv->pstAppCtx->stCurr.tv_sec);
		return -1;
	}

	if (pstOther->stOnline.tRoleLogin + LOGIN_NOT_EXCHG_TIME > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG14, pstOther->stOnline.tRoleLogin + LOGIN_NOT_EXCHG_TIME - pstEnv->pstAppCtx->stCurr.tv_sec);
		return -1;
	}

	/*if (pstPlayer->stOnline.State &CS_STAT_FIGHT )
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "战斗状态下无法交易");
		return -1;
	}

	if (pstOther->stOnline.State &CS_STAT_FIGHT )
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "对方在战斗状态下无法交易");
		return -1;
	}*/

	if (pstOther->stOnline.State &CS_STAT_STALL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG15);
		return -1;
	}

	if (pstOther->stOnline.State &CS_STAT_DEAD )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG16);
		return -1;
	}

	if (0 == z_player_act_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, RES_ACTION_EXCHG))
	{
		return -1;
	}

	if (pstOther->stOnline.stExchgOnline.bExchgStat != EXCHG_NO || 
		pstEnv->pstAppCtx->stCurr.tv_sec - pstOther->stOnline.stExchgOnline.iInViteTime < EXCHG_INVITE_TIME)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG17);
		return -1;
	}

	if (pstPlayer->stOnline.stExchgOnline.bExchgStat != EXCHG_NO || 
		pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.stExchgOnline.iInViteTime < EXCHG_INVITE_TIME)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG18);
		return -1;
	}


	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	}

	if (pstOther->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstOther, 0);
	}

	if (0 > z_check_op_dist(pstEnv, pstPlayer, pstOther->stRoleData.Map, &pstOther->stRoleData.Pos, EXCHG_MAX_DIST + 100))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG7);
		return -1;
	}

	pstPlayer->stOnline.stExchgOnline.iInViteTime = pstOther->stOnline.stExchgOnline.iInViteTime 
											= pstEnv->pstAppCtx->stCurr.tv_sec;

	exchg_msg_to_clt(pstEnv, pstPlayer, pstOther, INVITE);

	return 0;
}

int player_exchg_reject(ZONESVRENV* pstEnv, Player *pstPlayer, CSEXCHGCLT *pstExchgClt)
{
	Player *pstOther;
	
	pstPlayer->stOnline.stExchgOnline.iInViteTime = 0;
	
	pstOther = z_name_player(pstEnv->pstAppCtx,  pstEnv, pstExchgClt->RoleName);

	if (!pstOther)
	{
		return -1;
	}
	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		if(!is_together_world(pstEnv, pstPlayer, pstOther))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE2, ZONEERR_EXG11);
			return -1;
		}
	}

	if (pstExchgClt->Reason == INITIATIVE_REJECT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE2, ZONEERR_EXG19, pstPlayer->stRoleData.RoleName);
	}
	else if (pstExchgClt->Reason == ROLE_LOGOUT_REJECT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE2, ZONEERR_EXG20, pstPlayer->stRoleData.RoleName);	
	}
	else if (pstExchgClt->Reason == CHG_MAP_REJECT ||
			pstExchgClt->Reason == DIST_REJECT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE2, ZONEERR_EXG21);
	}
	else if (TIME_REJECT == pstExchgClt->Reason)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE2, ZONEERR_EXG22, pstPlayer->stRoleData.RoleName);	
	}
	
	exchg_msg_to_clt(pstEnv, pstPlayer, pstOther, REJECT);
	return 0;
}

int player_exchg_accept(ZONESVRENV* pstEnv, Player *pstPlayer, CSEXCHGCLT *pstExchgClt)
{
	Player *pstOther;
	
	pstOther = z_name_player(pstEnv->pstAppCtx,  pstEnv, pstExchgClt->RoleName);

	if (!pstOther)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG23);
		return -1;
	}
	
	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		if(!is_together_world(pstEnv, pstPlayer, pstOther))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG11);
			return -1;
		}
	}

	/*if (player_enemy_find_byname( &pstPlayer->stRoleData.MiscInfo, pstOther->stRoleData.RoleName) >= 0)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "对方在你宿敌名单中");
		return -1;
	}

	if (player_enemy_find_byname( &pstOther->stRoleData.MiscInfo, pstPlayer->stRoleData.RoleName) >= 0)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "你在对方宿敌名单中");
		return -1;
	}*/

	if (pstPlayer->stOnline.stExchgOnline.bExchgStat != EXCHG_NO )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG24);
		return -1;
	}
	
	if (pstOther->stOnline.stExchgOnline.bExchgStat != EXCHG_NO )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG17);
		return -1;
	}

	if (exchg_condition_check(pstEnv, pstPlayer,pstOther) < 0)
	{
		return -1;
	}

	pstPlayer->stOnline.stExchgOnline.bExchgStat = pstOther->stOnline.stExchgOnline.bExchgStat = EXCHG_READY;
	pstPlayer->stOnline.stExchgOnline.ullExChgObj = pstOther->stRoleData.RoleID;
	pstPlayer->stOnline.stExchgOnline.iMemID = pstOther->iMemID;
	
	pstOther->stOnline.stExchgOnline.ullExChgObj = pstPlayer->stRoleData.RoleID;
	pstOther->stOnline.stExchgOnline.iMemID = pstPlayer->iMemID;
		
	exchg_msg_to_clt(pstEnv, pstPlayer, pstOther, START);
	exchg_msg_to_clt(pstEnv, pstOther, pstPlayer, START);

	return 0;
}

int player_exchg_cancel(ZONESVRENV* pstEnv, Player *pstPlayer, CSEXCHGCLT *pstExchgClt)
{
	Player *pstOther;
//	char szMsg[128] = {0};
	int iMemID = pstPlayer->stOnline.stExchgOnline.iMemID;
	tdr_ulonglong ullRoleID = pstPlayer->stOnline.stExchgOnline.ullExChgObj;

	memset(&pstPlayer->stOnline.stExchgOnline, 0, sizeof(pstPlayer->stOnline.stExchgOnline));
	
	pstOther = z_id_player(pstEnv->pstAppCtx,  pstEnv, iMemID);
	if (!pstOther)
	{
		return -1;
	}
	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		if(!is_together_world(pstEnv, pstPlayer, pstOther))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG11);
			return -1;
		}
	}

	if (ullRoleID != pstOther->stRoleData.RoleID)
	{
		return -1;
	}

	if (pstOther->stOnline.stExchgOnline.bExchgStat == EXCHG_NO)
	{
		return -1;
	}

	/*if (pstExchgClt->Reason == INITIATIVE_REJECT)
	{
		snprintf(szMsg, sizeof(szMsg), "%s 取消了交易", pstPlayer->stRoleData.RoleName);
	}
	else if (pstExchgClt->Reason == ROLE_LOGOUT_REJECT)
	{
		snprintf(szMsg, sizeof(szMsg), "玩家%s 下线,交易取消", pstPlayer->stRoleData.RoleName);
	}
	else if (pstExchgClt->Reason == CHG_MAP_REJECT ||
			pstExchgClt->Reason == DIST_REJECT)
	{
		snprintf(szMsg, sizeof(szMsg), "你和%s 玩家距离太远,交易取消", pstPlayer->stRoleData.RoleName);
	}*/

	memset(&pstOther->stOnline.stExchgOnline, 0, sizeof(pstOther->stOnline.stExchgOnline));
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE2, ZONEERR_EXG25);
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG25);

	exchg_msg_to_clt(pstEnv, pstPlayer, pstOther, CANCEL);

	return 0;
	UNUSED(pstExchgClt);	//add by paraunused.pl
}

int player_exchg_cancel_in(ZONESVRENV* pstEnv, Player *pstPlayer,Player *pstOther)
{
	Player *pstTmp = NULL;
	
	if (pstOther)
	{
		memset(&pstPlayer->stOnline.stExchgOnline, 0, sizeof(pstPlayer->stOnline.stExchgOnline));
		memset(&pstOther->stOnline.stExchgOnline, 0, sizeof(pstOther->stOnline.stExchgOnline));
		exchg_msg_to_clt(pstEnv, pstPlayer, pstOther, CANCEL);
		exchg_msg_to_clt(pstEnv, pstOther, pstPlayer, CANCEL);
	}
	else
	{
		pstTmp = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stExchgOnline.iMemID);
		if (pstTmp)
		{
			memset(&pstTmp->stOnline.stExchgOnline, 0, sizeof(pstTmp->stOnline.stExchgOnline));
			exchg_msg_to_clt(pstEnv, pstPlayer, pstTmp, CANCEL);
			exchg_msg_to_clt(pstEnv, pstTmp, pstPlayer, CANCEL);
		}
		memset(&pstPlayer->stOnline.stExchgOnline, 0, sizeof(pstPlayer->stOnline.stExchgOnline));
	}
	return 0;
}

int player_exchg_lock(ZONESVRENV* pstEnv, Player *pstPlayer, CSEXCHGCLT *pstExchgClt)
{
	Player *pstOther;
	EXCHGENTIY stExchgEntity;
	
	pstOther = z_name_player(pstEnv->pstAppCtx,  pstEnv, pstExchgClt->RoleName);
	if (!pstOther)
	{
		goto errer;
	}

	if (pstPlayer->stOnline.stExchgOnline.ullExChgObj != pstOther->stRoleData.RoleID)
	{
		goto errer;
	}

	if (pstOther->stOnline.stExchgOnline.ullExChgObj != pstPlayer->stRoleData.RoleID)
	{
		goto errer;
	}

	if (player_check_safelock(pstEnv, pstPlayer) < 0)
	{
		goto errer;
	}
	
	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		if(!is_together_world(pstEnv, pstPlayer, pstOther))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG11);
			goto errer;
		}
	}

	if (pstPlayer->stOnline.stExchgOnline.bExchgStat != EXCHG_READY|| 
		pstOther->stOnline.stExchgOnline.bExchgStat == EXCHG_NO)
	{
		goto errer;
	}

	if (exchg_condition_check(pstEnv, pstPlayer,pstOther) < 0)
	{
		goto errer;
	}

	memcpy(&pstPlayer->stOnline.stExchgOnline.stExchgData, &pstExchgClt->ExchgData, sizeof(pstExchgClt->ExchgData));
	if (exchg_data_check(pstEnv, pstPlayer, pstOther, &stExchgEntity) < 0)
	{
		goto errer;
	}

	// 具体数据通知客户端
	pstPlayer->stOnline.stExchgOnline.bExchgStat = EXCHG_LOCK;
	exchg_lock_to_clt(pstEnv, pstPlayer, pstOther, &stExchgEntity);
	return 0;
	

	errer:
		player_exchg_cancel_in(pstEnv, pstPlayer,pstOther);
		return -1;
}

int player_exchg_deal(ZONESVRENV* pstEnv, Player *pstPlayer, CSEXCHGCLT *pstExchgClt)
{
	Player *pstOther = NULL;
	ExchgOnline *pstExchgOnline = &pstPlayer->stOnline.stExchgOnline;
	int iRet = 0;

	if (pstExchgOnline->bExchgStat == EXCHG_DEAL)
	{
		goto errer;
	}
	
	if (pstExchgOnline->bExchgStat < EXCHG_LOCK)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG26);
		goto errer;
	}

	pstOther = z_name_player(pstEnv->pstAppCtx,  pstEnv, pstExchgClt->RoleName);
	if (!pstOther)
	{
		goto errer;
	}

	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		if(!is_together_world(pstEnv, pstPlayer, pstOther))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG11);
			goto errer;
		}
	}

	if (	pstOther->eStatus != PLAYER_STATUS_ROLE_LOGIN ||
		pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		goto errer;
	}

	if (pstExchgOnline->ullExChgObj != pstOther->stRoleData.RoleID)
	{
		goto errer;
	}

	if (pstOther->stOnline.stExchgOnline.bExchgStat == EXCHG_NO)
	{
		goto errer;
	}

	if (exchg_condition_check(pstEnv, pstPlayer,pstOther) < 0)
	{
		goto errer;
	}

	if (pstOther->stOnline.stExchgOnline.bExchgStat == EXCHG_DEAL)
	{
		EXCHGENTIY stExchgEntiyPlayer, stExchgEntiyOther;

		if (exchg_data_check(pstEnv, pstPlayer, pstOther, &stExchgEntiyPlayer) <0 ||
			exchg_data_check(pstEnv, pstOther, pstPlayer, &stExchgEntiyOther) <0 )
		{
			//iRet = -1;
			goto errer;
		}
		else
		{
			if (exchg_pack_dec_add_test(pstEnv, pstPlayer, &stExchgEntiyOther) < 0)
			{
				goto errer;
			}
			else if (exchg_pack_dec_add_test(pstEnv, pstOther, &stExchgEntiyPlayer) < 0)
			{
	
				goto errer;
			}
			else
			{
				//日志流水号
				z_oplog_event_begin(pstEnv);
				
				// 记录交易对象,时间以及交易进来的物品宠物和钱
				z_role_exchg_oplog(pstEnv, pstPlayer,pstOther, &stExchgEntiyOther,&stExchgEntiyPlayer);
				z_role_exchg_oplog(pstEnv, pstOther,pstPlayer, &stExchgEntiyPlayer,&stExchgEntiyOther);
				
				exchg_pack_dec(pstEnv, pstPlayer);
				exchg_pack_dec(pstEnv, pstOther);
				exchg_pack_add(pstEnv, pstPlayer, &stExchgEntiyOther);
				exchg_pack_add(pstEnv, pstOther, &stExchgEntiyPlayer);
				
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG27);
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE2, ZONEERR_EXG27);
				iRet = 0;
				
			}
		}

		exchg_msg_to_clt(pstEnv, pstPlayer, pstOther, END);
		exchg_msg_to_clt(pstEnv, pstOther, pstPlayer, END);
		
		memset(&pstPlayer->stOnline.stExchgOnline, 0, sizeof(pstPlayer->stOnline.stExchgOnline));
		memset(&pstOther->stOnline.stExchgOnline, 0, sizeof(pstOther->stOnline.stExchgOnline));	
		
		return iRet;
		
	}
	else
	{
		pstPlayer->stOnline.stExchgOnline.bExchgStat = EXCHG_DEAL;
		exchg_msg_to_clt(pstEnv, pstPlayer, pstOther, DEAL);
		return 0;
	}


	errer:
		if(pstOther)
		{
			player_exchg_cancel_in(pstEnv, pstPlayer,pstOther);
		}
		else
		{
			player_exchg_cancel_in(pstEnv, pstPlayer,NULL);
		}
		
	return -1;
		
}

//  取消锁定
int player_exchg_cancel_lock(ZONESVRENV* pstEnv, Player *pstPlayer, CSEXCHGCLT *pstExchgClt)
{
	Player *pstOther;
	ExchgOnline *pstExchgOnline = &pstPlayer->stOnline.stExchgOnline;
	
	pstOther = z_name_player(pstEnv->pstAppCtx,  pstEnv, pstExchgClt->RoleName);
	if (!pstOther)
	{
		return -1;
	}

	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		if(!is_together_world(pstEnv, pstPlayer, pstOther))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_EXG11);
			return -1;
		}
	}
	
	if (pstOther->stOnline.stExchgOnline.bExchgStat == EXCHG_NO)
	{
		return -1;
	}

	if (pstExchgOnline->ullExChgObj != pstOther->stRoleData.RoleID ||
		pstOther->stOnline.stExchgOnline.ullExChgObj != pstPlayer->stRoleData.RoleID)
	{
		return -1;
	}

	if (pstExchgOnline->bExchgStat != EXCHG_LOCK &&  pstExchgOnline->bExchgStat != EXCHG_DEAL)
	{
		return -1;
	}

	if (exchg_condition_check(pstEnv, pstPlayer,pstOther) < 0)
	{
		return -1;
	}

	pstPlayer->stOnline.stExchgOnline.bExchgStat = pstOther->stOnline.stExchgOnline.bExchgStat = EXCHG_READY;
	
	exchg_msg_to_clt(pstEnv, pstPlayer, pstOther, CANCEL_LOCK);
	exchg_msg_to_clt(pstEnv, pstOther, pstPlayer, CANCEL_LOCK);
	
	return 0;
}

int player_exchg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSEXCHGCLT *pstExchgClt = &pstCsPkg->Body.ExchgClt;

	if(INST_MODE_ZONE_SVR != pstEnv->iInstMode )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
		return -1;
	}
	
	switch(pstExchgClt->type)
	{
	case INVITE:	
		return player_exchg_invite(pstEnv, pstPlayer, pstExchgClt);
		break;
	case REJECT:
		return player_exchg_reject(pstEnv, pstPlayer, pstExchgClt);
		break;
	case ACCEPT:
		return player_exchg_accept(pstEnv, pstPlayer, pstExchgClt);
		break;
	case CANCEL:
		return player_exchg_cancel(pstEnv, pstPlayer, pstExchgClt);
		break;
	case LOCK:
		return player_exchg_lock(pstEnv, pstPlayer, pstExchgClt);
		break;
	case DEAL:
		return player_exchg_deal(pstEnv, pstPlayer, pstExchgClt);
		break;
	case CANCEL_LOCK:
		return player_exchg_cancel_lock(pstEnv, pstPlayer, pstExchgClt);
		break;
	default:
		return -1;
		break;
	}
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

int exchg_logout(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	Player *pstOther;
	int iMemID = pstPlayer->stOnline.stExchgOnline.iMemID;

	if (iMemID <= 0)
	{
		return 0;
	}

	pstOther = z_id_player(pstEnv->pstAppCtx, pstEnv, iMemID);

	if (pstOther && pstOther->stOnline.stExchgOnline.bExchgStat != EXCHG_NO &&
		pstOther->stOnline.stExchgOnline.ullExChgObj == pstPlayer->stRoleData.RoleID)
	{
		memset (&pstOther->stOnline.stExchgOnline, 0, sizeof(pstOther->stOnline.stExchgOnline));
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE2, ZONEERR_EXG28);
		exchg_msg_to_clt(pstEnv, pstPlayer, pstOther,CANCEL);
	}

	memset (&pstPlayer->stOnline.stExchgOnline, 0, sizeof(pstPlayer->stOnline.stExchgOnline));
	
	return 0;
}

