


#include "zone_home.h"
#include "zone_db.h"
#include "zone_map.h"
#include "zone_shop.h"
#include "zone_db.h"
#include "zone_package.h"
#include "zone_mail.h"
#include "libdirty.h"
#include "zone_clt.h"
#include "zone_err.h"
#include "zone_npc.h"
#include "zone_arm.h"
#include "zone_zh_name.h"
#include "zone_err.h"
#include "zone_oplog.h"
#include "zone_map.h"

//#ifdef 0
#define HOME_STOP_PATCH 25

// 宠物操作消耗道具
#define HOME_PET_EAT_ITEM 5070000
#define HOME_PET_CLEAR_ITEM 5070001
#define HOME_PET_PLAY_ITEM 5070002
#define HOME_PET_DUANLIAN_ITEM 5070003

// 宠物操作数值
#define HOME_PET_EAT_ITEM_VAL 10
#define HOME_PET_CLEAR_ITEM_VAL 10
#define HOME_PET_PLAY_ITEM_VAL 10
#define HOME_PET_DUANLIAN_ITEM_VAL 10

// 宠物最大数值上线
#define HOME_PET_EAT_ITEM_MAX 100
#define HOME_PET_CLEAR_ITEM_MAX 100
#define HOME_PET_PLAY_ITEM_MAX 100
#define HOME_PET_DUANLIAN_ITEM_MAX 100

// 开垦土地消耗
#define HOME_FARM_KAIKEN_USE 10000

int home_update_req(ZONESVRENV* pstEnv, ZoneHome*pstZoneHome)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq =  &pstPkg->Body.UpdateReq;
	DBHomeInsertBack stBack;

	stBack.iID = pstZoneHome->stHome.HomeID;
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,0,&pstPkg->Head, 
								TORM_UPDATE_REQ,DB_HOME_UPDATE_RES, 
			               			"Home", 
			               			(char*)&stBack, 
			               			sizeof(stBack));
	
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;
	
	iDst = pstEnv->iMiscDBID;
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,	&pstZoneHome->stHome, 
			sizeof(pstZoneHome->stHome),&iDst,pstEnv->iRoleDBProtoVer);
}

int home_update_res(ZONESVRENV *pstEnv, TORMPKG *pstTormPkg, DBAsyn *pstDBAsyn)
{
	TORMUPDATERES *pstUpdateRes = &pstTormPkg->Body.UpdateRes;
	ZoneHome *pstHome = NULL;
	DBHomeInsertBack *pstBack;
	
	pstBack = (DBHomeInsertBack *)(pstDBAsyn->AsynCmd.szMaxBuf);

	if (TORM_ERR_IS_ERROR(pstUpdateRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "db update home failed." );
		return -1;
	}

	pstHome = zonehome_one_find_by_id( pstEnv,pstBack->iID);
	if (!pstHome)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "home_update_res:can not find home." );
		return -1;
	}

	pstHome->stZoneHomeInfo.iDirty = 0;
	return 0;
}

int z_db_home_finish(ZONESVRENV *pstEnv)
{
	HomeObj *pstObj = pstEnv->pstHomeObj;
	int i;
	ZoneHome *pstZoneHome;


	if (pstObj->stObjHomeInfo.iStat != HOME_STAT_FINI)
	{
		return 0;
	}

	for(i = 0; i < pstObj->iHomeNum; ++i)
	{
		pstZoneHome = &pstObj->astZoneHome[i];

		pstZoneHome->stZoneHomeInfo.iDirty = 2;
		home_update_req(pstEnv, pstZoneHome);
	}
	return 0;
}


int z_db_home_is_allclean(ZONESVRENV * pstEnv)
{
	HomeObj *pstObj = pstEnv->pstHomeObj;
	int i, iCount = 0;
	ZoneHome *pstZoneHome;


	if (pstObj->stObjHomeInfo.iStat != HOME_STAT_FINI)
	{
		return 1;
	}

	for(i = 0; i < pstObj->iHomeNum; ++i)
	{
		pstZoneHome = &pstObj->astZoneHome[i];
		if (pstZoneHome->stZoneHomeInfo.iDirty == 0)
		{
			++iCount;
		}
	}

	return (iCount == pstObj->iHomeNum );
}

HOMELVLDEF *home_lvl_def_find(ZONESVRENV* pstEnv, int iLv)
{
	HOMELVLDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Lvl = iLv;
	
	iIndex = bsearch_int(&stDef, pstObj->sHomeLvlDef, pstObj->iHomeLvlDef, HOME_LVL_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (HOMELVLDEF *)&pstObj->sHomeLvlDef[iIndex];
	}

	return NULL;
}

HOMEMONLVLDEF *home_mon_lvl_def_find(ZONESVRENV* pstEnv, int iLv)
{
	HOMEMONLVLDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Lvl = iLv;
	
	iIndex = bsearch_int(&stDef, pstObj->sHomeMonLvlDef, pstObj->iHomeMonLvlDef, HOME_MON_LVL_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (HOMEMONLVLDEF *)&pstObj->sHomeMonLvlDef[iIndex];
	}

	return NULL;
}

HOMEMONSEGDEF *home_mon_seg_def_find(ZONESVRENV* pstEnv, int iMonID)
{
	HOMEMONSEGDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.MonID = iMonID;
	
	iIndex = bsearch_int(&stDef, pstObj->sHomeMonSegDef, pstObj->iHomeMonSegDef, HOME_MON_SEG_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (HOMEMONSEGDEF *)&pstObj->sHomeMonSegDef[iIndex];
	}

	return NULL;
}

HOMEFARMDEF *home_farm_def_find(ZONESVRENV* pstEnv, int iLvl)
{
	HOMEFARMDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Lvl = iLvl;
	
	iIndex = bsearch_int(&stDef, pstObj->sHomeFarmDef, pstObj->iHomeFarmDef, HOME_FARM_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (HOMEFARMDEF *)&pstObj->sHomeFarmDef[iIndex];
	}

	return NULL;
}


int home_db_get(ZONESVRENV *pstEnv)
{
	HOME stHome;
	DBAsyn stDBAsyn;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	HomeObj *pstObj = pstEnv->pstHomeObj;

	stDBAsyn.iCmd = DB_HOME_SELECT;
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ,
	                 "Home", -1, &stDBAsyn);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OffSet = pstObj->iHomeNum;
	pstSelectReq->Limit = HOME_PAGE_MAX;
	strcpy(pstSelectReq->OperInfo.WhereDef, "where HomeID != 0");

	memset(&stHome, 0, sizeof(stHome));

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stHome;
	stMetaData.iBuff = sizeof(stHome);
	if (0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData,
	                           pstEnv->iMiscDBProtoVer))
	{
		goto error;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		goto error;
	}	
	return 0;
error:
	pstObj->stObjHomeInfo.iStat = HOME_STAT_FAIL;
	pstObj->iHomeNum = 0;
	return 0;
}

int home_init_fini(ZONESVRENV *pstEnv)
{
	int i;
	HomeObj *pstObj = pstEnv->pstHomeObj;
	ZoneHome*pstHome;

	for(i = 0; i < pstObj->iHomeNum; ++i)
	{
		pstHome = &pstObj->astZoneHome[i];

		// todo:创建家园副本,家园初始化
		home_build(pstEnv, &pstHome->stHome);
	}

	pstObj->stObjHomeInfo.iStat = HOME_STAT_FINI;
	return 0;
}

int home_init_all_pworld(ZONESVRENV *pstEnv)
{
	int i;
	HomeObj *pstObj = pstEnv->pstHomeObj;
	ZoneHome*pstHome;

	if (pstObj->stObjHomeInfo.iStat != HOME_STAT_FINI)
	{
		return 0;
	}

	for(i = 0; i < pstObj->iHomeNum; ++i)
	{
		pstHome = &pstObj->astZoneHome[i];
		home_build(pstEnv, &pstHome->stHome);
	}
	return 0;
}

int home_db_count_req(ZONESVRENV *pstEnv)
{
	DBAsyn stDBAsyn;
	HOME stHome;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;

	stDBAsyn.iCmd = DB_HOME_COUNT;
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head,
	                 TORM_SELECT_COUNT_REQ, "Home", -1, &stDBAsyn);
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);

	memset(&stHome, 0, sizeof(stHome));
	stMetaData.pszBuff = (char *)&stHome;
	stMetaData.iBuff = sizeof(stHome);
	if (0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData,
	                           pstEnv->iMiscDBProtoVer))
	{
		return -1;
	}

	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}
	
	return 0;
}

int home_db_count_res(ZONESVRENV *pstEnv, TORMSELECTCOUNTRES *pstSelectCountRes)
{
	HomeObj *pstObj = pstEnv->pstHomeObj;

	if (TORM_ERR_IS_ERROR(pstSelectCountRes->ErrNo))
	{
		goto error;
	}

	pstObj->stObjHomeInfo.iDBCount = pstSelectCountRes->RowsCount;

	if (0 == pstObj->stObjHomeInfo.iDBCount)
	{
		pstObj->stObjHomeInfo.iStat = HOME_STAT_FINI;
	}
	else
	{
		home_db_get(pstEnv);
	}
	return 0;

error:
	pstObj->stObjHomeInfo.iStat = HOME_STAT_FAIL;
	return 0;
}


int home_init(ZONESVRENV *pstEnv)
{
	HomeObj *pstObj = pstEnv->pstHomeObj;
	if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
	{
		if (pstObj->stObjHomeInfo.iStat == HOME_STAT_FINI ||pstObj->stObjHomeInfo.iStat == HOME_STAT_REQ)
		{
			return 0;
		}

		pstEnv->pstHomeObj->iHomeNum = 0;
		pstObj->stObjHomeInfo.iStat = HOME_STAT_REQ;
		if (home_db_count_req(pstEnv) < 0)
		{
			pstObj->stObjHomeInfo.iStat  = HOME_STAT_FAIL;
			return -1;
		}
	}
	return 0;
}

