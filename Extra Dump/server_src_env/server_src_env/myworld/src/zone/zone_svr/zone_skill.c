/*
**  @file $RCSfile: zone_skill.c,v $
**  general description of this module
**  $Id: zone_skill.c,v 1.177 2013/10/31 07:34:03 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2013/10/31 07:34:03 $
**  @version $Revision: 1.177 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_skill.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "role_db_meta.h"
#include "zone_clt.h"
#include "zone_attr.h"
#include "zone_package.h"
#include "zone_act_status.h"
#include "zone_player.h"
#include "zone_task.h"
#include "zone_move.h"
#include "zone_map.h"
#include "zone_status.h"
#include "zone_pet.h"
#include "zone_oplog.h"
#include "zone_arm.h"
#include "zone_err.h"
 
#define LOW_FIT_SKILL_LEVEL 6
#define MID_FIT_SKILL_LEVEL 10
#define MAX_FITTINGS_LEVEL 3

#define FITTINGS_UPDATE_MAX_PEARL_NUM 5

SKILLDEF *z_find_skill_zoneskill(ZONESVRENV* pstEnv, ZoneSkill *pstZoneSkill)
{
	SKILLDEF *pstSkillDef;
	SKILLDEF stSkillDef;
	int iSkillIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	CSSKILLDATA *pstSkillData = &pstZoneSkill->stSkillData;
	
	if (pstZoneSkill->DefIdx >= 0 && pstZoneSkill->DefIdx < pstObj->iSkillDef)
	{
		pstSkillDef = (SKILLDEF *)pstObj->sSkillDef[pstZoneSkill->DefIdx];
		if (pstSkillDef->SkillID == pstSkillData->SkillID && pstSkillDef->Level == pstSkillData->SkillLevel)
		{
			return pstSkillDef;
		}
	}

	stSkillDef.SkillID = pstSkillData->SkillID;
	stSkillDef.Level = pstSkillData->SkillLevel;
	iSkillIndex = bsearch_int(&stSkillDef, pstObj->sSkillDef, pstObj->iSkillDef, SKILL_DATA, &iEqu);
	if (iEqu)
	{
		pstZoneSkill->DefIdx = iSkillIndex;
		return (SKILLDEF *)&pstObj->sSkillDef[iSkillIndex];
	}

	return NULL;
}

SKILLDEF *z_find_skill_idx(ZONESVRENV* pstEnv, int iSkillID, int iSkillLevel, int *piIdx)
{
	SKILLDEF *pstSkillDef;
	SKILLDEF stSkillDef;
	int iSkillIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	if (piIdx && *piIdx >= 0 && *piIdx < pstObj->iSkillDef)
	{
		pstSkillDef = (SKILLDEF *)pstObj->sSkillDef[*piIdx];
		if (pstSkillDef->SkillID == iSkillID && pstSkillDef->Level == iSkillLevel)
		{
			return pstSkillDef;
		}
	}

	stSkillDef.SkillID = iSkillID;
	stSkillDef.Level = iSkillLevel;
	iSkillIndex = bsearch_int(&stSkillDef, pstObj->sSkillDef, pstObj->iSkillDef, SKILL_DATA, &iEqu);
	if (iEqu)
	{
		if (piIdx)
		{
			*piIdx = iSkillIndex;
		}
		
		return (SKILLDEF *)&pstObj->sSkillDef[iSkillIndex];
	}

	return NULL;
}

int z_check_addattr_skillid(ADDATTRDEF *pstAddAttrDef, unsigned short unSkillID)
{
	int i =0;

	for (i = 0; i< MAX_ADDATTR_SKILL; i++)
	{
		if (pstAddAttrDef->EffectSkillID[i] == 0)
		{
			break;
		}

		if (pstAddAttrDef->EffectSkillID[i] == unSkillID)
		{
			return 1;
		}
	}

	return 0;
}

int get_add_skill_level(ZONESVRENV* pstEnv, unsigned short unSkillID, unsigned short unSkillLevel, short nLevel)
{
	int iAddLevel = 0;
	SKILLDEF *pstSkillDef = NULL;

	for (iAddLevel = nLevel; iAddLevel >0; iAddLevel--)
	{
		pstSkillDef = z_find_skill(pstEnv, unSkillID, unSkillLevel + iAddLevel, NULL);
		if (pstSkillDef)
		{
			return iAddLevel;
		}
	}

	return 0;
}


int z_update_one_skill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, ZoneSkill *pstZoneSkill)
{
	int iIdx = -1, iSkillLevel = 0;
	SKILLDEF *pstSkillDef = NULL;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	CSSKILLDATA *pstSkillData = &pstZoneSkill->stSkillData;
	int i;

	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstRolePet);	//add by paraunused.pl

	// 初始化下天赋对技能的影响
	memset(&pstZoneSkill->stSkillMul,0,sizeof(pstZoneSkill->stSkillMul));

	//状态改变技能等级优先
	for(i=0;i<MAX_STATUS_CHG_SKILL_LEVEL;i++)
	{
		if(!pstDyn->astStatusChgSkill[i].unSkillID)
		{
			continue;
		}
		
		if(pstDyn->astStatusChgSkill[i].unSkillID == pstZoneSkill->stSkillData.SkillID)
		{
			iSkillLevel = pstDyn->astStatusChgSkill[i].ucLevel;
			pstSkillDef = z_find_skill_idx(pstEnv, pstZoneSkill->stSkillData.SkillID, iSkillLevel, NULL);
			break;
		}
	}

	if(NULL == pstSkillDef)
	{
		//装备影响
		iIdx = z_find_arm_dyn_skill(pstPlayer, pstSkillData->SkillID);
		if ( iIdx >= 0)
		{
			iSkillLevel = get_add_skill_level(pstEnv, pstSkillData->SkillID, pstSkillData->SkillLevel, pstDyn->astArmSkill[iIdx].ucLevel);
			pstSkillDef = z_find_skill_idx(pstEnv, pstSkillData->SkillID, pstSkillData->SkillLevel + iSkillLevel, NULL);
		}
		else
		{
			pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill);
		}
	}
	
	if (NULL == pstSkillDef)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player skill %d level %d no def",
					pstPlayer->stRoleData.Uin, pstZoneSkill->stSkillData.SkillID,  pstZoneSkill->stSkillData.SkillLevel);
		return -1;
	}

	pstSkillData->CollectNumMax = pstSkillDef->CollectNumMax;

	// 考虑配件的影响
	// z_skill_fittings_attr(pstEnv, pstPlayer, pstSkillData, pstSkillDef);

	//考虑素质对技能的影响
	z_skill_diathesis_attr(pstEnv, pstPlayer, pstZoneSkill, pstSkillDef);

	return 0;
}

int z_update_all_skill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	CSPKG stPkg;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;
	CSSKILLINFO *pstSkillInfo = &stPkg.Body.SkillInfo;
	CSSKILLLIST *pstSkillChg = &pstSkillInfo->Data.SkillChg;

	for (i=0; i<pstZoneSkillList->nSkillNum; i++)
	{
		z_update_one_skill(pstAppCtx, pstEnv, pstPlayer, NULL, pstZoneSkillList->astZoneSkills+i);
	}

	pstSkillInfo->Type = SKILL_INFO_CHANGE;
	pstSkillChg->SkillNum = pstZoneSkillList->nSkillNum;

	for (i=0; i<pstZoneSkillList->nSkillNum; i++)
	{
		memcpy(&pstSkillChg->Skills[i], &pstZoneSkillList->astZoneSkills[i].stSkillData,
				sizeof(pstSkillChg->Skills[i]));
	}

	Z_CSHEAD_INIT(&stPkg.Head, SKILL_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int z_diathesis_chg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, DIATHESISDATA *pstDiathesisData)
{
	CSPKG stPkg;
	CSDIATHESISINFO *pstDiaThesisInfo = &stPkg.Body.DiathesisInfo;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	int iOldAddShiNum = pstDyn->stMidAttrList.bShiListAdd;
	
	z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);
	z_update_all_skill(pstAppCtx, pstEnv, pstPlayer);

	pstDiaThesisInfo->Type = DIATHESIS_INFO_CHG;
	pstDiaThesisInfo->DiathesisInfoData.DiathesisChg.DiathesisNum = 1;
	pstDiaThesisInfo->DiathesisInfoData.DiathesisChg.DiathesisList[0] = *pstDiathesisData;
	pstDiaThesisInfo->DiathesisInfoData.DiathesisChg.LearnGeniusPoint = pstPlayer->stRoleData.DiathesisInfo.LearnGeniusPoint;
	pstDiaThesisInfo->DiathesisInfoData.DiathesisChg.UsedGeniusPoint = pstPlayer->stRoleData.DiathesisInfo.UsedGeniusPoint;

	Z_CSHEAD_INIT(&stPkg.Head, DIATHESIS_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	if (iOldAddShiNum < pstDyn->stMidAttrList.bShiListAdd)
	{
		package_enlarge(pstEnv->pstAppCtx, pstEnv, pstPlayer, LIST_TYPE_SHI, pstDyn->stMidAttrList.bShiListAdd-iOldAddShiNum);
	}

	return 0;
}

int z_genius_chg(ZONESVRENV* pstEnv, Player *pstPlayer,DIATHESISINFO *pstDiathesisInfo)
{
	CSPKG stPkg;
	CSDIATHESISINFO *pstDiaThesisInfo = &stPkg.Body.DiathesisInfo;

	pstDiaThesisInfo->Type = GENIUS_INFO_CHG;
	pstDiaThesisInfo->DiathesisInfoData.GeniusInfo.LearnGeniusPoint = pstDiathesisInfo->LearnGeniusPoint;
	pstDiaThesisInfo->DiathesisInfoData.GeniusInfo.UsedGeniusPoint = pstDiathesisInfo->UsedGeniusPoint;
	
	Z_CSHEAD_INIT(&stPkg.Head, DIATHESIS_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int z_diathesis_all_chg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSDIATHESISINFO *pstDiaThesisInfo = &stPkg.Body.DiathesisInfo;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	int iOldAddShiNum = pstDyn->stMidAttrList.bShiListAdd;
	
	z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);
	z_update_all_skill(pstAppCtx, pstEnv, pstPlayer);

	pstDiaThesisInfo->Type = DIATHESIS_ALL_DATA;
	memcpy(&pstDiaThesisInfo->DiathesisInfoData.DiathesisInfo, &pstPlayer->stRoleData.DiathesisInfo, 
		sizeof(DIATHESISINFO));

	Z_CSHEAD_INIT(&stPkg.Head, DIATHESIS_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	if (iOldAddShiNum < pstDyn->stMidAttrList.bShiListAdd)
	{
		package_enlarge(pstEnv->pstAppCtx, pstEnv, pstPlayer, LIST_TYPE_SHI, pstDyn->stMidAttrList.bShiListAdd-iOldAddShiNum);
	}

	return 0;
}

/*
int z_fittings_all(ZONESVRENV * pstEnv, Player * pstPlayer)
{
	CSPKG stPkg;
	CSFITTINGSINFO *pstFittingsInfo = &stPkg.Body.FittingsInfo;
	FITTINGSINFO *pstFitInfo = &pstPlayer->stRoleData.FittingsInfo;

	pstFittingsInfo->Type = FITTINGS_INFO_CHG;
	memcpy(&pstFittingsInfo->FittingsInfoData.FittingsChg, pstFitInfo, sizeof(*pstFitInfo));

	Z_CSHEAD_INIT(&stPkg.Head, FITTINGS_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}
*/

