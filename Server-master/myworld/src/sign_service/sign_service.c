
#include <stdio.h>

#include "tapp/tapp.h"
#include "pal/pal.h" 

#include "tdr/tdr.h"
#include "tlog/tlog.h"
#include "sign_service_conf_desc.h"
#include "version.h"
#include "tbus/tbus.h"
#include "comm_misc.h"

#include "soapH.h"
#include "SignServiceSoap.nsmap"
#include <iconv.h>
#include "fcgi_config.h"

#include <stdlib.h>
#include "comm/tconvert.h"
#include "fcgios.h"
#include "cs_net.h"



//#include "sign_service.h"

extern char **environ;
extern RSA_PUBLIC_KEY * IDRSAPublicKey;

char * rand_str(char* pszBuff, int iLen);


#include "fcgi_stdio.h"
//#include "cs_cgi.h"
//#include "../cs_service/soapCsServiceStub.h"
//#include "../cs_service/soapCsService.nsmap"


#ifndef WIN32
#define max(a, b)	((a) > (b) ? (a) : (b))
#define min(a, b)	((a) < (b) ? (a) : (b))
#endif


#define UNUSED( param ) ((void)(param))



#define MAX_SIGN_ERROR_LEN 2048 //错误信息长度 
//#define DEFAULT_TIME_OUT 2000 //默认超时时间
#define MAX_STR_XML_LEN 2048000 //最大返回xml字符串长度
#define SIGN_LOCAL_BUS_ADDR_START 10
#define SIGN_HEART_BEAT_INTERVAL 30
#define MAX_GBK_LENG 1024 //最大GBK字符串长度
#define MAX_GBK_TEXT_NUM 5 //最大GBK字符串数量
#define MAX_LOG_PATH_LENG 128 //日志目录长度
//#define MAX_SIGN_SQL_LEN 20480 //SQL长度 
#define MAX_SIGN_META_NAME_LEN 64 //元数据名 
#define MAX_SIGN_WHERE_LEN 1024 //元数据名 



typedef struct 
{
	char szTextGBK[MAX_GBK_LENG];
}GbkText;

struct tagSignDBInfo
{
	int iRegionID;
	int iKeepAlive;
	TDRDBHANDLE dbHandle;
};

typedef struct tagSignDBInfo SignDBInfo;

struct tagSignMetaInfo
{
	char szMetaName[MAX_SIGN_META_NAME_LEN];
	int iObjSize;
	char* pszObj;
	char szWhere[MAX_SIGN_WHERE_LEN];
	int iFailRecord;	//是否失败时记录
	int iUseSql;//使用sql语句
	int iEffectRowIgnore;//忽略影响行小于1
};

typedef struct tagSignMetaInfo SignMetaInfo;

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
	
}SignThreadMgr;

enum enumSignThread
{
	SIGN_THREAD_HEART_BEAT = 0,
	SIGN_THREAD_NUM,
};

typedef struct tagRegionAuth
{
	int iRegionID;                       
    char szAuthIP[24];                      	/*   auth_svr的IP地址 */
    int iAuthPort;                        	/*   auth_svr绑定的端口 */
	int iSocket;
	NETCLTAUTHHANDLE *pstNetCltAuthHandle;
	int iHeartBeatTime;
	pthread_mutex_t stDirtyMutex;
}RegionAuth;


struct tagsign_serviceEnv
{
	SIGN_SERVICECONF *pstConf;
	SIGN_SERVICERUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	TAPPCTX *pstAppCtx;
	pthread_mutex_t stLogMutex;
	//pthread_mutex_t stDirtyMutex;
	LPSSHANDLE pstSSHan;
	NETHANDLE *pstCSHan; 
	int iCSMetaVersion;
	//NETCLTAUTHHANDLE *pNetCltHan;
	struct soap soap;
	int *pCgiHandle;
	struct ns1__ErrorResponse stErrorResponse;
	char szErrorText[MAX_SIGN_ERROR_LEN];
	char szStrXML[MAX_STR_XML_LEN];
	char szStrXMLUtf8[MAX_STR_XML_LEN];
	int iGetFindHandleErr;
	int iTmp1;
	int iGbkTextUsedNum;
	GbkText aGbkText[MAX_GBK_TEXT_NUM];
	char szLogPath[MAX_LOG_PATH_LENG];
	int iDBConnNum;
	SignDBInfo astDBInfo[SIGN_MAX_REGION_NUM];
	char szSql[MAX_DB_SQL_LEN];
	char szOpLogSql[MAX_DB_SQL_LEN];
	TDRDBOBJECT stDBObj;
	int iThread;
	SignThreadMgr astThread[SIGN_THREAD_NUM];
	ACCDBFAILEDOPLOG stOpLog;
	SignDBInfo stLogDBInfo;
	int iErrCode;
	int iClientIP;
	int iClientTimeStamp;
	LPTDRMETALIB pstRoleDBMetaLib;
	struct sigaction stTappAct;
	char szLastErrLog[MAX_SIGN_ERROR_LEN];
	//NETCLTAUTHHANDLE *pstNetCltAuthHandle;
	//int iSocket;
	//int iHeartBeatTime;
	RSA *pstRsa;
	CSPKG stCSPkg;
	int iIsExit;
	int iRegionAuthNum;
	RegionAuth stRegionAuth[SIGN_MAX_REGION_NUM];
};

typedef struct tagsign_serviceEnv		SIGN_SERVICEENV;
typedef struct tagsign_serviceEnv		*LSIGN_SERVICEENV;

static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_sign_service[];
extern unsigned char g_szMetalib_proto_ss[];
extern unsigned char g_szMetalib_proto_cs[];


SIGN_SERVICEENV gs_stSignEnv;
static SIGN_SERVICERUN_STATUS gs_stStat;

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

