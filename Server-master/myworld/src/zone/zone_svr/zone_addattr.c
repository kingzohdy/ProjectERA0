/*
**  @file $RCSfile: zone_addattr.c,v $
**  general description of this module
**  $Id: zone_addattr.c,v 1.63 2013/12/20 07:20:33 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2013/12/20 07:20:33 $
**  @version $Revision: 1.63 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_clt.h"
#include "zone_attr.h"
#include "zone_package.h"
#include "zone_svr.h"
#include "zone_arm.h"
#include "zone_safelock.h"
#include "zone_oplog.h"
#include "zone_player.h"
#include "zone_task.h"
#include "zone_combat.h"
#include "zone_skill.h"
#include "zone_genius.h"
#include "zone_shop.h"
#include "zone_err.h"

NEWADDATTRDEF* z_find_new_addattr_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	NEWADDATTRDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.ID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sNewAddAttrDef, pstObj->iNewAddAttrDef, NEW_ADD_ATTR_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (NEWADDATTRDEF*)pstObj->sNewAddAttrDef[iIndex];
		
	}
	return NULL;
}

ADDATTRRANDDEF* z_find_addattr_rand_def(ZONESVRENV *pstEnv, int iArmLevel, int iImportant)
{
	int iIndex;
	int iEqu;
	ADDATTRRANDDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.ArmLevel = iArmLevel;
	stDef.Important = iImportant;
	iIndex = bsearch_int(&stDef, pstObj->sAddAttrRandDef, pstObj->iAddAttrRandDef, ADD_ATTR_RAND_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ADDATTRRANDDEF*)pstObj->sAddAttrRandDef[iIndex];
		
	}
	return NULL;
}

ADDATTRWASHDEF* z_find_addattr_wash_def(ZONESVRENV *pstEnv, int iNum)
{
	int iIndex;
	int iEqu;
	ADDATTRWASHDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.AddAttrNum = iNum;
	iIndex = bsearch_int(&stDef, pstObj->sAddAttrWashDef, pstObj->iAddAttrWashDef, ADD_ATTR_WASH_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ADDATTRWASHDEF*)pstObj->sAddAttrWashDef[iIndex];
		
	}
	return NULL;
}

//按穿戴位置、职业、等级、颜色分类记录便于查找
int add_attr_assort_insert(ZONESVRENV *pstEnv, NEWADDATTRDEF *pstDef, int iWearPos, int iCareer, int iFlag, int iLevel, int iImportant)
{
	AddAttrFastList *pstFastList = pstEnv->pstAddAttrFastList;
	OneAddAttrList *pstOneAddAttrList;

	if(iWearPos < 0 ||
		iWearPos >= MAX_ADD_ATTR_WEARPOS_LIST)
	{
		return -1;
	}

	if(iCareer < 0 ||
		iCareer >= MAX_ADD_ATTR_CAREER)
	{
		return -1;
	}

	if(iLevel < 0 ||
		iLevel >= MAX_ADD_ATTR_LEVEL_LIST)
	{
		return -1;
	}

	if(iImportant < 0 ||
		iImportant >= MAX_ADD_ATTR_IMPORTANT)
	{
		return -1;
	}
	
	if(iFlag == ADD_ATTR_CAREER_FLAG_RAND ||
		iFlag == ADD_ATTR_CAREER_FLAG_ALL)
	{
		pstOneAddAttrList = &pstFastList->astRandList[iWearPos][iCareer][iLevel][iImportant];
		if(pstOneAddAttrList->iAddAttrNum < MAX_ONE_ADDATTR_LIST-1)
		{
			pstOneAddAttrList->iDefID[pstOneAddAttrList->iAddAttrNum++] = pstDef->ID;
		}
	}

	if(iFlag == ADD_ATTR_CAREER_FLAG_WASH ||
		iFlag == ADD_ATTR_CAREER_FLAG_ALL)
	{
		pstOneAddAttrList = &pstFastList->astWashList[iWearPos][iCareer][iLevel][iImportant];
		if(pstOneAddAttrList->iAddAttrNum < MAX_ONE_ADDATTR_LIST-1)
		{
			pstOneAddAttrList->iDefID[pstOneAddAttrList->iAddAttrNum++] = pstDef->ID;
		}
	}

	return 0;
}

//穿戴位置、职业、等级之后再次颜色分类
int add_attr_assort_import(ZONESVRENV *pstEnv, NEWADDATTRDEF *pstDef, int iWearPos, int iCareer, int iFlag, int iLevel)
{
	if(pstDef->Important == 0)
	{
		add_attr_assort_insert(pstEnv, pstDef, iWearPos, iCareer, iFlag, iLevel, 0);
	}

	if(pstDef->Important <= 1)
	{
		add_attr_assort_insert(pstEnv, pstDef, iWearPos, iCareer, iFlag, iLevel, 1);
	}

	if(pstDef->Important <= 2)
	{
		add_attr_assort_insert(pstEnv, pstDef, iWearPos, iCareer, iFlag, iLevel, 2);
	}

	if(pstDef->Important <= 3)
	{
		add_attr_assort_insert(pstEnv, pstDef, iWearPos, iCareer, iFlag, iLevel, 3);
	}

	if(pstDef->Important <= 4)
	{
		add_attr_assort_insert(pstEnv, pstDef, iWearPos, iCareer, iFlag, iLevel, 4);
	}

	if(pstDef->Important <= 5)
	{
		add_attr_assort_insert(pstEnv, pstDef, iWearPos, iCareer, iFlag, iLevel, 5);
	}

	if(pstDef->Important <= 6)
	{
		add_attr_assort_insert(pstEnv, pstDef, iWearPos, iCareer, iFlag, iLevel, 6);
	}

	if(pstDef->Important <= 7)
	{
		add_attr_assort_insert(pstEnv, pstDef, iWearPos, iCareer, iFlag, iLevel, 7);
	}

	if(pstDef->Important <= 8)
	{
		add_attr_assort_insert(pstEnv, pstDef, iWearPos, iCareer, iFlag, iLevel, 8);
	}

	if(pstDef->Important <= 9)
	{
		add_attr_assort_insert(pstEnv, pstDef, iWearPos, iCareer, iFlag, iLevel, 9);
	}

	if(pstDef->Important <= 10)
	{
		add_attr_assort_insert(pstEnv, pstDef, iWearPos, iCareer, iFlag, iLevel, 10);
	}
	
	return 0;
}

//穿戴位置、职业之后再次等级分类
int add_attr_assort_level(ZONESVRENV *pstEnv, NEWADDATTRDEF *pstDef, int iWearPos, int iCareer, int iFlag)
{
	if(pstDef->MinLevel <= 0 && pstDef->MaxLevel >= 9)
	{
		add_attr_assort_import(pstEnv, pstDef, iWearPos, iCareer, iFlag, 0);
	}

	if(pstDef->MinLevel <= 10 && pstDef->MaxLevel >= 19)
	{
		add_attr_assort_import(pstEnv, pstDef, iWearPos, iCareer, iFlag, 1);
	}

	if(pstDef->MinLevel <=20 && pstDef->MaxLevel >= 29)
	{
		add_attr_assort_import(pstEnv, pstDef, iWearPos, iCareer, iFlag, 2);
	}

	if(pstDef->MinLevel <=30 && pstDef->MaxLevel >= 39)
	{
		add_attr_assort_import(pstEnv, pstDef, iWearPos, iCareer, iFlag, 3);
	}

	if(pstDef->MinLevel <=40 && pstDef->MaxLevel >= 49)
	{
		add_attr_assort_import(pstEnv, pstDef, iWearPos, iCareer, iFlag, 4);
	}
	
	if(pstDef->MinLevel <=50 && pstDef->MaxLevel >= 59)
	{
		add_attr_assort_import(pstEnv, pstDef, iWearPos, iCareer, iFlag, 5);
	}

	if(pstDef->MinLevel <=60  && pstDef->MaxLevel >= 69)
	{
		add_attr_assort_import(pstEnv, pstDef, iWearPos, iCareer, iFlag, 6);
	}

	if(pstDef->MinLevel <=70 && pstDef->MaxLevel >= 79)
	{
		add_attr_assort_import(pstEnv, pstDef, iWearPos, iCareer, iFlag, 7);
	}

	if(pstDef->MinLevel <=80 && pstDef->MaxLevel >= 89)
	{
		add_attr_assort_import(pstEnv, pstDef, iWearPos, iCareer, iFlag, 8);
	}

	if(pstDef->MinLevel <=90 && pstDef->MaxLevel >= 99)
	{
		add_attr_assort_import(pstEnv, pstDef, iWearPos, iCareer, iFlag, 9);
	}

	if(pstDef->MinLevel <=100 && pstDef->MaxLevel >= 109)
	{
		add_attr_assort_import(pstEnv, pstDef, iWearPos, iCareer, iFlag, 10);
	}

	if(pstDef->MinLevel <=110 && pstDef->MaxLevel >= 119)
	{
		add_attr_assort_import(pstEnv, pstDef, iWearPos, iCareer, iFlag, 11);
	}

	if(pstDef->MinLevel <=120 && pstDef->MaxLevel >= 120)
	{
		add_attr_assort_import(pstEnv, pstDef, iWearPos, iCareer, iFlag, 12);
	}
	
	return 0;
}

//穿戴位置之后再次职业分类
int add_attr_assort_career(ZONESVRENV *pstEnv, NEWADDATTRDEF *pstDef, int iWearPos)
{
	if(pstDef->CareerLimit.None > 0)
	{
		add_attr_assort_level(pstEnv, pstDef, iWearPos, ADD_ATTR_CAREER_NONE, pstDef->CareerLimit.None);
	}

	if(pstDef->CareerLimit.Warrior > 0)
	{
		add_attr_assort_level(pstEnv, pstDef, iWearPos, ADD_ATTR_CAREER_WARRIOR, pstDef->CareerLimit.Warrior);
	}

	if(pstDef->CareerLimit.Magicor > 0)
	{
		add_attr_assort_level(pstEnv, pstDef, iWearPos, ADD_ATTR_CAREER_MAGICOR, pstDef->CareerLimit.Magicor);
	}

	if(pstDef->CareerLimit.Guner > 0)
	{
		add_attr_assort_level(pstEnv, pstDef, iWearPos, ADD_ATTR_CAREER_GUNER, pstDef->CareerLimit.Guner);
	}

	if(pstDef->CareerLimit.Church > 0)
	{
		add_attr_assort_level(pstEnv, pstDef, iWearPos, ADD_ATTR_CAREER_CHURCH, pstDef->CareerLimit.Church);
	}

	if(pstDef->CareerLimit.SingleHandWarrior > 0)
	{
		add_attr_assort_level(pstEnv, pstDef, iWearPos, ADD_ATTR_CAREER_SINGLEHAND_WARRIOR, pstDef->CareerLimit.SingleHandWarrior);
	}

	if(pstDef->CareerLimit.DoubleHandWarrior > 0)
	{
		add_attr_assort_level(pstEnv, pstDef, iWearPos, ADD_ATTR_CAREER_DOUBLEHAND_WARRIOR, pstDef->CareerLimit.DoubleHandWarrior);
	}

	if(pstDef->CareerLimit.SingleHandMagicor>  0)
	{
		add_attr_assort_level(pstEnv, pstDef, iWearPos, ADD_ATTR_CAREER_SINGLEHAND_MAGICOR, pstDef->CareerLimit.SingleHandMagicor);
	}

	if(pstDef->CareerLimit.DoubleHandMagicor > 0)
	{
		add_attr_assort_level(pstEnv, pstDef, iWearPos, ADD_ATTR_CAREER_DOUBLEHAND_MAGICOR, pstDef->CareerLimit.DoubleHandMagicor);
	}

	if(pstDef->CareerLimit.SingleHandGuner > 0)
	{
		add_attr_assort_level(pstEnv, pstDef, iWearPos, ADD_ATTR_CAREER_SINGLEHAND_GUNER, pstDef->CareerLimit.SingleHandGuner);
	}

	if(pstDef->CareerLimit.DoubleHandGuner > 0)
	{
		add_attr_assort_level(pstEnv, pstDef, iWearPos, ADD_ATTR_CAREER_DOUBLEHAND_GUNER, pstDef->CareerLimit.DoubleHandGuner);
	}

	if(pstDef->CareerLimit.SingleHandChurch > 0)
	{
		add_attr_assort_level(pstEnv, pstDef, iWearPos, ADD_ATTR_CAREER_SINGLEHAND_CHURCH, pstDef->CareerLimit.SingleHandChurch);
	}

	if(pstDef->CareerLimit.DoubleHandChurch > 0)
	{
		add_attr_assort_level(pstEnv, pstDef, iWearPos, ADD_ATTR_CAREER_DOUBLEHAND_CHURCH, pstDef->CareerLimit.DoubleHandChurch);
	}
	
	return 0;
}

//穿戴位置分类
int add_attr_assort_wear(ZONESVRENV *pstEnv, NEWADDATTRDEF *pstDef)
{
	if(pstDef->WearPosLimit.RightHand > 0)
	{
		add_attr_assort_career(pstEnv, pstDef, ADD_ATTR_WEARPOS_TYPE_RIGHTHAND);
	}

	if(pstDef->WearPosLimit.Head> 0)
	{
		add_attr_assort_career(pstEnv, pstDef, ADD_ATTR_WEARPOS_TYPE_HEAD);
	}

	if(pstDef->WearPosLimit.Body> 0)
	{
		add_attr_assort_career(pstEnv, pstDef, ADD_ATTR_WEARPOS_TYPE_BODY);
	}

	if(pstDef->WearPosLimit.LeftHand> 0)
	{
		add_attr_assort_career(pstEnv, pstDef, ADD_ATTR_WEARPOS_TYPE_LEFTHAND);
	}

	if(pstDef->WearPosLimit.Face> 0)
	{
		add_attr_assort_career(pstEnv, pstDef, ADD_ATTR_WEARPOS_TYPE_FACE);
	}

	if(pstDef->WearPosLimit.Shoulder> 0)
	{
		add_attr_assort_career(pstEnv, pstDef, ADD_ATTR_WEARPOS_TYPE_SHOULDER);
	}

	if(pstDef->WearPosLimit.Shoe> 0)
	{
		add_attr_assort_career(pstEnv, pstDef, ADD_ATTR_WEARPOS_TYPE_SHOE);
	}

	if(pstDef->WearPosLimit.Amulet> 0)
	{
		add_attr_assort_career(pstEnv, pstDef, ADD_ATTR_WEARPOS_TYPE_AMULET);
	}

	if(pstDef->WearPosLimit.Ring> 0)
	{
		add_attr_assort_career(pstEnv, pstDef, ADD_ATTR_WEARPOS_TYPE_RING);
	}

	if(pstDef->WearPosLimit.NeckLace> 0)
	{
		add_attr_assort_career(pstEnv, pstDef, ADD_ATTR_WEARPOS_TYPE_NECKLACE);
	}

	if(pstDef->WearPosLimit.Back > 0)
	{
		add_attr_assort_career(pstEnv, pstDef, ADD_ATTR_WEARPOS_TYPE_BACK);
	}

	if(pstDef->WearPosLimit.Badge > 0)
	{
		add_attr_assort_career(pstEnv, pstDef, ADD_ATTR_WEARPOS_TYPE_BADGE);
	}
	
	return 0;
}

int add_attr_pre(ZONESVRENV *pstEnv)
{
	int i;
	NEWADDATTRDEF *pstDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	memset(&pstEnv->pstAddAttrFastList->astRandList, 0, sizeof(pstEnv->pstAddAttrFastList->astRandList));
	memset(&pstEnv->pstAddAttrFastList->astWashList, 0, sizeof(pstEnv->pstAddAttrFastList->astWashList));

	for(i=0; i<pstObj->iNewAddAttrDef; i++)
	{
		pstDef= (NEWADDATTRDEF*)pstObj->sNewAddAttrDef[i];
		add_attr_assort_wear(pstEnv, pstDef);
	}
	
	return 0;
}

//获得装备关键字数量
int add_attr_num_rand_get(ZONESVRENV *pstEnv, ITEMDEF *pstItemDef)
{
	int i, iRate, iRand;
	int iAddAttrNum = 0;
	ADDATTRRANDDEF *pstAddAttrRandDef;
	
	pstAddAttrRandDef = z_find_addattr_rand_def(pstEnv, get_arm_itemlevel(pstItemDef->WearPre.RoleLevelMin), pstItemDef->Important);
	if(!pstAddAttrRandDef)
	{
		return 0;
	}

	iRate = 0;
	iRand = RAND1(100);
	for(i=0; i<MAX_ADD_ATTR_RAND_LIST; i++)
	{
		iRate += pstAddAttrRandDef->RandList[i].Rate;
		if(iRand < iRate)
		{
			iAddAttrNum = pstAddAttrRandDef->RandList[i].AddAttrNum;
			break;
		}
	}

	return iAddAttrNum;
}

//获得物品的穿戴位置快速索引
int add_attr_wearpos_idx_get(ITEMDEF *pstItemDef)
{
	switch(pstItemDef->WearPos1)
	{
		case WEAR_POS_RIGHTHAND:
			return ADD_ATTR_WEARPOS_TYPE_RIGHTHAND;
			break;
		case WEAR_POS_HEAD:
			return ADD_ATTR_WEARPOS_TYPE_HEAD;
			break;
		case WEAR_POS_BODY:
			return ADD_ATTR_WEARPOS_TYPE_BODY;
			break;
		case WEAR_POS_LEFT:
			return ADD_ATTR_WEARPOS_TYPE_LEFTHAND;
			break;
		case WEAR_POS_FACE:
			return ADD_ATTR_WEARPOS_TYPE_FACE;
			break;
		case WEAR_POS_SHOULDER:
			return ADD_ATTR_WEARPOS_TYPE_SHOULDER;
			break;
		case WEAR_POS_SHOE:
			return ADD_ATTR_WEARPOS_TYPE_SHOE;
			break;
		case WEAR_POS_AMULET:
			return ADD_ATTR_WEARPOS_TYPE_AMULET;
			break;
		case WEAR_POS_RING:
			return ADD_ATTR_WEARPOS_TYPE_RING;
			break;
		case WEAR_POS_NECKLACE:
			return ADD_ATTR_WEARPOS_TYPE_NECKLACE;
			break;
		case WEAR_POS_FASHION_BACK:
			return ADD_ATTR_WEARPOS_TYPE_BACK;
			break;
		case WEAR_POS_BADGE:
			return ADD_ATTR_WEARPOS_TYPE_BADGE;
			break;
		default:
			return -1;
			break;
	}
	
	return -1;
}

//获得物品的职业快速索引
int add_attr_career_idx_get(ITEMDEF *pstItemDef)
{
	switch(pstItemDef->WearPre.Career)
	{
		case CAREER_NON:
			return ADD_ATTR_CAREER_NONE;
			break;
		case CAREER_WARRIOR:
			return ADD_ATTR_CAREER_WARRIOR;
			break;
		case CAREER_MAGICOR:
			return ADD_ATTR_CAREER_MAGICOR;
			break;
		case CAREER_GUNER:
			return ADD_ATTR_CAREER_GUNER;
			break;
		case CAREER_CHURCH:
			return ADD_ATTR_CAREER_CHURCH;
			break;
		case CAREER_WARRIOR_SINGLEHAND:
			return ADD_ATTR_CAREER_SINGLEHAND_WARRIOR;
			break;
		case CAREER_WARRIOR_DOUBLEHAND:
			return ADD_ATTR_CAREER_DOUBLEHAND_WARRIOR;
			break;
		case CAREER_MAGICOR_SINGLEHAND:
			return ADD_ATTR_CAREER_SINGLEHAND_MAGICOR;
			break;
		case CAREER_MAGICOR_DOUBLEHAND:
			return ADD_ATTR_CAREER_DOUBLEHAND_MAGICOR;
			break;
		case CAREER_GUNER_SINGLEHAND:
			return ADD_ATTR_CAREER_SINGLEHAND_GUNER;
			break;
		case CAREER_GUNER_DOUBLEHAND:
			return ADD_ATTR_CAREER_DOUBLEHAND_GUNER;
			break;
		case CAREER_CHURCH_SINGLEHAND:
			return ADD_ATTR_CAREER_SINGLEHAND_CHURCH;
			break;
		case CAREER_CHURCH_DOUBLEHAND:
			return ADD_ATTR_CAREER_DOUBLEHAND_CHURCH;
			break;
		default:
			return -1;
			break;
	}
	
	return -1;
}

//获得物品的等级快速索引
int add_attr_level_idx_get(ITEMDEF *pstItemDef)
{
	int iLevel = (pstItemDef->WearPre.RoleLevelMin-1)/10;

	if(iLevel < 0)
		iLevel = 0;
	
	return iLevel;
}

/*	
	获得一个物品的关键字库
	fWash:  1 洗练关键字库 0 随机关键字库
*/
OneAddAttrList* add_attr_one_list_get(ZONESVRENV *pstEnv, ITEMDEF *pstItemDef, int fWash)
{
	int iWearPos, iCareer, iLevel, iImportant;
	AddAttrFastList *pstFastList = pstEnv->pstAddAttrFastList;

	iWearPos = add_attr_wearpos_idx_get(pstItemDef);
	if(iWearPos < 0 ||
		iWearPos >= MAX_ADD_ATTR_WEARPOS_LIST)
	{
		return NULL;
	}

	iCareer = add_attr_career_idx_get(pstItemDef);
	if(iCareer < 0 ||
		iCareer >= MAX_ADD_ATTR_CAREER)
	{
		return NULL;
	}
	
	iLevel = get_arm_itemlevel(pstItemDef->WearPre.RoleLevelMin);
	if(iLevel < 0 ||
		iLevel >= MAX_ADD_ATTR_LEVEL_LIST)
	{
		return NULL;
	}

	iImportant = pstItemDef->Important;
	if(iImportant < 0 ||
		iImportant >= MAX_ADD_ATTR_IMPORTANT)
	{
		return NULL;
	}

	if(fWash)
	{
		return &pstFastList->astWashList[iWearPos][iCareer][iLevel][iImportant];
	}
	else
	{
		return &pstFastList->astRandList[iWearPos][iCareer][iLevel][iImportant];
	}
	
	return NULL;
}