int z_skill_online(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	ZoneSkill *pstZoneSkill;
	int iTmp;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;
	SKILLINFO *pstSkillInfo = &pstPlayer->stRoleData.SkillInfo;

	TV_CLONE(pstZoneSkillList->stCommCoolEnd, pstAppCtx->stCurr);
	pstZoneSkillList->nSkillNum = pstSkillInfo->SkillNum;
	pstZoneSkillList->nSkillPos = -1;
	for (i=0; i<(int)pstSkillInfo->SkillNum; i++)
	{
		pstZoneSkill = &pstZoneSkillList->astZoneSkills[i];
		pstZoneSkill->stSkillData.SkillID = pstSkillInfo->SkillList[i].SkillID;
		pstZoneSkill->stSkillData.SkillLevel = pstSkillInfo->SkillList[i].SkillLevel;
		pstZoneSkill->stSkillData.CollectNum = pstSkillInfo->SkillList[i].CollectNum;
		pstZoneSkill->stSkillData.CoolLeft = pstSkillInfo->SkillList[i].CoolLeft;
		pstZoneSkill->stSkillData.RuneSlot = pstSkillInfo->SkillList[i].RuneSlot;
		pstZoneSkill->stSkillData.RuneSlotEnabled = pstSkillInfo->SkillList[i].RuneSlotEnabled;
		pstZoneSkill->stSkillData.RuneItemID = pstSkillInfo->SkillList[i].RuneItemID;
		pstZoneSkill->stSkillData.RuneNum = pstSkillInfo->SkillList[i].RuneNum;
		
		memcpy(pstZoneSkill->stSkillData.RuneInfo,pstSkillInfo->SkillList[i].RuneInfo,sizeof(pstZoneSkill->stSkillData.RuneInfo));
		
		pstZoneSkill->DefIdx = -1;
		//z_skill_map_fittings(pstEnv, pstPlayer, pstZoneSkill, &pstSkillInfo->SkillList[i]);

		z_update_one_skill(pstAppCtx, pstEnv, pstPlayer, NULL, pstZoneSkill);

		iTmp = pstZoneSkill->stSkillData.CoolLeft*100.0;
		TV_ADD_MS(pstZoneSkill->stCoolEnd, pstAppCtx->stCurr, iTmp);	
	}
	
	return 0;
}

int z_skill_roledata(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i/*, j*/;
	struct timeval stTmp;
	long long  llMs;
	ZoneSkill *pstZoneSkill;
	SKILLDATA *pstSkillData;
	ZoneSkillList *pstSkillList = &pstPlayer->stOnline.stSkillList;
	SKILLINFO *pstSkillInfo = &pstPlayer->stRoleData.SkillInfo;

	UNUSED(pstEnv);	//add by paraunused.pl
	memset(&stTmp,0,sizeof(stTmp));

	pstSkillInfo->SkillNum = pstSkillList->nSkillNum;
	for (i=0; i<pstSkillList->nSkillNum;i++)
	{
		pstZoneSkill = &pstSkillList->astZoneSkills[i];
		pstSkillData = &pstSkillInfo->SkillList[i];
		
		pstSkillData->SkillID = pstZoneSkill->stSkillData.SkillID;
		pstSkillData->SkillLevel = pstZoneSkill->stSkillData.SkillLevel;
		pstSkillData->CollectNum = pstZoneSkill->stSkillData.CollectNum;
		pstSkillData->RuneSlotEnabled = pstZoneSkill->stSkillData.RuneSlotEnabled;
		pstSkillData->RuneSlot = pstZoneSkill->stSkillData.RuneSlot;
		pstSkillData->RuneItemID = pstZoneSkill->stSkillData.RuneItemID;

		pstSkillData->RuneNum = pstZoneSkill->stSkillData.RuneNum;
		
		memcpy(pstSkillData->RuneInfo,pstZoneSkill->stSkillData.RuneInfo,sizeof(pstSkillData->RuneInfo));
		
		TV_DIFF(stTmp, pstZoneSkill->stCoolEnd, pstAppCtx->stCurr);
		TV_TO_MS(llMs, stTmp);
		if (llMs > 0)
		{
			pstSkillData->CoolLeft = llMs/100;
		}
		else
		{
			pstSkillData->CoolLeft = 0;
		}

		/*
		pstSkillData->FittingsMap.FittingsMapRowNum = pstZoneSkill->stSkillData.FittingsRowNum;
		for (j=0; j<pstSkillData->FittingsMap.FittingsMapRowNum; j++)
		{
			memcpy(&pstSkillData->FittingsMap.FittingsMapList[j],  &pstZoneSkill->stSkillData.CSFittingsRowList[j].FittingsMap,
						sizeof(pstSkillData->FittingsMap.FittingsMapList[j]));
		}
		*/
	}
	
	return 0;
}

