/*
**  @file $RCSfile: zone_range.c,v $
**  general description of this module
**  $Id: zone_range.c,v 1.116 2014/05/30 09:41:35 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/05/30 09:41:35 $
**  @version $Revision: 1.116 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

/*
     注意: 新增加排行类型，需要关注如下函数:
     range_init
     range_delete_role
     range_once_type
     range_get_name
     range_compare
     range_up_role_name
     unite_svr.c
*/

#include "zone_range.h"
#include "zone_clt.h"
#include "zone_clan.h"
#include "zone_player.h"
#include "zone_pet.h"
#include "zone_map.h"
#include "zone_team.h"
#include "zone_spansvr.h"
#include "zone_itemlimit.h"
#include "zone_battle.h"
#include "zone_designation.h"
#include "zone_strongpoint.h"
#include "zone_span.h"
#include "zone_misc.h"
#include "zone_genius.h"
#include "zone_sparta.h"
#include "zone_err.h"
#include "zone_attr.h"
#include "zone_gm.h"
#include "zone_city.h"
#include "zone_db.h"

#define RANGE_UPDATE_NUM 200

extern int g_iDesigVar;

int range_db_update(ZONESVRENV* pstEnv);
int player_range_dsn(ZONESVRENV* pstEnv);
int range_do_chg_sparta_pre(ZONESVRENV* pstEnv, RangeOneMem *pstRangeOneMem);
int range_do_chg_weekcharm_pre(ZONESVRENV* pstEnv, RangeOneMem *pstRangeOneMem);
static int watchcmp(const void * pv1 , const void *pv2)
{
	const WATCHONE *p1 = (WATCHONE*) pv1;
	const WATCHONE *p2 = (WATCHONE*) pv2;
	return strcmp(p1->RoleName, p2->RoleName);
}

static unsigned int watchcode(const void* pvData)
{
	const WATCHONE *p1 = (WATCHONE*) pvData;
	return sht_get_code(p1->RoleName);
}

int range_init_one(ZONESVRENV* pstEnv, int iMainType, int iSubType)
{
	int i;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
		
	RangeOneMem *pstRangeOneMem = &pstObj->stRange.astRange[pstObj->stRange.iRangeNum];

	if (pstObj->stRange.iRangeNum >= RANGE_MAX)
	{
		return -1;
	}

	memset(pstRangeOneMem, 0 , sizeof(*pstRangeOneMem));
	pstRangeOneMem->iMainType = iMainType;
	pstRangeOneMem->iSubType = iSubType;

	for (i=0; i<RANGE_MAX_PAGE; i++)
	{
		pstRangeOneMem->stCurrRange.aiPageVersion[i] = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstRangeOneMem->stLastRange.aiPageVersion[i] = pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	pstObj->stRange.iRangeNum++;
		
	return 0;
}

RangeOneMem *range_find_onemem(ZONESVRENV* pstEnv, int iMainType, int iSubType)
{
	int i;
	RangeMem *pstRangeMem = &pstEnv->pstZoneObj->stRange;

	for (i=0; i<pstRangeMem->iRangeNum; i++)
	{
		if (pstRangeMem->astRange[i].iMainType == iMainType && pstRangeMem->astRange[i].iSubType == iSubType)
		{
			return &pstRangeMem->astRange[i];
		}
	}

	return NULL;
}





// 获取玩家在排行榜排名
int player_get_top_range(ZONESVRENV* pstEnv,Player *pstPlayer,  int iMainType, int iSubType)
{
	RangeOneMem *pstOneMem = NULL;
	int i;
	RangeOne *pstCurr;
	
	pstOneMem = range_find_onemem(pstEnv, iMainType, iSubType);
	if (!pstOneMem)
		return -1;

	pstCurr = &pstOneMem->stCurrRange;
	if (iMainType == RANGE_MAIN_SINGLE)
	{
		if(iSubType == RANGE_MAIN_SINGLE_SUB_LEVEL)
		{
			// 小于最后一个 , 肯定没上榜 ,返回0
			if (pstPlayer->stRoleData.Level < pstCurr->astRangeEntry[pstCurr->iNum-1].stRange.RangeData.Level)
			{
				return 0;
			}
		}
		else if (iSubType == RANGE_MAIN_SINGLE_SUB_MONEY)
		{
			if (pstPlayer->stRoleData.RoleDetail.Money < pstCurr->astRangeEntry[pstCurr->iNum-1].stRange.RangeData.RangeVal)
			{
				return 0;
			}
		}

		for (i=pstCurr->iNum-1; i>=0; i--)
		{
			if ( pstCurr->astRangeEntry[i].stRange.RangeData.GID == pstPlayer->stRoleData.RoleID)
			{
				return pstCurr->astRangeEntry[i].stRange.TopRange;
			}
		}
	}
	
	return 0;
}


int range_compare(TOPRANGE *p1, TOPRANGE *p2)
{
	if (p1->MainType == RANGE_MAIN_COMBAT && 
		(p1->SubType == RANGE_MAIN_COMBAT_SUB_PANSHAN_TIME ||
		p1->SubType == RANGE_MAIN_COMBAT_SUB_DRAGON_TIME ||
		p1->SubType == RANGE_MAIN_COMBAT_SUB_MOGUI_TIME))
	{
		if (p1->RangeData.RangeVal < p2->RangeData.RangeVal)
		{
			return 1;
		}
		else if (p1->RangeData.RangeVal == p2->RangeData.RangeVal)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		if (p1->RangeData.RangeVal > p2->RangeData.RangeVal)
		{
			return 1;
		}
		else if (p1->RangeData.RangeVal == p2->RangeData.RangeVal)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
}

int range_unique(TOPRANGE *p1, TOPRANGE *p2)
{
	if (p1->RangeData.GID == p2->RangeData.GID)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int range_set_one_replace(ZONESVRENV* pstEnv, RangeOne *pstRangeOne, TOPRANGE *pstData)
{
	int i;
	
	if (pstRangeOne->iNum <= 0)
	{
		memcpy(&pstRangeOne->astRangeEntry[pstRangeOne->iNum].stRange, pstData, sizeof(pstRangeOne->astRangeEntry[0].stRange));
		pstRangeOne->iNum++;
		return 0;
	}

	for (i=pstRangeOne->iNum-1; i>=0; i--)
	{
		if (0 == range_unique(&pstRangeOne->astRangeEntry[i].stRange, pstData))
		{
			return 0;
		}
	}

	if (0 <= range_compare(&pstRangeOne->astRangeEntry[pstRangeOne->iNum-1].stRange, pstData))
	{
		if (pstRangeOne->iNum < RANGE_ONE_NUM)
		{
			memcpy(&pstRangeOne->astRangeEntry[pstRangeOne->iNum].stRange, pstData, sizeof(pstRangeOne->astRangeEntry[0].stRange));
			pstRangeOne->iNum++;
		}
		
		return 0;
	}

	for (i=pstRangeOne->iNum-1; i>=0; i--)
	{
		if (0 <= range_compare(&pstRangeOne->astRangeEntry[i].stRange, pstData))
		{
			break;
		}
	}

	if (pstRangeOne->iNum < RANGE_ONE_NUM)
	{
		pstRangeOne->iNum += 1;
	}

	memmove(&pstRangeOne->astRangeEntry[i+2], &pstRangeOne->astRangeEntry[i+1],
				(pstRangeOne->iNum - i -2)*sizeof(pstRangeOne->astRangeEntry[0]));
	
	memcpy(&pstRangeOne->astRangeEntry[i+1].stRange, pstData, sizeof(pstRangeOne->astRangeEntry[0].stRange));
			
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int range_set_one_once(ZONESVRENV* pstEnv, RangeOne *pstRangeOne, TOPRANGE *pstData)
{
	int i, iPos, iSwitchNum;
	
	if (pstRangeOne->iNum <= 0)
	{
		memcpy(&pstRangeOne->astRangeEntry[pstRangeOne->iNum].stRange, pstData, sizeof(pstRangeOne->astRangeEntry[0].stRange));
		pstRangeOne->iNum++;
		return 0;
	}

	for (i=pstRangeOne->iNum-1; i>=0; i--)
	{
		if (0 == range_unique(&pstRangeOne->astRangeEntry[i].stRange, pstData))
		{
			break;
		}
	}
	
	if (i >= 0) //找到了
	{
		if (0 <= range_compare(&pstRangeOne->astRangeEntry[i].stRange, pstData))
		{
			//只更新信息，不修改排名
			unsigned int RangeVal;

			RangeVal = pstRangeOne->astRangeEntry[i].stRange.RangeData.RangeVal;
			memcpy(&pstRangeOne->astRangeEntry[i].stRange, pstData, sizeof(pstRangeOne->astRangeEntry[0].stRange));
			pstRangeOne->astRangeEntry[i].stRange.RangeData.RangeVal = RangeVal;
			
			return 0;
		}

		memcpy(&pstRangeOne->astRangeEntry[i].stRange, pstData, sizeof(pstRangeOne->astRangeEntry[0].stRange));

		iPos = i;
		iSwitchNum = 0;
		for (i=iPos-1; i>=0; i--)
		{
			if (0 <= range_compare(&pstRangeOne->astRangeEntry[i].stRange, pstData))
			{
				break;
			}
			else //交换
			{
				iSwitchNum ++;
			}
		}

		if (iSwitchNum)
		{
			memmove(&pstRangeOne->astRangeEntry[iPos+1-iSwitchNum], 
						&pstRangeOne->astRangeEntry[iPos-iSwitchNum], 
						iSwitchNum*sizeof(pstRangeOne->astRangeEntry[0]));
			memcpy(&pstRangeOne->astRangeEntry[iPos-iSwitchNum].stRange, pstData, 
					sizeof(pstRangeOne->astRangeEntry[0].stRange));
		}
		
		return 0;
	}

	if (0 <= range_compare(&pstRangeOne->astRangeEntry[pstRangeOne->iNum-1].stRange, pstData))
	{
		if (pstRangeOne->iNum < RANGE_ONE_NUM)
		{
			memcpy(&pstRangeOne->astRangeEntry[pstRangeOne->iNum].stRange, pstData, sizeof(pstRangeOne->astRangeEntry[0].stRange));
			pstRangeOne->iNum++;
		}
		
		return 0;
	}

	for (i=pstRangeOne->iNum-1; i>=0; i--)
	{
		if (0 <= range_compare(&pstRangeOne->astRangeEntry[i].stRange, pstData))
		{
			break;
		}
	}

	if (pstRangeOne->iNum < RANGE_ONE_NUM)
	{
		pstRangeOne->iNum += 1;
	}

	memmove(&pstRangeOne->astRangeEntry[i+2], &pstRangeOne->astRangeEntry[i+1],
				(pstRangeOne->iNum - i -2)*sizeof(pstRangeOne->astRangeEntry[0]));
	
	memcpy(&pstRangeOne->astRangeEntry[i+1].stRange, pstData, sizeof(pstRangeOne->astRangeEntry[0].stRange));
			
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int range_set_one_hold(ZONESVRENV* pstEnv, RangeOne *pstRangeOne, TOPRANGE *pstData)
{
	int i, iPos;
	int iSwitchNum;
	
	if (pstRangeOne->iNum <= 0)
	{
		memcpy(&pstRangeOne->astRangeEntry[pstRangeOne->iNum].stRange, pstData, sizeof(pstRangeOne->astRangeEntry[0].stRange));
		pstRangeOne->iNum++;
		return 0;
	}

	for (i=pstRangeOne->iNum-1; i>=0; i--)
	{
		if (0 == range_unique(&pstRangeOne->astRangeEntry[i].stRange, pstData))
		{
			break;
		}
	}
	
	if (i >= 0) //找到了
	{
		memcpy(&pstRangeOne->astRangeEntry[i].stRange, pstData, sizeof(pstRangeOne->astRangeEntry[0].stRange));

		iPos = i;
		iSwitchNum = 0;
		for (i=iPos-1; i>=0; i--)
		{
			if (0 <= range_compare(&pstRangeOne->astRangeEntry[i].stRange, pstData))
			{
				break;
			}
			else //交换
			{
				iSwitchNum ++;
			}
		}

		if (iSwitchNum)
		{
			memmove(&pstRangeOne->astRangeEntry[iPos+1-iSwitchNum], 
						&pstRangeOne->astRangeEntry[iPos-iSwitchNum], 
						iSwitchNum*sizeof(pstRangeOne->astRangeEntry[0]));
			memcpy(&pstRangeOne->astRangeEntry[iPos-iSwitchNum].stRange, pstData, 
					sizeof(pstRangeOne->astRangeEntry[0].stRange));
		}
		else	
		{
			for (i=iPos+1; i<pstRangeOne->iNum; i++)
			{
				if (0 <= range_compare(pstData, &pstRangeOne->astRangeEntry[i].stRange))
				{
					break;
				}
				else //交换
				{
					iSwitchNum++;
				}
			}

			if (iSwitchNum)
			{
				memmove(&pstRangeOne->astRangeEntry[iPos], 
						&pstRangeOne->astRangeEntry[iPos+1], 
						iSwitchNum*sizeof(pstRangeOne->astRangeEntry[0]));
				
				memcpy(&pstRangeOne->astRangeEntry[iPos+iSwitchNum].stRange, pstData, 
						sizeof(pstRangeOne->astRangeEntry[0].stRange));
			}
		}

		return 0;
	}


	if (0 <= range_compare(&pstRangeOne->astRangeEntry[pstRangeOne->iNum-1].stRange, pstData))
	{
		if (pstRangeOne->iNum < RANGE_ONE_NUM)
		{
			memcpy(&pstRangeOne->astRangeEntry[pstRangeOne->iNum].stRange, pstData, sizeof(pstRangeOne->astRangeEntry[0].stRange));
			pstRangeOne->iNum++;
		}
		
		return 0;
	}
	
	for (i=pstRangeOne->iNum-1; i>=0; i--)
	{
		if (0 <= range_compare(&pstRangeOne->astRangeEntry[i].stRange, pstData))
		{
			break;
		}
	}

	if (pstRangeOne->iNum < RANGE_ONE_NUM)
	{
		pstRangeOne->iNum += 1;
	}

	memmove(&pstRangeOne->astRangeEntry[i+2], &pstRangeOne->astRangeEntry[i+1],
				(pstRangeOne->iNum - i -2)*sizeof(pstRangeOne->astRangeEntry[0]));
	
	memcpy(&pstRangeOne->astRangeEntry[i+1].stRange, pstData, sizeof(pstRangeOne->astRangeEntry[0].stRange));
			
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int range_once_type(int iMainType, int iSubType)
{
	if (iMainType == RANGE_MAIN_SINGLE)
	{
		if (iSubType == RANGE_MAIN_SINGLE_SUB_LEVEL)
		{
			return 1;
		}
	}
	else if (iMainType == RANGE_MAIN_CAREER)
	{
		return 1;
	}
	else if (iMainType == RANGE_MAIN_COMBAT)
	{
		return 1;
	}
	else if (iMainType == RANGE_MAIN_BATTLE)
	{
		return 1;
	}

	return 0;
}
	
int range_set(ZONESVRENV* pstEnv, TOPRANGE *pstData)
{
	RangeOneMem *pstRangeOneMem;

	if (INST_MODE_ZONE_SVR != pstEnv->iInstMode)
	{
		Player *pstPlayer;
		SSPKG stSSPkg;
		SSCMDREQ* pstCmdReq = &stSSPkg.Body.CmdReq;
		int iBusID;

		if (pstData->MainType == RANGE_MAIN_PET)
		{
			pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstData->RangeData.SecondName);
			battle_rolename_remove_prefix(pstData->RangeData.SecondName);
		}
		else
		{
			pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstData->RangeData.FirstName);
			battle_rolename_remove_prefix(pstData->RangeData.FirstName);
		}
		
		if (NULL == pstPlayer)
		{
			return -1;
		}
				
		iBusID = player_master_busid_get(pstEnv, pstPlayer);
		
		memset(pstCmdReq,0,sizeof(*pstCmdReq));
		pstCmdReq->Cmd = SS_CMD_RANGE_PROXY;
		pstCmdReq->Data.RangeProxy = *pstData;
		
		Z_SSHEAD_INIT(&stSSPkg.Head, SS_CMD_REQ);
		z_ssmsg_send(pstEnv, &stSSPkg, iBusID);
		
		return 0;
	}

	if (pstEnv->pstZoneObj->stRange.iState < RANGE_STAT_DB_FINI)
	{
		return -1;
	}
	
	pstRangeOneMem = range_find_onemem(pstEnv, pstData->MainType, pstData->SubType);
	if (NULL == pstRangeOneMem)
	{
		return -1;
	}

	if (range_once_type(pstData->MainType, pstData->SubType))
	{
		range_set_one_once(pstEnv, &pstRangeOneMem->stCurrRange, pstData);
	}
	else
	{
		range_set_one_hold(pstEnv, &pstRangeOneMem->stCurrRange, pstData);
	}

	return 0;
}

int range_attr_set_in(ZONESVRENV* pstEnv, Player *pstPlayer, int iSubType, unsigned int  uiVal)
{
	TOPRANGE stRange;
	ZoneClan *pstClan;
	
	stRange.MainType = RANGE_MAIN_ATTR;
	stRange.SubType = iSubType;
	stRange.TopRange = -1;
	stRange.RangeData.Career = pstPlayer->stRoleData.Career;
	STRNCPY(stRange.RangeData.FirstName, pstPlayer->stRoleData.RoleName, sizeof(stRange.RangeData.FirstName));
	stRange.RangeData.GID = pstPlayer->stRoleData.RoleID;
	stRange.RangeData.LastRange = -1;
	stRange.RangeData.Level = pstPlayer->stRoleData.Level;
	stRange.RangeData.RangeVal = uiVal;

	if(pstEnv->iInstMode == INST_MODE_ZONE_SVR)
	{
		pstClan = player_get_clan(pstEnv, pstPlayer);
		if (pstClan)
		{
			STRNCPY(stRange.RangeData.SecondName, pstClan->stClan.ClanProf.Name, 
						sizeof(stRange.RangeData.SecondName));
		}
		else
		{
			stRange.RangeData.SecondName[0] = 0;
		}
	}
	else
	{
		if (pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID > 0)
		{
			STRNCPY(stRange.RangeData.SecondName, pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Name, 
						sizeof(stRange.RangeData.SecondName));
		}
		else
		{
			stRange.RangeData.SecondName[0] = 0;
		}
	}
	
	stRange.RangeData.Val1 = 0;
	
	return range_set(pstEnv, &stRange);
}

int range_attr_set(ZONESVRENV* pstEnv, Player *pstPlayer,DYNATTRLIST *pstOldDyn)
{
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	int iTmp1;
	int iTmp2;

	if (	pstPlayer->stRoleData.Perm > 0 ||
		(pstPlayer->UinFlag & UIN_FLAG_GM_ALL) )
	{
		return 0;
	}

	if (pstDyn->stDefChg.iDefHP != pstOldDyn->stDefChg.iDefHP)
	{
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_DIKANG_LVL, pstDyn->stDefChg.iDefHP);
	}

	if (pstDyn->stDefChg.iIgnoreDef != pstOldDyn->stDefChg.iIgnoreDef)
	{
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_POFANG_LVL, pstDyn->stDefChg.iIgnoreDef);
	}

	if (pstDyn->stMidAttrList.uiHPMax != pstOldDyn->stMidAttrList.uiHPMax)
	{
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_HP, pstDyn->stMidAttrList.uiHPMax);	
	}

	if (pstDyn->stMidAttrList.uiMPMax != pstOldDyn->stMidAttrList.uiMPMax)
	{
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_MP, pstDyn->stMidAttrList.uiMPMax);	
	}

	if (pstDyn->stHitRateChg.nHeavyHitVal != pstOldDyn->stHitRateChg.nHeavyHitVal)
	{
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_BAOJI_HURT, pstDyn->stHitRateChg.nHeavyHitVal);	
	}
	
	if (pstDyn->stHitRateChg.iHeavyHit != pstOldDyn->stHitRateChg.iHeavyHit)
	{
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_BAOJI_LVL, pstDyn->stHitRateChg.iHeavyHit);	
	}

	// 物理攻击
	iTmp1 = z_calc_player_attr_atk(pstDyn, ATTR_ID_PHSIC_MAX_ATK);
	iTmp2 = z_calc_player_attr_atk(pstOldDyn, ATTR_ID_PHSIC_MAX_ATK);

	if (	iTmp1 != iTmp2)
	{
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_WULI_ATK, iTmp1);	
	}

	// 物理防御
	iTmp1 = z_cacl_attr_def(&pstDyn->stDefChg, SKILL_ATTR_PHSIC);
	iTmp2 = z_cacl_attr_def(&pstOldDyn->stDefChg, SKILL_ATTR_PHSIC);
	if (iTmp1 != iTmp2)
	{
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_WULI_DEF, iTmp1);	
	}

	// 魔法攻击
	iTmp1 =  z_calc_player_attr_atk(pstDyn, ATTR_ID_MAGIC_MAX_ATK);
	iTmp2 =z_calc_player_attr_atk(pstOldDyn, ATTR_ID_MAGIC_MAX_ATK);
	if (iTmp1 != iTmp2)
	{
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_MOFA_ATK, iTmp1);	
	}

	// 魔法防御
	iTmp1 = z_cacl_attr_def(&pstDyn->stDefChg, SKILL_ATTR_ICE); 
	iTmp2 = z_cacl_attr_def(&pstOldDyn->stDefChg, SKILL_ATTR_ICE); 
	if (iTmp1 != iTmp2)
	{
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_MOFA_DEF, iTmp1);	
	}

	// 治疗加成
	if (pstDyn->stHealChg.nHealHPMul != pstOldDyn->stHealChg.nHealHPMul)
	{
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_ZHILIAO_ADD, pstDyn->stHealChg.nHealHPMul);	
	}
	return 0;
}

