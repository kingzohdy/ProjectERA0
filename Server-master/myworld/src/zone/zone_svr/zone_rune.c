/*
**  @file $RCSfile: zone_rune.c,v $
**  general description of this module
**  $Id: zone_rune.c,v 1.78 2014/07/14 02:34:53 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/07/14 02:34:53 $
**  @version $Revision: 1.78 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "proto_comm.h"
#include "proto_cs.h"
#include "role_db_meta.h"
#include "zone_clt.h"
#include "zone_svr.h"
#include "zone_package.h"
#include "zone_oplog.h"
#include "zone_skill.h"
#include "zone_rune.h"
#include "zone_combat.h"
#include "zone_attr.h"
#include "zone_status.h"
#include "zone_player.h"
#include "zone_shop.h"
#include "zone_arm.h"
#include "zone_err.h"

// 获取符文物品格子
static ROLEITEM *rune_item_get(Player *pstPlayer, const CSRUNEITEM *pstItem,
                               int *piPos);
ONERUNEADDATTR* player_rune_addattr_info_get(Player *pstPlayer, int iRuneType);


// 获取符文物品的颜色
static int rune_item_color(ZONESVRENV *pstEnv, const ROLEITEM *pstItem);

//各等级 各颜色的符文物品ID
static const int kRuneItemID[][RUNE_YELLOW + 1] = {
	[0] = {[RUNE_EMPTY] = 0,[RUNE_RED] = 5251207,[RUNE_BLUE] = 5251208,[RUNE_YELLOW] = 5251209},
	[1] = {[RUNE_EMPTY] = 0,[RUNE_RED] = 5251202,[RUNE_BLUE] = 5251203,[RUNE_YELLOW] = 5251204},
};

//符文属性重置消耗的物品ID
static const int kRuneResetCostID[] = {
	5251202,
	5251203,
	5251204,
	RUNE_ENABLE_ITEM_ID,
};

RUNEADDATTRDEF* z_find_run_addattr_def(ZONESVRENV *pstEnv, int iID)
{
	return (RUNEADDATTRDEF *)z_find_def(pstEnv->pstZoneObj->sRuneAddAttrDef, iID, RUNE_ADDATTR_DEF_DATA, 
								pstEnv->pstZoneObj->iRuneAddAttrDef, NULL);
}

int player_rune(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,
                TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	const CSRUNECLTEXTENSION *pstReq = &pstCsPkg->Body.RuneClt.Extension;

	UNUSED(pstAppCtx);
	UNUSED(pstFrameHead);

	switch ((int)pstCsPkg->Body.RuneClt.Operation)
	{
	case RUNE_CLT_ENABLE:
		player_rune_enable(pstEnv, pstPlayer, &pstReq->RuneEnable);
		break;

	case RUNE_CLT_SET:
		player_rune_set(pstEnv, pstPlayer, &pstReq->RuneSet);
		break;

	case RUNE_CLT_DEL:
		player_rune_del(pstEnv, pstPlayer, &pstReq->RuneDel);
		break;
	}
	return 0;
}

int player_rune_enable(ZONESVRENV *pstEnv, Player *pstPlayer,
                       const CSRUNEENABLECLT *pstReq)
{
	CSPKG stPkg;	
	CSRUNESVR *pstRes = &stPkg.Body.RuneSvr;	
	ZoneSkill *pstZoneSkill;	
	OPROLESKILLRUNECHG stOpBefore;	
	double dTmp;
	int iMoney;
	SKILLDEF *pstSkillDef;
	int iRet;
	SKILLRESULT *pstSkillResult = NULL;
	int iLevel = 0;

	pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstReq->SkillID);
	if (!pstZoneSkill)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE201);
		goto fail;
	}

	if (pstPlayer->stOnline.State & CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE202);
		goto fail;
	}

	if (pstZoneSkill->stSkillData.RuneSlotEnabled)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR,
		           ZONEERR_RUNE204);
		goto fail;
	}

	pstSkillDef = z_find_skill(pstEnv, pstZoneSkill->stSkillData.SkillID, 1, NULL);
	if(!pstSkillDef)
	{
		goto fail;
	}

	pstSkillResult = z_get_skill_result(pstSkillDef,RESULT_RUNE_ENABLE_LEVEL_LIMIT);
	if(pstSkillResult)
	{
		if(pstSkillResult->SkillResultVal1 != 0)
		{
			iLevel = pstSkillResult->SkillResultVal1;
		}
		else
		{
			iLevel = MIN_RUNE_LEVEL;
		}
	}
	else
	{
		iLevel = MIN_RUNE_LEVEL;
	}

	if (pstPlayer->stRoleData.Level < iLevel)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR,
		           ZONEERR_RUNE231, iLevel);
		goto fail;
	}
	
	dTmp = pow(pstSkillDef->Learn.LevelMin/10.0, 3)*115;
	dTmp = ceil(dTmp/1000.0);
	iMoney = dTmp*1000;
	
	if(0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, iMoney))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR,
		           ZONEERR_RUNE205, iMoney);
		goto fail;
	}

	iRet = package_dec_money(pstEnv, pstPlayer, ATTR_ID_BULL, iMoney);

	if(0 > iRet)
	{
		goto fail;
	}

	z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_RUNESLOT, pstSkillDef->SkillID, iMoney-iRet, iRet, "符文格");
	
	/*
	if (package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer,
		                 RUNE_ENABLE_ITEM_ID, 1, LIST_TYPE_SUNDRIES, NULL,PACKAGE_DEC_NONE) < 0)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_SVR,
		           "没有\"符文之力\"物品，无法执行该操作！");
		goto fail;
	}
	

	// 激活符文槽
	package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, RUNE_ENABLE_ITEM_ID, 1,
	            LIST_TYPE_SUNDRIES, NULL,PACKAGE_DEC_NONE);
	*/
	
	{
		rune_op_info(pstZoneSkill,&stOpBefore,RUNE_EMPTY,pstPlayer);
		
		// 通知客户端更新
		pstZoneSkill->stSkillData.RuneItemID = 0;
		pstZoneSkill->stSkillData.RuneSlot = RUNE_EMPTY;
		pstZoneSkill->stSkillData.RuneSlotEnabled = 1;
		z_skill_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstZoneSkill);
	}

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
	           ZONEERR_RUNE206);
	pstRes->Extension.RuneEnable.Succeeded = 1;

	z_role_skill_oplog(pstEnv, pstPlayer, OP_RUNE_SLOT_ENABLED, 
	pstZoneSkill->stSkillData.SkillID, pstZoneSkill->stSkillData.SkillLevel, pstZoneSkill->stSkillData.SkillLevel,
							pstPlayer->stRoleData.RoleDetail.SkillPoint, pstPlayer->stRoleData.RoleDetail.SkillPoint,
							NULL,&stOpBefore,0, NULL);

