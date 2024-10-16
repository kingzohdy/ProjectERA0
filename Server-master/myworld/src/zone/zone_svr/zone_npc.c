/*
**  @file $RCSfile: zone_npc.c,v $
**  general description of this module
**  $Id: zone_npc.c,v 1.521 2014/06/23 07:06:37 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2014/06/23 07:06:37 $
**  @version $Revision: 1.521 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_npc.h"
#include "zone_map.h"
#include "zone_clt.h"
#include "zone_move.h"
#include "zone_status.h"
#include "zone_attr.h"
#include "zone_ai.h"
#include "zone_script.h"
#include "zone_team.h"
#include "zone_ai.h"
#include "zone_package.h"
#include "zone_tmp.h"
#include "zone_player.h"
#include "zone_act_status.h"
#include "zone_combat.h"
#include "zone_script.h"
#include "zone_oplog.h"
#include "zone_task.h"
#include "zone_pet.h"
#include "zone_skill.h"
#include "zone_respact.h"
#include "zone_fairy.h"
#include "tconnapi.h"
#include "zone_range.h"
#include "mapmask.h"
#include "zone_machine.h"
#include "zone_clan_advance.h"
#include "zone_itemlimit.h"
#include "zone_clan.h"
#include "zone_booty.h"
#include "zone_fairy.h"
#include "zone_diffpworld.h"
#include "zone_safelock.h"
#include "zone_city.h"
#include "zone_clan_city.h"
#include "zone_unreal.h"
#include "zone_designation.h"
#include "zone_boss.h"
#include "zone_strongpoint.h"
#include "zone_arm.h"
#include "zone_vip.h"
#include "zone_pworld.h"
#include "zone_err.h"
#include "zone_span_strong.h"

extern ScriptEnv stSEnv;

#define BOOTY_PAK_ID 9900000

#define SELL_ITEM_SAVE_TIME 1800 

#define MAX_RAND_POSITION  48

int g_aiPositionMap[MAX_RAND_POSITION];
int g_iPositionRangeCurr;

void rand_position_init()
{
	int i;

	g_iPositionRangeCurr = MAX_RAND_POSITION;
	
	for(i = 0; i<g_iPositionRangeCurr; i++)
	{
		g_aiPositionMap[i] = MAX_RAND_POSITION- i;
	}

	return ;
}


// 1~48     第一圈 1~4  第二圈 5~12 。。。。。
static int get_rand_position()
{
	int iRange;
	int iRet = 0;
	int iIdx, i = 1, s = 0;;

	if(g_iPositionRangeCurr<= 0)
	{
		rand_position_init();
	}

	while(1)
	{
		s += i*4;
		if((MAX_RAND_POSITION - g_iPositionRangeCurr + 1) <= s)
		{
			break;
		}
		i++;
	}

	//每次随机范围扩大一圈
	iRange = s+ (i+1)*4 - (MAX_RAND_POSITION -g_iPositionRangeCurr);
	if(iRange >  g_iPositionRangeCurr)
	{
		iRange = g_iPositionRangeCurr;
	}
	
	iIdx = RAND1(iRange);

	//反向取
	iIdx = g_iPositionRangeCurr - (iIdx+1);
	iRet = g_aiPositionMap[iIdx];

	g_iPositionRangeCurr--;
	if(iIdx != g_iPositionRangeCurr)
	{
		 g_aiPositionMap[iIdx] = g_aiPositionMap[g_iPositionRangeCurr];
		// g_aiPositionMap[g_iPositionRangeCurr] = iRet;
	}
	
	return iRet;
}

extern int trl_comp_func_ll(const void* pv1, const void* pv2);

MONSTERSPEECHDEF *z_find_monsterspeech_def(ZONESVRENV *pstEnv, unsigned int uiMonsterID, int *pIidx)
{
	return (MONSTERSPEECHDEF *)z_find_def(pstEnv->pstZoneObj->sMonsterSpeechDef, uiMonsterID, MONSTER_SPEECH_DEF_DATA, 
									pstEnv->pstZoneObj->iMonsterSpeechDef, pIidx);
}

MONSTERSTATESKILLDEF *z_find_monsterstateskill_def(ZONESVRENV *pstEnv, unsigned int uiMonsterStateSkillID, int *pIidx)
{
	return (MONSTERSTATESKILLDEF *)z_find_def(pstEnv->pstZoneObj->sMonsterStateSkillDef, uiMonsterStateSkillID,
					MONSTERSTATESKILL_DEF_DATA, pstEnv->pstZoneObj->iMonsterStateSkillDef, pIidx);
}

int get_mon_lv_graden(Monster *pstMon)
{
	//return pstMon->stMonDynAttr.Level/10 + (pstMon->stMonDynAttr.Level%10 > 0 ? 1:0);
	// 1--9         	1
	// 10-19		2
	// .....
	// 110-120	12

	int iLv = pstMon->stMonDynAttr.Level;
	if (iLv == MAX_LEVEL)
		iLv --;
	
	return iLv/10 +1;
}

MONSTERDEF * z_get_mon_def(ZONESVRENV* pstEnv, Monster* pstMon)
{
	MONSTERDEF stMonDef;
	MONSTERDEF *pstMonDef;
	int iEqu;
	int iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	if (pstMon->iDefPos >= 0 && pstMon->iDefPos < pstObj->iMonsterDef)
	{
		pstMonDef = (MONSTERDEF *)pstObj->sMonsterDef[pstMon->iDefPos];
		if ((int)pstMonDef->MonsterID == pstMon->iDefIdx)
		{
			return pstMonDef;
		}
	}

	stMonDef.MonsterID = pstMon->iDefIdx;
	iIndex = bsearch_int(&stMonDef, pstObj->sMonsterDef, pstObj->iMonsterDef, MONSTER_DEF_DATA, &iEqu);
	if (iEqu)
	{
		pstMon->iDefPos = iIndex;
		return (MONSTERDEF *)pstObj->sMonsterDef[iIndex];
	}

	return NULL;
}

NPCSELLLIST *z_get_npc_selllist_ex(ZONESVRENV* pstEnv, int iID)
{
	NPCSELLLIST stSellList;
	int iEqu;
	int iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	

	stSellList.SellListID = iID;
	iIndex = bsearch_int(&stSellList, pstObj->sSellDef, pstObj->iSellDef, SELL_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (NPCSELLLIST *)pstObj->sSellDef[iIndex];
	}

	return NULL;
}


NPCSELLLIST *z_get_npc_selllist(ZONESVRENV* pstEnv, Npc *pstNpc)
{
	NPCSELLLIST stSellList;
	NPCSELLLIST *pstSellList;
	int iEqu;
	int iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	if (pstNpc->nSellListIdx >= 0 && pstNpc->nSellListIdx < pstObj->iSellDef)
	{
		pstSellList = (NPCSELLLIST *)pstObj->sSellDef[pstNpc->nSellListIdx];
		if ((int)pstSellList->SellListID == pstNpc->nSellListID)
		{
			return pstSellList;
		}
	}

	stSellList.SellListID = pstNpc->nSellListID;
	iIndex = bsearch_int(&stSellList, pstObj->sSellDef, pstObj->iSellDef, SELL_DEF_DATA, &iEqu);
	if (iEqu)
	{
		pstNpc->nSellListIdx = iIndex;
		return (NPCSELLLIST *)pstObj->sSellDef[iIndex];
	}

	return NULL;
}

/*
TRANSLIST *z_get_npc_translist(ZONESVRENV* pstEnv, Npc *pstNpc)
{
	TRANSLIST stTransList;
	TRANSLIST *pstTransList;
	int iEqu;
	int iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	if (pstNpc->nTransListIdx >= 0 && pstNpc->nTransListIdx < pstObj->iTransDef)
	{
		pstTransList = (TRANSLIST *)pstObj->sTransDef[pstNpc->nTransListIdx];
		if ((int)pstTransList->TransListID == pstNpc->nTransListID)
		{
			return pstTransList;
		}
	}

	stTransList.TransListID = pstNpc->nTransListID;
	iIndex = bsearch_int(&stTransList, pstObj->sTransDef, pstObj->iTransDef, TRANS_DEF_DATA, &iEqu);
	if (iEqu)
	{
		pstNpc->nTransListIdx = iIndex;
		return (TRANSLIST *)pstObj->sTransDef[iIndex];
	}

	return NULL;
}
*/

int is_convoy_car(Monster* pstMon)
{
	if(pstMon->bSubType == MON_SUB_TYPE_SINGLE_CAR || pstMon->bSubType == MON_SUB_TYPE_TEAM_CAR 
		|| pstMon->bSubType == MON_SUB_TYPE_SINGLE_SAFECAR || pstMon->bSubType == MON_SUB_TYPE_TEAM_SAFECAR )
	{
		return 1;
	}
	
	return 0;
}

int is_my_convoy_car(Player *pstPlayer, Monster* pstMon)
{
	if(is_convoy_car(pstMon) && 
		pstPlayer->stRoleData.MiscInfo.ConvoyInfo.ConvoyMonID == pstMon->iID)
	{
		return 1;
	}

	return 0;
}

int is_my_team_convoy_car(ZONESVRENV* pstEnv,Player *pstPlayer, Monster* pstMon)
{
	ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);

	if (!pstTeam)
	{
		return 0;
	}
	
	if(	pstMon->bSubType == MON_SUB_TYPE_TEAM_CAR || 
		pstMon->bSubType == MON_SUB_TYPE_TEAM_SAFECAR)
	{
		if (pstMon->stTeamInfo.ullTeamID == pstTeam->stTeamInfo.TeamID)
		{
			return 1;
		}
	}
	return 0;
}


int  z_fill_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster* pstMon, MONSTERDEF *pstMonDef)
{
	MONDYNATTRDEF *pstMonDynAttr = NULL;
	ZONEPWORLDINST *pstPworldInst = NULL;
	ZONEMAPINST *pstMapInst;
	ZONEPWORLD* pstPworld = NULL;
	
	if (!z_valid_pos(pstEnv->pstMapObj, pstMon->iInitMap, &pstMon->iInitMapPos, &pstMon->stInitPos)) 
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "monster %d in %d map pos(%d,%d) error", 
					pstMon->iDefIdx, pstMon->iInitMap, pstMon->stInitPos.X, pstMon->stInitPos.Y);
		return -1;
	}

	if (0 > z_mon_ai_data(pstEnv, pstMon, pstMonDef))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "monster %d ai data error", pstMon->iDefIdx);
		return -1;
	}

	if ( pstMon->bSubType < MON_SUB_TYPE_TMP )
	{
		pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
		if (pstMapInst)
		{
			pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
		}
	}


	if (pstPworldInst && pstPworldInst->DynType && pstMonDef->DynAttr)
	{
		pstPworld	= z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
		if( !pstPworld )
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "z_in_destroy_pworld_inst: Pworld %d defid %d not find", 
						pstPworldInst->iPworldInstID, pstPworldInst->stPworldIdx.iID);
		}
		
		pstMonDynAttr = z_find_mondyn(pstEnv, pstPworldInst->bPlayerMeanLevel);
		if (pstPworld != NULL && pstMonDynAttr != NULL  && pstPworldInst->stPworldselect.PworldGrade < MAX_DYN_GRADE )
		{
			PWORLDDYNATTR *pstDyn = &pstPworld->stPworldDef.DynAttr;
			
			pstMon->stMonDynAttr.Level = pstMonDynAttr->Level;
			pstMon->stMonDynAttr.IceDefMul = pstMonDynAttr->IceDefMul;
			pstMon->stMonDynAttr.PhsicDefMul = pstMonDynAttr->PhsicDefMul;
			pstMon->stMonDynAttr.HitMul = pstMonDynAttr->HitMul;
			pstMon->stMonDynAttr.FleeMul = pstMonDynAttr->FleeMul;
			pstMon->stMonDynAttr.TmpLevel = pstMonDynAttr->TmpLevel;
			pstMon->stMonDynAttr.IceHurtDefMul= pstMonDynAttr->IceHurtDefMul;
			pstMon->stMonDynAttr.PhsicHurtDefMul= pstMonDynAttr->PhsicHurtDefMul;
			memcpy(&pstMon->stMonDynAttr.ColorLevel, &pstMonDynAttr->ColorLevel, sizeof(pstMon->stMonDynAttr.ColorLevel));
			
			pstMon->stMonDynAttr.HPMul = pstMonDynAttr->DynHPMul[pstPworldInst->stPworldselect.PworldGrade]*(1 + pstDyn->DynHPMul[pstPworldInst->stPworldselect.PworldGrade]/100.0);
			pstMon->stMonDynAttr.DynHurtMul = pstMonDynAttr->DynHurtMul[pstPworldInst->stPworldselect.PworldGrade]*(1 + pstDyn->DynHurtMul[pstPworldInst->stPworldselect.PworldGrade]/100.0);
			pstMon->stMonDynAttr.ExpMul = pstMonDynAttr->DynExpMul[pstPworldInst->stPworldselect.PworldGrade]*(1 + pstDyn->DynExpMul[pstPworldInst->stPworldselect.PworldGrade]/100.0);
			pstMon->stMonDynAttr.IceHurtMul = pstMonDynAttr->IceHurtMul;
			pstMon->stMonDynAttr.PhsicHurtMul = pstMonDynAttr->PhsicHurtMul;
		}
		else
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "z_find_mondyn  error level= %d PworldGrade = %d",pstPworldInst->bPlayerMeanLevel, pstPworldInst->stPworldselect.PworldGrade);
		}
	}
	
	if (!pstMonDynAttr)
	{
		pstMon->stMonDynAttr.Level = pstMonDef->Level;
	}

	z_check_mon_script(pstEnv, pstMon, pstMonDef);
	
	return 0;
}

int z_refresh_mon_speech(ZONESVRENV* pstEnv, Monster* pstMon)
{
	MONSTERSPEECHDEF *pstMonSpeech;

	pstMonSpeech = z_find_monsterspeech_def(pstEnv,  pstMon->iDefIdx, NULL);
	if (pstMonSpeech)
	{
		int iActIdx;

		iActIdx = z_find_actspeech_by_conditiontype( pstMonSpeech, SPEECH_MON_REFRESH, 0);
		if (iActIdx > -1
			&&	pstMonSpeech->MonActSpeech[iActIdx].ConditionVal != 0
			&&	 (int )RAND1(10000) < pstMonSpeech->MonActSpeech[iActIdx].ConditionVal*100)
		{
			z_mon_speech(pstEnv, pstMon, NULL, pstMonSpeech->MonActSpeech[iActIdx].BroadcastType, 
							SPEECH_MON_REFRESH, iActIdx);
		}
	}

	return 0;
}

int z_refresh_mon_comm_union_data(ZONESVRENV *pstEnv, Monster *pstMon, int iCommType)
{
	if( pstMon->iCommType != 0 &&
		pstMon->iCommType != iCommType)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "mon refresh error, iCommType old %d iCommType new %d",
			pstMon->iCommType,iCommType);
		return -1;
	}
	
	pstMon->iCommType = iCommType;

	return 0;
}

//注意,pstDoCallSkillDef有可能是NULL
int mon_refresh_comm_union(ZONESVRENV *pstEnv, Monster *pstMon,SKILLDEF *pstDoCallSkillDef)
{
	MONSTERDEF *pstMonDef = NULL;
	
	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (!pstMonDef)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "mon %d no def", pstMon->iDefIdx);
		return -1;
	}

	//使用之前赋值iCommType，刷新前检查防范
	if(is_mon_machine(pstMon))
	{
		if(0 != z_refresh_mon_comm_union_data(pstEnv, pstMon, MON_UNION_TYPE_MACHINE) )
		{
			return -1;
		}
	}

	if(MON_SUB_TYPE_XUNLUO == pstMon->bSubType)
	{
		if(0 != z_refresh_mon_comm_union_data(pstEnv, pstMon, MON_UNION_TYPE_XUNLUO_MON))
		{
			return -1;
		}
		
		z_do_call_pos_xunluo_mon( pstEnv, pstMon,pstDoCallSkillDef );
	}

	return 0;
}

#if defined(ZONE_ID_TRACKED)
int z_refresh_mon_tracked(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
                          Monster *pstMon,SKILLDEF *pstDoCallSkillDef, const char *pszFile, int iLine)
#else
int z_refresh_mon(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster* pstMon,SKILLDEF *pstDoCallSkillDef)
#endif
{
	ZONEMAPINST *pstMapInst;
	MONSTERDEF *pstMonDef;
	int iRet;
	AIEvt stAIEvt;
	MapIndex * pstMapIdx;
	ZONEPWORLDINST *pstPworldInst;
	BCTMON *pstBctMon;

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (!pstMonDef)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "mon %d no def", pstMon->iDefIdx);
		return -1;
	}

	pstMon->bSubType = pstMonDef->SubType;
	
	if (pstMon->cType != OBJ_MONSTER && pstMon->cType != OBJ_NPC)	
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "refresh mon type = %d", pstMon->cType);
		return -1;
	}

	if (0 == pstMon->cFirstBorn && pstMon->cStat == MONSTER_DIE &&
		pstMon->tLeaveMap)  
	{
		z_monster_leave_map(pstAppCtx, pstEnv, pstMon);
	}

	if(!pstMon->cRefresh && !pstMon->cFirstBorn)
	{
		z_free_monster(pstAppCtx, pstEnv, pstMon);
		return 0;
	}

	if (pstMon->iID > 0)
	{
		z_id_clean(pstEnv, pstMon->iID);
	}
	
	memset((void *)((char *)pstMon + offsetof(Monster, cFirstBorn)), 0, sizeof(*pstMon)-offsetof(Monster, cFirstBorn));
	
	mon_refresh_skill(pstAppCtx, pstEnv, pstMon);
	pstMon->iID = z_get_mem_id(pstAppCtx, pstEnv, pstMon->iMIndex, pstMon->cType);
#if defined(ZONE_ID_TRACKED)
	tlog_info(pstEnv->pstLogCat, 0, 0,
	          "Allocate id, From:%s(%d): Type=Monster, DefID=%d, MemID=%d, ID=%d",
	          pszFile, iLine, pstMon->iDefIdx, pstMon->iMIndex, pstMon->iID);