//获取玩家普通攻击的定义
SKILLDEF *z_get_player_normalskilldef(ZONESVRENV * pstEnv,Player *pstPlayer)
{
	int i = 0;
	SKILLDEF *pstSkillDef = NULL;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;
	
	for (i=0; i<pstZoneSkillList->nSkillNum; i++)
	{
		pstSkillDef = z_find_skill_zoneskill(pstEnv, &pstZoneSkillList->astZoneSkills[i]);
		if( pstSkillDef && pstSkillDef->SkillType == SKILL_TYPE_NORMAL)
		{
			return pstSkillDef;
		}
	}
	return NULL;
}


ZoneSkill *z_get_player_zoneskill(Player *pstPlayer, unsigned short unSkillID)
{
	int i;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;

	if (pstZoneSkillList->nSkillPos >= 0 && pstZoneSkillList->nSkillPos < pstZoneSkillList->nSkillNum &&
		 pstZoneSkillList->astZoneSkills[pstZoneSkillList->nSkillPos].stSkillData.SkillID == unSkillID)
	{
		return &pstZoneSkillList->astZoneSkills[pstZoneSkillList->nSkillPos];
	}

	for (i=0; i<pstZoneSkillList->nSkillNum; i++)
	{
		if (pstZoneSkillList->astZoneSkills[i].stSkillData.SkillID == unSkillID)
		{
			pstZoneSkillList->nSkillPos = i;
			return &pstZoneSkillList->astZoneSkills[i];
		}
	}

	pstZoneSkillList->nSkillPos = -1;
	return NULL;
}

DIATHESISDATA *z_get_player_diathesis(Player *pstPlayer, unsigned short unDiathesisID)
{
	int i;
	DIATHESISDATA *pstDiathesisData;
	DIATHESISINFO *pstDiathesisInfo = &pstPlayer->stRoleData.DiathesisInfo;

	for (i=0; i<(int)pstDiathesisInfo->DiathesisNum; i++)
	{
		pstDiathesisData = &pstDiathesisInfo->DiathesisList[i];
		if (pstDiathesisData->DiathesisID == unDiathesisID)
		{
			return pstDiathesisData;
		}
	}

	return NULL;
}

DIATHESISDEF *z_find_diathesis(ZONESVRENV* pstEnv, unsigned short unDiathesisID, unsigned char bLevel)
{
	DIATHESISDEF stDiathesis;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDiathesis.DiathesisID = unDiathesisID;
	stDiathesis.Level = bLevel;

	iIndex = bsearch_int(&stDiathesis, pstObj->sDiathesisDef, pstObj->iDiathesisDef, DIATHESIS_DATA, &iEqu);
	if (iEqu)
	{
		return (DIATHESISDEF *)pstObj->sDiathesisDef[iIndex];
	}

	return NULL;
}

/*
FITTINGSDEF *z_find_fittings(ZONESVRENV* pstEnv, unsigned short bFittingsID, unsigned char bLevel, unsigned char bGroup,unsigned short *pucIdx)
{
	FITTINGSDEF stFittings;
	int iIndex;
	int iEqu;
	FITTINGSDEF *pstFittingsDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	if (pucIdx && *pucIdx < pstObj->iFittingsDef)
	{
		pstFittingsDef = (FITTINGSDEF *)pstObj->sFittingsDef[*pucIdx];
		if (pstFittingsDef->FittingsID == bFittingsID && pstFittingsDef->Level == bLevel && pstFittingsDef->GroupID == bGroup)
		{
			return pstFittingsDef;
		}
	}

	stFittings.FittingsID = bFittingsID;
	stFittings.Level = bLevel;
	stFittings.GroupID = bGroup;

	iIndex = bsearch_int(&stFittings, pstObj->sFittingsDef, pstObj->iFittingsDef, FITTINGS_DATA, &iEqu);
	if (iEqu)
	{
		if (pucIdx)
		{
			*pucIdx = iIndex;
		}
		
		return (FITTINGSDEF *)pstObj->sFittingsDef[iIndex];
	}

	return NULL;
}
*/

/*
int check_skill_fit_level(int iSkillLevel, int iFitLevel)
{
	if (iFitLevel == 1)
	{
		
	}
	else if(iFitLevel == 2)
	{
		if (iSkillLevel < LOW_FIT_SKILL_LEVEL )
		{
			return -1;
		}
	}
	else if(iFitLevel == 3)
	{
		if (iSkillLevel <  MID_FIT_SKILL_LEVEL)
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
*/

SKILLRESULT *z_get_skill_result( SKILLDEF *pstSkillDef,int iResultID)
{
	int i=0;
	
	for( i=0;i<MAX_SKILL_RESULT;i++)
	{
		if(pstSkillDef->SkillResult[i].SkillResultID == iResultID)
		{
			return &pstSkillDef->SkillResult[i];
		}
	}
	
	return NULL;
}

SKILLDEF* z_find_skill(ZONESVRENV* pstEnv, unsigned short unSkillID, unsigned short unSkillLevel, int *piIdx)
{
	SKILLDEF *pstSkillDef = NULL;
	SKILLDEF  stSkill;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	if(unSkillID == 0)
	{
		return NULL;
	}
	
	if (piIdx && *piIdx >= 0 && *piIdx < pstObj->iSkillDef)
	{
		pstSkillDef = (SKILLDEF *)pstObj->sSkillDef[*piIdx];
		if (pstSkillDef->SkillID == unSkillID && pstSkillDef->Level == unSkillLevel)
		{
			return pstSkillDef;
		}
	}

	stSkill.SkillID = unSkillID;
	stSkill.Level = unSkillLevel;

	iIndex = bsearch_int(&stSkill, pstObj->sSkillDef, pstObj->iSkillDef, SKILL_DATA, &iEqu);
	if (iEqu)
	{
		if (piIdx)
		{
			*piIdx = iIndex;
		}
		
		return (SKILLDEF *)pstObj->sSkillDef[iIndex];
	}
	
	return NULL;
}


int z_skill_chg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ZoneSkill *pstZoneSkill)
{
	CSPKG stPkg;
	CSSKILLLIST *pstSkillChg;
	CSSKILLINFO *pstSkillInfo = &stPkg.Body.SkillInfo;
	

	pstSkillInfo->Type = SKILL_INFO_CHANGE;
	pstSkillChg = &pstSkillInfo->Data.SkillChg;

	pstSkillChg->SkillNum = 1;
		
	memcpy(&pstSkillChg->Skills[0], &pstZoneSkill->stSkillData, sizeof(pstSkillChg->Skills[0]));
	
	if (pstZoneSkill->stCoolEnd.tv_sec > pstAppCtx->stCurr.tv_sec)
	{
		struct timeval stTmp;
		long long  llMs=0;
		SKILLDEF *pstSkillDef = NULL;
		
		memset(&stTmp,0,sizeof(stTmp));
		TV_DIFF(stTmp, pstZoneSkill->stCoolEnd, pstAppCtx->stCurr);
		TV_TO_MS(llMs, stTmp);
		
		pstSkillChg->Skills[0].CoolLeft = llMs/100;

		pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill);

		if(pstSkillDef && 0 == pstSkillDef->SelfCool)
		{
			pstSkillChg->Skills[0].CoolLeft = 0;
			pstZoneSkill->stCoolEnd.tv_sec = pstAppCtx->stCurr.tv_sec;
		}
	}

	Z_CSHEAD_INIT(&stPkg.Head, SKILL_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
	UNUSED(pstAppCtx);
}

int skill_action(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iSkillID, int iLevel, int iType)
{
	CSPKG stPkgRes;
	CSACTIONRES *pstActionRes = &stPkgRes.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_SKILL_LEARN;

	pstActionRes->Data.SkillLearn.ID = iSkillID;
	pstActionRes->Data.SkillLearn.Level = iLevel;
	pstActionRes->Data.SkillLearn.Type = iType;


	Z_CSHEAD_INIT(&stPkgRes.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkgRes, 0);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}
