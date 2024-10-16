#include "zone_vipbar.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "role_db_meta.h"
#include "zone_clt.h"
#include "zone_attr.h"
#include "zone_player.h"
#include "zone_oplog.h"
#include "zone_shop.h"
#include "masktypes.h"


int g_SelectCountVipBarDefNum = 0;
int g_LoadedVipBarDefNum = 0;

int g_SelectCountVipBarLevelDefNum = 0;
int g_LoadedVipBarLevelDefNum = 0;

int g_SelectCountVipBarNumDefNum = 0;
int g_LoadedVipBarNumDefNum = 0;

int g_iVipBarSendTime = 0;


int  hash_insert_vip_bar_def(ZONESVRENV *pstEnv,unsigned int uiKey,char* pszData,unsigned int iSize)
{
	int iRet = 0;
	VipBarBucket *pstPos, stDummy;
	VIPBARDEF *pstVipBarDefOld = NULL;

	if( iSize > sizeof(VipBarBucket))
	{
		return -1;
	}

	pstVipBarDefOld = z_find_vip_bar_def(pstEnv,uiKey);
	if( NULL!= pstVipBarDefOld )
	{
		if( memcmp(pszData,pstVipBarDefOld,sizeof(VIPBARDEF)) != 0   ) 
		{
			/* 修改的数据 */
			memcpy( pstVipBarDefOld, pszData,sizeof(VIPBARDEF));
			iRet = 1;
		}
	}
	else
	{
		iRet = 1;
	}
	
	// 不管有没有 都做插入操作
	stDummy.uiKey = uiKey;
	pstPos = (VipBarBucket *)sht_insert_unique(pstEnv->pstVipBarHash, &stDummy, hash_cmp_int,
		                        hash_code_int);
	if(pstPos)
	{
		pstPos->uiKey = uiKey;
		memcpy( pstPos->szData, pszData,iSize);
	}
	
	return iRet;
}

VIPBARDEF * z_find_vip_bar_def(ZONESVRENV *pstEnv,unsigned int uiKey)
{
	VipBarBucket *pstPos, stDummy;

	stDummy.uiKey = uiKey;
	pstPos = (VipBarBucket *)sht_find(pstEnv->pstVipBarHash, &stDummy, hash_cmp_int, hash_code_int);

	if( pstPos != NULL )
	{
		return (VIPBARDEF *)pstPos->szData;
	}
	
	return  NULL;
}

int shopping_db_vip_bar_def_count_req(ZONESVRENV *pstEnv)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTCOUNTREQ *pstSelectCountReq = &pstPkg->Body.SelectCountReq;
	VIPBARDEF stVipBarDef;

	memset(&stVipBarDef,0,sizeof(stVipBarDef));
	
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,-1,&pstPkg->Head, 
								TORM_SELECT_COUNT_REQ,DB_SELECT_COUNT_VIP_BAR_DEF_RES, 
			               			"VipBarDef", 
			               			NULL, 
			               			0);/* update 修改点1*/
	
	strcpy(pstSelectCountReq->WhereDef, "where BarID != 0");


	iDst = pstEnv->iRegionDBID;

	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,&stVipBarDef, 
				sizeof(VIPBARDEF),&iDst,pstEnv->iRegionDBProtoVer);

}

