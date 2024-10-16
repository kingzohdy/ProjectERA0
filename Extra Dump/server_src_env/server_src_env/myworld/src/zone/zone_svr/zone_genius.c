

#include "zone_genius.h"
#include "zone_attr.h"
#include "zone_clt.h"
#include "zone_package.h"
#include "zone_oplog.h"
#include "zone_combat.h"
#include "zone_range.h"
#include "zone_err.h"
#include  "zone_zh_name.h"

#define GENIUS_MAX_STEP 10
char GeniusTip[GENIUS_MAX_STEP+1][16]=
{
		LJY_ONE,
		LJY_TWO,
		LJY_THREE,
		LJY_FOUR,
		LJY_FIVE,
		LJY_SIX,
		LJY_SEVEN,
		LJY_EIGHT,
		LJY_NINE,
		LJY_TEN,
		LJY_UNKNOWN
};
int genius_get_mod(ZONESVRENV* pstEnv, Player *pstPlayer);
GENIUSSUBONE *genius_find_one(GENIUSINFO *pstGeniusInfo, int iType, int iAttrID);
GENIUSCAREERDEF *genius_career_def_find(ZONESVRENV* pstEnv, int iCareer)
{
	GENIUSCAREERDEF stDef;
	int iEqu, iIdx;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Career = iCareer;

	iIdx = bsearch_int(&stDef, pstObj->sGeniusCareerDef, pstObj->iGeniusCareerDef, GENIUS_CAREER_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (GENIUSCAREERDEF *)&pstObj->sGeniusCareerDef[iIdx];
	}
	
	return NULL;
}

GENIUSATTRDEF *genius_attr_def_find(ZONESVRENV* pstEnv, int iCareer, int iAttrID, int iLvl)
{
	GENIUSATTRDEF stDef;
	int iEqu, iIdx;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Career = iCareer;
	stDef.AttrID = iAttrID;
	stDef.AttrLvl = iLvl;

	iIdx = bsearch_int(&stDef, pstObj->sGeniusAttrDef, pstObj->iGeniusAttrDef, GENIUS_ATTR_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (GENIUSATTRDEF *)&pstObj->sGeniusAttrDef[iIdx];
	}
	
	return NULL;
}

GENIUSADDDEF *genius_add_def_find(ZONESVRENV* pstEnv, int iType, int iCareer)
{
	GENIUSADDDEF stDef;
	int iEqu, iIdx;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Type = iType;
	stDef.Career = iCareer;

	iIdx = bsearch_int(&stDef, pstObj->sGeniusAddDef, pstObj->iGeniusAddDef, GENIUS_ADD_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (GENIUSADDDEF *)&pstObj->sGeniusAddDef[iIdx];
	}
	return NULL;
}

