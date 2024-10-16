
#include <stdio.h>

#include "tapp/tapp.h"
#include "pal/pal.h" 

#include "tdr/tdr.h"
#include "tlog/tlog.h"
#include "chat_mon_service_conf_desc.h"
#include "version.h"
#include "tbus/tbus.h"
#include "comm_misc.h"

#include "soapH.h"
#include "ChatMonServiceSoap.nsmap"
#include <iconv.h>
#include "fcgi_config.h"
#include "tlog/tlogbin.h"
#include "libdirty.h"
#include "tdirtyapi.h"
#include "entity.h"
#include "md5.h"


#include <stdlib.h>
#include "comm/tconvert.h"
#include "tlog/tlogbindef.h"
#include "fcgios.h"

//#include "chat_mon_service.h"

extern char **environ;

#include "fcgi_stdio.h"
#include "cs_cgi.h"

#ifndef WIN32
#define max(a, b)	((a) > (b) ? (a) : (b))
#define min(a, b)	((a) < (b) ? (a) : (b))
#endif


#define UNUSED( param ) ((void)(param))



#define MAX_CHAT_MON_ERROR_LEN 2048 //错误信息长度 
//#define DEFAULT_TIME_OUT 2000 //默认超时时间
#define MAX_STR_XML_LEN 2048000 //最大返回xml字符串长度

#define MAX_GBK_LENG 1024 //最大GBK字符串长度
#define MAX_GBK_TEXT_NUM 5 //最大GBK字符串数量
#define MAX_LOG_PATH_LENG 128 //日志目录长度
//#define MAX_CHAT_MON_SQL_LEN 20480 //SQL长度 
//#define MAX_CHAT_MON_META_NAME_LEN 64 //元数据名 
#define MAX_CHAT_MON_WHERE_LEN 1024 //元数据名 
#define CHAT_MON_LOCAL_BUS_ADDR_START 10
#define CHAT_MON_CHANNEL_SIZE 1024*256
#define CHAT_MON_CONN_NUM_SOFT_EDGE 10
#define CHAT_MON_MAX_EVENTS		                8192
#define CHAT_MON_TIME_SLICE_COUNT                10
#define CHAT_MON_PAK_PER_SLICE                   300
#define CHAT_MON_CONND_WAIT_TIME                5
#define CHAT_MON_PKG_NAME "OpLogMonPkg"
#define OP_LOG_PKG_NAME "op_log_meta"

#define CHAT_MON_RECVCHECK_INTERVAL            	10
#define MAX_CHAT_LOG_BUFF_NUM 100 //最大缓存的log数量


#ifdef TDR_OS_LITTLEENDIAN
#define tdr_ntohq(x)                    tdr_os_swap64(x)
#define tdr_htonq(x)                    tdr_os_swap64(x)
#define tdr_ntohl_(x)                    tdr_os_swap32(x)
#define tdr_htonl_(x)                    tdr_os_swap32(x)
#define tdr_ntohs_(x)                    tdr_os_swap16(x)
#define tdr_htons_(x)                    tdr_os_swap16(x)
#else
#define tdr_ntohq(x)                    (x)
#define tdr_htonq(x)                    (x)
#define tdr_ntohl_(x)                   (x)
#define tdr_htonl_(x)                   (x)
#define tdr_ntohs_(x)                   (x)
#define tdr_htons_(x)                   (x)
#endif


typedef TMEMPOOL				CHATMONCONNPOOL;
typedef TMEMPOOL				*LPCHATMONCONNPOOL;

#define chat_mon_connd_init_pool(ppstPool, iMax, iUnit)	tmempool_new(ppstPool, iMax, iUnit)
#define chat_mon_connd_fini_pool(ppstPool)				tmempool_destroy(ppstPool)
#define chat_mon_connd_get_inst(pstPool, iIdx)			(ChatMonConnInst*)tmempool_get(pstPool, iIdx)
#define chat_mon_connd_alloc_inst(pstPool)				tmempool_alloc(pstPool)
#define chat_mon_connd_free_inst(pstPool, iIdx)			tmempool_free(pstPool, iIdx)
#define chat_mon_connd_find_used_first(pstPool, piIdx)    tmempool_find_used_first(pstPool, piIdx) 
#define chat_mon_connd_find_used_next(pstPool, piIdx)    tmempool_find_used_next(pstPool, piIdx) 

typedef struct tagPDULenTDRParserInst	PDULENTDRPARSERINST;
typedef struct tagPDULenTDRParserInst	*LPPDULENTDRPARSERINST;

/* 使用TDR方法来分析数据包的长度 */
struct tagPDULenTDRParserInst
{
	LPTDRMETA pstPkg;

	int iPkgLenNOff;	/*记录PDU长度成员相对PDU总结构的偏移*/
	int iPkgLenUnitSize;	/*记录PDU长度成员的存储空间*/
	int iHeadLenNOff;	/*记录PDU头部长度成员相对PDU总结构的偏移*/
	int iHeadLenUnitSize;	/*记录PDU头部长度成员的存储空间*/
	int iBodyLenNOff;	/*记录PDU消息体长度成员相对PDU总结构的偏移*/
	int iBodyLenUnitSize;	/*记录PDU消息体长度成员的存储空间*/
};

struct tagTDRSizeInfo
{
	TDROFF iNOff;   /**<网络传输信息偏移*/
	TDROFF iHOff;    /**<本地存储信息偏移*/
	int iUnitSize;	
	int idxSizeType;	/*保存sizeinfo 用简单类型打包方法时使用类型，在TDR_BUILD version 5时加入*/
};
typedef struct tagTDRSizeInfo TDRSIZEINFO;
typedef struct tagTDRSizeInfo *LPTDRSIZEINFO;



typedef struct tagChatMonConnInst
{
	short fListen;
	short fStream;
	int iIdx;
	int iID;

	int s;

	int iUseTimeStamp;

	int iBuff;
	int iOff;
	int iData;
	int fWaitFirstPkg;

	int iHeadLen;
	int iPkgLen;

	int iRecvMsg;
	int iRecvByte;

	time_t tLastRecvMsgCheck;
	struct sockaddr stAddr;
	time_t tCreate;
	time_t tLastRecv;
	
	//unsigned int uiQueueToken;
	int iIsInQueue;
	int iQueuePrev;
	int iQueueNext;

	time_t tInQueue;
	time_t tOutQueue;
	int iSendFirstFrame;
	int iSendFirstPkg;

	int iNeedFree;
	int iNotify;
	int iVersion;

	int iRegionID;
	int iWorldID;

	//int iShortCountBuff;

	char szBuff[1];
}ChatMonConnInst;

typedef struct 
{
	char szTextGBK[MAX_GBK_LENG];
}GbkText;

struct tagChatMonDBInfo
{
	int iRegionID;
	int iKeepAlive;
	TDRDBHANDLE dbHandle;
};

typedef struct tagChatMonDBInfo ChatMonDBInfo;

struct tagChatMonMetaInfo
{
	char szMetaName[MAX_DB_META_NAME_LEN];
	int iObjSize;
	char* pszObj;
	char szWhere[MAX_CHAT_MON_WHERE_LEN];
	int iFailRecord;	//是否失败时记录
	int iReverseOrder;	//倒序
};

typedef struct tagChatMonMetaInfo ChatMonMetaInfo;

typedef struct tagThreadMgr
{
	pthread_t tid;
	int iIdx;
	LPTBUSCHANNEL	pstChannel0;
	LPTBUSCHANNEL	pstChannel1;
	char cIsExit;
	char cIsValid;
	//MYSQL *pstConn;
	int iLastDBPing;
	int epfd;
	int iMsRecv;	/* the milli-second usecd to receive up message. */
	int iMsRecvPerLoop;
	int iRecvSlices;  /* time slices to receive up message*/
	int iWaitToSend; /*byte wait to send in down channel   */
	
}ChatMonThreadMgr;

enum enumChatMonThread
{
	CHAT_MON_THREAD_SOAP = 0,
	CHAT_MON_THREAD_TCP,
	CHAT_MON_THREAD_NUM,
};

typedef struct tagChatMonConnInfo
{
	int iSocket;	
	int iInstIdx;
}ChatMonConnInfo;

typedef struct tagLogBuffInfo
{
	int iNum;	
	int iBegin;
	LOGROLECHATMON astLog[MAX_CHAT_LOG_BUFF_NUM];
}LogBuffInfo;



struct tagchat_mon_serviceEnv
{
	CHAT_MON_SERVICECONF *pstConf;
	CHAT_MON_SERVICERUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	TAPPCTX *pstAppCtx;
	pthread_mutex_t stLogMutex;
	pthread_mutex_t stDirtyMutex;
	LPSSHANDLE pstSSHan;
	struct soap soap;
	int *pCgiHandle;
	struct ns1__ErrorResponse stErrorResponse;
	char szErrorText[MAX_CHAT_MON_ERROR_LEN];
	char szStrXML[MAX_STR_XML_LEN];
	char szStrXMLUtf8[MAX_STR_XML_LEN];
	int iGetFindHandleErr;
	int iTmp1;
	int iGbkTextUsedNum;
	GbkText aGbkText[MAX_GBK_TEXT_NUM];
	char szLogPath[MAX_LOG_PATH_LENG];
	char szSql[MAX_DB_SQL_LEN];
	char szOpLogSql[MAX_DB_SQL_LEN];
	TDRDBOBJECT stDBObj;
	ACCDBFAILEDOPLOG stOpLog;
	ChatMonDBInfo stLogDBInfo;
	int iErrCode;
	int iClientIP;
	int iClientTimeStamp;
	LPTDRMETALIB pstRoleDBMetaLib;
	struct sigaction stTappAct;
	int iThread;
	ChatMonThreadMgr astThread[CHAT_MON_THREAD_NUM];
	int iConnNum;
	CHATMONCONNPOOL* pstPool;
	OPLOGMONPKG stOpLogMonPkg;
	LPTDRMETA pstOpLogMonMeta;
	LPTDRMETA pstOpLogMeta;
	int iOpLogMonMetaVersion;
	PDULENTDRPARSERINST stTDRParser; 
	int iUnitSize;
	int iPoolUnitSize;
	int iConnLimit;
	TLOGBINHEAD stLogBinHead;
	OP_LOG_META stOpLogPkg;
	char szLogBinPgkBuff[TLOGBIN_MAX_DATA + sizeof(TLOGBINHEAD)];
	int iLogBinPgkBuff;
	ChatMonConnInfo astChatMonConnInfo[MAX_LOG_DB_CONN_NUM];
	int iPreHeatBeatTime;
	int iIsExit;
	int iLastInsertID;
	LogBuffInfo stLogBuffInfo;	
	char szLastErrLog[MAX_CHAT_MON_ERROR_LEN];
};
extern int tdr_sizeinfo_to_off_i(LPTDRSIZEINFO a_pstRedirector, LPTDRMETA a_pstMeta, int a_iEntry, const char* a_pszName);

#define TDR_GET_INT_NET(i, iSize, p)		switch(iSize)		      \
{									      \
	case 2:								      \
		i = (int)ntohs(*(unsigned short*)(p));			      \
		break;							      \
	case 4:								      \
		i = (int)ntohl(*(unsigned long*)(p));			      \
		break;							      \
	case 8:									\
		i = tdr_ntohq(*(tdr_ulonglong*)p);	\
		break;												\
	default:							      \
		i = (int)*(unsigned char*)(p);				      \
}

typedef struct tagchat_mon_serviceEnv		CHAT_MON_SERVICEENV;
typedef struct tagchat_mon_serviceEnv		*LCHAT_MON_SERVICEENV;

static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_chat_mon_service[];
extern unsigned char g_szMetalib_proto_ss[];


CHAT_MON_SERVICEENV gs_stChatMonEnv;
static CHAT_MON_SERVICERUN_STATUS gs_stStat;

typedef enum 
{
	MUTI_LOGLEVEL_MIN = 0,	
	MUTI_LOGLEVEL_TRACE,	
	MUTI_LOGLEVEL_DEBUG,	
	MUTI_LOGLEVEL_INFO,
	MUTI_LOGLEVEL_WARN,	
	MUTI_LOGLEVEL_ERROR,
	MUTI_LOGLEVEL_SEVERE,
}MUTILOGLEVEL;