int shopping_db_vip_bar_def_count_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg)
{	
	TORMSELECTCOUNTRES *pstSelectCountRes;
	int iRecordNum = 0;
	unsigned int uiBeginRecord = 0;

	
	pstSelectCountRes = &pstPkg->Body.SelectCountRes;
	if (TORM_ERR_IS_ERROR(pstSelectCountRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "select count res error errno=%d, errmsg=%s", 
					pstSelectCountRes->ErrNo,torm_error_string(pstSelectCountRes->ErrNo));
		return -1;
	}
	
	iRecordNum = pstSelectCountRes->RowsCount;

	g_SelectCountVipBarDefNum = iRecordNum;
	while ( iRecordNum > 0 )
	{
		/* 开始取数据了 */
		shopping_db_vip_bar_def_select_req( pstEnv, uiBeginRecord);
		
		uiBeginRecord +=  PRE_GET_DB_RECORD;
		iRecordNum -= 	PRE_GET_DB_RECORD;
	}

	pstEnv->pstZoneObj->iVipBarGetStat = 1;

	if (g_SelectCountVipBarDefNum < 1)
	{
		pstEnv->pstZoneObj->iVipBarGetStat = 2;
		z_check_vip_bar_all_loaded(pstEnv);
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int shopping_db_vip_bar_def_select_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg)
{
	int i;
	TORMSELECTRES *pstSelectRes;
	VIPBARDEF stVipBarDef;
	int iRet = 0;


	if (pstEnv->pstZoneObj->iVipBarGetStat != 1)
	{
		return -1;
	}
	
	pstSelectRes = &pstPkg->Body.SelectRes;
		
	for(i=0; i<pstSelectRes->SelectResult.ResultNum ; i++)
	{
		memset(&stVipBarDef,0,sizeof(stVipBarDef));
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stVipBarDef, sizeof(VIPBARDEF)))
		{
			continue;
		}
		/*数据都先弄出来,等扫描cache的时候我们再来做删除操作,清晰点*/

		iRet =   hash_insert_vip_bar_def(pstEnv,stVipBarDef.DefID,(char*)&stVipBarDef,sizeof(VIPBARDEF));

	}
	g_LoadedVipBarDefNum += pstSelectRes->SelectResult.ResultNum;

	if( g_LoadedVipBarDefNum >= g_SelectCountVipBarDefNum)
	{
		//读取完成
		pstEnv->pstZoneObj->iVipBarGetStat = 2;
		z_check_vip_bar_all_loaded(pstEnv);
	}

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int shopping_db_vip_bar_def_select_req( ZONESVRENV* pstEnv,unsigned int iBeginRecord)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	VIPBARDEF stVipBarDef;

	memset(&stVipBarDef,0,sizeof(stVipBarDef));
	
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,-1,&pstPkg->Head, 
								TORM_SELECT_REQ,DB_SELECT_VIP_BAR_DEF_RES, 
			               			"VipBarDef", 
			               			NULL, 
			               			0);/* update 修改点1*/
	
	pstSelectReq->OffSet = iBeginRecord;
	pstSelectReq->Limit = PRE_GET_DB_RECORD;
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	strcpy(pstSelectReq->OperInfo.WhereDef, "where BarID != 0");
	
	iDst = pstEnv->iRegionDBID;
	
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,&stVipBarDef, 
			sizeof(VIPBARDEF),&iDst,pstEnv->iRegionDBProtoVer);
	
}

int  hash_insert_vip_bar_level_def(ZONESVRENV *pstEnv,unsigned int uiKey,char* pszData,unsigned int iSize)
{
	int iRet = 0;
	VipBarLevelBucket *pstPos, stDummy;
	VIPBARLEVELDEF *pstVipBarLevelDefOld = NULL;

	if( iSize > sizeof(VipBarLevelBucket))
	{
		return -1;
	}

	pstVipBarLevelDefOld = z_find_vip_bar_level_def(pstEnv,uiKey);
	if( NULL!= pstVipBarLevelDefOld )
	{
		if( memcmp(pszData,pstVipBarLevelDefOld,sizeof(VIPBARLEVELDEF)) != 0   ) 
		{
			/* 修改的数据 */
			memcpy( pstVipBarLevelDefOld, pszData,sizeof(VIPBARLEVELDEF));
			iRet = 1;
		}
	}
	else
	{
		iRet = 1;
	}
	
	// 不管有没有 都做插入操作
	stDummy.uiKey = uiKey;
	pstPos = (VipBarLevelBucket *)sht_insert_unique(pstEnv->pstVipBarLevelHash, &stDummy, hash_cmp_int,
		                        hash_code_int);
	if(pstPos)
	{
		pstPos->uiKey = uiKey;
		memcpy( pstPos->szData, pszData,iSize);
	}
	
	return iRet;
}

VIPBARLEVELDEF * z_find_vip_bar_level_def(ZONESVRENV *pstEnv,unsigned int uiKey)
{
	VipBarLevelBucket *pstPos, stDummy;

	stDummy.uiKey = uiKey;
	pstPos = (VipBarLevelBucket *)sht_find(pstEnv->pstVipBarLevelHash, &stDummy, hash_cmp_int, hash_code_int);

	if( pstPos != NULL )
	{
		return (VIPBARLEVELDEF *)pstPos->szData;
	}
	
	return  NULL;
}