//插入一个关键字到物品
int item_add_attr_insert(ZONESVRENV *pstEnv, ROLEARM *pstRoleArm, int iAddAttrID)
{
	int iMulIdx;
	int iMinMul, iMaxMul;
	NEWADDATTRDEF *pstNewAddAttrDef;
	ADDATTRRANDDEF *pstAddAttrRandDef;
	ITEMDEF *pstItemDef;

	pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleArm);
	if(!pstItemDef)
	{
		return -1;
	}
	
	pstAddAttrRandDef = z_find_addattr_rand_def(pstEnv, get_arm_itemlevel(pstItemDef->WearPre.RoleLevelMin), pstItemDef->Important);
	if(!pstAddAttrRandDef)
	{
		return -1;
	}
	
	pstNewAddAttrDef = z_find_new_addattr_def(pstEnv, iAddAttrID);
	if(!pstNewAddAttrDef)
	{
		return -1;
	}

	iMulIdx = pstAddAttrRandDef->AddMulLevel-1;
	if(iMulIdx < 0 || 
		iMulIdx >= MAX_ADD_ATTR_RAND_LEVEL)
	{
		return -1;
	}

	iMinMul = pstNewAddAttrDef->RangeList[iMulIdx].MinMul;
	iMaxMul = pstNewAddAttrDef->RangeList[iMulIdx].MaxnMul;
	if(iMaxMul > iMinMul)
	{
		iMinMul = iMinMul + RAND1(iMaxMul-iMinMul+1);
	}

	if(pstRoleArm->AddAttrNum >= MAX_ARM_ADD_ATTR)
	{
		return -1;
	}
	pstRoleArm->AddAttr[pstRoleArm->AddAttrNum].AddAttrID = iAddAttrID;
	pstRoleArm->AddAttr[pstRoleArm->AddAttrNum].Val = iMinMul;
	pstRoleArm->AddAttrNum++;
	
	return 0;
}