int home_select_res(ZONESVRENV *pstEnv, TORMSELECTRES *pstSelectRes)
{
	HomeObj *pstObj = pstEnv->pstHomeObj;
	int i;
	ZoneHome *pstZoneHome;
	
	if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			       "home_select_res. dberr = %d, msg = %s.", 
			       pstSelectRes->ErrNo, torm_error_string(pstSelectRes->ErrNo));
		goto error;
	}

	if (0 == pstSelectRes->SelectResult.ResultNum ||
			pstSelectRes->SelectResult.ResultNum !=
			(int)pstSelectRes->SelectResult.ResultTotal)
	{
		goto error;
	}

	for(i = 0; i < pstSelectRes->SelectResult.ResultNum; ++i)
	{
		if (pstObj->iHomeNum >= SVR_MAX_HOME)
		{
			break;
		}	

		pstZoneHome = &pstObj->astZoneHome[pstObj->iHomeNum];
		bzero( pstZoneHome, sizeof(*pstZoneHome));
		
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&pstZoneHome->stHome,
		                            sizeof(pstZoneHome->stHome)))
		{
			goto error;
		}

		pstObj->iHomeNum ++;
	}

	if (pstObj->iHomeNum <pstObj->stObjHomeInfo.iDBCount &&
		pstObj->iHomeNum < SVR_MAX_HOME)
	{
		home_db_get(pstEnv);
	}
	else
	{
		home_init_fini(pstEnv);	
	}
	return 0;

error:
	pstObj->stObjHomeInfo.iStat = HOME_STAT_FAIL;
	pstObj->iHomeNum = 0;
	return 0;
}


int home_update(ZONESVRENV *pstEnv, ZoneHome*pstZoneHome)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
		
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, 
	                 "Home", -1, NULL);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&pstZoneHome->stHome;
	stMetaData.iBuff = sizeof(pstZoneHome->stHome);
	if (0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
	                           pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}

	pstZoneHome->stZoneHomeInfo.iDirty = 0;
	return 0;
}

ZONEMAPINST *home_get_map_inst(ZONESVRENV* pstEnv, HOME *pstHome)
{
	ZONEPWORLDINST *pstPInst;
//	ZONEMAPINST *pstMInst;
	
	pstPInst	= z_find_pworld_inst(pstEnv->pstMapObj, pstHome->Info.PworldinstID);
	if(NULL == pstPInst)
		return NULL;

	return  z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPInst->astMapInstIdx[0]);
}

void home_pet_dec_val(int *piVal, int iDec, int iMin)
{
	*piVal -= iDec;
	if (*piVal < iMin) *piVal = iMin;
}

int home_pet_tick(ZONESVRENV* pstEnv, HOME *pstHome)
{
	HOMEMON *pstPet = &pstHome->Info.Pet;
	HOMEMONLVLDEF *pstDef;
	ZONEMAPINST *pstMInst;

	pstDef = home_mon_lvl_def_find(pstEnv, pstPet->Lvl);
	if (!pstDef)
		return 0;

	pstPet->Exp += 60;
	home_pet_dec_val(&pstPet->Baoshi, 1, 0);
	home_pet_dec_val(&pstPet->Happy, 1, 0);
	home_pet_dec_val(&pstPet->Jiankang, 1, 0);
	home_pet_dec_val(&pstPet->Qingjie, 1, 0);

	// 满级
	if (pstDef->Exp <= 0)
	{
		pstPet->Exp = 0;	
	}
	else
	{
		if (pstPet->Exp  >= pstDef->Exp)
		{
			pstPet->Lvl ++;
			pstPet->Exp -= pstDef->Exp;
		}
	}

	pstMInst = home_get_map_inst(pstEnv, pstHome);
	if (pstMInst)
	{
		home_pet_notify(pstEnv, NULL, pstHome, pstMInst);
	}
		
	return 0;	
}


int home_tick(ZONESVRENV *pstEnv, ZoneHome*pstZoneHome)
{
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	pstZoneHome->stZoneHomeInfo.uiTimeCount ++;

	if (	iCurr - pstZoneHome->stHome.Info.LastEnterTime >= 10*24*3600 && 
		pstZoneHome->stHome.Info.LastEnterTime > 0)
	{
		
		return home_db_delete_req( pstEnv, 
			z_name_player(pstEnv->pstAppCtx,pstEnv, pstZoneHome->stHome.RoleName),
			&pstZoneHome->stHome);
	}

	// 6分钟一次的宠物tick
	if (pstZoneHome->stZoneHomeInfo.uiTimeCount %120 == 0)
	{
		home_pet_tick( pstEnv, &pstZoneHome->stHome);
	}

	// 10秒一次农场tick
	if (pstZoneHome->stZoneHomeInfo.uiTimeCount %2 == 0)
	{
		home_farm_tick(pstEnv, &pstZoneHome->stHome);
	}
	
	
	switch(pstZoneHome->stZoneHomeInfo.iDirty)
	{
	case 0:// 干净的
	
		break;
	case 1:// 定时更新
		if (pstZoneHome->stZoneHomeInfo.uiTimeCount % 100 == 0)
		{
			home_update(pstEnv, pstZoneHome);
		}
		break;
	case 2:// 立即更新
		home_update(pstEnv, pstZoneHome);
		break;
	}
	return 0;
}

#define HOME_ACTIVE_TIMER 5000

// 五秒一次
int z_active_home(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, int iCount)
{
	static struct timeval stRoundTv;
	static struct timeval stRoundStartTv;
	static int iActivePos = 0, iRound = 0;
	int i;
	struct timeval stSub;
	long long  llMs=0;
	ZoneHome*pstZoneHome = NULL;
	HomeObj *pstHomeObj = pstEnv->pstHomeObj;

	if (INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
	{
		return 0;
	}

	if (pstHomeObj->stObjHomeInfo.iStat != HOME_STAT_FINI)
	{
		return 0;
	}

	
	if (0 == stRoundTv.tv_sec)
	{
		llMs = HOME_ACTIVE_TIMER;
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
		
	if (llMs < HOME_ACTIVE_TIMER)
	{
		return 0;
	}

	if (0 == iActivePos)
	{
		TV_CLONE(stRoundStartTv, pstAppCtx->stCurr);
	}

	for(i = 0; i < iCount; ++i) 
	{
		if (iActivePos >= pstHomeObj->iHomeNum)
		{
			break;
		}

		pstZoneHome= &pstHomeObj->astZoneHome[iActivePos];
		home_tick(pstEnv,  pstZoneHome);

		++iActivePos;
	}
	
	if (iActivePos < pstHomeObj->iHomeNum)
	{
		return 0;
	}

	iActivePos = 0;
	++iRound;
	TV_CLONE(stRoundTv, pstAppCtx->stCurr);
	TV_DIFF(stSub, pstAppCtx->stCurr, stRoundStartTv);
	TV_TO_MS(llMs, stSub);
	if (llMs > HOME_ACTIVE_TIMER)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "active store loop %lld ms", llMs);
	}

	return 0;
}


int home_new_mon_by_dir(ZONESVRENV *pstEnv, int iMonID, RESPOS *pstPos, int iDir, ZONEMAPINST *pstMapInst)
{
	
	Monster stMonster;
	Monster *pstFind;
	MONSTERDEF *pstMonDef;

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.stInitPos = *pstPos;
	stMonster.cRefresh	= 0;
	stMonster.bDirection = iDir;
	stMonster.stCurrPos = *pstPos;

	pstMonDef = z_get_mon_def(pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		return -1;
	}

	pstFind =	z_new_monster(pstEnv->pstAppCtx, pstEnv, &stMonster, pstMonDef);

	if( !pstFind )
	{
		return -1;
	}

	pstFind->iPatrolID = 0;
	pstFind->cFirstBorn = 1;
	if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstFind,NULL))
	{
		tmempool_free(pstEnv->pstMonsterPool, pstFind->iMIndex);
		return -1;
	}
	pstFind->cFirstBorn = 0;

	return pstFind->iID;
}


int home_init_build(ZONESVRENV *pstEnv, HOME *pstHome)
{
	HOMELVLDEF *pstDef;
	ZONEPWORLD *pstPworld;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	int i;
	int iMem;

	pstDef = home_lvl_def_find(pstEnv, pstHome->Lvl);
	if (!pstDef)
	{
		return -1;
	}

	stIdx.iID = pstDef->PworldID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj,&stIdx);
	if (!pstPworld)
	{
		return -1;
	}

	ZONEPWORLDINST*pstZonePworldInst;
	PWORLDSELECT stPworldSelect;

	bzero(&stPworldSelect, sizeof(stPworldSelect));

	pstZonePworldInst = z_in_new_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstPworld, &stPworldSelect,  0, 0);
	if (!pstZonePworldInst)
	{
		return -1;
	}

	// 记一下这个副本属于谁的家园
	pstZonePworldInst->iHomeID = pstHome->HomeID;

	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstZonePworldInst->astMapInstIdx[0]);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstHome->Info.PworldinstID = pstZonePworldInst->iPworldInstID;

	// 农场
	for (i=0; i<FARM_MAX_SOIL; i++)
	{
		iMem = 0;
		// 未开垦的
		if (pstDef->Soil[i].Stat == 0)
		{
			iMem = home_new_mon_by_dir(pstEnv, pstDef->SoilMonID, &pstDef->Soil[i].Pos, 0, pstMapInst);
			if (iMem < 0)
			{
				return -1;
			}	
		}
		
		pstHome->Info.Siol.One[pstHome->Info.Siol.Num].Stat = pstDef->Soil[i].Stat;
		pstHome->Info.Siol.One[pstHome->Info.Siol.Num].Idx = i;
		pstHome->Info.Siol.One[pstHome->Info.Siol.Num].MonMemID = iMem;
		pstHome->Info.Siol.Num ++;
	}

	// 住宅不用管, 新建家园没有家具

	// 宠物
	home_pet_init(pstHome,  pstDef->GardenDefaultMonID);
	
	return 0;
}

