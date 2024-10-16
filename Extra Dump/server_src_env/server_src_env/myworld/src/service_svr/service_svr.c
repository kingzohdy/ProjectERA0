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
#include "service_svr_conf_desc.h"
#include "version.h"
#include "tbus/tbus.h"
#include "comm_misc.h"

#include "soapH.h"
#include "soapStub.h"
#include <iconv.h>
#include <scew/scew.h>
#include "entity.h"
#include <curl/curl.h>

#include "GameExchangeSoap.nsmap"
#include "mysql/mysql.h"
#include "mysql/errmsg.h"
#include "../auth_svr/auth_svr.h"
#include "md5.h"
#include "../auth_svr/auth_svr_conf_desc.h"
#include <json/json.h>



// 让编译器高兴
#define UNUSED( param ) ((void)(param))


#define MAX_THREAD 256
#define LOCAL_BUS_ADDR_START 10
#define CHANNEL_SIZE 1024*256
#define MAX_MSG_SIZE 64000
#define CHANNEL_APPEND 128
//char szWatchMsg[MAX_MSG_SIZE - sizeof()];
#define ACC_TABLE "ljy_member_profile"
#define BBS_TABLE "uc_members"
#define ACTIVE_TABLE "Active"
#define PASSWD_CARD_TABLE "user_pwdcard"


#define SQL_BUFF_LEN 2048
#define DB_PING_INTERVAL 60

#define SMS_PWD "100XTg7H[bC!"
#define MAX_WEB_POST_RECV 4096

#ifndef CCH
#define CCH(arr) (sizeof(arr)/sizeof(arr[0]))
#endif // CCH

#ifndef WIN32
#define max(a, b)	((a) > (b) ? (a) : (b))
#define min(a, b)	((a) < (b) ? (a) : (b))
#endif

typedef struct tagChannelAppend
{
	char sAppend[CHANNEL_APPEND];
}ChannelAppend;

typedef enum 
{
	THREAD_TYPE_AUTH = 0,	//帐号认证线程
	THREAD_TYPE_SMS,		//短信线程
}THREAD_TYPE;

typedef enum 
{
	SMS_DB_STAT_INIT = 0,	//初始化
	SMS_DB_STAT_PREPARE,		//已准备
}SMS_DB_STAT;

typedef struct tagThreadMgr
{
	pthread_t tid;
	int iIdx;
	int iType; //类型 THREAD_TYPE
	LPTBUSCHANNEL	pstChannel0;
	LPTBUSCHANNEL	pstChannel1;
	char cIsExit;
	char cIsValid;
	MYSQL *pstConn;
	int iLastDBPing;
	CURL *pstCurl;
	char szPostData[MAX_WEB_POST_RECV];                	/*   web验证请求的内容格式 */
	char szRecvData[MAX_WEB_POST_RECV];
}ThreadMgr;

struct tagservice_svrEnv
{
	SERVICE_SVRCONF *pstConf;
	SERVICE_SVRRUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	TAPPCTX *pstAppCtx;
	pthread_mutex_t stLogMutex;
	int iThread;
	int iBatchSmsIdx;//处理到第几批次
	int iBatchSmsOffset;//当前批次的手机号码偏移量
	int iBatchSmsLastSend;//批量短信最后发送时间
	int iBatchSmsDBStat;//批量短信db状态 SMS_DB_STAT
	int iBatchSmsLastCount;//最后次select的count计数
	int iBatchSmsFailCount;//当前批次的手机号码偏移量
	ThreadMgr astThread[MAX_THREAD];
	LPSSHANDLE pstSSHan;
	char szWebAuthPost[256];
	char szSmsPost[256];
};

typedef struct tagservice_svrEnv		SERVICE_SVRENV;
typedef struct tagservice_svrEnv		*LSERVICE_SVRENV;

static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_service_svr[];
extern unsigned char g_szMetalib_proto_ss[];


SERVICE_SVRENV gs_stEnv;
static SERVICE_SVRRUN_STATUS gs_stStat;

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
			pthread_mutex_lock(&gs_stEnv.stLogMutex); \
			switch(iLevel)\
			{\
			case MUTI_LOGLEVEL_TRACE:\
				tlog_trace(gs_stEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_DEBUG:\
				tlog_debug(gs_stEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_INFO:\
				tlog_info(gs_stEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_WARN:\
				tlog_warn(gs_stEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_ERROR:\
				tlog_error(gs_stEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_SEVERE:\
				tlog_fatal(gs_stEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			default:\
				break;\
			}\
			pthread_mutex_unlock(&gs_stEnv.stLogMutex); \
		  }while(0)

/* indentation size (in whitespaces) */
int const indent_size = 4;

void
print_indent(unsigned int indent)
{
	if (indent > 0)
	{
		printf("%*s", indent * indent_size, " ");
	}
}

void
print_attributes(scew_element* element)
{
	scew_attribute* attribute = NULL;

	if (element != NULL)
	{
		/**
		 * Iterates through the element's attribute list, printing the
		 * pair name-value.
		 */
		attribute = NULL;
		while ((attribute = scew_attribute_next(element, attribute)) != NULL)
		{
			printf(" %s=\"%s\"", scew_attribute_name(attribute),
				   scew_attribute_value(attribute));
		}
	}
}


void print_element(scew_element* element, unsigned int indent)
{
	scew_element* child = NULL;
	XML_Char const* contents = NULL;

	if (element == NULL)
	{
		return;
	}

	/**
	 * Prints the starting element tag with its attributes.
	 */
	print_indent(indent);
	printf("<%s", scew_element_name(element));
	print_attributes(element);
	printf(">");
	contents = scew_element_contents(element);
	if (contents == NULL)
	{
		printf("\n");
	}

	/**
	 * Call print_element function again for each child of the
	 * current element.
	 */
	child = NULL;
	while ((child = scew_element_next(element, child)) != NULL)
	{
		print_element(child, indent + 1);
	}

	/* Prints element's content. */
	if (contents != NULL)
	{
		printf("%s", contents);
	}
	else
	{
		print_indent(indent);
	}

	/**
	 * Prints the closing element tag.
	 */
	printf("</%s>\n", scew_element_name(element));
}

void parse_element(scew_element* element, int* piIdx,SSCDKEYRES* pstRes)
{
	scew_element* child = NULL;
	XML_Char const* contents = NULL;
	XML_Char const* pszName = NULL;

	if (element == NULL)
	{
		return;
	}

	pszName = scew_element_name(element);
	
	contents = scew_element_contents(element);

	/**
	 * Call print_element function again for each child of the
	 * current element.
	 */
	child = NULL;
	while ((child = scew_element_next(element, child)) != NULL)
	{
		parse_element(child, piIdx,pstRes);
	}
	
	if (contents != NULL)
	{
		if(0 == strcmp(pszName,"ErrorMessage"))
		{
			pstRes->Succ = atoi(contents);
		}
		else if (0 == strcmp(pszName,"ItemCode"))
		{
			if (*piIdx < CDKEY_ITEM_NUM)
			{
				pstRes->Items[*piIdx].ItemID = atoi(contents);
				pstRes->Items[*piIdx].ItemNum = 1;
			}
			else
			{
			}
			
		}
	}
	else
	{
		if(0 == strcmp(pszName,"AwardsInfo"))
		{
			(*piIdx)++;
		}
	}

}

// 生成密钥
static inline void ifm_gen_key(char *psKey, const char * pszPasswd)
{
	char szTmp[KEY_LEN];

	Md5HashBuffer(szTmp, pszPasswd, strlen(pszPasswd));
	Md5HashBuffer(psKey, szTmp, sizeof(szTmp));
}

// 生成用户中心密钥
static inline void uc_gen_key(char *pszKey, const char * pszPasswd,char * pszSalt)
{
	char szTmp[UC_KEY_LEN*2] = {0};
	char szTmp2[UC_KEY_LEN] = {0};

	

	Md5HashBuffer(szTmp, pszPasswd, strlen(pszPasswd));
	Md5HexString(szTmp,szTmp2);
	snprintf(szTmp,sizeof(szTmp),"%s%s",szTmp2,pszSalt);
	
	Md5HashBuffer(szTmp2, szTmp, strlen(szTmp));
	
	Md5HexString(szTmp2,szTmp);
	STRNCPY(pszKey,szTmp,UC_KEY_LEN+1);
	
}

void uc_gen_salt(char* pszOut)
{
	int i = 0;
	int iRand = RAND1(256*256*256-1);
	char * pszRand = (char*)&iRand;
	
	for (i=0; i<3; i++)
		sprintf(&pszOut[2*i], "%02x", pszRand[i] & 0xFF);
}

// 生成用户中心密钥
static inline void uc_gen_cookie_key(char *pszKey, const char * pszAccount,char * pszSalt)
{
	char szTmp[UC_KEY_LEN*2] = {0};
	char szTmp2[UC_KEY_LEN] = {0};

	

	Md5HashBuffer(szTmp, pszAccount, strlen(pszAccount));
	Md5HexString(szTmp,szTmp2);
	snprintf(szTmp,sizeof(szTmp),"%s%s",szTmp2,pszSalt);
	
	Md5HashBuffer(szTmp2, szTmp, strlen(szTmp));
	
	Md5HexString(szTmp2,szTmp);
	STRNCPY(pszKey,szTmp,UC_KEY_LEN+1);
	
}

// 窥视ExtraData中的密码，仅仅在开发模式中使用
static const char * ifm_peek_passwd(const void *pszExtraData, int iExtraDataLen)
{
	const char *pszRet = NULL;
	const AUTHINFO *pstAuthInfo = NULL;
	
	if(iExtraDataLen < (int)sizeof(SelectInfo))
	{
		pszRet = "";
	}
	else
	{
		pstAuthInfo = &(((SelectInfo *)(pszExtraData))->stAuthInfo);
		pszRet = pstAuthInfo->AccountLoginPasswd;
	}
	return pszRet;
}

// 窥视ExtraData中的AuthFlag，仅仅在开发模式中使用
static int ifm_peek_auth_flag(const void *pszExtraData, int iExtraDataLen,int *piFlag)
{
	SelectInfo * pstInfo = NULL;
	
	if(iExtraDataLen >= (int)sizeof(SelectInfo))
	{
		pstInfo = (SelectInfo *)pszExtraData;

		*piFlag = pstInfo->AuthFlag;
		
		return 0;
	}
	
	return -1;
}


int ss_cdkey_req(SERVICE_SVRENV* pstEnv,ThreadMgr *pstThread,SSCDKEYREQ* pstReq,SSPKG * pstResPkg)
{
	SSCDKEYRES* pstRes = &pstResPkg->Body.SSCDKeyRes;

	struct soap soap;
	struct _ns1__UseCode stUseCodeReq;
	struct _ns1__UseCodeResponse stUseCodeRes;
	
	char** ppRet = NULL;
	char szOut[MAX_MSG_SIZE];
	int iRegionID;
	int iSvrID;
	char szRegionID[NAME_LEN];
	char szSvrID[NAME_LEN];
	char szIP[NAME_LEN];

	memset(&stUseCodeReq,0,sizeof(stUseCodeReq));
	memset(pstRes,0,sizeof(*pstRes));

	pstRes->Succ = 9999;

	if (pstReq->CDKey[0] == 0 || pstReq->RoleName[0] == 0 || !pstReq->SvrID)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR,"SSCDKEYREQ");
		goto final;
		return 0;
	}

	GET_WORLD_ID(iSvrID, pstReq->SvrID);
	GET_REGION_ID(iRegionID,pstReq->SvrID);
	
	memset(szSvrID,0,sizeof(szSvrID));
	memset(szRegionID,0,sizeof(szRegionID));

	snprintf(szSvrID,sizeof(szSvrID),"%d",iSvrID);
	snprintf(szRegionID,sizeof(szRegionID),"%d",iRegionID);
	snprintf(szIP,sizeof(szIP),"%s",
			inet_ntoa(*(struct in_addr *)&pstReq->IP) );

	
	soap_init1(&soap, SOAP_C_UTFSTRING);

	
	stUseCodeReq.areaId = szRegionID;
	stUseCodeReq.code = pstReq->CDKey;
	stUseCodeReq.game = "LJY";
	stUseCodeReq.ip = szIP;
	stUseCodeReq.isSendByClient = 1;
	stUseCodeReq.pass9 = pstReq->Pass9Account;
	stUseCodeReq.roleId = (int)pstReq->RoleID;
	stUseCodeReq.roleName = pstReq->RoleName;
	stUseCodeReq.serverId = szSvrID;

	soap_call___ns1__UseCode(&soap, pstEnv->pstConf->WebServiceUrl, NULL, &stUseCodeReq, &stUseCodeRes);
	ppRet = &stUseCodeRes.UseCodeResult;


	  
	if (soap.error)
	{ 
		//soap_print_fault(&soap, stderr);
		
		MutiLog(MUTI_LOGLEVEL_ERROR,"soap error %d:%s",soap.error,*soap_faultstring(&soap));
		goto final;
		return 0;
	}
	else
	{
		size_t sInLen = strlen(*ppRet);
		size_t sOutLen = MAX_MSG_SIZE;

		scew_tree* tree = NULL;
	    scew_parser* parser = NULL;
		char * pszXml = NULL;
		int i = 0;
		iconv_t cd = 0;


	    /**
	     * Creates an SCEW parser. This is the first function to call.
	     */
	    parser = scew_parser_create();

	    scew_parser_ignore_whitespaces(parser, 1);
	    //parser_set_default_encoding(parser);

	    /* Loads an XML file */
		pszXml = *ppRet;
		for( i = 0; (size_t)i < sInLen; i++)
		{
			if (*pszXml == '<')
			{
				break;
			}
			pszXml++;
		}
		if (*pszXml != '<')
		{
			MutiLog(MUTI_LOGLEVEL_ERROR,"invalide xml %s",*ppRet);
			goto final;
			return 0;
		}
	    if (!scew_parser_load_buffer(parser, pszXml,sInLen-i))
	    {
	        scew_error code = scew_error_code();
	        //printf("Unable to load buffer (error #%d: %s)\n", code,
	        MutiLog(MUTI_LOGLEVEL_ERROR,"Unable to load buffer (error #%d: %s)\n", code,
	               scew_error_string(code));
			
	        if (code == scew_error_expat)
	        {
	            enum XML_Error expat_code = scew_error_expat_code(parser);
	            //printf("Expat error #%d (line %d, column %d): %s\n", expat_code,
				MutiLog(MUTI_LOGLEVEL_ERROR,"Expat error #%d (line %d, column %d): %s\n", expat_code,
	                   scew_error_expat_line(parser),
	                   scew_error_expat_column(parser),
	                   scew_error_expat_string(expat_code));
	        }
	        goto final;
			return 0;
	    }

	    tree = scew_parser_tree(parser);

	    /* parse full tree */
		i = 0;
	    parse_element(scew_tree_root(tree), &i,pstRes);

		if (pstEnv->pstConf->ServiceMode == SERVICE_MODE_DEVELOP)
		{
			char szItemID[CDKEY_LEN];
			char szSucc[CDKEY_LEN];
			char* pCDKey = pstReq->CDKey;
			char* pEmit = szItemID;
			for(i = 0; i < CDKEY_LEN; i++)
			{
				if (0 == *pCDKey)
				{
					break;
				}
				if (':' == *pCDKey)
				{
					pEmit = szSucc;
					pCDKey++;
					continue;
				}
				*pEmit = *pCDKey;
				pEmit++;
				pCDKey++;
			}
			pstRes->Items[0].ItemID = atoi(szItemID);
			pstRes->Succ = atoi(szSucc);
		}

	    /* Remember to free tree (scew_parser_free does not free it) */
	    scew_tree_free(tree);

	    /* Frees the SCEW parser */
	    scew_parser_free(parser);
		
		cd = iconv_open("gbk","utf-8");
		if ((iconv_t)(-1) != cd)
		{
			char* pOut = &szOut[0];
			char** ppOut = NULL;
			ppOut = &pOut;
			iconv(cd, ppRet, &sInLen,ppOut, &sOutLen);
			//printf("inlen = %d,outlen = %d\n\n", sInLen,sOutLen);
			//printf("locale = %s\n\n", setlocale(LC_CTYPE,""));
	    	//printf("result = %s\n\n", szOut);
			MutiLog(MUTI_LOGLEVEL_DEBUG, "web service recv %s", szOut);
			iconv_close(cd);
		}
	}
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);


final:	
	
	pstResPkg->Head.Cmd = SS_CDKEY_RES;
	pstResPkg->Head.Magic = PROTOCOL_MAGIC; 
	pstResPkg->Head.Ver = TDR_METALIB_PROTO_SS_VERSION;
	pstResPkg->Head.HeadLen = sizeof(pstResPkg->Head);

	memcpy(pstRes->RoleName,pstReq->RoleName,sizeof(pstRes->RoleName));
	memcpy(pstRes->CDKey,pstReq->CDKey,sizeof(pstRes->CDKey));
	
	return 0;
	UNUSED(pstThread);	//add by paraunused.pl
}

static MYSQL * db_init(SERVICE_SVRENV *pstEnv)
{
	MYSQL *conn = mysql_init(NULL);
	if(!conn)
		return NULL;

	if(!mysql_real_connect(
		conn,
		pstEnv->pstConf->MysqlIP,
		pstEnv->pstConf->MysqlUser,
		pstEnv->pstConf->MysqlPwd,
		pstEnv->pstConf->BBSDB,
		pstEnv->pstConf->MysqlPort,
		NULL,
		CLIENT_COMPRESS|CLIENT_MULTI_STATEMENTS))
	{
		mysql_close(conn);
		return NULL;
	}
	return conn;
}

static int db_select_active_key(SERVICE_SVRENV* pstEnv,USERCENTER* pstUser,
	char* loginname,ThreadMgr* pstThread)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	int err = 0;
	char szSql[SQL_BUFF_LEN] = {0};
	int iRet = -1;

	snprintf(szSql,sizeof(szSql),
		"select ActiveCode  from %s "
		"where Pass9Account ='%s' and IsActive = 1;", 
		ACTIVE_TABLE,loginname);
		
	mysql_select_db(pstThread->pstConn,pstEnv->pstConf->ActiveKeyDB);	
	
	err = mysql_query(pstThread->pstConn, szSql);
	if(err)
	{
		iRet = -2;
		goto final;
	}

	res = mysql_store_result(pstThread->pstConn);
	if(!res)
	{
		iRet = -2;
		goto final;
	}

	row = NULL;

	while((row = mysql_fetch_row(res)) != NULL)
	{
		if(!row[0])
		{
			continue;
		}

		iRet = 0;
		/*
		if(iFlag)
		{
			char szPreCode[8] = {0};
			STRNCPY(szPreCode,row[0],sizeof(szPreCode));
			
			db_add_acc_gift(pstEnv,pstUser,pstThread,szPreCode);
		}
		*/
	}

	
	mysql_free_result(res);


final:

	if (-2 == iRet)
	{
		MutiLog(MUTI_LOGLEVEL_INFO, "mysql err:%s",mysql_error(pstThread->pstConn));
	}

	err = mysql_errno(pstThread->pstConn);
	if (CR_SERVER_LOST == err || CR_SERVER_GONE_ERROR == err)
	{
		if (pstThread->pstConn)
		{
			mysql_close(pstThread->pstConn);
			pstThread->pstConn = NULL;
		}
		pstThread->pstConn = db_init(pstEnv);
		if (pstThread->pstConn)
		{
			MutiLog(MUTI_LOGLEVEL_INFO, "mysql reconnet");
		}
		else
		{
			MutiLog(MUTI_LOGLEVEL_INFO, "mysql reconnet fail");
		}
	}

	
	return iRet;
	UNUSED(pstUser);	//add by paraunused.pl
}



static int db_select_passwd_card(SERVICE_SVRENV* pstEnv,USERCENTER* pstUser,
	char* loginname,ThreadMgr* pstThread)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	int err = 0;
	char szSql[SQL_BUFF_LEN] = {0};
	int iRet = -1;
	int iXLen = 0;
	int iYLen = 0;

	PASSWDCARD *pstCard = NULL;


	snprintf(szSql,sizeof(szSql),
		"select x_num,y_num,matrix from %s "
		"where uid = %d and status = 1;", 
		PASSWD_CARD_TABLE,pstUser->Uin);
		
	mysql_select_db(pstThread->pstConn,pstEnv->pstConf->ACCDB);	
	
	err = mysql_query(pstThread->pstConn, szSql);
	if(err)
	{
		iRet = -2;
		goto final;
	}

	res = mysql_store_result(pstThread->pstConn);
	if(!res)
	{
		iRet = -2;
		goto final;
	}

	row = NULL;

	while((row = mysql_fetch_row(res)) != NULL)
	{
		if(!row[0])
		{
			continue;
		}

		

		iXLen = atoi(row[0]);
		iYLen = atoi(row[1]);

		
		pstCard = &pstUser->PasswdCard;
		
		iRet = str_to_passwd_card(row[2],iXLen,iYLen,pstCard);

		if (iRet != 0)
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "str_to_passwd_card err,account:%s ret:%d",
				loginname,iRet);
		}
	}

	
	mysql_free_result(res);