int range_single_set(ZONESVRENV* pstEnv, Player *pstPlayer, int iSubType, unsigned int  uiVal)
{
	TOPRANGE stRange;
	ZoneClan *pstClan;

	if (	pstPlayer->stRoleData.Perm > 0 ||
		(pstPlayer->UinFlag & UIN_FLAG_GM_ALL) )
	{
		return 0;
	}
	
	stRange.MainType = RANGE_MAIN_SINGLE;
	stRange.SubType = iSubType;
	stRange.TopRange = -1;
	stRange.RangeData.Career = pstPlayer->stRoleData.Career;
	STRNCPY(stRange.RangeData.FirstName, pstPlayer->stRoleData.RoleName, sizeof(stRange.RangeData.FirstName));
	stRange.RangeData.GID = pstPlayer->stRoleData.RoleID;
	stRange.RangeData.LastRange = -1;
	stRange.RangeData.Level = pstPlayer->stRoleData.Level;
	stRange.RangeData.RangeVal = uiVal;

	if(pstEnv->iInstMode == INST_MODE_ZONE_SVR)
	{
		pstClan = player_get_clan(pstEnv, pstPlayer);
		if (pstClan)
		{
			STRNCPY(stRange.RangeData.SecondName, pstClan->stClan.ClanProf.Name, 
						sizeof(stRange.RangeData.SecondName));
		}
		else
		{
			stRange.RangeData.SecondName[0] = 0;
		}
	}
	else
	{
		if (pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID > 0)
		{
			STRNCPY(stRange.RangeData.SecondName, pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Name, 
						sizeof(stRange.RangeData.SecondName));
		}
		else
		{
			stRange.RangeData.SecondName[0] = 0;
		}
	}
	
	stRange.RangeData.Val1 = 0;
	
	return range_set(pstEnv, &stRange);
}

int range_career_set(ZONESVRENV* pstEnv, Player *pstPlayer, unsigned int  uiVal)
{
	TOPRANGE stRange;
	ZoneClan *pstClan;

	if (	pstPlayer->stRoleData.Perm > 0 ||
		(pstPlayer->UinFlag & UIN_FLAG_GM_ALL) )
	{
		return 0;
	}
	
	stRange.MainType = RANGE_MAIN_CAREER;
	stRange.SubType = (pstPlayer->stRoleData.Career & 0x0f) - 1;
	stRange.TopRange = -1;
	stRange.RangeData.Career = pstPlayer->stRoleData.Career;
	STRNCPY(stRange.RangeData.FirstName, pstPlayer->stRoleData.RoleName, sizeof(stRange.RangeData.FirstName));
	stRange.RangeData.GID = pstPlayer->stRoleData.RoleID;
	stRange.RangeData.LastRange = -1;
	stRange.RangeData.Level = pstPlayer->stRoleData.Level;
	stRange.RangeData.RangeVal = uiVal;

	if(pstEnv->iInstMode == INST_MODE_ZONE_SVR)
	{
		pstClan = player_get_clan(pstEnv, pstPlayer);
		if (pstClan)
		{
			STRNCPY(stRange.RangeData.SecondName, pstClan->stClan.ClanProf.Name, 
						sizeof(stRange.RangeData.SecondName));
		}
		else
		{
			stRange.RangeData.SecondName[0] = 0;
		}
	}
	else
	{
		if (pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID > 0)
		{
			STRNCPY(stRange.RangeData.SecondName, pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Name, 
						sizeof(stRange.RangeData.SecondName));
		}
		else
		{
			stRange.RangeData.SecondName[0] = 0;
		}
	}
	
	stRange.RangeData.Val1 = 0;
	
	return range_set(pstEnv, &stRange);
}

int range_clan_form_data(ZoneClan *pstZoneClan, TOPRANGE *pstRange, int iSubType, unsigned int  uiVal)
{
	CLANPROF *pstClanProf = &pstZoneClan->stClan.ClanProf;
		
	pstRange->MainType = RANGE_MAIN_CLAN;
	pstRange->SubType = iSubType;
	pstRange->TopRange = -1;
	pstRange->RangeData.Career = 0;
	STRNCPY(pstRange->RangeData.FirstName, pstClanProf->Name, sizeof(pstRange->RangeData.FirstName));
	pstRange->RangeData.GID = pstClanProf->GID;
	pstRange->RangeData.LastRange = -1;
	pstRange->RangeData.Level = pstClanProf->Level;
	pstRange->RangeData.RangeVal = uiVal;
	STRNCPY(pstRange->RangeData.SecondName, pstZoneClan->stClan.ClanBrief.Leader.RoleName, 
					sizeof(pstRange->RangeData.SecondName));

	return 0;
}

int range_clan_set(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan, int iSubType, unsigned int  uiVal)
{
	TOPRANGE stRange;
	
	range_clan_form_data(pstZoneClan, &stRange, iSubType, uiVal);
		
	return range_set(pstEnv, &stRange);
}

int range_pet_set(ZONESVRENV* pstEnv, Player *pstOwner, ROLEPET *pstRolePet, PETDEF *pstPetDef, int iSubType, unsigned int  uiVal)
{
	TOPRANGE stRange;
			
	stRange.MainType = RANGE_MAIN_PET;
	stRange.SubType = iSubType;
	stRange.TopRange = -1;
	stRange.RangeData.Career = pstPetDef->PetAttrType;
	STRNCPY(stRange.RangeData.FirstName, pstPetDef->Name, sizeof(stRange.RangeData.FirstName));
	stRange.RangeData.GID = pstRolePet->WID;
	stRange.RangeData.LastRange = -1;
	stRange.RangeData.Level = pstRolePet->Level;
	stRange.RangeData.RangeVal = uiVal;
	STRNCPY(stRange.RangeData.SecondName, pstOwner->stRoleData.RoleName, 
					sizeof(stRange.RangeData.SecondName));
	stRange.RangeData.Val1 = 0;
		
	return range_set(pstEnv, &stRange);
}

int range_combat_set(ZONESVRENV* pstEnv, Player *pstPlayer, int iSubType, 
							int iPworldLevel, char *pszPworldName, unsigned int  uiVal)
{
	TOPRANGE stRange;
			
	stRange.MainType = RANGE_MAIN_COMBAT;
	stRange.SubType = iSubType;
	stRange.TopRange = -1;
	stRange.RangeData.Career = pstPlayer->stRoleData.Career;
	STRNCPY(stRange.RangeData.FirstName, pstPlayer->stRoleData.RoleName, sizeof(stRange.RangeData.FirstName));
	stRange.RangeData.GID = pstPlayer->stRoleData.RoleID;
	stRange.RangeData.LastRange = -1;
	stRange.RangeData.Level = iPworldLevel;
	stRange.RangeData.RangeVal = uiVal;
	STRNCPY(stRange.RangeData.SecondName, pszPworldName, 
					sizeof(stRange.RangeData.SecondName));
	stRange.RangeData.Val1 = 0;
		
	return range_set(pstEnv, &stRange);
}

