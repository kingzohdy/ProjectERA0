
/*
**  @file $RCSfile: zone_collect.c,v $
**  general description of this module
**  $Id: zone_collect.c,v 1.72 2014/06/23 08:51:55 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2014/06/23 08:51:55 $
**  @version $Revision: 1.72 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_collect.h"
#include "zone_player.h"
#include "tsec/tsecbasedef.h"
#include "tconnapi.h"
#include "comm/shtable.h"
#include "zone_clt.h"
#include "zone_map.h"
#include "zone_move.h"
#include "mapmask.h"
#include "zone_act_status.h"
#include "zone_attr.h"
#include "zone_script.h"
#include "zone_skill.h"
#include "zone_status.h"
#include "zone_oplog.h"
#include "zone_package.h"
#include "zone_err.h"
#include "zone_combat.h"
#include "zone_team.h"
#include "zone_task.h"
#include "zone_npc.h"
#include "zone_combat.h"
#include "zone_pet.h"
#include "zone_secondaryskill.h"
#include "zone_machine.h"
#include "zone_misc.h"
#include "comm_misc.h"
#include "zone_roll.h"
#include "zone_ai.h"
#include "zone_itemlimit.h"
#include "zone_booty.h"
#include "zone_city.h"
#include "zone_strongpoint.h"
#include "zone_err.h"
#include "zone_battle.h"
#include "zone_home.h"

extern ScriptEnv stSEnv;

int g_aiOrePositionMap[50];
int g_iOrePositionRangeCurr;

MAPOREDEF* z_find_map_ore_def(ZONESVRENV *pstEnv, int iMapID)
{
	return (MAPOREDEF *)z_find_def(pstEnv->pstMapObj->sMapOreDef, iMapID, MAP_ORE_DEF_DATA, 
									pstEnv->pstMapObj->iMapOreDef , NULL);
}

ROLEARM *z_wear_get_secondaryskill_arm(ZONESVRENV* pstEnv, ROLEWEAR *pstRoleWear, int iSecondarySkillType)
{
	int i;
	ROLEARM *pstWear;
	ITEMDEF *pstItemDef;
	RESULT *pstResult;

	for (i=0; i<pstRoleWear->ArmNum; i++)
	{
		pstWear = pstRoleWear->RoleArms+i;

		pstItemDef = z_get_itemdef_roleitem( pstEnv, (ROLEITEM * )pstWear);
		if(NULL == pstItemDef)
		{
			continue;
		}

		pstResult = z_item_get_result(pstItemDef, RESULT_SECONDARY_SKILL_ITEM);
		if (pstResult && pstResult->ResultVal1 == iSecondarySkillType)
		{
			return pstWear;
		}
	}

	return NULL;
}

int z_collect_secondaryskill_check(ZONESVRENV* pstEnv, Player *pstPlayer, MONCOLLECT *pstMonsterCollectDef)
{
	RECIPEDEF *pstRecipeDef;
	SECONDARYSKILLINFO *pstSkillInfo = get_player_secondary_skill_info(pstPlayer, pstMonsterCollectDef->CollectSecondarySkillType);
	if(NULL == pstSkillInfo)
	{
		return -1;
	}

	if (0 == z_get_skill_recipe(pstSkillInfo, pstMonsterCollectDef->CollectRecipeID))
	{
		return -1;
	}

	pstRecipeDef = z_find_recipe_def(pstEnv, pstMonsterCollectDef->CollectSecondarySkillType, pstMonsterCollectDef->CollectRecipeID);
	if(NULL == pstRecipeDef)
	{
		//不存在配方
		return -1;
	}

	//精力
	if (pstRecipeDef->ConsumeVim > pstPlayer->stRoleData.RoleDetail.VIMCurr)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NO_VIM);
		return -1;
	}

	if (pstMonsterCollectDef->CollectArmDur > 0)
	{
		ROLEARM *pstRoleArm;
		
		pstRoleArm = z_wear_get_secondaryskill_arm(pstEnv, &pstPlayer->stRoleData.Wear, pstMonsterCollectDef->CollectSecondarySkillType);
		if (NULL == pstRoleArm) 
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_COLLECT1);
			return -1;
		}

		if (pstRoleArm->Dur < pstMonsterCollectDef->CollectArmDur)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_COLLECT2);
			return -1;
		}
	}

	return 0;
}

int mon_booty_assign_collect(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMon, MONSTERDEF *pstMonDef, int iDecHP)
{
	int i;
	int iOreIdx;
	MONBOOTYITEM *pstBootyItem;
	ROLEGRID stRoleGrid;
	int iRet = 0;
	int fTaskBooty = 0;
	int iBootyFlag;
	Monster *pstMachine = NULL;
	MONMACHINEDEF *pstMacDef = NULL;

	if(is_player_ride_on_machine(pstPlayer))
	{	
		pstMachine = machine_get_by_player(pstEnv, pstPlayer);
		if(NULL == pstMachine)
		{
			return -1;
		}

		pstMacDef = z_find_mon_machine_def(pstEnv, pstMachine->iDefIdx);
		if (!pstMacDef)
		{
			return -1;
		}
	}

	//日志流水号
	z_oplog_event_begin(pstEnv);
	
	iBootyFlag = 0;
	for (i=0; i<MAX_MONBOOTY_ITEM; i++)
	{
		pstBootyItem = pstMonDef->Booty.Items + i;
		if (0 == pstBootyItem->ItemID)
		{
			break;
		}

		if (0 != pstBootyItem->TaskID1)
		{
			//一次只采集一个任务物品
			if(fTaskBooty)
			{
				continue;
			}
				
			if (0 > mon_booty_check_task(pstEnv, pstBootyItem, pstPlayer, 1))
			{	
				iRet = -1;
				continue;
			}
				
			fTaskBooty = 1;
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

		//超过全局限制就不掉落了
		if(0 > world_item_limit_check(pstEnv, pstBootyItem->ItemID))
		{
			continue;
		}

		iRet = 0;

		//记录下全局限制的掉落
		world_item_limit_add(pstEnv, pstBootyItem->ItemID, pstBootyItem->Num);

		//这个和怪物掉落相反，1代表掉地上
		if(pstBootyItem->DirectPakage == 1)
		{
			mon_booty_drop(pstEnv, pstMon, &stRoleGrid, pstPlayer);
		}
		else
		{
			//DirectPakage = 2 只有乘坐采集车才能采到的物品
			if(pstBootyItem->DirectPakage == 2 && !is_player_ride_on_machine(pstPlayer))
			{
				continue;
			}
			
			if(pstMachine && pstMacDef)
			{
				iOreIdx = machine_get_ore_idx_byid(pstMacDef, pstBootyItem->ItemID);
				if(iOreIdx >= 0)
				{
					iBootyFlag = 1;
					z_machine_attr_chg(pstEnv, pstMachine, MON_MACHINE_ATTR_ORE1+iOreIdx, pstBootyItem->Num);
					//装不下了
					if (machine_ores_sum(pstEnv, pstMachine)>= this_machine(pstEnv,pstMachine)->stMachineInfo.MaxPackOre)
					{
						return -1;
					}
					
					continue;
				}
			}
			
			if (0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
				mon_booty_drop(pstEnv, pstMon, &stRoleGrid, pstPlayer);
				iRet = -1;
			}
			else
			{
				ROLEGRID stDummy;
				
				// OP LOG
				memcpy(&stDummy, &stRoleGrid, sizeof(stDummy));
				package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0, IMPITEM_OPTYPE_PICK);

				//记录一下拾取日志
				z_role_pickitem_oplog( pstEnv, pstPlayer, &stDummy, 1, OP_PICK);
			}
		}
		
		iBootyFlag = 1;
		//第二次判断，看看增加道具后任务是否完成
		if (0 != pstBootyItem->TaskID1) 
		{
			if (0 > mon_booty_check_task(pstEnv, pstBootyItem, pstPlayer, 1))
			{
				iRet = -1;
				fTaskBooty = 0;
			}
		}
	}

	if(iBootyFlag == 0 &&  pstMonDef->Booty.Items[0].ItemID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_COLLECT3);
	}

	mon_booty_assign_resource(pstEnv, pstMonDef->Booty.Exp, pstPlayer);

	if(pstMonDef->Collect.CollectSecondarySkillType > 0)
	{
		CSSECONDARYSKILLRECIPE stRecipe;

		stRecipe.SkillType = pstMonDef->Collect.CollectSecondarySkillType;
		stRecipe.RecipeID = pstMonDef->Collect.CollectRecipeID;
		
		return player_recipe(pstEnv, pstPlayer, &stRecipe);
	}
	UNUSED(iDecHP);
	return iRet;
}

int mon_booty_group_assign_collect(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMon, MONSTERDEF *pstMonDef)
{
	int i;
	Player *apstPlayer[MAX_TEAM_MEMBER]; 
	int iValidTeamNum = 0;
	Player *pstPlayerTmp;
	MONBOOTYMONEY *pstBootyMoney;
	ZoneTeam *pstTeam = NULL;

	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(pstTeam)
	{
		for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
		{
			pstPlayerTmp = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
			if (NULL == pstPlayerTmp || 0 > mon_booty_valid_player(pstMon, pstPlayerTmp))
			{
				continue;
			}

			apstPlayer[iValidTeamNum] = pstPlayerTmp; 
			iValidTeamNum++;
		}
	}
	else
	{
		apstPlayer[iValidTeamNum] = pstPlayer; 
		iValidTeamNum = 1;
	}
	
	if (0 >= iValidTeamNum)
	{
		return -1;
	}

	for (i=0; i<MAX_MONBOOTY_MONEY; i++)
	{
		pstBootyMoney = &pstMonDef->Booty.Moneys[i];	

		if (ATTR_ID_MONEY != pstBootyMoney->MoneyAttrID && ATTR_ID_BULL != pstBootyMoney->MoneyAttrID)
		{
			break;
		}

		if ((int)(RAND1(10000)) >= pstBootyMoney->Rate * 100.0)
		{
			continue;
		}
		
		booty_money_drop(pstEnv, apstPlayer[(int) RAND1(iValidTeamNum)], pstMon, 
				pstBootyMoney->MoneyAttrID, pstBootyMoney->MoneyVal);
	}

	rand_position_init();


	pstMon->iFirstAtkerID = pstPlayer->iMemID;
	pstMon->iFirstAtkerMemID = pstPlayer->iMemID;
	pstMon->cFirstAtkerType = MON_BOOTY_OWNER_PLAYER;
	pstMon->ullFirstAtkerWid = pstPlayer->stRoleData.RoleID;
	
	//怪物战利品掉落表
	for (i=0; i<MAX_MON_BOOTY_GROUP_LIST; i++)
	{
		if(pstMonDef->BootyGroup[i].GroupID == 0)
		{
			continue;
		}

		if(pstMonDef->BootyGroup[i].Flag & BOOTY_FLAG_COLLECTER_ASSIGN)
		{
			apstPlayer[0] = pstPlayer;
			iValidTeamNum = 1;
			
			mon_booty_group_assign(pstEnv, pstMon, &pstMonDef->BootyGroup[i], NULL);
		}
		else
		{
			if(pstTeam)
			{
				mon_booty_group_assign(pstEnv, pstMon, &pstMonDef->BootyGroup[i], pstTeam);
			}
			else
			{
				mon_booty_group_assign(pstEnv, pstMon, &pstMonDef->BootyGroup[i], NULL);
			}
		}
	}
	
	return 0;
}

int player_collect_task_check(ZONESVRENV* pstEnv, Player *pstPlayer, TASKRUNONE *pstTaskRun, MONSTERDEF *pstMonDef, TASKDEF *pstTaskDef)
{
	MONBOOTYITEM *pstBootyItem;
	int iIdx = -1;
	int i;
	
	pstBootyItem = &pstMonDef->Booty.Items[0];
	if (pstBootyItem->ItemID == 0)
	{
		if(task_check_fini(pstEnv, pstPlayer, pstTaskRun, pstTaskDef))
		{
			return -1;
		}

		return 0;
	}

	for(i = 0; i<MAX_MONBOOTY_ITEM; i++)
	{
		pstBootyItem = &pstMonDef->Booty.Items[i];
		if(pstBootyItem->ItemID == 0)
		{
			break;
		}
		
		if(pstBootyItem->TaskID1 != pstTaskDef->ID && pstBootyItem->TaskID2 != pstTaskDef->ID)
		{
			continue;
		}

		iIdx = get_taskiteam_idx(pstTaskDef, pstBootyItem->ItemID);
		if(0 >iIdx)
		{
			return -1;
		}

		if(0 ==  package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
									pstTaskDef->GetItem[iIdx].ItemID, pstTaskDef->GetItem[iIdx].Num, -1, NULL,PACKAGE_DEC_NONE))
		{
			return -1;
		}
	}

	return 0;
} 

int player_collect_check(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMon, MONSTERDEF *pstMonDef)
{
	int i;
	TASKRUNONE *pstTaskRun;
	TASKDEF *pstTaskDef;
	int iFlag = 0;
	MONCOLLECT *pstMonsterCollectDef = &pstMonDef->Collect;
	Monster *pstMachine =NULL;
	int iPakFlag;

	if(pstMonDef->MonsterID == XUKONG_ENERGY_ID &&
		is_has_xukong_energy(pstPlayer))
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_COLLECT_301);
		return -1;
	}
	
	if (0 != pstMonsterCollectDef->TaskID[0])
	{
		iFlag = 0;
		for (i=0; i<MON_COLLECT_TASK_NUM; i++)
		{
			if (0 == pstMonsterCollectDef->TaskID[i])
			{
				break;
			}

			pstTaskRun = task_run_find(pstPlayer, pstMonsterCollectDef->TaskID[i]);
			if (pstTaskRun)
			{
				pstTaskDef = task_def_find(pstEnv, pstTaskRun->TaskID, &pstTaskRun->TaskIdx);
							
				if (pstTaskDef && 0 == player_collect_task_check(pstEnv, pstPlayer, pstTaskRun, pstMonDef, pstTaskDef))
				{
					iFlag = 1;
					break;
				}
			}
		}

		if (0 == iFlag)
		{
			//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, "未接取采集任务或任务已完成");
			return -1;
		}
	}

	if (pstMonsterCollectDef->CollectArm > 0)
	{
		ITEMDEF *pstItemDef;
		ROLEARM *pstRoleArm;

		pstItemDef = z_find_itemdef(pstEnv, pstMonsterCollectDef->CollectArm);
		if (NULL == pstItemDef || pstItemDef->ItemType != ITEM_TYPE_ARM)
		{
			return -1;
		}

		pstRoleArm = z_wear_get(&pstPlayer->stRoleData.Wear, pstMonsterCollectDef->CollectArm, -1);
		if (NULL == pstRoleArm)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_COLLECT4, pstItemDef->Name);
			return -1;
		}
		else if (pstRoleArm->Dur < pstMonsterCollectDef->CollectArmDur)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_COLLECT5, pstItemDef->Name);
			return -1;
		}
	}

	if (pstMonsterCollectDef->CollectSkillID > 0)
	{
		SKILLDEF *pstSkillDef;
		ZoneSkill *pstZoneSkill;

		pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstMonsterCollectDef->CollectSkillID);
		
		if (NULL == pstZoneSkill)
		{
			pstSkillDef = z_find_skill(pstEnv, pstMonsterCollectDef->CollectSkillID, 1, NULL);
			if (NULL == pstSkillDef)
			{
				return -1;
			}
			
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_COLLECT6, pstSkillDef->Name);
			return -1;
		}
		else if (pstZoneSkill->stSkillData.SkillLevel < pstMonDef->Level)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_COLLECT7, pstMonDef->Level);
			return -1;
		}
	}

	if (pstMonsterCollectDef->CollectItem> 0)
	{
		ITEMDEF *pstItemDef;
		pstItemDef = z_find_itemdef(pstEnv, pstMonsterCollectDef->CollectItem);
		if (!pstItemDef)
		{
			return -1;
		}
		
		if (pstMonsterCollectDef->CollectItemDur > 0)
		{
			if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMonsterCollectDef->CollectItem, pstMonsterCollectDef->CollectItemDur, -1, NULL,PACKAGE_DEC_NONE))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_COLLECT8, pstItemDef->Name);
				return -1;
			}
		}
		else
		{
			if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMonsterCollectDef->CollectItem, 1, -1, NULL,PACKAGE_DEC_NONE))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_COLLECT9, pstItemDef->Name);
				return -1;
			}
		}
	}

	iPakFlag = 0;
	for(i=0; i< MAX_MON_COLLECT_PACKAGE_ITEM; i++)
	{
		if (pstMonsterCollectDef->CollectPackgeItem[i] == 0)
		{
			break;
		}

		//需要拥有其中一个物品
		if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMonsterCollectDef->CollectPackgeItem[i], 1, -1, NULL,PACKAGE_DEC_NONE))
		{
			iPakFlag = 1;
		}
		else
		{
			iPakFlag = 0;
			break;
		}
	}

	if(iPakFlag)
	{
		return -1;
	}

	if(pstPlayer->bAasIncomeLimt > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
						ZONEERR_COLLECT10);
		return -1;
	}	

	if (pstMon->szOwner[0] && strcmp(pstMon->szOwner, pstPlayer->stRoleData.RoleName) != 0)
	{
		return -1;
	}

	if (pstMon->iOwnerID > 0 && pstMon->iOwnerID != pstPlayer->iMemID)
	{
		if(pstMon->stTeamInfo.ullTeamID >0 )
		{
			ZoneTeam *pstTeam;

			pstTeam = player_team_get(pstEnv, pstPlayer);
			if(!pstTeam || pstMon->stTeamInfo.ullTeamID != pstTeam->stTeamInfo.TeamID)
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}

	//生活技能
	if(pstMonsterCollectDef->CollectSecondarySkillType > 0)
	{
		if (0 > z_collect_secondaryskill_check(pstEnv, pstPlayer, pstMonsterCollectDef))
		{
			return -1;
		}
	}

	if(is_player_ride_on_machine(pstPlayer))
	{
		int i;
		MONMACHINEDEF *pstMachineDef;
		
		pstMachine = machine_get_by_player(pstEnv, pstPlayer);
		if(NULL == pstMachine)
		{
			return -1;
		}

		pstMachineDef = z_find_mon_machine_def(pstEnv, pstMachine->iDefIdx);
		if(NULL == pstMachineDef ||pstMachineDef->CollectType != pstMonsterCollectDef->CollectType)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
						ZONEERR_COLLECT11);
			return -1;
		}

		for(i=0; i<MAX_MACHINE_LIMIT_ORE;i++)
		{
			if(pstMachineDef->LimitOreMonD[i] == (int)pstMonDef->MonsterID)
			{
				break;
			}
		}

		if(i>= MAX_MACHINE_LIMIT_ORE)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
						ZONEERR_COLLECT11);
			return -1;
		}

		//装不下了
		if (machine_ores_sum(pstEnv, pstMachine)>= this_machine(pstEnv,pstMachine)->stMachineInfo.MaxPackOre)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_COLLECT14);
			return -1;
		}

		if(0 >= get_machine_attr_val(pstEnv, pstMachine, MON_MACHINE_ATTR_ENERGY))
		{
			return -1;
		}

		if(is_mon_big_ore(pstMon->iDefIdx) ||
			is_mon_big_oil(pstMon->iDefIdx))
		{
			WORLDOREINFO *pstWorldOreInfo;
			
			pstWorldOreInfo = global_world_ore_get_byid(pstEnv, pstMon->iDefIdx);
			if(NULL == pstWorldOreInfo)
			{
				return -1;
			}
			
			if(pstWorldOreInfo->Factory.MemID <= 0 ||
				pstWorldOreInfo->Factory.BuildTime > 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_COLLECT15);
				return -1;
			}
		}
	}
	else
	{
		if(pstMonsterCollectDef->CollectFlag == 1)
		{
			return -1;
		}
	}

	//同阵营了就不能在占领了
	if(is_camp_mon(pstMon) && is_player_same_camp_mon(pstPlayer, pstMon))
	{
		return -1;
	}

	if(is_city_build_mon(pstMon))
	{
		//攻城期间，进攻方可以采集
		if(is_in_siege_city(pstEnv))
		{
			CITYBUILDINFO *pstBuildInfo;

			
			pstBuildInfo = world_city_build_info_get(pstEnv, pstMon->iCityBuildDefID);
			if(NULL == pstBuildInfo)
			{
				return -1;
			}
			
			if(pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID == 0ULL)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_COLLECT16);
				return -1;
			}

			if(NULL == strong_get_by_player(pstEnv, pstPlayer))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_COLLECT16);
				return -1;
			}

			if(pstBuildInfo->Type != CITY_BUILD_TYPE_CONTROL)
			{
				if(pstMon->iPKType == MON_PK_TYPE_CLAN &&
					pstMon->ullPKID == pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_COLLECT17);
					return -1;
				}
			}
		}
		else
		{
			return -1;
		}
	}
	
	return 0;
}

int player_collect_item_start(ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstDropItem)
{
	Monster *pstMachine;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	if(!is_collect_drop_item(pstDropItem))
	{
		return -1;
	}
	
	if (0 > z_check_op_dist(pstEnv, pstPlayer, pstDropItem->stMap.iID, &pstDropItem->stPos, MAX_COLLECT_ITEM_DIST+100))
	{
		return -1;
	}

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_COLLECT;
	pstActionRes->Data.Collect.Flag = 0;
	pstActionRes->Data.Collect.MonID = pstDropItem->iID;
	pstActionRes->Data.Collect.MonCurrHP = 0;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	
	pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	if(pstMachine)
	{
		z_sendpkg_monview(pstEnv->pstAppCtx, pstEnv, pstMachine, &stPkg, 0);
	}
	else
	{
		z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	}

	pstPlayer->stOnline.State |= CS_STAT_COLLECT;
	pstPlayer->stOnline.tCollectAward = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstPlayer->stOnline.iCollectTarget = pstDropItem->iID;
	
	return 0;
}

int player_collect_mon_start(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMonCollectee)
{
	int iPlayerRadius = 0;
	CSPKG stPkg;
	MONSTERDEF *pstMonsterDef;
	Monster *pstMachine = NULL;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	if(is_player_ride_on_machine(pstPlayer))
	{
		pstMachine = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stMachineInfo.iMachineMonID);
		if(pstMachine)
		{
			iPlayerRadius = pstMachine->unRadius;
		}
	}

	if (0 > z_check_op_dist(pstEnv, pstPlayer, pstMonCollectee->stMap.iID, &pstMonCollectee->stCurrPos, OP_DIST/4+pstMonCollectee->unRadius+ iPlayerRadius))
	{
		return -1;
	}

	pstMonsterDef = z_get_mon_def(pstEnv, pstMonCollectee);
	if (NULL == pstMonsterDef)
	{
		return -1;
	}

	if (0 > player_collect_check(pstEnv, pstPlayer, pstMonCollectee, pstMonsterDef))
	{
		pstActionRes->ID = pstPlayer->iMemID;
		pstActionRes->ActionID = ACTION_COLLECT;
		pstActionRes->Data.Collect.Flag = 1;
		pstActionRes->Data.Collect.MonID = pstMonCollectee->iID;
		pstActionRes->Data.Collect.MonCurrHP = pstMonCollectee->iHP;

		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		
		return -1;
	}

	pstPlayer->stOnline.State |= CS_STAT_COLLECT;
	pstPlayer->stOnline.tCollectAward = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstPlayer->stOnline.iCollectTarget = pstMonCollectee->iID;

	if(pstMonCollectee->cMonScriptFlag & MON_SCRIPT_PROC_COLLECT_BEGIN_MAP)
	{
		char szName[128];
		
		pstMonCollectee->bDelayScript |= MON_SCRIPT_PROC_COLLECT_BEGIN;
		
		stSEnv.pstMonster = pstMonCollectee;
		stSEnv.pstNpc = NULL;
		stSEnv.pstPworldInst = NULL;
		stSEnv.pstPlayer = pstPlayer;
		stSEnv.pstKiller = NULL;
		stSEnv.pstMapinst = NULL;
		
		snprintf(szName, sizeof(szName), "monster%d.mac", pstMonsterDef->MonsterID);
		szName[sizeof(szName) - 1] = 0;
				
		z_exec_script(pstEnv->pstAppCtx, pstEnv, NULL, szName, &pstMonCollectee->stScriptIdx, MON_SCRIPT_PROC_COLLECT_BEGIN);
	}

	//脚本可能打断采集
	if(!(pstPlayer->stOnline.State & CS_STAT_COLLECT))
	{
		return -1;
	}
		
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_COLLECT;
	pstActionRes->Data.Collect.Flag = 0;
	pstActionRes->Data.Collect.MonID = pstMonCollectee->iID;
	pstActionRes->Data.Collect.MonCurrHP = pstMonCollectee->iHP;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);

	if(pstMachine)
	{
		z_sendpkg_monview(pstEnv->pstAppCtx, pstEnv, pstMachine, &stPkg, 0);
	}
	else
	{
		z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	}

	world_city_build_warning(pstEnv, pstMonCollectee);

	return 0;
}

int player_collect_start(ZONESVRENV* pstEnv, Player *pstPlayer, CSCOLLECTSTART *pstCollectStart)
{
	int iIndex;
	char cType;
	int iInSelfHome;
	
	if(CS_STAT_COLLECT & pstPlayer->stOnline.State)
	{
		if(pstPlayer->stOnline.iCollectTarget == pstCollectStart->MonID)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_COLLECT18);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_COLLECT19);
		}
		
		return -1;	
	}

	home_player_is_in(pstEnv, pstPlayer, &iInSelfHome);
	if (iInSelfHome == 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_04);
		return -1;
	}

	 

	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NO_COLLECT) ||
		z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOCLT))
	{
		CSPKG stPkg;
		CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	
		pstActionRes->ID = pstPlayer->iMemID;
		pstActionRes->ActionID = ACTION_COLLECT;
		pstActionRes->Data.Collect.Flag = 1;
		pstActionRes->Data.Collect.MonID = 0;
		pstActionRes->Data.Collect.MonCurrHP = 0;

		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_COLLECT11);
		return -1;
	}

	if(pstPlayer->stOnline.State & CS_STAT_QIANXIN)
	{
		z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, STATUS_ID_QIANXIN, 1, 0);
	}

	if((CS_STAT_SIT|CS_STAT_GAOJI_SIT) & pstPlayer->stOnline.State)
	{
		player_sit_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
		z_player_stop(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, NULL);
	}

	iIndex	=	z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstCollectStart->MonID, &cType);
	if( -1==iIndex )
	{
		return -1;
	}

	switch(cType)
	{
		case OBJ_MONSTER:
			{
				Monster *pstMonCollectee;
				
				pstMonCollectee	=	z_id_monster(pstEnv->pstAppCtx, pstEnv, pstCollectStart->MonID);
				if (NULL == pstMonCollectee || MONSTER_DIE == pstMonCollectee->cStat || 0 >= pstMonCollectee->iHP
					|| pstMonCollectee->bSubType != MON_SUB_TYPE_COLLECT)
				{
					return -1;
				}
				
				return player_collect_mon_start(pstEnv, pstPlayer, pstMonCollectee);
			}
			break;
		case OBJ_ITEM:
			{
				DropItem *pstDropItem;

				pstDropItem = z_id_dropitem(pstEnv->pstAppCtx, pstEnv, pstCollectStart->MonID);
				if (NULL == pstDropItem)
				{
					return -1;
				}

				return player_collect_item_start(pstEnv, pstPlayer, pstDropItem);
			}
		default:
			break;
	}

	return 0;
}

int player_collect_break(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMonCollectee)
{
	Monster *pstMachine = NULL;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	if (0 == (CS_STAT_COLLECT & pstPlayer->stOnline.State))
	{
		return 0;
	}

	pstPlayer->stOnline.State &= ~CS_STAT_COLLECT;
	pstPlayer->stOnline.tCollectAward = 0;
	pstPlayer->stOnline.iCollectTarget = 0;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_COLLECT;
	pstActionRes->Data.Collect.Flag = 1;
	if (pstMonCollectee)
	{
		pstActionRes->Data.Collect.MonID = pstMonCollectee->iID;
		pstActionRes->Data.Collect.MonCurrHP = pstMonCollectee->iHP;
	}
	else
	{
		pstActionRes->Data.Collect.MonID = 0;
		pstActionRes->Data.Collect.MonCurrHP = 0;
	}

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);

	pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	if(pstMachine)
	{
		z_sendpkg_monview(pstEnv->pstAppCtx, pstEnv, pstMachine, &stPkg, 0);
	}
	else
	{
		z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	}
	
	return 0;
}

int player_collect_trace_mon(ZONESVRENV* pstEnv, Monster *pstMon, Player *pstPlayer)
{
	Player *pstPlayerTmp;
	ZoneTeam *pstTeam;
	int i;
	
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if (NULL == pstTeam)
	{
		if (MISC_FLAG_TRACE & pstPlayer->stOnline.cMiscFlag)
		{
			task_trace_mon(pstEnv, pstPlayer, pstMon);
		}
	}
	else
	{
		for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
		{
			pstPlayerTmp = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
			if (NULL == pstPlayerTmp || 0 > mon_trace_valid_player(pstMon, pstPlayerTmp))
			{
				continue;
			}

			if (MISC_FLAG_TRACE & pstPlayerTmp->stOnline.cMiscFlag)
			{
				task_trace_mon(pstEnv, pstPlayerTmp, pstMon);
			}
		}
	}

	return 0;
}

int player_collect_mon_award(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMon)
{
	MONSTERDEF *pstMonDef;
	MONCOLLECT *pstMonCollect;
	ROLEARM *pstRoleArm = NULL;
	ZoneAni stAni;
	AIEvt stEvt;
	int iDecHP = 1;
	int iCollectTime = 0, iMachineEnergyDec = 0;
	Monster *pstMachine = NULL;

	if (NULL == pstMon || MONSTER_DIE == pstMon->cStat || 0 >= pstMon->iHP || pstMon->bSubType != MON_SUB_TYPE_COLLECT)
	{
		return player_collect_break(pstEnv, pstPlayer, pstMon);
	}

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (NULL == pstMonDef )
	{
		return -1;
	}

	pstMonCollect = &pstMonDef->Collect;
	if(is_player_ride_on_machine(pstPlayer))
	{
		MONMACHINEDEF *pstMacDef;
		int iCurrEnergy;
		
		pstMachine = machine_get_by_player(pstEnv, pstPlayer);
		if(NULL == pstMachine)
		{
			return player_collect_break(pstEnv, pstPlayer, pstMon);
		}

		pstMacDef = z_find_mon_machine_def(pstEnv, pstMachine->iDefIdx);
		if (!pstMacDef)
		{
			return player_collect_break(pstEnv, pstPlayer, pstMon);
		}

		iCurrEnergy = get_machine_attr_val(pstEnv, pstMachine, MON_MACHINE_ATTR_ENERGY);
		if( iCurrEnergy <= 0 ||pstMacDef->CollcetEnergyDec > iCurrEnergy)
		{
			player_collect_break(pstEnv, pstPlayer, pstMon);
		}

		iMachineEnergyDec = pstMacDef->CollcetEnergyDec;
		iCollectTime = this_machine(pstEnv,pstMachine)->stMachineInfo.CollcetTime;
		//iDecHP = this_machine(pstEnv,pstMachine)->stMachineInfo.CollcetVal;
	}
	else
	{
		iCollectTime = pstMonCollect->CollectTime;
	}

	if (pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.tCollectAward < iCollectTime)
	{
		return -1;
	}
	
	pstPlayer->stOnline.tCollectAward = pstEnv->pstAppCtx->stCurr.tv_sec;

	//耐久度
	if (pstMonCollect->CollectArm > 0 && pstMonCollect->CollectArmDur > 0)
	{
		pstRoleArm = z_wear_get(&pstPlayer->stRoleData.Wear, pstMonCollect->CollectArm, -1);
		if (NULL == pstRoleArm || pstRoleArm->Dur < pstMonCollect->CollectArmDur) //不应该出现
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "collect arm err");
			return player_collect_break(pstEnv, pstPlayer, pstMon);
		}

		z_arm_dur_dec(pstEnv, pstPlayer, pstRoleArm, pstMonCollect->CollectArmDur);
		if (pstRoleArm->Dur < pstMonCollect->CollectArmDur)
		{
			//采死了就不在提示了
			if(pstMon->iHP > 1 || pstMonDef->Collect.NoDecHP )
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_COLLECT12);
			}
			
			player_collect_break(pstEnv, pstPlayer, pstMon);
		}
	}

	//消耗物品
	if (pstMonCollect->CollectItem > 0 && pstMonCollect->CollectItemDur > 0)
	{
		if (0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMonCollect->CollectItem, pstMonCollect->CollectItemDur, -1, NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_COLLECT))
		{
			return player_collect_break(pstEnv, pstPlayer, pstMon);
		}

		if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstMonCollect->CollectItem, pstMonCollect->CollectItemDur, -1, NULL,PACKAGE_DEC_NONE))
		{
			//采死了就不在提示了
			if(pstMon->iHP > 1 || pstMonDef->Collect.NoDecHP )
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_COLLECT13);
			}
			
			player_collect_break(pstEnv, pstPlayer, pstMon);
		}
	}

	if(is_mon_big_ore(pstMon->iDefIdx) ||
			is_mon_big_oil(pstMon->iDefIdx))
	{
		Monster *pstFactory;
		WORLDOREINFO *pstWorldOreInfo;
		
		pstWorldOreInfo = global_world_ore_get_byid(pstEnv, pstMon->iDefIdx);
		if(NULL == pstWorldOreInfo)
		{
			return -1;
		}

		pstFactory = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstWorldOreInfo->Factory.MemID);
		if(pstFactory && pstWorldOreInfo->Factory.BuildTime == 0)
		{
			z_mon_status_chg_hp(pstEnv->pstAppCtx, pstEnv, pstFactory, -23, 0);
			mon_factory_warning_byhit(pstEnv, pstFactory);
		}
	}

	//怪物减血
	if(pstMonDef->Collect.NoDecHP == 0)
	{
		if(pstMon->iHP < iDecHP)
		{
			iDecHP = pstMon->iHP;
		}

		pstMon->iHP -= iDecHP;
	}

	if(is_player_ride_on_machine(pstPlayer))
	{
		pstMachine = machine_get_by_player(pstEnv, pstPlayer);
		if(pstMachine)
		{
			z_machine_attr_chg(pstEnv, pstMachine, MON_MACHINE_ATTR_ENERGY, -iMachineEnergyDec);
		}
	}

	if(pstMonDef->MonsterID == XUKONG_ENERGY_ID)
	{
		player_collect_xukong_energy(pstEnv, pstPlayer);
	}
	
	//怪物援助
	stEvt.iEvtID = AI_EVT_BEATK;
	stEvt.stEvtDetail.stBeAtk.iHurt = 1;
	stEvt.stEvtDetail.stBeAtk.iSkillPressRate = -100;
	stEvt.stEvtDetail.stBeAtk.stAtker.iType = OBJ_PLAYER;
	stEvt.stEvtDetail.stBeAtk.stAtker.stObj.pstPlayer = pstPlayer;

	mon_ai_stat(pstEnv, pstMon, &stEvt);
		
	if (0 > mon_booty_assign_collect(pstEnv, pstPlayer, pstMon, pstMonDef, iDecHP))
	{
		player_collect_break(pstEnv, pstPlayer, pstMon);
	}
	else if (pstMonCollect->CollectOnce)
	{
		player_collect_break(pstEnv, pstPlayer, pstMon);
	}

		//生活技能
	if(pstMonCollect->CollectSecondarySkillType > 0 && 
		0 > z_collect_secondaryskill_check(pstEnv, pstPlayer, pstMonCollect))
	{
		player_collect_break(pstEnv, pstPlayer, pstMon);
	}

	if (0 >= pstMon->iHP)
	{
		player_collect_break(pstEnv, pstPlayer, pstMon);
		
		player_collect_trace_mon(pstEnv, pstMon, pstPlayer);

		pstMon->cFirstAtkerType = 0;
		pstMon->ullFirstAtkerWid = 0;
		pstMon->iFirstAtkerID = 0; //没有战利品
		pstMon->iFirstAtkerMemID = 0;

		if (!pstMon->cRonglu)
		{
			pstMon->szOwner[0] = 0;	
		}

		stAni.iType = OBJ_PLAYER;
		stAni.stObj.pstPlayer = pstPlayer;
		z_mon_die(pstEnv->pstAppCtx, pstEnv, pstMon, pstMonDef, &stAni, 1, 1);

		if(pstMonDef->BootyGroup[0].GroupID > 0)
		{
			mon_booty_group_assign_collect(pstEnv, pstPlayer, pstMon, pstMonDef);
		}
		
	}
	else if (CS_STAT_COLLECT & pstPlayer->stOnline.State) //通知采集剩余
	{
		CSPKG stPkg;
		CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
		
		pstActionRes->ID = pstPlayer->iMemID;
		pstActionRes->ActionID = ACTION_COLLECT;
		pstActionRes->Data.Collect.Flag = 0;
		pstActionRes->Data.Collect.MonID = pstMon->iID;
		pstActionRes->Data.Collect.MonCurrHP = pstMon->iHP;

		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		if(pstMachine)
		{
			z_sendpkg_monview(pstEnv->pstAppCtx, pstEnv, pstMachine, &stPkg, 0);
		}
		else
		{
			z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
		}
	}

	if((CS_STAT_COLLECT & pstPlayer->stOnline.State))
	{
		char szName[128];		
		
		
		stSEnv.pstMonster = pstMon;
		stSEnv.pstNpc = NULL;
		stSEnv.pstPworldInst = NULL;
		stSEnv.pstPlayer = pstPlayer;
		stSEnv.pstKiller = NULL;
		stSEnv.pstMapinst = NULL;
		
		snprintf(szName, sizeof(szName), "monster%d.mac", pstMonDef->MonsterID);
		szName[sizeof(szName) - 1] = 0;

		if (pstMon->cMonScriptFlag & MON_SCRIPT_PROC_COLLECT_END_MAP)
		{
			z_exec_script(pstEnv->pstAppCtx, pstEnv, NULL, szName, &pstMon->stScriptIdx, MON_SCRIPT_PROC_COLLECT_END);
		}
		//脚本中可能break掉采集
		if ((CS_STAT_COLLECT & pstPlayer->stOnline.State) && (pstMon->cMonScriptFlag & MON_SCRIPT_PROC_COLLECT_BEGIN_MAP))
		{
			pstMon->bDelayScript |= MON_SCRIPT_PROC_COLLECT_BEGIN;
			z_exec_script(pstEnv->pstAppCtx, pstEnv, NULL, szName, &pstMon->stScriptIdx, MON_SCRIPT_PROC_COLLECT_BEGIN);
		}
	}

		
	return 0;
}

int player_collect_item_award(ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstDropItem)
{
	ROLEGRID stDummy;
	ROLEGRID *pstRoleGrid = &pstDropItem->stObj.stBootyGrid.stGrid;

	if(!is_collect_drop_item(pstDropItem))
	{
		return player_collect_break(pstEnv, pstPlayer, NULL);
	}
	
	if (pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.tCollectAward < pstDropItem->stObj.stBootyGrid.bCollectTime)
	{
		return -1;
	}

	// OP LOG
	memcpy(&stDummy, pstRoleGrid, sizeof(stDummy));

	if ( 0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRoleGrid, 1,0) )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_NOSPACE);
		player_collect_break(pstEnv, pstPlayer, NULL);
		return -1;
	}
	package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRoleGrid, 1,0, IMPITEM_OPTYPE_COLLECT);

	//结束采集
	player_collect_break(pstEnv, pstPlayer, NULL);

	//清除物品
	z_item_clean(pstEnv, pstDropItem);

	//记录一下拾取日志
	z_role_pickitem_oplog( pstEnv, pstPlayer, &stDummy, 1, OP_PICK);
	
	return 0;
}

int player_collect_award(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iIndex;
	char cType;

	iIndex	=	z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.iCollectTarget, &cType);
	if( -1==iIndex )
	{
		player_collect_break(pstEnv, pstPlayer, NULL);
		return -1;
	}

	switch(cType)
	{
		case OBJ_MONSTER:
			{
				Monster *pstMonCollectee;
				
				pstMonCollectee	=	z_id_monster(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.iCollectTarget);
				if (NULL == pstMonCollectee || MONSTER_DIE == pstMonCollectee->cStat || 0 >= pstMonCollectee->iHP
					|| pstMonCollectee->bSubType != MON_SUB_TYPE_COLLECT)
				{
					return player_collect_break(pstEnv, pstPlayer, NULL);
				}
				
				return player_collect_mon_award(pstEnv, pstPlayer, pstMonCollectee);
			}
			break;
		case OBJ_ITEM:
			{
				DropItem *pstDropItem;

				pstDropItem = z_id_dropitem(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.iCollectTarget);
				if (NULL == pstDropItem)
				{
					return player_collect_break(pstEnv, pstPlayer, NULL);
				}

				return player_collect_item_award(pstEnv, pstPlayer, pstDropItem);
			}
		default:
			break;
	}

	return 0;
}

int player_collect_end(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	Monster *pstMon;

	if (0 == (CS_STAT_COLLECT & pstPlayer->stOnline.State))
	{
		return -1;
	}

	pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.iCollectTarget);

	player_collect_break(pstEnv, pstPlayer, pstMon);
	
	return 0;
}

int player_collect(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSCOLLECTCLT *pstCollect = &pstCsPkg->Body.CollectClt;

	switch(pstCollect->OpType)
	{
		case COLLECT_START:
			player_collect_start(pstEnv, pstPlayer, &pstCollect->CollectCltData.CollectStart);
			break;
		case COLLECT_END:
			player_collect_end(pstEnv, pstPlayer);
			break;
		default:
			return -1;
			break;
	}

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

int map_ore_position_rand_init(ZONEMAPINST *pstMapInst, int iNum)
{
	int i, j;

	g_iOrePositionRangeCurr = iNum;
	if(g_iOrePositionRangeCurr <= 0 || g_iOrePositionRangeCurr > 50)
	{
		return -1;
	}

	for(i=0; i<g_iOrePositionRangeCurr; i++)
	{
		g_aiOrePositionMap[i] = i;
	}

	for(i=pstMapInst->iOre-1; i>=0; i--)
	{
		for(j=g_iOrePositionRangeCurr-1; j>=0; j--)
		{
			if(pstMapInst->astOres[i].iOrePositionIdx == g_aiOrePositionMap[j])
			{
				g_iOrePositionRangeCurr--;

				if(g_iOrePositionRangeCurr != j)
				{
					g_aiOrePositionMap[j] = g_aiOrePositionMap[g_iOrePositionRangeCurr];
				}

				break;
			}
		}
	}

	return 0;
}

/* 原来的
int  map_ore_position_rand_get()
{
	int iRand, iGridIdx;

	if(g_iOrePositionRangeCurr <=0 ||  g_iOrePositionRangeCurr > 50)
		return -1;

	iRand = RAND1(g_iOrePositionRangeCurr);
	iGridIdx = g_aiOrePositionMap[iRand];

	g_iOrePositionRangeCurr--;
	if(iRand != g_iOrePositionRangeCurr)
	{
		g_aiOrePositionMap[iRand] = g_aiOrePositionMap[g_iOrePositionRangeCurr];
	}
	
	return iGridIdx;
}
*/