final:

	if (-2 == iRet)
	{
		MutiLog(MUTI_LOGLEVEL_INFO, "mysql err:%s",mysql_error(pstThread->pstConn));
	}

	err = mysql_errno(pstThread->pstConn);
	if (CR_SERVER_LOST == err || CR_SERVER_GONE_ERROR == err)
	{
		if (pstThread->pstConn)
		{
			mysql_close(pstThread->pstConn);
			pstThread->pstConn = NULL;
		}
		pstThread->pstConn = db_init(pstEnv);
		if (pstThread->pstConn)
		{
			MutiLog(MUTI_LOGLEVEL_INFO, "mysql reconnet");
		}
		else
		{
			MutiLog(MUTI_LOGLEVEL_INFO, "mysql reconnet fail");
		}
	}

	
	return iRet;
	UNUSED(pstUser);	//add by paraunused.pl
}


static int db_add_acc_gift(SERVICE_SVRENV* pstEnv,USERCENTER* pstUser,
							ThreadMgr* pstThread,char* pszPreCode)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	int err = 0;
	char szSql[SQL_BUFF_LEN] = {0};
	int iRet = 0;
	int iRegionID = 0;
	int iSqlRet = 0;	
	int status = 0;

	GET_REGION_ID(iRegionID,pstEnv->pstAppCtx->iId);

	// x为区id,y为帐号名,z为区分媒体的激活码前几位,用ret判断哪一步成功或未执行成功
	snprintf(szSql,sizeof(szSql),		
		"set @ret = 0;"
		"set @stage = 0;"
		
		"use %s;"
		"select GiftName,ItemID,ItemNum into @giftName, @itemID, @itemNum from"
			" ActiveCodeGiftDef where PreCode = "
			"LEFT('%s', CHAR_LENGTH(PreCode)) ;"
		"select IF(FOUND_ROWS() > 0,1,0) into @stage;"
		"set @ret = @ret + @stage;"

		"use account_%d; "
		"SELECT Uin into @uin FROM `Account`  where Pass9Account = '%s'; "

		"select IF(FOUND_ROWS() > 0,2,0) into @stage;"
		"set @ret = @ret + @stage;"

		"use bill_%d; "

		"select ID,Uin,Name,GiftItems1_ItemID,GiftItems1_ItemNum from AccGift where "
			"	GiftItems1_ItemID = @itemID and GiftItems1_ItemNum = @itemNum and"
			" Uin = @uin and Name like @giftName;"

		"select IF(FOUND_ROWS() > 0,4,0) into @stage;"
		"set @ret = @ret + @stage;"

		"insert into AccGift(ID,Uin,RoleID,Name,GiftItems1_ItemID,GiftItems1_ItemNum,SetGiftTime)"
			"	SELECT ifnull(max(LAST_INSERT_ID(ID)+1),1), @uin,0, @giftName, @itemID, "
			"@itemNum,cast(NOW() as datetime) from AccGift WHERE @ret = 3 limit 1;"		

		"select IF(ROW_COUNT() > 0,8,0) into @stage;"
		"set @ret = @ret + @stage;"

		"select LAST_INSERT_ID(ID) into @giftID from AccGift limit 1;"

		"select @ret as ret , @uin as uin, @giftID as giftID, @giftName as giftName, "
			"@itemID as itemID, @itemNum as itemNum;"

		,
		pstEnv->pstConf->ActiveKeyDB,
		pszPreCode,
		iRegionID,
		pstUser->UserName,
		iRegionID//bill_x
		);
	
	MutiLog(MUTI_LOGLEVEL_INFO, "mysql sql:%s",szSql);
		
	mysql_select_db(pstThread->pstConn,pstEnv->pstConf->ActiveKeyDB);	
	
	err = mysql_query(pstThread->pstConn, szSql);
	if(err)
	{
		iRet = -2;
		goto final;
	}


	do {
		res = mysql_store_result(pstThread->pstConn);
		if (res)
		{
			if (mysql_field_count(pstThread->pstConn) == 6)
			{
				row = NULL;
	
				while((row = mysql_fetch_row(res)) != NULL)
				{
					if(!row[0])
					{
						continue;
					}
					iSqlRet = atoi(row[0]);
					MutiLog(MUTI_LOGLEVEL_INFO, "db_add_acc_gift ret:%s uin:%s giftid:%s "
												"giftname:%s itemid:%s itemnum:%s",
												row[0],row[1],row[2],
												row[3],row[4],row[5]);

				}
				

			}
			mysql_free_result(res);
		}
		else 
		{
			if (mysql_field_count(pstThread->pstConn) == 0)
			{
			}
			else
			{
				iRet = -2;
				goto final;
			}
		}
		/* more results? -1 = no, >0 = error, 0 = yes (keep looping) */
		if ((status = mysql_next_result(pstThread->pstConn)) > 0)
		{
			err = status;
			iRet = -2;
			goto final;
		}
		
	} while (status == 0);

	if (iSqlRet == 11)
	{
		iRet = 0;
	}

	
final:

	if (-2 == iRet)
	{
		MutiLog(MUTI_LOGLEVEL_INFO, "mysql err:%s",mysql_error(pstThread->pstConn));
	}

	if (CR_SERVER_LOST == err || CR_SERVER_GONE_ERROR == err)
	{
		if (pstThread->pstConn)
		{
			mysql_close(pstThread->pstConn);
			pstThread->pstConn = NULL;
		}
		pstThread->pstConn = db_init(pstEnv);
		
		MutiLog(MUTI_LOGLEVEL_INFO, "mysql reconnet");
	}
	
	return iRet;

}


