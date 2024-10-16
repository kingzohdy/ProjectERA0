
#include "zone_db.h"
#include "zone_span_strong.h"
#include "zone_mail.h"
#include "zone_npc.h"
#include "zone_map.h"
#include "zone_clt.h"
#include "zone_clan.h"
#include "zone_err.h"
#include "zone_package.h"
#include "zone_span.h"
#include "zone_strongpoint.h"
#include "zone_oplog.h"
#include "zone_attr.h"
#include "zone_status.h"
#include "zone_zh_name.h"

extern int g_iZoneBus;

#define SPAN_SELECT_HOUR 12
#define SPAN_FIGHT_HOUR 20
#define SPAN_FIGHT_MIN 30
#define MAX_SPAN_STRONG_WEEK 2

static int g_iSpanWeek[MAX_SPAN_STRONG_WEEK]={4};

#define HOUR_GET_BINDGOLD 10
#define HOUR_GET_BULL 100

int span_strong_get_zone_svr_id(int iBattleID, int iSvrID)
{
	int iRegionID;
	int iBusID = 0;;

	GET_REGION_ID(iRegionID, iBattleID) ;
	char szAddr[64];
	
	sprintf(szAddr, "%d.%d.%d.%d", iRegionID, iSvrID, FUNC_ZONE_SVR, 1);
	tbus_addr_aton(szAddr, (TBUSADDR *)&iBusID);
	
	return iBusID;
}

int span_strong_send_mail(ZONESVRENV * pstEnv, int iType, RSTRONGONE * pstOne, SPANSTRONGDEF * pstDef)
{
	
	SSPKG stPkg;
	SSSTRONGMAILRES *pstMail = &stPkg.Body.SpanStrongRes.Data.Mail;
	int iBus = 0;

	Z_SSHEAD_INIT(&stPkg.Head, SPAN_STRONG_RES);
	stPkg.Body.SpanStrongRes.OpType = SS_SPAN_STRONG_RES_MAIL;
	
	memset(pstMail, 0, sizeof(*pstMail));
	snprintf(pstMail->Title, sizeof(pstMail->Title), LJY_MAIL54, pstDef->Name);
	
	if (iType == 1)
	{
		snprintf(pstMail->Text, sizeof(pstMail->Text), LJY_MAIL42,
				pstOne->Sale.RSale[0].ClanName, 
				pstOne->Sale.RSale[0].Sale, pstDef->Name);
		pstMail->ClanID  =pstOne->Sale.RSale[0].ClanID;

		iBus = span_strong_get_zone_svr_id(pstEnv->pstAppCtx->iId, pstOne->Sale.RSale[0].SvrID);
		//GET_WORLD_ENTITY_BY_SVRID(iBus, pstEnv->pstAppCtx->iId, FUNC_ZONE_SVR, pstOne->Sale.RSale[0].SvrID);
	}
	else if (iType == 2)
	{
		snprintf(pstMail->Text, sizeof(pstMail->Text), LJY_MAIL44,
			pstOne->Sale.RSale[0].ClanName, pstOne->Sale.RSale[0].Sale, pstDef->Name,
			pstDef->Name, pstOne->Owner.ClanName, pstDef->Name);
		pstMail->ClanID  =pstOne->Sale.RSale[0].ClanID;

		//GET_WORLD_ENTITY_BY_SVRID(iBus, pstEnv->pstAppCtx->iId, FUNC_ZONE_SVR, 
		//								pstOne->Sale.RSale[0].SvrID);
		iBus = span_strong_get_zone_svr_id(pstEnv->pstAppCtx->iId, pstOne->Sale.RSale[0].SvrID);
	}
	else if (iType == 3)
	{
		snprintf(pstMail->Text, sizeof(pstMail->Text),  LJY_MAIL45,
			pstDef->Name, pstDef->Name,pstOne->Sale.RSale[0].Sale,pstDef->Name);
		pstMail->ClanID  =pstOne->Owner.ClanID;

		//GET_WORLD_ENTITY_BY_SVRID(iBus, pstEnv->pstAppCtx->iId, FUNC_ZONE_SVR, 
		//								pstOne->Owner.SvrID);

		iBus = span_strong_get_zone_svr_id(pstEnv->pstAppCtx->iId, pstOne->Owner.SvrID);
	}
	else if(iType == 4)
	{
		snprintf(pstMail->Text, sizeof(pstMail->Text), LJY_MAIL46,
			pstOne->Sale.RSale[0].ClanName,pstDef->Name);
		pstMail->ClanID  =pstOne->Sale.RSale[0].ClanID;

		//GET_WORLD_ENTITY_BY_SVRID(iBus, pstEnv->pstAppCtx->iId, FUNC_ZONE_SVR, 
		//								pstOne->Sale.RSale[0].SvrID);
		iBus = span_strong_get_zone_svr_id(pstEnv->pstAppCtx->iId, pstOne->Sale.RSale[0].SvrID);
	}
	else if (iType == 5)
	{
		snprintf(pstMail->Text, sizeof(pstMail->Text), LJY_MAIL47,
			pstOne->Sale.RSale[1].ClanName,pstDef->Name);
		pstMail->ClanID  =pstOne->Sale.RSale[1].ClanID;

		//GET_WORLD_ENTITY_BY_SVRID(iBus, pstEnv->pstAppCtx->iId, FUNC_ZONE_SVR, 
		//								pstOne->Sale.RSale[1].SvrID);
		iBus = span_strong_get_zone_svr_id(pstEnv->pstAppCtx->iId, pstOne->Sale.RSale[1].SvrID);
	}
	else
	{
		return -1;
	}

	z_ssmsg_send(pstEnv, &stPkg, iBus);
	return 0;
}

int span_strong_select_event(ZONESVRENV* pstEnv, RSTRONGONE *pstOne)
{
	SPANSTRONGDEF *pstDef;
	int iRet = 0;
	int i;
	char szTitle[32];
	
	memset(&pstOne->Pworld, 0, sizeof(pstOne->Pworld));
	memset(&pstOne->Atk, 0, sizeof(pstOne->Atk));
	
	// 无人竞拍
	if (pstOne->Sale.iSale <= 0)
	{
		return 0;
	}

	pstDef = span_strong_def_find( pstEnv, pstOne->StrongID);
	if (!pstDef)
		return 0;

	// 有人占领
	if (pstOne->Owner.ClanID > 0)
	{
		pstOne->Atk.ClanID = pstOne->Sale.RSale[0].ClanID;
		pstOne->Atk.SvrID = pstOne->Sale.RSale[0].SvrID;
		STRNCPY(pstOne->Atk.ClanName, pstOne->Sale.RSale[0].ClanName, sizeof(pstOne->Atk.ClanName));

		span_strong_send_mail(pstEnv, 2,  pstOne,  pstDef);
		span_strong_send_mail(pstEnv, 3,  pstOne,  pstDef);
	}
	else
	{
		// 竞拍第一名直接占领
		pstOne->Owner.ClanID = pstOne->Sale.RSale[0].ClanID;
		pstOne->Owner.SvrID = pstOne->Sale.RSale[0].SvrID;
		STRNCPY(pstOne->Owner.ClanName, pstOne->Sale.RSale[0].ClanName, sizeof(pstOne->Owner.ClanName));
		pstOne->Owner.OccTime = pstEnv->pstAppCtx->stCurr.tv_sec;

		// 第二名直接挑战
		if (pstOne->Sale.iSale == MAX_RSALE_TOP) 
		{
			pstOne->Atk.ClanID = pstOne->Sale.RSale[1].ClanID;
			pstOne->Atk.SvrID = pstOne->Sale.RSale[1].SvrID;
			STRNCPY(pstOne->Atk.ClanName, pstOne->Sale.RSale[1].ClanName, sizeof(pstOne->Atk.ClanName));

			span_strong_send_mail(pstEnv, 4,  pstOne,  pstDef);
			span_strong_send_mail(pstEnv, 5,  pstOne,  pstDef);
		}
		else
		{
			span_strong_send_mail(pstEnv, 1,  pstOne,  pstDef);
		}
	}

	// 退钱
	// 竟不是防守方也不是进攻方,竞拍失败
	snprintf(szTitle, sizeof(szTitle), "%s%s",pstDef->Name, LJY_STRONG701);
	for (i=0; i<pstOne->Sale.iPlayer; i++)
	{
		if (	(pstOne->Sale.RPlayer[i].SvrID != pstOne->Owner.SvrID ||
			pstOne->Sale.RPlayer[i].ClanID != pstOne->Owner.ClanID) &&
			(pstOne->Sale.RPlayer[i].SvrID != pstOne->Atk.SvrID ||
			pstOne->Sale.RPlayer[i].ClanID != pstOne->Atk.ClanID))
		{
			//GET_WORLD_ENTITY_BY_SVRID(g_iZoneBus, pstEnv->pstAppCtx->iId, FUNC_ZONE_SVR, 
			//							pstOne->Sale.RPlayer[i].SvrID);

			g_iZoneBus = span_strong_get_zone_svr_id(pstEnv->pstAppCtx->iId, pstOne->Sale.RPlayer[i].SvrID);
			player_sysmail_fast_send(pstEnv, pstOne->Sale.RPlayer[i].RoleName, 
				szTitle, LJY_STRONG702, NULL, 0, pstOne->Sale.RPlayer[i].Sale, NULL, 0);
			g_iZoneBus = 0;
		}
	}

	pstEnv->pstRegionGlobalInfo->stStat.iDirty = 2;
	return iRet;
}

// 公会匹配
int span_strong_clan_matching(int iSelfClanID, int iSelfSvrID, int iMatchClanID, int iMatchSvrID)
{
	if (iSelfClanID == iMatchClanID &&
		iSelfSvrID == iMatchSvrID)
	{
		return 1;
	}
	return 0;
}

Monster *span_strong_new_res_mon(ZONESVRENV* pstEnv,ZONEPWORLDINST *pstZInst,ZONEMAPINST* pstMInst,
							int iMonID, RESPOS *pstPos, int iCamp)
{
	Monster *pstMon, stMonster;
	MONSTERDEF *pstMonDef;
	
	memset(&stMonster, 0, sizeof(stMonster));
	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMInst->iMapInstID;
	stMonster.stMap.iPos =	pstMInst->iIndex;
	stMonster.iInitMapPos	=	pstMInst->iMapPos;	
	stMonster.stInitPos = *pstPos;
	stMonster.bDirection = RAND1(180);
	stMonster.stCurrPos = stMonster.stInitPos;
	mon_set_camp( &stMonster, iCamp);

	pstMonDef = z_get_mon_def(pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		return NULL;
	}

	pstMon =	z_new_monster(pstEnv->pstAppCtx, pstEnv, &stMonster, pstMonDef);
	if( !pstMon )
	{
		return NULL;
	}

	pstMon->cFirstBorn = 1;
	if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstMon, NULL))
	{
		tmempool_free(pstEnv->pstMonsterPool, pstMon->iMIndex);
		return NULL;
	}
	pstMon->cFirstBorn = 0;

	return pstMon;
}