int genius_list_notify(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	GENIUSINFO *pstGeniusInfo = &pstPlayer->stRoleData.MiscInfo.GeniusInfo;
	CSPKG stPkg;
	GUNIUSSVR*pstSvr = &stPkg.Body.GuniusSvr;
	
	pstSvr->Cmd = GUNIUS_SVR_OP_ALL;
	pstSvr->Data.All.All = *pstGeniusInfo;
	
	Z_CSHEAD_INIT(&stPkg.Head, GUNIUS_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int genius_one_notify(ZONESVRENV* pstEnv, Player *pstPlayer, int iType, int iAttrID, int iLvl)
{
//	GENIUSINFO *pstGeniusInfo = &pstPlayer->stRoleData.MiscInfo.GeniusInfo;
	CSPKG stPkg;
	GUNIUSSVR*pstSvr = &stPkg.Body.GuniusSvr;
	
	pstSvr->Cmd = GUNIUS_SVR_OP_ONE;
	pstSvr->Data.One.Type = iType;
	pstSvr->Data.One.One.AttrID = iAttrID;
	pstSvr->Data.One.One.Lvl = iLvl;
	
	Z_CSHEAD_INIT(&stPkg.Head, GUNIUS_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


// 角色60级开启天赋
int genius_player_open(ZONESVRENV *pstEnv, Player *pstPlayer, int iNotify)
{
	GENIUSINFO *pstGeniusInfo = &pstPlayer->stRoleData.MiscInfo.GeniusInfo;
	GENIUSCAREERDEF *pstDef;
	int i, j;

	pstDef = genius_career_def_find(pstEnv, pstPlayer->stRoleData.Career);
	if (!pstDef)
		return -1;

	pstGeniusInfo->Num = 0;
	for (i=0; i<MAX_GENIUS_TYPE; i++)
	{
		if (pstDef->Types[i].Type <=  0)
			break;
		
		pstGeniusInfo->One[pstGeniusInfo->Num].Type = pstDef->Types[i].Type;
		pstGeniusInfo->One[pstGeniusInfo->Num].Num = 0;
		for (j=0; j<MAX_GENIUS_SUBTYPE; j++)
		{
			if (pstDef->Types[i].AttrIDs[j] <= 0)
				break;
			pstGeniusInfo->One[pstGeniusInfo->Num].SubOne[j].AttrID = pstDef->Types[i].AttrIDs[j];
			pstGeniusInfo->One[pstGeniusInfo->Num].SubOne[j].Lvl = 0;
			pstGeniusInfo->One[pstGeniusInfo->Num].Num ++;
		}
		pstGeniusInfo->Num ++;
	}

	if (iNotify)
		genius_list_notify(pstEnv, pstPlayer);

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_GENUIS2);
	return 0;
}

// 策划可能会增加天赋ID以及天赋类型
int genius_player_check_in(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	GENIUSINFO *pstGeniusInfo = &pstPlayer->stRoleData.MiscInfo.GeniusInfo;
	GENIUSCAREERDEF *pstDef;
	int i, j;
	GENIUSINFO stGeniusInfo;
	GENIUSSUBONE *pstOne;
	int iNotify = 0;

	pstDef = genius_career_def_find(pstEnv, pstPlayer->stRoleData.Career);
	if (!pstDef)
		return -1;

	stGeniusInfo.Num = 0;
	for (i=0; i<MAX_GENIUS_TYPE; i++)
	{
		if (pstDef->Types[i].Type <=  0)
			break;
		
		stGeniusInfo.One[stGeniusInfo.Num].Type = pstDef->Types[i].Type;
		stGeniusInfo.One[stGeniusInfo.Num].Num = 0;
		for (j=0; j<MAX_GENIUS_SUBTYPE; j++)
		{
			if (pstDef->Types[i].AttrIDs[j] <= 0)
				break;
			stGeniusInfo.One[stGeniusInfo.Num].SubOne[j].AttrID = pstDef->Types[i].AttrIDs[j];
			stGeniusInfo.One[stGeniusInfo.Num].SubOne[j].Lvl = 0;
			
			pstOne = genius_find_one(pstGeniusInfo, pstDef->Types[i].Type, pstDef->Types[i].AttrIDs[j]);
			if (pstOne)
			{
				stGeniusInfo.One[stGeniusInfo.Num].SubOne[j].Lvl = pstOne->Lvl;
			}
			else
			{
				iNotify = 1;	
			}

			stGeniusInfo.One[stGeniusInfo.Num].Num ++;
		}
		stGeniusInfo.Num ++;
	}

	// 有增加
	if (iNotify)
	{
		*pstGeniusInfo = 	stGeniusInfo;
		//genius_list_notify(pstEnv, pstPlayer);
	}
	return 0;
}


// 角色天赋检查,登录时检查
int genius_player_check(ZONESVRENV *pstEnv, Player *pstPlayer)
{
//	GENIUSINFO *pstGeniusInfo = &pstPlayer->stRoleData.MiscInfo.GeniusInfo;
	
	if (pstPlayer->stRoleData.Level < GENIUS_OPEN_LVL)
	{
		return 0;
	}

	genius_player_check_in(pstEnv, pstPlayer);

	// 没有天赋
	/*if (pstGeniusInfo->Num <= 0)
	{
		genius_player_open(pstEnv, pstPlayer, 0);
	}*/
	return 0;
}



GENIUSSUBONE *genius_find_one(GENIUSINFO *pstGeniusInfo, int iType, int iAttrID)
{
	int i,j;

	for (i=0; i<pstGeniusInfo->Num; i++)
	{
		if (iType != pstGeniusInfo->One[i].Type)
			continue;
		
		for (j=0; j<pstGeniusInfo->One[i].Num; j++)
		{
			if (pstGeniusInfo->One[i].SubOne[j].AttrID == iAttrID)
			{
				return &pstGeniusInfo->One[i].SubOne[j];
			}
		}
	}
	return NULL;
}

char* genius_step_trans(int iStep)
{
	if (iStep > 0 && iStep <= GENIUS_MAX_STEP)
	{
		return GeniusTip[iStep-1];
	}
	return GeniusTip[GENIUS_MAX_STEP];
}

GENIUSMDFDEF *genius_mdf_def_find(ZONESVRENV* pstEnv, int iType)
{
	GENIUSMDFDEF stDef;
	int iEqu, iIdx;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Type = iType;
	
	iIdx = bsearch_int(&stDef, pstObj->sGeniusMdfDef, pstObj->iGeniusMdfDef, GENIUS_MDF_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (GENIUSMDFDEF *)&pstObj->sGeniusMdfDef[iIdx];
	}
	
	return NULL;
}

int genius_use_mul_calc(ZONESVRENV* pstEnv, int iGLvl, int iType, Player *pstPlayer)
{
	int iFyLvl;
	LEVELUP *pstLevelUp;
	GENIUSMDFDEF *pstDef;
	int iDiff;
	int i;

	if (pstEnv->pstGlobalObj->stGlobal.LevFeng.LevelFeng > 0)
	{
		iFyLvl = pstEnv->pstGlobalObj->stGlobal.LevFeng.LevelFeng;
	}
	else if (pstEnv->pstConf->MaxLevel > 0)
	{
		iFyLvl = pstEnv->pstConf->MaxLevel;
	}
	else
	{
		iFyLvl = MAX_LEVEL;
	}

	pstLevelUp = z_get_levelup(pstEnv, iFyLvl);
	if (!pstLevelUp || pstLevelUp->BaseGenius <= 0)
	{
		return 0;
	}

	pstDef = genius_mdf_def_find(pstEnv, iType);
	if (!pstDef)
	{
		return 0;
	}

	iDiff = iGLvl - pstLevelUp->BaseGenius;
	/*if (iDiff > 0 && pstPlayer->stRoleData.Level == pstEnv->pstConf->MaxLevel)
	{
		return 0;
	}*/
	
	for (i=0; i<GENIUS_MDF_MAX; i++)
	{
		if (	pstDef->Mdf[i].LowerLimit == 0 && 
			pstDef->Mdf[i].UpperLimit == 0)
		{
			break;
		}

		if (	iDiff >= pstDef->Mdf[i].LowerLimit && 
			iDiff <= pstDef->Mdf[i].UpperLimit)
		{
			return pstDef->Mdf[i].Mul;
		}
	}

	return 0;
}

int genius_clt_op_up( ZONESVRENV* pstEnv, Player *pstPlayer, GUNIUSUPCLT  *pstUp)
{
	GENIUSINFO *pstGeniusInfo = &pstPlayer->stRoleData.MiscInfo.GeniusInfo;
	GENIUSATTRDEF *pstGeniusAttrDef;
	GENIUSATTRDEF *pstGeniusAttrNextDef;
	GENIUSUPLIMIT *pstUpLimit;
	GENIUSSUBONE *pstSubOne = genius_find_one(pstGeniusInfo, pstUp->Type, pstUp->AttrID);
//	GENIUSSUBONE *pstSubElse;
//	GENIUSATTRDEF *pstGeniusElseDef;
	int iMod = 0;
    int iRet = 0;
	int iMul = 0;
	unsigned int uiExp, uiMoney;

	if (pstPlayer->stOnline.State & CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_GENUIS1);
		return -1;
	}

	
	if (pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
		return -1;
	}

	if (!pstSubOne)
		return -1;

	if (pstSubOne->Lvl != pstUp->Lvl)
	{
		return -1;
	}

	pstGeniusAttrDef = genius_attr_def_find(pstEnv,pstPlayer->stRoleData.Career, pstUp->AttrID, pstUp->Lvl);
	if (!pstGeniusAttrDef)
	{
		return -1;
	}

	if (!(pstGeniusAttrNextDef = genius_attr_def_find(pstEnv,pstPlayer->stRoleData.Career, pstUp->AttrID, pstUp->Lvl+1)))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_GENUIS3, pstGeniusAttrDef->Name);
		return -1;
	}

	pstUpLimit = &pstGeniusAttrDef->UpLimit;

	if (pstPlayer->stRoleData.Level < pstUpLimit->RoleLvlLimit)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_GENUIS4);
		return -1;
	}

	iMul = genius_use_mul_calc(pstEnv,pstSubOne->Lvl, pstUp->Type,pstPlayer);
	if (iMul > 0)
	{
		uiExp = pstUpLimit->UpUseExp*(iMul/10000.0);
		uiMoney = pstUpLimit->UpUseMoney *(iMul/10000.0);
	}
	else
	{
		uiExp = pstUpLimit->UpUseExp;
		uiMoney = pstUpLimit->UpUseMoney;
	}

	if(pstUp->Flag == 1)
	{
		//基础天赋不能以这种方式
		if(pstUp->Type == 1)
		{
			return -1;
		}

		uiExp = uiExp*2.0;
		uiMoney = 0;
	}
	
	if (pstPlayer->stRoleData.RoleDetail.Exp < uiExp)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_GENUIS5);
		return -1;
	}

	if(pstUp->Flag != 1)
	{
		if (package_dec_money_test(pstEnv, pstPlayer, pstUpLimit->UpUseMoneyType, uiMoney) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_GENUIS6, 
								z_get_money_name(pstUpLimit->UpUseMoneyType));
			return -1;
		}
	}

	if (pstUpLimit->ElseAttrLvlLimit > 0 )
	{
		iMod = genius_get_mod(pstEnv, pstPlayer);
		if (iMod < pstUpLimit->ElseAttrLvlLimit )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,  ZONEERR_GENUIS7,
						pstUpLimit->ElseAttrLvlLimit);
			return -1;
		}
		/*pstSubElse = genius_find_one(pstGeniusInfo, pstUp->Type, pstUpLimit->ElseAttrID);
		if (!pstSubElse)
		{		
			return -1;
		}

		pstGeniusElseDef = genius_attr_def_find(pstEnv,pstPlayer->stRoleData.Career, 
										pstUpLimit->ElseAttrID, pstUpLimit->ElseAttrLvlLimit);
		if (!pstGeniusElseDef)
		{
			return -1;
		}

		if (pstSubElse->Lvl < pstUpLimit->ElseAttrLvlLimit)
		{
			//z_sys_strf( pstEnv, pstPlayer, SYS_MIDDLE2, "需要%s 天赋达到%d阶%d级才可升级!", 
			//				pstGeniusElseDef->Name, pstGeniusElseDef->Step, pstGeniusElseDef->Lvl);

			z_sys_strf( pstEnv, pstPlayer, SYS_MIDDLE2, "需要%s 天赋达到%s阶%s级才可升级!", 
					pstGeniusElseDef->Name, genius_step_trans(pstGeniusElseDef->Step), genius_step_trans(pstGeniusElseDef->Lvl));
			return -1;
		}*/
	}

	pstSubOne->Lvl ++;
	genius_one_notify(pstEnv, pstPlayer, pstUp->Type, pstUp->AttrID, pstUp->Lvl+1);

	if(pstUp->Flag != 1)
	{
		iRet = package_dec_money(pstEnv, pstPlayer, pstUpLimit->UpUseMoneyType, uiMoney);
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_GENUINE, pstUp->AttrID, uiMoney-iRet, iRet, "天赋");
	}

	player_dec_exp(pstEnv, pstPlayer, uiExp);

	z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_GENUIS8, pstGeniusAttrNextDef->Name,
						genius_step_trans(pstGeniusAttrNextDef->Step), pstGeniusAttrNextDef->Lvl);

	range_single_set(pstEnv, pstPlayer, RANGE_MAIN_SINGLE_SUB_GENIUS, genius_get_mod(pstEnv, pstPlayer));

	 z_genius_oplog(pstEnv,pstPlayer,GUNIUS_CLT_OP_UP,iRet,uiMoney-iRet,  uiExp, pstUp->AttrID,pstUp->Type);
	return 0;
}