int  map_ore_position_rand_get(ZONESVRENV* pstEnv,MapIndex* pstMapIndex,int iMonID)
{
	if(g_iOrePositionRangeCurr <=0 ||  g_iOrePositionRangeCurr > 50)
		return -1;

	int iRand, iGridIdx;
	int j;
	int iIdx;
	MAPOREPOSITIONDEF *pstMapOrePositionDef;
	int aiFixOrePositionMap[50];
	int aiFixOrePositionIndexMap[50];
	int iaiFixNum=0;
	int iTempIndex = 0;
	
	for(j=g_iOrePositionRangeCurr-1; j>=0; j--)
	{
		iIdx = pstMapIndex->iOrePositionStart + g_aiOrePositionMap[j];
		if(iIdx >= pstEnv->pstMapObj->iMapOrePositionDef)
		{
			return -1;
		}

		pstMapOrePositionDef = (MAPOREPOSITIONDEF *)pstEnv->pstMapObj->sMapOrePositionDef[iIdx];
		if(pstMapOrePositionDef->MonsterID == iMonID)
		{
			aiFixOrePositionMap[iaiFixNum] = g_aiOrePositionMap[j];
			aiFixOrePositionIndexMap[iaiFixNum]=j;
			iaiFixNum++;
		}
	}

	if(iaiFixNum > 0)
	{
		iRand = RAND1(iaiFixNum);
		iGridIdx = aiFixOrePositionMap[iRand];
		iTempIndex = aiFixOrePositionIndexMap[iRand];

		g_iOrePositionRangeCurr--;
		if(iTempIndex != g_iOrePositionRangeCurr)
		{
			g_aiOrePositionMap[iTempIndex] = g_aiOrePositionMap[g_iOrePositionRangeCurr];
		}

		return iGridIdx;
	}

	return  -1;
}

