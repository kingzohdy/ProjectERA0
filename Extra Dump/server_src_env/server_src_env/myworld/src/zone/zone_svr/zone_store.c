

#include "zone_store.h"
#include "zone_db.h"
#include "role_db_meta.h"
#include "zone_shop.h"
#include "zone_clt.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "zone_mail.h"
#include "zone_package.h"
#include "zone_arm.h"
#include "libdirty.h"
#include "zone_err.h"
#include "zone_oplog.h"
#include "zone_vip.h"

#define MAX_STORE_GRID 40
#define STORE_GRID_INIT 8
int store_db_get(ZONESVRENV *pstEnv);
int store_reidx_hash(ZONESVRENV* pstEnv, int iIdx);
int store_calc_rent(ZONESVRENV *pstEnv, int iMax);

void store_tip1(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	if (pstPlayer)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_SVR, ZONEERR_STORE301);
	}	
}

void store_tip2(ZONESVRENV *pstEnv,Player *pstPlayer, ZoneStore*pstZoneStore)
{
	int iHour = 0;
	int iSec = 0;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;

	iHour = (pstZoneStore->stStore.StoreInfo.CloseTime - iCurr)/(60*60);
	if (iHour < 0)
	{
		iHour = 0;
	}
		
	iSec = ((pstZoneStore->stStore.StoreInfo.CloseTime - iCurr)%(60*60))/60;
	if (iSec < 0)
	{
		iSec = 0;
	}
	
	z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_SVR, 
			ZONEERR_STORE302,
			iHour, iSec);	
}

int store_db_count_req(ZONESVRENV *pstEnv)
{
	DBAsyn stDBAsyn;
	STORE stStore;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;

	stDBAsyn.iCmd = DB_STORE_COUNT;
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head,
	                 TORM_SELECT_COUNT_REQ, "Store", -1, &stDBAsyn);
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);

	memset(&stStore, 0, sizeof(stStore));
	stMetaData.pszBuff = (char *)&stStore;
	stMetaData.iBuff = sizeof(stStore);
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


int store_hash_cmp(const void* pv1 , const void* pv2)
{
	StoreHash *pstHash1 = (StoreHash *)pv1;
	StoreHash *pstHash2 = (StoreHash *)pv2;

	return pstHash1->uiID - pstHash2->uiID;
}


int store_cmp(const void* pv1 , const void* pv2)
{
	ZoneStore *pstP1 = (ZoneStore *)pv1;
	ZoneStore *pstP2 = (ZoneStore *)pv2;

	if (pstP1->stStore.StoreInfo.Tax != pstP2->stStore.StoreInfo.Tax)
	{
		return pstP2->stStore.StoreInfo.Tax - pstP1->stStore.StoreInfo.Tax;
	}
	else
	{
		return pstP2->stStore.StoreInfo.Exchg - pstP1->stStore.StoreInfo.Exchg;	
	}
}

int store_data_qsort(StoreObj *pstObj)
{
	if (pstObj->iStoreNum > 1)
	{
		qsort(pstObj->astZoneStore, pstObj->iStoreNum,
		      sizeof(pstObj->astZoneStore[0]), store_cmp);	
	}
	return 0;
}


unsigned int store_hash_code(const void* pvData)
{
	StoreHash *pstHash = (StoreHash *)pvData;
	
	return pstHash->uiID;
}

ZoneStore* store_id_get(ZONESVRENV *pstEnv, int  iId)
{
	ZoneStore* pstZoneStore = NULL;
	StoreHash stHash, *pstHash = NULL;

	stHash.uiID = iId;
	pstHash = sht_find(pstEnv->pstShtStore, &stHash, store_hash_cmp,
	                       store_hash_code);
	if (NULL == pstHash)
	{
		return NULL;
	}

	if (0 > pstHash->iIdx &&
		pstHash->iIdx >= pstEnv->pstStoreObj->iStoreNum)
	{
		return NULL;
	}

	pstZoneStore = &pstEnv->pstStoreObj->astZoneStore[pstHash->iIdx];
	if (pstZoneStore->stStore.RoleID != pstHash->RoleID)
	{
		return NULL;
	}

	return pstZoneStore;
}



int store_init_fini(ZONESVRENV *pstEnv)
{
	int i;
	StoreObj *pstObj = pstEnv->pstStoreObj;
	ZoneStore *pstStore;
	StoreHash stHash;
	StoreHash *pstHash;

	store_data_qsort(pstObj);

	for(i = 0; i < pstObj->iStoreNum; ++i)
	{
		pstStore = &pstObj->astZoneStore[i];
		
		stHash.uiID = pstStore->stStore.StoreID;
		pstHash = sht_insert_unique(pstEnv->pstShtStore, &stHash, 
		                                store_hash_cmp, store_hash_code);
		if (pstHash == NULL)
		{
			return -1;
		}

		pstHash->iIdx = i;
		pstHash->uiID = pstStore->stStore.StoreID;
		pstHash->RoleID = pstStore->stStore.RoleID;
	}

	pstObj->stObjStoreInfo.iStat = STORE_STAT_FINI;
	pstObj->stObjStoreInfo.iQsortTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	return 0;
}


int IsSameHour(time_t t1, time_t t2)
{
	struct tm curr1;
	struct tm curr2;

	//凌晨6点切换天
	t1 -= 6*3600;
	t2 -= 6*3600;
	
	localtime_r(&t1, &curr1);
	localtime_r(&t2, &curr2);

	if( curr1.tm_year==curr2.tm_year && curr1.tm_yday==curr2.tm_yday && curr1.tm_hour == curr2.tm_hour)
		return 1;

	return 0;
}

int store_qsort_by_time(ZONESVRENV *pstEnv)
{
	StoreObj *pstObj = pstEnv->pstStoreObj;
	
	store_data_qsort(pstObj);

	store_reidx_hash(pstEnv, 0);
	return 0;
}

int store_select_res(ZONESVRENV *pstEnv, TORMSELECTRES *pstSelectRes)
{
	StoreObj *pstObj = pstEnv->pstStoreObj;
	int i;
	ZoneStore *pstZoneStore;
	
	if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			       "store_select_res. dberr = %d, msg = %s.", 
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
		if (pstObj->iStoreNum >= MAX_STORE_NUM)
		{
			break;
		}	

		pstZoneStore = &pstObj->astZoneStore[pstObj->iStoreNum];
		bzero( pstZoneStore, sizeof(*pstZoneStore));
		
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&pstZoneStore->stStore,
		                            sizeof(pstZoneStore->stStore)))
		{
			goto error;
		}

		pstObj->iStoreNum ++;
	}

	if (pstObj->iStoreNum <pstObj->stObjStoreInfo.iDBCount &&
		pstObj->iStoreNum < MAX_STORE_NUM)
	{
		store_db_get(pstEnv);
	}
	else
	{
		 store_init_fini(pstEnv);	
	}
	return 0;

error:
	pstObj->stObjStoreInfo.iStat = STORE_STAT_FAIL;
	return 0;
}

int store_db_get(ZONESVRENV *pstEnv)
{
	STORE stStore;
	DBAsyn stDBAsyn;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	StoreObj *pstObj = pstEnv->pstStoreObj;

	stDBAsyn.iCmd = DB_STORE_SELECT;
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ,
	                 "Store", -1, &stDBAsyn);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OffSet = pstObj->iStoreNum;
	pstSelectReq->Limit = STORE_PAGE_MAX;
	strcpy(pstSelectReq->OperInfo.WhereDef, "where StoreID != 0");

	memset(&stStore, 0, sizeof(stStore));

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stStore;
	stMetaData.iBuff = sizeof(stStore);
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
	pstObj->stObjStoreInfo.iStat = STORE_STAT_FAIL;
	return 0;
}

int store_db_count_res(ZONESVRENV *pstEnv, TORMSELECTCOUNTRES *pstSelectCountRes)
{
	StoreObj *pstObj = pstEnv->pstStoreObj;

	if (TORM_ERR_IS_ERROR(pstSelectCountRes->ErrNo))
	{
		goto error;
	}

	pstObj->stObjStoreInfo.iDBCount = pstSelectCountRes->RowsCount;

	if (0 == pstObj->stObjStoreInfo.iDBCount)
	{
		pstObj->stObjStoreInfo.iStat = STORE_STAT_FINI;
	}
	else
	{
		store_db_get(pstEnv);
	}
	return 0;

error:
	pstObj->stObjStoreInfo.iStat = STORE_STAT_FAIL;
	return 0;
}