int range_battle_set(ZONESVRENV* pstEnv, Player *pstPlayer, int iSubType, 
							int iPworldLevel, char *pszPworldName, unsigned int  uiVal)
{
	TOPRANGE stRange;
			
	stRange.MainType = RANGE_MAIN_BATTLE;
	stRange.SubType = iSubType;
	stRange.TopRange = -1;
	stRange.RangeData.Career = pstPlayer->stRoleData.Career;
	STRNCPY(stRange.RangeData.FirstName, pstPlayer->stRoleData.RoleName, sizeof(stRange.RangeData.FirstName));
	stRange.RangeData.GID = pstPlayer->stRoleData.RoleID;
	stRange.RangeData.LastRange = -1;
	stRange.RangeData.Level = iPworldLevel;
	stRange.RangeData.RangeVal = uiVal;
	STRNCPY(stRange.RangeData.SecondName, pszPworldName, 
					sizeof(stRange.RangeData.SecondName));
	stRange.RangeData.Val1 = 0;
	return range_set(pstEnv, &stRange);
}

int range_sparta_set(ZONESVRENV* pstEnv, Player *pstPlayer, int iSubType)
{
	int iRate;
	TOPRANGE stRange;
			
	stRange.MainType = RANGE_MAIN_SPARTA;
	stRange.SubType = iSubType;
	stRange.TopRange = -1;
	stRange.RangeData.Career = pstPlayer->stRoleData.Career;
	STRNCPY(stRange.RangeData.FirstName, pstPlayer->stRoleData.RoleName, sizeof(stRange.RangeData.FirstName));
	stRange.RangeData.GID = pstPlayer->stRoleData.RoleID;
	stRange.RangeData.LastRange = -1;
	stRange.RangeData.Level = pstPlayer->stRoleData.Level;
	stRange.RangeData.RangeVal = pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.FightVal;
	iRate = player_sparta_victory_rate(pstPlayer);
	sprintf(stRange.RangeData.SecondName, "%d %%", iRate);
	stRange.RangeData.Val1 = pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.RunAway;
	stRange.RangeData.Val2 = pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.TotalWeek;
	return range_set(pstEnv, &stRange);
}

static int RangeHashNameCmp(const void *pv1 , const void *pv2)
{
	const RangeHashName *pstLhs = pv1, *pstRhs = pv2;

	if (pstLhs->GID == pstRhs->GID)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

static unsigned int RangeHashNameCode(const void* pvData)
{
	const RangeHashName *pstHash = pvData;
	return pstHash->GID;
}

char *range_get_name(TOPRANGE *pstRange)
{
	switch(pstRange->MainType)
	{
		case RANGE_MAIN_SINGLE:
		case RANGE_MAIN_CAREER:	
		case RANGE_MAIN_COMBAT:
		case RANGE_MAIN_BATTLE:	
		case RANGE_MAIN_SPARTA:
		case RANGE_MAIN_ATTR:
			return pstRange->RangeData.FirstName;
			break;
		case RANGE_MAIN_PET:
		case RANGE_MAIN_CLAN:	
			return pstRange->RangeData.SecondName;
			break;
		default:
			return NULL;
			break;
			
	}

	return NULL;
}

char *range_get_clanname(TOPRANGE *pstRange, int iSingle)
{
	switch(pstRange->MainType)
	{
		case RANGE_MAIN_SINGLE:
		case RANGE_MAIN_CAREER:	
		case RANGE_MAIN_ATTR:
			return pstRange->RangeData.SecondName;
			break;
		case RANGE_MAIN_CLAN:
			if (iSingle)
			{
				return NULL;
			}
			
			return pstRange->RangeData.FirstName;
			break;
		case RANGE_MAIN_COMBAT:
			if (pstRange->SubType == RANGE_MAIN_COMBAT_SUB_BCT)
			{
				return pstRange->RangeData.SecondName;
			}
			else
			{
				return NULL;
			}
			break;
		default:
			return NULL;
			break;
			
	}

	return NULL;
}


int range_page_version_chg(RangeOne *pstLast, RangeOne *pstCurr)
{
	int i, iStartPage, iEndPage;
	
	for (i=0; i<pstLast->iNum && i<pstCurr->iNum; i++)
	{
		/*
		if (0 != range_compare(&pstLast->astRangeEntry[i].stRange, &pstCurr->astRangeEntry[i].stRange) ||
			0 != range_unique(&pstLast->astRangeEntry[i].stRange, &pstCurr->astRangeEntry[i].stRange))
		{
			break;
		}*/

		if (0 != memcmp(&pstLast->astRangeEntry[i].stRange, &pstCurr->astRangeEntry[i].stRange, 
			sizeof(pstLast->astRangeEntry[i].stRange)))
		{
			break;
		}
	}

	if (i == pstLast->iNum && pstLast->iNum == pstCurr->iNum)
	{
		return 0;
	}

	iStartPage = i / RANGE_MAX_PAGE_ENTRY;
	iEndPage = ((pstCurr->iNum > pstLast->iNum?pstCurr->iNum:pstLast->iNum) - 1)/RANGE_MAX_PAGE_ENTRY;

	for (i=iStartPage; i<=iEndPage && i<RANGE_MAX_PAGE; i++)
	{
		pstCurr->aiPageVersion[i] += 1;
	}

	return 0;
}

int range_do_chg_clan(ZONESVRENV* pstEnv, RangeOneMem *pstRangeOneMem)
{
	int i;
	TOPRANGE stRange;
	ZoneClan *pstZoneClan;
	ClanObj *pstClanObj = pstEnv->pstClanObj;
	CSPKG stPkg;
	CSRANGESVR *pstRangeSvr = &stPkg.Body.RangeSvr;
	CSRANGESVRTIP *pstTip = &pstRangeSvr->RangeSvrData.RangeSvrTip;
	RangeOne *pstLast = &pstRangeOneMem->stLastRange;
	RangeOne *pstCurr = &pstRangeOneMem->stCurrRange;
	RangeHashName stRangeHashName;
	RangeHashName *pstRangeHashName;
	int iReplaceFlag;
	GlobalObj *pstGlobalObj = pstEnv->pstGlobalObj;
	int iClanCombatDay;

	pstRangeOneMem->stCurrRange.iNum = 0;

	for (i=0; i<pstLast->iNum; i++)
	{
		pstZoneClan = clan_find_by_wid(pstEnv, pstLast->astRangeEntry[i].stRange.RangeData.GID);
		if (pstZoneClan)
		{
			switch(pstRangeOneMem->iSubType)
			{
				case RANGE_MAIN_CLAN_SUB_DEVELOP:
					range_clan_form_data(pstZoneClan, &stRange, pstRangeOneMem->iSubType,
										clan_develop_val(pstZoneClan));
					break;
				case  RANGE_MAIN_CLAN_SUB_MEM:
					range_clan_form_data(pstZoneClan, &stRange, pstRangeOneMem->iSubType,
										   pstZoneClan->stClan.ClanProf.MemNum);
					break;
				case RANGE_MAIN_CLAN_SUB_LEVEL:
					range_clan_form_data(pstZoneClan, &stRange, pstRangeOneMem->iSubType,
										   pstZoneClan->stClan.ClanProf.Level);
					break;
				case RANGE_MAIN_CLAN_SUB_COMBAT:
					if (is_fini_global(pstEnv) &&
						pstGlobalObj->stGlobal.Misc.WorldCity.CityCore.ClanGID == pstZoneClan->stClan.ClanProf.GID &&
						pstGlobalObj->stGlobal.Misc.WorldCity.CityCore.ClanBeginTime != 0)
					{
						iClanCombatDay = pstEnv->pstAppCtx->stCurr.tv_sec - pstGlobalObj->stGlobal.Misc.WorldCity.CityCore.ClanBeginTime;
						if (0 > iClanCombatDay)
						{
							iClanCombatDay = 0;
						}
						iClanCombatDay = iClanCombatDay/86400 + 1;
						if (iClanCombatDay < (int)pstLast->astRangeEntry[i].stRange.RangeData.RangeVal)
						{
							iClanCombatDay = pstLast->astRangeEntry[i].stRange.RangeData.RangeVal;
						}
						
						range_clan_form_data(pstZoneClan, &stRange, pstRangeOneMem->iSubType, iClanCombatDay);
					}
					else
					{
						range_clan_form_data(pstZoneClan, &stRange, pstRangeOneMem->iSubType,
										   	pstLast->astRangeEntry[i].stRange.RangeData.RangeVal);
					}
					break;
				default:
					return -1;
					break;
			}

			range_set_one_replace(pstEnv, pstCurr, &stRange);
		}
	}
	
	for (i=0; i<pstClanObj->iClanNum; i++)
	{
		iReplaceFlag = 1;
		pstZoneClan = &pstClanObj->astZoneClan[i];

		switch(pstRangeOneMem->iSubType)
		{
			case RANGE_MAIN_CLAN_SUB_DEVELOP:
				range_clan_form_data(pstZoneClan, &stRange, pstRangeOneMem->iSubType,
									clan_develop_val(pstZoneClan));
				break;
			case  RANGE_MAIN_CLAN_SUB_MEM:
				range_clan_form_data(pstZoneClan, &stRange, pstRangeOneMem->iSubType,
									   pstZoneClan->stClan.ClanProf.MemNum);
				break;
			case RANGE_MAIN_CLAN_SUB_LEVEL:
				range_clan_form_data(pstZoneClan, &stRange, pstRangeOneMem->iSubType,
									   pstZoneClan->stClan.ClanProf.Level);
				break;
			case RANGE_MAIN_CLAN_SUB_COMBAT:
				if (is_fini_global(pstEnv) &&
						pstGlobalObj->stGlobal.Misc.WorldCity.CityCore.ClanGID == pstZoneClan->stClan.ClanProf.GID &&
						pstGlobalObj->stGlobal.Misc.WorldCity.CityCore.ClanBeginTime != 0)
				{
					iClanCombatDay = pstEnv->pstAppCtx->stCurr.tv_sec - pstGlobalObj->stGlobal.Misc.WorldCity.CityCore.ClanBeginTime;
					if (0 > iClanCombatDay)
					{
						iClanCombatDay = 0;
					}
					iClanCombatDay = iClanCombatDay/86400 + 1;
					if (iClanCombatDay < (int)pstLast->astRangeEntry[i].stRange.RangeData.RangeVal)
					{
						iClanCombatDay = pstLast->astRangeEntry[i].stRange.RangeData.RangeVal;
					}

					range_clan_form_data(pstZoneClan, &stRange, pstRangeOneMem->iSubType, iClanCombatDay);
				}
				else
				{
					iReplaceFlag = 0;
				}
				break;
			default:
				return -1;
				break;
		}

		if (iReplaceFlag)
		{
			range_set_one_replace(pstEnv, pstCurr, &stRange);
		}
	}

	sht_init(pstEnv->pstShtRangeName, pstEnv->pstShtRangeName->iBuff, pstEnv->pstShtRangeName->iBucket, 
			pstEnv->pstShtRangeName->iMax, pstEnv->pstShtRangeName->iDataUnitMin);

	Z_CSHEAD_INIT(&stPkg.Head, RANGE_SVR);
	pstRangeSvr->RangeSvrOp = RANGE_SVR_TIP;

	for (i=0; i<pstCurr->iNum; i++)
	{
		pstCurr->astRangeEntry[i].iDirty = 1;
		
		pstCurr->astRangeEntry[i].stRange.RangeData.LastRange = -1;

		pstCurr->astRangeEntry[i].stRange.TopRange = i+1;

		stRangeHashName.iIdx = i;
		stRangeHashName.GID = pstCurr->astRangeEntry[i].stRange.RangeData.GID;

		pstRangeHashName = sht_insert_unique(pstEnv->pstShtRangeName, &stRangeHashName, RangeHashNameCmp, RangeHashNameCode);
		if (pstRangeHashName)
		{
			memcpy(pstRangeHashName, &stRangeHashName, sizeof(RangeHashName));
		}
	}

	for (i=0; i<pstLast->iNum; i++)
	{
		stRangeHashName.GID = pstLast->astRangeEntry[i].stRange.RangeData.GID;
		pstRangeHashName = sht_find(pstEnv->pstShtRangeName, &stRangeHashName, RangeHashNameCmp, RangeHashNameCode);
		if (pstRangeHashName)
		{
			pstCurr->astRangeEntry[pstRangeHashName->iIdx].stRange.RangeData.LastRange = i+1;
		}
		else
		{
			//落榜通知
			pstZoneClan = clan_find_by_wid(pstEnv, pstLast->astRangeEntry[i].stRange.RangeData.GID);
			if (pstZoneClan)
			{
				pstTip->MainType = pstRangeOneMem->iMainType;
				pstTip->SubType = pstRangeOneMem->iSubType;
				pstTip->CurrRange = -1;
				pstTip->LastRange = i+1;

				clan_broadcast_if(pstEnv, pstZoneClan, &stPkg, ClanEveryBody);
			}
		}
	}

	//上榜通知
	for (i=0; i<pstCurr->iNum; i++)
	{
		pstZoneClan = clan_find_by_wid(pstEnv, pstCurr->astRangeEntry[i].stRange.RangeData.GID);
		if (pstZoneClan)
		{
			pstTip->MainType = pstRangeOneMem->iMainType;
			pstTip->SubType = pstRangeOneMem->iSubType;
			pstTip->CurrRange = i+1;
			pstTip->LastRange = pstCurr->astRangeEntry[i].stRange.RangeData.LastRange;

			clan_broadcast_if(pstEnv, pstZoneClan, &stPkg, ClanEveryBody);
		}
	}

	range_page_version_chg(pstLast, pstCurr);

	memcpy(pstLast, pstCurr, sizeof(*pstLast));

	return 0;
}

int range_do_chg_one(ZONESVRENV* pstEnv, RangeOneMem *pstRangeOneMem)
{
	int i;
	CSPKG stPkg;
	CSRANGESVR *pstRangeSvr = &stPkg.Body.RangeSvr;
	CSRANGESVRTIP *pstTip = &pstRangeSvr->RangeSvrData.RangeSvrTip;
	RangeOne *pstLast = &pstRangeOneMem->stLastRange;
	RangeOne *pstCurr = &pstRangeOneMem->stCurrRange;
	RangeHashName stRangeHashName;
	RangeHashName *pstRangeHashName;
	Player *pstPlayer;
	char *pszRoleName;

	//公会的某些排行需要特殊处理
	if (pstRangeOneMem->iMainType == RANGE_MAIN_CLAN)
	{
		return range_do_chg_clan(pstEnv, pstRangeOneMem);
	}

	range_do_chg_weekcharm_pre(pstEnv, pstRangeOneMem);
	range_do_chg_sparta_pre(pstEnv,pstRangeOneMem);

	sht_init(pstEnv->pstShtRangeName, pstEnv->pstShtRangeName->iBuff, pstEnv->pstShtRangeName->iBucket, 
			pstEnv->pstShtRangeName->iMax, pstEnv->pstShtRangeName->iDataUnitMin);

	Z_CSHEAD_INIT(&stPkg.Head, RANGE_SVR);
	pstRangeSvr->RangeSvrOp = RANGE_SVR_TIP;

	for (i=0; i<pstCurr->iNum; i++)
	{
		pstCurr->astRangeEntry[i].iDirty = 1;
		
		pstCurr->astRangeEntry[i].stRange.RangeData.LastRange = -1;

		//pstCurr->astRangeEntry[i].stRange.MainType = pstRangeOneMem->iMainType;

		//pstCurr->astRangeEntry[i].stRange.SubType = pstRangeOneMem->iSubType;

		pstCurr->astRangeEntry[i].stRange.TopRange = i+1;

		stRangeHashName.iIdx = i;
		stRangeHashName.GID = pstCurr->astRangeEntry[i].stRange.RangeData.GID;

		pstRangeHashName = sht_insert_unique(pstEnv->pstShtRangeName, &stRangeHashName, RangeHashNameCmp, RangeHashNameCode);
		if (pstRangeHashName)
		{
			memcpy(pstRangeHashName, &stRangeHashName, sizeof(RangeHashName));
		}
	}
	
	for (i=0; i<pstLast->iNum; i++)
	{
		pszRoleName = range_get_name(&pstLast->astRangeEntry[i].stRange);
		if (!pszRoleName)
		{
			continue;
		}
		
		stRangeHashName.GID = pstLast->astRangeEntry[i].stRange.RangeData.GID;
		pstRangeHashName = sht_find(pstEnv->pstShtRangeName, &stRangeHashName, RangeHashNameCmp, RangeHashNameCode);
		if (pstRangeHashName)
		{
			pstCurr->astRangeEntry[pstRangeHashName->iIdx].stRange.RangeData.LastRange = i+1;
		}
		else
		{
			pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pszRoleName);
			if (pstPlayer)
			{
				pstTip->MainType = pstRangeOneMem->iMainType;
				pstTip->SubType = pstRangeOneMem->iSubType;
				pstTip->CurrRange = -1;
				pstTip->LastRange = i+1;

				z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
			}
		}
	}

	//上榜通知
	//for (i=0; i<pstCurr->iNum; i++)
	for (i=pstCurr->iNum-1; i>=0; i--)	
	{
		pszRoleName = range_get_name(&pstCurr->astRangeEntry[i].stRange);
		if (!pszRoleName)
		{
			continue;
		}
		
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pszRoleName);
		if (pstPlayer)
		{
			pstTip->MainType = pstRangeOneMem->iMainType;
			pstTip->SubType = pstRangeOneMem->iSubType;
			pstTip->CurrRange = i+1;
			pstTip->LastRange = pstCurr->astRangeEntry[i].stRange.RangeData.LastRange;

			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		}
	}

	range_page_version_chg(pstLast, pstCurr);
	
	memcpy(pstLast, pstCurr, sizeof(*pstLast));
	
	return 0;
}