#define ChatMonMutiLog(iLevel, szFormat, args...) \
		do{\
			pthread_mutex_lock(&gs_stChatMonEnv.stLogMutex); \
			snprintf(gs_stChatMonEnv.szLastErrLog,sizeof(gs_stChatMonEnv.szLastErrLog),szFormat, ##args);\
			switch(iLevel)\
			{\
			case MUTI_LOGLEVEL_TRACE:\
				tlog_trace(gs_stChatMonEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_DEBUG:\
				tlog_debug(gs_stChatMonEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_INFO:\
				tlog_info(gs_stChatMonEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_WARN:\
				tlog_warn(gs_stChatMonEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_ERROR:\
				tlog_error(gs_stChatMonEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_SEVERE:\
				tlog_fatal(gs_stChatMonEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			default:\
				break;\
			}\
			pthread_mutex_unlock(&gs_stChatMonEnv.stLogMutex); \
		  }while(0)

#define ChatMonLastErrLog(szFormat, args...) \
		do{\
			snprintf(gs_stChatMonEnv.szLastErrLog,sizeof(gs_stChatMonEnv.szLastErrLog),szFormat, ##args);\
		  }while(0)

#define CHAT_MON_ERR_TEXT(pstErrRes, szFormat, args...) \
		do{\
			if (!pstErrRes)\
			{\
				pstErrRes = &gs_stChatMonEnv.stErrorResponse;\
			}\
			if (!pstErrRes->ErrorText)\
			{\
				pstErrRes->ErrorText = gs_stChatMonEnv.szErrorText;\
			}\
			if (pstErrRes->ErrorText)\
			{\
				snprintf(pstErrRes->ErrorText,MAX_CHAT_MON_ERROR_LEN,szFormat,##args);\
			}\
		  }while(0);	

int chat_mon_db_done(CHAT_MON_SERVICEENV *pstEnv);
int chat_mon_service_init(TAPPCTX *pstAppCtx, CHAT_MON_SERVICEENV *pstEnv);
int chat_mon_service_fini(TAPPCTX *pstAppCtx, CHAT_MON_SERVICEENV *pstEnv);
int chat_mon_oplog_limit_init(ACCDBFAILEDOPLOG* pstOpLog);
int chat_mon_heart_beat_to_log_db(CHAT_MON_SERVICEENV* pstEnv);
void* thread_proc_tcp(void* pvArg);
int chat_mon_db_op(CHAT_MON_SERVICEENV *pstEnv,int iRegionID,int iOpType,ChatMonMetaInfo* pstMetaInfo,
	TDRDBOBJECT* pstFetchDBObj,int* piFetchNum,int iFetchLimit,int* piFetchRegion);
int chat_mon_get_chat_mon_log(CHAT_MON_SERVICEENV* pstEnv,int iSeq,int iType,LOGROLECHATINFO *pstRes);


int chat_mon_get_log_path(CHAT_MON_SERVICEENV *pstEnv)
{
	int iLen = strlen(pstEnv->pstAppCtx->pszLogFile) - strlen(pstEnv->pstAppCtx->pszApp);

	pstEnv->szLogPath[0] = 0;
	if (iLen <= 0 || iLen+1 > MAX_LOG_PATH_LENG)
	{
		return 0;
	}
	STRNCPY(pstEnv->szLogPath,pstEnv->pstAppCtx->pszLogFile, iLen + 1);
	return 0;
}

int chat_mon_parse_args(int argc, char **argv)
{
   int c;
   CHAT_MON_SERVICEENV *pstEnv = &gs_stChatMonEnv;
   //int digit_optind = 0;

   while (1) {
	   //int this_option_optind = optind ? optind : 1;
	   int option_index = 0;
	   static struct option long_options[] = {
		   {"print-spawn-args", 1, 0, 1},
		   {"print-spawn-num", 1, 0, 2},
		   {"generate-spawn-conf", 1, 0, 3},
		   {"help", 0, 0, 0},
		   {0, 0, 0, 0}
	   };
		opterr = 0;
	   c = getopt_long(argc, argv, "",
				long_options, &option_index);
	   if (c == -1)
		   break;

	   switch (c) {
	   case 1:
			if (option_index == 0 && optarg)
			{
				
				FILE *fp = fopen(optarg, "w");
				if (fp)
				{
					fprintf(fp,"-a%s -p%d",pstEnv->pstConf->SpawnIP,
						pstEnv->pstConf->SpawnPort);
					
					fclose(fp); 
				}
				
			}
			exit(EXIT_SUCCESS);
		   break;
		case 2:
			if (optarg)
			{
				
				FILE *fp = fopen(optarg, "w");
				if (fp)
				{
					fprintf(fp,"%d",pstEnv->pstConf->SpawnNum);
					
					fclose(fp); 
				}
				
			}
			exit(EXIT_SUCCESS);
		   break;
		case 3:
			if (optarg)
			{
				TDRDATA stHost;
				
				LPTDRMETA pstMeta = NULL;
				//FILE *fp = NULL;
				//fp = fopen(optarg, "w");
				//if (fp)
				{
					if (pstEnv->pstAppCtx->iBusinessID)
					{
						pstEnv->pstConf->SpawnPort += pstEnv->pstAppCtx->iBusinessID - 1;
					}
					stHost.iBuff = sizeof(*pstEnv->pstConf);
					stHost.pszBuff = (char*) pstEnv->pstConf;
						

					pstMeta = tdr_get_meta_by_name((LPTDRMETALIB)g_szMetalib_chat_mon_service, 
						pstEnv->pstAppCtx->stConfData.pszMetaName);
					if (pstMeta)
					{
						tdr_output_file(pstMeta,optarg,&stHost, 0, TDR_IO_NEW_XML_VERSION);
					}
				}
				
			}
			exit(EXIT_SUCCESS);
		   break;

	   //case '?':
		   //break;

	   default:
		   //printf("?? getopt returned character code 0%o ??\n", c);
		   break;
		   
	   }
   }


   //exit(EXIT_SUCCESS);
   return 0;
}

int chat_mon_soap_done(CHAT_MON_SERVICEENV *pstEnv)
{
	
	pstEnv->iGbkTextUsedNum = 0;

	pstEnv->iErrCode = 0;
	pstEnv->szLastErrLog[0] = 0;
	return 0;
}



int chat_mon_service_heart_beat_tick(TAPPCTX *pstAppCtx, CHAT_MON_SERVICEENV *pstEnv)
{

	int iNow = pstAppCtx->stCurr.tv_sec;


	if (0 == pstEnv->iPreHeatBeatTime)
	{
		pstEnv->iPreHeatBeatTime = iNow;
		return 0;
	}

	if (iNow - pstEnv->iPreHeatBeatTime < pstEnv->pstConf->HeartBeatInterval)
	{
		return 0;
	}

	chat_mon_heart_beat_to_log_db(pstEnv);

	pstEnv->iPreHeatBeatTime = iNow;
	
	return 0;
}

int chat_mon_service_channel_proc(TAPPCTX *pstAppCtx, CHAT_MON_SERVICEENV *pstEnv)
{
	struct timeval stStart;
	struct timeval stEnd;
	struct timeval stSub;
	int iMsRecv = 0;
	int iIdx = 0;
	ChatMonMetaInfo stMetaInfo;
	LogBuffInfo *pstLogBuffInfo = &pstEnv->stLogBuffInfo;
	LOGROLECHATMON *pstLog = NULL;
	int iRet = 0;
	int iLen = 0;
	int iBegin = 0;

	memset(&stStart,0,sizeof(stStart));
	memset(&stEnd,0,sizeof(stEnd));

	gettimeofday(&stStart, NULL);

	iIdx = pstLogBuffInfo->iNum;
	if (iIdx >= MAX_CHAT_LOG_BUFF_NUM)
	{
		iIdx = pstLogBuffInfo->iBegin;
		iBegin = iIdx + 1;
	}

	pstLog = &pstLogBuffInfo->astLog[iIdx];
	iLen = sizeof(*pstLog);

	while (1)
	{
		iRet = tbus_channel_recv(pstEnv->astThread[CHAT_MON_THREAD_TCP].pstChannel0,
					pstLog, &iLen, 0);
		if (0 != iRet)
		{
			if ((int)TBUS_ERR_CHANNEL_EMPTY == iRet)
			{
				return 0;
			}
			else
			{
				ChatMonMutiLog(MUTI_LOGLEVEL_ERROR, "tbus_channel_recv error %d:%s", iRet,
					tbus_error_string(iRet));
				return 0;
			}				
		}

		if (iLen < (int)sizeof(*pstLog))
		{
			ChatMonMutiLog(MUTI_LOGLEVEL_ERROR, "tbus_channel_recv error iLen %d", iLen);
			continue;
		}

		memset(&stMetaInfo,0,sizeof(stMetaInfo));
	
		STRNCPY(stMetaInfo.szMetaName,"LogRoleChatMon",sizeof(stMetaInfo.szMetaName));
		stMetaInfo.iObjSize = sizeof(*pstLog);
		stMetaInfo.pszObj = (char*) pstLog;
		//stMetaInfo.iFailRecord = 1;
		//snprintf(stMetaInfo.szWhere,sizeof(stMetaInfo.szWhere),"where ");

		iRet = chat_mon_db_op(pstEnv,
			0,
			TDR_DBOP_INSERT,
			&stMetaInfo,
			NULL,
			NULL,
			0,
			NULL
			);

		if(iRet < 0)
		{
			iRet = pstEnv->iErrCode;
			ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"chat_mon_db_op error:%d",iRet);
		}
		pstLog->Seq = pstEnv->iLastInsertID;

		pstLogBuffInfo->iNum++;
		if (pstLogBuffInfo->iNum > MAX_CHAT_LOG_BUFF_NUM)
		{
			pstLogBuffInfo->iNum = MAX_CHAT_LOG_BUFF_NUM;
			pstLogBuffInfo->iBegin = iBegin;
		}

		gettimeofday(&stEnd, NULL);
	    TV_DIFF(stSub, stEnd, stStart);
		TV_TO_MS(iMsRecv, stSub);	

		if (iMsRecv > pstEnv->pstAppCtx->iTimer/2)
		{
			return 0;
		}
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}


int chat_mon_service_proc(TAPPCTX *pstAppCtx, CHAT_MON_SERVICEENV *pstEnv)
{
	int iBusy=0;

	chat_mon_service_heart_beat_tick(pstAppCtx,pstEnv);
	
	chat_mon_service_channel_proc(pstAppCtx,pstEnv);
		
	if (iBusy)
	{
		return 0;
	}
	else
	{
		return -1;
	}
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}

/*
int chat_mon_service_proc(TAPPCTX *pstAppCtx, CHAT_MON_SERVICEENV *pstEnv)
{
	
	int iBusy=0;
	struct soap *soap = &pstEnv->soap;

#ifndef WITH_FASTCGI
	unsigned int k = soap->max_keep_alive;
#endif
	//do
	//{
#ifndef WITH_FASTCGI
		if (soap->max_keep_alive > 0 && !--k)
			soap->keep_alive = 0;
#endif
		if (soap_begin_serve(soap))
		{
			//if (soap->error >= SOAP_STOP)
				//continue;
			goto err;
		}
		if (soap_serve_request(soap) || (soap->fserveloop && soap->fserveloop(soap)))
		{
#ifdef WITH_FASTCGI
			soap_send_fault(soap);
#else
			//return soap_send_fault(soap);
			soap_send_fault(soap);
#endif
		}

#ifdef WITH_FASTCGI
		//soap_destroy(soap);
		//soap_end(soap);
	//} while (1);
#else
	//} while (soap->keep_alive);
#endif
	//return SOAP_OK;

final:	

	
	
	soap_destroy(soap);
	//soap_end(soap);
	chat_mon_soap_done(pstEnv);

	if (iBusy)
	{
		return 0;
	}
	else
	{
		return -1;
	}
	
err:
	ChatMonMutiLog(MUTI_LOGLEVEL_ERROR, "soap err:%d", soap->error);
	goto final;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}*/

int chat_mon_service_reload(TAPPCTX *pstAppCtx, CHAT_MON_SERVICEENV *pstEnv)
{
	CHAT_MON_SERVICECONF   *pstPreConf;
	
	pstPreConf = (CHAT_MON_SERVICECONF   *)pstAppCtx->stConfPrepareData.pszBuff;
	
	printf("chat_mon_service reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "chat_mon_service reload");

	return 0;
}

int chat_mon_service_tick(TAPPCTX *pstAppCtx, CHAT_MON_SERVICEENV *pstEnv)
{
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}

int chat_mon_service_stop(TAPPCTX *pstAppCtx, CHAT_MON_SERVICEENV *pstEnv)
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

/*onidle:修改epoll wait 时间并sleep一会*/
int chat_mon_service_idle(TAPPCTX* pstAppCtx, CHAT_MON_SERVICEENV* pstEnv)
{
	UNUSED(pstEnv);

	pstAppCtx->iEpollWait = CHAT_MON_CONND_WAIT_TIME;
	usleep(pstAppCtx->iIdleSleep*1000);
	return 0;
}


int main(int argc, char **argv)
{ 
	//SOAP_SOCKET m, s; /* master and slave sockets */	//comment by paraunused.pl
	int iRet;
	void* pvArg	=	&gs_stChatMonEnv;

	memset(&gs_stAppCtx, 0, sizeof(gs_stAppCtx));
	memset(&gs_stChatMonEnv, 0, sizeof(gs_stChatMonEnv));

	gs_stAppCtx.argc	=	argc;
	gs_stAppCtx.argv	=	argv;

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)chat_mon_service_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_chat_mon_service;
	gs_stAppCtx.stConfData.pszMetaName = "chat_mon_serviceconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);
		
	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	chat_mon_parse_args(argc,argv);

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)chat_mon_service_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)chat_mon_service_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)chat_mon_service_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)chat_mon_service_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)chat_mon_service_stop;
	gs_stAppCtx.pfnIdle =   (PFNTAPPFUNC)chat_mon_service_idle;
	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	tapp_def_fini(&gs_stAppCtx, pvArg);
	

	
	return 0;
} 

int chat_mon_get_gbk_str_in(char ** ppIn,size_t * ppInLen,char** ppOut,size_t * ppOutLen)
{
	int iConvRet = 0;
	
	iconv_t cd = iconv_open("gbk","utf-8");

	iConvRet = iconv(cd, ppIn, ppInLen,ppOut, ppOutLen);
	iconv_close(cd);

	return iConvRet;
}


char* chat_mon_get_gbk_str(char * pszUTF8)
{
	size_t sInLen = strlen(pszUTF8);
	size_t sOutLen = sizeof(GbkText);
	char *pIn = pszUTF8;
	char *pOut = NULL;
	int iConvRet = 0;
	GbkText *pstText = NULL;
	CHAT_MON_SERVICEENV* pstEnv = &gs_stChatMonEnv;

	if (pstEnv->iGbkTextUsedNum < 0 ||
		pstEnv->iGbkTextUsedNum >= MAX_GBK_TEXT_NUM)
	{
		return NULL;
	}
	
	pstText = &pstEnv->aGbkText[pstEnv->iGbkTextUsedNum];
	pstEnv->iGbkTextUsedNum++;
	
	pOut = pstText->szTextGBK;	
	*pOut = 0;

	//memset(pstText,0,sizeof(*pstText));
	
	iConvRet =chat_mon_get_gbk_str_in(&pIn, &sInLen,&pOut, &sOutLen);
	
	if (iConvRet < 0)
	{			
		return NULL;
	}
	else
	{
		int iLen = sizeof(pstText->szTextGBK) - sOutLen;
		iLen = min(iLen,(int)sizeof(pstText->szTextGBK) - 1);

		if (iLen >= 0)
		{
			pstText->szTextGBK[iLen] = 0;
		}
		else
		{
			iLen = sizeof(pstText->szTextGBK) - 1;
			pstText->szTextGBK[iLen] = 0;
		}
	}

	return pstText->szTextGBK;
}

int chat_mon_sprint_xml(
	char **ppStrXml,
	const char *meta_name,
	const void *data,
	int data_size)
{
	int iRet = 0;
	int iLen = 0;

	TDRDATA host_data;
	TDRDATA net_data;
	LPTDRMETA pstMeta;

	CHAT_MON_SERVICEENV* pstEnv = &gs_stChatMonEnv;

	host_data.pszBuff = (char *)data;
	host_data.iBuff = data_size;

	net_data.pszBuff = gs_stChatMonEnv.szStrXML;
	net_data.iBuff = sizeof(gs_stChatMonEnv.szStrXML);

	//memset(gs_stChatMonEnv.szStrXML,0,sizeof(gs_stChatMonEnv.szStrXML));
	pstEnv->szStrXML[0] = 0;
	
	*ppStrXml = gs_stChatMonEnv.szStrXML;

	//iRet = cs_sprint_xml(gs_stChatMonEnv.pCgiHandle,meta_name,data,data_size,
		//gs_stChatMonEnv.szStrXML,sizeof(gs_stChatMonEnv.szStrXML));
	pstMeta = tdr_get_meta_by_name(pstEnv->pstSSHan->pstSSProtoLib,meta_name);
	if (!pstMeta)
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"tdr_get_meta_by_name fail,meta_name:%s",meta_name);
		return -1;
	}
	iRet = tdr_output(pstMeta,&net_data, &host_data,0, TDR_IO_NEW_XML_VERSION);

	if (0 == iRet)
	{
		iLen = net_data.iBuff;
		
		iLen = min(iLen,MAX_STR_XML_LEN -1);
		if (iLen >= 0 && iLen <MAX_STR_XML_LEN)
		{
			pstEnv->szStrXML[iLen] = 0;
		}
	}
	else
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,tdr_error_string(iRet));
		return iRet ;
	}

	if (0 == iRet && pstEnv->pstConf->SoapEncoding == SOAP_ENCODING_TYPE_UTF8)
	{
		size_t sInLen = strlen(pstEnv->szStrXML);
		size_t sOutLen = sizeof(pstEnv->szStrXMLUtf8);
		char *pIn = pstEnv->szStrXML;
		char *pOut = pstEnv->szStrXMLUtf8;
		int iConvRet = 0;
		iconv_t cd = 0;

		//memset(gs_stChatMonEnv.szStrXMLUtf8,0,sizeof(gs_stChatMonEnv.szStrXMLUtf8));
		pstEnv->szStrXMLUtf8[0] = 0;
		
		
		cd = iconv_open("utf-8","gbk");

		iConvRet = iconv(cd, &pIn, &sInLen,&pOut, &sOutLen);
		if (iConvRet < 0)
		{
			*ppStrXml = NULL;
			iRet = -2;
		}
		else
		{
			*pOut = 0;
			*ppStrXml = pstEnv->szStrXMLUtf8;
			iLen = sizeof(pstEnv->szStrXMLUtf8) - sOutLen;
	
			iLen = min(iLen,MAX_STR_XML_LEN -1);
			if (iLen >= 0 && iLen <MAX_STR_XML_LEN)
			{
				pstEnv->szStrXMLUtf8[iLen] = 0;
			}
		}
		
		//printf("inlen = %d,outlen = %d\n\n", sInLen,sOutLen);
		//printf("locale = ChatMonService = ChatMonService,chat_mon_service = chat_mon_service,CHAT_MON_SERVICE = CHAT_MON_SERVICE,chat_mon_ = chat_mon_,CHAT_MON_ERR = CHAT_MON_ERR\n\n", setlocale(LC_CTYPE,""));
		//printf("result = ChatMonService = ChatMonService,chat_mon_service = chat_mon_service,CHAT_MON_SERVICE = CHAT_MON_SERVICE,chat_mon_ = chat_mon_,CHAT_MON_ERR = CHAT_MON_ERR\n\n", szRet);
		iconv_close(cd);
	}

	return iRet;
}


