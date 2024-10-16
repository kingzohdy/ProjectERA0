/*
**  @file $RCSfile: zone_val.c,v $
**  general description of this module
**  $Id: zone_val.c,v 1.15 2013/10/25 08:26:11 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2013/10/25 08:26:11 $
**  @version $Revision: 1.15 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_val.h"
#include "zone_clt.h"
#include "zone_skill.h"
#include "zone_combat.h"
#include "zone_ai.h"
#include "zone_npc.h"
#include "zone_attr.h"
#include "zone_status.h"
#include "zone_err.h"

static int fight_dyn_attr_update(DYNATTRLIST *pstDyn, CSFIGHTOBJINFO *pstCSAtkInfo)
{
	int i, j;
	ATKCHG *pstAtkChg;
	CSPLAYERATTRVAL *pstAttrVal = &pstCSAtkInfo->FightAttrVal;

	//攻击系数
	if(pstCSAtkInfo->AtkLevelMul > 0)
	{
		for (i=0; i<MAX_ARM_ON; i++)
		{
			pstAtkChg = &pstDyn->stAtkChg;
			pstAtkChg->iAtkLevelMul = pstCSAtkInfo->AtkLevelMul;
		}
	}

	//防御系数
	if(pstCSAtkInfo->DefLevelMul > 0)
	{
		pstDyn->stDefChg.iDefLevelMul = pstCSAtkInfo->DefLevelMul;
	}

	for(i = 0; i<pstAttrVal->AttrNum; i++)
	{
		switch(pstAttrVal->AttrList[i].AttrID)
		{
			case ATTR_ID_ICE_ATK:
				{
					for (j=0; j<MAX_ARM_ON; j++)
					{
						pstAtkChg = &pstDyn->stAtkChg;
						pstAtkChg->unIceBaseArmAtk = pstAttrVal->AttrList[i].Value.IceAtk;
						pstAtkChg->nIceAtkChgMul = 0;
						pstAtkChg->iIceAtkChgAdd = 0;
					}
				}
				break;
			case ATTR_ID_PHSIC_ATK:
				{
					for (j=0; j<MAX_ARM_ON; j++)
					{
						pstAtkChg = &pstDyn->stAtkChg;
						pstAtkChg->unPhsicBaseArmAtk= pstAttrVal->AttrList[i].Value.PhsicAtk;
						pstAtkChg->nPhsicAtkChgMul = 0;
						pstAtkChg->iPhsicAtkChgAdd = 0;
					}
				}
				break;
			case ATTR_ID_WATER_ATK:
				{
					for (j=0; j<MAX_ARM_ON; j++)
					{
						pstAtkChg = &pstDyn->stAtkChg;
						pstAtkChg->unWaterBaseArmAtk = pstAttrVal->AttrList[i].Value.WaterAtk;
						pstAtkChg->nWaterAtkChgMul = 0;
						pstAtkChg->iWaterAtkChgAdd = 0;
					}
				}
				break;
			case ATTR_ID_FIRE_ATK:
				{
					for (j=0; j<MAX_ARM_ON; j++)
					{
						pstAtkChg = &pstDyn->stAtkChg;
						pstAtkChg->unFireBaseArmAtk += pstAttrVal->AttrList[i].Value.FireAtk;
						pstAtkChg->nFireAtkChgMul = 0;
						pstAtkChg->iFireAtkChgAdd = 0;
					}
				}
				break;
			case ATTR_ID_THUNDER_ATK:
				{
					for (j=0; j<MAX_ARM_ON; j++)
					{
						pstAtkChg = &pstDyn->stAtkChg;
						pstAtkChg->unThunderBaseArmAtk = pstAttrVal->AttrList[i].Value.ThunderAtk;
						pstAtkChg->nThunderAtkChgMul = 0;
						pstAtkChg->iThunderAtkChgAdd = 0;
					}
				}
				break;
			case ATTR_ID_LIGHT_ATK:
				{
					for (j=0; j<MAX_ARM_ON; j++)
					{
						pstAtkChg = &pstDyn->stAtkChg;
						pstAtkChg->unLightBaseArmAtk = pstAttrVal->AttrList[i].Value.LightAtk;
						pstAtkChg->nLightAtkChgMul = 0;
						pstAtkChg->iLightAtkChgAdd = 0;
					}
				}
				break;
			case ATTR_ID_NIGHT_ATK:
				{
					for (j=0; j<MAX_ARM_ON; j++)
					{
						pstAtkChg = &pstDyn->stAtkChg;
						pstAtkChg->unNightBaseArmAtk = pstAttrVal->AttrList[i].Value.NightAtk;
						pstAtkChg->nNightAtkChgMul = 0;
						pstAtkChg->iNightAtkChgAdd = 0;
					}
				}
				break;

			case ATTR_ID_HEAVY:
				pstDyn->stHitRateChg.nHeavyHitAdd = pstAttrVal->AttrList[i].Value.Heavy;
				break;
			case ATTR_ID_BASE_ATTR_HEAVY:
				pstDyn->stHitRateChg.iHeavyHit = pstAttrVal->AttrList[i].Value.BaseAttrHeavy;
				break;
			case ATTR_ID_HEAVY_VAL:
				pstDyn->stHitRateChg.nHeavyHitVal = pstAttrVal->AttrList[i].Value.HeavyVal*100;
				break;
			case ATTR_ID_HIT:
				pstDyn->stHitChg.unHitBaseAdd = pstAttrVal->AttrList[i].Value.Hit;
				pstDyn->stHitChg.nHitChgMul = 0;
				pstDyn->stHitChg.iHitChgAdd = 0;
				break;
			case ATTR_ID_HIT_RATE:
				pstDyn->stHitRateChg.nAtkerHitRateAdd = pstAttrVal->AttrList[i].Value.HitRate;
				break;

			case ATTR_ID_DEF_PHSIC_HURT:
				pstDyn->stAtkeeHurtChg.AtkeePhsicHurtAdd = pstAttrVal->AttrList[i].Value.DefPhsicHurt;
				break;
			case ATTR_ID_DEF_ICE_HURT:
				pstDyn->stAtkeeHurtChg.AtkeeIceHurtAdd = pstAttrVal->AttrList[i].Value.DefIceHurt;
				break;
			case ATTR_ID_PHSIC_HURT:
				{
					for (j=0; j<MAX_ARM_ON; j++)
					{
						pstAtkChg = &pstDyn->stAtkChg;
						pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtMul = pstAttrVal->AttrList[i].Value.PhsicHurt;
					}
				}
				break;
			case ATTR_ID_ICE_HURT:
				{
					for (j=0; j<MAX_ARM_ON; j++)
					{
						pstAtkChg = &pstDyn->stAtkChg;
						pstAtkChg->stAtkerHurtChg.AtkerIceHurtMul = pstAttrVal->AttrList[i].Value.IceHurt;
					}
				}
				break;
			case ATTR_ID_PHSIC_DEF:
				pstDyn->stDefChg.unPhsicDefBaseAdd = pstAttrVal->AttrList[i].Value.PhsicDef;
				pstDyn->stDefChg.nPhsicDefChgMul = 0;
				pstDyn->stDefChg.iPhsicDefChgAdd = 0;
				break;
			case ATTR_ID_ICE_DEF:
				pstDyn->stDefChg.unIceDefBaseAdd = pstAttrVal->AttrList[i].Value.IceDef;
				pstDyn->stDefChg.nIceDefChgMul = 0;
				pstDyn->stDefChg.iIceDefChgAdd = 0;
				break;
			case ATTR_ID_FLEE:
				pstDyn->stFleeChg.unFleeBaseAdd = pstAttrVal->AttrList[i].Value.Flee;
				pstDyn->stFleeChg.nFleeChgMul = 0;
				pstDyn->stFleeChg.iFleeChgAdd = 0;
				break;
			case ATTR_ID_BASE_ATTR_DEFHEAVY:
				pstDyn->stHitRateChg.iDefHeavyHit = pstAttrVal->AttrList[i].Value.BaseAttrDefHeavy;
				break;
			case ATTR_ID_WATER_DEF:
				pstDyn->stDefChg.unWaterDefBaseAdd = pstAttrVal->AttrList[i].Value.WaterDef;
				pstDyn->stDefChg.nWaterDefChgMul = 0;
				pstDyn->stDefChg.iWaterDefChgAdd	= 0;
				break;
			case ATTR_ID_FIRE_DEF:
				pstDyn->stDefChg.unFireDefBaseAdd = pstAttrVal->AttrList[i].Value.FireDef;
				pstDyn->stDefChg.nFireDefChgMul = 0;
				pstDyn->stDefChg.iFireDefChgAdd	= 0;
				break;
			case ATTR_ID_THUNDER_DEF:
				pstDyn->stDefChg.unThunderDefBaseAdd = pstAttrVal->AttrList[i].Value.ThunderDef;
				pstDyn->stDefChg.nThunderDefChgMul = 0;
				pstDyn->stDefChg.iThunderDefChgAdd = 0;
				break;
			case ATTR_ID_LIGHT_DEF:
				pstDyn->stDefChg.unLightDefBaseAdd = pstAttrVal->AttrList[i].Value.LightDef;
				pstDyn->stDefChg.nLightDefChgMul = 0;
				pstDyn->stDefChg.iLightDefChgAdd	= 0;
				break;
			case ATTR_ID_NIGHT_DEF:
				pstDyn->stDefChg.unNightDefBaseAdd = pstAttrVal->AttrList[i].Value.NightDef;
				pstDyn->stDefChg.nNightDefChgMul = 0;
				pstDyn->stDefChg.iNightDefChgAdd	= 0;
				break;
			default:
				break;
		}
	}

	return 0;
}

static int player_atker_info_set(ZONESVRENV* pstEnv, Player *pstPlayer, 
								AtkProcess *pstAtkProcess, CSFIGHTOBJINFO *pstCSAtkerInfo)
{
	int i;
	ATKCHG *pstAtkChg = NULL;
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;
	DYNATTRLIST *pstAtkerDyn = &pstAtkerInfo->stDyn;
	SKILLDEF *pstSkillDef = pstAtkProcess->stAtkerInfo.pstSkillDef;
	
	memcpy(pstAtkerDyn, &pstPlayer->stOnline.stDyn, sizeof(*pstAtkerDyn));

	if (0 > z_check_skill_arm(pstEnv, pstPlayer, NULL, pstSkillDef, &pstAtkProcess->iAtkChgIdx))
	{
		return -1;
	}
	
	if (0 > pstAtkProcess->iAtkChgIdx)
	{
		pstAtkProcess->iAtkChgIdx = 0;
		memset(&pstAtkerDyn->stAtkChg, 0, sizeof(pstAtkerDyn->stAtkChg));
		pstAtkerDyn->stAtkChg.iAtkLevelMul = get_player_level_mul(pstEnv, pstPlayer, LEVEL_MUL_ATK);
	}
	
	pstAtkChg = &pstAtkerDyn->stAtkChg;

	  //检查自己好的状态对技能的修正
	for (i=0; i<pstPlayer->stOnline.stStatusList.bGoodStatusNum; i++)
	{
		z_atker_status_result(pstEnv, pstSkillDef, pstAtkerInfo, pstAtkChg, &pstPlayer->stOnline.stStatusList.astGoodStatus[i],pstAtkProcess);
	}

	//状态影响技能效果
	if (pstSkillDef->StatusChg.StatusID > 0 && z_player_find_status(pstPlayer, pstSkillDef->StatusChg.StatusID))
	{
		int iStatusChgHurt = 0;
		int iStatusChgHurtMul = 0;
		ZoneStatus *pstZoneStatus = NULL;

		iStatusChgHurt = pstSkillDef->StatusChg.Hurt;
		iStatusChgHurtMul = pstSkillDef->StatusChg.HurtMul;
		
		pstZoneStatus = z_player_find_status(pstPlayer, pstSkillDef->StatusChg.StatusID);
		if(pstZoneStatus)
		{
			z_status_mul_skill_hurt(pstEnv,pstZoneStatus ,&iStatusChgHurt,&iStatusChgHurtMul);
		}

		pstAtkChg->stAtkerHurtChg.AtkerHurtAdd += iStatusChgHurt;
		pstAtkChg->stAtkerHurtChg.AtkerHurtMul += iStatusChgHurtMul;

		pstAtkerDyn->stHitRateChg.nHeavyHitAdd += pstSkillDef->StatusChg.Heavy;
	}

	fight_dyn_attr_update(pstAtkerDyn, pstCSAtkerInfo);

	return 0;
}

static int player_atkee_info_set(ZONESVRENV* pstEnv, Player *pstPlayer, AtkeeInfo *pstAtkeeInfo, AtkProcess *pstAtkProcess, CSFIGHTOBJINFO *pstCSAtkeeInfo)
{
	int i;
	ZoneStatusList *pstZoneStatusList = &pstPlayer->stOnline.stStatusList;
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;
	DYNATTRLIST *pstAtkerDyn = &pstAtkerInfo->stDyn;
	ATKCHG *pstAtkChg = &pstAtkerDyn->stAtkChg;
	
	pstAtkeeInfo->stAtkee.iType = OBJ_PLAYER;
	pstAtkeeInfo->stAtkee.stObj.pstPlayer = pstPlayer;
	
	memcpy(&pstAtkeeInfo->stDyn, &pstPlayer->stOnline.stDyn, sizeof(pstAtkeeInfo->stDyn));

	//被攻击者的状态影响攻击者的数值
	for (i=0; i<pstZoneStatusList->bBadStatusNum; i++)
	{
		z_atkee_status_result(pstEnv, pstAtkerInfo->pstSkillDef, pstAtkerInfo, pstAtkChg, &pstZoneStatusList->astBadStatus[i]);
	}

	if (pstAtkerInfo->stAtker.iType == OBJ_MONSTER && pstAtkerInfo->stAtker.stObj.pstMon->iOwnerID == 0)
	{
		pstAtkerDyn->stAtkeeHurtChg.AtkeeHurtMul += pstAtkerDyn->stMonChg.nAtkeeHurtMul;
		pstAtkerDyn->stAtkeeHurtChg.AtkeeHurtAdd +=  pstAtkerDyn->stMonChg.unAtkeeHurtAdd;
		pstAtkerDyn->stFleeChg.nFleeChgMul += pstAtkerDyn->stMonChg.nFleeChgMul;
		pstAtkerDyn->stFleeChg.iFleeChgAdd += pstAtkerDyn->stMonChg.unFleeChgAdd;
		pstAtkerDyn->stAtkeeHurtChg.AtkeeMagicHurtAdd += pstAtkerDyn->stMonChg.nAtkeeMagicDefAdd;
		pstAtkerDyn->stAtkeeHurtChg.AtkeeMagicHurtMul += pstAtkerDyn->stMonChg.nAtkeeMagicDefMul;
		pstAtkerDyn->stAtkeeHurtChg.AtkeeIceHurtAdd += pstAtkerDyn->stMonChg.nAtkeeIceDefAdd;
		pstAtkerDyn->stAtkeeHurtChg.AtkeeIceHurtMul += pstAtkerDyn->stMonChg.nAtkeeIceDefMul;
		pstAtkerDyn->stAtkeeHurtChg.AtkeePhsicHurtAdd += pstAtkerDyn->stMonChg.nAtkeePhsicDefAdd;
		pstAtkerDyn->stAtkeeHurtChg.AtkeePhsicHurtMul += pstAtkerDyn->stMonChg.nAtkeePhsicDefMul;
	}

	fight_dyn_attr_update(&pstAtkeeInfo->stDyn, pstCSAtkeeInfo);

	return 0;
}

static int mon_atker_info_set(ZONESVRENV* pstEnv, Monster *pstMon, 
								AtkProcess *pstAtkProcess, CSFIGHTOBJINFO *pstCSAtkerInfo)
{
	int i;
	MONSTERDEF *pstMonDef;
	ATKCHG *pstAtkChg;
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;
	DYNATTRLIST *pstAtkerDyn = &pstAtkerInfo->stDyn;
	
	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (NULL == pstMonDef)
	{
		return -1;
	}
	
	z_get_mon_dyn(pstEnv, pstMon, pstMonDef, pstAtkerDyn);
	pstAtkProcess->iAtkChgIdx = 0;
	pstAtkChg = &pstAtkerDyn->stAtkChg;

	 //检查自己好的状态对技能的修正
	for (i=0; i<pstMon->stStatusList.bGoodStatusNum; i++)
	{
		z_atker_status_result(pstEnv, pstAtkerInfo->pstSkillDef, pstAtkerInfo, pstAtkChg, &pstMon->stStatusList.astGoodStatus[i],pstAtkProcess);
	}

	z_cacl_mon_status_result(pstEnv, pstAtkerDyn, pstAtkChg, pstMon);


	fight_dyn_attr_update(pstAtkerDyn, pstCSAtkerInfo);

	return 0;
}

static int mon_atkee_info_set(ZONESVRENV* pstEnv, Monster *pstMon, AtkeeInfo *pstAtkeeInfo, AtkProcess *pstAtkProcess, CSFIGHTOBJINFO *pstCSAtkeeInfo)
{
	int i;
	MONSTERDEF *pstMonsterDef;
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;
	DYNATTRLIST *pstAtkerDyn = &pstAtkerInfo->stDyn;
	ATKCHG *pstAtkChg = &pstAtkerDyn->stAtkChg;

	pstMonsterDef = z_get_mon_def(pstEnv, pstMon);
	if (NULL == pstMonsterDef)
	{
		return -1;
	}
	
	//被攻击者信息
	pstAtkeeInfo->stAtkee.iType = OBJ_MONSTER;
	pstAtkeeInfo->stAtkee.stObj.pstMon = pstMon;

	//怪物数值
	z_get_mon_dyn(pstEnv, pstMon, pstMonsterDef, &pstAtkeeInfo->stDyn);
	z_cacl_mon_status_result(pstEnv, &pstAtkeeInfo->stDyn, NULL, pstMon);

	//被攻击者的状态影响攻击者的数值
	for (i=0; i<pstMon->stStatusList.bBadStatusNum; i++)
	{
		z_atkee_status_result(pstEnv, pstAtkerInfo->pstSkillDef, pstAtkerInfo, pstAtkChg, &pstMon->stStatusList.astBadStatus[i]);
	}

	fight_dyn_attr_update(&pstAtkeeInfo->stDyn, pstCSAtkeeInfo);
	
	return 0;
}

static int fight_atker_info(ZONESVRENV* pstEnv, AtkProcess *pstAtkProcess, 
						CSVALFIGHTCLT *pstFightClt, CSFIGHTOBJINFO *pstCSAtkerInfo)
{	int iIndex;
	char cType;
	TMEMBLOCK *pstMemBlock;
	Player *pstPlayer;
	Monster *pstMon;
	SKILLDEF *pstSkillDef;

	pstSkillDef = z_find_skill(pstEnv, pstFightClt->SkillID, pstFightClt->SkillLevel, NULL);
	if(NULL == pstSkillDef || pstSkillDef->NoHurtSkill)
	{
		return -1;
	}
	
	iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstCSAtkerInfo->MemID, &cType);
	if (0 > iIndex)
	{
		return -1;
	}

	switch(cType)
	{
		case OBJ_PLAYER:
			{
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					return -1;
				}

				pstPlayer = (Player *)pstMemBlock->szData;
				if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
				{
					return -1;
				}

				pstAtkProcess->stAtkerInfo.stAtker.iType = OBJ_PLAYER;
				pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer = pstPlayer;
				pstAtkProcess->stAtkerInfo.pstZoneSkill = NULL;
				pstAtkProcess->stAtkerInfo.pstSkillDef = pstSkillDef;
				pstAtkProcess->stAtkerInfo.pstOriginDef = pstSkillDef;

				if(0 > player_atker_info_set(pstEnv, pstPlayer, pstAtkProcess, pstCSAtkerInfo))
				{
					return -1;
				}
			}
			break;
		case OBJ_MONSTER:
			{
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					return -1;
				}

				pstMon = (Monster *)pstMemBlock->szData;
				if(pstMon->cStat == MONSTER_DIE || pstMon->bAiStat == AI_STAT_HOME)
				{
					return -1;
				}

				pstAtkProcess->stAtkerInfo.stAtker.iType = OBJ_MONSTER;
				pstAtkProcess->stAtkerInfo.stAtker.stObj.pstMon = pstMon;
				pstAtkProcess->stAtkerInfo.pstZoneSkill = NULL;
				pstAtkProcess->stAtkerInfo.pstSkillDef = pstSkillDef;
				pstAtkProcess->stAtkerInfo.pstOriginDef = pstSkillDef;

				if(0 > mon_atker_info_set(pstEnv, pstMon, pstAtkProcess, pstCSAtkerInfo))
				{
					return -1;
				}
			}
			break;
		default:
			return -1;
			break;
	}
	
	return 0;
}



static int fight_atkee_info(ZONESVRENV* pstEnv, AtkeeInfo *pstAtkeeInfo, AtkProcess *pstAtkProcess, CSFIGHTOBJINFO *pstCSAtkeeInfo)
{
	int iIndex;
	char cType;
	TMEMBLOCK *pstMemBlock;
	Player *pstPlayer;
	Monster *pstMon;

	iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstCSAtkeeInfo->MemID, &cType);
	if (0 > iIndex)
	{
		return -1;
	}

	switch(cType)
	{
		case OBJ_PLAYER:
			{
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					return -1;
				}

				pstPlayer = (Player *)pstMemBlock->szData;
				if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
				{
					return -1;
				}

				player_atkee_info_set(pstEnv, pstPlayer, pstAtkeeInfo, pstAtkProcess, pstCSAtkeeInfo);
			}
			break;
		case OBJ_MONSTER:
			{
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					return -1;
				}

				pstMon = (Monster *)pstMemBlock->szData;
				if(pstMon->cStat == MONSTER_DIE || pstMon->bAiStat == AI_STAT_HOME)
				{
					return -1;
				}

				if(0 > mon_atkee_info_set(pstEnv, pstMon, pstAtkeeInfo, pstAtkProcess, pstCSAtkeeInfo))
				{
					return -1;
				}
			}
			break;
		default:
			return -1;
			break;
	}

	return 0;
}

static CSFIGHTOBJINFO* z_get_fight_obj_info(CSVALFIGHTCLT *pstFightClt, int fAtk)
{
	if(pstFightClt->AtkerID == pstFightClt->ObjInfo[0].MemID)
	{
		if(fAtk)
		{
			return &pstFightClt->ObjInfo[0];
		}
		else
		{
			return &pstFightClt->ObjInfo[1];
		}
	}
	else if (pstFightClt->AtkerID == pstFightClt->ObjInfo[1].MemID)
	{
		if(fAtk)
		{
			return &pstFightClt->ObjInfo[1];
		}
		else
		{
			return &pstFightClt->ObjInfo[0];
		}
	}

	return NULL;
}

int z_get_mon_attr_list(DYNATTRLIST *pstDyn, CSPLAYERATTRVAL *pstAttrVal)
{
	ATKCHG *pstAtkChg = &pstDyn->stAtkChg;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_WATER_ATK;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.WaterAtk = z_cacl_attr_atk(pstAtkChg, SKILL_ATTR_WATER);
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_FIRE_ATK;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.FireAtk = z_cacl_attr_atk(pstAtkChg, SKILL_ATTR_FIRE);
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_THUNDER_ATK;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.ThunderAtk = z_cacl_attr_atk(pstAtkChg, SKILL_ATTR_THUNDER);
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_LIGHT_ATK;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.LightAtk = z_cacl_attr_atk(pstAtkChg, SKILL_ATTR_LIGHT);
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_NIGHT_ATK;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.NightAtk = z_cacl_attr_atk(pstAtkChg, SKILL_ATTR_NIGHT);
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_WATER_DEF;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.WaterDef = z_cacl_attr_def(&pstDyn->stDefChg, SKILL_ATTR_WATER);
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_FIRE_DEF;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.FireDef = z_cacl_attr_def(&pstDyn->stDefChg, SKILL_ATTR_FIRE);
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_THUNDER_DEF;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.ThunderDef = z_cacl_attr_def(&pstDyn->stDefChg, SKILL_ATTR_THUNDER);
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_LIGHT_DEF;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.LightDef = z_cacl_attr_def(&pstDyn->stDefChg, SKILL_ATTR_LIGHT);
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_NIGHT_DEF;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.NightDef = z_cacl_attr_def(&pstDyn->stDefChg, SKILL_ATTR_NIGHT);
	pstAttrVal->AttrNum++;
	
	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_PHSIC_ATK;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.PhsicAtk = z_cacl_attr_atk(pstAtkChg, SKILL_ATTR_PHSIC);

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_ICE_ATK;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.IceAtk =z_cacl_attr_atk(pstAtkChg, SKILL_ATTR_ICE);
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_PHSIC_DEF;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.PhsicDef = z_cacl_attr_def(&pstDyn->stDefChg, SKILL_ATTR_PHSIC);
	pstAttrVal->AttrNum++;
	
	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_ICE_DEF;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.IceDef = z_cacl_attr_def(&pstDyn->stDefChg, SKILL_ATTR_ICE);
	pstAttrVal->AttrNum++;
	
	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_DEF_PHSIC_HURT;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.DefPhsicHurt = pstDyn->stAtkeeHurtChg.AtkeePhsicHurtAdd;
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_DEF_ICE_HURT;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.DefIceHurt = pstDyn->stAtkeeHurtChg.AtkeeIceHurtAdd;
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_PHSIC_HURT;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.DefPhsicHurt = pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtMul;
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_ICE_HURT;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.DefIceHurt = pstAtkChg->stAtkerHurtChg.AtkerIceHurtMul;
	pstAttrVal->AttrNum++;
	
	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_HEAVY;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.Heavy = pstDyn->stHitRateChg.nHeavyHitAdd;
	pstAttrVal->AttrNum++;	

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_HIT;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.Hit = pstDyn->stHitChg.unHitBaseAdd*(1+pstDyn->stHitChg.nHitChgMul/100.0) + pstDyn->stHitChg.iHitChgAdd;
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_HIT_RATE;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.HitRate = pstDyn->stHitRateChg.nAtkerHitRateAdd;
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_FLEE;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.Flee = pstDyn->stFleeChg.unFleeBaseAdd*(1+pstDyn->stFleeChg.nFleeChgMul/100.0) + pstDyn->stFleeChg.iFleeChgAdd;
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_BASE_ATTR_HEAVY;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.BaseAttrHeavy = pstDyn->stHitRateChg.iHeavyHit;
	pstAttrVal->AttrNum++;	

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_BASE_ATTR_DEFHEAVY;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.BaseAttrDefHeavy = pstDyn->stHitRateChg.iDefHeavyHit;
	pstAttrVal->AttrNum++;	

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_HEAVY_VAL;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.HeavyVal = pstDyn->stHitRateChg.nHeavyHitVal;
	pstAttrVal->AttrNum++;	

	return 0;
}

static int z_heavy_hit_check(SKILLDEF *pstSkillDef, DYNATTRLIST *pstAtkerDyn, DYNATTRLIST *pstAtkeeDyn)
{
	int iHeavyHit = 0;
		
	if (pstSkillDef->HeavyHitRate > 0)
	{
		iHeavyHit = pstSkillDef->HeavyHitRate;
		iHeavyHit += pstAtkerDyn->stHitRateChg.nHeavyHitAdd;

		//暴击力/ (暴击力+ 暴击抵抗)/3
		if(pstAtkerDyn->stHitRateChg.iHeavyHit != 0 && pstAtkeeDyn->stHitRateChg.iDefHeavyHit != 0)
		{
			iHeavyHit += pstAtkerDyn->stHitRateChg.iHeavyHit*100.0/(pstAtkerDyn->stHitRateChg.iHeavyHit +pstAtkeeDyn->stHitRateChg.iDefHeavyHit)/3;
		}		
		if ((int)(RAND1(10000)) >= iHeavyHit*100)
		{
			iHeavyHit = 0;
		}
		else
		{
			iHeavyHit = 1;
		}
	}

	return iHeavyHit;
}	

int val_print_data(ZONESVRENV* pstEnv, Player *pstPlayer, const char *pszMetaName, const void *pvData,
	                int iDataSize)
{
	TDRDATA stHost, stOut;
	char buff[1024];

	stHost.pszBuff = (char *)pvData;
	stHost.iBuff = iDataSize;

	stOut.pszBuff = (char *)buff;
	stOut.iBuff = sizeof(buff);

	tdr_sprintf(tdr_get_meta_by_name(pstEnv->pstNetHan->pstCSProtoLib, pszMetaName), &stOut, &stHost, 0);

	z_sys_strf( pstEnv, pstPlayer, SYS_TIP, buff);

	return 0;
}

static int val_fight_process(ZONESVRENV* pstEnv, Player *pstPlayer, 
						AtkeeInfo *pstAtkeeInfo, AtkProcess *pstAtkProcess, CSVALFIGHTCLT *pstFightClt)
{	
	CSPKG stPkg;
	CSVALSVR *pstValSvr = &stPkg.Body.ValSvr;
	CSVALFIGHTSVR* pstFightSvr = &pstValSvr->Data.FightInfo;
	DYNATTRLIST *pstAtkerDyn = &pstAtkProcess->stAtkerInfo.stDyn;
	DYNATTRLIST *pstAtkeeDyn = &pstAtkeeInfo->stDyn;
	SKILLDEF *pstSkillDef = pstAtkProcess->stAtkerInfo.pstSkillDef;
	int i, iHeavyHit, iHurt;
	int iTotalHeavyHit;

	memset(pstFightSvr, 0, sizeof(*pstFightSvr));
	pstValSvr->Type = VAL_SVR_FIGHT_INFO;

	//攻击次数
	pstFightSvr->AtkCount = pstFightClt->Count;

	if(pstFightClt->Count >100000)
	{
		pstFightClt->Count =100000;
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_VAL1);
	}
	
	iTotalHeavyHit = 0;
	for(i=0; i<pstFightClt->Count; i++)
	{
		iHeavyHit = 0;
		
		if (0 > z_miss_check(&pstAtkProcess->stAtkerInfo, pstAtkeeInfo))
		{
			pstFightSvr->Miss++;
			continue;
		}

		iHeavyHit = z_heavy_hit_check(pstSkillDef, pstAtkerDyn, pstAtkeeDyn);
		iHurt = cacl_hurt(pstEnv, pstAtkProcess, pstAtkeeInfo, pstSkillDef, iHeavyHit);

		//总伤害
		pstFightSvr->TotalHurt += iHurt;

		//命中次数
		pstFightSvr->Hit++;
		
		if(iHeavyHit)
		{
			iTotalHeavyHit += iHurt;

			//暴击次数
			pstFightSvr->HeavyHit++;

			//暴击最大伤害
			if(pstFightSvr->MaxHeavyHitVal < iHurt)
			{
				pstFightSvr->MaxHeavyHitVal = iHurt;
			}

			//暴击最小伤害
			if(pstFightSvr->MinHeavyHitVal == 0 || iHurt < pstFightSvr->MinHeavyHitVal)
			{
				pstFightSvr->MinHeavyHitVal = iHurt;
			}
		}
		else
		{
			//非暴击最大伤害
			if(pstFightSvr->MaxHitVal < iHurt)
			{
				pstFightSvr->MaxHitVal = iHurt;
			}

			//非暴击最小伤害
			if(pstFightSvr->MinHitVal == 0 || iHurt < pstFightSvr->MinHitVal)
			{
				pstFightSvr->MinHitVal = iHurt;
			}
		}
	}

	if(pstFightSvr->AtkCount > 0 && pstFightSvr->Hit > 0)
	{
		//平均伤害
		pstFightSvr->AvgHurt = pstFightSvr->TotalHurt /pstFightSvr->Hit;
	}	

	if(pstFightSvr->HeavyHit > 0)
	{
		//非暴击平均伤害
		pstFightSvr->AvgHeavyHitVal = iTotalHeavyHit /pstFightSvr->HeavyHit;
	}

	if(pstFightSvr->Hit - pstFightSvr->HeavyHit > 0)
	{
		//非暴击平均伤害
		pstFightSvr->AvgHitVal = (pstFightSvr->TotalHurt -iTotalHeavyHit) / (pstFightSvr->Hit - pstFightSvr->HeavyHit);
	}

	//val_print_data(pstEnv, pstPlayer, "CSValFightSvr", pstFightSvr,sizeof(*pstFightSvr));
		
	Z_CSHEAD_INIT(&stPkg.Head, VAL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int val_fight(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSVALFIGHTCLT *pstFightClt)
{
	AtkProcess stAtkProcess;
	AtkeeInfo stAtkeeInfo;
	CSFIGHTOBJINFO *pstCSAtkerInfo = NULL , *pstCSAtkeeInfo = NULL;

	memset(&stAtkProcess, 0, sizeof(stAtkProcess));
	memset(&stAtkeeInfo, 0, sizeof(stAtkeeInfo));

	pstCSAtkerInfo = z_get_fight_obj_info(pstFightClt, 1);
	if(NULL == pstCSAtkerInfo)
	{
		return -1;
	}

	pstCSAtkeeInfo = z_get_fight_obj_info(pstFightClt, 0);
	if(NULL == pstCSAtkeeInfo)
	{
		return -1;
	}

	//攻击者的战斗数值影响计算
	if(0> fight_atker_info(pstEnv, &stAtkProcess, pstFightClt, pstCSAtkerInfo))
	{
		return -1;
	}

	//被攻击者数值计算
	if(0 > fight_atkee_info(pstEnv, &stAtkeeInfo, &stAtkProcess, pstCSAtkeeInfo))
	{
		return -1;
	}

	//技能效果影响
	z_cacl_skill_attr(pstAppCtx, pstEnv, &stAtkProcess, &stAtkeeInfo);

	//开干
	return val_fight_process(pstEnv, pstPlayer, &stAtkeeInfo, &stAtkProcess, pstFightClt);
}

int val_watch(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSVALWATCHCLT *pstCSWatch)
{
	int iIndex;
	char cType;
	TMEMBLOCK *pstMemBlock;
	Player *psTmptPlayer;
	Monster *pstMon;
	MONSTERDEF *pstMonDef;
	CSPKG stPkg;
	CSVALSVR *pstValSvr = &stPkg.Body.ValSvr;
	CSVALWATCHINFO *pstWatchInfo = &pstValSvr->Data.WatchInfo;

	pstValSvr->Type = VAL_SVR_WATCH_INFO;
	memset(pstWatchInfo, 0, sizeof(*pstWatchInfo));
	
	iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstCSWatch->MemID, &cType);
	if (0 > iIndex)
	{
		return -1;
	}

	switch(cType)
	{
		case OBJ_PLAYER:
			{
				DYNATTRLIST *pstDyn;
				
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					return -1;
				}

				psTmptPlayer = (Player *)pstMemBlock->szData;
				if(psTmptPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
				{
					return -1;
				}
				
				pstDyn = &pstPlayer->stOnline.stDyn;

				pstWatchInfo->MemID = psTmptPlayer->iMemID;
				pstWatchInfo->AtkLevelMul = pstDyn->stAtkChg.iAtkLevelMul;
				pstWatchInfo->DefLevelMul = pstDyn->stDefChg.iDefLevelMul;
				STRNCPY(pstWatchInfo->Name, psTmptPlayer->stRoleData.RoleName, CCH(pstWatchInfo->Name));
				z_get_attr_list(pstAppCtx, pstEnv, psTmptPlayer, &pstWatchInfo->FightAttrVal, 0);
			}
			break;
		case OBJ_MONSTER:
			{
				DYNATTRLIST stDyn;
				ATKCHG *pstAtkChg;
						
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					return -1;
				}

				pstMon = (Monster *)pstMemBlock->szData;
				if(pstMon->cStat == MONSTER_DIE || pstMon->bAiStat == AI_STAT_HOME)
				{
					return -1;
				}

				pstMonDef = z_get_mon_def(pstEnv, pstMon);
				if (NULL == pstMonDef)
				{
					return -1;
				}

				z_get_mon_dyn(pstEnv, pstMon, pstMonDef, &stDyn);
				pstAtkChg = &stDyn.stAtkChg;
				z_cacl_mon_status_result(pstEnv, &stDyn, pstAtkChg, pstMon);
				
				pstWatchInfo->MemID = pstMon->iID;
				pstWatchInfo->AtkLevelMul = pstAtkChg->iAtkLevelMul;
				pstWatchInfo->DefLevelMul = stDyn.stDefChg.iDefLevelMul;
				STRNCPY(pstWatchInfo->Name, pstMonDef->Name, CCH(pstWatchInfo->Name));
				z_get_mon_attr_list(&stDyn, &pstWatchInfo->FightAttrVal);
			}
			break;
		default:
			return -1;
			break;
	}

	val_print_data(pstEnv, pstPlayer, "CSValWatchInfo", pstWatchInfo,sizeof(*pstWatchInfo));

	Z_CSHEAD_INIT(&stPkg.Head, VAL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int val_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
			TFRAMEHEAD	 *pstFrameHead, CSPKG *pstCsPkg)
{
	CSVALCLT *pstValClt = &pstCsPkg->Body.ValClt;
	UNUSED(pstFrameHead);


	if (RES_LOAD_BIN == pstEnv->pstConf->ResloadType)
	{
		if (0 == (pstPlayer->stRoleData.Perm & UIN_FLAG_GM_ADMIN) && 
			0 == (pstPlayer->UinFlag & UIN_FLAG_GM_ADMIN))
		{
			return -1;
		}
	}
	else if (RES_LOAD_DB== pstEnv->pstConf->ResloadType)
	{
		//限制普通用户
		if(pstEnv->cLimitPerm)
		{
			if (0 == (pstPlayer->stRoleData.Perm & UIN_FLAG_GM_ADMIN) && 
				0 == (pstPlayer->UinFlag & UIN_FLAG_GM_ADMIN))
			{
				return -1;
			}
		}
	}

	switch(pstValClt->Type)
	{
		case VAL_CLT_WATCH:
			return val_watch(pstAppCtx, pstEnv, pstPlayer, &pstValClt->Data.Watch);
			break;
		case VAL_CLT_FIGHT:
			return val_fight(pstAppCtx, pstEnv, pstPlayer, &pstValClt->Data.Fight);
			break;
		default:
			return -1;
			break;
	}

	return 0;
}