#endif
	pstMon->stMspdInfo.MoveSpdCurr = pstMonDef->MoveSpd;
	TV_CLONE(pstMon->stMonAtkInfo.stLastAtk, pstAppCtx->stCurr);
	TV_CLONE(pstMon->stLastMoveTv, pstAppCtx->stCurr);
	pstMon->tBirthEnd = pstAppCtx->stCurr.tv_sec + pstMonDef->BirthTime;

	
	pstMon->cStat = MONSTER_ALIVE;

	//刷新之前检查iCommType 对应  stCommUnion, 并清理数据
	if(0 > mon_refresh_comm_union(pstEnv, pstMon,pstDoCallSkillDef))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "MonsterID %d  CommType = %d",
				           pstMon->iDefIdx, pstMon->iCommType);
		return -1;
	}		

	//百层塔德怪物指定数值
	pstBctMon = get_mon_bct_dyn(pstEnv, pstMon);
	if(pstBctMon)
	{
		pstMon->iMaxHP = pstBctMon->HPMul;
		pstMon->iHP = pstMon->iMaxHP;
	}
	else
	{
		if (pstMonDef->DynAttr)
		{
			pstMon->iMaxHP = pstMonDef->HP * (1 + pstMon->stMonDynAttr.HPMul/100.0) ;
			if(pstMon->iMaxHpbyCall > 0)
			{
				pstMon->iMaxHP = pstMon->iMaxHpbyCall * (1 + pstMon->stMonDynAttr.HPMul/100.0) ;
			}
			pstMon->iHP = pstMon->iMaxHP;
		}
		else
		{
			pstMon->iMaxHP = pstMonDef->HP;
			if(pstMon->iMaxHpbyCall > 0)
			{
				pstMon->iMaxHP = pstMon->iMaxHpbyCall;
			}
			pstMon->iHP = pstMon->iMaxHP;
		}
	}
			
	pstMon->stMap.iID	=	pstMon->iInitMap;
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	if ( MOVE_TYPE_ROUNDROLL == pstMonDef->MoveType)
	{
		pstMon->fRoundRollAngle = RAND1(2*PI);
	}
		
	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (pstPworldInst)
	{
		pstPworldInst->iMonHpTotal += pstMon->iMaxHP;
	}

	z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstMon->stInitPos, 
					&pstMon->stInitPos, pstMonDef->RefreshRadius, 0, &pstMon->stCurrPos);

	iRet = z_safe_monster_entermap(pstAppCtx, pstEnv, pstMon, pstMon->iInitMap, &pstMon->stCurrPos);

	if (0 == iRet)
	{
		stAIEvt.iEvtID = AI_EVT_SYSOUT;
		mon_ai_stat(pstEnv, pstMon, &stAIEvt);

		if ((pstMon->cMonScriptFlag & MON_SCRIPT_PROC_NEW_MAP))
		{
			pstMon->bDelayScript |= MON_SCRIPT_PROC_NEW_MAP;
		}

		/*
		if(pstMon->bSubType == MON_SUB_TYPE_JIN  || pstMon->bSubType == MON_SUB_TYPE_BOSS)
		{
			tlog_info(pstEnv->pstLogCat, 0, 0,	"monster refresh: DefID %d, MemID %d, Map %d",
				pstMon->iDefIdx, pstMon->iID, pstMon->stMap.iID);
		}
		*/

		z_refresh_mon_speech(pstEnv, pstMon);
		mon_ai_refreshed_skill(pstEnv, pstMon, &pstMonDef->AIRefreshedSkill);
	}
	
	return iRet;
}

int z_mon_speech(ZONESVRENV* pstEnv, Monster* pstMon, Player *pstPlayer, 
					int iBroadcastType, int iSpeechType, int iSpeechIdx)
{
	if (iBroadcastType == BROADCAST_MON_VIEW &&
		iSpeechType == SPEECH_MON_DEAD)
	{
		return 0;
	}
	
	switch (iBroadcastType)
	{
	case BROADCAST_MON_VIEW:
		z_mon_view_speech(pstEnv, pstMon, pstPlayer, iSpeechType, iSpeechIdx);
		break;
	case BROADCAST_MON_MAP:
		z_mon_map_speech(pstEnv, pstMon, pstPlayer, iSpeechType, iSpeechIdx);
		break;
	case BROADCAST_THE_WORLD:
		z_mon_world_speech(pstEnv, pstMon, pstPlayer, iSpeechType, iSpeechIdx);
		break;
	default:
		break;
	}
	
	return 0;
}

int z_mon_view_speech_msg(ZONESVRENV* pstEnv, Monster* pstMon,
                          Player *pstPlayer, const char *pszMsg,
                          int iSpeechType)
{
	int i;
	int iCount = 0;
	Player *apstSendPlayer[MAX_MONSTER_VPLAYER];
	
	for (i=0; i<pstMon->iVPlayer; i++)
	{
		apstSendPlayer[iCount] = z_id_player(pstEnv->pstAppCtx, pstEnv,
		                                     pstMon->aiVPlayer[i]);
		if(!apstSendPlayer[iCount] ||
		   apstSendPlayer[iCount]->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}
		
		iCount++;
	}

	if(iCount > 0)
	{
		CSPKG stPkg;
		CSMONSPEECHEX *pstMonSpeech = &stPkg.Body.MonSpeechEx;

		pstMonSpeech->MonsterID = pstMon->iDefIdx;
		pstMonSpeech->MemID= pstMon->iID;
		pstMonSpeech->SpeechType = iSpeechType;
		STRNCPY(pstMonSpeech->Content, pszMsg, CCH(pstMonSpeech->Content));
		pstMonSpeech->Career = 0;
		pstMonSpeech->Gender = 0;
		pstMonSpeech->RoleName[0] = 0;
		if(pstPlayer)
		{
			pstMonSpeech->Career = pstPlayer->stRoleData.Career;
			pstMonSpeech->Gender = pstPlayer->stRoleData.Gender;
			STRNCPY(pstMonSpeech->RoleName, pstPlayer->stRoleData.RoleName, CCH(pstMonSpeech->RoleName));
		}
		
		Z_CSHEAD_INIT(&stPkg.Head, MON_SPEECH_EX);
		return z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv,
		                                apstSendPlayer, iCount, &stPkg);
	}

	return 0;
}

int z_mon_map_speech_msg(ZONESVRENV *pstEnv, Monster *pstMon, Player *pstPlayer,
                         const char *pszMsg, int iSpeechType)
{
	int j,k;
	int iDyn;
	int iCount = 0;
	AREADYN *pstAreaDyn; 
	Player  *apstSendPlayer[MAX_PLAYER];
	ZONEMAPINST *pstMapInst;
	ZONEPWORLDINST *pstPworldInst;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if(!pstMapInst)
	{
		return -1;
	}

	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj,
	                                   pstMapInst->iPworldInstID);
	if(pstPworldInst)
	{
		CSPKG stPkg;
		CSMONSPEECHEX *pstMonSpeech = &stPkg.Body.MonSpeechEx;

		pstMonSpeech->MonsterID = pstMon->iDefIdx;
		pstMonSpeech->MemID= pstMon->iID;
		pstMonSpeech->SpeechType = iSpeechType;
		STRNCPY(pstMonSpeech->Content, pszMsg, CCH(pstMonSpeech->Content));
		pstMonSpeech->Career = 0;
		pstMonSpeech->Gender = 0;
		pstMonSpeech->RoleName[0] = 0;
		if (pstPlayer)
		{
			pstMonSpeech->Career = pstPlayer->stRoleData.Career;
			pstMonSpeech->Gender = pstPlayer->stRoleData.Gender;
			STRNCPY(pstMonSpeech->RoleName, pstPlayer->stRoleData.RoleName,
			        CCH(pstMonSpeech->RoleName));
		}
		
		Z_CSHEAD_INIT(&stPkg.Head, MON_SPEECH_EX);
		return pworld_map_broadcast(pstEnv->pstAppCtx, pstEnv, pstPworldInst, pstMapInst, 0,
		                        &stPkg);
	}

	iCount = 0;
	for(j = 0; j < pstMapInst->iAreaDyn; j++)
	{
		iDyn = pstMapInst->aiAreaDyn[j];
		if( iDyn<0)
		{
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		for(k = 0; k < pstAreaDyn->iAnimate; k++)
		{
			if(k >= MAX_AREA_ANIMATE)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d",
				           pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
				break;
			}

			apstSendPlayer[iCount] = z_id_player(pstEnv->pstAppCtx, pstEnv,
			                                     pstAreaDyn->aiAnimate[k]);
			if(!apstSendPlayer[iCount] ||
			   apstSendPlayer[iCount]->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				continue;
			}

			iCount++;
		}
	}

	if (iCount)
	{
		CSPKG stPkg;
		CSMONSPEECHEX *pstMonSpeech = &stPkg.Body.MonSpeechEx;

		pstMonSpeech->MonsterID = pstMon->iDefIdx;
		pstMonSpeech->MemID= pstMon->iID;
		pstMonSpeech->SpeechType = iSpeechType;
		STRNCPY(pstMonSpeech->Content, pszMsg, CCH(pstMonSpeech->Content));
		pstMonSpeech->Career = 0;
		pstMonSpeech->Gender = 0;
		pstMonSpeech->RoleName[0] = 0;
		if (pstPlayer)
		{
			pstMonSpeech->Career = pstPlayer->stRoleData.Career;
			pstMonSpeech->Gender = pstPlayer->stRoleData.Career;
			STRNCPY(pstMonSpeech->RoleName, pstPlayer->stRoleData.RoleName,
			        CCH(pstMonSpeech->RoleName));
		}
		
		Z_CSHEAD_INIT(&stPkg.Head, MON_SPEECH_EX);
		return z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv,
		                                apstSendPlayer, iCount, &stPkg);
	}

	return 0;
}

int z_mon_world_speech_msg(ZONESVRENV *pstEnv, Monster *pstMon,
                           Player *pstPlayer, const char *pszMsg,
                           int iSpeechType)
{
	CSPKG stPkg;
	CSMONSPEECHEX *pstMonSpeech = &stPkg.Body.MonSpeechEx;

	pstMonSpeech->MonsterID = pstMon->iDefIdx;
	pstMonSpeech->MemID= pstMon->iID;
	pstMonSpeech->SpeechType = iSpeechType;
	STRNCPY(pstMonSpeech->Content, pszMsg, CCH(pstMonSpeech->Content));
	pstMonSpeech->Career = 0;
	pstMonSpeech->Gender = 0;
	pstMonSpeech->RoleName[0] = 0;
	if (pstPlayer)
	{
		pstMonSpeech->Career = pstPlayer->stRoleData.Career;
		pstMonSpeech->Gender = pstPlayer->stRoleData.Gender;
		STRNCPY(pstMonSpeech->RoleName, pstPlayer->stRoleData.RoleName, CCH(pstMonSpeech->RoleName));
	}
		
	Z_CSHEAD_INIT(&stPkg.Head, MON_SPEECH_EX);
	return z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
}

int z_mon_self_speech_msg(ZONESVRENV *pstEnv, Monster *pstMon,
                          Player *pstPlayer, const char *pszMsg,
                          int iSpeechType)
{

	CSPKG stPkg;
	CSMONSPEECHEX *pstMonSpeech = &stPkg.Body.MonSpeechEx;

	if(!pstPlayer)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "player can not be null.");
		return -1;
	}

	pstMonSpeech->MonsterID = pstMon->iDefIdx;
	pstMonSpeech->MemID= pstMon->iID;
	pstMonSpeech->SpeechType = iSpeechType;
	STRNCPY(pstMonSpeech->Content, pszMsg, CCH(pstMonSpeech->Content));
	pstMonSpeech->Career = pstPlayer->stRoleData.Career;
	pstMonSpeech->Gender = pstPlayer->stRoleData.Gender;
	STRNCPY(pstMonSpeech->RoleName, pstPlayer->stRoleData.RoleName,
	        CCH(pstMonSpeech->RoleName));

	Z_CSHEAD_INIT(&stPkg.Head, MON_SPEECH_EX);
	return z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
}

int z_mon_speech_ex(ZONESVRENV* pstEnv, Monster* pstMon, Player *pstPlayer,
                    const char *pszMsg, int iBroadcastType, int iSpeechType)
{
	if (iBroadcastType == BROADCAST_MON_VIEW &&
		iSpeechType == SPEECH_MON_DEAD)
	{
		return 0;
	}
	
	switch (iBroadcastType)
	{
	case BROADCAST_ONE_SELF:
		z_mon_self_speech_msg(pstEnv, pstMon, pstPlayer, pszMsg, iSpeechType);
		break;
	case BROADCAST_MON_VIEW:
		z_mon_view_speech_msg(pstEnv, pstMon, pstPlayer, pszMsg, iSpeechType);
		break;
	case BROADCAST_MON_MAP:
		z_mon_map_speech_msg(pstEnv, pstMon, pstPlayer, pszMsg, iSpeechType);
		break;
	case BROADCAST_THE_WORLD:
		z_mon_world_speech_msg(pstEnv, pstMon, pstPlayer, pszMsg, iSpeechType);
		break;
	default:
		break;
	}
	
	return 0;
}

int z_mon_view_speech(ZONESVRENV* pstEnv, Monster* pstMon, Player *pstPlayer, int iSpeechType, int iSpeechIdx)
{
	int i;
	int iCount = 0;
	Player  *apstSendPlayer[MAX_MONSTER_VPLAYER];
	
	for (i=0; i<pstMon->iVPlayer; i++)
	{
		apstSendPlayer[iCount] = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->aiVPlayer[i]);
		if (NULL == apstSendPlayer[iCount] || PLAYER_STATUS_ROLE_LOGIN != apstSendPlayer[iCount]->eStatus)
		{
			continue;
		}
		
		iCount++;
	}

	if (iCount)
	{
		CSPKG stPkg;
		CSMONSPEECH *pstMonSpeech = &stPkg.Body.MonSpeech;

		pstMonSpeech->MonsterID = pstMon->iDefIdx;
		pstMonSpeech->MemID= pstMon->iID;
		pstMonSpeech->SpeechType = iSpeechType;
		pstMonSpeech->SpeechIdx = iSpeechIdx;
		pstMonSpeech->Career = 0;
		pstMonSpeech->Gender = 0;
		pstMonSpeech->RoleName[0] = 0;
		if (pstPlayer)
		{
			pstMonSpeech->Career = pstPlayer->stRoleData.Career;
			pstMonSpeech->Gender = pstPlayer->stRoleData.Gender;
			STRNCPY(pstMonSpeech->RoleName, pstPlayer->stRoleData.RoleName, CCH(pstMonSpeech->RoleName));
		}
		
		Z_CSHEAD_INIT(&stPkg.Head, MON_SPEECH);
		return z_cltmsg_limit_broadcast( pstEnv->pstAppCtx, pstEnv, apstSendPlayer, iCount, &stPkg );
	}

	return 0;
}

int z_mon_map_speech(ZONESVRENV* pstEnv, Monster* pstMon, Player *pstPlayer, int iSpeechType, int iSpeechIdx)
{
	int j,k;
	int iDyn;
	int iCount = 0;
	AREADYN *pstAreaDyn; 
	Player  *apstSendPlayer[MAX_PLAYER];
	ZONEMAPINST *pstMapInst;
	ZONEPWORLDINST *pstPworldInst;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (pstPworldInst)
	{
		CSPKG stPkg;
		CSMONSPEECH *pstMonSpeech = &stPkg.Body.MonSpeech;

		pstMonSpeech->MonsterID = pstMon->iDefIdx;
		pstMonSpeech->MemID= pstMon->iID;
		pstMonSpeech->SpeechType = iSpeechType;
		pstMonSpeech->SpeechIdx = iSpeechIdx;
		pstMonSpeech->Career = 0;
		pstMonSpeech->Gender = 0;
		pstMonSpeech->RoleName[0] = 0;
		if (pstPlayer)
		{
			pstMonSpeech->Career = pstPlayer->stRoleData.Career;
			pstMonSpeech->Gender = pstPlayer->stRoleData.Gender;
			STRNCPY(pstMonSpeech->RoleName, pstPlayer->stRoleData.RoleName, CCH(pstMonSpeech->RoleName));
		}
		
		Z_CSHEAD_INIT(&stPkg.Head, MON_SPEECH);
		return pworld_map_broadcast(pstEnv->pstAppCtx, pstEnv, pstPworldInst, pstMapInst, 0, &stPkg);
	}

	iCount = 0;
	for (j=0; j<pstMapInst->iAreaDyn; j++)
	{
		iDyn	 =	pstMapInst->aiAreaDyn[j];
		if( iDyn<0)
		{
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		for(k=0; k<pstAreaDyn->iAnimate; k++)
		{
			if( k>=MAX_AREA_ANIMATE )
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d", pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
				break;
			}

			apstSendPlayer[iCount] = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAreaDyn->aiAnimate[k]);
			if (NULL == apstSendPlayer[iCount] || PLAYER_STATUS_ROLE_LOGIN != apstSendPlayer[iCount]->eStatus)
			{
				continue;
			}

			iCount++;
		}
	}

	if (iCount)
	{
		CSPKG stPkg;
		CSMONSPEECH *pstMonSpeech = &stPkg.Body.MonSpeech;

		pstMonSpeech->MonsterID = pstMon->iDefIdx;
		pstMonSpeech->MemID= pstMon->iID;
		pstMonSpeech->SpeechType = iSpeechType;
		pstMonSpeech->SpeechIdx = iSpeechIdx;
		pstMonSpeech->Career = 0;
		pstMonSpeech->Gender = 0;
		pstMonSpeech->RoleName[0] = 0;
		if (pstPlayer)
		{
			pstMonSpeech->Career = pstPlayer->stRoleData.Career;
			pstMonSpeech->Gender = pstPlayer->stRoleData.Career;
			STRNCPY(pstMonSpeech->RoleName, pstPlayer->stRoleData.RoleName, CCH(pstMonSpeech->RoleName));
		}
		
		Z_CSHEAD_INIT(&stPkg.Head, MON_SPEECH);
		return z_cltmsg_limit_broadcast( pstEnv->pstAppCtx, pstEnv, apstSendPlayer, iCount, &stPkg );
	}

	return 0;
}

int z_mon_world_speech(ZONESVRENV* pstEnv, Monster* pstMon, Player *pstPlayer, int iSpeechType, int iSpeechIdx)
{
	CSPKG stPkg;
	CSMONSPEECH *pstMonSpeech = &stPkg.Body.MonSpeech;

	pstMonSpeech->MonsterID = pstMon->iDefIdx;
	pstMonSpeech->MemID= pstMon->iID;
	pstMonSpeech->SpeechType = iSpeechType;
	pstMonSpeech->SpeechIdx = iSpeechIdx;
	pstMonSpeech->Career = 0;
	pstMonSpeech->Gender = 0;
	pstMonSpeech->RoleName[0] = 0;
	if (pstPlayer)
	{
		pstMonSpeech->Career = pstPlayer->stRoleData.Career;
		pstMonSpeech->Gender = pstPlayer->stRoleData.Gender;
		STRNCPY(pstMonSpeech->RoleName, pstPlayer->stRoleData.RoleName, CCH(pstMonSpeech->RoleName));
	}
		
	Z_CSHEAD_INIT(&stPkg.Head, MON_SPEECH);
	return z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
}

int  z_find_actspeech_by_conditiontype(MONSTERSPEECHDEF *pstMonSpeech, int iType, int iValue)
{
	int i;
	int iIdx = -1;
	
	for (i = 0; i<MAX_MON_ACT_SPEECH; i++)
	{
		if (pstMonSpeech->MonActSpeech[i].ConditionType == 0)
		{
			break;
		}

		if (pstMonSpeech->MonActSpeech[i].ConditionType != iType )
		{
			continue;
		}

		//过滤内容空的
		if (pstMonSpeech->MonActSpeech[i].Desc[0] == 0 )
		{
			break;
		}

		if (iValue == 0)
		{
			iIdx = i;
			break;
		}

		if (pstMonSpeech->MonActSpeech[i].ConditionVal >= iValue )
		{
			if (iIdx == -1
				|| pstMonSpeech->MonActSpeech[i].ConditionVal < pstMonSpeech->MonActSpeech[iIdx].ConditionVal )
			{
				iIdx = i;
			}
		}
	}

	if (iIdx >= 0)
	{
		return iIdx;
	}
	
	return -1;
}

int z_mon_resile(ZONESVRENV* pstEnv, Monster* pstMon)
{
	Monster stMon;
	MONSTERDEF *pstMonDef;

	stMon.iDefIdx = pstMon->iFormDefID;
	stMon.iDefPos = -1;
	pstMonDef = z_get_mon_def(pstEnv, &stMon);
	if (!pstMonDef)
	{
		return -1;
	}

	pstMon->iDefIdx =stMon.iDefIdx;
	pstMon->iDefPos = stMon.iDefPos;
	pstMon->iFormDefID = 0;

	//动态属性修正 、脚本 填充
	if (0 > z_fill_monster(pstEnv->pstAppCtx, pstEnv, pstMon,  pstMonDef))
	{
		return -1;
	}
	
	return 0;
}