int chat_mon_service_fini(TAPPCTX *pstAppCtx, CHAT_MON_SERVICEENV *pstEnv)
{
	int i = 0;
	void *pvRet;
	ChatMonThreadMgr* pstThread = NULL;
	
	pstEnv->iIsExit = 1;

	for(i=0; i<pstEnv->iThread; i++)
	{
		pstThread	=	&pstEnv->astThread[i];

		if( pstThread->cIsValid )
		{
			pthread_join(pstThread->tid, &pvRet);
		}
		tbus_close_channel(&pstThread->pstChannel0);
		tbus_close_channel(&pstThread->pstChannel1);
	}
	
	chat_mon_db_done(pstEnv);

	soap_end(&gs_stChatMonEnv.soap);
	
	printf("service_svr stop\n");
	ChatMonMutiLog(MUTI_LOGLEVEL_INFO, "service_svr stop");
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}

int chat_mon_db_init(CHAT_MON_SERVICEENV *pstEnv)
{
	//int i = 0; 	//comment by paraunused.pl
	ChatMonDBInfo* pstInfo = NULL;
	
	int iRet = 0;
	TDRDBMS stDBMS;
	TORMCONNECTDBINFO *pstConnInfo = NULL;

	{
		pstConnInfo = &pstEnv->pstConf->ServiceDBInfo;
		pstInfo = &pstEnv->stLogDBInfo;
		pstInfo->iRegionID = 0;
		
		memset(&stDBMS, 0, sizeof(stDBMS));
		
		STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
		STRNCPY(stDBMS.szDBMSCurDatabaseName,pstConnInfo->DBMSCurDatabaseName, 
				sizeof(stDBMS.szDBMSCurDatabaseName));
		STRNCPY(stDBMS.szDBMSConnectionInfo, pstConnInfo->DBMSConnectionInfo, 
				sizeof(stDBMS.szDBMSConnectionInfo));	
		STRNCPY(stDBMS.szDBMSUser, pstConnInfo->DBMSUser, 
				sizeof(stDBMS.szDBMSUser));
		STRNCPY(stDBMS.szDBMSPassword, pstConnInfo->DBMSPassword,
				sizeof(stDBMS.szDBMSPassword));
		
		stDBMS.iReconnectOpt = 1;	
	
		iRet = tdr_open_dbhanlde(&pstInfo->dbHandle,&stDBMS,
					pstEnv->szErrorText);
		
		if (iRet < 0 || pstInfo->dbHandle == NULL)
		{
			ChatMonMutiLog(MUTI_LOGLEVEL_ERROR, "tdr_open_dbhanlde regionid:%d err:%s",
						pstInfo->iRegionID ,pstEnv->szErrorText);
			return -1;
		}
		pstInfo->iKeepAlive = 1;
	}
	
	
	

	return 0;
}

int chat_mon_db_done(CHAT_MON_SERVICEENV *pstEnv)
{
	//int i = 0;	//comment by paraunused.pl
	
	
	tdr_close_dbhanlde(&pstEnv->stLogDBInfo.dbHandle);

	memset(&pstEnv->stLogDBInfo,0,sizeof(pstEnv->stLogDBInfo));
	
	return 0;
	
}

int chat_mon_soap_prepare_init_recv(struct soap* soap)
{
	//int i = 0;	//comment by paraunused.pl
	int iRet = 0;
	
	
	iRet = tdr_keep_dbmsconnection(gs_stChatMonEnv.stLogDBInfo.dbHandle);
	if (iRet < 0)
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"tdr_keep_dbmsconnection err:%d",
			iRet);
	}
	else
	{
		gs_stChatMonEnv.stLogDBInfo.iKeepAlive = 1;
	}

	chat_mon_oplog_limit_init(&gs_stChatMonEnv.stOpLog);

	gs_stChatMonEnv.iGbkTextUsedNum = 0;
	
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

void chat_mon_on_sigusr1(int iSig)
{
	CHAT_MON_SERVICEENV *pstEnv = &gs_stChatMonEnv;
	int i = 0;

	pstEnv->iIsExit = 1;

	for(i=0; i<pstEnv->iThread; i++)
	{
		ChatMonThreadMgr* pstThread	=	&pstEnv->astThread[i];

		if( pstThread->cIsValid )
		{
			pthread_kill(pstThread->tid, iSig);
		}
	}
		
	OS_ShutdownPending();
	sigaction(SIGUSR1, &pstEnv->stTappAct,NULL);
	kill(getpid(),iSig);
	
}



void* thread_proc_soap(void* pvArg)
{
	CHAT_MON_SERVICEENV *pstEnv = &gs_stChatMonEnv;
	ChatMonThreadMgr *pstThread = NULL;	
	struct soap *soap = NULL;
	unsigned int k = 0;

	pstThread = (ChatMonThreadMgr *)pvArg;
	
	soap = &pstEnv->soap;

#ifndef WITH_FASTCGI
	k = soap->max_keep_alive;
#endif
	do
	{
		if (pstEnv->iIsExit)
		{
			goto end;
		}
#ifndef WITH_FASTCGI
		if (soap->max_keep_alive > 0 && !--k)
			soap->keep_alive = 0;
#endif
		if (soap_begin_serve(soap))
		{
			//if (soap->error >= SOAP_STOP)
				//continue;
			goto err;
		}
		if (soap_serve_request(soap) || (soap->fserveloop && soap->fserveloop(soap)))
		{
#ifdef WITH_FASTCGI
			soap_send_fault(soap);
#else
			//return soap_send_fault(soap);
			soap_send_fault(soap);
#endif
		}

final:			
		soap_destroy(soap);
		//soap_end(soap);
		chat_mon_soap_done(pstEnv);

#ifdef WITH_FASTCGI
			
		//soap_destroy(soap);
		//soap_end(soap);
	} while (1);
#else
	} while (soap->keep_alive);
#endif
	//return SOAP_OK;

err:
	ChatMonMutiLog(MUTI_LOGLEVEL_ERROR, "soap err:%d", soap->error);
	goto final;
	return 0;
	
end:
	ChatMonMutiLog(MUTI_LOGLEVEL_INFO, "thread_proc_soap exit");
	return 0;
	UNUSED(k);
}

int chat_mon_check_dirty(ChatMonThreadMgr* pstThread, ChatMonConnInst* pstInst,
						OPROLECHAT * pstRoleChat)
{
	CHAT_MON_SERVICEENV* pstEnv = &gs_stChatMonEnv;
	int iRet = 0;
	int iLv = 0;
	LOGROLECHATMON stLog;
	int iNow = pstEnv->pstAppCtx->stCurr.tv_sec;

	pthread_mutex_lock(&pstEnv->stDirtyMutex);
	
	iRet = tDirtyCheck(0, pstRoleChat->Content, &iLv);
	
	pthread_mutex_unlock(&pstEnv->stDirtyMutex);

	if (iRet < 0)
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"tDirtyCheck err:%d",iRet);
		return -1;
	}
	else if (iRet == 0)
	{
		return 0;
	}

	//dirty
	stLog.Seq = 0;
	stLog.OpRoleChat = *pstRoleChat;
	stLog.RegionID = pstInst->iRegionID;
	stLog.WorldID = pstInst->iWorldID;
	stLog.DirtyLevel = iRet;

	tdr_utctime_to_tdrdatetime (&stLog.LogTime, iNow);

	iRet = tbus_channel_send(pstThread->pstChannel1, 
									&stLog, sizeof(stLog), 0);
	if (0 != iRet)
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR, "tbus_channel_send error ret %d", iRet);
	}
	
	return 0;
	UNUSED(pstThread);	//add by paraunused.pl
}