int range_db_delete(ZONESVRENV* pstEnv)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMDELETEREQ *pstDeleteReq = &pstPkg->Body.DeleteReq;
	TOPRANGE stRange;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;

	pstEnv->pstZoneObj->tLastRange = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	stDBAsyn.iCmd = DB_RANGE_DELETE;
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_DELETE_REQ, "TopRange", -1, &stDBAsyn);
	pstDeleteReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDeleteReq->OperInfo.EntryNameList.EntryNameNum = 0;
	strcpy(pstDeleteReq->OperInfo.WhereDef, "where TopRange != 0");
			
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRange;
	stMetaData.iBuff = sizeof(stRange);
	
	memset(&stRange, 0, sizeof(stRange));
			
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}

	pstEnv->pstZoneObj->stRange.iState = RANGE_STAT_CHG;
	
	return 0;
}

int range_delete_res(ZONESVRENV* pstEnv, TORMPKG *pstTormPkg)
{
	TORMDELETERES *pstDeleteRes = &pstTormPkg->Body.DeleteRes;
	
	if(TORM_ERR_IS_ERROR(pstDeleteRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "db delete range failed.");
		return -1;
	}

	pstEnv->pstZoneObj->stRange.iState = RANGE_STAT_DB_UPDATE;
	pstEnv->pstZoneObj->tLastRange = pstEnv->pstAppCtx->stCurr.tv_sec;

	range_db_update(pstEnv);

	return 0;
}

int range_do_chg_weekcharm_pre(ZONESVRENV* pstEnv, RangeOneMem *pstRangeOneMem)
{
	RangeOne *pstCurr = &pstRangeOneMem->stCurrRange;
//	int i;
	time_t t = pstEnv->pstAppCtx->stCurr.tv_sec;
//	Player *pstPlayer;

	if (pstRangeOneMem->iMainType != RANGE_MAIN_SINGLE||
		pstRangeOneMem->iSubType != RANGE_MAIN_SINGLE_SUB_CHARM_WEEK)
	{
		return 0;
	}

	if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat != GLOBAL_FINI)
	{
		return 0;
	}

	if (is_same_week(pstEnv->pstGlobalObj->stGlobal.Misc.WeekCharmTm, t))
	{
		return 0;
	}
	
	range_weekcharm_award_dsn(pstEnv);
	
	pstEnv->pstGlobalObj->stGlobal.Misc.WeekCharmTm = t;

	pstCurr->iNum = 0;
	/*for (i=0; i<pstCurr->iNum; i++)
	{
		pstCurr->astRangeEntry[i].stRange.RangeData.RangeVal = 0;
	}*/
	return 0;
}

int range_do_chg_sparta_pre(ZONESVRENV* pstEnv, RangeOneMem *pstRangeOneMem)
{
	RangeOne *pstCurr = &pstRangeOneMem->stCurrRange;
	int i;
	time_t t = pstEnv->pstAppCtx->stCurr.tv_sec;
	Player *pstPlayer;
	RangeOne stTmp;

	if (pstRangeOneMem->iMainType != RANGE_MAIN_SPARTA ||
		pstRangeOneMem->iSubType != RANGE_MAIN_SPARTA_SUB_FIGHT)
	{
		return 0;
	}

	if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat != GLOBAL_FINI)
	{
		return 0;
	}

	if (pstEnv->pstZoneObj->stSpartaInfo.iNum < 150)
	{
		return 0;
	}

	if (is_same_real_week(pstEnv->pstGlobalObj->stGlobal.Misc.RangeSparteTm, t))
	{
		return 0;
	}

	pstEnv->pstGlobalObj->stGlobal.Misc.RangeSparteTm = t;

	for (i=0; i<pstCurr->iNum; i++)
	{
		if (pstCurr->astRangeEntry[i].stRange.RangeData.Val2 < 10)
		{	
			if (pstCurr->astRangeEntry[i].stRange.RangeData.RangeVal < INIT_SPARTA_FIGHT_VAL+100)
			{
				pstCurr->astRangeEntry[i].stRange.RangeData.RangeVal = INIT_SPARTA_FIGHT_VAL;
			}
			else
			{
				pstCurr->astRangeEntry[i].stRange.RangeData.RangeVal -= 100;
			}
			
			pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstCurr->astRangeEntry[i].stRange.RangeData.FirstName);
			if (pstPlayer)
			{
				pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.FightVal = pstCurr->astRangeEntry[i].stRange.RangeData.RangeVal;
				player_sparta_info(pstEnv, pstPlayer);	
			}
		}
		pstCurr->astRangeEntry[i].stRange.RangeData.Val2 = 0;
	}

	//重排
	memcpy(&stTmp, pstCurr, sizeof(stTmp));
	pstCurr->iNum = 0;
	for (i=0; i<stTmp.iNum; i++)
	{
		range_set_one_hold(pstEnv, pstCurr, &stTmp.astRangeEntry[i].stRange);
	}
	
	return 0;
}

int range_do_chg(ZONESVRENV* pstEnv)
{
	int i;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	WATCHONE *pstOne = NULL;
	int iValid = 0;

	if (pstObj->stRange.iState != RANGE_STAT_DB_FINI)
	{
		return -1;
	}

	pstEnv->pstZoneObj->tLastRange = pstEnv->pstAppCtx->stCurr.tv_sec;

	for (i=0; i<pstObj->stRange.iRangeNum; i++)
	{
		range_do_chg_one(pstEnv, &pstObj->stRange.astRange[i]);
	}

	range_db_delete(pstEnv);

	for(i = 0; i < pstEnv->pstWatchHash->iMax;i++)
	{
		pstOne = NULL;
		iValid = 0;
		
		pstOne = (WATCHONE *)sht_pos(pstEnv->pstWatchHash, i, &iValid);
		if( iValid && pstOne)
		{	
			sht_remove_by_pos(pstEnv->pstWatchHash, i);
		}
	}

	player_range_dsn(pstEnv);
	strong_check_open(pstEnv);
	world_max_level_broadcast(pstEnv);
	sparta_range_check(pstEnv);
	
	return 0;
}


int player_sparta_in_top_ten(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	RangeOneMem * pstOne;
	int i;
	
	pstOne = range_find_onemem(pstEnv,RANGE_MAIN_SPARTA, RANGE_MAIN_SPARTA_SUB_FIGHT);
	if (!pstOne)
	{
		return 0;
	}

	for (i=0; i<10 && i<pstOne->stLastRange.iNum; i++)
	{
		if (pstOne->stLastRange.astRangeEntry[i].stRange.RangeData.GID == pstPlayer->stRoleData.RoleID)
		{
			return 1;
		}
	}
	return 0;
}

int player_range_dsn(ZONESVRENV* pstEnv)
{
	int i,j;
	RANGDSNDEF *pstDef;
	RangeOneMem * pstOne;
	Player *pstPlayer;
	DESIGNATIONENTRY* pstDsn;
	const DESIGNATIONDEF* pstDsnDef;
	int iNotify = 0;
	

	for (i=0; i<pstEnv->pstZoneObj->iRangDsnDef; i++)
	{
		pstDef = (RANGDSNDEF *)&pstEnv->pstZoneObj->sRangDsnDef[i];
		
		if(pstDef->MainType == RANGE_MAIN_SINGLE && pstDef->SubType == RANGE_MAIN_SINGLE_SUB_CHARM_WEEK)
		{
			//周魅力跳过
			continue;
		}
		
		pstOne = range_find_onemem(pstEnv,pstDef->MainType, pstDef->SubType);
		if (!pstOne)
		{
			continue;
		}

		for (j=0; j<pstDef->Top && j<pstOne->stLastRange.iNum; j++)
		{
			if (j >= MAX_RANGDSN_TOP || pstDef->DsnID[j] <= 0)
				break;

			iNotify = 0;
			pstPlayer = NULL;
			if (	pstDef->MainType == RANGE_MAIN_SINGLE ||
				pstDef->MainType == RANGE_MAIN_CAREER ||
				pstDef->MainType == RANGE_MAIN_COMBAT ||
				pstDef->MainType == RANGE_MAIN_BATTLE ||
				pstDef->MainType == RANGE_MAIN_SPARTA)
			{
				pstPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,
								pstOne->stLastRange.astRangeEntry[j].stRange.RangeData.FirstName);
			}
			else if (pstDef->MainType == RANGE_MAIN_PET)
			{
				pstPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,
								pstOne->stLastRange.astRangeEntry[j].stRange.RangeData.SecondName);
			}

			if (pstPlayer)
			{
				pstDsn =  player_find_designation(pstEnv,pstPlayer,pstDef->DsnID[j]);
				if (pstDsn)
				{
					pstDsnDef = z_designation_def(pstEnv, pstDsn->ID);
					if (pstDsnDef && pstDsnDef->DesTime > 0)
					{
						pstDsn->EndTime = pstEnv->pstAppCtx->stCurr.tv_sec+pstDsnDef->DesTime;
						iNotify = 1;
					}
				}
				else
				{
					g_iDesigVar = 1;
					player_designation_add(pstEnv, pstPlayer,pstDef->DsnID[j]);
				}

				if (iNotify)
				{
					player_designation_info(pstEnv, pstPlayer);
				}
			}
		}
	}

	return 0;
}