int z_mon_aberrants(ZONESVRENV* pstEnv, Monster* pstMon)
{
	int i;
	Monster stNewMon;
	MONSTERDEF *pstNewMonDef, *pstMonDef;

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (!pstMonDef)
	{
		return 0;
	}

	for(i =0; i<MAX_MON_ABERRANT;i++)
	{
		if (pstMonDef->Aberrants[i].MonsterID == 0)
		{
			return 0;
		}

		if (pstMonDef->Aberrants[i].Rate != 0 && (int)RAND1(10000) < pstMonDef->Aberrants[i].Rate )
		{
			break;
		}
	}

	if (i >= MAX_MON_ABERRANT)
	{
		return 0;
	}

	stNewMon.iDefIdx = pstMonDef->Aberrants[i].MonsterID;
	stNewMon.iDefPos = -1;
	pstNewMonDef = z_get_mon_def(pstEnv, &stNewMon);
	if (!pstNewMonDef)
	{
		return 0;
	}

	pstMon->iFormDefID = pstMon->iDefIdx;
	pstMon->iDefIdx =stNewMon.iDefIdx;
	pstMon->iDefPos = stNewMon.iDefPos;

	//动态属性修正 、脚本 填充
	if (0 > z_fill_monster(pstEnv->pstAppCtx, pstEnv, pstMon,  pstNewMonDef))
	{
		return z_mon_resile(pstEnv, pstMon);
	}
	
	return 0;
}

Player * get_atker_player(ZONESVRENV* pstEnv,  ZoneAni *pstAtker)
{
	Player *pstPlayer = NULL;

	if (!pstAtker)
		return NULL;

	switch(pstAtker->iType)
	{
		case OBJ_PLAYER:
			pstPlayer = pstAtker->stObj.pstPlayer;
			break;
		case OBJ_PET:
			pstPlayer = pstAtker->stObj.stZonePet.pstOwner;
			break;
		case OBJ_MONSTER:
			if (pstAtker->stObj.pstMon->iOwnerID > 0)
			{
				pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAtker->stObj.pstMon->iOwnerID);
			}
			break;
		default:
			pstPlayer = NULL;
			break;
	}
	return pstPlayer;
}

int player_or_team_task_trace_mon(ZONESVRENV *pstEnv, Monster *pstMon)
{
	ZoneTeam *pstTeam = NULL;
	Player *pstPlayer = NULL;

	if(!pstMon->iFirstAtkerID && !pstMon->szOwner[0])
	{
		return -1;
	}
	
	if(!pstMon->szOwner[0] || is_convoy_car(pstMon))
	{
		if(pstMon->cFirstAtkerType == MON_BOOTY_OWNER_PLAYER)
		{
			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->iFirstAtkerID);
		}
		else if(pstMon->cFirstAtkerType == MON_BOOTY_OWNER_TEAM)
		{
			pstTeam = z_idx_team(pstEnv, pstMon->iFirstAtkerID, pstMon->ullFirstAtkerWid);
		}
	}
	else
	{
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstMon->szOwner);
	}
	
	if(!pstPlayer && !pstTeam)
	{
		return -1;
	}

	if(pstPlayer)
	{	
		pstTeam = player_team_get(pstEnv, pstPlayer);
	}
	
	if(!pstTeam)
	{
		if(!pstPlayer)
			return -1;
		
		if((pstPlayer->stOnline.cMiscFlag & MISC_FLAG_TRACE) &&
		   mon_trace_valid_player(pstMon, pstPlayer) == 0)
		{
			task_trace_mon(pstEnv, pstPlayer, pstMon);
		}
	}
	else
	{
		int i = 0;
		for(i = 0; i < pstTeam->stTeamInfo.MemberNum; ++i)
		{
			Player *pstPlayerTmp = z_name_player(pstEnv->pstAppCtx, pstEnv,
			                      pstTeam->stTeamInfo.TeamMember[i].MemberName);
			if(!pstPlayerTmp)
			{
				continue;
			}

			if((pstPlayerTmp->stOnline.cMiscFlag & MISC_FLAG_TRACE) &&
			   mon_trace_valid_player(pstMon, pstPlayerTmp) == 0)
			{
				task_trace_mon(pstEnv, pstPlayerTmp, pstMon);
			}
		}
	}
	return 0;
}

Player* mon_booty_owner_get_byteam(ZONESVRENV* pstEnv, Monster* pstMon, ZoneTeam *pstTeam)
{
	int i;
	Player *pstPlayerTmp;
	
	for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		pstPlayerTmp = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
		if (NULL == pstPlayerTmp || 0 > mon_booty_valid_player(pstMon, pstPlayerTmp))
		{
			continue;
		}
		
		return pstPlayerTmp;
	}
	
	return NULL;
}

int z_mon_die(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster* pstMon, 
				MONSTERDEF *pstMonDef, ZoneAni *pstAtker, int iNotify, int fPworldSrcipt)
{
	CSPKG stPkg;
	ZONEPWORLDINST *pstPworldInst;
	ZONEMAPINST *pstMapInst;
	char szName[128];
	CSACTIONRES *pstActRes = &stPkg.Body.ActionRes;
	MONSTERSPEECHDEF *pstMonSpeech;
	Player *pstPlayer = NULL;
	int fLinkedAllDie = 1;
	ZoneTeam *pstTeam = NULL;
	Player* pstKiller = NULL;
	int iZJret = 0; 

	//无效的怪物
	if (NULL == z_id_monster2(pstAppCtx, pstEnv, pstMon->iID))
	{
		return -1;
	}
	
	if (MONSTER_DIE == pstMon->cStat)
	{
		return -1;
	}

	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	//死亡相关日志流水号
	z_oplog_event_begin(pstEnv);

	pstMon->cStat = MONSTER_DIE;
	pstMon->cMoving = 0;
	pstMon->tRefresh = pstAppCtx->stCurr.tv_sec + pstMonDef->RefreshTime;
	pstMon->tLeaveMap = pstAppCtx->stCurr.tv_sec + MON_LEAVE_MAP_TIME;

	pstMon->bAiStat = AI_STAT_DEAD;

	if (0 == pstMon->szOwner[0] || is_convoy_car(pstMon) || is_city_build_mon(pstMon))
	{
		if (pstMon->iFirstAtkerID)
		{
			if(pstMon->cFirstAtkerType == MON_BOOTY_OWNER_PLAYER)
			{
				pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->iFirstAtkerID);
			}
			else if(pstMon->cFirstAtkerType == MON_BOOTY_OWNER_TEAM)
			{
				pstTeam =  z_idx_team(pstEnv, pstMon->iFirstAtkerID, pstMon->ullFirstAtkerWid);
				if(pstTeam)
				{
					pstPlayer = mon_booty_owner_get_byteam(pstEnv, pstMon, pstTeam);
				}
			}
		}
		else
		{
			pstPlayer = get_atker_player(pstEnv, pstAtker);
		}
	}
	else
	{
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstMon->szOwner);
	}

	if (pstPlayer)
	{
		unreal_up_kill(pstEnv,  pstPlayer,pstMonDef->MonsterID);
		player_get_vig(pstEnv, pstMon, pstPlayer); 
		if (pstMonDef->MonsterID == 241000) // 嘟嘟鸟| 称号
		{
			player_desig_get(pstEnv, pstPlayer, GET_DESIG_KILL_DUDUNIAO);
		}

		poly_pworld_mon_die(pstEnv, pstMon, pstPlayer);
	}

	boss_die( pstEnv, pstMon, pstMonDef);

	pstKiller = pstPlayer;
	if (pstTeam || !pstKiller)
	{
		pstKiller = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->iFirstAtkerMemID);//无视队伍
	}
	
	pstMon->iKillerObjType = OBJ_UNKNOW;
	if (pstKiller)
	{
		
		pstMon->iKillerObjType = OBJ_PLAYER;
		pstMon->iKillerMemID = pstKiller->iMemID;
	}
	else
	{
		 if (pstAtker && pstAtker->iType == OBJ_MONSTER) 
		 {
			pstMon->iKillerObjType = OBJ_MONSTER;
			pstMon->iKillerMemID = pstAtker->stObj.pstMon->iID;
		 }
			
	}

	pstMonSpeech = z_find_monsterspeech_def(pstEnv,  pstMon->iDefIdx, NULL);
	if (pstMonSpeech && pstPlayer)
	{
		int iActIdx;
		
		iActIdx = z_find_actspeech_by_conditiontype( pstMonSpeech, SPEECH_MON_DEAD, 0);
		if (iActIdx > -1
			&&	pstMonSpeech->MonActSpeech[iActIdx].ConditionVal != 0
			&&	 (int )RAND1(10000) < pstMonSpeech->MonActSpeech[iActIdx].ConditionVal*100)
		{
			z_mon_speech(pstEnv, pstMon, pstPlayer, pstMonSpeech->MonActSpeech[iActIdx].BroadcastType, SPEECH_MON_DEAD, iActIdx);
		}
	}

	// xp
	if(pstAtker)
	{
		mon_die_add_xp(pstEnv, pstMon, pstMonDef, pstAtker);
	}
	
	if ((pstMon->cMonScriptFlag & MON_SCRIPT_PROC_PRE_DIE_MAP))
	{
		pstMon->bDelayScript |= MON_SCRIPT_PROC_PRE_DIE_MAP;
		
		stSEnv.pstMonster = pstMon;
		stSEnv.pstNpc = NULL;
		stSEnv.pstPlayer = pstPlayer;
		stSEnv.pstPworldInst = NULL;
		stSEnv.pstKiller = NULL;
		stSEnv.pstMapinst = pstMapInst;;
		
		if (pstAtker)
		{
			stSEnv.pstKiller = get_atker_player(pstEnv, pstAtker);
		}
		
		if(!stSEnv.pstPlayer)
		{
			//为了副本里名字或获取问题
			stSEnv.pstPlayer = stSEnv.pstKiller;
		}
		
		snprintf(szName, sizeof(szName), "monster%d.mac", pstMonDef->MonsterID);
		szName[sizeof(szName) - 1] = 0;
				
		z_exec_script(pstAppCtx, pstEnv, NULL, szName, &pstMon->stScriptIdx, MON_SCRIPT_PROC_PRE_DIE);
		
	}


	if (iNotify == 1)
	{
		pstActRes->ID = pstMon->iID;
		pstActRes->ActionID = ACTION_DEAD;
		pstActRes->Data.Dead.x = pstMon->stCurrPos.X;
		pstActRes->Data.Dead.y = pstMon->stCurrPos.Y;
			
		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		z_sendpkg_monview(pstAppCtx, pstEnv, pstMon, &stPkg, 1);
	}
	else if(iNotify == 2)
	{
		pstActRes->ID = pstMon->iID;
		pstActRes->ActionID = ACTION_DESTROY;
		pstActRes->Data.Destroy.x = pstMon->stCurrPos.X;
		pstActRes->Data.Destroy.y = pstMon->stCurrPos.Y;
			
		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		z_sendpkg_monview(pstAppCtx, pstEnv, pstMon, &stPkg, 1);
	}
	else
	{
		//z_monster_move_notify(pstAppCtx,  pstEnv, pstMon, CS_MOVE_TURN);
	}

	mon_die_status_clear(pstAppCtx, pstEnv, pstMon);

	if(pstAtker)
	{
		z_player_fight_update(pstEnv, pstMon, pstAtker);
	}

	// 据点资源怪
	strong_resmon_die(pstEnv, pstMon, pstMonDef, get_atker_player(pstEnv, pstAtker));

	// 跨服据点资源怪
	span_strong_resmon_die(pstEnv, pstMon, pstMonDef, get_atker_player(pstEnv, pstAtker));

	//机械死亡
	mon_machine_die(pstEnv, pstMon, pstAtker);
	
	mon_ore_update(pstEnv, pstMon);
	mon_factory_update(pstEnv, pstMon);

	//佣兵死亡
	mon_bub_del_owner_info(pstEnv, pstMon);

	//公会建筑死亡
	world_city_build_die(pstEnv, pstMon, get_atker_player(pstEnv, pstAtker));
	
	//城市建筑死亡
	clan_city_build_warning_die(pstEnv, pstMon, get_atker_player(pstEnv, pstAtker));
		
	// 对连锁怪物的处理
 	if(pstMon->iLinkedNext >= 0 && pstMon->iLinkedPrev >= 0)
 	{
		Monster *pstLinkedMon = NULL;

		for(pstLinkedMon = tmempool_get(pstEnv->pstMonsterPool,
		                                pstMon->iLinkedNext);
		    pstLinkedMon != pstMon;
		    pstLinkedMon = tmempool_get(pstEnv->pstMonsterPool,
		                                pstLinkedMon->iLinkedNext))
		{
			if (!pstLinkedMon)
			{
				break;
			}

			if (pstLinkedMon->cStat != MONSTER_DIE)
			{
				fLinkedAllDie = 0;
				break;
			}
		}
	}

	//打死才计算死亡数量
	if(pstMon->iFirstAtkerID > 0 || (pstAtker != NULL && pstAtker->iType != OBJ_UNKNOW))
	{
		world_mon_event_die(pstEnv, pstMon);
	}

	if (pstMon->stBctInfo.iBctDefID <= 0 && 
		fLinkedAllDie &&
		!is_mon_bub(pstMon))
	{
		mon_booty_assign(pstEnv, pstMon, pstMonDef, get_atker_player(pstEnv, pstAtker));
	}
	else
	{
		// 没有战利品还是要跟踪任务
		player_or_team_task_trace_mon(pstEnv, pstMon);
	}
	
	if ((pstMapInst->cInstScriptFlag & MAP_INST_SCRIPT_PROC_MON_DIE_MAP ) &&
		pstMonDef->SubType <= MON_SUB_TYPE_JI)
	{
		char szName[128];
		MapIndex *pstMapIdx;
		
		stSEnv.pstMonster = pstMon;
		stSEnv.pstNpc = NULL;
		stSEnv.pstPworldInst = NULL;
		stSEnv.pstPlayer = pstPlayer;
		stSEnv.pstMapinst = pstMapInst;
		stSEnv.pstKiller = get_atker_player(pstEnv, pstAtker);
		
		pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
		if (pstMapIdx)
		{
			snprintf(szName, sizeof(szName), "map%d.mac", pstMapIdx->stMapDef.MapID);
			szName[sizeof(szName) - 1] = 0;
						
			z_exec_script(pstAppCtx, pstEnv, NULL, szName, &pstMapInst->stScriptIdx, MAP_INST_SCRIPT_PROC_MON_DIE);
		}
	}
		
	
	pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if(pstPworldInst && fLinkedAllDie)
	{
		//不管是不是评价副本都记录下
		//打死才计算死亡数量
		if(pstMon->iFirstAtkerID > 0 || (pstAtker != NULL && pstAtker->iType != OBJ_UNKNOW))
		{
			pworld_val_trace_mon_die(pstEnv, pstPworldInst, pstMon);
		}
	}
	
	if (fPworldSrcipt &&
		pstPworldInst && 
		(pstPworldInst->cPworldScriptFlag & PWORLD_SCRIPT_PROC_MON_DIE_MAP) &&
		MON_SUB_TYPE_TMP > pstMonDef->SubType &&
		pstMonDef->SubType != MON_SUB_TYPE_TOTEM && 
		0 == is_mon_machine(pstMon) &&
		pstMon->iOwnerID <= 0 )
	{
		stSEnv.pstMonster = pstMon;
		stSEnv.pstNpc = NULL;
		stSEnv.pstPlayer = pstPlayer;
		stSEnv.pstPworldInst = pstPworldInst;
		stSEnv.pstKiller = get_atker_player(pstEnv, pstAtker);
		stSEnv.pstMapinst = pstMapInst;
		if(!stSEnv.pstPlayer)
		{
			stSEnv.pstPlayer = stSEnv.pstKiller;
		}
		snprintf(szName, sizeof(szName), "pworld%d.mac", pstPworldInst->stPworldIdx.iID);
		szName[sizeof(szName) - 1] = 0;
				
		z_exec_script(pstAppCtx, pstEnv, NULL, szName, &pstPworldInst->stScriptIdx, PWORLD_SCRIPT_PROC_MON_DIE);
	}

	if ((pstMon->cMonScriptFlag & MON_SCRIPT_PROC_DIE_MAP))
	{
		pstMon->bDelayScript |= MON_SCRIPT_PROC_DIE_MAP;
		
		stSEnv.pstMonster = pstMon;
		stSEnv.pstNpc = NULL;
		stSEnv.pstPlayer = pstPlayer;
		stSEnv.pstPworldInst = pstPworldInst;
		stSEnv.pstKiller = get_atker_player(pstEnv, pstAtker);
		stSEnv.pstMapinst = pstMapInst;;
		if(!stSEnv.pstPlayer)
		{
			//为了副本里名字或获取问题
			stSEnv.pstPlayer = stSEnv.pstKiller;
		}
		snprintf(szName, sizeof(szName), "monster%d.mac", pstMonDef->MonsterID);
		szName[sizeof(szName) - 1] = 0;
				
		z_exec_script(pstAppCtx, pstEnv, NULL, szName, &pstMon->stScriptIdx, MON_SCRIPT_PROC_DIE);
	}

	if (pstMon->tRefresh <= pstAppCtx->stCurr.tv_sec)
	{
		if (pstMon->iFormDefID)
		{
			if ( 0 > z_mon_resile(pstEnv, pstMon))
			{
				goto err;
			}
		}
		else
		{	// 尝试变异
			if ( 0 > z_mon_aberrants( pstEnv, pstMon))
			{
				goto err;
			}
		}

		z_refresh_mon(pstAppCtx, pstEnv, pstMon,NULL);
	}

	// OP LOG
	if (pstMon->bSubType >MON_SUB_TYPE_NORM  && pstMon->bSubType < MON_SUB_TYPE_TMP)
	{
		z_monster_die_oplog( pstEnv, pstMonDef, pstMon, pstAtker );
	}


	//lzk   融焰怪死亡
	if(pstMon->iRongYanTeamCapId > 0)
	{
		mon_rongyan_boss_die(pstEnv,pstMon);	
	}

	//终极熔岩巨兽死亡
	iZJret  = zj_rongyan_mon_if_have_by_wid(pstEnv,pstMon->WID);
	if(iZJret > 0)
	{
		mon_zj_rongyan_die(pstEnv,pstMon);	
	}
_final:
	/*
	if(pstMon->bSubType == MON_SUB_TYPE_JIN || pstMon->bSubType == MON_SUB_TYPE_BOSS)
	{
		tlog_info(pstEnv->pstLogCat, 0, 0,	"monster die: DefID %d, MemID %d, Map %d",
			pstMon->iDefIdx, pstMon->iID, pstMon->stMap.iID);
	}
	*/
	return 0;
	
err:
	//还原不了就清除掉。
	if(pstMon->iLinkedNext < 0 && pstMon->iLinkedPrev < 0)
	{
		z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);
	}
	goto _final;
	return 0;
}

