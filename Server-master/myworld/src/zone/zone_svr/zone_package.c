/*
**  @file $RCSfile: zone_package.c,v $
**  general description of this module
**  $Id: zone_package.c,v 1.1125 2014/07/01 08:24:26 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/07/01 08:24:26 $
**  @version $Revision: 1.1125 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_package.h"
#include "role_db_meta.h"
#include "zone_clt.h"
#include "zone_db.h"
#include "zone_err.h"
#include "zone_attr.h"
#include "zone_skill.h"
#include "zone_move.h"
#include "zone_status.h"
#include "zone_act_status.h"
#include "zone_npc.h"
#include "zone_team.h"
#include "zone_map.h"
#include "zone_tmp.h"
#include "zone_task.h"
#include "zone_player.h"
#include "zone_script.h"
#include "zone_pet.h"
#include "proto_comm.h"
#include "zone_relation.h"
#include "zone_oplog.h"
#include "zone_pet.h"
#include "mapmask.h"
#include "zone_combat.h"
#include "zone_designation.h"
#include "zone_respact.h"
#include "zone_fairy.h"
#include "zone_arm.h"
#include "zone_ai.h"
#include "zone_attr.h"
#include "zone_range.h"
#include "zone_machine.h"
#include "zone_secondaryskill.h"
#include "zone_range.h"
#include "zone_roll.h"
#include "zone_booty.h"
#include "zone_safelock.h"
#include "zone_city.h"
#include "zone_clan_city.h"
#include "zone_addattr.h"
#include "zone_misc.h"
#include "zone_span.h"
#include "zone_clan.h"
#include "zone_genius.h"
#include "zone_vip.h"
#include "zone_sparta.h"
#include "zone_losttotem.h"
#include "zone_diffpworld.h"
#include "zone_clan_advance.h"
#include "zone_ploy.h"
#include "zone_clan_pet.h"

extern ScriptEnv stSEnv;
extern ZONESVRENV gs_stEnv;

int g_invaild_dec = 0;
int g_use_pre = 0;

extern int g_iOptype;


#define INIT_EXTENDED_CDTIME 10
#define HOLE_MIN_LEVEL 50

#define ITEM_EXP_BUFFID  1039



//#define MAX_PICK_DIST 400

typedef struct tagTowardGird
{
	int  iLevel;
	int  iGird;
	int  iType;
}ToWardGrid;

#define GEM_STAT_NO 0
#define GEM_STAT_POWER 1
#define GEM_STAT_NO_POWER 2

static ToWardGrid ToWardGrids[] = 
{
	{10, 8, LIST_TYPE_SUNDRIES},
	{30, 8, LIST_TYPE_SUNDRIES},
	{20, 8, LIST_TYPE_STUFF},
	{40, 8, LIST_TYPE_STUFF},
};

static int  OpenStashEt[]={3000,500,20,10};
static int  OpenSundEt[]={10000,10000,3000,500,20,10};
static int  StashEtMoneys[]={10000,50000,150000,300000};

int player_item_do_tmpsys(ZONESVRENV* pstEnv, Player *pstPlayer,  RESULT *pstResult);
static int pkg_add_hole(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult);
int z_arm_extra_dur(ITEMDEF *pstItemDef);
int package_booty_notify_team(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZoneTeam *pstTeam, Player *pstPlayer,
									ROLEGRID *pstRoleGrid, int iNum);
int pack_freevaild_grid_num(ROLEGRID*pstPackageRoleGrid, int iNum, int iMaxNum, int iValidGridNum);

static int pkg_use_item_switch_check(ZONESVRENV* pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef,
									ROLEITEM *pstRoleItem);

int package_use_clan_contri_check(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEITEM *pstRoleItem);

int package_use_clan_contri(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEITEM *pstRoleItem);
int pkg_use_add_wenbao(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEITEM *pstRoleItem, 
									ITEMDEF *pstItemDef, CSPACKAGEUSE *pstUse);
int pkg_use_add_impexp(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEITEM *pstRoleItem, 
									ITEMDEF *pstItemDef, CSPACKAGEUSE *pstUse);

int PakGridIdxCompare(const void *p1, const void *p2)
{
	ROLEGRID *pstGrid1 = (ROLEGRID *)p1;
	ROLEGRID *pstGrid2 = (ROLEGRID *)p2;
	
	return pstGrid2->GridData.RoleItem.GridIdx - pstGrid1->GridData.RoleItem.GridIdx;
}

int PakItemIdxCompare(const void *p1, const void *p2)
{
	ROLEITEM *pstItem1 = (ROLEITEM *)p1;
	ROLEITEM *pstItem2 = (ROLEITEM *)p2;
	
	return pstItem2->GridIdx - pstItem1->GridIdx;
}


// 获取背包有效格子数
int get_vaild_gird(ZONESVRENV * pstEnv, ROLEPACKAGE *pstPkg, int iPkgType)
{
	//ROLEPACKAGE *pstPkg = &pstPlayer->stRoleData.Package;
	int iVaildNum=0;
	int i;
	ROLEITEM *pstRoleItem;
	//ROLEGRID *pstRoleGrid;
	ITEMDEF *pstItemDef;
	RESULT *pstResult;

	if (iPkgType == LIST_TYPE_SUNDRIES)
	{
		iVaildNum = pstPkg->SundriesPackage.MaxNum;
		for (i=0; i<pstPkg->EtSundPkg.Num; i++)
		{
			pstRoleItem = &pstPkg->EtSundPkg.RoleItems[i];

			// 失效了
			if (pstRoleItem->ValidEnd != 0 && pstRoleItem->ValidEnd < pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				pstItemDef = z_find_itemdef(pstEnv, pstRoleItem->DefID);
				if (pstItemDef )
				{
					pstResult = z_item_get_result(pstItemDef, RESULT_ADD_SUNDPKG_GRID);
					if (pstResult)
					{
						iVaildNum -= pstResult->ResultVal1;
					}		
				}
			}
		}
	}
	else if(iPkgType == LIST_TYPE_STASH)
	{
		iVaildNum = pstPkg->StashPackage.MaxNum;
		for (i=0; i<pstPkg->EtStashPkg.Num; i++)
		{
			pstRoleItem = &pstPkg->EtStashPkg.RoleItems[i];
			// 失效了
			if (pstRoleItem->ValidEnd != 0 && pstRoleItem->ValidEnd < pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				pstItemDef = z_find_itemdef(pstEnv, pstRoleItem->DefID);
				if (pstItemDef )
				{
					pstResult = z_item_get_result(pstItemDef, RESULT_ADD_SUNDPKG_GRID);
					if (pstResult)
					{
						iVaildNum -= pstResult->ResultVal1;
					}		
				}
			}
		}
	}
	else if (iPkgType == LIST_TYPE_TASK)
	{
		// 任务包没有失效这一说
		return pstPkg->TaskPackage.MaxNum;
	}
	else if (iPkgType == LIST_TYPE_ET_STASH)
	{
		return pstPkg->EtStashPkg.MaxNum;
	}
	else if (iPkgType == LIST_TYPE_ET_SUND)
	{
		return pstPkg->EtSundPkg.MaxNum;
	}
	else if (iPkgType == LIST_TYPE_SHOPPING)
	{
		return pstPkg->ShoppingPackage.MaxNum;
	}
	else if (iPkgType == LIST_TYPE_STUFF)
	{
		return -1;
	}
	else
	{
		return -1;
	}
	return iVaildNum;
}

// 返回一个玩家的契约ID，如没有返回0.
unsigned int z_qiyue_id_from_player( ZONESVRENV * pstEnv, Player * pstPlayer )
{	
	UNUSED(pstEnv);
	
	return pstPlayer->stRoleData.Package.QiYueInfo.CurrQiYueID;
}

static inline int rand_range(int iLhs, int iRhs)
{
	int iLow = iLhs < iRhs ? iLhs : iRhs;
	int iRange = iLhs < iRhs ? iRhs - iLhs : iLhs - iRhs;
	return iLow + (int)RAND1(iRange);
}

FIXEDADDATTR* z_find_fixedaddattr(ZONESVRENV *pstEnv, int iID)
{
	FIXEDADDATTR stDummy;
	int iIndex = 0, fEquals = 0;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDummy.ID = iID;
	iIndex = bsearch_int(&stDummy, pstObj->sFixedAddAttr, pstObj->iFixedAddAttr,
	                     FIXED_ADD_ATTR_DATA, &fEquals);
	if(fEquals)
	{
		return (FIXEDADDATTR*)pstObj->sFixedAddAttr[iIndex];
	}
	return NULL;
}

ITEMSTARRANDDEF *z_find_item_star_rand_def(ZONESVRENV* pstEnv, int iID)
{
	ITEMSTARRANDDEF stItemStarRandDef;
	int iIndex = 0;
	int iEqu = 0;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stItemStarRandDef.ID= iID;
	iIndex = bsearch_int(&stItemStarRandDef, pstObj->sItemStarRandDef, pstObj->iItemStarRandDef, ITEM_STAR_RAND_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ITEMSTARRANDDEF *)pstObj->sItemStarRandDef[iIndex];
	}
	
	return NULL;	
}

EXCHGITEMDEF *z_find_exchg_item_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex = 0;
	int iEqu = 0;
	EXCHGITEMDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.ID= iID;
	iIndex = bsearch_int(&stDef, pstObj->sExchgItemDef, pstObj->iExchgItemDef, EXCHG_ITEM_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (EXCHGITEMDEF *)pstObj->sExchgItemDef[iIndex];
	}

	return NULL;
}

/* 如果已有物品索引的定义,这个函数尽量少用
 * 重构时可以考虑把数据存储在hash表里面 查找起来更快
 * note by ouyang v20101215
 */
ITEMDEF *z_find_itemdef(ZONESVRENV* pstEnv, int iItemID)
{
	ITEMDEF  stItem;
	int iIndex = 0;
	int iEqu = 0;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stItem.ItemID = iItemID;
	iIndex = bsearch_int(&stItem, pstObj->sItemDef, pstObj->iItemDef, ITEM_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ITEMDEF *)pstObj->sItemDef[iIndex];
	}
	
	return NULL;	
}
/* 如果已有物品索引的定义,这个函数尽量少用
 * 重构时可以考虑把数据存储在hash表里面 查找起来更快
 * note by ouyang v20101215
 */
RIDEDEF *z_find_ride_def(ZONESVRENV *pstEnv, unsigned short iRideID, unsigned short iRideLevel)
{
	int iIndex = 0;
	int iEqu = 0;
	RIDEDEF stRide;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stRide.RideID = iRideID;
	stRide.Level = iRideLevel;
	iIndex = bsearch_int(&stRide, pstObj->sRideDef, pstObj->iRideDef, RIDE_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (RIDEDEF *)pstObj->sRideDef[iIndex];
	}

	return NULL;
}

RIDEPINZHIDEF *z_find_ride_pinzhi_def(ZONESVRENV *pstEnv, int iPinZhi)
{
	int iIndex = 0;
	int iEqu = 0;
	RIDEPINZHIDEF stRidePinZhi;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stRidePinZhi.PinZhi = iPinZhi;
	iIndex = bsearch_int(&stRidePinZhi, pstObj->sRidePinZhiDef, pstObj->iRidePinZhiDef, RIDE_PINZHI_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (RIDEPINZHIDEF *)pstObj->sRidePinZhiDef[iIndex];
	}

	return NULL;
}

ITEMDEF *z_get_itemdef_roleitem(ZONESVRENV* pstEnv, ROLEITEM *pstRoleItem)
{
	ITEMDEF *pstItemDef = NULL;
	ITEMDEF stItemDef;
	int iItemIndex = 0;
	int iEqu = 0;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
		
	if (pstRoleItem->DefPos >= 0 && pstRoleItem->DefPos  < pstObj->iItemDef)
	{
		pstItemDef = (ITEMDEF *)pstObj->sItemDef[pstRoleItem->DefPos];
		if ((int)pstItemDef->ItemID == pstRoleItem->DefID)
		{
			return pstItemDef;
		}
	}

	stItemDef.ItemID = pstRoleItem->DefID;
	iItemIndex = bsearch_int(&stItemDef, pstObj->sItemDef, pstObj->iItemDef, ITEM_DEF_DATA, &iEqu);
	if (iEqu)
	{
		pstRoleItem->DefPos = iItemIndex;
		return (ITEMDEF *)pstObj->sItemDef[iItemIndex];
	}

	return NULL;
}


/*
 *根据MapID   去找MapRandTransmit表的一记录
 *note by lzk   time:20130606
 */
MAPRANDTRANSMIT *z_find_map_rand_transmit_def(ZONESVRENV *pstEnv, int iMapID)
{
	int iIndex = 0;
	int iEqu = 0;
	MAPRANDTRANSMIT stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.MapID = iMapID;
	iIndex = bsearch_int(&stDef, pstObj->sMapRandTransmitDef, pstObj->iMapRandTransmitDef, MAP_RAND_TRANS_DATA, &iEqu);
	if (iEqu)
	{
		return (MAPRANDTRANSMIT *)pstObj->sMapRandTransmitDef[iIndex];
	}

	return NULL;
}

/*
 *地图随机传送符道具检查
 *note by lzk time:20130606
 */
int map_rand_transmit_check(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	MAPRANDTRANSMIT *pstMapRandTrans;
	int iMapIndex = pstPlayer->stRoleData.Map;
	MapIndex *pstMapIdx;
	
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, iMapIndex);
	if(NULL == pstMapIdx)
	{
		goto _error;
	}
	
	pstMapRandTrans = z_find_map_rand_transmit_def(pstEnv,iMapIndex);
	if(NULL == pstMapRandTrans)
	{
		goto _error;
	}

	if(0 > pstMapRandTrans->PosNum)
	{
		goto _error;
	}

	if(pstMapRandTrans->PosNum > MAX_MAP_RAND_POS_NUM)
	{
		goto _error;
	}

	if (0 > z_player_goto_check(pstEnv, pstPlayer, 0, NULL))
	{
		return -1;
	}
	
	return 0;
_error:
	z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PACKAGE_301);
	return -1;
}

/*
 *地图随机传送符道具，在当前地图上随机传送到一个坐标点
 *note by lzk time:20130606
 */
int map_rand_transmit(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	MAPRANDTRANSMIT *pstMapRandTrans;
	int iRand = 0;
	RESPOS stResPos, stResPos1;
	int iMapIndex = pstPlayer->stRoleData.Map;
 	MapIndex *pstMapIdx;
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, iMapIndex);
	if(NULL == pstMapIdx)
	{
		goto _error;
	}

	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos	=	-1;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		goto _error;
	}

	pstMapRandTrans = z_find_map_rand_transmit_def(pstEnv,iMapIndex);
	if(NULL == pstMapRandTrans)
	{
		goto _error;
	}

	if(0 > pstMapRandTrans->PosNum)
	{
		goto _error;
	}

	if(pstMapRandTrans->PosNum > MAX_MAP_RAND_POS_NUM)
	{
		goto _error;
	}
	
	iRand = RAND1(pstMapRandTrans->PosNum);
	stResPos.X = pstMapRandTrans->Pos[iRand].X;
	stResPos.Y = pstMapRandTrans->Pos[iRand].Y;

	if (NULL == z_pos_area_dyn(pstMapIdx, pstEnv->pstAreaDynPool, pstMapInst, &stResPos))
	{
		goto _error;
	}

	if (0 == z_valid_pos(pstEnv->pstMapObj, iMapIndex, &pstMapInst->iMapPos, &stResPos))
	{
		goto _error;
	}

	z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &stResPos, &stResPos, 
						300, 0, &stResPos1);

	z_player_change_map(pstEnv->pstAppCtx, pstEnv, pstPlayer, iMapIndex, &stResPos1, 0, 0);
	
	return 0;
	
_error:
	z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PACKAGE_301);
	return -1;

}


/* 
 * 加购买的物品到商城的背包
 * note by ouyang v20101215
 */
int package_add_in_shopping(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ROLEPACKAGE *pstPlayerPackage, ROLEGRID *pstGrid, ROLEPACKAGECHG *pstPackageChg)
{
	ITEMDEF *pstItemDef = NULL;
	int iListType = 0;
	SHOPPINGPACKAGE *pstShoppingPackage = &pstPlayerPackage->ShoppingPackage;

	/* 判断下格子的类型 */
	switch(pstGrid->Type)
	{
		case GRID_ITEM_ITEM:
		case GRID_ITEM_ARM: //RoleArm与RoleItem前两个字段相同
			pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstGrid->GridData.RoleItem);
			if (NULL == pstItemDef)
			{
				return -1;
			}

			if (GRID_ITEM_ITEM == pstGrid->Type)
			{
				if (ITEM_TYPE_ARM == pstItemDef->ItemType)
				{
					return -1;
				}
			}
			else
			{
				if (ITEM_TYPE_ARM != pstItemDef->ItemType)
				{
					return -1;
				}
			}
			
			break;
		default:
			return -1;
			break;
	}
	if(!pstItemDef)
	{
		return -1;
	}
       /* 物品类型处理 */
	switch(pstItemDef->ItemType)
	{
		case ITEM_TYPE_ARM:
		case ITEM_TYPE_ITEM:	
		case ITEM_TYPE_STUFF:
			iListType = LIST_TYPE_SUNDRIES;
			break;
		//case ITEM_TYPE_STUFF:
			//iListType = LIST_TYPE_STUFF;
			//break;
		//case ITEM_TYPE_AMM:
			//iListType = LIST_TYPE_AMM;
			//break;
		case ITEM_TYPE_TASK:
			 iListType =  LIST_TYPE_TASK;
			 break;		
		default:
			return -1;
			break;
	}
	iListType = LIST_TYPE_SHOPPING; //强制转换下
	if (0 > package_add_grid_in(pstAppCtx, pstEnv, pstShoppingPackage->RoleGrids, &pstShoppingPackage->Num, pstShoppingPackage->MaxNum,
										iListType, pstGrid, pstItemDef, pstPackageChg,pstPlayerPackage))
	{
		return -1;
	}
	return 0;
}
// iShoppingFlag 1表示先加商品背包 2表示先加杂物背包 
int package_add_test(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, const ROLEGRID *pstGrid, int iGridNum,int iShoppingFlag)
{
	//return package_add_test_all(pstAppCtx,pstEnv,pstPlayer,pstGrid,iGridNum,0);
	int i = 0;
	ROLEGRID *pstRoleGrid = NULL;
	ROLEGRID stTmpGrid;
	ITEMDEF *pstItemDef = NULL;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	ROLEPACKAGE stRolePack;
	ROLEPACKAGECHG stPackageChg;

	assert(iGridNum <= MAX_CHG_NUM);

	stPackageChg.ItemChg.ChgNum = 0;
	stPackageChg.ArmChg.ChgNum = 0;

	memcpy(&stRolePack, pstPak, sizeof(*pstPak));
	
	//todo:处理BOOTY_CLAN_WAR_BRAND_ITEM_ID
	for(i=0; i<iGridNum; i++)
	{
		memcpy(&stTmpGrid, pstGrid+i, sizeof(stTmpGrid));
		pstRoleGrid = &stTmpGrid;
		pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstRoleGrid->GridData.RoleItem);
		if(pstItemDef ==NULL)
		{
			return -1;
		}

		/*
		if(iShoppingFlag == 1 || iShoppingFlag == 2)
		{
			if(iShoppingFlag == 2)
			{
				if( 0 > package_add_in(pstAppCtx, pstEnv, NULL, &stRolePack, pstRoleGrid, &stPackageChg) &&
					0 > package_add_in_shopping(pstAppCtx, pstEnv,  &stRolePack, pstRoleGrid, &stPackageChg))
				{
					// 商品背包加不了，并且身上的背包也加不了,就要失败了 
					return -1;
				}
			}
			else 
			{
				if( 0 > package_add_in_shopping(pstAppCtx, pstEnv,  &stRolePack, pstRoleGrid, &stPackageChg) &&
					0 > package_add_in(pstAppCtx, pstEnv, NULL, &stRolePack, pstRoleGrid, &stPackageChg) )
				{
					// 商品背包加不了，并且身上的背包也加不了,就要失败了 
					return -1;
				}
			}
		}
		else */if (0 > package_add_in(pstAppCtx, pstEnv, NULL, &stRolePack, pstRoleGrid, &stPackageChg))
		{
			return -1;
		}
	}
	
	return 0;
	UNUSED(iShoppingFlag);	//add by paraunused.pl
}

int package_dec_test_bull_in(ZONESVRENV* pstEnv, Player *pstPlayer, int iMoney)
{
	int iBull = 0;
	LEVELUP *pstLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);

	if (!pstLevelUp)
	{
		return 0;
	}

	if(pstLevelUp->DailyUseBull >0 && 
		pstPlayer->stRoleData.MiscInfo.DailyUseBull >= pstLevelUp->DailyUseBull)
	{
		goto _money;
	}

	iBull = iMoney;
	if(iBull > (int)pstPlayer->stRoleData.RoleDetail.Bull)
	{
		iBull = pstPlayer->stRoleData.RoleDetail.Bull;
	}

	if(pstLevelUp->DailyUseBull >0)
	{
		if(pstPlayer->stRoleData.MiscInfo.DailyUseBull+iBull > pstLevelUp->DailyUseBull)
		{
			iBull = pstLevelUp->DailyUseBull - pstPlayer->stRoleData.MiscInfo.DailyUseBull;
		}
	}

_money:
	if(iMoney - iBull > (int)pstPlayer->stRoleData.RoleDetail.Money)
	{
		return -1;
	}
	return 0;
}

int package_dec_money_test(ZONESVRENV* pstEnv, Player *pstPlayer, int iMoneyType, unsigned int uiMoney)
{
	UNUSED(pstEnv);
	
	switch(iMoneyType)
	{
		case ATTR_ID_MONEY:
			if (uiMoney > pstPlayer->stRoleData.RoleDetail.Money)
			{
				return -1;
			}
			break;
		case ATTR_ID_BULL:
			return package_dec_test_bull_in(pstEnv, pstPlayer, uiMoney);
			break;
		case ATTR_ID_GODCOIN:
			if( uiMoney > pstPlayer->stRoleData.RoleDetail.GodCoin )
			{
				return -1;
			}
			break;
		case ATTR_ID_BINDGODCOIN:
			if (uiMoney > (pstPlayer->stRoleData.RoleDetail.BindGodCoin))
			{
				return -1;
			}
			break;
		case ATTR_ID_VIPPOINT:
			if(uiMoney > pstPlayer->stRoleData.RoleDetail.VipPoint)
			{
				return -1;
			}
			break;
		case ATTR_ID_CONTRI:
			if(uiMoney > pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute)
			{
				return -1;
			}
			break;
		case ATTR_ID_BATTLE_GLORY:
			if(uiMoney > pstPlayer->stRoleData.RoleDetail.BlueJade)
			{
				return -1;
			}
			break;
		case ATTR_ID_LIFT_BRAND:
			if(uiMoney > pstPlayer->stRoleData.RoleDetail.GreenGem)
			{
				return -1;
			}
			break;
		case ATTR_ID_AOSHAN_BRAND:
			if(uiMoney > pstPlayer->stRoleData.RoleDetail.BlueGem)
			{
				return -1;
			}
			break;
		case ATTR_ID_SPARTA_GLORY:
			if(uiMoney > pstPlayer->stRoleData.RoleDetail.SpartaGlory)
			{
				return -1;
			}
			break;
		case ATTR_ID_SCHOOL_VAL:
			if(uiMoney > pstPlayer->stRoleData.RoleDetail.SchoolVal)
			{
				return -1;
			}
			break;
		case ATTR_ID_CLAN_WAR_BRAND:
			if(uiMoney > pstPlayer->stRoleData.RoleDetail.ClanWarBrand)
			{
				return -1;
			}
			break;
		case ATTR_ID_ZHENGYI:
			if(uiMoney > pstPlayer->stRoleData.RoleDetail.BlackJade)
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

//0 只消耗了绑定货币， >0 消耗的流通货币数量
int package_dec_bull_in(ZONESVRENV* pstEnv, Player *pstPlayer, int iMoney, PlayerChagAttrVal *pstChgVal)
{
	int iBull = 0;
	AttrChgVal *pstVal = NULL;
	LEVELUP *pstLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	
	if (!pstLevelUp)
	{
		return 0;
	}

	if(pstLevelUp->DailyUseBull >0 && 
		pstPlayer->stRoleData.MiscInfo.DailyUseBull >= pstLevelUp->DailyUseBull)
	{
		goto _money;
	}

	iBull = iMoney;
	if(iBull > (int)pstPlayer->stRoleData.RoleDetail.Bull)
	{
		//余下的扣银币
		iBull = pstPlayer->stRoleData.RoleDetail.Bull;
	}

	if(pstLevelUp->DailyUseBull >0)
	{
		if(pstPlayer->stRoleData.MiscInfo.DailyUseBull+iBull > pstLevelUp->DailyUseBull)
		{
			iBull = pstLevelUp->DailyUseBull - pstPlayer->stRoleData.MiscInfo.DailyUseBull;
		}
	}

	if(iBull > 0)
	{
		pstVal = &pstChgVal->AttrList[pstChgVal->AttrNum++];
		pstVal->AttrID = ATTR_ID_BULL;
		pstVal->nFlag = 1;
		pstVal->Value.Money = iBull;

		pstPlayer->stRoleData.MiscInfo.DailyUseBull += iBull;
		player_daily_bull_action(pstEnv, pstPlayer);
	}
	
_money:

	if(iMoney - iBull > 0)
	{
		pstVal = &pstChgVal->AttrList[pstChgVal->AttrNum++];
		pstVal->AttrID = ATTR_ID_MONEY;
		pstVal->nFlag = 1;
		pstVal->Value.Money = iMoney-iBull;
		return iMoney-iBull;
	}
	
	return  0;
}

//0 只消耗了指定货币， >0 额外消耗的流通货币数量
int package_dec_money(ZONESVRENV* pstEnv, Player *pstPlayer, int iMoneyType, unsigned int uiMoney)
{
	unsigned int uiOldMoney = pstPlayer->stRoleData.RoleDetail.Money;
	PlayerChagAttrVal stChgVal;
	AttrChgVal *pstVal = NULL;
	int iRet = 0;
	
	stChgVal.AttrNum = 0;

	pstVal = &stChgVal.AttrList[stChgVal.AttrNum++];
	pstVal->AttrID = iMoneyType;
	pstVal->nFlag = 1;
	switch(iMoneyType)
	{
		case ATTR_ID_MONEY:
			pstVal->Value.Money = uiMoney;
			break;
		case ATTR_ID_BULL:
			stChgVal.AttrNum = 0;
			iRet = package_dec_bull_in(pstEnv, pstPlayer, uiMoney, &stChgVal);
			break;
		case ATTR_ID_BINDGODCOIN:
			pstVal->Value.BindGodCoin = uiMoney;
			break;
		case ATTR_ID_VIPPOINT:
			pstVal->Value.VipPoint = uiMoney;
			break;
		case ATTR_ID_CONTRI:
			pstVal->Value.Contribute = uiMoney;
			break;
		case ATTR_ID_GODCOIN:
			pstVal->Value.GodCoin = uiMoney;
			break;
		case ATTR_ID_BATTLE_GLORY:
			pstVal->Value.BattleGlory = uiMoney;
			break;
		case ATTR_ID_LIFT_BRAND:
			pstVal->Value.LiftBrand = uiMoney;
			break;
		case ATTR_ID_AOSHAN_BRAND:
			pstVal->Value.AoShanBrand = uiMoney;
			break;
		case ATTR_ID_SPARTA_GLORY:
			pstVal->Value.SpartaGlory= uiMoney;
			break;
		case ATTR_ID_SCHOOL_VAL:
			pstVal->Value.SchoolVal = uiMoney;
			break;
		case ATTR_ID_CLAN_WAR_BRAND:
			pstVal->Value.ClanWarBrand = uiMoney;
			break;
		case ATTR_ID_ZHENGYI:
			pstVal->Value.Zhengyi = uiMoney;
			break;	
		default:
			return -1;
			break;
	}

	if (stChgVal.AttrNum > 0)
	{
		z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);
	}

	if (pstPlayer->stRoleData.RoleDetail.Money != uiOldMoney)
	{
		range_single_set(pstEnv, pstPlayer, RANGE_MAIN_SINGLE_SUB_MONEY, 
						pstPlayer->stRoleData.RoleDetail.Money+pstPlayer->stRoleData.Package.StashPackage.Money);
	}
	
	return iRet;
}

int package_add_money_test(ZONESVRENV* pstEnv, Player *pstPlayer, int iMoneyType, unsigned int uiMoney)
{
	LEVELUP *pstLevelUp = NULL;

	if (uiMoney == 0)
	{
		return 0;
	}

	/*if (iMoney < 0)
	{
		return -1;
	}*/

	pstLevelUp = (LEVELUP *)pstEnv->pstZoneObj->sLevelUpDef[pstPlayer->stRoleData.Level-1];
	if (!pstLevelUp)
	{
		return -1;
	}

	switch(iMoneyType)
	{
		case ATTR_ID_MONEY:
			if((uint64_t)pstPlayer->stRoleData.RoleDetail.Money +
				 (uint64_t)uiMoney >= MAX_MONEY)
			{
				return -1;
			}
			break;
		case ATTR_ID_BULL:
			if((uint64_t)pstPlayer->stRoleData.RoleDetail.Bull +
				 (uint64_t)uiMoney >= MAX_BULL)
			{
				return -1;
			}
			break;
		case ATTR_ID_BINDGODCOIN:
			if((uint64_t)pstPlayer->stRoleData.RoleDetail.BindGodCoin +
				 (uint64_t)uiMoney >= MAX_BINDGODCOIN)
			{
				return -1;
			}
			break;
		case ATTR_ID_GODCOIN:
			if((uint64_t)pstPlayer->stRoleData.RoleDetail.GodCoin +
				 (uint64_t)uiMoney >= MAX_GODCOIN)
			{
				return -1;
			}
			break;
		case ATTR_ID_VIPPOINT:
			
			break;
		case ATTR_ID_BATTLE_GLORY:
			if((uint64_t)pstPlayer->stRoleData.RoleDetail.BlueJade + 
				uiMoney > MAX_BATTLE_GLORY)
			{
				return -1;
			}
			break;
		case ATTR_ID_LIFT_BRAND:
			{
				unsigned short unTmp = uiMoney;
				player_add_lifebrand_test(pstEnv, pstPlayer, &unTmp);
				if (unTmp == 0)
				{
					return -1;
				}
			}
		
			if((uint64_t)pstPlayer->stRoleData.RoleDetail.GreenGem +
				uiMoney > MAX_LIFT_BRAND)
			{
				return -1;
			}
			break;
		case ATTR_ID_AOSHAN_BRAND:
			if((uint64_t)pstPlayer->stRoleData.RoleDetail.BlueGem +
				uiMoney > MAX_AOSHAN_BRAND)
			{
				return -1;
			}
			break;
		case ATTR_ID_SPARTA_GLORY:
			if((uint64_t)pstPlayer->stRoleData.RoleDetail.SpartaGlory+
				uiMoney > MAX_SPARTA_GLORY)
			{
				return -1;
			}
			break;
		case ATTR_ID_SCHOOL_VAL:
			if((uint64_t)pstPlayer->stRoleData.RoleDetail.SchoolVal +
				uiMoney > MAX_SPARTA_GLORY)
			{
				return -1;
			}
			break;
		case ATTR_ID_CLAN_WAR_BRAND:
			if((uint64_t)pstPlayer->stRoleData.RoleDetail.ClanWarBrand +
				uiMoney > MAX_CLAN_WAR_BRAND)
			{
				return -1;
			}
			else if ( 0 == MAX_WEEK_CLAN_WAR_BRAND)
			{
				//todo:限制每周获得上限
			}
			break;
		case ATTR_ID_ZHENGYI:
			
			break;
		default:
			return -1;
			break;
	}

	
	return 0;
}

int package_add_money(ZONESVRENV* pstEnv, Player *pstPlayer, int iMoneyType, unsigned int uiMoney)
{
	unsigned int uiOldMoney = pstPlayer->stRoleData.RoleDetail.Money;
	PlayerChagAttrVal stChgVal;
	AttrChgVal *pstVal = NULL;
	
	stChgVal.AttrNum = 0;

	pstVal = &stChgVal.AttrList[stChgVal.AttrNum++];
	pstVal->AttrID = iMoneyType;
	pstVal->nFlag = 0;
	switch(iMoneyType)
	{
		case ATTR_ID_MONEY:
			pstVal->Value.Money = uiMoney;
			break;
		case ATTR_ID_BULL:
			pstVal->Value.Bull = uiMoney;
			break;
		case ATTR_ID_BINDGODCOIN:
			pstVal->Value.BindGodCoin =uiMoney;
			break;
		case ATTR_ID_VIPPOINT:
			pstVal->Value.VipPoint = uiMoney;
			break;
		case ATTR_ID_GODCOIN:
			pstVal->Value.GodCoin = uiMoney;
			break;
		case ATTR_ID_BATTLE_GLORY:
			pstVal->Value.BattleGlory = uiMoney;
			break;
		case ATTR_ID_LIFT_BRAND:
			pstVal->Value.LiftBrand = uiMoney;
			break;
		case ATTR_ID_AOSHAN_BRAND:
			pstVal->Value.AoShanBrand = uiMoney;
			break;
		case ATTR_ID_SPARTA_GLORY:
			pstVal->Value.SpartaGlory = uiMoney;
			break;
		case ATTR_ID_SCHOOL_VAL:
			pstVal->Value.SchoolVal = uiMoney;
			break;
		case ATTR_ID_CLAN_WAR_BRAND:
			pstVal->Value.ClanWarBrand = uiMoney;
			break;
		case ATTR_ID_ZHENGYI:
			pstVal->Value.Zhengyi= uiMoney;
			break;	
		default:
			return -1;
			break;
	}

	if (stChgVal.AttrNum > 0)
	{
		z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);
	}

	if (pstPlayer->stRoleData.RoleDetail.Money != uiOldMoney)
	{
		range_single_set(pstEnv, pstPlayer, RANGE_MAIN_SINGLE_SUB_MONEY, 
						pstPlayer->stRoleData.RoleDetail.Money+pstPlayer->stRoleData.Package.StashPackage.Money);
	}
	
	return 0;
}

int package_get_free_itemgrid(ROLEITEM*pstPackageRoleItem, int iNum, int iMaxNum)
{
	int i = 0;
	int iGrid = 0;
	char sGrid[MAX_PACKAGE_GRID];

	assert(iMaxNum <= MAX_PACKAGE_GRID);
	
	memset(sGrid, 0, iMaxNum);
	for(i=0; i<iNum; i++)
	{
		iGrid = (pstPackageRoleItem+i)->GridIdx;
		if (iGrid >= 0 && iGrid < iMaxNum)
		{
			sGrid[iGrid] = 1;
		}
	}

	for (i=0; i<iMaxNum; i++)
	{
		if (!sGrid[i])
			return i;
	}

	return -1;
}

int package_itemgrid_pos(ROLEITEM*pstPackageRoleItem, int iNum, int iGridIdx)
{
	int i = 0;
	ROLEITEM *pstGridItem = NULL;

	for (i=0; i<iNum; i++)
	{
		pstGridItem = pstPackageRoleItem+i;
		if (pstGridItem->GridIdx == iGridIdx)
		{
			return i;
		}
	}

	return -1;
}
         
int wear_arm_pos(ROLEARM*pstRoleArms, int iNum, int iGridIdx)
{
	int i = 0;
	ROLEARM *pstArm = NULL;

	for (i=0; i<iNum; i++)
	{
		pstArm = pstRoleArms+i;
		if (pstArm->GridIdx == iGridIdx)
		{
			return i;
		}
	}

	return -1;
}

int package_get_free_grid(ROLEGRID*pstPackageRoleGrid, int iNum, int iMaxNum)
{
	int i = 0;
	ROLEGRID *pstGrid = NULL;
	int iGrid = 0;
	char sGrid[MAX_PACKAGE_GRID];

	assert(iMaxNum <= MAX_PACKAGE_GRID);

	memset(sGrid, 0, iMaxNum);
	for(i=0; i<iNum; i++)
	{
		pstGrid = pstPackageRoleGrid+i;
		switch (pstGrid->Type)
		{
			case GRID_ITEM_ITEM:
				iGrid = pstGrid->GridData.RoleItem.GridIdx;
				break;
			case GRID_ITEM_ARM:
				iGrid = pstGrid->GridData.RoleArm.GridIdx;
				break;
			default:
				return -1;
				break;
		}
		
		if (iGrid >= 0 && iGrid < iMaxNum)
		{
			sGrid[iGrid] = 1;
		}
	}

	for (i=0; i<iMaxNum; i++)
	{
		if (!sGrid[i])
			return i;
	}

	return -1;
}

int package_grid_pos(ROLEGRID*pstPackageRoleGrid, int iNum, int iGridIdx)
{
	int i = 0;
	ROLEGRID *pstGrid = NULL;
	
	for(i=0; i<iNum; i++)
	{
		pstGrid = pstPackageRoleGrid+i;
		switch (pstGrid->Type)
		{
			case GRID_ITEM_ITEM:
				if (iGridIdx == pstGrid->GridData.RoleItem.GridIdx)
				{
					return i;
				}
				break;
			case GRID_ITEM_ARM:
				if (iGridIdx == pstGrid->GridData.RoleArm.GridIdx)
				{
					return i;
				}
				break;
			default:
				return -1;
				break;
		}
	}

	return -1;
}

int pkg_get_pos(Player *pstPlayer, int iListType, int iGridIdx)
{
	ROLEPACKAGE *pstPkg = &pstPlayer->stRoleData.Package;
	if (iListType == LIST_TYPE_SUNDRIES)
	{
		return package_grid_pos(pstPkg->SundriesPackage.RoleGrids, pstPkg->SundriesPackage.Num, iGridIdx);
	}
	else
	{
		return -1;
	}
	return -1;
}

/*
 * 没有完全放下就返回-1, 已放下的item要改变输入参数中的数目
 * 杂物包 加物品到格子里
 */
int package_add_grid_in(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ROLEGRID *pstRoleGrids, unsigned short *punNum, 
							int iMaxNum, int iListType, ROLEGRID *pstGrid, 
							ITEMDEF *pstItemDef, 
							ROLEPACKAGECHG *pstPackageChg,
							ROLEPACKAGE *pstPkg)
{
	int i = 0;
	int iTotal = 0;
	ROLEITEM *pstGridItem = NULL;
	ROLEITEM *pstRoleItem = NULL;
	CSROLEARM *pstCSRoleArm = NULL;
	CSROLEITEM *pstCSRoleItem = NULL;
	ROLEGRID *pstRoleGrid = NULL;
	int iMaxPile = 0;
	int iLeftPile = 0;
	int count = 0;

	if (GRID_ITEM_ARM == pstGrid->Type)
	{
		if ((*punNum) >= iMaxNum)
		{
			return -1;
		}
		/* 装备加入 */
		pstGrid->GridData.RoleArm.Num = 1;
		/* 找到装备的空余索引 */
		pstGrid->GridData.RoleArm.GridIdx = package_get_free_grid(pstRoleGrids, *punNum, iMaxNum);

		if (gridIdx_is_unvaild(pstEnv, pstPkg, iListType, pstGrid->GridData.RoleArm.GridIdx) != 1)
		{
			return -1;
		}
		
		if (0 > pstGrid->GridData.RoleArm.GridIdx)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "grididx<0");
			return -1;
		}
		pstRoleGrid = pstRoleGrids+(*punNum);
		memcpy(pstRoleGrid, pstGrid, sizeof(ROLEGRID));
		(*punNum)++;

		/* 把这个数据清除下 */
		pstGrid->GridData.RoleArm.Num = 0;

		if (pstPackageChg->ArmChg.ChgNum >= MAX_CHG_NUM)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "pstPackageChg->ArmChg.ChgNum max");
			return 0;
		}
		/* 回包数据处理 */
		pstCSRoleArm = &pstPackageChg->ArmChg.Arms[pstPackageChg->ArmChg.ChgNum];
		pstPackageChg->ArmChg.ChgNum++;
		pstCSRoleArm->ListType = iListType;
		memcpy(&pstCSRoleArm->Arm, &pstRoleGrid->GridData.RoleArm, sizeof(pstCSRoleArm->Arm));
		
		return 0;
	}

	iMaxPile = pstItemDef->MaxPile;
	if (0 >= iMaxPile)
	{
		iMaxPile = 1;
	}

	if (gem_is_chongneng( pstItemDef, &pstGrid->GridData.RoleItem) == GEM_STAT_POWER)
	{
		iMaxPile = 1;
	}
	
	pstRoleItem = &pstGrid->GridData.RoleItem;
	iTotal = pstRoleItem->Num;

	if (iMaxPile > 1) //检查已有格子是否可以装下
	{
		qsort(pstRoleGrids, *punNum, sizeof(*pstRoleGrids), PakGridIdxCompare);
		
		for (i=(*punNum)-1; i>=0; i--) 
		{
			pstGridItem = &((pstRoleGrids+i)->GridData.RoleItem);
			if (is_same_roleitem(pstEnv,pstGridItem, pstRoleItem) && pstGridItem->Num < iMaxPile)
			{
				iLeftPile = iMaxPile - pstGridItem->Num;
				if (iTotal >= iLeftPile)
				{
					iTotal -= iLeftPile;
					pstGridItem->Num = iMaxPile;
				}
				else
				{
					pstGridItem->Num += iTotal;
					iTotal = 0;
				}

				if (pstPackageChg->ItemChg.ChgNum >= MAX_CHG_NUM)
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "pstPackageChg->ItemChg.ChgNum max");
					continue;
				}

				pstCSRoleItem = &pstPackageChg->ItemChg.Items[pstPackageChg->ItemChg.ChgNum];
				pstPackageChg->ItemChg.ChgNum++;
				pstCSRoleItem->ListType = iListType;
				memcpy(&pstCSRoleItem->Item, pstGridItem, sizeof(pstCSRoleItem->Item));

				if (iTotal <= 0)
				{
					break;
				}
			}
		}
	}

	while (iTotal > 0) //新增格子
	{
		if ((*punNum) < iMaxNum)
		{
			(pstRoleGrids+(*punNum))->Type = GRID_ITEM_ITEM;
			pstGridItem = &((pstRoleGrids+(*punNum))->GridData.RoleItem);
			memcpy(pstGridItem, pstRoleItem, sizeof(*pstGridItem));
			pstGridItem->GridIdx = package_get_free_grid(pstRoleGrids, *punNum, iMaxNum);
			if (gridIdx_is_unvaild(pstEnv, pstPkg, iListType, pstGridItem->GridIdx) != 1)
			{
				return -1;
			}
			
			if (gridIdx_is_unvaild(pstEnv, pstPkg, iListType, pstGridItem->GridIdx) != 1)
			{
				return -1;
			}
			if (0 > pstGridItem->GridIdx)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "grididx<0");
				break;
			}

			 if (count ==  0)
			{
				count = 1 ;
			}
			 else
			{
				/*取了个世界的唯一物品id*/
			 	pstGridItem->WID = z_get_gid(pstAppCtx, pstEnv);
			}
							
			if (iTotal >= iMaxPile)
			{
				pstGridItem->Num = iMaxPile;
				iTotal -= iMaxPile;
			}
			else
			{
				pstGridItem->Num = iTotal;
				iTotal = 0;
			}

			(*punNum)++;
			count++;

			if (pstPackageChg->ItemChg.ChgNum >= MAX_CHG_NUM)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "pstPackageChg->ItemChg.ChgNum max");
				continue;
			}

			pstCSRoleItem = &pstPackageChg->ItemChg.Items[pstPackageChg->ItemChg.ChgNum];
			pstPackageChg->ItemChg.ChgNum++;
			pstCSRoleItem->ListType = iListType;
			memcpy(&pstCSRoleItem->Item, pstGridItem, sizeof(pstCSRoleItem->Item));
		}
		else
		{
			break;
		}
	}
	
	pstRoleItem->Num = iTotal;
	if (pstRoleItem->Num > 0)
	{
		return -1;
	}
	
	return 0;
}

/*
 * 没有完全放下就返回-1, 已放下的item要改变输入参数中的数目
 * 材料包,任务包,武器背包加物品的函数处理
 */
int package_add_item_in(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, 
							DYNATTRLIST *pstDyn , ROLEITEM *pstPackageRoleItem,
							unsigned short *punNum, 
							int iMaxNum, int iListType, 
							ROLEITEM *pstRoleItem, 
							ITEMDEF *pstItemDef, 
							ROLEPACKAGECHG *pstPackageChg,
							ROLEPACKAGE *pstPkg)
{
	int i = 0;
	int iTotal = 0;
	ROLEITEM *pstGridItem = NULL;
	CSROLEITEM *pstCSRoleItem = NULL;
	int iMaxPile = 0;
	int iLeftPile = 0;
	//int iCheckAmm = 0;
	int count = 0 ;
	
	iMaxPile = pstItemDef->MaxPile;
	if (0 >= iMaxPile)
	{
		iMaxPile = 1;
	}

	if (gem_is_chongneng( pstItemDef, pstRoleItem) == GEM_STAT_POWER)
	{
		iMaxPile = 1;
	}
	
	iTotal = pstRoleItem->Num;
	
	if (iMaxPile > 1) //检查已有格子是否可以装下
	{
		qsort(pstPackageRoleItem, *punNum, sizeof(*pstPackageRoleItem), PakItemIdxCompare);
		
		for (i=(*punNum)-1; i>=0; i--) 
		{
			pstGridItem = pstPackageRoleItem+i;
			if (is_same_roleitem(pstEnv,pstGridItem, pstRoleItem) && pstGridItem->Num < iMaxPile)
			{
				iLeftPile = iMaxPile - pstGridItem->Num;
				if (iTotal >= iLeftPile)
				{
					iTotal -= iLeftPile;
					pstGridItem->Num = iMaxPile;
				}
				else
				{
					pstGridItem->Num += iTotal;
					iTotal = 0;
				}
	
				if (pstPackageChg->ItemChg.ChgNum >= MAX_CHG_NUM)
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "pstPackageChg->ItemChg.ChgNum max");
					continue;
				}
	
				pstCSRoleItem = &pstPackageChg->ItemChg.Items[pstPackageChg->ItemChg.ChgNum];
				pstPackageChg->ItemChg.ChgNum++;
				pstCSRoleItem->ListType = iListType;
				memcpy(&pstCSRoleItem->Item, pstGridItem, sizeof(pstCSRoleItem->Item));
	
				if (iTotal <= 0)
				{
					break;
				}
			}
		}
	}
	
	while (iTotal > 0) //新增格子
	{
		if ((*punNum) < iMaxNum)
		{
			pstGridItem = pstPackageRoleItem+(*punNum);
			memcpy(pstGridItem, pstRoleItem, sizeof(*pstGridItem));
			pstGridItem->GridIdx = package_get_free_itemgrid(pstPackageRoleItem, *punNum, iMaxNum);
			if (gridIdx_is_unvaild(pstEnv, pstPkg, iListType, pstGridItem->GridIdx) != 1)
			{
				return -1;
			}
			
			if (	0 > pstGridItem->GridIdx )
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "GridIdx < 0");
				break;
			}
			
			if (	count == 0)
			{
				count = 1;
			}
			else
			{
				pstGridItem->WID = z_get_gid(pstAppCtx, pstEnv);
			}
							
			if (iTotal >= iMaxPile)
			{
				pstGridItem->Num = iMaxPile;
				iTotal -= iMaxPile;
			}
			else
			{
				pstGridItem->Num = iTotal;
				iTotal = 0;
			}

			/*
			//检查武器弹药
			if (0 == iCheckAmm && pstDyn)
			{
				iCheckAmm = 1;
				
				for (i=0; i<MAX_ARM_ON; i++)
				{
					if (0 == pstDyn->astAtkChg[i].iArmID)
					{
						break;
					}

					if (pstDyn->astAtkChg[i].iAmmID == (int)pstItemDef->ItemID && 0 > pstDyn->astAtkChg[i].iAmmPos)
					{
						pstDyn->astAtkChg[i].iAmmPos = *punNum;
					}
				}
			}
			*/

			(*punNum)++;
		

			if (pstPackageChg->ItemChg.ChgNum >= MAX_CHG_NUM)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "pstPackageChg->ItemChg.ChgNum max");
				continue;
			}

			pstCSRoleItem = &pstPackageChg->ItemChg.Items[pstPackageChg->ItemChg.ChgNum];
			pstPackageChg->ItemChg.ChgNum++;
			pstCSRoleItem->ListType = iListType;
			memcpy(&pstCSRoleItem->Item, pstGridItem, sizeof(pstCSRoleItem->Item));
		}
		else
		{
			break;
		}
	}
	
	pstRoleItem->Num = iTotal;
	if (pstRoleItem->Num > 0)
	{
		return -1;
	}

	return 0;
	
	UNUSED(pstDyn);	//add by paraunused.pl
}

int package_add_by_id(ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID, int iNum, int iBind)
{
	ROLEGRID stGrid;

	if (item_create(pstEnv->pstAppCtx,pstEnv,&stGrid, iItemID) == 0)
	{
		stGrid.GridData.RoleItem.Num = iNum;
		if (iBind)
			stGrid.GridData.RoleItem.InstFlag|=INST_ITEM_BIND;
	}
	else
	{
		return -1;
	}

	if (package_add_test(pstEnv->pstAppCtx,  pstEnv, pstPlayer, &stGrid,1, 0) < 0)
	{	
		return -1;
	}	
	package_add(pstEnv->pstAppCtx,pstEnv, pstPlayer, &stGrid,1, 0, 0);
	return 0;
}

int package_add_in(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, DYNATTRLIST *pstDyn,  ROLEPACKAGE *pstPlayerPackage, ROLEGRID *pstGrid, ROLEPACKAGECHG *pstPackageChg)
{
	ITEMDEF *pstItemDef = NULL;
	ROLEITEM*pstPackageRoleItem = NULL;
	int iMaxItemNum = 0;
	int iListType = 0;
	unsigned short *punItemNum = NULL;
	SUNDRIESPACKAGE *pstSundriesPackage = &pstPlayerPackage->SundriesPackage;

	/* 判断下格子的类型 */
	switch(pstGrid->Type)
	{
		case GRID_ITEM_ITEM:
		case GRID_ITEM_ARM: //RoleArm与RoleItem前两个字段相同
			pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstGrid->GridData.RoleItem);
			if (NULL == pstItemDef)
			{
				return -1;
			}

			if (GRID_ITEM_ITEM == pstGrid->Type)
			{
				if (ITEM_TYPE_ARM == pstItemDef->ItemType)
				{
					return -1;
				}
			}
			else
			{
				if (ITEM_TYPE_ARM != pstItemDef->ItemType)
				{
					return -1;
				}
			}
			
			break;
		default:
			return -1;
			break;
	}
	if(!pstItemDef)
	{
		return -1;
	}
       /* 物品类型处理 */
	switch(pstItemDef->ItemType)
	{
		case ITEM_TYPE_ARM:
		case ITEM_TYPE_ITEM:	
		case ITEM_TYPE_STUFF:
			if (0 > package_add_grid_in(pstAppCtx, pstEnv, pstSundriesPackage->RoleGrids, &pstSundriesPackage->Num, pstSundriesPackage->MaxNum,
										LIST_TYPE_SUNDRIES, pstGrid, pstItemDef, pstPackageChg,pstPlayerPackage))
			{
				return -1;
			}

			break;
		case ITEM_TYPE_AMM:
		case ITEM_TYPE_TASK:
			if (ITEM_TYPE_AMM == pstItemDef->ItemType)
			{
				pstPackageRoleItem = pstPlayerPackage->AmmPackage.RoleItems;
				iMaxItemNum = pstPlayerPackage->AmmPackage.MaxNum;
				punItemNum = &pstPlayerPackage->AmmPackage.Num;
				iListType = LIST_TYPE_AMM;
			}
			else
			{
				pstPackageRoleItem = pstPlayerPackage->TaskPackage.RoleItems;
				iMaxItemNum = pstPlayerPackage->TaskPackage.MaxNum;
				punItemNum = &pstPlayerPackage->TaskPackage.Num;
				iListType =  LIST_TYPE_TASK;
			}

			if (0 > package_add_item_in(pstAppCtx, pstEnv, pstDyn, pstPackageRoleItem, 
										punItemNum, iMaxItemNum, iListType, 
										&pstGrid->GridData.RoleItem, 
										pstItemDef, pstPackageChg,
										pstPlayerPackage))
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

int item_get_msgtype(ITEMDEF *pstItemDef)
{
	int iSysType = -1;
	
	switch(pstItemDef->ItemType)
	{
		case ITEM_TYPE_ITEM:
			iSysType = SYS_MSG_ITEM;
			break;
		case ITEM_TYPE_ARM:
			iSysType = SYS_MSG_ARM;
			break;
		case ITEM_TYPE_STUFF:
			iSysType = SYS_MSG_STUFF;
			break;
		case ITEM_TYPE_TASK:
			iSysType = SYS_MSG_TASK;
			break;
		default:
			break;
	}

	return iSysType;
}


int package_add_msg_show(ZONESVRENV* pstEnv, char *pszName, ITEMDEF *pstItemDef,ROLEGRID *pstRoleGrid)
{
	int iID = pstItemDef->ItemID;
	if (	iID == 5610201 ||
		iID == 5610300 ||
		iID == 5610301 ||
		iID == 5610142 ||
		iID == 5610303 ||
		iID == 5600213 ||
		iID == 5610112 ||
		iID == 5610151)
	{
		CSPKG stPkgLink;
		z_fill_link_msg(&stPkgLink, SYS_SHOW, CHAT_LINK_ITEM, 
						(void *)&pstRoleGrid->GridData.RoleItem, 
						ZONEERR_ADD_MSG_SHOW,
						pszName, pstItemDef->Name);
		z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkgLink);
	}
	
	return 0;
}

int package_add_msg( ZONESVRENV* pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef, 
	ROLEGRID *pstRoleGrid, int iNum)
{
	int iSysType;
	CSPKG stPkg;
	
	iSysType = item_get_msgtype(pstItemDef);
	
	//消息通知
	switch(pstRoleGrid->Type)
	{
		case GRID_ITEM_ITEM:
		case GRID_ITEM_ARM:
			if (GRID_ITEM_ITEM == pstRoleGrid->Type)
			{
				iNum = iNum - pstRoleGrid->GridData.RoleItem.Num;
				if (iNum <= 0)
				{
					break;
				}
			}
			else
			{
				if (0 != pstRoleGrid->GridData.RoleArm.Num)
				{
					break;
				}
				
				iNum = 1;
			}

			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, iSysType, ZONEERR_PAK1, iNum, pstItemDef->Name);
			
			break;
		default:
			break;
		
	}

	if (iNum > 0)
	{
		if(pstRoleGrid->Type == GRID_ITEM_ITEM)
		{
			z_fill_link_msg(&stPkg, SYS_COMBAT, CHAT_LINK_ITEM, (void *)&pstRoleGrid->GridData.RoleItem, ZONEERR_LINK305, iNum);
			
		}
		else
		{
			z_fill_link_msg(&stPkg, SYS_COMBAT, CHAT_LINK_ARM, (void *)&pstRoleGrid->GridData.RoleArm, ZONEERR_LINK305, iNum);
		}
	
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	return iNum;
}

// iShoppingFlag 1表示先加商品背包 2表示先加杂物背包
int package_add(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,  ROLEGRID *pstGrid, int iGridNum,int iShoppingFlag, int iWhatWay)
{
	int i = 0;
	int iNum = 0;
	ROLEGRID *pstRoleGrid = NULL;
	ITEMDEF *pstItemDef = NULL;
	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstPackageChg = &pstPackageInfo->PackageInfoData.PackageChg;
	int iRet = 0;
	int iAdd;

	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstPackageChg->ItemChg.ChgNum = 0;
	pstPackageChg->ArmChg.ChgNum = 0;
		
	//todo:处理BOOTY_CLAN_WAR_BRAND_ITEM_ID
	for (i=0; i<iGridNum; i++)
	{
		pstRoleGrid = pstGrid + i;

		switch(pstRoleGrid->Type)
		{
			case GRID_ITEM_ITEM:
			case GRID_ITEM_ARM:
				pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstRoleGrid->GridData.RoleItem);
				if (NULL == pstItemDef)
				{
					continue;
				}
				
				if (GRID_ITEM_ITEM == pstRoleGrid->Type)
				{
					iNum = pstRoleGrid->GridData.RoleItem.Num;
				}
				break;

			default:
				continue;
				break;
			
		}

		/*
		if(iShoppingFlag == 1 || iShoppingFlag == 2)
		{
			if( iShoppingFlag == 2)
			{
				if( 0 > package_add_in(pstAppCtx, pstEnv,NULL, &pstPlayer->stRoleData.Package, pstRoleGrid, pstPackageChg) &&
					0 > package_add_in_shopping(pstAppCtx, pstEnv,  &pstPlayer->stRoleData.Package, pstRoleGrid,pstPackageChg))
				{
					tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player(%d) package_add_in_shopping fail", pstPlayer->stRoleData.Uin);
					iRet = -1;
				}
			}
			else
			{
				if( 0 > package_add_in_shopping(pstAppCtx, pstEnv,  &pstPlayer->stRoleData.Package, pstRoleGrid,pstPackageChg) &&
					0 > package_add_in(pstAppCtx, pstEnv,NULL, &pstPlayer->stRoleData.Package, pstRoleGrid, pstPackageChg))
				{
					tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player(%d) package_add_in_shopping fail", pstPlayer->stRoleData.Uin);
					iRet = -1;
				}
			}
		}
		else */if (0 > package_add_in(pstAppCtx, pstEnv, &pstPlayer->stOnline.stDyn, &pstPlayer->stRoleData.Package, pstRoleGrid, pstPackageChg))
		{
			tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player(%d) package_add_in fail", pstPlayer->stRoleData.Uin);
			iRet = -1;
		}
		
		if(pstItemDef)
		{
			iAdd = package_add_msg(pstEnv, pstPlayer, pstItemDef, pstRoleGrid, iNum);

			if ((ITEM_IMP_LOGTRACE & pstItemDef->CtrlFlag) && iAdd > 0 && iWhatWay > 0)
			{
				int iBindNum;

				if (INST_ITEM_BIND & pstRoleGrid->GridData.RoleItem.InstFlag)
				{
					iBindNum = iAdd;
				}
				else
				{
					iBindNum = 0;
				}
				
				z_impitem_create_oplog(pstEnv, pstPlayer,pstItemDef->ItemID, iWhatWay, iAdd, iBindNum);
			}
		}
	}

	if (pstPackageChg->ItemChg.ChgNum > 0 ||pstPackageChg->ArmChg.ChgNum > 0 )
	{
		Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	
	return iRet;
	UNUSED(iShoppingFlag);	//add by paraunused.pl
}

// fBind 1绑定 0 非绑定 iGnore 为1表示忽略失效的格子 
int z_get_pak_item_num_by_bindtype(ZONESVRENV* pstEnv, ROLEPACKAGE *pstPak, 
		int iItemID, int iListType, int iGnore, int fBind)
{
	int i = 0;
	ITEMDEF *pstItemDef = NULL;
	int iItemNum = 0;
	int iType = iListType;
		
	//if (-1 == iListType)
	// -1表示背包查找    -2表示背包和仓库查找

	pstItemDef = z_find_itemdef(pstEnv, iItemID);
	if (NULL == pstItemDef)
	{
		return 0;
	}
	if (iListType < 0 )
	{
		switch(pstItemDef->ItemType)
		{
			case ITEM_TYPE_AMM:
				return -1;
				break;
			case ITEM_TYPE_STUFF:
				for (i=0; i<pstPak->StuffPackage.Num; i++)
				{
					if (pstPak->StuffPackage.RoleItems[i].DefID == iItemID &&
						fBind == is_bind_roleitem(&pstPak->StuffPackage.RoleItems[i]))
					{
						if(	iGnore > 0 && 
							gridIdx_is_unvaild(pstEnv,pstPak,ITEM_TYPE_STUFF,pstPak->StuffPackage.RoleItems[i].GridIdx) <= 0)
						{
							continue;
						}
						iItemNum += pstPak->StuffPackage.RoleItems[i].Num;
					}
				}
				iListType = LIST_TYPE_SUNDRIES; // 杂物包里继续找
				break;
			case ITEM_TYPE_TASK:
				for (i=0; i<pstPak->TaskPackage.Num; i++)
				{
					if (pstPak->TaskPackage.RoleItems[i].DefID == iItemID && 
						fBind == is_bind_roleitem(&pstPak->TaskPackage.RoleItems[i]))
					{
						if(	iGnore > 0 && 
							gridIdx_is_unvaild(pstEnv,pstPak,ITEM_TYPE_TASK,pstPak->TaskPackage.RoleItems[i].GridIdx) <= 0)
						{
							continue;
						}
						iItemNum += pstPak->TaskPackage.RoleItems[i].Num;
					}
				}
				iListType = LIST_TYPE_SUNDRIES; // 杂物包里继续找
				break;
			default:
				for (i=0; i<pstPak->SundriesPackage.Num; i++)
				{
					if (pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem.DefID == iItemID &&
						fBind == is_bind_roleitem(&pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem))
					{
						if(	iGnore > 0 && 
							gridIdx_is_unvaild(	pstEnv,pstPak,LIST_TYPE_SUNDRIES,
												pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem.GridIdx) <= 0)
						{
							continue;
						}

						if (gem_is_chongneng( pstItemDef, &pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem)== GEM_STAT_POWER)
						{
							continue;
						}
						
						iItemNum += pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem.Num;
					}
				}

				//  仓库
				if (iType == -2)
				{
					for (i=0; i<pstPak->StashPackage.Num; i++)
					{
						if (pstPak->StashPackage.RoleGrids[i].GridData.RoleItem.DefID == iItemID &&
							fBind == is_bind_roleitem(&pstPak->StashPackage.RoleGrids[i].GridData.RoleItem))
						{
							if(	iGnore > 0 && 
								gridIdx_is_unvaild(	pstEnv,pstPak,LIST_TYPE_STASH,
												pstPak->StashPackage.RoleGrids[i].GridData.RoleItem.GridIdx) <= 0)
							{
								continue;
							}

							if (gem_is_chongneng( pstItemDef, &pstPak->StashPackage.RoleGrids[i].GridData.RoleItem)== GEM_STAT_POWER)
							{
								continue;
							}
							iItemNum += pstPak->StashPackage.RoleGrids[i].GridData.RoleItem.Num;
						}
					}
				}
				
				return iItemNum;
				break;
		}
	}

	switch(iListType)
	{
		case LIST_TYPE_AMM:
			for (i=0; i<pstPak->AmmPackage.Num; i++)
			{
				if (pstPak->AmmPackage.RoleItems[i].DefID == iItemID &&
					fBind == is_bind_roleitem(&pstPak->AmmPackage.RoleItems[i]))
				{
					if(	iGnore > 0 && 
						gridIdx_is_unvaild(pstEnv,pstPak,ITEM_TYPE_AMM,pstPak->AmmPackage.RoleItems[i].GridIdx) <= 0)
					{
						continue;
					}
					iItemNum += pstPak->AmmPackage.RoleItems[i].Num;
				}
			}

			break;
		case LIST_TYPE_STUFF:
			for (i=0; i<pstPak->StuffPackage.Num; i++)
			{
				if (pstPak->StuffPackage.RoleItems[i].DefID == iItemID &&
					fBind == is_bind_roleitem(&pstPak->StuffPackage.RoleItems[i]))
				{
					if(	iGnore > 0 && 
						gridIdx_is_unvaild(pstEnv,pstPak,LIST_TYPE_STUFF,pstPak->StuffPackage.RoleItems[i].GridIdx) <= 0)
					{
						continue;
					}
					iItemNum += pstPak->StuffPackage.RoleItems[i].Num;
				}
			}
			break;
		case LIST_TYPE_TASK:
			for (i=0; i<pstPak->TaskPackage.Num; i++)
			{
				if (pstPak->TaskPackage.RoleItems[i].DefID == iItemID &&
					fBind == is_bind_roleitem(&pstPak->TaskPackage.RoleItems[i]))
				{
					if(	iGnore > 0 && 
						gridIdx_is_unvaild(pstEnv,pstPak,LIST_TYPE_TASK,pstPak->TaskPackage.RoleItems[i].GridIdx) <= 0)
					{
						continue;
					}
					iItemNum  += pstPak->TaskPackage.RoleItems[i].Num;
				}
			}
			return iItemNum;
			break;
		case LIST_TYPE_SUNDRIES:
			for (i=0; i<pstPak->SundriesPackage.Num; i++)
			{
				if (pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem.DefID == iItemID &&
					fBind == is_bind_roleitem(&pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem))
				{
					if(	iGnore > 0 && 
						gridIdx_is_unvaild(	pstEnv,pstPak,LIST_TYPE_SUNDRIES,
											pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem.GridIdx) <= 0)
					{
						continue;
					}

					if (gem_is_chongneng( pstItemDef, &pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem)== GEM_STAT_POWER)
					{
						continue;
					}
					
					iItemNum += pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem.Num;
				}
			}
			break;
		case LIST_TYPE_STASH:
			for (i=0; i<pstPak->StashPackage.Num; i++)
			{
				if (pstPak->StashPackage.RoleGrids[i].GridData.RoleItem.DefID == iItemID &&
					fBind == is_bind_roleitem(&pstPak->StashPackage.RoleGrids[i].GridData.RoleItem))
				{
					if(	iGnore > 0 && 
						gridIdx_is_unvaild(	pstEnv,pstPak,LIST_TYPE_STASH,
											pstPak->StashPackage.RoleGrids[i].GridData.RoleItem.GridIdx) <= 0)
					{
						continue;
					}

					if (gem_is_chongneng( pstItemDef, &pstPak->StashPackage.RoleGrids[i].GridData.RoleItem)== GEM_STAT_POWER)
					{
						continue;
					}
					iItemNum += pstPak->StashPackage.RoleGrids[i].GridData.RoleItem.Num;
				}
			}
			break;
		case LIST_TYPE_SHOPPING:
			for (i=0; i<pstPak->ShoppingPackage.Num; i++)
			{
				if (pstPak->ShoppingPackage.RoleGrids[i].GridData.RoleItem.DefID == iItemID &&
					fBind == is_bind_roleitem(&pstPak->ShoppingPackage.RoleGrids[i].GridData.RoleItem))
				{
					iItemNum += pstPak->ShoppingPackage.RoleGrids[i].GridData.RoleItem.Num;
				}
			}
			break;
		default:
			return 0;
			break;
	}

	//  仓库
	if (iType == -2)
	{
		for (i=0; i<pstPak->StashPackage.Num; i++)
		{
			if (pstPak->StashPackage.RoleGrids[i].GridData.RoleItem.DefID == iItemID &&
				fBind == is_bind_roleitem(&pstPak->StashPackage.RoleGrids[i].GridData.RoleItem))
			{
				if(	iGnore > 0 && 
					gridIdx_is_unvaild(	pstEnv,pstPak,LIST_TYPE_STASH,
										pstPak->StashPackage.RoleGrids[i].GridData.RoleItem.GridIdx) <= 0)
				{
					continue;
				}

				if (gem_is_chongneng( pstItemDef, &pstPak->StashPackage.RoleGrids[i].GridData.RoleItem)== GEM_STAT_POWER)
				{
					continue;
				}
				
				iItemNum += pstPak->StashPackage.RoleGrids[i].GridData.RoleItem.Num;
			}
		}
	}

	return iItemNum;
}

int z_item_vaild_check(ZONESVRENV* pstEnv, ROLEITEM *pstRoleItem)
{
	if(g_invaild_dec && pstRoleItem->ValidEnd > 0 &&
		pstRoleItem->ValidEnd < pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return -1;
	}

	return 0;
}

// iGnore 为1表示忽略失效的格子
//iDecType   0 绑定和非绑定的一起扣， 1  只扣绑定的物品   2  只扣非绑定的物品	    
int z_get_pak_item_num(ZONESVRENV* pstEnv, ROLEPACKAGE *pstPak, int iItemID, int iListType, int iGnore,int iDecType)
{
	int i = 0;
	ITEMDEF *pstItemDef = NULL;
	int iItemNum = 0;
	int iType = iListType;
		
	//if (-1 == iListType)
	// -1表示背包查找    -2表示背包和仓库查找
	if (iListType < 0 )
	{
		pstItemDef = z_find_itemdef(pstEnv, iItemID);
		if (NULL == pstItemDef)
		{
			return 0;
		}

		switch(pstItemDef->ItemType)
		{
			case ITEM_TYPE_AMM:
				return -1;
				break;
			case ITEM_TYPE_STUFF:
				for (i=0; i<pstPak->StuffPackage.Num; i++)
				{
					if (pstPak->StuffPackage.RoleItems[i].DefID == iItemID)
					{
						if(	iGnore > 0 && 
							gridIdx_is_unvaild(pstEnv,pstPak,ITEM_TYPE_STUFF,pstPak->StuffPackage.RoleItems[i].GridIdx) <= 0)
						{
							continue;
						}

						if(0 > z_item_vaild_check(pstEnv, &pstPak->StuffPackage.RoleItems[i]))
						{
							continue;
						}

						if( (PACKAGE_DEC_BIND == iDecType && !is_bind_roleitem(&pstPak->StuffPackage.RoleItems[i])) ||
							(PACKAGE_DEC_UNBIND == iDecType && is_bind_roleitem(&pstPak->StuffPackage.RoleItems[i])) )
						{
							continue;
						}
						
						iItemNum += pstPak->StuffPackage.RoleItems[i].Num;
					}
				}
				iListType = LIST_TYPE_SUNDRIES; // 杂物包里继续找
				break;
			case ITEM_TYPE_TASK:
				for (i=0; i<pstPak->TaskPackage.Num; i++)
				{
					if (pstPak->TaskPackage.RoleItems[i].DefID == iItemID)
					{
						if(	iGnore > 0 && 
							gridIdx_is_unvaild(pstEnv,pstPak,ITEM_TYPE_TASK,pstPak->TaskPackage.RoleItems[i].GridIdx) <= 0)
						{
							continue;
						}

						if(0 > z_item_vaild_check(pstEnv, &pstPak->TaskPackage.RoleItems[i]))
						{
							continue;
						}
						
						if( (PACKAGE_DEC_BIND == iDecType && !is_bind_roleitem(&pstPak->TaskPackage.RoleItems[i])) ||
							(PACKAGE_DEC_UNBIND == iDecType && is_bind_roleitem(&pstPak->TaskPackage.RoleItems[i])) )
						{
							continue;
						}
						
						iItemNum += pstPak->TaskPackage.RoleItems[i].Num;
					}
				}
				iListType = LIST_TYPE_SUNDRIES; // 杂物包里继续找
				break;
			default:
				for (i=0; i<pstPak->SundriesPackage.Num; i++)
				{
					if (pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem.DefID == iItemID)
					{
						if(	iGnore > 0 && 
							gridIdx_is_unvaild(	pstEnv,pstPak,LIST_TYPE_SUNDRIES,
												pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem.GridIdx) <= 0)
						{
							continue;
						}

						if(0 > z_item_vaild_check(pstEnv, &pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem))
						{
							continue;
						}
						
						if( (PACKAGE_DEC_BIND == iDecType && !is_bind_roleitem(&pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem)) ||
							(PACKAGE_DEC_UNBIND == iDecType && is_bind_roleitem(&pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem)) )
						{
							continue;
						}
						iItemNum += pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem.Num;
					}
				}

				//  仓库
				if (iType == -2)
				{
					for (i=0; i<pstPak->StashPackage.Num; i++)
					{
						if (pstPak->StashPackage.RoleGrids[i].GridData.RoleItem.DefID == iItemID)
						{
							if(	iGnore > 0 && 
								gridIdx_is_unvaild(	pstEnv,pstPak,LIST_TYPE_STASH,
												pstPak->StashPackage.RoleGrids[i].GridData.RoleItem.GridIdx) <= 0)
							{
								continue;
							}

							if(0 > z_item_vaild_check(pstEnv, &pstPak->StashPackage.RoleGrids[i].GridData.RoleItem))
							{
								continue;
							}
							
							if( (PACKAGE_DEC_BIND == iDecType && !is_bind_roleitem(&pstPak->StashPackage.RoleGrids[i].GridData.RoleItem)) ||
								(PACKAGE_DEC_UNBIND == iDecType && is_bind_roleitem(&pstPak->StashPackage.RoleGrids[i].GridData.RoleItem)) )
							{
								continue;
							}
							
							iItemNum += pstPak->StashPackage.RoleGrids[i].GridData.RoleItem.Num;
						}
					}
				}
				
				return iItemNum;
				break;
		}
	}

	switch(iListType)
	{
		case LIST_TYPE_AMM:
			for (i=0; i<pstPak->AmmPackage.Num; i++)
			{
				if (pstPak->AmmPackage.RoleItems[i].DefID == iItemID)
				{
					if(	iGnore > 0 && 
						gridIdx_is_unvaild(pstEnv,pstPak,ITEM_TYPE_AMM,pstPak->AmmPackage.RoleItems[i].GridIdx) <= 0)
					{
						continue;
					}

					if(0 > z_item_vaild_check(pstEnv, &pstPak->AmmPackage.RoleItems[i]))
					{
						continue;
					}
					
					if( (PACKAGE_DEC_BIND == iDecType && !is_bind_roleitem(&pstPak->AmmPackage.RoleItems[i])) ||
						(PACKAGE_DEC_UNBIND == iDecType && is_bind_roleitem(&pstPak->AmmPackage.RoleItems[i])) )
					{
						continue;
					}
					iItemNum += pstPak->AmmPackage.RoleItems[i].Num;
				}
			}

			break;
		case LIST_TYPE_STUFF:
			for (i=0; i<pstPak->StuffPackage.Num; i++)
			{
				if (pstPak->StuffPackage.RoleItems[i].DefID == iItemID)
				{
					if(	iGnore > 0 && 
						gridIdx_is_unvaild(pstEnv,pstPak,LIST_TYPE_STUFF,pstPak->StuffPackage.RoleItems[i].GridIdx) <= 0)
					{
						continue;
					}

					if(0 > z_item_vaild_check(pstEnv, &pstPak->StuffPackage.RoleItems[i]))
					{
						continue;
					}

					if( (PACKAGE_DEC_BIND == iDecType && !is_bind_roleitem(&pstPak->StuffPackage.RoleItems[i])) ||
						(PACKAGE_DEC_UNBIND == iDecType && is_bind_roleitem(&pstPak->StuffPackage.RoleItems[i])) )
					{
						continue;
					}
					
					iItemNum += pstPak->StuffPackage.RoleItems[i].Num;
				}
			}
			break;
		case LIST_TYPE_TASK:
			for (i=0; i<pstPak->TaskPackage.Num; i++)
			{
				if (pstPak->TaskPackage.RoleItems[i].DefID == iItemID)
				{
					if(	iGnore > 0 && 
						gridIdx_is_unvaild(pstEnv,pstPak,LIST_TYPE_TASK,pstPak->TaskPackage.RoleItems[i].GridIdx) <= 0)
					{
						continue;
					}

					if(0 > z_item_vaild_check(pstEnv, &pstPak->TaskPackage.RoleItems[i]))
					{
						continue;
					}

					if( (PACKAGE_DEC_BIND == iDecType && !is_bind_roleitem(&pstPak->TaskPackage.RoleItems[i])) ||
						(PACKAGE_DEC_UNBIND == iDecType && is_bind_roleitem(&pstPak->TaskPackage.RoleItems[i])) )
					{
						continue;
					}
					
					iItemNum  += pstPak->TaskPackage.RoleItems[i].Num;
				}
			}
			return iItemNum;
			break;
		case LIST_TYPE_SUNDRIES:
			for (i=0; i<pstPak->SundriesPackage.Num; i++)
			{
				if (pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem.DefID == iItemID)
				{
					if(	iGnore > 0 && 
						gridIdx_is_unvaild(	pstEnv,pstPak,LIST_TYPE_SUNDRIES,
											pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem.GridIdx) <= 0)
					{
						continue;
					}

					if(0 > z_item_vaild_check(pstEnv, &pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem))
					{
						continue;
					}

					if( (PACKAGE_DEC_BIND == iDecType && !is_bind_roleitem(&pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem)) ||
						(PACKAGE_DEC_UNBIND == iDecType && is_bind_roleitem(&pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem)) )
					{
						continue;
					}
					
					iItemNum += pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem.Num;
				}
			}
			break;
		case LIST_TYPE_STASH:
			for (i=0; i<pstPak->StashPackage.Num; i++)
			{
				if (pstPak->StashPackage.RoleGrids[i].GridData.RoleItem.DefID == iItemID)
				{
					if(	iGnore > 0 && 
						gridIdx_is_unvaild(	pstEnv,pstPak,LIST_TYPE_STASH,
											pstPak->StashPackage.RoleGrids[i].GridData.RoleItem.GridIdx) <= 0)
					{
						continue;
					}

					if(0 > z_item_vaild_check(pstEnv, &pstPak->StashPackage.RoleGrids[i].GridData.RoleItem))
					{
						continue;
					}

					if( (PACKAGE_DEC_BIND == iDecType && !is_bind_roleitem(&pstPak->StashPackage.RoleGrids[i].GridData.RoleItem)) ||
						(PACKAGE_DEC_UNBIND == iDecType && is_bind_roleitem(&pstPak->StashPackage.RoleGrids[i].GridData.RoleItem)) )
					{
						continue;
					}
					
					iItemNum += pstPak->StashPackage.RoleGrids[i].GridData.RoleItem.Num;
				}
			}
			break;
		case LIST_TYPE_SHOPPING:
			for (i=0; i<pstPak->ShoppingPackage.Num; i++)
			{
				if (pstPak->ShoppingPackage.RoleGrids[i].GridData.RoleItem.DefID == iItemID)
				{
					if( (PACKAGE_DEC_BIND == iDecType && !is_bind_roleitem(&pstPak->ShoppingPackage.RoleGrids[i].GridData.RoleItem)) ||
						(PACKAGE_DEC_UNBIND == iDecType && is_bind_roleitem(&pstPak->ShoppingPackage.RoleGrids[i].GridData.RoleItem)) )
					{
						continue;
					}
					iItemNum += pstPak->ShoppingPackage.RoleGrids[i].GridData.RoleItem.Num;
				}
			}
			break;
		default:
			return 0;
			break;
	}

	//  仓库
	if (iType == -2)
	{
		for (i=0; i<pstPak->StashPackage.Num; i++)
		{
			if (pstPak->StashPackage.RoleGrids[i].GridData.RoleItem.DefID == iItemID)
			{
				if(	iGnore > 0 && 
					gridIdx_is_unvaild(	pstEnv,pstPak,LIST_TYPE_STASH,
										pstPak->StashPackage.RoleGrids[i].GridData.RoleItem.GridIdx) <= 0)
				{
					continue;
				}

				if(0 > z_item_vaild_check(pstEnv, &pstPak->StashPackage.RoleGrids[i].GridData.RoleItem))
				{
					continue;
				}
				
				if( (PACKAGE_DEC_BIND == iDecType && !is_bind_roleitem(&pstPak->StashPackage.RoleGrids[i].GridData.RoleItem)) ||
					(PACKAGE_DEC_UNBIND == iDecType && is_bind_roleitem(&pstPak->StashPackage.RoleGrids[i].GridData.RoleItem)) )
				{
					continue;
				}
				iItemNum += pstPak->StashPackage.RoleGrids[i].GridData.RoleItem.Num;
			}
		}
	}

	return iItemNum;
}

/*
	输入:  iListType  -1 不指定栏位，其他指定栏位
	              piPos NULL 没有建议的位置, 否则为建议的位置

	返回: 0 有足够的指定物品
	             -1  没有足够的指定物品

	iDecType   0 绑定和非绑定的一起扣， 1  只扣绑定的物品   2  只扣非绑定的物品	                            
*/
int package_dec_test(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID, 
				int iNum, int iListType, int *piPos, int iDecType)
{	
	int iItemNum = 0;
	ROLEITEM *pstRoleItem = NULL;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;

	UNUSED(pstAppCtx);

	if (-1 == iListType)
	{
		if (z_get_pak_item_num(pstEnv, pstPak, iItemID, iListType,0,iDecType) >= iNum)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}

	switch(iListType)
	{
		case LIST_TYPE_AMM:
			if (piPos && *piPos >=0 && *piPos < pstPak->AmmPackage.Num)
			{
				pstRoleItem = pstPak->AmmPackage.RoleItems + *piPos;
				if( (PACKAGE_DEC_BIND == iDecType && !is_bind_roleitem(pstRoleItem)) ||
					(PACKAGE_DEC_UNBIND == iDecType && is_bind_roleitem(pstRoleItem)) )
				{
					break;
				}
					
				if (pstRoleItem->DefID == iItemID && pstRoleItem->Num >= iNum)
				{
					return 0;
				}
			}

			iItemNum = z_get_pak_item_num(pstEnv, pstPak, iItemID, iListType,0,iDecType);
			break;
		case LIST_TYPE_STUFF:
			if (piPos && *piPos >=0 && *piPos < pstPak->StuffPackage.Num)
			{
				pstRoleItem = pstPak->StuffPackage.RoleItems + *piPos;
				if( (PACKAGE_DEC_BIND == iDecType && !is_bind_roleitem(pstRoleItem)) ||
					(PACKAGE_DEC_UNBIND == iDecType && is_bind_roleitem(pstRoleItem)) )
				{
					break;
				}
				if (pstRoleItem->DefID == iItemID && pstRoleItem->Num >= iNum)
				{
					return 0;
				}
			}

			iItemNum = z_get_pak_item_num(pstEnv, pstPak, iItemID, iListType,0,iDecType);
			break;
		case LIST_TYPE_TASK:
			if (piPos && *piPos >=0 && *piPos < pstPak->TaskPackage.Num)
			{
				pstRoleItem = pstPak->TaskPackage.RoleItems + *piPos;
				if( (PACKAGE_DEC_BIND == iDecType && !is_bind_roleitem(pstRoleItem)) ||
					(PACKAGE_DEC_UNBIND == iDecType && is_bind_roleitem(pstRoleItem)) )
				{
					break;
				}
				if (pstRoleItem->DefID == iItemID && pstRoleItem->Num >= iNum)
				{
					return 0;
				}
			}

			iItemNum = z_get_pak_item_num(pstEnv, pstPak, iItemID, iListType,0,iDecType);
			break;
		case LIST_TYPE_SUNDRIES:
			if (piPos && *piPos >=0 && *piPos < pstPak->SundriesPackage.Num)
			{
				//前提约束RoleArm/RoleItem前几个字段完全相同
				pstRoleItem = &pstPak->SundriesPackage.RoleGrids[*piPos].GridData.RoleItem;
				if( (PACKAGE_DEC_BIND == iDecType && !is_bind_roleitem(pstRoleItem)) ||
					(PACKAGE_DEC_UNBIND == iDecType && is_bind_roleitem(pstRoleItem)) )
				{
					break;
				}
				if (pstRoleItem->DefID == iItemID && pstRoleItem->Num >= iNum)
				{
					return 0;
				}
			}

			iItemNum = z_get_pak_item_num(pstEnv, pstPak, iItemID, iListType,0,iDecType);
			break;
		case LIST_TYPE_SHOPPING:
			if (piPos && *piPos >=0 && *piPos < pstPak->ShoppingPackage.Num)
			{
				pstRoleItem = &pstPak->ShoppingPackage.RoleGrids[*piPos].GridData.RoleItem;
				if( (PACKAGE_DEC_BIND == iDecType && !is_bind_roleitem(pstRoleItem)) ||
					(PACKAGE_DEC_UNBIND == iDecType && is_bind_roleitem(pstRoleItem)) )
				{
					break;
				}
				if (pstRoleItem->DefID == iItemID && pstRoleItem->Num >= iNum)
				{
					return 0;
				}
			}

			iItemNum = z_get_pak_item_num(pstEnv, pstPak, iItemID, iListType,0,iDecType);
			break;			
		default:
			return -1;
			break;
	}

	if (iItemNum >= iNum)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int is_bind_roleitem(ROLEITEM *pstRoleItem)
{
	if(pstRoleItem->InstFlag & INST_ITEM_BIND)
	{
		return 1;
	}

	return 0;
}

int is_same_roleitem(ZONESVRENV* pstEnv, ROLEITEM *pstRoleItem1, ROLEITEM *pstRoleItem2)
{
	ITEMDEF *p1;
	ITEMDEF *p2;
	if (pstRoleItem1->DefID != pstRoleItem2->DefID)
	{
		return 0;
	}

	if(is_bind_roleitem(pstRoleItem1) != is_bind_roleitem(pstRoleItem2))
	{
		return 0;
	}

	p1 = z_get_itemdef_roleitem(pstEnv, pstRoleItem1);
	p2 = z_get_itemdef_roleitem(pstEnv, pstRoleItem2);

	if (p1 && p2)
	{
		if (gem_is_chongneng(p1, pstRoleItem1) == GEM_STAT_POWER)
		{
			return 0;
		}

		if (gem_is_chongneng(p2, pstRoleItem2) == GEM_STAT_POWER)
		{
			return 0;
		}
	}

	return 1;
}

//fBind :  0 只扣除非绑定物品， 1 只扣除绑定物品
int package_dec_in_item_by_bindtype(int iItemID, ROLEITEM *pstRoleItems, unsigned short *punNum, 
	int iListType, unsigned short *piItemNum, int *piPos, ROLEPACKAGECHG *pstRolePakChg, int fBind)
{
	int i = 0;
	CSROLEITEM *pstCSRoleItem = NULL;
	ROLEITEM *pstItem = NULL;
	
	for (i=(*punNum)-1; i>=0; i--)
	{
		if (0 == (*piItemNum))
		{
			break;
		}
		
		pstItem = pstRoleItems + i;
		if(fBind != is_bind_roleitem(pstItem))
		{
				continue;
		}
		
		if (pstItem->DefID == iItemID && 0 == z_item_vaild_check(&gs_stEnv, pstItem))
		{
			if (pstItem->Num > *piItemNum)
			{
				pstItem->Num -= *piItemNum;
				*piItemNum = 0;

				if (piPos)
				{
					*piPos = i;
				}
			}
			else
			{
				*piItemNum -= pstItem->Num;
				pstItem->Num = 0;
			}

			if (pstRolePakChg->ItemChg.ChgNum < MAX_CHG_NUM)
			{
				pstCSRoleItem = pstRolePakChg->ItemChg.Items+pstRolePakChg->ItemChg.ChgNum;
				pstCSRoleItem->ListType = iListType;
				memcpy(&pstCSRoleItem->Item, pstItem, sizeof(pstCSRoleItem->Item));
				pstRolePakChg->ItemChg.ChgNum++;
			}

			if (0 == pstItem->Num)
			{
				(*punNum)--;
				if (i != *punNum)
				{
					memcpy(pstItem, pstRoleItems + (*punNum), sizeof(*pstItem));
				}
			}
			
		}
	}

	if (*piItemNum > 0)
	{
		if (piPos)
		{
			*piPos = -1;
		}

		return -1;
	}
	else
	{
		return 0;
	}
}

/*
	返回: 
			>0	扣掉了足够的物品，返回扣除的绑定物品的数量			
			0 	扣掉了足够的指定物品，扣除的物品中不包含绑定物品。
	             -1  	没有扣掉足够的指定物品
	iDecType   0 绑定和非绑定的一起扣， 1  只扣绑定的物品   2  只扣非绑定的物品	  	             
*/
int package_dec_in_item(int iItemID, ROLEITEM *pstRoleItems, unsigned short *punNum, int iListType, 
							unsigned short *piItemNum, int *piPos, ROLEPACKAGECHG *pstRolePakChg,int iDecType)
{
	int iRet = 0;
	int iRetDec = 0;
	CSROLEITEM *pstCSRoleItem = NULL;
	ROLEITEM *pstItem = NULL;
	int iOldItemNum = *piItemNum;

	if (piPos && (*piPos) >=0 && (*piPos) < (*punNum))
	{
		pstItem = pstRoleItems + (*piPos);
		if (pstItem->DefID == iItemID && 0 == z_item_vaild_check(&gs_stEnv, pstItem))
		{
			if( (PACKAGE_DEC_BIND == iDecType && !is_bind_roleitem(pstItem)) ||
				(PACKAGE_DEC_UNBIND == iDecType && is_bind_roleitem(pstItem)) )
			{
				tlog_error(gs_stEnv.pstLogCat, 0, 0,  "package_dec_in_item err %d, dectype:%d,bindtype:%d",
						pstItem->DefID,iDecType,is_bind_roleitem(pstItem));
			}
			else
			{
				if(pstItem->Num >= (*piItemNum))
				{
					pstItem->Num -= *piItemNum;
					if(is_bind_roleitem(pstItem))
					{
						iRet += *piItemNum;
					}
					
					*piItemNum = 0;
				}
				else
				{
					*piItemNum -= pstItem->Num;
					if(is_bind_roleitem(pstItem))
					{
						iRet += pstItem->Num;
					}
					
					pstItem->Num = 0;
				}

				if (pstRolePakChg->ItemChg.ChgNum < MAX_CHG_NUM)
				{
					pstCSRoleItem = pstRolePakChg->ItemChg.Items+pstRolePakChg->ItemChg.ChgNum;
					pstCSRoleItem->ListType = iListType;
					memcpy(&pstCSRoleItem->Item, pstItem, sizeof(pstCSRoleItem->Item));
					pstRolePakChg->ItemChg.ChgNum++;
				}

				if (0 == pstItem->Num)
				{
					(*punNum)--;
					if ((*piPos) != (*punNum))
					{
						memcpy(pstItem, pstRoleItems + (*punNum), sizeof(*pstItem));
					}
				}
			}
		}
	}

	if (0 == (*piItemNum))
	{
		return iRet;
	}

	//位置排整齐
	qsort(pstRoleItems, *punNum, sizeof(*pstRoleItems), PakItemIdxCompare);

	
	iOldItemNum = *piItemNum;
	//先扣除绑定的
	if( (PACKAGE_DEC_NONE == iDecType) ||
		(PACKAGE_DEC_BIND == iDecType) )
	{
		if(piPos)
		{
			package_dec_in_item_by_bindtype(iItemID, pstRoleItems, punNum, 
					iListType, piItemNum, piPos, pstRolePakChg, 1);
		}
		else
		{
			package_dec_in_item_by_bindtype(iItemID, pstRoleItems, punNum, 
					iListType, piItemNum, NULL, pstRolePakChg, 1);
		}
	}

	//如果扣了绑定的物品需要知道
	if(*piItemNum < iOldItemNum)
	{
		iRet += (iOldItemNum-(*piItemNum));
	}

	//在扣除非绑定的
	if( (PACKAGE_DEC_NONE == iDecType) ||
		(PACKAGE_DEC_UNBIND == iDecType) )
	{
		if(piPos)
		{
			iRetDec = package_dec_in_item_by_bindtype(iItemID, pstRoleItems, punNum, 
					iListType, piItemNum, piPos, pstRolePakChg, 0);
		}
		else
		{
			iRetDec = package_dec_in_item_by_bindtype(iItemID, pstRoleItems, punNum, 
					iListType, piItemNum, NULL, pstRolePakChg, 0);
		}
	}
	
	if(0 > iRetDec)
	{
		iRet = -1;
	}

	return iRet;
}

//fBind :  0 只扣除非绑定物品， 1 只扣除绑定物品
int package_dec_in_grid_by_bindtype(int iItemID, ROLEGRID *pstRoleGrids, unsigned short *punNum, int iListType, 
	unsigned short *piItemNum, int *piPos, ROLEPACKAGECHG *pstRolePakChg, int fBind)
{
	int i = 0;
	CSROLEITEM *pstCSRoleItem = NULL;
	CSROLEARM *pstCSRoleArm = NULL;
	ROLEGRID *pstGrid = NULL;
	ROLEITEM *pstItem = NULL;
	
	for (i=(*punNum)-1; i>=0; i--)
	{
		if (0 == (*piItemNum))
		{
			break;
		}
		
		pstGrid = pstRoleGrids + i;
		pstItem = &pstGrid->GridData.RoleItem;
		if (pstItem->DefID == iItemID && 0 == z_item_vaild_check(&gs_stEnv, pstItem))
		{
			if(fBind != is_bind_roleitem(pstItem))
			{
					continue;
			}
		
			if (pstItem->Num > *piItemNum)
			{
				pstItem->Num -= *piItemNum;
				*piItemNum = 0;

				if (piPos)
				{
					*piPos = i;
				}
			}
			else
			{
				*piItemNum -= pstItem->Num;
				pstItem->Num = 0;
			}

					
			if (pstGrid->Type == GRID_ITEM_ITEM)
			{
				if (pstRolePakChg->ItemChg.ChgNum < MAX_CHG_NUM)
				{
					pstCSRoleItem = pstRolePakChg->ItemChg.Items+pstRolePakChg->ItemChg.ChgNum;
					pstCSRoleItem->ListType = iListType;
					memcpy(&pstCSRoleItem->Item, pstItem, sizeof(pstCSRoleItem->Item));
					pstRolePakChg->ItemChg.ChgNum++;
				}
			}
			else if (pstGrid->Type == GRID_ITEM_ARM)
			{
				if (pstRolePakChg->ArmChg.ChgNum < MAX_CHG_NUM)
				{
					pstCSRoleArm = pstRolePakChg->ArmChg.Arms+pstRolePakChg->ArmChg.ChgNum;
					pstCSRoleArm->ListType = iListType;
					memcpy(&pstCSRoleArm->Arm, &pstGrid->GridData.RoleArm, sizeof(pstCSRoleArm->Arm));
					pstRolePakChg->ArmChg.ChgNum++;
				}
			}
			else
			{
				break;
			}
			

			if (0 == pstItem->Num)
			{
				(*punNum)--;
				if (i != *punNum)
				{
					memcpy(pstGrid, pstRoleGrids+(*punNum), sizeof(*pstGrid));
				}
			}
			
		}
	}

	if (*piItemNum > 0)
	{
		if (piPos)
		{
			*piPos = -1;
		}

		return -1;
	}
	else
	{
		return 0;
	}
}

/*
	返回: 
			>0	扣掉了足够的物品，返回扣除物品中绑定道具的数量
			0 	扣掉了足够的指定物品，扣除的物品中不包含绑定物品。
	             -1  	没有扣掉足够的指定物品

	iDecType   0 绑定和非绑定的一起扣， 1  只扣绑定的物品   2  只扣非绑定的物品	  	             
*/
int package_dec_in_grid(int iItemID, ROLEGRID *pstRoleGrids, unsigned short *punNum, int iListType,
						unsigned short *piItemNum, int *piPos, ROLEPACKAGECHG *pstRolePakChg,int iDecType)
{
	int iRet = 0;
	int iRetDec = 0;
	CSROLEITEM *pstCSRoleItem = NULL;
	CSROLEARM *pstCSRoleArm = NULL;
	ROLEGRID *pstGrid = NULL;
	ROLEITEM *pstItem = NULL;
	int iOldItemNum = *piItemNum;

	if (piPos && (*piPos) >=0 && (*piPos) < (*punNum))
	{
		pstGrid = pstRoleGrids + (*piPos);
		pstItem = &pstGrid->GridData.RoleItem;
		
		if (pstItem->DefID == iItemID && 0 == z_item_vaild_check(&gs_stEnv, pstItem))
		{
			if( (PACKAGE_DEC_BIND == iDecType && !is_bind_roleitem(pstItem)) ||
				(PACKAGE_DEC_UNBIND == iDecType && is_bind_roleitem(pstItem)) )
			{
				tlog_error(gs_stEnv.pstLogCat, 0, 0,  "package_dec_in_item err %d, dectype:%d,bindtype:%d",
						pstItem->DefID,iDecType,is_bind_roleitem(pstItem));
			}
			else
			{
				if(pstItem->Num >= (*piItemNum))
				{
					pstItem->Num -= *piItemNum;
					if(is_bind_roleitem(pstItem))
					{
						iRet += *piItemNum;
					}
					*piItemNum = 0;
				}
				else
				{
					*piItemNum -= pstItem->Num;
					if(is_bind_roleitem(pstItem))
					{
						iRet += pstItem->Num;
					}
					pstItem->Num = 0;
				}

				if (pstGrid->Type == GRID_ITEM_ITEM)
				{
					if (pstRolePakChg->ItemChg.ChgNum < MAX_CHG_NUM)
					{
						pstCSRoleItem = pstRolePakChg->ItemChg.Items+pstRolePakChg->ItemChg.ChgNum;
						pstCSRoleItem->ListType = iListType;
						memcpy(&pstCSRoleItem->Item, pstItem, sizeof(pstCSRoleItem->Item));
						pstRolePakChg->ItemChg.ChgNum++;
					}
				}
				else if (pstGrid->Type == GRID_ITEM_ARM)
				{
					if (pstRolePakChg->ArmChg.ChgNum < MAX_CHG_NUM)
					{
						pstCSRoleArm = pstRolePakChg->ArmChg.Arms+pstRolePakChg->ArmChg.ChgNum;
						pstCSRoleArm->ListType = iListType;
						memcpy(&pstCSRoleArm->Arm, &pstGrid->GridData.RoleArm, sizeof(pstCSRoleArm->Arm));
						pstRolePakChg->ArmChg.ChgNum++;
					}
				}
				else
				{
					return -1;
				}

				if (0 == pstItem->Num)
				{
					(*punNum)--;
					if ((*piPos) != (*punNum))
					{
						memcpy(pstGrid, pstRoleGrids+(*punNum), sizeof(*pstGrid));
					}
				}
			}

		}
	}

	if (0 == (*piItemNum))
	{
		return iRet;
	}

	//位置排整齐
	qsort(pstRoleGrids, *punNum, sizeof(*pstRoleGrids), PakGridIdxCompare);

	iOldItemNum = *piItemNum;
	//先扣除绑定的
	if( (PACKAGE_DEC_NONE == iDecType) ||
		(PACKAGE_DEC_BIND == iDecType) )
	{
		if(piPos)
		{
			package_dec_in_grid_by_bindtype(iItemID, pstRoleGrids, punNum, 
					iListType, piItemNum, piPos, pstRolePakChg, 1);
		}
		else
		{
			package_dec_in_grid_by_bindtype(iItemID, pstRoleGrids, punNum, 
					iListType, piItemNum, NULL, pstRolePakChg, 1);
		}
	}

	//如果扣了绑定的物品需要知道
	if(*piItemNum < iOldItemNum)
	{
		iRet += (iOldItemNum - (*piItemNum));
	}

	//扣除非绑定的物品
	if( (PACKAGE_DEC_NONE == iDecType) ||
		(PACKAGE_DEC_UNBIND == iDecType) )
	{
		if(piPos)
		{
			iRetDec = package_dec_in_grid_by_bindtype(iItemID, pstRoleGrids, punNum, 
					iListType, piItemNum, piPos, pstRolePakChg, 0);
		}
		else
		{
			iRetDec = package_dec_in_grid_by_bindtype(iItemID, pstRoleGrids, punNum, 
					iListType, piItemNum, NULL, pstRolePakChg, 0);
		}
	}
	
	if(0 > iRetDec)
	{
		iRet = -1;
	}

	return iRet;
}
//iDecType   0 绑定和非绑定的一起扣， 1  只扣绑定的物品   2  只扣非绑定的物品	  
int package_dec_in(ZONESVRENV* pstEnv, ROLEPACKAGE *pstPak,  int *piPos, int iItemID, 
						int iListType, unsigned short *piItemNum, ROLEPACKAGECHG *pstRolePakChg,int iDecType)
{
	ITEMDEF *pstItemDef = NULL;
		
	if (-1 == iListType)
	{
		pstItemDef = z_find_itemdef(pstEnv, iItemID);
		if (NULL == pstItemDef)
		{
			return 0;
		}

		switch(pstItemDef->ItemType)
		{
			case ITEM_TYPE_AMM:
				return package_dec_in_item(iItemID, pstPak->AmmPackage.RoleItems, &pstPak->AmmPackage.Num, LIST_TYPE_AMM, piItemNum, piPos, pstRolePakChg,iDecType);
				break;
			case ITEM_TYPE_STUFF:
				return package_dec_in_grid(iItemID, pstPak->SundriesPackage.RoleGrids, &pstPak->SundriesPackage.Num, LIST_TYPE_SUNDRIES, piItemNum, piPos, pstRolePakChg,iDecType);							
				break;
			case ITEM_TYPE_TASK:
				return package_dec_in_item(iItemID, pstPak->TaskPackage.RoleItems, &pstPak->TaskPackage.Num, LIST_TYPE_TASK, piItemNum, piPos, pstRolePakChg,iDecType);
				break;
			default:
				return package_dec_in_grid(iItemID, pstPak->SundriesPackage.RoleGrids, &pstPak->SundriesPackage.Num, LIST_TYPE_SUNDRIES, piItemNum, piPos, pstRolePakChg,iDecType);
				break;
		}
	}
	else if(-2 == iListType)
	{
		int ret = 0;
		unsigned short iItemNum = *piItemNum;
		pstItemDef = z_find_itemdef(pstEnv, iItemID);
		if (NULL == pstItemDef)
		{
			return 0;
		}

		switch(pstItemDef->ItemType)
		{
			case ITEM_TYPE_AMM:
				ret = package_dec_in_item(iItemID, pstPak->AmmPackage.RoleItems, &pstPak->AmmPackage.Num, LIST_TYPE_AMM, piItemNum, piPos, pstRolePakChg,iDecType);
				break;
			case ITEM_TYPE_STUFF:
				ret = package_dec_in_grid(iItemID, pstPak->SundriesPackage.RoleGrids, &pstPak->SundriesPackage.Num, LIST_TYPE_SUNDRIES, piItemNum, piPos, pstRolePakChg,iDecType);							
				break;
			case ITEM_TYPE_TASK:
				ret = package_dec_in_item(iItemID, pstPak->TaskPackage.RoleItems, &pstPak->TaskPackage.Num, LIST_TYPE_TASK, piItemNum, piPos, pstRolePakChg,iDecType);
				break;
			default:
				ret = package_dec_in_grid(iItemID, pstPak->SundriesPackage.RoleGrids, &pstPak->SundriesPackage.Num, LIST_TYPE_SUNDRIES, piItemNum, piPos, pstRolePakChg,iDecType);
				break;
		}

		if(iItemNum > (*piItemNum))
		{
			ret = package_dec_in_grid(iItemID, pstPak->StashPackage.RoleGrids, &pstPak->StashPackage.Num, LIST_TYPE_STASH, piItemNum, piPos, pstRolePakChg,iDecType);
		}
		
		return ret;
	}

	switch(iListType)
	{
		case LIST_TYPE_AMM:
			return package_dec_in_item(iItemID, pstPak->AmmPackage.RoleItems, &pstPak->AmmPackage.Num, LIST_TYPE_AMM, piItemNum, piPos, pstRolePakChg,iDecType);
			break;
		case LIST_TYPE_STUFF:
			return package_dec_in_item(iItemID, pstPak->StuffPackage.RoleItems, &pstPak->StuffPackage.Num, LIST_TYPE_STUFF, piItemNum, piPos, pstRolePakChg,iDecType);
			break;
		case LIST_TYPE_TASK:
			return package_dec_in_item(iItemID, pstPak->TaskPackage.RoleItems, &pstPak->TaskPackage.Num, LIST_TYPE_TASK, piItemNum, piPos, pstRolePakChg,iDecType);
			break;
		case LIST_TYPE_SUNDRIES:
			return package_dec_in_grid(iItemID, pstPak->SundriesPackage.RoleGrids, &pstPak->SundriesPackage.Num, LIST_TYPE_SUNDRIES, piItemNum, piPos, pstRolePakChg,iDecType);
			break;
		case LIST_TYPE_SHOPPING:
			return package_dec_in_grid(iItemID, pstPak->ShoppingPackage.RoleGrids, &pstPak->ShoppingPackage.Num, LIST_TYPE_SHOPPING, piItemNum, piPos, pstRolePakChg,iDecType);
			break;
		default:
			return -1;
			break;
	}
	
	return 0;
}



void itemdec_fill(ITEMDEC *pstItemDec, int iID, int iNum, int iBind)
{
	pstItemDec->ID = iID;
	pstItemDec->Num = iNum;
	pstItemDec->BindNum = 0;
	if(iBind >0 ) 
		pstItemDec->BindNum = iBind;
}

/*
	输入:  iListType  -1 不指定栏位，其他指定栏位
	              piPos NULL 没有建议的位置, 否则为建议的位置

	返回: 
			>0	扣掉了足够的物品，返回扣除物品中绑定道具的数量
			0 	扣掉了足够的指定物品，扣除的物品中不包含绑定物品。
	             -1  	没有扣掉足够的指定物品

	iDecType   0 绑定和非绑定的一起扣， 1  只扣绑定的物品   2  只扣非绑定的物品	   	             
	                            
*/
int package_dec(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID, 
						int iNum, int iListType, int *piPos,int iDecType, int iWhatWay)
{	
	CSPKG stPkg;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	unsigned short iItemNum = iNum;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstRolePakChg = NULL;
	int iRet = 0;
	ITEMDEF *pstItemDef;
	UNUSED(pstAppCtx);

	pstEnv->iRealItemDecNum = 0;

	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstRolePakChg = &pstPackageInfo->PackageInfoData.PackageChg;
	pstRolePakChg->ArmChg.ChgNum = 0;
	pstRolePakChg->ItemChg.ChgNum = 0;

	/*-2表示背包和仓库*/
	if (-1 == iListType || -2 == iListType)
	{
		iRet = package_dec_in(pstEnv, pstPak, NULL,  iItemID, iListType, &iItemNum, pstRolePakChg,iDecType);	
		if (pstRolePakChg->ArmChg.ChgNum > 0 || pstRolePakChg->ItemChg.ChgNum > 0)
		{
			Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		}
		
		goto _end;
	}

	switch(iListType)
	{
		case LIST_TYPE_AMM:
			iRet = package_dec_in(pstEnv, pstPak, piPos, iItemID, iListType, &iItemNum, pstRolePakChg,iDecType);	
			break;
		case LIST_TYPE_STUFF:
			iRet = package_dec_in(pstEnv, pstPak, piPos, iItemID, iListType, &iItemNum, pstRolePakChg,iDecType);	
			break;
		case LIST_TYPE_TASK:
			iRet = package_dec_in(pstEnv, pstPak, piPos, iItemID, iListType, &iItemNum, pstRolePakChg,iDecType);	
			break;
		case LIST_TYPE_SUNDRIES:
			iRet = package_dec_in(pstEnv, pstPak, piPos, iItemID, iListType, &iItemNum, pstRolePakChg,iDecType);	
			break;
		case LIST_TYPE_SHOPPING:
			iRet = package_dec_in(pstEnv, pstPak, piPos, iItemID, iListType, &iItemNum, pstRolePakChg,iDecType);	
			break;
		default:
			return -1;
			break;
	}

	if (pstRolePakChg->ArmChg.ChgNum > 0 || pstRolePakChg->ItemChg.ChgNum > 0)
	{
		Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

_end:
	pstEnv->iRealItemDecNum = iNum - iItemNum;

	if (pstEnv->iRealItemDecNum > 0 && iWhatWay > 0)
	{
		pstItemDef = z_find_itemdef(pstEnv, iItemID);
		if (pstItemDef && (ITEM_IMP_LOGTRACE & pstItemDef->CtrlFlag))
		{
			z_impitem_consume_oplog(pstEnv, pstPlayer, iItemID, iWhatWay, pstEnv->iRealItemDecNum, iRet);
		}
	}
	
	return iRet;
}

int check_npc_ui(ZONESVRENV* pstEnv, Player *pstPlayer, int iUIType)
{
	Npc *pstNpc = NULL;

	//骑乘携带NPC 打开仓库
	if (UI_TYPE_STASH == iUIType)
	{
		if (pstPlayer->stOnline.State & (CS_STAT_RIDE_ON|CS_STAT_FLY))
		{
			RIDEDEF *pstRideDef = NULL;
			RIDENPCSELECT *pstRideNpcSelect = NULL;
		
			pstRideDef = z_find_ride_def(pstEnv, pstPlayer->stOnline.RideItemID, pstPlayer->stOnline.RideLevel);
			if(pstRideDef)
			{
				pstRideNpcSelect = z_get_ride_npc(pstRideDef, RIDE_OPEN_STASH);
				if(pstRideNpcSelect)
				{
					return 0;
				}
			}	
		}

		if (NULL != star_get_result(pstEnv, pstPlayer, STAR_RESULT_STASH))
		{
			return 0;
		}
	}
	
	if (iUIType != pstPlayer->stOnline.cUIType)
	{
		return -1;
	}

	if (pstPlayer->stOnline.State & CS_STAT_STALL)
	{
		return -1;
	}

	pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.iDialogID);
	if (NULL == pstNpc)
	{
		return -1;
	}

	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	}

	if (0 > z_check_op_dist(pstEnv, pstPlayer, pstNpc->stMon.stMap.iID, &pstNpc->stMon.stCurrPos, OP_DIST+pstNpc->stMon.unRadius))
	{
		return -1;
	}

	
	return 0;
}

int player_login_check_pkgbug(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	EXTENDSTASHPKG *pstEtStashPkg = &pstPak->EtStashPkg;
	ITEMDEF *pstItemDef;
	int i;
	ROLEITEM *pstRoleItem;
	ROLEGRID stGrid[MAX_EXTEND_STASH_PKG];
	int iNum = 0;
	int iAdd = 0;

	for (i=0; i<pstEtStashPkg->Num; i++)
	{
		pstRoleItem = &pstEtStashPkg->RoleItems[i];
		pstItemDef = z_find_itemdef( pstEnv, pstRoleItem->DefID);
		if (pstItemDef)
		{
			if (pstItemDef->Result[0].ResultID != RESULT_ADD_SUNDPKG_GRID)
			{
				stGrid[iNum].Type = GRID_ITEM_ITEM;
				stGrid[iNum].GridData.RoleItem = *pstRoleItem;
				iNum ++;
			}
			else
			{
				iAdd+=pstItemDef->Result[0].ResultVal1;
			}
		}
	}

	if (iNum > 0)
	{
		if (package_add_test(pstEnv->pstAppCtx,pstEnv,pstPlayer, &stGrid[0], iNum,0) < 0)
		{
			return 0;
		}	

		// 实际格子
		iAdd += INIT_STASH_PACKAGE;

		if (pstPak->StashPackage.MaxNum > iAdd )
		{
			for (i=0; i<pstPak->StashPackage.Num; i++)
			{
				if (pstPak->StashPackage.RoleGrids[i].GridData.RoleItem.GridIdx >= iAdd)
				{
					return 0;
				}
			}	
		}

		pstPak->StashPackage.MaxNum = iAdd;
		package_add(pstEnv->pstAppCtx,pstEnv,pstPlayer, &stGrid[0], iNum,0,0);

		for (i=pstEtStashPkg->Num-1; i>=0; i--)
		{
			pstRoleItem = &pstEtStashPkg->RoleItems[i];
			pstItemDef = z_find_itemdef( pstEnv, pstRoleItem->DefID);
			if (pstItemDef)
			{
				if (pstItemDef->Result[0].ResultID != RESULT_ADD_SUNDPKG_GRID)
				{
					pstEtStashPkg->Num --;
					if (i != pstEtStashPkg->Num)
					{
						*pstRoleItem = pstEtStashPkg->RoleItems[pstEtStashPkg->Num];
					}
				}
			}
		}
	}
	return 0;
}

static int pkg_check_etstash_move(ZONESVRENV* pstEnv, ROLEPACKAGE *pstPkg,ROLEITEM *pstEtItem, 
				ROLEITEM* pstStashItem, int*iListType, int iSwap, int iDstGird, Player *pstPlayer)
{
	ITEMDEF *pstEtItemDef;
	ITEMDEF *pstStashItemDef;
	RESULT *pstEtResult;
	RESULT *pstStashResult;
	ROLEGRID *pstRoleGrid;
//	ROLEITEM *pstRoleItem;
	int i;
	int iGridDiff;
	int iStashVaild = get_vaild_gird(pstEnv, pstPkg, LIST_TYPE_STASH);
	
	if (pstStashItem)
	{
		// 仓库中的物品不允许过期
		if (pstStashItem->ValidEnd!= 0 && pstStashItem->ValidEnd <pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK193);
			return -1;
		}
	}

	if(iSwap)
	{
		if (!pstStashItem || !pstEtItem)
		{
			return -1;
		}

		if (pstStashItem->GridIdx >= iStashVaild)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK194);
			return -1;
		}
			
		pstStashItemDef = z_find_itemdef(pstEnv, pstStashItem->DefID);
		pstEtItemDef = z_find_itemdef(pstEnv, pstEtItem->DefID);
		if (!pstStashItemDef || !pstEtItemDef)
		{
			return -1;
		}
		pstStashResult = z_item_get_result(pstStashItemDef, RESULT_ADD_SUNDPKG_GRID);
		pstEtResult = z_item_get_result(pstEtItemDef, RESULT_ADD_SUNDPKG_GRID);
		if (pstStashResult && pstEtResult)
		{
			iGridDiff = pstStashResult->ResultVal1 - pstEtResult->ResultVal1;
			// 减格子 要判断剪掉的格子有没有道具
			// 还要判断移动的item是不是在无效的格子中
			if (iGridDiff < 0)
			{
				for (i=0; i<pstPkg->StashPackage.Num; i++)
				{
					pstRoleGrid = &pstPkg->StashPackage.RoleGrids[i];
					if (	pstRoleGrid->GridData.RoleItem.GridIdx >= pstPkg->StashPackage.MaxNum+iGridDiff)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK195);
						return -1;
					}
				}

				if (pstStashItem->GridIdx >= pstPkg->StashPackage.MaxNum+iGridDiff)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK196);
					return -1;
				}

				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK241, -iGridDiff);
			}
			else if (iGridDiff > 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK242, iGridDiff);
			}
			
			*iListType = LIST_TYPE_STASH;
			return 0;
		}
	}
	else 
	{
		if (pstStashItem)
		{
			if (pstPkg->EtStashPkg.Num >= pstPkg->EtStashPkg.MaxNum)
			{
				return -1;
			}
			pstStashItemDef = z_find_itemdef(pstEnv,pstStashItem->DefID);
			if (!pstStashItemDef)
				return -1;
			pstStashResult = z_item_get_result(pstStashItemDef, RESULT_ADD_SUNDPKG_GRID);
			if (!pstStashResult)
			{
				return -1;
			}
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK242, pstStashResult->ResultVal1);
			*iListType = LIST_TYPE_STASH;
			return 0;
		}
		else if (pstEtItem)
		{
			pstEtItemDef = z_find_itemdef(pstEnv,pstEtItem->DefID);
			if (!pstEtItemDef)
				return -1;
			
			pstEtResult = z_item_get_result(pstEtItemDef, RESULT_ADD_SUNDPKG_GRID);
			if (pstEtResult)
			{
				for (i=0; i<pstPkg->StashPackage.Num; i++)
				{
					pstRoleGrid = &pstPkg->StashPackage.RoleGrids[i];
					if (	pstRoleGrid->GridData.RoleItem.GridIdx >= pstPkg->StashPackage.MaxNum - pstEtResult->ResultVal1)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK195);
						return -1;
					}
					
				}

				if (iDstGird >= pstPkg->StashPackage.MaxNum - pstEtResult->ResultVal1)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK196);
					return -1;
				}

				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK241, pstEtResult->ResultVal1);
				*iListType = LIST_TYPE_STASH;
				return 0;
			}
		}
		else
		{
			return -1;
		}
	}
	
	return -1;
}

static int pkg_check_etstash2sund_move(ZONESVRENV* pstEnv, ROLEPACKAGE *pstPkg,ROLEITEM *pstEtItem, 
									ROLEITEM* pstPkgItem,int*iListType, int iSwap, int iDstGrid, Player *pstPlayer)
{
	int iSundVaild = get_vaild_gird(pstEnv, pstPkg, LIST_TYPE_SUNDRIES);
	ITEMDEF *pstPkgItemDef;
	ITEMDEF *pstEtItemDef;
	RESULT *pstEtResult;
	RESULT *pstPkgResult;
	int iGridDiff;
	ROLEGRID *pstRoleGrid;
//	ROLEITEM *pstRoleItem;
	int i;

	if (pstPkgItem)
	{
		// 背包中的物品不允许过期
		if (pstPkgItem->ValidEnd!= 0 && pstPkgItem->ValidEnd <pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK193);
			return -1;
		}
	}

	if (iSwap)
	{
		if (!pstPkgItem || !pstEtItem)
		{
			return -1;
		}

		if (pstPkgItem->GridIdx >= iSundVaild)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK194);
			return -1;
		}

		pstPkgItemDef = z_find_itemdef(pstEnv, pstPkgItem->DefID);
		pstEtItemDef = z_find_itemdef(pstEnv, pstEtItem->DefID);
		if (!pstPkgItemDef || !pstEtItemDef)
		{
			return -1;
		}

		pstPkgResult = z_item_get_result(pstPkgItemDef, RESULT_ADD_SUNDPKG_GRID);
		pstEtResult = z_item_get_result(pstEtItemDef, RESULT_ADD_SUNDPKG_GRID);
		if (pstPkgResult && pstEtResult)
		{
			iGridDiff = pstPkgResult->ResultVal1 - pstEtResult->ResultVal1;
			if (iGridDiff < 0)
			{
				for (i=0; i<pstPkg->StashPackage.Num; i++)
				{
					pstRoleGrid = &pstPkg->StashPackage.RoleGrids[i];
					if (	pstRoleGrid->GridData.RoleItem.GridIdx >= pstPkg->StashPackage.MaxNum+iGridDiff)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK194);
						return -1;
					}
				}

				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK241, -iGridDiff);
			}
			else if (iGridDiff > 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK242, iGridDiff);
			}
			*iListType = LIST_TYPE_STASH;
		}
		else
		{
			return -1;
		}
		
	}
	else
	{
		if (pstPkgItem)
		{
			if(pstPkg->EtStashPkg.Num == pstPkg->EtStashPkg.MaxNum)
				return -1;
			
			pstPkgItemDef = z_find_itemdef(pstEnv, pstPkgItem->DefID);
			if (!pstPkgItemDef)
				return -1;
			pstPkgResult = z_item_get_result(pstPkgItemDef, RESULT_ADD_SUNDPKG_GRID);
			if (!pstPkgResult)
				return -1;
			else
			{
				*iListType = LIST_TYPE_STASH;
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK242, pstPkgResult->ResultVal1);
			}
		
		}
		else if(pstEtItem)
		{
			pstEtItemDef = z_find_itemdef(pstEnv, pstEtItem->DefID);
			if (!pstEtItemDef)
				return -1;
			
			pstEtResult = z_item_get_result(pstEtItemDef, RESULT_ADD_SUNDPKG_GRID);
			if (!pstEtResult)
				return -1;
			
			for (i=0; i<pstPkg->StashPackage.Num; i++)
			{
				pstRoleGrid = &pstPkg->StashPackage.RoleGrids[i];
				if (	pstRoleGrid->GridData.RoleItem.GridIdx >= pstPkg->StashPackage.MaxNum -pstEtResult->ResultVal1)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK195);
					return -1;
				}
			}

			*iListType = LIST_TYPE_STASH;
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK241, pstEtResult->ResultVal1);
			
		}
	}

	return 0;
	UNUSED(iDstGrid);	//add by paraunused.pl
}

static int pkg_check_etsund_move(ZONESVRENV* pstEnv, ROLEPACKAGE *pstPkg,ROLEITEM *pstEtItem, 
				ROLEITEM* pstPkgItem,int*iListType, int iSwap, int iDstGrid, Player *pstPlayer)
{
	ITEMDEF *pstPkgItemDef;
	ITEMDEF *pstEtItemDef;
	RESULT *pstEtResult;
	RESULT *pstPkgResult;
	ROLEGRID *pstRoleGrid;
	int i;
	int iGridDiff;
	int iSundVaild = get_vaild_gird(pstEnv, pstPkg, LIST_TYPE_SUNDRIES);

	if (pstPkgItem)
	{
		// 背包中的物品不允许过期
		if (pstPkgItem->ValidEnd!= 0 && pstPkgItem->ValidEnd <pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK193);
			return -1;
		}
	}
	
	if (iSwap)
	{
		if (!pstPkgItem || !pstEtItem)
		{
			return -1;
		}

		if (pstPkgItem->GridIdx >= iSundVaild)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK194);
			return -1;
		}
		
		pstPkgItemDef = z_find_itemdef(pstEnv, pstPkgItem->DefID);
		pstEtItemDef = z_find_itemdef(pstEnv, pstEtItem->DefID);
		if (!pstPkgItemDef || !pstEtItemDef)
		{
			return -1;
		}
		pstPkgResult = z_item_get_result(pstPkgItemDef, RESULT_ADD_SUNDPKG_GRID);
		pstEtResult = z_item_get_result(pstEtItemDef, RESULT_ADD_SUNDPKG_GRID);
		if (pstPkgResult && pstEtResult)
		{
			iGridDiff = pstPkgResult->ResultVal1 - pstEtResult->ResultVal1;
			// 减格子 要判断剪掉的格子有没有道具
			// 还要判断移动的item是不是在无效的格子中
			if (iGridDiff < 0)
			{
				for (i=0; i<pstPkg->SundriesPackage.Num; i++)
				{
					pstRoleGrid = &pstPkg->SundriesPackage.RoleGrids[i];
					if (	pstRoleGrid->GridData.RoleItem.GridIdx >= pstPkg->SundriesPackage.MaxNum+iGridDiff)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK195);
						return -1;
					}
				}

				if (pstPkgItem->GridIdx >= pstPkg->SundriesPackage.MaxNum+iGridDiff)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK196);
					return -1;
				}

				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK198, -iGridDiff);
			}
			else if (iGridDiff > 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK197, iGridDiff);
			}
			*iListType = LIST_TYPE_SUNDRIES;
			
			return 0;
		}
	}
	else
	{
		if (pstPkgItem)
		{
			pstPkgItemDef = z_find_itemdef(pstEnv, pstPkgItem->DefID);
			if (!pstPkgItemDef)
				return -1;
			
			pstPkgResult = z_item_get_result(pstPkgItemDef, RESULT_ADD_SUNDPKG_GRID);
			if (!pstPkgResult)
				return -1;
			else
			{
				*iListType = LIST_TYPE_SUNDRIES;
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK197, pstPkgResult->ResultVal1);
			}
			
			return 0;
		}
		else if (pstEtItem)
		{
			pstEtItemDef = z_find_itemdef(pstEnv, pstEtItem->DefID);
			if (!pstEtItemDef)
				return -1;
			
			pstEtResult = z_item_get_result(pstEtItemDef, RESULT_ADD_SUNDPKG_GRID);
			if (pstEtResult)
			{
				for (i=0; i<pstPkg->SundriesPackage.Num; i++)
				{
					pstRoleGrid = &pstPkg->SundriesPackage.RoleGrids[i];
					if (	pstRoleGrid->GridData.RoleItem.GridIdx >= pstPkg->SundriesPackage.MaxNum -pstEtResult->ResultVal1)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK195);
						return -1;
					}
				}

				if (	iDstGrid >= pstPkg->SundriesPackage.MaxNum -pstEtResult->ResultVal1 )
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK196);
					return -1;
				}

				*iListType = LIST_TYPE_SUNDRIES;
				
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK198, pstEtResult->ResultVal1);
				return 0;
			}
		}
		else
		{
			return -1;
		}
		
	}
	return -1;
}

static int pkg_check_etsund2stash_move(ZONESVRENV* pstEnv, ROLEPACKAGE *pstPkg,ROLEITEM *pstEtItem, 
				ROLEITEM* pstPkgItem,int*iListType, int iSwap, int iDstGrid, Player *pstPlayer)
{
	ITEMDEF *pstPkgItemDef;
	ITEMDEF *pstEtItemDef;
	RESULT *pstEtResult;
	RESULT *pstPkgResult;
	ROLEGRID *pstRoleGrid;
	//ROLEITEM *pstRoleItem;	//comment by paraunused.pl
	int i;
	int iGridDiff;
	int iSundVaild = get_vaild_gird(pstEnv, pstPkg, LIST_TYPE_STASH);
	//int iStuffVaild;	//comment by paraunused.pl

	if (pstPkgItem)
	{
		// 背包中的物品不允许过期
		if (pstPkgItem->ValidEnd!= 0 && pstPkgItem->ValidEnd <pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK193);
			return -1;
		}
	}
	
	if (iSwap)
	{
		if (!pstPkgItem || !pstEtItem)
		{
			return -1;
		}

		if (pstPkgItem->GridIdx >= iSundVaild)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK194);
			return -1;
		}
		
		pstPkgItemDef = z_find_itemdef(pstEnv, pstPkgItem->DefID);
		pstEtItemDef = z_find_itemdef(pstEnv, pstEtItem->DefID);
		if (!pstPkgItemDef || !pstEtItemDef)
		{
			return -1;
		}
		pstPkgResult = z_item_get_result(pstPkgItemDef, RESULT_ADD_SUNDPKG_GRID);
		pstEtResult = z_item_get_result(pstEtItemDef, RESULT_ADD_SUNDPKG_GRID);
		if (pstPkgResult && pstEtResult)
		{
			iGridDiff = pstPkgResult->ResultVal1 - pstEtResult->ResultVal1;
			// 减格子 要判断剪掉的格子有没有道具
			// 还要判断移动的item是不是在无效的格子中
			if (iGridDiff < 0)
			{
				for (i=0; i<pstPkg->SundriesPackage.Num; i++)
				{
					pstRoleGrid = &pstPkg->SundriesPackage.RoleGrids[i];
					if (	pstRoleGrid->GridData.RoleItem.GridIdx >= pstPkg->SundriesPackage.MaxNum+iGridDiff)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK195);
						return -1;
					}
				}
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK198, -iGridDiff);
			}
			else if (iGridDiff > 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK197, iGridDiff);
			}
			*iListType = LIST_TYPE_SUNDRIES;
			
			return 0;
		}
	}
	else
	{
		if (pstPkgItem)
		{
			pstPkgItemDef = z_find_itemdef(pstEnv, pstPkgItem->DefID);
			if (!pstPkgItemDef)
				return -1;
			
			pstPkgResult = z_item_get_result(pstPkgItemDef, RESULT_ADD_SUNDPKG_GRID);
			if (!pstPkgResult)
				return -1;
			else
			{
				*iListType = LIST_TYPE_SUNDRIES;
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK197, pstPkgResult->ResultVal1);
			}
			
			return 0;
		}
		else if (pstEtItem)
		{
			pstEtItemDef = z_find_itemdef(pstEnv, pstEtItem->DefID);
			if (!pstEtItemDef)
				return -1;
			
			pstEtResult = z_item_get_result(pstEtItemDef, RESULT_ADD_SUNDPKG_GRID);
			if (pstEtResult)
			{
				for (i=0; i<pstPkg->SundriesPackage.Num; i++)
				{
					pstRoleGrid = &pstPkg->SundriesPackage.RoleGrids[i];
					if (	pstRoleGrid->GridData.RoleItem.GridIdx >= pstPkg->SundriesPackage.MaxNum -pstEtResult->ResultVal1)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK195);
						return -1;
					}
				}

				*iListType = LIST_TYPE_SUNDRIES;
				
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK198, pstEtResult->ResultVal1);
				return 0;
			}
		}
		else
		{
			return -1;
		}
		
	}
	return -1;
	UNUSED(iDstGrid);	//add by paraunused.pl
}


int pkg_num_chg(ZONESVRENV* pstEnv, Player *pstPlayer, int iListType)
{
	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGE *pstPkg = &pstPlayer->stRoleData.Package;
	int i;
	int iChg = 0;
	int iMaxNum;
	ROLEITEM *pstRoleItem;
	ITEMDEF *pstItemDef;
	RESULT *pstResult;

	if (iListType == LIST_TYPE_SUNDRIES)
	{
		iMaxNum = pstPkg->SundriesPackage.MaxNum;
		pstPkg->SundriesPackage.MaxNum = get_pkg_basenum(pstEnv, pstPlayer, iListType);
		for (i=0; i<pstPkg->EtSundPkg.Num; i++)
		{
			pstRoleItem = &pstPkg->EtSundPkg.RoleItems[i];
			pstItemDef = z_find_itemdef(pstEnv, pstRoleItem->DefID);
			if (pstItemDef)
			{
				pstResult = z_item_get_result(pstItemDef, RESULT_ADD_SUNDPKG_GRID);
				if (pstResult)
				{
					pstPkg->SundriesPackage.MaxNum += pstResult->ResultVal1;
				}
			}
		}

		if (pstPkg->SundriesPackage.MaxNum > MAX_SUNDRIES_PACKAGE)
		{
			pstPkg->SundriesPackage.MaxNum = MAX_SUNDRIES_PACKAGE;
		}
		
		if (iMaxNum != pstPkg->SundriesPackage.MaxNum)
		{
			iChg = 1;
			pstPackageInfo->Type = PACKAGE_SUNDRIES_NUM_CHG;
			pstPackageInfo->PackageInfoData.SundriesNum = pstPkg->SundriesPackage.MaxNum;
		}
	}
	else if(iListType == LIST_TYPE_STASH)
	{
		iMaxNum = pstPkg->StashPackage.MaxNum;
		pstPkg->StashPackage.MaxNum = get_pkg_basenum(pstEnv, pstPlayer, iListType);
		for (i=0; i<pstPkg->EtStashPkg.Num; i++)
		{
			pstRoleItem = &pstPkg->EtStashPkg.RoleItems[i];
			pstItemDef = z_find_itemdef(pstEnv, pstRoleItem->DefID);
			if (pstItemDef)
			{
				pstResult = z_item_get_result(pstItemDef, RESULT_ADD_SUNDPKG_GRID);
				if (pstResult)
				{
					pstPkg->StashPackage.MaxNum += pstResult->ResultVal1;
				}
			}
		}

		if (pstPkg->StashPackage.MaxNum > MAX_STASH_PACKAGE)
		{
			pstPkg->StashPackage.MaxNum = MAX_STASH_PACKAGE;
		}

		if (iMaxNum != pstPkg->StashPackage.MaxNum)
		{
			iChg = 1;
			pstPackageInfo->Type = PACKAGE_STASH_NUM_CHG;
			pstPackageInfo->PackageInfoData.StashNum = pstPkg->StashPackage.MaxNum;
		}
	}
	else if(iListType == LIST_TYPE_STUFF)
	{
		iMaxNum = pstPkg->StuffPackage.MaxNum;
		pstPkg->StuffPackage.MaxNum = get_pkg_basenum(pstEnv, pstPlayer, iListType);
		for (i=0; i<pstPkg->EtSundPkg.Num; i++)
		{
			pstRoleItem = &pstPkg->EtSundPkg.RoleItems[i];
			pstItemDef = z_find_itemdef(pstEnv, pstRoleItem->DefID);
			if (pstItemDef)
			{
				pstResult = z_item_get_result(pstItemDef, RESULT_ADD_STUFFPKG_GRID);
				if (pstResult)
				{
					pstPkg->StuffPackage.MaxNum += pstResult->ResultVal1;
				}
			}
		}

		if (pstPkg->StuffPackage.MaxNum > MAX_STUFF_PACKAGE)
		{
			pstPkg->StuffPackage.MaxNum = MAX_STUFF_PACKAGE;
		}

		if (iMaxNum != pstPkg->StuffPackage.MaxNum)
		{
			iChg = 1;
			pstPackageInfo->Type = PACKAGE_STUFF_NUM_CHG;
			pstPackageInfo->PackageInfoData.StuffNum = pstPkg->StuffPackage.MaxNum;
		}
	}
	else if (iListType == LIST_TYPE_SPAR)
	{
		if ( pstPkg->SparPkg.MaxNum > MAX_SPAR_PKG)
		{
			pstPkg->SparPkg.MaxNum = MAX_SPAR_PKG;
		}
		
		iChg = 1;
		pstPackageInfo->Type = PACKAGE_SPAR_NUM_CHG;
		pstPackageInfo->PackageInfoData.SparNum = pstPkg->SparPkg.MaxNum;
	}
	else
	{
		return -1;
	}

	if (iChg == 0)
		return 0;
	
	Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int package_drop_msg(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEGRID *pstRoleGrid)
{
	CSPKG stPkg;
	
	if(pstRoleGrid->Type == GRID_ITEM_ITEM)
	{
		z_fill_link_msg(&stPkg, SYS_COMBAT, CHAT_LINK_ITEM, (void *)&pstRoleGrid->GridData.RoleItem,
						ZONEERR_LINK306, pstRoleGrid->GridData.RoleItem.Num);
		
	}
	else
	{
		z_fill_link_msg(&stPkg, SYS_COMBAT, CHAT_LINK_ARM, (void *)&pstRoleGrid->GridData.RoleArm,
						ZONEERR_LINK306, pstRoleGrid->GridData.RoleItem.Num);
	}

	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int package_move_is_bind(ZONESVRENV * pstEnv,ROLEGRID *pstSrc, ROLEGRID *pstDsc)
{
	ITEMDEF *pstItemDef;
	if (!pstSrc || !pstDsc ||pstSrc->Type != GRID_ITEM_ITEM ||pstDsc->Type != GRID_ITEM_ITEM)
	{
		return 0;
	}

	pstItemDef = z_find_itemdef(pstEnv, pstDsc->GridData.RoleItem.DefID);
	if (!pstItemDef)
		return 0;

	if (	!(pstSrc->GridData.RoleItem.InstFlag& INST_ITEM_BIND) && 
		(pstDsc->GridData.RoleItem.InstFlag& INST_ITEM_BIND) &&
		pstSrc->GridData.RoleItem.DefID == pstDsc->GridData.RoleItem.DefID &&
		pstItemDef->MaxPile > 1 &&
		pstDsc->GridData.RoleItem.Num < pstItemDef->MaxPile)
	{
		return 1;
	}

	return 0;
}

int package_move_is_pile(ZONESVRENV * pstEnv,ROLEGRID *pstSrc, ROLEGRID *pstDsc)
{
	ITEMDEF *pstItemDef;
	if (!pstSrc || !pstDsc ||pstSrc->Type != GRID_ITEM_ITEM ||pstDsc->Type != GRID_ITEM_ITEM)
	{
		return 0;
	}

	if (pstSrc->GridData.RoleItem.DefID != pstDsc->GridData.RoleItem.DefID)
	{
		return 0;
	}

	pstItemDef = z_find_itemdef(pstEnv, pstSrc->GridData.RoleItem.DefID);
	if (!pstItemDef || pstItemDef->MaxPile == pstDsc->GridData.RoleItem.Num)
	{
		return 0;
	}

	if (pstItemDef)
	{
		if (gem_is_chongneng(pstItemDef, &pstSrc->GridData.RoleItem) == GEM_STAT_POWER)
		{
			return 0;
		}
	}

	pstItemDef = z_find_itemdef(pstEnv, pstDsc->GridData.RoleItem.DefID);
	if (pstItemDef)
	{
		if (gem_is_chongneng(pstItemDef, &pstDsc->GridData.RoleItem) == GEM_STAT_POWER)
		{
			return 0;
		}
	}

	if (	(pstSrc->GridData.RoleItem.InstFlag& INST_ITEM_BIND) &&
		!(pstDsc->GridData.RoleItem.InstFlag& INST_ITEM_BIND) )
	{
		return 0;
	}

	

	return 1;
}

int package_move(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSPACKAGEMOVE *pstPakMove)
{
	CSPKG *pstPkgRes = &pstEnv->stCSRes;
	CSPACKAGEINFO *pstPackageInfo = &pstPkgRes->Body.PackageInfo;
	ROLEPACKAGECHG *pstRolePakChg = NULL;
	CSROLEITEM *pstCSRoleItem = NULL;
	CSROLEARM  *pstCSRoleArm = NULL;
	ITEMDEF *pstItemDef = NULL, *pstDstItemDef=NULL;
	int  iPos;
	int iSrcGridNum = 0;
	int iMaxPile = 0;
	int iMovePile = pstPakMove->SrcGridNum;
	int iSwapFlag = 0;
	ROLEITEM *pstSrcGridItem = NULL;
	ROLEITEM *pstDstGridItem = NULL;
	ROLEGRID *pstSrcGrid = NULL;
	ROLEGRID *pstDstGrid = NULL, stSwapGrid;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	RESULT *pstItemResult;
	//RESULT *pstItemResult1;	//comment by paraunused.pl
	int iListType = -2;
	ROLEITEM *pstCommRoleItem = NULL;
//	int iBindSrc = 0;
	
	memset(&stSwapGrid, 0 , sizeof(stSwapGrid));
	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstRolePakChg = &pstPackageInfo->PackageInfoData.PackageChg;
	pstRolePakChg->ArmChg.ChgNum = 0;
	pstRolePakChg->ItemChg.ChgNum = 0;

	if (	(pstPakMove->SrcListType == LIST_TYPE_STASH &&
		(pstPakMove->DstListType == LIST_TYPE_SUNDRIES || 
		pstPakMove->DstListType == LIST_TYPE_STUFF ||
		pstPakMove->DstListType == LIST_TYPE_ET_SUND)) ||
		(pstPakMove->SrcListType == LIST_TYPE_ET_STASH &&
		pstPakMove->DstListType == LIST_TYPE_SUNDRIES) ||
		(pstPakMove->DstListType == LIST_TYPE_DROP))
	{
		if (player_check_safelock(pstEnv, pstPlayer) < 0)
		{
			return -1;
		}
	}

	
	if (pstPakMove->SrcGridNum <= 0)
	{
		return -1;
	}

	if (	pstPakMove->SrcListType == pstPakMove->DstListType 
		&& pstPakMove->SrcGridIdx == pstPakMove->DstGridIdx)
	{
		return -1;
	}

	
	// 目标格子失效
	if (pstPakMove->DstListType != -1)
	{
		if (gridIdx_is_unvaild(pstEnv, pstPak, pstPakMove->DstListType, pstPakMove->DstGridIdx) != 1)
		{		
			z_send_sys_str_msg( pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_PAK2);
			return -1;
		}
	}
	
	
	if( LIST_TYPE_SHOPPING == pstPakMove->DstListType &&
		pstPakMove->SrcListType != pstPakMove->DstListType )
	{
		/*商品背包的移动是单向的 */
		z_send_sys_str_msg( pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_PAK3);
		return -1;
	}

	switch(pstPakMove->SrcListType)
	{
		case LIST_TYPE_AMM:
			return -1;
			break;
		case LIST_TYPE_STUFF:
			return -1;
			break;
		case LIST_TYPE_TASK:
			if (pstPakMove->SrcGridIdx >= pstPak->TaskPackage.MaxNum ||
				(LIST_TYPE_DROP != pstPakMove->DstListType && LIST_TYPE_TASK != pstPakMove->DstListType &&
				LIST_TYPE_STASH != pstPakMove->DstListType))
			{
				return -1;
			}
			iPos = package_itemgrid_pos(pstPak->TaskPackage.RoleItems, pstPak->TaskPackage.Num, pstPakMove->SrcGridIdx);
			if (0 > iPos)
			{
				return -1;
			}

			pstSrcGridItem = pstPak->TaskPackage.RoleItems+iPos;
			iSrcGridNum = pstSrcGridItem->Num;
			pstItemDef = z_get_itemdef_roleitem(pstEnv, pstSrcGridItem);
			break;
		case LIST_TYPE_SUNDRIES:
			if (pstPakMove->SrcGridIdx >= pstPak->SundriesPackage.MaxNum || 
				(LIST_TYPE_DROP != pstPakMove->DstListType && 
				LIST_TYPE_SUNDRIES != pstPakMove->DstListType && 
				LIST_TYPE_STASH != pstPakMove->DstListType&&
				LIST_TYPE_ET_SUND!= pstPakMove->DstListType &&
				LIST_TYPE_ET_STASH!= pstPakMove->DstListType))
			{
				return -1;
			}
			iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, pstPakMove->SrcGridIdx);
			if (0 > iPos)
			{
				return -1;
			}

			pstSrcGrid = pstPak->SundriesPackage.RoleGrids+iPos;
			switch(pstSrcGrid->Type)
			{
				case GRID_ITEM_ARM:
					iSrcGridNum = pstSrcGrid->GridData.RoleArm.Num;
					break;
				case GRID_ITEM_ITEM:
					iSrcGridNum = pstSrcGrid->GridData.RoleItem.Num;
					break;
				default:
					return -1;
					break;
			}

			if (	losttotem_pkg_move_limit( pstEnv, pstPlayer, pstPakMove->SrcListType,
				pstPakMove->DstListType, &pstSrcGrid->GridData.RoleItem) )
			{
				return -1;
			}
			
			pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstSrcGrid->GridData.RoleItem);
			break;
		case LIST_TYPE_STASH:
			if (pstPakMove->SrcGridIdx >= pstPak->StashPackage.MaxNum ||
				(LIST_TYPE_DROP != pstPakMove->DstListType && LIST_TYPE_STASH != pstPakMove->DstListType && 
					LIST_TYPE_SUNDRIES != pstPakMove->DstListType && LIST_TYPE_STUFF != pstPakMove->DstListType&&
					LIST_TYPE_ET_STASH != pstPakMove->DstListType &&
					LIST_TYPE_ET_SUND!= pstPakMove->DstListType))
			{
				return -1;
			}
			if (0 > check_npc_ui(pstEnv, pstPlayer, UI_TYPE_STASH))
			{
				return -1;
			}
			iPos = package_grid_pos(pstPak->StashPackage.RoleGrids, pstPak->StashPackage.Num, pstPakMove->SrcGridIdx);
			if (0 > iPos)
			{
				return -1;
			}

			pstSrcGrid = pstPak->StashPackage.RoleGrids+iPos;
			switch(pstSrcGrid->Type)
			{
				case GRID_ITEM_ARM:
					iSrcGridNum = pstSrcGrid->GridData.RoleArm.Num;
					break;
				case GRID_ITEM_ITEM:
					iSrcGridNum = pstSrcGrid->GridData.RoleItem.Num;
					break;
				default:
					return -1;
					break;
			}
			
			pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstSrcGrid->GridData.RoleItem);
			break;
		case LIST_TYPE_SHOPPING:
			if (pstPakMove->SrcGridIdx >= pstPak->ShoppingPackage.MaxNum )
			{
				return -1;
			}
			iPos = package_grid_pos(pstPak->ShoppingPackage.RoleGrids, pstPak->ShoppingPackage.Num, pstPakMove->SrcGridIdx);
			if (0 > iPos)
			{
				return -1;
			}

			pstSrcGrid = pstPak->ShoppingPackage.RoleGrids+iPos;
			switch(pstSrcGrid->Type)
			{
				case GRID_ITEM_ARM:
					iSrcGridNum = pstSrcGrid->GridData.RoleArm.Num;
					break;
				case GRID_ITEM_ITEM:
					iSrcGridNum = pstSrcGrid->GridData.RoleItem.Num;
					break;
				default:
					return -1;
					break;
			}
			
			pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstSrcGrid->GridData.RoleItem);
			break;
		case LIST_TYPE_ET_STASH:
			if (	pstPakMove->DstListType != LIST_TYPE_ET_STASH &&
				pstPakMove->DstListType != LIST_TYPE_STASH &&
				pstPakMove->DstListType != LIST_TYPE_DROP &&
				pstPakMove->DstListType != LIST_TYPE_SUNDRIES)
			{
				return -1;
			}
			if (pstPakMove->SrcGridIdx >= pstPak->EtStashPkg.MaxNum)
			{
				return -1;
			}
			iPos = package_itemgrid_pos(pstPak->EtStashPkg.RoleItems, pstPak->EtStashPkg.Num,pstPakMove->SrcGridIdx);
			if (0>iPos)
			{
				return -1;
			}

			pstSrcGridItem = pstPak->EtStashPkg.RoleItems+iPos;
			iSrcGridNum = pstSrcGridItem->Num;
			if (iSrcGridNum != 1)
			{
				return -1;
			}

			if (pstSrcGridItem->ValidEnd != 0 && pstSrcGridItem->ValidEnd < pstEnv->pstAppCtx->stCurr.tv_sec &&
				pstPakMove->DstListType != LIST_TYPE_DROP)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK193);
				return -1;
			}
			
			pstItemDef = z_get_itemdef_roleitem(pstEnv, pstSrcGridItem);
			break;
		case LIST_TYPE_ET_SUND:
			if (	pstPakMove->DstListType != LIST_TYPE_ET_SUND &&
				pstPakMove->DstListType != LIST_TYPE_SUNDRIES &&
				pstPakMove->DstListType != LIST_TYPE_DROP &&
				pstPakMove->DstListType != LIST_TYPE_STASH)
			{
				return -1;
			}
			
			if (pstPakMove->SrcGridIdx >= pstPak->EtSundPkg.MaxNum)
			{
				return -1;
			}

			iPos = package_itemgrid_pos(pstPak->EtSundPkg.RoleItems, pstPak->EtSundPkg.Num,pstPakMove->SrcGridIdx);
			if (0>iPos)
			{
				return -1;
			}

			pstSrcGridItem = pstPak->EtSundPkg.RoleItems+iPos;
			iSrcGridNum = pstSrcGridItem->Num;
			if (iSrcGridNum != 1)
			{
				return -1;
			}

			if (pstSrcGridItem->ValidEnd != 0 && pstSrcGridItem->ValidEnd < pstEnv->pstAppCtx->stCurr.tv_sec &&
				pstPakMove->DstListType != LIST_TYPE_DROP)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK193);
				return -1;
			}
			
			pstItemDef = z_get_itemdef_roleitem(pstEnv, pstSrcGridItem);
			break;
		default:
			return -1;
			break;
	}

	if ((NULL == pstSrcGrid && NULL == pstSrcGridItem)|| 
		NULL == pstItemDef || 
		iSrcGridNum < pstPakMove->SrcGridNum || 
		(LIST_TYPE_DROP == pstPakMove->DstListType && pstItemDef->NoDrop))
	{
		return -1;
	}

	if(pstSrcGrid)
	{
		pstCommRoleItem = &pstSrcGrid->GridData.RoleItem;
	}
	else if(pstSrcGridItem)
	{
		pstCommRoleItem = pstSrcGridItem;
	}
	else
	{
		return -1;
	}

	iMaxPile = pstItemDef->MaxPile;
	if (iMaxPile <= 0)
	{
		iMaxPile = 1;
	}

	switch(pstPakMove->DstListType)
	{
		case LIST_TYPE_AMM:
			return -1;
			break;
		case LIST_TYPE_STUFF:
			return -1;
			break;
		case LIST_TYPE_TASK:
			if (pstItemDef->ItemType != ITEM_TYPE_TASK)
			{
				return -1;
			}
			
			if (pstPakMove->DstGridIdx >= pstPak->TaskPackage.MaxNum)
			{
				return -1;
			}
			
			iPos = package_itemgrid_pos(pstPak->TaskPackage.RoleItems, pstPak->TaskPackage.Num, pstPakMove->DstGridIdx);

			if (0 <= iPos)
			{
				pstDstGridItem = pstPak->TaskPackage.RoleItems+iPos;
			}

			
			if (pstDstGridItem && package_move_is_pile(pstEnv,pstSrcGrid,pstDstGrid) &&  iMaxPile> 1)
			
			{
				if (iMaxPile - pstDstGridItem->Num <= 0 )
				{
					return -1;
				}

				iMovePile = iMaxPile - pstDstGridItem->Num;
				if (pstPakMove->SrcGridNum < iMovePile)
				{
					iMovePile = pstPakMove->SrcGridNum;
				}
				
				pstDstGridItem->Num += iMovePile;
				if (package_move_is_bind(pstEnv,pstSrcGrid, pstDstGrid))
				{
					pstDstGrid->GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
				}
				
				pstCSRoleItem = pstRolePakChg->ItemChg.Items + pstRolePakChg->ItemChg.ChgNum;
				pstCSRoleItem->ListType = LIST_TYPE_TASK;
				memcpy(&pstCSRoleItem->Item, pstDstGridItem, sizeof(pstCSRoleItem->Item));
				pstRolePakChg->ItemChg.ChgNum++;
			}
			else
			{
				if (pstDstGridItem)
				{
					//拆分到有物品的格子
					if (iSrcGridNum != pstPakMove->SrcGridNum)
					{
						return -1;
					}

					stSwapGrid.Type = GRID_ITEM_ITEM;
					memcpy(&stSwapGrid.GridData.RoleItem, pstDstGridItem, sizeof(stSwapGrid.GridData.RoleItem));
					iSwapFlag = 1;
					if( LIST_TYPE_SHOPPING == pstPakMove->SrcListType)
					{
						z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_MIDDLE2,  ZONEERR_PAK4);
						return -1;
					}					
				}
				else
				{
					if (pstPak->TaskPackage.Num >= pstPak->TaskPackage.MaxNum)
					{
						return -1;
					}
					pstDstGridItem = pstPak->TaskPackage.RoleItems + pstPak->TaskPackage.Num;
				}	
				
				if (pstSrcGridItem)
				{
					memcpy(pstDstGridItem, pstSrcGridItem, sizeof(*pstDstGridItem));
				}
				else
				{
					if(pstSrcGrid)
					memcpy(pstDstGridItem, &pstSrcGrid->GridData.RoleItem, sizeof(*pstDstGridItem));
				}
				
				pstDstGridItem->Num = iMovePile;
				pstDstGridItem->GridIdx = pstPakMove->DstGridIdx;
				if (iSrcGridNum != pstPakMove->SrcGridNum)
				{
					pstDstGridItem->WID = z_get_gid(pstAppCtx, pstEnv);
				}

				if (iSwapFlag == 0)
				{
					pstPak->TaskPackage.Num++;
				}
				pstCSRoleItem = pstRolePakChg->ItemChg.Items + pstRolePakChg->ItemChg.ChgNum;
				pstCSRoleItem->ListType = LIST_TYPE_TASK;
				memcpy(&pstCSRoleItem->Item, pstDstGridItem, sizeof(pstCSRoleItem->Item));
				pstRolePakChg->ItemChg.ChgNum++;
			}
			break;
		case LIST_TYPE_SUNDRIES:
			
			if (pstItemDef->ItemType != ITEM_TYPE_ITEM && pstItemDef->ItemType != ITEM_TYPE_ARM && pstItemDef->ItemType != ITEM_TYPE_STUFF)
			{
				return -1;
			}
			
			iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, pstPakMove->DstGridIdx);
			if (0 <= iPos)
			{
				pstDstGrid = pstPak->SundriesPackage.RoleGrids+iPos;
			}

			if (	pstPakMove->SrcListType == LIST_TYPE_ET_SUND &&
				iMaxPile >1)
			{
				return -1;
			}

			//可叠加
			if ( pstDstGrid && 
				pstDstGrid->Type == GRID_ITEM_ITEM && 
				/*is_same_roleitem(&pstDstGrid->GridData.RoleItem, pstCommRoleItem)*/
				package_move_is_pile(pstEnv,pstSrcGrid,pstDstGrid)&& 
			
				iMaxPile > 1)
			{	
				if (iMaxPile -  pstDstGrid->GridData.RoleItem.Num <= 0 )
				{
					return -1;
				}
				
				iMovePile = iMaxPile - pstDstGrid->GridData.RoleItem.Num;
				if (pstPakMove->SrcGridNum < iMovePile)
				{
					iMovePile = pstPakMove->SrcGridNum;
				}
				
				pstDstGrid->GridData.RoleItem.Num += iMovePile;
				if (package_move_is_bind(pstEnv,pstSrcGrid, pstDstGrid))
				{
					pstDstGrid->GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
				}
				pstCSRoleItem = pstRolePakChg->ItemChg.Items + pstRolePakChg->ItemChg.ChgNum;
				pstCSRoleItem->ListType = LIST_TYPE_SUNDRIES;
				memcpy(&pstCSRoleItem->Item, &pstDstGrid->GridData.RoleItem, sizeof(pstCSRoleItem->Item));
				pstRolePakChg->ItemChg.ChgNum++;
			}
			else 
			{
				if (pstDstGrid)
				{
					//拆分到有物品的格子
					if (iSrcGridNum != pstPakMove->SrcGridNum)
					{
						return -1;
					}
					
					memcpy(&stSwapGrid, pstDstGrid, sizeof(stSwapGrid));
					iSwapFlag = 1;
					if( LIST_TYPE_SHOPPING == pstPakMove->SrcListType)
					{
						z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_PAK5);
						return -1;
					}

					if (pstPakMove->SrcListType == LIST_TYPE_ET_SUND)
					{
						if(pstSrcGridItem)
						{
							if (pkg_check_etsund_move(pstEnv, pstPak,pstSrcGridItem, 
														&pstDstGrid->GridData.RoleItem,&iListType, 1,pstPakMove->DstGridIdx,pstPlayer) < 0)
							{
								return -1;
							}
						}
						else 
						{
							if (pkg_check_etsund_move(pstEnv, pstPak,NULL, 
														&pstDstGrid->GridData.RoleItem,&iListType, 1,pstPakMove->DstGridIdx,pstPlayer) < 0)
							{
								return -1;
							}
						}
					}
					else if (pstPakMove->SrcListType == LIST_TYPE_ET_STASH)
					{
						if(pstSrcGridItem)
						{
							if (pkg_check_etstash2sund_move(pstEnv, pstPak,pstSrcGridItem, 
														&pstDstGrid->GridData.RoleItem,&iListType, 1,pstPakMove->DstGridIdx,pstPlayer) < 0)
							{
								return -1;
							}
						}
						else
						{
							if (pkg_check_etstash2sund_move(pstEnv, pstPak,NULL, 
														&pstDstGrid->GridData.RoleItem,&iListType, 1,pstPakMove->DstGridIdx,pstPlayer) < 0)
							{
								return -1;
							}
						}
					}
					else
					{
						if (gridIdx_is_unvaild(pstEnv, pstPak, pstPakMove->SrcListType, pstPakMove->SrcGridIdx) != 1)
						{		
							z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK194);
							return -1;
						}
					}
				}
				else
				{
					if (pstPakMove->SrcListType == LIST_TYPE_ET_SUND)
					{
						if(pstSrcGridItem)
						{
							if (pkg_check_etsund_move(pstEnv, pstPak,pstSrcGridItem, NULL,
															&iListType, 0,pstPakMove->DstGridIdx,pstPlayer) < 0)
							{
								return -1;
							}
						}
						else
						{
							if (pkg_check_etsund_move(pstEnv, pstPak,NULL, NULL,
															&iListType, 0,pstPakMove->DstGridIdx,pstPlayer) < 0)
							{
								return -1;
							}
						}
					}
					else if (pstPakMove->SrcListType == LIST_TYPE_ET_STASH)
					{
						if(pstSrcGridItem)
						{
							if (pkg_check_etstash2sund_move(pstEnv, pstPak,pstSrcGridItem, 
														NULL,&iListType, 0,pstPakMove->DstGridIdx,pstPlayer) < 0)
							{
								return -1;
							}
						}
						else
						{
							if (pkg_check_etstash2sund_move(pstEnv, pstPak,NULL, 
														NULL,&iListType, 0,pstPakMove->DstGridIdx,pstPlayer) < 0)
							{
								return -1;
							}
						}
					}
					
					
					if (pstPak->SundriesPackage.Num >= pstPak->SundriesPackage.MaxNum)
					{
						return -1;
					}
					pstDstGrid = pstPak->SundriesPackage.RoleGrids+pstPak->SundriesPackage.Num;
				}
				
				if (pstSrcGridItem)
				{
					pstDstGrid->Type = GRID_ITEM_ITEM;
					memcpy(&pstDstGrid->GridData.RoleItem, pstSrcGridItem, sizeof(pstDstGrid->GridData.RoleItem));
				}
				else
				{
					if (NULL == pstSrcGrid)
					{
						return -1;
					}
					memcpy(pstDstGrid, pstSrcGrid, sizeof(*pstDstGrid));
				}

				if (GRID_ITEM_ITEM == pstDstGrid->Type)
				{
					pstDstGrid->GridData.RoleItem.GridIdx = pstPakMove->DstGridIdx;
					pstDstGrid->GridData.RoleItem.Num = iMovePile;
					if (iSrcGridNum != pstPakMove->SrcGridNum)
					{
						pstDstGrid->GridData.RoleItem.WID = z_get_gid(pstAppCtx, pstEnv);
					}

					pstCSRoleItem = pstRolePakChg->ItemChg.Items + pstRolePakChg->ItemChg.ChgNum;
					pstCSRoleItem->ListType = LIST_TYPE_SUNDRIES;
					memcpy(&pstCSRoleItem->Item, &pstDstGrid->GridData.RoleItem, sizeof(pstCSRoleItem->Item));
					pstRolePakChg->ItemChg.ChgNum++;
				}
				else if (GRID_ITEM_ARM == pstDstGrid->Type)
				{
					pstDstGrid->GridData.RoleArm.GridIdx = pstPakMove->DstGridIdx;
					pstCSRoleArm = pstRolePakChg->ArmChg.Arms + pstRolePakChg->ArmChg.ChgNum;
					pstCSRoleArm->ListType = LIST_TYPE_SUNDRIES;
					memcpy(&pstCSRoleArm->Arm, &pstDstGrid->GridData.RoleArm, sizeof(pstCSRoleArm->Arm));
					pstRolePakChg->ArmChg.ChgNum++;
				}
				else
				{
					return -1;
				}

				if (iSwapFlag == 0)
				{
					pstPak->SundriesPackage.Num++;	
				}
			}
			break;
		case LIST_TYPE_STASH:
			
			if (0 > check_npc_ui(pstEnv, pstPlayer, UI_TYPE_STASH))
			{
				return -1;
			}

			iPos = package_grid_pos(pstPak->StashPackage.RoleGrids, pstPak->StashPackage.Num, pstPakMove->DstGridIdx);
			if (0 <= iPos)
			{
				pstDstGrid = pstPak->StashPackage.RoleGrids+iPos;
			}
			
			if ( pstDstGrid && 
				pstDstGrid->Type == GRID_ITEM_ITEM && 
				/*is_same_roleitem(&pstDstGrid->GridData.RoleItem, pstCommRoleItem)*/
				package_move_is_pile(pstEnv,pstSrcGrid,pstDstGrid)&& 
				iMaxPile > 1)
			{	
				if (iMaxPile - pstDstGrid->GridData.RoleItem.Num <= 0 )
				{
					return -1;
				}

				iMovePile = iMaxPile - pstDstGrid->GridData.RoleItem.Num;
				if (pstPakMove->SrcGridNum < iMovePile)
				{
					iMovePile = pstPakMove->SrcGridNum;
				}
				
				pstDstGrid->GridData.RoleItem.Num += iMovePile;

				if (package_move_is_bind(pstEnv,pstSrcGrid, pstDstGrid))
				{
					pstDstGrid->GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
				}
				
				pstCSRoleItem = pstRolePakChg->ItemChg.Items + pstRolePakChg->ItemChg.ChgNum;
				pstCSRoleItem->ListType = LIST_TYPE_STASH;
				memcpy(&pstCSRoleItem->Item, &pstDstGrid->GridData.RoleItem, sizeof(pstCSRoleItem->Item));
				pstRolePakChg->ItemChg.ChgNum++;
			}
			else 
			{
				if (pstDstGrid)
				{
					//拆分到有物品的格子
					if (iSrcGridNum != pstPakMove->SrcGridNum)
					{
						return -1;
					}

					pstDstItemDef = z_get_itemdef_roleitem(pstEnv, &pstDstGrid->GridData.RoleItem);
					if (!pstDstItemDef || 
						(pstDstItemDef->ItemType != pstItemDef->ItemType && pstPakMove->SrcListType != LIST_TYPE_STASH))
					{
						return -1;
					}
						

					//材料交换移动限制
					if( (pstItemDef->ItemType != ITEM_TYPE_ARM && pstItemDef->ItemType != ITEM_TYPE_ITEM) ||
						(pstDstItemDef->ItemType  != ITEM_TYPE_ARM && pstDstItemDef->ItemType != ITEM_TYPE_ITEM))
					{
						
						if (pstDstItemDef->ItemType != pstItemDef->ItemType && pstPakMove->SrcListType != LIST_TYPE_STASH)
						{
							return -1;
						}
					}
					memcpy(&stSwapGrid, pstDstGrid, sizeof(stSwapGrid));
					iSwapFlag = 1;
					if( LIST_TYPE_SHOPPING == pstPakMove->SrcListType)
					{
						z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_PAK6 );
						return -1;
					}	

					if (	pstPakMove->SrcListType == LIST_TYPE_ET_STASH)
					{
						if(pstSrcGridItem)
						{
							if (pkg_check_etstash_move(pstEnv, pstPak,pstSrcGridItem, 
									&pstDstGrid->GridData.RoleItem, &iListType, 1,pstPakMove->DstGridIdx,pstPlayer) < 0)
							{
								return -1;
							}
						}
						else
						{
							if (pkg_check_etstash_move(pstEnv, pstPak,NULL, 
									&pstDstGrid->GridData.RoleItem, &iListType, 1,pstPakMove->DstGridIdx,pstPlayer) < 0)
							{
								return -1;
							}
						}
					}
					else if (pstPakMove->SrcListType == LIST_TYPE_ET_SUND)
					{
						if(pstSrcGridItem)
						{
							if (pkg_check_etsund2stash_move(pstEnv, pstPak,pstSrcGridItem, &pstDstGrid->GridData.RoleItem,
												&iListType, 1,pstPakMove->DstGridIdx,pstPlayer) < 0)
							{
								return -1;
							}
						}
						else
						{
							if (pkg_check_etsund2stash_move(pstEnv, pstPak,NULL, &pstDstGrid->GridData.RoleItem,
												&iListType, 1,pstPakMove->DstGridIdx,pstPlayer) < 0)
							{
								return -1;
							}
						}
					}
					else
					{
						if (gridIdx_is_unvaild(pstEnv, pstPak, pstPakMove->SrcListType, pstPakMove->SrcGridIdx) != 1)
						{		
							z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK194);
							return -1;
						}
					}
				}
				else
				{
					if (pstPakMove->SrcListType == LIST_TYPE_ET_STASH )
					{
						if(pstSrcGridItem)
						{
							if (pkg_check_etstash_move(pstEnv, pstPak,pstSrcGridItem, 
												NULL, &iListType, 0,pstPakMove->DstGridIdx,pstPlayer) < 0)
							{
								return -1;
							}
						}
						else
						{
							if (pkg_check_etstash_move(pstEnv, pstPak,NULL, 
									NULL, &iListType, 0,pstPakMove->DstGridIdx,pstPlayer) < 0)
							{
								return -1;
							}

						}
					}
					else if (pstPakMove->SrcListType == LIST_TYPE_ET_SUND)
					{
						if(pstSrcGridItem)
						{
							if (pkg_check_etsund2stash_move(pstEnv, pstPak,pstSrcGridItem, NULL,
												&iListType, 0,pstPakMove->DstGridIdx,pstPlayer) < 0)
							{
								return -1;
							}
						}
						else
						{
							if (pkg_check_etsund2stash_move(pstEnv, pstPak,NULL, NULL,
												&iListType, 0,pstPakMove->DstGridIdx,pstPlayer) < 0)
							{
								return -1;
							}
						}
					}
					
					if (pstPak->StashPackage.Num >= pstPak->StashPackage.MaxNum)
					{
						return -1;
					}
					pstDstGrid = pstPak->StashPackage.RoleGrids+pstPak->StashPackage.Num;
				}
				
				if (pstSrcGridItem)
				{
					pstDstGrid->Type = GRID_ITEM_ITEM;
					memcpy(&pstDstGrid->GridData.RoleItem, pstSrcGridItem, sizeof(pstDstGrid->GridData.RoleItem));
				}
				else
				{
					if (NULL == pstSrcGrid)
					{
						return -1;
					}
					memcpy(pstDstGrid, pstSrcGrid, sizeof(*pstDstGrid));
				}
					
				if (GRID_ITEM_ITEM == pstDstGrid->Type)
				{
					pstDstGrid->GridData.RoleItem.GridIdx = pstPakMove->DstGridIdx;
					pstDstGrid->GridData.RoleItem.Num = iMovePile;
					if (iSrcGridNum != pstPakMove->SrcGridNum)
					{
						pstDstGrid->GridData.RoleItem.WID = z_get_gid(pstAppCtx, pstEnv);
					}

					pstCSRoleItem = pstRolePakChg->ItemChg.Items + pstRolePakChg->ItemChg.ChgNum;
					pstCSRoleItem->ListType = LIST_TYPE_STASH;
					memcpy(&pstCSRoleItem->Item, &pstDstGrid->GridData.RoleItem, sizeof(pstCSRoleItem->Item));
					pstRolePakChg->ItemChg.ChgNum++;
				}
				else if (GRID_ITEM_ARM == pstDstGrid->Type)
				{
					pstDstGrid->GridData.RoleArm.GridIdx = pstPakMove->DstGridIdx;
					pstCSRoleArm = pstRolePakChg->ArmChg.Arms + pstRolePakChg->ArmChg.ChgNum;
					pstCSRoleArm->ListType = LIST_TYPE_STASH;
					memcpy(&pstCSRoleArm->Arm, &pstDstGrid->GridData.RoleArm, sizeof(pstCSRoleArm->Arm));
					pstRolePakChg->ArmChg.ChgNum++;
				}
				else
				{
					return -1;
				}

				if (iSwapFlag == 0)
				{
					pstPak->StashPackage.Num++;	
				}
			}
			break;
		case LIST_TYPE_ET_STASH:
			pstItemResult = z_item_get_result(pstItemDef, RESULT_ADD_SUNDPKG_GRID);
			if (!pstItemResult)
			{
				return -1;
			}
			
			iPos = package_itemgrid_pos(pstPak->EtStashPkg.RoleItems, pstPak->EtStashPkg.Num, pstPakMove->DstGridIdx);
			if (0 <= iPos)
			{
				pstDstGridItem = pstPak->EtStashPkg.RoleItems+iPos;
			}

			if (pstDstGridItem)
			{
				stSwapGrid.Type = GRID_ITEM_ITEM;
				memcpy(&stSwapGrid.GridData.RoleItem, pstDstGridItem, sizeof(stSwapGrid.GridData.RoleItem));
				iSwapFlag = 1;

				if (pstSrcGrid && pstPakMove->SrcListType == LIST_TYPE_STASH )
				{
					if (pkg_check_etstash_move(pstEnv, pstPak,pstDstGridItem, 
											&pstSrcGrid->GridData.RoleItem, &iListType, 1,pstPakMove->DstGridIdx,pstPlayer) < 0)
					{
						return -1;
					}
				}
				else if (pstSrcGrid && pstPakMove->SrcListType == LIST_TYPE_SUNDRIES)
				{
					if (pkg_check_etstash2sund_move(pstEnv, pstPak,pstDstGridItem, 
									&pstSrcGrid->GridData.RoleItem,&iListType, 1,pstPakMove->DstGridIdx,pstPlayer) < 0)
					{
						return -1;
					}
				}
				else if (pstPakMove->SrcListType == LIST_TYPE_ET_STASH)
				{
					if (pstDstGridItem->ValidEnd != 0 && pstDstGridItem->ValidEnd < pstEnv->pstAppCtx->stCurr.tv_sec)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK193);
						return -1;
					}
				}
			}
			else
			{
				if (pstPak->EtStashPkg.Num >= pstPak->EtStashPkg.MaxNum)
				{
					return -1;
				}
				pstDstGridItem = pstPak->EtStashPkg.RoleItems + pstPak->EtStashPkg.Num;
				if (pstSrcGrid &&	pstPakMove->SrcListType == LIST_TYPE_STASH )
				{
					if (pkg_check_etstash_move(pstEnv, pstPak,NULL, 
									&pstSrcGrid->GridData.RoleItem, &iListType, 0,pstPakMove->DstGridIdx,pstPlayer) < 0)
					{
						return -1;
					}
				}
				else if( pstSrcGrid && pstPakMove->SrcListType == LIST_TYPE_SUNDRIES)
				{
					if (pkg_check_etstash2sund_move(pstEnv, pstPak,NULL, 
									&pstSrcGrid->GridData.RoleItem,&iListType, 0,pstPakMove->DstGridIdx,pstPlayer) < 0)
					{
						return -1;
					}
				}
			}

			if (pstSrcGridItem)
			{
				memcpy(pstDstGridItem, pstSrcGridItem, sizeof(*pstDstGridItem));
			}
			else
			{
				if(pstSrcGrid)
				memcpy(pstDstGridItem, &pstSrcGrid->GridData.RoleItem, sizeof(*pstDstGridItem));
			}
			
			pstDstGridItem->Num = 1;
			pstDstGridItem->GridIdx = pstPakMove->DstGridIdx;
			if (iSrcGridNum != pstPakMove->SrcGridNum)
			{
				return -1;
			}

			if (iSwapFlag == 0)
			{
				pstPak->EtStashPkg.Num++;
			}
			pstCSRoleItem = pstRolePakChg->ItemChg.Items + pstRolePakChg->ItemChg.ChgNum;
			pstCSRoleItem->ListType = LIST_TYPE_ET_STASH;
			memcpy(&pstCSRoleItem->Item, pstDstGridItem, sizeof(pstCSRoleItem->Item));
			pstRolePakChg->ItemChg.ChgNum++;
			break;

		case LIST_TYPE_ET_SUND:
			pstItemResult = z_item_get_result(pstItemDef, RESULT_ADD_SUNDPKG_GRID);
			if (!pstItemResult)
			{
				return -1;
			}
			
			iPos = package_itemgrid_pos(pstPak->EtSundPkg.RoleItems, pstPak->EtSundPkg.Num, pstPakMove->DstGridIdx);
			if (0 <= iPos)
			{
				pstDstGridItem = pstPak->EtSundPkg.RoleItems+iPos;
			}
			
			if (pstDstGridItem)
			{
				stSwapGrid.Type = GRID_ITEM_ITEM;
				memcpy(&stSwapGrid.GridData.RoleItem, pstDstGridItem, sizeof(stSwapGrid.GridData.RoleItem));
				iSwapFlag = 1;
				if (pstSrcGrid && pstPakMove->SrcListType == LIST_TYPE_SUNDRIES)
				{
					if (pkg_check_etsund_move(pstEnv, pstPak,pstDstGridItem, &pstSrcGrid->GridData.RoleItem,
											&iListType, 1,pstPakMove->DstGridIdx,pstPlayer) < 0)
					{
						return -1;
					}
				}
				else if (pstSrcGrid && pstPakMove->SrcListType == LIST_TYPE_STASH)
				{
					if (pkg_check_etsund2stash_move(pstEnv, pstPak,pstDstGridItem, &pstSrcGrid->GridData.RoleItem,
											&iListType, 1,pstPakMove->DstGridIdx,pstPlayer) < 0)
					{
						return -1;
					}
				}
				else if (pstPakMove->SrcListType == LIST_TYPE_ET_SUND)
				{
					if (pstDstGridItem->ValidEnd != 0 && pstDstGridItem->ValidEnd < pstEnv->pstAppCtx->stCurr.tv_sec)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK193);
						return -1;
					}
				}
			}
			else
			{
				if (pstPak->EtSundPkg.Num >= pstPak->EtSundPkg.MaxNum)
				{
					return -1;
				}
				pstDstGridItem = pstPak->EtSundPkg.RoleItems + pstPak->EtSundPkg.Num;
				if (pstSrcGrid && pstPakMove->SrcListType == LIST_TYPE_SUNDRIES )
				{
					if (pkg_check_etsund_move(pstEnv, pstPak,NULL, &pstSrcGrid->GridData.RoleItem,
											&iListType, 0,pstPakMove->DstGridIdx,pstPlayer) < 0)
					{
						return -1;
					}
				}
				else if (pstSrcGrid && pstPakMove->SrcListType == LIST_TYPE_STASH)
				{
					if (pkg_check_etsund2stash_move(pstEnv, pstPak,NULL, &pstSrcGrid->GridData.RoleItem,
											&iListType, 0,pstPakMove->DstGridIdx,pstPlayer) < 0)
					{
						return -1;
					}
				}
			}

			if (pstSrcGridItem)
			{
				memcpy(pstDstGridItem, pstSrcGridItem, sizeof(*pstDstGridItem));
			}
			else
			{
				if(pstSrcGrid)
				memcpy(pstDstGridItem, &pstSrcGrid->GridData.RoleItem, sizeof(*pstDstGridItem));
			}
			
			pstDstGridItem->Num = 1;
			pstDstGridItem->GridIdx = pstPakMove->DstGridIdx;
			if (iSrcGridNum != pstPakMove->SrcGridNum)
			{
				return -1;
			}

			if (iSwapFlag == 0)
			{
				pstPak->EtSundPkg.Num++;
			}
			pstCSRoleItem = pstRolePakChg->ItemChg.Items + pstRolePakChg->ItemChg.ChgNum;
			pstCSRoleItem->ListType = LIST_TYPE_ET_SUND;
			memcpy(&pstCSRoleItem->Item, pstDstGridItem, sizeof(pstCSRoleItem->Item));
			pstRolePakChg->ItemChg.ChgNum++;

			break;
		case LIST_TYPE_DROP:
			{
				ROLEGRID stGrid;
					
				// 丢弃扩充背包的东西  格子会改变
				if (pstPakMove->SrcListType == LIST_TYPE_ET_SUND)
				{
					if(pstSrcGridItem)
					{
						if (pkg_check_etsund_move(pstEnv, pstPak,pstSrcGridItem, NULL,
													&iListType, 0,-1,pstPlayer) < 0)
						{
							return -1;
						}
					}
					else
					{
						if (pkg_check_etsund_move(pstEnv, pstPak,NULL, NULL,
													&iListType, 0,-1,pstPlayer) < 0)
						{
							return -1;
						}
					}
				}
				else if (pstPakMove->SrcListType == LIST_TYPE_ET_STASH)
				{
					if(pstSrcGridItem)
					{
						if (pkg_check_etstash_move(pstEnv, pstPak,pstSrcGridItem, NULL,
													&iListType, 0,-1,pstPlayer) < 0)
						{
							return -1;
						}
					}
					else
					{
						if (pkg_check_etstash_move(pstEnv, pstPak,NULL, NULL,
													&iListType, 0,-1,pstPlayer) < 0)
						{
							return -1;
						}
					}
				}
				
				if(pstSrcGrid)
				{	
					stGrid = *pstSrcGrid;
				}
				else if(pstSrcGridItem)
				{
					stGrid.Type = GRID_ITEM_ITEM;
					stGrid.GridData.RoleItem = *pstSrcGridItem;
				}
				else
				{
					break;
				}

				if(pstPlayer->stOnline.stMachineInfo.ullLockItemWID > 0 && 
					pstPlayer->stOnline.stMachineInfo.ullLockItemWID == stGrid.GridData.RoleItem.WID)
				{
					return -1;
				}

				package_drop_msg(pstEnv, pstPlayer, &stGrid);

				if(pstItemDef->Important >= 3)
				{
					z_role_drop_record(pstEnv, pstPlayer, ROLE_DROP_GRID, &stGrid);
				}
				
				z_role_item_drop_oplog(pstEnv,pstPlayer,&stGrid);
			}
			break;
		default:
			return -1;
			break;
	}

	if (iSwapFlag)
	{
		if (pstSrcGrid && stSwapGrid.Type == GRID_ITEM_ARM)
		{
			pstSrcGrid->Type = GRID_ITEM_ARM;
			stSwapGrid.GridData.RoleArm.GridIdx = pstPakMove->SrcGridIdx;
			memcpy(&pstSrcGrid->GridData.RoleArm, &stSwapGrid.GridData.RoleArm, sizeof(pstSrcGrid->GridData.RoleArm));
			
			pstCSRoleArm = pstRolePakChg->ArmChg.Arms+ pstRolePakChg->ArmChg.ChgNum;
			pstCSRoleArm->ListType = pstPakMove->SrcListType;
			memcpy(&pstCSRoleArm->Arm, &pstSrcGrid->GridData.RoleArm, sizeof(pstCSRoleArm->Arm));
			pstRolePakChg->ArmChg.ChgNum++;
		}
		else if (stSwapGrid.Type == GRID_ITEM_ITEM)
		{
			
			stSwapGrid.GridData.RoleItem.GridIdx = pstPakMove->SrcGridIdx;
			if (pstSrcGrid)
			{
				pstSrcGrid->Type = GRID_ITEM_ITEM;
				memcpy(&pstSrcGrid->GridData.RoleItem, &stSwapGrid.GridData.RoleItem, sizeof(pstSrcGrid->GridData.RoleItem));
			}
			else
			{
				memcpy(pstSrcGridItem, &stSwapGrid.GridData.RoleItem, sizeof(*pstSrcGridItem));
			}		

			pstCSRoleItem = pstRolePakChg->ItemChg.Items + pstRolePakChg->ItemChg.ChgNum;
			pstCSRoleItem->ListType = pstPakMove->SrcListType;
			memcpy(&pstCSRoleItem->Item, &stSwapGrid.GridData.RoleItem, sizeof(pstCSRoleItem->Item));
			pstRolePakChg->ItemChg.ChgNum++;
		}
		else
		{
			return -1;
		}
		
		Z_CSHEAD_INIT(&pstPkgRes->Head, PACKAGE_INFO);
		z_cltmsg_send(pstEnv, pstPlayer, pstPkgRes);

		// 通知背包格子发生变化
		if (iListType != -2 )
		{
			pkg_num_chg(pstEnv, pstPlayer, iListType);
		}
		return 0;
	}
	
	if (pstSrcGridItem)
	{
		pstSrcGridItem->Num -= iMovePile;

		pstCSRoleItem = pstRolePakChg->ItemChg.Items + pstRolePakChg->ItemChg.ChgNum;
		pstCSRoleItem->ListType = pstPakMove->SrcListType;
		memcpy(&pstCSRoleItem->Item, pstSrcGridItem, sizeof(pstCSRoleItem->Item));
		pstRolePakChg->ItemChg.ChgNum++;
		
		if (0 == pstSrcGridItem->Num) //删除格子
		{
			switch(pstPakMove->SrcListType)
			{
				case LIST_TYPE_AMM:
					return -1;
					break;
				case LIST_TYPE_STUFF:
					return -1;
					break;
				case LIST_TYPE_TASK:
					pstPak->TaskPackage.Num--;
					if (pstSrcGridItem != (pstPak->TaskPackage.RoleItems+pstPak->TaskPackage.Num))
					{
						memcpy(pstSrcGridItem, pstPak->TaskPackage.RoleItems+pstPak->TaskPackage.Num, sizeof(*pstSrcGridItem));
					}
					break;
				case LIST_TYPE_ET_STASH:
					pstPak->EtStashPkg.Num--;
					if (pstSrcGridItem != (pstPak->EtStashPkg.RoleItems+pstPak->EtStashPkg.Num))
					{
						memcpy(pstSrcGridItem, pstPak->EtStashPkg.RoleItems+pstPak->EtStashPkg.Num, sizeof(*pstSrcGridItem));
					}
					break;

				case LIST_TYPE_ET_SUND:
					pstPak->EtSundPkg.Num--;
					if (pstSrcGridItem != (pstPak->EtSundPkg.RoleItems+pstPak->EtSundPkg.Num))
					{
						memcpy(pstSrcGridItem, pstPak->EtSundPkg.RoleItems+pstPak->EtSundPkg.Num, sizeof(*pstSrcGridItem));
					}
					break;
				default:
					return -1;
					break;
			}
		}
	}
	else
	{
		switch(pstPakMove->SrcListType)
		{
			case LIST_TYPE_SUNDRIES:
				if (pstSrcGrid && (GRID_ITEM_ARM ==  pstSrcGrid->Type) )
				{
					pstCSRoleArm = pstRolePakChg->ArmChg.Arms + pstRolePakChg->ArmChg.ChgNum;
					pstCSRoleArm->ListType = pstPakMove->SrcListType;
					memcpy(&pstCSRoleArm->Arm, &pstSrcGrid->GridData.RoleArm, sizeof(pstCSRoleArm->Arm));
					pstCSRoleArm->Arm.Num = 0;
					pstRolePakChg->ArmChg.ChgNum++;

					pstPak->SundriesPackage.Num--;
					if (pstSrcGrid != (pstPak->SundriesPackage.RoleGrids+pstPak->SundriesPackage.Num))
					{
						memcpy(pstSrcGrid, pstPak->SundriesPackage.RoleGrids+pstPak->SundriesPackage.Num, sizeof(*pstSrcGrid));
					}
				}
				else if(pstSrcGrid)
				{
					pstSrcGridItem = &pstSrcGrid->GridData.RoleItem;
					pstSrcGridItem->Num -= iMovePile;

					pstCSRoleItem = pstRolePakChg->ItemChg.Items + pstRolePakChg->ItemChg.ChgNum;
					pstCSRoleItem->ListType = pstPakMove->SrcListType;
					memcpy(&pstCSRoleItem->Item, pstSrcGridItem, sizeof(pstCSRoleItem->Item));
					pstRolePakChg->ItemChg.ChgNum++;

					if (0 == pstSrcGridItem->Num)
					{
						pstPak->SundriesPackage.Num--;
						if (pstSrcGrid != (pstPak->SundriesPackage.RoleGrids+pstPak->SundriesPackage.Num))
						{
							memcpy(pstSrcGrid, pstPak->SundriesPackage.RoleGrids+pstPak->SundriesPackage.Num, sizeof(*pstSrcGrid));
						}
					}
				}
				
				break;
			case LIST_TYPE_SHOPPING:
				if (pstSrcGrid && (GRID_ITEM_ARM ==  pstSrcGrid->Type) )
				{
					pstCSRoleArm = pstRolePakChg->ArmChg.Arms + pstRolePakChg->ArmChg.ChgNum;
					pstCSRoleArm->ListType = pstPakMove->SrcListType;
					memcpy(&pstCSRoleArm->Arm, &pstSrcGrid->GridData.RoleArm, sizeof(pstCSRoleArm->Arm));
					pstCSRoleArm->Arm.Num = 0;
					pstRolePakChg->ArmChg.ChgNum++;

					pstPak->ShoppingPackage.Num--;
					if (pstSrcGrid != (pstPak->ShoppingPackage.RoleGrids+pstPak->ShoppingPackage.Num))
					{
						memcpy(pstSrcGrid, pstPak->ShoppingPackage.RoleGrids+pstPak->ShoppingPackage.Num, sizeof(*pstSrcGrid));
					}
				}
				else if(pstSrcGrid)
				{
					pstSrcGridItem = &pstSrcGrid->GridData.RoleItem;
					pstSrcGridItem->Num -= iMovePile;

					pstCSRoleItem = pstRolePakChg->ItemChg.Items + pstRolePakChg->ItemChg.ChgNum;
					pstCSRoleItem->ListType = pstPakMove->SrcListType;
					memcpy(&pstCSRoleItem->Item, pstSrcGridItem, sizeof(pstCSRoleItem->Item));
					pstRolePakChg->ItemChg.ChgNum++;

					if (0 == pstSrcGridItem->Num)
					{
						pstPak->ShoppingPackage.Num--;
						if (pstSrcGrid != (pstPak->ShoppingPackage.RoleGrids+pstPak->ShoppingPackage.Num))
						{
							memcpy(pstSrcGrid, pstPak->ShoppingPackage.RoleGrids+pstPak->ShoppingPackage.Num, sizeof(*pstSrcGrid));
						}
					}
				}
				break;
			case LIST_TYPE_STASH:
				if (pstSrcGrid && (GRID_ITEM_ARM ==  pstSrcGrid->Type) )
				{
					pstCSRoleArm = pstRolePakChg->ArmChg.Arms + pstRolePakChg->ArmChg.ChgNum;
					pstCSRoleArm->ListType = pstPakMove->SrcListType;
					memcpy(&pstCSRoleArm->Arm, &pstSrcGrid->GridData.RoleArm, sizeof(pstCSRoleArm->Arm));
					pstCSRoleArm->Arm.Num = 0;
					pstRolePakChg->ArmChg.ChgNum++;

					pstPak->StashPackage.Num--;
					if (pstSrcGrid != (pstPak->StashPackage.RoleGrids+pstPak->StashPackage.Num))
					{
						memcpy(pstSrcGrid, pstPak->StashPackage.RoleGrids+pstPak->StashPackage.Num, sizeof(*pstSrcGrid));
					}
				}
				else if(pstSrcGrid)
				{
					pstSrcGridItem = &pstSrcGrid->GridData.RoleItem;
					pstSrcGridItem->Num -= iMovePile;

					pstCSRoleItem = pstRolePakChg->ItemChg.Items + pstRolePakChg->ItemChg.ChgNum;
					pstCSRoleItem->ListType = pstPakMove->SrcListType;
					memcpy(&pstCSRoleItem->Item, pstSrcGridItem, sizeof(pstCSRoleItem->Item));
					pstRolePakChg->ItemChg.ChgNum++;

					if (0 == pstSrcGridItem->Num)
					{
						pstPak->StashPackage.Num--;
						if (pstSrcGrid != (pstPak->StashPackage.RoleGrids+pstPak->StashPackage.Num))
						{
							memcpy(pstSrcGrid, pstPak->StashPackage.RoleGrids+pstPak->StashPackage.Num, sizeof(*pstSrcGrid));
						}
					}
				}
				break;

			default:
				return -1;
				break;
		}
	}

	Z_CSHEAD_INIT(&pstPkgRes->Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkgRes);

	// 通知背包格子发生变化
	if (iListType != -2)
	{
		pkg_num_chg(pstEnv, pstPlayer, iListType);
	}
	return 0;
}

int item_create_result_treasure_pic(ZONESVRENV* pstEnv, ITEMDEF *pstItemDef, ROLEITEM *pstRoleItem)
{
	int i = 0;
	int iCount = 0;
	MapIndex *pstMapIdx = NULL;
	int iMapPos = 0;
	RESPOS stPos;  
	RESPOS stNextPos;
	int iMaxDist = 0;
	int iMinDist = 0;
	ZONEMAPINST *pstMapInst;
	ZONEIDX stIdx;
	
	for (i=0; i<MAX_ITEM_RESULT; i++)
	{
		if (0 == pstItemDef->Result[i].ResultID)
		{
			break;
		}
		else if(RESULT_TREASURE_PIC == pstItemDef->Result[i].ResultID)
		{
			iCount ++;
		}
	}

	if(iCount <=0 || iCount > MAX_ITEM_RESULT)
	{
		return -1;
	}
	else
	{
		i = (int)(RAND1(iCount));
	}
	// 把随机地图坐标给取出来哈
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &iMapPos, pstItemDef->Result[i].ResultVal1);
	if (NULL == pstMapIdx)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,  "item_create_result itemid:%u,pstMapIdx NULL",
			pstItemDef->ItemID);
		return -1;
	}
		
	stPos.X = pstItemDef->Result[i].ResultVal2;
	stPos.Y = pstItemDef->Result[i].ResultVal3;
	iMaxDist = pstItemDef->Result[i].ResultVal4;
	
	stIdx.iID = pstMapIdx->stMapDef.MapID;
	stIdx.iPos	=	-1;	
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (0 > z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &stPos, &stPos, iMaxDist, iMinDist, &stNextPos))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,  "item_create_result fail itemid:%u,ResultVal1:%u,ResultVal2:%u,ResultVal3:%u,ResultVal4:%u",
			pstItemDef->ItemID,
			pstItemDef->Result[i].ResultVal1,
			pstItemDef->Result[i].ResultVal2,
			pstItemDef->Result[i].ResultVal3,
			pstItemDef->Result[i].ResultVal4);
	}
	pstRoleItem->Flag = pstItemDef->Result[i].ResultVal1;
	pstRoleItem->InstVal2 = stNextPos.X;
	pstRoleItem->InstVal3 = stNextPos.Y;
	return 0;
}
int item_create_result(ZONESVRENV* pstEnv, ITEMDEF *pstItemDef, ROLEITEM *pstRoleItem)
{

	int iRet = 0;
	
	if( RESULT_TREASURE_PIC == pstItemDef->Result[0].ResultID )
	{
		iRet = item_create_result_treasure_pic(pstEnv,pstItemDef,pstRoleItem);
	}
	
	return iRet;
}

int z_create_arm_fixedaddattr(ZONESVRENV* pstEnv, const FIXEDADDATTR *pstAttr,
                              ITEMDEF *pstDef, ROLEARM *pstArm)
{
	int i = 0;
	ARMADDATTRDEF *pstArmAddAttr;

	for(i = 0; i < (int)CCH(pstAttr->AddAttrs); i++)	
	{
		//没位置了
		if(pstArm->AddAttrNum >= MAX_ARM_ADD_ATTR)
		{
			break;
		}
		
		if(!pstAttr->AddAttrs[i].ID)
		{
			break;
		}

		pstArmAddAttr = z_find_armaddattrdef(pstEnv, pstAttr->AddAttrs[i].ID);
		if (!pstArmAddAttr)
		{
			continue;
		}

		if(!pstAttr->AddAttrs[i].Idx ||
		   pstAttr->AddAttrs[i].Idx > MAX_ARM_ADD_ATTR_RESULT)
		{
			continue;
		}

		z_add_arm_addattr(pstDef, pstArm, pstAttr->AddAttrs[i].ID, 
						  pstAttr->AddAttrs[i].Idx,
						  &pstArmAddAttr->Result[pstAttr->AddAttrs[i].Idx - 1]);
	}

	return 0;
}

int task_award_item_create(ZONESVRENV *pstEnv,
                           const TASKITEMAWARD *pstAwardItem,
                           ROLEGRID *pstRoleGrid)
{
	ROLEARM *pstArm = NULL;
	ITEMDEF *pstDef = z_find_itemdef(pstEnv, pstAwardItem->ID);
	if(!pstDef)
	{
		return -1;
	}

	memset(pstRoleGrid, 0, sizeof(*pstRoleGrid));
	if(pstDef->ItemType != ITEM_TYPE_ARM)
	{
		if(0 > item_create(pstEnv->pstAppCtx, pstEnv, pstRoleGrid,
		                   pstAwardItem->ID))
		{
			return -1;
		}
		
		// 处理装备上的孔和绑定的情况
		if(pstAwardItem->BindingAndSlot & 1)
		{
			pstRoleGrid->GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
		}
		
		return 0;
	}

	pstRoleGrid->Type = GRID_ITEM_ARM;
	pstArm = &pstRoleGrid->GridData.RoleArm;

	pstArm->DefID = pstAwardItem->ID;
	pstArm->DefPos = (long int)((char *)pstDef - 
	                  (char *)pstEnv->pstZoneObj->sItemDef) / ITEM_DEF_DATA;
	pstArm->Num = 1;
	pstArm->WID = z_get_gid(pstEnv->pstAppCtx, pstEnv);
	if(pstDef->ValidTime > 0)
	{
		pstArm->ValidEnd = pstEnv->pstAppCtx->stCurr.tv_sec +
		                   pstDef->ValidTime * 3600;
	}

	// 计算耐久度上限
	pstArm->Dur = z_get_arm_max_dur(pstEnv, pstDef) * DUR_UNIT; 
	
	pstArm->StarLv = pstDef->ItemStar;
	pstArm->FightLv = 0;

	// 处理装备上的孔和绑定的情况
	if(pstAwardItem->BindingAndSlot & 1)
	{
		pstArm->InstFlag |= INST_ITEM_BIND;
	}
	if(pstDef->ArmBind == ITEM_GET_BIND)
	{
		pstArm->InstFlag |= INST_ITEM_BIND;
	}
	if(pstAwardItem->BindingAndSlot & 2)
	{
		pstArm->HoleGem[pstArm->HoleNum++].HoleColor =
			(int )RAND1(GEM_COLOR_COLOR + 1);
	}
	else
	{
		pstArm->HoleNum = 0;
		if (pstDef->WearPre.RoleLevelMin > HOLE_MIN_LEVEL)
		{
			arm_add_hole(pstEnv,&pstDef->Hole[0], pstArm);
		}
	}
	if(pstArm->HoleNum > MAX_ARM_HOLE)
	{
		pstArm->HoleNum = MAX_ARM_HOLE;
	}

	item_add_attr_create(pstEnv, pstArm, pstDef);

	//基础数值随机的部分
	arm_base_attr_create(pstEnv, pstDef, pstArm);
	return 0;
}

int spar_item_create(ZONESVRENV* pstEnv, ROLEARM*pstArm, ROLEITEM *pstRoleItem)
{
	int iRand;
	ITEMDEF *pstItemDef;
	int iVal;
	double d;
	NEWADDATTRDEF* pstNewAddAttrDef = NULL;
	int i;

	pstItemDef = z_find_itemdef(pstEnv, pstRoleItem->DefID);
	if (!pstItemDef )
	{
		return -1;
	}

	if (!z_item_get_result(pstItemDef, RESULT_SPAR))
	{
		return 0;
	}

	if (pstArm->AddAttrNum <= 0)
	{
		return -1;
	}

	iRand = RAND1(pstArm->AddAttrNum);

	if (iRand >= pstArm->AddAttrNum)
	{
		return -1;
	}

	pstItemDef = z_find_itemdef(pstEnv, pstArm->DefID);
	if (!pstItemDef)
		return -1;

	pstNewAddAttrDef = z_find_new_addattr_def(pstEnv, pstArm->AddAttr[iRand].AddAttrID);
	if(!pstNewAddAttrDef )
	{
		return -1;
	}

	if (pstNewAddAttrDef->NotSpar )
	{
		for (i=0; i<pstArm->AddAttrNum; i++)
		{
			pstNewAddAttrDef = z_find_new_addattr_def(pstEnv, pstArm->AddAttr[i].AddAttrID);
			if(pstNewAddAttrDef && !pstNewAddAttrDef->NotSpar )
			{
				iRand = i;
				break;
			}	
		}

		// 是在找不到就算了
		if( i >=  pstArm->AddAttrNum)
		{
			return -1;
		}
	}

	if (	pstNewAddAttrDef->AddAttrResultID == ADD_ATTR_RESULT_HEAVY_HIT_VAL ||
		pstNewAddAttrDef->AddAttrResultID == ADD_ATTR_RESULT_HEAL_HP)
	{
		d = arm_addattr_result_addval_get_double(pstEnv, pstItemDef, pstArm->AddAttr[iRand].AddAttrID, 
											pstArm->AddAttr[iRand].Val);
		iVal = d+0.99;
	}
	else
	{
		iVal = arm_addattr_result_addval_get(pstEnv, pstItemDef, pstArm->AddAttr[iRand].AddAttrID, 
									pstArm->AddAttr[iRand].Val);		// 基础值
	}

	if (iVal <= 0) 
		iVal = 1;
	
	
	pstRoleItem->Flag = pstArm->AddAttr[iRand].AddAttrID;	// 关键字ID
	pstRoleItem->InstVal1= 1;								// 等级
	pstRoleItem->InstVal2 = iVal;						
	pstRoleItem->InstVal3 = 0;							// 经验
	return 0;
}

int item_create(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ROLEGRID *pstRoleGrid, int iIndex)
{
	ITEMDEF *pstItemDef = NULL;
	ROLEITEM *pstRoleItem = NULL;
	ROLEARM *pstRoleArm = NULL;
//	int i;

	pstItemDef = z_find_itemdef(pstEnv, iIndex);
	if (NULL == pstItemDef)
	{
		return -1;
	}

	memset(pstRoleGrid, 0, sizeof(*pstRoleGrid));
	
	if (ITEM_TYPE_ARM == pstItemDef->ItemType)
	{
		pstRoleGrid->Type = GRID_ITEM_ARM;
		pstRoleArm = &pstRoleGrid->GridData.RoleArm;

		pstRoleArm->DefID = iIndex;
		pstRoleArm->DefPos = (long int)((char *)pstItemDef - (char *)pstEnv->pstZoneObj->sItemDef)/ITEM_DEF_DATA;
		pstRoleArm->Num = 1;
		pstRoleArm->WID = z_get_gid(pstAppCtx, pstEnv);
		if (pstItemDef->ValidTime > 0)
		{
			pstRoleArm->ValidEnd = pstAppCtx->stCurr.tv_sec + pstItemDef->ValidTime*3600;
		}

		// 计算耐久度上限
		pstRoleArm->Dur = z_get_arm_max_dur(pstEnv, pstItemDef)*DUR_UNIT; 

		if(pstItemDef->ItemStar > 0)
		{
			int i, iMul, iRand, iRate, iStarLv = 0;
			ITEMSTARRANDDEF *pstItemStarRandDef;
			
			pstItemStarRandDef = z_find_item_star_rand_def(pstEnv, pstItemDef->ItemStar);
			if(pstItemStarRandDef)
			{
				iRate = 0;
				iRand = RAND1(10000);
				for(i=0; i<MAX_ARM_STAR_LEVEL; i++)
				{
					iRate += pstItemStarRandDef->StarLvRate[i];
					if(iRand < iRate)
					{
						iStarLv = i+1;
						break;
					}
				}
			}
			
			pstRoleArm->StarLv = iStarLv;
			for(i = 0; i<pstRoleArm->StarLv; i++)
			{	
				iMul = arm_star_mul_idx_get(pstEnv, pstRoleArm, i+1);
				if(0 > iMul)
				{
					iMul = 0;
				}
				
				pstRoleArm->StarInfo[i].ValIdx = iMul;
			}
		}
		
		pstRoleArm->FightLv = 0;

		// 计算第一个孔
		pstRoleArm->HoleNum = 0;
		if (pstItemDef->WearPre.RoleLevelMin > HOLE_MIN_LEVEL)
		{
			arm_add_hole(pstEnv,&pstItemDef->Hole[0],pstRoleArm);
		}
		
		if (pstRoleArm->HoleNum > MAX_ARM_HOLE)
		{
			pstRoleArm->HoleNum = MAX_ARM_HOLE;
		}

		if (pstItemDef->ArmBind == ITEM_GET_BIND)
		{
			pstRoleArm->InstFlag |= INST_ITEM_BIND;
		}

		/*
		//获得关键字
		z_create_arm_addattr(pstEnv, pstItemDef, pstRoleArm);
		
		if(pstItemDef->RandAddAttr)
		{
			z_rand_create_addattr(pstEnv, pstItemDef, pstRoleArm);
		}
		*/

		item_add_attr_create(pstEnv, pstRoleArm, pstItemDef);

		//基础数值随机的部分
		arm_base_attr_create(pstEnv, pstItemDef, pstRoleArm);

		// 装备技能数
		arm_create_skill_num(pstEnv,pstRoleArm,pstItemDef);
	}
	else
	{
		RESULT *pstResult;
		
		pstRoleGrid->Type = GRID_ITEM_ITEM;
		pstRoleItem = &pstRoleGrid->GridData.RoleItem;

		pstRoleItem->DefID = iIndex;
		pstRoleItem->DefPos = (long int)((char *)pstItemDef - (char *)pstEnv->pstZoneObj->sItemDef)/ITEM_DEF_DATA;
		pstRoleItem->Num = 1;
		pstRoleItem->WID = z_get_gid(pstAppCtx, pstEnv);

		//有使用次数的道具
		if(pstItemDef->UseMaxDur)
		{
			pstRoleItem->InstVal1 = pstItemDef->UseMaxDur;
		}
		
		if (pstItemDef->ValidTime > 0)
		{
			pstRoleItem->ValidEnd = pstAppCtx->stCurr.tv_sec + pstItemDef->ValidTime*3600;
		}

		if (pstItemDef->ArmBind == ITEM_GET_BIND)
		{
			pstRoleItem->InstFlag |= INST_ITEM_BIND;
		}
		// 宝藏生成的时候这个要用下
		if( 0 != item_create_result( pstEnv, pstItemDef, pstRoleItem) )
		{
			return -1;
		}

		pstResult = z_item_get_result(pstItemDef, RESULT_MACHINE_ITEM);
		if(pstResult)
		{
			machine_item_create(pstEnv, pstResult, pstRoleItem);
		}

		pstResult = z_item_get_result(pstItemDef, RESULT_LOST_TOTEM);
		if(pstResult)
		{
			pstRoleItem->InstVal1 = pstResult->ResultVal2; 	// 经验上限
			pstRoleItem->InstVal2 = 0;						// 当前经验
			pstRoleItem->InstFlag |= INST_ITEM_BIND;		
		}
	}

	return 0;
}

int find_grid_of_package(ROLEGRID*pstPackageRoleGrid, int iNum,ITEMDEF *pstItemDef, int fBind, int iGemStat)
{
	int i = 0;
	ROLEGRID *pstGrid = NULL;
	for(i=0; i<iNum; i++)
	{
		pstGrid = pstPackageRoleGrid+i;
		if(fBind != is_bind_roleitem(&pstGrid->GridData.RoleItem))
		{
			continue;
		}

		// 不是宝石
		if (iGemStat == 0)
		{
			if ((unsigned int)pstGrid->GridData.RoleItem.DefID == pstItemDef->ItemID)
			{
				return i;
			}
		}
		else
		{
			if ((unsigned int)pstGrid->GridData.RoleItem.DefID == pstItemDef->ItemID)
			{
				if (	iGemStat == GEM_STAT_NO_POWER && 
					gem_is_chongneng(pstItemDef, &pstGrid->GridData.RoleItem) == GEM_STAT_NO_POWER)
				{
					return i;
				}
			}
		}
		
		/*
		if(pstGrid->Type == GRID_ITEM_ITEM && (unsigned int)pstGrid->GridData.RoleItem.DefID == pstItemDef->ItemID)
		{
			return i;
		}
		else if(pstGrid->Type == GRID_ITEM_ARM && (unsigned int)pstGrid->GridData.RoleArm.DefID == pstItemDef->ItemID)
		{
			return i;
		}
		else
		{
		
		}*/	
	}
	return -1;
}

int find_item_of_package(ROLEITEM *pstItem, int iNum,ITEMDEF *pstItemDef, int fBind)
{
	int i;	
	ROLEITEM *pstRoleItem;
	for(i=0; i<iNum; i++)
	{
		pstRoleItem = pstItem +i;
		if( (unsigned int)pstRoleItem->DefID == pstItemDef->ItemID &&
			fBind == is_bind_roleitem(pstRoleItem))
		{
			return i;
		}
	}
	return -1;
}

int package_grid_cmp(const void *temp1, const void *temp2)
{
	int iItemID1,iItemID2;
	if(  ((ROLEGRID *)temp1)->Type == GRID_ITEM_ARM)
	{
		iItemID1 = ((ROLEGRID *)temp1)->GridData.RoleArm.DefID;
	}
	else
	{
		iItemID1 = ((ROLEGRID *)temp1)->GridData.RoleItem.DefID;
	}

	if(  ((ROLEGRID *)temp2)->Type == GRID_ITEM_ARM)
	{
		iItemID2 = ((ROLEGRID *)temp2)->GridData.RoleArm.DefID;
	}
	else
	{
		iItemID2 = ((ROLEGRID *)temp2)->GridData.RoleItem.DefID;
	}

	return (iItemID1 - iItemID2);
}

int package_item_cmp(const void *temp1, const void *temp2)
{
	return ((ROLEITEM*)temp1)->DefID - ((ROLEITEM*)temp2)->DefID;
}

int package_item_arrange(ZONESVRENV* pstEnv,ROLEITEM *pstRoleItem, unsigned short *punNum, 
				unsigned short unMax, ROLEPACKAGE *pstPak,char cListType)
{
	int i = 0;
	int iMaxPile = 0;
	int iItemNum = 0;
	int iTotal = 0;
	ROLEITEM *pstItem = NULL;
	ROLEITEM astItem[MAX_SUNDRIES_PACKAGE];
	ITEMDEF *pstItemDef = NULL;
	int iUnVaildNum = 0;
	ROLEITEM astUnVaildItem[MAX_SUNDRIES_PACKAGE];
	int iRet;
	int fBind;
	ROLEITEM stItem;

	if(*punNum < 1 || *punNum > sizeof(astItem)/sizeof(astItem[0]))
	{
		return -1;
	}
	
	for( i =0; i<*punNum; i++)
	{
		pstItem = pstRoleItem + i;
		iRet = gridIdx_is_unvaild(pstEnv,pstPak,cListType,pstItem->GridIdx);
		if (iRet < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "arrange err");
			return -1;
		}
		// 失效的
		else if(iRet == 0)
		{
			memcpy(&astUnVaildItem[iUnVaildNum],pstItem,sizeof(*pstItem));
			iUnVaildNum ++;
			continue;
		}

		
		pstItemDef = z_get_itemdef_roleitem(pstEnv, pstItem);
		if(pstItemDef == NULL)
		{
			return -1;
		}

		iMaxPile = pstItemDef->MaxPile;
		if(iMaxPile ==0)
		{
			iMaxPile = 1;
		}

		if (1 == iMaxPile)
		{
			if (iTotal >= unMax)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "arrange err");
				return -1;
			}
			
			memcpy(&astItem[iTotal],pstItem,sizeof(*pstItem));
			iTotal ++;

			continue;
		}

		//物品实例是否绑定
		fBind = is_bind_roleitem(pstItem);

		stItem = *pstItem;

		if(-1 == find_item_of_package(astItem,iTotal,pstItemDef, fBind))
		{
			iItemNum = z_get_pak_item_num_by_bindtype(pstEnv, pstPak, 
							pstItemDef->ItemID, cListType, 1, fBind);
			while(iItemNum > 0)
			{
				if (iTotal >= unMax)
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "arrange err");
					return -1;
				}
				
				if(iItemNum > iMaxPile)
				{
					stItem.Num = iMaxPile;
				}
				else
				{
					stItem.Num = iItemNum;
				}
				
				iItemNum -= iMaxPile;

				stItem.WID = z_get_gid(pstEnv->pstAppCtx, pstEnv);
				memcpy(&astItem[iTotal],&stItem,sizeof(stItem));

				iTotal ++;
			}
		}
	}
	if(iTotal > 1)
	{
		qsort(astItem, iTotal, sizeof(astItem[0]),package_item_cmp);
	}

	for(i=0; i<iTotal; i++)
	{
		astItem[i].GridIdx = i;
	}

	if (iTotal + iUnVaildNum > unMax)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "arrange err");
		return -1;
	}

	for (i=0; i<iUnVaildNum; i++)
	{
		if (astUnVaildItem[i].GridIdx < iTotal)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "arrange err");
			return -1;
		}
	}
	
	*punNum = iTotal+iUnVaildNum;
	if (iTotal > 0)
		memcpy(pstRoleItem,astItem,sizeof(astItem[0])*iTotal);
	if (iUnVaildNum > 0)
		memcpy(pstRoleItem+iTotal,astUnVaildItem,sizeof(astUnVaildItem[0])*iUnVaildNum);
	return 0;
}


// 检查某个grididx在某个背包是否失效
int gridIdx_is_unvaild(ZONESVRENV* pstEnv,ROLEPACKAGE *pstPak,char cListType,int iGridIdx)
{
//	int iMaxNum;
	int iValidGridNum = get_vaild_gird(pstEnv, pstPak,cListType);

	if (iValidGridNum < 0)
	{
		return -1;
	}
	
	if (iGridIdx < iValidGridNum)
	{
		return 1;
	}
	return 0;
}


static int pkg_arrange_get_bind_num(ROLEGRID *pastGrid, int iTotal, int iBind, ITEMDEF *pstItemDef)
{
	int iNum = 0;
	int i;
	ROLEGRID *pstGrid;

	for (i=0; i<iTotal; i++)
	{
		pstGrid = &pastGrid[i];
		if(iBind != is_bind_roleitem(&pstGrid->GridData.RoleItem))
		{
			continue;
		}

		if (gem_is_chongneng( pstItemDef, &pstGrid->GridData.RoleItem)== GEM_STAT_POWER)
		{
			continue;
		}

		if (pstGrid->GridData.RoleItem.DefID == (int)pstItemDef->ItemID)
		{
			iNum += pstGrid->GridData.RoleItem.Num;
		}
	}

	return iNum;
}

int package_grid_arrange(ZONESVRENV* pstEnv,ROLEGRID *pstRoleGrid, unsigned short *punNum, 
			unsigned short unMax, ROLEPACKAGE *pstPak,char cListType)
{
	int i = 0;
	int iMaxPile = 0;
	int iGridNum = 0;
	int iTotal = 0;
	ROLEGRID *pstGrid = NULL;
	ROLEGRID astGrid[MAX_SUNDRIES_PACKAGE];
	ITEMDEF *pstItemDef = NULL;
	// 记录失效的Grid;
	int iUnVaildNum = 0;
	ROLEGRID astUnVaildGrid[MAX_SUNDRIES_PACKAGE];
	int iRet;
	int iStuffNum = 0;
	ROLEGRID astStuffGrid[MAX_SUNDRIES_PACKAGE];
	int fBind;
	int iPreNum;
	int iLastNum;
	ROLEGRID stGrid;
	int iGemStat;

	if(*punNum < 1 || *punNum > sizeof(astGrid)/sizeof(astGrid[0]))
	{
		return -1;
	}
	
	for( i =0; i<*punNum; i++)
	{
		pstGrid = pstRoleGrid + i;
		iRet = gridIdx_is_unvaild(pstEnv,pstPak,cListType,pstGrid->GridData.RoleItem.GridIdx);
		if (iRet < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "arrange err");
			return -1;
		}
		//  失效
		else if(iRet == 0)
		{
			memcpy(&astUnVaildGrid[iUnVaildNum],pstGrid,sizeof(*pstGrid));
			iUnVaildNum ++;
			continue;
		}
		// 有效

		
		if(pstGrid->Type == GRID_ITEM_ARM)
		{
			if (iTotal >= unMax)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "arrange err");
				return -1;
			}
			
			memcpy(&astGrid[iTotal],pstGrid,sizeof(*pstGrid));
			iTotal ++;

			continue;
		}
		
		pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstGrid->GridData.RoleItem);
		if(pstItemDef ==NULL)
		{
			return -1;
		}

		iMaxPile = pstItemDef->MaxPile;
		if(iMaxPile ==0)
		{
			iMaxPile = 1;
		}

		// 冲过能的宝石占一个格子。
		if ((iGemStat = gem_is_chongneng( pstItemDef, &pstGrid->GridData.RoleItem)) == GEM_STAT_POWER)
		{
			iMaxPile = 1;
		}

		if (1 == iMaxPile)
		{
			if (iTotal >= unMax|| iStuffNum >= unMax)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "arrange err");
				return -1;
			}
			
			if (pstItemDef->ItemType == ITEM_TYPE_STUFF)
			{
				memcpy(&astStuffGrid[iStuffNum],pstGrid,sizeof(*pstGrid));
				iStuffNum ++;
			}	
			else
			{
				memcpy(&astGrid[iTotal],pstGrid,sizeof(*pstGrid));
				iTotal ++;
			}
			
			continue;
		}

		//物品实例是否绑定
		fBind = is_bind_roleitem(&pstGrid->GridData.RoleItem);

		stGrid = *pstGrid;
		// 前面有的 肯定是整理过的(充能的宝石要特殊判断一下)
		if(	-1 == find_grid_of_package(astGrid, iTotal, pstItemDef, fBind, iGemStat) &&
			-1 == find_grid_of_package(astStuffGrid, iStuffNum, pstItemDef, fBind, iGemStat))
		{
			// 这里不要计算充能的宝石
			iPreNum = iGridNum = z_get_pak_item_num_by_bindtype(pstEnv, pstPak, 
							pstItemDef->ItemID, cListType, 1, fBind);
			while(iGridNum > 0)
			{
				if (iTotal >= unMax|| iStuffNum >= unMax)
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "arrange err");
					return -1;
				}
				
				if(iGridNum > iMaxPile)
				{
					stGrid.GridData.RoleItem.Num = iMaxPile;
				}
				else
				{
					stGrid.GridData.RoleItem.Num = iGridNum;
				}
				
				iGridNum -= iMaxPile;
				stGrid.GridData.RoleItem.WID = z_get_gid(pstEnv->pstAppCtx, pstEnv);

				if (pstItemDef->ItemType == ITEM_TYPE_STUFF)
				{
					memcpy(&astStuffGrid[iStuffNum],&stGrid,sizeof(stGrid));
					iStuffNum ++;
				}	
				else
				{
					memcpy(&astGrid[iTotal],&stGrid,sizeof(stGrid));
					iTotal ++;
				}

			}

			if (pstItemDef->ItemType == ITEM_TYPE_STUFF)
			{
				iLastNum = pkg_arrange_get_bind_num(astStuffGrid, iStuffNum, fBind, pstItemDef);
			}
			else
			{
				iLastNum = pkg_arrange_get_bind_num(astGrid, iTotal, fBind, pstItemDef);
			}

			if (iLastNum != iPreNum)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, 
							"package_grid_arrange:prenum=%d lastnum=%d itemid=%d bind=%d listtype=%d\n",
							iPreNum, iLastNum,pstItemDef->ItemID, fBind, cListType);
				return -1;
			}
		}
	}

	if(iTotal > 1)
	{
		qsort(astGrid, iTotal, sizeof(astGrid[0]),package_grid_cmp);
	}

	if (iStuffNum > 1)
	{
		qsort(astStuffGrid, iStuffNum, sizeof(astStuffGrid[0]),package_grid_cmp);
	}
	
	for(i=0; i<iTotal; i++)
	{
		if(astGrid[i].Type == GRID_ITEM_ARM)
		{
			astGrid[i].GridData.RoleArm.GridIdx = i;
		}
		else
		{
			astGrid[i].GridData.RoleItem.GridIdx = i;
		}
	}

	for (i=0; i<iStuffNum; i++)
	{
		astStuffGrid[i].GridData.RoleItem.GridIdx = i+iTotal;
	}

	if (iTotal +iUnVaildNum +iStuffNum > unMax)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "arrange err");
		return -1;
	}

	for (i=0; i<iUnVaildNum; i++)
	{
		if (astUnVaildGrid[i].GridData.RoleItem.GridIdx < iTotal+iStuffNum)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "arrange err");
			return -1;
		}
	}

	*punNum = iTotal +iUnVaildNum + iStuffNum;
	if (iTotal > 0)
		memcpy(pstRoleGrid,  astGrid, sizeof(astGrid[0])*iTotal);
	if (iStuffNum > 0)
		memcpy(pstRoleGrid+iTotal,  astStuffGrid, sizeof(astStuffGrid[0])*iStuffNum);
	if (iUnVaildNum > 0)
		memcpy(pstRoleGrid+iTotal+iStuffNum,  astUnVaildGrid, sizeof(astUnVaildGrid[0])*iUnVaildNum);
	return 0;
}

int package_get_freegrid_num(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, char cListType)
{
	int iNum = 0;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	
	switch(cListType)
	{
	case LIST_TYPE_SUNDRIES:
		iNum = pstPak->SundriesPackage.MaxNum - pstPak->SundriesPackage.Num;
		break;
	case LIST_TYPE_TASK:
		iNum = pstPak->TaskPackage.MaxNum - pstPak->TaskPackage.Num;
		break;
	case LIST_TYPE_STUFF:
		iNum = pstPak->StuffPackage.MaxNum - pstPak->StuffPackage.Num;
		break;
	case LIST_TYPE_AMM:
		iNum = pstPak->AmmPackage.MaxNum - pstPak->AmmPackage.Num;
		break;
	case LIST_TYPE_STASH:
		iNum = pstPak->StashPackage.MaxNum - pstPak->StashPackage.Num;
		break;
	case LIST_TYPE_SHOPPING:
		iNum = pstPak->ShoppingPackage.MaxNum - pstPak->ShoppingPackage.Num;
		break;
	default:
		return -1;
		break;
	}
	
	return iNum;
}
int package_arrange(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, char cListType)
{
	CSPKG *pstPkgRes = &pstEnv->stCSRes;
	CSPACKAGEINFO *pstPackageInfo = &pstPkgRes->Body.PackageInfo;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	time_t t;
	int iRet;
	
	if (pstPlayer->stOnline.State & CS_STAT_STALL)
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK7 );
		return -1;
	}

	if (pstPlayer->stOnline.stExchgOnline.bExchgStat != EXCHG_NO)
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK8 );
		return -1;
	}

	if(cListType == LIST_TYPE_STASH)
	{
		t = pstPlayer->stOnline.tPackageArrange1;
	}
	else
	{
		t = pstPlayer->stOnline.tPackageArrange;	
	}

	if (pstAppCtx->stCurr.tv_sec - t < PACKAGE_ARRANGE_MIN)
	{
		z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_OP_LIMIT, PACKAGE_ARRANGE_MIN);
		return -1;
	}
	

	switch(cListType)
	{
	case LIST_TYPE_SUNDRIES:
		iRet = package_grid_arrange(pstEnv,pstPak->SundriesPackage.RoleGrids, &pstPak->SundriesPackage.Num,
								pstPak->SundriesPackage.MaxNum, pstPak,cListType);
		break;

	case LIST_TYPE_TASK:
		iRet = package_item_arrange( pstEnv,pstPak->TaskPackage.RoleItems, &pstPak->TaskPackage.Num, 
								pstPak->TaskPackage.MaxNum, pstPak,cListType);
		break;
	case LIST_TYPE_STUFF:
		//package_item_arrange( pstEnv,pstPak->StuffPackage.RoleItems, &pstPak->StuffPackage.Num,
								//pstPak->StuffPackage.MaxNum, pstPak,cListType);
		break;
	case LIST_TYPE_AMM:
		//package_item_arrange( pstEnv,pstPak->AmmPackage.RoleItems, &pstPak->AmmPackage.Num,
								//pstPak->AmmPackage.MaxNum, pstPak,cListType);
		break;
	case LIST_TYPE_STASH:
		iRet = package_grid_arrange(pstEnv,pstPak->StashPackage.RoleGrids, &pstPak->StashPackage.Num,
								pstPak->StashPackage.MaxNum, pstPak,cListType);
		break;
	case LIST_TYPE_SHOPPING:
		iRet = package_grid_arrange(pstEnv,pstPak->ShoppingPackage.RoleGrids, &pstPak->ShoppingPackage.Num,
								pstPak->ShoppingPackage.MaxNum, pstPak,cListType);
		break;
	default:
		return -1;
		break;
	}

	if (iRet  < 0)
		return -1;

	if(cListType == LIST_TYPE_STASH)
	{
		pstPlayer->stOnline.tPackageArrange1 = pstAppCtx->stCurr.tv_sec;
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK9);
	}
	else
	{
		pstPlayer->stOnline.tPackageArrange = pstAppCtx->stCurr.tv_sec;
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK10);
	}
	//pstPlayer->stOnline.tPackageArrange = pstAppCtx->stCurr.tv_sec;

	fairy_evolve_update_lift_time(pstEnv, pstPlayer);
	pstPackageInfo->Type = PACKAGE_INFO_ALL;
	memcpy(&pstPackageInfo->PackageInfoData.PackageAll, pstPak, sizeof(pstPackageInfo->PackageInfoData.PackageAll));
	Z_CSHEAD_INIT(&pstPkgRes->Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkgRes);
	
	return 0;
}

int package_money(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSPACKAGEMONEY *pstPackageMoney)
{
	PlayerChagAttrVal stAttrVal;
	long long llTmp = 0;
	LEVELUP *pstLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	
	if (!pstLevelUp)
	{
		return 0;
	}
	
	if (0 > check_npc_ui(pstEnv, pstPlayer, UI_TYPE_STASH))
	{
		return -1;
	}
	
	if (ATTR_ID_MONEY == pstPackageMoney->SrcMoneyAttrID)
	{
		if (ATTR_ID_STASHMONEY != pstPackageMoney->DstMoneyAttrID)
		{
			return -1;
		}

		llTmp = pstPlayer->stRoleData.Package.StashPackage.Money;
		if (pstPackageMoney->MoneyNum > pstPlayer->stRoleData.RoleDetail.Money ||
			llTmp+pstPackageMoney->MoneyNum > MAX_MONEY ||
			llTmp+pstPackageMoney->MoneyNum > pstLevelUp->MaxStashMoney)
		{
			return -1;
		}

		stAttrVal.AttrNum = 2;
		stAttrVal.AttrList[0].AttrID = ATTR_ID_MONEY;
		stAttrVal.AttrList[0].nFlag = 1;
		stAttrVal.AttrList[0].Value.Money = pstPackageMoney->MoneyNum ;
		stAttrVal.AttrList[1].AttrID = ATTR_ID_STASHMONEY;
		stAttrVal.AttrList[1].nFlag = 0;
		stAttrVal.AttrList[1].Value.StashMoney = pstPackageMoney->MoneyNum ;
	}
	else if (ATTR_ID_STASHMONEY == pstPackageMoney->SrcMoneyAttrID)
	{
		if (player_check_safelock(pstEnv, pstPlayer) < 0)
		{
			return -1;
		}
		if (ATTR_ID_MONEY != pstPackageMoney->DstMoneyAttrID)
		{
			return -1;
		}

		llTmp = pstPlayer->stRoleData.RoleDetail.Money;
		if (pstPackageMoney->MoneyNum > pstPlayer->stRoleData.Package.StashPackage.Money  || llTmp+pstPackageMoney->MoneyNum > MAX_MONEY)
		{
			return -1;
		}

		stAttrVal.AttrNum = 2;
		stAttrVal.AttrList[0].AttrID = ATTR_ID_MONEY;
		stAttrVal.AttrList[0].nFlag = 0;
		stAttrVal.AttrList[0].Value.Money = pstPackageMoney->MoneyNum ;
		stAttrVal.AttrList[1].AttrID = ATTR_ID_STASHMONEY;
		stAttrVal.AttrList[1].nFlag = 1;
		stAttrVal.AttrList[1].Value.StashMoney = pstPackageMoney->MoneyNum ;
	}
	else if(ATTR_ID_BULL == pstPackageMoney->SrcMoneyAttrID)			// bull 背包->仓库
	{
		if (ATTR_ID_STASHBULL != pstPackageMoney->DstMoneyAttrID)
		{
			return -1;
		}

		llTmp = pstPlayer->stRoleData.Package.StashPackage.Bull;
		if (pstPackageMoney->MoneyNum > pstPlayer->stRoleData.RoleDetail.Bull || llTmp+pstPackageMoney->MoneyNum > MAX_BULL)
		{
			return -1;
		}

		stAttrVal.AttrNum = 2;
		stAttrVal.AttrList[0].AttrID = ATTR_ID_BULL;
		stAttrVal.AttrList[0].nFlag = 1;
		stAttrVal.AttrList[0].Value.Bull = pstPackageMoney->MoneyNum ;
		stAttrVal.AttrList[1].AttrID = ATTR_ID_STASHBULL;
		stAttrVal.AttrList[1].nFlag = 0;
		stAttrVal.AttrList[1].Value.StashBull = pstPackageMoney->MoneyNum ;
	}
	else if(ATTR_ID_STASHBULL == pstPackageMoney->SrcMoneyAttrID)     // bull 仓库->背包
	{
		if (player_check_safelock(pstEnv, pstPlayer) < 0)
		{
			return -1;
		}
		
		if (ATTR_ID_BULL!= pstPackageMoney->DstMoneyAttrID)
		{
			return -1;
		}

		llTmp = pstPlayer->stRoleData.RoleDetail.Bull;
		if (pstPackageMoney->MoneyNum > pstPlayer->stRoleData.Package.StashPackage.Bull|| llTmp+pstPackageMoney->MoneyNum > MAX_BULL)
		{
			return -1;
		}

		stAttrVal.AttrNum = 2;
		stAttrVal.AttrList[0].AttrID = ATTR_ID_BULL;
		stAttrVal.AttrList[0].nFlag = 0;
		stAttrVal.AttrList[0].Value.Bull = pstPackageMoney->MoneyNum ;
		stAttrVal.AttrList[1].AttrID = ATTR_ID_STASHBULL;
		stAttrVal.AttrList[1].nFlag = 1;
		stAttrVal.AttrList[1].Value.StashBull= pstPackageMoney->MoneyNum ;
	}
	else
		return -1;

	z_attr_chg(pstAppCtx, pstEnv, pstPlayer, &stAttrVal, 1);
	
	return 0;
}

int package_check_ride(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSPACKAGEUSE *pstUse, RESULT *pstResult, ITEMDEF *pstItemDef)
{	
	int i = 0;
	ROLERIDELIST   *pstRideList = &pstPlayer->stRoleData.MiscInfo.RideList;
	RIDEDEF *pstRideDef;

	UNUSED(pstAppCtx);
	UNUSED(pstUse);
		
	if ( (CAREER_NON != pstItemDef->WearPre.Career && (pstPlayer->stRoleData.Career&0x0F) != (pstItemDef->WearPre.Career&0x0F)) ||
		 (CAREER_NON != (pstItemDef->WearPre.Career&0xF0) && (pstPlayer->stRoleData.Career&0xF0) != (pstItemDef->WearPre.Career&0xF0)) )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK11);
		return -1;
	}

	if (pstPlayer->stRoleData.Level < pstItemDef->WearPre.RoleLevelMin )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK12, pstItemDef->WearPre.RoleLevelMin);
		return -1;
	}

	if ( pstPlayer->stRoleData.Gender !=pstItemDef->WearPre.Gender && GENDER_NO != pstItemDef->WearPre.Gender)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,  ZONEERR_PAK13);
		return -1;

	}

	pstRideDef = z_find_ride_def(pstEnv, pstResult->ResultVal2, pstResult->ResultVal3);
	if(NULL == pstRideDef)
	{
		return -1;
	}

	for (i = 0 ; i <pstRideList->Num ; i++)
	{
		if((int)pstRideList->RideGrids[i].ItemID == pstResult->ResultVal2)
		{
			if(pstRideList->RideGrids[i].Endtime > 0)
			{
				return 0;
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK14 );
				return -1;
			}
		}
	}

	if ( pstRideList->Num >= pstRideList->MaxNum)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK15);
		return -1;
	}

	return 0;
}

int package_add_ride_grid_check( ZONESVRENV* pstEnv,Player *pstPlayer, RESULT *pstResult)
{
	
	ROLERIDELIST   *pstRideList = &pstPlayer->stRoleData.MiscInfo.RideList;
	UNUSED(pstResult);

	if(pstRideList->MaxNum >= MAX_RIDE_GRID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK16 );
		return -1;
	}
	
	return 0;
}

int package_use_add_ride_grid( ZONESVRENV* pstEnv,Player *pstPlayer, RESULT *pstResult)
{	
	ROLERIDELIST   *pstRideList = &pstPlayer->stRoleData.MiscInfo.RideList;

	if(RAND1(100) < pstResult->ResultVal2)
	{
		if(pstRideList->MaxNum + pstResult->ResultVal1 >= MAX_RIDE_GRID)
		{
			pstRideList->MaxNum = MAX_RIDE_GRID;
		}
		else
		{
			pstRideList->MaxNum += pstResult->ResultVal1;
		}

		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK199);
		ride_list_chg(pstEnv, pstPlayer);
	}
	else
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK200);
	}
	
	return 0;
}

void ride_list_chg( ZONESVRENV* pstEnv,Player *pstPlayer)
{
	CSPKG stPkg;
	CSRIDELISTCHG *pstRideListChg = &stPkg.Body.RideListChg;
	ROLERIDELIST   *pstRideList = &pstPlayer->stRoleData.MiscInfo.RideList;

	memcpy(&pstRideListChg->RideList,pstRideList,sizeof(pstPlayer->stRoleData.MiscInfo.RideList)) ;

	Z_CSHEAD_INIT(&stPkg.Head, RIDE_LIST_CHG);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return ;
}

int ride_action(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int RideItemID, int iRideLevel)
{		
	CSPKG stPkgRes;
	CSACTIONRES *pstActionRes = &stPkgRes.Body.ActionRes;
	UNUSED(pstAppCtx);

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_RIDE_LEARN;

	pstActionRes->Data.RideLearn.RideItemID = RideItemID;
	pstActionRes->Data.RideLearn.RideLevel = iRideLevel;

	Z_CSHEAD_INIT(&stPkgRes.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkgRes, 0);
	
	return 0;
}

int package_use_ride(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef)
{
	int iIdx;
	RESULT *pstResult = NULL;
	ROLERIDELIST   *pstRideList = &pstPlayer->stRoleData.MiscInfo.RideList;
	RIDEDEF *pstRideDef = NULL;
	RIDEGRID *pstRideGrid;

	pstResult = z_item_get_result(pstItemDef, RESULT_RIDE);
	if (NULL == pstResult )
	{
		return -1;
	}

	pstRideDef = z_find_ride_def(pstEnv, pstResult->ResultVal2, pstResult->ResultVal3);
	if(NULL == pstRideDef)
	{
		return -1;
	}

	iIdx = z_get_package_ride(pstPlayer, pstResult->ResultVal2);
	if (iIdx < 0)
	{	
		pstRideGrid = &pstRideList->RideGrids[pstRideList->Num++];
		pstRideGrid->ItemID = pstResult->ResultVal2;
		pstRideGrid->Level = pstResult->ResultVal3;
		pstRideGrid->Endtime = 0;
		pstRideGrid->Count = 0;
	}
	else
	{
		pstRideGrid = &pstRideList->RideGrids[iIdx];
	}
		
	if ( pstResult->ResultVal1 != 0 )
	{
		if(pstRideGrid->Endtime > pstAppCtx->stCurr.tv_sec)
		{
			pstRideGrid->Endtime += pstResult->ResultVal1 * 86400; 
		}
		else
		{
			pstRideGrid->Endtime = pstAppCtx->stCurr.tv_sec + pstResult->ResultVal1 * 86400; 
		}
	}
	else
	{
		pstRideGrid->Endtime = 0;
	}

	//提示
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK101, pstRideDef->RideName);

	//运营日志
	z_role_ride_oplog(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstResult->ResultVal2, pstResult->ResultVal3, OP_RIDE_LEARN);
	
	ride_list_chg(pstEnv, pstPlayer);
	ride_action(pstAppCtx,  pstEnv, pstPlayer, pstResult->ResultVal2, pstResult->ResultVal3);

	task_player_trace(pstEnv, pstPlayer, TASK_OBJ_RIDE);

	//成长之路
	{
		ACHIEVEMENTCOND stCond;

		memset(&stCond,0,sizeof(stCond));
		
		stCond.Type = COND_RIDE_GET;
		stCond.Val1 = 0;
		stCond.Val2 = pstRideDef->RideSpd;

		z_player_achievement_cond(pstEnv,pstPlayer,&stCond);

		if (CAREER_NON != pstItemDef->WearPre.Career)
		{
			stCond.Type = COND_RIDE_GET;
			stCond.Val1 = 1;
			stCond.Val2 = pstRideDef->RideSpd;

			z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
		}
		
	
	}
	
	return 0;
}

int package_use_skill_book(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, int iUseItemID)
{
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	UNUSED(pstPlayer);
	UNUSED(pstResult);
	
	if (pstResult->ResultVal1 == SKILL_LEARN)
	{
		if (z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstResult->ResultVal2, 1, 1) ==0)
		{
			skill_action(pstEnv->pstAppCtx,  pstEnv, pstPlayer, pstResult->ResultVal2, 1, SKILL_LEARN);
			task_player_trace(pstEnv, pstPlayer, TASK_OBJ_PLAYER_SKILL);

			z_role_skill_oplog(pstEnv, pstPlayer, OP_BOOK_LEARN_SKILL, pstResult->ResultVal2, 1, 0,
								pstPlayer->stRoleData.RoleDetail.SkillPoint, pstPlayer->stRoleData.RoleDetail.SkillPoint,
								NULL,NULL,iUseItemID,NULL);
		}
	}
	/*
	else if (pstResult->ResultVal1 == DIATHESIS_LEARN)
	{
		if (z_add_diathesis(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstResult->ResultVal2, 1) == 0)
		{
			skill_action(pstEnv->pstAppCtx,  pstEnv, pstPlayer, pstResult->ResultVal2, 1, DIATHESIS_LEARN);
			task_player_trace(pstEnv, pstPlayer, TASK_OBJ_PLAYER_DIATHESIS);	
			z_role_diathesis_oplog(pstEnv, pstPlayer, OP_BOOK_LEARN_DIATHESIS, pstResult->ResultVal2, 1, 0);
		}
	}
	*/
	else
	{
		return -1;
	}
	return 0;
}

/*int package_check_add_skill_point(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, 
								ITEMDEF *pstItemDef)
{
	if (pstItemDef->ArmType != 0)
	{
		return -1;
	}

	
	return 0;
}*/

int package_use_add_skill_point(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult)
{
	PlayerChagAttrVal stAttr;
	stAttr.AttrNum = 0;

	stAttr.AttrList[stAttr.AttrNum].nFlag = 0;
	stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_SKILLPOINT;
	stAttr.AttrList[stAttr.AttrNum].Value.SkillPoint = pstResult->ResultVal1;
	stAttr.AttrNum++;
	
	z_attr_chg(pstAppCtx, pstEnv, pstPlayer, &stAttr, 1);
	return 0;
}

int item_call_mon(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEMAPINST *pstMapInst, int iMonNum, int iMonID, int iRadius, int iLifeTime,
					int iX,int iY ,int iPatrol, int iDir, int iBctDefID)
{
	int i = 0;
	Monster stMonster;
	MapIndex *pstMapIdx = NULL;
	RESPOS stPos;
	MONSTERDEF *pstMonDef = NULL;
	Monster *pstFind = NULL;
	RESPOS stMonPos;
	int iRet = 0;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos=	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.tRefresh	=	pstEnv->pstAppCtx->stCurr.tv_sec;
	stMonster.iPatrolID = iPatrol;
	//stMonster.tLifeEnd = iLifeTime > 0?pstEnv->pstAppCtx->stCurr.tv_sec+iLifeTime:0;
	
	STRNCPY(stMonster.szOwner, pstPlayer->stRoleData.RoleName, sizeof(stMonster.szOwner));

	//白层塔怪物所在层
	if (iBctDefID > 0)
	{
		stMonster.szOwner[0] = 0;
		stMonster.stBctInfo.iBctDefID = iBctDefID;
		get_mon_bct_dyn(pstEnv, &stMonster);
	}

	pstMonDef = z_get_mon_def(pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		return -1;
	}

	if (iX == 0 && iY == 0)
	{
		stMonPos = pstPlayer->stRoleData.Pos;
	}
	else
	{
		stMonPos.X = iX;
		stMonPos.Y = iY;
	}
	
	for (i=0; i<iMonNum; i++)
	{
		if(iDir > 0)
		{
			stMonster.bDirection = iDir%180;
		}
		else
		{
			stMonster.bDirection = RAND1(180);
		}
		
		//z_get_ran_pos(pstEnv, pstMapIdx, &pstPlayer->stRoleData.Pos, &pstPlayer->stRoleData.Pos, iRadius, 0, &stPos);
		z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &stMonPos, &stMonPos, iRadius, 0, &stPos);
		stMonster.stInitPos = stPos;

		pstFind =	z_new_monster(pstEnv->pstAppCtx, pstEnv, &stMonster, pstMonDef);
	
		if( !pstFind )
			return -1;

		//白层塔怪物所在层
		if (iBctDefID > 0)
		{
			BCTMON *pstBctMon;
			
			pstFind->bAtkType = ATK_TYPE_INITIATIVE;
			pstFind->unViewDist = BCT_MON_VIEW_DIST;
			pstFind->unTraceDist = BCT_MON_TRACE_DIST;

			pstBctMon = get_mon_bct_dyn(pstEnv, pstFind);
			if(pstBctMon)
			{
				pstFind->stMonDynAttr.Level = pstBctMon->Level;
			}
		}

		pstFind->cFirstBorn = 1;
		pstFind->tLifeEnd = iLifeTime > 0?pstEnv->pstAppCtx->stCurr.tv_sec+iLifeTime:0;
		if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstFind,NULL))
		{
			tmempool_free(pstEnv->pstMonsterPool, pstFind->iMIndex);
			return -1;
		}
	
		pstFind->cFirstBorn = 0;
		if(i == 0)
		{
			iRet = pstFind->iID;
		}
	}
	
	return iRet;
}

int item_cool_check(ZONESVRENV* pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef)
{
	int i = 0;
	struct timeval stTmp;
	struct timeval stTmp1;
	long long  llMs = 0;
	ITEMUSECOOLLIST *pstItemUseCoolList = &pstPlayer->stRoleData.MiscInfo.ItemUseCoolList;

	memset(&stTmp,0,sizeof(stTmp));
	memset(&stTmp1,0,sizeof(stTmp1));
	
	if (pstItemDef->SelfCool > 0)
	{
		for (i=0; i<pstItemUseCoolList->ItemSelfCoolNum;i++)
		{
			if (pstItemUseCoolList->ItemSelfCools[i].ItemID != pstItemDef->ItemID)
			{
				continue;
			}
			
			stTmp.tv_sec = pstItemUseCoolList->ItemSelfCools[i].CoolEnd.BenchTimeSec;
			stTmp.tv_usec = pstItemUseCoolList->ItemSelfCools[i].CoolEnd.BenchTimeUSec;

			TV_DIFF(stTmp1, stTmp, pstEnv->pstAppCtx->stCurr);
			TV_TO_MS(llMs, stTmp1);

			if (llMs  > 200)
			{
				return -1;
			}
		}
	}

	if (pstItemDef->SelfSpeCoolType > 0)
	{
		for (i=0; i<pstItemUseCoolList->ItemSpeCoolNum;i++)
		{
			if (pstItemUseCoolList->ItemSpeCools[i].ItemCoolType != pstItemDef->SpeCoolType)
			{
				continue;
			}
			
			stTmp.tv_sec = pstItemUseCoolList->ItemSpeCools[i].CoolEnd.BenchTimeSec;
			stTmp.tv_usec = pstItemUseCoolList->ItemSpeCools[i].CoolEnd.BenchTimeUSec;

			TV_DIFF(stTmp1, stTmp, pstEnv->pstAppCtx->stCurr);
			TV_TO_MS(llMs, stTmp1);

			if (llMs  > 200)
			{
				return -1;
			}
		}
	}

	return 0;
}

int item_cool_set(ZONESVRENV* pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef)
{
	int i = 0;
	int iIdx = 0;
	long long  llMs = 0;
	struct timeval stTmp;
	struct timeval stTmp1;
	struct timeval stTmp2;
	ITEMUSECOOLLIST *pstItemUseCoolList = &pstPlayer->stRoleData.MiscInfo.ItemUseCoolList;

	memset(&stTmp,0,sizeof(stTmp));
	memset(&stTmp1,0,sizeof(stTmp1));
	memset(&stTmp2,0,sizeof(stTmp2));
	if (pstItemDef->SelfCool > 0)
	{
		llMs = pstItemDef->SelfCool*100.0;
		TV_ADD_MS(stTmp, pstEnv->pstAppCtx->stCurr, llMs);
		
		for (i=0; i<pstItemUseCoolList->ItemSelfCoolNum;i++)
		{
			if (pstItemUseCoolList->ItemSelfCools[i].ItemID == pstItemDef->ItemID)
			{
				if (i != pstItemUseCoolList->ItemSelfCoolNum-1)
				{
					memmove(&pstItemUseCoolList->ItemSelfCools[i], &pstItemUseCoolList->ItemSelfCools[i+1],
								(pstItemUseCoolList->ItemSelfCoolNum-1-i)*sizeof(pstItemUseCoolList->ItemSelfCools[0]));
				}

				iIdx = pstItemUseCoolList->ItemSelfCoolNum-1;

				break;
			}
		}

		if (i == pstItemUseCoolList->ItemSelfCoolNum)
		{
			if (pstItemUseCoolList->ItemSelfCoolNum < MAX_ITEM_SELFCOOL_NUM)
			{
				iIdx = pstItemUseCoolList->ItemSelfCoolNum;
				pstItemUseCoolList->ItemSelfCoolNum++;
			}
			else
			{
				memmove(&pstItemUseCoolList->ItemSelfCools[0], &pstItemUseCoolList->ItemSelfCools[1],
							(pstItemUseCoolList->ItemSelfCoolNum-1)*sizeof(pstItemUseCoolList->ItemSelfCools[0]));
				

				iIdx = pstItemUseCoolList->ItemSelfCoolNum-1;
			}
		}

		pstItemUseCoolList->ItemSelfCools[iIdx].ItemID = pstItemDef->ItemID;
		pstItemUseCoolList->ItemSelfCools[iIdx].CoolEnd.BenchTimeSec = stTmp.tv_sec;
		pstItemUseCoolList->ItemSelfCools[iIdx].CoolEnd.BenchTimeUSec = stTmp.tv_usec;
	}

	if (pstItemDef->SpeCool > 0 && pstItemDef->SpeCoolType > 0)
	{
		llMs = pstItemDef->SpeCool*100.0;
		TV_ADD_MS(stTmp, pstEnv->pstAppCtx->stCurr, llMs);
		
		for (i=0; i<pstItemUseCoolList->ItemSpeCoolNum;i++)
		{
			if (pstItemUseCoolList->ItemSpeCools[i].ItemCoolType == pstItemDef->SpeCoolType)
			{
				stTmp1.tv_sec = pstItemUseCoolList->ItemSpeCools[i].CoolEnd.BenchTimeSec;
				stTmp1.tv_usec = pstItemUseCoolList->ItemSpeCools[i].CoolEnd.BenchTimeUSec;
				TV_DIFF(stTmp2, stTmp, stTmp1);
				TV_TO_MS(llMs, stTmp2);
				if (llMs < 0)
				{
					return 0;
				}
				
				if (i != pstItemUseCoolList->ItemSpeCoolNum-1)
				{
					memmove(&pstItemUseCoolList->ItemSpeCools[i], &pstItemUseCoolList->ItemSpeCools[i+1],
								(pstItemUseCoolList->ItemSpeCoolNum-1-i)*sizeof(pstItemUseCoolList->ItemSpeCools[0]));
				}

				iIdx = pstItemUseCoolList->ItemSpeCoolNum-1;
				pstItemUseCoolList->ItemSpeCools[iIdx].ItemCoolType = pstItemDef->SpeCoolType;
				pstItemUseCoolList->ItemSpeCools[iIdx].CoolEnd.BenchTimeSec = stTmp.tv_sec;
				pstItemUseCoolList->ItemSpeCools[iIdx].CoolEnd.BenchTimeUSec = stTmp.tv_usec;

				return 0;
			}
		}

		if (pstItemUseCoolList->ItemSpeCoolNum < MAX_ITEM_SPECOOL_NUM)
		{
			iIdx = pstItemUseCoolList->ItemSpeCoolNum;
			pstItemUseCoolList->ItemSpeCoolNum++;
		}
		else
		{
			memmove(&pstItemUseCoolList->ItemSpeCools[0], &pstItemUseCoolList->ItemSpeCools[1],
							(pstItemUseCoolList->ItemSpeCoolNum-1)*sizeof(pstItemUseCoolList->ItemSpeCools[0]));
			iIdx = pstItemUseCoolList->ItemSpeCoolNum-1;
		}
		
		pstItemUseCoolList->ItemSpeCools[iIdx].ItemCoolType = pstItemDef->SpeCoolType;
		pstItemUseCoolList->ItemSpeCools[iIdx].CoolEnd.BenchTimeSec = stTmp.tv_sec;
		pstItemUseCoolList->ItemSpeCools[iIdx].CoolEnd.BenchTimeUSec = stTmp.tv_usec;
	}

	
	return 0;
}

RESULT *z_item_get_result(ITEMDEF *pstItemDef, int iResultID)
{
	int i = 0;

	for (i=0; i<MAX_ITEM_RESULT; i++) 
	{
		if (0 == pstItemDef->Result[i].ResultID)
		{
			break;
		}

		if (iResultID == pstItemDef->Result[i].ResultID)
		{
			return pstItemDef->Result+i;
		}
	}

	return NULL;
}


int package_check_skill_book(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, ITEMDEF *pstItemDef)
{
	/*
		pstResult->ResultVal1  操作类型
		pstResult->ResultVal2  技能ID
	*/
	SKILLDEF *pstSkillDef = NULL;
	DIATHESISDEF *pstDiathesisDef = NULL;
	ZoneSkill *pstZoneSkill = NULL;
	DIATHESISDATA *pstDiathesis = NULL;
	UNUSED(pstAppCtx);
	UNUSED(pstItemDef);
	
	if (pstResult->ResultVal1 == SKILL_LEARN)
	{
		pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstResult->ResultVal2);
		if (pstZoneSkill)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK102);
			return -1;
		}

		pstSkillDef = 	z_find_skill(pstEnv, pstResult->ResultVal2, 1 ,NULL);	
		if (pstSkillDef == NULL )
		{
			return -1;
		}

		if (skill_check_by_book(pstEnv, pstPlayer, &pstSkillDef->Learn) < 0)
		{
			return -1;
		}
	}
	
	else if(pstResult->ResultVal1 == DIATHESIS_LEARN)
	{
		pstDiathesis = z_get_player_diathesis(pstPlayer, pstResult->ResultVal2);
		if (pstDiathesis)
		{
			return -1;
		}

		pstDiathesisDef = z_find_diathesis(pstEnv, pstResult->ResultVal2, 1);
		if (pstDiathesisDef == NULL )
		{
			return -1;
		}

		if (0 > skill_check_by_book(pstEnv, pstPlayer, &pstDiathesisDef->Learn))
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

// 使用道具复活宠物前置检查
/*
int package_check_pet_revive(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, 
								ITEMDEF *pstItemDef,tdr_ulonglong WID)
{	
	UNUSED(pstItemDef);
	
	ROLEPET *pstRolePet = NULL;
	PETDEF *pstPetDef = NULL;

	pstRolePet = pet_find_rolepet(pstPlayer, WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}		
	
	pstPetDef = pet_find_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (!pstPetDef)
	{
		return -1;
	}

	// 血量
	if (pstResult->ResultVal1 <= 0 )
	{
		return -1;
	}
	
 	if (pstPetDef->Type == PET_TYPE_SHIWEI)
	{
		// 非死亡
		if (!(pstRolePet->StatFlag & PET_STAT_DEAD))
		{
			z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, "宠物不处于死亡状态,不能使用此类道具");
			return -1;
		}

		if (pstRolePet->Power != 0)
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

int package_use_revive_pet(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, tdr_ulonglong WID)
{
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	ROLEPET *pstRolePet = NULL;
	CSPETCHGDATA *pstPetChgDataList = NULL;

	pstRolePet = find_package_shi(pstPlayer, WID,0);
	if (!pstRolePet)
	{
		return -1;
	}

	pstPetChgDataList = &pstPetSvr->PetChgDataList[0];
	pstPetSvr->Num = 1;
	pstPetChgDataList->AttrNum = 0;
	pstPetChgDataList->Wid = WID;
	
	pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].Type = PET_CHG_POWER;
	pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].AttrValue.ChgPower.Power = pstRolePet->Power = pstResult->ResultVal1;
	pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].AttrValue.ChgPower.XieDaiPos = -1;
	pstPetChgDataList->AttrNum ++;

	pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].Type = PET_CHG_STAT;
	pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].AttrValue.ChgStatFlag = (pstRolePet->StatFlag &= ~PET_STAT_DEAD);
	pstPetChgDataList->AttrNum ++;

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int package_check_addarm_grid(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, 
								ITEMDEF *pstItemDef,tdr_ulonglong WID)
{
	UNUSED(pstResult);
	UNUSED(pstItemDef);
	ROLEPET *pstRolePet = NULL;
	PETDEF *pstPetDef = NULL;
	PETRACELEVELDEF *pstPetRaceLevelDef = NULL;
	PETARM *pstPetArms = NULL;    

	pstRolePet = find_package_shi(pstPlayer, WID,0);
	if (!pstRolePet)
	{
		return -1;
	}
	pstPetArms = &pstRolePet->PetArms;

	pstPetDef = pet_find_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (!pstPetDef)
	{
		return -1;
	}
	
	
	pstPetRaceLevelDef = pet_find_racelevel_def(pstEnv, pstPetDef->Race, pstRolePet->Level, NULL);
	if (!pstPetRaceLevelDef)
	{
		return -1;
	}

	if (pstPetArms->AddedCount >= pstPetRaceLevelDef->AddedArmColMax || pstPetArms->MaxNum >= MAX_PET_ARM)
	{
		z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, "宠物当前额外装备栏已达到上限");
		return -1;
	}
	return 0;
}

int package_check_pet_revive_laosi(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, 
								ITEMDEF *pstItemDef,tdr_ulonglong WID)
{
	ROLEPET *pstRolePet = NULL;
	PETDEF *pstPetDef = NULL;

	pstRolePet = pet_find_rolepet(pstPlayer, WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}
	
	pstPetDef = pet_find_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (!pstPetDef)
	{
		return -1;
	}

	if ((int)pstRolePet->Age < pstPetDef->MaxAge*PET_AGE_UNIT)
	{
		z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, "宠物年龄没有达到上限,不能使用此道具");
		return -1;
	}

	// 减少的年龄
	if (pstResult->ResultVal1 <= 0)
	{
		return -1;
	}
	
	return 0;
}*/

/*int package_use_revive_pet_laosi(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, tdr_ulonglong WID)
{
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	ROLEPET *pstRolePet = NULL;
	CSPETCHGDATA *pstPetChgDataList;

	pstRolePet = pet_find_rolepet(pstPlayer, WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}

	pstPetChgDataList = &pstPetSvr->PetChgDataList[0];
	pstPetSvr->Num = 1;
	pstPetChgDataList->AttrNum = 0;
	pstPetChgDataList->Wid = WID;

	if (pstRolePet->Power <= 0)
	{
		pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].Type = PET_CHG_POWER;
		pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].AttrValue.ChgPower.Power= pstRolePet->Power = 1;
		pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].AttrValue.ChgPower.XieDaiPos = -1;
		pstPetChgDataList->AttrNum ++;
	}
	
	pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].Type = PET_CHG_AGE;
	pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].AttrValue.ChgAge  = (pstRolePet->Age -= pstResult->ResultVal1*PET_AGE_UNIT);
	pstPetChgDataList->AttrNum ++;
	
	pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].Type = PET_CHG_STAT;
	pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].AttrValue.ChgStatFlag = (pstRolePet->StatFlag &= ~PET_STAT_DEAD);
	pstPetChgDataList->AttrNum ++;

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}*/

/*

//  使用道具增加额外装备栏
int package_use_add_petarm_colgrid(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, tdr_ulonglong WID)
{	
	UNUSED(pstResult);
	ROLEPET *pstRolePet = NULL;
	PETARM *pstPetArms;    
	
	pstRolePet = find_package_shi(pstPlayer, WID,0);
	if (!pstRolePet)
	{
		return -1;
	}
	pstPetArms = &pstRolePet->PetArms;

	pstPetArms->AddedCount ++;
	pstPetArms->MaxNum ++;
	
	pet_armchange_to_clt(pstEnv, pstPlayer, pstRolePet);
	return 0;
}
*/

int package_use_add_skill_grid(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, tdr_ulonglong WID)
{
	ROLEPET *pstRolePet = NULL;
	PETSKILL *pstPetSkills;    
	int iSkillType;
	int iRet;
	UNUSED(pstResult);

	pstRolePet = z_find_role_pet(pstPlayer, WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}

	if (0 > (iRet = package_dec_money(pstEnv, pstPlayer, ATTR_ID_BULL, PET_ADD_SKILL_GRID_MONEY)))
	{
		return -1;
	}

	pstPetSkills = &pstRolePet->PetSkills;
	iSkillType = get_ran_skillgrid_type(pstRolePet);
	if (iSkillType)
	{
		pstPetSkills->SkillGrid[pstPetSkills->SkillGridNum++].GridType = iSkillType;
		pstPetSkills->AddSkillGridNum++;

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
				ZONEERR_PAK201, pstRolePet->Name);
		pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);

		pet_svr_act(pstEnv, pstPlayer, PET_SVR_ACT_ADD_SKILLGRID);
	}
	
	return 0;
}

int package_check_add_skill_grid(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, 
								ITEMDEF *pstItemDef,tdr_ulonglong WID)
{	
	ROLEPET *pstRolePet = NULL;
	PETDEF *pstPetDef = NULL;
	PETSKILL *pstPetSkills = NULL;  
	UNUSED(pstAppCtx);
	UNUSED(pstResult);
	UNUSED(pstItemDef);

	if (check_npc_ui(pstEnv, pstPlayer, UI_TYPE_PET_ADD_SKILL_GRID) < 0)
	{
		return -1;
	}
	
	pstRolePet = z_find_role_pet(pstPlayer, WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}	

	if(pstRolePet->StatFlag & PET_STAT_FIGHT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
						ZONEERR_PAK202, pstRolePet->Name);
		return -1;
	}

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (!pstPetDef)
	{
		return -1;
	}

	pstPetSkills = &pstRolePet->PetSkills;
	if (pstPetSkills->SkillGridNum >=  MAX_PET_SKILL_GRID || pstPetSkills->AddSkillGridNum >= MAX_PET_ADD_SKILL_GRID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK203, pstRolePet->Name);
		return -1;
	}

	if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, PET_ADD_SKILL_GRID_MONEY))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK204, MONEY_TYPE_MONEY_NAME);
		return -1;
	}
	
	return 0;
}

int package_check_pet_addexp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, 
								ITEMDEF *pstItemDef,tdr_ulonglong WID)
{	
	ROLEPET *pstRolePet = NULL;
	PETDEF *pstPetDef = NULL;
	UNUSED(pstResult);
	UNUSED(pstItemDef);	

	pstRolePet = z_find_role_pet(pstPlayer, WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}	
	
	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (!pstPetDef)
	{
		return -1;
	}

	if (!(pstRolePet->StatFlag &  PET_STAT_FIGHT) )
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK103 );
		return -1;
	}

	if (pstRolePet->Level >= MAX_PET_LEVEL)
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK104);
		return -1;
	}

	if (pstRolePet->Level - pstPlayer->stRoleData.Level >= PET_LEVEL_LIMIT)
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK105);
		return -1;
	}
	
	return 0;
}

int package_use_pet_addexp(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, tdr_ulonglong WID)
{
	LEVELUP *pstLevelUp = NULL;
	ROLEPET *pstRolePet = NULL;
	int iRet;

	pstRolePet = z_find_role_pet(pstPlayer, WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}	

	pstLevelUp = z_get_levelup(pstEnv, pstRolePet->Level);
	if (!pstLevelUp)
	{
		return -1;
	}

	iRet = z_add_pet_exp(pstEnv,pstPlayer, pstRolePet, pstLevelUp->ContributeExp/10000.0*pstResult->ResultVal1+pstResult->ResultVal2);
	if(iRet > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, 
				ZONEERR_PAK205, pstRolePet->Name, iRet);
	}

	return 0;
}

int package_check_addpower(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, 
								ITEMDEF *pstItemDef,tdr_ulonglong WID)
{	
	ROLEPET *pstRolePet = NULL;
	UNUSED(pstItemDef);
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	UNUSED(pstResult);	

	pstRolePet = z_find_role_pet(pstPlayer, WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}

	if (pstRolePet->HP >= pstRolePet->MaxHP)
	{
		return -1;
	}

	return 0;
}

int package_use_addpower(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, tdr_ulonglong WID)
{
	int iOldHP;
	ROLEPET *pstRolePet = NULL;
	int iAddHp = pstResult->ResultVal1;

	pstRolePet = z_find_role_pet(pstPlayer, WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}

	iOldHP = pstRolePet->HP;

	//宠物天赋,药品回血	
	pet_native_skill_result(pstEnv,pstPlayer,RESULT_NATIVE_ITEM_HP_ADD_MAP, &iAddHp,NULL);
	pet_add_power(pstEnv, pstPlayer, pstRolePet, iAddHp);

	z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, 
				ZONEERR_PAK206, pstRolePet->Name, pstRolePet->HP-iOldHP);
	
	return 0;
}

/*
int package_check_pet_add_attr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, 
								ITEMDEF *pstItemDef,tdr_ulonglong WID)
{	
	ROLEPET *pstRolePet = NULL;
	PETDEF *pstPetDef = NULL;
	UNUSED(pstAppCtx);
	UNUSED(pstResult);
	UNUSED(pstItemDef);

	if (check_npc_ui(pstEnv, pstPlayer, UI_TYPE_PET_ADD_ATTR) < 0)
	{
		return -1;
	}

	pstRolePet = z_find_role_pet(pstPlayer, WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}	

	if(pstRolePet->StatFlag & PET_STAT_FIGHT)
	{
		z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
						"您的宠物处于出战状态中，不能进行增强操作。");
		return -1;
	}
	
	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (!pstPetDef)
	{
		return -1;
	}

	switch(pstResult->ResultVal1)
	{
		case PET_ATTR_ICE_ATK :
			if (pstRolePet->IceAtkMul >= pstPetDef->MaxIceAtkMul)
			{
				z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, 
							"宠物%s魔法攻击增强次数已达到上限，不能继续增加", pstRolePet->Name);
				return -1;
			}
			break;
		case PET_ATTR_PHSIC_ATK:
			if (pstRolePet->PhsicAtkMul >= pstPetDef->MaxPhsicAtkMul)
			{
				z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, 
							"宠物%s物理攻击增强次数已达到上限，不能继续增加", pstRolePet->Name);
				return -1;
			}
			break;
		case PET_ATTR_ICE_DEF:
			if (pstRolePet->IceDefMul >= pstPetDef->MaxIceDefMul)
			{
				z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, 
							"宠物%s魔法防御增强次数已达到上限，不能继续增加", pstRolePet->Name);
				return -1;
			}
			break;
		case PET_ATTR_PHSIC_DEF:
			if (pstRolePet->PhsicDefMul >= pstPetDef->MaxPhsicDefMul)
			{
				z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, 
							"宠物%s物理防御增强次数已达到上限，不能继续增加", pstRolePet->Name);
				return -1;
			}
			break;
		case PET_ATTR_HP:
			if (pstRolePet->HPMul >= pstPetDef->MaxHPMul)
			{
				z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, 
							"宠物%s血量上限增强次数已达到上限，不能继续增加", pstRolePet->Name);
				return -1;
			}
			break;
		case PET_ATTR_FLEE:
			if (pstRolePet->FleeMul >= pstPetDef->MaxFleeMul)
			{
				z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, 
							"宠物%s闪避增强次数已达到上限，不能继续增加", pstRolePet->Name);
				return -1;
			}
			break;
		case PET_ATTR_HIT_RATE:
			if (pstRolePet->HitRateMul >= pstPetDef->MaxHitRateMul)
			{
				z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, 
							"宠物%s命中增强次数已达到上限，不能继续增加", pstRolePet->Name);
				return -1;
			}
			break;
		case PET_ATTR_HEAVY_HIT_RATE:
			if (pstRolePet->HeavyHitRateMul >= pstPetDef->MaxHeavyHitRateMul)
			{
				z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, 
							"宠物%s暴击增强次数已达到上限，不能继续增加", pstRolePet->Name);
				return -1;
			}
			break;
		default:
			return -1;
			break;
	}

	return 0;
}


int package_use_pet_add_attr(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, tdr_ulonglong WID)
{
	char szMsg[256];
	ROLEPET *pstRolePet = NULL;
	PETDEF *pstPetDef = NULL;

	pstRolePet = z_find_role_pet(pstPlayer, WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (!pstPetDef)
	{
		return -1;
	}

	switch(pstResult->ResultVal1)
	{
		case PET_ATTR_ICE_ATK :
			if (pstRolePet->IceAtkMul + pstResult->ResultVal2 >= pstPetDef->MaxIceAtkMul)
			{
				pstRolePet->IceAtkMul = pstPetDef->MaxIceAtkMul;
			}
			else
			{
				pstRolePet->IceAtkMul += pstResult->ResultVal2;
			}

			snprintf(szMsg,sizeof(szMsg),"宠物%s 魔法攻击上升", pstRolePet->Name);
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, szMsg);
			break;
		case PET_ATTR_PHSIC_ATK:
			if (pstRolePet->PhsicAtkMul + pstResult->ResultVal2 >= pstPetDef->MaxPhsicAtkMul)
			{
				pstRolePet->PhsicAtkMul = pstPetDef->MaxPhsicAtkMul;
			}
			else
			{
				pstRolePet->PhsicAtkMul += pstResult->ResultVal2;
			}

			snprintf(szMsg,sizeof(szMsg),"宠物%s 物理攻击上升", pstRolePet->Name);
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, szMsg);
			break;
		case PET_ATTR_ICE_DEF:
			if (pstRolePet->IceDefMul + pstResult->ResultVal2 >= pstPetDef->MaxIceDefMul)
			{
				pstRolePet->IceDefMul = pstPetDef->MaxIceDefMul;
			}
			else
			{
				pstRolePet->IceDefMul += pstResult->ResultVal2;
			}
			snprintf(szMsg,sizeof(szMsg),"宠物%s 魔法防御上升", pstRolePet->Name);
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, szMsg);
			break;
		case PET_ATTR_PHSIC_DEF:
			if (pstRolePet->PhsicDefMul + pstResult->ResultVal2 >= pstPetDef->MaxPhsicDefMul)
			{
				pstRolePet->PhsicDefMul = pstPetDef->MaxPhsicDefMul;
			}
			else
			{
				pstRolePet->PhsicDefMul += pstResult->ResultVal2;
			}

			snprintf(szMsg,sizeof(szMsg),"宠物%s 物理防御上升", pstRolePet->Name);
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, szMsg);
			break;
		case PET_ATTR_HP:
			if (pstRolePet->HPMul + pstResult->ResultVal2 >= pstPetDef->MaxHPMul)
			{
				pstRolePet->HPMul = pstPetDef->MaxHPMul;
			}
			else
			{
				pstRolePet->HPMul += pstResult->ResultVal2;
			}
			snprintf(szMsg,sizeof(szMsg),"宠物%s 血量上限上升", pstRolePet->Name);
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, szMsg);
			break;
		case PET_ATTR_FLEE:
			if (pstRolePet->FleeMul + pstResult->ResultVal2 >= pstPetDef->MaxFleeMul)
			{
				pstRolePet->FleeMul = pstPetDef->MaxFleeMul;
			}
			else
			{
				pstRolePet->FleeMul += pstResult->ResultVal2;
			}

			snprintf(szMsg,sizeof(szMsg),"宠物%s 闪避上升", pstRolePet->Name);
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, szMsg);
			break;
		case PET_ATTR_HIT_RATE:
			if (pstRolePet->HitRateMul + pstResult->ResultVal2 >= pstPetDef->MaxHitRateMul)
			{
				pstRolePet->HitRateMul = pstPetDef->MaxHitRateMul;
			}
			else
			{
				pstRolePet->HitRateMul += pstResult->ResultVal2;
			}

			snprintf(szMsg,sizeof(szMsg),"宠物%s 命中上升", pstRolePet->Name);
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, szMsg);
			break;
		case PET_ATTR_HEAVY_HIT_RATE:
			if (pstRolePet->HeavyHitRateMul + pstResult->ResultVal2 >= pstPetDef->MaxHeavyHitRateMul)
			{
				pstRolePet->HeavyHitRateMul = pstPetDef->MaxHeavyHitRateMul;
			}
			else
			{
				pstRolePet->HeavyHitRateMul += pstResult->ResultVal2;
			}
			
			snprintf(szMsg,sizeof(szMsg),"宠物%s 暴击率上升", pstRolePet->Name);
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, szMsg);
			break;
		default:
			return -1;
			break;
	}

	pet_update_online(pstEnv, pstPlayer, pstRolePet);
	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);

	z_role_pet_oplog( pstEnv, pstPlayer, pstRolePet, NULL, OP_PET_ADD_ATTR, NULL);
	
	return 0;
}
*/

int package_check_pet_add_loveval(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, 
								ITEMDEF *pstItemDef,tdr_ulonglong WID)
{	
	ROLEPET *pstRolePet = NULL;
	PETDEF *pstPetDef = NULL;
	int iMaxLoveVal = MAX_PET_LOVE_VAL;
	UNUSED(pstResult);
	UNUSED(pstItemDef);

	pstRolePet = z_find_role_pet(pstPlayer, WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}	
	
	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (!pstPetDef)
	{
		return -1;
	}
	//宠物天赋,最大忠诚度
	pet_native_skill_result(pstEnv,pstPlayer,RESULT_NATIVE_MAX_LOVE_VAL_ADD_MAP, &iMaxLoveVal,pstRolePet);
	if(pstRolePet->LoveVal >= iMaxLoveVal)
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK106);
		return -1;
	}

	return 0;
}

int package_use_pet_add_loveval(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, tdr_ulonglong WID)
{
	ROLEPET *pstRolePet = NULL;

	pstRolePet = z_find_role_pet(pstPlayer, WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}

	pet_love_val_chg(pstEnv, pstPlayer, pstRolePet, pstResult->ResultVal1);
	
	return 0;
}

/*
int booty_pick(ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstDropItem, CSPKG *pstPkgRes)
{
	int iIdx = 0;
	CSPACKAGEINFO *pstPakInfo = &pstPkgRes->Body.PackageInfo;
	CSBOOTYINFO *pstCSBooty = NULL;
	ZoneRoleBooty *pstRoleBooty = NULL;

	if(pstPlayer->stOnline.State & CS_STAT_DEAD)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "死亡状态,无法拾取物品");
		return -1;
	}

	if (!z_pos_in_view(&pstPlayer->stRoleData.Pos, &pstDropItem->stPos))
	{
		return -1;
	}
	
	iIdx = mon_booty_owner(&pstDropItem->stObj.stBootyPak, pstPlayer);
	if (0 > iIdx)
	{
		return -1;
	}
	pstRoleBooty = &pstDropItem->stObj.stBootyPak.astRoleBooty[iIdx];
	
	pstPakInfo->Type = PACKAGE_INFO_BOOTY;
	pstCSBooty = &pstPakInfo->PackageInfoData.PackageBooty;

	pstCSBooty->ID = pstDropItem->iID;
	pstCSBooty->Num = pstRoleBooty->bNum;
	memcpy(pstCSBooty->Items, pstRoleBooty->astGrid, pstCSBooty->Num*sizeof(pstCSBooty->Items[0]));

	Z_CSHEAD_INIT(&pstPkgRes->Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkgRes);

	return 0;
}
*/

int booty_money_pick(ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstDropItem)
{
	int iType;
	ROLEGRID *pstRoleGrid, stDummy;
	ZoneTeam *pstZoneTeam;
	int i;
	int iMoneyVal;
	int iValidTeamNum;
	Player *pstPlayerTmp;
	Player *apstPlayer[MAX_TEAM_MEMBER];
		
	pstRoleGrid = &pstDropItem->stObj.stBootyGrid.stGrid;
	// OP LOG
	memcpy(&stDummy, pstRoleGrid, sizeof(stDummy));

	if(pstRoleGrid->GridData.RoleItem.DefID == BOOTY_MONEY_ID)
	{
		iType = ATTR_ID_MONEY;
	}
	else if(pstRoleGrid->GridData.RoleItem.DefID == BOOTY_BULL_ID)
	{
		iType = ATTR_ID_BULL;
	}
	else
	{
		return -1;
	}

	iValidTeamNum = 0;
	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	//if(pstZoneTeam && pstZoneTeam->stTeamInfo.TeamID == pstDropItem->stObj.stBootyGrid.ullTeamID)
	if(pstZoneTeam)
	{
		for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
		{
			pstPlayerTmp = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
			if (NULL == pstPlayerTmp)
			{
				continue;
			}

			if(pstPlayerTmp != pstPlayer)
			{
				//有效分配的队友
				if (PLAYER_STATUS_ROLE_LOGIN != pstPlayerTmp->eStatus || 
					pstPlayerTmp->stRoleData.Map != pstPlayer->stRoleData.Map || pstPlayerTmp->stOnline.State & CS_STAT_DEAD)
				{
					continue;
				}

				//副本全地图拾取都会分得到
				if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE &&
					!z_pos_in_view(&pstPlayerTmp->stRoleData.Pos, &pstPlayer->stRoleData.Pos))
				{
					continue;
				}
			}
			
			apstPlayer[iValidTeamNum++] = pstPlayerTmp;	
		}

		iMoneyVal = pstRoleGrid->GridData.RoleItem.Flag/iValidTeamNum;
	}
	else
	{
		apstPlayer[iValidTeamNum++] = pstPlayer;	
		iMoneyVal = pstRoleGrid->GridData.RoleItem.Flag/iValidTeamNum;
	}

	for (i=0; i<iValidTeamNum; i++)
	{
		if (0 > package_add_money_test(pstEnv, apstPlayer[i], iType, iMoneyVal))
		{
			char szBuff[32];
				
			if (money_type_to_string(iType, szBuff, sizeof(szBuff)) < 0 )
			{
				strcpy(szBuff, "金钱");
			}
			
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, apstPlayer[i], SYS_MIDDLE, ZONEERR_PAK207, szBuff);
			continue;
		}

		package_add_money(pstEnv, apstPlayer[i], iType, iMoneyVal);

		if (iType == ATTR_ID_MONEY)
		{
			z_money_create_oplog(pstEnv, apstPlayer[i], MONEY_OPTYPE_PICK, pstDropItem->stObj.stBootyGrid.iFromID, 0, iMoneyVal, "拾取");
		}
		else
		{
			z_money_create_oplog(pstEnv, apstPlayer[i], MONEY_OPTYPE_PICK, pstDropItem->stObj.stBootyGrid.iFromID, iMoneyVal, 0, "拾取");
		}

		range_single_set(pstEnv, apstPlayer[i], RANGE_MAIN_SINGLE_SUB_MONEY, 
						apstPlayer[i]->stRoleData.RoleDetail.Money+apstPlayer[i]->stRoleData.Package.StashPackage.Money);
	}

	z_item_clean(pstEnv, pstDropItem);
	z_role_pickitem_oplog( pstEnv, pstPlayer, &stDummy, 1, OP_PICK );

	return 0;
}


int is_money_booty_grid(ROLEGRID *pstRoleGrid)
{
	if(pstRoleGrid->GridData.RoleItem.DefID == BOOTY_MONEY_ID ||
		pstRoleGrid->GridData.RoleItem.DefID == BOOTY_BULL_ID)
	{
		return 1;
	}

	return 0;
}

int booty_grid_pick(ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstDropItem)
{
	int iOldNum = 0;
	int iNum = 0;
	ROLEGRID *pstRoleGrid, stDummy;
	ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);
	ZONEMAPINST *pstMapInst = NULL;
	RESULT *pstResult = NULL;
	ITEMDEF *pstItemDef = NULL;

	if(pstPlayer->stOnline.State & CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,  ZONEERR_PAK107);
		return -1;
	}

	if (0 > z_check_op_dist(pstEnv, pstPlayer, pstPlayer->stRoleData.Map, &pstDropItem->stPos, MAX_PICK_DIST +100))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PAK108);
		return -1;
	}

	if (!z_pos_in_view(&pstPlayer->stRoleData.Pos, &pstDropItem->stPos))
	{
		return -1;
	}

	if(is_collect_drop_item(pstDropItem))
	{
		return -1;
	}

	if(is_booty_grid_owner(pstPlayer, pstDropItem))
	{
		if(is_wait_roll_drop_item(pstDropItem) && 
			pstDropItem->tPickLimitTime > pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			return player_roll_booty_start(pstEnv, pstPlayer, pstDropItem);
		}
		else if(is_rolling_drop_item(pstDropItem))
		{
			if(pstDropItem->tPickLimitTime > pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				return -1;
			}
		}
	}
	else
	{
		if(pstDropItem->tPickLimitTime > pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK109 );
			return -1;
		}
	}
	
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstDropItem->stMap);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstRoleGrid = &pstDropItem->stObj.stBootyGrid.stGrid;

	//金钱
	if(is_money_booty_grid(pstRoleGrid))
	{
		booty_money_pick(pstEnv, pstPlayer, pstDropItem);
		return 0;
	}
	pstItemDef = z_find_itemdef(pstEnv,pstRoleGrid->GridData.RoleItem.DefID);
	if (pstItemDef)
	{
		pstResult = z_item_get_result(pstItemDef, RESULT_CLAN_EVERYDAY_PWORLD_SCORE);
		if (pstResult)
		{
			memcpy(&stDummy, pstRoleGrid, sizeof(stDummy));
			
			player_clan_everyday_pworld_val_chg(pstEnv,pstPlayer,pstResult->ResultVal1);
			z_item_clean(pstEnv, pstDropItem);
			z_role_pickitem_oplog( pstEnv, pstPlayer, &stDummy, 1, OP_PICK );
			return 0;
		}

		pstResult = z_item_get_result(pstItemDef, RESULT_VIGOR);
		if (pstResult && pstResult->ResultVal1 == ITEM_PICK_ATTR)
		{
			if (0 == player_add_vig_check(pstEnv,pstPlayer))
			{
				player_add_vig(pstEnv,pstPlayer,pstResult->ResultVal2, 0,1);
				memcpy(&stDummy, pstRoleGrid, sizeof(stDummy));
						
				z_item_clean(pstEnv, pstDropItem);
				z_role_pickitem_oplog( pstEnv, pstPlayer, &stDummy, 1, OP_PICK );
			}
			return 0;
		}
	}

	switch (pstRoleGrid->Type)
	{
		case GRID_ITEM_ARM:
			iOldNum = pstRoleGrid->GridData.RoleArm.Num;
			break;
		case GRID_ITEM_ITEM:
			iOldNum = pstRoleGrid->GridData.RoleItem.Num;
			break;
		default:
			break;
	}

	// OP LOG
	memcpy(&stDummy, pstRoleGrid, sizeof(stDummy));

	if ( 0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRoleGrid, 1,0) )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_NOSPACE);
		return -1;
	}
	
	package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRoleGrid, 1,0, IMPITEM_OPTYPE_PICK);
	
	switch (pstRoleGrid->Type)
	{
		case GRID_ITEM_ARM:
			iNum = pstRoleGrid->GridData.RoleArm.Num;
			stDummy.GridData.RoleArm.Num = iOldNum - iNum;
			break;
		case GRID_ITEM_ITEM:
			iNum = pstRoleGrid->GridData.RoleItem.Num;
			stDummy.GridData.RoleItem.Num = iOldNum - iNum;
			break;
		default:
			break;
	}

	if (pstItemDef && pstRoleGrid->Type == GRID_ITEM_ARM && pstItemDef->Important >= 5)
	{
		CSPKG stPkgLink;
		z_fill_link_msg(&stPkgLink, SYS_SHOW, CHAT_LINK_ARM, 
						(void *)&pstRoleGrid->GridData.RoleArm, 
						ZONEERR_PLAYER804,pstPlayer->stRoleData.RoleName);
		
		if ( pstEnv->pstConf->CountryVer != COUNTRY_VER_KOREA)
		{
			z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkgLink);
		}
	}
	else
	{
		package_add_msg_show(pstEnv,pstPlayer->stRoleData.RoleName, pstItemDef,pstRoleGrid);
	}

	if (iOldNum > iNum)
	{
		z_role_pickitem_oplog( pstEnv, pstPlayer, &stDummy, 1, OP_PICK );
	}

	if (iOldNum > iNum && pstDropItem->stObj.stBootyGrid.ullTeamID != 0)
	{
		if (pstTeam && pstTeam->stTeamInfo.TeamID == pstDropItem->stObj.stBootyGrid.ullTeamID)
		{
			package_booty_notify_team(pstEnv->pstAppCtx, pstEnv, pstTeam, pstPlayer, pstRoleGrid, iOldNum - iNum);
		}
	}

	if (0 >= iNum)
	{
		z_item_clean(pstEnv, pstDropItem);
	}

	return 0;
}

int tmp_pick(ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstDropItem, CSPKG *pstPkgRes)
{
	ITEMDEF *pstItemDef = NULL;
	CSACTIONRES *pstActionRes = &pstPkgRes->Body.ActionRes;

	if (z_distance(&pstPlayer->stRoleData.Pos, &pstDropItem->stPos) > 5*DIST_TORRENT)
	{
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player %d pos(%d, %d), dropitempos(%d,%d)",
					pstPlayer->stRoleData.Uin, pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y, 
					pstDropItem->stPos.X, pstDropItem->stPos.Y);
		return -1;
	}

	pstItemDef = z_find_itemdef(pstEnv, pstDropItem->iDefIdx);
	if (NULL == pstItemDef || ITEM_TMP_NO == pstItemDef->ItemTmp.Type)
	{
		return -1;
	}

	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOITEM))
	{
		return -1;
	}
	
	if (0 == z_player_act_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, RES_ACTION_USEITEM))
	{
		return -1;
	}

	if ( pstPlayer->stOnline.State & (CS_STAT_RIDE_ON|CS_STAT_FLY | CS_STAT_HITCH_RIDE))
	{
		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, "骑乘状态不能拾取临时机械");
		return -1;
	}

	if (ITEM_TMP_SUPPLY == pstItemDef->ItemTmp.Type)
	{
		if (item_result(pstEnv, pstPlayer, pstItemDef, NULL, NULL,NULL) < 0)
		{
			return -1;
		}

		pstActionRes->ID = pstPlayer->iMemID;
		pstActionRes->ActionID = ACTION_TMP;
		pstActionRes->Data.TmpItem.TmpItemID= pstItemDef->ItemID;
		pstActionRes->Data.TmpItem.OperType = 0;
		pstActionRes->Data.TmpItem.MemIndex = pstDropItem->iMIndex;
		Z_CSHEAD_INIT(&pstPkgRes->Head, ACTION_RES);
		z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, pstPkgRes, 0);	

		z_item_leave_map(pstEnv->pstAppCtx, pstEnv, pstDropItem);

		
		if(pstItemDef->ItemID == 1460310)
		{
			z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer,SYS_MIDDLE, ZONEERR_PACKAGE_302);
		}

		if(pstItemDef->ItemID == 1460311)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE, ZONEERR_PACKAGE_303);
		}

		if(pstItemDef->ItemID == 1460313)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE, ZONEERR_PACKAGE_304);
		}

		if(pstItemDef->ItemID == 1460312)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE, ZONEERR_PACKAGE_305);
		}		

		if(pstItemDef->ItemID == 1460314)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE, ZONEERR_PACKAGE_306);
		}	

		if(pstItemDef->ItemID == 1460315)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE, ZONEERR_PACKAGE_307);
		}	
	}
	else if ( ITEM_TMP_MACHINE == pstItemDef->ItemTmp.Type)
	{	
		if (0 == tmp_item_add(pstEnv, pstPlayer, pstDropItem, pstItemDef))
		{
			z_item_leave_map(pstEnv->pstAppCtx, pstEnv, pstDropItem);
		}
	}
	else
	{
		if (0 == tmp_item_add(pstEnv, pstPlayer, pstDropItem, pstItemDef))
		{
			z_item_leave_map(pstEnv->pstAppCtx, pstEnv, pstDropItem);
		}
	}

	return 0;
}

int package_pick(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSPACKAGEPICK *pstPick)
{
	CSPKG *pstPkgRes = &pstEnv->stCSRes;
	DropItem *pstDropItem = NULL;

	pstDropItem = z_id_dropitem(pstAppCtx, pstEnv, pstPick->ID);
	if (NULL == pstDropItem)
	{
		return -1;
	}

	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstAppCtx, pstEnv, pstPlayer, 0);
	}

	if(z_player_find_status(pstPlayer, ENTER_PWORLD_SAFE_STATUSID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_TIP, ZONEERR_PAK110);
		return -1;
	}
	
	if (DROP_ITEM_TMP == pstDropItem->cType)
	{
		tmp_pick(pstEnv, pstPlayer, pstDropItem, pstPkgRes);
	}
	if (DROP_ITEM_GRID == pstDropItem->cType)
	{
		booty_grid_pick(pstEnv, pstPlayer, pstDropItem);
	}
	else
	{
		return -1;
	}
	
	return 0;
}

int package_booty_notify_team(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZoneTeam *pstTeam, Player *pstPlayer,
									ROLEGRID *pstRoleGrid, int iNum)
{	
	ITEMDEF *pstItemDef = NULL;
	CSPKG stPkg;
	CSSYSMSG *pstSysMsg = &stPkg.Body.SysMsg;
	int iSysType;
	char szMsg[256];
	int iLinkItemType = CHAT_LINK_ITEM;
	UNUSED(pstAppCtx);

	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstRoleGrid->GridData.RoleItem);
	if (NULL == pstItemDef || 0 == pstItemDef->Important)
	{
		return -1;
	}

	switch(pstItemDef->ItemType)
	{
		case ITEM_TYPE_ITEM:
			iSysType = SYS_MSG_ITEM;
			break;
		case ITEM_TYPE_ARM:
			iSysType = SYS_MSG_ARM;
			iLinkItemType = CHAT_LINK_ARM;
			break;
		case ITEM_TYPE_STUFF:
			iSysType = SYS_MSG_STUFF;
			break;
		case ITEM_TYPE_TASK:
			iSysType = SYS_MSG_TASK;
			break;
		default:
			iSysType = SYS_COMBAT;
			break;
	}

	pstSysMsg->Type = iSysType;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
	
	snprintf(szMsg, sizeof(szMsg), LJY_SYSMSG7, pstPlayer->stRoleData.RoleName, iNum, pstItemDef->Name);
	szMsg[sizeof(szMsg) - 1] = 0;
	STRNCPY(pstSysMsg->Msg, szMsg, sizeof(pstSysMsg->Msg));
	
	Z_CSHEAD_INIT(&stPkg.Head, SYS_MSG);
	player_team_broadcast_except(pstEnv, pstTeam, &stPkg, pstPlayer);	

	
	if(pstItemDef->Important < 3 )
	{
		z_fill_link_msg(&stPkg, SYS_MSG_CHAT_TIP, iLinkItemType, (void *)&pstRoleGrid->GridData.RoleItem,
			ZONEERR_LINK307, pstPlayer->stRoleData.RoleName, iNum);
	}
	else
	{
		z_fill_link_msg(&stPkg, SYS_TIP, iLinkItemType, (void *)&pstRoleGrid->GridData.RoleItem,
			ZONEERR_LINK307, pstPlayer->stRoleData.RoleName, iNum);
	}
	
	player_team_broadcast_except(pstEnv, pstTeam, &stPkg, pstPlayer);	
	
	return 0;
}

/*
int package_booty_selectone(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstDropItem, ZoneRoleBooty *pstRoleBooty,
									int iIdx)
{
	int iOldNum = 0;
	int iNum = 0;
	ROLEGRID *pstRoleGrid, stDummy;
	ZoneTeam *pstTeam =NULL;

	pstRoleGrid = pstRoleBooty->astGrid + iIdx;

	switch (pstRoleGrid->Type)
	{
		case GRID_ITEM_ARM:
			iOldNum = pstRoleGrid->GridData.RoleArm.Num;
			break;
		case GRID_ITEM_ITEM:
			iOldNum = pstRoleGrid->GridData.RoleItem.Num;
			break;
		default:
			break;
	}

	// OP LOG
	memcpy(&stDummy, pstRoleGrid, sizeof(stDummy));
	if ( 0 > package_add(pstAppCtx, pstEnv, pstPlayer, pstRoleGrid, 1,0) )
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, "背包已满,无法拾取物品");
		//return -1;
	}
	
	switch (pstRoleGrid->Type)
	{
		case GRID_ITEM_ARM:
			iNum = pstRoleGrid->GridData.RoleArm.Num;
			stDummy.GridData.RoleArm.Num = iOldNum - iNum;
			break;
		case GRID_ITEM_ITEM:
			iNum = pstRoleGrid->GridData.RoleItem.Num;
			stDummy.GridData.RoleItem.Num = iOldNum - iNum;
			break;
		default:
			break;
	}

	if (iOldNum > iNum)
	{
		z_role_pickitem_oplog( pstEnv, pstPlayer, &stDummy, 1, OP_PICK );
	}

	if (iOldNum > iNum && pstDropItem->ullTeamID != 0)
	{
		pstTeam = player_team_get(pstEnv, pstPlayer);

		if (pstTeam && pstTeam->stTeamInfo.TeamID == pstDropItem->ullTeamID)
		{
			package_booty_notify_team(pstAppCtx, pstEnv, pstTeam, pstPlayer, pstRoleGrid, iOldNum - iNum);
		}
	}

	if (0 >= iNum)
	{
		pstRoleBooty->bNum--;

		if (iIdx != pstRoleBooty->bNum)
		{
			memmove(pstRoleGrid, pstRoleGrid+1, (pstRoleBooty->bNum - iIdx) * sizeof(*pstRoleGrid));
		}
	}

	return 0;
}


int package_booty_selectall(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstDropItem, ZoneRoleBooty *pstRoleBooty)
{
	int i = 0;
		
	for (i=pstRoleBooty->bNum -1 ; i>=0; i--)
	{
		package_booty_selectone(pstAppCtx, pstEnv, pstPlayer, pstDropItem, pstRoleBooty, i);
	}
		
	return 0;
}

int package_booty_select(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
								CSPACKAGEPICKSELECT *pstPickSelect)
{
	int iIdx = 0;
	DropItem *pstDropItem = NULL;
	ZoneBootyPak *pstZoneBooty = NULL;
	ZoneRoleBooty *pstRoleBooty = NULL;
	ZONEMAPINST *pstMapInst = NULL;

	if(pstPlayer->stOnline.State & CS_STAT_DEAD)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "死亡状态,无法拾取物品");
		return -1;
	}

	pstDropItem = z_id_dropitem(pstAppCtx, pstEnv, pstPickSelect->ID);
	if (NULL == pstDropItem)
	{
		return -1;
	}

	if (DROP_ITEM_BOOTY == pstDropItem->cType)
	{
		if (!z_pos_in_view(&pstPlayer->stRoleData.Pos, &pstDropItem->stPos))
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}

	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstDropItem->stMap);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstZoneBooty = &pstDropItem->stObj.stBootyPak;

	iIdx = mon_booty_owner(pstZoneBooty, pstPlayer);
	if (0 > iIdx)
	{
		return -1;
	}

	pstRoleBooty = &pstZoneBooty->astRoleBooty[iIdx];

	if ( -1 == pstPickSelect->Idx)
	{		
		package_booty_selectall(pstAppCtx, pstEnv, pstPlayer, pstDropItem, pstRoleBooty);
	}
	else
	{
		if (0 > pstPickSelect->Idx || pstPickSelect->Idx >= pstRoleBooty->bNum)
		{
			return -1;
		}
		package_booty_selectone(pstAppCtx, pstEnv, pstPlayer, pstDropItem, pstRoleBooty, pstPickSelect->Idx);
	}

	if (0 == pstRoleBooty->bNum)
	{
		pstZoneBooty->iRoleNum--;
		if (iIdx != pstZoneBooty->iRoleNum)
		{
			memcpy(&pstZoneBooty->astRoleBooty[iIdx], &pstZoneBooty->astRoleBooty[pstZoneBooty->iRoleNum],
					sizeof(pstZoneBooty->astRoleBooty[0]));
		}

		z_obj_leave_view(pstAppCtx, pstEnv, pstMapInst, pstPlayer, 0, pstDropItem->iID, OBJ_ITEM, 0);

		if (0 == pstZoneBooty->iRoleNum)
		{
			z_item_clean(pstEnv, pstDropItem);
		}
	}
	else
	{
		CSPKG *pstPkgRes = &pstEnv->stCSRes;
		CSPACKAGEINFO *pstPakInfo = &pstPkgRes->Body.PackageInfo;
		CSBOOTYINFO *pstCSBooty;
		ZoneRoleBooty *pstRoleBooty;

		pstRoleBooty = &pstDropItem->stObj.stBootyPak.astRoleBooty[iIdx];
		
		pstPakInfo->Type = PACKAGE_INFO_BOOTY;
		pstCSBooty = &pstPakInfo->PackageInfoData.PackageBooty;

		pstCSBooty->ID = pstDropItem->iID;
		pstCSBooty->Num = pstRoleBooty->bNum;
		memcpy(pstCSBooty->Items, pstRoleBooty->astGrid, pstCSBooty->Num*sizeof(pstCSBooty->Items[0]));

		Z_CSHEAD_INIT(&pstPkgRes->Head, PACKAGE_INFO);
		z_cltmsg_send(pstEnv, pstPlayer, pstPkgRes);
	}
	
	return 0;
}
*/


//打开仓库
int open_stash(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iID)
{	
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	UNUSED(pstAppCtx);
	
	pstPlayer->stOnline.State |= CS_STAT_STASH;

	pstActionRes->ID = iID;
	pstActionRes->ActionID = ACTION_UI;
	pstActionRes->Data.UI.UIType = UI_TYPE_STASH;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int open_coin_to_bull_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iID)
{	
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	UNUSED(pstAppCtx);
	
	//pstPlayer->stOnline.State |= CS_STAT_STASH;

	pstActionRes->ID = iID;
	pstActionRes->ActionID = ACTION_UI;
	pstActionRes->Data.UI.UIType = UI_TYPE_CHANGE_COIN_INTO;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int close_stash(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	pstPlayer->stOnline.State &= ~CS_STAT_STASH;

	return 0;	
}

int package_enlarge(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iListType, int iNum)
{
	CSPKG stPkg;
	int iTotal = 0, iOld = 0;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	CSPAKENLARGE *pstPakEnlarge = &pstActionRes->Data.PakEnlarge;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	UNUSED(pstAppCtx);

	if (iNum <= 0)
	{
		return -1;
	}
	
	switch(iListType)
	{
		case LIST_TYPE_AMM:
			iOld = pstPak->AmmPackage.MaxNum;
			if (iOld+iNum>MAX_AMM_PACKAGE)
			{
				pstPak->AmmPackage.MaxNum = MAX_AMM_PACKAGE;
			}
			else
			{
				pstPak->AmmPackage.MaxNum = iOld+iNum;
			}

			iTotal = pstPak->AmmPackage.MaxNum ;
			if (iTotal > iOld)
			{
				memset(pstPak->AmmPackage.RoleItems+iOld, 0, sizeof(pstPak->AmmPackage.RoleItems[0])*(iTotal-iOld));
			}
		
			break;
		case LIST_TYPE_STUFF:
			iOld = pstPak->StuffPackage.MaxNum;
			if (iOld+iNum>MAX_STUFF_PACKAGE)
			{
				pstPak->StuffPackage.MaxNum = MAX_STUFF_PACKAGE;
			}
			else
			{
				pstPak->StuffPackage.MaxNum = iOld+iNum;
			}

			iTotal = pstPak->StuffPackage.MaxNum ;
			if (iTotal > iOld)
			{
				memset(pstPak->StuffPackage.RoleItems+iOld, 0, sizeof(pstPak->StuffPackage.RoleItems[0])*(iTotal-iOld));
			}
			break;
		case LIST_TYPE_TASK:
			iOld = pstPak->TaskPackage.MaxNum;
			if (iOld+iNum>MAX_TASK_PACKAGE)
			{
				pstPak->TaskPackage.MaxNum = MAX_TASK_PACKAGE;
			}
			else
			{
				pstPak->TaskPackage.MaxNum = iOld+iNum;
			}

			iTotal = pstPak->TaskPackage.MaxNum ;
			if (iTotal > iOld)
			{
				memset(pstPak->TaskPackage.RoleItems+iOld, 0, sizeof(pstPak->TaskPackage.RoleItems[0])*(iTotal-iOld));
			}
			break;
		case LIST_TYPE_SUNDRIES:
			iOld = pstPak->SundriesPackage.MaxNum;
			if (iOld+iNum>MAX_SUNDRIES_PACKAGE)
			{
				pstPak->SundriesPackage.MaxNum = MAX_SUNDRIES_PACKAGE;
			}
			else
			{
				pstPak->SundriesPackage.MaxNum = iOld+iNum;
			}

			iTotal = pstPak->SundriesPackage.MaxNum ;
			if (iTotal > iOld)
			{
				memset(pstPak->SundriesPackage.RoleGrids+iOld, 0, sizeof(pstPak->SundriesPackage.RoleGrids[0])*(iTotal-iOld));
			}
			break;
		case LIST_TYPE_STASH:
			iOld = pstPak->StashPackage.MaxNum;
			if (iOld+iNum>MAX_STASH_PACKAGE)
			{
				pstPak->StashPackage.MaxNum = MAX_STASH_PACKAGE;
			}
			else
			{
				pstPak->StashPackage.MaxNum = iOld+iNum;
			}

			iTotal = pstPak->StashPackage.MaxNum ;
			if (iTotal > iOld)
			{
				memset(pstPak->StashPackage.RoleGrids+iOld, 0, sizeof(pstPak->StashPackage.RoleGrids[0])*(iTotal-iOld));
			}
			break;
		/*
		case LIST_TYPE_XIAN:
			iOld = pstPak->PetPackage.MaxXianNum;
			if (iOld+iNum>MAX_PET_XIAN_PACKAGE)
			{
				pstPak->PetPackage.MaxXianNum = MAX_PET_XIAN_PACKAGE;
			}
			else
			{
				pstPak->PetPackage.MaxXianNum = iOld+iNum;
			}

			iTotal = pstPak->PetPackage.MaxXianNum;
			if (iTotal > iOld)
			{
				memset(pstPak->PetPackage.RoleXianPets+iOld, 0, sizeof(pstPak->PetPackage.RoleXianPets[0])*(iTotal-iOld));
			}
			break;
		*/
		case LIST_TYPE_SHI:
			iOld = pstPak->PetPackage.MaxNum;
			if (iOld+iNum>MAX_PET_PACKAGE)
			{
				pstPak->PetPackage.MaxNum= MAX_PET_PACKAGE;
			}
			else
			{
				pstPak->PetPackage.MaxNum= iOld+iNum;
			}

			iTotal = pstPak->PetPackage.MaxNum;
			if (iTotal > iOld)
			{
				memset(pstPak->PetPackage.RolePets+iOld, 0, sizeof(pstPak->PetPackage.RolePets[0])*(iTotal-iOld));
			}
			break;
		default:
			return -1;
			break;
	}

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_PAK_ENLARGE;
	pstPakEnlarge->ListType = iListType;
	pstPakEnlarge->Num = iTotal;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send( pstEnv, pstPlayer, &stPkg);
		
	return 0;
}

int z_wear_pos_rep(ITEMDEF *pstItemDef1, ITEMDEF *pstItemDef2, unsigned char bWearPos1, unsigned char bWearPos2)
{
	char sMap[WEAR_POS_MAX+1];

	memset(sMap, 0, sizeof(sMap));

	if (pstItemDef1->WearPos1 == WEAR_POS_RING || 
		pstItemDef1->WearPos1 == WEAR_POS_AMULET ||
		pstItemDef1->WearPos1 == WEAR_POS_FASHION_BODY ||
		pstItemDef1->WearPos1 == WEAR_POS_FASHION_BACK ||
		pstItemDef1->WearPos1 == WEAR_POS_FASHION_HEAD ||
		pstItemDef1->WearPos1 == WEAR_POS_FASHION_HAIR ||
		pstItemDef1->WearPos1 == WEAR_POS_FASHION_SHOES ||
		pstItemDef1->WearPos1 == WEAR_POS_FASHION_HAND ||
		pstItemDef1->WearPos1 == WEAR_POS_FASHION_LEG)
	{
		sMap[bWearPos1] = 1;
	}
	else
	{
		if (pstItemDef1->WearPos1 != WEAR_POS_NO)
		{
			sMap[pstItemDef1->WearPos1] = 1;
		}
		if (pstItemDef1->WearPos2 != WEAR_POS_NO)
		{
			sMap[pstItemDef1->WearPos2] = 1;
		}
		if (pstItemDef1->WearPos3 != WEAR_POS_NO)
		{
			sMap[pstItemDef1->WearPos3] = 1;
		}
	}

	if (pstItemDef2->WearPos1 == WEAR_POS_RING || 
		pstItemDef2->WearPos1 == WEAR_POS_AMULET ||
		pstItemDef2->WearPos1 == WEAR_POS_FASHION_BODY ||
		pstItemDef2->WearPos1 == WEAR_POS_FASHION_BACK ||
		pstItemDef2->WearPos1 == WEAR_POS_FASHION_HEAD ||
		pstItemDef2->WearPos1 == WEAR_POS_FASHION_HAIR ||
		pstItemDef2->WearPos1 == WEAR_POS_FASHION_SHOES ||
		pstItemDef2->WearPos1 == WEAR_POS_FASHION_HAND ||
		pstItemDef2->WearPos1 == WEAR_POS_FASHION_LEG)
	{
		if (sMap[bWearPos2])
		{
			return 1;
		}
	}
	else
	{
		if (sMap[pstItemDef2->WearPos1] || sMap[pstItemDef2->WearPos2] || sMap[pstItemDef2->WearPos3])
		{
			return 1;
		}
	}

	return 0;
}

int z_wear_extended_bit_set(ZONESVRENV *pstEnv, Player *pstPlayer, int iWearIdx)
{
	const ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	const ROLEARM *pstArm = NULL;

	if(iWearIdx < 0 || iWearIdx >= MAX_WEAR)
	{
		return -1;
	}

	pstArm = pstRoleWear->RoleArms + iWearIdx;
	if(!pstArm->ExtendedFromDefID ||
	   z_extend_item_property(pstEnv, pstArm->ExtendedFromDefID, 0) !=
	   EXTENDED_TYPE_RESULT)
	{
		return -1;
	}

	switch(pstArm->ExtendedResult.ResultID)
	{
	// 需要在战斗时处理的效果
	case RESULT_STATUS:
	case RESULT_SELF_STATUS:
	// ...还有需要处理的效果直接加到这里
		pstPlayer->stOnline.nExtendedResultBitMap |= (1U << iWearIdx);
		break;
	}
	return 0;
}

int z_wear_del(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEARM *pstRoleArm, ROLEPACKAGECHG *pstPackageChg)
{
	int i = 0, fOk = -1;
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	CSROLEARM *pstCSArm = &pstPackageChg->ArmChg.Arms[pstPackageChg->ArmChg.ChgNum];
	UNUSED(pstEnv);

	for (i=0; i<pstRoleWear->ArmNum; i++)
	{
		if (pstRoleWear->RoleArms[i].DefID == pstRoleArm->DefID && pstRoleWear->RoleArms[i].WID == pstRoleArm->WID)
		{
			pstCSArm->ListType = LIST_TYPE_WEAR;
			memcpy(&pstCSArm->Arm, pstRoleWear->RoleArms+i, sizeof(pstCSArm->Arm));
			pstCSArm->Arm.Num = 0;
			pstPackageChg->ArmChg.ChgNum++;

			pstRoleWear->ArmNum--;
			if (i != pstRoleWear->ArmNum)
			{
				memcpy(pstRoleWear->RoleArms+i,
				       pstRoleWear->RoleArms+pstRoleWear->ArmNum,
				       sizeof(pstRoleWear->RoleArms[0]));
				memcpy(pstPlayer->stOnline.tExtendedResultEnd + i,
				       pstPlayer->stOnline.tExtendedResultEnd + pstRoleWear->ArmNum,
				       sizeof(pstPlayer->stOnline.tExtendedResultEnd[0]));
				       
			}
			fOk = 0;
			break;
		}
	}

	if(fOk == 0)
	{
		// 重新生成Bitmap
		pstPlayer->stOnline.nExtendedResultBitMap = 0;
		for(i = 0; i < pstRoleWear->ArmNum; ++i)
		{
			z_wear_extended_bit_set(pstEnv, pstPlayer, i);
		}
	}
		
	return fOk;
}

int z_wear_add(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEARM *pstWear, ITEMDEF *pstItemDef, ROLEPACKAGECHG *pstPackageChg)
{
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	CSROLEARM *pstCSArm = &pstPackageChg->ArmChg.Arms[pstPackageChg->ArmChg.ChgNum];
	UNUSED(pstEnv);
	UNUSED(pstItemDef);

	if (pstRoleWear->ArmNum >= MAX_WEAR)
	{
		return -1;
	}

	// 设置Bitmap与冷却时间
	if(z_wear_extended_bit_set(pstEnv, pstPlayer, pstRoleWear->ArmNum) == 0)
	{
		pstPlayer->stOnline.tExtendedResultEnd[pstRoleWear->ArmNum] =
			pstEnv->pstAppCtx->stCurr.tv_sec + INIT_EXTENDED_CDTIME;
	}

	memcpy(pstRoleWear->RoleArms+pstRoleWear->ArmNum, pstWear,
	       sizeof(pstRoleWear->RoleArms[0]));
	pstRoleWear->ArmNum++;

	pstCSArm->ListType = LIST_TYPE_WEAR;
	memcpy(&pstCSArm->Arm, pstWear, sizeof(pstCSArm->Arm));
	pstPackageChg->ArmChg.ChgNum++;

	return 0;
}

int z_pkg_check_clanlimit(ZONESVRENV* pstEnv, Player *pstPlayer,char cLimit)
{
	if (cLimit == 1 &&
		!is_world_city_master(pstEnv, pstPlayer))
	{
		return -1;
	}

	if (cLimit == 2 &&
		!is_world_city_owner(pstEnv, pstPlayer))
	{
		return -2;
	}
	
	return 0;
}

int z_package_use_pre(ZONESVRENV* pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef)
{
	DIATHESISDEF *pstDiathesisDef = NULL;
	int iPkVal;
	int iRet;
	
	if ( (CAREER_NON != pstItemDef->WearPre.Career && (pstPlayer->stRoleData.Career&0x0F) != (pstItemDef->WearPre.Career&0x0F)) || 
		 (CAREER_NON != (pstItemDef->WearPre.Career&0xF0) && (pstPlayer->stRoleData.Career&0xF0) != (pstItemDef->WearPre.Career&0xF0)) )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_PAK111 );
		return -1;
	}

	if(pstItemDef->UseTarget != USE_TARGET_PET)
	{
		if ((pstItemDef->WearPre.RoleLevelMin != 0 && pstPlayer->stRoleData.Level < pstItemDef->WearPre.RoleLevelMin) ||
			(pstItemDef->WearPre.RoleLevelMax != 0 && pstPlayer->stRoleData.Level > pstItemDef->WearPre.RoleLevelMax))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_PAK112 );
			return -1;
		}
	}

	if (pstItemDef->WearPre.DiathesisID != 0)
	{
		pstDiathesisDef = z_find_diathesis(pstEnv, pstItemDef->WearPre.DiathesisID, 1);
		if (!pstDiathesisDef)
		{
			return -1;
		}
		if (NULL == z_get_player_diathesis(pstPlayer, pstItemDef->WearPre.DiathesisID))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_PAK113, pstDiathesisDef->Name);
			return -1;
		}
	}

	if (pstItemDef->WearPre.Gender != 0)
	{
		if (pstItemDef->WearPre.Gender != pstPlayer->stRoleData.Gender)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_PAK114);
			return -1;
		}
	}

	iPkVal = pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal ;
	if (iPkVal < pstItemDef->WearPre.PKVal)
	{
		if (pstItemDef->WearPre.PKVal >= PLAYER_PK_PUBLIC_ENEMY )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_PAK115 );
		}
		else if(pstItemDef->WearPre.PKVal >= PLAYER_PK_FRENZY)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_PAK116);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK117,iPkVal);
		}
		return -1;	
	}

	iRet = z_pkg_check_clanlimit(pstEnv, pstPlayer,pstItemDef->WearPre.ClanLimit);
	if (iRet == -1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK118);
		return -1;
	}
	else if (iRet == -2)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK119 );
		return -1;
	}

	return 0;
}

int z_package_wear_use_pre(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEARM *pstRoleArm, ITEMDEF *pstItemDef)
{
	DIATHESISDEF *pstDiathesisDef = NULL;
	int iPkVal;
	int iDecLevel, iMinLv;
	int iRet;
	RESULT *pstResult = NULL;
	RESULT *pstResult2 = NULL;
	int iTemp1 = 0;
	int iTemp2 = 0;
	
	if ( (CAREER_NON != pstItemDef->WearPre.Career && (pstPlayer->stRoleData.Career&0x0F) != (pstItemDef->WearPre.Career&0x0F)) || 
		 (CAREER_NON != (pstItemDef->WearPre.Career&0xF0) && (pstPlayer->stRoleData.Career&0xF0) != (pstItemDef->WearPre.Career&0xF0)) )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_PAK111);
		return -1;
	}

	iDecLevel = player_addattr_dec_wearlevel(pstEnv, pstPlayer, pstRoleArm, pstItemDef);
	iMinLv = pstItemDef->WearPre.RoleLevelMin - iDecLevel;
	if(iMinLv < 0)
	{
		iMinLv = 0;
	}

	if (ITEM_NO_LEVELLIMIT & pstItemDef->CtrlFlag)
	{
		//
	}
	else if ((iMinLv != 0 && pstPlayer->stRoleData.Level < iMinLv) ||
		(pstItemDef->WearPre.RoleLevelMax != 0 && pstPlayer->stRoleData.Level > pstItemDef->WearPre.RoleLevelMax))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_PAK112);
		return -1;
	}

	if (pstItemDef->WearPre.DiathesisID != 0)
	{
		pstDiathesisDef = z_find_diathesis(pstEnv, pstItemDef->WearPre.DiathesisID, 1);
		if (!pstDiathesisDef)
		{
			return -1;
		}
		if (NULL == z_get_player_diathesis(pstPlayer, pstItemDef->WearPre.DiathesisID))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_PAK113, pstDiathesisDef->Name);
			return -1;
		}
	}

	if (pstItemDef->WearPre.Gender != 0)
	{
		if (pstItemDef->WearPre.Gender != pstPlayer->stRoleData.Gender)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_PAK114);
			return -1;
		}
	}

	if (pstItemDef->WearPre.GeniusLimit > 0)
	{
		if (genius_get_mod_by_type( pstEnv, pstPlayer, GENIUS_TYPE_BASE) < pstItemDef->WearPre.GeniusLimit )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer,SYS_MIDDLE2, 
					ZONEERR_PAK263, pstItemDef->WearPre.GeniusLimit);
			return -1;
		}
		
	/*	if (genius_get_mod( pstEnv, pstPlayer) < pstItemDef->WearPre.GeniusLimit )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer,SYS_MIDDLE2, 
					ZONEERR_PAK120, pstItemDef->WearPre.GeniusLimit);
			return -1;
		}*/	
	}

	iPkVal = pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal ;
	if (iPkVal < pstItemDef->WearPre.PKVal)
	{
		if (pstItemDef->WearPre.PKVal >= PLAYER_PK_PUBLIC_ENEMY )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_PAK115);
		}
		else if(pstItemDef->WearPre.PKVal >= PLAYER_PK_FRENZY)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2,  ZONEERR_PAK116);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK117,iPkVal);
		}
		return -1;	
	}

	iRet = z_pkg_check_clanlimit(pstEnv, pstPlayer,pstItemDef->WearPre.ClanLimit);
	if (iRet == -1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK118);
		return -1;
	}
	else if (iRet == -2)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK119);
		return -1;
	}

	if(pstItemDef->WearPre.SchoolValLimit > 0)
	{
		if((int)pstPlayer->stRoleData.RoleDetail.SchoolVal < pstItemDef->WearPre.SchoolValLimit)
		{
			z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PACKAGE_308);
			return -1;
		}
	}

	pstResult = z_item_get_result(pstItemDef, RESULT_ITEM_LOVER);
	pstResult2 = z_item_get_result(pstItemDef, RESULT_ITEM_SPOUSE);
	iTemp1 = find_lover_from_friend_list(&pstPlayer->stRoleData.Buddy);
	iTemp2 = find_spouse_from_friend_list(&pstPlayer->stRoleData.Buddy);
	
	if(pstResult && pstResult2)
	{
		if(iTemp1 < 0 && iTemp2 < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_WEDDING57,pstItemDef->Name);
			return -1;
		}
	}
	else if(pstResult && !pstResult2)
	{
		if(iTemp1 < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_WEDDING55,pstItemDef->Name);
			return -1;
		}
	}
	else if(!pstResult && pstResult2)
	{
		if(iTemp2 < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_WEDDING56,pstItemDef->Name);
			return -1;
		}
	}

	return 0;
}

/*
int package_del_pet_arm(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEARM *pstRoleArm)
{
	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstPackageChg = &pstPackageInfo->PackageInfoData.PackageChg;
	int iPos;
	SUNDRIESPACKAGE *pstSundriesPackage = &pstPlayer->stRoleData.Package.SundriesPackage;        

	iPos = package_grid_pos(pstSundriesPackage->RoleGrids, pstSundriesPackage->Num, pstRoleArm->GridIdx);

	pstSundriesPackage->Num--;
	if (iPos != pstSundriesPackage->Num)
	{
		memcpy(&pstSundriesPackage->RoleGrids[iPos], &pstSundriesPackage->RoleGrids[pstSundriesPackage->Num], sizeof(ROLEGRID));
	}

	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstPackageChg->ItemChg.ChgNum = 0;
	pstPackageChg->ArmChg.ChgNum = 1;

	pstPackageChg->ArmChg.Arms[0].ListType = LIST_TYPE_SUNDRIES;
	memcpy(&pstPackageChg->ArmChg.Arms[0].Arm, pstRoleArm, sizeof(ROLEARM));
	pstPackageChg->ArmChg.Arms[0].Arm.Num = 0;

	Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int z_wear_pre_of_pet(ZONESVRENV* pstEnv, ROLEPET *pstRolePet, ITEMDEF *pstItemDef)
{
	UNUSED(pstEnv);
	
	WEARPRE *pstWearPre = &pstItemDef->WearPre;

	if ((pstWearPre->RoleLevelMin != 0 && pstRolePet->Level < pstWearPre->RoleLevelMin)||
		(pstWearPre->RoleLevelMax != 0 && pstRolePet->Level > pstWearPre->RoleLevelMax))
	{
		return -1;
	}

	if (pstWearPre->DiathesisID != 0)
	{
		if (find_pet_diathesis(pstRolePet, pstWearPre->DiathesisID, NULL) == NULL)
		{
			return -1;
		}
	}
	return 0;
}
*/
/*
int pet_wear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSWEAR *pstCSWear)
{
	ITEMDEF *pstItemDef;
	ITEMDEF *pstPetArm;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	int iPos, i, iOldGridPet;
	ROLEGRID *pstGrid;
	ROLEGRID stRoleGrid;
	ROLEPET *pstRolePet;    
	int iFreeArmGrid;

	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;


	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOITEM))
	{
		return -1;
	}
	
	if (0 == z_player_act_status(pstAppCtx, pstEnv, pstPlayer, RES_ACTION_WEAR))
	{
		return -1;
	}

	switch(pstCSWear->ListType)
	{
		case LIST_TYPE_SUNDRIES:
			if (pstCSWear->GridIdx >= pstPak->SundriesPackage.MaxNum)
			{
				return -1;
			}
			iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, pstCSWear->GridIdx);
			if (0 > iPos)
			{
				return -1;
			}

			pstGrid = pstPak->SundriesPackage.RoleGrids+iPos;
			if (pstGrid->Type != GRID_ITEM_ARM || pstGrid->GridData.RoleArm.DefID != (int)pstCSWear->ArmDefID)
			{
				return -1;
			}

			break;
		default:
			return -1;
			break;
	}

	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstGrid->GridData.RoleItem);
	if (!pstItemDef || pstItemDef->ItemID != pstCSWear->ArmDefID || pstItemDef->ArmType == 0)
	{
		return -1;
	}

	if (pstItemDef->ValidTime > 0 && pstGrid->GridData.RoleArm.ValidEnd < pstAppCtx->stCurr.tv_sec)
	{
		z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM_VALIDEND, pstItemDef->Name);	
		return -1;
	}

	pstRolePet = find_package_shi(pstPlayer, pstCSWear->PetWID, 0);
	if (!pstRolePet)
	{
		return -1;
	}

	if (z_wear_pre_of_pet(pstEnv, pstRolePet, pstItemDef) < 0)
	{
		return -1;
	}
	
	for (i=0; i<pstRolePet->PetArms.Num; i++)
	{
		pstPetArm = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)&pstRolePet->PetArms.Arms[i]);
		if (pstPetArm->ArmType == pstItemDef->ArmType)
		{
			break;
		}
	}

	// 是否存在同类型装备
	if (i >= pstRolePet->PetArms.Num)
	{
		//  没格子
		if (pstRolePet->PetArms.Num >= pstRolePet->PetArms.MaxNum)
		{
			return -1;
		}

		iFreeArmGrid = pet_get_free_pos_of_arm(pstRolePet);
		if (-1 == iFreeArmGrid)
		{
			return -1;
		}
		package_del_pet_arm(pstEnv, pstPlayer, &pstGrid->GridData.RoleArm);


		memcpy(&pstRolePet->PetArms.Arms[pstRolePet->PetArms.Num],&pstGrid->GridData.RoleArm,sizeof(ROLEARM));
		pstRolePet->PetArms.Arms[pstRolePet->PetArms.Num].GridIdx = iFreeArmGrid;
		pstRolePet->PetArms.Num ++;		
		pet_armchange_to_clt(pstEnv, pstPlayer, pstRolePet);

	}
	else
	{
		iOldGridPet = pstRolePet->PetArms.Arms[i].GridIdx;
		
		stRoleGrid.Type = GRID_ITEM_ARM;
		memcpy(&stRoleGrid.GridData.RoleArm, &pstRolePet->PetArms.Arms[i], sizeof(ROLEARM));
		stRoleGrid.GridData.RoleItem.GridIdx = pstCSWear->GridIdx;
		
		memcpy(&pstRolePet->PetArms.Arms[i],&pstGrid->GridData.RoleArm,sizeof(ROLEARM));
		pstRolePet->PetArms.Arms[i].GridIdx = iOldGridPet;
		pet_armchange_to_clt(pstEnv, pstPlayer, pstRolePet);

		memcpy(pstGrid, &stRoleGrid, sizeof(ROLEARM));
		
		pstPackageInfo->Type = PACKAGE_INFO_CHG;      
		pstPackageInfo->PackageInfoData.PackageChg.ItemChg.ChgNum = 0;
		pstPackageInfo->PackageInfoData.PackageChg.ArmChg.ChgNum = 1;
		memcpy(&pstPackageInfo->PackageInfoData.PackageChg.ArmChg.Arms[0], &stRoleGrid, sizeof(ROLEARM));

		Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	// 计算修正
	
	pet_update_online(pstEnv, pstPlayer, pstRolePet);
	return 0;
}
*/

//获得评分
int get_equip_score(ZONESVRENV * pstEnv,ROLEARM * pstWear)
{
    int iScore = 0;
    int iA = 0;
    int       iB = 0;
    double       iC = 0;
    int       i = 0;
    double dNewAddAttrSum = 0.0;


    ITEMDEF* pstItemDef = z_find_itemdef(pstEnv,pstWear->DefID);

    if (!pstItemDef)
    {
           return 0;
    }
    iA = pstItemDef->WearPre.RoleLevelMin;
    iB = pstItemDef->Important;
    if (pstItemDef->SuitSetID)
    {
		/*
           SUITSET* pstSuit = z_find_suitset(pstEnv,pstItemDef->SuitSetID);
           if (pstSuit)
           {
                  iB = pstSuit->SuitSetType;
           }
           */
    }

    for(i =0; i < pstWear->BaseAttrNum;i++)
    {

           int iValue = pstWear->BaseAttr[i].Mul;
           iC += iValue/100.0;
/*
           for (k = 0; k < MAX_ITEM_RESULT; k++)
           {
                  if (!pstItemDef->Result[k].ResultID)
                  {
                         break;
                  }
                  if (pstItemDef->Result[k].ResultID == pstWear->BaseAttr[i].ResultID &&
                         pstItemDef->Result[k].ResultVal2 > 0)
                  {
                         iC += ((iValue/100.0) * pstItemDef->Result[k].ResultVal1)/pstItemDef->Result[k].ResultVal2;
                  }
           }
           */

    }

    if (i > 0)
    {
           iC /= i;
    }



    for( i = 0; i < pstWear->AddAttrNum; i++)
    {
           NEWADDATTRDEF* pstNewAddAttr = z_find_new_addattr_def(pstEnv,pstWear->AddAttr[i].AddAttrID);
           double iVal = 0;
           if (!pstNewAddAttr)
           {
                  continue;
           }

           iVal = arm_addattr_result_addval_get_double(pstEnv,pstItemDef, pstWear->AddAttr[i].AddAttrID,pstWear->AddAttr[i].Val);


			if (ADD_ATTR_RESULT_HEAVY_HIT_VAL == pstNewAddAttr->AddAttrResultID)
			{
				iVal *= 100;
			}
			
           if (ADD_ATTR_RESULT_HEAVY_HIT_VAL == pstNewAddAttr->AddAttrResultID ||
                  ADD_ATTR_RESULT_HEAL_HP == pstNewAddAttr->AddAttrResultID )
           {
                  iVal = iVal / 100.0 * pow(1.05,iA);
           }
           else
           {

                  iVal = (int)iVal;
           }

           iVal *= pstNewAddAttr->Score;

           dNewAddAttrSum += iVal;
    }


    iScore = (10 + 5 * iC) * pow(1.04 , ( iB * 8 + iA -1)) + (int)dNewAddAttrSum;
    return iScore;
}  

int player_wear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSWEAR *pstCSWear)
{
	ITEMDEF *pstItemDef=NULL;
	ITEMDEF *pstUnwearItemDef=NULL;
	ITEMDEF *pstUnwearLeftItemDef=NULL;
	int iPos = 0, i = 0;
	ROLEGRID *pstGrid = NULL;
	int iUnWearNum;
	ROLEARM stWear;
	//int iFlag;
	CSROLEARM *pstCSRoleArm;
	ROLEGRID *pstUnWearGrid;
	ROLEGRID astUnWear[MAX_WEAR_POS];
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	//AMMMAP *pstAmmMap = &pstRoleWear->AmmMap;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	//DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	//ATKCHG *pstAtkChg;
	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstPackageChg = &pstPackageInfo->PackageInfoData.PackageChg;
	//CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	int iGunerWearPosLeft = -1;
	int iValidGridNum = get_vaild_gird(pstEnv, pstPak,pstCSWear->ListType);
	int iVaildFreeNum;
	int iUnWearTmpNum = 0;
	
	if (0 == z_player_act_status(pstAppCtx, pstEnv, pstPlayer, RES_ACTION_WEAR))
	{
		return -1;
	}
	
	switch(pstCSWear->ListType)
	{
		case LIST_TYPE_SUNDRIES:
			if (pstCSWear->GridIdx >= pstPak->SundriesPackage.MaxNum)
			{
				return -1;
			}
			iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, pstCSWear->GridIdx);
			if (0 > iPos)
			{
				return -1;
			}

			pstGrid = pstPak->SundriesPackage.RoleGrids+iPos;
			if (pstGrid->Type != GRID_ITEM_ARM || pstGrid->GridData.RoleArm.DefID != (int)pstCSWear->ArmDefID)
			{
				return -1;
			}
			break;
		default:
			return -1;
			break;
	}
	if(!pstGrid)
	{
		return 0;
	}
	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstGrid->GridData.RoleItem);
	if (!pstItemDef || pstItemDef->ItemID != pstCSWear->ArmDefID || pstItemDef->ArmType != 0)
	{
		return -1;
	}

	if (	pstGrid->GridData.RoleArm.ValidEnd > 0 &&
		pstGrid->GridData.RoleArm.ValidEnd < pstAppCtx->stCurr.tv_sec)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PAK208, pstItemDef->Name);
		return -1;
	}
	
	if (  pstCSWear->WearPos < WEAR_POS_NO  || pstCSWear->WearPos > WEAR_POS_MAX ||
		(pstItemDef->WearPos1 == WEAR_POS_RING && (pstCSWear->WearPos < WEAR_POS_RING1 || pstCSWear->WearPos > WEAR_POS_RING3)) ||
		(pstItemDef->WearPos1 == WEAR_POS_AMULET && (pstCSWear->WearPos < WEAR_POS_AMULET1 || pstCSWear->WearPos > WEAR_POS_AMULET2)) ||
		(pstItemDef->WearPos1 == WEAR_POS_NO || pstItemDef->WearPosNum == 0 ))
	{
		return -1;
	}

	if (	z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_CHG_ARM) &&
		(pstItemDef->WearPos1 == WEAR_POS_RIGHTHAND ||
		pstItemDef->WearPos1 == WEAR_POS_LEFT ||
		pstItemDef->WearPos2 == WEAR_POS_RIGHTHAND ||
		pstItemDef->WearPos2 == WEAR_POS_LEFT ||
		pstItemDef->WearPos3 == WEAR_POS_RIGHTHAND ||
		pstItemDef->WearPos3 == WEAR_POS_LEFT ))
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK121);
		return -1;
	}

	if( 0 != check_gunner_wear_pos_left(pstEnv, pstPlayer,pstItemDef) )
	{
		return -1;
	}

	//检查罩装有效期
	if (pstItemDef->ValidTime > 0 && pstGrid->GridData.RoleArm.ValidEnd < pstAppCtx->stCurr.tv_sec)
	{
		z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM_VALIDEND, pstItemDef->Name);	
		return -1;
	}

	//todo 判断装备的职业等级素质等前置条件
	if (0 > z_package_wear_use_pre(pstEnv, pstPlayer,  &pstGrid->GridData.RoleArm, pstItemDef))
	{
		return -1;
	}

	iUnWearNum = 0;
	for (i=0; i<pstRoleWear->ArmNum; i++)
	{
		pstUnwearItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)&pstRoleWear->RoleArms[i]);
		if (pstUnwearItemDef)
		{
			if (z_wear_pos_rep(pstItemDef, pstUnwearItemDef, pstCSWear->WearPos, pstRoleWear->RoleArms[i].GridIdx))
			{
				if ( iUnWearNum>= MAX_WEAR_POS)
				{
					return -1;
				}

				pstUnWearGrid = &astUnWear[iUnWearNum];
				pstUnWearGrid->Type = GRID_ITEM_ARM;
				memcpy(&pstUnWearGrid->GridData.RoleArm, &pstRoleWear->RoleArms[i], sizeof(pstUnWearGrid->GridData.RoleArm));
				iUnWearNum++;

				if( iGunerWearPosLeft == -1)
				{
					iGunerWearPosLeft =  check_gunner_unwear_pos_left( pstEnv,  pstPlayer, pstUnwearItemDef);
					if(iGunerWearPosLeft >0 )
					{
						pstUnwearLeftItemDef = pstUnwearItemDef;
					}
				}	
			}
			else
			{
				//todo 检查装备的相容
			}		
		}
	}

	iUnWearTmpNum = iUnWearNum;
	if( iGunerWearPosLeft != -1)
	{
		// 如果枪手有副手武器,看看杂物包能否放下
		/*if (pstPak->SundriesPackage.Num+iUnWearNum > pstPak->SundriesPackage.MaxNum)
		{
			z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, "副手武器要和主手武器一起被卸下,但背包空间不足");	
			return -1;
		}*/
		iUnWearTmpNum = iUnWearNum+1;
	}
	/*else if (pstPak->SundriesPackage.Num-1+iUnWearNum > pstPak->SundriesPackage.MaxNum)
	{
		z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_NOSPACE);	
		return -1;
	}*/

	iVaildFreeNum = pack_freevaild_grid_num(pstPak->SundriesPackage.RoleGrids, 
											pstPak->SundriesPackage.Num, 
											pstPak->SundriesPackage.MaxNum,
											iValidGridNum);
	if (iVaildFreeNum < 0)
		return -1;

	// 来自失效栏位
	if (gridIdx_is_unvaild(pstEnv, pstPak, pstCSWear->ListType, pstCSWear->GridIdx) != 1)
	{		
		if (iUnWearTmpNum > iVaildFreeNum)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
			return -1;
		}
	}
	else
	{
		if (iUnWearTmpNum > iVaildFreeNum +1)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
			return -1;
		}
	}

	
	//检查穿戴绑定
	if (pstItemDef->ArmBind == ITEM_WEAR_BIND && !(pstGrid->GridData.RoleArm.InstFlag & INST_ITEM_BIND))
	{
		pstGrid->GridData.RoleArm.InstFlag |= INST_ITEM_BIND;
	}

	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstPackageChg->ItemChg.ChgNum = 0;
	pstPackageChg->ArmChg.ChgNum = 0;

	memcpy(&stWear, &pstGrid->GridData.RoleArm, sizeof(stWear));
	pstGrid->GridData.RoleArm.Num = 0;

	pstCSRoleArm = pstPackageChg->ArmChg.Arms+pstPackageChg->ArmChg.ChgNum;
	pstCSRoleArm->ListType = pstCSWear->ListType;
	memcpy(&pstCSRoleArm->Arm, &pstGrid->GridData.RoleArm, sizeof(pstCSRoleArm->Arm));
	pstPackageChg->ArmChg.ChgNum++;	
	
	pstPak->SundriesPackage.Num--;
	if (iPos != pstPak->SundriesPackage.Num)
	{
		memcpy(pstPak->SundriesPackage.RoleGrids+iPos, pstPak->SundriesPackage.RoleGrids+pstPak->SundriesPackage.Num, sizeof(pstPak->SundriesPackage.RoleGrids[0]));
	}
	
	if (pstItemDef->WearPos1 == WEAR_POS_RING ||
		pstItemDef->WearPos1 == WEAR_POS_AMULET ||
		pstItemDef->WearPos1 == WEAR_POS_FASHION_BODY ||
		pstItemDef->WearPos1 == WEAR_POS_FASHION_BACK ||
		pstItemDef->WearPos1 == WEAR_POS_FASHION_HEAD ||
		pstItemDef->WearPos1 == WEAR_POS_FASHION_HAIR ||
		pstItemDef->WearPos1 == WEAR_POS_FASHION_SHOES ||
		pstItemDef->WearPos1 == WEAR_POS_FASHION_HAND ||
		pstItemDef->WearPos1 == WEAR_POS_FASHION_LEG)
	{
		stWear.GridIdx = pstCSWear->WearPos;
	}
	else
	{
		stWear.GridIdx = pstItemDef->WearPos1;
	}

	for (i=0; i<iUnWearNum; i++)
	{
		pstUnWearGrid = astUnWear+i;

		if (0 > z_wear_del(pstEnv, pstPlayer, &pstUnWearGrid->GridData.RoleArm, pstPackageChg))
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player(%d) z_wear_del fail", pstPlayer->stRoleData.Uin);
		}

		if (0 > package_add_in(pstAppCtx, pstEnv, &pstPlayer->stOnline.stDyn, &pstPlayer->stRoleData.Package, pstUnWearGrid, pstPackageChg))
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player(%d) package_add_in fail", pstPlayer->stRoleData.Uin);
		}
	}

	if (0 > z_wear_add(pstEnv, pstPlayer, &stWear, pstItemDef, pstPackageChg))
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player(%d) z_wear_add fail", pstPlayer->stRoleData.Uin);
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	//更换avatar
	z_update_avatar(pstEnv, pstPlayer);

	z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);

	if (pstEnv->pstConf->CountryVer != COUNTRY_VER_CHINA_TAIWAN)
	{
		player_wear_skill_cool_update(pstAppCtx,pstEnv,pstPlayer);
	}

	z_arm_skill_update(pstEnv, pstPlayer);

	/*
	if (z_is_weapon(pstItemDef, pstPlayer->stRoleData.Career))
	{
		for (i=0; i<MAX_ARM_ON; i++)
		{
			pstAtkChg = pstDyn->astAtkChg+i;
			if (0 == pstAtkChg->iArmID)
			{
				break;
			}

			if (pstAtkChg->iArmID == (int)pstItemDef->ItemID && pstAtkChg->iNeedAmm && 0 == pstAtkChg->iAmmID)
			{
				iFlag = 0;
				if (0 != pstItemDef->WearPos2)
				{
					if (0 != pstAmmMap->AmmRight)
					{
						pstAmmMap->AmmRight = 0;
						iFlag = 1;
					}
				}
				else
				{
					switch(pstItemDef->WearPos1)
					{
						case WEAR_POS_LEFT:
							if (0 != pstAmmMap->AmmLeft)
							{
								pstAmmMap->AmmLeft = 0;
								iFlag = 1;
							}
							break;
						case WEAR_POS_RIGHTHAND:
							if (0 != pstAmmMap->AmmRight)
							{
								pstAmmMap->AmmRight = 0;
								iFlag = 1;
							}
							break;
						case WEAR_POS_SHOULDER:
							if (0 != pstAmmMap->AmmShoulder)
							{
								pstAmmMap->AmmShoulder = 0;
								iFlag = 1;
							}
							break;
						default:
							break;
					}
				}

				if (iFlag)
				{
					pstActionRes->ID = pstPlayer->iMemID;
					pstActionRes->ActionID = ACTION_AMM;
					memcpy(&pstActionRes->Data.Amm, pstAmmMap, sizeof(pstActionRes->Data.Amm));
					Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);

					z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
				}
			}
		}
	}
	*/

	task_player_trace(pstEnv, pstPlayer, TASK_OBJ_WEAR_ARM);
	task_player_trace(pstEnv, pstPlayer, TASK_OBJ_PERFECT_ARM);

	//成长之路
	{
		ACHIEVEMENTCOND stCond;

		memset(&stCond,0,sizeof(stCond));
		
		stCond.Type = COND_EQUIP_WEAR;
		stCond.Val1 = get_equip_score(pstEnv,&stWear);

		z_player_achievement_cond(pstEnv,pstPlayer,&stCond);

		if (pstPlayer->iShowEquipScore)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_SVR, ZONEERR_PAK209,
					pstItemDef->Name,pstItemDef->ItemID,stCond.Val1);
		}

		
	}

	//重新取下
	if(pstUnwearLeftItemDef && iGunerWearPosLeft != -1 )
	{
		iGunerWearPosLeft =  check_gunner_unwear_pos_left( pstEnv,  pstPlayer, pstUnwearLeftItemDef);
	}
	if(iGunerWearPosLeft != -1 )
	{
		// 如果枪手有副手武器,把它卸下来
		if( 0 != package_gunner_unwear_pos_left( pstAppCtx,  pstEnv, pstPlayer, iGunerWearPosLeft) )
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player(%d) guner unwear pos left fail!", pstPlayer->stRoleData.Uin);
		}
	}
	return 0;
}

ROLEARM *z_wear_get(ROLEWEAR *pstRoleWear, int iItemDefID, char cWearPos)
{
	int i;
	ROLEARM *pstWear;

	for (i=0; i<pstRoleWear->ArmNum; i++)
	{
		pstWear = pstRoleWear->RoleArms+i;
		if (pstWear->DefID == iItemDefID && ((-1 == cWearPos) || (cWearPos == pstWear->GridIdx)))
		{
			return pstWear;
		}
	}

	return NULL;
}


int z_update_avatar(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i, iValid, iQiYueID;
	ARMPROF *pstArmProf;
	UinCache *pstUinCache;
	ITEMDEF *pstItemDef;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	ROLEAVATARPROF *pstAvatar = &pstPlayer->stRoleData.AvatarProf;
	ROLEMISCINFO  *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	ROLEARM *pstRoleArm;

	pstAvatar->Num = 0;
	pstAvatar->BackModelID = 0;
	iQiYueID = 0; // 初始化一下。
	for (i=0; i<pstRoleWear->ArmNum; i++)
	{
		pstRoleArm = &pstRoleWear->RoleArms[i];
		// 过期的不发
		if (	pstRoleArm->ValidEnd > 0 &&
			pstRoleArm->ValidEnd < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			continue;
		}
		
		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)(pstRoleWear->RoleArms+i));
		if (NULL == pstItemDef)
		{
			continue;
		}

		if (z_pkg_check_clanlimit(pstEnv, pstPlayer,pstItemDef->WearPre.ClanLimit) < 0)
		{
			continue;
		}

		if (pstAvatar->Num >= MAX_AVATAR) 
			break;

		if (	(arm_is_head(pstItemDef) && (SHOW_HEAD_FLAG & pstMiscInfo->Flag))  ||
			(arm_is_normal_fashion(pstItemDef) && (SHOW_FASHION_FLAG & pstMiscInfo->Flag)) ||
			(arm_is_back(pstItemDef) && (SHOW_BACK_FLAG & pstMiscInfo->Flag)) ||
			(arm_is_face(pstItemDef) && (SHOW_FACE_FLAG & pstMiscInfo->Flag)) ||
			arm_is_avatar(pstItemDef))
		{
			if (pstPlayer->stRoleData.Map == BEACH_MAP_ID && WEAR_POS_FACE != pstItemDef->WearPos1 &&
				0 == arm_is_back(pstItemDef) && 0 == arm_is_normal_fashion(pstItemDef))
			{
				continue;
			}

			if(arm_is_normal_fashion(pstItemDef) && !is_valid_normal_fashion(pstPlayer, pstRoleWear->RoleArms[i].GridIdx))
			{
				continue;
			}

			if(arm_is_back(pstItemDef) && !is_valid_back(pstPlayer, pstRoleWear->RoleArms[i].GridIdx))
			{
				continue;
			}

			pstArmProf = &pstAvatar->Arms[pstAvatar->Num];
			pstArmProf->ArmDefID = pstRoleWear->RoleArms[i].DefID;
			pstArmProf->GridIdx = pstRoleWear->RoleArms[i].GridIdx;
			pstArmProf->StarLv = arm_star_perfect_num_get(&pstRoleWear->RoleArms[i]);
			pstAvatar->Num++;

			if(pstRoleWear->RoleArms[i].UnrealBackID > 0)
			{
				pstAvatar->BackModelID = pstRoleWear->RoleArms[i].UnrealBackID;
			}
		}
	}

	pstAvatar->ArmStar = z_wear_get_star_perfect_num(pstEnv, pstPlayer);
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_AVATAR;
	memcpy(&pstActionRes->Data.Avatar, &pstPlayer->stRoleData.AvatarProf, sizeof(pstActionRes->Data.Avatar));
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);

	pstUinCache = (UinCache *)sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos, &iValid);
	if (NULL == pstUinCache || 0 == iValid || pstUinCache->iUin != pstPlayer->stRoleData.Uin)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
					"uin %d get cache pos=%d error", pstPlayer->stRoleData.Uin, pstPlayer->iUinCachePos);
	}
	else
	{
		for (i=0; i<pstUinCache->ucRoleNum; i++)
		{
			if (pstUinCache->astRoleListInfo[i].RoleID == pstPlayer->stRoleData.RoleID)
			{
				memcpy(&pstUinCache->astRoleListInfo[i].AvatarProf, &pstPlayer->stRoleData.AvatarProf, sizeof(pstUinCache->astRoleListInfo[0].AvatarProf));
				break;
			}
		}

		if (i >= pstUinCache->ucRoleNum)
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
					"uin %d get cache pos=%d no role %s error", pstPlayer->stRoleData.Uin, pstPlayer->iUinCachePos, pstPlayer->stRoleData.RoleName);
		}
	}
	
	return 0;
}

/*
int pet_unwear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSWEAR *pstCSUnWear)
{
	ROLEARM *pstWear;
	ROLEPET *pstRolePet;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	int iPos = -1;
	int iPackPos;
	ROLEGRID *pstRoleGrid;

	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;

	
	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOITEM))
	{
		return -1;
	}
	
	if (0 == z_player_act_status(pstAppCtx, pstEnv, pstPlayer, RES_ACTION_WEAR))
	{
		return -1;
	}

	switch(pstCSUnWear->ListType)
	{
		case LIST_TYPE_SUNDRIES:
			if (pstCSUnWear->GridIdx >= pstPak->SundriesPackage.MaxNum)
			{
				return -1;
			}

			if (pstPak->SundriesPackage.Num >= pstPak->SundriesPackage.MaxNum)
			{
				return -1;
			}
			
			iPackPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, pstCSUnWear->GridIdx);
			if (iPackPos >=0)
			{
				return -1;
			}
			break;
		default:
			return -1;
			break;
	}

	pstRolePet = find_package_shi(pstPlayer, pstCSUnWear->PetWID, NULL);
	if (!pstRolePet || pstRolePet->PetArms.Num <= 0)
	{
		return -1;
	}

	pstWear = find_pet_arm( pstRolePet, pstCSUnWear->ArmDefID, &iPos);
	if (!pstWear || iPos == -1)
	{
		return -1;
	}
	

	
	// 更新背包数据  并发给客户端
	pstRoleGrid = &pstPak->SundriesPackage.RoleGrids[pstPak->SundriesPackage.Num];
	memcpy(&pstRoleGrid->GridData.RoleArm, pstWear, sizeof(ROLEARM));
	pstRoleGrid->Type = GRID_ITEM_ARM;
	pstRoleGrid->GridData.RoleArm.GridIdx = pstCSUnWear->GridIdx;
	pstPak->SundriesPackage.Num ++;

	pstPackageInfo->Type = PACKAGE_INFO_CHG;      
	pstPackageInfo->PackageInfoData.PackageChg.ItemChg.ChgNum = 0;
	pstPackageInfo->PackageInfoData.PackageChg.ArmChg.ChgNum = 1;
	memcpy(&pstPackageInfo->PackageInfoData.PackageChg.ArmChg.Arms[0].Arm, &pstRoleGrid->GridData.RoleArm, sizeof(ROLEARM));
	pstPackageInfo->PackageInfoData.PackageChg.ArmChg.Arms[0].ListType = LIST_TYPE_SUNDRIES;

	Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);	

	// 更新宠物数据  并发给客户端
	pstRolePet->PetArms.Num --;
	if (iPos != pstRolePet->PetArms.Num)
	{
		memcpy(&pstRolePet->PetArms.Arms[iPos], &pstRolePet->PetArms.Arms[pstRolePet->PetArms.Num], sizeof(ROLEARM));
	}
	pet_armchange_to_clt(pstEnv, pstPlayer, pstRolePet);

	pet_update_online(pstEnv, pstPlayer, pstRolePet);
	return 0;
}
*/
int z_arm_skill_update(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	ZoneSkill *pstZoneSkill = NULL;

	for (i = 0; i <MAX_ARM_SKILL; i++)
	{
		if ( pstDyn->astArmSkill[i].unSkillID == 0  ||  pstDyn->astArmSkill[i].ucLevel == 0)
		{
			break;
		}
		
		pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstDyn->astArmSkill[i].unSkillID);
		if (!pstZoneSkill)
		{
			continue;
		}
		
		z_update_one_skill( pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstZoneSkill);
		z_skill_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstZoneSkill);
	}

	return 0;
}

int z_find_arm_dyn_skill(Player *pstPlayer, unsigned short unSkillID)
{
	int i;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;

	for (i = 0; i <MAX_ARM_SKILL; i++)
	{
		if ( pstDyn->astArmSkill[i].unSkillID == 0  ||  pstDyn->astArmSkill[i].ucLevel == 0)
		{
			break;
		}

		if (unSkillID == pstDyn->astArmSkill[i].unSkillID)
		{
			return i;
		}
	}

	return -1;
}

int pack_freevaild_grid_num(ROLEGRID*pstPackageRoleGrid, int iNum, int iMaxNum, int iValidGridNum)
{
	int i = 0;
	ROLEGRID *pstGrid = NULL;
	int iGrid = 0;
	char sGrid[MAX_PACKAGE_GRID];
	int iVaildFreeNum = 0;

	assert(iMaxNum <= MAX_PACKAGE_GRID);

	memset(sGrid, 0, sizeof(sGrid[0])*iMaxNum);
	for(i=0; i<iNum; i++)
	{
		pstGrid = pstPackageRoleGrid+i;
		switch (pstGrid->Type)
		{
			case GRID_ITEM_ITEM:
				iGrid = pstGrid->GridData.RoleItem.GridIdx;
				break;
			case GRID_ITEM_ARM:
				iGrid = pstGrid->GridData.RoleArm.GridIdx;
				break;
			default:
				return -1;
				break;
		}
		
		if (iGrid >= 0 && iGrid < iMaxNum)
		{
			sGrid[iGrid] = 1;
		}
	}
	
	for (i=0; i<iMaxNum; i++)
	{
		if (!sGrid[i] && i<iValidGridNum)
			iVaildFreeNum++;
	}

	return iVaildFreeNum;
}

int player_unwear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSWEAR *pstCSUnWear)
{
	
	ITEMDEF *pstItemDef;
	ROLEARM *pstWear;
	int iPos;
	ROLEGRID stGrid;
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	CSPKG stPkg;
	CSROLEARM *pstCSRoleArm;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstPackageChg = &pstPackageInfo->PackageInfoData.PackageChg;
	int iGunerWearPosLeft = -1;
	int iValidGridNum = get_vaild_gird(pstEnv, pstPak,pstCSUnWear->ListType);
	int iUnWearNum = 1;
	int iVaildFreeNum;
    
	
	if (0 == z_player_act_status(pstAppCtx, pstEnv, pstPlayer, RES_ACTION_WEAR))
	{
		return -1;
	}

	pstWear = z_wear_get(pstRoleWear, pstCSUnWear->ArmDefID, pstCSUnWear->WearPos);
	if (!pstWear)
	{
		return -1;
	}

	pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstWear);
	if (!pstItemDef)
	{
		return -1;
	}
	
	switch(pstCSUnWear->ListType)
	{
		case LIST_TYPE_SUNDRIES:
			if (pstCSUnWear->GridIdx >= pstPak->SundriesPackage.MaxNum || pstPak->SundriesPackage.Num >= pstPak->SundriesPackage.MaxNum)
			{
				return -1;
			}
			iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, pstCSUnWear->GridIdx);
			if (0 <= iPos)
			{
				return -1;
			}

			break;
		default:
			return -1;
			break;
	}

	iGunerWearPosLeft =  check_gunner_unwear_pos_left( pstEnv,  pstPlayer, pstItemDef);
	if( iGunerWearPosLeft != -1)
	{
		// 如果枪手有副手武器,看看杂物包能否放下
		/*if (pstPak->SundriesPackage.Num + 2  > pstPak->SundriesPackage.MaxNum)
		{
			
			z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, "副手武器要和主手武器一起被卸下,但背包空间不足");	
			return -1;
		}*/

		iUnWearNum = 2;
	}

	iVaildFreeNum = pack_freevaild_grid_num(pstPak->SundriesPackage.RoleGrids, 
											pstPak->SundriesPackage.Num, 
											pstPak->SundriesPackage.MaxNum,
											iValidGridNum);
	
	if (iVaildFreeNum < iUnWearNum)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
		return -1;
	}
		
	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstPackageChg->ItemChg.ChgNum = 0;
	pstPackageChg->ArmChg.ChgNum = 0;

	stGrid.Type = GRID_ITEM_ARM;
	memcpy(&stGrid.GridData.RoleArm, pstWear, sizeof(stGrid.GridData.RoleArm));
	stGrid.GridData.RoleArm.GridIdx = pstCSUnWear->GridIdx;
		
	if (0 > z_wear_del(pstEnv, pstPlayer, pstWear, pstPackageChg))
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player(%d) z_wear_del fail", pstPlayer->stRoleData.Uin);
	}

	switch(pstCSUnWear->ListType)
	{
		case LIST_TYPE_SUNDRIES:
			memcpy(pstPak->SundriesPackage.RoleGrids+pstPak->SundriesPackage.Num, &stGrid	, sizeof(pstPak->SundriesPackage.RoleGrids[0]));	
			pstPak->SundriesPackage.Num++;

			pstCSRoleArm = pstPackageChg->ArmChg.Arms+pstPackageChg->ArmChg.ChgNum;
			pstCSRoleArm->ListType = LIST_TYPE_SUNDRIES;
			memcpy(&pstCSRoleArm->Arm, &stGrid.GridData.RoleArm, sizeof(pstCSRoleArm->Arm));
			pstPackageChg->ArmChg.ChgNum++;
			break;
		default:
			return -1;
			break;
	}

	
	Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	//更换avatar
	z_update_avatar(pstEnv, pstPlayer);

	z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);

	z_arm_skill_update(pstEnv, pstPlayer);

	//删除操作后，要重新获取下位置
	iGunerWearPosLeft =  check_gunner_unwear_pos_left( pstEnv,  pstPlayer, pstItemDef);
	if( iGunerWearPosLeft != -1)
	{
		// 如果枪手有副手武器,把它卸下来
		if( 0 != package_gunner_unwear_pos_left( pstAppCtx,  pstEnv, pstPlayer, iGunerWearPosLeft) )
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player(%d) guner unwear pos left fail!", pstPlayer->stRoleData.Uin);
		}
	}
	return 0;
}

// 检查失效道具
int z_login_check_vaildend(ZONESVRENV* pstEnv, Player *pstPlayer,int iVaildEnd, const char* pszItemName)
{
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	if (iVaildEnd < iCurr)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PAK210, pszItemName);
		return 2;
	}
	else if (iVaildEnd < iCurr+10*60)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PAK211, pszItemName);
		return 1;
	}
	else if(iVaildEnd < iCurr+3*60*60)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PAK212, pszItemName);
		return 1;
	}
	else if (iVaildEnd < iCurr+24*60*60)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PAK213, pszItemName);
		return 1;
	}
	else
	{
		return 0;
	}
}


int z_check_wear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	unsigned int uiOldDur;
	CSPKG stPkg;
	int iAvatarChg = 0;
	int iAttrValChg = 0;
	ITEMDEF *pstItemDef;
	ROLEARM *pstRoleArm;
	CSROLEARM *pstCSRoleArm;
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstPackageChg = &pstPackageInfo->PackageInfoData.PackageChg;
	int iRet = 0;
	char cOldPos = pstPlayer->stOnline.cClanPos;

	
	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstPackageChg->ItemChg.ChgNum = 0;
	pstPackageChg->ArmChg.ChgNum = 0;

	
	pstPlayer->stOnline.cClanPos = get_world_city_clan_pos(pstEnv, pstPlayer);
	
	for (i=pstRoleWear->ArmNum-1; i>=0; i--)
	{
		pstRoleArm = pstRoleWear->RoleArms+i;
		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleArm);
		if (NULL == pstItemDef)
		{
			continue;
		}

		// 刻印时间到期
		if (	pstRoleArm->KeYinEnd > 0 && 
			pstRoleArm->KeYinEnd < pstAppCtx->stCurr.tv_sec &&
			pstRoleArm->KeYinEnd > pstAppCtx->stCurr.tv_sec -20)
		{
			iAttrValChg = 1; // 更新属性

			pstRoleArm->KeYinEnd = 0;
			pstRoleArm->KeYinAddAttr.AddAttrID = 0;
			pstRoleArm->KeYinAddAttr.Val = 0;
			
			pstCSRoleArm = pstPackageChg->ArmChg.Arms+pstPackageChg->ArmChg.ChgNum;
			pstCSRoleArm->ListType = LIST_TYPE_WEAR;
			memcpy(&pstCSRoleArm->Arm, pstRoleArm, sizeof(pstCSRoleArm->Arm));
			pstPackageChg->ArmChg.ChgNum++;
		}

		/*if (pstItemDef->ValidTime > 0 && pstRoleArm->ValidEnd < pstAppCtx->stCurr.tv_sec)
		{
			z_wear_del(pstEnv, pstPlayer, pstRoleArm, pstPackageChg);
			iAvatarChg = 1;
			iAttrValChg = 1;
			z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM_VALIDEND, pstItemDef->Name);	
		}
		else if (pstPlayer->stOnline.iSecCount == 10 && 
				pstRoleArm->ValidEnd > 0 && 
				pstRoleArm->ValidEnd - pstAppCtx->stCurr.tv_sec <= 86400*3)
		{
			z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "%s即将到期,请注意查看", pstItemDef->Name);
		}*/

		// 身上有工会装备并且权限改变了
		if (	pstItemDef->WearPre.ClanLimit > 0 &&
			cOldPos != pstPlayer->stOnline.cClanPos)
		{
			iAvatarChg = 1;
			iAttrValChg = 1;
		}
		/*if (z_pkg_check_clanlimit(pstEnv, pstPlayer,pstItemDef->WearPre.ClanLimit) < 0)
		{
			// 保证只通知一次
			if (cOldPos != pstPlayer->stOnline.cClanPos)
			{
				iAvatarChg = 1;
				iAttrValChg = 1;
			}
		}*/
		
		if (	pstRoleArm->ValidEnd > 0)
		{
			// 上线通知一次
			if (pstPlayer->stOnline.iSecCount == 10)
			{
				iRet = z_login_check_vaildend(pstEnv, pstPlayer,pstRoleArm->ValidEnd, pstItemDef->Name);
				if (iRet == 2)
				{
					iAvatarChg = 1;
					iAttrValChg = 1;
				}
			}
			else	
			{
				if (	pstRoleArm->ValidEnd < pstAppCtx->stCurr.tv_sec  && 
					pstRoleArm->ValidEnd > pstAppCtx->stCurr.tv_sec - 15)
				{
					iAvatarChg = 1;
					iAttrValChg = 1;
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PAK210, pstItemDef->Name);
				}	
			}
		}
		
		if (pstPlayer->stOnline.tFightStatEnd > pstAppCtx->stCurr.tv_sec && pstItemDef->ArmDur.FightStatusDur > 0)
		{
			uiOldDur = pstRoleArm->Dur/DUR_UNIT;
			if (pstRoleArm->Dur > pstItemDef->ArmDur.FightStatusDur*DUR_UNIT)
			{
				//目前企划填表认为是实际值，以后可以改为更小的单位
				pstRoleArm->Dur -= pstItemDef->ArmDur.FightStatusDur*DUR_UNIT;
			}
			else
			{
				pstRoleArm->Dur = 0;
			}

			if (uiOldDur != pstRoleArm->Dur/DUR_UNIT)
			{
				pstCSRoleArm = pstPackageChg->ArmChg.Arms+pstPackageChg->ArmChg.ChgNum;
				pstCSRoleArm->ListType = LIST_TYPE_WEAR;
				memcpy(&pstCSRoleArm->Arm, pstRoleArm, sizeof(pstCSRoleArm->Arm));
				pstPackageChg->ArmChg.ChgNum++;

				if (0 == pstRoleArm->Dur/DUR_UNIT)
				{
					iAttrValChg = 1;
				}
			}
		}

		if(0 < back_unreal_result_check(pstEnv, pstPlayer,pstRoleArm))
		{
			iAvatarChg = 1;
			
			pstCSRoleArm = pstPackageChg->ArmChg.Arms+pstPackageChg->ArmChg.ChgNum;
			pstCSRoleArm->ListType = LIST_TYPE_WEAR;
			memcpy(&pstCSRoleArm->Arm, pstRoleArm, sizeof(pstCSRoleArm->Arm));
			pstPackageChg->ArmChg.ChgNum++;
		}
	}

	if (pstPackageChg->ArmChg.ChgNum > 0)
	{
		Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	if (iAvatarChg)
	{
		//更换avatar
		z_update_avatar(pstEnv, pstPlayer);
	}

	if (iAttrValChg)
	{
		z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);
	}

	return 0;
}

int item_use_arm_buchang(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEITEM *pstRoleItem, int iLvlSeg,CSPACKAGEUSE *pstUse)
{
	int iIdx = pstUse->SubIndex-1;
	ROLEGRID stGrid;
	ARMBUCHANGDEF *pstDef;
	
	pstDef = arm_buchang_def_find(pstEnv, iLvlSeg, pstPlayer->stRoleData.Career);
	if (!pstDef)
		return -1;

	if (iIdx < 0 ||iIdx >= MAX_ARM_BUCHANG_NUM)
	{
		return -1;
	}

	if (pstDef->ItemID[iIdx] <= 0)
	{
		return -1;
	}

	if (item_create(pstEnv->pstAppCtx, pstEnv, &stGrid, pstDef->ItemID[iIdx])  < 0)
	{
		return -1;	
	}

	stGrid.GridData.RoleItem.Num = 1;
	stGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;

	package_add(pstEnv->pstAppCtx,pstEnv,pstPlayer, &stGrid, 1, 0, IMPITEM_OPTYPE_BUCHANG);

	z_role_pickitem_oplog(pstEnv, pstPlayer, &stGrid, 1, OP_ITEM_RESULT);
	
	return 0;
	UNUSED(pstRoleItem);	//add by paraunused.pl
}

int item_use_arm_buchang_check(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEITEM *pstRoleItem, int iLvlSeg,CSPACKAGEUSE *pstUse)
{
	ARMBUCHANGDEF *pstDef;
	int iIdx = (int)pstUse->SubIndex-1;
		
	pstDef = arm_buchang_def_find(pstEnv, iLvlSeg, pstPlayer->stRoleData.Career);
	if (!pstDef)
		return -1;

	if (iIdx < 0 ||iIdx >= MAX_ARM_BUCHANG_NUM)
	{
		return -1;
	}

	if (pstDef->ItemID[iIdx] <= 0)
	{
		return -1;
	}

	if (package_get_freegrid_num(pstEnv->pstAppCtx, pstEnv, pstPlayer, LIST_TYPE_SUNDRIES) <=0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FENJIE1);
		return -1;
	}

	return 0;
	UNUSED(pstRoleItem);	//add by paraunused.pl
}

int z_check_ride_vaildend(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,Player *pstPlayer)
{
	int i;
	ROLERIDELIST *pstRideList = &pstPlayer->stRoleData.MiscInfo.RideList;
	RIDEGRID *pstRideGrid;
	int iRet;
	RIDEDEF *pstRideDef;
			
	for (i=0; i<pstRideList->Num; i++)
	{
		iRet = 0;
		pstRideGrid = &pstRideList->RideGrids[i];
		pstRideDef = z_find_ride_def(pstEnv, pstRideList->RideGrids[i].ItemID, pstRideList->RideGrids[i].Level);
		if (!pstRideDef)
			return -1;

		if (	(int)pstRideGrid->ItemID == pstPlayer->stOnline.RideItemID &&
			z_pkg_check_clanlimit(pstEnv, pstPlayer,pstRideDef->ClanLimit) < 0 )
		{
			player_do_ride_brk( pstEnv, pstPlayer, pstRideGrid->ItemID);	
		}
		
		if (pstRideGrid->Endtime > 0)
		{
			if (pstPlayer->stOnline.iSecCount == 10)
			{
				iRet = z_login_check_vaildend(pstEnv, pstPlayer,pstRideGrid->Endtime, pstRideDef->RideName);
				if (iRet == 2 && (int)pstRideGrid->ItemID == pstPlayer->stOnline.RideItemID)
				{
					player_do_ride_brk( pstEnv, pstPlayer, pstRideGrid->ItemID);	
				}
			}
			else
			{
				if (	pstRideGrid->Endtime < pstAppCtx->stCurr.tv_sec &&
					pstRideGrid->Endtime > pstAppCtx->stCurr.tv_sec - 15 &&
					(int)pstRideGrid->ItemID == pstPlayer->stOnline.RideItemID)
				{
					player_do_ride_brk( pstEnv, pstPlayer, pstRideGrid->ItemID);
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PAK210, pstRideDef->RideName);
				}
			}
		}
	}
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_check_et_pkg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ROLEITEM*pstRoleItems, unsigned short *punGrid, int iListType)
{
	int i;
	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstPackageChg = &pstPackageInfo->PackageInfoData.PackageChg;
	ROLEITEM *pstRoleItem;
	ITEMDEF *pstItemDef;
//	CSROLEITEM *pstCSRoleItem;
//	RESULT *pstResult;
//	ROLEPACKAGE *pstPkg = &pstPlayer->stRoleData.Package;
//	 int iType;
	
	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstPackageChg->ItemChg.ChgNum = 0;
	pstPackageChg->ArmChg.ChgNum = 0;
	
	for (i=(*punGrid)-1; i>=0; i--)
	{
		pstRoleItem = pstRoleItems + i;
		if (pstRoleItem->ValidEnd > 0)
		{
			pstItemDef = z_get_itemdef_roleitem(pstEnv, pstRoleItem);
			if (!pstItemDef)
			{
				return -1;
			}

			if (pstPlayer->stOnline.iSecCount == 10)
			{
				z_login_check_vaildend(pstEnv, pstPlayer,pstRoleItem->ValidEnd, pstItemDef->Name);
			}
		}
		/*iType = -2;
		pstRoleItem = pstRoleItems + i;
		if (0 != pstRoleItem->ValidEnd && pstRoleItem->ValidEnd < pstAppCtx->stCurr.tv_sec)
		{
			pstItemDef = z_get_itemdef_roleitem(pstEnv, pstRoleItem);
			if (!pstItemDef)
			{
				return -1;
			}

			if (iListType == LIST_TYPE_ET_STASH)
			{
				pstResult = z_item_get_result(pstItemDef, RESULT_ADD_SUNDPKG_GRID);
				if (!pstResult)
				{
					return -1;
				}
				
				for (j=0; j<pstPkg->StashPackage.Num; j++)
				{
					if (	pstPkg->StashPackage.RoleGrids[j].GridData.RoleItem.GridIdx >= 
						pstPkg->StashPackage.MaxNum - pstResult->ResultVal1)
					{
						break;
					}
				}
				if (j < pstPkg->StashPackage.Num)
					continue;

				iType = LIST_TYPE_STASH;
			}
			else if (iListType == LIST_TYPE_ET_SUND)
			{
				pstResult = z_item_get_result(pstItemDef, RESULT_ADD_SUNDPKG_GRID);
				if (pstResult)
				{
					for (j=0; j<pstPkg->SundriesPackage.Num; j++)
					{
						if (	pstPkg->SundriesPackage.RoleGrids[j].GridData.RoleItem.GridIdx >= 
							pstPkg->SundriesPackage.MaxNum - pstResult->ResultVal1)
						{
							break;
						}
					}
					if (j < pstPkg->SundriesPackage.Num)
						continue;
					
					iType = LIST_TYPE_SUNDRIES;
				}

				pstResult = z_item_get_result(pstItemDef, RESULT_ADD_STUFFPKG_GRID);
				if (pstResult)
				{
					if (iType == LIST_TYPE_SUNDRIES)
					{
						return -1;
					}
					
					for (j=0; j<pstPkg->StuffPackage.Num; j++)
					{
						if (	pstPkg->StuffPackage.RoleItems[j].GridIdx >= 
							pstPkg->StuffPackage.MaxNum - pstResult->ResultVal1)
						{
							break;
						}
					}

					if (j < pstPkg->StuffPackage.Num)
						continue;
					
					iType = LIST_TYPE_STUFF;
				}
			}
			else
			{
				return -1;
			}

			
			z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM_VALIDEND, pstItemDef->Name);
			
			if (pstPackageChg->ItemChg.ChgNum < MAX_CHG_NUM)
			{
				pstCSRoleItem = &pstPackageChg->ItemChg.Items[pstPackageChg->ItemChg.ChgNum++];
				pstCSRoleItem->ListType = iListType;
				memcpy(&pstCSRoleItem->Item, pstRoleItem, sizeof(pstCSRoleItem->Item));
				pstCSRoleItem->Item.Num = 0;
			}

			*punGrid = *punGrid - 1;
			if (i != *punGrid)
			{
				memcpy(pstRoleItems+i, pstRoleItems+(*punGrid), sizeof(ROLEITEM));
			}

			if (iType != -2)
			{
				pkg_num_chg(pstEnv, pstPlayer, iType);
			}
		}*/
	}

	/*if (pstPackageChg->ItemChg.ChgNum > 0)
	{
		Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}*/

	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(iListType);	//add by paraunused.pl
}

int z_check_grid(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ROLEGRID *pstRoleGrids, unsigned short *punGrid, int iListType)
{
	int i;
	CSPKG stPkg;
	ITEMDEF *pstItemDef;
	ROLEGRID *pstGrid;
	ROLEARM *pstRoleArm;
	ROLEITEM *pstRoleItem;
	CSROLEARM *pstCSRoleArm;
//	CSROLEITEM *pstCSRoleItem;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstPackageChg = &pstPackageInfo->PackageInfoData.PackageChg;
	
	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstPackageChg->ItemChg.ChgNum = 0;
	pstPackageChg->ArmChg.ChgNum = 0;

	for (i=(*punGrid)-1; i>=0; i--)
	{
		pstGrid = pstRoleGrids+i;
		if (pstGrid->Type == GRID_ITEM_ARM)
		{
			pstRoleArm = &pstGrid->GridData.RoleArm;

			// 刻印时间到期
			if (pstRoleArm->KeYinEnd > 0 && pstRoleArm->KeYinEnd < pstAppCtx->stCurr.tv_sec)
			{
				pstRoleArm->KeYinEnd = 0;
				pstRoleArm->KeYinAddAttr.AddAttrID = 0;
				pstRoleArm->KeYinAddAttr.Val = 0;
				
				pstCSRoleArm = pstPackageChg->ArmChg.Arms+pstPackageChg->ArmChg.ChgNum;
				pstCSRoleArm->ListType = iListType;
				memcpy(&pstCSRoleArm->Arm, pstRoleArm, sizeof(pstCSRoleArm->Arm));
				pstPackageChg->ArmChg.ChgNum++;
			}

			pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleArm);
			if (!pstItemDef)
				continue;
			
			if (	pstRoleArm->ValidEnd > 0)
			{
				if (pstPlayer->stOnline.iSecCount == 10)
				{
					z_login_check_vaildend(pstEnv, pstPlayer,pstRoleArm->ValidEnd, pstItemDef->Name);
				}
			}
			if (0 != pstRoleArm->ValidEnd && pstRoleArm->ValidEnd < pstAppCtx->stCurr.tv_sec)
			{
				if (pstRoleArm->WID == pstPlayer->stOnline.stMachineInfo.ullLockItemWID)
				{
					tmp_item_drop(pstEnv,pstPlayer,1);
				}
			}

			if(0 < back_unreal_result_check(pstEnv, pstPlayer,pstRoleArm))
			{			
				pstCSRoleArm = pstPackageChg->ArmChg.Arms+pstPackageChg->ArmChg.ChgNum;
				pstCSRoleArm->ListType = iListType;
				memcpy(&pstCSRoleArm->Arm, pstRoleArm, sizeof(pstCSRoleArm->Arm));
				pstPackageChg->ArmChg.ChgNum++;
			}
			
			/*if (0 != pstRoleArm->ValidEnd && pstRoleArm->ValidEnd < pstAppCtx->stCurr.tv_sec)
			{
				pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleArm);
				if (pstItemDef)
				{
					z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM_VALIDEND, pstItemDef->Name);
				}

				if (pstPackageChg->ArmChg.ChgNum < MAX_CHG_NUM)
				{
					pstCSRoleArm = &pstPackageChg->ArmChg.Arms[pstPackageChg->ArmChg.ChgNum++];
					pstCSRoleArm->ListType = iListType;
					memcpy(&pstCSRoleArm->Arm, pstRoleArm, sizeof(pstCSRoleArm->Arm));
					pstCSRoleArm->Arm.Num = 0;
				}

				*punGrid = *punGrid - 1;
				if (i != *punGrid)
				{
					memcpy(pstRoleGrids+i, pstRoleGrids+(*punGrid), sizeof(ROLEGRID));
				}
			}
			else if (pstPlayer->stOnline.iSecCount == 10 && 
					0 != pstRoleArm->ValidEnd  && 
					pstRoleArm->ValidEnd - pstAppCtx->stCurr.tv_sec <= 86400*3)
			{
				pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleArm);
				if (pstItemDef)
				{
					z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "%s即将到期,请注意查看", pstItemDef->Name);
				}
			}*/
		}
		else if (pstGrid->Type == GRID_ITEM_ITEM)
		{
			pstRoleItem = &pstGrid->GridData.RoleItem;
			pstItemDef = z_get_itemdef_roleitem(pstEnv, pstRoleItem);
			if (!pstItemDef)
				continue;
			
			if (	pstRoleItem->ValidEnd > 0)
			{
				if (pstPlayer->stOnline.iSecCount == 10)
				{
					z_login_check_vaildend(pstEnv, pstPlayer,pstRoleItem->ValidEnd, pstItemDef->Name);
				}
			}

			if (0 != pstRoleItem->ValidEnd && pstRoleItem->ValidEnd < pstAppCtx->stCurr.tv_sec)
			{
				if (pstRoleItem->WID == pstPlayer->stOnline.stMachineInfo.ullLockItemWID)
				{
					tmp_item_drop(pstEnv,pstPlayer,1);
				}
			}
			
			/*if (0 != pstRoleItem->ValidEnd && pstRoleItem->ValidEnd < pstAppCtx->stCurr.tv_sec)
			{
				pstItemDef = z_get_itemdef_roleitem(pstEnv, pstRoleItem);
				if (pstItemDef)
				{
					z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM_VALIDEND, pstItemDef->Name);
				}

				if (pstPackageChg->ItemChg.ChgNum < MAX_CHG_NUM)
				{
					pstCSRoleItem = &pstPackageChg->ItemChg.Items[pstPackageChg->ItemChg.ChgNum++];
					pstCSRoleItem->ListType = iListType;
					memcpy(&pstCSRoleItem->Item, pstRoleItem, sizeof(pstCSRoleItem->Item));
					pstCSRoleItem->Item.Num = 0;
				}

				*punGrid = *punGrid - 1;
				if (i != *punGrid)
				{
					memcpy(pstRoleGrids+i, pstRoleGrids+(*punGrid), sizeof(ROLEGRID));
				}
			}
			else if (pstPlayer->stOnline.iSecCount == 10 && 
					0 != pstRoleItem->ValidEnd && 
					pstRoleItem->ValidEnd - pstAppCtx->stCurr.tv_sec <= 86400*3)
			{
				pstItemDef = z_get_itemdef_roleitem(pstEnv, pstRoleItem);
				if (pstItemDef)
				{
					z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "%s即将到期,请注意查看", pstItemDef->Name);
				}
			}*/
		}
		else
		{
			continue;
		}
	}

	if (pstPackageChg->ArmChg.ChgNum > 0 || pstPackageChg->ItemChg.ChgNum > 0)
	{
		Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	return 0;
}

//宠物装备暂时不计算耐久
/*
int z_check_pet_wear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i, j;
	int iArmChg ;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;
	ITEMDEF *pstItemDef;
	ROLEPET *pstRolePet;
	ROLEARM *pstRoleArm;
	
	for (i=pstPetPackage->ShiNum-1; i>=0; i--)
	{
		iArmChg = 0;
		pstRolePet = &pstPetPackage->RoleShiPets[i];
		for (j=pstRolePet->PetArms.Num-1; j>=0; j--)
		{
			pstRoleArm = pstRolePet->PetArms.Arms + j;
			if (pstRoleArm->ValidEnd != 0 && pstRoleArm->ValidEnd < pstAppCtx->stCurr.tv_sec)
			{
				pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleArm);
				if (pstItemDef)	
				{
					z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM_VALIDEND, pstItemDef->Name);
				}
				
				pstRolePet->PetArms.Num--;
				if (j != pstRolePet->PetArms.Num)
				{
					memcpy(pstRolePet->PetArms.Arms+j, pstRolePet->PetArms.Arms+pstRolePet->PetArms.Num,
							sizeof(pstRolePet->PetArms.Arms[0]));
				}
				
				iArmChg = 1;
			}
		}

		if (iArmChg)
		{
			pet_armchange_to_clt(pstEnv, pstPlayer, pstRolePet);
			pet_update_online(pstEnv, pstPlayer, pstRolePet);
		}
	}
	
	return 0;
}
*/

int z_check_player_package(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	z_check_wear(pstAppCtx, pstEnv, pstPlayer);
	//z_check_pet_wear(pstAppCtx, pstEnv, pstPlayer);
	z_check_grid(pstAppCtx, pstEnv, pstPlayer, pstPlayer->stRoleData.Package.SundriesPackage.RoleGrids, 
						&pstPlayer->stRoleData.Package.SundriesPackage.Num, LIST_TYPE_SUNDRIES);
	z_check_grid(pstAppCtx, pstEnv, pstPlayer, pstPlayer->stRoleData.Package.StashPackage.RoleGrids, 
						&pstPlayer->stRoleData.Package.StashPackage.Num, LIST_TYPE_STASH);
	z_check_et_pkg(pstAppCtx, pstEnv, pstPlayer, pstPlayer->stRoleData.Package.EtStashPkg.RoleItems,
						&pstPlayer->stRoleData.Package.EtStashPkg.Num, LIST_TYPE_ET_STASH);
	z_check_et_pkg(pstAppCtx, pstEnv, pstPlayer, pstPlayer->stRoleData.Package.EtSundPkg.RoleItems,
						&pstPlayer->stRoleData.Package.EtSundPkg.Num, LIST_TYPE_ET_SUND);
	z_check_ride_vaildend(pstAppCtx, pstEnv,pstPlayer);
	return 0;
}

int player_die_wear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iDurRate)
{
	int i;
	unsigned int uiDurDec, uiOldDur;
	int iAttrValChg = 0;
	CSPKG stPkg;
	ROLEARM *pstRoleArm;
	ITEMDEF *pstItemDef;
	CSROLEARM *pstCSRoleArm;
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstPackageChg = &pstPackageInfo->PackageInfoData.PackageChg;

	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstPackageChg->ItemChg.ChgNum = 0;
	pstPackageChg->ArmChg.ChgNum = 0;
	
	for (i=0; i<pstRoleWear->ArmNum; i++)
	{
		pstRoleArm = pstRoleWear->RoleArms+i;
		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleArm);
		if (NULL == pstItemDef || 0 == pstItemDef->ArmDur.MaxDur)
		{
			continue;
		}

		uiDurDec = iDurRate*z_get_arm_max_dur(pstEnv, pstItemDef)*DUR_UNIT/100.0;

		uiOldDur = pstRoleArm->Dur/DUR_UNIT;
		if (pstRoleArm->Dur > uiDurDec)
		{
			pstRoleArm->Dur -= uiDurDec;
		}
		else
		{
			pstRoleArm->Dur = 0;
		}

		if (uiOldDur != pstRoleArm->Dur/DUR_UNIT)
		{
			pstCSRoleArm = pstPackageChg->ArmChg.Arms+pstPackageChg->ArmChg.ChgNum;
			pstCSRoleArm->ListType = LIST_TYPE_WEAR;
			memcpy(&pstCSRoleArm->Arm, pstRoleArm, sizeof(pstCSRoleArm->Arm));
			pstPackageChg->ArmChg.ChgNum++;

			if (0 == pstRoleArm->Dur/DUR_UNIT)
			{
				iAttrValChg = 1;
			}
		}
		
	}

	if (pstPackageChg->ArmChg.ChgNum > 0)
	{
		Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	if (iAttrValChg)
	{
		z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);
	}
	
	return 0;
}

// 计算额外的耐久度(Result)
int z_arm_extra_dur(ITEMDEF *pstItemDef)
{
	int i, iExtraDur = 0;
	
	i = MAX_ITEM_RESULT;
	while(i--)
	{
		// 计算额外的耐久度上限
		if(pstItemDef->Result[i].ResultID == RESULT_ADD_MAX_DUR)
		{
			iExtraDur = (pstItemDef->Result[i].ResultVal1 * pstItemDef->ArmDur.MaxDur) / 100 + pstItemDef->Result[i].ResultVal2;
			break;
		}
	}
	return iExtraDur;
}

//关键字增加耐久度
int z_get_arm_addattr_dur(ZONESVRENV* pstEnv, ITEMDEF *pstItemDef)
{
	/*
	int i, iDur = 0;
	
	for (i=0; i<MAX_ARM_ADD_ATTR_RESULT; i++)	
	{
		if (0 == pstItemDef->AddAttr[i].ID)
		{
			break;
		}

		if (ADD_ATTR_RESULT_MAX_DUR == pstItemDef->AddAttr[i].ID)
		{
			ARMADDATTRDEF *pstArmAddAttr;
			
			pstArmAddAttr = z_find_armaddattrdef(pstEnv, pstItemDef->AddAttr[i].ID);
			if (!pstArmAddAttr)
			{
				continue;
			}

			if (pstItemDef->AddAttr[i].Idx == 0 || pstItemDef->AddAttr[i].Idx > MAX_ARM_ADD_ATTR_RESULT)
			{
				continue;
			}

			iDur = pstArmAddAttr->Result[pstItemDef->AddAttr[i].Idx-1].ResultVal1*(pstItemDef->ArmDur.MaxDur/100.0);	
			break;
		}
	}

	return iDur;
	*/
	UNUSED(pstEnv);
	UNUSED(pstItemDef);
	return 0;
}

int z_get_arm_max_dur(ZONESVRENV* pstEnv, ITEMDEF *pstItemDef)
{
	return pstItemDef->ArmDur.MaxDur + z_arm_extra_dur(pstItemDef) + z_get_arm_addattr_dur(pstEnv, pstItemDef);
}


int z_arm_repair_price(ZONESVRENV* pstEnv, ROLEARM *pstRoleArm, ITEMDEF *pstItemDef,Player *pstPlayer)
{
	int iPrice;
	unsigned int unReBate;
	
	if (pstItemDef->ArmDur.MaxDur> 0)
	{
		// 持久为0     维修系数1.5      否则1.0
		if (pstRoleArm->Dur <= 0)
		{
			iPrice = pstItemDef->RepairBull * BROKEN/100.0;
		}
		else
		{
			iPrice = (1.0 - (pstRoleArm->Dur/DUR_UNIT)*1.0/(z_get_arm_max_dur(pstEnv,pstItemDef)))*pstItemDef->RepairBull * NOTBROKEN/100;
		}
	}
	else
	{
		iPrice = 0;
	}

	player_cacl_rebate(pstEnv, pstPlayer, &unReBate, NULL, NULL);

	return iPrice *= 1.0*unReBate/100;;
}

int z_arm_sell_price(ZONESVRENV* pstEnv, ROLEARM *pstRoleArm, ITEMDEF *pstItemDef)
{
	int iPrice;
	
	if (pstItemDef->ArmDur.MaxDur > 0)
	{
		iPrice = ((pstRoleArm->Dur/DUR_UNIT)*1.0/(z_get_arm_max_dur(pstEnv,pstItemDef)))*pstItemDef->SellPrice;
	}
	else
	{
		iPrice = pstItemDef->SellPrice;
	}

	return iPrice;
}

int z_arm_dur_dec(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEARM *pstRoleArm, int iDurDec)
{
	unsigned int uiOldDur;
	CSROLEARM *pstCSRoleArm;
	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstPackageChg = &pstPackageInfo->PackageInfoData.PackageChg;
	int iAttrValChg = 0;

	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstPackageChg->ItemChg.ChgNum = 0;
	pstPackageChg->ArmChg.ChgNum = 0;

	uiOldDur = pstRoleArm->Dur/DUR_UNIT;
	if ((int)pstRoleArm->Dur > iDurDec)
	{
		pstRoleArm->Dur -= iDurDec;
	}
	else
	{
		pstRoleArm->Dur = 0;
	}

	if (uiOldDur != pstRoleArm->Dur/DUR_UNIT)
	{
		pstCSRoleArm = pstPackageChg->ArmChg.Arms+pstPackageChg->ArmChg.ChgNum;
		pstCSRoleArm->ListType = LIST_TYPE_WEAR;
		memcpy(&pstCSRoleArm->Arm, pstRoleArm, sizeof(pstCSRoleArm->Arm));
		pstPackageChg->ArmChg.ChgNum++;

		if (0 == pstRoleArm->Dur/DUR_UNIT)
		{
			iAttrValChg = 1;
		}
	}

	if (pstPackageChg->ArmChg.ChgNum > 0)
	{
		Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	if (iAttrValChg)
	{
		z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}
	
	return 0;	
}

int player_package_check_mapitem(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	CSPKG stPkg;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstRolePakChg;
	ITEMDEF *pstItemDef;
	ROLEFAIRY *pstRoleFairy;

	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstRolePakChg = &pstPackageInfo->PackageInfoData.PackageChg;
	pstRolePakChg->ArmChg.ChgNum = 0;
	pstRolePakChg->ItemChg.ChgNum = 0;

	for (i=pstPak->StuffPackage.Num-1; i>=0; i--)
	{
		pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstPak->StuffPackage.RoleItems[i]);
		if (pstItemDef && pstItemDef->MapItem)
		{
			package_dec_in_item(pstItemDef->ItemID, pstPak->StuffPackage.RoleItems, &pstPak->StuffPackage.Num, 
								LIST_TYPE_STUFF, &pstPak->StuffPackage.RoleItems[i].Num, &i, pstRolePakChg,PACKAGE_DEC_NONE);
		}
	}

	for (i=pstPak->TaskPackage.Num-1; i>=0; i--)
	{
		pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstPak->TaskPackage.RoleItems[i]);
		if (pstItemDef && pstItemDef->MapItem)
		{
			package_dec_in_item(pstItemDef->ItemID, pstPak->TaskPackage.RoleItems, &pstPak->TaskPackage.Num, 
								LIST_TYPE_TASK, &pstPak->TaskPackage.RoleItems[i].Num, &i, pstRolePakChg,PACKAGE_DEC_NONE);
		}
	}

	for (i=pstPak->SundriesPackage.Num-1; i>=0; i--)
	{
		pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem);
		if (pstItemDef && pstItemDef->MapItem)
		{
			package_dec_in_grid(pstItemDef->ItemID, pstPak->SundriesPackage.RoleGrids, &pstPak->SundriesPackage.Num, 
				LIST_TYPE_SUNDRIES, &pstPak->SundriesPackage.RoleGrids[i].GridData.RoleItem.Num, &i, pstRolePakChg,PACKAGE_DEC_NONE);
		}
	}

	for (i=pstPak->StashPackage.Num-1; i>=0; i--)
	{
		pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstPak->StashPackage.RoleGrids[i].GridData.RoleItem);
		if (pstItemDef && pstItemDef->MapItem)
		{
			package_dec_in_grid(pstItemDef->ItemID, pstPak->StashPackage.RoleGrids, &pstPak->StashPackage.Num, 
				LIST_TYPE_STASH, &pstPak->StashPackage.RoleGrids[i].GridData.RoleItem.Num, &i, pstRolePakChg,PACKAGE_DEC_NONE);
		}
	}
	
	//清除临时精灵
	for (i=pstPak->FairyPackage.FairyNum-1; i>=0; i--)
	{
		pstRoleFairy = &pstPak->FairyPackage.RoleFairys[i];

		if (pstRoleFairy ->Flag & FAIRY_CTL_FLAG_TMP)
		{
			set_fairy_drop( pstEnv, pstPlayer, pstRoleFairy);
			pstPak->FairyPackage.FairyNum --;
			if (i != pstPak->FairyPackage.FairyNum)
			{
				memcpy(pstRoleFairy, &pstPak->FairyPackage.RoleFairys[pstPak->FairyPackage.FairyNum], sizeof(*pstRoleFairy));	
			}
		}
	}
	/*
	
	for (i=pstPak->PetPackage.XianNum-1; i>=0; i--)
	{
		pstRolePet = &pstPak->PetPackage.RoleXianPets[i];
		if (pstRolePet->StatFlag & PET_STAT_TEMP)
		{
			PETDEF *pstPetDef = NULL;
			
			pstPetDef = pet_find_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
			if (pstPetDef && (pstRolePet->StatFlag & PET_STAT_FIGHT))
			{
				pet_set_stat( pstEnv, pstPlayer, pstRolePet, 0);
			}
			package_pet_lose(pstEnv, pstPlayer, pstRolePet->WID);
		}
	}

	//清除临时宠物
	for (i=pstPak->PetPackage.ShiNum-1; i>=0; i--)
	{
		pstRolePet = &pstPak->PetPackage.RoleShiPets[i];
		if (pstRolePet->StatFlag & PET_STAT_TEMP)
		{
			PETDEF *pstPetDef = NULL;
			
			pstPetDef = pet_find_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
			if (pstPetDef && (pstRolePet->StatFlag & PET_STAT_FIGHT))
			{
				pet_set_stat( pstEnv, pstPlayer, pstRolePet, 0);
			}
			package_pet_lose(pstEnv, pstPlayer, pstRolePet->WID);
		}
	}
	*/
	
	if (pstRolePakChg->ArmChg.ChgNum > 0 || pstRolePakChg->ItemChg.ChgNum > 0)
	{
		Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	
	return 0;
}

#if 0
void z_check_ride_use( ZONESVRENV* pstEnv,Player *pstPlayer)
{	
	int i, flag = 0;
	unsigned int uiRideItemID = pstPlayer->stOnline.RideItemID;
	ROLERIDELIST   *pstRideList = &pstPlayer->stRoleData.MiscInfo.RideList;

	for (i=pstRideList->Num-1; i>=0; i--)	
	{
		if((CS_STAT_RIDE_ON & pstPlayer->stOnline.State) ||  (CS_STAT_FLY & pstPlayer->stOnline.State)) 
		{
			if ( uiRideItemID == pstRideList->RideGrids[i].ItemID)
			{continue;}
		}
		
		if (pstRideList->RideGrids[i].Endtime != 0 &&  (pstRideList->RideGrids[i].Endtime <= pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			char szMsg[256];
			RIDEDEF *pstRideDef;
			
			pstRideDef = z_find_ride_def(pstEnv, pstRideList->RideGrids[i].ItemID, pstRideList->RideGrids[i].Level);
			if (pstRideDef)
			{
				sprintf(szMsg, "%s 已到使用期限。", pstRideDef->RideName);
				z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, szMsg);
			}
			
			flag = 1;
			pstRideList->Num--;
			if (i != pstRideList->Num)
			{
				memmove(&pstRideList->RideGrids[i], &pstRideList->RideGrids[i+1], (pstRideList->Num-i)*sizeof(pstRideList->RideGrids[0]));
			}
		}
	}
	
	if (flag)
	{
		ride_list_chg(pstEnv, pstPlayer);
	}
	
	return ;

}
#endif

#if 0
int package_fittings_inlay(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,CSPACKAGEUSE *pstUse, RESULT *pstResult)
{
	FITTINGSDEF *pstFittingDef = NULL;
	ZoneSkill *pstZoneSkill = NULL;
	SKILLDEF *pstSkillDef = NULL;
	FITTINGSMAPLISTDATA *pstFittingsMap = NULL;      
	int i=0;
	unsigned short ushDefIdx = 0;
	UNUSED(pstAppCtx);

	if( 0 > package_fittings_inlay_check(pstAppCtx, pstEnv,pstPlayer,pstUse,pstResult) )
	{
		return -1;
	}

	/* 把定义找出来 */	
	pstZoneSkill = z_get_player_zoneskill(pstPlayer, (unsigned short)pstUse->WID);
	if ( NULL == pstZoneSkill )
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "fittings_inlay_check 没找到用户的技能(%llu) ",pstUse->WID);
		return -1;
	}
	pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill);	
	if (!pstSkillDef)	
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "fittings_inlay_check 没找到用户的技能(%llu) ",pstUse->WID);
		return -1;
	}

	if( pstUse->SubIndex >= pstSkillDef->FittingsGridNum )
	{
		return -1;
	}
		
	for(i=0; i<MAX_SKILL_FITTINGS; i++ )
	{
		if(pstResult->ResultVal1 == pstSkillDef->FittingLists[i].ID)
		{
			if(pstResult->ResultVal2 == 0 )
			{
				pstFittingDef = z_find_fittings(pstEnv, pstResult->ResultVal1, 1, pstSkillDef->FittingLists[i].Group,&ushDefIdx);
			}
			else
			{
				pstFittingDef = z_find_fittings( pstEnv, pstResult->ResultVal1, 
													pstResult->ResultVal2, 
													pstSkillDef->FittingLists[i].Group,&ushDefIdx);
			}
			break;
		}
	}
	if (!pstFittingDef)
	{
		return -1;
	}
	
	/* 装备配件 */
	pstFittingsMap = &pstZoneSkill->stSkillData.CSFittingsRowList[0].FittingsMap;
	
	pstFittingsMap->FittingsGrids[pstUse->SubIndex].FittingsID = pstFittingDef->FittingsID;
	pstFittingsMap->FittingsGrids[pstUse->SubIndex].DefIdx = ushDefIdx;
	pstFittingsMap->FittingsGrids[pstUse->SubIndex].GroupID = pstFittingDef->GroupID;
	pstFittingsMap->FittingsGrids[pstUse->SubIndex].Level = pstFittingDef->Level;	

	/* 有连续三个包下去,看客户端能否处理先*/
	z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, "技能的配件镶嵌成功");
	z_update_one_skill( pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstZoneSkill);
	z_skill_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstZoneSkill);
	return 0;
}
#endif

#if 0
int package_fittings_inlay_check(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,CSPACKAGEUSE *pstUse, RESULT *pstResult)
{ 
	// result1->配件ID
	FITTINGSDEF *pstFittingDef = NULL;
	ZoneSkill *pstZoneSkill = NULL;
	SKILLDEF *pstSkillDef = NULL;   
	unsigned int i=0;
	unsigned short ushDefIdx = 0;
	UNUSED(pstAppCtx);

	if( pstUse->SubIndex >= MAX_FITTINGS_GRID )
	{
		return -1;
	}
	/* 把定义找出来 */	
	pstZoneSkill = z_get_player_zoneskill(pstPlayer, (unsigned short)pstUse->WID);
	if ( NULL == pstZoneSkill )
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "fittings_inlay_check 没找到用户的技能(%llu) ",pstUse->WID);
		return -1;
	}
	pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill);	
	if (!pstSkillDef)	
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "fittings_inlay_check 没找到用户的技能(%llu) ",pstUse->WID);
		return -1;
	}

	if( pstUse->SubIndex >= pstSkillDef->FittingsGridNum )
	{
		return -1;
	}
		
	for(i=0; i<MAX_SKILL_FITTINGS; i++ )
	{
		if(pstResult->ResultVal1 == pstSkillDef->FittingLists[i].ID)
		{
			if(pstResult->ResultVal2 == 0 )
			{
				pstFittingDef = z_find_fittings(pstEnv, pstResult->ResultVal1, 1, pstSkillDef->FittingLists[i].Group,&ushDefIdx);
			}
			else
			{
				pstFittingDef = z_find_fittings( pstEnv, pstResult->ResultVal1, 
													pstResult->ResultVal2, 
													pstSkillDef->FittingLists[i].Group,&ushDefIdx);
			}
			break;
		}
	}
	if (!pstFittingDef)
	{
		return -1;
	}
		
	return 0;
}
#endif

int player_diathesis_up_check(ZONESVRENV* pstEnv, Player *pstPlayer, int iDiathesisID, int iLevel)
{
	DIATHESISDATA *pstDiathesis;
	DIATHESISDEF *pstDiaDef;

	pstDiaDef = z_find_diathesis(pstEnv, iDiathesisID, iLevel);
	if (!pstDiaDef)
	{
		return -1;
	}

	pstDiathesis = z_get_player_diathesis(pstPlayer, iDiathesisID);
	if (!pstDiathesis)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK122);
		return -1;
	}

	if (iLevel - pstDiathesis->DiathesisLevel != 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK123);
		return -1;
	}
	return 0;
}

int player_diathesis_up(ZONESVRENV* pstEnv, Player *pstPlayer, int iDiathesisID, int iLevel)
{
	DIATHESISDATA *pstDiathesis;
	int iPreLevel;
	DIATHESISDEF *pstDiatheDef;

	pstDiathesis = z_get_player_diathesis(pstPlayer, iDiathesisID);
	if (!pstDiathesis)
	{
		return -1;
	}

	if (iLevel - pstDiathesis->DiathesisLevel != 1)
	{
		return -1;
	}

	pstDiatheDef = z_find_diathesis(pstEnv, pstDiathesis->DiathesisID, iLevel);
	if (!pstDiatheDef)
	{
		return -1;
	}
	
	iPreLevel = pstDiathesis->DiathesisLevel;
	pstDiathesis->DiathesisLevel  = iLevel;
	z_diathesis_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstDiathesis);
	skill_action(pstEnv->pstAppCtx, pstEnv, pstPlayer, iDiathesisID, iLevel, DIATHESIS_LEVEL_UP);

	task_player_trace(pstEnv, pstPlayer, TASK_OBJ_PLAYER_DIATHESIS);

	z_role_diathesis_oplog(pstEnv, pstPlayer, OP_LEVEL_UP_DIATHESIS, iDiathesisID, iLevel, iPreLevel);
	
	return 0;
}

int player_item_do_skill_check(ZONESVRENV* pstEnv, Player *pstPlayer,  RESULT *pstResult, ZoneAni *pstTarAni)
{
	int iAtkeeRadius = 0;
	int iSkillDist;
	TMAPMASK *pstMask;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	ZONEIDX stIdx;
	RESPOS stPos;
	SKILLDEF *pstSkillDef;

	if (pstTarAni == NULL)
	{
		return -1;
	}

	/*
	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOSKILL))
	{
		return -1;
	}*/

	if (0 == z_player_act_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, RES_ACTION_ATK))
	{
		return -1;
	}

	if (pstTarAni->iType == OBJ_MONSTER)
	{
		stPos = pstTarAni->stObj.pstMon->stCurrPos;
		iAtkeeRadius =  pstTarAni->stObj.pstMon->unRadius;
	}
	else if (pstTarAni->iType == OBJ_PLAYER)
	{
		stPos = pstTarAni->stObj.pstPlayer->stRoleData.Pos;
	}
	else
	{
		return-1;
	}

	pstSkillDef = z_find_skill(pstEnv, pstResult->ResultVal1, pstResult->ResultVal2, NULL);
	if (NULL == pstSkillDef)
	{
		return -1;
	}

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst) return -1;
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}
	
	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);

	//检查打击点魔法阻挡
	if (!IsSkillThrough(pstEnv, pstMapInst, pstSkillDef, pstMask, pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y, stPos.X, stPos.Y))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PAK124);
		return -1;
	}

	//特殊目范的判断
	if (TARGET_RANGE_SELF == pstSkillDef->TargetRange) 
	{
		//使用目标是不是自己
		if (pstTarAni->iType != OBJ_PLAYER || pstPlayer != pstTarAni->stObj.pstPlayer)
		{
			return -1;
		}
	}
	else if (TARGET_RANGE_FRIEND == pstSkillDef->TargetRange)
	{
		if (pstTarAni->iType != OBJ_PLAYER )
		{
			return -1;
		}

		if (0 == player_team_same(pstEnv, pstPlayer, pstTarAni->stObj.pstPlayer))
		{
			return -1;
		}
	} 
	else
	{
		if (0 > z_player_filter_atkobj(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstTarAni, pstSkillDef,NULL))
		{
			return -1;
		}
	}

	if (pstSkillDef->DeadTarget)
	{
		iSkillDist =pstSkillDef->Distance + 6*DIST_TORRENT;
	}
	else if (pstSkillDef->MoveAttack)
	{
		iSkillDist = pstSkillDef->Distance + 3*DIST_TORRENT;
	}
	else
	{
		iSkillDist = pstSkillDef->Distance + 2*DIST_TORRENT;
	}
		
	if (pstSkillDef->MainRange.RangeType == SKILL_RANGE_NONE || pstSkillDef->MainRange.RangeType == SKILL_RANGE_SIGLE)
	{
		if (z_distance(&pstPlayer->stRoleData.Pos, &stPos)  > iSkillDist+iAtkeeRadius)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PAK125);
			return -1;
		}
	}
	else
	{
		if (0 > z_in_range(&stPos, &pstPlayer->stRoleData.Pos, &stPos, &pstSkillDef->MainRange, pstSkillDef->Distance, iAtkeeRadius, 0))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PAK125);
			return -1;
		}
	}

	//result check
	{
		int i = 0;
		SKILLRESULT* pstSkillResult = NULL;
		for(i = 0; i < MAX_SKILL_RESULT;i++)
		{
			pstSkillResult = &pstSkillDef->SkillResult[i];
			if (0 == pstSkillResult->SkillResultID)
			{
				break;
			}
			switch (pstSkillResult->SkillResultID)
			{
			case RESULT_DROP_TMP_ITEM:
				{
					Player* pstTarget = NULL;
					int k = 0;
					RESULT* pstTmpItemResult = NULL;
					ITEMDEF* pstTmpItemDef = NULL;
					int iValid = 0;
					int iImmune = 0;
					
					if (pstTarAni->iType != OBJ_PLAYER || !pstTarAni->stObj.pstPlayer)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK126);
						return -1;
					}
					pstTarget = pstTarAni->stObj.pstPlayer;

					if (0 == pstTarget->stRoleData.MiscInfo.TmpSys.TmpItemID)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK127);
						return -1;
					}
					
					pstTmpItemDef = z_find_itemdef(pstEnv,pstTarget->stRoleData.MiscInfo.TmpSys.TmpItemID);
					if (!pstTmpItemDef)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK127);
						return -1;
					}
					if (pstSkillResult->SkillResultVal1)
					{
						for (k = 0; k < MAX_ITEM_RESULT; k++)
						{
							pstTmpItemResult = pstTmpItemDef->Result + k;
							if (0 == pstTmpItemResult->ResultID)
							{
								break;
							}
							
							if (pstSkillResult->SkillResultVal1 == pstTmpItemResult->ResultID)
							{
								iValid = 1;
							}
							
							if (pstSkillResult->SkillResultVal3 == pstTmpItemResult->ResultID)
							{
								iValid = 0;
								iImmune = 1;
								break;								
							}
							
							
						}
						if (0 == iValid)
						{
							if (iImmune)
							{
								z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK128);
							}
							else
							{
								z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK127);
							}
							return -1;
						}
					}
									
				}
				break;
				
			case RESULT_STATUS:
				
				if (pstSkillResult->SkillResultVal2 == STATUS_BEACH_ID4)
				{
					ZoneStatus *pstZoneStatus = NULL;
					if (pstTarAni->iType != OBJ_PLAYER || !pstTarAni->stObj.pstPlayer)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK126);
						return -1;
					}
					
					pstZoneStatus =  z_player_find_status(pstTarAni->stObj.pstPlayer,STATUS_BEACH_ID4);
					if (pstZoneStatus)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK129);
						return -1;
					}
								
					pstZoneStatus =  z_player_find_status(pstPlayer,STATUS_BEACH_ID2);
					if (pstZoneStatus)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK130);
						return -1;
					}
				}	
				break;

			default:
					break;
			}
			
				
		}
	}

	return 0;
}

int z_fill_zoneskill_by_def(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneSkill *pstZoneSkill, SKILLDEF *pstSkillDef)
{
	UNUSED(pstEnv);
	UNUSED(pstPlayer);

	memset(pstZoneSkill, 0, sizeof(*pstZoneSkill));
	pstZoneSkill->DefIdx = -1;
	pstZoneSkill->stSkillData.SkillID = pstSkillDef->SkillID;
	pstZoneSkill->stSkillData.SkillLevel = pstSkillDef->Level;
	pstZoneSkill->stSkillData.CoolLeft = 0;

	//伪造一个配件,无配件效果,仅有技能信息
	//pstZoneSkill->stSkillData.FittingsRowNum = 1;
	//z_skill_fittings_attr(pstEnv, pstPlayer, &pstZoneSkill->stSkillData, pstSkillDef);
	return 0;
}

int z_get_skill_atklist(ZONESVRENV* pstEnv, Player *pstPlayer, SKILLDEF *pstSkillDef, ZoneAni *pstTarAni, ZoneObjList *pstZoneObjList, RESPOS *pstHitPos)
{
	int i;
	int iIndex;
	int iRadius = 0;
	char cType;
	ZoneAni *pstAni;
	TMEMBLOCK *pstMemBlock;
	RESPOS stAtkeePos;
	Player *pstPlayerTmp = NULL;
	TMAPMASK *pstMask;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	ZONEIDX stIdx;
	Player *pstAniPlayer = NULL;
	Monster *pstAniMon = NULL;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst) return -1;
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}
	
	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);
	
	pstZoneObjList->iObj = 0;

	if (pstTarAni && (0 == z_player_filter_atkobj(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstTarAni,  pstSkillDef,NULL)))
	{
		pstZoneObjList->iObj = 1;
		memcpy(&pstZoneObjList->astObj[0], pstTarAni, sizeof(pstZoneObjList->astObj[0]));
			
		if (OBJ_PLAYER == pstTarAni->iType)
		{
			pstAniPlayer = pstTarAni->stObj.pstPlayer;
		}
		else if (OBJ_MONSTER == pstTarAni->iType)
		{
			pstAniMon = pstTarAni->stObj.pstMon;
		}
		//宠物顺便看下主人
		else if(OBJ_PET == pstTarAni->iType && pstTarAni->stObj.stZonePet.pstOwner == pstPlayer)
		{
			ZoneAni stTmpAni;

			stTmpAni.iType = OBJ_PLAYER;
			stTmpAni.stObj.pstPlayer = pstPlayer;
			
			if (0 == z_player_filter_atkobj(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stTmpAni,  pstSkillDef,NULL))
			{
				memcpy(&pstZoneObjList->astObj[pstZoneObjList->iObj], &stTmpAni, sizeof(pstZoneObjList->astObj[0]));
				pstZoneObjList->iObj++;
			}
		}
		else
		{
			return -1;
		}
	}
	else
	{
		pstZoneObjList->iObj = 0;
	}

	for (i = 0; i < pstPlayer->stOnline.iVAnimate; i++)
	{
		
		if (pstZoneObjList->iObj >= pstSkillDef->TargetNum)
		{
			break;
		}

		if (pstPlayer->stOnline.aiVAnimate[i] >= PET_ID_BASE)
		{
			iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.aiVAnimate[i]%PET_ID_BASE, &cType);
		}
		else
		{
			iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv,  pstPlayer->stOnline.aiVAnimate[i], &cType);
		}

		if (0 > iIndex)
		{
			continue;
		}

		pstAni = &pstZoneObjList->astObj[pstZoneObjList->iObj];
		pstAni->iType = cType;

		if (cType == OBJ_PLAYER)
		{
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				continue;
			}
									
			pstAni->stObj.pstPlayer = pstPlayerTmp = (Player *)pstMemBlock->szData;

			//攻击宠物
			if (pstPlayer->stOnline.aiVAnimate[i] >= PET_ID_BASE)
			{
				pstAni->iType = OBJ_PET;
				pstAni->stObj.stZonePet.pstOwner = pstAni->stObj.pstPlayer;
				pstAni->stObj.stZonePet.pstPetFightShi = pet_get_shipet_by_xiedaipos(pstAni->stObj.pstPlayer, (pstPlayer->stOnline.aiVAnimate[i]/PET_ID_BASE)-1);
				if (NULL == pstAni->stObj.stZonePet.pstPetFightShi)	
				{
					continue;
				}
				pstAni->stObj.stZonePet.pstRolePet = z_find_role_pet(pstAni->stObj.pstPlayer, pstAni->stObj.stZonePet.pstPetFightShi->stRolePetOnline.ullWID, NULL);
				if (NULL == pstAni->stObj.stZonePet.pstRolePet)
				{
					continue;
				}
			}
			
			if (PLAYER_STATUS_ROLE_LOGIN != pstPlayerTmp->eStatus || 
				(0 == pstSkillDef->DeadTarget && ((pstPlayerTmp->stOnline.State & CS_STAT_DEAD) || pstPlayerTmp->stRoleData.RoleDetail.HPCurr == 0)) ||
				(pstPlayerTmp->stOnline.State & (CS_STAT_AUTOFLY | CS_STAT_QIANXIN)))
			{
				continue;
			}
			
			if (pstPlayer == pstPlayerTmp)
			{
				continue;
			}
			
			stAtkeePos = pstPlayerTmp->stRoleData.Pos;
			iRadius = 0;
		}
		else if (cType == OBJ_MONSTER)
		{
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				continue;
			}
									
			pstAni->stObj.pstMon = (Monster *)pstMemBlock->szData;
			if (AI_STAT_HOME == pstAni->stObj.pstMon->bAiStat ||
				pstAni->stObj.pstMon->bSubType >= MON_SUB_TYPE_TRAP ||
				(0 == pstSkillDef->DeadTarget && (pstAni->stObj.pstMon->cStat != MONSTER_ALIVE || pstAni->stObj.pstMon->iHP == 0)))
			{
				continue;
			}

			if (pstAni->stObj.pstMon == pstAniMon)
			{
				continue;
			}

			stAtkeePos = pstAni->stObj.pstMon->stCurrPos;
			iRadius = pstAni->stObj.pstMon->unRadius;
		}
		else
		{
			continue;
		}

		if (0 > z_player_filter_atkobj(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstAni, pstSkillDef,NULL))
		{
			continue;
		}

		if (0 > z_in_range(&stAtkeePos, &pstPlayer->stRoleData.Pos, pstHitPos, &pstSkillDef->MainRange, pstSkillDef->Distance, iRadius, 0))
		{
			continue;
		}

		if (!(pstSkillDef->NoSkillStop&NOSKILLSTOP_TYPE_MAGIC))
		{
			if (!IsPixelLineThrough(pstMask, pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y, stAtkeePos.X, stAtkeePos.Y, maskMagic) ||
				!dyn_msk_check(pstEnv, pstMapInst, pstMask, pstPlayer->stRoleData.Pos.X, 
					pstPlayer->stRoleData.Pos.Y, stAtkeePos.X, stAtkeePos.Y, 0,DYN_MSK_MAGIC))
			{
				continue;
			}
		}

		pstZoneObjList->iObj++;
	}

	return 0;
}

int player_item_do_skill(ZONESVRENV* pstEnv, Player *pstPlayer,  RESULT *pstResult, ZoneAni *pstTarAni)
{
	ZoneSkill stZoneSkill;
	SKILLDEF *pstSkillDef;
	ZoneObjList stList;
	CSSKILLATK stHit;
	MapIndex *pstMapIdx;
	ZONEIDX stIdx;

	if (pstTarAni == NULL)
	{
		return -1;
	}

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapIdx = z_map_inst_index(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapIdx)
	{
		return -1;
	}
	
	pstSkillDef = z_find_skill(pstEnv, pstResult->ResultVal1, pstResult->ResultVal2, NULL);
	if (NULL == pstSkillDef)
	{
		return -1;
	}

	bzero(&stHit, sizeof(stHit));
	
	stHit.SkillID = pstSkillDef->SkillID;
	stHit.RolePos = pstPlayer->stRoleData.Pos;
	stHit.SkillSeq = 0;
	//stHit.FittingsIdx = 0;
	
	if (pstTarAni->iType == OBJ_MONSTER)
	{
		stHit.AtkPos = pstTarAni->stObj.pstMon->stCurrPos;
		
	}
	else if (pstTarAni->iType == OBJ_PLAYER)
	{
		stHit.AtkPos = pstTarAni->stObj.pstPlayer->stRoleData.Pos;
	}
	else
	{
		return-1;
	}

	stList.iObj = 0;
	if (SKILL_RANGE_SIGLE == pstSkillDef->MainRange.RangeType || SKILL_RANGE_NONE == pstSkillDef->MainRange.RangeType)
	{	
		if (0 == z_player_filter_atkobj(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstTarAni, pstSkillDef,NULL))
		{
			stList.iObj = 1;
			stList.astObj[0] = *pstTarAni;
		}
	}
	else
	{
		z_get_skill_atklist(pstEnv, pstPlayer, pstSkillDef, pstTarAni, &stList, &stHit.AtkPos);
	}
	
	
	//填充一个zoneskill
	z_fill_zoneskill_by_def(pstEnv, pstPlayer, &stZoneSkill, pstSkillDef);
	
	z_player_hit_notify(pstEnv->pstAppCtx, pstEnv, pstPlayer, /*&stZoneSkill.stSkillData.CSFittingsRowList[0],*/ &stZoneSkill, pstSkillDef, pstSkillDef, &stList, &stHit, 0/*,0*/);

	return 0;
}

int player_use_recipe_item_check(ZONESVRENV* pstEnv, Player *pstPlayer,  int iSkillType, int iRecipeID)
{
	RECIPEDEF *pstRecipeDef;
	SECONDARYSKILLINFO *pstSkillInfo = get_player_secondary_skill_info(pstPlayer, iSkillType);

	if(NULL == pstSkillInfo)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK214);
		return -1;
	}

	pstRecipeDef = z_find_recipe_def(pstEnv, iSkillType, iRecipeID);
	if(NULL == pstRecipeDef)
	{
		return -1;
	}

	if(pstRecipeDef->LearnSkillLevel > pstSkillInfo->SkillLevel)
	{
		//z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "你还不可以学习%s", pstRecipeDef->NameDesc);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK215);
		return -1;
	}

	if(pstRecipeDef->RecipeID >= MAX_SECONDARY_SKILL_RECIPE*8)
	{
		//填错ID了
		return -1;
	}

	if (z_get_skill_recipe(pstSkillInfo, pstRecipeDef->RecipeID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK216, pstRecipeDef->NameDesc);
		return -1;
	}
				
	return 0;
}

int player_use_recipe_item(ZONESVRENV* pstEnv, Player *pstPlayer,  int iSkillType, int iRecipeID)
{
	RECIPEDEF *pstRecipeDef;
	SECONDARYSKILLINFO *pstSkillInfo = get_player_secondary_skill_info(pstPlayer, iSkillType);
	CSPKG stPkg;
	CSSECONDARYSKILLSVR *pstCSSecondarySkill = &stPkg.Body.SecondarySkillSvr;
	CSSECONDARYSKILLRECIPECHG *pstRecipeChg = &pstCSSecondarySkill->SvrData.RecipeChg;

	if(NULL == pstSkillInfo)
	{
		return -1;
	}

	pstRecipeDef = z_find_recipe_def(pstEnv, iSkillType, iRecipeID);
	if(NULL == pstRecipeDef)
	{
		return -1;
	}

	if(pstRecipeDef->RecipeID >= MAX_SECONDARY_SKILL_RECIPE*8)
	{
		return -1;
	}
	
	if (0 > z_set_skill_recipe(pstSkillInfo, pstRecipeDef->RecipeID, 1))
	{
		return -1;
	}

	pstCSSecondarySkill->SvrOp = SECONDARY_SKILL_SVR_RECIPE_CHG;
	pstRecipeChg->SkillType = pstRecipeDef->SkillType;
	pstRecipeChg->RecipeID = pstRecipeDef->RecipeID;
	pstRecipeChg->CoolDownTime = 0;
	
	Z_CSHEAD_INIT(&stPkg.Head, SECONDARY_SKILL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
					
	return 0;
}

int package_use_catch_pet_check(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneAni *pstTarAni)
{
	Monster *pstMon;
	MONSTERDEF *pstMonDef;
	ITEMDEF *pstPetItemDef;
	ROLEGRID stRoleGrid;
	
	if(NULL == pstTarAni || pstTarAni->iType != OBJ_MONSTER)
	{
		return -1;
	}
	
	pstMon = pstTarAni->stObj.pstMon;
	if(pstMon->cStat == MONSTER_DIE)
	{
		return -1;
	}

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (NULL == pstMonDef|| pstMonDef->Booty.BootyPetEgg <= 0)
	{
		return -1;
	}

	pstPetItemDef = z_find_itemdef(pstEnv, pstMonDef->Booty.BootyPetEgg);
	if(NULL == pstPetItemDef)
	{
		return -1;
	}

	if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstPetItemDef->ItemID))
	{
		return -1;
	}

	if(0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK217);
		return -1;
	}

	return 0;
}

int package_use_catch_pet(ZONESVRENV* pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef, RESULT *pstResult, ZoneAni *pstTarAni)
{
	Monster *pstMon;
	MONSTERDEF *pstMonDef;
	ITEMDEF *pstPetItemDef;
	ROLEGRID stRoleGrid;
	int iRate;
	CSPKG stPkgRes;
	CSACTIONRES *pstActionRes = &stPkgRes.Body.ActionRes;
	float fHpRate;
	PETDEF *pstPetDef;
	
	if(NULL == pstTarAni || pstTarAni->iType != OBJ_MONSTER)
	{
		return -1;
	}
	
	pstMon = pstTarAni->stObj.pstMon;
	if(pstMon->cStat == MONSTER_DIE)
	{
		return -1;
	}

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (NULL == pstMonDef|| pstMonDef->Booty.BootyPetEgg <= 0)
	{
		return -1;
	}

	pstPetItemDef = z_find_itemdef(pstEnv, pstMonDef->Booty.BootyPetEgg);
	if(NULL == pstPetItemDef)
	{
		return -1;
	}

	if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstPetItemDef->ItemID))
	{
		return -1;
	}

	pstPetDef = z_find_pet_def(pstEnv, pstPetItemDef->Result[0].ResultVal1, NULL);
	if (!pstPetDef)
	{
		return -1;
	}

	fHpRate = (100 - pstMon->iHP*100.0/pstMon->iMaxHP)/100.0;
	/*if (GET_MAPID(pstPlayer->stRoleData.Map) == LOST_MAP_ID_LAYER7)
	{
		if (fHpRate > 0.3f || pstItemDef->ItemID == 5233702 || pstItemDef->ItemID == 5233705)
		{
			iRate = 0;
		}	
		else
		{
			iRate = 	2-0.2*(1.0-fHpRate)/900;	
		}
		
	}
	else*/
//	{
		iRate = pstMonDef->Booty.BootyPetEggRate *fHpRate*(pstResult->ResultVal1/10000.0 - pstPetDef->FailRate/10000.0+1);
		//iRate += pstResult->ResultVal1;
//	}
	
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_CATCH_PET;
	pstActionRes->Data.CatchPet.TargetID= pstMon->iID;
	
	if(RAND1(10000) < iRate)
	{
		ZoneAni stAni;
		ITEMDEC stGetItem;
		
		package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0, IMPITEM_OPTYPE_PETCATCH);

		//成功
		pstActionRes->Data.CatchPet.Succ = 1;
		Z_CSHEAD_INIT(&stPkgRes.Head, ACTION_RES);
		z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkgRes, 0);

		stGetItem.ID = pstPetItemDef->ItemID;
		stGetItem.Num = 1;
		if(stRoleGrid.GridData.RoleItem.InstFlag & INST_ITEM_BIND)
		{
			stGetItem.BindNum = 1;
		}
		z_role_pet_catch_oplog(pstEnv, pstPlayer, pstItemDef->ItemID, pstMon->iDefIdx, &stGetItem);

		pstMon->cFirstAtkerType = 0;
		pstMon->ullFirstAtkerWid = 0;
		pstMon->iFirstAtkerID = 0; //没有战利品
		pstMon->szOwner[0] = 0;
		pstMon->cRefresh = 0;
		stAni.iType = OBJ_UNKNOW;
		
		z_mon_die(pstEnv->pstAppCtx, pstEnv, pstMon, pstMonDef, &stAni, 1, 1);	

		//成长之路
		if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
		{
			ACHIEVEMENTCOND stCond;

			memset(&stCond,0,sizeof(stCond));
			
			stCond.Type = COND_PET_CATCH;

			z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
		}
	}
	else
	{
		//成功
		pstActionRes->Data.CatchPet.Succ = 0;
		Z_CSHEAD_INIT(&stPkgRes.Head, ACTION_RES);
		z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkgRes, 0);
		
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK218, pstMonDef->Name);

		z_role_pet_catch_oplog(pstEnv, pstPlayer, pstItemDef->ItemID, pstMon->iDefIdx, NULL);
	}

	return 0;
}

int player_add_fashion_suit_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLEAVATARPROF *pstAvatar = &pstPlayer->stRoleData.AvatarProf;

	if(pstAvatar->AddFashion >= MAX_FASHION_SUIT -1)
	{
		return -1;
	}

	
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int player_add_clan_money_check(ZONESVRENV* pstEnv, Player *pstPlayer, int iValue)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);

	if (!pstZoneClan)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_PACKAGE_309);
		return -1;
	}

	if (clan_property_modify_test(pstEnv, pstZoneClan, CLAN_PROPERTY_MONEY, iValue, 0) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,ZONEERR_CLAN209);
		return -1;
	}
	return 0;
}

int player_add_clan_money(ZONESVRENV* pstEnv, Player *pstPlayer, int iValue)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);

	if (!pstZoneClan)
	{
		return -1;
	}

	clan_resource_inc(pstEnv, pstZoneClan, pstPlayer, CLAN_MONEY,iValue);
	return 0;
}

int player_use_add_fashion_suit(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLEAVATARPROF *pstAvatar = &pstPlayer->stRoleData.AvatarProf;

	if(pstAvatar->AddFashion >= MAX_FASHION_SUIT-1)
	{
		return -1;
	}

	pstAvatar->AddFashion++;
	z_update_avatar(pstEnv, pstPlayer);
	
	return 0;
}

static int item_result_add_machine_status_check(ZONESVRENV* pstEnv, Player *pstPlayer,RESULT* pstResult)
{
	Monster* pstMachine =NULL;
	if(!is_player_ride_on_machine(pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK219);
		return -1;
	}
	pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	if(!pstMachine)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK219);
		return -1;
	}
	return 0;
	UNUSED(pstResult);	//add by paraunused.pl
}

static int item_result_add_machine_status(ZONESVRENV* pstEnv, Player *pstPlayer,RESULT* pstResult)
{
	int iRand = 0;
	Monster* pstMachine =NULL;

	if (item_result_add_machine_status_check(pstEnv,pstPlayer,pstResult) < 0)
	{
		return -1;
	}

	pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	if(!pstMachine)
	{
		return -1;
	}
	
	iRand = RAND1(100);
	if (iRand >= pstResult->ResultVal1)
	{
		return -1;
	}

	if (z_mon_apply_status(pstEnv->pstAppCtx,pstEnv, pstMachine, 
						pstResult->ResultVal2, pstResult->ResultVal3, pstResult->ResultVal4, 1, 0) < 0)
	{
		return -1;
	}	
		
	return 0;
}



static int item_result_team_summon_check(ZONESVRENV* pstEnv, Player *pstPlayer,RESULT* pstResult)
{
	MapIndex *pstMapIdx;
	ZONEIDX stIdx;
	Player *pstTmpPlayer;
	ZoneTeam* pstTeam = player_team_get(pstEnv,pstPlayer);
	int i = 0;
	int iOnline = 0;
	//队伍检查
	if (!pstTeam)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_PAK220);
		return -1;
			
	}
	for(i = 0; i < pstTeam->stTeamInfo.MemberNum;i++)
	{
		pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);		
		if (!pstTmpPlayer ||pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}

		if(pstResult->ResultVal1 > 0 && 
			pstTmpPlayer->stOnline.stWarOL.cCampIdx != pstPlayer->stOnline.stWarOL.cCampIdx)
		{
			continue;
		}

		if(pstResult->ResultVal1 > 0 && 
			pstTmpPlayer->stRoleData.Map != pstPlayer->stRoleData.Map)
		{
			continue;
		}
		
		//if(0 == pstTeam->stTeamInfo.TeamMember[i].OffLineTime)
		{
			iOnline++;
		}
	}
	
	if (iOnline <= 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_PAK221);
		return -1;
	}

	if(pstResult->ResultVal1 > 0)
	{	
		ZONEPWORLD *pstPworld;
		ZONEIDX stIdx;
	
		stIdx.iID = GET_PWORLDID(pstPlayer->stRoleData.Map);
		stIdx.iPos = -1;
		
		pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);	
		if (NULL == pstPworld)
		{
			return -1;
		}
	
		if(pstPworld ->stPworldDef.BattleID != pstResult->ResultVal1)
		{
			return -1;
		}
	}
	else
	{
		//副本检查
		if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK222);
			return -1;
		}
	}

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapIdx = z_map_inst_index(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	if(pstMapIdx->stMapDef.CtrlFlag & MAP_CTRL_NO_USE_TEAM_SUMMON)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PACKAGE_310);
		return -1;
	}

	//坐牢,海滩,彩虹岛,勇士岛,中控中心不让用
	if (0 > player_use_trans_item_map_check(pstEnv,pstPlayer,1))
	{
		//中控中心 又允许去
		if (WORLD_CITY_MAPID != pstPlayer->stRoleData.Map)
		{
			return -1;
		}
	}
	return 0;
	UNUSED(pstResult);	//add by paraunused.pl
}

static int item_result_team_summon(ZONESVRENV* pstEnv, Player *pstPlayer,RESULT* pstResult)
{
	if (0 > item_result_team_summon_check(pstEnv,pstPlayer,pstResult))
	{
		return -1;
	}
	
	return player_team_summon(pstEnv,pstPlayer);
	UNUSED(pstResult);	//add by paraunused.pl
}

static int item_result_map_recorder_check(ZONESVRENV* pstEnv, Player *pstPlayer,RESULT* pstResult,ROLEITEM *pstRoleItem, ITEMDEF *pstItemDef,CSPACKAGEUSE *pstUse)
{
	short sIdx = (short)pstUse->SubIndex;
	
	if (!pstRoleItem || !pstItemDef || !pstUse)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK223);
		return -1;
	}
		
	if (pstUse->UseType != PACKAGE_USE_RECORD &&
		pstUse->UseType != PACKAGE_USE_TRANS)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK224);
		return -1;
	}
	
	if (sIdx >= pstResult->ResultVal1 ||
		sIdx >= MAX_SAVE_MAP_NUM ||
		sIdx < 0)
	{
		if (PACKAGE_USE_RECORD == pstUse->UseType)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK225);
			return -1;
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK226);
			return -1;
		}
	}
	
	if (PACKAGE_USE_RECORD == pstUse->UseType)
	{
		MapIndex *pstMapIdx;
		ZONEIDX stIdx;
		
		if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK222);
			return -1;
		}

		stIdx.iID = pstPlayer->stRoleData.Map;
		stIdx.iPos = pstPlayer->stOnline.iMapPos;
		pstMapIdx = z_map_inst_index(pstEnv->pstAppCtx, pstEnv, &stIdx);
		if (NULL == pstMapIdx)
		{
			return -1;
		}

		if(pstMapIdx->stMapDef.CtrlFlag & MAP_CTRL_NO_USE_DINGWEIFU)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PACKAGE_311);
			return -1;
		}

		//坐牢,海滩,彩虹岛,勇士岛,中控中心不让用
		if (0 > player_use_trans_item_map_check(pstEnv,pstPlayer,1))
		{
			return -1;
		}

		//状态检查
	
		if((pstPlayer->stOnline.State & CS_STAT_ATK)||
			(pstPlayer->stOnline.State & CS_STAT_MOVE_ATK) ||
			(pstPlayer->stOnline.State & CS_STAT_PREPARE) ||
			(pstPlayer->stOnline.State & CS_STAT_MOVE_PREPARE))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_PAK219);
			return -1;
		}	
	}
	else if (PACKAGE_USE_TRANS == pstUse->UseType)
	{
		ITEMMAPINFO* pstInfo = &pstRoleItem->ItemMapInfo[sIdx];
		RESPOS stPos;

		stPos.X = pstInfo->X;
		stPos.Y = pstInfo->Y;

		//状态检查
	
		if((pstPlayer->stOnline.State & CS_STAT_ATK)||
			(pstPlayer->stOnline.State & CS_STAT_MOVE_ATK) ||
			(pstPlayer->stOnline.State & CS_STAT_PREPARE) ||
			(pstPlayer->stOnline.State & CS_STAT_MOVE_PREPARE))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_PAK219);
			return -1;
		}	

		
			
		//传送检查
		if(0 > player_change_map_check(pstEnv, pstPlayer))
		{
			return -1;
		}

		if (0 > z_player_goto_check(pstEnv,pstPlayer, pstInfo->MapID, &stPos))
		{
			return -1;
		}

		if ( player_follow_enter_map_check(pstEnv, pstPlayer,pstInfo->MapID) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_PAK227);		
			return -1;
		}

	}
	
	return 0;
}

static int item_result_map_recorder(ZONESVRENV* pstEnv, Player *pstPlayer,RESULT* pstResult,ROLEITEM *pstRoleItem, ITEMDEF *pstItemDef,CSPACKAGEUSE *pstUse)
{
	short sIdx = (short)pstUse->SubIndex;
	

	if (0 > item_result_map_recorder_check(pstEnv, pstPlayer,pstResult,pstRoleItem,pstItemDef,pstUse))
	{
		return -1;
	}
	
	if (sIdx >= pstResult->ResultVal1 ||
		sIdx >= MAX_SAVE_MAP_NUM ||
		sIdx < 0)
	{
		if (PACKAGE_USE_RECORD == pstUse->UseType)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK225);
			return -1;
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK226);
			return -1;
		}
	}
	
	if (PACKAGE_USE_RECORD == pstUse->UseType)
	{

		pstRoleItem->ItemMapInfo[sIdx].MapID = pstPlayer->stRoleData.Map;
		pstRoleItem->ItemMapInfo[sIdx].X = pstPlayer->stRoleData.Pos.X;
		pstRoleItem->ItemMapInfo[sIdx].Y = pstPlayer->stRoleData.Pos.Y;
		pstRoleItem->ItemMapNum = max(sIdx + 1, pstRoleItem->ItemMapNum);

		//更新道具
		package_grid_update_to_clt(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstUse->SrcListType, pstRoleItem, NULL);
		
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK701);
	}
	else if (PACKAGE_USE_TRANS == pstUse->UseType)
	{
		ITEMMAPINFO* pstInfo = &pstRoleItem->ItemMapInfo[sIdx];
		RESPOS stPos;
		int iRet = 0;

		stPos.X = pstInfo->X;
		stPos.Y = pstInfo->Y;
		


		iRet = player_goto(pstEnv,pstPlayer, pstInfo->MapID,pstInfo->X,  pstInfo->Y,0);
		if (0 == iRet)
		{
			player_team_follow_cancel_svr(pstEnv, pstPlayer);
		}

		return iRet;
	}

	
	return 0;

	UNUSED(pstItemDef);	//add by paraunused.pl
}

static int item_result_trans_to_leader_check(ZONESVRENV* pstEnv, Player *pstPlayer,RESULT* pstResult)
{
	ZoneTeam* pstTeam = player_team_get(pstEnv,pstPlayer);
	Player* pstLeader = NULL;
	
	//队伍检查
	if (!pstTeam)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_PAK220);
		return -1;
			
	}

	pstLeader = player_team_get_captain_player(pstEnv,pstTeam);

	if (!pstLeader)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK228);
		return -1;
	}
	
	//副本检查
	if (pstLeader->stRoleData.Map >= PWORLD_MAPID_BASE)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK229);
		return -1;
	}

	if (pstLeader == pstPlayer)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK230);
		return -1;
	}

	//状态检查
	
	if((pstPlayer->stOnline.State & CS_STAT_ATK)||
		(pstPlayer->stOnline.State & CS_STAT_MOVE_ATK) ||
		(pstPlayer->stOnline.State & CS_STAT_PREPARE) ||
		(pstPlayer->stOnline.State & CS_STAT_MOVE_PREPARE))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_PAK231);
		return -1;
	}	
	
	if(0 > z_player_goto_check(pstEnv, pstPlayer, pstLeader->stRoleData.Map,&pstLeader->stRoleData.Pos))
	{
		return -1;
	}

	if ( player_follow_enter_map_check(pstEnv, pstPlayer,pstLeader->stRoleData.Map) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_PAK232);		
		return -1;
	}

	//队长坐牢,海滩,彩虹岛,勇士岛,中控中心不让用
	if (0 > player_use_trans_item_map_check(pstEnv,pstLeader,0))
	{

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_PAK233);
		return -1;
	}

	return 0;

	UNUSED(pstResult);	//add by paraunused.pl
}

static int item_result_trans_to_leader(ZONESVRENV* pstEnv, Player *pstPlayer,RESULT* pstResult)
{
	ZoneTeam* pstTeam = player_team_get(pstEnv,pstPlayer);	
	Player* pstLeader = NULL;
	
	//队伍检查
	if (!pstTeam)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_PAK220);
		return -1;
			
	}

	pstLeader = player_team_get_captain_player(pstEnv,pstTeam);

	if (!pstLeader)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK228);
		return -1;
	}
	
	//各种检查
	if(0 >item_result_trans_to_leader_check(pstEnv,pstPlayer,pstResult))
	{
		return -1;
	}

	player_goto_ignore_refer_line(pstEnv,pstPlayer, pstLeader->stRoleData.Map,
		pstLeader->stRoleData.Pos.X,  pstLeader->stRoleData.Pos.Y,
		pstLeader->stOnline.iMapPos);
	
	return 0;

	UNUSED(pstResult);	//add by paraunused.pl
}



int item_result(ZONESVRENV* pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef, 
	ZoneAni *pstTarAni, CSPACKAGEUSE *pstUse,ROLEITEM *pstRoleItem)
{
	tdr_longlong WID = 0;
	int i;
	int iTmp;
	int iTaskID;
	LEVELUP *pstLevelUp = NULL;
	PlayerChagAttrVal stAttr;
	ROLEPACKAGE *pstRolePackage = &pstPlayer->stRoleData.Package;
	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ZONEMAPINST *pstMapInst;
	ZONEIDX stIdx;

	if(pstUse != NULL )
	{
		WID =  pstUse->WID;
	}
	

	stAttr.AttrNum = 0;
	for (i=0; i<MAX_ITEM_RESULT; i++)
	{
		if (0 == pstItemDef->Result[i].ResultID)
		{
			break;
		}

		switch(pstItemDef->Result[i].ResultID)
		{
			case RESULT_ITEM_MT:
				{
					player_mt_gift_add( pstEnv, pstPlayer);
					z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_SHOP303);
				}
				break;
			case RESULT_PET_GENIUS_UP:
				pet_genius_up_high( pstEnv, pstPlayer, pstUse);
				break;
			case RESULT_PET_SILIAO:
				pkg_use_add_wenbao(pstEnv,pstPlayer,pstRoleItem,pstItemDef, pstUse);
				break;
			case RESULT_PET_QIANLI:
				pkg_use_add_impexp(pstEnv, pstPlayer, pstRoleItem,pstItemDef, pstUse);
				break;
			case RESULT_SELF_STATUS:
				if ((int)(RAND1(10000)) >= pstItemDef->Result[i].ResultVal1*100.0)
				{
					continue;
				}

				{
					int iPre = i - 1;
					//给目标加状态后给自己加
					if (iPre >= 0)
					{
						if(pstItemDef->Result[iPre].ResultID == RESULT_TARGET_STATUS && pstItemDef->Result[i].ResultVal1 == 100)
						{
							continue;
						}
					}
				}

				
				z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDef->Result[i].ResultVal2, 
										pstItemDef->Result[i].ResultVal3, pstItemDef->Result[i].ResultVal4, 1, 0);
										
				break;
			case RESULT_HP_ADD:
				iTmp = pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax * (pstItemDef->Result[i].ResultVal1/100.0) + pstItemDef->Result[i].ResultVal2;

				stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_HP;
				stAttr.AttrList[stAttr.AttrNum].nFlag = 0;
				stAttr.AttrList[stAttr.AttrNum].Value.HP = iTmp;
				stAttr.AttrNum++;
				
				break;
			case RESULT_MP_ADD:
				iTmp = pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax * (pstItemDef->Result[i].ResultVal1/100.0) + pstItemDef->Result[i].ResultVal2;

				stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_MP;
				stAttr.AttrList[stAttr.AttrNum].nFlag = 0;
				stAttr.AttrList[stAttr.AttrNum].Value.MP = iTmp;
				stAttr.AttrNum++;
				
				break;
			case RESULT_ADD_XP:
				// ResultVal1 最大XP 比率
				// ResultVal2 
				{
				int iFairyMaxXP = get_fairy_xp_upper(pstEnv, pstPlayer);

				if ((int)pstPlayer->stRoleData.RoleDetail.XP < iFairyMaxXP)
				{
					iTmp = iFairyMaxXP * (pstItemDef->Result[i].ResultVal1/100.0) + pstItemDef->Result[i].ResultVal2;
					//pstPlayer->stRoleData.MiscInfo.XpPrecise += iTmp * 100;
					stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_XP;
					stAttr.AttrList[stAttr.AttrNum].nFlag = 0;
					stAttr.AttrList[stAttr.AttrNum].Value.XP = iTmp;
					stAttr.AttrNum++;
				}
				

				// 副本内
				/*if (pstPlayer->stRoleData.Map > PWORLD_MAPID_BASE)
				{
					pstPlayer->stRoleData.MiscInfo.Flag |= PWORLD_ITEM_ADD_XP;
				}*/
				}				
				break;

			case RESULT_ITEM_TASK:
				iTaskID = pstItemDef->Result[i].ResultVal1;
				
				if (0 == task_accept(pstEnv, pstPlayer, iTaskID))
				{
					task_dialog(pstEnv, pstPlayer, 0, iTaskID, TASK_DIALOG_ACCEPT_TIP);
				}
				break;
			case RESULT_SKILL_BOOK:
				if(package_use_skill_book(pstEnv->pstAppCtx, pstEnv, pstPlayer, &pstItemDef->Result[i],pstItemDef->ItemID) < 0)
				{
					return -1;
				}
				break;
			case RESULT_RIDE:
				if ( package_use_ride(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDef) < 0)
				{
					return -1;
				}
				break;
			case RESULT_CALL_OWNER_MON:
				stIdx.iID = pstPlayer->stRoleData.Map;
				stIdx.iPos = pstPlayer->stOnline.iMapPos;
				pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
				if (NULL == pstMapInst)
				{
					return -1;
				}
				
				item_call_mon(pstEnv, pstPlayer, pstMapInst, pstItemDef->Result[i].ResultVal1,pstItemDef->Result[i].ResultVal2,
								pstItemDef->Result[i].ResultVal3,pstItemDef->Result[i].ResultVal4,0,0,0, 0,0);
				break;

			case RESULT_ADD_POWER:
				package_use_addpower(pstEnv, pstPlayer, &pstItemDef->Result[i], WID);
				break;
			case RESULT_ADD_EXP_PET:
				package_use_pet_addexp(pstEnv, pstPlayer, &pstItemDef->Result[i], WID);
				break;
			case RESULT_ADD_SKILL_GRID:
				//package_use_add_skill_grid(pstEnv, pstPlayer, &pstItemDef->Result[i], WID);
				break;
			case RESULT_SHI_LIST:
				pstPlayer->stRoleData.MiscInfo.AddPetGrid++;
				package_enlarge(pstEnv->pstAppCtx, pstEnv, pstPlayer, LIST_TYPE_SHI, pstItemDef->Result[i].ResultVal1);
				break;
			case RESULT_PET_ADD_ATTR:
				//package_use_pet_add_attr(pstEnv, pstPlayer, &pstItemDef->Result[i], WID);
				break;
			case RESULT_PET_ADD_LOVEVAL:
				package_use_pet_add_loveval(pstEnv, pstPlayer, &pstItemDef->Result[i], WID);
				break;
			case RESULT_ROLE_ADD_SKILLPOINT:
				package_use_add_skill_point(pstEnv->pstAppCtx, pstEnv, pstPlayer, &pstItemDef->Result[i]);
				break;
			/*
			case RESULT_FITTINGS_INLAY: 
				if(pstUse)
				{
						package_fittings_inlay(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstUse, &pstItemDef->Result[i]);
				}
				break;
			*/
				
			case RESULT_COOL_CLEAN:
				player_skill_cool_clean(pstEnv, pstPlayer, pstItemDef->Result[i].ResultVal2);
				break;

			case RESULT_PET_HATCH:
				if (0 > package_pet_use_hatch(pstEnv, pstPlayer, &pstItemDef->Result[i]))
				{
					return -1;
				}
				break;
			case RESULT_PLAYER_ADD_EXP:
				{
					tdr_ulonglong ullExp;
					pstLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
					if (!pstLevelUp)
					{
						return -1;
					}

					ullExp = (tdr_ulonglong)pstLevelUp->ContributeExp * pstItemDef->Result[i].ResultVal1/10000.0;
					pworld_player_add_exp( pstEnv, pstPlayer, ullExp);

					/*
					stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_EXP;
					stAttr.AttrList[stAttr.AttrNum].nFlag = 0;
					stAttr.AttrList[stAttr.AttrNum].Value.Exp = (tdr_ulonglong)pstLevelUp->ContributeExp * pstItemDef->Result[i].ResultVal1/10000.0;
					stAttr.AttrNum++;
					*/
				}

				break;
			case RESULT_PLAYER_ADD_EXP_NOLIMIT:
				{
					tdr_ulonglong ullExp;
					pstLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
					if (!pstLevelUp)
					{
						return -1;
					}

					ullExp = (tdr_ulonglong)pstLevelUp->ContributeExp * pstItemDef->Result[i].ResultVal1/10000.0;

					pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_EXPNODAYLIMIT;
					player_add_exp( pstEnv, pstPlayer, ullExp, 0);
					pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_EXPNODAYLIMIT;
				}
				break;
			case RESULT_DIATHESIS_UP:
				if (player_diathesis_up(pstEnv, pstPlayer,pstItemDef->Result[i].ResultVal1, pstItemDef->Result[i].ResultVal2) < 0)
				{
					return -1;
				}
				break;
			case RESULT_ADD_STUFF_PACKAGE:
				if (pstRolePackage->StuffPackage.MaxNum + pstItemDef->Result[i].ResultVal1 >= MAX_STUFF_PACKAGE)
				{
					pstRolePackage->StuffPackage.MaxNum = MAX_STUFF_PACKAGE;
				}
				else
				{
					pstRolePackage->StuffPackage.MaxNum += pstItemDef->Result[i].ResultVal1;
				}
				
				pstPackageInfo->Type = PACKAGE_STUFF_NUM_CHG;
				pstPackageInfo->PackageInfoData.StuffNum = pstRolePackage->StuffPackage.MaxNum;
				Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
				z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
				break;
			case RESULT_ADD_SUNDRIES_PACKAGE:
				if (pstRolePackage->SundriesPackage.MaxNum + pstItemDef->Result[i].ResultVal1  >= MAX_SUNDRIES_PACKAGE)
				{
					pstRolePackage->SundriesPackage.MaxNum = MAX_SUNDRIES_PACKAGE;
				}
				else
				{
					pstRolePackage->SundriesPackage.MaxNum += pstItemDef->Result[i].ResultVal1;
				}
				
				pstPackageInfo->Type = PACKAGE_SUNDRIES_NUM_CHG;
				pstPackageInfo->PackageInfoData.SundriesNum = pstRolePackage->SundriesPackage.MaxNum;
				Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
				z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
				break;
			case RESULT_ADD_TASK_PACKAGE:
				if (pstRolePackage->TaskPackage.MaxNum +  pstItemDef->Result[i].ResultVal1 >= MAX_TASK_PACKAGE)
				{
					pstRolePackage->TaskPackage.MaxNum = MAX_TASK_PACKAGE;
				}
				else
				{
					pstRolePackage->TaskPackage.MaxNum += pstItemDef->Result[i].ResultVal1;
				}
				
				pstPackageInfo->Type = PACKAGE_TASK_NUM_CHG;
				pstPackageInfo->PackageInfoData.TaskNum = pstRolePackage->TaskPackage.MaxNum;
				Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
				z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
				break;
			case RESULT_DO_SKILL:
				if (0 > player_item_do_skill(pstEnv, pstPlayer, &pstItemDef->Result[i], pstTarAni))
				{
					return -1;
				}
				break;
			case RESULT_BACK_HOME:
				{
					RESPOS stPos;
					int iMap;
					
					iMap 	= pstItemDef->Result[i].ResultVal1;
					stPos.X 	= pstItemDef->Result[i].ResultVal2;
					stPos.Y 	= pstItemDef->Result[i].ResultVal3;

					player_home(pstEnv, pstPlayer, iMap, &stPos);
				}
				break;

			case RESULT_CALL_NPC:
				z_player_call_npc(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDef->Result[i].ResultVal2, 
						pstItemDef->Result[i].ResultVal1);
				break;

			case RESULT_ADD_DESIGNATION: // 给角色增加一个称号
				if(player_designation_add(pstEnv, pstPlayer,
					                     pstItemDef->Result[i].ResultVal2) < 0)
				{
					return -1;
				}
				break;
			case RESULT_ADD_TMPENERGE:
				if(0 > player_item_do_tmpsys(pstEnv,pstPlayer, &pstItemDef->Result[i]) )
				{
					return -1;
				}
				break;
			case RESULT_ADD_RESPACT: // 道具增加声望
				if(player_respact_inc(pstEnv, pstPlayer,
				                      pstItemDef->Result[i].ResultVal2, 
				                      pstItemDef->Result[i].ResultVal3, 0) < 0)
				{
					return -1;
				}
				break;
			case RESULT_QIYUE:
				if(0 > player_item_do_qiyue(pstEnv,pstPlayer,&pstItemDef->Result[i]) )
				{
					return -1;
				}
				break;
			case RESULT_FAIRY_BORN:
				if (fairy_born(pstEnv, pstPlayer,pstItemDef->Result[i].ResultVal1) == NULL)
				{
					return -1;
				}
				break;
			case RESULT_ADD_VIM:
				iTmp = pstPlayer->stOnline.stDyn.stMidAttrList.uiVimMax * (pstItemDef->Result[i].ResultVal1/100.0) + pstItemDef->Result[i].ResultVal2;

				stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_VIM;
				stAttr.AttrList[stAttr.AttrNum].nFlag = 0;
				stAttr.AttrList[stAttr.AttrNum].Value.Vim = iTmp;
				stAttr.AttrNum++;
				break;
			case RESULT_ADD_RIDE_GRID:
				return package_use_add_ride_grid(pstEnv, pstPlayer, &pstItemDef->Result[i]);
				break;

			// resultval1表示减少的炼化时间  单位秒
			case RESULT_DEC_FAIRY_TM:
				//return fairy_evolve_use_item(pstEnv, pstPlayer, pstItemDef->Result[0].ResultVal1);
				break;
			case RESULT_SECONDARY_SKILL_RECIPE:
				return player_use_recipe_item(pstEnv, pstPlayer, pstItemDef->Result[i].ResultVal1, pstItemDef->Result[i].ResultVal2);
				break;
			case RESULT_ETPKG_HOLE:
				return pkg_add_hole(pstEnv, pstPlayer, &pstItemDef->Result[i]);
				break;
			case RESULT_ADD_TMP_ITEM:
				{
					int iApplyRet = tmp_apply_item(pstEnv,pstPlayer, pstItemDef->Result[i].ResultVal2,pstItemDef->Result[i].ResultVal1);
					if (0 == iApplyRet )
					{
						pstPlayer->stOnline.stMachineInfo.ullLockItemWID = WID;
					}
					return iApplyRet;
				}
				break;
			case RESULT_CATCH_PET:
				return package_use_catch_pet(pstEnv, pstPlayer, pstItemDef, &pstItemDef->Result[i], pstTarAni);
				break;
			case RESULT_MACHINE_ITEM:
				if(pstUse)
				{
					return package_use_machine_item(pstEnv, pstPlayer, &pstItemDef->Result[i], pstUse);
				}
				break;
			case RESULT_FACTORY_ITEM:
				if(pstUse)
				{
					return package_use_factory_item(pstEnv, pstPlayer, &pstItemDef->Result[i], pstUse->WID);
				}
				break;
			case RESULT_ADD_MACHINE_ENERGY:
				return package_use_machine_energy(pstEnv, pstPlayer, &pstItemDef->Result[i]);
				break;
			case RESULT_MACHINE_HP:
				return package_use_machine_hp(pstEnv, pstPlayer, &pstItemDef->Result[i]);
				break;
			case RESULT_MACHINE_STATUS:
				return package_use_machine_status(pstEnv, pstPlayer, &pstItemDef->Result[i]);
				break;

			case RESULT_TARGET_STATUS:				
				if (pstTarAni->iType == OBJ_PLAYER && pstTarAni->stObj.pstPlayer)
				{
					if ((int)(RAND1(10000)) >= pstItemDef->Result[i].ResultVal1*100.0)
					{
						continue;
					}
									
					z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstTarAni->stObj.pstPlayer, pstItemDef->Result[i].ResultVal2, 
											pstItemDef->Result[i].ResultVal3, pstItemDef->Result[i].ResultVal4, 1, pstPlayer->iMemID);
					{
						int iNext = i + 1;
						//给目标加状态后给自己加
						if (iNext < MAX_ITEM_RESULT)
						{
							if(pstItemDef->Result[iNext].ResultID == RESULT_SELF_STATUS && pstItemDef->Result[iNext].ResultVal1 == 100)
							{
								z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDef->Result[iNext].ResultVal2, 
											pstItemDef->Result[iNext].ResultVal3, pstItemDef->Result[iNext].ResultVal4, 1, 0);
							}
						}
					}
				}
				break;
				
			case RESULT_DROP_TMP_ITEM:
				if (pstTarAni->iType == OBJ_PLAYER && pstTarAni->stObj.pstPlayer)
				{
					if ((int)(RAND1(10000)) >= pstItemDef->Result[i].ResultVal2*100.0)
					{
						continue;
					}
					tmp_item_drop(pstEnv,pstTarAni->stObj.pstPlayer,1);
					
				}
				break;
				

			case RESULT_ITEM_SWITCH:

				break;

			case RESULT_ITEM_DEC_PKVAL:
				return player_pk_val_chg(pstEnv,pstPlayer, -pstItemDef->Result[i].ResultVal1);
				break;
			case RESULT_BACK_TO_CLAN_CITY:
				{
					int iMap;
					RESPOS stPost;
					
					if(get_pkval_type(pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal) == PK_VAL_TYPE_BLACK)
					{
						stPost.X = BLACK_NAME_REVIVE_POS_X;
						stPost.Y = BLACK_NAME_REVIVE_POS_Y;	
						iMap = BLACK_NAME_REVIVE_MAP;
						g_iDesigVar = 2;
						z_player_goto(pstEnv, pstPlayer, iMap, &stPost);;
						g_iDesigVar = 0;
					}
					else
					{
						stPost.X = pstItemDef->Result[i].ResultVal2;
						stPost.Y = pstItemDef->Result[i].ResultVal3;
						if(0 > clan_mem_enter_city(pstEnv,pstPlayer,&stPost))
						{
							return -1;
						}
					}
				}
				break;
			case RESULT_MEDIA_GIFT:
				if(!pstUse || 0 > player_mediagift_use(pstEnv,pstPlayer,pstItemDef->Result[i].ResultVal1,pstUse->SubIndex,pstRoleItem,pstItemDef->Name))
				{
					return -1;
				}
				break;
			case RESULT_LTR_GIFT:
				if(0 > player_ltrgift_use(pstEnv,pstPlayer,pstItemDef->Result[i].ResultVal1, pstRoleItem, pstItemDef->Name))
				{
					return -1;
				}
				break;

			case RESULT_VIGOR:
				if(0 > player_add_vig(pstEnv,pstPlayer,pstItemDef->Result[i].ResultVal2,0,1))
				{
					return -1;
				}
				break;
			case RESULT_TEAM_SUMMON:
				if(0 >item_result_team_summon(pstEnv,pstPlayer,&pstItemDef->Result[i]))
				{
					return -1;
				}
				break;	

			case RESULT_TEAM_RATE_OPEN_PKG:
			case RESULT_TEAM_RATE_PKG:
				{
					int iListType;
					
					if (pstItemDef->Result[i].ResultVal1 == 1)
					{
						pstPlayer->stRoleData.Package.EtSundPkg.MaxNum ++;
						iListType = LIST_TYPE_ET_SUND;
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK234);
					}
					else
					{
						pstPlayer->stRoleData.Package.EtStashPkg.MaxNum ++;
						iListType = LIST_TYPE_ET_STASH;
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK235);
					}	

					role_pkg_num_chg(pstEnv, pstPlayer, iListType);
				
				}
				break;	
				
			case RESULT_MAP_RECORDER:
				if (!pstRoleItem || !pstItemDef || !pstUse)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK223);
					return -1;
				}
				else
				{
					if(0 > item_result_map_recorder(pstEnv,pstPlayer,
						&pstItemDef->Result[i],pstRoleItem,pstItemDef,pstUse))
					{
						return -1;
					}
				}
				break;
				
			case RESULT_TRANS_TO_LEADER:
				if(0 > item_result_trans_to_leader(pstEnv,pstPlayer,
					&pstItemDef->Result[i]))
				{
					return -1;
				}
				break;
			case RESULT_SHAM_DEAD:
				return player_sham_dead(pstEnv, pstPlayer);
				break;
			case RESULT_COLOR_FONT_TIME:
				return package_use_color_font_limit(pstEnv, pstPlayer, &pstItemDef->Result[i]);
				break;
			case RESULT_MOTION_TIME:
				return package_use_motion_limit(pstEnv, pstPlayer, &pstItemDef->Result[i]);
				break;
			case RESULT_ADD_MACHINE_STATUS:
				if(0 > item_result_add_machine_status(pstEnv,pstPlayer,&pstItemDef->Result[i]))
				{
					return -1;
				}
				break;
			case RESULT_USE_OFFLINE_EXP:
				player_use_all_offline_exp(pstEnv, pstPlayer, pstItemDef->Result[i].ResultVal1);
				break;
			case RESULT_USE_PWORLD_OFFLINE_EXP:
				 player_use_all_pworld_offexp(pstEnv, pstPlayer, pstItemDef->Result[i].ResultVal1);
				break;
			case RESULT_CLAN_SUMMON:
				if(0 > player_clan_summon(pstEnv, pstPlayer, &pstItemDef->Result[i]))
				{
					return -1;
				}
				break;
			case RESULT_VIP:
				star_item_result(pstEnv, pstPlayer, &pstItemDef->Result[i]);
				break;
			case RESULT_ADD_FASHION_SUIT:
				if(0 > player_use_add_fashion_suit(pstEnv, pstPlayer))
				{
					return -1;
				}
				break;
			case RESULT_SPARTA_RUNAWAY_CLEAR:
				player_sparta_runaway_clear(pstEnv, pstPlayer);
				break;
			case RESULT_SPARTA_ALL_CLEAR:
				player_sparta_all_clear(pstEnv, pstPlayer);
				break;
			case RESULT_ADD_CLAN_MONEY:
				player_add_clan_money(pstEnv,pstPlayer, pstItemDef->Result[i].ResultVal2);
				break;
			case RESULT_MAP_RAND_TRANSMIT:
				if(0 > map_rand_transmit(pstEnv,pstPlayer))
			    {
					return -1;
				}
				break;
			case RESULT_CHARM:
				if (pstTarAni->iType == OBJ_PLAYER && pstTarAni->stObj.pstPlayer)
				{
					int iFlag = 0;
					if (pstItemDef->Result[i].ResultVal1 < 0)
					{
						iFlag = 1;
					}
					player_charm_chg( pstEnv, pstTarAni->stObj.pstPlayer, iFlag, abs(pstItemDef->Result[i].ResultVal1));
				}
				
				break;
			case RESULT_LOST_TOTEM:
				if (losttotem_use(pstEnv, pstPlayer, pstRoleItem, pstItemDef) < 0)
				{
					return -1;
				}
				break;
			case RESULT_ITEM_EXP_BUCHANG:
				{
					int iGold = ceil(pstRoleItem->InstVal1/1000000.0);
					int iExp = pstRoleItem->InstVal1;
					if (pstUse->SubIndex > 0)
					{
						iExp = pstRoleItem->InstVal1*2;
						if (iGold <= 0)
							iGold = 1;
						package_dec_money(pstEnv,pstPlayer, ATTR_ID_GODCOIN,iGold);
					}	

					pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_EXPNODAYLIMIT;
					player_add_exp(pstEnv, pstPlayer, iExp, 0);
					pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_EXPNODAYLIMIT;
				}
				
				break;
			case RESULT_ITEM_ARM_BUCHANG:
				item_use_arm_buchang(pstEnv,pstPlayer,pstRoleItem, pstPlayer->stRoleData.Level/10,pstUse);
				break;
			case RESULT_ITEM_ARM_SELECT:
				item_use_arm_buchang(pstEnv,pstPlayer,pstRoleItem, pstItemDef->Result[i].ResultVal1,pstUse);
				break;
			case RESULT_ITEM_CLAN_CONTRI:
				package_use_clan_contri(pstEnv, pstPlayer, pstRoleItem);
				break;
			case RESULT_ZHENGYI_ADD:
				package_add_money(pstEnv, pstPlayer, ATTR_ID_ZHENGYI, pstItemDef->Result[i].ResultVal1);
				break;
			case RESULT_ZHENGYI_LIMIT_ADD:
				player_add_zhengyi_limit(pstEnv, pstPlayer, pstItemDef->Result[i].ResultVal1);
				break;
			case RESULT_BUDDY_VAL:				
				if (pstTarAni->iType == OBJ_PLAYER && pstTarAni->stObj.pstPlayer)
				{
					ROLEBUDDYMEMBER *pstBuddyMember = NULL;
					ROLEBUDDYMEMBER *pstBuddyMember2 = NULL;
					int ret = 0;
					int ret2 = 0;
					short iResultVal = 0;
					int iTemp1 = 0;
					int iTemp2 = 0;
					unsigned short usOldBuddyVal = 0;
					unsigned short usOldBuddyVal2 = 0;
					char szText[512];
					char szText1[512];

					if (package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDef->ItemID, 1, -1, NULL,PACKAGE_DEC_NONE) < 0)
					{
						return -1;
					}
					
					ret = online_info_find_friend_byID(&pstPlayer->stRoleData.Buddy, pstTarAni->stObj.pstPlayer->stRoleData.RoleID);
					if(ret < 0)
					{
						return -1;
					}
					
					ret2 = online_info_find_friend_byID(&pstTarAni->stObj.pstPlayer->stRoleData.Buddy, pstPlayer->stRoleData.RoleID);
					if(ret2 < 0)
					{
						return -1;
					}

					if (0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDef->ItemID, 1, -1, NULL, 
							PACKAGE_DEC_NONE, IMPITEM_OPTYPE_WEDDING))
					{
						return -1;
					}
					
					pstBuddyMember = &pstTarAni->stObj.pstPlayer->stRoleData.Buddy.Friends[ret2];
					pstBuddyMember2 = &pstPlayer->stRoleData.Buddy.Friends[ret];

					usOldBuddyVal = pstBuddyMember->BuddyVal;
					usOldBuddyVal2 = pstBuddyMember2->BuddyVal;
					
					iResultVal = pstItemDef->Result[i].ResultVal1;

					//改变双方亲密度值
					iTemp1 = relation_buddyval_change(pstEnv, pstPlayer, pstBuddyMember2,iResultVal);
					iTemp2 = relation_buddyval_change(pstEnv, pstTarAni->stObj.pstPlayer, pstBuddyMember,iResultVal);					

					//双方播特效
					snprintf(szText, sizeof(szText),"playEffct(%d,%d,\"effect\\\\entity\\\\10543.ent\")",pstPlayer->stRoleData.Pos.X,pstPlayer->stRoleData.Pos.Y);
					play_effect(pstEnv, pstPlayer, 1, pstPlayer->iMemID, szText, NULL);
					snprintf(szText1, sizeof(szText1),"playEffct(%d,%d,\"effect\\\\entity\\\\10543.ent\")",pstTarAni->stObj.pstPlayer->stRoleData.Pos.X,pstTarAni->stObj.pstPlayer->stRoleData.Pos.Y);
					play_effect(pstEnv, pstTarAni->stObj.pstPlayer, 1, pstTarAni->stObj.pstPlayer->iMemID, szText1, NULL);
			
					if(iResultVal >= 0)
					{
						if(iTemp1 == 0)
						{
							z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_CHAT, ZONEERR_WEDDING58,
								pstTarAni->stObj.pstPlayer->stRoleData.RoleName,
								pstBuddyMember2->BuddyVal-usOldBuddyVal2);
						}

						if(iTemp2 == 0)
						{
							z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTarAni->stObj.pstPlayer, SYS_CHAT, 
								ZONEERR_WEDDING58,pstPlayer->stRoleData.RoleName,
								pstBuddyMember->BuddyVal-usOldBuddyVal);
						}
					}
					else
					{
						if(iTemp1 == 0)
						{
							z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_CHAT, 
								ZONEERR_WEDDING59,pstTarAni->stObj.pstPlayer->stRoleData.RoleName,
								usOldBuddyVal2-pstBuddyMember2->BuddyVal);
						}

						if(iTemp2 == 0)
						{
							z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTarAni->stObj.pstPlayer, 
								SYS_CHAT, ZONEERR_WEDDING59,pstPlayer->stRoleData.RoleName,
								usOldBuddyVal-pstBuddyMember->BuddyVal);
						}
					}
				}
				break;
			case RESULT_ITEM_GEM_PAG:
					item_use_gem_pag(pstEnv,pstPlayer, pstUse,pstItemDef->Result[i].ResultVal1);
					break;
			default:
				break;
		}
	}

	if (stAttr.AttrNum > 0)
	{
		z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stAttr, 1);
	}

	return 0;
}

int player_item_do_tmpsys(ZONESVRENV* pstEnv, Player *pstPlayer,  RESULT *pstResult)
{
	ROLETMPSYS *pstRoleTmpSys = NULL;
	ITEMDEF *pstItemDef = NULL;
	//int iEnerge = 0;
	//ZoneSkill *pstZoneSkill = NULL;
	int iSkillCollectNum = 0;
	PlayerChagAttrVal stChgVal;
	int iRet = -1;

	memset(&stChgVal,0,sizeof(stChgVal));
	pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;

	/* 没有装备临时道具 */
	if (0 == pstRoleTmpSys->TmpItemID)
	{
		return -1;
	}
	/* 获取当前装备的临时道具 */
	pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
	if (NULL == pstItemDef || 
		ITEM_TMP_MACHINE != pstItemDef->ItemTmp.Type)
	{
		return -1;
	}

	iSkillCollectNum = pstResult->ResultVal1;
	if((int)pstRoleTmpSys->TmpEnerge < pstItemDef->ItemTmp.MachineEnerge)
	{
		pstRoleTmpSys->TmpEnerge += iSkillCollectNum;
		if ((int)pstRoleTmpSys->TmpEnerge > pstItemDef->ItemTmp.MachineEnerge)
		{
			pstRoleTmpSys->TmpEnerge = pstItemDef->ItemTmp.MachineEnerge;
		}
		iRet = 0;
		stChgVal.AttrList[stChgVal.AttrNum].AttrID = ATTR_ID_TMPENERGE;
		if(pstRoleTmpSys->TmpEnerge>0)
		{
			stChgVal.AttrList[stChgVal.AttrNum].Value.Energe = pstRoleTmpSys->TmpEnerge;
		}
		else
		{
			stChgVal.AttrList[stChgVal.AttrNum].Value.Energe = 0;
		}
		stChgVal.AttrNum++;
			
	}
	else 
	{
		return -1;
	}

	/*
	if( pstItemDef->ItemTmp.SkillID_1 > 0 )
	{
		pstZoneSkill =  z_get_player_zoneskill(pstPlayer, pstItemDef->ItemTmp.SkillID_1);
		if( pstZoneSkill != NULL )
		{
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
	if (pstItemDef->ItemTmp.SkillID_2 > 0)
	{
		pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstItemDef->ItemTmp.SkillID_2);
		if (NULL != pstZoneSkill)
		{
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
	}*/
	
	if (stChgVal.AttrNum > 0)
	{
		z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);
	}

	return iRet;
}
/* 使用道具对临时系统的核实*/
int player_item_do_tmpsys_check(ZONESVRENV* pstEnv, Player *pstPlayer,  RESULT *pstResult)
{
	ROLETMPSYS *pstRoleTmpSys = NULL;
	ITEMDEF *pstItemDef = NULL;

	pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;

	if( pstResult->ResultID !=  RESULT_ADD_TMPENERGE ||
		 pstResult->ResultVal1 <=0 )
	{
		return -1;
	}
	/* 没有装备临时道具 */
	if (0 == pstRoleTmpSys->TmpItemID )
	{
		return -1;
	}
	/* 获取当前装备的临时道具 */
	pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
	if (NULL == pstItemDef || 
		ITEM_TMP_MACHINE != pstItemDef->ItemTmp.Type)
	{
		return -1;
	}

	if((int)pstRoleTmpSys->TmpEnerge >= pstItemDef->ItemTmp.MachineEnerge )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK131);
		return  -1;
	}
	
	return 0;
}
int player_item_do_qiyue_check(ZONESVRENV* pstEnv, Player *pstPlayer,  RESULT *pstResult)
{
	QIYUEINFO *pstQiYueInfo = NULL;
	int iIndex = 0;
	int iEqual = 0;
	
	pstQiYueInfo = &pstPlayer->stRoleData.Package.QiYueInfo;
	
	if( pstResult->ResultID != RESULT_QIYUE )
	{
		return -1;
	}
	
	iIndex = bsearch_int(&pstResult->ResultVal1, pstQiYueInfo->QiYueList, pstQiYueInfo->QiYueNum, sizeof(int), &iEqual);
	if( iEqual )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK132);
		return -1;
	}

	if( pstQiYueInfo->QiYueNum >= QIYUE_MAX_NUM )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK133);
		return -1;
	}

	return 0;
}

int player_item_do_qiyue(ZONESVRENV* pstEnv, Player *pstPlayer,  RESULT *pstResult)
{
	QIYUEINFO *pstQiYueInfo = NULL;
	
	pstQiYueInfo = &pstPlayer->stRoleData.Package.QiYueInfo;
	
	if( 0 > player_item_do_qiyue_check(pstEnv,pstPlayer,pstResult) )
	{
		return -1;
	}
		
	if ( !binsert_int(&pstResult->ResultVal1,pstQiYueInfo->QiYueList, (int *)&pstQiYueInfo->QiYueNum, sizeof(int), 1) )
	{
		 return -1;
	}

	/* 插入成功 返回列表*/
	player_team_send_user_qiyuelist(pstEnv,pstPlayer);

	/* 上报下日志 */
	z_role_qiyue_oplog(pstEnv,pstPlayer,pstResult->ResultVal1,1);
	return 0;
}


static int pkg_check_add_hole(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult)
{
//	EXTENDSTASHPKG *pstEtStashPkg = &pstPlayer->stRoleData.Package.EtStashPkg;                     
    	EXTENDSUNDPKG *pstEtSundPkg = &pstPlayer->stRoleData.Package.EtSundPkg;

	// 开杂物包的孔
	if (pstResult->ResultVal1 == 1)
	{
		if (pstEtSundPkg->MaxNum >= MAX_EXTEND_SUND_PKG )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK134);
			return -1;
		}
	}
	// 开材料包的孔
	else if(pstResult->ResultVal1 == 2)
	{
		//if (pstEtSundPkg->StuffNum >= 1)
		//{
			//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "扩充材料栏达到上限");
			return -1;
		//}
	}
	// 仓库
	else if (pstResult->ResultVal1 == 3)
	{
		//if (pstEtStashPkg->MaxNum >= MAX_ET_STASH)
		//{
			//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "扩充银行达到上限");
			return -1;
		//}
	}
	else
	{
		return -1;
	}
	return 0;
}

int pkg_move_etsund(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLEPACKAGE *pstPkg = &pstPlayer->stRoleData.Package;
	int i;
	ROLEITEM *pstRoleItem;
	RESULT *pstResult = NULL;
	ITEMDEF *pstItemDef;
	int iOldGid;

	if (pstPkg->EtSundPkg.StuffNum <= 0)
		return 0;
	
	for (i=0; i<pstPkg->EtSundPkg.Num; i++)
	{
		CSPKG stPkg;
		CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
		
		pstRoleItem = &pstPkg->EtSundPkg.RoleItems[i];
		pstItemDef = z_find_itemdef(pstEnv,pstRoleItem->DefID);
		if (!pstItemDef)
			continue;
		pstResult = z_item_get_result(pstItemDef, RESULT_ADD_STUFFPKG_GRID);
		if (!pstResult)
			continue;

		iOldGid = pstRoleItem->GridIdx;
		pstRoleItem->GridIdx = pstPkg->EtSundPkg.MaxNum -1;
		pstPackageInfo->Type = PACKAGE_INFO_CHG;
		pstPackageInfo->PackageInfoData.PackageChg.ItemChg.ChgNum = 2;
		pstPackageInfo->PackageInfoData.PackageChg.ItemChg.Items[0].ListType = LIST_TYPE_ET_SUND;
		pstPackageInfo->PackageInfoData.PackageChg.ItemChg.Items[0].Item = *pstRoleItem;
		pstPackageInfo->PackageInfoData.PackageChg.ItemChg.Items[1].ListType = LIST_TYPE_ET_SUND;
		pstPackageInfo->PackageInfoData.PackageChg.ItemChg.Items[1].Item = *pstRoleItem;
		pstPackageInfo->PackageInfoData.PackageChg.ItemChg.Items[1].Item.Num = 0;
		pstPackageInfo->PackageInfoData.PackageChg.ItemChg.Items[1].Item.GridIdx = iOldGid;
		Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	return 0;
}

static int pkg_add_hole(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult)
{
//	EXTENDSTASHPKG *pstEtStashPkg = &pstPlayer->stRoleData.Package.EtStashPkg;                     
    	EXTENDSUNDPKG *pstEtSundPkg = &pstPlayer->stRoleData.Package.EtSundPkg;
	int iListType;

	// 开杂物包的孔
	if (pstResult->ResultVal1 == 1)
	{
		pstEtSundPkg->MaxNum ++;
		iListType = LIST_TYPE_ET_SUND;
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK234);
		
	}
	// 开材料包的孔
	/*else if(pstResult->ResultVal1 == 2)
	{
		pstEtSundPkg->StuffNum ++;
		pstEtSundPkg->MaxNum ++;
		iListType = LIST_TYPE_ET_SUND;

		snprintf(szMsg, sizeof(szMsg), "材料包扩充栏位增加");
	
	}
	// 仓库
	else if (pstResult->ResultVal1 == 3)
	{
		pstEtStashPkg->MaxNum ++;
		iListType = LIST_TYPE_ET_STASH;

		snprintf(szMsg, sizeof(szMsg), "银行扩充栏位增加");
	}*/
	else
	{
		return -1;
	}

	role_pkg_num_chg(pstEnv, pstPlayer, iListType);

	if (pstResult->ResultVal1 == 1)
	{
		//pkg_move_etsund(pstEnv, pstPlayer);
	}

	return 0;
}


int pkg_use_check_rate_open_et(ZONESVRENV* pstEnv, Player *pstPlayer,RESULT *pstResult,ROLEITEM *pstItem)
{
	ROLEPACKAGE *pstPkg = &pstPlayer->stRoleData.Package;
	int iIdx;
	int iRate;
	int iRate1;
	int iPos;
	ROLEGRID stGrid;
	int iNum;
	int iBind = 0;

	if (INST_ITEM_BIND & pstItem->InstFlag)
	{
		iBind = 1;
	}

	if (item_create(pstEnv->pstAppCtx,pstEnv,&stGrid, pstResult->ResultVal2) <0)
	{
		return -1;
	}
	else
	{
		stGrid.GridData.RoleItem.Num = pstResult->ResultVal4;		
	}
	
	if (package_add_test(pstEnv->pstAppCtx, pstEnv,  pstPlayer, &stGrid, 1, 0) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FENJIE1);
		return -1;
	}
	
	if (pstResult->ResultVal1 == 1) // 开杂物包栏位
	{
		iIdx = pstPkg->EtSundPkg.MaxNum;
		if (iIdx >= MAX_EXTEND_SUND_PKG)
		{
			iRate = 0;
		}
		else
		{
			iRate = OpenSundEt[iIdx];
		}
	}
	else							// 开仓库栏位
	{
		iIdx = pstPkg->EtStashPkg.MaxNum;
		if (iIdx >= MAX_EXTEND_STASH_PKG)
		{
			iRate = 0;
		}
		else
		{
			iRate = OpenStashEt[iIdx];
		}	
	}
	
	iRate1 = RAND1(10000);
	if (iRate1 >= iRate)
	{
		iPos = package_grid_pos(pstPkg->SundriesPackage.RoleGrids, 
									pstPkg->SundriesPackage.Num, pstItem->GridIdx);
		if (0 > iPos)
		{
			return -1;
		}
		package_dec(pstEnv->pstAppCtx,pstEnv, pstPlayer, pstItem->DefID, 
						1, LIST_TYPE_SUNDRIES, &iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_PAK_EX);

		iNum = pstResult->ResultVal4 - pstResult->ResultVal3;
		if (iNum < 0)
			return -1;
		
		stGrid.GridData.RoleItem.Num = pstResult->ResultVal3+RAND1(iNum+1);
		if (iBind)
		{
			stGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
		}

		if (iRate != 0)
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK236, 
					stGrid.GridData.RoleItem.Num);
		package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer,&stGrid,1,0, IMPITEM_OPTYPE_PAK_EX);
		
		return -1;
	}
	
	return 0;
}

int pkg_use_check_open_et(ZONESVRENV* pstEnv, Player *pstPlayer,RESULT *pstResult,ROLEITEM *pstItem)
{
	ROLEPACKAGE *pstPkg = &pstPlayer->stRoleData.Package;
	int iOpen = pstResult->ResultVal2; // 开第几个格子 策划填的，从1开始
	int iRet = 0;
	//int iPos;

	if (pstResult->ResultVal1 == 1)
	{
		if (pstPkg->EtSundPkg.MaxNum == MAX_EXTEND_SUND_PKG)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK237);	
			return -1;
		}
		
		if (iOpen - pstPkg->EtSundPkg.MaxNum != 1)
		{
			iRet = -1;
		}
	}
	else
	{
		if (pstPkg->EtStashPkg.MaxNum == MAX_EXTEND_STASH_PKG)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK238);	
			return -1;
		}
		
		if (iOpen - pstPkg->EtStashPkg.MaxNum != 1)
		{
			iRet = -1;
		}
	}

	if (iRet )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK239);	
		return iRet;
	}
	
	return 0;
	UNUSED(pstItem);	//add by paraunused.pl
}

RESULT *item_have_result(ITEMDEF *pstItemDef, int iResultID)
{
	int i;
	for (i=0; i<MAX_ITEM_RESULT; i++)
	{
		if (0 == pstItemDef->Result[i].ResultID)
		{
			break;
		}

		if (iResultID == pstItemDef->Result[i].ResultID)
		{
			return &pstItemDef->Result[i];
		}
	}

	return NULL;
}

int pkg_use_add_impexp(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEITEM *pstRoleItem, 
									ITEMDEF *pstItemDef, CSPACKAGEUSE *pstUse)
{
	ROLEPET *pstRolePet = NULL;
//	PETDEF *pstPetDef = NULL;

	pstRolePet = z_find_role_pet(pstPlayer, pstUse->WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}	

	pstRolePet->ImpExp+=pstItemDef->Result[0].ResultVal1;

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET1, 
						pstRolePet->Name,pstItemDef->Result[0].ResultVal1);

	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);
	return 0;
}

int pkg_use_check_add_impexp(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEITEM *pstRoleItem, 
									ITEMDEF *pstItemDef, CSPACKAGEUSE *pstUse)
{
	ROLEPET *pstRolePet = NULL;
	PETDEF *pstPetDef = NULL;

	pstRolePet = z_find_role_pet(pstPlayer, pstUse->WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}	
	
	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (!pstPetDef)
	{
		return -1;
	}

	if (!(pstRolePet->StatFlag &  PET_STAT_FIGHT) )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK103 );
		return -1;
	}

	if (pstRolePet->Imp >= PET_MAX_IMP)
	{
		return -1;
	}

	return 0;
}

int pkg_use_check_add_wenbao(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEITEM *pstRoleItem, 
									ITEMDEF *pstItemDef, CSPACKAGEUSE *pstUse)
{
	ROLEPET *pstRolePet = NULL;
	PETDEF *pstPetDef = NULL;

	pstRolePet = z_find_role_pet(pstPlayer, pstUse->WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}	

	if (pstItemDef->Result[0].ResultVal2 > 0 && pstRolePet->Imp > pstItemDef->Result[0].ResultVal2)
	{
		return -1;
	}
	
	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (!pstPetDef)
	{
		return -1;
	}

	if (!(pstRolePet->StatFlag &  PET_STAT_FIGHT) )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK103 );
		return -1;
	}

	if (pstRolePet->WenBao >= PET_MAX_WENBAO)
	{
		return -1;
	}

	return 0;
}

int pkg_use_add_wenbao(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEITEM *pstRoleItem, 
									ITEMDEF *pstItemDef, CSPACKAGEUSE *pstUse)
{
	ROLEPET *pstRolePet = NULL;
//	int iOld = pstRolePet->WenBao;

	pstRolePet = z_find_role_pet(pstPlayer, pstUse->WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}	

	pstRolePet->WenBao+=pstItemDef->Result[0].ResultVal1;
	if (pstRolePet->WenBao > PET_MAX_WENBAO)
	{
		pstRolePet->WenBao = PET_MAX_WENBAO;	
	}
	
	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);
	return 0;
}

int item_result_check(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEITEM *pstRoleItem, ITEMDEF *pstItemDef, ZoneAni *pstTarAni, CSPACKAGEUSE *pstUse)
{
	int i;
	STATUSDEF *pstStatusDef;
	TASKDEF *pstTaskDef;
	RESULT *pstResult, *pstCheckResult, *pstResult1;
	//ROLEPACKAGE *pstRolePackage = &pstPlayer->stRoleData.Package;	//comment by paraunused.pl

	for (i=0; i<MAX_ITEM_RESULT; i++)
	{
		if (0 == pstItemDef->Result[i].ResultID)
		{
			break;
		}

		pstResult = pstItemDef->Result + i;

		switch(pstItemDef->Result[i].ResultID)
		{
			case RESULT_ITEM_MT:
				if (player_mt_gift_add_check(pstEnv, pstPlayer) < 0)
				{
					z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SHOP302);
					return -1;
				}
				break;
			case RESULT_PET_SILIAO:
				if (pkg_use_check_add_wenbao(pstEnv, pstPlayer, pstRoleItem, pstItemDef,pstUse) < 0)
				{
					return -1;
				}
				break;
			case RESULT_PET_QIANLI:
				if (pkg_use_check_add_impexp(pstEnv, pstPlayer, pstRoleItem,pstItemDef, pstUse) < 0)
				{
					return -1;
				}
				break;
			case RESULT_ITEM_TASK:
				pstTaskDef = task_def_find(pstEnv, pstResult->ResultVal1, NULL);
				if (NULL == pstTaskDef)
				{
					return -1;
				}

				if (0 > task_accept_check(pstEnv, pstPlayer, pstTaskDef, 0, 0))
				{
					return -1;
				}
				break;

			case RESULT_SKILL_BOOK:
				if (package_check_skill_book(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstResult, pstItemDef) < 0)
				{
					return -1;
				}
				break;

			case RESULT_RIDE:
				if ( package_check_ride(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstUse, pstResult, pstItemDef) < 0)
				{
					return -1;
				}
				break;

			case RESULT_ADD_POWER:
				if (package_check_addpower(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstResult, pstItemDef,pstUse->WID) < 0)
				{
					return -1;
				}
				break;
			case RESULT_ADD_EXP_PET:
				if (package_check_pet_addexp(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstResult, pstItemDef,pstUse->WID) < 0)
				{
					return -1;
				}
				break;
			case RESULT_ADD_SKILL_GRID:
				if (package_check_add_skill_grid(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstResult, pstItemDef,pstUse->WID) < 0)
				{
					return -1;
				}
				break;
			case RESULT_SHI_LIST:
				if(pstPlayer->stRoleData.Package.PetPackage.MaxNum >= MAX_PET_PACKAGE ||
					pstPlayer->stRoleData.MiscInfo.AddPetGrid >= MAX_ADD_PET_PACKAGE)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK135);
					return -1;
				}
				break;
			case RESULT_PET_ADD_ATTR:
				//if (package_check_pet_add_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstResult, pstItemDef,pstUse->WID) < 0)
				{
					return -1;
				}
				break;
			case RESULT_PET_ADD_LOVEVAL:
				if (package_check_pet_add_loveval(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstResult, pstItemDef,pstUse->WID) < 0)
				{
					return -1;
				}
				break;
			case RESULT_PET_HATCH:
				if (0 > pet_hatch_check(pstEnv, pstPlayer, pstResult, pstItemDef))
				{
					return -1;
				}
				break;
			case RESULT_ROLE_ADD_SKILLPOINT:
				
				break;
			/*
			case RESULT_FITTINGS_INLAY:
				if ( package_fittings_inlay_check(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstUse, pstResult) < 0)
				{
					return -1;
				}
				break;
			*/
			case RESULT_MP_ADD:
				{
					pstCheckResult  = z_item_get_result(pstItemDef, RESULT_MP_ADD);
					pstResult1 = z_item_get_result(pstItemDef, RESULT_HP_ADD);
					if (pstCheckResult == NULL)
						return -1;

					if (pstResult1)
					{
						if (pstPlayer->stRoleData.RoleDetail.MPCurr >= pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax &&
							pstPlayer->stRoleData.RoleDetail.HPCurr >= pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax)
						{
							z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK240);
							return -1;
						}
					}
					else
					{
						if (pstPlayer->stRoleData.RoleDetail.MPCurr >= pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax)
						{
							z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK243);
							return -1;
						}
					}
				}
			
				break;
			case RESULT_HP_ADD:
				{
					pstCheckResult  = z_item_get_result(pstItemDef, RESULT_HP_ADD);
					pstResult1 = z_item_get_result(pstItemDef, RESULT_MP_ADD);

					if (pstCheckResult == NULL)
						return -1;

					if (pstResult1)
					{
						if (pstPlayer->stRoleData.RoleDetail.MPCurr >= pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax &&
							pstPlayer->stRoleData.RoleDetail.HPCurr >= pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax)
						{
							z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK240);
							return -1;
						}
					}
					else
					{
						if (pstPlayer->stRoleData.RoleDetail.HPCurr >= pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax)
						{
							z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK240);
							return -1;
						}
					}
				}
				break;
			case RESULT_ADD_XP:
				if (pstPlayer->stOnline.stFairyOnline.ullCampWID <= 0)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK136);
					return -1;
				}
				//if (pstPlayer->stRoleData.RoleDetail.XP >= MAX_XP)
				//{
					//return -1;
				//}
				break;
			case RESULT_SELF_STATUS:

				if (pstItemDef->Result[i].ResultVal2 == DIE_REMIT_BUFFID)
				{
					if (z_player_find_status(pstPlayer, DIE_REMIT_BUFFID))
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK137);
						return -1;
					}
				}

				if (pstItemDef->Result[i].ResultVal2 == ITEM_EXP_BUFFID)
				{
					ZoneStatus *pstZoneStatus = NULL;
					int iLevel = 0;
				
					if(pstItemDef->Result[i].ResultVal3 == 2)
					{
						iLevel = 1;	
					}
					else if(pstItemDef->Result[i].ResultVal3 == 1)
					{
						iLevel = 2;	
					}
					else
					{
					}

					pstZoneStatus = z_player_find_status_by_level(pstPlayer, ITEM_EXP_BUFFID,iLevel);
					if (pstZoneStatus)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SUBJECT_04);
						return -1;
					}
				}
				
				pstStatusDef = z_find_status(pstEnv, pstResult->ResultVal2, pstResult->ResultVal3);
				if (!pstStatusDef)
				{
					return -1;
				}
				if (pstStatusDef->ModeID > 0 && !is_can_bianshen(pstEnv,pstPlayer,1,0))
				{
					return -1;
				}

				if (0 > z_player_apply_status_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstResult->ResultVal2, pstResult->ResultVal3))
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK138);
					return -1;
				}
				
				if (pstStatusDef->Type != STATUS_TYPE_BAD && pstStatusDef->Type != STATUS_TYPE_GOOD)
				{
					if (pstPlayer->stRoleData.StatusInfo.NoLimitStatusNum >= MAX_NOLIMIT_STATUS)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK139);
						return -1;
					}
				}
				
				if (pstPlayer->stOnline.State & CS_STAT_STALL )
				{
					if (pstResult->ResultVal2 != STATUS_ID_STALL_STYLE && pstResult->ResultVal2 != STATUS_ID_STALL_NAME_STYLE)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK140);
						return -1;
					}
				}

				if (	pstStatusDef &&
					pstStatusDef->IsWushuang &&
					pstPlayer->stRoleData.Map>= PWORLD_MAPID_BASE &&
					GET_PWORLDID(pstPlayer->stRoleData.Map) == BCT_PWORLD_ID)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK141);
					return -1;
				}

				/*
				if (pstResult->ResultVal2 == STATUS_FOOD_RESTORE_HP || pstResult->ResultVal2 == STATUS_FOOD_RESTORE_MP)
				{
					if (pstPlayer->stOnline.stStatusList.bBadStatusNum > 0)
					{
						z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, "不良状态下不能使用此道具");
						return -1;
					}
				}
				*/

				/*
				if (pstPlayer->stRoleData.MiscInfo.TmpSys.TmpItemID > 0)
				{
					ITEMDEF *pstItemDefTmp;

					pstItemDefTmp = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)&pstPlayer->stRoleData.MiscInfo.TmpSys);
					if (pstItemDefTmp && pstItemDefTmp->ItemTmp.Type == ITEM_TMP_MACHINE)
					{
						z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "当前条件下不能使用此道具");
						return -1;
					}
				}*/
				break;
			case RESULT_PLAYER_ADD_EXP:
				{
					/*
					LEVELUP *pstLevelUp;
					tdr_longlong iExp;
					int iRet;

					pstLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
					if (!pstLevelUp)
					{
						return -1;
					}

					iExp = pstLevelUp->ContributeExp * pstItemDef->Result[i].ResultVal1/10000.0;

					iRet = z_player_addexp_test(pstEnv, pstPlayer, &iExp);
					if (0 > iRet)
					{
						return -1;
					}
					*/
				}
				break;
			case RESULT_PLAYER_ADD_EXP_NOLIMIT:
				
				break;	
			case RESULT_ADD_STUFF_PACKAGE:
				//if (pstRolePackage->StuffPackage.MaxNum >= MAX_STUFF_PACKAGE)
				{
					return -1;
				}
				break;
			case RESULT_ADD_SUNDRIES_PACKAGE:
				//if (pstRolePackage->SundriesPackage.MaxNum >= MAX_SUNDRIES_PACKAGE)
				{
					return -1;
				}
				break;
			case RESULT_ADD_TASK_PACKAGE:
				//if (pstRolePackage->TaskPackage.MaxNum >= MAX_TASK_PACKAGE)
				{
					return -1;
				}
				break;
			case RESULT_DIATHESIS_UP:
				if (player_diathesis_up_check(pstEnv, pstPlayer,pstItemDef->Result[i].ResultVal1, pstItemDef->Result[i].ResultVal2) < 0)
				{
					return -1;
				}
				break;
			case RESULT_DO_SKILL:
				if (0 > player_item_do_skill_check(pstEnv, pstPlayer, pstResult, pstTarAni))
				{
					return -1;
				}
				break;
			case RESULT_BACK_HOME:
				{
					RESPOS stPos;
					int iMap = pstItemDef->Result[i].ResultVal1;
					stPos.X = pstItemDef->Result[i].ResultVal2;
					stPos.Y = pstItemDef->Result[i].ResultVal3;
				
					if (0 > z_player_goto_check(pstEnv, pstPlayer, iMap, &stPos))
					{
						return -1;
					}

					if (0 == g_use_pre && pstItemDef->ItemID >= 5030005 && pstItemDef->ItemID <= 5030007)
					{
						time_t now;
						struct tm *pstnow;

						now = pstEnv->pstAppCtx->stCurr.tv_sec;
						pstnow = localtime(&now);

						if (!(pstnow->tm_hour >= 16 && pstnow->tm_hour < 24))
						{
							return -1;
						}
					}
				}
			case RESULT_CALL_NPC:

				break;
			case RESULT_ADD_TMPENERGE:
				if(0 > player_item_do_tmpsys_check(pstEnv,pstPlayer, pstResult) )
				{
					return -1;
				}
				break;
			case RESULT_QIYUE:/*使用物品加契约*/
				if( 0 > player_item_do_qiyue_check(pstEnv,pstPlayer, pstResult) )
				{
					return -1;
				}
				break;	
			case RESULT_FAIRY_BORN:
				if (0 > fairy_born_check(pstEnv, pstPlayer,pstItemDef->Result[i].ResultVal1))
				{
					return -1;
				}
				break;
			case RESULT_ADD_DESIGNATION:
				if(player_find_designation(pstEnv, pstPlayer,
				                      pstItemDef->Result[i].ResultVal2) != NULL)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK244);
					return -1;
				}

				if (pstPlayer->stRoleData.MiscInfo.DesignationInfo.Num >= MAX_ROLE_DESIGNATION)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK245);
					return -1;	
				}
				break;
			case RESULT_ADD_RIDE_GRID:
				return package_add_ride_grid_check(pstEnv, pstPlayer, pstResult);
			case RESULT_DEC_FAIRY_TM:
				if (get_evolve_fairy(pstPlayer) == NULL)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK246);
					return -1;
				}
				break;
			case RESULT_USE_UNDISSOLVE:
				break;
			case RESULT_SECONDARY_SKILL_RECIPE:
				return player_use_recipe_item_check(pstEnv, pstPlayer, pstResult->ResultVal1, pstResult->ResultVal2);
				break;
			case RESULT_ETPKG_HOLE:
				return pkg_check_add_hole(pstEnv, pstPlayer, pstResult);
				break;
			
				break;
			case RESULT_ADD_VIM:
				{
					int iTmp;
					
					iTmp = pstPlayer->stOnline.stDyn.stMidAttrList.uiVimMax * (pstResult->ResultVal1/100.0) + pstResult->ResultVal2;

					if(pstPlayer->stRoleData.RoleDetail.VIMCurr >= pstPlayer->stOnline.stDyn.stMidAttrList.uiVimMax)
					{
						return -1;
					}
				}
				break;
			case RESULT_ADD_TMP_ITEM:
				if (pstPlayer->stRoleData.MiscInfo.TmpSys.TmpItemID > 0)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, 
						pstPlayer, SYS_MIDDLE, ZONEERR_PAK142);
					return -1;
				}
				else if (0 > check_replace_skill(pstEnv, pstPlayer, REPLACE_SKILL_OP_TMP))
				{
					return -1;
				}
				break;
			case RESULT_CATCH_PET:
				return package_use_catch_pet_check(pstEnv, pstPlayer, pstTarAni);
				break;
			case RESULT_MACHINE_ITEM:
				return package_use_machine_item_check(pstEnv, pstPlayer, pstRoleItem, pstItemDef, pstResult);
				break;
			case RESULT_FACTORY_ITEM:
				return package_use_factory_item_check(pstEnv, pstPlayer, pstResult, pstUse->WID);
				break;
			case RESULT_ADD_MACHINE_ENERGY:
				return package_use_machine_energy_check(pstEnv, pstPlayer, &pstItemDef->Result[i]);
				break;
			case RESULT_MACHINE_HP:
				return package_use_machine_hp_check(pstEnv, pstPlayer, &pstItemDef->Result[i]);
				break;

			case RESULT_MACHINE_STATUS:
				return package_use_machine_status_check(pstEnv, pstPlayer, &pstItemDef->Result[i]);
				break;	
			case RESULT_BEACH_EXP_MUL:
				if (player_beach_check_chair(pstEnv,pstPlayer,BEACH_CHAIR_RANGE) < 0)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK143);
					return -1;
				}
				{
					ZONEIDX stIdx;
					MapIndex * pstMapIdx = NULL;
					TMAPMASK * pstMask = NULL;
					ZONEMAPINST *pstMapInst = NULL; 

					stIdx.iID = pstPlayer->stRoleData.Map;
					stIdx.iPos = pstPlayer->stOnline.iMapPos;
					pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx,pstEnv, &stIdx);
							
					if (NULL == pstMapInst)
					{
						return -1;
					}

					pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);

					if (NULL == pstMapIdx)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK144);
						return -1;
					}

					pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask +pstMapIdx->iMaskOrigin);
					if(!IsPixelLoMagic(pstMask, pstPlayer->stRoleData.Pos.X,pstPlayer->stRoleData.Pos.Y))
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK144);
						return -1;
					}
							
				}
				break;
				
			case RESULT_TARGET_STATUS:
				if (pstTarAni->iType != OBJ_PLAYER || !pstTarAni->stObj.pstPlayer)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK126);
					return -1;
				}
				break;
			case RESULT_DROP_TMP_ITEM:
				{
					Player* pstTarget = NULL;
					int k = 0;
					RESULT* pstTmpItemResult = NULL;
					ITEMDEF* pstTmpItemDef = NULL;
					int iValid = 0;
					int iImmune = 0;
					
					if (pstTarAni->iType != OBJ_PLAYER || !pstTarAni->stObj.pstPlayer)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK126);
						return -1;
					}
					pstTarget = pstTarAni->stObj.pstPlayer;

					if (0 == pstTarget->stRoleData.MiscInfo.TmpSys.TmpItemID)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK127);
						return -1;
					}
					
					pstTmpItemDef = z_find_itemdef(pstEnv,pstTarget->stRoleData.MiscInfo.TmpSys.TmpItemID);
					if (!pstTmpItemDef)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK127);
						return -1;
					}
					if (pstResult->ResultVal1)
					{
						for (k = 0; k < MAX_ITEM_RESULT; k++)
						{
							pstTmpItemResult = pstTmpItemDef->Result + k;
							if (0 == pstTmpItemResult->ResultID)
							{
								break;
							}
							if (pstResult->ResultVal1 == pstTmpItemResult->ResultID)
							{
								iValid = 1;
							}

							if (pstResult->ResultVal3 == pstTmpItemResult->ResultID)
							{
								iValid = 0;
								iImmune = 1;
								break;
							}
							
						}
						if (0 == iValid)
						{
							if (iImmune)
							{
								z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK128);
							}
							else
							{
								z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK127);
							}
							
							return -1;
						}
					}
					
	
					
				}
				break;

			case RESULT_ITEM_SWITCH:
				
				break;

			case RESULT_ITEM_DEC_PKVAL:
				if (pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal <= 0)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK145);
					return -1;
				}
				break;
				
			case RESULT_IN_AREA_USE:
				if (item_have_result(pstItemDef, RESULT_ITEM_DEC_PKVAL))
				{
					RESULT *pstVipResult = star_get_result(pstEnv, pstPlayer, STAR_RESULT_DEC_PKVAL);
					if (pstVipResult)
					{
						break; 
					}
				}
				
				if (0 == is_map_same_refer(pstEnv, pstResult->ResultVal4, pstPlayer->stRoleData.Map) && 0 == is_map_same_line(pstEnv, pstResult->ResultVal4, pstPlayer->stRoleData.Map))
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK146);
					return -1;
				}
				else
				{
					if (pstResult->ResultVal2 && pstResult->ResultVal3 && pstResult->ResultVal1)
					{
						RESPOS stPos;
						stPos.X = pstResult->ResultVal2;
						stPos.Y = pstResult->ResultVal3;
						
						if (z_distance(&pstPlayer->stRoleData.Pos,&stPos) > pstResult->ResultVal1)
						{
							z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK147);
							return -1;
						}
					}
				}
				break;
			case RESULT_BACK_TO_CLAN_CITY:
				{
					if(0 > item_back_to_clan_city_check(pstEnv,pstPlayer))
					{
						return -1;
					}
					
					RESPOS stPost;
					stPost.X = pstItemDef->Result[i].ResultVal2;
					stPost.Y = pstItemDef->Result[i].ResultVal3;
					if(0 > clan_mem_enter_city_check(pstEnv,pstPlayer,&stPost))
					{
						return -1;
					}			

				}
				break;
			case RESULT_MEDIA_GIFT:
				if(0 > player_mediagift_use_check(pstEnv,pstPlayer,pstResult->ResultVal1,pstUse->SubIndex, pstRoleItem))
				{
					return -1;
				}
				break;
			case RESULT_LTR_GIFT:
				if(0 > player_ltrgift_use_check(pstEnv,pstPlayer,pstResult->ResultVal1, pstRoleItem))
				{
					return -1;
				}
				break;

			case RESULT_VIGOR:
				if(0 > player_add_vig_check(pstEnv,pstPlayer))
				{
					return -1;
				}
				break;
				
			case RESULT_TEAM_SUMMON:
				if(0 > item_result_team_summon_check(pstEnv,pstPlayer,&pstItemDef->Result[i]))
				{
					return -1;
				}
				break;
			case RESULT_TEAM_RATE_OPEN_PKG:
				return pkg_use_check_rate_open_et(pstEnv, pstPlayer,pstResult,pstRoleItem);
				break;
			case RESULT_TEAM_RATE_PKG:
				return pkg_use_check_open_et(pstEnv, pstPlayer,pstResult,pstRoleItem);
				break;

			case RESULT_MAP_RECORDER:
				if(0 >item_result_map_recorder_check(pstEnv,pstPlayer,&pstItemDef->Result[i],pstRoleItem,pstItemDef,pstUse))
				{
					return -1;
				}
				break;	
				
			case RESULT_TRANS_TO_LEADER:
				if(0 >item_result_trans_to_leader_check(pstEnv,pstPlayer,&pstItemDef->Result[i]))
				{
					return -1;
				}
				break;
			case RESULT_SHAM_DEAD:
				if(pstPlayer->stOnline.fShamDead > 0)
				{
					return -1;
				}
				break;
			case RESULT_COLOR_FONT_TIME:
			case RESULT_MOTION_TIME:
				break;
			case RESULT_ADD_MACHINE_STATUS:
				if(0 >item_result_add_machine_status_check(pstEnv,pstPlayer,&pstItemDef->Result[i]))
				{
					return -1;
				}
				break;	
				
			case RESULT_BEACH_EXP_MUL_IMMUNE:
				break;
			case RESULT_USE_OFFLINE_EXP:
				if(0 > player_use_all_offline_exp_check(pstEnv, pstPlayer))
				{
					//return -1;
				}
				break;
			case RESULT_USE_PWORLD_OFFLINE_EXP:
				if(0 > player_use_all_pworld_offexp_check(pstEnv, pstPlayer))
				{
					//return -1;
				}
				break;
			case RESULT_CLAN_SUMMON:
				if(0 > player_clan_summon_ask_check(pstEnv, pstPlayer, &pstItemDef->Result[i]))
				{
					return -1;
				}
				break;
			case RESULT_VIP:
				if (0 > star_check_item_result(pstEnv, pstPlayer, &pstItemDef->Result[i]))
				{
					return -1;
				}
				break;
			case RESULT_ADD_FASHION_SUIT:
				if(0 > player_add_fashion_suit_check(pstEnv, pstPlayer))
				{
					return -1;
				}
				break;
			case RESULT_ADD_CLAN_MONEY:
				if ( player_add_clan_money_check(pstEnv, pstPlayer, pstItemDef->Result[i].ResultVal2) < 0)
				{
					return -1;
				}
				break;
			case RESULT_SPARTA_RUNAWAY_CLEAR:
				break;
			case RESULT_SPARTA_ALL_CLEAR:
				break;
			case RESULT_MAP_RAND_TRANSMIT:
				if(0 > map_rand_transmit_check(pstEnv, pstPlayer))
				{
					return -1;
				}
				break;
			case RESULT_CHARM:

				break;
			case RESULT_LOST_TOTEM:
				if (losttotem_use_check(pstEnv, pstPlayer, pstRoleItem, pstItemDef) < 0)
				{
					return -1;
				}
				break;
			case RESULT_ITEM_EXP_BUCHANG:
				if (pstUse->SubIndex > 0)	// 表示金币领取
				{
					int iGold = ceil(pstRoleItem->InstVal1/1000000.0);
					if (iGold <= 0)
						iGold = 1;
					
					if (package_dec_money_test(pstEnv,pstPlayer, ATTR_ID_GODCOIN,iGold) < 0)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
										SYS_MIDDLE, ZONEERR_ARM_WASH4);
						return -1;
					}
				}
				break;
			case RESULT_ITEM_ARM_BUCHANG:
				if ( item_use_arm_buchang_check(pstEnv, pstPlayer, pstRoleItem, 
					pstPlayer->stRoleData.Level/10,pstUse) < 0)
				{
					return -1;
				}
				break;
			case RESULT_ITEM_ARM_SELECT:
				if ( item_use_arm_buchang_check(pstEnv, pstPlayer, pstRoleItem, 
					pstItemDef->Result[i].ResultVal1,pstUse) < 0)
				{
					return -1;
				}
				break;
			case RESULT_ITEM_CLAN_CONTRI:
				if(0 > package_use_clan_contri_check(pstEnv, pstPlayer, pstRoleItem))
				{
					return -1;
				}
				break;
			case RESULT_ZHENGYI_ADD:
				{
					unsigned int iZhengyi = pstResult->ResultVal1;
					if (0 > player_add_zhengyi_test(pstEnv,  pstPlayer, &iZhengyi))
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK703);
						return -1;
					}
				}
				break;
			case RESULT_ZHENGYI_LIMIT_ADD:
				break;
			case RESULT_PET_GENIUS_UP:
				if (pet_genius_up_high_check( pstEnv, pstPlayer,pstItemDef,pstUse) < 0)
				{
					return -1;
				}
				break;
			case RESULT_BUDDY_VAL:
				break;
			
			case RESULT_ITEM_GEM_PAG:
				if ( item_use_gem_pag_check(pstEnv, pstPlayer,pstUse,pstItemDef->Result[i].ResultVal1) < 0)
				{
					return -1;
				}
				break;
			default:
				return -2;
				break;
		}
	}

	return 0;
}

int package_get_item(ZONESVRENV* pstEnv, Player *pstPlayer, int iListType,
						int iGridIdx, ITEMDEF **ppstItemDef, ROLEITEM **ppstRoleItem,
						int *piPos)
{
	int iPos = 0;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	
	switch(iListType)
	{
		case LIST_TYPE_SUNDRIES:
			if (iGridIdx >= pstPak->SundriesPackage.MaxNum)
			{
				return -1;
			}
			iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, iGridIdx);
			if (0 > iPos)
			{
				return -1;
			}
			*ppstRoleItem = &pstPak->SundriesPackage.RoleGrids[iPos].GridData.RoleItem;
			*ppstItemDef = z_get_itemdef_roleitem(pstEnv, *ppstRoleItem);
			if (NULL == (*ppstItemDef))
			{
				return -1;
			}
			break;
		case LIST_TYPE_SHOPPING:
			if (iGridIdx >= pstPak->ShoppingPackage.MaxNum)
			{
				return -1;
			}
			iPos = package_grid_pos(pstPak->ShoppingPackage.RoleGrids, pstPak->ShoppingPackage.Num, iGridIdx);
			if (0 > iPos)
			{
				return -1;
			}

			*ppstRoleItem = &pstPak->ShoppingPackage.RoleGrids[iPos].GridData.RoleItem;
			*ppstItemDef = z_get_itemdef_roleitem(pstEnv, *ppstRoleItem);
			if (NULL == (*ppstItemDef))
			{
				return -1;
			}

			/*if ( GRID_ITEM_ARM == pstPak->ShoppingPackage.RoleGrids[iPos].Type)
			{
				return -1;
			}*/			
			break;
		case LIST_TYPE_TASK:
			
			if (iGridIdx >= pstPak->TaskPackage.MaxNum)
			{
				return -1;
			}

			iPos = package_itemgrid_pos(pstPak->TaskPackage.RoleItems, pstPak->TaskPackage.Num, iGridIdx);
			if (0 > iPos)
			{
				return -1;
			}

			*ppstRoleItem = pstPak->TaskPackage.RoleItems+iPos;
			*ppstItemDef = z_get_itemdef_roleitem(pstEnv, *ppstRoleItem);
			if (NULL == (*ppstItemDef))
			{
				return -1;
			}

			break;
				
		default:
			return -1;
			break;
	}

	*piPos = iPos;

	return 0;
}

int package_use_get_target(ZONESVRENV* pstEnv, Player *pstPlayer,ITEMDEF *pstItemDef, CSPACKAGEUSE *pstUse, ZoneAni *pstTarAni)
{
	 RESPOS stTarPos;
	 pstTarAni->iType = OBJ_UNKNOW;
	

	switch(pstItemDef->UseTarget)
	{
		case USE_TARGET_MONSTER:
			pstTarAni->iType = OBJ_MONSTER;
			pstTarAni->stObj.pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstUse->WID);
			if (!pstTarAni->stObj.pstMon || pstTarAni->stObj.pstMon->stMap.iID != pstPlayer->stRoleData.Map)
			{
				return -1;
			}

			stTarPos = pstTarAni->stObj.pstMon->stCurrPos;
			break;
		case USE_TARGET_NPC:
			pstTarAni->iType = OBJ_NPC;
			pstTarAni->stObj.pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstUse->WID);
			if (!pstTarAni->stObj.pstNpc ||pstTarAni->stObj.pstNpc->stMon.stMap.iID != pstPlayer->stRoleData.Map)
			{
				return -1;
			}

			stTarPos = pstTarAni->stObj.pstNpc->stMon.stCurrPos;
			break;
		case USE_TARGET_PLAYER:
			pstTarAni->iType = OBJ_PLAYER;
			pstTarAni->stObj.pstPlayer= z_id_player(pstEnv->pstAppCtx, pstEnv, pstUse->WID);
			if (!pstTarAni->stObj.pstPlayer || pstTarAni->stObj.pstPlayer->stRoleData.Map != pstPlayer->stRoleData.Map)
			{
				return -1;
			}

			stTarPos = pstTarAni->stObj.pstPlayer->stRoleData.Pos;
			break;

		case USE_TARGET_PET:
			pstTarAni->iType = OBJ_PET;
			pstTarAni->stObj.stZonePet.pstOwner = pstPlayer;
			pstTarAni->stObj.stZonePet.pstPetFightShi = NULL;
			pstTarAni->stObj.stZonePet.pstRolePet =  z_find_role_pet(pstPlayer, pstUse->WID, NULL);
			if (!pstTarAni->stObj.stZonePet.pstRolePet)
			{
				return -1;
			}

			stTarPos = pstPlayer->stRoleData.Pos;
		default:
			return 0;
			break;
	}

	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	}

	if(z_distance(&pstPlayer->stRoleData.Pos, &stTarPos) > MAX_USE_ITEM_DIST + DIST_TORRENT*3)
	{
		return -1;
	}

	return 0;
}

int package_use_test(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef,
						ROLEITEM *pstRoleItem, CSPACKAGEUSE *pstUse)
{
	ZoneAni stTarAni;
	int iMap = pstPlayer->stRoleData.Map;
	ZONEPWORLD *pstPworld;
	ZONEIDX stIdx;
	int iLevel = 0;
	LEVELUP *pstLevelUp = NULL;
	int iRet;

	if ((0 == (ITEM_IGNORE_NOITEM & pstItemDef->CtrlFlag)) && z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOITEM))
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK148);
		return -1;
	}

	
	if (0 == z_player_act_status(pstAppCtx, pstEnv, pstPlayer, RES_ACTION_USEITEM))
	{
		return -1;
	}

	if((pstPlayer->stOnline.State & CS_STAT_ATK)||
		(pstPlayer->stOnline.State & CS_STAT_MOVE_ATK) ||
		(pstPlayer->stOnline.State & CS_STAT_PREPARE) ||
		(pstPlayer->stOnline.State & CS_STAT_MOVE_PREPARE))
	{
		if(!(pstItemDef->CtrlFlag & ITEM_PRE_USE))
		{
			return -1;
		}
	}

	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		// 战场不能使用的物品
		if(pstItemDef->BattleNoUse)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK149);
			return -1;
		}
	}

	//不能在末世荣耀殿堂内使用
	if(WORLD_MO_SHI_MAPID == GET_MAPID(iMap))
	{
		if(pstItemDef->CtrlFlag & ITEM_MO_SHI_NO_USE)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK702);
			return -1;
		}
	}

	if(GET_PWORLDID(iMap) == DIANFENG_PWORLD_ID)
	{
		if(pstItemDef->CtrlFlag & ITEM_MO_SHI_NO_USE)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_DIANFENG2);
			return -1;
		}
	}
	
	if (iMap >= PWORLD_MAPID_BASE)
	{
		stIdx.iID = GET_PWORLDID(iMap);
		stIdx.iPos = -1;
		pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);	
		if (	pstPworld &&
			(pstPworld->stPworldDef.CtrlFlag & PWORLD_NO_RESTORE_ITEM) &&
			pstItemDef->IsReStoreItem)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK150);
			return -1;
		}

		if(pstItemDef->ItemTmp.Type == ITEM_TMP_CLAN)
		{
			if (pstPworld && (pstPworld->stPworldDef.CtrlFlag & PWORLD_NO_CLAN_ITEM))
			{
				z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK141);
				return -1;
			}
		}
	}

	if (pstItemDef->ValidTime > 0 && pstRoleItem->ValidEnd !=0 && pstRoleItem->ValidEnd < pstAppCtx->stCurr.tv_sec)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK151);
		return -1;
	}

	if (ITEM_CHGNAME_ID == pstItemDef->ItemID)
	{
		if (pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stRoleData.MiscInfo.NameChangeTimes < 7*24*3600)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_PAK152);
			return -1;
		}
		return 0;
	}

	if (0 > z_package_use_pre(pstEnv, pstPlayer, pstItemDef))
	{
		return -1;
	}
	 
	 if( 0 == pstItemDef->Result[0].ResultID && 
	 	 (0 == (ITEM_SCRIPT_USE_MAP & pstItemDef->ItemScript)) &&
	 	 (pstItemDef->ItemTmp.Type != ITEM_TMP_CLAN) )
	 {
	 	return -1;
	 }

	 if ((pstItemDef->UseTarget == USE_TARGET_NO && pstUse->WID > 0) || 
	 	(pstItemDef->UseTarget == USE_TARGET_PET && pstUse->WID == 0) ||
	 	(pstItemDef->UseTarget == USE_TARGET_MONSTER && pstUse->WID == 0) ||
	 	(pstItemDef->UseTarget == USE_TARGET_NPC && pstUse->WID == 0) ||
	 	(pstItemDef->UseTarget == USE_TARGET_PLAYER && pstUse->WID == 0))
	 {
	 	return -1;
	 }

	 /* 镶嵌的判定 */
	 if (   pstItemDef->UseTarget == USE_TARGET_INLAY && 
	 	    pstUse->WID == 0 && 
	 	    pstUse->SubIndex == 0 )
	 {
	 	return -1;
	 }

	 if (0 > package_use_get_target(pstEnv, pstPlayer, pstItemDef, pstUse, &stTarAni))
	 {
	 	return -1;
	 }

	 if(pstItemDef->UseTarget == USE_TARGET_PLAYER)
	{
		Player *pstTempPlayer = stTarAni.stObj.pstPlayer;
		
		if (0 > check_buddy_val(pstEnv, pstPlayer,pstTempPlayer,pstItemDef))
		{
			return -1;
		}
	}
	
	if (PURSUE_ITEM_ID == pstItemDef->ItemID && pstItemDef->UseTarget == USE_TARGET_PLAYER)
	{
		Player *pstTempPlayer = stTarAni.stObj.pstPlayer;
		if (0 > check_pursue_item(pstEnv, pstPlayer,pstTempPlayer,pstItemDef))
		{
			return -1;
		}
	}

	 if(pstItemDef->UseTarget == USE_TARGET_PET)
	 {
		ROLEPET *pstRolePet = stTarAni.stObj.stZonePet.pstRolePet;

		if ((pstItemDef->WearPre.RoleLevelMin != 0 && pstRolePet->Level < pstItemDef->WearPre.RoleLevelMin) ||
			(pstItemDef->WearPre.RoleLevelMax != 0 && pstRolePet->Level > pstItemDef->WearPre.RoleLevelMax))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_PAK247);
			return -1;
		}
	 }

	/* 镶嵌类物品不做冷却判断 */
	if( pstItemDef->UseTarget != USE_TARGET_INLAY )
	{
		//判断冷却
		if (0 > item_cool_check(pstEnv, pstPlayer, pstItemDef))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK248);
			return -1;
		}
	}

	if(pstItemDef->MaxUseOneDay > 0)
	{
		USEITEMONEDAYINFO *pstItemOneDayInfo;
		
		pstItemOneDayInfo = z_get_item_oneday_info(pstEnv, pstPlayer, pstItemDef->ItemID);
		if( pstItemOneDayInfo && pstItemOneDayInfo->OneDayNum >= (int)pstItemDef->MaxUseOneDay)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK153);
			return -1;
		}
	}

	if (pkg_use_item_switch_check(pstEnv, pstPlayer, pstItemDef,pstRoleItem) < 0)
	{
		return -1;
	}
	
	iRet = item_result_check(pstEnv, pstPlayer, pstRoleItem, pstItemDef, &stTarAni, pstUse);
	if ( (-1 == iRet) || (-2 == iRet  && (0 == (ITEM_SCRIPT_USE_MAP & pstItemDef->ItemScript)) ))
	{
		return -1;
	}

	// 转职物品
	if( CAREERCHG_ITEMID == pstItemDef->ItemID )
	{
		iLevel = pstPlayer->stRoleData.Level;
		if( iLevel != CAREERCHG_LEVEL )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_PAK154);
			return -1;
		}
		pstLevelUp = (LEVELUP *)pstEnv->pstZoneObj->sLevelUpDef[iLevel-1];
		if(pstPlayer->stRoleData.RoleDetail.Exp <  (int)pstLevelUp->LevelNextExp)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_PAK155);
			return -1;
		}
	}

	//战斗中不可以使用的物品
	if(ITEM_FIGHT_NO_USE & pstItemDef->CtrlFlag)
	{
		if(pstPlayer->stOnline.State & CS_STAT_FIGHT)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK156);
			return -1;
		}
	}

	if(is_player_ride_on_machine(pstPlayer))
	{
		if(!(pstItemDef->CtrlFlag & ITEM_RIDE_MACHINE_USE))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK157);
			return -1;
		}
	}

	if( pstItemDef->ItemTmp.Type == ITEM_TMP_CLAN )
	{
		if (0 > check_replace_skill(pstEnv, pstPlayer, REPLACE_SKILL_OP_CLANITEM))
		{
			return -1;
		}
	}
	
	return 0;
}

int package_use_clan_item(ZONESVRENV* pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef)
{
	ITEMDEF *pstItemDefOld = NULL;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_CLAN_ITEM;
	
	if (pstItemDef && pstItemDef->ItemTmp.Type != ITEM_TMP_CLAN)
	{
		return -1;
	}

	if(pstPlayer->stOnline.ClanItemID>0)
	{
		if (0 > check_replace_skill(pstEnv, pstPlayer, REPLACE_SKILL_OP_CLANITEM))
		{
			return -1;
		}
		
		pstItemDefOld = z_find_itemdef(pstEnv, pstPlayer->stOnline.ClanItemID);
		if(pstItemDefOld)
		{
			if (pstItemDefOld->ItemTmp.SkillID_1 > 0)
			{
				z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDefOld->ItemTmp.SkillID_1, 1);
			}

			if (pstItemDefOld->ItemTmp.SkillID_2 > 0)
			{
				z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDefOld->ItemTmp.SkillID_2, 1);
			}
			pstActionRes->Data.ClanItem.ClanItemID = 0;
			Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
			//z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
			z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
		}
	}
	else if(!pstItemDef)
	{
		pstActionRes->Data.ClanItem.ClanItemID = 0;
		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		//z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	}

	pstPlayer->stOnline.ClanItemID = 0;
	
	if (pstItemDef)
	{
		/*
		tmp_item_drop( pstEnv,  pstPlayer,0);
		if(pstRoleTmpSys->TmpItemID>0)
		{
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "有临时道具在使用中!");
			return -1;
		}*/
		
		if (pstItemDef->ItemTmp.SkillID_1 > 0)
		{
			z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstItemDef->ItemTmp.SkillID_1, pstItemDef->ItemTmp.SkillLevel_1, 1);
		}

		if (pstItemDef->ItemTmp.SkillID_2 > 0)
		{
			z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstItemDef->ItemTmp.SkillID_2, pstItemDef->ItemTmp.SkillLevel_2, 1);
		}
		
		pstPlayer->stOnline.ClanItemID = pstItemDef->ItemID;
			
		pstActionRes->Data.ClanItem.ClanItemID = pstItemDef->ItemID;
		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		//z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	}
	
	return 0;
}

static int pkg_use_item_switch_check(ZONESVRENV* pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef,
									ROLEITEM *pstRoleItem)
{
	if (pstItemDef->Result[0].ResultID == RESULT_ITEM_SWITCH && pstRoleItem->InstVal1 > 0)
	{
		if (pstItemDef->Result[0].ResultVal1 == RESULT_ITEM_SWITCH_TYPE_EXP)
		{
			tdr_longlong ullExp = pstRoleItem->InstVal1;
			if (z_player_addexp_test(pstEnv,pstPlayer,&ullExp) < 0)
			{
				return -1;
			}
		}
		else if(pstItemDef->Result[0].ResultVal1 == RESULT_ITEM_SWITCH_TYPE_GOLD)
		{
			if (package_add_money_test(pstEnv, pstPlayer, ATTR_ID_BINDGODCOIN, pstRoleItem->InstVal1) < 0)
			{
				return -1;
			}
		}
		else if(pstItemDef->Result[0].ResultVal1 == RESULT_ITEM_SWITCH_TYPE_BULL)
		{
			if (package_add_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, pstRoleItem->InstVal1) < 0)
			{
				return -1;
			}
		}
		else if(pstItemDef->Result[0].ResultVal1 == RESULT_ITEM_SWITCH_TYPE_MONEY1)
		{
			if (package_add_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, pstRoleItem->InstVal1) < 0)
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}
	return 0;
}

static int pkg_use_item_switch(ZONESVRENV* pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef,
								ROLEITEM *pstRoleItem)
{
	if (pstItemDef->Result[0].ResultID == RESULT_ITEM_SWITCH && pstRoleItem->InstVal1 > 0)
	{
		if (pstItemDef->Result[0].ResultVal1 == RESULT_ITEM_SWITCH_TYPE_EXP)
		{
			player_add_exp(pstEnv, pstPlayer,pstRoleItem->InstVal1, 0);
		}
		else if(pstItemDef->Result[0].ResultVal1 == RESULT_ITEM_SWITCH_TYPE_GOLD)
		{
			package_add_money(pstEnv, pstPlayer, ATTR_ID_BINDGODCOIN, pstRoleItem->InstVal1);
			z_bindgold_create_oplog(pstEnv, pstPlayer, BINDGOLD_OPTYPE_ITEM, pstItemDef->ItemID, 
						pstRoleItem->InstVal1, "道具效果");
		}
		else if(pstItemDef->Result[0].ResultVal1 == RESULT_ITEM_SWITCH_TYPE_BULL)
		{
			package_add_money(pstEnv, pstPlayer, ATTR_ID_BULL, pstRoleItem->InstVal1);
			z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ITEM, pstItemDef->ItemID, pstRoleItem->InstVal1, 0, "道具效果");
		}
		else if(pstItemDef->Result[0].ResultVal1 == RESULT_ITEM_SWITCH_TYPE_MONEY1)
		{
			package_add_money(pstEnv, pstPlayer, ATTR_ID_MONEY, pstRoleItem->InstVal1);
			z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ITEM, pstItemDef->ItemID, 0,pstRoleItem->InstVal1,  "item result");
		}
	}
	return 0;
}
int package_use_in(ZONESVRENV* pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef,
						ROLEITEM *pstRoleItem, CSPACKAGEUSE *pstUse, int iPos)
{
	ScriptIndex *pstScriptInd;
	RESULT *pstResult;
	RESULT *pstMapRecorderResult;
	ZoneAni stTarAni;
	char szScript[32];	
	ROLEITEM stRoleItem;
	int iOldHP = pstPlayer->stRoleData.RoleDetail.HPCurr;
	int iOldMP = pstPlayer->stRoleData.RoleDetail.MPCurr;
	ITEMDEC stDec;
	int iDecFlag = 0;

	memcpy(&stRoleItem, pstRoleItem, sizeof(stRoleItem));
	stDec.ID = stRoleItem.DefID;
	stDec.Num = 1;
	stDec.BindNum = 0;
	if(pstRoleItem->InstFlag & INST_ITEM_BIND)
		stDec.BindNum = 1;

	//检查绑定
	if (pstItemDef->ArmBind == ITEM_WEAR_BIND && !(pstRoleItem->InstFlag & INST_ITEM_BIND))
	{
		pstRoleItem->InstFlag |= INST_ITEM_BIND;
		package_grid_update_to_clt(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstUse->SrcListType, pstRoleItem, NULL);
	}
	
	pstResult = z_item_get_result(pstItemDef, RESULT_USE_UNDISSOLVE);
	pstMapRecorderResult = z_item_get_result(pstItemDef, RESULT_MAP_RECORDER);
	if (!pstResult ) 
	{
		if (pstUse->UseType == PACKAGE_USE_RECORD && pstMapRecorderResult)
		{
			//do nothing
		}	
		else
		{
			//有使用次数的道具
			if(pstRoleItem->InstVal1 > 1 && pstItemDef->UseMaxDur > 0)
			{
				pstRoleItem->InstVal1--;
				stRoleItem.InstVal1 --;
		
				z_role_item_dec_oplog(pstEnv,pstPlayer, &stDec,OP_ITEM_DEC_USE_ONCE);

				//更新道具
				package_grid_update_to_clt(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstUse->SrcListType, pstRoleItem, NULL);
			}
			else if( pstItemDef->ItemTmp.Type == ITEM_TMP_CLAN )
			{
				if (0 > package_use_clan_item(pstEnv, pstPlayer, pstItemDef))
				{
					return -1;
				}
			}
			else
			{	
				if(pstRoleItem->InstVal1 && pstItemDef->UseMaxDur > 0)
				{
					stRoleItem.InstVal1 = 0;
				}

				if (0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDef->ItemID, 1, pstUse->SrcListType, &iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_USE))
				{
					return -1;
				}

				iDecFlag = 1;
				
				z_role_item_dec_oplog(pstEnv,pstPlayer, &stDec,OP_ITEM_DEC_USE);
			}
		}
	}

	package_use_get_target(pstEnv, pstPlayer, pstItemDef, pstUse, &stTarAni);

	if (iDecFlag)
	{
		item_result(pstEnv, pstPlayer, pstItemDef, &stTarAni, pstUse, &stRoleItem);
	}
	else
	{
		item_result(pstEnv, pstPlayer, pstItemDef, &stTarAni, pstUse, pstRoleItem);
	}
	
	//完成使用物品任务
	if(pstPlayer->stOnline.cMiscFlag & MISC_FLAG_USE_ITEM)
	{
		task_use_item_fini_trace(pstEnv, pstPlayer, pstItemDef);
	}
	
	
	if ((int)pstPlayer->stRoleData.RoleDetail.HPCurr != iOldHP ||
		(int)pstPlayer->stRoleData.RoleDetail.MPCurr != iOldMP)
	{
		player_hp_action(pstEnv, pstPlayer, iOldHP, iOldMP, 0);
	}

	if (pstItemDef->ItemScript & ITEM_SCRIPT_USE_MAP)
	{
		stSEnv.pstMonster = NULL;
		stSEnv.pstNpc = NULL;
		stSEnv.pstPlayer = pstPlayer;
		stSEnv.pstPworldInst = NULL;
		stSEnv.pstRoleItem = &stRoleItem;
		stSEnv.stCsPackageUse = *pstUse;
		stSEnv.iItemDefID = pstItemDef->ItemID;
		stSEnv.pstMapinst = NULL;
		stSEnv.pstKiller = NULL;
		if (pstUse->WID > 0)
		{
			stSEnv.iMemID = pstUse->WID;
		}
		
		if (stTarAni.iType == OBJ_MONSTER)
		{
			stSEnv.pstMonster = stTarAni.stObj.pstMon;
		}
		else if (stTarAni.iType == OBJ_PLAYER)
		{
			
		}
		else if (stTarAni.iType == OBJ_NPC)
		{
			stSEnv.pstNpc = stTarAni.stObj.pstNpc;
		}
		
		snprintf(szScript, sizeof(szScript), "item%d.mac", pstItemDef->ItemID);				
		pstScriptInd = get_script_ind(szScript);

		if ( !pstScriptInd )
		{
			printf("get_script_ind fail\n");
			tlog_error(pstEnv->pstLogCat, 0, 0, "get_script_ind fail");
			return -1;
		}
		
		z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, szScript, pstScriptInd, ITEM_SCRIPT_USE);
		
		stSEnv.iMemID = 0;
		stSEnv.pstRoleItem = NULL;
		stSEnv.iItemDefID = 0;
	}


	if (OBJ_UNKNOW == stTarAni.iType )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_COMBAT, ZONEERR_PAK249,pstItemDef->Name);
	}
	else
	{
		char szName[64];
		szName[0] = 0;
		
		if (stTarAni.iType == OBJ_MONSTER)
		{
			MONSTERDEF* pstMonDef = z_get_mon_def(pstEnv, stTarAni.stObj.pstMon);
			if (pstMonDef)
			{
				STRNCPY(szName,pstMonDef->Name,sizeof(szName));
			}
		}
		else if (stTarAni.iType == OBJ_PLAYER)
		{
			char szSelf[ROLE_NAME_LEN];
			
			STRNCPY(szName,stTarAni.stObj.pstPlayer->stRoleData.RoleName,sizeof(szName));
			battle_rolename_remove_prefix(szName);

			STRNCPY(szSelf, pstPlayer->stRoleData.RoleName,sizeof(szSelf));
			battle_rolename_remove_prefix(szSelf);
			
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, stTarAni.stObj.pstPlayer, SYS_CHAT, ZONEERR_PAK250, szSelf, pstItemDef->Name);
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, stTarAni.stObj.pstPlayer, SYS_COMBAT, ZONEERR_PAK250, szSelf, pstItemDef->Name);
		}
		else if (stTarAni.iType == OBJ_NPC)
		{
			MONSTERDEF* pstMonDef = z_get_mon_def(pstEnv, &stTarAni.stObj.pstNpc->stMon);
			if (pstMonDef)
			{
				STRNCPY(szName,pstMonDef->Name,sizeof(szName));
			}
		}
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_PAK251,szName,pstItemDef->Name);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_CHAT, ZONEERR_PAK251,szName,pstItemDef->Name);
	}

	if (pstItemDef->ItemID == ITEM_CHGNAME_ID)
	{
		open_chg_name_ui(pstEnv->pstAppCtx, 
					pstEnv, pstPlayer, pstPlayer->iMemID, UI_TYPE_CHG_ROLE_NAME);
	}
	
	if (PURSUE_ITEM_ID == pstItemDef->ItemID && pstItemDef->UseTarget == USE_TARGET_PLAYER)
	{
		Player *pstPlayerA = stTarAni.stObj.pstPlayer;
		pstPlayer->stOnline.ullLoveID = pstPlayerA->stRoleData.RoleID;
		pstPlayer->stOnline.cPursueName[0] = 0;
		STRNCPY(pstPlayer->stOnline.cPursueName,pstPlayerA->stRoleData.RoleName,sizeof(pstPlayer->stOnline.cPursueName));
		relation_involve_marry_svr(pstEnv,pstPlayerA,pstPlayer->stRoleData.RoleName,TYPE_PURSUE_INFO_INFORM_SVR);
	}

	return 0;
}

int package_use(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSPACKAGEUSE *pstUse)
{
	ITEMDEF *pstItemDef;
	CSPKG *pstPkgRes = &pstEnv->stCSRes;
	CSACTIONRES *pstActionRes = &pstPkgRes->Body.ActionRes;
	ROLEITEM *pstRoleItem;
	int iPos;
	int iID;
	
	if (0 > package_get_item(pstEnv, pstPlayer, pstUse->SrcListType, 
							pstUse->SrcGridIdx, &pstItemDef, &pstRoleItem, &iPos))
	{
		goto _error;
	}

	if ( GRID_ITEM_ARM == pstItemDef->ItemType)
	{
		goto _error;
	}

	if(pstItemDef->ItemUsePre > 0)
	{
		if(pstPlayer->stOnline.stItemUseInfo.tPreTime + pstItemDef->ItemUsePre  > pstAppCtx->stCurr.tv_sec + 1)
		{
			goto _error;
		}
	}

	if (0 > package_use_test(pstAppCtx, pstEnv, pstPlayer, pstItemDef, pstRoleItem, pstUse))
	{
		goto _error;
	}

	pkg_use_item_switch(pstEnv, pstPlayer, pstItemDef,pstRoleItem);
	package_use_in(pstEnv, pstPlayer, pstItemDef, pstRoleItem, pstUse, iPos);
	
	//设置冷却
	item_cool_set(pstEnv, pstPlayer, pstItemDef);

	//清理物品吟唱数据
	memset(&pstPlayer->stOnline.stItemUseInfo, 0, sizeof(pstPlayer->stOnline.stItemUseInfo));

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_USE_ITEM;
	pstActionRes->Data.UseItem.ItemID = pstItemDef->ItemID;
		
	Z_CSHEAD_INIT(&pstPkgRes->Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, pstPkgRes, 0);

	// 称号
	iID = pstItemDef->ItemID;
	if (	iID == 5010002 || iID == 5010003 || iID == 5010004 || iID == 5010005 ||
		iID == 5011002 || iID == 5011003 || iID == 5011004 || iID == 5011005)
	{
		player_desig_get(pstEnv, pstPlayer,GET_DESIG_USE_FOOD);
	}
	return 0;

_error:

	if(pstPlayer->stOnline.stItemUseInfo.tPreTime > 0)
	{
		package_use_pre_brk(pstEnv, pstPlayer);
	}

	return 0;
}

#if 0
FITTINGSDEF *pakcage_check_skill_inlay_fitting(ZONESVRENV*pstEnv,ITEMDEF *pstItemDef,SKILLDEF *pstSkillDef,
													unsigned short *pushDefIdx)
{
	int i = 0;
	int j = 0;
	RESULT *pstResult =NULL;
	FITTINGSDEF *pstFittingDef = NULL;
	
	if(pstItemDef == NULL || pstSkillDef == NULL  )
	{
		return NULL;
	}
	
	for (i=0; i<MAX_ITEM_RESULT; i++)
	{
		if (0 == pstItemDef->Result[i].ResultID)
		{
			break;
		}

		pstResult = pstItemDef->Result + i;

		switch(pstItemDef->Result[i].ResultID)
		{	
			case RESULT_FITTINGS_INLAY:
				for(j=0; j<MAX_SKILL_FITTINGS; j++ )
				{
					if(pstResult->ResultVal1 == pstSkillDef->FittingLists[j].ID)
					{
						if(pstResult->ResultVal2 == 0 )
						{
							pstFittingDef = z_find_fittings(pstEnv, pstResult->ResultVal1, 1, pstSkillDef->FittingLists[j].Group,pushDefIdx);
						}
						else
						{
							pstFittingDef = z_find_fittings( pstEnv, pstResult->ResultVal1, 
																pstResult->ResultVal2, 
																pstSkillDef->FittingLists[j].Group,pushDefIdx);
						}
						return pstFittingDef;
					}
				}
				break;
			default:
				break;
		}
	}

	return NULL;
}
#endif

#if 0
int package_req_fittings_pick(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, 
								CSPACKAGEFITTINGSPICK *pstPackageFittingsPick)
{
	ZoneSkill *pstZoneSkill = NULL;
	SKILLDEF *pstSkillDef = NULL;
	FITTINGSMAPLISTDATA *pstFittingsMap = NULL;
	FITTINGSDEF *pstFittingDef = NULL;
	unsigned short ushDefIdx = 0;
	unsigned int uiBull = 0;
	ROLEGRID stRoleGrid;

	if(pstPackageFittingsPick->SubIndex  >= MAX_FITTINGS_GRID )
	{
		z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "参数错误,摘取配件失败");
		return -1;
	}
	
	/* 把定义找出来 */	
	pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstPackageFittingsPick->SkillID);
	if ( NULL == pstZoneSkill )
	{
		z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "没找到用户的技能");
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "package_req_fittings_pick 没找到用户的技能(%d) ",pstPackageFittingsPick->SkillID);
		return -1;
	}
	pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill);	
	if (!pstSkillDef)	
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "package_req_fittings_pick 没找到用户的技能(%d) ",pstPackageFittingsPick->SkillID);
		return -1;
	}		

	/* 先把初始化的数据给取下*/
	pstFittingsMap = &pstZoneSkill->stSkillData.CSFittingsRowList[0].FittingsMap;
	if (0 == pstFittingsMap->FittingsGrids[pstPackageFittingsPick->SubIndex].FittingsID)
	{
		z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "该位置没有配件!");
		return -1;
	}
		
	pstFittingDef = z_find_fittings( pstEnv, pstFittingsMap->FittingsGrids[pstPackageFittingsPick->SubIndex].FittingsID, 
										pstFittingsMap->FittingsGrids[pstPackageFittingsPick->SubIndex].Level, 
										pstSkillDef->FittingLists[pstPackageFittingsPick->SubIndex].Group,
										&ushDefIdx);

	if( !pstFittingDef )
	{
		z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "摘取配件失败!");
		return -1;
	}

	//看看背包是否能装下
	if(pstFittingDef->PickItemID > 0)
	{
		memset(&stRoleGrid,0,sizeof(stRoleGrid));
		if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstFittingDef->PickItemID))
		{
			z_send_sys_str( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, "摘取配件失败!");
			return -1;
		}

		/*if (GRID_ITEM_ITEM == stRoleGrid.Type)
		{
			stRoleGrid.GridData.RoleItem.Num = 1;
		}*/
		
		if (0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0))
		{
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "背包空间不足,请先整理背包!");
			return -1;
		}
	}

	uiBull = pstFittingDef->PickBull;
	/* 开始扣流程了*/
	if( uiBull > 0 )
	{
		if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, uiBull) )
		{
			z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "当前%s不足", MONEY_TYPE_MONEY_NAME);
			return -1;
		}
	}

	package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, uiBull);
	
	package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0);
	
	//更改配件相关的东西	
	pstFittingsMap->FittingsGrids[pstPackageFittingsPick->SubIndex].FittingsID = 0;
	pstFittingsMap->FittingsGrids[pstPackageFittingsPick->SubIndex].DefIdx = 0;
	pstFittingsMap->FittingsGrids[pstPackageFittingsPick->SubIndex].GroupID = 0;
	pstFittingsMap->FittingsGrids[pstPackageFittingsPick->SubIndex].Level = 0;	

	z_update_one_skill( pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstZoneSkill);
	z_skill_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstZoneSkill);

	z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, "技能的配件摘取成功");
		
	return 0;
}
#endif

#if 0
int package_req_fittings_inlay(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, 
								CSPACKAGEINLAY *pstPackageFittingsInlay)
{
	/* 配件的镶嵌规则这里来弄了*/
	FITTINGSDEF *pstFittingDef = NULL;
	ZoneSkill *pstZoneSkill = NULL;
	SKILLDEF *pstSkillDef = NULL;
	int i = 0;
	int j = 0;
	unsigned short ushDefIdx = 0;
	CSPACKAGEUSE	*pstUse = NULL;
	ITEMDEF *pstItemDef = NULL;
	//ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	int iPos;
	//ROLEGRID *pstSrcGrid;
	ROLEITEM *pstRoleItem;
	FITTINGSMAPLISTDATA stFittingsMap;    
	int bFlag  = 0; /* 取一次标志位 */
	unsigned int uiBull = 0;
	short arrType[MAX_FITTINGS_GRID];

	memset(arrType,0,sizeof(arrType));
		
	for(j = 0; j< pstPackageFittingsInlay->ArrNum;j++)
	{
		if( j >= MAX_FITTINGS_GRID)
		{
			
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "package_req_fittings_inlay参数错误,arrnum:%u",
				 pstPackageFittingsInlay->ArrNum);
			return -1;
		}

		pstUse = &pstPackageFittingsInlay->ItemUse[j];
		if(  pstUse->SubIndex >= MAX_FITTINGS_GRID )
		{
			return -1;
		}
		/* 把定义找出来 */	
		pstZoneSkill = z_get_player_zoneskill(pstPlayer, (unsigned short)pstUse->WID);
		if ( NULL == pstZoneSkill )
		{
			z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "没找到用户的技能");
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "fittings_inlay_check 没找到用户的技能(%llu) ",pstUse->WID);
			return -1;
		}
		pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill);	
		if (!pstSkillDef)	
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "fittings_inlay_check 没找到用户的技能(%llu) ",pstUse->WID);
			return -1;
		}		
		if(bFlag == 0 )
		{
			bFlag = 1;

			/* 先把初始化的数据给取下*/
			stFittingsMap = pstZoneSkill->stSkillData.CSFittingsRowList[0].FittingsMap;
			for( i=0; i<MAX_FITTINGS_GRID; i++ )
			{
				if (0 == stFittingsMap.FittingsGrids[i].FittingsID)
				{
					continue;
				}
				
				pstFittingDef = z_find_fittings( pstEnv, stFittingsMap.FittingsGrids[i].FittingsID, 
													stFittingsMap.FittingsGrids[i].Level, 
													pstSkillDef->FittingLists[i].Group,
													&ushDefIdx);
				if( pstFittingDef )
				{
					arrType[i] = pstFittingDef->Type;
				}
			}
		}
		
		if( pstUse->SubIndex >= pstSkillDef->FittingsGridNum )
		{
			return -1;
		}

		if (0 > package_get_item(pstEnv, pstPlayer, pstUse->SrcListType, pstUse->SrcGridIdx,
							&pstItemDef, &pstRoleItem, &iPos))
		{
			return -1;
		}

		if(pstItemDef == NULL)
		{
			return -1;
		}

		if (0 > package_use_test(pstAppCtx, pstEnv, pstPlayer, pstItemDef, pstRoleItem, pstUse))
		{
			return -1;
		}
		
		pstFittingDef = pakcage_check_skill_inlay_fitting(pstEnv, pstItemDef, pstSkillDef,&ushDefIdx);
		if (!pstFittingDef)
		{
			z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "本技能不能镶嵌这个配件");
			return -1;
		}
		
		if ( (CAREER_NON != pstFittingDef->CareerLimit && (pstFittingDef->CareerLimit&0x0F) != (pstPlayer->stRoleData.Career&0x0F)) || 
			 (CAREER_NON != (pstFittingDef->CareerLimit&0xF0) && (pstFittingDef->CareerLimit&0xF0) != (pstPlayer->stRoleData.Career&0xF0))  )
		{
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "职业不符,不能镶嵌此配件");
			return -1;
		}	

		uiBull +=  pstFittingDef->InlayBull ;
			
		/* 核实这个格子是否能装备这个类型 */
		if( pstUse->SubIndex == 0 )
		{
			if( pstSkillDef->FittingsGridType1 == 0  && 
				pstFittingDef->Type != 0 )
			{
				z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "该镶嵌孔只能装备无花类配件");
				return -1;
			}
			else if(pstSkillDef->FittingsGridType1 != 0  && 
					pstFittingDef->Type == 0 )
			{
				z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "该镶嵌孔只能装备花类配件");
				return -1;
			}
		}
		else if( pstUse->SubIndex == 1)
		{
			if( pstSkillDef->FittingsGridType2 == 0  && 
				pstFittingDef->Type != 0 )
			{
				z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "该镶嵌孔只能装备无花类配件");
				return -1;
			}
			else if(pstSkillDef->FittingsGridType2 != 0  && 
					pstFittingDef->Type == 0 )
			{
				z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "该镶嵌孔只能装备花类配件");
				return -1;
			}
		}
		else if( pstUse->SubIndex == 2)
		{
			if( pstSkillDef->FittingsGridType3 == 0  && 
				pstFittingDef->Type != 0 )
			{
				z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "该镶嵌孔只能装备无花类配件");
				return -1;
			}
			else if(pstSkillDef->FittingsGridType3 != 0  && 
					pstFittingDef->Type == 0 )
			{
				z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "该镶嵌孔只能装备花类配件");
				return -1;
			}	
		}
		else
		{
			return -1;
		}

		stFittingsMap.FittingsGrids[ pstUse->SubIndex ].DefIdx = ushDefIdx;
		stFittingsMap.FittingsGrids[ pstUse->SubIndex ].FittingsID = pstFittingDef->FittingsID;
		stFittingsMap.FittingsGrids[ pstUse->SubIndex ].GroupID = pstFittingDef->GroupID;
		stFittingsMap.FittingsGrids[ pstUse->SubIndex ].Level = pstFittingDef->Level;
		arrType[pstUse->SubIndex] =  pstFittingDef->Type;
	}
	
	/* 判断下 这三件是否可以共存 */
	for( i=0; i< MAX_FITTINGS_GRID; i++ )
	{
		if (0 == stFittingsMap.FittingsGrids[i].FittingsID)
		{
			continue;
		}
		for(j = i+1;j< MAX_FITTINGS_GRID; j++ )
		{
			if( arrType[i] == 0  && 
				arrType[j] == 0  )
			{
				if( stFittingsMap.FittingsGrids[i].FittingsID == stFittingsMap.FittingsGrids[j].FittingsID )
				{
					z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "不能镶嵌相同id的配件");
					return -1;
				}
			}
			else if( arrType[i]  != 0  && arrType[j] != 0  )
			{
				if(arrType[i] == arrType[j] )
				{
					z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "不能镶嵌相同类型的配件");
					return -1;
				}
			}
		}
	}
	
	/* 开始扣流程了*/
	if( uiBull > 0 )
	{
		//金钱
		if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, uiBull) )
		{
			z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "当前%s不足", MONEY_TYPE_MONEY_NAME);
			return -1;
		}

		if (0 > package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, uiBull))
		{
			z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "扣%s失败", MONEY_TYPE_MONEY_NAME);
			return -1;
		}
	}

	/* 扣物品 */
	for(i = 0; i<pstPackageFittingsInlay->ArrNum && i<MAX_FITTINGS_GRID; i++ )
	{
		package_use(pstAppCtx, pstEnv,pstPlayer, &pstPackageFittingsInlay->ItemUse[i]);
	}
	
	return 0;
}
#endif

int package_req_move_all_shopping(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, 
								 CSPACKAGEMOVEALLSHOPPING *pstPackageMoveAllShopping)
{

	CSPKG *pstPkgRes = &pstEnv->stCSRes;
	CSPACKAGEINFO *pstPackageInfo = &pstPkgRes->Body.PackageInfo;
	
	int i = 0;
	SHOPPINGPACKAGE *pstShoppingPackage = &pstPlayer->stRoleData.Package.ShoppingPackage;
	ROLEGRID *pstRoleGrids = NULL;

	if(LIST_TYPE_SHOPPING !=  pstPackageMoveAllShopping->ListType )
	{
		return -1;
	}
		
	if( pstShoppingPackage->Num <=0 )
	{
		return -1;
	}

	if (player_check_safelock(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}
	
	for( i = pstShoppingPackage->Num-1; i>=0; i--)
	{
		pstRoleGrids = &pstShoppingPackage->RoleGrids[i];
	
		if(package_add_test(pstAppCtx, pstEnv, pstPlayer, pstRoleGrids, 1,0) == 0)
		{
			package_add(pstAppCtx, pstEnv, pstPlayer, pstRoleGrids, 1,0, 0);

			/* 不是最后一个数据 */
			pstShoppingPackage->Num--;
			if( i !=  pstShoppingPackage->Num )
			{
				memcpy(pstRoleGrids,&pstShoppingPackage->RoleGrids[pstShoppingPackage->Num],sizeof(ROLEGRID));
			}				
		}
		
	}

	if( pstShoppingPackage->Num > 0 )
	{
		 z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
	}

	/*下发整个背包数据 */
	pstPackageInfo->Type = PACKAGE_INFO_ALL;
	memcpy(&pstPackageInfo->PackageInfoData.PackageAll, &pstPlayer->stRoleData.Package, sizeof(pstPackageInfo->PackageInfoData.PackageAll));
	Z_CSHEAD_INIT(&pstPkgRes->Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkgRes);
	return 0;
}




int package_use_pre_action(ZONESVRENV* pstEnv, Player *pstPlayer, CSPACKAGEUSE *pstPackageUsePre)
{
	CSPKG *pstPkgRes = &pstEnv->stCSRes;
	CSACTIONRES *pstActionRes = &pstPkgRes->Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_USE_ITEM_PRE;
	pstActionRes->Data.UseItemPre.ItemID = pstPlayer->stOnline.stItemUseInfo.iPreItemID;
	pstActionRes->Data.UseItemPre.TargetID = pstPlayer->stOnline.stItemUseInfo.iPreTargetID;
	pstActionRes->Data.UseItemPre.ListType = pstPackageUsePre->SrcListType;
	pstActionRes->Data.UseItemPre.GridIdx = pstPackageUsePre->SrcGridIdx;
		
	Z_CSHEAD_INIT(&pstPkgRes->Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, pstPkgRes, 0);
	
	return 0;
}

int package_use_pre_brk(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG *pstPkgRes = &pstEnv->stCSRes;
	CSACTIONRES *pstActionRes = &pstPkgRes->Body.ActionRes;

	memset(&pstPlayer->stOnline.stItemUseInfo, 0, sizeof(pstPlayer->stOnline.stItemUseInfo));

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_USE_ITEM_PRE;
	pstActionRes->Data.UseItemPre.ItemID = 0;
	pstActionRes->Data.UseItemPre.TargetID = 0;
	pstActionRes->Data.UseItemPre.ListType = 0;
	pstActionRes->Data.UseItemPre.GridIdx = 0;
		
	Z_CSHEAD_INIT(&pstPkgRes->Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, pstPkgRes, 0);


	return 0;
}

int package_use_pre(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSPACKAGEUSE *pstPackageUsePre)
{
	ITEMDEF *pstItemDef;
	ROLEITEM *pstRoleItem;
	int iPos;
	int iRet = 0;
	ZoneAni stTarAni;

	if (0 > package_get_item(pstEnv, pstPlayer, pstPackageUsePre->SrcListType, 
							pstPackageUsePre->SrcGridIdx, &pstItemDef, &pstRoleItem, &iPos))
	{
		return package_use_pre_brk(pstEnv, pstPlayer);
	}

	if ( GRID_ITEM_ARM == pstItemDef->ItemType)
	{
		return package_use_pre_brk(pstEnv, pstPlayer);
	}

	g_use_pre = 1;
	iRet = package_use_test(pstAppCtx, pstEnv, pstPlayer, pstItemDef, pstRoleItem, pstPackageUsePre);
	g_use_pre = 0;

	if (0 > iRet)
	{
		return package_use_pre_brk(pstEnv, pstPlayer);
	}

	//物品的脚本测试	
	iRet = package_use_get_target(pstEnv, pstPlayer, pstItemDef, pstPackageUsePre, &stTarAni);
	if(iRet <  0)
	{
		return package_use_pre_brk(pstEnv, pstPlayer);
	}
		 
	if (pstItemDef->ItemScript & ITEM_SCRIPT_USE_TEST_MAP)
	{
		char szScript[128];
		ScriptIndex *pstScriptInd = NULL;

		memset(szScript,0,sizeof(szScript));
		
		stSEnv.pstMonster = NULL;
		stSEnv.pstNpc = NULL;
		stSEnv.pstPlayer = pstPlayer;
		stSEnv.pstPworldInst = NULL;
		stSEnv.pstRoleItem = pstRoleItem;
		stSEnv.stCsPackageUse = *pstPackageUsePre;
		stSEnv.iItemDefID = pstItemDef->ItemID;
		stSEnv.pstMapinst = NULL;
		stSEnv.pstKiller = NULL;
		if (pstPackageUsePre->WID > 0)
		{
			stSEnv.iMemID = pstPackageUsePre->WID;
		}
		
		if (stTarAni.iType == OBJ_MONSTER)
		{
			stSEnv.pstMonster = stTarAni.stObj.pstMon;
		}
		else if (stTarAni.iType == OBJ_PLAYER)
		{
			
		}
		else if (stTarAni.iType == OBJ_NPC)
		{
			stSEnv.pstNpc = stTarAni.stObj.pstNpc;
		}
		
		snprintf(szScript, sizeof(szScript), "item%d.mac", pstItemDef->ItemID);				
		pstScriptInd = get_script_ind(szScript);

		if ( !pstScriptInd )
		{
			printf("get_script_ind fail\n");
			tlog_error(pstEnv->pstLogCat, 0, 0, "get_script_ind fail");
		}
		else
		{		
			z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, szScript, pstScriptInd, ITEM_SCRIPT_USE_TEST);

			stSEnv.iMemID = 0;
			stSEnv.pstRoleItem = NULL;
			stSEnv.iItemDefID = 0;

			if (stSEnv.iReturnFlag & RETURN_FLAG_INT)
			{
				if (0 != stSEnv.iReturnValue)
				{
					return package_use_pre_brk(pstEnv, pstPlayer);
				}
			}
		}
	}

	pstPlayer->stOnline.stItemUseInfo.iPreItemID = pstItemDef->ItemID;
	pstPlayer->stOnline.stItemUseInfo.tPreTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	if(pstItemDef->UseTarget > 0)
	{
		pstPlayer->stOnline.stItemUseInfo.iPreTargetID = pstPackageUsePre->WID;
	}
	
	return package_use_pre_action(pstEnv, pstPlayer, pstPackageUsePre);
}

/*
int stash_et_add_hole(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	EXTENDSTASHPKG *pstEtStashPkg = &pstPlayer->stRoleData.Package.EtStashPkg;
	 
	if (pstEtStashPkg->MaxNum >= MAX_EXTEND_STASH_PKG)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "扩充银行达到上限");
		return -1;
	}

	if (pstEtStashPkg->MaxNum >= (int)sizeof(StashEtMoneys)/sizeof(StashEtMoneys[0]))
	{
		return -1;
	}
	
	if (package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, StashEtMoneys[pstEtStashPkg->MaxNum]) < 0)
	{
		z_sys_strf(pstEnv, pstPlayer,SYS_MIDDLE2,"银行扩充第%d个栏位需要%d %s",
					pstEtStashPkg->MaxNum,StashEtMoneys[pstEtStashPkg->MaxNum],MONEY_TYPE_MONEY_NAME);
		return -1;
	}

	package_dec_money(pstEnv, pstPlayer, ATTR_ID_BULL, StashEtMoneys[pstEtStashPkg->MaxNum]);
	
	pstEtStashPkg->MaxNum ++;
	role_pkg_num_chg(pstEnv, pstPlayer, LIST_TYPE_ET_STASH);
	z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "银行扩充栏位增加");
	return 0;
	UNUSED(pstAppCtx);
}
*/

int player_package_req(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSPACKAGEREQ *pstPackageReq = &pstCsPkg->Body.PackageReq;
	UNUSED(pstFrameHead);

	switch(pstPackageReq->Type)
	{
		case PACKAGE_REQ_ALL:
			/* 登录时下行包量太大，服务器在登录1秒后发送此回应
			if (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_PACKAGE_ALL)
			{
				return -1;
			}
			
			pstPackageInfo->Type = PACKAGE_INFO_ALL;
			memcpy(&pstPackageInfo->PackageInfoData.PackageAll, &pstPlayer->stRoleData.Package, sizeof(pstPackageInfo->PackageInfoData.PackageAll));
			Z_CSHEAD_INIT(&pstPkgRes->Head, PACKAGE_INFO);
			if (0 <= z_cltmsg_send(pstEnv, pstPlayer, pstPkgRes))
			{
				pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_PACKAGE_ALL;
			}*/

			break;
			
		case PACKAGE_REQ_MOVE:
			package_move(pstAppCtx, pstEnv, pstPlayer, &pstPackageReq->PackageReqData.PackageMove);
			break;

		case PACKAGE_REQ_ARRANGE:
			package_arrange(pstAppCtx, pstEnv, pstPlayer, pstPackageReq->PackageReqData.PackageArrange.ListType);
			break;

		case PACKAGE_REQ_MONEY:
			package_money(pstAppCtx, pstEnv, pstPlayer, &pstPackageReq->PackageReqData.PackageMoney);
			break;

		case PACKAGE_REQ_USE:
			package_use(pstAppCtx, pstEnv, pstPlayer, &pstPackageReq->PackageReqData.PackageUse);
			break;

		case PACKAGE_REQ_PICK:
			package_pick(pstAppCtx, pstEnv, pstPlayer, &pstPackageReq->PackageReqData.PackagePick);
			break;

		case PACKAGE_REQ_BOOTY_SELECT:
			//package_booty_select(pstAppCtx, pstEnv, pstPlayer, &pstPackageReq->PackageReqData.PackagePickSelect);
			break;

		case PACKAGE_REQ_TMPDROP:
			tmp_item_drop(pstEnv, pstPlayer,0);
			break;
		/*
		case PACKAGE_REQ_FITTINGS_INLAY:
			package_req_fittings_inlay(pstAppCtx,pstEnv, pstPlayer,  &pstPackageReq->PackageReqData.PackageFittingsInlay);
			break;
		case PACKAGE_REQ_FITTINGS_PICK:
			package_req_fittings_pick(pstAppCtx,pstEnv, pstPlayer,  &pstPackageReq->PackageReqData.PackageFittingsPick);
			break;
		*/
		case PACKAGE_REQ_MOVE_ALL_SHOPPING:
			package_req_move_all_shopping(pstAppCtx,pstEnv, pstPlayer,  &pstPackageReq->PackageReqData.PackageMoveAllShopping);
			break;
		case PACKAGE_REQ_CANGBAOTU_RESET:
			package_cangbaotu_reset(pstEnv, pstPlayer,  &pstPackageReq->PackageReqData.PackageCangBaoTuReset);	
			break;
		case PACKAGE_REQ_USE_PRE:
			package_use_pre(pstAppCtx, pstEnv, pstPlayer, &pstPackageReq->PackageReqData.PackageUsePre);
			break;
		case PACKAGE_REQ_ADD_STASH_ET:
			//stash_et_add_hole(pstAppCtx, pstEnv, pstPlayer);
			break;
		case PACKAGE_REQ_USE_PRE_BRK:
			package_use_pre_brk(pstEnv, pstPlayer);
			break;
		default:
			break;
	}
	
	return 0; 
}

int package_current_item_check(ZONESVRENV *pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef, ROLEITEM *pstItem)
{
	if (!pstPlayer ||!pstItemDef || !pstItem)
	{
		return -1;
	}

	if(pstPlayer->stOnline.stMachineInfo.ullLockItemWID > 0 && 
		pstPlayer->stOnline.stMachineInfo.ullLockItemWID == pstItem->WID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK159, pstItemDef->Name);
		return -1;
	}

	if (	pstItem->ValidEnd > 0 &&
		pstItem->ValidEnd < pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK160, pstItemDef->Name);
		return -1;
	}
	
	if (pstItem->InstFlag & INST_ITEM_BIND)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK158);
		return -1;
	}

	if (pstItemDef->NoDeal || pstItemDef->NoDrop )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK161, pstItemDef->Name);
		return -1;
	}

	/*if (pstItemDef->ItemType == ITEM_TYPE_TASK)
	{
		snprintf(szMsg, sizeof(szMsg), "物品 %s 是任务物品,不能交易",pstItemDef->Name);
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, szMsg);
		return -1;
	}

	if (pstItemDef->ArmBind != ITEM_NONE_BIND)
	{
		snprintf(szMsg, sizeof(szMsg), "物品 %s 是绑定物品,不能交易",pstItemDef->Name);
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, szMsg);
		return -1;
	}*/
	
	if (pstItemDef ->ExchgLevel > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK162, pstItemDef->Name);
		return -1;
		/*if (!pstBuddyMember)
		{
			snprintf(szMsg, sizeof(szMsg), "物品 %s 只能在好友间交流",pstItemDef->Name);
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, szMsg);
			return -1;
		}
			
		if ( 0 > trust_buddyval_check(pstBuddyMember->BuddyVal, pstItemDef ->ExchgLevel))
		{
				z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "好友度未达到流通要求");
				return -1;
		}*/
	}				

	return 0;
}

int  player_skill_cool_clean(ZONESVRENV* pstEnv, Player *pstPlayer, unsigned short unSkillID)
{
	int i;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;
	ZoneSkill *pstZoneSkill = NULL;
	CSPKG stPkg;
	CSSKILLINFO *pstSkillInfo = &stPkg.Body.SkillInfo;

	pstSkillInfo->Type = SKILL_COOL_CLEAN;
	pstSkillInfo->Data.SkillCool.SkillID = unSkillID;
	for (i =0; i < pstZoneSkillList->nSkillNum; i++)
	{
		pstZoneSkill = &pstZoneSkillList->astZoneSkills[i];
		
		if (unSkillID && pstZoneSkill->stSkillData.SkillID == unSkillID)
		{
			TV_CLONE(pstZoneSkill->stCoolEnd, pstEnv->pstAppCtx->stCurr);

			pstSkillInfo->Data.SkillCool.SkillType = SKILL_COOL_ONE;
			Z_CSHEAD_INIT(&stPkg.Head, SKILL_INFO);
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

			return 0;
		}

		if (!unSkillID)
		{
			TV_CLONE(pstZoneSkill->stCoolEnd, pstEnv->pstAppCtx->stCurr);
		}
	}

	if (!unSkillID)
	{
		pstSkillInfo->Data.SkillCool.SkillType = SKILL_COOL_ALL;
		Z_CSHEAD_INIT(&stPkg.Head, SKILL_INFO);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	return 0;	
}

BIRTHITEMDEF *birthitem_def_find(ZONESVRENV *pstEnv, int iCareer, int iGender)
{
	BIRTHITEMDEF  stBirthItem;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stBirthItem.Career = iCareer;
	stBirthItem.Gender = iGender;
	
	iIndex = bsearch_int(&stBirthItem, pstObj->sBirthItemDef, pstObj->iBirthItemDef, BIRTH_ITEM_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (BIRTHITEMDEF *)pstObj->sBirthItemDef[iIndex];
	}
	
	return NULL;	
}

int z_player_wear_pos_check(ZONESVRENV* pstEnv, ROLEWEAR *pstRoleWear, ITEMDEF *pstItemDef)
{
	int i;
	char sMap[WEAR_POS_MAX+1];
	ITEMDEF *pstwearItemDef = NULL;

	memset(sMap, 0, sizeof(sMap));
	
	for (i=0; i<pstRoleWear->ArmNum; i++)
	{
		pstwearItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)&pstRoleWear->RoleArms[i]);
		if (!pstwearItemDef)
		{
			continue;
		}

		if (pstwearItemDef->WearPos1 == WEAR_POS_RING || pstwearItemDef->WearPos1 == WEAR_POS_AMULET)
		{
			sMap[pstRoleWear->RoleArms[i].GridIdx] = 1;
			continue;
		}
		
		if (z_wear_pos_rep(pstItemDef, pstwearItemDef, pstItemDef->WearPos1, pstRoleWear->RoleArms[i].GridIdx))
		{
			return -1;
		}
	}

	if (pstItemDef->WearPos1 == WEAR_POS_RING)
	{
		for (i = WEAR_POS_RING1 ; i <= WEAR_POS_RING2; i++)
		{
			if (sMap[i] == 0)
			{
				return i;
			}
		}

		return -1;
	}
	else if (pstItemDef->WearPos1 == WEAR_POS_AMULET)
	{
		for (i = WEAR_POS_AMULET1; i <= WEAR_POS_AMULET2; i++)
		{
			if (sMap[i] == 0)
			{
				return i;
			}
		}

		return -1;
	}

	return 0;
}


int z_player_add_and_wear_arm(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, 
									Player *pstPlayer, ITEMDEF *pstItemDef,ROLEGRID *pstRoleGrid)
{
	int iRet;
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	CSWEAR stCSWear;
	
	if (package_add_test(pstAppCtx, pstEnv, pstPlayer, pstRoleGrid, 1,0) == 0)
	{
		package_add(pstAppCtx, pstEnv, pstPlayer, pstRoleGrid, 1,0, 0);

		iRet = z_player_wear_pos_check(pstEnv, pstRoleWear, pstItemDef);
		if ( iRet < 0)
		{	
			tlog_error(pstEnv->pstLogCat, 0, 0, "z_player_wear_pos_check:(ItemID=%d)",pstItemDef->ItemID);
			return -1;
		}
		
		stCSWear.ArmDefID = pstRoleGrid->GridData.RoleArm.DefID;
		stCSWear.GridIdx = pstRoleGrid->GridData.RoleArm.GridIdx;
		stCSWear.ListType = LIST_TYPE_SUNDRIES;
		stCSWear.WearPos = iRet;
		stCSWear.PetWID = 0;
		if (player_wear(pstAppCtx, pstEnv, pstPlayer, &stCSWear) < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "player_wear:(ItemID=%d)",pstItemDef->ItemID);
		}
		
		return 0;
	}
	return -1;
}

int z_player_add_gift(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i = 0;
	ROLEGRID stRoleGrid;
	BIRTHITEMDEF *pstBirthItem = NULL;
	ITEMDEF *pstItemDef = NULL;
	int iApperalIdx = pstPlayer->stRoleData.ApperalSelect;
	int iFirstApperalID = 0;
	int iIdx = -1;
	int iFind = 0;

	pstBirthItem = birthitem_def_find(pstEnv, pstPlayer->stRoleData.Career, pstPlayer->stRoleData.Gender);
	if (!pstBirthItem)
	{
		return -1;
	}

	for(i = 0; i< MAX_BIRTH_ITEM; i++)
	{
		if (pstBirthItem->Items[i].ItemID == 0)
			{break;}
		
		pstItemDef = z_find_itemdef(pstEnv, pstBirthItem->Items[i].ItemID);
		if (!pstItemDef)
		{
			continue;
		}

		if (0 > item_create(pstAppCtx, pstEnv, &stRoleGrid, pstItemDef->ItemID))
		{
			continue;
		}

		//stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
		
		if (pstItemDef->ItemType != ITEM_TYPE_ARM)
		{
			stRoleGrid.GridData.RoleItem.Num = pstBirthItem->Items[i].ItemNum;
			if (package_add_test(pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0) == 0)
			{
				package_add(pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0, 0);
			}
			continue;
		}	

		if (	pstItemDef->WearPos1 == WEAR_POS_BODY ||
			pstItemDef->WearPos2 == WEAR_POS_BODY ||
			pstItemDef->WearPos3 == WEAR_POS_BODY)
		{
			iIdx++;
			if (iApperalIdx != iIdx)
			{
				if (iFirstApperalID == 0)
					iFirstApperalID = 	pstItemDef->ItemID;
				continue;
			}
			else
			{
				iFind = 1;
			}
		}
		z_player_add_and_wear_arm(pstAppCtx, pstEnv, pstPlayer,pstItemDef, &stRoleGrid);
	}

	if (!iFind && iFirstApperalID > 0)
	{
		pstItemDef = z_find_itemdef(pstEnv, iFirstApperalID);
		if (!pstItemDef)
			return -1;
		if (item_create(pstAppCtx, pstEnv, &stRoleGrid, iFirstApperalID) == 0)
		{
			//stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
			z_player_add_and_wear_arm(pstAppCtx, pstEnv, pstPlayer,pstItemDef, &stRoleGrid);
		}
	}
	
	return 0;
}


int arm_is_head(ITEMDEF *pstDef)
{
	//if (	pstDef->WearPos1 == WEAR_POS_HEAD ||
		//pstDef->WearPos1 == WEAR_POS_FACE)
	if(pstDef->WearPos1 == WEAR_POS_HEAD)
	{
		return 1;
	}
	return 0;
}

//lzk
int arm_is_face(ITEMDEF *pstDef)
{
	if (pstDef->WearPos1 == WEAR_POS_FACE)
	{
		return 1;
	}
	return 0;
}

int arm_is_avatar(ITEMDEF *pstDef)
{
	if (	pstDef->WearPos1 <= WEAR_POS_NO ||
		pstDef->WearPos1 > WEAR_POS_MAX) 
		return 0;

	if (arm_is_head(pstDef)) 
		return 0;

	if (arm_is_back(pstDef))
		return 0;

	if(arm_is_face(pstDef))
		return 0;
	
	if (	pstDef->WearPos1 >= WEAR_POS_FASHION_BODY&& 
		pstDef->WearPos1 <= WEAR_POS_FASHION_LEG3)
		return 0;
	return 1;
}

int wear_is_normal_fashion1(int iWearPos)
{
	if (	iWearPos == WEAR_POS_FASHION_HAIR ||
		iWearPos == WEAR_POS_FASHION_HEAD||
		iWearPos == WEAR_POS_FASHION_BODY ||
		iWearPos == WEAR_POS_FASHION_SHOES ||
		iWearPos == WEAR_POS_FASHION_HAND ||
		iWearPos == WEAR_POS_FASHION_LEG )
	{
		return 1;
	}
	return 0;
}


int wear_is_normal_fashion2(int iWearPos)
{
	if (	iWearPos == WEAR_POS_FASHION_HAIR2 ||
		iWearPos == WEAR_POS_FASHION_HEAD2||
		iWearPos == WEAR_POS_FASHION_BODY2 ||
		iWearPos == WEAR_POS_FASHION_SHOES2 ||
		iWearPos == WEAR_POS_FASHION_HAND2 ||
		iWearPos == WEAR_POS_FASHION_LEG2 )
	{
		return 1;
	}
	return 0;
}

int wear_is_normal_fashion3(int iWearPos)
{
	if (	iWearPos == WEAR_POS_FASHION_HAIR3 ||
		iWearPos == WEAR_POS_FASHION_HEAD3||
		iWearPos == WEAR_POS_FASHION_BODY3 ||
		iWearPos == WEAR_POS_FASHION_SHOES3 ||
		iWearPos == WEAR_POS_FASHION_HAND3 ||
		iWearPos == WEAR_POS_FASHION_LEG3 )
	{
		return 1;
	}
	return 0;
}

int arm_is_normal_fashion(ITEMDEF *pstDef)
{
	if (pstDef->WearPos1 == WEAR_POS_FASHION_HAIR ||
		pstDef->WearPos1 == WEAR_POS_FASHION_HEAD||
		pstDef->WearPos1 == WEAR_POS_FASHION_BODY ||
		pstDef->WearPos1 == WEAR_POS_FASHION_SHOES ||
		pstDef->WearPos1 == WEAR_POS_FASHION_HAND ||
		pstDef->WearPos1 == WEAR_POS_FASHION_LEG )
	{
		return -1;
	}
	
	return 0;
}

int arm_is_fashion(ITEMDEF *pstDef)
{
	if (pstDef->WearPos1 == WEAR_POS_FASHION_HAIR ||
		pstDef->WearPos1 == WEAR_POS_FASHION_HEAD||
		pstDef->WearPos1 == WEAR_POS_FASHION_BACK||
		pstDef->WearPos1 == WEAR_POS_FASHION_BODY ||
		pstDef->WearPos1 == WEAR_POS_FASHION_SHOES ||
		pstDef->WearPos1 == WEAR_POS_FASHION_HAND ||
		pstDef->WearPos1 == WEAR_POS_FASHION_LEG )
	{
		return -1;
	}
	
	return 0;
}

int arm_is_back1(int iWearPos)
{
	if (iWearPos == WEAR_POS_FASHION_BACK)
	{
		return 1;
	}

	return 0;
}

int arm_is_back2(int iWearPos)
{
	if (iWearPos == WEAR_POS_FASHION_BACK2)
	{
		return 1;
	}

	return 0;
}

int arm_is_back3(int iWearPos)
{
	if (iWearPos == WEAR_POS_FASHION_BACK3)
	{
		return 1;
	}

	return 0;
}

// 装备坑位是不会有背部装备了
int arm_is_back(ITEMDEF *pstDef)
{
	if(pstDef->WearPos1 == WEAR_POS_FASHION_BACK)
	{
		return 1;
	}

	return 0;
}

int is_valid_normal_fashion(Player *pstPlayer, int iWearPos)
{
	ROLEAVATARPROF *pstRoleAvatar = &pstPlayer->stRoleData.AvatarProf;

	if(pstRoleAvatar->SelectFashion == 1)
	{
		if(wear_is_normal_fashion2(iWearPos))
		{
			return 1;
		}
	}
	else if(pstRoleAvatar->SelectFashion == 2)
	{
		if(wear_is_normal_fashion3(iWearPos))
		{
			return 1;
		}
	}
	else
	{
		if(wear_is_normal_fashion1(iWearPos))
		{
			return 1;
		}
	}
	
	return 0;
}

int is_valid_back(Player *pstPlayer, int iWearPos)
{
	ROLEAVATARPROF *pstRoleAvatar = &pstPlayer->stRoleData.AvatarProf;

	if(pstRoleAvatar->SelectFashion == 1)
	{
		if(arm_is_back2(iWearPos))
		{
			return 1;
		}
	}
	else if(pstRoleAvatar->SelectFashion == 2)
	{
		if(arm_is_back3(iWearPos))
		{
			return 1;
		}
	}
	else
	{
		if(arm_is_back1(iWearPos))
		{
			return 1;
		}
	}
	
	return 0;
}

int z_birth_avatar(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,
			ROLEAVATARPROF *pstAvatar, unsigned char ucCareer, unsigned char ucGender,int iApperalIdx)
{
	int i;
	int iRet = 0;
	ROLEGRID stRoleGrid;
	ARMPROF *pstArmProf;
	ITEMDEF *pstItemDef;
	BIRTHITEMDEF *pstBirthItem;
	ROLEWEAR stRoleWear;
	int iFirstApperalID = 0;	//第一件衣服ItemID
	int iIdx = -1;
	int iFind = 0;

	memset(&stRoleWear, 0, sizeof(stRoleWear));
	
	pstBirthItem = birthitem_def_find(pstEnv, ucCareer, ucGender);
	if (!pstBirthItem ||iApperalIdx < 0)
	{
		return -1;
	}

	//记录下新角色将要穿着的
	for(i = 0; i< MAX_BIRTH_ITEM; i++)
	{
		if (pstBirthItem->Items[i].ItemID == 0)
			{break;}
		
		pstItemDef = z_find_itemdef(pstEnv, pstBirthItem->Items[i].ItemID);
		if (!pstItemDef)
		{
			continue;
		}

		if (pstItemDef->ItemType != ITEM_TYPE_ARM)
		{
			continue;
		}		

		if (pstItemDef->WearPos1 == WEAR_POS_RING || pstItemDef->WearPos1 == WEAR_POS_AMULET)
		{
			continue;
		}

		
		if (	pstItemDef->WearPos1 == WEAR_POS_BODY ||
			pstItemDef->WearPos2 == WEAR_POS_BODY ||
			pstItemDef->WearPos3 == WEAR_POS_BODY)
		{
			iIdx++;
			if (iIdx != iApperalIdx)
			{
				if (iFirstApperalID == 0)
					iFirstApperalID = 	pstItemDef->ItemID;
				continue;
			}
			else
			{
				iFind = 1;
			}
		}	
	
		if (0 > item_create(pstAppCtx, pstEnv, &stRoleGrid, pstItemDef->ItemID))
		{
			continue;
		}
		
		iRet = z_player_wear_pos_check(pstEnv, &stRoleWear, pstItemDef);
		if ( iRet < 0)
		{	
			continue;
		}

		memcpy(&stRoleWear.RoleArms[stRoleWear.ArmNum], &stRoleGrid.GridData.RoleArm, sizeof(stRoleWear.RoleArms[0]));
		stRoleWear.RoleArms[stRoleWear.ArmNum].GridIdx = pstItemDef->WearPos1;
		stRoleWear.ArmNum++;
	}
	
	if (!iFind && iFirstApperalID > 0)
	{
		if (item_create(pstAppCtx, pstEnv, &stRoleGrid, iFirstApperalID) == 0)
		{
			pstItemDef = z_find_itemdef(pstEnv, iFirstApperalID);
			if (	pstItemDef &&
				z_player_wear_pos_check(pstEnv, &stRoleWear, pstItemDef) == 0)
			{
				memcpy(&stRoleWear.RoleArms[stRoleWear.ArmNum], &stRoleGrid.GridData.RoleArm, sizeof(stRoleWear.RoleArms[0]));
				stRoleWear.RoleArms[stRoleWear.ArmNum].GridIdx = pstItemDef->WearPos1;
				stRoleWear.ArmNum++;
			}
		}
	}
		
	pstAvatar->Num = 0;
	for (i=0; i<stRoleWear.ArmNum; i++)
	{
		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)(stRoleWear.RoleArms+i));
		if (NULL == pstItemDef)
		{
			continue;
		}
		
		if( !arm_is_avatar(pstItemDef))
		{
			continue;
		}
		
		if (pstAvatar->Num >= MAX_AVATAR)
		{
			break;
		}
			
		pstArmProf = &pstAvatar->Arms[pstAvatar->Num];
		pstArmProf->ArmDefID = stRoleWear.RoleArms[i].DefID;
		pstArmProf->GridIdx = stRoleWear.RoleArms[i].GridIdx;

		pstAvatar->Num++;
	}

	return 0;
}

int ride_drop(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSRIDEDROP *pstRideDrop)
{
	int iIdx;
	int iOpRideLevel;
	ROLERIDELIST   *pstRideList = &pstPlayer->stRoleData.MiscInfo.RideList;
	RIDEDEF *pstRideDef;
	UNUSED(pstAppCtx);

	iIdx = z_get_package_ride(pstPlayer,  pstRideDrop->RideItemID);
	if (iIdx < 0)
	{
		return -1;
	}

	if (player_check_safelock(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}
	
	if( ((CS_STAT_RIDE_ON & pstPlayer->stOnline.State) ||  (CS_STAT_FLY & pstPlayer->stOnline.State)) && 
		pstPlayer->stOnline.RideItemID == (int)pstRideDrop->RideItemID)
	{
		//z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK163);
		return -1;
	}
	
	pstRideDef = z_find_ride_def(pstEnv, pstRideList->RideGrids[iIdx].ItemID, pstRideList->RideGrids[iIdx].Level);
	if(pstRideDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK164, pstRideDef->RideName);
	}

	iOpRideLevel = pstRideList->RideGrids[iIdx].Level;
	
	pstRideList->Num--;
	if (iIdx != pstRideList->Num)
	{
		memmove(&pstRideList->RideGrids[iIdx], &pstRideList->RideGrids[iIdx+1], (pstRideList->Num - iIdx)*sizeof(RIDEGRID));
	}

	ride_list_chg(pstEnv, pstPlayer);

	//运营日志
	z_role_ride_oplog(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRideDrop->RideItemID, iOpRideLevel, OP_RIDE_DEL);
	
	return 0;
}

int z_hitch_ride(ZONESVRENV* pstEnv, Player *pstRidePlayer, Player *pstPlayer)
{
	CSPKG stPkgRes;
	CSACTIONRES *pstActionRes = &stPkgRes.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_HITCH_RIDE;

	pstActionRes->Data.HitchRide.OwnerID = pstRidePlayer->stOnline.stRideOnInfo.iOwnerID;
	pstActionRes->Data.HitchRide.RideID = pstRidePlayer->stOnline.RideItemID;
	pstActionRes->Data.HitchRide.RideLevel = pstRidePlayer->stOnline.RideLevel;
	pstActionRes->Data.HitchRide.PassengerNum = pstRidePlayer->stOnline.stRideOnInfo.iPassengerNum;
	pstActionRes->Data.HitchRide.Pos = pstPlayer->stRoleData.Pos;
	memcpy(&pstActionRes->Data.HitchRide.PassengerID[0], 
				&pstRidePlayer->stOnline.stRideOnInfo.PassengerID[0],
				sizeof(pstActionRes->Data.HitchRide.PassengerID));
	
	Z_CSHEAD_INIT(&stPkgRes.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkgRes, 0);
	
	return 0;
}

int z_leave_ride(ZONESVRENV* pstEnv, Player *pstRidePlayer, Player *pstPlayer)
{
	CSPKG stPkgRes;
	CSACTIONRES *pstActionRes = &stPkgRes.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_LEAVE_RIDE;

	pstActionRes->Data.LeaveRide.OwnerID = pstRidePlayer->stOnline.stRideOnInfo.iOwnerID;
	pstActionRes->Data.LeaveRide.RideID = pstRidePlayer->stOnline.RideItemID;
	pstActionRes->Data.LeaveRide.RideLevel = pstRidePlayer->stOnline.RideLevel;
	pstActionRes->Data.LeaveRide.PassengerNum = pstRidePlayer->stOnline.stRideOnInfo.iPassengerNum;
	pstActionRes->Data.HitchRide.Pos = pstPlayer->stRoleData.Pos;
	memcpy(&pstActionRes->Data.LeaveRide.PassengerID[0], 
				&pstRidePlayer->stOnline.stRideOnInfo.PassengerID[0],
				sizeof(pstActionRes->Data.LeaveRide.PassengerID));
	
	Z_CSHEAD_INIT(&stPkgRes.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkgRes, 0);
	
	return 0;
}

int player_ride_broadcast(ZONESVRENV* pstEnv, Player *pstRidePlayer, CSPKG *pstPkg)
{
	int i;
	Player *pstTmpPlayer;
	Player *apstPlayer[MAX_RIDE_PASSENGER];
	int iPlayerNum = 0;

	if (!(pstRidePlayer->stOnline.State & (CS_STAT_RIDE_ON |CS_STAT_FLY)))
	{
		return -1;
	}

	iPlayerNum = 0;
	apstPlayer[iPlayerNum++] = pstRidePlayer;
	for(i=0; i<pstRidePlayer->stOnline.stRideOnInfo.iPassengerNum; i++)
	{
		pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstRidePlayer->stOnline.stRideOnInfo.PassengerID[i]);
		if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}

		apstPlayer[iPlayerNum++] = pstTmpPlayer;
	}

	z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iPlayerNum, pstPkg);
	
	return 0;
}

int z_send_msg_to_ride_member(ZONESVRENV* pstEnv, Player *pstRidePlayer, char *pszMsg)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;

	pstSysMsg->Type = SYS_MIDDLE;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
	STRNCPY(pstSysMsg->Msg, pszMsg, CCH(pstSysMsg->Msg));

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);

	return player_ride_broadcast(pstEnv, pstRidePlayer, pstPkg);
}

int together_move_info(Player *pstPlayer, Player *pstRidePlayer)
{
	int i;
	
	//主人移动信息复制
	pstPlayer->stOnline.cMoving = pstRidePlayer->stOnline.cMoving;
	pstPlayer->stOnline.cCurrPoint = pstRidePlayer->stOnline.cCurrPoint;
	pstPlayer->stOnline.cFuturePoints = pstRidePlayer->stOnline.cFuturePoints;
	pstPlayer->stOnline.cLastPath = pstRidePlayer->stOnline.cLastPath;
	pstPlayer->stOnline.cLastPathPoints = pstRidePlayer->stOnline.cLastPathPoints;
	pstPlayer->stOnline.cWalkPath = pstRidePlayer->stOnline.cWalkPath;

	for(i=0; i<RECORD_TURN; i++)
	{
		pstPlayer->stOnline.stTurnPoints[i] = pstRidePlayer->stOnline.stTurnPoints[i];
	}
	
	pstPlayer->stOnline.stLastMoveTv = pstRidePlayer->stOnline.stLastMoveTv;
	pstPlayer->stOnline.stLastCltMoveTime = pstRidePlayer->stOnline.stLastCltMoveTime;
	pstPlayer->stOnline.stLastCltTime = pstRidePlayer->stOnline.stLastCltTime;
	pstPlayer->stOnline.stLastCltStopTime = pstRidePlayer->stOnline.stLastCltStopTime;

	return 0;
}

int player_do_hitch_ride(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstRidePlayer)
{
	char szName[ROLE_NAME_LEN];
	char szMsg[256];
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;
	RESPOS stPos;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	
	stIdx.iID = pstRidePlayer->stRoleData.Map;
	stIdx.iPos = pstRidePlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst)
	{
		return -1;
	}
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx)
	{
		return -1;
	}
	
	//骑马的先下马
	if( (CS_STAT_RIDE_ON | CS_STAT_FLY | CS_STAT_RIDE_PREPARE) & pstPlayer->stOnline.State)
	{	
		z_ride_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	//清除临时道具
	if(pstRoleTmpSys->TmpItemID)
	{
		tmp_item_drop(pstEnv, pstPlayer,1);
	}

	//停下来在上马
	z_player_stop(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, NULL);
	
	//上马咯
	pstPlayer->stOnline.State |= CS_STAT_HITCH_RIDE;
	pstPlayer->stOnline.stRideOnInfo.iOwnerID = pstRidePlayer->iMemID;
	pstPlayer->stOnline.RideItemID = pstRidePlayer->stOnline.RideItemID;
	pstPlayer->stOnline.RideLevel = pstRidePlayer->stOnline.RideLevel;

	memcpy(&stPos, &pstPlayer->stRoleData.Pos, sizeof(stPos));
	pstPlayer->stRoleData.Pos = pstRidePlayer->stRoleData.Pos;
	z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstPlayer->iMemID, &stPos, 0);

	//路径信息同步
	together_move_info(pstPlayer, pstRidePlayer);
	
	//改变速度
	pstPlayer->stOnline.stMspdInfo = pstRidePlayer->stOnline.stMspdInfo;

	//找 座位
	pstRidePlayer->stOnline.stRideOnInfo.PassengerID[pstRidePlayer->stOnline.stRideOnInfo.iPassengerNum++] = pstPlayer->iMemID;

	//通知大家
	z_hitch_ride(pstEnv, pstRidePlayer, pstPlayer);

	//提示
	STRNCPY(szName, pstPlayer->stRoleData.RoleName, CCH(szName));
	battle_rolename_remove_prefix(szName);
	
	sprintf(szMsg, LJY_SYSMSG21, szName);
	z_send_msg_to_ride_member(pstEnv, pstRidePlayer, szMsg);
		
	if(pstRidePlayer->stOnline.cMoving)
	{
		z_player_move_notify_one(pstEnv->pstAppCtx, pstEnv, pstRidePlayer, pstPlayer);
	}

	return 0;
}

//申请上马
int ride_apply(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSRIDEAPPLY *pstRideApply)
{
	Player *pstRidePlayer;
	RIDEDEF *pstRideDef;
	CSPKG stPkg;
	CSRIDESVR *pstRideSvr = &stPkg.Body.RideSvr;

	//已经搭车的人不管
	if(CS_STAT_HITCH_RIDE & pstPlayer->stOnline.State)
	{
		return -1;
	}

	pstRidePlayer = z_id_player(pstAppCtx, pstEnv, pstRideApply->RideOwnerID);
	if (pstRidePlayer == NULL || pstRidePlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		//对方不在线
		return -1;
	}

	if (pstRidePlayer->stRoleData.Map != pstPlayer->stRoleData.Map)
	{
		//不在一个地图
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK165);
		return -1;
	}
 
	if(0 == player_team_same(pstEnv, pstPlayer, pstRidePlayer))
	{
		//不在同一个队伍中
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK166);
		return -1;
	}

	if(!( (CS_STAT_RIDE_ON | CS_STAT_FLY) & pstRidePlayer->stOnline.State))
	{
		//对方未骑乘
		return -1;
	}

	if (pstRidePlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstRidePlayer, 0);
	}

	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	}

	//上马距离
	if(z_distance(&pstRidePlayer->stRoleData.Pos, &pstPlayer->stRoleData.Pos) > MAX_HITCH_RIDE_AREA)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK165);
		return -1;
	}

	pstRideDef = z_find_ride_def(pstEnv, pstRidePlayer->stOnline.RideItemID, pstRidePlayer->stOnline.RideLevel);
	if(pstRideDef == NULL || pstRideDef->RideOnNum == 0 )
	{
		//不可乘坐
		return -1;
	}

	if( pstRideDef->RideOnNum <= pstRidePlayer->stOnline.stRideOnInfo.iPassengerNum)
	{
		//满人
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK167);
		return -1;
	}

	pstRideSvr->Type = RIDE_SVR_ACCEPT;
	pstRideSvr->Data.ApplyInfo.MemID = pstPlayer->iMemID;
	STRNCPY(pstRideSvr->Data.ApplyInfo.RoleName, pstPlayer->stRoleData.RoleName, CCH(pstRideSvr->Data.ApplyInfo.RoleName));

	Z_CSHEAD_INIT(&stPkg.Head, RIDE_SVR);
	z_cltmsg_send(pstEnv, pstRidePlayer, &stPkg);

	return 0;
}

//接受上马申请
int ride_apply_accpet(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSRIDEAPPLYACCPET *pstRideApplyAccpet)
{
	Player *pstAccpetPlayer;
	RIDEDEF *pstRideDef;
	
	pstAccpetPlayer = z_id_player(pstAppCtx, pstEnv, pstRideApplyAccpet->RideApplyID);
	if (pstAccpetPlayer == NULL || pstAccpetPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		//对方不在线
		return -1;
	}

	//已经搭车的人不管
	if(CS_STAT_HITCH_RIDE & pstAccpetPlayer->stOnline.State)
	{
		return -1;
	}

	if (pstAccpetPlayer->stRoleData.Map != pstPlayer->stRoleData.Map)
	{
		//不在一个地图
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK165);
		return -1;
	}
	
	if(0 == player_team_same(pstEnv, pstPlayer, pstAccpetPlayer))
	{
		//不在同一个队伍中
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK168);
		return -1;
	}

	if(!( (CS_STAT_RIDE_ON | CS_STAT_FLY) & pstPlayer->stOnline.State))
	{
		//自己都没上马
		return -1;
	}

	pstRideDef = z_find_ride_def(pstEnv, pstPlayer->stOnline.RideItemID, pstPlayer->stOnline.RideLevel);
	if(pstRideDef == NULL || pstRideDef->RideOnNum == 0 )
	{
		//不可乘坐
		return -1;
	}

	//满人
	if( pstRideDef->RideOnNum <= pstPlayer->stOnline.stRideOnInfo.iPassengerNum)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK167);
		return -1;
	}

	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	}
	
	//上马距离
	if(z_distance(&pstAccpetPlayer->stRoleData.Pos, &pstPlayer->stRoleData.Pos) > MAX_HITCH_RIDE_AREA)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK165);
		return -1;
	}
	player_drop_clan_tem(pstEnv,pstAccpetPlayer);
	//上马
	player_do_hitch_ride(pstEnv, pstAccpetPlayer, pstPlayer);
	
	return 0;
}

int ride_invite(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSRIDEINVITE *pstRideInvite)
{
	int i;
	Player *pstTmpPlayer;
	ZoneTeam *pstTeam;
	RIDEDEF *pstRideDef;
	TEAMMEMBER *pstTeamMember;
	ROLETMPSYS *pstRoleTmpSys;
	CSPKG stPkg;
	CSRIDESVR *pstRideSvr = &stPkg.Body.RideSvr;

	//自己都没上马
	if(!( (CS_STAT_RIDE_ON | CS_STAT_FLY) & pstPlayer->stOnline.State))
	{
		return -1;
	}

	//单人的邀请不了别人
	pstRideDef = z_find_ride_def(pstEnv, pstPlayer->stOnline.RideItemID, pstPlayer->stOnline.RideLevel);
	if(pstRideDef == NULL || pstRideDef->RideOnNum == 0 )
	{
		return -1;
	}

	if( pstRideDef->RideOnNum <= pstPlayer->stOnline.stRideOnInfo.iPassengerNum)
	{
		//满人了撒
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK167);
		return -1;
	}

	pstTeam = player_team_get(pstEnv, pstPlayer);
	if (pstTeam == NULL)
	{
		if(pstPlayer->stOnline.eStatusAssistant & PLAYER_STATUS_BATTLE_LOGIN_READY)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
				ZONEERR_BATTLE_SVR_OP);
		}
		return -1;
	}

	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	}

	//全队邀请
	if (pstRideInvite->RideInviteID == 0)
	{
		for(i = 0; i<pstTeam->stTeamInfo.MemberNum; i++)
		{
			pstTeamMember = &pstTeam->stTeamInfo.TeamMember[i];
			pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeamMember->MemberName);
			if (pstTmpPlayer == NULL || pstPlayer == pstTmpPlayer)
			{
				continue;
			}

			//已经搭车的人不管
			if(CS_STAT_HITCH_RIDE & pstTmpPlayer->stOnline.State)
			{
				continue;
			}

			pstRoleTmpSys = &pstTmpPlayer->stRoleData.MiscInfo.TmpSys;
			if(pstRoleTmpSys->TmpItemID > 0)
			{
				ITEMDEF *pstItemDef;
				
				pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
				if (pstItemDef && ITEM_TMP_MACHINE == pstItemDef->ItemTmp.Type)
				{
					continue;
				}
			}

			if (pstTmpPlayer->stOnline.cMoving)
			{
				z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstTmpPlayer, 0);
			}

			if (pstTmpPlayer->stRoleData.Map != pstPlayer->stRoleData.Map)
			{
				continue;
			}
			
			//距离太远邀请不了
			if(z_distance(&pstTmpPlayer->stRoleData.Pos, &pstPlayer->stRoleData.Pos) > MAX_HITCH_RIDE_AREA)
			{
				continue;
			}

			pstRideSvr->Type = RIDE_SVR_INVITE;
			pstRideSvr->Data.InviteInfo.MemID = pstPlayer->iMemID;
			STRNCPY(pstRideSvr->Data.InviteInfo.RoleName, pstPlayer->stRoleData.RoleName, CCH(pstRideSvr->Data.InviteInfo.RoleName));

			Z_CSHEAD_INIT(&stPkg.Head, RIDE_SVR);
			z_cltmsg_send(pstEnv, pstTmpPlayer, &stPkg);
		}
	}
	else
	{
		pstTmpPlayer  = z_id_player(pstAppCtx, pstEnv, pstRideInvite->RideInviteID);
		if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			//对方不在线
			return -1;
		}

		//不在同一个队伍中
		if(0 == player_team_same(pstEnv, pstPlayer, pstTmpPlayer))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK169);
			return -1;
		}

		//已经搭车的人不管
		if(CS_STAT_HITCH_RIDE & pstTmpPlayer->stOnline.State)
		{
			return -1;
		}
		
		if (pstTmpPlayer->stOnline.cMoving)
		{
			z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstTmpPlayer, 0);
		}

		pstRoleTmpSys = &pstTmpPlayer->stRoleData.MiscInfo.TmpSys;
		if(pstRoleTmpSys->TmpItemID > 0)
		{
			ITEMDEF *pstItemDef;
			
			pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
			if (pstItemDef && ITEM_TMP_MACHINE == pstItemDef->ItemTmp.Type)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK170);
				return -1;
			}
		}

		if (pstTmpPlayer->stRoleData.Map != pstPlayer->stRoleData.Map)
		{
			//不在一个地图
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK165);
			return -1;
		}

		//距离太远邀请不了
		if(z_distance(&pstTmpPlayer->stRoleData.Pos, &pstPlayer->stRoleData.Pos) > MAX_HITCH_RIDE_AREA)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK165);
			return -1;
		}

		pstRideSvr->Type = RIDE_SVR_INVITE;
		pstRideSvr->Data.InviteInfo.MemID = pstPlayer->iMemID;
		STRNCPY(pstRideSvr->Data.InviteInfo.RoleName, pstPlayer->stRoleData.RoleName, CCH(pstRideSvr->Data.InviteInfo.RoleName));
		battle_rolename_remove_prefix(pstRideSvr->Data.InviteInfo.RoleName);
		
		Z_CSHEAD_INIT(&stPkg.Head, RIDE_SVR);
		z_cltmsg_send(pstEnv, pstTmpPlayer, &stPkg);
	}
	
	return 0;
}

int ride_invite_accpet(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSRIDEINVITEACCPET *pstRideInviteAccpet)
{
	Player *pstRidePlayer;
	RIDEDEF *pstRideDef;
	ROLETMPSYS *pstRoleTmpSys;

	pstRidePlayer = z_id_player(pstAppCtx, pstEnv, pstRideInviteAccpet->RideOwnerID);
	if (pstRidePlayer == NULL || pstRidePlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		//对方不在线
		return -1;
	}

	//已经搭车的人不管
	if(CS_STAT_HITCH_RIDE & pstPlayer->stOnline.State)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK171);
		return -1;
	}

	pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;
	if(pstRoleTmpSys->TmpItemID > 0)
	{
		ITEMDEF *pstItemDef;
		
		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
		if (pstItemDef && ITEM_TMP_MACHINE == pstItemDef->ItemTmp.Type)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK172);
			return -1;
		}
	}

	//不在一个地图
	if (pstRidePlayer->stRoleData.Map != pstPlayer->stRoleData.Map)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK165);
		return -1;
	}

	//不在同一个队伍中
	if(0 == player_team_same(pstEnv, pstPlayer, pstRidePlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK173);
		return -1;
	}

	//对方未骑乘
	if(!( (CS_STAT_RIDE_ON | CS_STAT_FLY) & pstRidePlayer->stOnline.State))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK174);
		return -1;
	}

	//不可乘坐
	pstRideDef = z_find_ride_def(pstEnv, pstRidePlayer->stOnline.RideItemID, pstRidePlayer->stOnline.RideLevel);
	if(pstRideDef == NULL || pstRideDef->RideOnNum == 0 )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK175);
		return -1;
	}

	//满人
	if( pstRideDef->RideOnNum <= pstRidePlayer->stOnline.stRideOnInfo.iPassengerNum)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK167);
		return -1;
	}

	if (pstRidePlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstRidePlayer, 0);
	}
	
	//上马距离
	if(z_distance(&pstRidePlayer->stRoleData.Pos, &pstPlayer->stRoleData.Pos) > MAX_HITCH_RIDE_AREA)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PAK176);
		return -1;
	}

	player_drop_clan_tem(pstEnv,pstPlayer);
	player_do_hitch_ride(pstEnv, pstPlayer, pstRidePlayer);
	
	return 0;
}

NPCSELLLIST *z_get_sell_list_byid(ZONESVRENV* pstEnv, int iSellListID)
{
	NPCSELLLIST stSellList;
	int iEqu;
	int iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;


	stSellList.SellListID = iSellListID;
	iIndex = bsearch_int(&stSellList, pstObj->sSellDef, pstObj->iSellDef, SELL_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (NPCSELLLIST *)pstObj->sSellDef[iIndex];
	}

	return NULL;
}

RIDENPCSELECT *z_get_ride_npc(RIDEDEF *pstRideDef, int iSelectType)
{
	int i;

	for(i = 0; i< MAX_RIDE_NPC_SELECT; i++)
	{
		if (pstRideDef->NpcSelect[i].SelectType == iSelectType)
		{
			return &pstRideDef->NpcSelect[i];
		}
	}

	return NULL;
}

int open_shop_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, int iSellListID, int iVip)
{
	int i;
	SHOPLIST *pstShopList;
	NPCSELLLIST *pstSellList;
	CSPKG stPkg;
	CSSHOPSVR *pstShopSvr = &stPkg.Body.ShopSvr;
	
	pstSellList = z_get_sell_list_byid(pstEnv, iSellListID);
	if (NULL == pstSellList)
	{
		return -1;
	}

	memset(pstShopSvr, 0, sizeof(*pstShopSvr));

	pstShopSvr->OpType = SHOP_LIST;
	pstShopList = &pstShopSvr->ShopSvrData.ShopList;
	pstShopList->ConditionNum = 0;
		
	for (i=0; i<(int)pstSellList->Count; i++)
	{
		pstShopList->ShopSellItems[i].GoodsID = pstSellList->Goods[i].GoodsID;
		pstShopList->ShopSellItems[i].GoodsType = pstSellList->Goods[i].GoodsType;
		
		if (pstSellList->Goods[i].Condition.Type > 0)
		{
			pstShopList->Conditions[pstShopList->ConditionNum].GridIdx = i;
			pstShopList->Conditions[pstShopList->ConditionNum].Condition = pstSellList->Goods[i].Condition;
			
			pstShopList->ConditionNum++;
		}

		if (0 != pstSellList->Goods[i].SpecialMoneyType)
		{
			pstShopList->ShopSellItems[i].Price = pstSellList->Goods[i].SpecialMoneyNum;
			pstShopList->ShopSellItems[i].SpecialMoneyType = pstSellList->Goods[i].SpecialMoneyType;
		}
		else
		{
			switch(pstShopList->ShopSellItems[i].GoodsType)
			{
			case SHOP_ITEM_ITEM:
				{
					const ITEMDEF *pstDef = z_find_itemdef(stSEnv.pstEnv,
					                         pstShopList->ShopSellItems[i].GoodsID);
					if (!pstDef)
					{
						return -1;
					}

					if (pstDef->ClanContri <= 0)
					{
						pstShopList->ShopSellItems[i].Price = pstDef->BuyPrice;
					}
					else
					{
						pstShopList->ShopSellItems[i].Price = pstDef->ClanContri;
					}
				}
				break;

			case SHOP_ITEM_CLAN_ITEM:
				{
					const CLANPRODUCTDEF *pstDef = clan_product_def(stSEnv.pstEnv,
					                         pstShopList->ShopSellItems[i].GoodsID);
					if (!pstDef)
					{
						return -1;
					}
					pstShopList->ShopSellItems[i].Price = pstDef->BuyPrice;
				}
				break;

			default:
				return -1;
				break;
			}
		}
	}

	if (iVip)
	{
		pstShopList->NpcID = -1;
	}
	else
	{
		pstShopList->NpcID = 0;
	}
	pstShopList->SellListID = pstSellList->SellListID;
	pstShopList->ShopItemNum = pstSellList->Count;
	selled_del(pstEnv, &pstPlayer->stRoleData.MiscInfo.ItemSellSave);
	memcpy(&pstShopList->ItemSellSave, &pstPlayer->stRoleData.MiscInfo.ItemSellSave, sizeof(pstShopList->ItemSellSave));
	player_cacl_rebate(pstEnv, pstPlayer, &pstShopList->RebateCount,
						&pstShopList->RebateTypeNum, pstShopList->RebateTypes);

	Z_CSHEAD_INIT(&stPkg.Head, SHOP_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int ride_npc_select(ZONESVRENV* pstEnv, Player *pstPlayer, CSRIDENPCSELECT *pstNpcSelect)
{
	RIDEDEF *pstRideDef;
	RIDENPCSELECT *pstRideNpc;

	if(!( (CS_STAT_RIDE_ON | CS_STAT_FLY) & pstPlayer->stOnline.State))
	{	
		return -1;
	}
	
	pstRideDef = z_find_ride_def(pstEnv, pstPlayer->stOnline.RideItemID, pstPlayer->stOnline.RideLevel);
	if(pstRideDef == NULL)
	{
		return -1;
	}

	pstRideNpc = z_get_ride_npc(pstRideDef, pstNpcSelect->SelectType);
	if(pstRideNpc == NULL)
	{
		return -1;
	}
		
	switch(pstNpcSelect->SelectType)
	{
		case RIDE_OPEN_SHOP:
			return open_shop_to_clt(pstEnv, pstPlayer, pstRideNpc->SelectVal, 0);
			break;
		case RIDE_OPEN_STASH:
			return open_stash(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
			break;
		default:
			break;
	}

	return 0;
}

int ride_level_up_check(ZONESVRENV* pstEnv, Player *pstPlayer, CSRIDELEVELUP *pstRideLevelUp, int *piPos, RIDEDEF *pstRideDef, ITEMDEF *pstItemDef)
{
	int i;
	
	for (i = 0; i < MAX_RIDE_STUFF; i++)
	{	
		if (pstRideDef->LevelUpStuff[i].ItemID == 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK177);
			return -1;
		}

		
		if(pstItemDef->ItemID == pstRideDef->LevelUpStuff[i].ItemID)
		{
			if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRideDef->LevelUpStuff[i].ItemID, 1, pstRideLevelUp->ListType, piPos,PACKAGE_DEC_NONE))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK177);
				return -1;
			}
			break;
		}
	}

	if (0 > package_dec_money_test(pstEnv, pstPlayer, pstRideDef->MoneyType, pstRideDef->MoneyVal))
	{
		char szBuff[32];
			
		if (money_type_to_string(pstRideDef->MoneyType, szBuff, sizeof(szBuff)) < 0 )
		{
			strcpy(szBuff, "金钱");
		}

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK178, szBuff);
		return -1;
	}

	return 0;
}

int ride_level_up(ZONESVRENV* pstEnv, Player *pstPlayer, CSRIDELEVELUP *pstRideLevelUp)
{
	int iIdx;
	RIDEDEF *pstRideDef;
	RIDEPINZHIDEF *pstRidePinZhiDef;
	ROLERIDELIST   *pstRideList = &pstPlayer->stRoleData.MiscInfo.RideList;
	ITEMDEF *pstItemDef;
	ROLEITEM *pstRoleItem;
	int iPos = -1, iMust = 0;
	int iRet;
	
			
	//if( ( (CS_STAT_RIDE_ON |CS_STAT_FLY) & pstPlayer->stOnline.State) && 
	//	pstPlayer->stOnline.RideItemID == pstRideLevelUp->RideID)
	//{	
	//	//先下马
	//	z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "不可强化正在骑乘的坐骑");
	//	return -1;
	//}

	iIdx = z_get_package_ride(pstPlayer, pstRideLevelUp->RideID);
	if (iIdx < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK179);
		return -1;
	}

	pstRideDef = z_find_ride_def(pstEnv, pstRideList->RideGrids[iIdx].ItemID, pstRideList->RideGrids[iIdx].Level+1);
	if(pstRideDef == NULL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK180);
		return -1;
	}
	
	pstRideDef = z_find_ride_def(pstEnv, pstRideList->RideGrids[iIdx].ItemID, pstRideList->RideGrids[iIdx].Level);
	if(pstRideDef == NULL)
	{
		return -1;
	}

	pstRidePinZhiDef = z_find_ride_pinzhi_def(pstEnv, pstRideDef->PinZhi);
	if(!pstRidePinZhiDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK180);
		return -1;
	}

	if(pstRideList->RideGrids[iIdx].Level >= pstRidePinZhiDef->MaxLevel)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK180);
		return -1;
	}

	if (0 > package_get_item(pstEnv, pstPlayer, pstRideLevelUp->ListType, 
						pstRideLevelUp->GridIdx, &pstItemDef, &pstRoleItem, &iPos))
	{
		return -1;
	}

	if( 0 > ride_level_up_check(pstEnv, pstPlayer, pstRideLevelUp, &iPos, pstRideDef, pstItemDef) )
	{
		return -1;
	}

	//减少材料
	if (0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDef->ItemID, 1, pstRideLevelUp->ListType, 
		&iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_RIDELEV))
	{
		return -1;
	}	

	if (0 > (iRet=package_dec_money(pstEnv, pstPlayer, pstRideDef->MoneyType, pstRideDef->MoneyVal)))
	{
		return -1;
	}

	if (pstRideDef->MoneyType == ATTR_ID_MONEY)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_RIDE, pstRideDef->RideID, 0, pstRideDef->MoneyVal, "坐骑升级");
	}
	else if (pstRideDef->MoneyType == ATTR_ID_BULL)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_RIDE, pstRideDef->RideID, pstRideDef->MoneyVal-iRet, iRet, "坐骑升级");
	}

	if(pstRideDef->HopeNum > 0 && pstRideList->RideGrids[iIdx].Count >= pstRideDef->HopeNum-1)
	{
		iMust = 1;
	}

	if (pstItemDef->ItemID == RIDE_ITEM_TESHU_ID)
	{
		iMust = 1;
	}
	
	if (iMust || (int )RAND1(10000) < pstRideDef->LevelUpRate)
	{
		//升级
		pstRideList->RideGrids[iIdx].Level++;
		pstRideList->RideGrids[iIdx].Count = 0;
		ride_list_chg(pstEnv, pstPlayer);	

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK181, 
							pstRideDef->RideName, pstRideList->RideGrids[iIdx].Level);

		if(pstRideList->RideGrids[iIdx].Level >= 50)
		{
			if ( pstEnv->pstConf->CountryVer != COUNTRY_VER_KOREA)
			{
				z_sys_strf_broadcast(pstEnv, SYS_CHAT, 
					ZONEERR_BROAD330,
				pstPlayer->stRoleData.RoleName, pstRideDef->RideName, pstRideList->RideGrids[iIdx].Level);
			}
		}

		task_player_trace(pstEnv, pstPlayer, TASK_OBJ_RIDE);
		task_player_trace(pstEnv, pstPlayer, TASK_OBJ_RIDE_STRENGTHEN);
		
		//运营日志
		z_role_ride_oplog(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRideList->RideGrids[iIdx].ItemID, pstRideList->RideGrids[iIdx].Level, OP_RIDE_LEVEL_UP);

		//成长之路
		{
			ACHIEVEMENTCOND stCond;

			memset(&stCond,0,sizeof(stCond));
			
			stCond.Type = COND_RIDE_SPEED_UP;
			stCond.Val1 = pstRideDef->RideSpd;

			z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
		}

		// 称号
		if (pstRideList->RideGrids[iIdx].Level  == 50)
		{
			player_desig_get(pstEnv, pstPlayer, GET_DESIG_RIDE_STREN);
		}
	}
	else
	{
		pstRideList->RideGrids[iIdx].Count++;
		
		//是否爆
		if ((int )RAND1(10000) < pstRideDef->AmerceRate*100 && 
			pstRideList->RideGrids[iIdx].Level > 1)
		{
			pstRideList->RideGrids[iIdx].Level--;
			ride_list_chg(pstEnv, pstPlayer);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PAK182, pstRideDef->RideName);
		}
	}

	if(((CS_STAT_RIDE_ON |CS_STAT_FLY) & pstPlayer->stOnline.State) &&
		pstPlayer->stOnline.RideItemID == (int)pstRideList->RideGrids[iIdx].ItemID)
	{
		CSPKG stPkg;
		CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	
		pstPlayer->stOnline.RideLevel = pstRideList->RideGrids[iIdx].Level;

		z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);

		pstActionRes->ActionID = ACTION_RIDE;
		pstActionRes->ID = pstPlayer->iMemID;
		pstActionRes->Data.Ride.RideItemID = pstPlayer->stOnline.RideItemID;
		pstActionRes->Data.Ride.RideLevel  = pstPlayer->stOnline.RideLevel;
		pstActionRes->Data.Ride.RideFlag = 1;

		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	}	
	
	return 0;
}

int ride_apply_refuse(ZONESVRENV* pstEnv, Player *pstPlayer, CSRIDEAPPLYREFUSE *pstRideApplyRefuse)
{
	Player *pstTmpPlayer;

	pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstRideApplyRefuse->RideApplyID);
	if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		//对方不在线
		return -1;
	}

	if(pstRideApplyRefuse->RefuseType == RIDE_REFUSE_CANCEL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTmpPlayer, SYS_MIDDLE2, ZONEERR_PAK183, pstPlayer->stRoleData.RoleName);
	}
	else
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTmpPlayer, SYS_MIDDLE2, ZONEERR_PAK184, pstPlayer->stRoleData.RoleName);
	}
	
	return 0;
}

int ride_invite_refuse(ZONESVRENV* pstEnv, Player *pstPlayer, CSRIDEINVITEREFUSE *pstRideInviteRefuse)
{
	Player *pstTmpPlayer;

	pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstRideInviteRefuse->RideOwnerID);
	if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		//对方不在线
		return -1;
	}

	if (pstRideInviteRefuse->RefuseType == RIDE_REFUSE_CANCEL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTmpPlayer, SYS_MIDDLE2, ZONEERR_PAK185, pstPlayer->stRoleData.RoleName);
	}
	else
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTmpPlayer, SYS_MIDDLE2, ZONEERR_PAK186, pstPlayer->stRoleData.RoleName);
	}
	
	return 0;
}

//踢人下马
int ride_kick(ZONESVRENV* pstEnv, Player *pstPlayer, CSRIDEKICK *pstRideKick)
{
	Player *pstTmpPlayer;

	pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstRideKick->MemID);
	if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		//对方不在线
		return -1;
	}

	if (0 == is_hatch_player_ride(pstTmpPlayer, pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTmpPlayer, SYS_MIDDLE2, 
						ZONEERR_PAK187);
		return -1;
	}

	//kick
	z_ride_break(pstEnv->pstAppCtx, pstEnv, pstTmpPlayer);

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTmpPlayer, SYS_MIDDLE2, ZONEERR_PAK188, pstPlayer->stRoleData.RoleName);

	return 0;
}

//坐骑等级转移
int player_ride_move_level(ZONESVRENV* pstEnv, Player *pstPlayer, CSRIDEMOVELEVEL *pstRideMoveLevel)
{
	int iRet;
	int iIdx;
	RIDEDEF *pstSrcRideDef, *pstDstRideDef;
	RIDEGRID *pstSrcRideGrid;
	RIDEGRID *pstDstRideGrid;
	RIDEPINZHIDEF *pstRidePinZhiDef;
	ROLERIDELIST   *pstRideList = &pstPlayer->stRoleData.MiscInfo.RideList;

	iIdx = z_get_package_ride(pstPlayer, pstRideMoveLevel->ScrRideID);
	if (iIdx < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK189);
		return -1;
	}
	pstSrcRideGrid = &pstRideList->RideGrids[iIdx];
	if(pstSrcRideGrid->Level <= 1)
	{
		return -1;
	}

	iIdx = z_get_package_ride(pstPlayer, pstRideMoveLevel->DstRideID);
	if (iIdx < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK189);
		return -1;
	}
	pstDstRideGrid = &pstRideList->RideGrids[iIdx];

	pstSrcRideDef = z_find_ride_def(pstEnv, pstSrcRideGrid->ItemID, pstSrcRideGrid->Level);
	if(!pstSrcRideDef)
	{
		return -1;
	}

	pstDstRideDef = z_find_ride_def(pstEnv, pstDstRideGrid->ItemID, pstDstRideGrid->Level);
	if(!pstSrcRideDef)
	{
		return -1;
	}

	pstRidePinZhiDef = z_find_ride_pinzhi_def(pstEnv, pstDstRideDef->PinZhi);
	if(!pstRidePinZhiDef)
	{
		return -1;
	}

	/*
	if(pstDstRideGrid->Level >= pstRidePinZhiDef->MaxLevel)
	{
		return -1;
	}*/

	if (0 > package_dec_money_test(pstEnv, pstPlayer, pstSrcRideDef->MoveLevelMoneyType, pstSrcRideDef->MoveLevelMoneyVal))
	{
		char szBuff[32];
			
		if (money_type_to_string(pstSrcRideDef->MoveLevelMoneyType, szBuff, sizeof(szBuff)) < 0 )
		{
			strcpy(szBuff, "金钱");
		}

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK190, szBuff);
		return -1;
	}

	iRet = package_dec_money(pstEnv, pstPlayer, pstSrcRideDef->MoveLevelMoneyType, pstSrcRideDef->MoveLevelMoneyVal);
	if (ATTR_ID_BULL == pstSrcRideDef->MoveLevelMoneyType)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_RIDE_MOVELEV, 
			pstSrcRideDef->RideID, pstSrcRideDef->MoveLevelMoneyVal-iRet, iRet, "坐骑等级转移");
	}
	else if (ATTR_ID_MONEY == pstSrcRideDef->MoveLevelMoneyType)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_RIDE_MOVELEV, 
			pstSrcRideDef->RideID, 0, pstSrcRideDef->MoveLevelMoneyVal, "坐骑等级转移");
	}

	//目标等级变化
	pstDstRideGrid->Level = pstSrcRideGrid->Level;
	if(pstDstRideGrid->Level > pstRidePinZhiDef->MaxLevel)
	{
		pstDstRideGrid->Level = pstRidePinZhiDef->MaxLevel;
	}

	//原坐骑等级
	pstSrcRideGrid->Level =1;
	
	ride_list_chg(pstEnv, pstPlayer);
	if((CS_STAT_RIDE_ON |CS_STAT_FLY) & pstPlayer->stOnline.State)
	{
		CSPKG stPkg;
		CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

		if(pstPlayer->stOnline.RideItemID == (int)pstSrcRideGrid->ItemID)
		{
			pstPlayer->stOnline.RideLevel = pstSrcRideGrid->Level;
		}
		else if(pstPlayer->stOnline.RideItemID == (int)pstDstRideGrid->ItemID)
		{
			pstPlayer->stOnline.RideLevel = pstDstRideGrid->Level;
		}
		else
		{
			return 0;
		}

		z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		pstActionRes->ActionID = ACTION_RIDE;
		pstActionRes->ID = pstPlayer->iMemID;
		pstActionRes->Data.Ride.RideItemID = pstPlayer->stOnline.RideItemID;
		pstActionRes->Data.Ride.RideLevel  = pstPlayer->stOnline.RideLevel;
		pstActionRes->Data.Ride.RideFlag = 1;

		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	}
	
	return 0;
}

int player_ride_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{	
	CSRIDECLT *pstRideClt = &pstCsPkg->Body.RideClt;
	UNUSED(pstFrameHead);

	//if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	//{
	//	z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "战场不允许搭乘");
	//	return -1;
	//}

	switch(pstRideClt->Type)
	{
		case RIDE_DROP:
			return ride_drop(pstAppCtx, pstEnv, pstPlayer, &pstRideClt->Data.RideDrop);
			break;
		case RIDE_APPLY:
			return ride_apply(pstAppCtx, pstEnv, pstPlayer, &pstRideClt->Data.RideApply);
			break;
		case RIDE_APPLY_ACCEPT:
			return ride_apply_accpet(pstAppCtx, pstEnv, pstPlayer, &pstRideClt->Data.RideApplyAccpet);
			break;
		case RIDE_INVITE:
			return ride_invite(pstAppCtx, pstEnv, pstPlayer, &pstRideClt->Data.RideInvite);
			break;
		case RIDE_INVITE_ACCEPT:
			return ride_invite_accpet(pstAppCtx, pstEnv, pstPlayer, &pstRideClt->Data.RideInviteAccpet);
			break;
		case RIDE_NPC_SELECT:
			return ride_npc_select(pstEnv, pstPlayer, &pstRideClt->Data.NpcSelect);
			break;
		case RIDE_LEVEL_UP:
			return ride_level_up(pstEnv, pstPlayer, &pstRideClt->Data.RideLevelUp);
			break;
		case RIDE_APPLY_REFUSE:
			return ride_apply_refuse(pstEnv, pstPlayer, &pstRideClt->Data.RideApplyRefuse);
			break;
		case RIDE_INVITE_REFUSE:
			return ride_invite_refuse(pstEnv, pstPlayer, &pstRideClt->Data.RideInviteRefuse	);
			break;
		case RIDE_KICK_PASSENGER:
			return ride_kick(pstEnv, pstPlayer, &pstRideClt->Data.RideKick);
			break;
		case RIDE_MOVE_LEVEL:
			return player_ride_move_level(pstEnv, pstPlayer, &pstRideClt->Data.MoveLevel);
			break;
		default:
			return -1;
			break;
	}

	return 0;
}

//搭在他人的坐骑上
int is_hatch_player_ride(Player *pstPlayer, Player *pstRidePlayer)
{
	if ((pstPlayer->stOnline.State & CS_STAT_HITCH_RIDE) && pstRidePlayer->iMemID == pstPlayer->stOnline.stRideOnInfo.iOwnerID)
	{
		return 1;		
	}

	return 0;
}

//同一个骑乘上
int z_player_same_ride(ZONESVRENV* pstEnv, Player *pstPlayer1, Player *pstPlayer2)
{
	if(pstPlayer1->stOnline.State & CS_STAT_HITCH_RIDE) //乘客
	{
		Player *pstOwner;
		
		pstOwner = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer1->stOnline.stRideOnInfo.iOwnerID);
		if (NULL == pstOwner)
		{
			return 0;
		}

		if (is_hatch_player_ride(pstPlayer2, pstOwner))
		{
			return 1;
		}
	}
	else if (pstPlayer1->stOnline.State & (CS_STAT_RIDE_ON |CS_STAT_FLY)) // 主人
	{
		if(is_hatch_player_ride(pstPlayer2, pstPlayer1))
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}

	return 0;
}

int package_grid_update_to_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iListType, ROLEITEM *pstRoleItem, ROLEARM *pstRoleArm)
{
	CSROLEARM *pstCSRoleArm;
	CSROLEITEM *pstCsRoleItem;
	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstPackageChg = &pstPackageInfo->PackageInfoData.PackageChg;
	UNUSED(pstAppCtx);

	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstPackageChg->ItemChg.ChgNum = 0;
	pstPackageChg->ArmChg.ChgNum = 0;

	if (pstRoleItem)
	{
		pstCsRoleItem = pstPackageChg->ItemChg.Items+pstPackageChg->ItemChg.ChgNum;
		pstCsRoleItem->ListType = iListType;
		memcpy(&pstCsRoleItem->Item, pstRoleItem, sizeof(pstCsRoleItem->Item));
		pstPackageChg->ItemChg.ChgNum++;
	}
	else if (pstRoleArm)
	{
		pstCSRoleArm = pstPackageChg->ArmChg.Arms+pstPackageChg->ArmChg.ChgNum;
		pstCSRoleArm->ListType = iListType;
		memcpy(&pstCSRoleArm->Arm, pstRoleArm, sizeof(pstCSRoleArm->Arm));
		pstPackageChg->ArmChg.ChgNum++;
	}
	else
	{
		return -1;
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;	
}

USEITEMONEDAYINFO* z_get_item_oneday_info(ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID)
{	
	int i;
	USEITEMONEDAY *pstUseItemOneDay = &pstPlayer->stRoleData.MiscInfo.UseItemOneDay;
	UNUSED(pstEnv);

	for(i= 0; i<pstUseItemOneDay->ItemNum; i++)
	{
		if(pstUseItemOneDay->ItemOneDayInfo[i].ItemID == iItemID)
		{
			if(!IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstUseItemOneDay->ItemOneDayInfo[i].Time))
			{
				pstUseItemOneDay->ItemOneDayInfo[i].OneDayNum = 0;
				pstUseItemOneDay->ItemOneDayInfo[i].Time = pstEnv->pstAppCtx->stCurr.tv_sec;
			}
			
			return &pstUseItemOneDay->ItemOneDayInfo[i];
		}
	}

	return NULL;
}

int item_add_oneday(ZONESVRENV *pstEnv, Player *pstPlayer, int iItemID)
{
	int i;
	USEITEMONEDAYINFO *pstItemOneDayInfo;
	USEITEMONEDAY *pstUseItemOneDay = &pstPlayer->stRoleData.MiscInfo.UseItemOneDay;

	for (i=0; i<pstUseItemOneDay->ItemNum; i++)
	{
		pstItemOneDayInfo = &pstUseItemOneDay->ItemOneDayInfo[i];
		if (pstItemOneDayInfo->ItemID == iItemID)
		{
			if (0 == IsSameDay(pstItemOneDayInfo->Time, pstEnv->pstAppCtx->stCurr.tv_sec))
			{
				pstItemOneDayInfo->OneDayNum = 1;
				pstItemOneDayInfo->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
			}
			else
			{
				pstItemOneDayInfo->OneDayNum++;
			}
			
			return pstItemOneDayInfo->OneDayNum;
		}
	}

	if (pstUseItemOneDay->ItemNum >= MAX_USE_ITEM_ONEDAY)
	{
		memmove(&pstUseItemOneDay->ItemOneDayInfo[0], &pstUseItemOneDay->ItemOneDayInfo[1], 
					(MAX_USE_ITEM_ONEDAY-1)*sizeof(pstUseItemOneDay->ItemOneDayInfo[0]));

		pstItemOneDayInfo = &pstUseItemOneDay->ItemOneDayInfo[MAX_USE_ITEM_ONEDAY-1];
	}
	else
	{
		pstItemOneDayInfo = &pstUseItemOneDay->ItemOneDayInfo[pstUseItemOneDay->ItemNum];
		
		pstUseItemOneDay->ItemNum++;
	}

	pstItemOneDayInfo->ItemID = iItemID;
	pstItemOneDayInfo->OneDayNum = 1;
	pstItemOneDayInfo->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	return pstItemOneDayInfo->OneDayNum;
}


int package_booty_view( ZONESVRENV* pstEnv, Player *pstPlayer, unsigned int  DropBootyID)
{
	CSPKG *pstPkgRes = &pstEnv->stCSRes;
	CSPACKAGEINFO *pstPakInfo = &pstPkgRes->Body.PackageInfo;
	
	pstPakInfo->Type = PACKAGE_NOTIFY_ITEM_VIEW;
	pstPakInfo->PackageInfoData.DropBootyID = DropBootyID;

	Z_CSHEAD_INIT(&pstPkgRes->Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkgRes);
	
	return 0;
}

static int chg_money2bull(ZONESVRENV* pstEnv, Player *pstPlayer, int iMoney)
{
	long long ullAddBull;
	int iTaxBull;
	
	if (iMoney <= 0 || iMoney > COIN_TO_BULL_MAX)
	{
		return -1;
	}

	if (package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, iMoney) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOMONEY3);
		return -1;
	}

	ullAddBull = (iMoney /100.0) * COIN_TO_BULL_RATE;
	iTaxBull = (ullAddBull /10000.0)* COIN_TO_BULL_TAX;		// 税收
	if (iTaxBull == 0)
		iTaxBull = 1;
	
	ullAddBull -= iTaxBull;
	if (ullAddBull > 0x7FFFFFFF)
	{
		return -1;
	}

	if (package_add_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, ullAddBull) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK252, MONEY_TYPE_BULL_NAME);
		return -1;
	}

	package_add_money(pstEnv, pstPlayer, ATTR_ID_BULL, ullAddBull);
	package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, iMoney);
	return 0;
}

int player_coin_to_bull(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSCHANGECOININTO *pstReq = &pstCsPkg->Body.ChangeCoinInto;

	UNUSED(pstFrameHead);
	UNUSED(pstAppCtx);
	
	if( pstEnv->iInstMode != INST_MODE_ZONE_SVR )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
		return 0;
	}
	
	if (pstReq->NpcID != pstPlayer->stOnline.iDialogID)
		return -1;

	if (check_npc_ui(pstEnv, pstPlayer, UI_TYPE_CHANGE_COIN_INTO) < 0)
		return -1;

	if (pstReq->WhichWay == COIN_MONEY2BULL)
		return chg_money2bull(pstEnv, pstPlayer, pstReq->From);

	return 0;
}

int money_type_to_string(int iMoneyType, char* pszBuff, int iSize)
{
	switch(iMoneyType)
	{
		case ATTR_ID_BULL:
			STRNCPY(pszBuff, MONEY_TYPE_BULL_NAME,iSize);
			break;
		case ATTR_ID_MONEY:
			STRNCPY(pszBuff, MONEY_TYPE_MONEY_NAME,iSize);
			break;
		case ATTR_ID_BINDGODCOIN:
			STRNCPY(pszBuff, MONEY_TYPE_BIND_GODCOIN_NAME,iSize);
			break;
		case ATTR_ID_GODCOIN:
			STRNCPY(pszBuff, MONEY_TYPE_GODCOIN_NAME,iSize);
			break;
		case ATTR_ID_ZHENGYI:
			STRNCPY(pszBuff, MONEY_TYPE_ZHENGYI,iSize);
			break;
		default:
			return -1;
			break;
	}

	return 0;
}

int z_fill_link_msg(CSPKG *pstPkg, int iSysType, int iLinkType, void *pstLinkData, int iErrno, ...)
{
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	CHATLINK stChatLink;
	CHATLINKDATA *pstChatLinkData;
	const char* pszErr;
	va_list marker;

	va_start( marker, iErrno );
	pszErr	=	ZONE_GET_ERR(iErrno, 0);

	memset(&stChatLink, 0, sizeof(stChatLink));

	pstSysMsg->Type = iSysType;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
	memset(pstSysMsg->LinkContent, 0, sizeof(pstSysMsg->LinkContent));

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

	stChatLink.Link = 1;
	pstChatLinkData = &stChatLink.LinkData[0];
	
	if(iLinkType == CHAT_LINK_ITEM)
	{
		ROLEITEM *pstRoleItem;

		pstRoleItem = (ROLEITEM *)pstLinkData;

		pstChatLinkData->Type = CHAT_LINK_ITEM;
		pstChatLinkData->Data.ItemLink.ItemID = pstRoleItem->DefID;
		pstChatLinkData->Data.ItemLink.ItemDur = pstRoleItem->InstVal1;
		pstChatLinkData->Data.ItemLink.InstVal1 = pstRoleItem->InstVal1;
		pstChatLinkData->Data.ItemLink.InstVal2 = pstRoleItem->InstVal2;
		pstChatLinkData->Data.ItemLink.InstVal3 = pstRoleItem->InstVal3;
		pstChatLinkData->Data.ItemLink.Flag = pstRoleItem->Flag;
	}
	else if (iLinkType == CHAT_LINK_ARM)
	{
		ROLEARM *pstRoleArm;

		pstRoleArm = (ROLEARM *)pstLinkData;

		pstChatLinkData->Type = CHAT_LINK_ARM;
		memcpy(&pstChatLinkData->Data.ArmLink, pstRoleArm, sizeof(pstChatLinkData->Data.ArmLink));
	}
	else if(iLinkType == CHAT_LINK_POSTION)
	{
		POSTIONLINK *pstPostionLink;

		pstPostionLink = (POSTIONLINK *)pstLinkData;

		pstChatLinkData->Type = CHAT_LINK_POSTION;
		memcpy(&pstChatLinkData->Data.PostionLink, pstPostionLink, sizeof(pstChatLinkData->Data.PostionLink));
	}
	else
	{
		return -1;
	}

	if(sizeof(stChatLink) > MAX_SYS_LINK)
	{
		return -1;
	}
	
	memcpy(pstSysMsg->LinkContent, (char *)&stChatLink, sizeof(stChatLink));
	pstSysMsg->LinkLen = MAX_SYS_LINK;

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	
	return 0;
}


int ride_clear_passenger(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	 if (!(pstPlayer->stOnline.State & (CS_STAT_RIDE_ON |CS_STAT_FLY)))
	 {
		return 0;
	 }
	 	
	//清理马上得乘客
	if (pstPlayer->stOnline.stRideOnInfo.iPassengerNum)
	{
		int i;
		Player *pstTmpPlayer;

		for(i = 0; i<pstPlayer->stOnline.stRideOnInfo.iPassengerNum; i++)
		{
			pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stRideOnInfo.PassengerID[i]);
			if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				continue;
			}

			//客人下马
			z_hitch_ride_clear(pstEnv, pstTmpPlayer);
		}
	}
		
	return 0;
}
//看看是否能装备抢手的左手武器
int check_gunner_wear_pos_left(ZONESVRENV* pstEnv, Player *pstPlayer,ITEMDEF *pstWearItemDef)
{
	int i = 0;
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	ITEMDEF *pstItemDef = NULL;
	ROLEARM *pstRoleArm = NULL;

	return 0;
	
	if( CAREER_GUNER != pstPlayer->stRoleData.Career && 
		CAREER_GUNER_SINGLEHAND != pstPlayer->stRoleData.Career &&
		CAREER_GUNER_DOUBLEHAND != pstPlayer->stRoleData.Career)
	{
		return 0;
	}
	//看看装备是否是左手
	if(WEAR_POS_LEFT !=  pstWearItemDef->WearPos1 )
	{
		//不是左手,就不用判断了
		return 0;
	}
	
	for (i=0; i<pstRoleWear->ArmNum; i++)
	{
		pstRoleArm = pstRoleWear->RoleArms+i;
		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleArm);
		if (NULL == pstItemDef )
		{
			continue;
		}
		
		if( WEAR_POS_RIGHTHAND == pstItemDef->WearPos1 && 
			WEAR_POS_LEFT == pstItemDef->WearPos2 )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_PAK191);
			// 确定有右手
			return 1;
		}	
		else if(WEAR_POS_RIGHTHAND == pstItemDef->WearPos1 && 
				 WEAR_POS_NO == pstItemDef->WearPos2)
		{
			return 0;
		}
	}

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_PAK192);
	return 1;
}

//ret -1表示没有装备枪手的左手武器,>-1表示装备了枪手的左手武器,并且也要卸下来 
int check_gunner_unwear_pos_left(ZONESVRENV* pstEnv, Player *pstPlayer,ITEMDEF *pstUnwearItemDef)
{
	int i = 0;
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	ITEMDEF *pstItemDef = NULL;
	ROLEARM *pstRoleArm = NULL;

	//不做左手武器的判断
	return -1;
	
	if( CAREER_GUNER != pstPlayer->stRoleData.Career && 
		CAREER_GUNER_SINGLEHAND != pstPlayer->stRoleData.Career &&
		CAREER_GUNER_DOUBLEHAND != pstPlayer->stRoleData.Career  )
	{
		return -1;
	}
	//看看卸下的是否是右手装备
	if(WEAR_POS_RIGHTHAND !=  pstUnwearItemDef->WearPos1 )
	{
		//不是右手,就不用判断左手了
		return -1;
	}
	
	for (i=0; i<pstRoleWear->ArmNum; i++)
	{
			
		pstRoleArm = pstRoleWear->RoleArms+i;
		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleArm);
		if (NULL == pstItemDef )
		{
			continue;
		}
		
		if( WEAR_POS_LEFT == pstItemDef->WearPos1 && 
			ARM_GUNNER_WEAR_LEFT == pstItemDef->NumType )
		{
			// 找到左手装备
			return i;
		}			
	}
	
	return -1;
}

int package_gunner_unwear_pos_left(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int  iPos)
{
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	SUNDRIESPACKAGE *pstSundriesPackage = &pstPlayer->stRoleData.Package.SundriesPackage;
	ITEMDEF *pstItemDef = NULL;
	ROLEARM *pstRoleArm = NULL;
	int 	iGridIdx = 0;
	
       CSWEAR stCSUnWear;
	memset(&stCSUnWear,0,sizeof(stCSUnWear));

	if (0 > iPos)
	{
		return -1;
	}
	pstRoleArm = pstRoleWear->RoleArms+iPos;
	pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleArm);
	if (NULL == pstItemDef )
	{
		return -1;
	}

	/* 找到装备的空余索引 */
	iGridIdx = package_get_free_grid(pstSundriesPackage->RoleGrids, pstSundriesPackage->Num, pstSundriesPackage->MaxNum);
	if (0 > iGridIdx)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "grididx<0");
		return -1;
	}
	
	stCSUnWear.ArmDefID = pstItemDef->ItemID;
	stCSUnWear.WearPos = WEAR_POS_LEFT;
	stCSUnWear.ListType = LIST_TYPE_SUNDRIES;
	stCSUnWear.GridIdx = iGridIdx;
	stCSUnWear.PetWID  = 0;
	
	// 直接调用
	return player_unwear( pstAppCtx, pstEnv, pstPlayer, &stCSUnWear);

}

int package_get_guner_wear_left_continuenum(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,SKILLDEF *pstSkillDef)
{
	int i = 0;
	int j = 0;
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	ITEMDEF *pstItemDef = NULL;
	ROLEARM *pstRoleArm = NULL;
	UNUSED(pstAppCtx);
	
	if( CAREER_GUNER != pstPlayer->stRoleData.Career && 
		CAREER_GUNER_SINGLEHAND != pstPlayer->stRoleData.Career &&
		CAREER_GUNER_DOUBLEHAND != pstPlayer->stRoleData.Career )
	{
		return -1;
	}

	for (i=0; i<pstRoleWear->ArmNum; i++)
	{
			
		pstRoleArm = pstRoleWear->RoleArms+i;
		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleArm);
		if (NULL == pstItemDef )
		{
			continue;
		}
		
		if( WEAR_POS_LEFT == pstItemDef->WearPos1 && 
			ARM_GUNNER_WEAR_LEFT == pstItemDef->NumType )
		{
			// 找到左手装备
			for( j=0; j<MAX_SKILL_ARM_CHG;j++)
			{
				if( ARM_GUNNER_WEAR_LEFT == pstSkillDef->SkillArmChg[j].ArmType )
				{
					return pstSkillDef->SkillArmChg[j].ContinueNum;
				}
			}
		}			
	}
	
	return -1;
}

ROLEARM *get_player_collect_arm(ZONESVRENV* pstEnv, ROLEPACKAGE *pstPak, MONCOLLECT *pstMonsterCollectDef)
{
	int i = 0;
	ROLEGRID* pstRoleGrid;
	ROLEARM *pstRoleArm = NULL;
	UNUSED(pstEnv);
	
	for (i=0; i<pstPak->SundriesPackage.Num; i++)
	{
		pstRoleGrid = &pstPak->SundriesPackage.RoleGrids[i];
		
		if(pstRoleGrid->Type != ITEM_TYPE_ARM)
		{
			continue;
		}

		if((int)pstMonsterCollectDef->CollectArm != pstRoleGrid->GridData.RoleArm.DefID)
		{
			continue;
		}

		if(pstRoleGrid->GridData.RoleArm.Dur < pstMonsterCollectDef->CollectArmDur)
		{
			//耐久不够的情况需要了解
			pstRoleArm = &pstRoleGrid->GridData.RoleArm;
			continue;
		}
		
		return &pstRoleGrid->GridData.RoleArm;
	}

	return pstRoleArm;
}

//是否骑乘状态
int is_player_on_ride(Player *pstPlayer)
{
	if(pstPlayer->stOnline.State & (CS_STAT_RIDE_ON |CS_STAT_FLY))
	{
		return 1;
	}

	return 0;
}

//角色骑乘携带的乘客数量
int player_get_ride_passenger_num(Player *pstPlayer)
{
	if( is_player_on_ride(pstPlayer) && pstPlayer->stOnline.stRideOnInfo.iPassengerNum > 0)
	{
		return pstPlayer->stOnline.stRideOnInfo.iPassengerNum;
	}

	return 0;
}

// 获取附魔相关属性
// iWhich:  0: 附魔类型
//          1: 附魔特殊效果的冷却时间
//          2: 附魔以后是否绑定
int z_extend_item_property(ZONESVRENV *pstEnv, int iDefID, int iWhich)
{
	size_t i = 0;
	const ITEMDEF *pstDef = z_find_itemdef(pstEnv, iDefID);
	if(!pstDef)
	{
		return 0;
	}

	switch(iWhich)
	{
	case 0:
		return pstDef->ExtendedType; // 蛋疼的附魔类型配置到此处
		break;

	case 1:
	case 2:
		for(i = 0; i < CCH(pstDef->Result); ++i)
		{
			if(!pstDef->Result[i].ResultID)
			{
				return 0;
			}
			if(pstDef->Result[i].ResultID == RESULT_EXTENDED_CONF)
			{
				break;
			}
		}
		if(i >= CCH(pstDef->Result))
		{
			return 0;
		}
		switch(iWhich)
		{
		case 1:
			return pstDef->Result[i].ResultVal2; // 冷却时间(秒)配置到此处
			break;

		case 2:
			return pstDef->Result[i].ResultVal1; // 附魔后是否绑定装备
			break;
		}
		break;

	default:
		break;
	}
	return 0;
}

// 通过物品给物品附魔
int z_extend_item_from_item(Player *pstPlayer, const ITEMDEF *pstBeUsedDef,
                            ROLEGRID *pstBeExtended)
{
	ExtendedOL *pstExtendedOL = &pstPlayer->stOnline.stExtendedOL;
	
	// 给装备加上附魔效果
	switch(pstBeUsedDef->ExtendedType)
	{
	case EXTENDED_TYPE_RESULT:
		// 装备效果
		if(!pstBeUsedDef->Result[0].ResultID)
		{
			return -1;
		}

		memset(pstExtendedOL, 0, sizeof(*pstExtendedOL));
		pstExtendedOL->WID = pstBeExtended->GridData.RoleArm.WID;
		pstExtendedOL->ExtendedFromDefID = pstBeUsedDef->ItemID;
		pstExtendedOL->ExtendedResult.ResultID = pstBeUsedDef->Result[0].ResultID;

		/*
		// 清除原先已有的附魔效果
		pstBeExtended->GridData.RoleArm.ExtendedFromDefID = 0;
		memset(&pstBeExtended->GridData.RoleArm.ExtendedResult, 0,
		       sizeof(pstBeExtended->GridData.RoleArm.ExtendedResult));

		pstBeExtended->GridData.RoleArm.ExtendedFromDefID =
			pstBeUsedDef->ItemID;
		pstBeExtended->GridData.RoleArm.ExtendedResult.ResultID =
			pstBeUsedDef->Result[0].ResultID;
		*/
		
		if(pstBeUsedDef->Result[0].ResultID == pstBeUsedDef->Result[1].ResultID)
		{
			pstExtendedOL->ExtendedResult.ResultVal1 = 
				rand_range(pstBeUsedDef->Result[0].ResultVal1,
				           pstBeUsedDef->Result[1].ResultVal1);
			pstExtendedOL->ExtendedResult.ResultVal2 = 
				rand_range(pstBeUsedDef->Result[0].ResultVal2,
				           pstBeUsedDef->Result[1].ResultVal2);
			pstExtendedOL->ExtendedResult.ResultVal3 = 
				rand_range(pstBeUsedDef->Result[0].ResultVal3,
				           pstBeUsedDef->Result[1].ResultVal3);
			pstExtendedOL->ExtendedResult.ResultVal4 = 
				rand_range(pstBeUsedDef->Result[0].ResultVal4,
				           pstBeUsedDef->Result[1].ResultVal4);
			/*
			// 随机数值
			pstBeExtended->GridData.RoleArm.ExtendedResult.ResultVal1 =
				rand_range(pstBeUsedDef->Result[0].ResultVal1,
				           pstBeUsedDef->Result[1].ResultVal1);
			pstBeExtended->GridData.RoleArm.ExtendedResult.ResultVal2 =
				rand_range(pstBeUsedDef->Result[0].ResultVal2,
				           pstBeUsedDef->Result[1].ResultVal2);
			pstBeExtended->GridData.RoleArm.ExtendedResult.ResultVal3 =
				rand_range(pstBeUsedDef->Result[0].ResultVal3,
				           pstBeUsedDef->Result[1].ResultVal3);
			pstBeExtended->GridData.RoleArm.ExtendedResult.ResultVal4 =
				rand_range(pstBeUsedDef->Result[0].ResultVal4,
				           pstBeUsedDef->Result[1].ResultVal4);
			*/
		}
		else
		{

			pstExtendedOL->ExtendedResult.ResultVal1 =
				pstBeUsedDef->Result[0].ResultVal1;
			pstExtendedOL->ExtendedResult.ResultVal2 =
				pstBeUsedDef->Result[0].ResultVal2;
			pstExtendedOL->ExtendedResult.ResultVal3 =
				pstBeUsedDef->Result[0].ResultVal3;
			pstExtendedOL->ExtendedResult.ResultVal4 =
				pstBeUsedDef->Result[0].ResultVal4;

			/*
			// 固定数值
			pstBeExtended->GridData.RoleArm.ExtendedResult.ResultVal1 =
				pstBeUsedDef->Result[0].ResultVal1;
			pstBeExtended->GridData.RoleArm.ExtendedResult.ResultVal2 =
				pstBeUsedDef->Result[0].ResultVal2;
			pstBeExtended->GridData.RoleArm.ExtendedResult.ResultVal3 =
				pstBeUsedDef->Result[0].ResultVal3;
			pstBeExtended->GridData.RoleArm.ExtendedResult.ResultVal4 =
				pstBeUsedDef->Result[0].ResultVal4;
			*/
		}
		break;

	case EXTENDED_TYPE_ATTR:
		return -1;
		/*
		// 装备关键字
		if(!pstBeUsedDef->Result[0].ResultID)
		{
			return -1;
		}

		// 清除原先已有的附魔效果
		pstBeExtended->GridData.RoleArm.ExtendedFromDefID  = 0;
		memset(&pstBeExtended->GridData.RoleArm.ExtendedResult, 0,
		       sizeof(pstBeExtended->GridData.RoleArm.ExtendedResult));

		pstBeExtended->GridData.RoleArm.ExtendedFromDefID =
			pstBeUsedDef->ItemID;
		pstBeExtended->GridData.RoleArm.ExtendedResult.ResultID =
			pstBeUsedDef->Result[0].ResultID;
		if(pstBeUsedDef->Result[0].ResultID == pstBeUsedDef->Result[1].ResultID)
		{
			// 随机数值
			pstBeExtended->GridData.RoleArm.ExtendedResult.ResultVal1 =
				rand_range(pstBeUsedDef->Result[0].ResultVal1,
				           pstBeUsedDef->Result[1].ResultVal1);
		}
		else
		{
			// 固定数值
			pstBeExtended->GridData.RoleArm.ExtendedResult.ResultVal1 =
				pstBeUsedDef->Result[0].ResultVal1;
		}*/
		break;

	default:
		return -1;
		break;
	}

	return 0;
}


int player_keyin_res(ZONESVRENV *pstEnv, Player *pstPlayer, int iSucc, EXTENDITEMCLT *pstReq)
{	
	CSPKG stPkg;
	EXTENDITEMSVR *pstRes = &stPkg.Body.KeyinItemSvr;

	pstRes->Succeeded = iSucc;	
	pstRes->BeExtended = pstReq->BeExtended;
	Z_CSHEAD_INIT(&stPkg.Head, KEYIN_ITEM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_keyin_add_attr(ZONESVRENV *pstEnv, Player *pstPlayer, ROLEGRID *pstArm, ITEMDEF* pstReelDef)
{
	UNUSED(pstPlayer);
	
	pstArm->GridData.RoleArm.KeYinAddAttr.Val = 
				pstReelDef->Result[0].ResultVal2 + 
				RAND1(pstReelDef->Result[0].ResultVal3 - 
				pstReelDef->Result[0].ResultVal2);
	pstArm->GridData.RoleArm.KeYinEnd = 
			pstEnv->pstAppCtx->stCurr.tv_sec + pstReelDef->Result[0].ResultVal4/1000*60;


	if (pstReelDef->Result[0].ResultID == RESULT_ELEMENT_ATK)
	{
		if (pstReelDef->Result[0].ResultVal1 == FAIRY_ATTR_FIRE)
		{
			pstArm->GridData.RoleArm.KeYinAddAttr.AddAttrID = RESULT_FIRE_ATK;
		}
		else if(pstReelDef->Result[0].ResultVal1 == FAIRY_ATTR_LIGHT)
		{
			pstArm->GridData.RoleArm.KeYinAddAttr.AddAttrID = RESULT_LIGHT_ATK;
		}
		else if(pstReelDef->Result[0].ResultVal1 == FAIRY_ATTR_NIGHT)
		{
			pstArm->GridData.RoleArm.KeYinAddAttr.AddAttrID = RESULT_NIGHT_ATK;
		}
		else if(pstReelDef->Result[0].ResultVal1 == FAIRY_ATTR_THUNDER)
		{
			pstArm->GridData.RoleArm.KeYinAddAttr.AddAttrID = RESULT_THUNDER_ATK;
		}	
		else if(pstReelDef->Result[0].ResultVal1 == FAIRY_ATTR_WATER)
		{
			pstArm->GridData.RoleArm.KeYinAddAttr.AddAttrID = RESULT_WATER_ATK;
		}
		else
		{
			return -1;
		}
	}
	else if (pstReelDef->Result[0].ResultID == RESULT_ELEMENT_DEF)
	{
		if (pstReelDef->Result[0].ResultVal1 == FAIRY_ATTR_FIRE)
		{
			pstArm->GridData.RoleArm.KeYinAddAttr.AddAttrID = RESULT_FIRE_DEF;
		}
		else if(pstReelDef->Result[0].ResultVal1 == FAIRY_ATTR_LIGHT)
		{
			pstArm->GridData.RoleArm.KeYinAddAttr.AddAttrID = RESULT_LIGHT_DEF;
		}
		else if(pstReelDef->Result[0].ResultVal1 == FAIRY_ATTR_NIGHT)
		{
			pstArm->GridData.RoleArm.KeYinAddAttr.AddAttrID = RESULT_NIGHT_DEF;
		}
		else if(pstReelDef->Result[0].ResultVal1 == FAIRY_ATTR_THUNDER)
		{
			pstArm->GridData.RoleArm.KeYinAddAttr.AddAttrID = RESULT_THUNDER_DEF;
		}	
		else if(pstReelDef->Result[0].ResultVal1 == FAIRY_ATTR_WATER)
		{
			pstArm->GridData.RoleArm.KeYinAddAttr.AddAttrID = RESULT_WATER_DEF;
		}
		else
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

int player_keyin_check_wearpos(ZONESVRENV *pstEnv, Player *pstPlayer, ITEMDEF *pstArmDef, ITEMDEF* pstReelDef)
{
	int iPosSucc = 0;
	
	if (pstReelDef->Result[0].ResultID == RESULT_ELEMENT_ATK)
	{
		if (pstArmDef->WearPos1 > 0 &&
			(pstArmDef->WearPos1 == WEAR_POS_RIGHTHAND ||
			pstArmDef->WearPos1 == WEAR_POS_LEFT ||
			pstArmDef->WearPos1 == WEAR_POS_SHOULDER))
		{
			iPosSucc = 1;
		}

		if (pstArmDef->WearPos2 > 0 &&
			(pstArmDef->WearPos2 == WEAR_POS_RIGHTHAND ||
			pstArmDef->WearPos2 == WEAR_POS_LEFT ||
			pstArmDef->WearPos2 == WEAR_POS_SHOULDER))
		{
			iPosSucc = 1;
		}

		if (pstArmDef->WearPos3 > 0 &&
			(pstArmDef->WearPos3 == WEAR_POS_RIGHTHAND ||
			pstArmDef->WearPos3 == WEAR_POS_LEFT ||
			pstArmDef->WearPos3 == WEAR_POS_SHOULDER))
		{
			iPosSucc = 1;
		}

		if (!iPosSucc)
		{	
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK253);
			return -1;
		}
	}
	else if (pstReelDef->Result[0].ResultID == RESULT_ELEMENT_DEF)
	{
		if (pstArmDef->WearPos1 > 0 &&
			(pstArmDef->WearPos1 == WEAR_POS_HEAD ||
			pstArmDef->WearPos1 == WEAR_POS_BODY ||
			pstArmDef->WearPos1 == WEAR_POS_FACE ||
			pstArmDef->WearPos1 == WEAR_POS_SHOE))
		{
			iPosSucc = 1;
		}

		if (pstArmDef->WearPos2 > 0 &&
			(pstArmDef->WearPos2 == WEAR_POS_HEAD ||
			pstArmDef->WearPos2 == WEAR_POS_BODY ||
			pstArmDef->WearPos2 == WEAR_POS_FACE ||
			pstArmDef->WearPos2 == WEAR_POS_SHOE))
		{
			iPosSucc = 1;
		}

		if (pstArmDef->WearPos3 > 0 &&
			(pstArmDef->WearPos3 == WEAR_POS_HEAD ||
			pstArmDef->WearPos3 == WEAR_POS_BODY ||
			pstArmDef->WearPos3 == WEAR_POS_FACE ||
			pstArmDef->WearPos3 == WEAR_POS_SHOE))
		{
			iPosSucc = 1;
		}
		if (!iPosSucc)
		{	
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK253);
			return -1;
		}
	}
	else
	{
		return -1;
	}
	return 0;
}

// 刻印
int player_keyin_item(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
                       Player *pstPlayer, TFRAMEHEAD *pstFrameHead,
                       CSPKG *pstCsPkg)
{
	EXTENDITEMCLT *pstReq = &pstCsPkg->Body.KeyinItemClt;
	ROLEPACKAGE *pstPackage = &pstPlayer->stRoleData.Package;
	ROLEGRID *pstArm = NULL, *pstReel = NULL;
	ITEMDEF *pstArmDef = NULL, *pstReelDef = NULL;
	int iReelPos = -1;
	int iPos;
	int iDec;
	MACHINNGEXPENDDATA stExpendData;
	int iSucc = 0;
	ROLEARM stOldArm;

	UNUSED(pstFrameHead);

	// 元素卷轴
	switch(pstReq->BeUsed.ListType)
	{		
	case LIST_TYPE_SUNDRIES:
		{
			iReelPos = package_grid_pos(pstPackage->SundriesPackage.RoleGrids,
				                          pstPackage->SundriesPackage.Num,
				                          pstReq->BeUsed.GridIdx);
			if(iReelPos < 0)
			{
				goto error;
			}

			pstReel = pstPackage->SundriesPackage.RoleGrids + iReelPos;
			if(pstReel->GridData.RoleItem.WID != pstReq->BeUsed.WID ||
			   pstReel->GridData.RoleItem.GridIdx != pstReq->BeUsed.GridIdx)
			{
				goto error;
			}

			pstReelDef = z_get_itemdef_roleitem(pstEnv,
			                                     &pstReel->GridData.RoleItem);
			if(!pstReelDef)
			{
				goto error;
			}
			
			if (	pstReelDef->Result[0].ResultID != RESULT_ELEMENT_ATK && 
				pstReelDef->Result[0].ResultID != RESULT_ELEMENT_DEF)
			{
				goto error;
			}
		}
		break;

	default:
		goto error;
		break;
	}

	 // 刻印装备
	switch(pstReq->BeExtended.ListType)
	{		
	case LIST_TYPE_SUNDRIES:
		{
			iPos = package_grid_pos(pstPackage->SundriesPackage.RoleGrids,
				                        pstPackage->SundriesPackage.Num,
				                        pstReq->BeExtended.GridIdx);
			if(iPos < 0)
			{
				goto error;
			}

			pstArm = pstPackage->SundriesPackage.RoleGrids + iPos;
			if(pstArm->GridData.RoleItem.WID != pstReq->BeExtended.WID ||
			   pstArm->GridData.RoleItem.GridIdx != pstReq->BeExtended.GridIdx)
			{
				goto error;
			}

			pstArmDef = z_get_itemdef_roleitem(pstEnv,
			                                 &pstArm->GridData.RoleItem);
			if(!pstArmDef)
			{
				goto error;
			}
			if(arm_is_fashion(pstArmDef))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_SVR,
				           ZONEERR_PAK254);
				goto error;
			}
			if(pstArm->Type != GRID_ITEM_ARM)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR,
				           ZONEERR_PAK255);
				goto error;
			}
		}
		break;

	default:
		goto error;
		break;
	}
	if(!pstArmDef || !pstReelDef || !pstArm)
	{
		goto error;
	}

	memcpy(&stOldArm, &pstArm->GridData.RoleArm, sizeof(stOldArm));
	
	// 装备等级检查
	if (	pstArmDef->WearPre.RoleLevelMin > 0 && 
		pstArmDef->WearPre.RoleLevelMin < pstReelDef->Result[0].ResultVal4%1000)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK256,  
					pstReelDef->Result[0].ResultVal4%1000);
		goto error;
	}

	// 位置检查
	if (player_keyin_check_wearpos(pstEnv, pstPlayer, pstArmDef, pstReelDef) < 0)
	{
		goto error;
	}
	
	if (pstReelDef->Result[0].ResultVal1 < FAIRY_ATTR_FIRE ||
		pstReelDef->Result[0].ResultVal1 > FAIRY_ATTR_WATER)
	{
		goto error;
	}

	iDec = package_dec(pstAppCtx, pstEnv, pstPlayer,
	               pstReelDef->ItemID, 1,pstReq->BeUsed.ListType, &iReelPos,
	               PACKAGE_DEC_NONE, IMPITEM_OPTYPE_ARMKEYIN);
	// 削减元素卷轴
	if(iDec < 0)
	{
		goto error;
	}

	// 装备刻印
	iPos = package_grid_pos(pstPackage->SundriesPackage.RoleGrids,
				                        pstPackage->SundriesPackage.Num,
				                        pstReq->BeExtended.GridIdx);
	if(iPos < 0)
		goto error;

	pstArm = pstPackage->SundriesPackage.RoleGrids + iPos;
	if(!pstArm)
	{
		goto error;
	}
	if ( player_keyin_add_attr(pstEnv, pstPlayer, pstArm, pstReelDef) < 0)
	{
		goto error;
	}

	iSucc = 1;

	if (iDec > 0)
	{
		pstArm->GridData.RoleArm.InstFlag |= INST_ITEM_BIND;
	}

	// 通知客户端装备属性改变
	package_grid_update_to_clt(pstAppCtx, pstEnv, pstPlayer,
	                           				pstReq->BeExtended.ListType, NULL,
	                          				 &pstArm->GridData.RoleArm);

	bzero(&stExpendData, sizeof(stExpendData));
	itemdec_fill(&stExpendData.StuffInfo[0], pstReelDef->ItemID, 1, iDec);
	stExpendData.StuffNum = 1;

	z_role_machining_oplog(pstEnv, pstPlayer, &stOldArm, &pstArm->GridData.RoleArm, 
			&stExpendData, OP_MACHINING_ARM_KEYIN, 1, NULL,0);
error:
	player_keyin_res(pstEnv, pstPlayer, iSucc, pstReq);
	return 0;
}


// 附魔
int player_extend_item(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
                       Player *pstPlayer, TFRAMEHEAD *pstFrameHead,
                       CSPKG *pstCsPkg)
{
	CSPKG stPkg;
	EXTENDITEMSVR *pstRes = &stPkg.Body.ExtendItemSvr;
	const EXTENDITEMCLT *pstReq = &pstCsPkg->Body.ExtendItemClt;
	ROLEPACKAGE *pstPackage = &pstPlayer->stRoleData.Package;
	ROLEGRID *pstBeUsed = NULL, *pstBeExtended = NULL;
	ITEMDEF *pstBeUsedDef = NULL, *pstBeExtendedDef = NULL;
	int iBeUsedPos = 0;
	MACHINNGEXPENDDATA stExpendData;
	ROLEARM stOldRoleArm;
	
	UNUSED(pstFrameHead);

	memset(pstRes, 0, sizeof(*pstRes));
		
	switch(pstReq->BeUsed.ListType)
	{		
	case LIST_TYPE_SUNDRIES:
		{
			iBeUsedPos = package_grid_pos(pstPackage->SundriesPackage.RoleGrids,
				                          pstPackage->SundriesPackage.Num,
				                          pstReq->BeUsed.GridIdx);
			if(iBeUsedPos < 0)
			{
				goto error;
			}

			pstBeUsed = pstPackage->SundriesPackage.RoleGrids + iBeUsedPos;
			if(pstBeUsed->GridData.RoleItem.WID != pstReq->BeUsed.WID ||
			   pstBeUsed->GridData.RoleItem.GridIdx != pstReq->BeUsed.GridIdx)
			{
				goto error;
			}

			pstBeUsedDef = z_get_itemdef_roleitem(pstEnv,
			                                     &pstBeUsed->GridData.RoleItem);
			if(!pstBeUsedDef)
			{
				goto error;
			}
			if(pstBeUsed->Type != GRID_ITEM_ITEM || !pstBeUsedDef->ExtendedType)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
				           ZONEERR_PAK257);
				goto error;
			}
		}
		break;

	default:
		goto error;
		break;
	}

	switch(pstReq->BeExtended.ListType)
	{		
	case LIST_TYPE_SUNDRIES:
		{
			int iPos = package_grid_pos(pstPackage->SundriesPackage.RoleGrids,
				                        pstPackage->SundriesPackage.Num,
				                        pstReq->BeExtended.GridIdx);
			if(iPos < 0)
			{
				goto error;
			}

			pstBeExtended = pstPackage->SundriesPackage.RoleGrids + iPos;
			if(pstBeExtended->GridData.RoleItem.WID != pstReq->BeExtended.WID ||
			   pstBeExtended->GridData.RoleItem.GridIdx !=
			   pstReq->BeExtended.GridIdx)
			{
				goto error;
			}

			pstBeExtendedDef = z_get_itemdef_roleitem(pstEnv,
			                                 &pstBeExtended->GridData.RoleItem);
			if(!pstBeExtendedDef)
			{
				goto error;
			}
			if(pstBeExtended->Type != GRID_ITEM_ARM)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
				           ZONEERR_PAK258);
				goto error;
			}
		}
		break;

	default:
		goto error;
		break;
	}
	if(!pstBeUsedDef)
	{
		goto error;
	}
	// 检查附魔的位置是否正确
	if(pstBeExtendedDef && pstBeUsedDef->WearPosNum)
	{
		char sMap[WEAR_POS_MAX+1];
		memset(sMap, 0, sizeof(sMap));

		sMap[pstBeExtendedDef->WearPos1] = !!pstBeExtendedDef->WearPos1;
		sMap[pstBeExtendedDef->WearPos2] = !!pstBeExtendedDef->WearPos2;
		sMap[pstBeExtendedDef->WearPos3] = !!pstBeExtendedDef->WearPos3;

		if(!sMap[pstBeUsedDef->WearPos1] && !sMap[pstBeUsedDef->WearPos2] &&
		   !sMap[pstBeUsedDef->WearPos3])
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
			           ZONEERR_PAK253);
			goto error;
		}
	}

	// 检查被附魔的装备的等级限制
	if(pstBeExtendedDef && pstBeUsedDef->WearPre.RoleLevelMin)
	{
		if(pstBeExtendedDef->WearPre.RoleLevelMin <
		   pstBeUsedDef->WearPre.RoleLevelMin)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
			           ZONEERR_PAK259,
			           pstBeUsedDef->WearPre.RoleLevelMin);
			goto error;
		}
	}
	if(pstBeExtendedDef && pstBeUsedDef->WearPre.RoleLevelMax)
	{
		if(pstBeExtendedDef->WearPre.RoleLevelMax >
		   pstBeUsedDef->WearPre.RoleLevelMax)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
			           ZONEERR_PAK260,
			           pstBeUsedDef->WearPre.RoleLevelMax);
			goto error;
		}
	}

	if(!pstBeExtended)
	{
		goto error;
	}

	memcpy(&stOldRoleArm, &pstBeExtended->GridData.RoleArm, sizeof(stOldRoleArm));
	
	// 给装备加上附魔效果
	if(z_extend_item_from_item(pstPlayer, pstBeUsedDef, pstBeExtended) < 0)
	{
		goto error;
	}

	// 附魔后绑定装备
	if(z_extend_item_property(pstEnv, pstBeUsedDef->ItemID, 2) ||
		(pstBeUsed && is_bind_roleitem(&pstBeUsed->GridData.RoleItem)))
	{
		pstBeExtended->GridData.RoleArm.InstFlag |= INST_ITEM_BIND;
	}
	package_grid_update_to_clt(pstAppCtx, pstEnv, pstPlayer,
	                           pstReq->BeExtended.ListType, NULL,
	                           &pstBeExtended->GridData.RoleArm);
	pstRes->Succeeded = 1;

	bzero(&stExpendData, sizeof(stExpendData));
	stExpendData.StuffNum = 1;
	//stExpendData.StuffInfo[0].ItemID = pstBeUsedDef->ItemID;
	//stExpendData.StuffInfo[0].ItemNum = 1;
	itemdec_fill(&stExpendData.StuffInfo[0], pstBeUsedDef->ItemID, 1, 0);
	z_role_machining_oplog(pstEnv, pstPlayer, &stOldRoleArm, &pstBeExtended->GridData.RoleArm, 
			&stExpendData, OP_MACHINING_ARM_EXT, 1, NULL,0);

	// 削减附魔卷轴
	if(pstBeUsed && package_dec(pstAppCtx, pstEnv, pstPlayer,
	               pstBeUsed->GridData.RoleItem.DefID, 1,
	               pstReq->BeUsed.ListType, &iBeUsedPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_ARM_FUMO) < 0)
	{
		goto error;
	}
	
final:
	pstRes->BeExtended = pstReq->BeExtended;
	if(pstRes->Succeeded)
	{
		pstRes->FromItemID = pstPlayer->stOnline.stExtendedOL.ExtendedFromDefID;
		pstRes->Result = pstPlayer->stOnline.stExtendedOL.ExtendedResult;
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, EXTEND_ITEM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;

error:
	pstRes->Succeeded = 0;
	goto final;
	return 0;
}

int player_extend_item_save(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
                       Player *pstPlayer, TFRAMEHEAD *pstFrameHead,
                       CSPKG *pstCsPkg)
{
	EXTENDITEMSAVE *pstReq = &pstCsPkg->Body.ExtendItemSave;
	ROLEPACKAGE *pstPackage = &pstPlayer->stRoleData.Package;
	ROLEGRID *pstBeExtended = NULL;
	ITEMDEF *pstBeExtendedDef = NULL;
	ROLEARM stOldRoleArm;
	ExtendedOL *pstExtendedOL = &pstPlayer->stOnline.stExtendedOL;

	switch(pstReq->BeExtended.ListType)
	{		
	case LIST_TYPE_SUNDRIES:
		{
			int iPos = package_grid_pos(pstPackage->SundriesPackage.RoleGrids,
				                        pstPackage->SundriesPackage.Num,
				                        pstReq->BeExtended.GridIdx);
			if(iPos < 0)
			{
				goto _error;
			}

			pstBeExtended = pstPackage->SundriesPackage.RoleGrids + iPos;
			if(pstBeExtended->GridData.RoleItem.WID != pstReq->BeExtended.WID ||
			   pstBeExtended->GridData.RoleItem.GridIdx !=
			   pstReq->BeExtended.GridIdx)
			{
				goto _error;
			}

			pstBeExtendedDef = z_get_itemdef_roleitem(pstEnv,
			                                 &pstBeExtended->GridData.RoleItem);
			if(!pstBeExtendedDef)
			{
				goto _error;
			}
			if(pstBeExtended->Type != GRID_ITEM_ARM)
			{
				goto _error;
			}
		}
		break;

	default:
		goto _error;
		break;
	}

	if(pstExtendedOL->WID != pstBeExtended->GridData.RoleArm.WID)
	{
		goto _error;
	}

	if(pstExtendedOL->ExtendedFromDefID != pstReq->FromItemID ||
		pstExtendedOL->ExtendedResult.ResultID != pstReq->Result.ResultID ||
		pstExtendedOL->ExtendedResult.ResultVal1 != pstReq->Result.ResultVal1 ||
		pstExtendedOL->ExtendedResult.ResultVal2 != pstReq->Result.ResultVal2 ||
		pstExtendedOL->ExtendedResult.ResultVal3 != pstReq->Result.ResultVal3 ||
		pstExtendedOL->ExtendedResult.ResultVal4 != pstReq->Result.ResultVal4)
	{
		goto _error;
	}

	memcpy(&stOldRoleArm, &pstBeExtended->GridData.RoleArm, sizeof(stOldRoleArm));

	pstBeExtended->GridData.RoleArm.ExtendedFromDefID = pstExtendedOL->ExtendedFromDefID;
	pstBeExtended->GridData.RoleArm.ExtendedResult = pstExtendedOL->ExtendedResult;
	memset(pstExtendedOL, 0, sizeof(*pstExtendedOL));
	
	package_grid_update_to_clt(pstAppCtx, pstEnv, pstPlayer,
	                           pstReq->BeExtended.ListType, NULL,
	                           &pstBeExtended->GridData.RoleArm);

	z_role_machining_oplog(pstEnv, pstPlayer, &stOldRoleArm, &pstBeExtended->GridData.RoleArm, 
			NULL, OP_MACHINING_ARM_EXT_SAVE, 1, NULL,0);
	return 0;
_error:
	return -1;
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

int package_cangbaotu_get_pos(ZONESVRENV *pstEnv,Player *pstPlayer, unsigned int ItemID,
										unsigned  int *piMapID,RESPOS *pstPos)
{
	int i = 0;
	int iCangBaoTuDef = 0;
	CANGBAOTUDEF  stCangBaoTuDef[MAX_CANGBAOTU_DEF];
	CANGBAOTUDEF *pstCangBaoTuDef = NULL;
	int iPosNum = 0;

	memset(stCangBaoTuDef,0,sizeof(CANGBAOTUDEF)*MAX_CANGBAOTU_DEF);
	for(i=0;i<pstEnv->pstZoneObj->iCangBaoTuDef;i++)
	{
	  	pstCangBaoTuDef = (CANGBAOTUDEF *)pstEnv->pstZoneObj->sCangBaoTuDef[i];
		if( pstCangBaoTuDef->ItemID == ItemID && 
			pstPlayer->stRoleData.Level >= pstCangBaoTuDef->Level )
		{
			stCangBaoTuDef[iCangBaoTuDef++]=*pstCangBaoTuDef;
		}
	}

	if( iCangBaoTuDef <=0 )
	{
		//没有这个藏宝图的配置
		return -1;
	}

	// 把行取出来
	iCangBaoTuDef = (int)RAND1(iCangBaoTuDef);

	for( i=0; i<CANGBAOTUPOS_NUM; i++ )
	{
		if( stCangBaoTuDef[iCangBaoTuDef].Pos[i].X==0 || stCangBaoTuDef[iCangBaoTuDef].Pos[i].Y == 0 )
		{
			break;
		}
		iPosNum++;	
	}

	if( iPosNum <= 0)
	{
		//没有配置坐标,纠结
		return -1;
	}
	iPosNum  = (int)RAND1(iPosNum);

	*pstPos = stCangBaoTuDef[iCangBaoTuDef].Pos[iPosNum];
	*piMapID = stCangBaoTuDef[iCangBaoTuDef].MapID;
	
	return 0;
}

int package_cangbaotu_check(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	int i = 0;
	CANGBAOTUINFO *pstCangBaoTuInfo; 
	unsigned  int iMapID = 0;
	RESPOS stPos;

	pstCangBaoTuInfo = &pstPlayer->stRoleData.MiscInfo.CangBaoTuInfo;

	for(i=0; i<CANGBAOTU_NUM;i++)
	{
		if( pstCangBaoTuInfo->CangBaoTuMapInfo[i].ItemID == 0 ||
			pstCangBaoTuInfo->CangBaoTuMapInfo[i].MapID == 0 ||
			pstCangBaoTuInfo->CangBaoTuMapInfo[i].Pos.Y== 0 ||
			pstCangBaoTuInfo->CangBaoTuMapInfo[i].Pos.X== 0 )
		{
			memset(&stPos,0,sizeof(stPos));
			iMapID = 0;
			if(i==0)
			{
				package_cangbaotu_get_pos(pstEnv,pstPlayer, CANGBAOTU_LOW,&iMapID,&stPos);
				pstCangBaoTuInfo->CangBaoTuMapInfo[i].ItemID = CANGBAOTU_LOW;
				pstCangBaoTuInfo->CangBaoTuMapInfo[i].MapID = iMapID;
				pstCangBaoTuInfo->CangBaoTuMapInfo[i].Pos = stPos;
			}
			else if(i==1)
			{
				package_cangbaotu_get_pos(pstEnv,pstPlayer, CANGBAOTU_MID,&iMapID,&stPos);
				pstCangBaoTuInfo->CangBaoTuMapInfo[i].ItemID = CANGBAOTU_MID;
				pstCangBaoTuInfo->CangBaoTuMapInfo[i].MapID = iMapID;
				pstCangBaoTuInfo->CangBaoTuMapInfo[i].Pos = stPos;
			}
			else
			{
				package_cangbaotu_get_pos(pstEnv,pstPlayer, CANGBAOTU_HIGH,&iMapID,&stPos);
				pstCangBaoTuInfo->CangBaoTuMapInfo[i].ItemID = CANGBAOTU_HIGH;
				pstCangBaoTuInfo->CangBaoTuMapInfo[i].MapID = iMapID;
				pstCangBaoTuInfo->CangBaoTuMapInfo[i].Pos = stPos;
			}			
		}
	}

	return 0;
}

int package_cangbaotu_reset(ZONESVRENV *pstEnv,Player *pstPlayer, CSPACKAGECANGBAOTURESET *pstReset)
{
	RESULT *pstVipResult;
	int iVip = 0;
	CANGBAOTUINFO *pstCangBaoTuInfo; 
	unsigned  int iMapID = 0;
	RESPOS stPos;
	pstCangBaoTuInfo = &pstPlayer->stRoleData.MiscInfo.CangBaoTuInfo;

	memset(&stPos,0,sizeof(stPos));
	iMapID = 0;

	pstVipResult = star_get_result(pstEnv, pstPlayer, STAR_RESULT_CANGBAOTU);
	if (pstVipResult)
	{
		iVip = pstVipResult->ResultVal1;
	}
	
	if(pstCangBaoTuInfo->DayTimes+iVip <= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, 
							ZONEERR_PAK261, CANGBAOTU_DAY_TIMES);
		return -1;
	}
	
	if( pstReset->ItemID == CANGBAOTU_LOW &&
		0 == package_cangbaotu_get_pos(pstEnv,pstPlayer, CANGBAOTU_LOW,&iMapID,&stPos) )
	{
		pstCangBaoTuInfo->CangBaoTuMapInfo[0].ItemID = CANGBAOTU_LOW;
		pstCangBaoTuInfo->CangBaoTuMapInfo[0].MapID = iMapID;
		pstCangBaoTuInfo->CangBaoTuMapInfo[0].Pos = stPos;
	}
	else if( pstReset->ItemID == CANGBAOTU_MID &&
		0 == package_cangbaotu_get_pos(pstEnv,pstPlayer, CANGBAOTU_MID,&iMapID,&stPos) )
	{
		pstCangBaoTuInfo->CangBaoTuMapInfo[1].ItemID = CANGBAOTU_MID;
		pstCangBaoTuInfo->CangBaoTuMapInfo[1].MapID = iMapID;
		pstCangBaoTuInfo->CangBaoTuMapInfo[1].Pos = stPos;
	}
	else if( pstReset->ItemID == CANGBAOTU_HIGH &&
		0 == package_cangbaotu_get_pos(pstEnv,pstPlayer, CANGBAOTU_HIGH,&iMapID,&stPos) )
	{
		pstCangBaoTuInfo->CangBaoTuMapInfo[2].ItemID = CANGBAOTU_HIGH;
		pstCangBaoTuInfo->CangBaoTuMapInfo[2].MapID = iMapID;
		pstCangBaoTuInfo->CangBaoTuMapInfo[2].Pos = stPos;
	}

	pstCangBaoTuInfo->DayTimes--;
	package_cangbaotu_send(pstEnv,pstPlayer);
	
	return 0;
}

int package_cangbaotu_send( ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG *pstPkgRes = &pstEnv->stCSRes;
	CSPACKAGEINFO *pstPakInfo = &pstPkgRes->Body.PackageInfo;
	CANGBAOTUINFO *pstCangBaoTuInfo = &pstPlayer->stRoleData.MiscInfo.CangBaoTuInfo;
	
	pstPakInfo->Type = PACKAGE_CANGBAOTU_RESET;
	pstPakInfo->PackageInfoData.CangBaoTuInfo = *pstCangBaoTuInfo;

	Z_CSHEAD_INIT(&pstPkgRes->Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkgRes);
	
	return 0;
}
int package_cangbaotu_tick( ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CANGBAOTUINFO *pstCangBaoTuInfo = &pstPlayer->stRoleData.MiscInfo.CangBaoTuInfo;
	
	if (0 == IsSameDay(pstCangBaoTuInfo->UpdateDayTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstCangBaoTuInfo->DayTimes = CANGBAOTU_DAY_TIMES;
		pstCangBaoTuInfo->UpdateDayTime = pstEnv->pstAppCtx->stCurr.tv_sec;

		package_cangbaotu_send( pstEnv, pstPlayer);
	}	

	return 0;
}


int package_maxnum_by_type(ROLEDATA *pstRoleData, int iListType)
{
	int iRet = 0;
	ROLEPACKAGE *pstPak = &pstRoleData->Package;

	switch(iListType)
	{
		case LIST_TYPE_STASH:
			iRet = pstPak->StashPackage.MaxNum;
			break;
		case LIST_TYPE_SUNDRIES:
			iRet = pstPak->SundriesPackage.MaxNum;
			break;
		case LIST_TYPE_STUFF:
			iRet = pstPak->StuffPackage.MaxNum;
			break;
		case LIST_TYPE_TASK:
			iRet = pstPak->TaskPackage.MaxNum;
			break;
		default:
			return 0;
			break;
	}
	
	return iRet;
}

int package_item_info_by_type(ROLEDATA *pstRoleData, int iListType,  SSGETPACKAGEITEMINFO *pstGetPackageItemInfo)
{
	ROLEPACKAGE *pstPak = &pstRoleData->Package;

	switch(iListType)
	{
		case LIST_TYPE_STASH:
			pstGetPackageItemInfo->StashPackage =  pstPak->StashPackage;
			break;
		case LIST_TYPE_SUNDRIES:
			pstGetPackageItemInfo->SundriesPackage =pstPak->SundriesPackage;
			break;
		case LIST_TYPE_STUFF:
			pstGetPackageItemInfo->StuffPackage = pstPak->StuffPackage;
			break;
		case LIST_TYPE_TASK:
			pstGetPackageItemInfo->TaskPackage = pstPak->TaskPackage;
			break;
		case LIST_TYPE_SHOPPING:
			pstGetPackageItemInfo->ShoppingPackage = pstPak->ShoppingPackage;
			break;
		default:
			return 0;
			break;
	}
	
	return 0;
}

int role_pkg_add_grid(ZONESVRENV* pstEnv, Player* pstPlayer, int iPkgType, int iNum)
{
	ROLEPACKAGE *pstPkg = &pstPlayer->stRoleData.Package;
	int iOldNum;
	
	if (iPkgType == LIST_TYPE_STASH)
	{
		iOldNum = pstPkg->StashPackage.MaxNum;
		pstPkg->StashPackage.MaxNum += iNum;
		if (pstPkg->StashPackage.MaxNum > MAX_STASH_PACKAGE)
		{
			pstPkg->StashPackage.MaxNum = MAX_STASH_PACKAGE;
		}
		
		if (iOldNum != pstPkg->StashPackage.MaxNum)
		{
			pstPkg->StashPackage.BaseNum += 
					(pstPkg->StashPackage.MaxNum-iOldNum);
			role_pkg_num_chg(pstEnv, pstPlayer, LIST_TYPE_STASH);
		}
		
	}
	else if(iPkgType == LIST_TYPE_SUNDRIES)
	{
		iOldNum = pstPkg->SundriesPackage.MaxNum;
		pstPkg->SundriesPackage.MaxNum += iNum;
		if (pstPkg->SundriesPackage.MaxNum > MAX_SUNDRIES_PACKAGE)
		{
			pstPkg->SundriesPackage.MaxNum = MAX_SUNDRIES_PACKAGE;
		}

		if (iOldNum != pstPkg->SundriesPackage.MaxNum)
		{
			pstPkg->SundriesPackage.BaseNum += 
					(pstPkg->SundriesPackage.MaxNum-iOldNum);
			role_pkg_num_chg(pstEnv, pstPlayer, LIST_TYPE_SUNDRIES);
		}
	}
	else if(iPkgType == LIST_TYPE_TASK)
	{
		return -1;
	}
	else if (iPkgType == LIST_TYPE_STUFF)
	{
		return -1;
	}
	else
	{
		return -1;
	}
	
	return 0;
}

int level_up_add_grid(ZONESVRENV* pstEnv, Player* pstPlayer)
{
	int i;
	char szPkg[16];
	for (i=0; i<(int)(sizeof(ToWardGrids)/sizeof(ToWardGrid)); i++)
	{
		if (ToWardGrids[i].iLevel == pstPlayer->stRoleData.Level)
		{
			role_pkg_add_grid(pstEnv, pstPlayer, ToWardGrids[i].iType, ToWardGrids[i].iGird);
			if (ToWardGrids[i].iType == LIST_TYPE_SUNDRIES)
			{
				strcpy(szPkg,"杂物包");
			}
			else if(ToWardGrids[i].iType == LIST_TYPE_STUFF)
			{
				strcpy(szPkg,"材料包");
			}
			else
			{
				continue;
			}
			
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PAK262,szPkg,ToWardGrids[i].iGird);
		}
	}
	return 0;
}

int get_pkg_basenum(ZONESVRENV* pstEnv, Player* pstPlayer, int iListType)
{
	int iBaseNum = -1;
	UNUSED(pstEnv);
	UNUSED(pstPlayer);

	if (iListType == LIST_TYPE_SUNDRIES)
	{
		iBaseNum = INIT_SUNDRIES_PACKAGE;
	}
	else if (iListType == LIST_TYPE_STUFF)
	{
		iBaseNum = INIT_STUFF_PACKAGE;
	}
	else if (iListType == LIST_TYPE_STASH)
	{
		iBaseNum = INIT_STASH_PACKAGE;
	}

	return iBaseNum;
}

int role_pkg_num_chg(ZONESVRENV* pstEnv, Player* pstPlayer, int iPkgType)
{
	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;

	if (iPkgType == LIST_TYPE_SUNDRIES)
	{
		pstPackageInfo->Type = PACKAGE_SUNDRIES_NUM_CHG;
		pstPackageInfo->PackageInfoData.SundriesNum = pstPlayer->stRoleData.Package.SundriesPackage.MaxNum;
	}
	else if(iPkgType == LIST_TYPE_STASH)
	{
		pstPackageInfo->Type = PACKAGE_STASH_NUM_CHG;
		pstPackageInfo->PackageInfoData.StashNum = pstPlayer->stRoleData.Package.StashPackage.MaxNum;
	}
	else if(iPkgType == LIST_TYPE_TASK)
	{
		pstPackageInfo->Type = PACKAGE_TASK_NUM_CHG;
		pstPackageInfo->PackageInfoData.TaskNum = pstPlayer->stRoleData.Package.TaskPackage.MaxNum;
	}
	else if (iPkgType == LIST_TYPE_STUFF)
	{
		return -1;
		//pstPackageInfo->Type = PACKAGE_STUFF_NUM_CHG;
		//pstPackageInfo->PackageInfoData.StuffNum = pstPlayer->stRoleData.Package.StuffPackage.MaxNum;
	}
	else if (iPkgType == LIST_TYPE_ET_SUND)
	{
		pstPackageInfo->Type = PACKAGE_ET_NUM;
		pstPackageInfo->PackageInfoData.EtPkgNum.PkgType = 1;
		pstPackageInfo->PackageInfoData.EtPkgNum.MaxNum = pstPlayer->stRoleData.Package.EtSundPkg.MaxNum;
		pstPackageInfo->PackageInfoData.EtPkgNum.StuffNum = pstPlayer->stRoleData.Package.EtSundPkg.StuffNum;
	}
	else if (iPkgType == LIST_TYPE_ET_STASH)
	{
		pstPackageInfo->Type = PACKAGE_ET_NUM;
		pstPackageInfo->PackageInfoData.EtPkgNum.PkgType = 2;
		pstPackageInfo->PackageInfoData.EtPkgNum.MaxNum = pstPlayer->stRoleData.Package.EtStashPkg.MaxNum;
	}
	else
	{
		return -1;
	}

	Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_daily_bull_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkgRes;
	CSACTIONRES *pstActionRes = &stPkgRes.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_DAILY_BULL;
	pstActionRes->Data.DailyBull.Bull = pstPlayer->stRoleData.MiscInfo.DailyUseBull;

	Z_CSHEAD_INIT(&stPkgRes.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);

	return 0;
}

//假死取消
int player_sham_dead_clear(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkgRes;
	CSACTIONRES *pstActionRes = &stPkgRes.Body.ActionRes;

	if(pstPlayer->stOnline.fShamDead == 0)
	{
		return 0;
	}
	
	pstPlayer->stOnline.fShamDead = 0;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_SHAM_DEAD;
	pstActionRes->Data.ShamDead.Dead = 0;

	Z_CSHEAD_INIT(&stPkgRes.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkgRes, 0);
	return 0;
}

//假死
int player_sham_dead(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkgRes;
	CSACTIONRES *pstActionRes = &stPkgRes.Body.ActionRes;

	pstPlayer->stOnline.fShamDead = 1;
	player_view_mon_delpress(pstEnv, pstPlayer);

	//打断一下吟唱
	player_trace_act_break_prepare(pstEnv, pstPlayer, BRK_PRE_ACT_SHAM_DEAD);

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_SHAM_DEAD;
	pstActionRes->Data.ShamDead.Dead = 1;

	Z_CSHEAD_INIT(&stPkgRes.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkgRes, 0);
	return 0;
}

//彩色字体时效
int player_color_font_limit_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkgRes;
	CSACTIONRES *pstActionRes = &stPkgRes.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_COLOR_FONT_LIMIT;
	pstActionRes->Data.ColorFontLimit.ColorFontEnd = pstPlayer->stRoleData.MiscInfo.ColorFontEnd;

	Z_CSHEAD_INIT(&stPkgRes.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);
	return 0;
}

//动作表情时效
int player_motion_limit_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkgRes;
	CSACTIONRES *pstActionRes = &stPkgRes.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_MOTION_LIMIT;
	pstActionRes->Data.MotionLimit.MotionEnd = pstPlayer->stRoleData.MiscInfo.MotionEnd;

	Z_CSHEAD_INIT(&stPkgRes.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);
	return 0;
}

int package_use_color_font_limit(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult)
{
	if(pstPlayer->stRoleData.MiscInfo.ColorFontEnd > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		pstPlayer->stRoleData.MiscInfo.ColorFontEnd += pstResult->ResultVal1*86400;
	}
	else
	{
		pstPlayer->stRoleData.MiscInfo.ColorFontEnd = pstEnv->pstAppCtx->stCurr.tv_sec + pstResult->ResultVal1*86400;
	}

	player_color_font_limit_action(pstEnv, pstPlayer);
	return 0;
}

int package_use_motion_limit(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult)
{
	if(pstPlayer->stRoleData.MiscInfo.MotionEnd > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		pstPlayer->stRoleData.MiscInfo.MotionEnd += pstResult->ResultVal1*86400;
	}
	else
	{
		pstPlayer->stRoleData.MiscInfo.MotionEnd = pstEnv->pstAppCtx->stCurr.tv_sec + pstResult->ResultVal1*86400;
	}
	
	player_motion_limit_action( pstEnv, pstPlayer);
	return 0;
}

int package_use_clan_contri_check(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEITEM *pstRoleItem)
{
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	if(!pstClan)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PACKAGE_312);
		return -1;
	}
	
	return 0;
	UNUSED(pstRoleItem);	//add by paraunused.pl
}

int package_use_clan_contri(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEITEM *pstRoleItem)
{
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	if(!pstClan)
	{
		return -1;
	}
	
	g_iOptype = OP_OPTYPE_CONTRI_ITEM;
	clan_resource_inc(pstEnv, pstClan, pstPlayer, CLAN_OWNED_CONTRIBUT,
		pstRoleItem->InstVal1);
	g_iOptype = 0;
		
	return 0;
}

int check_buddy_val(ZONESVRENV* pstEnv, Player *pstPlayer,Player *pstTempPlayer,ITEMDEF *pstItemDef)
{
	int ret;
	int ret2;
	RESULT *pstResult;
	char szSelf[ROLE_NAME_LEN];
		
	pstResult = z_item_get_result(pstItemDef, RESULT_BUDDY_VAL);
	if(!pstResult)
	{
		return 0;
	}

	if(pstEnv->iInstMode == INST_MODE_BATTLE_SVR || 
		PLAYER_STATUS_ROLE_NOTHING == pstPlayer->eStatus || 
		PLAYER_STATUS_ROLE_NOTHING == pstTempPlayer->eStatus)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG19);
		return -1;
	}

	STRNCPY(szSelf, pstTempPlayer->stRoleData.RoleName,sizeof(szSelf));
	battle_rolename_remove_prefix(szSelf);
			
	ret = online_info_find_friend_byID(&pstPlayer->stRoleData.Buddy, pstTempPlayer->stRoleData.RoleID);
	if(ret < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING1,
		szSelf,pstItemDef->Name);
		return -1;
	}
	
	ret2 = online_info_find_friend_byID(&pstTempPlayer->stRoleData.Buddy, pstPlayer->stRoleData.RoleID);
	if(ret2 < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING2,
		szSelf,pstItemDef->Name);
		return -1;
	}
		
	if(pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	}
	
	if(pstTempPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstTempPlayer, 0);
	}
	
	if(z_distance(&pstPlayer->stRoleData.Pos, &pstTempPlayer->stRoleData.Pos) > 2000)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING3,
		szSelf,pstItemDef->Name);
		return -1;
	}

	if(pstPlayer->stRoleData.Buddy.Friends[ret].BuddyVal >= MAX_BUDDY_VAL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING66,szSelf);
		return -1;
	}

	if(pstTempPlayer->stRoleData.Buddy.Friends[ret2].BuddyVal >= MAX_BUDDY_VAL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING66,szSelf);
		return -1;
	}
	
	return 0;
}

int check_pursue_item(ZONESVRENV* pstEnv, Player *pstPlayer,Player *pstTempPlayer,ITEMDEF *pstItemDef)
{
	int ret;
	int ret2;
	char szSelf[ROLE_NAME_LEN];
	int ret3 = 0;
	int ret4 = 0;
		
	STRNCPY(szSelf, pstTempPlayer->stRoleData.RoleName,sizeof(szSelf));
	battle_rolename_remove_prefix(szSelf);
			
	ret3 = online_info_find_friend_byID(&pstPlayer->stRoleData.Buddy, pstTempPlayer->stRoleData.RoleID);
	if(ret3 < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING1,
		szSelf,pstItemDef->Name);
		return -1;
	}
	
	ret4 = online_info_find_friend_byID(&pstTempPlayer->stRoleData.Buddy, pstPlayer->stRoleData.RoleID);
	if(ret4 < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING2,
		szSelf,pstItemDef->Name);
		return -1;
	}
		
	if(pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	}
	
	if(pstTempPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstTempPlayer, 0);
	}
	
	if(z_distance(&pstPlayer->stRoleData.Pos, &pstTempPlayer->stRoleData.Pos) > 2000)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING3,
		szSelf,pstItemDef->Name);
		return -1;
	}
	
	ret = find_lover_byID(&pstPlayer->stRoleData.Buddy,pstTempPlayer->stRoleData.RoleID);
	ret2 = find_lover_byID(&pstTempPlayer->stRoleData.Buddy, pstPlayer->stRoleData.RoleID);
	//双方互为情侣
	if(ret >= 0 && ret2 >= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING4,pstItemDef->Name);
		return -1;
	}
	
	ret = find_spouse_byID(&pstPlayer->stRoleData.Buddy,pstTempPlayer->stRoleData.RoleID);
	ret2 = find_spouse_byID(&pstTempPlayer->stRoleData.Buddy, pstPlayer->stRoleData.RoleID);
	//双方互为夫妻
	if(ret >= 0 && ret2 >= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING5,pstItemDef->Name);
		return -1;
	}

	ret = find_lover_from_friend_list(&pstPlayer->stRoleData.Buddy);
	ret2 = find_spouse_from_friend_list(&pstPlayer->stRoleData.Buddy);
	//自己当前已建立情侣夫妻关系
	if(ret >= 0 || ret2 >= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING7,pstItemDef->Name);
		return -1;
	}
	
	ret = find_lover_from_friend_list(&pstTempPlayer->stRoleData.Buddy);
	ret2 = find_spouse_from_friend_list(&pstTempPlayer->stRoleData.Buddy);
	//对方当前已建立情侣夫妻关系
	if(ret >= 0 || ret2 >= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING6,szSelf,pstItemDef->Name);
		return -1;
	}

	//达不到亲密度最小值，不能表白
	if(pstPlayer->stRoleData.Buddy.Friends[ret3].BuddyVal < PURSUE_MIN_BUDDY_VAL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING60,PURSUE_MIN_BUDDY_VAL);
		return -1;
	}

	if(pstTempPlayer->stRoleData.Buddy.Friends[ret4].BuddyVal < PURSUE_MIN_BUDDY_VAL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING60,PURSUE_MIN_BUDDY_VAL);
		return -1;
	}

	if(pstTempPlayer->stOnline.ullLoveID == pstPlayer->stRoleData.RoleID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING67,szSelf,pstItemDef->Name);
		pstPlayer->stOnline.ullLoveID = pstTempPlayer->stRoleData.RoleID;
		pstPlayer->stOnline.cPursueName[0] = 0;
		STRNCPY(pstPlayer->stOnline.cPursueName,pstTempPlayer->stRoleData.RoleName,sizeof(pstPlayer->stOnline.cPursueName));
		relation_involve_marry_svr(pstEnv,pstPlayer,pstTempPlayer->stRoleData.RoleName,TYPE_PURSUE_INFO_INFORM_SVR);
		return -1;
	}
	
	return 0;
}

GEMPAGDEF *gem_pag_def_find(ZONESVRENV* pstEnv, int Id)
{
	GEMPAGDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Id = Id;
	iIndex = bsearch_int(&stDef, pstObj->sGemPagDef, pstObj->iGemPagDef, GEM_PAG_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (GEMPAGDEF *)&pstObj->sGemPagDef[iIndex];
	}
	
	return NULL;
}

int item_use_gem_pag_check(ZONESVRENV* pstEnv, Player *pstPlayer,CSPACKAGEUSE *pstUse,int ID)
{
	GEMPAGDEF *pstDef;
	int iIdx = (int)pstUse->SubIndex-1;
	int iNum = 0;
	ROLEGRID stRoleGrid;
		
	pstDef = gem_pag_def_find(pstEnv, ID);
	if (!pstDef)
		return -1;

	if (iIdx < 0 ||iIdx >= MAX_GEM_PAG_GEM_NUM)
	{
		return -1;
	}

	if (pstDef->GemInfo[iIdx].ItemID <= 0)
	{
		return -1;
	}

	iNum = pstDef->GemInfo[iIdx].Num;
	if(iNum <= 0)
	{
		return -1;
	}

	if(0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstDef->GemInfo[iIdx].ItemID))
	{
		return -1;
	}

	stRoleGrid.GridData.RoleItem.Num = iNum;

	if(0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1, 0))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
		return -1;
	}

	return 0;
}

//使用宝石包获得道具
int item_use_gem_pag(ZONESVRENV* pstEnv, Player *pstPlayer, CSPACKAGEUSE *pstUse,int ID)
{
	int iIdx = pstUse->SubIndex-1;
	ROLEGRID stGrid;
	GEMPAGDEF *pstDef;
	
	pstDef = gem_pag_def_find(pstEnv, ID);
	if (!pstDef)
		return -1;

	if (iIdx < 0 ||iIdx >= MAX_GEM_PAG_GEM_NUM)
	{
		return -1;
	}

	if (pstDef->GemInfo[iIdx].ItemID <= 0)
	{
		return -1;
	}

	if (item_create(pstEnv->pstAppCtx, pstEnv, &stGrid, pstDef->GemInfo[iIdx].ItemID)  < 0)
	{
		return -1;	
	}

	stGrid.GridData.RoleItem.Num = pstDef->GemInfo[iIdx].Num ;
	
	if(pstDef->GemInfo[iIdx].Bind == 1)
	{
		stGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
	}
	
	package_add(pstEnv->pstAppCtx,pstEnv,pstPlayer, &stGrid, 1, 0, IMPITEM_OPTYPE_GEM_PAG);

	z_role_pickitem_oplog(pstEnv, pstPlayer, &stGrid, 1, OP_ITEM_RESULT);
	
	return 0;
}

//脱下来装备更新身上记录的技能冷却信息
int player_unwear_skill_cool_update(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,ZoneSkill *pstZoneSkill)
{
	int i = 0;
	WEARSKILLCOOLINFO *pstWearSkillCoolInfo = NULL;
	int iNum = 0;
	SKILLCOOLINFO stSkillCoolInfo;
	
	if(pstZoneSkill->stCoolEnd.tv_sec > pstAppCtx->stCurr.tv_sec)
	{
		//冷却时间小于RECORD_WEAR_SKILL_COOL_TIME 就算了
		if(pstZoneSkill->stCoolEnd.tv_sec - pstAppCtx->stCurr.tv_sec <= RECORD_WEAR_SKILL_COOL_TIME)
		{
			return 0;
		}
		else
		{
			pstWearSkillCoolInfo = &pstPlayer->stRoleData.MiscInfo.WearSkillCoolInfo;
			iNum = pstWearSkillCoolInfo->Num;
			if(iNum > MAX_PLAYER_WEAR_SKILL_COOL_NUM)
			{
				return -1;
			}

			for(i=iNum-1;i>=0;i--)
			{
				if(pstZoneSkill->stSkillData.SkillID == pstWearSkillCoolInfo->SkillCoolInfo[i].SkillID
				&& pstZoneSkill->stSkillData.SkillLevel == pstWearSkillCoolInfo->SkillCoolInfo[i].SkillLevel)
				{
					break;
				}
			}

			memset(&stSkillCoolInfo,0,sizeof(stSkillCoolInfo));
			stSkillCoolInfo.SkillID = pstZoneSkill->stSkillData.SkillID;
			stSkillCoolInfo.SkillLevel = pstZoneSkill->stSkillData.SkillLevel;
			stSkillCoolInfo.CoolLeft = pstZoneSkill->stCoolEnd.tv_sec - pstAppCtx->stCurr.tv_sec;
			
			if(i<0)
			{
				//增加	
				if(pstWearSkillCoolInfo->Num >= MAX_PLAYER_WEAR_SKILL_COOL_NUM)
				{
					memmove(&pstWearSkillCoolInfo->SkillCoolInfo[0], &pstWearSkillCoolInfo->SkillCoolInfo[1],sizeof(pstWearSkillCoolInfo->SkillCoolInfo[0])*(MAX_PLAYER_WEAR_SKILL_COOL_NUM-1));
					memcpy(&pstWearSkillCoolInfo->SkillCoolInfo[MAX_PLAYER_WEAR_SKILL_COOL_NUM-1],&stSkillCoolInfo,sizeof(pstWearSkillCoolInfo->SkillCoolInfo[MAX_PLAYER_WEAR_SKILL_COOL_NUM-1]));
				}
				else
				{
					memcpy(&pstWearSkillCoolInfo->SkillCoolInfo[pstWearSkillCoolInfo->Num],&stSkillCoolInfo,sizeof(pstWearSkillCoolInfo->SkillCoolInfo[pstWearSkillCoolInfo->Num]));
					pstWearSkillCoolInfo->Num++;
				}
			}
			else
			{
				//替换
				pstWearSkillCoolInfo->Num--;
				if(pstWearSkillCoolInfo->Num != i)
				{
					memmove(&pstWearSkillCoolInfo->SkillCoolInfo[i], &pstWearSkillCoolInfo->SkillCoolInfo[i+1],sizeof(pstWearSkillCoolInfo->SkillCoolInfo[0])*(pstWearSkillCoolInfo->Num-i));	
				}
			
				memcpy(&pstWearSkillCoolInfo->SkillCoolInfo[pstWearSkillCoolInfo->Num],&stSkillCoolInfo,sizeof(pstWearSkillCoolInfo->SkillCoolInfo[pstWearSkillCoolInfo->Num]));
				pstWearSkillCoolInfo->Num++;
			}	
		}
	}

	return 0;
}

//穿戴装备时，相关技能冷却时间更新(假如身上有记录的话)
int player_wear_skill_cool_update(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	ZoneSkill *pstZoneSkill = NULL;
	unsigned int uiCoolLeft;
	int ret;

	for (i = 0; i <MAX_ARM_ADD_SKILL; i++)
	{
		if ( pstDyn->astAddSkill[i].unSkillID == 0  ||  pstDyn->astAddSkill[i].ucLevel == 0)
		{
			break;
		}
		
		pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstDyn->astAddSkill[i].unSkillID);
		if (!pstZoneSkill)
		{
			continue;
		}
		
		ret = player_wear_skill_cool_left(pstAppCtx, pstEnv,pstPlayer,pstZoneSkill, &uiCoolLeft);
		if(ret > 0)
		{
			pstZoneSkill->stCoolEnd.tv_sec = pstAppCtx->stCurr.tv_sec + uiCoolLeft;
			z_skill_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstZoneSkill);
		}
	}

	return 0;
}

int player_wear_skill_cool_left(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,ZoneSkill *pstZoneSkill, unsigned int *puiCoolLeft)
{
	int i;
	WEARSKILLCOOLINFO *pstWearSkillCoolInfo = &pstPlayer->stRoleData.MiscInfo.WearSkillCoolInfo;
	int flag = 0;
	unsigned int uiTempCool = 0;
	int iNum = 0;
	iNum = pstWearSkillCoolInfo->Num;

	if(iNum > MAX_PLAYER_WEAR_SKILL_COOL_NUM)
	{
		return 0;
	}

	for(i=iNum-1;i>=0;i--)
	{
		if(pstZoneSkill->stSkillData.SkillID == pstWearSkillCoolInfo->SkillCoolInfo[i].SkillID
				&& pstZoneSkill->stSkillData.SkillLevel == pstWearSkillCoolInfo->SkillCoolInfo[i].SkillLevel)
		{
			uiTempCool = pstWearSkillCoolInfo->SkillCoolInfo[i].CoolLeft;
			flag = 1;
			break;
		}
	}

	if(flag == 1)
	{
		pstWearSkillCoolInfo->Num--;
		if(i != pstWearSkillCoolInfo->Num)
		{
			memmove(&pstWearSkillCoolInfo->SkillCoolInfo[i], &pstWearSkillCoolInfo->SkillCoolInfo[i+1],sizeof(pstWearSkillCoolInfo->SkillCoolInfo[0])*(pstWearSkillCoolInfo->Num-i));	
		}

		*puiCoolLeft = uiTempCool;
		return 1;
	}

	return 0;
}