int chat_mon_heart_beart_res(ChatMonThreadMgr* pstThread, ChatMonConnInst* pstInst,
						MONHEARTBEAT* pstRes)
{
	CHAT_MON_SERVICEENV* pstEnv = &gs_stChatMonEnv;	

	ChatMonConnInfo* pstInfo = NULL;
//	int iNetID = 0;
	char* pszRegionID = NULL;
	char* pszWroldID = NULL;

	unsigned int i = 0;

	if (pstRes->ID < 0 || pstRes->ID >= MAX_LOG_DB_CONN_NUM)
	{
		return -1;
	}
	
	pstInfo = &pstEnv->astChatMonConnInfo[pstRes->ID];

	if (pstInfo->iInstIdx >= 0 && pstInfo->iInstIdx != pstInst->iIdx)
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_INFO,"client reconnect,id = %d",pstRes->ID);

		chat_mon_connd_free_inst(pstEnv->pstPool,pstInfo->iInstIdx);
		pstInfo->iInstIdx = -1;
		pstEnv->iConnNum--;
	}
	
	pstInst->iID = pstRes->ID;
	pszRegionID = pstRes->AppID;
	for(i = 0; i < sizeof(pstRes->AppID); i++)
	{
		if (!pstRes->AppID[i])
		{
			break;
		}
		if (pstRes->AppID[i] == '.')
		{
			pstRes->AppID[i] = 0;
			pszWroldID = &pstRes->AppID[i+1];
			break;
		}
	}
	
	pstInst->iRegionID = atoi(pszRegionID);
	if (pszWroldID)
	{
		pstInst->iWorldID = atoi(pszWroldID);
	}
	
	//tbus_addr_aton(pstRes->AppID, &iNetID);
	
	
	//GET_REGION_ID(pstInst->iRegionID,iNetID);
	//GET_WORLD_ID(pstInst->iWorldID, iNetID);

	pstInfo->iInstIdx = pstInst->iIdx;
	
	return 0;
	UNUSED(pstThread);	//add by paraunused.pl
}

int chat_mon_tlog_db_msg(ChatMonThreadMgr* pstThread, ChatMonConnInst* pstInst,
						OPLOGMONPKG* pstMon)
{
	switch (pstMon->Head.Cmd)
	{
	case OP_LOG_MON_HEART_BEAT_REQ:
		break;
	case OP_LOG_MON_HEART_BEAT_RES:
		return chat_mon_heart_beart_res(pstThread,pstInst,&pstMon->Body.HeartBeatRes);
		break;
	case OP_LOG_MON_DATA:
		if (pstMon->Body.OpLogData.OpLogType == OP_LOG_ROLE_CHAT)
		{
			OPROLECHAT * pstRoleChat = &pstMon->Body.OpLogData.OpLog.RoleChat;
			return chat_mon_check_dirty(pstThread,pstInst,pstRoleChat);
		}
		else
		{
			return -1;
		}
		break;
	default:
		return -1;
		break;
	}
	return 0;
}

int chat_mon_init_heart_beat(CHAT_MON_SERVICEENV* pstEnv)
{
	int i = 0; 
	for (i = 0; i < pstEnv->pstConf->ChatLogDBConnNum;i++)
	{
		CHATMONLOGDBCONNINFO* pstConnInfo = &pstEnv->pstConf->ChatMonLogDBConnInfo[i];
		pstEnv->astChatMonConnInfo[i].iSocket = -1;
		pstEnv->astChatMonConnInfo[i].iInstIdx = -1;
		pstEnv->astChatMonConnInfo[i].iSocket = tnet_connect(pstConnInfo->Url, 0);
		if (0 > pstEnv->astChatMonConnInfo[i].iSocket)
		{
			ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"tnet connect error,name = %s url = %s",
							pstConnInfo->Name,pstConnInfo->Url);			
		}
	}
	

	
	return 0;
}

int chat_mon_heart_beat_to_log_db(CHAT_MON_SERVICEENV* pstEnv)
{
	int i = 0; 
	TDRDATA stNet;
	TDRDATA stHost;
	OP_LOG_META stOpLog;
	OPCHATMONHEARTBEATREQ *pstReq = &stOpLog.OpLog.OpChatMonHeartBeatReq;
	char szBuff[2*sizeof(OP_LOG_META)];

	stOpLog.OpLogFrom = 0;
	stOpLog.OpLogType = OP_LOG_CHAT_MON_HEART_BEAT_REQ;

	pstReq->Seq = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstReq->Version = pstEnv->iOpLogMonMetaVersion;
	snprintf(pstReq->ConnUrl,sizeof(pstReq->ConnUrl),"%s",
				pstEnv->pstConf->Listener.Url);
	
	for (i = 0; i < pstEnv->pstConf->ChatLogDBConnNum;i++)
	{
		CHATMONLOGDBCONNINFO* pstConnInfo = &pstEnv->pstConf->ChatMonLogDBConnInfo[i];
		
		if (pstEnv->astChatMonConnInfo[i].iSocket >= 0)
		{
			
			
			pstReq->ID = i;
			
			stHost.iBuff = sizeof(OP_LOG_META);
			stHost.pszBuff = (char *)&stOpLog;	
			stNet.iBuff = sizeof(szBuff);
			stNet.pszBuff = szBuff;

			if (0 > tdr_hton(pstEnv->pstOpLogMeta, &stNet, &stHost, TDR_METALIB_OP_LOG_META_VERSION))
			{
				ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"tdr_hton fail,name = %s url = %s",
									pstConnInfo->Name,pstConnInfo->Url);
			}
			else
			{

				TLOGBIN_INIT_HEAD(&pstEnv->stLogBinHead,0,0,0,0,
					pstEnv->iOpLogMonMetaVersion,stNet.iBuff,&pstEnv->pstAppCtx->stCurr);

				pstEnv->iLogBinPgkBuff = tlogbin_make_pkg(pstEnv->szLogBinPgkBuff,sizeof(pstEnv->szLogBinPgkBuff),
								&pstEnv->stLogBinHead,stNet.pszBuff,stNet.iBuff);
				
				if(tnet_sendall(pstEnv->astChatMonConnInfo[i].iSocket, pstEnv->szLogBinPgkBuff, pstEnv->iLogBinPgkBuff, 
			            1000) < 0)
				{
					ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"tnet_sendall fail,name = %s url = %s",
									pstConnInfo->Name,pstConnInfo->Url);
				}
			}
		}
		else
		{
			
			pstEnv->astChatMonConnInfo[i].iSocket = -1;
			pstEnv->astChatMonConnInfo[i].iSocket = tnet_connect(pstConnInfo->Url, 0);
			if (0 > pstEnv->astChatMonConnInfo[i].iSocket)
			{
				ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"tnet connect error,name = %s url = %s",
								pstConnInfo->Name,pstConnInfo->Url);			
			}
		}
	}
	return 0;
}


int chat_mon_init_tdrparser(CHAT_MON_SERVICEENV* pstEnv)
{

	int iRet ;
	TDRSIZEINFO stSizeInfo;
	LPPDULENTDRPARSERINST pstTDRParserInst; 

	

	pstTDRParserInst = &pstEnv->stTDRParser;
	pstTDRParserInst->pstPkg = tdr_get_meta_by_name(pstEnv->pstSSHan->pstSSProtoLib,CHAT_MON_PKG_NAME);
	if (!pstTDRParserInst->pstPkg)
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"tdr_get_meta_by_name failed,name:%s",CHAT_MON_PKG_NAME);
		return -1;
	}
	


	iRet = tdr_sizeinfo_to_off_i(&stSizeInfo, pstTDRParserInst->pstPkg, -1, "");
	if (TDR_SUCCESS == iRet)
	{
		pstTDRParserInst->iPkgLenNOff = stSizeInfo.iNOff;
		pstTDRParserInst->iPkgLenUnitSize = stSizeInfo.iUnitSize;
	}
	iRet = tdr_sizeinfo_to_off_i(&stSizeInfo, pstTDRParserInst->pstPkg, -1, "Head.HeadLen");
	if (TDR_SUCCESS == iRet)
	{
		pstTDRParserInst->iHeadLenNOff = stSizeInfo.iNOff;
		pstTDRParserInst->iHeadLenUnitSize = stSizeInfo.iUnitSize;
	}
	iRet = tdr_sizeinfo_to_off_i(&stSizeInfo, pstTDRParserInst->pstPkg, -1, "Head.BodyLen");
	if (TDR_SUCCESS == iRet)
	{
		pstTDRParserInst->iBodyLenNOff = stSizeInfo.iNOff;
		pstTDRParserInst->iBodyLenUnitSize = stSizeInfo.iUnitSize;
	}
	if ((0 >= pstTDRParserInst->iPkgLenUnitSize) && (
		(0 >= pstTDRParserInst->iHeadLenUnitSize) || (0 >= pstTDRParserInst->iBodyLenUnitSize)))
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"failed to get the entry contained length of PDU<%s>",CHAT_MON_PKG_NAME);
		return -1;
	}	
	

	pstEnv->iUnitSize =	tdr_get_meta_size(pstTDRParserInst->pstPkg);
	pstEnv->iUnitSize	=	(pstEnv->iUnitSize + 0x400 - 1)/0x400*0x400;

	pstEnv->iPoolUnitSize=	pstEnv->iUnitSize + (int) offsetof(ChatMonConnInst, szBuff); // + SHORT_COUNT_BUFF;
	pstEnv->iConnLimit = pstEnv->pstConf->ChatLogDBConnNum + CHAT_MON_CONN_NUM_SOFT_EDGE;
	
	iRet	=	chat_mon_connd_init_pool(&pstEnv->pstPool, pstEnv->iConnLimit, pstEnv->iPoolUnitSize);

	if( iRet<0 )
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR, "chat_mon_connd_init_pool failed, max=%d, unit=%d.\n", 
						pstEnv->iConnLimit, pstEnv->iPoolUnitSize);
		return -1;
	}


	return 0;
}


int chat_mon_connd_get_pkglen(ChatMonConnInst* pstInst, ChatMonThreadMgr* pstThread)
{
	CHAT_MON_SERVICEENV* pstEnv = &gs_stChatMonEnv;
	
	//TRANSINST* pstTransInst;
	//PDUINST* pstPDUInst;
	char* pszBuff;
	LPPDULENTDRPARSERINST pstTDRParser; 
	char *pszDataBase;

	pstTDRParser = &pstEnv->stTDRParser;

	pszDataBase = pstInst->szBuff + pstInst->iOff; 
	if( 0 < pstTDRParser->iPkgLenUnitSize )
	{
		if( pstInst->iData < (pstTDRParser->iPkgLenNOff + pstTDRParser->iPkgLenUnitSize))
		{
			return -1;
		}

		pszBuff	=	pszDataBase + pstTDRParser->iPkgLenNOff;

		TDR_GET_INT_NET(pstInst->iPkgLen, pstTDRParser->iPkgLenUnitSize, pszBuff);

	}
	else
	{
		if( pstInst->iData < (pstTDRParser->iHeadLenNOff + pstTDRParser->iHeadLenUnitSize))
		{
			return -1;
		}

		pszBuff		=	pszDataBase + pstTDRParser->iHeadLenNOff;

		TDR_GET_INT_NET(pstInst->iHeadLen, pstTDRParser->iHeadLenUnitSize, pszBuff);

		if( pstInst->iData < (pstTDRParser->iBodyLenNOff + pstTDRParser->iBodyLenUnitSize))
		{
			return -1;
		}

		pszBuff		=	pszDataBase + pstTDRParser->iBodyLenNOff;

		TDR_GET_INT_NET(pstInst->iPkgLen, pstTDRParser->iBodyLenUnitSize, pszBuff);

		pstInst->iPkgLen	+=	pstInst->iHeadLen;
	}

	if( pstInst->iPkgLen<=0 || pstInst->iPkgLen > pstEnv->iUnitSize)
	{
		pstInst->iNeedFree	=	STOP_REASON_BADPKG;
		pstInst->iNotify	=	1;
		return -1;
	}
	else
	{
		return 0;
	}
	UNUSED(pstThread);	//add by paraunused.pl
}


int chat_mon_connd_recv_pkg(ChatMonThreadMgr* pstThread, ChatMonConnInst* pstInst)
{
	CHAT_MON_SERVICEENV* pstEnv = &gs_stChatMonEnv;
	time_t tNow = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	int iCount;

	TDRDATA stPkg;
	TDRDATA stMsg;
	TDRDATA stHost;

	int iRet = 0;

	iCount	=	0;

	if( !pstInst->iPkgLen )
	{
		iRet = chat_mon_connd_get_pkglen(pstInst, pstThread);
	}

	if( 0 != iRet)
	{
		return iRet;
	}

	while( pstInst->iData >= pstInst->iPkgLen )
	{
		stPkg.pszBuff	=	pstInst->szBuff + pstInst->iOff;
		stPkg.iBuff	=	pstInst->iPkgLen;

		iRet		=	0;
		iCount++;

		//iRet	=	tconnd_pkg2msg(&stMsg, &stPkg, pstThread, pstInst);
		stMsg.pszBuff	=	stPkg.pszBuff;
		stMsg.iBuff	=	stPkg.iBuff;

		stHost.pszBuff = (char *)&pstEnv->stOpLogMonPkg;
		stHost.iBuff = sizeof(pstEnv->stOpLogMonPkg);
		
		iRet = tdr_ntoh(pstEnv->pstOpLogMonMeta,&stHost, &stMsg, pstInst->iVersion);
			
		if(0 != iRet)
		{
			ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"tdr_ntoh failed\n");
		}
		else
		{
        	iRet	=	chat_mon_tlog_db_msg(pstThread,pstInst,&pstEnv->stOpLogMonPkg);
			if( 0 != iRet)
			{
        		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"chat_mon_check_dirty failed\n");
			}
		}

		if( tNow > pstInst->tLastRecvMsgCheck + CHAT_MON_RECVCHECK_INTERVAL )
		{
			pstInst->tLastRecvMsgCheck	=	tNow;
			pstInst->iRecvMsg	=	0;
			pstInst->iRecvByte	=	0;
		}
		else
		{
			pstInst->iRecvMsg++;
			pstInst->iRecvByte	+=	stPkg.iBuff;
		}
		
		pstInst->iData	-=	pstInst->iPkgLen;
		pstInst->iOff	+=	pstInst->iPkgLen;
		pstInst->iPkgLen	=	0;

		iRet = chat_mon_connd_get_pkglen(pstInst, pstThread);

		if(0 != iRet)
		{
			break;
		}

	}/*while( pstInst->iData>=pstInst->iPkgLen )*/

	if( pstInst->iOff )
	{
		if( pstInst->iData )
		{
			memmove(pstInst->szBuff, pstInst->szBuff+pstInst->iOff, pstInst->iData);
		}

		pstInst->iOff	=	0;
	}

	return iCount;
}