//生成随机关键字
int item_add_attr_rand_create(ZONESVRENV *pstEnv, ROLEARM *pstRoleArm, ITEMDEF *pstItemDef, int iAddAttrNum)
{
	int i, iIdx;
	OneAddAttrList stRandList;
	OneAddAttrList *pstAddAttrRandList;

	pstAddAttrRandList = add_attr_one_list_get(pstEnv, pstItemDef, 0);
	if(!pstAddAttrRandList || pstAddAttrRandList->iAddAttrNum <= 0)
	{
		return -1;
	}

	memcpy(&stRandList, pstAddAttrRandList, sizeof(stRandList));
	for(i=0; i<iAddAttrNum; i++)
	{
		if(stRandList.iAddAttrNum <= 0)
		{
			break;
		}
		
		if(pstRoleArm->AddAttrNum >= iAddAttrNum)
		{
			break;
		}

		if(pstRoleArm->AddAttrNum >= MAX_ARM_ADD_ATTR)
		{
			break;
		}

		iIdx = RAND1(stRandList.iAddAttrNum);
		item_add_attr_insert(pstEnv, pstRoleArm, stRandList.iDefID[iIdx]);
		stRandList.iAddAttrNum--;
		if(iIdx != stRandList.iAddAttrNum)
		{
			memmove(&stRandList.iDefID[iIdx], &stRandList.iDefID[iIdx+1], sizeof(stRandList.iDefID[0])*(stRandList.iAddAttrNum - iIdx));
		}
	}
	
	return 0;
}

//物品关键字生成
int item_add_attr_create(ZONESVRENV *pstEnv, ROLEARM *pstRoleArm, ITEMDEF *pstItemDef)
{
	int i;
	int iAddAttrNum = add_attr_num_rand_get(pstEnv, pstItemDef);
	if(iAddAttrNum <= 0)
	{
		return 0;
	}

	//固定
	for(i=0; i<MAX_ARM_ADD_ATTR; i++)
	{
		if(pstRoleArm->AddAttrNum >= iAddAttrNum)
		{
			break;
		}

		if(pstRoleArm->AddAttrNum >= MAX_ARM_ADD_ATTR)
		{
			break;
		}

		if(pstItemDef->AddAttr[i].ID == 0)
		{
			continue;
		}

		item_add_attr_insert(pstEnv, pstRoleArm, pstItemDef->AddAttr[i].ID);
	}

	//随机
	item_add_attr_rand_create(pstEnv, pstRoleArm, pstItemDef, iAddAttrNum);
	
	return 0;
}

int add_attr_wearpos_mul_get(NEWADDATTRDEF *pstNewAddAttrDef, ITEMDEF *pstItemDef)
{
	switch(pstItemDef->WearPos1)
	{
		case WEAR_POS_RIGHTHAND:
			return pstNewAddAttrDef->WearPosLimit.RightHand;
			break;
		case WEAR_POS_HEAD:
			return pstNewAddAttrDef->WearPosLimit.Head;
			break;
		case WEAR_POS_BODY:
			return pstNewAddAttrDef->WearPosLimit.Body;
			break;
		case WEAR_POS_LEFT:
			return pstNewAddAttrDef->WearPosLimit.LeftHand;
			break;
		case WEAR_POS_FACE:
			return pstNewAddAttrDef->WearPosLimit.Face;
			break;
		case WEAR_POS_SHOULDER:
			return pstNewAddAttrDef->WearPosLimit.Shoulder;
			break;
		case WEAR_POS_SHOE:
			return pstNewAddAttrDef->WearPosLimit.Shoe;
			break;
		case WEAR_POS_AMULET:
			return pstNewAddAttrDef->WearPosLimit.Amulet;
			break;
		case WEAR_POS_RING:
			return pstNewAddAttrDef->WearPosLimit.Ring;
			break;
		case WEAR_POS_NECKLACE:
			return pstNewAddAttrDef->WearPosLimit.NeckLace;
			break;
		case WEAR_POS_FASHION_BACK:
			return pstNewAddAttrDef->WearPosLimit.Back;
			break;
		case WEAR_POS_BADGE:
			return pstNewAddAttrDef->WearPosLimit.Badge;
			break;
		default:
			return 0;
			break;
	}
	
	return 0;
}

double arm_addattr_result_addval_get_double(ZONESVRENV* pstEnv, ITEMDEF *pstItemDef, int iAddAttrID, int iVal)
{
	double iAddVal, iMul = 0, iBase = 0;
	ADDATTRRANDDEF *pstAddAttrRandDef;
	NEWADDATTRDEF *pstNewAddAttrDef;
	
	pstAddAttrRandDef = z_find_addattr_rand_def(pstEnv, get_arm_itemlevel(pstItemDef->WearPre.RoleLevelMin), pstItemDef->Important);
	if(!pstAddAttrRandDef)
	{
		return 0;
	}

	if(pstAddAttrRandDef->ValLevel-1  < 0 || 
		pstAddAttrRandDef->ValLevel-1 >= MAX_ADD_ATTR_VAL_LEVEL)
	{
		return 0;
	}

	pstNewAddAttrDef = z_find_new_addattr_def(pstEnv, iAddAttrID);
	if(!pstNewAddAttrDef)
	{
		return 0;
	}

	//部位修正
	iMul = add_attr_wearpos_mul_get(pstNewAddAttrDef, pstItemDef);

	//基础值
	iBase = pstNewAddAttrDef->Val1[pstAddAttrRandDef->ValLevel-1];

	//具体值=数值*区间系数*部位修正
	//iAddVal = iBase*iVal/100.0*iMul/100.0;     (会遗失精度，例如2 * 141 /100.0*100/100.0 = 2.8199999999)
	iAddVal = iBase*iVal/100.0*iMul/100.0 + 0.005;
	if (iAddVal == 0)
	{
		iAddVal = 1;
	}

	return iAddVal;
}


