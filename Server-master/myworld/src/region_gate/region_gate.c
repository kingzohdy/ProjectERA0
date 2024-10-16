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
#include "region_gate_conf_desc.h"
#include "version.h"
#include "comm/tmempool.h"
#include "comm_misc.h"
#include "comm/shtable_shm.h"
#include "entity.h"
#include "tbus/tbus.h"
#include "tconnapi.h"
#include "proto_ss.h"  
#include "region_gate.h"
#include "../zone/zone_svr/zone_svr.h"

static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_region_gate[];
extern unsigned char g_szMetalib_proto_ss[];

REGION_GATEENV gs_stEnv;
static REGION_GATERUN_STATUS gs_stStat;

#if defined(UNUSED)
#undef UNUSED
#define UNUSED(param) ((void)(param))
#endif

#if defined(CCH)
#undef CCH
#define CCH(array)    ((int)(sizeof(array)/sizeof(array[0])))
#endif


#define BATTLE_QUEUE_TIMEOUT 30  // 战场队列请求超时
#define ZONE_QUEUE_ALIVE_TIME 60 // 战场队列存活时间
//#define BATTLE_HEARTBEAT_INTERVAL 120 // 战场服务器存活超时

int UinHashCmp( const void * pv1 , const void *pv2 )
{
	UinOnlineHash *pstData1 = (UinOnlineHash *)pv1;
	UinOnlineHash *pstData2 = (UinOnlineHash *)pv2;

	return pstData1->iUin - pstData2->iUin;
}

unsigned int UinHashCode(const void* pvData)
{
	UinOnlineHash *pstData = (UinOnlineHash *)pvData;
	
	return (unsigned int)pstData->iUin;
}

UinOnlineInst* uin_worldinst_alloc(REGION_GATEENV* pstEnv)
{
	int iIdx;
	UinOnlineInst *pstRet = NULL;
	
	iIdx = tmempool_alloc(pstEnv->pstUinPool);
	if( iIdx < 0 )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "uin worldinst alloc error");
		return NULL;
	}

	pstRet = tmempool_get( pstEnv->pstUinPool, iIdx );
	bzero( pstRet, sizeof(*pstRet) );

	dlist_init( pstEnv->pstUinPool, iIdx );

	return pstRet;
}

UinOnlineInst*  insert_world_inst(REGION_GATEENV * pstEnv, UinOnlineHash* pstUinHash, int iSvrID, int fStatus)
{
	UinOnlineInst  *pstInst = NULL;
	
	pstInst = uin_worldinst_alloc(pstEnv);
	if (pstInst == NULL)
	{
		return NULL;
	}

	pstInst->stSvrInfo.iSvrID = iSvrID;
	pstInst->stSvrInfo.fStatus = fStatus;
	pstInst->stSvrInfo.tActive = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	dlist_push_front( pstEnv->pstUinPool, pstUinHash->iInstList, pstInst->stEntry.iIdx );
	
	return pstInst;
}

int remove_world_inst(REGION_GATEENV * pstEnv, UinOnlineHash* pstUinHash, int iRegionID )
{
	int i;
	int UinRegionID;
	UinOnlineInst * pstOnline = NULL;
	
	i = dlist_head( pstEnv->pstUinPool, pstUinHash->iInstList);
	while (i != pstUinHash->iInstList)
	{
		if( i < 0
			||(pstOnline = tmempool_get( pstEnv->pstUinPool, i ) ) == NULL)
		{
			break;
		}

		i = pstOnline->stEntry.iNext;

		GET_REGION_ID(UinRegionID, pstOnline->stSvrInfo.iSvrID);
		if( UinRegionID == iRegionID)
		{
			dlist_remove( pstEnv->pstUinPool, pstOnline->stEntry.iIdx );
			tmempool_free( pstEnv->pstUinPool, pstOnline->stEntry.iIdx );
			break;
		}
	}
	
	return 0;
}

UinOnlineInst*  find_world_inst(REGION_GATEENV * pstEnv, UinOnlineHash* pstUinHash, int iRegionID )
{
	int i;
	int UinRegionID = 0;
	UinOnlineInst * pstOnline = NULL;

	i = dlist_head( pstEnv->pstUinPool, pstUinHash->iInstList);
	while (i != pstUinHash->iInstList)
	{
		if( i < 0
			||(pstOnline = tmempool_get( pstEnv->pstUinPool, i ) ) == NULL)
		{
			break;
		}

		i = pstOnline->stEntry.iNext;
		GET_REGION_ID(UinRegionID, pstOnline->stSvrInfo.iSvrID);

		if( UinRegionID == iRegionID)
		{
			return pstOnline;
		}
	}
	
	return NULL;
}

UinOnlineHash* insert_uin_info(REGION_GATEENV* pstEnv, int iUin)
{
	UinOnlineHash stUinHash, *pstUinHash = NULL;

	stUinHash.iUin = iUin;
	pstUinHash = sht_insert_unique( pstEnv->pstShtUin, &stUinHash, UinHashCmp, UinHashCode );
	if(NULL == pstUinHash)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "UinHash insert  error");
		return NULL;
	}

	pstUinHash->iUin= iUin;
	
	pstUinHash->iInstList = dlist_create(pstEnv->pstUinPool);
	if (pstUinHash->iInstList < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "uin world_inst creat  error");
		return NULL;
	}

	return pstUinHash;
}

int region_worldid_binsert(REGION_GATEENV *pstEnv,unsigned int key)
{
	if(pstEnv->iZoneBusIDNum < MAX_REGION_WORLD_NUM )
	{
		if ( !binsert_int(&key, &pstEnv->aiZoneBusID, &pstEnv->iZoneBusIDNum, sizeof(int), 1) )
		{
			 return 0;
		}
	}
	else
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "region_worldid_binsert fail busidnum:%d,define num:%d", 
			pstEnv->iZoneBusIDNum,MAX_REGION_WORLD_NUM );
	}
	
	return 1;
}

