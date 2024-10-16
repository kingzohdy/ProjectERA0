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
#include "pal/tresource.h"
#include "tapp/tapp.h"
#include "tdr/tdr.h"
#include "tlog/tlog.h"
#include "dir_svr_conf_desc.h"
#include "version.h"
#include "black_white.h"
#include "tconnapi.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "proto_ss.h"
#include "comm/tini.h"
#include "tbus/tbus.h"
#include "cs_net.h"
#include "entity.h"
#include "comm_misc.h"
#include "dirreport.h"

#define UNUSED( var ) ((void)var)
#define LOOP_NUM 100
#define CS_MAX_MSG CS_MAX_BODY+sizeof(CSPKGHEAD)+sizeof(TFRAMEHEAD)
#define DIR_TIME_OUT  60


struct tagDir_SvrEnv
{
	DIR_SVRCONF *pstConf;
	DIR_SVRRUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	BlackWhiteHandle *pstBlackWhiteH;
	int iInnerSock; //for zone report online player num;
	int iBakSock;
	int iSlaveSockNum;
	int aiSlaveSock[MAX_DIR_SLAVE];
	unsigned long int iTconnapiHandle;
	CSDIRWORLDLIST stWorldList;
	time_t astWorldActive[MAX_WORLD_NUM];
	time_t astWorldMudActive[MAX_WORLD_NUM];
	NETHANDLE *pstCSHan; 
	SSHANDLE *pstSSHan; 
	char szRecvBuffer[CS_MAX_MSG];
	char szSendBuffer[CS_MAX_MSG];
	int iConnID;
	unsigned long int hReportHandle;
	
	int iReportSock;
	int iSendOnlineTime;
	
	int WorldNum;                       
       DIRINFODB DirInfo[MAX_WORLD_NUM]; 

	int iXmlTime;
};

typedef struct tagDir_SvrEnv		DIR_SVRENV;
typedef struct tagDir_SvrEnv		*LDIR_SVRENV;


static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_dir_svr[];
extern unsigned char g_szMetalib_proto_cs[];
extern unsigned char g_szMetalib_proto_ss[];


DIR_SVRENV gs_stEnv;
static DIR_SVRRUN_STATUS gs_stStat;

#define STAT_FLAG_RECOMMEND 0x10


typedef struct  tagWorldEntry
{
    int   iWorldID ;
    char  szWorldName[WORLD_NAME_LEN];
	int   iRecommend;
	char  RegionID;
}WorldEntry ;

typedef struct tagWorldList
{
   int iWorldNum ;
   WorldEntry astWorld[MAX_WORLD_NUM] ;	
}WORLDLIST;


typedef struct tagRegionEntry
{
	char RegionID;
	char szRegionName[WORLD_NAME_LEN];
	char szUrl[REGION_UPDATA_URL_LEN];
	unsigned int StatFlag;
}RegionEntry;

typedef struct tagRegionList
{
	int iRegionNum;
	RegionEntry astRegion[MAX_BIGWORLD_NUM];
	
}REGIONLIST;

TINISECTIONDESC astRegionDesc[] =
{
	{"REGION" , TINI_SECTION_ARRAY , offsetof(REGIONLIST, astRegion) ,
		{
			{"RegionID" , TINI_INT , offsetof(RegionEntry , RegionID) , "0", 0, 0 },
			{"RegionName" , TINI_STRING, offsetof(RegionEntry , szRegionName), "0", 0, 0 } ,
			{"URL" , TINI_STRING , offsetof(RegionEntry , szUrl) , "0", 0, 0 },	
			{"StatFlag" , TINI_INT , offsetof(RegionEntry , StatFlag) , "0", 0, 0 },	

		}, offsetof(REGIONLIST , iRegionNum) , sizeof(RegionEntry) ,MAX_BIGWORLD_NUM
	} ,
} ;

TINISECTIONDESC astWorldListDesc[] =
{
	{"WORLD" , TINI_SECTION_ARRAY , offsetof(WORLDLIST, astWorld) ,
		{
			{"WorldID" , TINI_INT , offsetof(WorldEntry , iWorldID) , "0", 0, 0 },
			{"WorldName" , TINI_STRING, offsetof(WorldEntry , szWorldName), "0", 0, 0 } ,
			{"Recommend" , TINI_INT , offsetof(WorldEntry , iRecommend) , "0", 0, 0 },	
			{"RegionID" , TINI_INT, offsetof(WorldEntry , RegionID), "0", 0, 0 } ,

		}, offsetof(WORLDLIST , iWorldNum) , sizeof(WorldEntry) ,MAX_WORLD_NUM

	} ,
} ;

TINITYPEDESC astVersionConfigDesc[] = {
		{"Version", TINI_INT, offsetof(DIR_SVRENV, stWorldList)+offsetof(CSDIRWORLDLIST, LastVersion), "0", 0, 0},
};

int dir_check_world_name(WORLDLIST *pstWorldList)
{
	int i,j;

	for (i=0; i<pstWorldList->iWorldNum-1; i++)
	{

		for (j=i+1; j<pstWorldList->iWorldNum; j++)
		{
			if (strcmp(pstWorldList->astWorld[i].szWorldName, pstWorldList->astWorld[j].szWorldName) == 0)
			{
				printf("World Name error!\n");
				return -1;
			}
		}
	}

	return 0;
}

int dir_check_region_name(REGIONLIST *pstRegionList)
{
	int i,j;

	for (i=0; i<pstRegionList->iRegionNum-1; i++)
	{
		for (j=i+1; j<pstRegionList->iRegionNum; j++)
		{
			if (strcmp(pstRegionList->astRegion[i].szRegionName, pstRegionList->astRegion[j].szRegionName) == 0)
			{
				printf("Region Name error!\n");
				return -1;
			}
		}
	}

	return 0;
}

int dir_cltmsg_send(DIR_SVRENV *pstEnv, CSPKG *pstCSPkgRes,TFRAMEHEAD *pstFrameHead, int iCltProtoVer)
{
	TDRDATA stNet;

	stNet.pszBuff = &pstEnv->szSendBuffer[0];	
	if (!pstCSPkgRes)
	{
		stNet.iBuff = 0;
	}
	else
	{
		stNet.iBuff = sizeof(pstEnv->szSendBuffer);
		
		if (0 > net_cs_pack(pstEnv->pstCSHan, &stNet, pstCSPkgRes, iCltProtoVer))
		{
			return -2;
		}
	}
	
	pstFrameHead->Cmd = TFRAMEHEAD_CMD_STOP;
	pstFrameHead->TimeStampType = 0;
	pstFrameHead->ID = -1;
	
	return tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iConnID, stNet.pszBuff, stNet.iBuff, pstFrameHead);
}