//关键字附加效果值
int arm_addattr_result_addval_get(ZONESVRENV* pstEnv, ITEMDEF *pstItemDef, int iAddAttrID, int iVal)
{
	return (int)arm_addattr_result_addval_get_double(pstEnv,pstItemDef,iAddAttrID,iVal);
}

//关键字属性计算
int cacl_arm_add_attr_result(ZONESVRENV* pstEnv, Player *pstPlayer, DYNATTRLIST *pstDyn,
	ITEMDEF *pstItemDef, int iAddAttrID, int iVal)
{
	double iAddVal;
	NEWADDATTRDEF *pstNewAddAttrDef;
	ADDATTRRANDDEF *pstAddAttrRandDef  = NULL;
	UNUSED(pstPlayer);
	
	pstNewAddAttrDef = z_find_new_addattr_def(pstEnv, iAddAttrID);
	if(!pstNewAddAttrDef)
	{
		return -1;
	}

	if(pstItemDef->ItemType == ITEM_TYPE_ARM)
	{
		pstAddAttrRandDef = z_find_addattr_rand_def(pstEnv, get_arm_itemlevel(pstItemDef->WearPre.RoleLevelMin), pstItemDef->Important);
		if(!pstAddAttrRandDef)
		{
			return -1;
		}

		iAddVal = arm_addattr_result_addval_get_double(pstEnv, pstItemDef, iAddAttrID, iVal);
		if(iAddVal <= 0)
		{
			return -1;
		}
	}
	else
	{
		iAddVal = iVal;
	}
	
	switch(pstNewAddAttrDef->AddAttrResultID)
	{
		case ADD_ATTR_RESULT_ATK:
		case ADD_ATTR_RESULT_MIN_ATK:
		case ADD_ATTR_RESULT_ENERGE_ATK:
		case ADD_ATTR_RESULT_CANNON_ATK:
			pstDyn->stAtkChg.unIceBaseArmAtk += iAddVal;
			pstDyn->stAtkChg.unPhsicBaseArmAtk += iAddVal;
			break;
		case ADD_ATTR_RESULT_PHSIC_ATK:
			pstDyn->stAtkChg.unPhsicBaseArmAtk += iAddVal;
			break;
		case ADD_ATTR_RESULT_ICE_ATK:
			pstDyn->stAtkChg.unIceBaseArmAtk += iAddVal;
			break;
		case ADD_ATTR_RESULT_HP:
		case ADD_ATTR_RESULT_MIN_HP:
			pstDyn->stHealChg.iHPMaxBase += iAddVal;
			break;
		case ADD_ATTR_RESULT_MP:
		case ADD_ATTR_RESULT_MIN_MP:
			pstDyn->stHealChg.iMPMaxBase += iAddVal;
			break;
		case ADD_ATTR_RESULT_MIN_EXP:
		case ADD_ATTR_RESULT_EXP:
			pstDyn->stHealChg.nArmExpMul += iAddVal;
			break;
		case ADD_ATTR_RESULT_PHSIC_DEF:
			pstDyn->stDefChg.unPhsicDefBaseAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_ICE_DEF:
			pstDyn->stDefChg.unIceDefBaseAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_ADD_DEF:
			pstDyn->stDefChg.unPhsicDefBaseAdd += iAddVal;
			pstDyn->stDefChg.unIceDefBaseAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_DEF_PHISICHURT:
			pstDyn->stAtkeeHurtChg.AtkeePhsicHurtAdd += iAddVal;
			break;	
		case ADD_ATTR_RESULT_DEF_ICEHURT:
			pstDyn->stAtkeeHurtChg.AtkeeIceHurtAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_DEF_ICE_PHISIC:
			pstDyn->stAtkeeHurtChg.AtkeePhsicHurtAdd +=iAddVal;
			pstDyn->stAtkeeHurtChg.AtkeeIceHurtAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_FLEE:
			pstDyn->stFleeChg.unFleeBaseAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_FIGHTVAL:
			pstDyn->stMidAttrList.uiFight += iAddVal;
			break;
		case ADD_ATTR_RESULT_HPADD:
			pstDyn->stHealChg.unArmHPAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_MPADD:
			pstDyn->stHealChg.unArmMPAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_NOFIGHT_HPADD:
			pstDyn->stHealChg.unNoFightRestoreHPAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_NOFIGHT_MPADD:
			pstDyn->stHealChg.unNoFightRestoreMPAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_FIGHT_HPADD:
			pstDyn->stHealChg.unFightRestoreHPAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_FIGHT_MPADD:
			pstDyn->stHealChg.unFightRestoreMPAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_SKILL_LEVEL:
			if(pstAddAttrRandDef)
			{
				z_arm_skill_levelup(pstDyn, 
					pstNewAddAttrDef->Val2, 
					pstNewAddAttrDef->Val1[pstAddAttrRandDef->ValLevel]);
			}
			break;
		case ADD_ATTR_RESULT_BADSTATUS_RATE:
			pstDyn->stHealChg.ucBadStatusRate += iAddVal;
			break;
		case ADD_ATTR_RESULT_BADSTATUS_DEF:
			pstDyn->stHealChg.ucBadStatusDefRate += iAddVal;
			break;
		case ADD_ATTR_RESULT_BADSTATUS_DEC_TIME:
			pstDyn->stHealChg.unBadStatusDecTime += iAddVal;
			break;
		case ADD_ATTR_RESULT_HEAVY_HIT:
			pstDyn->stHitRateChg.nHeavyHitAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_HITRATE:
			pstDyn->stHitChg.iHitChgAdd+= iAddVal;
			break;
		case ADD_ATTR_RESULT_HITRATE_RATE:
			pstDyn->stHitRateChg.nAtkerHitRateAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_TASK_ITEM_RATE:
			if (pstDyn->stHealChg.ucTaskItemRate+iAddVal> 100)
			{
				pstDyn->stHealChg.ucTaskItemRate = 100;
			}
			else
			{
				pstDyn->stHealChg.ucTaskItemRate+= iAddVal;
			}
			break;
		case ADD_ATTR_RESULT_STUFF_ITEM_RATE:
			if (pstDyn->stHealChg.ucStuffItemRate+iAddVal> 100)
			{
				pstDyn->stHealChg.ucStuffItemRate = 100;
			}
			else
			{
				pstDyn->stHealChg.ucStuffItemRate+= iAddVal;
			}
			break;
		case ADD_ATTR_RESULT_EGG_RATE:
			if (pstDyn->stHealChg.ucEggRate+iAddVal> 100)
			{
				pstDyn->stHealChg.ucEggRate = 100;
			}
			else
			{
				pstDyn->stHealChg.ucEggRate+= iAddVal;
			}
			break;
		case ADD_ATTR_RESULT_ADD_BULL:
			if (pstDyn->stHealChg.ucBullRate+iAddVal> 100)
			{
				pstDyn->stHealChg.ucBullRate = 100;
			}
			else
			{
				pstDyn->stHealChg.ucBullRate+= iAddVal;
			}
			break;
		case ADD_ATTR_RESULT_ADD_PET_EXP:
			pstDyn->stHealChg.unPetExpMul += iAddVal;
			break;
		case ADD_ATTR_RESULT_ADD_SKILLPOINT:
			pstDyn->stHealChg.unSkillPonitMul += iAddVal;
			break;
		case ADD_ATTR_RESULT_HEAVY_HIT_VAL:
			pstDyn->stHitRateChg.nHeavyHitVal += iAddVal*100;
			break;
		case ADD_ATTR_RESULT_BASE_ATTR_HEAVYHIT:
			pstDyn->stHitRateChg.iHeavyHit += iAddVal;
			break;
		case ADD_ATTR_RESULT_DEF_HEAVY_HIT:
			pstDyn->stHitRateChg.iDefHeavyHit += iAddVal;
			break;
		case ADD_ATTR_RESULT_LILIANG:
			pstDyn->stMidAttrList.iLiliangAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_ZHILI:
			pstDyn->stMidAttrList.iZhiliAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_TIZHI:
			pstDyn->stMidAttrList.iTiliAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_FANGHU:
			pstDyn->stMidAttrList.iFangyuAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_JINGSHEN:
			pstDyn->stMidAttrList.iJingshenAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_MINJIE:
			pstDyn->stMidAttrList.iMinJieAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_WATERATK:
			pstDyn->stAtkChg.unWaterBaseArmAtk += iAddVal;
			break;
		case ADD_ATTR_RESULT_FIREATK:
			pstDyn->stAtkChg.unFireBaseArmAtk += iAddVal;
			break;
		case ADD_ATTR_RESULT_THUNDERATK:
			pstDyn->stAtkChg.unThunderBaseArmAtk += iAddVal;
			break;
		case ADD_ATTR_RESULT_LIGHTATK:
			pstDyn->stAtkChg.unLightBaseArmAtk += iAddVal;
			break;
		case ADD_ATTR_RESULT_NIGHTATK:
			pstDyn->stAtkChg.unNightBaseArmAtk += iAddVal;
			break;
		case ADD_ATTR_RESULT_WATERDEF:
			pstDyn->stDefChg.unWaterDefBaseAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_FIREDEF:
			pstDyn->stDefChg.unFireDefBaseAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_THUNDERDEF:
			pstDyn->stDefChg.unThunderDefBaseAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_LIGHTDEF:
			pstDyn->stDefChg.unLightDefBaseAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_NIGHTDEF:
			pstDyn->stDefChg.unNightDefBaseAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_REFLECTION_HP:
			pstDyn->stDefChg.iReflectHP += iAddVal;
			break;
		case ADD_ATTR_RESULT_IGNORE_DEF:
			pstDyn->stDefChg.iIgnoreDef += iAddVal;
			break;
		case ADD_ATTR_RESULT_DEF_HP:
			pstDyn->stDefChg.iDefHP += iAddVal;
			break;
		case ADD_ATTR_RESULT_HEAL_HP:
			pstDyn->stHealChg.nHealHPMul += iAddVal;
			break;
		case ADD_ATTR_RESULT_ADD_SKILL:
			if(pstAddAttrRandDef)
			{
				z_arm_add_skill(pstEnv, pstDyn, 
					pstNewAddAttrDef->Val2, 
					pstNewAddAttrDef->Val1[pstAddAttrRandDef->ValLevel]);
			}
			break;
		case ADD_ATTR_RESULT_KILLLER_ADD_HP:
			pstDyn->stHealChg.iKillerAddHP += iAddVal;
			break;
		case ADD_ATTR_RESULT_KILLLER_ADD_MP:
			pstDyn->stHealChg.iKillerAddMP += iAddVal;
			break;
		case ADD_ATTR_RESULT_MSPD:
			pstDyn->stHealChg.nSpeedAdd += iAddVal*BASE_MSPD/100.0;
			break;
		case ADD_ATTR_RESULT_NOFIGHT_ADD_HP:
			pstDyn->stHealChg.iNoFightRestoreHPMul += iAddVal;
			break;
		case ADD_ATTR_RESULT_NOFIGHT_ADD_MP:
			pstDyn->stHealChg.iNoFightRestoreMPMul += iAddVal;
			break;
		case ADD_ATTR_RESULT_ATK_PET_HURTMUL:
			pstDyn->stAtkChg.iAtkPetHurtMul += iAddVal;
			break;
		case ADD_ATTR_RESULT_ATK_NORMALMON_HURTMUL:
			pstDyn->stAtkChg.iAtkNormalMonHurtMul += iAddVal;
			break;
		case ADD_ATTR_RESULT_ATK_JYMON_HURTMUL:
			pstDyn->stAtkChg.iAtkJYMonHurtMul += iAddVal;
			break;
		case ADD_ATTR_RESULT_ATK_BOSSMON_HURTMUL:
			pstDyn->stAtkChg.iAtkBossMonHurtMul += iAddVal;
			break;
		case ADD_ATTR_RESULT_ATK_REDPLAYER_HURTMUL:
			pstDyn->stAtkChg.iAtkRedHurtMul+= iAddVal;
			break;
		case ADD_ATTR_RESULT_TONGXING_ADD_FLEE:
			pstDyn->stHitRateChg.iTongXingHitRate+= iAddVal;
			break;
		case ADD_ATTR_RESULT_TONGXING_ADD_HEAVYHIT:
			pstDyn->stHitRateChg.iTongXingHeavyHit += iAddVal;
			break;
		case ADD_ATTR_RESULT_YIXING_ADD_HEAVYHIT:
			pstDyn->stHitRateChg.iYiXingHeavyHit += iAddVal;
			break;
		case ADD_ATTR_RESULT_YIXING_ADD_FLEE:
			pstDyn->stHitRateChg.iYiXingHitRate += iAddVal;
			break;
		case ADD_ATTR_RESULT_ATK_ZS_HURTMUL:
			pstDyn->stAtkChg.iAtkZSHurtMul += iAddVal;
			break;
		case ADD_ATTR_RESULT_ATK_FS_HURTMUL:
			pstDyn->stAtkChg.iAtkFSHurtMul += iAddVal;
			break;
		case ADD_ATTR_RESULT_ATK_JS_HURTMUL:
			pstDyn->stAtkChg.iAtkJSHurtMul += iAddVal;
			break;
		case ADD_ATTR_RESULT_ATK_QS_HURTMUL:
			pstDyn->stAtkChg.iAtkQSHurtMul += iAddVal;
			break;
		case ADD_ATTR_RESULT_DEC_ZS_HURTMUL:
			pstDyn->stAtkChg.iDecZSHurtMul += iAddVal;
			break;
		case ADD_ATTR_RESULT_DEC_FS_HURTMUL:
			pstDyn->stAtkChg.iDecFSHurtMul += iAddVal;
			break;
		case ADD_ATTR_RESULT_DEC_JS_HURTMUL:
			pstDyn->stAtkChg.iDecJSHurtMul += iAddVal;
			break;
		case ADD_ATTR_RESULT_DEC_QS_HURTMUL:
			pstDyn->stAtkChg.iDecQSHurtMul += iAddVal;
			break;
		case ADD_ATTR_RESULT_HP_PERCENT:
			pstDyn->stHealChg.iHpMulWanFengLv += iAddVal;
			break;
		default:
			break;
	}

	return 0;
}