final:
	Z_CSHEAD_INIT(&stPkg.Head, RUNE_SVR);
	pstRes->Operation = RUNE_SVR_ENABLE;
	pstRes->Extension.RuneEnable.SkillID = pstReq->SkillID;
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;

fail:
	pstRes->Extension.RuneEnable.Succeeded = 0;
	goto final;
}

int player_rune_set(ZONESVRENV *pstEnv, Player *pstPlayer,
                    const CSRUNESETCLT *pstReq)
{
	CSPKG stPkg;
	int iColor = 0, iPos = -1;
	unsigned short iTransferedID = 0;
	CSRUNESVR *pstRes = &stPkg.Body.RuneSvr;
	SKILLDEF *pstSkillDef = NULL;
	ZoneSkill *pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstReq->SkillID);
	ROLEITEM *pstItem = rune_item_get(pstPlayer, &pstReq->RuneItem, &iPos);
	OPROLESKILLRUNECHG stOpBefore;
	int iSpeSkillID = 0;

	if (!pstZoneSkill)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE207);
		goto fail;
	}

	if ((pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill)) == NULL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE207);
		goto fail;
	}

	if (!pstItem || iPos < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE208);
		goto fail;
	}

	if (pstPlayer->stOnline.State & CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE202);
		goto fail;
	}

	if (pstPlayer->stRoleData.Level < MIN_RUNE_LEVEL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR,
		           ZONEERR_RUNE209, MIN_RUNE_LEVEL);
		goto fail;
	}

	if (!pstZoneSkill->stSkillData.RuneSlotEnabled)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE210);
		goto fail;
	}

	iColor = rune_item_color(pstEnv, pstItem);
	switch (iColor)
	{
	case RUNE_RED:
		if (!pstSkillDef->RedRuneTransferedID)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR,
			           		           ZONEERR_RUNE211);
			goto fail;
		}
		iTransferedID = pstSkillDef->RedRuneTransferedID;
		break;

	case RUNE_BLUE:
		if (!pstSkillDef->BlueRuneTransferedID)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_SVR,
			                     ZONEERR_RUNE211);
			goto fail;
		}
		iTransferedID = pstSkillDef->BlueRuneTransferedID;
		break;

	case RUNE_YELLOW:
		if (!pstSkillDef->YellowRuneTransferedID)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_SVR,
			                     ZONEERR_RUNE211);
			goto fail;
		}
		iTransferedID = pstSkillDef->YellowRuneTransferedID;
		break;

	default:
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE212);
		goto fail;
		break;
	}

	if (!z_find_skill(pstEnv, iTransferedID, pstSkillDef->Level, NULL))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE213);
		goto fail;
	}

	//此类技能符文需要一一对应
	if(pstSkillDef->CtrlFlag & SKILL_FLAG_SPE_RUNE)
	{
		const ITEMDEF *pstDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstItem);
		if (!pstDef)
		{
			goto fail;
		}
		
		iSpeSkillID = rune_only_inlay_which_skill(pstEnv, pstDef);
		if(iSpeSkillID != pstSkillDef->SkillID)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE_1,pstDef->Name,pstSkillDef->Name);
			goto fail;
		}
	}

	rune_op_info(pstZoneSkill,&stOpBefore,RUNE_EMPTY,pstPlayer);

	// 设置符文到技能上
	pstZoneSkill->stSkillData.RuneItemID = pstItem->DefID;
	pstZoneSkill->stSkillData.RuneSlot = iColor;
	pstZoneSkill->stCoolEnd.tv_sec = pstEnv->pstAppCtx->stCurr.tv_sec; //策划要求符文操作后,冷却时间置

	pstZoneSkill->stSkillData.RuneInfo[iColor].SkillID = iTransferedID;
	pstZoneSkill->stSkillData.RuneInfo[iColor].RuneItemID=  pstItem->DefID;
	
	z_skill_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstZoneSkill);

	pstRes->Extension.RuneSet.Succeeded = 1;
	/*
	z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2,
	           "技能【%s】成功激活了\"%s\"属性。", pstSkillDef->Name,
	           kRuneColorName[iColor]);
	           */
	z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2,
	           ZONEERR_RUNE214, pstSkillDef->Name);
	
	// 先把物品减掉
	package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItem->DefID, 1,
	            pstReq->RuneItem.ListType, &iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_RUNE);



	player_rune_num_check(pstEnv,pstPlayer);

	z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);

	z_role_skill_oplog(pstEnv, pstPlayer, OP_RUNE_SLOT_ADD, 
						pstZoneSkill->stSkillData.SkillID, pstZoneSkill->stSkillData.SkillLevel, pstZoneSkill->stSkillData.SkillLevel,
						pstPlayer->stRoleData.RoleDetail.SkillPoint, pstPlayer->stRoleData.RoleDetail.SkillPoint,
						NULL,&stOpBefore,0,NULL);

	
	//成长之路
	{
		ACHIEVEMENTCOND stCond;
		ITEMDEF *pstDef = z_find_itemdef(pstEnv,pstItem->DefID);
		RESULT* pstResult = NULL;

		if (pstDef)
		{
			pstResult = z_item_get_result(pstDef, RESULT_RUNE);
		}

		if (pstResult)
		{	
			memset(&stCond,0,sizeof(stCond));
			
			stCond.Type = COND_RUNE_ADD;
			stCond.Val1 = pstResult->ResultVal1;
		
			z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
		}
	}
	
		
final:
	Z_CSHEAD_INIT(&stPkg.Head, RUNE_SVR);
	pstRes->Operation = RUNE_SVR_SET;
	pstRes->Extension.RuneSet.SkillID = pstReq->SkillID;
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;

fail:
	pstRes->Extension.RuneSet.Succeeded = 0;
	goto final;
}

