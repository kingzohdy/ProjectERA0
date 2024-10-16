/*
**  @file $RCSfile: zone_tmp.c,v $
**  general description of this module
**  $Id: zone_tmp.c,v 1.99 2013/12/18 09:20:42 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2013/12/18 09:20:42 $
**  @version $Revision: 1.99 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_tmp.h"
#include "zone_skill.h"
#include "zone_map.h"
#include "zone_package.h"
#include "zone_clt.h"
#include "zone_attr.h"
#include "zone_move.h"
#include "mapmask.h"
#include "zone_player.h"
#include "zone_status.h"
#include "zone_ai.h"
#include "zone_task.h"
#include "zone_combat.h"
#include "zone_clan_city.h"
#include "zone_err.h"

#define TMP_LOGOUT_TIME 900

int z_safe_item_entermap(ZONESVRENV* pstEnv, DropItem *pstDropItem)
{
	int iRet;

	iRet	=	z_item_enter_map(pstEnv, pstDropItem);

	switch(iRet)
	{
		case -1:
		case -2:
			tlog_error(pstEnv->pstLogCat, 0, 0, "item %d %d enter map  %d error, ret %d", pstDropItem->iID, pstDropItem->iDefIdx, 
						pstDropItem->stMap.iID, iRet);
			z_id_clean(pstEnv, pstDropItem->iID);
			tmempool_free(pstEnv->pstItemPool, pstDropItem->iMIndex);
			break;
		case -3:
			tlog_error(pstEnv->pstLogCat, 0, 0, "item %d %d enter map  %d error, ret %d", pstDropItem->iID, pstDropItem->iDefIdx, 
						pstDropItem->stMap.iID, iRet);
			z_id_clean(pstEnv,  pstDropItem->iID);
			tmempool_free(pstEnv->pstItemPool, pstDropItem->iMIndex);
			
			break;
		default:
			break;
	}

	return iRet;
}

#if defined(ZONE_ID_TRACKED)
int z_item_refresh_tracked(ZONESVRENV *pstEnv, DropItem *pstDropItem,
                           int iEnergeFlag, const char *pszFile, int iLine)
#else
int z_item_refresh(ZONESVRENV* pstEnv, DropItem *pstDropItem,int iEnergeFlag)
#endif
{
	ITEMDEF *pstItemDef;

	pstItemDef = z_find_itemdef(pstEnv, pstDropItem->iDefIdx);
	if (NULL == pstItemDef)
	{
		return -1;
	}
	
	pstDropItem->iID = z_get_mem_id(pstEnv->pstAppCtx, pstEnv, pstDropItem->iMIndex, OBJ_ITEM);
#if defined(ZONE_ID_TRACKED)
	tlog_info(pstEnv->pstLogCat, 0, 0,
	          "Allocate id, From:%s(%d): Type=DropItem, DefID=%d, MemID=%d, ID=%d",
	          pszFile, iLine, pstDropItem->iDefIdx, pstDropItem->iMIndex,
	          pstDropItem->iID);
#endif
	pstDropItem->cStat = DROP_ITEM_STAY;

	TV_CLONE(pstDropItem->stDropTime, pstEnv->pstAppCtx->stCurr);
	
	if (pstItemDef->ItemTmp.MachineEnerge > 0 &&
		iEnergeFlag == 0 )
	{
		pstDropItem->stObj.stTmpItem.iEnerge = pstItemDef->ItemTmp.MachineEnerge;
	}

	return z_safe_item_entermap(pstEnv, pstDropItem);
}

int tmp_item_create(ZONESVRENV* pstEnv,  Player *pstPlayer,int iItemID, ZONEIDX *pstMapIdx, RESPOS *pstPos, 
						int iEnerge, int iRefresh, int iStay, int iDir,
						int iSkill_1_collectnum,int iSkill_2_collectnum,
						DropItem **pstItem)
{
	TMEMBLOCK *pstMemBlock;
	DropItem *pstDropItem;
	int iIndex;
	int iRet;
	ITEMDEF *pstItemDef;
	
	pstItemDef = z_find_itemdef(pstEnv, iItemID);
	if (NULL == pstItemDef ||ITEM_TMP_NO == pstItemDef->ItemTmp.Type)
	{
		return -1;
	}
	
	iIndex = tmempool_alloc(pstEnv->pstItemPool);
	if (0 > iIndex)
	{
		return -1;
	}

	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstItemPool, iIndex);
	pstDropItem = (DropItem *)pstMemBlock->szData;
	memset(pstDropItem,0,sizeof(DropItem));
	pstDropItem->cType = DROP_ITEM_TMP;
	pstDropItem->cStat = DROP_ITEM_NOSTAY;
	pstDropItem->unRefreshTime = iRefresh;
	pstDropItem->iStayTime = iStay;
	pstDropItem->stObj.stTmpItem.iEnerge = 0;
	pstDropItem->iDefIdx = iItemID;
	pstDropItem->iID = 0;
	pstDropItem->stMap = *pstMapIdx;
	pstDropItem->stPos = *pstPos;
	pstDropItem->iMIndex = iIndex;
	pstDropItem->ullTeamID = 0;
	pstDropItem->bDir = iDir;
	pstDropItem->stObj.stTmpItem.iEnerge = iEnerge;
	if( pstPlayer != NULL )
	{
		pstDropItem->stObj.stTmpItem.iPlayerMemID = pstPlayer->iMemID;
		STRNCPY(pstDropItem->stObj.stTmpItem.OwnerName, pstPlayer->stRoleData.RoleName,
			CCH(pstDropItem->stObj.stTmpItem.OwnerName) );
	}

	if (0 > (iRet = z_item_refresh(pstEnv, pstDropItem,1)))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "item refresh fail ret %d, item %d %d, map %d, pos (%d,%d)", iRet, pstDropItem->iID,
					pstDropItem->iDefIdx, pstDropItem->stMap.iID, pstDropItem->stPos.X, pstDropItem->stPos.Y);
	}

	pstDropItem->stObj.stTmpItem.iSkill_1_collectnum = iSkill_1_collectnum;
	pstDropItem->stObj.stTmpItem.iSkill_2_collectnum = iSkill_2_collectnum;
	if( pstItem != NULL )
	{
		*pstItem = pstDropItem;
	}
	return 0;
}

//返回值: -1 不合法，0 不改变，1 替换
int tmp_item_replace_check(ITEMDEF *pstItemDefOld, ITEMDEF *pstItemDef)
{
	int iTmpType = pstItemDef->ItemTmp.Type;

	if (NULL == pstItemDefOld)
	{
		return 1;
	}
	
	switch (pstItemDefOld->ItemTmp.Type)
	{
		case ITEM_TMP_NO:
		case ITEM_TMP_SUPPLY:	
			return 1;
			break;
			
		case ITEM_TMP_ARM_AMM:
		case ITEM_TMP_MACHINE_ENERGE:
			if (ITEM_TMP_ARM_AMM > iTmpType)
			{
				return -1;
			}
			else if (pstItemDefOld->ItemTmp.Type == iTmpType)
			{
				return 0;
			}
			else
			{
				return 1;
			}
			break;

		case ITEM_TMP_ITEM:
			if (ITEM_TMP_ITEM > iTmpType)
			{
				return -1;
			}
			else if (pstItemDefOld->ItemID == pstItemDef->ItemID)
			{
				return 0;
			}
			else
			{
				return 1;
			}
			break;

		case ITEM_TMP_ARM:
			if (ITEM_TMP_ARM_AMM > iTmpType)
			{
				return -1;
			}
			else if (pstItemDefOld->ItemID == pstItemDef->ItemID || ITEM_TMP_ARM_AMM == iTmpType)
			{
				return 0;
			}
			else
			{
				return 1;
			}
			break;

		case ITEM_TMP_MACHINE:
			if (ITEM_TMP_MACHINE_ENERGE == iTmpType)
			{
				return 0;
			}
			else
			{
				return -1;
			}
			break;
			
		default:
			return -1;
			break;
	}
		
	return 0;
}

int tmp_resource_chg(ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstDropItem, ITEMDEF *pstItemDefOld, ITEMDEF *pstItemDef,int *piTmpEnerge)
{
	PlayerChagAttrVal stChgVal;
	int iSkillCollectNum = 0;
	ZoneSkill *pstZoneSkill;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;
	int iTmpEnerge = 0;

	stChgVal.AttrNum = 0;
		
	if (ITEM_TMP_MACHINE != pstItemDefOld->ItemTmp.Type) //改变积累数
	{
		if( pstDropItem->stObj.stTmpItem.iSkill_1_collectnum <= 0 &&
			pstDropItem->stObj.stTmpItem.iSkill_2_collectnum <= 0 )
		{
			iSkillCollectNum = pstItemDef->ItemTmp.SkillDefaultCollectNum;
			iSkillCollectNum += genius_add_tmpitem_collectnum(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstItemDef);
		}
		
		if (pstItemDefOld->ItemTmp.SkillID_1 > 0)
		{
			if( pstDropItem->stObj.stTmpItem.iSkill_1_collectnum > 0 )
			{
				iSkillCollectNum = pstDropItem->stObj.stTmpItem.iSkill_1_collectnum;
				iSkillCollectNum += genius_add_tmpitem_collectnum(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstItemDef);
			}
			
			pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstItemDefOld->ItemTmp.SkillID_1);
			if (NULL == pstZoneSkill)
			{
				tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player %d zoneskill %d null", pstPlayer->stRoleData.Uin,
							pstItemDefOld->ItemTmp.SkillID_1);
				return -1;
			}

			if (pstZoneSkill->stSkillData.CollectNum < pstZoneSkill->stSkillData.CollectNumMax)
			{
				pstZoneSkill->stSkillData.CollectNum += iSkillCollectNum;
				if (pstZoneSkill->stSkillData.CollectNum > pstZoneSkill->stSkillData.CollectNumMax)
				{
					pstZoneSkill->stSkillData.CollectNum = pstZoneSkill->stSkillData.CollectNumMax;
				}

				stChgVal.AttrList[stChgVal.AttrNum].AttrID = ATTR_ID_COLLECTNUM;
				stChgVal.AttrList[stChgVal.AttrNum].Value.CollectNum.SkillID = pstZoneSkill->stSkillData.SkillID;
				stChgVal.AttrList[stChgVal.AttrNum].Value.CollectNum.CollectNum = pstZoneSkill->stSkillData.CollectNum;
				stChgVal.AttrNum++;
			}

			if (pstItemDefOld->ItemTmp.Type == ITEM_TMP_MACHINE_ENERGE)
			{
				pstRoleTmpSys->TmpEnerge += iSkillCollectNum;
				if ((int)pstRoleTmpSys->TmpEnerge > pstZoneSkill->stSkillData.CollectNumMax)
				{
					pstRoleTmpSys->TmpEnerge = pstZoneSkill->stSkillData.CollectNumMax;
				}
			}
		}

		if (pstItemDefOld->ItemTmp.SkillID_2 > 0)
		{
			if( pstDropItem->stObj.stTmpItem.iSkill_2_collectnum > 0 )
			{
				iSkillCollectNum = pstDropItem->stObj.stTmpItem.iSkill_2_collectnum;
				iSkillCollectNum += genius_add_tmpitem_collectnum(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstItemDef);
			}
			
			pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstItemDefOld->ItemTmp.SkillID_2);
			if (NULL == pstZoneSkill)
			{
				tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player %d zoneskill %d null", pstPlayer->stRoleData.Uin,
							pstItemDefOld->ItemTmp.SkillID_2);
				return -1;
			}

			if (pstZoneSkill->stSkillData.CollectNum < pstZoneSkill->stSkillData.CollectNumMax)
			{
				pstZoneSkill->stSkillData.CollectNum += iSkillCollectNum;
				if (pstZoneSkill->stSkillData.CollectNum > pstZoneSkill->stSkillData.CollectNumMax)
				{
					pstZoneSkill->stSkillData.CollectNum = pstZoneSkill->stSkillData.CollectNumMax;
				}

				stChgVal.AttrList[stChgVal.AttrNum].AttrID = ATTR_ID_COLLECTNUM;
				stChgVal.AttrList[stChgVal.AttrNum].Value.CollectNum.SkillID = pstZoneSkill->stSkillData.SkillID;
				stChgVal.AttrList[stChgVal.AttrNum].Value.CollectNum.CollectNum = pstZoneSkill->stSkillData.CollectNum;
				stChgVal.AttrNum++;
			}
		}
	}
	else //改变能量
	{
		if (pstRoleTmpSys->TmpEnerge < pstItemDefOld->ItemTmp.MachineEnerge)
		{
			if (pstItemDefOld == pstItemDef)
			{
				pstRoleTmpSys->TmpEnerge += pstDropItem->stObj.stTmpItem.iEnerge;
				//天赋对能量的影响
				iTmpEnerge = genius_add_tmpitem_energy(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstItemDef);

				//没有过渡消耗才能加天赋能量 
				//if(pstDropItem->stObj.stTmpItem.iEnerge > 0)
				//{
					pstRoleTmpSys->TmpEnerge += iTmpEnerge;
				//}
			}
			else
			{
				pstRoleTmpSys->TmpEnerge += pstItemDef->ItemTmp.SkillDefaultCollectNum;
			}
			
			if (pstRoleTmpSys->TmpEnerge > (pstItemDefOld->ItemTmp.MachineEnerge + iTmpEnerge))
			{
				pstRoleTmpSys->TmpEnerge = (pstItemDefOld->ItemTmp.MachineEnerge + iTmpEnerge );
			}

			stChgVal.AttrList[stChgVal.AttrNum].AttrID = ATTR_ID_TMPENERGE;
			if( pstRoleTmpSys->TmpEnerge > 0)
			{
				stChgVal.AttrList[stChgVal.AttrNum].Value.Energe = pstRoleTmpSys->TmpEnerge;
			}
			else 
			{
				stChgVal.AttrList[stChgVal.AttrNum].Value.Energe = 0;
			}
			stChgVal.AttrNum++;
		}
	}

	if (stChgVal.AttrNum > 0)
	{
		z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);
		if(iTmpEnerge > 0 && piTmpEnerge)
		{
			*piTmpEnerge = iTmpEnerge+pstItemDefOld->ItemTmp.MachineEnerge;
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

int tmp_item_add(ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstDropItem, ITEMDEF *pstItemDef)
{
	int iRet;
	unsigned short TmpLevel = 0;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	ITEMDEF *pstItemDefOld = NULL;
	ZONEPWORLDINST *pstPworldInst;
	ZONEMAPINST *pstMapInst;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;
	PATROLDEF *pstPatrolDef = NULL;
	int iPatrolIdx = 0;
	int iTmpEnerge = 0;

	if( pstDropItem->stObj.stTmpItem.iPlayerMemID != 0 && 
		pstDropItem->stObj.stTmpItem.iPlayerMemID != pstPlayer->iMemID )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TMP101);	
		return -1;
	}

	if (0 > check_replace_skill(pstEnv, pstPlayer, REPLACE_SKILL_OP_TMP))
	{
		return -1;
	}
	
	if (0 < pstRoleTmpSys->TmpItemID)
	{
		pstItemDefOld = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
	}
	
	if(!pstItemDefOld)
	{
		iRet = tmp_item_replace_check(NULL, pstItemDef);
	}
	else
	{
		iRet = tmp_item_replace_check(pstItemDefOld, pstItemDef);
	}

	
	if (0 > iRet)
	{
		return -1;
	}
	else if (0 == iRet) //保持
	{
		if(0 > tmp_resource_chg(pstEnv, pstPlayer, pstDropItem, pstItemDefOld, pstItemDef,NULL))
		{
			return -1;
		}
	}
	else //替换
	{
		/* 替换之前把原来的临时道具先drop下 */
		if (pstItemDefOld && pstItemDefOld->ItemTmp.Type >= ITEM_TMP_ITEM)
		{
			tmp_item_drop(pstEnv,pstPlayer,1);
		}

		if (pstItemDefOld)
		{
			if (pstItemDefOld->ItemTmp.SkillID_1 > 0)
			{
				z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDefOld->ItemTmp.SkillID_1, 1);
			}

			if (pstItemDefOld->ItemTmp.SkillID_2 > 0)
			{
				z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDefOld->ItemTmp.SkillID_2, 1);
			}
		}

		//调整临时物品(道具、装备、机械)在动态副本中的等级
		if (pstItemDef->ItemTmp.Type >= ITEM_TMP_ITEM)
		{
			pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstDropItem->stMap);
			if (pstMapInst)
			{
				pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
				if (pstPworldInst && pstPworldInst->DynType)
				{
					TmpLevel = pstPworldInst->TmpLevel;
				}
			}
		}
		
		pstRoleTmpSys->TmpItemID = pstDropItem->iDefIdx;
		pstRoleTmpSys->Dir = pstDropItem->bDir;
		pstRoleTmpSys->StatusID = pstDropItem->stObj.stTmpItem.iStatus;
		pstRoleTmpSys->PatrolID = pstDropItem->stObj.stTmpItem.iPatrolID;
		pstRoleTmpSys->OwerID = pstDropItem->stObj.stTmpItem.iPlayerMemID;
		pstRoleTmpSys->TmpItemFlag = pstDropItem->iTmpItemFlag;
		
		if (pstItemDefOld == NULL ||  pstItemDefOld->ItemTmp.Type != ITEM_TMP_MACHINE_ENERGE || pstItemDef->ItemTmp.Type != ITEM_TMP_MACHINE)
		{
			pstRoleTmpSys->TmpEnerge = 0;
		}

		if (pstItemDef->ItemTmp.SkillID_1 > 0)
		{
			if (TmpLevel)
			{
				z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstItemDef->ItemTmp.SkillID_1, TmpLevel, 1);
			}
			else
			{
				z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstItemDef->ItemTmp.SkillID_1, pstItemDef->ItemTmp.SkillLevel_1, 1);
			}
		}

		if (pstItemDef->ItemTmp.SkillID_2 > 0)
		{
			if (TmpLevel)
			{
				z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstItemDef->ItemTmp.SkillID_2, TmpLevel, 1);
			}
			else
			{
				z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstItemDef->ItemTmp.SkillID_2, pstItemDef->ItemTmp.SkillLevel_2, 1);
			}
		}

		tmp_resource_chg(pstEnv, pstPlayer, pstDropItem, pstItemDef, pstItemDef,&iTmpEnerge);
				
		if (pstItemDef->ItemTmp.SpeedMul !=0 || pstItemDef->ItemTmp.SpeedAdd != 0 ||
			(pstItemDefOld && (pstItemDefOld->ItemTmp.SpeedMul !=0 || pstItemDefOld->ItemTmp.SpeedAdd != 0)))
		{
			z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}

		if (ITEM_TMP_MACHINE == pstItemDef->ItemTmp.Type)
		{
			pstPlayer->stOnline.bDir = pstDropItem->bDir;
			z_player_chgpos(pstEnv->pstAppCtx, pstEnv, pstPlayer, &pstDropItem->stPos);
		}
		// 如果有状态,需要加临时机械的状态
		if(pstDropItem->stObj.stTmpItem.iStatus > 0 )
		{
			z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstDropItem->stObj.stTmpItem.iStatus, 
				pstDropItem->stObj.stTmpItem.iStatusLevel, pstDropItem->stObj.stTmpItem.iLast, 1, 0);
		}

		// 任务跟踪
		if(pstPlayer->stOnline.cMiscFlag & MISC_FLAG_OWNED_TMP_ITEM_TRACE)
		{
			task_player_trace(pstEnv, pstPlayer, TASK_OBJ_OWNED_TMP_ITEM);
		}
		
		//设置玩家的路径
		if( pstDropItem->stObj.stTmpItem.iPatrolID > 0 )
		{
			pstPatrolDef = z_find_patroldef(pstEnv, pstDropItem->stObj.stTmpItem.iPatrolID, &iPatrolIdx);
			if (NULL != pstPatrolDef)
			{
				// 把这个数据先发下
				pstActionRes->ID = pstPlayer->iMemID;
				pstActionRes->ActionID = ACTION_TMP;
				pstActionRes->Data.TmpItem.TmpItemID = pstRoleTmpSys->TmpItemID;
				pstActionRes->Data.TmpItem.OperType = 0;
				pstActionRes->Data.TmpItem.OperType |= TMPITEM_ON_PATRONID;
				pstActionRes->Data.TmpItem.MemIndex = pstDropItem->iID;
				Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
				z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);	
				// 然后发走路
				z_svr_player_move_by_patroldef(pstEnv,  pstPlayer,  pstPatrolDef->PatrolPos,MAX_PATROL_POINT);
				return 0;
			}
		}
				
	}

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_TMP;
	pstActionRes->Data.TmpItem.TmpItemID = pstRoleTmpSys->TmpItemID;
	pstActionRes->Data.TmpItem.OperType = 0;
	pstActionRes->Data.TmpItem.MemIndex = pstDropItem->iID;
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);	

	if(iTmpEnerge>0)
	{
		tmp_tmpenerge_max_action(pstEnv,pstPlayer,iTmpEnerge);
	}

	if (pstItemDef)
	{
		RESULT* pstResult = z_item_get_result(pstItemDef,RESULT_BEACH_EXP_MUL);
		if (pstResult)
		{
			BEACHINFO *pstBeachInfo = &pstPlayer->stRoleData.MiscInfo.BeachInfo;
			if (BEACH_EXP_HIGH <= pstBeachInfo->Sec)
			{
				player_beach_low_exp_ui_to_clt(pstEnv, pstPlayer);
			}
		}
	}
	
	return 0;
}