int span_strong_resmon_die(ZONESVRENV* pstEnv, Monster* pstMon, MONSTERDEF *pstMonDef, Player *pstKiller)
{
	ZONEMAPINST *pstMInst;
	ZONEPWORLD *pstZonePworld;
	RSTRONGONE *pstOne;
	int iCamp = 0;
	int i;
	SPANSTRONGDEF *pstDef;
	int iMonID;
	Monster* pstMon1;
	
	if (pstMonDef->GetRes <= 0 )
	{
		return 0;
	}

	pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if (NULL == pstMInst)
	{
		return 0;
	}

	pstZonePworld = z_find_pworld2(pstEnv, pstMon->stMap.iID);
	if (!pstZonePworld ||strong_is_normal(pstZonePworld))
	{
		return 0;
	}

	pstOne = span_strong_one_find(pstEnv, pstZonePworld->stPworldDef.StrongMapID);
	if (!pstOne)
	{
		return -1;
	}

	pstDef = span_strong_def_find(pstEnv, pstZonePworld->stPworldDef.StrongMapID);
	if (!pstDef)
	{
		return -1;
	}

	if (pstKiller)
	{
		iCamp = pstKiller->stOnline.stWarOL.cCampIdx;
		pstKiller->stOnline.stWarOL.iOre ++;
	}
	else
	{
		for (i=0; i<pstOne->Pworld.MonNum; i++)
		{
			if (	pstOne->Pworld.Mon[i].MemID == pstMon->iID )
			{
				if (pstOne->Pworld.Mon[i].Camp  == 1)
				{
					iCamp =2;
				}
				else if (pstOne->Pworld.Mon[i].Camp  == 2)
				{
					iCamp =1;
				}
				break;
			}
		}	
	}

	for (i=0; i<pstOne->Pworld.MonNum; i++)
	{
		if (	pstOne->Pworld.Mon[i].MemID == pstMon->iID)
		{
			pstDef = span_strong_def_find(pstEnv, pstZonePworld->stPworldDef.StrongMapID);
			if (!pstDef)
			{
				return 0;
			}

			if (iCamp == 1)
			{
				z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE, ZONEERR_BROADMAP317, 
						pstOne->Owner.ClanName, pstMonDef->Name);
				iMonID = pstDef->ResMon[i].DefMonID;
				
			}
			else if (iCamp == 2)
			{
				z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE, ZONEERR_BROADMAP317, 
						pstOne->Atk.ClanName, pstMonDef->Name);	
				iMonID = pstDef->ResMon[i].AtkMonID;
			}
			else if (iCamp == 0)
			{
				iMonID = pstDef->ResMon[i].MonID;
			}
			else
			{
				return -1;
			}

			pstMon1 = span_strong_new_res_mon(pstEnv,NULL,pstMInst,
							iMonID,&pstMon->stCurrPos, iCamp);
			if (pstMon1)
			{
				pstOne->Pworld.Mon[i].MemID = pstMon1->iID;
				pstOne->Pworld.Mon[i].Camp = iCamp;
				pstOne->Pworld.Mon[i].X =  pstMon->stCurrPos.X;
				pstOne->Pworld.Mon[i].Y = pstMon->stCurrPos.Y;
				span_strong_pworld_info(pstEnv, NULL, 0, pstMInst, pstZonePworld->stPworldDef.StrongMapID);
			}

			break;
		}	
	}
	return 0;
}

int strong_is_span(ZONEPWORLD *pstPworld)
{
	if (	pstPworld && 
		pstPworld->stPworldDef.StrongMapID > 0 && 
		pstPworld->stPworldDef.PolyType == PWORLD_POLY_SPAN_STRONG)
	{
		return 1;
	}
	return 0;
}

int span_strong_war_one(ZONESVRENV* pstEnv, Player *pstPlayer,CSSPANSTRONGWARONE *pstOne, int iStrongID)
{
	WarFieldOL *pstWarOL = &pstPlayer->stOnline.stWarOL;

	pstOne->Camp = pstWarOL->cCampIdx;
	pstOne->AssitNum = pstPlayer->stRoleData.MiscInfo.MiscWar.Assit;
	pstOne->Career = pstPlayer->stRoleData.Career;
	pstOne->CountKill = pstPlayer->stRoleData.MiscInfo.MiscWar.Kill;
	pstOne->EnterTime = pstPlayer->stRoleData.MiscInfo.MiscWar.FightTime;
	pstOne->MaxContinueKill = pstWarOL->iKillNum;
	pstOne->ResGet = pstWarOL->iOre;
	pstOne->CurrContinueKill = pstWarOL->iContinueKill;
	pstOne->MapID = iStrongID;
	pstOne->Die = pstPlayer->stRoleData.MiscInfo.MiscWar.Die;
	strncpy(pstOne->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstOne->RoleName));
	strncpy(pstOne->ClanName, pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Name, sizeof(pstOne->ClanName));
	return 0;
}

int span_strong_war_one_notify(ZONESVRENV* pstEnv, Player *pstPlayer, int iStrongID)
{
	CSPKG stPkg;
	CSSPANSTRONGSVR *pstSvr  = &stPkg.Body.SpanStrongSvr;

	memset(pstSvr, 0, sizeof(*pstSvr));
	pstSvr->Cmd = SPAN_STRONG_SVR_WAR_ONE;
	span_strong_war_one(pstEnv, pstPlayer,&pstSvr->Data.WarOne, iStrongID);
	
	Z_CSHEAD_INIT(&stPkg.Head, SPAN_STRONG_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int span_strong_war_all_notify(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPInst, int iEnd, int iStrongID)
{
	CSPKG stPkg;
	CSSPANSTRONGSVR *pstSvr  = &stPkg.Body.SpanStrongSvr;
	int i;
	Player *pstPlayer;
	Player *apstPlayer[STRONG_PLAYER_MAX];
	int iNum = 0;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	int iTime;
	
	memset(pstSvr, 0, sizeof(*pstSvr));
	pstSvr->Cmd = SPAN_STRONG_SVR_WAR_ALL;

	if (iEnd)
	{
		// iEnd参数 实际传进来的是获胜方阵营
		pstSvr->Data.WarALL.WinCamp = iEnd;
	}

	for (i=0; i<pstPInst->iPlayer; i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPInst->aiPlayerID[i]);
		if (!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}

		apstPlayer[iNum] = pstPlayer;
		iNum ++;

		iTime = iCurr-pstPlayer->stOnline.stWarOL.tFightTime;
		pstPlayer->stRoleData.MiscInfo.MiscWar.FightTime += iTime;
		pstPlayer->stOnline.stWarOL.tFightTime = iCurr;

		if (iEnd)
		{
			z_spanstrong_oplog( pstEnv, pstPlayer, iStrongID, iEnd);
		}
	
		span_strong_war_one(pstEnv, pstPlayer,&pstSvr->Data.WarALL.Ones[pstSvr->Data.WarALL.Num], iStrongID);
		pstSvr->Data.WarALL.Num++;
		if (pstSvr->Data.WarALL.Num >= STRONG_PLAYER_MAX)
		{
			break;
		}
	}

	if (iNum <= 0)
	{
		return 0;
	}
		
	pstSvr->Data.WarALL.End = iEnd;
	Z_CSHEAD_INIT(&stPkg.Head, SPAN_STRONG_SVR);
	z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer,iNum, &stPkg);
	return 0;
}

int span_strong_player_die(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstKiller)
{
	ZONEMAPINST *pstMInst;
	ZONEPWORLD *pstZonePworld;
	ZONEIDX stIdx;
	int i;
	Player *pstAssit;
	char szName[ROLE_NAME_LEN];
	SPANSTRONGDEF *pstDef;
	MISCWAR *pstWar = &pstKiller->stRoleData.MiscInfo.MiscWar;
	
	
	if (!pstKiller)
		return -1;

	WarFieldOL *pstWarOL = &pstKiller->stOnline.stWarOL;
	
	pstZonePworld = z_find_pworld2(pstEnv, pstPlayer->stRoleData.Map);
	if (!pstZonePworld ||!strong_is_span(pstZonePworld))
	{
		return -1;

	}

	pstDef = span_strong_def_find( pstEnv, pstZonePworld->stPworldDef.StrongMapID);
	if (!pstDef)
		return -1;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = -1;
	pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (!pstMInst)
		return -1;


	// 击杀方玩家
	pstWarOL->iContinueKill ++;  	// 连杀数
	if (pstWarOL->iContinueKill > pstWarOL->iKillNum)
	{
		pstWarOL->iKillNum  = pstWarOL->iContinueKill;    // 最大连杀数	
	}

	pstWar->Kill ++;
	pstKiller->stOnline.iDieNum = 0;

	STRNCPY(szName, pstKiller->stRoleData.RoleName, CCH(szName));
	battle_rolename_remove_prefix(szName);
	
	if (pstWarOL->iContinueKill == 100)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE,ZONEERR_BROADMAP305,
			szName, pstWarOL->iContinueKill);	
	}
	else if (pstWarOL->iContinueKill == 50)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE,ZONEERR_BROADMAP306,
			szName, pstWarOL->iContinueKill);	
	}
	else if (pstWarOL->iContinueKill == 20)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE,ZONEERR_BROADMAP307,
			szName, pstWarOL->iContinueKill);
	}
	else if(pstWarOL->iContinueKill == 10)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE,ZONEERR_BROADMAP308,
			szName, pstWarOL->iContinueKill);
	}
	else if (pstWarOL->iContinueKill == 5)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE,ZONEERR_BROADMAP309,
			szName, pstWarOL->iContinueKill);
	}
	else if (pstWarOL->iContinueKill == 3)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE,ZONEERR_BROADMAP310,
			szName, pstWarOL->iContinueKill);
	}
	
	if (pstPlayer->stOnline.stWarOL.iContinueKill > 2)
	{
		char szName1[ROLE_NAME_LEN];
		STRNCPY(szName, pstKiller->stRoleData.RoleName, CCH(szName));
		battle_rolename_remove_prefix(szName);

		STRNCPY(szName1, pstPlayer->stRoleData.RoleName, CCH(szName1));
		battle_rolename_remove_prefix(szName1);
		
		z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE,ZONEERR_BROADMAP312,
				szName, szName1,pstPlayer->stOnline.stWarOL.iContinueKill);
	}

	// 计算助攻
	for (i = 0; i < pstKiller->stOnline.iVAnimate; i++) 
	{
		pstAssit = z_id_player(pstEnv->pstAppCtx, pstEnv, pstKiller->stOnline.aiVAnimate[i]);
		if(NULL == pstAssit || pstAssit->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}

		if(CS_STAT_DEAD & pstAssit->stOnline.State)
		{
			continue;
		}

		pstAssit->stRoleData.MiscInfo.MiscWar.Assit ++;
	}

	// 杀人获取buff  todo:
	for (i=MAX_STRONG_KILL_BUFF -1; i>=0; i--)
	{
		if (pstDef->KillBuff[i].KillNum <= 0)
		{
			continue;
		}

		if (pstWarOL->iContinueKill == pstDef->KillBuff[i].KillNum)
		{
			z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstKiller, pstDef->KillBuff[i].BuffID, 
									pstDef->KillBuff[i].BuffLvl, pstDef->KillBuff[i].BuffTime, 1, 0);
			break;
		}
	}
	

	// 被击杀玩家
	pstPlayer->stOnline.stWarOL.iDieSec ++; 		// 死亡累加
	pstPlayer->stOnline.stWarOL.iContinueKill = 0;	// 连杀清零
	pstPlayer->stRoleData.MiscInfo.MiscWar.Die ++;
	
	// 通知客户端个人信息 todo:
	span_strong_war_one_notify(pstEnv, pstKiller, pstZonePworld->stPworldDef.StrongMapID);
	span_strong_war_one_notify(pstEnv, pstPlayer, pstZonePworld->stPworldDef.StrongMapID);
	return 0;
}

int span_strong_exit_clear_buff(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLD *pstPworld)
{
	SPANSTRONGDEF *pstDef;
	ZoneStatus *pstStat;
	int i;

	if (pstPworld->stPworldDef.StrongMapID <= 0 ||pstPworld->stPworldDef.PolyType != PWORLD_POLY_SPAN_STRONG)
		return 0;

	pstDef = span_strong_def_find( pstEnv, pstPworld->stPworldDef.StrongMapID);
	if (!pstDef)
		return 0;

	pstStat = z_player_find_status(pstPlayer, pstDef->DieBuff.BuffID);
	if (pstStat)
	{
		z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstDef->DieBuff.BuffID, 1, 0);
	}

	for (i=0; i<MAX_STRONG_KILL_BUFF; i++)
	{
		if (pstDef->KillBuff[i].BuffID <= 0)
		{
			break;
		}
		pstStat = z_player_find_status(pstPlayer, pstDef->KillBuff[i].BuffID);
		if (pstStat)
		{
			z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstDef->KillBuff[i].BuffID, 1, 0);
		}	
	}
	return 0;
}


int span_strong_owner_chg(ZONESVRENV* pstEnv,RSTRONGONE *pstOne, char *pszClanName, 
								int iClanID, int iSvrID)
{
	pstOne->Owner.ClanID = iClanID;
	pstOne->Owner.SvrID = iSvrID;
	strncpy(pstOne->Owner.ClanName, pszClanName, sizeof(pstOne->Owner.ClanName));
	pstOne->Owner.OccTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	pstEnv->pstRegionGlobalInfo->stStat.iDirty = 2;
	return 0;
}


