/*
**  @file $RCSfile: zone_battle.c,v $
**  general description of this module
**  $Id: zone_battle.c,v 1.134 2014/06/22 03:13:59 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/06/22 03:13:59 $
**  @version $Revision: 1.134 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_battle.h"

#include "zone_map.h"
#include "zone_move.h"
#include "mapmask.h"
#include "zone_clt.h"
#include "zone_npc.h"
#include "zone_script.h"
#include "zone_ai.h"
#include "zone_team.h"
#include "zone_db.h"
#include "comm_misc.h"
#include "zone_tmp.h"
#include "zone_package.h"
#include "zone_combat.h"
#include "zone_player.h"
#include "zone_oplog.h"
#include "zone_attr.h"
#include "zone_clan.h"
#include "tconnapi.h"
#include "zone_range.h"
#include "zone_mail.h"
#include "zone_status.h"
#include "zone_diffpworld.h"
#include "zone_spansvr.h"
#include "zone_misc.h"
#include "zone_span.h"
#include "zone_err.h"
#include "zone_zh_name.h"
#include "zone_span_strong.h"

typedef struct
{
	int iMaxQueuer;
	int iQueuer;     
}CampQueuer;

typedef struct
{
	int iCampNum;
	CampQueuer stCampQueuer[MAX_CAMP_NUM];
}CampQueueInfo;

int g_ApplyIng = 0;

int xukong_pworld_get_vcamp(ZONESVRENV * pstEnv, ZONEPWORLDINST *pstPworldInst);

int battle_ploy_range(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, 
				BATTLEDEF *pstBattleDef, Player *pstPlayer);

int is_valid_xukong_battle_award_player(ZONESVRENV* pstEnv, CSBATTLEXUKONGINFO *pstRoleInfo, int iIdx, int iMaxRole);
int is_valid_clan_war_battle_award_player(ZONESVRENV* pstEnv, CSBATTLECLANWARINFO *pstRoleInfo, int iIdx, int iMaxRole);

int z_player_remove_wait_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iWaitIdx);

int invite_player_enter_wait_queue(ZONESVRENV* pstEnv, ZONEPWORLD *pstPworld, PworldApplyQueue* PworldApplyQueue, int iWaitIdx);

int z_player_online_remove_queue(ZONESVRENV* pstEnv, Player *pstPlayer);

int battle_check_apply_single(ZONESVRENV *pstEnv,Player *pstPlayer, ZONEPWORLD *pstZonePworld);

int battle_check_apply_team(ZONESVRENV *pstEnv,Player *pstPlayer, ZONEPWORLD *pstZonePworld, Player** apstPlayer, int *piPlayer);
int battle_check_apply_clan(ZONESVRENV *pstEnv,Player *pstPlayer, ZONEPWORLD *pstZonePworld,
				Player** apstPlayer, int *piPlayer);
int z_player_remove_apply_queue(ZONESVRENV* pstEnv, Player *pstPlayer,
								PworldApplyQueue *pstApplyQueue);

int z_player_remove_clan_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iQeueIdx);
int z_pworld_clan_queue_match_check(ZONESVRENV* pstEnv,ZONEPWORLD *pstPworld,int iQueuerID);
int z_pworld_clan_queue_check(ZONESVRENV* pstEnv, PworldClanQueue *pstClanQueue);
int z_clear_pworld_clan_queue(ZONESVRENV* pstEnv, int iClanQueueIdx, int iPworldID);

BATTLEDEF *z_find_battledef(ZONESVRENV* pstEnv, int iBattleID)
{
	BATTLEDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.BattleID = iBattleID;
	iIndex = bsearch_int(&stDef, pstObj->sBattleDef, pstObj->iBattleDef, BATTLE_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (BATTLEDEF *)&pstObj->sBattleDef[iIndex];
	}
	return NULL;
}

XUKONGPOSDEF *z_find_xukong_pos_def(ZONESVRENV* pstEnv, int iCamp, int iPosIndex)
{
	XUKONGPOSDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Camp = iCamp;
	stDef.PosIndex = iPosIndex;
	iIndex = bsearch_int(&stDef, pstObj->sXuKongPosDef, pstObj->iXuKongPosDef, XUKONG_POS_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (XUKONGPOSDEF *)&pstObj->sXuKongPosDef[iIndex];
	}
	return NULL;
}

BATTLEGLORYDEF *z_find_battle_glory(ZONESVRENV* pstEnv, int iPworldID)
{
	BATTLEGLORYDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.PworldID = iPworldID;
	iIndex = bsearch_int(&stDef, pstObj->sBattleGlory , pstObj->iBattleGlory, BATTLE_GLORY_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (BATTLEGLORYDEF *)&pstObj->sBattleGlory[iIndex];
	}
	return NULL;
}


//是否是一般的战场(BattleDef的都是)
int is_normal_battle_pworld(ZONEPWORLD *pstZonePworld)
{
	if(pstZonePworld->stPworldDef.Type != PWORLD_TYPE_PLOY)
	{
		return 0;
	}

	if(pstZonePworld->stPworldDef.PolyType != PWORLD_POLY_BATTLE)
	{
		return 0;
	}

	return 1;
}

int is_battle_pworld(ZONEPWORLD *pstZonePworld)
{
	if (	pstZonePworld->stPworldDef.Type == PWORLD_TYPE_PLOY &&
		(pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_CAMP_LIFT ||
		pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_RAND_LIFT ||
		pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_SPAN_WORLD ||
		pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_SPAN_STRONG))
	{
		return 1;
	}

	if(is_normal_battle_pworld(pstZonePworld))
	{
		return 1;
	}
		
	return 0;
}

int is_battle_clan_queue(ZONESVRENV* pstEnv, ZONEPWORLD *pstZonePworld)
{
	BATTLEDEF *pstBattleDef;
	
	if(!is_normal_battle_pworld(pstZonePworld))
	{
		return 0;
	}

	pstBattleDef = z_find_battledef(pstEnv, pstZonePworld->stPworldDef.BattleID);
	if(NULL == pstBattleDef)
	{
		return -1;
	}

	if(pstBattleDef->FightType == BATTLE_FIGHT_TYPE_CLAN_WAR)
	{
		return 1;
	}
	
	return 0;
}

int is_battle_team_queue(ZONESVRENV* pstEnv, ZONEPWORLD *pstZonePworld)
{
	BATTLEDEF *pstBattleDef;
	
	if(!is_normal_battle_pworld(pstZonePworld))
	{
		return 0;
	}

	pstBattleDef = z_find_battledef(pstEnv, pstZonePworld->stPworldDef.BattleID);
	if(NULL == pstBattleDef)
	{
		return -1;
	}

	if(pstBattleDef->FightType == BATTLE_FIGHT_TYPE_AOSHAN)
	{
		return 1;
	}
	
	return 0;
}

int is_same_camp_player(Player *pstPlayer1, Player *pstPlayer2)
{
	if(pstPlayer1->stOnline.stWarOL.cCampIdx > 0 && 
		pstPlayer1->stOnline.stWarOL.cCampIdx == pstPlayer2->stOnline.stWarOL.cCampIdx)
	{
		return 1;
	}

	return 0;
}

int is_in_over_battle_pworld(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZONEIDX stMapIdx;
	ZONEMAPINST *pstMapInst = NULL;
	ZONEPWORLDINST *pstPworldInst = NULL;
	ZONEPWORLD *pstPworld;

	stMapIdx.iID = pstPlayer->stRoleData.Map;
	stMapIdx.iPos = pstPlayer->stOnline.iMapPos;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stMapIdx);
	if (NULL == pstMapInst)
	{
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (NULL == pstPworldInst || pstPworldInst->bOver == 0)
	{
		return 0;
	}

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if (NULL == pstPworld || !is_normal_battle_pworld(pstPworld))
	{
		return 0;
	}

	return 1;
}

static int battle_aoshan_info_cmp(CSBATTLEROLEINFO *pInfo1, CSBATTLEROLEINFO *pInfo2)
{
	/*
		1.	绿洲战场：首先按照积分排名，积分排名相同时，
		按照最大杀人数排名，最大杀人数相同时，
		按照助攻数排名，所有数据都相同时先进战场的排在前面
	*/
	//return pInfo2->FightInfo.AoShanInfo.Kill - pInfo1->FightInfo.AoShanInfo.Kill;

	if (pInfo1->FightInfo.AoShanInfo.PolyFraction != pInfo2->FightInfo.AoShanInfo.PolyFraction)
	{
		return pInfo2->FightInfo.AoShanInfo.PolyFraction - pInfo1->FightInfo.AoShanInfo.PolyFraction;
	}
	else
	{
		if (pInfo1->FightInfo.AoShanInfo.Kill != pInfo2->FightInfo.AoShanInfo.Kill)
		{
			return pInfo2->FightInfo.AoShanInfo.Kill - pInfo1->FightInfo.AoShanInfo.Kill;
		}
		else
		{
			if (pInfo2->FightInfo.AoShanInfo.AssistKill != pInfo1->FightInfo.AoShanInfo.AssistKill)
			{
				return pInfo2->FightInfo.AoShanInfo.AssistKill - pInfo1->FightInfo.AoShanInfo.AssistKill;
			}
			else
			{
				return pInfo1->EnterTime - pInfo2->EnterTime;
			}
			
		}
	}
}

static int battle_lift_info_cmp(CSBATTLEROLEINFO *pInfo1, CSBATTLEROLEINFO *pInfo2)
{
	CSBATTLESURVIVALINFO *pstALL1 = &pInfo1->FightInfo.CampLiftInfo; 
	CSBATTLESURVIVALINFO *pstALL2 = &pInfo2->FightInfo.CampLiftInfo;
	
	if (pstALL1->PolyFraction != pstALL2->PolyFraction)
	{
		return pstALL2->PolyFraction - pstALL1->PolyFraction;
	}
	else
	{
		if (pstALL1->MaxKill != pstALL2->MaxKill)
		{
			return pstALL2->MaxKill - pstALL1->MaxKill;
		}
		else
		{
			if (pstALL1->MaxContinueKill != pstALL2->MaxContinueKill)
			{
				return  pstALL2->MaxContinueKill - pstALL1->MaxContinueKill;
			}
			else
			{
				return pInfo1->EnterTime - pInfo2->EnterTime;
			}
		}
	}
	//return 0;
}
static int battle_clan_war_info_cmp(CSBATTLEROLEINFO *pInfo1, CSBATTLEROLEINFO *pInfo2)
{
	CSBATTLECLANWARINFO *pstALL1 = &pInfo1->FightInfo.ClanWarInfo; 
	CSBATTLECLANWARINFO *pstALL2 = &pInfo2->FightInfo.ClanWarInfo;
	if (pstALL1->PolyFraction != pstALL2->PolyFraction)
	{
		return pstALL2->PolyFraction - pstALL1->PolyFraction;
	}
	else
	{
		if (pstALL1->Kill != pstALL2->Kill)
		{
			return pstALL2->Kill - pstALL1->Kill;
		}
		else
		{
			if (pstALL1->MaxContinueKill != pstALL2->MaxContinueKill)
			{
				return  pstALL2->MaxContinueKill - pstALL1->MaxContinueKill;
			}
			else
			{
				return pInfo1->EnterTime - pInfo2->EnterTime;
			}
		}
	}
	//return 0;
}


static int battle_xukong_info_cmp(CSBATTLEROLEINFO *pInfo1, CSBATTLEROLEINFO *pInfo2)
{
	if (pInfo1->FightInfo.XuKongInfo.PolyFraction != pInfo2->FightInfo.XuKongInfo.PolyFraction)
	{
		return pInfo2->FightInfo.XuKongInfo.PolyFraction - pInfo1->FightInfo.XuKongInfo.PolyFraction;
	}
	else
	{
		if (pInfo1->FightInfo.XuKongInfo.Kill != pInfo2->FightInfo.XuKongInfo.Kill)
		{
			return pInfo2->FightInfo.XuKongInfo.Kill - pInfo1->FightInfo.XuKongInfo.Kill;
		}
		else
		{
			if (pInfo2->FightInfo.XuKongInfo.AssistKill != pInfo1->FightInfo.XuKongInfo.AssistKill)
			{
				return pInfo2->FightInfo.XuKongInfo.AssistKill - pInfo1->FightInfo.XuKongInfo.AssistKill;
			}
			else
			{
				return pInfo1->EnterTime - pInfo2->EnterTime;
			}
			
		}
	}
}

/*static int battle_lift_info_cmp(CSBATTLESURVIVALINFO *pstALL1, CSBATTLESURVIVALINFO *pstALL2)
{
	if (pstALL1->PolyFraction != pstALL2->PolyFraction)
	{
		return pstALL2->PolyFraction - pstALL1->PolyFraction;
	}
	else
	{
		if (pstALL1->MaxKill != pstALL2->MaxKill)
		{
			return pstALL2->MaxKill - pstALL1->MaxKill;
		}
		else
		{
			if (pstALL1->MaxContinueKill != pstALL2->MaxContinueKill)
			{
				return  pstALL2->MaxContinueKill - pstALL1->MaxContinueKill;
			}
			else
			{
				return pstALL2->EnterTime - pstALL2->EnterTime;
			}
		}
	}
	//return 0;
}*/

static int battle_info_qsort_cmp(const void *p1, const void *p2)
{
	CSBATTLEROLEINFO *pInfo1 = (CSBATTLEROLEINFO*)p1;
	CSBATTLEROLEINFO *pInfo2 = (CSBATTLEROLEINFO*)p2;

	switch(pInfo1->FightType)
	{
		case BATTLE_FIGHT_TYPE_AOSHAN:
			return  battle_aoshan_info_cmp(pInfo1, pInfo2);
			break;
		case BATTLE_FIGHT_TYPE_CAMP_LIFT:
			return battle_lift_info_cmp(pInfo1, pInfo2);
			break;
		case BATTLE_FIGHT_TYPE_RAND_LIFT:
			return battle_lift_info_cmp(pInfo1, pInfo2);
			break;
		case BATTLE_FIGHT_TYPE_XUKONG:
			return battle_xukong_info_cmp(pInfo1, pInfo2);
			break;
		case BATTLE_FIGHT_TYPE_CLAN_WAR:
			return battle_clan_war_info_cmp(pInfo1, pInfo2);
			break;
		default:
			return -1;
			break;
	}

	return 0;
}

int qsort_normal_battle_info(CSBATTLEINFO *pstBattleInfo)
{
	qsort(&pstBattleInfo->RoleList[0], pstBattleInfo->RoleNum,
			sizeof(pstBattleInfo->RoleList[0]), battle_info_qsort_cmp);
	
	return 0;
}

static int single_lift_battle_polyfraction(Player *pstPlayer)
{
	WarFieldOL *pstOL = &pstPlayer->stOnline.stWarOL;
	double dTmp = pstOL->iKillNum + pstOL->iContinueKill/2.0;
	int iFraction;
	
	if (dTmp <= 0)
	{
		dTmp = 1;
	}

	iFraction = ceil(pstOL->iOre + 
				pstOL->iCrystal *5 + 
				25.0*pstOL->iKillNum*pstOL->iContinueKill/dTmp + 
				pstOL->iAssistKill*4.0);

	return iFraction;		
}

static int single_xukong_battle_polyfraction(Player *pstPlayer)
{
	WarFieldOL *pstOL = &pstPlayer->stOnline.stWarOL;
	double dTmp = pstOL->iKillNum + pstOL->iContinueKill/2.0;
	int iFraction;
	
	if (dTmp <= 0)
	{
		dTmp = 1;
	}

	iFraction = ceil(pstOL->iOre*10 + 
				25.0*pstOL->iKillNum*pstOL->iContinueKill/dTmp + 
				pstOL->iAssistKill*4.0);

	return iFraction;		
}

static int single_clan_war_battle_polyfraction(Player *pstPlayer)
{
	//复用一下，作为绿洲的积分
	return pstPlayer->stOnline.stWarOL.iOre;
}

static int single_aoshan_battle_polyfraction(Player *pstPlayer)
{
	//复用一下，作为绿洲的积分
	return pstPlayer->stOnline.stWarOL.iOre;
}

static int single_aoshan_info(CSBATTLEAOSHANINFO *pstInfo, Player* pstPlayer)
{
	WarFieldOL *pstOL = &pstPlayer->stOnline.stWarOL;
	
	pstInfo->Kill = pstOL->iKillNum;	
	pstInfo->DieNum = pstOL->iDieNum;
	pstInfo->MaxContinueKill = pstOL->iContinueKill;
	pstInfo->AssistKill = pstOL->iAssistKill;
	pstInfo->PolyFraction = single_aoshan_battle_polyfraction(pstPlayer);
	pstInfo->CurrKill = pstOL->iCurrKill;
	
	return 0;
}

static int single_lift_battle_info(CSBATTLESURVIVALINFO *pstInfo, Player* pstPlayer)
{
	WarFieldOL *pstOL = &pstPlayer->stOnline.stWarOL;

	pstInfo->Ore = pstOL->iOre;
	pstInfo->Crystal = pstOL->iCrystal;
	pstInfo->DieNum = pstOL->iDieNum;
	pstInfo->MaxKill = pstOL->iKillNum;
	pstInfo->MaxLife = pstOL->iMaxLife;
	pstInfo->MaxContinueKill = pstOL->iContinueKill;
	pstInfo->AssistKill = pstOL->iAssistKill;
	pstInfo->PolyFraction = single_lift_battle_polyfraction(pstPlayer);
	pstInfo->CurrKill = pstOL->iCurrKill;
	
	return 0;
}
static int single_clan_war_battle_info(CSBATTLECLANWARINFO *pstInfo, Player* pstPlayer)
{
	WarFieldOL *pstOL = &pstPlayer->stOnline.stWarOL;

	pstInfo->Kill = pstOL->iKillNum;
	pstInfo->MaxContinueKill = pstOL->iContinueKill;
	pstInfo->AssistKill = pstOL->iAssistKill;
	pstInfo->PolyFraction = single_clan_war_battle_polyfraction(pstPlayer);
	pstInfo->CurrKill = pstOL->iCurrKill;
	
	return 0;
}


static int single_xukong_battle_info(CSBATTLEXUKONGINFO *pstInfo, Player* pstPlayer)
{
	WarFieldOL *pstOL = &pstPlayer->stOnline.stWarOL;

	pstInfo->Contri = pstOL->iOre;
	pstInfo->DieNum = pstOL->iDieNum;
	pstInfo->Kill = pstOL->iKillNum;
	pstInfo->MaxContinueKill = pstOL->iContinueKill;
	pstInfo->AssistKill = pstOL->iAssistKill;
	pstInfo->PolyFraction = single_xukong_battle_polyfraction(pstPlayer);
	pstInfo->CurrKill = pstOL->iCurrKill;
	
	return 0;
}

//战场信息
int role_battle_fight_info(CSBATTLEROLEINFO*pstInfo, Player* pstPlayer, BATTLEDEF *pstBattleDef)
{
	memset(pstInfo, 0, sizeof(*pstInfo));
	
	pstInfo->CampIdx = pstPlayer->stOnline.stWarOL.cCampIdx;
	pstInfo->Career = pstPlayer->stRoleData.Career;
	pstInfo->MemID = pstPlayer->iMemID;
	pstInfo->FightType = pstBattleDef->FightType;
	pstInfo->EnterTime = pstPlayer->stOnline.stWarOL.tFightTime;
	
	STRNCPY(pstInfo->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstInfo->RoleName));
	pstInfo->ClanName[0] = 0;
	if (pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id > 0)
	{
		STRNCPY(pstInfo->ClanName, pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Name, sizeof(pstInfo->ClanName));
	}

	switch(pstBattleDef->FightType)
	{
		case BATTLE_FIGHT_TYPE_AOSHAN:
			single_aoshan_info(&pstInfo->FightInfo.AoShanInfo, pstPlayer);
			break;
		case BATTLE_FIGHT_TYPE_CAMP_LIFT:
			single_lift_battle_info(&pstInfo->FightInfo.CampLiftInfo, pstPlayer);
			break;
		case BATTLE_FIGHT_TYPE_RAND_LIFT:
			single_lift_battle_info(&pstInfo->FightInfo.RandLiftInfo, pstPlayer);
			break;
		case BATTLE_FIGHT_TYPE_XUKONG:
			single_xukong_battle_info(&pstInfo->FightInfo.XuKongInfo, pstPlayer);
			break;
		case BATTLE_FIGHT_TYPE_CLAN_WAR:
			single_clan_war_battle_info(&pstInfo->FightInfo.ClanWarInfo, pstPlayer);
			break;
		default:
			return -1;
			break;
	}
	
	return 0;
}

//奖章数量= floor(2*个人积分/积分总量*作战时间，1）
int cacl_aoshan_battle_self_award(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, int iFraction)
{
	int iTime = (pstEnv->pstAppCtx->stCurr.tv_sec -pstPworldInst->tStart)/60;
	int iTotalFraction = pstPworldInst->stPworldBattleInfo.iFraction;

	if(iTotalFraction <= 0 ||iTime <= 0)
	{
		return 0;
	}

	return ceil(4.0*iFraction/iTotalFraction*iTime);
}

int cacl_aoshan_battle_award_exp(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, BATTLEDEF *pstBattleDef,
	Player *pstPlayer, CSBATTLEAOSHANINFO *pstInfo, int fWin)
{
	int iSec;
	
		
	pstInfo->Exp = 0;
	pstInfo->FirstWinExp = 0;
	iSec = pstEnv->pstAppCtx->stCurr.tv_sec -pstPlayer->stOnline.stWarOL.tFightTime;
	if(iSec > 0)
	{
		ZONEPWORLD *pstPworld = NULL;
		LEVELUP *pstLevelUp;
		int iTimeOut;

		if(pstPworldInst->bPlayerMeanLevel >= pstEnv->pstZoneObj->iLevelUpDef)
		{
			return 0;
		}

		pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
		if (NULL == pstPworld)
		{
			return 0;
		}

		iTimeOut = pstPworld->stPworldDef.Timeout*60;
		if(iTimeOut <= 0)
		{
			iTimeOut = 3600; 
		}

		if(iSec > iTimeOut+5)
		{
			char szBuff[128];
		
			snprintf(szBuff, CCH(szBuff), "RoleName=%s, RoleID=%llu, iSec =%d, CampIdx =%d, PloyType =%d BattleSecExp Warning", 
				pstPlayer->stRoleData.RoleName, pstPlayer->stRoleData.RoleID, iSec,
				pstPlayer->stOnline.stWarOL.cCampIdx, pstPlayer->stOnline.stWarOL.cPloyType);
			z_game_notify(pstEnv, szBuff, CCH(szBuff));
			
			iSec = pstPworld->stPworldDef.Timeout*60;
		}
		
		pstLevelUp = (LEVELUP *)pstEnv->pstZoneObj->sLevelUpDef[pstPworldInst->bPlayerMeanLevel-1];

		if(fWin)
		{
			pstInfo->Exp = iSec*pstLevelUp->BattleWinExp;
		}
		else
		{
			pstInfo->Exp = iSec*pstLevelUp->BattleFailExp;
		}
	}

	if(1 == battle_limit_win_get(pstPlayer, pstBattleDef->BattleID) && fWin)
	{
		BATTLEGLORYDEF *pstGlory;
		pstGlory = z_find_battle_glory(pstEnv, pstPworldInst->stPworldIdx.iID);
		if (!pstGlory)
			pstInfo->FirstWinExp = 0;
		else
			pstInfo->FirstWinExp = pstGlory->FirstVExp;
	}

	return 0;
}

int cacl_clan_war_battle_player_award_in(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, BATTLEDEF *pstBattleDef,
	Player *pstPlayer, CSBATTLECLANWARINFO *pstInfo, int iSec,CLANWARAWARDDEF* pstClanWarAwardDef,LEVELUP *pstLevelUp)
{
	int i;
	PLAYERAWARDDEF* pstPlayerAward;
	//CLANAWARDDEF* pstClanAward;	//comment by paraunused.pl
	PlayerAwardDefTail* pstPlayerAwardTail;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	pstPlayerAward = z_find_player_award_def(pstEnv,pstClanWarAwardDef->PlayerAwardDefID);
	if (pstPlayerAward)
	{
		pstPlayerAwardTail = z_get_player_award_tail(pstPlayerAward);
		for( i = 0; i < pstPlayerAwardTail->iPosCount; i++)
		{
			int idx = i + pstPlayerAwardTail->iPosStart;
			
			PLAYERAWARDDEF* pstAward = (PLAYERAWARDDEF*)pstObj->sPlayerAwardDef[idx];

			switch (pstAward->Type)
			{
				case PLAYER_AWARD_TYPE_BULL:
					pstInfo->Bull += iSec*pstAward->Value * (1 + pstPworldInst->iAwardMul /100.0);
					break;
				case PLAYER_AWARD_TYPE_EXP:
					pstInfo->Exp += iSec*pstAward->Value * (1 + pstPworldInst->iAwardMul /100.0);
					break;
				case PLAYER_AWARD_TYPE_CLAN_WAR_BRAND:
					//todo:
					break;
				case PLAYER_AWARD_TYPE_LEVEL_EXP:
					pstInfo->Exp += iSec*(pstAward->Value/1000000.0) * pstLevelUp->ContributeExp * 
								(1 + pstPworldInst->iAwardMul /100.0);
					break;
				case PLAYER_AWARD_TYPE_LEVEL_BULL:
					pstInfo->Bull += iSec*(pstAward->Value/1000000.0) * pstLevelUp->ContributeMoney * 
								(1 + pstPworldInst->iAwardMul /100.0);
					break;
					
				default:
					break;
			}
		}
	}
	return 0;

	UNUSED(pstBattleDef);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}
int cacl_clan_war_battle_award_exp(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, BATTLEDEF *pstBattleDef,
	Player *pstPlayer, CSBATTLECLANWARINFO *pstInfo, int fWin,CLANWARAWARDDEF* pstClanWarAwardDef)
{
	int iSec;


	

	pstInfo->Exp = 0;
	pstInfo->Bull = 0;
	//pstInfo->FirstWinExp = 0;
	iSec = pstEnv->pstAppCtx->stCurr.tv_sec -pstPlayer->stOnline.stWarOL.tFightTime;
	if(iSec > 0)
	{
		LEVELUP *pstLevelUp;
		ZONEPWORLD *pstPworld = NULL;
		int iTimeOut;

		//if(pstPworldInst->bPlayerMeanLevel > pstEnv->pstZoneObj->iLevelUpDef)
		if (pstPlayer->stRoleData.Level > pstEnv->pstZoneObj->iLevelUpDef)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "pstPworldInst->bPlayerMeanLevel(%d) >  pstEnv->pstZoneObj->iLevelUpDef(%d)",
				pstPworldInst->bPlayerMeanLevel , pstEnv->pstZoneObj->iLevelUpDef);
			return 0;
		}

		pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
		if (NULL == pstPworld)
		{
			return 0;
		}

		iTimeOut = pstPworld->stPworldDef.Timeout*60;
		if(iTimeOut <= 0)
		{
			iTimeOut = 3600; 
		}
		
		if(iSec > iTimeOut+5)
		{
			char szBuff[128];
		
			snprintf(szBuff, CCH(szBuff), "RoleName=%s, RoleID=%llu, iSec =%d, CampIdx =%d, PloyType =%d BattleSecExp Warning", 
				pstPlayer->stRoleData.RoleName, pstPlayer->stRoleData.RoleID, iSec,
				pstPlayer->stOnline.stWarOL.cCampIdx, pstPlayer->stOnline.stWarOL.cPloyType);
			z_game_notify(pstEnv, szBuff, CCH(szBuff));
			
			iSec = pstPworld->stPworldDef.Timeout*60;
		}
		
		pstLevelUp = (LEVELUP *)pstEnv->pstZoneObj->sLevelUpDef[pstPlayer->stRoleData.Level-1];
		if(fWin)
		{
			cacl_clan_war_battle_player_award_in(pstEnv,pstPworldInst,pstBattleDef,
						pstPlayer,pstInfo,iSec,pstClanWarAwardDef,pstLevelUp);
		}
		else
		{
			cacl_clan_war_battle_player_award_in(pstEnv,pstPworldInst,pstBattleDef,
				pstPlayer,pstInfo,iSec,pstClanWarAwardDef,pstLevelUp);
		}
	}


	if(	1 == battle_limit_win_get(pstPlayer, pstBattleDef->BattleID) &&
		fWin)
	{
/*
		BATTLEGLORYDEF *pstGlory;
		pstGlory = z_find_battle_glory(pstEnv, pstPworldInst->stPworldIdx.iID);
		if (!pstGlory)
			pstInfo->FirstWinExp = 0;
		else
			pstInfo->FirstWinExp = pstGlory->FirstVExp;

*/
	}


	return 0;
	UNUSED(pstBattleDef);	//add by paraunused.pl
}