int home_build(ZONESVRENV *pstEnv, HOME *pstHome)
{
	HOMELVLDEF *pstDef;
	ZONEPWORLD *pstPworld;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	int i;
	MONSTERDEF *pstMonDef;
	Monster stMon;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	int iMemID;

	pstDef = home_lvl_def_find(pstEnv, pstHome->Lvl);
	if (!pstDef)
	{
		return -1;
	}

	stIdx.iID = pstDef->PworldID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj,&stIdx);
	if (!pstPworld)
	{
		return -1;
	}

	ZONEPWORLDINST*pstZonePworldInst;
	PWORLDSELECT stPworldSelect;

	bzero(&stPworldSelect, sizeof(stPworldSelect));

	pstZonePworldInst = z_in_new_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstPworld, &stPworldSelect,  0, 0);
	if (!pstZonePworldInst)
	{
		return -1;
	}

	// 记一下这个副本属于谁的家园
	pstZonePworldInst->iHomeID = pstHome->HomeID;

	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstZonePworldInst->astMapInstIdx[0]);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstHome->Info.PworldinstID = pstZonePworldInst->iPworldInstID;

	// 刷住宅摆设
	for (i=0; i<pstHome->Info.House.Num; i++)
	{
		home_new_mon_by_dir(pstEnv, pstHome->Info.House.One[i].MonID, &pstHome->Info.House.One[i].Pos, 
									pstHome->Info.House.One[i].Dir, pstMapInst);
	}

	// 菜地
	for (i=0; i<pstHome->Info.Siol.Num; i++)
	{
		// 为开垦的土地
		if (pstHome->Info.Siol.One[i].Stat == 0)
		{
			iMemID = home_new_mon_by_dir(pstEnv, pstDef->SoilMonID, &pstDef->Soil[i].Pos, 0, pstMapInst);
			if (iMemID <= 0)
			{
				continue;
			}

			pstHome->Info.Siol.One[i].MonMemID = iMemID;
		}
		else
		{
			if (pstHome->Info.Siol.One[i].MonID <= 0)
			{
				continue;
			}
			
			stMon.iDefIdx = pstHome->Info.Siol.One[i].MonID;
			pstMonDef = z_get_mon_def( pstEnv, &stMon);
			if (!pstMonDef)
			{
				return -1;
			}		

			// 成熟了
			if (	pstHome->Info.Siol.One[i].ChengshuTime >0 && 
				iCurr < pstHome->Info.Siol.One[i].ChengshuTime &&
				pstMonDef->ChengshuID > 0)
			{
				iMemID = home_new_mon_by_dir(pstEnv, pstMonDef->ChengshuID, &pstDef->Soil[i].Pos, 0, pstMapInst);
				if (iMemID <= 0)
				{
					continue;
				}

				pstHome->Info.Siol.One[i].ChengshuTime = 0;
				pstHome->Info.Siol.One[i].MonID = pstMonDef->ChengshuID;
				pstHome->Info.Siol.One[i].MonMemID = 0; // 这里清理掉memid,脚本采集完直接怪物死亡
			}
			else
			{
				iMemID = home_new_mon_by_dir(pstEnv, pstHome->Info.Siol.One[i].MonID, &pstDef->Soil[i].Pos, 0, pstMapInst);
				if (iMemID <= 0)
				{
					continue;
				}
				pstHome->Info.Siol.One[i].MonMemID = iMemID;
				
			}
		}
	}


	// 花园宠物让客户自己刷,要弹出ui
	// 模仿宠物寄养,客户端刷个模型就可以了
	
	return 0;
}

//  判断角色是否在家园里
int home_player_is_in(ZONESVRENV* pstEnv, Player *pstPlayer, int *piIn)
{
	ZONEIDX stIdx;
	ZONEMAPINST *pstMInst;
	ZONEPWORLDINST *pstPInst;
	ZoneHome *pstZoneHome;

	*piIn = 0;
	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = -1;
	pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMInst)
	{
		return 0;
	}
		
	pstPInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMInst->iPworldInstID);
	if(NULL == pstPInst)
		return 0;

	if (pstPInst->iHomeID <= 0)
	{
		return 0;
	}
	
	*piIn = 1; // 在家园里

	pstZoneHome = zonehome_one_find_by_id( pstEnv, pstPlayer->stRoleData.MiscInfo.Home.ID);
	if (!pstZoneHome)
	{
		return 0;
	}
	
	if (pstZoneHome->stHome.Info.PworldinstID == pstMInst->iPworldInstID)
	{	
		*piIn = 2;//在自己的家园里
	}
	return 0;
}


int home_player_login_in(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZONEIDX stIdx;
	ZONEMAPINST *pstMInst;
	ZONEPWORLDINST *pstPInst;
	ZoneHome *pstZoneHome;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = -1;
	pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMInst)
	{
		return 0;
	}
		
	pstPInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMInst->iPworldInstID);
	if(NULL == pstPInst)
		return 0;

	if (pstPInst->iHomeID <= 0)
	{
		return 0;
	}

	pstZoneHome = zonehome_one_find_by_id( pstEnv, pstPInst->iHomeID);
	if (!pstZoneHome)
	{
		return 0;
	}
	

	home_single_notify(pstEnv, pstPlayer, &pstZoneHome->stHome);
	return 0;
}

HOME* home_one_find_by_player(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	HomeObj *pstObj = pstEnv->pstHomeObj;
	
	for (i=0; i<pstObj->iHomeNum; i++)
	{
		if (pstPlayer->stRoleData.RoleID == pstObj->astZoneHome[i].stHome.RoleID)
		{
			return &pstObj->astZoneHome[i].stHome;
		}
	}	
	return NULL;
}

HOME* home_one_find_by_id(ZONESVRENV* pstEnv,int iID)
{
	int i;
	HomeObj *pstObj = pstEnv->pstHomeObj;
	
	for (i=0; i<pstObj->iHomeNum; i++)
	{
		if (iID  == (int)pstObj->astZoneHome[i].stHome.HomeID)
		{
			return &pstObj->astZoneHome[i].stHome;
		}
	}	
	return NULL;
}


ZoneHome* zonehome_one_find_by_id(ZONESVRENV* pstEnv,int iID)
{
	int i;
	HomeObj *pstObj = pstEnv->pstHomeObj;
	
	for (i=0; i<pstObj->iHomeNum; i++)
	{
		if (iID  == (int)pstObj->astZoneHome[i].stHome.HomeID)
		{
			return &pstObj->astZoneHome[i];
		}
	}	
	return NULL;
}


int home_db_insert_req( ZONESVRENV* pstEnv,Player *pstPlayer, HOME *pstHome)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMINSERTREQ *pstInsertReq =  &pstPkg->Body.InsertReq;
	DBHomeInsertBack stBack;

	stBack.RoleID = pstPlayer->stRoleData.RoleID;
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,pstPlayer->stRoleData.Uin,&pstPkg->Head, 
								TORM_INSERT_REQ,DB_HOME_INSERT_RES, 
			               			"Home", 
			               			(char*)&stBack, 
			               			sizeof(stBack));
	
	pstInsertReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstInsertReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstInsertReq->OperInfo.WhereDef[0] = 0;
	
	iDst = pstEnv->iMiscDBID;
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,pstHome, 
			sizeof(*pstHome),&iDst,pstEnv->iRoleDBProtoVer);
}

int home_db_insert_res(ZONESVRENV* pstEnv, Player *pstPlayer, DBAsyn *pstDBAsyn, TORMINSERTRES *pstInsertRes)
{
	DBHomeInsertBack *pstBack;
	int i;
	HomeObj *pstObj = pstEnv->pstHomeObj;
	HOME *pstHome=NULL;

	pstBack = (DBHomeInsertBack *)(pstDBAsyn->AsynCmd.szMaxBuf);
	
	if (TORM_ERR_IS_ERROR(pstInsertRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "home_db_insert_res error 1" );
		goto error;
	}

	for (i=0; i<pstObj->iHomeNum; i++)
	{
		if (pstObj->astZoneHome[i].stHome.RoleID == pstBack->RoleID)
		{
			pstObj->astZoneHome[i].stHome.HomeID = pstInsertRes->LastInsertID;
			pstHome = &pstObj->astZoneHome[i].stHome;
			break;
		}
	}

	if (i>=pstObj->iHomeNum)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "home_db_insert_res error 2" );
		goto error;
	}

	if (home_init_build(pstEnv, pstHome) < 0)
	{
		
	}

	if ( !pstPlayer ||
		pstPlayer->stRoleData.RoleID != pstBack->RoleID ||
		PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
	{
		// 这种情况 角色登录的时候检查一下
		return 0;
	}

	pstPlayer->stRoleData.MiscInfo.Home.ID = pstInsertRes->LastInsertID;
	home_single_notify(pstEnv,  pstPlayer, pstHome);
	return 0;
	
error:
	
	for (i = pstObj->iHomeNum-1; i>=0; i--)
	{
		if (pstObj->astZoneHome[i].stHome.RoleID == pstBack->RoleID)
		{
			pstObj->iHomeNum -- ;
			if (i != pstObj->iHomeNum)
			{
				pstObj->astZoneHome[i] = pstObj->astZoneHome[pstObj->iHomeNum];
			}
			break;
		}
	}
	return -1;
}



int home_new(ZONESVRENV* pstEnv, Player *pstPlayer, HOMENEW *pstNew)
{
	HomeObj *pstObj = pstEnv->pstHomeObj;
//	int i;
	HOME *pstHome;
	HOMELVLDEF *pstDef;
	ITEMDEF *pstItemDef;

	// 大于等于200不让建了
	if (pstObj->iHomeNum >= HOME_CLT_MAX)
	{
		return -1;
	}

	// 自己有家园了
	pstHome = home_one_find_by_player(pstEnv, pstPlayer);
	if (pstHome)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_07);
		return -1;
	}

	pstDef = home_lvl_def_find(pstEnv, 1);
	if (!pstDef)
		return -1;

	if (pstDef->NewUseMoney > 0)
	{
		if (package_dec_money_test(pstEnv,  pstPlayer, ATTR_ID_MONEY, pstDef->NewUseMoney) < 0)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STORE1);
			return -1;
		}	
	}
	

	if (pstDef->NewUseItem> 0)
	{
		if (package_dec_test(pstEnv->pstAppCtx,  pstEnv,pstPlayer, pstDef->NewUseItem, 1, -1,NULL,0) < 0)
		{
			pstItemDef = z_find_itemdef(pstEnv, pstDef->NewUseItem);
			if (pstItemDef)
			{
				z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_HOME1,pstItemDef->Name);
			}
			return -1;
		}
	}
	

	memset(&pstObj->astZoneHome[pstObj->iHomeNum], 0, sizeof(pstObj->astZoneHome[pstObj->iHomeNum]));
	pstHome = &pstObj->astZoneHome[pstObj->iHomeNum].stHome;
	
	pstHome->CreateTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstHome->RoleID = pstPlayer->stRoleData.RoleID;
	pstHome->Lvl = 1;
	pstHome->Info.Siol.Lvl = 1;
	strncpy(pstHome->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstHome->RoleName));
	strncpy(pstHome->Info.Name, LJY_HOME_DEFAULT_NAME, sizeof(pstHome->Info.Name));
	pstObj->iHomeNum ++;
	
	if (home_db_insert_req(  pstEnv,pstPlayer, pstHome) < 0)
	{
		pstObj->iHomeNum--;
		return -1;
	}

	if (pstDef->NewUseMoney > 0)
	{
		package_dec_money(pstEnv,  pstPlayer, ATTR_ID_MONEY, pstDef->NewUseMoney);
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_HOME, 
				0, 0, pstDef->NewUseMoney, "家园新建");
	}

	if (pstDef->NewUseItem> 0)
	{
		package_dec(pstEnv->pstAppCtx,  pstEnv,pstPlayer, pstDef->NewUseItem, 1, -1,NULL,0,IMPITEM_OPTYPE_HOME);
	}

	z_home_oplog(pstEnv,pstPlayer,NULL,pstHome->HomeID,
					  pstHome->Lvl,pstDef->NewUseMoney,pstDef->NewUseItem,pstHome->CreateTime,"家园新建");
	
	pstHome->Info.LastEnterTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	return 0;
}


