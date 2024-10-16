
#include "tapp/tapp.h"
#include "tdr/tdr.h"
#include "tlog/tlog.h"
#include "cs_service_conf_desc.h"
#include "version.h"
#include "tbus/tbus.h"
#include "comm_misc.h"

#include "soapCsServiceH.h"
#include "soapCsService.nsmap"
#include <iconv.h>
#include "fcgi_config.h"
#include "comm/tconvert.h"


#include <stdlib.h>
//#include <stdsoap2.h>
//#include "cs_service.h"

extern char **environ;


#include "fcgi_stdio.h"
#include "cs_cgi.h"
#include "fcgios.h"

#ifndef WIN32
#define max(a, b)	((a) > (b) ? (a) : (b))
#define min(a, b)	((a) < (b) ? (a) : (b))
#endif


#define UNUSED( param ) ((void)(param))

SOAP_NMAC struct Namespace namespaces[] =
{
	{NULL, NULL, NULL, NULL}
};



//#define MAX_FIND_CONTEXT_NUM 20 //最大查找句柄数量
#define MAX_ERROR_LEN 128 //错误信息长度 
#define DEFAULT_TIME_OUT 5000 //默认超时时间
#define MAX_STR_XML_LEN 4096000 //最大返回xml字符串长度
//#define FIND_HANDLE_LIVE_TIME 1200 //查询句柄生存时间
#define MAX_GBK_LENG 204800 //最大GBK字符串长度
#define CS_MAX_GBK_TEXT_NUM 8 //最大GBK字符串数量

#define MAX_LOG_PATH_LENG 128 //日志目录长度
#define CS_ALLOW_OP_FLAG_LEN 32 //(1024/32) //允许的操作flag长度
#define MAX_CS_ERROR_LEN 2048 //错误信息长度 

typedef struct 
{
	char szTextGBK[MAX_GBK_LENG];
}GbkText;


/* FIND_HANDLE_TYPE*/
enum tagFIND_HANDLE_TYPE 
{
	FIND_HANDLE_TYPE_NULL = 0, 	/* 空类型 */
    FIND_HANDLE_TYPE_ACC = 1, 	/* 帐号查询句柄 */
	FIND_HANDLE_TYPE_ACC_LIMIT = 2, 	/* 区帐号查询句柄 */
	FIND_HANDLE_TYPE_ROLE = 3, /* 查询角色句柄 */
	FIND_HANDLE_TYPE_MAIL = 4, /* 查询邮件句柄 */
};

union tagFindHandle
{
    FIND_ACCOUNT_HANDLE pFindHandleAcc;	/* FIND_HANDLE_TYPE_ACC, 帐号查询句柄  */
	LIMITED_FIND_ACCOUNT_HANDLE pFindHandleAccLimit;	/* FIND_HANDLE_TYPE_ACC_LIMIT, 区帐号查询句柄  */
	FIND_ACCOUNT_ROLE_HANDLE pFindHandleRole;/* FIND_HANDLE_TYPE_ROLE,查询角色句柄*/
	FIND_MAILLIST_HANDLE pFindHandleMail;/*FIND_HANDLE_TYPE_MAIL,查询邮件句柄*/
};

typedef union  tagFindHandle                     	FINDHANDLE;

typedef struct 
{
	FINDHANDLE unFindHandle; //查询句柄
  	int iLiveTime;	//生存时间
  	int iType;	//句柄类型 FIND_HANDLE_TYPE_ACC
  	int iID;//id
}FindHandleInfo;


struct tagcs_serviceEnv
{
	CS_SERVICECONF *pstConf;
	CS_SERVICERUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	TAPPCTX *pstAppCtx;
	pthread_mutex_t stLogMutex;
	LPSSHANDLE pstSSHan;
	FindHandleInfo *pstFindHandle;
	int iFindHandleNum;
  	int iLastFindHandleID;
	struct soap soap;
	int *pCgiHandle;
	struct soapCsService1__ErrorResponse stErrorResponse;
	char szErrorText[MAX_ERROR_LEN];
	char szStrXML[MAX_STR_XML_LEN];
	char szStrXMLUtf8[MAX_STR_XML_LEN];
	int iGetFindHandleErr;
	int iTmp1;
	int iGbkTextUsedNum;
	GbkText aGbkText[CS_MAX_GBK_TEXT_NUM];
	char *pszGbk;
	char *pszGbk2;
	char szLogPath[MAX_LOG_PATH_LENG];
	struct sigaction stTappAct;
	int iClientIP; 
	int iClientRestrictLevel;
	int iRestrictIPNum;
	int aiRestrictIP[RESTRICT_IP_NUM];
	int aiAllowOpFlag[CS_ALLOW_OP_FLAG_LEN];
	int aiAllowOpInitFlag[CS_ALLOW_OP_FLAG_LEN];
	int iServerIP;
	char szLastErrLog[MAX_CS_ERROR_LEN];
};

typedef struct tagcs_serviceEnv		CS_SERVICEENV;
typedef struct tagcs_serviceEnv		*LCS_SERVICEENV;

static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_cs_service[];
extern unsigned char g_szMetalib_proto_ss[];


CS_SERVICEENV gs_stCsEnv;
static CS_SERVICERUN_STATUS gs_stStat;

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