int cacl_lift_battle_award_exp(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, BATTLEDEF *pstBattleDef,
	Player *pstPlayer, CSBATTLESURVIVALINFO *pstInfo, int fWin)
{
	int iSec;

	pstInfo->Exp = 0;
	pstInfo->FirstWinExp = 0;
	iSec = pstEnv->pstAppCtx->stCurr.tv_sec -pstPlayer->stOnline.stWarOL.tFightTime;
	if(iSec > 0)
	{
		LEVELUP *pstLevelUp;
		ZONEPWORLD *pstPworld = NULL;
		int iTimeOut;

		if(pstPworldInst->bPlayerMeanLevel >= pstEnv->pstZoneObj->iLevelUpDef)
		{
			return 0;
		}

		pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
		if (NULL == pstPworld)
		{
			return 0;
		}

		iTimeOut = pstPworld->stPworldDef.Timeout*60;
		if(iTimeOut <= 0)
		{
			iTimeOut = 3600; 
		}
		
		if(iSec > iTimeOut+5)
		{
			char szBuff[128];
		
			snprintf(szBuff, CCH(szBuff), "RoleName=%s, RoleID=%llu, iSec =%d, CampIdx =%d, PloyType =%d BattleSecExp Warning", 
				pstPlayer->stRoleData.RoleName, pstPlayer->stRoleData.RoleID, iSec,
				pstPlayer->stOnline.stWarOL.cCampIdx, pstPlayer->stOnline.stWarOL.cPloyType);
			z_game_notify(pstEnv, szBuff, CCH(szBuff));
			
			iSec = pstPworld->stPworldDef.Timeout*60;
		}
		
		pstLevelUp = (LEVELUP *)pstEnv->pstZoneObj->sLevelUpDef[pstPworldInst->bPlayerMeanLevel-1];
		if(fWin)
		{
			pstInfo->Exp = iSec*pstLevelUp->BattleWinExp;
		}
		else
		{
			pstInfo->Exp = iSec*pstLevelUp->BattleFailExp;
		}
	}

	if(	1 == battle_limit_win_get(pstPlayer, pstBattleDef->BattleID) &&
		fWin)
	{
		BATTLEGLORYDEF *pstGlory;
		pstGlory = z_find_battle_glory(pstEnv, pstPworldInst->stPworldIdx.iID);
		if (!pstGlory)
			pstInfo->FirstWinExp = 0;
		else
			pstInfo->FirstWinExp = pstGlory->FirstVExp;
	}

	return 0;
}

int cacl_xukong_battle_award_exp(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, BATTLEDEF *pstBattleDef,
	Player *pstPlayer, CSBATTLEXUKONGINFO *pstInfo, int fWin)
{
	int iSec;
	
		
	pstInfo->Exp = 0;
	pstInfo->FirstWinExp = 0;
	iSec = pstEnv->pstAppCtx->stCurr.tv_sec -pstPlayer->stOnline.stWarOL.tFightTime;
	if(iSec > 0)
	{
		ZONEPWORLD *pstPworld = NULL;
		LEVELUP *pstLevelUp;
		int iTimeOut;

		if(pstPworldInst->bPlayerMeanLevel >= pstEnv->pstZoneObj->iLevelUpDef)
		{
			return 0;
		}

		pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
		if (NULL == pstPworld)
		{
			return 0;
		}

		iTimeOut = pstPworld->stPworldDef.Timeout*60;
		if(iTimeOut <= 0)
		{
			iTimeOut = 3600; 
		}

		if(iSec > iTimeOut+5)
		{
			char szBuff[128];
		
			snprintf(szBuff, CCH(szBuff), "RoleName=%s, RoleID=%llu, iSec =%d, CampIdx =%d, PloyType =%d BattleSecExp Warning", 
				pstPlayer->stRoleData.RoleName, pstPlayer->stRoleData.RoleID, iSec,
				pstPlayer->stOnline.stWarOL.cCampIdx, pstPlayer->stOnline.stWarOL.cPloyType);
			z_game_notify(pstEnv, szBuff, CCH(szBuff));
			
			iSec = pstPworld->stPworldDef.Timeout*60;
		}
		
		pstLevelUp = (LEVELUP *)pstEnv->pstZoneObj->sLevelUpDef[pstPworldInst->bPlayerMeanLevel-1];

		if(fWin)
		{
			pstInfo->Exp = iSec*pstLevelUp->BattleWinExp;
		}
		else
		{
			pstInfo->Exp = iSec*pstLevelUp->BattleFailExp;
		}
	}

	if(1 == battle_limit_win_get(pstPlayer, pstBattleDef->BattleID) && fWin)
	{
		BATTLEGLORYDEF *pstGlory;
		pstGlory = z_find_battle_glory(pstEnv, pstPworldInst->stPworldIdx.iID);
		if (!pstGlory)
			pstInfo->FirstWinExp = 0;
		else
			pstInfo->FirstWinExp = pstGlory->FirstVExp;
	}

	return 0;
}


int battle_get_over_num(Player *pstPlayer, int iBattleID)
{
	int i;

	for (i=0; i<pstPlayer->stRoleData.MiscInfo.LimitBattleNum; i++)
	{
		if (pstPlayer->stRoleData.MiscInfo.LimitBattle[i].BattleID ==iBattleID)
		{
			return pstPlayer->stRoleData.MiscInfo.LimitBattle[i].OverNum;
		}
	}

	return 0;
}

int battle_get_glory_awd(ZONESVRENV* pstEnv,Player *pstPlayer, int iIdx,int iBattleID,
							BATTLEGLORYDEF *pstGlory,int iV, BATTLEAWD *pstBattleAwd)
{
	int iEnterNum;
	int iMul = 10000;
	int iGlory = 0;
	
	//等级修正
	//第一个最小等级和最大等级是否有值作为开关
	if(pstGlory->GloryLevelMul[0].MaxLevel && pstGlory->GloryLevelMul[0].MinLevel)
	{
		int i;
		for (i = 0; i < MAX_GLORY_LEVEL_MUL;i++)
		{
			if (!pstGlory->GloryLevelMul[i].MaxLevel && 
				!pstGlory->GloryLevelMul[i].MinLevel)
			{
				break;
			}
			if (pstPlayer->stRoleData.Level >= pstGlory->GloryLevelMul[i].MinLevel &&
				pstPlayer->stRoleData.Level <= pstGlory->GloryLevelMul[i].MaxLevel)
			{
				iMul = pstGlory->GloryLevelMul[i].Mul;
			}
		}
	}
	
	// 胜利
	if (iV)
	{
		iGlory = pstGlory->VGlory * (iMul/10000.0);
		pstBattleAwd->GloryCount += iGlory;
		pstBattleAwd->VGlory = iGlory;
		pstBattleAwd->FGlory = 0;
	}
	else
	{
		iGlory = pstGlory->FGlory * (iMul/10000.0);
		pstBattleAwd->GloryCount += iGlory;	
		pstBattleAwd->VGlory = 0;
		pstBattleAwd->FGlory = iGlory;
	}

	// 名次
	if (iIdx < POS_GLORY)
	{
		iGlory = pstGlory->PosGlory[iIdx] * (iMul/10000.0);
		pstBattleAwd->GloryCount += iGlory;
		pstBattleAwd->PosGlory = iGlory;
	}
	else
	{
		iGlory = pstGlory->PosGlory[POS_GLORY -1] * (iMul/10000.0);
		
		pstBattleAwd->GloryCount += iGlory;
		pstBattleAwd->PosGlory = iGlory;
	}

	// 场次
	iEnterNum = battle_get_over_num(pstPlayer,iBattleID);
	if (iEnterNum < 1)
	{
		// todo  error log
		iEnterNum = 1;
	}

	pstBattleAwd->FiniNum = iEnterNum;
	if (iEnterNum < NUM_GLORY)
	{
		iGlory = pstGlory->NumGlory[iEnterNum -1]  * (iMul/10000.0);
		pstBattleAwd->GloryCount += iGlory;
		pstBattleAwd->NumGlory = iGlory;
	}
	else
	{
		iGlory = pstGlory->NumGlory[NUM_GLORY -1]  * (iMul/10000.0);
		pstBattleAwd->GloryCount += iGlory;
		pstBattleAwd->NumGlory = iGlory;
	}
	UNUSED(pstEnv);
	return pstBattleAwd->GloryCount;
}

int battle_limit_brand_add(Player *pstPlayer, int iBattleID, int iVal)
{
	int i;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;

	for (i=pstMiscInfo->LimitBattleNum-1; i>=0; i--)
	{	
		if (pstMiscInfo->LimitBattle[i].BattleID == iBattleID)
		{
			pstMiscInfo->LimitBattle[i].BrandNum += iVal;
			return pstMiscInfo->LimitBattle[i].BrandNum;
		}
	}
	
	return 0;
}

int battle_limit_brand_get(Player *pstPlayer, int iBattleID)
{
	int i;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;

	for (i=pstMiscInfo->LimitBattleNum-1; i>=0; i--)
	{	
		if (pstMiscInfo->LimitBattle[i].BattleID == iBattleID)
		{
			return pstMiscInfo->LimitBattle[i].BrandNum;
		}
	}
	
	return 0;
}

int battle_get_brand_awd(Player *pstPlayer,BATTLEDEF *pstBattleDef,BATTLEAWD *pstBattleAwd, int fWin)
{
	int iBrandNum;
	
	pstBattleAwd->TotalBrandVal = iBrandNum = battle_limit_brand_get(pstPlayer,pstBattleDef->BattleID);
	if (iBrandNum >= 3)
	{
		return 0;
	}

	if(fWin)
	{
		pstBattleAwd->BrandType = pstBattleDef->BrandAwd.MoneyType;
		pstBattleAwd->BrandVal = 3 - iBrandNum;
		pstBattleAwd->TotalBrandVal = battle_limit_brand_add(pstPlayer, pstBattleDef->BattleID, pstBattleAwd->BrandVal);
	}
	else
	{
		pstBattleAwd->BrandType = pstBattleDef->BrandAwd.MoneyType;
		pstBattleAwd->BrandVal = 1;
		pstBattleAwd->TotalBrandVal = battle_limit_brand_add(pstPlayer, pstBattleDef->BattleID, 1);
	}

	return 0;
}

int battle_is_win_camp(CSBATTLEINFO *pstBattleInfo, int iIdx)
{
	if (pstBattleInfo->VCamp == pstBattleInfo->RoleList[iIdx].CampIdx)
		return 1;
	return 0;
}

int battle_end_up_time_limit(ZONESVRENV* pstEnv,Player *pstPlayer, int *piEndTime)
{
	LIMITBATTLETIME *pstLimit = &pstPlayer->stRoleData.MiscInfo.LimitBattleTime;

	*piEndTime = 0;
	if (pstLimit->Time == 0)
	{
		pstLimit->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstLimit->Num = 1;
	}
	else
	{
		pstLimit->Num ++;
		// 4小时5次完成战场限制
		if (	pstLimit->Num >= BATTLE_HOURS_ENTER_LIMIT &&
			pstEnv->pstAppCtx->stCurr.tv_sec -pstLimit->Time < BATTLE_TIME_LIMIT *60 )
		{
			*piEndTime = BATTLE_TIME_LIMIT *60 - 
						(pstEnv->pstAppCtx->stCurr.tv_sec -pstLimit->Time);
		}
	}

	player_battle_limit_time_action(pstEnv, pstPlayer);
	
	return 0;
}


// 战场完毕更新完成次数
int battle_end_up_overnum(ZONESVRENV *pstEnv,Player *pstPlayer, int iBattleID)
{
	int i;
	//完成竞技任务
	task_bat_fini_trace(pstEnv, pstPlayer,iBattleID);
	for (i=0; i<pstPlayer->stRoleData.MiscInfo.LimitBattleNum; i++)
	{
		if (pstPlayer->stRoleData.MiscInfo.LimitBattle[i].BattleID ==iBattleID)
		{
			pstPlayer->stRoleData.MiscInfo.LimitBattle[i].OverNum ++;
			return 0;
		}
	}
	return 0;
}


int normal_battle_aoshan_award(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, 
		BATTLEDEF *pstBattleDef, CSBATTLEINFO *pstBattleInfo)
{
	int i;
	Player *pstPworldPlayer;
	CSBATTLEAOSHANINFO *pstInfo;
	OPROLEBATTLEAWARDINFO stOpAwardInfo;
	int iV;
	int iGlory;
	BATTLEGLORYDEF *pstGlory;

	pstGlory = z_find_battle_glory(pstEnv, pstPworldInst->stPworldIdx.iID);
	if (!pstGlory)
	{
		// 返回-1会导致没有奖励 记个日志吧
		tlog_error(pstEnv->pstLogCat, 0, 0, "normal_battle_aoshan_award:z_find_battle_glory fail");
		return -1;
	}

	for(i=0; i<pstBattleInfo->RoleNum; i++)
	{
		pstPworldPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstBattleInfo->RoleList[i].MemID);
		if(!pstPworldPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPworldPlayer->eStatus)
		{
			continue;
		}

		if(pstBattleInfo->RoleList[i].FightType != BATTLE_FIGHT_TYPE_AOSHAN)
		{
			continue;
		}

		pstInfo = &pstBattleInfo->RoleList[i].FightInfo.AoShanInfo;

		// exp
		//胜利方
		if(pstBattleInfo->VCamp == pstBattleInfo->RoleList[i].CampIdx)
		{
			//战场胜利完成任务
			task_player_trace(pstEnv, pstPworldPlayer, TASK_OBJ_LVZHOU_WIN);
			
			pstPworldPlayer->stRoleData.MiscInfo.LvZhouWinNum++;
			battle_limit_win_add(pstPworldPlayer, pstBattleDef->BattleID);
			cacl_aoshan_battle_award_exp(pstEnv, pstPworldInst, pstBattleDef, pstPworldPlayer, pstInfo, 1);
		}
		//失败方
		else
		{
			cacl_aoshan_battle_award_exp(pstEnv, pstPworldInst, pstBattleDef, pstPworldPlayer, pstInfo, 0);
		}

		pworld_player_add_exp(pstEnv, pstPworldPlayer, pstInfo->FirstWinExp+pstInfo->Exp);

		if(is_valid_aoshan_battle_award_player(pstEnv, pstInfo, i, pstBattleInfo->RoleNum))
		{
			battle_end_up_overnum(pstEnv,pstPworldPlayer, pstBattleDef->BattleID);
			
			// glory
			iV = battle_is_win_camp(pstBattleInfo, i);
			iGlory = battle_get_glory_awd(pstEnv,pstPworldPlayer,i,pstBattleDef->BattleID,pstGlory,iV, &pstInfo->BattleAwd);
			if (iGlory > 0)
				package_add_money(pstEnv, pstPworldPlayer, ATTR_ID_BATTLE_GLORY, iGlory);

			// brand
			battle_get_brand_awd(pstPworldPlayer,pstBattleDef, &pstInfo->BattleAwd, iV);
			if (pstInfo->BattleAwd.BrandVal > 0)
				package_add_money(pstEnv, pstPworldPlayer, pstInfo->BattleAwd.BrandType, pstInfo->BattleAwd.BrandVal);
		}

		battle_end_up_time_limit(pstEnv, pstPworldPlayer, &pstBattleInfo->RoleList[i].TimeLimitEnd);


		battle_ploy_range(pstEnv, pstPworldInst, pstBattleDef, pstPworldPlayer);
		
		memset(&stOpAwardInfo, 0, sizeof(stOpAwardInfo));
		stOpAwardInfo.Exp = pstInfo->Exp;
		stOpAwardInfo.FirstVExp = pstInfo->FirstWinExp;
		stOpAwardInfo.BattleAwd = pstInfo->BattleAwd;
		z_role_battle_award_oplog(pstEnv, pstPworldPlayer, pstBattleDef->BattleID, &stOpAwardInfo);
	}

	return 0;
}




int battle_enter_up_time_limit(ZONESVRENV* pstEnv,Player *pstPlayer, int fNotify)
{
	LIMITBATTLETIME *pstLimit = &pstPlayer->stRoleData.MiscInfo.LimitBattleTime;
	if (pstEnv->pstAppCtx->stCurr.tv_sec -pstLimit->Time >= BATTLE_TIME_LIMIT *60)
	{
		pstLimit->Time = 0;
		pstLimit->Num = 0;	
	}
	else
	{
		if (pstLimit->Num >= BATTLE_HOURS_ENTER_LIMIT)
		{
			if(fNotify)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PWORLD110);
			}
			return -1;
		}
	}
	return 0;
}


int normal_battle_lift_award(ZONESVRENV* pstEnv, BATTLEDEF *pstBattleDef, ZONEPWORLDINST *pstPworldInst, 
	CSBATTLEINFO *pstBattleInfo)
{
	int i;
	Player *pstPworldPlayer;
	CSBATTLESURVIVALINFO *pstInfo;
	OPROLEBATTLEAWARDINFO stOpAwardInfo;
	int iV;
	int iGlory;
	BATTLEGLORYDEF *pstGlory;

	pstGlory = z_find_battle_glory(pstEnv, pstPworldInst->stPworldIdx.iID);
	if (!pstGlory)
	{
		// 返回-1会导致没有奖励 记个日志吧
		tlog_error(pstEnv->pstLogCat, 0, 0, "normal_battle_lift_award:z_find_battle_glory fail");
		return -1;
	}

	for(i=0; i<pstBattleInfo->RoleNum; i++)
	{
		pstPworldPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstBattleInfo->RoleList[i].MemID);
		if(!pstPworldPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPworldPlayer->eStatus)
		{
			continue;
		}

		if(pstBattleInfo->RoleList[i].FightType == BATTLE_FIGHT_TYPE_CAMP_LIFT)
		{
			pstInfo = &pstBattleInfo->RoleList[i].FightInfo.CampLiftInfo;
		}
		else if(pstBattleInfo->RoleList[i].FightType == BATTLE_FIGHT_TYPE_RAND_LIFT)
		{
			pstInfo = &pstBattleInfo->RoleList[i].FightInfo.RandLiftInfo;
		}
		else
		{
			continue;
		}

		iV = battle_is_win_camp(pstBattleInfo, i);
		//胜利
		if(iV)
		{
			//战场胜利完成任务
			task_player_trace(pstEnv, pstPworldPlayer, TASK_OBJ_KUANGSHA_WIN);
			
			pstPworldPlayer->stRoleData.MiscInfo.KuangShaWinNum++;
			battle_limit_win_add(pstPworldPlayer, pstBattleDef->BattleID);
		}

		//计算奖励经验
		cacl_lift_battle_award_exp(pstEnv, pstPworldInst, pstBattleDef, pstPworldPlayer, pstInfo, iV);
		pworld_player_add_exp(pstEnv, pstPworldPlayer, pstInfo->FirstWinExp+pstInfo->Exp);

		if(is_valid_lift_battle_award_player(pstEnv, pstInfo, i, pstBattleInfo->RoleNum))
		{
			battle_end_up_overnum(pstEnv,pstPworldPlayer, pstBattleDef->BattleID);
			
			// glory
			iGlory = battle_get_glory_awd(pstEnv,pstPworldPlayer,i,pstBattleDef->BattleID,pstGlory,iV,&pstInfo->BattleAwd);
			if (iGlory > 0)
				package_add_money(pstEnv, pstPworldPlayer, ATTR_ID_BATTLE_GLORY, iGlory);

			// brand
			battle_get_brand_awd(pstPworldPlayer,pstBattleDef, &pstInfo->BattleAwd, iV);
			if (pstInfo->BattleAwd.BrandVal > 0)
				package_add_money(pstEnv, pstPworldPlayer, pstInfo->BattleAwd.BrandType, pstInfo->BattleAwd.BrandVal);
		}
		
		// 4小时5次战场倒计时
		battle_end_up_time_limit(pstEnv, pstPworldPlayer, &pstBattleInfo->RoleList[i].TimeLimitEnd);

		battle_ploy_range(pstEnv, pstPworldInst, pstBattleDef, pstPworldPlayer);
		
		// todo:log
		memset(&stOpAwardInfo, 0, sizeof(stOpAwardInfo));
		stOpAwardInfo.Exp = pstInfo->Exp;
		stOpAwardInfo.FirstVExp = pstInfo->FirstWinExp;
		stOpAwardInfo.BattleAwd = pstInfo->BattleAwd;
		z_role_battle_award_oplog(pstEnv, pstPworldPlayer, pstBattleDef->BattleID, &stOpAwardInfo);
	}

	return 0;
}

int player_clan_war_award(ZONESVRENV* pstEnv,Player *pstPlayer,CSBATTLECLANWARINFO *pstInfo,int iGlory)
{
	CSATTRVAL *pstAttr;
	CLANWARSELFINFO *pstClanWarSelfInfo;


	pstClanWarSelfInfo = &pstPlayer->stRoleData.MiscInfo.ClanWarSelfInfo;
	
	memset(&pstClanWarSelfInfo->AwardInfo,0,sizeof(pstClanWarSelfInfo->AwardInfo));
	

	if (pstInfo->Exp)
	{
		pstAttr = &pstClanWarSelfInfo->AwardInfo.AwardAttr[pstClanWarSelfInfo->AwardInfo.AwardAttrNum];
		pstClanWarSelfInfo->AwardInfo.AwardAttrNum++;
		
		pstAttr->AttrID = ATTR_ID_EXP;
		pstAttr->Value.Exp = pstInfo->Exp;
	}

	if (pstInfo->Bull)
	{

		pstAttr = &pstClanWarSelfInfo->AwardInfo.AwardAttr[pstClanWarSelfInfo->AwardInfo.AwardAttrNum];
		pstClanWarSelfInfo->AwardInfo.AwardAttrNum++;
		
		pstAttr->AttrID = ATTR_ID_BULL;
		pstAttr->Value.Bull = pstInfo->Bull;
	}

	if (pstInfo->BattleAwd.BrandType && pstInfo->BattleAwd.BrandVal)
	{
		pstAttr = &pstClanWarSelfInfo->AwardInfo.AwardAttr[pstClanWarSelfInfo->AwardInfo.AwardAttrNum];
		pstClanWarSelfInfo->AwardInfo.AwardAttrNum++;
		
		pstAttr->AttrID = pstInfo->BattleAwd.BrandType;
		pstAttr->Value.AoShanBrand = pstInfo->BattleAwd.BrandVal;
	}

	if (iGlory)
	{
		pstAttr = &pstClanWarSelfInfo->AwardInfo.AwardAttr[pstClanWarSelfInfo->AwardInfo.AwardAttrNum];
		pstClanWarSelfInfo->AwardInfo.AwardAttrNum++;
	
		pstAttr->AttrID = ATTR_ID_BATTLE_GLORY;
		pstAttr->Value.BattleGlory = iGlory;
	}

	if (pstClanWarSelfInfo->AwardInfo.AwardAttrNum)
	{
		pstClanWarSelfInfo->AwardInfo.Time = pstEnv->pstAppCtx->stCurr.tv_sec;
		player_action_clan_war_award_info(pstEnv,pstPlayer);
	}	
	
	return 0;
}

int clan_battle_award(ZONESVRENV *pstEnv,CLANID* pstID,int iClanWarAwardDefID,int iWin,int iDstID,int iMul)
{
	if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		COROUTINEDATA stData;
		 
		stData.ClanBattleAward.ClanID = *pstID;
		stData.ClanBattleAward.ClanWarAwardDefID = iClanWarAwardDefID;
		stData.ClanBattleAward.Win = iWin;
		stData.ClanBattleAward.Mul = iMul;
		
		return clan_battle_award_co(pstEnv,NULL,&stData,iDstID);
	}
	else
	{
		return clan_battle_award_in(pstEnv,pstID,iClanWarAwardDefID,iWin,iMul);
	}
	
	return 0;
}

int normal_battle_clan_war_award(ZONESVRENV* pstEnv, BATTLEDEF *pstBattleDef, ZONEPWORLDINST *pstPworldInst, 
	CSBATTLEINFO *pstBattleInfo)
{
	int i;
	Player *pstPworldPlayer;
	CSBATTLECLANWARINFO *pstInfo = NULL;
	OPROLEBATTLEAWARDINFO stOpAwardInfo;
	int iV;
	int iGlory = 0;	
	BATTLEGLORYDEF *pstGlory;
	CLANWARAWARDDEF* pstClanWarAwardDef = NULL;
	int aiPlayerNum[MAX_CAMP_NUM];	

	

	pstGlory = z_find_battle_glory(pstEnv, pstPworldInst->stPworldIdx.iID);
	if (!pstGlory)
	{
		// 返回-1会导致没有奖励 记个日志吧
		tlog_error(pstEnv->pstLogCat, 0, 0, "normal_battle_clan_war_award:z_find_battle_glory fail");
		return -1;
	}

	memset(aiPlayerNum,0,sizeof(aiPlayerNum));

	for(i=0; i<pstBattleInfo->RoleNum; i++)
	{
		pstPworldPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstBattleInfo->RoleList[i].MemID);
		if(!pstPworldPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPworldPlayer->eStatus)
		{
			continue;
		}

		if(pstBattleInfo->RoleList[i].FightType == BATTLE_FIGHT_TYPE_CLAN_WAR)
		{
			pstInfo = &pstBattleInfo->RoleList[i].FightInfo.ClanWarInfo;
		}
		else
		{
			continue;
		}

		if (!pstInfo)
		{
			continue;
		}


		iV = battle_is_win_camp(pstBattleInfo, i);
		//胜利
		if(iV)
		{
			//战场胜利完成任务
			//task_player_trace(pstEnv, pstPworldPlayer, TASK_OBJ_KUANGSHA_WIN);
			
			//pstPworldPlayer->stRoleData.MiscInfo.KuangShaWinNum++;
			//battle_limit_win_add(pstPworldPlayer, pstBattleDef->BattleID);
			pstClanWarAwardDef = z_find_clan_war_award_def(pstEnv,pstGlory->ClanWarAwardDefID[0]);
		}
		else
		{
			pstClanWarAwardDef = z_find_clan_war_award_def(pstEnv,pstGlory->ClanWarAwardDefID[1]);
		}

		if (!pstClanWarAwardDef)
		{
			continue;
		}
		//todo:如果奖励礼包中有东西,发邮件
		//player_clan_war_award_mail(pstEnv,pstPworldPlayer);
		player_take_clan_war_award_in(pstEnv,pstPworldPlayer);

		//计算奖励经验
		cacl_clan_war_battle_award_exp(pstEnv, pstPworldInst, pstBattleDef, pstPworldPlayer, pstInfo, iV,pstClanWarAwardDef);

		if(is_valid_clan_war_battle_award_player(pstEnv, pstInfo, i, pstBattleInfo->RoleNum))
		{
			battle_end_up_overnum(pstEnv,pstPworldPlayer, pstBattleDef->BattleID);
			
			// glory
			iGlory = battle_get_glory_awd(pstEnv,pstPworldPlayer,i,pstBattleDef->BattleID,pstGlory,iV,&pstInfo->BattleAwd);
			if (iGlory > 0)
			{
			 	//package_add_money(pstEnv, pstPworldPlayer, ATTR_ID_BATTLE_GLORY, iGlory);
			}

			// brand
			battle_get_brand_awd(pstPworldPlayer,pstBattleDef, &pstInfo->BattleAwd, iV);
			if (pstInfo->BattleAwd.BrandVal > 0)
			{
				
				//package_add_money(pstEnv, pstPworldPlayer, pstInfo->BattleAwd.BrandType, pstInfo->BattleAwd.BrandVal);
			}
		}

		if (pstBattleInfo->RoleList[i].CampIdx >0 &&
			pstBattleInfo->RoleList[i].CampIdx <= MAX_CAMP_NUM)
		{
			aiPlayerNum[pstBattleInfo->RoleList[i].CampIdx-1]++;
		}
		
		

		//放到奖励背包中
		player_clan_war_award(pstEnv,pstPworldPlayer,pstInfo,iGlory);

	
		// 4小时5次战场倒计时
		//battle_end_up_time_limit(pstEnv, pstPworldPlayer, &pstBattleInfo->RoleList[i].TimeLimitEnd);

		//todo:排行榜
		//battle_ploy_range(pstEnv, pstPworldInst, pstBattleDef, pstPworldPlayer);
		
		// todo:log
		memset(&stOpAwardInfo, 0, sizeof(stOpAwardInfo));
		stOpAwardInfo.Exp = pstInfo->Exp;
		stOpAwardInfo.FirstVExp = 0;//pstInfo->FirstWinExp;
		stOpAwardInfo.Bull = pstInfo->Bull;
		stOpAwardInfo.BattleAwd = pstInfo->BattleAwd;
		z_role_battle_award_oplog(pstEnv, pstPworldPlayer, pstBattleDef->BattleID, &stOpAwardInfo);
	}

	//公会奖励
	for (i = 0; i < pstPworldInst->stPworldCamp.iCampNum; i++)
	{
		PworldCampPlayerInfo *pstCampInfo = &pstPworldInst->stPworldCamp.stCampInfo[i];
		CLANID *pstID = &pstCampInfo->stClanID;
		int iClanAwardDefID = 0;
		int iWin = 0;
		if (aiPlayerNum[i] <= 0  || pstCampInfo->iPlayerNum < 0)
		{
			continue;//结束时没人在,不给奖励
		}

		if (pstBattleInfo->VCamp == i+1)
		{
			iClanAwardDefID = pstGlory->ClanWarAwardDefID[0];
			iWin = 1;
		}
		else
		{
			iClanAwardDefID = pstGlory->ClanWarAwardDefID[1];//默认给第2名的
			if (pstCampInfo->stMapShowMon.iRangePos && 
				pstCampInfo->stMapShowMon.iRangePos < MAX_CAMP_NUM)
			{
				iClanAwardDefID = pstGlory->ClanWarAwardDefID[pstCampInfo->stMapShowMon.iRangePos];
			}
		}

		
		pstClanWarAwardDef = z_find_clan_war_award_def(pstEnv,iClanAwardDefID);

		if (!pstClanWarAwardDef)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "z_find_clan_war_award_def fail id:%d",iClanAwardDefID);

			continue;			
		}

		clan_battle_award(pstEnv,pstID,iClanAwardDefID,iWin,pstPworldInst->stPworldCamp.stCampInfo[i].iSvrBusID,
							 pstPworldInst->iAwardMul);
	}
	
	return 0;
}
int normal_battle_xukong_award(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, 
		BATTLEDEF *pstBattleDef, CSBATTLEINFO *pstBattleInfo)
{
	int i;
	Player *pstPworldPlayer;
	CSBATTLEXUKONGINFO *pstInfo;
	OPROLEBATTLEAWARDINFO stOpAwardInfo;
	int iV;
	int iGlory;
	BATTLEGLORYDEF *pstGlory;

	pstGlory = z_find_battle_glory(pstEnv, pstPworldInst->stPworldIdx.iID);
	if (!pstGlory)
	{
		// 返回-1会导致没有奖励 记个日志吧
		tlog_error(pstEnv->pstLogCat, 0, 0, "normal_battle_aoshan_award:z_find_battle_glory fail");
		return -1;
	}

	for(i=0; i<pstBattleInfo->RoleNum; i++)
	{
		pstPworldPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstBattleInfo->RoleList[i].MemID);
		if(!pstPworldPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPworldPlayer->eStatus)
		{
			continue;
		}

		if(pstBattleInfo->RoleList[i].FightType != BATTLE_FIGHT_TYPE_XUKONG)
		{
			continue;
		}

		pstInfo = &pstBattleInfo->RoleList[i].FightInfo.XuKongInfo;

		// exp
		//胜利方
		if(pstBattleInfo->VCamp == pstBattleInfo->RoleList[i].CampIdx)
		{
			//战场胜利完成任务
			//task_player_trace(pstEnv, pstPworldPlayer, TASK_OBJ_LVZHOU_WIN);
			
			//pstPworldPlayer->stRoleData.MiscInfo.LvZhouWinNum++;
			
			battle_limit_win_add(pstPworldPlayer, pstBattleDef->BattleID);
			cacl_xukong_battle_award_exp(pstEnv, pstPworldInst, pstBattleDef, pstPworldPlayer, pstInfo, 1);
		}
		//失败方
		else
		{
			cacl_xukong_battle_award_exp(pstEnv, pstPworldInst, pstBattleDef, pstPworldPlayer, pstInfo, 0);
		}

		pworld_player_add_exp(pstEnv, pstPworldPlayer, pstInfo->FirstWinExp+pstInfo->Exp);

		if(is_valid_xukong_battle_award_player(pstEnv, pstInfo, i, pstBattleInfo->RoleNum))
		{
			battle_end_up_overnum(pstEnv,pstPworldPlayer, pstBattleDef->BattleID);
			
			// glory
			iV = battle_is_win_camp(pstBattleInfo, i);
			iGlory = battle_get_glory_awd(pstEnv,pstPworldPlayer,i,pstBattleDef->BattleID,pstGlory,iV, &pstInfo->BattleAwd);
			if (iGlory > 0)
				package_add_money(pstEnv, pstPworldPlayer, ATTR_ID_BATTLE_GLORY, iGlory);

			// brand
			battle_get_brand_awd(pstPworldPlayer,pstBattleDef, &pstInfo->BattleAwd, iV);
			if (pstInfo->BattleAwd.BrandVal > 0)
				package_add_money(pstEnv, pstPworldPlayer, pstInfo->BattleAwd.BrandType, pstInfo->BattleAwd.BrandVal);
		}

		battle_end_up_time_limit(pstEnv, pstPworldPlayer, &pstBattleInfo->RoleList[i].TimeLimitEnd);


		battle_ploy_range(pstEnv, pstPworldInst, pstBattleDef, pstPworldPlayer);
		
		memset(&stOpAwardInfo, 0, sizeof(stOpAwardInfo));
		stOpAwardInfo.Exp = pstInfo->Exp;
		stOpAwardInfo.FirstVExp = pstInfo->FirstWinExp;
		stOpAwardInfo.BattleAwd = pstInfo->BattleAwd;
		z_role_battle_award_oplog(pstEnv, pstPworldPlayer, pstBattleDef->BattleID, &stOpAwardInfo);
	}

	return 0;
}