int shopping_db_vip_bar_level_def_count_req(ZONESVRENV *pstEnv)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTCOUNTREQ *pstSelectCountReq = &pstPkg->Body.SelectCountReq;
	VIPBARLEVELDEF stVipBarLevelDef;

	memset(&stVipBarLevelDef,0,sizeof(stVipBarLevelDef));
	
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,-1,&pstPkg->Head, 
								TORM_SELECT_COUNT_REQ,DB_SELECT_COUNT_VIP_BAR_LEVEL_DEF_RES, 
			               			"VipBarLevelDef", 
			               			NULL, 
			               			0);/* update 修改点1*/
	
	strcpy(pstSelectCountReq->WhereDef, "where Level != 0");


	iDst = pstEnv->iRegionDBID;

	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,&stVipBarLevelDef, 
				sizeof(VIPBARLEVELDEF),&iDst,pstEnv->iRegionDBProtoVer);

}

int shopping_db_vip_bar_level_def_count_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg)
{	
	TORMSELECTCOUNTRES *pstSelectCountRes;
	int iRecordNum = 0;
	unsigned int uiBeginRecord = 0;

	
	pstSelectCountRes = &pstPkg->Body.SelectCountRes;
	if (TORM_ERR_IS_ERROR(pstSelectCountRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "select count res error errno=%d, errmsg=%s", 
					pstSelectCountRes->ErrNo,torm_error_string(pstSelectCountRes->ErrNo));
		return -1;
	}
	
	iRecordNum = pstSelectCountRes->RowsCount;

	g_SelectCountVipBarLevelDefNum = iRecordNum;
	while ( iRecordNum > 0 )
	{
		/* 开始取数据了 */
		shopping_db_vip_bar_level_def_select_req( pstEnv, uiBeginRecord);
		
		uiBeginRecord +=  PRE_GET_DB_RECORD;
		iRecordNum -= 	PRE_GET_DB_RECORD;
	}

	pstEnv->pstZoneObj->iVipBarLevelGetStat = 1;

	if (g_SelectCountVipBarLevelDefNum < 1)
	{
		pstEnv->pstZoneObj->iVipBarLevelGetStat = 2;
		z_check_vip_bar_all_loaded(pstEnv);
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int shopping_db_vip_bar_level_def_select_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg)
{
	int i;
	TORMSELECTRES *pstSelectRes;
	VIPBARLEVELDEF stVipBarLevelDef;
	int iRet = 0;


	if (pstEnv->pstZoneObj->iVipBarLevelGetStat != 1)
	{
		return -1;
	}
	
	pstSelectRes = &pstPkg->Body.SelectRes;
		
	for(i=0; i<pstSelectRes->SelectResult.ResultNum ; i++)
	{
		memset(&stVipBarLevelDef,0,sizeof(stVipBarLevelDef));
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stVipBarLevelDef, sizeof(VIPBARLEVELDEF)))
		{
			continue;
		}
		/*数据都先弄出来,等扫描cache的时候我们再来做删除操作,清晰点*/

		iRet =   hash_insert_vip_bar_level_def(pstEnv,stVipBarLevelDef.Level,(char*)&stVipBarLevelDef,sizeof(VIPBARLEVELDEF));

	}
	g_LoadedVipBarLevelDefNum += pstSelectRes->SelectResult.ResultNum;

	if( g_LoadedVipBarLevelDefNum >= g_SelectCountVipBarLevelDefNum)
	{
		//读取完成
		pstEnv->pstZoneObj->iVipBarLevelGetStat = 2;
		z_check_vip_bar_all_loaded(pstEnv);
	}

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int shopping_db_vip_bar_level_def_select_req( ZONESVRENV* pstEnv,unsigned int iBeginRecord)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	VIPBARLEVELDEF stVipBarLevelDef;

	memset(&stVipBarLevelDef,0,sizeof(stVipBarLevelDef));
	
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,-1,&pstPkg->Head, 
								TORM_SELECT_REQ,DB_SELECT_VIP_BAR_LEVEL_DEF_RES, 
			               			"VipBarLevelDef", 
			               			NULL, 
			               			0);/* update 修改点1*/
	
	pstSelectReq->OffSet = iBeginRecord;
	pstSelectReq->Limit = PRE_GET_DB_RECORD;
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	strcpy(pstSelectReq->OperInfo.WhereDef, "where Level != 0");
	
	iDst = pstEnv->iRegionDBID;
	
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,&stVipBarLevelDef, 
			sizeof(VIPBARLEVELDEF),&iDst,pstEnv->iRegionDBProtoVer);
	
}

