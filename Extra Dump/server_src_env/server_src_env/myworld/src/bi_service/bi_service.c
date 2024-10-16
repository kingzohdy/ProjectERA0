
#include "tapp/tapp.h"
#include "tdr/tdr.h"
#include "tlog/tlog.h"
#include "bi_service_conf_desc.h"
#include "version.h"
#include "tbus/tbus.h"
#include "comm_misc.h"

#include "soapH.h"
#include "BiServiceSoap.nsmap"
#include <iconv.h>
#include "fcgi_config.h"

#include <stdlib.h>
#include "comm/tconvert.h"
#include <mysqld_error.h>
#include "../cs_service/soapCsServiceStub.h"
#include "../cs_service/soapCsService.nsmap"



//#include "bi_service.h"

extern char **environ;


#include "fcgi_stdio.h"
#include "cs_cgi.h"
#include "fcgios.h"

#ifndef WIN32
#define max(a, b)	((a) > (b) ? (a) : (b))
#define min(a, b)	((a) < (b) ? (a) : (b))
#endif


#define UNUSED( param ) ((void)(param))



#define MAX_BI_ERROR_LEN 2048 //错误信息长度 
//#define DEFAULT_TIME_OUT 2000 //默认超时时间
#define MAX_STR_XML_LEN 2048000 //最大返回xml字符串长度

#define MAX_GBK_LENG 1024 //最大GBK字符串长度
#define MAX_GBK_TEXT_NUM 5 //最大GBK字符串数量
#define MAX_LOG_PATH_LENG 128 //日志目录长度
//#define MAX_BI_SQL_LEN 20480 //SQL长度 
//#define MAX_BI_META_NAME_LEN 64 //元数据名 
#define MAX_BI_WHERE_LEN 1024 //元数据名 



typedef struct 
{
	char szTextGBK[MAX_GBK_LENG];
}GbkText;

struct tagBiDBInfo
{
	int iRegionID;
	int iKeepAlive;
	TDRDBHANDLE dbHandle;
	TDRDBHANDLE dbBillHandle;
};

typedef struct tagBiDBInfo BiDBInfo;

struct tagBiMetaInfo
{
	char szMetaName[MAX_DB_META_NAME_LEN];
	int iObjSize;
	char* pszObj;
	char szWhere[MAX_BI_WHERE_LEN];
	int iFailRecord;	//是否失败时记录
	int iBillDB;//是否Bill数据库
	int iUseSql;//使用sql语句
	int iDuplicateKeyUpdate;
};

typedef struct tagBiMetaInfo BiMetaInfo;


struct tagbi_serviceEnv
{
	BI_SERVICECONF *pstConf;
	BI_SERVICERUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	TAPPCTX *pstAppCtx;
	pthread_mutex_t stLogMutex;
	LPSSHANDLE pstSSHan;
	struct soap soap;
	int *pCgiHandle;
	struct ns1__ErrorResponse stErrorResponse;
	char szErrorText[MAX_BI_ERROR_LEN];
	char szStrXML[MAX_STR_XML_LEN];
	char szStrXMLUtf8[MAX_STR_XML_LEN];
	int iGetFindHandleErr;
	int iTmpRes1;
	int iTmpRes2;
	int iGbkTextUsedNum;
	GbkText aGbkText[MAX_GBK_TEXT_NUM];
	char szLogPath[MAX_LOG_PATH_LENG];
	int iDBConnNum;
	BiDBInfo astDBInfo[BI_MAX_REGION_NUM];
	char szSql[MAX_DB_SQL_LEN];
	char szOpLogSql[MAX_DB_SQL_LEN];
	TDRDBOBJECT stDBObj;
	BIDBFAILEDOPLOG stOpLog;
	BiDBInfo stLogDBInfo;
	int iErrCode;
	int iClientIP;
	int iClientTimeStamp;
	LPTDRMETALIB pstRoleDBMetaLib;
	struct sigaction stTappAct;
	int iDbmsErrNo;
	int iMysqlStage;
	int iLastInsertID;
};

typedef struct tagbi_serviceEnv		BI_SERVICEENV;
typedef struct tagbi_serviceEnv		*LBI_SERVICEENV;

static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_bi_service[];
extern unsigned char g_szMetalib_proto_ss[];


BI_SERVICEENV gs_stBiEnv;
static BI_SERVICERUN_STATUS gs_stStat;

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