int store_init(ZONESVRENV *pstEnv)
{
	StoreObj *pstObj = pstEnv->pstStoreObj;
	if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
	{
		if (pstObj->stObjStoreInfo.iStat == STORE_STAT_FINI ||pstObj->stObjStoreInfo.iStat == STORE_STAT_REQ)
		{
			return 0;
		}

		pstEnv->pstStoreObj->iStoreNum = 0;
		pstObj->stObjStoreInfo.iStat = STORE_STAT_REQ;
		if (store_db_count_req(pstEnv) < 0)
		{
			pstObj->stObjStoreInfo.iStat  = STORE_STAT_FAIL;
			return -1;
		}
	}
	return 0;
}

ZoneStore *store_player_get(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZoneStore *pstStore;
	if (pstPlayer->stRoleData.MiscInfo.Store.ID <= 0)
	{
		return NULL;
	}

	pstStore = store_id_get(pstEnv, pstPlayer->stRoleData.MiscInfo.Store.ID);
	if (!pstStore || pstStore->stStore.RoleID != pstPlayer->stRoleData.RoleID)
	{
		return NULL;	
	}

	return pstStore;
}


int store_db_insert_req( ZONESVRENV* pstEnv,Player *pstPlayer, STORE *pstStore)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMINSERTREQ *pstInsertReq =  &pstPkg->Body.InsertReq;
	DBInsertBack stBack;

	stBack.RoleID = pstPlayer->stRoleData.RoleID;
	strncpy(stBack.StoreName, pstStore->StoreInfo.StoreName, sizeof(stBack.StoreName));
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,pstPlayer->stRoleData.Uin,&pstPkg->Head, 
								TORM_INSERT_REQ,DB_STORE_INSERT_RES, 
			               			"Store", 
			               			(char*)&stBack, 
			               			sizeof(stBack));
	
	pstInsertReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstInsertReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstInsertReq->OperInfo.WhereDef[0] = 0;
	
	iDst = pstEnv->iMiscDBID;
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,pstStore, 
			sizeof(*pstStore),&iDst,pstEnv->iRoleDBProtoVer);
	
}


int store_db_delete_req( ZONESVRENV* pstEnv, STORE *pstStore)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMDELETEREQ *pstReq =  &pstPkg->Body.DeleteReq;
	DBInsertBack stBack;

	stBack.iID = pstStore->StoreID;
	strncpy(stBack.StoreName, pstStore->StoreInfo.StoreName, sizeof(stBack.StoreName));
	strncpy(stBack.RoleName, pstStore->Name, sizeof(stBack.RoleName));
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,0,&pstPkg->Head, 
								TORM_DELETE_REQ,DB_STORE_DELETE_RES, 
			               			"Store", 
			               			(char*)&stBack, 
			               			sizeof(stBack));
	
	pstReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstReq->OperInfo.EntryNameList.EntryNameNum = 0;

	snprintf(pstReq->OperInfo.WhereDef,sizeof(pstReq->OperInfo.WhereDef),"where StoreID = %d",pstStore->StoreID);
	
	iDst = pstEnv->iMiscDBID;
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,pstStore, 
			sizeof(*pstStore),&iDst,pstEnv->iRoleDBProtoVer);
	
}

int store_open_notify(ZONESVRENV* pstEnv, Player *pstPlayer, int iID, int iSucc, const char *StoreName)
{
	CSPKG stPkg;
	CSSTORESVR *pstSvr = &stPkg.Body.StoreSvr;

	pstSvr->Cmd = STORE_SVR_OP_OPEN;
	pstSvr->Data.Open.ID = iID;
	pstSvr->Data.Open.Succ = iSucc;
	pstSvr->Data.Open.StoreName[0] = 0;
	if (StoreName && StoreName[0] != 0)
	{
		strncpy(pstSvr->Data.Open.StoreName,StoreName, sizeof(pstSvr->Data.Open.StoreName) );
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, STORE_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int store_money_chg_notify(ZONESVRENV* pstEnv, Player *pstPlayer, int iID, int iMoney)
{
	CSPKG stPkg;
	CSSTORESVR *pstSvr = &stPkg.Body.StoreSvr;

	pstSvr->Cmd = STORE_SVR_OP_MONEY_CHG;
	pstSvr->Data.MoneyChg.ID = iID;
	pstSvr->Data.MoneyChg.Money = iMoney;
	
	Z_CSHEAD_INIT(&stPkg.Head, STORE_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int store_pos_notify(ZONESVRENV* pstEnv, Player *pstPlayer, int iPos, int iID, unsigned int uiExchg)
{
	CSPKG stPkg;
	CSSTORESVR *pstSvr = &stPkg.Body.StoreSvr;

	pstSvr->Cmd = STORE_SVR_OP_POS;
	pstSvr->Data.Pos.Pos = iPos;
	pstSvr->Data.Pos.ID = iID;
	pstSvr->Data.Pos.Exchg = uiExchg;

	Z_CSHEAD_INIT(&stPkg.Head, STORE_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}



int store_item_chg_notify(ZONESVRENV* pstEnv, Player *pstPlayer,  STOREGRID *pstStoreGrid, int iID)
{
	CSPKG stPkg;
	CSSTORESVR *pstSvr = &stPkg.Body.StoreSvr;

	pstSvr->Cmd = STORE_SVR_OP_ITEM_CHG;
	pstSvr->Data.ItemChg.ID = iID;
	pstSvr->Data.ItemChg.StoreGrid = *pstStoreGrid;
	
	Z_CSHEAD_INIT(&stPkg.Head, STORE_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int store_close_notify(ZONESVRENV* pstEnv, Player *pstPlayer,  int iID)
{
	CSPKG stPkg;
	CSSTORESVR *pstSvr = &stPkg.Body.StoreSvr;

	pstSvr->Cmd = STORE_SVR_OP_CLOSE;
	pstSvr->Data.Close.ID = iID;
	
	Z_CSHEAD_INIT(&stPkg.Head, STORE_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int store_page_fill(STORE *pstStore, STOREPAGEONE *pstPageOne)
{
	pstPageOne->ID = pstStore->StoreID;
	strncpy(pstPageOne->RoleName , pstStore->Name, NAME_LEN);
	strncpy(pstPageOne->StoreName, pstStore->StoreInfo.StoreName, NAME_LEN);
	return 0;
}

int store_page_notify(ZONESVRENV* pstEnv, Player *pstPlayer,  int iPage, int iCount)
{
	CSPKG stPkg;
	CSSTORESVR *pstSvr = &stPkg.Body.StoreSvr;
	StoreObj *pstObj = pstEnv->pstStoreObj;
	int i;
	STORE *pstStore;

	pstSvr->Cmd = STORE_SVR_OP_PAGE;
	pstSvr->Data.Page.Page = iPage;
	pstSvr->Data.Page.PageCount = iCount;
	pstSvr->Data.Page.Num = 0;
	for (i=iPage*STORE_PAGE_MAX; i<pstObj->iStoreNum; i++)
	{
		pstStore = &pstEnv->pstStoreObj->astZoneStore[i].stStore;
		store_page_fill(pstStore, &pstSvr->Data.Page.PageOne[pstSvr->Data.Page.Num]);
		pstSvr->Data.Page.Num ++;

		if (pstSvr->Data.Page.Num >= STORE_PAGE_MAX)
		{
			break;
		}
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, STORE_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int stroe_open_ui_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	StoreObj *pstObj;

	pstObj = pstEnv->pstStoreObj;
	
	if (INST_MODE_ZONE_SVR != pstEnv->iInstMode)
	{
		return -1;
	}

	if (pstObj->stObjStoreInfo.iStat != STORE_STAT_FINI)
	{
		return -1;
	}

	if (pstPlayer->stOnline.State & CS_STAT_DEAD)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STORE3);
		return -1;
	}

	if (pstPlayer->stOnline.State & CS_STAT_FIGHT)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STORE4);
		return -1;
	}

	if (pstPlayer->stOnline.State & CS_STAT_STALL)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STORE5);
		return -1;
	}

	if (pstPlayer->stOnline.stExchgOnline.iMemID > 0)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STORE6);
		return -1;
	}

	if (pstPlayer->stOnline.stQiecuoInfo.iMemID > 0)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STORE7);
		return -1;
	}
	return 0;
}