int dir_get_bigworld_idx(CSDIRWORLDLIST *pstWorldList, int iRegionID, REGIONLIST *pstRegionList, unsigned char *pucBigWorldIdx)
{
	int i;
	RegionEntry *pstRegion = NULL;

	for (i=0; i<pstRegionList->iRegionNum; i++)
	{
		if (iRegionID == pstRegionList->astRegion[i].RegionID)
		{
			pstRegion = &pstRegionList->astRegion[i];
			break;
		}
	}

	if (!pstRegion)
	{
		return -1;
	}

	for (i=0; i<pstWorldList->BigWorldNum; i++)
	{
		if (0 == strcasecmp(pstWorldList->BigWorldList[i].BigWorldName, pstRegion->szRegionName))
		{
			*pucBigWorldIdx = (unsigned char)i;
			return 0;
		}
	}

	if (pstWorldList->BigWorldNum >= MAX_BIGWORLD_NUM)
	{
		return -1;
	}
	else
	{
		STRNCPY(pstWorldList->BigWorldList[pstWorldList->BigWorldNum].BigWorldName,
					pstRegion->szRegionName, sizeof(pstWorldList->BigWorldList[pstWorldList->BigWorldNum].BigWorldName));
		STRNCPY(pstWorldList->BigWorldList[pstWorldList->BigWorldNum].URL,
					pstRegion->szUrl, sizeof(pstWorldList->BigWorldList[pstWorldList->BigWorldNum].URL));
		pstWorldList->BigWorldList[pstWorldList->BigWorldNum].StatFlag = pstRegion->StatFlag;
		
		pstWorldList->BigWorldNum++;
		*pucBigWorldIdx = (unsigned char)(pstWorldList->BigWorldNum-1);	

		return 0;
	}
}


int get_auth_conn(DIR_SVRENV* pstEnv, CSDIRWORLDINFO *pstWorldInfo)
{
	int i;

	if (pstEnv->pstConf->AuthConnNum > MAX_REGION)
	{
		return -1;
	}

	for (i=0; i<pstEnv->pstConf->AuthConnNum; i++)
	{
		if (pstWorldInfo->RegionID == pstEnv->pstConf->AuthConn[i].RegionID)
		{
			STRNCPY(pstWorldInfo->AuthIP, pstEnv->pstConf->AuthConn[i].AuthConnIP, sizeof(pstWorldInfo->AuthIP));
			pstWorldInfo->AuthPort = HTONS(pstEnv->pstConf->AuthConn[i].AuthConnPort);
			pstWorldInfo->AuthIP_Bak1 = (int)inet_addr(pstEnv->pstConf->AuthConn[i].AuthConnIP_Bak1);
			pstWorldInfo->AuthIP_Bak2 = (int)inet_addr(pstEnv->pstConf->AuthConn[i].AuthConnIP_Bak2);
			
			break;
		}

	}

	return 0;
}

/*int dir_get_bigworld_idx(CSDIRWORLDLIST *pstWorldList, char *pstBigWorldName, unsigned char *pucBigWorldIdx)
{
	int i;

	for (i=0; i<pstWorldList->BigWorldNum; i++)
	{
		if (0 == strcasecmp(pstWorldList->BigWorldList[i].BigWorldName, pstBigWorldName))
		{
			*pucBigWorldIdx = (unsigned char)i;
			return 0;
		}
	}

	if (pstWorldList->BigWorldNum >= MAX_BIGWORLD_NUM)
	{
		return -1;
	}
	else
	{
		STRNCPY(pstWorldList->BigWorldList[pstWorldList->BigWorldNum].BigWorldName,
					pstBigWorldName, sizeof(pstWorldList->BigWorldList[pstWorldList->BigWorldNum].BigWorldName));
		pstWorldList->BigWorldNum++;
		*pucBigWorldIdx = (unsigned char)(pstWorldList->BigWorldNum-1);	

		return 0;
	}
}*/


int dir_load_world_list(DIR_SVRENV *pstEnv)
{
	int i;
	WORLDLIST stWorldList;
	CSDIRWORLDINFO *pstWorldInfo;
	WorldEntry *pstWorldEntry;
	REGIONLIST stRegionList;

	memset(&pstEnv->stWorldList, 0 , sizeof(pstEnv->stWorldList));
	
	if (0 > tini_get_type(pstEnv->pstConf->WorldListCfgPath, pstEnv, astVersionConfigDesc))
	{
		printf("tini_get_type %s fail\n", pstEnv->pstConf->WorldListCfgPath);
		return -1;
	}
		
	if (0 > tini_get_ini(pstEnv->pstConf->WorldListCfgPath, (void *)&stWorldList, astWorldListDesc))
	{
		printf("tini_get_ini %s fail\n", pstEnv->pstConf->WorldListCfgPath);
		return -1;
	}

	if ( dir_check_world_name(&stWorldList) < 0)
	{
		return -1;
	}

	if (0 > tini_get_ini(pstEnv->pstConf->WorldListCfgPath, (void *)&stRegionList, astRegionDesc))
	{
		printf("tini_get_ini %s fail\n", pstEnv->pstConf->WorldListCfgPath);
		return -1;
	}

	if (dir_check_region_name(&stRegionList) < 0)
	{
		return -1;
	}

	for (i=0; i<stWorldList.iWorldNum; i++)
	{
		if (i >= MAX_WORLD_NUM)
		{
			printf("max world num is %d\n", MAX_WORLD_NUM);
			return -1;
		}
		
		pstWorldInfo = &pstEnv->stWorldList.ListInfoList[i];
		pstWorldEntry = &stWorldList.astWorld[i];

		pstWorldInfo->WorldID = pstWorldEntry->iWorldID;
		pstWorldInfo->RegionID = pstWorldEntry->RegionID;
		STRNCPY(pstWorldInfo->WorldName, pstWorldEntry->szWorldName, sizeof(pstWorldInfo->WorldName));
		if (pstWorldEntry->iRecommend)
		{
			pstWorldInfo->StatFlag |= STAT_FLAG_RECOMMEND;  
		}
		if(0 > dir_get_bigworld_idx(&pstEnv->stWorldList, pstWorldEntry->RegionID, &stRegionList,&pstWorldInfo->BigWorldIdx))
		{
			return -1;
		}

		get_auth_conn(pstEnv, pstWorldInfo);
	}
	pstEnv->stWorldList.WorldNum = stWorldList.iWorldNum;

	return 0;
}