// ipage---第几页 从0开始
int home_page(ZONESVRENV* pstEnv, Player *pstPlayer, int iPage)
{
	HomeObj *pstObj = pstEnv->pstHomeObj;
	int iCount;
	int i;

	iCount = pstObj->iHomeNum/HOME_PAGE_MAX +(pstObj->iHomeNum%HOME_PAGE_MAX?1:0);
	
	if (iPage > iCount)
	{
		return 0;
	}
	
	CSPKG stPkg;
	CSHOMESVR *pstSvr = &stPkg.Body.HomeSvr;

	memset(pstSvr, 0, sizeof(*pstSvr));

	pstSvr->Cmd = HOME_SVR_OP_PAGE;
	pstSvr->Data.Page.CountPage = iCount;
	pstSvr->Data.Page.Page = iPage;
	pstSvr->Data.Page.CountHome = pstObj->iHomeNum;
	
	for (i=iPage*HOME_PAGE_MAX; i<pstObj->iHomeNum; i++)
	{
		pstSvr->Data.Page.Homes[(int)pstSvr->Data.Page.Num] = pstObj->astZoneHome[i].stHome;
		pstSvr->Data.Page.Num ++;

		if (pstSvr->Data.Page.Num >= HOME_PAGE_MAX)
		{
			break;
		}
	}
	
	Z_CSHEAD_INIT(&stPkg.Head,HOME_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}


int home_single_notify(ZONESVRENV* pstEnv, Player *pstPlayer, HOME *pstHome)
{
	CSPKG stPkg;
	CSHOMESVR *pstSvr = &stPkg.Body.HomeSvr;

	memset(pstSvr, 0, sizeof(*pstSvr));

	pstSvr->Cmd = HOME_SVR_OP_SINGLE;
	pstSvr->Data.Single.Home = *pstHome;

	Z_CSHEAD_INIT(&stPkg.Head,HOME_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int home_chgname_notify(ZONESVRENV* pstEnv, Player *pstPlayer, HOME *pstHome)
{
	CSPKG stPkg;
	CSHOMESVR *pstSvr = &stPkg.Body.HomeSvr;

	memset(pstSvr, 0, sizeof(*pstSvr));

	pstSvr->Cmd = HOME_SVR_OP_NAME_CHG;
	pstSvr->Data.Name.ID = pstHome->HomeID;
	strncpy(pstSvr->Data.Name.Name, pstHome->Info.Name, sizeof(pstSvr->Data.Name.Name));

	Z_CSHEAD_INIT(&stPkg.Head,HOME_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int home_house_notify(ZONESVRENV* pstEnv, Player *pstPlayer, HOME *pstHome)
{
	CSPKG stPkg;
	CSHOMESVR *pstSvr = &stPkg.Body.HomeSvr;

	memset(pstSvr, 0, sizeof(*pstSvr));

	pstSvr->Cmd = HOME_SVR_OP_HOUSE_INFO;
	pstSvr->Data.House.House= pstHome->Info.House;

	Z_CSHEAD_INIT(&stPkg.Head,HOME_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int home_chgsale_notify(ZONESVRENV* pstEnv, Player *pstPlayer, HOME *pstHome)
{
	CSPKG stPkg;
	CSHOMESVR *pstSvr = &stPkg.Body.HomeSvr;

	memset(pstSvr, 0, sizeof(*pstSvr));

	pstSvr->Cmd = HOME_SVR_OP_SALE;
	pstSvr->Data.Sale.ID = pstHome->HomeID;
	pstSvr->Data.Sale.Sale = pstHome->Info.Sale;

	Z_CSHEAD_INIT(&stPkg.Head,HOME_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int home_del_notify(ZONESVRENV* pstEnv, Player *pstPlayer, int iID)
{
	CSPKG stPkg;
	CSHOMESVR *pstSvr = &stPkg.Body.HomeSvr;

	memset(pstSvr, 0, sizeof(*pstSvr));

	pstSvr->Cmd = HOME_SVR_OP_DEL;
	pstSvr->Data.Del.ID = iID;

	Z_CSHEAD_INIT(&stPkg.Head,HOME_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int home_pet_notify(ZONESVRENV* pstEnv, Player *pstPlayer, HOME *pstHome, ZONEMAPINST *pstMInst)
{
	CSPKG stPkg;
	CSHOMESVR *pstSvr = &stPkg.Body.HomeSvr;

	memset(pstSvr, 0, sizeof(*pstSvr));

	pstSvr->Cmd = HOME_SVR_OP_PET;
	pstSvr->Data.Pet.Pet = pstHome->Info.Pet;

	Z_CSHEAD_INIT(&stPkg.Head,HOME_SVR);

	if (pstMInst)
	{
		return map_broadcast(pstEnv->pstAppCtx, pstEnv, pstMInst, &stPkg);
	}

	if (pstPlayer)
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int home_buy_notify(ZONESVRENV* pstEnv, Player *pstPlayer, HOME *pstHome)
{
	CSPKG stPkg;
	CSHOMESVR *pstSvr = &stPkg.Body.HomeSvr;

	memset(pstSvr, 0, sizeof(*pstSvr));

	pstSvr->Cmd = HOME_SVR_OP_BUY;
	pstSvr->Data.Buy.ID = pstHome->HomeID;
	strncpy(pstSvr->Data.Buy.Name, pstHome->RoleName, sizeof(pstSvr->Data.Buy.Name));
	

	Z_CSHEAD_INIT(&stPkg.Head,HOME_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int home_liuyan_notify(ZONESVRENV* pstEnv, Player *pstPlayer, HOME *pstHome)
{
	CSPKG stPkg;
	CSHOMESVR *pstSvr = &stPkg.Body.HomeSvr;

	memset(pstSvr, 0, sizeof(*pstSvr));

	pstSvr->Cmd = HOME_SVR_OP_LIUYAN_CHG;
	pstSvr->Data.Liuyan.ID = pstHome->HomeID;
	pstSvr->Data.Liuyan.Liuyan = pstHome->Info.Liuyan;
	
	Z_CSHEAD_INIT(&stPkg.Head,HOME_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int home_stat_notify(ZONESVRENV* pstEnv, Player *pstPlayer, HOME *pstHome)
{
	CSPKG stPkg;
	CSHOMESVR *pstSvr = &stPkg.Body.HomeSvr;

	memset(pstSvr, 0, sizeof(*pstSvr));

	pstSvr->Cmd = HOME_SVR_OP_STAT_CHG;
	pstSvr->Data.Stat.ID = pstHome->HomeID;
	pstSvr->Data.Stat.Stat = pstHome->Info.Stat;
	
	Z_CSHEAD_INIT(&stPkg.Head,HOME_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int home_set_name(ZONESVRENV* pstEnv, Player *pstPlayer, HOMESETNAME*pstSetName)
{
	HOME *pstHome;

	pstHome = home_one_find_by_id(pstEnv,pstSetName->Id);
	if (!pstHome)
		return -1;

	if (pstHome->RoleID != pstPlayer->stRoleData.RoleID)
	{
		return -1;
	}

	if (pstSetName->Name[0] == '\0')
	{
		//return -1;
	}

	if (strcmp(pstSetName->Name,pstHome->Info.Name) == 0)
	{
		return -1;
	}

	if ((CheckDirty(pstSetName->Name , 0x1f) & 0x1f ) )
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIRTY);
		return -1;
	}

	strncpy(pstHome->Info.Name, pstSetName->Name, sizeof(pstHome->Info.Name));

	// 通知客户端
	home_chgname_notify( pstEnv, pstPlayer,  pstHome);
	return 0;
}

int home_set_stat(ZONESVRENV* pstEnv, Player *pstPlayer, HOMESETSTAT*pstSetStat)
{
	HOME *pstHome;

	pstHome = home_one_find_by_id(pstEnv,pstSetStat->Id);
	if (!pstHome)
		return -1;

	if (pstHome->RoleID != pstPlayer->stRoleData.RoleID)
	{
		return -1;
	}

	if (pstHome->Info.Stat == pstSetStat->Stat)
	{
		return -1;
	}

	if (pstSetStat->Stat < HOME_STAT_OPEN || pstSetStat->Stat > HOME_STAT_CLOSE)
	{
		return -1;
	}

	pstHome->Info.Stat = pstSetStat->Stat;

	// 通知客户端
	home_stat_notify(pstEnv, pstPlayer, pstHome);
	return 0;
}

int home_msg(ZONESVRENV* pstEnv, Player *pstPlayer, HOMEMSG *pstMsg)
{
	HOME *pstHome;
	int iIdx;

	pstHome = home_one_find_by_id(pstEnv,pstMsg->Id);
	if (!pstHome)
		return -1;

	if (pstMsg->Msg[0] == '\0')
	{
		return -1;
	}

	if ((CheckDirty(pstMsg->Msg, 0x1f) & 0x1f ) )
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIRTY);
		return -1;
	}

	if (pstHome->Info.Liuyan.Num >= HOME_MAX_LIUYAN)
	{
		memmove(&pstHome->Info.Liuyan.One[0],
					&pstHome->Info.Liuyan.One[1],
					sizeof(pstHome->Info.Liuyan.One[0])*(HOME_MAX_LIUYAN-1));	
		iIdx = HOME_MAX_LIUYAN-1;
	}
	else
	{
		iIdx = pstHome->Info.Liuyan.Num;
		pstHome->Info.Liuyan.Num ++;
	}

	strncpy(pstHome->Info.Liuyan.One[iIdx].Text, pstMsg->Msg, 
				sizeof(pstHome->Info.Liuyan.One[iIdx].Text));
	strncpy(pstHome->Info.Liuyan.One[iIdx].Name,pstPlayer->stRoleData.RoleName, 
			sizeof(pstHome->Info.Liuyan.One[iIdx].Name));
	pstHome->Info.Liuyan.One[iIdx].Time = pstEnv->pstAppCtx->stCurr.tv_sec;

	// 通知客户端
	home_liuyan_notify( pstEnv, pstPlayer,  pstHome);
	return 0;
}


int home_db_delete_req( ZONESVRENV* pstEnv, Player *pstPlayer,HOME *pstHome)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMDELETEREQ *pstReq =  &pstPkg->Body.DeleteReq;
	DBHomeInsertBack stBack;

	stBack.iID = pstHome->HomeID;
	if (pstPlayer)
	{
		strncpy(stBack.RoleName, pstPlayer->stRoleData.RoleName, sizeof(stBack.RoleName));
	}
	else
	{
		stBack.RoleName[0] = '\0';
	}
	
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,0,&pstPkg->Head, 
								TORM_DELETE_REQ,DB_HOME_DELETE_RES, 
			               			"Home", 
			               			(char*)&stBack, 
			               			sizeof(stBack));
	
	pstReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstReq->OperInfo.EntryNameList.EntryNameNum = 0;

	snprintf(pstReq->OperInfo.WhereDef,sizeof(pstReq->OperInfo.WhereDef),"where HomeID = %d",pstHome->HomeID);
	
	iDst = pstEnv->iMiscDBID;
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,pstHome, 
			sizeof(*pstHome),&iDst,pstEnv->iRoleDBProtoVer);
	
}

int home_player_clear_id(Player *pstPlayer)
{
	pstPlayer->stRoleData.MiscInfo.Home.ID = 0;
	return 0;
}

int home_player_id(Player *pstPlayer, int iID)
{
	pstPlayer->stRoleData.MiscInfo.Home.ID = iID;
	return 0;
}


int home_login_check(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	HomeObj *pstHomeObj = pstEnv->pstHomeObj;
	int i;

	for (i=0; i<pstHomeObj->iHomeNum; i++)
	{
		if (pstHomeObj->astZoneHome[i].stHome.RoleID == pstPlayer->stRoleData.RoleID)
		{
			if (pstPlayer->stRoleData.MiscInfo.Home.ID != (int)pstHomeObj->astZoneHome[i].stHome.HomeID)
			{
				home_player_id( pstPlayer, pstHomeObj->astZoneHome[i].stHome.HomeID);
			}
		}

		if (pstPlayer->stRoleData.MiscInfo.Home.ID == (int)pstHomeObj->astZoneHome[i].stHome.HomeID)
		{
			if (pstHomeObj->astZoneHome[i].stHome.RoleID != pstPlayer->stRoleData.RoleID)
			{
				home_player_clear_id(pstPlayer);
			}
		}
	}
	return 0;
}

int home_db_delete_res(ZONESVRENV* pstEnv, DBAsyn *pstDBAsyn, TORMDELETERES *pstRes)
{
	DBHomeInsertBack *pstBack;
	HomeObj *pstHomeObj = pstEnv->pstHomeObj;
	Player *pstOwner;
	int i;
	int iPInst = 0;
	char Lvl = 0;    

	pstBack = (DBHomeInsertBack *)(pstDBAsyn->AsynCmd.szMaxBuf);
	if (TORM_ERR_IS_ERROR(pstRes->ErrNo))
	{
		goto error;
	}

	pstOwner = z_name_player(pstEnv->pstAppCtx, pstEnv, pstBack->RoleName);

	// 防止角色在战场数据被覆盖
	if (pstOwner && pstOwner->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		pstOwner = NULL;
	}

	for (i=pstHomeObj->iHomeNum-1; i>=0; i--)
	{
		if ((int)pstHomeObj->astZoneHome[i].stHome.HomeID == pstBack->iID)
		{
			iPInst = pstHomeObj->astZoneHome[i].stHome.Info.PworldinstID;
			Lvl = pstHomeObj->astZoneHome[i].stHome.Lvl;
			pstHomeObj->iHomeNum--;
			if (i != pstHomeObj->iHomeNum)
			{
				memmove(&pstHomeObj->astZoneHome[i], &pstHomeObj->astZoneHome[i+1], 
						sizeof(pstHomeObj->astZoneHome[0])*(pstHomeObj->iHomeNum-i));
			}

			break;
		}
	}
	
	if (pstOwner)
	{
		home_player_clear_id(pstOwner);
		// 通知客户端
		home_del_notify(pstEnv, pstOwner, pstBack->iID);
	}
	
	if (iPInst)
	{
		ZONEPWORLDINST *pstPinst;
		pstPinst = z_find_pworld_inst(pstEnv->pstMapObj,iPInst);
		if (pstPinst)
		{
			z_in_destroy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstPinst);
		}
	}

	z_home_oplog(pstEnv,pstOwner,NULL,pstBack->iID,
					  Lvl,0,0,pstEnv->pstAppCtx->stCurr.tv_sec,"家园销毁");
error:
	
	return 0;
}

int home_destory(ZONESVRENV* pstEnv, Player *pstPlayer, HOMEDESTORY*pstDestory)
{
	HOME *pstHome;

	pstHome = home_one_find_by_id(pstEnv,pstDestory->Id);
	if (!pstHome)
		return -1;

	if (pstPlayer->stRoleData.RoleID != pstHome->RoleID)
	{
		return -1;
	}

	if (home_db_delete_req( pstEnv, pstPlayer,pstHome) < 0)
	{
		return -1;
	}

	return 0;
}

int home_sale(ZONESVRENV* pstEnv, Player *pstPlayer, HOMESALE*pstSale)
{
	ZoneHome *pstHome;

	pstHome = zonehome_one_find_by_id(pstEnv,pstSale->Id);
	if (!pstHome)
		return -1;

	if (pstPlayer->stRoleData.RoleID != pstHome->stHome.RoleID)
	{
		return -1;
	}

	if (pstSale->Money < 0)
	{
		return -1;
	}
	
	pstHome->stHome.Info.Sale = pstSale->Money;

	
	pstHome->stZoneHomeInfo.iDirty = 2; // 立即更新

	// 通知客户端
	home_chgsale_notify(pstEnv,pstPlayer, &pstHome->stHome);
	return 0;
}

void home_pet_add_val(int *piVal, int iAdd, int iMax)
{
	*piVal += iAdd;
	if (*piVal > iMax) *piVal = iMax;
}


void home_pet_init(HOME *pstHome, int iID)
{
	pstHome->Info.Pet.Baoshi = HOME_PET_EAT_ITEM_MAX;
	pstHome->Info.Pet.Exp = 0;
	pstHome->Info.Pet.Happy = HOME_PET_PLAY_ITEM_MAX;
	pstHome->Info.Pet.ID = iID;
	pstHome->Info.Pet.Jiankang = HOME_PET_DUANLIAN_ITEM_MAX;
	pstHome->Info.Pet.Lvl = 1;
	pstHome->Info.Pet.Qingjie = HOME_PET_CLEAR_ITEM_MAX;
}

int home_pet_replace(ZONESVRENV* pstEnv, Player *pstPlayer, HOME *pstHome, int iIdx, ZONEMAPINST *pstMInst)
{
	ROLEGRID *pstUseGrid;
	int iPos;
	ITEMDEF *pstItemDef;
	HOMEMONSEGDEF *pstSegDef;
	
	pstUseGrid =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, iIdx);
	if (!pstUseGrid || pstUseGrid->Type != GRID_ITEM_ITEM)
	{
		return -1;
	}

	iPos = pstUseGrid - &pstPlayer->stRoleData.Package.SundriesPackage.RoleGrids[0];
	if (iPos < 0 || iPos >= pstPlayer->stRoleData.Package.SundriesPackage.Num)
	{
		return -1;
	}

	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstUseGrid->GridData.RoleItem);
	if (!pstItemDef)
		return -1;

	if (pstItemDef->Result[0].ResultID != RESULT_ITEM_HOME_PET)
	{
		return -1;
	}

	if (pstHome->Info.Pet.ID == pstItemDef->Result[0].ResultVal2)
	{
		return -1;
	}

	pstSegDef = home_mon_seg_def_find( pstEnv, pstItemDef->Result[0].ResultVal2);
	if (!pstSegDef)
		return -1;

	home_pet_init(pstHome, pstItemDef->Result[0].ResultVal2);
	
	package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDef->ItemID, 1,
				LIST_TYPE_SUNDRIES, &iPos,0, IMPITEM_OPTYPE_HOME);

	home_pet_notify(pstEnv, pstPlayer, pstHome, pstMInst);
	return 0;
}

int home_pet_get(ZONESVRENV* pstEnv, Player *pstPlayer, HOME *pstHome)
{
	HOMEMONSEGDEF *pstDef;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	GARNDENMONSEG *pstSeg =NULL;
	int i;
	int iCountRate = 0;
	int iRate;
	int iEggID = 0;
	
	pstDef = home_mon_seg_def_find( pstEnv, pstHome->Info.Pet.ID);
	if (!pstDef)
		return -1;

	if (IsSameDay(pstHome->Info.Pet.GetTime, iCurr))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_02);
		return -1;
	}

	if (package_get_freegrid_num(pstEnv->pstAppCtx,  pstEnv,pstPlayer, LIST_TYPE_SUNDRIES) < 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_03);
		return -1;
	}

	for (i=0; i<GARDEN_MAX_LVL_SEG; i++)
	{
		if (pstHome->Info.Pet.Lvl <= pstDef->Seg[i].Lvl)
		{
			pstSeg = &pstDef->Seg[i];
			break;
		}
	}

	if (!pstSeg)
	{
		return -1;
	}

	for (i=0; i<GARDEN_MON_MAX_EGG; i++)
	{
		iCountRate+=pstSeg->Egg[i].Rate;
	}

	if (iCountRate <= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_14);
		return -1;
	}

	iRate = RAND1(iCountRate);
	iCountRate = 0;

	for (i=0; i<GARDEN_MON_MAX_EGG; i++)
	{
		iCountRate+=pstSeg->Egg[i].Rate;
		if (iRate < iCountRate)
		{
			iEggID = pstSeg->Egg[i].EggID;
			break;
		}
	}

	if (iEggID <= 0)
		return -1;

	if (package_add_by_id(pstEnv, pstPlayer, iEggID, 1, 0) < 0)
	{
		return -1;
	}

	pstHome->Info.Pet.GetTime = iCurr;
	return 0;
}



int home_pet(ZONESVRENV* pstEnv, Player *pstPlayer, HOMEPET *pstPet)
{
	int iItemID = 0;
	ZONEPWORLDINST *pstPInst;
	ZONEMAPINST *pstMInst;
	ZONEIDX stIdx;
	ZoneHome *pstZoneHome;
	HOME *pstHome;
	HOMEMON *pstHomePet;
	
	int *piVal = NULL;
	int iAdd,iMax;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = -1;
	pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMInst)
		return -1;


	pstPInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMInst->iPworldInstID);
	if(NULL == pstPInst)
		return -1;

	if (pstPInst->iHomeID <= 0)
	{
		return -1;
	}
	
	pstZoneHome = zonehome_one_find_by_id( pstEnv, pstPInst->iHomeID);
	if (!pstZoneHome)
	{
		return -1;
	}

	if (pstZoneHome->stHome.RoleID != pstPlayer->stRoleData.RoleID)
	{	
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_04);
		return -1;
	}

	pstHome = &pstZoneHome->stHome;
	pstHomePet = &pstHome->Info.Pet;
	switch(pstPet->Type)
	{
	case HOME_PET_OP_EAT:
		{
			iItemID = HOME_PET_EAT_ITEM;
			if (pstHomePet->Baoshi >= HOME_PET_EAT_ITEM_MAX)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_09);
				return -1;
			}

			piVal = &pstHomePet->Baoshi;
			iAdd = HOME_PET_EAT_ITEM_VAL;
			iMax = HOME_PET_EAT_ITEM_MAX;
		}
		break;
	case HOME_PET_OP_DUANLIAN:
		{
			iItemID = HOME_PET_DUANLIAN_ITEM;
			if (pstHomePet->Jiankang >= HOME_PET_DUANLIAN_ITEM_MAX)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_12);
				return -1;
			}

			piVal = &pstHomePet->Jiankang;
			iAdd = HOME_PET_DUANLIAN_ITEM_VAL;
			iMax = HOME_PET_DUANLIAN_ITEM_MAX;
		}
		break;
	case HOME_PET_OP_CLAER:
		{
			iItemID = HOME_PET_CLEAR_ITEM;
			if (pstHomePet->Qingjie >= HOME_PET_CLEAR_ITEM_MAX)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_10);
				return -1;
			}
			piVal = &pstHomePet->Qingjie;
			iAdd = HOME_PET_CLEAR_ITEM_VAL;
			iMax = HOME_PET_CLEAR_ITEM_MAX;
		}
		break;
	case HOME_PET_OP_PLAY:
		{
			iItemID = HOME_PET_PLAY_ITEM;
			if (pstHomePet->Happy >= HOME_PET_PLAY_ITEM_MAX)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_11);
				return -1;
			}
			piVal = &pstHomePet->Happy;
			iAdd  = HOME_PET_CLEAR_ITEM_VAL;
			iMax = HOME_PET_CLEAR_ITEM_MAX;
		}
		break;
	case HOME_PET_OP_GET:
		{
			return home_pet_get(pstEnv, pstPlayer, pstHome);
		}
		break;
	case HOME_PET_OP_REPLACE:
		return home_pet_replace(pstEnv, pstPlayer, pstHome, pstPet->GridIdx, pstMInst);
		break;
	default:
		return -1;
		break;
	}

	

	if (piVal)
	{
		if (package_dec_test(pstEnv->pstAppCtx,  pstEnv,  pstPlayer,iItemID,1, -1, NULL, 0) < 0)
		{
			ITEMDEF *pstItemDef;

			pstItemDef= z_find_itemdef(pstEnv, iItemID);
			if (pstItemDef)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_MACHINE22, pstItemDef->Name);

			}
			return -1;
		}

		package_dec(pstEnv->pstAppCtx,  pstEnv, pstPlayer,iItemID, 1,-1,NULL,0,IMPITEM_OPTYPE_HOME);

		
		home_pet_add_val(piVal, iAdd, iMax);
		home_pet_notify(pstEnv, pstPlayer, pstHome, pstMInst);
	}

	
	return 0;
}