int z_add_skill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, unsigned short unSkillID, unsigned short unLevel, int iNotify)
{
	ZoneSkill *pstZoneSkill;
	SKILLDEF* pstSkillDef;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;

	if (pstZoneSkillList->nSkillNum >= MAX_ROLE_SKILL)
	{
		return -1;
	}

	pstSkillDef = z_find_skill(pstEnv, unSkillID, unLevel, NULL);
	if (NULL == pstSkillDef)
	{
		return -1;
	}

	if (z_get_player_zoneskill(pstPlayer, unSkillID))
	{
		return -1;
	}

	// 如果 转职了,升级给技能,如果职业不相符,就不会给
	if ( (CAREER_NON != pstSkillDef->Learn.Career && (pstPlayer->stRoleData.Career&0x0F) != (pstSkillDef->Learn.Career&0x0F)) ||
		 (CAREER_NON != (pstSkillDef->Learn.Career&0xF0) && (pstPlayer->stRoleData.Career&0xF0) != (pstSkillDef->Learn.Career&0xF0)) )
	{
		return -1;
	}

	pstZoneSkill = &pstZoneSkillList->astZoneSkills[pstZoneSkillList->nSkillNum];
	memset(pstZoneSkill, 0, sizeof(*pstZoneSkill));
	pstZoneSkill->DefIdx = -1;
	TV_CLONE(pstZoneSkill->stCoolEnd, pstAppCtx->stCurr);
	pstZoneSkill->stSkillData.SkillID = unSkillID;
	pstZoneSkill->stSkillData.SkillLevel = unLevel;
	//pstZoneSkill->stSkillData.FittingsRowNum = pstSkillDef->FittingsGridRowNum;
	pstZoneSkill->stSkillData.CoolLeft = 0;
	pstZoneSkill->stSkillData.RuneItemID = 0;
	pstZoneSkill->stSkillData.RuneSlot = RUNE_EMPTY;
	//if(pstPlayer->stRoleData.Level >= MIN_RUNE_LEVEL)
	//{
	//	pstZoneSkill->stSkillData.RuneSlotEnabled = 1;
	//}

	pstZoneSkill->stSkillData.RuneNum = RUNE_TYPE_NUM;	
	pstZoneSkill->stSkillData.RuneInfo[RUNE_EMPTY].SkillID = unSkillID;
	

	// 不做默认配件的逻辑
		
	z_update_one_skill(pstAppCtx, pstEnv, pstPlayer, pstRolePet, pstZoneSkill);
	pstZoneSkillList->nSkillNum++;

	if (iNotify)
	{
		z_skill_chg(pstAppCtx, pstEnv, pstPlayer, pstZoneSkill);

		if (0 == pstSkillDef->BindSkill)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SKILL201, pstSkillDef->Name);
		}
	}

	task_player_trace(pstEnv, pstPlayer, TASK_OBJ_PLAYER_SKILL);

	//成长之路
	{
		ACHIEVEMENTCOND stCond;

		memset(&stCond,0,sizeof(stCond));
		
		stCond.Type = COND_SKILL_LEARN;
		stCond.Val1 = pstSkillDef->Learn.LevelMin;

		z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
	}
	
	return 0;
}

/*
int z_vigor_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, DIATHESISDEF *pstDiathesisDef)
{
	int i;
	CSPKG stPkg;
	CSPLAYERATTRVAL *pstAttrVal = &stPkg.Body.AttrChg;
	SKILLRESULT *pstResult;	
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;

	for (i=0; i<(int)(sizeof(pstDiathesisDef->DiathesisResult)/sizeof(pstDiathesisDef->DiathesisResult[0])); i++)
	{
		pstResult = &pstDiathesisDef->DiathesisResult[i];
		if (pstResult->SkillResultID <= 0)
		{
			break;
		}
		
		if (pstResult->SkillResultID == RESULT_VIGOR)
		{
			pstAttrVal->AttrList[0].AttrID = ATTR_ID_VIGOR;
			pstAttrVal->AttrList[0].Value.Vigor = pstPlayer->stRoleData.RoleDetail.VigorCurr = pstDyn->stMidAttrList.unVigorMax;
			pstAttrVal->AttrNum = 1;
			Z_CSHEAD_INIT(&stPkg.Head, ATTR_CHG);
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

			pstPlayer->stRoleData.MiscInfo.VigorTime = pstEnv->pstAppCtx->stCurr.tv_sec/(3600*24);
			return 0;
		}
	}
	return 0;
}
*/

SKILLRESULT *z_diathesis_def_get_result(DIATHESISDEF *pstDiathesisDef, int iResultID)
{
	int i = 0;

	for (i=0; i<MAX_DIATHESIS_RESULT; i++) 
	{
		if (0 == pstDiathesisDef->DiathesisResult[i].SkillResultID)
		{
			break;
		}

		if (iResultID == pstDiathesisDef->DiathesisResult[i].SkillResultID)
		{
			return pstDiathesisDef->DiathesisResult+i;
		}
	}

	return NULL;
}

int z_get_diathesis_result(ZONESVRENV* pstEnv, Player *pstPlayer, int iResultID, SKILLRESULT *pstResult)
{
	int i,j;
	DIATHESISDEF *pstDiathesisDef;
	DIATHESISDATA *pstDiathesisData;
	DIATHESISINFO *pstDiathesisInfo = &pstPlayer->stRoleData.DiathesisInfo;

	memset(pstResult, 0, sizeof(*pstResult));
	for (i=0; i<(int)pstDiathesisInfo->DiathesisNum; i++)
	{
		pstDiathesisData = &pstDiathesisInfo->DiathesisList[i];
		pstDiathesisDef = z_find_diathesis(pstEnv, pstDiathesisData->DiathesisID, pstDiathesisData->DiathesisLevel);
		if(pstDiathesisDef)
		{
			for (j=0; j<(int)(sizeof(pstDiathesisDef->DiathesisResult)/sizeof(pstDiathesisDef->DiathesisResult[0])); j++)
			{
				if (pstDiathesisDef->DiathesisResult[j].SkillResultID == 0)
				{
					break;
				}
				if(iResultID == pstDiathesisDef->DiathesisResult[j].SkillResultID)
				{
					pstResult->SkillResultID = iResultID;
					pstResult->SkillResultVal1 += pstDiathesisDef->DiathesisResult[j].SkillResultVal1;
					pstResult->SkillResultVal2 += pstDiathesisDef->DiathesisResult[j].SkillResultVal2;
					pstResult->SkillResultVal3 += pstDiathesisDef->DiathesisResult[j].SkillResultVal3;
					pstResult->SkillResultVal4 += pstDiathesisDef->DiathesisResult[j].SkillResultVal4;
				}
			}
		}
	}

	return 0;
}
int z_get_diathesis_result_by_value1(ZONESVRENV* pstEnv, Player *pstPlayer, int iResultID,int iVal1, SKILLRESULT *pstResult)
{
	int i,j;
	DIATHESISDEF *pstDiathesisDef;
	DIATHESISDATA *pstDiathesisData;
	DIATHESISINFO *pstDiathesisInfo = &pstPlayer->stRoleData.DiathesisInfo;

	memset(pstResult, 0, sizeof(*pstResult));
	for (i=0; i<(int)pstDiathesisInfo->DiathesisNum; i++)
	{
		pstDiathesisData = &pstDiathesisInfo->DiathesisList[i];
		pstDiathesisDef = z_find_diathesis(pstEnv, pstDiathesisData->DiathesisID, pstDiathesisData->DiathesisLevel);
		if(pstDiathesisDef)
		{
			for (j=0; j<(int)(sizeof(pstDiathesisDef->DiathesisResult)/sizeof(pstDiathesisDef->DiathesisResult[0])); j++)
			{
				if (pstDiathesisDef->DiathesisResult[j].SkillResultID == 0)
				{
					break;
				}
				if(iResultID == pstDiathesisDef->DiathesisResult[j].SkillResultID && 
					pstDiathesisDef->DiathesisResult[j].SkillResultVal1 == iVal1)
				{
					pstResult->SkillResultID = iResultID;
					pstResult->SkillResultVal1 = pstDiathesisDef->DiathesisResult[j].SkillResultVal1;
					pstResult->SkillResultVal2 = pstDiathesisDef->DiathesisResult[j].SkillResultVal2;
					pstResult->SkillResultVal3 = pstDiathesisDef->DiathesisResult[j].SkillResultVal3;
					pstResult->SkillResultVal4 = pstDiathesisDef->DiathesisResult[j].SkillResultVal4;
					return 0;
				}
			}
		}
	}

	return 0;
}