int store_open_ui_notify(ZONESVRENV* pstEnv, Player *pstPlayer,  int iPage, int iCount, int iNpcID, STORE *pstStore)
{
	CSPKG stPkg;
	CSSTORESVR *pstSvr = &stPkg.Body.StoreSvr;
	StoreObj *pstObj = pstEnv->pstStoreObj;
	int i;
	STORE *pstStore1;

	pstSvr->Cmd = STORE_SVR_OP_OPEN_UI;

	memset(&pstSvr->Data.OpenUI, 0, sizeof(pstSvr->Data.OpenUI));
	pstSvr->Data.OpenUI.NpcID = iNpcID;
	pstSvr->Data.OpenUI.StorePageSvr.Page = iPage;
	pstSvr->Data.OpenUI.StorePageSvr.PageCount = iCount;
	if (pstStore)
	{
		pstSvr->Data.OpenUI.Store = *pstStore;
	}
	
	for (i=iPage*STORE_PAGE_MAX; i<pstObj->iStoreNum; i++)
	{
		pstStore1 = &pstEnv->pstStoreObj->astZoneStore[i].stStore;
		store_page_fill(pstStore1, &pstSvr->Data.OpenUI.StorePageSvr.PageOne[pstSvr->Data.OpenUI.StorePageSvr.Num]);
		pstSvr->Data.OpenUI.StorePageSvr.Num ++;

		if (pstSvr->Data.OpenUI.StorePageSvr.Num >= STORE_PAGE_MAX)
		{
			break;
		}
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, STORE_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int store_info_notify(ZONESVRENV* pstEnv, Player *pstPlayer,  ZoneStore *pstZoneStore)
{
	CSPKG stPkg;
	CSSTORESVR *pstSvr = &stPkg.Body.StoreSvr;

	pstSvr->Cmd = STORE_SVR_OP_ONE_INFO;
	pstSvr->Data.One.Store = pstZoneStore->stStore;
	pstSvr->Data.One.Self = (pstPlayer->stRoleData.MiscInfo.Store.ID == (int)pstZoneStore->stStore.StoreID?1:0);
	
	Z_CSHEAD_INIT(&stPkg.Head, STORE_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int store_grid_chg_notify(ZONESVRENV* pstEnv, Player *pstPlayer,  ZoneStore *pstZoneStore)
{
	CSPKG stPkg;
	CSSTORESVR *pstSvr = &stPkg.Body.StoreSvr;

	pstSvr->Cmd = STORE_SVR_OP_GRID_CHG;
	pstSvr->Data.GridChg.ID = pstZoneStore->stStore.StoreID;
	pstSvr->Data.GridChg.Max = pstZoneStore->stStore.StoreInfo.MaxNum;
	
	Z_CSHEAD_INIT(&stPkg.Head, STORE_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}





int store_reidx_hash(ZONESVRENV* pstEnv, int iIdx)
{
	int i;
	StoreHash stHash;
	StoreHash *pstHash;
	ZoneStore *pstZoneStore;
	
	StoreObj *pstStoreObj = pstEnv->pstStoreObj;

	for(i = iIdx; i < pstStoreObj->iStoreNum; ++i)
	{
		pstZoneStore = &pstStoreObj->astZoneStore[i];

		stHash.uiID = pstZoneStore->stStore.StoreID;
		pstHash = sht_find(pstEnv->pstShtStore, &stHash, store_hash_cmp, store_hash_code);
		if (!pstHash)
		{
			// log
			continue;
		}
		pstHash->iIdx = i;
	}
	
	return 0;
}

int store_player_clear_id(Player *pstPlayer)
{
	pstPlayer->stRoleData.MiscInfo.Store.ID = 0;
	return 0;
}

int store_db_delete_res(ZONESVRENV* pstEnv, DBAsyn *pstDBAsyn, TORMDELETERES *pstRes)
{
	DBInsertBack *pstBack;
	ZoneStore *pstZoneStore;
	int iIdx;
	StoreObj *pstStoreObj = pstEnv->pstStoreObj;
	StoreHash stHash;
	Player *pstOwner;
	ROLEGRID stGrid[MAX_STORE_GRID];
	int iNum = 0;;
	int i;
	int iMail = 0;
	int iMoney = 0;

	pstBack = (DBInsertBack *)(pstDBAsyn->AsynCmd.szMaxBuf);
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

	pstZoneStore = store_id_get(pstEnv, pstBack->iID);
	if (!pstZoneStore)
	{
		// todo log
		return -1;
	}

	for (i=0; i<pstZoneStore->stStore.StoreInfo.Num; i++)
	{
		stGrid[iNum] = pstZoneStore->stStore.StoreInfo.Grid[i].Grid;
		iNum ++;
	}	

	iMoney = pstZoneStore->stStore.StoreInfo.Money;
	if (iMoney < 0)
	{
		iMoney = 0;
	}
	
	if (iNum > 0 || iMoney > 0)
	{
		if (pstOwner)
		{
			if (iNum > 0)
			{
				if (package_add_test(pstEnv->pstAppCtx, pstEnv,  pstOwner, &stGrid[0], iNum, 0) < 0)
				{
					iMail = 1;
				}
				else
				{
					package_add(pstEnv->pstAppCtx, pstEnv,  pstOwner, &stGrid[0], iNum, 0, 0);
				}
			}

			if (!iMail && iMoney > 0)
			{
				if (package_add_money_test(pstEnv, pstOwner, ATTR_ID_MONEY, iMoney)< 0)
				{
					iMail  =1;
				}
				else
				{
					package_add_money(pstEnv, pstOwner, ATTR_ID_MONEY, iMoney);	
				}
			}
		}
		else
		{
			iMail = 1;
		}
	}

	if (iMail)
	{
		player_sysmail_fast_send(pstEnv,pstBack->RoleName, "商铺关闭",
										"由于您不在线或者背包空间不足,商铺物品通过邮件返还!",
										&stGrid[0], iNum, iMoney,NULL, 0);	
	}
	
	
	iIdx = pstZoneStore - &pstStoreObj->astZoneStore[0];
		
	// 删除哈希
	stHash.uiID = pstBack->iID;
	sht_remove(pstEnv->pstShtStore, &stHash, store_hash_cmp, store_hash_code);

	// 删除数据
	--pstStoreObj->iStoreNum;
	if (iIdx != pstStoreObj->iStoreNum)
	{
		memmove(pstZoneStore, pstZoneStore + 1,
		        (pstStoreObj->iStoreNum - iIdx) * sizeof(*pstZoneStore));
	}

	//整理hash
	store_reidx_hash( pstEnv, iIdx);

	// 通知店铺主人
	
	if (pstOwner)
	{
		store_player_clear_id(pstOwner);
		store_close_notify(pstEnv, pstOwner, pstBack->iID);	
	}
error:
	
	return 0;
}

int store_db_insert_res(ZONESVRENV* pstEnv, Player *pstPlayer, DBAsyn *pstDBAsyn, TORMINSERTRES *pstInsertRes)
{
	DBInsertBack *pstBack;
	int iUseMoney;
	StoreHash stHash, *pstHash=NULL;
	int i;
	StoreObj *pstObj = pstEnv->pstStoreObj;
	int iReIdx = 0;

	pstBack = (DBInsertBack *)(pstDBAsyn->AsynCmd.szMaxBuf);
	//pstEnv->pstStoreObj->stObjStoreInfo.iInsertLock = 0;
	//pstEnv->pstStoreObj->stObjStoreInfo.iInsertTime = 0;
	
	if (TORM_ERR_IS_ERROR(pstInsertRes->ErrNo))
	{
		goto error;
	}

	for (i=0; i<pstObj->iStoreNum; i++)
	{
		if (pstObj->astZoneStore[i].stStore.RoleID == pstBack->RoleID)
		{
			pstObj->astZoneStore[i].stStore.StoreID = pstInsertRes->LastInsertID;
			break;
		}
	}

	if (i>=pstObj->iStoreNum)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "store_db_insert_res error 1" );
		return -1;
	}

	stHash.uiID = pstInsertRes->LastInsertID;
	pstHash = sht_insert_unique(pstEnv->pstShtStore, &stHash, store_hash_cmp, store_hash_code);
	if(pstHash)
	{
		pstHash->iIdx = i;
		pstHash->RoleID = pstBack->RoleID;
		pstHash->uiID = pstInsertRes->LastInsertID;		
	}
	else
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "store_db_insert_res error 2" );
		return -1;
	}

	// 开店成功
	if ( !pstPlayer ||
		pstPlayer->stRoleData.RoleID != pstBack->RoleID ||
		PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
	{
		// 这种情况 角色登录的时候检查一下
		return -1;
	}
	
	pstPlayer->stRoleData.MiscInfo.Store.ID = pstInsertRes->LastInsertID;
	store_open_notify(pstEnv, pstPlayer, pstInsertRes->LastInsertID, 0, pstBack->StoreName);
	store_info_notify(pstEnv, pstPlayer, &pstObj->astZoneStore[i]);
	return 0;
error:

	iUseMoney = store_calc_rent(pstEnv, STORE_GRID_INIT); 
	if (iUseMoney > 0)
	{
		player_sysmail_fast_send(pstEnv, pstBack->RoleName, "开店失败,银币返还!", 
				"您好，系统内部故障，开店失败，银币通过此邮件返还!", NULL, 0,
				iUseMoney, LJY_MAIL43, 0);
	}
		
	for (i = pstObj->iStoreNum-1; i>=0; i--)
	{
		if (pstObj->astZoneStore[i].stStore.RoleID == pstBack->RoleID)
		{
			pstObj->iStoreNum -- ;
			if (i != pstObj->iStoreNum)
			{
				pstObj->astZoneStore[i] = pstObj->astZoneStore[pstObj->iStoreNum];
				iReIdx = 1;
			}
			break;
		}
	}

	// 整理下哈希,以防万一
	if (iReIdx)
	{
		store_reidx_hash(pstEnv,i);	
	}

	store_open_notify(pstEnv, pstPlayer, 0, 1, NULL);
	return -1;
}

int store_open(ZONESVRENV* pstEnv, Player *pstPlayer, STOREOPEN *pstOpen)
{
	ZoneStore *pstZoneStore;
	StoreObj *pstObj = pstEnv->pstStoreObj;
	int iUseMoney;
	STORE stStore;
	int i;

	// 插入锁
	//if (pstObj->stObjStoreInfo.iInsertLock)
	//{
	//	return -1;
	//}
	
	if (pstObj->iStoreNum >= MAX_STORE_NUM)
	{
		return -1;
	}
	
	pstZoneStore = store_player_get( pstEnv,  pstPlayer);
	if (pstZoneStore)
	{
		return -1;
	}

	if (pstOpen->StoreName[0] == '\0')
	{
		return -1;
	}

	if ((CheckDirty(pstOpen->StoreName , 0x1f) & 0x1f ) )
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIRTY);
		return -1;
	}

	for(i = 0; i < pstObj->iStoreNum; ++i)
	{
		if (strcmp(pstObj->astZoneStore[i].stStore.StoreInfo.StoreName, pstOpen->StoreName) == 0)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STORE2);
			return -1;
		}
	}

	iUseMoney = store_calc_rent(pstEnv, STORE_GRID_INIT); 
	
	if (package_dec_money_test( pstEnv, pstPlayer, ATTR_ID_MONEY, iUseMoney ) < 0)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STORE1);
		return -1;
	}

	// 加锁,不允许同一时刻插入多条记录
	// 例如有499个商铺的时候，有10个玩家申请开店铺,有可能数据库会插入多条
	//pstObj->stObjStoreInfo.iInsertLock = 1; 
	//pstObj->stObjStoreInfo.iInsertTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	package_dec_money( pstEnv, pstPlayer, ATTR_ID_MONEY, iUseMoney);

	z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_STORE_OPEN, 
				0, 0, iUseMoney, "创建商铺");

	memset(&stStore, 0, sizeof(stStore));
	stStore.RoleID = pstPlayer->stRoleData.RoleID;
	strncpy(stStore.Name,pstPlayer->stRoleData.RoleName,sizeof(stStore.Name));
	strncpy(stStore.StoreInfo.StoreName,pstOpen->StoreName,sizeof(stStore.StoreInfo.StoreName));
	stStore.StoreInfo.OpenTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	stStore.StoreInfo.MaxNum = STORE_GRID_INIT;

	if (store_db_insert_req( pstEnv, pstPlayer, &stStore) < 0)
	{
		return -1;
	}

	// 先占位，防止N个玩家同时插入数据库
	pstObj->astZoneStore[pstObj->iStoreNum].stStore = stStore;
	pstObj->iStoreNum++;

	ZoneStore stZoneStore;
	stZoneStore.stStore = stStore;
	z_store_buy_oplog(pstEnv,pstPlayer, &stZoneStore, NULL, iUseMoney, 0, 0, 0);
	return 0;
}