int genius_cacl_attr_var(ZONESVRENV* pstEnv, Player *pstPlayer, int iAttrID, int iLvl)
{
	GENIUSATTRDEF *pstAttrDef = genius_attr_def_find(pstEnv,pstPlayer->stRoleData.Career, iAttrID, iLvl);
	int iGeniusAdd = 0;

	if (iAttrID <= GENIUS_SUBTYPE_DIKANG_LVL)
	{
		iGeniusAdd  = pstPlayer->stOnline.stDyn.stDefChg.iGeniusMul;
	}
	
	if (pstAttrDef)
	{
		return pstAttrDef->AttrVar*(1+iGeniusAdd/100.0);
	}
	return 0;
}

int genius_cacl_add_attr(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	GENIUSINFO *pstGeniusInfo = &pstPlayer->stRoleData.MiscInfo.GeniusInfo;
	int i,j,k;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	GENIUSADDDEF *pstAddDef;
	int iMin;

	for (i=0; i<pstGeniusInfo->Num; i++)
	{	
		pstAddDef = genius_add_def_find(pstEnv, pstGeniusInfo->One[i].Type, pstPlayer->stRoleData.Career);
		if (!pstAddDef)
			continue;

		iMin = genius_get_min_lvl(pstPlayer, pstAddDef->Type);
		if (iMin <= 0)
			continue;

		for (j=0; j<MAX_GENIUS_SEG; j++)
		{
			if (	iMin >= pstAddDef->Seg[j].From &&
				iMin <= pstAddDef->Seg[j].To)
			{
				for (k=0; k<MAX_GENIUS_RESULT; k++)
				{
					if (pstAddDef->Seg[j].Add[k].ResultID != 0)
					{
						z_attr_result(pstEnv, pstDyn, NULL, &pstAddDef->Seg[j].Add[k]);
					}
				}	
				break;
			}
		}
	}
	return 0;
}