static int db_use_active_key(SERVICE_SVRENV* pstEnv,USERCENTER* pstUser,char* loginname,
							char* pszActiveCode,ThreadMgr* pstThread)
{
	//MYSQL_RES *res;
	//MYSQL_ROW row;
	int err = 0;
	char szSql[SQL_BUFF_LEN] = {0};
	char szRealSql[SQL_BUFF_LEN] = {0};
	int iRet = 0;
	char szActiveCode[SQL_BUFF_LEN] = {0};
	

	mysql_real_escape_string(pstThread->pstConn,szRealSql,loginname,strlen(loginname));
	mysql_real_escape_string(pstThread->pstConn,szActiveCode,pszActiveCode,strlen(pszActiveCode));

	snprintf(szSql,sizeof(szSql),
		"update %s set IsActive = 1 , Pass9Account ='%s'"
		" where ActiveCode = '%s' and IsActive = 0;", 
		ACTIVE_TABLE,szRealSql,szActiveCode);

	MutiLog(MUTI_LOGLEVEL_INFO, "mysql sql:%s",szSql);
		
	mysql_select_db(pstThread->pstConn,pstEnv->pstConf->ActiveKeyDB);	
	
	err = mysql_query(pstThread->pstConn, szSql);
	if(err)
	{
		iRet = -2;
		goto final;
	}

final:

	if (-2 == iRet)
	{
		MutiLog(MUTI_LOGLEVEL_INFO, "mysql err:%s",mysql_error(pstThread->pstConn));
	}

	if (CR_SERVER_LOST == err || CR_SERVER_GONE_ERROR == err)
	{
		if (pstThread->pstConn)
		{
			mysql_close(pstThread->pstConn);
			pstThread->pstConn = NULL;
		}
		pstThread->pstConn = db_init(pstEnv);
		
		MutiLog(MUTI_LOGLEVEL_INFO, "mysql reconnet");
	}
	
	return iRet;

	UNUSED(pstUser);	//add by paraunused.pl
}

static int db_update_id_card(SERVICE_SVRENV* pstEnv,USERCENTER* pstUser,ThreadMgr* pstThread)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	int err = 0;
	char szSql[SQL_BUFF_LEN] = {0};
	int iRet = 0;
	int status = 0;



	snprintf(szSql,sizeof(szSql),
		"set names utf8;"
		"update %s.%s set "
		"birthday = '%s',idcard = '%s',realname = '%s',email = '%s'"
		" where username = '%s';"	
		"set names latin1;"
		" select uid into @res from %s.%s  where username = '%s';"
		" select ifnull(@res,0) as res;"
		"update %s.%s set "
		"email = '%s'"
		" where username = '%s';"		
		,
		pstEnv->pstConf->ACCDB,ACC_TABLE,
		pstUser->Birthday,pstUser->CertID,pstUser->RealName,pstUser->Email,
		pstUser->UserName,
		pstEnv->pstConf->ACCDB,ACC_TABLE,pstUser->UserName,
		
		pstEnv->pstConf->BBSDB,	BBS_TABLE,
		pstUser->Email,
		pstUser->UserName
		);

	MutiLog(MUTI_LOGLEVEL_INFO, "mysql sql:%s",szSql);
		
	mysql_select_db(pstThread->pstConn,pstEnv->pstConf->BBSDB);	
	
	err = mysql_query(pstThread->pstConn, szSql);
	if(err)
	{
		iRet = -2;
		goto final;
	}

	do {
		res = mysql_store_result(pstThread->pstConn);
		if (res)
		{
			if (mysql_field_count(pstThread->pstConn) > 0)
			{
				row = NULL;
	
				while((row = mysql_fetch_row(res)) != NULL)
				{
					if (atoi(row[0]) > 0)
					{
						iRet = 0;
					}
					else
					{
						iRet = -1;
					}
				}
				

			}
			mysql_free_result(res);
		}
		else 
		{
			if (mysql_field_count(pstThread->pstConn) == 0)
			{
			}
			else
			{
				iRet = -2;
				goto final;
			}
		}
		/* more results? -1 = no, >0 = error, 0 = yes (keep looping) */
		if ((status = mysql_next_result(pstThread->pstConn)) > 0)
		{
			err = status;
			iRet = -2;
			goto final;
		}
		
	} while (status == 0);


	
final:

	if (-2 == iRet)
	{
		MutiLog(MUTI_LOGLEVEL_INFO, "mysql err:%s",mysql_error(pstThread->pstConn));
	}

	if (CR_SERVER_LOST == err || CR_SERVER_GONE_ERROR == err)
	{
		if (pstThread->pstConn)
		{
			mysql_close(pstThread->pstConn);
			pstThread->pstConn = NULL;
		}
		pstThread->pstConn = db_init(pstEnv);
		
		MutiLog(MUTI_LOGLEVEL_INFO, "mysql reconnet");
	}
	
	return iRet;

}





static int db_ping(SERVICE_SVRENV* pstEnv,ThreadMgr* pstThread)
{
	if (pstEnv->pstAppCtx->stCurr.tv_sec - pstThread->iLastDBPing >= DB_PING_INTERVAL &&
		pstThread->pstConn)
	{
		int iRet = 0;
		
		iRet = mysql_ping ( pstThread->pstConn) ;
		if ( 0 == iRet )
		{
			return 0;
		}
		else
		{
			mysql_close(pstThread->pstConn);
			pstThread->pstConn = NULL;
		
			pstThread->pstConn = db_init(pstEnv);
			if (pstThread->pstConn)
			{
				MutiLog(MUTI_LOGLEVEL_INFO, "mysql reconnet");
			}
			else
			{
				MutiLog(MUTI_LOGLEVEL_INFO, "mysql reconnet fail");
			}
		}
	}
	
	if (pstThread->pstConn)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

#define test_setopt(A,B,C) \
  if((res = curl_easy_setopt((A),(B),(C))) != CURLE_OK) goto test_cleanup

struct WriteThis {
  char *readptr;
  size_t sizeleft;
};

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp)
{
	struct WriteThis *pooh = (struct WriteThis *)userp;
	int iLen;

	if(size*nmemb < 1)
	{
		return 0;
	}

	iLen= min(size*nmemb,pooh->sizeleft);
	if (iLen < 1)
	{
		return 0;
	}
	
	memcpy(ptr,pooh->readptr,iLen);
	pooh->readptr += iLen;
	pooh->sizeleft -= iLen;
	return iLen;

/*
	if(pooh->sizeleft) {
		*(char *)ptr = pooh->readptr[0]; // copy one single byte/
		pooh->readptr++;                 // advance pointer /
		pooh->sizeleft--;                // less data left /
		return 1;                        // we return 1 byte at a time! /
	}


	return 0;                         // no more data left to deliver 
*/
}

static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userp)
{
	//int amount = (int)size * nmemb;	//comment by paraunused.pl
	struct WriteThis *pooh = (struct WriteThis *)userp;
	int iLen;

	iLen= min(size*nmemb,pooh->sizeleft);
	if (iLen < 1)
	{
		return 0;
	}

	memcpy(pooh->readptr,ptr,iLen);
	pooh->readptr += iLen;
	pooh->sizeleft -= iLen;
	
	//printf("ret :%.*s\n", amount, (char *)ptr);

	return iLen;
}

static int web_auth_account(SERVICE_SVRENV* pstEnv,USERCENTER* pstUser,char* loginname,const  char* pszPasswd,ThreadMgr* pstThread)
{
	CURL *curl;
	CURLcode res=CURLE_OK;
	int iAuthRet = 1;
	int iUin = 0;

	struct WriteThis pooh;
	struct WriteThis poohWrite;

	pooh.readptr = pstThread->szPostData;
	pooh.sizeleft = strlen(pstThread->szPostData);

	poohWrite.readptr = pstThread->szRecvData;
	poohWrite.sizeleft = sizeof(pstThread->szRecvData);

	iUin = pstUser->Uin;
	pstUser->Uin = 0;

	memset(pstThread->szRecvData,0,sizeof(pstThread->szRecvData));

	if (pooh.sizeleft <= 0)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "pooh.sizeleft err");
		return -1;
	}

	if (0 ==  pstEnv->pstConf->WebAuthUrl[0])
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "pstEnv->pstConf->WebAuthUrl err");
		return -1;
	}

	if ( pstThread->pstCurl == NULL)
	{
		pstThread->pstCurl = curl_easy_init();
		MutiLog(MUTI_LOGLEVEL_DEBUG, "curl_easy_init");
	}

	curl = pstThread->pstCurl;

	if (!curl)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "curl_easy_init failed");
		return -1;
	}

	MutiLog(MUTI_LOGLEVEL_DEBUG, "szWebAuthPostData:%s",
			pstThread->szPostData);

	/* First set the URL that is about to receive our POST. */
	test_setopt(curl, CURLOPT_URL, pstEnv->pstConf->WebAuthUrl);

	/* Now specify we want to POST data */
	test_setopt(curl, CURLOPT_POST, 1L);

	/* Set the expected POST size */
	test_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)pooh.sizeleft);

	/* we want to use our own read function */
	test_setopt(curl, CURLOPT_READFUNCTION, read_callback);

	/* pointer to pass to our read function */
	test_setopt(curl, CURLOPT_INFILE, &pooh);

	/* Write callback */
	test_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

	/* Write callback */
	test_setopt(curl, CURLOPT_WRITEDATA, &poohWrite);

	//2秒超时
	test_setopt(curl,CURLOPT_TIMEOUT_MS,pstEnv->pstConf->WebAuthTimeoutMS);

	test_setopt(curl,CURLOPT_NOSIGNAL,1);

	if (pstEnv->pstConf->ServiceMode == SERVICE_MODE_DEVELOP)
	{
		/* get verbose debug output please */
		test_setopt(curl, CURLOPT_VERBOSE, 1L);
		

		/* include headers in the output */
		//test_setopt(curl, CURLOPT_HEADER, 1L);

		//todo:debug func
		//CURLOPT_DEBUGFUNCTION
	}

	

	/* Perform the request, res will get the return code */
	res = curl_easy_perform(curl);

	MutiLog(MUTI_LOGLEVEL_INFO, "curl_easy_perform res:%d",res);
	if (CURLE_OK ==res )
	{
		json_object *new_obj;
		json_object *ret_obj;
		json_object *uin_obj;
		
		MutiLog(MUTI_LOGLEVEL_INFO, "recv:%s",pstThread->szRecvData);

		new_obj = json_tokener_parse(pstThread->szRecvData);
		if(is_error(new_obj))
		{
			MutiLog(MUTI_LOGLEVEL_ERROR,"json_tokener_parse failed\n");
		}
		else
		{		
			MutiLog(MUTI_LOGLEVEL_DEBUG,"new_obj.to_string()=%s\n", json_object_to_json_string(new_obj));
			ret_obj = json_object_object_get(new_obj,"ret");
			uin_obj = json_object_object_get(new_obj,"accountno");
			if(is_error(ret_obj) || is_error(uin_obj))
			{
				MutiLog(MUTI_LOGLEVEL_ERROR,"json_object_object_get ret failed\n");
				
			}
			else
			{
				if (json_object_is_type(ret_obj,json_type_int))
				{
					iAuthRet = json_object_get_int(ret_obj);
				}
				else
				{
					MutiLog(MUTI_LOGLEVEL_ERROR,"json_object_is_type ret json_type_int failed\n");
				}

				if (json_object_is_type(uin_obj,json_type_int))
				{
					pstUser->Uin = json_object_get_int(uin_obj);
				}
				else
				{
					iAuthRet = 1;
					MutiLog(MUTI_LOGLEVEL_ERROR,"json_object_is_type accountno json_type_int failed\n");
				}

			}
			json_object_put(new_obj);
		}
	
		

	}
	else
	{
		if (CURLE_OPERATION_TIMEDOUT == res)
		{
			iAuthRet = AUTH_REASON_TIMEOUT;
		}
	}
		

test_cleanup:

	/* always cleanup */
	curl_easy_cleanup(curl);
	curl = NULL;
	pstThread->pstCurl = NULL;

	MutiLog(MUTI_LOGLEVEL_INFO, "account:%s ret: %d authret:%d iWebUin:%d Uin:%d",
					loginname,
					res ,iAuthRet,pstUser->Uin,iUin);

	if (CURLE_OK ==res && iAuthRet == 0)
	{
		return 0;
	}
	else
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "curl_easy_perform res:%d err:%s auth:%d",res,
				curl_easy_strerror(res),iAuthRet);
		if (iAuthRet == AUTH_REASON_TIMEOUT)
		{
			return iAuthRet;
		}
		else
		{
			return -1;
		}
	}
	UNUSED(loginname);	//add by paraunused.pl
	UNUSED(pszPasswd);	//add by paraunused.pl
}