MAPORELIST* map_ore_list_get_byid(MAPORELIST *pastOreList, int iNum, int iID)
{
	int i;
	
	for(i=0; i<iNum; i++)
	{
		if(pastOreList[i].MonsterID == iID)
		{
			return &pastOreList[i];
		}
	}

	return NULL;
}

int map_inst_ore_num_get(ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, int iID)
{
	int i;
	int iCount = 0;
	Monster *pstMon;

	for(i=0; i<pstMapInst->iOre; i++)
	{
		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstMapInst->astOres[i].iOreID);
		if(pstMon && pstMon->iDefIdx == iID)
		{
			iCount++;
		}
	}

	return iCount;
}

int map_ore_rand_pos_get(ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, MapIndex* pstMapIndex, RESPOS *pstPos, int iMonID)
{
	int iIdx;
	int iPositionIdx;
	MAPOREPOSITIONDEF *pstMapOrePositionDef;
	
//	iPositionIdx = map_ore_position_rand_get();   原来的
	iPositionIdx = map_ore_position_rand_get(pstEnv,pstMapIndex,iMonID);
	if(iPositionIdx < 0)
	{
		return -1;
	}
	
	iIdx = pstMapIndex->iOrePositionStart + iPositionIdx;
	if(iIdx >= pstEnv->pstMapObj->iMapOrePositionDef)
	{
		return -1;
	}

	pstMapOrePositionDef = (MAPOREPOSITIONDEF *)pstEnv->pstMapObj->sMapOrePositionDef[iIdx];
	z_get_ran_pos(pstEnv, pstMapInst, pstMapIndex, &pstMapOrePositionDef->Pos,
		&pstMapOrePositionDef->Pos, pstMapOrePositionDef->Radius, 0, pstPos);

	return iPositionIdx;
}