int dir_svr_init(TAPPCTX *pstAppCtx, DIR_SVRENV *pstEnv)
{
	int i;

	memset(pstEnv, 0, sizeof(*pstEnv));
	
	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		return -1;
	}
	
	pstEnv->pstConf = (DIR_SVRCONF *)pstAppCtx->stConfData.pszBuff;

	pstEnv->pstRunStat = (DIR_SVRRUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
	if (NULL == pstEnv->pstRunStat)
	{
		pstEnv->pstRunStat = &gs_stStat;
	}

	if (0 > tapp_get_category(NULL, (void *)(&pstEnv->pstLogCat)))	
	{		
		printf("tapp_get_category run fail\n");
		return -1;	
	}	

	pstEnv->pstBlackWhiteH = NULL;
	if (pstEnv->pstConf->BlackWhiteCfgPath[0] && 0 > black_white_init(pstEnv->pstConf->BlackWhiteCfgPath, &pstEnv->pstBlackWhiteH))
	{
		printf("%s black_white_init fail\n", pstEnv->pstConf->BlackWhiteCfgPath);		
	}
	//printf("black_white_check %s: %d\n", "192.168.1.1", black_white_check(pstEnv->pstBlackWhiteH, inet_addr("192.168.1.1")));

	if (0 > dir_load_world_list(pstEnv))
	{
		printf("load world list %s error\n", pstEnv->pstConf->WorldListCfgPath);
		return -1;
	}

	if (0 > tconnapi_initialize(pstAppCtx->pszGCIMKey, pstAppCtx->iBusinessID))
	{
		printf("tbus gcim key %s init fail\n", pstAppCtx->pszGCIMKey) ;
		return -1;
	}
	
	if (0 > tconnapi_create(pstAppCtx->iId, &pstEnv->iTconnapiHandle))
	{
		printf("tbus id %s init fail\n", pstAppCtx->pszId);
		return -1;
	}

	if (0 > net_han_create_c(g_szMetalib_proto_cs , &pstEnv->pstCSHan))
	{
		return -1;
	}

	pstEnv->iReportSock = -1;
	if (pstEnv->pstConf->ReportAddr[0] != 0)
	{
		pstEnv->iReportSock = tnet_connect(pstEnv->pstConf->ReportAddr, 5);
		if (pstEnv->iReportSock < 0)
		{
			return -1;
		}
	}
	
	if (0 > ss_han_create(g_szMetalib_proto_ss, &pstEnv->pstSSHan))
	{
		return -1;
	}

	if (0 > init_report_interface(&pstEnv->hReportHandle, pstAppCtx->stCurr.tv_sec))
	{
		return -1;
	}

	pstEnv->iInnerSock = tnet_listen(pstEnv->pstConf->BindInnerAddr, 5);
	if (0 > pstEnv->iInnerSock)
	{
		perror("net listen error");
		return -1;
	}
	if (0 > tnet_set_nonblock(pstEnv->iInnerSock , 1))
	{
		perror("net listen set nonblock error");
		return -1;
	}

	if (DIR_MASTER == pstEnv->pstConf->MasterFlag)
	{
		pstEnv->iBakSock = tnet_connect(pstEnv->pstConf->DirBakAddr, 0);
		if (0 > pstEnv->iBakSock)
		{
			perror("connect error");
			return -1;
		}

		pstEnv->iSlaveSockNum = pstEnv->pstConf->DirSlaveAddrNum;
		for (i=0; i<pstEnv->iSlaveSockNum; i++)
		{
			pstEnv->aiSlaveSock[i] = tnet_connect(pstEnv->pstConf->DirSlaveAddrList[i], 0);
			if (0 > pstEnv->aiSlaveSock[i])
			{
				perror("connect error");
				return -1;
			}
		}

		GET_CLUSTER_ENTITY_ID(pstEnv->iConnID, pstAppCtx->iId, FUNC_DIR_SVR_CONN);
	}
	else if (DIR_BAK == pstEnv->pstConf->MasterFlag)
	{
		pstEnv->iSlaveSockNum = pstEnv->pstConf->DirSlaveAddrNum;
		for (i=0; i<pstEnv->iSlaveSockNum; i++)
		{
			pstEnv->aiSlaveSock[i] = tnet_connect(pstEnv->pstConf->DirSlaveAddrList[i], 0);
			if (0 > pstEnv->aiSlaveSock[i])
			{
				perror("connect error");
				return -1;
			}
		}
		
		GET_CLUSTER_ENTITY_ID(pstEnv->iConnID, pstAppCtx->iId, FUNC_DIR_BAK_CONN);
	}
	else if (DIR_SLAVE == pstEnv->pstConf->MasterFlag)
	{
		GET_CLUSTER_ENTITY_ID(pstEnv->iConnID, pstAppCtx->iId, FUNC_DIR_SLAVE_CONN);
	}
	else if (DIR_SLAVE1 == pstEnv->pstConf->MasterFlag)
	{
		GET_CLUSTER_ENTITY_ID(pstEnv->iConnID, pstAppCtx->iId, FUNC_DIR_SLAVE1_CONN);
	}

	pstEnv->iSendOnlineTime = pstAppCtx->stCurr.tv_sec;
	pstEnv->iXmlTime = pstAppCtx->stCurr.tv_sec;
	
	printf("dir_svr start\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "dir_svr start");

	tlog_info_dr(pstEnv->pstLogCat, 0, 0, (LPTDRMETA)pstAppCtx->stConfData.iMeta, pstAppCtx->stConfData.pszBuff, 
				pstAppCtx->stConfData.iLen, 0);	

	return 0;
}




int dir_clt_msg(TAPPCTX* pstAppCtx, DIR_SVRENV* pstEnv, char *pszMsg, int iMsgLen, TFRAMEHEAD *pstFrameHead)
{
	CSPKG stCsPkg;
	int iLatestVersion;
	int i;
	int iRet;
	int iCltProtoVer;
	
	if (TFRAMEHEAD_CMD_START != pstFrameHead->Cmd)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "pstFrameHead->Cmd=%d", pstFrameHead->Cmd);
		pstEnv->pstRunStat->BadCltMsg++;

		dir_cltmsg_send(pstEnv, NULL, pstFrameHead, 0);
		return -1;
	}

	
	if (0 > net_cs_unpack(pstEnv->pstCSHan, pszMsg, iMsgLen, &stCsPkg, 0))
	{
		pstEnv->pstRunStat->BadCltMsg++;
		tlog_error(pstEnv->pstLogCat, 0, 0, "net_cs_unpack fail");

		dir_cltmsg_send(pstEnv, NULL, pstFrameHead, 0);
		return -1;
	}

	iCltProtoVer = stCsPkg.Head.Ver;

	//检查黑白名单
	if (TFRAMEHEAD_EXTRA_IP == pstFrameHead->ExtraType)
	{
		if (black_white_check(pstEnv->pstBlackWhiteH, pstFrameHead->ExtraInfo.IPInfo.ip))
		{
			stCsPkg.Head.Magic = PROTOCOL_MAGIC;
			stCsPkg.Head.Cmd = DIR_RES;
			stCsPkg.Head.Ver = TDR_METALIB_PROTO_CS_VERSION;

			stCsPkg.Body.DirRes.Result = DIR_ERR;
			strcpy(stCsPkg.Body.DirRes.DirInfo.Err.ErrMsg, "很抱歉，服务仍未对外开放");
			dir_cltmsg_send(pstEnv, &stCsPkg, pstFrameHead, iCltProtoVer);
			
			return -1;
		}
	}

	if (DIR_REQ != stCsPkg.Head.Cmd)
	{
		if (stCsPkg.Head.Cmd == REPORT_DATA_REQ )
		{
			if (0 == report_check(pstEnv->hReportHandle, stCsPkg.Body.ReportDataReq.Num, stCsPkg.Body.ReportDataReq.Req) &&
				0 == report_get(pstEnv->hReportHandle, &stCsPkg.Body.ReportDataRes.Num, stCsPkg.Body.ReportDataRes.Res,
								MAX_REPORT_DATA_RES))
			{
				stCsPkg.Head.Magic 	= PROTOCOL_MAGIC;
				stCsPkg.Head.Cmd 	= REPORT_DATA_RES;
				stCsPkg.Head.Ver 		= TDR_METALIB_PROTO_CS_VERSION;

				dir_cltmsg_send(pstEnv, &stCsPkg, pstFrameHead, iCltProtoVer);

				return 0;
			}
		}

		pstEnv->pstRunStat->BadCltMsg++;
		dir_cltmsg_send(pstEnv, NULL, pstFrameHead, 0);
				
		return -1;
	}

	pstEnv->pstRunStat->RecvCltMsg++;

	iLatestVersion = stCsPkg.Body.DirReq.LastVersion;

	stCsPkg.Head.Magic 	= PROTOCOL_MAGIC;
	stCsPkg.Head.Cmd 	= DIR_RES;
	stCsPkg.Head.Ver 		= TDR_METALIB_PROTO_CS_VERSION;
	
	if (iLatestVersion == pstEnv->stWorldList.LastVersion)
	{
		CSDIRLATEST *pstLatestList = &stCsPkg.Body.DirRes.DirInfo.DirLatest;

		stCsPkg.Body.DirRes.Result = DIR_LATEST;
				
		for (i=0; i<pstEnv->stWorldList.WorldNum; i++)
		{
			pstLatestList->LatestList[i].WorldID  	 = pstEnv->stWorldList.ListInfoList[i].WorldID;
			pstLatestList->LatestList[i].StatFlag  	 = pstEnv->stWorldList.ListInfoList[i].StatFlag;
			STRNCPY(pstLatestList->LatestList[i].IP, pstEnv->stWorldList.ListInfoList[i].IP, 
						sizeof(pstLatestList->LatestList[i].IP));
			pstLatestList->LatestList[i].IP_Bak1 = pstEnv->stWorldList.ListInfoList[i].IP_Bak1;
			pstLatestList->LatestList[i].IP_Bak2 = pstEnv->stWorldList.ListInfoList[i].IP_Bak2;
			pstLatestList->LatestList[i].Port 		 = pstEnv->stWorldList.ListInfoList[i].Port;
			pstLatestList->LatestList[i].RegionID 	 = pstEnv->stWorldList.ListInfoList[i].RegionID;
			STRNCPY(pstLatestList->LatestList[i].AuthIP, pstEnv->stWorldList.ListInfoList[i].AuthIP,
						sizeof(pstLatestList->LatestList[i].AuthIP));
			pstLatestList->LatestList[i].AuthIP_Bak1 = pstEnv->stWorldList.ListInfoList[i].AuthIP_Bak1;
			pstLatestList->LatestList[i].AuthIP_Bak2 = pstEnv->stWorldList.ListInfoList[i].AuthIP_Bak2;
			pstLatestList->LatestList[i].AuthPort 	 = pstEnv->stWorldList.ListInfoList[i].AuthPort;
			STRNCPY(pstLatestList->LatestList[i].MudIP, pstEnv->stWorldList.ListInfoList[i].MudIP,
						sizeof(pstLatestList->LatestList[i].MudIP));
			pstLatestList->LatestList[i].MudIP_Bak1 = pstEnv->stWorldList.ListInfoList[i].MudIP_Bak1;
			pstLatestList->LatestList[i].MudIP_Bak2 = pstEnv->stWorldList.ListInfoList[i].MudIP_Bak2;
			pstLatestList->LatestList[i].MudPort 		 = pstEnv->stWorldList.ListInfoList[i].MudPort;
			pstLatestList->LatestList[i].MudStatFlag  	 = pstEnv->stWorldList.ListInfoList[i].MudStatFlag;
			
		}
		pstLatestList->WorldNum = pstEnv->stWorldList.WorldNum;

		pstEnv->pstRunStat->SendLatestWorldInfo++;
	}
	else
	{
		stCsPkg.Body.DirRes.Result = DIR_SUCC;

		memcpy(&stCsPkg.Body.DirRes.DirInfo.WorldList, &pstEnv->stWorldList, sizeof(stCsPkg.Body.DirRes.DirInfo.WorldList));

		pstEnv->pstRunStat->SendAllWorldInfo++;
	}

	iRet = dir_cltmsg_send(pstEnv, &stCsPkg, pstFrameHead, iCltProtoVer);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "dir_cltmsg_send fail %d", iRet);
		pstEnv->pstRunStat->SendCltMsgFail++;
	}

	return iRet;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