int player_rune_del(ZONESVRENV *pstEnv, Player *pstPlayer,
                    const CSRUNEDELCLT *pstReq)
{
	CSPKG stPkg;
	int iPos = -1;
	SKILLDEF *pstSkillDef = NULL;
	CSRUNESVR *pstRes = &stPkg.Body.RuneSvr;
	ZoneSkill *pstZoneSkill = z_get_player_zoneskill(pstPlayer,  pstReq->SkillID);
	ROLEITEM *pstItem = rune_item_get(pstPlayer, &pstReq->RuneItem, &iPos);
	OPROLESKILLRUNECHG stOpBefore;

	if (!pstZoneSkill)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE207);
		goto fail;
	}

	if ((pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill)) == NULL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE207);
		goto fail;
	}

	if (!pstItem || iPos < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE208);
		goto fail;
	}

	if (pstItem->DefID != RUNE_FORGET_ITEM_ID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE215);
		goto fail;
	}

	if (pstPlayer->stOnline.State & CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE202);
		goto fail;
	}

	if (pstPlayer->stRoleData.Level < MIN_RUNE_LEVEL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR,
		   					ZONEERR_RUNE203, MIN_RUNE_LEVEL);
		goto fail;
	}

	if (!pstZoneSkill->stSkillData.RuneSlotEnabled)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR,
		           ZONEERR_RUNE210);
		goto fail;
	}

	if (!pstZoneSkill->stSkillData.RuneSlot || !pstZoneSkill->stSkillData.RuneItemID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR,
		           				ZONEERR_RUNE216);
		goto fail;
	}
	
	rune_op_info(pstZoneSkill,&stOpBefore,RUNE_EMPTY,pstPlayer);
	
	// 清除符文
	pstZoneSkill->stSkillData.RuneItemID = 0;
	pstZoneSkill->stSkillData.RuneSlot = 0;
	pstZoneSkill->stCoolEnd.tv_sec = pstEnv->pstAppCtx->stCurr.tv_sec; //策划要求符文操作后,冷却时间置0
	
	z_skill_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstZoneSkill);
	
	pstRes->Extension.RuneDel.Succeeded = 1;

	// 先把物品减掉
	package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItem->DefID, 1,
	            pstReq->RuneItem.ListType, &iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_RUNE);

	z_role_skill_oplog(pstEnv, pstPlayer, OP_RUNE_SLOT_DEL, 
		pstZoneSkill->stSkillData.SkillID, pstZoneSkill->stSkillData.SkillLevel, pstZoneSkill->stSkillData.SkillLevel,
								pstPlayer->stRoleData.RoleDetail.SkillPoint, pstPlayer->stRoleData.RoleDetail.SkillPoint,
								NULL,&stOpBefore,0,NULL);

final:
	Z_CSHEAD_INIT(&stPkg.Head, RUNE_SVR);
	pstRes->Operation = RUNE_SVR_DEL;
	pstRes->Extension.RuneDel.SkillID = pstReq->SkillID;
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;

fail:
	pstRes->Extension.RuneDel.Succeeded = 0;
	goto final;
}

static int rune_item_color(ZONESVRENV *pstEnv, const ROLEITEM *pstItem)
{
	int i = 0;
	const ITEMDEF *pstDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstItem);

	if (!pstDef)
	{
		return RUNE_EMPTY;
	}

	for (i = 0; i < (int)CCH(pstDef->Result); ++i)
	{
		if (!pstDef->Result[i].ResultID)
		{
			break;
		}
		if (pstDef->Result[i].ResultID == RESULT_RUNE && 
			pstDef->Result[i].ResultVal1 >= 0 && 
			pstDef->Result[i].ResultVal1 <= 2 &&
			pstDef->Result[i].ResultVal2 > RUNE_EMPTY &&
			pstDef->Result[i].ResultVal2 < RUNE_TYPE_NUM)
		{
			return pstDef->Result[i].ResultVal2;
		}
	}
	return RUNE_EMPTY;
}

static ROLEITEM *rune_item_get(Player *pstPlayer, const CSRUNEITEM *pstItem,
                               int *piPos)
{
	int iPos = -1;
	ROLEITEM *pstRoleItem = NULL;
	ROLEPACKAGE *pstPackage = &pstPlayer->stRoleData.Package;

	switch (pstItem->ListType)
	{			
	case LIST_TYPE_STUFF:
		{
			ROLEITEM *pstRoleItem = NULL;
			iPos = package_itemgrid_pos(pstPackage->StuffPackage.RoleItems,
				                        pstPackage->StuffPackage.Num,
				                        pstItem->GridIdx);
			if (iPos < 0)
			{
				goto final;
			}

			pstRoleItem = pstPackage->StuffPackage.RoleItems + iPos;
			if (pstRoleItem->GridIdx == pstItem->GridIdx &&
				pstRoleItem->WID == pstItem->WID)
			{
				goto final;
			}
			else
			{
				pstRoleItem = NULL;
			}
		}
		break;
		
	case LIST_TYPE_SUNDRIES:
		{
			ROLEGRID *pstRoleGrid = NULL;
			iPos = package_grid_pos(pstPackage->SundriesPackage.RoleGrids, 
				                        pstPackage->SundriesPackage.Num,
				                        pstItem->GridIdx);
			if (iPos < 0)
			{
				goto final;
			}

			pstRoleGrid = pstPackage->SundriesPackage.RoleGrids + iPos;
			if (pstRoleGrid->GridData.RoleItem.GridIdx == 
				pstItem->GridIdx &&
				pstRoleGrid->GridData.RoleItem.WID == pstItem->WID)
			{
				pstRoleItem = &pstRoleGrid->GridData.RoleItem;
			}
		}
		break;
	}

final:
	*piPos = iPos;
	return pstRoleItem;
}