#define BiMutiLog(iLevel, szFormat, args...) \
		do{\
			switch(iLevel)\
			{\
			case MUTI_LOGLEVEL_TRACE:\
				tlog_trace(gs_stBiEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_DEBUG:\
				tlog_debug(gs_stBiEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_INFO:\
				tlog_info(gs_stBiEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_WARN:\
				tlog_warn(gs_stBiEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_ERROR:\
				tlog_error(gs_stBiEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_SEVERE:\
				tlog_fatal(gs_stBiEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			default:\
				break;\
			}\
		  }while(0)

#define BI_ERR_TEXT(pstErrRes, szFormat, args...) \
		do{\
			if (!pstErrRes)\
			{\
				pstErrRes = &gs_stBiEnv.stErrorResponse;\
			}\
			if (!pstErrRes->ErrorText)\
			{\
				pstErrRes->ErrorText = gs_stBiEnv.szErrorText;\
			}\
			if (pstErrRes->ErrorText)\
			{\
				snprintf(pstErrRes->ErrorText,MAX_BI_ERROR_LEN,szFormat,##args);\
			}\
		  }while(0);	

int bi_db_done(BI_SERVICEENV *pstEnv);
int bi_service_init(TAPPCTX *pstAppCtx, BI_SERVICEENV *pstEnv);
int bi_service_fini(TAPPCTX *pstAppCtx, BI_SERVICEENV *pstEnv);
int bi_oplog_limit_init(BIDBFAILEDOPLOG* pstOpLog);


int bi_get_log_path(BI_SERVICEENV *pstEnv)
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

int bi_parse_args(int argc, char **argv)
{
	int c;
   BI_SERVICEENV *pstEnv = &gs_stBiEnv;
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
						

					pstMeta = tdr_get_meta_by_name((LPTDRMETALIB)g_szMetalib_bi_service, 
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

int bi_soap_done(BI_SERVICEENV *pstEnv)
{
	int i = 0;
	
	pstEnv->iGbkTextUsedNum = 0;
	pstEnv->iErrCode = 0;
	pstEnv->iDbmsErrNo = 0;
	pstEnv->iMysqlStage = 0;
	for (i = 0;i < pstEnv->iDBConnNum; i++)
	{
		pstEnv->astDBInfo[i].iKeepAlive = 0;		
	}
	return 0;
}

int bi_service_proc(TAPPCTX *pstAppCtx, BI_SERVICEENV *pstEnv)
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
	bi_soap_done(pstEnv);

	if (iBusy)
	{
		return 0;
	}
	else
	{
		return -1;
	}
	
err:
	BiMutiLog(MUTI_LOGLEVEL_ERROR, "soap err:%d", soap->error);
	goto final;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int bi_service_reload(TAPPCTX *pstAppCtx, BI_SERVICEENV *pstEnv)
{
	BI_SERVICECONF   *pstPreConf;
	
	pstPreConf = (BI_SERVICECONF   *)pstAppCtx->stConfPrepareData.pszBuff;
	
	printf("bi_service reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "bi_service reload");

	return 0;
}

int bi_service_tick(TAPPCTX *pstAppCtx, BI_SERVICEENV *pstEnv)
{
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}

int bi_service_stop(TAPPCTX *pstAppCtx, BI_SERVICEENV *pstEnv)
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
	void* pvArg	=	&gs_stBiEnv;

	memset(&gs_stAppCtx, 0, sizeof(gs_stAppCtx));
	memset(&gs_stBiEnv, 0, sizeof(gs_stBiEnv));

	gs_stAppCtx.argc	=	argc;
	gs_stAppCtx.argv	=	argv;

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)bi_service_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_bi_service;
	gs_stAppCtx.stConfData.pszMetaName = "bi_serviceconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);
		
	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	bi_parse_args(argc,argv);

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)bi_service_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)bi_service_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)bi_service_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)bi_service_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)bi_service_stop;
	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	tapp_def_fini(&gs_stAppCtx, pvArg);
	

	
	return 0;
} 

int bi_get_gbk_str_in(char ** ppIn,size_t * ppInLen,char** ppOut,size_t * ppOutLen)
{
	int iConvRet = 0;
	
	iconv_t cd = iconv_open("gbk","utf-8");

	iConvRet = iconv(cd, ppIn, ppInLen,ppOut, ppOutLen);
	iconv_close(cd);

	return iConvRet;
}


char* bi_get_gbk_str(char * pszUTF8)
{
	size_t sInLen = strlen(pszUTF8);
	size_t sOutLen = sizeof(GbkText);
	char *pIn = pszUTF8;
	char *pOut = NULL;
	int iConvRet = 0;
	GbkText *pstText = NULL;

	if (gs_stBiEnv.iGbkTextUsedNum < 0 ||
		gs_stBiEnv.iGbkTextUsedNum >= MAX_GBK_TEXT_NUM)
	{
		return NULL;
	}
	
	pstText = &gs_stBiEnv.aGbkText[gs_stBiEnv.iGbkTextUsedNum];
	gs_stBiEnv.iGbkTextUsedNum++;
	
	pOut = pstText->szTextGBK;	

	memset(pstText,0,sizeof(*pstText));
	
	iConvRet =bi_get_gbk_str_in(&pIn, &sInLen,&pOut, &sOutLen);
	
	if (iConvRet < 0)
	{			
		return NULL;
	}

	return pstText->szTextGBK;
}

int bi_sprint_xml(
	char **ppStrXml,
	const char *meta_name,
	const void *data,
	int data_size)
{
	int iRet = 0;
	int iLen = 0;

	TDRDATA host_data;
	TDRDATA net_data;

	BI_SERVICEENV* pstEnv = &gs_stBiEnv;

	host_data.pszBuff = (char *)data;
	host_data.iBuff = data_size;

	net_data.pszBuff = gs_stBiEnv.szStrXML;
	net_data.iBuff = sizeof(gs_stBiEnv.szStrXML);

	memset(gs_stBiEnv.szStrXML,0,sizeof(gs_stBiEnv.szStrXML));
	//gs_stBiEnv.szStrXML[0] = 0;
	
	*ppStrXml = gs_stBiEnv.szStrXML;

	//iRet = cs_sprint_xml(gs_stBiEnv.pCgiHandle,meta_name,data,data_size,
		//gs_stBiEnv.szStrXML,sizeof(gs_stBiEnv.szStrXML));
	iRet = tdr_output(tdr_get_meta_by_name(pstEnv->pstSSHan->pstSSProtoLib,meta_name),
	                   &net_data, &host_data,0, TDR_IO_NEW_XML_VERSION);

	iLen = strlen(gs_stBiEnv.szStrXML);
	
	iLen = min(iLen,MAX_STR_XML_LEN -1);
	if (iLen >= 0 && iLen <MAX_STR_XML_LEN)
	{
		gs_stBiEnv.szStrXML[iLen] = 0;
	}

	if (0 == iRet && gs_stBiEnv.pstConf->SoapEncoding == SOAP_ENCODING_TYPE_UTF8)
	{
		size_t sInLen = strlen(gs_stBiEnv.szStrXML);
		size_t sOutLen = sizeof(gs_stBiEnv.szStrXMLUtf8);
		char *pIn = gs_stBiEnv.szStrXML;
		char *pOut = gs_stBiEnv.szStrXMLUtf8;
		int iConvRet = 0;
		iconv_t cd = 0;

		memset(gs_stBiEnv.szStrXMLUtf8,0,sizeof(gs_stBiEnv.szStrXMLUtf8));
		//gs_stBiEnv.szStrXMLUtf8[0] = 0;
		
		
		cd = iconv_open("utf-8","gbk");

		iConvRet = iconv(cd, &pIn, &sInLen,&pOut, &sOutLen);
		if (iConvRet < 0)
		{
			*ppStrXml = NULL;
			iRet = -2;
		}
		else
		{
			*ppStrXml = gs_stBiEnv.szStrXMLUtf8;
			iLen = strlen(gs_stBiEnv.szStrXMLUtf8);
	
			iLen = min(iLen,MAX_STR_XML_LEN -1);
			if (iLen >= 0 && iLen <MAX_STR_XML_LEN)
			{
				gs_stBiEnv.szStrXMLUtf8[iLen] = 0;
			}
		}
		
		//printf("inlen = %d,outlen = %d\n\n", sInLen,sOutLen);
		//printf("locale = BiService = BiService,bi_service = bi_service,BI_SERVICE = BI_SERVICE,bi_ = bi_,BI_ERR = BI_ERR\n\n", setlocale(LC_CTYPE,""));
		//printf("result = BiService = BiService,bi_service = bi_service,BI_SERVICE = BI_SERVICE,bi_ = bi_,BI_ERR = BI_ERR\n\n", szRet);
		iconv_close(cd);
	}

	return iRet;
}


int bi_service_fini(TAPPCTX *pstAppCtx, BI_SERVICEENV *pstEnv)
{
	bi_db_done(pstEnv);

	soap_end(&gs_stBiEnv.soap);
	
	printf("service_svr stop\n");
	BiMutiLog(MUTI_LOGLEVEL_INFO, "service_svr stop");
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}

int bi_db_init(BI_SERVICEENV *pstEnv)
{
	int i = 0; 
	BiDBInfo* pstInfo = NULL;
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
			BiMutiLog(MUTI_LOGLEVEL_ERROR, "tdr_open_dbhanlde regionid:%d err:%s",
						pstInfo->iRegionID ,pstEnv->szErrorText);
			return -1;
		}

		
		pstInfo->iKeepAlive = 1;
	}
	
	for (i =0; i < pstEnv->pstConf->RegionDBNum;i++)
	{
		pstDBConf = &pstEnv->pstConf->RegionDBInfo[i];
		pstConnInfo = &pstDBConf->DBConnectInfo;

		pstInfo = &pstEnv->astDBInfo[pstEnv->iDBConnNum];
		pstEnv->iDBConnNum++;
		pstInfo->iRegionID = pstDBConf->RegionID;

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
			BiMutiLog(MUTI_LOGLEVEL_ERROR, "tdr_open_dbhanlde regionid:%d err:%s",
						pstInfo->iRegionID ,pstEnv->szErrorText);
			return -1;
		}

		pstConnInfo = &pstDBConf->BillDBConnectInfo;

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

		iRet = tdr_open_dbhanlde(&pstInfo->dbBillHandle,&stDBMS,
					pstEnv->szErrorText);
		
		if (iRet < 0 || pstInfo->dbBillHandle == NULL)
		{
			BiMutiLog(MUTI_LOGLEVEL_ERROR, "tdr_open_dbhanlde regionid:%d err:%s",
						pstInfo->iRegionID ,pstEnv->szErrorText);
			return -1;
		}
		
		pstInfo->iKeepAlive = 1;
	}

	return 0;
}

