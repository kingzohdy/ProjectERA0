/*
**  @file RCSfile
**  general description of this module
**  Id
**  @author Author
**  @date Date
**  @version Revision
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "tapp/tapp.h"
#include "tdr/tdr.h"
#include "tlog/tlog.h"
#include "aas_svr_conf_desc.h"
#include "comm_misc.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "proto_ss.h"
#include "comm/shtable_shm.h"
#include "comm/tmempool.h"
#include "tbus/tbus.h"
#include "tconnapi.h"
#include "aas_svr.h"
#include "entity.h"


static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_aas_svr[];
extern unsigned char g_szMetalib_proto_ss[];

AAS_SVRENV gs_stEnv;
static AAS_SVRRUN_STATUS gs_stStat;

int AasHashCmp( const void * pv1 , const void *pv2 )
{
	AasHash *pstAasHashData1 = (AasHash *)pv1;
	AasHash *pstAasHashData2 = (AasHash *)pv2;

	return pstAasHashData1->iUin - pstAasHashData2->iUin;
}

unsigned int AasHashCode(const void* pvData)
{
	AasHash *pstAasShtData = (AasHash *)pvData;
	
	return (unsigned int)pstAasShtData->iUin;
}

int CheckGameInst(AAS_SVRENV * pstEnv, AasHash* pstAasHash)
{
	int i;

	i = dlist_head( pstEnv->pstAasPool, pstAasHash->iInstList);
	while (i != pstAasHash->iInstList)
	{
		AasOnlineInst * pstOnlineInst = NULL;

		if( i < 0
			||(pstOnlineInst = tmempool_get( pstEnv->pstAasPool, i ) ) == NULL)
		{
			break;
		}

		i = pstOnlineInst->stEntry.iNext;
		
		if( pstEnv->pstAppCtx->stCurr.tv_sec - pstOnlineInst->stSvrInfo.tActive > AAS_TIME_OUT )
		{
			
			dlist_remove( pstEnv->pstAasPool, pstOnlineInst->stEntry.iIdx );
			tmempool_free( pstEnv->pstAasPool, pstOnlineInst->stEntry.iIdx );
			continue;
		}
	}

	// 用户本区均离线
	i = dlist_head( pstEnv->pstAasPool, pstAasHash->iInstList);
	if (i == pstAasHash->iInstList)
	{
		if (pstAasHash->tActive < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			pstAasHash->tOnline  += pstEnv->pstAppCtx->stCurr.tv_sec-pstAasHash->tActive;
		}
		pstAasHash->tActive = pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	
	return 0;
}

AasOnlineInst*  CheckSvrInst( AAS_SVRENV * pstEnv, AasHash* pstAasHash, int iSvrID )
{
	int i;
	
	dlist_foreach( pstEnv->pstAasPool, i, pstAasHash->iInstList)
	{
		AasOnlineInst * pstOnline = NULL;

		if( i < 0
			||(pstOnline = tmempool_get( pstEnv->pstAasPool, i ) ) == NULL)
		{
			break;
		}

		if( pstOnline->stSvrInfo.SvrID == iSvrID)
		{
			pstOnline->stSvrInfo.tActive = pstEnv->pstAppCtx->stCurr.tv_sec;
			return pstOnline;
		}
	}
	
	return NULL;
}

int CheckAasOffline(AAS_SVRENV* pstEnv, AasHash *pstAasHash)
{
	if (pstAasHash->tActive < pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		pstAasHash->tOffline += pstEnv->pstAppCtx->stCurr.tv_sec-pstAasHash->tActive;
	}	
	pstAasHash->tActive = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	if (pstAasHash->tOffline >= AAS_OFFLINE_TIME)
	{
		pstAasHash->tOnline = 0;
		pstAasHash->tOffline = 0;
		
		return 1;
	}

	return 0;
}

AasOnlineInst* gameinst_alloc(AAS_SVRENV* pstEnv)
{
	int iIdx;
	AasOnlineInst *pstRet = NULL;
	
	iIdx = tmempool_alloc(pstEnv->pstAasPool);
	if( iIdx < 0 )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "Aas gameinst alloc error");
		return NULL;
	}

	pstRet = tmempool_get( pstEnv->pstAasPool, iIdx );
	bzero( pstRet, sizeof(*pstRet) );

	dlist_init( pstEnv->pstAasPool, iIdx );

	return pstRet;
}

AasOnlineInst*  InsertSvrInst( AAS_SVRENV * pstEnv, AasHash* pstAasHash, int iSvrID)
{
	AasOnlineInst  *pstInst = NULL;
	
	pstInst = gameinst_alloc(pstEnv);
	if (pstInst == NULL)
	{
		return NULL;
	}

	pstInst->stSvrInfo.SvrID = iSvrID;
	pstInst->stSvrInfo.tActive = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	dlist_push_front( pstEnv->pstAasPool, pstAasHash->iInstList, pstInst->stEntry.iIdx );
	
	return pstInst;
}

int RemoveSvrInst( AAS_SVRENV * pstEnv, AasHash* pstAasHash, int iSvrID )
{
	int i;

	i = dlist_head( pstEnv->pstAasPool, pstAasHash->iInstList);
	while (i != pstAasHash->iInstList)
	{
		AasOnlineInst * pstOnline = NULL;

		if( i < 0
			||(pstOnline = tmempool_get( pstEnv->pstAasPool, i ) ) == NULL)
		{
			break;
		}

		i = pstOnline->stEntry.iNext;
		if( pstOnline->stSvrInfo.SvrID == iSvrID)
		{
			dlist_remove( pstEnv->pstAasPool, pstOnline->stEntry.iIdx );
			tmempool_free( pstEnv->pstAasPool, pstOnline->stEntry.iIdx );
			break;
		}
	}
	
	return 0;
}

AasHash* InsertAasInfo(AAS_SVRENV* pstEnv, int iUin)
{
	AasHash stAasHash, *pstAasHash = NULL;

	stAasHash.iUin = iUin;
	pstAasHash = sht_insert_unique( pstEnv->pstShtAas, &stAasHash, AasHashCmp, AasHashCode );
	if(NULL == pstAasHash)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "AasHash insert  error");
		return NULL;
	}

	pstAasHash->iUin= iUin;
	pstAasHash->tOnline =  0;
	pstAasHash->tOffline =	0;
	pstAasHash->tActive =  pstEnv->pstAppCtx->stCurr.tv_sec;
	
	pstAasHash->iInstList = dlist_create(pstEnv->pstAasPool);
	if (pstAasHash->iInstList < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "Aas gaminst creat  error");
		return NULL;
	}

	return pstAasHash;
}

int aas_send_msg(AAS_SVRENV* pstEnv, AasHash *pstAasHash)
{
	int iSrcID = pstEnv->pstAppCtx->iId;	
	//int iDstID = pstEnv->iRegionID;
	int iDstID = 0;
	char szBuff[sizeof(SSPKG)];
	TDRDATA stNet;
	SSPKG stPkgRes;
	SSAASRES*pstSSAasRes = &stPkgRes.Body.AasRes;

	Z_SSHEAD_INIT(&stPkgRes.Head, AAS_GAME_RES);
	pstSSAasRes->Uin = pstAasHash->iUin;
	pstSSAasRes->tOnline = pstAasHash->tOnline;
	pstSSAasRes->tOffline = pstAasHash->tOffline;

	stNet.pszBuff = szBuff;
	stNet.iBuff = sizeof(szBuff);
	if (0 > ss_pack(pstEnv->pstSSHan, &stNet, &stPkgRes, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "report dir ss_pack_err");
		return -1;
	}
	
	if (0 > tbus_backward(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, stNet.pszBuff, stNet.iBuff, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail");
		return -1;
	}

	tlog_debug(pstEnv->pstLogCat, pstAasHash->iUin, 0, "uin = %d  aas start   online = %d offline = %d", 
					pstAasHash->iUin, pstAasHash->tOnline, pstAasHash->tOffline);

	return 0;
}

int aas_game_active(AAS_SVRENV* pstEnv, int iUin, int iSvrID, int fStart)
{
	int iIdx;
	int fSend = 0;
	AasOnlineInst *pstOnline = NULL;
	AasHash stAasHash, *pstAasHash = NULL;

	stAasHash.iUin = iUin;
	pstAasHash = sht_find(pstEnv->pstShtAas, &stAasHash, AasHashCmp, AasHashCode);
	if (!pstAasHash)
	{
		pstAasHash = InsertAasInfo(pstEnv, iUin);
		if (!pstAasHash)
		{
			return -1;
		}
	}

	iIdx = dlist_head( pstEnv->pstAasPool, pstAasHash->iInstList );
	if (iIdx == pstAasHash->iInstList)
	{
		if (pstAasHash->tActive < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			pstAasHash->tOffline += pstEnv->pstAppCtx->stCurr.tv_sec-pstAasHash->tActive;
		}	
		pstAasHash->tActive = pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	if (pstAasHash->tOffline >= AAS_OFFLINE_TIME)
	{
		pstAasHash->tOnline = 0;
		pstAasHash->tOffline = 0;
	}

	pstOnline = CheckSvrInst( pstEnv, pstAasHash, iSvrID);
	if (!pstOnline)
	{
		fSend = 1;
		pstOnline = InsertSvrInst(pstEnv, pstAasHash, iSvrID);
		if (pstOnline == NULL)
		{
			return -1;
		}
	}
	
	if (pstAasHash->tActive < pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		pstAasHash->tOnline  += pstEnv->pstAppCtx->stCurr.tv_sec-pstAasHash->tActive;
	}
	pstAasHash->tActive = pstEnv->pstAppCtx->stCurr.tv_sec;

	if (fStart || fSend)
	{
		aas_send_msg(pstEnv, pstAasHash);
	}
	
	return 0;
}

int aas_game_start(AAS_SVRENV* pstEnv, int iUin, int iSvrID)
{
	
	if ( 0 > aas_game_active(pstEnv, iUin, iSvrID, 1) )
	{
		return -1;
	}

	return 0;
}

int aas_time_set(AAS_SVRENV* pstEnv, SSAASREQ *pstSSAasReq)
{
	int iIdx;
	AasOnlineInst *pstOnline = NULL;
	AasHash stAasHash, *pstAasHash = NULL;

	stAasHash.iUin = pstSSAasReq->Uin;
	pstAasHash = sht_find(pstEnv->pstShtAas, &stAasHash, AasHashCmp, AasHashCode);
	if (!pstAasHash)
	{
		pstAasHash = InsertAasInfo(pstEnv, pstSSAasReq->Uin);
		if (!pstAasHash)
		{
			return -1;
		}
	}

	iIdx = dlist_head( pstEnv->pstAasPool, pstAasHash->iInstList );
	if (iIdx == pstAasHash->iInstList)
	{
		if (pstAasHash->tActive < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			pstAasHash->tOffline += pstEnv->pstAppCtx->stCurr.tv_sec-pstAasHash->tActive;
		}	
		pstAasHash->tActive = pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	if (pstSSAasReq->tOnline)
	{
		pstAasHash->tOnline = pstSSAasReq->tOnline;
	}

	if (pstSSAasReq->tOffline)
	{
		pstAasHash->tOffline = pstSSAasReq->tOffline;
	}
	pstAasHash->tActive = pstEnv->pstAppCtx->stCurr.tv_sec;

	pstOnline = CheckSvrInst( pstEnv, pstAasHash, pstSSAasReq->SvrID);
	if (!pstOnline)
	{
		pstOnline = InsertSvrInst(pstEnv, pstAasHash, pstSSAasReq->SvrID);
		if (pstOnline == NULL)
		{
			return -1;
		}
	}
	
	aas_send_msg(pstEnv, pstAasHash);
	
	return 0;
}

int aas_game_end(AAS_SVRENV* pstEnv, int iUin, int iSvrID)
{
	AasHash stAasHash, *pstAasHash = NULL;

	stAasHash.iUin = iUin;
	pstAasHash = sht_find(pstEnv->pstShtAas, &stAasHash, AasHashCmp, AasHashCode);
	if (!pstAasHash)
	{
		return 0;
	}

	RemoveSvrInst(pstEnv, pstAasHash, iSvrID);

	if (pstAasHash->tActive < pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		pstAasHash->tOnline  += pstEnv->pstAppCtx->stCurr.tv_sec-pstAasHash->tActive;
	}
	pstAasHash->tActive = pstEnv->pstAppCtx->stCurr.tv_sec;

	return 0;
}

int z_svr_msg(TAPPCTX* pstAppCtx, AAS_SVRENV* pstEnv, char *pszMsg, int iMsgLen)
{
	SSPKG stPkgReq;
	SSAASREQ *pstSSAasReq = &stPkgReq.Body.AasReq;

	UNUSED(pstAppCtx);
	
	if(ss_unpack(pstEnv->pstSSHan, pszMsg, iMsgLen, &stPkgReq, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_unpack fail");
		return -1;
	}

	if (stPkgReq.Head.Cmd != AAS_GAME_REQ)
	{
		return -1;
	}
	
	switch(pstSSAasReq->ActType)
	{
		case AAS_GAME_START:
			aas_game_start(pstEnv, pstSSAasReq->Uin, pstSSAasReq->SvrID);
			break;
		case AAS_GAME_ACTIVE:
			aas_game_active(pstEnv, pstSSAasReq->Uin, pstSSAasReq->SvrID, 0);
			break;
		case AAS_GAME_END:
			aas_game_end(pstEnv, pstSSAasReq->Uin, pstSSAasReq->SvrID);
			break;
		case AAS_TIME_SET:
			aas_time_set(pstEnv, pstSSAasReq);
			break;
		default:
			return -1;
			break;
	}
	
	return 0;
}

int z_tick_aas(AAS_SVRENV* pstEnv, int iCount)
{
	int i;
	LPSHITEM pstItem = NULL;
	AasHash*pstAasHash = NULL;
	static int iCheckPos = 0;

	if (sht_is_empty(pstEnv->pstShtAas))
	{
		return 0;
	}

	for (i = 0; i < iCount; i++) 
	{
		if (iCheckPos >= pstEnv->pstShtAas->iMax) 
		{
			break;
		}
		
		pstItem = SHT_GET_ITEM(pstEnv->pstShtAas, iCheckPos++);
		if ( !pstItem->fValid )
		{
	            continue;
	        }
		
		pstAasHash = (AasHash *)pstItem->szData;
		
		i = dlist_head( pstEnv->pstAasPool, pstAasHash->iInstList);
		if (i != pstAasHash->iInstList)
		{
			CheckGameInst( pstEnv, pstAasHash );
			continue;
		}
		
		if  (CheckAasOffline( pstEnv, pstAasHash ) )
		{
			AasHash stAasHash;
			
			stAasHash.iUin = pstAasHash->iUin;
			sht_remove(pstEnv->pstShtAas, &stAasHash, AasHashCmp, AasHashCode);
		}
	}
	
	if (iCheckPos < pstEnv->pstShtAas->iMax)
	{
		return 0;
	}
	
	iCheckPos = 0;
	
	return 0;
}

int load_shm(AAS_SVRENV *pstEnv)
{
	int iShmID;
	int iIsExist=0;
			
	iShmID	=	shmget(pstEnv->pstConf->ResShmKey, sizeof(ZoneObj), 0666 | IPC_CREAT | IPC_EXCL);
	if( iShmID<0 )
	{
		iIsExist	=	1;
		iShmID		=	shmget(pstEnv->pstConf->ResShmKey, sizeof(ZoneObj), 0666);
	}
	
	if( iShmID<0 )
		return -1;
	
	if (pstEnv->pstZoneObj)
	{
		shmdt(pstEnv->pstZoneObj);
	}

	pstEnv->pstZoneObj =	(ZoneObj *)shmat(iShmID, NULL, 0);
	
	if( (void *) -1  == pstEnv->pstZoneObj )
		return -1;

	if (!iIsExist)
	{
		pstEnv->pstZoneObj->iVersion = OBJ_SHM_VER;
		
		if (0 > tmempool_init(&pstEnv->pstAasPool, MAX_AAS_GAME_INST, GAME_INST_DATA,
							pstEnv->pstZoneObj->sAasInst, sizeof(pstEnv->pstZoneObj->sAasInst)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_init AasInst fail");
			return -1;
		}

		pstEnv->pstShtAas = sht_init(pstEnv->pstZoneObj->sAasSht, sizeof(pstEnv->pstZoneObj->sAasSht),
												AAS_BUCKET, MAX_AAS_PLAYER, sizeof(AasHash));
		if (NULL == pstEnv->pstShtAas)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  Aas hash fail");
			return -1;
		}
	}
	else
	{
		
		if (OBJ_SHM_VER != pstEnv->pstZoneObj->iVersion)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "shm ver=%d", pstEnv->pstZoneObj->iVersion);
			return -1;
		}

		if (0 > tmempool_attach(&pstEnv->pstAasPool, MAX_AAS_GAME_INST, GAME_INST_DATA, 
								pstEnv->pstZoneObj->sAasInst, sizeof(pstEnv->pstZoneObj->sAasInst)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_init item fail");
			return -1;
		}

		pstEnv->pstShtAas = sht_attach(pstEnv->pstZoneObj->sAasSht, sizeof(pstEnv->pstZoneObj->sAasSht),
											AAS_BUCKET, MAX_AAS_PLAYER, sizeof(AasHash));
		if (NULL == pstEnv->pstShtAas)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach  trust hash fail");
			return -1;
		}
	}
	
	pstEnv->iZoneObjShmID = iShmID;

	return 0;
}


int aas_svr_init(TAPPCTX *pstAppCtx, AAS_SVRENV *pstEnv)
{
	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		return -1;
	}

	memset(pstEnv, 0, sizeof(*pstEnv));
	
	pstEnv->pstAppCtx = pstAppCtx;
	
	pstEnv->pstConf = (AAS_SVRCONF *)pstAppCtx->stConfData.pszBuff;

	pstEnv->pstRunStat = (AAS_SVRRUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
	if (NULL == pstEnv->pstRunStat)
	{
		pstEnv->pstRunStat = &gs_stStat;
	}

	if (0 > tapp_get_category(NULL, (void *)(&pstEnv->pstLogCat)))	
	{		
		printf("tapp_get_category run fail\n");
		return -1;	
	}	

	if ( 0 > load_shm(pstEnv) )
	{
		return -1;
	}

	if (0 > tconnapi_initialize(pstAppCtx->pszGCIMKey, pstAppCtx->iBusinessID))
	{
		printf("tbus gcim key %s init fail\n", pstAppCtx->pszGCIMKey);
		return -1;
	}

	//区服务器
	GET_REGION_ENTITY_ID(pstEnv->iRegionID, pstEnv->pstAppCtx->iId, FUNC_REGION_GATE);

	
	//if (0 > tconnapi_create(pstAppCtx->iId, &pstEnv->iTconnapiHandle))
	//{
	//	printf("tbus id %s init fail\n", pstAppCtx->pszId);
	//	return -1;
	//}

	if (0 > ss_han_create(g_szMetalib_proto_ss, &pstEnv->pstSSHan))
	{
		printf("ss_han_create fail\n");
		return -1;
	}
	
	printf("aas_svr start\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "aas_svr start");

	return 0;
}

int aas_svr_proc(TAPPCTX *pstAppCtx, AAS_SVRENV *pstEnv)
{
	int iBusy=0;
	int iLoopNum = 0;
	TBUSADDR iMsgDst ;
	TBUSADDR iMsgSrc ;
	int iMsgLen;
	char *pszMsg;

	while (LOOP_NUM > iLoopNum)
	{
		iLoopNum++;
		
		iMsgDst = pstAppCtx->iId;
		iMsgSrc = TBUS_ADDR_ALL;
		iMsgLen = sizeof(pstEnv->szRecvBuffer);
		pszMsg = &pstEnv->szRecvBuffer[0];
		if ( TBUS_SUCCESS != tbus_recv(pstAppCtx->iBus, &iMsgSrc, &iMsgDst, 
			(void *)pszMsg, (size_t *)&iMsgLen, 0) )
		{
			break;
		}
		iBusy = 1;

		pstEnv->pstRunStat->RecvCltMsg++;

		z_svr_msg(pstAppCtx, pstEnv, pszMsg, iMsgLen);
	}
	
	if (iBusy)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int aas_svr_reload(TAPPCTX *pstAppCtx, AAS_SVRENV *pstEnv)
{
	AAS_SVRCONF   *pstPreConf;

	UNUSED(pstAppCtx);
	
	pstPreConf = (AAS_SVRCONF   *)pstAppCtx->stConfPrepareData.pszBuff;
	
	printf("aas_svr reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "aas_svr reload");

	return 0;
}

int aas_svr_fini(TAPPCTX *pstAppCtx, AAS_SVRENV *pstEnv)
{

	UNUSED(pstAppCtx);
		
	printf("aas_svr finish\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "aas_svr finish");
	
	return 0;
}

int aas_svr_tick(TAPPCTX *pstAppCtx, AAS_SVRENV *pstEnv)
{
	UNUSED(pstAppCtx);
	
	z_tick_aas(pstEnv, 200);
	return 0;
}

int aas_svr_stop(TAPPCTX *pstAppCtx, AAS_SVRENV *pstEnv)
{
	int iReady=0;

	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	
	iReady = 1;
	if (iReady)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int main(int argc, char* argv[])
{
	int iRet;
	void* pvArg	=	&gs_stEnv;

	memset(&gs_stAppCtx, 0, sizeof(gs_stAppCtx));
	memset(&gs_stEnv, 0, sizeof(gs_stEnv));

	gs_stAppCtx.argc	=	argc;
	gs_stAppCtx.argv	=	argv;

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)aas_svr_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_aas_svr;
	gs_stAppCtx.stConfData.pszMetaName = "aas_svrconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);
		
	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)aas_svr_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)aas_svr_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)aas_svr_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)aas_svr_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)aas_svr_stop;
	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	tapp_def_fini(&gs_stAppCtx, pvArg);
	
	return iRet;
}