int home_buy(ZONESVRENV* pstEnv, Player *pstPlayer, HOMEBUY*pstBuy)
{
	ZoneHome *pstZoneHome;
	HOME *pstHome;
	Player *pstSale;

	pstZoneHome = zonehome_one_find_by_id(pstEnv,pstBuy->Id);
	if (!pstZoneHome)
		return -1;

	pstHome = &pstZoneHome->stHome;
	if (pstHome->Info.Sale <= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_08, pstHome->RoleName);
		return -1;
	}

	if (home_one_find_by_player(pstEnv, pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_07);
		return -1;
	}

	if (package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, pstHome->Info.Sale) < 0)
	{	
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_05);
		return -1;
	}

	pstSale = z_name_player(pstEnv->pstAppCtx,  pstEnv, pstHome->RoleName);
	if (pstSale)
	{
		home_player_clear_id(pstSale);
	}

	player_sysmail_fast_send(pstEnv, pstHome->RoleName, LJY_SALE_HOME, LJY_SALE_HOME_TEXT, NULL, 0,
				pstHome->Info.Sale*0.95, LJY_MAIL43, 0);
	
	home_player_id(pstPlayer, pstHome->HomeID);
	
	pstHome->RoleID = pstPlayer->stRoleData.RoleID;
	
	strncpy(pstHome->RoleName, pstPlayer->stRoleData.RoleName,sizeof(pstHome->RoleName));

	pstZoneHome->stZoneHomeInfo.iDirty = 2; // 立即更新

	package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, pstHome->Info.Sale);
	z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_HOME, 
				0, 0, pstHome->Info.Sale, "家园购买");

	z_home_oplog(pstEnv,pstPlayer,pstSale,pstHome->HomeID,
					  pstHome->Lvl,pstHome->Info.Sale,0,pstEnv->pstAppCtx->stCurr.tv_sec,"家园购买");

	pstHome->Info.Sale = 0;

	home_buy_notify( pstEnv,pstPlayer, pstHome);
	return 0;
}