//战场奖励
int normal_battle_award(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, BATTLEDEF *pstBattleDef,
			CSBATTLEINFO *pstBattleInfo)
{
	//战场奖励日志流水号
	z_oplog_event_begin(pstEnv);
	
	switch(pstBattleDef->FightType)
	{
		case BATTLE_FIGHT_TYPE_AOSHAN:
			{
				normal_battle_aoshan_award(pstEnv, pstPworldInst, pstBattleDef, pstBattleInfo);
			}
			break;
		
		case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		//case BATTLE_FIGHT_TYPE_RAND_LIFT:
			{
				normal_battle_lift_award(pstEnv, pstBattleDef, pstPworldInst, pstBattleInfo);
			}
			break;
		case BATTLE_FIGHT_TYPE_XUKONG:
			normal_battle_xukong_award(pstEnv, pstPworldInst, pstBattleDef, pstBattleInfo);
			break;
		case BATTLE_FIGHT_TYPE_CLAN_WAR:
			{
				normal_battle_clan_war_award(pstEnv, pstBattleDef, pstPworldInst, pstBattleInfo);
			}
			break;
		default:
			break;
	}
	
	return 0;
}

int battle_lift_get_wincamp(CSBATTLEINFO *pstBattleInfo)
{
	int i;
	int v1=0,v2=0,v3=0;
	int iMax;

	for (i=0; i<pstBattleInfo->RoleNum; i++)
	{
		if (pstBattleInfo->RoleList[i].CampIdx ==1)
		{
			v1 += pstBattleInfo->RoleList[i].FightInfo.CampLiftInfo.PolyFraction;
		}
		else if (pstBattleInfo->RoleList[i].CampIdx ==2)
		{
			v2 += pstBattleInfo->RoleList[i].FightInfo.CampLiftInfo.PolyFraction;
		}
		else if (pstBattleInfo->RoleList[i].CampIdx ==3)
		{
			v3 += pstBattleInfo->RoleList[i].FightInfo.CampLiftInfo.PolyFraction;
		}
	}

	iMax = max(v1,max(v2,v3));
	if (iMax == v1)
		return 1;
	else if (iMax == v2)
		return 2;
	else
		return 3;
}

int battle_clan_war_get_wincamp(ZONESVRENV* pstEnv,ZONEPWORLDINST	 *pstPworldInst)
{
	//int i;
	int v1=0,v2=0,v3=0;
	int iMax;
	//int iMaxHP = 0;
	Monster* pstMon;

	if (pstPworldInst->stPworldCamp.iCampNum > 3 ||
		pstPworldInst->stPworldCamp.iCampNum <= 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "stPworldCamp.iCampNum(%d) err",
					pstPworldInst->stPworldCamp.iCampNum);

		return 0;
	}

	v1 = pstPworldInst->stPworldCamp.stCampInfo[0].stMapShowMon.iHP;
	v2 = pstPworldInst->stPworldCamp.stCampInfo[1].stMapShowMon.iHP;
	v3 = pstPworldInst->stPworldCamp.stCampInfo[2].stMapShowMon.iHP;

	pstMon = z_id_monster(pstEnv->pstAppCtx,pstEnv,pstPworldInst->stPworldCamp.stCampInfo[0].stMapShowMon.iMemID);
	if (pstMon && pstMon->iDefIdx == pstPworldInst->stPworldCamp.stCampInfo[0].stMapShowMon.iDefID)
	{
		v1 = pstMon->iHP;
	}

	pstMon = z_id_monster(pstEnv->pstAppCtx,pstEnv,pstPworldInst->stPworldCamp.stCampInfo[1].stMapShowMon.iMemID);
	if (pstMon && pstMon->iDefIdx == pstPworldInst->stPworldCamp.stCampInfo[1].stMapShowMon.iDefID)
	{
		v2 = pstMon->iHP;
	}

	pstMon = z_id_monster(pstEnv->pstAppCtx,pstEnv,pstPworldInst->stPworldCamp.stCampInfo[2].stMapShowMon.iMemID);
	if (pstMon && pstMon->iDefIdx == pstPworldInst->stPworldCamp.stCampInfo[2].stMapShowMon.iDefID)
	{
		v3 = pstMon->iHP;
	}

	iMax = max(v1,max(v2,v3));
	if (iMax == v1)
	{
		if (v2 >= v3)
		{
			pstPworldInst->stPworldCamp.stCampInfo[1].stMapShowMon.iRangePos = 1;
			pstPworldInst->stPworldCamp.stCampInfo[2].stMapShowMon.iRangePos = 2;
		}
		else
		{
			pstPworldInst->stPworldCamp.stCampInfo[1].stMapShowMon.iRangePos = 2;
			pstPworldInst->stPworldCamp.stCampInfo[2].stMapShowMon.iRangePos = 1;
		}
		return 1;
	}
	else if (iMax == v2)
	{
		if (v1 >= v3)
		{
			pstPworldInst->stPworldCamp.stCampInfo[0].stMapShowMon.iRangePos = 1;
			pstPworldInst->stPworldCamp.stCampInfo[2].stMapShowMon.iRangePos = 2;
		}
		else
		{
			pstPworldInst->stPworldCamp.stCampInfo[0].stMapShowMon.iRangePos = 2;
			pstPworldInst->stPworldCamp.stCampInfo[2].stMapShowMon.iRangePos = 1;
		}
		
		return 2;
	}
	else
	{
		if (v1 >= v2)
		{
			pstPworldInst->stPworldCamp.stCampInfo[0].stMapShowMon.iRangePos = 1;
			pstPworldInst->stPworldCamp.stCampInfo[1].stMapShowMon.iRangePos = 2;
		}
		else
		{
			pstPworldInst->stPworldCamp.stCampInfo[0].stMapShowMon.iRangePos = 2;
			pstPworldInst->stPworldCamp.stCampInfo[1].stMapShowMon.iRangePos = 1;
		}
		return 3;
	}
}

int battle_camp_info( ZONEPWORLDINST *pstPworldInst, CSBATTLEINFO *pstBattleInfo)
{
	int iCamp = 0;
	
	PworldCampPlayerInfo *pstCampInfo;

	pstBattleInfo->CampNum = 0;
	for (iCamp = 0; iCamp < pstPworldInst->stPworldCamp.iCampNum; iCamp++)
	{
		if (iCamp >= MAX_CAMP_NUM)
		{
			break;
		}
		
		pstCampInfo = &pstPworldInst->stPworldCamp.stCampInfo[iCamp];
		pstBattleInfo->CampInfo[iCamp].CampID = iCamp + 1;
		pstBattleInfo->CampInfo[iCamp].CampVal = pstCampInfo->CampVal;
		STRNCPY(pstBattleInfo->CampInfo[iCamp].CampName,pstCampInfo->stClanID.Name,
				sizeof(pstBattleInfo->CampInfo[iCamp].CampName));
		pstBattleInfo->CampNum++;
		
	}
			
	return 0;
}

int battle_all_info_update(ZONESVRENV* pstEnv, ZONEPWORLDINST	 *pstPworldInst,	
	BATTLEDEF *pstBattleDef, int iVCamp, int fEnd)
{
	CSPKG stPkg;
	CSBATTLESVR *pstBattleSvr = &stPkg.Body.BattleSvr;
	int i, iPlayer = 0;
	Player *pstPworldPlayer;
	Player *apstPlayer[MAX_PWORLD_MEMBER];
	
	if (!pstPworldInst)
	{
		return -1;
	}
		
	pstBattleSvr->SvrData.BattleInfo.RoleNum = 0;
	for (i=0; i<pstPworldInst->iPlayer; i++)
	{
		pstPworldPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i]);
		if (!pstPworldPlayer)
			continue;
		
		if (0 > role_battle_fight_info(&pstBattleSvr->SvrData.BattleInfo.RoleList[pstBattleSvr->SvrData.BattleInfo.RoleNum++], 
							pstPworldPlayer, pstBattleDef))
		{
			continue;
		}

		apstPlayer[iPlayer++] = pstPworldPlayer;

		//结束的时候更新下排行榜信息
		if(fEnd)
		{	
			//battle_ploy_range(pstEnv, pstPworldInst, pstBattleDef, pstPworldPlayer);

			z_player_clean_badbuff(pstEnv, pstPworldPlayer);
		}

		if(pstBattleSvr->SvrData.BattleInfo.RoleNum >= 0xff)
		{
			break;
		}
	}

	if(fEnd)
	{
		//	胜利方阵营
		// 	狂沙按积分算输赢
		if (pstBattleDef->FightType == BATTLE_FIGHT_TYPE_CAMP_LIFT)
		{
			iVCamp = battle_lift_get_wincamp(&pstBattleSvr->SvrData.BattleInfo);
		}
		else if(pstBattleDef->FightType == BATTLE_FIGHT_TYPE_XUKONG)
		{
			if(iVCamp == 0)
			{
				iVCamp = xukong_pworld_get_vcamp(pstEnv, pstPworldInst);
			}
		}
		else if (pstBattleDef->FightType == BATTLE_FIGHT_TYPE_CLAN_WAR)
		{
			if (iVCamp == 0)
			{
				iVCamp = battle_clan_war_get_wincamp(pstEnv,pstPworldInst);
			}
		}
		
		pstBattleSvr->SvrData.BattleInfo.VCamp = iVCamp;
		
		//排序
		qsort_normal_battle_info(&pstBattleSvr->SvrData.BattleInfo);

		//奖励
		normal_battle_award(pstEnv, pstPworldInst, pstBattleDef, &pstBattleSvr->SvrData.BattleInfo);
		
		pstBattleSvr->SvrData.BattleInfo.OpType = BATTLE_OP_TYPE_END;
	}
	else
	{
		pstBattleSvr->SvrData.BattleInfo.VCamp = 0;
		pstBattleSvr->SvrData.BattleInfo.OpType = BATTLE_OP_TYPE_UPDATE;
	}

	pstBattleSvr->SvrData.BattleInfo.XuKongVal1 = pstPworldInst->stPworldBattleInfo.stXuKongInfo.iCampVal1;
	pstBattleSvr->SvrData.BattleInfo.XuKongVal2 = pstPworldInst->stPworldBattleInfo.stXuKongInfo.iCampVal2;
	//阵营信息
	battle_camp_info(pstPworldInst,&pstBattleSvr->SvrData.BattleInfo);
	
	pstBattleSvr->SvrType = BATTLE_SVR_INFO;
	Z_CSHEAD_INIT(&stPkg.Head, BATTLE_SVR);
	z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer,iPlayer, &stPkg);

	return 0;
}

// iFlag==0 自己    1--所有人未排序信息    2--结束且所有人排序信息
// Vcamp表示胜利方正营
int normal_battle_svr_info(ZONESVRENV* pstEnv,Player* pstPlayer, ZONEPWORLD *pstPworld,
							ZONEPWORLDINST	 *pstPworldInst,	int iFlag, int iVCamp)
{
	CSPKG stPkg;
	CSBATTLESVR *pstBattleSvr = &stPkg.Body.BattleSvr;
	BATTLEDEF *pstBattleDef = z_find_battledef(pstEnv, pstPworld->stPworldDef.BattleID);

	if (!pstBattleDef)
		return -1;

	switch(iFlag)
	{
		//自己
		case 0:
			{
				if (!pstPlayer)
					return -1;

				pstBattleSvr->SvrType = BATTLE_SVR_INFO;
				pstBattleSvr->SvrData.BattleInfo.OpType = BATTLE_OP_TYPE_SELF;
				pstBattleSvr->SvrData.BattleInfo.RoleNum = 1;
				pstBattleSvr->SvrData.BattleInfo.VCamp = iVCamp;
				pstBattleSvr->SvrData.BattleInfo.XuKongVal1 = pstPworldInst->stPworldBattleInfo.stXuKongInfo.iCampVal1;
				pstBattleSvr->SvrData.BattleInfo.XuKongVal2 = pstPworldInst->stPworldBattleInfo.stXuKongInfo.iCampVal2;
				//阵营信息
				battle_camp_info(pstPworldInst,&pstBattleSvr->SvrData.BattleInfo);
				
				if( 0 > role_battle_fight_info(&pstBattleSvr->SvrData.BattleInfo.RoleList[0], pstPlayer, pstBattleDef))
				{
					return -1;
				}
				
				Z_CSHEAD_INIT(&stPkg.Head, BATTLE_SVR);
				z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
			}
			break;
		//全量更新
		case 1:
			battle_all_info_update(pstEnv, pstPworldInst, pstBattleDef, 0, 0);
			break;
		//结束
		case 2:
			battle_all_info_update(pstEnv, pstPworldInst, pstBattleDef, iVCamp, 1);
			break;
		case 3:
			{
				ZONEMAPINST * pstMapInst;

				pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPworldInst->astMapInstIdx[0]);
				if(NULL == pstMapInst)
				{
					return -1;
				}
				
				if(pstBattleDef->FightType != BATTLE_FIGHT_TYPE_XUKONG &&
					pstBattleDef->FightType != BATTLE_FIGHT_TYPE_CLAN_WAR)
				{
					return -1;
				}
				
				pstBattleSvr->SvrType = BATTLE_SVR_INFO;
				pstBattleSvr->SvrData.BattleInfo.OpType = BATTLE_OP_TYPE_UPDATE_ENERGY;
				pstBattleSvr->SvrData.BattleInfo.RoleNum = 0;
				pstBattleSvr->SvrData.BattleInfo.VCamp = 0;
				pstBattleSvr->SvrData.BattleInfo.XuKongVal1 = pstPworldInst->stPworldBattleInfo.stXuKongInfo.iCampVal1;
				pstBattleSvr->SvrData.BattleInfo.XuKongVal2 = pstPworldInst->stPworldBattleInfo.stXuKongInfo.iCampVal2;
				//阵营信息
				battle_camp_info(pstPworldInst,&pstBattleSvr->SvrData.BattleInfo);

				Z_CSHEAD_INIT(&stPkg.Head, BATTLE_SVR);
				pworld_map_broadcast(pstEnv->pstAppCtx, pstEnv, pstPworldInst, pstMapInst, 0, &stPkg);
			}
			break;
		default:
			break;
	}
	
	return 0;
}

//逃跑BUFF
int z_add_runaway_buff(ZONESVRENV* pstEnv, Player* pstPlayer)
{
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;
	ZONEPWORLD *pstPworld;
	ZONEPWORLDINST *pstPworldInst;
	BATTLEDEF *pstBattleDef;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if( !pstMapInst )
		return -1;
	
	//副本没结束就算逃跑
	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (pstPworldInst == NULL)
	{
		return -1;
	}

	if(pstPworldInst->tEnd && pstPworldInst->tEnd < pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return 0;
	}

	if(pstPworldInst->bOver)
	{
		return 0;
	}

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if (pstPworld == NULL || !is_battle_pworld(pstPworld))
	{
		return -1;
	}

	//如果是跨服公会战不加逃跑debuf
	pstBattleDef = z_find_battledef(pstEnv, pstPworld->stPworldDef.BattleID);	
	if(pstBattleDef && pstBattleDef->FightType == BATTLE_FIGHT_TYPE_CLAN_WAR)
	{
		return 0;
	}

	z_player_apply_status( pstEnv->pstAppCtx, pstEnv, pstPlayer, 
				STATUS_ID_RUNAWAY, 1, RUNAWAY_DEBUFF_TIME, 1, 0);

	return 0;
	
}

int z_get_free_wait_queue_idx(ZONESVRENV* pstEnv)
{
	int i;
	PworldWaitQueue *pstWaitQueue;
	WorldPworldQueue *pstPworldQueue = pstEnv->pstPworldQueue;
	
	for(i = 0; i<MAX_PWORLD_WAIT_QUEUE; i++)
	{
		pstWaitQueue = &pstPworldQueue->stWaitQueue[i];

		if(pstWaitQueue->iPworldInstID == 0)
		{
			return i;
		}
	}
	
	return -1;
}

int z_get_free_clan_queue_idx(ZONESVRENV* pstEnv)
{
	int i;
	PworldClanQueue *pstClanQueue;
	WorldPworldQueue *pstPworldQueue = pstEnv->pstPworldQueue;
	
	for(i = 0; i<MAX_PWORLD_CLAN_QUEUE; i++)
	{
		pstClanQueue = &pstPworldQueue->stClanQueue[i];

		if(pstClanQueue->iPworldID == 0)
		{
			return i;
		}
	}

	return -1;
}

//获得指定副本公会的的报名队列
PworldClanQueue *get_pworld_clan_queue(ZONESVRENV* pstEnv, int iPworldID,
						tdr_ulonglong ullClanGID,PworldApplyQueue *pstApplyQueue)
{
	int i;
	PworldClanQueue *pstClanQueue;
	WorldPworldQueue *pstPworldQueue = pstEnv->pstPworldQueue;

	//副本公会的排队队列
	for(i = 0; i<MAX_PWORLD_CLAN_QUEUE; i++)
	{
		pstClanQueue = &pstPworldQueue->stClanQueue[i];

		if(pstClanQueue->iPworldID == 0)
		{
			continue;
		}

		if(pstClanQueue->stClanID.GID == ullClanGID)
		{
			int iIdx = 0;
			//some thing wrong
			if (pstClanQueue->iPworldID != iPworldID)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "pstClanQueue->iPworldID(%d) != iPworldID(%d)",
					pstClanQueue->iPworldID , iPworldID);

				//clear 掉
				z_clear_pworld_clan_queue(pstEnv,i,pstClanQueue->iPworldID);
				return NULL;
			}

			if (pstClanQueue->iQueueType == PWORLD_APPLY_QUEUE)
			{
				iIdx = pstApplyQueue - pstPworldQueue->stApplyQueue;

				if (iIdx != pstClanQueue->iQueueIdx)
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "pstClanQueue->iApplyQueueIdx(%d) != (%d)",
						pstClanQueue->iQueueIdx , iIdx);

					//clear 掉
					z_clear_pworld_clan_queue(pstEnv,i,pstClanQueue->iPworldID);
					return NULL;
				}
			}
			
			return pstClanQueue;
		}
	}
	
	return NULL;
}

PworldClanQueue *new_pworld_clan_queue(ZONESVRENV* pstEnv, int iPworldID,
										CLANID *pstClanID,PworldApplyQueue *pstApplyQueue,
										int iBusID,int iBattleID)
{
	int iQueueIdx;
	int i;
	PworldClanQueue *pstClanQueue;
	WorldPworldQueue *pstPworldQueue = pstEnv->pstPworldQueue;
	int iIdx = 0;
	
	iQueueIdx = z_get_free_clan_queue_idx(pstEnv);
	if(0 > iQueueIdx )
	{
		return NULL;
	}

	iIdx = pstApplyQueue - pstPworldQueue->stApplyQueue;
	
	for(i = 0; i<iQueueIdx; i++)
	{
		pstClanQueue = &pstPworldQueue->stClanQueue[i];

		if(pstClanQueue->stClanID.GID == pstClanID->GID)
		{
			//已经有个公会对应的报名队列,但是pworldid 或idx不同
			
			
			tlog_error(pstEnv->pstLogCat, 0, 0, 
					"pstClanQueue->iPworldID=%d pstClanQueue->iQueueIdx=%d "
					"iPworldID=%d,ClanGID=%ull,iIdx=%d,iQueueType=%d",
					pstClanQueue->iPworldID ,pstClanQueue->iQueueIdx,
					iPworldID,pstClanID->GID,iIdx,pstClanQueue->iQueueType);
			
			return NULL;
		}
	}

	memset(&pstPworldQueue->stClanQueue[iQueueIdx],0,sizeof(pstPworldQueue->stClanQueue[iQueueIdx]));

	pstPworldQueue->stClanQueue[iQueueIdx].iPworldID = iPworldID;
	pstPworldQueue->stClanQueue[iQueueIdx].iQueuerNum = 0;
	pstPworldQueue->stClanQueue[iQueueIdx].iPworldFlag = 0;
	pstPworldQueue->stClanQueue[iQueueIdx].stClanID = *pstClanID;
	pstPworldQueue->stClanQueue[iQueueIdx].iQueueType = PWORLD_APPLY_QUEUE;
	pstPworldQueue->stClanQueue[iQueueIdx].iQueueIdx = iIdx;
	pstPworldQueue->stClanQueue[iQueueIdx].iMatchStat = PWORLD_SVR_MATCH_CAN_NOT;
	pstPworldQueue->stClanQueue[iQueueIdx].iPworldInstID = 0;
	pstPworldQueue->stClanQueue[iQueueIdx].iBusID = iBusID;
	pstPworldQueue->stClanQueue[iQueueIdx].iBattleID = iBattleID;
	

	return &pstPworldQueue->stClanQueue[iQueueIdx];
}

int z_get_free_apply_queue_idx(ZONESVRENV* pstEnv)
{
	int i;
	PworldApplyQueue *pstApplyQueue;
	WorldPworldQueue *pstPworldQueue = pstEnv->pstPworldQueue;
	
	for(i = 0; i<MAX_PWORLD_WAIT_QUEUE; i++)
	{
		pstApplyQueue = &pstPworldQueue->stApplyQueue[i];

		if(pstApplyQueue->iPworldID == 0)
		{
			return i;
		}
	}

	return -1;
}

//获得指定副本的报名队列
PworldApplyQueue *get_pworld_apply_queue(ZONESVRENV* pstEnv, int iPworldID)
{
	int i;
	PworldApplyQueue *pstApplyQueue;
	WorldPworldQueue *pstPworldQueue = pstEnv->pstPworldQueue;

	//副本排队队列
	for(i = 0; i<MAX_PWORLD_APPLY_QUEUE; i++)
	{
		pstApplyQueue = &pstPworldQueue->stApplyQueue[i];

		if(pstApplyQueue->iPworldID == 0)
		{
			continue;
		}

		if(pstApplyQueue->iPworldID == iPworldID)
		{
			return pstApplyQueue;
		}
	}
	
	return NULL;
}

PworldApplyQueue *new_pworld_apply_queue(ZONESVRENV* pstEnv, int iPworldID,int iUnitType)
{
	int iQueueIdx;
	WorldPworldQueue *pstPworldQueue = pstEnv->pstPworldQueue;
	
	iQueueIdx = z_get_free_apply_queue_idx(pstEnv);
	if(0 > iQueueIdx )
	{
		return NULL;
	}

	pstPworldQueue->stApplyQueue[iQueueIdx].iPworldID = iPworldID;
	pstPworldQueue->stApplyQueue[iQueueIdx].iQueuerNum = 0;
	pstPworldQueue->stApplyQueue[iQueueIdx].iPworldFlag = 0;
	pstPworldQueue->stApplyQueue[iQueueIdx].iQueuerType = iUnitType;//默认玩家

	return &pstPworldQueue->stApplyQueue[iQueueIdx];
}

//获得指定副本的等待队列
PworldClanQueue *get_pworld_clan_queue_by_id(ZONESVRENV* pstEnv, int iClanQueueIdx,
					int iPworldID)
{
	PworldClanQueue *pstClanQueue;
	WorldPworldQueue *pstPworldQueue = pstEnv->pstPworldQueue;

	if (iClanQueueIdx <0 ||iClanQueueIdx >= MAX_PWORLD_CLAN_QUEUE)
	{
		return NULL;
	}

	pstClanQueue = &pstPworldQueue->stClanQueue[iClanQueueIdx];
	if(pstClanQueue->iPworldID != iPworldID)
	{
		return NULL;
	}

	return pstClanQueue;
}

//获得指定副本的等待队列
PworldWaitQueue *get_pworld_wait_queue(ZONESVRENV* pstEnv, int iWaitQueueIdx, int iPworldInstID)
{
	PworldWaitQueue *pstWaitQueue;
	WorldPworldQueue *pstPworldQueue = pstEnv->pstPworldQueue;

	if (iWaitQueueIdx <0 ||iWaitQueueIdx >= MAX_PWORLD_WAIT_QUEUE)
	{
		return NULL;
	}

	pstWaitQueue = &pstPworldQueue->stWaitQueue[iWaitQueueIdx];
	if(pstWaitQueue->iPworldInstID != iPworldInstID)
	{
		return NULL;
	}

	return pstWaitQueue;
}

int z_clear_pworld_clan_queue(ZONESVRENV* pstEnv, int iClanQueueIdx, int iPworldID)
{
	int i, iCount = 0;
	Player *pstPlayer;
	Player *apstPlayers[MAX_CLAN_QUEUER];
	CSPKG stPkg;
	CSPWORLDSVR *pstPworldSvr = &stPkg.Body.PworldSvr;
	CSEXITPWORLDQUEUE *pstExit = &pstPworldSvr->SvrData.ExitPworldQueue;
	PworldClanQueue *pstClanQueue;
	int iBusID = 0;

	pstClanQueue = get_pworld_clan_queue_by_id(pstEnv, iClanQueueIdx, iPworldID);
	if(NULL == pstClanQueue)
	{
		return -1;
	}
	
	for(i = 0; i < pstClanQueue->iQueuerNum; i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv,
			                                    pstClanQueue->stQueuerInfo[i].iQueuerID);
		if(NULL == pstPlayer)
		{
			continue;
		}

	
		if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
		{
			if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
			{
				continue;
			}
		}
		else 
		{
			if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				continue;
			}
		}

		z_player_online_remove_queue(pstEnv, pstPlayer);
		
		apstPlayers[iCount++] = pstPlayer;
	}

	if(iCount > 0)
	{
		pstPworldSvr->SvrType = PWORLD_SVR_EXIT_PWORLD_QUEUE;
		pstExit->PworldID = pstClanQueue->iPworldID;
		
		Z_CSHEAD_INIT(&stPkg.Head, PWORLD_SVR);
		z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
		                                iCount, &stPkg);
	}

	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
	{
		
		iBusID = pstClanQueue->iBusID;
		
		//通知zone_svr
		clan_span_exit_pworld_queue_req(pstEnv,&pstClanQueue->stClanID,iBusID);
		
		for(i=0; i<iCount; i++)
		{
			player_span_exit_pworld_queue_req(pstEnv, apstPlayers[i]);

			//暴力清除
			z_free_player(pstEnv->pstAppCtx, pstEnv, apstPlayers[i]);
		}

		
	}
	else
	{
		//公会的状态更新下
		ZoneClan *pstClan = z_id_clan(pstEnv, pstClanQueue->stClanID.Id);
		if (pstClan)
		{
			ClanBattleApplyData *pstBattleApplyData = &pstClan->stZoneClanInfo.stBattleApplyData;
			
			
			clan_battle_apply_info_action(pstEnv,pstClan,0);
			pstBattleApplyData->iBattleID = 0;
		}
	}

	

	memset(pstClanQueue, 0, sizeof(*pstClanQueue));
	
	return 0;
}