int player_range_page(ZONESVRENV* pstEnv, Player *pstPlayer, CSRANGECLTPAGE *pstRangeCltPage)
{
	int i, iPageStart;
	CSPKG stPkg;
	CSRANGESVR *pstRangeSvr =&stPkg.Body.RangeSvr;
	CSRANGESVRPAGE *pstRangePage = &pstRangeSvr->RangeSvrData.RangeSvrPage;
	RangeOneMem *pstRangeOneMem;
	RangeOne *pstLast;

	pstRangeOneMem = range_find_onemem(pstEnv, pstRangeCltPage->MainType, pstRangeCltPage->SubType);
	if (NULL == pstRangeOneMem)
	{
		return -1;
	}

	if (pstRangeCltPage->PageIdx < 1 || pstRangeCltPage->PageIdx > RANGE_MAX_PAGE)
	{
		return -1;
	}

	pstLast = &pstRangeOneMem->stLastRange;

	if (pstRangeCltPage->PageIdx > (pstLast->iNum + RANGE_MAX_PAGE_ENTRY -1)/RANGE_MAX_PAGE_ENTRY)
	{
		return 0;
	}

	Z_CSHEAD_INIT(&stPkg.Head, RANGE_SVR);
	pstRangeSvr->RangeSvrOp = RANGE_SVR_PAGE;

	pstRangePage->MainType = pstRangeCltPage->MainType;
	pstRangePage->SubType = pstRangeCltPage->SubType;
	pstRangePage->TotalNum = pstLast->iNum;
	pstRangePage->PageIdx = pstRangeCltPage->PageIdx;
	pstRangePage->PageVersion = pstLast->aiPageVersion[pstRangeCltPage->PageIdx-1];
	pstRangePage->EntryNum = 0;

	if (pstRangeCltPage->PageVersion == pstLast->aiPageVersion[pstRangeCltPage->PageIdx-1])
	{
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		return 0;
	}
	
	iPageStart = (pstRangeCltPage->PageIdx-1)*RANGE_MAX_PAGE_ENTRY;

	for (i=0; i<RANGE_MAX_PAGE_ENTRY; i++)
	{
		if (iPageStart >= pstLast->iNum)
		{
			break;
		}
		
		memcpy(&pstRangePage->Entrys[pstRangePage->EntryNum++], &pstLast->astRangeEntry[iPageStart++].stRange,
				sizeof(pstRangePage->Entrys[0]));
	}

	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		
	return 0;
}


int player_watch_offline_in(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong ullPetWID, 
								WATCHONE *pstOne)
{
	CSPKG stPkg;
	int i;
	
	if (ullPetWID == 0)
	{
		CSWATCHPLAYERSVR *pstRes = &stPkg.Body.WatchPlayerSvr;

		*pstRes = pstOne->Player;
		pstRes->PlayerMemID = 0;
		Z_CSHEAD_INIT(&stPkg.Head, WATCH_PLAYER_SVR);
		return z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	else
	{
		CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
		CSPETWATCHINFO *pstWatchInfo = &pstPetSvr->PetSvrData.WatchInfo;

		for (i=0; i<pstOne->Pets.Num; i++)
		{
			if (pstOne->Pets.RolePets[i].WID == ullPetWID)
			{
				pstPetSvr->PetSvrType = PET_SVR_WATCH;
				STRNCPY(	pstWatchInfo->RoleName, 
							pstOne->RoleName, 
							CCH(pstWatchInfo->RoleName));

				memcpy(&pstWatchInfo->Pet, &pstOne->Pets.RolePets[i], sizeof(pstWatchInfo->Pet));

				Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
				z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
				break;
			}
		}

		if (i >= pstOne->Pets.Num)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
								ZONEERR_RANGE201);
		}
	}
	return 0;
}


int player_watch_select_db(ZONESVRENV* pstEnv, Player *pstPlayer, char *pszRoleName, 
								tdr_ulonglong ullPetWID)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstDBReq = &pstPkg->Body.SelectReq;

	DBAsyn stDBAsyn;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	ROLEDATA stKey;
		
	stDBAsyn.iCmd = DB_SELECT_WATCH_RES;
	stDBAsyn.AsynCmd.stWatchRole.ullPetWid = ullPetWID;
	stDBAsyn.AsynCmd.stWatchRole.ullRoleID = pstPlayer->stRoleData.RoleID;
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, "RoleData", 
						pstPlayer->stRoleData.Uin, &stDBAsyn);
		
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OperInfo.WhereDef[0] = 0;
	snprintf(&pstDBReq->OperInfo.WhereDef[0], sizeof(pstDBReq->OperInfo.WhereDef), 
						"WHERE RoleName='%s'", pszRoleName);
	pstDBReq->OffSet = 0;
	pstDBReq->Limit = 1;

	memset(&stKey, 0, sizeof(stKey));
	stKey.Uin = pstPlayer->stRoleData.Uin;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stKey;
	stMetaData.iBuff = sizeof(stKey);
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}	
	return 0;
}

int player_watch_offline_player(ZONESVRENV* pstEnv, Player *pstPlayer, char *pszRoleName, 
									tdr_ulonglong ullPetWID)
{
	WATCHONE stOne;
	WATCHONE *pstOne;
	
	STRNCPY(stOne.RoleName, pszRoleName, sizeof(stOne.RoleName));
	pstOne = (WATCHONE *)sht_find(pstEnv->pstWatchHash, &stOne, watchcmp, watchcode);
	if (pstOne)
	{
		player_watch_offline_in(pstEnv, pstPlayer, ullPetWID, pstOne);	
	}

	// 找不到的查表。
	else
	{
		player_watch_select_db(pstEnv, pstPlayer, pszRoleName, ullPetWID);
	}
	
	return 0;
}

int player_save_watch_cache(WATCHONE *pstOne, ROLEDATA *pstRoleData)
{
	pstOne->Pets = pstRoleData->Package.PetPackage;
	pstOne->Player.PlayerMemID = 0;
	pstOne->Player.RoleAttrVal = pstRoleData->MiscInfo.AttrVal;
	pstOne->Player.RoleBaseAttrVal = pstRoleData->MiscInfo.BaseAttrVal;
	pstOne->Player.RoleCareer = pstRoleData->Career;
	pstOne->Player.RoleFace = pstRoleData->Face;
	pstOne->Player.RoleGender = pstRoleData->Gender;
	pstOne->Player.RoleHair = pstRoleData->Hair;
	pstOne->Player.RoleHead = pstRoleData->Head;
	pstOne->Player.RoleLevel = pstRoleData->Level;
	pstOne->Player.RoleMiscFlag = pstRoleData->MiscInfo.Flag;
	pstOne->Player.RoleWear = pstRoleData->Wear;
	pstOne->Player.SparPkg = pstRoleData->Package.SparPkg;
	pstOne->Player.ClanWID = pstRoleData->MiscInfo.ClanInfo.ClanId.GID;
	pstOne->Player.Genius = genius_get_mod1(pstRoleData);
	pstOne->Player.DaoBaInfo = pstRoleData->MiscInfo.DaoBaInfo;
	pstOne->Player.GeniusInfo = pstRoleData->MiscInfo.GeniusInfo;
	strncpy(pstOne->Player.RoleName, pstRoleData->RoleName, sizeof(pstOne->Player.RoleName));
	strncpy(pstOne->RoleName, pstRoleData->RoleName, sizeof(pstOne->RoleName));	
	return 0;
}

int player_logout_save_watch_cache(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	WATCHONE stOne;
	WATCHONE *pstOne;

	strncpy(stOne.RoleName, pstPlayer->stRoleData.RoleName, sizeof(stOne.RoleName));
	pstOne =  (WATCHONE *)sht_find(pstEnv->pstWatchHash, &stOne, watchcmp, watchcode);
	if (pstOne)
	{
		player_save_watch_cache(pstOne, &pstPlayer->stRoleData);
	}
	return 0;
}

int z_db_watch_select_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
								TORMSELECTRES *pstSelectRes, tdr_ulonglong ullPetWID)
{
//	CSPKG stPkg;
//	int i;
	WATCHONE stOne;
	WATCHONE *pstOne;
	ROLEDATA stRoleData;

	if ( pstSelectRes->SelectResult.ResultNum == 1)
	{
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stRoleData, sizeof(stRoleData)))
		{
			return -1;
		}
	}
	else
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RANGE204);
		return -1;
	}
	
	strncpy(stOne.RoleName, stRoleData.RoleName, sizeof(stOne.RoleName));	
	pstOne = sht_insert_unique(pstEnv->pstWatchHash ,&stOne, watchcmp, watchcode);
	if (pstOne)
	{
		player_save_watch_cache(pstOne, &stRoleData);
	}
	else
	{
		return -1;
	}
	player_watch_offline_in(pstEnv, pstPlayer, ullPetWID, pstOne);
	return 0;
}

int player_range_watch_player(ZONESVRENV* pstEnv, Player *pstPlayer, CSRANGECLTWATCHPLAYER *pstClt)
{
	Player *pstTarget;

	if(INST_MODE_ZONE_SVR==pstEnv->iInstMode )
	{
		if(0==player_check_same_world(pstEnv,pstClt->RoleName))
		{
			battle_rolename_remove_prefix(pstClt->RoleName);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
									ZONEERR_RANGE202);
			return 0;
		}
	}
	else
	{
		char szName[NAME_LEN];
		unsigned short unWorldID;

		if((strstr(pstClt->RoleName, BATTLE_NAME_SYMBOL) == NULL))
		{
			unWorldID = role_world_id_get(pstPlayer);
			snprintf(szName,sizeof(szName),"w%d"BATTLE_NAME_SYMBOL"%s",
					unWorldID,
					pstClt->RoleName);

			STRNCPY(pstClt->RoleName, szName, CCH(pstClt->RoleName));
		}

		pstTarget = z_name_player(pstEnv->pstAppCtx, pstEnv, pstClt->RoleName);
		if (!pstTarget || PLAYER_STATUS_ROLE_LOGIN != pstTarget->eStatus)
		{
			SSPKG stSSPkg;
			SSCMDREQ* pstCmdReq = &stSSPkg.Body.CmdReq;
			int iBusID;

			iBusID = player_master_busid_get(pstEnv, pstPlayer);
			
			memset(pstCmdReq,0,sizeof(*pstCmdReq));
			pstCmdReq->Cmd = SS_CMD_RANGE_WATCH_PROXY;
			STRNCPY(pstCmdReq->szName, pstPlayer->stRoleData.RoleName, sizeof(pstCmdReq->szName));
			battle_rolename_remove_prefix(pstCmdReq->szName);
			STRNCPY(pstCmdReq->szReceiver, pstClt->RoleName, sizeof(pstCmdReq->szReceiver));
			battle_rolename_remove_prefix(pstCmdReq->szReceiver);
			
			Z_SSHEAD_INIT(&stSSPkg.Head, SS_CMD_REQ);
			z_ssmsg_send(pstEnv, &stSSPkg, iBusID);

			// 称号
			player_desig_get( pstEnv, pstPlayer, GET_DESIG_WATCH);
		}
		else
		{
			return player_watch_role(pstEnv, pstPlayer, pstTarget);
		}
		
		return 0;
	}
	
	pstTarget = z_name_player(pstEnv->pstAppCtx, pstEnv, pstClt->RoleName);
	if (pstTarget)
	{
		return player_watch_role(pstEnv, pstPlayer, pstTarget);
	}
	else
	{
		 player_watch_offline_player(pstEnv, pstPlayer, pstClt->RoleName, 0);

		 // 称号
		 player_desig_get( pstEnv, pstPlayer, GET_DESIG_WATCH);
	}
	
	return 0;
}

int player_range_watch_pet(ZONESVRENV* pstEnv, Player *pstPlayer, CSRANGECLTWATCHPET *pstClt)
{
	Player *pstTarget;
	ROLEPET *pstRolePet;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	CSPETWATCHINFO *pstWatchInfo = &pstPetSvr->PetSvrData.WatchInfo;

	if(pstEnv->iInstMode == INST_MODE_ZONE_SVR)
	{		
		if(0>player_check_same_world(pstEnv, pstClt->OwnerName))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RANGE204);
			return -1;
		}
		else
		{
			battle_rolename_remove_prefix(pstClt->OwnerName);
		}
	}
	
	pstTarget = z_name_player(pstEnv->pstAppCtx, pstEnv, pstClt->OwnerName);
	if (pstTarget)
	{
		if(pstTarget->stOnline.stSpanOnline.iStat == SPAN_STAT_CONNECT)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RANGE205);
			return -1;
		}
		
		pstRolePet = z_find_role_pet(pstTarget, pstClt->WID, NULL);
		if (NULL == pstRolePet)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
								ZONEERR_RANGE201);
		}
		else
		{
			pstPetSvr->PetSvrType = PET_SVR_WATCH;
			STRNCPY(pstWatchInfo->RoleName, 
					pstClt->OwnerName, 
					CCH(pstWatchInfo->RoleName));
	
			memcpy(&pstWatchInfo->Pet, pstRolePet, sizeof(pstWatchInfo->Pet));

			Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		}
	}
	else
	{
		if(pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RANGE204);
			return -1;
		}
		
		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "宠物主人不在线");
		if (pstClt->WID <= 0)
			return -1;
		player_watch_offline_player(pstEnv, pstPlayer, pstClt->OwnerName, pstClt->WID);
	}
	
	return 0;
}

int player_range_join_clan(ZONESVRENV* pstEnv, Player *pstPlayer, CSRANGECLTJOINCLAN *pstClt)
{
	ZoneClan *pstZoneClan;
	CLANID stClan;

	pstZoneClan = clan_find_by_wid(pstEnv, pstClt->ClanWID);
	if (NULL == pstZoneClan)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RANGE203);
		return 0;
	}

	stClan.GID = pstZoneClan->stClan.ClanProf.GID;
	stClan.Id = pstZoneClan->stClan.ClanProf.Id;
	STRNCPY(stClan.Name, pstZoneClan->stClan.ClanProf.Name, sizeof(stClan.Name));
	
	return player_clan_apply_join_clan(pstEnv, pstPlayer, &stClan);
}