int  hash_insert_vip_bar_num_def(ZONESVRENV *pstEnv,unsigned int uiKey,char* pszData,unsigned int iSize)
{
	int iRet = 0;
	VipBarNumBucket *pstPos, stDummy;
	VIPBARNUMDEF *pstVipBarNumDefOld = NULL;

	if( iSize > sizeof(VipBarNumBucket))
	{
		return -1;
	}

	pstVipBarNumDefOld = z_find_vip_bar_num_def(pstEnv,uiKey);
	if( NULL!= pstVipBarNumDefOld )
	{
		if( memcmp(pszData,pstVipBarNumDefOld,sizeof(VIPBARNUMDEF)) != 0   ) 
		{
			/* 修改的数据 */
			memcpy( pstVipBarNumDefOld, pszData,sizeof(VIPBARNUMDEF));
			iRet = 1;
		}
	}
	else
	{
		iRet = 1;
	}
	
	// 不管有没有 都做插入操作
	stDummy.uiKey = uiKey;
	pstPos = (VipBarNumBucket *)sht_insert_unique(pstEnv->pstVipBarNumHash, &stDummy, hash_cmp_int,
		                        hash_code_int);
	if(pstPos)
	{
		pstPos->uiKey = uiKey;
		memcpy( pstPos->szData, pszData,iSize);
	}
	
	return iRet;
}

VIPBARNUMDEF * z_find_vip_bar_num_def(ZONESVRENV *pstEnv,unsigned int uiKey)
{
	VipBarNumBucket *pstPos, stDummy;

	stDummy.uiKey = uiKey;
	pstPos = (VipBarNumBucket *)sht_find(pstEnv->pstVipBarNumHash, &stDummy, hash_cmp_int, hash_code_int);

	if( pstPos != NULL )
	{
		return (VIPBARNUMDEF *)pstPos->szData;
	}
	
	return  NULL;
}

int shopping_db_vip_bar_num_def_count_req(ZONESVRENV *pstEnv)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTCOUNTREQ *pstSelectCountReq = &pstPkg->Body.SelectCountReq;
	VIPBARNUMDEF stVipBarNumDef;

	memset(&stVipBarNumDef,0,sizeof(stVipBarNumDef));
	
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,-1,&pstPkg->Head, 
								TORM_SELECT_COUNT_REQ,DB_SELECT_COUNT_VIP_BAR_NUM_DEF_RES, 
			               			"VipBarNumDef", 
			               			NULL, 
			               			0);/* update 修改点1*/
	
	strcpy(pstSelectCountReq->WhereDef, "where Num != 0");


	iDst = pstEnv->iRegionDBID;

	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,&stVipBarNumDef, 
				sizeof(VIPBARNUMDEF),&iDst,pstEnv->iRegionDBProtoVer);

}

