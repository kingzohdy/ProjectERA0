
#include <stdio.h>

#include "tapp/tapp.h"
#include "pal/pal.h" 

#include "tdr/tdr.h"
#include "tlog/tlog.h"
#include "acc_service_conf_desc.h"
#include "version.h"
#include "tbus/tbus.h"
#include "comm_misc.h"

#include "soapH.h"
#include "AccServiceSoap.nsmap"
#include <iconv.h>
#include "fcgi_config.h"

#include <stdlib.h>
#include "comm/tconvert.h"
#include "fcgios.h"



//#include "acc_service.h"

extern char **environ;

#include "fcgi_stdio.h"
#include "cs_cgi.h"
#include "../cs_service/soapCsServiceStub.h"
#include "../cs_service/soapCsService.nsmap"


#ifndef WIN32
#define max(a, b)	((a) > (b) ? (a) : (b))
#define min(a, b)	((a) < (b) ? (a) : (b))
#endif


#define UNUSED( param ) ((void)(param))



#define MAX_ACC_ERROR_LEN 2048 //错误信息长度 
//#define DEFAULT_TIME_OUT 2000 //默认超时时间
#define MAX_STR_XML_LEN 2048000 //最大返回xml字符串长度

#define MAX_GBK_LENG 1024 //最大GBK字符串长度
#define MAX_GBK_TEXT_NUM 5 //最大GBK字符串数量
#define MAX_LOG_PATH_LENG 128 //日志目录长度
//#define MAX_ACC_SQL_LEN 20480 //SQL长度 
//#define MAX_ACC_META_NAME_LEN 64 //元数据名 
#define MAX_ACC_WHERE_LEN 1024 //元数据名 



typedef struct 
{
	char szTextGBK[MAX_GBK_LENG];
}GbkText;

struct tagAccDBInfo
{
	int iRegionID;
	int iKeepAlive;
	TDRDBHANDLE dbHandle;
};

typedef struct tagAccDBInfo AccDBInfo;

struct tagAccMetaInfo
{
	char szMetaName[MAX_DB_META_NAME_LEN];
	int iObjSize;
	char* pszObj;
	char szWhere[MAX_ACC_WHERE_LEN];
	int iFailRecord;	//是否失败时记录
	int iUseSql;//使用sql语句
	int iEffectRowIgnore;//忽略影响行小于1
};

typedef struct tagAccMetaInfo AccMetaInfo;


struct tagacc_serviceEnv
{
	ACC_SERVICECONF *pstConf;
	ACC_SERVICERUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	TAPPCTX *pstAppCtx;
	pthread_mutex_t stLogMutex;
	LPSSHANDLE pstSSHan;
	struct soap soap;
	int *pCgiHandle;
	struct ns1__ErrorResponse stErrorResponse;
	char szErrorText[MAX_ACC_ERROR_LEN];
	char szStrXML[MAX_STR_XML_LEN];
	char szStrXMLUtf8[MAX_STR_XML_LEN];
	int iGetFindHandleErr;
	int iTmp1;
	int iGbkTextUsedNum;
	GbkText aGbkText[MAX_GBK_TEXT_NUM];
	char szLogPath[MAX_LOG_PATH_LENG];
	int iDBConnNum;
	AccDBInfo astDBInfo[ACC_MAX_REGION_NUM];
	char szSql[MAX_DB_SQL_LEN];
	char szOpLogSql[MAX_DB_SQL_LEN];
	TDRDBOBJECT stDBObj;
	ACCDBFAILEDOPLOG stOpLog;
	AccDBInfo stLogDBInfo;
	int iErrCode;
	int iClientIP;
	int iClientTimeStamp;
	LPTDRMETALIB pstRoleDBMetaLib;
	struct sigaction stTappAct;
	char szLastErrLog[MAX_ACC_ERROR_LEN];
};

typedef struct tagacc_serviceEnv		ACC_SERVICEENV;
typedef struct tagacc_serviceEnv		*LACC_SERVICEENV;

static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_acc_service[];
extern unsigned char g_szMetalib_proto_ss[];


ACC_SERVICEENV gs_stAccEnv;
static ACC_SERVICERUN_STATUS gs_stStat;

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