int span_strong_war_award_one(ZONESVRENV* pstEnv, Player *pstPlayer, int iWinCamp, SPANSTRONGDEF *pstDef)
{
	int i;
	ROLEGRID stRoleGrid[MAX_STRONG_ITEM];
	int iNum = 0;
	STRONGAWARD *pstAward;       
	char szMsg[128];
	char szTital[32];
	LEVELUP *pstLvl;
	char szName[32];
	
	if (	pstPlayer->stRoleData.MiscInfo.MiscWar.Assit + pstPlayer->stRoleData.MiscInfo.MiscWar.Kill< 40 &&
		pstPlayer->stRoleData.MiscInfo.MiscWar.Die < 20 && 
		pstPlayer->stRoleData.MiscInfo.MiscWar.FightTime < 1200)
	{
		return 0;
	}

	snprintf(szTital, sizeof(szTital), LJY_MAIL55, pstDef->Name);
	pstLvl = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	if (!pstLvl)
		return 0;

	STRNCPY(szName, pstPlayer->stRoleData.RoleName, CCH(szName));
	battle_rolename_remove_prefix(szName);
	
	if (pstPlayer->stOnline.stWarOL.cCampIdx == iWinCamp)
	{
		pstAward = &pstDef->WinAward;
		snprintf(szMsg, sizeof(szMsg), LJY_MAIL48,szName, pstDef->Name);
	}
	else
	{
		pstAward = &pstDef->FailAward;
		snprintf(szMsg, sizeof(szMsg), LJY_MAIL49,szName, pstDef->Name);
	}
	
	for (i=0; i<MAX_STRONG_ITEM; i++)
	{
		if (pstAward->Item[i].ItemID <= 0)
		{
			break;
		}

		if (item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid[iNum], pstAward->Item[i].ItemID) < 0)
		{
			stRoleGrid[i].GridData.RoleItem.Num = pstAward->Item[i].ItemNum;
			continue;
		}

		iNum ++;
	}

	if (iNum > 0)
	{
		g_iZoneBus = span_strong_get_zone_svr_id(pstEnv->pstAppCtx->iId, pstPlayer->stRoleData.WorldID);
		player_sysmail_fast_send(pstEnv, szName, szTital,szMsg,stRoleGrid, iNum, 0,NULL, 0);
		g_iZoneBus = 0;
	}

	for (i=0; i<MAX_STRONG_MONEY; i++)
	{
		if (pstAward->Money[i].Type <= 0)
		{
			break;
		}

		package_add_money( pstEnv, pstPlayer, pstAward->Money[i].Type, pstAward->Money[i].Num);
		if (pstAward->Money[i].Type == ATTR_ID_BULL)
		{
			z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_STRONGWAR, 0,  
				pstAward->Money[i].Num, 0, "据点战奖励");
		}
		else if (pstAward->Money[i].Type == ATTR_ID_MONEY)
		{
			z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_STRONGWAR, 0,  0, 
				pstAward->Money[i].Num, "据点战奖励");
		}
	}

	pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_EXP_NOLEVEL_LIMIT;
	player_add_exp(pstEnv, pstPlayer, pstLvl->ContributeExp*(1.0*pstAward->Exp/10000), 0);
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_EXP_NOLEVEL_LIMIT;
	return 0;
}

int span_strong_war_award(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPInst, SPANSTRONGDEF *pstDef,int iWinCamp)
{	
	int i;
	Player *pstPlayer;

	for (i=0; i<pstPInst->iPlayer; i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPInst->aiPlayerID[i]);
		if (!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}

		span_strong_war_award_one( pstEnv, pstPlayer, iWinCamp, pstDef);
	}
	return 0;
}

int span_strong_pworld_tick(ZONESVRENV* pstEnv, ZONEPWORLD *pstZonePworld, 
								ZONEPWORLDINST *pstPInst, int iFlag)
{
	RSTRONGONE *pstOne;
	SPANSTRONGDEF *pstDef;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	ZONEMAPINST *pstMInst;
	int iRes;
	int iNotify = 0;
	int iWin;
	
	if (!strong_is_span(pstZonePworld))
	{
		return 0;
	}

	if (pstPInst->bOver  == 1)
	{
		return 0;
	}

	pstOne =  span_strong_one_find(pstEnv, pstZonePworld->stPworldDef.StrongMapID);
	if (!pstOne)
	{
		return 0;
	}

	pstDef = span_strong_def_find(pstEnv,  pstZonePworld->stPworldDef.StrongMapID);
	if (!pstDef)
	{
		return 0;
	}

	pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPInst->astMapInstIdx[0]);
	if (!pstMInst)
	{
		return 0;
	}

	// 添加资源
	span_strong_pworld_res_add(pstEnv, pstDef,pstOne);

	// 副本信息
	span_strong_pworld_info( pstEnv, NULL, 0, pstMInst, pstZonePworld->stPworldDef.StrongMapID);

	if (iFlag)
	{
		if (iCurr + 5*60 >= pstPInst->tEnd)
		{
			if (!(pstPInst->iStrongFlag& 1))
			{
				z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE, 
						ZONEERR_BROADMAP318);
				pstPInst->iStrongFlag|= 1;		
			}
		}
		else if (iCurr + 10*60 >= pstPInst->tEnd)
		{
			if (!(pstPInst->iStrongFlag& 2))
			{
				z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE, 
						ZONEERR_BROADMAP319);
				pstPInst->iStrongFlag|= 2;		
			}
		}
		else if (iCurr + 20*60 >= pstPInst->tEnd)
		{
			if (!(pstPInst->iStrongFlag& 4))
			{
				z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE, 
						ZONEERR_BROADMAP320);
				pstPInst->iStrongFlag|= 4;		
			}
		}
		
		if (	pstOne->Pworld.AtkRes >= pstDef->ResLimit || 
			pstOne->Pworld.DefRes >= pstDef->ResLimit)
		{
			z_pworld_over(pstEnv,pstPInst);	
			pstPInst->iStrongEnd = iCurr+30;

			if (pstOne->Pworld.DefRes >= pstDef->ResLimit)
			{
				// 防守成功
				iWin = 1;
			}
			else
			{
				span_strong_owner_chg(pstEnv,pstOne, pstOne->Atk.ClanName,
										pstOne->Atk.ClanID, pstOne->Atk.SvrID);
				iWin = 2;
			}

			z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_SVR, 
						ZONEERR_BROADMAP321, pstOne->Owner.ClanName);
			span_strong_war_all_notify( pstEnv, pstPInst, iWin,pstZonePworld->stPworldDef.StrongMapID);
			span_strong_war_award(pstEnv, pstPInst,pstDef,iWin);
			iNotify = 1;

		}
	}
	else
	{
		z_pworld_over(pstEnv,pstPInst);	
		pstPInst->iStrongEnd = iCurr+30;
		// 各种提示 todo
		if (pstOne->Pworld.AtkRes > pstOne->Pworld.DefRes)
		{
			span_strong_owner_chg(pstEnv,pstOne, pstOne->Atk.ClanName,
										pstOne->Atk.ClanID, pstOne->Atk.SvrID);
			iRes = pstOne->Pworld.AtkRes;
			iWin = 2;
		}
		else
		{
			// 防守成功
			iRes = pstOne->Pworld.DefRes;
			iWin = 1;
		}

		z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_SVR, 
				ZONEERR_BROADMAP322, pstOne->Owner.ClanName,iRes);
		span_strong_war_all_notify( pstEnv, pstPInst, iWin, pstZonePworld->stPworldDef.StrongMapID);
		iNotify = 1;
		span_strong_war_award(pstEnv, pstPInst,pstDef,iWin);

	}

	if (iNotify)
	{
		memset(&pstOne->Pworld, 0, sizeof(pstOne->Pworld));
		memset(&pstOne->Atk, 0, sizeof(pstOne->Atk));
		pstEnv->pstRegionGlobalInfo->stStat.iDirty = 2;
		span_strong_one_world_notify(pstEnv,pstOne);
	}
	return 0;
}



int span_strong_refresh_map_inst(ZONESVRENV* pstEnv,ZONEPWORLDINST *pstZInst,ZONEMAPINST* pstMInst, int StrongMapID)
{
	int i;
	SPANSTRONGDEF *pstDef;
	RSTRONGONE *pstOne = NULL;
	Monster *pstMon;
	MONSTERDEF *pstMonDef;


	pstDef = span_strong_def_find(pstEnv, StrongMapID);
	if (!pstDef)
	{
		return -1;
	}

	pstOne = span_strong_one_find(pstEnv, StrongMapID);
	if (!pstOne)
		return -1;
	
	pstOne->Pworld.MonNum = 0;

	// 刷资源怪
	for (i=0; i<MAX_RES_MON; i++)
	{
		if (pstDef->ResMon[i].MonID == 0)
		{
			break;
		}

		if ((pstMon = span_strong_new_res_mon(pstEnv,pstZInst,pstMInst,
							pstDef->ResMon[i].MonID, &pstDef->ResMon[i].Pos, 0)))
		{
			pstMonDef =  z_get_mon_def(pstEnv, pstMon);
			if (pstMonDef && pstMonDef->MiscType&MON_MISC_TYPE_STRONG_RES)
			{
				pstOne->Pworld.Mon[pstOne->Pworld.MonNum].Flag = 1;
			}
			else
			{
				pstOne->Pworld.Mon[pstOne->Pworld.MonNum].Flag = 0;
			}
			
			pstOne->Pworld.Mon[pstOne->Pworld.MonNum].MemID = pstMon->iID;
			pstOne->Pworld.Mon[pstOne->Pworld.MonNum].X = pstDef->ResMon[i].Pos.X;
			pstOne->Pworld.Mon[pstOne->Pworld.MonNum].Y = pstDef->ResMon[i].Pos.Y;
			pstOne->Pworld.Mon[pstOne->Pworld.MonNum].Camp = 0;
			pstOne->Pworld.MonNum ++;
		}
	}

	// 刷复活点怪---防守方
	span_strong_new_res_mon(pstEnv,pstZInst,pstMInst,
							pstDef->ReviveMonID[0].MonID, &pstDef->ReviveMonID[0].Pos, 1);

	// 刷复活点怪---进攻方
	span_strong_new_res_mon(pstEnv,pstZInst,pstMInst,
							pstDef->ReviveMonID[1].MonID, &pstDef->ReviveMonID[1].Pos, 2);
	return 0;
}

int span_strong_get_map_pos(ZONESVRENV* pstEnv, int iStrongID, MapIndex *pstMapIdx, 
								RESPOS *pstPos, Player *pstPlayer, int iRevive)
{
	RSTRONGONE* pstOne;
	int iClanID = pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id;
	int iSvrID = pstPlayer->stRoleData.WorldID;

	pstOne = span_strong_one_find(pstEnv, iStrongID);
	if (!pstOne)
	{
		return -1;
	}

	// 防守方pos
	if ( span_strong_clan_matching(iClanID, iSvrID, pstOne->Owner.ClanID, pstOne->Owner.SvrID))
	{
		if (iRevive)
		{
			*pstPos = pstMapIdx->stMapDef.RevivePs[0];
		}
		else
		{
			*pstPos = pstMapIdx->stMapDef.TransPos[0];
		}
		
	}
	// 进攻方pos
	else if (span_strong_clan_matching(iClanID, iSvrID, pstOne->Atk.ClanID, pstOne->Atk.SvrID))
	{
		if (iRevive)
		{
			*pstPos = pstMapIdx->stMapDef.RevivePs[1];
		}
		else
		{
			*pstPos = pstMapIdx->stMapDef.TransPos[1];
		}
	}
	else
	{
		return -1;
	}
	
	return 0;
}

int span_strong_get_camp(ZONESVRENV* pstEnv, Player *pstPlayer, int iStrongID)
{
//	RSTRONGINFO*pstInfo = &pstEnv->pstRegionGlobalInfo->stGlobal.Misc.RStrongInfo;
	int iSvrID = pstPlayer->stRoleData.WorldID;
	int iClanID = pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id;
	RSTRONGONE *pstOne;

	pstOne = span_strong_one_find( pstEnv, iStrongID);
	if (!pstOne)
		return 0;

	
	if (span_strong_clan_matching(iClanID, iSvrID, pstOne->Owner.ClanID, pstOne->Owner.SvrID))
	{
		return 1;
	}
	else if (span_strong_clan_matching(iClanID, iSvrID, pstOne->Atk.ClanID, pstOne->Atk.SvrID))
	{
		return 2;
	}
	
	return 0;
}