int player_range_find(ZONESVRENV* pstEnv, Player *pstPlayer, CSRANGECLTFIND *pstFind)
{
	int i;
	RangeOneMem *pstRangeOneMem;
	RangeOne *pstRangeOne;
	TOPRANGE *pstRange;
	CSPKG stPkg;
	CSRANGESVR *pstRangeSvr = &stPkg.Body.RangeSvr;
	CSRANGESVRFIND *pstRangeSvrFind = &pstRangeSvr->RangeSvrData.RangeSvrFind;

	Z_CSHEAD_INIT(&stPkg.Head, RANGE_SVR);
	pstRangeSvr->RangeSvrOp = RANGE_SVR_FIND;
	memcpy(&pstRangeSvrFind->CltFind, pstFind, sizeof(pstRangeSvrFind->CltFind));
	pstRangeSvrFind->EntryNum = 0;

	pstRangeOneMem = range_find_onemem(pstEnv, pstFind->MainType, pstFind->SubType);
	if (NULL == pstRangeOneMem || pstFind->Name[0] == 0)
	{
		return -1;
	}

	pstRangeOne = &pstRangeOneMem->stLastRange;

	for (i=0; i<pstRangeOne->iNum; i++)
	{
		pstRange = &pstRangeOne->astRangeEntry[i].stRange;
			
		if (pstFind->Flag)
		{
			if (pstFind->MainType == RANGE_MAIN_PET)
			{
				if (0 == strcmp(pstRange->RangeData.SecondName, pstFind->Name))
				{
					memcpy(&pstRangeSvrFind->Entrys[pstRangeSvrFind->EntryNum++],
						pstRange, sizeof(pstRangeSvrFind->Entrys[0]));
				}
			}
			else
			{
				if (0 == strcmp(pstRange->RangeData.FirstName, pstFind->Name))
				{
					memcpy(&pstRangeSvrFind->Entrys[pstRangeSvrFind->EntryNum++],
						pstRange, sizeof(pstRangeSvrFind->Entrys[0]));
					break;
				}
			}
		}
		else
		{
			if (strstr(pstRange->RangeData.FirstName, pstFind->Name))
			{
				memcpy(&pstRangeSvrFind->Entrys[pstRangeSvrFind->EntryNum++],
						pstRange, sizeof(pstRangeSvrFind->Entrys[0]));
			}
		}
	}

	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int player_range(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						     TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSRANGECLT *pstRangeClt = &pstCsPkg->Body.RangeClt;

	if( pstRangeClt->RangeCltOp!= RANGE_CLT_WATCH_PLAYER &&
		INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		if(pstRangeClt->RangeCltOp == RANGE_CLT_WATCH_PET)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_RANGE204);
		}
		else
		{	
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
		}
		return -1;
	}
	
	if (pstRangeClt->RangeCltOp!= RANGE_CLT_WATCH_PLAYER &&
		pstEnv->pstZoneObj->stRange.iState == RANGE_STAT_INIT)
	{
		return -1;
	}
	
	switch(pstRangeClt->RangeCltOp)
	{
		case RANGE_CLT_PAGE:
			player_range_page(pstEnv, pstPlayer, &pstRangeClt->RangeCltData.RangeCltPage);
			break;
		case RANGE_CLT_WATCH_PLAYER:
			player_range_watch_player(pstEnv, pstPlayer, &pstRangeClt->RangeCltData.RangeCltWatchPlayer);
			break;
		case RANGE_CLT_WATCH_PET:
			player_range_watch_pet(pstEnv, pstPlayer, &pstRangeClt->RangeCltData.RangeCltWatchPet);
			break;
		case RANGE_CLT_JOIN_CLAN:
			player_range_join_clan(pstEnv, pstPlayer, &pstRangeClt->RangeCltData.RangeCltJoinClan);
			break;
		case RANGE_CLT_FIND:
			player_range_find(pstEnv, pstPlayer, &pstRangeClt->RangeCltData.RangeCltFind);
			break;
		default:
			return -1;
			break;
	}
		
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

int range_insert(ZONESVRENV *pstEnv, TOPRANGE *pstRange)
{
	int iOldNum;
	RangeOneMem *pstRangeOneMem = range_find_onemem(pstEnv, pstRange->MainType, pstRange->SubType);

	if (NULL == pstRangeOneMem)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "db range maintype=%d,subtype=%d", pstRange->MainType, pstRange->SubType);
		return -1;
	}

	iOldNum = pstRangeOneMem->stCurrRange.iNum;
	range_set_one_hold(pstEnv, &pstRangeOneMem->stCurrRange, pstRange);
	if (iOldNum >= pstRangeOneMem->stCurrRange.iNum)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "db range err maintype=%d,subtype=%d,RangeVal=%d,firstname=%s", 
			pstRange->MainType, pstRange->SubType, pstRange->RangeData.RangeVal, pstRange->RangeData.FirstName);
	}
	
	return 0;
}


int range_db_get(ZONESVRENV *pstEnv)
{
	RangeMem *pstRangeMem = &pstEnv->pstZoneObj->stRange;
	TOPRANGE stRange;
	DBAsyn stDBAsyn;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;

	stDBAsyn.iCmd = DB_RANGE_SELECT;
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ,
	                 "TopRange", -1, &stDBAsyn);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	strcpy(pstSelectReq->OperInfo.WhereDef, "where TopRange != 0 order by MainType,SubType,TopRange ");
	pstSelectReq->OffSet = pstRangeMem->iDBRecvNum;
	pstSelectReq->Limit = RANGE_ONE_NUM;

	memset(&stRange, 0, sizeof(stRange));

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRange;
	stMetaData.iBuff = sizeof(stRange);
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData,
	                          pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if(0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}

	pstEnv->pstZoneObj->tLastRange = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstRangeMem->iState = RANGE_STAT_GET_REQ;
		
	return 0;
}

int range_check_one(ZONESVRENV *pstEnv, RangeOneMem *pstRangeOneMem)
{
	int i;
	RangeOne stTmp;
	RangeOne *pstLast = &pstRangeOneMem->stLastRange;
	RangeOne *pstCurr = &pstRangeOneMem->stCurrRange;

	memset(&stTmp, 0, sizeof(stTmp));

	for (i=0; i<pstCurr->iNum; i++)
	{
		range_set_one_hold(pstEnv, &stTmp, &pstCurr->astRangeEntry[i].stRange);
	}

	if (stTmp.iNum != pstCurr->iNum)
	{
		memcpy(pstLast , &stTmp, sizeof(*pstLast));
		memcpy(pstCurr, &stTmp, sizeof(*pstCurr));

		for (i=0; i<RANGE_MAX_PAGE; i++)
		{
			pstRangeOneMem->stCurrRange.aiPageVersion[i] = pstEnv->pstAppCtx->stCurr.tv_sec;
			pstRangeOneMem->stLastRange.aiPageVersion[i] = pstEnv->pstAppCtx->stCurr.tv_sec;
		}

		tlog_error(pstEnv->pstLogCat, 0, 0, "range check maintype %d subtype %d err", 
			pstRangeOneMem->iMainType, pstRangeOneMem->iSubType);
	}
	
	return 0;
}

int range_check(ZONESVRENV *pstEnv)
{
	int i;
	RangeMem *pstRangeMem = &pstEnv->pstZoneObj->stRange;
	
	for (i=0; i<pstRangeMem->iRangeNum; i++)
	{
		range_check_one(pstEnv, &pstRangeMem->astRange[i]);
	}
	
	return 0;
}

int range_db_get_fini(ZONESVRENV *pstEnv)
{
	int i,j;
	RangeMem *pstRangeMem = &pstEnv->pstZoneObj->stRange;

	pstRangeMem->iState = RANGE_STAT_DB_FINI;
	
	for (i=0; i<pstRangeMem->iRangeNum; i++)
	{
		for (j=0; j<pstRangeMem->astRange[i].stCurrRange.iNum; j++)
		{
			pstRangeMem->astRange[i].stCurrRange.astRangeEntry[j].stRange.TopRange = j+1;
		}
		
		memcpy(&pstRangeMem->astRange[i].stLastRange, &pstRangeMem->astRange[i].stCurrRange,
			sizeof(pstRangeMem->astRange[i].stLastRange));
	}
	
	return 0;
}

int range_select_res(ZONESVRENV *pstEnv, TORMSELECTRES *pstSelectRes)
{
	int i;
	TOPRANGE stRange;
	RangeMem *pstRangeMem = &pstEnv->pstZoneObj->stRange;

	if (pstRangeMem->iState != RANGE_STAT_GET_REQ)
	{
		return -1;
	}
	
	if(0 == pstSelectRes->SelectResult.ResultNum ||
	   pstSelectRes->SelectResult.ResultNum !=
	   (int)pstSelectRes->SelectResult.ResultTotal)
	{
		pstRangeMem->iState = RANGE_STAT_INIT;
		return -1;
	}

	for(i = 0; i < pstSelectRes->SelectResult.ResultNum; i++)
	{
		memset(&stRange, 0, sizeof(stRange));

		if(0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stRange,
			 sizeof(stRange)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,	"torm_fetch_one_meta fail");
			pstRangeMem->iState = RANGE_STAT_INIT;
			return -1;
		}

		pstRangeMem->iDBRecvNum++;
		range_insert(pstEnv, &stRange);
	}

	if(pstRangeMem->iDBRecvNum < pstRangeMem->iDBNum)
	{
		range_db_get(pstEnv);
	}
	else
	{
		range_db_get_fini(pstEnv);
	}

	return 0;
}

int range_db_update_one(ZONESVRENV* pstEnv, TOPRANGE *pstRange, int iIdx)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pstRange;
	stMetaData.iBuff = sizeof(*pstRange);

	stDBAsyn.iCmd = DB_RANGE_UPDATE;
	stDBAsyn.AsynCmd.stRangeUpdate.iMainRange  = pstRange->MainType;
	stDBAsyn.AsynCmd.stRangeUpdate.iSubType = pstRange->SubType;
	stDBAsyn.AsynCmd.stRangeUpdate.iIdx = iIdx;
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, "TopRange", -1, &stDBAsyn);

	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 1;

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}

	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}
	
	return 0;
}

int range_check_fini(ZONESVRENV* pstEnv)
{
	int i, j;
	RangeOne *pstPageOne;
	RangeMem *pstRangeMem = &pstEnv->pstZoneObj->stRange;
	
	for (i=0; i<pstRangeMem->iRangeNum; i++)
	{
		pstPageOne = &pstRangeMem->astRange[i].stLastRange;
		for (j=0; j<pstPageOne->iNum; j++)
		{
			if (pstPageOne->astRangeEntry[j].iDirty)
			{
				return 0;
			}
		}
	}

	pstRangeMem->iState = RANGE_STAT_DB_FINI;
	
	return 0;
}

int range_update_res(ZONESVRENV* pstEnv, TORMPKG *pstTormPkg, DBAsyn *pstDBAsyn)
{
	RangeOneMem *pstRangeOneMem;
	TORMUPDATERES *pstUpdateRes = &pstTormPkg->Body.UpdateRes;
	RangeUpdate *pstRangeUpdate = &pstDBAsyn->AsynCmd.stRangeUpdate;

	if (TORM_ERR_IS_ERROR(pstUpdateRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "range_update_res failed." );
		return -1;
	}
	
	pstRangeOneMem = range_find_onemem(pstEnv, pstRangeUpdate->iMainRange,
										pstRangeUpdate->iSubType);

	if (NULL == pstRangeOneMem || pstRangeUpdate->iIdx < 0 || pstRangeUpdate->iIdx >= RANGE_ONE_NUM)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "range_update_res failed." );
		return -1;
	}

	pstRangeOneMem->stLastRange.astRangeEntry[pstRangeUpdate->iIdx].iDirty = 0;

	//加快zone_stop的速度
	if (pstEnv->cShutdown > 1)
	{
		range_check_fini(pstEnv);
	}
	
	return 0;
}

int range_db_update(ZONESVRENV* pstEnv)
{
	int i, j;
	RangeOne *pstPageOne;
	RangeMem *pstRangeMem = &pstEnv->pstZoneObj->stRange;
	int iUpdateNum = 0;

	pstEnv->pstZoneObj->tLastRange = pstEnv->pstAppCtx->stCurr.tv_sec;
		
	for (i=0; i<pstRangeMem->iRangeNum; i++)
	{
		pstPageOne = &pstRangeMem->astRange[i].stLastRange;
		for (j=0; j<pstPageOne->iNum; j++)
		{
			if (pstPageOne->astRangeEntry[j].iDirty)
			{
				range_db_update_one(pstEnv, &pstPageOne->astRangeEntry[j].stRange, j);	
				
				iUpdateNum++;
				if ((pstEnv->cShutdown <=1 && iUpdateNum >= RANGE_UPDATE_NUM) ||
					iUpdateNum >= RANGE_UPDATE_NUM*20) //加快zone_stop的速度
				{
					return 0;
				}
			}
		}
	}

	if (iUpdateNum == 0)
	{
		pstRangeMem->iState = RANGE_STAT_DB_FINI;
	}
	
	return 0;
}

int range_get_db_count_req(ZONESVRENV* pstEnv)
{
	DBAsyn stDBAsyn;
	TOPRANGE stRange;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTCOUNTREQ *pstSelectCountReq = &pstPkg->Body.SelectCountReq;

	stDBAsyn.iCmd = DB_RANGE_COUNT_SELECT;
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head,
	                 TORM_SELECT_COUNT_REQ, "TopRange", -1, &stDBAsyn);
	strcpy(pstSelectCountReq->WhereDef, "where TopRange != 0");
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);

	memset(&stRange, 0, sizeof(stRange));
	stMetaData.pszBuff = (char *)&stRange;
	stMetaData.iBuff = sizeof(stRange);
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData,
	                          pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}

	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}

	pstEnv->pstZoneObj->tLastRange = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstEnv->pstZoneObj->stRange.iState = RANGE_STAT_GET_COUNT_REQ;

	return 0;
}

int range_count_res(ZONESVRENV *pstEnv, TORMSELECTCOUNTRES *pstSelectCountRes)
{
	RangeMem *pstRangeMem = &pstEnv->pstZoneObj->stRange;
		
	if(pstRangeMem->iState != RANGE_STAT_GET_COUNT_REQ)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "range stat %d",
		           		pstRangeMem->iState);
		return -1;
	}

	pstRangeMem->iDBNum = pstSelectCountRes->RowsCount;
	pstRangeMem->iDBRecvNum = 0;
	
	if (0 == pstRangeMem->iDBNum)
	{
		pstRangeMem->iState = RANGE_STAT_DB_FINI;
	}
	else
	{
		range_db_get(pstEnv);
	}
	
	return 0;
}