int z_battle_heart_beat(ZONESVRENV *pstEnv,BATLLEHEARTBEAT *pstBatlleHeartBeat)
{
	ZoneClan* pstClan = z_id_clan(pstEnv, pstBatlleHeartBeat->ClanID.Id);
	if (pstClan && pstClan->stClan.ClanProf.GID == pstBatlleHeartBeat->ClanID.GID)
	{
		ClanBattleApplyData *pstBattleApplyData = &pstClan->stZoneClanInfo.stBattleApplyData;
		
		if (pstBattleApplyData->iBattleID != pstBatlleHeartBeat->BattleID )
		{
			//先发个关闭的
			if (pstBattleApplyData->iBattleID)
			{
				clan_battle_apply_info_action(pstEnv,pstClan,0);
			}

			pstBattleApplyData->iBattleID = pstBatlleHeartBeat->BattleID;
			pstBattleApplyData->iApplyTime = pstEnv->pstAppCtx->stCurr.tv_sec;
			pstBattleApplyData->iApplyStaus = CLAN_BATTLE_STATUS_APPLY;

			clan_battle_apply_info_action(pstEnv,pstClan,1);
		}
		else
		{
			int iOld = pstBattleApplyData->iApplyStaus;
			
			pstBattleApplyData->iApplyTime = pstEnv->pstAppCtx->stCurr.tv_sec;
			pstBattleApplyData->iApplyStaus = CLAN_BATTLE_STATUS_APPLY;

			if (iOld == CLAN_BATTLE_STATUS_CHECK)
			{
				Player* pstPlayer = NULL;
				 CSPWORLDAPPLYINVITE stInvite;
				//第一次收到
				clan_battle_apply_info_action(pstEnv,pstClan,1);
				//自动邀请其他人
				stInvite.BattleID = pstBattleApplyData->iBattleID;
				stInvite.RoleName[0] = 0;
				pstPlayer = z_id_player(pstEnv->pstAppCtx,pstEnv,pstBattleApplyData->iApplyMemID);
				if (pstPlayer)
				{
					STRNCPY(stInvite.RoleName,pstPlayer->stRoleData.RoleName,sizeof(stInvite.RoleName));
				}
				
				player_pworld_apply_invite_in(pstEnv,pstClan,pstPlayer,&stInvite);				
				
			}
		}
	}
	return 0;
}

int z_clear_pworld_wait_queue(ZONESVRENV* pstEnv, int iWaitIdx, int iPworldInstID)
{
	int i, iCount = 0;
	Player *pstPlayer;
	Player *apstPlayers[MAX_APPLY_QUEUER];
	CSPKG stPkg;
	CSPWORLDSVR *pstPworldSvr = &stPkg.Body.PworldSvr;
	CSEXITPWORLDQUEUE *pstExit = &pstPworldSvr->SvrData.ExitPworldQueue;
	PworldWaitQueue *pstWaitQueue;

	pstWaitQueue = get_pworld_wait_queue(pstEnv, iWaitIdx, iPworldInstID);
	if(NULL == pstWaitQueue)
	{
		return -1;
	}

	if (pstWaitQueue->iQueuerType == PWORLD_APPLY_UNIT_CLAN)
	{
		int iClanQueueID;
		for(i = 0; i < pstWaitQueue->iQueuerNum; i++)
		{
			iClanQueueID = pstWaitQueue->stQueuerInfo[i].stRoleInfo.iQueuerID;
			z_clear_pworld_clan_queue(pstEnv,iClanQueueID,pstWaitQueue->iPworldID);
		}
	}
	else
	{
	
		for(i = 0; i < pstWaitQueue->iQueuerNum; i++)
		{
			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv,
				                                    pstWaitQueue->stQueuerInfo[i].stRoleInfo.iQueuerID);
			if(NULL == pstPlayer)
			{
				continue;
			}

			if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
			{
				if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
				{
					continue;
				}
			}
			else 
			{
				if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
				{
					continue;
				}
			}

			z_player_online_remove_queue(pstEnv, pstPlayer);
			
			apstPlayers[iCount++] = pstPlayer;
		}

		if(iCount > 0)
		{
			pstPworldSvr->SvrType = PWORLD_SVR_EXIT_PWORLD_QUEUE;
			pstExit->PworldID = pstWaitQueue->iPworldID;
			
			Z_CSHEAD_INIT(&stPkg.Head, PWORLD_SVR);
			z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
			                                iCount, &stPkg);
		}

		if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
		{
			for(i=0; i<iCount; i++)
			{
				player_span_exit_pworld_queue_req(pstEnv, apstPlayers[i]);

				//暴力清除
				z_free_player(pstEnv->pstAppCtx, pstEnv, apstPlayers[i]);
			}
		}
	}
	memset(pstWaitQueue, 0, sizeof(*pstWaitQueue));
	
	
	return 0;
}

int z_clear_pworld_apply_queue(ZONESVRENV* pstEnv, PworldApplyQueue *pstApplyQueue)
{
	int i, iCount = 0;
	Player *pstPlayer;
	Player *apstPlayers[MAX_APPLY_QUEUER];
	CSPKG stPkg;
	CSPWORLDSVR *pstPworldSvr = &stPkg.Body.PworldSvr;
	CSEXITPWORLDQUEUE *pstExit = &pstPworldSvr->SvrData.ExitPworldQueue;

	if (pstApplyQueue->iQueuerType == PWORLD_CLAN_QUEUE)
	{
		int iClanQueueID;
		for(i = 0; i < pstApplyQueue->iQueuerNum; i++)
		{
			iClanQueueID = pstApplyQueue->stQueuerInfo[i].iQueuerID;
			z_clear_pworld_clan_queue(pstEnv,iClanQueueID,pstApplyQueue->iPworldID);
		}
	}
	else
	{
		for(i = 0; i < pstApplyQueue->iQueuerNum; i++)
		{
			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv,
				                                    pstApplyQueue->stQueuerInfo[i].iQueuerID);
			if(NULL == pstPlayer)
			{
				continue;
			}

			if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
			{
				if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
				{
					continue;
				}
			}
			else 
			{
				if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
				{
					continue;
				}
			}

			apstPlayers[iCount++] = pstPlayer;
			memset(&pstPlayer->stOnline.stPworldQueue, 0, sizeof(pstPlayer->stOnline.stPworldQueue));
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PWORLD111);
		}

		if(iCount > 0)
		{
			pstPworldSvr->SvrType = PWORLD_SVR_EXIT_PWORLD_QUEUE;
			pstExit->PworldID = pstApplyQueue->iPworldID;
			
			Z_CSHEAD_INIT(&stPkg.Head, PWORLD_SVR);
			z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
			                                iCount, &stPkg);
		}

		if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
		{
			for(i=0; i<iCount; i++)
			{
				player_span_exit_pworld_queue_req(pstEnv, apstPlayers[i]);

				//暴力清除
				z_free_player(pstEnv->pstAppCtx, pstEnv, apstPlayers[i]);
			}
		}
	}

	memset(pstApplyQueue, 0, sizeof(*pstApplyQueue));
	
	return 0;
}

//角色的队列信息
RolePworldQueue *z_get_player_online_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID)
{
	if (pstPlayer->stOnline.stPworldQueue.iPworldID == iPworldID &&
		pstPlayer->stOnline.stPworldQueue.iQueueType > 0)
	{
		return &pstPlayer->stOnline.stPworldQueue;
	}

	return NULL;
	UNUSED(pstEnv);	//add by paraunused.pl
}

//退出副本排队、等待队列
int z_exit_pworld_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID, int fNotify)
{
	CSPKG stPkg;
	CSPWORLDSVR *pstPworldSvr = &stPkg.Body.PworldSvr;
	CSEXITPWORLDQUEUE *pstExitQueue = &pstPworldSvr->SvrData.ExitPworldQueue;
	//int i;	//comment by paraunused.pl
	RolePworldQueue *pstRoleQueue;

	pstRoleQueue = z_get_player_online_queue(pstEnv, pstPlayer, iPworldID);
	if(NULL == pstRoleQueue)
	{
		return -1;
	}

	if(pstRoleQueue->iQueueType == PWORLD_APPLY_QUEUE)
	{	
		PworldApplyQueue *pstApplyQueue;
		
		pstApplyQueue = get_pworld_apply_queue(pstEnv, iPworldID);
		if(NULL == pstApplyQueue)
		{
			goto _final;
		}
		
		z_player_remove_apply_queue(pstEnv, pstPlayer, pstApplyQueue);
	}
	else if (pstRoleQueue->iQueueType == PWORLD_WAIT_QUEUE)
	{
		z_player_remove_wait_queue(pstEnv, pstPlayer, pstRoleQueue->iQueueIdx);
	}
	else if (PWORLD_CLAN_QUEUE == pstRoleQueue->iQueueType)
	{
		z_player_remove_clan_queue(pstEnv,pstPlayer, pstRoleQueue->iQueueIdx);
	}

_final:
	
	z_player_online_remove_queue(pstEnv, pstPlayer);
	
	if(fNotify)
	{
		if(g_ApplyIng == 0)
		{
			player_span_exit_pworld_queue_req(pstEnv,pstPlayer);
		}
			
		pstPworldSvr->SvrType = PWORLD_SVR_EXIT_PWORLD_QUEUE;
		pstExitQueue->PworldID = iPworldID;
		
		Z_CSHEAD_INIT(&stPkg.Head, PWORLD_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	return 0;
}

//角色进入报名队列
int z_player_insert_apply_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID)
{
	PworldApplyQueue *pstApplyQueue;
	
	pstApplyQueue = get_pworld_apply_queue(pstEnv, iPworldID);
	if(NULL == pstApplyQueue)
	{
		return -1;
	}

	if(pstApplyQueue->iQueuerNum >= MAX_APPLY_QUEUER)
	{
		//请稍等
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BUSY);
		return -1;
	}

	if (pstPlayer->stOnline.stPworldQueue.iPworldID > 0 &&
		pstPlayer->stOnline.stPworldQueue.iQueueType > 0)
	{
		if (0 > z_exit_pworld_queue(pstEnv, pstPlayer, pstPlayer->stOnline.stPworldQueue.iPworldID, 1))
		{
			return -1;
		}
	}

	pstPlayer->stOnline.stPworldQueue.iQueueType 	= PWORLD_APPLY_QUEUE;
	pstPlayer->stOnline.stPworldQueue.iPworldID 		= iPworldID;
	pstPlayer->stOnline.stPworldQueue.iPworldInstID 	= 0;


	memset(&pstApplyQueue->stQueuerInfo[pstApplyQueue->iQueuerNum], 0, sizeof(pstApplyQueue->stQueuerInfo[0]));
	pstApplyQueue->stQueuerInfo[pstApplyQueue->iQueuerNum++].iQueuerID = pstPlayer->iMemID;;
	return 0;
}

int z_player_apply_pworld_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID)
{
	ZONEIDX stIdx;
	ZONEPWORLD *pstPworld;
	CSPKG stPkg;
	CSPWORLDSVR *pstPworldSvr = &stPkg.Body.PworldSvr;
	CSENTERPWORLDQUEUE *pstEnterQueue = &pstPworldSvr->SvrData.EnterPworldQueue;

	//在副本里面了
	if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE) 
	{
		return -1;
	}
	
	stIdx.iID = iPworldID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (	pstPworld == NULL || 
		pstPworld->stPworldDef.Type != PWORLD_TYPE_PLOY)
	{
		return -1;
	}

	if(	pstPworld->stPworldDef.PolyType != PWORLD_POLY_CAMP_LIFT &&
		pstPworld->stPworldDef.PolyType != PWORLD_POLY_RAND_LIFT &&
		!is_normal_battle_pworld(pstPworld))
	{
		return -1;
	}

	/*
	if(z_get_player_online_queue(pstEnv, pstPlayer, iPworldID))
	{
		//已经 在队列中了
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "你已经报过名了!");
		return -1;
	}
	*/

	if(pstPlayer->stOnline.stPworldQueue.iPworldID > 0 && 
		pstPlayer->stOnline.stPworldQueue.iQueueType > 0)
	{
		// 退其他的报名
		z_exit_pworld_queue(pstEnv, pstPlayer, pstPlayer->stOnline.stPworldQueue.iPworldID, 1);
	}
	
	//可同时排的副本有限，顶提最前面的
	if (0 > z_player_insert_apply_queue(pstEnv, pstPlayer, iPworldID))
	{
		return -1;
	}

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR2);

	pstPworldSvr->SvrType = PWORLD_SVR_ENTER_QUEUE;
	pstEnterQueue->PworldID = iPworldID;

	Z_CSHEAD_INIT(&stPkg.Head, PWORLD_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

// 副本排队
int z_player_pworld_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID,
								CSJOINQUEUE	*pstJoin)
{
	ZONEPWORLD *pstZonePworld;
	ZONEIDX stIdx;
	int iPlayer = 0;
	Player *apstPlayer[MAX_TEAM_MEMBER];
		
	if (z_player_find_status(pstPlayer, STATUS_ID_RUNAWAY) )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PWORLD112);
		return -1;
	}

	if(pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PWORLD113);
		return -1;
	}

	stIdx.iID = iPworldID;
	stIdx.iPos = -1;
	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (NULL == pstZonePworld)
	{
		return -1;
	}

	if (z_check_enter_again(pstEnv,pstPlayer,pstZonePworld) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PWORLD114);
		return -1;
	}

	//团体报名
	if(TEAM_JOIN_CLAN == pstJoin->TeamJoin)
	{
		if(is_battle_clan_queue(pstEnv, pstZonePworld))
		{
			//只在第一次公会官员请求时调用占位
			if (0 > battle_check_apply_clan(pstEnv, pstPlayer, pstZonePworld, apstPlayer, &iPlayer))
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}
	else if(TEAM_JOIN_TEAM == pstJoin->TeamJoin)
	{
		if(is_battle_team_queue(pstEnv, pstZonePworld))
		{
			if (0 > battle_check_apply_team(pstEnv, pstPlayer, pstZonePworld, apstPlayer, &iPlayer))
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}
	 //个人报名
	else
	{
		if (0 > battle_check_apply_single(pstEnv, pstPlayer, pstZonePworld))
		{
			return -1;
		}

		apstPlayer[iPlayer++] = pstPlayer;	
	}

	if(0 == player_spansvr_battle_apply(pstEnv, pstPlayer, pstJoin, pstZonePworld))
	{
		return 0;
	}
	else
	{
		if (z_player_apply_battle_pworld_queue(pstEnv, pstPlayer, apstPlayer,
								iPlayer, pstZonePworld,pstJoin->TeamJoin,0) < 0)
		{
			return -1;
		}
	}

	return 0;
}

// 战场报名本地检查
int z_player_normal_battle_queue_local_check(ZONESVRENV* pstEnv, Player *pstPlayer,
											CSJOINQUEUE *pstJoin,BATTLEDEF *pstBattleDef)
{
	ZoneClan* pstClan;
	ClanBattleApplyData *pstApplyData ;
	CLANMEM *pstMem;
	SpanOL *pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;

	
	//公会成员,官员权限,公会战子类型

	//目前只检查跨服公会战
	if (pstBattleDef->FightType != BATTLE_FIGHT_TYPE_CLAN_WAR)
	{		
		return 0;
	}
	
	pstClan = player_get_clan(pstEnv,pstPlayer);
	if (!pstClan)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE103);
		return -1;
	}

	pstApplyData = &pstClan->stZoneClanInfo.stBattleApplyData;


	if (pstApplyData->iApplyTime + CLAN_BATTLE_HB_TIMEOUT <= pstEnv->pstAppCtx->stCurr.tv_sec &&
		pstApplyData->iBattleID)
	{
		memset(pstApplyData,0,sizeof(*pstApplyData));
	}
	
	if (pstSpanOnline->stApply.iBattleApply != SPAN_APPLY_NONE)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE104);
		return -1;
	}

	//return 0;//本地先不检查

	if (pstApplyData->iBattleID )
	{
		if (pstApplyData->iBattleID != pstJoin->BattleID)
		{
			if (pstJoin->TeamJoin != TEAM_JOIN_CLAN)
			{
				z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE105);
			}
			else
			{
				z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE106);
			}
			return -1;
		}
		
	}
	else
	{
		if (pstJoin->TeamJoin != TEAM_JOIN_CLAN)
		{
			z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE107);
			return -1;
		}

		pstMem = clan_id_mem(pstClan, pstPlayer->stRoleData.RoleID, 
				&pstPlayer->stOnline.iClanMemIdx);

		if (!pstMem)
		{
			z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE108);
			return -1;
		}

		if (pstMem->Position != CLAN_POS_PRESIDENT &&
			pstMem->Position != CLAN_POS_VICE_PRESIDENT)
		{
			z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE112);
			return -1;
		}

		pstApplyData->iBattleID = pstJoin->BattleID;
		pstApplyData->iApplyStaus = CLAN_BATTLE_STATUS_CHECK;
		pstApplyData->iApplyTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstApplyData->iApplyMemID = pstPlayer->iMemID;

		
	}
	
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pstJoin);	//add by paraunused.pl
}
// 战场报名
int z_player_normal_battle_queue(ZONESVRENV* pstEnv, Player *pstPlayer, CSJOINQUEUE *pstJoin)
{
	int i, iPworldID = 0;
	int iLevel = pstPlayer->stRoleData.Level;
	BATTLEDEF *pstBattleDef = z_find_battledef(pstEnv, pstJoin->BattleID);
	SpanOL *pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;

	ZoneTeam *pstTeam =NULL;
	//lzk  融焰怪,	活动中限制玩家进入战场
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(pstTeam && 1 == pstTeam->iRongYanBegin)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE109);
		return -1;
	}

	//末世巅峰准备地图和决赛副本中不能战场报名
	if (pstPlayer->stRoleData.Map == SHZ_READY_MAP_ID || pstPlayer->stRoleData.Map == ZHZ_READY_MAP_ID ||
		pstPlayer->stRoleData.Map == JNZ_READY_MAP_ID || pstPlayer->stRoleData.Map == LHS_READY_MAP_ID ||
		pstPlayer->stRoleData.Map == LSZ_READY_MAP_ID || pstPlayer->stRoleData.Map == HMZ_READY_MAP_ID ||
		pstPlayer->stRoleData.Map == SYZ_READY_MAP_ID || pstPlayer->stRoleData.Map == YLS_READY_MAP_ID ||
		(GET_PWORLDID(pstPlayer->stRoleData.Map) == DIANFENG_PWORLD_ID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_DIANFENG24);
		return -1;
	}

	//终极熔岩巨兽召唤者不能进入战场	
	if(0 < player_if_call_zj_rongyan(pstEnv,pstPlayer))
	{	
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE109);
		return -1;
	}

	if(pstSpanOnline->stApply.iSpartaApply != SPAN_APPLY_NONE)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE110);
		return -1;
	}
	
	if (NULL == pstBattleDef)
	{
		return 0;
	}

	//削弱战场机制
	if(0 > z_player_level_remain_tm_check(pstEnv, pstPlayer,pstBattleDef))
	{
		return -1;
	}

	for (i=0; i<MAX_BATTLE_PWORLD_QUEUE; i++)
	{		
		if (	iLevel >= pstBattleDef->BattleQueue[i].MinLevel &&
			iLevel <= pstBattleDef->BattleQueue[i].MaxLevel)
		{
			iPworldID = pstBattleDef->BattleQueue[i].PworldID;
			break;
		}
	}

	if (iPworldID <= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PWORLD115, pstBattleDef->Name);
		return -1;
	}

	if(INST_MODE_ZONE_SVR == pstEnv->iInstMode)
	{
		//战场限制由活动事件开启
		if(pstBattleDef->EventLimit)
		{
			GameEventInst stEventInst, *pstEvent;
			
			stEventInst.iEventID = pstBattleDef->EventLimit;
			pstEvent = sht_find(pstEnv->pstShtEvent, &stEventInst, EventHashCmp, EventHashCode);
			if (NULL == pstEvent)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_PWORLD116, pstBattleDef->Name);
				return -1;
			}
		}
		
		// 战场报名本地检查
		if (0 > z_player_normal_battle_queue_local_check(pstEnv,pstPlayer,pstJoin,pstBattleDef))
		{
			return -1;
		}
	}
	
	return z_player_pworld_queue(pstEnv, pstPlayer, iPworldID, pstJoin);
}

int z_player_remove_clan_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iQeueIdx)
{
	PworldClanQueue *pstClanQueue;
	int i;
	int iLevel;

	pstClanQueue = get_pworld_clan_queue_by_id(pstEnv, iQeueIdx, 
			pstPlayer->stOnline.stPworldQueue.iPworldID);
	if(NULL == pstClanQueue)
	{
		return -1;
	}

	for(i = pstClanQueue->iQueuerNum-1; i>=0; i--)
	{	
		if(pstClanQueue->stQueuerInfo[i].iQueuerID == pstPlayer->iMemID)
		{
			break;
		}
	}

	if(i <0)
	{
		return -1;
	}

	iLevel = pstClanQueue->stQueuerInfo[i].iLevel;
	
	pstClanQueue->iQueuerNum--;

	if(i != pstClanQueue->iQueuerNum)
	{
		memcpy(&pstClanQueue->stQueuerInfo[i], &pstClanQueue->stQueuerInfo[pstClanQueue->iQueuerNum], 
			sizeof(pstClanQueue->stQueuerInfo[0]));
	}

	//更新匹配信息
	{
		ZONEIDX stIdx;
		ZONEPWORLD*  pstPworld;

		stIdx.iID = pstClanQueue->iPworldID;
		stIdx.iPos = -1;
		
		pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
		if (pstPworld)
		{
			player_pworld_clan_queue_all_match_stat(pstEnv,pstPworld,pstClanQueue);
		}		
	}
	
/*
	//更新阵营信息
	if (pstClanQueue->iQueueType == PWORLD_WAIT_QUEUE)
	{
		PworldWaitQueue* pstWaitQueue = NULL;
		ZONEPWORLDINST* pstPworldInst = NULL;
		PworldCampPlayerInfo *pstPworldCampInfo = NULL;

		pstWaitQueue = get_pworld_wait_queue(pstEnv, pstClanQueue->iQueueIdx,
									pstClanQueue->iPworldInstID);

		if (!pstWaitQueue)
		{
			return -1;//log?
		}
		
		pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstClanQueue->iPworldInstID);
		if(pstPworldInst == NULL || pstPworldInst->bOver)
		{
			return 0;
		}
		
		if (pstClanQueue->iCampIdx >=0 && 
			pstClanQueue->iCampIdx < pstPworldInst->stPworldCamp.iCampNum)
		{
			pstPworldCampInfo = 
				&pstPworldInst->stPworldCamp.stCampInfo[pstClanQueue->iCampIdx];
		}

		if (!pstPworldCampInfo)
		{
			//log ?
			return -1;
		}
		
		pstPworldCampInfo->iPlayerNum--;
		pstPworldCampInfo->iCampLv -= iLevel;
				

	}

*/
	

	return 0;
}

int z_player_remove_apply_queue(ZONESVRENV* pstEnv, Player *pstPlayer,
								PworldApplyQueue *pstApplyQueue)
{
	int i;
	
	for(i = pstApplyQueue->iQueuerNum-1; i>=0; i--)
	{
		if (pstApplyQueue->stQueuerInfo[i].iQueuerID == pstPlayer->iMemID)
		{
			break;
		}
	}

	if(i >= 0)
	{
		pstApplyQueue->iQueuerNum--;

		if(i != pstApplyQueue->iQueuerNum)
		{
			memmove(&pstApplyQueue->stQueuerInfo[i], &pstApplyQueue->stQueuerInfo[i+1], 
				sizeof(pstApplyQueue->stQueuerInfo[0])*(pstApplyQueue->iQueuerNum-i));
		}
	}
	
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int z_player_remove_wait_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iWaitIdx)
{
	PworldWaitQueue *pstWaitQueue;
	int i;

	pstWaitQueue = get_pworld_wait_queue(pstEnv, iWaitIdx, pstPlayer->stOnline.stPworldQueue.iPworldInstID);
	if(NULL == pstWaitQueue)
	{
		return -1;
	}

	for(i = pstWaitQueue->iQueuerNum-1; i>=0; i--)
	{	
		if(pstWaitQueue->stQueuerInfo[i].stRoleInfo.iQueuerID == pstPlayer->iMemID)
		{
			break;
		}
	}

	if(i <0)
	{
		return -1;
	}

	//更新阵营信息
	pstWaitQueue->stPworldCamp.stCampInfo[pstWaitQueue->stQueuerInfo[i].iCamp-1].iPlayerNum--;
	pstWaitQueue->stPworldCamp.stCampInfo[pstWaitQueue->stQueuerInfo[i].iCamp-1].iCampLv -= 
			pstWaitQueue->stQueuerInfo[i].stRoleInfo.iLevel;

	pstWaitQueue->iQueuerNum--;

	if(i != pstWaitQueue->iQueuerNum)
	{
		memcpy(&pstWaitQueue->stQueuerInfo[i], &pstWaitQueue->stQueuerInfo[pstWaitQueue->iQueuerNum], 
			sizeof(pstWaitQueue->stQueuerInfo[0]));
	}
	
	return 0;
}

//离开队列角色信息同步
int z_player_online_remove_queue(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	//pstPlayer->stOnline.stPworldQueue.iPworldID = 0;
	//pstPlayer->stOnline.stPworldQueue.iPworldInstID = 0;
	pstPlayer->stOnline.stPworldQueue.iQueueType = 0;
	pstPlayer->stOnline.stPworldQueue.iQueueIdx = 0;

	//memset (&pstPlayer->stOnline.stPworldQueue, 0, sizeof(pstPlayer->stOnline.stPworldQueue));
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int player_clan_queue_invite(ZONESVRENV *pstEnv, ZONEPWORLD *pstPworld,
							int iPworldInstID,int iClanQueueId,int iCamp,
							Player* pstPlayer,int iWaitQueueIdx)
{
	RolePworldQueue *pstRoleQueue;
	CSPKG stPkg;
	CSPWORLDSVR *pstPworldSvr = &stPkg.Body.PworldSvr;
	CSINVITEENTERPWORLD *pstInvite = &pstPworldSvr->SvrData.InviteEnterPworld;
	
	//自己身上有没有
	pstRoleQueue = z_get_player_online_queue(pstEnv, pstPlayer, pstPworld->stPworldDef.PworldID);
	if(NULL == pstRoleQueue || pstRoleQueue->iQueueType != PWORLD_CLAN_QUEUE)
	{
		return -1;
	}

	pstRoleQueue->iQueueType = PWORLD_CLAN_QUEUE;
	pstRoleQueue->iPworldInstID = iPworldInstID;
	pstRoleQueue->iQueueIdx = iClanQueueId;
	pstRoleQueue->iCamp = iCamp+1;
	pstRoleQueue->iPworldID = pstPworld->stPworldDef.PworldID;



	pstPworldSvr->SvrType = PWORLD_SVR_INVITE_ENTER_PWORLD;
	pstInvite->PworldID = pstPworld->stPworldDef.PworldID;
	//用于 区分 副本实例 
	pstInvite->PworldIdx = iWaitQueueIdx +1;
	
	Z_CSHEAD_INIT(&stPkg.Head, PWORLD_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	
	return 0;
}

//通知报名匹配状态
int player_pworld_clan_queue_match_stat(ZONESVRENV *pstEnv, ZONEPWORLD *pstPworld,							
							Player** ppstPlayer,int iPlayer,
							PworldClanQueue* pstClanQueue,
							PworldCampPlayerInfo *pstCampInfo)
{
	CSPKG stPkg;
	CSPWORLDSVR *pstPworldSvr = &stPkg.Body.PworldSvr;
	CSPWORLDMATCHSTATUS *pstMatch = &pstPworldSvr->SvrData.MatchStatus;
	 

	if (0 == pstPworld->stPworldDef.MatchType)
	{
		return 0;
	}

	//匹配状态
	
	pstPworldSvr->SvrType = PWORLD_SVR_MATCH;
	pstMatch->PworldID = pstClanQueue->iPworldID;
	pstMatch->PlayerNum = pstClanQueue->iQueuerNum;
	pstMatch->MatchStatus = pstClanQueue->iMatchStat;

	if (pstCampInfo)
	{
		pstMatch->PlayerNum += pstCampInfo->iPlayerNum;
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, PWORLD_SVR);

	if (iPlayer == 1)
	{
		return z_cltmsg_send(pstEnv, *ppstPlayer, &stPkg);
	}
	else if(iPlayer > 1)
	{
		return z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, ppstPlayer,
		                                iPlayer, &stPkg);
	}
	else
	{
		return -1;
	}

	
	return 0;
}

//全队列通知报名匹配状态
int player_pworld_clan_queue_all_match_stat(ZONESVRENV *pstEnv, ZONEPWORLD *pstPworld,							
							PworldClanQueue* pstClanQueue)
{
	int i;

	if (0 == pstPworld->stPworldDef.MatchType)
	{
		return 0;
	}
	
	i = pstClanQueue->iQueuerNum;
	while(i--)
	{		
		Player* pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv,
						pstClanQueue->stQueuerInfo[i].iQueuerID);
		
		if(NULL == pstPlayer)
		{
			continue;
		}	

		if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
		{
			if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
			{
				continue;
			}
		}
		else 
		{
			if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				continue;
			}
		}

		
		player_pworld_clan_queue_match_stat(pstEnv,pstPworld,&pstPlayer,1,
									pstClanQueue,NULL);
	}
	return 0;
}