int strong_get_enter_pos(ZONESVRENV* pstEnv, ZONEPWORLD *pstPworld, MapIndex *pstMapIdx, 
								RESPOS *pstPos, Player *pstPlayer, int iRevive)
{
//	int iCamp = 0;
	
	if (!pstPworld || pstPworld->stPworldDef.StrongMapID <= 0)
	{
		return 0;
	}

	if (strong_is_normal(pstPworld))
	{
		if (strong_get_map_pos( pstEnv,pstPworld->stPworldDef.StrongMapID,pstMapIdx, pstPos, pstPlayer,iRevive) < 0)
		{
			return -1;
		}
	}
	else
	{
		if (span_strong_get_map_pos(pstEnv,pstPworld->stPworldDef.StrongMapID, pstMapIdx, pstPos, pstPlayer, iRevive) < 0)
		{
			return -1;
		}	
	}
	return 0;
}

int span_strong_enter_init(ZONESVRENV * pstEnv, Player *pstPlayer,ZONEPWORLD *pstPworld)
{
	int iCamp = 0;
	
	iCamp = span_strong_get_camp(pstEnv,pstPlayer, pstPworld->stPworldDef.StrongMapID);
	if (iCamp <= 0 || iCamp > 2)
	{
		return -1;
	}

	pstPlayer->stOnline.stWarOL.cCampIdx = iCamp;
	pstPlayer->stOnline.stWarOL.iLevel = pstPlayer->stRoleData.Level;
	//pstPlayer->stOnline.stWarOL.cPloyType = PWORLD_POLY_BATTLE;
	pstPlayer->stOnline.stWarOL.tFightTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	return 0;	
}

int span_strong_pworld_info(ZONESVRENV* pstEnv, Player *pstPlayer, int iSingle, ZONEMAPINST *pstMInst, int iStrongID)
{
	CSPKG stPkg;
	CSSPANSTRONGSVR *pstSvr  = &stPkg.Body.SpanStrongSvr;
	RSTRONGONE *pstOne;
	int i;

	memset(pstSvr, 0, sizeof(*pstSvr));
	pstSvr->Cmd = SPAN_STRONG_SVR_PWORLD_INFO;

	pstOne = span_strong_one_find( pstEnv,iStrongID);
	if (!pstOne)
		return -1;

	pstSvr->Data.Pworld.MonNum = pstOne->Pworld.MonNum;
	for (i=0; i<pstOne->Pworld.MonNum; i++)
	{
		pstSvr->Data.Pworld.Mon[i] = pstOne->Pworld.Mon[i];
	}

	pstSvr->Data.Pworld.AtkRes = pstOne->Pworld.AtkRes;
	pstSvr->Data.Pworld.DefRes = pstOne->Pworld.DefRes;
	pstSvr->Data.Pworld.AtkSvrID = pstOne->Atk.SvrID;
	pstSvr->Data.Pworld.DefSvrID = pstOne->Owner.SvrID;
	strncpy(pstSvr->Data.Pworld.AtkClanName, pstOne->Atk.ClanName, sizeof(pstSvr->Data.Pworld.AtkClanName));
	strncpy(pstSvr->Data.Pworld.DefClanName, pstOne->Owner.ClanName, sizeof(pstSvr->Data.Pworld.DefClanName));
	
	Z_CSHEAD_INIT(&stPkg.Head, SPAN_STRONG_SVR);
	if (iSingle)
	{
		if (pstPlayer)
		{
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		}
	}
	else
	{
		if (pstMInst)
		{
			map_broadcast(pstEnv->pstAppCtx, pstEnv, pstMInst, &stPkg);
		}
	}
	return 0;
}


int span_strong_fight_event(ZONESVRENV* pstEnv, RSTRONGONE *pstOne)
{
	SPANSTRONGDEF *pstDef;
	ZONEPWORLDINST *pstPInst;
	ZONEIDX stIdx;
	ZONEPWORLD *pstPworld;
	int iBusID = 0;
	
	pstDef = span_strong_def_find(pstEnv, pstOne->StrongID);
	if (!pstDef)
	{
		return -1;
	}

	if (	pstOne->Owner.ClanID <= 0 ||
		pstOne->Atk.ClanID <= 0)
	{
		return -1;
	}
	
	// 创建副本	
	stIdx.iID = pstDef->PworldID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (pstPworld == NULL || pstPworld->stPworldDef.Type != PWORLD_TYPE_PLOY)
	{
		return -1;
	}

	pstPInst = z_new_ploy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstPworld, pstPworld->stPworldDef.Timeout*60,0);
	if (!pstPInst)
	{
		return -1;
	}

	// 防守方1正营
	// 进攻方2正营
	pstPInst->stPworldCamp.stCampInfo[0].iSvrBusID = pstOne->Owner.SvrID;
	pstPInst->stPworldCamp.stCampInfo[0].stClanID.Id = pstOne->Owner.ClanID;
	pstPInst->stPworldCamp.stCampInfo[1].iSvrBusID = pstOne->Atk.SvrID;
	pstPInst->stPworldCamp.stCampInfo[1].stClanID.Id = pstOne->Atk.ClanID;
	pstPInst->stPworldCamp.iCampNum = 2;
	
	pstOne->Pworld.InstID = pstPInst->iPworldInstID;
	pstOne->Pworld.PworldWid = pstPInst->ullPworldWID;

	// 通知两个服对应的公会副本开启todo:
	//GET_WORLD_ENTITY_BY_SVRID(iBusID, pstEnv->pstAppCtx->iId, FUNC_ZONE_SVR, pstOne->Owner.SvrID);
	iBusID = span_strong_get_zone_svr_id(pstEnv->pstAppCtx->iId, pstOne->Owner.SvrID);
	z_sys_strf_span_broadcast_clan(pstEnv, iBusID, pstOne->Owner.ClanID,SYS_CLAN,
									"%s%s", pstDef->Name, LJY_STRONG703);

	//GET_WORLD_ENTITY_BY_SVRID(iBusID, pstEnv->pstAppCtx->iId, FUNC_ZONE_SVR, pstOne->Atk.SvrID);
	iBusID = span_strong_get_zone_svr_id(pstEnv->pstAppCtx->iId, pstOne->Atk.SvrID);
	z_sys_strf_span_broadcast_clan(pstEnv, iBusID, pstOne->Atk.ClanID,SYS_CLAN, 
									"%s%s", pstDef->Name, LJY_STRONG703);

	pstEnv->pstRegionGlobalInfo->stStat.iDirty = 2;
	return 0;
}

int span_strong_tick(ZONESVRENV* pstEnv)
{
	int i,j;
	RSTRONGINFO*pstInfo = &pstEnv->pstRegionGlobalInfo->stGlobal.Misc.RStrongInfo;
	SPANSTRONGDEF *pstDef;
	struct tm stNow;
	time_t tNow = pstEnv->pstAppCtx->stCurr.tv_sec;
	int iWeek,iNextWeek;
	int iHour;
	RSTRONGONE *pstOne;
	int iNotify = 0;
	time_t tOpen;  // 开搞时间

	return 0;
	
	localtime_r(&tNow, &stNow);
	iWeek = stNow.tm_wday;
	if (iWeek == 0)
		iWeek = 7;
	iHour = stNow.tm_hour;
	
	stNow.tm_hour = SPAN_FIGHT_HOUR;
	stNow.tm_min = SPAN_FIGHT_MIN;
	tOpen = mktime(&stNow);

	for (i=0; i<pstInfo->Num; i++)
	{
		iNextWeek = -1;
		pstOne = &pstInfo->Strong[i];

		if (pstOne->IsOpen == STRONG_CLOSE)
			continue;
		
		pstDef = 	span_strong_def_find(pstEnv,pstOne->StrongID);
		if (!pstDef)
			continue;

		for (j=0; j<MAX_SPAN_STRONG_WEEK; j++)
		{
			if (iWeek <= g_iSpanWeek[j])
			{
				iNextWeek = g_iSpanWeek[j];
				break;
			}
		}

		if (iNextWeek == -1)
			iNextWeek = g_iSpanWeek[0];

		// 暗拍时间
		if (iWeek+1 == iNextWeek)
		{
			if (pstOne->Stat != STRONG_STAT_SALE)
			{
				memset(&pstOne->Sale, 0, sizeof(pstOne->Sale));
				pstOne->Stat = STRONG_STAT_SALE;
				iNotify = 1;
			}
		}
		else if (iWeek == iNextWeek)
		{
			if (iHour < SPAN_SELECT_HOUR)
			{
				if (pstOne->Stat != STRONG_STAT_TONGJI)
				{
					pstOne->Stat = STRONG_STAT_TONGJI;
					iNotify = 1;
				}
			}
			else 
			{
				if (tNow < tOpen)
				{
					if (	pstOne->Stat != STRONG_STAT_SELECT )
					{
						span_strong_select_event(pstEnv, pstOne); 
						pstOne->Stat = STRONG_STAT_SELECT;
						iNotify = 1;
					}
				}
				else
				{
					if (tNow < tOpen+60*60)
					{
						if (pstOne->Stat == STRONG_STAT_SELECT)
						{
							span_strong_fight_event( pstEnv, pstOne);
							pstOne->Stat = STRONG_STAT_FIGHT;
							iNotify = 1;
						}

						// 副本结束了 
						if (pstOne->Pworld.InstID <= 0)
						{
							if (pstOne->Stat != STRONG_STAT_IDLE)
							{
								pstOne->Stat = STRONG_STAT_IDLE;
								iNotify = 1;
							}	
						}
					}
					else
					{
						// 副本实例还在不搞
						if (pstOne->Pworld.InstID > 0)
						{
							continue;
						}
						
						if (pstOne->Stat != STRONG_STAT_IDLE)
						{
							pstOne->Stat = STRONG_STAT_IDLE;
							iNotify = 1;
						}
					}
				}
			}
		}
		else
		{
			if (pstOne->Stat != STRONG_STAT_IDLE)
			{
				pstOne->Stat = STRONG_STAT_IDLE;
				iNotify = 1;
			}
		}
	}

	//  所有的状态变化都要通知每个服
	if (iNotify)
	{
		span_strong_all_world_notify( pstEnv);
	}

	return 0;
}

int span_global_get(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;
	REGIONGLOBAL stGlobal;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	RegionGlobalInfo *pstInfo = pstEnv->pstRegionGlobalInfo;
	
	stDBAsyn.iCmd = DB_REGION_GLOBAL_SELECT;
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, "RegionGlobal", -1, &stDBAsyn);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 0;

	memset(&stGlobal, 0, sizeof(stGlobal));

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stGlobal;
	stMetaData.iBuff = sizeof(stGlobal);
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iRegionDBID))
	{
		return -1;
	}

	pstInfo->stStat.iStat = REGION_GLOBAL_STAT_REQ;
	return 0;
}

SPANSTRONGDEF * span_strong_def_find(ZONESVRENV* pstEnv, int iStrongID)
{
	SPANSTRONGDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.StrongID = iStrongID;

	iIndex = bsearch_int(&stDef, pstObj->sSpanStrongDef, pstObj->iSpanStrongDef, SPAN_STRONG_DEF_SIZE, &iEqu);
	if (iEqu)
	{
		return (SPANSTRONGDEF *)&pstObj->sSpanStrongDef[iIndex];
	}
	return NULL;
}