int store_get_free_grid(STOREGRID *pstStoreGrid, int iNum, int iMaxNum)
{
	int i = 0;
	STOREGRID *pstGrid = NULL;
	int iGrid = 0;
	char sGrid[MAX_STORE_GRID];

	assert(iMaxNum <= MAX_STORE_GRID);

	memset(sGrid, 0, iMaxNum);
	for(i=0; i<iNum; i++)
	{
		pstGrid = pstStoreGrid+i;
		iGrid = pstGrid->Grid.GridData.RoleItem.GridIdx;
		
		if (iGrid >= 0 && iGrid < iMaxNum)
		{
			sGrid[iGrid] = 1;
		}
	}

	for (i=0; i<iMaxNum; i++)
	{
		if (!sGrid[i])
			return i;
	}

	return -1;
}


int store_item_up(ZONESVRENV* pstEnv, Player *pstPlayer, STOREUP*pstUp)
{
	ZoneStore *pstZoneStore;
	int iPos;
	ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
	ROLEGRID *pstSrcGrid;
	int iFree;
	STOREINFO *pstStoreInfo;
	ITEMDEF *pstItemDef;

	pstZoneStore = store_player_get( pstEnv,  pstPlayer);
	if (!pstZoneStore )
	{
		return -1;
	}

	pstStoreInfo = &pstZoneStore->stStore.StoreInfo;
	if (pstStoreInfo->Num >= pstStoreInfo->MaxNum)
	{
		return -1;
	}

	iPos = package_grid_pos(pstPak->SundriesPackage.RoleGrids, 
							pstPak->SundriesPackage.Num, pstUp->GridIdx);
	if (0 > iPos)
	{
		return -1;
	}

	pstSrcGrid = pstPak->SundriesPackage.RoleGrids+iPos;
	if (pstSrcGrid->GridData.RoleItem.InstFlag & INST_ITEM_BIND)
	{
		return -1;
	}
	
	if (pstSrcGrid->GridData.RoleItem.ValidEnd > 0 && 
		pstSrcGrid->GridData.RoleItem.ValidEnd < pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return -1;
	}

	if (pstUp->Num > pstSrcGrid->GridData.RoleItem.Num)
	{
		return -1;
	}

	iFree = store_get_free_grid(pstStoreInfo->Grid, pstStoreInfo->Num, pstStoreInfo->MaxNum);
	if (iFree < 0)
		return -1;

	pstItemDef = z_find_itemdef( pstEnv,pstSrcGrid->GridData.RoleItem.DefID);
	if (!pstItemDef)
		return -1;

	pstStoreInfo->Grid[pstStoreInfo->Num].Grid = *pstSrcGrid;
	pstStoreInfo->Grid[pstStoreInfo->Num].Grid.GridData.RoleItem.GridIdx = iFree;
	pstStoreInfo->Grid[pstStoreInfo->Num].Grid.GridData.RoleItem.Num = pstUp->Num;
	if (pstItemDef->MaxPile !=0 && pstItemDef->MaxPile !=1)
	{
		pstStoreInfo->Grid[pstStoreInfo->Num].Grid.GridData.RoleItem.WID = z_get_gid(pstEnv->pstAppCtx, pstEnv);
	}
	pstStoreInfo->Grid[pstStoreInfo->Num].Price = pstUp->Price;

	z_store_buy_oplog(pstEnv,pstPlayer, pstZoneStore, pstSrcGrid, 0, 0, 1, pstZoneStore->stStore.StoreInfo.Money);
	store_item_chg_notify(pstEnv, pstPlayer,  &pstStoreInfo->Grid[pstStoreInfo->Num], pstZoneStore->stStore.StoreID);
	pstStoreInfo->Num ++;

	package_dec(pstEnv->pstAppCtx,pstEnv,pstPlayer,pstSrcGrid->GridData.RoleItem.DefID, 
					pstUp->Num,LIST_TYPE_SUNDRIES,&iPos, PACKAGE_DEC_NONE,0);

	pstZoneStore->stZoneStoreInfo.iDirty = 2; // 立即更新
	return 0;
}


STOREGRID *store_get_grid(ZoneStore *pstZoneStore, tdr_ulonglong WID, int iGridIdx)
{
	int i;

	for (i=0; i<pstZoneStore->stStore.StoreInfo.Num; i++)
	{
		if (pstZoneStore->stStore.StoreInfo.Grid[i].Grid.GridData.RoleItem.GridIdx == iGridIdx &&
			pstZoneStore->stStore.StoreInfo.Grid[i].Grid.GridData.RoleItem.WID == WID)
		{
			return &pstZoneStore->stStore.StoreInfo.Grid[i];
		}
	}
	return NULL;
}