int z_clan_insert_wait_queue(ZONESVRENV *pstEnv, ZONEPWORLD *pstPworld, int iPworldInstID, 
									int iWaitQueueIdx, PworldClanQueue *pstClanQueue,
									int iCamp)
{
	PworldWaitQueue *pstWaitQueue;
	WorldPworldQueue *pstPworldQueue = pstEnv->pstPworldQueue;
	PworldCampPlayerInfo *pstCampInfo;

	int i;
	int iClanQueueId =0;
	

	if (iWaitQueueIdx <0 ||iWaitQueueIdx >= MAX_PWORLD_WAIT_QUEUE)
	{
		return -1;
	}

	pstWaitQueue = &pstPworldQueue->stWaitQueue[iWaitQueueIdx];
	if (pstWaitQueue->iQueuerNum >= pstPworld->stPworldDef.CampNum)
	{
		return -1;
	}

	iClanQueueId = pstClanQueue - pstPworldQueue->stClanQueue;

	pstWaitQueue->stQueuerInfo[pstWaitQueue->iQueuerNum].stRoleInfo.iQueuerID =iClanQueueId; 
		
	
	pstWaitQueue->stQueuerInfo[pstWaitQueue->iQueuerNum].stRoleInfo.iLevel = 0;
	pstWaitQueue->stQueuerInfo[pstWaitQueue->iQueuerNum].iCamp = iCamp + 1;
	pstWaitQueue->stQueuerInfo[pstWaitQueue->iQueuerNum].tValidTime = 0;//一直有效
	pstWaitQueue->iQueuerNum++;

	//阵营信息
	pstCampInfo = &pstWaitQueue->stPworldCamp.stCampInfo[iCamp];
	pstCampInfo->iCampLv = 0;
	pstCampInfo->iPlayerNum = 0;//pstClanQueue->iQueuerNum;
	pstCampInfo->stClanID = pstClanQueue->stClanID;
	pstCampInfo->CampVal = 0;
	

	

	pstClanQueue->iQueueType = PWORLD_WAIT_QUEUE;//从报名队列转入报名队列
	pstClanQueue->iQueueIdx = iWaitQueueIdx;
	pstClanQueue->iPworldInstID = iPworldInstID;
	pstClanQueue->iCampIdx = iCamp;
	pstClanQueue->iMatchStat = PWORLD_SVR_MATCH_DONE;
	
	
	i = pstClanQueue->iQueuerNum;
	while(i--)
	{		
		Player* pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv,
						pstClanQueue->stQueuerInfo[i].iQueuerID);
		
		if(NULL == pstPlayer)
		{
			continue;
		}	

		if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
		{
			if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
			{
				continue;
			}
		}
		else 
		{
			if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				continue;
			}
		}

		pstCampInfo->iCampLv += pstPlayer->stRoleData.Level;

		//匹配状态
		if (pstPworld->stPworldDef.MatchType)
		{
			player_pworld_clan_queue_match_stat(pstEnv,pstPworld,&pstPlayer,1,
							pstClanQueue,pstCampInfo);
		}
		

		if ( 0 == player_clan_queue_invite(pstEnv,pstPworld,iPworldInstID,
							iClanQueueId,iCamp,pstPlayer,iWaitQueueIdx))
		{
			pstClanQueue->stQueuerInfo[i].iInvite = 1;//已经邀请
			pstClanQueue->stQueuerInfo[i].tValidTime = 
				pstEnv->pstAppCtx->stCurr.tv_sec + INVITE_ENTER_PWORLD_TIME;
		}
	}
	
	return 0;
}

int z_player_inset_wait_queue(ZONESVRENV *pstEnv, ZONEPWORLD *pstPworld, int iPworldInstID, 
									int iWaitQueueIdx, Player *pstPlayer, int iCamp)
{
	PworldWaitQueue *pstWaitQueue;
	WorldPworldQueue *pstPworldQueue = pstEnv->pstPworldQueue;
	RolePworldQueue *pstRoleQueue;

	if (iWaitQueueIdx <0 ||iWaitQueueIdx >= MAX_PWORLD_WAIT_QUEUE)
	{
		return -1;
	}

	pstWaitQueue = &pstPworldQueue->stWaitQueue[iWaitQueueIdx];
	if (pstWaitQueue->iQueuerNum >= pstPworld->stPworldDef.PlayerNum)
	{
		return -1;
	}

	//自己身上有没有
	pstRoleQueue = z_get_player_online_queue(pstEnv, pstPlayer, pstPworld->stPworldDef.PworldID);
	if(NULL == pstRoleQueue || pstRoleQueue->iQueueType != PWORLD_APPLY_QUEUE)
	{
		return -1;
	}

	pstRoleQueue->iQueueType = PWORLD_WAIT_QUEUE;
	pstRoleQueue->iPworldInstID = iPworldInstID;
	pstRoleQueue->iQueueIdx = iWaitQueueIdx;
	pstRoleQueue->iCamp = iCamp+1;
	pstRoleQueue->iPworldID = pstPworld->stPworldDef.PworldID;
	//pstRoleQueue->stBattleCampInfo.BattleBusID = pstWaitQueue->iBattleBusID;
	//pstRoleQueue->stBattleCampInfo.PworldCampID = pstWaitQueue->PworldCampID;
	pstRoleQueue->stBattleCampInfo.PworldDefID = pstPworld->stPworldDef.PworldID ;
	pstRoleQueue->stBattleCampInfo.PworldInstID = pstWaitQueue->iPworldInstID;
	pstRoleQueue->stBattleCampInfo.PworldWID = pstWaitQueue->PworldWID;
	//pstRoleQueue->stBattleCampInfo.ZoneBusID = pstEnv->pstAppCtx->iId;

	pstWaitQueue->stQueuerInfo[pstWaitQueue->iQueuerNum].stRoleInfo.iQueuerID = pstPlayer->iMemID;
	pstWaitQueue->stQueuerInfo[pstWaitQueue->iQueuerNum].stRoleInfo.iLevel = pstPlayer->stRoleData.Level;
	pstWaitQueue->stQueuerInfo[pstWaitQueue->iQueuerNum].iCamp = iCamp+1;
	pstWaitQueue->stQueuerInfo[pstWaitQueue->iQueuerNum].tValidTime = 
		pstEnv->pstAppCtx->stCurr.tv_sec + INVITE_ENTER_PWORLD_TIME;
	pstWaitQueue->iQueuerNum++;

	//阵营信息
	pstWaitQueue->stPworldCamp.stCampInfo[iCamp].iCampLv += pstPlayer->stRoleData.Level;
	pstWaitQueue->stPworldCamp.stCampInfo[iCamp].iPlayerNum++;
	
	return 0;
}

//转入等待队列
int z_enter_wait_queue(ZONESVRENV *pstEnv, ZONEPWORLD *pstPworld, 
						   int iPworldInstID, int iWaitQueueIdx, QueueInfo *pstQueuerInfo,
						   int iEnterNum)
{
	int i, iCount = 0;
	Player *pstPlayer;
	Player *apstPlayers[MAX_APPLY_QUEUER];
	CSPKG stPkg;
	CSPWORLDSVR *pstPworldSvr = &stPkg.Body.PworldSvr;
	CSINVITEENTERPWORLD *pstInvite = &pstPworldSvr->SvrData.InviteEnterPworld;
	
	for(i = 0; i < iEnterNum; i++)
	{
		if(pstQueuerInfo[i].iQueuerID <= 0)
		{
			continue;
		}

		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv,pstQueuerInfo[i].iQueuerID);
		if(NULL == pstPlayer)
		{
			continue;
		}

		if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
		{
			if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
			{
				continue;
			}
		}
		else 
		{
			if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				continue;
			}
		}

		if (0 > z_player_inset_wait_queue(pstEnv, pstPworld, iPworldInstID,iWaitQueueIdx, pstPlayer, 0))
		{
			continue;
		}

		apstPlayers[iCount++] = pstPlayer;
	}

	if(iCount > 0)
	{
		pstPworldSvr->SvrType = PWORLD_SVR_INVITE_ENTER_PWORLD;
		pstInvite->PworldID = pstPworld->stPworldDef.PworldID;

		//用于 区分 副本实例 
		pstInvite->PworldIdx = iWaitQueueIdx +1;
		
		Z_CSHEAD_INIT(&stPkg.Head, PWORLD_SVR);
		
		return z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
		                                iCount, &stPkg);
	}
	
	return 0;
}

int z_apply2wait_queue(ZONESVRENV* pstEnv, ZONEPWORLD *pstPworld, 
		PworldApplyQueue *pstApplyQueue, int iEnterNum, int  iPworldInstID, int iWaitQueueIdx)
{
	//进入等待队列
	z_enter_wait_queue(pstEnv, pstPworld, iPworldInstID, iWaitQueueIdx,
						pstApplyQueue->stQueuerInfo, iEnterNum);

	if(pstApplyQueue->iQueuerNum > iEnterNum)
	{
		memmove(&pstApplyQueue->stQueuerInfo[0], &pstApplyQueue->stQueuerInfo[iEnterNum], 
					sizeof(pstApplyQueue->stQueuerInfo[0])*(pstApplyQueue->iQueuerNum-iEnterNum));
	}
	pstApplyQueue->iQueuerNum -= iEnterNum;

	return 0;
}

int get_battle_enter_validtime(ZONEPWORLD *pstPworld)
{
	int iEnterTime = 0;
	
	if (pstPworld->stPworldDef.EnterTime > 0)
	{
		iEnterTime = pstPworld->stPworldDef.EnterTime*60;
	}
	else
	{
		iEnterTime = pstPworld->stPworldDef.Timeout*60/2;
	}

	if(iEnterTime <= 0)
	{
		iEnterTime = 600;
	}

	return iEnterTime;
}

int get_apply_queue_first_player_num(PworldApplyQueue *pstApplyQueue)
{
	int i;
	int iCount = 0;

	for(i =0;i <pstApplyQueue->iQueuerNum; i++)
	{
		if(pstApplyQueue->stQueuerInfo[i].ullTeamWID != pstApplyQueue->stQueuerInfo[0].ullTeamWID)
		{
			break;
		}

		iCount++;
		if(pstApplyQueue->stQueuerInfo[i].ullTeamWID == 0)
		{
			break;
		}
	}

	return iCount;
}


int get_camp_empty_player_num(ZONESVRENV* pstEnv, ZONEPWORLD *pstPworld, 
	ZONEPWORLDINST *pstPworldInst, int iCamp)
{
	int iEmpty;
	PworldWaitQueue *pstWaitQueue = &pstEnv->pstPworldQueue->stWaitQueue[pstPworldInst->iWaitQueueIdx];

	if(iCamp >= pstPworldInst->stPworldCamp.iCampNum)
	{
		return 0;
	}
	
	iEmpty = pstPworldInst->stPworldCamp.stCampInfo[iCamp].iPlayerNum +
				pstWaitQueue->stPworldCamp.stCampInfo[iCamp].iPlayerNum;

	iEmpty = pstPworld->stPworldDef.PlayerNum/pstPworld->stPworldDef.CampNum - iEmpty;
	if(iEmpty < 0)
	{
		iEmpty = 0;
	}

	return iEmpty;
}

//各个阵营的空位统计
int get_battle_queue_info(ZONESVRENV* pstEnv , ZONEPWORLD *pstPworld, 
	ZONEPWORLDINST *pstPworldInst, CampQueueInfo *pstQueueInfo)
{
	int i;
	
	memset(pstQueueInfo, 0, sizeof(*pstQueueInfo));

	//阵营数
	pstQueueInfo->iCampNum = pstPworldInst->stPworldCamp.iCampNum;

	//统计空位
	for(i=0; i<pstPworldInst->stPworldCamp.iCampNum; i++)
	{
		pstQueueInfo->stCampQueuer[i].iMaxQueuer = 
			get_camp_empty_player_num(pstEnv, pstPworld, pstPworldInst, i);
	}
	
	return 0;
}

int queuers_remove_applyqueue(PworldApplyQueue *pstApplyQueue, int iQueuerNum, int *paiQueuer)
{
	int i = 0;
	int iNum = 0;

	if(iQueuerNum <= 0)
		return 0;
	
	for(i=0; i<pstApplyQueue->iQueuerNum; i++)
	{
		if(pstApplyQueue->stQueuerInfo[i].iQueuerID != paiQueuer[0])
		{
			continue;
		}

		if(iQueuerNum > 1 && i+iQueuerNum <= pstApplyQueue->iQueuerNum)
		{
			iNum = iQueuerNum;
		}
		else
		{
			iNum = 1;
		}

		break;
	}

	if(i < pstApplyQueue->iQueuerNum)
	{
		pstApplyQueue->iQueuerNum -= iNum;
		if( i != pstApplyQueue->iQueuerNum)
		{
			memmove(&pstApplyQueue->stQueuerInfo[i], &pstApplyQueue->stQueuerInfo[i+iNum], 
						sizeof(pstApplyQueue->stQueuerInfo[0])*(pstApplyQueue->iQueuerNum-i));
		}	
	}
	
	return 0;
}

//把一个单位的人员放入等待队列，并且从排队队列中清除
int queuers_inset_waitqueue(ZONESVRENV* pstEnv , ZONEPWORLD *pstPworld, 
	ZONEPWORLDINST *pstPworldInst, int iQueuerNum, int *paiQueuer, int iCamp)
{
	int i;
	Player *pstPlayer;
	PworldApplyQueue *pstApplyQueue = get_pworld_apply_queue(pstEnv, pstPworld->stPworldDef.PworldID);
	CSPKG stPkg;
	CSPWORLDSVR *pstPworldSvr = &stPkg.Body.PworldSvr;
	CSINVITEENTERPWORLD *pstInvite = &pstPworldSvr->SvrData.InviteEnterPworld;


	if (iQueuerNum <=0)
	{
		return 0;
	}

	if (!pstApplyQueue)
	{
		return -1;
	}
	
	//移除排队队列
	if(pstApplyQueue)
	{
		queuers_remove_applyqueue(pstApplyQueue, iQueuerNum, paiQueuer);
	}

	if (pstApplyQueue->iQueuerType == PWORLD_APPLY_UNIT_PLAYER)
	{
		i = iQueuerNum;
		while(i--)
		{
			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, paiQueuer[i]);
			if(NULL == pstPlayer)
			{
				continue;
			}	

			if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
			{
				if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
				{
					continue;
				}
			}
			else 
			{
				if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
				{
					continue;
				}
			}

			//放入等待队列
			z_player_inset_wait_queue(pstEnv, pstPworld, pstPworldInst->iPworldInstID, 
				pstPworldInst->iWaitQueueIdx, pstPlayer, iCamp);


			pstPworldSvr->SvrType = PWORLD_SVR_INVITE_ENTER_PWORLD;
			pstInvite->PworldID = pstPworld->stPworldDef.PworldID;
			//用于 区分 副本实例 
			pstInvite->PworldIdx = pstPworldInst->iWaitQueueIdx +1;
			
			Z_CSHEAD_INIT(&stPkg.Head, PWORLD_SVR);
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		}
	}
	else if (pstApplyQueue->iQueuerType == PWORLD_APPLY_UNIT_CLAN)
	{
		PworldClanQueue* pstClanQueue;
		i = iQueuerNum;
		iCamp = 0;
		while(i--)
		{	
			PworldCampPlayerInfo *pstCampInfo;
			
			if (paiQueuer[i] < 0 || 
				paiQueuer[i] >= MAX_PWORLD_CLAN_QUEUE)
			{
				continue;
			}
			pstClanQueue = &pstEnv->pstPworldQueue->stClanQueue[paiQueuer[i]];

			//副本阵营信息
			pstCampInfo = &pstPworldInst->stPworldCamp.stCampInfo[iCamp];
			pstCampInfo->CampVal = 1;
			pstCampInfo->stClanID = pstClanQueue->stClanID;
			pstCampInfo->iSvrBusID = pstClanQueue->iBusID;
		
			//放入等待队列
			z_clan_insert_wait_queue(pstEnv, pstPworld, pstPworldInst->iPworldInstID, 
				pstPworldInst->iWaitQueueIdx, pstClanQueue, iCamp);

		
			
			iCamp++;
			
			if (iCamp >= pstPworld->stPworldDef.CampNum || iCamp >= MAX_CAMP_NUM)
			{
				break;
			}

			

		}
	}
	else
	{
		//do nothing
	}
		

	return 0;
}

//邀请队列人员分配
int queuer_assign_waitqueue(ZONESVRENV* pstEnv , ZONEPWORLD *pstPworld, 
	ZONEPWORLDINST *pstPworldInst, CampQueueInfo *pstQueueInfo, 
	int iQueuerNum, int  *paiQueuer)
{
	int i, iCampLv;
	int iIdx = -1, iRet = -1;
	CampQueuer *pstCampQueueInfo;
	PworldWaitQueue *pstWaitQueue = &pstEnv->pstPworldQueue->stWaitQueue[pstPworldInst->iWaitQueueIdx];
	
	for(i=0; i<pstQueueInfo->iCampNum; i++)
	{
		pstCampQueueInfo = &pstQueueInfo->stCampQueuer[i];

		//还能进人
		if(pstCampQueueInfo->iMaxQueuer > pstCampQueueInfo->iQueuer)
		{
			iRet = -2;
		}
		
		if(pstCampQueueInfo->iQueuer + iQueuerNum > pstCampQueueInfo->iMaxQueuer)
		{
			continue;
		}

		iCampLv = pstWaitQueue->stPworldCamp.stCampInfo[i].iCampLv 
			+ pstPworldInst->stPworldCamp.stCampInfo[i].iCampLv;

		//根据等级最低的一方先放人
		if(iIdx != -1 &&
			iCampLv >=  pstWaitQueue->stPworldCamp.stCampInfo[iIdx].iCampLv + pstPworldInst->stPworldCamp.stCampInfo[iIdx].iCampLv)
		{
			continue;
		}
		
		iIdx = i;
	}

	if(iIdx < 0)
	{
		// -1 满了， -2 还有位置
		return iRet;
	}
	
	queuers_inset_waitqueue(pstEnv, pstPworld, pstPworldInst, iQueuerNum, paiQueuer, iIdx);
	
	return 0;
}


//排队转入邀请队列
int normal_battle_apply2wait(ZONESVRENV* pstEnv, ZONEPWORLD *pstPworld, 
	ZONEPWORLDINST *pstPworldInst, PworldApplyQueue *pstApplyQueue)
{
	int i, iRet;
	int iQueuer, aiQueuer[MAX_TEAM_MEMBER];
	CampQueueInfo stTmpQueueInfo;
	PworldApplyQueue stTmpApplyQueue;

	stTmpApplyQueue = *pstApplyQueue;
	
	//找一下有多少个空位置
	get_battle_queue_info(pstEnv, pstPworld, pstPworldInst, &stTmpQueueInfo);

	//通过applyqueue,找到clanqueue,再整个公会一起进
	if (PWORLD_APPLY_UNIT_PLAYER ==stTmpApplyQueue.iQueuerType)
	{
		iQueuer = 0;
		for(i = 0; i<stTmpApplyQueue.iQueuerNum; i++)
		{		
			//不管是否是有效的人，无效的顺便也剔除掉
			aiQueuer[iQueuer++] = stTmpApplyQueue.stQueuerInfo[i].iQueuerID;

			if(i+1 < stTmpApplyQueue.iQueuerNum && stTmpApplyQueue.stQueuerInfo[i].ullTeamWID > 0)
			{
				//整组人一起
				if(stTmpApplyQueue.stQueuerInfo[i].ullTeamWID == stTmpApplyQueue.stQueuerInfo[i+1].ullTeamWID)
				{
					continue;
				}
			}
			
			//把一个单位放进去
			iRet = queuer_assign_waitqueue(pstEnv, pstPworld, pstPworldInst, &stTmpQueueInfo, iQueuer, aiQueuer);
			//放不下人了
			if(iRet == -1)
			{
				break;
			}
			
			iQueuer = 0;
		}
	}
	else if (PWORLD_APPLY_UNIT_CLAN == stTmpApplyQueue.iQueuerType)
	{
		iQueuer = 0;
		for (i = 0; i < pstApplyQueue->iQueuerNum; i ++)
		{
			int iClanQueueID = pstApplyQueue->stQueuerInfo[i].iQueuerID;
			if ( 0 == z_pworld_clan_queue_match_check(pstEnv, pstPworld,iClanQueueID))
			{
				aiQueuer[iQueuer++] = iClanQueueID;
			}
			
			if (iQueuer >= pstPworld->stPworldDef.CampNum)
			{
				break;
			}			
		}

		if(iQueuer < pstPworld->stPworldDef.CampNum)
		{
			return 0;
		}	

		iRet = queuers_inset_waitqueue(pstEnv, pstPworld, pstPworldInst,
										iQueuer, aiQueuer,0);
			
	}
	else
	{
		//do nothing
	}

	
	return 0;
}


//公会战区别对待
//副本等待队列，有效人员  检查
int z_pworld_wait_queue_check(ZONESVRENV* pstEnv,  PworldWaitQueue *pstWaitQueue)
{
	int i, iEnterTime;
	Player *pstPlayer;
	ZONEPWORLDINST *pstPworldInst;
	ZONEPWORLD *pstPworld;

	if (pstWaitQueue->iQueuerType == PWORLD_APPLY_UNIT_CLAN)
	{
		int iClanQueueID;
		for(i = 0; i < pstWaitQueue->iQueuerNum; i++)
		{
			PworldClanQueue *pstClanQueue;
			
			iClanQueueID = pstWaitQueue->stQueuerInfo[i].stRoleInfo.iQueuerID;
			pstClanQueue = get_pworld_clan_queue_by_id(pstEnv, iClanQueueID,
										pstWaitQueue->iPworldID);
			if (!pstClanQueue)
			{
				continue;
			}
			z_pworld_clan_queue_check(pstEnv,pstClanQueue);
		}


		return 0;
	}
	else
	{

		for(i = pstWaitQueue->iQueuerNum-1; i>=0; i--)
		{
			if (pstWaitQueue->stQueuerInfo[i].tValidTime >=  pstEnv->pstAppCtx->stCurr.tv_sec)
			{

				continue;
			}

			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstWaitQueue->stQueuerInfo[i].stRoleInfo.iQueuerID);
			if(pstPlayer)
			{
				z_player_online_remove_queue(pstEnv, pstPlayer);
				if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
				{
					player_span_exit_pworld_queue_req(pstEnv, pstPlayer);
					//暴力清除
					z_free_player(pstEnv->pstAppCtx, pstEnv, pstPlayer);
				}
			}

			//更新阵营信息
			pstWaitQueue->stPworldCamp.stCampInfo[pstWaitQueue->stQueuerInfo[i].iCamp-1].iPlayerNum--;
			pstWaitQueue->stPworldCamp.stCampInfo[pstWaitQueue->stQueuerInfo[i].iCamp-1].iCampLv -= 
					pstWaitQueue->stQueuerInfo[i].stRoleInfo.iLevel;
			
			pstWaitQueue->iQueuerNum--;

			if(i != pstWaitQueue->iQueuerNum)
			{
				memcpy(&pstWaitQueue->stQueuerInfo[i], &pstWaitQueue->stQueuerInfo[pstWaitQueue->iQueuerNum], 
					sizeof(pstWaitQueue->stQueuerInfo[0]));
			}
		}

		//有位置的就放人进去
		pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstWaitQueue->iPworldInstID);
		if(pstPworldInst == NULL || pstPworldInst->bOver)
		{
			return 0;
		}

		pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
		if (pstPworld == NULL)
		{
			return -1;
		}

		//可进入的有效时间
		iEnterTime = get_battle_enter_validtime(pstPworld);
		
		if(pstPworldInst->tStart + iEnterTime >= pstEnv->pstAppCtx->stCurr.tv_sec &&
			pstPworldInst->iPlayer + pstWaitQueue->iQueuerNum < pstPworld->stPworldDef.PlayerNum)
		{
			PworldApplyQueue *pstApplyQueue;

			if(!is_normal_battle_pworld(pstPworld))
			{
				return 0;
			}
			
			pstApplyQueue = get_pworld_apply_queue(pstEnv, pstPworld->stPworldDef.PworldID);
			if(NULL == pstApplyQueue || pstApplyQueue->iQueuerNum <= 0)
			{
				return 0;
			}
			
			if(is_battle_team_queue(pstEnv, pstPworld))
			{
				normal_battle_apply2wait(pstEnv, pstPworld, pstPworldInst, pstApplyQueue);
			}
			else if(is_battle_clan_queue(pstEnv, pstPworld))
			{
				return 0;//不会到这里
			}
			else
			{
				int iEnterNum = 0;
				
				if (pstApplyQueue->iQueuerNum  > (pstPworld->stPworldDef.PlayerNum -pstPworldInst->iPlayer))
				{
					iEnterNum = pstPworld->stPworldDef.PlayerNum -pstPworldInst->iPlayer;
				}
				else
				{
					iEnterNum = pstApplyQueue->iQueuerNum;
				}
				
				z_apply2wait_queue(pstEnv, pstPworld, pstApplyQueue, iEnterNum, 
							pstPworldInst->iPworldInstID, pstPworldInst->iWaitQueueIdx);
			}
		}
	}
	
	return 0;
}

int z_pwrold_clan_queue_heart_beat(ZONESVRENV* pstEnv, PworldClanQueue *pstClanQueue)
{
	SSPKG stSSPkg;
	SSCMDREQ* pstCmdReq = &stSSPkg.Body.CmdReq;
	int iBusID;
	
	iBusID = pstClanQueue->iBusID;
	
	memset(pstCmdReq,0,sizeof(*pstCmdReq));
	pstCmdReq->Cmd = SS_CMD_BATLLE_HEART_BEAT;

	pstCmdReq->Data.BatlleHeartBeat.BattleID = pstClanQueue->iBattleID;
	pstCmdReq->Data.BatlleHeartBeat.ClanID = pstClanQueue->stClanID;

	if (INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
	{
		Z_SSHEAD_INIT(&stSSPkg.Head, SS_CMD_REQ);
		z_ssmsg_send(pstEnv, &stSSPkg, iBusID);
	}
	else
	{
		BATLLEHEARTBEAT *pstBatlleHeartBeat = &pstCmdReq->Data.BatlleHeartBeat;
		z_battle_heart_beat(pstEnv,pstBatlleHeartBeat);
	}
	return 0;
}

int z_pworld_clan_queue_check(ZONESVRENV* pstEnv, PworldClanQueue *pstClanQueue)
{
	ZONEPWORLDINST* pstPworldInst;
	ZONEPWORLD *pstPworld;

	//同步下zone_svr的状态
	//if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
	//{
		z_pwrold_clan_queue_heart_beat(pstEnv,pstClanQueue);
	//}

	if (pstClanQueue->iQueueType == PWORLD_APPLY_QUEUE)
	{
		//报名阶段
	}
	else if (pstClanQueue->iQueueType == PWORLD_WAIT_QUEUE)
	{
		PworldCampPlayerInfo *pstPworldCampInfo = NULL;
		int iCmpPlayer;
		int iEnterTime;
		int i;
		Player* pstPlayer;
		QueueInfo* pstQueueInfo;
		PworldWaitQueue* pstWaitQueue;


		pstWaitQueue = get_pworld_wait_queue(pstEnv, pstClanQueue->iQueueIdx,
									pstClanQueue->iPworldInstID);

		if (!pstWaitQueue)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "get_pworld_wait_queue fail %d",
								pstClanQueue->iQueueIdx );
			return -1;
		}

		pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstClanQueue->iPworldInstID);
		if(pstPworldInst == NULL || pstPworldInst->bOver)
		{
			return 0;
		}

		if (pstClanQueue->iCampIdx >=0 && 
			pstClanQueue->iCampIdx < pstPworldInst->stPworldCamp.iCampNum)
		{
			pstPworldCampInfo = &pstPworldInst->stPworldCamp.stCampInfo[pstClanQueue->iCampIdx];
		}

		if (!pstPworldCampInfo)
		{
			//log ?
			return -1;
		}

		for(i = pstClanQueue->iQueuerNum-1; i>=0; i--)
		{
			pstQueueInfo = &pstClanQueue->stQueuerInfo[i];
			if (0 == pstQueueInfo->iInvite ||
				pstQueueInfo->tValidTime >= pstEnv->pstAppCtx->stCurr.tv_sec)
			{

				continue;
			}

			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstQueueInfo->iQueuerID);
			if(pstPlayer)
			{
				z_player_online_remove_queue(pstEnv, pstPlayer);
				if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
				{
					player_span_exit_pworld_queue_req(pstEnv, pstPlayer);
					//暴力清除
					z_free_player(pstEnv->pstAppCtx, pstEnv, pstPlayer);
				}
			}

			//更新阵营信息

			//pstPworldCampInfo->iPlayerNum--;
			//pstPworldCampInfo->iCampLv -= pstQueueInfo->iLevel;


			pstClanQueue->iQueuerNum--;

			if(i != pstClanQueue->iQueuerNum)
			{
				memcpy(&pstClanQueue->stQueuerInfo[i], &
					pstClanQueue->stQueuerInfo[pstClanQueue->iQueuerNum], 
					sizeof(pstClanQueue->stQueuerInfo[0]));
			}
		}

		if (pstClanQueue->iQueuerNum <= 0)
		{
			return 0;
		}
	
		//检查队列中与当前公会队列状态不一致的玩家
		//有位置的就放人进去
		

		pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
		if (pstPworld == NULL)
		{
			return -1;
		}
		

		//可进入的有效时间
		iEnterTime = get_battle_enter_validtime(pstPworld);

		iCmpPlayer = pstPworld->stPworldDef.PlayerNum/pstPworld->stPworldDef.CampNum;

		
		if(pstPworldInst->tStart + iEnterTime >= pstEnv->pstAppCtx->stCurr.tv_sec &&
			pstPworldCampInfo->iPlayerNum  < iCmpPlayer)
		{
			int i = pstClanQueue->iQueuerNum;
			int iClanQueueId = pstClanQueue - pstEnv->pstPworldQueue->stClanQueue;
			
			while(i--)
			{				
				Player* pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv,
								pstClanQueue->stQueuerInfo[i].iQueuerID);
				
				if(NULL == pstPlayer)
				{
					continue;
				}	

				if (pstClanQueue->stQueuerInfo[i].iInvite)
				{
					continue;
				}

				if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
				{
					if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
					{
						continue;
					}
				}
				else 
				{
					if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
					{
						continue;
					}
				}

				if ( 0 == player_clan_queue_invite(pstEnv,pstPworld,
							pstClanQueue->iPworldInstID,
									iClanQueueId,pstClanQueue->iCampIdx,
									pstPlayer,pstClanQueue->iQueueIdx))
				{
					pstClanQueue->stQueuerInfo[i].iInvite = 1;//已经邀请
					pstClanQueue->stQueuerInfo[i].tValidTime = 
						pstEnv->pstAppCtx->stCurr.tv_sec + INVITE_ENTER_PWORLD_TIME;
				}
			}	
		}
		

	}
	else
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "clan queue type err: %d",
								pstClanQueue->iQueueType);
	}

	
	
	return 0;
}