int home_enter(ZONESVRENV* pstEnv, Player *pstPlayer, HOMEENTER*pstEnter)
{
	ZoneHome *pstZoneHome;
	HOME *pstHome;
	ZONEPWORLD *pstZonePworld;
	ZONEPWORLDINST *pstPInst;
	ZONEIDX stIdx;
	HOMELVLDEF *pstDef;

	pstZoneHome = zonehome_one_find_by_id(pstEnv,pstEnter->Id);
	if (!pstZoneHome)
		return -1;

	pstHome = &pstZoneHome->stHome;

	if (pstPlayer->stRoleData.RoleID != pstHome->RoleID &&
		pstHome->Info.Stat != HOME_STAT_OPEN)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_06,pstZoneHome->stHome.RoleName);
		
		return -1;
	}

	if (pstHome->Info.PworldinstID <= 0)
	{
		return -1;
	}

	pstDef = home_lvl_def_find( pstEnv, pstHome->Lvl);
	if (!pstDef)
		return -1;

	stIdx.iID = pstDef->PworldID;
	stIdx.iPos = -1;
	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (NULL == pstZonePworld)
	{
		return -1;
	}

	pstPInst = z_find_pworld_inst(pstEnv->pstMapObj, pstHome->Info.PworldinstID);
	if(NULL == pstPInst)
	{
		return -1;
	}

	z_player_enter_pworld_by_inst(pstEnv, pstPlayer, pstZonePworld, pstPInst);

	home_single_notify(pstEnv,  pstPlayer, pstHome);
	pstZoneHome->stHome.Info.LastEnterTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	return 0;
}


int home_point_is_in_rect(RESPOS *pstPos, HOMEPOS *pstRect)
{
	int x1 = pstPos->X - pstRect->OPos.X;
	int y1 = pstPos->Y - pstRect->OPos.Y;

	if( (x1>= -pstRect->Xlen/2) && 
		(x1<= pstRect->Xlen/2) && 
		(y1>= -pstRect->Ylen/2) &&
		(y1<= pstRect->Ylen/2) ) 
		return 1;
	else 
		return 0;
	
	return 0;
}