int shopping_db_vip_bar_num_def_count_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg)
{	
	TORMSELECTCOUNTRES *pstSelectCountRes;
	int iRecordNum = 0;
	unsigned int uiBeginRecord = 0;

	
	pstSelectCountRes = &pstPkg->Body.SelectCountRes;
	if (TORM_ERR_IS_ERROR(pstSelectCountRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "select count res error errno=%d, errmsg=%s", 
					pstSelectCountRes->ErrNo,torm_error_string(pstSelectCountRes->ErrNo));
		return -1;
	}
	
	iRecordNum = pstSelectCountRes->RowsCount;

	g_SelectCountVipBarNumDefNum = iRecordNum;
	while ( iRecordNum > 0 )
	{
		/* 开始取数据了 */
		shopping_db_vip_bar_num_def_select_req( pstEnv, uiBeginRecord);
		
		uiBeginRecord +=  PRE_GET_DB_RECORD;
		iRecordNum -= 	PRE_GET_DB_RECORD;
	}

	pstEnv->pstZoneObj->iVipBarNumGetStat = 1;

	if (g_SelectCountVipBarNumDefNum < 1)
	{
		pstEnv->pstZoneObj->iVipBarNumGetStat = 2;
		z_check_vip_bar_all_loaded(pstEnv);
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int shopping_db_vip_bar_num_def_select_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg)
{
	int i;
	TORMSELECTRES *pstSelectRes;
	VIPBARNUMDEF stVipBarNumDef;
	int iRet = 0;


	if (pstEnv->pstZoneObj->iVipBarNumGetStat != 1)
	{
		return -1;
	}
	
	pstSelectRes = &pstPkg->Body.SelectRes;
		
	for(i=0; i<pstSelectRes->SelectResult.ResultNum ; i++)
	{
		memset(&stVipBarNumDef,0,sizeof(stVipBarNumDef));
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stVipBarNumDef, sizeof(VIPBARNUMDEF)))
		{
			continue;
		}
		/*数据都先弄出来,等扫描cache的时候我们再来做删除操作,清晰点*/

		iRet =   hash_insert_vip_bar_num_def(pstEnv,stVipBarNumDef.Num,(char*)&stVipBarNumDef,sizeof(VIPBARNUMDEF));

	}
	g_LoadedVipBarNumDefNum += pstSelectRes->SelectResult.ResultNum;

	if( g_LoadedVipBarNumDefNum >= g_SelectCountVipBarNumDefNum)
	{
		//读取完成

		pstEnv->pstZoneObj->iVipBarNumGetStat = 2;
		z_check_vip_bar_all_loaded(pstEnv);
	}

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int shopping_db_vip_bar_num_def_select_req( ZONESVRENV* pstEnv,unsigned int iBeginRecord)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	VIPBARNUMDEF stVipBarNumDef;

	memset(&stVipBarNumDef,0,sizeof(stVipBarNumDef));
	
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,-1,&pstPkg->Head, 
								TORM_SELECT_REQ,DB_SELECT_VIP_BAR_NUM_DEF_RES, 
			               			"VipBarNumDef", 
			               			NULL, 
			               			0);/* update 修改点1*/
	
	pstSelectReq->OffSet = iBeginRecord;
	pstSelectReq->Limit = PRE_GET_DB_RECORD;
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	strcpy(pstSelectReq->OperInfo.WhereDef, "where Num != 0 order by Num");
	
	iDst = pstEnv->iRegionDBID;
	
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,&stVipBarNumDef, 
			sizeof(VIPBARNUMDEF),&iDst,pstEnv->iRegionDBProtoVer);
	
}

int z_check_vip_bar_all_loaded(ZONESVRENV *pstEnv)
{
	int i = 0;
	if (pstEnv->pstZoneObj->iVipBarGetStat ==2 &&
		pstEnv->pstZoneObj->iVipBarNumGetStat == 2 &&
		pstEnv->pstZoneObj->iVipBarLevelGetStat == 2 &&
		g_SelectCountVipBarDefNum > 0)
	{
		pstEnv->pstZoneObj->iVipBarDef = 1;//表示全部读取完成并有数据
	}
	else
	{
		return 0;
	}

	for (i=0; i<MAX_PLAYER; i++)
	{
		TMEMBLOCK* pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		Player * pstPlayer = NULL;
		
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (Player *)pstMemBlock->szData;
		
		if(PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus && 
			PLAYER_STATUS_ROLE_NOTHING != pstPlayer->eStatus) 
		{
			continue;
		}

		
		if (pstPlayer->stOnline.stVipBarInfo.iVipBarID)
		{
			player_vip_bar_buff(pstEnv,pstPlayer,FAKE_STATUS_OP_DEL);
		}
	}


	//初始化各吧

	for(i = 0; i < pstEnv->pstVipBarHash->iMax;i++)
	{
		VipBarBucket *pstPos = NULL;
		int iValid = 0;


		pstPos = (VipBarBucket *)sht_pos(pstEnv->pstVipBarHash, i, &iValid);

		if( iValid && pstPos)
		{
			VIPBARDEF *pstBarDef = (VIPBARDEF *)pstPos->szData;
			VIPBARONLINE stBarOnline;

			memset(&stBarOnline,0,sizeof(stBarOnline));

			stBarOnline.VipBarID = pstBarDef->BarID;
			
			hash_insert_vip_bar_online(pstEnv,stBarOnline.VipBarID,(char*)&stBarOnline,sizeof(stBarOnline));
			

		}
	}

	//更新player的vipbar信息
	
	for (i=0; i<MAX_PLAYER; i++)
	{
		TMEMBLOCK* pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		Player * pstPlayer = NULL;
		
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (Player *)pstMemBlock->szData;
		
		if(PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus && 
			PLAYER_STATUS_ROLE_NOTHING != pstPlayer->eStatus) 
		{
			continue;
		}

		
		player_vip_bar_check(pstEnv,pstPlayer);
	}

	
	
	return 0;
}

