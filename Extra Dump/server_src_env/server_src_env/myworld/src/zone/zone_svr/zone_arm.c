/*
**  @file $RCSfile: zone_arm.c,v $
**  general description of this module
**  $Id: zone_arm.c,v 1.535 2014/06/30 02:31:36 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2014/06/30 02:31:36 $
**  @version $Revision: 1.535 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_arm.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "zone_clt.h"
#include "zone_gm.h"
#include "zone_package.h"
#include "zone_act_status.h"
#include "zone_attr.h"
#include "zone_oplog.h"
#include "zone_task.h"
#include "zone_db.h"
#include "zone_move.h"
#include "zone_mail.h"
#include "zone_shop.h"
#include "zone_player.h"
#include "zone_safelock.h"
#include "zone_addattr.h"
#include "zone_genius.h"
#include "zone_err.h"
#include "zone_status.h"

char g_cResultFlag = 0;

int arm_skill_career_get(ARMSKILLATTRDEF *pstAttrDef, int iCareer);
int z_wear_get_star_perfect_num(ZONESVRENV* pstEnv, Player *pstPlayer);
void arm_cacl_star_mul(ZONESVRENV* pstEnv,DYNATTRLIST *pstDyn, ITEMDEF *pstItemDef,ROLEARM *pstRoleArm, RESULT *pstResult);

ARMLEVELUPDEF *z_find_arm_levelup_def(ZONESVRENV *pstEnv, int iItemID)
{
	int iIndex;
	int iEqu;
	ARMLEVELUPDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.ItemID = iItemID;
	iIndex = bsearch_int(&stDef, pstObj->sArmLevelUPDef, pstObj->iArmLevelUPDef, ARM_LEVELUP_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ARMLEVELUPDEF*)pstObj->sArmLevelUPDef[iIndex];
		
	}
	return NULL;
}

ARMTESHUDEF*z_find_arm_teshu_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	ARMTESHUDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	stDef.TeshuID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sArmTeshuDef, pstObj->iArmTeshuDef,ARM_TESHU_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ARMTESHUDEF*)pstObj->sArmTeshuDef[iIndex];
		
	}
	return NULL;
}

SPARDEF *z_find_spardef(ZONESVRENV *pstEnv, int iItemID)
{
	int iIndex;
	int iEqu;
	SPARDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	stDef.ID = iItemID;
	iIndex = bsearch_int(&stDef, pstObj->sSparDef, pstObj->iSparDef,SPAR_DEF_SIZE, &iEqu);
	if (iEqu)
	{
		return (SPARDEF*)pstObj->sSparDef[iIndex];
		
	}
	return NULL;
}

GEMNEWDEF* z_find_gemnewdef(ZONESVRENV *pstEnv, int iItemID)
{
	int iIndex;
	int iEqu;
	GEMNEWDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	stDef.ItemID = iItemID;
	iIndex = bsearch_int(&stDef, pstObj->sGemNewDef, pstObj->iGemNewDef, GEM_NEW_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (GEMNEWDEF*)pstObj->sGemNewDef[iIndex];
		
	}
	return NULL;
}

USEDEF* z_find_usedef(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	USEDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	stDef.UseID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sUseDef, pstObj->iUseDef, USE_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (USEDEF*)pstObj->sUseDef[iIndex];
		
	}
	return NULL;
}

GEMATTRSTARDEF* z_find_gemattrstardef(ZONESVRENV *pstEnv, short iAttrID, char cStar, char cLevel)
{
	int iIndex;
	int iEqu;
	GEMATTRSTARDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	stDef.AttrID = iAttrID;
	stDef.Level = cLevel;
	stDef.Star = cStar;
	iIndex = bsearch_int(&stDef, pstObj->sGemAttrStarDef, pstObj->iGemAttrStarDef, GEM_ATTR_STAR_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (GEMATTRSTARDEF*)pstObj->sGemAttrStarDef[iIndex];
		
	}
	return NULL;
}

GEMLEVELSTARDEF* z_find_gemlevelstardef(ZONESVRENV *pstEnv, short iLevel, short iStar)
{
	int iIndex;
	int iEqu;
	GEMLEVELSTARDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	stDef.Level = iLevel;
	stDef.Star = iStar;
	iIndex = bsearch_int(&stDef, pstObj->sGemLevelStarDef, pstObj->iGemLevelStarDef, GEM_LEVEL_STAR_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (GEMLEVELSTARDEF*)pstObj->sGemLevelStarDef[iIndex];
		
	}
	return NULL;
}

ARMGRADEDEF *armgrade_def_find(ZONESVRENV *pstEnv, unsigned short iArmType, unsigned short iArmLevel)
{
	int iIndex;
	int iEqu;
	ARMGRADEDEF stArmGrade;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stArmGrade.ArmType = iArmType;
	stArmGrade.ArmLevel = iArmLevel;
	iIndex = bsearch_int(&stArmGrade, pstObj->sArmGradeDef, pstObj->iArmGradeDef, ARMGRADE_DATA, &iEqu);
	if (iEqu)
	{
		return (ARMGRADEDEF *)pstObj->sArmGradeDef[iIndex];
	}

	return NULL;
}

// level  相关等级,装备相关的概率, LevelType 1表示升星等级 2表示装备绑定 3表示装备星级转移 
ARMRANDDEF *armrand_def_find(ZONESVRENV *pstEnv, unsigned short LevelType, unsigned short Level)
{
	int iIndex;
	int iEqu;
	ARMRANDDEF stArmRand;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stArmRand.Level = Level;
	stArmRand.LevelType = LevelType;
	
	iIndex = bsearch_int(&stArmRand, pstObj->sArmRandDef, pstObj->iArmRandDef, ARMRAND_DATA, &iEqu);
	if (iEqu)
	{
		return (ARMRANDDEF *)pstObj->sArmRandDef[iIndex];
	}

	return NULL;
}

ARMHIDEATTRDEF *armhideattr_def_find(ZONESVRENV *pstEnv, short Career, short ArmStarLevel)
{
	int iIndex;
	int iEqu;
	ARMHIDEATTRDEF stArmHideAttr;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stArmHideAttr.Career = Career;
	stArmHideAttr.ArmStarLevel = ArmStarLevel;
	
	iIndex = bsearch_int(&stArmHideAttr, pstObj->sArmHideAttrDef, pstObj->iArmHideAttrDef, ARMHIDEATTR_DATA, &iEqu);
	if (iEqu)
	{
		return (ARMHIDEATTRDEF *)pstObj->sArmHideAttrDef[iIndex];
	}

	return NULL;
}

ARMSTARDEF *z_find_arm_star_def(ZONESVRENV *pstEnv, int  iStar)
{
	int iIndex;
	int iEqu;
	ARMSTARDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.Star = iStar;
	iIndex = bsearch_int(&stDef, pstObj->sArmStarDef, pstObj->iArmStarDef, ARM_STAR_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ARMSTARDEF *)pstObj->sArmStarDef[iIndex];
	}
	
	return NULL;	
}

/*int z_arm_expenddata(MACHINNGEXPENDDATA *pstExpendData, int iSuffNum, STUFFIDXDATA *pstStuffData, unsigned int uiMoney, unsigned int uiSafeItemID )
{
	if (!pstExpendData)
	{
		return 0;
	}

	if (iSuffNum && pstStuffData)
	{
		int i;
		
		pstExpendData->StuffNum = iSuffNum;
		for(i = 0; i< iSuffNum; i++)
		{
			pstExpendData->StuffInfo[i].ItemID= pstStuffData[i].ItemID;
			pstExpendData->StuffInfo[i].ItemNum = 1;
		}
	}

	pstExpendData->Money = uiMoney;
	pstExpendData->SafeItemID = uiSafeItemID;

	return 0;
}
*/
ROLEGRID *get_pak_item_grid(Player *pstPlayer, char cListType, unsigned short unGridIdx)
{
	int iPos = -1;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;

	switch(cListType)
	{
		case LIST_TYPE_SUNDRIES:
			if (unGridIdx >= pstPak->SundriesPackage.MaxNum)
			{
				return NULL;
			}
			iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, unGridIdx);
			if (0 > iPos)
			{
				return NULL;
			}
			
			return (ROLEGRID *) pstPak->SundriesPackage.RoleGrids+iPos;
			break;

		default:
			break;
	}

	return NULL;
}

//统计材料
int items_total(ConvergeItem *pstItems, int *piNum,  unsigned int iItemID)
{
	int i = 0;

	for (i = 0; i < *piNum; i++)
	{
		if (pstItems[i].ItemID == iItemID)
		{	
			pstItems[i].Num++;
			return 0;
		}
	}

	pstItems[*piNum].ItemID = iItemID;
	pstItems[*piNum].Num = 1;
	*piNum += 1;

	return 0;
}

int tmp_grid_total(TotalStuffGridTmp *pstGridTmp, int iItemID, int iNum, int iPos, int iListType)
{
	int i = 0;
	long long llTmp;

	if (0> iNum)
	{
		return -1;
	}

	llTmp = iNum;

	for (i = 0; i < pstGridTmp->iNum; i++)
	{
		if (pstGridTmp->stGrid[i].iItemID == iItemID && 
			pstGridTmp->stGrid[i].iPos == iPos)
		{
			llTmp = llTmp + pstGridTmp->stGrid[i].iNum;
			pstGridTmp->stGrid[i].iNum += iNum;

			if (llTmp != pstGridTmp->stGrid[i].iNum)
			{
				return -1;
			}
			
			return 0;
		}
	}

	if(pstGridTmp->iNum >= 8)
	{
		return -1;
	}

	pstGridTmp->stGrid[pstGridTmp->iNum].iItemID = iItemID;
	llTmp = llTmp + pstGridTmp->stGrid[pstGridTmp->iNum].iNum;
	pstGridTmp->stGrid[pstGridTmp->iNum].iNum += iNum;
	if (llTmp != pstGridTmp->stGrid[pstGridTmp->iNum].iNum)
	{
		return -1;
	}
	
	pstGridTmp->stGrid[pstGridTmp->iNum].iPos = iPos;
	pstGridTmp->stGrid[pstGridTmp->iNum].iListType = iListType;
	pstGridTmp->iNum++;
	
	return 0;
}

int get_arm_typenum(unsigned	int uiItemID)
{
	int iTypeNum = 0;
	char szItemID[16];

	memset(szItemID, 0, sizeof(szItemID));
	sprintf(szItemID, "%u", uiItemID);

	szItemID[3] = 0;
	iTypeNum = atoi(szItemID);
	
	return iTypeNum;
}

int get_arm_itemlevel(int RoleWearLevel)
{
	//1等级0=1-9 、1=10-19.....

	return RoleWearLevel / 10;
}

int arm_one_grid_update(ZONESVRENV* pstEnv,  Player *pstPlayer, ROLEGRID *pstArmGrid)
{
	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstPackageChg = &pstPackageInfo->PackageInfoData.PackageChg;

	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstPackageChg->ArmChg.ChgNum = 0;
	pstPackageChg->ItemChg.ChgNum = 0;
	pstPackageChg->ArmChg.Arms[pstPackageChg->ArmChg.ChgNum].ListType = LIST_TYPE_SUNDRIES;
	memcpy(&pstPackageChg->ArmChg.Arms[pstPackageChg->ArmChg.ChgNum].Arm, &pstArmGrid->GridData.RoleArm, sizeof(ROLEARM));
	pstPackageChg->ArmChg.ChgNum++;

	Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int item_one_grid_update(ZONESVRENV* pstEnv,  Player *pstPlayer, ROLEGRID *pstArmGrid)
{
	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstPackageChg = &pstPackageInfo->PackageInfoData.PackageChg;

	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstPackageChg->ArmChg.ChgNum = 0;
	pstPackageChg->ItemChg.ChgNum = 0;
	pstPackageChg->ItemChg.Items[pstPackageChg->ItemChg.ChgNum].ListType = LIST_TYPE_SUNDRIES;
	pstPackageChg->ItemChg.Items[pstPackageChg->ItemChg.ChgNum].Item = pstArmGrid->GridData.RoleItem;
	pstPackageChg->ItemChg.ChgNum ++;

	Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int item_one_grid_update_ex(ZONESVRENV* pstEnv,  Player *pstPlayer, ROLEITEM*pstRoleItem, int iListType)
{
	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstPackageChg = &pstPackageInfo->PackageInfoData.PackageChg;

	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstPackageChg->ArmChg.ChgNum = 0;
	pstPackageChg->ItemChg.ChgNum = 0;
	pstPackageChg->ItemChg.Items[pstPackageChg->ItemChg.ChgNum].ListType = iListType;
	pstPackageChg->ItemChg.Items[pstPackageChg->ItemChg.ChgNum].Item = *pstRoleItem;
	pstPackageChg->ItemChg.ChgNum ++;

	Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int arm_one_grid_update_ex(ZONESVRENV* pstEnv,  Player *pstPlayer,ROLEARM *pstRoleArm,int iListType)
{
	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstPackageChg = &pstPackageInfo->PackageInfoData.PackageChg;

	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstPackageChg->ArmChg.ChgNum = 0;
	pstPackageChg->ItemChg.ChgNum = 0;
	pstPackageChg->ArmChg.Arms[pstPackageChg->ArmChg.ChgNum].ListType = iListType;
	memcpy(&pstPackageChg->ArmChg.Arms[pstPackageChg->ArmChg.ChgNum].Arm, pstRoleArm, sizeof(ROLEARM));
	pstPackageChg->ArmChg.ChgNum++;

	Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int arm_add_hole(ZONESVRENV* pstEnv,RNDHOLE *pstRndHole, ROLEARM *pstArm)
{
	int iRnd;
	int iTaoZhuang=0;
	char szItemID[16];
	ITEMDEF *pstItemDef = NULL;

	memset(szItemID, 0, sizeof(szItemID));
	sprintf(szItemID, "%u", pstArm->DefID);

	//套装命令规则, 取前2 位
	szItemID[2] = 0;
	iTaoZhuang = atoi(szItemID);

	if(iTaoZhuang == 30 )
	{
		//套装不打孔
		return -1;
	}

	pstItemDef = z_find_itemdef(pstEnv, pstArm->DefID);
	if(	!pstItemDef ||
		pstItemDef->Important < 1 )
	{
		//不重要的装备不打孔
		return -1;
	}

	// 时装不打孔
	if (arm_is_fashion(pstItemDef))
		return -1;

       //第一孔	第二孔	第三孔	第四孔
       //100%	65%	35%	10%
	iRnd = (int )RAND1(100);
	if( (pstArm->HoleNum == 0 && iRnd < GEM_HOLE1_RANDOM )||
		(pstArm->HoleNum == 1 && iRnd < GEM_HOLE2_RANDOM) ||
		(pstArm->HoleNum == 2 && iRnd < GEM_HOLE3_RANDOM) ||
		(pstArm->HoleNum == 3 && iRnd < GEM_HOLE4_RANDOM) )
	{
				
	}
	else
	{
		return -1;
	}

	// 随即颜色 
	iRnd = (int )RAND1(10000);
	if (pstArm->HoleNum < MAX_ARM_HOLE)
	{
		if (iRnd < 100*pstRndHole->RndRed)
		{
			pstArm->HoleGem[pstArm->HoleNum].HoleColor = GEM_COLOR_RED;
			pstArm->HoleGem[pstArm->HoleNum].HoleGemID = 0;
			pstArm->HoleNum ++;
		}
		else if(iRnd < 100*(pstRndHole->RndBlue+pstRndHole->RndRed))
		{
			pstArm->HoleGem[pstArm->HoleNum].HoleColor = GEM_COLOR_BLUE;
			pstArm->HoleGem[pstArm->HoleNum].HoleGemID = 0;
			pstArm->HoleNum ++;
		}
		else if(iRnd < 100*(pstRndHole->RndGreen+pstRndHole->RndBlue+pstRndHole->RndRed))
		{
			pstArm->HoleGem[pstArm->HoleNum].HoleColor = GEM_COLOR_GREEN;
			pstArm->HoleGem[pstArm->HoleNum].HoleGemID = 0;
			pstArm->HoleNum ++;
		}
		else if(iRnd < 100*(pstRndHole->RndColor+pstRndHole->RndGreen+pstRndHole->RndBlue+pstRndHole->RndRed))
		{
			pstArm->HoleGem[pstArm->HoleNum].HoleColor = GEM_COLOR_COLOR;
			pstArm->HoleGem[pstArm->HoleNum].HoleGemID = 0;
			pstArm->HoleNum ++;
		}
		else 
		{
			pstArm->HoleGem[pstArm->HoleNum].HoleColor = GEM_COLOR_COLOR;
			pstArm->HoleGem[pstArm->HoleNum].HoleGemID = 0;
			pstArm->HoleNum ++;
		}
	}
	else 
	{
		return -1;
	}
	
	return 0;
}

int arm_star_mul_idx_get(ZONESVRENV* pstEnv, ROLEARM *pstRoleArm, int iStar)
{
	int i;
	int iRate, iRand;
	ARMSTARDEF *pstArmStarDef;
	
	pstArmStarDef = z_find_arm_star_def(pstEnv, iStar);
	if(!pstArmStarDef)
	{
		return -1;
	}

	if(pstRoleArm->StrengthenCount >= pstArmStarDef->PerfectNum)
	{
		pstRoleArm->StrengthenCount = 0;
		return MAX_STAR_MUL_LIST-1;
	}
	else if(pstRoleArm->StrengthenCount >= 0 || abs(pstRoleArm->StrengthenCount) > pstArmStarDef->PerfectNum)
	{
		if(RAND1(1000000) < pstArmStarDef->PerfectRate)
		{
			pstRoleArm->StrengthenCount = 0;
			return MAX_STAR_MUL_LIST-1;
		}
	}

	iRate = 0;
	iRand = RAND1(1000000);
	for(i=0; i<MAX_STAR_MUL_LIST; i++)
	{
		iRate += pstArmStarDef->StarInfo[i].Rate;
		if(iRand < iRate)
		{
			return i;
		}
	}
	
	return -1;
}

// 品质升级
int arm_star_up(ZONESVRENV* pstEnv, Player *pstPlayer, ARMSTARDATA  *pstCSArmStar)
{
	CSPKG stPkg;
	CSACTIONRES  *pstActionRes = &stPkg.Body.ActionRes;
	ROLEGRID *pstArmGrid =NULL;
	ITEMDEF *pstArmItem = NULL;
	int iStarLevel = 0, iDec = 0, iMulIdx; 
	ARMRANDDEF *pstArmRandStarDef = NULL;
	ARMSTARDEF *pstArmStarDef;
	unsigned int uiMoney = 0;
	int iRet;
	int iOldStrengthenCount;
	int MustItemID = 0;
	ITEMDEF *pstItemDef = NULL;

	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		goto _error;
	}

	pstArmStarDef = z_find_arm_star_def(pstEnv, pstCSArmStar->SelectStar);
	if(!pstArmStarDef)
	{
		goto _error;
	}

	if( pstCSArmStar->ArmData.ArmListType == LIST_TYPE_WEAR)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM_STAR);
		goto _error;
	}
	
	pstArmGrid = get_pak_item_grid(pstPlayer, pstCSArmStar->ArmData.ArmListType, pstCSArmStar->ArmData.ArmGridIdx);
	if (!pstArmGrid || pstArmGrid->Type != GRID_ITEM_ARM)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM_STAR1);
		goto _error;
	}
	
	pstArmItem = z_get_itemdef_roleitem(pstEnv, &pstArmGrid->GridData.RoleItem);
	if (!pstArmItem)
	{
		goto _error;
	}
	if(arm_is_fashion(pstArmItem))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM_STAR2);
		goto _error;
	}
	iStarLevel = pstArmGrid->GridData.RoleArm.StarLv;
	if (iStarLevel > MAX_ARM_STAR_LEVEL ||
		(pstArmItem->StarLimit != 0 && iStarLevel > pstArmItem->StarLimit))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM_STAR3);
		goto _error;
	}	

	if(pstCSArmStar->SelectStar > pstArmItem->StarLimit || 
		pstCSArmStar->SelectStar > MAX_ARM_STAR_LEVEL ||
		pstCSArmStar->SelectStar == 0 ||
		pstCSArmStar->SelectStar > iStarLevel+1)
	{
		goto _error;
	}

	if (pstArmGrid->GridData.RoleArm.StarInfo[pstCSArmStar->SelectStar-1].ValIdx >= MAX_STAR_MUL_LIST-1
		&& pstArmGrid->GridData.RoleArm.StarLv >= pstCSArmStar->SelectStar)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM801);
		goto _error;
	}

	iOldStrengthenCount = pstArmGrid->GridData.RoleArm.StrengthenCount;
		
	pstArmRandStarDef = armrand_def_find(pstEnv, ARM_UP_STAR_TYPE, pstCSArmStar->SelectStar);
	if(!pstArmRandStarDef)
	{
		goto _error;
	}

	//强化消耗=装备星级^2*50+50*装备星级
	uiMoney = pstCSArmStar->SelectStar*10;
	
	if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, uiMoney))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM105, MONEY_TYPE_MONEY_NAME);
		goto _error;
	}

	if(pstCSArmStar->Must)
	{
		//lzk
		if(1 == pstCSArmStar->Must)
		{
			if(pstArmStarDef->MustItemID == 0)
			{
				goto _error;
			}
			MustItemID = pstArmStarDef->MustItemID;
		}
		else if(2 == pstCSArmStar->Must)
		{
			//将装备第一个（从1星开始数）未完美强化的星级一次性强化到完美
			int i;
			int iStarNum = pstArmGrid->GridData.RoleArm.StarLv;
			for(i=0;i<iStarNum;i++)
			{
				if(pstArmGrid->GridData.RoleArm.StarInfo[i].ValIdx != MAX_STAR_MUL_LIST-1)
				{
					break;
				}
			}

			if(pstCSArmStar->SelectStar != i+1)
			{
				goto _error;
			}
			MustItemID = ITEM_STAT_UP_FIRST_PERFECT;
		}
		else
		{
			goto _error;
		}
		
		if(pstCSArmStar->Bind)
		{
			if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, MustItemID, 1, -1, NULL,PACKAGE_DEC_BIND))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx , pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM106);
				goto _error;
			}
		}
		else
		{
			if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, MustItemID, 1, -1, NULL,PACKAGE_DEC_UNBIND))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx , pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM107);
				goto _error;
			}
		}

		//扣钱,扣宝石
		iRet = package_dec_money( pstEnv, pstPlayer, ATTR_ID_BULL, uiMoney);
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ARMSTAR, pstArmItem->ItemID, uiMoney-iRet, iRet, "装备强化");
		if(pstCSArmStar->Bind)
		{
			iDec = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, MustItemID, 1, -1, NULL,PACKAGE_DEC_BIND, IMPITEM_OPTYPE_ARMSTAR);
			if (0 > iDec)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "package dec err");
				goto _error;
			}
		}
		else
		{
			iDec = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, MustItemID, 1, -1, NULL,PACKAGE_DEC_UNBIND, IMPITEM_OPTYPE_ARMSTAR);
			if (0 > iDec)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "package dec err");
				goto _error;
			}
		}

		pstArmGrid = get_pak_item_grid(pstPlayer, pstCSArmStar->ArmData.ArmListType, pstCSArmStar->ArmData.ArmGridIdx);
		if (!pstArmGrid)
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "Grid error");
			goto _error;
		}

		if(iDec > 0 && !(pstArmGrid->GridData.RoleArm.InstFlag & INST_ITEM_BIND))
		{
			pstArmGrid->GridData.RoleArm.InstFlag |= INST_ITEM_BIND;
		}
		
		iMulIdx = MAX_STAR_MUL_LIST-1; 
		if(0 > iMulIdx)
		{
			goto _error;
		}

		//lzk   完美强化直接先保存
		if(pstCSArmStar->SelectStar > pstArmGrid->GridData.RoleArm.StarLv)
		{
			pstArmGrid->GridData.RoleArm.StarLv++;
		}
		pstArmGrid->GridData.RoleArm.StarInfo[pstCSArmStar->SelectStar -1].ValIdx = iMulIdx;
	
		pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstArmGrid->GridData.RoleItem);
		if (NULL == pstItemDef)
		{
			goto _error;
		}
		if(pstCSArmStar->SelectStar >= 4)
		{
			if (pstEnv->pstConf->CountryVer != COUNTRY_VER_KOREA)
			{
				z_sys_strf_broadcast(pstEnv, SYS_SHOW, 
					ZONEERR_ARM901, pstPlayer->stRoleData.RoleName, pstItemDef->Name, pstCSArmStar->SelectStar);
			}
		}

		if(pstCSArmStar->SelectStar >= 1 && pstCSArmStar->SelectStar <= 3)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_ARM918,pstItemDef->Name,pstCSArmStar->SelectStar);
		}

		arm_one_grid_update(pstEnv,  pstPlayer, pstArmGrid);
	}
	else
	{
		if(pstCSArmStar->Bind)
		{
			if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, ARM_STAR_ITEM, pstArmRandStarDef->ArmStarDelGem, -1, NULL,PACKAGE_DEC_BIND))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx , pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM106);
				goto _error;
			}
		}
		else
		{
			if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, ARM_STAR_ITEM, pstArmRandStarDef->ArmStarDelGem, -1, NULL,PACKAGE_DEC_UNBIND))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx , pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM107);
				goto _error;
			}
		}

		//扣钱,扣宝石
		iRet = package_dec_money( pstEnv, pstPlayer, ATTR_ID_BULL, uiMoney);
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ARMSTAR, pstArmItem->ItemID, uiMoney-iRet, iRet, "装备强化");
		if(pstCSArmStar->Bind)
		{
			iDec = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, ARM_STAR_ITEM, pstArmRandStarDef->ArmStarDelGem, -1, NULL,PACKAGE_DEC_BIND, IMPITEM_OPTYPE_ARMSTAR);
			if (0 > iDec)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "package dec err");
				goto _error;
			}
		}
		else
		{
			iDec = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, ARM_STAR_ITEM, pstArmRandStarDef->ArmStarDelGem, -1, NULL,PACKAGE_DEC_UNBIND, IMPITEM_OPTYPE_ARMSTAR);
			if (0 > iDec)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "package dec err");
				goto _error;
			}
		}

		pstArmGrid = get_pak_item_grid(pstPlayer, pstCSArmStar->ArmData.ArmListType, pstCSArmStar->ArmData.ArmGridIdx);
		if (!pstArmGrid)
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "Grid error");
			goto _error;
		}

		pstArmGrid->GridData.RoleArm.StrengthenCount += pstArmRandStarDef->ArmStarDelGem;
		if(iDec > 0 && !(pstArmGrid->GridData.RoleArm.InstFlag & INST_ITEM_BIND))
		{
			pstArmGrid->GridData.RoleArm.InstFlag |= INST_ITEM_BIND;
		}
		
		iMulIdx = arm_star_mul_idx_get(pstEnv, &pstArmGrid->GridData.RoleArm, pstCSArmStar->SelectStar);
		if(0 > iMulIdx)
		{
			goto _error;
		}

		//lzk   完美强化直接先保存
		if(MAX_STAR_MUL_LIST-1 == iMulIdx)
		{
			if(pstCSArmStar->SelectStar > pstArmGrid->GridData.RoleArm.StarLv)
			{
				pstArmGrid->GridData.RoleArm.StarLv++;
			}
			pstArmGrid->GridData.RoleArm.StarInfo[pstCSArmStar->SelectStar -1].ValIdx = iMulIdx;

			pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstArmGrid->GridData.RoleItem);
			if (NULL == pstItemDef)
			{
				goto _error;
			}
			if(pstCSArmStar->SelectStar >= 4)
			{
				if (pstEnv->pstConf->CountryVer != COUNTRY_VER_KOREA)
				{
					z_sys_strf_broadcast(pstEnv, SYS_SHOW, 
						ZONEERR_ARM901, pstPlayer->stRoleData.RoleName, pstItemDef->Name, pstCSArmStar->SelectStar);
				}
			}

			if(pstCSArmStar->SelectStar >= 1 && pstCSArmStar->SelectStar <= 3)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_ARM918,pstItemDef->Name,pstCSArmStar->SelectStar);
			}
		}
		
		arm_one_grid_update(pstEnv,  pstPlayer, pstArmGrid);
	}
	
	pstPlayer->stOnline.stArmStarOL.ArmWID = pstArmGrid->GridData.RoleArm.WID;
	pstPlayer->stOnline.stArmStarOL.iSelectStar = pstCSArmStar->SelectStar;
	pstPlayer->stOnline.stArmStarOL.iMulIdx = iMulIdx;
	
	//成功标记
	pstActionRes->Data.ArmStar.Succ = 1;
	pstActionRes->Data.ArmStar.Star = pstCSArmStar->SelectStar;
	pstActionRes->Data.ArmStar.MulIdx = pstPlayer->stOnline.stArmStarOL.iMulIdx;
	task_arm_trace(pstEnv, pstPlayer, pstArmGrid, TASK_OBJ_ARM_STAR);

	if (iOldStrengthenCount >= pstArmGrid->GridData.RoleArm.StrengthenCount && iMulIdx != MAX_STAR_MUL_LIST-1)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "strengthencount err:%d,%d,%d", iOldStrengthenCount, pstArmGrid->GridData.RoleArm.StrengthenCount, iMulIdx);
	}
	
	//log
	{	
		MACHINNGEXPENDDATA stExpendData;
		bzero(&stExpendData, sizeof(stExpendData));
		stExpendData.Money = iRet;
		stExpendData.Bull = uiMoney - iRet;
		stExpendData.StuffNum = 1;
		if(pstCSArmStar->Must)
		{
			itemdec_fill(&stExpendData.StuffInfo[0], MustItemID, 1, iDec);
		}
		else
		{
			itemdec_fill(&stExpendData.StuffInfo[0], ARM_STAR_ITEM, pstArmRandStarDef->ArmStarDelGem, iDec);
		}
		
		z_role_machining_oplog(pstEnv, pstPlayer, &pstArmGrid->GridData.RoleArm, NULL,
			&stExpendData, OP_MACHINING_ARM_STAR, 1, NULL,0);
	}
	
_final:
	
	pstActionRes->ActionID = ACTION_ARM_STAR;
	pstActionRes->ID = pstPlayer->iMemID;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	//成长之路
	if (pstActionRes->Data.ArmStar.Succ)
	{
		ACHIEVEMENTCOND stCond;

		memset(&stCond,0,sizeof(stCond));
		
		stCond.Type = COND_EQUIP_STRENGTHEN;

		z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
	}
	return 0;

_error:
	
	pstActionRes->Data.ArmStar.Succ = 0;
	pstActionRes->Data.ArmStar.Star = 0;
	pstActionRes->Data.ArmStar.MulIdx = 0;
	goto _final;
	return 0;
}


int arm_star_save(ZONESVRENV* pstEnv, Player *pstPlayer, ARMSTARSAVEDATA *pstCSArmStar)
{
	ROLEARM stOldRoleArm;
	ROLEGRID *pstArmGrid =NULL;
	ITEMDEF *pstItemDef = NULL;

	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		return -1;
	}
	
	if( pstCSArmStar->ArmData.ArmListType == LIST_TYPE_WEAR)
	{
		return -1;
	}
	
	pstArmGrid = get_pak_item_grid(pstPlayer, pstCSArmStar->ArmData.ArmListType, pstCSArmStar->ArmData.ArmGridIdx);
	if (!pstArmGrid || pstArmGrid->Type != GRID_ITEM_ARM)
	{
		return -1;
	}
	memcpy(&stOldRoleArm,&pstArmGrid->GridData.RoleArm,sizeof(stOldRoleArm));

	if(pstArmGrid->GridData.RoleArm.WID != pstPlayer->stOnline.stArmStarOL.ArmWID ||
		pstCSArmStar->SelectStar != pstPlayer->stOnline.stArmStarOL.iSelectStar ||
		pstCSArmStar->MulIdx != pstPlayer->stOnline.stArmStarOL.iMulIdx)
	{
		return -1;
	}

	if(pstCSArmStar->SelectStar > pstArmGrid->GridData.RoleArm.StarLv)
	{
		pstArmGrid->GridData.RoleArm.StarLv++;
	}
	pstArmGrid->GridData.RoleArm.StarInfo[pstCSArmStar->SelectStar -1].ValIdx = pstCSArmStar->MulIdx;
	arm_one_grid_update(pstEnv,  pstPlayer, pstArmGrid);

	if(pstCSArmStar->SelectStar >= 4 && pstCSArmStar->MulIdx == MAX_STAR_MUL_LIST-1)
	{
		pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstArmGrid->GridData.RoleItem);
		if (pstItemDef)
		{
			if (pstEnv->pstConf->CountryVer != COUNTRY_VER_KOREA)
			{
				z_sys_strf_broadcast(pstEnv, SYS_SHOW, 
					ZONEERR_ARM901, pstPlayer->stRoleData.RoleName, pstItemDef->Name, pstCSArmStar->SelectStar);
			}
		}
	}

	memset(&pstPlayer->stOnline.stArmStarOL, 0, sizeof(pstPlayer->stOnline.stArmStarOL));
	{	
		MACHINNGEXPENDDATA stExpendData;
		bzero(&stExpendData, sizeof(stExpendData));
		
		z_role_machining_oplog(pstEnv, pstPlayer, &stOldRoleArm, &pstArmGrid->GridData.RoleArm, 
			&stExpendData, OP_MACHINING_ARM_STAR_SAVE, 1, NULL,0);
	}

	return 0;
}
/*
//战斗力升级
int arm_attr_up(ZONESVRENV* pstEnv, Player *pstPlayer, ARMUPDATA *pstArmUp, unsigned char  ucGemType)
{
	int i= 0;
	int iLevel = 0;
	int iActType = 0;
	int iMoney = 0, iRate = 0, iRnd = 0;
	char szMsg[256];
	GEMDEF *pstGemDef = NULL;
	ROLEARM stOldRoleArm;
	ROLEGRID *pstArmGrid =NULL;
	ITEMDEF *pstArmItem = NULL;
	ITEMDEF *pstItemDef = NULL;
	ITEMDEF *pstSafeDef = NULL;
	RESULT *pstResult = NULL;
	MACHINNGEXPENDDATA stExpendData;
	ROLEITEM *pstRoleItem;
	int iPos = -1;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	int iDec ;

	pstArmGrid = get_pak_item_grid(pstPlayer, pstArmUp->ArmData.ArmListType, pstArmUp->ArmData.ArmGridIdx);
	if (!pstArmGrid)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, "请先放入需要强化的装备");
		return -1;
	}
	memcpy(&stOldRoleArm, &pstArmGrid->GridData.RoleArm, sizeof(stOldRoleArm));

	pstArmItem = z_get_itemdef_roleitem(pstEnv, &pstArmGrid->GridData.RoleItem);
	if (!pstArmItem)
	{
		return -1;
	}
	
	if (ucGemType == GEM_EFFECT_STAR)
	{
		iLevel = pstArmGrid->GridData.RoleArm.StarLv;
		pstArmSvr->ArmSvrType = ARM_ACT_STAR;
		iActType = OP_MACHINING_ARM_STAR;
	}
	else if (ucGemType == GEM_EFFECT_FIGHT)
	{
		iLevel = pstArmGrid->GridData.RoleArm.FightLv;
		pstArmSvr->ArmSvrType = ARM_ACT_FIGHT;
		iActType = OP_MACHINING_ARM_FIGHT;
	}
	else
	{
		return -1;
	}

	if (0 > arm_attr_up_check(pstEnv, pstPlayer, pstArmUp, pstArmGrid, pstArmItem, ucGemType) )	
	{
		return -1;
	}

	bzero(&stExpendData, sizeof(stExpendData));
	for (i = 0; i < pstArmUp->StuffNum; i++)
	{
		pstGemDef = gem_def_find(pstEnv, pstArmUp->StuffData[i].ItemID);
		if (pstGemDef == NULL || pstGemDef->GemEffectType != ucGemType)
		{
			return -1;
		}
		iMoney += pstGemDef->GemAttr[iLevel].GemMoney;
		iRate += pstGemDef->GemAttr[iLevel].GemRate;

		if (0 > package_get_item(pstEnv, pstPlayer, pstArmUp->StuffData[i].ListType, 
							pstArmUp->StuffData[i].GridIdx, &pstItemDef, &pstRoleItem, &iPos))
		{
			goto error;
		}

		iDec =  package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
							pstArmUp->StuffData[i].ItemID, 1, 
							pstArmUp->StuffData[i].ListType, &iPos,PACKAGE_DEC_NONE);
		if (iDec < 0)
		{
			goto error;
		}	

		itemdec_fill(&stExpendData.StuffInfo[stExpendData.StuffNum++], pstArmUp->StuffData[i].ItemID,1, iDec);
	}

	//保险符
	if (pstArmUp->SafeData.ItemID)
	{
		if (0 > package_get_item(pstEnv, pstPlayer, pstArmUp->SafeData.ListType, 
							pstArmUp->SafeData.GridIdx, &pstSafeDef, &pstRoleItem, &iPos))
		{
			goto error;
		}

		iDec = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
							pstArmUp->SafeData.ItemID, 1, 
							pstArmUp->SafeData.ListType, &iPos,PACKAGE_DEC_NONE);
		if (iDec < 0 )
		{
			goto error;
		}	

		itemdec_fill(&stExpendData.SafeItem, pstArmUp->SafeData.ItemID,1, iDec);
	}

	if (0 > package_dec_money(pstEnv, pstPlayer, ATTR_ID_BULL, iMoney))
	{
		goto error;
	}

	pstArmGrid = get_pak_item_grid(pstPlayer, pstArmUp->ArmData.ArmListType, pstArmUp->ArmData.ArmGridIdx);
	if (!pstArmGrid)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "Arm attr_up  pstArmGrid error");
		return -1;
	}
	
	iRnd = (int )RAND1(10000);
	if (iRnd < iRate*100)
	{
		//成功
		//品质升级\孔3 6 9
		if (ucGemType == GEM_EFFECT_STAR)
		{
			pstArmGrid->GridData.RoleArm.StarLv++;
			if(pstArmGrid->GridData.RoleArm.StarLv > 5)
			{
				CSPKG stPkgLink;
				
				sprintf(szMsg, "%s 经过不懈努力，终于成功将@ITEMLINK@ 强化至%d级",
					pstPlayer->stRoleData.RoleName, pstArmGrid->GridData.RoleArm.StarLv);

				z_fill_link_msg(&stPkgLink, szMsg, SYS_TIP, CHAT_LINK_ARM, (void *)&pstArmGrid->GridData.RoleArm);
				z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkgLink);
			}
			
			sprintf(szMsg, "%s 强化成功品质+%d ", pstArmItem->Name, pstArmGrid->GridData.RoleArm.StarLv);

			task_arm_trace(pstEnv, pstPlayer, pstArmGrid, TASK_OBJ_ARM_STAR);
		}
		else if (ucGemType == GEM_EFFECT_FIGHT)
		{
			pstArmGrid->GridData.RoleArm.FightLv++;

			sprintf(szMsg, "%s 战斗力+%d 升级成功", pstArmItem->Name, pstArmGrid->GridData.RoleArm.FightLv);
		}

		//  耐久度 恢复
		pstArmGrid->GridData.RoleArm.Dur = z_get_arm_max_dur(pstEnv, pstArmItem)*DUR_UNIT;

		//更新升级物品
		arm_one_grid_update(pstEnv, pstPlayer, pstArmGrid);		

		// 发送成功信息
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, szMsg);
		pstArmSvr->Flag = 1;
		
	}
	else
	{
		iRnd = (int )RAND1(10000);

		if (pstSafeDef)
		{
			pstResult = z_item_get_result(pstSafeDef, RESULT_ARM_STAR_SAFE);
		}
		
		if (pstArmUp->SafeData.ItemID == 0 || !pstResult || iRnd > pstResult->ResultVal1 *100)
		{
			if (ucGemType == GEM_EFFECT_STAR && 
				pstArmGrid->GridData.RoleArm.StarLv > 1)
			{
				pstArmGrid->GridData.RoleArm.StarLv--;
				arm_one_grid_update(pstEnv, pstPlayer, pstArmGrid);		
				z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, "装备强化失败，星级降低一级");
			}
		}
		
		//发送失败信息
		pstArmSvr->Flag = 0;
	}

	pstArmSvr->RoleName[0] = 0;
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	//日志信息
	//bzero(&stExpendData, sizeof(stExpendData));
	//z_arm_expenddata(&stExpendData, pstArmUp->StuffNum, &pstArmUp->StuffData[0], iMoney, pstArmUp->SafeData.ItemID);
	stExpendData.Money = iMoney;
	z_role_machining_oplog(pstEnv, pstPlayer, &stOldRoleArm, &pstArmGrid->GridData.RoleArm, &stExpendData, iActType, pstArmSvr->Flag, NULL);

	return 0;

error:
	tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "Arm attr_up  dec error");
	return -1;
}

int arm_level_up_check(ZONESVRENV* pstEnv, Player *pstPlayer, ARMUPDATA *pstArmUp, ROLEGRID *pstArmGrid, ITEMDEF *pstArmItem)
{
	int i=0;
	GEMDEF *pstGemDef = NULL;
	ITEMDEF *pstItemDef;
	ROLEITEM *pstRoleItem;
	int iPos = -1;
	
	if ( GRID_ITEM_ARM != pstArmGrid->Type)
	{
		return -1;
	}

	if (!pstArmItem->NextItemID)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, "装备等级不可提升");
		return -1;
	}

	//检查材料
	if(pstArmUp->StuffNum <= 0)
	{	
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, "缺少升级强化的材料");
		return -1;
	}
	
	for (i = 0; i < pstArmUp->StuffNum; i++)
	{
		pstGemDef = gem_def_find(pstEnv, pstArmUp->StuffData[i].ItemID);
		if (pstGemDef == NULL || pstGemDef->GemEffectType != GEM_EFFECT_LEVEL)
		{
			return -1;
		}

		if (0 > package_get_item(pstEnv, pstPlayer, pstArmUp->StuffData[i].ListType, 
							pstArmUp->StuffData[i].GridIdx, &pstItemDef, &pstRoleItem, &iPos))
		{
			return -1;
		}

		if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
								pstArmUp->StuffData[i].ItemID,  1, 
								pstArmUp->StuffData[i].ListType, &iPos,PACKAGE_DEC_NONE))
		{
			return -1;
		}
	}

	//金钱
	if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, pstArmItem->NextItemMoney))
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "当前%s不够", MONEY_TYPE_MONEY_NAME);
		return -1;
	}

	return 0;
}

int arm_level_up(ZONESVRENV* pstEnv, Player *pstPlayer, ARMUPDATA *pstArmUp)
{
	int i= 0;
	int iRate = 0, iRnd = 0;
	char szMsg[256];
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	ROLEARM stOldRoleArm;
	MACHINNGEXPENDDATA stExpendData;
	GEMDEF *pstGemDef = NULL;
	ROLEGRID *pstArmGrid =NULL;
	ITEMDEF *pstGemItem =NULL;
	ITEMDEF *pstArmItem = NULL, *pstNextItem = NULL;
	ROLEITEM *pstRoleItem;
	int iPos = -1;
	int iDec;

	pstArmGrid = get_pak_item_grid(pstPlayer, pstArmUp->ArmData.ArmListType, pstArmUp->ArmData.ArmGridIdx);
	if (!pstArmGrid || pstArmGrid->Type != GRID_ITEM_ARM)
	{
		return -1;
	}
	memcpy(&stOldRoleArm, &pstArmGrid->GridData.RoleArm, sizeof(stOldRoleArm));

	pstArmItem = z_get_itemdef_roleitem(pstEnv, &pstArmGrid->GridData.RoleItem);
	if (!pstArmItem)
	{
		return -1;
	}

	pstNextItem = z_find_itemdef(pstEnv, pstArmItem->NextItemID);
	if (!pstNextItem)
	{
		return -1;
	}

	if (0 > arm_level_up_check(pstEnv, pstPlayer, pstArmUp, pstArmGrid, pstArmItem) )	
	{
		return -1;
	}

	bzero(&stExpendData, sizeof(stExpendData));
	for (i = 0; i < pstArmUp->StuffNum; i++)
	{
		pstGemDef = gem_def_find(pstEnv, pstArmUp->StuffData[i].ItemID);
		if (pstGemDef == NULL || pstGemDef->GemEffectType != GEM_EFFECT_LEVEL)
		{
			return -1;
		}
		iRate += pstGemDef->GemAttr[pstArmItem->NextItemLevel-1].GemRate;

		if (0 > package_get_item(pstEnv, pstPlayer, pstArmUp->StuffData[i].ListType, 
								pstArmUp->StuffData[i].GridIdx, &pstGemItem, &pstRoleItem, &iPos))
		{
			return -1;
		}
		if(!pstGemItem)
		{
			return -1;
		}

		iDec = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
								pstArmUp->StuffData[i].ItemID,  1, 
								pstArmUp->StuffData[i].ListType, &iPos,PACKAGE_DEC_NONE);
		if (iDec < 0 )
		{
			return -1;
		}

		itemdec_fill(&stExpendData.StuffInfo[stExpendData.StuffNum++], pstArmUp->StuffData[i].ItemID, 1, iDec);
			
		sprintf(szMsg, "增强装备,消耗1个%s ", pstGemItem->Name);
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, szMsg);
	}

	//金钱
	if (0 > package_dec_money(pstEnv, pstPlayer, ATTR_ID_BULL, pstArmItem->NextItemMoney))
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "当前%s不够", MONEY_TYPE_MONEY_NAME);
		return -1;
	}

	//确保减物品后格子正确
	pstArmGrid = get_pak_item_grid(pstPlayer, pstArmUp->ArmData.ArmListType, pstArmUp->ArmData.ArmGridIdx);
	if (!pstArmGrid)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "arm_level_up  pstArmGrid error");
		return -1;
	}

	iRnd = (int )RAND1(10000);
	if (iRnd < iRate*100  )
	{
		//成功升级为下一件物品
		pstArmGrid->GridData.RoleArm.DefID = pstNextItem->ItemID;
		pstArmGrid->GridData.RoleArm.Dur = z_get_arm_max_dur(pstEnv, pstNextItem)*DUR_UNIT;
		pstArmGrid->GridData.RoleArm.DefPos = ((int)pstNextItem - (int)pstEnv->pstZoneObj->sItemDef)/ITEM_DEF_DATA;
		pstArmGrid->GridData.RoleArm.Num = 1;
		if (pstNextItem->ValidTime > 0)
		{
			pstArmGrid->GridData.RoleArm.ValidEnd = pstEnv->pstAppCtx->stCurr.tv_sec + pstNextItem->ValidTime*3600;
		}

		//品质-1
		if(pstArmGrid->GridData.RoleArm.StarLv > 0)
		{
			pstArmGrid->GridData.RoleArm.StarLv--;
		}
		
		arm_one_grid_update(pstEnv, pstPlayer, pstArmGrid);
		
		// 成功信息
		sprintf(szMsg, "%s 升级为%s", pstArmItem->Name, pstNextItem->Name);
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, szMsg);
		pstArmSvr->Flag = 1;
	}
	else
	{
		//失败
		pstArmSvr->Flag = 0;
	}

	pstArmSvr->ArmSvrType = ARM_ACT_LEVEL;
	pstArmSvr->RoleName[0] = 0;
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	//日志信息
	//z_arm_expenddata(&stExpendData, pstArmUp->StuffNum, &pstArmUp->StuffData[0], pstArmItem->NextItemMoney, 0);
	stExpendData.Money = pstArmItem->NextItemMoney;
	z_role_machining_oplog(pstEnv, pstPlayer, &stOldRoleArm, &pstArmGrid->GridData.RoleArm, &stExpendData, 
							OP_MACHINING_ARM_LEVEL, pstArmSvr->Flag, NULL);

	return 0;	
}

int arm_identifiability_check(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEGRID *pstArmGrid, ITEMDEF *pstArmItem)
{
	
	if ( GRID_ITEM_ARM != pstArmGrid->Type ||
		pstArmItem->IdentifiabilityType != NORMAL_IDENTIFIABILITY ||
		pstArmItem->IdentifiabilityItems[0].ItemID == 0)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "装备不可鉴定");
		return -1;
	}

	if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, pstArmItem->IdentifiabilityMoney))
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "当前%s不够", MONEY_TYPE_MONEY_NAME);
		return -1;
	}

	return 0;
}
	
int arm_identifiability(ZONESVRENV* pstEnv, Player *pstPlayer, ARMIDXDATA *pstArmIdentifiability)
{
	int i = 0;
	int iRnd = 0, iItemID = 0, iJiChu = 0;
	ITEMDEF *pstArmItem = NULL, *pstIdentifiabilityItem = NULL;
	ROLEGRID *pstArmGrid = NULL;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	ROLEARM stOldRoleArm;
	MACHINNGEXPENDDATA stExpendData;

	pstArmGrid = get_pak_item_grid(pstPlayer, pstArmIdentifiability->ArmListType, pstArmIdentifiability->ArmGridIdx);
	if (!pstArmGrid)
	{
		return -1;
	}
	memcpy(&stOldRoleArm, &pstArmGrid->GridData.RoleArm, sizeof(stOldRoleArm));

	pstArmItem = z_get_itemdef_roleitem(pstEnv, &pstArmGrid->GridData.RoleItem);
	if (!pstArmItem)
	{
		return -1;
	}

	if (0 > arm_identifiability_check(pstEnv, pstPlayer, pstArmGrid, pstArmItem))	
	{
		return -1;
	}

	//随即获得鉴定
	iRnd = (int)RAND1(10000);
	for (i = 0; i< MAX_IDENTIFIABILITY_ITEM; i++)
	{
		if( pstArmItem->IdentifiabilityItems[i].ItemID == 0)
		{
			break;
		}
		
		if (iRnd < ( pstArmItem->IdentifiabilityItems[i].ItemRate+ iJiChu )*100)
		{
			iItemID = pstArmItem->IdentifiabilityItems[i].ItemID;
			break;
		}
		
		iJiChu +=pstArmItem->IdentifiabilityItems[i].ItemRate;
		
	}

	pstIdentifiabilityItem =  z_find_itemdef(pstEnv, iItemID);
	if (!pstIdentifiabilityItem)
	{
		return -1;
	}

	if (0 > package_dec_money(pstEnv, pstPlayer, ATTR_ID_BULL, pstArmItem->IdentifiabilityMoney))
	{
		return -1;
	}

	//鉴定物 
	pstArmGrid->GridData.RoleArm.DefID = pstIdentifiabilityItem->ItemID;
	pstArmGrid->GridData.RoleArm.Dur = z_get_arm_max_dur(pstEnv, pstIdentifiabilityItem)*DUR_UNIT;
	pstArmGrid->GridData.RoleArm.DefPos = ((int)pstIdentifiabilityItem - (int)pstEnv->pstZoneObj->sItemDef)/ITEM_DEF_DATA;
	pstArmGrid->GridData.RoleArm.Num = 1;
	pstArmGrid->GridData.RoleArm.FightLv = 0;
	pstArmGrid->GridData.RoleArm.StarLv = 0;
	pstArmGrid->GridData.RoleArm.HoleNum = 0;
	arm_add_hole(pstEnv,&pstArmItem->Hole[0], &pstArmGrid->GridData.RoleArm);
	
	if (pstArmGrid->GridData.RoleArm.HoleNum > MAX_ARM_HOLE)
	{
		pstArmGrid->GridData.RoleArm.HoleNum = MAX_ARM_HOLE;
	}
	
//	bzero(pstArmGrid->GridData.RoleArm.HoleGemID, sizeof(pstArmGrid->GridData.RoleArm.HoleGemID));
	if (pstIdentifiabilityItem->ValidTime > 0)
	{
		pstArmGrid->GridData.RoleArm.ValidEnd = pstEnv->pstAppCtx->stCurr.tv_sec + pstIdentifiabilityItem->ValidTime*3600;
	}

	arm_one_grid_update(pstEnv, pstPlayer, pstArmGrid);

	pstArmSvr->ArmSvrType = ARM_ACT_IDENTIFIABILITY;
	pstArmSvr->Flag = 1;
	pstArmSvr->RoleName[0] = 0;
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	//日志信息
	bzero(&stExpendData, sizeof(stExpendData));
	stExpendData.Money = pstArmItem->IdentifiabilityMoney;
	//z_arm_expenddata(&stExpendData, 0, NULL, pstArmItem->IdentifiabilityMoney, 0);
	z_role_machining_oplog(pstEnv, pstPlayer, &stOldRoleArm, &pstArmGrid->GridData.RoleArm, &stExpendData, 
							OP_MACHINING_ARM_IDENTIFIABILITY, 1,NULL);
	
	return 0;
}
*/

typedef struct gemlevelup
{
	int iID;
	int iRate;
}GemLvlUp;

static int gem_level_up_stuff_info(GemLvlUp *apstLvlUp, int *piUp, int iSize,int iGemID, int iRate)
{
	int i;
	int iUp = *piUp;

	for (i=0; i<iUp; i++)
	{
		if (apstLvlUp[i].iID == iGemID)
		{
			apstLvlUp[i].iRate += iRate;
			break;
		}
	}

	if (i>= iUp && i<iSize)
	{
		apstLvlUp[i].iID = iGemID;
		apstLvlUp[i].iRate = iRate;
		(*piUp) ++;
	}
	return 0;
}

static int gem_level_up_get_rand(GemLvlUp *apstLvlUp, int iUp)
{
	int i;
	int iCount = 0;
	int iRnd;

	for (i=0; i<iUp; i++)
	{
		iCount += apstLvlUp[i].iRate;
	}

	if ( iCount > 10000)
	{
		 iCount = 10000;
	}

	iRnd = RAND1(10000);

	if (iRnd < iCount)
	{
		iCount = 0;
		for (i=0; i<iUp; i++)
		{
			iCount += apstLvlUp[i].iRate;
			if (iRnd < iCount)
			{
				return apstLvlUp[i].iID;
			}
		}
	}
	
	return 0;
	
}

static int gem_level_up_get_rand1(GemLvlUp *apstLvlUp, int iUp)
{
	int i;
	int iCount = 0;
	int iRnd;

	for (i=0; i<iUp; i++)
	{
		iCount += apstLvlUp[i].iRate;
	}

	iRnd = RAND1(iCount);

	iCount = 0;
	for (i=0; i<iUp; i++)
	{
		iCount += apstLvlUp[i].iRate;
		if (iRnd < iCount)
		{
			return apstLvlUp[i].iID;
		}
	}
	return 0;
}


int gem_get_power(ZONESVRENV* pstEnv, ROLEITEM *pstItem, int iLvl)
{
	int iPower = 	pstItem->InstVal2;
	int i;
	int iStar = pstItem->InstVal1;
	GEMLEVELSTARDEF *pstDef;

	for (i=iStar -1; i>=0; i--)
	{
		pstDef = z_find_gemlevelstardef( pstEnv,iLvl, i);
		if (!pstDef)
		{
			return -1;
		}

		iPower += pstDef->PowerUp;
	}

	return iPower;
}

int gem_level_up_check(ZONESVRENV* pstEnv, Player *pstPlayer, GEMLEVELUPDATA *pstGemLevelUp,
							int *piUpID, ROLEGRID *pstRoleGrid,int *pPerfectRate, int *piMoney)
{
	int i;
	int iPos = -1;
	RESULT *pstResult;
	GEMNEWDEF *pstGemNewDef;
	ITEMDEF *pstItemDef;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	int iBind = 0;
	ITEMDEF *pstItemDef1;
	GemLvlUp stUp[5];
	int iUp = 0;
	int iGemID;
	TotalStuffGridTmp stGridTmp;
	int iPower = 0;
	int iTmp = 0;
	int iMoney = 0;
	int count = 0;
	int iZhongji = 0; // 终极合成符
	int iLvl;

	memset(&stGridTmp, 0, sizeof(stGridTmp));
	
	if (pstGemLevelUp->GemNum <= 0)
		return -1;

	// 升级符
	if (pstGemLevelUp->StuffData.ListType != LIST_TYPE_SUNDRIES)
	{
		return -1;
	}

	iPos = pkg_get_pos(	pstPlayer, pstGemLevelUp->StuffData.ListType,  
						pstGemLevelUp->StuffData.GridIdx);
	if (iPos < 0 )
	{
		return -1;
	}

	if(pstPak->SundriesPackage.RoleGrids[iPos].GridData.RoleItem.InstFlag & INST_ITEM_BIND)
	{
		iBind = 1;
	}
	
	if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
								pstGemLevelUp->StuffData.ItemID,  1, 
								pstGemLevelUp->StuffData.ListType, 
								&iPos,PACKAGE_DEC_NONE))
	{
		return -1;
	}	

	pstItemDef = z_find_itemdef(pstEnv, pstGemLevelUp->StuffData.ItemID);
	if (!pstItemDef)
	{
		return -1;
	}

	pstResult = z_item_get_result(pstItemDef, RESULT_GEM_LEVEL_UP_STUFF);
	if (!pstResult || (pstResult->ResultVal1 <= 0 && pstResult->ResultVal2 <= 0))
	{
		return -1;
	}

	// 终极保险符
	if (pstResult->ResultVal2)
	{
		iZhongji = 1;
	}

	if (pstGemLevelUp->IfBaoXian == YES_BAO_XIAN_GAOJI && pstGemLevelUp->GemNum < 5)
	{
		return -1;
	}
	
	// 宝石
	for (i=0; i<pstGemLevelUp->GemNum; i++)
	{
		pstGemNewDef = z_find_gemnewdef(pstEnv,pstGemLevelUp->GemData[i].ItemID);
		if (!pstGemNewDef ||pstGemNewDef->UpRate <= 0)
		{
			return -1;
		}

		if (i==0)
			iLvl = pstGemNewDef->Level;


		if (!iZhongji)
		{
			if (pstResult->ResultVal1 != pstGemNewDef->Level)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
									ZONEERR_ARM108, pstResult->ResultVal1 );
				return -1;
			}
		}
		else
		{
			// 终极保险符必须等级一致
			if (pstGemNewDef->Level != iLvl)
			{
				return -1;
			}
		}
		
		pstItemDef1= z_find_itemdef(pstEnv, pstGemNewDef->ItemID);
		if (!pstItemDef1)
		{
			return -1;
		}

		if (pstGemNewDef->UpID <= 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
						ZONEERR_ARM110, pstItemDef1->Name);
			return -1;
		}

		if (pstGemLevelUp->GemData[i].ListType != LIST_TYPE_SUNDRIES)
		{
			return -1;
		}

		iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, 
								pstPak->SundriesPackage.Num, 
								pstGemLevelUp->GemData[i].GridIdx);
		if (0 > iPos)
		{
			return -1;
		}

		if(pstPak->SundriesPackage.RoleGrids[iPos].GridData.RoleItem.InstFlag & INST_ITEM_BIND)
		{
			iBind = 1;
		}

		iTmp = gem_get_power(pstEnv, &pstPak->SundriesPackage.RoleGrids[iPos].GridData.RoleItem,
									pstGemNewDef->Level);
		if (iTmp < 0)
		{
			return -1;
		}
		iPower += iTmp;
		
		if (0 > tmp_grid_total(&stGridTmp, pstGemLevelUp->GemData[i].ItemID, 1, iPos, pstGemLevelUp->GemData[i].ListType))
		{
			return -1;
		}
		gem_level_up_stuff_info(stUp, &iUp, sizeof(stUp)/sizeof(stUp[0]),pstGemNewDef->ItemID, 
									pstGemNewDef->UpRate);
	}

	
	// 勾选高级保险必须是终极保险符,5级宝石
	if (iLvl < 5 )
	{
		if (pstGemLevelUp->IfBaoXian == YES_BAO_XIAN_GAOJI)
		{
			return -1;
		}
	}	
		
	for(i=0; i<stGridTmp.iNum; i++)
	{
		if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
								stGridTmp.stGrid[i].iItemID,  stGridTmp.stGrid[i].iNum, 
								stGridTmp.stGrid[i].iListType, 
								&stGridTmp.stGrid[i].iPos,PACKAGE_DEC_NONE))
		{
			return -1;
		}
	}

	for (i=0; i<iUp; i++)
	{
		count += stUp[i].iRate;
	}

	//如果概率100%以下，就要检查保险金了
	if(count < 10000)
	{
		//判断是否买保险，保险金足够不
		if(pstGemLevelUp->IfBaoXian == YES_BAO_XIAN)
		{
			if(0 > gem_level_up_baoxian_val(iLvl,&iMoney))
			{
				return -1;
			}
			
			if ( 0 > package_dec_money_test(pstEnv, pstPlayer,ATTR_ID_GODCOIN, iMoney) )
			{
				return -2;
			}
		}
		else if (pstGemLevelUp->IfBaoXian == YES_BAO_XIAN_GAOJI)
		{
			if (iLvl == 5)
			{	
				iMoney = FIVE_GEM_HECHENG_GODCOIN_GJ;
			}
			else if (iLvl == 6)
			{
				iMoney = SIX_GEM_HECHENG_GODCOIN_GJ;	
			}
			else if (iLvl == 7)
			{
				iMoney = SEVEN_GEM_HECHENG_GODCOIN_GJ;	
			}
			else if (iLvl == 8)
			{
				iMoney = EIGHT_GEM_HECHENG_GODCOIN_GJ;	
			}
			else
			{
				return -1;
			}

			if ( 0 > package_dec_money_test(pstEnv, pstPlayer,ATTR_ID_GODCOIN, iMoney) )
			{
				return -2;
			}
		}

		*piMoney = iMoney;
	}
	else
	{
		//别 扣保险费了
		*pPerfectRate = 1;
	}

	// 获取生成的宝石
	iGemID = gem_level_up_get_rand(stUp, iUp);
	if (iGemID <= 0)
	{
		//高级保险必定会成功
		if (pstGemLevelUp->IfBaoXian == YES_BAO_XIAN_GAOJI)
		{
			iGemID =  gem_level_up_get_rand1(stUp, iUp);
		}
		else
		{
			return 0;
		}
	}

	pstGemNewDef = z_find_gemnewdef(pstEnv,iGemID);
	if (!pstGemNewDef )
	{
		return -1;
	}

	iGemID = pstGemNewDef->UpID;
	pstGemNewDef = z_find_gemnewdef(pstEnv,iGemID);
	if (!pstGemNewDef )
	{
		return -1;
	}

	if (item_create(pstEnv->pstAppCtx,pstEnv,pstRoleGrid, iGemID) == 0)
	{
		pstRoleGrid->GridData.RoleItem.Num = 1;
		if (iBind)
			pstRoleGrid->GridData.RoleItem.InstFlag |= INST_ITEM_BIND;

		// 妈的 这里又要走一遍宝石充能...
		GEMLEVELSTARDEF *pstLevelStarDef;
		int iCurrP = 0, iOldP = 0;
		int iFlag = 0;
		int iStar = 0;

		pstLevelStarDef = z_find_gemlevelstardef( pstEnv, pstGemNewDef->Level, iStar);
		if (!pstLevelStarDef )
			return -1;
		
		while(iPower > 0 && pstLevelStarDef->PowerUp > 0)
		{
			iFlag = 0;
			iOldP = iCurrP;
			iCurrP += iPower;
			iPower -= (pstLevelStarDef->PowerUp-iOldP);
			if (iCurrP >= pstLevelStarDef->PowerUp)
			{
				iCurrP = 0;
				iStar ++;
				iFlag = 1;			// 星级变了,重新拉取等级对照表
			}
			
			if (!iFlag)
				continue;
			
			pstLevelStarDef =  z_find_gemlevelstardef( pstEnv, pstGemNewDef->Level, iStar);
			if (!pstLevelStarDef)
			{
				return -1;
			}
		}

		pstRoleGrid->GridData.RoleItem.InstVal1 = iStar;
		pstRoleGrid->GridData.RoleItem.InstVal2 = iCurrP;
		
	}
	else
	{
		return -1;
	}

	if (package_add_test(pstEnv->pstAppCtx, pstEnv,pstPlayer, pstRoleGrid,  1,0) <0)
	{
		return -1;
	}

	*piUpID = iGemID;
	return iGemID;
}

int gem_level_up(ZONESVRENV* pstEnv, Player *pstPlayer, GEMLEVELUPDATA *pstGemLevelUp)
{
	int i = 0;
	int  iGemID = 0;
	ITEMDEF  *pstGemUpItem = NULL;
	ROLEGRID stRoleGrid;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	MACHINNGEXPENDDATA stExpendData;
	int iDec;
	ITEMDEC stObTainItem;
	GEMNEWDEF *pstGemNewDef;
	int iPos = -1;
	int iRet;
	int iAdd = 0;
	int iGetNum = 0;
	int iBind = 0;
	//tdr_ulonglong ullWid = 0;
	
	int iMoney = 0;
//	RESULT *pstResult;
//	ITEMDEF *pstItemDef;
	int iPerfectRate = 0;

	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		goto final;
	}

	iRet = gem_level_up_check(pstEnv, pstPlayer, pstGemLevelUp,&iGemID,&stRoleGrid,&iPerfectRate,&iMoney);

	// 小于0条件不满足 合成失败
	if (iRet <= 0)
	{
		pstArmSvr->Flag = 0;
		if(iRet == -2)
		{
			goto end;
		}

		if (iRet < 0)
		{
			goto final;
		}
		
	}
	// 成功
	else
	{
		iAdd = 1;
		pstArmSvr->Flag = 1;
	}

	if (iAdd)
	{
		pstGemNewDef = z_find_gemnewdef(pstEnv, iGemID);
		if (!pstGemNewDef)
		{
			goto final;
		}

		pstGemUpItem = z_find_itemdef(pstEnv, iGemID);
		if (!pstGemUpItem)
		{
			goto final;
		}
	}

	if(iPerfectRate != 1)
	{
		// jerry -------modify-------------start
		
		//买保险就扣保险费
		/*if(pstGemLevelUp->IfBaoXian == YES_BAO_XIAN)
		{
			pstItemDef = z_find_itemdef(pstEnv, pstGemLevelUp->StuffData.ItemID);
			if (!pstItemDef)
			{
				goto final;
			}

			pstResult = z_item_get_result(pstItemDef, RESULT_GEM_LEVEL_UP_STUFF);
			if (!pstResult || pstResult->ResultVal1 <= 0)
			{
				goto final;
			}
			
			if(0 > gem_level_up_baoxian_val(pstResult->ResultVal1,&iMoney))
			{
				goto final;
			}

			package_dec_money(pstEnv, pstPlayer,ATTR_ID_GODCOIN, iMoney);

			godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
						pstPlayer->stRoleData.Level,GODCOIN_OPER_FLAG_GEM_LEVEL_UP,pstResult->ResultVal1,0, 0,
						iMoney,"宝石合成保险");
		}*/

			
		if (iMoney > 0)
		{
			
			package_dec_money(pstEnv, pstPlayer,ATTR_ID_GODCOIN, iMoney);

			godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
								pstPlayer->stRoleData.Level,GODCOIN_OPER_FLAG_GEM_LEVEL_UP,
								0,0, 0,iMoney,"宝石合成保险");
		}
		// jerry -------modify-------------end
	}
	
	
	bzero(&stExpendData, sizeof(stExpendData));
	bzero(&stObTainItem, sizeof(stObTainItem));
	stExpendData.Money = 0;

	if(iRet > 0 || (iRet == 0 && pstGemLevelUp->IfBaoXian == NO_BAO_XIAN))
	{
		// 材料
		for (i = 0; i<pstGemLevelUp->GemNum; i++ )
		{
			iPos = pkg_get_pos(	pstPlayer, pstGemLevelUp->GemData[i].ListType,
								pstGemLevelUp->GemData[i].GridIdx);
			iDec = package_dec(	pstEnv->pstAppCtx, pstEnv, pstPlayer, 
								pstGemLevelUp->GemData[i].ItemID, 1, 
								pstGemLevelUp->GemData[i].ListType, 
								&iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_GEMLEVUP);
			if (iDec > 0)
			{
				iBind = 1;
			}

			itemdec_fill(&stExpendData.StuffInfo[stExpendData.StuffNum++],pstGemLevelUp->GemData[i].ItemID, 1, iDec);
		}
	}

	// 保险附
	iPos = pkg_get_pos(	pstPlayer, pstGemLevelUp->StuffData.ListType,
						pstGemLevelUp->StuffData.GridIdx);
	iDec = package_dec(	pstEnv->pstAppCtx, pstEnv, pstPlayer, 
						pstGemLevelUp->StuffData.ItemID, 1, 
						pstGemLevelUp->StuffData.ListType, 
						&iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_GEMLEVUP);
	if (iDec > 0)
	{
		iBind = 1;
	}
	itemdec_fill(&stExpendData.SafeItem, pstGemLevelUp->StuffData.ItemID, 1, iDec);

	if (iAdd)
	{
		if(iBind)
		{
			stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
		}
		
		package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid,  1,0, IMPITEM_OPTYPE_GEMLEVUP);
		pstArmSvr->IndexID = stRoleGrid.GridData.RoleItem.DefID;
		pstArmSvr->WID = stRoleGrid.GridData.RoleItem.WID;
		itemdec_fill(&stObTainItem,stRoleGrid.GridData.RoleItem.DefID,1, 
					(stRoleGrid.GridData.RoleItem.InstFlag&INST_ITEM_BIND)?1:0);
		iGetNum ++;

		//获得物品提示
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GEM_UP1, pstGemUpItem->Name);
	}

	//日志信息
	z_role_machining_oplog(pstEnv, pstPlayer, NULL, NULL, &stExpendData, 
							OP_MACHINING_GEM_LEVEL_UP, pstArmSvr->Flag, &stObTainItem,iGetNum);

final:
	pstArmSvr->ArmSvrType = GEM_ACT_LEVEL_UP;
	pstArmSvr->RoleName[0] = 0;
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
end:
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_DIANFENG35);
	return 0;
}


int gem_inlay_check(ZONESVRENV* pstEnv, Player *pstPlayer, GEMINLAYDATA *pstGemInlay, ROLEARM *pstRoleArms, 
		ITEMDEF *pstArmItem, ITEMDEF *pstGemItem, GEMNEWDEF *pstGemNewDef)
{
	int iPos = -1;
	UNUSED(pstGemItem);
	unsigned int iMoney;

	// 从1开始
	if (pstGemInlay->ArmHoleIdx == 0 ||pstGemInlay->ArmHoleIdx > pstRoleArms->HoleNum )
	{
		return -1;
	}

	if (pstRoleArms->HoleNum < pstGemInlay->ArmHoleIdx)
	{
		return -1;
	}
	
	// 颜色不对
	if (pstRoleArms->HoleGem[pstGemInlay->ArmHoleIdx -1].HoleColor != pstGemNewDef->Color)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GEM_INLAY);
		return -1;
	}

	// 等级不对
	if (pstArmItem->WearPre.RoleLevelMin < pstGemNewDef->ArmLevel)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GEM_INLAY1);
		return -1;
	}

	// 背包没有此道具
	iPos = pkg_get_pos(	pstPlayer, pstGemInlay->GemData.ListType, 
						pstGemInlay->GemData.GridIdx);
	if (iPos < 0)
	{
		return -1;
	}
	if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
							pstGemInlay->GemData.ItemID, 1, 
							pstGemInlay->GemData.ListType, 
							&iPos,PACKAGE_DEC_NONE))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GEM_INLAY2);
		return -1;
	}

	iMoney = pstGemNewDef->InlayUseMoney*pstArmItem->WearPre.GemInlayMoney;
	if (iMoney > 0)
	{
		if (0 > package_dec_money_test(pstEnv, pstPlayer, pstGemNewDef->InlayUseType, 
				pstGemNewDef->InlayUseMoney*pstArmItem->WearPre.GemInlayMoney))
		{
			return -1;
		}
	}
	

	return 0;
}

int gem_open_hole(ZONESVRENV* pstEnv, Player *pstPlayer, GEMOPENHOLEDATA *pstGemOpenHole)
{
	ROLEGRID *pstArmGrid =NULL;
	ROLEARM   *pstRoleArm = NULL;
	ITEMDEF   *pstArmItem = NULL;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	int i = 0;
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	MACHINNGEXPENDDATA stExpendData;
	ROLEARM stOldArm;

	memset(pstArmSvr,0,sizeof(CSARMSVR));
	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		goto error;
	}

	if( pstGemOpenHole->ArmData.ArmListType == LIST_TYPE_WEAR)
	{
	
		for(i = 0; i<pstRoleWear->ArmNum; i++)
		{
			if(pstRoleWear->RoleArms[i].GridIdx == pstGemOpenHole->ArmData.ArmGridIdx)
			{
				break;
			}
		}
		if(i>=pstRoleWear->ArmNum)
		{
			goto error;
		}
		pstArmItem = z_find_itemdef(pstEnv, pstRoleWear->RoleArms[i].DefID);
		if (!pstArmItem)
		{
			goto error;
		}
		if(arm_is_fashion(pstArmItem))
		{
			goto error;
		}
		
		pstRoleArm = &pstRoleWear->RoleArms[i];

		if(pstRoleArm->HoleNum >= MAX_ARM_HOLE)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE, ZONEERR_ARM112);
			goto error;
		}
	}
	else 
	{
		pstArmGrid = get_pak_item_grid(pstPlayer, pstGemOpenHole->ArmData.ArmListType, pstGemOpenHole->ArmData.ArmGridIdx);
		if (!pstArmGrid)
		{
			goto error;
		}
		pstArmItem = z_get_itemdef_roleitem(pstEnv, &pstArmGrid->GridData.RoleItem);
		if (!pstArmItem)
		{
			goto error;
		}
		if(arm_is_fashion(pstArmItem))
		{
			goto error;
		}
		if ( GRID_ITEM_ARM != pstArmGrid->Type)
		{
			goto error;
		}

		pstRoleArm = &pstArmGrid->GridData.RoleArm;

		if(pstRoleArm->HoleNum >= MAX_ARM_HOLE)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE, ZONEERR_ARM112);
			goto error;
		}
	}

	memcpy(&stOldArm, pstRoleArm, sizeof(stOldArm));
	
	if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, pstArmItem->WearPre.GemOpenHoleMoney))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM113, MONEY_TYPE_MONEY_NAME);
		goto error;
	}

	if (0 > package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, pstArmItem->WearPre.GemOpenHoleMoney))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM113, MONEY_TYPE_MONEY_NAME);
		goto error;
	}

	z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ARMHOLE, pstArmItem->ItemID,  0, pstArmItem->WearPre.GemOpenHoleMoney, "装备开孔");
	
	if( 0  > arm_add_hole(pstEnv,&pstArmItem->Hole[pstRoleArm->HoleNum], pstRoleArm) )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM114);
		goto _fail;
	}

	//开启成功了
	//arm_wear_update( pstEnv,  pstPlayer, pstRoleArm);
	if( pstGemOpenHole->ArmData.ArmListType == LIST_TYPE_WEAR)
	{
		arm_wear_update( pstEnv,  pstPlayer, pstRoleArm);
	}
	else if(pstArmGrid)
	{
		arm_one_grid_update(pstEnv,  pstPlayer, pstArmGrid);
	}


	pstArmSvr->Flag = 1;

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM115);

_fail:
	bzero(&stExpendData, sizeof(stExpendData));
	stExpendData.Money = pstArmItem->WearPre.GemOpenHoleMoney;
	z_role_machining_oplog(pstEnv, pstPlayer, &stOldArm, pstRoleArm, &stExpendData, 
		OP_MACHINING_ARM_OPENHOLE, pstArmSvr->Flag, NULL,0);

error:
	pstArmSvr->ArmSvrType = GEM_ACT_OPEN_HOLE;
	pstArmSvr->RoleName[0] = 0;
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

//获取宝石的其中一个属性
int gem_inlay_get_gemresultindex(ZONESVRENV* pstEnv,ITEMDEF *pstGemItem )
{
	int i=0;
	int iCount = 0;
	int iRand = 0;
	UNUSED(pstEnv);
	
	for(i=0;i<MAX_ITEM_RESULT;i++)
	{
		if(pstGemItem->Result[i].ResultID != 0)
		{
			iCount++; 
		}
		else
		{
			break;
		}
	}

	iRand =  RAND1(iCount);
	
	return iRand;
}

// 2 表示完美镶嵌
int gen_inlay_check_best(ZONESVRENV* pstEnv,ITEMDEF *pstGemItem,int iIndex)
{
	int i=0;
	UNUSED(pstEnv);
	
	for(i=0;i<MAX_ITEM_RESULT;i++)
	{
		if( pstGemItem->Result[i].ResultID != 0 &&
			(pstGemItem->Result[i].ResultVal1 > pstGemItem->Result[iIndex].ResultVal1 ||
			 pstGemItem->Result[i].ResultVal2 > pstGemItem->Result[iIndex].ResultVal2 ||
			 pstGemItem->Result[i].ResultVal3 > pstGemItem->Result[iIndex].ResultVal3 ||
			 pstGemItem->Result[i].ResultVal4 > pstGemItem->Result[iIndex].ResultVal4 ))
		{
			return 1;
		}
	}
	
	return 2;
}

int arm_wear_update(ZONESVRENV* pstEnv,  Player *pstPlayer, ROLEARM  *pstRoleArm)
{
	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstPackageChg = &pstPackageInfo->PackageInfoData.PackageChg;

	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstPackageChg->ArmChg.ChgNum = 0;
	pstPackageChg->ItemChg.ChgNum = 0;
	pstPackageChg->ArmChg.Arms[pstPackageChg->ArmChg.ChgNum].ListType = LIST_TYPE_WEAR;
	memcpy(&pstPackageChg->ArmChg.Arms[pstPackageChg->ArmChg.ChgNum].Arm, pstRoleArm, sizeof(ROLEARM));
	pstPackageChg->ArmChg.ChgNum++;

	Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int gem_get_rand_attr(GEMATTR *pstAttr, int iNum)
{
	int i;
	int iRate = 0;
	int iRateCount = 0;
	for (i=0; i<iNum; i++)
	{
		if (pstAttr[i].Rate == 0)
		{
			break;
		}
		iRateCount += pstAttr[i].Rate;
	}

	if (iRateCount <= 0)
		return -1;
	
	iRate = RAND1(iRateCount);
	iRateCount = 0;
	for (i=0; i<iNum; i++)
	{
		iRateCount +=pstAttr[i].Rate ;
		if (iRate < iRateCount)
		{
			return i;
		}
	}
	return -1;
}


char gem_get_rand_mul(GEMATTRSTARDEF *pstAttrStarDef, int *piValIdx)
{
	int iRand;
	if (pstAttrStarDef->MulLower >= pstAttrStarDef->MulUp)
	{
		return 0;
	}

	iRand =  RAND1((pstAttrStarDef->MulUp - pstAttrStarDef->MulLower)+1)+pstAttrStarDef->MulLower;

	if (pstAttrStarDef->Attr[*piValIdx].AttrID*(1.0+iRand/100.0f) < pstAttrStarDef->Attr[0].AttrID)
	{
		*piValIdx = 0;
		return 0;
	}

	if (pstAttrStarDef->Attr[*piValIdx].AttrID*(1.0+iRand/100.0f) > pstAttrStarDef->Attr[MAX_GEM_VAL-1].AttrID)
	{
		*piValIdx = MAX_GEM_VAL-1;
		return 0;
	}

	return iRand;
}

int gem_inlay_fill_armhole(ZONESVRENV* pstEnv,HOLEGEM *pstHole, ITEMDEF *pstGemItemDef, 
							GEMNEWDEF *pstGemNewDef, ROLEITEM *pstGemRoleItem,int *piBest)
{
	int iIdx, i;
	GEMATTRSTARDEF *pstAttrStarDef;
	HOLEGEM stOldHole;
	stOldHole = *pstHole;
	
	iIdx = gem_get_rand_attr(pstGemNewDef->Attr, MAX_GEM_ATTR);
	if (iIdx < 0)
		goto error;
	pstHole->GemAttrIdx = iIdx;			// 属性随机,记索引,resultid会大于255

	pstAttrStarDef = z_find_gemattrstardef(pstEnv, pstGemNewDef->Attr[iIdx].AttrID,  
										pstGemRoleItem->InstVal1,pstGemNewDef->Level);
	if (!pstAttrStarDef)
		goto error;
	
	iIdx = gem_get_rand_attr(pstAttrStarDef->Attr, MAX_GEM_VAL);
	if (iIdx < 0)
		goto error;

	pstHole->Mul = gem_get_rand_mul(pstAttrStarDef, &iIdx); // 修正
	
	pstHole->HoleGemResultIndex = iIdx;	// 数值随机，记索引  数值会大于255
	pstHole->HoleGemID = pstGemItemDef->ItemID;
	pstHole->GemPower = pstGemRoleItem->InstVal2;
	pstHole->GemStar = pstGemRoleItem->InstVal1;

	*piBest = 2; 							// 完美镶嵌
	for (i=0; i<MAX_GEM_VAL; i++)
	{
		if (pstAttrStarDef->Attr[i].AttrID > pstAttrStarDef->Attr[iIdx].AttrID)
		{
			*piBest = 1;
			break;
		}
	}
	return 0;

error:
	*pstHole = stOldHole; 					// 出错还原宝石孔
	return -1;
}

int gem_inlay(ZONESVRENV* pstEnv, Player *pstPlayer, GEMINLAYDATA *pstGemInlay)
{
	ROLEGRID *pstArmGrid =NULL;
	ITEMDEF *pstArmItem = NULL;
	ITEMDEF *pstGemItem = NULL;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	ROLEARM stOldRoleArm;
	MACHINNGEXPENDDATA stExpendData;
	ITEMDEF *pstItemDef;
	ROLEITEM *pstRoleItem;
	int iPos = -1;
	int i=0;
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	ROLEARM *pstRoleArms = NULL;
	int iDec;
	GEMNEWDEF *pstGemNewDef;
	int iBest;
	unsigned int iUseMoney = 0;

	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		goto error;
	}
	
	if( pstGemInlay->ArmData.ArmListType == LIST_TYPE_WEAR)
	{
		for(i = 0; i<pstRoleWear->ArmNum; i++)
		{
			if(pstRoleWear->RoleArms[i].GridIdx == pstGemInlay->ArmData.ArmGridIdx)
			{
				break;
			}
		}
		if(i>=pstRoleWear->ArmNum)
		{
			goto error;
		}
		
		pstRoleArms = &pstRoleWear->RoleArms[i];
		
		pstArmItem = z_find_itemdef(pstEnv, pstRoleArms->DefID);
		if (!pstArmItem)
		{
			goto error;
		}

		//lzk
		if(arm_is_badge(pstArmItem) < 0 )
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ARM905);
			return -1;
		}

		
		if(arm_is_fashion(pstArmItem))
		{
			goto error;
		}
	}
	else 
	{
		pstArmGrid = get_pak_item_grid(pstPlayer, pstGemInlay->ArmData.ArmListType, pstGemInlay->ArmData.ArmGridIdx);
		if (!pstArmGrid)
		{
			goto error;
		}
		pstArmItem = z_get_itemdef_roleitem(pstEnv, &pstArmGrid->GridData.RoleItem);
		if (!pstArmItem)
		{
			goto error;
		}

		
		//lzk
		if(arm_is_badge(pstArmItem) < 0 )
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ARM905);
			return -1;
		}
			
		if(arm_is_fashion(pstArmItem))
		{
			goto error;
		}
		if ( GRID_ITEM_ARM != pstArmGrid->Type)
		{
			goto error;
		}

		pstRoleArms = &pstArmGrid->GridData.RoleArm;
	}

	memcpy(&stOldRoleArm, pstRoleArms, sizeof(stOldRoleArm));

	pstGemNewDef = z_find_gemnewdef(pstEnv, pstGemInlay->GemData.ItemID);
	if (!pstGemNewDef )
	{
		goto error;
	}

	pstGemItem =  z_find_itemdef(pstEnv, pstGemInlay->GemData.ItemID);
	if (!pstGemItem)
	{
		goto error;
	}
		
	if (0 > gem_inlay_check(pstEnv, pstPlayer, pstGemInlay, pstRoleArms, pstArmItem, pstGemItem, pstGemNewDef))
	{
		return -1;
	}	

	if (0 > package_get_item(pstEnv, pstPlayer, 
							pstGemInlay->GemData.ListType, pstGemInlay->GemData.GridIdx, 
							&pstItemDef, &pstRoleItem, &iPos))
	{
		goto error;
	}

	if (iPos < 0)
	{
		return -1;
	}

	if (gem_inlay_fill_armhole(pstEnv,&pstRoleArms->HoleGem[pstGemInlay->ArmHoleIdx-1], 
								pstGemItem, pstGemNewDef,pstRoleItem,&iBest) < 0)
	{
		goto error;
	}

	iUseMoney = pstGemNewDef->InlayUseMoney*pstArmItem->WearPre.GemInlayMoney;
	if (iUseMoney)
	{
		if (0 > package_dec_money(pstEnv, pstPlayer, pstGemNewDef->InlayUseType, iUseMoney))
		{
			goto error;
		}

		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_GEMIN, pstArmItem->ItemID, 0, iUseMoney, "宝石镶嵌");
	}

	if(pstRoleItem->InstFlag & INST_ITEM_BIND)
	{
		pstRoleArms->InstFlag |= INST_ITEM_BIND;
	}
	
	if( pstGemInlay->ArmData.ArmListType == LIST_TYPE_WEAR)
	{
		arm_wear_update( pstEnv,  pstPlayer, pstRoleArms);
		z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}
	else if(pstArmGrid)
	{
		arm_one_grid_update(pstEnv,  pstPlayer, pstArmGrid);
	}

	// -----------------------------------------
	pstArmSvr->Flag = iBest;
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GEM_INLAY3, pstArmItem->Name, pstGemItem->Name);
	pstArmSvr->IndexID  = pstGemInlay->ArmHoleIdx;

	if(pstGemNewDef->Level >= 3)
	{
		CSPKG stPkgLink;
		z_fill_link_msg(&stPkgLink, SYS_SHOW, CHAT_LINK_ARM, (void *)pstRoleArms, ZONEERR_LINK301,
						pstPlayer->stRoleData.RoleName, pstGemItem->Name);
		
		if ( pstEnv->pstConf->CountryVer != COUNTRY_VER_KOREA)
		{
			z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkgLink);
		}
	}
	

	pstArmSvr->ArmSvrType = GEM_ACT_INLAY;
	pstArmSvr->RoleName[0] = 0;
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	iDec = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
						pstGemInlay->GemData.ItemID, 1, 
						pstGemInlay->GemData.ListType, 
						&iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_GEMIN);

	task_player_trace(pstEnv, pstPlayer, TASK_OBJ_INLAY_GEM);
	
	//日志信息
	bzero(&stExpendData, sizeof(stExpendData));
	itemdec_fill(&stExpendData.StuffInfo[0], pstGemInlay->GemData.ItemID, 1, iDec);
	stExpendData.Money = pstArmItem->WearPre.GemInlayMoney;
	stExpendData.StuffNum = 1;
	z_role_machining_oplog(pstEnv, pstPlayer, &stOldRoleArm, pstRoleArms, 
			&stExpendData, OP_MACHINING_GEM_INLAY, pstArmSvr->Flag, NULL, 0);
	return 0;
	
error:
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GEM_INLAY4);
	return -1;
}

int gem_pick_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{	

	if(0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, ARM_GEM_PICK_ITEM,1, -1,0,PACKAGE_DEC_NONE))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GEM_PICK);
		return -1;
	}

	return 0;
}

int gem_hole_clean(HOLEGEM *pstHole)
{
	int iColor = pstHole->HoleColor;

	memset(pstHole, 0, sizeof(HOLEGEM));
	pstHole->HoleColor = iColor;
	return 0;
}

int gem_is_chongneng(ITEMDEF *pstItemDef, ROLEITEM *pstItem)
{
	if (pstItemDef->Result[0].ResultID == RESULT_GEM)
	{
		//  星级大于0或者能量大于0都判定为冲过能
		if (pstItem->InstVal1 > 0 || pstItem->InstVal2 > 0)
		{
			return 1;
		}

		return 2; // 没有冲过能的宝石
	}
	return 0; // 不是宝石
}

int gem_pick(ZONESVRENV* pstEnv, Player *pstPlayer, GEMPICKDATA *pstGemPick)
{
	ROLEGRID stRoleGrid;
	ROLEGRID *pstArmGrid =NULL;
	ITEMDEF *pstArmItem = NULL;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	ROLEARM stOldRoleArm;
	MACHINNGEXPENDDATA stExpendData;
	int i = 0;
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	ROLEARM *pstRoleArms = NULL;
	int iDec = -1;
	ITEMDEC stGem;
	GEMNEWDEF *pstGemNewDef;

	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		goto error;
	}

	if( pstGemPick->ArmData.ArmListType == LIST_TYPE_WEAR)
	{
		for(i = 0; i<pstRoleWear->ArmNum; i++)
		{
			if(pstRoleWear->RoleArms[i].GridIdx == pstGemPick->ArmData.ArmGridIdx)
			{
				break;
			}
		}
		if(i>=pstRoleWear->ArmNum)
		{
			goto error;
		}
		
		pstRoleArms = &pstRoleWear->RoleArms[i];
		
		pstArmItem = z_find_itemdef(pstEnv, pstRoleArms->DefID);
		if (!pstArmItem)
		{
			goto error;
		}

		//lzk
		if(arm_is_badge(pstArmItem) < 0)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ARM906);
			return -1;
		}
		
	}
	else 
	{
		pstArmGrid = get_pak_item_grid(pstPlayer, pstGemPick->ArmData.ArmListType, pstGemPick->ArmData.ArmGridIdx);
		if (!pstArmGrid)
		{
			goto error;
		}
		pstArmItem = z_get_itemdef_roleitem(pstEnv, &pstArmGrid->GridData.RoleItem);
		if (!pstArmItem)
		{
			goto error;
		}

		//lzk
		if(arm_is_badge(pstArmItem) < 0)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ARM906);
			return -1;
		}

		if ( GRID_ITEM_ARM != pstArmGrid->Type)
		{
			goto error;
		}

		pstRoleArms = &pstArmGrid->GridData.RoleArm;
	}

	memcpy(&stOldRoleArm, pstRoleArms, sizeof(stOldRoleArm));
	
	if (	pstGemPick->ArmHoleIdx == 0 ||
		pstGemPick->ArmHoleIdx > pstRoleArms->HoleNum || 
		pstRoleArms->HoleGem[pstGemPick->ArmHoleIdx -1].HoleGemID == 0)
	{
		return -1;
	}

	pstGemNewDef = z_find_gemnewdef(pstEnv, pstRoleArms->HoleGem[pstGemPick->ArmHoleIdx -1].HoleGemID);
	if (!pstGemNewDef)
	{
		return -1;
	}

	if ( 0 > gem_pick_check(pstEnv, pstPlayer))
	{
		return -1;
	}

	// 生成宝石
	if (0 == item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid,  pstRoleArms->HoleGem[pstGemPick->ArmHoleIdx -1].HoleGemID))
	{
		if(pstRoleArms->InstFlag & INST_ITEM_BIND)
		{
			stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
		}
		// 摘下来要记录星级和能量点
		stRoleGrid.GridData.RoleItem.InstVal1 = pstRoleArms->HoleGem[pstGemPick->ArmHoleIdx -1].GemStar;
		stRoleGrid.GridData.RoleItem.InstVal2 = pstRoleArms->HoleGem[pstGemPick->ArmHoleIdx -1].GemPower;
		if (package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid,  1,0) == -1)
		{
			return -1;
		}			
	}
	else
	{
		return -1;
	}

	// 恢复宝石孔
	gem_hole_clean(&pstRoleArms->HoleGem[pstGemPick->ArmHoleIdx -1]);
	
	if( pstGemPick->ArmData.ArmListType == LIST_TYPE_WEAR)
	{
		arm_wear_update( pstEnv,  pstPlayer, pstRoleArms);
		z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}
	else if(pstArmGrid)
	{
		arm_one_grid_update(pstEnv,  pstPlayer, pstArmGrid);
	}
	
	//摘取所需道具
	iDec = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
						ARM_GEM_PICK_ITEM, 1, -1, NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_GEMPICK);
	if(iDec < 0)
	{
		goto error;
	}
	else if (iDec > 0)
	{
		stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
	}
	
	package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid,  1,0, IMPITEM_OPTYPE_GEMPICK);
	
	pstArmSvr->ArmSvrType = GEM_ACT_PICK;
	pstArmSvr->Flag = 1;
	pstArmSvr->RoleName[0] = 0;
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	//日志信息
	bzero(&stExpendData, sizeof(stExpendData));
	itemdec_fill(&stExpendData.StuffInfo[0], ARM_GEM_PICK_ITEM, 1, iDec);
	
	itemdec_fill(&stGem, stRoleGrid.GridData.RoleArm.DefID, 1, (stRoleGrid.GridData.RoleArm.InstFlag&INST_ITEM_BIND)?1:0);
	z_role_machining_oplog(pstEnv, pstPlayer, &stOldRoleArm, pstRoleArms, &stExpendData, 
						OP_MACHINING_GEM_PICK, 1, &stGem,1);
	return 0;
	
error:
	tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "Arm gem_pick error");
	return -1;
}

int z_get_arm_result_total(ITEMDEF *pstItemDef, RESULT *pstResult, int iResultID)
{
	int i;

	bzero(pstResult, sizeof(*pstResult));
	
	for (i=0; i<MAX_ITEM_RESULT; i++)	
	{
		if (0 ==pstItemDef->Result[i].ResultID)
		{
			//break;
			continue;
		}

		if (pstItemDef->Result[i].ResultID == iResultID)
		{
			pstResult->ResultVal1 += pstItemDef->Result[i].ResultVal1;
			pstResult->ResultVal2 += pstItemDef->Result[i].ResultVal2;
			pstResult->ResultVal3 += pstItemDef->Result[i].ResultVal3;
			pstResult->ResultVal4 += pstItemDef->Result[i].ResultVal4;
		}
	}

	return 0;
}


int z_arm_hideattr_calc_one(ZONESVRENV* pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef, ROLEARM *pstRoleArm)
{
	int iStarMin;
	ARMHIDEATTRDEF *pstArmHideAttrDef = NULL;
	int i;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	
	iStarMin =  z_wear_get_star_perfect_num(pstEnv, pstPlayer);
	if (iStarMin < 0)
		return -1;

	pstArmHideAttrDef = armhideattr_def_find(pstEnv, pstPlayer->stRoleData.Career, iStarMin);
	if(!pstArmHideAttrDef)
	{
		return -1;
	}

	for (i=0; i<MAX_ARM_HIDE_ATTR_RESULT; i++)
	{
		if (0 == pstArmHideAttrDef->Result[i].ResultID)
		{
			break;
		}

		arm_cacl_star_mul(pstEnv,pstDyn, pstItemDef, pstRoleArm,&pstArmHideAttrDef->Result[i]);
	}
	return 0;
}

int z_wear_get_star_perfect_num(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLEWEAR *pstWear =  &pstPlayer->stRoleData.Wear;
	ROLEARM *pstRoleArm = NULL;
	ITEMDEF *pstItemDef = NULL;
	int iCount = 0;
	int i = 0, iStarNum;
	int iStarMin = MAX_ARM_STAR_LEVEL;
	
	for(i=0; i<pstWear->ArmNum; i++)
	{
		pstRoleArm = pstWear->RoleArms+i;
		
		// 检查装备是否失效 ，失效的装备不计算属性， 也不计算套装属性
		if (pstRoleArm->ValidEnd > 0 &&
			pstRoleArm->ValidEnd < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			continue;
		}

		/*if(is_fengyin_arm(pstRoleArm))
		{
			continue;
		}*/
	
		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleArm);
		if (!pstItemDef)
		{
			continue;
		}

		
		if (pstItemDef->ArmDur.MaxDur > 0 && 0 == pstRoleArm->Dur/DUR_UNIT)
		{
			continue;
		}

		//头，衣，面，鞋，肩，项链，戒指*2，护符*2，武器（主副手）
		if(  pstItemDef->WearPos1 == WEAR_POS_HEAD ||
			 pstItemDef->WearPos2 == WEAR_POS_HEAD ||
			 pstItemDef->WearPos3 == WEAR_POS_HEAD ||
			 
			 pstItemDef->WearPos1 == WEAR_POS_BODY ||
			 pstItemDef->WearPos2 == WEAR_POS_BODY ||
			 pstItemDef->WearPos3 == WEAR_POS_BODY ||
			 
			 pstItemDef->WearPos1 == WEAR_POS_FACE ||
			 pstItemDef->WearPos2 == WEAR_POS_FACE ||
			 pstItemDef->WearPos3 == WEAR_POS_FACE ||

			 pstItemDef->WearPos1 == WEAR_POS_SHOE ||
			 pstItemDef->WearPos2 == WEAR_POS_SHOE ||
			 pstItemDef->WearPos3 == WEAR_POS_SHOE ||

		     pstItemDef->WearPos1 == WEAR_POS_SHOULDER ||
			 pstItemDef->WearPos2 == WEAR_POS_SHOULDER ||
			 pstItemDef->WearPos3 == WEAR_POS_SHOULDER ||

		     pstItemDef->WearPos1 == WEAR_POS_NECKLACE ||
			 pstItemDef->WearPos2 == WEAR_POS_NECKLACE ||
			 pstItemDef->WearPos3 == WEAR_POS_NECKLACE ||

			 pstItemDef->WearPos1 == WEAR_POS_RING ||
			 pstItemDef->WearPos2 == WEAR_POS_RING ||
			 pstItemDef->WearPos3 == WEAR_POS_RING ||

		     pstItemDef->WearPos1 == WEAR_POS_AMULET ||
			 pstItemDef->WearPos2 == WEAR_POS_AMULET1 ||
			 pstItemDef->WearPos3 == WEAR_POS_AMULET1 ||

			 pstItemDef->WearPos1 == WEAR_POS_AMULET ||
			 pstItemDef->WearPos2 == WEAR_POS_AMULET2 ||
			 pstItemDef->WearPos3 == WEAR_POS_AMULET2 ||

			 pstItemDef->WearPos1 == WEAR_POS_RIGHTHAND ||
			 pstItemDef->WearPos2 == WEAR_POS_RIGHTHAND ||
			 pstItemDef->WearPos3 == WEAR_POS_RIGHTHAND ||

			 pstItemDef->WearPos1 == WEAR_POS_LEFT ||
			 pstItemDef->WearPos2 == WEAR_POS_LEFT ||
			 pstItemDef->WearPos3 == WEAR_POS_LEFT  )
		{
			iCount = iCount + pstItemDef->WearPosNum;

			iStarNum = arm_star_perfect_num_get(pstRoleArm);
			if( iStarNum < iStarMin)
			{
				iStarMin = iStarNum;
			}			
		}
	}

	if(iCount < 12 )
	{
		return -1;
	}

	return iStarMin;
}

int z_arm_hideattr_calc(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	//ITEMDEF *pstItemDef = NULL;
	//ROLEARM *pstRoleArm = NULL;
	//ROLEWEAR *pstWear =  &pstPlayer->stRoleData.Wear;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	int i = 0;
	//int iStarNum;
	int iStarMin = MAX_ARM_STAR_LEVEL;
//	int iCount = 0;
	ARMHIDEATTRDEF *pstArmHideAttrDef = NULL;

	/*
	if( pstPlayer->stRoleData.Level < MAX_SHOW_HIDE_ATTR)
	{
		return -1;
	}

	for(i=0; i<pstWear->ArmNum; i++)
	{
		pstRoleArm = pstWear->RoleArms+i;
		
		// 检查装备是否失效 ，失效的装备不计算属性， 也不计算套装属性
		if (pstRoleArm->ValidEnd > 0 &&
			pstRoleArm->ValidEnd < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			continue;
		}


		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleArm);
		if (!pstItemDef)
		{
			continue;
		}

		
		if (pstItemDef->ArmDur.MaxDur > 0 && 0 == pstRoleArm->Dur/DUR_UNIT)
		{
			continue;
		}

		//头，衣，面，鞋，肩，项链，戒指*2，护符*2，武器（主副手）
		if(  pstItemDef->WearPos1 == WEAR_POS_HEAD ||
			 pstItemDef->WearPos2 == WEAR_POS_HEAD ||
			 pstItemDef->WearPos3 == WEAR_POS_HEAD ||
			 
			 pstItemDef->WearPos1 == WEAR_POS_BODY ||
			 pstItemDef->WearPos2 == WEAR_POS_BODY ||
			 pstItemDef->WearPos3 == WEAR_POS_BODY ||
			 
			 pstItemDef->WearPos1 == WEAR_POS_FACE ||
			 pstItemDef->WearPos2 == WEAR_POS_FACE ||
			 pstItemDef->WearPos3 == WEAR_POS_FACE ||

			 pstItemDef->WearPos1 == WEAR_POS_SHOE ||
			 pstItemDef->WearPos2 == WEAR_POS_SHOE ||
			 pstItemDef->WearPos3 == WEAR_POS_SHOE ||

		     pstItemDef->WearPos1 == WEAR_POS_SHOULDER ||
			 pstItemDef->WearPos2 == WEAR_POS_SHOULDER ||
			 pstItemDef->WearPos3 == WEAR_POS_SHOULDER ||

		     pstItemDef->WearPos1 == WEAR_POS_NECKLACE ||
			 pstItemDef->WearPos2 == WEAR_POS_NECKLACE ||
			 pstItemDef->WearPos3 == WEAR_POS_NECKLACE ||

			 pstItemDef->WearPos1 == WEAR_POS_RING ||
			 pstItemDef->WearPos2 == WEAR_POS_RING ||
			 pstItemDef->WearPos3 == WEAR_POS_RING ||

		     pstItemDef->WearPos1 == WEAR_POS_AMULET ||
			 pstItemDef->WearPos2 == WEAR_POS_AMULET1 ||
			 pstItemDef->WearPos3 == WEAR_POS_AMULET1 ||

			 pstItemDef->WearPos1 == WEAR_POS_AMULET ||
			 pstItemDef->WearPos2 == WEAR_POS_AMULET2 ||
			 pstItemDef->WearPos3 == WEAR_POS_AMULET2 ||

			 pstItemDef->WearPos1 == WEAR_POS_RIGHTHAND ||
			 pstItemDef->WearPos2 == WEAR_POS_RIGHTHAND ||
			 pstItemDef->WearPos3 == WEAR_POS_RIGHTHAND ||

			 pstItemDef->WearPos1 == WEAR_POS_LEFT ||
			 pstItemDef->WearPos2 == WEAR_POS_LEFT ||
			 pstItemDef->WearPos3 == WEAR_POS_LEFT  )
		{
			iCount = iCount + pstItemDef->WearPosNum;

			iStarNum = arm_star_perfect_num_get(pstRoleArm);
			if( iStarNum < iStarMin)
			{
				iStarMin = iStarNum;
			}			
		}
	}

	if(iCount < 12 )
	{
		return -1;
	}*/

	iStarMin =  z_wear_get_star_perfect_num(pstEnv, pstPlayer);
	if (iStarMin < 0)
		return -1;

	pstArmHideAttrDef = armhideattr_def_find(pstEnv, pstPlayer->stRoleData.Career, iStarMin);
	if(!pstArmHideAttrDef)
	{
		return -1;
	}

	for (i=0; i<MAX_ARM_HIDE_ATTR_RESULT; i++)
	{
		if (0 == pstArmHideAttrDef->Result[i].ResultID)
		{
			break;
		}

		z_attr_result(pstEnv, pstDyn, NULL, &pstArmHideAttrDef->Result[i]);
	}
	
			
	return 0;
	
}

int arm_star_total_mul_get(ZONESVRENV* pstEnv, ROLEARM*pstRoleArm)
{
	int i;
	int iMul;
	ARMSTARDEF *pstArmStarDef;

	iMul = 100;
	for(i=0; i<pstRoleArm->StarLv; i++)
	{
		pstArmStarDef = z_find_arm_star_def(pstEnv, i+1);
		if(!pstArmStarDef)
		{
			continue;
		}

		iMul = iMul*(100+pstArmStarDef->StarInfo[pstRoleArm->StarInfo[i].ValIdx].Mul)/100;
	}
	
	iMul -= 100;
	return iMul;
}

int arm_star_perfect_num_get(ROLEARM*pstRoleArm)
{
	int i;
	int iNum = 0;

	for(i=0; i<pstRoleArm->StarLv; i++)
	{
		if(pstRoleArm->StarInfo[i].ValIdx == MAX_STAR_MUL_LIST -1)
		{
			iNum++;
		}
	}
	
	return iNum;
}

//装备品质,战斗力计算
int  z_arm_star_fight(ZONESVRENV* pstEnv, DYNATTRLIST *pstDyn, ROLEARM*pstRoleArm)
{
	ITEMDEF *pstItemDef;
	STARATTR *pstStarAttr = NULL;
	ARMGRADEDEF *pstArmGradeDef = NULL;
	
	pstItemDef =  z_find_itemdef(pstEnv, pstRoleArm->DefID);
	if (!pstItemDef)
	{
		return -1;
	}

	pstArmGradeDef = armgrade_def_find(pstEnv, pstItemDef->NumType, get_arm_itemlevel(pstItemDef->WearPre.RoleLevelMin));
       if (!pstArmGradeDef)
       {
               return -1;
       }
	
	//品质计算只影响装备自身的属性提升
	if (pstRoleArm->StarLv > 0)
	{
		int iVal, iAdd;
		int iStarNum = arm_star_perfect_num_get(pstRoleArm);
		int iMul = arm_star_total_mul_get(pstEnv, pstRoleArm);

		
		//上下限浮动攻击
		iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_WEAPON_MIN_PHSIC_ATK);
		if(iVal > 0)
		{
			iAdd = iVal*iMul/100.0;
			if(iAdd <= 0 && iMul > 0)
			{
				iAdd = 1;
			}
			pstDyn->stAtkChg.iPhsicMinAtk += iAdd;
		}

		iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_WEAPON_MAX_PHSIC_ATK);
		if(iVal > 0)
		{
			iAdd = iVal*iMul/100.0;
			if(iAdd <= 0 && iMul > 0)
			{
				iAdd = 1;
			}
			pstDyn->stAtkChg.iPhsicMaxAtk += iAdd;
		}

		iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_WEAPON_MIN_MAGIC_ATK);
		if(iVal > 0)
		{
			iAdd = iVal*iMul/100.0;
			if(iAdd <= 0 && iMul > 0)
			{
				iAdd = 1;
			}
			pstDyn->stAtkChg.iIceMinAtk += iAdd;
		}

		iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_WEAPON_MAX_MAGIC_ATK);
		if(iVal > 0)
		{
			iAdd = iVal*iMul/100.0;
			if(iAdd <= 0 && iMul > 0)
			{
				iAdd = 1;
			}
			pstDyn->stAtkChg.iIceMaxAtk += iAdd;
		}

		iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_WEAPON_MIN_ATK);
		if(iVal > 0)
		{
			iAdd = iVal*iMul/100.0;
			if(iAdd <= 0 && iMul > 0)
			{
				iAdd = 1;
			}
			pstDyn->stAtkChg.iIceMinAtk += iAdd;
			pstDyn->stAtkChg.iPhsicMinAtk += iAdd;
		}

		iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_WEAPON_MAX_ATK);
		if(iVal > 0)
		{
			iAdd = iVal*iMul/100.0;
			if(iAdd <= 0 && iMul > 0)
			{
				iAdd = 1;
			}
			pstDyn->stAtkChg.iIceMaxAtk += iAdd;
			pstDyn->stAtkChg.iPhsicMaxAtk += iAdd;
		}

		//固定攻击
		iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_BASE_ATTR_PHSIC_ATK);
		if(iVal > 0)
		{
			iAdd = iVal*iMul/100.0;
			if(iAdd <= 0 && iMul > 0)
			{
				iAdd = 1;
			}
			pstDyn->stAtkChg.iPhsicMinAtk += iAdd;
			pstDyn->stAtkChg.iPhsicMaxAtk += iAdd;
		}

		iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_BASE_ATTR_MAGIC_ATK);
		if(iVal > 0)
		{
			iAdd = iVal*iMul/100.0;
			if(iAdd <= 0 && iMul > 0)
			{
				iAdd = 1;
			}
			pstDyn->stAtkChg.iIceMinAtk += iAdd;
			pstDyn->stAtkChg.iIceMaxAtk += iAdd;
		}

		iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_BASE_ATTR_HP);
		if(iVal > 0)
		{
			iAdd = iVal*iMul/100.0;
			if(iAdd <= 0 && iMul > 0)
			{
				iAdd = 1;
			}
			pstDyn->stHealChg.iHPMaxBase += iAdd;
		}
		
		iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_BASE_ATTR_PHSIC_DEF);
		if(iVal > 0)
		{
			iAdd = iVal*iMul/100.0;
			if(iAdd <= 0 && iMul > 0)
			{
				iAdd = 1;
			}
			pstDyn->stDefChg.unPhsicDefBaseAdd += iAdd;
		}

		iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_BASE_ATTR_MAGIC_DEF);
		if(iVal > 0)
		{
			iAdd = iVal*iMul/100.0;
			if(iAdd <= 0 && iMul > 0)
			{
				iAdd = 1;
			}
			pstDyn->stDefChg.unIceDefBaseAdd += iAdd;
		}

		iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_BASE_ATTR_WIND_DEF);
		if(iVal > 0)
		{
			iAdd = iVal*iMul/100.0;
			if(iAdd <= 0 && iMul > 0)
			{
				iAdd = 1;
			}
			pstDyn->stDefChg.unFireDefBaseAdd += iAdd;
		}
		
		iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_BASE_ATTR_SOIL_DEF);
		if(iVal > 0)
		{
			iAdd = iVal*iMul/100.0;
			if(iAdd <= 0 && iMul > 0)
			{
				iAdd = 1;
			}
			pstDyn->stDefChg.unWaterDefBaseAdd += iAdd;
		}

		iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_BASE_ATTR_THUNDER_DEF);
		if(iVal > 0)
		{
			iAdd = iVal*iMul/100.0;
			if(iAdd <= 0 && iMul > 0)
			{
				iAdd = 1;
			}
			pstDyn->stDefChg.unThunderDefBaseAdd += iAdd;
		}

		iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_BASE_ATTR_LIGHT_DEF);
		if(iVal > 0)
		{
			iAdd = iVal*iMul/100.0;
			if(iAdd <= 0 && iMul > 0)
			{
				iAdd = 1;
			}
			pstDyn->stDefChg.unLightDefBaseAdd += iAdd;
		}

		iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_BASE_ATTR_NIGHT_DEF);
		if(iVal > 0)
		{
			iAdd = iVal*iMul/100.0;
			if(iAdd <= 0 && iMul > 0)
			{
				iAdd = 1;
			}
			pstDyn->stDefChg.unNightDefBaseAdd += iAdd;
		}
	
		iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_BASE_ATTR_FLEE_LEVEL);
		if(iVal > 0)
		{
			iAdd = iVal*iMul/100.0;
			if(iAdd <= 0 && iMul > 0)
			{
				iAdd = 1;
			}
			pstDyn->stFleeChg.unFleeBaseAdd += iAdd;
		}
		
		iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_BASE_ATTR_HIT_LEVEL);
		if(iVal > 0)
		{
			iAdd = iVal*iMul/100.0;
			if(iAdd <= 0 && iMul > 0)
			{
				iAdd = 1;
			}
			pstDyn->stHitChg.unHitBaseAdd += iAdd;
		}

		iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_BASE_ATTR_HEAVY_HIT_VAL);
		if(iVal > 0)
		{
			iAdd = iVal*iMul/100.0;
			if(iAdd <= 0 && iMul > 0)
			{
				iAdd = 1;
			}
			pstDyn->stHitRateChg.nHeavyHitVal += iAdd;
		}
		

		/*iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_ADD_BASE_ATTR_STR);
		if(iVal > 0)
		{
			iAdd = iVal*iMul/100.0;
			if(iAdd <= 0 && iMul > 0)
			{
				iAdd = 1;
			}
			pstDyn->stMidAttrList.unLiliang += iAdd;
		}*/
	
		//品质的附加属性
		pstStarAttr = &pstArmGradeDef->ArmStar[iStarNum-1];
		calc_addattr(pstEnv, pstDyn, pstStarAttr->SuitAttrID);
		
	}

	return 0;
}

/*int  z_gem_attr(DYNATTRLIST *pstDyn, ATKCHG *pstAtkChg, ITEMDEF *pstItemDef)
{
	int i;
	
	for (i=0; i<MAX_ITEM_RESULT; i++)
	{
		if (0 == pstItemDef->Result[i].ResultID)
		{
			break;
		}
		
		z_attr_result(pstDyn, pstAtkChg, &pstItemDef->Result[i], 1);
	}

	return 0;
}*/

//装备宝石属性计算,宝石只计算其中一个镶嵌时的result
int  z_arm_gem(ZONESVRENV* pstEnv, DYNATTRLIST *pstDyn, ROLEARM *pstRoleArm)
{
	int i = 0;
	//ITEMDEF *pstItemDef = NULL;
//	int iArr = 0;
	RESULT stResult;
	GEMNEWDEF *pstGemNewDef;
	GEMATTRSTARDEF *pstAttrStarDef;
	int iVal = 0;

	for (i = 0; i <pstRoleArm->HoleNum; i++)
	{
		if (pstRoleArm->HoleGem[i].HoleGemID == 0)
		{
			continue;
		}

		pstGemNewDef = z_find_gemnewdef(pstEnv,pstRoleArm->HoleGem[i].HoleGemID);
		if (!pstGemNewDef)
			continue;

		pstAttrStarDef = z_find_gemattrstardef(pstEnv, 
								pstGemNewDef->Attr[pstRoleArm->HoleGem[i].GemAttrIdx].AttrID, 
								pstRoleArm->HoleGem[i].GemStar,pstGemNewDef->Level);
		if (!pstAttrStarDef)
			continue;

		iVal = pstAttrStarDef->Attr[pstRoleArm->HoleGem[i].HoleGemResultIndex].AttrID*
				(1+pstRoleArm->HoleGem[i].Mul/100.0f);

		memset(&stResult, 0, sizeof(stResult));
		stResult.ResultID = pstAttrStarDef->AttrID;
		switch(stResult.ResultID)
		{
		case RESULT_DEF_ICEHURT:
		case RESULT_DEF_FIREHURT:
		case RESULT_DEF_THUNDERHURT:
		case RESULT_DEF_LIGHTHURT:
		case RESULT_DEF_NIGHTHURT:
		case RESULT_DEF_PHISICHURT:
		case RESULT_DEF_MAGICHURT:
		case RESULT_ICEHURT:
		case RESULT_FIREHURT:
		case RESULT_THUNDERHURT:
		case RESULT_LIGHTHURT:
		case RESULT_NIGHTHURT:
		case RESULT_PHISICHURT:
		case RESULT_MAGICHURT:
		case RESULT_HURT:
		case RESULT_DEF_HURT:
		case RESULT_MON_FLEE:
		case RESULT_MON_DEF_HURT:
		case RESULT_MON_MAGIC_HURT:
		case RESULT_MON_ICE_HURT:
		case RESULT_MON_PHSIC_HURT:
		case RESULT_HEAL_HP:
		case RESULT_DAMAGE:
		case RESULT_ADD_BASE_ATTR_DEF:
		case RESULT_ADD_BASE_ATTR_STR:
		case RESULT_ADD_BASE_ATTR_INT:
		case RESULT_ADD_BASE_ATTR_STA:
		case RESULT_ADD_BASE_ATTR_SPR:
		case RESULT_ADD_BASE_ATTR_AGILE:
		case RESULT_HITRATE:
			stResult.ResultVal2 = iVal;
			break;
		default:

			// 这里血量计算超出short上限，借用val4
			if (iVal > 0x7fff)
			{
				stResult.ResultVal4 = iVal;
				g_cResultFlag = 1;
			}
			else
			{
				stResult.ResultVal1 = iVal;
			}
			
			break;
		}
		
		z_attr_result(pstEnv, pstDyn, pstRoleArm, &stResult);
		g_cResultFlag = 0;
	}
 
	return 0;
}

// 品质转移 
int arm_star_move(ZONESVRENV* pstEnv, Player *pstPlayer, ARMMOVESTARDATA *pstArmMoveStar )
{
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	ROLEGRID *pstSrcArmGrid =NULL;
	ROLEGRID *pstDstArmGrid =NULL;
	ITEMDEF *pstSrcItemDef = NULL;
	ITEMDEF *pstDstItemDef = NULL;
	unsigned int uiMoney = 0;
	ARMRANDDEF *pstArmMoveRandDef = NULL;
	//unsigned int uiGemNum = 0;
	int iDec = 0;
	int iBind;
	ARMSTARDEF *pstArmStarDef;

	memset(pstArmSvr,0,sizeof(CSARMSVR));
	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		pstArmSvr->Flag = 0;
		goto error;
	}

	if( pstArmMoveStar->SrcArmData.ArmListType == LIST_TYPE_WEAR ||
		pstArmMoveStar->DstArmData.ArmListType == LIST_TYPE_WEAR )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM_MOVE);
		pstArmSvr->Flag = 0;
		goto error;
	}
	
	pstSrcArmGrid = get_pak_item_grid(pstPlayer, pstArmMoveStar->SrcArmData.ArmListType, pstArmMoveStar->SrcArmData.ArmGridIdx);
	if (!pstSrcArmGrid || pstSrcArmGrid->Type != GRID_ITEM_ARM)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM_MOVE1);
		pstArmSvr->Flag = 0;
		goto error;
	}
	pstDstArmGrid = get_pak_item_grid(pstPlayer, pstArmMoveStar->DstArmData.ArmListType, pstArmMoveStar->DstArmData.ArmGridIdx);
	if (!pstDstArmGrid || pstDstArmGrid->Type != GRID_ITEM_ARM)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM_MOVE2);
		pstArmSvr->Flag = 0;
		goto error;
	}
	
	pstSrcItemDef = z_get_itemdef_roleitem(pstEnv, &pstSrcArmGrid->GridData.RoleItem);
	pstDstItemDef = z_get_itemdef_roleitem(pstEnv, &pstDstArmGrid->GridData.RoleItem);
	if(!pstSrcItemDef || !pstDstItemDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM_MOVE3);
		pstArmSvr->Flag = 0;
		goto error;
	}

	
	//lzk
	if(arm_is_badge(pstSrcItemDef) < 0 || arm_is_badge(pstDstItemDef) < 0)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ARM907);
		return -1;
	}
	
	if(arm_is_fashion(pstSrcItemDef) || arm_is_fashion(pstDstItemDef))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM_MOVE4);
		pstArmSvr->Flag = 0;
		goto error;
	}
	
	if( pstSrcItemDef->WearPos1 != pstDstItemDef->WearPos1 ||
		pstSrcItemDef->WearPos2 != pstDstItemDef->WearPos2 ||
		pstSrcItemDef->WearPos3 != pstDstItemDef->WearPos3 )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM_MOVE5);
		pstArmSvr->Flag = 0;
		goto error;
	}

/*
	if( pstDstItemDef->WearPre.RoleLevelMin - pstSrcItemDef->WearPre.RoleLevelMin > 10)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, "装备等级不符合");
		pstArmSvr->Flag = 0;
		goto error;
	}
*/

	// 2013-12-11 modify--------- start
	if ( pstSrcArmGrid->GridData.RoleArm.StarLv <=0 /*||
		 (pstSrcItemDef->StarLimit > pstDstItemDef->StarLimit)  ||
		 (pstDstItemDef->StarLimit != 0 && (pstSrcArmGrid->GridData.RoleArm.StarLv > pstDstItemDef->StarLimit))*/ )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM_MOVE6);
		pstArmSvr->Flag = 0;
		goto error;
	}
	// 2013-12-11 modify--------- end

	/*
	//转星消耗 = CEILING(装备等级 /10 , 1)* CEILING(装备等级 /10 , 1) * 5000
	uiMoney = ceil(pstDstItemDef->WearPre.RoleLevelMin/10.0)*100*pstSrcArmGrid->GridData.RoleArm.StarLv;
	if(pstDstItemDef->Important == 0)
	{
		uiMoney = uiMoney* 80.0/100;
	}
	else if(pstDstItemDef->Important == 1)
	{
		uiMoney = uiMoney* 100.0/100;
	}
	else if(pstDstItemDef->Important == 2)
	{
		uiMoney = uiMoney* 150.0/100;
	}
	else 
	{
		uiMoney = uiMoney* 300.0/100;
	}
	*/
	
	//找到品质转移的概率 
	pstArmMoveRandDef = armrand_def_find(pstEnv, ARM_START_MOVE_TYPE, pstSrcArmGrid->GridData.RoleArm.StarLv);
	if(!pstArmMoveRandDef)
	{
		pstArmSvr->Flag = 0;
		goto error;
	}

	pstArmStarDef = z_find_arm_star_def(pstEnv, arm_star_perfect_num_get(&pstSrcArmGrid->GridData.RoleArm));
	if(pstArmStarDef && pstDstItemDef->WearPre.RoleLevelMin >= 60)
	{
		uiMoney = pstDstItemDef->WearPre.RoleLevelMin*pstDstItemDef->WearPre.RoleLevelMin*pstArmStarDef->MoveMoneyVal/100/10*10;
	}
	
	//扣下钱 

	if(uiMoney > 0)
	{
		if (0 > package_dec_money_test( pstEnv, pstPlayer, ATTR_ID_MONEY, uiMoney))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM116, uiMoney, MONEY_TYPE_MONEY_NAME);
			pstArmSvr->Flag = 0;
			goto error;
		}
	}
	
	
	if(pstArmMoveStar->Bind)
	{
		iBind = PACKAGE_DEC_BIND;
	}
	else
	{
		iBind = PACKAGE_DEC_UNBIND;
	}

	if(0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, ARM_STAR_ITEM,pstArmMoveRandDef->ArmMoveStarDelGem, -1,NULL,iBind))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM117);
		pstArmSvr->Flag = 0;
		goto error;
	}
	
	//扣钱,扣宝石
	iDec = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, ARM_STAR_ITEM, pstArmMoveRandDef->ArmMoveStarDelGem, 
						-1, NULL,iBind, IMPITEM_OPTYPE_ARMSTARMOV);

	if (0 > iDec)
	{
	   	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM118);
		pstArmSvr->Flag = 0;
		goto error;
	}

	if(uiMoney > 0)
	{
		package_dec_money( pstEnv, pstPlayer, ATTR_ID_MONEY, uiMoney);
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_STARMOV, pstSrcItemDef->ItemID, 0, uiMoney, "装备转星");
	}

	//成功概率 
	if( RAND1(100) >= pstArmMoveRandDef->ArmMoveStarRand )
	{
		//扣钱,扣宝石
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM119);
		pstArmSvr->Flag = 0;
		goto _fail;
	}
	//消耗了材料，再次查找道具格子
	pstSrcArmGrid = get_pak_item_grid(pstPlayer, pstArmMoveStar->SrcArmData.ArmListType, pstArmMoveStar->SrcArmData.ArmGridIdx);
	pstDstArmGrid = get_pak_item_grid(pstPlayer, pstArmMoveStar->DstArmData.ArmListType, pstArmMoveStar->DstArmData.ArmGridIdx);
	if (!pstDstArmGrid || !pstSrcArmGrid)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "Grid error");
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM119);
		pstArmSvr->Flag = 0;
		goto _fail;
	}
	//避免指针移动
	if (pstSrcArmGrid->GridData.RoleArm.StarLv > pstDstItemDef->StarLimit)
	{
		pstDstArmGrid->GridData.RoleArm.StarLv = pstDstItemDef->StarLimit;
	}
	else
	{
		pstDstArmGrid->GridData.RoleArm.StarLv = pstSrcArmGrid->GridData.RoleArm.StarLv;
	}
	
	//pstDstArmGrid->GridData.RoleArm.StarLv = pstSrcArmGrid->GridData.RoleArm.StarLv;
	pstDstArmGrid->GridData.RoleArm.StrengthenCount = pstSrcArmGrid->GridData.RoleArm.StrengthenCount;
	pstSrcArmGrid->GridData.RoleArm.StrengthenCount = 0;

	memset(&pstDstArmGrid->GridData.RoleArm.StarInfo[0], 0,
			sizeof(pstDstArmGrid->GridData.RoleArm.StarInfo));
	
	memcpy(	&pstDstArmGrid->GridData.RoleArm.StarInfo[0], 
			&pstSrcArmGrid->GridData.RoleArm.StarInfo[0], 
			sizeof(pstDstArmGrid->GridData.RoleArm.StarInfo[0])*pstDstArmGrid->GridData.RoleArm.StarLv);
	
	pstDstArmGrid->GridData.RoleArm.InstFlag |= INST_ITEM_BIND;
	
	arm_one_grid_update(pstEnv,  pstPlayer, pstDstArmGrid);
	
	//重置材料装备星级
	pstSrcArmGrid->GridData.RoleArm.StarLv = 0;
	memset(&pstSrcArmGrid->GridData.RoleArm.StarInfo[0], 0,sizeof(pstSrcArmGrid->GridData.RoleArm.StarInfo));
	arm_one_grid_update(pstEnv,  pstPlayer, pstSrcArmGrid);	

	pstArmSvr->ArmSvrType = ARM_MOVE_STAR;
	pstArmSvr->IndexID = ARM_STAR_ITEM;
	pstArmSvr->Flag = 1;

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM120);
	
_fail:
	if(1)
	{	
		MACHINNGEXPENDDATA stExpendData;
		bzero(&stExpendData, sizeof(stExpendData));
		stExpendData.Money = uiMoney;
		itemdec_fill(&stExpendData.StuffInfo[0], ARM_STAR_ITEM, pstArmMoveRandDef->ArmMoveStarDelGem, iDec);
		stExpendData.StuffNum = 1;
		//stExpendData.StuffInfo[0].ItemID = ;
		//stExpendData.StuffInfo[0].ItemNum = pstArmMoveRandDef->ArmMoveStarDelGem;
		z_role_machining_oplog(pstEnv, pstPlayer, &pstSrcArmGrid->GridData.RoleArm, &pstDstArmGrid->GridData.RoleArm, 
			&stExpendData, OP_MACHINING_ARM_MOVE, pstArmSvr->Flag,  NULL,0);
	}

error:
	pstArmSvr->RoleName[0] = 0;
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int arm_bind(ZONESVRENV* pstEnv, Player *pstPlayer, ARMBINDDATA *pstArmBind )
{
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	ROLEGRID *pstSrcGrid =NULL;
	ITEMDEF *pstItemDef = NULL;
	ARMRANDDEF *pstArmBindRandDef = NULL;
	int iLevel = 0;
	unsigned int uiMoney = 0;
	ROLEARM stOldRoleArm;
	int iRet;

	memset(pstArmSvr,0,sizeof(CSARMSVR));

	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		goto error;
	}

	if( pstArmBind->ArmData.ArmListType == LIST_TYPE_WEAR  )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_BIND1);
		pstArmSvr->Flag = 0;
		goto error;
	}
	
	pstSrcGrid = get_pak_item_grid(pstPlayer, pstArmBind->ArmData.ArmListType, pstArmBind->ArmData.ArmGridIdx);
	if (!pstSrcGrid || pstSrcGrid->Type != GRID_ITEM_ARM)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_BIND2);
		pstArmSvr->Flag = 0;
		goto error;
	}

	memcpy(&stOldRoleArm,&pstSrcGrid->GridData.RoleArm,sizeof(stOldRoleArm));
	
	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstSrcGrid->GridData.RoleItem);
	if(!pstItemDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_BIND3);
		pstArmSvr->Flag = 0;
		goto error;
	}

	if (DIE_DROP_TYPE_DROP == pstItemDef->DieDropFlag)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_BIND4);
		pstArmSvr->Flag = 0;
		goto error;
	}
	
	if( pstSrcGrid->GridData.RoleItem.InstFlag & INST_ITEM_BIND)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_BIND5);
		pstArmSvr->Flag = 0;
		goto error;
	}

	iLevel = get_arm_itemlevel(pstItemDef->WearPre.RoleLevelMin);
	
	pstArmBindRandDef = armrand_def_find(pstEnv, ARM_BIND_TYPE, iLevel+1);
	if(!pstArmBindRandDef)
	{
		pstArmSvr->Flag = 0;
		goto error;
	}

	uiMoney = pstArmBindRandDef->ArmBindMoney;
	if(pstItemDef->Important == 0)
	{
		uiMoney = uiMoney* 80.0/100;
	}
	else if(pstItemDef->Important == 1)
	{
		uiMoney = uiMoney* 100.0/100;
	}
	else if(pstItemDef->Important == 2)
	{
		uiMoney = uiMoney* 150.0/100;
	}
	else 
	{
		uiMoney = uiMoney* 300.0/100;
	}

	//扣下钱 
	if (0 > package_dec_money_test( pstEnv, pstPlayer, ATTR_ID_BULL, uiMoney))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM116, uiMoney, MONEY_TYPE_MONEY_NAME);
		pstArmSvr->Flag = 0;
		goto error;
	}

	iRet = package_dec_money( pstEnv, pstPlayer, ATTR_ID_BULL, uiMoney);
	z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ARMBIND, pstItemDef->ItemID, uiMoney-iRet, iRet, "装备绑定");

	pstSrcGrid->GridData.RoleItem.InstFlag |= INST_ITEM_BIND;

	arm_one_grid_update(pstEnv,  pstPlayer, pstSrcGrid);
	
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_BIND6);
	pstArmSvr->Flag = 1;

		//日志信息
	if(1)
	{	
		MACHINNGEXPENDDATA stExpendData;
		bzero(&stExpendData, sizeof(stExpendData));
		stExpendData.Money = iRet;
		stExpendData.Bull = uiMoney - iRet;
		z_role_machining_oplog(pstEnv, pstPlayer, &stOldRoleArm, &pstSrcGrid->GridData.RoleArm, 
			&stExpendData, OP_MACHINING_ARM_BIND, 1, NULL,0);
	}
		
error:
	pstArmSvr->ArmSvrType = ARM_BIND;
	pstArmSvr->RoleName[0] = 0;
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int arm_unbind(ZONESVRENV* pstEnv, Player *pstPlayer, ARMUNBINDDATA *pstArmUnBind )
{
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	ROLEGRID *pstSrcGrid =NULL;
	ITEMDEF *pstItemDef = NULL;
	ARMRANDDEF *pstArmUnBindRandDef = NULL;
	int iLevel = 0;
	ITEMDEF *pstItemStuffDef = NULL;
	ROLEITEM *pstStuffRoleItem = NULL;
	int iPos = -1;
	unsigned int uiMoney = 0;

	MACHINNGEXPENDDATA stExpendData;
	ROLEARM   stOldRoleArm;
	
	bzero(&stExpendData, sizeof(stExpendData));
	memset(pstArmSvr,0,sizeof(CSARMSVR));

	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		goto error;
	}

	if( pstArmUnBind->ArmData.ArmListType == LIST_TYPE_WEAR  )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_UNBIND1);
		pstArmSvr->Flag = 0;
		goto error;
	}
	
	pstSrcGrid = get_pak_item_grid(pstPlayer, pstArmUnBind->ArmData.ArmListType, pstArmUnBind->ArmData.ArmGridIdx);
	if (!pstSrcGrid || pstSrcGrid->Type != GRID_ITEM_ARM)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_UNBIND2);
		pstArmSvr->Flag = 0;
		goto error;
	}
	
	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstSrcGrid->GridData.RoleItem);
	if(!pstItemDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_UNBIND2);
		pstArmSvr->Flag = 0;
		goto error;
	}

	memcpy(&stOldRoleArm,&pstSrcGrid->GridData.RoleArm,sizeof(stOldRoleArm));
	
	iLevel = get_arm_itemlevel(pstItemDef->WearPre.RoleLevelMin);
	
	pstArmUnBindRandDef = armrand_def_find(pstEnv, ARM_UNBIND_TYPE, iLevel+1);
	if(!pstArmUnBindRandDef)
	{
		pstArmSvr->Flag = 0;
		goto error;
	}

	if( pstArmUnBind->StuffData.ItemID == ARM_UNBIND_ITEM_LOW ||
		pstArmUnBind->StuffData.ItemID == ARM_UNBIND_ITEM_MID ||
		pstArmUnBind->StuffData.ItemID == ARM_UNBIND_ITEM_HIGH )
	{
		if (0 > package_get_item(pstEnv, pstPlayer, pstArmUnBind->StuffData.ListType, 
							pstArmUnBind->StuffData.GridIdx, &pstItemStuffDef, &pstStuffRoleItem, &iPos))
		{
			pstArmSvr->Flag = 0;
			goto error;
		}
		
		//消耗指定格子物品
		if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
								pstArmUnBind->StuffData.ItemID,  1, 
								pstArmUnBind->StuffData.ListType, &iPos,PACKAGE_DEC_NONE))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_UNBIND3);
			pstArmSvr->Flag = 0;
			goto error;
		}	

		//判断等级是否符合
		if( (pstArmUnBind->StuffData.ItemID == ARM_UNBIND_ITEM_LOW &&
			pstItemDef->WearPre.RoleLevelMin < 60) ||
			(pstArmUnBind->StuffData.ItemID == ARM_UNBIND_ITEM_MID &&
			pstItemDef->WearPre.RoleLevelMin < 90) ||
			pstArmUnBind->StuffData.ItemID == ARM_UNBIND_ITEM_HIGH )
		{
			int iDec;
			iDec = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
								pstArmUnBind->StuffData.ItemID,  1, 
								pstArmUnBind->StuffData.ListType, &iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_ARMUNBIND);
			pstSrcGrid = get_pak_item_grid(pstPlayer, pstArmUnBind->ArmData.ArmListType, pstArmUnBind->ArmData.ArmGridIdx);
			if (!pstSrcGrid)
			{
				tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "Grid error");
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_UNBIND4);
				pstArmSvr->Flag = 0;
				goto error;
			}

			stExpendData.StuffNum = 1;
			itemdec_fill(&stExpendData.StuffInfo[0], pstArmUnBind->StuffData.ItemID, 1, iDec);
			//stExpendData.StuffInfo[0].ItemID = pstArmUnBind->StuffData.ItemID;
			//stExpendData.StuffInfo[0].ItemNum = 1;
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_UNBIND5);
			pstArmSvr->Flag = 0;
			goto error;
		}
		
	}
	else
	{
		uiMoney = pstArmUnBindRandDef->ArmUnBindGodcoin;
		/*if(pstItemDef->Important == 0)
		{
			uiMoney = uiMoney* 80.0/100;
		}
		else if(pstItemDef->Important == 1)
		{
			uiMoney = uiMoney* 100.0/100;
		}
		else if(pstItemDef->Important == 2)
		{
			uiMoney = uiMoney* 150.0/100;
		}
		else 
		{
			uiMoney = uiMoney* 300.0/100;
		}*/

		//扣下钱 
		if (0 > package_dec_money_test( pstEnv, pstPlayer, ATTR_ID_GODCOIN, uiMoney))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM116, uiMoney, MONEY_TYPE_GODCOIN_NAME);
			pstArmSvr->Flag = 0;
			goto error;
		}

		stExpendData.JinBi = uiMoney;
		package_dec_money( pstEnv, pstPlayer, ATTR_ID_GODCOIN, uiMoney);
		
		godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
					pstPlayer->stRoleData.Level, GODCOIN_OPER_FLAG_UNBIND,pstItemDef->ItemID,0,0,
					uiMoney,"装备解绑");
	}

	pstSrcGrid->GridData.RoleItem.InstFlag &= ~INST_ITEM_BIND;
	
	arm_one_grid_update(pstEnv,  pstPlayer, pstSrcGrid);
	
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_UNBIND6);
	pstArmSvr->Flag = 1;

	//日志信息
	z_role_machining_oplog(pstEnv, pstPlayer, &stOldRoleArm, &pstSrcGrid->GridData.RoleArm, 
		&stExpendData, OP_MACHINING_ARM_UNBIND, 1,NULL,0);
	
error:
	pstArmSvr->ArmSvrType = ARM_UNBIND;
	pstArmSvr->RoleName[0] = 0;
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

unsigned int arm_unfengyin_use(ZONESVRENV* pstEnv, ROLEARM *pstArm)
{
	ITEMDEF *pstItemDef;
	ARMRANDDEF *pstArmRandDef;
	GEMNEWDEF *pstGemNewDef;
	int i;
	unsigned int uiUse=0;
	unsigned int uiGemUse=0;
	int iPinzhi;

	pstItemDef = z_find_itemdef(pstEnv, pstArm->DefID);
	if (!pstItemDef) 
		return 0;

	pstArmRandDef = armrand_def_find(pstEnv, ARM_UNFENGYIN_TYPE, arm_star_perfect_num_get(pstArm));
	if (!pstArmRandDef)
		return 0;

	for (i=0; i<MAX_ARM_HOLE; i++)
	{
		if (pstArm->HoleGem[i].HoleGemID <= 0)
		{
			continue;
		}
		pstGemNewDef = z_find_gemnewdef(pstEnv, pstArm->HoleGem[i].HoleGemID);
		if (!pstGemNewDef)
			continue;

		uiGemUse += pstGemNewDef->GemMerit;
	}

	if(pstItemDef->Important == 0)
	{
		iPinzhi = 80;
	}
	else if(pstItemDef->Important == 1)
	{
		iPinzhi = 100;
	}
	else if(pstItemDef->Important == 2)
	{
		iPinzhi = 150;
	}
	else if (pstItemDef->Important > 2 && pstItemDef->Important < 7)
	{
		iPinzhi = 300;
	}
	else 
	{
		iPinzhi = 500;
	}
	// 填表系数(pstArmRandDef->ArmUnFengYinUse)*
	// 品质系数(iPinzhi)*
	// 装备等级(pstItemDef->WearPre.RoleLevelMin)/10000 + 
	// 宝石价值(uiGemUse)
	uiUse = 1.0*pstArmRandDef->ArmUnFengYinUse* iPinzhi/100 *pstItemDef->WearPre.RoleLevelMin/10000 +uiGemUse;
	return uiUse>0?uiUse:1;
}

int arm_unfengyin_check(ZONESVRENV* pstEnv,int iListType,int iGridIdx,Player *pstPlayer,ROLEGRID **pstGrid,ROLEARM **pstArm)
{
	int iPos;
	ROLEPACKAGE *pstPkg = &pstPlayer->stRoleData.Package;
	ROLEGRID *pstGrid1;
	ROLEARM *pstArm1;	
	ITEMDEF *pstArmDef;
	if (iListType != LIST_TYPE_SUNDRIES)
	{
		return -1;
	}

	iPos = package_grid_pos(pstPkg->SundriesPackage.RoleGrids, pstPkg->SundriesPackage.Num, iGridIdx);
	if (0 > iPos)
	{
		return -1;
	}

	pstGrid1 = pstPkg->SundriesPackage.RoleGrids+iPos;
	if (pstGrid1->Type == ITEM_TYPE_ITEM)
		return -1;

	pstArm1 = &pstGrid1->GridData.RoleArm;
	if (!is_fengyin_arm(pstArm1))
	{
		return -1;
	}
	
	pstArmDef = z_find_itemdef(pstEnv, pstArm1->DefID);
	if(NULL == pstArmDef)
	{
		return -1;
	}
	
	if(arm_is_fashion(pstArmDef))
	{
		return -1;
	}
	if (pstGrid) *pstGrid = pstGrid1;
	if (pstArm) *pstArm = pstArm1;
	return 0;
}

int arm_unfengyin_pre(ZONESVRENV* pstEnv, Player *pstPlayer, ARMUNFENGYINPRE *pstArmUnFengYinPre)
{
	ROLEARM *pstArm=NULL;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	int iListType = pstArmUnFengYinPre->ArmData.ArmListType;
	int iGridIdx = pstArmUnFengYinPre->ArmData.ArmGridIdx;

	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		return -1;
	}
	
	if (arm_unfengyin_check(pstEnv,iListType,iGridIdx,pstPlayer,NULL,&pstArm) < 0)
	{
		return -1;
	}

	pstPlayer->stOnline.stUnFengYin.iGrid = iGridIdx;
	pstPlayer->stOnline.stUnFengYin.iListType = iListType;
	pstPlayer->stOnline.stUnFengYin.szRoleName[0] = 0;

	if (pstArm==NULL)
		return -1;
	// 被怪干死 
	if (pstArm->FengYinID <= 0)
	{
		memset(pstArmSvr,0,sizeof(*pstArmSvr));
		pstArmSvr->ArmSvrType = ARM_UNFENGYIN_PRE;
		pstArmSvr->Flag = 1;
		Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	else
	{
		// 去NameID表找名字
		z_db_unfengyin_select_nameid(pstEnv->pstAppCtx, 
			pstEnv,pstPlayer->stRoleData.RoleID,pstPlayer->stRoleData.Uin,pstArm->FengYinID);
	}

	return 0;
}


int arm_unfengyin(ZONESVRENV* pstEnv, Player *pstPlayer, ARMUNFENGYIN *pstArmUnFengYin)
{
	ROLEGRID *pstGrid = NULL;
	ROLEARM *pstArm = NULL;
	int iListType = pstArmUnFengYin->ArmData.ArmListType;
	int iGridIdx = pstArmUnFengYin->ArmData.ArmGridIdx;
	unsigned int iUse;
	int iSucc = 1;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;

	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		goto notify;
	}
	
	if (	pstPlayer->stOnline.stUnFengYin.iGrid != iGridIdx ||
		pstPlayer->stOnline.stUnFengYin.iListType != iListType)
	{
		iSucc = 0;
		goto notify;
	}

	if (arm_unfengyin_check(pstEnv,iListType,iGridIdx,pstPlayer,&pstGrid,&pstArm) < 0)
	{
		iSucc = 0;
		goto notify;
	}

	if (pstGrid == NULL || pstArm == NULL)
	{
		iSucc = 0;
		goto notify;
	}
	
	iUse = arm_unfengyin_use(pstEnv,pstArm);
	if (iUse <= 0)
	{
		iSucc = 0;
		goto notify;
	}


	if (pstEnv->pstConf->UnFyUse)
	{
		if (package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BINDGODCOIN, iUse) < 0)
		{
			iSucc = 0;
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ARM121);
			goto notify;
		}

		package_dec_money(pstEnv, pstPlayer, ATTR_ID_BINDGODCOIN, iUse);
		z_bindgold_consume_oplog(pstEnv,pstPlayer, BINDGOLD_OPTYPE_UNFENGYIN,pstArm->DefID,
						iUse,"装备解封印");
	}
	else
	{
		if (package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_GODCOIN, iUse) < 0)
		{
			iSucc = 0;
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ARM122);
			goto notify;
		}

		package_dec_money(pstEnv, pstPlayer, ATTR_ID_GODCOIN, iUse);

		godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
						pstPlayer->stRoleData.Level, GODCOIN_OPER_FLAG_UNFENGYIN,pstArm->DefID,0,0,
						iUse,"装备解封印");

		iUse/=2;
		if (	pstPlayer->stOnline.stUnFengYin.szRoleName[0] != 0 &&
			iUse > 0)
		{
			char szMsg[128];
			snprintf(szMsg,sizeof(szMsg),LJY_MAIL4,
									pstPlayer->stRoleData.RoleName,pstPlayer->stRoleData.RoleName);
			// 邮寄给封印者
			player_sysmail_fast_send(pstEnv, pstPlayer->stOnline.stUnFengYin.szRoleName, LJY_MAIL3,
										szMsg, NULL, 0, iUse,NULL, 1);
			
		}
		
		
	}
	pstArm->FengyinFlag = 0;
	pstArm->FengYinID = 0;
	memset(&pstPlayer->stOnline.stUnFengYin, 0, sizeof(pstPlayer->stOnline.stUnFengYin));

	z_role_fengyin_arm_oplog(pstEnv,pstPlayer,NULL,1,iUse,pstArm);
notify:
	if (iSucc)
	{
		// 通知客户端装备发生变化
		arm_one_grid_update(pstEnv,  pstPlayer, pstGrid);
	}
	memset(pstArmSvr,0,sizeof(*pstArmSvr));
	pstArmSvr->ArmSvrType = ARM_UNFENGYIN;
	pstArmSvr->Flag = iSucc;
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}



//根据物品等级、品质查找Fenjie项
ARMFENJIEDEF * z_find_fenjie_def(ZONESVRENV *pstEnv, unsigned short iLevel, unsigned short iImportant)
{
	int iIndex;
	int iEqu;
	ARMFENJIEDEF stFenJie;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	stFenJie.Level = iLevel;
	stFenJie.Important = iImportant;
	iIndex = bsearch_int(&stFenJie, pstObj->sArmFenJieDef, pstObj->iArmFenJieDef, ARMFENJIE_DATA, &iEqu);
	if (iEqu)
	{
		return (ARMFENJIEDEF*)pstObj->sArmFenJieDef[iIndex];
		
	}

	return NULL;
}



int arm_fenjie_check(ZONESVRENV* pstEnv,Player* pstPlayer, ARMFENJIEDEF *pstArmFenJieDef)
{
	int i;
	ROLEGRID stGridTmp;
//	int iMoney = 0;

	//吟唱时间偷个懒。不验证了。总共就0.5秒。不记录时间了
	
	for(i=0; i<MAX_ARMFENJIE_ITEM; i++)
	{	
		if(pstArmFenJieDef->GetItem[i].ItemID > 0)
		{
			if(0 == item_create(pstEnv->pstAppCtx, pstEnv, &stGridTmp, pstArmFenJieDef->GetItem[i].ItemID))
			{
				stGridTmp.GridData.RoleItem.Num = pstArmFenJieDef->GetItem[i].Num;

				if(0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stGridTmp, 1, 0))
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FENJIE1);
					return -1;
				}
			}
			else
			{
				return -1;
			}
		}
	}

	if (package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, pstArmFenJieDef->UseMoney) < 0)
	{
		return -1;
	}
	return 0;
}

int gem_fenjie_check(ZONESVRENV* pstEnv,Player* pstPlayer, GEMNEWDEF *pstGemNewDef,int iGemNum)
{
	int i,j;
	ROLEGRID stGridTmp[MAX_GEM_FENGJIE];
	int iNum = 0;
	unsigned int iMoney = 0;
	long long llMoney = 0;
	int iOldNum = 0;

	if (!pstGemNewDef) 
		return -1;

	
	for (i=0; i<MAX_GEM_FENGJIE; i++)
	{
		if (pstGemNewDef->GemFengJie[i].ID > 0)
		{
			if (	pstGemNewDef->GemFengJie[i].Max < pstGemNewDef->GemFengJie[i].Min ||
				pstGemNewDef->GemFengJie[i].Min <= 0)
			{
				return -1;
			}

			memset(&stGridTmp[i], 0, sizeof(stGridTmp[i]));
			
			for (j=0; j<iGemNum; j++)
			{
				iOldNum = stGridTmp[i].GridData.RoleItem.Num;
				if(0 == item_create(pstEnv->pstAppCtx, pstEnv, &stGridTmp[i], pstGemNewDef->GemFengJie[i].ID))
				{
					// check的时候按最多的给
					stGridTmp[i].GridData.RoleItem.Num += pstGemNewDef->GemFengJie[i].Max+iOldNum;
				}
				else
				{
					return -1;
				}
			}
			iNum ++;
		}
	}

	if (iNum <= 0)
	{
		return -1;
	}
	
	if(0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, stGridTmp, iNum, 0))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FENJIE1);
		return -1;
	}

	llMoney = pstGemNewDef->UseMoney *1.0*iGemNum;
	iMoney = pstGemNewDef->UseMoney *1.0*iGemNum;
	if (llMoney != iMoney)
	{
		return -1;
	}

	if (package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, iMoney) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NOMONEY);
		return -1;
	}

	return 0;
}

int arm_fenjie_pre_brk(ZONESVRENV* pstEnv, Player* pstPlayer, int iArmFenJiePreBrk)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	CSACTIONFENJIEPRE *pstFenJiePre = &pstActionRes->Data.FenJiePre;

	if (iArmFenJiePreBrk == 0)
	{
		pstPlayer->stOnline.State &= ~CS_STAT_ARM_FENJIE_PRE;
	}
	else
	{
		pstPlayer->stOnline.State &= ~CS_STAT_GEM_FENJIE_PRE;
	}
	
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_ARM_FENJIE_PRE;
	pstFenJiePre->PreFlag = 0;
	pstFenJiePre->Type = iArmFenJiePreBrk;
	memset(&pstFenJiePre->ArmData, 0, sizeof(pstFenJiePre->ArmData));
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	return z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
}

int arm_fenjie_pre(ZONESVRENV* pstEnv, Player* pstPlayer, ARMFENJIE* pstArmFenJie)
{	
	int iPos;
	ROLEGRID *pstGrid;
	ITEMDEF *pstItemDef = NULL;
	ROLEARM* pstRoleArm = NULL;
	ARMFENJIEDEF *pstArmFenJieDef = NULL;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	CSACTIONFENJIEPRE *pstFenJiePre = &pstActionRes->Data.FenJiePre;
	int iType = pstArmFenJie->Type;
	ROLEITEM *pstRoleItem;
	int iStat;

	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		goto _error;
	}
	//判断背包类型是否是杂物包
	if(pstArmFenJie->ArmData.ArmIdxData.ArmListType !=LIST_TYPE_SUNDRIES)
	{
		goto _error;
	}
	//物品数组索引
	iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, pstArmFenJie->ArmData.ArmIdxData.ArmGridIdx);
	if (0 > iPos)
	{
		goto _error;
	}
	pstGrid = pstPak->SundriesPackage.RoleGrids+iPos;
	pstItemDef = z_find_itemdef(pstEnv,pstGrid->GridData.RoleItem.DefID);
	if (!pstItemDef)
	{
		goto _error;
	}

	if (iType == 0) 	// arm
	{
		if(arm_is_fashion(pstItemDef))
		{
			goto _error;
		}
		pstRoleArm = &pstGrid->GridData.RoleArm;
		if (pstGrid->Type != GRID_ITEM_ARM || pstRoleArm->WID!=pstArmFenJie->ArmData.WId)
		{
			goto _error;
		}
		
		if(pstItemDef->NoResolve)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_FENJIE2);
			goto _error;
		}

		pstArmFenJieDef = z_find_fenjie_def(pstEnv,
						get_arm_itemlevel(pstItemDef->WearPre.RoleLevelMin), 
						pstItemDef->Important);
		if(!pstArmFenJieDef)
		{
			goto _error;
		}

		if(0 > arm_fenjie_check(pstEnv, pstPlayer, pstArmFenJieDef))
		{
			goto _error;
		}

		iStat = CS_STAT_ARM_FENJIE_PRE;
	}
	else				// gem
	{
		pstRoleItem = &pstGrid->GridData.RoleItem;
		if (z_item_get_result(pstItemDef, RESULT_GEM) == NULL)
		{
			//goto _error;
		}

		if (	gem_fenjie_check(pstEnv, pstPlayer, 
			z_find_gemnewdef(pstEnv, pstRoleItem->DefID),pstRoleItem->Num) < 0)
		{
			goto _error;
		}

		iStat = CS_STAT_GEM_FENJIE_PRE;
	}

	pstPlayer->stOnline.State |= iStat;
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_ARM_FENJIE_PRE;
	pstFenJiePre->PreFlag = 1;
	pstFenJiePre->Type = iType;
	pstFenJiePre->ArmData = pstArmFenJie->ArmData;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	return z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	
_error:

	arm_fenjie_pre_brk(pstEnv, pstPlayer, iType);
	return 0;
}


int arm_fenjie_get_append(ZONESVRENV* pstEnv,Player* pstPlayer, ARMFENJIEDEF *pstArmFJDef, 
							ROLEGRID *pstGridTmp, ROLEARM *pstRoleArm, int iAddRate)
{
	int i;
	int iRand = 0;
	int iGetRand;
	
	for (i=0; i<MAX_ARMFENJIE_APPEND; i++)
	{
		if (	pstArmFJDef->AppendItem[i].ItemID <= 0 ||
			pstArmFJDef->AppendItem[i].Num <= 0 ||
			pstArmFJDef->AppendItem[i].Rate <= 0)
		{
			break;
		}

		iRand += pstArmFJDef->AppendItem[i].Rate;
	}

	if (iRand <= 0)
	{
		return 0;
	}

	iGetRand = RAND1(10000);
	iGetRand-=iAddRate;
	if (iGetRand <= 0)
	{
		iGetRand = 1;
	}
	
	if (iGetRand < iRand)
	{
		iRand = 0;
		for (i=0; i<MAX_ARMFENJIE_APPEND; i++)
		{
			if (	pstArmFJDef->AppendItem[i].ItemID <= 0 ||
				pstArmFJDef->AppendItem[i].Num <= 0 ||
				pstArmFJDef->AppendItem[i].Rate <= 0)
			{
				return 0;
			}
			iRand += pstArmFJDef->AppendItem[i].Rate;
			if (iGetRand < iRand)
			{
				break;	
			}
		}

		if (i>=MAX_ARMFENJIE_APPEND)
		{
			return 0;
		}

		if (item_create(pstEnv->pstAppCtx, pstEnv, pstGridTmp, pstArmFJDef->AppendItem[i].ItemID) == 0)
		{
			pstGridTmp->GridData.RoleItem.Num = pstArmFJDef->AppendItem[i].Num;
			if (spar_item_create(pstEnv, pstRoleArm, &pstGridTmp->GridData.RoleItem) < 0)
			{
				return 0;
			}
			return pstArmFJDef->AppendItem[i].ItemID;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

//装备分解
int arm_fenjie(ZONESVRENV* pstEnv,Player* pstPlayer,ARMFENJIE* pstArmFenJie)
{
	int i,j;
	int iSucc = 0;
	int iListType = pstArmFenJie->ArmData.ArmIdxData.ArmListType;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	CSACTIONFENJIE *pstFenJie = &pstActionRes->Data.FenJie;
	ITEMDEF *pstItemDef = NULL;
	ROLEARM* pstRoleArm = NULL;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	//装备分解定义
	ARMFENJIEDEF *pstArmFenJieDef = NULL;
	int iPos = 0;
	int iRnd;	//随机数
	int iGailvTotal = 0;	//总概率数值
	ROLEGRID *pstGrid = NULL;
	int currGailvTotal = 0;
	ROLEGRID stGridTmp;
	ROLEARM stOldArm;
	MACHINNGEXPENDDATA stExpendData;
	ITEMDEC astObTainItem[MAX_OBTAIN];
	int iType = pstArmFenJie->Type;
	int iStat;
	int iGetNum=0;
	int iRet;
//	int iAdd = 0;
	ITEMDEF *pstTiLianDef;
	ROLEGRID *pstTilianGrid;
	int iTilianPos;
	int iTilianIdx = -1;
	int iNotSpar = 0;

	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		goto _error;
	}
	
	if(iListType!=LIST_TYPE_SUNDRIES)
	{
		goto _error;
	}
	
	iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, 
							pstPak->SundriesPackage.Num, 
							pstArmFenJie->ArmData.ArmIdxData.ArmGridIdx);
	if (0 > iPos)
	{
		goto _error;
	}
	pstGrid = pstPak->SundriesPackage.RoleGrids+iPos;
	if (pstGrid->GridData.RoleItem.WID != pstArmFenJie->ArmData.WId)
	{
		goto _error;	
	}
	
	pstItemDef = z_find_itemdef(pstEnv,pstGrid->GridData.RoleItem.DefID);
	if (!pstItemDef)
	{
		goto _error;
	}

	if (iType == 0 )
	{
		if(arm_is_fashion(pstItemDef))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_ARM123);
			goto _error;
		}

		if (pstGrid->Type != GRID_ITEM_ARM)
		{
			goto _error;
		}

		/*if (arm_is_uncheck(&pstGrid->GridData.RoleArm))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE, ZONEERR_ARM201);
			goto _error;
		}*/
		
		iStat = CS_STAT_ARM_FENJIE_PRE;
		if (!(pstPlayer->stOnline.State & iStat))
		{
			goto _error;
		}
		
		pstRoleArm = &pstGrid->GridData.RoleArm;
		memcpy(&stOldArm, pstRoleArm, sizeof(stOldArm));

		if (pstItemDef->WearPre.RoleLevelMin -pstPlayer->stRoleData.Level > 20)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, 
								ZONEERR_ARM124);
			goto _error;
		}
		
		if(pstItemDef->NoResolve)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
							SYS_MIDDLE2, ZONEERR_FENJIE2);
			goto _error;
		}
		
		pstArmFenJieDef = z_find_fenjie_def(pstEnv, 
						get_arm_itemlevel(pstItemDef->WearPre.RoleLevelMin), pstItemDef->Important);
		if(!pstArmFenJieDef)
		{
			goto _error;
		}

		if(0 > arm_fenjie_check(pstEnv, pstPlayer, pstArmFenJieDef))
		{
			goto _error;
		}
		
		for(i=0;i<MAX_ARMFENJIE_ITEM;i++)
		{
			iGailvTotal += pstArmFenJieDef->GetItem[i].Rate;
		}
		
		//总概率数不能为0
		if(!iGailvTotal)
		{
			goto _error;
		}

		iRnd = (int )RAND1(iGailvTotal);
		currGailvTotal = 0;
		bzero(&stExpendData, sizeof(stExpendData));
		for(i = 0; i < MAX_ARMFENJIE_ITEM; i++)
		{
			currGailvTotal += pstArmFenJieDef->GetItem[i].Rate;
			if(iRnd>=currGailvTotal)
			{
				continue; 
			}

			if(0 != item_create(pstEnv->pstAppCtx, pstEnv, &stGridTmp, pstArmFenJieDef->GetItem[i].ItemID))
			{
				goto _error;
			}

			iSucc = 1;
			stGridTmp.GridData.RoleItem.Num = pstArmFenJieDef->GetItem[i].Num;
			
			//log
			stExpendData.StuffNum = 1;
			itemdec_fill(&stExpendData.StuffInfo[0], pstRoleArm->DefID, 1, 
						(pstRoleArm->InstFlag&INST_ITEM_BIND)?1:0);
			itemdec_fill(&astObTainItem[0],pstArmFenJieDef->GetItem[i].ItemID,pstArmFenJieDef->GetItem[i].Num,
				(stGridTmp.GridData.RoleItem.InstFlag&INST_ITEM_BIND)?pstArmFenJieDef->GetItem[i].Num:0);
			iGetNum ++;
			break;
		}

		package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRoleArm->DefID,1, iListType, &iPos, PACKAGE_DEC_NONE, IMPITEM_OPTYPE_ARMFENJIE);
		iRet = package_dec_money(pstEnv, pstPlayer, ATTR_ID_BULL, pstArmFenJieDef->UseMoney);
		stExpendData.Money =  iRet;
		stExpendData.Bull = pstArmFenJieDef->UseMoney - iRet;
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_FENJIE, pstItemDef->ItemID, pstArmFenJieDef->UseMoney-iRet, iRet, "分解");
		package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stGridTmp, 1, 0, IMPITEM_OPTYPE_ARMFENJIE);
		
		// 附加道具
		// 只有鉴定的才出晶石
		if (!arm_is_uncheck(&pstGrid->GridData.RoleArm))
		{
			if (arm_fenjie_get_append(pstEnv, pstPlayer, pstArmFenJieDef,  &stGridTmp, &stOldArm, 0) > 0)
			{
				itemdec_fill(&astObTainItem[1],stGridTmp.GridData.RoleItem.DefID,stGridTmp.GridData.RoleItem.Num,
					(stGridTmp.GridData.RoleItem.InstFlag&INST_ITEM_BIND)?stGridTmp.GridData.RoleItem.Num:0);
				iGetNum ++;
				package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stGridTmp, 1, 0, IMPITEM_OPTYPE_ARMFENJIE);
			}
		}
		
		z_role_machining_oplog(pstEnv, pstPlayer, &stOldArm, NULL, 
							&stExpendData, OP_MACHINING_ARM_FENJIE, 1, astObTainItem, iGetNum);
	}
	else if (iType == 2)
	{
		if(arm_is_fashion(pstItemDef))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_ARM125);
			goto _error;
		}

		if (pstItemDef->WearPre.RoleLevelMin -pstPlayer->stRoleData.Level > 20)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_ARM126);
			goto _error;
		}

		if (genius_get_mod(pstEnv, pstPlayer) < 100)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_ARM127	);
			goto _error;	
		}

		if (pstGrid->Type != GRID_ITEM_ARM)
		{
			goto _error;
		}

		if (arm_is_uncheck(&pstGrid->GridData.RoleArm))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE, ZONEERR_ARM201);
			goto _error;
		}
		
		pstRoleArm = &pstGrid->GridData.RoleArm;
		memcpy(&stOldArm, pstRoleArm, sizeof(stOldArm));


		iTilianPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, 
							pstPak->SundriesPackage.Num, 
							pstArmFenJie->TiLianData.ArmIdxData.ArmGridIdx);
		if (0 > iTilianPos)
		{
			goto _error;
		}
		pstTilianGrid = pstPak->SundriesPackage.RoleGrids+iTilianPos;
		if (pstTilianGrid->GridData.RoleItem.WID != pstArmFenJie->TiLianData.WId)
		{
			goto _error;	
		}
		
		pstTiLianDef = z_find_itemdef(pstEnv,pstTilianGrid->GridData.RoleItem.DefID);
		if (!pstTiLianDef)
		{
			goto _error;
		}

		if (pstTiLianDef->Result[0].ResultID != RESULT_TILIAN)
		{
			goto _error;
		}

		if (pstTiLianDef->Result[0].ResultVal4 < pstItemDef->Important)
		{
			goto _error;
		}

		for (i=0; i<pstRoleArm->AddAttrNum; i++)
		{
			NEWADDATTRDEF *pstNewAddAttrDef;
			pstNewAddAttrDef = z_find_new_addattr_def(pstEnv, pstRoleArm->AddAttr[i].AddAttrID);
			if(!pstNewAddAttrDef ||pstNewAddAttrDef->NotSpar )
			{
				iNotSpar ++;
			}	
		}

		if (iNotSpar >= pstRoleArm->AddAttrNum)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_RONGYAN5);
			goto _error;
		}

		pstArmFenJieDef = z_find_fenjie_def(pstEnv, 
						get_arm_itemlevel(pstItemDef->WearPre.RoleLevelMin), pstItemDef->Important);
		if(!pstArmFenJieDef)
		{
			goto _error;
		}

		if (package_dec_money_test(pstEnv, pstPlayer, pstTiLianDef->Result[0].ResultVal2,  pstTiLianDef->Result[0].ResultVal3) < 0)
		{
			goto _error;
		}

		if (arm_fenjie_get_append(pstEnv, pstPlayer, pstArmFenJieDef,  &stGridTmp, &stOldArm, pstTiLianDef->Result[0].ResultVal1) > 0)
		{
			package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRoleArm->DefID,1, iListType, &iPos, 
						PACKAGE_DEC_NONE, IMPITEM_OPTYPE_ARMFENJIE);

			iTilianIdx = package_get_free_grid(pstPak->SundriesPackage.RoleGrids, 
						pstPak->SundriesPackage.Num,pstPak->SundriesPackage.MaxNum);
			
			package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stGridTmp, 1, 0, IMPITEM_OPTYPE_ARMFENJIE);
			iSucc = 1;
		}

		iTilianPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, 
							pstPak->SundriesPackage.Num, 
							pstArmFenJie->TiLianData.ArmIdxData.ArmGridIdx);
		if (0 > iTilianPos)
		{
			goto _error;
		}

		package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstTiLianDef->ItemID,1, iListType, &iTilianPos, 
						PACKAGE_DEC_NONE, IMPITEM_OPTYPE_ARMFENJIE);

		iRet = package_dec_money(pstEnv, pstPlayer, pstTiLianDef->Result[0].ResultVal2,  pstTiLianDef->Result[0].ResultVal3) ;
		if (ATTR_ID_GODCOIN == pstTiLianDef->Result[0].ResultVal2)
		{
			godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
						pstPlayer->stRoleData.Level, GODCOIN_OPER_FLAG_FENJIE,pstItemDef->ItemID,0,0,
						pstTiLianDef->Result[0].ResultVal3,"分解");
		}
		else if (ATTR_ID_BINDGODCOIN == pstTiLianDef->Result[0].ResultVal2)
		{
			z_bindgold_consume_oplog(pstEnv,pstPlayer, BINDGOLD_OPTYPE_FENJIE,pstItemDef->ItemID,
						pstTiLianDef->Result[0].ResultVal3,"分解");
		}
		else if (ATTR_ID_BULL == pstTiLianDef->Result[0].ResultVal2)
		{
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_FENJIE, 
				pstItemDef->ItemID, pstTiLianDef->Result[0].ResultVal3-iRet, iRet, "分解");
		}
		else if (ATTR_ID_MONEY == pstTiLianDef->Result[0].ResultVal2)
		{
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_FENJIE, 
				pstItemDef->ItemID, 0, pstTiLianDef->Result[0].ResultVal3, "分解");
		}
	}
	else if (iType == 1)
	{
		ROLEITEM *pstRoleItem;
		GEMNEWDEF *pstGemNewDef;
		ROLEGRID astGrid[MAX_GEM_FENGJIE];
		int iNum = 0;
		unsigned int iMoney = 0;
		int iRand;
		int iBind = 0;
		
		pstRoleItem = &pstGrid->GridData.RoleItem;

		if (pstRoleItem->InstFlag&INST_ITEM_BIND)
		{
			iBind |=INST_ITEM_BIND;
		}

		iStat = CS_STAT_GEM_FENJIE_PRE;
		if (!(pstPlayer->stOnline.State & iStat))
		{
			goto _error;
		}
		
		if (z_item_get_result(pstItemDef, RESULT_GEM) == NULL)
		{
			//goto _error;
		}

		pstGemNewDef = z_find_gemnewdef(pstEnv, pstRoleItem->DefID);
		if (!pstGemNewDef)
		{
			goto _error;	
		}
		
		if (	gem_fenjie_check(pstEnv, pstPlayer,pstGemNewDef, pstRoleItem->Num) < 0)
		{
			goto _error;
		}

		iMoney = pstGemNewDef->UseMoney * pstRoleItem->Num;
		iSucc = 1;
	
		bzero(&stExpendData, sizeof(stExpendData));
		iGetNum = 0;
		for (i=0; i<MAX_GEM_FENGJIE; i++)
		{
			if (pstGemNewDef->GemFengJie[i].ID <= 0)
			{
				break;
			}

			for (j=0; j<pstRoleItem->Num; j++)
			{
				if ( j == 0)
				{
					if(0 != item_create(pstEnv->pstAppCtx, pstEnv, &astGrid[i], pstGemNewDef->GemFengJie[i].ID))
					{
						iSucc = 0;
						goto _error;
					}
					astGrid[i].GridData.RoleItem.Num = 0;
				}
				iRand = 	pstGemNewDef->GemFengJie[i].Min+
						RAND1(pstGemNewDef->GemFengJie[i].Max - 
						pstGemNewDef->GemFengJie[i].Min+1);
				astGrid[i].GridData.RoleItem.Num +=  iRand;
				astGrid[i].GridData.RoleItem.InstFlag = iBind;
			}
			iNum ++;

			if (iGetNum <MAX_OBTAIN )
			{
				itemdec_fill(&astObTainItem[i],astGrid[i].GridData.RoleItem.DefID,astGrid[i].GridData.RoleItem.Num,
						(astGrid[i].GridData.RoleItem.InstFlag&INST_ITEM_BIND)?astGrid[i].GridData.RoleItem.Num:0);
				iGetNum++;
			}		
		}

		stExpendData.Money = iMoney;
		stExpendData.StuffNum = 1;
		itemdec_fill(&stExpendData.StuffInfo[0], pstGemNewDef->ItemID, pstRoleItem->Num, 
					(pstRoleItem->InstFlag&INST_ITEM_BIND)?1:0);

		package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRoleItem->DefID,
						pstRoleItem->Num, iListType, &iPos, PACKAGE_DEC_NONE, IMPITEM_OPTYPE_ARMFENJIE);
		package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, iMoney);

		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_FENJIE, pstItemDef->ItemID, 0, iMoney, "分解");
		
		package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, astGrid, iNum, 0, IMPITEM_OPTYPE_ARMFENJIE);
		z_role_machining_oplog(pstEnv, pstPlayer, NULL, NULL, 
							&stExpendData, OP_MACHINING_GEM_FENJIE, 1, astObTainItem, iGetNum);
	}
	
	
_final:

	pstPlayer->stOnline.State &= ~iStat;
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_ARM_FENJIE;
	pstFenJie->Succ = iSucc;
	pstFenJie->Type = iType;
	pstFenJie->GridIdx = iTilianIdx;
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);

	if (iType == 2)
	{
		if (iSucc)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_ARM128);	
		}
	}

	if(pstFenJie->Succ)
	{
		task_player_trace(pstEnv, pstPlayer, TASK_OBJ_FENJIE);
	}
	
	//成长之路
	if (iSucc && iType == 1)
	{
		ACHIEVEMENTCOND stCond;

		memset(&stCond,0,sizeof(stCond));
		
		stCond.Type = COND_EQUIP_BREAK;

		z_player_achievement_cond(pstEnv,pstPlayer,&stCond);

		
	}
	return z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	
_error:
	iSucc = 0;
	goto _final;
	return 0;
}

// 获取操作的装备
int get_op_arm(ZONESVRENV* pstEnv, Player *pstPlayer, int iListType, int iGridIdx, 
					ROLEARM	 **ppstRoleArm,ROLEGRID **ppstRoleGrid)
{
	int i;
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	ROLEARM	   *pstRoleArm = NULL;
	ROLEGRID  *pstRoleGrid = NULL;
	
	if( iListType == LIST_TYPE_WEAR)
	{
		for(i = 0; i<pstRoleWear->ArmNum; i++)
		{
			if(pstRoleWear->RoleArms[i].GridIdx == iGridIdx)
			{
				break;
			}
		}
		if(i>=pstRoleWear->ArmNum)
		{
			return -1;
		}
		pstRoleArm= &pstRoleWear->RoleArms[i];
	}
	else if (iListType == LIST_TYPE_SUNDRIES)
	{
		pstRoleGrid = get_pak_item_grid(pstPlayer, iListType, iGridIdx);
		if (!pstRoleGrid)
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}

	*ppstRoleGrid = pstRoleGrid;
	*ppstRoleArm = pstRoleArm;
	UNUSED(pstEnv);
	return 0;
}


int gem_hole_check(ROLEARM *pstRoleArm, int iHoleIdx, int iOp)
{
	HOLEGEM *pstHole;
	if (iHoleIdx <= 0 || iHoleIdx > pstRoleArm->HoleNum)
	{
		return -1;
	}

	if (GEM_RESET_ATTR == iOp)
	{
		pstHole = &pstRoleArm->HoleGem[iHoleIdx-1];
		if (pstHole->HoleGemID <= 0)
		{
			return -1;
		}
	}
	
	return 0;
}

int arm_reset_gem_use(ZONESVRENV* pstEnv, Player *pstPlayer, int iUseType,GEMNEWDEF *pstGemNewDef,int iCheck)
{
	int iID = 0;
	int iMoney = 0;
	char *pszMoneyName;

	//120919策划将金券和金币名称互换，但没动表格
	if (iUseType == 0)		// ATTR_ID_BINDGODCOIN 		金券
	{
		iID = ATTR_ID_BINDGODCOIN;
		iMoney = pstGemNewDef->ReSet.JinBi;
		pszMoneyName = MONEY_TYPE_BIND_GODCOIN_NAME;
	}
	else if (iUseType == 1)	// ATTR_ID_GODCOIN			金币
	{
		iID = ATTR_ID_GODCOIN;
		iMoney = pstGemNewDef->ReSet.JinQuan;
		pszMoneyName = MONEY_TYPE_GODCOIN_NAME;
	}
	else
	{
		return -1;
	}

	if (iCheck)
	{
		if (package_dec_money_test(pstEnv, pstPlayer, iID, iMoney) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOENOUGH, pszMoneyName);
			return -1;
		}
	}
	else
	{
		package_dec_money(pstEnv, pstPlayer, iID, iMoney)	;

		if (iID == ATTR_ID_GODCOIN)
		{
			godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
					pstPlayer->stRoleData.Level, GODCOIN_OPER_FLAG_ARMRESET,pstGemNewDef->ItemID,0,0,
					iMoney,"宝石重置");
		}
		else if (iID == ATTR_ID_BINDGODCOIN)
		{
			z_bindgold_consume_oplog(pstEnv, pstPlayer, BINDGOLD_OPTYPE_ARMRESET, pstGemNewDef->ItemID,
					iMoney, "宝石重置");
		}
				
		return 0;
	}
	
	return 0;
}

int arm_reset_in(ZONESVRENV* pstEnv, Player *pstPlayer, HOLEGEM *pstHole,GEMNEWDEF *pstGemNewDef,int *piBest)
{
	GEMATTRSTARDEF *pstAttrStarDef;
	int iIdx;
	int i;
	int iOldval;

	if (pstHole->GemAttrIdx >= MAX_GEM_ATTR)
	{
		return -1;
	}
	
	pstAttrStarDef = z_find_gemattrstardef(pstEnv, pstGemNewDef->Attr[pstHole->GemAttrIdx].AttrID, 
										pstHole->GemStar,pstGemNewDef->Level);
	if (!pstAttrStarDef)
	{
		return -1;
	}

	if (pstHole->HoleGemResultIndex >=  MAX_GEM_VAL)
	{
		return -1;
	}
	iOldval = pstAttrStarDef->Attr[pstHole->HoleGemResultIndex].AttrID;
	iIdx = gem_get_rand_attr(pstAttrStarDef->Attr, MAX_GEM_VAL); // 数值随机
	if (iIdx < 0)
		return -1;

	pstHole->HoleGemResultIndex = iIdx;

	iIdx = gem_get_rand_attr(pstGemNewDef->Attr, MAX_GEM_ATTR); // 属性随机
	if (iIdx < 0)
		return -1;

	pstHole->GemAttrIdx = iIdx;
	
	*piBest = 2; 							// 完美镶嵌
	for (i=0; i<MAX_GEM_VAL; i++)
	{
		if (pstAttrStarDef->Attr[i].AttrID > pstAttrStarDef->Attr[iIdx].AttrID)
		{
			*piBest = 1;
			break;
		}
	}

	if (iOldval == pstAttrStarDef->Attr[pstHole->HoleGemResultIndex].AttrID)
	{
		// 数值没变化，不需要通知客户端
		// return 1;
	}
	UNUSED(pstPlayer);
	return 0;
}


int arm_reset_fill_online(ZONESVRENV* pstEnv, Player *pstPlayer, GemReSetOL *pstReSetOL,
						GEMNEWDEF *pstGemNewDef, HOLEGEM *pstHole)
{
	GEMATTRSTARDEF *pstAttrStarDef;
	int iIdx;
//	int i;

	if (pstHole->GemAttrIdx >= MAX_GEM_ATTR)
	{
		return -1;
	}
	
	pstAttrStarDef = z_find_gemattrstardef(pstEnv, pstGemNewDef->Attr[pstHole->GemAttrIdx].AttrID, 
										pstHole->GemStar,pstGemNewDef->Level);
	if (!pstAttrStarDef)
	{
		return -1;
	}

	if (pstHole->HoleGemResultIndex >=  MAX_GEM_VAL)
	{
		return -1;
	}

	iIdx = gem_get_rand_attr(pstAttrStarDef->Attr, MAX_GEM_VAL); // 数值随机
	if (iIdx < 0)
		return -1;

	pstReSetOL->cMul = gem_get_rand_mul(pstAttrStarDef, &iIdx); // 修正
	pstReSetOL->cValIdx = iIdx;

	iIdx = gem_get_rand_attr(pstGemNewDef->Attr, MAX_GEM_ATTR); // 属性随机
	if (iIdx < 0)
		return -1;
	pstReSetOL->cAttrIdx = iIdx;

	return 0;
}

int arm_reset_gem_save(ZONESVRENV* pstEnv, Player *pstPlayer, GEMRESETSAVE *pstSave)
{
	ROLEARM	 *pstRoleArm=NULL;
	ROLEGRID *pstArmGrid=NULL;
	GemReSetOL *pstGemResetOL = &pstPlayer->stOnline.stGemReSetOL;
	HOLEGEM *pstHole;
	int iRet = 0;
	int iOldAttr, iOldVal,iOldMul;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	
	if (	get_op_arm(pstEnv, pstPlayer, pstSave->ArmData.ArmListType, 
					pstSave->ArmData.ArmGridIdx,&pstRoleArm,&pstArmGrid) < 0)
	{
		goto final;
	}

	if (pstArmGrid)
	{
		pstRoleArm = &pstArmGrid->GridData.RoleArm;
	}

	if (!pstRoleArm || pstRoleArm->WID != pstGemResetOL->ullArmWID)
		goto final;

	if (	pstGemResetOL->cHoleIdx < 1 ||
		pstGemResetOL->cHoleIdx > MAX_ARM_HOLE)
	{
		goto final;
	}
	
	pstHole = &pstRoleArm->HoleGem[(int)(pstGemResetOL->cHoleIdx-1)];
	if (pstHole->HoleGemID <= 0)
	{
		goto final;
	}

	iOldAttr = pstHole->GemAttrIdx;
	iOldVal = pstHole->HoleGemResultIndex;
	iOldMul = pstHole->Mul ;
	pstHole->GemAttrIdx = pstGemResetOL->cAttrIdx;
	pstHole->HoleGemResultIndex = pstGemResetOL->cValIdx;
	pstHole->Mul = pstGemResetOL->cMul;

	iRet = 1;
	if (	iOldAttr != pstHole->GemAttrIdx ||
		iOldVal != pstHole->HoleGemResultIndex ||
		iOldMul != pstHole->Mul)
	{
		if (pstSave->ArmData.ArmListType == LIST_TYPE_WEAR)
		{
			arm_wear_update( pstEnv,  pstPlayer, pstRoleArm);
			z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}	
		else
		{
			if (pstArmGrid)
			{
				arm_one_grid_update(pstEnv,  pstPlayer, pstArmGrid);
			}
		}	
	}
final:

	
	memset(pstArmSvr, 0, sizeof(*pstArmSvr));
	pstArmSvr->Flag = iRet;
	pstArmSvr->ArmSvrType = GEM_RESET_ATTR_SAVE;
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

/*	if (iRet)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_SAVE_SUCC);
	}
	else
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_SAVE_FAIL);
	}*/
	return 0;
}

int arm_reset_gem(ZONESVRENV* pstEnv, Player *pstPlayer, GEMRESET *pstGemReSet)
{
	ROLEARM	 *pstRoleArm=NULL;
	ROLEGRID *pstArmGrid=NULL;
	ITEMDEF *pstItemDef;
	GEMNEWDEF *pstGemNewDef;
	int iRet;
	ROLEARM stOldArm;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	MACHINNGEXPENDDATA stExpendData;
	GemReSetOL *pstReSetOL = &pstPlayer->stOnline.stGemReSetOL;

	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		goto error;
	}
	
	if (get_op_arm(pstEnv, pstPlayer, pstGemReSet->ArmData.ArmListType, 
					pstGemReSet->ArmData.ArmGridIdx,&pstRoleArm,&pstArmGrid) < 0)
	{
		goto error;
	}

	if (pstArmGrid)
	{
		pstRoleArm = &pstArmGrid->GridData.RoleArm;
	}

	if (!pstRoleArm)
		goto error;

	stOldArm = *pstRoleArm;
	
	pstItemDef = z_find_itemdef(pstEnv, pstRoleArm->DefID);
	if (!pstItemDef)
	{
		goto error;
	}

	if (gem_hole_check(pstRoleArm, pstGemReSet->ArmHoleIdx, GEM_RESET_ATTR) < 0)
	{
		goto error;
	}

	pstGemNewDef = z_find_gemnewdef(pstEnv, pstRoleArm->HoleGem[pstGemReSet->ArmHoleIdx-1].HoleGemID);
	if (!pstGemNewDef)
	{
		goto error;
	}

	if (arm_reset_gem_use(pstEnv, pstPlayer, pstGemReSet->UseType,pstGemNewDef, 1) < 0)
	{
		goto error;
	}

	memset(pstReSetOL, 0, sizeof(*pstReSetOL));
	pstReSetOL->ullArmWID = pstRoleArm->WID;
	pstReSetOL->cHoleIdx = pstGemReSet->ArmHoleIdx;
	
	iRet = arm_reset_fill_online(pstEnv, pstPlayer, pstReSetOL,pstGemNewDef,
						&pstRoleArm->HoleGem[pstGemReSet->ArmHoleIdx-1]);

	//iRet = arm_reset_in(pstEnv, pstPlayer, &pstRoleArm->HoleGem[pstGemReSet->ArmHoleIdx-1],
	//					pstGemNewDef, &iBest);
	if (iRet < 0)
	{
		goto error;
	}

	arm_reset_gem_use(pstEnv, pstPlayer, pstGemReSet->UseType,pstGemNewDef, 0);

	// 通知变化
	/*if (iRet != 1)
	{
		if (pstGemReSet->ArmData.ArmListType == LIST_TYPE_WEAR)
		{
			arm_wear_update( pstEnv,  pstPlayer, pstRoleArm);
			z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}	
		else
		{
			if (pstArmGrid)
			{
				arm_one_grid_update(pstEnv,  pstPlayer, pstArmGrid);
			}
		}
	}*/

	if(pstGemReSet->UseType == 0 && !(pstRoleArm->InstFlag & INST_ITEM_BIND))
	{
		pstRoleArm->InstFlag |= INST_ITEM_BIND;
		if (pstGemReSet->ArmData.ArmListType == LIST_TYPE_WEAR)
		{
			arm_wear_update( pstEnv,  pstPlayer, pstRoleArm);
		}	
		else
		{
			if (pstArmGrid)
			{
				arm_one_grid_update(pstEnv,  pstPlayer, pstArmGrid);
			}
		}	
	}

	// todo:clt notify
	memset(pstArmSvr, 0, sizeof(*pstArmSvr));
	pstArmSvr->Flag = 1;
	pstArmSvr->ArmSvrType = GEM_RESET_ATTR;
	pstArmSvr->ValIdx = pstReSetOL->cValIdx;
	pstArmSvr->AttrIdx = pstReSetOL->cAttrIdx;
	pstArmSvr->Mul = pstReSetOL->cMul;
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	// todo log
	bzero(&stExpendData, sizeof(stExpendData));

	//120919，策划把金币和金券名称互换但没动表格
	if (pstGemReSet->UseType == 0)
	{
		stExpendData.JinQuan = pstGemNewDef->ReSet.JinBi;
	}
	else
	{
		stExpendData.JinBi= pstGemNewDef->ReSet.JinQuan;
	}
	z_role_machining_oplog(pstEnv, pstPlayer, &stOldArm, pstRoleArm, &stExpendData, 
							OP_MACHINING_GEM_RESET, pstArmSvr->Flag, NULL,0);
	return 0;
	
error:

	memset(pstArmSvr, 0, sizeof(*pstArmSvr));
	memset(pstReSetOL, 0, sizeof(*pstReSetOL));
	pstArmSvr->IndexID  = pstGemReSet->ArmHoleIdx;
	pstArmSvr->ArmSvrType = GEM_RESET_ATTR;
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
//	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GEM_RESET1);
	return -1;	
	
}


int gem_get_sorb_num(ZONESVRENV* pstEnv, int iLevel, int iStar, int iCurrP)
{
	int iSorb = iCurrP;
	GEMLEVELSTARDEF *pstLevelStarDef;
		
	while((pstLevelStarDef = z_find_gemlevelstardef(pstEnv,iLevel, iStar)))
	{
		iStar++;
		iSorb += pstLevelStarDef->PowerUp;
	}

	return iSorb;
}

int gem_add_power_check_stuff(ZONESVRENV* pstEnv, Player *pstPlayer, GEMADDPOWER *pstGemAddPower,
									ROLEARM *pstRoleArm, ROLEGRID *pstRoleGrid, TotalStuffGridTmp *pstGridTmp, int *piBind,int *piUse)
{
	int i,j;
	int iPos;
	ITEMDEF *pstItemDef;
	RESULT *pstResult;
	long long iPower = 0;
	ROLEGRID *pstGird;
	ROLEITEM *pstRoleItem;
	GEMNEWDEF *pstGemNewDef;
	GEMLEVELSTARDEF *pstLevelStarDef;
	char *pszTip;
	USEDEF *pstUseDef = NULL;
	*piBind = 0;

	int iMoneyType;
	int iUse;
	HOLEGEM *pstHole;
	int iHoleIdx = pstGemAddPower->ArmHoleIdx;
	int iStar,iCurrP;
	int iTopPower = 0; // 宝石升到顶需要的能量点
	
	int iFreeflag = 0; // 是否是不消耗钱的宝石充能道具
	int iFreeGet = 0; // 免费的数额

	memset(pstGridTmp, 0, sizeof(*pstGridTmp));

	pstUseDef = z_find_usedef(pstEnv, USE_ID_POWER);
	if (!pstUseDef)
	{
		return -1;
	}

	if (pstRoleArm)
	{
		if (iHoleIdx <= 0 || iHoleIdx > pstRoleArm->HoleNum )
		{
			return -1;
		}
		pstHole = &pstRoleArm->HoleGem[iHoleIdx-1];
		pstGemNewDef = z_find_gemnewdef(pstEnv, pstHole->HoleGemID);
		if (!pstGemNewDef)
			return -1;
		iStar = pstHole->GemStar;
		iCurrP = pstHole->GemPower;

	}
	else if (pstRoleGrid)
	{
		if (pstRoleGrid->Type == GRID_ITEM_ARM)
		{
			pstHole = &pstRoleGrid->GridData.RoleArm.HoleGem[iHoleIdx-1];
			iStar = pstHole->GemStar;
			iCurrP = pstHole->GemPower;
			pstGemNewDef = z_find_gemnewdef(pstEnv, pstHole->HoleGemID);
			if (!pstGemNewDef)
				return -1;
		}
		else
		{
			iStar = pstRoleGrid->GridData.RoleItem.InstVal1;
			iCurrP = pstRoleGrid->GridData.RoleItem.InstVal2;
			pstGemNewDef = z_find_gemnewdef(pstEnv, pstRoleGrid->GridData.RoleItem.DefID);
			if (!pstGemNewDef)
				return -1;
		}
	}
	else
	{
		return -1;
	}

	pstLevelStarDef = z_find_gemlevelstardef(pstEnv,pstGemNewDef->Level, iStar);
	if (!pstLevelStarDef)
		return -1;


	if (pstGemAddPower->OpType == 1)		// vip用金券或金币升级
	{
		if (pstGemAddPower->UseType == 0)
		{
			iMoneyType = ATTR_ID_BINDGODCOIN;
			iUse = pstUseDef->UseJinbi;
			pszTip = MONEY_TYPE_BIND_GODCOIN_NAME;
		}
		else
		{
			iMoneyType = ATTR_ID_GODCOIN;
			iUse = pstUseDef->UseJinquan;
			pszTip = MONEY_TYPE_GODCOIN_NAME;
		}
		
		if (pstLevelStarDef->PowerUp - iCurrP <= 0)
		{
				return -1;
		}

		if (package_dec_money_test(pstEnv, pstPlayer,iMoneyType, 
									(pstLevelStarDef->PowerUp - iCurrP)*iUse) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_NOENOUGH,pszTip);
			return -1;
		}

		*piUse = pstLevelStarDef->PowerUp - iCurrP;
		
		return pstLevelStarDef->PowerUp - iCurrP; 	// vip升级只升到下一个星级
	}

	iTopPower = gem_get_sorb_num(pstEnv, pstGemNewDef->Level, iStar+1,  pstLevelStarDef->PowerUp - iCurrP);
	if (iTopPower <= 0)
	{
		return -1;
	}

	int k;
	for (i=0; i<pstGemAddPower->StuffNum; i++)
	{
		for (k=i+1; k<pstGemAddPower->StuffNum; k++)
		{
			if (pstGemAddPower->StuffData[i].GridIdx == pstGemAddPower->StuffData[k].GridIdx)
			{
				return -1;
			}
		}
		pstGird = NULL;
		pstItemDef = z_find_itemdef(pstEnv, pstGemAddPower->StuffData[i].ItemID);
		if (!pstItemDef)
			return -1;

		if (iPower >= iTopPower)
		{
			break;
		}
		
		iPos = pkg_get_pos( 	pstPlayer, pstGemAddPower->StuffData[i].ListType, 
							pstGemAddPower->StuffData[i].GridIdx);
		if (iPos < 0)
		{
			return -1;
		}

		pstGird = get_pak_item_grid(pstPlayer, pstGemAddPower->StuffData[i].ListType,
											pstGemAddPower->StuffData[i].GridIdx);
		if (!pstGird)
		{
			return -1;
		}

		if (pstGird->GridData.RoleItem.InstFlag &INST_ITEM_BIND)
		{
			*piBind = 1;	
		}

		iFreeflag  = 0; 
		if(z_item_get_result(pstItemDef, RESULT_GEM_ADD_POWER_FREE))
		{
			iFreeflag = 1;
		}
		
		pstResult = z_item_get_result(pstItemDef, RESULT_GEM_CHIP);
		if (pstResult)
		{
			if (pstResult->ResultVal1 <= 0)
			{
				return -1;
			}

			for (j=0; j<pstGemAddPower->StuffData[i].ItemNum; j++)
			{
				if (iPower >= iTopPower)
				{
					break;
				}

				iPower += 1.0*pstResult->ResultVal1;

				if(iFreeflag == 1)
				{
					iFreeGet += 1.0*pstResult->ResultVal1;
				}
			}

			if (j==0)
			{
				break;
			}
			if (0 > tmp_grid_total(pstGridTmp, pstGemAddPower->StuffData[i].ItemID, j, iPos, pstGemAddPower->StuffData[i].ListType))
			{
				return -1;
			}
		}
		else
		{
			
			pstRoleItem = &pstGird->GridData.RoleItem;
			pstGemNewDef = z_find_gemnewdef(pstEnv, pstGemAddPower->StuffData[i].ItemID);
			if (!pstGemNewDef)
			{
				return -1;
			}
			pstLevelStarDef = z_find_gemlevelstardef(pstEnv,pstGemNewDef->Level, pstRoleItem->InstVal1);
			if (!pstLevelStarDef)
			{
				return -1;
			}

			if (pstLevelStarDef->GetPower <= 0)
			{
				return -1;
			}

			/*if(pstGemNewDef->Level >= GEM_HECHENG_LEVEL )
			{
				// 3级级以上宝石不可能叠加
				if (pstGemAddPower->StuffData[i].ItemNum > 1)
				{
					return -1;
				}

				// 3级级以上宝石加上能量点
				iPower += 1.0*pstLevelStarDef->GetPower + pstRoleItem->InstVal2;
				if (0 > tmp_grid_total(pstGridTmp, pstGemAddPower->StuffData[i].ItemID,1 , iPos, pstGemAddPower->StuffData[i].ListType))
				{
					return -1;
				}
			}
			else
			{*/
				for (j=0; j<pstGemAddPower->StuffData[i].ItemNum; j++)
				{
					if (iPower >= iTopPower)
					{
						break;
					}

					iPower += 1.0*pstLevelStarDef->GetPower+ pstRoleItem->InstVal2;

					if(iFreeflag == 1)
					{
						iFreeGet += 1.0*pstLevelStarDef->GetPower+ pstRoleItem->InstVal2;
					}
				}

				if (j==0)
				{
					break;
				}
				if (0 > tmp_grid_total(pstGridTmp, pstGemAddPower->StuffData[i].ItemID, j, iPos, pstGemAddPower->StuffData[i].ListType))
				{
					return -1;
				};
			//}
			
		}
	}

	for(i=0; i<pstGridTmp->iNum; i++)
	{
		if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
								pstGridTmp->stGrid[i].iItemID,  pstGridTmp->stGrid[i].iNum, 
								pstGridTmp->stGrid[i].iListType, 
								&pstGridTmp->stGrid[i].iPos,PACKAGE_DEC_NONE))
		{
			return -1;
		}
	}

	if (iPower > 0 && iPower < 0x7fffffff)
	{
		//int iMoney = iPower*pstUseDef->UseMoney;
		//long long llTmp = iPower*pstUseDef->UseMoney;
		
		int iTempMoney = 0;
		int iTempPower = 0;
		long long llTmp = 0; 

		iTempPower = iPower; 
		if(iFreeGet > 0)
		{
			//把免费的减掉
			 iTempPower = iTempPower - iFreeGet;
		}

		iTempMoney = iTempPower*pstUseDef->UseMoney;
		llTmp = iTempPower*pstUseDef->UseMoney;
		
		if (llTmp != iTempMoney)
		{
			return -1;
		}

		if (package_dec_money_test(pstEnv, pstPlayer,ATTR_ID_MONEY, iTempMoney) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_NOMONEY);
			return -1;
		}

		*piUse = iTempPower;
		return iPower;
	}
	
	return -1;
}


int gem_add_power_in(ZONESVRENV* pstEnv, Player *pstPlayer,
						ROLEARM* pstRoleArm,ROLEGRID *pstRoleGrid, 
						GEMADDPOWER *pstGemAddPower, int iPower, int iBind)
{
	HOLEGEM *pstHole;
	int iStar;
	GEMNEWDEF *pstGemNewDef;
	GEMLEVELSTARDEF *pstLevelStarDef;
	int iCurrP,iOldP;
	int iFlag = 0;
	int iGemNewID;

	if (pstRoleArm)
	{
		if (pstGemAddPower->ArmHoleIdx <= 0 || pstGemAddPower->ArmHoleIdx > pstRoleArm->HoleNum )
		{
			return -1;
		}
		pstHole = &pstRoleArm->HoleGem[pstGemAddPower->ArmHoleIdx-1];
		pstGemNewDef = z_find_gemnewdef(pstEnv, pstHole->HoleGemID);
		if (!pstGemNewDef)
			return -1;
		iStar = pstHole->GemStar;
		iCurrP = pstHole->GemPower;

	}
	else if (pstRoleGrid)
	{
		if (pstRoleGrid->Type == GRID_ITEM_ARM)
		{
			if (pstGemAddPower->ArmHoleIdx <= 0 || pstGemAddPower->ArmHoleIdx > pstRoleGrid->GridData.RoleArm.HoleNum)
			{
				return -1;
			}
			pstHole = &pstRoleGrid->GridData.RoleArm.HoleGem[pstGemAddPower->ArmHoleIdx-1];
			iGemNewID = pstHole->HoleGemID;
			iStar = pstHole->GemStar;
			iCurrP = pstHole->GemPower;
		}
		else
		{
			iGemNewID = pstRoleGrid->GridData.RoleItem.DefID;
			iStar = pstRoleGrid->GridData.RoleItem.InstVal1;
			iCurrP = pstRoleGrid->GridData.RoleItem.InstVal2;
		}
		pstGemNewDef = z_find_gemnewdef(pstEnv,iGemNewID);
		if (!pstGemNewDef)
			return -1;
		
	}
	else
	{
		return -1;
	}

	if (pstGemNewDef->Level < GEM_HECHENG_LEVEL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ARM129, GEM_HECHENG_LEVEL);
		return -1;
	}

	pstLevelStarDef = z_find_gemlevelstardef( pstEnv, pstGemNewDef->Level, iStar);
	if (!pstLevelStarDef || pstLevelStarDef->PowerUp <= 0)
	{
		return -1;
	}

	// 1-3级宝石不能充能, 	PowerUp字段填0
	// 最高级别宝石5星,		PowerUp字段填0
	if (pstLevelStarDef->PowerUp <= 0 )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ARM130);
		return -1;
	}

	// 宝石最高星级
//	iMaxStar = (pstGemNewDef->Level -2)*9;
	
	// 如果策划改表了导致当前power比上限要高的情况
	if (iCurrP >= pstLevelStarDef->PowerUp && iCurrP > 0)
	{
		iCurrP = pstLevelStarDef->PowerUp -1;
	}

	if (iPower < pstLevelStarDef->PowerUp -iCurrP)
	{
		iCurrP += iPower;
	}
	else
	{
		while(iPower > 0)
		{
			iFlag = 0;
			iOldP = iCurrP;
			iCurrP += iPower;
			iPower -= (pstLevelStarDef->PowerUp-iOldP);
			if (iCurrP >= pstLevelStarDef->PowerUp)
			{
				iCurrP = 0;
				iStar ++;
				iFlag = 1;			// 星级变了,重新拉取等级对照表
			}
			
			if (!iFlag)
				continue;
			
			pstLevelStarDef = z_find_gemlevelstardef( pstEnv, pstGemNewDef->Level, iStar);
			if (!pstLevelStarDef || pstLevelStarDef->PowerUp <= 0)
			{
				break;
			}
		}
	}

	if (pstRoleArm)
	{
		// 属性和数值还是原来的索引
		pstHole->GemStar = iStar;
		pstHole->GemPower = iCurrP;
		/*if (iNextGemID > 0)
		{
			pstHole->HoleGemID = iNextGemID;
		}*/

		if(pstGemAddPower->OpType == 1 && pstGemAddPower->UseType == 0)
		{
			pstRoleArm->InstFlag |= INST_ITEM_BIND;
		}

		if (iBind)
		{
			pstRoleArm->InstFlag |= INST_ITEM_BIND;	
		}

		arm_wear_update( pstEnv,  pstPlayer, pstRoleArm);
		z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}
	else
	{
		if (pstRoleGrid->Type == GRID_ITEM_ARM)
		{
			pstHole = &pstRoleGrid->GridData.RoleArm.HoleGem[pstGemAddPower->ArmHoleIdx-1];
			pstHole->GemStar = iStar;
			pstHole->GemPower = iCurrP;
			/*if (iNextGemID > 0)
			{
				pstHole->HoleGemID = iNextGemID;
			}*/
			
			if(pstGemAddPower->OpType == 1 && pstGemAddPower->UseType == 0)
			{
				pstRoleGrid->GridData.RoleArm.InstFlag |= INST_ITEM_BIND;
			}

			if (iBind)
			{
				pstRoleGrid->GridData.RoleArm.InstFlag |= INST_ITEM_BIND;
			}
			
			arm_one_grid_update(pstEnv,  pstPlayer, pstRoleGrid);
		}
		else
		{
			pstRoleGrid->GridData.RoleItem.InstVal1 = iStar;
			pstRoleGrid->GridData.RoleItem.InstVal2 = iCurrP;
			/*if (iNextGemID > 0)
			{
				pstRoleGrid->GridData.RoleItem.DefID = iNextGemID;
			}*/

			if(pstGemAddPower->OpType == 1 && pstGemAddPower->UseType == 0)
			{
				pstRoleGrid->GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
			}

			if (iBind)
			{
				pstRoleGrid->GridData.RoleArm.InstFlag |= INST_ITEM_BIND;
			}
			
		 	item_one_grid_update(pstEnv,  pstPlayer, pstRoleGrid);
		}
	}
	return 0;
}

int gem_add_power(ZONESVRENV* pstEnv, Player *pstPlayer, GEMADDPOWER *pstGemAddPower)
{
	ITEMDEF *pstItemDef;
	ROLEARM	 *pstRoleArm=NULL;
	ROLEGRID *pstRoleGrid=NULL;
	int i;
	int iPower;
	int iItemID;
	int iRet;
	int iDec;
	ROLEARM stOldArm;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	int iMoneyType;
	int iUse;
	tdr_ulonglong ullWid;
	TotalStuffGridTmp stGridTmp;
	int iBind = 0;
	int iMoneyUse = 0;
		
	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		goto error;
	}
	
	if (get_op_arm(pstEnv, pstPlayer, pstGemAddPower->ArmData.ArmListType, 
					pstGemAddPower->ArmData.ArmGridIdx,&pstRoleArm,&pstRoleGrid) < 0)
	{
		goto error;
	}

	if (LIST_TYPE_WEAR == pstGemAddPower->ArmData.ArmListType)
	{
		if (!pstRoleArm)
		{
			goto error;
		}
		stOldArm = *pstRoleArm;
		iItemID = pstRoleArm->DefID;
		ullWid = pstRoleArm->WID;
	}
	else
	{
		if (!pstRoleGrid)
		{
			goto error;
		}
		iItemID = pstRoleGrid->GridData.RoleItem.DefID;
		if (pstRoleGrid->Type == GRID_ITEM_ARM)
		{
			stOldArm = pstRoleGrid->GridData.RoleArm;
		}
		else
		{
			if (pstRoleGrid->GridData.RoleItem.Num > 1)
			{
				z_send_sysmsg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_ARM908);
				goto error;
			}
		}

		ullWid = pstRoleGrid->GridData.RoleItem.WID;
	}
	
	pstItemDef = z_find_itemdef(pstEnv, iItemID);
	if (!pstItemDef)
	{
		goto error;
	}

	USEDEF *pstUseDef = z_find_usedef(pstEnv, USE_ID_POWER);
	if (!pstUseDef)
	{
		goto error;
	}

	iPower = gem_add_power_check_stuff(pstEnv,pstPlayer, pstGemAddPower,pstRoleArm,pstRoleGrid,&stGridTmp, &iBind,&iMoneyUse);
	if (iPower <= 0)
	{
		goto error;	
	}

	iRet = gem_add_power_in(pstEnv, pstPlayer,pstRoleArm,pstRoleGrid, 
							pstGemAddPower,iPower, iBind);
	if(iRet < 0)
	{
		goto error;	
	}

	int iPos;
	MACHINNGEXPENDDATA stExpendData;
	bzero(&stExpendData, sizeof(stExpendData));

	if (pstGemAddPower->OpType == 1)
	{
		//120919，策划把金币和金券名称互换但没动表格
		if (pstGemAddPower->UseType == 0)
		{
			iMoneyType = ATTR_ID_BINDGODCOIN;
			iUse = pstUseDef->UseJinbi;
			//stExpendData.JinQuan = iUse*iPower;
			stExpendData.JinQuan = iUse*iMoneyUse;
		}
		else
		{
			iMoneyType = ATTR_ID_GODCOIN;
			iUse = pstUseDef->UseJinquan;
			//stExpendData.JinBi = iUse*iPower;
			stExpendData.JinBi = iUse*iMoneyUse;
		}
	}
	else
	{
		iMoneyType = ATTR_ID_MONEY;
		iUse = pstUseDef->UseMoney;
		//stExpendData.Money = iUse*iPower;
		stExpendData.Money = iUse*iMoneyUse;

		for (i=0; i<stGridTmp.iNum; i++)
		{
			iPos = pkg_get_pos( 	pstPlayer, pstGemAddPower->StuffData[i].ListType, 
								pstGemAddPower->StuffData[i].GridIdx);
			if (iPos < 0)
				return -1;
			
			iDec = package_dec(	pstEnv->pstAppCtx, pstEnv, pstPlayer, 
								stGridTmp.stGrid[i].iItemID,
								stGridTmp.stGrid[i].iNum, 
								stGridTmp.stGrid[i].iListType, 
								&iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_GEMADDPOWER);

			if (stExpendData.StuffNum < MAX_ARM_STUFF)
			{
				itemdec_fill(&stExpendData.StuffInfo[stExpendData.StuffNum++],
							stGridTmp.stGrid[i].iItemID, 
							stGridTmp.stGrid[i].iNum, iDec);
			}
		}
		
		/*for (i=0; i<pstGemAddPower->StuffNum; i++)
		{
			
			iPos = pkg_get_pos( 	pstPlayer, pstGemAddPower->StuffData[i].ListType, 
								pstGemAddPower->StuffData[i].GridIdx);
			iDec = package_dec(	pstEnv->pstAppCtx, pstEnv, pstPlayer, 
								pstGemAddPower->StuffData[i].ItemID,
								pstGemAddPower->StuffData[i].ItemNum, 
								pstGemAddPower->StuffData[i].ListType, 
								&iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_GEMADDPOWER);

			if (stExpendData.StuffNum < MAX_ARM_STUFF)
			{
				itemdec_fill(&stExpendData.StuffInfo[stExpendData.StuffNum++],
							pstGemAddPower->StuffData[i].ItemID, 
							pstGemAddPower->StuffData[i].ItemNum, iDec);
			}
		}*/
	}

	/* 
	package_dec_money(pstEnv, pstPlayer,iMoneyType, iUse*iPower);
	if (ATTR_ID_GODCOIN == iMoneyType)
	{
		godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
					pstPlayer->stRoleData.Level, GODCOIN_OPER_FLAG_ARMPOWER,iItemID,0,0,
					iUse*iPower,"宝石充能");
	}
	else if (ATTR_ID_BINDGODCOIN == iMoneyType)
	{
		z_bindgold_consume_oplog(pstEnv, pstPlayer, BINDGOLD_OPTYPE_ARMPOWER, iItemID, 
					iUse*iPower,"宝石充能");
	}
	else if (ATTR_ID_MONEY == iMoneyType)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ADDPOWER, iItemID, 0, iUse*iPower, "宝石充能");
	}
	*/

	package_dec_money(pstEnv, pstPlayer,iMoneyType, iUse*iMoneyUse);
	if (ATTR_ID_GODCOIN == iMoneyType)
	{
		godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
					pstPlayer->stRoleData.Level, GODCOIN_OPER_FLAG_ARMPOWER,iItemID,0,0,
					iUse*iMoneyUse,"宝石充能");
	}
	else if (ATTR_ID_BINDGODCOIN == iMoneyType)
	{
		z_bindgold_consume_oplog(pstEnv, pstPlayer, BINDGOLD_OPTYPE_ARMPOWER, iItemID, 
					iUse*iMoneyUse,"宝石充能");
	}
	else if (ATTR_ID_MONEY == iMoneyType)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ADDPOWER, iItemID, 0, iUse*iMoneyUse, "宝石充能");
	}
	
	pstArmSvr->Flag = 1;
	pstArmSvr->IndexID  = -1;
	if (LIST_TYPE_WEAR == pstGemAddPower->ArmData.ArmListType)
	{
		pstArmSvr->IndexID = pstGemAddPower->ArmHoleIdx;
	}
	pstArmSvr->ArmSvrType = GEM_ADD_POWER;
	pstArmSvr->RoleName[0] = 0;
	pstArmSvr->WID = ullWid;
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	// todo: log
	if (pstRoleArm)
	{
		z_role_machining_oplog(pstEnv, pstPlayer, &stOldArm, pstRoleArm, &stExpendData, 
							OP_MACHINING_GEM_POWER, pstArmSvr->Flag, NULL,0);
	}
	else
	{
		if (pstRoleGrid->Type == GRID_ITEM_ARM)
		{
			z_role_machining_oplog(pstEnv, pstPlayer, &stOldArm, &pstRoleGrid->GridData.RoleArm, &stExpendData, 
							OP_MACHINING_GEM_POWER, pstArmSvr->Flag, NULL,0);
		}
		else
		{
			z_role_machining_oplog(pstEnv, pstPlayer, NULL, NULL, &stExpendData, 
							OP_MACHINING_GEM_POWER, pstArmSvr->Flag, NULL,0);
		}
	}
	return 0;
	
error:

	pstArmSvr->Flag = 0;
	pstArmSvr->IndexID  = -1;
	if (LIST_TYPE_WEAR == pstGemAddPower->ArmData.ArmListType)
	{
		pstArmSvr->IndexID = pstGemAddPower->ArmHoleIdx;
	}
	pstArmSvr->ArmSvrType = GEM_ADD_POWER;
	pstArmSvr->RoleName[0] = 0;
	pstArmSvr->WID = ullWid;
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
//	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_GEM_POWER);
	return -1;	
}


/*	-lxk
	重铸装备条件，则扣除相应的材料和金钱
	返回值:
			>=0 根据类型返回金钱值(即普通重铸OPERATE_TYPE_ONCE:返回银币，高级重铸类型:返回金币OR金券)
			< 0 重铸装备条件不满足
	参数:iDec表示普通重铸时消耗的是否是绑定材料
*/
int arm_recast_dec(ZONESVRENV *pstEnv,Player *pstPlayer,ITEMDEF *pstArmDef,ARMRECASTDATA *pstArmRecastData,int iDec, MACHINNGEXPENDDATA *pstExpend)
{
	int iMoney = 0;
	ITEMDEF *pstItemStuffDef;
	RESULT *pstResult;
	int iRet;
	
	if(!pstArmDef)
	{
		return -1;
	}
	if(pstArmRecastData->OperateType == OPERATE_TYPE_ONCE)
	{
		if(pstArmRecastData->Must == 0)
		{
			pstItemStuffDef = z_find_itemdef(pstEnv,pstArmRecastData->ItemID);
			if(!pstItemStuffDef)
			{
				return -1;
			}
			pstResult = z_item_get_result(pstItemStuffDef, RESULT_ARM_RECAST);
			if(!pstResult)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM131);
				return -1;
			}
			if(pstResult->ResultVal1 < pstArmDef->WearPre.RoleLevelMin)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM132);
				return -1;
			}
		}
		else
		{
			pstItemStuffDef = z_find_itemdef(pstEnv,pstArmRecastData->ItemID);
			if(!pstItemStuffDef)
			{
				return -1;
			}
			pstResult = z_item_get_result(pstItemStuffDef, RESULT_ARM_RECAST_MUST);
			if(!pstResult)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM131);
				return -1;
			}
			if(pstResult->ResultVal1 < pstArmDef->WearPre.RoleLevelMin)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM132);
				return -1;
			}
		}
		
		//重铸消耗银币=power(ceiling(装备等级/10,1),2)*20
		iMoney = ceil(pstArmDef->WearPre.RoleLevelMin/10.0)*5;
		if ( 0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, iMoney) )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM133, MONEY_TYPE_MONEY_NAME);
			return -1;
		}

		if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
							 pstArmRecastData->ItemID, 1, LIST_TYPE_SUNDRIES, NULL,iDec))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM134);
			return -1;
		}
		
		
		iRet = package_dec_money(pstEnv, pstPlayer, ATTR_ID_BULL, iMoney);
		pstExpend->Money = iRet;
		pstExpend->Bull = iMoney - iRet;
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ARMRECAST, pstArmDef->ItemID, iMoney-iRet, iRet, "装备重铸");
				
		package_dec(pstEnv->pstAppCtx,pstEnv,pstPlayer,pstArmRecastData->ItemID,1,
						LIST_TYPE_SUNDRIES,NULL,iDec, IMPITEM_OPTYPE_ARMRECAST);
		
	}
	else
	{
		int level = pstArmDef->WearPre.RoleLevelMin;
		
		if(level < 40)
		{
			iMoney = LEVEL40;
		}
		else if(level < 60)
		{
			iMoney = LEVEL60;
		}
		else if(level < 80)
		{
			iMoney = LEVEL80;
		}
		else if(level < 100)
		{
			iMoney = LEVEL100;
		}
		else if(level <= 120)
		{
			iMoney = LEVEL120;
		}
		if(pstArmRecastData->OperateType == OPERATE_TYPE_BATCH)
		{
			iMoney *=10;
		}
		//ATTR_ID_BINDGODCOIN
		if(pstArmRecastData->ConsumeType == CONSUME_TYPE_BINDGODCOIN)
		{
			if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BINDGODCOIN, iMoney))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM135, MONEY_TYPE_BIND_GODCOIN_NAME);
				return -1;
			}
			package_dec_money(pstEnv, pstPlayer, ATTR_ID_BINDGODCOIN, iMoney);
			z_bindgold_consume_oplog(pstEnv,pstPlayer,BINDGOLD_OPTYPE_ARMRECAST, pstArmDef->ItemID, iMoney,"装备重铸");
			pstExpend->JinQuan = iMoney;
		}
		else
		{
			if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_GODCOIN, iMoney))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM135, MONEY_TYPE_GODCOIN_NAME);
				return -1;
			}
			package_dec_money(pstEnv, pstPlayer, ATTR_ID_GODCOIN, iMoney);
			godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
					pstPlayer->stRoleData.Level, GODCOIN_OPER_FLAG_ARMRECAST,pstArmDef->ItemID,0,0,
					iMoney,"装备重铸");
			pstExpend->JinBi = iMoney;
		}
	}
	return iMoney;
}

/*	-lxk
	根据背包类型[杂物包]、格子位置查找装备
*/
ROLEARM * get_rolearm_byIdx(Player * pstPlayer,int iListType,int iIdx)
{
	int iPos = 0;
	ROLEARM * pstArm = NULL;
	ROLEGRID *pstGrid;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	if(iListType == LIST_TYPE_SUNDRIES)
	{
		iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, 
								pstPak->SundriesPackage.Num, 
								iIdx);
		if (0 > iPos)
		{
			return NULL;
		}
		pstGrid = pstPak->SundriesPackage.RoleGrids+iPos;
		if(pstGrid->Type != GRID_ITEM_ARM)
		{
			return NULL;
		}
		pstArm = &pstGrid->GridData.RoleArm;
	}
	/*
	else if(iListType == LIST_TYPE_WEAR)
	{
		pstArm = player_get_wear_rolearm_by_grididx(pstPlayer, iIdx);
		if(!pstArm)
		{
			return NULL;
		}
	}
	*/
	return pstArm;
}

/*	-lxk
	装备重铸
*/
int arm_recast(ZONESVRENV * pstEnv,Player * pstPlayer,ARMRECASTDATA * pstArmRecastData)
{
	int i,j,iSucc,iNum,iResultID,iMul,iBind, fBind = 0;
	ROLEARM *pstArm;
	ROLEGRID *pstArmGrid;
	ITEMDEF *pstItemArmDef;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	CSRECASTINFO *pstCsRecastInfo = &pstActionRes->Data.Recast;
	//角色身上保存的重铸信息
	RECASTINFO *pstRecaseInfo =  &pstPlayer->stOnline.stRecaseInfo;	
	RECASTITEMSDETAIL *pstRecastItemsDetail;
	MACHINNGEXPENDDATA stExpendData;
	
	bzero(&stExpendData, sizeof(stExpendData));
	iSucc = 1;
	iBind = INI_VALUE;	//消耗的材料类型(绑定OR非绑定的),并根据此值判断是否消耗材料
	
	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		goto _error;
	}
	
	//查找装备实例
	pstArm = get_rolearm_byIdx(pstPlayer,pstArmRecastData->ArmInfo.ListType,
							   pstArmRecastData->ArmInfo.Idx);
	if(!pstArm)
	{
		goto _error;
	}
	//根据装备查找道具定义
	pstItemArmDef = z_get_itemdef_roleitem( pstEnv, (ROLEITEM * )pstArm);
	if(!pstItemArmDef)
	{
		goto _error;
	}

	//lzk
	if(arm_is_badge(pstItemArmDef) < 0 )
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ARM909);
		goto _error;
	}

	if(arm_is_fashion(pstItemArmDef))
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ARM910);
		goto _error;
	}

	if(pstArmRecastData->Must && pstArmRecastData->OperateType != OPERATE_TYPE_ONCE)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ARM911);
		goto _error;
	}
	
	//普通重铸 消耗的重铸石是否绑定
	if(pstArmRecastData->OperateType == OPERATE_TYPE_ONCE)
	{
		if(pstArmRecastData->Bind)
		{
			iBind = PACKAGE_DEC_BIND;
			fBind = 1;
		}
		else
		{
			iBind = PACKAGE_DEC_UNBIND;
		}
	}
	else
	{
		if(pstArmRecastData->ConsumeType == CONSUME_TYPE_BINDGODCOIN)
		{
			fBind = 1;
		}
	}
	
	//检查重铸条件，如果条件满足，则扣除相应的材料及金钱，返回消费的钱(银子、金币、金券)
	if (0 > arm_recast_dec(pstEnv,pstPlayer,pstItemArmDef,pstArmRecastData,iBind, &stExpendData))
	{
		goto _error;
	}

	//如果消耗了背包中的材料，根据位置重新查找装备
	if(iBind != INI_VALUE)
	{
		pstArm = get_rolearm_byIdx(pstPlayer,pstArmRecastData->ArmInfo.ListType,
									pstArmRecastData->ArmInfo.Idx);
		if(!pstArm)
		{
			goto _error;
		}
	}
	
	//重铸次数:批量=10，单次=1
	if(pstArmRecastData->OperateType == OPERATE_TYPE_BATCH)
	{
		iNum = MAX_ARM_RECAST_BATCH;
	}
	else
	{
		iNum = 1;
	}

	memset(pstRecaseInfo, 0, sizeof(*pstRecaseInfo));
	pstRecaseInfo->WID = pstArm->WID;

	//随机装备基础属性值
	for(i = 0; i < iNum; i++)
	{
		for(j = 0; j < MAX_ITEM_RESULT; j++)
		{
			iResultID = pstItemArmDef->Result[j].ResultID;
			if (0 == iResultID)
			{
				break;
			}
			iMul = arm_base_attr_rand_get(pstEnv,iResultID);

			pstRecastItemsDetail = &pstRecaseInfo->RecastItemsDetail[i];
			if(pstRecastItemsDetail->BaseAttrNum == MAX_ARM_BASE_ATTR)
			{
				memmove(&pstRecastItemsDetail->BaseInfo[0],
						&pstRecastItemsDetail->BaseInfo[1],
						(MAX_ARM_BASE_ATTR - 1) * sizeof(pstRecastItemsDetail->BaseInfo[0]));
				pstRecastItemsDetail->BaseAttrNum--;
					    
			}

			if(pstArmRecastData->Must)
			{
				pstRecastItemsDetail->BaseInfo[pstRecastItemsDetail->BaseAttrNum].ResultID = iResultID;
				pstRecastItemsDetail->BaseInfo[pstRecastItemsDetail->BaseAttrNum].Mul = 100;
				pstRecastItemsDetail->BaseAttrNum++;
			}
			else
			{
				pstRecastItemsDetail->BaseInfo[pstRecastItemsDetail->BaseAttrNum].ResultID = iResultID;
				pstRecastItemsDetail->BaseInfo[pstRecastItemsDetail->BaseAttrNum].Mul = iMul;
				pstRecastItemsDetail->BaseAttrNum++;
			}
		}

		pstRecaseInfo->RecaseNum++;
	}
	
	if(fBind && !(pstArm->InstFlag & INST_ITEM_BIND))
	{
		pstArm->InstFlag |= INST_ITEM_BIND;
		pstArmGrid = get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstArm->GridIdx);
		arm_one_grid_update(pstEnv,  pstPlayer, pstArmGrid);
	}

	
	//日志
	{
		if(pstArmRecastData->OperateType == OPERATE_TYPE_ONCE)
		{
			
			stExpendData.StuffNum = 1;
			itemdec_fill(&stExpendData.StuffInfo[0], pstArmRecastData->ItemID, 1, iBind);
		}
		
		z_role_machining_oplog(pstEnv, pstPlayer, pstArm, NULL, 
							   &stExpendData, OP_MACHINING_ARM_RECAST_CONSUME, 1, NULL,0);
	}
	
	_final:

		pstActionRes->ID = pstPlayer->iMemID;
		pstActionRes->ActionID = ACTION_ARM_RECAST;
		pstCsRecastInfo->Succ = iSucc;
		if(iSucc)
		{
			memcpy(&pstCsRecastInfo->RecastInfo,pstRecaseInfo,sizeof(pstCsRecastInfo->RecastInfo));

			//成长之路
			{
				ACHIEVEMENTCOND stCond;

				memset(&stCond,0,sizeof(stCond));
				
				stCond.Type = COND_EQUIP_REBUILD;

				z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
			}

			task_player_trace(pstEnv, pstPlayer, TASK_OBJ_ARM_RECAST);
		}
		else
		{
			memset(&pstCsRecastInfo->RecastInfo,0,sizeof(pstCsRecastInfo->RecastInfo));
		}
		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		return 0;
	_error:
		iSucc = 0;
		goto _final;
		return 0;
}

/*	-lxk
	保存装备重铸结果
*/
int arm_recast_save(ZONESVRENV * pstEnv,Player * pstPlayer,ARMSAVERECASTDATA * pstArmSaveRecastData)
{
	int i;
	ROLEARM stOldArm;
	ROLEARM *pstArm;

	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		return -1;
	}
	//查找装备实例
	pstArm = get_rolearm_byIdx(pstPlayer,pstArmSaveRecastData->ArmInfo.ListType,
							   pstArmSaveRecastData->ArmInfo.Idx);
	if(!pstArm)
	{
		return -1;
	}
	if(pstArm->WID != pstPlayer->stOnline.stRecaseInfo.WID)
	{
		return -1;
	}
	if(pstPlayer->stOnline.stRecaseInfo.RecaseNum <= pstArmSaveRecastData->selectIdx)
	{
		return -1;
	}
	memcpy(&stOldArm,pstArm,sizeof(stOldArm));

	RECASTITEMSDETAIL *pstRecastItemsDetail = &pstPlayer->stOnline.stRecaseInfo.RecastItemsDetail[pstArmSaveRecastData->selectIdx];
	//设置装备基础属性
	for(i = 0; i < pstRecastItemsDetail->BaseAttrNum; i++)
	{
		arm_base_attr_mul_set(pstArm, pstRecastItemsDetail->BaseInfo[i].ResultID, pstRecastItemsDetail->BaseInfo[i].Mul);
	}
	//更新客户端数据
	arm_one_grid_update_ex(pstEnv,pstPlayer, pstArm,pstArmSaveRecastData->ArmInfo.ListType);

	//日志		
	z_role_machining_oplog(pstEnv, pstPlayer, &stOldArm, pstArm,NULL, 
							OP_MACHINING_ARM_RECAST_SAVE, 1, NULL,0);

	memset(&pstPlayer->stOnline.stRecaseInfo,0,sizeof(pstPlayer->stOnline.stRecaseInfo));
	return 0;
}


int spar_inlay(ZONESVRENV* pstEnv, Player *pstPlayer, SPARINLAYDATA *pstSparInlay)
{
	ROLEGRID *pstSrcGrid=NULL;
	ROLEITEM *pstSrcItem = NULL;
	ROLEITEM *pstDscItem = NULL;
	int i;
	ITEMDEF *pstSrcDef;
	SPARPKG *pstSparPkg = &pstPlayer->stRoleData.Package.SparPkg;
	SUNDRIESPACKAGE *pstSund =  &pstPlayer->stRoleData.Package.SundriesPackage;
	ROLEITEM stOldDscItem;
	int iPos;

	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstRolePakChg = NULL;
	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstRolePakChg = &pstPackageInfo->PackageInfoData.PackageChg;
	pstRolePakChg->ArmChg.ChgNum = 0;
	pstRolePakChg->ItemChg.ChgNum = 0;
	int iRet = 0;
	int iVaild;

	//  src 一定是杂物包   dsc 一定是晶石包
	if (	pstSparInlay->Src.ArmListType != LIST_TYPE_SUNDRIES ||
		pstSparInlay->Dsc.ArmListType != LIST_TYPE_SPAR ||
		pstSparInlay->Dsc.ArmGridIdx >= pstSparPkg->MaxNum)
	{
		goto final;
	}

	// 检查客户端传的格子是否有效
	iVaild = get_vaild_gird(pstEnv, &pstPlayer->stRoleData.Package, LIST_TYPE_SUNDRIES);
	if (pstSparInlay->Src.ArmGridIdx >= iVaild)
	{
		goto final;
	}
	
	// 源Item
	iPos = package_grid_pos(pstSund->RoleGrids, pstSund->Num, pstSparInlay->Src.ArmGridIdx);
	if (iPos >= 0)
	{
		pstSrcGrid = &pstSund->RoleGrids[iPos];
		pstSrcItem = &pstSrcGrid->GridData.RoleItem;
		pstSrcDef = z_find_itemdef(pstEnv, pstSrcItem->DefID);
		if (!pstSrcDef)
		{
			goto final;
		}

		if (	!z_item_get_result(pstSrcDef, RESULT_SPAR) &&
			!z_item_get_result(pstSrcDef, RESULT_SPAR_SHOP))
		{
			goto final;
		}
	}
	
	// 目标Item
	pstDscItem = NULL;
	for (i=0; i<pstSparPkg->Num; i++)
	{
		if (pstSparPkg->RoleItems[i].GridIdx == pstSparInlay->Dsc.ArmGridIdx)
		{
			pstDscItem = &pstSparPkg->RoleItems[i];
			break;
		}
	}

	if (pstDscItem)
	{
		if (pstSrcItem)	// 交换
		{
			stOldDscItem = *pstDscItem;
			*pstDscItem = *pstSrcItem;
			pstDscItem->GridIdx = stOldDscItem.GridIdx;
			
			// 通知客户端变化
			pstRolePakChg->ItemChg.Items[pstRolePakChg->ItemChg.ChgNum].Item = *pstDscItem;
			pstRolePakChg->ItemChg.Items[pstRolePakChg->ItemChg.ChgNum].ListType = LIST_TYPE_SPAR;
			pstRolePakChg->ItemChg.ChgNum ++;
			
			*pstSrcItem = stOldDscItem;
			pstSrcItem->GridIdx = pstSparInlay->Src.ArmGridIdx;
			
			// 通知客户端变化
			pstRolePakChg->ItemChg.Items[pstRolePakChg->ItemChg.ChgNum].Item = *pstSrcItem;
			pstRolePakChg->ItemChg.Items[pstRolePakChg->ItemChg.ChgNum].ListType = LIST_TYPE_SUNDRIES;
			pstRolePakChg->ItemChg.ChgNum ++;

			z_spar_oplog(pstEnv, pstPlayer, 1, pstSrcItem, pstDscItem, NULL);

			//成长之路
			if (pstSrcDef)
			{
				ACHIEVEMENTCOND stCond;

				memset(&stCond,0,sizeof(stCond));
				
				stCond.Type = COND_SPAR_INLAY;
				stCond.Val1 = pstSrcDef->Important;

				z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
			}
		}	
		else				// 取下
		{
			pstRolePakChg->ItemChg.Items[pstRolePakChg->ItemChg.ChgNum].Item = *pstDscItem;
			pstRolePakChg->ItemChg.Items[pstRolePakChg->ItemChg.ChgNum].Item.Num = 0;
			pstRolePakChg->ItemChg.Items[pstRolePakChg->ItemChg.ChgNum].ListType = LIST_TYPE_SPAR;
			pstRolePakChg->ItemChg.ChgNum ++;	

			pstRolePakChg->ItemChg.Items[pstRolePakChg->ItemChg.ChgNum].Item = *pstDscItem;
			pstRolePakChg->ItemChg.Items[pstRolePakChg->ItemChg.ChgNum].Item.GridIdx = 
																pstSparInlay->Src.ArmGridIdx;
			pstRolePakChg->ItemChg.Items[pstRolePakChg->ItemChg.ChgNum].ListType = LIST_TYPE_SUNDRIES;
			pstRolePakChg->ItemChg.ChgNum ++;	

			pstSund->RoleGrids[pstSund->Num].Type = GRID_ITEM_ITEM;
			pstSund->RoleGrids[pstSund->Num].GridData.RoleItem = *pstDscItem;
			pstSund->RoleGrids[pstSund->Num].GridData.RoleItem.GridIdx = pstSparInlay->Src.ArmGridIdx;
			pstSund->Num ++;

			z_spar_oplog(pstEnv, pstPlayer, 2, pstDscItem, NULL, NULL);
			
			for (i=pstSparPkg->Num-1; i>=0; i--)
			{
				if (pstSparPkg->RoleItems[i].GridIdx == pstSparInlay->Dsc.ArmGridIdx)
				{
					pstSparPkg->Num --;
					if (i != pstSparPkg->Num)
					{
						pstSparPkg->RoleItems[i] = pstSparPkg->RoleItems[pstSparPkg->Num];
					}
					break;
				}
			}

		}
	}
	else
	{
		if (pstSrcItem)	// 镶嵌
		{
			if (pstSparPkg->Num >= pstSparPkg->MaxNum)
			{	
				goto final;
			}

			iPos = pkg_get_pos(pstPlayer, pstSparInlay->Src.ArmListType,pstSparInlay->Src.ArmGridIdx);
			if (iPos < 0)
			{
				goto final;
			}

			if (package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
								pstSrcItem->DefID, 1, pstSparInlay->Src.ArmListType, 
								&iPos, 0) < 0)
			{
				goto final;
			}
			
			pstSparPkg->RoleItems[pstSparPkg->Num] = *pstSrcItem;
			pstSparPkg->RoleItems[pstSparPkg->Num].GridIdx =  pstSparInlay->Dsc.ArmGridIdx;

			z_spar_oplog(pstEnv, pstPlayer, 3, NULL, pstSrcItem, NULL);
			
			// 通知客户端变化 todo;
			pstRolePakChg->ItemChg.Items[pstRolePakChg->ItemChg.ChgNum].Item = 
														pstSparPkg->RoleItems[pstSparPkg->Num];
			pstRolePakChg->ItemChg.Items[pstRolePakChg->ItemChg.ChgNum].ListType = LIST_TYPE_SPAR;
			pstRolePakChg->ItemChg.ChgNum ++;

			pstSparPkg->Num ++;

			package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
							pstSrcItem->DefID, 1, pstSparInlay->Src.ArmListType, &iPos, PACKAGE_DEC_NONE, IMPITEM_OPTYPE_SPARIN);

			//成长之路
			if (pstSrcDef)
			{
				ACHIEVEMENTCOND stCond;

				memset(&stCond,0,sizeof(stCond));
				
				stCond.Type = COND_SPAR_INLAY;
				stCond.Val1 = pstSrcDef->Important;

				z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
			}
		}
		else
		{
			return -1;
		}
	}

	Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	// 重新计算数值todo:
	z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	iRet = 1;

final:
	{
		CSPKG stPkg1;
		CSARMSVR  *pstArmSvr = &stPkg1.Body.ArmSvr;
		memset(pstArmSvr, 0, sizeof(*pstArmSvr));
		pstArmSvr->Flag = iRet;
		pstArmSvr->ArmSvrType = SPAR_INLAY;
		Z_CSHEAD_INIT(&stPkg1.Head, ARM_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg1);

		task_player_trace(pstEnv, pstPlayer, TASK_OBJ_INLAY_SPAR);
	}
	
	return 0;
}

int spar_cacl_can_sorb(ROLEITEM *pstRoleItem, SPARDEF *pstSparDef)
{
	int i;
	int iCanSorb = 0;
	int iLvl = pstRoleItem->InstVal1;

	iCanSorb = pstSparDef->UpExp[iLvl-1] -pstRoleItem->InstVal3;
	if (iCanSorb < 0)
		iCanSorb = 0;
	
	for (i=iLvl; i<pstSparDef->MaxLvl-1; i++)
	{
		iCanSorb += pstSparDef->UpExp[i];
	}

	
	return iCanSorb;
}

int spar_one_exp_get(SPARDEF *pstSparDef, ROLEITEM *pstRoleItem)
{
	int iGet = 0;
	int i;
	
	/*
		Instval1-晶石等级
		Instval3-晶石当前经验
	*/

	iGet += pstSparDef->SparExp; 		// 本身经验
	iGet += pstRoleItem->InstVal3;	// 当前经验
	for (i=0; i<pstRoleItem->InstVal1-1; i++)	// 曾经升级经验
	{
		iGet += pstSparDef->UpExp[i];
	}

	return iGet;
}

int spar_sorb_check_stuff(ZONESVRENV * pstEnv, Player *pstPlayer,SPARSORBDATA *pstSparSorb, int iCanSorb, 
							int *pDelNum, int *pastDel, SPARDEF *pstSparDef, int *piUse)
{
	int i,j;
	int iGet = 0;
	ROLEGRID *pstGrid = NULL;
	ROLEITEM *pstItem = NULL;
	ITEMDEF *pstItemDef;
	ARMFENJIEDEF *pstArmFjDef;
	SPARDEF *pstSparDef1;
	int iPos;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	USEDEF *pstUseDef;
	int iUse = 0;
	TotalStuffGridTmp stGridTmp;
	
	int iFreeflag = 0; // 是否是不消耗钱的新能源充能道具
	int iFreeGet = 0; // 免费的数额
	int iTempGet = 0;

	memset(&stGridTmp, 0, sizeof(stGridTmp));

	pstUseDef = z_find_usedef(pstEnv, USE_ID_SPAR_SORB);
	if (!pstUseDef)
	{
		return -1;
	}

	for (i=0; i<pstSparSorb->StuffNum; i++)
	{
		if (pstSparSorb->StuffData[i].ListType != LIST_TYPE_SUNDRIES)
		{
			return -1;
		}

		iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, 
								pstPak->SundriesPackage.Num, 
								pstSparSorb->StuffData[i].GridIdx);
		if (0 > iPos)
		{
			return -1;
		}

		pstGrid = &pstPak->SundriesPackage.RoleGrids[iPos];
		if (!pstGrid)
		{
			return -1;
		}

		pstItem = &pstGrid->GridData.RoleItem;
		pstItemDef = z_find_itemdef(pstEnv, pstItem->DefID);
		if (!pstItemDef)
		{
			return -1;
		}

		iFreeflag = 0;
		if (z_item_get_result(pstItemDef, RESULT_SPAR_SORB_FREE))
		{
			iFreeflag = 1;
		}
	
		if (pstGrid->Type == GRID_ITEM_ARM)
		{
			if(arm_is_fashion(pstItemDef))
			{
				return -1;
			}
			pstArmFjDef = z_find_fenjie_def(pstEnv, get_arm_itemlevel(pstItemDef->WearPre.RoleLevelMin), 
											pstItemDef->Important);
			if (!pstArmFjDef)
			{
				return -1;
			}
			
			iGet += pstArmFjDef->SparExp;
			
			if(iFreeflag == 1)
			{
				iFreeGet += pstArmFjDef->SparExp;
			}
				
			//iUse += pstUseDef->UseMoney;
			pastDel[(*pDelNum)] = 1;
			(*pDelNum) ++;
		}
		else
		{
			if (	pstItemDef->Result[0].ResultID == RESULT_SPAR ||
				pstItemDef->Result[0].ResultID == RESULT_SPAR_SHOP)
			{
				pstSparDef1 = z_find_spardef(pstEnv,pstItemDef->ItemID);
				if (!pstSparDef1)
				{
					return -1;
				}

				pastDel[(*pDelNum)] = pstSparSorb->StuffData[i].ItemNum;
				for (j=0; j<pstSparSorb->StuffData[i].ItemNum;j++)
				{
					//iGet += pstSparDef1->SparExp;

					iTempGet = spar_one_exp_get(pstSparDef1, &pstGrid->GridData.RoleItem);
					iGet += iTempGet;

					if(iFreeflag == 1)
					{
						iFreeGet += iTempGet;
					}
					
					if (iGet >= iCanSorb)
					{
						pastDel[(*pDelNum)] = j+1;
						break;
					}
				}
				(*pDelNum) ++;
			}
			else
			{
				return -1;
			}
		}

		/*
		if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
								pstSparSorb->StuffData[i].ItemID,  pstSparSorb->StuffData[i].ItemNum, 
								LIST_TYPE_SUNDRIES, &iPos,PACKAGE_DEC_NONE))
		{
			return -1;
		}
		*/

		if (0 > tmp_grid_total(&stGridTmp, pstSparSorb->StuffData[i].ItemID, pstSparSorb->StuffData[i].ItemNum, iPos, pstSparSorb->StuffData[i].ListType))
		{
			return -1;
		}

		// 大于能获取能量上限了  
		if (iGet >= iCanSorb)
		{
			break;
		}
	}

	for(i=0; i<stGridTmp.iNum; i++)
	{
		if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
								stGridTmp.stGrid[i].iItemID,  stGridTmp.stGrid[i].iNum, 
								stGridTmp.stGrid[i].iListType, 
								&stGridTmp.stGrid[i].iPos,PACKAGE_DEC_NONE))
		{
			return -1;
		}
	}

	iUse = iGet > iCanSorb ?iCanSorb:iGet;

	//免费的数额要减掉
	if(iFreeGet > 0)
	{
		iUse = iUse - iFreeGet;
	}
	
	if (iUse > 0)
	{
		if (package_dec_money_test(pstEnv,pstPlayer,ATTR_ID_BULL, iUse) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOMONEY3);
			return -1;
		}	
	}

	*piUse = iUse;
	return iGet;
}

int spar_sorb_in(ROLEITEM *pstSrcItem, int iGet, SPARDEF *pstSparDef)
{
	int iTmp;
	while(iGet > 0)
	{
		iTmp = pstSparDef->UpExp[pstSrcItem->InstVal1 -1]-pstSrcItem->InstVal3;
		if (iGet > iTmp)
		{
			pstSrcItem->InstVal1 ++;
			pstSrcItem->InstVal3 = 0;
			iGet -= iTmp;
		}
		else if (iGet == iTmp)
		{
			pstSrcItem->InstVal1 ++;
			pstSrcItem->InstVal3 = 0;
			iGet = 0;	
			
		}
		else
		{
			pstSrcItem->InstVal3 += iGet;
			iGet = 0;	
		}

		if (pstSrcItem->InstVal1 >= pstSparDef->MaxLvl)
		{
			return 0;
		}
	}
	return 0;
}

// 晶石吸收
int spar_sorb(ZONESVRENV* pstEnv, Player *pstPlayer, SPARSORBDATA *pstSparSorb)
{
	ROLEGRID *pstSrcGrid = NULL;
	ROLEITEM *pstSrcItem = NULL;
	ROLEITEM  stSparItemOld;
	SPARPKG *pstSparPkg = &pstPlayer->stRoleData.Package.SparPkg;
	ITEMDEF *pstSrcDef;
	SPARDEF *pstSparDef;
	int iCanSorb; // 此晶石最多能吸收的能量值
	int iDelNum = 0; // 删除材料数
	int astDel[MAX_SPAR_SORB_STUFF];
	int iGet;
	int i, iPos, iDec;
	int iRet = 0;
	int iUse = 0;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	MACHINNGEXPENDDATA stExpendData;
	
	memset(pstArmSvr, 0, sizeof(*pstArmSvr));

	// 检查源
	if (pstSparSorb->Src.ArmListType == LIST_TYPE_SUNDRIES)
	{
		pstSrcGrid = get_pak_item_grid(pstPlayer, pstSparSorb->Src.ArmListType, pstSparSorb->Src.ArmGridIdx);
		if (!pstSrcGrid)
		{
			goto final;
		}

		pstSrcItem = &pstSrcGrid->GridData.RoleItem;
	}
	else if (pstSparSorb->Src.ArmListType == LIST_TYPE_SPAR)
	{
		for (i=0; i<pstSparPkg->Num; i++)
		{
			if (pstSparPkg->RoleItems[i].GridIdx == pstSparSorb->Src.ArmGridIdx)
			{
				pstSrcItem = &pstSparPkg->RoleItems[i];
				break;
			}
		}
	}
	else
	{
		goto final;
	}

	if (!pstSrcItem)
	{
		goto final;
	}
	pstSrcDef = z_find_itemdef(pstEnv, pstSrcItem->DefID);
	if (!pstSrcDef)
	{
		goto final;
	}

	if (	!z_item_get_result(pstSrcDef, RESULT_SPAR) )
	{
		goto final;
	}
	
	pstSparDef = z_find_spardef(pstEnv, pstSrcDef->ItemID);
	if (!pstSparDef || pstSparDef->MaxLvl > SPAR_MAX_LEVEL)
	{
		goto final;
	}

	if (pstSrcItem->InstVal1 >= pstSparDef->MaxLvl)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ARM136);
		pstArmSvr->Flag = iRet;
		pstArmSvr->ArmSvrType = SPAR_SORB;
		Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		return -1;
	}

	// 计算产生多少exp
	iCanSorb = spar_cacl_can_sorb(pstSrcItem, pstSparDef);
	if (iCanSorb <= 0)
	{
		goto final;
	}

	//z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "%d", iCanSorb);

	// 检查材料 计算能吸收多少exp
	memset (astDel, 0, sizeof(astDel));
	iGet =  spar_sorb_check_stuff(pstEnv, pstPlayer,pstSparSorb,iCanSorb, &iDelNum, astDel,pstSparDef, &iUse);
	if (iGet <= 0)
	{
		goto final;
	}

	stSparItemOld = *pstSrcItem;

	spar_sorb_in(pstSrcItem, iGet, pstSparDef);

	bzero(&stExpendData, sizeof(stExpendData));
	
	if (iUse > 0)
	{
		iRet = package_dec_money(pstEnv,pstPlayer,ATTR_ID_BULL, iUse);
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_SPAR, pstSrcDef->ItemID, iUse-iRet, iRet, "晶石吸收");
		stExpendData.Money = iRet;
		stExpendData.Bull = iUse - iRet;
	}

	for (i = 0; i<iDelNum; i++ )
	{
		iPos = pkg_get_pos(	pstPlayer, LIST_TYPE_SUNDRIES,
							pstSparSorb->StuffData[i].GridIdx);
		iDec = package_dec(	pstEnv->pstAppCtx, pstEnv, pstPlayer, 
							pstSparSorb->StuffData[i].ItemID, astDel[i], 
							LIST_TYPE_SUNDRIES, 
							&iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_SPARIN);

		itemdec_fill(&stExpendData.StuffInfo[stExpendData.StuffNum++],pstSparSorb->StuffData[i].ItemID, astDel[i], iDec);
	}

	item_one_grid_update_ex(pstEnv,  pstPlayer, pstSrcItem, pstSparSorb->Src.ArmListType);
	if (pstSparSorb->Src.ArmListType == LIST_TYPE_SPAR)
	{
		z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	iRet = 1;

	if (pstSparSorb->Src.ArmListType == LIST_TYPE_SUNDRIES)
	{
		z_spar_oplog(pstEnv, pstPlayer, 4, &stSparItemOld, pstSrcItem, &stExpendData);
	}
	else
	{
		z_spar_oplog(pstEnv, pstPlayer, 5, &stSparItemOld, pstSrcItem, &stExpendData);
	}

final:

	{
		pstArmSvr->Flag = iRet;
		pstArmSvr->ArmSvrType = SPAR_SORB;
		Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

		if (iRet)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_SPAR_SORB);
			task_player_trace(pstEnv, pstPlayer, TASK_OBJ_SPAR_ADD_EXP);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_SPAR_SORB1);
		}
	}
	return 0;
}


int player_cacl_spar_attr(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	SPARPKG *pstSparPkg = &pstPlayer->stRoleData.Package.SparPkg;
	int iVal;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	NEWADDATTRDEF *pstNewAttrDef;
	ITEMDEF *pstItemDef;

	// 晶石关键字
	for (i=0; i<pstSparPkg->Num; i++)
	{
		if (	pstSparPkg->RoleItems[i].Flag <= 0 ||
			pstSparPkg->RoleItems[i].InstVal2 <= 0)
		{
			continue;
		}

		pstNewAttrDef = z_find_new_addattr_def(pstEnv, pstSparPkg->RoleItems[i].Flag);
		if (!pstNewAttrDef)
		{
			continue;
		}

		pstItemDef = z_find_itemdef(pstEnv,pstSparPkg->RoleItems[i].DefID);
		if (!pstItemDef)
		{
			continue;
		}

		// 晶石属性值=基础值*（2+等级）/3
		iVal = 1.0*pstSparPkg->RoleItems[i].InstVal2*(2+pstSparPkg->RoleItems[i].InstVal1)/3;	

		cacl_arm_add_attr_result(pstEnv, pstPlayer, pstDyn, pstItemDef, pstSparPkg->RoleItems[i].Flag, iVal);

	}

	return 0;
}

int player_arm_levelup_check(ZONESVRENV* pstEnv, Player *pstPlayer, ARMLEVELUPDEF *pstArmLevelDef, ROLEGRID *pstRoleGrid)
{
	int i;
	
	if(0 > package_dec_money_test(pstEnv, pstPlayer, pstArmLevelDef->MoneyType, pstArmLevelDef->MoneyVal))
	{
		return -1;
	}

	for(i=0; i<MAX_ARM_LEVELUP_STUFF; i++)
	{
		if(pstArmLevelDef->Stuff[i].ItemID == 0)

		if(0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
			pstArmLevelDef->Stuff[i].ItemID, pstArmLevelDef->Stuff[i].Num, -1, NULL, PACKAGE_DEC_NONE))
		{
			return -1;
		}
	}

	if(0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRoleGrid, 1, 0))
	{
		return -1;
	}
	
	return 0;
}

int player_arm_levelup_cost(ZONESVRENV* pstEnv, Player *pstPlayer, ARMLEVELUPDEF *pstArmLevelDef, MACHINNGEXPENDDATA *pstExpendData)
{
	int i;
	int iRet;
	int iDec;

	iRet = package_dec_money(pstEnv, pstPlayer, pstArmLevelDef->MoneyType, pstArmLevelDef->MoneyVal);
	if(0 > iRet)
	{
		return -1;
	}

	switch(pstArmLevelDef->MoneyType)
	{
		case ATTR_ID_BULL:
			pstExpendData->Money = iRet;
			pstExpendData->Bull = pstArmLevelDef->MoneyVal - iRet;
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ARM_LEVELUP, 
				pstArmLevelDef->ItemID, pstArmLevelDef->MoneyVal-iRet, iRet, "装备升级");
			break;
		case ATTR_ID_MONEY:
			pstExpendData->Money = pstArmLevelDef->MoneyVal;
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ARM_LEVELUP, 
				pstArmLevelDef->ItemID, 0, pstArmLevelDef->MoneyVal, "装备升级");
			break;
		case ATTR_ID_GODCOIN:
			pstExpendData->JinBi = pstArmLevelDef->MoneyVal;
			godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
						pstPlayer->stRoleData.Level, GODCOIN_OPER_FLAG_ARM_LEVELUP,pstArmLevelDef->ItemID,0,0,
						pstArmLevelDef->MoneyVal,"装备升级");
			break;
		case ATTR_ID_BINDGODCOIN:
			pstExpendData->JinQuan	= pstArmLevelDef->MoneyVal;
			z_bindgold_consume_oplog(pstEnv,pstPlayer, BINDGOLD_OPTYPE_ARM_LEVELUP,pstArmLevelDef->ItemID,
						pstArmLevelDef->MoneyVal,"装备升级");
			break;
		default:
			break;
	}

	for(i=0; i<MAX_ARM_LEVELUP_STUFF; i++)
	{
		if(pstArmLevelDef->Stuff[i].ItemID == 0)
		{
			continue;
		}

		iDec = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
			pstArmLevelDef->Stuff[i].ItemID, pstArmLevelDef->Stuff[i].Num, -1, NULL, PACKAGE_DEC_NONE, IMPITEM_OPTYPE_ARM_LEVELUP);
		if(0 > iDec)
		{
			return -1;
		}

		itemdec_fill(&pstExpendData->StuffInfo[pstExpendData->StuffNum], 
			pstArmLevelDef->Stuff[i].ItemID, pstArmLevelDef->Stuff[i].Num, iDec);
		pstExpendData->StuffNum++;
	}
	
	return 0;
}

int player_arm_level_up(ZONESVRENV* pstEnv, Player *pstPlayer, ARMLEVELUP *pstCsLevelUP)
{
	int iPos;
	CSPKG stPkg;
	CSACTIONRES  *pstActionRes = &stPkg.Body.ActionRes;
	ROLEGRID *pstArmGrid =NULL;
	ROLEGRID stRoleGrid;
	ROLEARM *pstNextRoleArm, stOldRoleArm;
	ITEMDEF *pstItemDef= NULL;
	ARMLEVELUPDEF *pstArmLevelUPDef;
	MACHINNGEXPENDDATA stExpendData;
	ITEMDEF *pstTempItemDef= NULL;
	
	bzero(&stExpendData, sizeof(stExpendData));
	
	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		goto _error;
	}

	if(pstCsLevelUP->ArmData.ArmListType == LIST_TYPE_WEAR)
	{
		goto _error;
	}
	
	pstArmGrid = get_pak_item_grid(pstPlayer, pstCsLevelUP->ArmData.ArmListType, pstCsLevelUP->ArmData.ArmGridIdx);
	if (!pstArmGrid || pstArmGrid->Type != GRID_ITEM_ARM)
	{
		goto _error;
	}

	stOldRoleArm = pstArmGrid->GridData.RoleArm;
	if (is_fengyin_arm(&pstArmGrid->GridData.RoleArm))
	{
		goto _error;
	}

	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstArmGrid->GridData.RoleItem);
	if (!pstItemDef)
	{
		goto _error;
	}

	//lzk
	if(arm_is_badge(pstItemDef) < 0 )
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ARM912);
		return -1;
	}

	pstArmLevelUPDef = z_find_arm_levelup_def(pstEnv, pstItemDef->ItemID);
	if(!pstArmLevelUPDef)
	{
		goto _error;
	}

	if(0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstArmLevelUPDef->NextItemID))
	{
		goto _error;
	}

	if(0 > player_arm_levelup_check(pstEnv, pstPlayer, pstArmLevelUPDef, &stRoleGrid))
	{
		goto _error;
	}

	pstTempItemDef = z_get_itemdef_roleitem(pstEnv, &stRoleGrid.GridData.RoleItem);
	if (!pstTempItemDef)
	{
		goto _error;
	}

	//80级+的升阶装备,角色等级必须大于等于升阶之后的装备等级
	if(pstItemDef->WearPre.RoleLevelMin >= 80 &&
		pstTempItemDef->WearPre.RoleLevelMin > pstPlayer->stRoleData.Level)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ARM_LEVELUP1,pstTempItemDef->WearPre.RoleLevelMin);
		goto _error;
	}

	pstNextRoleArm = &stRoleGrid.GridData.RoleArm;
	
	//强化
	pstNextRoleArm->StrengthenCount = pstArmGrid->GridData.RoleArm.StrengthenCount;
	pstNextRoleArm->StarLv = pstArmGrid->GridData.RoleArm.StarLv;
	memcpy(&pstNextRoleArm->StarInfo[0], 
		&pstArmGrid->GridData.RoleArm.StarInfo[0], MAX_ARM_STAR_LEVEL*sizeof(pstNextRoleArm->StarInfo[0]));

	//宝石
	pstNextRoleArm->HoleNum = pstArmGrid->GridData.RoleArm.HoleNum;
	memcpy(&pstNextRoleArm->HoleGem[0], 
		&pstArmGrid->GridData.RoleArm.HoleGem[0], MAX_ARM_HOLE*sizeof(pstNextRoleArm->HoleGem[0]));

	//橙色装备不保留
	if(pstItemDef->Important != 7)
	{
		//关键字
		pstNextRoleArm->AddAttrNum = pstArmGrid->GridData.RoleArm.AddAttrNum;
		memcpy(&pstNextRoleArm->AddAttr[0], 
			&pstArmGrid->GridData.RoleArm.AddAttr[0], MAX_ARM_ADD_ATTR*sizeof(pstNextRoleArm->AddAttr[0]));
		//洗练关键字
		pstNextRoleArm->WashAttrNum = pstArmGrid->GridData.RoleArm.WashAttrNum;
		memcpy(&pstNextRoleArm->WashAddAttr[0], 
			&pstArmGrid->GridData.RoleArm.WashAddAttr[0], MAX_ARM_WASH_ADD_ATTR*sizeof(pstNextRoleArm->WashAddAttr[0]));

		//附魔
		pstNextRoleArm->ExtendedResult = pstArmGrid->GridData.RoleArm.ExtendedResult;
		pstNextRoleArm->ExtendedFromDefID = pstArmGrid->GridData.RoleArm.ExtendedFromDefID;
		
		//刻印
		pstNextRoleArm->KeYinAddAttr = pstArmGrid->GridData.RoleArm.KeYinAddAttr;
		pstNextRoleArm->KeYinEnd = pstArmGrid->GridData.RoleArm.KeYinEnd;

		// 阶层
		pstNextRoleArm->Step = pstArmGrid->GridData.RoleArm.Step;

		// 装备技能
		pstNextRoleArm->ArmSkill = pstArmGrid->GridData.RoleArm.ArmSkill;
	}
	
	if(0 > player_arm_levelup_cost(pstEnv, pstPlayer, pstArmLevelUPDef, &stExpendData))
	{
		goto _error;
	}

	iPos = pkg_get_pos(pstPlayer, pstCsLevelUP->ArmData.ArmListType, pstCsLevelUP->ArmData.ArmGridIdx);
	if(0 > iPos)
	{
		goto _error;
	}

	 if (0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
			pstItemDef->ItemID, 1, pstCsLevelUP->ArmData.ArmListType, &iPos, 
			PACKAGE_DEC_NONE, IMPITEM_OPTYPE_ARM_LEVELUP))
	 {
		goto _error;
	 }

	 stRoleGrid.GridData.RoleArm.InstFlag |= INST_ITEM_BIND;
	 package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1, 0, IMPITEM_OPTYPE_ARM_LEVELUP);

	pstActionRes->Data.ArmLevelUP.Succ = 1;
	pstActionRes->Data.ArmLevelUP.NextWID = stRoleGrid.GridData.RoleArm.WID;

	z_role_machining_oplog(pstEnv, pstPlayer, &stOldRoleArm, &stRoleGrid.GridData.RoleArm,
		&stExpendData, OP_MACHINING_ARM_LEVELUP, 1, NULL,0);
	
_final:
	pstActionRes->ActionID = ACTION_ARM_LEVELUP;
	pstActionRes->ID = pstPlayer->iMemID;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
	
_error:
	pstActionRes->Data.ArmLevelUP.Succ = 0;
	pstActionRes->Data.ArmLevelUP.NextWID = 0;
	goto _final;
	return 0;
}


int player_arm_attr_move(ZONESVRENV* pstEnv, Player *pstPlayer, ARMATTRMOVE*pstMove)
{
	ROLEGRID *pstSrc;
	ROLEGRID *pstDst;
	ROLEGRID *pstUseGrid;
	ITEMDEF *pstUseItemDef;
	ITEMDEF *pstArmItemDef;
	ITEMDEF *pstDstItemDef;
	USEDEF *pstUseDef;
	int iPos;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	int iSucc = 0;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	MACHINNGEXPENDDATA stExpendData;
	int iDec = 0;
	int iGaoji = 0;
	int iUseID;
	
	bzero(&stExpendData, sizeof(stExpendData));

	memset(pstArmSvr, 0, sizeof(*pstArmSvr));
	
	pstSrc =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstMove->SrcData.ArmGridIdx);
	if (!pstSrc)
	{
		goto final;
	}

	if (arm_is_uncheck(&pstSrc->GridData.RoleArm))
	{
		goto final;
	}


	pstDst =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstMove->DstData.ArmGridIdx);
	if (!pstDst)
	{
		goto final;
	}

	if (arm_is_uncheck(&pstDst->GridData.RoleArm))
	{
		goto final;
	}

	pstUseGrid =  get_pak_item_grid(pstPlayer, pstMove->UseData.ArmListType, pstMove->UseData.ArmGridIdx);
	if (!pstUseGrid)
	{
		goto final;
	}

	pstUseItemDef = z_get_itemdef_roleitem(pstEnv, &pstUseGrid->GridData.RoleItem);
	if (!pstUseItemDef)
	{
		goto final;
	}

	if (pstUseItemDef->Result[0].ResultID!= RESULT_ARM_ATTR_MOVE)
	{
		goto final;
	}

	pstArmItemDef = z_get_itemdef_roleitem(pstEnv, &pstSrc->GridData.RoleItem);
	if (!pstArmItemDef)
	{
		goto final;
	}

	//lzk
	if(arm_is_badge(pstArmItemDef) < 0 || arm_is_badge(pstUseItemDef) < 0)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ARM913);
		return -1;
	}

	
	if (pstArmItemDef->WearPre.RoleLevelMin > pstUseItemDef->Result[0].ResultVal1)
	{
		goto final;
	}
	
	if (!pstUseItemDef->Result[0].ResultVal2)
	{
		// 低阶转移石检查
		if (	pstSrc->GridData.RoleItem.DefID != pstDst->GridData.RoleItem.DefID ||
			pstSrc->Type != GRID_ITEM_ARM)
		{
			goto final;
		}
	}
	else
	{
		iGaoji = 1;
		// 高阶转移石检查
		pstDstItemDef = z_get_itemdef_roleitem(pstEnv, &pstDst->GridData.RoleItem);
		if (!pstDstItemDef)
		{
			goto final;	
		}	

		if (pstDstItemDef->Important < 3 || pstArmItemDef->Important < 3 )
		{
			goto final;
		}

		if (pstDstItemDef->Important == 7)
		{
			goto final;
		}

		if (pstDstItemDef->WearPre.RoleLevelMin != pstArmItemDef->WearPre.RoleLevelMin)
		{
			goto final;
		}

		if (pstDstItemDef->WearPre.Career != pstArmItemDef->WearPre.Career)
		{
			goto final;
		}

		if (pstDstItemDef->WearPos1 != pstArmItemDef->WearPos1)
		{
			goto final;
		}
	}


	if (pstMove->Type == 0)
	{
		iUseID = USE_ID_ATTR_MOVE;
	}
	else
	{
		iUseID = USE_ID_ARM_SKILL_MOVE;
	}
	
	pstUseDef = z_find_usedef(pstEnv, iUseID);
	if (!pstUseDef)
	{
		goto final;
	}

	if ( 0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, pstUseDef->UseMoney) )
	{
		goto final;
	}


	// 删除转移石
	iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, 
							pstMove->UseData.ArmGridIdx);
	if (0 > iPos)
	{
		goto final;
	}

	if (pstMove->Type == 0)
	{
		pstDst->GridData.RoleArm.AddAttrNum = pstSrc->GridData.RoleArm.AddAttrNum;
       	memcpy(pstDst->GridData.RoleArm.AddAttr, pstSrc->GridData.RoleArm.AddAttr,
                       	sizeof(pstDst->GridData.RoleArm.AddAttr));
	}
	else
	{
		if (pstSrc->GridData.RoleArm.ArmSkill.Max > 0)
		{
			pstDst->GridData.RoleArm.ArmSkill = pstSrc->GridData.RoleArm.ArmSkill;
		}
	}
        pstDst->GridData.RoleArm.InstFlag |= INST_ITEM_BIND;
		
        // 通知变化
       arm_one_grid_update(pstEnv,  pstPlayer, pstDst);


	iDec = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstUseItemDef->ItemID, 
				1, LIST_TYPE_SUNDRIES, &iPos,PACKAGE_DEC_NONE, 0);
	itemdec_fill(&stExpendData.StuffInfo[stExpendData.StuffNum], pstUseItemDef->ItemID, 1, iDec);
	stExpendData.StuffNum++;

	package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, pstUseDef->UseMoney);

	if (pstMove->Type == 0)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ARM_ATTR_MOVE, 
					pstArmItemDef->ItemID, 0, pstUseDef->UseMoney, "装备基础属性转移");
	}
	else
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ARM_ATTR_MOVE, 
					pstArmItemDef->ItemID, 0, pstUseDef->UseMoney, "装备技能属性转移");
	}
	
	stExpendData.Money = pstUseDef->UseMoney;
	
	// 删除源装备
	iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, 
							pstMove->SrcData.ArmGridIdx);
	if (0 > iPos)
	{
		goto final;
	}

	iDec = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstArmItemDef->ItemID, 
				1, LIST_TYPE_SUNDRIES, &iPos,PACKAGE_DEC_NONE, 0);
	itemdec_fill(&stExpendData.StuffInfo[stExpendData.StuffNum], pstArmItemDef->ItemID, 1, iDec);
	stExpendData.StuffNum++;
	z_role_machining_oplog(pstEnv, pstPlayer,NULL, NULL,
			&stExpendData, OP_MACHINING_ARM_ATTR_MOVE, 1, NULL,0);

	iSucc = 1;
final:

	pstArmSvr->Flag = iSucc;
	pstArmSvr->ArmSvrType = ARM_ADDATTR_MOVE;
	pstArmSvr->IndexID = iGaoji;
	pstArmSvr->WID = pstMove->Type;

	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_arm_attr_reset(ZONESVRENV* pstEnv, Player *pstPlayer, ARMATTRRESET *pstReset)
{
	ROLEGRID *pstSrc;
	ITEMDEF *pstSrcDef;
	ITEMDEF *pstUseItemDef;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	int iSucc = 0;
//	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	ROLEARM *pstArm;
	ADDATTRINST *pstAddAttr = NULL;
	NEWADDATTRDEF *pstNewAddAttrDef;
	ADDATTRRANDDEF *pstAddAttrRandDef;
	USEDEF *pstUseDef;
	int iEnd = 0;
	int iMin,iMax;

	memset(pstArmSvr, 0, sizeof(*pstArmSvr));
	
	pstSrc =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstReset->ArmData.ArmGridIdx);
	if (!pstSrc || pstSrc->Type != GRID_ITEM_ARM)
	{
		goto final;
	}

	if (arm_is_uncheck(&pstSrc->GridData.RoleArm))
	{
		//z_sys_strf(pstEnv, pstPlayer,SYS_MIDDLE, "此装备还未鉴定,不能进行此操作!");
		goto final;
	}

	pstSrcDef = z_get_itemdef_roleitem(pstEnv, &pstSrc->GridData.RoleItem);
	if (!pstSrcDef)
	{
		goto final;
	}

	pstUseItemDef = z_find_itemdef(pstEnv,pstReset->UseItemID);
	if (!pstUseItemDef || pstUseItemDef->Result[0].ResultID != RESULT_ARM_RECAST)
	{
		goto final;
	}

	if (pstSrcDef->WearPre.RoleLevelMin > pstUseItemDef->Result[0].ResultVal1)
	{
		goto final;
	}

	pstArm = &pstSrc->GridData.RoleArm;
	if (pstReset->Idx >=pstArm->AddAttrNum + pstArm->WashAttrNum)
	{
		goto final;
	}	

	if (pstReset->Idx < pstArm->AddAttrNum)
	{
		pstAddAttr = &pstArm->AddAttr[pstReset->Idx];
	}
	else
	{
		pstAddAttr = &pstArm->WashAddAttr[pstReset->Idx -  pstArm->AddAttrNum];
	}

	pstNewAddAttrDef = z_find_new_addattr_def(pstEnv, pstAddAttr->AddAttrID);
	if(!pstNewAddAttrDef )
	{
		goto final;
	}

	pstAddAttrRandDef = z_find_addattr_rand_def(pstEnv, 
			get_arm_itemlevel(pstSrcDef->WearPre.RoleLevelMin), pstSrcDef->Important);
	if(!pstAddAttrRandDef)
	{
		goto final;
	}

	if(	pstAddAttrRandDef->AddMulLevel-1< 0 ||
		pstAddAttrRandDef->AddMulLevel-1 >= MAX_ADD_ATTR_RAND_LEVEL)
	{
		goto final;
	}

	// 最大值了
	if (pstAddAttr->Val >=  pstNewAddAttrDef->RangeList[pstAddAttrRandDef->AddMulLevel-1].MaxnMul)
	{
		goto final;
	}

	pstUseDef = z_find_usedef(pstEnv,USE_ID_ATTR_RESET);
	if (!pstUseDef)
	{
		goto final;	
	}

	if ( 0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, pstUseDef->UseMoney) )
	{
		goto final;
	}

	if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
								 pstReset->UseItemID, 1, LIST_TYPE_SUNDRIES, NULL,
								 pstReset->Bind?1:2))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM134);
		goto final;
	}

	// 先根据用掉的重铸石随机
	if (pstArm->WashNum > 0)
	{
		if (RAND1(400) < pstArm->WashNum)
		{
			pstArm->WashNum = 0;
			iMin = pstNewAddAttrDef->RangeList[pstAddAttrRandDef->AddMulLevel-1].MidMul;
			iMax = pstNewAddAttrDef->RangeList[pstAddAttrRandDef->AddMulLevel-1].MaxnMul;	
			if(iMax > iMin)
			{
				pstAddAttr->Val = iMin + RAND1(iMax-iMin+1);
			}
			//pstAddAttr->Val = pstNewAddAttrDef->RangeList[pstAddAttrRandDef->AddMulLevel-1].MaxnMul;
			iEnd = 1;

			// 测试
			//z_sys_strf(pstEnv,  pstPlayer, SYS_MIDDLE2, "啦啦啦啦德玛西亚!!");
		}
	}
	
	if (!iEnd)
	{
		pstArm->WashNum  ++;
		iMin = pstNewAddAttrDef->RangeList[pstAddAttrRandDef->AddMulLevel-1].MinMul;

		if (pstSrcDef->WearPre.RoleLevelMin >= 60)
		{
			iMax = pstNewAddAttrDef->RangeList[pstAddAttrRandDef->AddMulLevel-1].MidMul;
		}
		else
		{
			iMax = pstNewAddAttrDef->RangeList[pstAddAttrRandDef->AddMulLevel-1].MaxnMul;	
		}
		
		if(iMax > iMin)
		{
			pstAddAttr->Val = iMin + RAND1(iMax-iMin+1);
		}
	}
	// 通知变化
	if (pstReset->Bind)
		pstSrc->GridData.RoleArm.InstFlag |= INST_ITEM_BIND;
	arm_one_grid_update(pstEnv,  pstPlayer, pstSrc);

	package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
				  pstReset->UseItemID, 1, LIST_TYPE_SUNDRIES, NULL,
				pstReset->Bind?1:2, 0);

	package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, pstUseDef->UseMoney);
	z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ARM_ATTR_RESET, 
						pstSrcDef->ItemID, 0, pstUseDef->UseMoney, "装备单条重铸");


	MACHINNGEXPENDDATA stExpendData;
	bzero(&stExpendData, sizeof(stExpendData));
	stExpendData.StuffNum = 1;
	itemdec_fill(&stExpendData.StuffInfo[0],  pstReset->UseItemID, 1, pstReset->Bind);
	z_role_machining_oplog(pstEnv, pstPlayer,NULL, &pstSrc->GridData.RoleArm,
							&stExpendData, OP_MACHINING_ARM_ATTR_RESET, 1, NULL,0);
	iSucc = 1;
	
final:

	pstArmSvr->Flag = iSucc;
	pstArmSvr->ArmSvrType = ARM_ATTR_RESET;

	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int player_arm_attr_active(ZONESVRENV* pstEnv, Player *pstPlayer, ARMATTRACTIVE *pstActive)
{
	ROLEGRID *pstSrc;
	int iSucc = 0;
	ROLEARM *pstArm;
	USEDEF *pstUseDef;
	int iUseID;
	OneAddAttrList *pstAddAttrWashList;
	OneAddAttrList stWashList;
	ITEMDEF *pstItemDef;
	int i,j;
	ADDATTRRANDDEF* pstAddAttrRandDef; 
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;

	memset(pstArmSvr, 0, sizeof(*pstArmSvr));
	
	pstSrc =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstActive->ArmData.ArmGridIdx);
	if (!pstSrc || pstSrc->Type != GRID_ITEM_ARM)
	{
		goto final;
	}

	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstSrc->GridData.RoleItem);
	if (!pstItemDef)
		goto final;

	pstArm = &pstSrc->GridData.RoleArm;
	if (pstArm->WashAttrNum != pstActive->Idx)
	{
		goto final;
	}

	if (pstActive->Idx >= MAX_ARM_WASH_ADD_ATTR)
	{
		goto final;
	}

	iUseID = USE_ID_ACTIVE_ATTR_1+pstActive->Idx;
	pstUseDef = z_find_usedef( pstEnv, iUseID);
	if (!pstUseDef)
	{
		goto final;
	}

	if ( 0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_GODCOIN, pstUseDef->UseJinbi) )
	{
		goto final;
	}

	pstAddAttrWashList = add_attr_one_list_get(pstEnv, pstItemDef, 1);
	if(!pstAddAttrWashList || pstAddAttrWashList->iAddAttrNum <= 0)
	{
		goto final;
	}

	pstAddAttrRandDef = z_find_addattr_rand_def(pstEnv, get_arm_itemlevel(pstItemDef->WearPre.RoleLevelMin), 
					pstItemDef->Important);
	if(!pstAddAttrRandDef)
	{
		return -1;
	}

	memcpy(&stWashList, pstAddAttrWashList, sizeof(stWashList));

	// 提出已有的蓝色关键字
	for (i=stWashList.iAddAttrNum-1; i>=0; i--)
	{
		for (j=0; j<pstArm->WashAttrNum;j++)
		{
			if (stWashList.iDefID[i] == pstArm->WashAddAttr[j].AddAttrID)
			{
				stWashList.iAddAttrNum--;
				if(i != stWashList.iAddAttrNum)
				{
					memmove(&stWashList.iDefID[i], &stWashList.iDefID[i+1], sizeof(stWashList.iDefID[0])*(stWashList.iAddAttrNum - i));
				}
			}
		}
	}

	if (stWashList.iAddAttrNum <= 0)
	{
		goto final;
	}

	// 直接插入到装备上
	if (wash_addattr_online_insert(pstEnv, pstPlayer, pstAddAttrRandDef, 
							stWashList.iDefID[RAND1(stWashList.iAddAttrNum)],0,pstArm) < 0)
	{
		goto final;
	}
	arm_one_grid_update(pstEnv,  pstPlayer, pstSrc);
	
	package_dec_money(pstEnv, pstPlayer, ATTR_ID_GODCOIN, pstUseDef->UseJinbi) ;
	godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
						pstPlayer->stRoleData.Level, GODCOIN_OPER_FLAG_ARM_ACTIVE,pstItemDef->ItemID,0,0,
						pstUseDef->UseJinbi,"装备属性激活");
	iSucc = 1;
	
final:

	pstArmSvr->Flag = iSucc;
	pstArmSvr->ArmSvrType = ARM_ATTR_ACTIVE;

	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


ARMSTEPUPDEF*z_find_arm_stepup_def(ZONESVRENV *pstEnv, int iLvl, int iWpos, int iImp)
{
	int iIndex;
	int iEqu;
	ARMSTEPUPDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	stDef.Imp = iImp;
	stDef.Lvl = iLvl;
	stDef.WPos = iWpos;
	
	iIndex = bsearch_int(&stDef, pstObj->sArmStepUpDef, pstObj->iArmStepUpDef,ARM_STEP_UP_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ARMSTEPUPDEF*)pstObj->sArmStepUpDef[iIndex];
	}
	return NULL;
}


ARMRESETDEF*z_find_arm_reset_def(ZONESVRENV *pstEnv, int iLvl)
{
	int iIndex;
	int iEqu;
	ARMRESETDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	stDef.Lvl = iLvl;

	iIndex = bsearch_int(&stDef, pstObj->sArmReSetDef, pstObj->iArmReSetDef,ARM_RESET_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ARMRESETDEF*)pstObj->sArmReSetDef[iIndex];
	}
	return NULL;
}

ARMSKILLNUMDEF*z_find_arm_skillnum_def(ZONESVRENV *pstEnv, int iLvl,  int iWpos, int iImp )
{
	int iIndex;
	int iEqu;
	ARMSKILLNUMDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	stDef.Lvl = iLvl;
	stDef.Imp = iImp;
	stDef.WPos = iWpos;

	iIndex = bsearch_int(&stDef, pstObj->sArmSkillNumDef, pstObj->iArmSkillNumDef,ARM_SKILL_NUM_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ARMSKILLNUMDEF*)pstObj->sArmSkillNumDef[iIndex];
	}
	return NULL;
}

ARMSKILLATTRDEF*z_find_arm_skillattr_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	ARMSKILLATTRDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	stDef.ID = iID;

	iIndex = bsearch_int(&stDef, pstObj->sArmSkillAttrDef, pstObj->iArmSkillAttrDef,ARM_SKILL_ATTR_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ARMSKILLATTRDEF*)pstObj->sArmSkillAttrDef[iIndex];
	}
	return NULL;
}



ARMGRADEVALDEF*z_find_arm_val_def(ZONESVRENV *pstEnv, int iImp)
{
	int iIndex;
	int iEqu;
	ARMGRADEVALDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	stDef.Imp = iImp;

	iIndex = bsearch_int(&stDef, pstObj->sArmGradeValDef, pstObj->iArmGradeValDef,ARM_VAL_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ARMGRADEVALDEF*)pstObj->sArmGradeValDef[iIndex];
	}
	return NULL;
}


static int base_reset_calc_dist(ARMRESETDEF *pstResetDef,int iVal)
{
	int i;
	for (i=0; i<ARM_RESET_MAX_DIST; i++)
	{
		if (iVal <= pstResetDef->Reset[i].Val)
		{
			return i;
		}
	}

	return  ARM_RESET_MAX_DIST-1;		
}

static int base_reset_calc_good(ARMRESETDEF * pstResetDef, int iIdx, int iImp)
{
	RESETRATE *pstRate = &pstResetDef->Reset[iIdx];
	int iCountRate = 0;
	int i;
	int iRate;
	int iGood = 0;

	iRate = RAND1(10000);

	//   抽不到就是0条,策划这个概率不用填成总和为10000
	for (i=0; i<ARM_RESET_MAX_RATE; i++)
	{
		iCountRate += pstRate->Rate[i];
		if (iRate <= iCountRate)
		{
			iGood = i+1;
			break;
		}
	}

	//  	绿色装备最多取2条主属性，
	//	蓝色装备最多取3条主属性，紫色、粉色最多取4条主属性、
	//	橙色取5条主属性

	if (iImp == 1)
	{
		if (iGood > 2)
		{
			iGood = 2;
		}
	}
	else if (iImp == 2)
	{
		if (iGood > 3)
		{
			iGood = 3;
		}
	}
	else if (iImp == 3)
	{
		if (iGood > 4)
		{
			iGood = 4;
		}
	}

	return iGood;
}


#define ATTR_MAIN_JISI 1
#define ATTR_MAIN_ZHANSHI 2
#define ATTR_MAIN_FASHI 4
#define ATTR_MAIN_QIANGSHOU 8
#define ATTR_MAIN_DANAI 16
#define ATTR_MAIN_ERNAI 32
#define ATTR_MAIN_HUIMIE 64
#define ATTR_MAIN_LIESHA 128
#define ATTR_MAIN_JUNENG 256
#define ATTR_MAIN_LIANHUN 512
#define ATTR_MAIN_SHOUHU 1024
#define ATTR_MAIN_ZHENGFU 2048

#define ATTR_RIGHTHAND 1
#define ATTR_HEAD 2
#define ATTR_BODY 4
#define ATTR_LEFTHAND 8
#define ATTR_FACE 16
#define ATTR_SHOULDER 32
#define ATTR_SHOE 64
#define ATTR_AMULET 128
#define ATTR_RING 256
#define ATTR_NECKLACE 512
#define ATTR_BACK 1024
#define ATTR_BADGE 2048


static int arm_base_reset_in(ZONESVRENV* pstEnv, Player *pstPlayer,ROLEARM *pstArm, ITEMDEF *pstItemDef, int iGood)
{
	OneAddAttrList *pstAddAttrRandList;
	OneAddAttrList stGood;
	OneAddAttrList stBad;
	NEWADDATTRDEF* pstAttrDef;
	int i;
	int iIsGood;
	int iIdx;
	
	int iAddAttrNum = add_attr_num_rand_get(pstEnv, pstItemDef);
	if(iAddAttrNum <= 0 || iAddAttrNum > MAX_ARM_ADD_ATTR)
	{
		return -1;
	}

	if (iGood > iAddAttrNum)
	{
		return -1;
	}

	pstAddAttrRandList = add_attr_one_list_get(pstEnv, pstItemDef, 0);
	if(!pstAddAttrRandList || pstAddAttrRandList->iAddAttrNum <= 0)
	{
		return -1;
	}

	stGood.iAddAttrNum = 0;
	stBad.iAddAttrNum = 0;
	
	for (i=0; i<pstAddAttrRandList->iAddAttrNum; i++)
	{
		iIsGood = 0;
		pstAttrDef = z_find_new_addattr_def(pstEnv, pstAddAttrRandList->iDefID[i]);
		if (!pstAttrDef)
		{
			return -1;
		}

		switch(pstItemDef->WearPre.Career)
		{
		case CAREER_WARRIOR:
			if (pstAttrDef->Main&ATTR_MAIN_ZHANSHI)
			{
				iIsGood = 1;
			}
			break;
		case CAREER_MAGICOR:
			if (pstAttrDef->Main&ATTR_MAIN_FASHI)
			{
				iIsGood = 1;
			}
			break;
		case CAREER_GUNER:
			if (pstAttrDef->Main&ATTR_MAIN_QIANGSHOU)
			{
				iIsGood = 1;
			}
			break;
		case CAREER_CHURCH:
			if (pstAttrDef->Main&ATTR_MAIN_JISI)
			{
				iIsGood = 1;
			}
			break;
		case CAREER_WARRIOR_SINGLEHAND:
			if (pstAttrDef->Main&ATTR_MAIN_SHOUHU)
			{
				iIsGood = 1;
			}
			break;
		case CAREER_WARRIOR_DOUBLEHAND:
			if (pstAttrDef->Main&ATTR_MAIN_ZHENGFU)
			{
				iIsGood = 1;
			}
			break;
		case CAREER_MAGICOR_SINGLEHAND:
			if (pstAttrDef->Main&ATTR_MAIN_JUNENG)
			{
				iIsGood = 1;
			}
			break;
		case CAREER_MAGICOR_DOUBLEHAND:
			if (pstAttrDef->Main&ATTR_MAIN_LIANHUN)
			{
				iIsGood = 1;
			}
			break;
		case CAREER_GUNER_SINGLEHAND:
			if (pstAttrDef->Main&ATTR_MAIN_LIESHA)
			{
				iIsGood = 1;
			}
			break;
		case CAREER_GUNER_DOUBLEHAND:
			if (pstAttrDef->Main&ATTR_MAIN_HUIMIE)
			{
				iIsGood = 1;
			}
			break;
		case CAREER_CHURCH_SINGLEHAND:
			if (pstAttrDef->Main&ATTR_MAIN_DANAI)
			{
				iIsGood = 1;
			}
			break;
		case CAREER_CHURCH_DOUBLEHAND:
			if (pstAttrDef->Main&ATTR_MAIN_ERNAI)
			{
				iIsGood = 1;
			}
			break;
		default:
			break;
		}

		if (iIsGood)
		{
			stGood.iDefID[stGood.iAddAttrNum] = pstAddAttrRandList->iDefID[i];
			stGood.iAddAttrNum ++;
		}
		else
		{
			stBad.iDefID[stBad.iAddAttrNum] = pstAddAttrRandList->iDefID[i];
			stBad.iAddAttrNum ++;
		}
	}

	if (stGood.iAddAttrNum < iGood)
	{
		return -1;
	}

	if (stBad.iAddAttrNum < iAddAttrNum - iGood )
	{
		return -1;	
	}

	ROLEARM stArm = *pstArm;
	stArm.AddAttrNum = 0;
	for (i=0; i<iGood; i++)
	{
		iIdx = RAND1(stGood.iAddAttrNum);
		item_add_attr_insert(pstEnv, &stArm, stGood.iDefID[iIdx]);
		stGood.iAddAttrNum--;
		if(iIdx != stGood.iAddAttrNum)
		{
			memmove(&stGood.iDefID[iIdx], &stGood.iDefID[iIdx+1], sizeof(stGood.iDefID[0])*(stGood.iAddAttrNum - iIdx));
		}
	}


	for (i=0; i<iAddAttrNum - iGood; i++)
	{
		iIdx = RAND1(stBad.iAddAttrNum);
		item_add_attr_insert(pstEnv, &stArm, stBad.iDefID[iIdx]);
		stBad.iAddAttrNum--;
		if(iIdx != stBad.iAddAttrNum)
		{
			memmove(&stBad.iDefID[iIdx], &stBad.iDefID[iIdx+1], sizeof(stBad.iDefID[0])*(stBad.iAddAttrNum - iIdx));
		}
	}

	arm_base_attr_create(pstEnv, pstItemDef, &stArm);

	// 保存到online中,掉线不管了
	pstPlayer->stOnline.stArmResetOL.iAttrNum = stArm.AddAttrNum;
	memcpy(&pstPlayer->stOnline.stArmResetOL.AddAttr[0], &stArm.AddAttr[0], 
			sizeof(pstPlayer->stOnline.stArmResetOL.AddAttr));
	pstPlayer->stOnline.stArmResetOL.WID = pstArm->WID;
	
	return 0;
}


int player_arm_skill_active(ZONESVRENV* pstEnv, Player *pstPlayer, ARMSKILLACTIVE *pstActive)
{
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	int iSucc = 0;
	ROLEGRID *pstSrc;
	ROLEARM *pstArm;
	ITEMDEF *pstItemDef;
	ARMSKILLNUMDEF *pstNumDef;
	ROLEGRID *pstUseGrid;
	ITEMDEF *pstUseItemDef;
	int iPos;
	int iMoney;

	memset(pstArmSvr, 0, sizeof(*pstArmSvr));

	pstSrc =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstActive->Src.ArmGridIdx);
	if (!pstSrc || pstSrc->Type != GRID_ITEM_ARM)
	{
		goto end;
	}

	pstArm = &pstSrc->GridData.RoleArm;
	if (pstArm->ArmSkill.Max <= pstArm->ArmSkill.Num)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_ARM914);
		goto end;
	}

	if (pstActive->Idx != pstArm->ArmSkill.Num ||
		pstArm->ArmSkill.Num >= MAX_ARM_SKILL_NUM)
	{
		goto end;
	}

	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstSrc->GridData.RoleItem);
	if (!pstItemDef)
	{
		goto end;
	}

	pstUseGrid =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstActive->Use.ArmGridIdx);
	if (!pstUseGrid || pstUseGrid->Type != GRID_ITEM_ITEM)
	{
		goto end;
	}

	iPos = pstUseGrid - &pstPlayer->stRoleData.Package.SundriesPackage.RoleGrids[0];
	if (iPos < 0 || iPos >= pstPlayer->stRoleData.Package.SundriesPackage.Num)
	{
		goto end;
	}

	pstUseItemDef = z_get_itemdef_roleitem(pstEnv, &pstUseGrid->GridData.RoleItem);
	if (!pstUseItemDef ||pstUseItemDef->Result[0].ResultID != RESULT_ARM_SKILL_ACTIVE)
	{
		goto end;
	}

	pstNumDef = z_find_arm_skillnum_def(pstEnv, pstItemDef->WearPre.RoleLevelMin/10,
										pstItemDef->WearPos1, pstItemDef->Important);
	if (!pstNumDef || (int)pstUseItemDef->ItemID != pstNumDef->Active[pstArm->ArmSkill.Num].UseItemID)
	{
		goto end;
	}

	if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstUseItemDef->ItemID, 
							pstNumDef->Active[pstArm->ArmSkill.Num].UseItemNum, 
							LIST_TYPE_SUNDRIES, &iPos,0))
	{
		goto end;
	}

	iMoney = pstNumDef->Active[pstArm->ArmSkill.Num].UseMoney;
	if (package_dec_money_test(pstEnv, pstPlayer,ATTR_ID_MONEY, iMoney) < 0)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_ARM915);
		goto end;
	}	

	package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstUseItemDef->ItemID, 
							pstNumDef->Active[pstArm->ArmSkill.Num].UseItemNum, 
							LIST_TYPE_SUNDRIES, &iPos,0, 0);
	package_dec_money(pstEnv, pstPlayer,ATTR_ID_MONEY, iMoney);


	pstSrc =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstActive->Src.ArmGridIdx);
	if (!pstSrc || pstSrc->Type != GRID_ITEM_ARM)
	{
		goto end;
	}

	pstArm = &pstSrc->GridData.RoleArm;
	pstArm->ArmSkill.Num ++;
	arm_one_grid_update(pstEnv,  pstPlayer, pstSrc);

	iSucc = 1;
end:

	pstArmSvr->Flag = iSucc;
	pstArmSvr->ArmSvrType = ARM_SKILL_ACTIVE;
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_arm_base_reset_save(ZONESVRENV* pstEnv, Player *pstPlayer, ARMBASERESETSAVE*pstSave)
{
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	int iSucc = 0;
	ROLEGRID *pstSrc;
	ROLEARM *pstArm;

	memset(pstArmSvr, 0, sizeof(*pstArmSvr));

	pstSrc =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstSave->Src.ArmGridIdx);
	if (!pstSrc || pstSrc->Type != GRID_ITEM_ARM)
	{
		goto end;
	}

	pstArm = &pstSrc->GridData.RoleArm;

	if (pstPlayer->stOnline.stArmResetOL.iAttrNum != pstArm->AddAttrNum ||
		pstPlayer->stOnline.stArmResetOL.WID != pstArm->WID)
	{
		goto end;
	}

	memcpy(&pstArm->AddAttr[0], &pstPlayer->stOnline.stArmResetOL.AddAttr[0],sizeof(pstArm->AddAttr));

	//pstArm->InstFlag |= INST_ITEM_BIND;
	arm_one_grid_update(pstEnv,  pstPlayer, pstSrc);

	pstPlayer->stOnline.stArmResetOL.iAttrNum = 0;
	iSucc = 1;

end:

	pstArmSvr->Flag = iSucc;
	pstArmSvr->ArmSvrType = ARM_BASE_ATTR_RESET_SAVE;
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_arm_base_reset(ZONESVRENV* pstEnv, Player *pstPlayer, ARMBASERESET*pstReset)
{
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	ROLEGRID *pstSrc;
	ITEMDEF *pstItemDef;

	ROLEGRID *pstUseItem;
	ITEMDEF *pstUseItemDef;
	int iUsePos = -1;
	int iSucc = 0;
	int i;
	int iPos;
	ROLEGRID *pstUseArm;
	ITEMDEF *pstUseArmDef;
	int iVal = 0;
	ARMGRADEVALDEF *pstValDef;
	ARMRESETDEF *pstResetDef;
	int iBind = 0;

	memset(pstArmSvr, 0, sizeof(*pstArmSvr));


	// 必定杂物包
	pstSrc =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstReset->Src.ArmGridIdx);
	if (!pstSrc || pstSrc->Type != GRID_ITEM_ARM)
	{
		goto end;
	}

	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstSrc->GridData.RoleItem);
	if (!pstItemDef)
	{
		goto end;
	}

	// 白装不可以重置
	if (pstItemDef->Important < 1)
	{
		goto end;
	}

	// 可重置部位：主手、副手、头盔、肩膀、胸甲、鞋子面饰
	if (	pstItemDef->WearPos1 != WEAR_POS_RIGHTHAND &&
		pstItemDef->WearPos1 != WEAR_POS_HEAD &&
		pstItemDef->WearPos1 != WEAR_POS_BODY &&
		pstItemDef->WearPos1 != WEAR_POS_LEFT &&
		pstItemDef->WearPos1 != WEAR_POS_SHOULDER &&
		pstItemDef->WearPos1 != WEAR_POS_SHOE )
	{
		goto end;
	}

	pstUseItem =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstReset->UseItem.ArmGridIdx);
	if (!pstUseItem)
	{
		goto end;
	}

	if (pstUseItem->GridData.RoleItem.InstFlag &INST_ITEM_BIND)
	{
		iBind = 1;
	}

	pstUseItemDef = z_get_itemdef_roleitem(pstEnv, &pstUseItem->GridData.RoleItem);
	if (!pstUseItemDef ||pstUseItemDef->Result[0].ResultID != RESULT_BASE_ATTR_RESET)
	{
		goto end;
	}

	// 齿轮等级为可以重置多少级以下的装备
	if (pstItemDef->WearPre.RoleLevelMin > pstUseItemDef->Result[0].ResultVal1)
	{
		goto end;
	}

	iUsePos = pstUseItem - &pstPlayer->stRoleData.Package.SundriesPackage.RoleGrids[0];
	if (iUsePos < 0 || iUsePos >= pstPlayer->stRoleData.Package.SundriesPackage.Num)
	{
		goto end;
	}


	if (pstReset->ArmNum > ARM_BASE_ATTR_RESET_MAX)
	{
		goto end;
	}
	
	for (i=0; i<pstReset->ArmNum; i++)
	{
		iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, 
								pstReset->ArmStuffs[i].ArmGridIdx);
		if (0 > iPos)
		{
			goto end;
		}	

		pstUseArm = pstPak->SundriesPackage.RoleGrids+iPos;
		pstUseArmDef = z_get_itemdef_roleitem(pstEnv, &pstUseArm->GridData.RoleItem);
		if (!pstUseArmDef)
		{
			goto end;
		}

		if (pstUseArm->GridData.RoleItem.InstFlag &INST_ITEM_BIND)
		{
			iBind = 1;
		}

		if (pstUseArmDef->WearPre.RoleLevelMin/10 != pstItemDef->WearPre.RoleLevelMin/10)
		{
			goto end;
		}

		pstValDef = z_find_arm_val_def(pstEnv, pstUseArmDef->Important);
		if (!pstValDef)
		{
			goto end;
		}

		iVal += pstValDef->Val;
		
	}

	pstResetDef = z_find_arm_reset_def(pstEnv, pstItemDef->WearPre.RoleLevelMin/10);
	if (!pstResetDef)
	{
		goto end;
	}

	if (pstResetDef->UseMoney > 0)
	{
		if (package_dec_money_test(pstEnv, pstPlayer,ATTR_ID_MONEY, pstResetDef->UseMoney) < 0)
		{
			z_send_sysmsg(pstEnv->pstAppCtx,pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_ARM916);
			goto end;
		}	
	}


	// 求出在第几个区间
	int iIdx = base_reset_calc_dist(pstResetDef, iVal);
	
	// 求出好的关键字有几个
	int iGood = base_reset_calc_good(pstResetDef, iIdx, pstItemDef->Important);


	// 重置绿色属性
	if (arm_base_reset_in(pstEnv,pstPlayer,&pstSrc->GridData.RoleArm, pstItemDef,  iGood) < 0)
	{
		goto end;
	}

	if (iBind && !(pstSrc->GridData.RoleArm.InstFlag & INST_ITEM_BIND))
	{
		pstSrc->GridData.RoleArm.InstFlag|=	INST_ITEM_BIND;
		arm_one_grid_update(pstEnv,  pstPlayer, pstSrc);
	}

	if (pstResetDef->UseMoney > 0)
	{
		package_dec_money(pstEnv, pstPlayer,ATTR_ID_MONEY, pstResetDef->UseMoney);
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ARM_LEVELUP, 
				0, 0, pstResetDef->UseMoney, "绿色属性重置");
	}

	z_role_machining_oplog(pstEnv, pstPlayer, &pstSrc->GridData.RoleArm, NULL,
			NULL, OP_MACHINING_ARM_BASE_RESET, 1, NULL,0);

	package_dec(pstEnv->pstAppCtx, pstEnv,pstPlayer, pstUseItemDef->ItemID,1,LIST_TYPE_SUNDRIES,&iUsePos,0,0);

	for (i=0; i<pstReset->ArmNum; i++)
	{
		iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, 
								pstReset->ArmStuffs[i].ArmGridIdx);
		if (0 > iPos)
		{
			return -1;
		}	

		pstUseArm = pstPak->SundriesPackage.RoleGrids+iPos;

		z_role_item_drop_oplog(pstEnv,pstPlayer, pstUseArm);
		package_dec(pstEnv->pstAppCtx, pstEnv,pstPlayer,pstUseArm->GridData.RoleItem.DefID,1,LIST_TYPE_SUNDRIES,&iPos,0,0);
	}

	iSucc = 1;

end:	

	pstArmSvr->Flag = iSucc;
	pstArmSvr->ArmSvrType = ARM_BASE_ATTR_RESET;

	if (iSucc)
	{
		pstArmSvr->AttrNum = pstPlayer->stOnline.stArmResetOL.iAttrNum;
		memcpy(&pstArmSvr->AddAttr[0], &pstPlayer->stOnline.stArmResetOL.AddAttr[0], sizeof(pstArmSvr->AddAttr));
		pstArmSvr->WID = pstSrc->GridData.RoleArm.WID;
	}

	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_arm_step_up(ZONESVRENV* pstEnv, Player *pstPlayer, ARMSTEPUP *pstUp)
{
	ROLEGRID *pstSrc;
	ITEMDEF *pstItemDef;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	int iPos = -1;
	int i;
	ROLEGRID *pstDst;
	ARMSTEPUPDEF *pstDef;
	int iCurrStep;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	ITEMDEF *pstStuffItemDef;
	int iSucc = 0;
	int iBind = 0;

	memset(pstArmSvr, 0, sizeof(*pstArmSvr));

	// 必定杂物包
	pstSrc =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstUp->Src.ArmGridIdx);
	if (!pstSrc || pstSrc->Type != GRID_ITEM_ARM)
	{
		goto end;
	}

	// 检查升阶装备
	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstSrc->GridData.RoleItem);
	if (!pstItemDef)
	{
		goto end;
	}
	
	if (pstItemDef->WearPre.RoleLevelMin < 60)
	{
		goto end;
	}

	if (pstItemDef->Important < 3)
	{
		goto end;
	}

	if (	pstItemDef->WearPos1 > WEAR_POS_AMULET2 ||
		pstItemDef->WearPos1 == WEAR_POS_SACHET ||
		pstItemDef->WearPos1 == WEAR_POS_GRAIL ||
		pstItemDef->WearPos1 == WEAR_POS_QIYUE ||
		WEAR_POS_FASHION_BODY == pstItemDef->WearPos1 ||
		WEAR_POS_BACK == pstItemDef->WearPos1 ||
		WEAR_POS_NO == pstItemDef->WearPos1 )
	{
		goto end;
	}

	pstDef = z_find_arm_stepup_def(pstEnv, pstItemDef->WearPre.RoleLevelMin/10, 
									pstItemDef->WearPos1, pstItemDef->Important);
	if (!pstDef)
	{
		goto end;
	}

	iCurrStep = pstSrc->GridData.RoleArm.Step;
	if (iCurrStep >= pstDef->MaxStep || iCurrStep >= ARM_MAX_STEP)
	{
		goto end;
	}


	// 检查消耗---start----
	/*if (pstUp->ArmNum != pstDef->StepAttr[iCurrStep].ArmNum)
	{
		goto end;
	}*/

	if (package_dec_money_test(pstEnv, pstPlayer, pstDef->MoneyID, 
								pstDef->StepAttr[iCurrStep].MoneyNum*pstUp->ArmNum) < 0)
	{
		goto end;
	}

	if (pstDef->ItemID > 0 && pstDef->StepAttr[iCurrStep].ItemNum)
	{
		if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstDef->ItemID, 
							pstDef->StepAttr[iCurrStep].ItemNum * pstUp->ArmNum, -1, NULL,0))
		{
			goto end;
		}
	}
	
	for (i=0; i<pstUp->ArmNum; i++)
	{
		iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, 
								pstUp->ArmStuffs[i].ArmGridIdx);
		if (0 > iPos)
		{
			goto end;
		}

		pstDst = &pstPak->SundriesPackage.RoleGrids[iPos];
		pstStuffItemDef = z_get_itemdef_roleitem(pstEnv, &pstDst->GridData.RoleItem);
		if (!pstStuffItemDef)
		{
			goto end;
		}
		
		if (pstStuffItemDef->WearPre.RoleLevelMin != pstItemDef->WearPre.RoleLevelMin)
		{
			goto end;
		}

		if (pstStuffItemDef->Important < pstDef->UseArmImp)
		{
			goto end;
		}

		if (pstStuffItemDef->WearPos1 != pstItemDef->WearPos1)
		{
			goto end;
		}

		if (pstDst->GridData.RoleItem.InstFlag & INST_ITEM_BIND)
		{
			iBind = 1;
		}
	}
	// 检查消耗---end----


	// 删除金钱，材料
	package_dec_money(pstEnv, pstPlayer, pstDef->MoneyID, 
								pstDef->StepAttr[iCurrStep].MoneyNum*pstUp->ArmNum);

	if (pstDef->ItemID > 0 && pstDef->StepAttr[iCurrStep].ItemNum)
	{
		if (package_dec(pstEnv->pstAppCtx, pstEnv,pstPlayer, pstDef->ItemID,
					pstDef->StepAttr[iCurrStep].ItemNum*pstUp->ArmNum,-1,NULL,0,0) > 0)
		{
			iBind = 1;
		} 
	}

	// 重新获取下指针
	pstSrc =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstUp->Src.ArmGridIdx);
	if (!pstSrc || pstSrc->Type != GRID_ITEM_ARM)
	{
		// log
		goto end;
	}

	// 阶层加1,通知客户端 
	pstSrc->GridData.RoleArm.StepUse += pstUp->ArmNum;
	if (pstSrc->GridData.RoleArm.StepUse >= pstDef->StepAttr[iCurrStep].ArmNum)
	{
		pstSrc->GridData.RoleArm.Step ++;
		pstSrc->GridData.RoleArm.StepUse -= pstDef->StepAttr[iCurrStep].ArmNum;
	}
	
	//背饰进阶后幻化效果会消失
	pstSrc->GridData.RoleArm.UnrealBackID = 0;
	pstSrc->GridData.RoleArm.UnrealBackTmEnd = 0;

	if (iBind)
		pstSrc->GridData.RoleArm.InstFlag |= INST_ITEM_BIND;
	arm_one_grid_update(pstEnv,  pstPlayer, pstSrc);	
	
	
	for (i=0; i<pstUp->ArmNum; i++)
	{
		iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, 
								pstUp->ArmStuffs[i].ArmGridIdx);
		if (0 > iPos)
		{
			return -1;
		}

		pstDst = &pstPak->SundriesPackage.RoleGrids[iPos];
		package_dec(pstEnv->pstAppCtx, pstEnv,pstPlayer, pstDst->GridData.RoleArm.DefID,
						1,LIST_TYPE_SUNDRIES,&iPos,0,0);
	}	

	iSucc = 1;

end:
	pstArmSvr->Flag = iSucc;
	pstArmSvr->ArmSvrType = ARM_ACT_STEP_UP;

	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int  z_arm_step_attr_calc(ZONESVRENV* pstEnv, DYNATTRLIST *pstDyn, ROLEARM *pstRoleArm, ITEMDEF *pstDef)
{
	ARMSTEPUPDEF *pstStepDef;
	RESULT stResult;
	
	if (pstRoleArm->Step <= 0)
	{
		return 0;
	}

	pstStepDef = z_find_arm_stepup_def(pstEnv, pstDef->WearPre.RoleLevelMin/10, 
										pstDef->WearPos1, pstDef->Important);
	if (!pstStepDef || pstRoleArm->Step > pstStepDef->MaxStep)
	{
		return 0;
	}

	stResult.ResultID = pstStepDef->AttrID;
	stResult.ResultVal1 = pstStepDef->StepAttr[pstRoleArm->Step-1].Val;
	stResult.ResultVal2 = 0;
	stResult.ResultVal3 = 0;
	stResult.ResultVal4 = 0;

	z_attr_result(pstEnv, pstDyn, pstRoleArm, &stResult);
	return 0;
}

int arm_legend_up_in(ZONESVRENV* pstEnv, ROLEARM *pstSrc, ARMLEGENDUP *pstUp, 
						ROLEGRID *pstCreate,ARMLEVELUPDEF *pstLvlDef)
{
	int i,j;
	ROLEARM *pstDst = &pstCreate->GridData.RoleArm;
	char szC[MAX_ARM_ADD_ATTR];
	
	if (item_create(pstEnv->pstAppCtx, pstEnv, pstCreate,pstLvlDef->NextItemID) < 0)
	{
		return -1;
	}

	memset(szC, 0, sizeof(szC));

	// 继承属性
	for (i=pstUp->Num-1; i>=0; i--)
	{
		if (pstUp->Idxs[i] >= pstSrc->AddAttrNum)
		{
			return -1;
		}

		// 如果有相同的继承属性,赋值就行了
		for (j=0; j<pstDst->AddAttrNum; j++)
		{
			if (pstDst->AddAttr[j].AddAttrID == pstSrc->AddAttr[(int)pstUp->Idxs[i]].AddAttrID)
			{
				pstDst->AddAttr[j].Val = pstSrc->AddAttr[(int)pstUp->Idxs[i]].Val;
				szC[j] = 1;

				pstUp->Num --;
				if (i != pstUp->Num)
				{
					pstUp->Idxs[i] = pstUp->Idxs[(int)pstUp->Num];
				}
				break;
			}
		}
	}

	for (i=0; i<pstUp->Num; i++)
	{	
		for (j=0;j<MAX_ARM_ADD_ATTR;j++)
		{
			if (!szC[j])
			{
				pstDst->AddAttr[j] = pstSrc->AddAttr[(int)pstUp->Idxs[i]];
				szC[j] = 1;
				break;
			}
		}	
	}
	

	//  宝石
	pstDst->HoleNum = pstSrc->HoleNum;
	memcpy(&pstDst->HoleGem[0], &pstSrc->HoleGem[0], sizeof(pstDst->HoleGem));

	// 星级
	pstDst->StrengthenCount = pstSrc->StrengthenCount;
	pstDst->StarLv = pstSrc->StarLv;
	memcpy(&pstDst->StarInfo[0], &pstSrc->StarInfo[0], MAX_ARM_STAR_LEVEL*sizeof(pstDst->StarInfo[0]));

	//附魔
	pstDst->ExtendedResult = pstSrc->ExtendedResult;
	pstDst->ExtendedFromDefID = pstSrc->ExtendedFromDefID;

	// 阶层
	pstDst->Step = pstSrc->Step;

	// 装备技能
	pstDst->ArmSkill = pstSrc->ArmSkill;
	return 0;
}

int player_arm_legend_up(ZONESVRENV* pstEnv, Player *pstPlayer, ARMLEGENDUP *pstUp)
{
	ROLEGRID *pstSrcItem;
	ROLEGRID *pstSrcArm;
	USEDEF *pstUseDef;
	int iSucc = 0;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	int iBind = 0;
	ARMLEVELUPDEF *pstLvlDef;
	ITEMDEF *pstSrcItemDef;
	//ITEMDEF *pstSrcArmDef;
	ITEMDEF *pstDstDef;
	int i;
	int iUseID=0;
	int iMoney = 0;
	ROLEGRID stCreate;
	int iPos;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;

	pstSrcItem =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstUp->SrcScroll.ArmGridIdx);
	if (!pstSrcItem || pstSrcItem->Type != GRID_ITEM_ITEM)
	{
		goto final;
	}

	if (pstSrcItem->GridData.RoleItem.InstFlag&INST_ITEM_BIND)
	{
		iBind =1;
	}

	pstSrcItemDef =  z_get_itemdef_roleitem(pstEnv, &pstSrcItem->GridData.RoleItem);
	if (!pstSrcItemDef )
	{
		goto final;
	}

	pstSrcArm =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstUp->SrcArm.ArmGridIdx);
	if (!pstSrcArm || pstSrcArm->Type != GRID_ITEM_ARM)
	{
		goto final;
	}

	if (	pstSrcItemDef->Result[0].ResultID != RESULT_LEGEND_SCROLL ||
		pstSrcItemDef->Result[0].ResultVal2 != pstSrcArm->GridData.RoleItem.DefID)
	{
		goto final;	
	}

	if (pstSrcArm->GridData.RoleItem.InstFlag&INST_ITEM_BIND)
	{
		iBind =1;
	}

	 pstLvlDef = z_find_arm_levelup_def(pstEnv, pstSrcArm->GridData.RoleItem.DefID);
	 if (!pstLvlDef)
	 {
		goto final;
	 }

	if (pstUp->Num == 1)
	{
		iUseID = USE_ID_LEGEND_UP_1;
	}
	else if (pstUp->Num == 2)
	{
		iUseID = USE_ID_LEGEND_UP_2;
	}
	else if (pstUp->Num == 3)
	{
		iUseID = USE_ID_LEGEND_UP_3;
	}

	if (iUseID > 0)
	{
		pstUseDef = z_find_usedef(pstEnv, iUseID);
		if (!pstUseDef)
		{
			goto final;
		}

		iMoney = pstUseDef->UseMoney + pstLvlDef->MoneyVal;
	}
	else
	{
		iMoney = pstLvlDef->MoneyVal;	
	}
	
	if (iMoney > 0)
	{
		if (package_dec_money_test(pstEnv, pstPlayer,ATTR_ID_MONEY,iMoney) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NOMONEY);
			goto final;
		}
	}
	
	for (i=0; i<MAX_ARM_LEVELUP_STUFF; i++)
	{
		if (	pstLvlDef->Stuff[i].ItemID <= 0 ||
			pstLvlDef->Stuff[i].Num <= 0)
		{
			break;
		}

		if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstLvlDef->Stuff[i].ItemID, 
				pstLvlDef->Stuff[i].Num, -1, NULL,0))
		{
			goto final;
		}
	}
	
	if (arm_legend_up_in(pstEnv, &pstSrcArm->GridData.RoleArm,pstUp, &stCreate,pstLvlDef) < 0)
	{
		goto final;	
	}

	
	iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num,  
							pstUp->SrcScroll.ArmGridIdx);
	if (0 > iPos)
	{
		goto final;
	}
	package_dec(pstEnv->pstAppCtx,pstEnv,pstPlayer, pstPak->SundriesPackage.RoleGrids[iPos].GridData.RoleItem.DefID, 
					1, LIST_TYPE_SUNDRIES, &iPos, 0, 0);

	iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num,  
							pstUp->SrcArm.ArmGridIdx);
	if (0 > iPos)
	{
		goto final;
	}
	package_dec(pstEnv->pstAppCtx,pstEnv,pstPlayer, pstPak->SundriesPackage.RoleGrids[iPos].GridData.RoleItem.DefID, 
					1, LIST_TYPE_SUNDRIES, &iPos, 0, 0);

	if (iMoney > 0)
	{
		package_dec_money(pstEnv, pstPlayer,ATTR_ID_MONEY,iMoney);
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ARM_LEVELUP, 
				0, 0, iMoney, "升级为传奇装备");
	}

	for (i=0; i<MAX_ARM_LEVELUP_STUFF; i++)
	{
		if (	pstLvlDef->Stuff[i].ItemID <= 0 ||
			pstLvlDef->Stuff[i].Num <= 0)
		{
			break;

		}

		if (package_dec(pstEnv->pstAppCtx,pstEnv,pstPlayer,pstLvlDef->Stuff[i].ItemID,
						pstLvlDef->Stuff[i].Num, -1,NULL,0,0) > 0)
		{
			iBind = 1;
		}
	}

	if (iBind)
	{
		stCreate.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
	}

	package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer,&stCreate, 1 ,0,0);

	pstDstDef = z_find_itemdef(pstEnv, stCreate.GridData.RoleItem.DefID);
	if (pstDstDef)
	{
		CSPKG stPkgLink;
		z_fill_link_msg(&stPkgLink, SYS_SHOW, CHAT_LINK_ARM, 
						(void *)&stCreate.GridData.RoleArm, 
						ZONEERR_ARM917,pstPlayer->stRoleData.RoleName);
		z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkgLink);

		
		/*z_sys_strf_broadcast(pstEnv, SYS_SHOW, 
			ZONEERR_ARM917,
			pstPlayer->stRoleData.RoleName, pstDstDef->Name);*/
	}

	iSucc = 1;
	
final:
	pstArmSvr->Flag = iSucc;
	pstArmSvr->ArmSvrType = ARM_LEGEND_UP;

	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);		
	return 0;
}

int player_arm_check(ZONESVRENV* pstEnv, Player *pstPlayer, ARMCHECK*pstCheck)
{
	ROLEGRID *pstSrc;
	ROLEGRID *pstUse;
	int iSucc = 0;
	ITEMDEF *pstUseItemDef;
	ITEMDEF *pstSrcItemDef;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	int iPos = -1;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	tdr_ulonglong ullWid = 0;
	int iBind;

	memset(pstArmSvr, 0, sizeof(*pstArmSvr));
	
	pstSrc =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstCheck->Arm.ArmGridIdx);
	if (!pstSrc || pstSrc->Type != GRID_ITEM_ARM)
	{
		goto final;
	}

	if (!(pstSrc->GridData.RoleArm.InstFlag & INST_ITEM_UNCHECK))
	{
		goto final;
	}

	ullWid = pstSrc->GridData.RoleArm.WID;

	pstSrcItemDef = z_get_itemdef_roleitem(pstEnv, &pstSrc->GridData.RoleItem);
	if (!pstSrcItemDef)
	{
		goto final;
	}

	pstUse =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstCheck->UseItem.ArmGridIdx);
	if (!pstUse)
	{
		goto final;
	}

	pstUseItemDef = z_get_itemdef_roleitem(pstEnv, &pstUse->GridData.RoleItem);
	if (!pstUseItemDef || pstUseItemDef->Result[0].ResultID != RESULT_ARM_CHECK)
	{
		goto final;
	}

	if (pstUseItemDef->Result[0].ResultVal1 < pstSrcItemDef->WearPre.RoleLevelMin)
	{
		goto final;
	}

	iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, 
							pstCheck->UseItem.ArmGridIdx);
	if (0 > iPos)
	{
		goto final;
	}


	item_add_attr_create(pstEnv, &pstSrc->GridData.RoleArm, pstSrcItemDef);
	arm_base_attr_create(pstEnv, pstSrcItemDef, &pstSrc->GridData.RoleArm);
	pstSrc->GridData.RoleArm.InstFlag &= ~INST_ITEM_UNCHECK;
	if (pstUse->GridData.RoleItem.InstFlag&INST_ITEM_BIND)
	{
		pstSrc->GridData.RoleArm.InstFlag |= INST_ITEM_BIND;	
	}
	arm_one_grid_update(pstEnv,  pstPlayer, pstSrc);
	
	
	iBind = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstUseItemDef->ItemID, 
				1, LIST_TYPE_SUNDRIES, &iPos,PACKAGE_DEC_NONE, 0);

	MACHINNGEXPENDDATA stExpendData;
	bzero(&stExpendData, sizeof(stExpendData));
	stExpendData.StuffNum = 1;
	itemdec_fill(&stExpendData.StuffInfo[0],  pstUseItemDef->ItemID, 1, iBind);
	z_role_machining_oplog(pstEnv, pstPlayer,NULL, &pstSrc->GridData.RoleArm,
							&stExpendData, OP_MACHINING_ARM_ATTR_CHECK, 1, NULL,0);
	iSucc = 1;
final:

	pstArmSvr->Flag = iSucc;
	pstArmSvr->ArmSvrType = ARM_ACT_CHECK;
	pstArmSvr->WID = ullWid;

	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int arm_skill_lock(ZONESVRENV* pstEnv, Player *pstPlayer, ARMSKILLRECASTLOCK *pstLock)
{
	ROLEGRID *pstSrc;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	int iSucc = 0;
	int i;
	int iLock = 0;

	memset(pstArmSvr, 0, sizeof(*pstArmSvr));
	
	pstSrc =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstLock->Src.ArmGridIdx);
	if (!pstSrc || pstSrc->Type != GRID_ITEM_ARM)
	{
		goto end;
	}

	ARMSKILLDATA *pstSkill = &pstSrc->GridData.RoleArm.ArmSkill;
	for (i=0; i<pstSkill->Num; i++)
	{
		if (pstSkill->One[i].Lock == 1)
		{
			iLock++;
		}
	}


	if (pstSkill->One[pstLock->Idx].Lock == 0)
	{
		if (iLock >= pstSkill->Num-1)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET702);
			goto end;
		}
			
		pstSkill->One[pstLock->Idx].Lock = 1;	
	}
	else
	{
		pstSkill->One[pstLock->Idx].Lock = 0;
	}

	arm_one_grid_update(pstEnv,  pstPlayer, pstSrc);
	iSucc = 1;
end:

	pstArmSvr->Flag = iSucc;
	pstArmSvr->ArmSvrType = ARM_SKILL_RECAST_LOCK;

	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

ARMSKILLRECASTDEF *z_find_armskillrecastdef(ZONESVRENV *pstEnv, int iLvl)
{
	int iIndex;
	int iEqu;
	ARMSKILLRECASTDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	stDef.Lvl =iLvl;
	iIndex = bsearch_int(&stDef, pstObj->sArmSkillRecastDef, pstObj->iArmSkillRecastDef,SKILL_ARM_RECAST_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ARMSKILLRECASTDEF*)pstObj->sArmSkillRecastDef[iIndex];
		
	}
	return NULL;
}

int arm_skill_recast_reset(ZONESVRENV* pstEnv, Player *pstPlayer,ARMSKILLRECASTRESET *pstReset)
{
	ROLEGRID *pstSrc;
	//ROLEGRID *pstStuff;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	int iSucc = 0;
	int iLock = 0;
	int i,j;
	int iLockuse = 0;
	int iLockItem = 0;


	memset(pstArmSvr, 0, sizeof(*pstArmSvr));

	pstSrc =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstReset->Src.ArmGridIdx);
	if (!pstSrc || pstSrc->Type != GRID_ITEM_ARM)
	{
		goto end;
	}

	/*pstStuff = get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstReset->Stuff.ArmGridIdx);
	if (!pstStuff || pstStuff->Type == GRID_ITEM_ARM)
	{
		goto end;
	}

	iPos = pstStuff - &pstPlayer->stRoleData.Package.SundriesPackage.RoleGrids[0];*/

	ARMSKILLATTRDEF *pstAttrDef;
	ARMSKILLRECASTDEF *pstDef;
	ITEMDEF *pstItemDef;

	pstItemDef = z_find_itemdef( pstEnv, pstSrc->GridData.RoleItem.DefID);
	if (!pstItemDef)
	{
		goto end;
	}

	pstDef = z_find_armskillrecastdef(pstEnv, pstItemDef->WearPre.RoleLevelMin/10);
	if (!pstDef)
		goto end;

	ARMSKILLDATA *pstSkill = &pstSrc->GridData.RoleArm.ArmSkill;
	if (pstSkill->Num <= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET703);
		
		goto end;
	}
	
	for (i=0; i<pstSkill->Num; i++)
	{
		if (pstSkill->One[i].Lock)
		{
			iLock ++;
		}
	}

	iLockuse = pstDef->ResetMoneyNum/**(1.0+0.1*iLock)*/;
	iLockItem = pstDef->ResetItemNum +iLock;
	if (pstDef->ResetItemID > 0 && iLockItem > 0)
	{
		if (package_dec_test(pstEnv->pstAppCtx,pstEnv,pstPlayer, pstDef->ResetItemID,
						iLockItem, LIST_TYPE_SUNDRIES, NULL,0) < 0)
		{
			goto end;
		}
	}

	if (pstDef->ResetMoneyType > 0 && iLockuse > 0)
	{
		if (package_dec_money_test(pstEnv, pstPlayer,pstDef->ResetMoneyType,iLockuse) < 0)
		{
			goto end;
		}
	}

	ARMSKILLATTRDEF *ppstDef[100];
	int iNum = 0;
	int iIdx;

	for (i=0; i<pstEnv->pstZoneObj->iArmSkillAttrDef; i++)
	{
		pstAttrDef = (ARMSKILLATTRDEF *)pstEnv->pstZoneObj->sArmSkillAttrDef[i];
		
		if (arm_skill_career_get(pstAttrDef, pstItemDef->WearPre.Career))
		{
			ppstDef[iNum++] = pstAttrDef;

			if (iNum >= 100)
				break;
		}
	}

	if (iNum < 3)
	{
		goto end;
	}	

	for (i=0; i<pstSkill->Num; i++)
	{
		if (pstSkill->One[i].Lock)
		{
			for (j=iNum-1;j>=0; j--)
			{
				if (ppstDef[j]->ID == pstSkill->One[i].ID)
				{
					iNum -- ;
					if (j != iNum)
					{
						ppstDef[j] = ppstDef[iNum];
					}
				}
			}
			continue;
		}

		iIdx = RAND1(iNum);
		
		pstSkill->One[i].ID = ppstDef[iIdx]->ID;
		pstSkill->One[i].Idx = RAND1(MAX_IMP_GRADE); 

		for (j=iNum-1;j>=0; j--)
		{
			if (ppstDef[j]->ID == pstSkill->One[i].ID)
			{
				iNum -- ;
				if (j != iNum)
				{
					ppstDef[j] = ppstDef[iNum];
				}
			}
		}
	}

	pstSrc->GridData.RoleArm.InstFlag |= INST_ITEM_BIND;
	arm_one_grid_update(pstEnv,  pstPlayer, pstSrc);
	
	if (pstDef->ResetItemID > 0 && iLockItem > 0)
	{
		package_dec(pstEnv->pstAppCtx,pstEnv,pstPlayer, pstDef->ResetItemID,
						iLockItem, LIST_TYPE_SUNDRIES, NULL,0,0) ;
	}

	if (pstDef->ResetMoneyType > 0 && iLockuse > 0)
	{
		package_dec_money(pstEnv, pstPlayer,pstDef->ResetMoneyType,iLockuse);
	}

	
	
	iSucc = 1;
end:
	pstArmSvr->Flag = iSucc;
	pstArmSvr->ArmSvrType = ARM_SKILL_RECAST_RESET;

	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int arm_skill_recast_active(ZONESVRENV* pstEnv, Player *pstPlayer, ARMSKILLRECASTACTIVE *pstActive)
{
	ROLEGRID *pstSrc;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;
	int iSucc = 0;
	int i, j;
	ARMSKILLATTRDEF *pstAttrDef;
	ARMSKILLRECASTDEF *pstDef;
	ITEMDEF *pstItemDef;
	int iIdx;
	
	memset(pstArmSvr, 0, sizeof(*pstArmSvr));
	
	pstSrc =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstActive->Src.ArmGridIdx);
	if (!pstSrc || pstSrc->Type != GRID_ITEM_ARM)
	{
		goto end;
	}

	pstItemDef = z_find_itemdef( pstEnv, pstSrc->GridData.RoleItem.DefID);
	if (!pstItemDef)
	{
		goto end;
	}

	pstDef = z_find_armskillrecastdef(pstEnv, pstItemDef->WearPre.RoleLevelMin/10);
	if (!pstDef)
		goto end;

	
	ARMSKILLDATA *pstSkill = &pstSrc->GridData.RoleArm.ArmSkill;
	
	if (pstSkill->Num >= MAX_ARM_SKILL_NUM)
	{
		goto end;
	}

	iIdx = pstSkill->Num;
	if (pstDef->Active[iIdx].ItemID >0 && pstDef->Active[iIdx].ItemNum > 0)
	{
		if (package_dec_test(pstEnv->pstAppCtx,pstEnv,pstPlayer, pstDef->Active[iIdx].ItemID,
						pstDef->Active[iIdx].ItemNum, LIST_TYPE_SUNDRIES, NULL,0) < 0)
		{
			goto end;
		}
	}

	if (pstDef->Active[iIdx].MoneyNum > 0 && pstDef->Active[iIdx].MoneyType > 0)
	{
		if (package_dec_money_test(pstEnv, pstPlayer,pstDef->Active[iIdx].MoneyType,
									pstDef->Active[iIdx].MoneyNum) < 0)
		{
			goto end;
		}
	}


	for (i=0; i<pstEnv->pstZoneObj->iArmSkillAttrDef; i++)
	{
		pstAttrDef = (ARMSKILLATTRDEF *)pstEnv->pstZoneObj->sArmSkillAttrDef[i];
		for (j=0; j<pstSkill->Num; j++)
		{
			if (pstAttrDef->ID == pstSkill->One[j].ID)
			{
				break;
			}
		}

		if (j < pstSkill->Num)
		{
			continue;
		}

		if (arm_skill_career_get(pstAttrDef, pstItemDef->WearPre.Career))
		{
			pstSkill->One[pstSkill->Num].ID = pstAttrDef->ID;
			pstSkill->One[pstSkill->Num].Idx = RAND1(MAX_IMP_GRADE);  	// 随机修正
			pstSkill->Num++;

			pstSkill->Max = pstSkill->Num;
			break;
		}
	}

	if (i>=pstEnv->pstZoneObj->iArmSkillAttrDef)
	{
		goto end;
	}

	pstSrc->GridData.RoleArm.InstFlag |= INST_ITEM_BIND;
	arm_one_grid_update(pstEnv,  pstPlayer, pstSrc);

	if (pstDef->Active[iIdx].ItemID >0 && pstDef->Active[iIdx].ItemNum > 0)
	{
		package_dec(pstEnv->pstAppCtx,pstEnv,pstPlayer, pstDef->Active[iIdx].ItemID,
						pstDef->Active[iIdx].ItemNum, LIST_TYPE_SUNDRIES, NULL,0,0) ;
	}

	if (pstDef->Active[iIdx].MoneyNum > 0 && pstDef->Active[iIdx].MoneyType > 0)
	{
		package_dec_money(pstEnv, pstPlayer,pstDef->Active[iIdx].MoneyType,
									pstDef->Active[iIdx].MoneyNum) ;	
	}

	
	iSucc = 1;
end:

	pstArmSvr->Flag = iSucc;
	pstArmSvr->ArmSvrType = ARM_SKILL_RECAST_ACTIVE;

	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_arm(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSARMCLT *pstArmClt = &pstCsPkg->Body.ArmClt;
	
	UNUSED(pstFrameHead);

	switch(pstArmClt->ArmCltType)
	{
		case ARM_ACT_STAR:		
			//if (check_npc_ui(pstEnv,pstPlayer, UI_TYPE_ARM_OPERATE) < 0
			//	&& check_npc_ui(pstEnv, pstPlayer, UI_TYPE_ARM_STAR) < 0)
			//{
			//	return -1;
			//}
			return arm_star_up(pstEnv, pstPlayer, &pstArmClt->ArmCltData.ArmStar);
			break;

		case ARM_ACT_LEVEL:			
			//return arm_level_up(pstEnv, pstPlayer, &pstArmClt->ArmCltData.ArmLevel);
			break;

		case ARM_ACT_FIGHT:
			//return arm_attr_up(pstEnv, pstPlayer, &pstArmClt->ArmCltData.ArmFight, GEM_EFFECT_FIGHT);
			break;

		case ARM_ACT_IDENTIFIABILITY:	
			//暂时取消入口
			//return arm_identifiability(pstEnv, pstPlayer,  &pstArmClt->ArmCltData.ArmIdentifiability);
			break;

		case GEM_ACT_LEVEL_UP:			
			return gem_level_up(pstEnv, pstPlayer,  &pstArmClt->ArmCltData.GemLevelUp);
			break;

		case GEM_ACT_INLAY:
			return gem_inlay(pstEnv, pstPlayer,  &pstArmClt->ArmCltData.GemInlay);
			break;

		case GEM_ACT_PICK:			
			return gem_pick(pstEnv, pstPlayer,  &pstArmClt->ArmCltData.GemPick);
			break;
		case GEM_ACT_OPEN_HOLE:
			gem_open_hole( pstEnv, pstPlayer, &pstArmClt->ArmCltData.GemOpenHole);
			break;
		case ARM_MOVE_STAR:	
			//if(check_npc_ui(pstEnv,pstPlayer,UI_TYPE_ARM_OPERATE) < 0
			//   && check_npc_ui(pstEnv,pstPlayer,UI_TYPE_ARM_MOVE) < 0)
			//{
			//	return -1;
			//}
			arm_star_move(pstEnv, pstPlayer, &pstArmClt->ArmCltData.ArmMoveStar);
			break;
		case ARM_BIND:	
			//if (check_npc_ui(pstEnv,pstPlayer,UI_TYPE_ARM_OPERATE) < 0
			//	&& check_npc_ui(pstEnv, pstPlayer, UI_TYPE_ARM_BIND) < 0)
			//{
			//	return -1;
			//}
			arm_bind(pstEnv, pstPlayer, &pstArmClt->ArmCltData.ArmBind);
			break;
		case ARM_UNBIND:
			arm_unbind(pstEnv, pstPlayer, &pstArmClt->ArmCltData.ArmUnBind);
			break;
		case ARM_UNFENGYIN_PRE:
			//if (check_npc_ui(pstEnv, pstPlayer, UI_TYPE_ARM_UNFENGYIN) < 0)
			//{
			//	return -1;
			//}
			arm_unfengyin_pre(pstEnv, pstPlayer, &pstArmClt->ArmCltData.ArmUnFengYinPre);
			break;
		case ARM_UNFENGYIN:
			//if (check_npc_ui(pstEnv, pstPlayer, UI_TYPE_ARM_UNFENGYIN) < 0)
			//{
			//	return -1;
			//}
			arm_unfengyin(pstEnv, pstPlayer, &pstArmClt->ArmCltData.ArmUnFengYin);
			break;
		case ARM_FENJIE:
			arm_fenjie(pstEnv,pstPlayer,&pstArmClt->ArmCltData.ArmFenJie);
			break;
		case ARM_FENJIE_PRE:
			arm_fenjie_pre(pstEnv,pstPlayer,&pstArmClt->ArmCltData.ArmFenJie);
			break;
		case ARM_FENJIE_PRE_BRK:
			arm_fenjie_pre_brk(pstEnv, pstPlayer, pstArmClt->ArmCltData.ArmFenJiePreBrk);
			break;
		case GEM_ADD_POWER:
			 gem_add_power(pstEnv, pstPlayer, &pstArmClt->ArmCltData.GemAddPower);
			break;
		case GEM_RESET_ATTR:
			arm_reset_gem(pstEnv,pstPlayer,  &pstArmClt->ArmCltData.GemReSet);
			break;
		case GEM_RESET_ATTR_SAVE:
			arm_reset_gem_save(pstEnv, pstPlayer, &pstArmClt->ArmCltData.GemReSetSave);
			break;
		case ARM_RECAST:
			//if (check_npc_ui(pstEnv,pstPlayer,UI_TYPE_ARM_OPERATE) < 0
			//	&& check_npc_ui(pstEnv, pstPlayer, UI_TYPE_ARM_RECAST) < 0 
			//	&& check_npc_ui(pstEnv, pstPlayer, UI_TYPE_ARM_RECAST_ADVANCE) < 0)
			//{
			//	return -1;
			//}
			arm_recast(pstEnv,pstPlayer,&pstArmClt->ArmCltData.ArmRecast);
			break;
		case ARM_RECAST_SAVE:
			//if (check_npc_ui(pstEnv,pstPlayer,UI_TYPE_ARM_OPERATE) < 0
			//	&& check_npc_ui(pstEnv, pstPlayer, UI_TYPE_ARM_RECAST) < 0 
			//	&& check_npc_ui(pstEnv, pstPlayer, UI_TYPE_ARM_RECAST_ADVANCE) < 0)
			//{
			//	return -1;
			//}
			arm_recast_save(pstEnv,pstPlayer,&pstArmClt->ArmCltData.ArmRecastSave);
			break;
		case ARM_STAR_SAVE:		
			//if (check_npc_ui(pstEnv,pstPlayer,UI_TYPE_ARM_OPERATE) < 0
			//	&& check_npc_ui(pstEnv, pstPlayer, UI_TYPE_ARM_STAR) < 0)
			//{
			//	return -1;
			//}
			return arm_star_save(pstEnv, pstPlayer, &pstArmClt->ArmCltData.ArmStarSave);
			break;

		case SPAR_INLAY: 
			if (pstEnv->pstConf->IsMatchWorld)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET3);
				return 0;
			}
			spar_inlay(pstEnv, pstPlayer, &pstArmClt->ArmCltData.SparInlay);
			break;
		case SPAR_SORB:
			if(player_check_safelock(pstEnv, pstPlayer) < 0)
			{
				return -1;
			}
			spar_sorb(pstEnv, pstPlayer, &pstArmClt->ArmCltData.SparSorb);
			break;
		case ARM_WASH_ADDATTR:
			//if(check_npc_ui(pstEnv,pstPlayer,UI_TYPE_ARM_OPERATE) < 0
			//   && check_npc_ui(pstEnv,pstPlayer,UI_TYPE_ARM_WASH) < 0)
			//{
			//	return -1;
			//}
			return player_arm_wash(pstEnv, pstPlayer, &pstArmClt->ArmCltData.ArmWash);
			break;
		case ARM_WASH_ADDATTR_SAVE:
			//if(check_npc_ui(pstEnv,pstPlayer,UI_TYPE_ARM_OPERATE) < 0
			//   && check_npc_ui(pstEnv,pstPlayer,UI_TYPE_ARM_WASH) < 0)
			//{
			//	return -1;
			//}
			return player_arm_wash_save(pstEnv, pstPlayer, &pstArmClt->ArmCltData.ArmWashSave);
			break;
		case ARM_LEVEL_UP:
			return player_arm_level_up(pstEnv, pstPlayer, &pstArmClt->ArmCltData.LevelUP);
			break;
		case ARM_ADDATTR_MOVE:
			return player_arm_attr_move( pstEnv, pstPlayer, &pstArmClt->ArmCltData.AttrMove);
			break;
		case ARM_ATTR_RESET:
			return player_arm_attr_reset(pstEnv, pstPlayer, &pstArmClt->ArmCltData.AttrReSet);
			break;
		case ARM_ATTR_ACTIVE:
			return player_arm_attr_active(pstEnv, pstPlayer, &pstArmClt->ArmCltData.AttrActive);
			break;
		case ARM_ACT_CHECK:
			return player_arm_check( pstEnv, pstPlayer, &pstArmClt->ArmCltData.ArmCheck);
			break;
		case ARM_ACT_STEP_UP:
			return player_arm_step_up(pstEnv, pstPlayer, &pstArmClt->ArmCltData.StepUp);
			break;
		case ARM_BASE_ATTR_RESET:
			return player_arm_base_reset(pstEnv,pstPlayer, &pstArmClt->ArmCltData.BaseReSet);
			break;
		case ARM_BASE_ATTR_RESET_SAVE:
			return player_arm_base_reset_save(pstEnv, pstPlayer, &pstArmClt->ArmCltData.BaseReSetSave);
			break;
		case ARM_SKILL_ACTIVE:
			return player_arm_skill_active(pstEnv, pstPlayer, &pstArmClt->ArmCltData.SkillActive);
			break;
		case ARM_LEGEND_UP:
			return player_arm_legend_up(pstEnv, pstPlayer, &pstArmClt->ArmCltData.LegendUp);
			break;
		case ARM_BACK_UNREAL:
			return arm_back_unreal(pstEnv, pstPlayer, &pstArmClt->ArmCltData.BackUnreal);
			break;
		case ARM_SKILL_RECAST_LOCK:
			return arm_skill_lock( pstEnv, pstPlayer, &pstArmClt->ArmCltData.RecastLock);
			break;
		case ARM_SKILL_RECAST_ACTIVE:
			return arm_skill_recast_active(pstEnv, pstPlayer, &pstArmClt->ArmCltData.RecastActive);
			break;
		case ARM_SKILL_RECAST_RESET:
			return arm_skill_recast_reset(pstEnv, pstPlayer,&pstArmClt->ArmCltData.RecastReset);
			break;
		default:
			return -1;
			break;
	}

	return 0;
	UNUSED(pstAppCtx);
}

//装备加工UI
int open_arm_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID, int iUIType)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	UNUSED( pstAppCtx );
	
	pstActionRes->ActionID = ACTION_UI;
	pstActionRes->ID = iNpcID;
	pstActionRes->Data.UI.UIType = iUIType;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int is_fengyin_arm(ROLEARM *pstRoleArm)
{
	if (pstRoleArm->FengyinFlag > 0)
	{
		return 1;
	}
	return 0;
}


// 身上是否有装备被封印
int wear_has_one_fengyin(Player *pstPlayer)
{
	ROLEWEAR *pstWear = &pstPlayer->stRoleData.Wear;
	int i;

	for (i=0; i<pstWear->ArmNum; i++)
	{
		if (is_fengyin_arm(&pstWear->RoleArms[i]))
		{
			return 1;
		}
	}
	return 0;
}

int arm_fengyin_in1(ROLEARM *pstRoleArm)
{
	int iCount = pstRoleArm->AddAttrNum + pstRoleArm->WashAttrNum;
	int iFyNum = 0;
	int iIdx = RAND1(iCount);	// 第几条开始封印
	int i;

	if (iCount == 0)
	{
		return -1;
	}

	iFyNum = iCount/2 + iCount%2;	// 封印条数向上取整
	for (i=0; i<iFyNum; i++)
	{
		pstRoleArm->FengyinFlag |= (int)pow(2.0,iIdx); 
		iIdx = (iIdx+1)%iCount;
	}
	return 0;
}


// iidx--第几条  ,iFlag--1表示基础关键字
int arm_attr_is_fengyin(ROLEARM *pstRoleArm, int iIdx, int iFlag)
{

	if (!is_fengyin_arm(pstRoleArm))
	{
		return 0;
	}
	
	if (iFlag == 1)
	{
		if (iIdx >= pstRoleArm->AddAttrNum)
		{
			return 0;
		}

		if (pstRoleArm->FengyinFlag & (int)pow(2.0,iIdx))
		{
			return 1;
		}
	}
	else
	{
		if (iIdx >= pstRoleArm->WashAttrNum)
		{
			return 0;
		}

		if (pstRoleArm->FengyinFlag & (int)pow(2.0,iIdx+pstRoleArm->AddAttrNum))
		{
			return 1;
		}
	}
	return 0; 
}



int arm_fengyin_in(ZONESVRENV* pstEnv, Player *pstPlayer, int iIdx,Player *pstKiller,int iListType)
{
	/*
		INST_ITEM_FENGYIN 才是封印唯一标识
		怪物杀死没有FengyinID
	*/
	ROLEGRID *pstGrid=NULL;
	ROLEARM *pstArm=NULL;
	char szMsg[256];
	ITEMDEF *pstItemDef;
	// 视野内玩家通知一下
	CSPKG stPkg;
	CSSYSMSG *pstSysMsg ;
	int iPos;
	ROLEPACKAGE *pstPkg = &pstPlayer->stRoleData.Package;
	ROLEWEAR *pstWear = &pstPlayer->stRoleData.Wear;
	int i;

	if (iListType == LIST_TYPE_WEAR)
	{
		for(i=0; i<pstWear->ArmNum; i++)
		{
			if (pstWear->RoleArms[i].GridIdx == iIdx)
			{
				pstArm = &pstWear->RoleArms[i];
				break;
			}
		}
		
		if (i>=pstWear->ArmNum)
		{
			return -1;
		}
	}
	else if(iListType == LIST_TYPE_SUNDRIES)
	{
		iPos = package_grid_pos(pstPkg->SundriesPackage.RoleGrids, pstPkg->SundriesPackage.Num, iIdx);
		if (0 > iPos)
		{
			return -1;
		}
		pstGrid = &pstPlayer->stRoleData.Package.SundriesPackage.RoleGrids[iPos];
		if (pstGrid->Type == GRID_ITEM_ITEM)
			return -1;
		pstArm = &pstGrid->GridData.RoleArm;
	}
	else
	{
		return -1;
	}

	if(!pstArm)
	{
		return -1;
	}
	pstItemDef = z_find_itemdef(pstEnv, pstArm->DefID);
	if (!pstItemDef)
		return -1;

	/*if (pstArm->FengYinID > 0 ||is_fengyin_arm(pstArm)) 
		return -1;*/

	if (is_fengyin_arm(pstArm))
	{
		return -1;
	}

	//lzk
	if(arm_is_badge(pstItemDef) < 0 )
	{
		return -1;
	}

	if (pstKiller)
	{
		pstArm->FengYinID = pstKiller->stRoleData.RoleID;
	}

	if (arm_fengyin_in1(pstArm) < 0)
	{
		return -1;
	}
	//pstArm->InstFlag |= INST_ITEM_FENGYIN;

	if (iListType == LIST_TYPE_WEAR)
	{
		 arm_wear_update(pstEnv,  pstPlayer, pstArm);
		 // 更新数值
		 z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer); 
	}
	else
	{
		if (pstGrid)
			arm_one_grid_update(pstEnv,  pstPlayer, pstGrid);
	}

	
	if (pstKiller)
	{
		snprintf(szMsg,sizeof(szMsg),LJY_SYSMSG1,
				pstKiller->stRoleData.RoleName, pstPlayer->stRoleData.RoleName,pstItemDef->Name);	
	}
	else
	{
		snprintf(szMsg,sizeof(szMsg), LJY_SYSMSG2,pstPlayer->stRoleData.RoleName,pstItemDef->Name);
	}

	if (pstKiller)
	{
		z_role_fengyin_arm_oplog(pstEnv,pstPlayer,pstKiller,0,0,pstArm);
	}
	else
	{
		z_role_fengyin_arm_oplog(pstEnv,pstPlayer,NULL,0,0,pstArm);
	}

	// 视野内玩家通知一下
	pstSysMsg = &stPkg.Body.SysMsg;

	pstSysMsg->Type = SYS_MIDDLE2;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
	STRNCPY(pstSysMsg->Msg, szMsg, sizeof(szMsg));

	Z_CSHEAD_INIT(&stPkg.Head, SYS_MSG);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 1);
	return 0;
}


int spar_hole_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	SPARPKG *pstSparPkg = &pstPlayer->stRoleData.Package.SparPkg;
	int i;
	LEVELUP *pstLevelUp;
	int iLevel = pstPlayer->stRoleData.Level;
	int iHole = 0;

	for (i=1; i<=iLevel; i++)
	{
		pstLevelUp = z_get_levelup(pstEnv, i);
		if (!pstLevelUp)
		{
			return 0;
		}

		if (	pstLevelUp->AddSparPkg )
		{
			iHole ++;
		}
	}

	if (iHole > pstSparPkg->MaxNum)
	{
		pstSparPkg->MaxNum = iHole;
	}

	return 0;
}

// INST_ITEM_UNCHECK是判断装备有没有鉴定的唯一标识
int arm_is_uncheck(ROLEARM *pstArm)
{
	return pstArm->InstFlag&INST_ITEM_UNCHECK;
}

int arm_is_badge(ITEMDEF *pstItemDef)
{
	if(WEAR_POS_BADGE == pstItemDef->WearPos1)
	{
		return -1;
	}
	
	return 0;
}

int arm_skill_fast_insert(ZONESVRENV* pstEnv, ARMSKILLATTRDEF *pstAttrDef, int iLvl,int iWpos, int iCareer, int iImp)
{
	ArmSkillAttrFast *pstFast = &pstEnv->pstZoneObj->stArmSkillFast;
	OneArmAttrList *pstList;

	pstList = &pstFast->astRandList[iLvl][iWpos][iCareer][iImp];

	if (pstList->iNum >= MAX_ONE_ARMATTR_LIST)
	{
		printf("arm_skill_fast_insert fail !\n");
		return -1;
	}

	pstList->iDefID[pstList->iNum++] = pstAttrDef->ID;
	return 0;
}

int arm_skill_fast_imp(ZONESVRENV* pstEnv, ARMSKILLATTRDEF *pstAttrDef, int iLvl,int iWpos, int iCareer)
{
	int i;

	for (i=9; i>=pstAttrDef->ImpLimit; i--)
	{
		arm_skill_fast_insert(pstEnv, pstAttrDef, iLvl,iWpos,iCareer, i);	
	}
	
	/*if (pstAttrDef->ImpLimit >= 0)
	{
		arm_skill_fast_insert(pstEnv, pstAttrDef, iLvl,iWpos,iCareer, 0);
	}

	if (pstAttrDef->ImpLimit >= 1)
	{
		arm_skill_fast_insert(pstEnv, pstAttrDef, iLvl,iWpos,iCareer, 1);
	}

	if (pstAttrDef->ImpLimit >= 2)
	{
		arm_skill_fast_insert(pstEnv, pstAttrDef, iLvl,iWpos,iCareer, 2);
	}

	if (pstAttrDef->ImpLimit >= 3)
	{
		arm_skill_fast_insert(pstEnv, pstAttrDef, iLvl,iWpos,iCareer, 3);
	}

	if (pstAttrDef->ImpLimit >= 4)
	{
		arm_skill_fast_insert(pstEnv, pstAttrDef, iLvl,iWpos,iCareer, 4);
	}

	if (pstAttrDef->ImpLimit >= 5)
	{
		arm_skill_fast_insert(pstEnv, pstAttrDef, iLvl,iWpos,iCareer, 5);
	}

	if (pstAttrDef->ImpLimit >= 6)
	{
		arm_skill_fast_insert(pstEnv, pstAttrDef, iLvl,iWpos,iCareer, 6);
	}

	if (pstAttrDef->ImpLimit >= 7)
	{
		arm_skill_fast_insert(pstEnv, pstAttrDef, iLvl,iWpos,iCareer, 7);
	}

	if (pstAttrDef->ImpLimit >= 8)
	{
		arm_skill_fast_insert(pstEnv, pstAttrDef, iLvl,iWpos,iCareer, 8);
	}

	if (pstAttrDef->ImpLimit >= 9)
	{
		arm_skill_fast_insert(pstEnv, pstAttrDef, iLvl,iWpos,iCareer, 9);
	}*/
	
	return 0;
}

int arm_skill_fast_career(ZONESVRENV* pstEnv, ARMSKILLATTRDEF *pstAttrDef, int iLvl,int iWpos)
{
	if (pstAttrDef->CareerLimit&ATTR_MAIN_JISI)
	{
		arm_skill_fast_imp(pstEnv, pstAttrDef, iLvl,iWpos, ADD_ATTR_CAREER_CHURCH-1);
	}
	if (pstAttrDef->CareerLimit&ATTR_MAIN_ZHANSHI)
	{
		arm_skill_fast_imp(pstEnv, pstAttrDef, iLvl,iWpos, ADD_ATTR_CAREER_WARRIOR-1);
	}
	if (pstAttrDef->CareerLimit&ATTR_MAIN_FASHI)
	{
		arm_skill_fast_imp(pstEnv, pstAttrDef, iLvl,iWpos, ADD_ATTR_CAREER_MAGICOR-1);
	}
	if (pstAttrDef->CareerLimit&ATTR_MAIN_QIANGSHOU)
	{
		arm_skill_fast_imp(pstEnv, pstAttrDef, iLvl,iWpos, ADD_ATTR_CAREER_GUNER-1);
	}
	if (pstAttrDef->CareerLimit&ATTR_MAIN_DANAI)
	{
		arm_skill_fast_imp(pstEnv, pstAttrDef, iLvl,iWpos, ADD_ATTR_CAREER_SINGLEHAND_CHURCH-1);
	}
	if (pstAttrDef->CareerLimit&ATTR_MAIN_ERNAI)
	{
		arm_skill_fast_imp(pstEnv, pstAttrDef, iLvl,iWpos, ADD_ATTR_CAREER_DOUBLEHAND_CHURCH-1);
	}
	if (pstAttrDef->CareerLimit&ATTR_MAIN_HUIMIE)
	{
		arm_skill_fast_imp(pstEnv, pstAttrDef, iLvl,iWpos, ADD_ATTR_CAREER_DOUBLEHAND_GUNER-1);
	}
	if (pstAttrDef->CareerLimit&ATTR_MAIN_LIESHA)
	{
		arm_skill_fast_imp(pstEnv, pstAttrDef, iLvl,iWpos, ADD_ATTR_CAREER_SINGLEHAND_GUNER-1);
	}
	if (pstAttrDef->CareerLimit&ATTR_MAIN_JUNENG)
	{
		arm_skill_fast_imp(pstEnv, pstAttrDef, iLvl,iWpos, ADD_ATTR_CAREER_SINGLEHAND_MAGICOR-1);
	}
	if (pstAttrDef->CareerLimit&ATTR_MAIN_LIANHUN)
	{
		arm_skill_fast_imp(pstEnv, pstAttrDef, iLvl,iWpos, ADD_ATTR_CAREER_DOUBLEHAND_MAGICOR-1);
	}
	if (pstAttrDef->CareerLimit&ATTR_MAIN_SHOUHU)
	{
		arm_skill_fast_imp(pstEnv, pstAttrDef, iLvl,iWpos, ADD_ATTR_CAREER_SINGLEHAND_WARRIOR-1);
	}
	if (pstAttrDef->CareerLimit&ATTR_MAIN_ZHENGFU)
	{
		arm_skill_fast_imp(pstEnv, pstAttrDef, iLvl,iWpos, ADD_ATTR_CAREER_DOUBLEHAND_WARRIOR-1);
	}
	return 0;
}

int arm_skill_fast_wpos(ZONESVRENV* pstEnv, ARMSKILLATTRDEF *pstAttrDef, int iLvl)
{
	if (pstAttrDef->WposLimit&ATTR_RIGHTHAND)
	{
		arm_skill_fast_career(pstEnv,pstAttrDef, iLvl,ADD_ATTR_WEARPOS_TYPE_RIGHTHAND);
	}
	if (pstAttrDef->WposLimit&ATTR_HEAD)
	{
		arm_skill_fast_career(pstEnv,pstAttrDef, iLvl,ADD_ATTR_WEARPOS_TYPE_HEAD);
	}
	if (pstAttrDef->WposLimit&ATTR_BODY)
	{
		arm_skill_fast_career(pstEnv,pstAttrDef, iLvl,ADD_ATTR_WEARPOS_TYPE_BODY);
	}
	if (pstAttrDef->WposLimit&ATTR_LEFTHAND)
	{
		arm_skill_fast_career(pstEnv,pstAttrDef, iLvl,ADD_ATTR_WEARPOS_TYPE_LEFTHAND);
	}
	if (pstAttrDef->WposLimit&ATTR_FACE)
	{
		arm_skill_fast_career(pstEnv,pstAttrDef, iLvl,ADD_ATTR_WEARPOS_TYPE_FACE);
	}
	if (pstAttrDef->WposLimit&ATTR_SHOULDER)
	{
		arm_skill_fast_career(pstEnv,pstAttrDef, iLvl,ADD_ATTR_WEARPOS_TYPE_SHOULDER);
	}
	if (pstAttrDef->WposLimit&ATTR_SHOE)
	{
		arm_skill_fast_career(pstEnv,pstAttrDef, iLvl,ADD_ATTR_WEARPOS_TYPE_SHOE);
	}
	if (pstAttrDef->WposLimit&ATTR_AMULET)
	{
		arm_skill_fast_career(pstEnv,pstAttrDef, iLvl,ADD_ATTR_WEARPOS_TYPE_AMULET);
	}
	if (pstAttrDef->WposLimit&ATTR_RING)
	{
		arm_skill_fast_career(pstEnv,pstAttrDef, iLvl,ADD_ATTR_WEARPOS_TYPE_RING);
	}
	if (pstAttrDef->WposLimit&ATTR_NECKLACE)
	{
		arm_skill_fast_career(pstEnv,pstAttrDef, iLvl,ADD_ATTR_WEARPOS_TYPE_NECKLACE);
	}
	if (pstAttrDef->WposLimit&ATTR_BACK)
	{
		arm_skill_fast_career(pstEnv,pstAttrDef, iLvl,ADD_ATTR_WEARPOS_TYPE_BACK);
	}
	if (pstAttrDef->WposLimit&ATTR_BADGE)
	{
		arm_skill_fast_career(pstEnv,pstAttrDef, iLvl,ADD_ATTR_WEARPOS_TYPE_BADGE);
	}
	return 0;
}


int arm_skill_career_get(ARMSKILLATTRDEF *pstAttrDef, int iCareer)
{
	switch(iCareer)
	{
		case CAREER_NON:
			return 0;
			break;
		case CAREER_WARRIOR:
			return pstAttrDef->CareerLimit&ATTR_MAIN_ZHANSHI;
			break;
		case CAREER_MAGICOR:
			return pstAttrDef->CareerLimit&ATTR_MAIN_FASHI;
			break;
		case CAREER_GUNER:
			return pstAttrDef->CareerLimit&ATTR_MAIN_QIANGSHOU;
			break;
		case CAREER_CHURCH:
			return pstAttrDef->CareerLimit&ATTR_MAIN_JISI;
			break;
		case CAREER_WARRIOR_SINGLEHAND:
			return pstAttrDef->CareerLimit&ATTR_MAIN_SHOUHU;
			break;
		case CAREER_WARRIOR_DOUBLEHAND:
			return pstAttrDef->CareerLimit&ATTR_MAIN_ZHENGFU;
			break;
		case CAREER_MAGICOR_SINGLEHAND:
			return pstAttrDef->CareerLimit&ATTR_MAIN_JUNENG;
			break;
		case CAREER_MAGICOR_DOUBLEHAND:
			return pstAttrDef->CareerLimit&ATTR_MAIN_LIANHUN;
			break;
		case CAREER_GUNER_SINGLEHAND:
			return pstAttrDef->CareerLimit&ATTR_MAIN_LIESHA;
			break;
		case CAREER_GUNER_DOUBLEHAND:
			return pstAttrDef->CareerLimit&ATTR_MAIN_HUIMIE;
			break;
		case CAREER_CHURCH_SINGLEHAND:
			return pstAttrDef->CareerLimit&ATTR_MAIN_DANAI;
			break;
		case CAREER_CHURCH_DOUBLEHAND:
			return pstAttrDef->CareerLimit&ATTR_MAIN_ERNAI;
			break;
		default:
			return 0;
			break;
	}
	return 0;
	
}

int arm_skill_fast_lvl(ZONESVRENV* pstEnv, ARMSKILLATTRDEF *pstAttrDef, int iLvl)
{
	arm_skill_fast_wpos(pstEnv,pstAttrDef,iLvl);
	return 0;	
}

int arm_skill_fast_init(ZONESVRENV* pstEnv)
{
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	int i;
	ARMSKILLATTRDEF *pstAttrDef;

	memset(&pstEnv->pstZoneObj->stArmSkillFast, 0, sizeof(pstEnv->pstZoneObj->stArmSkillFast));
	for (i=0; i<pstObj->iArmSkillAttrDef; i++)
	{
		pstAttrDef = (ARMSKILLATTRDEF *)pstObj->sArmSkillAttrDef[i];
		if (pstAttrDef->Lvl < 1 || pstAttrDef->Lvl > 3)
		{	
			printf("arm_skill_fast_init err 1!\n");
			return -1;
		}

		// arm实例中为了省点内存记录了一个ushort型的ID
		if (pstAttrDef->ID > 0xffff)
		{
			printf("arm_skill_fast_init err 2!\n");
			return -1;
		}

		if (pstAttrDef->BaseVal <= 0)
		{
			printf("arm_skill_fast_init err 3!\n");
			return -1;
		}

		if (arm_skill_fast_lvl(pstEnv, pstAttrDef, pstAttrDef->Lvl -1) < 0)
		{
			return -1;
		}
	}
	return 0;
}

int arm_create_wpos_idx(ITEMDEF *pstItemDef)
{
	return  add_attr_wearpos_idx_get(pstItemDef);	
}

int arm_create_career_idx(ITEMDEF *pstItemDef)
{
	return add_attr_career_idx_get(pstItemDef)-1;
}

// 剔除等级不符的
int arm_create_del_skill_attr(ZONESVRENV* pstEnv, OneArmAttrList *pstList, ITEMDEF *pstItemDef)
{
	int i;
	ARMSKILLATTRDEF *pstAttrDef;
		
	for (i=pstList->iNum-1; i>=0; i--)
	{
		pstAttrDef = 	z_find_arm_skillattr_def(pstEnv, pstList->iDefID[i]);
		if (!pstAttrDef)
		{
			return -1;
		}

		if (	pstAttrDef->MinLvlLimit == 0 &&
			pstAttrDef->MaxLvlLimit == 0)
		{
			continue;
		}
		
		if (	pstItemDef->WearPre.RoleLevelMin < pstAttrDef->MinLvlLimit ||
			pstItemDef->WearPre.RoleLevelMin > pstAttrDef->MaxLvlLimit)
		{
			pstList->iNum --;
			if (i != pstList->iNum)
			{
				pstList->iDefID[i] = pstList->iDefID[pstList->iNum];
			}
		}
	}

	return 0;
}

int arm_skill_wpos_get_mul(ARMSKILLATTRDEF *psAttrDef, ITEMDEF *pstItemDef)
{
	switch(pstItemDef->WearPos1)
	{
		case WEAR_POS_RIGHTHAND:
			return psAttrDef->WposMdf.RightHand;
			break;
		case WEAR_POS_HEAD:
			return psAttrDef->WposMdf.Head;
			break;
		case WEAR_POS_BODY:
			return psAttrDef->WposMdf.Body;
			break;
		case WEAR_POS_LEFT:
			return psAttrDef->WposMdf.LeftHand;
			break;
		case WEAR_POS_FACE:
			return psAttrDef->WposMdf.Face;
			break;
		case WEAR_POS_SHOULDER:
			return psAttrDef->WposMdf.Shoulder;
			break;
		case WEAR_POS_SHOE:
			return psAttrDef->WposMdf.Shoe;
			break;
		case WEAR_POS_AMULET:
			return psAttrDef->WposMdf.Amulet;
			break;
		case WEAR_POS_RING:
			return psAttrDef->WposMdf.Ring;
			break;
		case WEAR_POS_NECKLACE:
			return psAttrDef->WposMdf.NeckLace;
			break;
		case WEAR_POS_FASHION_BACK:
			return psAttrDef->WposMdf.Back;
			break;
		case WEAR_POS_BADGE:
			return psAttrDef->WposMdf.Badge;
			break;
		default:
			return 0;
			break;
	}
	
	return 0;
}

int arm_create_skill_num(ZONESVRENV* pstEnv,ROLEARM *pstArm, ITEMDEF *pstItemDef)
{
	ARMSKILLNUMDEF *pstDef;
	int iRate;
	int iTmp=0;
	int i;
	int iNum = 0;
	ARMSKILLDATA *pstData;
	ArmSkillAttrFast *pstFast = &pstEnv->pstZoneObj->stArmSkillFast;
	OneArmAttrList *pstList;
	OneArmAttrList stList;

	
	pstDef = z_find_arm_skillnum_def(pstEnv, pstItemDef->WearPre.RoleLevelMin/10,
									pstItemDef->WearPos1,pstItemDef->Important);
	if (!pstDef)
	{
		return 0;
	}

	if (pstItemDef->SkillNum > 0)
	{
		iNum = pstItemDef->SkillNum;
	}
	else
	{
		iRate = RAND1(10000);

		for (i=0; i<MAX_ARM_SKILL_NUM; i++)
		{
			iTmp += pstDef->Active[i].Rate;
			if (iRate < iTmp)
			{
				iNum = i+1;
				break;
			}
		}
	}

	// 没有装备技能
	if (iNum == 0)
	{
		return 0;
	}

	pstData = &pstArm->ArmSkill;

	int iWposIdx = arm_create_wpos_idx(pstItemDef);
	if (iWposIdx < 0)
	{
		return 0;
	}

	int iImpIdx = pstItemDef->Important;
	if (iImpIdx < 0 || iImpIdx > 9)
	{
		return 0;
	}

	int iCareerIdx = arm_create_career_idx(pstItemDef);
	if (iCareerIdx < 0 || iCareerIdx > ADD_ATTR_CAREER_DOUBLEHAND_CHURCH -1)
	{
		return 0;
	}

	
	for (i=0; i<iNum; i++)
	{
		pstList = &pstFast->astRandList[i][iWposIdx][iCareerIdx][iImpIdx];		
		stList = *pstList;
		if (arm_create_del_skill_attr(pstEnv, &stList, pstItemDef) < 0)
		{
			return 0;
		}

		if (stList.iNum < 1)
		{
			return 0;
		}
		
		pstData->One[i].ID = stList.iDefID[RAND1(stList.iNum)];
		pstData->One[i].Idx = RAND1(MAX_IMP_GRADE);  	// 随机修正
		pstData->Max ++;
	}

	// 全部激活
	pstData->Num = pstData->Max;
	
	return 0;
}

//宝石合成保险金数额
int gem_level_up_baoxian_val(short ResultVal1,int *piMoney)
{	
	switch(ResultVal1)
	{
		case 1:
			*piMoney = ONE_GEM_HECHENG_GODCOIN;				
			break;
		case 2:
			*piMoney = TWO_GEM_HECHENG_GODCOIN;
			break;
		case 3:
			*piMoney = THREE_GEM_HECHENG_GODCOIN;
			break;
		case 4:
			*piMoney = FOUR_GEM_HECHENG_GODCOIN;
			break;
		case 5:
			*piMoney = FIVE_GEM_HECHENG_GODCOIN;
			break;
		case 6:
			*piMoney = SIX_GEM_HECHENG_GODCOIN;
			break;
		case 7:
			*piMoney = SEVEN_GEM_HECHENG_GODCOIN;
			break;
		case 8:
			*piMoney = EIGHT_GEM_HECHENG_GODCOIN;
			break;
		default:
			return -1;
			break;
	}
	
	return 0;
}


int arm_calc_teshu_attr(ZONESVRENV* pstEnv,Player *pstPlayer, ROLEARM *pstRoleArm,ITEMDEF *pstItemDef)
{
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	int i;
	ARMTESHUDEF *pstDef;

	if (pstItemDef->TeshuID <= 0)
	{
		return 0;
	}

	pstDef = z_find_arm_teshu_def(pstEnv, pstItemDef->TeshuID);
	if (!pstDef ||pstDef->Rate != 0)
	{
		return 0;
	}

	for (i=0; i<MAX_TESHU_RESULT; i++)
	{
		if (pstDef->Result[i].ResultID <= 0)
		{
			break;
		}

		z_attr_result(pstEnv, pstDyn, pstRoleArm, &pstDef->Result[i]);
	}

	if( pstPlayer->stOnline.stDyn.stHealChg.nSpeedMul > 0 ||
		pstPlayer->stOnline.stDyn.stHealChg.nSpeedAdd > 0	)
	{	
		// 有对速度的修正要下发给用户
		z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}
	
	return 0;
}

int arm_calc_teshu_buff(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	ROLEWEAR *pstWear =  &pstPlayer->stRoleData.Wear;
	ITEMDEF *pstItemDef;
	int i;
	ROLEARM *pstRoleArm;
	ARMTESHUDEF *pstDef;
	int j;

	
	for(i=0; i<pstWear->ArmNum; i++)
	{
		pstRoleArm = pstWear->RoleArms+i;
		
		if (	pstRoleArm->ValidEnd > 0 &&
			pstRoleArm->ValidEnd < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			continue;
		}

		if (is_fengyin_arm(pstRoleArm))
		{
			continue;
		}

		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleArm);
		if (!pstItemDef || pstItemDef->TeshuID <= 0)
		{
			continue;
		}

		pstDef = z_find_arm_teshu_def(pstEnv, pstItemDef->TeshuID);
		if (!pstDef ||pstDef->Rate == 0)
		{
			continue;
		}

		if (RAND1(10000) > pstDef->Rate)
		{
			continue;
		}

		for (j=0; j<MAX_TESHU_RESULT; j++)
		{
			if (pstDef->Result[j].ResultVal1 <= 0)
			{
				break;
			}

			z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstDef->Result[j].ResultVal1,
									pstDef->Result[j].ResultVal2,pstDef->Result[j].ResultVal3, 1, 0);
		}
	}
	return 0;
}

//背饰幻化
int arm_back_unreal(ZONESVRENV* pstEnv, Player *pstPlayer, ARMBACKUNREAL *pstCSBackUnreal)
{
	ROLEGRID *pstSrc;
	ROLEGRID *pstDsc;
	int iVaild;
	ITEMDEF *pstDscItemDef;
	ITEMDEF *pstSrcItemDef;
	unsigned int uiMoney = 0;
	int iSucc = 1;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;

	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		return -1;
	}

	if( pstCSBackUnreal->Src.ArmListType == LIST_TYPE_WEAR ||
		pstCSBackUnreal->Dsc.ArmListType == LIST_TYPE_WEAR )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM_UNREAL5);
		iSucc  = 0;
		goto _error;
	}

	iVaild = get_vaild_gird(pstEnv, &pstPlayer->stRoleData.Package, LIST_TYPE_SUNDRIES);
	if (pstCSBackUnreal->Src.ArmGridIdx >= iVaild || pstCSBackUnreal->Dsc.ArmGridIdx >= iVaild )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM_UNREAL5);
		iSucc  = 0;
		goto _error;
	}

	pstSrc =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstCSBackUnreal->Src.ArmGridIdx);
	if (!pstSrc || pstSrc->Type != GRID_ITEM_ARM)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM_UNREAL5);
		iSucc  = 0;
		goto _error;
	}

	pstDsc =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstCSBackUnreal->Dsc.ArmGridIdx);
	if (!pstDsc || pstDsc->Type != GRID_ITEM_ARM)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM_UNREAL5);
		iSucc  = 0;
		goto _error;
	}

	pstSrcItemDef = z_get_itemdef_roleitem(pstEnv, &pstSrc->GridData.RoleItem);
	pstDscItemDef = z_get_itemdef_roleitem(pstEnv, &pstDsc->GridData.RoleItem);
	if (!pstSrcItemDef || !pstDscItemDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM_UNREAL7);
		iSucc  = 0;
		goto _error;
	}

	if (!arm_is_back(pstSrcItemDef) || !arm_is_back(pstDscItemDef))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM_UNREAL6);
		iSucc  = 0;
		goto _error;
	}

	//过期
	if(pstSrc->GridData.RoleArm.ValidEnd > 0 && pstSrc->GridData.RoleArm.ValidEnd <= pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM_UNREAL4);
		iSucc  = 0;
		goto _error;
	}

	if(pstDsc->GridData.RoleArm.ValidEnd > 0 && pstDsc->GridData.RoleArm.ValidEnd <= pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM_UNREAL4);
		iSucc  = 0;
		goto _error;
	}

	if(pstSrcItemDef->CtrlFlag & ITEM_CAN_STEP_UP_BACK)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM_UNREAL1);
		iSucc  = 0;
		goto _error;
	}

	if(!(pstDscItemDef->CtrlFlag & ITEM_CAN_STEP_UP_BACK))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM_UNREAL2);
		iSucc  = 0;
		goto _error;
	}

	uiMoney =  ceil( pstPlayer->stRoleData.Level * pstPlayer->stRoleData.Level / 100.0) * 100;
	if (0 > package_dec_money_test( pstEnv, pstPlayer, ATTR_ID_MONEY, uiMoney))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM116, uiMoney, MONEY_TYPE_MONEY_NAME);
		iSucc  = 0;
		goto _error;
	}

	if(uiMoney > 0)
	{
		package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, uiMoney);
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_BACK_UNREAL, pstDscItemDef->ItemID, 0, uiMoney, "背饰幻化");
	}

	pstSrc->GridData.RoleArm.InstFlag |= INST_ITEM_BIND;
	pstDsc->GridData.RoleArm.InstFlag |= INST_ITEM_BIND;

	pstDsc->GridData.RoleArm.UnrealBackID = pstSrcItemDef->ItemID;
	pstDsc->GridData.RoleArm.UnrealBackTmEnd = pstSrc->GridData.RoleArm.ValidEnd;

	iSucc  = 1;

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM_UNREAL3);
_error:
	if (iSucc)
	{
		// 通知客户端装备发生变化
		arm_one_grid_update(pstEnv,  pstPlayer, pstSrc);
		arm_one_grid_update(pstEnv,  pstPlayer, pstDsc);
	}
	memset(pstArmSvr,0,sizeof(*pstArmSvr));
	pstArmSvr->ArmSvrType = ARM_BACK_UNREAL;
	pstArmSvr->Flag = iSucc;
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int back_unreal_result_check(ZONESVRENV* pstEnv, Player *pstPlayer,ROLEARM *pstRoleArm)
{
	if (pstRoleArm->UnrealBackTmEnd > 0 &&
			pstRoleArm->UnrealBackTmEnd < pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		pstRoleArm->UnrealBackID = 0;
		pstRoleArm->UnrealBackTmEnd = 0;
		return 1;
	}
	
	return 0;
}