int is_valid_booty_team(ZONESVRENV* pstEnv , Player *pstPlayer)
{
	ZONEIDX stIdx;
	ZONEPWORLD *pstPworld;
		
	if(pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
	{	
		return 1;
	}

	stIdx.iID = GET_PWORLDID(pstPlayer->stRoleData.Map);
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (pstPworld  == NULL)
	{
		return 1;
	}

	if (PWORLD_TYPE_SINGLE == pstPworld->stPworldDef.Type)
	{
		return 0;
	}
	else
	{
		if (pstPworld->stPworldDef.CtrlFlag & PWORLD_NO_TEAM)
		{
			return 0;
		}
	}

	//todo:单人队伍都能进的副本是否还要处理下

	return 1;
}

int mon_booty_owner_valid_player(Monster *pstMon, Player *pstPlayer, Player *pstAtker)
{
	if (NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus || 
		pstPlayer->stRoleData.Map != pstMon->stMap.iID || pstPlayer->stOnline.State & CS_STAT_DEAD)
	{
		return -1;
	}

	if ((pstAtker != pstPlayer) && pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE && !z_pos_in_view(&pstMon->stCurrPos, &pstPlayer->stRoleData.Pos))
	{
		return -1;
	}
	//有指定状态不分
	if(z_player_find_status(pstPlayer, ENTER_PWORLD_SAFE_STATUSID))
	{
		return -1;
	}
	
	return 0;
}

int mon_trace_valid_player (Monster *pstMon, Player *pstPlayer)
{
	if (NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus || 
		pstPlayer->stRoleData.Map != pstMon->stMap.iID )
	{
		return -1;
	}

	if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE && !z_pos_in_view(&pstMon->stCurrPos, &pstPlayer->stRoleData.Pos))
	{
		return -1;
	}
	
	return 0;
}

int mon_booty_drop(ZONESVRENV* pstEnv, Monster *pstMon, ROLEGRID *pstRoleGrid, Player *pstPlayer)
{
	ITEMDEF  *pstItemDef;
	ZoneBootyGrid stBootyGrid;
	MONSTERDEF *pstMonDef;
	ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);
	
	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if(NULL == pstMonDef)
	{
		return -1;
	}

	memset(&stBootyGrid, 0, sizeof(stBootyGrid));
	
	stBootyGrid.ullRoleID = pstPlayer->stRoleData.RoleID;
	stBootyGrid.bViewType = pstMonDef->BootyViewType;
	stBootyGrid.bType = pstMonDef->BootyDropType;
	if(pstTeam)
	{
		stBootyGrid.ullTeamID = pstTeam->stTeamInfo.TeamID;
	}
	
	memcpy(&stBootyGrid.stGrid, pstRoleGrid, sizeof(stBootyGrid.stGrid));

	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstRoleGrid->GridData.RoleItem);
	if(pstItemDef && pstItemDef->DropSelfView)
	{
		stBootyGrid.bViewType = BOOTY_DROP_VIEW_SELF;
	}

	if(stBootyGrid.stGrid.GridData.RoleItem.Num > 0)
	{
		dropitem_create(pstEnv, &stBootyGrid, &pstMon->stCurrPos, pstMon->stMap.iID, 0);
	}
	
	return 0;
}

int get_player_rate_mul(Player *pstOwner, int iItemID)
{
	int iRate = 0;
	
	if ( iItemID >= ITEM_NUM_TASK_MIN && iItemID <= ITEM_NUM_TASK_MAX) //任务物品
	{
		iRate += pstOwner->stOnline.stDyn.stHealChg.ucTaskItemRate /*+ pstOwner->stOnline.stFitOneline.unTaskItemRate*/;
	}
	else if ( iItemID >= ITEM_NUM_STUFF_MIN && iItemID <= ITEM_NUM_STUFF_MAX) //材料物品
	{
		iRate += pstOwner->stOnline.stDyn.stHealChg.ucStuffItemRate /*+ pstOwner->stOnline.stFitOneline.unStuffItemRate*/;
	}

	return iRate;
}

int player_view_booty_valid(ZONESVRENV* pstEnv, DropItem *pstDropItem, Player *pstPlayer)
{
	ZoneTeam *pstTeam;

	if(pstDropItem->cType == DROP_ITEM_GRID)
	{
		//所有者可见
		if(pstDropItem->stObj.stBootyGrid.ullRoleID == pstPlayer->stRoleData.RoleID)
		{
			return 1;
		}
		
		//所有者可见
		switch(pstDropItem->stObj.stBootyGrid.bViewType)
		{
			case BOOTY_DROP_VIEW_SELF:
				//所有者
				if(pstDropItem->stObj.stBootyGrid.ullRoleID != pstPlayer->stRoleData.RoleID)
				{
					return 0;
				}
				break;
			case BOOTY_DROP_VIEW_TEAM:
				pstTeam = player_team_get(pstEnv, pstPlayer);
				if(NULL == pstTeam || pstTeam->stTeamInfo.TeamID != pstDropItem->stObj.stBootyGrid.ullTeamID)
				{
					return 0;
				}
				break;
			case BOOTY_DROP_VIEW_ALL:
				return 1;
				break;
			default:
				return 0;
		}
	}
	else
	{
		return 1;
	}

	return 1;
}

int booty_discrete_drop_item(ZONESVRENV* pstEnv, Monster * pstMon, int iID, int iNum)
{
	int iCount = 0;
	int iDec = 0;
	int iDropNum;
	ROLEGRID stRoleGrid;
	ZoneBootyGrid stBootyGrid;
	MONSTERDEF *pstMonDef;

	if(iID <= 0 || iNum <= 0)
	{
		return 0;
	}

	if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, iID))
	{
		return -1;
	}

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if(NULL == pstMonDef)
	{
		return -1;
	}
	
	//iNum >= 3 则保底3堆	
	iDec = iNum > 3 ? 3 : iNum;
	
	while(iNum > 0)
	{	
		iDropNum = RAND1(iNum - iDec) + 1;
		if(iDec > 0)
		{
			iDec --;
		}
	
		iCount++;
		//上限5堆
		if(iCount >= 5)
		{
			iDropNum = iNum;
		}
		
		iNum -= iDropNum;

		if (GRID_ITEM_ITEM == stRoleGrid.Type)
		{
			stRoleGrid.GridData.RoleItem.Num = iDropNum;
		}

		memset(&stBootyGrid, 0, sizeof(stBootyGrid));
		memcpy(&stBootyGrid.stGrid, &stRoleGrid, sizeof(stBootyGrid.stGrid));
		//人人都可以拾取
		stBootyGrid.ullRoleID = 0;
		stBootyGrid.bViewType = BOOTY_DROP_VIEW_ALL;
		stBootyGrid.bType = pstMonDef->BootyDropType;

		dropitem_create(pstEnv, &stBootyGrid, &pstMon->stCurrPos, pstMon->stMap.iID, pstMon->iID);
	}
	
	return 0;
}

/*	-lxk
	功能:战利品 - 掉钱
	参数:
		iIsOwnPlayer:战利品是否有所属角色(0没有；1有)
		iUseMonDefDropType:如果值 等于 USE_MONDEF_BOOTYDROPTYPE ，则使用怪物定义表中填写的掉落类型；
						   否则，根据值iUseMonDefDropType去设置掉落方式
		iFlag:客户端表示标记(0延迟播放;1立即播放)[主要用来表示黑暗物质受击后立即掉钱]				   
*/
int booty_money_drop_ex(ZONESVRENV* pstEnv, Player *pstPlayer, Monster * pstMon, 
								int iMoneyType, int iMoneyVal,int iIsOwnPlayer,
								int iUseMonDefDropType, int iFlag)
{
	int iMoneyItemID;
	ZoneBootyGrid stBootyGrid;
	ZoneTeam *pstTeam = NULL;
	if(iIsOwnPlayer)
	{
		pstTeam = player_team_get(pstEnv, pstPlayer);
	}
	
	switch(iMoneyType)
	{
		case ATTR_ID_MONEY:
			iMoneyItemID = BOOTY_MONEY_ID;
			break;
		case ATTR_ID_BULL:
			iMoneyItemID = BOOTY_BULL_ID;
			break;
		default:
			return -1;
	}

	if(iMoneyVal <=0)
	{
		return -1;
	}

	memset(&stBootyGrid, 0, sizeof(stBootyGrid));

	stBootyGrid.stGrid.GridData.RoleItem.DefID = iMoneyItemID;
	stBootyGrid.stGrid.GridData.RoleItem.Flag = iMoneyVal;
	stBootyGrid.bViewType = BOOTY_DROP_VIEW_ALL;
	stBootyGrid.szShowFlag = iFlag;
	stBootyGrid.iFromID = pstMon->iDefIdx;
	
	if(iIsOwnPlayer)
	{
		stBootyGrid.ullRoleID = pstPlayer->stRoleData.RoleID;
		if(pstTeam)
		{
			stBootyGrid.ullTeamID = pstTeam->stTeamInfo.TeamID;
		}
	}
	else
	{
		stBootyGrid.ullRoleID = 0;
		stBootyGrid.ullTeamID = 0;
	}
	if(iUseMonDefDropType == USE_MONDEF_BOOTYDROPTYPE)
	{
		MONSTERDEF *pstMonDef;
		pstMonDef = z_get_mon_def(pstEnv, pstMon);
		if(pstMonDef)
		{	
			stBootyGrid.bType = pstMonDef->BootyDropType;
		}
	}
	else
	{
		stBootyGrid.bType = iUseMonDefDropType;
	}
	dropitem_create(pstEnv, &stBootyGrid, &pstMon->stCurrPos, pstMon->stMap.iID, pstMon->iID);
	
	return 0;
}

int booty_money_drop(ZONESVRENV* pstEnv, Player *pstPlayer, Monster * pstMon, int iMoneyType, int iMoneyVal)
{
	return booty_money_drop_ex(pstEnv,pstPlayer,pstMon,iMoneyType,iMoneyVal, 1, USE_MONDEF_BOOTYDROPTYPE, 0);
	/*
	int iMoneyItemID;
	ZoneBootyGrid stBootyGrid;
	MONSTERDEF *pstMonDef;
	ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);
	
	switch(iMoneyType)
	{
		case ATTR_ID_MONEY:
			iMoneyItemID = BOOTY_MONEY_ID;
			break;
		case ATTR_ID_BULL:
			iMoneyItemID = BOOTY_BULL_ID;
			break;
		default:
			return -1;
	}

	if(iMoneyVal <=0)
	{
		return -1;
	}

	memset(&stBootyGrid, 0, sizeof(stBootyGrid));

	stBootyGrid.stGrid.GridData.RoleItem.DefID = iMoneyItemID;
	stBootyGrid.stGrid.GridData.RoleItem.Flag = iMoneyVal;
	stBootyGrid.bViewType = BOOTY_DROP_VIEW_ALL;
	stBootyGrid.ullRoleID = pstPlayer->stRoleData.RoleID;
	if(pstTeam)
	{
		stBootyGrid.ullTeamID = pstTeam->stTeamInfo.TeamID;
	}

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if(pstMonDef)
	{	
		stBootyGrid.bType = pstMonDef->BootyDropType;
	}
	
	dropitem_create(pstEnv, &stBootyGrid, &pstMon->stCurrPos, pstMon->stMap.iID, pstMon->iID);
	
	return 0;
	*/
}

// 第几个 宽 长 每行几个 每列几个 打击点 输出点
int erea_(int iIdx,int iK,int iC,int iRn,int iCn,int iHitx,int iHity,int *piOutX,int *piOutY)
{
	// 左下角为第一个
	int x1=iHitx-(iK/2);
	int y1=iHity-(iC/2);
	
	// 放不满的情况
	if (iIdx >= iRn*iCn)return -1;
	if(iRn<=1 ||iCn<=1)return -1;

	*piOutX=(iIdx%iRn)*(iK/(iRn-1))+x1;
	*piOutY=(iIdx/iRn)*(iC/(iCn-1))+y1;
	return 0;
}

// 第几个 |每圈间距 |角度| 多少列 |打击点 |输出| 朝向偏移
int arc_(ZONESVRENV *pstEnv,int i,int iRound, int iAngle,int iCn,RESPOS o,RESPOS *pstCallPos,int iAngelArc)
{
	int iJd;
	RESPOS p;
	int iAngleRotate = 0;

	if(iCn%2 == 0)
	{
		iAngelArc = iAngelArc + ( ((iCn-1)/2.0)*iAngle);
	}
	else
	{
		iAngelArc = iAngelArc + ((iCn/2)*iAngle);
	}
	
	iJd = (i%iCn)*iAngle; 
	p.X = o.X+(i/iCn+1)*iRound;
	p.Y = o.Y;
	iAngleRotate = (iAngelArc - iJd)%360;
	*pstCallPos = z_rotate_pos(o,-iAngleRotate,p);

	//tlog_error(pstEnv->pstLogCat, 0, 0, "arc_ %d pos(%d,%d),pos(%d,%d)", iAngleRotate,o.X,o.Y,p.X,p.Y);
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

// 第几个|每圈距离|第一圈多少个 以后每圈翻倍|打击点|输出
int round_(int i,int iRound, int iRoundNum,int iHitx,int iHity,int *piOutX,int *piOutY)
{
	double iJd;
	int iIdx =1;
	int iCount = iRoundNum;
	if (iRoundNum <=0)return -1;
	while (1)
	{
		if(i<iCount)
			break;
		iIdx++;
		iCount += iRoundNum*iIdx;
	}

	iJd = 360.0/(iIdx*iRoundNum);
	*piOutX = (int) (iHitx - iIdx*iRound * cos(i%(iIdx*iRoundNum)*iJd*PI/180.0f));
	*piOutY = (int) (iHity + iIdx*iRound * sin(i%(iIdx*iRoundNum)*iJd*PI/180.0f));

	return 0;
}

/*
	              	1*step       	2*step       	3*step       	4*step
	                第一圈	 第二圈	 第三圈	 第四圈

	求N所在圈， ( N/step  )  <= 等差数列的前m项和,m既所在圈
*/
void WindingItemAxisDir(int n, int iStep, int *piDir, int *piRound)
{
	int i, s =0;
	int k, r, top;

	if(n <= 0)
	{
		*piRound = 0;
		*piDir = 0;
		return ;
	}

	i = 1;
	while(1)
	{
		s +=i*iStep;
		if(n <= s)
		{
			break;
		}
		i++;
	}

	//k=第几圈
	k = i;
	//一圈有多少个
	r = i*iStep;
	//起始点
	top = s-i*iStep+1;

	*piDir = 360/r*(n-top);
	if (*piDir >= 360)
	{
		*piDir -= 360;
	}
	
	*piRound = k;

	return ;
}


/*
int Get2NearPower(unsigned int n)
{
	int iResult = -1;
	int i, m =0x80000000;

	if(n == 0)
	{
		return -1;
	}
	
	for(i = 0; i< 32; i++)
	{
		m  = n << i;

		if(m & 0x80000000)
		{
			iResult = 31-i;
			break;
		}
	}

	return iResult;
}
*/

/*
	2^0	 2^1 	 2^2  		2^3  		2^4   		2^5
	1       1      	1       		1       		1       		1
	                第一圈	 第二圈	 第三圈	 第四圈


	当前N所在圈=  (N+3), 所包含的最高位所在的圈
	
*/

/*
void WindingItemAxisDir2(int n, int *piDir, int *piRound)
{
	int k, r, top;

	if(n <= 0)
	{
		*piRound = 0;
		*piDir = 0;
		return ;
	}
	
	//k=第几圈
	k = Get2NearPower(n+3);
	k--;

	//一圈有多少个
	r = pow(2,(k+1));
	//起始点
	top = pow(2,(k+1))-3;

	*piDir = 360/r*(n-top);
	if (*piDir >= 360)
	{
		*piDir -= 360;
	}
	
	*piRound = k;

	return ;
}

//同心圆边线分布 , 1 圈 4 个， 2圈 12 个 3圈 20
void WindingItemAxisDir(int n, int *piDir, int *piRound)
{
	int k, r, top;

	if(n <= 0)
	{
		*piRound = 0;
		*piDir = 0;
		return ;
	}

	//k=第几圈
	k = (int) sqrt(n);
	if(k%2 == 0)
	{
		k = k/2;
	}
	else
	{
		k = k/2 + 1;
	}

	//一圈有多少个
	r = ((k*2)-1)*4;   
	//起始点
	top = (k-1)*(k-1)*4 + 1;

	*piDir = 360/r*(n-top);
	if (*piDir >= 360)
	{
		*piDir -= 360;
	}
	
	*piRound = k;

	return ;
}
*/

int dropitem_get_valid_pos(ZONESVRENV* pstEnv, int iMap, RESPOS *pstCurrPos)
{
	int i, iFaceAngle;
	int iRound, iDist;
	int iPosition;
	RESPOS stPos;
	ZONEMAPINST* pstMapInst;
	MapIndex* pstMapIdx;
	ZONEIDX stIdx;
	TMAPMASK *pstMask;

	stIdx.iID = iMap;
	stIdx.iPos = -1;
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

	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);
	
	for(i = 0; i<MAX_RAND_POSITION; i++ )
	{
		iPosition = get_rand_position();
		WindingItemAxisDir(iPosition, 4, &iFaceAngle, &iRound);

		iDist = iRound*100;
		
		stPos.X		=	(int) (pstCurrPos->X + iDist * cos((iFaceAngle)*PI/180.0f));
		stPos.Y		=	(int) (pstCurrPos->Y - iDist * sin((iFaceAngle)*PI/180.0f));
		
		if (IsPixelLineThrough(pstMask, pstCurrPos->X, pstCurrPos->Y, stPos.X, stPos.Y, maskStop) &&
			dyn_msk_check(pstEnv, pstMapInst, pstMask, pstCurrPos->X, pstCurrPos->Y, stPos.X, stPos.Y, 0,DYN_MSK_STOP) &&
			z_valid_pos(pstEnv->pstMapObj, iMap, NULL, &stPos)) 
		{
			*pstCurrPos = stPos;
			break;
		}
	}
	
	return 0;
}