static int web_sms_develop(SERVICE_SVRENV* pstEnv,char* loginname,int iUin,ThreadMgr* pstThread,char* pszSms)
{
	FILE *fp;
	char szID[64];
	char szFilePath[128];
	char szBuff[512];

	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) pstEnv->pstAppCtx->iId), sizeof(szID));
	snprintf(szFilePath, sizeof(szFilePath), "../log/service_svr_sms.log");

	fp = fopen(szFilePath, "a");
	if (NULL == fp)
	{
		return -1;
	}

	
	snprintf(szBuff, CCH(szBuff), "%s\tservice=%s\tacc=%s\tuin=%d\tsms=%s\n", 
		time2string(pstEnv->pstAppCtx->stCurr.tv_sec), szID, 
		loginname,iUin,pszSms);
	
	fwrite(szBuff, strlen(szBuff), 1, fp);
	
	fclose(fp);
	
	return 0;
	UNUSED(pstThread);	//add by paraunused.pl
}

static int web_sms_produce(SERVICE_SVRENV* pstEnv,char* loginname,int iUin,ThreadMgr* pstThread,char* pszRawText)
{
	CURL *curl;
	CURLcode res=CURLE_OK;
	int iSmsRet = -99;
	char* pszUrl;

	struct WriteThis pooh;
	struct WriteThis poohWrite;

	pooh.readptr = pstThread->szPostData;
	pooh.sizeleft = strlen(pstThread->szPostData);

	poohWrite.readptr = pstThread->szRecvData;
	poohWrite.sizeleft = sizeof(pstThread->szRecvData);

	memset(pstThread->szRecvData,0,sizeof(pstThread->szRecvData));

	if (pooh.sizeleft <= 0)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "pooh.sizeleft err");
		return -1;
	}

	if (pstThread->iType == THREAD_TYPE_AUTH)
	{
		if (0 ==  pstEnv->pstConf->WebAuthUrl[0])
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "pstEnv->pstConf->WebAuthUrl err");
			return -1;
		}
		pszUrl = pstEnv->pstConf->WebAuthUrl;
	}
	else
	{
		if (0 ==  pstEnv->pstConf->SmsUrl[0])
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "pstEnv->pstConf->SmsUrl err");
			return -1;
		}
		pszUrl = pstEnv->pstConf->SmsUrl;
	}

	if ( pstThread->pstCurl == NULL)
	{
		pstThread->pstCurl = curl_easy_init();
		MutiLog(MUTI_LOGLEVEL_DEBUG, "curl_easy_init");
	}

	curl = pstThread->pstCurl;

	if (!curl)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "curl_easy_init failed");
		return -1;
	}

	MutiLog(MUTI_LOGLEVEL_DEBUG, "sms szPostData:%s utf8:%s",
			pszRawText,pstThread->szPostData);

	/* First set the URL that is about to receive our POST. */
	test_setopt(curl, CURLOPT_URL, pszUrl);

	/* Now specify we want to POST data */
	test_setopt(curl, CURLOPT_POST, 1L);

	/* Set the expected POST size */
	test_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)pooh.sizeleft);

	/* we want to use our own read function */
	test_setopt(curl, CURLOPT_READFUNCTION, read_callback);

	/* pointer to pass to our read function */
	test_setopt(curl, CURLOPT_INFILE, &pooh);

	/* Write callback */
	test_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

	/* Write callback */
	test_setopt(curl, CURLOPT_WRITEDATA, &poohWrite);

	//2秒超时
	test_setopt(curl,CURLOPT_TIMEOUT_MS,pstEnv->pstConf->WebAuthTimeoutMS);

	test_setopt(curl,CURLOPT_NOSIGNAL,1L);

	if (pstEnv->pstConf->ServiceMode == SERVICE_MODE_DEVELOP)
	{
		/* get verbose debug output please */
		test_setopt(curl, CURLOPT_VERBOSE, 1L);
		

		/* include headers in the output */
		//test_setopt(curl, CURLOPT_HEADER, 1L);

		//todo:debug func
		//CURLOPT_DEBUGFUNCTION
	}

	

	/* Perform the request, res will get the return code */
	res = curl_easy_perform(curl);

	MutiLog(MUTI_LOGLEVEL_INFO, "curl_easy_perform res:%d",res);
	if (CURLE_OK ==res )
	{
		
		MutiLog(MUTI_LOGLEVEL_INFO, "recv:%s",pstThread->szRecvData);
		iSmsRet = atoi(pstThread->szRecvData);		
	}
	else
	{
		if (CURLE_OPERATION_TIMEDOUT == res)
		{
			iSmsRet = -100;
		}
	}
		

test_cleanup:

	/* always cleanup */
	curl_easy_cleanup(curl);
	curl = NULL;
	pstThread->pstCurl = NULL;

	MutiLog(MUTI_LOGLEVEL_INFO, "account:%s uin:%d ret: %d smsret:%d",
					loginname,iUin,
					res ,iSmsRet);

	if (CURLE_OK ==res && iSmsRet == 0)
	{
		return 0;
	}
	else
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "curl_easy_perform res:%d err:%s sms:%d acc:%s uin:%d",res,
				curl_easy_strerror(res),iSmsRet,loginname,iUin);
	
		return iSmsRet;
	}
	UNUSED(loginname);	//add by paraunused.pl
}

static int db_select_account(SERVICE_SVRENV* pstEnv,USERCENTER* pstUser,char* loginname,ThreadMgr* pstThread)
{
	//读数据库
	MYSQL_RES *res;
	MYSQL_ROW row;
	int err = 0;
	int iFound = 0;
	char szSql[SQL_BUFF_LEN] = {0};
	int iRet = 0;



	snprintf(szSql,sizeof(szSql),"select uid,username,password,salt from %s where username = '%s'",
		BBS_TABLE,loginname);

	MutiLog(MUTI_LOGLEVEL_INFO, "mysql sql:%s",szSql);
		
	mysql_select_db(pstThread->pstConn,pstEnv->pstConf->BBSDB);	
	
	err = mysql_query(pstThread->pstConn, szSql);
	if(err)
	{
		iRet = -2;
		goto final;
	}

	res = mysql_store_result(pstThread->pstConn);
	if(!res)
	{
		iRet = -2;
		goto final;
	}

	row = NULL;

	while((row = mysql_fetch_row(res)) != NULL)
	{
		if(!row[0])
		{
			continue;
		}

		pstUser->Uin = atoi(row[0]);
		STRNCPY(pstUser->UserName,row[1],sizeof(pstUser->UserName));
		STRNCPY(pstUser->PassWord,row[2],sizeof(pstUser->PassWord));
		STRNCPY(pstUser->Salt,row[3],sizeof(pstUser->Salt));
	}

	mysql_free_result(res);

	if (!pstUser->Uin)
	{
		iRet = -1;
		goto final;
	}

	snprintf(szSql,sizeof(szSql),"select gender,birthday from %s where username = '%s' and uid=%d",
		ACC_TABLE,loginname,pstUser->Uin);

	mysql_select_db(pstThread->pstConn,pstEnv->pstConf->ACCDB);
	
	err = mysql_query(pstThread->pstConn, szSql);
	if(err)
	{
		iRet = -2;
		goto final;
	}


	res = mysql_store_result(pstThread->pstConn);
	if(!res)
	{
		iRet = -2;
		goto final;
	}

	row = NULL;

	while((row = mysql_fetch_row(res)) != NULL)
	{
		if(!row[0])
		{
			continue;
		}

		pstUser->Gender = atoi(row[0]);
		STRNCPY(pstUser->Birthday,row[1],sizeof(pstUser->Birthday));
		iFound = 1;

	}

	mysql_free_result(res);

	if (!iFound)
	{
		iRet = -1;
		goto final;
	}

final:

	if (-2 == iRet)
	{
		MutiLog(MUTI_LOGLEVEL_INFO, "mysql err:%s",mysql_error(pstThread->pstConn));
	}
	err = mysql_errno(pstThread->pstConn);
	if (CR_SERVER_LOST == err || CR_SERVER_GONE_ERROR == err)
	{
		if (pstThread->pstConn)
		{
			mysql_close(pstThread->pstConn);
			pstThread->pstConn = NULL;
		}
		pstThread->pstConn = db_init(pstEnv);
		if (pstThread->pstConn)
		{
			MutiLog(MUTI_LOGLEVEL_INFO, "mysql reconnet");
		}
		else
		{
			MutiLog(MUTI_LOGLEVEL_INFO, "mysql reconnet fail");
		}
	}
	
	return iRet;

}