int tmp_item_del(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ITEMDEF *pstItemDef;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;

	if (0 == pstRoleTmpSys->TmpItemID)
	{
		return 0;
	}

	pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
	if (pstItemDef && pstItemDef->ItemTmp.SkillID_1 > 0)
	{
		z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDef->ItemTmp.SkillID_1, 1);
	}

	if (pstItemDef && pstItemDef->ItemTmp.SkillID_2 > 0)
	{
		z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDef->ItemTmp.SkillID_2, 1);
	}

	// 删除下玩家状态
	if( pstRoleTmpSys->StatusID > 0 )
	{
		z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRoleTmpSys->StatusID, 1, 0);
	}
	pstRoleTmpSys->TmpItemID = 0;
	pstRoleTmpSys->TmpEnerge = 0;
	pstRoleTmpSys->StatusID = 0;
	
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_TMP;
	pstActionRes->Data.TmpItem.TmpItemID = pstRoleTmpSys->TmpItemID;
	pstActionRes->Data.TmpItem.OperType = 0;	
	pstActionRes->Data.TmpItem.MemIndex = 0;
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);	

	if (pstItemDef && (pstItemDef->ItemTmp.SpeedMul !=0 || pstItemDef->ItemTmp.SpeedAdd != 0) )
	{
		z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	// 任务跟踪
	if(pstPlayer->stOnline.cMiscFlag & MISC_FLAG_OWNED_TMP_ITEM_TRACE)
	{
		task_player_trace(pstEnv, pstPlayer, TASK_OBJ_OWNED_TMP_ITEM);
	}

	if( pstRoleTmpSys->PatrolID  >0 )
	{
		PATROLDEF *pstPatrolDef = z_find_patroldef(pstEnv, pstRoleTmpSys->PatrolID, NULL);
		
		if(pstPlayer->stOnline.cMoving )
		{
			struct timeval stCurr;
			stCurr = pstEnv->pstAppCtx->stCurr;
			z_player_move_curr_bytime(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0, &stCurr, 1);
			z_player_stop(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, NULL);
		}
		
		if( pstRoleTmpSys->PatrolID > 0 &&
			pstRoleTmpSys->DelPos.X != pstPlayer->stRoleData.Pos.X &&
			pstRoleTmpSys->DelPos.Y != pstPlayer->stRoleData.Pos.Y)
		{
			//pstPlayer->stRoleData.Pos = pstRoleTmpSys->DelPos;
			if(pstPatrolDef)
			{
				z_player_chgpos(pstEnv->pstAppCtx, pstEnv, pstPlayer, &pstPatrolDef->PatrolPos[0]);
			}
			else
			{
				z_player_chgpos(pstEnv->pstAppCtx, pstEnv, pstPlayer, &pstRoleTmpSys->DelPos);
			}
		}
		else
		{
			z_player_chgpos(pstEnv->pstAppCtx, pstEnv, pstPlayer, &pstRoleTmpSys->DelPos);
		}

		pstRoleTmpSys->PatrolID = 0;
						
		//这里不再做刷新处理了
		return 1;
	}
	else if (pstItemDef && ITEM_TMP_MACHINE == pstItemDef->ItemTmp.Type)
	{
		z_player_stop(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, NULL);
	}
	
	return 0;
}