int region_worldid_bsearch(REGION_GATEENV *pstEnv,unsigned int key)
{
	int iEqu = 0;
	int iArr_Index = 0;
	iArr_Index = bsearch_int(&key,pstEnv->aiZoneBusID,pstEnv->iZoneBusIDNum, sizeof(int), &iEqu);
	if(iEqu)
	{
		return 0;
	}
	/* 没找到 */
	return 1;
}

int z_load_shm(REGION_GATEENV *pstEnv)
{
	int iShmID;
	int iIsExist=0;
			
	iShmID	=	shmget(pstEnv->pstConf->ObjShmKey, sizeof(RegionObj), 0666 | IPC_CREAT | IPC_EXCL);
	if( iShmID<0 )
	{
		iIsExist	=	1;
		iShmID		=	shmget(pstEnv->pstConf->ObjShmKey, sizeof(RegionObj), 0666);
	}
	
	if( iShmID<0 )
		return -1;
	
	if (pstEnv->pstObj)
	{
		shmdt(pstEnv->pstObj);
	}

	pstEnv->pstObj =	(RegionObj *)shmat(iShmID, NULL, 0);
	
	if( (void *) -1  == pstEnv->pstObj )
		return -1;

	pstEnv->iObjShmID = iShmID;

	if (!iIsExist)
	{
		pstEnv->pstObj->iVersion = REGION_OBJ_SHM_VER;

		if (0 > tmempool_init(&pstEnv->pstUinPool, MAX_UIN_WORLD_INST, UIN_WORLD_INST_DATA,
							pstEnv->pstObj->sUinOnlineInst, sizeof(pstEnv->pstObj->sUinOnlineInst)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_init uinInst fail");
			return -1;
		}

		pstEnv->pstShtUin = sht_init(pstEnv->pstObj->sUinOnlineSht, sizeof(pstEnv->pstObj->sUinOnlineSht),
												UIN_ONLINE_BUCKET, MAX_REGION_UIN_NUM, sizeof(UinOnlineHash));
		if (NULL == pstEnv->pstShtUin)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  uin hash fail");
			return -1;
		}

		memset(pstEnv->pstObj->sGlobalBattles, 0,
		       sizeof(pstEnv->pstObj->sGlobalBattles));
		memset(pstEnv->pstObj->sGlobalPworlds, 0,
		       sizeof(pstEnv->pstObj->sGlobalPworlds));
	}
	else
	{
		if (REGION_OBJ_SHM_VER != pstEnv->pstObj->iVersion)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "shm ver=%d", pstEnv->pstObj->iVersion);
			return -1;
		}

		if (0 > tmempool_attach(&pstEnv->pstUinPool, MAX_UIN_WORLD_INST, UIN_WORLD_INST_DATA, 
								pstEnv->pstObj->sUinOnlineInst, sizeof(pstEnv->pstObj->sUinOnlineInst)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_init uinInst fail");
			return -1;
		}
		
		pstEnv->pstShtUin = sht_attach(pstEnv->pstObj->sUinOnlineSht, sizeof(pstEnv->pstObj->sUinOnlineSht),
											UIN_ONLINE_BUCKET, MAX_REGION_UIN_NUM, sizeof(UinOnlineHash));
		if (NULL == pstEnv->pstShtUin)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach  uin hash fail");
			return -1;
		}
	}
	
	pstEnv->pstGlobalBattles = (GlobalBattles *)pstEnv->pstObj->sGlobalBattles;
	pstEnv->pstGlobalPworlds = (GlobalPworlds *)pstEnv->pstObj->sGlobalPworlds;

	return 0;
}

int z_uin_online_check(REGION_GATEENV* pstEnv, UinOnlineHash *pstUinHash)
{
	int i;
	UinOnlineInst * pstOnline = NULL;
	
	i = dlist_head( pstEnv->pstUinPool, pstUinHash->iInstList);
	while (i != pstUinHash->iInstList)
	{
		if( i < 0
			||(pstOnline = tmempool_get( pstEnv->pstUinPool, i ) ) == NULL)
		{
			break;
		}

		i = pstOnline->stEntry.iNext;
		if( pstOnline->stSvrInfo.tActive + UIN_ONLINE_TIME_OUT < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			dlist_remove( pstEnv->pstUinPool, pstOnline->stEntry.iIdx );
			tmempool_free( pstEnv->pstUinPool, pstOnline->stEntry.iIdx );
			break;
		}
	}

	return 0;
}

int z_active_uin(REGION_GATEENV *pstEnv, int iCount)
{
	int i;
	LPSHITEM pstItem = NULL;
	UinOnlineHash *pstUinHash = NULL;
	static int iCheckPos = 0;

	if (sht_is_empty(pstEnv->pstShtUin))
	{
		return 0;
	}

	for (i = 0; i < iCount; i++) 
	{
		if (iCheckPos >= pstEnv->pstShtUin->iMax) 
		{
			break;
		}
		
		pstItem = SHT_GET_ITEM(pstEnv->pstShtUin, iCheckPos++);
		if ( !pstItem->fValid )
		{
	            continue;
	        }
		
		pstUinHash = (UinOnlineHash *)pstItem->szData;
		
		i = dlist_head( pstEnv->pstUinPool, pstUinHash->iInstList);
		if (i == pstUinHash->iInstList)
		{
			UinOnlineHash stUinHash;
			
			stUinHash.iUin = pstUinHash->iUin;
			sht_remove(pstEnv->pstShtUin, &stUinHash, UinHashCmp, UinHashCode);
			continue;
		}
		
		z_uin_online_check(pstEnv, pstUinHash);
	}
	
	if (iCheckPos < pstEnv->pstShtUin->iMax)
	{
		return 0;
	}
	
	iCheckPos = 0;
	
	return 0;
}