int genius_cacl_attr(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	GENIUSINFO *pstGeniusInfo = &pstPlayer->stRoleData.MiscInfo.GeniusInfo;
	int i,j;
	int iAttrID, iLvl;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;

	genius_cacl_add_attr( pstEnv, pstPlayer);

	for (i=0; i<pstGeniusInfo->Num; i++)
	{	
		for (j=0; j<pstGeniusInfo->One[i].Num; j++)
		{
			iAttrID = pstGeniusInfo->One[i].SubOne[j].AttrID;
			iLvl = pstGeniusInfo->One[i].SubOne[j].Lvl;

			switch(iAttrID)
			{
			// 基础天赋
			case GENIUS_SUBTYPE_HP:
				pstDyn->stHealChg.iHPMaxBase += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_PHYATK:
				pstDyn->stAtkChg.unPhsicBaseArmAtk += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_ICEATK:
				pstDyn->stAtkChg.unIceBaseArmAtk += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_PHYDEF:
				pstDyn->stDefChg.unPhsicDefBaseAdd += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_ICEDEF:
				pstDyn->stDefChg.unIceDefBaseAdd += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_MINGZHONG_LVL:
				pstDyn->stHitChg.iHitChgAdd += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_SHANBI_LVL:
				pstDyn->stFleeChg.unFleeBaseAdd += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_BAOJI_LVL:
				pstDyn->stHitRateChg.iHeavyHit  += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_DIKANG_LVL: // 改成了暴击抵抗
				pstDyn->stHitRateChg.iDefHeavyHit += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;

			// 狂暴天赋
			case GENIUS_SUBTYPE_SH_ADD_DAM:
				pstDyn->stAtkChg.iAtkSHHurtMul += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_ZF_ADD_DAM:
				pstDyn->stAtkChg.iAtkZFHurtMul += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_SY_ADD_DAM:
				pstDyn->stAtkChg.iAtkSYHurtMul += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_YL_ADD_DAM:
				pstDyn->stAtkChg.iAtkYLHurtMul += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_JN_ADD_DAM:
				pstDyn->stAtkChg.iAtkJNHurtMul += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_LH_ADD_DAM:
				pstDyn->stAtkChg.iAtkLHHurtMul += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_LS_ADD_DAM:
				pstDyn->stAtkChg.iAtkLSHurtMul += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_HM_ADD_DAM:
				pstDyn->stAtkChg.iAtkHMHurtMul += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;

			// 防御天赋
			case GENIUS_SUBTYPE_SH_DEC_DAM:
				pstDyn->stAtkChg.iDecSHHurtMul += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_ZF_DEC_DAM:
				pstDyn->stAtkChg.iDecZFHurtMul += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_SY_DEC_DAM:
				pstDyn->stAtkChg.iDecSYHurtMul += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_YL_DEC_DAM:
				pstDyn->stAtkChg.iDecYLHurtMul += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_JN_DEC_DAM:
				pstDyn->stAtkChg.iDecJNHurtMul += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_LH_DEC_DAM:
				pstDyn->stAtkChg.iDecLHHurtMul += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_LS_DEC_DAM:
				pstDyn->stAtkChg.iDecLSHurtMul += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			case GENIUS_SUBTYPE_HM_DEC_DAM:
				pstDyn->stAtkChg.iDecHMHurtMul += genius_cacl_attr_var(pstEnv, pstPlayer,  iAttrID, iLvl);
				break;
			default:
				break;
			}
		}
	}

	
	return 0;
}