CSDIRWORLDINFO *dir_find_world(CSDIRWORLDLIST *pstWorldList, unsigned short unWorldID, int iRegionID,int *piIdx)
{
	int i;

	for (i=0; i<pstWorldList->WorldNum; i++)
	{
		if (unWorldID == pstWorldList->ListInfoList[i].WorldID &&
			iRegionID == pstWorldList->ListInfoList[i].RegionID)
		{
			*piIdx = i;
			return &pstWorldList->ListInfoList[i];
		}
	}

	return NULL;
}

static int record_online_info(TAPPCTX *pstAppCtx, DIR_SVRENV *pstEnv,SSDIRREPORT *pstReport/*int iRegionID,int iWorldID, int iOnlineNum*/)
{
	int i;
	int iNum = (int)(sizeof(pstEnv->DirInfo)/sizeof(pstEnv->DirInfo[0]));

	for (i=0; i<pstEnv->WorldNum; i++)
	{
		if ( 	pstEnv->DirInfo[i].WorldID 	== pstReport->WorldID &&
		    	pstEnv->DirInfo[i].RegionID 	== pstReport->RegionID  )
		{
			if (pstEnv->DirInfo[i].OnlineNum < pstReport->PlayerNum)
			{
				pstEnv->DirInfo[i].OnlineNum = pstReport->PlayerNum;
			}

			STRNCPY(	pstEnv->DirInfo[i].MapInfo, 
					pstReport->MapInfo, 
					sizeof(pstEnv->DirInfo[i].MapInfo));
			return 0;
		}
	}

	if (pstEnv->WorldNum >= iNum)
	{
		return -1;
	}

	memset(&pstEnv->DirInfo[pstEnv->WorldNum], 0, sizeof(pstEnv->DirInfo[0]));
	pstEnv->DirInfo[pstEnv->WorldNum].OnlineNum 	= pstReport->PlayerNum;
	pstEnv->DirInfo[pstEnv->WorldNum].WorldID	= pstReport->WorldID;
	pstEnv->DirInfo[pstEnv->WorldNum].RegionID 	= pstReport->RegionID;
	STRNCPY(	pstEnv->DirInfo[pstEnv->WorldNum].MapInfo, 
			pstReport->MapInfo, 
			sizeof(pstEnv->DirInfo[pstEnv->WorldNum].MapInfo));
	pstEnv->WorldNum ++;
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

static int record_mud_online_info(TAPPCTX *pstAppCtx, DIR_SVRENV *pstEnv,SSDIRREPORT *pstReport)
{
	int i;
	
	for (i=0; i<pstEnv->WorldNum; i++)
	{
		if ( 	pstEnv->DirInfo[i].WorldID 	== pstReport->WorldID &&
		    	pstEnv->DirInfo[i].RegionID 	== pstReport->RegionID  )
		{
			pstEnv->DirInfo[i].MudOnlineNum = pstReport->PlayerNum;
			return 0;
		}
	}

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int dir_svr_msg(TAPPCTX *pstAppCtx, DIR_SVRENV *pstEnv, char * pszRecvBuffer, int iRecv)
{
	SSPKG stPkg;
	SSDIRREPORT *pstReport;
	CSDIRWORLDINFO *pstWorld;
	unsigned char ucBusyDegree;
	BUSY_DEGREE *pstDegree;
	int iPersent;
	int i;
//	int iIdx;
	int iVersion;

	iVersion = NTOHS(*(uint16_t *)(pszRecvBuffer+8));
	
	if (0 > ss_unpack(pstEnv->pstSSHan, pszRecvBuffer, iRecv, &stPkg, iVersion))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_unpack fail, Pkg len=%d", iRecv);
		return -1;
	}

	switch(stPkg.Head.Cmd)
	{
		case DIR_REPORT:
			pstReport = &stPkg.Body.DirReport;

			tlog_info(pstEnv->pstLogCat, 0, 0, "world report: worldid=%d, MaxPlayer=%d, OnlinePlayer=%d, WeiHuFlag=%d", 
							pstReport->WorldID, pstReport->PlayerMaxNum, pstReport->PlayerNum, pstReport->WeiHuFlag);

			for (i=0; i<pstEnv->stWorldList.WorldNum; i++)
			{
				if (pstReport->WorldID != pstEnv->stWorldList.ListInfoList[i].WorldID ||
					pstReport->RegionID !=pstEnv->stWorldList.ListInfoList[i].RegionID)
				{
				//	tlog_info(pstEnv->pstLogCat, 0, 0, "no world found");

					continue;
				}

				pstWorld = &pstEnv->stWorldList.ListInfoList[i];
				if (0 == pstReport->PlayerMaxNum || pstReport->PlayerNum > pstReport->PlayerMaxNum)
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "report max player %d / online player %d", pstReport->PlayerMaxNum, pstReport->PlayerNum);
					continue;
				}

				record_online_info(pstAppCtx, pstEnv,pstReport);

				record_data(pstReport->RegionID, pstReport->WorldID, pstReport->PlayerNum);
			
				STRNCPY(pstWorld->IP, pstReport->IP, sizeof(pstWorld->IP));
				pstWorld->Port = pstReport->Port;
				pstWorld->IP_Bak1 = pstReport->IP_Bak1;
				pstWorld->IP_Bak2 = pstReport->IP_Bak2;

				iPersent = pstReport->PlayerNum*100;
				iPersent = iPersent/pstReport->PlayerMaxNum;

				pstDegree = &pstEnv->pstConf->BusyDegree;

				if (pstReport->WeiHuFlag)
				{
					ucBusyDegree = DIR_NOT_WORK;
				}
				else
				{
					if( iPersent >= pstDegree->Full)
					{
						ucBusyDegree = DIR_FULL;
					}
					else if (iPersent >= pstDegree->Busy)
					{
						ucBusyDegree = DIR_BUSY;
					}
					else if (iPersent >= pstDegree->Normal)
					{
						ucBusyDegree = DIR_NORMAL;
					}
					else
					{
						ucBusyDegree = DIR_EXCELLENCE;
					}				
				}
				pstWorld->StatFlag &= 0xF0;
				pstWorld->StatFlag |= ucBusyDegree;
				pstEnv->astWorldActive[i] = pstAppCtx->stCurr.tv_sec;
			}

			pstEnv->pstRunStat->RecvSvrMsg++;
			
			/*pstWorld = dir_find_world(&pstEnv->stWorldList, pstReport->WorldID,pstReport->RegionID, &iIdx);
			if (NULL == pstWorld)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "report world id %d not exist", pstReport->WorldID);
				return -1;
			}

			if (0 == pstReport->PlayerMaxNum || pstReport->PlayerNum > pstReport->PlayerMaxNum)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "report max player %d / online player %d", pstReport->PlayerMaxNum, pstReport->PlayerNum);
				return -1;
			}

			record_online_info(pstAppCtx, pstEnv,pstReport);

			record_data(pstReport->RegionID, pstReport->WorldID, pstReport->PlayerNum);
		
			tlog_notice(pstEnv->pstLogCat, 0, 0, "world report: worldid=%d, MaxPlayer=%d, OnlinePlayer=%d", 
						pstReport->WorldID, pstReport->PlayerMaxNum, pstReport->PlayerNum);


			pstEnv->pstRunStat->RecvSvrMsg++;

			STRNCPY(pstWorld->IP, pstReport->IP, sizeof(pstWorld->IP));
			pstWorld->Port = pstReport->Port;
			pstWorld->IP_Bak1 = pstReport->IP_Bak1;
			pstWorld->IP_Bak2 = pstReport->IP_Bak2;

			iPersent = pstReport->PlayerNum*100;
			iPersent = iPersent/pstReport->PlayerMaxNum;

			pstDegree = &pstEnv->pstConf->BusyDegree;

			if (pstReport->WeiHuFlag)
			{
				ucBusyDegree = DIR_NOT_WORK;
				//pstWorld->StatFlag = 0;
			}
			else
			{
				if( iPersent >= pstDegree->Full)
				{
					ucBusyDegree = DIR_FULL;
				}
				else if (iPersent >= pstDegree->Busy)
				{
					ucBusyDegree = DIR_BUSY;
				}
				else if (iPersent >= pstDegree->Normal)
				{
					ucBusyDegree = DIR_NORMAL;
				}
				else
				{
					ucBusyDegree = DIR_EXCELLENCE;
				}				
			}
			pstWorld->StatFlag &= 0xF0;
			pstWorld->StatFlag |= ucBusyDegree;
			pstEnv->astWorldActive[iIdx] = pstAppCtx->stCurr.tv_sec;*/
			break;
		case MUD_DIR_REPORT:
			pstReport = &stPkg.Body.MudDirReport;
			tlog_notice(pstEnv->pstLogCat, 0, 0, "mud report: worldid=%d, MaxPlayer=%d, OnlinePlayer=%d", 
				pstReport->WorldID, pstReport->PlayerMaxNum, pstReport->PlayerNum);
			
			for (i=0; i<pstEnv->stWorldList.WorldNum; i++)
			{
				if (pstReport->WorldID != pstEnv->stWorldList.ListInfoList[i].WorldID ||
					pstReport->RegionID !=pstEnv->stWorldList.ListInfoList[i].RegionID)
				{
					continue;
				}

				pstWorld = &pstEnv->stWorldList.ListInfoList[i];

				if (0 == pstReport->PlayerMaxNum || pstReport->PlayerNum > pstReport->PlayerMaxNum)
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "report max player %d / online player %d", pstReport->PlayerMaxNum, pstReport->PlayerNum);
					return -1;
				}

				record_mud_online_info(pstAppCtx, pstEnv,pstReport);
	
				STRNCPY(pstWorld->MudIP, pstReport->IP, sizeof(pstWorld->MudIP));
				pstWorld->MudPort = pstReport->Port;
				pstWorld->MudIP_Bak1 = pstReport->IP_Bak1;
				pstWorld->MudIP_Bak2 = pstReport->IP_Bak2;

				iPersent = pstReport->PlayerNum*100;
				iPersent = iPersent/pstReport->PlayerMaxNum;

				pstDegree = &pstEnv->pstConf->BusyDegree;

				if (iPersent <= pstDegree->Excellence)
				{
					ucBusyDegree = DIR_EXCELLENCE;
				}
				else if (iPersent <= pstDegree->Normal)
				{
					ucBusyDegree = DIR_NORMAL;
				}
				else if (iPersent <= pstDegree->Busy)
				{
					ucBusyDegree = DIR_BUSY;
				}
				else
				{
					ucBusyDegree = DIR_FULL;
				}
				
				pstWorld->MudStatFlag |= ucBusyDegree;
				pstEnv->astWorldMudActive[i] = pstAppCtx->stCurr.tv_sec;
			}
			pstEnv->pstRunStat->RecvSvrMsg++;

			
			/*pstWorld = dir_find_world(&pstEnv->stWorldList, pstReport->WorldID,pstReport->RegionID, &iIdx);
			if (NULL == pstWorld)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "report world id %d not exist", pstReport->WorldID);
				return -1;
			}

			if (0 == pstReport->PlayerMaxNum || pstReport->PlayerNum > pstReport->PlayerMaxNum)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "report max player %d / online player %d", pstReport->PlayerMaxNum, pstReport->PlayerNum);
				return -1;
			}

			tlog_notice(pstEnv->pstLogCat, 0, 0, "mud report: worldid=%d, MaxPlayer=%d, OnlinePlayer=%d", 
			pstReport->WorldID, pstReport->PlayerMaxNum, pstReport->PlayerNum);

			record_mud_online_info(pstAppCtx, pstEnv,pstReport);
			pstEnv->pstRunStat->RecvSvrMsg++;

			//pstWorld->MudIP = pstReport->IP;
			STRNCPY(pstWorld->MudIP, pstReport->IP, sizeof(pstWorld->MudIP));
			pstWorld->MudPort = pstReport->Port;
			pstWorld->MudIP_Bak1 = pstReport->IP_Bak1;
			pstWorld->MudIP_Bak2 = pstReport->IP_Bak2;

			iPersent = pstReport->PlayerNum*100;
			iPersent = iPersent/pstReport->PlayerMaxNum;

			pstDegree = &pstEnv->pstConf->BusyDegree;

			if (iPersent <= pstDegree->Excellence)
			{
				ucBusyDegree = DIR_EXCELLENCE;
			}
			else if (iPersent <= pstDegree->Normal)
			{
				ucBusyDegree = DIR_NORMAL;
			}
			else if (iPersent <= pstDegree->Busy)
			{
				ucBusyDegree = DIR_BUSY;
			}
			else
			{
				ucBusyDegree = DIR_FULL;
			}
			
			pstWorld->MudStatFlag |= ucBusyDegree;
			pstEnv->astWorldMudActive[iIdx] = pstAppCtx->stCurr.tv_sec;*/
			
			break;
		default:
			return -1;
			break;
	}

	if (DIR_MASTER == pstEnv->pstConf->MasterFlag || DIR_BAK == pstEnv->pstConf->MasterFlag)
	{
		for (i=0; i<pstEnv->iSlaveSockNum; i++)
		{
			tnet_send(pstEnv->aiSlaveSock[i], pszRecvBuffer, iRecv, 0);
		}
	}
	
	return 0;
}