int z_kick_off_acc(REGION_GATEENV *pstEnv, int Uin, int iDstID)
{
	int iSrcID = pstEnv->pstAppCtx->iId;	
	char szBuff[sizeof(SSPKG)];
	TDRDATA stNet;
	SSPKG stPkgRes;
	SSKICKOFFACCOUNT *pstSSKick = &stPkgRes.Body.KickOffAccount;

	Z_SSHEAD_INIT(&stPkgRes.Head, KICK_OFF_ACC);
	pstSSKick->Uin = Uin;
	
	stNet.pszBuff = szBuff;
	stNet.iBuff = sizeof(szBuff);
	if (0 > ss_pack(pstEnv->pstSSHan, &stNet, &stPkgRes, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "report dir ss_pack_err");
		return -1;
	}
	
	if (0 > tbus_send(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, stNet.pszBuff, stNet.iBuff, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail");
		return -1;
	}

	return 0;
}

static int region_sendpkg(REGION_GATEENV *pstEnv, SSPKG *pstPkg, int iCmd,
                          int iDst)
{
	int iSrc = pstEnv->pstAppCtx->iId;
	char szBuf[sizeof(*pstPkg)];
	TDRDATA stNet;

	Z_SSHEAD_INIT(&pstPkg->Head, iCmd);

	stNet.pszBuff = szBuf;
	stNet.iBuff = sizeof(szBuf);
	if(ss_pack(pstEnv->pstSSHan, &stNet, pstPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -1;
	}

	if(tbus_send(pstEnv->pstAppCtx->iBus, &iSrc, &iDst, stNet.pszBuff,
	             stNet.iBuff, 0) < 0)
	{
		char szDstAddr[128];

		STRNCPY(szDstAddr, tbus_addr_ntoa(iDst), CCH(szDstAddr));
		tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail [%s] -> [%s]",
		           tbus_addr_ntoa(iSrc), szDstAddr);
		return -1;
	}

	return 0;
}

int z_acc_heartbeat(REGION_GATEENV *pstEnv, int iUin, int iSvrID, int fStatus)
{
	int iRegionID = 0;
	UinOnlineInst *pstOnline = NULL;
	UinOnlineHash stUinHash, *pstUinHash = NULL;

	stUinHash.iUin = iUin;
	pstUinHash = sht_find(pstEnv->pstShtUin, &stUinHash, UinHashCmp, UinHashCode);
	if (NULL == pstUinHash)
	{
		pstUinHash = insert_uin_info(pstEnv, iUin);
		if (NULL == pstUinHash)
		{
			return -1;
		}
	}

	GET_REGION_ID(iRegionID, iSvrID);
	pstOnline = find_world_inst(pstEnv, pstUinHash, iRegionID);
	if(NULL == pstOnline)
	{
		insert_world_inst(pstEnv, pstUinHash, iSvrID, fStatus);
		return 0;
	}
	//通知游戏用户退出
	else if(pstOnline->stSvrInfo.fStatus != fStatus)
	{
		z_kick_off_acc(pstEnv, pstUinHash->iUin, pstOnline->stSvrInfo.iSvrID);
	}

	pstOnline->stSvrInfo.iSvrID = iSvrID;
	pstOnline->stSvrInfo.fStatus = fStatus;
	pstOnline->stSvrInfo.tActive = pstEnv->pstAppCtx->stCurr.tv_sec;

	return 0;
}

// 寻找负载最低的battle_svr
static BattleStat* region_most_idle_battle_svr(REGION_GATEENV *pstEnv)
{
	static BattleStat kDummy = { -1, -1, 0x7fffffff, 0, };

	BattleStat *pstMostIdle = &kDummy;
	GlobalBattles *pstBattles = pstEnv->pstGlobalBattles;
	int i = 0;
	
	for(i = 0; i < pstBattles->iBattleNum; ++i)
	{
		BattleStat *pstStat = pstBattles->astBattleStat + i;
		if(pstStat->iPlayerNum >= MAX_PLAYER_SOFT)
		{
			continue;
		}
		if(pstStat->tAlive < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			continue;
		}

		// TODO: 检查副本数量限制

		if(pstStat->iPlayerNum < pstMostIdle->iPlayerNum)
		{
			pstMostIdle = pstStat;
		}
	}

	return pstMostIdle == &kDummy ? NULL : pstMostIdle;
}

static int region_queue_unlock(REGION_GATEENV *pstEnv, int iPworldDefID,
                               int iBusID)
{
	GlobalPworlds *pstPworlds = pstEnv->pstGlobalPworlds;
	PworldStat *pstStat = NULL;
	PworldQueue *pstQueue = NULL;
	int i = 0;

	for(i = 0; i < pstPworlds->iPworldDefNum; ++i)
	{
		pstStat = pstPworlds->astPworldStat + i;
		if(pstStat->iPworldDefID == iPworldDefID)
		{
			break;
		}
	}
	
	if(i >= pstPworlds->iPworldDefNum )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "zone from[%s]: can not found pworld define!",
		           tbus_addr_ntoa(pstEnv->iLastSrc));
		return -1;
	}

	for(i = 0; i < pstStat->iQueueNum; ++i)
	{
		pstQueue = pstStat->astRequestQueue + i;
		if(pstQueue->iZoneBusID == iBusID)
		{
			break;
		}
	}
	if(i >= pstStat->iQueueNum)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "zone from[%s]: can not found zone_bus_id!",
		           tbus_addr_ntoa(pstEnv->iLastSrc));
		return -1;
	}
	pstQueue->tInvalid = 0; // 解锁

	if(pstStat->iCampNum <= 0)
	{
		return -1;
	}
	pstQueue->iPlayerNum -= (pstStat->iMaxPlayerNum / pstStat->iCampNum);
	tlog_info(pstEnv->pstLogCat, 0, 0, 
	          "zone from[%s]: battle queue unlock. DefID = %d",
	          tbus_addr_ntoa(pstEnv->iLastSrc), pstStat->iPworldDefID);
	return 0;
}