int player_rune_level_up(ZONESVRENV *pstEnv, Player *pstPlayer,
                    const CSACTIONRUNELEVELUP *pstReq)
{
	int iLevelUPPos = 0;
	ROLEITEM* pstLevelUP = rune_item_get(pstPlayer,&pstReq->LevelUpItem, &iLevelUPPos);
	ROLEITEM* pstRune;
	ITEMDEF* pstLevelUPDef = NULL;
	ITEMDEF* pstRuneDef = NULL;
	RESULT* pstLevelUPResult = NULL;
	RESULT* pstRuneResult = NULL;
	int iItemID = 0;
	ROLEGRID stRoleGrid;	
	int iLevel = 0;
	int i, iBind, iPos;
	TotalStuffGridTmp stGridTmp;
	int iRate = 0;
	int iFlag = 0;
	int iID = 0;

	memset(&stGridTmp, 0, sizeof(stGridTmp));
	
	if (!pstLevelUP)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE208);
		return -1;
	}

	if (pstPlayer->stOnline.State & CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE202);
		return -1;
	}

	pstLevelUPDef = z_get_itemdef_roleitem(pstEnv,pstLevelUP);
	if (!pstLevelUPDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE208);
		return -1;
	}
	
	//pstLevelUPResult = z_item_get_result(pstLevelUPDef, RESULT_RUNE_LEVEL_UP);
	pstLevelUPResult = z_item_get_result(pstLevelUPDef, RESULT_RUNE_LEVEL_UP_NEW);
	if (!pstLevelUPResult)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE217);
		
		return -1;
	}
	
 	if (package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer,
		                 pstLevelUP->DefID, 1, LIST_TYPE_SUNDRIES, NULL,PACKAGE_DEC_NONE) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE218);
		return -1;
	}


	for(i=0; i<MAX_RUNE_STUFF_NUM; i++)
	{
		if(0 >package_get_item(pstEnv, pstPlayer, 
					pstReq->RuneItem[i].ListType, pstReq->RuneItem[i].GridIdx, &pstRuneDef, &pstRune, &iPos))
		{
			return -1;
		}

		if (i==0)
			iID = pstRuneDef->ItemID;

		if(iID != (int)pstRuneDef->ItemID)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE219);
			return -1;
		}

		pstRuneResult = z_item_get_result(pstRuneDef, RESULT_RUNE);
		if (!pstRuneResult)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE220);
			return -1;
		}

		//检查级别是否对应
		if (pstRuneResult->ResultVal1	!= 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE221,pstRuneDef->Name);
			return -1;
		}

		if (pstRuneResult->ResultVal2 <= RUNE_EMPTY || pstRuneResult->ResultVal2 > RUNE_YELLOW)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE222);
			return -1;
		}

		if (iItemID == 0)
		{
			iLevel = pstRuneResult->ResultVal1 + 1;
			iItemID = kRuneItemID[iLevel][pstRuneResult->ResultVal2];
		}
		
		if (0 > tmp_grid_total(&stGridTmp, pstRuneDef->ItemID, 1, iPos, pstReq->RuneItem[i].ListType))
		{
			return -1;
		}
		
		/*if (pstRuneResult->ResultVal1 != pstLevelUPResult->ResultVal1 || 
			pstRuneResult->ResultVal1 < 0 || pstRuneResult->ResultVal1 >= 2)
		{
			z_sys_strf(pstEnv, pstPlayer, SYS_SVR, "符文和升级道具的等级不符");
			return -1;
		}

			//检查颜色
		iLevel = pstRuneResult->ResultVal1 + 1;
		if (iLevel < 0 || iLevel > 2)
		{
			z_sys_strf(pstEnv, pstPlayer, SYS_SVR, "未知的符文等级");
			return -1;
		}

		if (pstRuneResult->ResultVal2 <= RUNE_EMPTY || pstRuneResult->ResultVal2 > RUNE_YELLOW)
		{
			z_sys_strf(pstEnv, pstPlayer, SYS_SVR, "未知的符文颜色");
			return -1;
		}

		iItemID = kRuneItemID[iLevel][pstRuneResult->ResultVal2];
			
		if (0 > tmp_grid_total(&stGridTmp, pstRuneDef->ItemID, 1, iPos, pstReq->RuneItem[i].ListType))
		{
			return -1;
		}*/
	}
	
	for(i=0; i<stGridTmp.iNum; i++)
	{
		if(0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer,
		                 stGridTmp.stGrid[i].iItemID, stGridTmp.stGrid[i].iNum, LIST_TYPE_SUNDRIES, &stGridTmp.stGrid[i].iPos,PACKAGE_DEC_NONE))
		{
			return -1;
		}
	}
	
	if (!iItemID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE223);
		return -1;
	}

	if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, iItemID))
	{
		return -1;
	}

	stRoleGrid.GridData.RoleItem.Num = 1;	

	if(package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_NOSPACE);
		return -1;
	}

	iBind = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstLevelUP->DefID, 1,
					pstReq->LevelUpItem.ListType, &iLevelUPPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_RUNE);
	if(iBind)
	{
		stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
	}

	iRate = RAND1(100);
	if (iRate < pstLevelUPResult->ResultVal1)
	{
		// 成功
		for (i = 0; i<MAX_RUNE_STUFF_NUM; i++ )
		{
			if(0 >package_get_item(pstEnv, pstPlayer, 
						pstReq->RuneItem[i].ListType, pstReq->RuneItem[i].GridIdx, &pstRuneDef, &pstRune, &iPos))
			{
				return -1;
			}
			
			iBind = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRune->DefID, 1,
		           			pstReq->RuneItem[i].ListType, &iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_RUNE);
			if(iBind)
			{
				stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
			}
		}
		
		package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0, IMPITEM_OPTYPE_RUNE);	
		iFlag = 1;
	}
	else
	{
		// 失败
		iRate = RAND1(3)+1;
		for (i = 0; i<iRate; i++ )
		{
			if(0 >package_get_item(pstEnv, pstPlayer, 
						pstReq->RuneItem[i].ListType, pstReq->RuneItem[i].GridIdx, &pstRuneDef, &pstRune, &iPos))
			{
				return -1;
			}
			
			package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRune->DefID, 1,
		           			pstReq->RuneItem[i].ListType, &iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_RUNE);
		}
	}

	
	//  通知客户端
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;

	memset(pstArmSvr, 0, sizeof(*pstArmSvr));
	pstArmSvr->Flag = iFlag;
	pstArmSvr->ArmSvrType = RUNE_UP_SVR_RES;
	if (!iFlag)
		pstArmSvr->IndexID = iRate;
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_rune_skill_chg(ZONESVRENV *pstEnv, Player *pstPlayer,
                    const CSACTIONRUNNESKILLCHG *pstReq)
{
	CSPKG stPkg;
	unsigned short iTransferedID = 0;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;	
	SKILLDEF *pstSkillDef = NULL;
	ZoneSkill *pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstReq->SkillID);

	if (pstPlayer->stOnline.State & CS_STAT_FIGHT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE224);
		goto fail;
	}

	if (pstZoneSkill->iRuneChgCoolEnd && (pstZoneSkill->iRuneChgCoolEnd > pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE225);

		if ((pstZoneSkill->iRuneChgCoolEnd - pstEnv->pstAppCtx->stCurr.tv_sec) > RUNE_CHG_COOL)
		{
			pstZoneSkill->iRuneChgCoolEnd = pstEnv->pstAppCtx->stCurr.tv_sec + RUNE_CHG_COOL;
		}
		goto fail;
	}
	
	if (!pstZoneSkill)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE207);
		goto fail;
	}

	if ((pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill)) == NULL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE207);
		goto fail;
	}

	
	if (pstPlayer->stOnline.State & CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE202);
		goto fail;
	}



	if (pstReq->RuneType < RUNE_EMPTY || pstReq->RuneType >= RUNE_TYPE_NUM)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE226);
		goto fail;
	}
	iTransferedID = pstZoneSkill->stSkillData.RuneInfo[pstReq->RuneType].SkillID;
	if (!iTransferedID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE227);
		goto fail;
	
	}

	if (!z_find_skill(pstEnv, iTransferedID, pstSkillDef->Level, NULL))
	{
		goto fail;
	}

	
	// 设置符文到技能上
	pstZoneSkill->stSkillData.RuneItemID = pstZoneSkill->stSkillData.RuneInfo[pstReq->RuneType].RuneItemID;
	pstZoneSkill->stSkillData.RuneSlot = pstReq->RuneType;
	pstZoneSkill->iRuneChgCoolEnd = pstEnv->pstAppCtx->stCurr.tv_sec + RUNE_CHG_COOL;

	
	z_skill_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstZoneSkill);

			
	
	memset(pstAction,0,sizeof(*pstAction));
	
	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_RUNNE_SKILL_CHG_RET;
	
	pstAction->Data.RunneSkillChgRet.SkillID = pstReq->SkillID;
	pstAction->Data.RunneSkillChgRet.RuneType = pstReq->RuneType;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	//z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		