int dropitem_create(ZONESVRENV* pstEnv, ZoneBootyGrid *pstBootyGrid, RESPOS *pstPos, int iMap, int iFromID)
{
	int iIndex;
	TMEMBLOCK *pstMemBlock;
	DropItem *pstDropItem;
	int iRet;
	RESPOS stDropPos;
	ROLEGRID stTmpGrid;
	Monster *pstMon;
	ITEMDEF *pstItemdef;

	iIndex = tmempool_alloc(pstEnv->pstItemPool);
	if (0 > iIndex)
	{
		return -1;
	}

	pstMon = z_id_monster2(pstEnv->pstAppCtx, pstEnv, iFromID);

	stDropPos = *pstPos;
	dropitem_get_valid_pos(pstEnv, iMap, &stDropPos);

	//oplog
	stTmpGrid = pstBootyGrid->stGrid;

	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstItemPool, iIndex);
	pstDropItem = (DropItem *)pstMemBlock->szData;

	memset(pstDropItem,0,sizeof(DropItem));
	pstDropItem->cType = DROP_ITEM_GRID;
	pstDropItem->cStat = DROP_ITEM_NOSTAY;
	pstDropItem->unRefreshTime = 0;
	memcpy(&pstDropItem->stObj.stBootyGrid, pstBootyGrid, sizeof(pstDropItem->stObj.stBootyGrid));
	pstDropItem->iDefIdx = pstBootyGrid->stGrid.GridData.RoleItem.DefID;
	pstDropItem->iID = 0;
	pstDropItem->stMap.iID = iMap;
	pstDropItem->stMap.iPos = -1;
	pstDropItem->stPos = stDropPos;
	pstDropItem->iMIndex = iIndex;
	pstDropItem->iFromID = iFromID;
	pstDropItem->ullTeamID = 0;
	
	if(pstDropItem->stObj.stBootyGrid.bCollectTime > 0)
	{
		pstDropItem->iStayTime = 7200;
	}
	else
	{
		if(iMap >= PWORLD_MAPID_BASE)
		{
			pstDropItem->iStayTime = 1200;
		}
		else
		{
			pstDropItem->iStayTime = dropitem_get_stay_time(pstEnv, pstDropItem);
		}
		
	}

	//掉落的时候有归属的或者需要Roll点的道具有保护时间
	if(pstBootyGrid->bRoll == ITEM_ROLL_WAIT || pstBootyGrid->ullRoleID > 0 || pstBootyGrid->ullTeamID > 0)
	{
		pstDropItem->tPickLimitTime = pstEnv->pstAppCtx->stCurr.tv_sec + dropitem_get_pick_limit_time(pstEnv, pstDropItem);
	}

	if(pstBootyGrid->ullClanGID > 0)
	{
		pstDropItem->tPickLimitTime = pstEnv->pstAppCtx->stCurr.tv_sec+pstDropItem->iStayTime+2;
	}

	if (pstMon && pstMon->cRonglu)
	{
		pstDropItem->tPickLimitTime = pstEnv->pstAppCtx->stCurr.tv_sec+pstDropItem->iStayTime+2;
	}

	pstItemdef = z_find_itemdef(pstEnv, pstDropItem->iDefIdx);
	if (pstItemdef && pstItemdef->CtrlFlag&ITEM_DROP_TIME)
	{
		if (pstDropItem->tPickLimitTime > 0)
		{
			pstDropItem->tPickLimitTime = pstEnv->pstAppCtx->stCurr.tv_sec+pstDropItem->iStayTime+2;
		}
	}

	if(pstMon && OBJ_NPC == pstMon->cType)
	{
		pstDropItem->iFromID = 0;
		pstDropItem->tPickLimitTime = pstEnv->pstAppCtx->stCurr.tv_sec+pstDropItem->iStayTime+2;	
	}

	if (0 > (iRet = z_item_refresh(pstEnv, pstDropItem,0)))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "item refresh fail ret %d, item %d %d, map %d, pos (%d,%d)", iRet, pstDropItem->iID,
					pstDropItem->iDefIdx, pstDropItem->stMap.iID, pstDropItem->stPos.X, pstDropItem->stPos.Y);
	}

	//掉落日志
	z_role_booty_drop_oplog(pstEnv->pstAppCtx,pstEnv,
					iMap, &stDropPos,&stTmpGrid, ITEM_DROP_TYPE_MAP, iFromID);

	return 0;
}

int selled_del(ZONESVRENV* pstEnv,ITEMSELLSAVE *pstItemSellSave)
{
	int i;
	ITEMSELL *pstItemSell;
	
	for (i=pstItemSellSave->ItemSellNum-1; i>=0; i--)
	{
		pstItemSell = &pstItemSellSave->ItemSells[i];
		if (pstItemSell->SellTime+SELL_ITEM_SAVE_TIME < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			pstItemSellSave->ItemSellNum--;

			if (i != pstItemSellSave->ItemSellNum)
			{
				memmove(pstItemSell, pstItemSell+1, (pstItemSellSave->ItemSellNum-i)*sizeof(*pstItemSell));
			}
		}
	}

	return 0;
}

int shop_selled_save(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEGRID *pstRoleGrid, int iMoneyType, unsigned int uiPrice)
{
	ITEMSELL *pstItemSell;
	CSPKG stPkg;
	CSSHOPSVR *pstShopSvr = &stPkg.Body.ShopSvr;
	ITEMSELLSAVE *pstItemSellSave = &pstPlayer->stRoleData.MiscInfo.ItemSellSave;

	selled_del(pstEnv,pstItemSellSave);

	if (pstItemSellSave->ItemSellNum >= MAX_SELL_SAVE)
	{
		memmove(&pstItemSellSave->ItemSells[0], &pstItemSellSave->ItemSells[1], 
					(pstItemSellSave->ItemSellNum-1)*sizeof(pstItemSellSave->ItemSells[0]));
		pstItemSell = &pstItemSellSave->ItemSells[pstItemSellSave->ItemSellNum-1];
		memcpy(&pstItemSell->RoleGrid, pstRoleGrid, sizeof(pstItemSell->RoleGrid));
		pstItemSell->SellPrice = uiPrice;
		pstItemSell->MoneyType = iMoneyType;
		pstItemSell->SellTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	else
	{
		pstItemSell = &pstItemSellSave->ItemSells[pstItemSellSave->ItemSellNum];
		memcpy(&pstItemSell->RoleGrid, pstRoleGrid, sizeof(pstItemSell->RoleGrid));
		pstItemSell->SellPrice = uiPrice;
		pstItemSell->SellTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstItemSell->MoneyType = iMoneyType;
		pstItemSellSave->ItemSellNum++;
	}

	pstShopSvr->OpType = SHOP_SELLED_LIST;
	memcpy(&pstShopSvr->ShopSvrData.ShopSelledList, pstItemSellSave, sizeof(pstShopSvr->ShopSvrData.ShopSelledList));

	Z_CSHEAD_INIT(&stPkg.Head, SHOP_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int shop_selled_save_del(ZONESVRENV* pstEnv, Player *pstPlayer, int iIdx)
{
	int i;
	ITEMSELL *pstItemSell;
	CSPKG stPkg;
	CSSHOPSVR *pstShopSvr = &stPkg.Body.ShopSvr;
	ITEMSELLSAVE *pstItemSellSave = &pstPlayer->stRoleData.MiscInfo.ItemSellSave;

	for (i=pstItemSellSave->ItemSellNum-1; i>=0; i--)
	{
		pstItemSell = &pstItemSellSave->ItemSells[i];
		if (i == iIdx || pstItemSell->SellTime+SELL_ITEM_SAVE_TIME < pstEnv->pstAppCtx->stCurr.tv_sec )
		{
			pstItemSellSave->ItemSellNum--;

			if (i != pstItemSellSave->ItemSellNum)
			{
				memmove(pstItemSell, pstItemSell+1, (pstItemSellSave->ItemSellNum-i)*sizeof(*pstItemSell));
			}
		}
	}

	pstShopSvr->OpType = SHOP_SELLED_LIST;
	memcpy(&pstShopSvr->ShopSvrData.ShopSelledList, pstItemSellSave, sizeof(pstShopSvr->ShopSvrData.ShopSelledList));

	Z_CSHEAD_INIT(&stPkg.Head, SHOP_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int shop_buy(ZONESVRENV* pstEnv, Player *pstPlayer, Npc *pstNpc, CSSHOPBUY *pstBuy)
{
	NPCSELLLIST *pstSellList;
	SELLGOODS *pstSellGoods;
	ITEMDEF *pstItemDef;
	ROLEGRID stRoleGrid;
	OPSHOPDATA OldShopData;
	ZoneClan *pstClan = NULL;
	unsigned int uiClanContri = 0;
	ITEMDEC stItemDec;
	unsigned int uiReBate;
	unsigned int uiUseMoney = 0;
	int i;
	long long llUseTmp;
	unsigned int uiUseTmp;
	int iRet;
	
	if (player_check_safelock(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}

	memset(&stItemDec, 0, sizeof(stItemDec));

	if (pstNpc->nSellListID < 10000)
	{
		pstSellList = z_get_npc_selllist( pstEnv, pstNpc);
		if (NULL == pstSellList || pstBuy->Idx >= pstSellList->Count)
		{
			return -1;
		}

		pstSellGoods = &pstSellList->Goods[pstBuy->Idx];
	}
	else
	{
		for (i=pstNpc->nSellListID; i<pstNpc->nSellListID+10; i++)
		{
			pstSellList = z_get_npc_selllist_ex( pstEnv,i);
			if (!pstSellList )
			{
				continue;
			}

			if (pstBuy->Idx >= pstSellList->Count)
			{
				continue;
			}

			pstSellGoods = &pstSellList->Goods[pstBuy->Idx];
			if (	pstSellGoods->GoodsID == pstBuy->GoodsID &&
				pstSellGoods->GoodsType == pstBuy->GoodsType)
			{
				break;
			}
		}

		if (i >= pstNpc->nSellListID+10)
		{
			return -1;
		}
	}
	
	if (pstSellGoods->GoodsID != pstBuy->GoodsID || pstSellGoods->GoodsType != pstBuy->GoodsType)
	{
		return -1;
	}

	switch (pstSellGoods->Condition.Type)
	{
	case SHOP_CONDITION_NONE:
		break;
		
	case SHOP_CONDITION_CLAN_TECH:
		{
			pstClan = player_get_clan(pstEnv, pstPlayer);
			if (!pstClan)
			{
				return -1;
			}
			
			if (pstSellGoods->Condition.Value1 >= 0 && pstSellGoods->Condition.Value1 < MAX_CLAN_TECH_CATEGORY)
			{
				if (pstClan->stClan.ClanAdvance.Tech.TechLevel[pstSellGoods->Condition.Value1] < pstSellGoods->Condition.Value2)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NPC101);
					return -1;
				}
			}
		}
		break;
		
	case SHOP_CONDITION_WORLD_CITY_KEEP_NUM:
		if (!is_world_city_owner(pstEnv, pstPlayer))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NPC102);
			return -1;
		}
		else
		{
			if (world_city_fight_keep_num(pstEnv) < pstSellGoods->Condition.Value1)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NPC103, pstSellGoods->Condition.Value1);
				return -1;
			}
		}
		break;
	
		
	default:
		return -1;
		break;
	}

	player_cacl_rebate(pstEnv, pstPlayer, &uiReBate, NULL, NULL);
	
	switch(pstSellGoods->GoodsType)
	{
	case SHOP_ITEM_ITEM:
		break;

	/*
	case SHOP_ITEM_CLAN_ITEM:
		return player_clan_shop_buy(pstEnv, pstPlayer, pstNpc, pstSellGoods,
		                            pstBuy->Num);
		break;
		*/

	default:
		return -1;
		break;
	}

	pstItemDef = z_find_itemdef(pstEnv, pstBuy->GoodsID);
	if (NULL == pstItemDef || 0 >= pstBuy->Num /*|| pstBuy->Num > pstItemDef->MaxPile8*/)
	{
		return -1;
	}

	/*if (!(pstItemDef->BuyLimitFlag&ITEM_BUY_LIMIT_BLACK_NAME) &&
		get_pkval_type(pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal) == PK_VAL_TYPE_BLACK)
	{
		z_sys_strf(pstEnv, pstPlayer,SYS_MIDDLE2, 
			"由于您是黑名玩家,物品%s 商人不愿出售",pstItemDef->Name);
		return -1;
	}*/

	// 检查声望需求
	if(pstItemDef->NeedRespact.Value > 0 && player_respact_value_check(pstEnv, pstPlayer, &pstItemDef->NeedRespact)
	   < 0)
	{
		const RESPACTPARTYDEF *pstPartyDef =
			z_respact_party_def(pstEnv, pstItemDef->NeedRespact.ID);
		const RESPACTPHASEDEF *pstPhaseDef =
			z_respact_phase_refer_value(pstEnv, pstItemDef->NeedRespact.Value);

		if(pstPartyDef && pstPhaseDef)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NPC104, pstPartyDef->PartyName, pstPhaseDef->PhaseText);
		}
		
		return -1;
	}

	if(pstItemDef->NeedSpartaFight > 0 && 
		pstItemDef->NeedSpartaFight > pstPlayer->stRoleData.MiscInfo.SpartaMiscInfo.FightVal)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NPC_301);
		return -1;
	}

	if (0 != pstSellGoods->SpecialMoneyType)
	{
		if (ATTR_ID_BULL == pstSellGoods->SpecialMoneyType || ATTR_ID_MONEY == pstSellGoods->SpecialMoneyType)
		{
			uiUseMoney = 1.0*uiReBate/100 * pstSellGoods->SpecialMoneyNum;
		}
		else
		{
			uiUseMoney = pstSellGoods->SpecialMoneyNum;
		}

		if (0 > package_dec_money_test(pstEnv, pstPlayer, pstSellGoods->SpecialMoneyType, uiUseMoney*pstBuy->Num))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
				ZONEERR_NPC105, pstItemDef->BuyPrice*pstBuy->Num, z_get_money_name(pstSellGoods->SpecialMoneyType));
			return -1;
		}
	}
	else
	{
		if (pstItemDef->ClanContri <= 0 && pstItemDef->ExchgItemID <= 0 && 
			(ATTR_ID_BULL == pstItemDef->MoneyType || ATTR_ID_MONEY == pstItemDef->MoneyType))
		{
			uiUseMoney = 1.0*uiReBate/100 * pstItemDef->BuyPrice; 
		}
		else
		{
			uiUseMoney = pstItemDef->BuyPrice; 
		}

		if (pstItemDef->ClanContri <= 0)
		{
			if (pstItemDef->ExchgItemID <= 0)
			{
				//检查溢出
				llUseTmp = uiUseMoney;
				llUseTmp = llUseTmp*pstBuy->Num;
				uiUseTmp = uiUseMoney*pstBuy->Num;
				
				if (llUseTmp != uiUseTmp)
				{
					return -1;
				}
				
				if (0 > package_dec_money_test(pstEnv, pstPlayer, pstItemDef->MoneyType, uiUseMoney*pstBuy->Num))
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
						ZONEERR_NPC105, pstItemDef->BuyPrice*pstBuy->Num, z_get_money_name(pstItemDef->MoneyType));
					return -1;
				}
			}
			else
			{
				//检查溢出
				llUseTmp = pstItemDef->BuyPrice;
				llUseTmp = llUseTmp*pstBuy->Num;
				uiUseTmp = pstItemDef->BuyPrice*pstBuy->Num;
				
				if (llUseTmp != uiUseTmp)
				{
					return -1;
				}

				if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
						pstItemDef->ExchgItemID,pstItemDef->BuyPrice*pstBuy->Num, -1, NULL,PACKAGE_DEC_NONE))
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NPC106);
					return -1;
				}				
			}
		}
		else
		{
			if (player_check_clan(pstEnv, pstPlayer) < 0)
			{
				return -1;
			}

			//检查溢出
			llUseTmp = pstItemDef->ClanContri;
			llUseTmp = llUseTmp*pstBuy->Num;
			uiUseTmp = pstItemDef->ClanContri*pstBuy->Num;
			
			if (llUseTmp != uiUseTmp)
			{
				return -1;
			}

			uiClanContri = pstItemDef->ClanContri*pstBuy->Num;
			if (0 > player_contribute_dec(pstEnv,pstPlayer,uiClanContri,1))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NPC114);
				return -1;
			}
		}

		// 检查额外消耗，战场那些东西
		for  (i=0; i<MAX_OTHER_MONEY_USE; i++)
		{
			if (	pstItemDef->MoneyUses[i].ID == 0 ||
				pstItemDef->MoneyUses[i].Val == 0)
			{
				break;
			}

			//检查溢出
			llUseTmp = pstItemDef->MoneyUses[i].Val;
			llUseTmp = llUseTmp*pstBuy->Num;
			uiUseTmp = pstItemDef->MoneyUses[i].Val*pstBuy->Num;
			
			if (llUseTmp != uiUseTmp)
			{
				return -1;
			}

			if (package_dec_money_test(pstEnv, pstPlayer, pstItemDef->MoneyUses[i].ID, pstItemDef->MoneyUses[i].Val*pstBuy->Num) < 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NPC107,
									z_get_money_name(pstItemDef->MoneyUses[i].ID));
				return -1;
			}
		}
	}

	
	if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstBuy->GoodsID))
	{
		return -1;
	}

	if (GRID_ITEM_ITEM == stRoleGrid.Type)
	{
		stRoleGrid.GridData.RoleItem.Num = pstBuy->Num;
	}

	if(pstSellList->Bind)
	{
		stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
	}

	if (0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
		return -1;
	}
	
	OldShopData.GoodsNum = z_get_pak_item_num(pstEnv, &pstPlayer->stRoleData.Package, pstItemDef->ItemID, -1,0,PACKAGE_DEC_NONE);
	OldShopData.Money = pstPlayer->stRoleData.RoleDetail.Money;
	OldShopData.Bull = pstPlayer->stRoleData.RoleDetail.Bull;

	if (0 != pstSellGoods->SpecialMoneyType)
	{
		iRet = package_dec_money(pstEnv, pstPlayer, pstSellGoods->SpecialMoneyType, uiUseMoney*pstBuy->Num);
		if (pstSellGoods->SpecialMoneyType == ATTR_ID_BULL)
		{
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_NPCSELL, pstItemDef->ItemID, uiUseMoney*pstBuy->Num-iRet, iRet, "npc买东西");
		}
		else if (pstSellGoods->SpecialMoneyType == ATTR_ID_MONEY)
		{
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_NPCSELL, pstItemDef->ItemID, 0, uiUseMoney*pstBuy->Num, "npc买东西");
		}
	}
	else
	{
		if (pstItemDef->ClanContri <= 0)
		{
			if (pstItemDef->ExchgItemID <= 0)
			{
				iRet = package_dec_money(pstEnv, pstPlayer, pstItemDef->MoneyType, uiUseMoney*pstBuy->Num);
				if (pstItemDef->MoneyType == ATTR_ID_BULL)
				{
					z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_NPCSELL, pstItemDef->ItemID, uiUseMoney*pstBuy->Num-iRet, iRet, "npc买东西");
				}
				else if (pstItemDef->MoneyType == ATTR_ID_MONEY)
				{
					z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_NPCSELL, pstItemDef->ItemID, 0, uiUseMoney*pstBuy->Num, "npc买东西");
				}
			}
			else
			{
				int iBind ;
				
				iBind = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDef->ExchgItemID,
							pstItemDef->BuyPrice*pstBuy->Num, -1, NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_SHOP);

				itemdec_fill(&stItemDec, pstItemDef->ExchgItemID, pstItemDef->BuyPrice, iBind);
			}
		}
		else
		{
			
			ZoneClan* pstClan = player_get_clan(pstEnv, pstPlayer);
			uiClanContri = pstItemDef->ClanContri*pstBuy->Num;
			
			player_contribute_dec(pstEnv,pstPlayer,uiClanContri,0);
			if (pstClan)
			{
				z_contri_consume_oplog(pstEnv, pstClan, OP_OPTYPE_SHOP, pstItemDef->ItemID, uiClanContri);
			}
			
			
		}

		for  (i=0; i<MAX_OTHER_MONEY_USE; i++)
		{
			if (pstItemDef->MoneyUses[i].ID == 0 ||
				pstItemDef->MoneyUses[i].Val == 0)
			{
				break;
			}

			if ((iRet = package_dec_money(pstEnv, pstPlayer, pstItemDef->MoneyUses[i].ID, pstItemDef->MoneyUses[i].Val*pstBuy->Num)) < 0)
			{
				return -1;
			}

			if (pstItemDef->MoneyUses[i].ID == ATTR_ID_BULL)
			{
				z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_NPCSELL, pstItemDef->ItemID, pstItemDef->MoneyUses[i].Val*pstBuy->Num-iRet, iRet, "npc买东西");
			}
			else if (pstItemDef->MoneyUses[i].ID == ATTR_ID_MONEY)
			{
				z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_NPCSELL, pstItemDef->ItemID, 0, pstItemDef->MoneyUses[i].Val*pstBuy->Num, "npc买东西");
			}
			else
			{
				z_battle_money_oplog(pstEnv, pstPlayer, pstItemDef->MoneyUses[i].ID, 
						pstItemDef->MoneyUses[i].Val*pstBuy->Num, "npc买东西");
			}
		}

	}

	if (package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0, IMPITEM_OPTYPE_SHOP) < 0)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "shop_buy package_add %d player name= %s",
					pstPlayer->stRoleData.Uin, pstPlayer->stRoleData.RoleName);	
		return -1;
	}

	task_npcbuy_trace(pstEnv, pstPlayer, pstItemDef);
	
	z_roleshop_oplog(pstEnv,pstPlayer ,pstItemDef , &OldShopData , pstBuy->Num ,
					stRoleGrid.GridData.RoleItem.WID , OP_SHOP_BUY ,&stItemDec, (int)uiClanContri);
	return 0;
}

