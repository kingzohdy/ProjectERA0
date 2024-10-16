#include "zone_back.h"
#include "zone_package.h"
#include "zone_svr.h"
#include "zone_arm.h"
#include "zone_clt.h"
#include "zone_safelock.h"

BACKLEVELDEF * z_find_back_level_def(ZONESVRENV *pstEnv, int iItemID)
{
	int iIndex;
	int iEqu;
	BACKLEVELDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.ItemID = iItemID;
	iIndex = bsearch_int(&stDef, pstObj->sBackLevelDef, pstObj->iBackLevelDef, BACK_LEVEL_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (BACKLEVELDEF*)pstObj->sBackLevelDef[iIndex];
		
	}
	return NULL;
}

BACKLEVELDEF *z_find_chg_back_def(ZONESVRENV *pstEnv, int iType, int iLevel)
{
	int i;
	BACKLEVELDEF *pstDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	for(i=0; i < pstObj->iBackLevelDef; i++)
	{
		pstDef = (BACKLEVELDEF*)pstObj->sBackLevelDef[i];
		if(pstDef->SubType == iType && pstDef->Level == iLevel)
		{
			return pstDef;
		}
	}
	
	return NULL;
}

int player_back_levelup(ZONESVRENV* pstEnv, Player *pstPlayer, CSBACKLEVELUP *pstCSLevelUP)
{
	int iDec, iBind = 0;
	int iStuffPos, iPos;
	ROLEITEM *pstRoleItem;
	ROLEGRID *pstArmGrid, stRoleGrid;
	ITEMDEF *pstItemDef, *pstArmItemDef;
	BACKLEVELDEF *pstBackLevelDef;
	RESULT *pstResult;
	CSPKG stPkg;
	CSBACKSVR *pstBackSvr = &stPkg.Body.BackSvr;
	
	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		goto _error;
	}

	if(pstCSLevelUP->Back.ListType != LIST_TYPE_SUNDRIES)
	{
		goto _error;
	}
	
	pstArmGrid = get_pak_item_grid(pstPlayer, pstCSLevelUP->Back.ListType, pstCSLevelUP->Back.GridIdx);
	if (!pstArmGrid || pstArmGrid->Type != GRID_ITEM_ARM)
	{
		goto _error;
	}

	pstArmItemDef = z_get_itemdef_roleitem(pstEnv, &pstArmGrid->GridData.RoleItem);
	if (NULL == pstArmItemDef || pstArmItemDef->WearPos1 != WEAR_POS_FASHION_BACK)
	{
		goto _error;
	}

	if (0 > package_get_item(pstEnv, pstPlayer, pstCSLevelUP->BeUsed.ListType, 
							pstCSLevelUP->BeUsed.GridIdx, &pstItemDef, &pstRoleItem, &iStuffPos))
	{
		goto _error;
	}

	pstResult = z_item_get_result(pstItemDef, RESULT_BACK_LEVELUP);
	if(!pstResult)
	{
		goto _error;
	}

	pstBackLevelDef = z_find_back_level_def(pstEnv, pstArmItemDef->ItemID);
	if(!pstBackLevelDef)
	{
		goto _error;
	}

	if(pstBackLevelDef->Level != pstResult->ResultVal1)
	{
		goto _error;
	}

	if(0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstBackLevelDef->NextItemID))
	{
		goto _error;
	}

	iDec = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
			pstItemDef->ItemID, 1, pstCSLevelUP->BeUsed.ListType, &iStuffPos, PACKAGE_DEC_NONE, IMPITEM_OPTYPE_BACK_LEVELUP_DEC_STUFF);
	if(0 > iDec)
	{
		goto _error;
	}

	if(iDec > 0)
	{
		iBind = 1;
	}

	iPos = pkg_get_pos(pstPlayer, pstCSLevelUP->Back.ListType, pstCSLevelUP->Back.GridIdx);
	if(0 > iPos)
	{
		goto _error;
	}

	iDec = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
			pstArmItemDef->ItemID, 1, pstCSLevelUP->Back.ListType, &iPos, 
			PACKAGE_DEC_NONE, IMPITEM_OPTYPE_BACK_LEVELUP_ARM);
	if (0 > iDec)
	{
		goto _error;
	}

	if(iDec > 0)
	{
		iBind = 1;
	}

	if(iBind)
	{
		stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
	}

	 package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid,  1,0, IMPITEM_OPTYPE_BACK_LEVELUP_NEXT_ARM);

	pstBackSvr->Data.LevelUPSucc.Succ = 1;
	pstBackSvr->Data.LevelUPSucc.NewItemWID = stRoleGrid.GridData.RoleItem.WID;