int map_inst_ore_add(ZONEMAPINST* pstMapInst, int iID, int iPositionIdx)
{
	if(pstMapInst->iOre >= MAX_MAP_ORE)
	{
		memmove(&pstMapInst->astOres[0], &pstMapInst->astOres[1], sizeof(pstMapInst->astOres[0])*(MAX_MAP_ORE-1));
		pstMapInst->astOres[MAX_MAP_ORE-1].iOreID = iID;
		pstMapInst->astOres[MAX_MAP_ORE-1].iOrePositionIdx = iPositionIdx;
	}
	else
	{
		pstMapInst->astOres[pstMapInst->iOre].iOreID = iID;
		pstMapInst->astOres[pstMapInst->iOre].iOrePositionIdx = iPositionIdx;
		pstMapInst->iOre++;
	}
	return 0;
}

int map_inst_ore_del(ZONEMAPINST* pstMapInst, Monster *pstMon)
{
	int i;

	for(i=pstMapInst->iOre-1; i>=0; i--)
	{
		if(pstMapInst->astOres[i].iOreID == pstMon->iID)
		{
			pstMapInst->iOre--;
			if(i != pstMapInst->iOre)
			{
				memcpy(&pstMapInst->astOres[i], 
					&pstMapInst->astOres[pstMapInst->iOre], sizeof(pstMapInst->astOres[0]));
			}
		}
	}

	return 0;
}