int range_init(ZONESVRENV* pstEnv)
{
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	pstObj->tLastRange = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstObj->stRange.iState = RANGE_STAT_INIT;
	pstObj->stRange.iRangeNum = 0;

	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		return 0;
	}

	range_init_one(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_LEVEL);
	range_init_one(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_MONEY);
	range_init_one(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_PK);
	range_init_one(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_CLANCT);
	range_init_one(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_GENIUS);
	range_init_one(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_CHARM);
	range_init_one(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_CHARM_WEEK);

	range_init_one(pstEnv, RANGE_MAIN_CAREER, RANGE_MAIN_CAREER_SUB_WARRIOR);
	range_init_one(pstEnv, RANGE_MAIN_CAREER, RANGE_MAIN_CAREER_SUB_MAGICOR);
	range_init_one(pstEnv, RANGE_MAIN_CAREER, RANGE_MAIN_CAREER_SUB_GUNER);
	range_init_one(pstEnv, RANGE_MAIN_CAREER, RANGE_MAIN_CAREER_SUB_CHURCH);

	range_init_one(pstEnv, RANGE_MAIN_CLAN, RANGE_MAIN_CLAN_SUB_DEVELOP);
	range_init_one(pstEnv, RANGE_MAIN_CLAN, RANGE_MAIN_CLAN_SUB_MEM);
	range_init_one(pstEnv, RANGE_MAIN_CLAN, RANGE_MAIN_CLAN_SUB_COMBAT);
	range_init_one(pstEnv, RANGE_MAIN_CLAN, RANGE_MAIN_CLAN_SUB_LEVEL);
	
	range_init_one(pstEnv, RANGE_MAIN_PET, RANGE_MAIN_PET_SUB_FIGHT);

	range_init_one(pstEnv, RANGE_MAIN_COMBAT, RANGE_MAIN_COMBAT_SUB_PANSHAN_TIME);
	range_init_one(pstEnv, RANGE_MAIN_COMBAT, RANGE_MAIN_COMBAT_SUB_DRAGON_TIME);
	range_init_one(pstEnv, RANGE_MAIN_COMBAT, RANGE_MAIN_COMBAT_SUB_MOGUI_TIME);
	range_init_one(pstEnv, RANGE_MAIN_COMBAT, RANGE_MAIN_COMBAT_SUB_PANSHAN_NUM);
	range_init_one(pstEnv, RANGE_MAIN_COMBAT, RANGE_MAIN_COMBAT_SUB_DRAGON_NUM);
	range_init_one(pstEnv, RANGE_MAIN_COMBAT, RANGE_MAIN_COMBAT_SUB_MOGUI_NUM);
	range_init_one(pstEnv, RANGE_MAIN_COMBAT, RANGE_MAIN_COMBAT_SUB_BCT);

	range_init_one(pstEnv, RANGE_MAIN_BATTLE, RANGE_MAIN_BATTLE_SUB_SCSURVIVE_GOD);
	range_init_one(pstEnv, RANGE_MAIN_BATTLE, RANGE_MAIN_BATTLE_SUB_SCSURVIVE_KILLER);
	range_init_one(pstEnv, RANGE_MAIN_BATTLE, RANGE_MAIN_BATTLE_SUB_SCSURVIVE_NODEAD);
	range_init_one(pstEnv, RANGE_MAIN_BATTLE, RANGE_MAIN_BATTLE_SUB_LZSURVIVE_GOD);
	range_init_one(pstEnv, RANGE_MAIN_BATTLE, RANGE_MAIN_BATTLE_SUB_LZSURVIVE_KILLER);
	range_init_one(pstEnv, RANGE_MAIN_BATTLE, RANGE_MAIN_BATTLE_SUB_LZSURVIVE_NODEAD);
	range_init_one(pstEnv, RANGE_MAIN_BATTLE, RANGE_MAIN_BATTLE_SUB_LZSURVIVE_END);
	range_init_one(pstEnv, RANGE_MAIN_BATTLE, RANGE_MAIN_BATTLE_SUB_LZWIN_NUM);
	range_init_one(pstEnv, RANGE_MAIN_BATTLE, RANGE_MAIN_BATTLE_SUB_KUANGSHA_WIN_NUM);

	range_init_one(pstEnv, RANGE_MAIN_SPARTA, RANGE_MAIN_SPARTA_SUB_FIGHT);

	// 初始化属性排行榜
	range_init_one(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_HP);
	range_init_one(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_MP);
	range_init_one(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_WULI_ATK);
	range_init_one(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_MOFA_ATK);
	range_init_one(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_WULI_DEF);
	range_init_one(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_MOFA_DEF);
	range_init_one(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_BAOJI_LVL);
	range_init_one(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_BAOJI_HURT);
	range_init_one(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_DIKANG_LVL);
	range_init_one(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_POFANG_LVL);
	range_init_one(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_ZHILIAO_ADD);
	
	range_get_db_count_req(pstEnv);

	return 0;
}


int range_tick(ZONESVRENV* pstEnv)
{
	static int iRetry = 0;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	switch (pstObj->stRange.iState)
	{
		case RANGE_STAT_INIT:
			if (iRetry > 6)
			{
				pstObj->stRange.iState = RANGE_STAT_FAIL;
			}
			else
			{
				range_init(pstEnv);
			}
			break;
		case RANGE_STAT_GET_COUNT_REQ:
			if (pstObj->tLastRange + 5 < pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				//range_get_db_count_req(pstEnv);

				iRetry++;
				pstObj->stRange.iState = RANGE_STAT_INIT;
				tlog_error(pstEnv->pstLogCat, 0, 0, "range get count fail");
			}
			break;
		case RANGE_STAT_GET_REQ:
			if (pstObj->tLastRange + 5 < pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				//range_db_get(pstEnv);

				iRetry++;
				pstObj->stRange.iState = RANGE_STAT_INIT;
				tlog_error(pstEnv->pstLogCat, 0, 0, "range get fail");
			}
			break;
		case RANGE_STAT_CHG:
			if (pstObj->tLastRange + 5 < pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				range_db_delete(pstEnv);
				tlog_error(pstEnv->pstLogCat, 0, 0, "range delete fail");
			}
			break;
		case RANGE_STAT_DB_UPDATE:
			if (pstObj->tLastRange + 2 < pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				range_db_update(pstEnv);
			}
			break;
		case RANGE_STAT_DB_FINI:
			if (pstObj->tLastRange/3600 != pstEnv->pstAppCtx->stCurr.tv_sec/3600)
			{
				range_do_chg(pstEnv);
			}
			break;
		default:
			break;
	}
	
	return 0;
}

ROLERANGEINFOENTRY * range_add_range_info_val(ZONESVRENV* pstEnv, Player *pstPlayer, 
													int iMainType, int iSubType, unsigned int uiVal)
{
	int i;
	ROLERANGEINFO *pstRoleRangeInfo = &pstPlayer->stRoleData.MiscInfo.RoleRangeInfo;
	ROLERANGEINFOENTRY *pstRoleRangeInfoEntry = NULL;

	for (i=0; i<pstRoleRangeInfo->Num; i++)
	{
		if (iMainType == pstRoleRangeInfo->RangeEntrys[i].MainType &&
			iSubType == pstRoleRangeInfo->RangeEntrys[i].SubType)
		{
			pstRoleRangeInfoEntry = &pstRoleRangeInfo->RangeEntrys[i];
			break;
		}
	}

	if (NULL == pstRoleRangeInfoEntry)
	{
		if (pstRoleRangeInfo->Num >= MAX_RANGE_INFO)
		{
			return NULL;
		}

		pstRoleRangeInfoEntry = &pstRoleRangeInfo->RangeEntrys[pstRoleRangeInfo->Num++];
		pstRoleRangeInfoEntry->MainType = iMainType;
		pstRoleRangeInfoEntry->SubType = iSubType;
		pstRoleRangeInfoEntry->Val = uiVal;
	}
	else
	{
		pstRoleRangeInfoEntry->Val += uiVal;
	}

	return pstRoleRangeInfoEntry;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int range_combat_script_set_one(ZONESVRENV* pstEnv, Player *pstPlayer, int iSubType, 
										ZONEPWORLDINST *pstPworldInst, ZONEPWORLD* pstPworld)
{
	unsigned int uiVal = 0;
	ROLERANGEINFOENTRY *pstRoleRangeInfoEntry;
	char szSecondName[NAME_LEN];

	STRNCPY(szSecondName,pstPworld->stPworldDef.Name, sizeof(szSecondName));
	
	switch(iSubType)
	{
		case RANGE_MAIN_COMBAT_SUB_PANSHAN_TIME:
		case RANGE_MAIN_COMBAT_SUB_DRAGON_TIME:
		case RANGE_MAIN_COMBAT_SUB_MOGUI_TIME:
			uiVal = pstEnv->pstAppCtx->stCurr.tv_sec - pstPworldInst->tStart;
			break;
		case RANGE_MAIN_COMBAT_SUB_PANSHAN_NUM:
		case RANGE_MAIN_COMBAT_SUB_DRAGON_NUM:
		case RANGE_MAIN_COMBAT_SUB_MOGUI_NUM:	
			pstRoleRangeInfoEntry = range_add_range_info_val(pstEnv, pstPlayer,
										RANGE_MAIN_COMBAT , iSubType, 1);
			if (pstRoleRangeInfoEntry)
			{
				uiVal = pstRoleRangeInfoEntry->Val;
			}
			else
			{
				return -1;
			}
			break;
		case RANGE_MAIN_COMBAT_SUB_BCT:
			{
				ZoneClan *pstClan;
				
				pstClan = player_get_clan(pstEnv, pstPlayer);
				if (pstClan)
				{
					STRNCPY(szSecondName, pstClan->stClan.ClanProf.Name, sizeof(szSecondName));
				}
				else
				{
					szSecondName[0] = 0;
				}

				uiVal = pstPlayer->stRoleData.MiscInfo.BCTHistoryTop;
			}
			break;
		default:
			return -1;
			break;
	}

	range_combat_set(pstEnv, pstPlayer, iSubType, pstPworldInst->stPworldselect.PworldGrade,
						szSecondName, uiVal);

	return 0;
}

int range_battle_script_set_one(ZONESVRENV* pstEnv, Player *pstPlayer, int iSubType, 
										ZONEPWORLDINST *pstPworldInst, ZONEPWORLD* pstPworld)
{
	unsigned int uiVal = 0;
	ROLERANGEINFOENTRY *pstRoleRangeInfoEntry;
	char szSecondName[NAME_LEN];
	BATTLEDEF *pstBattleDef;

	pstBattleDef = z_find_battledef(pstEnv, pstPworld->stPworldDef.BattleID);
	if (pstBattleDef)
	{
		STRNCPY(szSecondName,pstBattleDef->Name, sizeof(szSecondName));
	}
	else
	{
		STRNCPY(szSecondName,pstPworld->stPworldDef.Name, sizeof(szSecondName));
	}
	
	switch(iSubType)
	{
		case RANGE_MAIN_BATTLE_SUB_LZSURVIVE_END:	
			pstRoleRangeInfoEntry = range_add_range_info_val(pstEnv, pstPlayer,
										RANGE_MAIN_BATTLE , iSubType, 1);
			if (pstRoleRangeInfoEntry)
			{
				uiVal = pstRoleRangeInfoEntry->Val;
			}
			else
			{
				return -1;
			}
			break;
		default:
			return -1;
			break;
	}

	range_battle_set(pstEnv, pstPlayer, iSubType, pstPworldInst->stPworldselect.PworldGrade,
						szSecondName, uiVal);

	return 0;
}

int range_combat_script_set(ZONESVRENV* pstEnv, Player *pstPlayer, int iMainType, int iSubType, ZONEPWORLDINST *pstPworldInst)
{
	int i;
	ZoneTeam *pstZoneTeam;
	ZONEPWORLD* pstPworld;
	Player *pstPlayerMember;
			
	if (NULL == pstPlayer || NULL == pstPworldInst)
	{
		return -1;
	}

	pstPworld	 =	z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if (NULL == pstPworld)
	{
		return -1;
	}

	//动态副本只排困难的
	/*
	if (pstPworld->stPworldDef.DynType && pstPworldInst->stPworldselect.PworldGrade < PWORLD_GRADE_B)
	{
		return 0;
	}*/

	if (iMainType == RANGE_MAIN_COMBAT)
	{
		pstZoneTeam = player_team_get(pstEnv, pstPlayer);

		if (NULL == pstZoneTeam)
		{
			range_combat_script_set_one(pstEnv, pstPlayer, iSubType, pstPworldInst,
										 pstPworld);
		}
		else
		{
			TEAMMEMBER *pstTeamMember;
			TEAMINFO *pstTeam = &pstZoneTeam->stTeamInfo;

			range_combat_script_set_one(pstEnv, pstPlayer, iSubType, pstPworldInst,
										 pstPworld);

			for (i=pstTeam->MemberNum-1; i>=0; i--)
			{
				pstTeamMember = &pstTeam->TeamMember[i];
				if (pstTeamMember->OffLineTime > 0)
				{
					continue;
				}
				
				pstPlayerMember = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeamMember->MemberName);
				if (pstPlayerMember && pstPlayerMember != pstPlayer && pstPlayerMember->stRoleData.Map == pstPlayer->stRoleData.Map)
				{
					range_combat_script_set_one(pstEnv, pstPlayerMember, iSubType, pstPworldInst,
										 		pstPworld);
				}
			}
		}
	}
	else if (iMainType == RANGE_MAIN_BATTLE)
	{
		range_battle_script_set_one(pstEnv, pstPlayer, iSubType, pstPworldInst, pstPworld);
	}

	return 0;
}

int range_delete(ZONESVRENV* pstEnv, int iMainType, int iSubType, tdr_ulonglong ullGID)
{
	int i;
	RangeOneMem *pstRangeOneMem;
	RangeOne *pstCurr;

	pstRangeOneMem = range_find_onemem(pstEnv, iMainType, iSubType);
	if (NULL == pstRangeOneMem)
	{
		return -1;
	}

	pstCurr = &pstRangeOneMem->stCurrRange;
	for (i=pstCurr->iNum-1; i>=0; i--)
	{
		if (pstCurr->astRangeEntry[i].stRange.RangeData.GID == ullGID)
		{
			pstCurr->iNum--;

			if (i < pstCurr->iNum)
			{
				memmove(&pstCurr->astRangeEntry[i], &pstCurr->astRangeEntry[i+1],
							(pstCurr->iNum-i)*sizeof(pstCurr->astRangeEntry[0]));
			}
			
			return 0;
		}
	}

	return 0;
}

