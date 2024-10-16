/*
**  @file $RCSfile: zone_attr.c,v $
**  general description of this module
**  $Id: zone_attr.c,v 1.588 2014/06/23 09:43:20 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/06/23 09:43:20 $
**  @version $Revision: 1.588 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_attr.h"
#include "zone_clt.h"
#include "zone_err.h"
#include "zone_skill.h"
#include "zone_move.h"
#include "zone_status.h"
#include "zone_npc.h"
#include "zone_package.h"
#include "zone_relation.h"
#include "zone_pet.h"
#include "zone_arm.h"
#include "zone_combat.h"
#include "zone_task.h"
#include "zone_player.h"
#include "zone_clan.h"
#include "zone_fairy.h"
#include "zone_range.h"
#include "zone_oplog.h"
#include "zone_machine.h"
#include "zone_script.h"
#include "zone_ai.h"
#include "zone_city.h"
#include "zone_rune.h"
#include "zone_addattr.h"
#include "zone_designation.h"
#include "zone_genius.h"
#include "zone_levfeng.h"
#include "zone_err.h"
#include "zone_syncrelation.h"
#include "zone_ploy.h"

extern char g_cResultFlag;

#define ATTR_TYPE_UNSHORT			0x01
#define ATTR_TYPE_UNINT			0x02
#define ATTR_TYPE_UNCHAR			0x03
#define ATTR_TYPE_STRUCT			0x04

typedef struct tagSuitCount
{
	int iSuitID;
	int iCount;
}SuitCount;

typedef struct tagAttrMapper
{
	unsigned char bAttr;	
	unsigned char bDataType;	
	unsigned int uiMax;
	int iOff;	
	int (* AttrAddFun)(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, AttrChgVal *pstVal, CSPLAYERATTRVAL *pstChg);
	char *pszNotify;
}AttrMapper;

#define ATTR_DYNOFF   offsetof(Player, stOnline)+offsetof(PlayerOnline, stDyn)
#define ATTR_ROLEDETAIL  offsetof(Player, stRoleData)+offsetof(ROLEDATA, RoleDetail)
#define ATTR_MID  ATTR_DYNOFF+offsetof(DYNATTRLIST, stMidAttrList)
#define ATTR_PACKAGE offsetof(Player, stRoleData)+offsetof(ROLEDATA, Package)
#define ATTR_MISC offsetof(Player, stRoleData)+offsetof(ROLEDATA,MiscInfo)

int z_add_attr_exp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, AttrChgVal *pstVal, CSPLAYERATTRVAL *pstChg);
int z_add_attr_zhengyi(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, AttrChgVal *pstVal, CSPLAYERATTRVAL *pstChg);
LEVELATTR *get_career_level_attr(LEVELATTRDEF *pstLevelAttrDef, int iCareer);
int get_pet_level_mul(ZONESVRENV* pstEnv, int iLevel, int iType);
int z_add_attr_lifebrand(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, AttrChgVal *pstVal, CSPLAYERATTRVAL *pstChg);


#define MAX_CHARM 10000000

static AttrMapper s_mapper[]	=
{
	{ATTR_ID_HP,		                ATTR_TYPE_UNINT,		MAX_HP,             ATTR_ROLEDETAIL+offsetof(ROLEDETAIL, HPCurr),         NULL, NULL},
	{ATTR_ID_HPMAX,		         ATTR_TYPE_UNINT,		MAX_HP,             ATTR_MID+offsetof(MIDATTRLIST, uiHPMax),              NULL, NULL},
	{ATTR_ID_MP,		         ATTR_TYPE_UNINT,		MAX_MP,             ATTR_ROLEDETAIL+offsetof(ROLEDETAIL, MPCurr),         NULL, NULL},
	{ATTR_ID_MPMAX,		         ATTR_TYPE_UNINT,		MAX_MP,             ATTR_MID+offsetof(MIDATTRLIST, uiMPMax),              NULL, NULL},
	{ATTR_ID_LILIANG,		     ATTR_TYPE_UNSHORT,		SHORT_MAX,             ATTR_MID+offsetof(MIDATTRLIST, unLiliang),         NULL, NULL},
	{ATTR_ID_VIM,		         ATTR_TYPE_UNSHORT,		MAX_VIM,            ATTR_ROLEDETAIL+offsetof(ROLEDETAIL, VIMCurr),        NULL, NULL},
	{ATTR_ID_BAOJI,	             ATTR_TYPE_UNSHORT,		0,    -1,   NULL, NULL},
	{ATTR_ID_EXP,		         ATTR_TYPE_UNINT,		0/*MAX_EXP*/,            ATTR_ROLEDETAIL+offsetof(ROLEDETAIL, Exp),            z_add_attr_exp, NULL},
	{ATTR_ID_ZHILI,	     ATTR_TYPE_UNSHORT,	    SHORT_MAX,     ATTR_MID+offsetof(MIDATTRLIST, unZhili),      NULL, NULL},
	{ATTR_ID_MINJIE,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_FANGYU,     ATTR_TYPE_UNSHORT,	    SHORT_MAX,      ATTR_MID+offsetof(MIDATTRLIST, unFangyu),    NULL, NULL},
	{ATTR_ID_MINGZHONG,  ATTR_TYPE_UNSHORT,	    0,      -1,  NULL, NULL},	
	{ATTR_ID_TILI,       ATTR_TYPE_UNSHORT,	    SHORT_MAX,      ATTR_MID+offsetof(MIDATTRLIST, unTili),            NULL, NULL},
	{ATTR_ID_JINGSHEN,   ATTR_TYPE_UNSHORT,	    SHORT_MAX,      ATTR_MID+offsetof(MIDATTRLIST, unJingshen),       NULL, NULL},
	{ATTR_ID_LEVEL,      ATTR_TYPE_UNSHORT,	    MAX_LEVEL,      offsetof(Player, stRoleData)+offsetof(ROLEDATA, Level),  NULL, NULL},
	{ATTR_ID_COLLECTNUM,      ATTR_TYPE_STRUCT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_NLMAX,		         ATTR_TYPE_UNINT,		MAX_NL,            -1,              NULL, NULL},
	{ATTR_ID_VIMMAX,		   ATTR_TYPE_UNINT,		MAX_VIM,           ATTR_MID+offsetof(MIDATTRLIST, uiVimMax),              NULL, NULL},
	{ATTR_ID_SPEMOVEMAX,		   ATTR_TYPE_UNSHORT,	MAX_SPE_MOVE_POWER,           ATTR_MID+offsetof(MIDATTRLIST, uiSpeMoveMax),              NULL, NULL},
	{ATTR_ID_SKILLPOINT,		   ATTR_TYPE_UNINT,	             MAX_SKILL_POINT,           ATTR_ROLEDETAIL+offsetof(ROLEDETAIL, SkillPoint),              NULL, LJY_ATTR1},	
	{ATTR_ID_MONEY,		       ATTR_TYPE_UNINT,	             MAX_MONEY,           ATTR_ROLEDETAIL+offsetof(ROLEDETAIL, Money),              NULL, MONEY_TYPE_MONEY_NAME},	
	{ATTR_ID_BULL,		       ATTR_TYPE_UNINT,	             MAX_BULL,           ATTR_ROLEDETAIL+offsetof(ROLEDETAIL, Bull),              NULL, MONEY_TYPE_BULL_NAME},	

	{ATTR_ID_WATER_DEF,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_WATER_ATK,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_FIRE_ATK,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_THUNDER_ATK,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_LIGHT_ATK,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_NIGHT_ATK,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_HIT_RATE,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_FLEE_RATE,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_MSPD,		 ATTR_TYPE_UNSHORT,	             0,           -1,              NULL, NULL},		
	
	{ATTR_ID_PHSIC_HURT,		 ATTR_TYPE_UNSHORT,	             0,          -1,              NULL, NULL},	
	{ATTR_ID_STASHMONEY,	 ATTR_TYPE_UNINT,	             MAX_MONEY,       ATTR_PACKAGE+offsetof(ROLEPACKAGE, StashPackage)+offsetof(STASHPACKAGE, Money),              NULL, NULL},		
	{ATTR_ID_FIGHT,		       ATTR_TYPE_UNINT,	             MAX_FIGHT,         ATTR_MID+offsetof(MIDATTRLIST, uiFight),              NULL, NULL},
	{ATTR_ID_TMPENERGE,        ATTR_TYPE_STRUCT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_PHSIC_ATK,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_ICE_ATK,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_DAMAGE,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_NOFIGHT_HPADD,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_NOFIGHT_MPADD,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_BADSTATUS_DEF,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},	
	{ATTR_ID_PHSIC_DEF,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_ICE_DEF,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_FIRE_DEF,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_THUNDER_DEF,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_LIGHT_DEF,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_NIGHT_DEF,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},	
	{ATTR_ID_DEF_PHSIC_HURT,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_DEF_ICE_HURT,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_DEF_FIRE_HURT,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_DEF_THUNDER_HURT,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_DEF_LIGHT_HURT,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_DEF_NIGHT_HURT,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},	
	{ATTR_ID_HEAVY,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},	
	{ATTR_ID_HIT,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},	
	{ATTR_ID_FLEE,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},	
	{ATTR_ID_HPADD,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},	
	{ATTR_ID_MPADD,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},	
	{ATTR_ID_VIGOR,        ATTR_TYPE_UNINT, MAX_VIG, ATTR_ROLEDETAIL+offsetof(ROLEDETAIL, GreenJade), NULL, NULL},	
	{ATTR_ID_VIGORMAX,  ATTR_TYPE_UNINT, MAX_VIG, ATTR_MID+offsetof(MIDATTRLIST, uiVigorMax), NULL, NULL},	
	{ATTR_ID_GODCOIN,		       ATTR_TYPE_UNINT,	             MAX_GODCOIN,           ATTR_ROLEDETAIL+offsetof(ROLEDETAIL, GodCoin),              NULL, MONEY_TYPE_GODCOIN_NAME},	
	{ATTR_ID_BINDGODCOIN,		       ATTR_TYPE_UNINT,	             MAX_BINDGODCOIN,           ATTR_ROLEDETAIL+offsetof(ROLEDETAIL, BindGodCoin),              NULL, MONEY_TYPE_BIND_GODCOIN_NAME},	
	{ATTR_ID_VIPPOINT,		       ATTR_TYPE_UNINT,	             MAX_VIPPOINT,           ATTR_ROLEDETAIL+offsetof(ROLEDETAIL, VipPoint),              NULL, LJY_ATTR2},	
	{ATTR_ID_XP,		       ATTR_TYPE_UNINT,	             MAX_XP,           ATTR_ROLEDETAIL+offsetof(ROLEDETAIL, XP),              NULL, NULL},	
	{ATTR_ID_XPMAX,		       ATTR_TYPE_UNINT,	             MAX_XP,            ATTR_MID+offsetof(MIDATTRLIST, uiXpMax),          NULL, NULL},	

	{ATTR_ID_STASHBULL, ATTR_TYPE_UNINT, MAX_BULL,         ATTR_PACKAGE+offsetof(ROLEPACKAGE, StashPackage)+offsetof(STASHPACKAGE, Bull),          NULL, NULL},	
	{ATTR_ID_CONTRI, ATTR_TYPE_UNINT,MAX_CONTRIBUTE,  ATTR_MISC+offsetof(ROLEMISCINFO,ClanInfo)+offsetof(CLANINFO,Contribute),          NULL, LJY_ATTR3},	
	{ATTR_ID_COLLECTSKILLUSE, ATTR_TYPE_UNINT,MAX_COLLECTSKILLUSE,  ATTR_MISC+offsetof(ROLEMISCINFO,CollectSkillInfo)+offsetof(COLLECTSKILLINFO,CollectSkill_SlotCurr), NULL, NULL},	

	{ATTR_ID_ICE_HURT,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_HEAL_HP_ADD,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_HEAL_HP_MUL,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_ATKEE_HEAL_HP_ADD,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_ATKEE_HEAL_HP_MUL,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},

	{ATTR_ID_BASE_ATTR_HEAVY,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_BASE_ATTR_DEFHEAVY,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_HEAVY_VAL,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_BATTLE_GLORY,		       ATTR_TYPE_UNINT,     	  MAX_BATTLE_GLORY,       ATTR_ROLEDETAIL+offsetof(ROLEDETAIL, BlueJade),              NULL, MONEY_TYPE_BATTLE_GLORY},	
	{ATTR_ID_AOSHAN_BRAND,		       ATTR_TYPE_UNSHORT,    MAX_AOSHAN_BRAND,     ATTR_ROLEDETAIL+offsetof(ROLEDETAIL, BlueGem),              NULL, MONEY_TYPE_AOSHAN_BRAND},	
	{ATTR_ID_LIFT_BRAND,		       ATTR_TYPE_UNSHORT,	  MAX_LIFT_BRAND,           ATTR_ROLEDETAIL+offsetof(ROLEDETAIL, GreenGem),             z_add_attr_lifebrand, MONEY_TYPE_LIFT_BRAND},	
	{ATTR_ID_REFLECTION_HP,        ATTR_TYPE_UNINT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_IGNORE_DEF,        ATTR_TYPE_UNINT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_DEF_HP,        ATTR_TYPE_UNINT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_PHSIC_MAX_ATK,        ATTR_TYPE_UNINT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_MAGIC_MAX_ATK,        ATTR_TYPE_UNINT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_SPARTA_GLORY,        ATTR_TYPE_UNINT,	 MAX_SPARTA_GLORY, ATTR_ROLEDETAIL+offsetof(ROLEDETAIL, SpartaGlory),  NULL, LJY_ATTR7},
	{ATTR_ID_DODGE,        ATTR_TYPE_UNSHORT,	    0,                           -1,                                                                           NULL, NULL},
	{ATTR_ID_CHARM,        ATTR_TYPE_UNINT, MAX_CHARM, ATTR_ROLEDETAIL+offsetof(ROLEDETAIL, WhiteJade), NULL, NULL},	
	{ATTR_ID_SCHOOL_VAL,        ATTR_TYPE_UNINT, MAX_SCHOOL_VAL, ATTR_ROLEDETAIL+offsetof(ROLEDETAIL, SchoolVal), NULL, NULL},	
	{ATTR_ID_CLAN_WAR_BRAND,	   ATTR_TYPE_UNINT,	  MAX_CLAN_WAR_BRAND,           ATTR_ROLEDETAIL+offsetof(ROLEDETAIL, ClanWarBrand),             NULL, MONEY_TYPE_CLAN_WAR_BRAND},	
	{ATTR_ID_ZHENGYI,		         ATTR_TYPE_UNINT,	MAX_ZHENGYI,            ATTR_ROLEDETAIL+offsetof(ROLEDETAIL, BlackJade),            z_add_attr_zhengyi, MONEY_TYPE_ZHENGYI},	
}; 	

#define ATTR_MAP_SIZE			(sizeof(s_mapper)/sizeof(AttrMapper))
char sWatchAttr[MAX_ATTR_VAL - ATTR_MAP_SIZE];

ARMADDATTRDEF *z_find_armaddattrdef(ZONESVRENV* pstEnv, int AddAttrID)
{
	ARMADDATTRDEF  stArmAddAttr;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stArmAddAttr.AddAttrID = AddAttrID;
	iIndex = bsearch_int(&stArmAddAttr, pstObj->sArmAddAttrDef, pstObj->iArmAddAttrDef, ARM_ADDATTR_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ARMADDATTRDEF *)pstObj->sArmAddAttrDef[iIndex];
	}
	
	return NULL;	
}

ADDATTRGROUPDEF *z_find_addattr_group_def(ZONESVRENV* pstEnv, int iID)
{
	ADDATTRGROUPDEF  stAddAttGroup;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stAddAttGroup.GroupID = iID;
	iIndex = bsearch_int(&stAddAttGroup, pstObj->sAddAttrGroupDef, pstObj->iAddAttrGroupDef, ADD_ATTR_GROUP_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ADDATTRGROUPDEF *)pstObj->sAddAttrGroupDef[iIndex];
	}
	
	return NULL;	
}

ADDATTRBASEDEF *z_find_addattr_base_def(ZONESVRENV* pstEnv, int iLevel)
{
	ADDATTRBASEDEF  stAddAttrBase;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stAddAttrBase.Level= iLevel;
	iIndex = bsearch_int(&stAddAttrBase, pstObj->sAddAttrBaseDef, pstObj->iAddAttrBaseDef, ADD_ATTR_BASE_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ADDATTRBASEDEF *)pstObj->sAddAttrBaseDef[iIndex];
	}
	
	return NULL;	
}

ATTRCONTRASTDEF *z_find_attr_contrast_def(ZONESVRENV* pstEnv, int iID, int iCareer)
{
	ATTRCONTRASTDEF stAttrContrast;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stAttrContrast.BaseAttrID = iID;
	stAttrContrast.Career = iCareer;
	iIndex = bsearch_int(&stAttrContrast, pstObj->sAttrContrastDef, pstObj->iAttrContrastDef, ATTR_CONTRAST_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ATTRCONTRASTDEF *)pstObj->sAttrContrastDef[iIndex];
	}
	
	return NULL;	
}

LEVELATTRDEF  *z_find_level_attr_def(ZONESVRENV* pstEnv, int iLevel)
{
	LEVELATTRDEF stAttrDef;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stAttrDef.Level = iLevel;
	iIndex = bsearch_int(&stAttrDef, pstObj->sLevelAttrDef, pstObj->iLevelAttrDef, LEVEL_ATTR_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (LEVELATTRDEF *)pstObj->sLevelAttrDef[iIndex];
	}
	
	return NULL;	
}

ARMRECASTDEF *z_find_arm_recast_def(ZONESVRENV* pstEnv, int iID)
{
	ARMRECASTDEF stDef;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.ID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sArmRecastDef, pstObj->iArmReCastDef, ARM_RECAST_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ARMRECASTDEF *)pstObj->sArmRecastDef[iIndex];
	}
	
	return NULL;	
}

int z_add_one_baseattr(Player* pstPlayer, unsigned char bAttrID, unsigned int uiVal, short nFlag)
{
	AttrMapper *pstMap;
	long long llVal;
	unsigned int uiMax;
	
	if (bAttrID >= ATTR_MAP_SIZE || NULL == pstPlayer)
	{
		return -1;
	}
	
	pstMap = &s_mapper[bAttrID];
				
	if (pstMap->AttrAddFun || 0 > pstMap->iOff)
	{
		return -1;
	}
	
	switch(pstMap->bDataType)
	{
		case ATTR_TYPE_UNSHORT:
			llVal = *((unsigned short*)((char *)pstPlayer + pstMap->iOff));
			if (nFlag)
			{
				llVal = llVal -uiVal ;
			}
			else
			{
				llVal = llVal + uiVal;
			}

			//几个属性 特殊处理
			switch(bAttrID)
			{
				case ATTR_ID_VIM:
					uiMax = pstPlayer->stOnline.stDyn.stMidAttrList.uiVimMax;
					break;
				//case ATTR_ID_SPEMOVE:
					//uiMax = pstPlayer->stOnline.stDyn.stMidAttrList.uiSpeMoveMax;
					//break;
				//case ATTR_ID_NL:
					//uiMax = pstPlayer->stOnline.stDyn.stMidAttrList.uiNlMax;
					//break;
				//case ATTR_ID_VIGOR:
					//uiMax = pstPlayer->stOnline.stDyn.stMidAttrList.unVigorMax;
					//break;
				default:
					uiMax = pstMap->uiMax;
					break;
			}
			
			if (llVal > uiMax)
			{
				llVal = uiMax;
			}
			else if (llVal < 0)
			{
				llVal = 0;
			}

			*((unsigned short*)((char *)pstPlayer + pstMap->iOff)) = llVal;
			break;
			
		case ATTR_TYPE_UNINT:
			llVal = *((unsigned int*)((char *)pstPlayer + pstMap->iOff));
			if (nFlag)
			{
				llVal = llVal - uiVal;
			}
			else
			{
				llVal = llVal + uiVal;
			}

			//几个属性 特殊处理
			switch(bAttrID)
			{
				case ATTR_ID_HP:
					uiMax = pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax;
					break;
				case ATTR_ID_MP:
					uiMax = pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax;
					break;
				case ATTR_ID_XP:
					uiMax = pstPlayer->stOnline.stDyn.stMidAttrList.uiXpMax;
					break;
				case ATTR_ID_VIGOR:
					uiMax = pstPlayer->stOnline.stDyn.stMidAttrList.uiVigorMax;
					break;
				default:
					uiMax = pstMap->uiMax;
					break;
			}
			
			if (llVal > uiMax)
			{
				llVal = uiMax;
			}
			else if (llVal < 0)
			{
				llVal = 0;
			}

			*((unsigned int*)((char *)pstPlayer + pstMap->iOff)) = llVal;
			break;
			
		case ATTR_TYPE_UNCHAR:
			llVal = *((unsigned char*)((char *)pstPlayer + pstMap->iOff));

			if (nFlag)
			{
				llVal = llVal - uiVal;
			}
			else
			{
				llVal = llVal + uiVal;
			}
			
			if (llVal > pstMap->uiMax)
			{
				llVal = pstMap->uiMax;
			}
			else if (llVal < 0)
			{
				llVal = 0;
			}

			*((unsigned char*)((char *)pstPlayer + pstMap->iOff)) = llVal;
			break;
			
		default:
			return -1;
			break;
	}

	return 0;
}


int player_add_exp(ZONESVRENV* pstEnv, Player *pstPlayer, int iExp, int iFlag)
{
	PlayerChagAttrVal stAttr;
	
	stAttr.AttrNum = 0;
	stAttr.AttrList[stAttr.AttrNum].nFlag = iFlag;
	stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_EXP;
	stAttr.AttrList[stAttr.AttrNum].Value.Exp = iExp;
	stAttr.AttrNum++;	

	z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stAttr, 1);

	return 0;
}

int z_ploy_info_chg(ZONESVRENV* pstEnv, Player *pstPlayer, int iCmd)
{
	CSPKG stPkg;
	CSPOLYFBINFO *pstPolyPBInfo = &stPkg.Body.PolyFBInfo;
	ROLEPOLYBASEINFO *pstSelf = &pstPolyPBInfo->PolyFBInfo.SelfInfo;
	WarFieldOL *pstOL = &pstPlayer->stOnline.stWarOL;

	pstPolyPBInfo->Type 				= iCmd;
	pstSelf->Crystal 					= pstOL->iCrystal;
	pstSelf->Ore    					= pstOL->iOre;
	pstSelf->DieNum 					= pstOL->iDieNum;
	pstSelf->MaxContinueKill 			 = pstOL->iContinueKill;
	pstSelf->MaxKill 					= pstOL->iKillNum;
	pstSelf->MaxLife 					= pstOL->iMaxLife;
	pstSelf->CampIdx 				= pstOL->cCampIdx;
	pstSelf->Career 					= pstPlayer->stRoleData.Career;
	pstSelf->ClanName[0] 				= 0;
	STRNCPY(pstSelf->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstSelf->RoleName));
	if (pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID > 0)
	{
		STRNCPY(pstSelf->ClanName, pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Name, sizeof(pstSelf->ClanName));
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, PLOY_FB_INFO_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int player_add_hp(ZONESVRENV* pstEnv, Player *pstPlayer, int iHp, int iFlag,int iNotify)
{
	PlayerChagAttrVal stAttr;
	
	stAttr.AttrNum = 0;
	stAttr.AttrList[stAttr.AttrNum].nFlag = iFlag;
	stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_HP;
	stAttr.AttrList[stAttr.AttrNum].Value.HP = iHp;
	stAttr.AttrNum++;	

	z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stAttr, iNotify);

	return 0;
}

int player_add_xp(ZONESVRENV* pstEnv, Player *pstPlayer, int iXp, int iFlag,int iNotify)
{
	PlayerChagAttrVal stAttr;
	
	stAttr.AttrNum = 0;
	stAttr.AttrList[stAttr.AttrNum].nFlag = iFlag;
	stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_XP;
	stAttr.AttrList[stAttr.AttrNum].Value.XP = iXp;
	stAttr.AttrNum++;	
	z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stAttr, iNotify);
	return 0;
}

int player_add_vig_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	unsigned int iOld = pstPlayer->stRoleData.RoleDetail.GreenJade;

	if(iOld == pstPlayer->stOnline.stDyn.stMidAttrList.uiVigorMax)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ATTR101);
		return -1;
	}


	return 0;
}


int player_add_vig(ZONESVRENV* pstEnv, Player *pstPlayer, int iVig, int iFlag,int iNotify)
{
	CSPKG stPkg;
	CSPLAYERATTRVAL *pstChg = &stPkg.Body.AttrChg;
	unsigned int iOld = pstPlayer->stRoleData.RoleDetail.GreenJade;

	if(iFlag == 0)
	{
		pstPlayer->stRoleData.RoleDetail.GreenJade += iVig;
		if(pstPlayer->stRoleData.RoleDetail.GreenJade > pstPlayer->stOnline.stDyn.stMidAttrList.uiVigorMax)
		{
			pstPlayer->stRoleData.RoleDetail.GreenJade = pstPlayer->stOnline.stDyn.stMidAttrList.uiVigorMax;
		}
	}
	else
	{
		iVig = min((unsigned int)iVig,pstPlayer->stRoleData.RoleDetail.GreenJade);
		
		pstPlayer->stRoleData.RoleDetail.GreenJade -= iVig;

	}

	if ( iOld != pstPlayer->stRoleData.RoleDetail.GreenJade)
	{
		if (pstPlayer->stRoleData.RoleDetail.GreenJade > iOld)
		{
			if (pstPlayer->stOnline.iVigorEx <= 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_ATTR102, 
								pstPlayer->stRoleData.RoleDetail.GreenJade - iOld);
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_ATTR103, 
								pstPlayer->stRoleData.RoleDetail.GreenJade - iOld, pstPlayer->stOnline.iVigorEx);
				pstPlayer->stOnline.iVigorEx = 0;
			}
		}
		else 
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_ATTR104, 
								iOld - pstPlayer->stRoleData.RoleDetail.GreenJade );
		}
	}

	if(iNotify)
	{
		pstChg->AttrNum = 0;
		pstChg->AttrList[pstChg->AttrNum].AttrID = ATTR_ID_VIGOR;
		pstChg->AttrList[pstChg->AttrNum].Value.Vigor = pstPlayer->stRoleData.RoleDetail.GreenJade;
		pstChg->AttrNum++;
		
		Z_CSHEAD_INIT(&stPkg.Head, ATTR_CHG);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	return 0;
}

void z_level_up_attr( ZONESVRENV* pstEnv, Player* pstPlayer, LEVELUP *pstLevelUp)
{
	int i;
	LEVELUP *pstNextLevelUp;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;
	int iFairyMaxXP;
	UNUSED(pstLevelUp);

	pstNextLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	if (!pstNextLevelUp)
	{
		return;
	}
	
	// 加满XP
	iFairyMaxXP = get_fairy_xp_upper(pstEnv, pstPlayer);
	if ((int)pstPlayer->stRoleData.RoleDetail.XP < iFairyMaxXP)
	{
		player_add_xp(pstEnv, pstPlayer, iFairyMaxXP-pstPlayer->stRoleData.RoleDetail.XP, 0,1);
	}

	if(pstNextLevelUp->AddRolePetGrid &&  pstPetPackage->MaxNum < MAX_PET_PACKAGE)
	{
		if (pstNextLevelUp->AddRolePetGrid + pstPetPackage->MaxNum >= MAX_PET_PACKAGE)
		{
			package_enlarge(pstEnv->pstAppCtx, pstEnv, pstPlayer, LIST_TYPE_SHI, MAX_PET_PACKAGE-pstPetPackage->MaxNum);
		}
		else
		{
			package_enlarge(pstEnv->pstAppCtx, pstEnv, pstPlayer, LIST_TYPE_SHI, pstNextLevelUp->AddRolePetGrid);
		}

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ATTR105);
	}

	// 升级加晶石背包格子
	if (	pstNextLevelUp->AddSparPkg )
	{
		pstPlayer->stRoleData.Package.SparPkg.MaxNum ++;
		pkg_num_chg(pstEnv, pstPlayer, LIST_TYPE_SPAR);
	}

	task_player_trace( pstEnv, pstPlayer, TASK_OBJ_PLAYER);

	switch(pstPlayer->stRoleData.Career)
	{
		case CAREER_WARRIOR:
		case CAREER_WARRIOR_SINGLEHAND:
		case CAREER_WARRIOR_DOUBLEHAND:	
			for (i=0; i<(int)(sizeof(pstNextLevelUp->WarriorSkillList)/sizeof(pstNextLevelUp->WarriorSkillList[0])); i++)
			{
				if (pstNextLevelUp->WarriorSkillList[i] <= 0)
				{
					break;
				}
				z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstNextLevelUp->WarriorSkillList[i], 1, 1);
			}
			
			break;
		case CAREER_MAGICOR:
		case CAREER_MAGICOR_SINGLEHAND:
		case CAREER_MAGICOR_DOUBLEHAND:
			for (i=0; i<(int)(sizeof(pstNextLevelUp->MagicorSkillList)/sizeof(pstNextLevelUp->MagicorSkillList[0])); i++)
			{
				if (pstNextLevelUp->MagicorSkillList[i] <= 0)
				{
					break;
				}
				z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstNextLevelUp->MagicorSkillList[i], 1, 1);
			}
			
			break;
		case CAREER_GUNER:
		case CAREER_GUNER_SINGLEHAND:
		case CAREER_GUNER_DOUBLEHAND:
			for (i=0; i<(int)(sizeof(pstNextLevelUp->GunerSkillList)/sizeof(pstNextLevelUp->GunerSkillList[0])); i++)
			{
				if (pstNextLevelUp->GunerSkillList[i] <= 0)
				{
					break;
				}
				z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstNextLevelUp->GunerSkillList[i], 1, 1);
			}
			break;
		case CAREER_CHURCH:
		case CAREER_CHURCH_SINGLEHAND:
		case CAREER_CHURCH_DOUBLEHAND:	
			for (i=0; i<(int)(sizeof(pstNextLevelUp->ChurchSkillList)/sizeof(pstNextLevelUp->ChurchSkillList[0])); i++)
			{
				if (pstNextLevelUp->ChurchSkillList[i] <= 0)
				{
					break;
				}
				z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstNextLevelUp->ChurchSkillList[i], 1, 1);
			}
			break;
		default:
			return;
			break;
	}

	if (pstPlayer->stRoleData.Level == SAFELOCK_LEVEL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ATTR106);
	}

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_ATTR107, pstPlayer->stRoleData.Level);

	del_taskrecycle_lvlup(pstEnv, pstPlayer);
	z_player_script_event(pstEnv, pstPlayer, PLAYER_EVENT_SCRIPT_PROC_LEVEL);//升级事件

	pstPlayer->stAccShareData.iAccMaxRoleLevel = max(pstPlayer->stAccShareData.iAccMaxRoleLevel,pstPlayer->stRoleData.Level);

	pstPlayer->stRoleData.MiscInfo.LevelBuffExpInfo.AwardNum = 0;	
	/*
	if (pstPlayer->stRoleData.Level == MIN_RUNE_LEVEL)
	{
		player_skill_rune_enable(pstEnv,pstPlayer);
	}
	*/
	return ;
}

void z_level_up_attrlist(ZONESVRENV* pstEnv, Player* pstPlayer, CSPLAYERATTRVAL *pstChg)
{
	ROLEDETAIL *pstRoleDetail = &pstPlayer->stRoleData.RoleDetail;
	
	pstChg->AttrList[pstChg->AttrNum].AttrID = ATTR_ID_LEVEL;
	pstChg->AttrList[pstChg->AttrNum].Value.Level = pstPlayer->stRoleData.Level;
	pstChg->AttrNum++;

	pstChg->AttrList[pstChg->AttrNum].AttrID = ATTR_ID_HP;
	pstChg->AttrList[pstChg->AttrNum].Value.HP = pstPlayer->stRoleData.RoleDetail.HPCurr = pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax;
	pstChg->AttrNum++;

	pstChg->AttrList[pstChg->AttrNum].AttrID = ATTR_ID_MP;
	pstChg->AttrList[pstChg->AttrNum].Value.MP = pstPlayer->stRoleData.RoleDetail.MPCurr = pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax;
	pstChg->AttrNum++;

	pstChg->AttrList[pstChg->AttrNum].AttrID = ATTR_ID_SKILLPOINT;
	pstChg->AttrList[pstChg->AttrNum].Value.SkillPoint = pstRoleDetail->SkillPoint;
	pstChg->AttrNum++;

	pstChg->AttrList[pstChg->AttrNum].AttrID = ATTR_ID_VIGOR;
	pstChg->AttrList[pstChg->AttrNum].Value.Vigor = 
						pstPlayer->stRoleData.RoleDetail.GreenJade = 
						pstPlayer->stOnline.stDyn.stMidAttrList.uiVigorMax;
	pstChg->AttrNum++;

	//最大HP MP
	player_hp_action(pstEnv, pstPlayer, pstPlayer->stRoleData.RoleDetail.HPCurr, pstPlayer->stRoleData.RoleDetail.MPCurr, 0);

	range_single_set(pstEnv, pstPlayer, RANGE_MAIN_SINGLE_SUB_LEVEL, 
					pstPlayer->stRoleData.Level);

	range_career_set(pstEnv, pstPlayer, pstPlayer->stRoleData.Level);

	school_student_levelup(pstEnv, pstPlayer);

	level_feng_levelup(pstEnv, pstPlayer);

	pstPlayer->stRoleData.MiscInfo.DayExpLimitLevel = pstPlayer->stRoleData.Level;
	pstPlayer->stOnline.stDyn.stMidAttrList.uiDayExpMax = get_player_dayexplimit(pstEnv->pstAppCtx, pstEnv, pstPlayer);

	player_zhengyi_limit_chg(pstEnv, pstPlayer);

	player_life_brand_limit_chg(pstEnv, pstPlayer);

	return;	
}

int z_add_attr_exp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, AttrChgVal *pstVal, CSPLAYERATTRVAL *pstChg)
{
	long long llExpOri;
	long long llExp;
	long long llTmpExp;
	unsigned int uiLevExp;
	LEVELUP *pstLevelUp;
	ROLEDETAIL *pstRoleDetail = &pstPlayer->stRoleData.RoleDetail;
	ROLEMISCINFO *pstRoleMisc = &pstPlayer->stRoleData.MiscInfo;
	//DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	char cLevelUp = 0;
	int iOldLevel = pstPlayer->stRoleData.Level;
	tdr_longlong iOldExp = pstPlayer->stRoleData.RoleDetail.Exp;
	int iCurrLevel;
	int iOldLevFengExpDec = 0;
	int iCurrLevFengExpDec = 0;

	if (iOldLevel < 1)
	{
		return -1;
	}

	if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat == GLOBAL_FINI &&
				(pstEnv->pstGlobalObj->stGlobal.LevFeng.LevelFeng > 0 && pstPlayer->stRoleData.Level == pstEnv->pstGlobalObj->stGlobal.LevFeng.LevelFeng))
	{
		iOldLevFengExpDec = level_feng_get_expdec(pstEnv, pstPlayer);
	}

	llExpOri = pstRoleDetail->Exp ;
	if (pstVal->nFlag)
	{
		/*
		if ((double) llExpOri - pstVal->Value.Exp < LLONG_MIN)
		{
			return -1;
		}*/

		llExp = llExpOri - pstVal->Value.Exp;
		
		pstRoleDetail->Exp = llExp;
		z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_EXP_DEC, llExpOri-llExp);
	}
	else
	{
		char szOfflineExp[128];
		char szVipBarExp[128];
		char szLevelExp[128];
		
		szOfflineExp[0] = 0;
		szLevelExp[0] = 0;
		szVipBarExp[0] = 0;
		
		z_player_addexp_test(pstEnv, pstPlayer, &pstVal->Value.Exp);
		
		llExp = pstVal->Value.Exp;

		/*
		if (IsSameDay(pstRoleMisc->DayExpTime, pstEnv->pstAppCtx->stCurr.tv_sec) == 0)
		{
			pstDyn->stMidAttrList.uiDayExpMax = get_player_dayexplimit(pstAppCtx, pstEnv, pstPlayer);
		}

		if (pstDyn->stMidAttrList.uiDayExpMax > 0)
		{
			llTmpExp = llExp + pstRoleMisc->DayExpCount;
			if (llTmpExp > pstDyn->stMidAttrList.uiDayExpMax)
			{
				llExp = (pstDyn->stMidAttrList.uiDayExpMax - pstRoleMisc->DayExpCount);
			}
		}*/

		if (llExp == 0)
		{
			return 0;
		}

		if (pstPlayer->stOnline.iUsedOfflineExp > 0 &&
			pstPlayer->stOnline.iUsedOfflineExp < llExp )
		{
			snprintf(szOfflineExp, sizeof(szOfflineExp), LJY_ATTR4,
					 pstPlayer->stOnline.iUsedOfflineExp);
			pstPlayer->stOnline.iUsedOfflineExp = 0;
		}

		if (pstPlayer->stOnline.iAddWorldLevelExp > 0 &&
			pstPlayer->stOnline.iAddWorldLevelExp < llExp )
		{
			snprintf(szLevelExp, sizeof(szLevelExp), LJY_ATTR5,
					 pstPlayer->stOnline.iAddWorldLevelExp);
			pstPlayer->stOnline.iAddWorldLevelExp = 0;
		}

		if (pstPlayer->stOnline.stVipBarInfo.iVipBarEffectExp > 0 && 
			pstPlayer->stOnline.stVipBarInfo.iVipBarEffectExp < llExp)
		{
			snprintf(szVipBarExp, sizeof(szVipBarExp), LJY_ATTR6,
					 pstPlayer->stOnline.stVipBarInfo.iVipBarEffectExp );
			pstPlayer->stOnline.stVipBarInfo.iVipBarEffectExp = 0;
		}

		if (0 == (MISC_FLAG_EXPNODAYLIMIT & pstPlayer->stOnline.cMiscFlag))
		{
			pstRoleMisc->DayExpCount += llExp;
		}

		if(pstPlayer->stOnline.fAasMsgNotify)
		{
			if (0 == szOfflineExp[0])
			{
				z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_EXP1, llExp);
			}
			else
			{
				z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_EXP2, llExp,szOfflineExp);
			}
		}
		else
		{
			if (0 == szOfflineExp[0] && 0 == szVipBarExp[0] && 0 == szLevelExp[0])
			{
				z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_EXP_ADD, llExp);
			}
			else
			{
				z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_EXP3,  llExp,szVipBarExp,szOfflineExp, szLevelExp);	
			}
		}
		
		while(1)
		{			
			pstLevelUp = (LEVELUP *)pstEnv->pstZoneObj->sLevelUpDef[pstPlayer->stRoleData.Level-1];
			uiLevExp = pstLevelUp->LevelNextExp;

			if (	pstLevelUp->LevelStand || 
				pstPlayer->stRoleData.Level >= pstEnv->pstConf->MaxLevel || 
				pstPlayer->stRoleData.Level >= MAX_LEVEL ||
				pstPlayer->stRoleData.Level >= pstEnv->pstZoneObj->iLevelUpDef ||
				(pstPlayer->stRoleData.Level == CAREERCHG_LEVEL && ((pstPlayer->stRoleData.Career&0xF0) == 0))||
				genius_check_level_up(pstEnv, pstPlayer, pstLevelUp) < 0)
			{
				llTmpExp = pstRoleDetail->Exp + llExp;
				if (llTmpExp > MAX_EXP)
				{
					llTmpExp = MAX_EXP;
				}
				pstRoleDetail->Exp =  llTmpExp;
				
				break;
			}

			if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat != GLOBAL_FINI ||
				(pstEnv->pstGlobalObj->stGlobal.LevFeng.LevelFeng > 0 && pstPlayer->stRoleData.Level >= pstEnv->pstGlobalObj->stGlobal.LevFeng.LevelFeng))
			{
				llTmpExp = pstRoleDetail->Exp + llExp;
				if (llTmpExp > MAX_EXP)
				{
					llTmpExp = MAX_EXP;
				}
				pstRoleDetail->Exp =  llTmpExp;
				
				break;
			}
			
			if( llExp + pstRoleDetail->Exp >= uiLevExp )
			{
				llExp	=	llExp - uiLevExp + pstRoleDetail->Exp;
				pstPlayer->stRoleData.Level++;  
				pstRoleDetail->Exp = 0;
				cLevelUp = 1;

				z_level_up_attr(pstEnv,pstPlayer, pstLevelUp);

				pstPlayer->stRoleData.MiscInfo.LevelRemainInfo.LevelRemainDay = 0;
			}
			else
			{
				pstRoleDetail->Exp	+=	llExp;
				break;
			}
		}
	}

	if (cLevelUp)
	{
		CSPKG stPkg;
		CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

		pstActionRes->ID = pstPlayer->iMemID;
		pstActionRes->ActionID = ACTION_LEVEL;
		pstActionRes->Data.Level = pstPlayer->stRoleData.Level;
		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		z_sendpkg_playerview(pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);

		z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);
		z_level_up_attrlist(pstEnv, pstPlayer, pstChg);

		iCurrLevel = pstPlayer->stRoleData.Level;
		if (iCurrLevel >= 15 && iCurrLevel <= 60)
		{
			if ((iCurrLevel/5) > (iOldLevel/5))
			{
				//player_auto_mate(pstEnv, pstPlayer);
			}
		}

		if(iOldLevel <= 39)
		{
			PlayerChagAttrVal stAttr;

			stAttr.AttrNum = 0;
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_VIM;
			stAttr.AttrList[stAttr.AttrNum].Value.Vim =
				z_get_player_vimmax(pstAppCtx, pstEnv, pstPlayer);
			stAttr.AttrList[stAttr.AttrNum].nFlag = 0;
			stAttr.AttrNum++;
			z_attr_chg(pstAppCtx, pstEnv, pstPlayer, &stAttr, 1);
		}

		if (iOldLevel < LUCKY_LEVEL &&
			pstPlayer->stRoleData.Level >= LUCKY_LEVEL)
		{
			player_login_lucky_ui( pstEnv, pstPlayer);
		}

		if (iOldLevel < GC_PLOY_LVL &&
			pstPlayer->stRoleData.Level >= GC_PLOY_LVL)
		{
			ploy_gc_player_login(pstEnv, pstPlayer);
		}
		
		relation_dbbuddy_chg(pstEnv, pstPlayer, 0,1);

		player_update_cache_level(pstEnv, pstPlayer);
		
		player_clan_update_mem(pstEnv, pstPlayer);

		package_cangbaotu_check(pstEnv,pstPlayer);
		package_cangbaotu_send(pstEnv,pstPlayer);

		z_role_lvup_snapshot_oplog(pstEnv, pstPlayer, iOldLevel, iOldExp,0);
	}
	else
	{
		pstChg->AttrList[pstChg->AttrNum].AttrID = ATTR_ID_EXP;
		pstChg->AttrList[pstChg->AttrNum].Value.Exp = pstRoleDetail->Exp;
		pstChg->AttrNum++;
	}

	// 称号
	player_desig_get(pstEnv, pstPlayer,GET_DESIG_EXP_99999);

	if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat == GLOBAL_FINI &&
				(pstEnv->pstGlobalObj->stGlobal.LevFeng.LevelFeng > 0 && pstPlayer->stRoleData.Level == pstEnv->pstGlobalObj->stGlobal.LevFeng.LevelFeng))
	{
		iCurrLevFengExpDec = level_feng_get_expdec(pstEnv, pstPlayer);
		if (iCurrLevFengExpDec < iOldLevFengExpDec)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ATTR108, iCurrLevFengExpDec);
		}
	}
	
	return 0;
}

int z_add_attr_zhengyi(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, AttrChgVal *pstVal, CSPLAYERATTRVAL *pstChg)
{
	ZHENGYILIMITINFO *pstZhengyiLimit = &pstPlayer->stRoleData.MiscInfo.ZhengyiLimitInfo;
	unsigned int iOldZhengyi = pstPlayer->stRoleData.RoleDetail.BlackJade;

	if (pstVal->nFlag)
	{
		unsigned int iOld = pstPlayer->stRoleData.RoleDetail.BlackJade;
		if (pstPlayer->stRoleData.RoleDetail.BlackJade >= pstVal->Value.Zhengyi)
		{
			pstPlayer->stRoleData.RoleDetail.BlackJade -= pstVal->Value.Zhengyi;
		}
		else
		{
			pstPlayer->stRoleData.RoleDetail.BlackJade = 0;
		}

		if (iOld > pstPlayer->stRoleData.RoleDetail.BlackJade)
		{
			z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_ZHENGYI_DEC, iOld-pstPlayer->stRoleData.RoleDetail.BlackJade);
			z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ZHENGYI_DEC, iOld-pstPlayer->stRoleData.RoleDetail.BlackJade);
		}
	}
	else
	{
		player_add_zhengyi_test(pstEnv, pstPlayer, &pstVal->Value.Zhengyi);
		
		pstPlayer->stRoleData.RoleDetail.BlackJade += pstVal->Value.Zhengyi;
		pstZhengyiLimit->DayCount += pstVal->Value.Zhengyi;

		if (pstVal->Value.Zhengyi > 0)
		{
			z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_ZHENGYI_ADD, pstVal->Value.Zhengyi);
			z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ZHENGYI_ADD, pstVal->Value.Zhengyi);
		}
	}

	if (iOldZhengyi != pstPlayer->stRoleData.RoleDetail.BlackJade)
	{
		pstChg->AttrList[pstChg->AttrNum].AttrID = ATTR_ID_ZHENGYI;
		pstChg->AttrList[pstChg->AttrNum].Value.Zhengyi = pstPlayer->stRoleData.RoleDetail.BlackJade;
		pstChg->AttrNum++;
	}
	
	return 0;
}

int player_dec_exp(ZONESVRENV* pstEnv, Player *pstPlayer, int uiExp)
{
	AttrChgVal *pstVal;
	PlayerChagAttrVal stChgVal;

	stChgVal.AttrNum = 0;

	pstVal = &stChgVal.AttrList[stChgVal.AttrNum++];
	pstVal->AttrID = ATTR_ID_EXP;
	pstVal->nFlag = 1;
	pstVal->Value.Exp = uiExp;

	z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);
	
	return 0;
}

int get_player_sit_restore_hp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	LEVELUP *pstLevelUp;
	HEALCHG *pstHealChg = &pstPlayer->stOnline.stDyn.stHealChg;
	SKILLRESULT stResult;
	UNUSED(pstAppCtx);
	
	pstLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	if (pstLevelUp)
	{
		z_get_diathesis_result(pstEnv, pstPlayer, RESULT_NOSIT_ADD_HP, &stResult);
		pstPlayer->stOnline.stDyn.stMidAttrList.uiLevelUpAddHp = pstLevelUp->SitAddHp*(stResult.SkillResultVal1/100.0);
		return pstLevelUp->SitAddHp *(1+pstHealChg->nSitHPMul/100.0);
	}
	else
	{
		return 0;
	}
}

int get_player_dayexplimit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{	
	LEVELUP *pstLevelUp;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;

	if (pstMiscInfo->DayExpTime == 0 || IsSameDay(pstMiscInfo->DayExpTime, pstEnv->pstAppCtx->stCurr.tv_sec) == 0)
	{
		pstMiscInfo->DayExpCount = 0;
		pstMiscInfo->DayExpTime = pstAppCtx->stCurr.tv_sec;
		pstMiscInfo->DayExpLimitLevel = pstPlayer->stRoleData.Level;
	}
	
	pstLevelUp = z_get_levelup(pstEnv, pstMiscInfo->DayExpLimitLevel);
	if (pstLevelUp)
	{
		return pstLevelUp->DayExpLimit;
	}
	else
	{
		return 0;
	}
}
	

int get_player_sit_restore_mp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	LEVELUP *pstLevelUp;
	HEALCHG *pstHealChg = &pstPlayer->stOnline.stDyn.stHealChg;
	SKILLRESULT stResult;
	UNUSED(pstAppCtx);

	
	pstLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);

	if (pstLevelUp)
	{
		z_get_diathesis_result(pstEnv, pstPlayer, RESULT_NOSIT_ADD_MP, &stResult);
		pstPlayer->stOnline.stDyn.stMidAttrList.uiLevelUpAddMp = pstLevelUp->SitAddMp*(stResult.SkillResultVal1/100.0);
		return pstLevelUp->SitAddMp *(1+pstHealChg->nSitMPMul/100.0);
	}
	else
	{
		return 0;
	}
}

int get_player_sit_add_exp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{	
	LEVELUP *pstLevelUp;
	HEALCHG *pstHealChg = &pstPlayer->stOnline.stDyn.stHealChg;
	UNUSED(pstAppCtx);
	
	pstLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	
	if (pstLevelUp)
	{
		return pstLevelUp->ContributeExp/100.0*GAOJI_SIT_INVAR* (1+pstHealChg->nSitExpMul/100.0);
	}
	else
	{
		return 0;
	}
}

unsigned int z_get_max_vig(ZONESVRENV* pstEnv,int iLvl)
{
	LEVELUP *pstLevelUp = NULL;
	
	pstLevelUp = z_get_levelup(pstEnv, iLvl);
	if (!pstLevelUp || pstLevelUp->VigUp <= 0)
	{
		return 0;
	}

	return pstLevelUp->VigUp;
}

unsigned int z_get_player_max_vig(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	unsigned int iMaxVig = z_get_max_vig(pstEnv, pstPlayer->stRoleData.Level);
	
	if(pstPlayer->stRoleData.RoleDetail.GreenJade > iMaxVig)
	{
		pstPlayer->stRoleData.RoleDetail.GreenJade = iMaxVig;
	}
	
	return iMaxVig;
}

int z_get_career_basehp(ZONESVRENV* pstEnv, char cCareer, unsigned int ucLevel)
{
	int iHP = 0;
	int iAttrCareer;
	ATTRCONTRASTDEF *pstAttrContrastTiZhi, *pstAttrContrastFangHu;
	LEVELUP *pstLevelUp = NULL;
	LEVELATTRDEF *pstLevelAttrDef;
	LEVELATTR *pstAttr;

	pstLevelUp = z_get_levelup(pstEnv, ucLevel);
	if (!pstLevelUp)
	{
		return 0;
	}

	if(ucLevel <= CAREERCHG_LEVEL)
	{
		iAttrCareer = 1;
	}
	else
	{	
		iAttrCareer = cCareer;
	}

	pstAttrContrastTiZhi = z_find_attr_contrast_def(pstEnv, BASE_ATTR_TYPE_TIZHI, iAttrCareer);
	if(NULL == pstAttrContrastTiZhi)
	{
		return 0;
	}

	pstAttrContrastFangHu = z_find_attr_contrast_def(pstEnv, BASE_ATTR_TYPE_FANGHU, iAttrCareer);
	if(NULL == pstAttrContrastFangHu)
	{
		return 0;
	}

	// 一级属性
	switch( cCareer )
	{
		case CAREER_WARRIOR:
		case CAREER_WARRIOR_SINGLEHAND:	
		case CAREER_WARRIOR_DOUBLEHAND:	
			iHP = pstLevelUp->WarriorTili * pstAttrContrastTiZhi->HP + pstLevelUp->WarriorFangyu*pstAttrContrastFangHu->HP;
			
			break;
		case CAREER_MAGICOR:
		case CAREER_MAGICOR_SINGLEHAND:
		case CAREER_MAGICOR_DOUBLEHAND:
			iHP = pstLevelUp->MagicorTili * pstAttrContrastTiZhi->HP + pstLevelUp->MagicorFangyu*pstAttrContrastFangHu->HP;
			break;
		case CAREER_GUNER:
		case CAREER_GUNER_SINGLEHAND:
		case CAREER_GUNER_DOUBLEHAND:
			iHP = pstLevelUp->GunerTili * pstAttrContrastTiZhi->HP + pstLevelUp->GunerFangyu*pstAttrContrastFangHu->HP;
			break;
		case CAREER_CHURCH:
		case CAREER_CHURCH_SINGLEHAND:
		case CAREER_CHURCH_DOUBLEHAND:
			iHP = pstLevelUp->ChurchTili * pstAttrContrastTiZhi->HP + pstLevelUp->ChurchFangyu*pstAttrContrastFangHu->HP;
			break;
		default:
			return 0;
			break;
	}

	//等级提升2级属性
	pstLevelAttrDef = z_find_level_attr_def(pstEnv, ucLevel);
	if(pstLevelAttrDef)
	{
		pstAttr = get_career_level_attr(pstLevelAttrDef, cCareer);
		if(pstAttr)
		{
			iHP += pstAttr->HP;
		}
	}

	return iHP;
}

int z_get_career_basemp(ZONESVRENV* pstEnv, char cCareer, unsigned char ucLevel)
{
	int iMP = 0;
	int iAttrCareer;
	LEVELUP *pstLevelUp = NULL;
	ATTRCONTRASTDEF *pstAttrContrastJingShen;
	LEVELATTRDEF *pstLevelAttrDef;
	LEVELATTR *pstAttr;

	pstLevelUp = z_get_levelup(pstEnv, ucLevel);
	if (!pstLevelUp)
	{
		return 0;
	}

	if(ucLevel <= CAREERCHG_LEVEL)
	{
		iAttrCareer = 1;
	}
	else
	{	
		iAttrCareer = cCareer;
	}

	pstAttrContrastJingShen = z_find_attr_contrast_def(pstEnv, BASE_ATTR_TYPE_JINGSHEN, iAttrCareer);
	if(NULL == pstAttrContrastJingShen)
	{
		return 0;
	}

	switch( cCareer )
	{
		case CAREER_WARRIOR:
		case CAREER_WARRIOR_SINGLEHAND:
		case CAREER_WARRIOR_DOUBLEHAND:				
			iMP = pstLevelUp->WarriorJingshen * pstAttrContrastJingShen->MP;
			break;
		case CAREER_MAGICOR:
		case CAREER_MAGICOR_SINGLEHAND:
		case CAREER_MAGICOR_DOUBLEHAND:			
			iMP = pstLevelUp->MagicorJingshen * pstAttrContrastJingShen->MP;
			break;
		case CAREER_GUNER:
		case CAREER_GUNER_SINGLEHAND:
		case CAREER_GUNER_DOUBLEHAND:				
			iMP = pstLevelUp->GunerJingshen * pstAttrContrastJingShen->MP;
			break;
		case CAREER_CHURCH:
		case CAREER_CHURCH_SINGLEHAND:
		case CAREER_CHURCH_DOUBLEHAND:			
			iMP = pstLevelUp->ChurchJingshen * pstAttrContrastJingShen->MP;
			break;
		default:
			return 0;
			break;
	}

	//等级提升2级属性
	pstLevelAttrDef = z_find_level_attr_def(pstEnv, ucLevel);
	if(pstLevelAttrDef)
	{
		pstAttr = get_career_level_attr(pstLevelAttrDef, cCareer);
		if(pstAttr)
		{
			iMP += pstAttr->MP;
		}
	}

	return iMP;
}

int z_get_player_hpmax(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	unsigned int iHP;
	STATUSRESULT stResult;
	HEALCHG *pstHealChg = &pstPlayer->stOnline.stDyn.stHealChg;
	UNUSED(pstAppCtx);
	
	iHP = z_dyn_attr_translate(pstEnv, pstPlayer, BASE2ATTR_TYPE_HP);

	iHP += get_player_level_attr(pstEnv, pstPlayer, BASE2ATTR_TYPE_HP);
	
	z_get_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_MAXHP, &stResult, 0);
	iHP = (iHP + pstHealChg->iHPMaxBase + stResult.ResultVal1)*(1+(pstHealChg->nHPMaxMul+stResult.ResultVal2)/100.0+(pstHealChg->iHpMulWanFengLv/10000.0))+
			pstHealChg->nHPMaxAdd + stResult.ResultVal3;
	
	if (iHP > MAX_HP)
	{
		iHP = MAX_HP;
	}
	else if (iHP <= 0)
	{
		iHP = 1;
	}
	
	if (pstPlayer->stRoleData.RoleDetail.HPCurr > iHP)
	{
		pstPlayer->stRoleData.RoleDetail.HPCurr = iHP;
	}
	
	return iHP;
}

int z_get_player_xpmax(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	// xp上限现在改为策划填表了
	/*unsigned int iXp = INIT_XP;

	iXp += pstPlayer->stOnline.stDyn.stHealChg.nMaxXpAdd;

	if (iXp > MAX_XP)
	{
		iXp = MAX_XP;
	}
	else if(iXp < INIT_XP)
	{
		iXp = INIT_XP;
	}

	if (pstPlayer->stRoleData.RoleDetail.XP > iXp)
	{
		pstPlayer->stRoleData.RoleDetail.XP = iXp;
	}
	
	
	return iXp;*/

	int iXp = 0;
	FAIRYDEF *pstFairyDef;
	ROLEFAIRY *pstRoleFairy = NULL;
	int i;
	FAIRYPACKAGE *pstPack = &pstPlayer->stRoleData.Package.FairyPackage;

	for (i=0; i<pstPack->FairyNum; i++)
	{
		if (pstPack->RoleFairys[i].Stat & FAIRY_STAT_CAMP)
		{
			pstRoleFairy = &pstPack->RoleFairys[i];
			break;
		}
	}

	if (!pstRoleFairy)
		return 0;
	
	pstFairyDef = faity_find(pstEnv, pstRoleFairy->FairyID,pstRoleFairy->FairyLevel);
	if (!pstFairyDef)
		return 0;

	iXp =  pstFairyDef->XpUpperLimit;
		
	if ((int)pstPlayer->stRoleData.RoleDetail.XP > iXp)
	{
		pstPlayer->stRoleData.RoleDetail.XP = iXp;
	}

	return iXp;
}

int z_get_player_mpmax(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	unsigned int iMP;
	STATUSRESULT stResult;
	HEALCHG *pstHealChg = &pstPlayer->stOnline.stDyn.stHealChg;
	UNUSED(pstAppCtx);

	iMP = z_dyn_attr_translate(pstEnv, pstPlayer, BASE2ATTR_TYPE_MP);

	iMP += get_player_level_attr(pstEnv, pstPlayer, BASE2ATTR_TYPE_MP);
	
	z_get_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_MAXMP, &stResult, 0);
	iMP = (iMP + pstHealChg->iMPMaxBase + stResult.ResultVal1)*(1+(pstHealChg->nMPMaxMul+stResult.ResultVal2)/100.0)+
			pstHealChg->nMPMaxAdd+stResult.ResultVal3;

	if (iMP > MAX_MP)
	{
		iMP = MAX_MP;
	}
		
	if (pstPlayer->stRoleData.RoleDetail.MPCurr > iMP)
	{
		pstPlayer->stRoleData.RoleDetail.MPCurr = iMP;
	}
	
	return iMP;
}

int z_player_chk_maxmphp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSPLAYERATTRVAL *pstChg = &stPkg.Body.AttrChg;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	unsigned int uiOldHPMax = pstDyn->stMidAttrList.uiHPMax;
	unsigned int uiOldMPMax = pstDyn->stMidAttrList.uiMPMax;

	pstDyn->stMidAttrList.uiHPMax = z_get_player_hpmax(pstAppCtx, pstEnv, pstPlayer);
	pstDyn->stMidAttrList.uiMPMax = z_get_player_mpmax(pstAppCtx, pstEnv, pstPlayer);

	pstChg->AttrNum = 0;
	
	if (uiOldHPMax != pstDyn->stMidAttrList.uiHPMax)
	{
		pstChg->AttrList[pstChg->AttrNum].AttrID = ATTR_ID_HP;
		pstChg->AttrList[pstChg->AttrNum].Value.HP = pstPlayer->stRoleData.RoleDetail.HPCurr;
		pstChg->AttrNum++;
			
		pstChg->AttrList[pstChg->AttrNum].AttrID = ATTR_ID_HPMAX;
		pstChg->AttrList[pstChg->AttrNum].Value.HPMax = pstDyn->stMidAttrList.uiHPMax;
		pstChg->AttrNum++;
	}

	if (uiOldMPMax != pstDyn->stMidAttrList.uiMPMax)
	{
		pstChg->AttrList[pstChg->AttrNum].AttrID = ATTR_ID_MP;
		pstChg->AttrList[pstChg->AttrNum].Value.MP = pstPlayer->stRoleData.RoleDetail.MPCurr;
		pstChg->AttrNum++;
			
		pstChg->AttrList[pstChg->AttrNum].AttrID = ATTR_ID_MPMAX;
		pstChg->AttrList[pstChg->AttrNum].Value.MPMax = pstDyn->stMidAttrList.uiMPMax;
		pstChg->AttrNum++;
	}

	if (pstChg->AttrNum > 0)
	{
		Z_CSHEAD_INIT(&stPkg.Head, ATTR_CHG);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

		//更新最大MP HP
		player_hp_action(pstEnv, pstPlayer, 
					pstPlayer->stRoleData.RoleDetail.HPCurr, 
					pstPlayer->stRoleData.RoleDetail.MPCurr, 0);
	}
	
	return 0;
}

/*int z_get_player_nlmax(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iNL;

	iNL = NEW_NL;
	if (iNL > MAX_NL)
	{
		iNL = MAX_NL;
	}
	
	if (pstPlayer->stRoleData.RoleDetail.NLCurr > iNL)
	{
		pstPlayer->stRoleData.RoleDetail.NLCurr = iNL;
	}
	
	return iNL;
}
*/
int z_get_player_vimmax(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iVim;
	HEALCHG *pstHealChg = &pstPlayer->stOnline.stDyn.stHealChg;
	LEVELUP *pstLvUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	UNUSED(pstAppCtx);
	
	if(NULL == pstLvUp)
	{
		return MAX_VIM;
	}

	// 精力根据等级变化
	iVim = (pstLvUp->VimMax + pstHealChg->nVimMaxBase) * (1+pstHealChg->nVimMaxMul / 100.0) +
	       (pstLvUp->VimAdd + pstHealChg->nVimMaxAdd);
	if (iVim > MAX_VIM)
	{
		iVim = MAX_VIM;
	}
	else if (iVim <= 0)
	{
		iVim = 0;
	}
	
	if (pstPlayer->stRoleData.RoleDetail.VIMCurr > iVim)
	{
		pstPlayer->stRoleData.RoleDetail.VIMCurr = iVim;
	}
	
	return iVim;
}

int z_role_vimmax(ZONESVRENV* pstEnv, int iLevel)
{
	const LEVELUP *pstLvUp = z_get_levelup(pstEnv, iLevel);
	if(!pstLvUp)
	{
		return 0;
	}
	
	return pstLvUp->VimMax + pstLvUp->VimAdd;
}

int z_get_player_spemovemax(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iSpeMove;
	HEALCHG *pstHealChg = &pstPlayer->stOnline.stDyn.stHealChg;
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	
	iSpeMove= (NEW_SPEMOVE+pstHealChg->nSpdMoveMaxBase)*(1+pstHealChg->nSPdMoveMaxMul/100.0)+(pstHealChg->nSPdMoveMaxAdd);

	if (iSpeMove > MAX_SPE_MOVE_POWER)
	{
		iSpeMove = MAX_SPE_MOVE_POWER;
	}
	
	if (pstPlayer->stRoleData.RoleDetail.SpeMovePower > iSpeMove)
	{
		pstPlayer->stRoleData.RoleDetail.SpeMovePower = iSpeMove;
	}
	
	return iSpeMove;
}

int z_get_player_spdfly(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iFlyMove;
	HEALCHG *pstHealChg = &pstPlayer->stOnline.stDyn.stHealChg;
	int iMspd;
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);

	if (pstPlayer->stRoleData.RoleDetail.SpeMovePower <= 0)
	{
		iMspd = NEW_FLY_SLOW;
	}
	else
	{
		iMspd = NEW_FLY_SPD;
	}

	iFlyMove= (iMspd+pstHealChg->nSpdFlyBase)*(1+pstHealChg->nSPdFlyMul/100.0)+(pstHealChg->nSPdFlyAdd);
	
	return iFlyMove;
}

int z_get_player_mspd(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iMspd;
	STATUSRESULT stResult;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;
	HEALCHG *pstHealChg = &pstPlayer->stOnline.stDyn.stHealChg;

	iMspd = BASE_MSPD;
	//天赋对速度的修正
	iMspd = iMspd * (1 + (pstHealChg->nSpeedMul%100)/100.0) + pstHealChg->nSpeedAdd;
	
	//搭车
	if ( CS_STAT_HITCH_RIDE & pstPlayer->stOnline.State )
	{
		Player *pstRidePlayer;

		pstRidePlayer = z_id_player(pstAppCtx, pstEnv, pstPlayer->stOnline.stRideOnInfo.iOwnerID);
		if(pstRidePlayer && pstRidePlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN && 
			(!(CS_STAT_HITCH_RIDE & pstRidePlayer->stOnline.State)))
		{
			return z_get_player_mspd(pstAppCtx, pstEnv, pstRidePlayer);
		}
	}
	else if (CS_STAT_RIDE_MON_MACHINE & pstPlayer->stOnline.State)
	{
		Monster *pstMon;
		MONSTERDEF *pstMonDef = NULL;

		pstMon = z_id_monster(pstAppCtx, pstEnv, pstPlayer->stOnline.stMachineInfo.iMachineMonID);
		if(pstMon)
		{
			pstMonDef = z_get_mon_def(pstEnv, pstMon);
			if(pstMonDef)
			{
				return z_get_mon_spd(pstAppCtx, pstEnv, pstMon, pstMonDef);
			}	
		}
	}

	if ( (CS_STAT_RIDE_ON & pstPlayer->stOnline.State) || (CS_STAT_FLY & pstPlayer->stOnline.State))
	{
		RIDEDEF *pstRideDef;

		pstRideDef = z_find_ride_def(pstEnv, pstPlayer->stOnline.RideItemID, pstPlayer->stOnline.RideLevel);
		if (pstRideDef)
		{
			iMspd += pstRideDef->RideSpd;
			iMspd = 	(iMspd + pstHealChg->nSpdRideBase)* (1+pstHealChg->nSPdRideMul/100.0)+pstHealChg->nSPdRideAdd;
		}	
	}
	
	if (pstRoleTmpSys->TmpItemID > 0)
	{
		ITEMDEF *pstItemDef;

		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
		if (pstItemDef && (pstItemDef->ItemTmp.SpeedMul != 0 || pstItemDef->ItemTmp.SpeedAdd != 0))
		{
			iMspd = iMspd * (1 + pstItemDef->ItemTmp.SpeedMul/100.0) + pstItemDef->ItemTmp.SpeedAdd;
		}

		if( pstItemDef && ITEM_TMP_MACHINE == pstItemDef->ItemTmp.Type)
		{
			genius_add_tmpmachine_spd(pstAppCtx, pstEnv, pstPlayer,pstItemDef,&iMspd);
		}
	}

	z_get_player_status_result_min(pstEnv, pstPlayer, STATUS_RESULT_MSPD, &iMspd);
	
	memset(&stResult,0,sizeof(stResult));
	z_get_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_STEP_DEC_MSPD, &stResult, 0);
	iMspd = iMspd * (1 + stResult.ResultVal1/100.0) + stResult.ResultVal2;

	if (iMspd < 0)
		iMspd = 0;
	else if (iMspd > MAX_MSPD)
		iMspd = MAX_MSPD;
	
	return iMspd;
}

int z_get_mon_spd(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster* pstMon, MONSTERDEF *pstMonDef)
{
	STATUSRESULT stResult;
	int iMspd;
	UNUSED(pstAppCtx);
	
	switch(pstMon->bAiStat)
	{
		case AI_STAT_MOVE:
			iMspd = pstMonDef->MoveSpd;
			break;
		case AI_STAT_ATK:
		case AI_STAT_TRACE:
		case AI_STAT_RUNAWAY:
			iMspd = pstMonDef->FightMoveSpd;
			if (pstMon->stBctInfo.iBctDefID > 0 &&
				pstMon->bSubType <= MON_SUB_TYPE_JI)
			{
				iMspd = 600;  // 地心中的怪，只要是战斗状态移动速度就是600
			}
			break;
		case AI_STAT_HOME:
			iMspd = pstMonDef->HomeMoveSpd;
			if (iMspd > (int)pstMonDef->MoveSpd)
			{
				return iMspd;
			}
			break;
		default:
			iMspd = 0;
			break;
	}

	if (z_is_mon_status_result(pstEnv, pstMon, STATUS_RESULT_RANDOM_RUN) 
								&& pstMonDef->FightMoveSpd > pstMonDef->MoveSpd)
	{
		iMspd = pstMonDef->FightMoveSpd;
	}

	if(pstMon->bSubType == MON_SUB_TYPE_MACHINE)
	{
		if(this_machine(pstEnv, pstMon)->stMachineInfo.Energy <= 0)
		{
			iMspd = pstMonDef->MoveSpd*0.6;
		}
		else
		{
			iMspd = pstMonDef->MoveSpd;
		}
	}
	
	z_get_mon_status_result_min(pstEnv, pstMon, STATUS_RESULT_MSPD, &iMspd);
	
	memset(&stResult,0,sizeof(stResult));
	z_get_mon_status_result(pstEnv, pstMon, STATUS_RESULT_STEP_DEC_MSPD, &stResult);
	iMspd = iMspd * (1 + stResult.ResultVal1/100.0) + stResult.ResultVal2;

	if (iMspd < 0)
		iMspd = 0;
	else if (iMspd > MAX_MSPD)
		iMspd = MAX_MSPD;

	if( pstMon->bSubType == MON_SUB_TYPE_XUNLUO &&
		pstMon->iCommType == MON_UNION_TYPE_XUNLUO_MON &&
		pstMon->stCommUnion.stXunLuoMon.a != 0 &&
		pstMon->stForcedPos.X >0 &&
		pstMon->stForcedPos.Y >0)
	{
		//通过距离算物理速度
		int iDist = 0;
		float ftime = 0;
		iDist = z_distance(&pstMon->stCurrPos, &pstMon->stForcedPos);
		ftime = z_move_get_t_time(0,pstMon->stCommUnion.stXunLuoMon.a,iDist);
		iMspd = z_move_get_v_spd(0,pstMon->stCommUnion.stXunLuoMon.a,ftime);
	}
	
	return iMspd;
}

//规则是所有速度变化延迟500ms处理，速度变化了则使用action 消息通知
int z_mon_chk_mspd(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster* pstMon, int iNotify)
{
	unsigned short unMspd;
	CSPKG stPkg;
	MONSTERDEF *pstMonDef;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	int iChgFlag = 0;
	CSMSPDINFO *pstMspdInfo = &pstMon->stMspdInfo;

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (NULL == pstMonDef)
	{
		return -1;
	}
	
	unMspd = z_get_mon_spd(pstAppCtx, pstEnv, pstMon, pstMonDef);

	//同时更新机械上的人物速度
	if(is_mon_machine(pstMon))
	{
		int i;
		Player *pstPlayer;

		for(i=0; i<MAX_MON_MACHINE_PASSENGER; i++)
		{
			if(i >= this_machine(pstEnv,pstMon) ->stMachineInfo.MaxPassenger)
			{
				break;
			}

			if(this_machine(pstEnv,pstMon) ->stMachineInfo.PassengerID[i] <= 0)
			{
				continue;
			}

			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, this_machine(pstEnv,pstMon) ->stMachineInfo.PassengerID[i] );
			if (pstPlayer == NULL || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				continue;
			}

			z_player_chk_mspd(pstAppCtx, pstEnv, pstPlayer);
		}
	}

	if (pstMspdInfo->NextNum > 0)
	{
		if (unMspd != pstMspdInfo->MoveSpdNext.MoveSpd)
			iChgFlag = 1;
	}
	else
	{
		if (unMspd != pstMspdInfo->MoveSpdCurr)
			iChgFlag = 1;
	}

	if (!iChgFlag)
		return 0;

	pstMspdInfo->NextNum = 1;
	pstMspdInfo->MoveSpdNext.MoveSpd = unMspd;
	pstMspdInfo->MoveSpdNext.SvrTime.BenchTimeUSec = pstAppCtx->stCurr.tv_usec + MSPD_DELAY;
	if (pstMspdInfo->MoveSpdNext.SvrTime.BenchTimeUSec >= 1000000)
	{
		pstMspdInfo->MoveSpdNext.SvrTime.BenchTimeUSec -= 1000000;
		pstMspdInfo->MoveSpdNext.SvrTime.BenchTimeSec = pstAppCtx->stCurr.tv_sec + 1;
	}
	else
	{
		pstMspdInfo->MoveSpdNext.SvrTime.BenchTimeSec = pstAppCtx->stCurr.tv_sec;
	}

	if (iNotify)
	{
		pstActionRes->ID = pstMon->iID;
		pstActionRes->ActionID = ACTION_MSPD_CHG;
		memcpy(&pstActionRes->Data.MspdChg, pstMspdInfo, sizeof(pstActionRes->Data.MspdChg));
		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);

		z_sendpkg_monview(pstAppCtx, pstEnv, pstMon, &stPkg, 0);
	}

	return 0;
}

//规则是所有速度变化延迟500ms处理，速度变化了则使用action 消息通知
int z_player_chk_mspd(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	unsigned short unMspd;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	int iChgFlag = 0;
	CSMSPDINFO *pstMspdInfo = &pstPlayer->stOnline.stMspdInfo;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;
	
	unMspd = z_get_player_mspd(pstAppCtx, pstEnv, pstPlayer);

	if (pstMspdInfo->NextNum > 0)
	{
		if (unMspd != pstMspdInfo->MoveSpdNext.MoveSpd)
			iChgFlag = 1;
	}
	else
	{
		if (unMspd != pstMspdInfo->MoveSpdCurr)
			iChgFlag = 1;
	}



	if (!iChgFlag)
		return 0;

	pstMspdInfo->NextNum = 1;
	pstMspdInfo->MoveSpdNext.MoveSpd = unMspd;
	pstMspdInfo->MoveSpdNext.SvrTime.BenchTimeUSec = pstAppCtx->stCurr.tv_usec + MSPD_DELAY;
	if (pstMspdInfo->MoveSpdNext.SvrTime.BenchTimeUSec >= 1000000)
	{
		pstMspdInfo->MoveSpdNext.SvrTime.BenchTimeUSec -= 1000000;
		pstMspdInfo->MoveSpdNext.SvrTime.BenchTimeSec = pstAppCtx->stCurr.tv_sec + 1;
	}
	else
	{
		pstMspdInfo->MoveSpdNext.SvrTime.BenchTimeSec = pstAppCtx->stCurr.tv_sec;
	}

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_MSPD_CHG;
	memcpy(&pstActionRes->Data.MspdChg, pstMspdInfo, sizeof(pstActionRes->Data.MspdChg));
	
	if(pstRoleTmpSys->TmpItemID != 0 && 
		pstRoleTmpSys->PatrolID > 0 )
	{
		//玩家跑快些
		PATROLDEF *pstPatrolDef = NULL;
		pstPatrolDef = z_find_patroldef(pstEnv, pstRoleTmpSys->PatrolID, NULL);
		if (NULL != pstPatrolDef)
		{
			pstActionRes->Data.MspdChg.NextNum = 1;
			if(pstActionRes->Data.MspdChg.MoveSpdNext.MoveSpd)
			{
				pstActionRes->Data.MspdChg.MoveSpdNext.MoveSpd *= (1+pstPatrolDef->CltSpeedMul/1000.0); 
			}
			else
			{
				pstActionRes->Data.MspdChg.MoveSpdNext.MoveSpd = 
					pstActionRes->Data.MspdChg.MoveSpdCurr * (1+pstPatrolDef->CltSpeedMul/1000.0);
			}
		}
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);

	if(((CS_STAT_RIDE_ON |CS_STAT_FLY) & pstPlayer->stOnline.State) && pstPlayer->stOnline.stRideOnInfo.iPassengerNum )
	{
		int i;
		Player *pstTmpPlayer;
		
		//同步乘客速度
		for(i=0; i<pstPlayer->stOnline.stRideOnInfo.iPassengerNum; i++)
		{
			pstTmpPlayer = z_id_player(pstAppCtx, pstEnv, pstPlayer->stOnline.stRideOnInfo.PassengerID[i]);
			if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				continue;
			}

			pstMspdInfo = &pstTmpPlayer->stOnline.stMspdInfo;
			
			pstMspdInfo->NextNum = 1;
			pstMspdInfo->MoveSpdNext.MoveSpd = unMspd;
			pstMspdInfo->MoveSpdNext.SvrTime.BenchTimeUSec = pstAppCtx->stCurr.tv_usec + MSPD_DELAY;
			if (pstMspdInfo->MoveSpdNext.SvrTime.BenchTimeUSec >= 1000000)
			{
				pstMspdInfo->MoveSpdNext.SvrTime.BenchTimeUSec -= 1000000;
				pstMspdInfo->MoveSpdNext.SvrTime.BenchTimeSec = pstAppCtx->stCurr.tv_sec + 1;
			}
			else
			{
				pstMspdInfo->MoveSpdNext.SvrTime.BenchTimeSec = pstAppCtx->stCurr.tv_sec;
			}
		}	
	}
		
	return 0;
}

int z_is_weapon(ITEMDEF *pstItemDef, char cCareer)
{
	UNUSED(&cCareer);
	// 128是枪手的左手装备,不是武器
	if (pstItemDef->NumType < 100 || pstItemDef->NumType > 159 || pstItemDef->NumType == 106 || pstItemDef->NumType == 118 ||
		pstItemDef->NumType == 158 )
	{
		return 0;
	}

	return 1;
}

int z_arm_skill_levelup(DYNATTRLIST *pstDyn, unsigned short unSkillID, unsigned char ucLevel)
{
	int i;

	for (i = 0; i <MAX_ARM_SKILL; i++)
	{
		if ( pstDyn->astArmSkill[i].unSkillID == 0  ||  pstDyn->astArmSkill[i].ucLevel == 0)
		{
			break;
		}

		if (unSkillID == pstDyn->astArmSkill[i].unSkillID)
		{
			pstDyn->astArmSkill[i].ucLevel += ucLevel;
			return 0;
		}
	}

	if (i < MAX_ARM_SKILL)
	{
		pstDyn->astArmSkill[i].unSkillID = unSkillID;
		pstDyn->astArmSkill[i].ucLevel = ucLevel;
	}

	return 0;
}

int is_in_arm_add_skill(DYNATTRLIST *pstDyn, unsigned short unSkillID, unsigned char ucLevel)
{	
	int i;
	
	for (i = 0; i <MAX_ARM_ADD_SKILL; i++)
	{
		if ( pstDyn->astAddSkill[i].unSkillID == 0  ||  pstDyn->astAddSkill[i].ucLevel == 0)
		{
			break;
		}

		if (unSkillID == pstDyn->astAddSkill[i].unSkillID && 
			ucLevel == pstDyn->astAddSkill[i].ucLevel)
		{
			return 1;
		}
	}

	return 0;
}

int z_arm_add_skill(ZONESVRENV* pstEnv, DYNATTRLIST *pstDyn, unsigned short unSkillID, unsigned char ucLevel)
{
	int i;
	SKILLDEF *pstSkillDef;

	pstSkillDef = z_find_skill(pstEnv, unSkillID, ucLevel, NULL);
	if (NULL == pstSkillDef || pstSkillDef->BindSkill != BINDSKILL_TYPE_ARM)
	{
		return -1;
	}

	for (i = 0; i <MAX_ARM_ADD_SKILL; i++)
	{
		if ( pstDyn->astAddSkill[i].unSkillID == 0  ||  pstDyn->astAddSkill[i].ucLevel == 0)
		{
			break;
		}

		if (unSkillID == pstDyn->astAddSkill[i].unSkillID )
		{
			if(pstDyn->astAddSkill[i].ucLevel < ucLevel)
			{
				pstDyn->astArmSkill[i].ucLevel = ucLevel;
			}
			
			return 0;
		}
	}

	if (i < MAX_ARM_ADD_SKILL)
	{
		pstDyn->astAddSkill[i].unSkillID = unSkillID;
		pstDyn->astAddSkill[i].ucLevel = ucLevel;
	}

	return 0;
}

int z_arm_add_skill_update(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	SKILLDEF *pstSkillDef;
	ZoneSkill *pstZoneSkill;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;
	CSPKG stPkg;
	CSSKILLINFO *pstSkillInfo = &stPkg.Body.SkillInfo;

	//更新删除无效的附加技能
	for (i=pstZoneSkillList->nSkillNum-1; i>=0; i--)
	{
		pstZoneSkill = &pstZoneSkillList->astZoneSkills[i];
		pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill);
		if (NULL == pstSkillDef || pstSkillDef->BindSkill != BINDSKILL_TYPE_ARM)
		{
			continue;
		}

		if(!is_in_arm_add_skill(pstDyn, pstZoneSkill->stSkillData.SkillID, pstZoneSkill->stSkillData.SkillLevel))
		{
			pstSkillInfo->Type = SKILL_INFO_DEL;
			pstSkillInfo->Data.SkillDelID = pstZoneSkill->stSkillData.SkillID;

			player_unwear_skill_cool_update(pstEnv->pstAppCtx, pstEnv,pstPlayer,pstZoneSkill);
			
			Z_CSHEAD_INIT(&stPkg.Head, SKILL_INFO);
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
			
			pstZoneSkillList->nSkillNum--;
			if(i != pstZoneSkillList->nSkillNum)
			{
				memcpy(pstZoneSkill, pstZoneSkillList->astZoneSkills+pstZoneSkillList->nSkillNum, sizeof(*pstZoneSkill));
			}
		}
	}
	
	//装备附加技能
	for (i = 0; i <MAX_ARM_ADD_SKILL; i++)
	{
		if (pstDyn->astAddSkill[i].unSkillID == 0 ||pstDyn->astAddSkill[i].ucLevel == 0)
		{
			break;
		}

		z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, 
			pstDyn->astAddSkill[i].unSkillID, pstDyn->astAddSkill[i].ucLevel, 1);
	}
	
	return 0;
}

int z_add_arm_addattr(ITEMDEF *pstItemDef, ROLEARM *pstRoleArm, int iAddAttrID, int iAddAttrIdx, ARMRESULT *pstResult)
{
	int iMaxVal, iMinVal;
	int iVal1;
	UNUSED(pstItemDef);

	if (pstRoleArm->AddAttrNum >= MAX_ARM_ADD_ATTR)
	{
		//装不下了
		return -1;
	}

	iMinVal = pstResult->ResultVal1;
	iMaxVal = pstResult->ResultVal2;
	//范围随即
	if (iMaxVal > iMinVal)
	{
		iVal1 = iMinVal + RAND1(iMaxVal-iMinVal+1);
	}
	//固定值
	else
	{
		iVal1 = iMinVal;
	}

	//特殊的关键字，只记录对应索引，计算时在取
	if (iAddAttrID == ADD_ATTR_RESULT_SKILL_LEVEL || 
		iAddAttrID == ADD_ATTR_RESULT_ADD_SKILL )
	{
		iVal1 = iAddAttrIdx;
	}

	//记录到实例信息中
	pstRoleArm->AddAttr[pstRoleArm->AddAttrNum].AddAttrID = iAddAttrID;
	pstRoleArm->AddAttr[pstRoleArm->AddAttrNum].Val =  iVal1;
	pstRoleArm->AddAttrNum ++;
	
	return 0;
}

int get_rand_addattr_level(RANDADDATTR *pstRandAddAttr)
{
	int i;
	int iRate, iRnd;
	
	iRate = 0;
	iRnd = (int)RAND1(10000);
	for (i=0; i<MAX_RAND_ADD_ATTR_LEVEL; i++)	
	{
		if(pstRandAddAttr->AddAttrLevel[i].Rate == 0 ||
			pstRandAddAttr->AddAttrLevel[i].Val== 0)
		{
			break;
		}
		
		iRate += pstRandAddAttr->AddAttrLevel[i].Rate;
		if (iRnd < iRate*100)
		{
			return pstRandAddAttr->AddAttrLevel[i].Val;
		}
	}

	return 0;
}

int z_add_rand_addattr(ZONESVRENV* pstEnv,ITEMDEF *pstItemDef, ROLEARM *pstRoleArm, int iAddAttrID, int iLevel)
{
	ARMADDATTRDEF *pstArmAddAttr;
	
	pstArmAddAttr = z_find_armaddattrdef(pstEnv, iAddAttrID);
	if (NULL == pstArmAddAttr)
	{
		return -1;
	}

	if (iLevel == 0 || iLevel > MAX_ARM_ADD_ATTR_RESULT)
	{
		return -1;
	}

	return z_add_arm_addattr(pstItemDef, pstRoleArm, iAddAttrID, iLevel, &pstArmAddAttr->Result[iLevel-1]);
}

int z_rand_create_addattr(ZONESVRENV* pstEnv, ITEMDEF *pstItemDef, ROLEARM *pstRoleArm)
{
	int i, j;
	int iRate, iRateBase, iRnd;
	int iAddAttrNum;
	int iAddAttrLevel;
	char sAddAttrMap[MAX_RAND_ADD_ATTR];
	ADDATTRGROUPDEF *pstAddAttrGroupDef;

	pstAddAttrGroupDef = z_find_addattr_group_def(pstEnv, pstItemDef->AddAttrGroupID);
	if(NULL == pstAddAttrGroupDef)
	{
		return -1;
	}

	iRate = 0;
	iAddAttrNum = 0;
	iRnd = (int)RAND1(10000);

	//确定关键字总数
	for (i=0; i<MAX_RAND_ADD_ATTR_SUM; i++)	
	{
		if(pstAddAttrGroupDef->AddAttSum[i].Rate == 0 ||
			pstAddAttrGroupDef->AddAttSum[i].Val== 0)
		{
			break;
		}

		iRate += pstAddAttrGroupDef->AddAttSum[i].Rate;
		if (iRnd < iRate*100)
		{
			iAddAttrNum = pstAddAttrGroupDef->AddAttSum[i].Val;
			break;
		}
	}

	//如果固定和随机关键字组合。先出去固定关键字已经占据的数目
	iAddAttrNum -= pstRoleArm->AddAttrNum;

	iRateBase = 100;
	memset(sAddAttrMap, 0, sizeof(sAddAttrMap));
	for (i=0; i<iAddAttrNum; i++)
	{
		iRate = 0;
		iRnd = (int)RAND1(iRateBase*100);
		
		//没位置了
		if(pstRoleArm->AddAttrNum >= MAX_ARM_ADD_ATTR)
		{
			break;
		}

		//获得一个关键字
		for (j=0; j<MAX_RAND_ADD_ATTR; j++)
		{
			if(pstAddAttrGroupDef->AddAtt[j].AddAttrID == 0 ||
				pstAddAttrGroupDef->AddAtt[j].Rate == 0 || 
				iRateBase <= 0)
			{
				break;
			}

			//曾经获得过的排除掉
			if (sAddAttrMap[j])
			{
				continue;
			}

			iRate += pstAddAttrGroupDef->AddAtt[j].Rate;
			if(iRnd < iRate*100)
			{
				//不可 重复获得
				sAddAttrMap[j] = 1;

				iRateBase -= pstAddAttrGroupDef->AddAtt[j].Rate;

				iAddAttrLevel = get_rand_addattr_level(&pstAddAttrGroupDef->AddAtt[j]);
				if(iAddAttrLevel)
				{
					z_add_rand_addattr(pstEnv, pstItemDef, pstRoleArm, pstAddAttrGroupDef->AddAtt[j].AddAttrID, iAddAttrLevel);
				}

				break;
			}
		}
	}

	return 0;
}

int z_create_arm_addattr(ZONESVRENV* pstEnv, ITEMDEF *pstItemDef, ROLEARM *pstRoleArm)
{
	int i;
	ARMADDATTRDEF *pstArmAddAttr;

	for (i=0; i<MAX_ITEM_ADD_ATTR; i++)	
	{
		//没位置了
		if(pstRoleArm->AddAttrNum >= MAX_ARM_ADD_ATTR)
		{
			break;
		}
		
		if (0 == pstItemDef->AddAttr[i].ID)
		{
			break;
		}

		pstArmAddAttr = z_find_armaddattrdef(pstEnv, pstItemDef->AddAttr[i].ID);
		if (!pstArmAddAttr)
		{
			continue;
		}

		if (pstItemDef->AddAttr[i].Idx == 0 || pstItemDef->AddAttr[i].Idx > MAX_ARM_ADD_ATTR_RESULT)
		{
			continue;
		}

		z_add_arm_addattr(pstItemDef, pstRoleArm, pstItemDef->AddAttr[i].ID, 
								pstItemDef->AddAttr[i].Idx, &pstArmAddAttr->Result[pstItemDef->AddAttr[i].Idx-1]);
	}

	return 0;
}

int z_arm_add_attr_result(ZONESVRENV* pstEnv, Player *pstPlayer, DYNATTRLIST *pstDyn, 
	ITEMDEF *pstItemDef, int iAddAttrID, int iVal)
{
	int iAddVal;
	ARMLEVELHURTDEF *pstArmLevelHurt;
	ARMADDATTRDEF *pstArmAddAttrDef;

	ADDATTRBASEDEF *pstAddAttrBaseDef;

	pstAddAttrBaseDef = z_find_addattr_base_def(pstEnv, pstItemDef->ItemLevel);
	if(NULL == pstAddAttrBaseDef)
	{
		return -1;
	}
	
	switch(iAddAttrID)
	{
		case ADD_ATTR_RESULT_ATK:
		case ADD_ATTR_RESULT_MIN_ATK:
		case ADD_ATTR_RESULT_ENERGE_ATK:
		case ADD_ATTR_RESULT_CANNON_ATK:
			iAddVal = pstAddAttrBaseDef->IceAtkBase*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stAtkChg.unIceBaseArmAtk += iAddVal;

			iAddVal = pstAddAttrBaseDef->PhsicAtkBase*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stAtkChg.unPhsicBaseArmAtk += iAddVal;
			break;
		case ADD_ATTR_RESULT_PHSIC_ATK:
			iAddVal = pstAddAttrBaseDef->PhsicAtkBase*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}	
			pstDyn->stAtkChg.unPhsicBaseArmAtk += iAddVal;
			break;
		case ADD_ATTR_RESULT_ICE_ATK:
			iAddVal = pstAddAttrBaseDef->IceAtkBase*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}	
			pstDyn->stAtkChg.unIceBaseArmAtk += iAddVal;
			break;
		case ADD_ATTR_RESULT_HP:
		case ADD_ATTR_RESULT_MIN_HP:
			iAddVal = pstAddAttrBaseDef->MaxHPBase*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}	
			pstDyn->stHealChg.iHPMaxBase += iAddVal;
			break;
		case ADD_ATTR_RESULT_MP:
		case ADD_ATTR_RESULT_MIN_MP:
			iAddVal = pstAddAttrBaseDef->MaxMPBase*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}	
			pstDyn->stHealChg.iMPMaxBase += iAddVal;
			break;
		case ADD_ATTR_RESULT_DAMAGE:
			pstArmLevelHurt = z_find_armlevelhurt(pstEnv, pstItemDef->NumType, pstPlayer->stRoleData.Level);
			if (pstArmLevelHurt)
			{
				pstDyn->stAtkChg.nDamageMul += iVal;
			}
			break;	
		case ADD_ATTR_RESULT_MIN_EXP:
		case ADD_ATTR_RESULT_EXP:
			pstDyn->stHealChg.nArmExpMul += iVal;
			break;
		case ADD_ATTR_RESULT_PHSIC_DEF:
			iAddVal = pstAddAttrBaseDef->PhsicDefBase*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stDefChg.unPhsicDefBaseAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_ICE_DEF:
			iAddVal = pstAddAttrBaseDef->IceDefBase*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stDefChg.unIceDefBaseAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_ADD_DEF:
			iAddVal = pstAddAttrBaseDef->PhsicDefBase*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stDefChg.unPhsicDefBaseAdd += iAddVal;
			
			iAddVal = pstAddAttrBaseDef->IceDefBase*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stDefChg.unIceDefBaseAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_DEF_PHISICHURT:
			iAddVal = pstAddAttrBaseDef->DefPhsicHurtBase*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stAtkeeHurtChg.AtkeePhsicHurtAdd += iAddVal;
			break;	
		case ADD_ATTR_RESULT_DEF_ICEHURT:
			iAddVal = pstAddAttrBaseDef->DefIceHurtBase*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stAtkeeHurtChg.AtkeeIceHurtAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_DEF_ICE_PHISIC:
			iAddVal = pstAddAttrBaseDef->DefPhsicHurtBase*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stAtkeeHurtChg.AtkeePhsicHurtAdd +=iAddVal;
			iAddVal = pstAddAttrBaseDef->DefIceHurtBase*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stAtkeeHurtChg.AtkeeIceHurtAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_FLEE:
			iAddVal = pstAddAttrBaseDef->FleeBase*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stFleeChg.unFleeBaseAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_FIGHTVAL:
			pstDyn->stMidAttrList.uiFight += iVal;
			break;
		case ADD_ATTR_RESULT_HPADD:
			iAddVal = pstAddAttrBaseDef->MaxHPBase*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stHealChg.unArmHPAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_MPADD:
			iAddVal = pstAddAttrBaseDef->MaxMPBase*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stHealChg.unArmMPAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_NOFIGHT_HPADD:
			iAddVal = pstAddAttrBaseDef->MaxHPBase*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stHealChg.unNoFightRestoreHPAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_NOFIGHT_MPADD:
			iAddVal = pstAddAttrBaseDef->MaxMPBase*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stHealChg.unNoFightRestoreMPAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_FIGHT_HPADD:
			pstDyn->stHealChg.unFightRestoreHPAdd += iVal;
			break;
		case ADD_ATTR_RESULT_FIGHT_MPADD:
			pstDyn->stHealChg.unFightRestoreMPAdd += iVal;
			break;
		case ADD_ATTR_RESULT_SKILL_LEVEL:
			pstArmAddAttrDef = z_find_armaddattrdef(pstEnv, iAddAttrID);
			if (pstArmAddAttrDef)
			{
				if (iVal > 0 &&  iVal <  MAX_ARM_ADD_ATTR_RESULT)
				{
					z_arm_skill_levelup(pstDyn, pstArmAddAttrDef->Result[iVal].ResultVal1, pstArmAddAttrDef->Result[iVal].ResultVal2);
				}
			}
			break;
		case ADD_ATTR_RESULT_BADSTATUS_RATE:
			pstDyn->stHealChg.ucBadStatusRate += iVal;
			break;
		case ADD_ATTR_RESULT_BADSTATUS_DEF:
			pstDyn->stHealChg.ucBadStatusDefRate += iVal;
			break;
		case ADD_ATTR_RESULT_BADSTATUS_DEC_TIME:
			pstDyn->stHealChg.unBadStatusDecTime += iVal;
			break;
		case ADD_ATTR_RESULT_HEAVY_HIT:
			pstDyn->stHitRateChg.nHeavyHitAdd += iVal;
			break;
		case ADD_ATTR_RESULT_HITRATE:
			iAddVal = pstAddAttrBaseDef->HitRateBase*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stHitChg.iHitChgAdd+= iAddVal;
			break;
		case ADD_ATTR_RESULT_HITRATE_RATE:
			pstDyn->stHitRateChg.nAtkerHitRateAdd += iVal;
			break;
		case ADD_ATTR_RESULT_TASK_ITEM_RATE:
			if (pstDyn->stHealChg.ucTaskItemRate+iVal> 100)
			{
				pstDyn->stHealChg.ucTaskItemRate = 100;
			}
			else
			{
				pstDyn->stHealChg.ucTaskItemRate+= iVal;
			}
			break;
		case ADD_ATTR_RESULT_STUFF_ITEM_RATE:
			if (pstDyn->stHealChg.ucStuffItemRate+iVal> 100)
			{
				pstDyn->stHealChg.ucStuffItemRate = 100;
			}
			else
			{
				pstDyn->stHealChg.ucStuffItemRate+= iVal;
			}
			break;
		case ADD_ATTR_RESULT_EGG_RATE:
			if (pstDyn->stHealChg.ucEggRate+iVal> 100)
			{
				pstDyn->stHealChg.ucEggRate = 100;
			}
			else
			{
				pstDyn->stHealChg.ucEggRate+= iVal;
			}
			break;
		case ADD_ATTR_RESULT_ADD_BULL:
			if (pstDyn->stHealChg.ucBullRate+iVal> 100)
			{
				pstDyn->stHealChg.ucBullRate = 100;
			}
			else
			{
				pstDyn->stHealChg.ucBullRate+= iVal;
			}
			break;
		case ADD_ATTR_RESULT_ADD_PET_EXP:
			pstDyn->stHealChg.unPetExpMul += iVal;
			break;
		case ADD_ATTR_RESULT_ADD_SKILLPOINT:
			pstDyn->stHealChg.unSkillPonitMul += iVal;
			break;
		case ADD_ATTR_RESULT_HEAVY_HIT_VAL:
			pstDyn->stHitRateChg.nHeavyHitVal += iVal*100;
			break;
		case ADD_ATTR_RESULT_BASE_ATTR_HEAVYHIT:
			iAddVal = pstAddAttrBaseDef->HeavyHit*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stHitRateChg.iHeavyHit += iAddVal;
			break;
		case ADD_ATTR_RESULT_DEF_HEAVY_HIT:
			iAddVal = pstAddAttrBaseDef->DefHeavyHit*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stHitRateChg.iDefHeavyHit += iAddVal;
			break;

		case ADD_ATTR_RESULT_LILIANG:
			iAddVal = pstAddAttrBaseDef->LiLiang*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stMidAttrList.iLiliangAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_ZHILI:
			iAddVal = pstAddAttrBaseDef->Zhili*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stMidAttrList.iZhiliAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_TIZHI:
			iAddVal = pstAddAttrBaseDef->TiZhi*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stMidAttrList.iTiliAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_FANGHU:
			iAddVal = pstAddAttrBaseDef->FangHu*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stMidAttrList.iFangyuAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_JINGSHEN:
			iAddVal = pstAddAttrBaseDef->JingShen*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stMidAttrList.iJingshenAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_MINJIE:
			iAddVal = pstAddAttrBaseDef->MinJie*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stMidAttrList.iMinJieAdd += iAddVal;
			break;
		case ADD_ATTR_RESULT_WATERATK:
			pstDyn->stAtkChg.unWaterBaseArmAtk += iVal;
			break;
		case ADD_ATTR_RESULT_FIREATK:
			pstDyn->stAtkChg.unFireBaseArmAtk += iVal;
			break;
		case ADD_ATTR_RESULT_THUNDERATK:
			pstDyn->stAtkChg.unThunderBaseArmAtk += iVal;
			break;
		case ADD_ATTR_RESULT_LIGHTATK:
			pstDyn->stAtkChg.unLightBaseArmAtk += iVal;
			break;
		case ADD_ATTR_RESULT_NIGHTATK:
			pstDyn->stAtkChg.unNightBaseArmAtk += iVal;
			break;
		case ADD_ATTR_RESULT_WATERDEF:
			/*iAddVal = pstAddAttrBaseDef->WaterDef*iVal/100.0;
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stDefChg.unWaterDefBaseAdd += iAddVal;*/
			pstDyn->stDefChg.unWaterDefBaseAdd += iVal;
			break;
		case ADD_ATTR_RESULT_FIREDEF:
			/*iAddVal = pstAddAttrBaseDef->FireDef*iVal/100.0;
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stDefChg.unFireDefBaseAdd += iAddVal;*/
			pstDyn->stDefChg.unFireDefBaseAdd += iVal;
			break;
		case ADD_ATTR_RESULT_THUNDERDEF:
			/*iAddVal = pstAddAttrBaseDef->ThunderDef*iVal/100.0;
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stDefChg.unThunderDefBaseAdd += iAddVal;*/
			pstDyn->stDefChg.unThunderDefBaseAdd += iVal;
			break;
		case ADD_ATTR_RESULT_LIGHTDEF:
			/*iAddVal = pstAddAttrBaseDef->LightDef*iVal/100.0;
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stDefChg.unLightDefBaseAdd += iAddVal;*/
			pstDyn->stDefChg.unLightDefBaseAdd += iVal;
			break;
		case ADD_ATTR_RESULT_NIGHTDEF:
			/*iAddVal = pstAddAttrBaseDef->NightDef*iVal/100.0;
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stDefChg.unNightDefBaseAdd += iAddVal;*/
			pstDyn->stDefChg.unNightDefBaseAdd += iVal;
			break;
		case ADD_ATTR_RESULT_REFLECTION_HP:
			iAddVal = pstAddAttrBaseDef->ReflectHP*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stDefChg.iReflectHP += iAddVal;
			break;
		case ADD_ATTR_RESULT_IGNORE_DEF:
			iAddVal = pstAddAttrBaseDef->IgnoreDef*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stDefChg.iIgnoreDef += iAddVal;
			break;
		case ADD_ATTR_RESULT_DEF_HP:
			iAddVal = pstAddAttrBaseDef->DefHP*(iVal/100.0);
			if (iAddVal == 0)
			{
				iAddVal = 1;
			}
			pstDyn->stDefChg.iDefHP += iAddVal;
			break;
		case ADD_ATTR_RESULT_HEAL_HP:
			pstDyn->stHealChg.nHealHPMul += iVal;
			break;
		case ADD_ATTR_RESULT_ADD_SKILL:
			pstArmAddAttrDef = z_find_armaddattrdef(pstEnv, iAddAttrID);
			if (pstArmAddAttrDef)
			{
				if (iVal > 0 &&  iVal <  MAX_ARM_ADD_ATTR_RESULT)
				{
					z_arm_add_skill(pstEnv, pstDyn, pstArmAddAttrDef->Result[iVal].ResultVal1, pstArmAddAttrDef->Result[iVal].ResultVal2);
				}
			}
			break;
		case ADD_ATTR_RESULT_KILLLER_ADD_HP:
			pstDyn->stHealChg.iKillerAddHP += iVal;
			break;
		case ADD_ATTR_RESULT_KILLLER_ADD_MP:
			pstDyn->stHealChg.iKillerAddMP += iVal;
			break;
		case ADD_ATTR_RESULT_MSPD:
			pstDyn->stHealChg.nSpeedAdd += iVal*BASE_MSPD/100.0;
			break;
		case ADD_ATTR_RESULT_NOFIGHT_ADD_HP:
			pstDyn->stHealChg.iNoFightRestoreHPMul += iVal;
			break;
		case ADD_ATTR_RESULT_NOFIGHT_ADD_MP:
			pstDyn->stHealChg.iNoFightRestoreMPMul += iVal;
			break;
		case ADD_ATTR_RESULT_ATK_PET_HURTMUL:
			pstDyn->stAtkChg.iAtkPetHurtMul += iVal;
			break;
		case ADD_ATTR_RESULT_ATK_NORMALMON_HURTMUL:
			pstDyn->stAtkChg.iAtkNormalMonHurtMul += iVal;
			break;
		case ADD_ATTR_RESULT_ATK_JYMON_HURTMUL:
			pstDyn->stAtkChg.iAtkJYMonHurtMul += iVal;
			break;
		case ADD_ATTR_RESULT_ATK_BOSSMON_HURTMUL:
			pstDyn->stAtkChg.iAtkBossMonHurtMul += iVal;
			break;
		case ADD_ATTR_RESULT_ATK_REDPLAYER_HURTMUL:
			pstDyn->stAtkChg.iAtkRedHurtMul+= iVal;
			break;
		case ADD_ATTR_RESULT_TONGXING_ADD_FLEE:
			pstDyn->stHitRateChg.iTongXingHitRate+= iVal;
			break;
		case ADD_ATTR_RESULT_TONGXING_ADD_HEAVYHIT:
			pstDyn->stHitRateChg.iTongXingHeavyHit += iVal;
			break;
		case ADD_ATTR_RESULT_YIXING_ADD_HEAVYHIT:
			pstDyn->stHitRateChg.iYiXingHeavyHit += iVal;
			break;
		case ADD_ATTR_RESULT_YIXING_ADD_FLEE:
			pstDyn->stHitRateChg.iYiXingHitRate += iVal;
			break;
		case ADD_ATTR_RESULT_ATK_ZS_HURTMUL:
			pstDyn->stAtkChg.iAtkZSHurtMul += iVal;
			break;
		case ADD_ATTR_RESULT_ATK_FS_HURTMUL:
			pstDyn->stAtkChg.iAtkFSHurtMul += iVal;
			break;
		case ADD_ATTR_RESULT_ATK_JS_HURTMUL:
			pstDyn->stAtkChg.iAtkJSHurtMul += iVal;
			break;
		case ADD_ATTR_RESULT_ATK_QS_HURTMUL:
			pstDyn->stAtkChg.iAtkQSHurtMul += iVal;
			break;
		case ADD_ATTR_RESULT_DEC_ZS_HURTMUL:
			pstDyn->stAtkChg.iDecZSHurtMul += iVal;
			break;
		case ADD_ATTR_RESULT_DEC_FS_HURTMUL:
			pstDyn->stAtkChg.iDecFSHurtMul += iVal;
			break;
		case ADD_ATTR_RESULT_DEC_JS_HURTMUL:
			pstDyn->stAtkChg.iDecJSHurtMul += iVal;
			break;
		case ADD_ATTR_RESULT_DEC_QS_HURTMUL:
			pstDyn->stAtkChg.iDecQSHurtMul += iVal;
			break;
		default:
			break;
	}

	return 0;
}


void arm_cacl_star_mul(ZONESVRENV* pstEnv,DYNATTRLIST *pstDyn, ITEMDEF *pstItemDef, ROLEARM *pstRoleArm,RESULT *pstResult)
{
	int iVal;
	int iAdd;

	switch(pstResult->ResultID)
	{
		case RESULT_BASE_PHISC_ATK:
			iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_WEAPON_MIN_PHSIC_ATK);
			if(iVal > 0)
			{
				iAdd = iVal*(pstResult->ResultVal1/100.0);
				pstDyn->stAtkChg.iPhsicMinAtk += iAdd;
			}

			iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_WEAPON_MAX_PHSIC_ATK);
			if(iVal > 0)
			{
				iAdd = iVal*(pstResult->ResultVal1/100.0);
				pstDyn->stAtkChg.iPhsicMaxAtk += iAdd;
			}

			// 双系攻击
			iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_WEAPON_MIN_ATK);
			if(iVal > 0)
			{
				iAdd = iVal*(pstResult->ResultVal1/100.0);
				pstDyn->stAtkChg.iPhsicMinAtk += iAdd;
			}

			iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_WEAPON_MAX_ATK);
			if(iVal > 0)
			{
				iAdd = iVal*(pstResult->ResultVal1/100.0);
				pstDyn->stAtkChg.iPhsicMaxAtk += iAdd;
			}
			break;
		case RESULT_BASE_ICE_ATK:
			iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_WEAPON_MIN_MAGIC_ATK);
			if(iVal > 0)
			{
				iAdd = iVal*(pstResult->ResultVal1/100.0);
				pstDyn->stAtkChg.iIceMinAtk += iAdd;
			}

			iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_WEAPON_MAX_MAGIC_ATK);
			if(iVal > 0)
			{
				iAdd = iVal*(pstResult->ResultVal1/100.0);
				pstDyn->stAtkChg.iIceMaxAtk += iAdd;
			}

			// 双系攻击
			iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_WEAPON_MIN_ATK);
			if(iVal > 0)
			{
				iAdd = iVal*(pstResult->ResultVal1/100.0);
				pstDyn->stAtkChg.iIceMinAtk += iAdd;
			}

			iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_WEAPON_MAX_ATK);
			if(iVal > 0)
			{
				iAdd = iVal*(pstResult->ResultVal1/100.0);
				pstDyn->stAtkChg.iIceMaxAtk += iAdd;
			}
			break;
		case RESULT_BASE_PHISC_DEF:
			iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_BASE_ATTR_PHSIC_DEF);
			if(iVal > 0)
			{
				iAdd = iVal*(pstResult->ResultVal1/100.0);
				pstDyn->stDefChg.unPhsicDefBaseAdd += iAdd;
			}
			break;
		case RESULT_BASE_ICE_DEF:
			iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_BASE_ATTR_MAGIC_DEF);
			if(iVal > 0)
			{
				iAdd = iVal*(pstResult->ResultVal1/100.0);
				pstDyn->stDefChg.unIceDefBaseAdd += iAdd;
			}
			break;
		case RESULT_BASE_HP_ADD:
			iVal = arm_base_attr_val_get(pstRoleArm, pstItemDef, RESULT_BASE_ATTR_HP);
			if(iVal > 0)
			{
				iAdd = iVal*(pstResult->ResultVal1/100.0);
				pstDyn->stHealChg.iHPMaxBase += iAdd;
			}
			break;
	}
	UNUSED(pstEnv);	//add by paraunused.pl
}

int z_attr_result(ZONESVRENV* pstEnv, DYNATTRLIST *pstDyn, ROLEARM *pstRoleArm, RESULT *pstResult)
{
	ATKEEHURTCHG *pstAtkeeHurtChg = &pstDyn->stAtkeeHurtChg;
	
	switch(pstResult->ResultID)
	{
		// 绝对防御取最小值
		case RESULT_ABS_DEF:
			if(pstDyn->stDefChg.iAbsDef <=0)
			{
				pstDyn->stDefChg.iAbsDef = pstResult->ResultVal1;	
			}
			else
			{
				if (pstResult->ResultVal1 < pstDyn->stDefChg.iAbsDef )
				{
					pstDyn->stDefChg.iAbsDef = pstResult->ResultVal1;
				}
			}
			
			break;
		case RESULT_BASE_GENIUS_MUL:
			pstDyn->stDefChg.iGeniusMul += pstResult->ResultVal1; 
			break;
		case RESULT_MAX_MOVE_POWER:
			pstDyn->stHealChg.nSpdMoveMaxBase += pstResult->ResultVal1;
			pstDyn->stHealChg.nSPdMoveMaxMul += pstResult->ResultVal2;
			pstDyn->stHealChg.nSPdMoveMaxAdd += pstResult->ResultVal3;
			break;
		
		case RESULT_FLY:
			pstDyn->stHealChg.nSpdFlyBase += pstResult->ResultVal1;
			pstDyn->stHealChg.nSPdFlyMul += pstResult->ResultVal2;
			pstDyn->stHealChg.nSPdFlyAdd += pstResult->ResultVal3;
			break;
		case RESULT_ICE_ATK:
			pstDyn->stAtkChg.unIceBaseArmAtk += pstResult->ResultVal1;
			pstDyn->stAtkChg.nIceAtkChgMul += pstResult->ResultVal2;
			pstDyn->stAtkChg.iIceAtkChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_FIRE_ATK:
			pstDyn->stAtkChg.unFireBaseArmAtk += pstResult->ResultVal1;
			pstDyn->stAtkChg.nFireAtkChgMul += pstResult->ResultVal2;
			pstDyn->stAtkChg.iFireAtkChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_THUNDER_ATK:
			pstDyn->stAtkChg.unThunderBaseArmAtk += pstResult->ResultVal1;
			pstDyn->stAtkChg.nThunderAtkChgMul += pstResult->ResultVal2;
			pstDyn->stAtkChg.iThunderAtkChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_LIGHT_ATK:
			pstDyn->stAtkChg.unLightBaseArmAtk += pstResult->ResultVal1;
			pstDyn->stAtkChg.nLightAtkChgMul += pstResult->ResultVal2;
			pstDyn->stAtkChg.iLightAtkChgAdd += pstResult->ResultVal3;
			break;	
		case RESULT_NIGHT_ATK:
			pstDyn->stAtkChg.unNightBaseArmAtk += pstResult->ResultVal1;
			pstDyn->stAtkChg.nNightAtkChgMul += pstResult->ResultVal2;
			pstDyn->stAtkChg.iNightAtkChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_PHSIC_ATK:
			pstDyn->stAtkChg.unPhsicBaseArmAtk += pstResult->ResultVal1;
			pstDyn->stAtkChg.nPhsicAtkChgMul += pstResult->ResultVal2;
			pstDyn->stAtkChg.iPhsicAtkChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_HPMAX:
			if (g_cResultFlag)
			{
				pstDyn->stHealChg.iHPMaxBase += pstResult->ResultVal4;
				pstDyn->stHealChg.nHPMaxMul += pstResult->ResultVal2;
				pstDyn->stHealChg.iHPMaxBase += pstResult->ResultVal3;
			}
			else
			{
				pstDyn->stHealChg.iHPMaxBase += pstResult->ResultVal1;
				pstDyn->stHealChg.nHPMaxMul += pstResult->ResultVal2;
				pstDyn->stHealChg.iHPMaxBase += pstResult->ResultVal3;
			}
		
			break;
		case RESULT_MPMAX:
			pstDyn->stHealChg.iMPMaxBase += pstResult->ResultVal1;
			pstDyn->stHealChg.nMPMaxMul += pstResult->ResultVal2;
			pstDyn->stHealChg.nMPMaxAdd += pstResult->ResultVal3;
			break;	
		case RESULT_HITRATE:
			pstDyn->stHitRateChg.nAtkerHitRateAdd += pstResult->ResultVal1;
			pstDyn->stHitChg.iHitChgAdd+= pstResult->ResultVal2;
			break;
		case RESULT_FLEE:
			pstDyn->stFleeChg.unFleeBaseAdd += pstResult->ResultVal1;
			pstDyn->stFleeChg.nFleeChgMul += pstResult->ResultVal2;
			pstDyn->stFleeChg.iFleeChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_ICE_DEF:
			pstDyn->stDefChg.unIceDefBaseAdd += pstResult->ResultVal1;
			pstDyn->stDefChg.nIceDefChgMul += pstResult->ResultVal2;
			pstDyn->stDefChg.iIceDefChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_FIRE_DEF:
			pstDyn->stDefChg.unFireDefBaseAdd += pstResult->ResultVal1;
			pstDyn->stDefChg.nFireDefChgMul += pstResult->ResultVal2;
			pstDyn->stDefChg.iFireDefChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_THUNDER_DEF:
			pstDyn->stDefChg.unThunderDefBaseAdd += pstResult->ResultVal1;
			pstDyn->stDefChg.nThunderDefChgMul += pstResult->ResultVal2;
			pstDyn->stDefChg.iThunderDefChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_LIGHT_DEF:
			pstDyn->stDefChg.unLightDefBaseAdd += pstResult->ResultVal1;
			pstDyn->stDefChg.nLightDefChgMul += pstResult->ResultVal2;
			pstDyn->stDefChg.iLightDefChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_NIGHT_DEF:
			pstDyn->stDefChg.unNightDefBaseAdd += pstResult->ResultVal1;
			pstDyn->stDefChg.nNightDefChgMul += pstResult->ResultVal2;
			pstDyn->stDefChg.iNightDefChgAdd += pstResult->ResultVal3;
			break;	
		case RESULT_PHSIC_DEF:
			pstDyn->stDefChg.unPhsicDefBaseAdd += pstResult->ResultVal1;
			pstDyn->stDefChg.nPhsicDefChgMul += pstResult->ResultVal2;
			pstDyn->stDefChg.iPhsicDefChgAdd += pstResult->ResultVal3;
			break;	
		case RESULT_ATK_ADDMP:
			pstDyn->stResourceAddChg.unMPAtkAdd += pstResult->ResultVal1;
			pstDyn->stResourceAddChg.unMPAtkMul += pstResult->ResultVal2;
			pstDyn->stResourceAddChg.unMPDeadAdd += pstResult->ResultVal3;
			pstDyn->stResourceAddChg.unMPDeadMul += pstResult->ResultVal4;
			break;
		case RESULT_DEF_ICEHURT:
			pstAtkeeHurtChg->AtkeeIceHurtMul += pstResult->ResultVal1;
			pstAtkeeHurtChg->AtkeeIceHurtAdd += pstResult->ResultVal2;
			break;
		case RESULT_DEF_FIREHURT:
			pstAtkeeHurtChg->AtkeeFireHurtMul += pstResult->ResultVal1;
			pstAtkeeHurtChg->AtkeeFireHurtAdd += pstResult->ResultVal2;
			break;
		case RESULT_DEF_THUNDERHURT:
			pstAtkeeHurtChg->AtkeeThunderHurtMul += pstResult->ResultVal1;
			pstAtkeeHurtChg->AtkeeThunderHurtAdd += pstResult->ResultVal2;
			break;
		case RESULT_DEF_LIGHTHURT:
			pstAtkeeHurtChg->AtkeeLightHurtMul += pstResult->ResultVal1;
			pstAtkeeHurtChg->AtkeeLightHurtAdd += pstResult->ResultVal2;
			break;
		case RESULT_DEF_NIGHTHURT:
			pstAtkeeHurtChg->AtkeeNightHurtMul += pstResult->ResultVal1;
			pstAtkeeHurtChg->AtkeeNightHurtAdd += pstResult->ResultVal2;
			break;
		case RESULT_DEF_PHISICHURT:
			pstAtkeeHurtChg->AtkeePhsicHurtMul += pstResult->ResultVal1;
			pstAtkeeHurtChg->AtkeePhsicHurtAdd += pstResult->ResultVal2;
			break;	
		case RESULT_DEF_MAGICHURT:
			pstAtkeeHurtChg->AtkeeMagicHurtMul += pstResult->ResultVal1;
			pstAtkeeHurtChg->AtkeeMagicHurtAdd += pstResult->ResultVal2;
			break;
		case RESULT_ICEHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerIceHurtMul += pstResult->ResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerIceHurtAdd += pstResult->ResultVal2;
			break;
		case RESULT_FIREHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerFireHurtMul += pstResult->ResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerFireHurtAdd += pstResult->ResultVal2;
			break;	
		case RESULT_THUNDERHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerThunderHurtMul += pstResult->ResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerThunderHurtAdd += pstResult->ResultVal2;
			break;	
		case RESULT_LIGHTHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerLightHurtMul += pstResult->ResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerLightHurtAdd += pstResult->ResultVal2;
			break;		
		case RESULT_NIGHTHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerNightHurtMul += pstResult->ResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerNightHurtAdd += pstResult->ResultVal2;
			break;	
		case RESULT_PHISICHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerPhsicHurtMul += pstResult->ResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerPhsicHurtAdd += pstResult->ResultVal2;
			break;	
		case RESULT_MAGICHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerMagicHurtMul += pstResult->ResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerMagicHurtAdd += pstResult->ResultVal2;
			break;	
		case RESULT_HEAVY_HIT:
			pstDyn->stHitRateChg.nHeavyHitAdd += pstResult->ResultVal1;
			break;
		case RESULT_HURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerHurtMul += pstResult->ResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerHurtAdd += pstResult->ResultVal2;
			break;
		case RESULT_DEF_HURT:
			pstAtkeeHurtChg->AtkeeHurtMul += pstResult->ResultVal1;
			pstAtkeeHurtChg->AtkeeHurtAdd += pstResult->ResultVal2;
			break;

		case RESULT_MON_FLEE:
			pstDyn->stMonChg.nFleeChgMul += pstResult->ResultVal1;
			pstDyn->stMonChg.unFleeChgAdd += pstResult->ResultVal2;
			break;

		case RESULT_MON_DEF_HURT:
			pstDyn->stMonChg.nAtkeeHurtMul += pstResult->ResultVal1;
			pstDyn->stMonChg.unAtkeeHurtAdd += pstResult->ResultVal2;
			break;
		case RESULT_MON_MAGIC_HURT:
			pstDyn->stMonChg.nAtkeeMagicDefMul += pstResult->ResultVal1;
			pstDyn->stMonChg.nAtkeeMagicDefAdd += pstResult->ResultVal2;
			break;
		case RESULT_MON_ICE_HURT:
			pstDyn->stMonChg.nAtkeeIceDefMul += pstResult->ResultVal1;
			pstDyn->stMonChg.nAtkeeIceDefAdd += pstResult->ResultVal2;
			break;
		case RESULT_MON_PHSIC_HURT:
			pstDyn->stMonChg.nAtkeePhsicDefMul += pstResult->ResultVal1;
			pstDyn->stMonChg.nAtkeePhsicDefAdd += pstResult->ResultVal2;
			break;

		case RESULT_HEAL_HP:
			pstDyn->stHealChg.nBaseHealHPMul += pstResult->ResultVal1;
			pstDyn->stHealChg.nHealHPMul += pstResult->ResultVal2;
			pstDyn->stHealChg.unHealHPAdd += pstResult->ResultVal3;
			break;

		case RESULT_HEAL_MP:
			pstDyn->stHealChg.nBaseHealMPMul += pstResult->ResultVal1;
			pstDyn->stHealChg.nHealMPMul += pstResult->ResultVal2;
			pstDyn->stHealChg.unHealMPAdd += pstResult->ResultVal3;
			break;	

		case RESULT_ARM_HPADD:
			pstDyn->stHealChg.unArmHPAdd += pstResult->ResultVal1;
			break;

		case RESULT_ARM_MPADD:
			pstDyn->stHealChg.unArmMPAdd += pstResult->ResultVal1;
			break;

		case RESULT_DAMAGE:
			pstDyn->stAtkChg.nDamageMul += pstResult->ResultVal1;
			pstDyn->stAtkChg.unDamageAdd += pstResult->ResultVal2;
			break;
		case RESULT_FIGHTVAL:
			pstDyn->stMidAttrList.uiFight += pstResult->ResultVal1;
			break;
		case RESULT_ARM_SKILL_LEVEL:
			z_arm_skill_levelup(pstDyn, pstResult->ResultVal1, pstResult->ResultVal2);
			break;
		case RESULT_ARM_ADD_EXP:
			pstDyn->stHealChg.nArmExpMul += pstResult->ResultVal1;
			break;
		case RESULT_ADD_MON_EXP:
			pstDyn->iClanSkillExpMul += pstResult->ResultVal1;
			break;
		case RESULT_ADD_VIGOR:
			pstDyn->iClanSkillVigorMul += pstResult->ResultVal1;
			pstDyn->iClanSkillVigorVal += pstResult->ResultVal2;
			break;
		case RESULT_ADD_BULL_DROP:
			if (pstDyn->stHealChg.ucBullRate+pstResult->ResultVal1> 100)
			{
				pstDyn->stHealChg.ucBullRate = 100;
			}
			else
			{
				pstDyn->stHealChg.ucBullRate+= pstResult->ResultVal1;
			}
			break;
		case RESULT_PHISIC_ICE_ATK:
			pstDyn->stAtkChg.unIceBaseArmAtk += pstResult->ResultVal1;
			pstDyn->stAtkChg.nIceAtkChgMul += pstResult->ResultVal2;
			pstDyn->stAtkChg.iIceAtkChgAdd += pstResult->ResultVal3;
			
			pstDyn->stAtkChg.unPhsicBaseArmAtk += pstResult->ResultVal1;
			pstDyn->stAtkChg.nPhsicAtkChgMul += pstResult->ResultVal2;
			pstDyn->stAtkChg.iPhsicAtkChgAdd += pstResult->ResultVal3;		
			break;	
		case RESULT_ADD_BASE_ATTR:
			/* 增加一级属性 1-力 2-智 3-体 4-精 */
			pstDyn->stMidAttrList.iLiliangAdd += pstResult->ResultVal1;
			pstDyn->stMidAttrList.iZhiliAdd += pstResult->ResultVal2;
			pstDyn->stMidAttrList.iTiliAdd += pstResult->ResultVal3;
			pstDyn->stMidAttrList.iFangyuAdd += pstResult->ResultVal4;
			break;
		//一级属性防护
		case RESULT_ADD_BASE_ATTR_DEF:
			pstDyn->stMidAttrList.iFangyuMul += pstResult->ResultVal1;
			pstDyn->stMidAttrList.iFangyuAdd += pstResult->ResultVal2;
			//pstDyn->stMidAttrList.iMinJieMul += pstResult->ResultVal1;
			//pstDyn->stMidAttrList.iMinJieAdd += pstResult->ResultVal2;
			break;
		//一级属性力量
		case RESULT_ADD_BASE_ATTR_STR:
			pstDyn->stMidAttrList.iLiliangMul += pstResult->ResultVal1;
			pstDyn->stMidAttrList.iLiliangAdd += pstResult->ResultVal2;
			break;
		//一级属性智力
		case RESULT_ADD_BASE_ATTR_INT:
			pstDyn->stMidAttrList.iZhiliMul += pstResult->ResultVal1;
			pstDyn->stMidAttrList.iZhiliAdd += pstResult->ResultVal2;
			break;
		//一级属性体质
		case RESULT_ADD_BASE_ATTR_STA:
			pstDyn->stMidAttrList.iTiliMul += pstResult->ResultVal1;
			pstDyn->stMidAttrList.iTiliAdd += pstResult->ResultVal2;
			break;
		//一级属性精神
		case RESULT_ADD_BASE_ATTR_SPR:
			pstDyn->stMidAttrList.iJingshenMul += pstResult->ResultVal1;
			pstDyn->stMidAttrList.iJingshenAdd += pstResult->ResultVal2;
			break;
		/*
		//一级属性命中
		case RESULT_ADD_BASE_ATTR_HIT:
			pstDyn->stMidAttrList.iMingzhongMul += pstResult->ResultVal1;
			pstDyn->stMidAttrList.iMingzhongAdd += pstResult->ResultVal2;
			break;
		//一级属性闪避
		case RESULT_ADD_BASE_ATTR_FLEE:
			pstDyn->stMidAttrList.iShanbiMul += pstResult->ResultVal1;
			pstDyn->stMidAttrList.iShanbiAdd += pstResult->ResultVal2;
			break;
		//一级属性暴击
		case RESULT_ADD_BASE_ATTR_HEAVY_HIT:
			pstDyn->stMidAttrList.iBaojiMul += pstResult->ResultVal1;
			pstDyn->stMidAttrList.iBaojiAdd += pstResult->ResultVal2;
			break;
		*/
		case RESULT_HEAVY_HIT_VAL:
			pstDyn->stHitRateChg.nHeavyHitVal += pstResult->ResultVal1*100;
			break;
		case RESULT_SECONDARY_SKILL_PRODUCE_CHG:
			pstDyn->stHealChg.unSecondarySkillType = pstResult->ResultVal1;
			pstDyn->stHealChg.unRecipeProduceIdx = pstResult->ResultVal2;
			break;
		case RESULT_ADD_BASE_ATTR_AGILE:
			pstDyn->stMidAttrList.iMinJieMul += pstResult->ResultVal1;
			pstDyn->stMidAttrList.iMinJieAdd += pstResult->ResultVal2;
			//pstDyn->stMidAttrList.iFangyuMul += pstResult->ResultVal1;
			//pstDyn->stMidAttrList.iFangyuAdd += pstResult->ResultVal2;
			break;
		case RESULT_WATER_ATK:
			pstDyn->stAtkChg.unWaterBaseArmAtk += pstResult->ResultVal1;
			pstDyn->stAtkChg.nWaterAtkChgMul += pstResult->ResultVal2;
			pstDyn->stAtkChg.iWaterAtkChgAdd += pstResult->ResultVal3;
			break;	
		case RESULT_SIEGE_ATK:
			pstDyn->stAtkChg.unSiegeBaseArmAtk += pstResult->ResultVal1;
			pstDyn->stAtkChg.nSiegeAtkChgMul += pstResult->ResultVal2;
			pstDyn->stAtkChg.iSiegeAtkChgAdd += pstResult->ResultVal3;
			break;	
		case RESULT_HOLY_ATK:
			pstDyn->stAtkChg.unHolyBaseArmAtk += pstResult->ResultVal1;
			pstDyn->stAtkChg.nHolyAtkChgMul += pstResult->ResultVal2;
			pstDyn->stAtkChg.iHolyAtkChgAdd += pstResult->ResultVal3;
			break;	
		case RESULT_MACHINE_ATK:
			pstDyn->stAtkChg.unMachineBaseArmAtk += pstResult->ResultVal1;
			pstDyn->stAtkChg.nMachineAtkChgMul += pstResult->ResultVal2;
			pstDyn->stAtkChg.iMachineAtkChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_WATER_DEF:
			pstDyn->stDefChg.unWaterDefBaseAdd += pstResult->ResultVal1;
			pstDyn->stDefChg.nWaterDefChgMul += pstResult->ResultVal2;
			pstDyn->stDefChg.iWaterDefChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_SIEGE_DEF:
			pstDyn->stDefChg.unSiegeDefBaseAdd += pstResult->ResultVal1;
			pstDyn->stDefChg.nSiegeDefChgMul += pstResult->ResultVal2;
			pstDyn->stDefChg.iSiegeDefChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_HOLY_DEF:
			pstDyn->stDefChg.unHolyDefBaseAdd += pstResult->ResultVal1;
			pstDyn->stDefChg.nHolyDefChgMul += pstResult->ResultVal2;
			pstDyn->stDefChg.iHolyDefChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_MACHINE_DEF:
			pstDyn->stDefChg.unMachineDefBaseAdd += pstResult->ResultVal1;
			pstDyn->stDefChg.nMachineDefChgMul += pstResult->ResultVal2;
			pstDyn->stDefChg.iMachineDefChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_BASE_ATTR_HEAVYHIT:
			pstDyn->stHitRateChg.iHeavyHit += pstResult->ResultVal1;
			break;
		case RESULT_DEF_HEAVYHIT:
			pstDyn->stHitRateChg.iDefHeavyHit += pstResult->ResultVal1;
			break;
		case RESULT_REFLECTION_HP:
			pstDyn->stDefChg.iReflectHP += pstResult->ResultVal1;
			break;
		case RESULT_IGNORE_DEF:
			pstDyn->stDefChg.iIgnoreDef += pstResult->ResultVal1;
			break;
		case RESULT_DEF_HP:
			pstDyn->stDefChg.iDefHP += pstResult->ResultVal1;
			break;
		case RESULT_ADD_SKILL:
			z_arm_add_skill(pstEnv, pstDyn, pstResult->ResultVal1, pstResult->ResultVal2);
			break;
			
		case RESULT_BEACH_EXP_MUL:
			pstDyn->iBeachExpMul += pstResult->ResultVal1;
			break;
		case RESULT_WEAPON_MIN_PHSIC_ATK:
			{
				BASEATTRINFO *pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, pstResult->ResultID);
				pstDyn->stAtkChg.iPhsicMinAtk += pstResult->ResultVal1;
				if(pstBaseAttrInfo)
				{
					pstDyn->stAtkChg.iPhsicMinAtk += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
				}
			}
			break;
		case RESULT_WEAPON_MAX_PHSIC_ATK:
			{
				BASEATTRINFO *pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, pstResult->ResultID);
				pstDyn->stAtkChg.iPhsicMaxAtk += pstResult->ResultVal1;
				if(pstBaseAttrInfo)
				{
					pstDyn->stAtkChg.iPhsicMaxAtk += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
				}
			}
			break;
		case RESULT_WEAPON_MIN_MAGIC_ATK:
			{
				BASEATTRINFO *pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, pstResult->ResultID);
				pstDyn->stAtkChg.iIceMinAtk += pstResult->ResultVal1;
				if(pstBaseAttrInfo)
				{
					pstDyn->stAtkChg.iIceMinAtk += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
				}
			}
			break;
		case RESULT_WEAPON_MAX_MAGIC_ATK:
			{
				BASEATTRINFO *pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, pstResult->ResultID);
				pstDyn->stAtkChg.iIceMaxAtk += pstResult->ResultVal1;
				if(pstBaseAttrInfo)
				{
					pstDyn->stAtkChg.iIceMaxAtk += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
				}
			}
			break;
		case RESULT_BASE_ATTR_HP:
			{
				BASEATTRINFO *pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, pstResult->ResultID);
				
				pstDyn->stHealChg.iHPMaxBase += pstResult->ResultVal1;
				if(pstBaseAttrInfo)
				{
					pstDyn->stHealChg.iHPMaxBase += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
				}
			}
			break;
		case RESULT_BASE_ATTR_PHSIC_DEF:
			{
				BASEATTRINFO *pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, pstResult->ResultID);
				
				pstDyn->stDefChg.unPhsicDefBaseAdd += pstResult->ResultVal1;
				if(pstBaseAttrInfo)
				{
					pstDyn->stDefChg.unPhsicDefBaseAdd += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
				}
			}
			break;
		case RESULT_BASE_ATTR_MAGIC_DEF:
			{
				BASEATTRINFO *pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, pstResult->ResultID);
				
				pstDyn->stDefChg.unIceDefBaseAdd += pstResult->ResultVal1;
				if(pstBaseAttrInfo)
				{
					pstDyn->stDefChg.unIceDefBaseAdd += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
				}
			}
			break;
		case RESULT_BASE_ATTR_SOIL_DEF:
			{
				BASEATTRINFO *pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, pstResult->ResultID);
				
				pstDyn->stDefChg.unWaterDefBaseAdd += pstResult->ResultVal1;
				if(pstBaseAttrInfo)
				{
					pstDyn->stDefChg.unWaterDefBaseAdd += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
				}
			}
			break;
		case RESULT_BASE_ATTR_WIND_DEF:
			{
				BASEATTRINFO *pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, pstResult->ResultID);
				
				pstDyn->stDefChg.unFireDefBaseAdd += pstResult->ResultVal1;
				if(pstBaseAttrInfo)
				{
					pstDyn->stDefChg.unFireDefBaseAdd += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
				}
			}
			break;
		case RESULT_BASE_ATTR_THUNDER_DEF:
			{
				BASEATTRINFO *pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, pstResult->ResultID);
				
				pstDyn->stDefChg.unThunderDefBaseAdd += pstResult->ResultVal1;
				if(pstBaseAttrInfo)
				{
					pstDyn->stDefChg.unThunderDefBaseAdd += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
				}
			}
			break;
		case RESULT_BASE_ATTR_LIGHT_DEF:
			{
				BASEATTRINFO *pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, pstResult->ResultID);
				
				pstDyn->stDefChg.unLightDefBaseAdd += pstResult->ResultVal1;
				if(pstBaseAttrInfo)
				{
					pstDyn->stDefChg.unLightDefBaseAdd += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
				}
			}
			break;
		case RESULT_BASE_ATTR_NIGHT_DEF:
			{
				BASEATTRINFO *pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, pstResult->ResultID);
				
				pstDyn->stDefChg.unNightDefBaseAdd += pstResult->ResultVal1;
				if(pstBaseAttrInfo)
				{
					pstDyn->stDefChg.unNightDefBaseAdd += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
				}
			}
			break;
		case RESULT_BASE_ATTR_FLEE_LEVEL:
			{
				BASEATTRINFO *pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, pstResult->ResultID);
				
				pstDyn->stFleeChg.unFleeBaseAdd += pstResult->ResultVal1;
				if(pstBaseAttrInfo)
				{
					pstDyn->stFleeChg.unFleeBaseAdd += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
				}
			}
			break;
		case RESULT_BASE_ATTR_HIT_LEVEL:
			{
				BASEATTRINFO *pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, pstResult->ResultID);
				
				pstDyn->stHitChg.iHitChgAdd += pstResult->ResultVal1;
				if(pstBaseAttrInfo)
				{
					pstDyn->stHitChg.iHitChgAdd += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
				}
			}
			break;
		case RESULT_BASE_ATTR_HEAVY_HIT_VAL:
			{
				BASEATTRINFO *pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, pstResult->ResultID);
				
				pstDyn->stHitRateChg.nHeavyHitVal += pstResult->ResultVal1;
				if(pstBaseAttrInfo)
				{
					pstDyn->stHitRateChg.nHeavyHitVal += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
				}
			}
			break;
		case RESULT_BASE_ATTR_MAGIC_ATK:
			{
				BASEATTRINFO *pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, pstResult->ResultID);
				pstDyn->stAtkChg.iIceMinAtk += pstResult->ResultVal1;
				pstDyn->stAtkChg.iIceMaxAtk += pstResult->ResultVal1;
				if(pstBaseAttrInfo)
				{
					pstDyn->stAtkChg.iIceMinAtk += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
					pstDyn->stAtkChg.iIceMaxAtk += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
				}
			}
			break;
		case RESULT_BASE_ATTR_PHSIC_ATK:
			{
				BASEATTRINFO *pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, pstResult->ResultID);
				pstDyn->stAtkChg.iPhsicMinAtk += pstResult->ResultVal1;
				pstDyn->stAtkChg.iPhsicMaxAtk += pstResult->ResultVal1;
				if(pstBaseAttrInfo)
				{
					pstDyn->stAtkChg.iPhsicMinAtk += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
					pstDyn->stAtkChg.iPhsicMaxAtk += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
				}
			}
			break;
		case RESULT_POFANG_LEVEL:
			{
				int iTmp = pstDyn->iRoleLvl/10;
				if (iTmp)
				{
					pstDyn->stDefChg.iIgnoreDef += pstResult->ResultVal1/100.0f*(iTmp*iTmp*10.0f/3);
				}
			}
			break;
		case RESULT_DIKANG_LEVEL:
			{
				int iTmp = pstDyn->iRoleLvl/10;
				if (iTmp)
				{
					pstDyn->stDefChg.iDefHP += pstResult->ResultVal1/100.0f*(iTmp*iTmp*10.0f/3);
				}
			}
			break;
		case RESULT_DODGE:
			{
				pstDyn->stHitRateChg.nAtkeeHitRateAdd += pstResult->ResultVal1;
			}
			break;
		case RESULT_WEAPON_MIN_ATK:
			{
				BASEATTRINFO *pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, pstResult->ResultID);
				pstDyn->stAtkChg.iIceMinAtk += pstResult->ResultVal1;
				pstDyn->stAtkChg.iPhsicMinAtk += pstResult->ResultVal1;
				if(pstBaseAttrInfo)
				{
					pstDyn->stAtkChg.iIceMinAtk += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
					pstDyn->stAtkChg.iPhsicMinAtk += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
				}
			}
			break;
		case RESULT_WEAPON_MAX_ATK:
			{
				BASEATTRINFO *pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, pstResult->ResultID);
				pstDyn->stAtkChg.iIceMaxAtk += pstResult->ResultVal1;
				pstDyn->stAtkChg.iPhsicMaxAtk += pstResult->ResultVal1;
				if(pstBaseAttrInfo)
				{
					pstDyn->stAtkChg.iIceMaxAtk += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
					pstDyn->stAtkChg.iPhsicMaxAtk += pstResult->ResultVal2*pstBaseAttrInfo->Mul/100.0;
				}
			}
			break;
		case RESULT_DEF_DOUBLE:
			pstDyn->stDefChg.unIceDefBaseAdd += pstResult->ResultVal1;
			pstDyn->stDefChg.nIceDefChgMul += pstResult->ResultVal2;
			pstDyn->stDefChg.iIceDefChgAdd += pstResult->ResultVal3;

			pstDyn->stDefChg.unPhsicDefBaseAdd += pstResult->ResultVal1;
			pstDyn->stDefChg.nPhsicDefChgMul += pstResult->ResultVal2;
			pstDyn->stDefChg.iPhsicDefChgAdd += pstResult->ResultVal3;
			break;
		case RESULT_ALL_ATTR:
			pstDyn->stMidAttrList.iZhiliAdd += pstResult->ResultVal1;
			pstDyn->stMidAttrList.iLiliangAdd+= pstResult->ResultVal1;
			pstDyn->stMidAttrList.iTiliAdd+= pstResult->ResultVal1;
			pstDyn->stMidAttrList.iJingshenAdd+= pstResult->ResultVal1;
			pstDyn->stMidAttrList.iFangyuAdd+= pstResult->ResultVal1;
			pstDyn->stMidAttrList.iMinJieAdd+= pstResult->ResultVal1;
			break;
		case RESULT_ADD_SPEED:
			pstDyn->stHealChg.nSpeedMul += pstResult->ResultVal1;
			pstDyn->stHealChg.nSpeedAdd += pstResult->ResultVal2;
			break;
		case RESULT_WEAPON_ATK_MUL:
			pstDyn->stAtkChg.nPhsicAtkChgMul += pstResult->ResultVal1;
			pstDyn->stAtkChg.nIceAtkChgMul += pstResult->ResultVal1;
			break;
		case RESULT_WEAPON_DEF_MUL:
			pstDyn->stDefChg.nIceDefChgMul += pstResult->ResultVal1;
			pstDyn->stDefChg.nPhsicDefChgMul += pstResult->ResultVal1;
			break;
		default:
			break;
	}

	return 0;
}

int calc_addattr(ZONESVRENV *pstEnv, DYNATTRLIST *pstDyn, int iSuitAttrID)
{
	int i;
	ADDATTRDEF *pstAddAttrDef;

	pstAddAttrDef = z_find_addattr(pstEnv, iSuitAttrID);
	if (NULL == pstAddAttrDef)
	{
		return -1;
	}

	for (i=0; i<MAX_ADDATTR_RESULT; i++)
	{
		if (0 == pstAddAttrDef->Result[i].ResultID)
		{
			break;
		}

		z_attr_result(pstEnv, pstDyn, NULL, pstAddAttrDef->Result+i);
	}
	
	return 0;
}

ARMLEVELHURTDEF *z_find_armlevelhurt(ZONESVRENV* pstEnv, unsigned short unArmType, unsigned short unLevel)
{
	ARMLEVELHURTDEF stArmLevelHurt;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stArmLevelHurt.ArmType = unArmType;
	stArmLevelHurt.Level = unLevel;

	iIndex = bsearch_int(&stArmLevelHurt, pstObj->sArmLevelHurtDef, pstObj->iArmLevelHurtDef, ARM_LEVEL_HURT_DATA, &iEqu);
	if (iEqu)
	{
		return (ARMLEVELHURTDEF *)pstObj->sArmLevelHurtDef[iIndex];
	}

	return NULL;
}

/*
int z_weapon_attr(ZONESVRENV* pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef, ATKCHG *pstAtkChg)
{
	int i;
	ARMLEVELHURTDEF *pstArmLevelHurt;
	ITEMDEF *pstAmmDef;
	int iAmmID = 0;
	AMMPACKAGE *pstAmmPak = &pstPlayer->stRoleData.Package.AmmPackage;
	AMMMAP *pstAmmMap = &pstPlayer->stRoleData.Wear.AmmMap;

	pstAtkChg->iNeedAmm = pstItemDef->WeaponAmmNeed;
	if (pstAtkChg->iNeedAmm)
	{

		if (0 != pstItemDef->WearPos2) //右手武器弹药
		{
			iAmmID = pstAmmMap->AmmRight;
		}
		else
		{
			switch(pstItemDef->WearPos1)
			{
				case WEAR_POS_LEFT:
					iAmmID = pstAmmMap->AmmLeft;
					break;
				case WEAR_POS_RIGHTHAND:
					iAmmID = pstAmmMap->AmmRight;
					break;
				case WEAR_POS_SHOULDER:
					iAmmID = pstAmmMap->AmmShoulder;
					break;
				default:
					break;
			}
		}

		if (iAmmID > 0)
		{
			pstAmmDef = z_find_itemdef(pstEnv, iAmmID);
			if (pstAmmDef && pstAmmDef->AmmType == pstItemDef->WeaponAmmType)
			{
				pstAtkChg->iAmmID = iAmmID;
				pstAtkChg->iAmmPos = -1;
				for (i=0; i<pstAmmPak->Num; i++)
				{
					if (pstAmmPak->RoleItems[i].DefID == iAmmID)
					{
						pstAtkChg->iAmmPos = i;
						break;
					}
				}

				pstAtkChg->unBaseAmmAtk = pstAmmDef->AmmAtk;
			}
		}
	}

	pstArmLevelHurt = z_find_armlevelhurt(pstEnv, pstItemDef->NumType, pstPlayer->stRoleData.Level);
	if (pstArmLevelHurt)
	{
		pstAtkChg->nArmLevChgMul = pstArmLevelHurt->HurtMul;
	}

	return 0;
}
*/
/*
int z_sort_weapon_dyn_atkchg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	int iWeaponNum = 0;
	ITEMDEF *pstItemDef;
	ROLEARM *pstRoleArm;
	ATKCHG *pstAtkChg;
	ROLEWEAR *pstWear =  &pstPlayer->stRoleData.Wear;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	UNUSED(pstAppCtx);
	
	for(i=0; i<pstWear->ArmNum; i++)
	{
		pstRoleArm = pstWear->RoleArms+i;
		
		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleArm);
		if (!pstItemDef)
		{
			continue;
		}

		if (z_is_weapon(pstItemDef, pstPlayer->stRoleData.Career))
		{
			pstAtkChg = &pstDyn->astAtkChg[iWeaponNum];
			pstAtkChg->iArmID = pstItemDef->ItemID;
			pstAtkChg->iArmDefPos = (long int)((char *)pstItemDef- (char *)pstEnv->pstZoneObj->sItemDef)/ITEM_DEF_DATA;
			pstAtkChg->iArmType = pstItemDef->NumType;
			iWeaponNum++;
		}
	}
	
	return 0;
}
ATKCHG * z_get_weapon_dyn_atkchg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,ITEMDEF *pstItemDef)
{
	int i=0;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);

	if( pstItemDef == NULL || pstPlayer==NULL)
	{
		return NULL;
	}
	

	for( i=0;i<MAX_ARM_ON;i++)
	{
		if( (unsigned int)pstDyn->astAtkChg[i].iArmID == pstItemDef->ItemID &&
		 	pstDyn->astAtkChg[i].iArmType == pstItemDef->NumType)
		{
			return &pstDyn->astAtkChg[i];
		}
	}

	return NULL;
}
*/

//炮
int is_gunner_cannon(int iArmType)
{
	if(iArmType == ARM_LIUDAN_GUN ||
		iArmType == ARM_FUYOU_GUN ||
		iArmType == ARM_HUOJIAN_GUN)
	{
		return 1;
	}

	return 0;
}

int z_arm_attr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i, j, k;
	int  idxSuitCount = 0;
	ITEMDEF *pstItemDef;
	ROLEARM *pstRoleArm;
	RESULT *pstResult;
	SuitCount astSuitCount[MAX_WEAR];
	SUITSET *pstSuitSet;
	ROLEWEAR *pstWear =  &pstPlayer->stRoleData.Wear;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	UNUSED(pstAppCtx);

	memset(&astSuitCount, 0, sizeof(astSuitCount));

	//确保第一个是右手
	//z_sort_weapon_dyn_atkchg(pstAppCtx, pstEnv, pstPlayer);
	
	for(i=0; i<pstWear->ArmNum; i++)
	{
		pstRoleArm = pstWear->RoleArms+i;
		
		if (	pstRoleArm->ValidEnd > 0 &&
			pstRoleArm->ValidEnd < pstAppCtx->stCurr.tv_sec)
		{
			continue;
		}

		/*if (pstRoleArm->InstFlag & INST_ITEM_FENGYIN)
		{
			continue;
		}*/

		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleArm);
		if (!pstItemDef)
		{
			continue;
		}

		if(arm_is_back(pstItemDef) && !is_valid_back(pstPlayer, pstRoleArm->GridIdx))
		{
			continue;
		}

		if (z_pkg_check_clanlimit(pstEnv, pstPlayer,pstItemDef->WearPre.ClanLimit) < 0)
		{
			continue;
		}

		if (pstItemDef->ArmDur.MaxDur > 0 && 0 == pstRoleArm->Dur/DUR_UNIT)
		{
			continue;
		}

		if (0 < pstItemDef->SuitSetID)
		{
			/* look for suit combination */
			for (j = 0; j < idxSuitCount; j++)
			{
				if(pstItemDef->SuitSetID == astSuitCount[j].iSuitID)
				{
					for(k=0; k<i; k++)
					{
						if(pstWear->RoleArms[k].DefID == (int)pstItemDef->ItemID)
						{
							break;
						}
					}

					if(k>= i)
					{
						astSuitCount[j].iCount += pstItemDef->WearPosNum;
					}
					
					break;
				}
			}

			/* not found, unique */
			if (idxSuitCount == j)
			{
				astSuitCount[idxSuitCount].iSuitID = pstItemDef->SuitSetID;
				astSuitCount[idxSuitCount].iCount++;
				idxSuitCount++;
			}
		}

		pstDyn->stMidAttrList.uiFight += pstItemDef->ArmFight;

		for (j=0; j<MAX_ITEM_RESULT; j++)	
		{
			pstResult = &pstItemDef->Result[j];

			if (0 == pstResult->ResultID)
			{
				//break;
				continue;
			}
			
			z_attr_result(pstEnv, pstDyn, pstRoleArm, pstResult);
		}

		arm_calc_teshu_attr(pstEnv,pstPlayer, pstRoleArm,pstItemDef);

		//装备关键字效果
		for (j=0; j<pstRoleArm->AddAttrNum; j++)	
		{
			if (pstRoleArm->AddAttr[j].AddAttrID == 0)
			{
				continue;
			}

			// 此条被封印了
			if (arm_attr_is_fengyin(pstRoleArm,j, 1))
			{
				continue;
			}
			
			cacl_arm_add_attr_result(pstEnv, pstPlayer, pstDyn, pstItemDef, 
									pstRoleArm->AddAttr[j].AddAttrID, pstRoleArm->AddAttr[j].Val);
		}

		//装备洗炼关键字效果
		for (j=0; j<pstRoleArm->WashAttrNum; j++)	
		{
			if (pstRoleArm->WashAddAttr[j].AddAttrID == 0)
			{
				continue;
			}

			if (arm_attr_is_fengyin(pstRoleArm,j, 0))
			{
				continue;
			}
			
			cacl_arm_add_attr_result(pstEnv, pstPlayer, pstDyn, pstItemDef, 
									pstRoleArm->WashAddAttr[j].AddAttrID, pstRoleArm->WashAddAttr[j].Val);
		}

		// 附魔效果
		switch(z_extend_item_property(pstEnv, pstRoleArm->ExtendedFromDefID, 0))
		{
		case EXTENDED_TYPE_RESULT:
			{
				RESULT stDummy;

				stDummy.ResultID = pstRoleArm->ExtendedResult.ResultID;
				stDummy.ResultVal1 = pstRoleArm->ExtendedResult.ResultVal1;
				stDummy.ResultVal2 = pstRoleArm->ExtendedResult.ResultVal2;
				stDummy.ResultVal3 = pstRoleArm->ExtendedResult.ResultVal3;
				stDummy.ResultVal4 = pstRoleArm->ExtendedResult.ResultVal4;
				z_attr_result(pstEnv, pstDyn, pstRoleArm, &stDummy);
			}
			break;

		case EXTENDED_TYPE_ATTR:
			break;
		}

		// 装备刻印
		if (pstRoleArm->KeYinAddAttr.AddAttrID > 0)
		{
			RESULT stDummy;

			stDummy.ResultID = pstRoleArm->KeYinAddAttr.AddAttrID;
			stDummy.ResultVal1 = pstRoleArm->KeYinAddAttr.Val;
			stDummy.ResultVal2 = 0;
			stDummy.ResultVal3 = 0;
			stDummy.ResultVal4 = 0;
			z_attr_result(pstEnv, pstDyn, pstRoleArm, &stDummy);
		}

		// 装备阶层计算
		z_arm_step_attr_calc(pstEnv, pstDyn, pstRoleArm, pstItemDef);
		
		//装备品质,战斗力计算
		//装备宝石属性计算
		z_arm_star_fight(pstEnv, pstDyn, pstRoleArm);
		z_arm_gem(pstEnv, pstDyn, pstRoleArm);

		// 计算单件装备完美星属性
		z_arm_hideattr_calc_one( pstEnv, pstPlayer, pstItemDef,pstRoleArm);
		
	}

	//套装属性
	for (i = 0; i < idxSuitCount; i++)
	{
		pstSuitSet = z_find_suitset(pstEnv, astSuitCount[i].iSuitID);

		if (NULL == pstSuitSet)
			continue;

		for (j = 0; j < MAX_SUIT_ATTR; j++)
		{
			if (astSuitCount[i].iCount >= (int)pstSuitSet->SuitAttr[j].MinSuitNum)
			{
				calc_addattr(pstEnv, pstDyn, pstSuitSet->SuitAttr[j].SuitAttrID);
			}
		}
	}

	//装备的隐藏属性
	z_arm_hideattr_calc(pstEnv, pstPlayer);
	return 0;
} 
int z_skill_diathesis_attr(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneSkill *pstZoneSkill, SKILLDEF *pstSkillDef)
{
	int i, j;
	DIATHESISDEF *pstDiathesisDef;
	DIATHESISDATA *pstDiathesisData;
	SKILLRESULT *pstResult;
	int iCollectSkillUse = 0;
	int iGeniusCollectSkillDec = 0;
	DIATHESISINFO *pstDiathesisInfo = &pstPlayer->stRoleData.DiathesisInfo;
	UNUSED(pstSkillDef);
	
	for (i=0; i<(int)pstDiathesisInfo->DiathesisNum; i++)
	{
		pstDiathesisData = &pstDiathesisInfo->DiathesisList[i];
		pstDiathesisDef = z_find_diathesis(pstEnv, pstDiathesisData->DiathesisID, pstDiathesisData->DiathesisLevel);
		if (!pstDiathesisDef)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "DiathesisID %d DiathesisLevel %d no def", pstDiathesisData->DiathesisID, pstDiathesisData->DiathesisLevel);
			continue;
		}

		for (j=0; j<(int)(sizeof(pstDiathesisDef->DiathesisResult)/sizeof(pstDiathesisDef->DiathesisResult[0])); j++)
		{
			pstResult = pstDiathesisDef->DiathesisResult + j;
			if (RESULT_REDUCE_SKILL_CD_TIME == pstResult->SkillResultID)
			{
				if(pstResult->SkillResultVal1 ==  pstSkillDef->SkillID )
				{
					genius_atk_reduce_skill_cd_time(pstPlayer,&pstZoneSkill->stSkillData, pstResult);
				}
			}
			else if(RESULT_ADD_SKILL_HURT == pstResult->SkillResultID)
			{
				genius_atk_skill_add_hurt( pstPlayer,pstZoneSkill, pstResult);
			}
			else if(RESULT_ADD_SKILL_HEAVYHITRATE == pstResult->SkillResultID)
			{
				genius_atk_skill_add_heavy_hit_rate( pstPlayer, pstZoneSkill,pstResult);
			}
		}
	}

	//天赋对技能积累数减少
	iCollectSkillUse = pstSkillDef->CollectSkillUse;
	iGeniusCollectSkillDec = genius_collectskill_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstSkillDef->SkillID);
	if( iCollectSkillUse > iGeniusCollectSkillDec)
	{
		iCollectSkillUse -= iGeniusCollectSkillDec;
	}

	//pstZoneSkill->stSkillData.CSFittingsRowList[0].CollectUse = iCollectSkillUse;
	
	return 0;
}
int z_calc_diathesis(ZONESVRENV* pstEnv, Player *pstPlayer, SKILLRESULT *pstResult)
{
	ATKCHG *pstAtkChg;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	ATKEEHURTCHG *pstAtkeeHurtChg = &pstDyn->stAtkeeHurtChg;

	switch(pstResult->SkillResultID)
	{
		case RESULT_HITRATE:
			pstDyn->stHitRateChg.nAtkerHitRateAdd += pstResult->SkillResultVal1;
			pstDyn->stHitChg.iHitChgAdd+= pstResult->SkillResultVal2;
			break;
		case RESULT_XP:				 // 素质影响xp修正
			pstDyn->stHealChg.nXpMul += pstResult->SkillResultVal1;
			pstDyn->stHealChg.nXpAdd += pstResult->SkillResultVal2;
			break;
		case RESULT_SIT_HP_MODIFY:  	// 打坐回复hp修正
			pstDyn->stHealChg.nSitHPMul += pstResult->SkillResultVal1;
			break;
		case RESULT_SIT_MP_MODIFY:
			pstDyn->stHealChg.nSitMPMul += pstResult->SkillResultVal1;
			break;
		case RESULT_SIT_EXP_MODIFY:
			pstDyn->stHealChg.nSitExpMul += pstResult->SkillResultVal1;
			break;
		case RESULT_CONSUME_VIGOR_MODIFY:
			pstDyn->stHealChg.nSitVigorMul += pstResult->SkillResultVal1;
			break;
		case RESULT_VIGOR:			// 最大vigor
			//pstDyn->stMidAttrList.unVigorMax = pstResult->SkillResultVal1;
			break;
		case RESULT_MOVE_POWER_RESTORE:
			pstDyn->stHealChg.nSpdMoveRestore += pstResult->SkillResultVal1;
			break;
		case RESULT_MAX_MOVE_POWER:
			pstDyn->stHealChg.nSpdMoveMaxBase += pstResult->SkillResultVal1;
			pstDyn->stHealChg.nSPdMoveMaxMul += pstResult->SkillResultVal2;
			pstDyn->stHealChg.nSPdMoveMaxAdd += pstResult->SkillResultVal3;
			break;
		case RESULT_HPMAX:
			pstDyn->stHealChg.iHPMaxBase += pstResult->SkillResultVal1;
			pstDyn->stHealChg.nHPMaxMul += pstResult->SkillResultVal2;
			pstDyn->stHealChg.nHPMaxAdd += pstResult->SkillResultVal3;
			break;

		case RESULT_MPMAX:
			pstDyn->stHealChg.iMPMaxBase += pstResult->SkillResultVal1;
			pstDyn->stHealChg.nMPMaxMul += pstResult->SkillResultVal2;
			pstDyn->stHealChg.nMPMaxAdd += pstResult->SkillResultVal3;
			break;	
			
		case RESULT_VIMMAX:
			pstDyn->stHealChg.nVimMaxBase += pstResult->SkillResultVal1;
			pstDyn->stHealChg.nVimMaxMul += pstResult->SkillResultVal2;
			pstDyn->stHealChg.nVimMaxAdd += pstResult->SkillResultVal3;
			break;
			
		case RESULT_ICE_ATK:
			pstAtkChg = &pstDyn->stAtkChg;
			pstAtkChg->unIceBaseArmAtk += pstResult->SkillResultVal1;
			pstAtkChg->nIceAtkChgMul += pstResult->SkillResultVal2;
			pstAtkChg->iIceAtkChgAdd += pstResult->SkillResultVal3;
			break;

		 case RESULT_FIRE_ATK:
			pstAtkChg = &pstDyn->stAtkChg;
			pstAtkChg->unFireBaseArmAtk += pstResult->SkillResultVal1;
			pstAtkChg->nFireAtkChgMul += pstResult->SkillResultVal2;
			pstAtkChg->iFireAtkChgAdd += pstResult->SkillResultVal3;
		 	break;

		 case RESULT_THUNDER_ATK:
			pstAtkChg = &pstDyn->stAtkChg;
			pstAtkChg->unThunderBaseArmAtk += pstResult->SkillResultVal1;
			pstAtkChg->nThunderAtkChgMul += pstResult->SkillResultVal2;
			pstAtkChg->iThunderAtkChgAdd += pstResult->SkillResultVal3;
		 	break;	
		 case RESULT_LIGHT_ATK:
			pstAtkChg = &pstDyn->stAtkChg;
			pstAtkChg->unLightBaseArmAtk += pstResult->SkillResultVal1;
			pstAtkChg->nLightAtkChgMul += pstResult->SkillResultVal2;
			pstAtkChg->iLightAtkChgAdd += pstResult->SkillResultVal3;
		 	break;	

		 case RESULT_NIGHT_ATK:
			pstAtkChg = &pstDyn->stAtkChg;
			pstAtkChg->unNightBaseArmAtk += pstResult->SkillResultVal1;
			pstAtkChg->nNightAtkChgMul += pstResult->SkillResultVal2;
			pstAtkChg->iNightAtkChgAdd += pstResult->SkillResultVal3;
		 	break;
		case RESULT_PHSIC_ATK:
			pstAtkChg = &pstDyn->stAtkChg;
			pstAtkChg->unPhsicBaseArmAtk += pstResult->SkillResultVal1;
			pstAtkChg->nPhsicAtkChgMul += pstResult->SkillResultVal2;
			pstAtkChg->iPhsicAtkChgAdd += pstResult->SkillResultVal3;
		 	break;	
		case RESULT_DEF_ICEHURT:
			pstAtkeeHurtChg->AtkeeIceHurtMul += pstResult->SkillResultVal1;
			pstAtkeeHurtChg->AtkeeIceHurtAdd += pstResult->SkillResultVal2;
			break;
		case RESULT_DEF_FIREHURT:
			pstAtkeeHurtChg->AtkeeFireHurtMul += pstResult->SkillResultVal1;
			pstAtkeeHurtChg->AtkeeFireHurtAdd += pstResult->SkillResultVal2;
			break;
		case RESULT_DEF_THUNDERHURT:
			pstAtkeeHurtChg->AtkeeThunderHurtMul += pstResult->SkillResultVal1;
			pstAtkeeHurtChg->AtkeeThunderHurtAdd += pstResult->SkillResultVal2;
			break;
		case RESULT_DEF_LIGHTHURT:
			pstAtkeeHurtChg->AtkeeLightHurtMul += pstResult->SkillResultVal1;
			pstAtkeeHurtChg->AtkeeLightHurtAdd += pstResult->SkillResultVal2;
			break;
		case RESULT_DEF_NIGHTHURT:
			pstAtkeeHurtChg->AtkeeNightHurtMul += pstResult->SkillResultVal1;
			pstAtkeeHurtChg->AtkeeNightHurtAdd += pstResult->SkillResultVal2;
			break;
		case RESULT_DEF_PHISICHURT:
			pstAtkeeHurtChg->AtkeePhsicHurtMul += pstResult->SkillResultVal1;
			pstAtkeeHurtChg->AtkeePhsicHurtAdd += pstResult->SkillResultVal2;
			break;	
		case RESULT_DEF_MAGICHURT:
			pstAtkeeHurtChg->AtkeeMagicHurtMul += pstResult->SkillResultVal1;
			pstAtkeeHurtChg->AtkeeMagicHurtAdd += pstResult->SkillResultVal2;
			break;
		case RESULT_ICEHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerIceHurtMul += pstResult->SkillResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerIceHurtAdd += pstResult->SkillResultVal2;
			break;
		case RESULT_FIREHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerFireHurtMul += pstResult->SkillResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerFireHurtAdd += pstResult->SkillResultVal2;
			break;	
		case RESULT_THUNDERHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerThunderHurtMul += pstResult->SkillResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerThunderHurtAdd += pstResult->SkillResultVal2;
			break;	
		case RESULT_LIGHTHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerLightHurtMul += pstResult->SkillResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerLightHurtAdd += pstResult->SkillResultVal2;
			break;		
		case RESULT_NIGHTHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerNightHurtMul += pstResult->SkillResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerNightHurtAdd += pstResult->SkillResultVal2;
			break;	
		case RESULT_PHISICHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerPhsicHurtMul += pstResult->SkillResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerPhsicHurtAdd += pstResult->SkillResultVal2;
			break;	
		case RESULT_MAGICHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerMagicHurtMul += pstResult->SkillResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerMagicHurtAdd += pstResult->SkillResultVal2;
			break;	
		
		case RESULT_HURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerHurtMul += pstResult->SkillResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerHurtAdd += pstResult->SkillResultVal2;
			break;

		case RESULT_DEF_HURT:
			pstAtkeeHurtChg->AtkeeHurtMul += pstResult->SkillResultVal1;
			pstAtkeeHurtChg->AtkeeHurtAdd += pstResult->SkillResultVal2;
			break;

		case RESULT_XIAN_LIST:
			pstDyn->stMidAttrList.bXianListAdd += pstResult->SkillResultVal1;
			break;

		case RESULT_SHI_LIST:
			pstDyn->stMidAttrList.bShiListAdd += pstResult->SkillResultVal1;
			break;
		case RESULT_XP_MAX:
			pstDyn->stHealChg.nMaxXpAdd += pstResult->SkillResultVal1;
			break;
		case RESULT_ADD_BASE_ATTR:
			/* 增加一级属性 1-力 2-智 3-体 4-精 被动 */
			pstDyn->stMidAttrList.iZhiliAdd += pstResult->SkillResultVal2;
			pstDyn->stMidAttrList.iLiliangAdd += pstResult->SkillResultVal1;
			pstDyn->stMidAttrList.iTiliAdd += pstResult->SkillResultVal3;
			pstDyn->stMidAttrList.iJingshenAdd += pstResult->SkillResultVal4;
			break;
		//一级属性防护
		case RESULT_ADD_BASE_ATTR_DEF:
			pstDyn->stMidAttrList.iFangyuMul += pstResult->SkillResultVal1;
			pstDyn->stMidAttrList.iFangyuAdd += pstResult->SkillResultVal2;
			//pstDyn->stMidAttrList.iMinJieMul += pstResult->SkillResultVal1;
			//pstDyn->stMidAttrList.iMinJieAdd += pstResult->SkillResultVal2;
			break;
		//一级属性力量
		case RESULT_ADD_BASE_ATTR_STR:
			pstDyn->stMidAttrList.iLiliangMul += pstResult->SkillResultVal1;
			pstDyn->stMidAttrList.iLiliangAdd += pstResult->SkillResultVal2;
			break;
		//一级属性智力
		case RESULT_ADD_BASE_ATTR_INT:
			pstDyn->stMidAttrList.iZhiliMul += pstResult->SkillResultVal1;
			pstDyn->stMidAttrList.iZhiliAdd += pstResult->SkillResultVal2;
			break;
		//一级属性体质
		case RESULT_ADD_BASE_ATTR_STA:
			pstDyn->stMidAttrList.iTiliMul += pstResult->SkillResultVal1;
			pstDyn->stMidAttrList.iTiliAdd += pstResult->SkillResultVal2;
			break;
		//一级属性精神
		case RESULT_ADD_BASE_ATTR_SPR:
			pstDyn->stMidAttrList.iJingshenMul += pstResult->SkillResultVal1;
			pstDyn->stMidAttrList.iJingshenAdd += pstResult->SkillResultVal2;
			break;
		/*
		//一级属性暴击
		case RESULT_ADD_BASE_ATTR_HEAVY_HIT:
			pstDyn->stMidAttrList.iBaojiMul += pstResult->SkillResultVal1;
			pstDyn->stMidAttrList.iBaojiAdd += pstResult->SkillResultVal2;
			break;
		//一级属性命中
		case RESULT_ADD_BASE_ATTR_HIT:
			pstDyn->stMidAttrList.iMingzhongMul += pstResult->SkillResultVal1;
			pstDyn->stMidAttrList.iMingzhongAdd += pstResult->SkillResultVal2;
			break;
		//一级属性闪避
		case RESULT_ADD_BASE_ATTR_FLEE:
			pstDyn->stMidAttrList.iShanbiMul += pstResult->SkillResultVal1;
			pstDyn->stMidAttrList.iShanbiAdd += pstResult->SkillResultVal2;
			break;
		*/
		case RESULT_DAMAGE:
			pstDyn->stAtkChg.nDamageMul += pstResult->SkillResultVal1;
			pstDyn->stAtkChg.unDamageAdd += pstResult->SkillResultVal2;
			break;

		// 被动技能 加血加魔
		case RESULT_ARM_HPADD:
			pstDyn->stHealChg.unArmHPAdd += pstResult->SkillResultVal1;
			break;

		case RESULT_ARM_MPADD:
			pstDyn->stHealChg.unArmMPAdd += pstResult->SkillResultVal1;
			break;
		case RESULT_ADD_SPEED_STATUS_FLEE:
			//函数实现
			break;
		/* 不良状态抗性 */
		case RESULT_BADSTATUS_DEF:
			pstDyn->stHealChg.ucBadStatusDefRate  += pstResult->SkillResultVal1;
			break;
		case RESULT_DEC_PRE_TIME_RATE:
			// 函数实现
			break;
		case RESULT_REDUCE_SKILL_CD_TIME_BY_HEAVYHIT:
			if( pstDyn->stHealChg.ushCDResultNum < MAX_CHG_SKILL_CD_NUM )
			{
				pstDyn->stHealChg.astCDResult[pstDyn->stHealChg.ushCDResultNum] = *pstResult;
				pstDyn->stHealChg.ushCDResultNum++;
			}
			break;
		case RESULT_ADD_SPEED:
			pstDyn->stHealChg.nSpeedMul += pstResult->SkillResultVal1;
			pstDyn->stHealChg.nSpeedAdd += pstResult->SkillResultVal2;
			break;
		case RESULT_RANDOM_ADD_STATUS_BY_ATK:
			// 函数实现
			break;
		case RESULT_ATK_RANDOM_ADD_HP:
			pstDyn->stHealChg.nAtkHPMul += pstResult->SkillResultVal1;
			pstDyn->stHealChg.nAtkHPAdd += pstResult->SkillResultVal2;
			pstDyn->stHealChg.nAtkHpRate += pstResult->SkillResultVal3;
			break;
		case RESULT_REDUCE_SKILL_CD_TIME:
			//函数实现
			break;
		case RESULT_ADD_SKILL_HURT: // 天赋 对指定技能增强
			//函数实现
			break;	
		case RESULT_ADD_SKILL_HEAVYHITRATE:
			//函数实现
			break;
		case RESULT_ATK_RANDOM_ADD_MP:
			pstDyn->stHealChg.nGeniusHealMPMul += pstResult->SkillResultVal1;
			pstDyn->stHealChg.unGeniusHealMPAdd += pstResult->SkillResultVal2;
			pstDyn->stHealChg.nGeniusAtkMPRate += pstResult->SkillResultVal3;
			break;
		case RESULT_ATK_RANDOM_ADD_HURT:
			pstDyn->stHealChg.nGeniusHealHurtMul += pstResult->SkillResultVal1;
			pstDyn->stHealChg.unGeniusHealHurtAdd += pstResult->SkillResultVal2;
			pstDyn->stHealChg.nGeniusAtkHurtRate += pstResult->SkillResultVal3;
			break;
		case RESULT_ATK_RANDOM_ADD_STATUS:
			//函数实现
			break;
		case RESULT_HEAVY_HIT:
			pstDyn->stHitRateChg.nHeavyHitAdd += pstResult->SkillResultVal1;
			break;
		case RESULT_HEAL_HP:
			pstDyn->stHealChg.nBaseHealHPMul += pstResult->SkillResultVal1;
			pstDyn->stHealChg.nHealHPMul += pstResult->SkillResultVal2;
			pstDyn->stHealChg.unHealHPAdd += pstResult->SkillResultVal3;
			break;
		case RESULT_HEAL_MP:
			pstDyn->stHealChg.nBaseHealMPMul += pstResult->SkillResultVal1;
			pstDyn->stHealChg.nHealMPMul += pstResult->SkillResultVal2;
			pstDyn->stHealChg.unHealMPAdd += pstResult->SkillResultVal3;
			break;	
		case RESULT_ICE_DEF:
			pstDyn->stDefChg.unIceDefBaseAdd += pstResult->SkillResultVal1;
			pstDyn->stDefChg.nIceDefChgMul += pstResult->SkillResultVal2;
			pstDyn->stDefChg.iIceDefChgAdd += pstResult->SkillResultVal3;
			break;
		case RESULT_PHSIC_DEF:
			pstDyn->stDefChg.unPhsicDefBaseAdd += pstResult->SkillResultVal1;
			pstDyn->stDefChg.nPhsicDefChgMul += pstResult->SkillResultVal2;
			pstDyn->stDefChg.iPhsicDefChgAdd += pstResult->SkillResultVal3;
			break;	
		case RESULT_FLEE:
			pstDyn->stFleeChg.unFleeBaseAdd += pstResult->SkillResultVal1;
			pstDyn->stFleeChg.nFleeChgMul += pstResult->SkillResultVal2;
			pstDyn->stFleeChg.iFleeChgAdd += pstResult->SkillResultVal3;
			break;
		case RESULT_COLLECTSKILL_ADDVAL:
			pstDyn->stHealChg.nGeniusCollectSkillAdd += pstResult->SkillResultVal1;
			break;
		case RESULT_COLLECTSKILL_DECVAL:
			//pstDyn->stHealChg.nGeniusCollectSkillDec += pstResult->SkillResultVal1;
			//函数实现
			break;
		case RESULT_COLLECTSKILL_MAXVAL:
			pstDyn->stHealChg.nGeniusCollectSkillMax += pstResult->SkillResultVal1;
			break;			
		case RESULT_TMPMACHINE_ADDSPD:
			//函数实现
			break;
		case RESULT_TMPITEM_ADDNUM:
			//函数实现
			break;
		case RESULT_TMPITEM_ADDENERGY:
			//函数实现
			break;	
		case RESULT_GN_SKILL_NUM_STAUTS:
			if (pstDyn->iGnUseSkill >= MAX_GN_TOUCH_BUFF)
			{
				return 0;
			}
			pstDyn->astGnUseSkill[pstDyn->iGnUseSkill].iStatusID = pstResult->SkillResultVal1;
			pstDyn->astGnUseSkill[pstDyn->iGnUseSkill].iLevel = pstResult->SkillResultVal2;
			pstDyn->astGnUseSkill[pstDyn->iGnUseSkill].iTime = pstResult->SkillResultVal3;
			pstDyn->astGnUseSkill[pstDyn->iGnUseSkill].iTouchNum = pstResult->SkillResultVal4%100;
			pstDyn->astGnUseSkill[pstDyn->iGnUseSkill].iSkillID = pstResult->SkillResultVal4/100;
			pstDyn->iGnUseSkill ++;
			break;
		
		case RESULT_ADD_KILL_HP:		// 击杀增加hp获取量
			pstDyn->stMidAttrList.uiKillAddHp += pstResult->SkillResultVal1;
			if (pstPlayer &&pstResult->SkillResultVal2 > 0)
			{
				pstDyn->stMidAttrList.uiKillAddHp += 
				pstResult->SkillResultVal2/100.0*z_get_player_hpmax(pstEnv->pstAppCtx, pstEnv, pstPlayer);
			}
		break;

		case RESULT_ADD_KILL_MP:		// 击杀增加mp获取量
			pstDyn->stMidAttrList.uiKillAddMp += pstResult->SkillResultVal1;
			if (pstPlayer &&pstResult->SkillResultVal2 > 0)
			{
				pstDyn->stMidAttrList.uiKillAddMp += 
				pstResult->SkillResultVal2/100.0*z_get_player_mpmax(pstEnv->pstAppCtx, pstEnv, pstPlayer);
			}
		break;

		case RESULT_HEAVY_HIT_VAL:		// 天赋增加暴击伤害
			pstDyn->stHitRateChg.nHeavyHitVal += pstResult->SkillResultVal1*100;
			break;
		case RESULT_ADD_BASE_ATTR_AGILE:
			pstDyn->stMidAttrList.iMinJieMul += pstResult->SkillResultVal1;
			pstDyn->stMidAttrList.iMinJieAdd += pstResult->SkillResultVal2;
			//pstDyn->stMidAttrList.iFangyuMul += pstResult->SkillResultVal1;
			//pstDyn->stMidAttrList.iFangyuAdd += pstResult->SkillResultVal2;
			break;
		case RESULT_WATER_ATK:
			pstDyn->stAtkChg.unWaterBaseArmAtk += pstResult->SkillResultVal1;
			pstDyn->stAtkChg.nWaterAtkChgMul += pstResult->SkillResultVal2;
			pstDyn->stAtkChg.iWaterAtkChgAdd += pstResult->SkillResultVal3;
			break;	
		case RESULT_SIEGE_ATK:
			pstDyn->stAtkChg.unSiegeBaseArmAtk += pstResult->SkillResultVal1;
			pstDyn->stAtkChg.nSiegeAtkChgMul += pstResult->SkillResultVal2;
			pstDyn->stAtkChg.iSiegeAtkChgAdd += pstResult->SkillResultVal3;
			break;	
		case RESULT_HOLY_ATK:
			pstDyn->stAtkChg.unHolyBaseArmAtk += pstResult->SkillResultVal1;
			pstDyn->stAtkChg.nHolyAtkChgMul += pstResult->SkillResultVal2;
			pstDyn->stAtkChg.iHolyAtkChgAdd += pstResult->SkillResultVal3;
			break;	
		case RESULT_MACHINE_ATK:
			pstDyn->stAtkChg.unMachineBaseArmAtk += pstResult->SkillResultVal1;
			pstDyn->stAtkChg.nMachineAtkChgMul += pstResult->SkillResultVal2;
			pstDyn->stAtkChg.iMachineAtkChgAdd += pstResult->SkillResultVal3;
			break;
		case RESULT_WATER_DEF:
			pstDyn->stDefChg.unWaterDefBaseAdd += pstResult->SkillResultVal1;
			pstDyn->stDefChg.nWaterDefChgMul += pstResult->SkillResultVal2;
			pstDyn->stDefChg.iWaterDefChgAdd += pstResult->SkillResultVal3;
			break;
		case RESULT_SIEGE_DEF:
			pstDyn->stDefChg.unSiegeDefBaseAdd += pstResult->SkillResultVal1;
			pstDyn->stDefChg.nSiegeDefChgMul += pstResult->SkillResultVal2;
			pstDyn->stDefChg.iSiegeDefChgAdd += pstResult->SkillResultVal3;
			break;
		case RESULT_HOLY_DEF:
			pstDyn->stDefChg.unHolyDefBaseAdd += pstResult->SkillResultVal1;
			pstDyn->stDefChg.nHolyDefChgMul += pstResult->SkillResultVal2;
			pstDyn->stDefChg.iHolyDefChgAdd += pstResult->SkillResultVal3;
			break;
		case RESULT_MACHINE_DEF:
			pstDyn->stDefChg.unMachineDefBaseAdd += pstResult->SkillResultVal1;
			pstDyn->stDefChg.nMachineDefChgMul += pstResult->SkillResultVal2;
			pstDyn->stDefChg.iMachineDefChgAdd += pstResult->SkillResultVal3;
			break;
		case RESULT_PET_OWNER_LILIANG:
			pstDyn->stMidAttrList.iLiliangMul += pstResult->SkillResultVal1;
			pstDyn->stMidAttrList.iLiliangAdd += pstResult->SkillResultVal2;
			break;
		case RESULT_PET_OWNER_GANZHI:
			pstDyn->stMidAttrList.iMinJieMul += pstResult->SkillResultVal1;
			pstDyn->stMidAttrList.iMinJieAdd += pstResult->SkillResultVal2;
			break;
		
		case RESULT_PET_OWNER_MINJIE:
			pstDyn->stMidAttrList.iFangyuMul += pstResult->SkillResultVal1;
			pstDyn->stMidAttrList.iFangyuAdd += pstResult->SkillResultVal2;
			break;
		case RESULT_PET_OWNER_JINGSHEN:
			pstDyn->stMidAttrList.iJingshenMul += pstResult->SkillResultVal1;
			pstDyn->stMidAttrList.iJingshenAdd += pstResult->SkillResultVal2;
			break;
		case RESULT_PET_OWNER_TIZHI:
			pstDyn->stMidAttrList.iTiliMul += pstResult->SkillResultVal1;
			pstDyn->stMidAttrList.iTiliAdd += pstResult->SkillResultVal2;
			break;
		case RESULT_PET_OWNER_ZHILI:
			pstDyn->stMidAttrList.iZhiliMul += pstResult->SkillResultVal1;
			pstDyn->stMidAttrList.iZhiliAdd += pstResult->SkillResultVal2;
			break;
		case RESULT_PET_OWNER_ICE_DEF:
			pstDyn->stDefChg.nIceDefChgMul += pstResult->SkillResultVal1;
			pstDyn->stDefChg.iIceDefChgAdd += pstResult->SkillResultVal2;
			break;
		case RESULT_PET_OWNER_PHI_DEF:
			pstDyn->stDefChg.nPhsicDefChgMul += pstResult->SkillResultVal1;
			pstDyn->stDefChg.iPhsicDefChgAdd += pstResult->SkillResultVal2;
			break;
		case RESULT_PET_OWNER_ICE_ATK:
			pstDyn->stAtkChg.nIceAtkChgMul += pstResult->SkillResultVal1;
			pstDyn->stAtkChg.iIceAtkChgAdd += pstResult->SkillResultVal2;
			break;
		case RESULT_PET_OWNER_PHI_ATK:
			pstDyn->stAtkChg.nPhsicAtkChgMul += pstResult->SkillResultVal1;
			pstDyn->stAtkChg.iPhsicAtkChgAdd += pstResult->SkillResultVal2;
			break;
		case RESULT_PET_OWNER_HP:
			pstDyn->stHealChg.nHPMaxMul += pstResult->SkillResultVal1;
			pstDyn->stHealChg.nHPMaxAdd += pstResult->SkillResultVal2;
			break;
		case RESULT_PET_OWNER_BAOJI_LEVEL:
			pstDyn->stHitRateChg.iHeavyHit += pstResult->SkillResultVal1;
			break;
		case RESULT_PET_OWNER_BAOJI_DIKANG:
			pstDyn->stHitRateChg.iDefHeavyHit +=pstResult->SkillResultVal1;
			break;
		case RESULT_PET_OWNER_BAOJI_SHANGHAI:
			pstDyn->stHitRateChg.nHeavyHitVal += pstResult->SkillResultVal1;
			break;
		case RESULT_PET_OWNER_POFANG_LEVEL:
			pstDyn->stDefChg.iIgnoreDef += pstResult->SkillResultVal1;
			break;
		case RESULT_PET_OWNER_DIKANG_LEVEL:
			pstDyn->stDefChg.iDefHP += pstResult->SkillResultVal1;
			break;
		default:
			break;
	}

	return 0;
}

int z_pet_add_player_attr_by_genius(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet)
{
	DIATHESISDEF *pstDiathesisDef;
	int j;
	SKILLRESULT *pstResult;
	
	if (pstRolePet->NativeSkill.SkillID <= 0)
	{
		return 0;
	}

	pstDiathesisDef = z_find_diathesis(pstEnv, pstRolePet->NativeSkill.SkillID, pstRolePet->NativeSkill.SkillLevel);
	if(!pstDiathesisDef)	
	{
		return 0;
	}

	for (j=0; j<(int)(sizeof(pstDiathesisDef->DiathesisResult)/sizeof(pstDiathesisDef->DiathesisResult[0])); j++)
	{
		pstResult = pstDiathesisDef->DiathesisResult + j;
		if (RESULT_NONE == pstResult->SkillResultID)
		{
			break;
		}

		z_calc_diathesis(pstEnv,pstPlayer, pstResult);
	}
	return 0;
}

int z_pet_add_player_attr(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet)
{
	int i;
	int j;
	SKILLRESULT *pstResult;
	DIATHESISDEF *pstDiathesisDef;
	
	for(i=0; i<pstRolePet->PetSkills.SkillGridNum; i++)
	{
		if(pstRolePet->PetSkills.SkillGrid[i].SkillID == 0)
		{
			continue;
		}

		if(pstRolePet->PetSkills.SkillGrid[i].GridType != PET_SKILL_OWNER_BUFF)
		{
			continue;
		}

		pstDiathesisDef = z_find_diathesis(pstEnv, pstRolePet->PetSkills.SkillGrid[i].SkillID, pstRolePet->PetSkills.SkillGrid[i].SkillLevel);
		if(!pstDiathesisDef)	
		{
			continue;
		}

		for (j=0; j<(int)(sizeof(pstDiathesisDef->DiathesisResult)/sizeof(pstDiathesisDef->DiathesisResult[0])); j++)
		{
			pstResult = pstDiathesisDef->DiathesisResult + j;
			if (RESULT_NONE == pstResult->SkillResultID)
			{
				break;
			}

			z_calc_diathesis(pstEnv,pstPlayer, pstResult);
		}
	}
	
	return 0;
}

int z_pet_diathesis_attr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	ROLEPET *pstRolePet;
	PetOnline *pstPetOnline = &pstPlayer->stOnline.stPetOnline;
	UNUSED(pstAppCtx);
	
	for(i=0; i<pstPetOnline->bFightShiNum; i++)
	{
		pstRolePet = z_find_role_pet(pstPlayer, pstPetOnline->astFightShi[i].stRolePetOnline.ullWID, NULL);
		if (NULL == pstRolePet)
		{
			continue;
		}

		z_pet_add_player_attr_by_genius(pstEnv, pstPlayer, pstRolePet);

		// 被动模式素质无效
		if(pstRolePet->Passive)
		{
			continue;
		}
		
		z_pet_add_player_attr(pstEnv, pstPlayer, pstRolePet);
	}

	if( pstPlayer->stOnline.stDyn.stHealChg.nSpeedMul > 0 ||
		pstPlayer->stOnline.stDyn.stHealChg.nSpeedAdd > 0	)
	{	
		// 有对速度的修正要下发给用户
		z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	return 0;
}

int z_fairy_diathesis_attr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i, j;
	FairyOnline *pstFairyOnline = &pstPlayer->stOnline.stFairyOnline;
	DIATHESISDEF *pstDiathesisDef;
	SKILLRESULT *pstResult;
	UNUSED(pstAppCtx);
	
	// 出征精灵被动技能计算到角色身上
	for (i=0; i<pstFairyOnline->iDiathNum; i++)
	{
		pstDiathesisDef = z_find_diathesis(pstEnv, pstFairyOnline->FairyDiathList[i].DiathID, pstFairyOnline->FairyDiathList[i].DiathLV);
		if (!pstDiathesisDef)
		{
			//tlog_error(pstEnv->pstLogCat, 0, 0, "DiathesisID %d DiathesisLevel %d no def", pstDiathesisData->DiathesisID, pstDiathesisData->DiathesisLevel);
			continue;
		}

		for (j=0; j<(int)(sizeof(pstDiathesisDef->DiathesisResult)/sizeof(pstDiathesisDef->DiathesisResult[0])); j++)
		{
			pstResult = pstDiathesisDef->DiathesisResult + j;
			if (RESULT_NONE == pstResult->SkillResultID)
			{
				break;
			}

			z_calc_diathesis(pstEnv,pstPlayer, pstResult);
		}
	}

	if( pstPlayer->stOnline.stDyn.stHealChg.nSpeedMul > 0 ||
		pstPlayer->stOnline.stDyn.stHealChg.nSpeedAdd > 0	)
	{	
		// 有对速度的修正要下发给用户
		z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}
	
	return 0;
}

int z_diathesis_attr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i, j;
	DIATHESISDEF *pstDiathesisDef;
	DIATHESISDATA *pstDiathesisData;
	SKILLRESULT *pstResult;
	DIATHESISINFO *pstDiathesisInfo = &pstPlayer->stRoleData.DiathesisInfo;
	
	pstPlayer->stOnline.stDyn.iGnUseSkill = 0;
	UNUSED(pstAppCtx);
	
	for (i=0; i<(int)pstDiathesisInfo->DiathesisNum; i++)
	{
		pstDiathesisData = &pstDiathesisInfo->DiathesisList[i];
		pstDiathesisDef = z_find_diathesis(pstEnv, pstDiathesisData->DiathesisID, pstDiathesisData->DiathesisLevel);
		if (!pstDiathesisDef)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "DiathesisID %d DiathesisLevel %d no def", pstDiathesisData->DiathesisID, pstDiathesisData->DiathesisLevel);
			continue;
		}

		for (j=0; j<(int)(sizeof(pstDiathesisDef->DiathesisResult)/sizeof(pstDiathesisDef->DiathesisResult[0])); j++)
		{
			pstResult = pstDiathesisDef->DiathesisResult + j;
			if (RESULT_NONE == pstResult->SkillResultID)
			{
				break;
			}

			z_calc_diathesis(pstEnv,pstPlayer, pstResult);
		}
	}

	if( pstPlayer->stOnline.stDyn.stHealChg.nSpeedMul > 0 ||
		pstPlayer->stOnline.stDyn.stHealChg.nSpeedAdd > 0	)
	{	
		// 有对速度的修正要下发给用户
		z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	return 0;
}

int get_attr_translate_val(DYNATTRLIST *pstDyn, int iType, int iRate)
{
	int iVal = 0;

	if(iRate <= 0)
	{
		return 0;
	}
	
	switch(iType)
	{
		case BASE_ATTR_TYPE_LILIANG:
			iVal = pstDyn->stMidAttrList.unLiliang*iRate;
			break;
		case BASE_ATTR_TYPE_ZHILI:
			iVal = pstDyn->stMidAttrList.unZhili*iRate;
			break;
		case BASE_ATTR_TYPE_TIZHI:
			iVal = pstDyn->stMidAttrList.unTili*iRate;
			break;
		case BASE_ATTR_TYPE_JINGSHEN:
			iVal = pstDyn->stMidAttrList.unJingshen*iRate;
			break;
		case BASE_ATTR_TYPE_FANGHU:
			iVal = pstDyn->stMidAttrList.unFangyu*iRate;
			break;
		case BASE_ATTR_TYPE_MINJIE:
			iVal = pstDyn->stMidAttrList.unMinJie*iRate;
			break;
		default:
			return 0;
			break;
	}

	return iVal;
}

int z_translate_career2_type(Player *pstPlayer)
{
	if(pstPlayer->stRoleData.Level <= CAREERCHG_LEVEL)
	{
		return 1;
	}

	return pstPlayer->stRoleData.Career;
}

//计算人物一级属性转化的某项二级属性
int z_dyn_attr_translate(ZONESVRENV* pstEnv, Player *pstPlayer, int iType)
{
	int i;
	int iVal = 0;
	int iCareer = z_translate_career2_type(pstPlayer);
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	ATTRCONTRASTDEF *pstAttrContrastDef;

	for(i=0; i<pstObj->iAttrContrastDef; i++)
	{
		//属性对照表，力 智。。。等等对应的属性成长
		pstAttrContrastDef = (ATTRCONTRASTDEF *)pstObj->sAttrContrastDef[i];

		if(pstAttrContrastDef->Career != iCareer)
		{
			continue;
		}

		switch(iType)
		{
			case BASE2ATTR_TYPE_HP:
				iVal += get_attr_translate_val(pstDyn, pstAttrContrastDef->BaseAttrID, pstAttrContrastDef->HP);
				break;
			case BASE2ATTR_TYPE_MP:
				iVal += get_attr_translate_val(pstDyn, pstAttrContrastDef->BaseAttrID, pstAttrContrastDef->MP);
				break;
			case BASE2ATTR_TYPE_PHSICATK:
				iVal += get_attr_translate_val(pstDyn, pstAttrContrastDef->BaseAttrID, pstAttrContrastDef->PhsicAtk);
				break;
			case BASE2ATTR_TYPE_PHSICDEF:
				iVal += get_attr_translate_val(pstDyn, pstAttrContrastDef->BaseAttrID, pstAttrContrastDef->PhsicDef);
				break;
			case BASE2ATTR_TYPE_ICEATK:
				iVal += get_attr_translate_val(pstDyn, pstAttrContrastDef->BaseAttrID, pstAttrContrastDef->IceAtk);
				break;
			case BASE2ATTR_TYPE_ICEDEF:
				iVal += get_attr_translate_val(pstDyn, pstAttrContrastDef->BaseAttrID, pstAttrContrastDef->IceDef);
				break;
			case BASE2ATTR_TYPE_HIT:
				iVal += get_attr_translate_val(pstDyn, pstAttrContrastDef->BaseAttrID, pstAttrContrastDef->Hit);
				break;
			case BASE2ATTR_TYPE_FLEE:
				iVal += get_attr_translate_val(pstDyn, pstAttrContrastDef->BaseAttrID, pstAttrContrastDef->Flee);
				break;
			case BASE2ATTR_TYPE_HEAVYHIT:
				iVal += get_attr_translate_val(pstDyn, pstAttrContrastDef->BaseAttrID, pstAttrContrastDef->HeavyHit);
				break;
			case BASE2ATTR_TYPE_DEFHEAVYHIT:
				iVal += get_attr_translate_val(pstDyn, pstAttrContrastDef->BaseAttrID, pstAttrContrastDef->DefHeavyHit);
				break;
			default:
				return 0;
				break;
		}
	}

	return iVal;
}


void z_dyn_tmp(ZONESVRENV * pstEnv, Player *pstPlayer, DYNATTRLIST *pstDyn)
{
	DEFCHG *pstDefChg;
	int iAtkMul;

	pstDefChg = &pstDyn->stDefChg;
	
	pstDyn->stMidAttrList.unLiliang = (pstDyn->stMidAttrList.unLiliang + pstDyn->stMidAttrList.iLiliangAdd)*(1+ pstDyn->stMidAttrList.iLiliangMul/100.0);
	pstDyn->stMidAttrList.unZhili = (pstDyn->stMidAttrList.unZhili + pstDyn->stMidAttrList.iZhiliAdd)*(1+ pstDyn->stMidAttrList.iZhiliMul/100.0);
	pstDyn->stMidAttrList.unJingshen = (pstDyn->stMidAttrList.unJingshen + pstDyn->stMidAttrList.iJingshenAdd)*(1+ pstDyn->stMidAttrList.iJingshenMul/100.0);
	pstDyn->stMidAttrList.unTili = (pstDyn->stMidAttrList.unTili + pstDyn->stMidAttrList.iTiliAdd) *(1 + pstDyn->stMidAttrList.iTiliMul/100.0);
	pstDyn->stMidAttrList.unFangyu = (pstDyn->stMidAttrList.unFangyu + pstDyn->stMidAttrList.iFangyuAdd)*(1 + pstDyn->stMidAttrList.iFangyuMul/100.0);
	pstDyn->stMidAttrList.unMinJie = (pstDyn->stMidAttrList.unMinJie + pstDyn->stMidAttrList.iMinJieAdd) *(1 + pstDyn->stMidAttrList.iMinJieMul/100.0);
	
	//命中
	pstDyn->stHitChg.unHitBaseAdd += z_dyn_attr_translate(pstEnv, pstPlayer, BASE2ATTR_TYPE_HIT);

	//闪避
	pstDyn->stFleeChg.unFleeBaseAdd += z_dyn_attr_translate(pstEnv, pstPlayer, BASE2ATTR_TYPE_FLEE);

	//防御
	pstDefChg->unPhsicDefBaseAdd += z_dyn_attr_translate(pstEnv, pstPlayer, BASE2ATTR_TYPE_PHSICDEF);
	pstDefChg->unIceDefBaseAdd += z_dyn_attr_translate(pstEnv, pstPlayer, BASE2ATTR_TYPE_ICEDEF);

	//攻击
	pstDyn->stAtkChg.unIceBaseArmAtk += z_dyn_attr_translate(pstEnv, pstPlayer, BASE2ATTR_TYPE_ICEATK);
	pstDyn->stAtkChg.unPhsicBaseArmAtk +=  z_dyn_attr_translate(pstEnv, pstPlayer, BASE2ATTR_TYPE_PHSICATK);

	//暴击力
	pstDyn->stHitRateChg.iHeavyHit += z_dyn_attr_translate(pstEnv, pstPlayer, BASE2ATTR_TYPE_HEAVYHIT);

	//暴击抵抗
	pstDyn->stHitRateChg.iDefHeavyHit += z_dyn_attr_translate(pstEnv, pstPlayer, BASE2ATTR_TYPE_DEFHEAVYHIT);
		
	//攻击系数
	iAtkMul = get_player_level_mul(pstEnv, pstPlayer, LEVEL_MUL_ATK);
	pstDyn->stAtkChg.iAtkLevelMul += iAtkMul;

	//防御系数
	pstDyn->stDefChg.iDefLevelMul += get_player_level_mul(pstEnv, pstPlayer, LEVEL_MUL_DEF);

	//等级增加2级属性
	pstDyn->stHitChg.unHitBaseAdd += get_player_level_attr(pstEnv, pstPlayer, BASE2ATTR_TYPE_HIT);
	pstDyn->stFleeChg.unFleeBaseAdd += get_player_level_attr(pstEnv, pstPlayer, BASE2ATTR_TYPE_FLEE);

	//暴击力
	pstDyn->stHitRateChg.iHeavyHit += get_player_level_attr(pstEnv, pstPlayer, BASE2ATTR_TYPE_HEAVYHIT);

	//暴击抵抗
	pstDyn->stHitRateChg.iDefHeavyHit += get_player_level_attr(pstEnv, pstPlayer, BASE2ATTR_TYPE_DEFHEAVYHIT);
	
	pstDefChg->unPhsicDefBaseAdd += get_player_level_attr(pstEnv, pstPlayer, BASE2ATTR_TYPE_PHSICDEF);
	pstDefChg->unIceDefBaseAdd += get_player_level_attr(pstEnv, pstPlayer, BASE2ATTR_TYPE_ICEDEF);

	pstDyn->stAtkChg.unIceBaseArmAtk += get_player_level_attr(pstEnv, pstPlayer, BASE2ATTR_TYPE_ICEATK);
	pstDyn->stAtkChg.unPhsicBaseArmAtk +=  get_player_level_attr(pstEnv, pstPlayer, BASE2ATTR_TYPE_PHSICATK);

	return;
}

int z_ride_attr_update(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	RESULT *pstResult;
	ITEMDEF *pstItemDef;
	HEALCHG *pstHealChg = &pstPlayer->stOnline.stDyn.stHealChg;
	UNUSED(pstAppCtx);
	
	if (!(pstPlayer->stOnline.State & CS_STAT_RIDE_ON) && !(pstPlayer->stOnline.State & CS_STAT_FLY))
	{
		return 0;
	}
	
	pstItemDef = z_find_itemdef(pstEnv, pstPlayer->stOnline.RideItemID);
	if (NULL == pstItemDef)
	{
		
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player %d Ride update no ItemDef", 
					pstPlayer->stRoleData.Uin, pstPlayer->stOnline.RideItemID);
		return -1;
	}
	
	pstResult = z_item_get_result(pstItemDef, RESULT_RIDE_MOVE);
	if (pstResult)
	{
		pstHealChg->nSpdRideBase += pstResult ->ResultVal1;
		pstHealChg->nSPdRideMul += pstResult ->ResultVal2;
		pstHealChg->nSPdRideAdd += pstResult ->ResultVal3;
	}

	pstResult = z_item_get_result(pstItemDef, RESULT_RIDE_POWER_RESTORE);
	if (pstResult)
	{
		pstHealChg->nSpdMoveRestore += pstResult->ResultVal1;
	}

	return 0;
}

LEVELATTR *get_career_level_attr(LEVELATTRDEF *pstLevelAttrDef, int iCareer)
{
	LEVELATTR *pstAttr = NULL;
	
	switch(iCareer)
	{
		case CAREER_WARRIOR:
		case CAREER_WARRIOR_SINGLEHAND:
			pstAttr = &pstLevelAttrDef->Warrior;
			break;
			
		case CAREER_WARRIOR_DOUBLEHAND:				
			pstAttr = &pstLevelAttrDef->SwordWarrior;
			break;
			
		case CAREER_MAGICOR:
		case CAREER_MAGICOR_SINGLEHAND:			
			pstAttr = &pstLevelAttrDef->Magician;
			break;
			
		case CAREER_MAGICOR_DOUBLEHAND:				
			pstAttr = &pstLevelAttrDef->FireMagician;
			break;
			
		case CAREER_GUNER:
		case CAREER_GUNER_SINGLEHAND:	
			pstAttr = &pstLevelAttrDef->Guner;
			break;
			
		case CAREER_GUNER_DOUBLEHAND:			
			pstAttr = &pstLevelAttrDef->DoubleGuner;
			break;
			
		case CAREER_CHURCH:
		case CAREER_CHURCH_SINGLEHAND:
			pstAttr = &pstLevelAttrDef->Church;
			break;
			
		case CAREER_CHURCH_DOUBLEHAND:			
			pstAttr = &pstLevelAttrDef->ThunderChurch;
			break;

		default:
			return NULL;
			break;
	}

	return pstAttr;
}

int get_player_level_attr(ZONESVRENV* pstEnv, Player *pstPlayer, int iType)
{
	LEVELATTRDEF *pstLevelAttrDef;
	LEVELATTR *pstAttr;

	pstLevelAttrDef = z_find_level_attr_def(pstEnv, pstPlayer->stRoleData.Level);
	if(NULL == pstLevelAttrDef)
	{
		return 0;
	}

	pstAttr = get_career_level_attr(pstLevelAttrDef, pstPlayer->stRoleData.Career);
	if(NULL == pstAttr)
	{
		return 0;
	}

	switch(iType)
	{
		case BASE2ATTR_TYPE_HP:
			return pstAttr->HP;
			break;
			
		case BASE2ATTR_TYPE_MP:
			return pstAttr->MP;
			break;
			
		case BASE2ATTR_TYPE_ICEATK:
			return pstAttr->IceAtk;
			break;
			
		case BASE2ATTR_TYPE_ICEDEF:
			return pstAttr->IceDef;
			break;

		case BASE2ATTR_TYPE_PHSICATK:
			return pstAttr->PhsicAtk;
			break;

		case BASE2ATTR_TYPE_PHSICDEF:
			return pstAttr->PhsicDef;
			break;
		
		case BASE2ATTR_TYPE_HIT:
			return pstAttr->Hit;
			break;

		case BASE2ATTR_TYPE_FLEE:
			return pstAttr->Flee;
			break;

		case BASE2ATTR_TYPE_HEAVYHIT:
			return pstAttr->HeavyHit;
			break;

		case BASE2ATTR_TYPE_DEFHEAVYHIT:
			return pstAttr->DefHeavyHit;
			break;
			
		default:
			return 0;
			break;
	}
		
	return 0;
}

int cacl_base_attr(TAPPCTX*pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	LEVELUP *pstLevelUp;
	MIDATTRLIST *pstMidAttrList = &pstPlayer->stOnline.stDyn.stMidAttrList;
	UNUSED(pstAppCtx);
	
	pstLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	if (!pstLevelUp)
	{
		return 0;
	}

	// 计算等级一级属性
	switch( pstPlayer->stRoleData.Career)
	{
		case CAREER_WARRIOR:
		case CAREER_WARRIOR_SINGLEHAND:
			pstMidAttrList->unLiliang += pstLevelUp->WarriorLiliang;
			pstMidAttrList->unZhili += pstLevelUp->WarriorZhili;
			pstMidAttrList->unFangyu += pstLevelUp->WarriorFangyu;
			pstMidAttrList->unTili += pstLevelUp->WarriorTili;
			pstMidAttrList->unJingshen += pstLevelUp->WarriorJingshen;
			pstMidAttrList->unMinJie += pstLevelUp->WarriorAgile;
			break;
		case CAREER_WARRIOR_DOUBLEHAND:				
			pstMidAttrList->unLiliang += pstLevelUp->SwordWarriorLiliang;
			pstMidAttrList->unZhili += pstLevelUp->SwordWarriorZhili;
			pstMidAttrList->unFangyu += pstLevelUp->SwordWarriorFangyu;
			pstMidAttrList->unTili += pstLevelUp->SwordWarriorTili;
			pstMidAttrList->unJingshen += pstLevelUp->SwordWarriorJingshen;
			pstMidAttrList->unMinJie += pstLevelUp->SwordWarriorAgile;
			break;
		case CAREER_MAGICOR:
		case CAREER_MAGICOR_SINGLEHAND:			
			pstMidAttrList->unLiliang += pstLevelUp->MagicorLiliang;
			pstMidAttrList->unZhili += pstLevelUp->MagicorZhili;
			pstMidAttrList->unFangyu += pstLevelUp->MagicorFangyu;
			pstMidAttrList->unTili += pstLevelUp->MagicorTili;
			pstMidAttrList->unJingshen += pstLevelUp->MagicorJingshen;
			pstMidAttrList->unMinJie += pstLevelUp->MagicorAgile;
			break;
		case CAREER_MAGICOR_DOUBLEHAND:				
			pstMidAttrList->unLiliang += pstLevelUp->FireMagicorLiliang;
			pstMidAttrList->unZhili += pstLevelUp->FireMagicorZhili;
			pstMidAttrList->unFangyu += pstLevelUp->FireMagicorFangyu;
			pstMidAttrList->unTili += pstLevelUp->FireMagicorTili;
			pstMidAttrList->unJingshen += pstLevelUp->FireMagicorJingshen;
			pstMidAttrList->unMinJie += pstLevelUp->FireMagicorAgile;
			break;
		case CAREER_GUNER:
		case CAREER_GUNER_SINGLEHAND:	
			pstMidAttrList->unLiliang += pstLevelUp->GunerLiliang;
			pstMidAttrList->unZhili += pstLevelUp->GunerZhili;
			pstMidAttrList->unFangyu += pstLevelUp->GunerFangyu;
			pstMidAttrList->unTili += pstLevelUp->GunerTili;
			pstMidAttrList->unJingshen += pstLevelUp->GunerJingshen;
			pstMidAttrList->unMinJie += pstLevelUp->GunerAgile;
			break;
		case CAREER_GUNER_DOUBLEHAND:			
			pstMidAttrList->unLiliang += pstLevelUp->DoubleGunerLiliang;
			pstMidAttrList->unZhili += pstLevelUp->DoubleGunerZhili;
			pstMidAttrList->unFangyu += pstLevelUp->DoubleGunerFangyu;
			pstMidAttrList->unTili += pstLevelUp->DoubleGunerTili;
			pstMidAttrList->unJingshen += pstLevelUp->DoubleGunerJingshen;
			pstMidAttrList->unMinJie += pstLevelUp->DoubleGunerAgile;
			break;
		case CAREER_CHURCH:
		case CAREER_CHURCH_SINGLEHAND:
			pstMidAttrList->unLiliang += pstLevelUp->ChurchLiliang;
			pstMidAttrList->unZhili += pstLevelUp->ChurchZhili;
			pstMidAttrList->unFangyu += pstLevelUp->ChurchFangyu;
			pstMidAttrList->unTili += pstLevelUp->ChurchTili;
			pstMidAttrList->unJingshen += pstLevelUp->ChurchJingshen;
			pstMidAttrList->unMinJie += pstLevelUp->ChurchAgile;
			break;
		case CAREER_CHURCH_DOUBLEHAND:			
			pstMidAttrList->unLiliang += pstLevelUp->ThunderChurchLiliang;
			pstMidAttrList->unZhili += pstLevelUp->ThunderChurchZhili;
			pstMidAttrList->unFangyu += pstLevelUp->ThunderChurchFangyu;
			pstMidAttrList->unTili += pstLevelUp->ThunderChurchTili;
			pstMidAttrList->unJingshen += pstLevelUp->ThunderChurchJingshen;
			pstMidAttrList->unMinJie += pstLevelUp->ThunderChurchAgile;
			break;
		default:
			return 0;
			break;
	}
	
	return 0;
}

int z_cacl_dyn_status_result(DYNATTRLIST *pstDyn, ZoneStatus *pstZoneStatus, STATUSRESULT *pstResult)
{
	ATKEEHURTCHG *pstAtkeeHurtChg = &pstDyn->stAtkeeHurtChg;
	DEFCHG *pstDefChg = &pstDyn->stDefChg;
	STATUSRESULT stResult;
	
	stResult = *pstResult;

	//状态叠加
	if(pstZoneStatus)
	{
		stResult.ResultVal1 = pstResult->ResultVal1*(1+pstZoneStatus->nOverlayMul/100.0);
		stResult.ResultVal2 = pstResult->ResultVal2*(1+pstZoneStatus->nOverlayMul/100.0);
		stResult.ResultVal3 = pstResult->ResultVal3*(1+pstZoneStatus->nOverlayMul/100.0);
		stResult.ResultVal4 = pstResult->ResultVal4*(1+pstZoneStatus->nOverlayMul/100.0);
	}
	
	switch(pstResult->ResultID)
	{
		case STATUS_RESULT_FEAR_ICEHURT:
			pstAtkeeHurtChg->AtkeeIceHurtMul -= stResult.ResultVal1;
			pstAtkeeHurtChg->AtkeeIceHurtAdd -= stResult.ResultVal2;
			
			break;
		case STATUS_RESULT_FEAR_FIREHURT:
			pstAtkeeHurtChg->AtkeeFireHurtMul -= stResult.ResultVal1;
			pstAtkeeHurtChg->AtkeeFireHurtAdd -= stResult.ResultVal2;
			
			break;
		case STATUS_RESULT_FEAR_THUNDERHURT:
			pstAtkeeHurtChg->AtkeeThunderHurtMul -= stResult.ResultVal1;
			pstAtkeeHurtChg->AtkeeThunderHurtAdd -= stResult.ResultVal2;
			
			break;
		case STATUS_RESULT_FEAR_LIGHTHURT:
			pstAtkeeHurtChg->AtkeeLightHurtMul -= stResult.ResultVal1;
			pstAtkeeHurtChg->AtkeeLightHurtAdd -= stResult.ResultVal2;
			
			break;
		case STATUS_RESULT_FEAR_NIGHTHURT:
			pstAtkeeHurtChg->AtkeeNightHurtMul -= stResult.ResultVal1;
			pstAtkeeHurtChg->AtkeeNightHurtAdd -= stResult.ResultVal2;
			
			break;
		case STATUS_RESULT_FEAR_PHISICHURT:
			pstAtkeeHurtChg->AtkeePhsicHurtMul -= stResult.ResultVal1;
			pstAtkeeHurtChg->AtkeePhsicHurtAdd -= stResult.ResultVal2;
			
			break;
		case STATUS_RESULT_DEF_ICEHURT:
			pstAtkeeHurtChg->AtkeeIceHurtMul += stResult.ResultVal1;
			pstAtkeeHurtChg->AtkeeIceHurtAdd += stResult.ResultVal2;
			
			break;
		case STATUS_RESULT_DEF_FIREHURT:
			pstAtkeeHurtChg->AtkeeFireHurtMul += stResult.ResultVal1;
			pstAtkeeHurtChg->AtkeeFireHurtAdd += stResult.ResultVal2;
			
			break;
		case STATUS_RESULT_DEF_THUNDERHURT:
			pstAtkeeHurtChg->AtkeeThunderHurtMul += stResult.ResultVal1;
			pstAtkeeHurtChg->AtkeeThunderHurtAdd += stResult.ResultVal2;
			
			break;
		case STATUS_RESULT_DEF_LIGHTHURT:
			pstAtkeeHurtChg->AtkeeLightHurtMul += stResult.ResultVal1;
			pstAtkeeHurtChg->AtkeeLightHurtAdd += stResult.ResultVal2;
			
			break;
		case STATUS_RESULT_DEF_NIGHTHURT:
			pstAtkeeHurtChg->AtkeeNightHurtMul += stResult.ResultVal1;
			pstAtkeeHurtChg->AtkeeNightHurtAdd += stResult.ResultVal2;
			
			break;
		case STATUS_RESULT_DEF_PHISICHURT:
			pstAtkeeHurtChg->AtkeePhsicHurtMul += stResult.ResultVal1;
			pstAtkeeHurtChg->AtkeePhsicHurtAdd += stResult.ResultVal2;
			
			break;
		case STATUS_RESULT_ICEATK:
			pstDyn->stAtkChg.nIceAtkChgMul += stResult.ResultVal1;
			pstDyn->stAtkChg.iIceAtkChgAdd += stResult.ResultVal2;
			break;
		case STATUS_RESULT_ICEATK_DEC:
			pstDyn->stAtkChg.nIceAtkChgMul -= stResult.ResultVal1;
			pstDyn->stAtkChg.iIceAtkChgAdd -= stResult.ResultVal2;
			break;	
		case STATUS_RESULT_FIREATK:
			pstDyn->stAtkChg.nFireAtkChgMul += stResult.ResultVal1;
			pstDyn->stAtkChg.iFireAtkChgAdd += stResult.ResultVal2;
			break;
		case STATUS_RESULT_THUNDERATK:
			pstDyn->stAtkChg.nThunderAtkChgMul += stResult.ResultVal1;
			pstDyn->stAtkChg.iThunderAtkChgAdd += stResult.ResultVal2;
			break;
		case STATUS_RESULT_LIGHTATK:
			pstDyn->stAtkChg.nLightAtkChgMul += stResult.ResultVal1;
			pstDyn->stAtkChg.iLightAtkChgAdd += stResult.ResultVal2;
			break;
		case STATUS_RESULT_NIGHTATK:
			pstDyn->stAtkChg.nNightAtkChgMul += stResult.ResultVal1;
			pstDyn->stAtkChg.iNightAtkChgAdd += stResult.ResultVal2;
			break;
		case STATUS_RESULT_PHISICATK:
			pstDyn->stAtkChg.nPhsicAtkChgMul += stResult.ResultVal1;
			pstDyn->stAtkChg.iPhsicAtkChgAdd += stResult.ResultVal2;
			break;
		case STATUS_RESULT_PHSICATK_DEC:
			pstDyn->stAtkChg.nPhsicAtkChgMul -= stResult.ResultVal1;
			pstDyn->stAtkChg.iPhsicAtkChgAdd -= stResult.ResultVal2;
			break;	
		case STATUS_RESULT_ICEDEF:
			pstDefChg->nIceDefChgMul += stResult.ResultVal1;
			pstDefChg->iIceDefChgAdd += stResult.ResultVal2;
			
			break;
		case STATUS_RESULT_ICEDEF_DEC:
			pstDefChg->nIceDefChgMul -= stResult.ResultVal1;
			pstDefChg->iIceDefChgAdd -= stResult.ResultVal2;
			
			break;	
		case STATUS_RESULT_FIREDEF:
			pstDefChg->nFireDefChgMul += stResult.ResultVal1;
			pstDefChg->iFireDefChgAdd += stResult.ResultVal2;
			
			break;
		case STATUS_RESULT_THUNDERDEF:
			pstDefChg->nThunderDefChgMul += stResult.ResultVal1;
			pstDefChg->iThunderDefChgAdd += stResult.ResultVal2;
			
			break;
		case STATUS_RESULT_LIGHTDEF:
			pstDefChg->nLightDefChgMul += stResult.ResultVal1;
			pstDefChg->iLightDefChgAdd += stResult.ResultVal2;
			break;
		case STATUS_RESULT_NIGHTDEF:
			pstDefChg->nNightDefChgMul += stResult.ResultVal1;
			pstDefChg->iNightDefChgAdd += stResult.ResultVal2;
			break;
			
		case STATUS_RESULT_PHISICDEF:
			pstDefChg->nPhsicDefChgMul += stResult.ResultVal1;
			pstDefChg->iPhsicDefChgAdd += stResult.ResultVal2;
			break;
		case STATUS_RESULT_PHSICDEF_DEC:
			pstDefChg->nPhsicDefChgMul -= stResult.ResultVal1;
			pstDefChg->iPhsicDefChgAdd -= stResult.ResultVal2;
			break;	
             case STATUS_RESULT_ICEHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerIceHurtMul += stResult.ResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerIceHurtAdd += stResult.ResultVal2;
			break;
		case STATUS_RESULT_ICEHURT_DEC:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerIceHurtMul -= stResult.ResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerIceHurtAdd -= stResult.ResultVal2;
			break;	
		case STATUS_RESULT_FIREHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerFireHurtMul += stResult.ResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerFireHurtAdd += stResult.ResultVal2;		
			break;
		case STATUS_RESULT_THUNDERHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerThunderHurtMul += stResult.ResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerThunderHurtAdd += stResult.ResultVal2;
			break;
		case STATUS_RESULT_LIGHTHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerLightHurtMul += stResult.ResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerLightHurtAdd += stResult.ResultVal2;
			break;
		case STATUS_RESULT_NIGHTHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerNightHurtMul += stResult.ResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerNightHurtAdd += stResult.ResultVal2;
			break;
		case STATUS_RESULT_PHSICHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerPhsicHurtMul += stResult.ResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerPhsicHurtAdd += stResult.ResultVal2;
			break;
		case STATUS_RESULT_PHSICHURT_DEC:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerPhsicHurtMul -= stResult.ResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerPhsicHurtAdd -= stResult.ResultVal2;
			break;	
		case STATUS_RESULT_MAGICHURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerMagicHurtMul += stResult.ResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerMagicHurtAdd += stResult.ResultVal2;
			break;
		case STATUS_RESULT_MAGICHURT_DEC:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerMagicHurtMul -= stResult.ResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerMagicHurtAdd -= stResult.ResultVal2;
			break;	
		case STATUS_RESULT_HURT:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerHurtMul += stResult.ResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerHurtAdd += stResult.ResultVal2;
			break;	
		case STATUS_RESULT_HURT_DEC:
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerHurtMul -= stResult.ResultVal1;
			pstDyn->stAtkChg.stAtkerHurtChg.AtkerHurtAdd -= stResult.ResultVal2;
			break;	
		case STATUS_RESULT_FIGHTVAL: //状态对战斗力的影响都是负的
			pstDyn->stMidAttrList.uiFight += stResult.ResultVal1;
				
			break;
		case STATUS_RESULT_BAOJI_RATE:
			pstDyn->stHitRateChg.nHeavyHitAdd += stResult.ResultVal1;
			
			break;
		case STATUS_RESULT_CHGSIZE:
			//客户端自己实现
			break;
		case STATUS_RESULT_DIAPHANEITY:
			//客户端自己实现
			break;
		case STATUS_RESULT_HIT_RATE:
			pstDyn->stHitRateChg.nAtkerHitRateAdd += pstResult->ResultVal1;
			pstDyn->stHitRateChg.nAtkerHitRateMul += pstResult->ResultVal2;
			break;
		case STATUS_RESULT_FLEE_VAL:
			pstDyn->stFleeChg.nFleeChgMul += pstResult->ResultVal1;
			pstDyn->stFleeChg.iFleeChgAdd += pstResult->ResultVal2;
			break;
		case STATUS_RESULT_HIT_VAL:
			pstDyn->stHitChg.nHitChgMul += pstResult->ResultVal1;
			pstDyn->stHitChg.iHitChgAdd += pstResult->ResultVal2;
			break;
		case STATUS_RESULT_HEAVY_HIT_VAL:
			pstDyn->stHitRateChg.nHeavyHitVal += pstResult->ResultVal1*100;
			break;
		case STATUS_RESULT_DEF_HURT:
			pstDyn->stAtkeeHurtChg.AtkeeHurtAdd += pstResult->ResultVal1;
			pstDyn->stAtkeeHurtChg.AtkeeHurtMul += pstResult->ResultVal2;
			break;
		case STATUS_RESULT_DAMAGE:
			pstDyn->stAtkChg.nDamageMul += pstResult->ResultVal1;
			pstDyn->stAtkChg.unDamageAdd += pstResult->ResultVal1;
			break;
		case STATUS_RESULT_HEAL_HP:
			pstDyn->stHealChg.nBaseHealHPMul += pstResult->ResultVal1;
			pstDyn->stHealChg.nHealHPMul += pstResult->ResultVal2;
			pstDyn->stHealChg.unHealHPAdd += pstResult->ResultVal3;
			break;
		case STATUS_RESULT_ATKEE_HEAL_HP:
			pstDyn->stHealChg.nAtkeeHealHPMul += pstResult->ResultVal1;
			pstDyn->stHealChg.unAtkeeHealHPAdd += pstResult->ResultVal2;
			break;
		case STATUS_RESULT_FEAR_WATERHURT:
			pstAtkeeHurtChg->AtkeeWaterHurtMul -= stResult.ResultVal1;
			pstAtkeeHurtChg->AtkeeWaterHurtAdd -= stResult.ResultVal2;
			
			break;
		case STATUS_RESULT_DEF_WATERHURT:
			pstAtkeeHurtChg->AtkeeWaterHurtMul += stResult.ResultVal1;
			pstAtkeeHurtChg->AtkeeWaterHurtAdd += stResult.ResultVal2;
			
			break;
		case STATUS_RESULT_WATERATK:
			pstDyn->stAtkChg.nWaterAtkChgMul += stResult.ResultVal1;
			pstDyn->stAtkChg.iWaterAtkChgAdd += stResult.ResultVal2;
			break;
		case STATUS_RESULT_WATERDEF:
			pstDefChg->nWaterDefChgMul += stResult.ResultVal1;
			pstDefChg->iWaterDefChgAdd += stResult.ResultVal2;
			
			break;
		case STATUS_RESULT_BASEATTR_HEAVY:
			pstDyn->stHitRateChg.iHeavyHit += stResult.ResultVal1;
			break;

		case STATUS_RESULT_BASE_ATTR_DEF:		/* 增加一级属性防护|val1-乘修正 val2-加修正 */
			pstDyn->stMidAttrList.iFangyuMul += pstResult->ResultVal1;
			pstDyn->stMidAttrList.iFangyuAdd += pstResult->ResultVal2;
			break;
		case STATUS_RESULT_BASE_ATTR_STR:		/* 增加一级属性力量|val1-乘修正 val2-加修正 */
			pstDyn->stMidAttrList.iLiliangMul += pstResult->ResultVal1;
			pstDyn->stMidAttrList.iLiliangAdd += pstResult->ResultVal2;
			break;
		case STATUS_RESULT_BASE_ATTR_INT:		/* 增加一级属性智力|val1-乘修正 val2-加修正 */
			pstDyn->stMidAttrList.iZhiliMul += pstResult->ResultVal1;
			pstDyn->stMidAttrList.iZhiliAdd += pstResult->ResultVal2;
			break;
		case STATUS_RESULT_BASE_ATTR_STA:		/* 增加一级属性体质|val1-乘修正 val2-加修正 */
			pstDyn->stMidAttrList.iTiliMul += pstResult->ResultVal1;
			pstDyn->stMidAttrList.iTiliAdd += pstResult->ResultVal2;
			break;
		case STATUS_RESULT_BASE_ATTR_SPR:		/* 增加一级属性精神|val1-乘修正 val2-加修正 */
			pstDyn->stMidAttrList.iJingshenMul += pstResult->ResultVal1;
			pstDyn->stMidAttrList.iJingshenAdd += pstResult->ResultVal2;
			break;
		case STATUS_RESULT_BASE_ATTR_AGILE:	/* 增加一级属性敏捷|val1-乘修正 val2-加修正 */
			pstDyn->stMidAttrList.iMinJieMul += pstResult->ResultVal1;
			pstDyn->stMidAttrList.iMinJieAdd += pstResult->ResultVal2;
			break;
		case STATUS_RESULT_DEF_HEAVY_HIT:
			pstDyn->stHitRateChg.iDefHeavyHit += pstResult->ResultVal1;
			break;
		case STATUS_RESULT_IGNORE_DEF:
			pstDyn->stDefChg.iIgnoreDef += pstResult->ResultVal1;
			break;
		case STATUS_RESULT_IGNORE_PHISIC_DEFEN:
			pstDyn->stAtkChg.iPhsicDecDefMul += pstResult->ResultVal1;
			pstDyn->stAtkChg.iPhsicDecDef += pstResult->ResultVal2;
			break;
		case STATUS_RESULT_IGNORE_MAGIC_DEFEN:
			pstDyn->stAtkChg.iIceDecDefMul += pstResult->ResultVal1;
			pstDyn->stAtkChg.iIceDecDef += pstResult->ResultVal2;
			break;
		case STATUS_RESULT_REDU_ATKEE_HEAL_HP:
			pstDyn->stHealChg.nReduceAtkeeHealHPMul += pstResult->ResultVal1;
			break;
		case STATUS_RESULT_ADD_EVADE:
			pstDyn->stHitRateChg.nAtkeeHitRateAdd += pstResult->ResultVal1;
			break;
		case STATUS_RESULT_DEF_HP:
			pstDyn->stDefChg.iDefHP += pstResult->ResultVal1;
			break;
		default:
			break;
	}
	
	return 0;
}

int z_status_attr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{	
	int i, j;
	STATUSDEF *pstStatusDef;
	ZoneStatus *pstZoneStatus;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	UNUSED(pstAppCtx);
	
	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;

		for (j=0; j<MAX_STATUS_RESULT; j++)
		{
			if (STATUS_RESULT_NONE == pstStatusDef->Result[j].ResultID)
			{
				break;
			}

			z_cacl_dyn_status_result(&pstPlayer->stOnline.stDyn, pstZoneStatus, &pstStatusDef->Result[j]);
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;

		for (j=0; j<MAX_STATUS_RESULT; j++)
		{
			if (STATUS_RESULT_NONE == pstStatusDef->Result[j].ResultID)
			{
				break;
			}

			z_cacl_dyn_status_result(&pstPlayer->stOnline.stDyn, pstZoneStatus, &pstStatusDef->Result[j]);
		}
	}

	for (i=0; i<pstStatusList->bNoLimitStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;

		for (j=0; j<MAX_STATUS_RESULT; j++)
		{
			if (STATUS_RESULT_NONE == pstStatusDef->Result[j].ResultID)
			{
				break;
			}

			z_cacl_dyn_status_result(&pstPlayer->stOnline.stDyn, pstZoneStatus, &pstStatusDef->Result[j]);
		}
	}
	
	return 0;
}

int get_fairy_base_xp(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	FAIRYDEF *pstFairyDef;
	ROLEFAIRY *pstRoleFairy = NULL;
	tdr_ulonglong ullWID = pstPlayer->stOnline.stFairyOnline.ullCampWID;
      	if ( ullWID > 0)
   	{
		pstRoleFairy = get_pack_fairy(pstPlayer, ullWID);
		if (pstRoleFairy)
		{
			pstFairyDef = faity_find(pstEnv, pstRoleFairy->FairyID, pstRoleFairy->FairyLevel);
			if (pstFairyDef)
			{
				return pstFairyDef->BaseXp;
			}
		}
   	}
	return 0;
}

int get_fairy_xp_upper(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	FAIRYDEF *pstFairyDef;
	ROLEFAIRY *pstRoleFairy = NULL;
	tdr_ulonglong ullWID = pstPlayer->stOnline.stFairyOnline.ullCampWID;
      	if ( ullWID > 0)
   	{
		pstRoleFairy = get_pack_fairy(pstPlayer, ullWID);
		if (pstRoleFairy)
		{
			pstFairyDef = faity_find(pstEnv, pstRoleFairy->FairyID, pstRoleFairy->FairyLevel);
			if (pstFairyDef)
			{
				return pstFairyDef->XpUpperLimit;
			}
		}
   	}
	return 0;
}

int calc_self_attr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	DYNATTRLIST stTmpDyn;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	CSPKG stPkg;
	CSPLAYERATTRVAL *pstAttrVal = &stPkg.Body.BaseAttrChg;
	
	// 计算一级属性
	cacl_base_attr(pstAppCtx, pstEnv, pstPlayer);
	
	// 计算装备的影响
	// 以后要加一级属性
	z_arm_attr(pstAppCtx, pstEnv, pstPlayer);	

	//计算自身素质的影响
	z_diathesis_attr(pstAppCtx, pstEnv, pstPlayer);

	achievement_addattr(pstEnv,pstPlayer);

	stTmpDyn = *pstDyn;
	
	//计算临时值
	z_dyn_tmp(pstEnv, pstPlayer, pstDyn);

	//基础数值 白色字
	pstPlayer->stOnline.stBaseAttrList.AttrNum = 0;
	z_get_attr_list(pstAppCtx, pstEnv, pstPlayer, &pstPlayer->stOnline.stBaseAttrList, 1);
	pstPlayer->stRoleData.MiscInfo.BaseAttrVal = pstPlayer->stOnline.stBaseAttrList;

	*pstAttrVal = pstPlayer->stOnline.stBaseAttrList;
	Z_CSHEAD_INIT(&stPkg.Head, BASE_ATTR_CHG);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	//恢复计算前。防止重复增加数值
	*pstDyn = stTmpDyn;

	return 0;
}

int calc_other_attr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	
	//计算状态的影响
	z_status_attr(pstAppCtx, pstEnv, pstPlayer);

	//宠物素质影响
	z_pet_diathesis_attr(pstAppCtx, pstEnv, pstPlayer);

	//精灵素质影响
	z_fairy_diathesis_attr(pstAppCtx, pstEnv, pstPlayer);

	// 计算宠物合体属性
	pet_heti_attr_move(pstEnv,pstPlayer);

	//符文额外附加数值
	rune_addattr_self(pstEnv, pstPlayer);

	// 晶石
	player_cacl_spar_attr(pstEnv, pstPlayer);

	// 计算方舟属性
//	fz_attr_cacl(pstEnv, pstPlayer);
	genius_cacl_attr(pstEnv, pstPlayer);

	//公会技能
	clan_skill_addattr_self(pstEnv,pstPlayer);
	
	//计算临时值,最终结果,必须放在最后面
	z_dyn_tmp(pstEnv, pstPlayer, pstDyn);

	if(pstPlayer->stRoleData.Career == CAREER_CHURCH_SINGLEHAND)
	{
		pstDyn->stHealChg.nHealHPMul += 
			pstDyn->stMidAttrList.unJingshen*10000.0/(pstDyn->stMidAttrList.unJingshen*2 + pstPlayer->stRoleData.Level*pstPlayer->stRoleData.Level/20);
	}
	
	return 0;
}
int z_get_attr_hitrate( ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iHit = 0;
	int iFlee  = 0;
	int iHitRate = 0;//万分率
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	LEVELUP *pstLevelUp;

	pstLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	if (!pstLevelUp)
	{
		return 0;
	}
	
	iHit = pstDyn->stHitChg.unHitBaseAdd*(1+pstDyn->stHitChg.nHitChgMul/100.0) + pstDyn->stHitChg.iHitChgAdd;
	iFlee = pstLevelUp->UIBaseFlee;
	if (iFlee > 0)
	{
		//命中率=0.6+攻击者命中等级*0.3/被击者闪避等级
		iHitRate = 60 + 0.3*(iHit*100/iFlee);
	}
	else
	{
		iHitRate = 100;
	}
	
	iHitRate = iHitRate*(1+pstDyn->stHitRateChg.nAtkerHitRateMul/100.0) + (pstDyn->stHitRateChg.nAtkerHitRateAdd);
	//if(iHitRate>100)
	//{
	//	iHitRate = 100;
	//}
	
	return (iHitRate*100);
}

int z_get_attr_hit( ZONESVRENV* pstEnv, Player *pstPlayer)
{
int iHit = 0;
int iFlee  = 0;
int iHitRate = 0;//万分率
DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
LEVELUP *pstLevelUp;

pstLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
if (!pstLevelUp)
{
    return 0;
}

iHit = pstDyn->stHitChg.unHitBaseAdd*(1+pstDyn->stHitChg.nHitChgMul/100.0) + pstDyn->stHitChg.iHitChgAdd;
iFlee = pstLevelUp->UIBaseFlee;
if (iFlee > 0)
{
    //命中率=0.6+攻击者命中等级*0.3/被击者闪避等级
    iHitRate = 60 + 0.3*(iHit*100/iFlee);
}
else
{
    iHitRate = 100;
}

iHitRate = iHitRate*(1+pstDyn->stHitRateChg.nAtkerHitRateMul/100.0) + (pstDyn->stHitRateChg.nAtkerHitRateAdd);
if(iHitRate>100)
{
    iHitRate = 100;
}

//反推
 
iHit = (iHitRate-60)*(10/3.0)/100.0*iFlee;
//return (iHitRate*100);	
return iHit;

}


int z_calc_player_attr_atk(DYNATTRLIST *pstDyn, int iAttrID)
{
	switch(iAttrID)
	{
	case ATTR_ID_PHSIC_ATK:
		return (pstDyn->stAtkChg.iPhsicMinAtk + pstDyn->stAtkChg.unPhsicBaseArmAtk)*
				(1+pstDyn->stAtkChg.nPhsicAtkChgMul/100.0)+pstDyn->stAtkChg.iPhsicAtkChgAdd;
		break;
	case ATTR_ID_PHSIC_MAX_ATK:
		return (pstDyn->stAtkChg.iPhsicMaxAtk +pstDyn->stAtkChg.unPhsicBaseArmAtk)*
				(1+pstDyn->stAtkChg.nPhsicAtkChgMul/100.0)+pstDyn->stAtkChg.iPhsicAtkChgAdd;
		break;
	case ATTR_ID_ICE_ATK:
		return (pstDyn->stAtkChg.iIceMinAtk + pstDyn->stAtkChg.unIceBaseArmAtk)*
				(1+pstDyn->stAtkChg.nIceAtkChgMul/100.0)+pstDyn->stAtkChg.iIceAtkChgAdd;
			break;
	case ATTR_ID_MAGIC_MAX_ATK:
		return (pstDyn->stAtkChg.iIceMaxAtk + pstDyn->stAtkChg.unIceBaseArmAtk)*
				(1+pstDyn->stAtkChg.nIceAtkChgMul/100.0)+pstDyn->stAtkChg.iIceAtkChgAdd;
		break;
	default:
		return 0;
		break;
	}
}

int z_get_attr_list(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSPLAYERATTRVAL *pstPlayerAttrVal, int fBaseAttr)
{
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_FIGHT;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.Fight = pstDyn->stMidAttrList.iPetFight + pstDyn->stMidAttrList.uiFight;
	pstPlayerAttrVal->AttrNum++;

	//基础数值计算的时候不处理HP MP
	if(!fBaseAttr)
	{
		pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_HPMAX;
		pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.HPMax = z_get_player_hpmax(pstAppCtx, pstEnv, pstPlayer);
		pstPlayerAttrVal->AttrNum++;
		
		pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_HP;
		pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.HP = pstPlayer->stRoleData.RoleDetail.HPCurr;
		pstPlayerAttrVal->AttrNum++;

		pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_MPMAX;
		pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.MPMax = z_get_player_mpmax(pstAppCtx, pstEnv, pstPlayer);
		pstPlayerAttrVal->AttrNum++;

		pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_MP;
		pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.MP = pstPlayer->stRoleData.RoleDetail.MPCurr;
		pstPlayerAttrVal->AttrNum++;

		pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_ZHENGYI;
		pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.Zhengyi= pstPlayer->stRoleData.RoleDetail.BlackJade;
		pstPlayerAttrVal->AttrNum++;
	}

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_LILIANG; 		//4 一级属性力量
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.Liliang = pstDyn->stMidAttrList.unLiliang;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_ZHILI;			//4 一级属性智力
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.Zhili = pstDyn->stMidAttrList.unZhili;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_FANGYU;		    //4 一级属性防御
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.Fangyu = pstDyn->stMidAttrList.unFangyu;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_TILI;		    //4 一级属性体力
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.Tili = pstDyn->stMidAttrList.unTili;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_JINGSHEN;		//4 一级属性精神
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.Jingshen = pstDyn->stMidAttrList.unJingshen;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_MINJIE;		//4 一级属性敏捷
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.MinJie = pstDyn->stMidAttrList.unMinJie;
	pstPlayerAttrVal->AttrNum++;
	
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_VIMMAX;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.VimMax = z_get_player_vimmax(pstAppCtx, pstEnv, pstPlayer);
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_VIM;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.Vim = pstPlayer->stRoleData.RoleDetail.VIMCurr;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_EXP;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.Exp = pstPlayer->stRoleData.RoleDetail.Exp;
	pstPlayerAttrVal->AttrNum++;

	//如果属性显示不包括素质等的影响，以下代码放入z_arm_attr 中

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_WATER_ATK;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.WaterAtk = z_cacl_attr_atk(&pstDyn->stAtkChg, SKILL_ATTR_WATER);
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_FIRE_ATK;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.FireAtk = z_cacl_attr_atk(&pstDyn->stAtkChg, SKILL_ATTR_FIRE);
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_THUNDER_ATK;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.ThunderAtk = z_cacl_attr_atk(&pstDyn->stAtkChg, SKILL_ATTR_THUNDER);
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_LIGHT_ATK;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.LightAtk = z_cacl_attr_atk(&pstDyn->stAtkChg, SKILL_ATTR_LIGHT);
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_NIGHT_ATK;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.NightAtk = z_cacl_attr_atk(&pstDyn->stAtkChg, SKILL_ATTR_NIGHT);
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_WATER_DEF;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.WaterDef = z_cacl_attr_def(&pstDyn->stDefChg, SKILL_ATTR_WATER);
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_FIRE_DEF;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.FireDef = z_cacl_attr_def(&pstDyn->stDefChg, SKILL_ATTR_FIRE);
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_THUNDER_DEF;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.ThunderDef = z_cacl_attr_def(&pstDyn->stDefChg, SKILL_ATTR_THUNDER);
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_LIGHT_DEF;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.LightDef = z_cacl_attr_def(&pstDyn->stDefChg, SKILL_ATTR_LIGHT);
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_NIGHT_DEF;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.NightDef = z_cacl_attr_def(&pstDyn->stDefChg, SKILL_ATTR_NIGHT);
	pstPlayerAttrVal->AttrNum++;
	
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_PHSIC_ATK;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.PhsicAtk = z_calc_player_attr_atk(pstDyn, ATTR_ID_PHSIC_ATK);
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_PHSIC_MAX_ATK;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.PhsicMaxAtk = z_calc_player_attr_atk(pstDyn, ATTR_ID_PHSIC_MAX_ATK);
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_ICE_ATK;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.IceAtk = z_calc_player_attr_atk(pstDyn, ATTR_ID_ICE_ATK);
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_MAGIC_MAX_ATK;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.MagicMaxAtk = z_calc_player_attr_atk(pstDyn, ATTR_ID_MAGIC_MAX_ATK);
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_PHSIC_DEF;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.PhsicDef = z_cacl_attr_def(&pstDyn->stDefChg, SKILL_ATTR_PHSIC); 
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_ICE_DEF;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.IceDef = z_cacl_attr_def(&pstDyn->stDefChg, SKILL_ATTR_ICE); 
	pstPlayerAttrVal->AttrNum++;
	
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_DEF_PHSIC_HURT;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.DefPhsicHurt = pstDyn->stAtkeeHurtChg.AtkeePhsicHurtAdd;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_DEF_ICE_HURT;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.DefIceHurt = pstDyn->stAtkeeHurtChg.AtkeeIceHurtAdd;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_PHSIC_HURT;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.PhsicHurt = pstDyn->stAtkChg.stAtkerHurtChg.AtkerPhsicHurtMul;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_ICE_HURT;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.IceHurt = pstDyn->stAtkChg.stAtkerHurtChg.AtkerIceHurtMul;
	pstPlayerAttrVal->AttrNum++;
	
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_HEAVY;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.Heavy = pstDyn->stHitRateChg.nHeavyHitAdd;
	pstPlayerAttrVal->AttrNum++;	

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_HIT;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.Hit = pstDyn->stHitChg.unHitBaseAdd*(1+pstDyn->stHitChg.nHitChgMul/100.0) + pstDyn->stHitChg.iHitChgAdd;
	//pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.Hit = z_get_attr_hit( pstEnv, pstPlayer);
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_HIT_RATE;
	//pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.HitRate = pstDyn->stHitRateChg.nAtkerHitRateAdd;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.HitRate = z_get_attr_hitrate( pstEnv, pstPlayer);
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_FLEE;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.Flee = pstDyn->stFleeChg.unFleeBaseAdd*(1+pstDyn->stFleeChg.nFleeChgMul/100.0) + pstDyn->stFleeChg.iFleeChgAdd;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_DAMAGE;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.Damage = pstDyn->stAtkChg.nArmLevChgMul*(1+pstDyn->stAtkChg.nDamageMul/100.0)+pstDyn->stAtkChg.unDamageAdd;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_NOFIGHT_HPADD;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.NofightHPAdd= pstDyn->stHealChg.unNoFightRestoreHPAdd;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_NOFIGHT_MPADD;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.NofightMPAdd= pstDyn->stHealChg.unNoFightRestoreMPAdd;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_BADSTATUS_DEF;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.BadStatusDefRate = pstDyn->stHealChg.ucBadStatusDefRate;
	pstPlayerAttrVal->AttrNum++;
	
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_HPADD;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.HPAdd = pstDyn->stHealChg.unArmHPAdd;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_MPADD;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.MPAdd = pstDyn->stHealChg.unArmMPAdd;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_XP;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.XP= pstPlayer->stRoleData.RoleDetail.XP;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_XPMAX;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.XPMax= z_get_player_xpmax(pstEnv,pstPlayer);
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_VIGORMAX;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.VigorMax= z_get_player_max_vig(pstEnv, pstPlayer);
	pstPlayerAttrVal->AttrNum++;
	
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_VIGOR;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.Vigor= pstPlayer->stRoleData.RoleDetail.GreenJade;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_MSPD;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.MSpd = z_get_player_mspd(pstEnv->pstAppCtx, pstEnv,pstPlayer);
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_HEAL_HP_ADD;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.HealHPAdd = pstDyn->stHealChg.unHealHPAdd;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_HEAL_HP_MUL;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.HealHPMul = pstDyn->stHealChg.nHealHPMul;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_ATKEE_HEAL_HP_ADD;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.AtkeeHealHPAdd = pstDyn->stHealChg.unAtkeeHealHPAdd;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_ATKEE_HEAL_HP_MUL;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.AtkeeHealHPMul = pstDyn->stHealChg.nAtkeeHealHPMul;
	pstPlayerAttrVal->AttrNum++;

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_BASE_ATTR_HEAVY;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.BaseAttrHeavy = pstDyn->stHitRateChg.iHeavyHit;
	pstPlayerAttrVal->AttrNum++;	

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_BASE_ATTR_DEFHEAVY;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.BaseAttrDefHeavy = pstDyn->stHitRateChg.iDefHeavyHit;
	pstPlayerAttrVal->AttrNum++;	

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_HEAVY_VAL;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.HeavyVal = pstDyn->stHitRateChg.nHeavyHitVal;
	pstPlayerAttrVal->AttrNum++;	

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_REFLECTION_HP;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.ReflectionHP = pstDyn->stDefChg.iReflectHP;
	pstPlayerAttrVal->AttrNum++;	

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_IGNORE_DEF;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.IgnoreDef = pstDyn->stDefChg.iIgnoreDef;
	pstPlayerAttrVal->AttrNum++;	

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_DEF_HP;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.DefHP = pstDyn->stDefChg.iDefHP;
	pstPlayerAttrVal->AttrNum++;	

	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].AttrID = ATTR_ID_DODGE;
	pstPlayerAttrVal->AttrList[pstPlayerAttrVal->AttrNum].Value.Dodge = pstDyn->stHitRateChg.nAtkeeHitRateAdd;
	pstPlayerAttrVal->AttrNum++;	

	//pstPlayer->stRoleData.MiscInfo.AttrVal = *pstPlayerAttrVal;
	return 0;
}

int z_calc_all_attr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSPLAYERATTRVAL *pstPlayerAttrVal)
{
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	unsigned int uiOldMaxHP = pstDyn->stMidAttrList.uiHPMax;
	unsigned int uiOldMaxMP = pstDyn->stMidAttrList.uiMPMax;
	unsigned int uiOldZhengyi = pstDyn->stMidAttrList.uiDayZhengyiMax;
	unsigned short unOldDayLifeBrandMax = pstDyn->stMidAttrList.usDayLifeBrandMax;
	
	memset(pstDyn, 0, sizeof(pstPlayer->stOnline.stDyn));

	pstDyn->stMidAttrList.uiDayZhengyiMax = uiOldZhengyi;
	pstDyn->stMidAttrList.usDayLifeBrandMax = unOldDayLifeBrandMax;

	pstDyn->iRoleLvl = pstPlayer->stRoleData.Level;
	//计算自身的属性
	calc_self_attr(pstAppCtx, pstEnv, pstPlayer);

	//计算额外的属性
	calc_other_attr(pstAppCtx, pstEnv, pstPlayer);
	
	pstDyn->stMidAttrList.uiHPMax = z_get_player_hpmax(pstAppCtx, pstEnv, pstPlayer);

	pstDyn->stMidAttrList.uiMPMax = z_get_player_mpmax(pstAppCtx, pstEnv, pstPlayer);

	pstDyn->stMidAttrList.uiVimMax = z_get_player_vimmax(pstAppCtx, pstEnv, pstPlayer);

	pstDyn->stMidAttrList.uiXpMax = z_get_player_xpmax(pstEnv,pstPlayer);

	pstDyn->stMidAttrList.uiVigorMax = z_get_player_max_vig(pstEnv, pstPlayer);

	//ATTR_CHG 所有属性显示信息
	z_get_attr_list(pstAppCtx, pstEnv, pstPlayer, pstPlayerAttrVal, 0);
	pstPlayer->stRoleData.MiscInfo.AttrVal = *pstPlayerAttrVal;

	if(uiOldMaxHP != pstDyn->stMidAttrList.uiHPMax || uiOldMaxMP != pstDyn->stMidAttrList.uiMPMax)
	{
		//更新最大血量
		player_hp_action(pstEnv, pstPlayer, pstPlayer->stRoleData.RoleDetail.HPCurr, 
				pstPlayer->stRoleData.RoleDetail.MPCurr, 0);
	}

	// 保存在这里,以免每次sec都要读表
	pstDyn->stMidAttrList.uiSitRestoreHP = get_player_sit_restore_hp(pstAppCtx, pstEnv, pstPlayer);
	pstDyn->stMidAttrList.uiSitRestoreMP = get_player_sit_restore_mp(pstAppCtx, pstEnv, pstPlayer);
	pstDyn->stMidAttrList.uiSitAddExp = get_player_sit_add_exp(pstAppCtx, pstEnv, pstPlayer);
	pstDyn->stMidAttrList.iSitConsumeVigor = GAOJI_SIT_CONSUME_VIGOR*(1+pstDyn->stHealChg.nSitVigorMul/100.0);
	pstDyn->stMidAttrList.uiDayExpMax = get_player_dayexplimit(pstAppCtx, pstEnv, pstPlayer);
	
	pstDyn->stHealChg.nBaseXp = get_fairy_base_xp(pstEnv, pstPlayer);

	//装备附加技能
	z_arm_add_skill_update(pstEnv, pstPlayer);
	z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);

	//状态改变技能等级
	z_status_chg_skill_level(pstEnv,pstPlayer);
	return 0;
}

int role_login_attr_chg(ZONESVRENV* pstEnv, Player *pstPlayer, CSPLAYERATTRVAL *pstPlayerAttrVal)
{
	CSPKG stPkg;
	CSPLAYERATTRVAL *pstAttrVal;

	//参考值
	pstAttrVal = &stPkg.Body.BaseAttrChg;
	*pstAttrVal = pstPlayer->stOnline.stBaseAttrList;

	Z_CSHEAD_INIT(&stPkg.Head, BASE_ATTR_CHG);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	//当前属性值
	pstAttrVal = &stPkg.Body.AttrChg;
	*pstAttrVal = *pstPlayerAttrVal;
	
	Z_CSHEAD_INIT(&stPkg.Head, ATTR_CHG);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}


int z_update_all_attr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSPLAYERATTRVAL *pstAttrVal = &stPkg.Body.AttrChg;
	DYNATTRLIST stDyn = pstPlayer->stOnline.stDyn;
	
	pstAttrVal->AttrNum = 0;
	z_calc_all_attr(pstAppCtx, pstEnv, pstPlayer, pstAttrVal);

	Z_CSHEAD_INIT(&stPkg.Head, ATTR_CHG);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	range_attr_set(pstEnv, pstPlayer,&stDyn);
	return 0;
}

int z_attr_chg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, PlayerChagAttrVal *pstVal, int iNotify)
{
	int i;
	int iChg;
	//int iSysType;
	CSPKG stPkg;
	AttrMapper *pstMap;
	AttrChgVal *pstAttrChg;
	long long llOldVal;
	CSPKG *pstPkg = &stPkg;
	CSPLAYERATTRVAL *pstChg = &pstPkg->Body.AttrChg;

	pstChg->AttrNum = 0;
	for (i=0; i<pstVal->AttrNum; i++)
	{
		pstAttrChg = &pstVal->AttrList[i];
		if (pstAttrChg->AttrID >= (int)ATTR_MAP_SIZE || pstAttrChg->AttrID < 0)
		{
			return -1;	
		}
		
		pstMap = &s_mapper[pstAttrChg->AttrID];
				
		if (pstMap->AttrAddFun )
		{
			pstMap->AttrAddFun(pstAppCtx, pstEnv, pstPlayer, pstAttrChg, pstChg);
			continue;
		}

		iChg = 0;
		switch(pstMap->bDataType)
		{
			case ATTR_TYPE_UNSHORT:
				llOldVal = *((unsigned short*)((char *)pstPlayer + pstMap->iOff));
				if (0 > z_add_one_baseattr(pstPlayer, pstAttrChg->AttrID, pstVal->AttrList[i].Value.Liliang, pstVal->AttrList[i].nFlag))
				{
					return -1;
				}

				iChg = *((unsigned short*)((char *)pstPlayer + pstMap->iOff)) - llOldVal;

				if (llOldVal == *((unsigned short*)((char *)pstPlayer + pstMap->iOff)))
				{
					break;
				}
				
				pstChg->AttrList[pstChg->AttrNum].AttrID = pstVal->AttrList[i].AttrID;
				pstChg->AttrList[pstChg->AttrNum].Value.Liliang= *((unsigned short*)((char *)pstPlayer + pstMap->iOff));
				pstChg->AttrNum++;
				break;
			case ATTR_TYPE_UNINT:
				// 禁止回蓝回血的快速实现
				switch(pstAttrChg->AttrID)
				{
				case ATTR_ID_HP:
					if(pstAttrChg->nFlag == 0 &&
					   (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_CANNOT_ADDHP))
					{
						return 0;
					}
					break;
				case ATTR_ID_MP:
					if(pstAttrChg->nFlag == 0 &&
					   pstPlayer->stOnline.cMiscFlag & MISC_FLAG_CANNOT_ADDMP)
					{
						return 0;
					}
					break;
				default:
					break;
				}
			
				llOldVal = *((unsigned int*)((char *)pstPlayer + pstMap->iOff));
				if (0 > z_add_one_baseattr(pstPlayer, pstAttrChg->AttrID, pstVal->AttrList[i].Value.Exp, pstVal->AttrList[i].nFlag))
				{
					return -1;
				}

				iChg = *((unsigned int*)((char *)pstPlayer + pstMap->iOff)) - llOldVal;

				if (llOldVal == *((unsigned int*)((char *)pstPlayer + pstMap->iOff)))
				{
					break;
				}

				pstChg->AttrList[pstChg->AttrNum].AttrID = pstVal->AttrList[i].AttrID;
				pstChg->AttrList[pstChg->AttrNum].Value.Exp = *((unsigned int*)((char *)pstPlayer + pstMap->iOff));
				pstChg->AttrNum++;
				break;
			case ATTR_TYPE_UNCHAR:
				llOldVal = *((unsigned char*)((char *)pstPlayer + pstMap->iOff));
				if (0 > z_add_one_baseattr(pstPlayer, pstAttrChg->AttrID, pstVal->AttrList[i].Value.Level, pstVal->AttrList[i].nFlag))
				{
					return -1;
				}

				iChg = *((unsigned char*)((char *)pstPlayer + pstMap->iOff)) - llOldVal;

				if (llOldVal == *((unsigned char*)((char *)pstPlayer + pstMap->iOff)))
				{
					return -1;
				}

				pstChg->AttrList[pstChg->AttrNum].AttrID = pstVal->AttrList[i].AttrID;
				pstChg->AttrList[pstChg->AttrNum].Value.Level = *((unsigned char*)((char *)pstPlayer + pstMap->iOff));
				pstChg->AttrNum++;
				break;
			case ATTR_TYPE_STRUCT:
				pstChg->AttrList[pstChg->AttrNum].AttrID = pstVal->AttrList[i].AttrID;
				memcpy(&pstChg->AttrList[pstChg->AttrNum].Value, &pstVal->AttrList[i].Value, sizeof(pstChg->AttrList[pstChg->AttrNum].Value));
				pstChg->AttrNum++;
				break;
			default:
				return -1;
				break;
		}

		if (0 == iNotify || NULL == pstMap->pszNotify) continue;

		/*
		if (1 == iNotify)
		{
			iSysType = SYS_TIP;
		}
		else
		{
			iSysType = SYS_COMBAT;
		}
		*/
		

		if (iChg > 0)
		{

			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_ATTR1, iChg, pstMap->pszNotify);

			if(iNotify  == 1)
			{
				z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ATTR1, iChg, pstMap->pszNotify);
			}
		}
		else if (iChg < 0)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_ATTR2, -iChg, pstMap->pszNotify);

			if(iNotify  == 1)
			{
				z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ATTR2, -iChg, pstMap->pszNotify);
			}
		}
	}

	if (pstChg->AttrNum > 0 && iNotify)
	{
		Z_CSHEAD_INIT(&pstPkg->Head, ATTR_CHG);
		z_cltmsg_send(pstEnv, pstPlayer, pstPkg);
	}
	
	return 0;
}

char *z_get_money_name(int iMoneyType)
{
	static char szName[32] = "未知";
		
	if ((unsigned int)iMoneyType >= ATTR_MAP_SIZE)
	{
		return szName;
	}

	if (NULL == s_mapper[iMoneyType].pszNotify)
	{
		return szName;
	}

	return s_mapper[iMoneyType].pszNotify;
}

int player_update_cache_level(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	UinCache *pstUinCache;
	int iValid;

	pstUinCache = (UinCache *)sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos, &iValid);
	if (pstUinCache && iValid)
	{
		for (i=0; i<pstUinCache->ucRoleNum; i++)
		{
			if (pstUinCache->astRoleListInfo[i].RoleID == pstPlayer->stRoleData.RoleID)
			{
				pstUinCache->astRoleListInfo[i].Level = pstPlayer->stRoleData.Level;
				break;
			}
		}
	}
	
	return 0;
}
int player_update_cache_career(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	UinCache *pstUinCache;
	int iValid;

	pstUinCache = (UinCache *)sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos, &iValid);
	if (pstUinCache && iValid)
	{
		for (i=0; i<pstUinCache->ucRoleNum; i++)
		{
			if (pstUinCache->astRoleListInfo[i].RoleID == pstPlayer->stRoleData.RoleID)
			{
				pstUinCache->astRoleListInfo[i].Career = pstPlayer->stRoleData.Career;
				break;
			}
		}
	}
	
	return 0;
}

void pet_attr_result(RESULT *pstResult, Player *pstPlayer,PetPowerOnline *pstPetPowerOnline, PetExpOnline *pstPetExpOnline, PetFightAttrOnline *pstPetFightAttrOnline, PetTiShenOnline *pstPetTiShenOnline)
{
//	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	switch (pstResult->ResultID)
	{
		case RESULT_PHISICHURT:
			if (pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stAtkChg.stAtkerHurtChg.AtkerPhsicHurtMul += pstResult->ResultVal1;
				pstPetFightAttrOnline->stAtkChg.stAtkerHurtChg.AtkerPhsicHurtAdd += pstResult->ResultVal2;
			}
			break;
		case RESULT_MAGICHURT:
			if (pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stAtkChg.stAtkerHurtChg.AtkerMagicHurtMul += pstResult->ResultVal1;
				pstPetFightAttrOnline->stAtkChg.stAtkerHurtChg.AtkerMagicHurtAdd += pstResult->ResultVal2;
			}
			break;
		case RESULT_HURT:
			if (pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stAtkChg.stAtkerHurtChg.AtkerHurtMul += pstResult->ResultVal1;
				pstPetFightAttrOnline->stAtkChg.stAtkerHurtChg.AtkerHurtAdd += pstResult->ResultVal2;
			}
			break;
		case RESULT_DEF_PHISICHURT:
			if (pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stAtkeeChg.AtkeePhsicHurtMul += pstResult->ResultVal1;
				pstPetFightAttrOnline->stAtkeeChg.AtkeePhsicHurtAdd += pstResult->ResultVal2;
			}
			break;
		case RESULT_DEF_MAGICHURT:
			if (pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stAtkeeChg.AtkeeMagicHurtMul += pstResult->ResultVal1;
				pstPetFightAttrOnline->stAtkeeChg.AtkeeMagicHurtAdd += pstResult->ResultVal2;
			}
			break;
		case RESULT_DEF_HURT:
			if (pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stAtkeeChg.AtkeeHurtMul += pstResult->ResultVal1;
				pstPetFightAttrOnline->stAtkeeChg.AtkeeHurtAdd += pstResult->ResultVal2;
			}
			break;
		case RESULT_HITRATE:
			if (pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stHitChg.nHitChgMul += pstResult->ResultVal1;
				pstPetFightAttrOnline->stHitChg.iHitChgAdd += pstResult->ResultVal2;
			}
			break;
		case RESULT_HEAVY_HIT:
			if (pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stHitRateChg.nHeavyHitAdd += pstResult->ResultVal1;
			}
			break;
		case RESULT_FLEE:
			if (pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stFleeChg.nFleeChgMul += pstResult->ResultVal1;
				pstPetFightAttrOnline->stFleeChg.iFleeChgAdd += pstResult->ResultVal2;
			}
			break;
		case RESULT_ARM_HPADD:
		case RESULT_ARM_MPADD:	
			if (pstPetPowerOnline)
			{
				pstPetPowerOnline->nPetPowerRestoreAdd += pstResult->ResultVal1;
			}
			break;
		case RESULT_HPMAX:
		case RESULT_MPMAX:	
			if (pstPetPowerOnline)
			{
				pstPetPowerOnline->iPowerMaxBase += pstResult->ResultVal1;
				pstPetPowerOnline->nPowerMaxMul += pstResult->ResultVal2; 
				pstPetPowerOnline->nPowerMaxAdd += pstResult->ResultVal3; 
			}
			break;
		case RESULT_EXP:
			if (pstPetExpOnline)
			{
				pstPetExpOnline->nExpMul += pstResult->ResultVal1;
				pstPetExpOnline->unExpAdd += pstResult->ResultVal2;
			}
			break;
		case  RESULT_DEC_DEF:
			if (pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stAtkChg.iPhsicDecDef += pstResult->ResultVal1;
				pstPetFightAttrOnline->stAtkChg.iIceDecDef += pstResult->ResultVal2;
			}
			break;
		case RESULT_ICE_DEF:
			if (pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stDefChg.unIceDefBaseAdd += pstResult->ResultVal1;
				pstPetFightAttrOnline->stDefChg.nIceDefChgMul += pstResult->ResultVal2;
				pstPetFightAttrOnline->stDefChg.iIceDefChgAdd += pstResult->ResultVal3;
			}
			break;
		case RESULT_PHSIC_DEF:
			if (pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stDefChg.unPhsicDefBaseAdd += pstResult->ResultVal1;
				pstPetFightAttrOnline->stDefChg.nPhsicDefChgMul += pstResult->ResultVal2;
				pstPetFightAttrOnline->stDefChg.iPhsicDefChgAdd += pstResult->ResultVal3;
			}
			break;		
		case RESULT_ICE_ATK:
			if (pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stAtkChg.unIceBaseArmAtk += pstResult->ResultVal1;
				pstPetFightAttrOnline->stAtkChg.nIceAtkChgMul += pstResult->ResultVal2;
				pstPetFightAttrOnline->stAtkChg.iIceAtkChgAdd += pstResult->ResultVal3;
			}
			break;
		case RESULT_PHSIC_ATK:
			if (pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stAtkChg.unPhsicBaseArmAtk += pstResult->ResultVal1;
				pstPetFightAttrOnline->stAtkChg.nPhsicAtkChgMul += pstResult->ResultVal2;
				pstPetFightAttrOnline->stAtkChg.iPhsicAtkChgAdd += pstResult->ResultVal3;
			}
		case RESULT_HEAVY_HIT_VAL:
			if (pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stHitRateChg.nHeavyHitVal +=  pstResult->ResultVal1*100;
			}
			break;
		case RESULT_HITRATE_RATE:
			if(pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stHitRateChg.nAtkerHitRateAdd += pstResult->ResultVal1;
			}
			break;
		//case RESULT_PET_TISHEN:
		//	if(pstPetTiShenOnline)
		//	{
		//		pstPetTiShenOnline->iYongQiAdd += pstResult->ResultVal1;
		//	}
		case RESULT_DAMAGE:
			if(pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stAtkChg.nDamageMul += pstResult->ResultVal1;
				pstPetFightAttrOnline->stAtkChg.unDamageAdd += pstResult->ResultVal2;
			}
			break;
		case RESULT_NATIVE_EXP_ADD:
			if(pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stNativeInfo.iFlag |= RESULT_NATIVE_EXP_ADD_MAP;
				pstPetFightAttrOnline->stNativeInfo.sExpAdd += pstResult->ResultVal1;				
			}
			break;
		case RESULT_NATIVE_MAX_LOVE_VAL_ADD:
			if(pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stNativeInfo.iFlag |= RESULT_NATIVE_MAX_LOVE_VAL_ADD_MAP;
				pstPetFightAttrOnline->stNativeInfo.sMaxLoveValAdd += pstResult->ResultVal1;				
			}
			break;
		case RESULT_NATIVE_ITEM_HP_ADD:
			if(pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stNativeInfo.iFlag |= RESULT_NATIVE_ITEM_HP_ADD_MAP;
				pstPetFightAttrOnline->stNativeInfo.sItemHpAdd += pstResult->ResultVal1;				
			}
			break;
		case RESULT_NATIVE_OWNER_DIE_EXP:
			if(pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stNativeInfo.iFlag |= RESULT_NATIVE_OWNER_DIE_EXP_MAP;
				pstPetFightAttrOnline->stNativeInfo.sOwnerDieExp += pstResult->ResultVal1;				
			}
			break;
		case RESULT_NATIVE_OWNER_DIE_MONEY:
			if(pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stNativeInfo.iFlag |= RESULT_NATIVE_OWNER_DIE_MONEY_MAP;
				pstPetFightAttrOnline->stNativeInfo.sOwnerDieMoney += pstResult->ResultVal1;				
			}
			break;
		case RESULT_NATIVE_XP_ADD:
			if(pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stNativeInfo.iFlag |= RESULT_NATIVE_XP_ADD_MAP;
				pstPetFightAttrOnline->stNativeInfo.sXpAdd += pstResult->ResultVal1;				
			}
			break;
		case RESULT_NATIVE_REPAIR_COST:
			if(pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stNativeInfo.iFlag |= RESULT_NATIVE_REPAIR_COST_MAP;
				pstPetFightAttrOnline->stNativeInfo.sRepairCost += pstResult->ResultVal1;				
			}
			break;
		case RESULT_NATIVE_LOVE_VAL_DIE:
			if(pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stNativeInfo.iFlag |= RESULT_NATIVE_LOVE_VAL_DIE_MAP;
				pstPetFightAttrOnline->stNativeInfo.sLoveValDie += pstResult->ResultVal1;				
			}
			break;
		case RESULT_PET_YONGQI:
			if(pstPetTiShenOnline)
			{
				pstPetTiShenOnline->iYongQiAdd += pstResult->ResultVal1;
			}
			break;
		case RESULT_PET_DEC_HURTMUL:
			if(pstPetTiShenOnline)
			{
				pstPetTiShenOnline->iDecHurtMul += pstResult->ResultVal1;
			}
			break;
		case RESULT_PET_LOVE_LEVEL_BASE_ATTR:
			if(pstPetFightAttrOnline)
			{
				pstPetFightAttrOnline->stLoveLevelInfo.sBaseAttrMul += pstResult->ResultVal1;
			}
			break;
		
		default:
			break;
	}
	
	return;
}

int pet_attr_diathesis(ZONESVRENV *pstEnv, Player *pstPlayer,PETSKILLGRID *pstDiaThesisOne, PetPowerOnline *pstPetPowerOnline, PetExpOnline *pstPetExpOnline, PetFightAttrOnline *pstPetFightAttrOnline, PetTiShenOnline *pstPetTiShenOnline)
{
	int i;
	DIATHESISDEF *pstDiathesisDef;

	pstDiathesisDef = z_find_diathesis(pstEnv, pstDiaThesisOne->SkillID, pstDiaThesisOne->SkillLevel);
	if (NULL == pstDiathesisDef)
	{
		return -1;
	}

	for (i=0; i<MAX_DIATHESIS_RESULT; i++)
	{
		if (0 == pstDiathesisDef->DiathesisResult[i].SkillResultID)
		{
			break;
		}

		pet_attr_result((RESULT *)&pstDiathesisDef->DiathesisResult[i],pstPlayer, pstPetPowerOnline, pstPetExpOnline, pstPetFightAttrOnline, pstPetTiShenOnline);
	}
	
	return 0;
}

//宠物性格影响
int pet_trait_attr(ZONESVRENV *pstEnv, Player *pstPlayer,ROLEPET *pstRolePet, PetPowerOnline *pstPetPowerOnline, PetFightAttrOnline *pstPetFightAttrOnline, PetTiShenOnline *pstPetTiShenOnline)
{
	PETTRAITDEF *pstTraitDef;
	ROLEFAIRY *pstRoleFairy;

	UNUSED(pstRolePet);
	
	pstRoleFairy = fairy_get_camp(pstEnv, pstPlayer);
	if (!pstRoleFairy)
	{
		return -1;
	}

	pstTraitDef = z_find_pet_trait_def(pstEnv, pstRoleFairy->TraitID, pstRoleFairy->StarLvl);
	if (NULL == pstTraitDef)
	{
		return -1;
	}

	if (pstPetFightAttrOnline)
	{
		//魔法攻击
		pstPetFightAttrOnline->stAtkChg.nIceAtkChgMul += pstTraitDef->IceAtkMul;
		//物理攻击
		pstPetFightAttrOnline->stAtkChg.nPhsicAtkChgMul += pstTraitDef->PhsicAtkMul;
		//魔法防御
		pstPetFightAttrOnline->stDefChg.nIceDefChgMul += pstTraitDef->IceDefMul;
		//物理防御
		pstPetFightAttrOnline->stDefChg.nPhsicDefChgMul += pstTraitDef->PhsicDefMul;
		//闪避
		pstPetFightAttrOnline->stFleeChg.nFleeChgMul +=pstTraitDef->FleeMul;
		//命中 
		pstPetFightAttrOnline->stHitChg.nHitChgMul += pstTraitDef->HitRateMul;
		//暴击
		pstPetFightAttrOnline->stHitRateChg.nHeavyHitAdd += pstTraitDef->HeavyHitRateMul;
	}

	if (pstPetPowerOnline)
	{
		pstPetPowerOnline->nPowerMaxMul += pstTraitDef->HPMul;
	}

	if(pstPetTiShenOnline)
	{
		pstPetTiShenOnline->iYongQiAdd += pstTraitDef->TiShenMul;
	}

	return 0;
}

/*
//增幅道具影响宠物属性
int pet_add_attr(ROLEPET *pstRolePet, PetPowerOnline *pstPetPowerOnline, PetFightAttrOnline *pstPetFightAttrOnline)
{	
	if (pstPetFightAttrOnline)
	{
		//魔法攻击
		pstPetFightAttrOnline->stAtkChg.nIceAtkChgMul += pstRolePet->IceAtkMul;
		//物理攻击
		pstPetFightAttrOnline->stAtkChg.nPhsicAtkChgMul += pstRolePet->PhsicAtkMul;
		//魔法防御
		pstPetFightAttrOnline->stDefChg.nIceDefChgMul += pstRolePet->IceDefMul;
		//物理防御
		pstPetFightAttrOnline->stDefChg.nPhsicDefChgMul += pstRolePet->PhsicDefMul;
		//闪避
		pstPetFightAttrOnline->stFleeChg.nFleeChgMul +=pstRolePet->FleeMul;
		//命中 
		pstPetFightAttrOnline->stHitChg.nHitChgMul += pstRolePet->HitRateMul;
		//暴击
		pstPetFightAttrOnline->stHitRateChg.nHeavyHitAdd += pstRolePet->HeavyHitRateMul;
	}

	if (pstPetPowerOnline)
	{
		pstPetPowerOnline->nPowerMaxMul += pstRolePet->HPMul;
	}

	return 0;
}
*/

//宠物战斗相关属性
int z_fill_fight_attr(ZONESVRENV *pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, RolePetOnline *pstRolePetOnline)
{		
	PetFightAttrOnline *pstPetFightAttrOnline;
	PetTiShenOnline *pstPetTiShenOnline;
	UNUSED(pstEnv);
	
	pstPetFightAttrOnline = pet_find_fightattronline_byonline(pstPlayer, pstRolePet, pstRolePetOnline);
	if (NULL == pstPetFightAttrOnline)
	{
		return -1;
	}

	pstRolePet->FightAttr.PhsicAtk = pstPetFightAttrOnline->stAtkChg.unPhsicBaseArmAtk*(1+pstPetFightAttrOnline->stAtkChg.nPhsicAtkChgMul/100.0)+pstPetFightAttrOnline->stAtkChg.iPhsicAtkChgAdd;
	pstRolePet->FightAttr.IceAtk = pstPetFightAttrOnline->stAtkChg.unIceBaseArmAtk*(1+pstPetFightAttrOnline->stAtkChg.nIceAtkChgMul/100.0)+pstPetFightAttrOnline->stAtkChg.iIceAtkChgAdd;
	pstRolePet->FightAttr.PhsicDef = pstPetFightAttrOnline->stDefChg.unPhsicDefBaseAdd*(1+ pstPetFightAttrOnline->stDefChg.nPhsicDefChgMul/100.0)+pstPetFightAttrOnline->stDefChg.iPhsicDefChgAdd; 
	pstRolePet->FightAttr.IceDef = pstPetFightAttrOnline->stDefChg.unIceDefBaseAdd*(1+pstPetFightAttrOnline->stDefChg.nIceDefChgMul/100.0)+pstPetFightAttrOnline->stDefChg.iIceDefChgAdd;
	pstRolePet->FightAttr.HitRate = pstPetFightAttrOnline->stHitChg.unHitBaseAdd*(1+pstPetFightAttrOnline->stHitChg.nHitChgMul/100.0) + pstPetFightAttrOnline->stHitChg.iHitChgAdd;;
	pstRolePet->FightAttr.Flee = pstPetFightAttrOnline->stFleeChg.unFleeBaseAdd*(1+pstPetFightAttrOnline->stFleeChg.nFleeChgMul/100.0) + pstPetFightAttrOnline->stFleeChg.iFleeChgAdd;
	pstRolePet->FightAttr.HeavyHitRate = pstPetFightAttrOnline->stHitRateChg.nHeavyHitAdd;

	pstRolePet->FightAttr.YongQi = 0;
	pstRolePet->FightAttr.DecHurtMul = 0;
	pstPetTiShenOnline = pet_find_tishenonline_byonline(pstPlayer, pstRolePet, pstRolePetOnline);
	if(pstPetTiShenOnline)
	{
		pstRolePet->FightAttr.YongQi = pstPetTiShenOnline->iYongQiAdd;
		if(pstRolePet->FightAttr.YongQi < 0)
		{
			pstRolePet->FightAttr.YongQi = 0;
		}

		pstRolePet->FightAttr.DecHurtMul = pstPetTiShenOnline->iDecHurtMul;
	}
		
	return 0;
}
/*
void z_pet_dyn_tmp(PetFightAttrOnline *pstPetFightAttrOnline)
{
	DEFCHG *pstDefChg;
	
	//命中
	pstPetFightAttrOnline->stHitChg.iHitTmp = (pstPetFightAttrOnline->stHitChg.unHitBaseAdd)*
									(1+pstPetFightAttrOnline->stHitChg.nHitChgMul/100.0) + pstPetFightAttrOnline->stHitChg.iHitChgAdd;

	//闪避
	pstPetFightAttrOnline->stFleeChg.iFleeTmp = pstPetFightAttrOnline->stFleeChg.unFleeBaseAdd*(1+pstPetFightAttrOnline->stFleeChg.nFleeChgMul/100.0) + pstPetFightAttrOnline->stHitChg.iHitChgAdd;

	//防御
	pstDefChg = &pstPetFightAttrOnline->stDefChg;
	pstDefChg->iDefIceTmp = pstDefChg->unIceDefBaseAdd;
	pstDefChg->iDefFireTmp = pstDefChg->unFireDefBaseAdd;
	pstDefChg->iDefThunderTmp = pstDefChg->unThunderDefBaseAdd;
	pstDefChg->iDefLightTmp = pstDefChg->unLightDefBaseAdd;
	pstDefChg->iDefNightTmp = pstDefChg->unNightDefBaseAdd;
	pstDefChg->iDefPhsicTmp = pstDefChg->unPhsicDefBaseAdd;
		
	return;
}
*/
int pet_attr_update(ZONESVRENV *pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, RolePetOnline *pstRolePetOnline)
{
	int i;
	char cPetXieDaiPos = -1;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr  = &stPkg.Body.PetSvr;
	CSPETATTRCHG *pstPetChgData= &pstPetSvr->PetSvrData.AttrChg;
	PETSKILL *pstPetSkill = &pstRolePet->PetSkills;
	PETDEF *pstPetDef;
	PETLEVELDEF *pstPetLevelDef;
	int iMaxPowerMul = 0;
	int iMaxPowerAdd = 0;
	int iMaxPowerBase = 0;
	int iOldExpNext = 0;
	unsigned int uiOldPower;
	unsigned int uiOldPowerMax;
	PetExpOnline *pstPetExpOnline=NULL;
	PetFightAttrOnline *pstPetFightAttrOnline=NULL;
	PetPowerOnline *pstPetPowerOnline=NULL;
	PetTiShenOnline *pstPetTiShenOnline=NULL;
	STATUSRESULT stResult;
	int iStrengthenVal = pet_strengthen_val_get(pstRolePet);
	int iBaseAttrMul  = iStrengthenVal;

	memset(&stResult,0,sizeof(stResult));
	
	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (NULL == pstPetDef)
	{
		return -1;
	}

	pstPetLevelDef = z_find_pet_level_def(pstEnv, pstRolePet->Level, NULL);
	if (!pstPetLevelDef)
	{
		return -1;
	}
	
	pstPetPowerOnline  = pet_find_poweronline_byonline(pstPlayer, pstRolePet, pstRolePetOnline);
	if (pstPetPowerOnline)
	{
		memset(pstPetPowerOnline, 0, sizeof(*pstPetPowerOnline));
	}
	
	pstPetExpOnline = pet_find_exponline_byonline(pstPlayer, pstRolePet, pstRolePetOnline);
	if (pstPetExpOnline)
	{
		iOldExpNext = pstPetExpOnline->iNextLevelExp;
		memset(pstPetExpOnline, 0, sizeof(*pstPetExpOnline));
	}

	pstPetTiShenOnline = pet_find_tishenonline_byonline(pstPlayer, pstRolePet, pstRolePetOnline);
	if(pstPetTiShenOnline)
	{
		memset(pstPetTiShenOnline, 0, sizeof(*pstPetTiShenOnline));
		pstPetTiShenOnline->iYongQiAdd = pstPetDef->TiShen;
		pstPetTiShenOnline->iDecHurtMul = PET_DECHURT_MUL;
	}
	 
	pstPetFightAttrOnline = pet_find_fightattronline_byonline(pstPlayer, pstRolePet, pstRolePetOnline);
	if (pstPetFightAttrOnline)
	{		
		memset(pstPetFightAttrOnline, 0, sizeof(*pstPetFightAttrOnline));
	}

		//宠物爱心等级
	{
		PETLOVELEVELDEF* pstLoveLevelDef = z_find_pet_love_level_def(pstEnv,pstRolePet->LoveLevel);
		if (pstLoveLevelDef && pstLoveLevelDef->DiathesisInfo.SkillID &&
			pstLoveLevelDef->DiathesisInfo.SkillLevel)
		{
			pet_attr_diathesis(pstEnv, pstPlayer,&pstLoveLevelDef->DiathesisInfo, pstPetPowerOnline, pstPetExpOnline, pstPetFightAttrOnline, pstPetTiShenOnline);
		}
	}
		
	if (pstPetFightAttrOnline)
	{
		DYNATTRLIST stDyn;
		iBaseAttrMul += pstPetFightAttrOnline->stLoveLevelInfo.sBaseAttrMul;
		
		//宠物状态效果
		memset(&stDyn, 0, sizeof(stDyn));
		z_cacl_pet_status_result(pstEnv, pstPlayer, &stDyn, pstRolePet);

		pstPetFightAttrOnline->stAtkChg = stDyn.stAtkChg;
		pstPetFightAttrOnline->stAtkeeChg = stDyn.stAtkeeHurtChg; 
		pstPetFightAttrOnline->stHitChg = stDyn.stHitChg;
		pstPetFightAttrOnline->stFleeChg = stDyn.stFleeChg;
		pstPetFightAttrOnline->stHitRateChg = stDyn.stHitRateChg;
		pstPetFightAttrOnline->stDefChg = stDyn.stDefChg;
	
		//基础成长影响


		//魔法攻击
		pstPetFightAttrOnline->stAtkChg.unIceBaseArmAtk += pstPetLevelDef->IceAtk*(pstRolePet->IceAtkGenius*(100+iBaseAttrMul)/100.0)/1000.0;
		//物理攻击
		pstPetFightAttrOnline->stAtkChg.unPhsicBaseArmAtk += pstPetLevelDef->PhsicAtk*(pstRolePet->PhsicAtkGenius*(100+iBaseAttrMul)/100.0)/1000.0;
		//魔法防御
		pstPetFightAttrOnline->stDefChg.unIceDefBaseAdd += pstPetLevelDef->IceDef*(pstRolePet->IceDefGenius*(100+iBaseAttrMul)/100.0)/1000.0;
		//物理防御
		pstPetFightAttrOnline->stDefChg.unPhsicDefBaseAdd += pstPetLevelDef->PhsicDef*(pstRolePet->DefGenius*(100+iBaseAttrMul)/100.0)/1000.0;
		//闪避
		pstPetFightAttrOnline->stFleeChg.unFleeBaseAdd +=pstPetLevelDef->Flee*(0.8+pstRolePet->FleeGenius*(100+iBaseAttrMul)/100.0/5000.0);
		//命中 
		pstPetFightAttrOnline->stHitChg.unHitBaseAdd += pstPetLevelDef->HitRate*(0.8+pstRolePet->HitRateGenius*(100+iBaseAttrMul)/100.0/5000.0);	

		pstPetFightAttrOnline->stAtkChg.nArmLevChgMul += pstPetLevelDef->HurtMul;

		pstPetFightAttrOnline->stAtkChg.iAtkLevelMul += get_pet_level_mul(pstEnv, pstRolePet->Level, LEVEL_MUL_ATK);
		pstPetFightAttrOnline->stDefChg.iDefLevelMul += get_pet_level_mul(pstEnv, pstRolePet->Level, LEVEL_MUL_DEF);
		pstPetFightAttrOnline->stHitRateChg.iDefHeavyHit += get_pet_level_mul(pstEnv, pstRolePet->Level, LEVEL_MUL_DEF_HEAVYHIT);

		pstPetFightAttrOnline->stHitRateChg.nHeavyHitAdd += pstPetDef->HeavyHitRate;

		if(0==z_get_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_PET_ADD_ATK, &stResult, 0))
		{
			pstPetFightAttrOnline->stAtkChg.unPhsicBaseArmAtk += stResult.ResultVal1;
		}
	}

	//计算通灵
	if (fairy_is_tongling( pstEnv, pstPlayer))
	{
		// 宠物元素攻击
		if (pstPetFightAttrOnline)
		{
			if(pstPetExpOnline)
			{
				fairy_tongling_up_attr(pstEnv, pstPlayer, pstPetFightAttrOnline, pstPetExpOnline);
			}
			else
			{	
				fairy_tongling_up_attr(pstEnv, pstPlayer, pstPetFightAttrOnline, NULL);
			}
		}

		// 性格影响
		if(!pstPetPowerOnline && pstPetTiShenOnline)
        	{
                     pet_trait_attr(pstEnv, pstPlayer,pstRolePet, NULL, pstPetFightAttrOnline, pstPetTiShenOnline);         
		}
		else if(pstPetPowerOnline && !pstPetTiShenOnline)
		{
 			pet_trait_attr(pstEnv, pstPlayer,pstRolePet, pstPetPowerOnline, pstPetFightAttrOnline, NULL);
		}
		else if(pstPetPowerOnline && pstPetTiShenOnline)
		{
			pet_trait_attr(pstEnv, pstPlayer,pstRolePet, pstPetPowerOnline, pstPetFightAttrOnline, pstPetTiShenOnline);
		}
		else
		{
			pet_trait_attr(pstEnv, pstPlayer,pstRolePet, NULL, pstPetFightAttrOnline, NULL);
		}
	}
	
	//pet_add_attr(pstRolePet, pstPetPowerOnline, pstPetFightAttrOnline);
	
	//宠物技能影响(实际是素质)
	for (i=0; i<pstPetSkill->SkillGridNum; i++)
	{	
		if (pstPetSkill->SkillGrid[i].SkillID == 0 || 
			pstPetSkill->SkillGrid[i].GridType != PET_SKILL_SELF_BUFF)
		{
			continue;
		}
		
		 if(pstPetExpOnline)
		{
			pet_attr_diathesis(pstEnv, pstPlayer,&pstPetSkill->SkillGrid[i], pstPetPowerOnline, pstPetExpOnline, pstPetFightAttrOnline, pstPetTiShenOnline);
		}
		else
		{
			pet_attr_diathesis(pstEnv, pstPlayer,&pstPetSkill->SkillGrid[i], pstPetPowerOnline, NULL, pstPetFightAttrOnline, pstPetTiShenOnline);
		}
	}

	if(pstRolePet->NativeSkill.SkillID > 0 && pstRolePet->NativeSkill.GridType == PET_SKILL_SELF_BUFF)
	{
		pet_attr_diathesis(pstEnv, pstPlayer,&pstRolePet->NativeSkill, pstPetPowerOnline, pstPetExpOnline, pstPetFightAttrOnline, pstPetTiShenOnline);
	}



	if(pstPetTiShenOnline && pstPetTiShenOnline->iYongQiAdd < 0)
	{
		pstPetTiShenOnline->iYongQiAdd = 0;
	}

	//更新宠物战斗相关属性
	z_fill_fight_attr(pstEnv, pstPlayer, pstRolePet, pstRolePetOnline);
	pet_fight_attr_to_clt(pstEnv, pstPlayer, pstRolePet);

	if (pstPetExpOnline)
	{
		pstPetExpOnline->iNextLevelExp = iOldExpNext;
	}

	//计算Power
	if (pstPetPowerOnline)
	{
		iMaxPowerBase = pstPetPowerOnline->iPowerMaxBase;
		iMaxPowerMul = pstPetPowerOnline->nPowerMaxMul;
		iMaxPowerAdd = pstPetPowerOnline->nPowerMaxAdd;
	}

	uiOldPower = 	pstRolePet->HP;
	uiOldPowerMax = pstRolePet->MaxHP;
	
	//资质影响HP
	pstRolePet->MaxHP  = pstPetLevelDef->HP*(pstRolePet->HPGenius*(100+iBaseAttrMul)/100.0)/1000.0;
	//其他影响HP
	pstRolePet->MaxHP = (pstRolePet->MaxHP + iMaxPowerBase) * (1+iMaxPowerMul/100.0) + iMaxPowerAdd;

	if(0==z_get_pet_status_result(pstEnv, pstPlayer, pstRolePet, STATUS_RESULT_MAXHP, &stResult))
	{
		pstRolePet->MaxHP = (pstRolePet->MaxHP+stResult.ResultVal1)*(1+stResult.ResultVal2/100.0)+ stResult.ResultVal3;
	}
	
	
	if (pstRolePet->HP > pstRolePet->MaxHP)
	{
		pstRolePet->HP = pstRolePet->MaxHP;
	}
	
	cPetXieDaiPos = get_pet_shi_xiedaipos(pstEnv, pstPlayer, pstRolePet);
	
	if (uiOldPower != pstRolePet->HP)
	{
		pstPetSvr->PetSvrType = PET_ATTR_CHG;
		
		pstPetChgData->Type = PET_CHG_HP;
		pstPetChgData->Wid = pstRolePet->WID;
		pstPetChgData->AttrValue.ChgHP.HP= pstRolePet->HP;
		pstPetChgData->AttrValue.ChgHP.XieDaiPos = cPetXieDaiPos;

		Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	if (uiOldPowerMax != pstRolePet->MaxHP)
	{
		pstPetSvr->PetSvrType = PET_ATTR_CHG;
		
		pstPetChgData->Type = PET_CHG_MAXHP;
		pstPetChgData->Wid = pstRolePet->WID;
		pstPetChgData->AttrValue.ChgMaxHP.HP = pstRolePet->MaxHP;
		pstPetChgData->AttrValue.ChgMaxHP.XieDaiPos = cPetXieDaiPos;

		Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	if (pstRolePet->StatFlag & PET_STAT_FIGHT && uiOldPowerMax != pstRolePet->MaxHP )
	{
		pet_hp_action(pstEnv, pstPlayer, pstRolePet, pstRolePet->HP);
	}
	
	return 0;
}


/*

int pet_attr_arm(ZONESVRENV *pstEnv, ROLEARM *pstArm, PetPowerOnline *pstPetPowerOnline, PetExpOnline *pstPetExpOnline, PetFightAttrOnline *pstPetFightAttrOnline)
{
	int i;
	ITEMDEF *pstItemDef;

	pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstArm);
	if (NULL == pstItemDef)
	{
		return -1;
	}

	for (i=0; i<MAX_ITEM_RESULT; i++)
	{
		if (0 == pstItemDef->Result[i].ResultID)
		{
			break;
		}

		pet_attr_result(&pstItemDef->Result[i], pstPetPowerOnline, pstPetExpOnline, pstPetFightAttrOnline);
	}
	
	return 0;
}

int pet_attr_addattr(ZONESVRENV *pstEnv, unsigned short unAddAttrID, PetPowerOnline *pstPetPowerOnline, PetExpOnline *pstPetExpOnline, PetFightAttrOnline *pstPetFightAttrOnline)
{
	int i;
	ADDATTRDEF *pstAddAttrDef;

	pstAddAttrDef = z_find_addattr(pstEnv, unAddAttrID);
	if (NULL == pstAddAttrDef)
	{
		return -1;
	}

	for (i=0; i<MAX_ADDATTR_RESULT; i++)
	{
		if (0 == pstAddAttrDef->Result[i].ResultID)
		{
			break;
		}

		pet_attr_result(&pstAddAttrDef->Result[i], pstPetPowerOnline, pstPetExpOnline, pstPetFightAttrOnline);
	}
	
	return 0;
}

*/

int z_get_watched_attr(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,
	                     CSPLAYERATTRVAL *pstAttrVal)
{
	pstAttrVal->AttrNum = 0;

	z_get_attr_list(pstAppCtx, pstEnv, pstPlayer, pstAttrVal, 0);

	return 0;
}

int player_career_chg_action(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	DYNATTRLIST *pstDyn =  &pstPlayer->stOnline.stDyn;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	COLLECTSKILLINFO *pCollectSkillInfo = &pstActionRes->Data.CareerChg.CollectSkillInfo;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_CAREER_CHG;

	pstActionRes->Data.CareerChg.Career = pstPlayer->stRoleData.Career;
	pCollectSkillInfo->CollectSkill_SlotMax = pstPlayer->stRoleData.MiscInfo.CollectSkillInfo.CollectSkill_SlotMax+pstDyn->stHealChg.nGeniusCollectSkillMax;
	pCollectSkillInfo->CollectSkill_SlotCurr = pstPlayer->stRoleData.MiscInfo.CollectSkillInfo.CollectSkill_SlotCurr;
	pCollectSkillInfo->CollectSkill_AddType = pstPlayer->stRoleData.MiscInfo.CollectSkillInfo.CollectSkill_AddType;
		
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	
	return 0;
}

int player_collectskill_add_slot( ZONESVRENV* pstEnv, Player *pstPlayer,int iVal,int iAddType)
{
	COLLECTSKILLINFO *pstCollectSkillInfo = NULL;
	DYNATTRLIST *pstDyn =  &pstPlayer->stOnline.stDyn;

	pstCollectSkillInfo = &pstPlayer->stRoleData.MiscInfo.CollectSkillInfo;

	//先不用了
	return -1;
	
	if( pstPlayer->stRoleData.Career <= CAREER_CHURCH )
	{
		return -1;
	}

	//到了最大就不要在加了
	if( pstCollectSkillInfo->CollectSkill_SlotCurr >= (pstCollectSkillInfo->CollectSkill_SlotMax+pstDyn->stHealChg.nGeniusCollectSkillMax) )
	{
		return -1;
	}
	
	if( COLLECTSKILL_ADDTYPE_VAL == iAddType &&
		iVal > 0)
	{
		pstCollectSkillInfo->CollectSkill_SlotCurr += iVal;
		pstCollectSkillInfo->CollectSkill_SlotCurr += pstDyn->stHealChg.nGeniusCollectSkillAdd;
		if( pstCollectSkillInfo->CollectSkill_SlotCurr > (pstCollectSkillInfo->CollectSkill_SlotMax+pstDyn->stHealChg.nGeniusCollectSkillMax)  )
		{
			pstCollectSkillInfo->CollectSkill_SlotCurr = pstCollectSkillInfo->CollectSkill_SlotMax+pstDyn->stHealChg.nGeniusCollectSkillMax;
		}
	}	
	else if( COLLECTSKILL_ADDTYPE_AKER == pstCollectSkillInfo->CollectSkill_AddType &&
		COLLECTSKILL_ADDTYPE_AKER == iAddType )
	{
		pstCollectSkillInfo->CollectSkill_SlotCurr += 10;
		pstCollectSkillInfo->CollectSkill_SlotCurr += pstDyn->stHealChg.nGeniusCollectSkillAdd;
		if( pstCollectSkillInfo->CollectSkill_SlotCurr > (pstCollectSkillInfo->CollectSkill_SlotMax+pstDyn->stHealChg.nGeniusCollectSkillMax)  )
		{
			pstCollectSkillInfo->CollectSkill_SlotCurr = pstCollectSkillInfo->CollectSkill_SlotMax+pstDyn->stHealChg.nGeniusCollectSkillMax;
		}
	}
	else if( COLLECTSKILL_ADDTYPE_AKEE == pstCollectSkillInfo->CollectSkill_AddType &&
			  COLLECTSKILL_ADDTYPE_AKEE == iAddType )
	{
		pstCollectSkillInfo->CollectSkill_SlotCurr += 10;
		pstCollectSkillInfo->CollectSkill_SlotCurr += pstDyn->stHealChg.nGeniusCollectSkillAdd;
		if( pstCollectSkillInfo->CollectSkill_SlotCurr > (pstCollectSkillInfo->CollectSkill_SlotMax+pstDyn->stHealChg.nGeniusCollectSkillMax) )
		{
			pstCollectSkillInfo->CollectSkill_SlotCurr = pstCollectSkillInfo->CollectSkill_SlotMax+pstDyn->stHealChg.nGeniusCollectSkillMax;
		}
	}
	else if( COLLECTSKILL_ADDTYPE_TIME == pstCollectSkillInfo->CollectSkill_AddType &&
			  COLLECTSKILL_ADDTYPE_TIME == iAddType )
	{
		if ( !( (pstPlayer->stOnline.State & CS_STAT_FIGHT) || (pstPlayer->stOnline.State & CS_STAT_ATK) || (pstPlayer->stOnline.State & CS_STAT_MOVE_ATK)) )
		{
			return 0;
		}
		pstCollectSkillInfo->CollectSkill_SlotCurr += 10;
		pstCollectSkillInfo->CollectSkill_SlotCurr += pstDyn->stHealChg.nGeniusCollectSkillAdd;
		if( pstCollectSkillInfo->CollectSkill_SlotCurr > (pstCollectSkillInfo->CollectSkill_SlotMax+pstDyn->stHealChg.nGeniusCollectSkillMax) )
		{
			pstCollectSkillInfo->CollectSkill_SlotCurr = pstCollectSkillInfo->CollectSkill_SlotMax+pstDyn->stHealChg.nGeniusCollectSkillMax;
		}
	}
	else
	{
		return -1;
	}

	player_career_chg_action(pstEnv,pstPlayer);
	
	return 0;
}

int player_collectskill_dec_slot( ZONESVRENV* pstEnv, Player *pstPlayer)
{
	COLLECTSKILLINFO *pstCollectSkillInfo = &pstPlayer->stRoleData.MiscInfo.CollectSkillInfo;

	return -1;

	if( pstPlayer->stRoleData.Career <= CAREER_CHURCH )
	{
		return -1;
	}
	
	if ( (pstPlayer->stOnline.State & CS_STAT_FIGHT)  ||
		  (pstPlayer->stOnline.State & CS_STAT_ATK) || 
		  (pstPlayer->stOnline.State & CS_STAT_MOVE_ATK)  ||
		  pstCollectSkillInfo->CollectSkill_SlotCurr == 0 )
	{
		return 0;
	}
	
	if( COLLECTSKILL_ADDTYPE_AKER == pstCollectSkillInfo->CollectSkill_AddType )
	{
		pstCollectSkillInfo->CollectSkill_SlotCurr -= 5;
		if( pstCollectSkillInfo->CollectSkill_SlotCurr < 0)
		{
			pstCollectSkillInfo->CollectSkill_SlotCurr = 0;
		}
	}
	else if( COLLECTSKILL_ADDTYPE_AKEE == pstCollectSkillInfo->CollectSkill_AddType )
	{
		pstCollectSkillInfo->CollectSkill_SlotCurr -= 5;
		if( pstCollectSkillInfo->CollectSkill_SlotCurr < 0 )
		{
			pstCollectSkillInfo->CollectSkill_SlotCurr = 0;
		}
	}
	else if( COLLECTSKILL_ADDTYPE_TIME == pstCollectSkillInfo->CollectSkill_AddType )
	{
		pstCollectSkillInfo->CollectSkill_SlotCurr -= 5;
		if( pstCollectSkillInfo->CollectSkill_SlotCurr < 0 )
		{
			pstCollectSkillInfo->CollectSkill_SlotCurr = 0;
		}
	}
	else if( COLLECTSKILL_ADDTYPE_VAL == pstCollectSkillInfo->CollectSkill_AddType )
	{
		pstCollectSkillInfo->CollectSkill_SlotCurr -= 5;
		if( pstCollectSkillInfo->CollectSkill_SlotCurr < 0 )
		{
			pstCollectSkillInfo->CollectSkill_SlotCurr = 0;
		}
	}
	

	player_career_chg_action(pstEnv,pstPlayer);
	return 0;
}

//天赋 被暴击加hp
int genius_add_hp_by_heavy_hit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	SKILLRESULT stResult;
	UNUSED(pstAppCtx);
	
	memset(&stResult,0,sizeof(stResult));
	z_get_diathesis_result(pstEnv, pstPlayer, RESULT_ADD_HP_BY_HEAVY_HIT, &stResult);
	
	return pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax * (stResult.SkillResultVal1/100.0) + stResult.SkillResultVal2;	
}

//天赋 被闪避加速
int genius_add_status_by_flee(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	SKILLRESULT stResult;
	UNUSED(pstAppCtx);
	
	memset(&stResult,0,sizeof(stResult));
	z_get_diathesis_result(pstEnv, pstPlayer, RESULT_ADD_SPEED_STATUS_FLEE, &stResult);

	if( stResult.SkillResultVal1 > 0)
	{
		z_player_apply_status(pstAppCtx, pstEnv, pstPlayer, stResult.SkillResultVal1,
			stResult.SkillResultVal2, stResult.SkillResultVal3, 1, 0);
	}
	return 0;
}

//天赋 普通攻击触发对自身的buff
int genius_atk_random_add_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,char SkillType)
{
	SKILLRESULT stResult;
	UNUSED(pstAppCtx);

	if( SkillType != 1)
	{
		return -1;
	}
	memset(&stResult,0,sizeof(stResult));
	z_get_diathesis_result(pstEnv, pstPlayer, RESULT_ATK_RANDOM_ADD_STATUS, &stResult);

	if( stResult.SkillResultVal1 > 0)
	{
		if( (int)(RAND1(100)) <  stResult.SkillResultVal4 )
		{
			z_player_apply_status(pstAppCtx, pstEnv, pstPlayer, stResult.SkillResultVal1,
				stResult.SkillResultVal2, stResult.SkillResultVal3, 1, 0);
		}
	}
	return 0;
}


//天赋 吟唱打退时间比率
int genius_reduce_pre_time_rate_by_atk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	SKILLRESULT stResult;
	UNUSED(pstAppCtx);
		
	memset(&stResult,0,sizeof(stResult));
	z_get_diathesis_result(pstEnv, pstPlayer, RESULT_DEC_PRE_TIME_RATE, &stResult);

	return  stResult.SkillResultVal1;
}

//天赋  被暴击减少技能CD
int genius_reduce_skill_cd_time_by_heavyhit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstAtkee, struct timeval *pstCltTime,int iHeavyHit)
{	
	int i = 0;
	long long  llMs=0;
	int iTmp;
	struct timeval stDiff;
	DYNATTRLIST *pstDyn =  &pstAtkee->stOnline.stDyn;
	SKILLRESULT *pstResult = NULL;
	ZoneSkillList *pstZoneSkillList = &pstAtkee->stOnline.stSkillList;
	ZoneSkill *pstOtherZoneSkill;
	const SKILLDEF *pstSkillDef = NULL;

	CSPKG stPkg;
	CSDIATHESISINFO *pstDiaThesisInfo = &stPkg.Body.DiathesisInfo;
	CSSKILLREDUCECDTIMEARR *pstSkillReduceTime = &pstDiaThesisInfo->DiathesisInfoData.SkillReduceCdTime;

	UNUSED(pstAppCtx);
	UNUSED(pstEnv);

	
	if( iHeavyHit != 1 )
	{
		return 0;
	}
	memset(pstSkillReduceTime,0,sizeof(CSSKILLREDUCECDTIMEARR));
	pstSkillReduceTime->MaxSkillNum = MAX_CHG_SKILL_CD_NUM;
	
	//天赋 以下是被暴击对技能的cd影响
	for( i=0; i<pstDyn->stHealChg.ushCDResultNum &&
		           i< MAX_CHG_SKILL_CD_NUM; i++ )
	{
		pstResult = &pstDyn->stHealChg.astCDResult[i];
		if( RESULT_REDUCE_SKILL_CD_TIME_BY_HEAVYHIT == pstResult->SkillResultID )
		{
			//借用下val4来存储下上次找到的zone_skill的索引
			if( pstResult->SkillResultVal4 >= 0 && 
				pstResult->SkillResultVal4 < pstZoneSkillList->nSkillNum )
			{
				pstOtherZoneSkill = &pstZoneSkillList->astZoneSkills[pstResult->SkillResultVal4];
				if( pstResult->SkillResultVal1 == pstOtherZoneSkill->stSkillData.SkillID )
				{
					pstSkillDef = z_find_skill_zoneskill(pstEnv,
					                                     pstOtherZoneSkill);
					if (!pstSkillDef)
					{
						continue;
					}

					//不管coolend是啥时候,都做个时间差的缩小比例处理
					TV_DIFF(stDiff, pstOtherZoneSkill->stCoolEnd, *pstCltTime);
					TV_TO_MS(llMs, stDiff);
					if( llMs <= 0 )
					{
						// 已经超时的不管
						continue;
					}
					//减少对自身技能的时间
					//iTmp = pstOtherZoneSkill->stSkillData.CSFittingsRowList[0].SelfCoolTime;
					iTmp = pstSkillDef->SelfCool;
					iTmp = iTmp * ((pstResult->SkillResultVal2 % 100) / 100.0);
					iTmp = iTmp * 100;//弄成ms来算
					if(llMs > iTmp )
					{
						llMs = llMs - iTmp;
					}
					else 
					{
						llMs = 0;
					}
					TV_ADD_MS(pstOtherZoneSkill->stCoolEnd, *pstCltTime, llMs);
					pstSkillReduceTime->CSSkillReduceCdTime[pstSkillReduceTime->ChgNum].SkillID = pstOtherZoneSkill->stSkillData.SkillID;
					pstSkillReduceTime->CSSkillReduceCdTime[pstSkillReduceTime->ChgNum].ReduceCDTime = iTmp;
					pstSkillReduceTime->ChgNum++;
					
					continue;
				}
			}
			
			for (i=0; i<pstZoneSkillList->nSkillNum; i++)
			{
				pstOtherZoneSkill = &pstZoneSkillList->astZoneSkills[i];
				if( pstResult->SkillResultVal1 != pstOtherZoneSkill->stSkillData.SkillID )
				{
					continue;
				}

				pstSkillDef = z_find_skill_zoneskill(pstEnv, pstOtherZoneSkill);
				if (!pstSkillDef)
				{
					continue;
				}
				
				pstResult->SkillResultVal4 = i;
				
				//不管coolend是啥时候,都做个时间差的缩小比例处理
				TV_DIFF(stDiff, pstOtherZoneSkill->stCoolEnd, *pstCltTime);
				TV_TO_MS(llMs, stDiff);
				if( llMs <= 0 )
				{
					// 已经超时的不管
					break;
				}
				// 减少的时间
				//iTmp = pstOtherZoneSkill->stSkillData.CSFittingsRowList[0].SelfCoolTime;
				iTmp = pstSkillDef->SelfCool;
				iTmp = iTmp * ((pstResult->SkillResultVal2 % 100) / 100.0);
				iTmp = iTmp * 100;//弄成ms来算
				if(llMs > iTmp )
				{
					llMs = llMs - iTmp;
				}
				else
				{
					llMs = 0;
				}
				TV_ADD_MS(pstOtherZoneSkill->stCoolEnd, *pstCltTime, llMs);
				pstSkillReduceTime->CSSkillReduceCdTime[pstSkillReduceTime->ChgNum].SkillID = pstOtherZoneSkill->stSkillData.SkillID;
				pstSkillReduceTime->CSSkillReduceCdTime[pstSkillReduceTime->ChgNum].ReduceCDTime = iTmp;
				pstSkillReduceTime->ChgNum++;
			
				break;
			}
		}
		else if( 0 == pstResult->SkillResultID)
		{
			break;
		}
	}

	if( pstSkillReduceTime->ChgNum > 0)
	{
		pstDiaThesisInfo->Type = SKILL_REDUCE_CD_TIME;		
		Z_CSHEAD_INIT(&stPkg.Head, DIATHESIS_INFO);
		z_cltmsg_send(pstEnv, pstAtkee, &stPkg);
	}

	
	return 0;
}
// 天赋 对指定技能cd减少
int genius_atk_reduce_skill_cd_time( Player *pstAtker,CSSKILLDATA *pstSkillData, SKILLRESULT *pstResult )
{
	UNUSED(pstAtker);
	UNUSED(pstSkillData);
	UNUSED(pstResult);
	/*
	CSFITTINGSROW *pstFittings;
	UNUSED(pstAtker);
	
	if( RESULT_REDUCE_SKILL_CD_TIME == pstResult->SkillResultID &&
		pstResult->SkillResultVal1 == pstSkillData->SkillID )
	{
			pstFittings = &pstSkillData->CSFittingsRowList[0];
			//不管coolend是啥时候,都做个时间差的缩小比例处理
			pstFittings->SelfCoolTime = pstFittings->SelfCoolTime*( 1 - (pstResult->SkillResultVal2%100)/100.0);
	}
	*/
	return 0;
}
// 被击触发虚弱 --玩家
int genius_add_status_by_player_atk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstAtker, Player *pstAtkee)
{
	SKILLRESULT stResult;
	UNUSED(pstAppCtx);
	
	memset(&stResult,0,sizeof(stResult));
	z_get_diathesis_result(pstEnv, pstAtkee, RESULT_RANDOM_ADD_STATUS_BY_ATK, &stResult);

	if( stResult.SkillResultVal1 > 0)
	{
		if( (int)(RAND1(100)) <  stResult.SkillResultVal4 )
		{
			z_player_apply_status(pstAppCtx, pstEnv, pstAtker, stResult.SkillResultVal1,
			stResult.SkillResultVal2, stResult.SkillResultVal3, 1, 0);
		}
	}
	return 0;
}

// 击杀触发状态
int genius_add_status_by_player_kill(ZONESVRENV* pstEnv, Player *pstAtker)
{
	SKILLRESULT stResult;

	memset(&stResult,0,sizeof(stResult));
	z_get_diathesis_result(pstEnv, pstAtker, RESULT_KILL_TOUCH_STATUS, &stResult);

	if( stResult.SkillResultVal1 > 0)
	{
		if( (int)(RAND1(100)) <  stResult.SkillResultVal4 )
		{
			z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstAtker, stResult.SkillResultVal1,
			stResult.SkillResultVal2, stResult.SkillResultVal3, 1, 0);
		}
	}
	return 0;
}

// 被击触发虚弱 --宠物
int genius_add_status_by_pet_atk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstAtker, ROLEPET *pstRolePet, Player *pstAtkee)
{
	SKILLRESULT stResult;
	UNUSED(pstAppCtx);
	
	memset(&stResult,0,sizeof(stResult));
	z_get_diathesis_result(pstEnv, pstAtkee, RESULT_RANDOM_ADD_STATUS_BY_ATK, &stResult);

	if( stResult.SkillResultVal1 > 0)
	{
		if( (int)(RAND1(100)) <  stResult.SkillResultVal4 )
		{
			z_pet_apply_status(pstAppCtx, pstEnv, pstAtker,pstRolePet, stResult.SkillResultVal1,
			stResult.SkillResultVal2, stResult.SkillResultVal3, 0);
		}
	}
	return 0;
}
 
// 被击触发虚弱 --怪物
int genius_add_status_by_monster_atk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstAtkee, Monster *pstMon)
{
	SKILLRESULT stResult;
	UNUSED(pstAppCtx);
	
	memset(&stResult,0,sizeof(stResult));
	z_get_diathesis_result(pstEnv, pstAtkee, RESULT_RANDOM_ADD_STATUS_BY_ATK, &stResult);

	if( stResult.SkillResultVal1 > 0)
	{
		if( (int)(RAND1(100)) <  stResult.SkillResultVal4 )
		{
			z_mon_apply_status(pstAppCtx, pstEnv, pstMon, stResult.SkillResultVal1,
			stResult.SkillResultVal2, stResult.SkillResultVal3, 1, 0);
		}
	}
	return 0;
}

//天赋 普通攻击触发回血
int genius_atk_random_add_hp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstAtker, char SkillType )
{
	DYNATTRLIST *pstDyn;
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	if( SkillType != 1 )
	{
		return 0;
	}
	pstDyn = &pstAtker->stOnline.stDyn;

	if( (int)(RAND1(100)) <  pstDyn->stHealChg.nAtkHpRate )
	{
		return pstAtker->stOnline.stDyn.stMidAttrList.uiHPMax * (pstDyn->stHealChg.nAtkHPMul/100.0) + 
				pstDyn->stHealChg.nAtkHPAdd;	
	}
	
	return 0;
}

//天赋 普通攻击触发回蓝
int genius_atk_random_add_mp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstAtker, char SkillType )
{
	DYNATTRLIST *pstDyn;
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	if( SkillType != 1 )
	{
		return 0;
	}
	pstDyn = &pstAtker->stOnline.stDyn;

	if( (int)(RAND1(100)) <  pstDyn->stHealChg.nGeniusAtkMPRate )
	{
		return pstAtker->stOnline.stDyn.stMidAttrList.uiMPMax * (pstDyn->stHealChg.nGeniusHealMPMul/100.0) + 
				pstDyn->stHealChg.unGeniusHealMPAdd;	
	}
	
	return 0;
}
// 天赋 普通攻击触发增加伤害
int genius_atk_random_add_hurt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstAtker, char SkillType,ATKCHG *pstAtkChg )
{
	DYNATTRLIST *pstDyn;
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	if( SkillType != 1 )
	{
		return 0;
	}
	pstDyn = &pstAtker->stOnline.stDyn;

	if( (int)(RAND1(100)) <  pstDyn->stHealChg.nGeniusAtkHurtRate )
	{
		pstAtkChg->stAtkerHurtChg.AtkerHurtMul += pstDyn->stHealChg.nGeniusHealHurtMul;
		pstAtkChg->stAtkerHurtChg.AtkerHurtAdd += pstDyn->stHealChg.unGeniusHealHurtAdd;
	}
				
	return 0;
}

// 天赋 对指定技能增强
int genius_atk_skill_add_hurt( Player *pstAtker, ZoneSkill *pstZoneSkill,SKILLRESULT *pstResult)
{
	UNUSED(pstAtker);
	if( RESULT_ADD_SKILL_HURT !=  pstResult->SkillResultID	)
	{
		return 0;
	}
	
	if( pstResult->SkillResultVal1 != pstZoneSkill->stSkillData.SkillID )
	{
		return 0;
	}
   	pstZoneSkill->stSkillMul.AtkerHurtMul += pstResult->SkillResultVal2;                    	/*   伤害乘修正 */
  	pstZoneSkill->stSkillMul.AtkerHurtAdd += pstResult->SkillResultVal3;                    	/*   伤害加修正 */
	
	return 0;
}
// 天赋 技能暴击率的增强
int genius_atk_skill_add_heavy_hit_rate( Player *pstAtker,ZoneSkill *pstZoneSkill, SKILLRESULT *pstResult)
{
	UNUSED(pstAtker);
	if( RESULT_ADD_SKILL_HURT !=  pstResult->SkillResultID	)
	{
		return 0;
	}
	if( pstResult->SkillResultVal1 != pstZoneSkill->stSkillData.SkillID )
	{
		return 0;
	}
   	pstZoneSkill->stSkillMul.HeavyHitRateMul += pstResult->SkillResultVal2;                    	/*   暴击加率修正 */
  	
	return 0;
}

// 天赋 临时机械速度
int genius_add_tmpmachine_spd(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,ITEMDEF *pstItemDef,int *iMspd)
{
	SKILLRESULT stResult;
	UNUSED(pstAppCtx);

	if( ITEM_TMP_MACHINE != pstItemDef->ItemTmp.Type )
	{
		return -1;
	}
	memset(&stResult,0,sizeof(SKILLRESULT));
	z_get_diathesis_result(pstEnv, pstPlayer, RESULT_TMPMACHINE_ADDSPD, &stResult);

	if( stResult.SkillResultID > 0)
	{
		*iMspd = (*iMspd) * (1 + stResult.SkillResultVal2/100.0) + stResult.SkillResultVal1;
	}
	return -1;
}

// 天赋 临时道具的积累数量
int genius_add_tmpitem_collectnum(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,ITEMDEF *pstItemDef)
{
	SKILLRESULT stResult;
	int iCollectNum = 0;
	UNUSED(pstAppCtx);
	
	memset(&stResult,0,sizeof(stResult));
	z_get_diathesis_result(pstEnv, pstPlayer, RESULT_TMPITEM_ADDNUM, &stResult);

	if( stResult.SkillResultID > 0)
	{
	 	iCollectNum = pstItemDef->ItemTmp.SkillDefaultCollectNum * ( stResult.SkillResultVal2/100.0) + stResult.SkillResultVal1;
	}
	
	return iCollectNum;
}

// 天赋 临时机械能量上限
int genius_add_tmpitem_energy(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,ITEMDEF *pstItemDef)
{
	SKILLRESULT stResult;
	int iMachineEnerge = 0;
	UNUSED(pstAppCtx);
	
	memset(&stResult,0,sizeof(stResult));
	z_get_diathesis_result(pstEnv, pstPlayer, RESULT_TMPITEM_ADDENERGY, &stResult);

	if( stResult.SkillResultID > 0)
	{
	 	iMachineEnerge = pstItemDef->ItemTmp.MachineEnerge * ( stResult.SkillResultVal2/100.0) + stResult.SkillResultVal1;
	}
	
	return iMachineEnerge;
}

// 天赋 技能消耗积累数减少
int genius_collectskill_dec(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,unsigned short SkillID)
{
	SKILLRESULT stResult;
	int iGeniusCollectSkillDec = 0;
	UNUSED(pstAppCtx);
	
	memset(&stResult,0,sizeof(stResult));
	z_get_diathesis_result(pstEnv, pstPlayer, RESULT_COLLECTSKILL_DECVAL, &stResult);

	if( stResult.SkillResultID > 0 &&  stResult.SkillResultVal1 == SkillID)
	{
	 	iGeniusCollectSkillDec = stResult.SkillResultVal2;
	}
	
	return iGeniusCollectSkillDec;
}

// 天赋 影响主动技能的等级
int genius_atkskill_addlevel(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,unsigned short SkillID)
{
	SKILLRESULT stResult;
	int iAddLevel = 0;
	UNUSED(pstAppCtx);
	
	memset(&stResult,0,sizeof(stResult));
	//z_get_diathesis_result(pstEnv, pstPlayer, RESULT_ATKSKILL_ADDLEVEL, &stResult);
	z_get_diathesis_result_by_value1(pstEnv, pstPlayer, RESULT_ATKSKILL_ADDLEVEL,SkillID, &stResult );

	if( stResult.SkillResultID > 0 &&  stResult.SkillResultVal1 == SkillID)
	{
	 	iAddLevel = stResult.SkillResultVal2;
	}
	
	return iAddLevel;
}

int get_player_level_mul(ZONESVRENV* pstEnv, Player *pstPlayer, int iType)
{
	LEVELUP *pstLevelUp;

	pstLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	if (!pstLevelUp)
	{
		return 0;
	}

	switch(iType)
	{
		case LEVEL_MUL_ATK:
			return pstLevelUp->AtkLevelMul;
			break;
		case LEVEL_MUL_DEF:
			return pstLevelUp->DefLevelMul;
			break;
		default:
			break;
	}
	
	return 0;
}

int get_mon_level_mul(ZONESVRENV* pstEnv, Monster *pstMon, int iType)
{
	LEVELUP *pstLevelUp;

	pstLevelUp = z_get_levelup(pstEnv, pstMon->stMonDynAttr.Level);
	if (!pstLevelUp)
	{
		return 0;
	}

	switch(iType)
	{
		case LEVEL_MUL_ATK:
			return pstLevelUp->AtkLevelMul;
			break;
		case LEVEL_MUL_DEF:
			return pstLevelUp->DefLevelMul;
			break;
		case LEVEL_MUL_DEF_HEAVYHIT:
			return pstLevelUp->MonDefHeavyHit;
			break;
		default:
			break;
	}
	
	return 0;
}


int get_pet_level_mul(ZONESVRENV* pstEnv, int iLevel, int iType)
{
	LEVELUP *pstLevelUp;

	pstLevelUp = z_get_levelup(pstEnv, iLevel);
	if (!pstLevelUp)
	{
		return 0;
	}

	switch(iType)
	{
		case LEVEL_MUL_ATK:
			return pstLevelUp->AtkLevelMul;
			break;
		case LEVEL_MUL_DEF:
			return pstLevelUp->DefLevelMul;
			break;
		case LEVEL_MUL_DEF_HEAVYHIT:
			return pstLevelUp->MonDefHeavyHit;
			break;
		default:
			break;
	}
	
	return 0;
}

//随机获得一个附加数值
int arm_base_attr_rand_get(ZONESVRENV* pstEnv, int iResultID)
{
	int i;
	int iRate, iRand;
	ARMRECASTDEF *pstArmRecastDef;

	pstArmRecastDef = z_find_arm_recast_def(pstEnv, iResultID);
	if (!pstArmRecastDef)
	{
		return 0;
	}
	
	iRate = 0;
	iRand = RAND1(10000);
	for(i=0; i<MAX_ARM_RECAST_LIST; i++)
	{
		iRate += pstArmRecastDef->RecastInfo[i].Rate;
		if(iRand < iRate)
		{
			int iMin = pstArmRecastDef->RecastInfo[i].MinMul;
			int iMax = pstArmRecastDef->RecastInfo[i].MaxnMul;

			if(iMax > iMin)
			{
				return (RAND1(iMax-iMin+1)+iMin);
			}
			else
			{
				return iMin;
			}
		}
	}
	
	return 0;
}

//初始附加数值
int arm_base_attr_create_rand_get(ZONESVRENV* pstEnv, int iResultID)
{
	int i;
	int iRate, iRand;
	ARMRECASTDEF *pstArmRecastDef;

	pstArmRecastDef = z_find_arm_recast_def(pstEnv, iResultID);
	if (!pstArmRecastDef)
	{
		return 0;
	}
	
	iRate = 0;
	iRand = RAND1(10000);
	for(i=0; i<MAX_ARM_RECAST_LIST; i++)
	{
		iRate += pstArmRecastDef->RecastInfo[i].CreateRate;
		if(iRand < iRate)
		{
			int iMin = pstArmRecastDef->RecastInfo[i].MinMul;
			int iMax = pstArmRecastDef->RecastInfo[i].MaxnMul;

			if(iMax > iMin)
			{
				return (RAND1(iMax-iMin+1)+iMin);
			}
			else
			{
				return iMin;
			}
		}
	}
	
	return 0;
}

//增加个基础属性的附加信息，满了就顶替前面的。
int arm_add_base_attr(ROLEARM *pstRoleArm, int iResultID, int iVal)
{
	//没位置了顶替掉最前面的
	if(pstRoleArm->BaseAttrNum >= MAX_ARM_BASE_ATTR)
	{
		memmove(&pstRoleArm->BaseAttr[0], 
			&pstRoleArm->BaseAttr[1], (MAX_ARM_BASE_ATTR-1)*sizeof(pstRoleArm->BaseAttr[0]));

		pstRoleArm->BaseAttr[MAX_ARM_BASE_ATTR-1].ResultID = iResultID;
		pstRoleArm->BaseAttr[MAX_ARM_BASE_ATTR-1].Mul = iVal;
	}
	else
	{
		pstRoleArm->BaseAttr[pstRoleArm->BaseAttrNum].ResultID = iResultID;
		pstRoleArm->BaseAttr[pstRoleArm->BaseAttrNum].Mul = iVal;
		pstRoleArm->BaseAttrNum++;
	}

	return 0;
}

//查找一个基础属性的附加信息
BASEATTRINFO* arm_base_attr_info_get(ROLEARM *pstRoleArm, int iResultID)
{
	int i;

	if(!pstRoleArm)
	{
		return NULL;
	}
	
	for(i=0; i<pstRoleArm->BaseAttrNum; i++)
	{
		if(pstRoleArm->BaseAttr[i].ResultID == iResultID)
		{
			return &pstRoleArm->BaseAttr[i];
		}
	}

	return NULL;
}

//设置装备基础属性ID附加比例
int arm_base_attr_mul_set(ROLEARM *pstRoleArm, int iResultID, int iVal)
{
	BASEATTRINFO *pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, iResultID);

	if(!pstBaseAttrInfo)
	{
		arm_add_base_attr(pstRoleArm, iResultID, iVal);
	}
	else
	{
		pstBaseAttrInfo->Mul = iVal;
	}
	
	return 0;
}

//装备随机属性数值初始值生成
int arm_base_attr_create(ZONESVRENV* pstEnv, ITEMDEF *pstItemDef, ROLEARM *pstRoleArm)
{
	int i;
	
	for (i=0; i<MAX_ITEM_RESULT; i++)	
	{
		//没位置了
		if(pstRoleArm->BaseAttrNum >= MAX_ARM_BASE_ATTR)
		{
			break;
		}
		
		if (0 == pstItemDef->Result[i].ResultID)
		{
			break;
		}

		arm_add_base_attr(pstRoleArm, 
			pstItemDef->Result[i].ResultID, 
			arm_base_attr_create_rand_get(pstEnv, pstItemDef->Result[i].ResultID));
	}

	return 0;
}

//获得装备指定效果增加的数值
int arm_base_attr_val_get(ROLEARM *pstRoleArm , ITEMDEF *pstItemDef, int iResultID)
{
	RESULT stResult;
	BASEATTRINFO *pstBaseAttrInfo;
		
	z_get_arm_result_total(pstItemDef, &stResult, iResultID);
	pstBaseAttrInfo = arm_base_attr_info_get(pstRoleArm, iResultID);
	if(pstBaseAttrInfo)
	{
		return (stResult.ResultVal1+ stResult.ResultVal2*pstBaseAttrInfo->Mul/100.0);
	}

	return 0;
}

int player_zhengyi_limit_chg(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstRes = &stPkg.Body.PlayerMiscSvr;
	ZHENGYICHG  *pstZhengyiChg = &pstRes->PlayerMiscSvrData.ZhengyiChg;
	LEVELUP *pstLevelUP;
	ZHENGYILIMITINFO *pstZhengyiLimit = &pstPlayer->stRoleData.MiscInfo.ZhengyiLimitInfo;

	if (0 == IsSameDay(pstZhengyiLimit->DayCountTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstZhengyiLimit->DayCount = 0;
		pstZhengyiLimit->DayCountTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	if (0 == IsSameDay(pstZhengyiLimit->ItemAddLimitTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstZhengyiLimit->ItemAddLimitTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstZhengyiLimit->ItemAddLimitCount = 0;
	}

	pstLevelUP = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	if (NULL == pstLevelUP)
	{
		return -1;
	}

	if (pstLevelUP->DayZhengyiLimit == 0)
	{
		pstPlayer->stOnline.stDyn.stMidAttrList.uiDayZhengyiMax = 0;
	}
	else
	{
		pstPlayer->stOnline.stDyn.stMidAttrList.uiDayZhengyiMax = 
			pstLevelUP->DayZhengyiLimit + pstZhengyiLimit->ItemAddLimitCount;
	}

	pstRes->PlayerMiscType = ZHENGYI_LIMIT_CHG;
	pstZhengyiChg->DayCount = pstZhengyiLimit->DayCount;
	pstZhengyiChg->DayCountLimit = pstPlayer->stOnline.stDyn.stMidAttrList.uiDayZhengyiMax;

	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int player_add_zhengyi_test(ZONESVRENV* pstEnv, Player *pstPlayer, unsigned int *piAdd)
{
	ZHENGYILIMITINFO *pstZhengyiLimit = &pstPlayer->stRoleData.MiscInfo.ZhengyiLimitInfo;
	MIDATTRLIST *pstMidAttrList = &pstPlayer->stOnline.stDyn.stMidAttrList;
	unsigned int iMax;

	if (pstMidAttrList->uiDayZhengyiMax > 0)
	{
		iMax = pstMidAttrList->uiDayZhengyiMax;
	}
	else
	{
		iMax = MAX_ZHENGYI;
	}

	if ((*piAdd) + pstZhengyiLimit->DayCount > iMax)
	{
		if (iMax > (unsigned int)pstZhengyiLimit->DayCount)
		{
			*piAdd = iMax - pstZhengyiLimit->DayCount;
		}
		else
		{
			*piAdd = 0;
		}
		
		return -1;
	}

	return 0;
}

int player_add_zhengyi_limit(ZONESVRENV* pstEnv, Player *pstPlayer, int iZhengyiLimit)
{
	ZHENGYILIMITINFO *pstZhengyiLimit = &pstPlayer->stRoleData.MiscInfo.ZhengyiLimitInfo;

	if (0 == IsSameDay(pstZhengyiLimit->ItemAddLimitTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstZhengyiLimit->ItemAddLimitTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstZhengyiLimit->ItemAddLimitCount = 0;
	}

	pstZhengyiLimit->ItemAddLimitCount += iZhengyiLimit;

	player_zhengyi_limit_chg(pstEnv, pstPlayer);
	
	return 0;
}


int player_life_brand_limit_chg(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	LEVELUP *pstLevelUP;
	LIFEBRANDLIMITINFO *pstLifeBrandLimit = &pstPlayer->stRoleData.MiscInfo.LifeBrandLimitInfo;

	if (0 == IsSameDay(pstLifeBrandLimit->DayCountTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstLifeBrandLimit->DayCount = 0;
		pstLifeBrandLimit->DayCountTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	pstLevelUP = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	if (NULL == pstLevelUP)
	{
		return -1;
	}

	if (pstLevelUP->GreenGemLimit == 0)
	{
		pstPlayer->stOnline.stDyn.stMidAttrList.usDayLifeBrandMax= 0;
	}
	else
	{
		pstPlayer->stOnline.stDyn.stMidAttrList.usDayLifeBrandMax = pstLevelUP->GreenGemLimit;
	}
	
	return 0;
}


int player_add_lifebrand_test(ZONESVRENV* pstEnv, Player *pstPlayer, unsigned short *piAdd)
{
	LIFEBRANDLIMITINFO *pstLifeBrandLimit = &pstPlayer->stRoleData.MiscInfo.LifeBrandLimitInfo;
	MIDATTRLIST *pstMidAttrList = &pstPlayer->stOnline.stDyn.stMidAttrList;
	unsigned short iMax;

	if (pstMidAttrList->usDayLifeBrandMax> 0)
	{
		iMax = pstMidAttrList->usDayLifeBrandMax;

		if ((*piAdd) + pstLifeBrandLimit->DayCount > iMax)
		{
			if (iMax > (unsigned short)pstLifeBrandLimit->DayCount)
			{
				*piAdd = iMax - pstLifeBrandLimit->DayCount;
			}
			else
			{
				*piAdd = 0;
			}
			
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ATTR802);
		}
	}
	
	if(pstPlayer->stRoleData.RoleDetail.GreenGem + (*piAdd) > MAX_LIFT_BRAND)
	{
		if(MAX_LIFT_BRAND > pstPlayer->stRoleData.RoleDetail.GreenGem)
		{
			*piAdd = MAX_LIFT_BRAND - pstPlayer->stRoleData.RoleDetail.GreenGem;
		}
		else
		{
			*piAdd =0;
		}

		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ATTR801);
		return -1;
	}
	
	return 0;
}


int z_add_attr_lifebrand(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, AttrChgVal *pstVal, CSPLAYERATTRVAL *pstChg)
{
	LIFEBRANDLIMITINFO *pstLifeBrandLimit = &pstPlayer->stRoleData.MiscInfo.LifeBrandLimitInfo;
	unsigned short iOldLifeBrand = pstPlayer->stRoleData.RoleDetail.GreenGem;

	if (pstVal->nFlag)
	{
		unsigned short iOld = pstPlayer->stRoleData.RoleDetail.GreenGem;
		if (pstPlayer->stRoleData.RoleDetail.GreenGem >= pstVal->Value.LiftBrand)
		{
			pstPlayer->stRoleData.RoleDetail.GreenGem -= pstVal->Value.LiftBrand;
		}
		else
		{
			pstPlayer->stRoleData.RoleDetail.GreenGem = 0;
		}

		if (iOld > pstPlayer->stRoleData.RoleDetail.GreenGem)
		{
			z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_LIFE_BRAND_DEC, iOld-pstPlayer->stRoleData.RoleDetail.GreenGem);
			z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_LIFE_BRAND_DEC, iOld-pstPlayer->stRoleData.RoleDetail.GreenGem);
		}
	}
	else
	{
		player_add_lifebrand_test(pstEnv, pstPlayer, &pstVal->Value.LiftBrand);
		
		pstPlayer->stRoleData.RoleDetail.GreenGem += pstVal->Value.LiftBrand;
		pstLifeBrandLimit->DayCount += pstVal->Value.LiftBrand;

		if (pstVal->Value.LiftBrand > 0)
		{
			z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_LIFE_BRAND_ADD, pstVal->Value.LiftBrand);
			z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_LIFE_BRAND_ADD, pstVal->Value.LiftBrand);
		}
	}

	if (iOldLifeBrand != pstPlayer->stRoleData.RoleDetail.GreenGem)
	{
		pstChg->AttrList[pstChg->AttrNum].AttrID = ATTR_ID_LIFT_BRAND;
		pstChg->AttrList[pstChg->AttrNum].Value.LiftBrand = pstPlayer->stRoleData.RoleDetail.GreenGem;
		pstChg->AttrNum++;
	}
	
	return 0;
}


//lzk   状态改变技能等级
int z_status_chg_skill_level(ZONESVRENV* pstEnv,Player* pstPlayer)
{
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	STATUSRESULT stResult;
	
	z_get_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_CHG_SKILL_LEVEL, &stResult, 1);

	{
		if(stResult.ResultVal1)
		{
			pstDyn->astStatusChgSkill[0].unSkillID = stResult.ResultVal1;
			pstDyn->astStatusChgSkill[0].ucLevel = stResult.ResultVal2;
		}
	}
	
	return 0;
}