#define MutiLog(iLevel, szFormat, args...) \
		do{\
			switch(iLevel)\
			{\
			case MUTI_LOGLEVEL_TRACE:\
				tlog_trace(gs_stCsEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_DEBUG:\
				tlog_debug(gs_stCsEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_INFO:\
				tlog_info(gs_stCsEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_WARN:\
				tlog_warn(gs_stCsEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_ERROR:\
				tlog_error(gs_stCsEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_SEVERE:\
				tlog_fatal(gs_stCsEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			default:\
				break;\
			}\
		  }while(0)

#define CS_ERR_TEXT(pstErrRes, szFormat, args...) \
		do{\
			if (!pstErrRes)\
			{\
				pstErrRes = &gs_stCsEnv.stErrorResponse;\
			}\
			if (!pstErrRes->ErrorText)\
			{\
				pstErrRes->ErrorText = gs_stCsEnv.szErrorText;\
			}\
			if (pstErrRes->ErrorText)\
			{\
				snprintf(pstErrRes->ErrorText,MAX_ERROR_LEN,szFormat,##args);\
			}\
		  }while(0);	

int cs_service_init(TAPPCTX *pstAppCtx, CS_SERVICEENV *pstEnv);
int cs_service_fini(TAPPCTX *pstAppCtx, CS_SERVICEENV *pstEnv);
int c_close_handle(FindHandleInfo * pstInfo);
int c_get_log_path(CS_SERVICEENV *pstEnv)
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

int c_parse_args(int argc, char **argv)
{
   int c;
   //int digit_optind = 0;

   while (1) {
	   //int this_option_optind = optind ? optind : 1;
	   int option_index = 0;
	   static struct option long_options[] = {
		   {"print-spawn-args", 1, 0, 1},
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
					fprintf(fp,"-a%s -p%d",gs_stCsEnv.pstConf->SpawnIP,
						gs_stCsEnv.pstConf->SpawnPort);
					
					fclose(fp); 
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

int cs_service_proc(TAPPCTX *pstAppCtx, CS_SERVICEENV *pstEnv)
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
		if (soapCsService_serve_request(soap) || (soap->fserveloop && soap->fserveloop(soap)))
		{
#ifdef WITH_FASTCGI
			soap_send_fault(soap);

#else
			//return soap_send_fault(soap);
			soap_send_fault(soap);
#endif
		}

#ifdef WITH_FASTCGI
		{
		        char **env = environ;

		        while (*env)
		        {
		                //printf("%s\n",*env);
						MutiLog(MUTI_LOGLEVEL_DEBUG, "fcgi env:%s", *env);
		                env++;
		        }
		}

		//soap_destroy(soap);
		//soap_end(soap);
	//} while (1);
#else
	//} while (soap->keep_alive);
#endif
	//return SOAP_OK;

final:	
	{
		char **env = environ;

		while (*env)
		{
				//printf("%s\n",*env);
				MutiLog(MUTI_LOGLEVEL_DEBUG, "fcgi env:%s", *env);
				env++;
		}
	}

	soap_destroy(soap);
	soap_end(soap);

	if (iBusy)
	{
		return 0;
	}
	else
	{
		return -1;
	}
	
err:
	MutiLog(MUTI_LOGLEVEL_ERROR, "soap err:%d", soap->error);
	goto final;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int cs_service_reload(TAPPCTX *pstAppCtx, CS_SERVICEENV *pstEnv)
{
	CS_SERVICECONF   *pstPreConf;
	
	pstPreConf = (CS_SERVICECONF   *)pstAppCtx->stConfPrepareData.pszBuff;
	
	printf("cs_service reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "cs_service reload");

	return 0;
}

int cs_service_tick(TAPPCTX *pstAppCtx, CS_SERVICEENV *pstEnv)
{
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}

int cs_service_stop(TAPPCTX *pstAppCtx, CS_SERVICEENV *pstEnv)
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
	void* pvArg	=	&gs_stCsEnv;

	memset(&gs_stAppCtx, 0, sizeof(gs_stAppCtx));
	memset(&gs_stCsEnv, 0, sizeof(gs_stCsEnv));

	gs_stAppCtx.argc	=	argc;
	gs_stAppCtx.argv	=	argv;

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)cs_service_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_cs_service;
	gs_stAppCtx.stConfData.pszMetaName = "cs_serviceconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);
		
	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	c_parse_args(argc,argv);

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)cs_service_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)cs_service_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)cs_service_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)cs_service_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)cs_service_stop;
	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	tapp_def_fini(&gs_stAppCtx, pvArg);
	

	
	return 0;
} 

int cs_gbk_to_utf8_in(char ** ppIn,size_t * ppInLen,char** ppOut,size_t * ppOutLen)
{
	int iConvRet = 0;

	iconv_t cd ;
	
	//iconv_t cd = iconv_open("gbk","utf-8");
	cd = iconv_open("utf-8//IGNORE",gs_stCsEnv.pstConf->ConvertEncoding);

	iConvRet = iconv(cd, ppIn, ppInLen,ppOut, ppOutLen);
	iconv_close(cd);

	return iConvRet;
}

int cs_get_gbk_str_in(char ** ppIn,size_t * ppInLen,char** ppOut,size_t * ppOutLen)
{
	int iConvRet = 0;
	iconv_t cd ;
	
	//iconv_t cd = iconv_open("gbk","utf-8");
	//if (0 != strnicmp(gs_stCsEnv.pstConf->ConvertEncoding,"utf",3 ))
	{
		cd = iconv_open(gs_stCsEnv.pstConf->ConvertEncoding,"utf-8");

		iConvRet = iconv(cd, ppIn, ppInLen,ppOut, ppOutLen);
		iconv_close(cd);
	}
	//else
	//{
	//	iConvRet = STRNCPY(*ppOut,*ppIn, *ppInLen);
	//}

	return iConvRet;
}


char* cs_get_gbk_str(char * pszUTF8)
{
	size_t sInLen = strlen(pszUTF8);
	size_t sOutLen = sizeof(GbkText);
	char *pIn = pszUTF8;
	char *pOut = NULL;
	int iConvRet = 0;
	GbkText *pstText = NULL;
	CS_SERVICEENV *pstEnv = &gs_stCsEnv;

	if (pstEnv->iGbkTextUsedNum < 0 ||
		pstEnv->iGbkTextUsedNum >= CS_MAX_GBK_TEXT_NUM)
	{
		return NULL;
	}
	
	pstText = &pstEnv->aGbkText[pstEnv->iGbkTextUsedNum];
	pstEnv->iGbkTextUsedNum++;
	
	pOut = pstText->szTextGBK;	

	memset(pstText,0,sizeof(*pstText));
	
	iConvRet =cs_get_gbk_str_in(&pIn, &sInLen,&pOut, &sOutLen);
	
	if (iConvRet < 0)
	{			
		return NULL;
	}

	return pstText->szTextGBK;
}


int c_sprint_xml(
	char **ppStrXml,
	const char *meta_name,
	const void *data,
	int data_size)
{
	int iRet = 0;
	int iLen = 0;

	memset(gs_stCsEnv.szStrXML,0,sizeof(gs_stCsEnv.szStrXML));
	//gs_stCsEnv.szStrXML[0] = 0;
	
	*ppStrXml = gs_stCsEnv.szStrXML;

	iRet = cs_sprint_xml(gs_stCsEnv.pCgiHandle,meta_name,data,data_size,
		gs_stCsEnv.szStrXML,sizeof(gs_stCsEnv.szStrXML));

	iLen = strlen(gs_stCsEnv.szStrXML);
	
	iLen = min(iLen,MAX_STR_XML_LEN -1);
	if (iLen >= 0 && iLen <MAX_STR_XML_LEN)
	{
		gs_stCsEnv.szStrXML[iLen] = 0;
	}

	// &&
	//	(0 != strnicmp(gs_stCsEnv.pstConf->ConvertEncoding,"utf",3 ))
	if (0 == iRet && gs_stCsEnv.pstConf->SoapEncoding == SOAP_ENCODING_TYPE_UTF8)
	{
		size_t sInLen = strlen(gs_stCsEnv.szStrXML);
		size_t sOutLen = sizeof(gs_stCsEnv.szStrXMLUtf8);
		char *pIn = gs_stCsEnv.szStrXML;
		char *pOut = gs_stCsEnv.szStrXMLUtf8;
		int iConvRet = 0;
		//iconv_t cd = 0;

		memset(gs_stCsEnv.szStrXMLUtf8,0,sizeof(gs_stCsEnv.szStrXMLUtf8));
		//gs_stCsEnv.szStrXMLUtf8[0] = 0;
		
		
/*
		cd = iconv_open("utf-8","gbk");

		iConvRet = iconv(cd, &pIn, &sInLen,&pOut, &sOutLen);

*/
		iConvRet = cs_gbk_to_utf8_in(&pIn, &sInLen,&pOut, &sOutLen);
		if (iConvRet < 0)
		{
			*ppStrXml = NULL;
			iRet = -2;
		}
		else
		{
			*ppStrXml = gs_stCsEnv.szStrXMLUtf8;
			iLen = strlen(gs_stCsEnv.szStrXMLUtf8);
	
			iLen = min(iLen,MAX_STR_XML_LEN -1);
			if (iLen >= 0 && iLen <MAX_STR_XML_LEN)
			{
				gs_stCsEnv.szStrXMLUtf8[iLen] = 0;
			}
		}
		
		//printf("inlen = %d,outlen = %d\n\n", sInLen,sOutLen);
		//printf("locale = %s\n\n", setlocale(LC_CTYPE,""));
		//printf("result = %s\n\n", szRet);
		//iconv_close(cd);
	}

	return iRet;
}


int cs_service_fini(TAPPCTX *pstAppCtx, CS_SERVICEENV *pstEnv)
{
	if (pstEnv->pstFindHandle)
	{
		int i = 0;
		for (i = 0 ; i < pstEnv->iFindHandleNum; i++)
		{			
			FindHandleInfo * pstInfo = &pstEnv->pstFindHandle[i];
			pstInfo->iID = i;
			
			if (pstInfo->unFindHandle.pFindHandleAcc)
			{
				c_close_handle(pstInfo);
				pstInfo->unFindHandle.pFindHandleAcc = NULL;
			}
		}
	}

	if (pstEnv->pstFindHandle)
	{
		free(pstEnv->pstFindHandle);
	}
	
	pstEnv->pstFindHandle = NULL;

	soap_end(&gs_stCsEnv.soap);
	
	printf("service_svr stop\n");
	MutiLog(MUTI_LOGLEVEL_INFO, "service_svr stop");
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

void cs_on_sigusr1(int iSig)
{
	CS_SERVICEENV *pstEnv = &gs_stCsEnv;
		
	OS_ShutdownPending();
	sigaction(SIGUSR1, &pstEnv->stTappAct,NULL);
	kill(getpid(),iSig);
}


int cs_check_client_restrict_op(struct soap* soap,int iType,char *pszTag)
{
	int i = 0;
	int k = 0;

	CS_SERVICEENV* pstEnv = &gs_stCsEnv;
	
	if (!pstEnv->iClientIP || !pszTag)
	{
		return 0;
	}

	if (pstEnv->iClientIP == pstEnv->iServerIP)
	{
		return 0;
	}

	for (i = 0; i < RESTRICT_IP_NUM; i++)
	{
		if (i >= pstEnv->iRestrictIPNum)
		{
			return 0;
		}
		if (pstEnv->iClientIP == pstEnv->aiRestrictIP[i])
		{
			int iIdx = 0;
			int iPos = 0;


			iIdx = iType / CS_ALLOW_OP_FLAG_LEN;
			iPos = iType % 32;

			if (iIdx >= CS_ALLOW_OP_FLAG_LEN)
			{
				MutiLog(MUTI_LOGLEVEL_ERROR,
						"op type(%d) out range(%d)",
						iType,
						CS_ALLOW_OP_FLAG_LEN*32);
				
				return -1;
			}
			else
			{
				int iAllow = 0;
				int iInit = 0;
				iInit = pstEnv->aiAllowOpInitFlag[iIdx] & (1 << iPos);
				if (!iInit)
				{
					for (k = 0; k < pstEnv->pstConf->RestrictInfo.AllowOperationNum;k++)
					{
						if (0== strcmp(pszTag,
								pstEnv->pstConf->RestrictInfo.AllowOperation[k].Name))
						{
							pstEnv->aiAllowOpFlag[iIdx] |= (1 << iPos);
							break;
						}
					}
					pstEnv->aiAllowOpInitFlag[iIdx] |= (1 << iPos);
				}
				
				iAllow = pstEnv->aiAllowOpFlag[iIdx] & (1 << iPos);
				
				if (iAllow)
				{
					return 0;
				}
				else
				{
					char * pszRemoteAddr = getenv("REMOTE_ADDR");
					if (pszRemoteAddr)
					{
						MutiLog(MUTI_LOGLEVEL_ERROR,
							"invalid client request ip:%s req:%s",
							pszRemoteAddr,
							pszTag);
					}
					else
					{
						MutiLog(MUTI_LOGLEVEL_ERROR,
							"invalid client request ip:%s req:%s",
							"",
							pszTag);
					}
					
					return -1;
				}
				
			}
			break;
		}
		
	}
	
	
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}


int cs_soap_prepare_init_recv(struct soap* soap)
{
	CS_SERVICEENV* pstEnv = &gs_stCsEnv;
	char *pszIP = NULL;
	char *pszServerIP = NULL;
	
	pszIP = getenv("REMOTE_ADDR");
	pszServerIP = getenv("SERVER_ADDR");
	if (pszIP)
	{
		pstEnv->iClientIP = (int)inet_network(pszIP);
	}
	else
	{
		pstEnv->iClientIP = 0;
	}

	if (pszServerIP)
	{
		pstEnv->iServerIP = (int)inet_network(pszServerIP);
	}
	else
	{
		pstEnv->iServerIP = 0;
	}

	
	pstEnv->iGbkTextUsedNum = 0;
	
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

int cs_restrict_init(CS_SERVICEENV *pstEnv)
{
	int i = 0;
	for (i = 0;i < pstEnv->pstConf->RestrictInfo.RestrictIpNum;i++)
	{
		pstEnv->aiRestrictIP[i] = (int)inet_network(pstEnv->pstConf->RestrictInfo.RestrictIP[i]); 
	}

	pstEnv->iRestrictIPNum = i;

	return 0;
}

int cs_service_init(TAPPCTX *pstAppCtx, CS_SERVICEENV *pstEnv)
{
	char szLogFile[MAX_LOG_PATH_LENG];
		

	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		return -1;
	}
	pstEnv->pstConf = (CS_SERVICECONF *)pstAppCtx->stConfData.pszBuff;

	pstEnv->pstRunStat = (CS_SERVICERUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
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

	if(init_interface(pstEnv->pstConf->TdrFile, &pstEnv->pCgiHandle,
		pstEnv->pstConf->CgiConfigFile))
	{
		printf("CGI init_interface 初始化失败!\n");
		return -1;
	}

	pstEnv->iFindHandleNum = pstEnv->pstConf->FindHandleNum;
	pstEnv->pstFindHandle = NULL;
	if (pstEnv->iFindHandleNum > 0)
	{
		pstEnv->pstFindHandle = calloc(pstEnv->iFindHandleNum,
			sizeof(*pstEnv->pstFindHandle));
	}

	setenv("FCGI_WEB_SERVER_ADDRS",pstEnv->pstConf->FcgiWebServerIPList,1);

	srand((unsigned)time( NULL ));

	if (NULL == cs_get_gbk_str("abc"))
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "cs_get_gbk_str fail");
		return -1;
	}

	soap_init1(&gs_stCsEnv.soap,SOAP_C_UTFSTRING|SOAP_XML_IGNORENS);
	soap_set_namespaces(&gs_stCsEnv.soap, soapCsService_namespaces);
	
	c_get_log_path(&gs_stCsEnv);
	snprintf(szLogFile,sizeof(szLogFile),"%s%s%s",
		gs_stCsEnv.szLogPath,
		gs_stCsEnv.pstAppCtx->pszApp,
		"_soap_test.log");
	#ifdef SOAP_DEBUG
	soap_set_test_logfile(&gs_stCsEnv.soap,szLogFile );
#endif

	snprintf(szLogFile,sizeof(szLogFile),"%s%s%s",
		gs_stCsEnv.szLogPath,
		gs_stCsEnv.pstAppCtx->pszApp,
		"_soap_sent.log");

#ifdef SOAP_DEBUG
	soap_set_sent_logfile(&gs_stCsEnv.soap, szLogFile);
#endif

	snprintf(szLogFile,sizeof(szLogFile),"%s%s%s",
		gs_stCsEnv.szLogPath,
		gs_stCsEnv.pstAppCtx->pszApp,
		"_soap_recv.log");

#ifdef SOAP_DEBUG
	soap_set_recv_logfile(&gs_stCsEnv.soap, szLogFile);
#endif

	pstEnv->soap.fprepareinitrecv = cs_soap_prepare_init_recv;

	cs_restrict_init(pstEnv);
  
	//soap_init1(&gs_stCsEnv.soap,SOAP_C_MBSTRING|SOAP_XML_IGNORENS);
	//SOAP_ENC_LATIN
	//soap_init1(&gs_stCsEnv.soap,SOAP_ENC_LATIN|SOAP_XML_IGNORENS);
	//gs_stCsEnv.soap.http_content = "text/xml; charset=gbk";
	//gs_stCsEnv.soap.prolog = "<?xml version=\"1.0\" encoding=\"GBK\"?>\n";

	{
		struct sigaction stAct;
		memset(&stAct, 0, sizeof(stAct));

		stAct.sa_handler	=	cs_on_sigusr1;
		sigaction(SIGUSR1, &stAct, &pstEnv->stTappAct);
	}
	
	printf("cs_service start\n");
	MutiLog(MUTI_LOGLEVEL_INFO, "cs_service start");

	return 0;
}	

int c_close_handle(FindHandleInfo * pstInfo)
{
/*	switch (pstInfo->iType)
	{
	case FIND_HANDLE_TYPE_ACC:
		
		break;
	defualt:
		break;
	}
	*/
	if (pstInfo->unFindHandle.pFindHandleAcc)
	{
		cs_close_handle(pstInfo->unFindHandle.pFindHandleAcc);		
	}
	
	
	pstInfo->unFindHandle.pFindHandleAcc = NULL;
	pstInfo->iLiveTime = 0;
	pstInfo->iType = 0;
	pstInfo->iID = 0;
	
	return 0;
}

int c_find_handle_info_refresh(CS_SERVICEENV *pstEnv,FindHandleInfo* pstInfo)
{
	pstInfo->iLiveTime = pstEnv->pstAppCtx->stCurr.tv_sec + 
						pstEnv->pstConf->FindHandleTimeout;
	return 0;
}


FindHandleInfo* c_get_find_handle_info(CS_SERVICEENV *pstEnv,
	int* piFindHandleID,
	int iType /*句柄类型 FIND_HANDLE_TYPE_ACC*/)
{
	int iFindHandleID = -1;
	FindHandleInfo* pstInfo = NULL;
	
	if (!piFindHandleID)
	{
		pstEnv->iGetFindHandleErr = -1;
		return NULL;
	}
	
	iFindHandleID = *piFindHandleID;

	if (iFindHandleID < 0 || iFindHandleID >= pstEnv->iFindHandleNum)
	{	
		pstEnv->iGetFindHandleErr = -2;
		return NULL;	
	}

	pstInfo = &gs_stCsEnv.pstFindHandle[iFindHandleID];

	if (gs_stCsEnv.pstAppCtx->stCurr.tv_sec > 
		pstInfo->iLiveTime)
	{
		pstEnv->iGetFindHandleErr = -3;
		return NULL;			
	}

	if (FIND_HANDLE_TYPE_NULL != iType && pstInfo->iType != iType)
	{
		pstEnv->iGetFindHandleErr = -4;
		return NULL;	
	}

	return pstInfo;
}

int cs_gbk_to_utf8(char **ppStrXml,char* pszIn,int iInLen)
{
	size_t sInLen = iInLen;
	size_t sOutLen = sizeof(gs_stCsEnv.szStrXMLUtf8);
	char *pIn = pszIn;
	char *pOut = gs_stCsEnv.szStrXMLUtf8;
	int iConvRet = 0;
	//iconv_t cd = 0;	
	int iLen = 0;
	int iRet = 0;

	//if(0 == strnicmp(gs_stCsEnv.pstConf->ConvertEncoding,"utf",3 ))
	//{
	//	*ppStrXml  = pszIn;
	//	return iRet;
	//}
	//else
	{

		//memset(gs_stCsEnv.szStrXMLUtf8,0,sizeof(gs_stCsEnv.szStrXMLUtf8));
		gs_stCsEnv.szStrXMLUtf8[0] = 0;
		
		
		//cd = iconv_open("utf-8//IGNORE","gbk");
		//cd = iconv_open("utf-8","GB18030");
		//cd = iconv_open("utf-8",gs_stCsEnv.pstConf->ConvertEncoding);
		iConvRet = cs_gbk_to_utf8_in(&pIn, &sInLen,&pOut, &sOutLen);
		

		//iConvRet = iconv(cd, &pIn, &sInLen,&pOut, &sOutLen);
		if (iConvRet < 0)
		{
			*ppStrXml = NULL;
			iRet = -2;
		}
		else
		{
			*ppStrXml = gs_stCsEnv.szStrXMLUtf8;
			iLen = sizeof(gs_stCsEnv.szStrXMLUtf8) - sOutLen;

			iLen = min(iLen,MAX_STR_XML_LEN -1);
			if (iLen >= 0 && iLen <MAX_STR_XML_LEN)
			{
				gs_stCsEnv.szStrXMLUtf8[iLen] = 0;
			}
		}
	}
	
	//printf("inlen = %d,outlen = %d\n\n", sInLen,sOutLen);
	//printf("locale = %s\n\n", setlocale(LC_CTYPE,""));
	//printf("result = %s\n\n", szRet);
	//iconv_close(cd);
	return iRet;
}

FindHandleInfo* c_get_free_find_handle_info(CS_SERVICEENV *pstEnv,
	int iType /*句柄类型 FIND_HANDLE_TYPE_ACC*/ )
{
	int i = 0;
	if (!pstEnv->pstFindHandle)
	{
		return NULL;
	}
	
	for (i = 0 ; i < pstEnv->iFindHandleNum; i++)
	{
		FindHandleInfo * pstInfo = &pstEnv->pstFindHandle[i];
		pstInfo->iID = i;
		
		if (!pstInfo->unFindHandle.pFindHandleAcc)
		{
			pstInfo->iType = iType;
			return pstInfo;
		}

		if (pstInfo->iLiveTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			c_close_handle(pstInfo);
			pstInfo->iID = i;
			pstInfo->iType = iType;
			return pstInfo;
		}
	}
	return NULL;
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__FindAccountByNameFirst(struct soap* soap, 
	struct soapCsService1__FindAccountByNameFirstReq *InBody, 
	struct soapCsService1__FindAccountByNameFirstRes *OutBody)
{
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ACCFINDRES stRes;
	
	FindHandleInfo* pstInfo = NULL;

	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__FindAccountByNameFirstReq,"FindAccountByNameFirstReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}	

	pstInfo = c_get_free_find_handle_info(&gs_stCsEnv,FIND_HANDLE_TYPE_ACC);

	if (!pstInfo)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"no free find handle");
		return 0;
	}

	if (!InBody->AccName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->AccName");
		return 0;
	}

	gs_stCsEnv.pszGbk = cs_get_gbk_str(InBody->AccName);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}

	

	OutBody->FindHandleID = &pstInfo->iID;


	iRet = cs_find_first_account_by_loginname(gs_stCsEnv.pCgiHandle,
		gs_stCsEnv.pszGbk,
		&pstInfo->unFindHandle.pFindHandleAcc,&stRes, iTimeOut);
	
	if(iRet < 0)		
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}

	c_find_handle_info_refresh(&gs_stCsEnv,pstInfo);

	c_sprint_xml(&OutBody->StrXML,"AccFindRes", &stRes, sizeof(stRes));
	
	
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__FindHandleClose(struct soap* soap, 
	struct soapCsService1__FindHandleCloseReq *InBody, 
	struct soapCsService1__FindHandleCloseRes *OutBody)
{	
	FindHandleInfo * pstInfo = NULL;

	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__FindHandleCloseReq,"FindHandleCloseReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}

	if (!InBody->FindHandleID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->FindHandleID");
		return 0;
	}

	pstInfo = c_get_find_handle_info(&gs_stCsEnv,InBody->FindHandleID,
		FIND_HANDLE_TYPE_NULL);

	if (!pstInfo)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"c_get_find_handle_info err:%d",
			gs_stCsEnv.iGetFindHandleErr);
		return 0;		
	}

	
	c_close_handle(pstInfo);
	
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__FindAccountByRoleFirst(struct soap* soap, 
	struct soapCsService1__FindAccountByRoleFirstReq *InBody, 
	struct soapCsService1__FindAccountByRoleFirstRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ACCFINDRES stRes;
	FindHandleInfo* pstInfo = NULL;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__FindAccountByRoleFirstReq,"FindAccountByRoleFirstReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}	

	pstInfo = c_get_free_find_handle_info(&gs_stCsEnv,FIND_HANDLE_TYPE_ACC);

	if (!pstInfo)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"no free find handle");
		return 0;
	}

	if (!InBody->RoleName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RoleName");
		return 0;
	}

	gs_stCsEnv.pszGbk = cs_get_gbk_str(InBody->RoleName);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"get_gbk_str err");
		return 0;
	}
	
	iRet = cs_find_first_account_by_rolename(gs_stCsEnv.pCgiHandle, 
		gs_stCsEnv.pszGbk,
		&pstInfo->unFindHandle.pFindHandleAcc, &stRes, iTimeOut);
	
	if(iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}

	c_find_handle_info_refresh(&gs_stCsEnv,pstInfo);

	OutBody->FindHandleID = &pstInfo->iID;
	
	
	
	
	iRet = c_sprint_xml(&OutBody->StrXML, "AccFindRes", &stRes, sizeof(stRes));
			

	if (iRet < 0)
	{
		OutBody->ErrorCode =CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}
	
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__FindAccountByRoleNext(struct soap* soap, 
	struct soapCsService1__FindAccountByRoleNextReq *InBody, 
	struct soapCsService1__FindAccountByRoleNextRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ACCFINDRES stRes;
	FindHandleInfo* pstInfo = NULL;

	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__FindAccountByRoleNextReq,"FindAccountByRoleNextReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}
	
	if (!InBody->FindHandleID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->FindHandleID");
		return 0;
	}

	pstInfo = c_get_find_handle_info(&gs_stCsEnv,InBody->FindHandleID,
		FIND_HANDLE_TYPE_ACC);
	
	if (!pstInfo)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"c_get_find_handle_info err:%d",
			gs_stCsEnv.iGetFindHandleErr);
		return 0;		
	}

	
	iRet = cs_find_next_account(pstInfo->unFindHandle.pFindHandleAcc,
				&stRes);
	
	if(iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}

	
	
	
	iRet = c_sprint_xml(&OutBody->StrXML, "AccFindRes", &stRes, sizeof(stRes));
			

	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}	
	
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__GetRegionWorldInfo(struct soap* soap, 
	struct soapCsService1__GetRegionWorldInfoReq *InBody, 
	struct soapCsService1__GetRegionWorldInfoRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	REGIONWORLDINFO stRes;

	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__GetRegionWorldInfoReq,"GetRegionWorldInfoReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}
	
	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}
	
	iRet = cs_get_regionworld_info(gs_stCsEnv.pCgiHandle,&stRes);
	
	if(iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}

	
	//OutBody->StrXML = gs_stCsEnv.szStrXMLUtf8;
	
	iRet = c_sprint_xml(&OutBody->StrXML, "RegionWorldInfo", &stRes, sizeof(stRes));
			