#define SignLastErrLog(szFormat, args...) \
		do{\
			snprintf(gs_stSignEnv.szLastErrLog,sizeof(gs_stSignEnv.szLastErrLog),szFormat, ##args);\
		  }while(0)

#define SignMutiLog(iLevel, szFormat, args...) \
		do{\
			pthread_mutex_lock(&gs_stSignEnv.stLogMutex); \
			snprintf(gs_stSignEnv.szLastErrLog,sizeof(gs_stSignEnv.szLastErrLog),szFormat, ##args);\
			switch(iLevel)\
			{\
			case MUTI_LOGLEVEL_TRACE:\
				tlog_trace(gs_stSignEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_DEBUG:\
				tlog_debug(gs_stSignEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_INFO:\
				tlog_info(gs_stSignEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_WARN:\
				tlog_warn(gs_stSignEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_ERROR:\
				tlog_error(gs_stSignEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_SEVERE:\
				tlog_fatal(gs_stSignEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			default:\
				break;\
			}\
			pthread_mutex_unlock(&gs_stSignEnv.stLogMutex); \
		  }while(0)

#define SIGN_ERR_TEXT(pstErrRes, szFormat, args...) \
		do{\
			if (!pstErrRes)\
			{\
				pstErrRes = &gs_stSignEnv.stErrorResponse;\
			}\
			if (!pstErrRes->ErrorText)\
			{\
				pstErrRes->ErrorText = gs_stSignEnv.szErrorText;\
			}\
			if (pstErrRes->ErrorText)\
			{\
				snprintf(pstErrRes->ErrorText,MAX_SIGN_ERROR_LEN,szFormat,##args);\
			}\
		  }while(0);	

int sign_db_done(SIGN_SERVICEENV *pstEnv);
int sign_service_init(TAPPCTX *pstAppCtx, SIGN_SERVICEENV *pstEnv);
int sign_service_fini(TAPPCTX *pstAppCtx, SIGN_SERVICEENV *pstEnv);
//int sign_oplog_limit_init(ACCDBFAILEDOPLOG* pstOpLog);


int sign_get_log_path(SIGN_SERVICEENV *pstEnv)
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

int sign_parse_args(int argc, char **argv)
{
   int c;
   SIGN_SERVICEENV *pstEnv = &gs_stSignEnv;
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
						

					pstMeta = tdr_get_meta_by_name((LPTDRMETALIB)g_szMetalib_sign_service, 
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

int sign_soap_done(SIGN_SERVICEENV *pstEnv)
{
	int i = 0;
	
	pstEnv->iGbkTextUsedNum = 0;
	for (i = 0;i < pstEnv->iDBConnNum; i++)
	{
		pstEnv->astDBInfo[i].iKeepAlive = 0;		
	}
	pstEnv->iErrCode = 0;
	return 0;
}

int sign_service_proc(TAPPCTX *pstAppCtx, SIGN_SERVICEENV *pstEnv)
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
	sign_soap_done(pstEnv);

	if (iBusy)
	{
		return 0;
	}
	else
	{
		return -1;
	}
	
err:
	SignMutiLog(MUTI_LOGLEVEL_ERROR, "soap err:%d", soap->error);
	goto final;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int sign_service_reload(TAPPCTX *pstAppCtx, SIGN_SERVICEENV *pstEnv)
{
	SIGN_SERVICECONF   *pstPreConf;
	
	pstPreConf = (SIGN_SERVICECONF   *)pstAppCtx->stConfPrepareData.pszBuff;
	
	printf("sign_service reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "sign_service reload");

	return 0;
}

int sign_service_tick(TAPPCTX *pstAppCtx, SIGN_SERVICEENV *pstEnv)
{
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}

int sign_service_stop(TAPPCTX *pstAppCtx, SIGN_SERVICEENV *pstEnv)
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



int main(int argc, char **argv)
{ 
	//SOAP_SOCKET m, s; /* master and slave sockets */	//comment by paraunused.pl
	int iRet;
	void* pvArg	=	&gs_stSignEnv;

	memset(&gs_stAppCtx, 0, sizeof(gs_stAppCtx));
	memset(&gs_stSignEnv, 0, sizeof(gs_stSignEnv));

	gs_stAppCtx.argc	=	argc;
	gs_stAppCtx.argv	=	argv;

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)sign_service_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_sign_service;
	gs_stAppCtx.stConfData.pszMetaName = "sign_serviceconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);
		
	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)sign_service_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)sign_service_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)sign_service_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)sign_service_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)sign_service_stop;
	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	tapp_def_fini(&gs_stAppCtx, pvArg);
	

	
	return 0;
} 

int sign_get_gbk_str_in(char ** ppIn,size_t * ppInLen,char** ppOut,size_t * ppOutLen)
{
	int iConvRet = 0;
	
	iconv_t cd = iconv_open("gbk","utf-8");

	iConvRet = iconv(cd, ppIn, ppInLen,ppOut, ppOutLen);
	iconv_close(cd);

	return iConvRet;
}


char* sign_get_gbk_str(char * pszUTF8)
{
	size_t sInLen = strlen(pszUTF8);
	size_t sOutLen = sizeof(GbkText);
	char *pIn = pszUTF8;
	char *pOut = NULL;
	int iConvRet = 0;
	GbkText *pstText = NULL;

	if (gs_stSignEnv.iGbkTextUsedNum < 0 ||
		gs_stSignEnv.iGbkTextUsedNum >= MAX_GBK_TEXT_NUM)
	{
		return NULL;
	}
	
	pstText = &gs_stSignEnv.aGbkText[gs_stSignEnv.iGbkTextUsedNum];
	gs_stSignEnv.iGbkTextUsedNum++;
	
	pOut = pstText->szTextGBK;	

	memset(pstText,0,sizeof(*pstText));
	
	iConvRet =sign_get_gbk_str_in(&pIn, &sInLen,&pOut, &sOutLen);
	
	if (iConvRet < 0)
	{			
		return NULL;
	}

	return pstText->szTextGBK;
}

int sign_sprint_tdr(
	char **ppStrXml,
	const char *meta_name,
	const void *data,
	int data_size)
{
	int iRet = 0;
	int iLen = 0;

	TDRDATA host_data;
	TDRDATA net_data;

	SIGN_SERVICEENV* pstEnv = &gs_stSignEnv;

	host_data.pszBuff = (char *)data;
	host_data.iBuff = data_size;

	net_data.pszBuff = pstEnv->szStrXML;
	net_data.iBuff = sizeof(pstEnv->szStrXML);

	//memset(pstEnv->szStrXML,0,sizeof(pstEnv->szStrXML));
	pstEnv->szStrXML[0] = 0;
	
	*ppStrXml = pstEnv->szStrXML;

	//iRet = cs_sprint_xml(pstEnv->pCgiHandle,meta_name,data,data_size,
		//pstEnv->szStrXML,sizeof(pstEnv->szStrXML));
	iRet = tdr_output(tdr_get_meta_by_name(pstEnv->pstSSHan->pstSSProtoLib,meta_name),
	                   &net_data, &host_data,0, TDR_IO_NEW_XML_VERSION);

	if (0 == iRet)
	{
		iLen = net_data.iBuff;
	
		iLen = min(iLen,MAX_STR_XML_LEN -1);
		if (iLen >= 0 && iLen <MAX_STR_XML_LEN)
		{
			pstEnv->szStrXML[iLen] = 0;
		}
		else
		{
			pstEnv->szStrXML[MAX_STR_XML_LEN -1] = 0;
		}
	}
	else
	{
		SignMutiLog(MUTI_LOGLEVEL_ERROR,tdr_error_string(iRet));
		return iRet ;
	}	

	return iRet;
}


int sign_sprint_xml(
	char **ppStrXml,
	const char *meta_name,
	const void *data,
	int data_size)
{
	int iRet = 0;
	int iLen = 0;

	SIGN_SERVICEENV* pstEnv = &gs_stSignEnv;

	
	iRet = sign_sprint_tdr(ppStrXml,meta_name,data,data_size);

	if (0 == iRet && pstEnv->pstConf->SoapEncoding == SOAP_ENCODING_TYPE_UTF8)
	{
		size_t sInLen = strlen(pstEnv->szStrXML);
		size_t sOutLen = sizeof(pstEnv->szStrXMLUtf8);
		char *pIn = pstEnv->szStrXML;
		char *pOut = pstEnv->szStrXMLUtf8;
		int iConvRet = 0;
		iconv_t cd = 0;

		//memset(pstEnv->szStrXMLUtf8,0,sizeof(pstEnv->szStrXMLUtf8));
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
		
		iconv_close(cd);
	}

	return iRet;
}



int sign_service_fini(TAPPCTX *pstAppCtx, SIGN_SERVICEENV *pstEnv)
{
	int i = 0;
	void *pvRet;
	SignThreadMgr* pstThread = NULL;
	
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
	
	sign_db_done(pstEnv);

	for (i = 0 ; i < pstEnv->iRegionAuthNum; i++)
	{
		RegionAuth * pstRegionAuth = NULL;
		if (i >= SIGN_MAX_REGION_NUM)
		{
			break;
		}

		pstRegionAuth = &pstEnv->stRegionAuth[i];
		
		if (pstRegionAuth->pstNetCltAuthHandle)
		{
			net_clt_han_auth_destroy(&pstRegionAuth->pstNetCltAuthHandle);
			pstRegionAuth->pstNetCltAuthHandle = NULL;
		}

		if (pstRegionAuth->iSocket)
		{
			tnet_close(pstRegionAuth->iSocket);
			pstRegionAuth->iSocket = -1;
		}
	}


	if (pstEnv->pstRsa)
	{
		rsa_free(pstEnv->pstRsa);
	}

	

	soap_end(&gs_stSignEnv.soap);
	
	printf("service_svr stop\n");
	SignMutiLog(MUTI_LOGLEVEL_INFO, "service_svr stop");
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}

int sign_db_init(SIGN_SERVICEENV *pstEnv)
{
	//int i = 0; 
	SignDBInfo* pstInfo = NULL;
	//REGIONDBINFO* pstDBConf = NULL;
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
			SignMutiLog(MUTI_LOGLEVEL_ERROR, "tdr_open_dbhanlde regionid:%d err:%s",
						pstInfo->iRegionID ,pstEnv->szErrorText);
			return -1;
		}
		pstInfo->iKeepAlive = 1;
	}
	/*
	for (i =0; i < pstEnv->pstConf->RegionDBNum;i++)
	{
		pstDBConf = &pstEnv->pstConf->RegionDBInfo[i];

		pstInfo = &pstEnv->astDBInfo[pstEnv->iDBConnNum];
		pstEnv->iDBConnNum++;
		pstInfo->iRegionID = pstDBConf->RegionID;

		memset(&stDBMS, 0, sizeof(stDBMS));
		STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
		STRNCPY(stDBMS.szDBMSCurDatabaseName,pstDBConf->DBConnectInfo.DBMSCurDatabaseName, 
				sizeof(stDBMS.szDBMSCurDatabaseName));
		STRNCPY(stDBMS.szDBMSConnectionInfo, pstDBConf->DBConnectInfo.DBMSConnectionInfo, 
				sizeof(stDBMS.szDBMSConnectionInfo));	
		STRNCPY(stDBMS.szDBMSUser, pstDBConf->DBConnectInfo.DBMSUser, 
				sizeof(stDBMS.szDBMSUser));
		STRNCPY(stDBMS.szDBMSPassword, pstDBConf->DBConnectInfo.DBMSPassword,
				sizeof(stDBMS.szDBMSPassword));
		
		stDBMS.iReconnectOpt = 1;	
	
		iRet = tdr_open_dbhanlde(&pstInfo->dbHandle,&stDBMS,
					pstEnv->szErrorText);
		
		if (iRet < 0 || pstInfo->dbHandle == NULL)
		{
			SignMutiLog(MUTI_LOGLEVEL_ERROR, "tdr_open_dbhanlde regionid:%d err:%s",
						pstInfo->iRegionID ,pstEnv->szErrorText);
			return -1;
		}
		pstInfo->iKeepAlive = 1;
	}*/

	return 0;
}

int sign_db_done(SIGN_SERVICEENV *pstEnv)
{
	int i = 0;
	
	for (i = 0; i < pstEnv->iDBConnNum;i++)
	{
		tdr_close_dbhanlde(&pstEnv->astDBInfo[i].dbHandle);
	
		memset(&pstEnv->astDBInfo[i],0,sizeof(pstEnv->astDBInfo[i]));
	}

	tdr_close_dbhanlde(&pstEnv->stLogDBInfo.dbHandle);

	memset(&pstEnv->stLogDBInfo,0,sizeof(pstEnv->stLogDBInfo));
	
	return 0;
	
}

int sign_soap_prepare_init_recv(struct soap* soap)
{
	//int i = 0;
	//int iRet = 0;
	SIGN_SERVICEENV *pstEnv = &gs_stSignEnv;
	
	/*
	for (i = 0; i < pstEnv->iDBConnNum;i++)
	{
		iRet = tdr_keep_dbmsconnection(pstEnv->astDBInfo[i].dbHandle);
		if (iRet < 0)
		{
			SignMutiLog(MUTI_LOGLEVEL_ERROR,"tdr_keep_dbmsconnection err:%d",
				iRet);
		}
		else
		{
			pstEnv->astDBInfo[i].iKeepAlive = 1;
		}
	}

	iRet = tdr_keep_dbmsconnection(pstEnv->stLogDBInfo.dbHandle);
	if (iRet < 0)
	{
		SignMutiLog(MUTI_LOGLEVEL_ERROR,"tdr_keep_dbmsconnection err:%d",
			iRet);
	}
	else
	{
		pstEnv->stLogDBInfo.iKeepAlive = 1;
	}*/

	//sign_oplog_limit_init(&pstEnv->stOpLog);

	pstEnv->iGbkTextUsedNum = 0;

	pstEnv->szLastErrLog[0] = 0;
	
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

void sign_on_sigusr1(int iSig)
{
	SIGN_SERVICEENV *pstEnv = &gs_stSignEnv;
	int i = 0;

	pstEnv->iIsExit = 1;

	for(i=0; i<pstEnv->iThread; i++)
	{
		SignThreadMgr* pstThread	=	&pstEnv->astThread[i];

		if( pstThread->cIsValid )
		{
			pthread_kill(pstThread->tid, iSig);
		}
	}
		
	OS_ShutdownPending();
	sigaction(SIGUSR1, &pstEnv->stTappAct,NULL);
	kill(getpid(),iSig);
}

void* thread_proc_heart_beat(void* pvArg)
{	

	int iRet = 0;
	SignThreadMgr *pstThread;
	int iIdle = 0;
	SIGN_SERVICEENV *pstEnv = NULL;
	int iReconn =0;
	int iNow = 0;
	static int siAuthIdx = 0;
	RegionAuth* pstRegionAuth = NULL;

	

	pstThread = (SignThreadMgr *)pvArg;
	pstEnv = &gs_stSignEnv;


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

		if (siAuthIdx >= pstEnv->iRegionAuthNum || siAuthIdx >= SIGN_MAX_REGION_NUM)
		{
			siAuthIdx = 0;
		}

		pstRegionAuth =  &pstEnv->stRegionAuth[siAuthIdx];
		siAuthIdx++;

		if (-1 == pstRegionAuth->iSocket)
		{
			continue;
		}
		
		iNow = time(NULL);
		
		if (pstRegionAuth->iHeartBeatTime + SIGN_HEART_BEAT_INTERVAL >  iNow)
		{
			continue;
		}
		
		iReconn  = 0;

		if (0 == pthread_mutex_trylock(&pstRegionAuth->stDirtyMutex))
		{		
			iRet = net_clt_auth_heart_beat_web_sign(pstRegionAuth->pstNetCltAuthHandle,
												&pstEnv->stCSPkg,pstRegionAuth->iSocket);

			if (iRet < 0)
			{
				tnet_close(pstRegionAuth->iSocket);
				
				iRet = net_clt_auth_conn(pstRegionAuth->szAuthIP,
								pstRegionAuth->iAuthPort,&pstRegionAuth->iSocket);
				if (iRet < 0)
				{
					pstRegionAuth->iSocket = -1;
				}
				else
				{
					iRet = net_clt_auth_heart_beat_web_sign(pstRegionAuth->pstNetCltAuthHandle,
													&pstEnv->stCSPkg,pstRegionAuth->iSocket);
				}

				iReconn = 1;
				
			}
			
			pthread_mutex_unlock(&pstRegionAuth->stDirtyMutex);
		}	

		if (iRet < 0)
		{
			if (!iReconn)
			{
				SignMutiLog(MUTI_LOGLEVEL_ERROR, "net_clt_auth_heart_beat fail:%d region:%d",iRet,pstRegionAuth->iRegionID);
			}
			else
			{
				SignMutiLog(MUTI_LOGLEVEL_ERROR, "net_clt_auth_conn fail:%d region:%d",iRet,pstRegionAuth->iRegionID);
			}
		}
		else
		{
			pstRegionAuth->iHeartBeatTime = iNow;
			if (iReconn)
			{
				SignMutiLog(MUTI_LOGLEVEL_INFO, "net_clt_auth_conn ");
			}
		}
		
	}

	return (void*)0;

}

int sign_thread_init_one(SIGN_SERVICEENV *pstEnv, SignThreadMgr *pstThread, 
							int iIdx)
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
	/*
	if (0 > tbus_open_channel(&pstThread->pstChannel0,  pstEnv->pstAppCtx->iId, SIGN_LOCAL_BUS_ADDR_START+iIdx,  iKey, CHAT_MON_CHANNEL_SIZE))
	{
		return -1;
	}

	if (0 > tbus_open_channel(&pstThread->pstChannel1, SIGN_LOCAL_BUS_ADDR_START+iIdx, pstEnv->pstAppCtx->iId,   iKey, CHAT_MON_CHANNEL_SIZE))
	{
		return -1;
	}*/

	pstThread->cIsValid = 1;
	if (iIdx == SIGN_THREAD_HEART_BEAT)
	{		
		iRet = pthread_create(&pstThread->tid, NULL, thread_proc_heart_beat, pstThread);
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

int sign_thread_init(SIGN_SERVICEENV *pstEnv)
{
	int i;

	pstEnv->iThread = SIGN_THREAD_NUM;

	for (i=0; i<pstEnv->iThread; i++)
	{
		if (0 > sign_thread_init_one(pstEnv, &pstEnv->astThread[i], i))
		{
			return -1;
		}
	}
	
	return 0;
}

int sign_service_init(TAPPCTX *pstAppCtx, SIGN_SERVICEENV *pstEnv)
{
	char szLogFile[MAX_LOG_PATH_LENG];
	char szBid[64];
	int i = 0;
		

	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		return -1;
	}
	pstEnv->pstConf = (SIGN_SERVICECONF *)pstAppCtx->stConfData.pszBuff;

	pstEnv->pstRunStat = (SIGN_SERVICERUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
	if (NULL == pstEnv->pstRunStat)
	{
		pstEnv->pstRunStat = &gs_stStat;
	}

	pstEnv->pstAppCtx = pstAppCtx; 

	if(ss_han_create(g_szMetalib_proto_ss, &pstEnv->pstSSHan) < 0)
	{
		return -1;
	}

	if (0 > net_han_create_c(g_szMetalib_proto_cs , &pstEnv->pstCSHan))
	{
		return -1;
	}

	pstEnv->iCSMetaVersion = tdr_get_meta_current_version(
											pstEnv->pstCSHan->pstCSProtoPkgMeta);

	pthread_mutex_init(&pstEnv->stLogMutex, NULL);
	
	
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

	sign_parse_args(pstAppCtx->argc,pstAppCtx->argv);


	setenv("FCGI_WEB_SERVER_ADDRS",pstEnv->pstConf->FcgiWebServerIPList,1);

	srand((unsigned)time( NULL ));

	szBid[0] = 0;
	if (pstEnv->pstAppCtx->iBusinessID)
	{
		snprintf(szBid,sizeof(szBid),"_%d",pstEnv->pstAppCtx->iBusinessID);
	}

	soap_init1(&pstEnv->soap,SOAP_C_UTFSTRING|SOAP_XML_IGNORENS);
	//soap_init1(&pstEnv->soap,SOAP_C_MBSTRING|SOAP_XML_IGNORENS);
	sign_get_log_path(pstEnv);
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

	pstEnv->soap.fprepareinitrecv = sign_soap_prepare_init_recv;
	
  
	//soap_init1(&gs_stSignEnv.soap,SOAP_C_MBSTRING|SOAP_XML_IGNORENS);
	//SOAP_ENC_LATIN
	//soap_init1(&gs_stSignEnv.soap,SOAP_ENC_LATIN|SOAP_XML_IGNORENS);
	//gs_stSignEnv.soap.http_content = "text/xml; charset=gbk";
	//gs_stSignEnv.soap.prolog = "<?xml version=\"1.0\" encoding=\"GBK\"?>\n";
	/*
	if (sign_db_init(pstEnv) < 0)
	{
		return -1;
	}*/
	/*
	pstEnv->stOpLog.Ip = inet_addr(pstEnv->pstConf->SpawnIP);
	pstEnv->stOpLog.Port = pstEnv->pstConf->SpawnPort;
	*/
	

	{
		struct sigaction stAct;
		memset(&stAct, 0, sizeof(stAct));

		stAct.sa_handler	=	sign_on_sigusr1;
		sigaction(SIGUSR1, &stAct, &pstEnv->stTappAct);
	}

	

	memset(&pstEnv->stCSPkg,0,sizeof(pstEnv->stCSPkg));
	pstEnv->iRegionAuthNum = 0;
	
	for (i = 0 ; i < pstEnv->pstConf->RegionAuthNum; i++)
	{
		RegionAuth * pstRegionAuth = NULL;
		if (i >= SIGN_MAX_REGION_NUM)
		{
			break;
		}
		pstEnv->iRegionAuthNum++;

		pstRegionAuth = &pstEnv->stRegionAuth[i];
		
		pstRegionAuth->iAuthPort = pstEnv->pstConf->RegionAuthInfo[i].AuthPort;
		pstRegionAuth->iRegionID = pstEnv->pstConf->RegionAuthInfo[i].RegionID;
		STRNCPY(pstRegionAuth->szAuthIP,pstEnv->pstConf->RegionAuthInfo[i].AuthIP,
				sizeof(pstRegionAuth->szAuthIP));
		
		pstRegionAuth->iHeartBeatTime = 0;
		pstRegionAuth->pstNetCltAuthHandle = NULL;
		pthread_mutex_init(&pstRegionAuth->stDirtyMutex, NULL);
		
		pstRegionAuth->iSocket = -1;

		if ( 0 > net_clt_auth_han_create_init(pstEnv->pstCSHan->pstCSProtoLib,
								&pstRegionAuth->pstNetCltAuthHandle))
		{
			printf("net_clt_auth_han_create_init fail\n");
			SignMutiLog(MUTI_LOGLEVEL_ERROR, "net_clt_auth_han_create_init failed");
			return -1;
		}

		if (0 > net_clt_auth_conn(pstRegionAuth->szAuthIP, 
								pstRegionAuth->iAuthPort,&pstRegionAuth->iSocket))
		{
			printf("net_clt_auth_conn fail\n");
			SignMutiLog(MUTI_LOGLEVEL_ERROR, "net_clt_auth_conn failed");
			return -1;
		}

		if (0 > net_clt_auth_heart_beat_web_sign(pstRegionAuth->pstNetCltAuthHandle,&pstEnv->stCSPkg,
						pstRegionAuth->iSocket))
		{
			printf("net_clt_auth_heart_beat fail\n");
			SignMutiLog(MUTI_LOGLEVEL_ERROR, "net_clt_auth_heart_beat failed");
			return -1;
		}

		pstRegionAuth->iHeartBeatTime = time(NULL);
	}

	
	pstEnv->pstRsa = rsa_from_public_key(IDRSAPublicKey);

	if (!pstEnv->pstRsa)
	{
		printf("rsa_from_public_key fail\n");
		
		return -1;
	}	

	if (sign_thread_init(pstEnv) < 0)
	{		
		SignMutiLog(MUTI_LOGLEVEL_ERROR, "chat_mon_thread_init failed");
		return -1;
	}
	
	printf("sign_service start\n");
	SignMutiLog(MUTI_LOGLEVEL_INFO, "sign_service start");

	UNUSED(HEX_value);
		
	return 0;
}	

/*
int sign_oplog_limit_init(ACCDBFAILEDOPLOG* pstOpLog)
{
	pstOpLog->ClientIp = 0;
	pstOpLog->ClientTimeStamp = 0;
	pstOpLog->ErrCode = 0;
	pstOpLog->RegionID = 0;
	pstOpLog->OpSql[0] = 0;
	memset(&pstOpLog->MetaInfo,0,sizeof(pstOpLog->MetaInfo));

	return 0;
}
*/

int sign_db_op_in(SIGN_SERVICEENV *pstEnv,TDRDBHANDLE dbHandle,int iRegionID,
	int iOpType,SignMetaInfo* pstMetaInfo,TDRDATA *pstSql,TDRDBOBJECT* pstFetchDBObj,
	int* piFetchNum,int iFetchLimit)
{
	int iRet = 0;
	TDRDBRESULTHANDLE hDBResult = NULL;	
	int iNumRows = 0;

	//pstEnv->stOpLog.RegionID = iRegionID;	
	
	if (!dbHandle)
	{
		pstEnv->iErrCode = SIGN_ERR_DB_HANDLE;
		return -1;
	}

	if (pstMetaInfo->iUseSql)
	{
		pstSql->iBuff = pstMetaInfo->iUseSql;
	}
	else
	{
	
		iRet = tdr_obj2sql(dbHandle, pstSql, iOpType, &pstEnv->stDBObj,pstMetaInfo->szWhere);
		if (0 != iRet)
		{
			SignMutiLog(MUTI_LOGLEVEL_ERROR,
				"生成查询资源记录的SQL语句失败: %s dbms error:%s region:%d meta:%s op:%d",
				tdr_error_string(iRet), tdr_dbms_error(dbHandle),
				iRegionID,pstMetaInfo->szMetaName,iOpType);

			pstEnv->iErrCode = SIGN_ERR_SQL_CREATE;
			return iRet;
		}
	}

/*
	if (pstMetaInfo->iFailRecord)
	{
		STRNCPY(pstEnv->stOpLog.OpSql,pstSql->pszBuff, sizeof(pstEnv->stOpLog.OpSql));
	}
	*/

	SignMutiLog(MUTI_LOGLEVEL_DEBUG,
			"region:%d meta:%s op:%d sql:%s",
			iRegionID,pstMetaInfo->szMetaName,iOpType,pstEnv->szSql);

	iRet = tdr_query(&hDBResult, dbHandle, pstSql);

	if(iRet != 0)
	{
		SignMutiLog(MUTI_LOGLEVEL_ERROR,"执行SQL语句失败:%s"
			" dbms error:%s region:%d meta:%s op:%d sql:%s",
			tdr_error_string(iRet), tdr_dbms_error(dbHandle),
			iRegionID,pstMetaInfo->szMetaName,iOpType,pstEnv->szSql);
		pstEnv->iErrCode = SIGN_ERR_SQL_QUERY;
		return iRet;
	}

	if (!hDBResult)
	{
		SignMutiLog(MUTI_LOGLEVEL_ERROR,
			" null hDBResult region:%d meta:%s op:%d sql:%s",
			iRegionID,pstMetaInfo->szMetaName,iOpType,pstEnv->szSql);
		pstEnv->iErrCode = SIGN_ERR_DB_RESULT;
		return -1;
	}

	
	if (!pstMetaInfo->iEffectRowIgnore && 
		(TDR_DBOP_DELETE == iOpType ||
		TDR_DBOP_UPDATE == iOpType ||
		TDR_DBOP_INSERT == iOpType))
	{
		iNumRows = tdr_affected_rows(hDBResult);
		if(iNumRows < 1)
		{
			SignMutiLog(MUTI_LOGLEVEL_ERROR,"effect rows:%d region:%d sql:%s",
						iNumRows,
						iRegionID,
						pstEnv->szSql);

			tdr_free_dbresult(&hDBResult);
			pstEnv->iErrCode = SIGN_ERR_SQL_EFFECT_ROW;
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
			pstEnv->iErrCode = SIGN_ERR_NOT_FOUND;
			iRet = -1;
		}
		else
		{
			pstRowObj.pszObj += (*piFetchNum) * pstRowObj.iObjSize;
			iRet = tdr_fetch_row(hDBResult, &pstRowObj);
			if (iRet != 0)
			{
				SignMutiLog(MUTI_LOGLEVEL_ERROR,
					"从数据库表中读取数据记录失败:%s dbms error:%s",
					tdr_error_string(iRet), tdr_dbms_error(dbHandle)
					);
				iRet = -1;
			}
			(*piFetchNum)++;
		}
	}

	tdr_free_dbresult(&hDBResult);

	return iRet;
}

int sign_db_op_log_in(SIGN_SERVICEENV *pstEnv,
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
		SignMutiLog(MUTI_LOGLEVEL_ERROR,
			"生成查询资源记录的SQL语句失败: %s dbms error:%s",
			tdr_error_string(iRet), tdr_dbms_error(dbHandle)
			);

		return iRet;
	}


	SignMutiLog(MUTI_LOGLEVEL_DEBUG,
			"sql:%s",
			pstEnv->szOpLogSql);

	iRet = tdr_query(&hDBResult, dbHandle, pstSql);

	if(iRet != 0)
	{
		SignMutiLog(MUTI_LOGLEVEL_ERROR,"执行SQL语句失败:%s"
			" dbms error:%s sql:%s",
			tdr_error_string(iRet), tdr_dbms_error(dbHandle),
			pstEnv->szOpLogSql);

		return iRet;
	}

	if (!hDBResult)
	{
		SignMutiLog(MUTI_LOGLEVEL_ERROR,
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
			SignMutiLog(MUTI_LOGLEVEL_ERROR,"effect rows:%d sql:%s",
						iNumRows,
						pstEnv->szOpLogSql);

			tdr_free_dbresult(&hDBResult);
			return -1;
		}	
	}

	

	tdr_free_dbresult(&hDBResult);

	return 0;
}

/*
int sign_db_op_log(SIGN_SERVICEENV *pstEnv)
{
	TDRDBOBJECT stDBObj;
	TDRDATA stSql;	

	pstEnv->stOpLog.ErrCode = pstEnv->iErrCode;
	pstEnv->stOpLog.ClientTimeStamp = pstEnv->iClientTimeStamp;
	pstEnv->stOpLog.ClientIp = pstEnv->iClientIP;
	pstEnv->stOpLog.LogTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	
	stSql.iBuff = sizeof(pstEnv->szOpLogSql);
	stSql.pszBuff = pstEnv->szOpLogSql;
	
	stDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstRoleDBMetaLib, "SignDBFailedOpLog");
	stDBObj.iVersion = tdr_get_meta_current_version(stDBObj.pstMeta);
	stDBObj.iObjSize = sizeof(pstEnv->stOpLog);
	stDBObj.pszObj = (char*) &pstEnv->stOpLog;

	sign_db_op_log_in(pstEnv, TDR_DBOP_INSERT, &stDBObj, &stSql);
		


	return 0;
}*/

int sign_db_op(SIGN_SERVICEENV *pstEnv,int iRegionID,int iOpType,SignMetaInfo* pstMetaInfo,
	TDRDBOBJECT* pstFetchDBObj,int* piFetchNum,int iFetchLimit,int* piFetchRegion)
{
	TDRDATA stSql;	
	int iRet = 0;
	int i = 0;
	int iFind = 0;
	int iLastErrNum = 0;

	if (!pstMetaInfo ||
		!pstMetaInfo->pszObj ||
		0 == pstMetaInfo->szMetaName[0] ||
		0 >= pstMetaInfo->iObjSize)
	{
		SignMutiLog(MUTI_LOGLEVEL_ERROR,"pstMetaInfo err");
		pstEnv->iErrCode = SIGN_ERR_ARGS;
		return -1;
	}

	if (piFetchNum)
	{
		*piFetchNum = 0;
	}

	//pstEnv->stOpLog.OpType = iOpType;
	
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

	//pstEnv->stOpLog.MetaInfo.Version = pstEnv->stDBObj.iVersion;

	for (i = 0; i < pstEnv->iDBConnNum;i++)
	{
		SignDBInfo* pstInfo = &pstEnv->astDBInfo[i];
		int* piRegion = NULL;

		if (piFetchNum && piFetchRegion)
		{
			piRegion = piFetchRegion;
			piRegion += *piFetchNum;
		}
		
		if (iRegionID != 0 &&
			iRegionID != pstInfo->iRegionID)
		{
			continue;
		}

		iFind = 1;

		if (pstFetchDBObj)
		{
			iRet = sign_db_op_in(pstEnv,pstInfo->dbHandle,pstInfo->iRegionID,iOpType,
					pstMetaInfo, &stSql,pstFetchDBObj,piFetchNum,iFetchLimit);
		}
		else
		{
			iRet = sign_db_op_in(pstEnv,pstInfo->dbHandle,pstInfo->iRegionID,iOpType,
					pstMetaInfo, &stSql,NULL,piFetchNum,iFetchLimit);
		}

		if(iRet != 0)
		{
			SignMutiLog(MUTI_LOGLEVEL_ERROR,"sign_db_op_in err:%d",iRet);
			/*
			if (pstMetaInfo->iFailRecord)
			{
				sign_db_op_log(pstEnv);
			}*/
			iLastErrNum = iRet;
		}	

		if(piRegion)
		{
			*piRegion = pstInfo->iRegionID;
		}

		if (iRegionID != 0 &&
			iRegionID == pstInfo->iRegionID)
		{
			break;
		}
	}

	if (!iFind)
	{
		iRet = -1;
		SignMutiLog(MUTI_LOGLEVEL_ERROR,"region not found:%d",iRegionID);
		pstEnv->iErrCode = SIGN_ERR_REGION_NOT_FOUND;
		/*
		if (pstMetaInfo->iFailRecord)
		{
			sign_db_op_log(pstEnv);
		}
		*/
	}	

	return iLastErrNum;
}

RegionAuth* get_region_auth_info(SIGN_SERVICEENV* pstEnv,int iRegionID)
{
	int i = 0;
	for (i = 0; i < SIGN_MAX_REGION_NUM; i++)
	{
		if (i >= pstEnv->iRegionAuthNum)
		{
			break;
		}
		
		if (pstEnv->stRegionAuth[i].iRegionID == iRegionID)
		{
			return &pstEnv->stRegionAuth[i];
		}
	}
	
	return NULL;
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetSign(struct soap* soap, 
	struct ns1__GetSignReq *InBody, 
	struct ns1__GetSignRes *OutBody)
{	
	SIGN_SERVICEENV* pstEnv = &gs_stSignEnv;
	int iRet = 0;	
	WEBSIGNINFOHEX stRes;
	//WEBSIGNINFO stWebSign;
	int iLen = 0;
	//char * p ;
	NetCltAuthInfo stNetCltAuthInfo;
	char szUin[12];
	char szPwd[16];
	char szDecrypt[AUTH_LEN];
	int iHexLen;
	char *p = NULL;
	int iReconn = 0;
	RegionAuth* pstRegionAuth = NULL;

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = SIGN_ERR_SYS;
		SIGN_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	pstRegionAuth = get_region_auth_info(pstEnv,*InBody->RegionID);
	if (!pstRegionAuth)
	{
		OutBody->ErrorCode = SIGN_ERR_SYS;
		SIGN_ERR_TEXT(OutBody->StrError,"get_region_auth_info fail,region id:%d",*InBody->RegionID);
		return 0;
	}

	if (!InBody->Uin)
	{
		OutBody->ErrorCode = SIGN_ERR_SYS;
		SIGN_ERR_TEXT(OutBody->StrError,"need InBody->Uin");
		return 0;
	}

	snprintf(szUin,sizeof(szUin),"%d",*InBody->Uin);

	if (!InBody->AccName)
	{
		OutBody->ErrorCode = SIGN_ERR_SYS;
		SIGN_ERR_TEXT(OutBody->StrError,"need InBody->AccName");
		return 0;
	}
	

	if (!InBody->Birthday)
	{
		OutBody->ErrorCode = SIGN_ERR_SYS;
		SIGN_ERR_TEXT(OutBody->StrError,"need InBody->Birthday");
		return 0;
	}

	if (!InBody->ClientIP)
	{
		OutBody->ErrorCode = SIGN_ERR_SYS;
		SIGN_ERR_TEXT(OutBody->StrError,"need InBody->ClientIP");
		return 0;
	}

	if (pstRegionAuth->iSocket == -1)
	{
		OutBody->ErrorCode = SIGN_ERR_NET;
		SIGN_ERR_TEXT(OutBody->StrError,"invalid socket");
		return 0;
	}

	rand_str(szPwd,sizeof(szPwd));

	iRet = net_clt_auth_msg_create(pstRegionAuth->pstNetCltAuthHandle->pstAuthMeta,
										pstEnv->pstRsa,
										InBody->AccName,
										szPwd,	
										InBody->Birthday,
										*InBody->Uin,
										inet_addr(InBody->ClientIP),
										NULL,
										NULL,
										NULL,
										NULL,
										CLT_WEB,
										AUTH_FLAG_WEB_SIGN,
										&pstEnv->stCSPkg,
										NULL);
	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		SIGN_ERR_TEXT(OutBody->StrError,"net_clt_auth_msg_create error:%d",iRet);
		return 0;
	}



	net_clt_auth_info_assign(InBody->AccName,szPwd,&pstEnv->stCSPkg,
								pstEnv->iCSMetaVersion);
	
	//memcpy(RandK1, pstPkg2->Body.AuthClt.RandK1, sizeof(RandK1));
	
	
	pthread_mutex_lock(&pstRegionAuth->stDirtyMutex);
	
	iRet = net_auth_send(pstRegionAuth->pstNetCltAuthHandle,&pstEnv->stCSPkg,
							pstRegionAuth->iSocket,1000);
	//尝试连接
	if (AUTH_ERR_NET == iRet)
	{
		tnet_close(pstRegionAuth->iSocket);

		iRet = net_clt_auth_conn(pstRegionAuth->szAuthIP,
								pstRegionAuth->iAuthPort,&pstRegionAuth->iSocket);
		if (iRet < 0)
		{
			pstRegionAuth->iSocket = -1;
		}
		else
		{
			iRet = net_auth_send(pstRegionAuth->pstNetCltAuthHandle,&pstEnv->stCSPkg,
							pstRegionAuth->iSocket,1000);
		}
		iReconn = 1;
	}

	pthread_mutex_unlock(&pstRegionAuth->stDirtyMutex);

	
	

	if (0 > iRet)
	{		
		OutBody->ErrorCode = iRet;
		SIGN_ERR_TEXT(OutBody->StrError,"net_auth_send error:%d",iRet);

		SignMutiLog(MUTI_LOGLEVEL_ERROR, "net_auth_send fail:socket=%d uin=%d acc=%s ",
									pstRegionAuth->iSocket,*InBody->Uin,InBody->AccName);
		return 0;
	}

	if (iReconn)
	{
		SignMutiLog(MUTI_LOGLEVEL_INFO, "net_clt_auth_conn");
	}

	get_clt_auth_han_info(&stNetCltAuthInfo);

	iLen = sizeof(szDecrypt);
	iRet = net_auth_recv(pstRegionAuth->pstNetCltAuthHandle, &pstEnv->stCSPkg,
				pstRegionAuth->iSocket,6000,
				stNetCltAuthInfo.szMd5, stNetCltAuthInfo.stAuthClt.RandK1,
				szDecrypt,&iLen);

	if (AUTH_ERR_NET == iRet)
	{
		tnet_close(pstRegionAuth->iSocket);

		
		iRet = net_clt_auth_conn(pstRegionAuth->szAuthIP,
								pstRegionAuth->iAuthPort,&pstRegionAuth->iSocket);
		if (iRet < 0)
		{
			pstRegionAuth->iSocket = -1;
		}
		else
		{
			iRet = net_auth_send(pstRegionAuth->pstNetCltAuthHandle,&pstEnv->stCSPkg,
							pstRegionAuth->iSocket,1000);
		}
		iReconn = 1;

		if (iRet == 0)
		{
			iLen = sizeof(szDecrypt);
			iRet = net_auth_recv(pstRegionAuth->pstNetCltAuthHandle, &pstEnv->stCSPkg,
						pstRegionAuth->iSocket,6000,
						stNetCltAuthInfo.szMd5, stNetCltAuthInfo.stAuthClt.RandK1,
						szDecrypt,&iLen);
			
		}
	}
	
	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		SIGN_ERR_TEXT(OutBody->StrError,"net_auth_recv error:%d",iRet);

		SignMutiLog(MUTI_LOGLEVEL_INFO, "net_auth_recv fail:uin=%d acc=%s ret:%d",
									*InBody->Uin,InBody->AccName,iRet);
		return 0;
	}

	if (iReconn)
	{
		SignMutiLog(MUTI_LOGLEVEL_INFO, "net_clt_auth_conn");
	}

	iRet = szDecrypt[1];

	if(iRet != AUTH_IFM_WEB_SIGN_SUCC)
	{
		OutBody->ErrorCode = iRet;
		SIGN_ERR_TEXT(OutBody->StrError,"GetSign error:%d",iRet);

		SignMutiLog(MUTI_LOGLEVEL_INFO, "GetSign fail:uin=%d acc=%s ret:%d",
									*InBody->Uin,InBody->AccName,iRet);
		return 0;
	}

		

	memset(stRes.Sign,0,sizeof(stRes.Sign));
	
	iHexLen = sizeof(stRes.Sign);
	
	iLen = sizeof(szDecrypt) + KEY_LEN;
	
	p = Bin2Hex((unsigned char*)(szDecrypt + 2),iLen,
				stRes.Sign, &iHexLen);

	if(!p)
	{
		OutBody->ErrorCode = SIGN_ERR_XML;
		SIGN_ERR_TEXT(OutBody->StrError,"Bin2Hex errno:%d errstr:%s",iRet,pstEnv->szLastErrLog);
		return 0;
	}



	iLen =  sizeof(stRes.Sign);
	/*p = Bin2Hex((unsigned char *) &stWebSign,sizeof(stWebSign),
					stRes.Sign,&iLen);
	if(!p)
	{
		OutBody->ErrorCode = SIGN_ERR_XML;
		SIGN_ERR_TEXT(OutBody->StrError,"Bin2Hex errno:%d errstr:%s",iRet,pstEnv->szLastErrLog);
		return 0;
	}
	*/

	//get_clt_auth_han_info(&stNetCltAuthInfo);
	//STRNCPY(stRes.Sign,stNetCltAuthInfo.szWebSign,sizeof(stRes.Sign));
	
	//stRes.Sign[MAX_WEB_SIGN_HEX_LEN - 1] = 0;
	
	iRet = sign_sprint_tdr(&OutBody->StrXML, "WebSignInfoHex", &stRes, sizeof(stRes));
	if(iRet < 0)
	{
		OutBody->ErrorCode = SIGN_ERR_XML;
		SIGN_ERR_TEXT(OutBody->StrError,"sign_sprint_xml errno:%d errstr:%s",iRet,pstEnv->szLastErrLog);
		return 0;
	}

	return 0;
	UNUSED(soap);
}