int span_strong_init(ZONESVRENV* pstEnv)
{
	int i,j;
	RSTRONGINFO *pstInfo = &pstEnv->pstRegionGlobalInfo->stGlobal.Misc.RStrongInfo;
	SPANSTRONGDEF *pstDef;

	// 正向检查
	for (i=pstInfo->Num-1; i>=0; i--)
	{	
		for (j=0;j<pstEnv->pstZoneObj->iSpanStrongDef; j++)
		{
			pstDef = (SPANSTRONGDEF *)&pstEnv->pstZoneObj->sSpanStrongDef[j];
			if (pstDef->StrongID == pstInfo->Strong[i].StrongID)
			{
				break;
			}
		}

		if (j >= pstEnv->pstZoneObj->iSpanStrongDef)
		{
			pstInfo->Num --;
			if (pstInfo->Num != i)
			{
				pstInfo->Strong[i] = pstInfo->Strong[pstInfo->Num];
			}
		}
	}

	
	// 反向检查
	for (i=0;i<pstEnv->pstZoneObj->iSpanStrongDef; i++)
	{	
		pstDef = (SPANSTRONGDEF *)&pstEnv->pstZoneObj->sSpanStrongDef[i];
		for (j=0; j<pstInfo->Num; j++)
		{
			if (pstDef->StrongID == pstInfo->Strong[j].StrongID)
			{
				break;	
			}	
		}

		if (j>=pstInfo->Num && pstInfo->Num < MAX_SPAN_STRONG_NUM)
		{
			memset(&pstInfo->Strong[pstInfo->Num], 0, sizeof(pstInfo->Strong[pstInfo->Num]));
			pstInfo->Strong[pstInfo->Num].StrongID = pstDef->StrongID;
			pstInfo->Num ++;
		}
	}

	//  检查据点开启情况 todo:
	if (pstEnv->pstConf->OpenSpanStrongLvl > 0)
	{
		for (i=0; i<pstInfo->Num; i++)
		{
			pstDef = span_strong_def_find( pstEnv, pstInfo->Strong[i].StrongID);
			if (!pstDef)
			{	
				continue;
			}

			if (pstDef->Lvl <= pstEnv->pstConf->OpenSpanStrongLvl &&
				pstInfo->Strong[i].IsOpen == STRONG_CLOSE)
			{
				pstInfo->Strong[i].IsOpen = STRONG_OPEN;
			}
		}
	}
	
	
	return 0;
}


int span_global_res(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes)
{
	REGIONGLOBAL stGlobal;

	if (!is_battle_sparta_svr( pstEnv))
	{
		return 0;
	}
	
	if (1 == pstSelectRes->SelectResult.ResultNum)
	{
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stGlobal, sizeof(stGlobal)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,	"span_global_res torm_fetch_one_meta fail");
			return 0;
		}

		pstEnv->pstRegionGlobalInfo->stGlobal = stGlobal;
		
	}
	else
	{
		memset(pstEnv->pstRegionGlobalInfo, 0, sizeof(*pstEnv->pstRegionGlobalInfo));
		pstEnv->pstRegionGlobalInfo->stStat.iDirty = 2; // 没有数据的话立即插入一条
	}

	pstEnv->pstRegionGlobalInfo->stStat.iStat = REGION_GLOBAL_STAT_FINI;

	// 这里应该要初始化据点数据了:todo
	span_strong_init(pstEnv);

	// 初始化完据点,要发给各个服务器
	span_strong_all_world_notify(pstEnv);
	
	return 0;
}


int span_global_update(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	if (!is_battle_sparta_svr( pstEnv))
	{
		return 0;
	}
		
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	UNUSED(pstAppCtx);

	
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, "RegionGlobal", -1, NULL);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 1;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)(&(pstEnv->pstRegionGlobalInfo->stGlobal));
	stMetaData.iBuff = sizeof(pstEnv->pstRegionGlobalInfo->stGlobal);
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iRegionDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iRegionDBID))
	{
		return -1;
	}	

	pstEnv->pstRegionGlobalInfo->stStat.iDirty = 0;
	return 0;
}

int span_global_init(ZONESVRENV* pstEnv)
{
	if (pstEnv->pstRegionGlobalInfo->stStat.iStat == REGION_GLOBAL_STAT_FINI)
	{
		// 如果在battle,还是要初始化数据和所有服通知
		// 因为配置文件,表格数据都有可能变化
		if (is_battle_sparta_svr( pstEnv))
		{
			span_strong_init(pstEnv);
			span_strong_all_world_notify(pstEnv);
		}
		return 0;
	}

	// battle	--去数据库拉取
	// zone	--去找battle要
	if (is_battle_sparta_svr( pstEnv))
	{
		span_global_get(pstEnv->pstAppCtx,  pstEnv);
	}
	else if (INST_MODE_ZONE_SVR == pstEnv->iInstMode)
	{
		span_global_info_req( pstEnv);
	}
	
	return 0;
}

int span_global_tick(ZONESVRENV* pstEnv)
{
	static int iCount = 0;

	if (!is_battle_sparta_svr( pstEnv))
	{
		return 0;
	}

	if (pstEnv->pstRegionGlobalInfo->stStat.iStat != REGION_GLOBAL_STAT_FINI)
	{
		return 0;
	}

	iCount ++;

	if (2 == pstEnv->pstRegionGlobalInfo->stStat.iDirty)
	{
		span_global_update(pstEnv->pstAppCtx,  pstEnv);
	}
	else if (iCount%600 == 0 && 1 == pstEnv->pstRegionGlobalInfo->stStat.iDirty) //定时
	{
		span_global_update(pstEnv->pstAppCtx, pstEnv);
	}

	if (iCount%3 == 0)
	{
		span_strong_tick(pstEnv);
	}
	return 0;
}

int span_msg_world_notify(ZONESVRENV* pstEnv, SSPKG *pstPkg, unsigned int iSrcID)
{
	unsigned int iDstList[1024];
	int iVecCnt = 1024;
	int i;
	unsigned char ucEntity = 0;
	TDRDATA stNet;
	int iDst;

	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = pstEnv->szSendBuffer;
	if(ss_pack(pstEnv->pstSSHan, &stNet, pstPkg, 0) < 0)
	{
		return -1;
	}
	
	if ( tbus_get_dst_list(pstEnv->pstAppCtx->iBus, iDstList, &iVecCnt) < 0)
	{
		return -1;
	}

	for (i=0; i<iVecCnt; i++)
	{
		GET_FUNC_ENTITY(ucEntity,iDstList[i]);
		if (ucEntity == FUNC_ZONE_SVR && iDstList[i] != iSrcID)
		{
			iDst = iDstList[i];
			tbus_send(pstEnv->pstAppCtx->iBus,  &pstEnv->pstAppCtx->iId, &iDst, stNet.pszBuff, stNet.iBuff, 0);
		}
	}
	return 0;
}

// battle->zone
int span_strong_all_world_notify(ZONESVRENV* pstEnv)
{
	SSPKG stPkg;
	SSSPANSTRONGRES *pstRes = &stPkg.Body.SpanStrongRes;
	
	Z_SSHEAD_INIT(&stPkg.Head, SPAN_STRONG_RES);
	pstRes->OpType = SS_SPAN_STRONG_RES_ALLINFO;
	pstRes->Data.All.AllInfo = pstEnv->pstRegionGlobalInfo->stGlobal.Misc.RStrongInfo;
	
	span_msg_world_notify(pstEnv, &stPkg, 0);
	return 0;
}

// battle->zone
int span_strong_one_world_notify(ZONESVRENV* pstEnv, RSTRONGONE *pstOne)
{
	SSPKG stPkg;
	SSSPANSTRONGRES *pstRes = &stPkg.Body.SpanStrongRes;
	
	Z_SSHEAD_INIT(&stPkg.Head, SPAN_STRONG_RES);
	pstRes->OpType = SS_SPAN_STRONG_RES_ONE;
	
	pstRes->Data.One.OneInfo = *pstOne;
	
	span_msg_world_notify(pstEnv, &stPkg, 0);
	return 0;
}

// zone->battle
int span_global_info_req(ZONESVRENV* pstEnv)
{
	SSPKG stPkg;
	SSSPANSTRONGREQ *pstReq = &stPkg.Body.SpanStrongReq;

	Z_SSHEAD_INIT(&stPkg.Head, SPAN_STRONG_REQ);
	pstReq->OpType = SS_SPAN_STRONG_REQ_ALLINFO;
	pstReq->Data.StrongInfoReq.MsgSrcID = pstEnv->pstAppCtx->iId;

	z_ssmsg_send(pstEnv, &stPkg, pstEnv->iBattleSpartaID);
	return 0;
}

int span_strong_chg_name(ZONESVRENV* pstEnv, int iType, const char* pszOld, const char *pszNew)
{
	SSPKG stPkg;
	SSSPANSTRONGREQ *pstReq = &stPkg.Body.SpanStrongReq;

	Z_SSHEAD_INIT(&stPkg.Head, SPAN_STRONG_REQ);
	pstReq->OpType = SS_SPAN_STRONG_REQ_CHGNAME;

	pstReq->Data.ChgName.Type = iType;
	pstReq->Data.ChgName.MsgSrcID = pstEnv->pstAppCtx->iId;
	strncpy(pstReq->Data.ChgName.OldName, pszOld, sizeof(pstReq->Data.ChgName.OldName));
	strncpy(pstReq->Data.ChgName.NewName, pszNew, sizeof(pstReq->Data.ChgName.NewName));

	z_ssmsg_send(pstEnv, &stPkg, pstEnv->iBattleSpartaID);
	return 0;
}

int span_strong_clan_destory(ZONESVRENV* pstEnv, const char* pszClanName)
{
	SSPKG stPkg;
	SSSPANSTRONGREQ *pstReq = &stPkg.Body.SpanStrongReq;

	Z_SSHEAD_INIT(&stPkg.Head, SPAN_STRONG_REQ);
	pstReq->OpType = SS_SPAN_STRONG_CLAN_DESTORY;

	pstReq->Data.Destory.MsgSrcID = pstEnv->pstAppCtx->iId;
	strncpy(pstReq->Data.Destory.ClanName, pszClanName, sizeof(pstReq->Data.Destory));
	z_ssmsg_send(pstEnv, &stPkg, pstEnv->iBattleSpartaID);
	return 0;
}

// battle->zone
int span_global_info_res(ZONESVRENV* pstEnv, int iDstID)
{
	SSPKG stPkg;
	SSSPANSTRONGRES *pstRes = &stPkg.Body.SpanStrongRes;
	
	Z_SSHEAD_INIT(&stPkg.Head, SPAN_STRONG_RES);
	pstRes->OpType = SS_SPAN_STRONG_RES_ALLINFO;

	if (pstEnv->pstRegionGlobalInfo->stStat.iStat != REGION_GLOBAL_STAT_FINI)
	{
		memset(&pstRes->Data.All.AllInfo, 0, sizeof(pstRes->Data.All.AllInfo));	
	}
	else
	{
		pstRes->Data.All.AllInfo = pstEnv->pstRegionGlobalInfo->stGlobal.Misc.RStrongInfo;
	}
	z_ssmsg_send(pstEnv, &stPkg, iDstID);
	return 0;
}

int span_strong_sale_ss_req(ZONESVRENV* pstEnv, ZoneClan*pstClan, Player *pstPlayer, SPANSTRONGSALE *pstSale)
{
	SSPKG stPkg;
	SSSPANSTRONGREQ *pstReq = &stPkg.Body.SpanStrongReq;

	pstReq->OpType = SS_SPAN_STRONG_REQ_SALE;
	pstReq->Data.StrongSaleReq.SvrID = pstPlayer->stRoleData.WorldID;
	pstReq->Data.StrongSaleReq.ClanID = pstClan->stClan.ClanProf.Id;
	pstReq->Data.StrongSaleReq.StrongID = pstSale->StrongID;
	pstReq->Data.StrongSaleReq.Money = pstSale->Money;
	strncpy(pstReq->Data.StrongSaleReq.ClanName, pstClan->stClan.ClanProf.Name, 
				sizeof(pstReq->Data.StrongSaleReq.ClanName));
	strncpy(pstReq->Data.StrongSaleReq.RoleName, pstPlayer->stRoleData.RoleName,
				sizeof(pstReq->Data.StrongSaleReq.ClanName));

	Z_SSHEAD_INIT(&stPkg.Head, SPAN_STRONG_REQ);
	return z_ssmsg_send(pstEnv, &stPkg, pstEnv->iBattleSpartaID);
}

int span_strong_gm_occ_ss_req(ZONESVRENV* pstEnv, ZoneClan*pstClan, int iStrongID)
{
	SSPKG stPkg;
	SSSPANSTRONGREQ *pstReq = &stPkg.Body.SpanStrongReq;

	pstReq->OpType = SS_SPAN_STRONG_OCC;
	pstReq->Data.Occ.ClanID = pstClan->stClan.ClanProf.Id;
	pstReq->Data.Occ.StrongID = iStrongID;
	pstReq->Data.Occ.MsgSrcID = pstEnv->pstAppCtx->iId;
	STRNCPY(pstReq->Data.Occ.ClanName, pstClan->stClan.ClanProf.Name, CCH(pstReq->Data.Occ.ClanName));

	Z_SSHEAD_INIT(&stPkg.Head, SPAN_STRONG_REQ);
	return z_ssmsg_send(pstEnv, &stPkg, pstEnv->iBattleSpartaID);
}

