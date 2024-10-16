
#include "zone_boss.h"
#include "zone_npc.h"
#include "zone_map.h"
#include "zone_clt.h"
#include "zone_err.h"
#include "zone_span_strong.h"


WORLDBOSSONE *boss_one_find(WORLDBOSS *pstWorldBoss, int iMonID)
{
	int i;
	for (i=0; i<pstWorldBoss->iBoss; i++)
	{
		if (pstWorldBoss->stOne[i].iID == iMonID)
		{
			return &pstWorldBoss->stOne[i];
		}
	}
	return NULL;
}

WORLDBOSSDEF *boss_def_find(ZONESVRENV* pstEnv,int iMonID)
{
	WORLDBOSSDEF stDef;
	int iEqu, iIdx;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.MonID = iMonID;

	iIdx = bsearch_int(&stDef, pstObj->sWorldBossDef, pstObj->iWorldBossDef, WORLD_BOSS_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (WORLDBOSSDEF *)&pstObj->sWorldBossDef[iIdx];
	}
	return NULL;
}

int boss_fresh_time_cacl(WORLDBOSSDEF *pstDef, time_t tCurr)
{
	struct tm stNow;
	int iWeek;
	int iDefWeek = pstDef->Week;
	int i;
	int iDefHour = 0;
	char *pstMin = NULL;
	int iDefMin = 0;
	int iDay = 0;
	int iHour = 0;
	int iMin = 0;
	int iTmp;
	int iSec=0;

	if (pstDef->Week <= 0)
	{
		return 0;
	}
	
	localtime_r(&tCurr, &stNow);
	iWeek = stNow.tm_wday;
	if (iWeek == 0)
		iWeek = 7;

	if (	pstDef->FreshTime[0] == '\0')
	{
		iDefHour = 0;
		iDefMin = 0;
	}
	else
	{
		iDefHour = atoi(pstDef->FreshTime);	
		pstMin = strstr(pstDef->FreshTime, ":");
		if (pstMin)
		{
			iDefMin = atoi(++pstMin);
		}
	}

	// �������ʱ��ȶ���ˢ��ʱ�������Ųһ��
	if (	iDefHour <stNow.tm_hour ||
		(iDefHour == stNow.tm_hour && iDefMin <= stNow.tm_min ))
	{
		iWeek = (iWeek +1)%7;
		iDay ++;

		if (iWeek == 0)
			iWeek = 7;
	}

	iHour 	= iDefHour - stNow.tm_hour;
	iMin 		= iDefMin - stNow.tm_min;
	iSec   	= 0 - stNow.tm_sec;

	// 1100001��ʾ��1,2,��ˢ��
	// 1��ͬ��0000001����λ���㲹0,��ʾ����ˢ��
	for (i=1; i<=7; i++)
	{
		iTmp = iDefWeek/pow(10.0,7-iWeek);
		if (iTmp & 0x1)
		{
			break;
		}

		iWeek ++;
		if (iWeek >7)
			iWeek = 1;
		iDay ++;
	}

	// �����һ��ˢ�µ�������Сʱ����������
	return tCurr + iDay*3600*24 + iHour*3600 + iMin*60 + iSec;
}


GLOBALBOSSINFO* boss_global_find(GLOBALBOSS *pstGlobalBoss, int iID)
{
	int i;

	for (i=0; i<pstGlobalBoss->Num; i++)
	{
		if (pstGlobalBoss->BossInfos[i].ID == iID)
		{
			return &pstGlobalBoss->BossInfos[i];
		}
	}
	return NULL;
}