int z_add_diathesis(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, unsigned short iDiathesisID, unsigned short unLevel)
{
	DIATHESISDEF *pstDiathesisDef;
	DIATHESISDATA *pstDiathesisData;
	DIATHESISINFO *pstDiathesisInfo = &pstPlayer->stRoleData.DiathesisInfo;

	pstDiathesisDef = z_find_diathesis(pstEnv, iDiathesisID, unLevel);
	if (NULL == pstDiathesisDef)
	{
		return -1;
	}

	pstDiathesisData = z_get_player_diathesis(pstPlayer, iDiathesisID);
	if (NULL == pstDiathesisData)
	{
		if (pstDiathesisInfo->DiathesisNum >= sizeof(pstDiathesisInfo->DiathesisList)/sizeof(pstDiathesisInfo->DiathesisList[0]))
		{
			return -1;
		}

		pstDiathesisData = &pstDiathesisInfo->DiathesisList[pstDiathesisInfo->DiathesisNum];
		pstDiathesisData->DiathesisID = iDiathesisID;
		pstDiathesisData->DiathesisLevel = unLevel;
		if( pstDiathesisDef->UseGeniusPoint >0  || pstDiathesisDef->PreUsedGeniusPoint >0 )
		{
			pstDiathesisData->GeniusFlag = 1;
		}
		pstDiathesisInfo->DiathesisNum++;
	}
	else
	{
		return -1;
	}
	
	z_diathesis_chg(pstAppCtx, pstEnv, pstPlayer, pstDiathesisData);


	z_role_diathesis_oplog(pstEnv, pstPlayer, OP_LEARN_DIATHESIS, iDiathesisID, unLevel, 0);
	
	return 0;
}

int z_del_skill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, unsigned short unSkillID, int iNotify)
{	
	int i;
	ZoneSkill *pstZoneSkill;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;	
	UNUSED(pstAppCtx);

	for (i=0; i<pstZoneSkillList->nSkillNum; i++)
	{
		pstZoneSkill = &pstZoneSkillList->astZoneSkills[i];
		if (pstZoneSkill->stSkillData.SkillID == unSkillID)
		{
			break;
		}
	}

	if (i == pstZoneSkillList->nSkillNum)
	{
		return -1;
	}

	pstZoneSkillList->nSkillNum--;
	if (i<pstZoneSkillList->nSkillNum)
	{
		memcpy(pstZoneSkill, pstZoneSkillList->astZoneSkills+pstZoneSkillList->nSkillNum, sizeof(*pstZoneSkill));
	}

	if (iNotify)
	{
		CSPKG stPkg;
		CSSKILLINFO *pstSkillInfo = &stPkg.Body.SkillInfo;

		pstSkillInfo->Type = SKILL_INFO_DEL;
		pstSkillInfo->Data.SkillDelID = unSkillID;
		
		Z_CSHEAD_INIT(&stPkg.Head, SKILL_INFO);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	
	return 0;
}

int skill_check_by_book(ZONESVRENV* pstEnv, Player* pstPlayer, SKILLLEARN *pstSkillLearn)
{
	ROLEDATA *pstRoleData = &pstPlayer->stRoleData;
	DIATHESISDATA *pstDiathesis;
	int iRet = 0;

	if ( (CAREER_NON != pstSkillLearn->Career && (pstRoleData->Career&0x0F) != (pstSkillLearn->Career&0x0F)) ||
		 (CAREER_NON != (pstSkillLearn->Career&0xF0) && (pstRoleData->Career&0xF0) != (pstSkillLearn->Career&0xF0)) )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL202);
		return -1;
	}

	if (pstRoleData->Level < pstSkillLearn->LevelMin)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL203, pstSkillLearn->LevelMin);
		return -1;
	}

	if (pstSkillLearn->Mode == SKILL_LEARN_UI)
	{
		return -1;
	}

	if (pstSkillLearn->PreDiathesisID != 0)
	{
		pstDiathesis = z_get_player_diathesis(pstPlayer, pstSkillLearn->PreDiathesisID);
		if (pstDiathesis == NULL)
		{
			iRet = -1;
		}
		else
		{
			if (pstDiathesis->DiathesisLevel < pstSkillLearn->PreDiathesisLevel)
			{
				iRet = -1;
			}
		}

		if (iRet == -1)
		{
			DIATHESISDEF *pstDiathesisDef = z_find_diathesis(pstEnv, pstSkillLearn->PreDiathesisID, pstSkillLearn->PreDiathesisLevel);
			if (pstDiathesisDef)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL204, pstSkillLearn->PreDiathesisLevel,pstDiathesisDef->Name);
			}
			return -1;
		}
	}

	if (pstSkillLearn->TaskID > 0)
	{
		if (task_find_fini(pstPlayer, pstSkillLearn->TaskID) < 0)
		{
			TASKDEF *pstTaskDef =  task_def_find(pstEnv, pstSkillLearn->TaskID, NULL);
			if (pstTaskDef)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL205, pstTaskDef->Name);
			}
			return -1;
		}
	}
	return 0;
}

int skill_check_by_ui(ZONESVRENV* pstEnv, Player* pstPlayer, SKILLLEARN *pstSkillLearn)
{
	
	ROLEDATA *pstRoleData = &pstPlayer->stRoleData;
	ROLEDETAIL *pstRoleDetail = &pstRoleData->RoleDetail;
	DIATHESISDATA *pstDiathesis;
	int iRet = 0;
	

	if ( (CAREER_NON != pstSkillLearn->Career && (pstRoleData->Career&0x0F) != (pstSkillLearn->Career&0x0F)) ||
		 (CAREER_NON != (pstSkillLearn->Career&0xF0) && (pstRoleData->Career&0xF0) != (pstSkillLearn->Career&0xF0)) )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL202);
		return -1;
	}

	if (pstRoleData->Level < pstSkillLearn->LevelMin)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL203, pstSkillLearn->LevelMin);
		return -1;
	}
	if (pstSkillLearn->Mode == SKILL_LEARN_BOOK)
	{
		return -1;
	}

	if (pstSkillLearn->PreDiathesisID != 0)
	{
		pstDiathesis = z_get_player_diathesis(pstPlayer, pstSkillLearn->PreDiathesisID);
		if (pstDiathesis == NULL)
		{
			iRet = -1;
		}
		else
		{
			if (pstDiathesis->DiathesisLevel < pstSkillLearn->PreDiathesisLevel)
			{
				iRet = -1;
			}
		}

		if (iRet == -1)
		{
			DIATHESISDEF *pstDiathesisDef = z_find_diathesis(pstEnv, pstSkillLearn->PreDiathesisID, pstSkillLearn->PreDiathesisLevel);
			if (pstDiathesisDef)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL204, pstSkillLearn->PreDiathesisLevel,pstDiathesisDef->Name);
			}
			return -1;
		}
	}

	if (pstSkillLearn->ItemID > 0)
	{
		if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstSkillLearn->ItemID, 1, -1, NULL,PACKAGE_DEC_NONE))
		{
			ITEMDEF * pstItemdef = z_find_itemdef(pstEnv, pstSkillLearn->ItemID);
			if (pstItemdef)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL206, pstItemdef->Name);
			}
			return -1;
		}
	}

	/*if (pstSkillLearn->SkillPoint > 0)
	{
		if (pstRoleDetail->SkillPoint < pstSkillLearn->SkillPoint)
		{
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, "当前技能点不够");
			return -1;
		}
	}*/

	if (pstSkillLearn->Exp > 0 && pstRoleDetail->Exp < (int)pstSkillLearn->Exp)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL207);
		return -1;
	}

	if (pstSkillLearn->TaskID > 0)
	{
		if (task_find_fini(pstPlayer, pstSkillLearn->TaskID) < 0)
		{	
			TASKDEF *pstTaskDef =  task_def_find(pstEnv, pstSkillLearn->TaskID, NULL);
			if (pstTaskDef)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL205, pstTaskDef->Name);
			}
			return -1;
		}
	}

	if (pstSkillLearn->MoneyAttrID >= ATTR_ID_MONEY && pstSkillLearn->MoneyAttrID <= ATTR_ID_BULL)
	{
		if (0 > package_dec_money_test(pstEnv, pstPlayer, pstSkillLearn->MoneyAttrID, pstSkillLearn->MoneyVal))
		{
			char szBuff[32];
			
			if (money_type_to_string(pstSkillLearn->MoneyAttrID, szBuff, sizeof(szBuff)) < 0 )
			{
				strcpy(szBuff, "金钱");
			}

			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOHUOBI);
			return -1;
		}
	}
	
	return 0;
}