// battle收到竞拍后
int span_strong_sale_res(ZONESVRENV * pstEnv, SSSTRONGSALEREQ *pstSale)
{
//	RSTRONGINFO *pstInfo = &pstEnv->stRegionGlobalInfo.stGlobal.Misc.RStrongInfo;
	RSTRONGONE *pstOne;
//	int i;
	RSALEONE stSaleOne;
	RSALEPLAYERONE*pstSalePlayerOne;
	
	pstOne = span_strong_one_find(pstEnv, pstSale->StrongID);
	if (!pstOne)
	{
		return 0;
	}

	if (pstOne->Sale.iPlayer >= MAX_RSALE_NUM)
	{
		return 0;
	}

	stSaleOne.Sale = pstSale->Money;
	stSaleOne.SvrID = pstSale->SvrID;
	stSaleOne.ClanID = pstSale->ClanID;
	strncpy(stSaleOne.ClanName, pstSale->ClanName, sizeof(stSaleOne.ClanName));
	
	if (pstOne->Sale.iSale == 0)
	{
		pstOne->Sale.iSale  = 1;
		pstOne->Sale.RSale[0] = stSaleOne;
	}
	else if (pstOne->Sale.iSale == 1)
	{
		if (pstSale->Money > pstOne->Sale.RSale[0].Sale)
		{
			pstOne->Sale.RSale[1]	= pstOne->Sale.RSale[0];
			pstOne->Sale.RSale[0] = stSaleOne;
		}
		else
		{
			pstOne->Sale.RSale[1] = stSaleOne;
		}
		pstOne->Sale.iSale = 2;
	}
	else if (pstOne->Sale.iSale == 2)
	{
		if (pstSale->Money > pstOne->Sale.RSale[0].Sale)
		{
			pstOne->Sale.RSale[1]	= pstOne->Sale.RSale[0];
			pstOne->Sale.RSale[0] = stSaleOne;
		}
		else if (pstSale->Money > pstOne->Sale.RSale[1].Sale)
		{
			pstOne->Sale.RSale[1] = stSaleOne;
		}
	}
	
	pstSalePlayerOne = &pstOne->Sale.RPlayer[pstOne->Sale.iPlayer];
	pstOne->Sale.iPlayer ++;

	pstSalePlayerOne->ClanID = pstSale->ClanID;
	pstSalePlayerOne->SvrID = pstSale->SvrID;
	pstSalePlayerOne->Sale = pstSale->Money;
	strncpy(pstSalePlayerOne->RoleName,pstSale->RoleName, sizeof(pstSalePlayerOne->RoleName));

	// 通知zone更新下信息
	span_strong_one_world_notify(pstEnv, pstOne);
	pstEnv->pstRegionGlobalInfo->stStat.iDirty = 2;
	return 0;
}


int span_strong_info_clear(ZONESVRENV * pstEnv)
{
	RSTRONGINFO *pstInfo = &pstEnv->pstRegionGlobalInfo->stGlobal.Misc.RStrongInfo;
	int i;
	int iOffset = offsetof(RSTRONGONE, Owner);

	for (i=0; i<pstInfo->Num; i++)
	{
		memset(((char*)&pstInfo->Strong[i]+iOffset), 0, sizeof(RSTRONGONE)-iOffset);	
	}

	span_strong_all_world_notify( pstEnv);
	pstEnv->pstRegionGlobalInfo->stStat.iDirty = 2;
	return 0;
}

void span_strong_chg_name_in(ZONESVRENV * pstEnv, SSSTRONGCHGNAMEREQ *pstChg)
{
	RSTRONGINFO *pstInfo = &pstEnv->pstRegionGlobalInfo->stGlobal.Misc.RStrongInfo;
	int i,j;
	RSTRONGONE *pstOne;
	int iSvrID;
	int iChg = 0;

	GET_WORLD_ID(iSvrID, pstChg->MsgSrcID);

	if (pstChg->Type == 0)
	{
		for (i=0; i<pstInfo->Num; i++)
		{
			pstOne = &pstInfo->Strong[i];
			for (j=0; j<pstOne->Sale.iPlayer; j++)
			{
				if (	pstOne->Sale.RPlayer[j].SvrID == iSvrID &&
					strcmp(pstChg->OldName, pstOne->Sale.RPlayer[j].RoleName) == 0)
				{
					strcpy(pstOne->Sale.RPlayer[j].RoleName, pstChg->NewName);	
					iChg = 1;
				}	
			}
			
		}
	}
	else if (pstChg->Type == 1)
	{
		for (i=0; i<pstInfo->Num; i++)
		{
			pstOne = &pstInfo->Strong[i];
			if (	pstOne->Owner.SvrID == iSvrID &&
				strcmp(pstChg->OldName, pstOne->Owner.ClanName) == 0)
			{
				strcpy(pstOne->Owner.ClanName, pstChg->NewName);	
				iChg = 1;
			}

			if (	pstOne->Atk.SvrID == iSvrID &&
				strcmp(pstChg->OldName, pstOne->Atk.ClanName) == 0)
			{
				strcpy(pstOne->Atk.ClanName, pstChg->NewName);	
				iChg = 1;
			}

			for (j=0; j<pstOne->Sale.iSale; j++)
			{
				if (	pstOne->Sale.RSale[j].SvrID == iSvrID &&
					strcmp(pstChg->OldName, pstOne->Sale.RSale[j].ClanName) == 0)
				{
					strcpy(pstOne->Sale.RSale[j].ClanName, pstChg->NewName);	
					iChg = 1;
				}
			}
		}
		
	}


	if (iChg)
	{
		 span_strong_all_world_notify(pstEnv);
	}
	pstEnv->pstRegionGlobalInfo->stStat.iDirty = 2;
}

void span_strong_destory_in(ZONESVRENV * pstEnv, SSSTRONGCLANDESTORYREQ*pstDestory)
{
	RSTRONGINFO *pstInfo = &pstEnv->pstRegionGlobalInfo->stGlobal.Misc.RStrongInfo;
	int i;
	RSTRONGONE *pstOne;
	int iSvrID;
	int iChg = 0;
	ZONEPWORLDINST *pstPInst;
	ZONEMAPINST *pstMInst;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;

	GET_WORLD_ID(iSvrID, pstDestory->MsgSrcID);

	for (i=0; i<pstInfo->Num; i++)
	{
		pstOne = &pstInfo->Strong[i];
		if (pstOne->Stat == STRONG_STAT_FIGHT)
		{
			pstPInst =  z_find_pworld_inst(pstEnv->pstMapObj, pstOne->Pworld.InstID);
			if (!pstPInst)
				continue;
			pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPInst->astMapInstIdx[0]);
			if (!pstMInst)
				continue;
			
			if (	pstOne->Owner.SvrID == iSvrID &&
				strcmp(pstDestory->ClanName, pstOne->Owner.ClanName) == 0)
			{
				z_pworld_over(pstEnv,pstPInst);	
				pstPInst->iStrongEnd = iCurr+30;
				
				z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_SVR, 
						ZONEERR_BROADMAP316, 
						pstOne->Owner.ClanName, pstOne->Atk.ClanName);	
				span_strong_owner_chg(pstEnv,pstOne, pstOne->Atk.ClanName,
									pstOne->Atk.ClanID, pstOne->Atk.SvrID);
				iChg = 1;
			}
			else if (	pstOne->Atk.SvrID == iSvrID &&
					strcmp(pstDestory->ClanName, pstOne->Atk.ClanName) == 0)
			{
				z_pworld_over(pstEnv,pstPInst);	
				pstPInst->iStrongEnd = iCurr+30;
				
				z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_SVR, 
						ZONEERR_BROADMAP316, 
						pstOne->Atk.ClanName, pstOne->Owner.ClanName);	
				span_strong_owner_chg(pstEnv,pstOne, pstOne->Owner.ClanName,
									pstOne->Owner.ClanID, pstOne->Owner.SvrID);
				iChg = 1;
			}
		}
		else if (pstOne->Stat == STRONG_STAT_SALE)
		{
			if (pstOne->Sale.iSale <= 0)
				continue;

			if (pstOne->Sale.iSale == 1)
			{
				if (pstOne->Sale.RSale[0].SvrID == iSvrID &&
				strcmp(pstDestory->ClanName, pstOne->Sale.RSale[0].ClanName) == 0)
				{
					pstOne->Sale.iSale = 0;
				}
			}
			else if (pstOne->Sale.iSale == 2)
			{
				if (pstOne->Sale.RSale[0].SvrID == iSvrID &&
					strcmp(pstDestory->ClanName, pstOne->Sale.RSale[0].ClanName) == 0)
				{
					
					pstOne->Sale.RSale[0] = pstOne->Sale.RSale[1];
					pstOne->Sale.iSale = 1;
					iChg = 1;
				}
				else if (pstOne->Sale.RSale[1].SvrID == iSvrID &&
					strcmp(pstDestory->ClanName, pstOne->Sale.RSale[1].ClanName) == 0)
				{
					pstOne->Sale.iSale = 1;
					iChg = 1;
				}
			}	
		}
		else
		{
			if (	pstOne->Owner.SvrID == iSvrID &&
				strcmp(pstDestory->ClanName, pstOne->Owner.ClanName) == 0)
			{
				if (	pstOne->Atk.ClanName[0] != 0 &&
					pstOne->Atk.ClanID > 0 &&
					pstOne->Atk.SvrID > 0)
				{
					span_strong_owner_chg(pstEnv,pstOne, pstOne->Atk.ClanName,
									pstOne->Atk.ClanID, pstOne->Atk.SvrID);
				}
				else
				{
					memset(&pstOne->Owner, 0, sizeof(pstOne->Owner));
					memset(&pstOne->Atk, 0, sizeof(pstOne->Atk));
				}
				
				iChg = 1;
			}
			else if (	pstOne->Atk.SvrID == iSvrID &&
					strcmp(pstDestory->ClanName, pstOne->Atk.ClanName) == 0)
			{
				memset(&pstOne->Atk, 0, sizeof(pstOne->Atk));
				iChg = 1;
			}
		}
		
	}

	if (iChg)
	{
		 span_strong_all_world_notify(pstEnv);
	}

	pstEnv->pstRegionGlobalInfo->stStat.iDirty = 2;
}

int span_strong_gm_occ(ZONESVRENV * pstEnv,SSSTRONGOCC *pstOcc)
{
	RSTRONGONE *pstOne;
//	RSALEPLAYERONE*pstSalePlayerOne;
	
	pstOne = span_strong_one_find(pstEnv, pstOcc->StrongID);
	if (!pstOne)
	{
		return 0;
	}

	if (pstOne->IsOpen == STRONG_CLOSE)
	{
		return 0;
	}

	pstOne->Owner.ClanID = pstOcc->ClanID;
	pstOne->Owner.OccTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	GET_WORLD_ID(pstOne->Owner.SvrID, pstOcc->MsgSrcID);
	strncpy(pstOne->Owner.ClanName, pstOcc->ClanName, sizeof(pstOne->Owner.ClanName));

	span_strong_all_world_notify(pstEnv);

	pstEnv->pstRegionGlobalInfo->stStat.iDirty = 2;
	return 0;
}