int boss_data_init(ZONESVRENV* pstEnv)
{
	WORLDBOSSDEF *pstDef;
	int i, j;
	WORLDBOSS *pstWorldBoss = &pstEnv->pstZoneObj->stBoss;
	WORLDBOSSONE *pstOne;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	GLOBALBOSS *pstGlobalBoss = &pstEnv->pstGlobalObj->stGlobal.Misc.GlobalBoss;
	GLOBALBOSSINFO *pstGlobleInfo;
	
	// ɾ�����������û��
	for (i=pstWorldBoss->iBoss-1; i>=0; i--)
	{
		for (j=0; j<pstEnv->pstZoneObj->iWorldBossDef; j++)
		{
			pstDef = (WORLDBOSSDEF *)&pstEnv->pstZoneObj->sWorldBossDef[j];
			
			if (pstWorldBoss->stOne[i].iID == pstDef->MonID)
			{
				break;
			}
		}			

		if (j>=pstEnv->pstZoneObj->iWorldBossDef)
		{
			pstWorldBoss->iBoss --;
			if (i != pstWorldBoss->iBoss )
			{
				pstWorldBoss->stOne[i] = pstWorldBoss->stOne[pstWorldBoss->iBoss];
			}
		}
	}	

	// ���
	for (i=0; i<pstEnv->pstZoneObj->iWorldBossDef; i++)
	{
		pstDef = (WORLDBOSSDEF *)&pstEnv->pstZoneObj->sWorldBossDef[i];
		pstOne = boss_one_find(pstWorldBoss, pstDef->MonID);
		if (!pstOne && pstWorldBoss->iBoss < MAX_WORLD_BOSS_DEF)
		{
			memset(&pstWorldBoss->stOne[pstWorldBoss->iBoss], 0, 
					sizeof(pstWorldBoss->stOne[pstWorldBoss->iBoss]));
			
			pstWorldBoss->stOne[pstWorldBoss->iBoss].iID = pstDef->MonID;
			pstWorldBoss->iBoss ++;
		}
	}

	for (i=pstGlobalBoss->Num -1; i>=0; i--)
	{
		for (j=0; j<pstEnv->pstZoneObj->iWorldBossDef; j++)
		{
			pstDef = (WORLDBOSSDEF *)&pstEnv->pstZoneObj->sWorldBossDef[j];
			
			if (pstGlobalBoss->BossInfos[i].ID == pstDef->MonID)
			{
				break;
			}
		}

		if (j>=pstEnv->pstZoneObj->iWorldBossDef)
		{
			pstGlobalBoss->Num --;
			if (i != pstGlobalBoss->Num)
			{
				pstGlobalBoss->BossInfos[i] = pstGlobalBoss->BossInfos[pstGlobalBoss->Num];
			}
		}
	}

	for (i=0; i<pstEnv->pstZoneObj->iWorldBossDef; i++)
	{
		pstDef = (WORLDBOSSDEF *)&pstEnv->pstZoneObj->sWorldBossDef[i];
		if (pstDef->FreshType != BOSS_FRESH_TIME)
		{
			continue;
		}
		
		pstGlobleInfo = boss_global_find(pstGlobalBoss, pstDef->MonID);
		if (!pstGlobleInfo)
		{
			pstGlobalBoss->BossInfos[pstGlobalBoss->Num].ID = pstDef->MonID;
			pstGlobalBoss->BossInfos[pstGlobalBoss->Num].DayFreshNum = 0;
			pstGlobalBoss->BossInfos[pstGlobalBoss->Num].NextFreshTine = 0;
			pstGlobalBoss->Num  ++;
		}
	}


	for (i=0; i<pstWorldBoss->iBoss; i++)
	{
		pstOne = &pstWorldBoss->stOne[i];
		pstDef = boss_def_find(pstEnv,pstOne->iID);
		if (!pstDef)
		{
			return -1;
		}

		pstOne->iTip = 0;
		
		if (pstDef->FreshType == BOSS_FRESH_OPEN_SVR)
		{
			if (pstOne->iNextTime < iCurr)
			{
				pstOne->iNextTime = iCurr;  // ����ˢ
			}	
		}
		else if(pstDef->FreshType == BOSS_FRESH_MON_DIE)
		{
			pstOne->iNextTime = 0;   		// ֻ�е�ĳ���������˲ſ��ܻ�ˢ
		}
		else if (pstDef->FreshType == BOSS_FRESH_TIME)
		{
			// ��ʱˢ����globle�ֶ�Ϊ׼
			pstGlobleInfo = boss_global_find(pstGlobalBoss, pstOne->iID);
			if (!pstGlobleInfo)
				return -1;

			pstGlobleInfo->NextFreshTine = pstOne->iNextTime = boss_fresh_time_cacl(pstDef, iCurr);
			
			/*if (pstGlobleInfo->NextFreshTine < iCurr)
			{
				// ���¼�����ˢ��ʱ��
				pstGlobleInfo->NextFreshTine = pstOne->iNextTime = boss_fresh_time_cacl(pstDef, iCurr);
			}
			else
			{
				pstOne->iNextTime = pstGlobleInfo->NextFreshTine;
			}*/
		}

		span_map_boss_data_send(pstEnv,pstDef->MapID,pstOne);
	}

	return 0;
}