int shop_buy_selled(ZONESVRENV* pstEnv, Player *pstPlayer, CSSHOPBUYSELLED *pstBuySelled)
{
	int iNum;
	OPSHOPDATA OldShopData;
	ITEMDEF *pstItemDef;
	ITEMSELL *pstItemSell;
	int iRet;
	ITEMSELLSAVE *pstItemSellSave = &pstPlayer->stRoleData.MiscInfo.ItemSellSave;

	if (pstBuySelled->Idx >= pstItemSellSave->ItemSellNum)
	{
		return -1;
	}

	pstItemSell = &pstItemSellSave->ItemSells[pstBuySelled->Idx];
	if (pstBuySelled->WID != pstItemSell->RoleGrid.GridData.RoleItem.WID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NPC108);
		return -1;
	}

	if (pstItemSell->SellTime+SELL_ITEM_SAVE_TIME < pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NPC108);
		shop_selled_save_del(pstEnv, pstPlayer, pstBuySelled->Idx);
		return -1;
	}

	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstItemSell->RoleGrid.GridData.RoleItem);
	if (NULL == pstItemDef )
	{
		return -1;
	}
	

	if (0 > package_dec_money_test(pstEnv, pstPlayer, pstItemSell->MoneyType, pstItemSell->SellPrice))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
						ZONEERR_NPC105, pstItemSell->SellPrice, z_get_money_name(pstItemSell->MoneyType));
		return -1;
	}

	if (0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &pstItemSell->RoleGrid, 1,0))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
		return -1;
	}
	
	
	OldShopData.GoodsNum = z_get_pak_item_num(pstEnv, &pstPlayer->stRoleData.Package, pstItemDef->ItemID, -1, 0,PACKAGE_DEC_NONE);
	OldShopData.Money = pstPlayer->stRoleData.RoleDetail.Money;
	OldShopData.Bull = pstPlayer->stRoleData.RoleDetail.Bull;
	iNum = pstItemSell->RoleGrid.GridData.RoleItem.Num;

	iRet = package_dec_money(pstEnv, pstPlayer, pstItemSell->MoneyType, pstItemSell->SellPrice);
	if (pstItemSell->MoneyType == ATTR_ID_BULL)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_BUYSELLED, pstItemDef->ItemID, pstItemSell->SellPrice-iRet, iRet, "回购卖给npc的道具");
	}
	else if (pstItemSell->MoneyType == ATTR_ID_MONEY)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_BUYSELLED, pstItemDef->ItemID, 0, pstItemSell->SellPrice, "回购卖给npc的道具");
	}
		
	package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &pstItemSell->RoleGrid, 1,0, 0);

	z_roleshop_oplog(pstEnv,pstPlayer ,pstItemDef , &OldShopData , iNum, 
		pstItemSell->RoleGrid.GridData.RoleItem.WID, OP_SHOP_BUYSELL ,NULL,0);
	
	shop_selled_save_del(pstEnv, pstPlayer, pstBuySelled->Idx);
	
	return 0;
}	

int shop_sell(ZONESVRENV* pstEnv, Player *pstPlayer, CSSHOPSELL *pstShopSell)
{
	int iPos;
	int iSrcGridNum = 0;
	int iMoneyType = 0;
	int iPrice = 0;
	OPSHOPDATA OldShopData;
	ROLEGRID stSelledSave;
	ROLEITEM *pstSrcGridItem ;
	ROLEGRID *pstSrcGrid ;
 	ITEMDEF *pstItemDef = NULL;
	ROLEPACKAGECHG *pstRolePakChg;
	CSROLEITEM *pstCSRoleItem = NULL;
	CSROLEARM  *pstCSRoleArm = NULL;
	CSPKG *pstPkgRes = &pstEnv->stCSRes;
	CSPACKAGEINFO *pstPackageInfo = &pstPkgRes->Body.PackageInfo;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;

	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstRolePakChg = &pstPackageInfo->PackageInfoData.PackageChg;
	pstRolePakChg->ArmChg.ChgNum = 0;
	pstRolePakChg->ItemChg.ChgNum = 0;

	if (player_check_safelock(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}

	
	OldShopData.Money = pstPlayer->stRoleData.RoleDetail.Money;
	OldShopData.Bull = pstPlayer->stRoleData.RoleDetail.Bull;
		
	switch(pstShopSell->SrcListType)
	{
		case LIST_TYPE_STUFF:
			if (pstShopSell->SrcGridIdx >= pstPak->StuffPackage.MaxNum)
			{
				return -1;
			}
			
			iPos = package_itemgrid_pos(pstPak->StuffPackage.RoleItems, pstPak->StuffPackage.Num, pstShopSell->SrcGridIdx);
			if (0 > iPos)
			{
				return -1;
			}

			pstSrcGridItem = pstPak->StuffPackage.RoleItems+iPos;
			iSrcGridNum = pstSrcGridItem->Num;
			pstItemDef = z_get_itemdef_roleitem(pstEnv, pstSrcGridItem);
			
			if (NULL == pstItemDef  || pstShopSell->SrcGridNum <= 0 || pstShopSell->SrcGridNum > iSrcGridNum)
			{
				return -1;
			}

			OldShopData.GoodsNum = 
					z_get_pak_item_num(pstEnv, &pstPlayer->stRoleData.Package, pstItemDef->ItemID, -1,0,PACKAGE_DEC_NONE);

			if (pstItemDef->NoNpcSell)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NPC109);
				return -1;
			}

			iMoneyType = pstItemDef->MoneyType;
			iPrice = pstItemDef->SellPrice*pstShopSell->SrcGridNum;

			if (0 > package_add_money_test(pstEnv, pstPlayer, iMoneyType, iPrice))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NPC110);
				return -1;
			}
			
			stSelledSave.Type = GRID_ITEM_ITEM;
			memcpy(&stSelledSave.GridData.RoleItem, pstSrcGridItem, sizeof(stSelledSave.GridData.RoleItem));
			stSelledSave.GridData.RoleItem.Num = pstShopSell->SrcGridNum;

			pstSrcGridItem->Num -= pstShopSell->SrcGridNum;
			pstCSRoleItem = pstRolePakChg->ItemChg.Items + pstRolePakChg->ItemChg.ChgNum;
			pstCSRoleItem->ListType = pstShopSell->SrcListType;
			memcpy(&pstCSRoleItem->Item, pstSrcGridItem, sizeof(pstCSRoleItem->Item));
			pstRolePakChg->ItemChg.ChgNum++;

			if (0 == pstSrcGridItem->Num)
			{
				pstPak->StuffPackage.Num--;
				if (pstSrcGridItem != (pstPak->StuffPackage.RoleItems+pstPak->StuffPackage.Num))
				{
					memcpy(pstSrcGridItem, pstPak->StuffPackage.RoleItems+pstPak->StuffPackage.Num, sizeof(*pstSrcGridItem));
				}
			}
			
			break;
		case LIST_TYPE_TASK:
			if (pstShopSell->SrcGridIdx >= pstPak->TaskPackage.MaxNum)
			{
				return -1;
			}
			
			iPos = package_itemgrid_pos(pstPak->TaskPackage.RoleItems, pstPak->TaskPackage.Num, pstShopSell->SrcGridIdx);
			if (0 > iPos)
			{
				return -1;
			}

			pstSrcGridItem = pstPak->TaskPackage.RoleItems+iPos;
			iSrcGridNum = pstSrcGridItem->Num;
			pstItemDef = z_get_itemdef_roleitem(pstEnv, pstSrcGridItem);
			if (NULL == pstItemDef ||  pstShopSell->SrcGridNum <= 0 || pstShopSell->SrcGridNum > iSrcGridNum)
			{
				return -1;
			}

			OldShopData.GoodsNum = 
				z_get_pak_item_num(pstEnv, &pstPlayer->stRoleData.Package, pstItemDef->ItemID, -1,0,PACKAGE_DEC_NONE);

			if (pstItemDef->NoNpcSell)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NPC109);
				return -1;
			}

			iMoneyType = pstItemDef->MoneyType;
			iPrice = pstItemDef->SellPrice*pstShopSell->SrcGridNum;
			if (0 > package_add_money_test(pstEnv, pstPlayer, iMoneyType, iPrice))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NPC110);
				return -1;
			}
			
			stSelledSave.Type = GRID_ITEM_ITEM;
			memcpy(&stSelledSave.GridData.RoleItem, pstSrcGridItem, sizeof(stSelledSave.GridData.RoleItem));
			stSelledSave.GridData.RoleItem.Num = pstShopSell->SrcGridNum;

			pstSrcGridItem->Num -= pstShopSell->SrcGridNum;
			pstCSRoleItem = pstRolePakChg->ItemChg.Items + pstRolePakChg->ItemChg.ChgNum;
			pstCSRoleItem->ListType = pstShopSell->SrcListType;
			memcpy(&pstCSRoleItem->Item, pstSrcGridItem, sizeof(pstCSRoleItem->Item));
			pstRolePakChg->ItemChg.ChgNum++;

			if (0 == pstSrcGridItem->Num)
			{
				pstPak->TaskPackage.Num--;
				if (pstSrcGridItem != (pstPak->TaskPackage.RoleItems+pstPak->TaskPackage.Num))
				{
					memcpy(pstSrcGridItem, pstPak->TaskPackage.RoleItems+pstPak->TaskPackage.Num, sizeof(*pstSrcGridItem));
				}
			}
			
			break;
		case LIST_TYPE_SUNDRIES:
			if (pstShopSell->SrcGridIdx >= pstPak->SundriesPackage.MaxNum)
			{
				return -1;
			}
			iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, pstShopSell->SrcGridIdx);
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
			
			pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstSrcGrid->GridData.RoleItem);
			
			if (NULL == pstItemDef || pstShopSell->SrcGridNum <= 0 || pstShopSell->SrcGridNum > iSrcGridNum)
			{
				return -1;
			}

			OldShopData.GoodsNum = 
					z_get_pak_item_num(pstEnv, &pstPlayer->stRoleData.Package, pstItemDef->ItemID, -1,0,PACKAGE_DEC_NONE);

			if (pstItemDef->NoNpcSell)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NPC109);
				return -1;
			}

			memcpy(&stSelledSave, pstSrcGrid, sizeof(stSelledSave));
			if (GRID_ITEM_ITEM == stSelledSave.Type)
			{
				stSelledSave.GridData.RoleItem.Num = pstShopSell->SrcGridNum;
			}

			if (GRID_ITEM_ARM ==  pstSrcGrid->Type)
			{
				iMoneyType = pstItemDef->MoneyType;
				iPrice = z_arm_sell_price(pstEnv, &pstSrcGrid->GridData.RoleArm, pstItemDef);
				if (0 > package_add_money_test(pstEnv, pstPlayer, iMoneyType, iPrice))
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NPC110);
					return -1;
				}

				pstCSRoleArm = pstRolePakChg->ArmChg.Arms + pstRolePakChg->ArmChg.ChgNum;
				pstCSRoleArm->ListType = pstShopSell->SrcListType;
				memcpy(&pstCSRoleArm->Arm, &pstSrcGrid->GridData.RoleArm, sizeof(pstCSRoleArm->Arm));
				pstCSRoleArm->Arm.Num = 0;
				pstRolePakChg->ArmChg.ChgNum++;

				pstPak->SundriesPackage.Num--;
				if (pstSrcGrid != (pstPak->SundriesPackage.RoleGrids+pstPak->SundriesPackage.Num))
				{
					memcpy(pstSrcGrid, pstPak->SundriesPackage.RoleGrids+pstPak->SundriesPackage.Num, sizeof(*pstSrcGrid));
				}
			}
			else
			{
				iMoneyType = pstItemDef->MoneyType;
				iPrice = pstItemDef->SellPrice*pstShopSell->SrcGridNum;

				if (0 > package_add_money_test(pstEnv, pstPlayer, iMoneyType, iPrice))
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NPC110);
					return -1;
				}
				
				pstSrcGridItem = &pstSrcGrid->GridData.RoleItem;
				pstSrcGridItem->Num -= pstShopSell->SrcGridNum;

				pstCSRoleItem = pstRolePakChg->ItemChg.Items + pstRolePakChg->ItemChg.ChgNum;
				pstCSRoleItem->ListType = pstShopSell->SrcListType;
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
		default:
			return -1;
			break;
	}
	
	Z_CSHEAD_INIT(&pstPkgRes->Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkgRes);

	package_add_money(pstEnv, pstPlayer, iMoneyType, iPrice);

	if (iMoneyType == ATTR_ID_BULL)
	{
		z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_NPCSELL, 0,  iPrice, 0, "卖npc商店");
	}
	else if (iMoneyType == ATTR_ID_MONEY)
	{
		z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_NPCSELL, 0,  0, iPrice, "卖npc商店");
	}
		
	shop_selled_save(pstEnv, pstPlayer, &stSelledSave, iMoneyType, iPrice);

	z_roleshop_oplog(pstEnv, pstPlayer ,pstItemDef , &OldShopData , stSelledSave.GridData.RoleItem.Num 
		, stSelledSave.GridData.RoleItem.WID, OP_SHOP_SELL, NULL, 0);

	if ((ITEM_IMP_LOGTRACE & pstItemDef->CtrlFlag))
	{
		int iBindNum;

		if(stSelledSave.GridData.RoleItem.InstFlag & INST_ITEM_BIND)
		{
			iBindNum = pstShopSell->SrcGridNum;
		}
		else
		{
			iBindNum = 0;
		}
		
		z_impitem_consume_oplog(pstEnv, pstPlayer,pstItemDef->ItemID, IMPITEM_OPTYPE_SHOP, 
									pstShopSell->SrcGridNum, iBindNum);
	}
	
	return 0;
}

int shop_repair_one(ZONESVRENV* pstEnv, Player *pstPlayer, CSSHOPREPAIRONE *pstRepairOne)
{
	int iPos;
	int iPrice = 0;
	int iOldDur;
	OPSHOPDATA OldShopData;
	ROLEGRID *pstSrcGrid;
	ITEMDEF *pstItemDef = NULL;
	ROLEARM *pstRoleArm = NULL;
	ROLEPACKAGECHG *pstRolePakChg;
	CSROLEARM  *pstCSRoleArm;
	CSPKG *pstPkgRes = &pstEnv->stCSRes;
	CSPACKAGEINFO *pstPackageInfo = &pstPkgRes->Body.PackageInfo;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	int iRet;

	task_player_repair_trace(pstEnv, pstPlayer);

	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstRolePakChg = &pstPackageInfo->PackageInfoData.PackageChg;
	pstRolePakChg->ArmChg.ChgNum = 0;
	pstRolePakChg->ItemChg.ChgNum = 0;
	
	switch(pstRepairOne->SrcListType)
	{
		case LIST_TYPE_SUNDRIES:
			if (pstRepairOne->SrcGridIdx >= pstPak->SundriesPackage.MaxNum)
			{
				return -1;
			}
			iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, pstPak->SundriesPackage.Num, pstRepairOne->SrcGridIdx);
			if (0 > iPos)
			{
				return -1;
			}

			pstSrcGrid = pstPak->SundriesPackage.RoleGrids+iPos;
			if (GRID_ITEM_ARM != pstSrcGrid->Type)
			{
				return -1;
			}

			pstRoleArm = &pstSrcGrid->GridData.RoleArm;
						
			pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstSrcGrid->GridData.RoleItem);
			if (NULL == pstItemDef || (int)pstItemDef->ItemID != pstRepairOne->ItemID)
			{
				return -1;
			}

			iPrice = z_arm_repair_price(pstEnv, pstRoleArm, pstItemDef,pstPlayer);
				
			break;

		case LIST_TYPE_WEAR:
			pstRoleArm = z_wear_get(pstRoleWear, pstRepairOne->ItemID, pstRepairOne->SrcGridIdx);
			if (NULL == pstRoleArm)
			{
				return -1;
			}

			pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleArm);
			if (NULL == pstItemDef)
			{
				return -1;
			}

			iPrice = z_arm_repair_price(pstEnv, pstRoleArm, pstItemDef,pstPlayer);
			break;
		default:
			return -1;
			break;
	}

	if( !pstRoleArm || !pstItemDef)
	{
		return -1;
	}
	
	if(z_item_get_result(pstItemDef, RESULT_NO_REPAIR))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NPC115);
		return -1;
	}
	
	if (0 == pstItemDef->ArmDur.MaxDur || 
		  (int)pstRoleArm->Dur/DUR_UNIT >= z_get_arm_max_dur(pstEnv, pstItemDef))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NPC111);
		return -1;
	}

	if (0 >= iPrice)
	{
		iPrice = 1;		
	}
	//宠物天赋,修理
	pet_native_skill_result(pstEnv,pstPlayer,RESULT_NATIVE_REPAIR_COST_MAP, &iPrice,NULL);

	if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, iPrice))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NPC105, iPrice, MONEY_TYPE_MONEY_NAME);
		return -1;
	}

	
	OldShopData.Money = pstPlayer->stRoleData.RoleDetail.Money;
	OldShopData.Bull = pstPlayer->stRoleData.RoleDetail.Bull;
	OldShopData.GoodsDur = pstRoleArm->Dur;
	
	iRet = package_dec_money(pstEnv, pstPlayer, ATTR_ID_BULL, iPrice);
	z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_REPAIR, 0, iPrice-iRet, iRet, "修理");
	
	iOldDur = pstRoleArm->Dur;
	pstRoleArm->Dur = z_get_arm_max_dur(pstEnv, pstItemDef)*DUR_UNIT;

	pstCSRoleArm = pstRolePakChg->ArmChg.Arms + pstRolePakChg->ArmChg.ChgNum;
	pstCSRoleArm->ListType = pstRepairOne->SrcListType;
	memcpy(&pstCSRoleArm->Arm, pstRoleArm, sizeof(pstCSRoleArm->Arm));
	pstRolePakChg->ArmChg.ChgNum++;


	Z_CSHEAD_INIT(&pstPkgRes->Head, PACKAGE_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkgRes);

	if (0 == iOldDur/DUR_UNIT && LIST_TYPE_WEAR == pstRepairOne->SrcListType)
	{
		z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	z_roleshop_oplog(pstEnv,pstPlayer ,pstItemDef , &OldShopData , pstRoleArm->Dur,
		pstRoleArm->WID, OP_SHOP_REPONE ,NULL, 0);
	
	return 0;
}