int chat_mon_tconnd_safe_close(ChatMonThreadMgr* pstThread, int iIdx, int iReason, int iIsNotify)
{
	CHAT_MON_SERVICEENV* pstEnv = &gs_stChatMonEnv;
	ChatMonConnInst* pstInst;

	pstInst	=	chat_mon_connd_get_inst( pstEnv->pstPool, iIdx);

	if( !pstInst )
	{
		return -1;
	}

	if( pstInst->s>=0 )
	{
		tnet_close(pstInst->s);
		ChatMonMutiLog(MUTI_LOGLEVEL_INFO, "close the connection with Idx=%d, id=%d:client(%s:%d:%d), iReason:%d",
					iIdx, pstInst->iID, inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr), 
					NTOHS(((struct sockaddr_in*)&pstInst->stAddr)-> sin_port),pstInst->s, iReason);

		pstInst->s	=	-1;
	}
	
	//pstInst->tLastRecv = ;
	chat_mon_connd_free_inst(pstEnv->pstPool, iIdx);

	pstEnv->iConnNum--;

	return 0;
	UNUSED(pstThread);	//add by paraunused.pl
	UNUSED(iIsNotify);	//add by paraunused.pl
}


int chat_mon_tconnd_scan_idle(ChatMonThreadMgr* pstThread, int iIsShutdown)
{
	CHAT_MON_SERVICEENV* pstEnv = &gs_stChatMonEnv;

	int iMax;
	int i;
	TMEMBLOCK* pstBlock;
	ChatMonConnInst* pstInst;
	//LISTENERLIST* pstListenerList;
	//LISTENER* pstListener;
	struct epoll_event event;
	//int iIdle;	//comment by paraunused.pl


	iMax	=	TMEMPOOL_GET_CAP(pstEnv->pstPool);

	for(i=0; i<iMax; i++)
	{
		pstBlock	=	TMEMPOOL_GET_PTR(pstEnv->pstPool, i);

		if( !pstBlock->fValid )
		{
			continue;
		}

		pstInst		=	(ChatMonConnInst*)TMEMBLOCK_GET_DATA(pstBlock);

		if( pstInst->fListen && !iIsShutdown )
		{
			continue;
		}

		if( iIsShutdown )
		{
			epoll_ctl(pstThread->epfd, EPOLL_CTL_DEL, pstInst->s, &event);
      		chat_mon_tconnd_safe_close(pstThread, TMEMPOOL_GET_BLOCK_IDX(pstBlock), STOP_REASON_SHUTDOWN, 1);
		}
		else if( pstInst->iNeedFree )
		{
			chat_mon_tconnd_safe_close(pstThread, TMEMPOOL_GET_BLOCK_IDX(pstBlock), pstInst->iNeedFree, pstInst->iNotify);
		}
		else
		{
			
		}
	}

	return 0;
}


int chat_mon_add_conn(ChatMonThreadMgr* pstThread, int s, int iIsListen, struct sockaddr* pstAddr)
{
	CHAT_MON_SERVICEENV* pstEnv = &gs_stChatMonEnv;
	CHATMONLISTENER* pstListener;
	ChatMonConnInst* pstInst;
	time_t tNow = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	int iNeedFree = 0;
	
	int iRet;
	int iType;
	epoll_event_t e;

	pstListener	=	&pstEnv->pstConf->Listener;

	if( -1==s )
	{
		if( iIsListen )
		{
			ChatMonMutiLog(MUTI_LOGLEVEL_ERROR, "listen on %s failed.", pstListener->Url);
		}
		else
		{
			ChatMonMutiLog(MUTI_LOGLEVEL_ERROR, "accept from epoll failed.errorstring=%s",strerror(errno));
		}

		return -1;
	}

	if(pstEnv->iConnNum >= pstEnv->iConnLimit)
	{
		tnet_close(s);

		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,
		           "reach max conn limit %d+%d.",
		            pstEnv->pstConf->ChatLogDBConnNum,
		            CHAT_MON_CONN_NUM_SOFT_EDGE);
		
		iNeedFree = STOP_REASON_LIMIT;
		return -1;
	}

	iType	=	SOCK_STREAM;

	tsocket_get_type(s, &iType);
	
	iRet	=	tnet_set_nonblock(s, 1);
	if( iRet<0 )
	{
		tnet_close(s);
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR, "set socket to non-block failed. ");
		return -1;
	}

	if(iIsListen ) //accept的socket会继承
	{
		if( pstListener->SendBuff>0 )
		{
			tnet_set_sendbuff(s, pstListener->SendBuff);
		}
		if( pstListener->RecvBuff>0 )
		{
			tnet_set_recvbuff(s, pstListener->RecvBuff);
		}
	}

	//show_sock_opt(s);
	
	e.events	=	EPOLLIN;
	e.data.fd	=	chat_mon_connd_alloc_inst(pstEnv->pstPool);
	
	if(e.data.fd<0)
	{
		tnet_close(s);
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR, "alloc conn instance for listen socket failed.");
		return -1;
	}

	(&e.data.fd)[1]	=	s;
	
	pstInst		=	chat_mon_connd_get_inst(pstEnv->pstPool, e.data.fd);

	memset(pstInst, 0, offsetof(ChatMonConnInst, szBuff));

	pstInst->s		=	s;
	pstInst->iIdx		=	e.data.fd;
	pstInst->fListen	=	iIsListen;
	pstInst->iNeedFree  =   iNeedFree;


	if( SOCK_STREAM==iType )
	{
		pstInst->fStream	=	1;
	}
	else
	{
		pstInst->fStream	=	0;
	}

	if( !iIsListen && SOCK_STREAM==iType )
	{
		pstInst->fWaitFirstPkg	=	1;
	}
	else
	{
		pstInst->fWaitFirstPkg	=	0;
	}

	pstInst->iID		=	-1;

	pstInst->tCreate	=	tNow;
	pstInst->tLastRecv	=	tNow;
			
	pstInst->iQueuePrev	=	-1;
	pstInst->iQueueNext	=	-1;

	if( pstAddr )
	{
		memcpy(&pstInst->stAddr, pstAddr, sizeof(pstInst->stAddr));
	}
	else
	{
		memset(&pstInst->stAddr, 0, sizeof(pstInst->stAddr));
	}

	pstInst->iBuff  	=	pstEnv->iUnitSize;

	if(!iNeedFree && -1 == epoll_ctl(pstThread->epfd, EPOLL_CTL_ADD, s, &e))
	{

		tnet_close(s);
		chat_mon_connd_free_inst(pstEnv->pstPool, e.data.fd);

		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR, "add listen socket to epoll-fd failed.\n");

		return -1;
	}

	if (!iIsListen)
	{
		pstEnv->iConnNum++;

		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR, "Add one socket(%d) to connInst(%d), peer info(%s:%d)",
			pstInst->s, pstInst->iIdx, inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr), 
				NTOHS(((struct sockaddr_in*)&pstInst->stAddr)->sin_port));
	}

	return 0;
}


int chat_mon_connd_proc_recv(ChatMonThreadMgr* pstThread)
{
	ChatMonConnInst* pstInst;
	int s;
	struct sockaddr sa;
	int iLen;
	struct epoll_event events[CHAT_MON_MAX_EVENTS];
	int iEvents;
	int i;
	CHATMONCONNPOOL* pstPool;
	CHAT_MON_SERVICEENV* pstEnv = &gs_stChatMonEnv;
	time_t tNow = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	struct timeval stStart;
	struct timeval stEnd;
	struct timeval stSub;
	int iRecvPackage=0;
	int iTotalRecv=0;

	memset(&stStart,0,sizeof(stStart));
	memset(&stEnd,0,sizeof(stEnd));

	pstThread->iMsRecv = 0;   
	iEvents	=	epoll_wait(pstThread->epfd, events, CHAT_MON_MAX_EVENTS, pstEnv->pstAppCtx->iEpollWait);

	if( iEvents<=0 )
	{
		if(0 != iEvents )
		{
        	ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"epoll_wait error errostring=%s\n", strerror(errno)); 
		}	  
		pstEnv->pstAppCtx->iEpollWait = 0;
		return 0;
	}
	
	gettimeofday(&stStart, NULL);

	pstPool		=	pstEnv->pstPool;

	
	for(i=0; i<iEvents; i++)
	{
		pstInst	=	chat_mon_connd_get_inst(pstPool, events[i].data.fd);

		/* bad packet. */
		if( !pstInst )
		{
			epoll_ctl(pstThread->epfd, EPOLL_CTL_DEL, (&events[i].data.fd)[1], events+i);
			ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"pstInst not find, idx=%d\n", events[i].data.fd);
			continue;
		}

		pstInst->tLastRecv	=	tNow;


		if( pstInst->fListen && pstInst->fStream )
		{
			iLen	=	(int) sizeof(sa);

			s	=	epoll_accept(pstThread->epfd, pstInst->s, events+i, &sa, &iLen);

			chat_mon_add_conn(pstThread, s,  0, &sa);
		}
		else
		{
			int iAddrLen;
			iAddrLen    =   (int) sizeof(pstInst->stAddr);

			if( pstInst->fStream )
			{
				iLen    =   epoll_recv(pstThread->epfd, pstInst->s, events+i, 
								pstInst->szBuff+pstInst->iData, 
								pstInst->iBuff - pstInst->iData);
				if( iLen<=0 )
				{		
					if( iLen<0 )
					{
						pstInst->iNeedFree	=	STOP_REASON_NETWORK;
						ChatMonMutiLog(MUTI_LOGLEVEL_INFO, "client  %s:%d exit unnormal or connection fail !need to free!errorstring =%s\n",
					 	                                                           inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr),
					 	                                                           NTOHS(((struct sockaddr_in*)&pstInst->stAddr)->sin_port),
					 	                                                           strerror(errno));

					}
					else
					{
						pstInst->iNeedFree	=	STOP_REASON_CLOSE;
						ChatMonMutiLog(MUTI_LOGLEVEL_DEBUG, "tconnd recv close package from %s:%d !\n",
					 	                                                           inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr),
					 	                                                           NTOHS(((struct sockaddr_in*)&pstInst->stAddr)->sin_port));
					}

					epoll_ctl(pstThread->epfd, EPOLL_CTL_DEL, pstInst->s, events+i); 

					ChatMonMutiLog(MUTI_LOGLEVEL_DEBUG, "EPOLL_CTL_DEL from %s:%d !\n",
					 	                                                           inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr),
					 	                                                           NTOHS(((struct sockaddr_in*)&pstInst->stAddr)->sin_port));

					pstInst->iNotify	=	1;
					
					continue;
				}
			}
			else
			{
				pstInst->iData  =   0;
				iLen    =   epoll_recvfrom(pstThread->epfd, pstInst->s, events+i, pstInst->szBuff+pstInst->iData, pstInst->iBuff - pstInst->iData, &pstInst->stAddr, &iAddrLen);
				if( iLen<=0 ) 
				{
				    continue;
				}
			}

			pstInst->iData	+=	iLen;

			if( pstInst->iPkgLen && pstInst->iData<pstInst->iPkgLen )
			{
				continue;
			}
			
    		ChatMonMutiLog(MUTI_LOGLEVEL_DEBUG, "tconnd recv package from %s:%d;datelen =%d\n",
			 	inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr),
			 	NTOHS(((struct sockaddr_in*)&pstInst->stAddr)->sin_port),
			 	pstInst->iData);
					 
			iRecvPackage = chat_mon_connd_recv_pkg(pstThread, pstInst);

			if( pstInst->iNeedFree )
			{
				epoll_ctl(pstThread->epfd, EPOLL_CTL_DEL, pstInst->s, events+i);

				ChatMonMutiLog(MUTI_LOGLEVEL_DEBUG, "EPOLL_CTL_DEL from %s:%d !\n",
                               inet_ntoa(((struct sockaddr_in*)&pstInst->stAddr)->sin_addr),
                               NTOHS(((struct sockaddr_in*)&pstInst->stAddr)->sin_port));

				chat_mon_connd_free_inst(pstEnv->pstPool,pstInst->iIdx);
				pstEnv->iConnNum--;
			}

			if(iRecvPackage > 0)
			{
            	iTotalRecv+=iRecvPackage;
			}
			
			if( iTotalRecv >= CHAT_MON_PAK_PER_SLICE )
			{
                 //check recv time every Package_Per_Slice package received
                 iTotalRecv = iTotalRecv - CHAT_MON_PAK_PER_SLICE;
                 gettimeofday(&stEnd, NULL);
       	         TV_DIFF(stSub, stEnd, stStart);
	             TV_TO_MS(pstThread->iMsRecv, stSub);	
			     if( pstThread->iMsRecv > pstThread->iMsRecvPerLoop)
			     {
                     return 0;
			     }
			}

		}
	}

	gettimeofday(&stEnd, NULL);
    TV_DIFF(stSub, stEnd, stStart);
	TV_TO_MS(pstThread->iMsRecv, stSub);	

	return 0;
}