//技能面板需要消耗道具，道具方式在道具使用中消耗
int skill_consume(ZONESVRENV* pstEnv, Player* pstPlayer, SKILLLEARN *pstSkillLearn)
{
	
	ROLEDATA *pstRoleData = &pstPlayer->stRoleData;
	ROLEDETAIL *pstRoleDetail = &pstRoleData->RoleDetail;
	int iRet;

	CSPKG stPkg;
	CSPLAYERATTRVAL *pstChg = &stPkg.Body.AttrChg;

	if( pstSkillLearn->Exp > 0 )
	{
		if (pstRoleDetail->Exp >= (int)pstSkillLearn->Exp)
		{
			pstRoleDetail->Exp -= pstSkillLearn->Exp;

			pstChg->AttrNum = 1;
			pstChg->AttrList[0].AttrID = ATTR_ID_EXP;
			pstChg->AttrList[0].Value.Exp = pstRoleDetail->Exp;

			Z_CSHEAD_INIT(&stPkg.Head, ATTR_CHG);
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SKILL213);
			return -1;
		}
	}

	if (pstSkillLearn->ItemID > 0 )
	{
		if (0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstSkillLearn->ItemID, 
			1, -1, NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_SKILL))
		{
			return -1;
		}
	}

	if (pstSkillLearn->MoneyAttrID >= ATTR_ID_MONEY && pstSkillLearn->MoneyAttrID <= ATTR_ID_BULL)
	{
		iRet = package_dec_money(pstEnv, pstPlayer, pstSkillLearn->MoneyAttrID, pstSkillLearn->MoneyVal);
		if (iRet < 0)
		{
			return -1;
		}

		if (pstSkillLearn->MoneyAttrID == ATTR_ID_MONEY)
		{
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_SKILL, 0, 0, pstSkillLearn->MoneyVal, "技能消耗");
		}
		else if (pstSkillLearn->MoneyAttrID == ATTR_ID_BULL)
		{
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_SKILL, 0, pstSkillLearn->MoneyVal-iRet, iRet, "技能消耗");
		}
			
	}

	return 0;
}

int skill_update(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,int iSkillID, int iLevel)
{
	ZoneSkill *pstZoneSkill;
	SKILLDEF *pstSkillDef;
	CSSKILLDATA *pstSkillData;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;

	pstSkillDef = z_find_skill(pstEnv, iSkillID, iLevel, NULL);
	if (!pstSkillDef)
	{
		return -1;
	}

	pstZoneSkill = z_get_player_zoneskill(pstPlayer, iSkillID);
	if (NULL == pstZoneSkill)
	{
		if (pstZoneSkillList->nSkillNum >= MAX_ROLE_SKILL)
		{
			return -1;
		}

		z_add_skill(pstAppCtx, pstEnv, pstPlayer, NULL, iSkillID, iLevel, 1);
		
	}
	else
	{
		pstSkillData = &pstZoneSkill->stSkillData;
			
		if (pstSkillData->SkillLevel != iLevel)
		{
			/*
			if (pstSkillData->FittingsRowNum < pstSkillDef->FittingsGridRowNum)
			{
				memset(&pstSkillData->CSFittingsRowList[pstSkillData->FittingsRowNum], 0, 
						(pstSkillDef->FittingsGridRowNum-pstSkillData->FittingsRowNum)*sizeof(pstSkillData->CSFittingsRowList[0]));
				pstSkillData->FittingsRowNum = pstSkillDef->FittingsGridRowNum;
			}
			else if (pstSkillData->FittingsRowNum > pstSkillDef->FittingsGridRowNum)
			{
				memset(&pstSkillData->CSFittingsRowList[pstSkillDef->FittingsGridRowNum], 0, 
						(pstSkillData->FittingsRowNum - pstSkillDef->FittingsGridRowNum)*sizeof(pstSkillData->CSFittingsRowList[0]));
				pstSkillData->FittingsRowNum = pstSkillDef->FittingsGridRowNum;
			}
			*/

			pstSkillData->SkillLevel = iLevel;
			z_update_one_skill(pstAppCtx, pstEnv, pstPlayer, NULL, pstZoneSkill);
			pstSkillData->CoolLeft = 0;
			/*
			if (pstZoneSkill->stCoolEnd.tv_sec > pstAppCtx->stCurr.tv_sec)
			{
				struct timeval stTmp;
				long long  llMs=0;
				memset(&stTmp,0,sizeof(stTmp));
				TV_DIFF(stTmp, pstZoneSkill->stCoolEnd, pstAppCtx->stCurr);
				TV_TO_MS(llMs, stTmp);
				
				pstSkillData->CoolLeft = llMs/100;
			}
			*/
			z_skill_chg(pstAppCtx, pstEnv, pstPlayer, pstZoneSkill);

			task_player_trace(pstEnv, pstPlayer, TASK_OBJ_PLAYER_SKILL);
		}
	}
	return 0;
}

int skill_learn(ZONESVRENV* pstEnv, Player* pstPlayer, int iSkillID, int iLevel)
{
	SKILLDEF *pstSkillDef;
	ZoneSkill *pstZoneSkill;
	tdr_longlong iPreExp;

	if (iLevel != 1)
	{
		return -1;
	}
	
	pstSkillDef = z_find_skill(pstEnv, iSkillID, iLevel,NULL);
	if (NULL == pstSkillDef )
	{
		return -1;
	}
	
	//检查是否已经学会
	pstZoneSkill = z_get_player_zoneskill(pstPlayer, iSkillID);
	if (pstZoneSkill)
	{
		return -1;
	}
	
	//技能学习前提的检查
	if (0 > skill_check_by_ui(pstEnv, pstPlayer, &pstSkillDef->Learn))
	{
		return -1;
	}

	iPreExp  = pstPlayer->stRoleData.RoleDetail.Exp;

	//技能消耗
	if (0 == skill_consume(pstEnv, pstPlayer, &pstSkillDef->Learn))
	{
		if (z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, iSkillID, iLevel, 1) == 0)
		{
			skill_action(pstEnv->pstAppCtx, pstEnv, pstPlayer, iSkillID, iLevel, SKILL_LEARN);

			z_role_skill_oplog(pstEnv, pstPlayer, OP_FACE_LEARN_SKILL, iSkillID, iLevel, 0,
								pstPlayer->stRoleData.RoleDetail.Exp, iPreExp, &pstSkillDef->Learn,NULL,0,NULL);
		}
	}
	
	return 0;
}

int skill_levelup(ZONESVRENV* pstEnv, Player* pstPlayer, int iSkillID, int iLevel)
{
	SKILLDEF *pstSkillDef;
	ZoneSkill *pstZoneSkill;
	tdr_longlong iPreExp;
	int iPreLevel;

	pstSkillDef = z_find_skill(pstEnv, iSkillID, iLevel, NULL);
	if (NULL == pstSkillDef )
	{
		return -1;
	}

	pstZoneSkill = z_get_player_zoneskill(pstPlayer, iSkillID);
	if (pstZoneSkill == NULL ||  iLevel - pstZoneSkill->stSkillData.SkillLevel != 1)
	{
		return -1;
	}

	//技能面板学习最大限制
	if (pstSkillDef->LearnLevelLimit != 0  && pstZoneSkill->stSkillData.SkillLevel >= pstSkillDef->LearnLevelLimit)
	{
		return -1;
	}

	//技能学习前提的检查
	if (0 > skill_check_by_ui(pstEnv, pstPlayer, &pstSkillDef->Learn))
	{
		return -1;
	}

	iPreExp = pstPlayer->stRoleData.RoleDetail.Exp;
	iPreLevel = pstZoneSkill->stSkillData.SkillLevel;
	//技能消耗
	if (0 == skill_consume(pstEnv, pstPlayer, &pstSkillDef->Learn))
	{
		if (skill_update(pstEnv->pstAppCtx, pstEnv,pstPlayer,iSkillID, iLevel) == 0)
		{
			skill_action(pstEnv->pstAppCtx, pstEnv, pstPlayer, iSkillID, iLevel, SKILL_LEVEL_UP);

			z_role_skill_oplog(pstEnv, pstPlayer, OP_FACE_LEARN_SKILL, iSkillID, iLevel, iPreLevel,
								pstPlayer->stRoleData.RoleDetail.Exp, iPreExp, &pstSkillDef->Learn,NULL,0,NULL);
		}
	}
	
	return 0;
}