int home_put(ZONESVRENV* pstEnv, Player *pstPlayer, HOMEPUT*pstPut)
{
	ZoneHome *pstZoneHome;
	HOME *pstHome;
	ZONEPWORLD *pstZonePworld;
	ZONEPWORLDINST *pstPInst;
	HOMELVLDEF *pstDef;
	ROLEGRID *pstUseGrid;
	int iPos;
	ITEMDEF *pstItemDef;
	int iMonID = 0;
	ZONEMAPINST *pstMInst;

	pstZoneHome = zonehome_one_find_by_id(pstEnv,pstPlayer->stRoleData.MiscInfo.Home.ID);
	if (!pstZoneHome)
		return -1;

	pstHome = &pstZoneHome->stHome;

	if (pstHome->RoleID != pstPlayer->stRoleData.RoleID)
	{
		return -1;
	}

	pstZonePworld = z_find_pworld2(pstEnv, pstPlayer->stRoleData.Map);
	if (NULL == pstZonePworld)
	{
		return -1;
	}

	pstPInst = z_find_pworld_inst(pstEnv->pstMapObj, pstHome->Info.PworldinstID);
	if(NULL == pstPInst)
	{
		return -1;
	}

	pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv,  &pstPInst->astMapInstIdx[0]);
	if (!pstMInst)
		return -1;

	
	if (pstHome->Info.House.Num >= HOME_MAX_JIAJU)
	{
		return -1;
	}

	pstDef = home_lvl_def_find(pstEnv, pstHome->Lvl);
	if (!pstDef)
	{
		return -1;
	}

	pstUseGrid =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstPut->GridIdx);
	if (!pstUseGrid || pstUseGrid->Type != GRID_ITEM_ITEM)
	{
		return -1;
	}

	iPos = pstUseGrid - &pstPlayer->stRoleData.Package.SundriesPackage.RoleGrids[0];
	if (iPos < 0 || iPos >= pstPlayer->stRoleData.Package.SundriesPackage.Num)
	{
		return -1;
	}

	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstUseGrid->GridData.RoleItem);
	if (!pstItemDef)
		return -1;

	if (pstItemDef->Result[0].ResultID != RESULT_ITEM_JIAJU)
	{
		return -1;
	}

	iMonID = pstItemDef->Result[0].ResultVal2;
	if (iMonID <= 0)
		return -1;
	

	if (! home_point_is_in_rect(&pstPut->Pos, &pstDef->HousePos))
	{
		//return -1;
	}

	if (home_new_mon_by_dir(pstEnv, iMonID,&pstPut->Pos, pstPut->Dir, pstMInst)< 0)
	{
		return -1;
	}

	package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDef->ItemID, 1,
				LIST_TYPE_SUNDRIES, &iPos,0, IMPITEM_OPTYPE_HOME);
	
	pstHome->Info.House.One[pstHome->Info.House.Num].MonID = iMonID;
	pstHome->Info.House.One[pstHome->Info.House.Num].Dir = pstPut->Dir;
	pstHome->Info.House.One[pstHome->Info.House.Num].Pos = pstPut->Pos;
	pstHome->Info.House.Num++;

	home_house_notify( pstEnv, pstPlayer,pstHome);
	return 0;
}


// 农场信息变化
int home_farm_notify(ZONESVRENV* pstEnv, Player *pstPlayer, HOME *pstHome, ZONEMAPINST *pstMInst)
{
	CSPKG stPkg;
	CSHOMESVR *pstSvr = &stPkg.Body.HomeSvr;

	memset(pstSvr, 0, sizeof(*pstSvr));

	pstSvr->Cmd = HOME_SVR_OP_FARM;
	pstSvr->Data.Farm.Farm = pstHome->Info.Siol;

	Z_CSHEAD_INIT(&stPkg.Head,HOME_SVR);

	if (pstMInst)
	{
		return map_broadcast(pstEnv->pstAppCtx, pstEnv, pstMInst, &stPkg);
	}

	if (pstPlayer)
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

// 脚本调用采集完后农场加经验
int home_farm_add_exp(ZONESVRENV* pstEnv, Player *pstPlayer, int iExp)
{
	ZoneHome *pstZoneHome;
	HOME *pstHome;
	HOMESIOL *pstSiol;
	HOMEFARMDEF *pstDef;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMInst;
	ZONEPWORLDINST *pstPInst;
	
	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = -1;
	pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMInst)
	{
		return 0;
	}
		
	pstPInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMInst->iPworldInstID);
	if(NULL == pstPInst)
		return 0;

	if (pstPInst->iHomeID <= 0)
	{
		return 0;
	}
	
	pstZoneHome = zonehome_one_find_by_id( pstEnv, pstPInst->iHomeID);
	if (!pstZoneHome)
	{
		return 0;
	}

	pstHome = &pstZoneHome->stHome;
	pstSiol = &pstHome->Info.Siol;

	pstDef = home_farm_def_find( pstEnv, pstSiol->Lvl);
	if (!pstDef)
		return 0;

	pstSiol->Exp += iExp;
	if (pstSiol->Exp >= pstDef->Exp)
	{
		pstSiol->Exp -= pstDef->Exp;
		pstSiol->Lvl ++;
	}

	home_farm_notify(pstEnv, pstPlayer, pstHome,NULL);
	return 0;
}



int home_get_kaiken_num(HOME *pstHome)
{
	int iNum = 0;
	int i;

	for (i=0; i<pstHome->Info.Siol.Num; i++)
	{
		if (pstHome->Info.Siol.One[i].Stat == 1)
		{		
			iNum++;
		}
	}

	return iNum;
}

// 脚本调用开垦土地
int home_farm_kaiken(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZoneHome *pstZoneHome;
	HOME *pstHome;
	HOMESIOL *pstSiol;
	HOMEFARMDEF *pstDef;
	int i;
	Monster *pstMon;
	int iIn=0;
	HOMESOILDEF *pstHomeSoilDef = NULL;
	int iNum = 0;
	
	pstZoneHome = zonehome_one_find_by_id( pstEnv, pstPlayer->stRoleData.MiscInfo.Home.ID);
	if (!pstZoneHome)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_04);
		return -1;
	}

	home_player_is_in(pstEnv, pstPlayer, &iIn);
	if (iIn != 2)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_04);
		return -1;
	}

	pstHome = &pstZoneHome->stHome;
	pstSiol = &pstHome->Info.Siol;

	pstDef = home_farm_def_find( pstEnv, pstSiol->Lvl);
	if (!pstDef)
		return 0;

	iNum = home_get_kaiken_num(pstHome);
	if (iNum >= pstDef->SoilNum)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_13);
		return -1;
	}

	pstHomeSoilDef = home_soil_def_find(pstEnv, iNum+1);
	if(!pstHomeSoilDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_13);
		return -1;
	}

	if(pstSiol->Lvl < pstHomeSoilDef->VegFieldLevel)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_15);
		return -1;
	}

	if (pstHomeSoilDef->MoneyType && pstHomeSoilDef->MoneyNum && 
		package_dec_money_test(pstEnv, pstPlayer, pstHomeSoilDef->MoneyType, pstHomeSoilDef->MoneyNum) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_NOHUOBI);
		return -1;
	}

	for (i=0; i<pstHome->Info.Siol.Num; i++)
	{
		if (pstHome->Info.Siol.One[i].Stat == 0)
		{
			pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstHome->Info.Siol.One[i].MonMemID);
			if (pstMon)
			{
				z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
				z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);
			}

			pstHome->Info.Siol.One[i].Stat =1;
			pstHome->Info.Siol.One[i].MonID = 0;
			pstHome->Info.Siol.One[i].MonMemID = 0;

			home_farm_notify(pstEnv, pstPlayer, pstHome, NULL);

			if(pstHomeSoilDef->MoneyType && pstHomeSoilDef->MoneyNum)
			{
				package_dec_money(pstEnv, pstPlayer, pstHomeSoilDef->MoneyType, pstHomeSoilDef->MoneyNum);

				if(pstHomeSoilDef->MoneyType == ATTR_ID_GODCOIN)
				{
					godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
								pstPlayer->stRoleData.Level,GODCOIN_OPER_FLAG_HOME_KAIKEN,
								0,0, 0,pstHomeSoilDef->MoneyNum,"家园开垦土地");
				}
				else if(pstHomeSoilDef->MoneyType == ATTR_ID_BINDGODCOIN)
				{
					z_bindgold_consume_oplog(pstEnv, pstPlayer, BINDGOLD_OPTYPE_HOME_KAIKEN, 0, 
						pstHomeSoilDef->MoneyNum, "家园开垦土地");
				}
				else
				{
					z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_HOME, 
					                     0, 0, pstHomeSoilDef->MoneyNum, "家园开垦土地");
				}
			}
			break;
		}
	}
	return 0;
}

// 农场tick
int home_farm_tick(ZONESVRENV* pstEnv, HOME *pstHome)
{
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	int i;
	HOMESIOL *pstSiol;
	Monster *pstMon;
	ZONEMAPINST *pstMInst; 
	MONSTERDEF *pstMonDef;
	Monster stMon;
	int iMemID;
	HOMELVLDEF *pstDef;
	int iChg = 0;


	pstSiol = &pstHome->Info.Siol;
	pstMInst = home_get_map_inst( pstEnv, pstHome);
	if (!pstMInst)
		return 0;

	for (i=0; i<pstSiol->Num; i++)
	{
		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstSiol->One[i].MonMemID);
		if (!pstMon && pstSiol->One[i].MonMemID > 0)
		{
			pstSiol->One[i].MonMemID = 0;
			pstSiol->One[i].ChengshuTime = 0;
			pstSiol->One[i].MonID = 0;

			iChg = 1;
			continue;
		}
		
		if (pstSiol->One[i].ChengshuTime <= 0)
		{
			continue;
		}

		if (iCurr < pstSiol->One[i].ChengshuTime)
		{
			continue;
		}

		pstSiol->One[i].ChengshuTime = 0;
		
		if (pstMon)
		{
			z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
			z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);
		}	

		stMon.iDefIdx = pstSiol->One[i].MonID;
		pstMonDef = z_get_mon_def(pstEnv, &stMon);
		if (!pstMonDef)
			continue;

		pstDef = home_lvl_def_find(pstEnv, pstHome->Lvl);
		if (!pstDef)
			continue;
		
		iMemID = home_new_mon_by_dir(pstEnv, pstMonDef->ChengshuID,
				&pstDef->Soil[i].Pos, 0, pstMInst);

		if (iMemID < 0)
			continue;

		pstSiol->One[i].MonID = pstMonDef->ChengshuID;
		pstSiol->One[i].MonMemID = iMemID;

		iChg = 1;
	}

	if (iChg)
	{
		home_farm_notify(pstEnv, NULL, pstHome,pstMInst);	
	}
	return 0;
}