int z_load_vip_bar_data(ZONESVRENV *pstEnv)
{

	if(INST_MODE_ZONE_SVR != pstEnv->iInstMode)
	{
		return 0;
	}

	g_SelectCountVipBarDefNum = 0;
	g_LoadedVipBarDefNum = 0;
	g_SelectCountVipBarNumDefNum = 0;
	g_LoadedVipBarNumDefNum = 0;
	g_SelectCountVipBarLevelDefNum = 0;
	g_LoadedVipBarLevelDefNum = 0;
	
	pstEnv->pstZoneObj->iVipBarDef = 0;
	
	pstEnv->pstZoneObj->iVipBarGetStat = 0;
	pstEnv->pstZoneObj->iVipBarNumGetStat = 0;
	pstEnv->pstZoneObj->iVipBarLevelGetStat = 0;
	
	g_iVipBarSendTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	

	shopping_db_vip_bar_def_count_req(pstEnv);
	shopping_db_vip_bar_level_def_count_req(pstEnv);
	shopping_db_vip_bar_num_def_count_req(pstEnv);
	
	return 0;
}

int player_vip_bar_check(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	int i = 0;
	int iValid = 0;

	if (!pstEnv->pstZoneObj->iVipBarDef)
	{
		return 0;
	}

	if (pstPlayer->stOnline.stVipBarInfo.iVipBarID)
	{
		player_vip_bar_buff(pstEnv,pstPlayer,FAKE_STATUS_OP_DEL);
	}

	for(i = 0; i < pstEnv->pstVipBarHash->iMax;i++)
	{
		VipBarBucket *pstPos;


		pstPos = (VipBarBucket *)sht_pos(pstEnv->pstVipBarHash, i, &iValid);

		if( iValid && pstPos)
		{
			VIPBARDEF *pstBarDef = (VIPBARDEF *)pstPos->szData;
			int iIp = (int)inet_addr(pstBarDef->IP);
			int iMask = (int)inet_addr(pstBarDef->Mask);
			
			if ((int)(pstPlayer->stConnectInfo.stExtraInfo.IPInfo.ip & iMask) ==
				(iIp & iMask))
			{
				pstPlayer->stOnline.stVipBarInfo.iVipBarID = pstBarDef->BarID;
				pstPlayer->stOnline.stVipBarInfo.iVipBarLevel = pstBarDef->Level;

				player_vip_bar_buff(pstEnv,pstPlayer,FAKE_STATUS_OP_ADD);
				break;
			}
		}
	}
	return 0;

}