Monster* boss_new(ZONESVRENV* pstEnv, int iMapID, int iX, int iY, int iMonID, int iLife, int iPatrolID)
{
	Monster stMonster;
	MONSTERDEF *pstMonDef;
	Monster *pstFind;

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = iMapID;
	stMonster.stMap.iPos =	-1;
	stMonster.iInitMapPos=	-1;	
	stMonster.tRefresh	=	pstEnv->pstAppCtx->stCurr.tv_sec;

	pstMonDef = z_get_mon_def(pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		return NULL;
	}
	
	stMonster.bDirection = RAND1(180);	
	stMonster.stInitPos.X  = iX;
	stMonster.stInitPos.Y = iY;

	pstFind =	z_new_monster(pstEnv->pstAppCtx, pstEnv, &stMonster, pstMonDef);

	if( !pstFind )
		return NULL;

	pstFind->iPatrolID = iPatrolID;

	pstFind->cFirstBorn = 1;
	pstFind->tLifeEnd = iLife > 0?pstEnv->pstAppCtx->stCurr.tv_sec+iLife:0;
	
	if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstFind,NULL))
	{
		tmempool_free(pstEnv->pstMonsterPool, pstFind->iMIndex);
		return NULL;
	}
	pstFind->cFirstBorn = 0;

	if (iPatrolID > 0)
	{
		pstFind->bMoveType = MOVE_TYPE_PATROL;
	}
	
	return pstFind;
}


int boss_fresh_tick(ZONESVRENV* pstEnv)
{
	WORLDBOSSDEF *pstDef;
	int i;
	WORLDBOSS *pstWorldBoss = &pstEnv->pstZoneObj->stBoss;
	WORLDBOSSONE *pstOne;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
//	GLOBALBOSS *pstGlobalBoss = &pstEnv->pstGlobalObj->stGlobal.Misc.GlobalBoss;
//	GLOBALBOSSINFO *pstGlobleInfo;

	for (i=0; i<pstWorldBoss->iBoss; i++)
	{
		pstOne = &pstWorldBoss->stOne[i];

		if (	pstOne->iFreshTime > 0 &&
			!IsSameDay(pstOne->iFreshTime , iCurr) )
		{
			pstOne->iDayNum = 0;
		}

		if (pstOne->iNextTime <= 0 )
		{
			continue;
		}

		pstDef = boss_def_find(pstEnv, pstOne->iID);
		if (!pstDef)
		{
			continue;
		}

		if ( iCurr >= pstOne->iNextTime)
		{
			if (boss_new(pstEnv, pstDef->MapID, pstDef->X, pstDef->Y, 
						pstDef->MonID, pstDef->LifeTime, pstDef->PatrolID) == NULL)
			{
				continue;
			}

			pstOne->iFreshTime = iCurr;
			pstOne->iNextTime = 0;
			pstOne->iStat = 1;
			pstOne->iDayNum ++;

			span_map_boss_data_send(pstEnv,pstDef->MapID,pstOne);

			if (pstDef->FreshMsg[0])
				z_sys_strf_broadcast(pstEnv,pstDef->TipChannel, ZONEERR_BROAD302, pstDef->FreshMsg);
			// ��װ���ɼ�ˢ�´���û��Ҫ����global��,��ˢһ��ҲӰ�첻��
		}
		else if (!pstOne->iTip && iCurr+pstDef->TipTime >= pstOne->iNextTime)
		{
			pstOne->iTip = 1;

			span_map_boss_data_send(pstEnv,pstDef->MapID,pstOne);
			
			if (pstDef->TipMsg[0])
				z_sys_strf_broadcast(pstEnv,pstDef->TipChannel, ZONEERR_BROAD302, pstDef->TipMsg);
		}
	}
	return 0;
}