/*
	{
		size_t sInLen = strlen(gs_stCsEnv.szStrXML);
		size_t sOutLen = sizeof(gs_stCsEnv.szStrXMLUtf8);
		char *pIn = gs_stCsEnv.szStrXML;
		char *pOut = gs_stCsEnv.szStrXMLUtf8;
		
		iconv_t cd = iconv_open("utf-8","gbk");

		iconv(cd, &pIn, &sInLen,&pOut, &sOutLen);
		//printf("inlen = %d,outlen = %d\n\n", sInLen,sOutLen);
		//printf("locale = %s\n\n", setlocale(LC_CTYPE,""));
		//printf("result = %s\n\n", szRet);
		iconv_close(cd);
	}*/
	

	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}	
		
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__FindRegionAccountByNameRoleFirst(struct soap* soap, 
	struct soapCsService1__FindRegionAccountByNameRoleFirstReq *InBody, 
	struct soapCsService1__FindRegionAccountByNameRoleFirstRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ACCFINDRES stRes;
	FindHandleInfo* pstInfo = NULL;

	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__FindRegionAccountByNameRoleFirstReq,"FindRegionAccountByNameRoleFirstReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->AccName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->AccName");
		return 0;
	}

	gs_stCsEnv.pszGbk = cs_get_gbk_str(InBody->AccName);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}	
	
	pstInfo = c_get_free_find_handle_info(&gs_stCsEnv,FIND_HANDLE_TYPE_ACC_LIMIT);

	if (!pstInfo)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"no free find handle");
		return 0;
	}	

	iRet = cs_limited_find_first_account_by_loginname(gs_stCsEnv.pCgiHandle, 
		*InBody->RegionID,
		gs_stCsEnv.pszGbk,
		&pstInfo->unFindHandle.pFindHandleAccLimit, &stRes, iTimeOut);

	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}

	
	
	iRet = c_sprint_xml(&OutBody->StrXML, "AccFindRes", &stRes, sizeof(stRes));
			

	c_find_handle_info_refresh(&gs_stCsEnv, pstInfo);

	OutBody->FindHandleID = &pstInfo->iID;

	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}
	
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__FindRegionAccountByNameRoleNext(struct soap* soap, 
	struct soapCsService1__FindRegionAccountByNameRoleNextReq *InBody, 
	struct soapCsService1__FindRegionAccountByNameRoleNextRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ACCFINDRES stRes;
	FindHandleInfo* pstInfo = NULL;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__FindRegionAccountByNameRoleNextReq,"FindRegionAccountByNameRoleNextReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->FindHandleID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->FindHandleID");
		return 0;
	}

	pstInfo = c_get_find_handle_info(&gs_stCsEnv,InBody->FindHandleID, FIND_HANDLE_TYPE_ACC_LIMIT);
	if (!pstInfo)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"c_get_find_handle_info err:%d",
			gs_stCsEnv.iGetFindHandleErr);
		return 0;		
	}
	
	
	iRet = cs_limited_find_next_account( 
		pstInfo->unFindHandle.pFindHandleAccLimit, &stRes);

	if(iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}
	
	
	
	iRet = c_sprint_xml(&OutBody->StrXML, "AccFindRes", &stRes, sizeof(stRes));
			

	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__KickOffByLoginName(struct soap* soap, 
	struct soapCsService1__KickOffByLoginNameReq *InBody, 
	struct soapCsService1__KickOffByLoginNameRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__KickOffByLoginNameReq,"KickOffByLoginNameReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->AccName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->AccName");
		return 0;
	}	

	gs_stCsEnv.pszGbk = cs_get_gbk_str(InBody->AccName);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}

	iRet = cs_kick_off_by_loginname(gs_stCsEnv.pCgiHandle,
		*InBody->RegionID,
		gs_stCsEnv.pszGbk,iTimeOut);
	
	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}
	
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__SetAccountFreeze(struct soap* soap, 
	struct soapCsService1__SetAccountFreezeReq *InBody, 
	struct soapCsService1__SetAccountFreezeRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__SetAccountFreezeReq,"SetAccountFreezeReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}	

	if (!InBody->AccName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->AccName");
		return 0;
	}	

	if (!InBody->Enable)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->Enable");
		return 0;
	}	

	if (!InBody->Time)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->Time");
		return 0;
	}	

	if (!InBody->Reason)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->Reason");
		return 0;
	}	
	
	gs_stCsEnv.pszGbk = cs_get_gbk_str(InBody->AccName);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}

	gs_stCsEnv.pszGbk2 = cs_get_gbk_str(InBody->Reason);

	if (!gs_stCsEnv.pszGbk2)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}

	iRet = cs_set_account_freeze(gs_stCsEnv.pCgiHandle, 
		*InBody->RegionID,
		gs_stCsEnv.pszGbk,
		*InBody->Enable,
		*InBody->Time,
		gs_stCsEnv.pszGbk2,
		iTimeOut);
	
	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}
	
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__SetAccountSilence(struct soap* soap, 
	struct soapCsService1__SetAccountSilenceReq *InBody, 
	struct soapCsService1__SetAccountSilenceRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__SetAccountSilenceReq,"SetAccountSilenceReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}	

	if (!InBody->AccName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->AccName");
		return 0;
	}	

	if (!InBody->Enable)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->Enable");
		return 0;
	}	

	if (!InBody->Time)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->Time");
		return 0;
	}		

	gs_stCsEnv.pszGbk = cs_get_gbk_str(InBody->AccName);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}

	iRet = cs_set_account_silence(gs_stCsEnv.pCgiHandle, 
		*InBody->RegionID,
		gs_stCsEnv.pszGbk,
		*InBody->Enable,
		*InBody->Time,
		iTimeOut);
	
	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}
	
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__SetAccSafelock(struct soap* soap, 
	struct soapCsService1__SetAccSafelockReq *InBody, 
	struct soapCsService1__SetAccSafelockRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	SETACCSAFELOCKRES stRes;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__SetAccSafelockReq,"SetAccSafelockReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}	

	if (!InBody->AccName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->AccName");
		return 0;
	}	

	if (!InBody->SafePwd)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->SafePwd");
		return 0;
	}	

	gs_stCsEnv.pszGbk = cs_get_gbk_str(InBody->AccName);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}

	gs_stCsEnv.pszGbk2 = cs_get_gbk_str(InBody->SafePwd);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}
	
	iRet = cs_set_acc_safelock(gs_stCsEnv.pCgiHandle, 
		*InBody->RegionID,
		gs_stCsEnv.pszGbk,
		gs_stCsEnv.pszGbk2,
		&stRes,
		iTimeOut);
	
	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}

	
	
	
	iRet = c_sprint_xml(&OutBody->StrXML, "SetAccSafeLockRes", &stRes, sizeof(stRes));
			

	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}


SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__SetSecondPwd(struct soap* soap, 
	struct soapCsService1__SetSecondPwdReq *InBody, 
	struct soapCsService1__SetSecondPwdRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__SetSecondPwdReq,"SetSecondPwdReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}	

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}	
		
	if (!InBody->RoleName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RoleName");
		return 0;
	}	

	//if (!InBody->SecondPassWd)
	//{
	//	OutBody->ErrorCode = CS_ERR_SYS;
	//	CS_ERR_TEXT(OutBody->StrError,"need InBody->SecondPassWd");
	//	return 0;
	//}	

	gs_stCsEnv.pszGbk = cs_get_gbk_str(InBody->RoleName);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}

	gs_stCsEnv.pszGbk2 = cs_get_gbk_str(InBody->SecondPassWd);

	//if (!gs_stCsEnv.pszGbk)
	//{
	//	OutBody->ErrorCode = CS_ERR_SYS;
	//	CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
	//	return 0;
	//}
	
	iRet = cs_set_second_pwd(gs_stCsEnv.pCgiHandle, 
		*InBody->RegionID,
		*InBody->WorldID,
		gs_stCsEnv.pszGbk,
		gs_stCsEnv.pszGbk2,
		iTimeOut);
	
	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}
	
	/*iRet = c_sprint_xml(&OutBody->StrXML, "SetSecondPwdRes", &stRes, sizeof(stRes));
			
	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	} 	*/
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}


SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__GetRoleList(struct soap* soap, 
	struct soapCsService1__GetRoleListReq *InBody, 
	struct soapCsService1__GetRoleListRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ZONEROLELISTRES stRes;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__GetRoleListReq,"GetRoleListReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}	

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}	

	if (!InBody->Uin)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->Uin");
		return 0;
	}


	iRet = cs_get_role_list(gs_stCsEnv.pCgiHandle,
		*InBody->RegionID,
		*InBody->WorldID,
		*InBody->Uin,
		 &stRes,
		 iTimeOut);
	
	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}
	
	iRet = c_sprint_xml(&OutBody->StrXML, "ZoneRoleListRes", &stRes, sizeof(stRes));

	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__FindFirstRoleByRolename(struct soap* soap, 
	struct soapCsService1__FindFirstRoleByRolenameReq *InBody, 
	struct soapCsService1__FindFirstRoleByRolenameRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ACCFINDROLELISTRES stRes;

	FindHandleInfo* pstInfo = NULL;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__FindFirstRoleByRolenameReq,"FindFirstRoleByRolenameReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}	

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}	

	if (!InBody->RoleName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RoleName");
		return 0;
	}	

	pstInfo = c_get_free_find_handle_info(&gs_stCsEnv,FIND_HANDLE_TYPE_ROLE);

	if (!pstInfo)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"no free find handle");
		return 0;
	}

	gs_stCsEnv.pszGbk = cs_get_gbk_str(InBody->RoleName);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}
	
	iRet = cs_find_first_role_by_rolename(gs_stCsEnv.pCgiHandle,
		*InBody->RegionID,
		*InBody->WorldID,
		gs_stCsEnv.pszGbk,
		&pstInfo->unFindHandle.pFindHandleRole,
		&stRes, 
		iTimeOut);
	
	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}

	c_find_handle_info_refresh(&gs_stCsEnv,pstInfo);

	OutBody->FindHandleID = &pstInfo->iID;
	
	iRet = c_sprint_xml(&OutBody->StrXML, "AccFindRoleListRes", &stRes, sizeof(stRes));
	
	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__FindNextRole(struct soap* soap, 
	struct soapCsService1__FindNextRoleReq *InBody, 
	struct soapCsService1__FindNextRoleRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ACCFINDROLELISTRES stRes;
	FindHandleInfo* pstInfo = NULL;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__FindNextRoleReq,"FindNextRoleReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->FindHandleID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->FindHandleID");
		return 0;
	}	

	pstInfo = c_get_find_handle_info(&gs_stCsEnv,InBody->FindHandleID,
		FIND_HANDLE_TYPE_ROLE);
	
	if (!pstInfo)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"c_get_find_handle_info err:%d",
			gs_stCsEnv.iGetFindHandleErr);
		return 0;		
	}
	
	iRet = cs_find_next_role(pstInfo->unFindHandle.pFindHandleRole, 
			&stRes);

	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}

	OutBody->FindHandleID = &pstInfo->iID;
	
	
	
	
	iRet = c_sprint_xml(&OutBody->StrXML, "AccFindRoleListRes", &stRes, sizeof(stRes));
			

	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__GetRoleInfo(struct soap* soap, 
	struct soapCsService1__GetRoleInfoReq *InBody, 
	struct soapCsService1__GetRoleInfoRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ZONEGETROLEDETAILRES stRes;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__GetRoleInfoReq,"GetRoleInfoReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}	

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}	

	if (!InBody->RoleName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RoleName");
		return 0;
	}
	
	gs_stCsEnv.pszGbk = cs_get_gbk_str(InBody->RoleName);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}

	iRet =  cs_get_role_info(gs_stCsEnv.pCgiHandle, 
				*InBody->RegionID,
				*InBody->WorldID,
				gs_stCsEnv.pszGbk,
				&stRes,
				iTimeOut);
	
	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}
	
	
	
	iRet = c_sprint_xml(&OutBody->StrXML, "ZoneGetRoleDetailRes", &stRes, sizeof(stRes));
			
	

	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__GetItemInfo(struct soap* soap, 
	struct soapCsService1__GetItemInfoReq *InBody, 
	struct soapCsService1__GetItemInfoRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ZONEGETROLEITEMLISTRES stRes;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__GetItemInfoReq,"GetItemInfoReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}	

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}	

	if (!InBody->RoleName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RoleName");
		return 0;
	}	

	gs_stCsEnv.pszGbk = cs_get_gbk_str(InBody->RoleName);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}
	
	iRet = cs_get_item_info(gs_stCsEnv.pCgiHandle,
				*InBody->RegionID,
				*InBody->WorldID,
				gs_stCsEnv.pszGbk,		
				&stRes, iTimeOut);
	
	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}

	
	
	
	iRet = c_sprint_xml(&OutBody->StrXML, "ZoneGetRoleItemListRes", &stRes, sizeof(stRes));
			

	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__GetSkillInfo(struct soap* soap, 
	struct soapCsService1__GetSkillInfoReq *InBody, 
	struct soapCsService1__GetSkillInfoRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ZONEGETROLESKILLINFORES stRes;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__GetSkillInfoReq,"GetSkillInfoReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}	

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}	

	if (!InBody->RoleName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RoleName");
		return 0;
	}	

	gs_stCsEnv.pszGbk = cs_get_gbk_str(InBody->RoleName);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}
	
	iRet = cs_get_skill_info(gs_stCsEnv.pCgiHandle,
				*InBody->RegionID,
				*InBody->WorldID,
				gs_stCsEnv.pszGbk,			
				&stRes, iTimeOut);
	
	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}

	
	
	
	iRet = c_sprint_xml(&OutBody->StrXML, "ZoneGetRoleSkillInfoRes", &stRes, sizeof(stRes));
			

	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__BlockRole(struct soap* soap, 
	struct soapCsService1__BlockRoleReq *InBody, 
	struct soapCsService1__BlockRoleRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ZONEBLOCKPLAYERRES stRes;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__BlockRoleReq,"BlockRoleReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}	

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}	

	if (!InBody->RoleName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RoleName");
		return 0;
	}		

	if (!InBody->Time)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->Time");
		return 0;
	}	

	gs_stCsEnv.pszGbk = cs_get_gbk_str(InBody->RoleName);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}
	
	iRet = cs_block_role(gs_stCsEnv.pCgiHandle, 
				*InBody->RegionID,
				*InBody->WorldID,
				gs_stCsEnv.pszGbk,	
				*InBody->Time,
				&stRes, iTimeOut);
	
	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}
	
	
	
	
	iRet = c_sprint_xml(&OutBody->StrXML, "ZoneBlockPlayerRes", &stRes, sizeof(stRes));
			
	
	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__SilenceRole(struct soap* soap, 
	struct soapCsService1__SilenceRoleReq *InBody, 
	struct soapCsService1__SilenceRoleRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ZONESILENCEROLERES stRes;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__SilenceRoleReq,"SilenceRoleReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}	

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}	

	if (!InBody->RoleName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RoleName");
		return 0;
	}		

	if (!InBody->Time)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->Time");
		return 0;
	}	

	gs_stCsEnv.pszGbk = cs_get_gbk_str(InBody->RoleName);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}
	
	iRet = cs_silence_role(gs_stCsEnv.pCgiHandle,
				*InBody->RegionID,
				*InBody->WorldID,
				gs_stCsEnv.pszGbk,	
				*InBody->Time,		
				&stRes, iTimeOut);
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}

	
	
	iRet = c_sprint_xml(&OutBody->StrXML, "ZoneSilenceRoleRes", &stRes, sizeof(stRes));
			

	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__ChgRolePos(struct soap* soap, 
	struct soapCsService1__ChgRolePosReq *InBody, 
	struct soapCsService1__ChgRolePosRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ZONECHGROLEPOSRES stRes;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__ChgRolePosReq,"ChgRolePosReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}	

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}	

	if (!InBody->RoleName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RoleName");
		return 0;
	}		

	if (!InBody->MapID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->MapID");
		return 0;
	}	

	if (!InBody->X)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->X");
		return 0;
	}

	if (!InBody->Y)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->Y");
		return 0;
	}

	gs_stCsEnv.pszGbk = cs_get_gbk_str(InBody->RoleName);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}
	
	iRet = cs_chg_role_pos(gs_stCsEnv.pCgiHandle, 
				*InBody->RegionID,
				*InBody->WorldID,
				gs_stCsEnv.pszGbk,	
				*InBody->MapID,			
				*InBody->X,
				*InBody->Y,
				&stRes, iTimeOut);

	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}

	
	
	iRet = c_sprint_xml(&OutBody->StrXML, "ZoneChgRolePosRes", &stRes, sizeof(stRes));
			
	
	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}
	
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__FindFirstMaillist(struct soap* soap, 
	struct soapCsService1__FindFirstMaillistReq *InBody, 
	struct soapCsService1__FindFirstMaillistRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ZONEGETMAILLISTRES stRes;
	FindHandleInfo* pstInfo = NULL;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__FindFirstMaillistReq,"FindFirstMaillistReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}	

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}	

	if (!InBody->RoleName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RoleName");
		return 0;
	}	

	
	pstInfo = c_get_free_find_handle_info(&gs_stCsEnv,FIND_HANDLE_TYPE_MAIL);
	
	if (!pstInfo)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"no free find handle");
		return 0;
	}	
	
	gs_stCsEnv.pszGbk = cs_get_gbk_str(InBody->RoleName);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}

	iRet = cs_find_first_maillist(gs_stCsEnv.pCgiHandle, 
				*InBody->RegionID,
				*InBody->WorldID,
				gs_stCsEnv.pszGbk,	
				&pstInfo->unFindHandle.pFindHandleMail,
				&stRes, iTimeOut);

	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}

	OutBody->FindHandleID = &pstInfo->iID;

	c_find_handle_info_refresh(&gs_stCsEnv,pstInfo);

	
	
	iRet = c_sprint_xml(&OutBody->StrXML, "ZoneGetMailListRes", &stRes, sizeof(stRes));
			
	
	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}
	
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__FindNextMaillist(struct soap* soap, 
	struct soapCsService1__FindNextMaillistReq *InBody, 
	struct soapCsService1__FindNextMaillistRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ZONEGETMAILLISTRES stRes;
	FindHandleInfo* pstInfo = NULL;

	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__FindNextMaillistReq,"FindNextMaillistReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->FindHandleID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->FindHandleID");
		return 0;
	}	

	pstInfo = c_get_find_handle_info(&gs_stCsEnv,InBody->FindHandleID,
		FIND_HANDLE_TYPE_MAIL);
	
	if (!pstInfo)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"c_get_find_handle_info err:%d",
			gs_stCsEnv.iGetFindHandleErr);
		return 0;		
	}

	iRet = cs_find_next_maillist(pstInfo->unFindHandle.pFindHandleMail,
		&stRes);

	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}

	
	
	iRet = c_sprint_xml(&OutBody->StrXML, "ZoneGetMailListRes", &stRes, sizeof(stRes));
			
	
	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__GetRideInfo(struct soap* soap, 
	struct soapCsService1__GetRideInfoReq *InBody, 
	struct soapCsService1__GetRideInfoRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ZONEGETROLERIDEINFORES stRes;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__GetRideInfoReq,"GetRideInfoReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}	

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}	

	if (!InBody->RoleName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RoleName");
		return 0;
	}	

	gs_stCsEnv.pszGbk = cs_get_gbk_str(InBody->RoleName);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}
	
	iRet = cs_get_ride_info(gs_stCsEnv.pCgiHandle, 
		*InBody->RegionID,
		*InBody->WorldID,
		gs_stCsEnv.pszGbk,
		&stRes,
		iTimeOut);

	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}

	
	
	iRet = c_sprint_xml(&OutBody->StrXML, "ZoneGetRoleRideInfoRes", &stRes, sizeof(stRes));
			
	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__GetBulletinList(struct soap* soap, 
	struct soapCsService1__GetBulletinListReq *InBody, 
	struct soapCsService1__GetBulletinListRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ZONEGETBULLETINLISTRES stRes;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__GetBulletinListReq,"GetBulletinListReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}	

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}	
	
	iRet = cs_get_bulletin_list(gs_stCsEnv.pCgiHandle,
		*InBody->RegionID,
		*InBody->WorldID,
		&stRes, iTimeOut);
	
	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}

	
	
	iRet = c_sprint_xml(&OutBody->StrXML, "ZoneGetBulletinListRes", &stRes, sizeof(stRes));
			
	
	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__AddBulletin(struct soap* soap, 
	struct soapCsService1__AddBulletinReq *InBody, 
	struct soapCsService1__AddBulletinRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__AddBulletinReq,"AddBulletinReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}	

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}	

	if (!InBody->Text)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->Text");
		return 0;
	}	

	if (!InBody->RollInterval)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RollInterval");
		return 0;
	}	

	if (!InBody->ActiveTime)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->ActiveTime");
		return 0;
	}

	if (!InBody->ActiveTimeStart)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->ActiveTimeStart");
		return 0;
	}	
	
	gs_stCsEnv.pszGbk = cs_get_gbk_str(InBody->Text);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}

	iRet = cs_add_bulletin(gs_stCsEnv.pCgiHandle,
		*InBody->RegionID,
		*InBody->WorldID,
		gs_stCsEnv.pszGbk,
		*InBody->RollInterval,
		*InBody->ActiveTime,		
		*InBody->ActiveTimeStart,
		&gs_stCsEnv.iTmp1,
		iTimeOut);

	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}

	OutBody->ID = &gs_stCsEnv.iTmp1;
	
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__AddBulletinToAll(struct soap* soap, 
	struct soapCsService1__AddBulletinToAllReq *InBody, 
	struct soapCsService1__AddBulletinToAllRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__AddBulletinToAllReq,"AddBulletinToAllReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->Text)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->Text");
		return 0;
	}	

	if (!InBody->RollInterval)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RollInterval");
		return 0;
	}	

	if (!InBody->ActiveTime)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->ActiveTime");
		return 0;
	}

	if (!InBody->ActiveTimeStart)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->ActiveTimeStart");
		return 0;
	}	

	gs_stCsEnv.pszGbk = cs_get_gbk_str(InBody->Text);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}
	
	iRet = cs_add_bulletin_to_all(gs_stCsEnv.pCgiHandle,
		gs_stCsEnv.pszGbk,
		*InBody->RollInterval,
		*InBody->ActiveTime,		
		*InBody->ActiveTimeStart,
		&gs_stCsEnv.iTmp1,
		iTimeOut);
	
	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}
	
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__DelBulletin(struct soap* soap, 
	struct soapCsService1__DelBulletinReq *InBody, 
	struct soapCsService1__DelBulletinRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__DelBulletinReq,"DelBulletinReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}	

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}	

	if (!InBody->ID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->ID");
		return 0;
	}	
	
	iRet = cs_del_bulletin(gs_stCsEnv.pCgiHandle, 
		*InBody->RegionID,
		*InBody->WorldID,
		*InBody->ID,
		iTimeOut);

	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}
	
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__DelBulletinFromAll(struct soap* soap, 
	struct soapCsService1__DelBulletinFromAllReq *InBody, 
	struct soapCsService1__DelBulletinFromAllRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__DelBulletinFromAllReq,"DelBulletinFromAllReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->ID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}	

	
	iRet = cs_del_bulletin_from_all(gs_stCsEnv.pCgiHandle, *InBody->ID,
		iTimeOut);
	
	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}
	
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__FindAccountByNameNext(struct soap* soap, 
	struct soapCsService1__FindAccountByNameNextReq *InBody, 
	struct soapCsService1__FindAccountByNameNextRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ACCFINDRES stRes;
	FindHandleInfo* pstInfo = NULL;
		
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__FindAccountByNameNextReq,"FindAccountByNameNextReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}
	
	if (!InBody->FindHandleID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->FindHandleID");
		return 0;
	}	

	pstInfo = c_get_find_handle_info(&gs_stCsEnv, InBody->FindHandleID,
		FIND_HANDLE_TYPE_ACC);

	if (!pstInfo)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"c_get_find_handle_info err:%d",
						gs_stCsEnv.iGetFindHandleErr);
		return 0;
	}

	iRet = cs_find_next_account(pstInfo->unFindHandle.pFindHandleAcc,&stRes);

	if(iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}

	c_find_handle_info_refresh(&gs_stCsEnv,pstInfo);
	
	
	
	
	iRet = c_sprint_xml(&OutBody->StrXML, "AccFindRes", &stRes, sizeof(stRes));
			

	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}


SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__ReloadServer(struct soap* soap, 
	struct soapCsService1__ReloadServerReq *InBody, 
	struct soapCsService1__ReloadServerRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ZONERELOADSERVERRES stRes;

	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__ReloadServerReq,"ReloadServerReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}

	iRet = cs_reload_server(gs_stCsEnv.pCgiHandle,
		*InBody->RegionID,
		*InBody->WorldID,
		&stRes,
		iTimeOut);
	
	if(iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"ReloadServer error:%d",iRet);
		return 0;
	}
	
	iRet = c_sprint_xml(&OutBody->StrXML, "ZoneReloadServerRes", &stRes, sizeof(stRes));
	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml error:%d",iRet);
		return 0;
	}

	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__ParseBlob(struct soap* soap, 
	struct soapCsService1__ParseBlobReq *InBody, 
	struct soapCsService1__ParseBlobRes *OutBody)
{	
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	//ROLEDATA stRes;
	char szMsg[2048000];
	char szBuff[2048000];
	int iBuff = 0;
	int iMsg = 0;
	int iLen = 0;


	szMsg[0] = 0;
	szBuff[0] = 0;

	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__ParseBlobReq,"ParseBlobReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->MetaVersion)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->MetaVersion");
		return 0;
	}

	if (!InBody->MetaName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->MetaName");
		return 0;
	}


	if (!InBody->DataHex)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->DataHex");
		return 0;
	}

	//iRet = init_analy_interface("../../cfg/db_meta/role_db_meta.tdr",&pHandle);
	//printf("Result: %d,%p\n", ret,pHandle);

	iBuff = sizeof(szBuff);

	iLen = strlen(InBody->DataHex);

	MutiLog(MUTI_LOGLEVEL_DEBUG, "hex len:%d",iLen);

	Hex2Bin(InBody->DataHex,iLen,(unsigned char *)szBuff, &iBuff);

	iBuff = min(iBuff, (int)sizeof(szBuff) -1);
	
	szBuff[iBuff]  = 0;	
		
	iMsg = sizeof(szMsg);

	iRet = analy_interface((int*)gs_stCsEnv.pstSSHan->pstSSProtoLib,
		InBody->MetaName,
		szBuff,
		iBuff,
		szMsg,
		&iMsg
		);
	
	iMsg = min(iMsg,(int)sizeof(szMsg) - 1);
	
	szMsg[iMsg] = 0;
		
	if(iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"ParseBlob error:%d",iRet);
		return 0;
	}

	if (0 == iRet && gs_stCsEnv.pstConf->SoapEncoding == SOAP_ENCODING_TYPE_UTF8 )
	{
		iRet = cs_gbk_to_utf8(&OutBody->StrXML,szMsg,iMsg);
	}

	
	//iRet = c_sprint_xml(&OutBody->StrXML, "RoleData", &stRes, sizeof(stRes));
	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml errno:%d errstr:%s",iRet,gs_stCsEnv.szLastErrLog);
		return 0;
	}
	
	UNUSED(HEX_value);
	
	return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__CashPropsGiftBill(struct soap* soap, 
	struct soapCsService1__CashPropsGiftBillReq *InBody, 
	struct soapCsService1__CashPropsGiftBillRes *OutBody)
{	
	CS_SERVICEENV* pstEnv = &gs_stCsEnv;
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	CASHPROPSGIFTINFORES stRes;	


	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__CashPropsGiftBillReq,"CashPropsGiftBillReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->AccName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->AccName");
		return 0;
	}
	
	if (!InBody->Uin)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->Uin");
		return 0;
	}

	if (!InBody->GroupID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->GroupID");
		return 0;
	}

	if (!InBody->AddCash)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->AddCash");
		return 0;
	}

	iRet = cs_cash_props_gift_bill(gs_stCsEnv.pCgiHandle,
		*InBody->RegionID,
		InBody->AccName,
		*InBody->Uin,
		*InBody->GroupID,
		*InBody->AddCash,
		&stRes,
		iTimeOut);
	
	if(iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"CashPropsGiftBill error:%d",iRet);
		return 0;
	}
	
	iRet = c_sprint_xml(&OutBody->StrXML, "CashPropsGiftInfoRes", &stRes, sizeof(stRes));
	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml errno:%d errstr:%s",iRet,pstEnv->szLastErrLog);
		return 0;
	}

	return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__SendSysMail(struct soap* soap, 
	struct soapCsService1__SendSysMailReq *InBody, 
	struct soapCsService1__SendSysMailRes *OutBody)
{	
	CS_SERVICEENV* pstEnv = &gs_stCsEnv;
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	char* pszToNameGbk = NULL;
	char* pszFromNameGbk = NULL;
	char* pszTitleGbk = NULL;
	char* pszTextGbk = NULL;
	char* pszMailItemsGbk = NULL;
	MAILITEMS stMailItems;
	LPTDRMETA pstMeta = NULL;
	TDRDATA stHost;
	TDRDATA stXml;

	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__SendSysMailReq,"SendSysMailReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}