int vip_bar_num_chg(ZONESVRENV *pstEnv,int iBarID,int iChg)
{
	VIPBARONLINE * pstOnline = z_find_vip_bar_online(pstEnv,iBarID);
	int iBarNum = -1;

	if (!pstOnline)
	{
		return -1;
	}

	pstOnline->OnlineNum += iChg;//需要保持加1或减1

	if (pstOnline->OnlineNum < 0)
	{
		pstOnline->OnlineNum = 0;
	}
	
	if (pstOnline->OnlineNum > 0)
	{
		VIPBARNUMDEF* pstNum = z_find_vip_bar_num_def(pstEnv,pstOnline->OnlineNum);
		if (pstNum)
		{
			iBarNum = pstNum->Num;
		}
	}
	else
	{
		iBarNum = 0;
	}


	if (pstOnline->OnlineNum < pstOnline->CurEffectNum && iBarNum < 0)
	{
		int i;
		int iPreNum = 0;
		VipBarNumBucket *pstPos = NULL;
		
		for(i = 0; i < pstEnv->pstVipBarNumHash->iMax;i++)
		{
			int iValid = 0;

			pstPos = (VipBarNumBucket *)sht_pos(pstEnv->pstVipBarNumHash, i, &iValid);

			if( iValid && pstPos)
			{
				VIPBARNUMDEF *pstNum = (VIPBARNUMDEF *)pstPos->szData;

				if (pstNum->Num > pstOnline->OnlineNum )
				{
					continue;
				}
				iPreNum = max(iPreNum,pstNum->Num);
			}
		}

		iBarNum = iPreNum;
	}


	if (iBarNum > -1)
	{
		int i = 0;
		pstOnline->CurEffectNum = iBarNum;
		
		//更新player的vipbar num信息
		
		for (i=0; i<MAX_PLAYER; i++)
		{
			TMEMBLOCK* pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
			Player * pstPlayer = NULL;
			
			if (!pstMemBlock->fValid)
			{
				continue;
			}

			pstPlayer = (Player *)pstMemBlock->szData;
			
			

			if (iBarID != pstPlayer->stOnline.stVipBarInfo.iVipBarID)
			{
				continue;
			}


			if(PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus ||
				PLAYER_STATUS_ROLE_NOTHING == pstPlayer->eStatus) 
			{
				player_vip_bar_buff(pstEnv,pstPlayer,FAKE_STATUS_OP_UPDATE);
			}
			
			
		}
	}
	
	return 0;
}

int player_vip_bar_buff_chg_num(ZONESVRENV *pstEnv,Player *pstPlayer,int iOpType,int iChg)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;
	VIPBARINFO* pstVipBarInfo = &pstPlayer->stOnline.stVipBarInfo;

	if (!pstVipBarInfo->iVipBarID )
	{
		return 0;
	}


	switch(iOpType)
	{
	case FAKE_STATUS_OP_ADD:
		{
			if (iChg)
			{
				vip_bar_num_chg(pstEnv,pstVipBarInfo->iVipBarID,1);
			}
		}
		break;
	case FAKE_STATUS_OP_DEL:
		{
			if (iChg)
			{
				vip_bar_num_chg(pstEnv,pstVipBarInfo->iVipBarID,-1);
				memset(pstVipBarInfo,0,sizeof(*pstVipBarInfo));
			}
		}
		break;
	case FAKE_STATUS_OP_UPDATE:
		break;
		
	default:
		return -1;
	}

	memset(pstAction,0,sizeof(*pstAction));

	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_FAKE_STATUS;

	memset(&pstAction->Data.FakeStatus,0,sizeof(pstAction->Data.FakeStatus));
	
	pstAction->Data.FakeStatus.ID = VIP_BAR_STATUS_ID;
	pstAction->Data.FakeStatus.Level = VIP_BAR_STATUS_LEVEL;
	pstAction->Data.FakeStatus.OpType = iOpType;

	pstVipBarInfo->iVipBarExpMul = 0;
	pstVipBarInfo->iVipBarDropMul = 0;

	if (pstVipBarInfo->iVipBarID && MAX_FAKE_STATUS_DYN_VALUE >= 5)
	{
		VIPBARLEVELDEF * pstLevel = z_find_vip_bar_level_def(pstEnv,pstVipBarInfo->iVipBarLevel);
		VIPBARONLINE* pstOnline = z_find_vip_bar_online(pstEnv,pstVipBarInfo->iVipBarID);
		VIPBARNUMDEF * pstNum = NULL;
		if (pstOnline)
		{
			pstNum = z_find_vip_bar_num_def(pstEnv,pstOnline->CurEffectNum);
		}
		if (pstLevel)
		{
			pstVipBarInfo->iVipBarExpMul = pstLevel->ExpRate;
			pstVipBarInfo->iVipBarDropMul = pstLevel->BootyRate;
			
			pstAction->Data.FakeStatus.DynValue[0] = pstVipBarInfo->iVipBarLevel;
			pstAction->Data.FakeStatus.DynValue[1] = pstVipBarInfo->iVipBarExpMul;
			pstAction->Data.FakeStatus.DynValue[2] = pstVipBarInfo->iVipBarDropMul;
			pstAction->Data.FakeStatus.DynValue[3] = 0;
			pstAction->Data.FakeStatus.DynValue[4] = 0;
			if (pstNum)
			{
				pstVipBarInfo->iVipBarExpMul += pstNum->ExExpRate;
				pstVipBarInfo->iVipBarDropMul += pstNum->ExBootyRate;
				
				pstAction->Data.FakeStatus.DynValue[3] = pstNum->ExExpRate;
				pstAction->Data.FakeStatus.DynValue[4] = pstNum->ExBootyRate;
			}
			pstAction->Data.FakeStatus.DynValueNum = 5;
		}
	}

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	if (PLAYER_STATUS_ROLE_NOTHING == pstPlayer->eStatus)
	{
		SSPKG stSSPkg;
		SSCMDREQ* pstCmdReq = &stSSPkg.Body.CmdReq;
		unsigned int unWorldID = 0;
		
		if(pstPlayer->stOnline.stSpanOnline.iStat != SPAN_STAT_CONNECT)
		{
			return -1;
		}
		
		memset(pstCmdReq,0,sizeof(*pstCmdReq));
		pstCmdReq->Cmd = SS_CMD_SYNC_VIP_BAR_INFO;
		STRNCPY(pstCmdReq->szReceiver, pstPlayer->stRoleData.RoleName, sizeof(pstCmdReq->szReceiver));
		
		GET_WORLD_ID(unWorldID,pstEnv->pstAppCtx->iId);
		snprintf(pstCmdReq->szName,sizeof(pstCmdReq->szName),"w%d"BATTLE_NAME_SYMBOL"%s",
					unWorldID,
					pstPlayer->stRoleData.RoleName);


		pstCmdReq->Data.SyncVipBarInfo.VipBarInfo = pstPlayer->stOnline.stVipBarInfo;
		
		Z_SSHEAD_INIT(&stSSPkg.Head, SS_CMD_REQ);
		z_ssmsg_send(pstEnv, &stSSPkg, pstPlayer->stOnline.stSpanOnline.stInfo.stConnect.iDstID);
	}


	return 0;
}