fail:

	return -1;
}


int player_rune_reset_ex(ZONESVRENV *pstEnv, Player *pstPlayer,
                    const CSACTIONRESETRUNEEX *pstReq)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;
	OPROLESKILLRUNECHG stBefore;
	RUNEADDATTR *pstRuneAddattr = &pstPlayer->stRoleData.MiscInfo.RuneAddAttr;
	int iRet;
	

	if (pstReq->RuneType <= RUNE_EMPTY || pstReq->RuneType > RUNE_YELLOW)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE222);
		return -1;
	}

	if (pstPlayer->stOnline.State & CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE202);
		return -1;
	}

	if (pstPlayer->stOnline.State & CS_STAT_FIGHT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_FIGHT_OP);
		return -1;
	}
/*
	if(is_battle_pve_autoteam_svr(pstEnv) || 
		 pstPlayer->stOnline.eStatusAssistant & PLAYER_STATUS_BATTLE_LOGIN_READY)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "当前处于跨服状态，不允许该操作");
		return -1;
	}
	*/

	if (pstReq->OpType <0 || pstReq->OpType > 2)
	{
		return -1;
	}

	if (pstPlayer->stOnline.aiRuneColorCount[pstReq->RuneType] < RUNE_EX_ENABLE_NUM)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE228);
		return -1;
	}
	
	if ( 2 == pstReq->OpType)
	{
		if (!pstRuneAddattr->iUnsavedRuneType || !pstRuneAddattr->iUnsavedRuneAttrID)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE229);
			return -1;
		}
		rune_op_info(NULL,&stBefore,pstRuneAddattr->iUnsavedRuneType,pstPlayer);
		player_rune_addattr_set(pstEnv,pstPlayer,pstRuneAddattr->iUnsavedRuneAttrID,pstRuneAddattr->iUnsavedRuneType);

		pstRuneAddattr->iUnsavedRuneType = 0;
		pstRuneAddattr->iUnsavedRuneAttrID = 0; 

		
		player_rune_ex_enable_info(pstEnv,pstPlayer);

		z_role_skill_oplog(pstEnv, pstPlayer, OP_RUNE_ATTR_SAVE, 
							0, 0, 0,
							pstPlayer->stRoleData.RoleDetail.SkillPoint, pstPlayer->stRoleData.RoleDetail.SkillPoint,
							NULL,&stBefore,0,NULL);
		
		return 0;
	}
	else
	{
		unsigned int iCost = 0;
		int iMoneyType = 0;
		int i = 0;
		OPROLESKILLRUNECOST stCost;
		int iItemNum = 0;

		memset(&stCost,0,sizeof(stCost));
		
		if ( 0 == pstReq->OpType)
		{
			iCost = RUNE_REST_COST_MONEY;
			
			iMoneyType = ATTR_ID_BULL;

			stCost.Money = RUNE_REST_COST_MONEY;

			if (package_dec_money_test(pstEnv,pstPlayer,iMoneyType,iCost) < 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_SVR, ZONEERR_NOMONEY);
				return -1;
			}

			for(i = 0; i < (int)(sizeof(kRuneResetCostID)/sizeof(kRuneResetCostID[0])); i++)
			{
				int iID = kRuneResetCostID[i];
				int iPos = 0;
				if (!iID)
				{
					continue;
				}

				if (package_dec_test(pstEnv->pstAppCtx,pstEnv,pstPlayer, iID,1, LIST_TYPE_SUNDRIES, &iPos, 0) < 0)
				{
					ITEMDEF* pstDef = z_find_itemdef(pstEnv,iID);
					if (pstDef)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_SVR, ZONEERR_RUNE230,pstDef->Name);
					}
					
					return -1;
				}
				stCost.Item[iItemNum].ID = iID;
				stCost.Item[iItemNum].Num = 1;
				iItemNum++;
			}
		}
		else if (1 == pstReq->OpType)
		{
			iCost = RUNE_REST_COST_GOLD;

			iMoneyType = ATTR_ID_GODCOIN;

			stCost.Gold = RUNE_REST_COST_GOLD;

			if (package_dec_money_test(pstEnv,pstPlayer,iMoneyType,iCost) < 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_SVR, ZONEERR_NOMONEY2);
				return -1;
			}
		}
				
		iRet = package_dec_money(pstEnv,pstPlayer,iMoneyType,iCost);
		if (iMoneyType == ATTR_ID_GODCOIN)
		{
			godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
					pstPlayer->stRoleData.Level,GODCOIN_OPER_FLAG_RUNE,0,0,0,
					iCost,"符文属性重置");
		}
		else if (iMoneyType == ATTR_ID_BULL)
		{
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_RUNERESET, 0, iCost-iRet, iRet, "符文属性重置");
		}
		else if (ATTR_ID_MONEY == iMoneyType)
		{
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_RUNERESET, 
				0, 0, iCost, "符文属性重置");
		}

		if (0 == pstReq->OpType)
		{
			for(i = 0; i < (int)(sizeof(kRuneResetCostID)/sizeof(kRuneResetCostID[0])); i++)
			{
				int iID = kRuneResetCostID[i];
				int iPos = 0;
				if (!iID)
				{
					continue;
				}

				package_dec(pstEnv->pstAppCtx,pstEnv,pstPlayer, iID,1, LIST_TYPE_SUNDRIES, &iPos, PACKAGE_DEC_NONE, IMPITEM_OPTYPE_RUNE);
				
			}
		}
		
		pstRuneAddattr->iUnsavedRuneType = pstReq->RuneType;
		pstRuneAddattr->iUnsavedRuneAttrID = player_rune_addattr_rand_get(pstEnv,pstPlayer);

		memset(pstAction,0,sizeof(*pstAction));

		pstAction->ID = pstPlayer->iMemID;
		pstAction->ActionID = ACTION_RESET_RUNE_EX_RES;
		
		pstAction->Data.ResetRuneExRes.RuneExID = pstRuneAddattr->iUnsavedRuneAttrID;
		pstAction->Data.ResetRuneExRes.RuneType = pstRuneAddattr->iUnsavedRuneType ;

		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

		z_role_skill_oplog(pstEnv, pstPlayer, OP_RUNE_ATTR_RESET, 
							0, 0, 0,
							pstPlayer->stRoleData.RoleDetail.SkillPoint, pstPlayer->stRoleData.RoleDetail.SkillPoint,
							NULL,NULL,0,&stCost);
	}
	
	return 0;
}