static int region_queue_release(REGION_GATEENV *pstEnv,
                                const SSBATTLEENTERRES *pstRes)
{
	if(pstRes->Result)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "zone from[%s]: player enter battle fail!",
		           tbus_addr_ntoa(pstEnv->iLastSrc));
		return -1;
	}

	return region_queue_unlock(pstEnv, pstRes->BattleCampInfo.PworldDefID,
	                           pstEnv->iLastSrc);
}

static int region_create_pworld_res(REGION_GATEENV *pstEnv,
                                    const SSBATTLECREATEPWORLDRES *pstRes)
{
	/*
	SSPKG stPkg;
	SSBATTLEENTERREQ *pstReq = &stPkg.Body.CreatePworldInfo;
	int i = 0;

	if(pstRes->Result)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "battle from[%s]: create pworld fail!",
		           tbus_addr_ntoa(pstEnv->iLastSrc));

		// 创建副本失败解锁全部
		i = pstRes->BattlePworldInfo.CampNum;
		while(i--)
		{
			region_queue_unlock(pstEnv,
			             pstRes->BattlePworldInfo.BattleCampInfo[i].PworldDefID,
			             pstRes->BattlePworldInfo.BattleCampInfo[i].ZoneBusID);
		}
		return -1;
	}

	// TODO: 检查回复的包

	i = pstRes->BattlePworldInfo.CampNum;
	while(i--)
	{
		int iDst = pstRes->BattlePworldInfo.BattleCampInfo[i].ZoneBusID;
		char szDstAddr[128];

		pstReq->BattleCampInfo = pstRes->BattlePworldInfo.BattleCampInfo[i];

		// 通知zone_svr,副本已建立,可以进入了
		if(region_sendpkg(pstEnv, &stPkg, REGION_CREATE_PWORLD_INFO, iDst) < 0)
		{
			return -1;
		}

		STRNCPY(szDstAddr, tbus_addr_ntoa(iDst), CCH(szDstAddr));
		tlog_info(pstEnv->pstLogCat, 0, 0, "battle from[%s]: create pworld "
		          "succeeded, notify zone[%s], DefID = %d",
		          tbus_addr_ntoa(pstEnv->iLastSrc), szDstAddr,
		          pstReq->BattleCampInfo.PworldDefID);
	}
	*/
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstRes);	//add by paraunused.pl
}

static int region_create_pworld_req(REGION_GATEENV *pstEnv, PworldStat *pstStat,
                                    const int *aiIdxs, int iNum)
{
	const int kNumPerCamp = pstStat->iMaxPlayerNum / pstStat->iCampNum;
	int i = 0;
	SSPKG stPkg;
	SSBATTLECREATEPWORLDREQ *pstReq = &stPkg.Body.BattleCreatePworldReq;
	BattleStat *pstBattleStat = region_most_idle_battle_svr(pstEnv);
	if(!pstBattleStat)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "can not found any idle battle_svr!");
		return -1;
	}
	if(iNum != pstStat->iCampNum)
	{
		return -1; // 逻辑错误
	}

	pstReq->PworldDefID = pstStat->iPworldDefID;
	pstReq->CampNum = pstStat->iCampNum;
	i = pstReq->CampNum;
	while(i--)
	{
		PworldQueue *pstQueue = pstStat->astRequestQueue + aiIdxs[i];
		pstReq->BattleCreateZoneInfo[i].ZoneBusID = pstQueue->iZoneBusID;

		pstQueue->iPlayerNum -= kNumPerCamp;
		pstQueue->tInvalid = pstEnv->pstAppCtx->stCurr.tv_sec +
		                     BATTLE_QUEUE_TIMEOUT;

		tlog_info(pstEnv->pstLogCat, 0, 0, "zone [%s]: battle queue locked. "
		          "DefID = %d", tbus_addr_ntoa(pstQueue->iZoneBusID),
		          pstStat->iPworldDefID);
	}

	tlog_info(pstEnv->pstLogCat, 0, 0, "create a new battle pworld to[%s] "
	          "DefID = %d MaxPlayer = %d",
	          tbus_addr_ntoa(pstBattleStat->iBattleBusID),
	          pstStat->iPworldDefID, pstStat->iMaxPlayerNum);

	++pstBattleStat->iPworldNum; // 预先+1防止重复创建副本
	// 通知battle_svr建立副本
	return region_sendpkg(pstEnv, &stPkg, BATTLE_CREATE_PWORLD_REQ,
	                      pstBattleStat->iBattleBusID);
}

static int region_try_create_pworld(REGION_GATEENV *pstEnv,
                                    PworldStat *pstPworldStat,
                                    int iFirstIdx)
{
	int i = 0, k = 0, aiIdxs[MAX_PWORLD_CAMP];
	const int kNumPerCamp = pstPworldStat->iMaxPlayerNum /
	                        pstPworldStat->iCampNum;
	
	aiIdxs[k++] = iFirstIdx;
	for(i = 0; i < pstPworldStat->iQueueNum; ++i)
	{
		PworldQueue *pstQueue = pstPworldStat->astRequestQueue + i;
		if(i == iFirstIdx)
		{
			continue;
		}
		if(!pstQueue->tInvalid && pstQueue->iPlayerNum >= kNumPerCamp)
		{
			// 排队人数达到了副本要求的人数
			aiIdxs[k++] = i;
			if(k == pstPworldStat->iCampNum) // 凑齐所有的阵营
			{
				// 创建副本了
				region_create_pworld_req(pstEnv, pstPworldStat, aiIdxs, k);
				break;
			}
		}
	}
	return 0;
}