	if (!InBody->ToName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->ToName");
		return 0;
	}
	pszToNameGbk = cs_get_gbk_str(InBody->ToName);

	if (!pszToNameGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"get_gbk_str InBody->ToName err");
		return 0;
	}

	if (!InBody->FromName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->FromName");
		return 0;
	}
	pszFromNameGbk = cs_get_gbk_str(InBody->FromName);

	if (!pszFromNameGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"get_gbk_str InBody->FromName err");
		return 0;
	}

	if (!InBody->Title)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->Title");
		return 0;
	}
	pszTitleGbk = cs_get_gbk_str(InBody->Title);

	if (!pszTitleGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"get_gbk_str InBody->Title err");
		return 0;
	}

	if (!InBody->Text)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->Text");
		return 0;
	}
	pszTextGbk = cs_get_gbk_str(InBody->Text);

	if (!pszTextGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"get_gbk_str InBody->Text err");
		return 0;
	}

	if (!InBody->Money)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->Money");
		return 0;
	}

	if (!InBody->MailItems)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->MailItems");
		return 0;
	}
	pszMailItemsGbk = cs_get_gbk_str(InBody->MailItems);

	if (!pszMailItemsGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"get_gbk_str InBody->MailItems err");
		return 0;
	}

	pstMeta = tdr_get_meta_by_name(pstEnv->pstSSHan->pstSSProtoLib, "MailItems");

	if (!pstMeta)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"no MailItems meta");
		return 0;
	}

	memset(&stMailItems,0,sizeof(stMailItems));
	
	stHost.pszBuff = (char*)&stMailItems;
	stHost.iBuff = sizeof(stMailItems);
	stXml.pszBuff = pszMailItemsGbk;
	stXml.iBuff = strlen(pszMailItemsGbk);

	if (*pszMailItemsGbk)
	{
		iRet = tdr_input(pstMeta, &stHost, &stXml, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
		if( iRet < 0)
		{
			OutBody->ErrorCode = CS_ERR_SYS;
			CS_ERR_TEXT(OutBody->StrError,"tdr_input ret:%d",iRet);
			return 0;
		}
	}

	{
		int k;
		for (k = 0; k < stMailItems.ArmNum; k++)
		{
			if (stMailItems.ItemNum >= MAIL_MAX_ITEM || stMailItems.ItemNum < 0)
			{
				break;
			}
			
			stMailItems.RoleGrids[stMailItems.ItemNum].Type = GRID_ITEM_ARM;
			stMailItems.RoleGrids[stMailItems.ItemNum].GridData.RoleArm = stMailItems.RoleArms[k];
			stMailItems.ItemNum++;
		}
	}

	iRet = cs_send_sys_mail(pstEnv->pCgiHandle,
		*InBody->RegionID,
		*InBody->WorldID,
		pszToNameGbk,
		pszFromNameGbk,
		pszTitleGbk,
		pszTextGbk,
		stMailItems.RoleGrids,
		stMailItems.ItemNum,
		*InBody->Money,		
		iTimeOut);
	
	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"SendSysMail ret:%d xml:",iRet);

		stMailItems.ArmNum = 1;
		stMailItems.ItemNum = 1;
		
		//c_sprint_xml(&OutBody->StrError->ErrorText, "MailItems", &stMailItems, sizeof(stMailItems));
			
		return 0;
	}

	OutBody->ErrorCode = CS_ERR_SUCCEEDED;

	return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__SetAccountUinFlag(struct soap* soap, 
	struct soapCsService1__SetAccountUinFlagReq *InBody, 
	struct soapCsService1__SetAccountUinFlagRes *OutBody)
{	
	CS_SERVICEENV* pstEnv = &gs_stCsEnv;
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;

	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__SetAccountUinFlagReq,"SetAccountUinFlagReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->AccName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->AccName");
		return 0;
	}


	if (!InBody->UinFlag)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->UinFlag");
		return 0;
	}

	if (!InBody->Type)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->Type");
		return 0;
	}

	iRet = cs_set_account_uin_flag(pstEnv->pCgiHandle,
		*InBody->RegionID,
		InBody->AccName,
		*InBody->UinFlag,
		*InBody->Type,
		iTimeOut);
	
	if(iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"SetAccountUinFlag error:%d",iRet);

		return 0;
	}

	return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__ExchangeClanLeader(struct soap* soap, 
	struct soapCsService1__ExchangeClanLeaderReq *InBody, 
	struct soapCsService1__ExchangeClanLeaderRes *OutBody)
{	
	CS_SERVICEENV* pstEnv = &gs_stCsEnv;
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	char* pszClanNameGbk = NULL;
	char* pszRoleNameGbk = NULL;

	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__ExchangeClanLeaderReq,"ExchangeClanLeaderReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}

	if (!InBody->ClanName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->ClanName");
		return 0;
	}
	pszClanNameGbk = cs_get_gbk_str(InBody->ClanName);

	if (!pszClanNameGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"get_gbk_str InBody->ClanName err");
		return 0;
	}

	if (!InBody->RoleName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RoleName");
		return 0;
	}
	pszRoleNameGbk = cs_get_gbk_str(InBody->RoleName);

	if (!pszRoleNameGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"get_gbk_str InBody->RoleName err");
		return 0;
	}

	iRet = cs_exchange_clan_leader(pstEnv->pCgiHandle,
		*InBody->RegionID,
		*InBody->WorldID,
		pszClanNameGbk,
		pszRoleNameGbk,
		iTimeOut);
	if(iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"ExchangeClanLeader error:%d",iRet);
		return 0;
	}

	OutBody->ErrorCode = 0;

	return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__RestoreDeletedRole(struct soap* soap, 
	struct soapCsService1__RestoreDeletedRoleReq *InBody, 
	struct soapCsService1__RestoreDeletedRoleRes *OutBody)
{	
	CS_SERVICEENV* pstEnv = &gs_stCsEnv;
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	unsigned long long llRoleWID = 0;
	

	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__RestoreDeletedRoleReq,"RestoreDeletedRoleReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}

	if (!InBody->RoleWID || *InBody->RoleWID == 0)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RoleWID");
		return 0;
	}

	llRoleWID = atoull(InBody->RoleWID);

	iRet = cs_restore_deleted_role(pstEnv->pCgiHandle,
		*InBody->RegionID,
		*InBody->WorldID,
		llRoleWID,
		iTimeOut);
	
	if(iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"RestoreDeletedRole error:%d",iRet);
		return 0;
	}

	return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__ChgRoleName(struct soap* soap, 
	struct soapCsService1__ChgRoleNameReq *InBody, 
	struct soapCsService1__ChgRoleNameRes *OutBody)
{	
	CS_SERVICEENV* pstEnv = &gs_stCsEnv;
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
		ZONECHGROLENAMERES stRes;
	char* pszRoleNameGbk = NULL;
	char* pszNewRoleNameGbk = NULL;

	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__ChgRoleNameReq,"ChgRoleNameReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}

	if (!InBody->RoleName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RoleName");
		return 0;
	}
	pszRoleNameGbk = cs_get_gbk_str(InBody->RoleName);

	if (!pszRoleNameGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"get_gbk_str InBody->RoleName err");
		return 0;
	}

	if (!InBody->NewRoleName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->NewRoleName");
		return 0;
	}
	pszNewRoleNameGbk = cs_get_gbk_str(InBody->NewRoleName);

	if (!pszNewRoleNameGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"get_gbk_str InBody->NewRoleName err");
		return 0;
	}

	iRet = cs_chg_role_name(gs_stCsEnv.pCgiHandle,
		*InBody->RegionID,
		*InBody->WorldID,
		pszRoleNameGbk,
		pszNewRoleNameGbk,
		&stRes,
		iTimeOut);
	
	if(iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"ChgRoleName error:%d",iRet);
		return 0;
	}
	
	iRet = c_sprint_xml(&OutBody->StrXML, "ZoneChgRoleNameRes", &stRes, sizeof(stRes));
	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml errno:%d errstr:%s",iRet,pstEnv->szLastErrLog);
		return 0;
	}

	return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__ShopCgi(struct soap* soap, 
	struct soapCsService1__ShopCgiReq *InBody, 
	struct soapCsService1__ShopCgiRes *OutBody)
{	
	CS_SERVICEENV* pstEnv = &gs_stCsEnv;
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	SHOPCGIREQ stReq;
	SHOPCGIRES stRes;

	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__ShopCgiReq,"ShopCgiReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}