int player_rune_enable_info_get_id(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i =0;
	int aiID[RUNE_YELLOW + 1];
	RUNEADDATTR *pstRuneAddattr = &pstPlayer->stRoleData.MiscInfo.RuneAddAttr;
	

	memset(aiID,0,sizeof(aiID));
	
	for(i = 0; i < pstRuneAddattr->Num; i++)
	{
		if (pstRuneAddattr->Info[i].RuneType > RUNE_YELLOW || 
			pstRuneAddattr->Info[i].RuneType < 0)
		{
			continue;
		}

		aiID[pstRuneAddattr->Info[i].RuneType] = pstRuneAddattr->Info[i].ID;
	}
	
	for (i = 0; i < MAX_RUNE_EX; i++)
	{
		RUNEEXENABLEINFO* pstInfo = &pstPlayer->stOnline.RuneExEnableInfo[i];

		if (pstInfo->RuneType > RUNE_YELLOW || 
			pstInfo->RuneType < 0)
		{
			continue;
		}
		pstInfo->RuneExID = aiID[pstInfo->RuneType];
		
	}
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}


int player_rune_ex_enable_info(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;		

	player_rune_enable_info_get_id(pstEnv,pstPlayer);

	memset(pstAction,0,sizeof(*pstAction));

	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_RUNE_EX_ENABLE_INFO;

	memcpy(&pstAction->Data.RuneExEnableInfo.RuneExEnableInfo,
		pstPlayer->stOnline.RuneExEnableInfo,
		sizeof(pstAction->Data.RuneExEnableInfo.RuneExEnableInfo));

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int player_rune_num_check(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i = 0;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;
	int* piColorCount = pstPlayer->stOnline.aiRuneColorCount;
	int k = 0;
	RESULT* pstResult = NULL;
	int m = 0;
	int iMinNum = -1;
	int aiLevelCount[RUNE_TYPE_NUM];

	memset(pstPlayer->stOnline.aiRuneColorCount,0,sizeof(pstPlayer->stOnline.aiRuneColorCount));

	memset(aiLevelCount,0,sizeof(aiLevelCount));

	for (i=0; i<pstZoneSkillList->nSkillNum; i++)
	{
		CSSKILLDATA* pstData = &pstZoneSkillList->astZoneSkills[i].stSkillData;
		ITEMDEF* pstDef = NULL;
		SKILLDEF* pstSkillDef = NULL;
		
		if (pstData->SkillID == 0)
		{
			continue;
		}

		pstSkillDef = z_find_skill_zoneskill(pstEnv, &pstZoneSkillList->astZoneSkills[i]);

		if (!pstSkillDef)
		{
			continue;
		}

		if(!pstSkillDef->RedRuneTransferedID && 
			!pstSkillDef->BlueRuneTransferedID &&
			!pstSkillDef->YellowRuneTransferedID )
		{
			continue;
		}
		
		for (m = 0; m < RUNE_TYPE_NUM; m++)
		{
			if (!pstData->RuneInfo[m].RuneItemID)
			{
				continue;
			}
			pstDef = z_find_itemdef(pstEnv,pstData->RuneInfo[m].RuneItemID);
			
			if (!pstDef )
			{
				continue;
				
			}

			
			pstResult = z_item_get_result(pstDef, RESULT_RUNE);
			if (!pstResult)
			{
				continue;
			}

			aiLevelCount[m] = max(aiLevelCount[m],pstResult->ResultVal1);

			if (pstResult->ResultVal1 >= 2)
			{
				piColorCount[m]++;
			}
		}
	}


	
	for( i = RUNE_RED; i < (int)(sizeof(pstPlayer->stOnline.aiRuneColorCount)/sizeof(pstPlayer->stOnline.aiRuneColorCount[0])); i++)
	{
		RUNEEXENABLEINFO* pstInfo = &pstPlayer->stOnline.RuneExEnableInfo[k];
		if (i > RUNE_YELLOW)
		{
			break;
		}
		if (k >= MAX_RUNE_EX)
		{
			break;
		}
		pstInfo->RuneType = i;
		pstInfo->RuneNum = piColorCount[i];

		if (iMinNum < 0)
		{
			iMinNum = pstInfo->RuneNum;
		}
		else
		{
			iMinNum = min(pstInfo->RuneNum,iMinNum);
		}

		if (pstInfo->RuneNum >= RUNE_EX_ENABLE_NUM)
		{
			ONERUNEADDATTR *pstOneRuneAddAttrInfo;
			pstOneRuneAddAttrInfo = player_rune_addattr_info_get(pstPlayer,i);
			if(!pstOneRuneAddAttrInfo || !pstOneRuneAddAttrInfo->ID)
			{
				OPROLESKILLRUNECHG stBefore;
				int iID = player_rune_addattr_rand_get(pstEnv,pstPlayer);

				rune_op_info(NULL,&stBefore,i,pstPlayer);
				
				player_rune_addattr_set(pstEnv,pstPlayer,iID,i);

				z_role_skill_oplog(pstEnv, pstPlayer, OP_RUNE_ATTR_ENABLE, 
								0, 0, 0,
								pstPlayer->stRoleData.RoleDetail.SkillPoint, pstPlayer->stRoleData.RoleDetail.SkillPoint,
								NULL,&stBefore,0,NULL);
			}
		}
		
		k++;
	}

	
	
	player_rune_ex_enable_info(pstEnv,pstPlayer);

	//成长之路
	if (iMinNum > 0)
	{
		ACHIEVEMENTCOND stCond;
		int iLevel = aiLevelCount[RUNE_RED];

		memset(&stCond,0,sizeof(stCond));

		for (m = RUNE_RED; m < RUNE_TYPE_NUM; m++)
		{
			iLevel = min(iLevel,aiLevelCount[m]);
		}
		
		stCond.Type = COND_RUNE_ALL_ADD;
		stCond.Val1 = iLevel;

		z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
	}
	
	return 0;
}




//尝试发动符文附加额外技能效果
int rune_addattr_do_skill_result(ZONESVRENV *pstEnv, ONERUNEADDATTR *pstRunAddattrInfo, 
		AtkProcess *pstAtkProcess, AtkeeInfo* pstAtkeeInfo)
{
	int i;
	Player *pstPlayer;
	RUNEADDATTRDEF *pstRunAddAttrDef;
	SKILLRESULT stSkillResult;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	if(pstAtkProcess->stAtkerInfo.stAtker.iType != OBJ_PLAYER)
	{
		return -1;
	}

	pstPlayer = pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer;
	
	pstRunAddAttrDef = z_find_run_addattr_def(pstEnv, pstRunAddattrInfo->ID);
	if(!pstRunAddAttrDef || pstRunAddAttrDef->AttrType != RUNE_ADD_ATTR_SKILL_RESULT)
	{
		return -1;
	}

	if (pstRunAddattrInfo->RuneType <= RUNE_EMPTY || pstRunAddattrInfo->RuneType > RUNE_YELLOW)
	{
		return -1;
	}
	
	if (pstPlayer->stOnline.aiRuneColorCount[pstRunAddattrInfo->RuneType] < RUNE_EX_ENABLE_NUM)
	{
		return -1;
	}
	//CD
	if(pstRunAddattrInfo->CoolDownEndTime > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return -1;
	}

	if(RAND1(1000000) >= pstRunAddAttrDef->Rate)
	{
		return -1;
	}
	
	for (i=0; i<MAX_RUNE_ATTR_RESULT; i++)
	{
		if(pstRunAddAttrDef->Result[i].ResultID == 0)
		{
			continue;
		}

		stSkillResult.SkillResultID = pstRunAddAttrDef->Result[i].ResultID;
		stSkillResult.SkillResultVal1 = pstRunAddAttrDef->Result[i].ResultVal1;
		stSkillResult.SkillResultVal2 = pstRunAddAttrDef->Result[i].ResultVal2;
		stSkillResult.SkillResultVal3 = pstRunAddAttrDef->Result[i].ResultVal3;
		stSkillResult.SkillResultVal4 = pstRunAddAttrDef->Result[i].ResultVal4;

		if(RESULT_SELF_STATUS == stSkillResult.SkillResultID)
		{
			if ((int)RAND1(10000) < stSkillResult.SkillResultVal1*100.0)
			{
				z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, stSkillResult.SkillResultVal2,
					stSkillResult.SkillResultVal3, stSkillResult.SkillResultVal4, 1, pstPlayer->iMemID);
			}
		}
		else
		{
			z_do_skill_result(pstEnv->pstAppCtx, pstEnv, pstAtkProcess, pstAtkeeInfo, &stSkillResult);
		}
	}

	//CD
	pstRunAddattrInfo->CoolDownEndTime = pstEnv->pstAppCtx->stCurr.tv_sec + pstRunAddAttrDef->CoolDown;

	pstActionRes->ActionID	= ACTION_DO_RUNE_EX;
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->Data.DoRuneEx.RuneExID = pstRunAddAttrDef->ID;
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

//符文附加额外技能效果
int rune_addattr_skill(ZONESVRENV *pstEnv, AtkProcess *pstAtkProcess, AtkeeInfo *pstAtkeeInfo)
{
	int i;
	Player *pstPlayer;
	ONERUNEADDATTR *pstRunAddattrInfo;
	RUNEADDATTRDEF *pstRunAddAttrDef;
	
	if(pstAtkProcess->stAtkerInfo.stAtker.iType != OBJ_PLAYER)
	{
		return -1;
	}

	pstPlayer = pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer;
	for(i = 0; i<pstPlayer->stRoleData.MiscInfo.RuneAddAttr.Num; i++)
	{
		pstRunAddattrInfo = &pstPlayer->stRoleData.MiscInfo.RuneAddAttr.Info[i];
		
		pstRunAddAttrDef = z_find_run_addattr_def(pstEnv, pstRunAddattrInfo->ID);
		if(!pstRunAddAttrDef || pstRunAddAttrDef->AttrType != RUNE_ADD_ATTR_SKILL_RESULT)
		{
			continue;
		}

		//尝试发动
		rune_addattr_do_skill_result(pstEnv, pstRunAddattrInfo, pstAtkProcess, pstAtkeeInfo);
	}
	return 0;
}

//符文附加额外人物自身属性
int rune_addattr_self(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i, j,k;
	RUNEADDATTRDEF *pstRunAddAttrDef;
	ONERUNEADDATTR *pstRunAddattrInfo;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;
	RESULT* pstResult = NULL;

	//符文本身增加的属性
	for (i=0; i<pstZoneSkillList->nSkillNum; i++)
	{
		CSSKILLDATA* pstData = &pstZoneSkillList->astZoneSkills[i].stSkillData;
		ITEMDEF* pstDef = NULL;
		
		if (pstData->SkillID == 0 || 
			pstData->RuneSlot > RUNE_YELLOW)
		{
			continue;
		}

		for(k = 0; k < RUNE_TYPE_NUM; k++)
		{
			pstDef = z_find_itemdef(pstEnv,pstData->RuneInfo[k].RuneItemID);
			
			if (!pstDef )
			{
				continue;
				
			}

			pstResult = z_item_get_result(pstDef, RESULT_RUNE);
			if (!pstResult)
			{
				continue;
			}

			for (j=0; j<MAX_ITEM_RESULT; j++)
			{
				if(pstDef->Result[j].ResultID == 0)
				{
					break;
				}

				z_attr_result(pstEnv, pstDyn, NULL, &pstDef->Result[j]);
			}
		}
		
	}

	

	for(i = 0; i<pstPlayer->stRoleData.MiscInfo.RuneAddAttr.Num; i++)
	{
		pstRunAddattrInfo = &pstPlayer->stRoleData.MiscInfo.RuneAddAttr.Info[i];

		pstRunAddAttrDef = z_find_run_addattr_def(pstEnv, pstRunAddattrInfo->ID);
		if(!pstRunAddAttrDef || pstRunAddAttrDef->AttrType != RUNE_ADD_ATTR_ARM_RESULT)
		{
			continue;
		}

		for (j=0; j<MAX_RUNE_ATTR_RESULT; j++)
		{
			if(pstRunAddAttrDef->Result[j].ResultID == 0)
			{
				break;
			}

			z_attr_result(pstEnv, pstDyn, NULL, &pstRunAddAttrDef->Result[j]);
		}
	}
	
	return 0;
}

//角色是否拥有此符文额外效果
int is_in_player_rune_addattr(Player *pstPlayer, int iID)
{
	int i;
	RUNEADDATTR *pstRuneAddattr = &pstPlayer->stRoleData.MiscInfo.RuneAddAttr;

	for(i=0; i<pstRuneAddattr->Num; i++)
	{
		if(pstRuneAddattr->Info[i].ID == iID)
		{
			return 1;
		}
	}
	
	return 0;
}

//随机抽取一个角色未拥有的符文额外效果
int player_rune_addattr_rand_get(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i;
	int iRand, iRate;
	int iBaseRand = 1000000;
	RUNEADDATTRDEF *pstRunAddAttrDef;
	RUNEADDATTR *pstRuneAddattr = &pstPlayer->stRoleData.MiscInfo.RuneAddAttr;

	for(i=0; i<pstRuneAddattr->Num; i++)
	{
		pstRunAddAttrDef = z_find_run_addattr_def(pstEnv, pstRuneAddattr->Info[i].ID);
		if(!pstRunAddAttrDef)
		{
			continue;
		}

		iBaseRand -= pstRunAddAttrDef->ResetRate;
	}

	iRate = 0;
	iRand = RAND1(1000000);
	for(i=0; i<pstEnv->pstZoneObj->iRuneAddAttrDef; i++)
	{
			pstRunAddAttrDef = (RUNEADDATTRDEF *)pstEnv->pstZoneObj->sRuneAddAttrDef[i];

			if(is_in_player_rune_addattr(pstPlayer, pstRunAddAttrDef->ID))
			{
				continue;
			}

			iRate += pstRunAddAttrDef->ResetRate;
			if(iRand < (int) iRate*1.0/iBaseRand*1000000)
			{
				return pstRunAddAttrDef->ID;
			}
	}
	
	return 0;
}

//获取角色已有某类符文额外效果信息
ONERUNEADDATTR* player_rune_addattr_info_get(Player *pstPlayer, int iRuneType)
{
	int i;
	RUNEADDATTR *pstRuneAddattr = &pstPlayer->stRoleData.MiscInfo.RuneAddAttr;
	
	for(i=0; i<pstRuneAddattr->Num; i++)
	{
		if(pstRuneAddattr->Info[i].RuneType == iRuneType)
		{
			return &pstRuneAddattr->Info[i];
		}
	}

	return NULL;
}

//设置角色额外符文效果
int player_rune_addattr_set(ZONESVRENV *pstEnv, Player *pstPlayer, int iRuneID, int iRuneType)
{
	ONERUNEADDATTR *pstOneRuneAddAttrInfo;
	RUNEADDATTR *pstRuneAddattr = &pstPlayer->stRoleData.MiscInfo.RuneAddAttr;
	
	pstOneRuneAddAttrInfo = player_rune_addattr_info_get(pstPlayer, iRuneType);
	if(pstOneRuneAddAttrInfo)
	{
		pstOneRuneAddAttrInfo->ID = iRuneID;
		pstOneRuneAddAttrInfo->CoolDownEndTime = 0;

		goto _final;
	}

	if(pstRuneAddattr->Num >= MAX_RUNE_ADD_ATTR)
	{
		return -1;
	}

	pstRuneAddattr->Info[pstRuneAddattr->Num].ID = iRuneID;
	pstRuneAddattr->Info[pstRuneAddattr->Num].RuneType = iRuneType;
	pstRuneAddattr->Info[pstRuneAddattr->Num].CoolDownEndTime = 0;
	pstRuneAddattr->Num++;


_final:
	
	z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	return 0;
}

int player_skill_rune_enable(ZONESVRENV * pstEnv, Player * pstPlayer)
{
	int i;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;
	ZoneSkill *pstZoneSkill = NULL;


	for (i=0; i<pstZoneSkillList->nSkillNum; i++)
	{
		pstZoneSkill = &pstZoneSkillList->astZoneSkills[i];
		// 通知客户端更新
		pstZoneSkill->stSkillData.RuneItemID = 0;
		pstZoneSkill->stSkillData.RuneSlot = RUNE_EMPTY;
		pstZoneSkill->stSkillData.RuneSlotEnabled = 1;
		z_skill_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstZoneSkill);
	}
	return 0;
	
}