int shop_repair_all(ZONESVRENV* pstEnv, Player *pstPlayer, int iRePairFlag)
{
	int i=0;
	CSPKG *pstPkgRes = &pstEnv->stCSRes;
	CSSHOPSVR *pstShopSvr = &pstPkgRes->Body.ShopSvr;
	int iPrice = 0;
	int iFlag = 0;
	OPSHOPDATA OldShopData;
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	ROLEARM *pstRoleArm = NULL;
	ITEMDEF *pstItemDef = NULL;
	int iNotRePairNum = 0;
	int iRet;
		
	SUNDRIESPACKAGE *pstPack = &pstPlayer->stRoleData.Package.SundriesPackage;
	ROLEGRID *pstGrid = NULL;

	task_player_repair_trace(pstEnv, pstPlayer);

	OldShopData.Money = pstPlayer->stRoleData.RoleDetail.Money;
	OldShopData.Bull = pstPlayer->stRoleData.RoleDetail.Bull;
	
	if (iRePairFlag == REPAIR_WAER)
	{
		for (i=0; i<pstRoleWear->ArmNum; i++)
		{
			pstRoleArm = &pstRoleWear->RoleArms[i];
			pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleArm);
			if (NULL == pstItemDef || pstItemDef->ArmDur.MaxDur <= 0 || 
				z_item_get_result(pstItemDef, RESULT_NO_REPAIR) ||
				(int)pstRoleArm->Dur/DUR_UNIT >= z_get_arm_max_dur(pstEnv, pstItemDef) )
			{
				iNotRePairNum ++;
				continue;
			}

			iPrice += z_arm_repair_price(pstEnv, pstRoleArm, pstItemDef,pstPlayer);
		}

		if (iNotRePairNum == pstRoleWear->ArmNum)
		{
			if ( wear_has_one_fengyin(pstPlayer))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NPC112);
				return 0;
			}
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NPC113);
			return 0;
		}
		
		if (0 >= iPrice)
		{
			iPrice = 1;
		}
		
		//宠物天赋,修理
		pet_native_skill_result(pstEnv,pstPlayer,RESULT_NATIVE_REPAIR_COST_MAP, &iPrice,NULL);	

		if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, iPrice))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
								ZONEERR_NPC105, iPrice, MONEY_TYPE_MONEY_NAME);
			return -1;
		}

		iRet = package_dec_money(pstEnv, pstPlayer, ATTR_ID_BULL, iPrice);
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_REPAIR, 0, iPrice-iRet, iRet, "修理");

		for (i=0; i<pstRoleWear->ArmNum; i++)
		{
			pstRoleArm = &pstRoleWear->RoleArms[i];
			pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleArm);
			if (NULL == pstItemDef || pstItemDef->ArmDur.MaxDur <= 0 ||
				z_item_get_result(pstItemDef, RESULT_NO_REPAIR) ||
				(int)pstRoleArm->Dur/DUR_UNIT >= z_get_arm_max_dur(pstEnv, pstItemDef))
			{
				continue;
			}

			if (0 == pstRoleArm->Dur/DUR_UNIT)
			{
				iFlag = 1;
			}
			
			pstRoleArm->Dur = z_get_arm_max_dur(pstEnv, pstItemDef)*DUR_UNIT;
		}
		if (iFlag)
		{
			z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}
	}
	else if(iRePairFlag == REPAIR_PACK)
	{
		for (i=0; i<pstPack->Num; i++)
		{
			pstGrid = &pstPack->RoleGrids[i];
			if (pstGrid->Type == GRID_ITEM_ITEM)
			{
				continue;
			}
			
			pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstGrid->GridData.RoleItem);
			if (!pstRoleArm ||
				NULL == pstItemDef || pstItemDef->ArmDur.MaxDur <= 0 || 
				z_item_get_result(pstItemDef, RESULT_NO_REPAIR) ||
				(int)pstRoleArm->Dur/DUR_UNIT >= z_get_arm_max_dur(pstEnv, pstItemDef) )
			{
				iNotRePairNum ++;
				continue;
			}

			iPrice += z_arm_repair_price(pstEnv,  &pstGrid->GridData.RoleArm, pstItemDef,pstPlayer);
		}

		if (iNotRePairNum == pstPack->Num)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NPC113);
			return 0;
		}
		
		if (0 >= iPrice)
		{
			iPrice = 1;
		}

		//宠物天赋,修理
		pet_native_skill_result(pstEnv,pstPlayer,RESULT_NATIVE_REPAIR_COST_MAP, &iPrice,NULL);		

		if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, iPrice))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
								ZONEERR_NPC105, iPrice, MONEY_TYPE_MONEY_NAME);
			return -1;
		}

		iRet = package_dec_money(pstEnv, pstPlayer, ATTR_ID_BULL, iPrice);
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_REPAIR, 0, iPrice-iRet, iRet, "修理");

		for (i=0; i<pstPack->Num; i++)
		{
			pstGrid = &pstPack->RoleGrids[i];
			if (pstGrid->Type == GRID_ITEM_ITEM)
			{
				continue;
			}
			pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstGrid->GridData.RoleItem);
			if (!pstRoleArm ||
				NULL == pstItemDef || pstItemDef->ArmDur.MaxDur <= 0 ||
				z_item_get_result(pstItemDef, RESULT_NO_REPAIR) ||
				(int)pstRoleArm->Dur/DUR_UNIT >= z_get_arm_max_dur(pstEnv, pstItemDef))
			{
				continue;
			}

			pstGrid->GridData.RoleArm.Dur = z_get_arm_max_dur(pstEnv, pstItemDef)*DUR_UNIT;
		}

	}
	else 
	{
		return -1;
	}

	pstShopSvr->OpType = SHOP_REPAIR_ALL;
	pstShopSvr->ShopSvrData.ShopRepairAll = iRePairFlag;
	Z_CSHEAD_INIT(&pstPkgRes->Head, SHOP_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkgRes);

	z_roleshop_oplog(pstEnv, pstPlayer ,NULL , &OldShopData , 0, 0 , OP_SHOP_REPALL ,NULL, 0);

	return 0;
}

int get_ride_shop_selllist_id(ZONESVRENV* pstEnv, Player *pstPlayer, CSSHOPCLT *pstShopClt)
{
	RIDEDEF *pstRideDef = NULL;
	RIDENPCSELECT *pstRideNpcSelect;
	UNUSED(pstShopClt);
	
	if(!(pstPlayer->stOnline.State & (CS_STAT_RIDE_ON|CS_STAT_FLY)))
	{
		return 0;
	}

	pstRideDef = z_find_ride_def(pstEnv, pstPlayer->stOnline.RideItemID, pstPlayer->stOnline.RideLevel);
	if(NULL == pstRideDef)
	{
		return 0;
	}

	pstRideNpcSelect = z_get_ride_npc(pstRideDef, RIDE_OPEN_SHOP);
	if(NULL == pstRideNpcSelect)
	{
		return 0;
	}

	return pstRideNpcSelect->SelectVal;
}

// unpRebateCount--总折扣--out
// pRebateTypeNum -- 折扣类型数量---out
// pRebateTypes -- 折扣类型数组---out
int player_cacl_rebate( ZONESVRENV* pstEnv, Player *pstPlayer, unsigned int *unpRebateCount,
						char *pRebateTypeNum, char *pRebateTypes)
{
	unsigned int unRebate = 100;

	if (pRebateTypeNum)
		*pRebateTypeNum = 0;

	// 红名折扣
	if (pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal >= PLAYER_PK_FRENZY)
	{
		unRebate *=  1.0*REBATE_RED_NAME/100;
		if (pRebateTypeNum && pRebateTypes)
		{
			pRebateTypes[(int)(*pRebateTypeNum)] = REBATE_TYPE_RED_NAME;
			(*pRebateTypeNum)++;
		}
	}

	// 沙巴克折扣
	if (is_world_city_owner(pstEnv, pstPlayer))
	{
		int iWeek = world_city_fight_keep_num(pstEnv);
		int iCityRebate;
		int iType;
		// 1周90%
		if (iWeek < 1)
		{
			iCityRebate = REBATE_CITY_OWNER_1;
			iType = REBATE_TYPE_CITY_OWNER_1;
		}
		// 2-4周85%
		else if (iWeek < 4)
		{
			iCityRebate = REBATE_CITY_OWNER_2;
			iType = REBATE_TYPE_CITY_OWNER_2;
		}
		// 5周 80%
		else
		{
			iCityRebate = REBATE_CITY_OWNER_3;
			iType = REBATE_TYPE_CITY_OWNER_3;
		}

		if (pRebateTypeNum && pRebateTypes)
		{
			pRebateTypes[(int)(*pRebateTypeNum)] = iType;
			(*pRebateTypeNum)++;
		}
		unRebate *=  1.0*iCityRebate/100;
	}

	*unpRebateCount = unRebate;
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int player_shop(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	Npc *pstNpc, stNpc;
	int iSellListID;
	CSSHOPCLT *pstShopClt = &pstCsPkg->Body.ShopClt;
	UNUSED(pstFrameHead);

	if (pstShopClt->NpcID == -1) //vip 随身仓库
	{
		RESULT *pstVipResult;

		pstVipResult = star_get_result(pstEnv, pstPlayer, STAR_RESULT_SHOP);
		if (NULL == pstVipResult)
		{
			return -1;
		}

		stNpc.nSellListIdx = -1;
		stNpc.nSellListID = pstVipResult->ResultVal1;
		pstNpc = &stNpc;
	}
	else if (pstShopClt->NpcID != pstPlayer->stOnline.iDialogID)
	{
		//骑乘NPC 售卖列表id
		iSellListID = get_ride_shop_selllist_id(pstEnv, pstPlayer, pstShopClt);
		if(iSellListID == 0)
		{
			return -1;
		}

		//伪造购买需要用到的NPC 相关信息 
		stNpc.nSellListIdx = -1;
		stNpc.nSellListID = iSellListID;
		pstNpc = &stNpc;
	}
	else
	{
		pstNpc = z_id_npc(pstAppCtx, pstEnv, pstShopClt->NpcID);
		if (NULL == pstNpc || pstNpc->nSellListID <= 0)
		{
			return -1;
		}

		if (0 == z_player_act_status(pstAppCtx, pstEnv, pstPlayer, RES_ACTION_DIALOG))
		{
			return -1;
		}
		
		if (0 > z_check_op_dist(pstEnv, pstPlayer, pstNpc->stMon.stMap.iID, &pstNpc->stMon.stCurrPos, OP_DIST+pstNpc->stMon.unRadius))
		{
			return -1;
		}

		pstNpc->stMon.tFreezeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + 10;
		if (pstNpc->stMon.cMoving)
		{
			z_mon_stop(pstEnv, &pstNpc->stMon); 
		}
	}

	switch(pstShopClt->OpType)
	{
		case SHOP_BUY:
			shop_buy(pstEnv, pstPlayer, pstNpc, &pstShopClt->ShopCltData.ShopBuy);
			break;
		case SHOP_BUY_SELLED:
			shop_buy_selled(pstEnv, pstPlayer, &pstShopClt->ShopCltData.ShopBuySelled);
			break;
		case SHOP_SELL:
			shop_sell(pstEnv, pstPlayer, &pstShopClt->ShopCltData.ShopSell);
			break;
		case SHOP_REPAIR_ONE:
			shop_repair_one(pstEnv, pstPlayer, &pstShopClt->ShopCltData.ShopRepairOne);
			break;
		case SHOP_REPAIR_ALL:
			shop_repair_all(pstEnv, pstPlayer, pstShopClt->ShopCltData.ShopRepairAll);
			break;
		default:
			return -1;
			break;
	}
	
	return 0;
}

NPCINFODEF *npcinfo_def_find(ZONESVRENV *pstEnv, int iNpcID, int *piNpcInfoIdx)
{
	return (NPCINFODEF *)z_find_def(pstEnv->pstZoneObj->sNpcInfoDef, iNpcID, NPC_INFO_DATA, 
									pstEnv->pstZoneObj->iNpcInfoDef, piNpcInfoIdx);
}

int z_check_npc_script(ZONESVRENV* pstEnv, Npc *pstNpc)
{	
	ScriptIndex *pstScriptInd;
	UNUSED(pstEnv);

	pstScriptInd = &pstNpc->stScriptIdx;
	if (pstNpc->szScriptName[0] == 0)
	{
		return 0;
	}
	
	if (0 == check_script_proc(pstNpc->szScriptName, pstScriptInd, NPC_SCRIPT_PROC_ACCEPT_TASK))
	{
		pstNpc->cScriptFlag |= NPC_SCRIPT_PROC_ACCEPT_TASK_MAP;
	}

	if (0 == check_script_proc(pstNpc->szScriptName, pstScriptInd, NPC_SCRIPT_PROC_AWARD_TASK))
	{
		pstNpc->cScriptFlag |= NPC_SCRIPT_PROC_AWARD_TASK_MAP;
	}

	return 0;
}


int z_active_npc(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iCount)
{
	static struct timeval stRoundTv;
	static struct timeval stRoundStartTv;
	static unsigned int iActivePos = 0, iRound = 0;
	int i;
	struct timeval stSub;
	long long llMs=0;
	Npc *pstNpc;
	TMEMBLOCK *pstMemBlock;
	UNUSED(&iCount);

	if (0 == stRoundTv.tv_sec)
	{
		llMs = NPC_ACTIVE_TIMER;
	}
	else
	{
		TV_DIFF(stSub, pstAppCtx->stCurr, stRoundTv);
		TV_TO_MS(llMs, stSub);
	}

	if (0 > llMs)
	{
		TV_CLONE(stRoundTv, pstAppCtx->stCurr);
		TV_CLONE(stRoundStartTv, pstAppCtx->stCurr);
	}
		
	if (llMs < NPC_ACTIVE_TIMER)
	{
		return 0;
	}

	if (0 == iActivePos)
	{
		TV_CLONE(stRoundStartTv, pstAppCtx->stCurr);
	}

	for (i=0; i<MAX_NPC; i++)
	{
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstNpcPool, iActivePos);
		iActivePos++;

		if (!pstMemBlock->fValid)
		{
			continue;
		}
		
		pstNpc = (Npc *)pstMemBlock->szData;
		
		if (pstNpc->stMon.tLifeEnd &&pstNpc->stMon.tLifeEnd < pstAppCtx->stCurr.tv_sec)
		{
			z_monster_leave_map(pstAppCtx, pstEnv, &pstNpc->stMon);
			z_free_monster(pstAppCtx, pstEnv, &pstNpc->stMon);

			continue;
		}

		mon_active(pstAppCtx, pstEnv, pstMemBlock, iActivePos, iRound);
	}
	
	if (iActivePos < MAX_NPC)
	{
		return 0;
	}
	
	iActivePos = 0;
	iRound++;
	TV_CLONE(stRoundTv, pstAppCtx->stCurr);

	TV_DIFF(stSub, pstAppCtx->stCurr, stRoundStartTv);
	TV_TO_MS(llMs, stSub);
	if (llMs > NPC_ACTIVE_TIMER)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "active npc loop %lld ms", llMs);
	}
	
	return 0;
}

 //介个是否还有用?
int player_add_booty(ZONESVRENV* pstEnv, Player *pstPlayer, TASKGETITEM *apstItemID,unsigned int iNum,
						int iviewFlag, RESPOS *pstPos)
{
	int iIndex;
	TMEMBLOCK *pstMemBlock;
	DropItem *pstDropItem;
	int iRet = 0;
	ROLEGRID *pstRoleGrid;
	TASKGETITEM    stItem;
	unsigned  int i = 0;
	ZoneBootyGrid stBootyGrid;
	ITEMDEF  *pstItemDef;
	ZoneTeam *pstTeam;

	//日志流水号
	z_oplog_event_begin(pstEnv);
	
	pstTeam = player_team_get(pstEnv, pstPlayer);
	
	for( i=0; i<iNum && i<MAX_ROLE_BOOTY; i++ )
	{
		// 按照char的移位
		memcpy(&stItem,(char*)apstItemID+i*sizeof(TASKGETITEM),sizeof(TASKGETITEM));
		if( stItem.ItemID== 0 )
		{
			break;
		}
		pstRoleGrid = &stBootyGrid.stGrid;
		if (0 > item_create(pstEnv->pstAppCtx, pstEnv, pstRoleGrid, stItem.ItemID))
		{
			return -1;
		}
		
		pstRoleGrid->GridData.RoleItem.Num = stItem.Num;

		//超过全局限制就不掉落了
		if(0 > world_item_limit_check(pstEnv, stItem.ItemID))
		{
			continue;
		}

		//记录下全局限制的掉落
		world_item_limit_add(pstEnv, stItem.ItemID, stItem.Num);
		
		stBootyGrid.ullRoleID = pstPlayer->stRoleData.RoleID;
		stBootyGrid.bType = 0;
		stBootyGrid.bViewType = 0;
		stBootyGrid.bRoll = 0;
		if(pstTeam)
		{
			stBootyGrid.ullTeamID = pstTeam->stTeamInfo.TeamID;
		}
		
		pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstRoleGrid->GridData.RoleItem);
		if(pstItemDef && pstItemDef->DropSelfView)
		{
			stBootyGrid.bViewType = BOOTY_DROP_VIEW_SELF;
		}

		iIndex = tmempool_alloc(pstEnv->pstItemPool);
		if (0 > iIndex)
		{
			return -1;
		}
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstItemPool, iIndex);
		pstDropItem = (DropItem *)pstMemBlock->szData;

		memset(pstDropItem,0,sizeof(DropItem));
		pstDropItem->cType = DROP_ITEM_GRID;
		pstDropItem->cStat = DROP_ITEM_NOSTAY;
		pstDropItem->unRefreshTime = 0;
		memcpy(&pstDropItem->stObj.stBootyGrid, &stBootyGrid, sizeof(pstDropItem->stObj.stBootyGrid));
		pstDropItem->iDefIdx = stBootyGrid.stGrid.GridData.RoleItem.DefID;
		pstDropItem->iID = 0;
		//pstDropItem->stMap = pstMon->stMap;;
		pstDropItem->stMap.iID = pstPlayer->stRoleData.Map;
		pstDropItem->stMap.iPos = pstPlayer->stOnline.iMapPos;
		pstDropItem->stPos = *pstPos;
		pstDropItem->iStayTime = DROP_ITEM_TIME;
		pstDropItem->iMIndex = iIndex;
		pstDropItem->ullTeamID = 0;
		pstDropItem->iFromID = 0;
		
		

		if (0 > (iRet = z_item_refresh(pstEnv, pstDropItem,0)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "item refresh fail ret %d, item %d %d, map %d, pos (%d,%d)", iRet, pstDropItem->iID,
						pstDropItem->iDefIdx, pstDropItem->stMap.iID, pstDropItem->stPos.X, pstDropItem->stPos.Y);
		}
		else
		{
			
			z_role_booty_drop_oplog(pstEnv->pstAppCtx,pstEnv,pstDropItem->stMap.iID,&pstDropItem->stPos,
					&stBootyGrid.stGrid, ITEM_DROP_TYPE_MAP, 0);
		}
	}

	//if(iviewFlag ==1 )
	//{
	//	package_booty_view( pstEnv, pstPlayer, pstDropItem->iID);
	//}
	
	return iRet;
	UNUSED(iviewFlag);	//add by paraunused.pl
}