int bi_db_done(BI_SERVICEENV *pstEnv)
{
	int i = 0;
	
	for (i = 0; i < pstEnv->iDBConnNum;i++)
	{
		tdr_close_dbhanlde(&pstEnv->astDBInfo[i].dbHandle);
		tdr_close_dbhanlde(&pstEnv->astDBInfo[i].dbBillHandle);
		memset(&pstEnv->astDBInfo[i],0,sizeof(pstEnv->astDBInfo[i]));
	}

	tdr_close_dbhanlde(&pstEnv->stLogDBInfo.dbHandle);

	memset(&pstEnv->stLogDBInfo,0,sizeof(pstEnv->stLogDBInfo));
	
	return 0;
	
}

int bi_soap_prepare_init_recv(struct soap* soap)
{
	int i = 0;
	int iRet = 0;
	BI_SERVICEENV* pstEnv = &gs_stBiEnv;
	char *pszIP = NULL;
	
	for (i = 0; i < pstEnv->iDBConnNum;i++)
	{
		iRet = tdr_keep_dbmsconnection(pstEnv->astDBInfo[i].dbHandle);
		if (iRet < 0)
		{
			BiMutiLog(MUTI_LOGLEVEL_ERROR,"tdr_keep_dbmsconnection err:%d",
				iRet);
		}
		else
		{
			pstEnv->astDBInfo[i].iKeepAlive = 1;
		}

		iRet = tdr_keep_dbmsconnection(pstEnv->astDBInfo[i].dbBillHandle);
		if (iRet < 0)
		{
			BiMutiLog(MUTI_LOGLEVEL_ERROR,"tdr_keep_dbmsconnection err:%d",
				iRet);
		}
	}

	iRet = tdr_keep_dbmsconnection(pstEnv->stLogDBInfo.dbHandle);
	if (iRet < 0)
	{
		BiMutiLog(MUTI_LOGLEVEL_ERROR,"tdr_keep_dbmsconnection err:%d",
			iRet);
	}
	else
	{
		pstEnv->stLogDBInfo.iKeepAlive = 1;
	}

	bi_oplog_limit_init(&pstEnv->stOpLog);

	pstEnv->iGbkTextUsedNum = 0;

	pstEnv->iTmpRes1 = 0;
	pstEnv->iTmpRes2 = 0;
	pstEnv->iLastInsertID = 0;

	pszIP = getenv("REMOTE_ADDR");
	if (pszIP)
	{
		pstEnv->iClientIP = (int)inet_network(pszIP);
	}
	else
	{
		pstEnv->iClientIP = 0;
	}

	
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

void bi_on_sigusr1(int iSig)
{
	BI_SERVICEENV *pstEnv = &gs_stBiEnv;
		
	OS_ShutdownPending();
	sigaction(SIGUSR1, &pstEnv->stTappAct,NULL);
	kill(getpid(),iSig);
}


int bi_service_init(TAPPCTX *pstAppCtx, BI_SERVICEENV *pstEnv)
{
	char szLogFile[MAX_LOG_PATH_LENG];
	char szBid[64];
		

	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		return -1;
	}
	pstEnv->pstConf = (BI_SERVICECONF *)pstAppCtx->stConfData.pszBuff;

	pstEnv->pstRunStat = (BI_SERVICERUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
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
	bi_get_log_path(pstEnv);
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

	pstEnv->soap.fprepareinitrecv = bi_soap_prepare_init_recv;
	
  
	//soap_init1(&gs_stBiEnv.soap,SOAP_C_MBSTRING|SOAP_XML_IGNORENS);
	//SOAP_ENC_LATIN
	//soap_init1(&gs_stBiEnv.soap,SOAP_ENC_LATIN|SOAP_XML_IGNORENS);
	//gs_stBiEnv.soap.http_content = "text/xml; charset=gbk";
	//gs_stBiEnv.soap.prolog = "<?xml version=\"1.0\" encoding=\"GBK\"?>\n";
	
	if (bi_db_init(pstEnv) < 0)
	{
		return -1;
	}

	{
		struct sigaction stAct;
		memset(&stAct, 0, sizeof(stAct));

		stAct.sa_handler	=	bi_on_sigusr1;
		sigaction(SIGUSR1, &stAct, &pstEnv->stTappAct);
	}

	pstEnv->stOpLog.Ip = inet_addr(pstEnv->pstConf->SpawnIP);
	pstEnv->stOpLog.Port = pstEnv->pstConf->SpawnPort;
	
	printf("bi_service start\n");
	BiMutiLog(MUTI_LOGLEVEL_INFO, "bi_service start");

	UNUSED(HEX_value);
		
	return 0;
}	

int bi_oplog_limit_init(BIDBFAILEDOPLOG* pstOpLog)
{
	pstOpLog->ClientIp = 0;
	pstOpLog->ClientTimeStamp = 0;
	pstOpLog->ErrCode = 0;
	pstOpLog->RegionID = 0;
	pstOpLog->OpSql[0] = 0;
	pstOpLog->MysqlErrCode = 0;
	pstOpLog->MysqlStage = 0;
	pstOpLog->Uin = 0;
	pstOpLog->AccName[0] = 0;
	
	memset(&pstOpLog->MetaInfo,0,sizeof(pstOpLog->MetaInfo));

	return 0;
}

int bi_db_op_in(BI_SERVICEENV *pstEnv,TDRDBHANDLE dbHandle,int iRegionID,
	int iOpType,BiMetaInfo* pstMetaInfo,TDRDATA *pstSql,TDRDBOBJECT* pstFetchDBObj,
	int* piFetchNum,int iFetchLimit)
{
	int iRet = 0;
	TDRDBRESULTHANDLE hDBResult = NULL;	
	int iNumRows = 0;

	pstEnv->stOpLog.RegionID = iRegionID;	
	pstEnv->iLastInsertID = 0;
	
	if (!dbHandle)
	{
		pstEnv->iErrCode = BI_ERR_DB_HANDLE;
		return -1;
	}
	if (!pstMetaInfo->iUseSql)
	{
		int iSqlLen = pstSql->iBuff;
		
		iRet = tdr_obj2sql(dbHandle, pstSql, iOpType, &pstEnv->stDBObj,pstMetaInfo->szWhere);
		if (0 != iRet)
		{
			BiMutiLog(MUTI_LOGLEVEL_ERROR,
				"生成查询资源记录的SQL语句失败: %s dbms error:%s region:%d meta:%s op:%d",
				tdr_error_string(iRet), tdr_dbms_error(dbHandle),
				iRegionID,pstMetaInfo->szMetaName,iOpType);

			pstEnv->iErrCode = BI_ERR_SQL_CREATE;
			return iRet;
		}

		if (pstMetaInfo->iDuplicateKeyUpdate && iOpType == TDR_DBOP_INSERT)
		{
			int iWhereLen = strlen(pstMetaInfo->szWhere);
			char *pszStr = pstSql->pszBuff + pstSql->iBuff - 1;
			
			iSqlLen -= pstSql->iBuff;
			

			if (iSqlLen > iWhereLen && iWhereLen > 0)
			{
				iSqlLen = snprintf(pszStr,iSqlLen," %s",pstMetaInfo->szWhere);
				pstSql->iBuff += iSqlLen;
			}
			else
			{
				BiMutiLog(MUTI_LOGLEVEL_ERROR,
					"on duplicate key update sql statement add fail");
				
				iRet = -1;
				
				return iRet;
			}
		}
	}
	else
	{
		pstSql->iBuff = pstMetaInfo->iUseSql + 1;
	}

	if (pstMetaInfo->iFailRecord)
	{
		STRNCPY(pstEnv->stOpLog.OpSql,pstSql->pszBuff, sizeof(pstEnv->stOpLog.OpSql));
	}

	BiMutiLog(MUTI_LOGLEVEL_DEBUG,
			"region:%d meta:%s op:%d sql:%s",
			iRegionID,pstMetaInfo->szMetaName,iOpType,pstEnv->szSql);
	
	iRet = tdr_query(&hDBResult, dbHandle, pstSql);

	if(iRet != 0)
	{
		BiMutiLog(MUTI_LOGLEVEL_ERROR,"执行SQL语句失败:%s"
			" dbms error:%s region:%d meta:%s op:%d sql:%s",
			tdr_error_string(iRet), tdr_dbms_error(dbHandle),
			iRegionID,pstMetaInfo->szMetaName,iOpType,pstSql->pszBuff);
		
		pstEnv->iErrCode = BI_ERR_SQL_QUERY;

		//如果是主键冲突
		pstEnv->iDbmsErrNo = tdr_dbms_errno(dbHandle);		
		if (ER_DUP_KEY == pstEnv->iDbmsErrNo || ER_DUP_ENTRY == pstEnv->iDbmsErrNo || ER_DUP_UNIQUE == pstEnv->iDbmsErrNo)
		{
			pstEnv->iErrCode = BI_ERR_DUP;
		}
		return -1;
	}

	if (!hDBResult)
	{
		BiMutiLog(MUTI_LOGLEVEL_ERROR,
			" null hDBResult region:%d meta:%s op:%d sql:%s",
			iRegionID,pstMetaInfo->szMetaName,iOpType,pstEnv->szSql);
		pstEnv->iErrCode = BI_ERR_DB_RESULT;
		return -1;
	}

	
	if (TDR_DBOP_DELETE == iOpType ||
		TDR_DBOP_UPDATE == iOpType ||
		TDR_DBOP_INSERT == iOpType)		
	{
		iNumRows = tdr_affected_rows(hDBResult);
		if(iNumRows < 1)
		{
			BiMutiLog(MUTI_LOGLEVEL_ERROR,"effect rows:%d region:%d sql:%s",
						iNumRows,
						iRegionID,
						pstEnv->szSql);

			tdr_free_dbresult(&hDBResult);
			pstEnv->iDbmsErrNo = tdr_dbms_errno(dbHandle);	
			pstEnv->iErrCode = BI_ERR_SQL_EFFECT_ROW;
			iRet = -1;
		}	
		else
		{
			if (TDR_DBOP_INSERT == iOpType)
			{
				pstEnv->iLastInsertID = tdr_dbms_insert_id(dbHandle);
			}
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
				BiMutiLog(MUTI_LOGLEVEL_ERROR,
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

int bi_db_op_log_in(BI_SERVICEENV *pstEnv,
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
		BiMutiLog(MUTI_LOGLEVEL_ERROR,
			"生成查询资源记录的SQL语句失败: %s dbms error:%s",
			tdr_error_string(iRet), tdr_dbms_error(dbHandle)
			);

		return iRet;
	}


	BiMutiLog(MUTI_LOGLEVEL_DEBUG,
			"sql:%s",
			pstEnv->szOpLogSql);

	iRet = tdr_query(&hDBResult, dbHandle, pstSql);

	if(iRet != 0)
	{
		BiMutiLog(MUTI_LOGLEVEL_ERROR,"执行SQL语句失败:%s"
			" dbms error:%s sql:%s",
			tdr_error_string(iRet), tdr_dbms_error(dbHandle),
			pstEnv->szOpLogSql);

		return iRet;
	}

	if (!hDBResult)
	{
		BiMutiLog(MUTI_LOGLEVEL_ERROR,
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
			BiMutiLog(MUTI_LOGLEVEL_ERROR,"effect rows:%d sql:%s",
						iNumRows,
						pstEnv->szOpLogSql);

			tdr_free_dbresult(&hDBResult);
			return -1;
		}	
	}

	

	tdr_free_dbresult(&hDBResult);

	return 0;
}


int bi_db_op_log(BI_SERVICEENV *pstEnv)
{
	TDRDBOBJECT stDBObj;
	TDRDATA stSql;	

	pstEnv->stOpLog.ErrCode = pstEnv->iErrCode;
	pstEnv->stOpLog.ClientTimeStamp = pstEnv->iClientTimeStamp;
	pstEnv->stOpLog.ClientIp = pstEnv->iClientIP;
	pstEnv->stOpLog.LogTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstEnv->stOpLog.MysqlErrCode = pstEnv->iDbmsErrNo;
	pstEnv->stOpLog.MysqlStage = pstEnv->iMysqlStage;

	
	stSql.iBuff = sizeof(pstEnv->szOpLogSql);
	stSql.pszBuff = pstEnv->szOpLogSql;
	
	stDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstRoleDBMetaLib, "BiDBFailedOpLog");
	stDBObj.iVersion = tdr_get_meta_current_version(stDBObj.pstMeta);
	stDBObj.iObjSize = sizeof(pstEnv->stOpLog);
	stDBObj.pszObj = (char*) &pstEnv->stOpLog;

	bi_db_op_log_in(pstEnv, TDR_DBOP_INSERT, &stDBObj, &stSql);
		


	return 0;
}

int bi_db_op(BI_SERVICEENV *pstEnv,int iRegionID,int iOpType,BiMetaInfo* pstMetaInfo,
	TDRDBOBJECT* pstFetchDBObj,int* piFetchNum,int iFetchLimit,int* piFetchRegion)
{
	TDRDATA stSql;	
	int iRet = 0;
	int i = 0;
	int iFind = 0;

	if (!pstMetaInfo ||
		!pstMetaInfo->pszObj ||
		0 == pstMetaInfo->szMetaName[0] ||
		0 >= pstMetaInfo->iObjSize)
	{
		BiMutiLog(MUTI_LOGLEVEL_ERROR,"pstMetaInfo err");
		pstEnv->iErrCode = BI_ERR_ARGS;
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
		BiDBInfo* pstInfo = &pstEnv->astDBInfo[i];
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

		if (pstMetaInfo->iBillDB)
		{
			if (pstFetchDBObj)
			{
				iRet = bi_db_op_in(pstEnv,pstInfo->dbBillHandle,pstInfo->iRegionID,iOpType,
						pstMetaInfo, &stSql,pstFetchDBObj,piFetchNum,iFetchLimit);
			}
			else
			{
				iRet = bi_db_op_in(pstEnv,pstInfo->dbBillHandle,pstInfo->iRegionID,iOpType,
						pstMetaInfo, &stSql,NULL,piFetchNum,iFetchLimit);
			}
		}
		else
		{
			if (pstFetchDBObj)
			{
				iRet = bi_db_op_in(pstEnv,pstInfo->dbHandle,pstInfo->iRegionID,iOpType,
						pstMetaInfo, &stSql,pstFetchDBObj,piFetchNum,iFetchLimit);
			}
			else
			{
				iRet = bi_db_op_in(pstEnv,pstInfo->dbHandle,pstInfo->iRegionID,iOpType,
						pstMetaInfo, &stSql,NULL,piFetchNum,iFetchLimit);
			}
		}

		if(iRet != 0)
		{
			BiMutiLog(MUTI_LOGLEVEL_ERROR,"bi_db_op_in err:%d",iRet);

			if (pstMetaInfo->iFailRecord )
			{
				bi_db_op_log(pstEnv);
			}
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
		BiMutiLog(MUTI_LOGLEVEL_ERROR,"region not found:%d",iRegionID);
		pstEnv->iErrCode = BI_ERR_REGION_NOT_FOUND;
		if (pstMetaInfo->iFailRecord)
		{
			bi_db_op_log(pstEnv);
		}
	}	

	return iRet;
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetAccountStat(struct soap* soap, 
	struct ns1__GetAccountStatReq *InBody, 
	struct ns1__GetAccountStatRes *OutBody)
{	
	BI_SERVICEENV* pstEnv = &gs_stBiEnv;
	int iRet = 0;
	BiMetaInfo stMetaInfo;
	ACCOUNTFREEZEENDTIME stAccountFreezeEndTime;
	ACCOUNTFREEZEENDTIME stRes;
	TDRDBOBJECT stFetchObj;
	int iResNum;

	if (!InBody->RegionID || 0 == *InBody->RegionID)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->AccName)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->AccName");
		return 0;
	}
	

	memset(&stMetaInfo,0,sizeof(stMetaInfo));
	
	STRNCPY(stMetaInfo.szMetaName,"AccountFreezeEndTime",sizeof(stMetaInfo.szMetaName));
	stMetaInfo.iObjSize = sizeof(stAccountFreezeEndTime);
	stMetaInfo.pszObj = (char*) &stAccountFreezeEndTime;
	snprintf(stMetaInfo.szWhere,sizeof(stMetaInfo.szWhere),
			"where Pass9Account = '%s'",
			InBody->AccName);

	//iRet = bi_get_account_stat(pstEnv,
		//*InBody->RegionID,
		//pszAccNameGbk
		//);

	memset(&stAccountFreezeEndTime,0,sizeof(stAccountFreezeEndTime));
	
	memset(&stFetchObj,0,sizeof(stFetchObj));
	stFetchObj.pszObj = (char*) &stRes;
	stFetchObj.iObjSize = sizeof(stRes);

	iRet = bi_db_op(pstEnv,
		*InBody->RegionID,
		TDR_DBOP_SELECT,
		&stMetaInfo,
		&stFetchObj,
		&iResNum,
		1,
		NULL
		);

	if(iRet < 0)
	{
		OutBody->ErrorCode = pstEnv->iErrCode;
		
		BI_ERR_TEXT(OutBody->StrError,"GetAccountStat error:%d",iRet);
		return 0;
	}

	pstEnv->iTmpRes1 = stRes.Uin;	
	if (pstEnv->pstAppCtx->stCurr.tv_sec >= stRes.FreezeEndTime )
	{
		pstEnv->iTmpRes2 = 1;
	}
	else
	{
		pstEnv->iTmpRes2 = 0;
	}

	iRet = bi_sprint_xml(&OutBody->StrXML, "AccountFreezeEndTime", &stRes, sizeof(stRes));
	if(iRet < 0)
	{
		OutBody->ErrorCode = BI_ERR_XML;
		BI_ERR_TEXT(OutBody->StrError,"bi_sprint_xml error:%d",iRet);
		return 0;
	}

	OutBody->Uin = &pstEnv->iTmpRes1;
	OutBody->CanCharge = &pstEnv->iTmpRes2;

	return 0;
	UNUSED(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__ChargeAccount(struct soap* soap, 
	struct ns1__ChargeAccountReq *InBody, 
	struct ns1__ChargeAccountRes *OutBody)
{	
	BI_SERVICEENV* pstEnv = &gs_stBiEnv;
	int iRet = 0;
	TDRDBOBJECT stFetchObj;
	BiMetaInfo stMetaInfo;
	ADDBILLRECORD stAddBillRecord;
	SSCASHBILLRES stRes;
	BILL stBill;
	char* pszPropsNameGbk = NULL;
	int iFetchNum = 0;

	if (!InBody->RegionID || 0 == *InBody->RegionID)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->AccName)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->AccName");
		return 0;
	}
	
	if (!InBody->Uin)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->Uin");
		return 0;
	}

	if (!InBody->Sequence)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->Sequence");
		return 0;
	}

	if (!InBody->BillType)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->BillType");
		return 0;
	}

	if (!InBody->Points)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->Points");
		return 0;
	}

	if (!InBody->PropsID)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->PropsID");
		return 0;
	}

	if (!InBody->PropsName)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->PropsName");
		return 0;
	}
	pszPropsNameGbk = bi_get_gbk_str(InBody->PropsName);

	if (!pszPropsNameGbk)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"get_gbk_str InBody->PropsName err");
		return 0;
	}

	memset(&stMetaInfo,0,sizeof(stMetaInfo));
	
	STRNCPY(stMetaInfo.szMetaName,"AddBillRecord",sizeof(stMetaInfo.szMetaName));
	stMetaInfo.iObjSize = sizeof(stAddBillRecord);
	stMetaInfo.pszObj = (char*) &stAddBillRecord;
	stMetaInfo.iBillDB = 1;
	stMetaInfo.iFailRecord = 1;
	//snprintf(stMetaInfo.szWhere,sizeof(stMetaInfo.szWhere),"where ");

	memset(&stAddBillRecord,0,sizeof(stAddBillRecord));

	stAddBillRecord.BillType = *InBody->BillType;
	stAddBillRecord.Points = *InBody->Points;
	stAddBillRecord.PropsID = *InBody->PropsID;
	STRNCPY(stAddBillRecord.PropsName,InBody->PropsName,sizeof(stAddBillRecord.PropsName));
	STRNCPY(stAddBillRecord.Sequence,InBody->Sequence,sizeof(stAddBillRecord.Sequence));
	stAddBillRecord.ServerID = *InBody->RegionID;
	stAddBillRecord.Uin = *InBody->Uin;
	
	tdr_utctime_to_tdrdatetime (&stAddBillRecord.LogTime, pstEnv->pstAppCtx->stCurr.tv_sec);

	pstEnv->stOpLog.MetaInfo.Data.AddBillRecord = stAddBillRecord;
	pstEnv->stOpLog.MetaInfo.Type = META_DATA_TYPE_ADDBILLRECORD;
	STRNCPY(pstEnv->stOpLog.MetaInfo.Name,"AddBillRecord",
			sizeof(pstEnv->stOpLog.MetaInfo.Name));

	pstEnv->stOpLog.Uin = *InBody->Uin;
	STRNCPY(pstEnv->stOpLog.AccName,InBody->AccName,
			sizeof(pstEnv->stOpLog.AccName));
	
	

	/*
	memset(&stFetchObj,0,sizeof(stFetchObj));
	stFetchObj.pszObj = (char*) stRes.AddBillRecord;
	stFetchObj.iObjSize = sizeof(stRes.AddBillRecord[0]);
	*/

	iRet = bi_db_op(pstEnv,
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
		OutBody->ErrorCode = pstEnv->iErrCode;

		BI_ERR_TEXT(OutBody->StrError,"ChargeAccount error:%d",iRet);
		return 0;
	}

	memset(&stMetaInfo,0,sizeof(stMetaInfo));
	
	STRNCPY(stMetaInfo.szMetaName,"Bill",sizeof(stMetaInfo.szMetaName));
	stMetaInfo.iObjSize = sizeof(stBill);
	stMetaInfo.pszObj = (char*) &stBill;
	stMetaInfo.iBillDB = 1;
	stMetaInfo.iUseSql = 1;
	stMetaInfo.iFailRecord = 1;
	//snprintf(stMetaInfo.szWhere,sizeof(stMetaInfo.szWhere),"where ");

	memset(&stBill,0,sizeof(stBill));
	stBill.Uin = *InBody->Uin;
	stBill.Points = *InBody->Points;
	stBill.CashTotalPoints = stBill.Points;

	pstEnv->stOpLog.MetaInfo.Data.AddBillRecord = stAddBillRecord;
	pstEnv->stOpLog.MetaInfo.Type = META_DATA_TYPE_BILL;
	STRNCPY(pstEnv->stOpLog.MetaInfo.Name,"Bill",
			sizeof(pstEnv->stOpLog.MetaInfo.Name));

	pstEnv->iMysqlStage++;
	
	stMetaInfo.iUseSql = 
				snprintf(pstEnv->szSql,sizeof(pstEnv->szSql),
					"INSERT INTO Bill"
					" (Uin,Sequence,ServerID,Amount,Points,"
						"Bonus,FreezePoint,FreezeLastTime,CashTotalPoints,UsedTotalPoints) "
					" values(%d,0,%d,0,%d,"
							"0,0,0,%d,0) "
					"ON DUPLICATE KEY UPDATE Points=Points+%d, CashTotalPoints=CashTotalPoints+%d;",
						stBill.Uin,*InBody->RegionID,*InBody->Points,
						*InBody->Points,
						*InBody->Points,*InBody->Points);


	iRet = bi_db_op(pstEnv,
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
		OutBody->ErrorCode = pstEnv->iErrCode;
		BI_ERR_TEXT(OutBody->StrError,"ChargeAccount error:%d",iRet);
		return 0;
	}

	memset(&stRes,0,sizeof(stRes));
	/*
	iRet = bi_sprint_xml(&OutBody->StrXML, "SSCashBillRes", &stRes, sizeof(stRes));
	if(iRet < 0)
	{
		OutBody->ErrorCode = BI_ERR_XML;
		BI_ERR_TEXT(OutBody->StrError,"bi_sprint_xml error:%d",iRet);
		return 0;
	}
	*/
	/*
	if (PROPS_LIMIT_MONTH < 2)
	{
		BiMutiLog(MUTI_LOGLEVEL_ERROR,
			"PROPS_LIMIT_MONTH < 2"	);
	}
	else */
	{
		//充值礼包
		PROPSDEF astDef[PROPS_LIMIT_MONTH];
		
		memset(&stMetaInfo,0,sizeof(stMetaInfo));
	
		STRNCPY(stMetaInfo.szMetaName,"PropsDef",sizeof(stMetaInfo.szMetaName));
		stMetaInfo.iObjSize = sizeof(astDef);
		stMetaInfo.pszObj = (char*) &astDef;
		stMetaInfo.iBillDB = 1;
		//snprintf(stMetaInfo.szWhere,sizeof(stMetaInfo.szWhere),
				//"where CAST(EndDate as datetime) between DATE_SUB(now(),INTERVAL %d month) and DATE_ADD(now(),INTERVAL 1 month)",
				//PROPS_LIMIT_MONTH - 2);

		snprintf(stMetaInfo.szWhere,sizeof(stMetaInfo.szWhere),
				"where now() between CAST(BeginDate as datetime) and "
				"DATE_ADD(CAST(EndDate as datetime), INTERVAL 1 day) group by GroupID;"
				);
		//"where now() between CAST(BeginDate as datetime) and DATE_ADD(CAST(EndDate as datetime), INTERVAL 1 day) group by GroupID"


		memset(&astDef,0,sizeof(astDef));
		
		memset(&stFetchObj,0,sizeof(stFetchObj));
		stFetchObj.pszObj = (char*) &astDef;
		stFetchObj.iObjSize = sizeof(astDef);

		iRet = bi_db_op(pstEnv,
			*InBody->RegionID,
			TDR_DBOP_SELECT,
			&stMetaInfo,
			&stFetchObj,
			&iFetchNum,
			PROPS_LIMIT_MONTH,
			NULL
			);

		if(iRet < 0)
		{
			BiMutiLog(MUTI_LOGLEVEL_ERROR,
					"PropsDef select ret:%d err:%d",iRet,pstEnv->iErrCode);
		}
		else
		{
			int i = 0; 
			PROPSGIFT stGift;
			struct soap *soap2 = NULL;

			
			memset(&stGift,0,sizeof(stGift));
			
			tdr_utctime_to_tdrdatetime (&stGift.LastCashTime, pstEnv->pstAppCtx->stCurr.tv_sec);
			stGift.Uin = *InBody->Uin;
			stGift.CashNow = *InBody->Points;

			memset(&stMetaInfo,0,sizeof(stMetaInfo));
	
			STRNCPY(stMetaInfo.szMetaName,"PropsGift",sizeof(stMetaInfo.szMetaName));
			stMetaInfo.iObjSize = sizeof(stGift);
			stMetaInfo.pszObj = (char*) &stGift;
			stMetaInfo.iBillDB = 1;
			stMetaInfo.iDuplicateKeyUpdate = 1;
			//stMetaInfo.iFailRecord = 1;
			stMetaInfo.iDuplicateKeyUpdate =
				snprintf(stMetaInfo.szWhere,sizeof(stMetaInfo.szWhere),
					" ON DUPLICATE KEY UPDATE CashNow=CashNow+%d, LastCashTime=now();",
					*InBody->Points);

			/*
			pstEnv->stOpLog.MetaInfo.Data.AddBillRecord = stAddBillRecord;
			pstEnv->stOpLog.MetaInfo.Type = META_DATA_TYPE_BILL;
			STRNCPY(pstEnv->stOpLog.MetaInfo.Name,"Bill",
					sizeof(pstEnv->stOpLog.MetaInfo.Name));
			*/
	
			
			
			for (i = 0; i < iFetchNum; i++)
			{
				soap2 = soap_copy(soap);
				soap_set_namespaces(soap2, soapCsService_namespaces);
				soap2->userid = pstEnv->pstConf->HttpBaseUserID;
				soap2->passwd = pstEnv->pstConf->HttpBaseUserPasswd;
				
				stGift.GroupID = astDef[i].GroupID;

				iRet = bi_db_op(pstEnv,
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
					BiMutiLog(MUTI_LOGLEVEL_ERROR,
						"PropsGift insert ret:%d err:%d uin:%d account:%s groupid:%d points:%d",
						iRet,pstEnv->iErrCode,*InBody->Uin,InBody->AccName,stGift.GroupID,
						*InBody->Points);
				}
				else
				{
					struct soapCsService1__CashPropsGiftBillReq stReq;
					struct soapCsService1__CashPropsGiftBillRes stRes;

					stReq.AccName = InBody->AccName;
					stReq.AddCash = InBody->Points;
					stReq.GroupID = (int*)&stGift.GroupID;
					stReq.RegionID = InBody->RegionID;
					stReq.TimeOut = 0;
					stReq.Uin = InBody->Uin;
					
					iRet = soap_call___soapCsService1__CashPropsGiftBill(soap2, pstEnv->pstConf->CsServiceUrl, NULL,&stReq, &stRes);
					if (iRet)
					{
						soap_receiver_fault(soap, "Cannot connect to cs_service", NULL);
					}

					if (soap2->error)
					{ 
						pstEnv->szErrorText[0] = 0;
						soap_sprint_fault(soap2, pstEnv->szErrorText,sizeof(pstEnv->szErrorText));
						
						BiMutiLog(MUTI_LOGLEVEL_ERROR,"CashPropsGiftBill fault:%s",pstEnv->szErrorText);
						
						pstEnv->szErrorText[0] = 0;
					}
					else
					{
						if (stRes.ErrorCode)
						{
							BiMutiLog(MUTI_LOGLEVEL_ERROR,"CashPropsGiftBill code:%d err:%s",
								stRes.ErrorCode,
								stRes.StrError->ErrorText);
						}
					}
				
				}
				soap_end(soap2);
				soap_free(soap2);
			}
			//soap_set_namespaces(soap2, namespaces);
			
		}
		
	}

	return 0;
	UNUSED(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetListBill(struct soap* soap, 
	struct ns1__GetListBillReq *InBody, 
	struct ns1__GetListBillRes *OutBody)
{	
	BI_SERVICEENV* pstEnv = &gs_stBiEnv;
	int iRet = 0;
	TDRDBOBJECT stFetchObj;
	BiMetaInfo stMetaInfo;
	BILL stBill;
	LISTBILL stRes;

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->AccName)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->AccName");
		return 0;
	}


	if (!InBody->Uin)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->Uin");
		return 0;
	}

	memset(&stMetaInfo,0,sizeof(stMetaInfo));
	
	STRNCPY(stMetaInfo.szMetaName,"Bill",sizeof(stMetaInfo.szMetaName));
	stMetaInfo.iObjSize = sizeof(stBill);
	stMetaInfo.pszObj = (char*) &stBill;
	stMetaInfo.iBillDB = 1;
	//snprintf(stMetaInfo.szWhere,sizeof(stMetaInfo.szWhere),"where Uin = %d",);


	memset(&stBill,0,sizeof(stBill));
	stBill.Uin = *InBody->Uin;

	/*
	pstEnv->stOpLog.MetaInfo.Data.Bill = stBill;
	pstEnv->stOpLog.MetaInfo.Type = META_DATA_TYPE_BILL;
	STRNCPY(pstEnv->stOpLog.MetaInfo.Name,"Bill",
			sizeof(pstEnv->stOpLog.MetaInfo.Name));
	*/

	
	memset(&stFetchObj,0,sizeof(stFetchObj));
	stFetchObj.pszObj = (char*) stRes.Bill;
	stFetchObj.iObjSize = sizeof(stRes.Bill[0]);
	

	iRet = bi_db_op(pstEnv,
		*InBody->RegionID,
		TDR_DBOP_SELECT,
		&stMetaInfo,
		&stFetchObj,
		&stRes.Num,
		MAX_LIST_BILL_NUM,
		NULL
		);

	if(iRet < 0)
	{
		OutBody->ErrorCode = pstEnv->iErrCode;
		BI_ERR_TEXT(OutBody->StrError,"GetListBill error:%d",iRet);
		return 0;
	}
	
	iRet = bi_sprint_xml(&OutBody->StrXML, "ListBill", &stRes, sizeof(stRes));
	if(iRet < 0)
	{
		OutBody->ErrorCode = BI_ERR_XML;
		BI_ERR_TEXT(OutBody->StrError,"bi_sprint_xml error:%d",iRet);
		return 0;
	}

	return 0;
	UNUSED(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 __ns1__InsertAccGift(struct soap* soap, 
	struct ns1__InsertAccGiftReq *InBody, 
	struct ns1__InsertAccGiftRes *OutBody)
{	
	BI_SERVICEENV* pstEnv = &gs_stBiEnv;
	int iRet = 0;
	TDRDBOBJECT stFetchObj;
	BiMetaInfo stMetaInfo;
	ACCGIFT stAccGift;
	char* pszGiftNameGbk = NULL;
	char* pszGiftDescGbk = NULL;
	int iInsertType = 0;
	tdr_ulonglong ulRoleID = 0;
	int iUin = 0;

	if (!InBody->RegionID || 0 == *InBody->RegionID)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->AccName)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->AccName");
		return 0;
	}

	if (!InBody->Uin)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->Uin");
		return 0;
	}

	iUin = *InBody->Uin;

	
	if (!InBody->RoleID)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->RoleID");
		return 0;
	}

	ulRoleID = strtoull(InBody->RoleID,NULL,10);

	if (!InBody->InsertType)
	{
		
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->InsertType");
		return 0;
	}
	iInsertType = *InBody->InsertType;

	if (!InBody->GiftName)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->GiftName");
		return 0;
	}
	pszGiftNameGbk = bi_get_gbk_str(InBody->GiftName);

	if (!pszGiftNameGbk)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"get_gbk_str InBody->GiftName err");
		return 0;
	}

	if (!InBody->GiftDesc)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->GiftDesc");
		return 0;
	}
	pszGiftDescGbk = bi_get_gbk_str(InBody->GiftDesc);

	if (!pszGiftDescGbk)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"get_gbk_str InBody->GiftDesc err");
		return 0;
	}

	if (!InBody->GiftItemID)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->GiftItemID");
		return 0;
	}

	if (!InBody->GiftItemNum)
	{
		OutBody->ErrorCode = BI_ERR_SYS;
		BI_ERR_TEXT(OutBody->StrError,"need InBody->GiftItemNum");
		return 0;
	}

	memset(&stMetaInfo,0,sizeof(stMetaInfo));
	
	STRNCPY(stMetaInfo.szMetaName,"AccGift",sizeof(stMetaInfo.szMetaName));
	stMetaInfo.iObjSize = sizeof(stAccGift);
	stMetaInfo.pszObj = (char*) &stAccGift;
	stMetaInfo.iBillDB = 1;
	//snprintf(stMetaInfo.szWhere,sizeof(stMetaInfo.szWhere),"where ");
	
	//0:无条件插入 1:所有同名同帐号礼包都已领取才插入 2:角色没有相同的礼包(包括已领取的)才插入
	//3:帐号没有相同的礼包才插入
	if (iInsertType > 0)
	{
		int iFetchNum = 0;
		
		if (iInsertType == 1)
		{
			snprintf(stMetaInfo.szWhere,sizeof(stMetaInfo.szWhere),
				"where Uin = %d and RoleID = %llu and (GetGiftTime is NULL or GetGiftTime = 0) and Name = '%s' limit 1 ",
				iUin,ulRoleID,pszGiftNameGbk);
		}
		else if (iInsertType == 2)
		{
			snprintf(stMetaInfo.szWhere,sizeof(stMetaInfo.szWhere),
				"where Uin = %d and RoleID = %llu and Name = '%s' limit 1 ",
				iUin,ulRoleID,pszGiftNameGbk);
		}
		else if (iInsertType == 3)
		{
			snprintf(stMetaInfo.szWhere,sizeof(stMetaInfo.szWhere),
				"where Uin = %d  and Name = '%s' limit 1 ",
				iUin,pszGiftNameGbk);
		}

		
		memset(&stFetchObj,0,sizeof(stFetchObj));
		stFetchObj.pszObj = (char*) &stAccGift;
		stFetchObj.iObjSize = sizeof(stAccGift);

		

		iRet = bi_db_op(pstEnv,
			*InBody->RegionID,
			TDR_DBOP_SELECT,
			&stMetaInfo,
			&stFetchObj,
			&iFetchNum,
			1,
			NULL
			);

		if (iRet < 0)
		{
			if (pstEnv->iErrCode == BI_ERR_NOT_FOUND)
			{
				iRet = 0;
			}			
		}
		else
		{
			iRet = BI_ERR_DUP;
		}

		if(iRet < 0)
		{
			OutBody->ErrorCode = iRet;
			BI_ERR_TEXT(OutBody->StrError,"InsertAccGift error:%d",iRet);
			return 0;
		}
	}



	memset(&stAccGift,0,sizeof(stAccGift));

	stAccGift.Uin = iUin;
	stAccGift.RoleID = ulRoleID;
	tdr_utctime_to_tdrdatetime(&stAccGift.SetGiftTime, pstEnv->pstAppCtx->stCurr.tv_sec);
	STRNCPY(stAccGift.Name,pszGiftNameGbk,sizeof(stAccGift.Name));
	STRNCPY(stAccGift.GiftDesc,pszGiftDescGbk,sizeof(stAccGift.GiftDesc));
	stAccGift.GiftItems[0].ItemID = *InBody->GiftItemID;
	stAccGift.GiftItems[0].ItemNum = *InBody->GiftItemNum;

	/*
	pstEnv->stOpLog.MetaInfo.Data.AccGift = stAccGift;
	pstEnv->stOpLog.MetaInfo.Type = META_DATA_TYPE_ACCGIFT;
	STRNCPY(pstEnv->stOpLog.MetaInfo.Name,"AccGift",
			sizeof(pstEnv->stOpLog.MetaInfo.Name));
	*/

	/*
	memset(&stFetchObj,0,sizeof(stFetchObj));
	stFetchObj.pszObj = (char*) stRes.AccGift;
	stFetchObj.iObjSize = sizeof(stRes.AccGift[0]);
	*/

	iRet = bi_db_op(pstEnv,
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
		OutBody->ErrorCode = pstEnv->iErrCode;
		BI_ERR_TEXT(OutBody->StrError,"InsertAccGift error:%d",iRet);
		return 0;
	}

	OutBody->AccGiftID = &pstEnv->iLastInsertID;

	return 0;
	UNUSED(soap);
}