int genius_hurt_dec_get(ZONESVRENV *pstEnv, AtkerInfo *pstAtkerInfo, AtkeeInfo *pstAtkeeInfo)
{
	int iHurtMul = 0;
	ZoneAni *pstAtkerAni = &pstAtkerInfo->stAtker;
	ZoneAni *pstAtkeeAni = &pstAtkeeInfo->stAtkee;

	if(pstAtkeeAni->iType == OBJ_PLAYER &&
		pstAtkerAni->iType == OBJ_PLAYER)
	{
		Player *psAtker = pstAtkerAni->stObj.pstPlayer;
		DYNATTRLIST *pstDyn = &pstAtkeeInfo->stDyn;


		switch(psAtker->stRoleData.Career)
		{
			case CAREER_WARRIOR_SINGLEHAND:
				if(pstDyn->stAtkChg.iDecSHHurtMul > 0)
				{
					iHurtMul -= pstDyn->stAtkChg.iDecSHHurtMul;
				}
				break;
			case CAREER_WARRIOR_DOUBLEHAND:	
				if(pstDyn->stAtkChg.iDecZFHurtMul > 0)
				{
					iHurtMul -= pstDyn->stAtkChg.iDecZFHurtMul;
				}
				break;
			case CAREER_MAGICOR_SINGLEHAND:	
				if(pstDyn->stAtkChg.iDecJNHurtMul > 0)
				{
					iHurtMul -= pstDyn->stAtkChg.iDecJNHurtMul;
				}
				break;
			case CAREER_MAGICOR_DOUBLEHAND:				
				if(pstDyn->stAtkChg.iDecLHHurtMul > 0)
				{
					iHurtMul -= pstDyn->stAtkChg.iDecLHHurtMul;
				}
				break;
			case CAREER_GUNER_SINGLEHAND:	
				if(pstDyn->stAtkChg.iDecLSHurtMul > 0)
				{
					iHurtMul -= pstDyn->stAtkChg.iDecLSHurtMul;
				}
				break;
			case CAREER_GUNER_DOUBLEHAND:			
				if(pstDyn->stAtkChg.iDecHMHurtMul > 0)
				{
					iHurtMul -= pstDyn->stAtkChg.iDecHMHurtMul;
				}
				break;
		
			case CAREER_CHURCH_SINGLEHAND:
				if(pstDyn->stAtkChg.iDecSYHurtMul > 0)
				{
					iHurtMul -= pstDyn->stAtkChg.iDecSYHurtMul;
				}
				break;
			case CAREER_CHURCH_DOUBLEHAND:			
				if(pstDyn->stAtkChg.iDecYLHurtMul > 0)
				{
					iHurtMul -= pstDyn->stAtkChg.iDecYLHurtMul;
				}
				break;
			default:
				break;
		}
	}
	return iHurtMul;
}