int z_pworld_clan_queue_match_check(ZONESVRENV* pstEnv,ZONEPWORLD *pstPworld,int iQueuerID)
{
	PworldClanQueue *pstClanQueue;
	int iCmpMinPlayer;

	iCmpMinPlayer = pstPworld->stPworldDef.MinPlayerNum/pstPworld->stPworldDef.CampNum;
	
	if (iQueuerID < 0 ||
		iQueuerID > MAX_PWORLD_CLAN_QUEUE)
	{
		return -1;
	}

	pstClanQueue = &pstEnv->pstPworldQueue->stClanQueue[iQueuerID];
	
	//pstClanQueue->iMatchStat 判断
	if (pstPworld->stPworldDef.MatchType == 1)
	{
		if (pstClanQueue->iMatchStat < PWORLD_SVR_MATCH_DOING)
		{
			return -1;
		}
		else
		{
			//return 0;//允许gm指令修改匹配状态
		}
	}
		
	if (pstClanQueue->iQueuerNum < iCmpMinPlayer)
	{
		return -1;
	}
				
	return 0;
}

//副本报名队列检查
//公会战区别对待,同一个公会的进入同一个wait queque
//z_pworld_apply_queue_check
int z_pworld_apply_queue_check(ZONESVRENV* pstEnv, PworldApplyQueue *pstApplyQueue,int iImmeCreateSwitch)
{
	ZONEIDX stIdx;
	ZONEPWORLD *pstPworld;
	ZONEPWORLDINST *pstTmpPworldInst;
	int iEnterNum = 0;
	int i;
	PworldWaitQueue *pstWaitQueue;
	BATTLEDEF *pstBattleDef;
	ACTIVEHELPER *pstActiveHelper;
	int iCurrSec = pstEnv->pstAppCtx->stCurr.tv_sec;
	int iEnterTime;
	
	stIdx.iID = pstApplyQueue->iPworldID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (pstPworld == NULL || pstPworld->stPworldDef.Type != PWORLD_TYPE_PLOY)
	{
		return -1;
	}
	
	pstBattleDef = z_find_battledef(pstEnv, pstPworld->stPworldDef.BattleID);
	if(NULL == pstBattleDef)
	{
		return -1;
	}

	if(pstBattleDef->EventLimit > 0)
	{
		GAMEEVENTDEF *pstEventDef = z_find_event_def(pstEnv, pstBattleDef->EventLimit, NULL);

		if(!pstEventDef)
		{
			return -1;
		}
		
		pstActiveHelper = z_find_active_help_def(pstEnv, pstEventDef->ReferActiveID, NULL);
		if (NULL == pstActiveHelper)
		{
			return -1;
		}

		//检查时间过期，提示一下
		if (0 > autoteam_check_active_running(pstEnv, pstActiveHelper))
		{
			z_clear_pworld_apply_queue(pstEnv, pstApplyQueue);
			return -1;
		}
	}

	//可进入的有效时间
	iEnterTime = get_battle_enter_validtime(pstPworld);
	
	if (pstPworld->stPworldDef.PolyType == PWORLD_POLY_SPAN_WORLD)
	{
		for (i=0; i<MAX_PWORLD_WAIT_QUEUE; i++)
		{
			pstWaitQueue = &pstEnv->pstPworldQueue->stWaitQueue[i];
			if (pstWaitQueue->iPworldID != (int)pstPworld->stPworldDef.PworldID)
			{
				continue;
			}

			// 超过进副本的时间,不邀请了
			if (pstWaitQueue->iCreateTime + iEnterTime< iCurrSec )
			{
				continue;
			}

			invite_player_enter_wait_queue(pstEnv, pstPworld, pstApplyQueue, i);
			
			if (pstApplyQueue->iQueuerNum == 0)
				break;
		}

		// 没副本可去  通知region
		if (pstApplyQueue->iQueuerNum > iImmeCreateSwitch)
		{
			z_pworld_queue_notify_region(pstEnv,pstApplyQueue);
		}
	}
	else if(is_normal_battle_pworld(pstPworld))
	{
		int iCampPlayerNum = 0;
		int iMinLevel = 0;

		if (pstPworld->stPworldDef.CampNum < 1)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "stPworldDef.CampNum = %d",
								pstPworld->stPworldDef.CampNum );
			return -1;
		}

		iCampPlayerNum = pstPworld->stPworldDef.PlayerNum/pstPworld->stPworldDef.CampNum;

		if (pstApplyQueue->iQueuerType == PWORLD_APPLY_UNIT_PLAYER)
		{
			if(pstApplyQueue->iQueuerNum < pstPworld->stPworldDef.CreateQueuerNum)
			{
				return 0;
			}

			//一方阵营人数必须大于报名最大单位的人数
			if(iCampPlayerNum < MAX_TEAM_MEMBER)
			{
				return 0;
			}
		}
		else if (pstApplyQueue->iQueuerType == PWORLD_APPLY_UNIT_CLAN)
		{
			int iMatchNum = 0;
			//PworldClanQueue* pstClanQueue;	//comment by paraunused.pl
			int iClanQueueID = 0;
			int iCmpMinPlayer = 0;
			

			iCmpMinPlayer = pstPworld->stPworldDef.MinPlayerNum/pstPworld->stPworldDef.CampNum;
			
/*
			if(pstApplyQueue->iQueuerNum < pstPworld->stPworldDef.CampNum)
			{
				return 0;
			}

*/
			for (i = 0; i < pstApplyQueue->iQueuerNum; i ++)
			{
				PworldClanQueue *pstClanQueue;
				
				iClanQueueID = pstApplyQueue->stQueuerInfo[i].iQueuerID;

				
				if (iClanQueueID < 0 ||
					iClanQueueID > MAX_PWORLD_CLAN_QUEUE)
				{
					continue;
				}

				pstClanQueue = &pstEnv->pstPworldQueue->stClanQueue[iClanQueueID];

				//这里也check下
				z_pworld_clan_queue_check(pstEnv,pstClanQueue);

				if (0 > z_pworld_clan_queue_match_check(pstEnv,pstPworld, iClanQueueID))
				{
					continue;
				}

				iMatchNum++;

				if (0 == iMinLevel)
				{
					iMinLevel = pstClanQueue->iSrcMaxLevel;
				}
				else
				{
					iMinLevel = min(iMinLevel,pstClanQueue->iSrcMaxLevel);
				}

				if (iMatchNum >= pstPworld->stPworldDef.CampNum)
				{
					break;
				}
				
			}

			//匹配的公会队列数必须足够
			if(iMatchNum < pstPworld->stPworldDef.CampNum)
			{
				return 0;
			}	

			
		}
		else
		{
			return 0;
		}

		pstTmpPworldInst = z_new_ploy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstPworld, pstPworld->stPworldDef.Timeout*60,iMinLevel);
		if(NULL == pstTmpPworldInst)
		{
			return -1;
		}

		//副本实例变量中,如果是公会战场则记录经验奖励

		if (pstBattleDef->AwardMul && pstBattleDef->AwardMulEventID)
		{
			GAMEEVENTDEF *pstEventDef = z_find_event_def(pstEnv, pstBattleDef->AwardMulEventID, NULL);

			if(pstEventDef)
			{
				if (z_event_check_active_runing(pstEnv,pstEventDef) >=0 )
				{
					//高倍经验时间
					pstTmpPworldInst->iAwardMul = pstBattleDef->AwardMul;
				}
			}
			
		}
		
		if(is_battle_team_queue(pstEnv, pstPworld))
		{
			normal_battle_apply2wait(pstEnv, pstPworld, pstTmpPworldInst, pstApplyQueue);
		}
		else if(is_battle_clan_queue(pstEnv, pstPworld))
		{
			//同个公会的一起进入
			//同一个公会的放到同一个公会queue中
			normal_battle_apply2wait(pstEnv, pstPworld, pstTmpPworldInst, pstApplyQueue);
		}
		else
		{
			if (pstApplyQueue->iQueuerNum > pstPworld->stPworldDef.PlayerNum)
			{
				iEnterNum = pstPworld->stPworldDef.PlayerNum;
			}
			else
			{
				iEnterNum = pstApplyQueue->iQueuerNum;
			}
			
			z_apply2wait_queue(pstEnv, pstPworld, pstApplyQueue, iEnterNum, 
						pstTmpPworldInst->iPworldInstID, pstTmpPworldInst->iWaitQueueIdx);
		}
	}
	
	return 0;
}

int z_pworld_queue_tick(ZONESVRENV* pstEnv)
{
	static unsigned int iCount = 0;
	int i;
	PworldWaitQueue *pstWaitQueue;
	PworldApplyQueue *pstApplyQueue;
	//PworldClanQueue *pstClanQueue;	//comment by paraunused.pl
	WorldPworldQueue *pstPworldQueue = pstEnv->pstPworldQueue;

	//每十秒
	if(iCount % 10 == 0)
	{
		//副本实例等待进入的队列
		for(i = 0; i<MAX_PWORLD_WAIT_QUEUE; i++)
		{
			pstWaitQueue = &pstPworldQueue->stWaitQueue[i];
			if(pstWaitQueue->iPworldInstID == 0)
			{
				continue;
			}

			if (	pstWaitQueue->iBattleBusID > 0 &&
				pstWaitQueue->iCreateTime + pstWaitQueue->iLiftTime*60 < pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				z_clear_pworld_wait_queue(pstEnv, i, pstWaitQueue->iPworldInstID);
				continue;
			}

			z_pworld_wait_queue_check(pstEnv, pstWaitQueue);
		}

		//副本排队队列
		for(i = 0; i<MAX_PWORLD_APPLY_QUEUE; i++)
		{
			pstApplyQueue = &pstPworldQueue->stApplyQueue[i];

			if(pstApplyQueue->iPworldID == 0 || pstApplyQueue->iQueuerNum == 0)
			{
				continue;
			}

			z_pworld_apply_queue_check(pstEnv, pstApplyQueue,0);
		}

		
		//不在这里检查
/*
		//副本公会队列队列
		for(i = 0; i<MAX_PWORLD_CLAN_QUEUE; i++)
		{
			pstClanQueue = &pstPworldQueue->stClanQueue[i];

			if(pstClanQueue->iPworldID == 0 || pstClanQueue->iQueuerNum == 0)
			{
				continue;
			}
			//z_pworld_clan_queue_check(pstEnv, pstClanQueue);
		}		

*/
	}

	iCount++;

	return 0;
}

static int get_min_camplv(ZONEPWORLDINST *pstZonePworldInst)
{
	int i,iIdx;
	int iLv;

	iIdx = 0;
	iLv = pstZonePworldInst->stPworldCamp.stCampInfo[0].iCampLv;
	
	for (i=1; i<pstZonePworldInst->stPworldCamp.iCampNum; i++)
	{
		if (pstZonePworldInst->stPworldCamp.stCampInfo[i].iCampLv < iLv)
		{
			iLv = pstZonePworldInst->stPworldCamp.stCampInfo[i].iCampLv;
			iIdx = i;
		}
	}

	return iIdx;
		
}

static int get_min_campnum(ZONEPWORLDINST *pstZonePworldInst)
{
	int i,iIdx;
	int iNum;

	iIdx = 0;
	iNum = pstZonePworldInst->stPworldCamp.stCampInfo[0].iPlayerNum;
	for (i=1; i<pstZonePworldInst->stPworldCamp.iCampNum; i++)
	{
		if (pstZonePworldInst->stPworldCamp.stCampInfo[i].iPlayerNum < iNum)
		{
			iNum = pstZonePworldInst->stPworldCamp.stCampInfo[i].iPlayerNum;
			iIdx = i;
		}
	}

	return iIdx;
}

int player_set_battle_info(ZONESVRENV * pstEnv, Player *pstPlayer, MapIndex *pstMapIndex,
	ZONEPWORLD *pstZonePworld, ZONEPWORLDINST *pstZonePworldInst, RESPOS *pstPos)
{
	int iCamp = 0, iPos = 0;
	BATTLEDEF *pstBattleDef = z_find_battledef(pstEnv, pstZonePworld->stPworldDef.BattleID);

	if (!pstBattleDef)
		return -1;

	switch(pstBattleDef->FightType)
	{
		case BATTLE_FIGHT_TYPE_AOSHAN:
			{	
				RolePworldQueue *pstRoleQueue = 
					z_get_player_online_queue(pstEnv, pstPlayer, pstZonePworld->stPworldDef.PworldID);
				
				if(NULL == pstRoleQueue || pstRoleQueue->iCamp <= 0)
				{
					return -1;
				}

				iCamp = pstRoleQueue->iCamp;
				iPos = iCamp -1;
			}
			break;
		case BATTLE_FIGHT_TYPE_CAMP_LIFT:
			{
				iCamp = get_min_camplv(pstZonePworldInst) + 1;
				iPos = iCamp -1;
			}
			break;
		case BATTLE_FIGHT_TYPE_RAND_LIFT:
			{
				iCamp = 0;
				iPos = (int)(RAND1(REVIVEPOS_NUM));
			}
			break;

		case BATTLE_FIGHT_TYPE_XUKONG:
			{
				iCamp = get_min_campnum(pstZonePworldInst) + 1;
				iPos = iCamp -1;
			}
			break;
			
		case BATTLE_FIGHT_TYPE_CLAN_WAR:
			{
				//获取玩家公会对应的阵营
				RolePworldQueue *pstRoleQueue = 
					z_get_player_online_queue(pstEnv, pstPlayer, pstZonePworld->stPworldDef.PworldID);
				
				if(NULL == pstRoleQueue || pstRoleQueue->iCamp <= 0)
				{
					return -1;
				}

				iCamp = pstRoleQueue->iCamp;
				iPos = iCamp -1;
			}
			break;
		
		default:
			break;
	}

	//阵营统计信息
	if(iCamp > 0 && iCamp<= MAX_CAMP_NUM)
	{
		pstZonePworldInst->stPworldCamp.stCampInfo[iCamp-1].iCampLv += pstPlayer->stRoleData.Level;
		pstZonePworldInst->stPworldCamp.stCampInfo[iCamp-1].iPlayerNum++;
	}

	//出生点
	if(iPos>=0 && iPos<REVIVEPOS_NUM )
	{
		if (pstMapIndex->stMapDef.RevivePs[iPos].X > 0 && pstMapIndex->stMapDef.RevivePs[iPos].Y > 0)
		{
			*pstPos = pstMapIndex->stMapDef.RevivePs[iPos];
		}
	}	

	pstPlayer->stOnline.stWarOL.cCampIdx = iCamp;
	pstPlayer->stOnline.stWarOL.iLevel = pstPlayer->stRoleData.Level;
	pstPlayer->stOnline.stWarOL.cPloyType = PWORLD_POLY_BATTLE;
	pstPlayer->stOnline.stWarOL.tFightTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	return 0;
}

void inset_limit_battle_info(ZONESVRENV* pstEnv, LIMITBATTLE *pstLimitBattle, BATTLEDEF *pstBattleDef)
{
	pstLimitBattle->BattleID = pstBattleDef->BattleID;
	pstLimitBattle->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstLimitBattle->Num = 1;
	pstLimitBattle->WinNum = 0;

	return ;
}

int battle_limit_info_get(ZONESVRENV* pstEnv, Player* pstPlayer)
{
	int i;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	CSPKG stPkg;
	CSBATTLELIMIT *pstBattleLimit = &stPkg.Body.BattleLimit;
	CSBATTLELIMITALL *pstAll = &pstBattleLimit->BattleLimitData.All;
	
	for (i=pstMiscInfo->LimitBattleNum-1; i>=0; i--)
	{
		if (IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitBattle[i].Time) == 0)
		{
			pstMiscInfo->LimitBattleNum--;
			if (i != pstMiscInfo->LimitBattleNum)
			{
				memmove(&pstMiscInfo->LimitBattle[i], &pstMiscInfo->LimitBattle[i+1], 
							(pstMiscInfo->LimitBattleNum - i) * sizeof(pstMiscInfo->LimitBattle[0]));
			}
			continue;
		}
	}

	pstBattleLimit->Type = BATTLE_LIMIT_TYPE_ALL;
	pstAll->LimitBattleNum = pstMiscInfo->LimitBattleNum;
	memcpy(&pstAll->LimitBattle[0], &pstMiscInfo->LimitBattle[0], pstAll->LimitBattleNum*sizeof(pstAll->LimitBattle[0]));

	Z_CSHEAD_INIT(&stPkg.Head, BATTLE_LIMIT_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

void player_insert_limit_battle(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLD * pstZonePworld)
{
	int i;
	int iIdx;
	CSPKG stPkg;
	CSBATTLELIMIT *pstBattleLimit = &stPkg.Body.BattleLimit;
	BATTLEDEF *pstBattleDef;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	LIMITBATTLE *pstLimitBattle = NULL;

	if(!is_normal_battle_pworld(pstZonePworld))
	{
		return ;
	}

	pstBattleDef = z_find_battledef(pstEnv, pstZonePworld->stPworldDef.BattleID);
	if(NULL == pstBattleDef)
	{
		return ;
	}
	
	iIdx = -1;
	for (i=0; i<pstMiscInfo->LimitBattleNum; i++)
	{
		if (pstMiscInfo->LimitBattle[i].BattleID == (int)pstBattleDef->BattleID)
		{
		
			if (IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitBattle[i].Time))
			{
				pstLimitBattle = &pstMiscInfo->LimitBattle[i];
				pstLimitBattle->Num++;
				goto _end;
			}
			else
			{
				iIdx = i;
				break;
			}
		}

		// 随便找一个可以替换的
		if (0 == IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitBattle[i].Time))
		{
			iIdx = i;
			continue;
		}
	}

	if (iIdx >= 0)
	{
		pstLimitBattle = &pstMiscInfo->LimitBattle[iIdx];
		inset_limit_battle_info(pstEnv, pstLimitBattle, pstBattleDef);
		goto _end;
	}
	else
	{
		if (pstMiscInfo->LimitBattleNum < MAX_LIMIT_BATTLE_NUM)
		{
			iIdx = pstMiscInfo->LimitBattleNum;
			memset(&pstMiscInfo->LimitBattle[iIdx], 0, sizeof(pstMiscInfo->LimitBattle[0]));

			pstLimitBattle = &pstMiscInfo->LimitBattle[iIdx];
			inset_limit_battle_info(pstEnv, pstLimitBattle, pstBattleDef);
			
			pstMiscInfo->LimitBattleNum++;
			goto _end;
		}
		else
		{
			memmove(&pstMiscInfo->LimitBattle[0], &pstMiscInfo->LimitBattle[1],  
					(MAX_LIMIT_BATTLE_NUM-1) * sizeof(pstMiscInfo->LimitBattle[0]));

			memset(&pstMiscInfo->LimitBattle[MAX_LIMIT_BATTLE_NUM-1], 0, sizeof(pstMiscInfo->LimitBattle[0]));

			pstLimitBattle = &pstMiscInfo->LimitBattle[MAX_LIMIT_BATTLE_NUM-1];
			inset_limit_battle_info(pstEnv, pstLimitBattle, pstBattleDef);
			
			goto _end;
		}
	}

	return ;
	
_end:
	
	if (pstLimitBattle)
	{
		pstBattleLimit->Type = BATTLE_LIMIT_TYPE_CHG;
		memcpy(&pstBattleLimit->BattleLimitData.Chg, pstLimitBattle, sizeof(pstBattleLimit->BattleLimitData.Chg));

		Z_CSHEAD_INIT(&stPkg.Head, BATTLE_LIMIT_INFO);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	
	return ;
}

int battle_limit_win_add(Player *pstPlayer, int iBattleID)
{
	int i;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;

	for (i=pstMiscInfo->LimitBattleNum-1; i>=0; i--)
	{	
		if (pstMiscInfo->LimitBattle[i].BattleID == iBattleID)
		{
			pstMiscInfo->LimitBattle[i].WinNum++;
			return pstMiscInfo->LimitBattle[i].WinNum;
		}
	}
	
	return 0;
}

int battle_limit_win_get(Player *pstPlayer, int iBattleID)
{
	int i;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;

	for (i=pstMiscInfo->LimitBattleNum-1; i>=0; i--)
	{	
		if (pstMiscInfo->LimitBattle[i].BattleID == iBattleID)
		{
			return pstMiscInfo->LimitBattle[i].WinNum;
		}
	}
	
	return 0;
}

//战场进入次数检查
int z_check_battle_enter_num(ZONESVRENV * pstEnv, Player *pstPlayer, ZONEPWORLD * pstZonePworld, int iNotify)
{
	int i;
	BATTLEDEF *pstBattleDef;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;

	if(!is_normal_battle_pworld(pstZonePworld))
	{
		return 0;
	}

	pstBattleDef = z_find_battledef(pstEnv, pstZonePworld->stPworldDef.BattleID);
	if(NULL == pstBattleDef)
	{
		return 0;
	}
	
	//检查每日副本次数
	for (i=pstMiscInfo->LimitBattleNum-1; i>=0; i--)
	{
		if (0 == IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitBattle[i].Time) )
		{
			pstMiscInfo->LimitBattleNum--;
			if (i != pstMiscInfo->LimitBattleNum)
			{
				memmove(&pstMiscInfo->LimitBattle[i], &pstMiscInfo->LimitBattle[i+1],  (pstMiscInfo->LimitBattleNum - i) * sizeof(pstMiscInfo->LimitBattle[0]));
			}
			
			continue;
		}
		
		if (pstMiscInfo->LimitBattle[i].BattleID == (int)pstBattleDef->BattleID)
		{
			if (	0 < pstBattleDef->NumEveryDay &&
				pstMiscInfo->LimitBattle[i].Num >= pstBattleDef->NumEveryDay)
			{
				goto _error;
			}
		}
	}

	return 0;

_error:
	
	if(iNotify)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PWORLD117, pstBattleDef->Name);
	}
	return -1;
}

int battle_ploy_range(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, 
							BATTLEDEF *pstBattleDef, Player *pstPlayer)
{
	ROLERANGEINFOENTRY *pstRoleRangeInfoEntry;
	WarFieldOL *pstOL = &pstPlayer->stOnline.stWarOL;
	
	switch(pstBattleDef->FightType)
	{
		case BATTLE_FIGHT_TYPE_CAMP_LIFT:
			{
				if (pstOL->iContinueKill >= SURVIVE_SUPERGOD_NUM)
				{
					pstRoleRangeInfoEntry = range_add_range_info_val(pstEnv, pstPlayer, RANGE_MAIN_BATTLE, 
																	RANGE_MAIN_BATTLE_SUB_SCSURVIVE_GOD, 1);
					if (pstRoleRangeInfoEntry)
					{
						range_battle_set(pstEnv, pstPlayer, RANGE_MAIN_BATTLE_SUB_SCSURVIVE_GOD, 
							pstPworldInst->stPworldselect.PworldGrade, 
							pstBattleDef->Name, pstRoleRangeInfoEntry->Val);
					}
				}

				if (pstOL->iKillNum > 0)
				{
					pstRoleRangeInfoEntry = range_add_range_info_val(pstEnv, pstPlayer, RANGE_MAIN_BATTLE, 
																		RANGE_MAIN_BATTLE_SUB_SCSURVIVE_KILLER, pstOL->iKillNum);
					if (pstRoleRangeInfoEntry)
					{
						range_battle_set(pstEnv, pstPlayer, RANGE_MAIN_BATTLE_SUB_SCSURVIVE_KILLER, 
							pstPworldInst->stPworldselect.PworldGrade, 
							pstBattleDef->Name, pstRoleRangeInfoEntry->Val);
					}
				}

				if (pstOL->iContinueKill > 0)
				{
					range_battle_set(pstEnv, pstPlayer, RANGE_MAIN_BATTLE_SUB_SCSURVIVE_NODEAD, 
							pstPworldInst->stPworldselect.PworldGrade, 
							pstBattleDef->Name, pstOL->iContinueKill);
				}

				if (pstPlayer->stRoleData.MiscInfo.KuangShaWinNum > 0)
				{
					range_battle_set(pstEnv, pstPlayer, RANGE_MAIN_BATTLE_SUB_KUANGSHA_WIN_NUM, 
							pstPworldInst->stPworldselect.PworldGrade, 
							pstBattleDef->Name, pstPlayer->stRoleData.MiscInfo.KuangShaWinNum);
				}
			}
			break;
		case BATTLE_FIGHT_TYPE_AOSHAN:
			{
				if (pstOL->iContinueKill >= SURVIVE_SUPERGOD_NUM)
				{
					pstRoleRangeInfoEntry = range_add_range_info_val(pstEnv, pstPlayer, RANGE_MAIN_BATTLE, 
																	RANGE_MAIN_BATTLE_SUB_LZSURVIVE_GOD, 1);
					if (pstRoleRangeInfoEntry)
					{
						range_battle_set(pstEnv, pstPlayer, RANGE_MAIN_BATTLE_SUB_LZSURVIVE_GOD, 
							pstPworldInst->stPworldselect.PworldGrade, 
							pstBattleDef->Name, pstRoleRangeInfoEntry->Val);
					}
				}

				if (pstOL->iKillNum > 0)
				{
					pstRoleRangeInfoEntry = range_add_range_info_val(pstEnv, pstPlayer, RANGE_MAIN_BATTLE, 
																		RANGE_MAIN_BATTLE_SUB_LZSURVIVE_KILLER, pstOL->iKillNum);
					if (pstRoleRangeInfoEntry)
					{
						range_battle_set(pstEnv, pstPlayer, RANGE_MAIN_BATTLE_SUB_LZSURVIVE_KILLER, 
							pstPworldInst->stPworldselect.PworldGrade, 
							pstBattleDef->Name, pstRoleRangeInfoEntry->Val);
					}
				}

				if (pstOL->iContinueKill > 0)
				{
					range_battle_set(pstEnv, pstPlayer, RANGE_MAIN_BATTLE_SUB_LZSURVIVE_NODEAD, 
							pstPworldInst->stPworldselect.PworldGrade, 
							pstBattleDef->Name, pstOL->iContinueKill);
				}

				if (pstPlayer->stRoleData.MiscInfo.LvZhouWinNum > 0)
				{
					range_battle_set(pstEnv, pstPlayer, RANGE_MAIN_BATTLE_SUB_LZWIN_NUM, 
							pstPworldInst->stPworldselect.PworldGrade, 
							pstBattleDef->Name, pstPlayer->stRoleData.MiscInfo.LvZhouWinNum);
				}
			}
			break;	
		//todo:排行榜
		case BATTLE_FIGHT_TYPE_CLAN_WAR:
			{
/*
				if (pstOL->iContinueKill >= SURVIVE_SUPERGOD_NUM)
				{
					pstRoleRangeInfoEntry = range_add_range_info_val(pstEnv, pstPlayer, RANGE_MAIN_BATTLE, 
																	RANGE_MAIN_BATTLE_SUB_SCSURVIVE_GOD, 1);
					if (pstRoleRangeInfoEntry)
					{
						range_battle_set(pstEnv, pstPlayer, RANGE_MAIN_BATTLE_SUB_SCSURVIVE_GOD, 
							pstPworldInst->stPworldselect.PworldGrade, 
							pstBattleDef->Name, pstRoleRangeInfoEntry->Val);
					}
				}

				if (pstOL->iKillNum > 0)
				{
					pstRoleRangeInfoEntry = range_add_range_info_val(pstEnv, pstPlayer, RANGE_MAIN_BATTLE, 
																		RANGE_MAIN_BATTLE_SUB_SCSURVIVE_KILLER, pstOL->iKillNum);
					if (pstRoleRangeInfoEntry)
					{
						range_battle_set(pstEnv, pstPlayer, RANGE_MAIN_BATTLE_SUB_SCSURVIVE_KILLER, 
							pstPworldInst->stPworldselect.PworldGrade, 
							pstBattleDef->Name, pstRoleRangeInfoEntry->Val);
					}
				}

				if (pstOL->iContinueKill > 0)
				{
					range_battle_set(pstEnv, pstPlayer, RANGE_MAIN_BATTLE_SUB_SCSURVIVE_NODEAD, 
							pstPworldInst->stPworldselect.PworldGrade, 
							pstBattleDef->Name, pstOL->iContinueKill);
				}

				if (pstPlayer->stRoleData.MiscInfo.KuangShaWinNum > 0)
				{
					range_battle_set(pstEnv, pstPlayer, RANGE_MAIN_BATTLE_SUB_KUANGSHA_WIN_NUM, 
							pstPworldInst->stPworldselect.PworldGrade, 
							pstBattleDef->Name, pstPlayer->stRoleData.MiscInfo.KuangShaWinNum);
				}

*/
			}
			break;
		default:
			break;
	}
	
	return 0;
}


/* 
	跨服战场中用到的一些(暂时无用。后面可能不需要了)
*/
static int swap_apply_queue_pos(QueueInfo *p1, QueueInfo *p2)
{
	QueueInfo stInfo;

	memcpy(&stInfo, p1, sizeof(stInfo));
	*p1 = *p2;
	*p2 = stInfo;
	return 0;
}