int range_up_role_name(ZONESVRENV* pstEnv, tdr_ulonglong ullRoleID, const char* pszOldName, const char* pszNewName)
{
	RangeMem *pstRange = &pstEnv->pstZoneObj->stRange;
	RangeOne *pstCurr;
	char *pszRoleName;
	int i,j;
	
	for (i=0; i<pstRange->iRangeNum; i++)
	{
		pstCurr = &pstRange->astRange[i].stLastRange;
		for (j=0; j<pstCurr->iNum; j++)
		{
			pszRoleName = range_get_name(&pstCurr->astRangeEntry[j].stRange);
			if (!pszRoleName)
			{
				continue;
			}
			
			if (strcmp(pszRoleName, pszOldName)  == 0)
			{
				STRNCPY(pszRoleName,pszNewName, ROLE_NAME_LEN);
			}
		}

		pstCurr = &pstRange->astRange[i].stCurrRange;
		for (j=0; j<pstCurr->iNum; j++)
		{
			pszRoleName = range_get_name(&pstCurr->astRangeEntry[j].stRange);
			if (!pszRoleName)
			{
				continue;
			}

			if (strcmp(pszRoleName, pszOldName)  == 0)
			{
				STRNCPY(pszRoleName,pszNewName, ROLE_NAME_LEN);
			}
		}
	}
	
	return 0;
}

int range_up_clanname(ZONESVRENV* pstEnv, tdr_ulonglong RoleID, const char* pszOldName, const char* pszNewName, int iSingle)
{
	RangeMem *pstRange = &pstEnv->pstZoneObj->stRange;
	RangeOne *pstCurr;
	char *pszRoleName;
	int i,j;
	
	for (i=0; i<pstRange->iRangeNum; i++)
	{
		pstCurr = &pstRange->astRange[i].stLastRange;
		for (j=0; j<pstCurr->iNum; j++)
		{
			if (iSingle)
			{
				if (pstCurr->astRangeEntry[j].stRange.RangeData.GID != RoleID)
				{
					continue;
				}
			}
			
			pszRoleName = range_get_clanname(&pstCurr->astRangeEntry[j].stRange, iSingle);
			if (!pszRoleName)
			{
				continue;
			}
			
			if (strcmp(pszRoleName, pszOldName)  == 0)
			{
				STRNCPY(pszRoleName,pszNewName, ROLE_NAME_LEN);
			}
			
		}

		pstCurr = &pstRange->astRange[i].stCurrRange;
		for (j=0; j<pstCurr->iNum; j++)
		{
			if (iSingle)
			{
				if (pstCurr->astRangeEntry[j].stRange.RangeData.GID != RoleID)
				{
					continue;
				}
			}
			
			pszRoleName = range_get_clanname(&pstCurr->astRangeEntry[j].stRange, iSingle);
			if (!pszRoleName)
			{
				continue;
			}

			if (strcmp(pszRoleName, pszOldName)  == 0)
			{
				STRNCPY(pszRoleName,pszNewName, ROLE_NAME_LEN);
			}
		}
	}
	
	return 0;
}

int range_delete_role(ZONESVRENV* pstEnv, tdr_ulonglong ullRoleID, int iCareer)
{
	range_delete(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_LEVEL, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_MONEY, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_PK, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_CLANCT, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_GENIUS, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_CHARM, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_CHARM_WEEK, ullRoleID);

	iCareer &= 0x0f;
	if (iCareer > CAREER_NON)
	{
		range_delete(pstEnv, RANGE_MAIN_CAREER, iCareer-1, ullRoleID);
	}
	else
	{
		range_delete(pstEnv, RANGE_MAIN_CAREER, RANGE_MAIN_CAREER_SUB_WARRIOR, ullRoleID);
		range_delete(pstEnv, RANGE_MAIN_CAREER, RANGE_MAIN_CAREER_SUB_MAGICOR, ullRoleID);
		range_delete(pstEnv, RANGE_MAIN_CAREER, RANGE_MAIN_CAREER_SUB_GUNER, ullRoleID);
		range_delete(pstEnv, RANGE_MAIN_CAREER, RANGE_MAIN_CAREER_SUB_CHURCH, ullRoleID);
	}

	range_delete(pstEnv, RANGE_MAIN_COMBAT, RANGE_MAIN_COMBAT_SUB_PANSHAN_TIME, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_COMBAT, RANGE_MAIN_COMBAT_SUB_DRAGON_TIME, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_COMBAT, RANGE_MAIN_COMBAT_SUB_MOGUI_TIME, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_COMBAT, RANGE_MAIN_COMBAT_SUB_PANSHAN_NUM, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_COMBAT, RANGE_MAIN_COMBAT_SUB_DRAGON_NUM, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_COMBAT, RANGE_MAIN_COMBAT_SUB_MOGUI_NUM, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_COMBAT, RANGE_MAIN_COMBAT_SUB_BCT, ullRoleID);

	range_delete(pstEnv, RANGE_MAIN_BATTLE, RANGE_MAIN_BATTLE_SUB_SCSURVIVE_GOD, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_BATTLE, RANGE_MAIN_BATTLE_SUB_SCSURVIVE_KILLER, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_BATTLE, RANGE_MAIN_BATTLE_SUB_SCSURVIVE_NODEAD, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_BATTLE, RANGE_MAIN_BATTLE_SUB_LZSURVIVE_GOD, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_BATTLE, RANGE_MAIN_BATTLE_SUB_LZSURVIVE_KILLER, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_BATTLE, RANGE_MAIN_BATTLE_SUB_LZSURVIVE_NODEAD, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_BATTLE, RANGE_MAIN_BATTLE_SUB_LZSURVIVE_END, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_BATTLE, RANGE_MAIN_BATTLE_SUB_LZWIN_NUM, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_BATTLE, RANGE_MAIN_BATTLE_SUB_KUANGSHA_WIN_NUM, ullRoleID);

	range_delete(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_HP, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_MP, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_WULI_ATK, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_MOFA_ATK, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_WULI_DEF, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_MOFA_DEF, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_BAOJI_LVL, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_BAOJI_HURT, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_DIKANG_LVL, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_POFANG_LVL, ullRoleID);
	range_delete(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_ZHILIAO_ADD, ullRoleID);
	
	return 0;
}

int range_weekcharm_award_dsn(ZONESVRENV* pstEnv)
{
	int i,j;
	RANGDSNDEF *pstDef;
	RangeOneMem * pstOne;
	Player *pstPlayer = NULL;
	DESIGNATIONENTRY* pstDsn;
	const DESIGNATIONDEF* pstDsnDef;
	int iNotify = 0;
	
	for (i=0; i<pstEnv->pstZoneObj->iRangDsnDef; i++)
	{
		pstDef = (RANGDSNDEF *)&pstEnv->pstZoneObj->sRangDsnDef[i];
		
		if(pstDef->MainType != RANGE_MAIN_SINGLE || pstDef->SubType != RANGE_MAIN_SINGLE_SUB_CHARM_WEEK)
		{
			continue;
		}
		
		pstOne = range_find_onemem(pstEnv,pstDef->MainType, pstDef->SubType);
		if (!pstOne)
		{
			continue;
		}

		for (j=0; j<pstDef->Top && j<pstOne->stLastRange.iNum; j++)
		{
			if (j >= MAX_RANGDSN_TOP || pstDef->DsnID[j] <= 0)
				break;

			iNotify = 0;
			
			pstPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,
								pstOne->stLastRange.astRangeEntry[j].stRange.RangeData.FirstName);
		
			if (pstPlayer)
			{
				pstDsn =  player_find_designation(pstEnv,pstPlayer,pstDef->DsnID[j]);
				if (pstDsn)
				{
					pstDsnDef = z_designation_def(pstEnv, pstDsn->ID);
					if (pstDsnDef && pstDsnDef->DesTime > 0)
					{
						pstDsn->EndTime = pstEnv->pstAppCtx->stCurr.tv_sec+pstDsnDef->DesTime;
						iNotify = 1;
					}
				}
				else
				{
					g_iDesigVar = 1;
					player_designation_add(pstEnv, pstPlayer,pstDef->DsnID[j]);
				}

				if (iNotify)
				{
					player_designation_info(pstEnv, pstPlayer);
				}
			}
			else
			{
				DBMiscInfo stDBMiscInfo;
				tdr_ulonglong ullRoleID = pstOne->stLastRange.astRangeEntry[j].stRange.RangeData.GID;
				memset(&stDBMiscInfo,0,sizeof(stDBMiscInfo));
			
				stDBMiscInfo.iCmd = DB_MISC_INFO_DSN_UPDATE;
				stDBMiscInfo.AsynMiscInfo.stMiscInfoDsn.ullRoleID = ullRoleID;
				stDBMiscInfo.AsynMiscInfo.stMiscInfoDsn.uiDsn = pstDef->DsnID[j];
			
				z_get_role_misc_info(pstEnv,&stDBMiscInfo,ullRoleID);
			}
		}
	}

	return 0;
}

int range_find(ZONESVRENV* pstEnv, int iMainType, int iSubType, tdr_ulonglong ullGID)
{
	int i;
	RangeOneMem *pstRangeOneMem;
	RangeOne *pstCurr;

	pstRangeOneMem = range_find_onemem(pstEnv, iMainType, iSubType);
	if (NULL == pstRangeOneMem)
	{
		return -1;
	}

	pstCurr = &pstRangeOneMem->stCurrRange;
	for (i=pstCurr->iNum-1; i>=0; i--)
	{
		if (pstCurr->astRangeEntry[i].stRange.RangeData.GID == ullGID)
		{
			return 1;
		}
	}

	return 0;
}

//排行榜上原来有的属性排名,就替换
int range_attr_replace(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	int iTmp1 = 0;
	tdr_ulonglong ullRoleID = 0;

	if (	pstPlayer->stRoleData.Perm > 0 ||
		(pstPlayer->UinFlag & UIN_FLAG_GM_ALL) )
	{
		return 0;
	}

	ullRoleID = pstPlayer->stRoleData.RoleID;
	if(0 < range_find(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_DIKANG_LVL, ullRoleID))
	{
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_DIKANG_LVL, pstDyn->stDefChg.iDefHP);
	}

	if(0 < range_find(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_POFANG_LVL, ullRoleID))
	{
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_POFANG_LVL, pstDyn->stDefChg.iIgnoreDef);
	}

	if(0 < range_find(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_HP, ullRoleID))
	{
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_HP, pstDyn->stMidAttrList.uiHPMax);	
	}

	if(0 < range_find(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_MP, ullRoleID))
	{
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_MP, pstDyn->stMidAttrList.uiMPMax);	
	}

	if(0 < range_find(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_BAOJI_HURT, ullRoleID))
	{
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_BAOJI_HURT, pstDyn->stHitRateChg.nHeavyHitVal);	
	}
	
	if(0 < range_find(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_BAOJI_LVL, ullRoleID))
	{
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_BAOJI_LVL, pstDyn->stHitRateChg.iHeavyHit);	
	}

	if(0 < range_find(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_WULI_ATK, ullRoleID))
	{
		// 物理攻击
		iTmp1 = z_calc_player_attr_atk(pstDyn, ATTR_ID_PHSIC_MAX_ATK);
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_WULI_ATK, iTmp1);	
	}

	if(0 < range_find(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_WULI_DEF, ullRoleID))
	{
		// 物理防御
		iTmp1 = z_cacl_attr_def(&pstDyn->stDefChg, SKILL_ATTR_PHSIC);
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_WULI_DEF, iTmp1);	
	}

	if(0 < range_find(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_MOFA_ATK, ullRoleID))
	{
		// 魔法攻击
		iTmp1 =  z_calc_player_attr_atk(pstDyn, ATTR_ID_MAGIC_MAX_ATK);
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_MOFA_ATK, iTmp1);	
	}
	
	if(0 < range_find(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_MOFA_DEF, ullRoleID))
	{
		// 魔法防御
		iTmp1 = z_cacl_attr_def(&pstDyn->stDefChg, SKILL_ATTR_ICE); 
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_MOFA_DEF, iTmp1);	
	}

	if(0 < range_find(pstEnv, RANGE_MAIN_ATTR, RANGE_MAIN_ATTR_SUB_ZHILIAO_ADD, ullRoleID))
	{
		// 治疗加成
		range_attr_set_in(pstEnv, pstPlayer, RANGE_MAIN_ATTR_SUB_ZHILIAO_ADD, pstDyn->stHealChg.nHealHPMul);	
	}
	
	return 0;
}

//排行榜上原来有的个人排名,就替换
int range_single_replace(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	tdr_ulonglong ullRoleID = pstPlayer->stRoleData.RoleID;
	
	if(0 < range_find(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_LEVEL, ullRoleID))
	{
		range_single_set(pstEnv, pstPlayer, RANGE_MAIN_SINGLE_SUB_LEVEL, pstPlayer->stRoleData.Level);
	}
	
	if(0 < range_find(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_PK, ullRoleID))
	{
		range_single_set(pstEnv, pstPlayer, RANGE_MAIN_SINGLE_SUB_PK, pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal);
	}

	if(0 < range_find(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_GENIUS, ullRoleID))
	{
		range_single_set(pstEnv, pstPlayer, RANGE_MAIN_SINGLE_SUB_GENIUS, genius_get_mod(pstEnv, pstPlayer));
	}

	if(0 < range_find(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_CHARM, ullRoleID))
	{
		range_single_set(pstEnv, pstPlayer, RANGE_MAIN_SINGLE_SUB_CHARM, pstPlayer->stRoleData.RoleDetail.WhiteJade);
	}

	if(0 < range_find(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_CHARM_WEEK, ullRoleID))
	{
		range_single_set(pstEnv, pstPlayer, RANGE_MAIN_SINGLE_SUB_CHARM_WEEK, pstPlayer->stRoleData.MiscInfo.WeekCharmInfo.Charm);
	}

	if(0 < range_find(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_MONEY, ullRoleID))
	{
		range_single_set(pstEnv, pstPlayer, RANGE_MAIN_SINGLE_SUB_MONEY, 
							pstPlayer->stRoleData.RoleDetail.Money+pstPlayer->stRoleData.Package.StashPackage.Money);
	}
	
	return 0;
}