int chat_mon_thread_init_tcp(CHAT_MON_SERVICEENV *pstEnv, ChatMonThreadMgr *pstThread)
{
	CHATMONLISTENER *pstListener = &pstEnv->pstConf->Listener;
	int s = -1;
	
	pstThread->epfd	=	epoll_create(pstEnv->iConnLimit);
	if( -1==pstThread->epfd )
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR, "create epoll-fd[MAXFD=%d] failed.errorstring=%s\n", pstEnv->iConnLimit,strerror(errno));

		return -1;
	}

	s	=	tnet_listen(pstListener->Url, pstListener->Backlog);

	pstThread->iRecvSlices = CHAT_MON_TIME_SLICE_COUNT/2;
	pstThread->iMsRecvPerLoop = (pstThread->iRecvSlices*pstEnv->pstAppCtx->iTimer)/CHAT_MON_TIME_SLICE_COUNT;
	pstThread->iWaitToSend=0;

	if( chat_mon_add_conn(pstThread, s,  1, NULL)<0 )
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR, "chat_mon_add_conn failed.\n");
		return -1;
	}

	
	
	return 0;
}


int chat_mon_thread_init_one(CHAT_MON_SERVICEENV *pstEnv, ChatMonThreadMgr *pstThread, int iIdx)
{
	int iKey;
	int iRet;
	
	if (0 == pstEnv->pstConf->ChannelKeyStart)
	{
		iKey = ftok(pstEnv->pstAppCtx->pszPidFile, iIdx);
	}
	else
	{
		iKey = pstEnv->pstConf->ChannelKeyStart + iIdx;
	}

	pstThread->iIdx = iIdx;
	if (0 > tbus_open_channel(&pstThread->pstChannel0,  pstEnv->pstAppCtx->iId, CHAT_MON_LOCAL_BUS_ADDR_START+iIdx,  iKey, CHAT_MON_CHANNEL_SIZE))
	{
		return -1;
	}

	if (0 > tbus_open_channel(&pstThread->pstChannel1, CHAT_MON_LOCAL_BUS_ADDR_START+iIdx, pstEnv->pstAppCtx->iId,   iKey, CHAT_MON_CHANNEL_SIZE))
	{
		return -1;
	}

	pstThread->cIsValid = 1;
	if (iIdx == CHAT_MON_THREAD_SOAP)
	{
		//先从数据库中读点放到缓存
		{
			LOGROLECHATINFO stRes;
			chat_mon_get_chat_mon_log(pstEnv,0,0, &stRes);
		}
		
		iRet = pthread_create(&pstThread->tid, NULL, thread_proc_soap, pstThread);
	}
	else if (iIdx == CHAT_MON_THREAD_TCP)
	{
		iRet = chat_mon_thread_init_tcp(pstEnv,pstThread);
		if (iRet == 0)
		{		
			iRet = pthread_create(&pstThread->tid, NULL, thread_proc_tcp, pstThread);
		}
	}
	else
	{
		return -1;
	}
		
	
	if( iRet <0 )
	{
		return -1;
	}
/*
	pstThread->pstConn = db_init(pstEnv);
	

	if (!pstThread->pstConn)
	{
		return -1;
	}

	pstThread->iLastDBPing = pstEnv->pstAppCtx->stCurr.tv_sec;
*/
	return 0;
}

int chat_mon_thread_init(CHAT_MON_SERVICEENV *pstEnv)
{
	int i;

	pstEnv->iThread = CHAT_MON_THREAD_NUM;

	for (i=0; i<pstEnv->iThread; i++)
	{
		if (0 > chat_mon_thread_init_one(pstEnv, &pstEnv->astThread[i], i))
		{
			return -1;
		}
	}
	
	return 0;
}


int chat_mon_service_init(TAPPCTX *pstAppCtx, CHAT_MON_SERVICEENV *pstEnv)
{
	char szLogFile[MAX_LOG_PATH_LENG];
	char szBid[64];
	
		

	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		return -1;
	}
	pstEnv->pstConf = (CHAT_MON_SERVICECONF *)pstAppCtx->stConfData.pszBuff;

	pstEnv->pstRunStat = (CHAT_MON_SERVICERUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
	if (NULL == pstEnv->pstRunStat)
	{
		pstEnv->pstRunStat = &gs_stStat;
	}

	pstEnv->pstAppCtx = pstAppCtx; 

	if(ss_han_create(g_szMetalib_proto_ss, &pstEnv->pstSSHan) < 0)
	{
		return -1;
	}

	pthread_mutex_init(&pstEnv->stLogMutex, NULL);
	pthread_mutex_init(&pstEnv->stDirtyMutex, NULL);
	if (0 > tapp_get_category(NULL, (void *)(&pstEnv->pstLogCat)))	
	{		
		printf("tapp_get_category run fail\n");
		return -1;	
	}	

/*	pstEnv->iFindHandleNum = pstEnv->pstConf->FindHandleNum;
	pstEnv->pstFindHandle = NULL;
	if (pstEnv->iFindHandleNum > 0)
	{
		pstEnv->pstFindHandle = calloc(pstEnv->iFindHandleNum,
			sizeof(*pstEnv->pstFindHandle));
	}
	*/

	if (0 > tdr_load_metalib(&pstEnv->pstRoleDBMetaLib, pstEnv->pstConf->RoleDBMetaFile))
	{
		printf("load meta lib role_db_meta fail\n");
		return -1;
	}


	setenv("FCGI_WEB_SERVER_ADDRS",pstEnv->pstConf->FcgiWebServerIPList,1);

	srand((unsigned)time( NULL ));

	szBid[0] = 0;
	if (pstEnv->pstAppCtx->iBusinessID)
	{
		snprintf(szBid,sizeof(szBid),"_%d",pstEnv->pstAppCtx->iBusinessID);
	}

	soap_init1(&pstEnv->soap,SOAP_C_UTFSTRING|SOAP_XML_IGNORENS);
	//soap_init1(&pstEnv->soap,SOAP_C_MBSTRING|SOAP_XML_IGNORENS);
	chat_mon_get_log_path(pstEnv);
	snprintf(szLogFile,sizeof(szLogFile),"%s%s%s%s",
		pstEnv->szLogPath,
		pstEnv->pstAppCtx->pszApp,
		szBid,
		"_soap_test.log");
	
#ifdef SOAP_DEBUG
	soap_set_test_logfile(&pstEnv->soap,szLogFile );
#endif

	snprintf(szLogFile,sizeof(szLogFile),"%s%s%s%s",
		pstEnv->szLogPath,
		pstEnv->pstAppCtx->pszApp,
		szBid,
		"_soap_sent.log");

#ifdef SOAP_DEBUG
	soap_set_sent_logfile(&pstEnv->soap, szLogFile);
#endif

	snprintf(szLogFile,sizeof(szLogFile),"%s%s%s%s",
		pstEnv->szLogPath,
		pstEnv->pstAppCtx->pszApp,
		szBid,
		"_soap_recv.log");

#ifdef SOAP_DEBUG
	soap_set_recv_logfile(&pstEnv->soap, szLogFile);
#endif

	pstEnv->soap.fprepareinitrecv = chat_mon_soap_prepare_init_recv;
	
  
	//soap_init1(&gs_stChatMonEnv.soap,SOAP_C_MBSTRING|SOAP_XML_IGNORENS);
	//SOAP_ENC_LATIN
	//soap_init1(&gs_stChatMonEnv.soap,SOAP_ENC_LATIN|SOAP_XML_IGNORENS);
	//gs_stChatMonEnv.soap.http_content = "text/xml; charset=gbk";
	//gs_stChatMonEnv.soap.prolog = "<?xml version=\"1.0\" encoding=\"GBK\"?>\n";
	
	if (chat_mon_db_init(pstEnv) < 0)
	{
		return -1;
	}

	if (chat_mon_init_tdrparser(pstEnv) < 0)
	{		
		return -1;
	}

	

	if (chat_mon_thread_init(pstEnv) < 0)
	{		
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR, "chat_mon_thread_init failed");
		return -1;
	}

	pstEnv->stOpLog.Ip = inet_addr(pstEnv->pstConf->SpawnIP);
	pstEnv->stOpLog.Port = pstEnv->pstConf->SpawnPort;

	{
		struct sigaction stAct;
		memset(&stAct, 0, sizeof(stAct));

		stAct.sa_handler	=	chat_mon_on_sigusr1;
		sigaction(SIGUSR1, &stAct, &pstEnv->stTappAct);
	}

	pstEnv->pstOpLogMonMeta = tdr_get_meta_by_name(pstEnv->pstSSHan->pstSSProtoLib,
												CHAT_MON_PKG_NAME);
	if (!pstEnv->pstOpLogMonMeta)
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR, "tdr_get_meta_by_name %s failed",CHAT_MON_PKG_NAME);
		return -1;
	}

	
	pstEnv->pstOpLogMeta = tdr_get_meta_by_name(pstEnv->pstSSHan->pstSSProtoLib,
												OP_LOG_PKG_NAME);
	if (!pstEnv->pstOpLogMeta)
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR, "tdr_get_meta_by_name %s failed",OP_LOG_PKG_NAME);
		return -1;
	}

	

	chat_mon_init_heart_beat(pstEnv);

	chat_mon_heart_beat_to_log_db(pstEnv);
	
	pstEnv->iOpLogMonMetaVersion = tdr_get_metalib_version(
									pstEnv->pstSSHan->pstSSProtoLib);

	InitDirtySystem2(pstEnv->pstConf->DirtyWordFile,pstEnv->pstConf->DirtyWordMd5File);
	
	printf("chat_mon_service start\n");
	ChatMonMutiLog(MUTI_LOGLEVEL_INFO, "chat_mon_service start");

	UNUSED(HEX_value);
		
	return 0;
}	

int chat_mon_oplog_limit_init(ACCDBFAILEDOPLOG* pstOpLog)
{
	pstOpLog->ClientIp = 0;
	pstOpLog->ClientTimeStamp = 0;
	pstOpLog->ErrCode = 0;
	pstOpLog->RegionID = 0;
	pstOpLog->OpSql[0] = 0;
	memset(&pstOpLog->MetaInfo,0,sizeof(pstOpLog->MetaInfo));

	return 0;
}