// 移动排队队列中的玩家 保证队伍一起进副本
int z_apply_queue_team_move(PworldApplyQueue* pstQueue, int *iEnterNum)
{
	int i;
	int iPreTeamNum = 0; 
	int iLastTeamNum = 0;
	int iPreSingleNum = 0;
	int iLastSingleNum = 0;
	int iSwapNum = 0;
	
	if (*iEnterNum <= 0 || *iEnterNum > MAX_APPLY_QUEUER )
		return 0;
	
	if (pstQueue->stQueuerInfo[(*iEnterNum)-1].ullTeamWID <= 0)
		return 0;

	if (pstQueue->iQueuerNum == *iEnterNum)
		return 0;

	// 队伍被切断了
	if (	pstQueue->stQueuerInfo[(*iEnterNum)-1].ullTeamWID >0 &&
		pstQueue->stQueuerInfo[*iEnterNum].ullTeamWID == pstQueue->stQueuerInfo[(*iEnterNum)-1].ullTeamWID)
	{
		for (i=0; i<pstQueue->iQueuerNum; i++)
		{
			if (i<(*iEnterNum))
			{
				if (pstQueue->stQueuerInfo[i].ullTeamWID == pstQueue->stQueuerInfo[(*iEnterNum)-1].ullTeamWID)
				{
					iPreTeamNum ++;	 	// 切断之前队伍人数
				}
				else
				{
					iPreSingleNum ++;		// 切断之前单人人数
				}
			}
			else
			{
				if(pstQueue->stQueuerInfo[i].ullTeamWID == pstQueue->stQueuerInfo[(*iEnterNum)-1].ullTeamWID)
				{
					iLastTeamNum ++;		// 切断之后队伍人数
				}
				else
				{
					iLastSingleNum ++;	// 切断之后单人人数
				}
			}
		}

		// 前面的单人比截断的队伍人数多  优先队伍进
		if (iPreSingleNum >= iLastTeamNum)
		{
			iSwapNum = 0;
			for (i=0; i<(*iEnterNum)-iPreTeamNum; i++)
			{
				// 找到一个单人
				if (pstQueue->stQueuerInfo[i].ullTeamWID <=0)
				{
					swap_apply_queue_pos(&pstQueue->stQueuerInfo[i], &pstQueue->stQueuerInfo[(*iEnterNum)+iSwapNum]);
					iSwapNum ++;
					if (iSwapNum == iLastTeamNum)
					{
						break;
					}
				}
			}
		}
		else
		{
			// 用单人替换队伍
			if (iLastSingleNum >= iPreTeamNum)
			{
				iSwapNum = 0;
				for (i=(*iEnterNum)+iLastTeamNum; i<pstQueue->iQueuerNum; i++)
				{
					// 找到一个单人
					if (pstQueue->stQueuerInfo[i].ullTeamWID <=0)
					{
						swap_apply_queue_pos(&pstQueue->stQueuerInfo[i], 
							&pstQueue->stQueuerInfo[(*iEnterNum)-iPreTeamNum+iSwapNum]);
						iSwapNum ++;
						if (iSwapNum == iPreTeamNum)
						{
							break;
						}
					}
				}
			}
			// 实在没办法了  两边都放不下，只能队伍下次在进了
			else
			{
				(*iEnterNum) -= iPreTeamNum;
			}

		}
		
	}
	return 0;
}

int invite_player_enter_wait_queue(ZONESVRENV* pstEnv, ZONEPWORLD *pstPworld, 
			PworldApplyQueue* PworldApplyQueue, int iWaitIdx)
{
	int iEnterNum = 0;
	PworldWaitQueue* pstWaitQueue = &pstEnv->pstPworldQueue->stWaitQueue[iWaitIdx];
	
	if (PworldApplyQueue->iQueuerNum <= (pstWaitQueue->iPlayerMax-pstWaitQueue->iPlayerNum-pstWaitQueue->iQueuerNum))
	{
		iEnterNum = PworldApplyQueue->iQueuerNum;
	}
	else
	{
		iEnterNum = pstWaitQueue->iPlayerMax-pstWaitQueue->iPlayerNum-pstWaitQueue->iQueuerNum;
	}

	z_apply_queue_team_move(PworldApplyQueue, &iEnterNum);

	if(iEnterNum > 0)
	{
		z_enter_wait_queue(pstEnv, pstPworld, pstWaitQueue->iPworldInstID,
							iWaitIdx, PworldApplyQueue->stQueuerInfo, 
							iEnterNum);

		if(PworldApplyQueue->iQueuerNum > iEnterNum)
		{
			memmove(&PworldApplyQueue->stQueuerInfo[0], &PworldApplyQueue->stQueuerInfo[iEnterNum], 
						sizeof(PworldApplyQueue->stQueuerInfo[0])*(PworldApplyQueue->iQueuerNum-iEnterNum));
		}
		PworldApplyQueue->iQueuerNum -= iEnterNum;
	}		
	return 0;
}

/* 
	end 跨服战场中用到的一些(暂时无用。后面可能不需要了)
*/

int battle_check_apply_single(ZONESVRENV *pstEnv,Player *pstPlayer, ZONEPWORLD *pstZonePworld)
{
	if (pstPlayer->stRoleData.Level > pstZonePworld->stPworldDef.MaxLv ||
			pstPlayer->stRoleData.Level < pstZonePworld->stPworldDef.MinLv)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_TIP, ZONEERR_PWORLD118);
		return -1;
	}

	/*
	if( pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN )
	{
		z_sys_strf(pstEnv,pstPlayer,SYS_TIP, "当前不在登陆状态,不能进入战场");
		return -1;
	}
	*/

	if (battle_enter_up_time_limit(pstEnv,pstPlayer, 0) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_TIP, ZONEERR_PWORLD119);
		return -1;
	}

	if(0 > z_check_battle_enter_num(pstEnv, pstPlayer, pstZonePworld, 0))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_TIP, 
			ZONEERR_PWORLD120,
			pstZonePworld->stPworldDef.Name);
		return -1;
	}

	// 思过厅中的黑名玩家不让组队进副本
	if (pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal >= PLAYER_PK_PUBLIC_ENEMY &&
		player_is_zuolao(pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PWORLD121);
		return -1;
	}

	//末世巅峰准备地图和决赛副本中不能战场报名
	if (pstPlayer->stRoleData.Map == SHZ_READY_MAP_ID || pstPlayer->stRoleData.Map == ZHZ_READY_MAP_ID ||
		pstPlayer->stRoleData.Map == JNZ_READY_MAP_ID || pstPlayer->stRoleData.Map == LHS_READY_MAP_ID ||
		pstPlayer->stRoleData.Map == LSZ_READY_MAP_ID || pstPlayer->stRoleData.Map == HMZ_READY_MAP_ID ||
		pstPlayer->stRoleData.Map == SYZ_READY_MAP_ID || pstPlayer->stRoleData.Map == YLS_READY_MAP_ID ||
		(GET_PWORLDID(pstPlayer->stRoleData.Map) == DIANFENG_PWORLD_ID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_DIANFENG24);
		return -1;
	}

	return 0;
}

int battle_check_apply_clan(ZONESVRENV *pstEnv,Player *pstPlayer, ZONEPWORLD *pstZonePworld,
				Player** apstPlayer, int *piPlayer)
{
	//前面已经check过了
	apstPlayer[*piPlayer] = pstPlayer;	
	(*piPlayer)++;
	return 0;
	UNUSED(pstZonePworld);	//add by paraunused.pl
	UNUSED(apstPlayer);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(piPlayer);	//add by paraunused.pl
}

int battle_check_apply_team(ZONESVRENV *pstEnv,Player *pstPlayer, ZONEPWORLD *pstZonePworld,
				Player** apstPlayer, int *piPlayer)
{
	int i;
	int iRet = 0;
	ZoneTeam *pstZoneTeam;
	TEAMMEMBER *pstCap;
	Player *pstTeamPlayer;
	SpanOL *pstSpanOnline;

	*piPlayer = 0;
	 
	pstZoneTeam = player_team_get(pstEnv,pstPlayer);
	if (NULL == pstZoneTeam)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_TIP, ZONEERR_PWORLD122);
		return -1;
	}
	
	pstCap = player_team_get_captain(pstZoneTeam);
	if (!pstCap)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_TIP, ZONEERR_PWORLD123);
		return -1;
	}

	if (strcmp(pstCap->MemberName, pstPlayer->stRoleData.RoleName) != 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_TIP, ZONEERR_PWORLD124);
		return -1;
	}

	pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
	if(pstSpanOnline->stApply.iSpartaApply == SPAN_APPLY_ING)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE110);
		return -1;
	}

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstTeamPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstTeamPlayer)
		{
			if (battle_enter_up_time_limit(pstEnv,pstTeamPlayer, 0) < 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, 
					ZONEERR_PWORLD125 ,pstTeamPlayer->stRoleData.RoleName);
				//z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, 
					//"%s 处于蓄势待发状态,暂时不能报名战场" ,pstTeamPlayer->stRoleData.RoleName);
				iRet = -1;
				continue;
			}
			
			if (z_player_find_status(pstTeamPlayer, STATUS_ID_RUNAWAY) )
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, 
					ZONEERR_PWORLD126 ,pstTeamPlayer->stRoleData.RoleName);
				//z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, 
					//"%s 有 捣乱者 状态! 不能报名排队!" ,pstTeamPlayer->stRoleData.RoleName);
				iRet = -1;
				continue;
			}
			
			// 判断所有玩家的等级
			if (pstTeamPlayer->stRoleData.Level > pstZonePworld->stPworldDef.MaxLv ||
				pstTeamPlayer->stRoleData.Level < pstZonePworld->stPworldDef.MinLv)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_TIP, ZONEERR_PWORLD127,pstTeamPlayer->stRoleData.RoleName);
				//z_sys_strf(pstEnv,pstPlayer,SYS_MIDDLE, "%s 等级不符合，无法进入战场",pstTeamPlayer->stRoleData.RoleName);
				iRet = -1;
				continue;
			}

			if( pstTeamPlayer->eStatus == PLAYER_STATUS_ROLE_NOTHING)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_TIP, ZONEERR_PWORLD128,pstTeamPlayer->stRoleData.RoleName);
				//z_sys_strf(pstEnv,pstPlayer,SYS_MIDDLE, "%s 正在战场中,不能报名",pstTeamPlayer->stRoleData.RoleName);
				iRet = -1;
				continue;
			}

			/*
			if( pstTeamPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN )
			{
				z_sys_strf(pstEnv,pstPlayer,SYS_TIP, "%s 不再登陆状态,不能报名",pstTeamPlayer->stRoleData.RoleName);
				//z_sys_strf(pstEnv,pstPlayer,SYS_MIDDLE, "%s 不再登陆状态,不能报名",pstTeamPlayer->stRoleData.RoleName);
				iRet = -1;
				continue;
			}*/

			if( pstTeamPlayer->stRoleData.Map >= PWORLD_MAPID_BASE )
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_TIP, ZONEERR_PWORLD129, pstTeamPlayer->stRoleData.RoleName);
				//z_sys_strf(pstEnv,pstPlayer,SYS_MIDDLE, "%s 正在副本中,不能报名",pstTeamPlayer->stRoleData.RoleName);
				iRet = -1;
				continue;
			}

			// 思过厅中的黑名玩家不让组队进副本
			if (pstTeamPlayer->stRoleData.MiscInfo.PkInfo.PkVal >= PLAYER_PK_PUBLIC_ENEMY &&
				player_is_zuolao(pstTeamPlayer))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PWORLD130);
				//z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "思过厅中黑名玩家不能报名战场");
				iRet = -1;
				continue;
			}

			//末世巅峰准备地图和决赛副本中不能战场报名
			if (pstTeamPlayer->stRoleData.Map == SHZ_READY_MAP_ID || pstTeamPlayer->stRoleData.Map == ZHZ_READY_MAP_ID ||
				pstTeamPlayer->stRoleData.Map == JNZ_READY_MAP_ID || pstTeamPlayer->stRoleData.Map == LHS_READY_MAP_ID ||
				pstTeamPlayer->stRoleData.Map == LSZ_READY_MAP_ID || pstTeamPlayer->stRoleData.Map == HMZ_READY_MAP_ID ||
				pstTeamPlayer->stRoleData.Map == SYZ_READY_MAP_ID || pstTeamPlayer->stRoleData.Map == YLS_READY_MAP_ID ||
				(GET_PWORLDID(pstTeamPlayer->stRoleData.Map) == DIANFENG_PWORLD_ID))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_DIANFENG25,pstTeamPlayer->stRoleData.RoleName);
				iRet = -1;
				continue;
			}

			pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
			if(pstSpanOnline->stApply.iSpartaApply == SPAN_APPLY_ING)
			{
				z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE111,
					pstTeamPlayer->stRoleData.RoleName);
				return -1;
			}

			if(0 > z_check_battle_enter_num(pstEnv, pstTeamPlayer, pstZonePworld, 0))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_TIP, 
					ZONEERR_PWORLD131,
					pstTeamPlayer->stRoleData.RoleName,
					pstZonePworld->stPworldDef.Name);

				/*z_sys_strf(pstEnv,pstPlayer,SYS_MIDDLE, 
					"%s的%s进入次数已达到每天的次数上限",
					pstTeamPlayer->stRoleData.RoleName,
					pstZonePworld->stPworldDef.Name);*/
				
				iRet = -1;
				continue;
			}

			apstPlayer[(*piPlayer) ++] = pstTeamPlayer;
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_TIP, ZONEERR_PWORLD132, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);

			//z_sys_strf(pstEnv, pstPlayer,SYS_MIDDLE, "%s不在线,不能进行报名", pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
			iRet = -1;
			continue;
		}
	}

	return iRet;
}

int battle_check_apply_player(ZONESVRENV *pstEnv,Player *pstPlayer, 
				Player** apstPlayer, int *piPlayer,ZONEPWORLD *pstZonePworld)
{
	ZoneTeam *pstZoneTeam;
	
	*piPlayer = 0;
	
	pstZoneTeam = player_team_get(pstEnv,pstPlayer);
	if (NULL == pstZoneTeam)
	{
		if (0 > battle_check_apply_single(pstEnv, pstPlayer, pstZonePworld))
		{
			return -1;
		}
		
		apstPlayer[(*piPlayer) ++] = pstPlayer;		// 单人报名
	}
	else
	{
		
		if(0 > battle_check_apply_team(pstEnv, pstPlayer, pstZonePworld, apstPlayer, piPlayer))
		{
			return -1;
		}
	}
	return 0;
}

int player_enter_pworld_clan_queue(ZONESVRENV* pstEnv, Player *pstPlayer, 
									Player ** ppstPlayer, int iPlayer,
									ZONEPWORLD *pstZonePworld,PworldClanQueue *pstClanQueue)
{
	int i;
	CSPKG stPkg;
	CSPWORLDSVR *pstPworldSvr = &stPkg.Body.PworldSvr;
	CSENTERPWORLDQUEUE *pstEnterQueue = &pstPworldSvr->SvrData.EnterPworldQueue;
	//CSINVITEENTERPWORLD *pstInvite = &pstPworldSvr->SvrData.InviteEnterPworld;	//comment by paraunused.pl
	//CSPWORLDMATCHSTATUS *pstMatch = &pstPworldSvr->SvrData.MatchStatus;	//comment by paraunused.pl
	PworldWaitQueue* pstWaitQueue = NULL;
	ZONEPWORLDINST* pstPworldInst = NULL;
	PworldCampPlayerInfo *pstPworldCampInfo = NULL;
	int iOldMatchStat;
	PWORLDDEF *pstDef = &pstZonePworld->stPworldDef;
	int iCampPlayerNum = 0;

	if (iPlayer < 1)
	{
		return -1;
	}

	if ((pstClanQueue->iQueueType != PWORLD_APPLY_QUEUE) && 
		(pstClanQueue->iQueueType != PWORLD_WAIT_QUEUE))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "unknown pstClanQueue->iQueueType = %d",
			pstClanQueue->iQueueType);
		return -1;
	}

	// 保证一个队伍同时报名
	if(pstClanQueue->iQueuerNum + iPlayer >  MAX_CLAN_QUEUER)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_BUSY);
		return -1;
	}

	if (pstZonePworld->stPworldDef.CampNum > 0)
	{
		iCampPlayerNum = pstZonePworld->stPworldDef.PlayerNum/pstZonePworld->stPworldDef.CampNum;
	}

	if(pstClanQueue->iQueueType == PWORLD_WAIT_QUEUE)
	{
		
		pstWaitQueue = get_pworld_wait_queue(pstEnv, pstClanQueue->iQueueIdx,
									pstClanQueue->iPworldInstID);

		if (!pstWaitQueue)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "get_pworld_wait_queue fail %d",
								pstClanQueue->iQueueIdx );
			return -1;
		}
		
		pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstClanQueue->iPworldInstID);
		if(pstPworldInst == NULL || pstPworldInst->bOver)
		{
			return 0;
		}
		
		if (pstClanQueue->iCampIdx >=0 && 
			pstClanQueue->iCampIdx < pstPworldInst->stPworldCamp.iCampNum)
		{
			pstPworldCampInfo = 
				&pstPworldInst->stPworldCamp.stCampInfo[pstClanQueue->iCampIdx];
		}

		if (!pstPworldCampInfo)
		{
			//log ?
			return -1;
		}

		if (pstClanQueue->iQueuerNum + iPlayer + pstPworldCampInfo->iPlayerNum >  
			iCampPlayerNum)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_RONGYAN12);
			return -1;
		}
	}
	else
	{
		if (pstClanQueue->iQueuerNum + iPlayer > iCampPlayerNum)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_RONGYAN12);
			return -1;
		}
	}

	

	// 先看看是不是已经报名了
	// 把报名的队员从队列里面删除 
	for (i=0; i<iPlayer; i++)
	{
		QueueInfo *pstQueueInfo;

		g_ApplyIng = 1;
		z_exit_pworld_queue(pstEnv, ppstPlayer[i], ppstPlayer[i]->stOnline.stPworldQueue.iPworldID, 1);
		g_ApplyIng = 0;
	
		ppstPlayer[i]->stOnline.stPworldQueue.iQueueType 	= PWORLD_CLAN_QUEUE;
		ppstPlayer[i]->stOnline.stPworldQueue.iPworldID 		= pstClanQueue->iPworldID;
		ppstPlayer[i]->stOnline.stPworldQueue.iPworldInstID 	= 0;
		ppstPlayer[i]->stOnline.stPworldQueue.iQueueIdx = pstClanQueue -
			pstEnv->pstPworldQueue->stClanQueue;

		pstQueueInfo = &pstClanQueue->stQueuerInfo[pstClanQueue->iQueuerNum];
		memset(pstQueueInfo, 0,sizeof(*pstQueueInfo));
		
		pstQueueInfo->iQueuerID = ppstPlayer[i]->iMemID;
		pstQueueInfo->iLevel = ppstPlayer[i]->stRoleData.Level;
		

		// 大于1肯定是队伍报名  否则是单人报名
		if (iPlayer > 1)
		{
			pstQueueInfo->iTeamIdx = 
									ppstPlayer[i]->stRoleData.MiscInfo.TeamIdx;
			pstQueueInfo->ullTeamWID = 
									ppstPlayer[i]->stRoleData.MiscInfo.TeamID;
		}


		//如果在wait queue 中
		if(pstWaitQueue && pstPworldCampInfo)
		{			
			ppstPlayer[i]->stOnline.stPworldQueue.iPworldInstID = pstClanQueue->iPworldInstID;
			ppstPlayer[i]->stOnline.stPworldQueue.iCamp = pstClanQueue->iCampIdx +1;

			//更新阵营信息
			//pstPworldCampInfo->iPlayerNum++;
			//pstPworldCampInfo->iCampLv += pstQueueInfo->iLevel;

		}


		pstClanQueue->iQueuerNum++;
		
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, ppstPlayer[i], SYS_MIDDLE2, ZONEERR_BATTLE_SVR2);


	}

	

	



	//排队成功
	pstPworldSvr->SvrType = PWORLD_SVR_ENTER_QUEUE;
	pstEnterQueue->PworldID = pstClanQueue->iPworldID;

	Z_CSHEAD_INIT(&stPkg.Head, PWORLD_SVR);
	
	z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, ppstPlayer,
                                iPlayer, &stPkg);

	//匹配状态
	if (pstDef->MatchType)
	{
		
		
		iOldMatchStat = pstClanQueue->iMatchStat;
		if (iOldMatchStat < PWORLD_SVR_MATCH_CAN &&
			pstDef->CampNum > 0)
		{
			
			if (pstClanQueue->iQueuerNum >= pstDef->MinPlayerNum /pstDef->CampNum)
			{
				//pstClanQueue->iMatchStat = PWORLD_SVR_MATCH_CAN;
				pstClanQueue->iMatchStat = PWORLD_SVR_MATCH_DOING;
			}
		}
		//if (iOldMatchStat !=  pstClanQueue->iMatchStat)
		{
			//队列中的所有玩家都发
			player_pworld_clan_queue_all_match_stat(pstEnv,pstZonePworld, pstClanQueue);
		}
		//else
		//{		
			//player_pworld_clan_queue_match_stat(pstEnv,pstZonePworld,ppstPlayer,iPlayer,
			//			pstClanQueue,pstPworldCampInfo);
		//}
	}

	if (pstClanQueue->iQueueType == PWORLD_APPLY_QUEUE)
	{	
				
	}
	else if(pstClanQueue->iQueueType == PWORLD_WAIT_QUEUE)
	{
		//马上邀请?

	}

	
	
	return 0;
}

int z_player_apply_battle_pworld_queue(ZONESVRENV* pstEnv, Player *pstPlayer, 
											Player ** ppstPlayer, int iPlayer,
											ZONEPWORLD *pstZonePworld,
											int iTeamJoinType,
											int iSrcMaxLevel)
{
	int i;
	int iPworldId = pstZonePworld->stPworldDef.PworldID;
	PworldApplyQueue *pstApplyQueue;
	PworldApplyQueue *pstOldApplyQueue;
	PworldClanQueue *pstClanQueue = NULL;
	CSPKG stPkg;
	CSPWORLDSVR *pstPworldSvr = &stPkg.Body.PworldSvr;
	CSENTERPWORLDQUEUE *pstEnterQueue = &pstPworldSvr->SvrData.EnterPworldQueue;

	pstApplyQueue = get_pworld_apply_queue(pstEnv, iPworldId);
	if(NULL == pstApplyQueue)
	{
		pstApplyQueue = new_pworld_apply_queue(pstEnv, iPworldId,
			pstZonePworld->stPworldDef.ApplyUnitType);
	}

	if(NULL == pstApplyQueue)
	{
		return -1;
	}

	if (pstApplyQueue->iQueuerType == PWORLD_APPLY_UNIT_CLAN)
	{
		CLANINFO *pstInfo;

		pstInfo = &pstPlayer->stRoleData.MiscInfo.ClanInfo;

		pstClanQueue = get_pworld_clan_queue(pstEnv,iPworldId,
			pstInfo->ClanId.GID,pstApplyQueue);

		if (pstClanQueue)
		{
			int iClanQueueIdx = pstClanQueue - pstEnv->pstPworldQueue->stClanQueue;
			
			//确认clan queue在apply queue中
			if (pstClanQueue->iQueueType == PWORLD_APPLY_QUEUE )
			{
				if (pstClanQueue->iQueueIdx ==
				 		(pstApplyQueue - pstEnv->pstPworldQueue->stApplyQueue))
				{
					//do nothing
				}
				else
				{
					//已经排队,但不是同一个副本
					//如果是发起一个新的,旧的先clear掉
					if (iTeamJoinType == TEAM_JOIN_CLAN)
					{
						//再判断下公会官职
						if (pstInfo->Position == CLAN_POS_VICE_PRESIDENT ||
							pstInfo->Position == CLAN_POS_PRESIDENT)
						{
							
							pstOldApplyQueue = get_pworld_apply_queue(pstEnv, 
												pstClanQueue->iPworldID);
							//移除排队队列
							if(pstOldApplyQueue)
							{
								queuers_remove_applyqueue(pstOldApplyQueue, 1,
													&iClanQueueIdx);
							}
							
							z_clear_pworld_clan_queue(pstEnv,iClanQueueIdx,pstClanQueue->iPworldID);

							pstClanQueue = NULL;//下面new一个
						}
						else
						{
							z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE112);
							return -1;
						}
					
						
					}
					else
					{
						//下面会修正掉
					}
				}
			}
			else
			{
				//检查副本是否存在,不存在不能clear 掉
				ZONEPWORLDINST*  pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, 
													pstClanQueue->iPworldInstID);
				
				
				if(pstPworldInst == NULL || pstPworldInst->bOver)
				{
					z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE113);
					return -1;
/*
					z_clear_pworld_clan_queue(pstEnv,iClanQueueIdx,pstClanQueue->iPworldID);
					
					pstClanQueue = NULL;//下面new一个

*/
				}
			}
		}


		if (!pstClanQueue)
		{
			//再判断下公会官职
			if (pstInfo->Position == CLAN_POS_VICE_PRESIDENT ||
				pstInfo->Position == CLAN_POS_PRESIDENT)
			{
				int iBusID = player_master_busid_get(pstEnv, pstPlayer);
				
				pstClanQueue = new_pworld_clan_queue(pstEnv,iPworldId,
					&pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId,pstApplyQueue,
					iBusID,pstZonePworld->stPworldDef.BattleID);
				if (0 == iSrcMaxLevel)
				{
					iSrcMaxLevel = get_curr_max_level(pstEnv);
				}
				pstClanQueue->iSrcMaxLevel = iSrcMaxLevel;

				
				memset(&pstApplyQueue->stQueuerInfo[pstApplyQueue->iQueuerNum], 0, sizeof(pstApplyQueue->stQueuerInfo[0]));
				pstApplyQueue->stQueuerInfo[pstApplyQueue->iQueuerNum].iQueuerID = 
					pstClanQueue - pstEnv->pstPworldQueue->stClanQueue;

				pstApplyQueue->iQueuerNum++;
				


				if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
				{
					//通知zone_svr
					//用heartbeat同步,这里不用通知
				}
				z_pwrold_clan_queue_heart_beat(pstEnv,pstClanQueue);
			}
			else
			{
				z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE112);
				return -1;
			}
		}
		

		if (!pstClanQueue)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_BATTLE114);
			
			return -1;
		}

		return player_enter_pworld_clan_queue(pstEnv,pstPlayer,ppstPlayer,iPlayer,
					pstZonePworld, pstClanQueue);
	}
	else if (pstApplyQueue->iQueuerType == PWORLD_APPLY_UNIT_PLAYER)
	{	
		// 保证一个队伍同时报名
		if(pstApplyQueue->iQueuerNum + iPlayer >  MAX_APPLY_QUEUER)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_BUSY);
			return -1;
		}

		// 先看看是不是已经报名了
		// 把报名的队员从队列里面删除 
		for (i=0; i<iPlayer; i++)
		{
			g_ApplyIng = 1;
			z_exit_pworld_queue(pstEnv, ppstPlayer[i], ppstPlayer[i]->stOnline.stPworldQueue.iPworldID, 1);
			g_ApplyIng = 0;
	
			ppstPlayer[i]->stOnline.stPworldQueue.iQueueType 	= PWORLD_APPLY_QUEUE;
			ppstPlayer[i]->stOnline.stPworldQueue.iPworldID 		= iPworldId;
			ppstPlayer[i]->stOnline.stPworldQueue.iPworldInstID 	= 0;

			memset(&pstApplyQueue->stQueuerInfo[pstApplyQueue->iQueuerNum], 0, sizeof(pstApplyQueue->stQueuerInfo[0]));
			pstApplyQueue->stQueuerInfo[pstApplyQueue->iQueuerNum].iQueuerID = ppstPlayer[i]->iMemID;

			// 大于1肯定是队伍报名  否则是单人报名
			if (iPlayer > 1)
			{
				pstApplyQueue->stQueuerInfo[pstApplyQueue->iQueuerNum].iTeamIdx = 
																	ppstPlayer[i]->stRoleData.MiscInfo.TeamIdx;
				pstApplyQueue->stQueuerInfo[pstApplyQueue->iQueuerNum].ullTeamWID = 
																	ppstPlayer[i]->stRoleData.MiscInfo.TeamID;
			}

			pstApplyQueue->iQueuerNum++;
			
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, ppstPlayer[i], SYS_MIDDLE2, ZONEERR_BATTLE_SVR2);

			pstPworldSvr->SvrType = PWORLD_SVR_ENTER_QUEUE;
			pstEnterQueue->PworldID = iPworldId;

			Z_CSHEAD_INIT(&stPkg.Head, PWORLD_SVR);
			z_cltmsg_send(pstEnv, ppstPlayer[i], &stPkg);
		}
	}
	else
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_RONGYAN13);
		return -1;
	}

	return 0;
}

int player_leave_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_LEAVE_STAT;
	pstActionRes->Data.LeaveState.LeaveState= pstPlayer->stOnline.iLeaveStat;
		
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);

	return 0;
}

int leave_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	UNUSED(pstAppCtx);
	UNUSED(pstFrameHead);

	pstPlayer->stOnline.iLeaveStat = pstCsPkg->Body.LeaveClt.LeaveState;
	
	return player_leave_action(pstEnv, pstPlayer);
}

int player_clan_war_battle_add_fraction(ZONEPWORLDINST *pstPworldInst, Player *pstPlayer, int iFraction)
{
	//个人积分
	pstPlayer->stOnline.stWarOL.iOre += iFraction;

	//总积分
	pstPworldInst->stPworldBattleInfo.iFraction += iFraction;

	return 0;
}	

int player_aoshan_battle_add_fraction(ZONEPWORLDINST *pstPworldInst, Player *pstPlayer, int iFraction)
{
	//个人积分
	pstPlayer->stOnline.stWarOL.iOre += iFraction;

	//总积分
	pstPworldInst->stPworldBattleInfo.iFraction += iFraction;

	return 0;
}	