int dir_svr_proc(TAPPCTX *pstAppCtx, DIR_SVRENV *pstEnv)
{
	int iBusy=0;
	int iLoopNum = 0;
	TBUSADDR iMsgDst ;
	TBUSADDR iMsgSrc ;
	int iMsgLen;
	int iRet = 0;
	int iRecv;
	char *pszMsg;
	TFRAMEHEAD stFrameHead;
	struct sockaddr stCliAddr;
	socklen_t iCliAddrLen;
	int iFrameHeadLen ;

	//先收client的请求
	while (LOOP_NUM > iLoopNum)
	{
		iLoopNum++;
		
		iMsgDst = pstAppCtx->iId;
		iMsgSrc = TBUS_ADDR_ALL;	// 接收发到tbus的所有来源的数据
		iMsgLen = sizeof(pstEnv->szRecvBuffer);
		pszMsg = &pstEnv->szRecvBuffer[0];
		if ( TBUS_SUCCESS != tbus_recv(pstAppCtx->iBus, &iMsgSrc, &iMsgDst, 
			(void *)pszMsg, (size_t *)&iMsgLen, 0) )
		{
			//no msg
			break;
		}
		iBusy = 1;

		iFrameHeadLen = sizeof(stFrameHead);
		iRet = tconnapi_decode(pszMsg, iMsgLen, &stFrameHead, &iFrameHeadLen);
		if (0 != iRet)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tconnapi_decode failed, iRet0x%x, Msglen:%d", iRet, iMsgLen);
			continue;
		}

		pszMsg += iFrameHeadLen;
		iMsgLen -= iFrameHeadLen;
		iRet = dir_clt_msg(pstAppCtx, pstEnv, pszMsg, iMsgLen, &stFrameHead);
	}

	//再处理udp 端口的消息
	iCliAddrLen = sizeof(stCliAddr);
	iRecv = recvfrom(pstEnv->iInnerSock, pstEnv->szRecvBuffer, sizeof(pstEnv->szRecvBuffer), 0, &stCliAddr, &iCliAddrLen);
	if (0 < iRecv)
	{
		iBusy = 1;
		
		iRet = dir_svr_msg(pstAppCtx, pstEnv, pstEnv->szRecvBuffer, iRecv);
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

int dir_copy_svrinfo(DIR_SVRENV *pstEnv, CSDIRWORLDLIST *pstWorldList, time_t *pstWorldTimer, time_t *pstWorldMudActive)
{
	int i;
	int iIdx;
	CSDIRWORLDINFO *pstWorldInfo;
	CSDIRWORLDINFO *pstOldWorldInfo;
	CSDIRWORLDLIST *pstNewWorldList = &pstEnv->stWorldList;

	for (i=0; i<pstNewWorldList->WorldNum; i++)
	{
		pstWorldInfo = &pstNewWorldList->ListInfoList[i];
		pstOldWorldInfo = dir_find_world(pstWorldList, pstWorldInfo->WorldID, pstWorldInfo->RegionID, &iIdx);
		if (pstOldWorldInfo)
		{
			STRNCPY(pstWorldInfo->IP , pstOldWorldInfo->IP, sizeof(pstWorldInfo->IP));
			pstWorldInfo->Port = pstOldWorldInfo->Port;
			pstWorldInfo->StatFlag |= (pstOldWorldInfo->StatFlag & 0x0F) ;

			STRNCPY(pstWorldInfo->MudIP ,pstOldWorldInfo->MudIP, sizeof(pstWorldInfo->MudIP));
			pstWorldInfo->MudPort = pstOldWorldInfo->MudPort;
			pstWorldInfo->MudStatFlag |= (pstOldWorldInfo->MudStatFlag & 0x0F) ;

			pstEnv->astWorldActive[i] = pstWorldTimer[iIdx];
			pstEnv->astWorldMudActive[i] = pstWorldMudActive[iIdx];
		}
	}
	
	return 0;
}

int dir_svr_reload(TAPPCTX *pstAppCtx, DIR_SVRENV *pstEnv)
{
	DIR_SVRCONF   *pstPreConf;
	CSDIRWORLDLIST stWorldList;
	time_t astWorldActive[MAX_WORLD_NUM];
	time_t astWorldMudActive[MAX_WORLD_NUM];

	memcpy(&stWorldList, &pstEnv->stWorldList, sizeof(stWorldList));
	memcpy(astWorldActive, pstEnv->astWorldActive, sizeof(astWorldActive));
	memcpy(astWorldMudActive, pstEnv->astWorldMudActive, sizeof(astWorldMudActive));
	
	pstPreConf = (DIR_SVRCONF   *)pstAppCtx->stConfPrepareData.pszBuff;
		
	memcpy(pstEnv->pstConf, pstPreConf, sizeof(*pstEnv->pstConf));
	memset(pstEnv->astWorldActive, 0, sizeof(pstEnv->astWorldActive));
	memset(pstEnv->astWorldMudActive, 0, sizeof(pstEnv->astWorldMudActive));

	if (0 > dir_load_world_list(pstEnv))
	{
		return -1;
	}

	dir_copy_svrinfo(pstEnv, &stWorldList, astWorldActive, astWorldMudActive);

	if (pstEnv->pstBlackWhiteH)
	{
		black_white_fini(&pstEnv->pstBlackWhiteH);
	}

	pstEnv->pstBlackWhiteH = NULL;
	if (pstEnv->pstConf->BlackWhiteCfgPath[0] && 0 > black_white_init(pstEnv->pstConf->BlackWhiteCfgPath, &pstEnv->pstBlackWhiteH))
	{
		printf("init black-white file:%s fail\n", pstEnv->pstConf->BlackWhiteCfgPath);;		
		tlog_info(pstEnv->pstLogCat, 0, 0, "init black-white file:%s fail", pstEnv->pstConf->BlackWhiteCfgPath);
	}
	
	printf("dir_svr reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "dir_svr reload");

	return 0;
}

int dir_svr_fini(TAPPCTX *pstAppCtx, DIR_SVRENV *pstEnv)
{
	int i;
	
	if (pstEnv->pstBlackWhiteH)
	{
		black_white_fini(&pstEnv->pstBlackWhiteH);
	}

	net_han_destroy(&pstEnv->pstCSHan);
	ss_han_destroy(&pstEnv->pstSSHan);

	tnet_close(pstEnv->iInnerSock);
	tnet_close(pstEnv->iReportSock);

	if (DIR_MASTER == pstEnv->pstConf->MasterFlag)
	{
		tnet_close(pstEnv->iBakSock);

		for (i=0; i<pstEnv->iSlaveSockNum; i++)
		{
			tnet_close(pstEnv->aiSlaveSock[i]);
		}
	}
	else if (DIR_BAK == pstEnv->pstConf->MasterFlag)
	{
		for (i=0; i<pstEnv->iSlaveSockNum; i++)
		{
			tnet_close(pstEnv->aiSlaveSock[i]);
		}
	}
	destory_reprot_interface(pstEnv->hReportHandle);

	printf("dir_svr finish\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "dir_svr finish");
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

static int send_online_info_report(TAPPCTX *pstAppCtx, DIR_SVRENV *pstEnv)
{
	int i;
	SSPKG stSSPkg;
	TDRDATA stNet;
	char szBuff[sizeof(SSPKG)];
	SSDIRINFODB *pstSSDirInfoDB;
	
	stSSPkg.Head.Magic = PROTOCOL_MAGIC;
	stSSPkg.Head.HeadLen = sizeof(stSSPkg.Head);

	stSSPkg.Head.Cmd = DIR_INFO_DB;
	pstSSDirInfoDB = &stSSPkg.Body.DirInfoDB;

	pstSSDirInfoDB->WorldNum = 0;
	for (i=0; i<pstEnv->WorldNum; i++)
	{
		memcpy(&pstSSDirInfoDB->DirInfo[pstSSDirInfoDB->WorldNum],
			&pstEnv->DirInfo[i], sizeof(pstSSDirInfoDB->DirInfo[0]));
		pstSSDirInfoDB->WorldNum++;

		if (pstSSDirInfoDB->WorldNum >= MAX_DIR_INFO)
		{
			stNet.pszBuff = szBuff;
			stNet.iBuff = sizeof(szBuff);
			ss_pack(pstEnv->pstSSHan, &stNet, &stSSPkg, 0);
			tnet_send(pstEnv->iReportSock, stNet.pszBuff, stNet.iBuff, 10);
			pstSSDirInfoDB->WorldNum = 0;
		}
	}

	if (pstSSDirInfoDB->WorldNum > 0)
	{
		stNet.pszBuff = szBuff;
		stNet.iBuff = sizeof(szBuff);
		ss_pack(pstEnv->pstSSHan, &stNet, &stSSPkg, 0);
		tnet_send(pstEnv->iReportSock, stNet.pszBuff, stNet.iBuff, 10);
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

static int send_online_info(TAPPCTX *pstAppCtx, DIR_SVRENV *pstEnv)
{
	int i;
	unsigned int iSendSec;// = pstEnv->pstConf->UpdataDBInterVal * 60;
	unsigned int iCurrTime = pstAppCtx->stCurr.tv_sec;
	
	if (pstEnv->iReportSock < 0)
	{
		return -1;
	}

	if (pstEnv->pstConf->UpdataDBInterVal <= 0 )
	{
		pstEnv->pstConf->UpdataDBInterVal = 60;
	}

	iSendSec = pstEnv->pstConf->UpdataDBInterVal * 60;
	
	if (iCurrTime >= pstEnv->iSendOnlineTime + iSendSec)
	{
		
		for (i=0; i<pstEnv->WorldNum; i++)
		{
			pstEnv->DirInfo[i].Time = iCurrTime;	
		}

		send_online_info_report(pstAppCtx, pstEnv);
		
		for (i=0; i<pstEnv->WorldNum; i++)
		{
			pstEnv->DirInfo[i].OnlineNum = 0;
		}

		pstEnv->iSendOnlineTime = iCurrTime;
	}
	return 0;
}


int dir_create_xml_mkdir(const char *szXmlName)
{
	int iLen = strlen(szXmlName);
	int i;
	char szDir[256];

	for (i=0; i<iLen-1; i++)
	{
		if (szXmlName[i] == '/')
		{
			if (i == 0)continue;
			
			strncpy(szDir, &szXmlName[0], i);
			szDir[i] = 0;
			if(access(szDir, F_OK) < 0)
			{
				mkdir(szDir, S_IWRITE|S_IREAD|S_IEXEC);
				chmod(szDir,775);
			}
		}
	}
	return 0;
}

int dir_create_xml(TAPPCTX *pstAppCtx, DIR_SVRENV *pstEnv)
{
	LPTDRMETA pstMeta = NULL;
	if(pstEnv->pstConf->XmlFileName[0]  == 0)
		return 0;

	dir_create_xml_mkdir(pstEnv->pstConf->XmlFileName);
	pstMeta = tdr_get_meta_by_name(pstEnv->pstCSHan->pstCSProtoLib, "CSDirWorldList");
	return res_mem_to_xml(pstMeta, pstEnv->pstConf->XmlFileName,(char *)&pstEnv->stWorldList, sizeof(pstEnv->stWorldList));

	UNUSED(pstAppCtx);
}

int dir_svr_tick(TAPPCTX *pstAppCtx, DIR_SVRENV *pstEnv)
{
	int i;
	CSDIRWORLDINFO *pstWorld;

	for (i=0; i<pstEnv->stWorldList.WorldNum; i++)
	{
		pstWorld = &pstEnv->stWorldList.ListInfoList[i];

		if (pstEnv->astWorldActive[i] + DIR_TIME_OUT < pstAppCtx->stCurr.tv_sec)
		{
			pstWorld->StatFlag &= 0xF0;
			tlog_info(pstEnv->pstLogCat, 0, 0, "world %d time out, active %d, now %d",
				pstWorld->WorldID, pstEnv->astWorldActive[i], pstAppCtx->stCurr.tv_sec);
		}

		if (pstEnv->astWorldMudActive[i] + DIR_TIME_OUT < pstAppCtx->stCurr.tv_sec)
		{
			pstWorld->MudStatFlag &= 0xF0;
		}
	}

	//record_tick();
	
	if (DIR_SLAVE == pstEnv->pstConf->MasterFlag)
	{
		send_online_info(pstAppCtx, pstEnv);
	}

	if (	pstEnv->iXmlTime + pstEnv->pstConf->XmlInterval < pstAppCtx->stCurr.tv_sec &&
		pstEnv->pstConf->XmlInterval > 0 &&
		DIR_MASTER == pstEnv->pstConf->MasterFlag)
	{
		// 生成xml文件
		dir_create_xml(pstAppCtx, pstEnv);
		pstEnv->iXmlTime = pstAppCtx->stCurr.tv_sec;
	}
	
	return 0;
}

int dir_svr_stop(TAPPCTX *pstAppCtx, DIR_SVRENV *pstEnv)
{
	int iReady=0;
	
	iReady = 1;
	if (iReady)
	{
		return -1;
	}
	else
	{
		return 0;
	}
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}

int main(int argc, char* argv[])
{
	int iRet;
	void* pvArg	=	&gs_stEnv;

	memset(&gs_stAppCtx, 0, sizeof(gs_stAppCtx));
	memset(&gs_stEnv, 0, sizeof(gs_stEnv));

	gs_stAppCtx.argc	=	argc;
	gs_stAppCtx.argv	=	argv;

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)dir_svr_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_dir_svr;
	gs_stAppCtx.stConfData.pszMetaName = "dir_svrconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);
		
	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)dir_svr_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)dir_svr_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)dir_svr_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)dir_svr_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)dir_svr_stop;
	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	tapp_def_fini(&gs_stAppCtx, pvArg);
	
	return iRet;
}