int is_tmp_machine(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	ITEMDEF *pstItemDef;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;

	if (0 == pstRoleTmpSys->TmpItemID)
	{
		return 0;
	}

	pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
	if (pstItemDef && ITEM_TMP_MACHINE == pstItemDef->ItemTmp.Type)
	{
		return 1;
	}
	
	return 0;
}

int tmp_item_drop(ZONESVRENV* pstEnv, Player *pstPlayer,int iDropFlag)
{
	int iDir;
	RESPOS stNextPos;
	RESPOS stPos;
	int iEnerge;
	ITEMDEF *pstItemDef;
	ZONEIDX stIdx;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;
	ZoneSkill *pstZoneSkill = NULL ;
	int iSkillID_1_CollectNum = 0; 
	int iSkillID_2_CollectNum = 0; 
	int  iTmpCollectNum = 0;
	int  iTmpEnerge = 0;
	int iGFMap;
	if (0 == pstRoleTmpSys->TmpItemID)
	{
		return 0;
	}

	iDir = pstRoleTmpSys->Dir;
	pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
	if (NULL == pstItemDef || ITEM_TMP_ARM_AMM > pstItemDef->ItemTmp.Type)
	{
		return -1;
	}

	if (pstPlayer->ucRoleLogoutReason == 0 &&
		pstPlayer->stOnline.stAutoMoveInfo.iTmpItemID != 0 && 
		(int)pstRoleTmpSys->TmpItemID == pstPlayer->stOnline.stAutoMoveInfo.iTmpItemID)
	{
		return -1;
	}

	if( !iDropFlag &&
		ITEM_TMP_MACHINE == pstItemDef->ItemTmp.Type &&
		pstPlayer->ucRoleLogoutReason == 0 &&
		pstRoleTmpSys->TmpItemID != 0 && 
		pstRoleTmpSys->PatrolID > 0 &&
		pstRoleTmpSys->DelPos.X != pstPlayer->stRoleData.Pos.X &&
		pstRoleTmpSys->DelPos.Y != pstPlayer->stRoleData.Pos.Y)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TMP102);	
		return -1;
	}

	if(ITEM_TMP_ITEM == pstItemDef->ItemTmp.Type )
	{
		/* 如果是手雷之类的,需要把积累数给记下*/
		pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstItemDef->ItemTmp.SkillID_1);
		if (NULL != pstZoneSkill)
		{
			iSkillID_1_CollectNum = pstZoneSkill->stSkillData.CollectNum;
			iTmpCollectNum = genius_add_tmpitem_collectnum(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstItemDef);
			if( iSkillID_1_CollectNum > iTmpCollectNum )
			{
				iSkillID_1_CollectNum -= iTmpCollectNum;
			}
			else 
			{
				iSkillID_1_CollectNum = 0;
			}
		}
		pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstItemDef->ItemTmp.SkillID_2);
		if (NULL != pstZoneSkill)
		{
			iSkillID_2_CollectNum = pstZoneSkill->stSkillData.CollectNum;
			iTmpCollectNum = genius_add_tmpitem_collectnum(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstItemDef);
			if( iSkillID_2_CollectNum > iTmpCollectNum )
			{
				iSkillID_2_CollectNum -= iTmpCollectNum;
			}
			else 
			{
				iSkillID_2_CollectNum = 0;
			}
		}
	}
	
	iEnerge = pstRoleTmpSys->TmpEnerge;

	// 处理天赋对能量的影响,可以是负数
	iTmpEnerge = genius_add_tmpitem_energy(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstItemDef);
	iEnerge -= iTmpEnerge;
	
	if (0 == tmp_item_del(pstEnv, pstPlayer))
	{
		if (ITEM_TMP_MACHINE == pstItemDef->ItemTmp.Type)
		{
			ZONEMAPINST *pstMapInst = NULL;
			MapIndex *pstMapIdx = NULL;
			
			stIdx.iID = pstPlayer->stRoleData.Map;
			stIdx.iPos = pstPlayer->stOnline.iMapPos;

			if (pstPlayer->stOnline.cMoving)
			{
				z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
			}
			iGFMap = GET_PWORLDID(pstPlayer->stRoleData.Map);
			//只有副本里或者地图刷新的才丢下来临时机械，野外非地图刷新的直接清除了(公会积分活动也直接清除)
			if(iGFMap != CLAN_EVERYDAY_PWORLD_ID &&( pstPlayer->stRoleData.Map > PWORLD_MAPID_BASE || (pstRoleTmpSys->TmpItemFlag & TMP_ITEM_FLAG_MAP_REFRESH)))
			{
				DropItem* pstDrop = NULL;
				
				if( pstRoleTmpSys->OwerID > 0)
				{
					tmp_item_create(pstEnv, pstPlayer,pstItemDef->ItemID, &stIdx, &pstPlayer->stRoleData.Pos, iEnerge, 0, 
								pstItemDef->ItemTmp.StayTime, pstRoleTmpSys->Dir,0,0,&pstDrop);
				}
				else
				{
					tmp_item_create(pstEnv, NULL,pstItemDef->ItemID, &stIdx, &pstPlayer->stRoleData.Pos, iEnerge, 0, 
								pstItemDef->ItemTmp.StayTime, pstRoleTmpSys->Dir,0,0,&pstDrop);
				}
				
				if (pstDrop)
				{
					pstDrop->iTmpItemFlag = pstRoleTmpSys->TmpItemFlag;
				}
				else
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "tmp item MapID=%d ItemID=%d",pstPlayer->stRoleData.Map,pstItemDef->ItemID);	
				}
			}

			pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
			if (NULL == pstMapInst)
			{
				pstMapIdx = NULL;
			}
			else
			{
				pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
			}
			if (pstMapIdx)
			{
				if (0 < pstPlayer->stOnline.cLastPathPoints)
				{
					int iPoint;
					int iFaceAngel;
										
					iPoint = (pstPlayer->stOnline.cLastPath + pstPlayer->stOnline.cLastPathPoints - 1)%RECORD_TURN;
					iFaceAngel = FaceAngleBetween(pstPlayer->stOnline.stTurnPoints[iPoint].X, pstPlayer->stOnline.stTurnPoints[iPoint].Y, 
													pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y);	
					stNextPos.X		=	(int) (pstPlayer->stRoleData.Pos.X - 250 * cos((iFaceAngel)*PI/180.0f));
					stNextPos.Y		=	(int) (pstPlayer->stRoleData.Pos.Y + 250 * sin((iFaceAngel)*PI/180.0f));
					if(pstMapInst)
					{
						z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstPlayer->stRoleData.Pos, &stNextPos, 50, 0, &stPos);
					}
					else
					{
						z_get_ran_pos(pstEnv, NULL, pstMapIdx, &pstPlayer->stRoleData.Pos, &stNextPos,50, 0, &stPos);
					}
				}
				else
				{
					stNextPos.X		=	(int) (pstPlayer->stRoleData.Pos.X - 250 * cos((2*iDir+90)*PI/180.0f));
					stNextPos.Y		=	(int) (pstPlayer->stRoleData.Pos.Y + 250 * sin((2*iDir+90)*PI/180.0f));
					if(pstMapInst)
					{
						z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstPlayer->stRoleData.Pos, &stNextPos, 50, 0, &stPos);
					}
					else
					{
						z_get_ran_pos(pstEnv, NULL, pstMapIdx, &pstPlayer->stRoleData.Pos, &stNextPos, 50, 0, &stPos);
					}
				}
				
				z_player_chgpos(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stPos);

			}
			
		}
		else if ( ITEM_TMP_ITEM == pstItemDef->ItemTmp.Type)
		{
			stIdx.iID = pstPlayer->stRoleData.Map;
			stIdx.iPos = pstPlayer->stOnline.iMapPos;

			if (pstPlayer->stOnline.cMoving)
			{
				z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
			}
			if( iSkillID_1_CollectNum <=0 && iSkillID_2_CollectNum <=0 )
			{
				return 0;
			}
			iGFMap = GET_PWORLDID(pstPlayer->stRoleData.Map);
			//只有副本里或者地图刷新的才丢下来临时机械，野外非地图刷新的直接清除了(公会积分活动也直接清除)
			if(iGFMap != CLAN_EVERYDAY_PWORLD_ID &&( pstPlayer->stRoleData.Map > PWORLD_MAPID_BASE || (pstRoleTmpSys->TmpItemFlag & TMP_ITEM_FLAG_MAP_REFRESH)))
			{
				DropItem* pstDrop = NULL;
				
				if( pstRoleTmpSys->OwerID > 0)
				{
					tmp_item_create(pstEnv,pstPlayer, pstItemDef->ItemID, &stIdx, &pstPlayer->stRoleData.Pos, iEnerge, 
								0, 
								pstItemDef->ItemTmp.StayTime, pstRoleTmpSys->Dir,
								iSkillID_1_CollectNum,iSkillID_2_CollectNum,&pstDrop);
				}
				else
				{
					tmp_item_create(pstEnv,NULL, pstItemDef->ItemID, &stIdx, &pstPlayer->stRoleData.Pos, iEnerge, 
								0, 
								pstItemDef->ItemTmp.StayTime, pstRoleTmpSys->Dir,
								iSkillID_1_CollectNum,iSkillID_2_CollectNum,&pstDrop);
				}

				if (pstDrop)
				{
					pstDrop->iTmpItemFlag = pstRoleTmpSys->TmpItemFlag;
				}
				else
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "tmp item MapID=%d ItemID=%d",pstPlayer->stRoleData.Map,pstItemDef->ItemID);	
				}
			}
		}
	}
	
	return 0;
}