// 回复生产模式的帐号信息
static int ss_produce_account_respond(TAPPCTX *pstAppCtx, SERVICE_SVRENV *pstEnv,
										ThreadMgr *pstThread,
	                                    const SSGETACCOUNTREQ * pstReq,SSPKG * pstResPkg)
{
	/* 反向激活 */

	char loginname[INFORMER_ACCOUNT_LOGIN_NAME_LEN*2] = {0};
	char password[64] = {0};
	char szSql[SQL_BUFF_LEN] = {0};
	USERCENTER stUser;

	SSGETACCOUNTRES *pstRes = &pstResPkg->Body.GetAccountRes;
	SelectInfo * pstExtraInfo = NULL;
	SelectInfo * pstExtraInfoRes = NULL;

	UNUSED(pstThread);


	memset(&stUser,0,sizeof(stUser));
	if (pstThread->pstConn)
	{
		mysql_real_escape_string(pstThread->pstConn,loginname,pstReq->LoginName,strlen(pstReq->LoginName));
	}
	else
	{
		STRNCPY(loginname,pstReq->LoginName,sizeof(pstReq->LoginName));
	}

	snprintf(password,sizeof(password),"%s",ifm_peek_passwd(pstReq->ExtraData, pstReq->ExtraDataLen));
	pstExtraInfo = (SelectInfo *)pstReq->ExtraData;

	MutiLog(MUTI_LOGLEVEL_INFO, "ss_produce_account_respond account %s,ActiveFlag:%d,AuthMode:%d,AskFlag:%d",		
		loginname,pstExtraInfo->ActiveFlag,
		pstExtraInfo->AuthMode,pstExtraInfo->AskActiveKeyStatusFlag);

	memset(pstRes,0,sizeof(*pstRes));
	STRNCPY(pstRes->Account.Pass9Account, pstReq->LoginName,   CCH(pstRes->Account.Pass9Account));
	ifm_gen_key(pstRes->Account.PasswdHash, 
		ifm_peek_passwd(pstReq->ExtraData, pstReq->ExtraDataLen) );
	pstRes->ExtraDataLen = pstReq->ExtraDataLen;
	memcpy(pstRes->ExtraData, pstReq->ExtraData, pstRes->ExtraDataLen);
	
	pstExtraInfoRes = (SelectInfo *)pstRes->ExtraData;
	
	if( pstExtraInfo->ActiveFlag == ACTIVE_FLAG_ACTIVE_CODE)
	{
		/* 激活码的逻辑 */

		pstRes->Found = AUTH_REASON_PASSWD_ERROR;
		
		if (db_select_account(pstEnv,&stUser,loginname,pstThread) < 0)
		{			
			return 0;
		}

		//验证密码
		uc_gen_key(password, ifm_peek_passwd(pstReq->ExtraData, pstReq->ExtraDataLen),
				stUser.Salt);

		if (0 != strcmp(stUser.PassWord,password))
		{
			return 0;
		}		

		if (0 == db_select_active_key(pstEnv,&stUser,loginname,pstThread))
		{
			pstRes->Found = AUTH_IFM_ACCOUNT_ACTIVATION;
			return 0;
		}

		pstRes->Found = AUTH_IFM_REGIST_CODE_ERROR;

		if (db_use_active_key(pstEnv,&stUser,loginname,pstExtraInfo->ActiveCode,pstThread) < 0)
		{
			return 0;
		}	

		if (0 > db_select_active_key(pstEnv,&stUser,loginname,pstThread))
		{
			return 0;
		}

		db_add_acc_gift(pstEnv,&stUser,pstThread,pstExtraInfo->ActiveCode);

		pstExtraInfoRes->AskActiveKeyStatusFlag |= ACTIVE_KEY_STATUS_FLAG_YES;
		
		pstRes->Found = AUTH_REASON_SUCCEEDED;

		
		pstRes->Account.Gender = stUser.Gender;
		STRNCPY(pstRes->Account.BirthDay,stUser.Birthday,sizeof(pstRes->Account.BirthDay));

	}
	else if(pstExtraInfo->ActiveFlag == ACTIVE_FLAG_REG )
	{
		// 4.1.	反向注册				
		
		pstRes->Found = AUTH_IFM_ACCOUNT_REGISTER_ERROR;

		if (0 == db_select_account(pstEnv,&stUser,loginname,pstThread))
		{
			pstRes->Found = AUTH_IFM_ACCOUNT_EXSIT;
			return 0;
		}
		
		uc_gen_salt(stUser.Salt);
		uc_gen_key(stUser.PassWord,
					ifm_peek_passwd(pstReq->ExtraData, pstReq->ExtraDataLen),
					stUser.Salt);

		uc_gen_cookie_key(stUser.CookiePassWord,
			loginname,
			stUser.Salt);

		STRNCPY(stUser.UserName,loginname,sizeof(stUser.UserName));
		
		mysql_real_escape_string(pstThread->pstConn,
				stUser.CertID,pstExtraInfo->stAuthInfo.CertID,
				strlen(pstExtraInfo->stAuthInfo.CertID));
		
		mysql_real_escape_string(pstThread->pstConn,
				stUser.RealName,pstExtraInfo->stAuthInfo.RealName,
				strlen(pstExtraInfo->stAuthInfo.RealName));

		mysql_real_escape_string(pstThread->pstConn,
				stUser.Email,pstExtraInfo->stAuthInfo.eMail,
				strlen(pstExtraInfo->stAuthInfo.eMail));

		mysql_real_escape_string(pstThread->pstConn,
				stUser.Mobile,pstExtraInfo->stAuthInfo.Mobile,
				strlen(pstExtraInfo->stAuthInfo.Mobile));

		{
			int iLen = strlen(stUser.CertID);
			if (18 == iLen)
			{
				snprintf(stUser.Birthday,9,&stUser.CertID[6]);
			}
			else if (15 == iLen)
			{
				snprintf(stUser.Birthday,9,"19%s",&stUser.CertID[6]);
			}
			else if (0 == iLen)
			{
				stUser.Birthday[0] = 0;
			}
			else
			{
				pstRes->Found = AUTH_IFM_CERTID_ERROR;
				return 0;
			}
		}

		snprintf(szSql,sizeof(szSql),
					"begin;"
					"set @birthday='%s';"
					"set @realname='%s';"
					"set @idcard='%s';"
					"set @mobile='%s';"
					"set @pwd='%s';"
					"use %s;"
					"insert into %s (username,password,salt,email, regip, regdate) "
						"values ('%s','%s','%s','%s', '%s', UNIX_TIMESTAMP()) ;"
					"insert into %s.%s(uid,username,birthday,realname,idcard,mobile,password,regtime) "
						"select uid,username,@birthday,@realname,@idcard,@mobile,@pwd,now() from %s where username = '%s';"
					"commit;"
					,
					stUser.Birthday,
					stUser.RealName,
					stUser.CertID,
					stUser.Mobile,
					stUser.CookiePassWord,
					pstEnv->pstConf->BBSDB,
					BBS_TABLE,
					stUser.UserName,stUser.PassWord,stUser.Salt,stUser.Email,
					inet_ntoa(*(struct in_addr *)&pstExtraInfo->iIP),
					pstEnv->pstConf->ACCDB,ACC_TABLE,
					BBS_TABLE,stUser.UserName
					);

		{
			int err = 0;
			int iStatus = 0;
			MYSQL_RES* res = NULL;
			
			mysql_select_db(pstThread->pstConn,pstEnv->pstConf->BBSDB);			
			
			err = mysql_query(pstThread->pstConn, szSql);
			if(err)
			{
				MutiLog(MUTI_LOGLEVEL_INFO, "mysql err:%s",mysql_error(pstThread->pstConn));
				return 0;
			}

			do 
			{
				res = mysql_store_result(pstThread->pstConn);

				if (res)
				{
					mysql_free_result(res);
				}
				
				iStatus = mysql_next_result(pstThread->pstConn);
			} while (0 == iStatus);
			
		}

		if (db_select_account(pstEnv,&stUser,loginname,pstThread) < 0)
		{
			return 0;
		}

		//验证密码
		uc_gen_key(password, ifm_peek_passwd(pstReq->ExtraData, pstReq->ExtraDataLen),
				stUser.Salt);

		pstRes->Account.Gender = stUser.Gender;
		STRNCPY(pstRes->Account.BirthDay,stUser.Birthday,sizeof(pstRes->Account.BirthDay));	

		

		if (0 == strcmp(stUser.PassWord,password))
		{			
						//查询激活码
			if (AUTH_MODE_PERMISSION == pstExtraInfo->AuthMode)
			{
				if (0 == db_select_active_key(pstEnv,&stUser,loginname,pstThread))
				{
					pstExtraInfoRes->AskActiveKeyStatusFlag |= ACTIVE_KEY_STATUS_FLAG_YES;
				}
				else
				{
					pstRes->Found = AUTH_IFM_NEED_REGIST_CODE_ERROR;
					return 0;
				}
				
			}
			
			pstRes->Found = AUTH_REASON_SUCCEEDED;

			pstRes->Account.Gender = stUser.Gender;
			STRNCPY(pstRes->Account.BirthDay,stUser.Birthday,sizeof(pstRes->Account.BirthDay));

			
		}
		else
		{
			pstRes->Found = AUTH_REASON_PASSWD_ERROR;
		}

		pstRes->Found = AUTH_REASON_SUCCEEDED;	
	}
	else if(pstExtraInfo->ActiveFlag == ACTIVE_FLAG_AUTH)
	{
		//反向激活		

		pstRes->Found = AUTH_REASON_NO_ACCOUNT;

		
		if (pstExtraInfo->AuthFlag & AUTH_FLAG_NOX)
		{
			//todo:nox
			//运营模式
			//pstEnv->pstConf->WebAuthPost
			int iWebUin = 0;
			int iRet = 0;
		
			pstRes->Found = AUTH_REASON_NO_ACCOUNT;

			if (pstExtraInfoRes->UinType)
			{
				iWebUin = pstExtraInfoRes->stAccount.Uin;
				stUser.Uin =  pstExtraInfoRes->stAccount.Uin;
			}
			
			pstThread->szPostData[0] = 0;
			snprintf(pstThread->szPostData,sizeof(pstThread->szPostData),
					pstEnv->szWebAuthPost,pstReq->LoginName,ifm_peek_passwd(pstReq->ExtraData, pstReq->ExtraDataLen));

			iRet = web_auth_account(pstEnv,&stUser,loginname,
				ifm_peek_passwd(pstReq->ExtraData, pstReq->ExtraDataLen),pstThread);
				
			if (iRet > 0)
			{
				pstRes->Found = iRet;
				return 0;
			}
			else if (iRet < 0)
			{
				return 0;
			}
	
			if (iWebUin && (iWebUin != stUser.Uin))
			{
				pstRes->Found = AUTH_REASON_UIN_ERROR;
			}
			else
			{
				pstRes->Found = AUTH_REASON_SUCCEEDED;
				pstRes->Account.WebUin = stUser.Uin;
			}

		}
		else
		{
			if (db_select_account(pstEnv,&stUser,loginname,pstThread) < 0)
			{
				return 0;
			}


			//验证密码
			uc_gen_key(password, ifm_peek_passwd(pstReq->ExtraData, pstReq->ExtraDataLen),
					stUser.Salt);

			pstRes->Account.Gender = stUser.Gender;
			
			STRNCPY(pstRes->Account.BirthDay,stUser.Birthday,sizeof(pstRes->Account.BirthDay));

			if (0 == strcmp(stUser.PassWord,password))
			{
				db_select_passwd_card(pstEnv,&stUser,loginname,pstThread);
				
				if (stUser.PasswdCard.RowNum > 0)
				{
					pstRes->Account.PasswdCard = stUser.PasswdCard;
					pstRes->Account.UinFlag |= UIN_FLAG_PASSWD_CARD;
				}	
							
				//查询激活码
				if (AUTH_MODE_PERMISSION == pstExtraInfo->AuthMode)
				{
					if (0 == db_select_active_key(pstEnv,&stUser,loginname,pstThread))
					{	
						pstExtraInfoRes->AskActiveKeyStatusFlag |= ACTIVE_KEY_STATUS_FLAG_YES;
					}
					else
					{
						pstRes->Found = AUTH_IFM_NEED_REGIST_CODE_ERROR;
						return 0;
					}
					
				}

				pstRes->Found = AUTH_REASON_SUCCEEDED;
				
			}
			else
			{
				pstRes->Found = AUTH_REASON_PASSWD_ERROR;
			}
		}

	}
	else
	{
		pstRes->Found = AUTH_IFM_NOUSE_SERVICE;
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}



// 回复开发帐号信息
static int ss_develop_account_respond(TAPPCTX *pstAppCtx, SERVICE_SVRENV *pstEnv,
										ThreadMgr *pstThread,
	                                    const SSGETACCOUNTREQ * pstReq,SSPKG * pstResPkg)
{
	SSGETACCOUNTRES *pstRes = &pstResPkg->Body.GetAccountRes;
	int iAuthFlag;
	int i = 0;
	int k = 0;
	int m = 0;
	PASSWDCARD* pstCard = NULL;
	PASSWDCARDCELL* pstCell = NULL;
	USERCENTER stUser;
	SelectInfo *pstExtraInfoRes;
	
	bzero(pstRes, sizeof(*pstRes));
	bzero(&stUser,sizeof(stUser));
	
	pstRes->Found = 0;
	pstRes->Account.UinFlag = 0; // UIN_FLAG_GM
	STRNCPY(pstRes->Account.Pass9Account, pstReq->LoginName,
		      CCH(pstRes->Account.Pass9Account));
	STRNCPY(pstRes->Account.BirthDay, "19810216", CCH(pstRes->Account.BirthDay));
	ifm_gen_key(pstRes->Account.PasswdHash, 
		           ifm_peek_passwd(pstReq->ExtraData, pstReq->ExtraDataLen) );
	pstRes->Account.Gender = 1;
	pstRes->ExtraDataLen = pstReq->ExtraDataLen;
	memcpy(pstRes->ExtraData, pstReq->ExtraData, pstRes->ExtraDataLen);

	pstExtraInfoRes = (SelectInfo *)pstRes->ExtraData;

	if (0 == ifm_peek_auth_flag(pstReq->ExtraData,pstReq->ExtraDataLen,&iAuthFlag) )
	{
		if (iAuthFlag & AUTH_FLAG_QUICK)
		{
			pstRes->Account.UinFlag |= UIN_FLAG_QUICK;
		}
		if (iAuthFlag & AUTH_FLAG_NOX)
		{
			int iWebret = 0;
			int iWebUin = 0;
			char szData[256];
			int iRet;
			//todo:nox
			//开发模式
			pstRes->Found = AUTH_REASON_NO_ACCOUNT;

			if (pstExtraInfoRes->UinType == 0)
			{
				iWebret = 0;
				iWebUin = 1;
			}
			else
			{
				iWebUin = pstExtraInfoRes->stAccount.Uin ;
				stUser.Uin =  pstExtraInfoRes->stAccount.Uin;
			}
			
			pstThread->szPostData[0] = 0;
			snprintf(szData,sizeof(szData),
					pstEnv->szWebAuthPost,pstReq->LoginName,ifm_peek_passwd(pstReq->ExtraData, pstReq->ExtraDataLen));
			
			snprintf(pstThread->szPostData,sizeof(pstThread->szPostData),
					"%s&debugret=%d&debuguin=%d",szData,
					iWebret,iWebUin);
			
			iRet = web_auth_account(pstEnv,&stUser,(char *)pstReq->LoginName,
				ifm_peek_passwd(pstReq->ExtraData, pstReq->ExtraDataLen),pstThread);
				
			if (iRet > 0)
			{
				pstRes->Found = iRet;
				return 0;
			}
			else if (iRet < 0)
			{
				return 0;
			}
			else
			{
				//if (iWebUin && (iWebUin != stUser.Uin + 1))
				if (iWebUin && (iWebUin != stUser.Uin))
				{
					pstRes->Found = AUTH_REASON_UIN_ERROR;
				}
				else
				{
					pstRes->Found = AUTH_REASON_SUCCEEDED;
					pstRes->Account.WebUin = stUser.Uin;
				}
				MutiLog(MUTI_LOGLEVEL_INFO, "account:%s ret: %d iWebUin:%d Uin:%d",
					pstRes->Account.Pass9Account,
					pstRes->Found ,iWebUin,stUser.Uin);
				return 0;
			}
		}
	}
	
	pstRes->Account.UinFlag |= UIN_FLAG_PASSWD_CARD;

	pstCard = &pstRes->Account.PasswdCard;

	for (i = 0; i < PASSWD_CARD_MAX_ROW; i++)
	{
		for (k = 0; k < PASSWD_CARD_MAX_COLUMN; k++)
		{
			pstCell = &pstCard->Row[i].Column[k];
			pstCell->CellLen = PASSWD_CARD_MAX_CELL_LEN - 1;

			for (m = 0; m < pstCell->CellLen; m++)
			{
				pstCell->Cell[m] = '0' + RAND1(10);
			}
			
			pstCard->Row[i].ColumnNum++;
		}
		pstCard->RowNum++;
	}

	stUser.Uin = 228;
	db_select_passwd_card(pstEnv,&stUser,pstRes->Account.Pass9Account,pstThread);

	*pstCard = stUser.PasswdCard;

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstThread);	//add by paraunused.pl
}

static int ss_develop_fill_id_card_respond(TAPPCTX *pstAppCtx, SERVICE_SVRENV *pstEnv,
										ThreadMgr *pstThread,
	                                    const SSFILLIDCARDREQ* pstReq,SSPKG * pstResPkg)
{
	SSFILLIDCARDRES* pstRes = &pstResPkg->Body.FillIdCardRes;
	int iLen;
	
	bzero(pstRes, sizeof(*pstRes));
	
	
	pstRes->Result = 0;
	pstRes->Uin = pstReq->Uin;
	STRNCPY(pstRes->Account.Pass9Account,pstReq->Pass9Account,sizeof(pstRes->Account.Pass9Account));
	iLen = strlen(pstReq->IDCard);
	if (18 == iLen)
	{
		snprintf(pstRes->Account.BirthDay,9,&pstReq->IDCard[6]);
	}
	else if (15 == iLen)
	{
		snprintf(pstRes->Account.BirthDay,9,"19%s",&pstReq->IDCard[6]);
	}
	else
	{
		pstRes->Result = 1;
	}


	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstThread);	//add by paraunused.pl
}