int boss_die_fresh(ZONESVRENV* pstEnv, WORLDBOSSDEF *pstDef, WORLDBOSSONE *pstElse)
{
//	WORLDBOSS *pstWorldBoss = &pstEnv->pstZoneObj->stBoss;
	WORLDBOSSDEF *pstDefLast;
 	struct tm stNow;
	time_t tCurr = pstEnv->pstAppCtx->stCurr.tv_sec;

	localtime_r(&tCurr, &stNow);


	pstDefLast = boss_def_find(pstEnv, pstElse->iID);
	if (!pstDefLast)
	{
		return 0;;
	}

	if (pstElse->iDayNum >= pstDef->DieFreshNum || pstElse->iStat)
	{
		return 0;
	}

	if (pstElse->iNextTime >= tCurr)
	{
		return pstElse->iNextTime;
	}

	// 8��֮�����һ��ûˢ�Ļ����ض�ˢһ��
	if (stNow.tm_hour > 20 && pstElse->iDayNum <= 0)
	{
		return tCurr+pstDef->DieFreshMonTime;
	}

	if (RAND1(10000) < pstDef->DieFreshMonRate)
	{
		return tCurr+pstDef->DieFreshMonTime;	
	}
	
	return 0;	
}

int boss_die(ZONESVRENV* pstEnv,Monster* pstMon, MONSTERDEF *pstMonDef)
{
	WORLDBOSSDEF *pstDef;
	WORLDBOSSONE *pstOne;
	WORLDBOSS *pstWorldBoss = &pstEnv->pstZoneObj->stBoss;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	WORLDBOSSONE *pstElse;
	GLOBALBOSSINFO *pstGlobalInfo;
	GLOBALBOSS *pstGlobalBoss = &pstEnv->pstGlobalObj->stGlobal.Misc.GlobalBoss;
	
	pstDef = boss_def_find(pstEnv, pstMonDef->MonsterID);
	if (!pstDef)
		return 0;

	pstOne = boss_one_find(pstWorldBoss,pstMonDef->MonsterID);
	if (!pstOne)
	{
		return 0;
	}

	if (pstDef->FreshType == BOSS_FRESH_OPEN_SVR)
	{
		if (pstDef->DieFreshTime > 0)
		{
			pstOne->iNextTime = iCurr + pstDef->DieFreshTime;
		}
		else
		{
			struct tm stNext;
			int iHour = 0;
			int iMin = 0;
			time_t t = pstEnv->pstAppCtx->stCurr.tv_sec+pstDef->DieFreshDay*24*60*60;
			char *pstMin = NULL;
			
			localtime_r(&t, &stNext);

			if (	pstDef->DieFreshPoint[0] == '\0')
			{
				iHour = 0;
				iMin = 0;
			}
			else
			{
				iHour = atoi(pstDef->DieFreshPoint);	
				pstMin = strstr(pstDef->DieFreshPoint, ":");
				if (pstMin)
				{
					iMin = atoi(++pstMin);
				}
			}

			stNext.tm_hour = iHour;
			stNext.tm_min = iMin;
			
			pstOne->iNextTime = mktime(&stNext);
		}
		

		// ȷ����װ���ɼ�ˢ��ʱ��
		if (pstDef->DieFreshMonID > 0)
		{
			pstElse = boss_one_find(pstWorldBoss,pstDef->DieFreshMonID);
			if (pstElse)
			{
				pstElse->iNextTime =  boss_die_fresh(pstEnv, pstDef, pstElse);
			}
		}
	}
	else if(pstDef->FreshType == BOSS_FRESH_MON_DIE)
	{
		pstOne->iNextTime = 0;
	}
	else if (pstDef->FreshType == BOSS_FRESH_TIME)
	{
		// ������˹���ˢ��ʱ��
		pstOne->iNextTime = boss_fresh_time_cacl(pstDef, iCurr);
		pstGlobalInfo = boss_global_find(pstGlobalBoss, pstOne->iID);
		if (pstGlobalInfo)
		{
			pstGlobalInfo->NextFreshTine = pstOne->iNextTime;
		}
	}
	pstOne->iStat = 0;    
	pstOne->iTip = 0;

	if (pstDef->DieMsg[0])
		z_sys_strf_broadcast(pstEnv,pstDef->TipChannel, ZONEERR_BROAD302, pstDef->DieMsg);

	span_map_boss_data_send(pstEnv,pstDef->MapID,pstOne);
	return 0;
}