static int region_battle_queue_info(REGION_GATEENV *pstEnv,
                                    const SSPWORLDQUEUEINFOREQ *pstReq)
{
	GlobalPworlds *pstPworlds = pstEnv->pstGlobalPworlds;
	PworldStat *pstStat = NULL;
	PworldQueue *pstQueue = NULL;
	int i = 0;

	if(pstReq->CampNum <= 0 || pstReq->PworldPlayerNum <= 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "zone report from[%s]: pworld define info error; CampNum"
		           " = %d, PworldPlayerNum = %d, PworldNum = %d",
		           tbus_addr_ntoa(pstEnv->iLastSrc), pstReq->CampNum,
		           pstReq->PworldPlayerNum, pstReq->PworldNum);
		return -1;
	}
	
	for(i = 0; i < pstPworlds->iPworldDefNum; ++i)
	{
		pstStat = pstPworlds->astPworldStat + i;
		if(pstStat->iPworldDefID == pstReq->PworldDefID)
		{
			pstStat->iCampNum = pstReq->CampNum;
			pstStat->iMaxPlayerNum = pstReq->PworldPlayerNum;
			pstStat->iMaxPworldNum = pstReq->PworldNum;
			break;
		}
	}

	if(i >= pstPworlds->iPworldDefNum)
	{
		if(pstPworlds->iPworldDefNum >= CCH(pstPworlds->astPworldStat))
		{
			// 达到上限了
			tlog_error(pstEnv->pstLogCat, 0, 0,
			           "pworld defid may error: %d", pstReq->PworldDefID);
			return -1;
		}
		pstStat = &pstPworlds->astPworldStat[pstPworlds->iPworldDefNum++];
		pstStat->iPworldDefID = pstReq->PworldDefID;
		pstStat->iQueueNum = 0;
		pstStat->iCampNum = pstReq->CampNum;
		pstStat->iMaxPlayerNum = pstReq->PworldPlayerNum;
		pstStat->iMaxPworldNum = pstReq->PworldNum;
	}

	for(i = 0; i < pstStat->iQueueNum; ++i)
	{
		pstQueue = pstStat->astRequestQueue + i;
		if(pstQueue->iZoneBusID == pstEnv->iLastSrc)
		{
			break;
		}
	}
	if(i >= pstStat->iQueueNum)
	{
		if(pstStat->iQueueNum >= CCH(pstStat->astRequestQueue))
		{
			// 达到上限了
			tlog_error(pstEnv->pstLogCat, 0, 0,
			           "zone_svr bus_id may error: %d", pstReq->ZoneBusID);
			return -1;
		}
		i = pstStat->iQueueNum;
		pstQueue = &pstStat->astRequestQueue[pstStat->iQueueNum++];
	}

	// 排队信息处于无效状态照样更新
	pstQueue->iPlayerNum = pstReq->QueuerNum;
	pstQueue->iZoneBusID = pstEnv->iLastSrc;
	pstQueue->tAlive = pstEnv->pstAppCtx->stCurr.tv_sec + ZONE_QUEUE_ALIVE_TIME;

	tlog_info(pstEnv->pstLogCat, 0, 0,
	          "zone report from[%s]: PworldDefID = %d, PlayerNum = %d, "
	          "Invalid = %d, Alive = %d", tbus_addr_ntoa(pstQueue->iZoneBusID),
	          pstStat->iPworldDefID, pstQueue->iPlayerNum, pstQueue->tInvalid,
	          pstQueue->tAlive);

	if(!pstQueue->tInvalid &&
	   pstQueue->iPlayerNum >= pstStat->iMaxPlayerNum / pstStat->iCampNum)
	{
		region_try_create_pworld(pstEnv, pstStat, i);
	}
	return 0;
}

static int region_battle_report_info(REGION_GATEENV *pstEnv,
                                     const SSBATTLEINFOSIMPLEREQ *pstReq)
{
	GlobalBattles *pstBattles = pstEnv->pstGlobalBattles;
	BattleStat *pstStat = NULL;
	int i = 0;
	for(i = 0; i < pstBattles->iBattleNum; ++i)
	{
		pstStat = pstBattles->astBattleStat + i;
		if(pstStat->iBattleBusID == pstEnv->iLastSrc)
		{
			break;
		}
	}
	
	if(i >= pstBattles->iBattleNum)
	{
		if(pstBattles->iBattleNum >= CCH(pstBattles->astBattleStat))
		{
			// 达到上限了
			if(NULL != pstStat)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "battle bus_id may error: %d",
			           pstStat->iBattleBusID);
			}
			return -1;
		}
		pstStat = &pstBattles->astBattleStat[pstBattles->iBattleNum++];
		pstStat->iBattleBusID = pstEnv->iLastSrc;
	}
       if( NULL == pstStat)
	{
		return -1;
	}
	pstStat->iPworldNum = pstReq->PworldInstNum;
	pstStat->iPlayerNum = pstReq->PlayerNum;
	// 更新存活时间
	pstStat->tAlive = pstEnv->pstAppCtx->stCurr.tv_sec +
	                  BATTLE_HEARTBEAT_INTERVAL * 2;

	tlog_info(pstEnv->pstLogCat, 0, 0,
	          "battle report from[%s]: PworldNum = %d, PlayerNum = %d",
	          tbus_addr_ntoa(pstStat->iBattleBusID), pstStat->iPworldNum,
	          pstStat->iPlayerNum);
	return 0;
}