static int ss_produce_fill_id_card_respond(TAPPCTX *pstAppCtx, SERVICE_SVRENV *pstEnv,
										ThreadMgr *pstThread,
	                                    const SSFILLIDCARDREQ* pstReq,SSPKG * pstResPkg)
{
	SSFILLIDCARDRES* pstRes = &pstResPkg->Body.FillIdCardRes;
	int iLen;
	USERCENTER stUser;
	USERCENTER stUserSelect;


	MutiLog(MUTI_LOGLEVEL_INFO, "ss_produce_fill_id_card_respond %s,%d,%s,%s,%s",		
		pstReq->Pass9Account,pstReq->Uin,pstReq->eMail,pstReq->IDCard,pstReq->Name);
	
	bzero(pstRes, sizeof(*pstRes));
	bzero(&stUser, sizeof(stUser));
	bzero(&stUserSelect, sizeof(stUserSelect));
	
	mysql_real_escape_string(pstThread->pstConn,
			stUser.RealName,pstReq->Name,
			strlen(pstReq->Name));

	{
		size_t sInLen = strlen(stUser.RealName);
		size_t sOutLen = ACCOUNT_REGIST_REALNAME_LEN;
		iconv_t cd = iconv_open("utf-8","gbk");

		
		if ((iconv_t)(-1) != cd)
		{
			char szOut[ACCOUNT_REGIST_REALNAME_LEN];			
			char* pOut = &szOut[0];
			char** ppOut = NULL;            

            char* pIn = &stUser.RealName[0];
            char** ppIn = NULL;	

			memset(szOut,0,sizeof(szOut));
			
			ppOut = &pOut;
            ppIn = &pIn;
			
            iconv(cd, ppIn, &sInLen,ppOut, &sOutLen);
			iLen = sizeof(szOut) - sOutLen;
			iLen = min((unsigned int)iLen,sizeof(stUser.RealName) -1);
			
            STRNCPY(stUser.RealName,szOut, sizeof(stUser.RealName));
			stUser.RealName[iLen] = 0;
			
            iconv_close(cd);
		}
	}

	mysql_real_escape_string(pstThread->pstConn,
			stUser.Email,pstReq->eMail,
			strlen(pstReq->eMail));

	mysql_real_escape_string(pstThread->pstConn,
			stUser.UserName,pstReq->Pass9Account,
			strlen(pstReq->Pass9Account));
	
	mysql_real_escape_string(pstThread->pstConn,
			stUser.CertID,pstReq->IDCard,
			strlen(pstReq->IDCard));	
	
	pstRes->Result = 1;
	pstRes->Uin = pstReq->Uin;
	STRNCPY(pstRes->Account.Pass9Account,pstReq->Pass9Account,sizeof(pstRes->Account.Pass9Account));
	
	iLen = strlen(pstReq->IDCard);
	if (18 == iLen)
	{
		snprintf(stUser.Birthday,9,&pstReq->IDCard[6]);
	}
	else if (15 == iLen)
	{
		snprintf(stUser.Birthday,9,"19%s",&pstReq->IDCard[6]);
	}
	else
	{
		pstRes->Result = 1;
		return 0;
	}

	if (0 == db_update_id_card(pstEnv,&stUser,pstThread))
	{
		//if (0 == db_select_account(pstEnv,&stUserSelect,stUser.UserName,pstThread))
		{
			pstRes->Result = 0;
			STRNCPY(pstRes->Account.BirthDay,stUser.Birthday,sizeof(pstRes->Account.BirthDay));
			pstRes->Account.Gender = stUser.Gender;
		}
	}
	else
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "id card err, Uin %d BirthDay %s", stUser.Uin, stUser.Birthday);
	}

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstThread);	//add by paraunused.pl
}



static int ss_get_account_req(TAPPCTX *pstAppCtx, SERVICE_SVRENV *pstEnv,ThreadMgr *pstThread,
	                            const SSGETACCOUNTREQ * pstGetAccountReq,SSPKG * pstResPkg)
{
	int iRet = -1;
	
	switch(pstEnv->pstConf->ServiceMode)
	{
	case SERVICE_MODE_DEVELOP:
		iRet = ss_develop_account_respond(pstAppCtx, pstEnv,pstThread, 
											pstGetAccountReq,pstResPkg);
		break;

	case SERVICE_MODE_PRODUCE:
		iRet = ss_produce_account_respond(pstAppCtx, pstEnv,pstThread, 
											pstGetAccountReq,pstResPkg);
		break;

	default:
		break;
	}

	pstResPkg->Head.Cmd = INFORMER_GET_ACCOUNT_RES;
	pstResPkg->Head.Magic = PROTOCOL_MAGIC; 
	pstResPkg->Head.Ver = TDR_METALIB_PROTO_SS_VERSION;
	pstResPkg->Head.HeadLen = sizeof(pstResPkg->Head);
	
	return iRet;
}

static int ss_fill_id_card_req(TAPPCTX *pstAppCtx, SERVICE_SVRENV *pstEnv,ThreadMgr *pstThread,
	                            const SSFILLIDCARDREQ * pstFillIdCardReq,SSPKG * pstResPkg)
{
	int iRet = -1;
	
	switch(pstEnv->pstConf->ServiceMode)
	{
	case SERVICE_MODE_DEVELOP:
		iRet = ss_develop_fill_id_card_respond(pstAppCtx, pstEnv,pstThread, 
											pstFillIdCardReq,pstResPkg);
		break;

	case SERVICE_MODE_PRODUCE:
		iRet = ss_produce_fill_id_card_respond(pstAppCtx, pstEnv,pstThread, 
											pstFillIdCardReq,pstResPkg);
		break;

	default:
		break;
	}

	pstResPkg->Head.Cmd = SS_FILL_ID_CARD_RES;
	pstResPkg->Head.Magic = PROTOCOL_MAGIC; 
	pstResPkg->Head.Ver = TDR_METALIB_PROTO_SS_VERSION;
	pstResPkg->Head.HeadLen = sizeof(pstResPkg->Head);
	
	return iRet;
}

static int gbk_to_utf8_in(char ** ppIn,size_t * ppInLen,char** ppOut,size_t * ppOutLen)
{
	int iConvRet = 0;
	
	//iconv_t cd = iconv_open("gbk","utf-8");
	iconv_t cd = iconv_open("utf-8","GB18030");

	iConvRet = iconv(cd, ppIn, ppInLen,ppOut, ppOutLen);
	iconv_close(cd);

	return iConvRet;
}

static int gbk_to_utf8(char *pOut,int iOutLen,char* pszIn,int iInLen)
{
	size_t sInLen = iInLen;
	size_t sOutLen = iOutLen;

	int iConvRet = 0;
	int iLen = 0;
	int iRet = 0;
	char * pszOut = pOut;

	
	
	//cd = iconv_open("utf-8//IGNORE","gbk");
	//cd = iconv_open("utf-8","GB18030");
	//cd = iconv_open("utf-8",gs_stCsEnv.pstConf->ConvertEncoding);
	iConvRet = gbk_to_utf8_in(&pszIn, &sInLen,&pOut, &sOutLen);
	

	//iConvRet = iconv(cd, &pIn, &sInLen,&pOut, &sOutLen);
	if (iConvRet < 0)
	{
		iRet = -2;
	}
	else
	{
		iLen = iOutLen - sOutLen;

		if (iLen >= 0 && iLen <iOutLen)
		{
			pszOut[iLen] = 0;
		}
	}
	

	return iRet;
}

static int ss_sms_req(TAPPCTX *pstAppCtx, SERVICE_SVRENV *pstEnv,ThreadMgr *pstThread,
	                            const SSSMSREQ * pstSmsReq,SSPKG * pstResPkg)
{
	int iRet = -101;
	char szPost[MAX_WEB_POST_RECV];
	int iLen = 0;
	int iLeft = 0;

	if (pstThread->iType != THREAD_TYPE_SMS)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "thread type err:%d",pstThread->iType);
		return -1;
	}

	iLen = strlen(pstSmsReq->Text);
	iLeft = sizeof(pstSmsReq->Text) - iLen;
	if (iLeft > 0)
	{
		snprintf((char*)&(pstSmsReq->Text[iLen]),iLeft,"%s",(char*)pstEnv->pstConf->SmsSign);
	}

	pstThread->szPostData[0] = 0;
	szPost[0] = 0;
	iLen = snprintf(szPost,sizeof(szPost),
		pstEnv->szSmsPost,SMS_PWD,pstSmsReq->Mobile,pstSmsReq->Text);

	if (0 > gbk_to_utf8((char*)pstThread->szPostData,sizeof(pstThread->szPostData),szPost,iLen))
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "gbk_to_utf8 fail:acc=%s uin=%d post=%s",
			pstSmsReq->Pass9Account,pstSmsReq->Uin,szPost);
		return -1;
	}
	//snprintf(pstThread->szPostData,sizeof(pstThread->szPostData),
		//pstEnv->szSmsPost,SMS_PWD,pstSmsReq->Mobile,pstSmsReq->Text);
	
	switch(pstEnv->pstConf->ServiceMode)
	{
	case SERVICE_MODE_DEVELOP:
		//todo:sms
		//写入到文件,77开发工具可访问
		iRet = 0;
		web_sms_develop(pstEnv,(char*)pstSmsReq->Pass9Account,pstSmsReq->Uin,pstThread,(char*)pstSmsReq->Text);
		
		break;

	case SERVICE_MODE_PRODUCE:
		iRet = web_sms_produce(pstEnv,(char*)pstSmsReq->Pass9Account,pstSmsReq->Uin,pstThread,(char*)pstSmsReq->Text);
		
		break;

	default:
		return -1;//不返回ss 协议
		break;
	}

	pstResPkg->Head.Cmd = SS_SMS_RES;
	pstResPkg->Head.Magic = PROTOCOL_MAGIC; 
	pstResPkg->Head.Ver = TDR_METALIB_PROTO_SS_VERSION;
	pstResPkg->Head.HeadLen = sizeof(pstResPkg->Head);

	pstResPkg->Body.SmsRes.Result = iRet;
	STRNCPY(pstResPkg->Body.SmsRes.Pass9Account,pstSmsReq->Pass9Account,sizeof(pstResPkg->Body.SmsRes.Pass9Account));
	pstResPkg->Body.SmsRes.Uin = pstSmsReq->Uin;
	
	return 0;//返回ss协议
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

void* thread_proc(void* pvArg)
{
	int iRet;
	ThreadMgr *pstThread;
	char szMsg[MAX_MSG_SIZE];
	int iLen;
	int iIdle = 0;
	SSPKG stPkg;
	SERVICE_SVRENV *pstEnv = NULL;
	SSPKG stPkgRes;
	TDRDATA stNet;
	char *pszBuff = NULL;
	int iPing = 1;
	

	pstThread = (ThreadMgr *)pvArg;
	pstEnv = &gs_stEnv;


	while(1)
	{
		if( pstThread->cIsExit )
			break;

		iLen = sizeof(szMsg);
		iRet = tbus_channel_recv(pstThread->pstChannel1, szMsg, &iLen, 0);
		if (0 != iRet)
		{
			if ((int)TBUS_ERR_CHANNEL_EMPTY == iRet)
			{
				iIdle++;
				if (iIdle > gs_stAppCtx.iIdleCount)
				{
					iIdle = 0;
					usleep(gs_stAppCtx.iIdleSleep*1000);
				}
			}
			else
			{
				MutiLog(MUTI_LOGLEVEL_ERROR, "tbus_channel_recv error Ret %d", iRet);
			}
			
			continue;
		}

		iIdle = 0;
		iLen -= (int)sizeof(ChannelAppend);
		
		if (iLen <= 0)
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "tbus_channel_recv error iLen %d", iLen);
			continue;
		}
		
		pszBuff = &szMsg[sizeof(ChannelAppend)];
		
		iRet = ss_unpack(pstEnv->pstSSHan, pszBuff, iLen, &stPkg, 0);
		if(iRet < 0)
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "ss_unpack error Ret %d", iRet);
			continue;
		}

		if (stPkg.Head.Cmd == INFORMER_GET_ACCOUNT_REQ )
		{			
			SelectInfo * pstExtraInfo = (SelectInfo *)stPkg.Body.GetAccountReq.ExtraData;
			if (pstExtraInfo->AuthFlag & AUTH_FLAG_NOX)
			{
				//不db_ping
				iPing = 0;
			}			
		}
		else if (stPkg.Head.Cmd == SS_SMS_REQ)
		{
			iPing = 0;
		}
		
		if (iPing)
		{
			iRet = db_ping(pstEnv,pstThread);	
		}

		if(iRet < 0)
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "db_ping error Ret %d", iRet);
			continue;
		}	
		
		switch(stPkg.Head.Cmd)
		{
		case SS_CDKEY_REQ:
			iRet = ss_cdkey_req(pstEnv,pstThread,&stPkg.Body.SSCDKeyReq,&stPkgRes);
			break;
			
		case INFORMER_GET_ACCOUNT_REQ:
			iRet = ss_get_account_req(pstEnv->pstAppCtx,pstEnv,pstThread,&stPkg.Body.GetAccountReq,&stPkgRes);
			break;
			
		//sms
		case SS_SMS_REQ:
			iRet = ss_sms_req(pstEnv->pstAppCtx,pstEnv,pstThread,&stPkg.Body.SmsReq,&stPkgRes);
			break;

		case SS_FILL_ID_CARD_REQ:
			iRet = ss_fill_id_card_req(pstEnv->pstAppCtx,pstEnv,pstThread,&stPkg.Body.FillIdCardReq,&stPkgRes);
			break;
		
		default:
			MutiLog(MUTI_LOGLEVEL_ERROR, "unknown cmd %d", stPkg.Head.Cmd);
			iRet = -1;
			break;
		}

		//消息处理并返回，返回消息打包在&szMsg[sizeof(ChannelAppend)]中，然后返回szMsg, Len 为消息长度+ sizeof(ChannelAppend)
		if (0 != iRet)
		{			
			continue;
		}

		stNet.iBuff = sizeof(szMsg) - iLen;
		stNet.pszBuff = pszBuff;
		
		iRet = ss_pack(pstEnv->pstSSHan, &stNet, &stPkgRes, 0);
		if(iRet < 0)
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "ss_pack error Ret %d:%s", iRet,tdr_error_string(iRet));
			
			continue;
		}

		iLen = sizeof(ChannelAppend) + stNet.iBuff;
		
		iRet = tbus_channel_send(pstThread->pstChannel1, szMsg, iLen, 0);
		if (0 != iRet)
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "tbus_channel_send error Ret %d", iRet);
			continue;
		}
	}


	return (void*)0;
}