int boss_info_req(ZONESVRENV* pstEnv, Player *pstPlayer, CSWORLDBOSSINFOREQ *pstReq)
{
	GlobalObjInfo *pstGlobalObjInfo = &pstEnv->pstGlobalObj->stGlobalObjInfo;
	WORLDBOSS *pstWorldBoss = &pstEnv->pstZoneObj->stBoss;
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstRes = &stPkg.Body.PlayerMiscSvr;
	CSWORLDBOSSINFORES *pstBossInfoRes = &pstRes->PlayerMiscSvrData.BossInfoRes;
	int i;

	if (GLOBAL_STAT_FINI != pstGlobalObjInfo->iGlobalStat)
		return 0;
		
	memset(pstRes, 0, sizeof(*pstRes));
	pstRes->PlayerMiscType = PLAYER_BOSS_INFO_RES;

	for (i=0; i<pstWorldBoss->iBoss; i++)
	{
		pstBossInfoRes->One[pstBossInfoRes->Num].MonID = pstWorldBoss->stOne[i].iID;
		pstBossInfoRes->One[pstBossInfoRes->Num].Stat = pstWorldBoss->stOne[i].iStat;
		pstBossInfoRes->One[pstBossInfoRes->Num].FreshTime = pstWorldBoss->stOne[i].iNextTime;
		pstBossInfoRes->Num ++;

		if (pstBossInfoRes->Num >= MAX_WORLD_BOSS_NUM)
			break;
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

//�Ƿ�Ϊ���Ұ���ͼ
int if_span_pvp_map(ZONESVRENV *pstEnv,int MapID)
{
	MapIndex* pstMapIndex = NULL;
	ZONEMAPINST* pstMapInst = NULL;
	ZONEIDX stIdx;
	
	stIdx.iID = MapID;
	stIdx.iPos = -1;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if(!pstMapInst)
	{
		return -1;
	}	

	pstMapIndex	= z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if( !pstMapIndex)
	{
		return -1;
	}
	
	if(!(pstMapIndex->stMapDef.CtrlFlag & MAP_CTRL_PVP_LOAD_MAP))
	{
		return -1;
	}

	return 0;
}

//���Ұ������bossͬ��������w
int span_map_world_boss_data_sync(ZONESVRENV *pstEnv,SSWORLDBOSSDATASYNC *pstWorldBossData)
{
	SSPKG stPkg;
	SSWORLDBOSSDATASYNC *pstRes = &stPkg.Body.WorldBossDataSync;

	memcpy(pstRes,pstWorldBossData,sizeof(*pstRes));
	
	Z_SSHEAD_INIT(&stPkg.Head, SS_WORLD_BOSS_DATA_SYNC);
	span_msg_world_notify(pstEnv, &stPkg, 0);
	return 0;
}

int span_map_boss_data_send(ZONESVRENV *pstEnv,int MapID,WORLDBOSSONE *pstOne)
{
	if(0 > if_span_pvp_map(pstEnv,MapID))
	{
		return 0;
	}

	if(!pstOne)
	{
		return 0;
	}

	if(!is_battle_pvp_svr(pstEnv))
	{
		return 0;
	}

	SSWORLDBOSSDATASYNC stWorldBossData;
	
	memset(&stWorldBossData,0,sizeof(stWorldBossData));

	stWorldBossData.iDayNum = pstOne->iDayNum;
	stWorldBossData.iFreshTime = pstOne->iFreshTime;
	stWorldBossData.iID = pstOne->iID;
	stWorldBossData.iNextTime =  pstOne->iNextTime;
	stWorldBossData.iStat =  pstOne->iStat;
	stWorldBossData.iTip =  pstOne->iTip;
		
	span_map_world_boss_data_sync(pstEnv,&stWorldBossData);
	return 0;
}