//获得装备洗炼关键字数量
int wash_add_attr_num_rand_get(ZONESVRENV *pstEnv, ROLEARM *pstRoleArm)
{
	int iRate, iRand;
	int iAddAttrNum = pstRoleArm->WashAttrNum;
	ADDATTRWASHDEF *pstAddAttrWashDef;
	
	pstAddAttrWashDef = z_find_addattr_wash_def(pstEnv, pstRoleArm->WashAttrNum);
	if(!pstAddAttrWashDef)
	{
		return 0;
	}

	iRate = 0;
	iRand = RAND1(100);

	iRate += pstAddAttrWashDef->DownRate;
	if(iRand < iRate)
	{
		if(iAddAttrNum > 0)
		{
			iAddAttrNum--;
		}
		goto _final;
	}

	iRate += pstAddAttrWashDef->KeepRate;
	if(iRand < iRate)
	{
		goto _final;
	}

	iRate += pstAddAttrWashDef->UpRate;
	if(iRand < iRate)
	{
		iAddAttrNum++;
		goto _final;
	}

_final:
	
	return iAddAttrNum;
}

int wash_addattr_online_insert(ZONESVRENV* pstEnv, Player *pstPlayer, 
	ADDATTRRANDDEF *pstAddAttrRandDef, int iAddAttrID, int iInsertOnline, ROLEARM *pstArm)
{
	int iMinMul, iMaxMul;
	NEWADDATTRDEF *pstNewAddAttrDef;
	AddAttrWashOL *pstWashOL = &pstPlayer->stOnline.stAddAttrWashOL;

	if(pstWashOL->iWashAddAttrNum >= MAX_ARM_WASH_ADD_ATTR)
	{
		return -1;
	}
	
	pstNewAddAttrDef = z_find_new_addattr_def(pstEnv, iAddAttrID);
	if(!pstNewAddAttrDef)
	{
		return -1;
	}

	if(pstAddAttrRandDef->AddMulLevel-1< 0 ||
		pstAddAttrRandDef->AddMulLevel-1 >= MAX_ADD_ATTR_RAND_LEVEL)
	{
		return -1;
	}

	iMinMul = pstNewAddAttrDef->RangeList[pstAddAttrRandDef->AddMulLevel-1].MinMul;
	iMaxMul = pstNewAddAttrDef->RangeList[pstAddAttrRandDef->AddMulLevel-1].MaxnMul;
	//iMaxMul = pstNewAddAttrDef->RangeList[pstAddAttrRandDef->AddMulLevel-1].MidMul;
	if(iMaxMul > iMinMul)
	{
		iMinMul = iMinMul + RAND1(iMaxMul-iMinMul+1);
	}

	if (iInsertOnline)
	{
		pstWashOL->stWashAddAttr[pstWashOL->iWashAddAttrNum].AddAttrID = iAddAttrID;
		pstWashOL->stWashAddAttr[pstWashOL->iWashAddAttrNum].Val = iMinMul;
		pstWashOL->iWashAddAttrNum++;
	}
	else
	{
		pstArm->WashAddAttr[pstArm->WashAttrNum].AddAttrID = iAddAttrID;
		pstArm->WashAddAttr[pstArm->WashAttrNum].Val= iMinMul;
		pstArm->WashAttrNum ++;
	}
	
	return 0;
}