int map_new_ore_monster(ZONESVRENV* pstEnv, MapIndex* pstMapIndex, ZONEMAPINST* pstMapInst, int iMonID, int iNum)
{		
	int i, iPositionIdx;
	int iRet = 0;
	Monster stMonster;
	Monster* pstFind;
	MONSTERDEF *pstMonDef;
	RESPOS stPos;

	memset(&stMonster, 0, sizeof(stMonster));
		
	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.bDirection= RAND1(180);
	
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos=	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;
	stMonster.tRefresh	=	pstEnv->pstAppCtx->stCurr.tv_sec;

	for(i=0; i<iNum; i++)
	{
		if(0 < map_ore_if_fix_mon_rand(pstEnv,pstMapIndex,iMonID))
		{
			iPositionIdx = map_ore_rand_pos_get(pstEnv, pstMapInst, pstMapIndex, &stPos,iMonID);
		}
		else
		{
			iPositionIdx = map_ore_rand_pos_get(pstEnv, pstMapInst, pstMapIndex, &stPos,0);
		}
		
		if(0 > iPositionIdx)
		{
			return -1;
		}

		memcpy(&stMonster.stInitPos, &stPos, sizeof(stMonster.stInitPos));
		memcpy(&stMonster.stCurrPos, &stPos, sizeof(stMonster.stCurrPos));
		
		pstMonDef = z_get_mon_def(pstEnv, &stMonster);
		if (!pstMonDef)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "map monster %d no def", stMonster.iDefIdx);
			return -1;
		}

		pstFind =	z_new_monster(pstEnv->pstAppCtx, pstEnv, &stMonster, pstMonDef);
		if( !pstFind )
			return -1;

		
		pstFind->cRefresh = 0;
		pstFind->cFirstBorn = 1;
		if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstFind, NULL))
		{
			tmempool_free(pstEnv->pstMonsterPool, pstFind->iMIndex);
			return -1;
		}

		map_inst_ore_add(pstMapInst, pstFind->iID, iPositionIdx);
		
		pstFind->cFirstBorn = 0;
	}
	
	return iRet;
}