int store_item_del(ZoneStore *pstZoneStore, tdr_ulonglong WID, int iGridIdx)
{
	int i;

	for (i=pstZoneStore->stStore.StoreInfo.Num-1; i>=0; i--)
	{
		if (pstZoneStore->stStore.StoreInfo.Grid[i].Grid.GridData.RoleItem.GridIdx == iGridIdx &&
			pstZoneStore->stStore.StoreInfo.Grid[i].Grid.GridData.RoleItem.WID == WID)
		{
			pstZoneStore->stStore.StoreInfo.Num --;
			if (i != pstZoneStore->stStore.StoreInfo.Num)
			{
				pstZoneStore->stStore.StoreInfo.Grid[i] = 
					pstZoneStore->stStore.StoreInfo.Grid[pstZoneStore->stStore.StoreInfo.Num];
			}
			break;
		}	
	}
	return 0;
}

int store_item_down(ZONESVRENV* pstEnv, Player *pstPlayer, STOREDOWN*pstDown)
{
	ZoneStore *pstZoneStore;
	STOREGRID *pstStoreGrid;
	STOREGRID stStoreGrid;

	pstZoneStore = store_player_get( pstEnv,  pstPlayer);
	if (!pstZoneStore)
	{
		return -1;
	}

	pstStoreGrid = store_get_grid(pstZoneStore, pstDown->WID, pstDown->GridIdx);
	if (!pstStoreGrid)
	{
		return -1;
	}

	stStoreGrid = *pstStoreGrid;

	if (package_add_test(pstEnv->pstAppCtx, pstEnv,  pstPlayer, &stStoreGrid.Grid, 1, 0) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FENJIE1);
		return -1;
	}

	z_store_buy_oplog(pstEnv,pstPlayer, pstZoneStore, &stStoreGrid.Grid, 0, 0, 2, pstZoneStore->stStore.StoreInfo.Money);
	
	pstStoreGrid->Grid.GridData.RoleItem.Num = 0; // 删除
	store_item_chg_notify(pstEnv, pstPlayer,  pstStoreGrid, pstZoneStore->stStore.StoreID);
	store_item_del(pstZoneStore,pstDown->WID, pstDown->GridIdx);

	package_add(pstEnv->pstAppCtx,pstEnv,pstPlayer,&stStoreGrid.Grid, 1, 0, 0);

	pstZoneStore->stZoneStoreInfo.iDirty = 2; // 立即更新

	
	return 0;
}

int store_exchg_record( STOREINFO *pstStoreInfo, int iCurr, Player *pstBuy, STOREGRID *pstGrid, int iNum)
{
	if (pstStoreInfo->RecordNum >= STORE_MAX_RECORD)
	{
		memmove(&pstStoreInfo->Record[0], &pstStoreInfo->Record[1], 
				sizeof(pstStoreInfo->Record[0])*(STORE_MAX_RECORD-1));
		pstStoreInfo->RecordNum = STORE_MAX_RECORD-1;
	}
	pstStoreInfo->Record[pstStoreInfo->RecordNum].Time = iCurr;
	pstStoreInfo->Record[pstStoreInfo->RecordNum].ItemID = pstGrid->Grid.GridData.RoleItem.DefID;
	strncpy(pstStoreInfo->Record[pstStoreInfo->RecordNum].Who, pstBuy->stRoleData.RoleName,NAME_LEN);
	pstStoreInfo->Record[pstStoreInfo->RecordNum].Price = pstGrid->Price*iNum;
	pstStoreInfo->Record[pstStoreInfo->RecordNum].Num = iNum;
	pstStoreInfo->RecordNum++;
	return 0;
}

int store_buy(ZONESVRENV* pstEnv, Player *pstPlayer, STOREBUY*pstBuy)
{
	ZoneStore *pstZoneStore;
	STOREGRID *pstStoreGrid;
	STOREGRID stStoreGrid;
	Player *pstOwner;
	unsigned int  uiMoneyCount = 0;
	int iPreMoney;

	pstZoneStore = store_id_get(pstEnv, pstBuy->ID);
	if (!pstZoneStore)
	{
		return -1;
	}

	pstOwner = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneStore->stStore.Name);

	pstStoreGrid = store_get_grid(pstZoneStore, pstBuy->WID, pstBuy->GridIdx);
	if (!pstStoreGrid)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv,pstPlayer,SYS_MIDDLE, ZONEERR_STORE303);
		return -1;
	}

	if (pstBuy->Money != pstStoreGrid->Price)
	{
		return -1;
	}

	if (pstStoreGrid->Grid.GridData.RoleItem.Num < pstBuy->Num)
	{
		return -1;
	}

	iPreMoney = pstZoneStore->stStore.StoreInfo.Money;

	uiMoneyCount = pstBuy->Money*pstBuy->Num;

	stStoreGrid = *pstStoreGrid;
	stStoreGrid.Grid.GridData.RoleItem.Num = pstBuy->Num;

	if (package_dec_money_test(pstEnv, pstPlayer,ATTR_ID_MONEY, uiMoneyCount) < 0)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STORE1);
		return -1;
	}
	
	if (package_add_test(pstEnv->pstAppCtx, pstEnv,  pstPlayer, &stStoreGrid.Grid, 1, 0) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FENJIE1);
		return -1;
	}

	pstZoneStore->stStore.StoreInfo.Money += uiMoneyCount*0.95f;
	pstZoneStore->stStore.StoreInfo.Exchg += uiMoneyCount*0.95f;
	store_exchg_record( &pstZoneStore->stStore.StoreInfo,pstEnv->pstAppCtx->stCurr.tv_sec,
						pstPlayer, &stStoreGrid, pstBuy->Num);

	// 资产为正,取消关闭
	if (pstZoneStore->stStore.StoreInfo.Money >= 0)
	{
		pstZoneStore->stStore.StoreInfo.CloseTime = 0;
	}

	// 通知商铺道具金钱变化
	pstStoreGrid->Grid.GridData.RoleItem.Num -= pstBuy->Num;
	store_item_chg_notify(pstEnv, pstPlayer,  pstStoreGrid, pstZoneStore->stStore.StoreID);
	if (pstOwner)
	{
		store_item_chg_notify(pstEnv, pstOwner,  pstStoreGrid, pstZoneStore->stStore.StoreID);
		store_money_chg_notify(pstEnv, pstOwner, pstZoneStore->stStore.StoreID, pstZoneStore->stStore.StoreInfo.Money);
	}

	// 删除商铺道具
	if (pstStoreGrid->Grid.GridData.RoleItem.Num == 0)
	{
		store_item_del(pstZoneStore,pstBuy->WID, pstBuy->GridIdx);
	}
	
	// 背包通知
	package_add(pstEnv->pstAppCtx,pstEnv,pstPlayer,&stStoreGrid.Grid, 1, 0, 0);
	package_dec_money(pstEnv, pstPlayer,ATTR_ID_MONEY, uiMoneyCount);

	z_store_buy_oplog(pstEnv,pstPlayer, pstZoneStore,&stStoreGrid.Grid, uiMoneyCount, pstBuy->Num, 3, iPreMoney);

	pstZoneStore->stZoneStoreInfo.iDirty = 2; // 立即更新
	return 0;
}


int store_money_in(ZONESVRENV* pstEnv, Player *pstPlayer, STOREMONEYIN*pstMoneyIn)
{
	ZoneStore *pstZoneStore;
	int iPreMoney;
	
	pstZoneStore = store_player_get( pstEnv,  pstPlayer);
	if (!pstZoneStore)
	{
		return -1;
	}

	iPreMoney = pstZoneStore->stStore.StoreInfo.Money;

	if (pstMoneyIn->Money <= 0)
	{
		return -1;
	}

	// 取
	if (pstMoneyIn->Flag == 0)
	{
		if (pstZoneStore->stStore.StoreInfo.Money < pstMoneyIn->Money)
		{
			return -1;
		}	

		if (package_add_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, pstMoneyIn->Money) < 0)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE, ZONEERR_STORE304);
			return -1;
		}
		
		pstZoneStore->stStore.StoreInfo.Money -= pstMoneyIn->Money;
		package_add_money(pstEnv, pstPlayer, ATTR_ID_MONEY, pstMoneyIn->Money);
		z_store_buy_oplog(pstEnv,pstPlayer, pstZoneStore, NULL, pstMoneyIn->Money, 0, 5, iPreMoney);
	}
	else
	{
		unsigned int uTmp = pstZoneStore->stStore.StoreInfo.Money +pstMoneyIn->Money;
		if (uTmp > MAX_MONEY)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE, ZONEERR_STORE305);
			return -1;
		}
		
		if (package_dec_money_test(pstEnv, pstPlayer,ATTR_ID_MONEY, pstMoneyIn->Money) < 0)
		{
			return -1;
		}

		package_dec_money(pstEnv, pstPlayer,ATTR_ID_MONEY, pstMoneyIn->Money);

		pstZoneStore->stStore.StoreInfo.Money += pstMoneyIn->Money;
		// 资产为正,取消关闭
		if (pstZoneStore->stStore.StoreInfo.Money >= 0)
		{
			pstZoneStore->stStore.StoreInfo.CloseTime = 0;
		}
		z_store_buy_oplog(pstEnv,pstPlayer, pstZoneStore, NULL, pstMoneyIn->Money, 0, 6, iPreMoney);
	}

	// 通知变化
	store_money_chg_notify(pstEnv, pstPlayer, pstZoneStore->stStore.StoreID, pstZoneStore->stStore.StoreInfo.Money);
	
	pstZoneStore->stZoneStoreInfo.iDirty = 2; // 立即更新
	return 0;
}