int player_arm_wash_process(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEARM *pstRoleArm, 
								ITEMDEF *pstItemDef,int iLockFlag, int iSafeFlag, int iMust)
{
	int i, iMinMul, iMaxMul;
	int iWashAttrNum, iIdx;
	NEWADDATTRDEF *pstNewAddAttrDef;
	ADDATTRRANDDEF *pstAddAttrRandDef;
	OneAddAttrList stWashList;
	OneAddAttrList *pstAddAttrWashList;
	AddAttrWashOL *pstWashOL = &pstPlayer->stOnline.stAddAttrWashOL;
	int iLockIdx = 0;
	int j;
	int iWashLock = 0;//蓝色属性锁了多少条
	ADDATTRINST stOld[10];

	memset(pstWashOL, 0, sizeof(*pstWashOL));
	pstWashOL->ullArmWID = pstRoleArm->WID;
	
	pstAddAttrRandDef = z_find_addattr_rand_def(pstEnv, get_arm_itemlevel(pstItemDef->WearPre.RoleLevelMin), pstItemDef->Important);
	if(!pstAddAttrRandDef)
	{
		return -1;
	}

	//天生关键字
	for(i=0; i<pstRoleArm->AddAttrNum; i++)
	{
		if (iLockFlag & (int)pow(2.0,i))
		{
			// 锁定了值不变
			iMinMul = pstRoleArm->AddAttr[i].Val;
		}
		else
		{
			pstNewAddAttrDef = z_find_new_addattr_def(pstEnv, pstRoleArm->AddAttr[i].AddAttrID);
			if(!pstNewAddAttrDef)
			{
				return -1;
			}

			if(pstAddAttrRandDef->AddMulLevel-1< 0 ||
				pstAddAttrRandDef->AddMulLevel-1 >= MAX_ADD_ATTR_RAND_LEVEL)
			{
				return -1;
			}

			iMinMul = pstNewAddAttrDef->RangeList[pstAddAttrRandDef->AddMulLevel-1].MinMul;
			iMaxMul = pstNewAddAttrDef->RangeList[pstAddAttrRandDef->AddMulLevel-1].MaxnMul;
			//iMaxMul = pstNewAddAttrDef->RangeList[pstAddAttrRandDef->AddMulLevel-1].MidMul;
			if(iMaxMul > iMinMul)
			{
				iMinMul = iMinMul + RAND1(iMaxMul-iMinMul+1);
			}
		}
		
		if(pstWashOL->iAddAttrNum >= MAX_ARM_ADD_ATTR)
		{
			break;
		}
		
		pstWashOL->stAddAttr[pstWashOL->iAddAttrNum].AddAttrID = pstRoleArm->AddAttr[i].AddAttrID;
		pstWashOL->stAddAttr[pstWashOL->iAddAttrNum].Val = iMinMul;
		pstWashOL->iAddAttrNum++;
	}

	iLockIdx = pstRoleArm->AddAttrNum;

	//洗炼关键字
	pstAddAttrWashList = add_attr_one_list_get(pstEnv, pstItemDef, 1);
	if(!pstAddAttrWashList || pstAddAttrWashList->iAddAttrNum <= 0)
	{
		return 0;
	}

	for (i=0; i<pstRoleArm->WashAttrNum; i++)
	{
		if (iLockFlag & (int)pow(2.0,i+iLockIdx))
		{
			stOld[iWashLock].AddAttrID = pstRoleArm->WashAddAttr[i].AddAttrID;
			stOld[iWashLock].Val = pstRoleArm->WashAddAttr[i].Val;
			iWashLock++;	
		}
	}

	iWashAttrNum = wash_add_attr_num_rand_get(pstEnv, pstRoleArm);
	if (iWashAttrNum < iWashLock)
	{
		// 随机出来的条数不能比锁定的少
		iWashAttrNum = iWashLock;
	}

	// 保护洗练不可能减条数
	if (iSafeFlag)
	{
		if (iWashAttrNum < pstRoleArm->WashAttrNum)
		{
			iWashAttrNum =  pstRoleArm->WashAttrNum	;
		}
	}

	if(iMust)
	{
		iWashAttrNum = MAX_ARM_WASH_ADD_ATTR;
	}
	
	memcpy(&stWashList, pstAddAttrWashList, sizeof(stWashList));
	
	// 剔除被锁定的条目
	if (iWashLock > 0)
	{
		for (i=stWashList.iAddAttrNum-1; i>=0; i--)
		{
			for (j=0; j<iWashLock;j++)
			{
				if (stWashList.iDefID[i] == stOld[j].AddAttrID)
				{
					stWashList.iAddAttrNum--;
					if(i != stWashList.iAddAttrNum)
					{
						memmove(&stWashList.iDefID[i], &stWashList.iDefID[i+1], sizeof(stWashList.iDefID[0])*(stWashList.iAddAttrNum - i));
					}	
					break;
				}
			}
		}
	}
	
	// 按原来的规则随机
	for(i=0; i<iWashAttrNum; i++)
	{
		if(pstAddAttrWashList->iAddAttrNum <= 0)
		{
			break;
		}
		
		if(pstWashOL->iWashAddAttrNum >= iWashAttrNum)
		{
			break;
		}

		if(pstWashOL->iWashAddAttrNum >= MAX_ARM_WASH_ADD_ATTR)
		{
			break;
		}

		iIdx = RAND1(stWashList.iAddAttrNum);
		wash_addattr_online_insert(pstEnv, pstPlayer, pstAddAttrRandDef, stWashList.iDefID[iIdx],1,NULL);
		
		stWashList.iAddAttrNum--;
		if(iIdx != stWashList.iAddAttrNum)
		{
			memmove(&stWashList.iDefID[iIdx], &stWashList.iDefID[iIdx+1], sizeof(stWashList.iDefID[0])*(stWashList.iAddAttrNum - iIdx));
		}
	}

	// 恢复锁定的条目
	if (iWashLock > 0)
	{
		for (i=0; i<iWashLock; i++)
		{
			pstWashOL->stWashAddAttr[i].AddAttrID = stOld[i].AddAttrID;
			pstWashOL->stWashAddAttr[i].Val = stOld[i].Val;
		}
	}
	
	return 0;
}

//装备洗炼检查
int player_arm_wash_check(ZONESVRENV* pstEnv, Player *pstPlayer, ARMWASHDATA *pstCSWash, ITEMDEF *pstArmItemDef, int iMoney)
{
	ITEMDEF *pstItemDef;
	RESULT *pstResult;

	if(pstCSWash->Must)
	{
		pstItemDef = z_find_itemdef(pstEnv, pstCSWash->ItemID);
		if(!pstItemDef)
		{
			return -1;
		}

		pstResult = z_item_get_result(pstItemDef, RESULT_ARM_WASH_STUFF_MUST);
		if(!pstResult)
		{
			return -1;
		}

		if(pstArmItemDef->WearPre.RoleLevelMin > pstResult->ResultVal1)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM101);
			return -1;
		}

		if(pstCSWash->Bind)
		{
			if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstCSWash->ItemID, 1, -1, NULL,PACKAGE_DEC_BIND))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM102);
				return -1;
			}
		}
		else
		{
			if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstCSWash->ItemID, 1, -1, NULL,PACKAGE_DEC_UNBIND))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM103);
				return -1;
			}
		}	
	}
	else
	{
		pstItemDef = z_find_itemdef(pstEnv, pstCSWash->ItemID);
		if(!pstItemDef)
		{
			return -1;
		}

		pstResult = z_item_get_result(pstItemDef, RESULT_ARM_WASH_STUFF);
		if(!pstResult)
		{
			return -1;
		}

		if(pstArmItemDef->WearPre.RoleLevelMin > pstResult->ResultVal1)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM101);
			return -1;
		}

		if(pstCSWash->Bind)
		{
			if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstCSWash->ItemID, 1, -1, NULL,PACKAGE_DEC_BIND))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM102);
				return -1;
			}
		}
		else
		{
			if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstCSWash->ItemID, 1, -1, NULL,PACKAGE_DEC_UNBIND))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM103);
				return -1;
			}
		}	
	}

	

	if ( 0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, iMoney) )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM104, MONEY_TYPE_MONEY_NAME);
		return -1;
	}
	
	return 0;
}


HIGHWASHUSEDEF* z_find_highwashuse_def(ZONESVRENV *pstEnv, int LockNum)
{
	int iIndex;
	int iEqu;
	HIGHWASHUSEDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.LockNum = LockNum;
	iIndex = bsearch_int(&stDef, pstObj->sHighWashUseDef, pstObj->iHighWashUseDef, HIGHWASH_USE_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (HIGHWASHUSEDEF*)pstObj->sHighWashUseDef[iIndex];
		
	}
	return NULL;
}