int tmp_skill_clear(ZoneSkillList *pstZoneSkillList )
{
	int i;

	for (i=pstZoneSkillList->nSkillNum-1; i>=0; i--)
	{
		if (pstZoneSkillList->astZoneSkills[i].stSkillData.SkillID >= TMP_SKILL_MIN && 
			pstZoneSkillList->astZoneSkills[i].stSkillData.SkillID <= TMP_SKILL_MAX)
		{
			pstZoneSkillList->nSkillNum--;
			if (i != pstZoneSkillList->nSkillNum)
			{
				memcpy(&pstZoneSkillList->astZoneSkills[i], &pstZoneSkillList->astZoneSkills[pstZoneSkillList->nSkillNum],
							sizeof(pstZoneSkillList->astZoneSkills[i]));
			}
		}
	}
	
	return 0;
}

int tmp_item_check(ZONESVRENV* pstEnv, Player *pstPlayer, int iOldMap)
{
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;

	if (pstRoleTmpSys->TmpItemID > 0)
	{
		ITEMDEF *pstItemDef;

		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
		if (pstItemDef && pstItemDef->ItemTmp.SkillID_1 > 0)
		{
			if ( NULL== z_get_player_zoneskill(pstPlayer, pstItemDef->ItemTmp.SkillID_1))
			{
				tmp_item_del(pstEnv, pstPlayer);
				return 0;
			}
		}

		if (pstItemDef && pstItemDef->ItemTmp.SkillID_2 > 0)
		{
			if ( NULL== z_get_player_zoneskill(pstPlayer, pstItemDef->ItemTmp.SkillID_2))
			{
				tmp_item_del(pstEnv, pstPlayer);
				return 0;
			}
		}
	}
	else
	{
		tmp_skill_clear(pstZoneSkillList);
	}

	// 登陆时要把临时道具删除掉
	tmp_item_del(pstEnv, pstPlayer);
	

	if (iOldMap >= PWORLD_MAPID_BASE && pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE && pstPlayer->stRoleData.RoleDetail.XP > 0)
	{
		
		if (pstPlayer->stRoleData.MiscInfo.Flag & PWORLD_ITEM_ADD_XP)
		{
			exit_pworld_clean_xp(pstEnv->pstAppCtx, pstEnv, pstPlayer);
			pstPlayer->stRoleData.MiscInfo.Flag &= ~PWORLD_ITEM_ADD_XP;
		}

		z_player_del_wushuang_status(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		
	}
	return 0;
}

int tmp_fill(ZONESVRENV* pstEnv , Player *pstPlayer, ZoneSkill *pstZoneSkill, Player *pstPlayerOther)
{
	PlayerChagAttrVal stChgVal;
	ITEMDEF *pstItemDef;
	ITEMDEF *pstItemDefOther;
	ZoneSkill *pstZoneSkillOther;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;
	ROLETMPSYS *pstRoleTmpSysOther = &pstPlayerOther->stRoleData.MiscInfo.TmpSys;

	if (pstRoleTmpSys->TmpItemID == 0 || pstRoleTmpSysOther->TmpItemID == 0)
	{
		return -1;
	}

	pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
	if (NULL == pstItemDef)
	{
		return -1;
	}

	pstItemDefOther = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSysOther);
	if (NULL == pstItemDefOther)
	{
		return -1;
	}

	if (ITEM_TMP_ARM_AMM == pstItemDef->ItemTmp.Type)
	{
		if (ITEM_TMP_ARM != pstItemDefOther->ItemTmp.Type)
		{
			return -1;
		}

		stChgVal.AttrNum = 0;

		if (pstItemDefOther->ItemTmp.SkillID_1 > 0)
		{
			pstZoneSkillOther = z_get_player_zoneskill(pstPlayerOther, pstItemDefOther->ItemTmp.SkillID_1);

			if (pstZoneSkillOther && pstZoneSkillOther->stSkillData.CollectNum < pstZoneSkillOther->stSkillData.CollectNumMax)
			{
				pstZoneSkillOther->stSkillData.CollectNum += pstZoneSkill->stSkillData.CollectNum;
				if (pstZoneSkillOther->stSkillData.CollectNum > pstZoneSkillOther->stSkillData.CollectNumMax)
				{
					pstZoneSkillOther->stSkillData.CollectNum = pstZoneSkillOther->stSkillData.CollectNumMax;
				}
				
				stChgVal.AttrList[stChgVal.AttrNum].AttrID = ATTR_ID_COLLECTNUM;
				stChgVal.AttrList[stChgVal.AttrNum].Value.CollectNum.SkillID = pstZoneSkillOther->stSkillData.SkillID;
				stChgVal.AttrList[stChgVal.AttrNum].Value.CollectNum.CollectNum = pstZoneSkillOther->stSkillData.CollectNum;
				stChgVal.AttrNum++;
			}
		}

		if (pstItemDefOther->ItemTmp.SkillID_2 > 0)
		{
			pstZoneSkillOther = z_get_player_zoneskill(pstPlayerOther, pstItemDefOther->ItemTmp.SkillID_2);

			if (pstZoneSkillOther && pstZoneSkillOther->stSkillData.CollectNum < pstZoneSkillOther->stSkillData.CollectNumMax)
			{
				pstZoneSkillOther->stSkillData.CollectNum += pstZoneSkill->stSkillData.CollectNum;
				if (pstZoneSkillOther->stSkillData.CollectNum > pstZoneSkillOther->stSkillData.CollectNumMax)
				{
					pstZoneSkillOther->stSkillData.CollectNum = pstZoneSkillOther->stSkillData.CollectNumMax;
				}
				
				stChgVal.AttrList[stChgVal.AttrNum].AttrID = ATTR_ID_COLLECTNUM;
				stChgVal.AttrList[stChgVal.AttrNum].Value.CollectNum.SkillID = pstZoneSkillOther->stSkillData.SkillID;
				stChgVal.AttrList[stChgVal.AttrNum].Value.CollectNum.CollectNum = pstZoneSkillOther->stSkillData.CollectNum;
				stChgVal.AttrNum++;
			}
		}

		if (stChgVal.AttrNum > 0)
		{
			z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayerOther, &stChgVal, 1);

			tmp_item_del(pstEnv, pstPlayer);
		}
	}
	else if (ITEM_TMP_MACHINE_ENERGE == pstItemDef->ItemTmp.Type)
	{
		if (ITEM_TMP_MACHINE != pstItemDefOther->ItemTmp.Type)
		{
			return -1;
		}

		if (pstRoleTmpSysOther->TmpEnerge < pstItemDefOther->ItemTmp.MachineEnerge)
		{
			stChgVal.AttrNum = 0;
			
			pstRoleTmpSysOther->TmpEnerge += pstZoneSkill->stSkillData.CollectNum;
			if (pstRoleTmpSysOther->TmpEnerge > pstItemDefOther->ItemTmp.MachineEnerge)
			{
				pstRoleTmpSysOther->TmpEnerge = pstItemDefOther->ItemTmp.MachineEnerge;
			}

			stChgVal.AttrList[0].AttrID = ATTR_ID_TMPENERGE;
			if(pstRoleTmpSysOther->TmpEnerge > 0 )
			{
				stChgVal.AttrList[0].Value.Energe = pstRoleTmpSysOther->TmpEnerge;
			}
			else
			{
				stChgVal.AttrList[0].Value.Energe = 0;
			}
			stChgVal.AttrNum = 1;

			z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayerOther, &stChgVal, 1);
				
			tmp_item_del(pstEnv, pstPlayer);
		}
	}

	return 0;
}