int diathesis_learn(ZONESVRENV* pstEnv, Player* pstPlayer, int iDiathesisID, int iLevel)
{
	
	DIATHESISDEF *pstDiathesisDef;
	DIATHESISDATA *pstDiathesis;
	DIATHESISINFO *pstDiathesisInfo;
	DIATHESISDATA *pstMetuxDiathesis = NULL;

	pstDiathesisInfo = &pstPlayer->stRoleData.DiathesisInfo;
	if (iLevel != 1)
	{
		return -1;
	}
	
	pstDiathesisDef = z_find_diathesis(pstEnv, iDiathesisID, iLevel);
	if (pstDiathesisDef == NULL )
	{
		return -1;
	}

	
	// 是不是学习过该技能
	pstDiathesis = z_get_player_diathesis(pstPlayer, iDiathesisID);
	if (pstDiathesis)
	{
		return -1;
	}

	//互斥天赋不能学
	pstMetuxDiathesis = z_get_player_diathesis(pstPlayer, pstDiathesisDef->MutexDiathesisID);
	if (pstMetuxDiathesis)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,  ZONEERR_SKILL208);
		return -1;
	}

	// 如果是需要天赋点来学习的,这里要判断下天赋点的情况
	if( pstDiathesisInfo->LearnGeniusPoint < pstDiathesisDef->UseGeniusPoint  )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,  ZONEERR_SKILL209);
		return -1;
	}
	if( pstDiathesisInfo->UsedGeniusPoint < pstDiathesisDef->PreUsedGeniusPoint  )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,  ZONEERR_SKILL210);
		return -1;
	}
	
	//技能学习前提的检查
	if (0 > skill_check_by_ui(pstEnv, pstPlayer, &pstDiathesisDef->Learn))
	{
		return -1;
	}	

	//技能消耗
	if (0 == skill_consume(pstEnv, pstPlayer, &pstDiathesisDef->Learn))
	{
		 if (z_add_diathesis(pstEnv->pstAppCtx, pstEnv, pstPlayer, iDiathesisID, iLevel) == 0)
		 {
			skill_action(pstEnv->pstAppCtx, pstEnv, pstPlayer, iDiathesisID, iLevel, DIATHESIS_LEARN);
			//加成功了,这里要算下天赋
			if( pstDiathesisInfo->LearnGeniusPoint >= pstDiathesisDef->UseGeniusPoint  )
			{
				pstDiathesisInfo->LearnGeniusPoint -= pstDiathesisDef->UseGeniusPoint;
				pstDiathesisInfo->UsedGeniusPoint += pstDiathesisDef->UseGeniusPoint;
				z_genius_chg( pstEnv, pstPlayer,&pstPlayer->stRoleData.DiathesisInfo);
			}
		 }
	}
	return 0;
}

int diathesis_levelup(ZONESVRENV* pstEnv, Player* pstPlayer, int iDiathesisID, int iLevel)
{
	DIATHESISDEF *pstDiathesisDef;
	DIATHESISINFO *pstDiathesisInfo = &pstPlayer->stRoleData.DiathesisInfo;
	DIATHESISDATA *pstDiathesis;

	pstDiathesisDef = z_find_diathesis(pstEnv, iDiathesisID, iLevel);
	if (pstDiathesisDef == NULL || pstDiathesisDef->DiathesisID != iDiathesisID)
	{
		return -1;
	}

	pstDiathesis = z_get_player_diathesis(pstPlayer, iDiathesisID);
	if (pstDiathesis == NULL)
	{
		return -1;
	}

	if (iLevel - pstDiathesis->DiathesisLevel != 1)
	{
		return -1;
	}
	// 如果是需要天赋点来学习的,这里要判断下天赋点的情况
	if( pstDiathesisInfo->LearnGeniusPoint < pstDiathesisDef->UseGeniusPoint  )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,  ZONEERR_SKILL209);
		return -1;
	}
	if( pstDiathesisInfo->UsedGeniusPoint < pstDiathesisDef->PreUsedGeniusPoint  )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,  ZONEERR_SKILL210);
		return -1;
	}	
	if (0 > skill_check_by_ui(pstEnv, pstPlayer, &pstDiathesisDef->Learn))
	{
		return -1;
	}


	if (0 == skill_consume(pstEnv, pstPlayer, &pstDiathesisDef->Learn))
	{
		pstDiathesis->DiathesisLevel = iLevel;
		z_diathesis_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstDiathesis);
		skill_action(pstEnv->pstAppCtx, pstEnv, pstPlayer, iDiathesisID, iLevel, DIATHESIS_LEVEL_UP);
		
		//加成功了,这里要算下天赋
		if( pstDiathesisInfo->LearnGeniusPoint >= pstDiathesisDef->UseGeniusPoint  )
		{
			pstDiathesisInfo->LearnGeniusPoint -= pstDiathesisDef->UseGeniusPoint;
			pstDiathesisInfo->UsedGeniusPoint += pstDiathesisDef->UseGeniusPoint;

			z_genius_chg( pstEnv, pstPlayer,&pstPlayer->stRoleData.DiathesisInfo);
		}

		z_role_diathesis_oplog(pstEnv, pstPlayer, OP_LEVEL_UP_DIATHESIS, iDiathesisID, iLevel, iLevel-1);
	}

	return 0;
}

int open_skill_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int iNpcID)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstActionRes->ActionID = ACTION_UI;
	pstActionRes->Data.UI.UIType = UI_TYPE_SKILL;
	pstActionRes->ID = iNpcID;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int open_skill_fitting_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int iNpcID,short uiType)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	UNUSED(pstAppCtx);

	pstActionRes->ActionID = ACTION_UI;
	//pstActionRes->Data.UI.UIType = UI_TYPE_FIT;
	pstActionRes->Data.UI.UIType = uiType;
	pstActionRes->ID = iNpcID;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int open_genius_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int iNpcID)
{		
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	
	pstActionRes->ActionID = ACTION_UI;
	pstActionRes->Data.UI.UIType = UI_TYPE_GENIUS;
	pstActionRes->ID = iNpcID;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}


int open_diathesis_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int iNpcID)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	
	pstActionRes->ActionID = ACTION_UI;
	pstActionRes->Data.UI.UIType = UI_TYPE_DIATHESIS;
	pstActionRes->ID = iNpcID;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int genius_all_reset(ZONESVRENV* pstEnv, Player* pstPlayer)
{
	DIATHESISDEF *pstDiathesisDef;
	DIATHESISINFO *pstDiathesisInfo;
	int i;
	DIATHESISDATA *pstDiathesisData;

	pstDiathesisInfo = &pstPlayer->stRoleData.DiathesisInfo;

	if(pstDiathesisInfo->DiathesisNum < 1)
	{
		return 0;
	}
	for (i=pstDiathesisInfo->DiathesisNum-1; i>=0; i--)
	{
		pstDiathesisData = &pstDiathesisInfo->DiathesisList[i];
		if (pstDiathesisData->DiathesisID > 0)
		{
			pstDiathesisDef = z_find_diathesis(pstEnv,pstDiathesisData->DiathesisID,pstDiathesisData->DiathesisLevel);
			if (pstDiathesisDef == NULL )
			{
				continue;
			}
			if( pstDiathesisDef->UseGeniusPoint >0  || 
				pstDiathesisDef->PreUsedGeniusPoint >0 ||
				pstDiathesisData->GeniusFlag == 1)
			{
				//如果定义里面有定义天赋的数据,认为这个素质是天赋
				//不是第一个元素,也不是最后一个元素,才做拷贝操作
				if( pstDiathesisInfo->DiathesisNum > 1 &&
					i != (int)pstDiathesisInfo->DiathesisNum-1)
				{
					memcpy(pstDiathesisData,	&pstDiathesisInfo->DiathesisList[pstDiathesisInfo->DiathesisNum-1],
						sizeof(DIATHESISDATA));
				}

				if(pstDiathesisInfo->DiathesisNum > 0)
				{
					pstDiathesisInfo->DiathesisNum--;
				}
			}
		}
	}

	pstDiathesisInfo->LearnGeniusPoint += pstDiathesisInfo->UsedGeniusPoint;
	pstDiathesisInfo->UsedGeniusPoint = 0;

	z_genius_chg( pstEnv, pstPlayer,&pstPlayer->stRoleData.DiathesisInfo);
	
	// 这里要下发下玩家的整个素质
	z_diathesis_all_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer);

	z_role_diathesis_oplog(pstEnv, pstPlayer, OP_RESET_DIATHESIS, 0, 0, 0);
	
	return 0;
}

