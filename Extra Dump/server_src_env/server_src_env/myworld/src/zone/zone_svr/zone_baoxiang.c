/*
**  @file $RCSfile: zone_baoxiang.c,v $
**  general description of this module
**  $Id: zone_baoxiang.c,v 1.29 2014/06/17 15:39:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2014/06/17 15:39:59 $
**  @version $Revision: 1.29 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_clt.h"
#include "zone_player.h"
#include "comm_misc.h"
#include "zone_package.h"
#include "zone_itemlimit.h"
#include "zone_err.h"
#include "zone_mail.h"
#include "zone_shop.h"
#include "zone_zh_name.h"
#include "zone_oplog.h"

int g_aiBoxCardIdxMap[MAX_CARD_BOX_CARD_NUM];
int g_iBoxCardRangeCurr;

BAOXIANGDEF* z_find_baoxiang_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	BAOXIANGDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.ID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sBaoXiangDef, pstObj->iBaoXiangDef, BAOXIANG_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (BAOXIANGDEF*)pstObj->sBaoXiangDef[iIndex];
		
	}
	return NULL;
}

CARDBOXDEF* z_find_cardbox_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	CARDBOXDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.ID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sCardBoxDef, pstObj->iCardBoxDef, CARD_BOX_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (CARDBOXDEF *)pstObj->sCardBoxDef[iIndex];
		
	}
	return NULL;
}

CARDBOXGROUPDEF* z_find_cardbox_group_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	CARDBOXGROUPDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.GroupID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sCardBoxGroupDef, pstObj->iCardBoxGroupDef, CARD_BOX_GROUP_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (CARDBOXGROUPDEF *)pstObj->sCardBoxGroupDef[iIndex];
		
	}
	return NULL;
}

int baoxiang_hide_item_get(ZONESVRENV* pstEnv, Player *pstPlayer, BAOXIANGDEF *pstBaoXiangDef, ROLEGRID *pstRoleGrid)
{
	ROLEGRID stRoleGrid;

	if(pstBaoXiangDef->HideItemID <= 0 || pstBaoXiangDef->HideCount <= 0)
	{
		return -1;
	}
	
	if(0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstBaoXiangDef->HideItemID))
	{
		return -1;
	}

	if(stRoleGrid.Type == GRID_ITEM_ITEM)
	{
		stRoleGrid.GridData.RoleItem.Num = pstBaoXiangDef->HideItemNum;
	}

	//先看能不能放下这个物品到背包，再看可否获得，防止损失好东西
	if(0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0))
	{
		return -1;
	}

	if(1 == world_baoxiang_add(pstEnv, pstBaoXiangDef->ID))
	{
		*pstRoleGrid = stRoleGrid;
		return 0;
	}

	return -1;
}


int baoxiang_rand_get(ZONESVRENV* pstEnv, BAOXIANGDEF *pstBaoXiangDef, ROLEGRID *pstRoleGrid)
{
	int i;
	int iRate = 0, iRand = 0;
	BAOXIANGITEM *pstBaoXiangItem = NULL;

	iRand = RAND1(1000000);
	for(i=0; i<MAX_BAOXIANG_ITEM; i++)
	{
		iRate += pstBaoXiangDef->Items[i].Rate;
		if(pstBaoXiangDef->Items[i].ItemID == 0)
		{
			continue;
		}

		if(iRand < iRate)
		{
			pstBaoXiangItem = &pstBaoXiangDef->Items[i];
			break;
		}
	}

	if(!pstBaoXiangItem)
	{
		return -1;
	}

	if(0 > item_create(pstEnv->pstAppCtx, pstEnv, pstRoleGrid, pstBaoXiangItem->ItemID))
	{
		return -1;
	}

	if(pstRoleGrid->Type == GRID_ITEM_ITEM)
	{
		pstRoleGrid->GridData.RoleItem.Num = pstBaoXiangItem->Num;
	}

	if(pstBaoXiangItem->Bind)
	{
		pstRoleGrid->GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
	}

	return pstBaoXiangItem->Broadcast;
}

int player_baoxiang_open(ZONESVRENV* pstEnv, Player *pstPlayer, CSBAOXIANGOPEN *pstReq)
{
	ITEMDEF *pstItemDef, *pstAwardItemDef;
	ROLEITEM *pstRoleItem;
	ROLEGRID stRoleGrid;
	BAOXIANGDEF *pstBaoXiangDef;
	RESULT *pstResult;
	int iPos, fBroadcast, iAwardNum = 0;
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	ROLEPACKAGE *pstPack = &pstPlayer->stRoleData.Package;
	int iDec = 0;
	GIFTITEM stDec[2];
	GIFTITEM stAdd;
	
	if (0 > package_get_item(pstEnv, pstPlayer, pstReq->ListType, 
							pstReq->GridIdx, &pstItemDef, &pstRoleItem, &iPos))
	{
		goto _error;
	}

	pstResult = z_item_get_result(pstItemDef, RESULT_BAOXIANG);
	if(!pstResult)
	{
		goto _error;
	}

	pstBaoXiangDef = z_find_baoxiang_def(pstEnv, pstResult->ResultVal1);
	if(!pstBaoXiangDef)
	{
		goto _error;
	}

	//需要有一个空格子
	if(pstPack->SundriesPackage.MaxNum - pstPack->SundriesPackage.Num < 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PWORLD109);
		goto _error;
	}

	//检查开宝箱所需其他物品
	if(pstBaoXiangDef->NeedItemID)
	{
		if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstBaoXiangDef->NeedItemID, 1, pstReq->ListType, NULL,PACKAGE_DEC_NONE))
		{
			z_sys_strf(pstEnv,pstPlayer,SYS_MIDDLE2, pstBaoXiangDef->ErrorTip);
			goto _error;
		}
	}
	
	fBroadcast = baoxiang_rand_get(pstEnv, pstBaoXiangDef, &stRoleGrid);
	if(fBroadcast < 0)
	{
		goto _error;
	}

	if(0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
			pstItemDef->ItemID, 1, pstReq->ListType, &iPos, PACKAGE_DEC_NONE, IMPITEM_OPTYPE_BAOXIANG))
	{
		goto _error;
	}

	stDec[0].ItemID = pstItemDef->ItemID;
	stDec[0].ItemNum = 1;
	iDec++;

	//扣除开宝箱所需其他物品
	if(pstBaoXiangDef->NeedItemID)
	{
		if(0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
			pstBaoXiangDef->NeedItemID, 1, pstReq->ListType, NULL, PACKAGE_DEC_NONE, IMPITEM_OPTYPE_BAOXIANG))
		{
			goto _error;
		}

		stDec[1].ItemID = pstBaoXiangDef->NeedItemID;
		stDec[1].ItemNum = 1;
		iDec++;
	}
	
	//最最后在看隐藏物品是否触发，防止被错误处理误增加开箱子次数
	if(0 == baoxiang_hide_item_get(pstEnv, pstPlayer, pstBaoXiangDef, &stRoleGrid))
	{
		//开到隐藏物品了
		fBroadcast = 1;
	}

	iAwardNum = stRoleGrid.GridData.RoleItem.Num;
	
	package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1, 0, IMPITEM_OPTYPE_BAOXIANG);
	stAdd.ItemID = stRoleGrid.GridData.RoleItem.DefID;
	stAdd.ItemNum = iAwardNum;

	pstMiscSvr->PlayerMiscSvrData.BaoXiangAwardInfo.Succ = 1;
	pstMiscSvr->PlayerMiscSvrData.BaoXiangAwardInfo.ItemWID = stRoleGrid.GridData.RoleItem.WID;
	pstMiscSvr->PlayerMiscSvrData.BaoXiangAwardInfo.ItemID = stRoleGrid.GridData.RoleItem.DefID;
	pstMiscSvr->PlayerMiscSvrData.BaoXiangAwardInfo.BaoXiangItemID = pstItemDef->ItemID;
	pstMiscSvr->PlayerMiscSvrData.BaoXiangAwardInfo.Num = iAwardNum;

	pstAwardItemDef = z_find_itemdef(pstEnv, stRoleGrid.GridData.RoleItem.DefID);
	if(pstAwardItemDef && fBroadcast)
	{
		z_sys_strf_broadcast(pstEnv, SYS_MIDDLE, 
			ZONEERR_BROAD301, 
			pstPlayer->stRoleData.RoleName,
			pstItemDef->Name,
			pstAwardItemDef->Name,
			iAwardNum);
	}

	z_role_open_box(pstEnv,pstPlayer,1, iDec, 1, stDec, &stAdd);
	
_final:

	pstMiscSvr->PlayerMiscType = PLAYER_BAOXIANG_AWARD_INFO;
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
_error:

	memset(&pstMiscSvr->PlayerMiscSvrData.BaoXiangAwardInfo, 0,
			sizeof(pstMiscSvr->PlayerMiscSvrData.BaoXiangAwardInfo));
	
	pstMiscSvr->PlayerMiscSvrData.BaoXiangAwardInfo.Succ = 0;
	
	goto _final;
	return 0;
}

int cardbox_grididx_rand_init(Player *pstPlayer)
{
	int i, j;
	CardBoxOL *pstCardOL = &pstPlayer->stOnline.stCardBoxOL;

	if(pstCardOL->iBoxSelectCount >= 4)
	{
		g_iBoxCardRangeCurr = MAX_CARD_BOX_CARD_NUM;
	}
	else
	{
		g_iBoxCardRangeCurr = 8;
	}

	for(i=0; i<g_iBoxCardRangeCurr; i++)
	{
		g_aiBoxCardIdxMap[i] = i;
	}

	for(i=g_iBoxCardRangeCurr -1; i>=0; i--)
	{
		for(j=pstCardOL->iCardNum-1; j>=0; j--)
		{
			if(pstCardOL->astCardInfo[j].iSelect != 0 &&
				pstCardOL->astCardInfo[j].iGridIdx == g_aiBoxCardIdxMap[i])
			{
				g_iBoxCardRangeCurr--;
				if(g_iBoxCardRangeCurr != i)
				{
					 g_aiBoxCardIdxMap[i] = g_aiBoxCardIdxMap[g_iBoxCardRangeCurr];
				}
			}
		}
	}

	return 0;
}

int cardbox_rand_idx_get()
{
	int iRand, iGridIdx;

	if(g_iBoxCardRangeCurr <=0)
		return -1;

	iRand = RAND1(g_iBoxCardRangeCurr);
	iGridIdx = g_aiBoxCardIdxMap[iRand];

	g_iBoxCardRangeCurr--;
	if(iRand != g_iBoxCardRangeCurr)
	{
		g_aiBoxCardIdxMap[iRand] = g_aiBoxCardIdxMap[g_iBoxCardRangeCurr];
	}
	
	return iGridIdx;
}

int cardbox_card_range_get(BOXGROUPSELECT *pstGroup)
{
	int i;
	int iRand, iRate;

	iRate = 0;
	iRand = RAND1(100);
	for(i=0; i<MAX_CARD_BOX_ITEM_RANGE; i++)
	{
		iRate += pstGroup->Range[i].Rate;
		if(iRand < iRate)
		{
			return pstGroup->Range[i].Num;
		}
	}
	
	return 0;
}

int cardbox_grid_item_create(Player *pstPlayer, CARDBOXITEM *pstCardBoxItem, int iGroupID, int iIdx)
{
	CardSelectGrid *pstCardGrid;
	CardBoxOL *pstCardOL = &pstPlayer->stOnline.stCardBoxOL;

	if(pstCardOL->iCardNum >= MAX_CARD_BOX_CARD_NUM)
	{
		return -1;
	}

	pstCardGrid = &pstCardOL->astCardInfo[pstCardOL->iCardNum++];

	pstCardGrid->iGridIdx = cardbox_rand_idx_get();
	pstCardGrid->iGroupID = iGroupID;
	pstCardGrid->iIdx = iIdx;
	pstCardGrid->iSelect = 0;
	pstCardGrid->iAwardIdx = 0;
	pstCardGrid->stItem = *pstCardBoxItem;
	
	return 0;
}

int cardbox_grid_award_item_create(Player *pstPlayer, CARDBOXITEM *pstCardBoxItem, int iGroupID, int iIdx)
{
	CardSelectGrid *pstCardGrid;
	CardBoxOL *pstCardOL = &pstPlayer->stOnline.stCardBoxOL;

	if(pstCardOL->iCardNum >= MAX_CARD_BOX_CARD_NUM)
	{
		return -1;
	}

	pstCardGrid = &pstCardOL->astCardInfo[pstCardOL->iCardNum++];

	pstCardGrid->iGridIdx = cardbox_rand_idx_get();
	pstCardGrid->iGroupID = iGroupID;
	pstCardGrid->iIdx = iIdx;
	pstCardGrid->iSelect = 0;
	pstCardGrid->iAwardIdx = pstCardOL->iCardNum;
	pstCardGrid->stItem = *pstCardBoxItem;
	
	return 0;
}

int cardbox_group_item_create(ZONESVRENV* pstEnv, Player *pstPlayer, CARDBOXDEF *pstCardBoxDef, int iGroupID, int iNum)
{
	int i, iRate, iRand;
	int iBase, iCreateNum;
	char sMap[MAX_CARD_BOX_GROUP_ITEM];
	CARDBOXGROUPDEF *pstCardBoxGroupDef;
	CardBoxOL *pstCardOL = &pstPlayer->stOnline.stCardBoxOL;

	pstCardBoxGroupDef = z_find_cardbox_group_def(pstEnv, iGroupID);
	if(!pstCardBoxGroupDef)
	{
		return -1;
	}

	memset(sMap, 0, sizeof(sMap));

	for(i = 0; i<pstCardOL->iCardNum; i++)
	{
		if(pstCardOL->astCardInfo[i].iGroupID == iGroupID)
		{
			sMap[pstCardOL->astCardInfo[i].iIdx] = 1;
		}
	}

	iBase = 0;
	for(i = 0; i<MAX_CARD_BOX_GROUP_ITEM; i++)
	{
		if(sMap[i])
		{
			continue;
		}
		
		iBase += pstCardBoxGroupDef->Cards[i].SelectRate;
	}

	iCreateNum = iNum;
	while(iCreateNum-- && iBase > 0)
	{
		iRate = 0;
		iRand = RAND1(iBase);
		for(i = 0; i<MAX_CARD_BOX_GROUP_ITEM; i++)
		{
			if(iBase <= 0)
			{
				break;
			}

			if(sMap[i])
			{
				continue;
			}
			
			iRate += pstCardBoxGroupDef->Cards[i].SelectRate;
			if(iRand < iRate)
			{
				iBase -= pstCardBoxGroupDef->Cards[i].SelectRate;
				sMap[i] = 1;
				
				cardbox_grid_item_create(pstPlayer, &pstCardBoxGroupDef->Cards[i], iGroupID, i);
				break;
			}	
		}
	}
	
	return 0;
}

int cardbox_group_award_item_create(ZONESVRENV* pstEnv, Player *pstPlayer, int iGroupID)
{
	int i, iRate, iRand;
	int iBase, iCreateNum, iMax;
	char sMap[MAX_CARD_BOX_GROUP_ITEM];
	CARDBOXGROUPDEF *pstCardBoxGroupDef;
	CardBoxOL *pstCardOL = &pstPlayer->stOnline.stCardBoxOL;

	pstCardBoxGroupDef = z_find_cardbox_group_def(pstEnv, iGroupID);
	if(!pstCardBoxGroupDef)
	{
		return -1;
	}

	memset(sMap, 0, sizeof(sMap));
	for(i = 0; i<pstCardOL->iCardNum; i++)
	{
		if(pstCardOL->astCardInfo[i].iGroupID == iGroupID)
		{
			sMap[pstCardOL->astCardInfo[i].iIdx] = 1;
		}
	}

	if(pstCardOL->iBoxSelectCount >= 4)
	{
		iCreateNum = 1;
		iMax = 1;
	}
	else
	{
		iCreateNum = MAX_GROUP_ITEM_AWARD_RATE_NUM - pstCardOL->iCardNum;
		iMax = MAX_GROUP_ITEM_AWARD_RATE_NUM;
	}
	
	while(iCreateNum--)
	{
		if(pstCardOL->iCardNum >= iMax)
		{
			break;
		}
		
		iBase = 0;
		for(i = 0; i<MAX_CARD_BOX_GROUP_ITEM; i++)
		{
			if(sMap[i])
			{
				continue;
			}
			
			iBase += pstCardBoxGroupDef->Cards[i].AwardRate[pstCardOL->iCardNum];
		}

		if( iBase <= 0)
		{
			break;
		}
	
		iRate = 0;
		iRand = RAND1(iBase);
		for(i = 0; i<MAX_CARD_BOX_GROUP_ITEM; i++)
		{
			if(iBase <= 0)
			{
				break;
			}

			if(sMap[i])
			{
				continue;
			}
			
			iRate += pstCardBoxGroupDef->Cards[i].AwardRate[pstCardOL->iCardNum];
			if(iRand < iRate)
			{
				iBase -= pstCardBoxGroupDef->Cards[i].AwardRate[pstCardOL->iCardNum];
				sMap[i] = 1;
				
				cardbox_grid_award_item_create(pstPlayer, &pstCardBoxGroupDef->Cards[i], iGroupID, i);
				break;
			}	
		}
	}
	
	return 0;
}

int cardbox_wash_clear(Player *pstPlayer)
{
	int i;
	CardBoxOL *pstCardOL = &pstPlayer->stOnline.stCardBoxOL;

	for(i=pstCardOL->iCardNum-1; i>=0; i--)
	{
		if(pstCardOL->astCardInfo[i].iSelect == 0)
		{
			pstCardOL->iCardNum--;
			if(pstCardOL->iCardNum != i)
			{
				 pstCardOL->astCardInfo[i] = pstCardOL->astCardInfo[pstCardOL->iCardNum];
			}
		}
	}
	
	return 0;
}

int cardbox_group_select_num_get(Player *pstPlayer, int iGroupID)
{
	int i;
	int iCount = 0;
	CardBoxOL *pstCardOL = &pstPlayer->stOnline.stCardBoxOL;

	for(i=0; i <pstCardOL->iCardNum; i++)
	{
		if(pstCardOL->astCardInfo[i].iGroupID == iGroupID)
		{
			iCount++;
		}
	}
	
	return iCount;
}

int cardbox_limit_item_add(ZONESVRENV* pstEnv, Player *pstPlayer, CARDBOXDEF *pstCardBoxDef)
{
	int i;
	CardSelectGrid *pstCardGrid;
	CARDBOXGROUPDEF *pstCardBoxGroupDef;
	CardBoxOL *pstCardOL = &pstPlayer->stOnline.stCardBoxOL;

	if(pstCardOL->iCardNum >= 8)
	{
		return -1;
	}
	
	if(0 == world_cardbox_check(pstEnv, pstCardBoxDef->ID))
	{
		return -1;
	}

	for(i=0; i<pstCardOL->iCardNum; i++)
	{
		if(pstCardOL->astCardInfo[i].iSelect &&
			pstCardOL->astCardInfo[i].stItem.ItemID == pstCardBoxDef->LimitID)
		{
			return -1;
		}
	}

	pstCardBoxGroupDef = z_find_cardbox_group_def(pstEnv, pstCardBoxDef->GroupID);
	if(!pstCardBoxGroupDef)
	{
		return -1;
	}

	for(i=0; i<MAX_CARD_BOX_GROUP_ITEM; i++)
	{
		if(pstCardBoxGroupDef->Cards[i].ItemID == pstCardBoxDef->LimitID)
		{
			pstCardGrid = &pstCardOL->astCardInfo[pstCardOL->iCardNum++];
			
			pstCardGrid->iGroupID = pstCardBoxGroupDef->GroupID;
			pstCardGrid->iIdx = i;
			pstCardGrid->iSelect = 0;
			pstCardGrid->iAwardIdx = 0;
			pstCardGrid->iGridIdx = cardbox_rand_idx_get();
			pstCardGrid->stItem = pstCardBoxGroupDef->Cards[i];
			break;
		}
	}
	
	return 0;
}

int cardbox_card_create(ZONESVRENV* pstEnv, Player *pstPlayer, CARDBOXDEF *pstCardBoxDef)
{
	int iNum, iMaxNum;
	CardBoxOL *pstCardOL = &pstPlayer->stOnline.stCardBoxOL;

	if(pstCardOL->iBoxSelectCount >= 4)
	{
		iMaxNum = MAX_CARD_BOX_CARD_NUM;
		pstCardOL->iCardNum = 0;
	}
	else
	{
		iMaxNum = 8;
	}

	//清理掉没有选择的卡牌
	cardbox_wash_clear(pstPlayer);
	cardbox_grididx_rand_init(pstPlayer);

	if(iMaxNum == MAX_CARD_BOX_CARD_NUM)
	{
		cardbox_group_award_item_create(pstEnv, pstPlayer, pstCardBoxDef->AddGroupID);
		iNum  = iMaxNum - pstCardOL->iCardNum;
		if(iNum <= 0)
		{
			return 0;
		}

		cardbox_group_item_create(pstEnv, pstPlayer, pstCardBoxDef, pstCardBoxDef->AddGroupID, iNum);
	}
	else
	{
		if(pstCardOL->iCardNum < MAX_GROUP_ITEM_AWARD_RATE_NUM)
		{
			cardbox_group_award_item_create(pstEnv, pstPlayer, pstCardBoxDef->GroupID);
		}

		//世界计数的先准备出来
		cardbox_limit_item_add(pstEnv, pstPlayer, pstCardBoxDef);
		
		iNum  = iMaxNum - pstCardOL->iCardNum;
		if(iNum <= 0)
		{
			return 0;
		}
		
		cardbox_group_item_create(pstEnv, pstPlayer, pstCardBoxDef, pstCardBoxDef->GroupID, iNum);
	}

	if(pstCardOL->iCardNum != iMaxNum)
	{
		return -1;
	}
	
	return 0;
}

int is_cardbox_free_grididx(Player *pstPlayer, int iGridIdx)
{
	int i;
	CardBoxOL *pstCardOL = &pstPlayer->stOnline.stCardBoxOL;

	if(iGridIdx >= pstCardOL->iCardNum)
	{
		return 0;
	}
	
	for(i=0; i<pstCardOL->iCardNum; i++)
	{
		if(pstCardOL->astCardInfo[i].iSelect && 
			pstCardOL->astCardInfo[i].iGridIdx == iGridIdx)
		{
			return 0;
		}
	}
	
	return 1;
}

int player_cardbox_open_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	CSPKG stPkg;
	CSCARDBOXSVR *pstCardBoxSvr = &stPkg.Body.CardBoxSvr;
	CardBoxOL *pstCardOL = &pstPlayer->stOnline.stCardBoxOL;

	pstCardBoxSvr->Type = CARDBOX_SVR_TYPE_BOX_INFO;
	pstCardBoxSvr->Data.CardBoxInfo.ID= pstCardOL->iBoxID;
	pstCardBoxSvr->Data.CardBoxInfo.CostCount = pstCardOL->iBoxCostCount;
	pstCardBoxSvr->Data.CardBoxInfo.SelectCount = pstCardOL->iBoxSelectCount;
	pstCardBoxSvr->Data.CardBoxInfo.Wash = pstCardOL->iBoxWashCount;
	pstCardBoxSvr->Data.CardBoxInfo.CardNum = pstCardOL->iCardNum;


	for(i=0; i<pstCardOL->iCardNum; i++)
	{
		pstCardBoxSvr->Data.CardBoxInfo.CardInfo[i].Select = pstCardOL->astCardInfo[i].iSelect;
		pstCardBoxSvr->Data.CardBoxInfo.CardInfo[i].GridIdx = pstCardOL->astCardInfo[i].iGridIdx;
		pstCardBoxSvr->Data.CardBoxInfo.CardInfo[i].ItemID = pstCardOL->astCardInfo[i].stItem.ItemID;
		pstCardBoxSvr->Data.CardBoxInfo.CardInfo[i].Num = pstCardOL->astCardInfo[i].stItem.Num;
	}

	Z_CSHEAD_INIT(&stPkg.Head, CARD_BOX_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int player_cardbox_open(ZONESVRENV* pstEnv, Player *pstPlayer, CSCARDBOXOPEN *pstCSCardBoxOpen)
{
	int iPos;
	ITEMDEF *pstItemDef;
	CARDBOXDEF *pstCardBoxDef;
	ROLEITEM *pstRoleItem;
	CardBoxOL *pstCardOL = &pstPlayer->stOnline.stCardBoxOL;
	GIFTITEM stDec;
	
	if (0 > package_get_item(pstEnv, pstPlayer, pstCSCardBoxOpen->ListType, 
							pstCSCardBoxOpen->GridIdx, &pstItemDef, &pstRoleItem, &iPos))
	{
		return -1;;
	}

	pstCardBoxDef = z_find_cardbox_def(pstEnv, pstItemDef->ItemID);
	if(!pstCardBoxDef)
	{
		return -1;
	}

	if(pstCardOL->iStat == 1)
	{
		return -1;
	}

	if(0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
			pstItemDef->ItemID, 1, pstCSCardBoxOpen->ListType, &iPos, PACKAGE_DEC_NONE, IMPITEM_OPTYPE_CARDBOX))
	{
		return -1;
	}

	stDec.ItemID = pstItemDef->ItemID;
	stDec.ItemNum = 1;

	memset(pstCardOL, 0, sizeof(*pstCardOL));
	pstCardOL->iStat  = 1;
	pstCardOL->iBoxID = pstCardBoxDef->ID;
	pstCardOL->iBoxCostCount = 1;

	world_cardbox_add(pstEnv, pstCardBoxDef->ID, pstCardBoxDef->GodGoin[pstCardOL->iBoxSelectCount]);

	if(0 > cardbox_card_create(pstEnv, pstPlayer, pstCardBoxDef))
	{
		return -1;
	}

	player_cardbox_open_info(pstEnv, pstPlayer);

	z_role_open_box(pstEnv,pstPlayer,2, 1, 0, &stDec, NULL);
	return 0;
}


int z_sys_strf_pkg(ZONESVRENV *pstEnv, CSPKG *pstPkg,int iSysType, int iErrno, ...)
{
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	const char* pszErr;
	va_list marker;

	va_start( marker, iErrno );
	pszErr	=	ZONE_GET_ERR(iErrno, 0);

	pstSysMsg->Type = iSysType;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;

	if( !pszErr )
	{
		snprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), "SysNo : %d", iErrno );
	}
	else
	{
		vsnprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), pszErr, marker);
	}

	pstSysMsg->Msg[sizeof(pstSysMsg->Msg) - 1] = 0;
	va_end(marker);

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	return 0;
}

int player_cardbox_select(ZONESVRENV* pstEnv, Player *pstPlayer, CSCARDBOXSELECT *pstCSCardBoxSelect)
{
	int i, iSelectIdx, iAward;
	ROLEGRID stRoleGrid;
	ITEMDEF *pstAwardItemDef;
	ITEMDEF *pstItemDef;
	CARDBOXDEF *pstCardBoxDef;
	CSPKG stPkg;
	CSCARDBOXSVR *pstCardBoxSvr = &stPkg.Body.CardBoxSvr;
	CSCARDBOXSELECTINFO *pstCardSelectInfo = &pstCardBoxSvr->Data.SelectInfo;
	CardBoxOL *pstCardOL = &pstPlayer->stOnline.stCardBoxOL;
	GIFTITEM stAdd;
	
	if(pstCardOL->iStat == 0)
	{
		return -1;
	}

	pstCardBoxDef = z_find_cardbox_def(pstEnv, pstCardOL->iBoxID);
	if(!pstCardBoxDef)
	{
		return -1;
	}

	pstItemDef = z_find_itemdef(pstEnv, pstCardBoxDef->ID);
	if(!pstItemDef)
	{
		return -1;
	}

	if(pstCardOL->iBoxSelectCount >= pstCardOL->iBoxCostCount)
	{
		return -1;
	}

	if(!is_cardbox_free_grididx(pstPlayer, pstCSCardBoxSelect->GridIdx))
	{
		return -1;
	}

	iAward = pstCardOL->iBoxSelectCount+1;
	if(pstCardOL->iBoxSelectCount >= 4)
	{
		iAward = 1;
	}

	iSelectIdx = -1;
	for(i = 0; i<pstCardOL->iCardNum; i++)
	{
		if(pstCardOL->astCardInfo[i].iSelect)
		{
			continue;
		}

		if(pstCardOL->astCardInfo[i].iAwardIdx == iAward)
		{
			iSelectIdx = i;
			break;
		}
	}

	if(world_cardbox_check(pstEnv, pstCardOL->iBoxID) &&
		pstCardOL->iBoxSelectCount < 4)
	{
		for(i = 0; i<pstCardOL->iCardNum; i++)
		{
			if(pstCardOL->astCardInfo[i].iSelect == 0 &&
				pstCardBoxDef->LimitID == pstCardOL->astCardInfo[i].stItem.ItemID)
			{
				iSelectIdx = i;

				world_cardbox_clear(pstEnv, pstCardOL->iBoxID);
			}
		}
	}

	if(iSelectIdx < 0)
	{
		return -1;
	}

	pstAwardItemDef = z_find_itemdef(pstEnv, pstCardOL->astCardInfo[iSelectIdx].stItem.ItemID);
	if(!pstAwardItemDef)
	{
		return -1;
	}

	if(0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstCardOL->astCardInfo[iSelectIdx].stItem.ItemID))
	{
		return -1;
	}

	if(pstCardOL->astCardInfo[iSelectIdx].stItem.Bind)
	{
		stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
	}

	stAdd.ItemID = pstCardOL->astCardInfo[iSelectIdx].stItem.ItemID;
	stAdd.ItemNum = 1;
	
	if(stRoleGrid.Type == GRID_ITEM_ITEM)
	{
		stRoleGrid.GridData.RoleItem.Num = pstCardOL->astCardInfo[iSelectIdx].stItem.Num;
		stAdd.ItemNum = pstCardOL->astCardInfo[iSelectIdx].stItem.Num;
	}

	pstCardOL->astCardInfo[iSelectIdx].iSelect = 1;
	pstCardOL->astCardInfo[iSelectIdx].iGridIdx = pstCSCardBoxSelect->GridIdx;

	cardbox_grididx_rand_init(pstPlayer);
	for(i=0; i<pstCardOL->iCardNum; i++)
	{
		if(pstCardOL->astCardInfo[i].iSelect)
		{
			continue;
		}

		pstCardOL->astCardInfo[i].iGridIdx = cardbox_rand_idx_get();
	}
	
	pstCardOL->iBoxSelectCount++;

	if(0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0))
	{
		char szText[256];

		snprintf(szText, sizeof(szText), 
			LJY_MAIL100, 
			pstItemDef->Name, pstAwardItemDef->Name);
		
		player_sysmail_fast_send(pstEnv, pstPlayer->stRoleData.RoleName, 
					pstItemDef->Name, 
					szText, 
					&stRoleGrid, 1, 0,NULL, 0);
		
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_BAOXIANG200);
	}
	else
	{
		package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1, 0, IMPITEM_OPTYPE_CARDBOX_SELECT);
	}
	
	if( pstCardOL->astCardInfo[iSelectIdx].stItem.Broadcast)
	{
		CSPKG stPkg;
		z_sys_strf_pkg(pstEnv, &stPkg,SYS_SHOW, ZONEERR_CARDBOX100, 
						pstPlayer->stRoleData.RoleName,
						pstItemDef->Name,
						pstAwardItemDef->Name,
						pstCardOL->astCardInfo[iSelectIdx].stItem.Num);

		if (pstEnv->pstConf->CountryVer == COUNTRY_VER_KOREA)
		{
			if (pstItemDef->ItemID == 5600308 || pstItemDef->ItemID == 5600315 ||
				pstItemDef->ItemID == 5250404 || pstItemDef->ItemID == 5610411)
			{
				z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
			
				z_pkg_all_world(pstEnv,&stPkg);
			}
		}
		else
		{
			z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
			
			z_pkg_all_world(pstEnv,&stPkg);
		}
	}

	pstCardBoxSvr->Type = CARDBOX_SVR_TYPE_SELECT_INFO;
	pstCardSelectInfo->ID = pstCardOL->iBoxID;
	pstCardSelectInfo->Count = pstCardOL->iBoxSelectCount;
	pstCardSelectInfo->SelectInfo.ItemID = pstCardOL->astCardInfo[iSelectIdx].stItem.ItemID;
	pstCardSelectInfo->SelectInfo.Num = pstCardOL->astCardInfo[iSelectIdx].stItem.Num;
	pstCardSelectInfo->SelectInfo.Select = pstCardOL->astCardInfo[iSelectIdx].iSelect;
	pstCardSelectInfo->SelectInfo.GridIdx = pstCardOL->astCardInfo[iSelectIdx].iGridIdx;
	
	pstCardSelectInfo->CardNum = pstCardOL->iCardNum;
	for(i=0; i<pstCardOL->iCardNum; i++)
	{
		pstCardSelectInfo->CardInfo[i].Select = pstCardOL->astCardInfo[i].iSelect;
		pstCardSelectInfo->CardInfo[i].GridIdx = pstCardOL->astCardInfo[i].iGridIdx;
		pstCardSelectInfo->CardInfo[i].ItemID = pstCardOL->astCardInfo[i].stItem.ItemID;
		pstCardSelectInfo->CardInfo[i].Num = pstCardOL->astCardInfo[i].stItem.Num;
	}

	Z_CSHEAD_INIT(&stPkg.Head, CARD_BOX_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);


	z_role_open_box(pstEnv,pstPlayer,3, 0, 1, NULL, &stAdd);
	return 0;
}

int player_cardbox_next(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CARDBOXDEF *pstCardBoxDef;
	CardBoxOL *pstCardOL = &pstPlayer->stOnline.stCardBoxOL;
	
	if(pstCardOL->iStat == 0)
	{
		return -1;
	}

	if(pstCardOL->iBoxCostCount != pstCardOL->iBoxSelectCount)
	{
		return -1;
	}

	if(pstCardOL->iBoxSelectCount >= 5)
	{	
		return -1;
	}

	pstCardBoxDef = z_find_cardbox_def(pstEnv, pstCardOL->iBoxID);
	if(!pstCardBoxDef)
	{
		return -1;
	}

	if(pstCardOL->iBoxSelectCount < 4)
	{
		if(0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_GODCOIN, pstCardBoxDef->GodGoin[pstCardOL->iBoxSelectCount]))
		{
			return -1;
		}
		package_dec_money(pstEnv, pstPlayer, ATTR_ID_GODCOIN, pstCardBoxDef->GodGoin[pstCardOL->iBoxSelectCount]);

		godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
						pstPlayer->stRoleData.Level, GODCOIN_OPER_FLAG_CARDBOX, pstCardBoxDef->ID, 0, 0,
						pstCardBoxDef->GodGoin[pstCardOL->iBoxSelectCount], "抽牌的箱子");

		world_cardbox_add(pstEnv, pstCardBoxDef->ID, pstCardBoxDef->GodGoin[pstCardOL->iBoxSelectCount]);
	}
	else
	{
		if(0 > cardbox_card_create(pstEnv, pstPlayer, pstCardBoxDef))
		{
			return -1;
		}
	}

	pstCardOL->iBoxCostCount++;
	player_cardbox_open_info(pstEnv, pstPlayer);
	return 0;
}

int player_cardbox_close(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSCARDBOXSVR *pstCardBoxSvr = &stPkg.Body.CardBoxSvr;
	CardBoxOL *pstCardOL = &pstPlayer->stOnline.stCardBoxOL;
	
	memset(pstCardOL, 0, sizeof(*pstCardOL));
	
	pstCardBoxSvr->Type = CARDBOX_SVR_TYPE_CLOSE;
	pstCardBoxSvr->Data.Close = 1;
	Z_CSHEAD_INIT(&stPkg.Head, CARD_BOX_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int player_cardbox_wash(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CARDBOXDEF *pstCardBoxDef;
	CardBoxOL *pstCardOL = &pstPlayer->stOnline.stCardBoxOL;
	
	if(pstCardOL->iStat == 0)
	{
		return -1;
	}

	if(pstCardOL->iBoxWashCount >= 3)
	{
		return -1;
	}

	if(pstCardOL->iBoxSelectCount >= 4)
	{
		return -1;
	}

	pstCardBoxDef = z_find_cardbox_def(pstEnv, pstCardOL->iBoxID);
	if(!pstCardBoxDef)
	{
		return -1;
	}

	if(0 > cardbox_card_create(pstEnv, pstPlayer, pstCardBoxDef))
	{
		return -1;
	}

	pstCardOL->iBoxWashCount++;
	player_cardbox_open_info(pstEnv, pstPlayer);
	return 0;
}

int player_cardbox_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSCARDBOXCLT *pstCardBoxClt = &pstCsPkg->Body.CardBoxClt;

	if (pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
		return -1;
	}
	
	switch(pstCardBoxClt->Type)
	{
		case CARDBOX_CLT_TYPE_OPEN:
			player_cardbox_open(pstEnv, pstPlayer, &pstCardBoxClt->Data.Open);
			break;
		case CARDBOX_CLT_TYPE_SELECT:
			player_cardbox_select(pstEnv, pstPlayer, &pstCardBoxClt->Data.Select);
			break;
		case CARDBOX_CLT_TYPE_NEXT:
			player_cardbox_next(pstEnv, pstPlayer);
			break;
		case CARDBOX_CLT_TYPE_CLOSE:
			player_cardbox_close(pstEnv, pstPlayer);
			break;
		case CARDBOX_CLT_TYPE_WASH:
			player_cardbox_wash(pstEnv, pstPlayer);
			break;
		default:
			break;
	}
	
	return 0;
}