static int region_active_battle(REGION_GATEENV *pstEnv, int iCount)
{
	static int iCheckPos = 0;
	GlobalPworlds *pstPworlds = pstEnv->pstGlobalPworlds;
	PworldStat *pstPworldStat = NULL;
	PworldQueue *pstQueue = NULL;
	int i = 0;

	if((iCheckPos++) % iCount != 0)
	{
		return 0;
	}
	iCheckPos = (iCheckPos >= iCount) ? 0 : iCheckPos;

	if(!pstPworlds->iPworldDefNum)
	{
		return 0;
	}

	pstPworlds->iItemSpace = (pstPworlds->iItemSpace + 1) %
	                         pstPworlds->iPworldDefNum;
	pstPworldStat = &pstPworlds->astPworldStat[pstPworlds->iItemSpace];
	if(!pstPworldStat->iQueueNum)
	{
		return 0;
	}

	for(i = 0; i < pstPworldStat->iQueueNum; ++i)
	{
		pstQueue = pstPworldStat->astRequestQueue + i;
		if(pstQueue->tInvalid &&
		   pstQueue->tInvalid < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			pstQueue->tInvalid = 0; // 超时了!

			tlog_info(pstEnv->pstLogCat, 0, 0,
			          "zone from[%s]: queue request timed out! unlock it. "
			          "PworldDefID = %d", tbus_addr_ntoa(pstQueue->iZoneBusID),
			          pstPworldStat->iPworldDefID);
		}

		if(pstQueue->tAlive &&
		   pstQueue->tAlive < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			pstQueue->tAlive = 0;
			pstQueue->iPlayerNum = 0; // 存活时间到

			tlog_info(pstEnv->pstLogCat, 0, 0,
			          "zone from[%s]: queue dead! clear it. PworldDefID = %d",
			          tbus_addr_ntoa(pstQueue->iZoneBusID),
			          pstPworldStat->iPworldDefID);
		}
	}
	return 0;
}

static int region_alive_battle(REGION_GATEENV *pstEnv, int iCount)
{
	static int iCheckPos = 0;
	const GlobalBattles *pstBattles = pstEnv->pstGlobalBattles;
	int i = 0;

	if((iCheckPos++) % iCount != 0)
	{
		return 0;
	}
	iCheckPos = (iCheckPos >= iCount) ? 0 : iCheckPos;

	i = pstBattles->iBattleNum;
	while(i--)
	{
		const BattleStat *pstStat = pstBattles->astBattleStat + i;
		if(pstStat->tAlive < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			tlog_info(pstEnv->pstLogCat, 0, 0, "battle_svr [%s]: is dead!",
			          tbus_addr_ntoa(pstStat->iBattleBusID));
		}
	}
	return 0;
}

int region_gate_init(TAPPCTX *pstAppCtx, REGION_GATEENV *pstEnv)
{
	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		return -1;
	}
	pstEnv->pstConf = (REGION_GATECONF *)pstAppCtx->stConfData.pszBuff;

	pstEnv->pstAppCtx = pstAppCtx;

	pstEnv->pstRunStat = (REGION_GATERUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
	if (NULL == pstEnv->pstRunStat)
	{
		pstEnv->pstRunStat = &gs_stStat;
	}

	memset(pstEnv->aiZoneBusID,0,sizeof(int)*MAX_REGION_WORLD_NUM);
	if(ss_han_create(g_szMetalib_proto_ss, &pstEnv->pstSSHan) < 0)
	{
		return -1;
	}
	
	GET_REGION_ENTITY_ID(pstEnv->iAasSvrID, pstEnv->pstAppCtx->iId, FUNC_ASS_SVR);
	GET_REGION_ENTITY_ID(pstEnv->iBillID, pstEnv->pstAppCtx->iId, FUNC_BILL_SVR);
	GET_REGION_ENTITY_ID(pstEnv->iInformerID, pstEnv->pstAppCtx->iId, FUNC_INFORMER_SVR);
	GET_REGION_ENTITY_ID(pstEnv->iServiceID, pstEnv->pstAppCtx->iId, FUNC_SERVICE_SVR);

	if (0 > tconnapi_initialize(pstAppCtx->pszGCIMKey, pstAppCtx->iBusinessID))
	{
		printf("tbus gcim key %s init fail\n", pstAppCtx->pszGCIMKey);
		return -1;
	}

	if (0 > tapp_get_category(NULL, (void *)(&pstEnv->pstLogCat)))	
	{		
		printf("tapp_get_category run fail\n");
		return -1;	
	}	

	if (0 > z_load_shm(pstEnv))
	{
		return -1;
	}
	
	printf("region_gate start\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "region_gate start");

	return 0;
}

int region_propsgift2zone(REGION_GATEENV *pstEnv, SSPKG *pstPkg,int iExcept)
{
	unsigned int iDstList[1024];
	int iVecCnt = 1024;
	int i;
	unsigned char ucEntity = 0;
	//SSPKG stPkg;
	//SSPROPGIFTBILLRES *pstPropGiftBillRes = &stPkg.Body.PropGiftBillRes;
	TDRDATA stNet;
	int iDst;

	//stPkg.Head.Cmd = PROP_GIFT_BILL_RES;
	//stPkg.Head.HeadLen = sizeof(SSPKGHEAD);
	//stPkg.Head.Magic = PROTOCOL_MAGIC;
	//stPkg.Head.Ver = TDR_METALIB_PROTO_SS_VERSION;

	//pstPropGiftBillRes->PropGifts = *pstPropsGift;
	//STRNCPY(pstPropGiftBillRes->Pass9Account, pszPass9, sizeof(pstPropGiftBillRes->Pass9Account));

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
		if (ucEntity == FUNC_ZONE_SVR)
		{
			iDst = iDstList[i];
			if (iExcept == iDst)
			{
				continue;
			}
			
			tbus_send(pstEnv->pstAppCtx->iBus,  &pstEnv->pstAppCtx->iId, &iDst, stNet.pszBuff, stNet.iBuff, 0);
		}
	}
	return 0;
}