	if (!InBody->Type)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->Type");
		return 0;
	}

	if (!InBody->Num)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->Num");
		return 0;
	}

	if (!InBody->Price)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->Price");
		return 0;
	}

	if (!InBody->ExchgNum)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->ExchgNum");
		return 0;
	}

	if (!InBody->StartTime)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->StartTime");
		return 0;
	}

	if (!InBody->EndTime)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->EndTime");
		return 0;
	}

	if (!InBody->InterVal)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->InterVal");
		return 0;
	}

	if (!InBody->TradeID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->TradeID");
		return 0;
	}

	if (!InBody->TradeFlag)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->TradeFlag");
		return 0;
	}

	if (!InBody->ReMoveType)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->ReMoveType");
		return 0;
	}

	
	stReq.Type = (char)*InBody->Type;
	stReq.Num = *InBody->Num;
	stReq.Price = *InBody->Price;
	stReq.ExchgNum = *InBody->ExchgNum;
	stReq.StartTime = *InBody->StartTime;
	stReq.EndTime= *InBody->EndTime;
	stReq.InterVal = *InBody->InterVal;
	stReq.TradeID = atoull(InBody->TradeID);
	stReq.TradeFlag = *InBody->TradeFlag;
	stReq.ReMoveType = *InBody->ReMoveType;

	iRet = cs_shop_cgi_req(gs_stCsEnv.pCgiHandle,
		*InBody->RegionID,
		*InBody->WorldID,
		&stReq,
		&stRes,
		iTimeOut);
	
	if(iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"ShopCgi error:%d",iRet);
		return 0;
	}
	
	iRet = c_sprint_xml(&OutBody->StrXML, "ShopCgiRes", &stRes, sizeof(stRes));
	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml errno:%d errstr:%s",iRet,pstEnv->szLastErrLog);
		return 0;
	}

	return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__SetPetInfo(struct soap* soap, 
	struct soapCsService1__SetPetInfoReq *InBody, 
	struct soapCsService1__SetPetInfoRes *OutBody)
{	
	CS_SERVICEENV* pstEnv = &gs_stCsEnv;
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	ZONESETROLEPETINFORES stRes;
	char* pszRoleNameGbk = NULL;
	char* pszRolePetXmlGbk = NULL;
	ROLEPET stRolePet;
	LPTDRMETA pstMeta;
	TDRDATA stHost;
	TDRDATA stXml;

	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__SetPetInfoReq,"SetPetInfoReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}

	if (!InBody->RoleName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RoleName");
		return 0;
	}
	pszRoleNameGbk = cs_get_gbk_str(InBody->RoleName);

	if (!pszRoleNameGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"get_gbk_str InBody->RoleName err");
		return 0;
	}

	if (!InBody->RolePetXml)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RolePetXml");
		return 0;
	}
	pszRolePetXmlGbk = cs_get_gbk_str(InBody->RolePetXml);

	if (!pszRolePetXmlGbk || !(*pszRolePetXmlGbk))
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"get_gbk_str InBody->RolePetXml err");
		return 0;
	}

	if (!InBody->OpMode)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->OpMode");
		return 0;
	}

	pstMeta = tdr_get_meta_by_name(pstEnv->pstSSHan->pstSSProtoLib, "RolePet");

	if (!pstMeta)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"no RolePet meta");
		return 0;
	}

	memset(&stRolePet,0,sizeof(stRolePet));
	
	stHost.pszBuff = (char*)&stRolePet;
	stHost.iBuff = sizeof(stRolePet);
	stXml.pszBuff = pszRolePetXmlGbk;
	stXml.iBuff = strlen(pszRolePetXmlGbk);

	if (*pszRolePetXmlGbk)
	{
		iRet = tdr_input(pstMeta, &stHost, &stXml, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
		if( iRet < 0)
		{
			OutBody->ErrorCode = CS_ERR_SYS;
			CS_ERR_TEXT(OutBody->StrError,"tdr_input ret:%d",iRet);
			return 0;
		}
	}

	iRet = cs_set_pet_info(gs_stCsEnv.pCgiHandle,
		*InBody->RegionID,
		*InBody->WorldID,
		pszRoleNameGbk,
		&stRolePet,
		*InBody->OpMode,
		&stRes,
		iTimeOut);
	if(iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"SetPetInfo error:%d",iRet);
		return 0;
	}
	
	iRet = c_sprint_xml(&OutBody->StrXML, "ZoneSetRolePetInfoRes", &stRes, sizeof(stRes));
	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml errno:%d errstr:%s",iRet,pstEnv->szLastErrLog);
		return 0;
	}

	return 0;
}


//lzk
SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__GetRoleMailData(struct soap* soap, 
	struct soapCsService1__GetRoleMailDataReq *InBody, 
	struct soapCsService1__GetRoleMailDataRes *OutBody)
{	
	CS_SERVICEENV* pstEnv = &gs_stCsEnv;
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
	MAIL stRes;
	
	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__GetRoleMailDataReq,"GetRoleMailDataReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}	

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}	

	if (!InBody->RoleName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RoleName");
		return 0;
	}

	if (!InBody->MailID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RoleName");
		return 0;
	}
	
	gs_stCsEnv.pszGbk = cs_get_gbk_str(InBody->RoleName);

	if (!gs_stCsEnv.pszGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}

	gs_stCsEnv.pszGbk2 = cs_get_gbk_str(InBody->MailID);
	
	if (!gs_stCsEnv.pszGbk2)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"cs_get_gbk_str err");
		return 0;
	}

	iRet =  cs_get_role_maildata(gs_stCsEnv.pCgiHandle, 
				*InBody->RegionID,
				*InBody->WorldID,
				gs_stCsEnv.pszGbk,
				atoull(gs_stCsEnv.pszGbk2),
				&stRes,
				iTimeOut);
	
	if (iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"cs_cgi error:%d",iRet);
		return 0;
	}

	iRet = c_sprint_xml(&OutBody->StrXML, "Mail", &stRes, sizeof(stRes));
	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml errno:%d errstr:%s",iRet,pstEnv->szLastErrLog);
		return 0;
	}
	return 0;
	UNUSED(soap);	//add by paraunused.pl
}

SOAP_FMAC5 int SOAP_FMAC6 __soapCsService1__SetItemInfo(struct soap* soap, 
	struct soapCsService1__SetItemInfoReq *InBody, 
	struct soapCsService1__SetItemInfoRes *OutBody)
{	
	CS_SERVICEENV* pstEnv = &gs_stCsEnv;
	int iRet = 0;
	int iTimeOut = DEFAULT_TIME_OUT;
		ZONESETROLEITEMLISTRES stRes;
	char* pszRoleNameGbk = NULL;
	char* pszRoleGridDataXmlGbk = NULL;
	ROLEGRID stRoleGrid;
	LPTDRMETA pstMeta;
	TDRDATA stHost;
	TDRDATA stXml;	

	if (0 > cs_check_client_restrict_op(soap,SOAP_TYPE_soapCsService1__SetItemInfoReq,"SetItemInfoReq"))
	{		
		OutBody->ErrorCode = CS_ERR_RESTRICT_OP;
		CS_ERR_TEXT(OutBody->StrError,"restrict operation");
		return 0;
	}	

	if (InBody->TimeOut > 0)
	{
		iTimeOut = InBody->TimeOut;
	}

	if (!InBody->RegionID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RegionID");
		return 0;
	}

	if (!InBody->WorldID)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->WorldID");
		return 0;
	}

	if (!InBody->RoleName)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RoleName");
		return 0;
	}
	pszRoleNameGbk = cs_get_gbk_str(InBody->RoleName);

	if (!pszRoleNameGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"get_gbk_str InBody->RoleName err");
		return 0;
	}

	if (!InBody->ListType)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->ListType");
		return 0;
	}

	if (!InBody->RoleGridDataXml)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->RoleGridDataXml");
		return 0;
	}
	pszRoleGridDataXmlGbk = cs_get_gbk_str(InBody->RoleGridDataXml);

	if (!pszRoleGridDataXmlGbk)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"get_gbk_str InBody->RoleGridDataXml err");
		return 0;
	}

	if (!InBody->OpMode)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"need InBody->OpMode");
		return 0;
	}

	if (*InBody->OpMode != ROLE_ITEM_DEL)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"wrong InBody->OpMode");
		return 0;
	}

	pstMeta = tdr_get_meta_by_name(pstEnv->pstSSHan->pstSSProtoLib, "RoleGrid");

	if (!pstMeta)
	{
		OutBody->ErrorCode = CS_ERR_SYS;
		CS_ERR_TEXT(OutBody->StrError,"no RoleGrid meta");
		return 0;
	}

	memset(&stRoleGrid,0,sizeof(stRoleGrid));
	
	stHost.pszBuff = (char*)&stRoleGrid;
	stHost.iBuff = sizeof(stRoleGrid);
	stXml.pszBuff = pszRoleGridDataXmlGbk;
	stXml.iBuff = strlen(pszRoleGridDataXmlGbk);

	if (*pszRoleGridDataXmlGbk)
	{
		iRet = tdr_input(pstMeta, &stHost, &stXml, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
		if( iRet < 0)
		{
			OutBody->ErrorCode = CS_ERR_SYS;
			CS_ERR_TEXT(OutBody->StrError,"tdr_input ret:%d",iRet);
			return 0;
		}
	}

	iRet = cs_set_item_info(gs_stCsEnv.pCgiHandle,
		*InBody->RegionID,
		*InBody->WorldID,
		pszRoleNameGbk,
		*InBody->ListType,
		&stRoleGrid,
		*InBody->OpMode,
		&stRes,
		iTimeOut);
	
	if(iRet < 0)
	{
		OutBody->ErrorCode = iRet;
		CS_ERR_TEXT(OutBody->StrError,"SetItemInfo error:%d",iRet);
		return 0;
	}
	
	iRet = c_sprint_xml(&OutBody->StrXML, "ZoneSetRoleItemListRes", &stRes, sizeof(stRes));
	if(iRet < 0)
	{
		OutBody->ErrorCode = CS_ERR_XML;
		CS_ERR_TEXT(OutBody->StrError,"cs_sprint_xml errno:%d errstr:%s",iRet,pstEnv->szLastErrLog);
		return 0;
	}

	return 0;
}