int thread_init_one(SERVICE_SVRENV *pstEnv, ThreadMgr *pstThread, int iIdx)
{
	int iKey;
	
	if (0 == pstEnv->pstConf->ChannelKeyStart)
	{
		iKey = ftok(pstEnv->pstAppCtx->pszPidFile, iIdx);
	}
	else
	{
		iKey = pstEnv->pstConf->ChannelKeyStart + iIdx;
	}

	pstThread->iIdx = iIdx;
	if (0 > tbus_open_channel(&pstThread->pstChannel0,  pstEnv->pstAppCtx->iId, LOCAL_BUS_ADDR_START+iIdx,  iKey, CHANNEL_SIZE))
	{
		return -1;
	}

	if (0 > tbus_open_channel(&pstThread->pstChannel1, LOCAL_BUS_ADDR_START+iIdx, pstEnv->pstAppCtx->iId,   iKey, CHANNEL_SIZE))
	{
		return -1;
	}

	pstThread->cIsValid = 1;
	
	if( pthread_create(&pstThread->tid, NULL, thread_proc, pstThread)<0 )
	{
		return -1;
	}

	if (iIdx < pstEnv->pstConf->MutiThreadNum)
	{
		pstThread->pstConn = db_init(pstEnv);
	

		if (!pstThread->pstConn)
		{
			//return -1;
		}

		pstThread->iLastDBPing = pstEnv->pstAppCtx->stCurr.tv_sec;

		pstThread->iType = THREAD_TYPE_AUTH;
	}
	else
	{
		pstThread->pstConn = NULL;
		pstThread->iType = THREAD_TYPE_SMS;
	}	

	pstThread->pstCurl = curl_easy_init();

	if (!pstThread->pstCurl)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "curl_easy_init failed");
		return -1;
	}

	MutiLog(MUTI_LOGLEVEL_INFO, "thread init,type:%d",pstThread->iType);

	return 0;
}

int thread_init(SERVICE_SVRENV *pstEnv)
{
	int i;

	pstEnv->iThread = pstEnv->pstConf->MutiThreadNum + pstEnv->pstConf->SmsThreadNum;
	if (0 == pstEnv->iThread ||MAX_THREAD < pstEnv->iThread)
	{
		pstEnv->iThread = MAX_THREAD;
		return -1;
	}

	for (i=0; i<pstEnv->iThread; i++)
	{
		if (0 > thread_init_one(pstEnv, &pstEnv->astThread[i], i))
		{
			return -1;
		}
	}
	
	return 0;
}

int db_check(SERVICE_SVRENV *pstEnv)
{
	MYSQL_RES *res;
	//MYSQL_ROW row;
	int err = 0;
	char szSql[1024];
	
	
	MYSQL *conn = mysql_init(NULL);
	if(!conn)
	{
		err = -1;
		goto final;
	}

	if(!mysql_real_connect(
		conn,
		pstEnv->pstConf->MysqlIP,
		pstEnv->pstConf->MysqlUser,
		pstEnv->pstConf->MysqlPwd,
		pstEnv->pstConf->ACCDB,
		pstEnv->pstConf->MysqlPort,
		NULL,
		CLIENT_COMPRESS|CLIENT_MULTI_STATEMENTS))
	{
		err = -2;
		goto final;
	}

	snprintf(szSql,sizeof(szSql),"SELECT * FROM %s LIMIT 1;",ACC_TABLE);
	err = mysql_query(conn, szSql);
	if(err)
	{
		err = -3;
		goto final;
	}

	res = mysql_store_result(conn);
	if(!res)
	{
		err = -4;
		goto final;
	}

	mysql_free_result(res);

	snprintf(szSql,sizeof(szSql),"SELECT * FROM %s.%s LIMIT 1;",
		pstEnv->pstConf->BBSDB,BBS_TABLE);
	
	err = mysql_query(conn, szSql);
	if(err)
	{
		err = -5;
		goto final;
	}

	res = mysql_store_result(conn);
	if(!res)
	{
		err = -6;
		goto final;
	}


	if (conn)
	{
		mysql_close(conn);
	}



	
final:

	if (err)
	{
		printf("mysql err:%s\n",mysql_error(conn));
	}
	
	return err;
}

int service_svr_init(TAPPCTX *pstAppCtx, SERVICE_SVRENV *pstEnv)
{
	int i ;
	
	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		return -1;
	}
	pstEnv->pstConf = (SERVICE_SVRCONF *)pstAppCtx->stConfData.pszBuff;

	pstEnv->pstRunStat = (SERVICE_SVRRUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
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

	{
		int iRet = db_check(pstEnv);
		if (0 > iRet)
		{
			printf("db_check fail:%d\n",iRet);
			//return -1;
		}
	}

	if (0 > thread_init(pstEnv))
	{
		printf("thread_init fail\n");
		//return -1;	
	}

	if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
		printf("curl_global_init() failed\n");
		return -1;
	}

	STRNCPY(pstEnv->szWebAuthPost,pstEnv->pstConf->WebAuthPost,
			sizeof(pstEnv->szWebAuthPost));
	
	for (i = 0;i < (int)sizeof(pstEnv->szWebAuthPost);i++)
	{
		if (0 == pstEnv->szWebAuthPost[i])
		{
			break;
		}

		if (pstEnv->szWebAuthPost[i] == ':')
		{
			pstEnv->szWebAuthPost[i] = '&';
		}
	}

	STRNCPY(pstEnv->szSmsPost,pstEnv->pstConf->SmsPost,
			sizeof(pstEnv->szSmsPost));
	
	for (i = 0;i < (int)sizeof(pstEnv->szSmsPost);i++)
	{
		if (0 == pstEnv->szSmsPost[i])
		{
			break;
		}

		if (pstEnv->szSmsPost[i] == ':')
		{
			pstEnv->szSmsPost[i] = '&';
		}
	}
	

	srand(time(NULL));
	
	printf("service_svr start\n");
	MutiLog(MUTI_LOGLEVEL_INFO, "service_svr start");

	return 0;
}

int store_appendinfo(TAPPCTX *pstAppCtx, ChannelAppend *pstChannelAppend)
{
	int iLen;

	iLen = sizeof(pstChannelAppend->sAppend);
	
	return tbus_get_pkg_header(pstAppCtx->iBus, pstChannelAppend->sAppend, &iLen);
}

int restore_appendinfo(TAPPCTX *pstAppCtx, ChannelAppend *pstChannelAppend)
{
	int iLen;

	iLen = sizeof(pstChannelAppend->sAppend);
	
	return tbus_set_pkg_header(pstAppCtx->iBus, pstChannelAppend->sAppend, iLen);
}

char* get_debug_mobile_list(SERVICE_SVRENV *pstEnv)
{
	static char szMobileList[2048];
	BATCHSMSCFG *pstBatchSmsCfg = &pstEnv->pstConf->BatchSmsCfg; 
	int i = 0;
	int iCount = 0;
	char *pszChar = NULL;
	int iLeft;
	int iLen = 0;
	int iFirst = 1;

	szMobileList[0] = 0;

	pszChar = (char* ) szMobileList;
	iLeft = sizeof(szMobileList);
	
	for (i = pstEnv->iBatchSmsOffset;i < pstBatchSmsCfg ->DebugMobileNum;i++)
	{
		if (iCount >= pstBatchSmsCfg->MobileNumPerSend)
		{
			break;
		}
		
		if (iFirst)
		{
			iLen = snprintf(pszChar,iLeft,"%s",pstBatchSmsCfg->DebugMobile[i]);
			iFirst = 0;
		}
		else
		{
			iLen = snprintf(pszChar,iLeft,",%s",pstBatchSmsCfg->DebugMobile[i]);
		}
		
		iLeft -= iLen;
		pszChar += iLen;
		iCount++;
		
		if (iLen >= iLeft)
		{
			break;
		}
		
		
	}

	pstEnv->iBatchSmsOffset += iCount;

	
	return szMobileList;
}

char* get_db_mobile_list(SERVICE_SVRENV *pstEnv,ThreadMgr* pstThread)
{
	static char szMobileList[2048];
	BATCHSMSCFG *pstBatchSmsCfg = &pstEnv->pstConf->BatchSmsCfg; 
	int iCount = 0;
	char *pszChar = NULL;
	int iLeft;
	int iLen = 0;
	int iRet = 0;
	char szDBName[32];
	char szTblName[64];
	int iRegionID = 0;
	//读数据库
	MYSQL_RES *res;
	MYSQL_ROW row;
	int err = 0;
	char szSql[SQL_BUFF_LEN] = {0};
	int iFirst = 1;
	int iFirstUin = 0;
	int iLastUin = 0;
	//char szCmd[1024];

	szMobileList[0] = 0;

	pszChar = (char* ) szMobileList;
	iLeft = sizeof(szMobileList);

	GET_REGION_ID(iRegionID, pstEnv->pstAppCtx->iId);
	snprintf(szDBName,sizeof(szDBName),"account_%d",iRegionID);
	snprintf(szTblName,sizeof(szTblName),"Account_%d",pstBatchSmsCfg->BatchSms[pstEnv->iBatchSmsIdx].SmsSendTime);

	
	if (!pstThread->pstConn)
	{
		pstThread->pstConn = db_init(pstEnv);
	}
	else
	{
		db_ping(pstEnv,pstThread);
	}

	if (!pstThread->pstConn)
	{
		MutiLog(MUTI_LOGLEVEL_INFO, "db_ping fail");
		goto final;
	}


	if(SMS_DB_STAT_INIT == pstEnv->iBatchSmsDBStat)
	{
		//iRet = system("chmod 755 ../cfg/sms_db_prepare.sh");
		/*
		iRet = system("ls");
		if (iRet != 0)
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "system fail:%d err:%d errstr:%s",iRet,errno,strerror(errno));
			goto final;
		}
		
		
		snprintf(szCmd,sizeof(szCmd),"sh ../cfg/sms_db_prepare.sh %s %s",szDBName,szTblName);

		iRet = system(szCmd);
		if (iRet != 0)
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "system fail:%d ",iRet);
			goto final;
		}
		*/
		pstEnv->iBatchSmsDBStat = SMS_DB_STAT_PREPARE;

		MutiLog(MUTI_LOGLEVEL_INFO, "srcdb:%s dsttbl:%s prepared",szDBName,szTblName);
		
	}




	snprintf(szSql,sizeof(szSql),"select Mobile,Uin from %s where Send = 0 order by Uin limit %d",
		szTblName,100);

	MutiLog(MUTI_LOGLEVEL_INFO, "mysql sql:%s",szSql);
		
	mysql_select_db(pstThread->pstConn,pstEnv->pstConf->ACCDB);	
	
	err = mysql_query(pstThread->pstConn, szSql);
	if(err)
	{
		iRet = -2;
		goto final;
	}

	res = mysql_store_result(pstThread->pstConn);
	if(!res)
	{
		iRet = -2;
		goto final;
	}

	row = NULL;

	while((row = mysql_fetch_row(res)) != NULL)
	{
		iLastUin = atoi(row[1]);
		
		if(!row[0])
		{
			continue;
		}
		
		if (iFirst)
		{
			iLen = snprintf(pszChar,iLeft,"%s",row[0]);
			iFirst = 0;
			iFirstUin = atoi(row[1]);
		}
		else
		{
			iLen = snprintf(pszChar,iLeft,",%s",row[0]);
		}

		iLeft -= iLen;
		pszChar += iLen;
		iCount++;
		
		if (iLen >= iLeft)
		{
			break;
		}
		
		
	}

	mysql_free_result(res);


	snprintf(szSql,sizeof(szSql),"update %s set Send = 1 where Uin >= %d and Uin <= %d",
		szTblName,iFirstUin,iLastUin);

	mysql_query(pstThread->pstConn, szSql);

	MutiLog(MUTI_LOGLEVEL_INFO, "mysql sql:%s",szSql);


	pstEnv->iBatchSmsOffset += iCount;
	pstEnv->iBatchSmsLastCount = iCount;

	
	return szMobileList;

final:
	if (pstThread->pstConn)
	{
		err = mysql_errno(pstThread->pstConn);
		if (-2 == iRet)
		{
			MutiLog(MUTI_LOGLEVEL_INFO, "mysql err:%s",mysql_error(pstThread->pstConn));
		}
	}
	
	
	
	/*
	if (CR_SERVER_LOST == err || CR_SERVER_GONE_ERROR == err)
	{
		if (pstThread->pstConn)
		{
			mysql_close(pstThread->pstConn);
			pstThread->pstConn = NULL;
		}
		pstThread->pstConn = db_init(pstEnv);
		if (pstThread->pstConn)
		{
			MutiLog(MUTI_LOGLEVEL_INFO, "mysql reconnet");
		}
		else
		{
			MutiLog(MUTI_LOGLEVEL_INFO, "mysql reconnet fail");
		}
	}
	*/
	
	return szMobileList;
}