int chat_mon_db_op_in(CHAT_MON_SERVICEENV *pstEnv,TDRDBHANDLE dbHandle,int iRegionID,
	int iOpType,ChatMonMetaInfo* pstMetaInfo,TDRDATA *pstSql,TDRDBOBJECT* pstFetchDBObj,
	int* piFetchNum,int iFetchLimit)
{
	int iRet = 0;
	TDRDBRESULTHANDLE hDBResult = NULL;	
	int iNumRows = 0;

	pstEnv->stOpLog.RegionID = iRegionID;	
	
	if (!dbHandle)
	{
		pstEnv->iErrCode = CHAT_MON_ERR_DB_HANDLE;
		return -1;
	}
	
	iRet = tdr_obj2sql(dbHandle, pstSql, iOpType, &pstEnv->stDBObj,pstMetaInfo->szWhere);
	if (0 != iRet)
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,
			"生成查询资源记录的SQL语句失败: %s dbms error:%s region:%d meta:%s op:%d",
			tdr_error_string(iRet), tdr_dbms_error(dbHandle),
			iRegionID,pstMetaInfo->szMetaName,iOpType);

		pstEnv->iErrCode = CHAT_MON_ERR_SQL_CREATE;
		return iRet;
	}

	if (pstMetaInfo->iFailRecord)
	{
		STRNCPY(pstEnv->stOpLog.OpSql,pstSql->pszBuff, sizeof(pstEnv->stOpLog.OpSql));
	}

	ChatMonMutiLog(MUTI_LOGLEVEL_DEBUG,
			"region:%d meta:%s op:%d sql:%s",
			iRegionID,pstMetaInfo->szMetaName,iOpType,pstEnv->szSql);

	iRet = tdr_query(&hDBResult, dbHandle, pstSql);

	if(iRet != 0)
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"执行SQL语句失败:%s"
			" dbms error:%s region:%d meta:%s op:%d sql:%s",
			tdr_error_string(iRet), tdr_dbms_error(dbHandle),
			iRegionID,pstMetaInfo->szMetaName,iOpType,pstEnv->szSql);
		pstEnv->iErrCode = CHAT_MON_ERR_SQL_QUERY;
		return iRet;
	}

	pstEnv->iLastInsertID = tdr_dbms_insert_id(dbHandle);

	if (!hDBResult)
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,
			" null hDBResult region:%d meta:%s op:%d sql:%s",
			iRegionID,pstMetaInfo->szMetaName,iOpType,pstEnv->szSql);
		pstEnv->iErrCode = CHAT_MON_ERR_DB_RESULT;
		return -1;
	}

	
	if (TDR_DBOP_DELETE == iOpType ||
		TDR_DBOP_UPDATE == iOpType ||
		TDR_DBOP_INSERT == iOpType)
	{
		iNumRows = tdr_affected_rows(hDBResult);
		if(iNumRows < 1)
		{
			ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"effect rows:%d region:%d sql:%s",
						iNumRows,
						iRegionID,
						pstEnv->szSql);

			tdr_free_dbresult(&hDBResult);
			pstEnv->iErrCode = CHAT_MON_ERR_SQL_EFFECT_ROW;
			return -1;
		}	
	}

	if (TDR_DBOP_SELECT == iOpType &&
		pstFetchDBObj &&
		piFetchNum &&
		*piFetchNum < iFetchLimit)
	{
		TDRDBOBJECT pstRowObj = *pstFetchDBObj;

		iNumRows = tdr_num_rows(hDBResult);
		if(iNumRows < 1)
		{
			pstEnv->iErrCode = BI_ERR_NOT_FOUND;
			iRet = -1;
		}
		else
		{
			int i = 0;
			pstRowObj.pszObj += (*piFetchNum) * pstRowObj.iObjSize;

			if (1 == pstMetaInfo->iReverseOrder)			
			{
				int iNum = min(iFetchLimit - (*piFetchNum),iNumRows);
				iNum--;
				if (iNum < 0)
				{
					ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,
						"iNum error:%d",
						iNum
						);
					iRet = -1;
				}
				else
				{
					pstRowObj.pszObj += iNum * pstRowObj.iObjSize;
				}
			}
			
			if (0 == iRet)
			{
				for (i = 0; i < iNumRows; i++)
				{			
					iRet = tdr_fetch_row(hDBResult, &pstRowObj);
					if (iRet != 0)
					{
						ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,
							"从数据库表中读取数据记录失败:%s dbms error:%s",
							tdr_error_string(iRet), tdr_dbms_error(dbHandle)
							);
						iRet = -1;
					}
					if (0 == pstMetaInfo->iReverseOrder)
					{
						pstRowObj.pszObj += pstRowObj.iObjSize;
					}
					else
					{
						pstRowObj.pszObj -= pstRowObj.iObjSize;
					}
					
					(*piFetchNum)++;
				}
			}
		}
	}

	tdr_free_dbresult(&hDBResult);

	return iRet;
}

int chat_mon_db_op_log_in(CHAT_MON_SERVICEENV *pstEnv,
	int iOpType,TDRDBOBJECT *pstDBObj,TDRDATA *pstSql)
{
	int iRet = 0;
	TDRDBRESULTHANDLE hDBResult = NULL;	
	int iNumRows = 0;
	TDRDBHANDLE dbHandle = pstEnv->stLogDBInfo.dbHandle;
	
	if (!dbHandle)
	{
		return -1;
	}
	
	iRet = tdr_obj2sql(dbHandle, pstSql, iOpType, pstDBObj,NULL);
	if (0 != iRet)
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,
			"生成查询资源记录的SQL语句失败: %s dbms error:%s",
			tdr_error_string(iRet), tdr_dbms_error(dbHandle)
			);

		return iRet;
	}


	ChatMonMutiLog(MUTI_LOGLEVEL_DEBUG,
			"sql:%s",
			pstEnv->szOpLogSql);

	iRet = tdr_query(&hDBResult, dbHandle, pstSql);

	if(iRet != 0)
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"执行SQL语句失败:%s"
			" dbms error:%s sql:%s",
			tdr_error_string(iRet), tdr_dbms_error(dbHandle),
			pstEnv->szOpLogSql);

		return iRet;
	}

	if (!hDBResult)
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,
			" null hDBResult sql:%s",
			pstEnv->szOpLogSql);
		return -1;
	}

	
	if (TDR_DBOP_DELETE == iOpType ||
		TDR_DBOP_UPDATE == iOpType ||
		TDR_DBOP_INSERT == iOpType)
	{
		iNumRows = tdr_affected_rows(hDBResult);
		if(iNumRows < 1)
		{
			ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"effect rows:%d sql:%s",
						iNumRows,
						pstEnv->szOpLogSql);

			tdr_free_dbresult(&hDBResult);
			return -1;
		}	
	}

	

	tdr_free_dbresult(&hDBResult);

	return 0;
}


int chat_mon_db_op_log(CHAT_MON_SERVICEENV *pstEnv)
{
	TDRDBOBJECT stDBObj;
	TDRDATA stSql;	

	pstEnv->stOpLog.ErrCode = pstEnv->iErrCode;
	pstEnv->stOpLog.ClientTimeStamp = pstEnv->iClientTimeStamp;
	pstEnv->stOpLog.ClientIp = pstEnv->iClientIP;
	pstEnv->stOpLog.LogTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	
	stSql.iBuff = sizeof(pstEnv->szOpLogSql);
	stSql.pszBuff = pstEnv->szOpLogSql;
	
	stDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstRoleDBMetaLib, "ChatMonDBFailedOpLog");
	stDBObj.iVersion = tdr_get_meta_current_version(stDBObj.pstMeta);
	stDBObj.iObjSize = sizeof(pstEnv->stOpLog);
	stDBObj.pszObj = (char*) &pstEnv->stOpLog;

	chat_mon_db_op_log_in(pstEnv, TDR_DBOP_INSERT, &stDBObj, &stSql);
		


	return 0;
}

void* thread_proc_tcp(void* pvArg)
{	

	int iRet;
	ChatMonThreadMgr *pstThread;


	int iIdle = 0;

	CHAT_MON_SERVICEENV *pstEnv = NULL;

	

	pstThread = (ChatMonThreadMgr *)pvArg;
	pstEnv = &gs_stChatMonEnv;


	while(1)
	{
		if( pstEnv->iIsExit )
		{
			break;
		}
		
		iIdle++;
		if (iIdle > gs_stAppCtx.iIdleCount)
		{
			iIdle = 0;
			usleep(gs_stAppCtx.iIdleSleep*1000);
		}
		
		iRet = chat_mon_connd_proc_recv(pstThread);
		if (iRet < 0)
		{
			ChatMonMutiLog(MUTI_LOGLEVEL_ERROR, "chat_mon_connd_proc_recv error ");
		}
		
	}

	chat_mon_tconnd_scan_idle(pstThread,1);


	return (void*)0;

	UNUSED(pvArg);	//add by paraunused.pl
}


int chat_mon_db_op(CHAT_MON_SERVICEENV *pstEnv,int iRegionID,int iOpType,ChatMonMetaInfo* pstMetaInfo,
	TDRDBOBJECT* pstFetchDBObj,int* piFetchNum,int iFetchLimit,int* piFetchRegion)
{
	TDRDATA stSql;	
	int iRet = 0;
	//int i = 0;	//comment by paraunused.pl
	int iFind = 0;

	if (!pstMetaInfo ||
		!pstMetaInfo->pszObj ||
		0 == pstMetaInfo->szMetaName[0] ||
		0 >= pstMetaInfo->iObjSize)
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"pstMetaInfo err");
		pstEnv->iErrCode = CHAT_MON_ERR_ARGS;
		return -1;
	}

	if (piFetchNum)
	{
		*piFetchNum = 0;
	}

	pstEnv->stOpLog.OpType = iOpType;
	
	stSql.iBuff = sizeof(pstEnv->szSql);
	stSql.pszBuff = pstEnv->szSql;
	
	pstEnv->stDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstSSHan->pstSSProtoLib, pstMetaInfo->szMetaName);
	pstEnv->stDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDBObj.pstMeta);
	pstEnv->stDBObj.iObjSize = pstMetaInfo->iObjSize;
	pstEnv->stDBObj.pszObj = pstMetaInfo->pszObj;

	if (pstFetchDBObj)
	{
		pstFetchDBObj->pstMeta = pstEnv->stDBObj.pstMeta;
		pstFetchDBObj->iVersion = pstEnv->stDBObj.iVersion;		
	}

	pstEnv->stOpLog.MetaInfo.Version = pstEnv->stDBObj.iVersion;

	//for (i = 0; i < pstEnv->iDBConnNum;i++)
	{
		ChatMonDBInfo* pstInfo = &pstEnv->stLogDBInfo;
		int* piRegion = NULL;

		if (piFetchNum && piFetchRegion)
		{
			piRegion = piFetchRegion;
			piRegion += *piFetchNum;
		}
		/*
		if (iRegionID != 0 &&
			iRegionID != pstInfo->iRegionID)
		{
			continue;
		}
		*/

		iFind = 1;

		if (pstFetchDBObj)
		{
			iRet = chat_mon_db_op_in(pstEnv,pstInfo->dbHandle,pstInfo->iRegionID,iOpType,
					pstMetaInfo, &stSql,pstFetchDBObj,piFetchNum,iFetchLimit);
		}
		else
		{
			iRet = chat_mon_db_op_in(pstEnv,pstInfo->dbHandle,pstInfo->iRegionID,iOpType,
					pstMetaInfo, &stSql,NULL,piFetchNum,iFetchLimit);
		}

		if(iRet != 0)
		{
			ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"chat_mon_db_op_in err:%d",iRet);

			if (pstMetaInfo->iFailRecord)
			{
				chat_mon_db_op_log(pstEnv);
			}
		}	

		if(piRegion)
		{
			*piRegion = pstInfo->iRegionID;
		}

		if (iRegionID != 0 &&
			iRegionID == pstInfo->iRegionID)
		{
			//break;
		}
	}

	if (!iFind)
	{
		iRet = -1;
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"region not found:%d",iRegionID);
		pstEnv->iErrCode = CHAT_MON_ERR_REGION_NOT_FOUND;
		if (pstMetaInfo->iFailRecord)
		{
			chat_mon_db_op_log(pstEnv);
		}
	}	

	return iRet;
}

//--------------------------------//

int chat_mon_get_dirty_word_list(CHAT_MON_SERVICEENV* pstEnv,	DIRTYWORDLIST *pstRes)
{
	FILE *pDirtyFile = NULL;
	int iRet = 0;
	struct stat stStat;
	long pos = 0;

	pstRes->ModifyTime = 0;
	pstRes->Content[0] = 0;

	pDirtyFile = fopen(pstEnv->pstConf->DirtyWordMd5File,"r");
	if (!pDirtyFile)
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"can not read file:%s",
						pstEnv->pstConf->DirtyWordMd5File);
		iRet = -1;
		goto final;
	}

	iRet = fstat(fileno(pDirtyFile),&stStat);
	if (0 == iRet)
	{
		pstRes->ModifyTime = stStat.st_mtim.tv_sec;
	}
	else
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"fstat faile, file:%s err:%s",
						pstEnv->pstConf->DirtyWordMd5File,strerror(errno));
		iRet = -1;
		goto final;
	}

	fread(pstRes->Content,sizeof(pstRes->Content),1,pDirtyFile);
	pos = ftell(pDirtyFile);
	pos = min(pos,MAX_DIRTY_WORD_CONTENT_LEN -1 );
	
	pstRes->Content[pos] = 0;

	iRet = feof(pDirtyFile);
	if (!iRet)
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"can not read all file content:%s",
						pstEnv->pstConf->DirtyWordMd5File);
		iRet = -1;
		goto final;
	}
	
final:
	if (pDirtyFile)
	{
		fclose(pDirtyFile);
		pDirtyFile = NULL;
	}
	
	return iRet;
}

int chat_mon_set_dirty_word_list(CHAT_MON_SERVICEENV* pstEnv,int iModifyTime,char* pszContent)
{
	FILE *pDirtyFile = NULL;
	int iRet = 0;
	struct stat stStat;
	int iLen;


	iLen = strlen(pszContent);

	if (iLen <= 0)
	{
		ChatMonLastErrLog("content length too small:%d",iLen);
		iRet = -1;
		goto final;
	}

	if (iLen >= MAX_DIRTY_WORD_CONTENT_LEN)
	{
		ChatMonLastErrLog("content length too big:%d >= %d",iLen,MAX_DIRTY_WORD_CONTENT_LEN);
		iRet = -1;
		goto final;
	}

	pDirtyFile = fopen(pstEnv->pstConf->DirtyWordMd5File,"r");
	if (!pDirtyFile)
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"fopen fail, file:%s",
						pstEnv->pstConf->DirtyWordMd5File);
		goto final;
	}

	iRet = fstat(fileno(pDirtyFile),&stStat);
	if (0 == iRet)
	{
		if (stStat.st_mtim.tv_sec != iModifyTime)
		{
			ChatMonLastErrLog("invalid modify time:%d != %d",iModifyTime,(int)stStat.st_mtim.tv_sec);
			iRet = -1;
			goto final;
		}
	}
	else
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"fstat fail, file:%s err:%s",
						pstEnv->pstConf->DirtyWordMd5File,strerror(errno));
		iRet = -1;
		goto final;
	}

	if (pDirtyFile)
	{
		fclose(pDirtyFile);
	}
	
	pDirtyFile = NULL;

	pDirtyFile = fopen(pstEnv->pstConf->DirtyWordMd5File,"w+");
	if (!pDirtyFile)
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"fopen fail, file:%s",
						pstEnv->pstConf->DirtyWordMd5File);
		goto final;
	}

	iRet = fwrite(pszContent,iLen,1,pDirtyFile);

	if (iRet < 1)
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_ERROR,"fwirte fail:%s",
						pstEnv->pstConf->DirtyWordMd5File);
		iRet = -1;
		goto final;
	}

	if (pDirtyFile)
	{
		fclose(pDirtyFile);
	}
	
	pDirtyFile = NULL;
	
	tSetDirtywordVersion(pstEnv->pstConf->DirtyWordMd5File);

	pthread_mutex_lock(&pstEnv->stDirtyMutex);
	iRet = tDirtyUpdateCheck();
	pthread_mutex_unlock(&pstEnv->stDirtyMutex);
	if (iRet < 0)
	{
		ChatMonLastErrLog("dirty update fail:%d",iRet);
		iRet = -1;
		goto final;
	}
	else
	{
		ChatMonMutiLog(MUTI_LOGLEVEL_INFO,"reload dirty file:%s",
						pstEnv->pstConf->DirtyWordFile);
	}