int genius_hurt_mul_get(ZONESVRENV *pstEnv, AtkerInfo *pstAtkerInfo, AtkeeInfo *pstAtkeeInfo)
{
	int iHurtMul = 0;
	ZoneAni *pstAtkerAni = &pstAtkerInfo->stAtker;
	ZoneAni *pstAtkeeAni = &pstAtkeeInfo->stAtkee;

	if(pstAtkeeAni->iType == OBJ_PLAYER &&
		pstAtkerAni->iType == OBJ_PLAYER)
	{
		Player *psAtkee = pstAtkeeAni->stObj.pstPlayer;
		DYNATTRLIST *pstDyn = &pstAtkerInfo->stDyn;


		switch(psAtkee->stRoleData.Career)
		{
			case CAREER_WARRIOR_SINGLEHAND:
				if(pstDyn->stAtkChg.iAtkSHHurtMul > 0)
				{
					iHurtMul += pstDyn->stAtkChg.iAtkSHHurtMul;
				}
				break;
			case CAREER_WARRIOR_DOUBLEHAND:	
				if(pstDyn->stAtkChg.iAtkZFHurtMul > 0)
				{
					iHurtMul += pstDyn->stAtkChg.iAtkZFHurtMul;
				}
				break;
			case CAREER_MAGICOR_SINGLEHAND:	
				if(pstDyn->stAtkChg.iAtkJNHurtMul > 0)
				{
					iHurtMul += pstDyn->stAtkChg.iAtkJNHurtMul;
				}
				break;
			case CAREER_MAGICOR_DOUBLEHAND:				
				if(pstDyn->stAtkChg.iAtkLHHurtMul > 0)
				{
					iHurtMul += pstDyn->stAtkChg.iAtkLHHurtMul;
				}
				break;
			case CAREER_GUNER_SINGLEHAND:	
				if(pstDyn->stAtkChg.iAtkLSHurtMul > 0)
				{
					iHurtMul += pstDyn->stAtkChg.iAtkLSHurtMul;
				}
				break;
			case CAREER_GUNER_DOUBLEHAND:			
				if(pstDyn->stAtkChg.iAtkHMHurtMul > 0)
				{
					iHurtMul += pstDyn->stAtkChg.iAtkHMHurtMul;
				}
				break;
		
			case CAREER_CHURCH_SINGLEHAND:
				if(pstDyn->stAtkChg.iAtkSYHurtMul > 0)
				{
					iHurtMul += pstDyn->stAtkChg.iAtkSYHurtMul;
				}
				break;
			case CAREER_CHURCH_DOUBLEHAND:			
				if(pstDyn->stAtkChg.iAtkYLHurtMul > 0)
				{
					iHurtMul += pstDyn->stAtkChg.iAtkYLHurtMul;
				}
				break;
			default:
				break;
		}

		
	}
	
	return iHurtMul;
}