int map_refresh_ore(ZONESVRENV* pstEnv, MapIndex* pstMapIndex, ZONEMAPINST* pstMapInst)
{
	int i, iOreNum;
	MAPOREDEF *pstMapOreDef;

	if (pstMapIndex->stMapDef.MapLine > 0)
	{
		pstMapOreDef = z_find_map_ore_def(pstEnv, pstMapIndex->stMapDef.MapID);
	}
	else
	{
		pstMapOreDef = z_find_map_ore_def(pstEnv, pstMapInst->iMapInstID&0xffff);
	}
	
	if(NULL == pstMapOreDef)
	{
		return -1;
	}

	//初始化可选坐标
	map_ore_position_rand_init(pstMapInst, pstMapIndex->iOrePositionCount);

	for(i=0; i<MAX_MAP_ORE_LIST; i++)
	{
		if(pstMapOreDef->OreList[i].MonsterID == 0)
		{
			continue;
		}

		if(pstMapInst->tRefreshOreTime[i] > pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			continue;
		}

		iOreNum = pstMapOreDef->OreList[i].Num -map_inst_ore_num_get(pstEnv, pstMapInst, pstMapOreDef->OreList[i].MonsterID);
		if(iOreNum > 0)
		{
			map_new_ore_monster(pstEnv, pstMapIndex, pstMapInst, pstMapOreDef->OreList[i].MonsterID, iOreNum);
		}

		if(pstMapOreDef->OreList[i].RefreshTime > 0)
		{
			pstMapInst->tRefreshOreTime[i] = pstEnv->pstAppCtx->stCurr.tv_sec + pstMapOreDef->OreList[i].RefreshTime;
		}
		else
		{
			pstMapInst->tRefreshOreTime[i] = pstEnv->pstAppCtx->stCurr.tv_sec + 600;
		}
	}

	return 0;
}