//分配积分
int player_aoshan_battle_assign_fraction(ZONESVRENV* pstEnv, ZONEPWORLD *pstZonePworld, 
                        ZONEPWORLDINST *pstPworldInst, Player *pstPlayer)
{
	int i;
	Player *pstAssit;
	Player *apstPlayer[MAX_VISIBLE_ANIMATE];
	int iPlayerNum;
	int iFraction = 0;
	int iAssitFraction = 0;

	iPlayerNum = 0;
	for (i = 0; i < pstPlayer->stOnline.iVAnimate; i++) 
	{
		pstAssit = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.aiVAnimate[i]);
		if(NULL == pstAssit || pstAssit->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}

		if(!is_same_camp_player(pstPlayer, pstAssit))
		{
			continue;
		}

		if(CS_STAT_DEAD & pstAssit->stOnline.State)
		{
			continue;
		}
		
		apstPlayer[iPlayerNum++] = pstAssit;
	}

	//计算杀人的积分
	iFraction = ceil(9.0/(iPlayerNum+1));
	if(iFraction <=0)
	{
		iFraction = 1;
	}

	//杀人者的积分
	player_aoshan_battle_add_fraction(pstPworldInst, pstPlayer, iFraction);
	
	normal_battle_svr_info(pstEnv, pstPlayer, pstZonePworld,pstPworldInst, 0, 0);

	//计算助攻的积分
	iAssitFraction = ceil(5.0/(iPlayerNum+1));
	if(iAssitFraction <=0)
	{
		iFraction = 1;
	}

	for(i=0; i<iPlayerNum; i++)
	{
		//助攻数
		apstPlayer[i]->stOnline.stWarOL.iAssistKill++;

		//助攻积分
		player_aoshan_battle_add_fraction(pstPworldInst, apstPlayer[i], iAssitFraction);

		normal_battle_svr_info(pstEnv, apstPlayer[i], pstZonePworld,pstPworldInst, 0, 0);
	}

	return 0;
}

int player_battle_limit_time_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	LIMITBATTLETIME *pstLimit = &pstPlayer->stRoleData.MiscInfo.LimitBattleTime;
	
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_BATTLE_LIMIT_TIME;
	pstActionRes->Data.BattleLimitTime.BeginTime = pstLimit->Time;
	pstActionRes->Data.BattleLimitTime.Num = pstLimit->Num;
	pstActionRes->Data.BattleLimitTime.MaxNum = BATTLE_HOURS_ENTER_LIMIT;
		
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int player_spansvr_battle_apply(ZONESVRENV* pstEnv, Player *pstPlayer,  CSJOINQUEUE *pstJoin, ZONEPWORLD *pstZonePworld)
{
	ZoneTeam *pstZoneTeam = NULL;
	Player *pstOne = NULL;
	int i = 0;
	SSPKG stPkg;
	SSBATTLEAPPLYQUEUE *pstBattleApplyQueue = &stPkg.Body.BattleApplyQueue;
	
	if( !(INST_MODE_ZONE_SVR == pstEnv->iInstMode &&
		pstEnv->pstConf->BattleFlag ) )
	{
		return -1;
	}

	pstBattleApplyQueue->PworldID = pstZonePworld->stPworldDef.PworldID;
	pstBattleApplyQueue->RoleNum = 0;

	pstBattleApplyQueue->SrcMaxLevel = get_curr_max_level(pstEnv);

	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if(pstZoneTeam && pstJoin->TeamJoin)
	{
		for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
		{
			pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
			if(pstOne)
			{
				if(0> player_span_apply_req(pstEnv, pstOne, SPAN_DST_TYPE_BATTLE))
				{
					continue;
				}

				pstBattleApplyQueue->RoleInfo[i].Uin = pstOne->stRoleData.Uin;
				pstBattleApplyQueue->RoleInfo[i].RoleID = pstOne->stRoleData.RoleID;
				pstBattleApplyQueue->RoleNum++;
			}			
		}
	}
	else
	{
		if(0> player_span_apply_req(pstEnv, pstPlayer, SPAN_DST_TYPE_BATTLE))
		{
			return -1;
		}

		pstBattleApplyQueue->RoleInfo[0].Uin = pstPlayer->stRoleData.Uin;
		pstBattleApplyQueue->RoleInfo[0].RoleID = pstPlayer->stRoleData.RoleID;
		pstBattleApplyQueue->RoleNum++;
	}

	Z_SSHEAD_INIT(&stPkg.Head, BATTLE_APPLY_QUEUE);
	z_ssmsg_send(pstEnv, &stPkg, pstEnv->iBattlePveBattleID);
	
	return 0;
}

int player_spansvr_battle_join_queue(ZONESVRENV * pstEnv, SSBATTLEAPPLYQUEUE *pstJoin)
{
	int iPlayer = 0, i;
	ZONEIDX stIdx;
	ZONEPWORLD * pstZonePworld;
	Player *pstPlayer = NULL;
	Player *pstTmpPlayer = NULL;
	Player *apstPlayer[MAX_TEAM_MEMBER];
	int iRet;
	
	for(i=0; i<pstJoin->RoleNum; i++)
	{
		pstTmpPlayer = player_get_by_uin(pstEnv, pstJoin->RoleInfo[i].Uin);
		if(!pstTmpPlayer)
		{
			continue;
		}

		if(pstTmpPlayer->stOnline.stSpanOnline.iStat != SPAN_STAT_APPLY ||
			pstTmpPlayer->stOnline.stSpanOnline.stInfo.stApply.ullRoleID != pstJoin->RoleInfo[i].RoleID)
		{
			continue;
		}

		apstPlayer[iPlayer] = pstTmpPlayer;
		iPlayer++;
		//第一个就是发起人
		if(iPlayer == 1)
		{
			pstPlayer = pstTmpPlayer;
		}
	}

	stIdx.iID = pstJoin->PworldID;
	stIdx.iPos = -1;
	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (NULL == pstZonePworld)
	{
		return -1;
	}

	if(!pstPlayer || iPlayer <= 0)
	{
		return -1;
	}

	iRet = z_player_apply_battle_pworld_queue(pstEnv, pstPlayer, apstPlayer,
									iPlayer, pstZonePworld,pstJoin->TeamJoin,
									pstJoin->SrcMaxLevel);

	if (iRet < 0)
	{
		
		if (pstJoin->TeamJoin == TEAM_JOIN_CLAN)
		{
			clan_span_exit_pworld_queue_req(pstEnv,
				&pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId,
				player_master_busid_get(pstEnv, pstPlayer));
		}
		
		for(i=0; i<iPlayer; i++)
		{
			player_span_exit_pworld_queue_req(pstEnv, apstPlayer[i]);


			//暴力清除
			z_free_player(pstEnv->pstAppCtx, pstEnv, apstPlayer[i]);

			
		}
	}
	
	return iRet;
}

int is_valid_aoshan_battle_award_player(ZONESVRENV* pstEnv, CSBATTLEAOSHANINFO *pstRoleInfo, int iIdx, int iMaxRole)
{
	if(pstRoleInfo->Kill + pstRoleInfo->AssistKill >= 25)
	{
		return 1;
	}
	else
	{
		//人数10以下，则最后一名无奖励，只剩1人。则这个人有奖励
		if(iMaxRole < 10)
		{
			if(iIdx != 0 && iIdx == iMaxRole-1)
			{
				return 0;
			}
		}
		else
		{
			if(iMaxRole >= 10 && (iIdx+1)*100/iMaxRole > 90)
			{
				return 0;
			}
		}
		
		return 1;
	}

	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int is_valid_clan_war_battle_award_player(ZONESVRENV* pstEnv, CSBATTLECLANWARINFO *pstRoleInfo, int iIdx, int iMaxRole)
{
	if(pstRoleInfo->Kill + pstRoleInfo->AssistKill >= 25)
	{
		return 1;
	}
	else
	{
		//人数10以下，则最后一名无奖励，只剩1人。则这个人有奖励
		if(iMaxRole < 10)
		{
			if(iIdx != 0 && iIdx == iMaxRole-1)
			{
				return 0;
			}
		}
		else
		{
			if(iMaxRole >= 10 && (iIdx+1)*100/iMaxRole > 90)
			{
				return 0;
			}
		}
		
		return 1;
	}

	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int is_valid_xukong_battle_award_player(ZONESVRENV* pstEnv, CSBATTLEXUKONGINFO *pstRoleInfo, int iIdx, int iMaxRole)
{
	if(pstRoleInfo->Kill + pstRoleInfo->AssistKill >= 25)
	{
		return 1;
	}
	else
	{
		//人数10以下，则最后一名无奖励，只剩1人。则这个人有奖励
		if(iMaxRole < 10)
		{
			if(iIdx != 0 && iIdx == iMaxRole-1)
			{
				return 0;
			}
		}
		else
		{
			if(iMaxRole >= 10 && (iIdx+1)*100/iMaxRole > 90)
			{
				return 0;
			}
		}
		
		return 1;
	}

	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int is_valid_lift_battle_award_player(ZONESVRENV* pstEnv, CSBATTLESURVIVALINFO *pstRoleInfo, int iIdx, int iMaxRole)
{
	if(pstRoleInfo->MaxKill + pstRoleInfo->AssistKill >= 25)
	{
		return 1;
	}
	else
	{
		//人数10以下，则最后一名无奖励，只剩1人。则这个人有奖励
		if(iMaxRole < 10)
		{
			if(iIdx != 0 && iIdx == iMaxRole-1)
			{
				return 0;
			}
		}
		else
		{
			if(iMaxRole >= 10 && (iIdx+1)*100/iMaxRole > 90)
			{
				return 0;
			}
		}
		
		return 1;
	}

	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

Monster *z_find_pworld_mon_byid(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, int iDefID)
{
	int i, j;
	int iDyn;
	Monster* pstMon;
	MapIndex *pstMapIdx;
	ZONEMAPINST* pstMapInst;
	AREADYN *pstAreaDyn;
	
	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPworldInst->astMapInstIdx[0]);
	if( NULL == pstMapInst )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_find_map_inst %d map inst fail", pstPworldInst->astMapInstIdx[0].iID);
		return NULL;
	}

	pstMapIdx	=	z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return NULL;
	}	

	//全地图找一遍
	for (i=0; i<pstMapInst->iAreaDyn; i++)
	{
		iDyn	 =	pstMapInst->aiAreaDyn[i];
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

		for(j=0; j<pstAreaDyn->iAnimate; j++)
		{
			if( j>=MAX_AREA_ANIMATE )
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d", pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
				break;
			}

			pstMon= z_id_monster(pstEnv->pstAppCtx, pstEnv, pstAreaDyn->aiAnimate[j]);
			if (pstMon && pstMon->iDefIdx == iDefID)
			{
				return pstMon;
			}
		}
	}

	return NULL;
}


Npc *z_find_pworld_npc_byid(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, int iDefID)
{
	int i, j;
	int iDyn;
	Npc* pstNpc;
	MapIndex *pstMapIdx;
	ZONEMAPINST* pstMapInst;
	AREADYN *pstAreaDyn;
	
	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPworldInst->astMapInstIdx[0]);
	if( NULL == pstMapInst )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_find_map_inst %d map inst fail", pstPworldInst->astMapInstIdx[0].iID);
		return NULL;
	}

	pstMapIdx	=	z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return NULL;
	}	

	//全地图找一遍
	for (i=0; i<pstMapInst->iAreaDyn; i++)
	{
		iDyn	 =	pstMapInst->aiAreaDyn[i];
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

		for(j=0; j<pstAreaDyn->iAnimate; j++)
		{
			if( j>=MAX_AREA_ANIMATE )
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d", pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
				break;
			}

			pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstAreaDyn->aiAnimate[j]);
			if (pstNpc && pstNpc->stMon.iDefIdx == iDefID)
			{
				return pstNpc;
			}
		}
	}

	return NULL;
}


int xukong_energy_update(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst)
{
	Npc *pstNpc;
	XUKONGPOSDEF *pstXuKongPosDef = NULL;
	int iPos = pstPworldInst->stPworldBattleInfo.stXuKongInfo.iCampVal1 - pstPworldInst->stPworldBattleInfo.stXuKongInfo.iCampVal2;
	
	if(iPos == 0)
	{
		pstXuKongPosDef = z_find_xukong_pos_def(pstEnv, 0, 1);
	}
	else if(iPos > 0)
	{
		pstXuKongPosDef = z_find_xukong_pos_def(pstEnv, 2, iPos);
	}
	else if(iPos < 0)
	{
		pstXuKongPosDef = z_find_xukong_pos_def(pstEnv, 1, -iPos);
	}

	if(!pstXuKongPosDef)
	{
		return -1;
	}

	pstNpc = z_find_pworld_npc_byid(pstEnv, pstPworldInst, XUKONG_NPC_ID);
	if(!pstNpc)
	{
		return -1;
	}

	if (pstNpc->stMon.cMoving)
	{
		z_monster_move_curr(pstEnv->pstAppCtx, pstEnv, &pstNpc->stMon, 0);
	}

	// 移动Monster到指定位置	
	pstNpc->stMon.stNextPos = pstXuKongPosDef->Pos;
	pstNpc->stMon.stForcedPos = pstXuKongPosDef->Pos;
	pstNpc->stMon.stInitPos = pstXuKongPosDef->Pos;

	pstNpc->stMon.bMoveType = MOVE_TYPE_FORCED; // 强制移动
	pstNpc->stMon.cMoving = 1;
	z_monster_move_notify(pstEnv->pstAppCtx, pstEnv, &pstNpc->stMon, CS_MOVE_WALK);
	TV_CLONE(pstNpc->stMon.stLastMoveTv, pstEnv->pstAppCtx->stCurr);		
	
	return 0;
}

int xukong_pworld_get_vcamp(ZONESVRENV * pstEnv, ZONEPWORLDINST *pstPworldInst)
{
	int iDist1, iDist2;
	Npc *pstNpc;
	ZONEMAPINST * pstMapInst;
	XUKONGPOSDEF *pstXuKongPosDef1, *pstXuKongPosDef2;
	
	pstNpc = z_find_pworld_npc_byid(pstEnv, pstPworldInst, XUKONG_NPC_ID);
	if(!pstNpc)
	{
		return 0;
	}

	pstXuKongPosDef1 = z_find_xukong_pos_def(pstEnv, 1, MAX_XUKONG_CAMP_POS);
	if(!pstXuKongPosDef1)
	{
		return 0;
	}

	pstXuKongPosDef2 = z_find_xukong_pos_def(pstEnv, 2, MAX_XUKONG_CAMP_POS);
	if(!pstXuKongPosDef2)
	{
		return 0;
	}

	if (pstNpc->stMon.cMoving)
	{
		z_monster_move_curr(pstEnv->pstAppCtx, pstEnv, &pstNpc->stMon, 0);
	}

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPworldInst->astMapInstIdx[0]);
	if(NULL == pstMapInst)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_find_map_inst %d map inst fail", pstPworldInst->astMapInstIdx[0].iID);
		return -1;
	}

	iDist1 = z_distance(&pstNpc->stMon.stCurrPos, &pstXuKongPosDef1->Pos);
	iDist2 = z_distance(&pstNpc->stMon.stCurrPos, &pstXuKongPosDef2->Pos);
	if(iDist1 < iDist2)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE, ZONEERR_BATTLE101,
			LJY_BATTLE_BLUE);
		return 2;
	}
	else
	{
		z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE, ZONEERR_BATTLE101,
			LJY_BATTLE_RED);
		return 1;
	}
	
	return 1;
}

int clan_war_pworld_tick(ZONESVRENV * pstEnv, BATTLEDEF * pstBattleDef,ZONEPWORLD *pstPworld, ZONEPWORLDINST *pstPworldInst)
{
	int i;
	PworldCampPlayerInfo *pstCampInfo;	

	//更新气势信息
	for (i = 0; i < MAX_CAMP_NUM; i++)
	{
		if (i >= pstPworldInst->stPworldCamp.iCampNum)
		{
			break;
		}
		
	
		pstCampInfo = &pstPworldInst->stPworldCamp.stCampInfo[i];
	
		pstCampInfo->CampVal = pstCampInfo->iKillNum + pstCampInfo->iPlayerNum;
	}

	normal_battle_svr_info(pstEnv, NULL, pstPworld,pstPworldInst, 3, 0);
	
/*	
	pstNpc = z_find_pworld_npc_byid(pstEnv, pstPworldInst, CLAN_WAR_NPC_ID);
	if(!pstNpc)
	{
		return -1;
	}

	pstXuKongPosDef1 = z_find_clan_war_pos_def(pstEnv, 1, MAX_CLAN_WAR_CAMP_POS);
	if(!pstXuKongPosDef1)
	{
		return -1;
	}

	pstXuKongPosDef2 = z_find_clan_war_pos_def(pstEnv, 2, MAX_CLAN_WAR_CAMP_POS);
	if(!pstXuKongPosDef2)
	{
		return -1;
	}

	if (pstNpc->stMon.cMoving)
	{
		z_monster_move_curr(pstEnv->pstAppCtx, pstEnv, &pstNpc->stMon, 0);
	}

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPworldInst->astMapInstIdx[0]);
	if(NULL == pstMapInst)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_find_map_inst %d map inst fail", pstPworldInst->astMapInstIdx[0].iID);
		return -1;
	}

	if(pstNpc->stMon.stCurrPos.X == pstXuKongPosDef2->Pos.X &&
		pstNpc->stMon.stCurrPos.Y == pstXuKongPosDef2->Pos.Y)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE, ZONEERR_BATTLE100,
			"红方", "蓝方");
		
		pstMon = z_find_pworld_mon_byid(pstEnv, pstPworldInst, 294052);
		if(pstMon)
		{
			pstMonDef = z_get_mon_def(pstEnv, pstMon);
			if (pstMonDef)
			{
				stAni.iType = OBJ_UNKNOW;	
				z_mon_die(pstEnv->pstAppCtx, pstEnv, pstMon , pstMonDef, &stAni, 1, 0);
			}
		}
			
		battle_all_info_update(pstEnv, pstPworldInst, pstBattleDef, 1, 1);
		//停止副本刷怪等。
		z_pworld_over(pstEnv, pstPworldInst);
		pstPworldInst->tEnd = pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	else if(pstNpc->stMon.stCurrPos.X == pstXuKongPosDef1->Pos.X &&
		pstNpc->stMon.stCurrPos.Y == pstXuKongPosDef1->Pos.Y)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE, ZONEERR_BATTLE100,
			"蓝方", "红方");

		pstMon = z_find_pworld_mon_byid(pstEnv, pstPworldInst, 294051);
		if(pstMon)
		{
			pstMonDef = z_get_mon_def(pstEnv, pstMon);
			if (pstMonDef)
			{
				stAni.iType = OBJ_UNKNOW;	
				z_mon_die(pstEnv->pstAppCtx, pstEnv, pstMon , pstMonDef, &stAni, 1, 0);
			}
		}
		
		battle_all_info_update(pstEnv, pstPworldInst, pstBattleDef, 2, 1);
		//停止副本刷怪等。
		z_pworld_over(pstEnv, pstPworldInst);
		pstPworldInst->tEnd = pstEnv->pstAppCtx->stCurr.tv_sec;
	}
*/	
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstBattleDef);	//add by paraunused.pl
	UNUSED(pstPworldInst);	//add by paraunused.pl
}
int xukong_pworld_tick(ZONESVRENV * pstEnv, BATTLEDEF * pstBattleDef, ZONEPWORLDINST *pstPworldInst)
{
	Npc *pstNpc;
	ZoneAni stAni;
	MONSTERDEF *pstMonDef;
	Monster *pstMon;
	XUKONGPOSDEF *pstXuKongPosDef1, *pstXuKongPosDef2;
	ZONEMAPINST * pstMapInst;
	
	pstNpc = z_find_pworld_npc_byid(pstEnv, pstPworldInst, XUKONG_NPC_ID);
	if(!pstNpc)
	{
		return -1;
	}

	pstXuKongPosDef1 = z_find_xukong_pos_def(pstEnv, 1, MAX_XUKONG_CAMP_POS);
	if(!pstXuKongPosDef1)
	{
		return -1;
	}

	pstXuKongPosDef2 = z_find_xukong_pos_def(pstEnv, 2, MAX_XUKONG_CAMP_POS);
	if(!pstXuKongPosDef2)
	{
		return -1;
	}

	if (pstNpc->stMon.cMoving)
	{
		z_monster_move_curr(pstEnv->pstAppCtx, pstEnv, &pstNpc->stMon, 0);
	}

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPworldInst->astMapInstIdx[0]);
	if(NULL == pstMapInst)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_find_map_inst %d map inst fail", pstPworldInst->astMapInstIdx[0].iID);
		return -1;
	}

	if(pstNpc->stMon.stCurrPos.X == pstXuKongPosDef2->Pos.X &&
		pstNpc->stMon.stCurrPos.Y == pstXuKongPosDef2->Pos.Y)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE, ZONEERR_BATTLE100,
			LJY_BATTLE_RED, LJY_BATTLE_BLUE);
		
		pstMon = z_find_pworld_mon_byid(pstEnv, pstPworldInst, 294052);
		if(pstMon)
		{
			pstMonDef = z_get_mon_def(pstEnv, pstMon);
			if (pstMonDef)
			{
				stAni.iType = OBJ_UNKNOW;	
				z_mon_die(pstEnv->pstAppCtx, pstEnv, pstMon , pstMonDef, &stAni, 1, 0);
			}
		}
			
		battle_all_info_update(pstEnv, pstPworldInst, pstBattleDef, 1, 1);
		//停止副本刷怪等。
		z_pworld_over(pstEnv, pstPworldInst);
		pstPworldInst->tEnd = pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	else if(pstNpc->stMon.stCurrPos.X == pstXuKongPosDef1->Pos.X &&
		pstNpc->stMon.stCurrPos.Y == pstXuKongPosDef1->Pos.Y)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE, ZONEERR_BATTLE100,
			LJY_BATTLE_BLUE, LJY_BATTLE_RED);

		pstMon = z_find_pworld_mon_byid(pstEnv, pstPworldInst, 294051);
		if(pstMon)
		{
			pstMonDef = z_get_mon_def(pstEnv, pstMon);
			if (pstMonDef)
			{
				stAni.iType = OBJ_UNKNOW;	
				z_mon_die(pstEnv->pstAppCtx, pstEnv, pstMon , pstMonDef, &stAni, 1, 0);
			}
		}
		
		battle_all_info_update(pstEnv, pstPworldInst, pstBattleDef, 2, 1);
		//停止副本刷怪等。
		z_pworld_over(pstEnv, pstPworldInst);
		pstPworldInst->tEnd = pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	
	return 0;
}
	
int battle_pworld_tick(ZONESVRENV * pstEnv, ZONEPWORLD *pstPworld, ZONEPWORLDINST *pstPworldInst, int iSec)
{
	BATTLEDEF *pstBattleDef = z_find_battledef(pstEnv, pstPworld->stPworldDef.BattleID);

	if (!pstBattleDef)
	{
		return -1;
	}

	switch(pstBattleDef->FightType)
	{
		case BATTLE_FIGHT_TYPE_XUKONG:
			xukong_pworld_tick(pstEnv, pstBattleDef, pstPworldInst);
			break;
		case BATTLE_FIGHT_TYPE_CLAN_WAR:
			clan_war_pworld_tick(pstEnv, pstBattleDef,pstPworld, pstPworldInst);
			break;
		default:
			break;
	}
	
	return 0;
	UNUSED(iSec);	//add by paraunused.pl
}

int is_has_xukong_energy(Player *pstPlayer)
{
	if(pstPlayer->stOnline.stWarOL.iEnergyStat == 1)
	{
		return 1;
	}
	
	return 0;
}

int player_xukong_energy_action(ZONESVRENV * pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_XUKONG_ENERGY;
	pstActionRes->Data.XuKongEnergy.EnergyStat = pstPlayer->stOnline.stWarOL.iEnergyStat;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	
	return 0;
}

int player_collect_xukong_energy(ZONESVRENV * pstEnv, Player *pstPlayer)
{
	pstPlayer->stOnline.stWarOL.iEnergyStat  = 1;
	player_xukong_energy_action(pstEnv, pstPlayer);

	z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_BATTLE115);
	return 0;
}

int player_xukong_energy_clear(ZONESVRENV * pstEnv, Player *pstPlayer)
{
	if(pstPlayer->stOnline.stWarOL.iEnergyStat == 0)
	{
		return 0;
	}
	
	pstPlayer->stOnline.stWarOL.iEnergyStat  = 0;
	player_xukong_energy_action(pstEnv, pstPlayer);
	return 0;
}

int player_commit_xukong_energy(ZONESVRENV * pstEnv, Player *pstPlayer)
{
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	ZONEPWORLDINST *pstPworldInst;
	ZONEPWORLD *pstPworld;
	BATTLEDEF *pstBattleDef;

	if(!is_has_xukong_energy(pstPlayer))
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_BATTLE116);
		return -1;
	}

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if( !pstMapInst )
	{
		return -1;
	}

	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if(!pstPworldInst)
	{
		return -1;
	}

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if (NULL == pstPworld)
	{
		return -1;
	}

	pstBattleDef = z_find_battledef(pstEnv, pstPworld->stPworldDef.BattleID);
	if (!pstBattleDef)
	{
		return -1;
	}

	if(pstBattleDef->FightType != BATTLE_FIGHT_TYPE_XUKONG)
	{
		return -1;
	}

	if(pstPlayer->stOnline.stWarOL.cCampIdx == 1)
	{
		pstPworldInst->stPworldBattleInfo.stXuKongInfo.iCampVal1++;

		z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE, 
			ZONEERR_BATTLE102,
			LJY_BATTLE_RED,
			pstPlayer->stRoleData.RoleName);
	}
	else
	{
		pstPworldInst->stPworldBattleInfo.stXuKongInfo.iCampVal2++;
		z_sys_strf_map_broadcast(pstEnv, pstMapInst, SYS_MIDDLE, 
			ZONEERR_BATTLE102,
			LJY_BATTLE_BLUE,
			pstPlayer->stRoleData.RoleName);
	}

	pstPlayer->stOnline.stWarOL.iOre++;
	normal_battle_svr_info(pstEnv, pstPlayer, pstPworld,pstPworldInst, 0, 0);

	xukong_energy_update(pstEnv, pstPworldInst);
	player_xukong_energy_clear(pstEnv, pstPlayer);

	//更新积分
	normal_battle_svr_info(pstEnv, NULL, pstPworld,pstPworldInst, 3, 0);

	return 0;
}

int z_player_exit_pworld_queue(ZONESVRENV* pstEnv, Player *pstPlayer, CSEXITPWORLDQUEUE *pstCSExit)
{
	if (pstCSExit->TeamJoin == TEAM_JOIN_CLAN)
	{
		CLANINFO* pstClanInfo;
		pstClanInfo = &pstPlayer->stRoleData.MiscInfo.ClanInfo;
		if (CLAN_POS_PRESIDENT ==  pstClanInfo->Position || 
			CLAN_POS_VICE_PRESIDENT == pstClanInfo->Position)
		{
			RolePworldQueue *pstRoleQueue;
			pstRoleQueue = z_get_player_online_queue(pstEnv, pstPlayer, pstCSExit->PworldID);
			if(NULL == pstRoleQueue)
			{
				z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_RONGYAN6);
				return -1;
			}
			
			if (PWORLD_CLAN_QUEUE != pstRoleQueue->iQueueType)
			{
				z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_RONGYAN7);
				return -1;
			}
			else				
			{
				PworldClanQueue *pstClanQueue;

				PworldApplyQueue *pstApplyQueue ;
				int iClanQueueIdx ;

				pstClanQueue = get_pworld_clan_queue_by_id(pstEnv, pstRoleQueue->iQueueIdx, 
						pstPlayer->stOnline.stPworldQueue.iPworldID);
				if(NULL == pstClanQueue)
				{
					z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
										ZONEERR_RONGYAN8);
					return -1;
				}

				if (pstClanQueue->iQueueType != PWORLD_APPLY_QUEUE)
				{
					z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
										ZONEERR_RONGYAN9);
					return -1;
				}
				
				pstApplyQueue = get_pworld_apply_queue(pstEnv, pstPlayer->stOnline.stPworldQueue.iPworldID);

				if (!pstApplyQueue)
				{
					z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
										ZONEERR_RONGYAN10);
					return -1;
				}

				iClanQueueIdx = pstClanQueue - pstEnv->pstPworldQueue->stClanQueue;

				//移除排队队列
				queuers_remove_applyqueue(pstApplyQueue, 1, &iClanQueueIdx);

				z_clear_pworld_clan_queue(pstEnv,iClanQueueIdx, pstClanQueue->iPworldID);

			}
		}
		else
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_RONGYAN11);
			return -1;
		}
	}
	else
	{
		z_exit_pworld_queue(pstEnv, pstPlayer, pstCSExit->PworldID, 1);
	}
	
	return 0;
}

int z_player_level_remain_tm_check(ZONESVRENV* pstEnv, Player *pstPlayer,BATTLEDEF *pstBattleDef)
{
	LEVELUP *pstLevelUP = NULL;
	
	if(pstBattleDef->FightType != BATTLE_FIGHT_TYPE_XUKONG &&
	   pstBattleDef->FightType != BATTLE_FIGHT_TYPE_AOSHAN &&
       pstBattleDef->FightType != BATTLE_FIGHT_TYPE_CAMP_LIFT)
	{
		return 0;
	}

	pstLevelUP = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	if (NULL == pstLevelUP)
	{
		return 0;
	}

	if(pstLevelUP->RemainTm != 0 && pstLevelUP->RemainTm <= pstPlayer->stRoleData.MiscInfo.LevelRemainInfo.LevelRemainDay)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_BATTLE117);
		return -1;
	}
	
	return 0;
}