int batch_sms_send(SERVICE_SVRENV *pstEnv)
{
	BATCHSMSCFG *pstBatchSmsCfg = &pstEnv->pstConf->BatchSmsCfg; 	
	char* pszSmsText = (char*) pstBatchSmsCfg->BatchSms[pstEnv->iBatchSmsIdx].SmsText;
	ThreadMgr stThread;//造一个假的
	int iRet = 0;
	//char szPost[MAX_WEB_POST_RECV];
	int iLen = strlen(pszSmsText);
	char szSmsTextUtf8[512]; // 短信息内容
	char *pszMobileList = NULL;
	int iAll = 0;
	int iInterval = 0;
	char szText[SHORT_DESC_LEN];

	memset(&stThread,0,sizeof(stThread));

	if (pstBatchSmsCfg->SendTimesPerMinute <= 0)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "SendTimesPerMinute err:%d",
			pstBatchSmsCfg->SendTimesPerMinute);
		return -1;
	}

	iInterval = 60/pstBatchSmsCfg->SendTimesPerMinute;

	if (pstEnv->iBatchSmsLastSend + iInterval > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return 0;//没到时间
	}

	snprintf(szText,sizeof(szText),"%s%s",pszSmsText,pstEnv->pstConf->SmsSign);

	iLen= strlen(szText);

	pstEnv->iBatchSmsLastSend = pstEnv->pstAppCtx->stCurr.tv_sec;
	

	stThread.iType= THREAD_TYPE_SMS;

	if (0 > gbk_to_utf8((char*)szSmsTextUtf8,sizeof(szSmsTextUtf8),szText,iLen))
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "gbk_to_utf8 fail:text=%s",
			szText);
		return -1;
	}

	if (pstBatchSmsCfg->DebugMode)
	{
		pszMobileList = get_debug_mobile_list(pstEnv);
		if (pstEnv->iBatchSmsOffset >= pstBatchSmsCfg->DebugMobileNum - 1)
		{
			iAll = 1;
		}
	}
	else
	{
		//数据库中取手机号
		pszMobileList = get_db_mobile_list(pstEnv,&stThread);
		if (pstEnv->iBatchSmsLastCount == 0)
		{
			iAll = 1;
		}
	}

	if (iAll)
	{
		MutiLog(MUTI_LOGLEVEL_INFO, "send all batch sms idx:%d count:%d fail:%d",
			pstEnv->iBatchSmsIdx,
			pstEnv->iBatchSmsOffset,
			pstEnv->iBatchSmsFailCount);
	}

	if ((0 == pszMobileList[0] && iAll))
	{
		return 1;
	}


	if (!pszMobileList || 0 == pszMobileList[0])
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "pszMobileList empty");
		return -1;
	}

	//szPost[0] = 0;
	iLen = snprintf(stThread.szPostData,sizeof(stThread.szPostData),
		pstEnv->szSmsPost,SMS_PWD,pszMobileList,(char *)szSmsTextUtf8);

	
	iRet = web_sms_produce(pstEnv,"batch sms",pstEnv->iBatchSmsIdx,&stThread,szText);
	if (iRet != 0)
	{
		//todo:log
		MutiLog(MUTI_LOGLEVEL_ERROR, "web_sms_produce fail:%d",iRet);
		pstEnv->iBatchSmsFailCount += pstEnv->iBatchSmsLastCount;
		return -1;
	}

	

	if (iAll)
	{
		return 1;
	}
	else
	{	
		return 0;
	}
}

int proc_multi_bus2channel(TAPPCTX *pstAppCtx, SERVICE_SVRENV *pstEnv)
{
	int i;
	int iBusy;
	int iRet;
	int iSrc;
	int iDst;
	TDRDATA stData;
	char szMsg[MAX_MSG_SIZE];
	static int iRobin = 0;
	static int iSmsRobin = 0;
	unsigned short usCmd = 0;
	BATCHSMSCFG *pstBatchSmsCfg = &pstEnv->pstConf->BatchSmsCfg; 

	iBusy = 0;
	for (i=0; i<pstEnv->iThread; i++)
	{
		iSrc = TBUS_ADDR_ALL;
		iDst = pstEnv->pstAppCtx->iId;
		stData.iBuff = sizeof(szMsg) - sizeof(ChannelAppend);
		stData.pszBuff = (char *)szMsg + sizeof(ChannelAppend);
		
		iRet = tbus_recv(pstAppCtx->iBus, &iSrc, &iDst, stData.pszBuff, (size_t *)&stData.iBuff, 0);
		if (0 == iRet)
		{
			iBusy = 1;

			memcpy(&usCmd,((char *)stData.pszBuff) + sizeof(usCmd),sizeof(usCmd));
			usCmd = NTOHS(usCmd);
			
			
			if (0 != store_appendinfo(pstAppCtx, (ChannelAppend *)szMsg))
			{
				MutiLog(MUTI_LOGLEVEL_ERROR, "store_appendinfo error");
			}
			
			stData.iBuff += sizeof(ChannelAppend);
			stData.pszBuff = (char *)szMsg;
			

			MutiLog(MUTI_LOGLEVEL_DEBUG, "cmd:%d",(int)usCmd);


			iRet = -9;
			
			//sms		
			if (usCmd != SS_SMS_REQ)
			{
				if (pstEnv->pstConf->MutiThreadNum >  0)
				{
					//根据策略选择一个线程,  暂时使用robin-round
					iRobin++;
					iRobin %= pstEnv->pstConf->MutiThreadNum;
					
					iRet = tbus_channel_send(pstEnv->astThread[iRobin].pstChannel0, 
											stData.pszBuff, stData.iBuff, 0);
				}
			}
			else 
			{
				if (pstEnv->pstConf->SmsThreadNum > 0)
				{
					iSmsRobin++;
					
					iSmsRobin %= pstEnv->pstConf->SmsThreadNum;
					
					iRet = tbus_channel_send(pstEnv->astThread[iSmsRobin + pstEnv->pstConf->MutiThreadNum].pstChannel0, 
											stData.pszBuff, stData.iBuff, 0);
				}
			}
			
			if (0 != iRet)
			{
				MutiLog(MUTI_LOGLEVEL_ERROR, "tbus_channel_send error ret %d", iRet);
			}
		}
		else
		{
			break;
		}
	}

	if (pstBatchSmsCfg->BatchSmsNum > 0 && 
		pstEnv->iBatchSmsIdx < MAX_BATCH_SMS_NUM &&
		pstEnv->iBatchSmsIdx < pstBatchSmsCfg->BatchSmsNum)
	{
		if (pstAppCtx->stCurr.tv_sec >=
			pstBatchSmsCfg->BatchSms[pstEnv->iBatchSmsIdx].SmsSendTime)
		{
			if (pstBatchSmsCfg->BatchSms[pstEnv->iBatchSmsIdx].SmsSendTime +
				pstBatchSmsCfg->BatchSms[pstEnv->iBatchSmsIdx].InvalidTime >=
				pstAppCtx->stCurr.tv_sec)
			{
				//有效时间内
				iRet = batch_sms_send(pstEnv);
			}
			else
			{
				iRet = 1;//直接下一批
			}
				
			if (1 == iRet)
			{
				//当前批次全部发了
				pstEnv->iBatchSmsIdx++;
				pstEnv->iBatchSmsOffset = 0;
				pstEnv->iBatchSmsDBStat = SMS_DB_STAT_INIT;
			}
			else if (iRet < 0)
			{
				//some thing wrong
			}
		}
		else
		{
			//没到时间
		}
	}
	
	return iBusy;	
}

int channel2bus_one(TAPPCTX *pstAppCtx, SERVICE_SVRENV *pstEnv, ThreadMgr *pstThread)
{
	int iLen;
	int iRet;
	char szMsg[MAX_MSG_SIZE];
	int iSrc, iDst;
	int iBusy = 0;

	iLen = sizeof(szMsg);
	iRet = tbus_channel_recv(pstThread->pstChannel0, szMsg, &iLen, 0);
	if (0 == iRet)
	{
		iBusy = 1;

		if (iLen <= (int)sizeof(ChannelAppend))
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "tbus_channel_recv len %d", iLen);
			return iBusy;
		}

		if (0 != restore_appendinfo(pstAppCtx, (ChannelAppend *)szMsg))
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "restore_appendinfo error");
			return iBusy;
		}

		iSrc = pstAppCtx->iId;
		iDst = 0;
				
		iRet = tbus_backward(pstAppCtx->iBus, &iSrc, &iDst, &szMsg[sizeof(ChannelAppend)], iLen-sizeof(ChannelAppend), 0);

		if (0 != iRet)
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "tbus_backward error ret %d", iRet);
		}
	}
	else if ((int)TBUS_ERR_CHANNEL_EMPTY != iRet)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "tbus_channel_recv error ret %d", iRet);
	}
		
	return iBusy;	
	UNUSED(pstEnv);	//add by paraunused.pl
}

int proc_multi_channel2bus(TAPPCTX *pstAppCtx, SERVICE_SVRENV *pstEnv)
{
	int i;
	int iBusy = 0;

	for (i=0; i<pstEnv->iThread; i++)
	{
		iBusy = channel2bus_one(pstAppCtx, pstEnv, &pstEnv->astThread[i]);
	}

	return iBusy;
}

int service_svr_proc(TAPPCTX *pstAppCtx, SERVICE_SVRENV *pstEnv)
{
	int iBusy=0;
	
	//如果进程还没不是退出状态，转发bus的消息到channel
	if (!tapp_is_exit())
	{
		iBusy = proc_multi_bus2channel(pstAppCtx, pstEnv);
	}

	/*读取内部Channel消息转发到bus*/
	iBusy = proc_multi_channel2bus(pstAppCtx, pstEnv);

	if (iBusy)
		return 0;
	else
		return -1;
}

int service_svr_reload(TAPPCTX *pstAppCtx, SERVICE_SVRENV *pstEnv)
{
	SERVICE_SVRCONF   *pstPreConf;
	
	pstPreConf = (SERVICE_SVRCONF   *)pstAppCtx->stConfPrepareData.pszBuff;
	
	printf("service_svr reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "service_svr reload");

	return 0;
}

int channel2bus_one_recvall(TAPPCTX *pstAppCtx, SERVICE_SVRENV *pstEnv, ThreadMgr* pstThread)
{
	while(channel2bus_one(pstAppCtx, pstEnv, pstThread))
	{
		;
	}

	return 0;
}

int service_svr_fini(TAPPCTX *pstAppCtx, SERVICE_SVRENV *pstEnv)
{
	int i;
	void* pvRet;
	ThreadMgr* pstThread;

	assert(pstAppCtx);

	for(i=0; i<pstEnv->iThread; i++)
	{
		pstThread	=	&pstEnv->astThread[i];

		if( pstThread->cIsValid )
			pstThread->cIsExit	=	1;
	}

	for(i=0; i<pstEnv->iThread; i++)
	{
		pstThread	=	&pstEnv->astThread[i];

		if( pstThread->cIsValid )
		{
			channel2bus_one_recvall(pstAppCtx, pstEnv, pstThread);	
			pthread_join(pstThread->tid, &pvRet);
			channel2bus_one_recvall(pstAppCtx, pstEnv, pstThread);	
		}
	}

	for (i=0; i<pstEnv->iThread; i++)
	{
		pstThread	=	&pstEnv->astThread[i];
		tbus_close_channel(&pstThread->pstChannel0);
		tbus_close_channel(&pstThread->pstChannel1);
	}

	for (i=0; i<pstEnv->iThread; i++)
	{
		pstThread	=	&pstEnv->astThread[i];
		if (pstThread->pstConn)
		{
			mysql_close(pstThread->pstConn);
			pstThread->pstConn = NULL;
		}
	}

	curl_global_cleanup();

	printf("service_svr stop\n");
	MutiLog(MUTI_LOGLEVEL_INFO, "service_svr stop");

	pthread_mutex_destroy(&pstEnv->stLogMutex);

	return 0;
}

int service_svr_tick(TAPPCTX *pstAppCtx, SERVICE_SVRENV *pstEnv)
{
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}

int service_svr_stop(TAPPCTX *pstAppCtx, SERVICE_SVRENV *pstEnv)
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

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)service_svr_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_service_svr;
	gs_stAppCtx.stConfData.pszMetaName = "service_svrconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);
		
	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)service_svr_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)service_svr_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)service_svr_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)service_svr_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)service_svr_stop;
	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	tapp_def_fini(&gs_stAppCtx, pvArg);
	
	return iRet;
}