// 获取某种类型的天赋系数
int genius_get_mod_by_type(ZONESVRENV* pstEnv, Player *pstPlayer, int iType)
{
	int iMod = 0;
	int i,j;
	GENIUSINFO *pstGeniusInfo = &pstPlayer->stRoleData.MiscInfo.GeniusInfo;

	for (i=0; i<pstGeniusInfo->Num; i++)
	{	
		if (pstGeniusInfo->One[i].Type == iType)
		{
			for (j=0; j<pstGeniusInfo->One[i].Num; j++)
			{
				iMod += pstGeniusInfo->One[i].SubOne[j].Lvl;
			}
			break;
		}
	}
	
	return iMod;
}

// 获取天赋系数
int genius_get_mod(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i,j;
	int iMod = 0;
	GENIUSINFO *pstGeniusInfo = &pstPlayer->stRoleData.MiscInfo.GeniusInfo;

	for (i=0; i<pstGeniusInfo->Num; i++)
	{	
		for (j=0; j<pstGeniusInfo->One[i].Num; j++)
		{
			iMod += pstGeniusInfo->One[i].SubOne[j].Lvl;
		}
	}
	return iMod;
}

int genius_get_mod1(ROLEDATA *pstRoleData)
{
	int i,j;
	int iMod = 0;
	GENIUSINFO *pstGeniusInfo = &pstRoleData->MiscInfo.GeniusInfo;

	for (i=0; i<pstGeniusInfo->Num; i++)
	{	
		for (j=0; j<pstGeniusInfo->One[i].Num; j++)
		{
			iMod += pstGeniusInfo->One[i].SubOne[j].Lvl;
		}
	}
	return iMod;
}