// battle处理zone的请求消息
int span_strong_ss_req(ZONESVRENV * pstEnv,SSPKG *pstPkg)
{
	SSSPANSTRONGREQ *pstReq = &pstPkg->Body.SpanStrongReq;

	// 收到这个消息的必定是battle
	if (!is_battle_sparta_svr( pstEnv))
	{
		return 0;
	}

	switch(pstReq->OpType)
	{
	case SS_SPAN_STRONG_REQ_ALLINFO:
		return span_global_info_res( pstEnv, pstReq->Data.StrongInfoReq.MsgSrcID);	
		break;
	case SS_SPAN_STRONG_REQ_SALE:
		return span_strong_sale_res( pstEnv, &pstReq->Data.StrongSaleReq);
		break;
	case SS_SPAN_STRONG_REQ_CLEAR:
		return span_strong_info_clear( pstEnv);
		break;
	case SS_SPAN_STRONG_REQ_TIP:
		{
			// 测试用的
			int iBusID=0;
			iBusID = span_strong_get_zone_svr_id(pstEnv->pstAppCtx->iId, pstReq->Data.StrongTip.MsgSrcID);
			//GET_WORLD_ENTITY_BY_SVRID(iBusID, pstEnv->pstAppCtx->iId, FUNC_ZONE_SVR, 
			//								pstReq->Data.StrongTip.MsgSrcID);
			z_sys_strf_span_broadcast_clan(pstEnv, iBusID, pstReq->Data.StrongTip.ClanID,SYS_CLAN,"123456789");
		}
		break;
	case SS_SPAN_STRONG_REQ_MAIL:
		{
			// 测试用的
			//GET_WORLD_ENTITY_BY_SVRID(g_iZoneBus, pstEnv->pstAppCtx->iId, FUNC_ZONE_SVR, 
			//								pstReq->Data.StrongMail.SvrID);
			g_iZoneBus = span_strong_get_zone_svr_id(pstEnv->pstAppCtx->iId, pstReq->Data.StrongMail.SvrID);
			player_sysmail_fast_send(pstEnv, pstReq->Data.StrongMail.Name, 
					"xxoo", "xxxxxooooo", NULL, 0, 9999, NULL, 0);
			g_iZoneBus = 0;
		}
		break;
	case SS_SPAN_STRONG_REQ_CHGNAME:
		span_strong_chg_name_in( pstEnv, &pstReq->Data.ChgName);
		break;
	case SS_SPAN_STRONG_CLAN_DESTORY:
		
		span_strong_destory_in( pstEnv, &pstReq->Data.Destory);
		break;
	case SS_SPAN_STRONG_OCC:
		return span_strong_gm_occ( pstEnv,&pstReq->Data.Occ);
		break;
	default:
		return 0;
		break;
	}
	
	
	return 0;
}


// 这里不支持mail道具
// 从battle发邮件给公会领导
int span_strong_mail_in(ZONESVRENV * pstEnv, SSSTRONGMAILRES *pstMail)
{
	ZoneClan *pstClan = z_id_clan( pstEnv, pstMail->ClanID);
	int i;

	if (!pstClan)
		return 0;

	player_sysmail_fast_send(pstEnv, pstClan->stClan.ClanBrief.Leader.RoleName, 
				pstMail->Title, pstMail->Text, NULL, 0, pstMail->Money, NULL, pstMail->IsGold);

	for (i=0; i<pstClan->stClan.ClanBrief.AssistNum; i++)
	{
		player_sysmail_fast_send(pstEnv, pstClan->stClan.ClanBrief.Assists[i].RoleName, 
				pstMail->Title, pstMail->Text, NULL, 0, pstMail->Money, NULL, pstMail->IsGold);	
	}
	return 0;
}

// zone处理battle的回复消息
int span_strong_ss_res(ZONESVRENV * pstEnv,SSPKG *pstPkg)
{
	SSSPANSTRONGRES *pstRes = &pstPkg->Body.SpanStrongRes;
	RSTRONGONE *pstRStrongOne;
	int i,j;

	// 收到这个消息的必定是zone
	if( INST_MODE_ZONE_SVR != pstEnv->iInstMode)
	{
		return 0;
	}

	if (pstRes->OpType == SS_SPAN_STRONG_RES_ALLINFO)
	{
		// 强制复制battle发过来的信息
		pstEnv->pstRegionGlobalInfo->stGlobal.Misc.RStrongInfo = pstRes->Data.All.AllInfo;
	}
	else if(pstRes->OpType == SS_SPAN_STRONG_RES_ONE)
	{
		// 强制复制battle发过来的信息
		pstRStrongOne = span_strong_one_find(pstEnv, pstRes->Data.One.OneInfo.StrongID);
		if (pstRStrongOne)
		{
			*pstRStrongOne = pstRes->Data.One.OneInfo;
		}
	}
	else if (pstRes->OpType == SS_SPAN_STRONG_RES_MAIL)
	{
		return span_strong_mail_in(pstEnv, &pstRes->Data.Mail);
	}

	if (	pstRes->OpType == SS_SPAN_STRONG_RES_ALLINFO || 
		pstRes->OpType == SS_SPAN_STRONG_RES_ONE)
	{
		RSTRONGINFO *pstInfo = &pstEnv->pstRegionGlobalInfo->stGlobal.Misc.RStrongInfo;
		SPANSTRONGDEF * pstDef;
		int iSvrID;
		pstEnv->iSpanMaxBull  = pstEnv->iSpanMaxBindGold = pstEnv->iSpanStrongExpMul =  pstEnv->iSpanStrongMaxLvl = 0;

		GET_WORLD_ID(iSvrID, pstEnv->pstAppCtx->iId);
		for (i=0; i<pstInfo->Num; i++)
		{
			pstDef = span_strong_def_find( pstEnv, pstInfo->Strong[i].StrongID);
			if (!pstDef)
				continue;

			// 本服占领
			if (pstInfo->Strong[i].Owner.SvrID == iSvrID)
			{
				if (pstDef->Lvl > pstEnv->iSpanStrongMaxLvl)
				{
					pstEnv->iSpanStrongMaxLvl = pstDef->Lvl;
				}

				for (j=0; j<MAX_WORLD_AWARD_TYPE; j++)
				{
					if (	pstDef->WorldAward[j].Type == 0 ||
						pstDef->WorldAward[j].Num == 0)
					{
						break;
					}

					if (pstDef->WorldAward[j].Type == 1)
					{
						if ( pstEnv->iSpanStrongExpMul <pstDef->WorldAward[j].Num)
						{
							pstEnv->iSpanStrongExpMul = pstDef->WorldAward[j].Num;
						}
					}
					else if (pstDef->WorldAward[j].Type == 2)
					{
						if ( pstEnv->iSpanMaxBull  <pstDef->WorldAward[j].Num)
						{
							pstEnv->iSpanMaxBull  = pstDef->WorldAward[j].Num;
						}
					}
					else if (pstDef->WorldAward[j].Type == 3)
					{
						if ( pstEnv->iSpanMaxBindGold  <pstDef->WorldAward[j].Num)
						{
							pstEnv->iSpanMaxBindGold  = pstDef->WorldAward[j].Num;
						}
					}
				}
			}
		}
		span_strong_occ_info_notify( pstEnv, NULL);
	}
	return 0;
}


RSTRONGONE *span_strong_one_find(ZONESVRENV* pstEnv, int iStrongID)
{
	int i;
	RSTRONGINFO *pstInfo = &pstEnv->pstRegionGlobalInfo->stGlobal.Misc.RStrongInfo;

	for (i=0; i<pstInfo->Num; i++)
	{
		if (pstInfo->Strong[i].StrongID == iStrongID)
		{
			return &pstInfo->Strong[i];
		}
	}
	return NULL;
}


static int issameday(time_t t1, time_t t2)
{
	struct tm curr1;
	struct tm curr2;
	
	localtime_r(&t1, &curr1);
	localtime_r(&t2, &curr2);

	if( curr1.tm_year==curr2.tm_year && curr1.tm_yday==curr2.tm_yday )
		return 1;

	return 0;
}

int span_strong_clt_sale(ZONESVRENV* pstEnv, Player *pstPlayer, SPANSTRONGSALE *pstSale)
{
	ZoneClan *pstClan;
	RSTRONGONE *pstOne;
	int iClanPosition;
//	RSTRONGINFO *pstInfo = &pstEnv->pstRegionGlobalInfo->stGlobal.Misc.RStrongInfo;
	int i;
	STRONGINFO *pstStrongInfo;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	SPANSTRONGSALEINFO *pstSaleInfo;
	SPANSTRONGDEF *pstDef;

	pstClan = player_get_clan( pstEnv,  pstPlayer);
	if (!pstClan)
	{
		return 0;
	}

	pstOne =  span_strong_one_find(pstEnv, pstSale->StrongID);
	if (!pstOne || pstOne->IsOpen == STRONG_CLOSE)
		return 0;

	iClanPosition = player_clan_position(pstEnv, pstPlayer);
	if (	iClanPosition != CLAN_POS_PRESIDENT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, 
							ZONEERR_SPAN_STRONG1);
		return 0;
	}

	if (span_strong_clan_matching(pstClan->stClan.ClanProf.Id, pstPlayer->stRoleData.WorldID,
									pstOne->Owner.ClanID, pstOne->Owner.SvrID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, 
							ZONEERR_STRONG214);
		return 0;
	}

	if (pstOne->Sale.iPlayer >= MAX_RSALE_NUM)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, 
							ZONEERR_SPAN_STRONG2);
		return 0;
	}
	
	if (pstOne->Stat != STRONG_STAT_SALE)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, 
							ZONEERR_STRONG215);
		return 0;
	}

	pstStrongInfo = &pstClan->stClan.ClanInner.StrongInfo;
	for (i=pstStrongInfo->SpanStrongNum -1; i>=0; i--)
	{
		if (!(issameday(iCurr, pstStrongInfo->SpanSaleInfo[i].Time)))
		{
			pstStrongInfo->SpanStrongNum --;
			if (i != pstStrongInfo->SpanStrongNum)
			{
				pstStrongInfo->SpanSaleInfo[i] = pstStrongInfo->SpanSaleInfo[pstStrongInfo->SpanStrongNum];
			}
			continue;
		}	
	}

	// 重复竞拍
	for (i=0; i<pstStrongInfo->SpanStrongNum; i++)
	{
		if (pstStrongInfo->SpanSaleInfo[i].StrongID == pstSale->StrongID)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, 
							ZONEERR_SPAN_STRONG3);
			return 0;
		}
	}

	// 最多拍3个据点
	if (pstStrongInfo->SpanStrongNum >= MAX_SALE_SPAN_STRONG)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, 
							ZONEERR_SPAN_STRONG4, MAX_SALE_SPAN_STRONG);
		return 0;
	}

	pstDef = span_strong_def_find( pstEnv, pstSale->StrongID);
	if (!pstDef)
		return 0;

	if (pstSale->Money < pstDef->BaseUse)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, 
							ZONEERR_SPAN_STRONG5);
		return 0;
	}

	if (package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, pstSale->Money) < 0)
	{
		return 0;
	}

	// 消息发给battle
	if (span_strong_sale_ss_req(pstEnv, pstClan, pstPlayer, pstSale) <0)
	{
		return 0;
	}

	// 假如battle处理出错,zone这边扣钱了咋办?
	package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, pstSale->Money);
	pstSaleInfo = &pstStrongInfo->SpanSaleInfo[pstStrongInfo->SpanStrongNum];
	pstStrongInfo->SpanStrongNum ++;
	
	pstSaleInfo->Money = pstSale->Money;
	pstSaleInfo->StrongID = pstSale->StrongID;
	pstSaleInfo->Time = iCurr;
	
	z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv,  pstPlayer, SYS_MIDDLE, 
					ZONEERR_STRONG217);
	return 0;
}

int span_strong_clt_enter(ZONESVRENV* pstEnv, Player *pstPlayer, SPANSTRONGENTER*pstEnter)
{
	ZoneClan *pstClan;
	RSTRONGONE *pstOne;
	int iClanID;
	int iSvrID;
	SPANSTRONGDEF *pstDef;


	pstClan = player_get_clan( pstEnv,  pstPlayer);
	if (!pstClan)
	{
		return 0;
	}

	iClanID = pstClan->stClan.ClanProf.Id;
	iSvrID = pstPlayer->stRoleData.WorldID;

	pstOne =  span_strong_one_find(pstEnv, pstEnter->StrongID);
	if (!pstOne)
		return 0;

	// 副本没开
	if (	pstOne->Pworld.InstID <= 0 ||
		pstOne->Pworld.PworldWid <= 0)
	{
		return 0;
	}

	if (pstOne->Stat != STRONG_STAT_FIGHT)
	{
		return 0;
	}

	if (	(pstOne->Owner.ClanID != iClanID ||pstOne->Owner.SvrID != iSvrID) &&
		(pstOne->Atk.ClanID != iClanID ||pstOne->Atk.SvrID != iSvrID))
	{
		return 0;
	}

	pstDef = span_strong_def_find( pstEnv, pstEnter->StrongID);
	if (!pstDef)
		return 0;

	CLANMEM *pstMem;
	pstMem = clan_id_mem(pstClan, pstPlayer->stRoleData.RoleID,&pstPlayer->stOnline.iClanMemIdx);
	if (!pstMem )
	{
		return -1;
	}

	if(pstMem->GroupID <= 0 || pstMem->GroupID > MAX_CLAN_GROUP_NUM)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv,  pstPlayer, SYS_MIDDLE, 
					ZONEERR_SPAN_STRONG10);
		return -1;
	}

	if(pstEnter->StrongID!= pstClan->stClan.ClanCore.ClanGroup.GroupInfo[pstMem->GroupID-1].SpanStrongID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv,  pstPlayer, SYS_MIDDLE, 
					ZONEERR_SPAN_STRONG10);
		return -1;
	}
	
	SSSPANDSTDATA stDstData;

	stDstData.DstType = SPAN_DST_TYPE_SPARTA;
	stDstData.Data.Sparta.PworldInstID = pstOne->Pworld.InstID;
	stDstData.Data.Sparta.PworldWID = pstOne->Pworld.PworldWid;
	stDstData.Data.Sparta.TeamID = 0;

	if(0 > player_span_state_check(pstEnv, pstPlayer))
	{
		return -1;
	}
	
	return player_span_hold_req(pstEnv, pstPlayer, &stDstData);
}