_final:
	pstBackSvr->Type = BACK_SVR_LEVELUP_SUCC;
	Z_CSHEAD_INIT(&stPkg.Head, BACK_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
_error:
	pstBackSvr->Data.LevelUPSucc.Succ = 0;
	goto _final;
	return 0;
}

int player_back_chg(ZONESVRENV* pstEnv, Player *pstPlayer, CSBACKCHG *pstCSChg)
{
	int iDec, iBind = 0;
	int iStuffPos, iPos;
	ROLEITEM *pstRoleItem;
	ROLEGRID *pstArmGrid, stRoleGrid;
	ITEMDEF *pstItemDef, *pstArmItemDef;
	BACKLEVELDEF *pstBackLevelDef, *pstNewBackLevelDef;
	RESULT *pstResult;
	CSPKG stPkg;
	CSBACKSVR *pstBackSvr = &stPkg.Body.BackSvr;
	
	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		goto _error;
	}

	if(pstCSChg->Back.ListType != LIST_TYPE_SUNDRIES)
	{
		goto _error;
	}
	
	pstArmGrid = get_pak_item_grid(pstPlayer, pstCSChg->Back.ListType, pstCSChg->Back.GridIdx);
	if (!pstArmGrid || pstArmGrid->Type != GRID_ITEM_ARM)
	{
		goto _error;
	}

	pstArmItemDef = z_get_itemdef_roleitem(pstEnv, &pstArmGrid->GridData.RoleItem);
	if (NULL == pstArmItemDef || pstArmItemDef->WearPos1 != WEAR_POS_FASHION_BACK)
	{
		goto _error;
	}

	if (0 > package_get_item(pstEnv, pstPlayer, pstCSChg->BeUsed.ListType, 
							pstCSChg->BeUsed.GridIdx, &pstItemDef, &pstRoleItem, &iStuffPos))
	{
		goto _error;
	}

	pstResult = z_item_get_result(pstItemDef, RESULT_BACK_CHG_TYPE);
	if(!pstResult)
	{
		goto _error;
	}

	pstBackLevelDef = z_find_back_level_def(pstEnv, pstArmItemDef->ItemID);
	if(!pstBackLevelDef || pstBackLevelDef->Level != pstBackLevelDef->Level)
	{
		goto _error;
	}

	pstNewBackLevelDef = z_find_chg_back_def(pstEnv, pstResult->ResultVal1, pstBackLevelDef->Level);
	if(!pstBackLevelDef)
	{
		goto _error;
	}

	if(0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstNewBackLevelDef->ItemID))
	{
		goto _error;
	}

	iDec = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
			pstItemDef->ItemID, 1, pstCSChg->BeUsed.ListType, &iStuffPos, PACKAGE_DEC_NONE, IMPITEM_OPTYPE_BACK_CHGTYPE_ARM);
	if(0 > iDec)
	{
		goto _error;
	}

	if(iDec > 0)
	{
		iBind = 1;
	}

	iPos = pkg_get_pos(pstPlayer, pstCSChg->Back.ListType, pstCSChg->Back.GridIdx);
	if(0 > iPos)
	{
		goto _error;
	}

	iDec = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
			pstArmItemDef->ItemID, 1, pstCSChg->Back.ListType, &iPos, 
			PACKAGE_DEC_NONE, IMPITEM_OPTYPE_BACK_CHGTYPE_DEC_STUFF);
	if (0 > iDec)
	{
		goto _error;
	}

	if(iDec > 0)
	{
		iBind = 1;
	}

	if(iBind)
	{
		stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
	}

	 package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid,  1,0, IMPITEM_OPTYPE_BACK_CHGTYPE_NEXT_ARM);

	pstBackSvr->Data.ChgTypeSucc.Succ = 1;
	pstBackSvr->Data.ChgTypeSucc.NewItemWID = stRoleGrid.GridData.RoleItem.WID;

_final:
	pstBackSvr->Type = BACK_SVR_CHGTYPE_SUCC;
	Z_CSHEAD_INIT(&stPkg.Head, BACK_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
_error:
	pstBackSvr->Data.LevelUPSucc.Succ = 0;
	goto _final;
	return 0;
}

int player_back_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSBACKCLT *pstCSBackClt = &pstCsPkg->Body.BackClt;

	switch(pstCSBackClt->Type)
	{
		case BACK_CLT_LEVELUP:
			return player_back_levelup(pstEnv, pstPlayer, &pstCSBackClt->Data.LevelUP);
			break;
		case BACK_CLT_CHG_TYPE:
			return player_back_chg(pstEnv, pstPlayer, &pstCSBackClt->Data.ChgType);
			break;
		default:
			break;
	}
	return 0;
}