int player_vip_bar_buff(ZONESVRENV *pstEnv,Player *pstPlayer,int iOpType)
{
	player_vip_bar_buff_chg_num(pstEnv,pstPlayer,iOpType,1);
	return 0;
}

int  hash_insert_vip_bar_online(ZONESVRENV *pstEnv,unsigned int uiKey,char* pszData,unsigned int iSize)
{
	int iRet = 0;
	VipBarOnlineBucket *pstPos, stDummy;
	VIPBARONLINE *pstVipBarOnlineOld = NULL;

	if( iSize > sizeof(VipBarOnlineBucket))
	{
		return -1;
	}

	pstVipBarOnlineOld = z_find_vip_bar_online(pstEnv,uiKey);
	if( NULL!= pstVipBarOnlineOld )
	{
		if( memcmp(pszData,pstVipBarOnlineOld,sizeof(VIPBARONLINE)) != 0   ) 
		{
			/* 修改的数据 */
			memcpy( pstVipBarOnlineOld, pszData,sizeof(VIPBARONLINE));
			iRet = 1;
		}
	}
	else
	{
		iRet = 1;
	}
	
	// 不管有没有 都做插入操作
	stDummy.uiKey = uiKey;
	pstPos = (VipBarOnlineBucket *)sht_insert_unique(pstEnv->pstVipBarOnlineHash, &stDummy, hash_cmp_int,
		                        hash_code_int);
	if(pstPos)
	{
		pstPos->uiKey = uiKey;
		memcpy( pstPos->szData, pszData,iSize);
	}
	
	return iRet;
}

VIPBARONLINE * z_find_vip_bar_online(ZONESVRENV *pstEnv,unsigned int uiKey)
{
	VipBarOnlineBucket *pstPos, stDummy;

	stDummy.uiKey = uiKey;
	pstPos = (VipBarOnlineBucket *)sht_find(pstEnv->pstVipBarOnlineHash, &stDummy, hash_cmp_int, hash_code_int);

	if( pstPos != NULL )
	{
		return (VIPBARONLINE *)pstPos->szData;
	}
	
	return  NULL;
}