int store_op_close(ZONESVRENV* pstEnv, Player *pstPlayer, STORECLOSE*pstClose)
{
	int iFree;
	ZoneStore *pstZoneStore;

	pstZoneStore = store_id_get(pstEnv, pstClose->ID);
	if (!pstZoneStore)
	{
		return -1;
	}

	iFree = package_get_freegrid_num(pstEnv->pstAppCtx, pstEnv, pstPlayer, LIST_TYPE_SUNDRIES);

	// 这里只做简单判断
	if (iFree < pstZoneStore->stStore.StoreInfo.Num)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FENJIE1);
		return -1;
	}

	if (pstZoneStore->stStore.StoreInfo.Money > 0)
	{
		if (package_add_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, pstZoneStore->stStore.StoreInfo.Money) < 0)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE, ZONEERR_STORE306);
			return -1;
		}
	}
	
	if (store_db_delete_req(pstEnv, &pstZoneStore->stStore) < 0)
	{
		return -1;
	}

	z_store_buy_oplog(pstEnv,pstPlayer, pstZoneStore, NULL, 0, 0, 4, pstZoneStore->stStore.StoreInfo.Money);
	
	return 0;
}

int store_view(ZONESVRENV* pstEnv, Player *pstPlayer, STOREVIEW*pstView)
{
	ZoneStore *pstZoneStore;

	pstZoneStore = store_id_get(pstEnv, pstView->ID);
	if (!pstZoneStore)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_STORE307);
		return -1;
	}

	store_info_notify(pstEnv,pstPlayer,  pstZoneStore);
	return 0;
}

int store_page(ZONESVRENV* pstEnv, Player *pstPlayer, STOREPAGE*pstPage)
{
	StoreObj *pstObj = pstEnv->pstStoreObj;
	int iPage = pstPage->Idx;
	int iCountPage;


	iCountPage = pstObj->iStoreNum/STORE_PAGE_MAX;
	if (pstObj->iStoreNum%STORE_PAGE_MAX)
	{
		iCountPage++;
	}

	if (iCountPage == 0)
	{
		return -1;
	}
	
	if (iPage >= iCountPage)
	{
		return -1;
	}

	store_page_notify(pstEnv,pstPlayer, iPage, iCountPage);
	return 0;
}


STOREGRIDDEF* store_grid_def_find(ZONESVRENV* pstEnv, int iIdx)
{
	STOREGRIDDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Idx = iIdx;
	iIndex = bsearch_int(&stDef, pstObj->sStoreGridDef, pstObj->iStoreGridDef, STORE_GRID_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (STOREGRIDDEF *)&pstObj->sStoreGridDef[iIndex];
	}
	return NULL;
}


int store_add_grid(ZONESVRENV* pstEnv, Player *pstPlayer, STOREADDGRID*pstAdd)
{
	ZoneStore *pstZoneStore;
	STOREGRIDDEF* pstDef;
	
	pstZoneStore = store_player_get( pstEnv,  pstPlayer);
	if (!pstZoneStore)
	{
		return -1;
	}

	if (pstZoneStore->stStore.StoreInfo.MaxNum >= MAX_STORE_GRID)
	{
		return -1;
	}
	
	if (pstAdd->Idx != pstZoneStore->stStore.StoreInfo.MaxNum)
	{
		return -1;
	}

	pstDef = store_grid_def_find(pstEnv, pstAdd->Idx);
	if (!pstDef)
	{
		return -1;
	}

	if (package_dec_test(pstEnv->pstAppCtx,pstEnv,pstPlayer,pstDef->UseItemID,pstDef->UseItemNum,LIST_TYPE_SUNDRIES,
						NULL,0) < 0)
	{	
		return -1;
	}

	package_dec(pstEnv->pstAppCtx,pstEnv,pstPlayer,pstDef->UseItemID,pstDef->UseItemNum,LIST_TYPE_SUNDRIES, 
					NULL,0, 0);
	pstZoneStore->stStore.StoreInfo.MaxNum ++;

	pstZoneStore->stZoneStoreInfo.iDirty = 2; // 立即更新

	store_grid_chg_notify(pstEnv, pstPlayer, pstZoneStore);
	return 0;
}

int store_dec_grid(ZONESVRENV* pstEnv, Player *pstPlayer, STOREDECGRID*pstDec)
{
	ZoneStore *pstZoneStore;
	int iDecIdx;
	int i;
	
	pstZoneStore = store_player_get( pstEnv,  pstPlayer);
	if (!pstZoneStore)
	{
		return -1;
	}

	if (pstZoneStore->stStore.StoreInfo.MaxNum <= STORE_GRID_INIT)
	{
		return -1;
	}

	iDecIdx = pstZoneStore->stStore.StoreInfo.MaxNum-1;

	for (i=0; i<pstZoneStore->stStore.StoreInfo.Num; i++)
	{
		if (pstZoneStore->stStore.StoreInfo.Grid[i].Grid.GridData.RoleItem.GridIdx == iDecIdx)
		{
			return -1;
		}
	}

	pstZoneStore->stStore.StoreInfo.MaxNum --;
	pstZoneStore->stZoneStoreInfo.iDirty = 2; // 立即更新
	
	store_grid_chg_notify(pstEnv, pstPlayer, pstZoneStore);
	return 0;
}




int store_pos_req(ZONESVRENV* pstEnv, Player *pstPlayer, CLTSTOREPOS*pstReq)
{
	ZoneStore *pstZoneStore;

	pstZoneStore = store_id_get(pstEnv, pstReq->ID);
	if (!pstZoneStore)
	{
		return -1;
	}

	return store_pos_notify(pstEnv, pstPlayer, pstZoneStore - &pstEnv->pstStoreObj->astZoneStore[0], pstReq->ID,
							pstZoneStore->stStore.StoreInfo.Exchg);
}	

int store_open_ui(ZONESVRENV* pstEnv, Player *pstPlayer, CLTSTOREUI *pstUI)
{
	RESULT *pstVipResult;
	ZoneStore *pstZoneStore;
	int iCountPage;
	StoreObj *pstObj;
	
	pstObj = pstEnv->pstStoreObj;

	pstVipResult = star_get_result(pstEnv, pstPlayer, STAR_RESULT_STORE);
	if (NULL == pstVipResult)
	{
		return -1;
	}

	if (stroe_open_ui_check( pstEnv, pstPlayer) < 0)
	{
		return -1;
	}
	
	iCountPage = pstObj->iStoreNum/STORE_PAGE_MAX;
	if (pstObj->iStoreNum%STORE_PAGE_MAX)
	{
		iCountPage++;
	}

	pstZoneStore = store_player_get(pstEnv, pstPlayer);

	store_open_ui_notify(pstEnv,pstPlayer, 0, iCountPage,0,pstZoneStore?(&pstZoneStore->stStore):NULL);
	return 0;
}