int mon_life_action(ZONESVRENV* pstEnv, Monster *pstMon)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstActionRes->ID = pstMon->iID;
	pstActionRes->ActionID = ACTION_MON_LIFE;

	//存活时间
	if (pstMon->tLifeEnd >=  pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		pstActionRes->Data.MonLife.LifeEndTime = pstMon->tLifeEnd - pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	else
	{
		pstActionRes->Data.MonLife.LifeEndTime = -1;
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_monview(pstEnv->pstAppCtx, pstEnv, pstMon, &stPkg, 0);

	return 0;
}

int z_npc_die(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Npc* pstNpc)
{
	CSPKG stPkg;
	CSACTIONRES *pstActRes = &stPkg.Body.ActionRes;
	MONSTERDEF *pstMonDef;
	
	//不刷新的怪物就直接删除了
	if(!pstNpc->stMon.cRefresh)
	{
		z_monster_leave_map(pstAppCtx, pstEnv,  &pstNpc->stMon);
		z_free_monster(pstAppCtx, pstEnv, &pstNpc->stMon);
		return 0;
	}
	
	if (MONSTER_DIE == pstNpc->stMon.cStat)
	{
		return -1;
	}
	
	pstMonDef = z_get_mon_def(pstEnv, &pstNpc->stMon);
	if(NULL == pstMonDef)
	{
		return -1;
	}

	pstNpc->stMon.cStat = MONSTER_DIE;
	pstNpc->stMon.cMoving = 0;
	pstNpc->stMon.tRefresh = pstAppCtx->stCurr.tv_sec + (pstMonDef->RefreshTime ? pstMonDef->RefreshTime: 300);
	pstNpc->stMon.tLeaveMap = pstAppCtx->stCurr.tv_sec + MON_LEAVE_MAP_TIME;
	
	pstActRes->ID = pstNpc->stMon.iID;
	pstActRes->ActionID = ACTION_DESTROY;
	pstActRes->Data.Destroy.x = pstNpc->stMon.stCurrPos.X;
	pstActRes->Data.Destroy.y = pstNpc->stMon.stCurrPos.Y;
		
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_mon_area_view(pstAppCtx, pstEnv, &pstNpc->stMon, &stPkg);
	

	return 0;
}

int mon_set_camp(Monster *pstMon, int iCamp)
{
	if(iCamp > 0)
	{
		pstMon->iCampIdx = iCamp;
		pstMon->cMonMiscFlag |= MON_MISC_FLAG_IMPVIEW;
		mon_set_pkinfo(pstMon, MON_PK_TYPE_CAMP, iCamp);
	}

	return iCamp;
}

int z_mon_def_data(ZONESVRENV* pstEnv, Monster* pstMon, MONSTERDEF *pstMonDef)
{
	MONDYNATTRDEF *pstMonDynAttr = NULL;
	ZONEPWORLDINST *pstPworldInst = NULL;
	ZONEMAPINST *pstMapInst;
	ZONEPWORLD* pstPworld = NULL;
	
	if (0 > z_mon_ai_data(pstEnv, pstMon, pstMonDef))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "monster %d ai data error", pstMon->iDefIdx);
		return -1;
	}

	if ( pstMon->bSubType < MON_SUB_TYPE_TMP || OBJ_NPC == pstMon->cType)
	{
		pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
		if (pstMapInst)
		{
			pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
		}
	}

	if (pstPworldInst && pstPworldInst->DynType && pstMonDef->DynAttr)
	{
		pstPworld	= z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
		if( !pstPworld )
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "z_in_destroy_pworld_inst: Pworld %d defid %d not find", 
						pstPworldInst->iPworldInstID, pstPworldInst->stPworldIdx.iID);
		}
		
		pstMonDynAttr = z_find_mondyn(pstEnv, pstPworldInst->bPlayerMeanLevel);
		if (pstPworld != NULL && pstMonDynAttr != NULL  && pstPworldInst->stPworldselect.PworldGrade < MAX_DYN_GRADE )
		{
			PWORLDDYNATTR *pstDyn = &pstPworld->stPworldDef.DynAttr;
			
			pstMon->stMonDynAttr.Level = pstMonDynAttr->Level;
			pstMon->stMonDynAttr.IceDefMul = pstMonDynAttr->IceDefMul;
			pstMon->stMonDynAttr.PhsicDefMul = pstMonDynAttr->PhsicDefMul;
			pstMon->stMonDynAttr.HitMul = pstMonDynAttr->HitMul;
			pstMon->stMonDynAttr.FleeMul = pstMonDynAttr->FleeMul;
			pstMon->stMonDynAttr.TmpLevel = pstMonDynAttr->TmpLevel;
			pstMon->stMonDynAttr.IceHurtDefMul= pstMonDynAttr->IceHurtDefMul;
			pstMon->stMonDynAttr.PhsicHurtDefMul= pstMonDynAttr->PhsicHurtDefMul;
			memcpy(&pstMon->stMonDynAttr.ColorLevel, &pstMonDynAttr->ColorLevel, sizeof(pstMon->stMonDynAttr.ColorLevel));
			
			pstMon->stMonDynAttr.HPMul = pstMonDynAttr->DynHPMul[pstPworldInst->stPworldselect.PworldGrade]*(1 + pstDyn->DynHPMul[pstPworldInst->stPworldselect.PworldGrade]/100.0);
			pstMon->stMonDynAttr.DynHurtMul = pstMonDynAttr->DynHurtMul[pstPworldInst->stPworldselect.PworldGrade]*(1 + pstDyn->DynHurtMul[pstPworldInst->stPworldselect.PworldGrade]/100.0);
			pstMon->stMonDynAttr.ExpMul = pstMonDynAttr->DynExpMul[pstPworldInst->stPworldselect.PworldGrade]*(1 + pstDyn->DynExpMul[pstPworldInst->stPworldselect.PworldGrade]/100.0);
			pstMon->stMonDynAttr.IceHurtMul = pstMonDynAttr->IceHurtMul;
			pstMon->stMonDynAttr.PhsicHurtMul = pstMonDynAttr->PhsicHurtMul;

			pstMon->stMonDynAttr.ElementAtkMul = pstMonDynAttr->ElementAtkMul[pstPworldInst->stPworldselect.PworldGrade];
			pstMon->stMonDynAttr.ElementDefMul = pstMonDynAttr->ElementDefMul[pstPworldInst->stPworldselect.PworldGrade];
		}
		else
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "z_find_mondyn  error level= %d PworldGrade = %d",pstPworldInst->bPlayerMeanLevel, pstPworldInst->stPworldselect.PworldGrade);
		}
	}
	
	if (!pstMonDynAttr)
	{
		pstMon->stMonDynAttr.Level = pstMonDef->Level;
	}

	if (pstMonDef->DynMask && pstMonDef->MoveType == MOVE_TYPE_NO)
	{
		pstMon->cMonMiscFlag |= MON_MISC_FLAG_DYN;
	}

	if (pstMonDef->MoveType == MOVE_TYPE_NO || (MON_MISC_TYPE_NO_DYNBAK & pstMonDef->MiscType))
	{
		pstMon->cMonMiscFlag |= MON_MISC_FLAG_NO_DYNBAK;
	}

	if ((MON_MISC_FLAG_DYN & pstMon->cMonMiscFlag) || 
		(OBJ_NPC == pstMon->cType && pstMapInst && CLAN_CITY_MAP_ID == GET_MAPID(pstMapInst->iMapInstID) &&
		  pstMonDef->MoveType == MOVE_TYPE_NO) ||
		  (OBJ_NPC == pstMon->cType && (pstMonDef->MiscType & MON_MISC_TYPE_NPC_MOVE)))
	{
		pstMon->cMonMiscFlag |= MON_MISC_FLAG_MAPBROAD;
	}
	else
	{
		if ( MON_SUB_TYPE_BOSS == pstMon->bSubType || 
			MON_SUB_TYPE_HARD == pstMon->bSubType ||
		 MON_SUB_TYPE_SKILL_DESTORY == pstMon->bSubType ||
		 MON_SUB_TYPE_SKILL_NODESTORY == pstMon->bSubType || 
		 OBJ_NPC == pstMon->cType ||
		 (MON_SUB_TYPE_HUMAN_FRIEND == pstMon->bSubType && pstMonDef->MoveType == MOVE_TYPE_NO))
		{
			pstMon->cMonMiscFlag |= MON_MISC_FLAG_IMPVIEW;
		}
	}

	z_check_mon_script(pstEnv, pstMon, pstMonDef);

	pstMon->WID = z_get_gid(pstEnv->pstAppCtx, pstEnv);

	return 0;
}

int z_mon_speech_begin_or_end_fight(ZONESVRENV* pstEnv, Monster* pstMon,int iType)
{
      MONSTERSPEECHDEF *pstMonSpeech;

      pstMonSpeech = z_find_monsterspeech_def(pstEnv,  pstMon->iDefIdx, NULL);
      if (pstMonSpeech)
      {
              int iActIdx;

              iActIdx = z_find_actspeech_by_conditiontype( pstMonSpeech, iType, 0);
              if (iActIdx > -1
                      &&      pstMonSpeech->MonActSpeech[iActIdx].ConditionVal != 0
                      &&       (int )RAND1(10000) < pstMonSpeech->MonActSpeech[iActIdx].ConditionVal*100)
              {
                      z_mon_speech(pstEnv, pstMon, NULL, pstMonSpeech->MonActSpeech[iActIdx].BroadcastType, iType, iActIdx);
              }
      }

      return 0;
}

int z_get_monsterdef_misctype(ZONESVRENV* pstEnv, Monster* pstMon)
{
	MONSTERDEF *pstMonDef = NULL;
	pstMonDef = z_get_mon_def( pstEnv, pstMon);
	if(pstMonDef)
	{
		return pstMonDef->MiscType;
	}
	return 0;
}

int mon_action_map_show(ZONESVRENV * pstEnv,  PworldCampMonInfo *pstMonInfo,ZONEMAPINST *pstMapInst,
	int iOpType,int iBroadcastType,Player* pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;		
	
	memset(pstAction,0,sizeof(*pstAction));
	
	pstAction->ID = pstMonInfo->iMemID;
	pstAction->ActionID = ACTION_MAP_SHOW;

	pstAction->Data.MapShow.OpType = iOpType;
	pstAction->Data.MapShow.Pos = pstMonInfo->stPos;
	pstAction->Data.MapShow.MapID = GET_MAPID(pstMapInst->iMapInstID);
	pstAction->Data.MapShow.MemID = pstMonInfo->iMemID;
	pstAction->Data.MapShow.DataType = MAP_SHOW_DATA_MON_CAMP;
	pstAction->Data.MapShow.Data.MapShowMonCamp.CampID = pstMonInfo->iCamp;
	pstAction->Data.MapShow.Data.MapShowMonCamp.MonsterID = pstMonInfo->iDefID;
	pstAction->Data.MapShow.Data.MapShowMonCamp.HP = pstMonInfo->iHP;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);

	if (0 == iBroadcastType && pstPlayer)
	{
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	else if (pstMapInst)
	{
		map_broadcast(pstEnv->pstAppCtx, pstEnv, pstMapInst,  &stPkg);
	}
	else
	{
		return -1;
	}

	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

//没有刷新出地图之前设置
int mon_set_pkinfo(Monster *pstMon, int iType, tdr_ulonglong iID)
{
	pstMon->iPKType = iType;
	pstMon->ullPKID = iID;
	return 0;
}

int shop_vip_open(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	RESULT *pstVipResult;

	pstVipResult = star_get_result(pstEnv, pstPlayer, STAR_RESULT_SHOP);
	if (NULL == pstVipResult)
	{
		return -1;
	}

	open_shop_to_clt(pstEnv, pstPlayer, pstVipResult->ResultVal1, 1);
	
	return 0;
}


//lzk 当融焰怪物死亡时调用
int mon_rongyan_boss_die(ZONESVRENV* pstEnv,Monster* pstMon)
{
	Player *pstPlayer = NULL;
	ZoneTeam *pstTeam = NULL;
	int iLevel = 0;
	CSRONGYANBOSSINFO stRongYanBossInfo;
	TEAMMEMBER *pstCaptain;
	Player *pstCaptainPlayer = NULL;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	int iRongYanTeamCapId = pstMon->iRongYanTeamCapId;

	int k;
	int TeamMemberNum;
	Player *pstMemberPlayer = NULL;
	
	pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, iRongYanTeamCapId);
	if(NULL == pstPlayer)
	{
		return -1;
	}
	
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(NULL == pstTeam)
	{
		return -1;
	}

	pstCaptain = player_team_get_captain(pstTeam);
	if(pstCaptain == NULL)
	{
		//没有队长
		return -1;
	}
	
	pstCaptainPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,pstCaptain->MemberName);
	if(pstCaptainPlayer == NULL)
	{
		return -1;
	}

	TeamMemberNum = pstTeam->stTeamInfo.MemberNum;
	if( TeamMemberNum <= 0)
	{
			return -1;
	}

	//当前层数
	iLevel = pstTeam->iRongYanCurLevel;
	
	if(iLevel > 0 && iLevel < 10)
	{
		stRongYanBossInfo.BeginSelectMul = pstTeam->iRongYanSelectMul;
		stRongYanBossInfo.CurLayer = pstTeam->iRongYanCurLevel;

		//每个队员都发真假boss 框
		for(k = 0;k<TeamMemberNum;k++)
		{
			pstMemberPlayer =  z_name_player(pstEnv->pstAppCtx, pstEnv,pstTeam->stTeamInfo.TeamMember[k].MemberName);
			if(NULL == pstMemberPlayer)
			{
				continue;
			}

			player_rongyan_boss_ui(pstEnv, pstMemberPlayer,&stRongYanBossInfo);
		}
	
		// 3分钟后，系统自动选择
		pstTeam->tRongYanBossSelectTimeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + 180;

		pstTeam->iRongYanStatus = 0;
		
		//给经验
		mon_rongyan_player_add_exp(pstEnv,pstTeam,pstMon);

	}
	else if(10 == iLevel)
	{
		//以下是为了得到地图名并世界播报----------begin--------------
		stIdx.iID = pstTeam->stRongYanZoneIdx.iID;
		stIdx.iPos = pstTeam->stRongYanZoneIdx.iPos;
		pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
		if (NULL == pstMapInst)
		{
			return -1;
		}

		pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
		if (NULL == pstMapIdx)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,  "pstMapIdx NULL");
			return -1;
		}

		if (pstEnv->pstConf->CountryVer != COUNTRY_VER_KOREA)
		{
			z_sys_strf_broadcast(pstEnv, SYS_SHOW,ZONEERR_TEAM317,pstCaptain->MemberName,pstMapIdx->stMapDef.Name,iLevel);
		}
		//-----------------------end------------------------------

		//给经验
		mon_rongyan_player_add_exp(pstEnv,pstTeam,pstMon);
		
		rongyan_all_status_clean(pstEnv,pstTeam,TeamMemberNum);
	}
	else if(0 == iLevel)
	{
		//gm清除
		rongyan_all_status_clean(pstEnv,pstTeam,TeamMemberNum);
	}
	else if(-1 == iLevel)
	{
		z_team_sys_strf(pstEnv, pstTeam, SYS_TEAM, ZONEERR_RONGYAN2);
		
		//队伍解散  情况   进行清0   和通知客户端
		rongyan_all_status_clean(pstEnv,pstTeam,TeamMemberNum);
	}
	else if(-2 == iLevel)
	{
	//在限定时间内没有将怪物击杀  ,zone_svr.c中mon_active  将pstTeam->iRongYanCurLevel 赋值为-2
		if(pstMon->iHP > 0)
		{
				z_team_sys_strf(pstEnv, pstTeam, SYS_TEAM, ZONEERR_RONGYAN3);
				rongyan_all_status_clean(pstEnv,pstTeam,TeamMemberNum);
		}
	}
	else
	{
		return -1;
	}
	
	pstMon->iRongYanTeamCapId = 0;
	return 0;
}

int mon_rongyan_die_player_add_exp(ZONESVRENV* pstEnv,Player *pstPlayer,int BeginSelectMul,int MonBasicExp,int DynExpMul,int CurLevel)
{
	RONGYANJUSHOUDEF *pstRongYanJuShouDef; 
	int SelectMul;
	tdr_longlong llExp;
		
	pstRongYanJuShouDef = z_find_rongyanjushou_def(pstEnv,CurLevel);
	if(!pstRongYanJuShouDef)
	{
		return -1;
	}

	//BeginSelectMul   只是索引而已，代表1,2,3,4
	SelectMul = pstRongYanJuShouDef->RongYan[BeginSelectMul-1].ExpMul;
	
    llExp = SelectMul*MonBasicExp*(1+DynExpMul/100.0);
	pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_EXPNODAYLIMIT;
	player_add_exp(pstEnv,pstPlayer,llExp,0);
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_EXPNODAYLIMIT;
	return 0;
}


//融焰怪死亡后。队伍中每一个人都经验
int mon_rongyan_player_add_exp(ZONESVRENV* pstEnv,ZoneTeam *pstTeam,Monster* pstMon)
{
	int i;
	int TeamMemberNum;
	Player *pstMemberPlayer = NULL;
	int iPlayerLevel = 0;
	RONGYANMONDYNATTRDEF *pstRongYanMonDynAttrDef;
	int MonBasicExp = 0;
	MONSTERDEF *pstMonDef;
	LEVELUP *pstLevelUP;
	
	TeamMemberNum = pstTeam->stTeamInfo.MemberNum;
	if( TeamMemberNum <= 0)
	{
		return -1;
	}

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if(NULL == pstMonDef)
	{
		return -1;
	}

	//给每个成员经验
	for(i = 0;i<TeamMemberNum;i++)
	{
		pstMemberPlayer =  z_name_player(pstEnv->pstAppCtx, pstEnv,pstTeam->stTeamInfo.TeamMember[i].MemberName);
		if(NULL == pstMemberPlayer)
		{
			continue;
		}

		//假设该玩家不和怪物同一张地图 ，不给奖励
		if(pstMemberPlayer->stRoleData.Map != pstMon->stMap.iID)
		{
			continue;
		}

		iPlayerLevel = pstMemberPlayer->stRoleData.Level;		

		pstLevelUP = z_get_levelup(pstEnv, iPlayerLevel);
		if (NULL == pstLevelUP)
		{
			return -1;
		}

		MonBasicExp = pstMonDef->Booty.Exp*(pstLevelUP->ContributeExp/10000.0);
		
		pstRongYanMonDynAttrDef = z_find_rongyanmondynattr_def(pstEnv,iPlayerLevel);
		if(NULL == pstRongYanMonDynAttrDef)
		{
			return -1;
		}

		mon_rongyan_die_player_add_exp(pstEnv,pstMemberPlayer,pstTeam->iRongYanSelectMul,MonBasicExp,pstRongYanMonDynAttrDef->DynExpMul,pstTeam->iRongYanCurLevel);
	}
	return 0;
}

//把熔岩怪队伍记住的数清0，并告诉客户端关闭所有相应面板
int rongyan_all_status_clean(ZONESVRENV* pstEnv,ZoneTeam *pstTeam,int iTeamMemberNum)
{
	CSRONGYANBOSSINFO stRongYanBossInfo;
	Player *pstMemberPlayer = NULL;
	int k;
	
	pstTeam->iRongYanCurLevel = 0;
	pstTeam->iRongYanMonId = 0;
	pstTeam->iRongYanSelectMul = 0;
	pstTeam->iRongYanStatus = 0;
	pstTeam->tRongYanBossSelectTimeEnd = 0;
	pstTeam->iRongYanBegin = 0;//活动结束
	pstTeam->iRongYanOldLevel = 0;
	pstTeam->stRongYanPos.X = 0;
	pstTeam->stRongYanPos.Y = 0;
	pstTeam->stRongYanZoneIdx.iID = 0;
	pstTeam->stRongYanZoneIdx.iPos = 0;

	stRongYanBossInfo.BeginSelectMul = pstTeam->iRongYanSelectMul;
	//-1  通知客户端结束。。。使用户可以继续使用融焰骇心
	stRongYanBossInfo.CurLayer = -1;
	
	for(k = 0;k<iTeamMemberNum;k++)
	{
		pstMemberPlayer =  z_name_player(pstEnv->pstAppCtx, pstEnv,pstTeam->stTeamInfo.TeamMember[k].MemberName);
		if(NULL == pstMemberPlayer)
		{
			continue;
		}

		player_rongyan_boss_ui(pstEnv, pstMemberPlayer,&stRongYanBossInfo);
	}

	return 0;
}