int player_skill_learn(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	SKILLDEF *pstSkillDef;
	CSSKILLLEARN *pstSkillLearn = &pstCsPkg->Body.SkillLearn;

	UNUSED(pstFrameHead);

	if ( (pstSkillLearn->NpcID!= pstPlayer->stOnline.iDialogID)
		&& pstSkillLearn->type != GENIUS_LEARN 
		&&pstSkillLearn->type != DIATHESIS_LEVEL_UP 
		&&pstSkillLearn->type != GENIUS_LEVEL_UP 
		)
	{
		return -1;
	}

	if (pstSkillLearn->type != GENIUS_LEARN 
		&&pstSkillLearn->type != DIATHESIS_LEVEL_UP 
		&&pstSkillLearn->type != GENIUS_LEVEL_UP 
		&& 0 == z_player_act_status(pstAppCtx, pstEnv, pstPlayer, RES_ACTION_DIALOG))
	{
		return -1;
	}

	switch(pstSkillLearn->type)
	{
	case SKILL_LEARN:
		if (0 > check_npc_ui(pstEnv, pstPlayer, UI_TYPE_SKILL))
		{
			break;
		}
		pstSkillDef = z_find_skill(pstEnv, pstSkillLearn->ID, pstSkillLearn->Level,NULL);
		if (NULL == pstSkillDef )
		{
			return -1;
		}
		//不是在面板处学习的话就退出
		if( pstSkillDef->Learn.Mode == SKILL_LEARN_UI ||
			pstSkillDef->Learn.Mode == SKILL_LEARN_ALL)
		{
			skill_learn(pstEnv, pstPlayer, pstSkillLearn->ID, pstSkillLearn->Level);
		}
		break;
	case SKILL_LEVEL_UP:
		if (0 > check_npc_ui(pstEnv, pstPlayer, UI_TYPE_SKILL))
		{
			break;
		}
		skill_levelup(pstEnv, pstPlayer, pstSkillLearn->ID, pstSkillLearn->Level);
		break;
	//case DIATHESIS_LEARN:
	case GENIUS_LEARN:
		/*if (0 > check_npc_ui(pstEnv, pstPlayer, UI_TYPE_GENIUS))
		{
			break;
		}*/
		diathesis_learn(pstEnv, pstPlayer, pstSkillLearn->ID, pstSkillLearn->Level);
		break;
	case DIATHESIS_LEVEL_UP:
	case GENIUS_LEVEL_UP:
		diathesis_levelup(pstEnv, pstPlayer, pstSkillLearn->ID, pstSkillLearn->Level);
		break;
	case GENIUS_ALL_RESET:
		//genius_all_reset(pstEnv, pstPlayer);
		break;
	default:
		return -1;
		break;
	}
	return 0;
}

int fly_action(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,  int iType)
{
	CSPKG stPkgRes;
	CSACTIONRES *pstActionRes = &stPkgRes.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_FLY;
	pstActionRes->Data.Fly.Type = iType;

	Z_CSHEAD_INIT(&stPkgRes.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkgRes, 0);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int player_fly(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,  Player *pstPlayer)
{
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;
	ZoneStatus *pstStutas;
	
	
	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos	=	pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	if (pstPlayer->stOnline.State & CS_STAT_FLY)
	{
		if (pstPlayer->stOnline.cMoving)
		{
			z_player_move_curr(pstAppCtx, pstEnv, pstPlayer, 0);
		}
	
		if (z_valid_pos(pstEnv->pstMapObj, pstPlayer->stRoleData.Map, &pstMapInst->iMapPos, &pstPlayer->stRoleData.Pos))
		{
			pstPlayer->stOnline.State &= ~CS_STAT_FLY;
			fly_action(pstAppCtx, pstEnv, pstPlayer, 1);
			
			z_player_chk_mspd(pstAppCtx, pstEnv, pstPlayer);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,  ZONEERR_SKILL211);
		}
	}
	else
	{
		if (pstPlayer->stOnline.State & CS_STAT_RIDE_ON)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,  ZONEERR_SKILL212);
			return -1;
		}
		//飞行和特殊移动不能并存
		pstStutas = z_player_find_status(pstPlayer, STATUS_ID_JIZOU);
		if (pstStutas)
		{
			z_player_del_status(pstAppCtx, pstEnv, pstPlayer, STATUS_ID_JIZOU, 1, 0);
		}

		pstStutas = z_player_find_status(pstPlayer, STATUS_ID_QIANXIN);
		if (pstStutas)
		{
			z_player_del_status(pstAppCtx, pstEnv, pstPlayer, STATUS_ID_QIANXIN, 1, 0);
		}
			
		pstPlayer->stOnline.State |= CS_STAT_FLY;
		fly_action(pstAppCtx, pstEnv, pstPlayer, 0);
		
		z_player_chk_mspd(pstAppCtx, pstEnv, pstPlayer);
	}
	return 0;
}

int sit_action(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,  int iType)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	UNUSED(pstAppCtx);

	pstActionRes->ActionID = ACTION_SIT_DOWN;
	pstActionRes->ID = pstPlayer->iMemID;

	pstActionRes->Data.Sit.Flag = iType;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	return 0;
}

int player_sit_break(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,  Player *pstPlayer)
{
	
	// 退出打坐
	pstPlayer->stOnline.State &= ~ CS_STAT_SIT;
	pstPlayer->stOnline.State &= ~ CS_STAT_GAOJI_SIT;

	sit_action(pstAppCtx, pstEnv, pstPlayer, 2);
	
	return 0;
}

int player_sit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,  Player *pstPlayer)
{
	if(pstPlayer->stOnline.State & CS_STAT_SIT)
	{
		return 0;
	}

	if(pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	}

	if(pstPlayer->stOnline.cMoving)
	{
		z_player_stop(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, NULL);
	}

	if(!z_player_act_status(pstAppCtx, pstEnv, pstPlayer, RES_ACTION_SIT))
	{
		return -1;
	}
	
	if(pstPlayer->stOnline.stStatusList.bBadStatusNum > 0)
	{
		//z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "不良状态下不能打坐");
		return -1;
	}

	pstPlayer->stOnline.State |=  CS_STAT_SIT;
	sit_action(pstAppCtx, pstEnv, pstPlayer, 0);

	return 0;
}

//是否是控制技能效果
int is_ctrl_skill_result(SKILLRESULT *pstResult)
{
	if(pstResult->SkillResultID == RESULT_HITBACK)
	{
		return 1;
	}

	if(pstResult->SkillResultID == RESULT_PULL)
	{
		return 1;
	}
	
	return 0;
}

int is_ctrl_skill(SKILLDEF *pstSkillDef)
{
	int i;

	for(i=0; i<MAX_SKILL_RESULT; i++)
	{
		if(pstSkillDef->SkillResult[i].SkillResultID == 0)
		{
			break;
		}

		if(is_ctrl_skill_result(&pstSkillDef->SkillResult[i]))
		{
			return 1;
		}
	}

	return 0;
}