//装备洗炼
int player_arm_wash(ZONESVRENV* pstEnv, Player *pstPlayer, ARMWASHDATA *pstCSWash)
{
	CSPKG stPkg;
	CSACTIONRES  *pstActionRes = &stPkg.Body.ActionRes;
	ROLEGRID *pstArmGrid =NULL;
	ITEMDEF *pstArmItemDef = NULL;
	int iMoney = 0, iDec = 0;
	int iRet;
	AddAttrWashOL *pstWashOL = &pstPlayer->stOnline.stAddAttrWashOL;
	int iLockNum = 0;		// 锁住了多少条
	int i;
	HIGHWASHUSEDEF *pstUseDef;
	int iAttrNum = 0;
	int iJinbi = 0;
	

	memset(&pstActionRes->Data.ArmWash, 0, sizeof(pstActionRes->Data.ArmWash));

	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		goto _error;
	}
	
	if( pstCSWash->ArmData.ArmListType == LIST_TYPE_WEAR)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ARM_WASH);
		goto _error;
	}
	
	pstArmGrid = get_pak_item_grid(pstPlayer, pstCSWash->ArmData.ArmListType, pstCSWash->ArmData.ArmGridIdx);
	if (!pstArmGrid || pstArmGrid->Type != GRID_ITEM_ARM)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM_WASH1);
		goto _error;
	}

	if (arm_is_uncheck(&pstArmGrid->GridData.RoleArm))
	{
		//z_sys_strf(pstEnv, pstPlayer,SYS_MIDDLE, "此装备还未鉴定,不能进行此操作!");
		goto _error;
	}

	if(is_fengyin_arm(&pstArmGrid->GridData.RoleArm))
	{
		goto _error;
	}

	pstArmItemDef = z_get_itemdef_roleitem(pstEnv, &pstArmGrid->GridData.RoleItem);
	if (!pstArmItemDef)
	{
		goto _error;
	}

	//lzk
	if(arm_is_badge(pstArmItemDef) < 0)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ARM902);
		goto _error;
	}

	if(arm_is_fashion(pstArmItemDef))
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_ARM903);
		goto _error;
	}

	if(pstCSWash->Must && (pstCSWash->SafeFlag || pstCSWash->LockFlag))
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ARM904);
		goto _error;
	}

	// 保护洗练要扣额外金币
	if (pstCSWash->SafeFlag)
	{
		USEDEF *pstUDef;
		pstUDef = z_find_usedef(pstEnv, USE_ID_WASH_SAFE);
		if (!pstUDef)
		{
			goto _error;
		}

		iJinbi += pstUDef->UseJinbi;
	}
	

	// 高级洗练
	if (pstCSWash->LockFlag > 0)
	{
		if (genius_get_mod(pstEnv,pstPlayer) < 150)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM_WASH2);
			goto _error;
		}

		iAttrNum = pstArmGrid->GridData.RoleArm.AddAttrNum+pstArmGrid->GridData.RoleArm.WashAttrNum;
		for (i=0; i<iAttrNum;i++)
		{
			if (pstCSWash->LockFlag & (int)pow(2.0,i))
			{
				iLockNum ++;	
			}	
		}	

		if (iLockNum >= iAttrNum)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM_WASH3);
			goto _error;
		}
	}
	else
	{
		// 保护洗练不扣绑银了
		if(!pstCSWash->SafeFlag)
		{
			iMoney = ceil(pstArmItemDef->WearPre.RoleLevelMin/10.0)*5;
		}
	}

	if(0 > player_arm_wash_check(pstEnv, pstPlayer, pstCSWash, pstArmItemDef, iMoney))
	{
		goto _error;
	}

	// 检查锁定消耗
	if (iLockNum)
	{	
		pstUseDef = z_find_highwashuse_def(pstEnv, iLockNum);
		if (!pstUseDef)
		{
			goto _error;
		}

		iJinbi += pstUseDef->UseVal;
	}

	if (iJinbi > 0)
	{
		if ( 0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_GODCOIN, iJinbi))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ARM_WASH4);
			goto _error;
		}
	}

	if (iMoney > 0)
	{
		if ( 0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, iMoney))
		{
			goto _error;
		}
	}

	if(pstCSWash->Bind)
	{
		iDec = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstCSWash->ItemID, 1, -1, NULL,PACKAGE_DEC_BIND, IMPITEM_OPTYPE_ARMWASH);
		if (0 > iDec)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "package dec err");
			goto _error;
		}
	}
	else
	{
		iDec = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,  pstCSWash->ItemID, 1, -1, NULL,PACKAGE_DEC_UNBIND, IMPITEM_OPTYPE_ARMWASH);
		if (0 > iDec)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "package dec err");
			goto _error;
		}
	}
	

	pstArmGrid = get_pak_item_grid(pstPlayer, pstCSWash->ArmData.ArmListType, pstCSWash->ArmData.ArmGridIdx);
	if (!pstArmGrid)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "Grid error");
		goto _error;
	}

	if(iDec > 0 && !(pstArmGrid->GridData.RoleArm.InstFlag & INST_ITEM_BIND))
	{
		pstArmGrid->GridData.RoleArm.InstFlag |= INST_ITEM_BIND;
		arm_one_grid_update(pstEnv,  pstPlayer, pstArmGrid);
	}

	if(0 > player_arm_wash_process(pstEnv, pstPlayer, &pstArmGrid->GridData.RoleArm, 
								pstArmItemDef,pstCSWash->LockFlag, pstCSWash->SafeFlag, pstCSWash->Must))
	{
		goto _error;
	}

	if (iJinbi > 0)
	{
		iRet = package_dec_money( pstEnv, pstPlayer, ATTR_ID_GODCOIN, iJinbi);
		godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
						pstPlayer->stRoleData.Level, GODCOIN_OPER_FLAG_ARM_HIGH_WASH,pstArmItemDef->ItemID,0,0,
						iJinbi,"高级洗练");
	}
		
	if (iMoney > 0)
	{
		iRet = package_dec_money( pstEnv, pstPlayer, ATTR_ID_BULL, iMoney);
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ARMWASH, pstArmItemDef->ItemID, 
						iMoney-iRet, iRet, "装备洗炼");
	}	

	pstActionRes->Data.ArmWash.Succ = 1;
	pstActionRes->Data.ArmWash.AddAttrNum = pstWashOL->iAddAttrNum;
	memcpy(&pstActionRes->Data.ArmWash.AddAttr[0], &pstWashOL->stAddAttr[0], 
		sizeof(pstActionRes->Data.ArmWash.AddAttr[0])*pstWashOL->iAddAttrNum);
	
	pstActionRes->Data.ArmWash.WashAttrNum = pstWashOL->iWashAddAttrNum;
	memcpy(&pstActionRes->Data.ArmWash.WashAddAttr[0], &pstWashOL->stWashAddAttr[0], 
		sizeof(pstActionRes->Data.ArmWash.WashAddAttr[0])*pstWashOL->iWashAddAttrNum);

	{	
		MACHINNGEXPENDDATA stExpendData;
		bzero(&stExpendData, sizeof(stExpendData));
		stExpendData.Money = iRet;
		stExpendData.Bull = iMoney - iRet;
		stExpendData.StuffNum = 1;
		itemdec_fill(&stExpendData.StuffInfo[0], pstCSWash->ItemID, 1, iDec);
		z_role_machining_oplog(pstEnv, pstPlayer, &pstArmGrid->GridData.RoleArm, NULL,
			&stExpendData, OP_MACHINING_ARM_WASH, 1, NULL,0);
	}

		//成长之路
	{
		ACHIEVEMENTCOND stCond;

		memset(&stCond,0,sizeof(stCond));
		
		stCond.Type = COND_EQUIP_BAPTIZE;


		z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
	}

	task_player_trace(pstEnv, pstPlayer, TASK_OBJ_ARM_WASH);	

_final:
	pstActionRes->ActionID = ACTION_ARM_WASH;
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
_error:
	pstActionRes->Data.ArmWash.Succ = 0;
	goto _final;
	return 0;
}

//装备洗炼保存
int player_arm_wash_save(ZONESVRENV* pstEnv, Player *pstPlayer, ARMWASHSAVEDATA *pstCSWash)
{
	ROLEARM stOldRoleArm, *pstRoleArm;
	ROLEGRID *pstArmGrid =NULL;
	AddAttrWashOL *pstWashOL = &pstPlayer->stOnline.stAddAttrWashOL;

	if (player_check_safelock( pstEnv, pstPlayer) < 0)
	{
		return -1;
	}
	
	if( pstCSWash->ArmData.ArmListType == LIST_TYPE_WEAR)
	{
		return -1;
	}
	
	pstArmGrid = get_pak_item_grid(pstPlayer, pstCSWash->ArmData.ArmListType, pstCSWash->ArmData.ArmGridIdx);
	if (!pstArmGrid || pstArmGrid->Type != GRID_ITEM_ARM)
	{
		return -1;
	}

	if(pstArmGrid->GridData.RoleArm.WID != pstPlayer->stOnline.stAddAttrWashOL.ullArmWID)
	{
		return -1;
	}
	
	memcpy(&stOldRoleArm,&pstArmGrid->GridData.RoleArm,sizeof(stOldRoleArm));

	pstRoleArm = &pstArmGrid->GridData.RoleArm;

	//天生关键字
	pstRoleArm->AddAttrNum = pstWashOL->iAddAttrNum;
	memcpy(&pstRoleArm->AddAttr[0], 
			&pstWashOL->stAddAttr[0], 
			sizeof(pstRoleArm->AddAttr[0])*pstWashOL->iAddAttrNum);

	//洗炼关键字
	pstRoleArm->WashAttrNum = pstWashOL->iWashAddAttrNum;
	memcpy(&pstRoleArm->WashAddAttr[0], 
			&pstWashOL->stWashAddAttr[0], 
			sizeof(pstRoleArm->WashAddAttr[0])*pstWashOL->iWashAddAttrNum);
	
	arm_one_grid_update(pstEnv,  pstPlayer, pstArmGrid);

	memset(pstWashOL, 0, sizeof(*pstWashOL));
	{	
		MACHINNGEXPENDDATA stExpendData;
		bzero(&stExpendData, sizeof(stExpendData));
		
		z_role_machining_oplog(pstEnv, pstPlayer, &stOldRoleArm, &pstArmGrid->GridData.RoleArm, 
			&stExpendData, OP_MACHINING_ARM_WASH_SAVE, 1, NULL,0);
	}


	return 0;
}