int store_search(ZONESVRENV* pstEnv, Player *pstPlayer, CLTSTORESEARCH* pstSearch )
{
	StoreObj *pstObj = pstEnv->pstStoreObj;
	int i,j;
	ZoneStore *pstStore ;
	STOREINFO *pstInfo;
	ITEMDEF *pstItemDef;

	if (pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stRoleData.MiscInfo.Store.SearchTime < 3)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_OP_LIMIT, 3);
		return -1;
	}

	if (pstSearch->KeyWorlds[0] == '\0')
	{
		return -1;
	}

	CSPKG stPkg;
	CSSTORESVR *pstSvr = &stPkg.Body.StoreSvr;
	pstSvr->Cmd = STORE_SVR_OP_SEARCH;
	pstSvr->Data.Search.Num = 0;

	for (i=0; i<pstObj->iStoreNum; i++)
	{
		pstStore = &pstObj->astZoneStore[i];
		pstInfo = &pstStore->stStore.StoreInfo;

		for (j=0; j<pstInfo->Num; j++)
		{
			pstItemDef = z_find_itemdef( pstEnv, pstInfo->Grid[j].Grid.GridData.RoleItem.DefID);
			if (!pstItemDef)
				continue;
			
			if (strstr(pstItemDef->Name, pstSearch->KeyWorlds))
			{
				pstSvr->Data.Search.IDs[pstSvr->Data.Search.Num] = pstStore->stStore.StoreID;
				pstSvr->Data.Search.Num++;	
				break;
			}	
		}
	}

	if (pstSvr->Data.Search.Num == 0)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STORE9);
	}
	else
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STORE8,pstSvr->Data.Search.Num);
	}

	Z_CSHEAD_INIT(&stPkg.Head, STORE_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	 pstPlayer->stRoleData.MiscInfo.Store.SearchTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	return 0;
}

int store_chg_name(ZONESVRENV* pstEnv, Player *pstPlayer, STORECHGNAME*pstChg)
{
	ZoneStore *pstZoneStore;
	int i;
	StoreObj *pstObj = pstEnv->pstStoreObj;
	
	pstZoneStore = store_player_get( pstEnv,  pstPlayer);
	if (!pstZoneStore)
	{
		return -1;
	}

	if (pstChg->StoreName[0] == '\0')
	{
		return -1;
	}

	if ((CheckDirty(pstChg->StoreName , 0x1f) & 0x1f ) )
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIRTY);
		return -1;
	}

	for(i = 0; i < pstObj->iStoreNum; ++i)
	{
		if (strcmp(pstObj->astZoneStore[i].stStore.StoreInfo.StoreName, pstChg->StoreName) == 0)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STORE2);
			return -1;
		}
	}

	strncpy(pstZoneStore->stStore.StoreInfo.StoreName, pstChg->StoreName, sizeof(pstZoneStore->stStore.StoreInfo.StoreName));

	store_info_notify(pstEnv, pstPlayer,pstZoneStore);
	return 0;
}

int store_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
					TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	StoreObj *pstObj = pstEnv->pstStoreObj;
	CSSTORECLT *pstClt = &pstCsPkg->Body.StoreClt;
	
	if (INST_MODE_ZONE_SVR != pstEnv->iInstMode)
	{
		return 0;
	}

	if (pstObj->stObjStoreInfo.iStat != STORE_STAT_FINI)
	{
		return 0;
	}

	switch(pstClt->Cmd)
	{
	case STORE_CLT_OP_OPEN:
		store_open(pstEnv, pstPlayer, &pstClt->Data.Open);
		break;
	case STORE_CLT_OP_BUY:
		 store_buy(pstEnv, pstPlayer, &pstClt->Data.Buy);
		break;
	case STORE_CLT_OP_ADD_GRID:
		store_add_grid(pstEnv, pstPlayer, &pstClt->Data.AddGrid);
		break;
	case STORE_CLT_OP_CLOSE:
		store_op_close(pstEnv, pstPlayer, &pstClt->Data.Close);
		break;
	case STORE_CLT_OP_DOWN:
		store_item_down(pstEnv, pstPlayer, &pstClt->Data.Down);
		break;
	case STORE_CLT_OP_MONEY_IN:
		store_money_in(pstEnv, pstPlayer, &pstClt->Data.MoneyIn);
		break;
	case STORE_CLT_OP_PAGE:
		store_page(pstEnv, pstPlayer, &pstClt->Data.Page);
		break;
	case STORE_CLT_OP_UP:
		store_item_up(pstEnv, pstPlayer, &pstClt->Data.Up);
		break;
	case STORE_CLT_OP_VIEW:
		store_view(pstEnv, pstPlayer, &pstClt->Data.View);
		break;
	case STORE_CLT_OP_DEC_GRID:
		store_dec_grid(pstEnv, pstPlayer, &pstClt->Data.DecGrid);
		break;
	case STORE_CLT_OP_POS:
		store_pos_req(pstEnv, pstPlayer, &pstClt->Data.Pos);
		break;
	case STORE_CLT_OP_OPEN_UI:
		store_open_ui(pstEnv, pstPlayer, &pstClt->Data.UI);
		break;
	case STORE_CLT_OP_SEARCH:
		store_search(pstEnv, pstPlayer, &pstClt->Data.Search);
		break;
	case STORE_CLT_OP_CHG_NAME:
		store_chg_name(pstEnv, pstPlayer, &pstClt->Data.ChgName);
		break;
	default:
		return -1;
		break;
	
	}
	return 0;
}


// 检查insert返回,找不到player的情况
void store_player_login(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	StoreObj *pstObj = pstEnv->pstStoreObj;
	int i;
	ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;
	ZoneStore *pstZoneStore;
//	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	if (INST_MODE_ZONE_SVR != pstEnv->iInstMode)
	{
		return ;
	}

	if (pstObj->stObjStoreInfo.iStat != STORE_STAT_FINI)
	{
		return ;
	}

	// 检查delete成功角色不在线的情况
	if (pstMisc->Store.ID > 0)
	{
		pstZoneStore = store_player_get( pstEnv, pstPlayer);
		if (!pstZoneStore)
		{
			store_player_clear_id(pstPlayer);
			store_tip1(pstEnv,pstPlayer);
		}
		else
		{
			if (pstZoneStore->stStore.StoreInfo.CloseTime > 0)
			{
				store_tip2(pstEnv,pstPlayer, pstZoneStore);
			}
		}
	}

	// 检查insert成功 角色不在线的情况
	for (i=0; i<pstObj->iStoreNum; i++)
	{
		if (pstObj->astZoneStore[i].stStore.RoleID == pstPlayer->stRoleData.RoleID &&
			pstPlayer->stRoleData.MiscInfo.Store.ID == 0)
		{
			pstPlayer->stRoleData.MiscInfo.Store.ID = pstObj->astZoneStore[i].stStore.StoreID;
			break;
		}
	}

	// 登录在检查下玩家改名
	store_process_role_chg_name(pstEnv, pstPlayer);
}

int store_update(ZONESVRENV *pstEnv, ZoneStore*pstZoneStore)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
		
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, 
	                 "Store", -1, NULL);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&pstZoneStore->stStore;
	stMetaData.iBuff = sizeof(pstZoneStore->stStore);
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

	pstZoneStore->stZoneStoreInfo.iDirty = 0;
	return 0;
}

int store_calc_rent(ZONESVRENV *pstEnv, int iMax)
{
	int iMoney =0;
	int iAdd;
	unsigned int iExchg = pstEnv->pstGlobalObj->stGlobal.GoldRate.GoldRate;

	if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat != GLOBAL_FINI)
	{
		return 1;
	}

	iAdd = iMax - STORE_GRID_INIT;
	iMoney = ceil((iExchg /20.0)*((0.2*iAdd)*(0.2*iAdd))+iExchg /20.0);
	return iMoney;
}