// 获取某个类型天赋中等级最小的那个
int genius_get_min_lvl(Player *pstPlayer, int iType)
{
	int iMin = 0x7fffffff;
	int i,j;
	GENIUSINFO *pstGeniusInfo = &pstPlayer->stRoleData.MiscInfo.GeniusInfo;

	for (i=0; i<pstGeniusInfo->Num; i++)
	{	
		if (pstGeniusInfo->One[i].Type != iType)
		{
			continue;
		}
		
		for (j=0; j<pstGeniusInfo->One[i].Num; j++)
		{
			if (iMin > pstGeniusInfo->One[i].SubOne[j].Lvl)
			{
				iMin = pstGeniusInfo->One[i].SubOne[j].Lvl;
			}
		}
	}

	if (iMin == 0x7fffffff)
		iMin = 0;
	
	return iMin;
}

// 升级检查天赋值
int genius_check_level_up(ZONESVRENV* pstEnv, Player *pstPlayer,  LEVELUP *pstLvlUp)
{
	int iMod = genius_get_mod(pstEnv, pstPlayer);

	if (pstLvlUp->FzMod > 0 && iMod < pstLvlUp->FzMod)
	{
		return -1;
	}
	return 0;
}

int genius_clt_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	GUNIUSCLT *pstClt = &pstCsPkg->Body.GuniusClt;

	if (pstClt->Cmd == GUNIUS_CLT_OP_UP)
	{
		if (pstEnv->pstConf->IsMatchWorld)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET3);
			return 0;
		}
		genius_clt_op_up(  pstEnv, pstPlayer, &pstClt->Data.Up);	
	}
	return 0;
}

int genius_gm_clear(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	GENIUSINFO *pstGeniusInfo = &pstPlayer->stRoleData.MiscInfo.GeniusInfo;
	int i,j;
	
	for (i=0; i<pstGeniusInfo->Num; i++)
	{
		for (j=0; j<pstGeniusInfo->One[i].Num; j++)
		{
			pstGeniusInfo->One[i].SubOne[j].Lvl = 0;
		}
	}

	z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	genius_list_notify(pstEnv, pstPlayer);
	return 0;
}