int player_map_ore_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	Monster *pstMon;
	CSPKG stPkg;
	CSMAPSVR *pstMapSvr = &stPkg.Body.MapSvr;
	CSMAPORELIST *pstMapOreList = &pstMapSvr->Data.OreList;
	MAPOREDEF *pstMapOreDef;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	if(pstMapIdx->stMapDef.MapLine > 0)
	{
		pstMapOreDef = z_find_map_ore_def(pstEnv, pstMapIdx->stMapDef.MapID);
	}
	else
	{
		pstMapOreDef = z_find_map_ore_def(pstEnv, pstMapInst->iMapInstID&0xffff);
	}
	
	if(NULL == pstMapOreDef || pstMapOreDef->MachineView == 0)
	{
		return -1;
	}

	pstMapSvr->SvrOp = MAP_SVR_ORE_INFO;
	pstMapOreList->OreNum = 0;
	for(i=0; i<pstMapInst->iOre; i++)
	{
		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstMapInst->astOres[i].iOreID);
		if(!pstMon)
		{	
			continue;
		}

		pstMapOreList->OreInfo[pstMapOreList->OreNum].ID = pstMon->iDefIdx;
		pstMapOreList->OreInfo[pstMapOreList->OreNum].MemID = pstMon->iID;
		pstMapOreList->OreInfo[pstMapOreList->OreNum].Map = GET_MAPID(pstMon->stMap.iID);
		pstMapOreList->OreInfo[pstMapOreList->OreNum].Pos = pstMon->stCurrPos;
		pstMapOreList->OreNum++;
	}

	if(pstMapOreList->OreNum > 0)
	{
		Z_CSHEAD_INIT(&stPkg.Head, MAP_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	return 0;
}

int player_map_bigore_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	Monster *pstMon;
	CSPKG stPkg;
	CSMAPSVR *pstMapSvr = &stPkg.Body.MapSvr;
	CSMAPBIGORELIST *pstMapOreList = &pstMapSvr->Data.BigOreList;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;

	if(!is_fini_global(pstEnv))
	{
		return -1;
	}
	
	pstMapSvr->SvrOp = MAP_SVR_BIG_ORE_INFO;
	pstMapOreList->OreNum = 0;
	for(i=MAX_WORLD_ORE-1; i>=0; i--)
	{
		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstWorldLimit->Ores[i].Ore.MemID);
		if(pstMon && pstWorldLimit->Ores[i].Ore.DefID == pstMon->iDefIdx)
		{
			pstMapOreList->OreInfo[pstMapOreList->OreNum].ID = pstMon->iDefIdx;
			pstMapOreList->OreInfo[pstMapOreList->OreNum].MemID = pstMon->iID;
			pstMapOreList->OreInfo[pstMapOreList->OreNum].Map = GET_MAPID(pstMon->stMap.iID);
			pstMapOreList->OreInfo[pstMapOreList->OreNum].Pos = pstMon->stCurrPos;
			pstMapOreList->OreNum++;
		}
	}

	if(pstMapOreList->OreNum > 0)
	{
		Z_CSHEAD_INIT(&stPkg.Head, MAP_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	return 0;
}

int world_big_ore_info_broadcast(ZONESVRENV *pstEnv)
{
	int i;
	Monster *pstMon;
	CSPKG stPkg;
	CSMAPSVR *pstMapSvr = &stPkg.Body.MapSvr;
	CSMAPBIGORELIST *pstMapOreList = &pstMapSvr->Data.BigOreList;
	WORLDLIMIT *pstWorldLimit = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldLimit;

	if(!is_fini_global(pstEnv))
	{
		return -1;
	}

	pstMapSvr->SvrOp = MAP_SVR_BIG_ORE_INFO;
	pstMapOreList->OreNum = 0;
	for(i=MAX_WORLD_ORE-1; i>=0; i--)
	{
		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstWorldLimit->Ores[i].Ore.MemID);
		if(pstMon && pstWorldLimit->Ores[i].Ore.DefID == pstMon->iDefIdx)
		{
			pstMapOreList->OreInfo[pstMapOreList->OreNum].ID = pstMon->iDefIdx;
			pstMapOreList->OreInfo[pstMapOreList->OreNum].MemID = pstMon->iID;
			pstMapOreList->OreInfo[pstMapOreList->OreNum].Map = GET_MAPID(pstMon->stMap.iID);
			pstMapOreList->OreInfo[pstMapOreList->OreNum].Pos = pstMon->stCurrPos;
			pstMapOreList->OreNum++;
		}
	}

	Z_CSHEAD_INIT(&stPkg.Head, MAP_SVR);
	return z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
}

int  map_ore_if_fix_mon_rand(ZONESVRENV* pstEnv,MapIndex* pstMapIndex,int iMonID)
{
	int j;
	int iIdx;
	MAPOREPOSITIONDEF *pstMapOrePositionDef;

	if(g_iOrePositionRangeCurr <=0 ||  g_iOrePositionRangeCurr > 50)
		return -1;
	
	for(j=g_iOrePositionRangeCurr-1; j>=0; j--)
	{
		iIdx = pstMapIndex->iOrePositionStart + g_aiOrePositionMap[j];
		if(iIdx >= pstEnv->pstMapObj->iMapOrePositionDef)
		{
			return -1;
		}

		pstMapOrePositionDef = (MAPOREPOSITIONDEF *)pstEnv->pstMapObj->sMapOrePositionDef[iIdx];
		if(pstMapOrePositionDef->MonsterID == iMonID)
		{
			return 1;
		}
	}

	return -1;
}