final:
	
	if (pDirtyFile)
	{
		fclose(pDirtyFile);
	}
	
	pDirtyFile = NULL;
	
	return iRet;
}



SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetDirtyWordList(struct soap* soap, 
	struct ns1__GetDirtyWordListReq *InBody, 
	struct ns1__GetDirtyWordListRes *OutBody)
{	
	CHAT_MON_SERVICEENV* pstEnv = &gs_stChatMonEnv;
	int iRet = 0;	
	DIRTYWORDLIST stRes;

/*
	if (!InBody->Res)
	{
		OutBody->ErrorCode = CHAT_MON_ERR_SYS;
		CHAT_MON_ERR_TEXT(OutBody->StrError,"need InBody->Res");
		return 0;
	}
*/

	iRet = chat_mon_get_dirty_word_list(pstEnv,&stRes);

	if(iRet < 0)
	{
		if (pstEnv->iErrCode) {
			OutBody->ErrorCode = pstEnv->iErrCode;
		}
		else
		{
			OutBody->ErrorCode = iRet;
		}
		CHAT_MON_ERR_TEXT(OutBody->StrError,"GetDirtyWordList errno:%d errstr:%s",iRet,
						pstEnv->szLastErrLog);
		return 0;
	}
	
	iRet = chat_mon_sprint_xml(&OutBody->StrXML, "DirtyWordList", &stRes, sizeof(stRes));
	if(iRet < 0)
	{
		OutBody->ErrorCode = CHAT_MON_ERR_XML;
		CHAT_MON_ERR_TEXT(OutBody->StrError,"chat_mon_sprint_xml error:%d",iRet);
		return 0;
	}

	return 0;
	UNUSED(soap);
	UNUSED(InBody);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetDirtyWordList(struct soap* soap, 
	struct ns1__SetDirtyWordListReq *InBody, 
	struct ns1__SetDirtyWordListRes *OutBody)
{	
	CHAT_MON_SERVICEENV* pstEnv = &gs_stChatMonEnv;
	int iRet = 0;
	char* pszContentGbk = NULL;

	if (!InBody->ModifyTime)
	{
		OutBody->ErrorCode = CHAT_MON_ERR_SYS;
		CHAT_MON_ERR_TEXT(OutBody->StrError,"need InBody->ModifyTime");
		return 0;
	}

	if (!InBody->Content)
	{
		OutBody->ErrorCode = CHAT_MON_ERR_SYS;
		CHAT_MON_ERR_TEXT(OutBody->StrError,"need InBody->Content");
		return 0;
	}
	pszContentGbk = chat_mon_get_gbk_str(InBody->Content);

	if (!pszContentGbk)
	{
		OutBody->ErrorCode = CHAT_MON_ERR_SYS;
		CHAT_MON_ERR_TEXT(OutBody->StrError,"get_gbk_str InBody->Content err");
		return 0;
	}

	tDirtySetAutoUpdate( 1 );	
	iRet = chat_mon_set_dirty_word_list(pstEnv,
										*InBody->ModifyTime,
										pszContentGbk
										);
	tDirtySetAutoUpdate( 0 );	
	
	if(iRet < 0)
	{
		if (pstEnv->iErrCode) {
			OutBody->ErrorCode = pstEnv->iErrCode;
		}
		else
		{
			OutBody->ErrorCode = iRet;
		}		
		CHAT_MON_ERR_TEXT(OutBody->StrError,"SetDirtyWordList errno:%d errstr:%s",iRet,pstEnv->szLastErrLog);
		return 0;
	}

	return 0;
	UNUSED(soap);
}

int chat_mon_get_chat_mon_log(CHAT_MON_SERVICEENV* pstEnv,int iSeq,int iType,LOGROLECHATINFO *pstRes)
{
	int iRet = 0;
	TDRDBOBJECT stFetchObj;
	ChatMonMetaInfo stMetaInfo;
	LOGROLECHATMON stLogRoleChatMon;
	int iBeginSeq = 0;
	int iLimit = MAX_CHAT_LOG_BUFF_NUM;
	LogBuffInfo * pstInfo = &pstEnv->stLogBuffInfo;

	
	pstRes->Num = 0;

	if (0 == iSeq && pstInfo->iNum > 0)
	{
		
		int iEndNum = pstInfo->iNum - pstInfo->iBegin;

		if (iEndNum < 1)
		{
			pstEnv->iErrCode = CHAT_MON_ERR_NOT_FOUND;
			ChatMonLastErrLog("chat mon log not found");
			return -1;
		}

		if (iEndNum > MAX_CHAT_LOG_BUFF_NUM)
		{
			ChatMonMutiLog(MUTI_LOGLEVEL_INFO,"iEndNum err %d > %d",
						iEndNum,MAX_CHAT_LOG_BUFF_NUM);
			return -1;
		}

		pstRes->Num = pstInfo->iNum;
		
		memcpy(pstRes->LogRoleChatMon,&pstInfo->astLog[pstInfo->iBegin],
				sizeof(pstInfo->astLog[0])*iEndNum);

		if (pstInfo->iBegin > 0)
		{
			if (iEndNum >= MAX_CHAT_LOG_BUFF_NUM  || 
				(iEndNum + pstInfo->iBegin) > MAX_CHAT_LOG_BUFF_NUM)
			{
				ChatMonMutiLog(MUTI_LOGLEVEL_INFO,"iEndNum err %d >= %d",
							iEndNum,MAX_CHAT_LOG_BUFF_NUM);
				return -1;
			}
			
			memcpy(&pstRes->LogRoleChatMon[iEndNum],&pstInfo->astLog[0],
				sizeof(pstInfo->astLog[0])*pstInfo->iBegin);
		}

		return 0;
	}

	iBeginSeq = iSeq;

	if (0 == iType && iBeginSeq >= pstInfo->astLog[pstInfo->iBegin].Seq &&
		pstInfo->iNum > 0)
	{
		int iIdx = 0;
		int i = 0;
		int iOffset = 0;
		int iFound = 0;
		for (i = 0; i < pstInfo->iNum; i++)
		{
			iIdx = i + pstInfo->iBegin;
			if (iIdx >= MAX_CHAT_LOG_BUFF_NUM)
			{
				iIdx -= MAX_CHAT_LOG_BUFF_NUM;
			}
			if (pstInfo->astLog[iIdx].Seq == iBeginSeq)
			{
				iIdx++;
				if (iIdx >= MAX_CHAT_LOG_BUFF_NUM)
				{
					iIdx -= MAX_CHAT_LOG_BUFF_NUM;
				}
				
				iOffset = iIdx - pstInfo->iBegin;
				iFound = 1;
				break;
			}
		}

		if (iFound)
		{
			int iEndNum = 0;
			int iPreNum = 0;

			pstRes->Num = pstInfo->iNum;
			if (iOffset >= 0)
			{
				pstRes->Num -= iOffset;		
				iEndNum = pstInfo->iNum - iIdx;
			}
			else
			{
				pstRes->Num = -iOffset;
				iEndNum = pstRes->Num;
			}

			iPreNum = pstRes->Num - iEndNum;

			if (iEndNum < 1)
			{
				pstEnv->iErrCode = CHAT_MON_ERR_NOT_FOUND;
				ChatMonLastErrLog("chat mon log not found");
				return -1;
			}
			
			if (iEndNum > MAX_CHAT_LOG_BUFF_NUM)
			{
				ChatMonMutiLog(MUTI_LOGLEVEL_INFO,"iEndNum err %d > %d",
							iEndNum,MAX_CHAT_LOG_BUFF_NUM);
				return -1;
			}
			
			memcpy(pstRes->LogRoleChatMon,&pstInfo->astLog[iIdx],
					sizeof(pstInfo->astLog[0])*iEndNum);

			if (iPreNum > 0)
			{
				if (iEndNum >= MAX_CHAT_LOG_BUFF_NUM || 
					(iEndNum + iPreNum) > MAX_CHAT_LOG_BUFF_NUM)
				{
					ChatMonMutiLog(MUTI_LOGLEVEL_INFO,"iEndNum err %d >= %d",
								iEndNum,MAX_CHAT_LOG_BUFF_NUM);
					return -1;
				}
				
				memcpy(&pstRes->LogRoleChatMon[iEndNum],&pstInfo->astLog[0],
					sizeof(pstInfo->astLog[0])*iPreNum);
			}
		}

		return 0;
	}

	if( 1 == iType)
	{
		iBeginSeq -= iLimit;
	}

	memset(&stLogRoleChatMon,0,sizeof(stLogRoleChatMon));
	
	memset(&stMetaInfo,0,sizeof(stMetaInfo));
	
	STRNCPY(stMetaInfo.szMetaName,"LogRoleChatMon",sizeof(stMetaInfo.szMetaName));
	stMetaInfo.iObjSize = sizeof(stLogRoleChatMon);
	stMetaInfo.pszObj = (char*) &stLogRoleChatMon;
	if (pstInfo->iNum >0)
	{
		snprintf(stMetaInfo.szWhere,sizeof(stMetaInfo.szWhere),"where Seq > %d and Seq < %d order by Seq limit %d",
				iBeginSeq,iSeq,iLimit);
	}
	else
	{
		stMetaInfo.iReverseOrder = 1;
		snprintf(stMetaInfo.szWhere,sizeof(stMetaInfo.szWhere),"order by Seq desc limit %d",
				iLimit);
	}

	memset(&stFetchObj,0,sizeof(stFetchObj));
	stFetchObj.pszObj = (char*) pstRes->LogRoleChatMon;
	stFetchObj.iObjSize = sizeof(pstRes->LogRoleChatMon[0]);
	
	iRet = chat_mon_db_op(pstEnv,
						0,
						TDR_DBOP_SELECT,
						&stMetaInfo,
						&stFetchObj,
						&pstRes->Num,
						iLimit,
						NULL
						);

	if (0 == iRet && pstInfo->iNum < 1 && pstRes->Num > 0)
	{
		pstInfo->iBegin = 0;
		pstInfo->iNum = min(pstRes->Num,MAX_CHAT_LOG_BUFF_NUM);
		
		memcpy(pstInfo->astLog,pstRes->LogRoleChatMon,
				sizeof(pstInfo->astLog[0])*pstInfo->iNum);
	}

	return iRet;
}


SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetChatMonLog(struct soap* soap, 
	struct ns1__GetChatMonLogReq *InBody, 
	struct ns1__GetChatMonLogRes *OutBody)
{	
	CHAT_MON_SERVICEENV* pstEnv = &gs_stChatMonEnv;
	int iRet = 0;
	LOGROLECHATINFO stRes;

	if (!InBody->Seq)
	{
		OutBody->ErrorCode = CHAT_MON_ERR_SYS;
		CHAT_MON_ERR_TEXT(OutBody->StrError,"need InBody->Seq");
		return 0;
	}

	if ((!InBody->Type) ||
		(*InBody->Type < 0 || *InBody->Type > 1) ||
		(*InBody->Seq == 0 && *InBody->Type != 0) )
	{
		OutBody->ErrorCode = CHAT_MON_ERR_SYS;
		CHAT_MON_ERR_TEXT(OutBody->StrError,"need valid InBody->Type");
		return 0;
	}

	iRet = chat_mon_get_chat_mon_log(pstEnv,
		*InBody->Seq,
		*InBody->Type,
		&stRes
		);

	if(iRet < 0)
	{
		if (pstEnv->iErrCode) 
		{
			OutBody->ErrorCode = pstEnv->iErrCode;
		}
		else
		{
			OutBody->ErrorCode = iRet;
		}		
		CHAT_MON_ERR_TEXT(OutBody->StrError,"GetChatMonLog errno:%d errstr:%s",iRet,pstEnv->szLastErrLog);
		return 0;
	}
	
	iRet = chat_mon_sprint_xml(&OutBody->StrXML, "LogRoleChatInfo", &stRes, sizeof(stRes));
	if(iRet < 0)
	{
		OutBody->ErrorCode = CHAT_MON_ERR_XML;
		CHAT_MON_ERR_TEXT(OutBody->StrError,"chat_mon_sprint_xml errno:%d errstr:%s",iRet,pstEnv->szLastErrLog);
		return 0;
	}

	return 0;
	UNUSED(soap);
}