int store_tick(ZONESVRENV *pstEnv, ZoneStore*pstZoneStore)
{
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	int iDec = 0;
	int iMoney;
	Player *pstOwner;
	int iOldMoney;

	pstOwner = z_name_player(pstEnv->pstAppCtx,pstEnv, pstZoneStore->stStore.Name);
	pstZoneStore->stZoneStoreInfo.uiTimeCount ++;
	
	if (	pstZoneStore->stStore.StoreInfo.Money < 0 &&
		pstZoneStore->stStore.StoreInfo.CloseTime  > 0 &&
		iCurr >= (int)pstZoneStore->stStore.StoreInfo.CloseTime )
	{
		// 关闭商店
		if (store_db_delete_req(pstEnv, &pstZoneStore->stStore) < 0)
		{
			return -1;
		}

		store_tip1(pstEnv,pstOwner);
		z_store_buy_oplog(pstEnv,pstOwner, pstZoneStore, NULL, 0, 0, 8, pstZoneStore->stStore.StoreInfo.Money);
		return 0;
	}
	
	if (pstZoneStore->stStore.StoreInfo.PreDecTime<= 0)
	{
		if (iCurr - pstZoneStore->stStore.StoreInfo.OpenTime >= 60*60)
		{
			iDec = 1;
		}
	}
	else
	{
		if (iCurr - pstZoneStore->stStore.StoreInfo.PreDecTime >= 60*60)
		{
			iDec = 1;
		}
	}

	if (iDec)
	{
		pstZoneStore->stStore.StoreInfo.PreDecTime = iCurr;
		
		iMoney = store_calc_rent(pstEnv, pstZoneStore->stStore.StoreInfo.MaxNum); 
		iOldMoney = pstZoneStore->stStore.StoreInfo.Money;
		
		pstZoneStore->stStore.StoreInfo.Money -= iMoney;
		pstZoneStore->stStore.StoreInfo.Tax += iMoney;
		pstZoneStore->stZoneStoreInfo.iDirty = 2; // 立即更新
		
		if (iOldMoney > 0)
		{
			int iLogMoney;

			iLogMoney = iMoney;
			if (pstZoneStore->stStore.StoreInfo.Money <= 0)
			{
				iLogMoney = iOldMoney;
			}

			z_money_consume_oplog(pstEnv, pstOwner, MONEY_OPTYPE_STORE_TAX, 
				0, 0, iLogMoney, "商铺租金");
			
			z_store_buy_oplog(pstEnv,pstOwner, pstZoneStore, NULL, iLogMoney, 0, 7, iOldMoney);
		}
		
		if (pstOwner)
		{
			store_money_chg_notify(pstEnv,pstOwner, pstZoneStore->stStore.StoreID, 
									pstZoneStore->stStore.StoreInfo.Money);
		}
		
		if (	pstZoneStore->stStore.StoreInfo.Money < 0 &&
			pstZoneStore->stStore.StoreInfo.CloseTime  == 0)
		{
			// 2天后关闭
			pstZoneStore->stStore.StoreInfo.CloseTime = iCurr+48*60*60;
		}

		if (pstZoneStore->stStore.StoreInfo.CloseTime > 0 && pstOwner)
		{
			store_tip2(pstEnv,pstOwner,pstZoneStore);
		}
	}

	switch(pstZoneStore->stZoneStoreInfo.iDirty)
	{
	case 0:// 干净的
	
		break;
	case 1:// 定时更新
		if (pstZoneStore->stZoneStoreInfo.uiTimeCount % 100 == 0)
		{
			store_update(pstEnv, pstZoneStore);	
		}
		break;
	case 2:// 立即更新
		store_update(pstEnv, pstZoneStore);	
		break;
	}
	return 0;
}

#define STORE_ACTIVE_TIMER 5000

// 五秒一次
int z_active_store(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, int iCount)
{
	static struct timeval stRoundTv;
	static struct timeval stRoundStartTv;
	static int iActivePos = 0, iRound = 0;
	int i;
	struct timeval stSub;
	long long  llMs=0;
	ZoneStore*pstZoneStore = NULL;
	StoreObj *pstStoreObj = pstEnv->pstStoreObj;

	if (INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
	{
		return 0;
	}

	if (pstStoreObj->stObjStoreInfo.iStat != STORE_STAT_FINI)
	{
		return 0;
	}

	// 关服阶段不排序了
	if (!IsSameHour(pstStoreObj->stObjStoreInfo.iQsortTime, pstEnv->pstAppCtx->stCurr.tv_sec) &&
		pstEnv->cShutdown == 0)
	{
		// 排序
		store_qsort_by_time(pstEnv);
		pstStoreObj->stObjStoreInfo.iQsortTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		return 0;
	}

	// 数据库10秒没有相应,自动解锁
	/*if (	pstStoreObj->stObjStoreInfo.iInsertLock &&
		pstEnv->pstAppCtx->stCurr.tv_sec >= pstStoreObj->stObjStoreInfo.iInsertTime+10)
	{
		pstStoreObj->stObjStoreInfo.iInsertLock = 0;
		pstStoreObj->stObjStoreInfo.iInsertTime = 0;	
	}*/
	
	if (0 == stRoundTv.tv_sec)
	{
		llMs = STORE_ACTIVE_TIMER;
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
		
	if (llMs < STORE_ACTIVE_TIMER)
	{
		return 0;
	}

	if (0 == iActivePos)
	{
		TV_CLONE(stRoundStartTv, pstAppCtx->stCurr);
	}

	for(i = 0; i < iCount; ++i) 
	{
		if (iActivePos >= pstStoreObj->iStoreNum)
		{
			break;
		}

		pstZoneStore = &pstStoreObj->astZoneStore[iActivePos];
		store_tick(pstEnv, pstZoneStore);

		++iActivePos;
	}
	
	if (iActivePos < pstStoreObj->iStoreNum)
	{
		return 0;
	}

	iActivePos = 0;
	++iRound;
	TV_CLONE(stRoundTv, pstAppCtx->stCurr);
	TV_DIFF(stSub, pstAppCtx->stCurr, stRoundStartTv);
	TV_TO_MS(llMs, stSub);
	if (llMs > STORE_ACTIVE_TIMER)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "active store loop %lld ms", llMs);
	}

	return 0;
}


int store_update_req(ZONESVRENV* pstEnv, ZoneStore*pstZoneStore)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq =  &pstPkg->Body.UpdateReq;
	DBInsertBack stBack;

	stBack.iID = pstZoneStore->stStore.StoreID;
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,0,&pstPkg->Head, 
								TORM_UPDATE_REQ,DB_STORE_UPDATE_RES, 
			               			"Store", 
			               			(char*)&stBack, 
			               			sizeof(stBack));
	
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;
	
	iDst = pstEnv->iMiscDBID;
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,	&pstZoneStore->stStore, 
			sizeof(pstZoneStore->stStore),&iDst,pstEnv->iRoleDBProtoVer);
}

int store_update_res(ZONESVRENV *pstEnv, TORMPKG *pstTormPkg, DBAsyn *pstDBAsyn)
{
	TORMUPDATERES *pstUpdateRes = &pstTormPkg->Body.UpdateRes;
	ZoneStore*pstStore = NULL;
	DBInsertBack *pstBack;
	
	pstBack = (DBInsertBack *)(pstDBAsyn->AsynCmd.szMaxBuf);

	if (TORM_ERR_IS_ERROR(pstUpdateRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "db update store failed." );
		return -1;
	}

	pstStore = store_id_get(pstEnv, pstBack->iID);
	if (!pstStore)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "store_update_res:can not find store." );
		return -1;
	}

	pstStore->stZoneStoreInfo.iDirty = 0;
	return 0;
}


#define STORE_STOP_PATCH 25

int z_db_store_finish(ZONESVRENV *pstEnv)
{
	StoreObj *pstObj = pstEnv->pstStoreObj;
	int i, iCount = 0;
	ZoneStore *pstZoneStore;


	if (pstObj->stObjStoreInfo.iStat != STORE_STAT_FINI)
	{
		return 0;
	}

	for(i = 0; i < pstObj->iStoreNum; ++i)
	{
		pstZoneStore = &pstObj->astZoneStore[i];

		pstZoneStore->stZoneStoreInfo.iDirty = 2;
		store_update_req(pstEnv, pstZoneStore);
		
		if (iCount++ > STORE_STOP_PATCH)
		{
			usleep(STORE_STOP_PATCH * 1000); 
			iCount = 0;
		}
	}
	return 0;
}

int z_db_store_is_allclean(ZONESVRENV *pstEnv)
{
	StoreObj *pstObj = pstEnv->pstStoreObj;
	int i, iCount = 0;
	ZoneStore *pstZoneStore;


	if (pstObj->stObjStoreInfo.iStat != STORE_STAT_FINI)
	{
		return 1;
	}

	for(i = 0; i < pstObj->iStoreNum; ++i)
	{
		pstZoneStore = &pstObj->astZoneStore[i];
		if (pstZoneStore->stZoneStoreInfo.iDirty == 0)
		{
			++iCount;
		}
	}

	return (iCount == pstObj->iStoreNum );
}


int store_process_role_chg_name(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	ZoneStore *pstZoneStore;
	StoreObj *pstObj = pstEnv->pstStoreObj;
	
	if (pstObj->stObjStoreInfo.iStat != STORE_STAT_FINI)
	{
		return 0;
	}
	
	pstZoneStore = store_player_get( pstEnv,  pstPlayer);
	if (!pstZoneStore)
	{
		return 0;
	}

	if (pstZoneStore->stStore.RoleID == pstPlayer->stRoleData.RoleID &&
		strcmp(pstZoneStore->stStore.Name, pstPlayer->stRoleData.RoleName))
	{
		strncpy(pstZoneStore->stStore.Name, pstPlayer->stRoleData.RoleName, sizeof(pstZoneStore->stStore.Name));
		pstZoneStore->stZoneStoreInfo.iDirty = 2; // 立即更新
	}
	return 0;
}