int arm_hurt_mul_get(ZONESVRENV *pstEnv, AtkerInfo *pstAtkerInfo, AtkeeInfo *pstAtkeeInfo)
{
	int iHurtMul = 0;
	ZoneAni *pstAtkerAni = &pstAtkerInfo->stAtker;
	ZoneAni *pstAtkeeAni = &pstAtkeeInfo->stAtkee;

	if(pstAtkeeAni->iType == OBJ_PLAYER)
	{
		Player *psAtkee = pstAtkeeAni->stObj.pstPlayer;

		if(pstAtkerInfo->stDyn.stAtkChg.iAtkRedHurtMul > 0 &&
			is_bad_palyer(psAtkee))
		{
			iHurtMul += pstAtkerInfo->stDyn.stAtkChg.iAtkRedHurtMul;
		}
		
		switch(psAtkee->stRoleData.Career)
		{
			case CAREER_WARRIOR:
			case CAREER_WARRIOR_SINGLEHAND:
			case CAREER_WARRIOR_DOUBLEHAND:	
				if(pstAtkerInfo->stDyn.stAtkChg.iAtkZSHurtMul > 0)
				{
					iHurtMul += pstAtkerInfo->stDyn.stAtkChg.iAtkZSHurtMul;
				}
				break;
			case CAREER_MAGICOR:
			case CAREER_MAGICOR_SINGLEHAND:			
			case CAREER_MAGICOR_DOUBLEHAND:				
				if(pstAtkerInfo->stDyn.stAtkChg.iAtkFSHurtMul > 0)
				{
					iHurtMul += pstAtkerInfo->stDyn.stAtkChg.iAtkFSHurtMul;
				}
				break;
			case CAREER_GUNER:
			case CAREER_GUNER_SINGLEHAND:	
			case CAREER_GUNER_DOUBLEHAND:			
				if(pstAtkerInfo->stDyn.stAtkChg.iAtkQSHurtMul > 0)
				{
					iHurtMul += pstAtkerInfo->stDyn.stAtkChg.iAtkQSHurtMul;
				}
				break;
			case CAREER_CHURCH:
			case CAREER_CHURCH_SINGLEHAND:
			case CAREER_CHURCH_DOUBLEHAND:			
				if(pstAtkerInfo->stDyn.stAtkChg.iAtkJSHurtMul > 0)
				{
					iHurtMul += pstAtkerInfo->stDyn.stAtkChg.iAtkJSHurtMul;
				}
				break;
			default:
				break;
		}

		if(pstAtkerAni->iType == OBJ_PLAYER)
		{
			switch(pstAtkerAni->stObj.pstPlayer->stRoleData.Career)
			{
			case CAREER_WARRIOR:
			case CAREER_WARRIOR_SINGLEHAND:
			case CAREER_WARRIOR_DOUBLEHAND:	
				if(pstAtkeeInfo->stDyn.stAtkChg.iDecZSHurtMul > 0)
				{
					iHurtMul -= pstAtkeeInfo->stDyn.stAtkChg.iDecZSHurtMul;
				}
				break;
			case CAREER_MAGICOR:
			case CAREER_MAGICOR_SINGLEHAND:			
			case CAREER_MAGICOR_DOUBLEHAND:				
				if(pstAtkeeInfo->stDyn.stAtkChg.iDecFSHurtMul > 0)
				{
					iHurtMul -= pstAtkeeInfo->stDyn.stAtkChg.iDecFSHurtMul;
				}
				break;
			case CAREER_GUNER:
			case CAREER_GUNER_SINGLEHAND:	
			case CAREER_GUNER_DOUBLEHAND:			
				if(pstAtkeeInfo->stDyn.stAtkChg.iDecQSHurtMul > 0)
				{
					iHurtMul -= pstAtkeeInfo->stDyn.stAtkChg.iDecQSHurtMul;
				}
				break;
			case CAREER_CHURCH:
			case CAREER_CHURCH_SINGLEHAND:
			case CAREER_CHURCH_DOUBLEHAND:			
				if(pstAtkeeInfo->stDyn.stAtkChg.iDecJSHurtMul > 0)
				{
					iHurtMul -= pstAtkeeInfo->stDyn.stAtkChg.iDecJSHurtMul;
				}
				break;
			default:
				break;
			}
		}
	}
	else if(pstAtkeeAni->iType == OBJ_MONSTER)
	{
		Monster *pstMon = pstAtkeeAni->stObj.pstMon;

		switch(pstMon->bSubType)
		{
			case MON_SUB_TYPE_MINI:
			case MON_SUB_TYPE_SOFT:
			case MON_SUB_TYPE_WEAK:
			
				if(pstAtkerInfo->stDyn.stAtkChg.iAtkNormalMonHurtMul> 0)
				{
					iHurtMul += pstAtkerInfo->stDyn.stAtkChg.iAtkNormalMonHurtMul;
				}
				break;
			case MON_SUB_TYPE_NORM:
			case MON_SUB_TYPE_JIN:
			
				if(pstAtkerInfo->stDyn.stAtkChg.iAtkJYMonHurtMul> 0)
				{
					iHurtMul += pstAtkerInfo->stDyn.stAtkChg.iAtkJYMonHurtMul;
				}
				break;
			case MON_SUB_TYPE_HARD:
			//case MON_SUB_TYPE_JI:
			case MON_SUB_TYPE_XI:
			case MON_SUB_TYPE_BOSS:
				if(pstAtkerInfo->stDyn.stAtkChg.iAtkBossMonHurtMul> 0)
				{
					iHurtMul += pstAtkerInfo->stDyn.stAtkChg.iAtkBossMonHurtMul;
				}
				break;
			default:
				break;
		}
	}
	else if(pstAtkeeAni->iType == OBJ_PET)
	{
		if(pstAtkerInfo->stDyn.stAtkChg.iAtkPetHurtMul > 0)
		{
			iHurtMul += pstAtkerInfo->stDyn.stAtkChg.iAtkPetHurtMul;
		}
	}

	return iHurtMul;
}

int player_kill_restore_hp_mp(ZONESVRENV* pstEnv, AtkerInfo *pstAtkerInfo, ZoneAni *pstAtkee)
{
	PlayerChagAttrVal stAttr;
	Player *pstPlayer;
	int iOldHP;
	int iOldMP;

	stAttr.AttrNum = 0;
	if(pstAtkerInfo->stAtker.iType != OBJ_PLAYER)
	{
		return -1;
	}

	if(pstAtkee->iType == OBJ_MONSTER)
	{
		if(pstAtkee->stObj.pstMon->bSubType > MON_SUB_TYPE_JI)
		{
			return -1;
		}
	}

	pstPlayer = pstAtkerInfo->stAtker.stObj.pstPlayer;
	if(CS_STAT_DEAD & pstPlayer->stOnline.State)
	{
		return -1;
	}

	iOldHP = pstPlayer->stRoleData.RoleDetail.HPCurr;
	iOldMP = pstPlayer->stRoleData.RoleDetail.MPCurr;
	if(pstAtkerInfo->stDyn.stHealChg.iKillerAddHP > 0)
	{
		stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_HP;
		stAttr.AttrList[stAttr.AttrNum].nFlag = 0;
		stAttr.AttrList[stAttr.AttrNum].Value.HP = pstAtkerInfo->stDyn.stHealChg.iKillerAddHP;
		stAttr.AttrNum++;
	}

	if(pstAtkerInfo->stDyn.stHealChg.iKillerAddMP > 0)
	{
		stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_MP;
		stAttr.AttrList[stAttr.AttrNum].nFlag = 0;
		stAttr.AttrList[stAttr.AttrNum].Value.HP = pstAtkerInfo->stDyn.stHealChg.iKillerAddMP;
		stAttr.AttrNum++;
	}

	z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stAttr, 0);
	player_hp_action(pstEnv, pstPlayer, iOldHP, iOldMP, 0);
	return 0;
}

int player_addattr_dec_wearlevel(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEARM *pstRoleArm, ITEMDEF *pstItemDef)
{
	int i;
	int iAddVal = 0;
	NEWADDATTRDEF *pstNewAddAttrDef;
	ADDATTRRANDDEF *pstAddAttrRandDef  = NULL;
	
	for(i=0; i<pstRoleArm->WashAttrNum; i++)
	{
		pstNewAddAttrDef = z_find_new_addattr_def(pstEnv, pstRoleArm->WashAddAttr[i].AddAttrID);
		if(!pstNewAddAttrDef)
		{
			continue;
		}

		if(pstNewAddAttrDef->AddAttrResultID != ADD_ATTR_RESULT_DEC_WEAR_LEVEL)
		{
			continue;
		}

		// 被封印了,不能减等级
		if (arm_attr_is_fengyin(pstRoleArm, i, 0))
		{
			continue;
		}
		
		pstAddAttrRandDef = z_find_addattr_rand_def(pstEnv, get_arm_itemlevel(pstItemDef->WearPre.RoleLevelMin), pstItemDef->Important);
		if(!pstAddAttrRandDef)
		{
			continue;
		}

		iAddVal = arm_addattr_result_addval_get(pstEnv, pstItemDef, pstRoleArm->WashAddAttr[i].AddAttrID, pstRoleArm->WashAddAttr[i].Val);
		break;
	}

	if(iAddVal < 0)
		iAddVal = 0;
	
	return iAddVal;
}

int player_addattr_heavyhit_add(AtkerInfo *pstAtkerInfo, AtkeeInfo *pstAtkeeInfo)
{
	DYNATTRLIST *pstAtkerDyn = &pstAtkerInfo->stDyn;
	
	if(pstAtkerInfo->stAtker.iType == OBJ_PLAYER && 
		pstAtkeeInfo->stAtkee.iType == OBJ_PLAYER)
	{
		if(pstAtkerInfo->stAtker.stObj.pstPlayer->stRoleData.Gender ==
			pstAtkeeInfo->stAtkee.stObj.pstPlayer->stRoleData.Gender)
		{
			if(pstAtkerDyn->stHitRateChg.iTongXingHeavyHit > 0)
			{
				return  pstAtkerDyn->stHitRateChg.iTongXingHeavyHit;
			}
		}
		else
		{
			if(pstAtkerDyn->stHitRateChg.iYiXingHeavyHit > 0)
			{
				return  pstAtkerDyn->stHitRateChg.iYiXingHeavyHit;
			}
		}
	}

	return 0;
}

//技能效果 目标血量符合条件增加暴击率百分比
int skill_result_add_heavyhit_rate(SKILLDEF *pstSkillDef, AtkeeInfo *pstAtkeeInfo)
{
	int iCurrHP, iMaxHP;
	SKILLRESULT *pstResult = z_get_skill_result(pstSkillDef, RESULT_TARGET_HP_HEAVYHIT_RATE);

	if(!pstResult)
	{
		return 0;
	}

	if(pstAtkeeInfo->stAtkee.iType == OBJ_PLAYER)
	{
		Player *pstPlayer = pstAtkeeInfo->stAtkee.stObj.pstPlayer;
		
		iCurrHP = pstPlayer->stRoleData.RoleDetail.HPCurr;
		iMaxHP = pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax;
	}
	else if(pstAtkeeInfo->stAtkee.iType == OBJ_MONSTER)
	{
		Monster *pstMon = pstAtkeeInfo->stAtkee.stObj.pstMon;

		iCurrHP = pstMon->iHP;
		iMaxHP = pstMon->iMaxHP;
	}
	
	if(iCurrHP*100.0/iMaxHP <= pstResult->SkillResultVal1)
	{
		return pstResult->SkillResultVal2;
	}

	return 0;
}