#define AccLastErrLog(szFormat, args...) \
		do{\
			snprintf(gs_stAccEnv.szLastErrLog,sizeof(gs_stAccEnv.szLastErrLog),szFormat, ##args);\
		  }while(0)

#define AccMutiLog(iLevel, szFormat, args...) \
		do{\
			snprintf(gs_stAccEnv.szLastErrLog,sizeof(gs_stAccEnv.szLastErrLog),szFormat, ##args);\
			switch(iLevel)\
			{\
			case MUTI_LOGLEVEL_TRACE:\
				tlog_trace(gs_stAccEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_DEBUG:\
				tlog_debug(gs_stAccEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_INFO:\
				tlog_info(gs_stAccEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_WARN:\
				tlog_warn(gs_stAccEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_ERROR:\
				tlog_error(gs_stAccEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_SEVERE:\
				tlog_fatal(gs_stAccEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			default:\
				break;\
			}\
		  }while(0)

#define ACC_ERR_TEXT(pstErrRes, szFormat, args...) \
		do{\
			if (!pstErrRes)\
			{\
				pstErrRes = &gs_stAccEnv.stErrorResponse;\
			}\
			if (!pstErrRes->ErrorText)\
			{\
				pstErrRes->ErrorText = gs_stAccEnv.szErrorText;\
			}\
			if (pstErrRes->ErrorText)\
			{\
				snprintf(pstErrRes->ErrorText,MAX_ACC_ERROR_LEN,szFormat,##args);\
			}\
		  }while(0);	

int acc_db_done(ACC_SERVICEENV *pstEnv);
int acc_service_init(TAPPCTX *pstAppCtx, ACC_SERVICEENV *pstEnv);
int acc_service_fini(TAPPCTX *pstAppCtx, ACC_SERVICEENV *pstEnv);
int acc_oplog_limit_init(ACCDBFAILEDOPLOG* pstOpLog);


int acc_get_log_path(ACC_SERVICEENV *pstEnv)
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

int acc_parse_args(int argc, char **argv)
{
   int c;
   ACC_SERVICEENV *pstEnv = &gs_stAccEnv;
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
						

					pstMeta = tdr_get_meta_by_name((LPTDRMETALIB)g_szMetalib_acc_service, 
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

int acc_soap_done(ACC_SERVICEENV *pstEnv)
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

int acc_service_proc(TAPPCTX *pstAppCtx, ACC_SERVICEENV *pstEnv)
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
	acc_soap_done(pstEnv);

	if (iBusy)
	{
		return 0;
	}
	else
	{
		return -1;
	}
	
err:
	AccMutiLog(MUTI_LOGLEVEL_ERROR, "soap err:%d", soap->error);
	goto final;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int acc_service_reload(TAPPCTX *pstAppCtx, ACC_SERVICEENV *pstEnv)
{
	ACC_SERVICECONF   *pstPreConf;
	
	pstPreConf = (ACC_SERVICECONF   *)pstAppCtx->stConfPrepareData.pszBuff;
	
	printf("acc_service reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "acc_service reload");

	return 0;
}

int acc_service_tick(TAPPCTX *pstAppCtx, ACC_SERVICEENV *pstEnv)
{
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}

int acc_service_stop(TAPPCTX *pstAppCtx, ACC_SERVICEENV *pstEnv)
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
	void* pvArg	=	&gs_stAccEnv;

	memset(&gs_stAppCtx, 0, sizeof(gs_stAppCtx));
	memset(&gs_stAccEnv, 0, sizeof(gs_stAccEnv));

	gs_stAppCtx.argc	=	argc;
	gs_stAppCtx.argv	=	argv;

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)acc_service_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_acc_service;
	gs_stAppCtx.stConfData.pszMetaName = "acc_serviceconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);
		
	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	acc_parse_args(argc,argv);

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)acc_service_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)acc_service_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)acc_service_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)acc_service_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)acc_service_stop;
	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	tapp_def_fini(&gs_stAppCtx, pvArg);
	

	
	return 0;
} 

int acc_get_gbk_str_in(char ** ppIn,size_t * ppInLen,char** ppOut,size_t * ppOutLen)
{
	int iConvRet = 0;
	
	iconv_t cd = iconv_open("gbk","utf-8");

	iConvRet = iconv(cd, ppIn, ppInLen,ppOut, ppOutLen);
	iconv_close(cd);

	return iConvRet;
}


char* acc_get_gbk_str(char * pszUTF8)
{
	size_t sInLen = strlen(pszUTF8);
	size_t sOutLen = sizeof(GbkText);
	char *pIn = pszUTF8;
	char *pOut = NULL;
	int iConvRet = 0;
	GbkText *pstText = NULL;
	ACC_SERVICEENV* pstEnv = &gs_stAccEnv;

	if (pstEnv->iGbkTextUsedNum < 0 ||
		pstEnv->iGbkTextUsedNum >= MAX_GBK_TEXT_NUM)
	{
		return NULL;
	}
	
	pstText = &pstEnv->aGbkText[pstEnv->iGbkTextUsedNum];
	pstEnv->iGbkTextUsedNum++;
	
	pOut = pstText->szTextGBK;	

	memset(pstText,0,sizeof(*pstText));
	
	iConvRet =acc_get_gbk_str_in(&pIn, &sInLen,&pOut, &sOutLen);
	
	if (iConvRet < 0)
	{			
		return NULL;
	}

	return pstText->szTextGBK;
}

int acc_sprint_xml(
	char **ppStrXml,
	const char *meta_name,
	const void *data,
	int data_size)
{
	int iRet = 0;
	int iLen = 0;

	TDRDATA host_data;
	TDRDATA net_data;

	ACC_SERVICEENV* pstEnv = &gs_stAccEnv;

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
	}
	else
	{
		AccMutiLog(MUTI_LOGLEVEL_ERROR,tdr_error_string(iRet));
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


int acc_service_fini(TAPPCTX *pstAppCtx, ACC_SERVICEENV *pstEnv)
{
	acc_db_done(pstEnv);

	soap_end(&gs_stAccEnv.soap);
	
	printf("service_svr stop\n");
	AccMutiLog(MUTI_LOGLEVEL_INFO, "service_svr stop");
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}

int acc_db_init(ACC_SERVICEENV *pstEnv)
{
	int i = 0; 
	AccDBInfo* pstInfo = NULL;
	REGIONDBINFO* pstDBConf = NULL;
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
			AccMutiLog(MUTI_LOGLEVEL_ERROR, "tdr_open_dbhanlde regionid:%d err:%s",
						pstInfo->iRegionID ,pstEnv->szErrorText);
			return -1;
		}
		pstInfo->iKeepAlive = 1;
	}
	
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
			AccMutiLog(MUTI_LOGLEVEL_ERROR, "tdr_open_dbhanlde regionid:%d err:%s",
						pstInfo->iRegionID ,pstEnv->szErrorText);
			return -1;
		}
		pstInfo->iKeepAlive = 1;
	}

	return 0;
}

int acc_db_done(ACC_SERVICEENV *pstEnv)
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

int acc_soap_prepare_init_recv(struct soap* soap)
{
	int i = 0;
	int iRet = 0;
	ACC_SERVICEENV *pstEnv = &gs_stAccEnv;
	
	for (i = 0; i < pstEnv->iDBConnNum;i++)
	{
		iRet = tdr_keep_dbmsconnection(pstEnv->astDBInfo[i].dbHandle);
		if (iRet < 0)
		{
			AccMutiLog(MUTI_LOGLEVEL_ERROR,"tdr_keep_dbmsconnection err:%d",
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
		AccMutiLog(MUTI_LOGLEVEL_ERROR,"tdr_keep_dbmsconnection err:%d",
			iRet);
	}
	else
	{
		pstEnv->stLogDBInfo.iKeepAlive = 1;
	}

	acc_oplog_limit_init(&pstEnv->stOpLog);

	pstEnv->iGbkTextUsedNum = 0;

	pstEnv->szLastErrLog[0] = 0;
	
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

void acc_on_sigusr1(int iSig)
{
	ACC_SERVICEENV *pstEnv = &gs_stAccEnv;
		
	OS_ShutdownPending();
	sigaction(SIGUSR1, &pstEnv->stTappAct,NULL);
	kill(getpid(),iSig);
}


int acc_service_init(TAPPCTX *pstAppCtx, ACC_SERVICEENV *pstEnv)
{
	char szLogFile[MAX_LOG_PATH_LENG];
	char szBid[64];
	
		

	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		return -1;
	}
	pstEnv->pstConf = (ACC_SERVICECONF *)pstAppCtx->stConfData.pszBuff;

	pstEnv->pstRunStat = (ACC_SERVICERUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
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
	acc_get_log_path(pstEnv);
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

	pstEnv->soap.fprepareinitrecv = acc_soap_prepare_init_recv;
	
  
	//soap_init1(&gs_stAccEnv.soap,SOAP_C_MBSTRING|SOAP_XML_IGNORENS);
	//SOAP_ENC_LATIN
	//soap_init1(&gs_stAccEnv.soap,SOAP_ENC_LATIN|SOAP_XML_IGNORENS);
	//gs_stAccEnv.soap.http_content = "text/xml; charset=gbk";
	//gs_stAccEnv.soap.prolog = "<?xml version=\"1.0\" encoding=\"GBK\"?>\n";
	
	if (acc_db_init(pstEnv) < 0)
	{
		return -1;
	}

	pstEnv->stOpLog.Ip = inet_addr(pstEnv->pstConf->SpawnIP);
	pstEnv->stOpLog.Port = pstEnv->pstConf->SpawnPort;

	{
		struct sigaction stAct;
		memset(&stAct, 0, sizeof(stAct));

		stAct.sa_handler	=	acc_on_sigusr1;
		sigaction(SIGUSR1, &stAct, &pstEnv->stTappAct);
	}
	
	printf("acc_service start\n");
	AccMutiLog(MUTI_LOGLEVEL_INFO, "acc_service start");

	UNUSED(HEX_value);
		
	return 0;
}	

int acc_oplog_limit_init(ACCDBFAILEDOPLOG* pstOpLog)
{
	pstOpLog->ClientIp = 0;
	pstOpLog->ClientTimeStamp = 0;
	pstOpLog->ErrCode = 0;
	pstOpLog->RegionID = 0;
	pstOpLog->OpSql[0] = 0;
	memset(&pstOpLog->MetaInfo,0,sizeof(pstOpLog->MetaInfo));

	return 0;
}

int acc_db_op_in(ACC_SERVICEENV *pstEnv,TDRDBHANDLE dbHandle,int iRegionID,
	int iOpType,AccMetaInfo* pstMetaInfo,TDRDATA *pstSql,TDRDBOBJECT* pstFetchDBObj,
	int* piFetchNum,int iFetchLimit)
{
	int iRet = 0;
	TDRDBRESULTHANDLE hDBResult = NULL;	
	int iNumRows = 0;

	pstEnv->stOpLog.RegionID = iRegionID;	
	
	if (!dbHandle)
	{
		pstEnv->iErrCode = ACC_ERR_DB_HANDLE;
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
			AccMutiLog(MUTI_LOGLEVEL_ERROR,
				"生成查询资源记录的SQL语句失败: %s dbms error:%s region:%d meta:%s op:%d",
				tdr_error_string(iRet), tdr_dbms_error(dbHandle),
				iRegionID,pstMetaInfo->szMetaName,iOpType);

			pstEnv->iErrCode = ACC_ERR_SQL_CREATE;
			return iRet;
		}
	}

	if (pstMetaInfo->iFailRecord)
	{
		STRNCPY(pstEnv->stOpLog.OpSql,pstSql->pszBuff, sizeof(pstEnv->stOpLog.OpSql));
	}

	AccMutiLog(MUTI_LOGLEVEL_DEBUG,
			"region:%d meta:%s op:%d sql:%s",
			iRegionID,pstMetaInfo->szMetaName,iOpType,pstEnv->szSql);

	iRet = tdr_query(&hDBResult, dbHandle, pstSql);

	if(iRet != 0)
	{
		AccMutiLog(MUTI_LOGLEVEL_ERROR,"执行SQL语句失败:%s"
			" dbms error:%s region:%d meta:%s op:%d sql:%s",
			tdr_error_string(iRet), tdr_dbms_error(dbHandle),
			iRegionID,pstMetaInfo->szMetaName,iOpType,pstEnv->szSql);
		pstEnv->iErrCode = ACC_ERR_SQL_QUERY;
		return iRet;
	}

	if (!hDBResult)
	{
		AccMutiLog(MUTI_LOGLEVEL_ERROR,
			" null hDBResult region:%d meta:%s op:%d sql:%s",
			iRegionID,pstMetaInfo->szMetaName,iOpType,pstEnv->szSql);
		pstEnv->iErrCode = ACC_ERR_DB_RESULT;
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
			AccMutiLog(MUTI_LOGLEVEL_ERROR,"effect rows:%d region:%d sql:%s",
						iNumRows,
						iRegionID,
						pstEnv->szSql);

			tdr_free_dbresult(&hDBResult);
			pstEnv->iErrCode = ACC_ERR_SQL_EFFECT_ROW;
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
			pstRowObj.pszObj += (*piFetchNum) * pstRowObj.iObjSize;
			iRet = tdr_fetch_row(hDBResult, &pstRowObj);
			if (iRet != 0)
			{
				AccMutiLog(MUTI_LOGLEVEL_ERROR,
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

int acc_db_op_log_in(ACC_SERVICEENV *pstEnv,
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
		AccMutiLog(MUTI_LOGLEVEL_ERROR,
			"生成查询资源记录的SQL语句失败: %s dbms error:%s",
			tdr_error_string(iRet), tdr_dbms_error(dbHandle)
			);

		return iRet;
	}


	AccMutiLog(MUTI_LOGLEVEL_DEBUG,
			"sql:%s",
			pstEnv->szOpLogSql);

	iRet = tdr_query(&hDBResult, dbHandle, pstSql);

	if(iRet != 0)
	{
		AccMutiLog(MUTI_LOGLEVEL_ERROR,"执行SQL语句失败:%s"
			" dbms error:%s sql:%s",
			tdr_error_string(iRet), tdr_dbms_error(dbHandle),
			pstEnv->szOpLogSql);

		return iRet;
	}

	if (!hDBResult)
	{
		AccMutiLog(MUTI_LOGLEVEL_ERROR,
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
			AccMutiLog(MUTI_LOGLEVEL_ERROR,"effect rows:%d sql:%s",
						iNumRows,
						pstEnv->szOpLogSql);

			tdr_free_dbresult(&hDBResult);
			return -1;
		}	
	}

	

	tdr_free_dbresult(&hDBResult);

	return 0;
}


int acc_db_op_log(ACC_SERVICEENV *pstEnv)
{
	TDRDBOBJECT stDBObj;
	TDRDATA stSql;	

	pstEnv->stOpLog.ErrCode = pstEnv->iErrCode;
	pstEnv->stOpLog.ClientTimeStamp = pstEnv->iClientTimeStamp;
	pstEnv->stOpLog.ClientIp = pstEnv->iClientIP;
	pstEnv->stOpLog.LogTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	
	stSql.iBuff = sizeof(pstEnv->szOpLogSql);
	stSql.pszBuff = pstEnv->szOpLogSql;
	
	stDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstRoleDBMetaLib, "AccDBFailedOpLog");
	stDBObj.iVersion = tdr_get_meta_current_version(stDBObj.pstMeta);
	stDBObj.iObjSize = sizeof(pstEnv->stOpLog);
	stDBObj.pszObj = (char*) &pstEnv->stOpLog;

	acc_db_op_log_in(pstEnv, TDR_DBOP_INSERT, &stDBObj, &stSql);
		


	return 0;
}

int acc_db_op(ACC_SERVICEENV *pstEnv,int iRegionID,int iOpType,AccMetaInfo* pstMetaInfo,
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
		AccMutiLog(MUTI_LOGLEVEL_ERROR,"pstMetaInfo err");
		pstEnv->iErrCode = ACC_ERR_ARGS;
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

	for (i = 0; i < pstEnv->iDBConnNum;i++)
	{
		AccDBInfo* pstInfo = &pstEnv->astDBInfo[i];
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
			iRet = acc_db_op_in(pstEnv,pstInfo->dbHandle,pstInfo->iRegionID,iOpType,
					pstMetaInfo, &stSql,pstFetchDBObj,piFetchNum,iFetchLimit);
		}
		else
		{
			iRet = acc_db_op_in(pstEnv,pstInfo->dbHandle,pstInfo->iRegionID,iOpType,
					pstMetaInfo, &stSql,NULL,piFetchNum,iFetchLimit);
		}

		if(iRet != 0)
		{
			AccMutiLog(MUTI_LOGLEVEL_ERROR,"acc_db_op_in err:%d",iRet);

			if (pstMetaInfo->iFailRecord)
			{
				acc_db_op_log(pstEnv);
			}
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
		AccMutiLog(MUTI_LOGLEVEL_ERROR,"region not found:%d",iRegionID);
		pstEnv->iErrCode = ACC_ERR_REGION_NOT_FOUND;
		if (pstMetaInfo->iFailRecord)
		{
			acc_db_op_log(pstEnv);
		}
	}	

	return iLastErrNum;
}


SOAP_FMAC5 int SOAP_FMAC6 __ns1__InsertAccount(struct soap* soap, 
	struct ns1__InsertAccountReq *InBody, 
	struct ns1__InsertAccountRes *OutBody)
{	
	int iRet = 0;
	char* pszAccNameGbk = NULL;
	ACCOUNT stInsert;
	int iLen = 0;
	AccMetaInfo stMetaInfo;

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->AccName)
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"need InBody->AccName");
		return 0;
	}
	pszAccNameGbk = acc_get_gbk_str(InBody->AccName);

	if (!pszAccNameGbk)
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"get_gbk_str InBody->AccName err");
		return 0;
	}

	if (!InBody->PasswordHex)
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"need InBody->PasswordHex");
		return 0;
	}


	if (!InBody->Birthday)
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"need InBody->Birthday");
		return 0;
	}

	memset(&stInsert,0,sizeof(stInsert));

	iLen = sizeof(stInsert.PasswdHash);
	Hex2Bin(InBody->PasswordHex,strlen(InBody->PasswordHex),
		(unsigned char*)stInsert.PasswdHash,&iLen);
	
	STRNCPY(stInsert.Pass9Account,pszAccNameGbk,sizeof(stInsert.Pass9Account));
	STRNCPY(stInsert.BirthDay,InBody->Birthday,sizeof(stInsert.BirthDay));	
	stInsert.AccountCreateTime = gs_stAccEnv.pstAppCtx->stCurr.tv_sec;

	memset(&stMetaInfo,0,sizeof(stMetaInfo));
	
	STRNCPY(stMetaInfo.szMetaName,"Account",sizeof(stMetaInfo.szMetaName));
	stMetaInfo.iObjSize = sizeof(stInsert);
	stMetaInfo.pszObj = (char*) &stInsert;
	

	iRet = acc_db_op(&gs_stAccEnv,
				*InBody->RegionID,
				TDR_DBOP_INSERT,
				&stMetaInfo,
				NULL,
				NULL,
				0,
				NULL
				);
	
	if(iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		ACC_ERR_TEXT(OutBody->StrError,"InsertAccount error:%d",iRet);
		return 0;
	}

	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__UpdateAccountPasswd(struct soap* soap, 
	struct ns1__UpdateAccountPasswdReq *InBody, 
	struct ns1__UpdateAccountPasswdRes *OutBody)
{	
	int iRet = 0;
	AccMetaInfo stMetaInfo;
	UPDATEPASSWD stUpdatePasswd;
	int iLen = 0;
	ACC_SERVICEENV* pstEnv = &gs_stAccEnv;

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->AccName)
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"need InBody->AccName");
		return 0;
	}


	if (!InBody->PasswordHex)
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"need InBody->PasswordHex");
		return 0;
	}

	memset(&stMetaInfo,0,sizeof(stMetaInfo));
	
	STRNCPY(stMetaInfo.szMetaName,"UpdatePasswd",sizeof(stMetaInfo.szMetaName));
	stMetaInfo.iObjSize = sizeof(stUpdatePasswd);
	stMetaInfo.pszObj = (char*) &stUpdatePasswd;
	snprintf(stMetaInfo.szWhere,sizeof(stMetaInfo.szWhere),"where Pass9Account = '%s'",
			InBody->AccName);
	stMetaInfo.iFailRecord = 1;

	memset(&stUpdatePasswd,0,sizeof(stUpdatePasswd));

	iLen = sizeof(stUpdatePasswd.PasswdHash);
	Hex2Bin(InBody->PasswordHex,strlen(InBody->PasswordHex),
		(unsigned char*)stUpdatePasswd.PasswdHash,&iLen);
	
	iLen = sizeof(pstEnv->szErrorText);
	Bin2Hex((unsigned char*)stUpdatePasswd.PasswdHash, sizeof(stUpdatePasswd.PasswdHash),
			pstEnv->szErrorText, 
			&iLen);
	AccMutiLog(MUTI_LOGLEVEL_INFO,"passwd:%s=%s",InBody->PasswordHex,
				pstEnv->szErrorText);
	pstEnv->szErrorText[0] = 0;

	pstEnv->stOpLog.MetaInfo.Data.UpdatePasswd = stUpdatePasswd;
	pstEnv->stOpLog.MetaInfo.Type = META_DATA_TYPE_UPDATEPASSWD;
	STRNCPY(pstEnv->stOpLog.MetaInfo.Name,"UpdatePasswd",
			sizeof(pstEnv->stOpLog.MetaInfo.Name));


	iRet = acc_db_op(pstEnv,
		*InBody->RegionID,
		TDR_DBOP_UPDATE,
		&stMetaInfo,
		NULL,
		NULL,
		0,
		NULL
		);

	if (iRet < 0 &&
		ACC_ERR_SQL_EFFECT_ROW == pstEnv->iErrCode)
	{
		iRet = 0;
	}

	if(iRet < 0)
	{
		OutBody->ErrorCode = pstEnv->iErrCode;
		ACC_ERR_TEXT(OutBody->StrError,"UpdateAccountPasswd error:%d",iRet);
		return 0;
	}

	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__UpdateAccountBirthday(struct soap* soap, 
	struct ns1__UpdateAccountBirthdayReq *InBody, 
	struct ns1__UpdateAccountBirthdayRes *OutBody)
{	
	ACC_SERVICEENV* pstEnv = &gs_stAccEnv;
	int iRet = 0;
	AccMetaInfo stMetaInfo;
	UPDATEACCOUNT stUpdateAccount;

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->AccName)
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"need InBody->AccName");
		return 0;
	}

	if (!InBody->Birthday)
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"need InBody->Birthday");
		return 0;
	}

	if (!InBody->Gender)
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"need InBody->Gender");
		return 0;
	}

	memset(&stMetaInfo,0,sizeof(stMetaInfo));
	
	STRNCPY(stMetaInfo.szMetaName,"UpdateAccount",sizeof(stMetaInfo.szMetaName));
	stMetaInfo.iObjSize = sizeof(stUpdateAccount);
	stMetaInfo.pszObj = (char*) &stUpdateAccount;
	stMetaInfo.iEffectRowIgnore = 1;
	snprintf(stMetaInfo.szWhere,sizeof(stMetaInfo.szWhere),"where Pass9Account = '%s'",InBody->AccName);

	memset(&stUpdateAccount,0,sizeof(stUpdateAccount));

	stUpdateAccount.Gender = *InBody->Gender;
	STRNCPY(stUpdateAccount.Pass9Account,InBody->AccName,sizeof(stUpdateAccount.Pass9Account));
	STRNCPY(stUpdateAccount.BirthDay,InBody->Birthday,sizeof(stUpdateAccount.BirthDay));
	
	pstEnv->stOpLog.MetaInfo.Data.UpdateAccount = stUpdateAccount;
	pstEnv->stOpLog.MetaInfo.Type = META_DATA_TYPE_UPDATEACCOUNT;
	STRNCPY(pstEnv->stOpLog.MetaInfo.Name,"UpdateAccount",
			sizeof(pstEnv->stOpLog.MetaInfo.Name));

	iRet = acc_db_op(pstEnv,
		*InBody->RegionID,
		TDR_DBOP_UPDATE,
		&stMetaInfo,
		NULL,
		NULL,
		0,
		NULL
		);

	
	if (iRet < 0 &&
		ACC_ERR_SQL_EFFECT_ROW == pstEnv->iErrCode)
	{
		iRet = 0;
	}

	if(iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		ACC_ERR_TEXT(OutBody->StrError,"UpdateAccountBirthday ret:%d,error:%d",iRet,pstEnv->iErrCode);
		return 0;
	}

	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetAccountPasswd(struct soap* soap, 
	struct ns1__GetAccountPasswdReq *InBody, 
	struct ns1__GetAccountPasswdRes *OutBody)
{	
	ACC_SERVICEENV* pstEnv = &gs_stAccEnv;
	int iRet = 0;
	AccMetaInfo stMetaInfo;
	UPDATEPASSWD stUpdatePasswd;
	ACCPASSWDRES stRes;
	TDRDBOBJECT stFetchObj ;

/*
	OutBody->ErrorCode = ACC_ERR_SYS;
	ACC_ERR_TEXT(OutBody->StrError,"GetAccountPasswd not available");
	return 0;
*/