int home_zhongzhi(ZONESVRENV* pstEnv, Player *pstPlayer, HOMEZHONGZHI*pstZhongzhi)
{
	ZoneHome *pstZoneHome;
	HOME *pstHome;
	ZONEPWORLDINST*pstPInst;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMInst;
	int i;
	HOMESIOL *pstSiol;
	int iPos;
	ROLEGRID *pstUseGrid;
	ITEMDEF *pstItemDef;
	HOMELVLDEF *pstDef;
	int iMemID;
	Monster *pstMon;
	

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = -1;

	pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMInst)
	{
		return 0;
	}
		
	pstPInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMInst->iPworldInstID);
	if(NULL == pstPInst)
		return 0;

	if (pstPInst->iHomeID <= 0)
	{
		return 0;
	}

	pstZoneHome = zonehome_one_find_by_id( pstEnv, pstPlayer->stRoleData.MiscInfo.Home.ID);
	if (!pstZoneHome)
	{
		return 0;
	}

	pstHome = &pstZoneHome->stHome;
	pstSiol = &pstHome->Info.Siol;
	
	if (pstHome->Info.PworldinstID != pstMInst->iPworldInstID)
	{
		return 0;
	}

	for (i=0; i<pstSiol->Num; i++)
	{
		if (pstZhongzhi->Idx == pstSiol->One[i].Idx)
		{
			// 这块菜地种植了东东
			if (pstSiol->One[i].MonMemID > 0)
			{
				pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstSiol->One[i].MonMemID);

				if (pstMon)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_01);
				}
				return 0;
			}

			break;
		}
	}

	//  没找到指定的菜地
	if (i>=pstSiol->Num)
	{
		return 0;
	}

	pstUseGrid =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstZhongzhi->GridIdx);
	if (!pstUseGrid || pstUseGrid->Type != GRID_ITEM_ITEM)
	{
		return -1;
	}

	iPos = pstUseGrid - &pstPlayer->stRoleData.Package.SundriesPackage.RoleGrids[0];
	if (iPos < 0 || iPos >= pstPlayer->stRoleData.Package.SundriesPackage.Num)
	{
		return -1;
	}

	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstUseGrid->GridData.RoleItem);
	if (!pstItemDef)
		return -1;

	if (pstItemDef->Result[0].ResultID != RESULT_ITEM_ZHONGZHI ||
		pstItemDef->Result[0].ResultVal1 <= 0 ||
		pstItemDef->Result[0].ResultVal2 <= 0)
	{
		return -1;
	}

	if(pstItemDef->Result[0].ResultVal3 > 0 && pstSiol->Lvl < pstItemDef->Result[0].ResultVal3)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_16);
		return -1;
	}

	pstDef = home_lvl_def_find( pstEnv, pstHome->Lvl);
	if (!pstDef)
		return -1;

	if (pstDef->Soil[pstZhongzhi->Idx].Pos.X <= 0 ||
		pstDef->Soil[pstZhongzhi->Idx].Pos.Y<= 0)
	{
		return -1;
	}	
	
	if ((iMemID = home_new_mon_by_dir(pstEnv, pstItemDef->Result[0].ResultVal2,
				&pstDef->Soil[pstZhongzhi->Idx].Pos, 0,pstMInst)) < 0)
	{
		return -1;
	}

	package_dec(pstEnv->pstAppCtx,pstEnv,  pstPlayer, pstItemDef->ItemID, 1, 
				LIST_TYPE_SUNDRIES, &iPos, 0, 0);

	pstSiol->One[i].ChengshuTime = pstEnv->pstAppCtx->stCurr.tv_sec+pstItemDef->Result[0].ResultVal1*60;
	pstSiol->One[i].MonID = pstItemDef->Result[0].ResultVal2;
	pstSiol->One[i].MonMemID = iMemID;

	home_farm_notify(pstEnv, pstPlayer, pstHome,pstMInst);
	return 0;
}

int home_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
					TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	HomeObj *pstObj = pstEnv->pstHomeObj;
	CSHOMECLT *pstClt = &pstCsPkg->Body.HomeClt;
	
	if (INST_MODE_ZONE_SVR != pstEnv->iInstMode)
	{
		return 0;
	}

	if (pstObj->stObjHomeInfo.iStat != HOME_STAT_FINI)
	{
		return 0;
	}

	switch(pstClt->Cmd)
	{
	case HOME_CLT_OP_NEW:
		return home_new(pstEnv, pstPlayer, &pstClt->Data.New);
		break;
	case HOME_CLT_OP_PAGE:
		return home_page( pstEnv, pstPlayer, pstClt->Data.Page.Idx);
		break;
	case HOME_CLT_OP_SET_NAME:
		return home_set_name(pstEnv, pstPlayer, &pstClt->Data.SetName);
		break;
	case HOME_CLT_OP_SET_STAT:
		return home_set_stat(pstEnv, pstPlayer, &pstClt->Data.SetStat);
		break;
	case HOME_CLT_OP_ENTER:
		return home_enter(pstEnv, pstPlayer, &pstClt->Data.Enter);
		break;
	case HOME_CLT_OP_MSG_BOARD:
		return home_msg(pstEnv, pstPlayer, &pstClt->Data.Msg);
		break;
	case HOME_CLT_OP_DESTORY:
		return home_destory(pstEnv,pstPlayer, &pstClt->Data.Destory);
		break;
	case HOME_CLT_OP_SALE:
		return home_sale(pstEnv, pstPlayer,  &pstClt->Data.Sale);
		break;
	case HOME_CLT_OP_BUY:
		return home_buy( pstEnv, pstPlayer,  &pstClt->Data.Buy);
		break;
	case HOME_CLT_OP_PET:
		return home_pet( pstEnv,  pstPlayer, &pstClt->Data.Pet);
		break;
	case HOME_CLT_OP_JIAJU_PUT:
		return home_put(pstEnv, pstPlayer, &pstClt->Data.Put);
		break;
	case HOME_CLT_OP_ZHONGZHI:
		return home_zhongzhi( pstEnv, pstPlayer, &pstClt->Data.Zhongzhi);
		break;
	case HOME_CLT_OP_JIAJU_ADJUST:
		return home_adjust_jiaju(pstEnv, pstPlayer, &pstClt->Data.JiaJuAdjust);
		break;
	default:
		return 0;
		break;
	}


}

//返回菜地等级
int home_get_soil_level(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZoneHome *pstZoneHome;
	HOME *pstHome;
	HOMESIOL *pstSiol;
	HOMEFARMDEF *pstDef;
	
	pstZoneHome = zonehome_one_find_by_id( pstEnv, pstPlayer->stRoleData.MiscInfo.Home.ID);
	if (!pstZoneHome)
	{
		return -1;
	}

	pstHome = &pstZoneHome->stHome;
	pstSiol = &pstHome->Info.Siol;

	pstDef = home_farm_def_find( pstEnv, pstSiol->Lvl);
	if (!pstDef)
		return -1;

	return pstSiol->Lvl;
}

//已开垦菜地的数量
int home_player_get_kaiken_num(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZoneHome *pstZoneHome;
	HOME *pstHome;
	
	pstZoneHome = zonehome_one_find_by_id( pstEnv, pstPlayer->stRoleData.MiscInfo.Home.ID);
	if (!pstZoneHome)
	{
		return -1;
	}

	pstHome = &pstZoneHome->stHome;

	return home_get_kaiken_num(pstHome);
}

//获取下次开垦的信息
int home_player_get_next_kaiken_info(ZONESVRENV* pstEnv, Player *pstPlayer,int iType)
{
	ZoneHome *pstZoneHome;
	HOME *pstHome;
	HOMESOILDEF *pstSoilDef;
	int iNum = 0;
	
	pstZoneHome = zonehome_one_find_by_id( pstEnv, pstPlayer->stRoleData.MiscInfo.Home.ID);
	if (!pstZoneHome)
		return -1;

	pstHome = &pstZoneHome->stHome;
	iNum = home_get_kaiken_num(pstHome);
	pstSoilDef = home_soil_def_find(pstEnv, iNum+1);
	if(!pstSoilDef)
		return -2;
		
	if(iType == 1)
	{
		return pstSoilDef->MoneyNum;
	}
	else if(iType == 2)
	{
		return pstSoilDef->MoneyType;
	}
	else if(iType == 3)
	{
		return pstSoilDef->VegFieldLevel;
	}
	else
	{
		return -1;
	}

	return 0;
}

HOMESOILDEF *home_soil_def_find(ZONESVRENV* pstEnv, int iNum)
{
	HOMESOILDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Num = iNum;
	
	iIndex = bsearch_int(&stDef, pstObj->sHomeSoilDef, pstObj->iHomeSoilDef, HOME_SOIL_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (HOMESOILDEF *)&pstObj->sHomeSoilDef[iIndex];
	}

	return NULL;
}

//调整家具
int home_adjust_jiaju(ZONESVRENV* pstEnv, Player *pstPlayer,  HOMEJIAJUADJUST *pstJiaJuAdjust)
{
	ZoneHome *pstZoneHome;
	HOME *pstHome;
	ZONEPWORLD *pstZonePworld;
	ZONEPWORLDINST *pstPInst;
	HOMELVLDEF *pstDef;
	ZONEMAPINST *pstMInst;
	int i;
	HOMEJIAJUONE *pstOne;      

	pstZoneHome = zonehome_one_find_by_id(pstEnv,pstPlayer->stRoleData.MiscInfo.Home.ID);
	if (!pstZoneHome)
		return -1;

	pstHome = &pstZoneHome->stHome;

	if (pstHome->RoleID != pstPlayer->stRoleData.RoleID)
	{
		return -1;
	}

	pstZonePworld = z_find_pworld2(pstEnv, pstPlayer->stRoleData.Map);
	if (NULL == pstZonePworld)
	{
		return -1;
	}

	pstPInst = z_find_pworld_inst(pstEnv->pstMapObj, pstHome->Info.PworldinstID);
	if(NULL == pstPInst)
	{
		return -1;
	}

	pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv,  &pstPInst->astMapInstIdx[0]);
	if (!pstMInst)
		return -1;

	pstDef = home_lvl_def_find(pstEnv, pstHome->Lvl);
	if (!pstDef)
	{
		return -1;
	}

	if (! home_point_is_in_rect(&pstJiaJuAdjust->New.Pos, &pstDef->HousePos))
	{
		//return -1;
	}

	for(i=0;i<pstHome->Info.House.Num;i++)
	{
		pstOne = &pstHome->Info.House.One[i];
		if(pstOne && pstOne->MonID == pstJiaJuAdjust->Old.MonID 
			&& pstJiaJuAdjust->Old.Dir == pstOne->Dir 
			&& pstJiaJuAdjust->Old.Pos.X == pstOne->Pos.X 
			&& pstJiaJuAdjust->Old.Pos.Y == pstOne->Pos.Y)
		{
			pstOne->MonID = pstJiaJuAdjust->New.MonID;
			pstOne->Dir = pstJiaJuAdjust->New.Dir;
			pstOne->Pos = pstJiaJuAdjust->New.Pos;
			break;
		}
	}
	
	home_house_notify( pstEnv, pstPlayer,pstHome);
	return 0;
}


//#endif