int tmp_tmpenerge_max_action(ZONESVRENV* pstEnv,Player *pstPlayer,int iTmpEnergeMax)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	CSACTIONTMPENERGEMAX *pTmpEnergeMax = &pstActionRes->Data.TmpEnergeMax;

	pstActionRes->ActionID = ACTION_TMPENERGE_MAX;

	pTmpEnergeMax->TmpEnergeMax = (unsigned short)iTmpEnergeMax;
		
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int tmp_apply_item(ZONESVRENV* pstEnv,Player *pstPlayer, int iItemID, int iStay)
{
	
	DropItem *pstDropItem;
	ITEMDEF *pstItemDef;
	ZONEMAPINST *pstMapInst;
	ZONEIDX stMapIdx;
	RESPOS stPos;

	pstItemDef = z_find_itemdef(pstEnv, iItemID);
	if (NULL == pstItemDef || NULL == pstPlayer)
	{
		return -1;
	}

	stMapIdx.iID = pstPlayer->stRoleData.Map;
	stMapIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stMapIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	stPos = pstPlayer->stRoleData.Pos;
	if (0 > tmp_item_create(pstEnv,NULL, iItemID, &stMapIdx, &stPos, pstItemDef->ItemTmp.MachineEnerge, pstItemDef->ItemTmp.RefreshTime, 
							iStay, 0,0,0,&pstDropItem))
	{
		return -1;
	}

	if (0 > tmp_item_add(pstEnv, pstPlayer, pstDropItem, pstItemDef))
	{
		return -1;
	}
	else
	{
		z_item_leave_map(pstEnv->pstAppCtx, pstEnv, pstDropItem);
	}
	
	return 0;
}