int region_chat_span(REGION_GATEENV *pstEnv, SSPKG *pstPkg, unsigned int iSrcID)
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

int region_gate_router(REGION_GATEENV *pstEnv,char *pszMsg,int iMsgLen,TBUSADDR iMsgSrc )
{
	int iDstID;
	SSPKG stPkgReq;
	int iSrcID = pstEnv->pstAppCtx->iId;

	if(ss_unpack(pstEnv->pstSSHan, pszMsg, iMsgLen, &stPkgReq, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_unpack fail");
		return -1;
	}

	switch(stPkgReq.Head.Cmd)
	{
	case AAS_GAME_REQ:
		/* 记录下world zone_svr的busid以备用 */
		region_worldid_binsert(pstEnv,iMsgSrc);
		
		/* zone_svr -->aas的请求数据 */
		iDstID = pstEnv->iAasSvrID;
		tbus_forward(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, pszMsg,
		             iMsgLen, 0);
		break;
	case AAS_GAME_RES:
		/* zone_svr <--aas的返回数据 */
		iDstID = 0;
		tbus_backward(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, pszMsg,
		              iMsgLen, 0);
		break;
	case CASH_BILL_REQ:
	case QUERY_BILL_REQ:
	case DEC_BILL_REQ:
	case USER_QUERY_BILL_REQ:
	case BILL_FREEZE_REQ:
	case BILL_QUERY_PROPSDEF_REQ:
	case BILL_QUERY_PROPSGIFT_REQ:
	case BILL_SET_PROPSGIFT_REQ:
	case BILL_CASH_PROPSGIFT_REQ:
	case ACC_GIFT_LIST_REQ:
	case ACC_GIFT_INSERT_REQ:
	case PROP_GIFT_REQ:
	case SS_CDKEY_REQ://xxx -> service
	case GODCOIN_CONSUME_LOG_REQ:
		/* xxx --> bill的请求*/
		iDstID = pstEnv->iBillID;
		tbus_forward(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, pszMsg,
		             iMsgLen, 0);
		break;
	case CASH_BILL_RES:
	case QUERY_BILL_RES:
	case DEC_BILL_RES:
	case USER_QUERY_BILL_RES:
	case BILL_FREEZE_RES:
	case BILL_QUERY_PROPSDEF_RES:
	case BILL_QUERY_PROPSGIFT_RES:
	case BILL_SET_PROPSGIFT_RES:
	case BILL_CASH_PROPSGIFT_RES:
	case ACC_GIFT_LIST_RES:
	case ACC_GIFT_INSERT_RES:
	case PROP_GIFT_RES:
	case SS_CDKEY_RES://service -> xxx
	case GODCOIN_CONSUME_LOG_RES:
		/* xxx <--bill的应答*/
		iDstID = 0;
		tbus_backward(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, pszMsg,
		              iMsgLen, 0);
		break;
	case MUD_HEARTBEAT:
		z_acc_heartbeat(pstEnv, stPkgReq.Body.MudHeartBeat.Uin, iMsgSrc,
		                MUD_SVR_LOGIN);
		break;
	case ZONE_HEARTBEAT:
		z_acc_heartbeat(pstEnv, stPkgReq.Body.ZoneHeartBeat.Uin, iMsgSrc,
		                ZONE_SVR_LOGIN);
		break;
	case BATTLE_QUEUE_INFO: // zone_svr -> region_gate
		region_battle_queue_info(pstEnv, &stPkgReq.Body.ZoneQueueInfo);
		break;
	case BATTLE_REPORT_INFO: // battle_svr -> region_gate
		region_battle_report_info(pstEnv, &stPkgReq.Body.BattleInfoSimpleReq);
		break;
	case BATTLE_CREATE_PWORLD_RES: // region_gate -> battle_svr
		region_create_pworld_res(pstEnv, &stPkgReq.Body.BattleCreatePworldRes);
		break;
	case ZONE_UNLOCK_NOTIFY: // zone_svr -> region_gate
		region_queue_release(pstEnv, &stPkgReq.Body.UnLock);
		break;
	case PROP_GIFT_BILL_RES:
		region_propsgift2zone(pstEnv, &stPkgReq,0);
		break;
	case INFORMER_GET_ACCOUNT_REQ://xxx -> service
		iDstID = pstEnv->iServiceID;
		tbus_forward(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, pszMsg,
		             iMsgLen, 0);
		break;
	case INFORMER_GET_ACCOUNT_RES://service -> xxx
		iDstID = 0;
		tbus_backward(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, pszMsg,
		              iMsgLen, 0);
		break;

	case SS_FILL_ID_CARD_REQ://xxx -> service
		iDstID = pstEnv->iServiceID;
		tbus_forward(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, pszMsg,
		             iMsgLen, 0);
		break;
	case SS_FILL_ID_CARD_RES://service -> xxx
		iDstID = 0;
		tbus_backward(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, pszMsg,
		              iMsgLen, 0);
		break;

	case SS_CMD_REQ://xxx -> zones SS_CMD_UIN_FLAG_CHANGE
		region_propsgift2zone(pstEnv, &stPkgReq,iMsgSrc);		
		break;
	case SS_CHAT:
		region_chat_span(pstEnv, &stPkgReq, iMsgSrc);
		break;

	case SS_SMS_REQ://xxx -> service
		iDstID = pstEnv->iServiceID;
		tbus_forward(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, pszMsg,
		             iMsgLen, 0);
		break;
	case SS_SMS_RES://service -> xxx
		iDstID = 0;
		tbus_backward(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, pszMsg,
		              iMsgLen, 0);
		break;
	case SS_MSG_TRANS:
		region_chat_span(pstEnv, &stPkgReq, iMsgSrc);
		break;
#if 0
	case SS_CDKEY_REQ://xxx -> service
		iDstID = pstEnv->iServiceID;
		tbus_forward(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, pszMsg,
		             iMsgLen, 0);
		break;
	case SS_CDKEY_RES://service -> xxx
		iDstID = 0;
		tbus_backward(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, pszMsg,
		              iMsgLen, 0);
		break;
#endif

	default:
		return -1;
		break;
	}

#if 0
	if ( AAS_GAME_REQ == stPkgReq.Head.Cmd )
	{
		/* 记录下world zone_svr的busid以备用 */
		region_worldid_binsert(pstEnv,iMsgSrc);
		
		/* zone_svr -->aas的请求数据 */
		iDstID = pstEnv->iAasSvrID;
		tbus_forward(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, pszMsg, iMsgLen, 0);
	}
	else if(AAS_GAME_RES == stPkgReq.Head.Cmd )
	{
		/* zone_svr <--aas的返回数据 */
		iDstID = 0;
		tbus_backward(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, pszMsg, iMsgLen, 0);
	}
	else if(CASH_BILL_REQ == stPkgReq.Head.Cmd ||
		        QUERY_BILL_REQ == stPkgReq.Head.Cmd ||
		        DEC_BILL_REQ    == stPkgReq.Head.Cmd ||
		        USER_QUERY_BILL_REQ == stPkgReq.Head.Cmd ||
		        BILL_FREEZE_REQ == stPkgReq.Head.Cmd)
	{
		/* informer --> bill的请求*/
		iDstID = pstEnv->iBillID;
		tbus_forward(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, pszMsg, iMsgLen, 0);
	}
	else if ( CASH_BILL_RES == stPkgReq.Head.Cmd ||
		        QUERY_BILL_RES == stPkgReq.Head.Cmd ||
		        DEC_BILL_RES     ==  stPkgReq.Head.Cmd ||
		        USER_QUERY_BILL_RES == stPkgReq.Head.Cmd ||
		        BILL_FREEZE_RES == stPkgReq.Head.Cmd )
	{
		/* informer <--bill的应答*/
		iDstID = 0;
		tbus_backward(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, pszMsg, iMsgLen, 0);
	}
	else if(MUD_HEARTBEAT== stPkgReq.Head.Cmd )
	{
		z_acc_heartbeat(pstEnv, stPkgReq.Body.MudHeartBeat.Uin, iMsgSrc, MUD_SVR_LOGIN);
	}
	else if(ZONE_HEARTBEAT == stPkgReq.Head.Cmd )
	{
		z_acc_heartbeat(pstEnv, stPkgReq.Body.ZoneHeartBeat.Uin, iMsgSrc, ZONE_SVR_LOGIN);
	}
	else 
	{
		return -1;
	}
#endif
	return 0;
}

int region_gate_proc(TAPPCTX *pstAppCtx, REGION_GATEENV *pstEnv)
{
	int iBusy=0;
	int iLoopNum = 0;
	TBUSADDR iMsgDst ;
	//TBUSADDR iMsgSrc ;
	int iMsgLen;
	char *pszMsg;

	while (100 > iLoopNum)
	{
		iLoopNum++;
		
		iMsgDst = pstAppCtx->iId;
		//iMsgSrc = TBUS_ADDR_ALL;
		pstEnv->iLastSrc = TBUS_ADDR_ALL;
		iMsgLen = sizeof(pstEnv->szRecvBuffer);
		pszMsg = &pstEnv->szRecvBuffer[0];
		if(TBUS_SUCCESS != tbus_recv(pstAppCtx->iBus, &pstEnv->iLastSrc,
			&iMsgDst, (void *)pszMsg, (size_t *)&iMsgLen, 0))
		{
			break;
		}
		iBusy = 1;
		
		region_gate_router(pstEnv, pszMsg, iMsgLen, pstEnv->iLastSrc);
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

int region_gate_reload(TAPPCTX *pstAppCtx, REGION_GATEENV *pstEnv)
{
	REGION_GATECONF   *pstPreConf;
	
	pstPreConf = (REGION_GATECONF   *)pstAppCtx->stConfPrepareData.pszBuff;
	
	printf("region_gate reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "region_gate reload");

	return 0;
}

int region_gate_fini(TAPPCTX *pstAppCtx, REGION_GATEENV *pstEnv)
{
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);

	if (pstEnv->cClearShm)
	{
		tshmdt(pstEnv->pstObj);
		tshmclose(pstEnv->iObjShmID, TSHMF_DELETE);
	}
	
	printf("region_gate finish\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "region_gate finish");
	return 0;
}

int region_gate_tick(TAPPCTX *pstAppCtx, REGION_GATEENV *pstEnv)
{
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);

	z_active_uin(pstEnv, 500);
	region_active_battle(pstEnv, 500);
	region_alive_battle(pstEnv, 500);
	return 0;
}

int region_gate_stop(TAPPCTX *pstAppCtx, REGION_GATEENV *pstEnv)
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

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)region_gate_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_region_gate;
	gs_stAppCtx.stConfData.pszMetaName = "region_gateconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);
		
	iRet = tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)region_gate_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)region_gate_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)region_gate_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)region_gate_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)region_gate_stop;
	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	if (TAPP_EXIT_QUIT == tapp_is_exit())
	{
		gs_stEnv.cClearShm = 0;
	}
	else if (TAPP_EXIT_STOP == tapp_is_exit())
	{
		gs_stEnv.cClearShm = 1;
	}

	tapp_def_fini(&gs_stAppCtx, pvArg);
	
	return iRet;
}