	if (!InBody->AccName)
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"need InBody->AccName");
		return 0;
	}




	memset(&stMetaInfo,0,sizeof(stMetaInfo));
	
	STRNCPY(stMetaInfo.szMetaName,"UpdatePasswd",sizeof(stMetaInfo.szMetaName));
	stMetaInfo.iObjSize = sizeof(stUpdatePasswd);
	stMetaInfo.pszObj = (char*) &stUpdatePasswd;
	snprintf(stMetaInfo.szWhere,sizeof(stMetaInfo.szWhere),"where Pass9Account = '%s'",
			InBody->AccName);

	//iRet = acc_get_account_passwd(pstEnv,
		//pszAccNameGbk
		//);

	memset(&stUpdatePasswd,0,sizeof(stUpdatePasswd));
	
	pstEnv->stOpLog.MetaInfo.Data.UpdatePasswd = stUpdatePasswd;
	pstEnv->stOpLog.MetaInfo.Type = META_DATA_TYPE_UPDATEPASSWD;
	STRNCPY(pstEnv->stOpLog.MetaInfo.Name,"UpdatePasswd",
			sizeof(pstEnv->stOpLog.MetaInfo.Name));

	memset(&stFetchObj,0,sizeof(stFetchObj));
	stFetchObj.pszObj = (char*) stRes.AccPasswd;
	stFetchObj.iObjSize = sizeof(stRes.AccPasswd[0]);

	iRet = acc_db_op(pstEnv,
		0,
		TDR_DBOP_SELECT,
		&stMetaInfo,
		&stFetchObj,
		&stRes.Num,
		MAX_ACC_PASSWD_NUM,
		stRes.RegionID
		);

	if(iRet < 0)
	{
		OutBody->ErrorCode = pstEnv->iErrCode;
		ACC_ERR_TEXT(OutBody->StrError,"GetAccountPasswd error:%d",iRet);
		return 0;
	}
	
	iRet = acc_sprint_xml(&OutBody->StrXML, "AccPasswdRes", &stRes, sizeof(stRes));
	if(iRet < 0)
	{
		OutBody->ErrorCode = ACC_ERR_XML;
		ACC_ERR_TEXT(OutBody->StrError,"acc_sprint_xml error:%d",iRet);
		return 0;
	}

	return 0;
	UNUSED(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__UpdateAccountPasswdCard(struct soap* soap, 
	struct ns1__UpdateAccountPasswdCardReq *InBody, 
	struct ns1__UpdateAccountPasswdCardRes *OutBody)
{	
	ACC_SERVICEENV* pstEnv = &gs_stAccEnv;
	int iRet = 0;
	AccMetaInfo stMetaInfo;
	UPDATEPASSWDCARD stUpdatePasswdCard;
	int iType = 0;

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->AccName)
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"need InBody->AccName");
		return 0;
	}

	if (!InBody->BindType || 
		(*InBody->BindType != 0 && *InBody->BindType != 1))
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"need InBody->BindType");
		return 0;
	}

	iType = *InBody->BindType;

	if (!InBody->RowNum)
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"need InBody->RowNum");
		return 0;
	}

	if (!InBody->ColumnNum)
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"need InBody->ColumnNum");
		return 0;
	}

	if (!InBody->PasswdCard)
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"need InBody->PasswdCard");
		return 0;
	}

	memset(&stMetaInfo,0,sizeof(stMetaInfo));
	
	STRNCPY(stMetaInfo.szMetaName,"UpdatePasswdCard",sizeof(stMetaInfo.szMetaName));
	stMetaInfo.iObjSize = sizeof(stUpdatePasswdCard);
	stMetaInfo.pszObj = (char*) &stUpdatePasswdCard;

	if (iType == 1)
	{
		//绑
		snprintf(stMetaInfo.szWhere,sizeof(stMetaInfo.szWhere),
			", UinFlag = IFNULL(UinFlag,0) | %d where Pass9Account = '%s'",
			UIN_FLAG_PASSWD_CARD,InBody->AccName);
	}
	else 
	{
		//解绑
		snprintf(stMetaInfo.szWhere,sizeof(stMetaInfo.szWhere),
			", UinFlag = UinFlag & ~%d where Pass9Account = '%s'",
			UIN_FLAG_PASSWD_CARD,InBody->AccName);
	}


	memset(&stUpdatePasswdCard,0,sizeof(stUpdatePasswdCard));

	if (iType == 1)
	{
		iRet = str_to_passwd_card(InBody->PasswdCard, *InBody->ColumnNum,
			*InBody->RowNum,&stUpdatePasswdCard.PasswdCard);

		if(iRet < 0)
		{
			OutBody->ErrorCode = iRet;
			ACC_ERR_TEXT(OutBody->StrError,"str_to_passwd_card error:%d",iRet);
			return 0;
		}
	}
	
	/*
	pstEnv->stOpLog.MetaInfo.Data.UpdatePasswdCard = stUpdatePasswdCard;
	pstEnv->stOpLog.MetaInfo.Type = META_DATA_TYPE_UPDATEPASSWDCARD;
	STRNCPY(pstEnv->stOpLog.MetaInfo.Name,"UpdatePasswdCard",
			sizeof(pstEnv->stOpLog.MetaInfo.Name));
			*/

	iRet = acc_db_op(pstEnv,
		*InBody->RegionID,
		TDR_DBOP_UPDATE,
		&stMetaInfo,
		NULL,
		NULL,
		0,
		NULL
		);

	if (iRet < 0 &&
		ACC_ERR_SQL_EFFECT_ROW == pstEnv->iErrCode)
	{
		iRet = 0;
	}

	if(iRet < 0)
	{
		OutBody->ErrorCode = pstEnv->iErrCode;
		ACC_ERR_TEXT(OutBody->StrError,"UpdateAccountPasswdCard error:%d",iRet);
		return 0;
	}

	return 0;
	UNUSED(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__BindSafeEmail(struct soap* soap, 
	struct ns1__BindSafeEmailReq *InBody, 
	struct ns1__BindSafeEmailRes *OutBody)
{	
	ACC_SERVICEENV* pstEnv = &gs_stAccEnv;
	int iRet = 0;
	AccMetaInfo stMetaInfo;
	ACCOUNTUINFLAG stAccountUinFlag;
	

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->AccName)
	{
		OutBody->ErrorCode = ACC_ERR_SYS;
		ACC_ERR_TEXT(OutBody->StrError,"need InBody->AccName");
		return 0;
	}
	

	memset(&stMetaInfo,0,sizeof(stMetaInfo));
	
	STRNCPY(stMetaInfo.szMetaName,"AccountUinFlag",sizeof(stMetaInfo.szMetaName));
	stMetaInfo.iObjSize = sizeof(stAccountUinFlag);
	stMetaInfo.pszObj = (char*) &stAccountUinFlag;
	stMetaInfo.iEffectRowIgnore = 1;
	stMetaInfo.iUseSql = 1;
	stMetaInfo.iUseSql = 
				snprintf(pstEnv->szSql,sizeof(pstEnv->szSql),
					"UPDATE Account SET UinFlag=IFNULL(UinFlag,0) | %d where Pass9Account='%s';",
						UIN_FLAG_SAFE_EMAIL,
						InBody->AccName);


	memset(&stAccountUinFlag,0,sizeof(stAccountUinFlag));
	
	pstEnv->stOpLog.MetaInfo.Data.AccountUinFlag = stAccountUinFlag;
	pstEnv->stOpLog.MetaInfo.Type = META_DATA_TYPE_ACCOUNTUINFLAG;
	STRNCPY(pstEnv->stOpLog.MetaInfo.Name,"AccountUinFlag",
			sizeof(pstEnv->stOpLog.MetaInfo.Name));

	iRet = acc_db_op(pstEnv,
		*InBody->RegionID,
		TDR_DBOP_UPDATE,
		&stMetaInfo,
		NULL,
		NULL,
		0,
		NULL
		);

	if(iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		ACC_ERR_TEXT(OutBody->StrError,"BindSafeEmail error:%d",iRet);
		return 0;
	}
	else
	{
		int i = 0; 
		struct soap *soap2 = NULL;
		int iRegionID = *InBody->RegionID;
		int iType = ACC_SET_UIN_FLAG_TYPE_NOTICE;
		int iUinFlag = UIN_FLAG_SAFE_EMAIL;
		struct soapCsService1__SetAccountUinFlagReq stReq;
		struct soapCsService1__SetAccountUinFlagRes stRes;

		

		
		for (i = 0; i < pstEnv->iDBConnNum;i++)
		{
			AccDBInfo* pstInfo = &pstEnv->astDBInfo[i];	

			soap2 = soap_copy(soap);
			soap_set_namespaces(soap2, soapCsService_namespaces);
			soap2->userid = pstEnv->pstConf->HttpBaseUserID;
			soap2->passwd = pstEnv->pstConf->HttpBaseUserPasswd;
		
			if (iRegionID != 0 &&
				iRegionID != pstInfo->iRegionID)
			{
				continue;
			}
			
			stReq.AccName = InBody->AccName;
			stReq.RegionID = &pstInfo->iRegionID;
			stReq.Type = &iType;
			stReq.UinFlag = &iUinFlag;
			stReq.TimeOut = 0;
			
			iRet = soap_call___soapCsService1__SetAccountUinFlag(soap2, pstEnv->pstConf->CsServiceUrl, NULL,&stReq, &stRes);
			if (iRet)
			{
				soap_receiver_fault(soap, "Cannot connect to cs_service", NULL);
			}

			if (soap2->error)
			{ 
				pstEnv->szErrorText[0] = 0;
				soap_sprint_fault(soap2, pstEnv->szErrorText,sizeof(pstEnv->szErrorText));
				
				AccMutiLog(MUTI_LOGLEVEL_ERROR,"SetAccountUinFlag fault:%s",pstEnv->szErrorText);
				
				pstEnv->szErrorText[0] = 0;
			}
			else
			{
				if (stRes.ErrorCode)
				{
					AccMutiLog(MUTI_LOGLEVEL_ERROR,"SetAccountUinFlag code:%d err:%s",
						stRes.ErrorCode,
						stRes.StrError->ErrorText);
				}
			}

			soap_end(soap2);
			soap_free(soap2);
		}
		//soap_set_namespaces(soap2, namespaces);
		
	}
	


	return 0;
	UNUSED(soap);
}