int span_strong_clt_all_info(ZONESVRENV* pstEnv, Player *pstPlayer, SPANSTRONGINFO*pstAllInfo)
{
	RSTRONGINFO *pstInfo = &pstEnv->pstRegionGlobalInfo->stGlobal.Misc.RStrongInfo;
	CSPKG stPkg;
	CSSPANSTRONGSVR *pstSvr = &stPkg.Body.SpanStrongSvr;
	
	memset(pstSvr, 0, sizeof(*pstSvr));
	pstSvr->Cmd = SPAN_STRONG_SVR_ALL_INFO;
	pstSvr->Data.AllInfo.All = *pstInfo;

	Z_CSHEAD_INIT(&stPkg.Head, SPAN_STRONG_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

// 判断自己公会跨服据点是否开启
int span_strong_pworld_is_open_by_clan(ZONESVRENV* pstEnv, int iClanID, int iSvrID)
{
	RSTRONGINFO *pstInfo = &pstEnv->pstRegionGlobalInfo->stGlobal.Misc.RStrongInfo;
	int i;

	for (i=0; i<pstInfo->Num; i++)
	{
		if (span_strong_clan_matching(iClanID, iSvrID, pstInfo->Strong[i].Owner.ClanID, pstInfo->Strong[i].Owner.SvrID) ||
			span_strong_clan_matching(iClanID, iSvrID, pstInfo->Strong[i].Atk.ClanID, pstInfo->Strong[i].Atk.SvrID))
		{
			if (pstInfo->Strong[i].Stat == STRONG_STAT_FIGHT)
			{
				return 1;
			}
		}
	}
	return 0;
}

int span_strong_group_req(ZONESVRENV* pstEnv, Player *pstPlayer, SPANSTRONGGROUP *pstGroup)
{
	CLANGROUPINFO *pstClanGroupInfo;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	int i;
	RSTRONGONE *pstOne;

	if (!pstClan)
		return -1;

	if(0 > clan_group_valid_check(pstClan, pstGroup->Group))
	{
		return -1;;
	}

	pstOne = span_strong_one_find(pstEnv, pstGroup->StrongID);
	if (!pstOne)
		return -1;

	/*if (!span_strong_clan_matching(pstClan->stClan.ClanProf.Id, pstPlayer->stRoleData.WorldID,
		pstOne->Owner.ClanID, pstOne->Owner.SvrID) &&
		!span_strong_clan_matching(pstClan->stClan.ClanProf.Id, pstPlayer->stRoleData.WorldID,
		pstOne->Atk.ClanID, pstOne->Atk.SvrID))
	{
		return -1;
	}*/	

	// pstGroup->StrongID 为0实际是取消战团
	if (pstGroup->StrongID < 0)
	{
		return -1;
	}

	pstClanGroupInfo = &pstClan->stClan.ClanCore.ClanGroup;
	if (pstClanGroupInfo->GroupInfo[pstGroup->Group-1].SpanStrongID == pstGroup->StrongID)
	{
		return -1;
	}
	
	if (span_strong_pworld_is_open_by_clan(pstEnv,pstClan->stClan.ClanProf.Id, pstPlayer->stRoleData.WorldID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_STRONG243);
		return -1;
	}

	// 取消之前的分配
	if (pstGroup->StrongID > 0)
	{
		for (i=0; i<pstClanGroupInfo->GroupNum; i++)
		{
			if (pstClanGroupInfo->GroupInfo[i].SpanStrongID == pstGroup->StrongID)
			{
				pstClanGroupInfo->GroupInfo[i].SpanStrongID = 0;
			}
		}	
	}
	
	pstClanGroupInfo->GroupInfo[pstGroup->Group-1].SpanStrongID = pstGroup->StrongID;
	player_clan_group_broadcast(pstEnv, pstClan);
	return 0;
}


int span_strong_clan_occ_num(ZONESVRENV* pstEnv, ZoneClan *pstClan, int *piMaxLvl)
{
	int iSvrID = 0;
	int iClanID = pstClan->stClan.ClanProf.Id;
	int i;
	RSTRONGINFO *pstInfo = &pstEnv->pstRegionGlobalInfo->stGlobal.Misc.RStrongInfo;
	int iNum = 0;
	SPANSTRONGDEF *pstDef;

	GET_WORLD_ID(iSvrID, pstEnv->pstAppCtx->iId);

	for (i=0; i<pstInfo->Num; i++)
	{
		if (span_strong_clan_matching(iClanID, iSvrID, pstInfo->Strong[i].Owner.ClanID, 
										pstInfo->Strong[i].Owner.SvrID))
		{
			iNum++;

			pstDef = span_strong_def_find( pstEnv, pstInfo->Strong[i].StrongID);
			if (!pstDef)
			{
				continue;
			}
			if (pstDef->Lvl > (*piMaxLvl))
			{
				(*piMaxLvl) = pstDef->Lvl;
			}
		}
	}
	return iNum;
}

void span_strong_occ_info_notify(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSSPANSTRONGSVR *pstSvr  = &stPkg.Body.SpanStrongSvr;
	int i;
	RSTRONGINFO *pstInfo = &pstEnv->pstRegionGlobalInfo->stGlobal.Misc.RStrongInfo;
	

	memset(pstSvr, 0, sizeof(*pstSvr));
	pstSvr->Cmd = SPAN_STRONG_SVR_OCC_INFO;
	pstSvr->Data.Occ.MaxLvl = pstEnv->iSpanStrongMaxLvl;
	pstSvr->Data.Occ.iSale = pstSvr->Data.Occ.Num = pstInfo->Num;
	pstSvr->Data.Occ.MaxExp = pstEnv->iSpanStrongExpMul;
	pstSvr->Data.Occ.MaxBull = pstEnv->iSpanMaxBull;
	pstSvr->Data.Occ.MaxBindGold = pstEnv->iSpanMaxBindGold;
		
	for (i=0; i<pstInfo->Num; i++)
	{
		pstSvr->Data.Occ.Occ[i].StrongID = pstInfo->Strong[i].StrongID;
		pstSvr->Data.Occ.Occ[i].Owner = pstInfo->Strong[i].Owner;
		pstSvr->Data.Occ.Sale[i].StrongID = pstInfo->Strong[i].StrongID;
		pstSvr->Data.Occ.Sale[i].Sale = pstInfo->Strong[i].Sale;
	}

	Z_CSHEAD_INIT(&stPkg.Head, SPAN_STRONG_SVR);
	if (pstPlayer)
	{
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	else
	{
		z_cltmsg_broadcast(pstEnv->pstAppCtx,pstEnv, &stPkg);
	}		 
}

int span_strong_clan_award(ZONESVRENV* pstEnv, Player *pstPlayer, SPANSTRONGCLANAWARD *pstAward)
{
	ZoneClan *pstClan;
	int iPos;
	int iOcc;
	int iMaxLvl=1;
		
	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_SPAN_STRONG7);
		return 0;
	}

	iPos = player_clan_position(pstEnv, pstPlayer);
	if (	iPos != CLAN_POS_PRESIDENT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_SPAN_STRONG7);
		return 0;
	}

	
	iOcc =  span_strong_clan_occ_num(pstEnv, pstClan, &iMaxLvl);
	if (iOcc <= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_SPAN_STRONG6);
		return 0;
	}

	if (IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstClan->stClan.ClanInner.StrongInfo.SpanClanMoneyGetTime))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPAN_STRONG8);
		return 0;
	}

	clan_resource_inc(pstEnv, pstClan, pstPlayer, CLAN_MONEY,iMaxLvl*5000+((iOcc-1)*2000));
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPAN_STRONG9);

	pstClan->stClan.ClanInner.StrongInfo.SpanClanMoneyGetTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	return 0;
}

int span_strong_clt_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSSPANSTRONGCLT *pstSpanStrongClt = &pstCsPkg->Body.SpanStrongClt;

	if(INST_MODE_ZONE_SVR != pstEnv->iInstMode )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
		return -1;
	}
	
	 switch(pstSpanStrongClt->Cmd)
	 {
	case SPAN_STRONG_CLT_OP_SALE:
		return span_strong_clt_sale( pstEnv, pstPlayer, &pstSpanStrongClt->Data.Sale);
		break;
	case SPAN_STRONG_CLT_OP_ENTER:
		return span_strong_clt_enter( pstEnv,pstPlayer,  &pstSpanStrongClt->Data.Enter);
		break;
	case SPAN_STRONG_CLT_OP_INFO_REQ:
		return span_strong_clt_all_info( pstEnv, pstPlayer, NULL);
		break;
	case SPAN_STRONG_CLT_OP_GROUP:
		return span_strong_group_req( pstEnv, pstPlayer, &pstSpanStrongClt->Data.Group);
		break;
	case SPAN_STRONG_CLT_OP_AWARD:
		return span_strong_clan_award(pstEnv,pstPlayer, &pstSpanStrongClt->Data.Award);
		break;
	 }
	
	return 0;
}


int span_strong_pworld_res_add(ZONESVRENV* pstEnv, SPANSTRONGDEF *pstDef,RSTRONGONE* pstOne)
{
	Monster *pstMon;
	int i;
	MONSTERDEF *pstMonDef;

	for (i=0; i<pstOne->Pworld.MonNum; i++)
	{
		if (pstOne->Pworld.Mon[i].Camp <= 0)
		{
			continue;
		}	

		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstOne->Pworld.Mon[i].MemID);
		if (!pstMon)
		{
			continue;
		}

		pstMonDef = z_get_mon_def(pstEnv, pstMon);
		if (!pstMonDef)
		{
			continue;
		}

		
		if (pstOne->Pworld.Mon[i].Camp == 1) // 防守方
		{
			pstOne->Pworld.DefRes += pstMonDef->GetRes;
			if (pstOne->Pworld.DefRes > pstDef->ResLimit)
				pstOne->Pworld.DefRes = pstDef->ResLimit;
		}
		else if (pstOne->Pworld.Mon[i].Camp == 2)  // 进攻方
		{
			pstOne->Pworld.AtkRes += pstMonDef->GetRes;
			if (pstOne->Pworld.AtkRes > pstDef->ResLimit)
				pstOne->Pworld.AtkRes = pstDef->ResLimit;
		}

	}
	return 0;
}


int span_strong_player_money_award(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iSec = pstPlayer->stRoleData.MiscInfo.ODTime.OndaySec;
	SPANSTRONGMISCINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.SpanStrongMiscInfo;
	
	
	if (pstEnv->iSpanMaxBindGold == 0 && pstEnv->iSpanMaxBull == 0)
	{
		return 0;
	}

	if (pstPlayer->stRoleData.Level < 60)
	{
		//return 0;
	}

	if (pstInfo->Award.BindGold < pstEnv->iSpanMaxBindGold && iSec - pstInfo->Award.PreBindGoldSec >= 60*60)
	{
		package_add_money( pstEnv, pstPlayer, ATTR_ID_BINDGODCOIN, HOUR_GET_BINDGOLD);
		pstInfo->Award.PreBindGoldSec = iSec;
		pstInfo->Award.BindGold += HOUR_GET_BINDGOLD;
	}

	if (pstInfo->Award.Bull < pstEnv->iSpanMaxBull && iSec -pstInfo->Award.PreBullSec >= 60)
	{
		package_add_money( pstEnv, pstPlayer, ATTR_ID_BULL, HOUR_GET_BULL);
		pstInfo->Award.PreBullSec = iSec;
		pstInfo->Award.Bull += HOUR_GET_BULL;
	}
	return 0;
}