int rune_op_info(ZoneSkill *pstZoneSkill,OPROLESKILLRUNECHG* pstRuneChg,int iRuneType,Player* pstPlayer)
{
	int i = 0;
	int iNum = 0;
	int k = 0;
	
	memset(pstRuneChg,0,sizeof(*pstRuneChg));
	
	if (pstZoneSkill)
	{
		for(i = RUNE_EMPTY +1; i < RUNE_TYPE_NUM; i++)
		{
			if (iNum >= MAX_RUNE_ADD_ATTR)
			{
				break;
			}

			pstRuneChg->RuneInfo[iNum].RuneType = i;
			
			pstRuneChg->RuneInfo[iNum].RuneItemID = pstZoneSkill->stSkillData.RuneInfo[i].RuneItemID;
			if (pstPlayer)
			{
				for (k = 0; k < MAX_RUNE_EX; k++)
				{
					RUNEEXENABLEINFO* pstInfo = &pstPlayer->stOnline.RuneExEnableInfo[k];
					ONERUNEADDATTR *pstOneRuneAddAttrInfo;

					

					if (pstInfo->RuneType != i)
					{
						continue;
					}
					pstRuneChg->RuneInfo[iNum].RuneNum = pstInfo->RuneNum;
					pstRuneChg->RuneInfo[iNum].RuneAttrID = pstInfo->RuneExID;
					
					pstOneRuneAddAttrInfo = player_rune_addattr_info_get(pstPlayer,i);
					if (pstOneRuneAddAttrInfo)
					{
						pstRuneChg->RuneInfo[iNum].RuneAttrID = pstOneRuneAddAttrInfo->ID;
					}
					
					break;
					
				}
			}

			iNum++;
		}
	}
	else
	{
		if (pstPlayer)
		{
			for (i = 0; i < MAX_RUNE_EX; i++)
			{
				RUNEEXENABLEINFO* pstInfo = &pstPlayer->stOnline.RuneExEnableInfo[i];
				ONERUNEADDATTR *pstOneRuneAddAttrInfo;

				

				if (pstInfo->RuneType != iRuneType)
				{
					continue;
				}
				pstRuneChg->RuneInfo[0].RuneType = iRuneType;
				pstRuneChg->RuneInfo[0].RuneNum = pstInfo->RuneNum;
				pstRuneChg->RuneInfo[0].RuneAttrID = pstInfo->RuneExID;

				pstOneRuneAddAttrInfo = player_rune_addattr_info_get(pstPlayer,iRuneType);
				if (pstOneRuneAddAttrInfo)
				{
					pstRuneChg->RuneInfo[0].RuneAttrID = pstOneRuneAddAttrInfo->ID;
				}
				
				break;
				
			}
		}
	}
	return 0;

}

//该符文只能镶嵌到哪个技能上
int rune_only_inlay_which_skill(ZONESVRENV *pstEnv, const ITEMDEF *pstDef)
{
	int i = 0;
	for (i = 0; i < (int)CCH(pstDef->Result); ++i)
	{
		if (!pstDef->Result[i].ResultID)
		{
			break;
		}
		if (pstDef->Result[i].ResultID == RESULT_RUNE && 
			pstDef->Result[i].ResultVal1 >= 0 && 
			pstDef->Result[i].ResultVal1 <= 2 &&
			pstDef->Result[i].ResultVal2 > RUNE_EMPTY &&
			pstDef->Result[i].ResultVal2 < RUNE_TYPE_NUM)
		{
			return pstDef->Result[i].ResultVal3;
		}
	}
	
	return 0;
}


