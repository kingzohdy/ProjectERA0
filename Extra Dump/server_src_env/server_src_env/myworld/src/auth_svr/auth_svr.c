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

#include "auth_svr_conf_desc.h"
#include "tapp/tapp.h"
#include "tdr/tdr.h"
#include "tlog/tlog.h"
#include "version.h"
#include "cs_net.h"
#include "entity.h"
#include "comm_misc.h"
#include "tbus/tbus.h"
#include "grav_rsa.h"
#include "md5.h"
#include "tormapi/torm_protocol_api.h"
#include "comm/shtable.h"
#include "auth_svr.h"
#include "ptgenimg.h"
#include "comm/tconvert.h"

extern RSA_PUBLIC_KEY * IDRSAPublicKey;


char * rand_str(char* pszBuff, int iLen);

#define LOOP_NUM 100
#define CS_MAX_MSG CS_MAX_BODY+sizeof(CSPKGHEAD)+sizeof(TFRAMEHEAD)

#define UNUSED(arg) ((void)arg)

#define OBJ_SHM_VER 0x0002

char szWatch[ TORM_MAX_ASYNCALLBACK_DATA_LEN - sizeof(TormCallback) ];

#define ACCOUNT_LIMIT_NUM 2000000
#define ACCOUNT_LIMIT_BUCKET_NUM (ACCOUNT_LIMIT_NUM)
#define CACHE_FLUSH_PERCENT 80

#define PASSWD_CARD_LIMIT_NUM 20000
#define PASSWD_CARD_LIMIT_BUCKET_NUM (PASSWD_CARD_LIMIT_NUM)
#define PASSWD_CARD_LIMIT_TIME  3600

#define SIGN_INFO_CACHENUM 100000
#define SIGN_INFO_CACHEBUCKET_NUM (SIGN_INFO_CACHENUM * 3)
#define SIGN_INFO_CACHETIME  3600

#define WEB_SIGN_CLIENT_NUM 128
#define WEB_SIGN_CLIENT_BUCKET_NUM (WEB_SIGN_CLIENT_NUM)

// 超过5小时的account数据就从cache中删除。
#define MAX_CACHE_FLUSH_TIME (5 * 3600)

//封号封10分钟
#define ACCOUNT_LIMIT_TIME  600

#define MAX_TRY_NUM 10

// 哈希表相关数据
typedef struct tagAccountLimit
{
	char szAccountName[ACCOUNT_LOGIN_NAME_LEN]; 
	tdr_ip_t IP;
	int iTryNum;
	time_t tTime;
}AccountLimit;

#define SIGN_INFO_STAT_INIT  0
#define SIGN_INFO_STAT_READY  1
#define SIGN_INFO_STAT_GETED 2
#define MD5_DIGEST_LENGTH 16




// 哈希表相关数据
typedef struct tagSignInfoCache
{
	int iWebUin;
	char szAccountName[ACCOUNT_LOGIN_NAME_LEN];
	AUTHINFO stAuthInfo;
	tdr_ip_t IP;
	time_t tTime;
	int iSignInfoStat;//签名信息状态SIGN_INFO_STAT_INIT 0初始化 1已生成签名信息但未被获取 2签名信息已被获取
	ACCOUNTAUTHINFO stAccountAuthInfo;
	char szMd5[MD5_DIGEST_LENGTH];
	char szEnc[AUTH_LEN];
	int iEncLen;
}SignInfoCache;


// 哈希表相关数据
typedef struct tagWebSignClient
{
	tdr_ip_t IP;
	int iConnIdx;
	time_t tTime;
}WebSignClient;

// 哈希表相关数据
typedef struct tagPasswdCardLimit
{
	char szAccountName[ACCOUNT_LOGIN_NAME_LEN]; 
	tdr_ip_t IP;
	int iConnIdx;
	time_t tTime;
	int iTryNum;
	PASSWDCARDPOSINFO stPasswdCardPosInfo; //密保卡位置信息
	int iPasswdCardRowNum; //密保卡行数
	int iPasswdCardColumnNum; //密保卡列数
}PasswdCardLimit;

typedef struct tagAuthMemory
{
	int iVersion;
	char sAccountSht[SHT_SIZE(ACCOUNT_LIMIT_BUCKET_NUM, ACCOUNT_LIMIT_NUM, sizeof(AccountLimit))];
	char sPasswdCardLimitSht[SHT_SIZE(PASSWD_CARD_LIMIT_BUCKET_NUM, PASSWD_CARD_LIMIT_NUM, sizeof(PasswdCardLimit))];
	char sSignInfoCacheSht[SHT_SIZE(SIGN_INFO_CACHEBUCKET_NUM, SIGN_INFO_CACHENUM, sizeof(SignInfoCache))];
	char sWebSignClientSht[SHT_SIZE(WEB_SIGN_CLIENT_BUCKET_NUM, WEB_SIGN_CLIENT_NUM, sizeof(WebSignClient))];
}AuthMemory;

//#define MAX_IP_LIMIT 256

typedef struct tagIPLimit
{
	int iIPLimit;
	char sIPLimit[MAX_IP_LIMIT][sizeof(IPBLOCK)];
}IPLIMIT;

struct tagauth_svrEnv
{
	AUTH_SVRCONF *pstConf;
	AUTH_SVRRUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	unsigned long int iTconnapiHandle;
	int iConnID;
	NETHANDLE *pstCSHan; 
	LPTDRMETA pstAuthMeta;
	LPTDRMETA pstSigMeta;
	LPTDRMETA pstAuthRoleInfoMeta;
	char szRecvBuffer[CS_MAX_MSG];
	char szSendBuffer[CS_MAX_MSG];
	RSA *pstRsa;
	AES stAESCtx;
	TORMPROTOCOLHANDLE hTorm;
	int iTormSeq;
	int iAccountCSID;
	int iAccountDBID;
	int iInformerID;
	int iServiceID;
	int iAccountDBProtoVer;
	//int fAccountDoInsert;
	LPSHTABLE pstAccountLimitCache;
	LPSHTABLE pstPasswdCardLimitCache;
	LPSHTABLE pstSignInfoCacheCache;
	LPSHTABLE pstWebSignClientCache;
	LPSSHANDLE pstSSHan;
	int iAuthShmID;
	AuthMemory *pstAuthMemory;
	TFRAMEHEAD stFrameHead;
	LPTDRMETALIB pstResLib;
	TDRDBHANDLE hResDBHandle;
	IPLIMIT stIPLimit;
	char cClearShm;
	unsigned int uiDebugIP;
	int iRegionID;
	TAPPCTX * pstAppCtx;
	tdr_ip_t IP;
};

typedef struct tagauth_svrEnv  AUTH_SVRENV;
typedef struct tagauth_svrEnv *LPAUTH_SVRENV;


extern RSA_PRIVATE_KEY * IDRSAPrivateKey;
extern const unsigned char sig_key[];

static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_auth_svr[];
extern unsigned char g_szMetalib_proto_cs[];
extern unsigned char g_szMetalib_proto_ss[];

AUTH_SVRENV gs_stEnv;
static AUTH_SVRRUN_STATUS gs_stStat;



// 哈希表相关函数
static int AccountLimitHashCmp(const void * pv1 , const void *pv2);
static unsigned int AccountLimitHashCode(const void* pv);

// 从informer_svr获取的account信息
static int ss_get_account_res(TAPPCTX * pstAppCtx, AUTH_SVRENV * pstEnv,
	                            const SSGETACCOUNTRES *pstGetAccountRes);

// 去informer_svr查找信息。
static int ss_req_get_account(TAPPCTX * pstAppCtx, AUTH_SVRENV * pstEnv,
                              SelectInfo * pstSelectInfo);

// 收到account从外部被修改的消息,清除缓存.
static int ss_update_account_res(TAPPCTX * pstAppCtx, AUTH_SVRENV * pstEnv,
	                               const SSUPDATEACCOUNTREP *pstUpdateAccountRep);

static void db_init_torm_head(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv, 
                              TORMPKGHEAD * pstHead,
                              int iCmd, 
                              int iCallbackCmd,
	                            const char *pszMetaName, 
	                            TormCallback * pstCallback, int iContextLen);

static int db_torm_send(TAPPCTX* pstAppCtx, AUTH_SVRENV* pstEnv, TORMPKG *pstPkg,
	                      int iDstID);

static int db_send_req(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv, 
	                     TORMPKG *pstTormPkg, const void *pvData, int iDataLen);

// 请求从数据库中获取account信息。
static int db_fetch_account(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv, 
                            const AUTHINFO * pstAuthInfo,
                            const ConnCtx *pstConnCtx,
                            const char cActiveFlag,
                            const char* szActiveCode,
                            int iAuthFlag,
                            int iUin,
                            int iCltType,
                            PASSWDCARDAUTHINFO *pstPasswdCardAuthInfo);

static int db_insert_develop_account(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv,
	                                   const AUTHINFO *pstAuthInfo, 
	                                   const ConnCtx *pstConnCtx,
	                                   SelectInfo *pstSelectInfo);

static int db_insert_produce_account(TAPPCTX * pstAppCtx, AUTH_SVRENV * pstEnv,
	                                   const INFORMERACCOUNT * pstIAccount,
	                                   const ConnCtx *pstConnCtx,int iActiveKeyStatus,
	                                   int iAuthFlag,SelectInfo *pstSelectInfo);

// 当数据库中没有account信息时候的处理
static int db_process_if_no_account(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv,
	                                  SelectInfo *pstInfo);

static int db_select_account_res(TAPPCTX * pstAppCtx, AUTH_SVRENV * pstEnv, 
	                               TORMPKGBODY * pstPkg, 
	                               TormCallbackContext * pstCtx);

static int db_insert_account_res(TAPPCTX * pstAppCtx, AUTH_SVRENV * pstEnv, 
	                               TORMPKGBODY * pstPkg, 
	                               TormCallbackContext * pstCtx);

static int db_update_account_res(TAPPCTX * pstAppCtx, AUTH_SVRENV * pstEnv, 
	                               TORMPKGBODY * pstPkg, 
	                               TormCallbackContext * pstCtx);

static int db_req_select_account(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv, 
	                               const ACCOUNT * pstAccount, 
	                               TormCallback * pstCallback);

static int db_req_insert_account(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv, 
	                               const ACCOUNT * pstAccount, 
	                               TormCallback * pstCallback);

static int db_req_update_account_freeze(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv,
	                                      const ACCOUNTFREEZEENDTIME *pstDummy,
	                                      TormCallback *pstCallback);

// 处理account_db发送的信息，多数是回调信息。
static int auth_process_account_db_msg(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv,
	                                     const char * pszMsg, int iMsgLen);

// 处理其他进程发送过来的信息。
static int auth_process_ss_msg(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv,
	                             const char *pszMsg, int iMsgLen);

// 填充Auth Buffer
static int auth_svr_fill_buf(TAPPCTX* pstAppCtx, AUTH_SVRENV* pstEnv, 
                             char * pszEnc, int iSize, 
                             const ACCOUNTAUTHINFO * pstAccount, int iCltProtoVer,
                             int iOffset);

// 身份验证通过，发送回应信息。
static int auth_svr_allowed_respond(TAPPCTX *pstAppCtx, AUTH_SVRENV* pstEnv, 
                                    const char * pszKey,
                                    const ACCOUNTAUTHINFO * pstAccount,
                                    const ConnCtx *pstConnCtx);

// 身份验证失败，回应一个假信息
static int auth_svr_phony_respond(AUTH_SVRENV* pstEnv, const char * pszKey,
                                  int iReason, const ConnCtx *pstConnCtx,char * pszAccount);

// 身份验证应答
static int auth_svr_respond(AUTH_SVRENV* pstEnv, const char * pszKey,
                            const char * pszEnc, int iLen,
                            const ConnCtx *pstConnCtx,int iFrameCmd,char * pszAccount);

// 重置冻结时间
static int auth_reset_freeze(TAPPCTX* pstAppCtx, AUTH_SVRENV *pstEnv, 
                             const ACCOUNT *pstAccount, 
                             const ConnCtx *pstConnCtx);

// 处理许可
static int auth_process_permission(TAPPCTX* pstAppCtx, AUTH_SVRENV *pstEnv, 
                                   SelectInfo* pstSelectInfo,
                                   const ACCOUNT *pstAccount,
                                   const ConnCtx *pstConnCtx);


static void auth_framehead_assign(TFRAMEHEAD *pstFrameHead, 
	                                const ConnCtx *pstConnCtx);
	                                
static void auth_connctx_assign(ConnCtx *pstConnCtx, 
	                              const TFRAMEHEAD *pstFrameHead, int iCltProtoVer);

// 主动断开连接
static int auth_disconnect(AUTH_SVRENV* pstEnv, const ConnCtx *pstConnCtx);

// 清洗缓存
static int auth_flush_account_limit_cache(TAPPCTX *pstAppCtx, AUTH_SVRENV* pstEnv, int iCount, int iForce);

int auth_svr_init(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv);
int auth_svr_msg_send(AUTH_SVRENV *pstEnv, CSPKG *pstCSPkgRes,
	                    const ConnCtx *pstConnCtx);
int auth_svr_msg(TAPPCTX* pstAppCtx, AUTH_SVRENV* pstEnv, char *pszMsg, 
	               int iMsgLen, TFRAMEHEAD *pstFrameHead);
int auth_svr_proc(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv);
int auth_svr_reload(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv);
int auth_svr_stop(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv);

static int db_req_insert_ipblock(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv, 
	                               IPBLOCK* pstIpBlock);

int auth_passwd_card_limit_insert(AUTH_SVRENV *pstEnv,
									char *pszAccount, tdr_ip_t ip,int iConnIdx);

int auth_passwd_card_limit_flush(AUTH_SVRENV *pstEnv,
									int iConnIdx);

PasswdCardLimit* auth_passwd_card_limit_find(AUTH_SVRENV *pstEnv,
									int iConnIdx);
static int auth_flush_passwd_card_limit_cache( AUTH_SVRENV* pstEnv, int iCount, int  iForce);

SignInfoCache * auth_sign_info_cache_insert(AUTH_SVRENV *pstEnv,
									char *pszAccount, tdr_ip_t ip,int iWebUin);

int auth_sign_info_cache_flush(AUTH_SVRENV *pstEnv,
									int iUin);

SignInfoCache* auth_sign_info_cache_find(AUTH_SVRENV *pstEnv,
									int iUin);
static int auth_flush_sign_info_cache( AUTH_SVRENV* pstEnv, int iCount, int  iForce);

static int auth_web_sign_respond( AUTH_SVRENV* pstEnv, 
                                    const char * pszKey,
                                    const ACCOUNTAUTHINFO* pstAccount,
                                    const ConnCtx *pstConnCtx,int iCode,
                                    char *szEnc,int iLen);

static int auth_sign_info_assign( AUTH_SVRENV* pstEnv, 
                                    SignInfoCache * pstSignCache,
                                    const ACCOUNTAUTHINFO* pstAccount,
                                    int iCltProtoVer,int iWebUin);


// 生成密钥
static inline void auth_gen_key(char *psKey, const char * pszPasswd)
{
	char szTmp[KEY_LEN];

	Md5HashBuffer(szTmp, pszPasswd, strlen(pszPasswd));
	Md5HashBuffer(psKey, szTmp, sizeof(szTmp));
}

// 主动断开连接
static inline int auth_disconnect(AUTH_SVRENV* pstEnv, const ConnCtx *pstConnCtx)
{
	auth_framehead_assign(&pstEnv->stFrameHead, pstConnCtx);
	pstEnv->stFrameHead.Cmd = TFRAMEHEAD_CMD_STOP;
	pstEnv->stFrameHead.TimeStampType = 0;
	pstEnv->stFrameHead.ID = pstConnCtx->iID;

	return tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iConnID, 
		pstEnv->szSendBuffer, 0, &pstEnv->stFrameHead);
}

// 随机填充函数
//static inline int auth_rand_fill(int iRandFd, void * iOut, int iLen);


// 哈希表相关函数
static int AccountLimitHashCmp(const void * pv1 , const void *pv2)
{
	const AccountLimit * pstLimit1 = pv1;
	const AccountLimit * pstLimit2 = pv2;

	if (pstLimit1->IP == pstLimit2->IP &&
		0 == strcmp(pstLimit1->szAccountName,pstLimit2->szAccountName))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

static unsigned int AccountLimitHashCode(const void* pv)
{
	const AccountLimit * pstLimit1 = pv;
	
	return pstLimit1->IP;
}


// 哈希表相关函数
static int PasswdCardLimitHashCmp(const void * pv1 , const void *pv2)
{
	const PasswdCardLimit * pstLimit1 = pv1;
	const PasswdCardLimit * pstLimit2 = pv2;

	if (pstLimit1->iConnIdx == pstLimit2->iConnIdx)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

static unsigned int PasswdCardLimitHashCode(const void* pv)
{
	const PasswdCardLimit * pstLimit1 = pv;
	
	return pstLimit1->iConnIdx;
}
// 哈希表相关函数
static int SignInfoCacheHashCmp(const void * pv1 , const void *pv2)
{
	const SignInfoCache * pstLimit1 = pv1;
	const SignInfoCache * pstLimit2 = pv2;

	if (pstLimit1->iWebUin == pstLimit2->iWebUin)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

static unsigned int SignInfoCacheHashCode(const void* pv)
{
	const SignInfoCache * pstLimit1 = pv;
	
	return pstLimit1->iWebUin;
}

// 哈希表相关函数
static int WebSignClientHashCmp(const void * pv1 , const void *pv2)
{
	const WebSignClient * pstLimit1 = pv1;
	const WebSignClient * pstLimit2 = pv2;

	if (pstLimit1->iConnIdx == pstLimit2->iConnIdx)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

static unsigned int WebSignClientHashCode(const void* pv)
{
	const WebSignClient * pstLimit1 = pv;
	
	return pstLimit1->iConnIdx;
}

// 处理account_db发送的信息，多数是回调信息。
static int auth_process_account_db_msg(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv,
	                                     const char * pszMsg, int iMsgLen)
{
	TDRDATA stData;
	TORMPKG stPkg;
	TormCallback stCallback;
	int iRet = 0;
	int iProtoVer = pstEnv->iAccountDBProtoVer;
	
	stData.pszBuff = (char *)pszMsg;
	stData.iBuff = iMsgLen;
	iRet =  torm_attach_res_msg(pstEnv->hTorm, &stData, &stPkg, iProtoVer);
	if (0 >iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			         "torm_attach_res_msg error:%s, iProtoVer=%d", 
		           torm_error_string(iRet), iProtoVer);
		return -1;
	}
	
	if(stPkg.Head.AsynCallBackDataLen > 0 && stPkg.Head.AsynCallBackDataLen <= (int)sizeof(stCallback))
	{
		memcpy(&stCallback, stPkg.Head.AsynCallBackData, 
			     stPkg.Head.AsynCallBackDataLen);
	}
	
	switch(stPkg.Head.HeadCmd.Cmd)
	{
	case TORM_EXGVER_RES:
		pstEnv->iAccountDBProtoVer = stPkg.Body.ExgVerRes.Version;
		break;
		
	case TORM_SELECT_RES:
		iRet = db_select_account_res(pstAppCtx, pstEnv, &stPkg.Body, 
			                           &stCallback.stContext);
		break;
		
	case TORM_INSERT_RES:
		iRet = db_insert_account_res(pstAppCtx, pstEnv, &stPkg.Body, 
			                           &stCallback.stContext);
		break;
		
	case TORM_UPDATE_RES:
		iRet = db_update_account_res(pstAppCtx, pstEnv, &stPkg.Body, 
			                           &stCallback.stContext);
		break;
		
	case TORM_DELETE_RES:
	case TORM_SELECT_COUNT_RES:
		break;
		
	default:
		iRet = -1;
		tlog_error(pstEnv->pstLogCat, 0, 0, "bad TORM cmd received %d.", stPkg.Head.HeadCmd.Cmd);
		break;
	}
	
	return iRet;
}


static int db_req_delete_ipblock(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv, 
	                               IPBLOCK* pstIpBlock)
{
	TORMPKG stPkg;
	TORMDELETEREQ *pstDeleteReq = &stPkg.Body.DeleteReq;
		
	db_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_DELETE_REQ, 0, "IPBlock", NULL, 0);
	
	pstDeleteReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDeleteReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDeleteReq->OperInfo.WhereDef[0] = 0;
	sprintf(pstDeleteReq->OperInfo.WhereDef, "where IP=%d and Mask=%d", pstIpBlock->IP,pstIpBlock->Mask);

	return db_send_req(pstAppCtx, pstEnv, &stPkg, pstIpBlock, sizeof(*pstIpBlock));
}

static int db_req_update_ipblock(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv, 
	                               IPBLOCK* pstIpBlock)
{
	TORMPKG stPkg;
	TORMUPDATEREQ *pstUpReq = &stPkg.Body.UpdateReq;
		
	db_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_UPDATE_REQ, 0, "IPBlock", NULL, 0);
	
	pstUpReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpReq->OperInfo.WhereDef[0] = 0;
	sprintf(pstUpReq->OperInfo.WhereDef, "where IP=%d and Mask=%d", pstIpBlock->IP,pstIpBlock->Mask);

	return db_send_req(pstAppCtx, pstEnv, &stPkg, pstIpBlock, sizeof(*pstIpBlock));
}

int ss_ip_limt_req(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv, ZONEIPLIMITREQ *pstIPLimitReq)
{
	IPLIMIT *pstIPLimit = &pstEnv->stIPLimit;
	int i;
	char cIsExist = 0;
	IPBLOCK *pstIpBlock;
	IPBLOCK stIpBlockTmp;

	for (i=0; i<pstIPLimit->iIPLimit; i++)
	{
		pstIpBlock = (IPBLOCK *)(&pstIPLimit->sIPLimit[i]);
		if (pstIPLimitReq->IP == pstIpBlock->IP && pstIPLimitReq->Mask == pstIpBlock->Mask)
		{
			cIsExist = 1;
			break;
		}
	}

	stIpBlockTmp.IP = pstIPLimitReq->IP;
	stIpBlockTmp.Mask= pstIPLimitReq->Mask;
	if (pstIPLimitReq->Sec > 0)
	{
		stIpBlockTmp.BlockEnd = pstAppCtx->stCurr.tv_sec + pstIPLimitReq->Sec;
	}
	else
	{
		stIpBlockTmp.BlockEnd = pstIPLimitReq->Sec;
	}
	
	if (pstIPLimitReq->Op == OP_ADD)
	{
		if (cIsExist)
		{
			return -1;
		}

		//pstIpBlockTmp = (IPBLOCK *)(&pstIPLimit->sIPLimit[pstIPLimit->iIPLimit ++]);
		
		// 插入数据库
		if (db_req_insert_ipblock(pstAppCtx, pstEnv, &stIpBlockTmp) < 0)
		{
			return -1;
		}

		memcpy(&pstIPLimit->sIPLimit[pstIPLimit->iIPLimit], &stIpBlockTmp, sizeof(stIpBlockTmp));
		pstIPLimit->iIPLimit ++;
		
	}
	else if(pstIPLimitReq->Op == OP_DEL)
	{
		if (!cIsExist)
		{
			return -1;
		}
		
		// 删除数据库
		if (db_req_delete_ipblock(pstAppCtx, pstEnv, &stIpBlockTmp) < 0)
		{
			return -1;
		}

		pstIPLimit->iIPLimit --;
		if (i != pstIPLimit->iIPLimit)
		{
			memcpy (pstIpBlock, &pstIPLimit->sIPLimit[pstIPLimit->iIPLimit], sizeof(*pstIpBlock));
		}
	}
	else if(pstIPLimitReq->Op == OP_MODIFY)
	{
		if (!cIsExist)
			{return -1;};

		if (pstIPLimitReq->Sec <= 0 && pstIPLimitReq->Sec == pstIpBlock->BlockEnd)
		{
			return -1;
		}

		if (pstIPLimitReq->Sec > 0 && pstIPLimitReq->Sec ==  (pstIpBlock->BlockEnd-pstAppCtx->stCurr.tv_sec))
		{
			return -1;
		}
		
		// 更新数据库
		if (db_req_update_ipblock(pstAppCtx, pstEnv, &stIpBlockTmp) < 0)
		{
			return -1;
		}

		memcpy(pstIpBlock,  &stIpBlockTmp, sizeof(stIpBlockTmp));
	}
	else
	{
		return -1;
	}
	return 0;
}

int ss_send_msg_account_cs(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv, SSPKG *pstSSPkg, char *pszBuff, int iBuff,int iCmd)
{
	int iSrcID;
	int iDstID;
	TDRDATA szNet;
	char szBuff[sizeof(SSPKG)];
	SSPKG stPkgRes;
	//SSCSCTRL *pstCsCtrl = &pstSSPkg->Body.CsCtrL;
	
	stPkgRes.Head.Cmd = pstSSPkg->Head.Cmd;
	stPkgRes.Head.Magic = pstSSPkg->Head.Magic;
	stPkgRes.Head.Ver = pstSSPkg->Head.Ver;
	stPkgRes.Head.HeadLen = sizeof(stPkgRes.Head);
	stPkgRes.Body.CsCtrL.Cmd = ZONE_RES;
	stPkgRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq = pstSSPkg->Body.CsCtrL.CsCmd.ZoneReq;
	
	if (iCmd == IP_LIMIT_REQ)
	{
		//memcpy(&stPkgRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq,&pstCsCtrl->CsCmd.ZoneReq,sizeof(pstCsCtrl->CsCmd.ZoneReq));
	}
	else if(iCmd == GET_IP_LIMIT_REQ)
	{
		if (iBuff > CS_ASYN_DATA)
		{
			return -1;
		}
		stPkgRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen = iBuff;
		memcpy (stPkgRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data, pszBuff, iBuff);
		
	}
	else
	{
		return -1;
	}
	
	stPkgRes.Body.CsCtrL.CsCmd.ZoneRes.Result = 0;
	
	szNet.iBuff = sizeof(szBuff);
	szNet.pszBuff = szBuff;

	if (0 > ss_pack(pstEnv->pstSSHan, &szNet, &stPkgRes,  0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -1;
	}

	iSrcID = pstAppCtx->iId;
	iDstID = pstEnv->iAccountCSID;
	if (0 > tbus_send(pstAppCtx->iBus, &iSrcID, &iDstID, szNet.pszBuff, szNet.iBuff, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail");
		return -1;
	}
	
	return 0;
}


int ss_get_ip_limit(AUTH_SVRENV *pstEnv,ZONEGETIPLIMITREQ *pstReq, ZONEGETIPLIMITRES *pstRes)
{
	int i;
	IPBLOCK *pstIPBlock;

	pstRes->iNum = 0;
	// 返回一条
	if (pstReq->Res > 0)
	{
		for (i=0; i<pstEnv->stIPLimit.iIPLimit; i++)
		{
			pstIPBlock = (IPBLOCK *)&pstEnv->stIPLimit.sIPLimit[i];
			if (pstIPBlock->IP == (int)pstReq->Res)
			{
				//pstRes->iNum = 1;
				STRNCPY(pstRes->SSIPLimit[pstRes->iNum].IP,  
						inet_ntoa(*(struct in_addr *)&pstIPBlock->IP), sizeof(pstRes->SSIPLimit[0].IP));
				STRNCPY(pstRes->SSIPLimit[pstRes->iNum].MASK,
						inet_ntoa(*(struct in_addr *)&pstIPBlock->Mask),  sizeof(pstRes->SSIPLimit[0].MASK));
				pstRes->SSIPLimit[pstRes->iNum].BlockEnd = pstIPBlock->BlockEnd;
				pstRes->iNum++;
			}
		}
	}
	// 返回列表
	else 
	{
		for (i=0; i<pstEnv->stIPLimit.iIPLimit; i++)
		{
			if (pstRes->iNum >MAX_IP_LIMIT)
				break;
			
			pstIPBlock = (IPBLOCK *)&pstEnv->stIPLimit.sIPLimit[i];
			STRNCPY(pstRes->SSIPLimit[pstRes->iNum].IP,  
						inet_ntoa(*(struct in_addr *)&pstIPBlock->IP), sizeof(pstRes->SSIPLimit[0].IP));
			STRNCPY(pstRes->SSIPLimit[pstRes->iNum].MASK,
					inet_ntoa(*(struct in_addr *)&pstIPBlock->Mask),  sizeof(pstRes->SSIPLimit[0].MASK));
			pstRes->SSIPLimit[pstRes->iNum].BlockEnd = pstIPBlock->BlockEnd;
			pstRes->iNum ++;
		}
	}

	return 0;
}

int ss_cs_ctrl_msg(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv, SSPKG *pstSSPkg)
{
	SSCSCTRL * pstCSCtrl = &pstSSPkg->Body.CsCtrL;

	if (pstCSCtrl->Cmd == ZONE_REQ)
	{
		if (pstCSCtrl->CsCmd.ZoneReq.Cmd == IP_LIMIT_REQ)
		{
			ZONEIPLIMITREQ stIPLimitReq;
			if (pstCSCtrl->CsCmd.ZoneReq.DataLen != sizeof(stIPLimitReq))
				return -1;
			memcpy(&stIPLimitReq, pstCSCtrl->CsCmd.ZoneReq.Data, sizeof(stIPLimitReq));

			if (ss_ip_limt_req(pstAppCtx, pstEnv, &stIPLimitReq) < 0)
			{
				return -1;
			}
			// 返回结果
			if (ss_send_msg_account_cs(pstAppCtx, pstEnv, pstSSPkg, NULL, 0,IP_LIMIT_REQ) < 0)
			{
				return -1;
			}
	
		}
		else if (pstCSCtrl->CsCmd.ZoneReq.Cmd == GET_IP_LIMIT_REQ)
		{
			ZONEGETIPLIMITREQ stGetLimitReq;
			ZONEGETIPLIMITRES stGetLimitRes;
			if (pstCSCtrl->CsCmd.ZoneReq.DataLen != sizeof(stGetLimitReq))
				return -1;
			memcpy(&stGetLimitReq, pstCSCtrl->CsCmd.ZoneReq.Data, sizeof(stGetLimitReq));
			ss_get_ip_limit(pstEnv,&stGetLimitReq, &stGetLimitRes);
			if (ss_send_msg_account_cs(pstAppCtx, pstEnv, pstSSPkg, (char*)&stGetLimitRes, 
										sizeof(stGetLimitRes),GET_IP_LIMIT_REQ) < 0)
			{
				return -1;
			}
			
		}
		else
		{
			return -1;	
		}
	}
	else 
	{
		return -1;
	}

	return 0;
}

// 处理其他进程发送过来的信息。
static int auth_process_ss_msg(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv,
	                             const char *pszMsg, int iMsgLen)
{
	SSPKG stPkg;
	int iRet = 0;

	if((iRet = ss_unpack(pstEnv->pstSSHan, (char *)pszMsg, iMsgLen, &stPkg, 0)) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,"ss_unpack error Ret %d:%s", iRet,tdr_error_string(iRet));
		return -1;
	}
	//MutiLog(MUTI_LOGLEVEL_INFO, "auth_process_ss_msg cmd:%d ", stPkg.Head.Cmd);
	tlog_info(pstEnv->pstLogCat, 0, 0, "auth_process_ss_msg cmd:%d ", stPkg.Head.Cmd);
	switch(stPkg.Head.Cmd)
	{
	case INFORMER_GET_ACCOUNT_RES:
		iRet = ss_get_account_res(pstAppCtx, pstEnv, &stPkg.Body.GetAccountRes);
		break;

	case UPDATE_ACCOUNT_REP:
		iRet = ss_update_account_res(pstAppCtx, pstEnv, &stPkg.Body.UpdateAccountRep);
		break;
	case CS_CTRL:
		iRet = ss_cs_ctrl_msg(pstAppCtx, pstEnv, &stPkg);
		break;
	default:
		iRet = -1;
		break;
	}

	return iRet;
}

// 收到account从外部被修改的消息,清除缓存.
static int ss_update_account_res(TAPPCTX * pstAppCtx, AUTH_SVRENV * pstEnv,
	                               const SSUPDATEACCOUNTREP *pstUpdateAccountRep)
{
	UNUSED(pstAppCtx);
	
	tlog_info(pstEnv->pstLogCat, 0, 0, "erase_cached account:  %s ", 
		pstUpdateAccountRep->LoginName);
	
	return 0;
}

static int db_req_update_account_active_key(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv,
	                                      	const INFORMERACCOUNT *pstIAccount,
	                                   		SelectInfo *pstExtra)
{
	TORMPKG stPkg;
	TORMUPDATEREQ *pstUpdateReq = &stPkg.Body.UpdateReq;
	TormCallback stCallback;
	ACCOUNTACTIVEKEY stDummy;
	
	stCallback.stContext.stUpdateInfo.stConnCtx = pstExtra->stConnCtx;

	STRNCPY(stCallback.stContext.stUpdateInfo.szLoginName,
	      pstIAccount->Pass9Account,
	      sizeof(stCallback.stContext.stUpdateInfo.szLoginName));

	
	stDummy.ActiveKeyStatus = ACTIVE_KEY_STATUS_ACTIVE;
	stDummy.Uin = pstExtra->Uin;
	
	db_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_UPDATE_REQ, 
			              TORM_UPDATE_RES, "AccountActiveKey", &stCallback, 
			              sizeof(stCallback.stContext.stUpdateInfo));
	
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;

	return db_send_req(pstAppCtx, pstEnv, &stPkg, &stDummy, sizeof(stDummy));
}

static int db_req_update_account_valid_passwd(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv,
	                                      	const INFORMERACCOUNT *pstIAccount,
	                                   		SelectInfo *pstExtra)
{
	TORMPKG stPkg;
	TORMUPDATEREQ *pstUpdateReq = &stPkg.Body.UpdateReq;
	TormCallback stCallback;
	ACCOUNTVALIDPASSWD stDummy;
	
	stCallback.stContext.stUpdateInfo.stConnCtx = pstExtra->stConnCtx;

	STRNCPY(stCallback.stContext.stUpdateInfo.szLoginName,
	      pstIAccount->Pass9Account,
	      sizeof(stCallback.stContext.stUpdateInfo.szLoginName));

	memset(&stDummy,0,sizeof(stDummy));
	
	stDummy.ActiveKeyStatus = ACTIVE_KEY_STATUS_INIT;

	if (pstExtra->AskActiveKeyStatusFlag & ACTIVE_KEY_STATUS_FLAG_YES)
	{
		stDummy.ActiveKeyStatus = ACTIVE_KEY_STATUS_ACTIVE;
	}
	
	stDummy.Uin = pstExtra->Uin;
	stDummy.UinFlag = pstIAccount->UinFlag | pstExtra->stAccount.UinFlag;
	stDummy.UinFlag &= ~UIN_FLAG_PASSWD_INVALID;
	stDummy.PasswdCard = pstIAccount->PasswdCard;
	memcpy(stDummy.PasswdHash,pstIAccount->PasswdHash,sizeof(stDummy.PasswdHash));
	
	db_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_UPDATE_REQ, 
			              TORM_UPDATE_RES, "AccountValidPasswd", &stCallback, 
			              sizeof(stCallback.stContext.stUpdateInfo));
	
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;

	return db_send_req(pstAppCtx, pstEnv, &stPkg, &stDummy, sizeof(stDummy));
}



// 从informer_svr获取的account信息
static int ss_get_account_res(TAPPCTX * pstAppCtx, AUTH_SVRENV * pstEnv,
	                            const SSGETACCOUNTRES *pstGetAccountRes)
{
	int iRet = 0;
	const INFORMERACCOUNT *pstIAccount = &pstGetAccountRes->Account;
	SelectInfo *pstExtra = (SelectInfo *)(pstGetAccountRes->ExtraData);

	if(pstGetAccountRes->Found == AUTH_IFM_NEED_REGIST_CODE_ERROR)
	{
		db_insert_produce_account(pstAppCtx, pstEnv, pstIAccount, 
			                               &pstExtra->stConnCtx,ACTIVE_KEY_STATUS_INIT,
			                               	pstExtra->AuthFlag & AUTH_FLAG_INSERT,
			                               	pstExtra);
	}

	// 从informer_svr获取的account信息如果存在，就将他插入数据，如果不存在，
	// 进发给客户端一个虚假的回应信息，防止客户端猜测密码。
	if( pstGetAccountRes->Found == AUTH_REASON_SUCCEEDED )
	{

		//拉取密码的返回
		if (pstExtra->UinType == 1)
		{
			//todo:nox
			//uin验证?
			if (pstEnv->pstConf->NoxFlag && (pstExtra->AuthFlag & AUTH_FLAG_NOX))
			{
				iRet = auth_svr_allowed_respond(pstAppCtx, pstEnv, pstExtra->stAccount.PasswdHash, 
			                             &pstExtra->stAccount, &pstExtra->stConnCtx);
			}
			else
			{

				//更新激活码状态,密码,密保卡
				db_req_update_account_valid_passwd(pstAppCtx, pstEnv, pstIAccount,pstExtra);

				if (pstExtra->stAccount.UinFlag & UIN_FLAG_PASSWD_CARD)
				{
					iRet = auth_svr_phony_respond(pstEnv, pstExtra->stAuthInfo.RandK1, 
			                             AUTH_REASON_NEED_PASSWD_CARD_ERROR, &pstExtra->stConnCtx,pstExtra->stAccount.Pass9Account);
				}
				else
				{

					iRet = auth_svr_allowed_respond(pstAppCtx, pstEnv, pstExtra->stAccount.PasswdHash, 
			                             &pstExtra->stAccount, &pstExtra->stConnCtx);
				}
			}
		}
		else
		{
		
			//todo:nox
			//指定uin插入
			if (pstEnv->pstConf->NoxFlag && (pstExtra->AuthFlag & AUTH_FLAG_NOX))
			{
				if (pstExtra->AskActiveKeyStatusFlag & ACTIVE_KEY_STATUS_FLAG_YES)
				{
					iRet = db_insert_produce_account(pstAppCtx, pstEnv, pstIAccount, 
				                               		&pstExtra->stConnCtx,ACTIVE_KEY_STATUS_ACTIVE,
				                               		pstExtra->AuthFlag,pstExtra);
				}
				else
				{
					iRet = db_insert_produce_account(pstAppCtx, pstEnv, pstIAccount, 
				                               		&pstExtra->stConnCtx,ACTIVE_KEY_STATUS_INIT,
				                               		pstExtra->AuthFlag,pstExtra);

				}
			}
			else
			{

				if (pstExtra->AskActiveKeyStatusFlag & ACTIVE_KEY_STATUS_FLAG_ASK)
				{			
					db_req_update_account_active_key(pstAppCtx,pstEnv,
													pstIAccount, 
					                               	pstExtra);

					if (pstExtra->stAccount.UinFlag & UIN_FLAG_PASSWD_CARD)
					{
						iRet = auth_svr_phony_respond(pstEnv, pstExtra->stAuthInfo.RandK1, 
				                             AUTH_REASON_NEED_PASSWD_CARD_ERROR, &pstExtra->stConnCtx,pstExtra->stAccount.Pass9Account);
					}
					else
					{

						iRet = auth_svr_allowed_respond(pstAppCtx, pstEnv, pstExtra->stAccount.PasswdHash, 
				                             &pstExtra->stAccount, &pstExtra->stConnCtx);
					}

				}
				else
				{
					if (pstExtra->AskActiveKeyStatusFlag & ACTIVE_KEY_STATUS_FLAG_YES)
					{
						iRet = db_insert_produce_account(pstAppCtx, pstEnv, pstIAccount, 
					                               		&pstExtra->stConnCtx,ACTIVE_KEY_STATUS_ACTIVE,
					                               		pstExtra->AuthFlag,pstExtra);
					}
					else
					{
						iRet = db_insert_produce_account(pstAppCtx, pstEnv, pstIAccount, 
					                               		&pstExtra->stConnCtx,ACTIVE_KEY_STATUS_INIT,
					                               		pstExtra->AuthFlag,pstExtra);

					}
					
				}
			}
		}
	}
	else
	{
	 	/* pstGetAccountRes->Found  为没找到的返回码,参考ifm反响激活失败返回码*/
		iRet = auth_svr_phony_respond(pstEnv, pstExtra->stAuthInfo.RandK1, 
			                            pstGetAccountRes->Found, &pstExtra->stConnCtx,pstExtra->stAccount.Pass9Account);
		tlog_debug(pstEnv->pstLogCat, 0, 0, "phony rand key=%s",
					pstExtra->stAuthInfo.RandK1);
	}
	
	return iRet;
}

// 去informer_svr查找信息。
static int ss_req_get_account(TAPPCTX * pstAppCtx, AUTH_SVRENV * pstEnv,
                              SelectInfo * pstExtraInfo)
{
	SSPKG stPkg;
	TDRDATA stNet;
	int iSrc, iDst;
	SSGETACCOUNTREQ *pstGetAccountReq = &stPkg.Body.GetAccountReq;

	STRNCPY(pstGetAccountReq->LoginName, pstExtraInfo->stAuthInfo.AccountLoginName,
		      sizeof(pstGetAccountReq->LoginName));
	pstGetAccountReq->ExtraDataLen = sizeof(*pstExtraInfo);
	memcpy(pstGetAccountReq->ExtraData, pstExtraInfo, sizeof(*pstExtraInfo));

	stPkg.Head.Cmd = INFORMER_GET_ACCOUNT_REQ;
	stPkg.Head.Magic = PROTOCOL_MAGIC; 
	stPkg.Head.Ver = TDR_METALIB_PROTO_SS_VERSION;
	stPkg.Head.HeadLen = sizeof(stPkg.Head);

	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = pstEnv->szSendBuffer;
	if(ss_pack(pstEnv->pstSSHan, &stNet, &stPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"ss_pack fail.");
		return -1;
	}

	iSrc = pstAppCtx->iId;
	iDst = pstEnv->iServiceID;
	if(tbus_forward(pstAppCtx->iBus, &iSrc, &iDst, stNet.pszBuff, stNet.iBuff, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"tbus_forward fail.");
		return -1;
	}
	return 0;
}

static int auth_get_account_auth_info(const ACCOUNT *pstAccount,ACCOUNTAUTHINFO *pstAccountAuth)
{
	memset(pstAccountAuth,0,sizeof(*pstAccountAuth));

	STRNCPY(pstAccountAuth->BirthDay, pstAccount->BirthDay,sizeof(pstAccountAuth->BirthDay));
	STRNCPY(pstAccountAuth->Pass9Account, pstAccount->Pass9Account,sizeof(pstAccountAuth->Pass9Account));
	pstAccountAuth->Uin = pstAccount->Uin;
	pstAccountAuth->SilenceEndTime = pstAccount->SilenceEndTime;
	pstAccountAuth->UinFlag = pstAccount->UinFlag;
	memcpy(pstAccountAuth->PasswdHash,pstAccount->PasswdHash,sizeof(pstAccountAuth->PasswdHash));
	
	return 0;
}

static void auth_selectinfo_to_insertinfo(SelectInfo *pstSelectInfo, 
	                               InsertInfo* pstInsertInfo,int iAuthFlag)
{
	pstInsertInfo->stConnCtx = pstSelectInfo->stConnCtx;
	pstInsertInfo->AuthFlag = iAuthFlag;
	pstInsertInfo->CltType = pstSelectInfo->CltType;
	pstInsertInfo->stAuthInfo = pstSelectInfo->stAuthInfo;
	pstInsertInfo->iWebUin = pstSelectInfo->iWebUin;
}


static int db_insert_produce_account(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv,
	                                   const INFORMERACCOUNT *pstIAccount,
	                                   const ConnCtx *pstConnCtx,int iActiveKeyStatus,
	                                   int iAuthFlag,SelectInfo *pstSelectInfo)
{
	TormCallback stCallback;
	ACCOUNT *pstAccount = NULL;
	ACCOUNT stAccount ;
	ACCOUNTAUTHINFO *pstAccountAuth = NULL;

	auth_selectinfo_to_insertinfo(pstSelectInfo,&stCallback.stContext.stInsertInfo,iAuthFlag);
	
	// 将从informer_svr获取的数据插入数据库
	pstAccountAuth = &stCallback.stContext.stInsertInfo.stAccount;
	pstAccount = &stAccount;
	
	memset(pstAccount, 0, sizeof(*pstAccount));
	memset(pstAccountAuth,0,sizeof(*pstAccountAuth));
	
	pstAccount->Uin = 0;
	//todo:nox
	//指定uin插入
	if (pstEnv->pstConf->NoxFlag && (iAuthFlag & AUTH_FLAG_NOX))
	{
		pstAccount->Uin = pstIAccount->WebUin;
	}
	
	pstAccount->UinFlag = pstIAccount->UinFlag;
	STRNCPY(pstAccount->Pass9Account, pstIAccount->Pass9Account, 
	      		sizeof(pstAccount->Pass9Account));
	memcpy(pstAccount->PasswdHash, pstIAccount->PasswdHash, 
		      sizeof(pstAccount->PasswdHash));
	STRNCPY(pstAccount->BirthDay, pstIAccount->BirthDay, 
		      sizeof(pstAccount->BirthDay));	
	memcpy(&pstAccount->Gender, &pstIAccount->Gender,1);	
	STRNCPY(pstAccount->RegisterTs, pstIAccount->RegisterTs, 
		      sizeof(pstAccount->RegisterTs));	
	pstAccount->SilenceEndTime = 0;
	memset(pstAccount->RegisterTs, 0, sizeof(pstAccount->RegisterTs));	
	pstAccount->FreezeStartTime = 0;
	pstAccount->FreezeEndTime = 0;
	pstAccount->ActiveKeyStatus = iActiveKeyStatus;
	pstAccount->AccountCreateTime = pstAppCtx->stCurr.tv_sec;
	pstAccount->FirstLoginCltType = pstSelectInfo->CltType;
	pstAccount->PasswdCard = pstIAccount->PasswdCard;

	auth_get_account_auth_info(pstAccount,pstAccountAuth);

	
	if (pstAccount->UinFlag & UIN_FLAG_PASSWD_CARD)
	{
		PasswdCardLimit* pstPasswdCardLimit = NULL;
		
		auth_passwd_card_limit_insert(pstEnv,stAccount.Pass9Account,
				pstConnCtx->stExtraInfo.IPInfo.ip,
				pstConnCtx->iConnIdx);

		pstPasswdCardLimit = auth_passwd_card_limit_find(pstEnv,pstConnCtx->iConnIdx);

		if (!pstPasswdCardLimit)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "auth_passwd_card_limit_find fail,item:%d max:%d connidx:%d account:%s",
			pstEnv->pstPasswdCardLimitCache->iItem, pstEnv->pstPasswdCardLimitCache->iMax,
			pstConnCtx->iConnIdx);
		}
		else
		{		
			pstPasswdCardLimit->iPasswdCardRowNum = stAccount.PasswdCard.RowNum;
			pstPasswdCardLimit->iPasswdCardColumnNum = stAccount.PasswdCard.Row[0].ColumnNum;
		}
	
	}

	return db_req_insert_account(pstAppCtx, pstEnv, pstAccount, &stCallback);
}




static int db_insert_develop_account(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv,
	                                   const AUTHINFO *pstAuthInfo, 
	                                   const ConnCtx *pstConnCtx,
	                                   SelectInfo *pstSelectInfo)
{
	TormCallback stCallback;
	ACCOUNT * pstAccount = NULL;
	ACCOUNT stAccount;
	ACCOUNTAUTHINFO* pstAccountAuth = NULL;
	
	auth_selectinfo_to_insertinfo(pstSelectInfo,&stCallback.stContext.stInsertInfo,pstSelectInfo->AuthFlag);

	// 随意填充数据插入数据库
	pstAccountAuth = &stCallback.stContext.stInsertInfo.stAccount;
	pstAccount = &stAccount;
	memset(pstAccount, 0, sizeof(*pstAccount));
	memset(pstAccountAuth,0,sizeof(*pstAccountAuth));
	
	pstAccount->Uin = 0;
	pstAccount->UinFlag = 0;
	pstAccount->FreezeEndTime = 0;
	pstAccount->FreezeStartTime = 0;
	pstAccount->SilenceEndTime = 0;
	pstAccount->AccountCreateTime = pstAppCtx->stCurr.tv_sec;
	STRNCPY(pstAccount->BirthDay, "19890604", sizeof(pstAccount->BirthDay));
	STRNCPY(pstAccount->Pass9Account, pstAuthInfo->AccountLoginName, 
		      sizeof(pstAccount->Pass9Account));

	if (pstSelectInfo->AuthFlag & AUTH_FLAG_QUICK)
	{
		pstAccount->UinFlag |= UIN_FLAG_QUICK;
		pstAccount->QuickStatus = QUICK_STATUS_QUICK;
	}
	pstAccount->FirstLoginCltType = pstSelectInfo->CltType;
	// 开发用帐号都是空密码
	auth_gen_key(pstAccount->PasswdHash, pstAuthInfo->AccountLoginPasswd);

	auth_get_account_auth_info(pstAccount,pstAccountAuth);
	
	return db_req_insert_account(pstAppCtx, pstEnv, pstAccount, &stCallback);
	UNUSED(pstConnCtx);	//add by paraunused.pl
}

// 当数据库中没有account信息时候的处理
static int db_process_if_no_account(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv,
                                    SelectInfo *pstInfo)
{
	int iRet = -1;
	int iAuthMode = pstInfo->AuthMode;

	if (pstInfo->stConnCtx.iConnIdx < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "iConnIdx=%d", pstInfo->stConnCtx.iConnIdx);
	}
	
	

	//nox验证
	if (pstEnv->pstConf->NoxFlag && (pstInfo->AuthFlag & AUTH_FLAG_NOX ))
	{
		//todo:nox
		//UinType标记为无记录
		//丢给service_svr
		pstInfo->UinType = 0;
		iRet = ss_req_get_account(pstAppCtx, pstEnv, pstInfo);
		return iRet;
	}

	//web端获取签名时,插入
	if (pstInfo->AuthFlag & AUTH_FLAG_WEB_SIGN )
	{
		INFORMERACCOUNT stIAccount; 
		memset(&stIAccount,0,sizeof(stIAccount));

		stIAccount.WebUin = pstInfo->stAuthInfo.WebUin;
		STRNCPY(stIAccount.Pass9Account,pstInfo->stAuthInfo.AccountLoginName,
				sizeof(stIAccount.Pass9Account));
		
		STRNCPY(stIAccount.BirthDay,pstInfo->stAuthInfo.Birthday,
				sizeof(stIAccount.BirthDay));

		stIAccount.UinFlag |= UIN_FLAG_PASSWD_INVALID;
		
		
		db_insert_produce_account(pstAppCtx, pstEnv, &stIAccount, 
			                               &pstInfo->stConnCtx,ACTIVE_KEY_STATUS_INIT,
			                               pstInfo->AuthFlag,
			                               pstInfo);
		return 0;
	}
	
	
	switch(iAuthMode)
	{
	case AUTH_MODE_DEVELOP:
		if (pstInfo->AuthFlag & AUTH_FLAG_DEQUICK)
		{
			iRet = ss_req_get_account(pstAppCtx, pstEnv, pstInfo);
		}
		else
		{
			iRet = db_insert_develop_account(pstAppCtx, pstEnv, &pstInfo->stAuthInfo,
			                               	&pstInfo->stConnCtx,pstInfo);
		}
		break;
	case AUTH_MODE_PERMISSION:
		iRet = ss_req_get_account(pstAppCtx, pstEnv, pstInfo);
		/*
		if (pstInfo->AuthFlag & AUTH_FLAG_QUICK)
		{
			iRet = db_insert_develop_account(pstAppCtx, pstEnv, &pstInfo->stAuthInfo,
			                               &pstInfo->stConnCtx,pstInfo);
		}
		else
		{
			iRet = auth_svr_phony_respond(pstEnv, pstInfo->stAuthInfo.RandK1, 		
			                            AUTH_REASON_NO_ACCOUNT, &pstInfo->stConnCtx);
		}
		*/
		break;
	case AUTH_MODE_PRODUCE:
		if (pstInfo->AuthFlag & AUTH_FLAG_QUICK)
		{
			iRet = db_insert_develop_account(pstAppCtx, pstEnv, &pstInfo->stAuthInfo,
			                               &pstInfo->stConnCtx,pstInfo);
		}
		else
		{
			iRet = ss_req_get_account(pstAppCtx, pstEnv, pstInfo);
		}
		break;
	
	case AUTH_MODE_CLOSE:
		iRet = auth_svr_phony_respond(pstEnv, pstInfo->stAuthInfo.RandK1,		
			 AUTH_REASON_NO_ACCOUNT, &pstInfo->stConnCtx, pstInfo->stAuthInfo.AccountLoginName);
		break;
	default:
		tlog_error(pstEnv->pstLogCat, 0, 0,	"auth mode error.");
		auth_disconnect(pstEnv, &pstInfo->stConnCtx);
		break;
	}
	return iRet;
}

static int db_req_update_account_clt_type(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv,
	                                      const UPDATEFIRSTLOGINCLTTYPE *pstDummy,
	                                      TormCallback *pstCallback)
{
	TORMPKG stPkg;
	TORMUPDATEREQ *pstUpdateReq = &stPkg.Body.UpdateReq;

	db_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_UPDATE_REQ, 
			              TORM_UPDATE_RES, "UpdateFirstLoginCltType", pstCallback, 
			              sizeof(pstCallback->stContext.stUpdateInfo));
	
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;

	return db_send_req(pstAppCtx, pstEnv, &stPkg, pstDummy, sizeof(*pstDummy));
}

int db_select_account_role_res(TAPPCTX * pstAppCtx, AUTH_SVRENV * pstEnv, 
	                               				TORMPKGBODY * pstPkg, 
	                               				TormCallbackContext * pstCtx)
{
	TORMSELECTRES * pstSelectRes = &pstPkg->SelectRes;
	ACCOUNTROLE stAccountRole;
	CSPKG stCsPkgSvr;
	ACCGETROLESVR *pstSvr = &stCsPkgSvr.Body.AccGetRoleSvr;
	
	int iSucc = 1;
	int iRet;
	int iNum = 0;
	int i;

	pstSvr->Num = 0;
	
	if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			         "Select accountRole info from db failed. DBErr = %d, Name = %s, Msg = %s.", 
			         pstSelectRes->DBMSErrNo,
			         pstCtx->stSelectInfo.stAuthInfo.AccountLoginName,
			         torm_error_string(pstSelectRes->ErrNo));
		iSucc = 0;
		goto end;
	}

	iNum = pstSelectRes->SelectResult.ResultNum;
	memset(pstSvr, 0, sizeof(*pstSvr));
	pstSvr->RegionID = pstEnv->iRegionID;

	for (i=0; i<iNum; i++)
	{
		if (pstSvr->Num >= ACC_ROLE_MAX)
		{
			break;
		}
		
		memset(&stAccountRole,0,sizeof(stAccountRole));
		
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stAccountRole, sizeof(stAccountRole)))
		{
			continue;
		}

		pstSvr->RoleInfo[pstSvr->Num].Level = stAccountRole.Level;
		pstSvr->RoleInfo[pstSvr->Num].WorldID = stAccountRole.ServerID;
		STRNCPY(pstSvr->RoleInfo[pstSvr->Num].RoleName, stAccountRole.RoleName, 
				sizeof(pstSvr->RoleInfo[pstSvr->Num].RoleName));
		pstSvr->RoleInfo[pstSvr->Num].LastLogout = stAccountRole.LastLogout;
		pstSvr->Num ++;
	}

end:
	stCsPkgSvr.Head.Magic = PROTOCOL_MAGIC;
	stCsPkgSvr.Head.Cmd = ACC_GET_ROLE_SVR;
	stCsPkgSvr.Head.Ver = TDR_METALIB_PROTO_CS_VERSION;
	pstSvr->Succ = iSucc;
		
	iRet = auth_svr_msg_send(pstEnv, &stCsPkgSvr, &pstCtx->stSelectInfo.stConnCtx);
	if( iRet < 0 )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "auth_svr_msg_send fail %d", iRet);
		pstEnv->pstRunStat->SendCltMsgFail++;
	}
	return iRet;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}




static int db_select_account_res(TAPPCTX * pstAppCtx, AUTH_SVRENV * pstEnv, 
	                               TORMPKGBODY * pstPkg, 
	                               TormCallbackContext * pstCtx)
{
	TORMSELECTRES * pstSelectRes = &pstPkg->SelectRes;
	ACCOUNT stAccount;
	int iAuthMode = pstCtx->stSelectInfo.AuthMode;
	int iSelectType = pstCtx->stSelectInfo.SelectType;
	PASSWDCARDAUTHINFO* pstPasswdCard = NULL;
	PASSWDCARDPOSINFO* pstPasswdCardPos = NULL;
	int i = 0;
	int iPassNum = 0;
	PASSWDCARDCELL *pstCell = NULL;
	RESPOS *pstPos = NULL;
	PasswdCardLimit* pstPasswdCardLimit = NULL;
	ConnCtx* pstConnCtx = &pstCtx->stSelectInfo.stConnCtx;
	int iRet = 0;

	if (iSelectType)
	{
		db_select_account_role_res(pstAppCtx, pstEnv, pstPkg, pstCtx);
		return 0;
	}

	if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			         "Select account info from db failed. DBErr = %d, Name = %s, Msg = %s.", 
			         pstSelectRes->DBMSErrNo,
			         pstCtx->stSelectInfo.stAuthInfo.AccountLoginName,
			         torm_error_string(pstSelectRes->ErrNo));
		return -1;
	}
	
	if(pstSelectRes->SelectResult.ResultNum == 0)
	{
		return db_process_if_no_account(pstAppCtx, pstEnv, &pstCtx->stSelectInfo);
	}

	iRet = torm_fetch_one_meta(pstEnv->hTorm, (char *)&stAccount, sizeof(stAccount));

	if(iRet < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"torm_fetch_one_meta fail");
		auth_disconnect(pstEnv, pstConnCtx);
		return -1;
	}

	auth_get_account_auth_info(&stAccount,&pstCtx->stSelectInfo.stAccount);

	if ((pstCtx->stSelectInfo.AuthFlag & AUTH_FLAG_NOX) && pstEnv->pstConf->NoxFlag)
	{
		//todo:nox
		//UinType标记为有记录
		//丢给service_svr
		pstCtx->stSelectInfo.UinType = 1;	
		
		auth_gen_key(pstCtx->stSelectInfo.stAccount.PasswdHash,pstCtx->stSelectInfo.stAuthInfo.AccountLoginPasswd);
		
			
		iRet = ss_req_get_account(pstAppCtx, pstEnv, &pstCtx->stSelectInfo);
		return iRet;
	}

	//web端获取签名
	if (pstCtx->stSelectInfo.AuthFlag & AUTH_FLAG_WEB_SIGN )
	{	
		SignInfoCache * pstSignCache = NULL;
			
		pstSignCache = auth_sign_info_cache_find(pstEnv, pstCtx->stSelectInfo.stAuthInfo.WebUin);
		if (!pstSignCache)
		{			
			iRet = auth_svr_phony_respond(pstEnv,pstCtx->stSelectInfo.stAuthInfo.RandK1,
						 		AUTH_IFM_WEB_SIGN_ERROR, &pstCtx->stSelectInfo.stConnCtx,
						 		pstCtx->stSelectInfo.stAuthInfo.AccountLoginName
						 		);
		}	
		else
		{
			if (pstSignCache->iSignInfoStat != SIGN_INFO_STAT_READY)
			{
				iRet = auth_sign_info_assign(pstEnv,pstSignCache,
								&pstCtx->stSelectInfo.stAccount,
								pstCtx->stSelectInfo.stConnCtx.iCltProtoVer,
								pstCtx->stSelectInfo.iWebUin);
			}
			else
			{
				iRet = 0;
			}

			if (0 == iRet)
			{
				//约定网站在获取签名前已经通过了激活码和密码卡验证
				iRet = auth_web_sign_respond(pstEnv,
							pstCtx->stSelectInfo.stAuthInfo.RandK1,
							&pstCtx->stSelectInfo.stAccount,
							&pstCtx->stSelectInfo.stConnCtx,
							AUTH_IFM_WEB_SIGN_SUCC,
							pstSignCache->szEnc,
							pstSignCache->iEncLen
							);
			}
			
		}
		
		return iRet;
	}
	else
	{
		//无效的密码,丢给service_svr
		if (stAccount.UinFlag & UIN_FLAG_PASSWD_INVALID)
		{
			pstCtx->stSelectInfo.UinType = 1;		
			
			ss_req_get_account(pstAppCtx, pstEnv, &pstCtx->stSelectInfo);
			return 0;
		}

		if (stAccount.UinFlag & UIN_FLAG_PASSWD_CARD)
		{
			
			auth_passwd_card_limit_insert(pstEnv,stAccount.Pass9Account,
					pstConnCtx->stExtraInfo.IPInfo.ip, 
					pstConnCtx->iConnIdx);

			pstPasswdCardLimit = auth_passwd_card_limit_find(pstEnv,pstConnCtx->iConnIdx);

			if (!pstPasswdCardLimit)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "auth_passwd_card_limit_find fail,item:%d max:%d connidx:%d account:%s",
				pstEnv->pstPasswdCardLimitCache->iItem, pstEnv->pstPasswdCardLimitCache->iMax,
				pstConnCtx->iConnIdx);
			}
			else
			{		
				pstPasswdCardLimit->iPasswdCardRowNum = stAccount.PasswdCard.RowNum;
				pstPasswdCardLimit->iPasswdCardColumnNum = stAccount.PasswdCard.Row[0].ColumnNum;
			}
		}
		
		if (iAuthMode == AUTH_MODE_PERMISSION &&
			stAccount.ActiveKeyStatus != ACTIVE_KEY_STATUS_ACTIVE)
		{
			auth_get_account_auth_info(&stAccount,&pstCtx->stSelectInfo.stAccount);
			
			pstCtx->stSelectInfo.Uin = stAccount.Uin;
			pstCtx->stSelectInfo.AskActiveKeyStatusFlag |= ACTIVE_KEY_STATUS_FLAG_ASK;
			
			return ss_req_get_account(pstAppCtx, pstEnv, &pstCtx->stSelectInfo);
		}

		if (stAccount.UinFlag & UIN_FLAG_PASSWD_CARD)
		{
			int iNeedPasswdCard = 1;

			pstPasswdCardLimit = auth_passwd_card_limit_find(pstEnv,pstConnCtx->iConnIdx);

			if (!pstPasswdCardLimit)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "auth_passwd_card_limit_find fail,item:%d max:%d connidx:%d account:%s",
				pstEnv->pstPasswdCardLimitCache->iItem, pstEnv->pstPasswdCardLimitCache->iMax,
				pstConnCtx->iConnIdx);
			}
			else
			{

				pstPasswdCard = &pstCtx->stSelectInfo.stPasswdCardAuthInfo;
				pstPasswdCardPos = &pstPasswdCardLimit->stPasswdCardPosInfo;
				
				if ((pstCtx->stSelectInfo.AuthFlag & AUTH_FLAG_PASSWD_CARD) &&
					pstPasswdCard->Num > 0 &&
					pstPasswdCard->Num == pstPasswdCardPos->Num)
				{
					for (i = 0; i < pstPasswdCard->Num; i++)
					{
						pstPos = &pstPasswdCardPos->Pos[i];
						if (pstPos->Y >= stAccount.PasswdCard.RowNum ||
							pstPos->X >= stAccount.PasswdCard.Row[pstPos->Y].ColumnNum)
						{
							continue;
						}
						
						pstCell = &stAccount.PasswdCard.Row[pstPos->Y].Column[pstPos->X];
						
						if (pstPasswdCard->Data[i].CellLen == pstCell->CellLen &&
							0 == strncmp(pstPasswdCard->Data[i].Cell, pstCell->Cell,pstCell->CellLen))
						{
							iPassNum++;
						}
					}

					if (iPassNum == pstPasswdCardPos->Num)
					{
						iNeedPasswdCard = 0;
					}
					
				}
			}

			if (iNeedPasswdCard)
			{
				return auth_svr_phony_respond(pstEnv, pstCtx->stSelectInfo.stAuthInfo.RandK1, 
				                              AUTH_REASON_NEED_PASSWD_CARD_ERROR, 
				                              &pstCtx->stSelectInfo.stConnCtx,pstCtx->stSelectInfo.stAuthInfo.AccountLoginName);
			}
			else
			{
				auth_passwd_card_limit_flush(pstEnv,pstCtx->stSelectInfo.stConnCtx.iConnIdx);
			}
		}
	}

	
/*
	//拿签名的时候,丢给service_svr
	if (pstCtx->stSelectInfo.AuthFlag & AUTH_FLAG_SIGN ||
		pstCtx->stSelectInfo.AuthMode == AUTH_MODE_DEVELOP)
*/		
	
	
	if(stAccount.FirstLoginCltType == CLT_NULL)
	{
		UPDATEFIRSTLOGINCLTTYPE stCltType;
		
		stCltType.FirstLoginCltType = pstCtx->stSelectInfo.CltType;
		stCltType.Uin = stAccount.Uin;

		db_req_update_account_clt_type(pstAppCtx, pstEnv, 
				                         &stCltType,NULL);
	}

	if(auth_process_permission(pstAppCtx, pstEnv, 
		                         &pstCtx->stSelectInfo,
		                         &stAccount, 
		                         &pstCtx->stSelectInfo.stConnCtx) < 0)
	{
		return -1;
	}

	return 0;
}

static int db_update_account_res(TAPPCTX * pstAppCtx, AUTH_SVRENV * pstEnv, 
	                               TORMPKGBODY * pstPkg, 
	                               TormCallbackContext * pstCtx)
{
	TORMUPDATERES * pstUpdateRes = &pstPkg->UpdateRes;

	UNUSED(pstAppCtx);

	if (TORM_ERR_IS_ERROR(pstUpdateRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			         "Update account info in db failed. DBErr = %d, Name = %s, Msg = %s.", 
			         pstUpdateRes->DBMSErrNo,
			         pstCtx->stUpdateInfo.szLoginName,
			         torm_error_string(pstUpdateRes->ErrNo));
		return -1;
	}

	return 0;
}

static int db_insert_account_res(TAPPCTX * pstAppCtx, AUTH_SVRENV * pstEnv, 
	                               TORMPKGBODY * pstPkg, 
	                               TormCallbackContext * pstCtx)
{
	TORMINSERTRES * pstInsertRes = &pstPkg->InsertRes;
	ACCOUNTAUTHINFO* pstAccountAuth = &pstCtx->stInsertInfo.stAccount;
	int iRet = 0;

	if (TORM_ERR_IS_ERROR(pstInsertRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			         "Insert account info in db failed. DBErr = %d, Name = %s, Msg = %s.", 
			         pstInsertRes->DBMSErrNo,
			         pstAccountAuth->Pass9Account,
			         torm_error_string(pstInsertRes->ErrNo));
		goto error;
	}

	if (pstCtx->stInsertInfo.AuthFlag & AUTH_FLAG_INSERT)
	{
		return 0;
	}
	
	pstAccountAuth->Uin = (int)(pstInsertRes->LastInsertID);

	if (pstAccountAuth->UinFlag & UIN_FLAG_PASSWD_CARD)
	{
		iRet = auth_svr_phony_respond(pstEnv, pstCtx->stInsertInfo.stAuthInfo.RandK1, 
			                              AUTH_REASON_NEED_PASSWD_CARD_ERROR, 
			                              &pstCtx->stInsertInfo.stConnCtx,pstCtx->stInsertInfo.stAuthInfo.AccountLoginName);
	}
	else
	{	
		if (pstCtx->stInsertInfo.AuthFlag & AUTH_FLAG_REG)
		{
			iRet = auth_svr_allowed_respond(pstAppCtx, pstEnv, pstAccountAuth->PasswdHash, 
			                              pstAccountAuth, &pstCtx->stInsertInfo.stConnCtx );
		}
		else if (pstEnv->pstConf->NoxFlag && (pstCtx->stInsertInfo.AuthFlag & AUTH_FLAG_NOX))
		{
			iRet = auth_svr_allowed_respond(pstAppCtx, pstEnv, pstAccountAuth->PasswdHash, 
			                              pstAccountAuth, &pstCtx->stInsertInfo.stConnCtx );
		}
		else
		{
			if (pstCtx->stInsertInfo.AuthFlag & AUTH_FLAG_WEB_SIGN ||
					pstCtx->stInsertInfo.AuthFlag & AUTH_FLAG_SIGN)
			{
				SignInfoCache * pstSignCache = NULL;

				pstSignCache = auth_sign_info_cache_insert(pstEnv, 
										pstCtx->stInsertInfo.stAuthInfo.AccountLoginName, 
										pstCtx->stInsertInfo.stConnCtx.IP,
										pstCtx->stInsertInfo.stAuthInfo.WebUin);
					
				pstSignCache = auth_sign_info_cache_find(pstEnv, pstCtx->stInsertInfo.stAuthInfo.WebUin);
				if (!pstSignCache)
				{			
					iRet = auth_svr_phony_respond(pstEnv,pstCtx->stInsertInfo.stAuthInfo.RandK1,
								 		AUTH_IFM_WEB_SIGN_ERROR, &pstCtx->stInsertInfo.stConnCtx,
								 		pstCtx->stInsertInfo.stAuthInfo.AccountLoginName);
				}	
				else
				{
					if (pstSignCache->iSignInfoStat != SIGN_INFO_STAT_READY)
					{
						iRet = auth_sign_info_assign(pstEnv,pstSignCache,
										&pstCtx->stInsertInfo.stAccount,
										pstCtx->stInsertInfo.stConnCtx.iCltProtoVer,
										pstCtx->stInsertInfo.iWebUin);
					}
					else
					{
						iRet = 0;
					}

					if (0 == iRet)
					{
						iRet = auth_web_sign_respond(pstEnv,
									pstCtx->stInsertInfo.stAccount.PasswdHash,
									pstAccountAuth,
									&pstCtx->stInsertInfo.stConnCtx,
									AUTH_IFM_WEB_SIGN_SUCC,
									pstSignCache->szEnc,
									pstSignCache->iEncLen
									);
					}
					
				}
			}
			else
			{
					iRet = auth_svr_allowed_respond(pstAppCtx, pstEnv, pstAccountAuth->PasswdHash, 
					                              pstAccountAuth, &pstCtx->stInsertInfo.stConnCtx);
			}
		}
	}
	
	if(iRet < 0)
	{
		goto error;
	}

final:
	return iRet;

error:
	iRet = -1;
	auth_disconnect(pstEnv, &pstCtx->stInsertInfo.stConnCtx);
	goto final;
	return 0;
}

static void db_init_torm_head(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv, 
                              TORMPKGHEAD * pstHead,
                              int iCmd, 
                              int iCallbackCmd,
	                            const char *pszMetaName, 
	                            TormCallback * pstCallback, int iContextLen)
{
	pstHead->HeadComm.InstID = pstAppCtx->iId;
	pstHead->HeadComm.Magic = TORM_MAGIC;
	pstHead->HeadCmd.Cmd = iCmd;
	

	if (pszMetaName)
	{
		STRNCPY(pstHead->HeadCmd.MetaNameVer.MetaName, pszMetaName, 
			      sizeof(pstHead->HeadCmd.MetaNameVer.MetaName));
	}
	pstEnv->iTormSeq++;
	if (pstEnv->iTormSeq <= 0)
	{
		pstEnv->iTormSeq = 1;
	}
	
	pstHead->HeadCmd.MetaNameVer.MetaVer = 0;
	pstHead->HeadApp.AppCmd = 0;
	pstHead->HeadApp.Seq = pstEnv->iTormSeq;
	pstHead->HeadApp.ObjID = 0;
	pstHead->AsynCallBackDataLen = 0;
	if(pstCallback != NULL)
	{
		int n;
		
		pstCallback->iMatchedCmd = iCallbackCmd;
		n = sizeof(pstCallback->iMatchedCmd);
		memcpy(pstHead->AsynCallBackData, &pstCallback->iMatchedCmd, n);
		pstHead->AsynCallBackDataLen += n;
		
		//n = sizeof(pstCallback->pfnOnBack);
		//memcpy(pstHead->AsynCallBackData + pstHead->AsynCallBackDataLen, 
		//	     &pstCallback->pfnOnBack, n);
		//pstHead->AsynCallBackDataLen += n;
		
		n = iContextLen;
		if(n > 0)
		{
			memcpy(pstHead->AsynCallBackData + pstHead->AsynCallBackDataLen, 
				     &pstCallback->stContext, n);
			pstHead->AsynCallBackDataLen += n;
		}
	}
}

static int db_torm_send(TAPPCTX* pstAppCtx, AUTH_SVRENV* pstEnv, TORMPKG *pstPkg,
	                      int iDstID)
{
	TDRDATA stData;
	int iSrc;
	int iDst;
	int iProtoVer;

	if (iDstID == pstEnv->iAccountDBID)
	{
		iProtoVer = pstEnv->iAccountDBProtoVer;
	}

	stData.pszBuff = pstEnv->szSendBuffer;
	stData.iBuff = sizeof(pstEnv->szSendBuffer);
	if(0 != torm_pack_msg(pstEnv->hTorm, &stData, pstPkg, iProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_pack_msg error");
		return -1;
	}
	
	iSrc = pstAppCtx->iId;
	iDst = iDstID;
	if (0 > tbus_send(pstAppCtx->iBus, &iSrc, &iDst, stData.pszBuff, stData.iBuff, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail");
		return -1;
	}

	return 0;
}

static int db_send_req(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv, 
	                     TORMPKG *pstTormPkg, const void *pvData, int iDataLen)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pvData;
	stMetaData.iBuff = iDataLen;
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstTormPkg, &stMetaData, 
		                        pstEnv->iAccountDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > db_torm_send(pstAppCtx, pstEnv, pstTormPkg, pstEnv->iAccountDBID))
	{
		return -1;
	}

	return 0;	
}

static int db_req_insert_account(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv, 
	                               const ACCOUNT * pstAccount, 
	                               TormCallback * pstCallback)
{
	TORMPKG stPkg;
	TORMINSERTREQ *pstInsertReq = &stPkg.Body.InsertReq;
		
	db_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_INSERT_REQ, 
			              TORM_INSERT_RES, "Account", pstCallback, 
			              sizeof(pstCallback->stContext.stInsertInfo));
	
	pstInsertReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstInsertReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstInsertReq->OperInfo.WhereDef[0] = 0;

	return db_send_req(pstAppCtx, pstEnv, &stPkg, pstAccount, sizeof(*pstAccount));
}

static int db_req_insert_ipblock(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv, 
	                               IPBLOCK* pstIpBlock)
{
	TORMPKG stPkg;
	TORMINSERTREQ *pstInsertReq = &stPkg.Body.InsertReq;
		
	db_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_INSERT_REQ, 0, "IPBlock", NULL, 0);
	
	pstInsertReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstInsertReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstInsertReq->OperInfo.WhereDef[0] = 0;

	return db_send_req(pstAppCtx, pstEnv, &stPkg, pstIpBlock, sizeof(*pstIpBlock));
}

static int db_req_select_account(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv, 
	                               const ACCOUNT * pstAccount, 
	                               TormCallback * pstCallback)
{
	TORMPKG stPkg;
	TORMSELECTREQ *pstSelectReq = &stPkg.Body.SelectReq;

	db_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_SELECT_REQ, 
			              TORM_SELECT_RES, "Account", pstCallback, 
			              sizeof(pstCallback->stContext.stSelectInfo));
	
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	snprintf(pstSelectReq->OperInfo.WhereDef, 
		       sizeof(pstSelectReq->OperInfo.WhereDef),
		       "where Pass9Account=\'%s\'",
		       pstAccount->Pass9Account);

	return db_send_req(pstAppCtx, pstEnv, &stPkg, pstAccount, sizeof(*pstAccount));
}

static int db_req_update_account_freeze(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv,
	                                      const ACCOUNTFREEZEENDTIME *pstDummy,
	                                      TormCallback *pstCallback)
{
	TORMPKG stPkg;
	TORMUPDATEREQ *pstUpdateReq = &stPkg.Body.UpdateReq;

	db_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_UPDATE_REQ, 
			              TORM_UPDATE_RES, "AccountFreezeEndTime", pstCallback, 
			              sizeof(pstCallback->stContext.stUpdateInfo));
	
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;

	return db_send_req(pstAppCtx, pstEnv, &stPkg, pstDummy, sizeof(*pstDummy));
}

static void auth_selectinfo_assign(AUTH_SVRENV* pstEnv,
									SelectInfo *pstSelectInfo, 
	                               const AUTHINFO * pstAuthInfo,
		                            const ConnCtx *pstConnCtx,
		                            const char cActiveFlag,
		                            const char* szActiveCode,
		                            int iAuthFlag,
		                            int iUin,
		                            int iCltType,
		                            PASSWDCARDAUTHINFO *pstPasswdCardAuthInfo)
{
	int iAuthMode = pstEnv->pstConf->AuthMode;

	//debug 用
	if (pstEnv->IP == 
		pstEnv->uiDebugIP)
	{
		iAuthMode = pstEnv->pstConf->DebugAuthMode;		
	}
	
	memset(pstSelectInfo,0,sizeof(*pstSelectInfo));
	
	pstSelectInfo->stAuthInfo = *pstAuthInfo;
	pstSelectInfo->stConnCtx = *pstConnCtx;
	pstSelectInfo->ActiveFlag = cActiveFlag;
	snprintf(pstSelectInfo->ActiveCode,INFORMER_ACTIVECODE_LEN,"%s",szActiveCode);
	pstSelectInfo->AuthFlag = iAuthFlag;
	pstSelectInfo->AuthMode = iAuthMode;


	pstSelectInfo->AskActiveKeyStatusFlag = 0;
	pstSelectInfo->Uin = iUin;
	pstSelectInfo->CltType = iCltType;
	pstSelectInfo->SelectType = 0;
	pstSelectInfo->stPasswdCardAuthInfo = *pstPasswdCardAuthInfo;
	pstSelectInfo->UinType = 0;
	pstSelectInfo->iIP = pstConnCtx->IP;
	pstSelectInfo->iWebUin = pstAuthInfo->WebUin;
}


														
// 请求获取account信息。
static int db_fetch_account(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv, 
                            const AUTHINFO * pstAuthInfo,
                            const ConnCtx *pstConnCtx,
                            const char cActiveFlag,
                            const char* szActiveCode,
                            int iAuthFlag,
                            int iUin,
                            int iCltType,
                            PASSWDCARDAUTHINFO *pstPasswdCardAuthInfo)
{
	TormCallback stCallback;
	ACCOUNT stAccount;


	
	
	auth_selectinfo_assign(pstEnv,
							&stCallback.stContext.stSelectInfo,
							pstAuthInfo,
							pstConnCtx,
							cActiveFlag,
							szActiveCode,
							iAuthFlag,
							iUin,
							iCltType,
							pstPasswdCardAuthInfo);
	
	
	bzero(&stAccount, sizeof(stAccount));
	STRNCPY(stAccount.Pass9Account, pstAuthInfo->AccountLoginName,
		      sizeof(stAccount.Pass9Account));
	return db_req_select_account(pstAppCtx, pstEnv, &stAccount, &stCallback);
}


int db_req_select_account_role(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv, 
							ACCOUNTROLE *pstAccountRole, TormCallback *pstCallback)
{
	TORMPKG stPkg;
	TORMSELECTREQ *pstSelectReq = &stPkg.Body.SelectReq;

	db_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_SELECT_REQ, 
			              TORM_SELECT_RES, "AccountRole", pstCallback, 
			              sizeof(pstCallback->stContext.stSelectInfo));
	
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->Limit = ACC_ROLE_MAX;
	snprintf(pstSelectReq->OperInfo.WhereDef, 
		       sizeof(pstSelectReq->OperInfo.WhereDef),
		       "where Pass9Account=\'%s\' and Uin=%d && DelTime=0 order by LastLogout Desc",
		       pstAccountRole->Pass9Account, pstAccountRole->Uin);

	return db_send_req(pstAppCtx, pstEnv, &stPkg, pstAccountRole, sizeof(*pstAccountRole));
}

static int db_fetch_account_role(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv, 
							const ConnCtx *pstConnCtx, ACCGETROLECLT *pstAccGetRoleList)
{
	TormCallback stCallback;
	ACCOUNTROLE stAccountRole;
	char szDec[RSA_ENC_LEN];
	char szDec1[AUTH_LEN+32];
	int iLen;
	TDRDATA stHost, stNet;
	ACCGETROLEINFO stAccGetRoleInfo;
	SIGINFO stSigInfo;

	memset(&stAccountRole, 0, sizeof(stAccountRole));

	stCallback.stContext.stSelectInfo.SelectType = 1;
	stCallback.stContext.stSelectInfo.stConnCtx = *pstConnCtx;

	if (0 > rsa_private_decrypt(pstEnv->pstRsa, sizeof(pstAccGetRoleList->Enc), 
		                          pstAccGetRoleList->Enc, szDec))
	{
		return -1;
	}

	stHost.iBuff = sizeof(stAccGetRoleInfo);
	stHost.pszBuff = (char *)&stAccGetRoleInfo;
	stNet.iBuff = sizeof(szDec);
	stNet.pszBuff = szDec;
	if (0 > tdr_ntoh(pstEnv->pstAuthRoleInfoMeta, &stHost, &stNet, pstConnCtx->iCltProtoVer))
	{
		return -1;
	}

	iLen = sizeof(szDec1);
	if (0 > taes_decrypt(stAccGetRoleInfo.Sig, stAccGetRoleInfo.SigLen, &pstEnv->stAESCtx, szDec1, &iLen))
	{
		return -1;
	}
	stHost.pszBuff = (char *)&stSigInfo;
	stHost.iBuff = sizeof(stSigInfo);
	stNet.pszBuff = szDec1;
	stNet.iBuff = iLen;
	if (0 > tdr_ntoh(pstEnv->pstSigMeta, &stHost, &stNet, 0))
	{
		return -1;
	}
	STRNCPY(stAccountRole.Pass9Account, stAccGetRoleInfo.AccountName, sizeof(stAccountRole.Pass9Account));
	stAccountRole.Uin = stSigInfo.Uin;
	
	return db_req_select_account_role(pstAppCtx, pstEnv, &stAccountRole, &stCallback);

}
static void auth_connctx_assign(ConnCtx *pstConnCtx, 
	                              const TFRAMEHEAD *pstFrameHead, int iCltProtoVer)
{
	memset(pstConnCtx,0,sizeof(*pstConnCtx));
	
	pstConnCtx->chExtraType = pstFrameHead->ExtraType;
	pstConnCtx->chTimeStampType = pstFrameHead->TimeStampType;
	pstConnCtx->chVer = pstFrameHead->Ver;
	pstConnCtx->iConnIdx = pstFrameHead->ConnIdx;
	pstConnCtx->iID = pstFrameHead->ID;
	pstConnCtx->stExtraInfo = pstFrameHead->ExtraInfo;
	pstConnCtx->stTimeStamp = pstFrameHead->TimeStamp;
	pstConnCtx->iCltProtoVer = iCltProtoVer;
}

static void auth_framehead_assign(TFRAMEHEAD *pstFrameHead, 
	                                const ConnCtx *pstConnCtx)
{
	pstFrameHead->ExtraType = pstConnCtx->chExtraType;
	pstFrameHead->TimeStampType = pstConnCtx->chTimeStampType;
	pstFrameHead->Ver = pstConnCtx->chVer;
	pstFrameHead->ConnIdx = pstConnCtx->iConnIdx;
	pstFrameHead->ID = pstConnCtx->iID;
	pstFrameHead->ExtraInfo = pstConnCtx->stExtraInfo;
	pstFrameHead->TimeStamp = pstConnCtx->stTimeStamp;
}

static int auth_svr_fill_sign_buf(AUTH_SVRENV* pstEnv, 
							AUTHINFO *pstAuthInfo,
                             char * pszEnc, int iSize, 
                             const ACCOUNTAUTHINFO * pstAccount, int iCltProtoVer,int iWebUin)
{
	TDRDATA stHost, stNet;
	//SIGINFO stSigInfo;
	//int iPad;
	int iLen = 0;
	int iOffset = 0;
	char* p;
	int iRet;
	char szRsaEnc[RSA_ENC_LEN];
	char szRsaEncHex[513];
	RSA *pstRsa = NULL;
	int iHexLen = 0;

	((void)iCltProtoVer);

	//扰乱
	pszEnc[0] = (char)((random() & 0xff000000) >> 24);
	pszEnc[1] = AUTH_IFM_WEB_SIGN_SUCC;

	iOffset = 2 ;	
	
	//gamekey
	//rand_str(pszEnc + 2, KEY_LEN);
	memset(pstAuthInfo,0,sizeof(*pstAuthInfo));
	
	rand_str(pstAuthInfo->RandK1,KEY_LEN);
	//rand_str(pstAuthInfo->AccountLoginName,sizeof(pstAuthInfo->AccountLoginName));
	//rand_str(pstAuthInfo->AccountLoginPasswd,sizeof(pstAuthInfo->AccountLoginPasswd));
	//rand_str(pstAuthInfo->CertID,sizeof(pstAuthInfo->CertID));
	//rand_str(pstAuthInfo->eMail,KEY_LEN);
	//rand_str(pstAuthInfo->Mobile,sizeof(pstAuthInfo->Mobile));
	//rand_str(pstAuthInfo->RealName,sizeof(pstAuthInfo->RealName));
	//rand_str(pstAuthInfo->Birthday,sizeof(pstAuthInfo->Birthday));

	p = &(pszEnc[iOffset]);
	
	memcpy(p,pstAuthInfo->RandK1,KEY_LEN);

	iOffset += KEY_LEN;
	
	pstAuthInfo->WebUin = iWebUin;
	pstAuthInfo->AuthInfoType = AUTH_INFO_TYPE_GET_SIGN;	
	
	STRNCPY(pstAuthInfo->AccountLoginName,pstAccount->Pass9Account,
		sizeof(pstAuthInfo->AccountLoginName));						
	
	// pstEnv->szSendBuffer作临时buffer.
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = pstEnv->szSendBuffer;
	stHost.iBuff = sizeof(*pstAuthInfo);
	stHost.pszBuff = (char *)pstAuthInfo;
	iRet = tdr_hton(pstEnv->pstAuthMeta, &stNet, &stHost, 0);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tdr_hton fail:%s",
					tdr_error_string(iRet));
		return -1;
	}

	// pstEnv->szRecvBuffer作临时buffer.
	//iLen = sizeof(pstEnv->szRecvBuffer);

	pstRsa = rsa_from_public_key(IDRSAPublicKey);

	memset(szRsaEnc,0,sizeof(szRsaEnc));
	
	if (0 > rsa_public_encrypt(pstRsa, stNet.iBuff, stNet.pszBuff, szRsaEnc))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "rsa_public_encrypt fail");
		return -1;
	}

	

	iLen = sizeof(szRsaEnc);
	

	/*
	if (0 > taes_encrypt(stNet.pszBuff, stNet.iBuff, &pstEnv->stAESCtx, 
				               pstEnv->szRecvBuffer, &iLen))
	{
		return -1;
	}*/

	if (iLen > iSize - iOffset - 32)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "size check fail:iLen=%d,iSize=%d,iOffset=%d",
					iLen,iSize,iOffset);
		return -1;
	}

	memcpy(pszEnc + iOffset, szRsaEnc, iLen);

	iHexLen = sizeof(szRsaEncHex);
	memset(szRsaEncHex,0,sizeof(szRsaEncHex));
	
	//Bin2Hex((unsigned char *)pszEnc + 2,iSize - 2, szRsaEncHex,&iHexLen);
	Bin2Hex((unsigned char *)szRsaEnc,sizeof(szRsaEnc), szRsaEncHex,&iHexLen);
	
	
	tlog_debug(pstEnv->pstLogCat, 0, 0, "ras enc:iHexLen=%d iLen=%d szRsaEnc=%s",
					iHexLen,iLen,szRsaEncHex);
	return iLen;
}


// 填充Auth Buffer
/*
 * +-+-+--------+---+---------+
 * |r|0|game-key|pad|E-SIGINFO|
 * +-+-+--------+---+---------+
 *
 */
static int auth_svr_fill_buf(TAPPCTX* pstAppCtx, AUTH_SVRENV* pstEnv, 
                             char * pszEnc, int iSize, 
                             const ACCOUNTAUTHINFO* pstAccountInfo, int iCltProtoVer,
                             int iOffset)
{
	TDRDATA stHost, stNet;
	SIGINFO stSigInfo;
	int iPad, iLen = 0;

	((void)iCltProtoVer);
	
	//gamekey
	rand_str(pszEnc + iOffset, KEY_LEN);
			
	stSigInfo.TimeStamp = pstAppCtx->stCurr.tv_sec;
	stSigInfo.Uin = pstAccountInfo->Uin;
	stSigInfo.UinFlag = pstAccountInfo->UinFlag;
	stSigInfo.AccSilenceEnd = pstAccountInfo->SilenceEndTime;
	STRNCPY(stSigInfo.BirthDay, pstAccountInfo->BirthDay, sizeof(stSigInfo.BirthDay));
	memcpy(stSigInfo.GameKey, pszEnc + iOffset, KEY_LEN);

	iPad = (int)HTONL((long)stSigInfo.Uin);
	memcpy(pszEnc + iOffset + KEY_LEN, &iPad, sizeof(int));

	// pstEnv->szSendBuffer作临时buffer.
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = pstEnv->szSendBuffer;
	stHost.iBuff = sizeof(stSigInfo);
	stHost.pszBuff = (char *)&stSigInfo;
	if (0 > tdr_hton(pstEnv->pstSigMeta, &stNet, &stHost, 0))
	{
		return -1;
	}

	// pstEnv->szRecvBuffer作临时buffer.
	iLen = sizeof(pstEnv->szRecvBuffer);
	if (0 > taes_encrypt(stNet.pszBuff, stNet.iBuff, &pstEnv->stAESCtx, 
				               pstEnv->szRecvBuffer, &iLen))
	{
		return -1;
	}

	if (iLen > iSize - iOffset - KEY_LEN - (int)sizeof(int) - 32)
	{
		return -1;
	}

	memcpy(pszEnc + iOffset + KEY_LEN + sizeof(int), pstEnv->szRecvBuffer, iLen);
	iLen += iOffset + KEY_LEN + sizeof(int);
	
	return iLen;
}

int auth_svr_msg_send_with_frame_cmd(AUTH_SVRENV *pstEnv, CSPKG *pstCSPkgRes, 
	                    const ConnCtx *pstConnCtx,char cCmd)
{
	TDRDATA stNet;

	if (0 > pstConnCtx->iConnIdx)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "iConnIdx %d", pstConnCtx->iConnIdx);
	}
		
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = &pstEnv->szSendBuffer[0];
	if (0 > net_cs_pack(pstEnv->pstCSHan, &stNet, pstCSPkgRes, 
		                  pstConnCtx->iCltProtoVer))
	{
		auth_disconnect(pstEnv, pstConnCtx);
		return -2;
	}

	auth_framehead_assign(&pstEnv->stFrameHead, pstConnCtx);
	pstEnv->stFrameHead.Cmd = cCmd;
	pstEnv->stFrameHead.TimeStampType = 0;
	//pstEnv->stFrameHead.ID = -1;
	pstEnv->stFrameHead.CmdData.Stop.Reason = 0;

	if (pstEnv->stFrameHead.Cmd == TFRAMEHEAD_CMD_START)
	{
		pstEnv->stFrameHead.ID = pstConnCtx->iConnIdx;
	}

	tlog_info(pstEnv->pstLogCat, 0, 0, "auth_svr_msg_send iConnID:%d,Cmd:%d,IP:%s,Port:%d",  
											pstEnv->iConnID,
											pstCSPkgRes->Head.Cmd,
											inet_ntoa(*(struct in_addr *)&pstConnCtx->IP),
											pstEnv->stFrameHead.ExtraInfo.IPInfo.port);
	
	return tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iConnID, stNet.pszBuff, 
		                   stNet.iBuff, &pstEnv->stFrameHead);
}

WebSignClient *auth_web_sign_client_find(AUTH_SVRENV *pstEnv,
									int iConnIdx)
{
	WebSignClient stDummy;
	WebSignClient * pstWebSignClient = NULL;

	stDummy.iConnIdx = iConnIdx;

	pstWebSignClient = (WebSignClient *)sht_find(pstEnv->pstWebSignClientCache, &stDummy, WebSignClientHashCmp, WebSignClientHashCode);
	
	return pstWebSignClient;
}



// 身份验证应答
static int auth_svr_respond(AUTH_SVRENV* pstEnv, const char * pszKey,
                            const char * pszEnc, int iLen,
                            const ConnCtx *pstConnCtx,int iFrameCmd,char * pszAccount)
{
	CSPKG stCsPkgSvr;
	CSAUTHSVR *pstAuthSvr = &stCsPkgSvr.Body.AuthSvr;
	AES stAes;
	int iRet = 0;
	int i = 0;
	PASSWDCARDPOSINFO *pstPosInfo = NULL;
	char szStr[128];
	char szPosX[2];
	int iCodeLen = 0;
//	ConnCtx *pstConnCtx2 = (ConnCtx *)pstConnCtx;
	PASSWDCARDAUTHSVR *pstPasswdCardAuthSvr = NULL; //密保卡验证信息
	PasswdCardLimit *pstPasswdCardLimit = NULL;
	char cResult = 0;
	WebSignClient * pstWebSignClient= NULL;

	memset(pstAuthSvr,0,sizeof(*pstAuthSvr));
	memset(szPosX,0,sizeof(szPosX));
	
	if (0 > taes_setkey(&stAes, (unsigned char *)pszKey))
	{
		return -1;
	}

	cResult = pszEnc[1];

	pstAuthSvr->AuthLen = sizeof(pstAuthSvr->Auth);
	if (0 > taes_encrypt(pszEnc,iLen, &stAes, 
		                   pstAuthSvr->Auth, &pstAuthSvr->AuthLen))
	{
		return -1;
	}

	if (cResult == AUTH_REASON_NEED_PASSWD_CARD_ERROR)
	{
		
		auth_passwd_card_limit_insert(pstEnv,pszAccount,
			pstConnCtx->stExtraInfo.IPInfo.ip,pstConnCtx->iConnIdx);

		pstPasswdCardLimit = auth_passwd_card_limit_find(pstEnv,pstConnCtx->iConnIdx);

		if (!pstPasswdCardLimit)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "auth_passwd_card_limit_find fail,item:%d max:%d connidx:%d account:%s",
			pstEnv->pstPasswdCardLimitCache->iItem, pstEnv->pstPasswdCardLimitCache->iMax,
			pstConnCtx->iConnIdx);
		}
		else
		{
			pstPasswdCardLimit->iTryNum++;
				
			pstPosInfo = &pstPasswdCardLimit->stPasswdCardPosInfo;
			pstPasswdCardAuthSvr = &pstAuthSvr->PasswdCardAuthSvr;
			pstPosInfo->Num = 0;
			for (i = 0; i < MAX_GD_IMAGE_NUM; i++)
			{
				int iY = 0;
				pstPosInfo->Pos[i].X = RAND1(pstPasswdCardLimit->iPasswdCardColumnNum);
				pstPosInfo->Pos[i].Y = RAND1(pstPasswdCardLimit->iPasswdCardRowNum);
				pstPosInfo->Num++;

				szPosX[0] = 'A' + pstPosInfo->Pos[i].X;
				iY = pstPosInfo->Pos[i].Y + 1;
				if (iY == 10)
				{
					iY = 0;
				}
				

				snprintf(szStr,sizeof(szStr),"%s%d",szPosX,iY);
				iCodeLen = strlen(szStr);

				if (0 > ptg_gen_image(szStr, iCodeLen, 
					pstPasswdCardAuthSvr->Image[i].Data, 
					&pstPasswdCardAuthSvr->Image[i].Len))
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "ptg_gen_image fail");
				}
				pstPasswdCardAuthSvr->ImageNum++;
			}
		}	
	}

	stCsPkgSvr.Head.Magic = PROTOCOL_MAGIC;
	stCsPkgSvr.Head.Cmd = AUTH_SVR;
	stCsPkgSvr.Head.Ver = TDR_METALIB_PROTO_CS_VERSION;
	
	if (cResult == AUTH_IFM_WEB_SIGN_ERROR)
	{
		//保持连接
		pstWebSignClient = auth_web_sign_client_find(pstEnv,pstConnCtx->iConnIdx);
		if (pstWebSignClient)
		{
			iFrameCmd = TFRAMEHEAD_CMD_START;
		}
	}
	else if (cResult == AUTH_REASON_NEED_PASSWD_CARD_ERROR)
	{
		iFrameCmd = TFRAMEHEAD_CMD_START;
	}

	if (iFrameCmd == TFRAMEHEAD_CMD_STOP)
	{
		iRet = auth_svr_msg_send(pstEnv, &stCsPkgSvr, pstConnCtx);
	}
	else
	{
		iRet = auth_svr_msg_send_with_frame_cmd(pstEnv, &stCsPkgSvr,pstConnCtx,iFrameCmd);
	}
	
	if( iRet < 0 )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "auth_svr_msg_send fail %d authlen:%d",
			iRet,pstAuthSvr->AuthLen);
		
		pstEnv->pstRunStat->SendCltMsgFail++;
	}
	else
	{
		tlog_debug(pstEnv->pstLogCat, 0, 0, "auth_svr_msg_send iConnID:%d,Cmd:%d,IP:%s,Port:%d,Result:%d",  
											pstEnv->iConnID,
											stCsPkgSvr.Head.Cmd,
											inet_ntoa(*(struct in_addr *)&pstConnCtx->IP),
											(int)cResult);
	}

	if (cResult == AUTH_REASON_PASSWD_CARD_ERROR )
	{		
		auth_disconnect(pstEnv,pstConnCtx);
		auth_passwd_card_limit_flush(pstEnv,pstConnCtx->iConnIdx);
	}
	
	return iRet;
}

// 身份验证通过，发送回应信息。
static int auth_svr_allowed_respond(TAPPCTX *pstAppCtx, AUTH_SVRENV* pstEnv, 
                                    const char * pszKey,
                                    const ACCOUNTAUTHINFO * pstAccount,
                                    const ConnCtx *pstConnCtx)
{
	char szEnc[AUTH_LEN];
	int iLen;
	int iNameLen = 0;
	int iNameLenNet = 0;
	int iOffset = 0;
	char *p = NULL;

	//扰乱
	szEnc[0] = (char)((random() & 0xff000000) >> 24);
	szEnc[1] = AUTH_REASON_SUCCEEDED;

	iNameLen = strlen(pstAccount->Pass9Account) + 1;	

	if (iNameLen >= (int)(sizeof(szEnc) - 2 - sizeof(int)))
	{
		goto error;
	}

	iNameLenNet = (int)HTONL((long)iNameLen);

	memcpy(&szEnc[2], &iNameLenNet, sizeof(int));

	iOffset = 2 + sizeof(int);

	p = &(szEnc[iOffset]);
	
	STRNCPY(p,pstAccount->Pass9Account,iNameLen);

	iOffset = iNameLen + 2 + sizeof(int);
		
	iLen = auth_svr_fill_buf(pstAppCtx, pstEnv, szEnc, sizeof(szEnc), pstAccount, 
		                       pstConnCtx->iCltProtoVer,iOffset); 
	if( iLen < 0 )
	{
		goto error;
	}

	if (iLen >= (int)sizeof(szEnc) - 32)
	{
		//名字太长?
		tlog_error(pstEnv->pstLogCat, 0, 0, "fill buf err: iLen=%d iNameLen=%d",
			iLen,iNameLen);
		goto error;
	}
	
	return auth_svr_respond(pstEnv, pszKey, szEnc, iLen, pstConnCtx,TFRAMEHEAD_CMD_STOP,(char*)pstAccount->Pass9Account);
	
error:
		auth_disconnect(pstEnv, pstConnCtx);
		return -1;
	
}

static int auth_sign_info_assign( AUTH_SVRENV* pstEnv, 
                                    SignInfoCache * pstSignCache,
                                    const ACCOUNTAUTHINFO* pstAccount,
                                    int iCltProtoVer,int iWebUin)
{
	int iFillLen = 0;
	int iOffset = 0;

	memset(pstSignCache->szEnc,0,sizeof(pstSignCache->szEnc));
	
	iFillLen = auth_svr_fill_sign_buf(pstEnv,
										&pstSignCache->stAuthInfo,
										pstSignCache->szEnc,
										sizeof(pstSignCache->szEnc), 
										pstAccount, 
			                       		iCltProtoVer,iWebUin); 
	if( iFillLen < 0 )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "auth_svr_fill_sign_buf fail:%d",iFillLen);
		return -1;
	}
			
	memcpy(&pstSignCache->stAccountAuthInfo,pstAccount,
			sizeof(pstSignCache->stAccountAuthInfo));
	
	iOffset = 2 + KEY_LEN;
	
	Md5HashBuffer(pstSignCache->szMd5,&pstSignCache->szEnc[iOffset], iFillLen);
	iOffset++;
	pstSignCache->szMd5[0] = pstSignCache->szEnc[iOffset];
	iOffset++;
	pstSignCache->szMd5[1] = pstSignCache->szEnc[iOffset];
	Md5HashBuffer(pstSignCache->szMd5,pstSignCache->szMd5,sizeof(pstSignCache->szMd5));

	pstSignCache->iEncLen = iFillLen + 2 + KEY_LEN;
	pstSignCache->iSignInfoStat = SIGN_INFO_STAT_READY;

	tlog_info(pstEnv->pstLogCat, 0, 0, "auth_sign_info_assign Uin:%d WebUin:%d",  
											pstAccount->Uin,iWebUin);

	{
		char szMd5Hex[33];
		int iHexLen = sizeof(szMd5Hex);
		szMd5Hex[0] = 0;
		
		Bin2Hex((unsigned char*)pstSignCache->szMd5, sizeof(pstSignCache->szMd5),szMd5Hex, &iHexLen);

		tlog_debug(pstEnv->pstLogCat, 0, 0, "auth_sign_info_assign md5:%s",  
											szMd5Hex);
		
	}
		
	return 0;
}


// 生成web签名的返回结果
static int auth_web_sign_respond( AUTH_SVRENV* pstEnv, 
                                    const char * pszKey,
                                    const ACCOUNTAUTHINFO* pstAccount,
                                    const ConnCtx *pstConnCtx,int iCode,char *szEnc,int iLen)
{	
	tlog_info(pstEnv->pstLogCat, 0, 0, "auth_web_sign_respond iConnID:%d",  
											pstEnv->iConnID);
	if (pstConnCtx->iID == -1)
	{
		return auth_svr_respond(pstEnv, pszKey, szEnc, iLen, pstConnCtx,TFRAMEHEAD_CMD_STOP,(char *)pstAccount->Pass9Account);
	}
	else
	{
		return auth_svr_respond(pstEnv, pszKey, szEnc, iLen, pstConnCtx,TFRAMEHEAD_CMD_START,
				(char *)pstAccount->Pass9Account);
	}
	
	UNUSED(pstAccount);	//add by paraunused.pl
	UNUSED(iCode);	//add by paraunused.pl
}


// 回应一个假信息，试图挫败客户端猜测密码的企图。
static int auth_svr_phony_respond(AUTH_SVRENV* pstEnv, const char * pszKey,
                                  int iReason, const ConnCtx *pstConnCtx,char * pszAccount)
{
	char szEnc[AUTH_LEN];
	int iLen;

	if (iReason == AUTH_REASON_NEED_PASSWD_CARD_ERROR )
	{		
		PasswdCardLimit* pstPasswdCardLimit = auth_passwd_card_limit_find(pstEnv,pstConnCtx->iConnIdx);

		if (pstPasswdCardLimit )
		{
			if (pstPasswdCardLimit->iTryNum >= PASSWD_CARD_MAX_TRY)
			{
				iReason = AUTH_REASON_PASSWD_CARD_ERROR;
				
				tlog_info(pstEnv->pstLogCat, 0, 0, "passwd card try max,connidx:%d account:%s",					
					pstConnCtx->iConnIdx,pszAccount);
			}
		}
		else
		{
			iReason = AUTH_REASON_PASSWD_CARD_ERROR;
			
			tlog_error(pstEnv->pstLogCat, 0, 0, "auth_passwd_card_limit_find fail,item:%d max:%d connidx:%d account:%s",
				pstEnv->pstPasswdCardLimitCache->iItem, pstEnv->pstPasswdCardLimitCache->iMax,
				pstConnCtx->iConnIdx,pszAccount);
		}
	}

	//扰乱
	szEnc[0] = (char)((random() & 0xff000000) >> 24);
	szEnc[1] = iReason;
	//iLen = sizeof(szEnc) - (2 + KEY_LEN + sizeof(int)) - 32;
	//rand_str(szEnc + 2, iLen); //扰乱
	rand_str(szEnc + 2, sizeof(szEnc)-2);
	
	iLen = 80;

	if (iReason == AUTH_IFM_WEB_SIGN_ERROR)
	{
		tlog_debug(pstEnv->pstLogCat, 0, 0, "auth_web_sign_respond IP:%s,Port:%d", 
					inet_ntoa(*(struct in_addr *)&pstConnCtx->IP),
					pstConnCtx->stExtraInfo.IPInfo.port);
	}
	
	return auth_svr_respond(pstEnv, pszKey, szEnc, iLen, pstConnCtx,TFRAMEHEAD_CMD_STOP,pszAccount);
}

// 重置冻结时间
static int auth_reset_freeze(TAPPCTX* pstAppCtx, AUTH_SVRENV *pstEnv, 
                             const ACCOUNT *pstAccount, 
                             const ConnCtx *pstConnCtx)
{
	TormCallback stCallback;
	ACCOUNTFREEZEENDTIME stDummy;

	stDummy.Uin = pstAccount->Uin;
	stDummy.FreezeStartTime = 0;
	stDummy.FreezeEndTime = 0;
	stDummy.FreezeStartTime = 0;
	memset(stDummy.FreezeReason,0,sizeof(stDummy.FreezeReason));

	stCallback.stContext.stUpdateInfo.stConnCtx = *pstConnCtx;
	STRNCPY(stCallback.stContext.stUpdateInfo.szLoginName,
		      pstAccount->Pass9Account,
		      sizeof(stCallback.stContext.stUpdateInfo.szLoginName));

	return db_req_update_account_freeze(pstAppCtx, pstEnv, &stDummy, &stCallback);
}


SignInfoCache * auth_sign_info_cache_insert(AUTH_SVRENV *pstEnv,
									char *pszAccount, tdr_ip_t ip,int iWebUin)
{
	SignInfoCache *pstSignInfoCache = NULL;
	SignInfoCache stDummy;

	if (!pszAccount)
	{
		pszAccount = "";
	}

	
	//STRNCPY(stDummy.szAccountName, pstAccount->Pass9Account, sizeof(stDummy.szAccountName));
	stDummy.iWebUin = iWebUin;
	//strlwr(stDummy.szAccountName);

	pstSignInfoCache = sht_find(pstEnv->pstSignInfoCacheCache, &stDummy, SignInfoCacheHashCmp, SignInfoCacheHashCode);
	if (pstSignInfoCache)
	{
		if (pstEnv->pstAppCtx->stCurr.tv_sec - pstSignInfoCache->tTime <= SIGN_INFO_CACHETIME &&
			pstSignInfoCache->iSignInfoStat == SIGN_INFO_STAT_READY)
		{
			
		}
		else
		{
			memset(pstSignInfoCache,0,sizeof(*pstSignInfoCache));
			STRNCPY(pstSignInfoCache->szAccountName, pszAccount, sizeof(pstSignInfoCache->szAccountName));
			pstSignInfoCache->iSignInfoStat = SIGN_INFO_STAT_INIT;
			pstSignInfoCache->tTime = pstEnv->pstAppCtx->stCurr.tv_sec;
			pstSignInfoCache->IP = ip;
			pstSignInfoCache->iWebUin = iWebUin;

			tlog_info(pstEnv->pstLogCat, 0, 0, "sign_info_cache_insert,webuin:%d account:%s ip:%s time:%d stat:%d",
					iWebUin,pstSignInfoCache->szAccountName, inet_ntoa(*(struct in_addr *)&pstSignInfoCache->IP),
					pstSignInfoCache->tTime, pstSignInfoCache->iSignInfoStat);
		}
	}
	else
	{
		if (pstEnv->pstSignInfoCacheCache->iItem >= pstEnv->pstSignInfoCacheCache->iMax)
		{
			auth_flush_sign_info_cache(pstEnv, 10, 1);
		}

		pstSignInfoCache = sht_insert_unique(pstEnv->pstSignInfoCacheCache, &stDummy, SignInfoCacheHashCmp, SignInfoCacheHashCode);
		if (pstSignInfoCache)
		{
			memset(pstSignInfoCache,0,sizeof(*pstSignInfoCache));
			STRNCPY(pstSignInfoCache->szAccountName, pszAccount, sizeof(pstSignInfoCache->szAccountName));
			pstSignInfoCache->iSignInfoStat = SIGN_INFO_STAT_INIT;
			pstSignInfoCache->tTime = pstEnv->pstAppCtx->stCurr.tv_sec;
			pstSignInfoCache->IP = ip;
			pstSignInfoCache->iWebUin = iWebUin;

			tlog_info(pstEnv->pstLogCat, 0, 0, "sign_info_cache_insert,webuin:%d account:%s ip:%s time:%d stat:%d",
					iWebUin,pstSignInfoCache->szAccountName, inet_ntoa(*(struct in_addr *)&pstSignInfoCache->IP),
					pstSignInfoCache->tTime, pstSignInfoCache->iSignInfoStat);
		}
	}

	if (pstSignInfoCache)
	{
		return pstSignInfoCache;
	}
	else
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "sign_info_cache_insert fail,item:%d max:%d webuin:%d account:%s",
			pstEnv->pstSignInfoCacheCache->iItem, pstEnv->pstSignInfoCacheCache->iMax,
			iWebUin,  pszAccount);
		
		return NULL;
	}
}

int auth_sign_info_cache_flush(AUTH_SVRENV *pstEnv,
									int iUin)
{
	SignInfoCache stDummy;
	
	stDummy.iWebUin = iUin;

	if (sht_remove(pstEnv->pstSignInfoCacheCache, &stDummy, SignInfoCacheHashCmp, SignInfoCacheHashCode))
	{
		tlog_info(pstEnv->pstLogCat, 0, 0, "sign info cache %d remove",iUin);
	}
	
	return 0;
}

SignInfoCache* auth_sign_info_cache_find(AUTH_SVRENV *pstEnv,
									int iUin)
{
	SignInfoCache *pstSignInfoCache = NULL;
	SignInfoCache stDummy;

	
	//STRNCPY(stDummy.szAccountName, pstAccount->Pass9Account, sizeof(stDummy.szAccountName));
	stDummy.iWebUin = iUin;
	//strlwr(stDummy.szAccountName);

	pstSignInfoCache = sht_find(pstEnv->pstSignInfoCacheCache, &stDummy, SignInfoCacheHashCmp, SignInfoCacheHashCode);
	if (pstSignInfoCache)
	{
		return pstSignInfoCache;
	}
	else
	{
		return NULL;
	}
	
	return 0;
}



WebSignClient *auth_web_sign_client_insert(TAPPCTX* pstAppCtx, AUTH_SVRENV *pstEnv,
									int iConnIdx,tdr_ip_t ip)
{
	WebSignClient *pstWebSignClient = NULL;
	WebSignClient stDummy;

	stDummy.iConnIdx = iConnIdx;

	pstWebSignClient = sht_find(pstEnv->pstWebSignClientCache, &stDummy, WebSignClientHashCmp, WebSignClientHashCode);
	if (pstWebSignClient)
	{
		pstWebSignClient->tTime = pstAppCtx->stCurr.tv_sec;
		pstWebSignClient->IP = ip;
	}
	else
	{
		if (pstEnv->pstWebSignClientCache->iItem >= pstEnv->pstWebSignClientCache->iMax)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "web sign cache iItem>=iMax",
					pstEnv->pstWebSignClientCache->iItem,pstEnv->pstWebSignClientCache->iMax);
			return NULL;
		}

		pstWebSignClient = sht_insert_unique(pstEnv->pstWebSignClientCache, &stDummy, WebSignClientHashCmp, WebSignClientHashCode);
		if (pstWebSignClient)
		{
			memset(pstWebSignClient,0,sizeof(*pstWebSignClient));
			
			pstWebSignClient->iConnIdx = stDummy.iConnIdx;
			pstWebSignClient->tTime = pstAppCtx->stCurr.tv_sec;
			pstWebSignClient->IP = ip;
		}
	}

	if (pstWebSignClient)
	{
		tlog_info(pstEnv->pstLogCat, 0, 0, "web sign connidx %d ip %s ",
			iConnIdx, inet_ntoa(*(struct in_addr *)(char *)&pstWebSignClient->IP));
	}
	else
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "web sign insert fail,connidx %d ip %s",
			iConnIdx, inet_ntoa(*(struct in_addr *)(char *)&ip));
	}
	
	return pstWebSignClient;
}

int auth_web_sign_client_flush(AUTH_SVRENV *pstEnv,
									int iConnIdx)
{
	WebSignClient stDummy;

	stDummy.iConnIdx = iConnIdx;

	if (sht_remove(pstEnv->pstWebSignClientCache, &stDummy, WebSignClientHashCmp, WebSignClientHashCode))
	{
		tlog_info(pstEnv->pstLogCat, 0, 0, "web sign client connidx %d remove", iConnIdx);
	}
	
	return 0;
}

int auth_account_limit_insert(TAPPCTX* pstAppCtx, AUTH_SVRENV *pstEnv,
									const ACCOUNT *pstAccount, tdr_ip_t ip)
{
	AccountLimit *pstAccountLimt;
	AccountLimit stDummy;

	
	STRNCPY(stDummy.szAccountName, pstAccount->Pass9Account, sizeof(stDummy.szAccountName));
	stDummy.IP = ip;
	strlwr(stDummy.szAccountName);

	pstAccountLimt = sht_find(pstEnv->pstAccountLimitCache, &stDummy, AccountLimitHashCmp, AccountLimitHashCode);
	if (pstAccountLimt)
	{
		if (pstAppCtx->stCurr.tv_sec - pstAccountLimt->tTime <= ACCOUNT_LIMIT_TIME)
		{
			pstAccountLimt->iTryNum++;
		}
		else
		{
			STRNCPY(pstAccountLimt->szAccountName, pstAccount->Pass9Account, sizeof(pstAccountLimt->szAccountName));
			pstAccountLimt->iTryNum = 1;
			pstAccountLimt->tTime = pstAppCtx->stCurr.tv_sec;
			pstAccountLimt->IP = ip;
		}
	}
	else
	{
		if (pstEnv->pstAccountLimitCache->iItem >= pstEnv->pstAccountLimitCache->iMax)
		{
			auth_flush_account_limit_cache(pstAppCtx, pstEnv, 10, 1);
		}

		pstAccountLimt = sht_insert_unique(pstEnv->pstAccountLimitCache, &stDummy, AccountLimitHashCmp, AccountLimitHashCode);
		if (pstAccountLimt)
		{
			STRNCPY(pstAccountLimt->szAccountName, pstAccount->Pass9Account, sizeof(pstAccountLimt->szAccountName));
			pstAccountLimt->iTryNum = 1;
			pstAccountLimt->tTime = pstAppCtx->stCurr.tv_sec;
			pstAccountLimt->IP = ip;
		}
	}

	if (pstAccountLimt)
	{
		tlog_info(pstEnv->pstLogCat, 0, 0, "limit account %s ip %s time %d try %d",
			pstAccountLimt->szAccountName, inet_ntoa(*(struct in_addr *)&pstAccountLimt->IP),
			pstAccountLimt->tTime, pstAccountLimt->iTryNum);
	}
	
	return 0;
}

int auth_account_limit_flush(TAPPCTX* pstAppCtx, AUTH_SVRENV *pstEnv,
									const ACCOUNT *pstAccount, tdr_ip_t ip)
{
	AccountLimit stDummy;

	STRNCPY(stDummy.szAccountName, pstAccount->Pass9Account, sizeof(stDummy.szAccountName));
	stDummy.IP = ip;

	if (sht_remove(pstEnv->pstAccountLimitCache, &stDummy, AccountLimitHashCmp, AccountLimitHashCode))
	{
		tlog_info(pstEnv->pstLogCat, 0, 0, "limit account %s remove", pstAccount->Pass9Account);
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}



int auth_passwd_card_limit_insert(AUTH_SVRENV *pstEnv,
									char *pszAccount, tdr_ip_t ip,int iConnIdx)
{
	PasswdCardLimit *pstPasswdCardLimit;
	PasswdCardLimit stDummy;

	if (!pszAccount)
	{
		pszAccount = "";
	}

	
	//STRNCPY(stDummy.szAccountName, pstAccount->Pass9Account, sizeof(stDummy.szAccountName));
	stDummy.iConnIdx = iConnIdx;
	//strlwr(stDummy.szAccountName);

	pstPasswdCardLimit = sht_find(pstEnv->pstPasswdCardLimitCache, &stDummy, PasswdCardLimitHashCmp, PasswdCardLimitHashCode);
	if (pstPasswdCardLimit)
	{
		if (pstEnv->pstAppCtx->stCurr.tv_sec - pstPasswdCardLimit->tTime <= PASSWD_CARD_LIMIT_TIME)
		{
			//pstPasswdCardLimit->iTryNum++;
		}
		else
		{
			memset(pstPasswdCardLimit,0,sizeof(*pstPasswdCardLimit));
			STRNCPY(pstPasswdCardLimit->szAccountName, pszAccount, sizeof(pstPasswdCardLimit->szAccountName));
			pstPasswdCardLimit->iTryNum = 0;
			pstPasswdCardLimit->tTime = pstEnv->pstAppCtx->stCurr.tv_sec;
			pstPasswdCardLimit->IP = ip;
			pstPasswdCardLimit->iConnIdx = iConnIdx;

			tlog_info(pstEnv->pstLogCat, 0, 0, "passwd card limit insert,connidx:%d account:%s ip:%s time:%d try:%d",
					iConnIdx,pstPasswdCardLimit->szAccountName, inet_ntoa(*(struct in_addr *)&pstPasswdCardLimit->IP),
					pstPasswdCardLimit->tTime, pstPasswdCardLimit->iTryNum);
		}
	}
	else
	{
		if (pstEnv->pstPasswdCardLimitCache->iItem >= pstEnv->pstPasswdCardLimitCache->iMax)
		{
			auth_flush_passwd_card_limit_cache(pstEnv, 10, 1);
		}

		pstPasswdCardLimit = sht_insert_unique(pstEnv->pstPasswdCardLimitCache, &stDummy, PasswdCardLimitHashCmp, PasswdCardLimitHashCode);
		if (pstPasswdCardLimit)
		{
			memset(pstPasswdCardLimit,0,sizeof(*pstPasswdCardLimit));
			STRNCPY(pstPasswdCardLimit->szAccountName, pszAccount, sizeof(pstPasswdCardLimit->szAccountName));
			pstPasswdCardLimit->iTryNum = 0;
			pstPasswdCardLimit->tTime = pstEnv->pstAppCtx->stCurr.tv_sec;
			pstPasswdCardLimit->IP = ip;
			pstPasswdCardLimit->iConnIdx = iConnIdx;

			tlog_info(pstEnv->pstLogCat, 0, 0, "passwd card limit insert,connidx:%d account:%s ip:%s time:%d try:%d",
					iConnIdx,pstPasswdCardLimit->szAccountName, inet_ntoa(*(struct in_addr *)&pstPasswdCardLimit->IP),
					pstPasswdCardLimit->tTime, pstPasswdCardLimit->iTryNum);
		}
	}

	if (pstPasswdCardLimit)
	{
		
	}
	else
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "passwd card limit insert fail,item:%d max:%d connidx:%d account:%s",
			pstEnv->pstPasswdCardLimitCache->iItem, pstEnv->pstPasswdCardLimitCache->iMax,
			iConnIdx,  pszAccount);
	}
	
	return 0;
}

int auth_passwd_card_limit_flush(AUTH_SVRENV *pstEnv,
									int iConnIdx)
{
	PasswdCardLimit stDummy;
	
	stDummy.iConnIdx = iConnIdx;

	if (sht_remove(pstEnv->pstPasswdCardLimitCache, &stDummy, PasswdCardLimitHashCmp, PasswdCardLimitHashCode))
	{
		tlog_info(pstEnv->pstLogCat, 0, 0, "passwd card limit %d remove",iConnIdx);
	}
	
	return 0;
}

PasswdCardLimit* auth_passwd_card_limit_find(AUTH_SVRENV *pstEnv,
									int iConnIdx)
{
	PasswdCardLimit *pstPasswdCardLimit = NULL;
	PasswdCardLimit stDummy;

	
	//STRNCPY(stDummy.szAccountName, pstAccount->Pass9Account, sizeof(stDummy.szAccountName));
	stDummy.iConnIdx = iConnIdx;
	//strlwr(stDummy.szAccountName);

	pstPasswdCardLimit = sht_find(pstEnv->pstPasswdCardLimitCache, &stDummy, PasswdCardLimitHashCmp, PasswdCardLimitHashCode);
	if (pstPasswdCardLimit)
	{
		return pstPasswdCardLimit;
	}
	else
	{
		return NULL;
	}
	
	return 0;
}



// 处理许可
static int auth_process_permission(TAPPCTX* pstAppCtx, AUTH_SVRENV *pstEnv, 
                                   SelectInfo* pstSelectInfo,
                                   const ACCOUNT *pstAccount,
                                   const ConnCtx *pstConnCtx)
{
	char szKey[KEY_LEN];
	int iRet = 0;
	AUTHINFO *pstAuthInfo = &pstSelectInfo->stAuthInfo;
	ACCOUNTAUTHINFO stAccountAuth;

	if (0 > pstConnCtx->iConnIdx)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "iConnIdx %d", pstConnCtx->iConnIdx);
	}

	if(pstAccount->FreezeEndTime != 0 )
	{
		if(pstAccount->FreezeEndTime > 0 &&
			 pstAccount->FreezeEndTime <= pstAppCtx->stCurr.tv_sec)
		{
			// 冻结时间已过
			iRet = auth_reset_freeze(pstAppCtx, pstEnv, pstAccount, pstConnCtx);
		}
		else
		{
			// 冻结时间未过
			if (0 == strcmp(pstAccount->FreezeReason, "7"))
			{
				iRet = auth_svr_phony_respond(pstEnv, pstAuthInfo->RandK1, 
			                              AUTH_REASON_FREEZE_CHECKTOOL, pstConnCtx,(char*)pstAccount->Pass9Account);
			}
			else
			{
				iRet = auth_svr_phony_respond(pstEnv, pstAuthInfo->RandK1, 
			                              AUTH_REASON_FREEZE, pstConnCtx,(char*)pstAccount->Pass9Account);
			}
			goto out;
		}
	}

	if(pstSelectInfo->AuthFlag & AUTH_FLAG_REG)
	{
		iRet = auth_svr_phony_respond(pstEnv, pstAuthInfo->RandK1, 
			                            AUTH_IFM_ACCOUNT_EXSIT, pstConnCtx,(char*)pstAccount->Pass9Account);
	}
	else
	{
		// 如果密码验证失败，将发送一个虚假的回应消息。
		auth_gen_key(szKey, pstAuthInfo->AccountLoginPasswd);
		if(memcmp(szKey, pstAccount->PasswdHash, sizeof(szKey)) == 0)
		{
			auth_get_account_auth_info(pstAccount,&stAccountAuth);
			
			iRet = auth_svr_allowed_respond(pstAppCtx, pstEnv, szKey, &stAccountAuth, 
			                              pstConnCtx);
			auth_account_limit_flush(pstAppCtx, pstEnv, pstAccount, pstConnCtx->stExtraInfo.IPInfo.ip);
		}
		else
		{		

			iRet = auth_svr_phony_respond(pstEnv, pstAuthInfo->RandK1, 
			                            AUTH_REASON_PASSWD_ERROR, pstConnCtx,(char*)pstAccount->Pass9Account);
			
			tlog_error(pstEnv->pstLogCat, 0, 0, 
				         "check passwd failed. account:%s, user_pwd:%s", 
				         pstAuthInfo->AccountLoginName,
				         str2hex(szKey,KEY_LEN)	
				         );
			tlog_error(pstEnv->pstLogCat, 0, 0, 
				         "check passwd failed. account:%s,   db_pwd:%s", 
				         pstAuthInfo->AccountLoginName,
				         str2hex(pstAccount->PasswdHash,KEY_LEN)	
				         );

			//插入一个IP限制
			if (pstConnCtx->chExtraType == TFRAMEHEAD_EXTRA_IP)
			{
				auth_account_limit_insert(pstAppCtx, pstEnv, pstAccount, pstConnCtx->stExtraInfo.IPInfo.ip);
			}
		}	
	}

out:
	return iRet;
}

// 清洗缓存
static int auth_flush_sign_info_cache( AUTH_SVRENV* pstEnv, int iCount, int  iForce)
{
	int i;
	static int iCheckPos = 0;
	LPSHITEM pstItem = NULL;
	SignInfoCache *pstSignInfoCache = NULL;

	// 尽量让数据留在Cache中
	/*
	if(pstEnv->pstAccountLimitCache->iItem <= 
		 pstEnv->pstAccountLimitCache->iMax * 1.0* CACHE_FLUSH_PERCENT / 100)
	{
		return 0;
	}*/

	for(i = 0; i < iCount; i++) 
	{
		iCheckPos = (iCheckPos + 1) % pstEnv->pstSignInfoCacheCache->iMax;
				
		pstItem = SHT_GET_ITEM(pstEnv->pstSignInfoCacheCache, iCheckPos);
		if(!pstItem->fValid)
		{
			continue;
		}
		
		pstSignInfoCache = (SignInfoCache*)pstItem->szData;

		if(pstEnv->pstAppCtx->stCurr.tv_sec - pstSignInfoCache->tTime > SIGN_INFO_CACHETIME || iForce)
		{
			sht_remove_by_pos(pstEnv->pstSignInfoCacheCache, iCheckPos);
			if (iForce)
			{
				tlog_info(pstEnv->pstLogCat, 0, 0, "sign_info_cache_force_remove,uin:%d account:%s ip:%s time:%d stat:%d",
					pstSignInfoCache->iWebUin,pstSignInfoCache->szAccountName, inet_ntoa(*(struct in_addr *)&pstSignInfoCache->IP),
					pstSignInfoCache->tTime, pstSignInfoCache->iSignInfoStat);
			}
		}
	}
	
	return 0;
}

// 清洗缓存
static int auth_flush_account_limit_cache(TAPPCTX *pstAppCtx, AUTH_SVRENV* pstEnv, int iCount, int  iForce)
{
	int i;
	static int iCheckPos = 0;
	LPSHITEM pstItem = NULL;
	AccountLimit *pstAccountLimit = NULL;

	// 尽量让数据留在Cache中
	/*
	if(pstEnv->pstAccountLimitCache->iItem <= 
		 pstEnv->pstAccountLimitCache->iMax * 1.0* CACHE_FLUSH_PERCENT / 100)
	{
		return 0;
	}*/

	for(i = 0; i < iCount; i++) 
	{
		iCheckPos = (iCheckPos + 1) % pstEnv->pstAccountLimitCache->iMax;
				
		pstItem = SHT_GET_ITEM(pstEnv->pstAccountLimitCache, iCheckPos);
		if(!pstItem->fValid)
		{
			continue;
		}
		
		pstAccountLimit = (AccountLimit *)pstItem->szData;

		if(pstAppCtx->stCurr.tv_sec - pstAccountLimit->tTime > ACCOUNT_LIMIT_TIME || iForce)
		{
			sht_remove_by_pos(pstEnv->pstAccountLimitCache, iCheckPos);
		}
	}
	
	return 0;
}

// 清洗缓存
static int auth_flush_passwd_card_limit_cache( AUTH_SVRENV* pstEnv, int iCount, int  iForce)
{
	int i;
	static int iCheckPos = 0;
	LPSHITEM pstItem = NULL;
	PasswdCardLimit *pstPasswdCardLimit = NULL;

	// 尽量让数据留在Cache中
	/*
	if(pstEnv->pstAccountLimitCache->iItem <= 
		 pstEnv->pstAccountLimitCache->iMax * 1.0* CACHE_FLUSH_PERCENT / 100)
	{
		return 0;
	}*/

	for(i = 0; i < iCount; i++) 
	{
		iCheckPos = (iCheckPos + 1) % pstEnv->pstPasswdCardLimitCache->iMax;
				
		pstItem = SHT_GET_ITEM(pstEnv->pstPasswdCardLimitCache, iCheckPos);
		if(!pstItem->fValid)
		{
			continue;
		}
		
		pstPasswdCardLimit = (PasswdCardLimit*)pstItem->szData;

		if(pstEnv->pstAppCtx->stCurr.tv_sec - pstPasswdCardLimit->tTime > ACCOUNT_LIMIT_TIME || iForce)
		{
			sht_remove_by_pos(pstEnv->pstPasswdCardLimitCache, iCheckPos);
		}
	}
	
	return 0;
}

int auth_init_shm(AUTH_SVRENV *pstEnv)
{
	int iShmID;
	int iIsExist=0;
			
	iShmID = shmget(pstEnv->pstConf->ResShmKey, sizeof(AuthMemory), 
		              0666 | IPC_CREAT | IPC_EXCL);
	if(iShmID < 0)
	{
		iIsExist = 1;
		iShmID = shmget(pstEnv->pstConf->ResShmKey, sizeof(AuthMemory), 0666);
	}
	
	if(iShmID < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "shmget fail");
		return -1;
	}
	
	if (pstEnv->pstAuthMemory)
	{
		shmdt(pstEnv->pstAuthMemory);
	}

	pstEnv->pstAuthMemory =	(AuthMemory *)shmat(iShmID, NULL, 0);
	if( (void *) -1  == pstEnv->pstAuthMemory )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "shmat fail");
		return -1;
	}

	if (!iIsExist)
	{
		pstEnv->pstAuthMemory->iVersion = OBJ_SHM_VER;
	
		pstEnv->pstAccountLimitCache = sht_init(pstEnv->pstAuthMemory->sAccountSht, 
			                                 sizeof(pstEnv->pstAuthMemory->sAccountSht),
												               ACCOUNT_LIMIT_BUCKET_NUM, ACCOUNT_LIMIT_NUM, 
												               sizeof(AccountLimit));
		if(!pstEnv->pstAccountLimitCache)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  Account cache fail");
			return -1;
		}
	
		pstEnv->pstPasswdCardLimitCache = sht_init(pstEnv->pstAuthMemory->sPasswdCardLimitSht, 
			                                 sizeof(pstEnv->pstAuthMemory->sPasswdCardLimitSht),
												               PASSWD_CARD_LIMIT_BUCKET_NUM, PASSWD_CARD_LIMIT_NUM, 
												               sizeof(PasswdCardLimit));
		if(!pstEnv->pstPasswdCardLimitCache)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  Account cache fail");
			return -1;
		}

		pstEnv->pstSignInfoCacheCache = sht_init(pstEnv->pstAuthMemory->sSignInfoCacheSht, 
			                                 sizeof(pstEnv->pstAuthMemory->sSignInfoCacheSht),
												               SIGN_INFO_CACHEBUCKET_NUM, SIGN_INFO_CACHENUM, 
												               sizeof(SignInfoCache));
		if(!pstEnv->pstSignInfoCacheCache)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  Account cache fail");
			return -1;
		}
	
		pstEnv->pstWebSignClientCache = sht_init(pstEnv->pstAuthMemory->sWebSignClientSht, 
			                                 sizeof(pstEnv->pstAuthMemory->sWebSignClientSht),
												               WEB_SIGN_CLIENT_BUCKET_NUM, WEB_SIGN_CLIENT_NUM, 
												               sizeof(WebSignClient));
		if(!pstEnv->pstWebSignClientCache)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  Account cache fail");
			return -1;
		}
	}
	else
	{
		if(pstEnv->pstAuthMemory->iVersion != OBJ_SHM_VER)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "shm ver=%d",  
				         pstEnv->pstAuthMemory->iVersion);
			return -1;
		}

		pstEnv->pstAccountLimitCache = sht_attach(pstEnv->pstAuthMemory->sAccountSht, 
			                                   sizeof(pstEnv->pstAuthMemory->sAccountSht),
			                                  ACCOUNT_LIMIT_BUCKET_NUM, ACCOUNT_LIMIT_NUM, 
				                           sizeof(AccountLimit));
		if(!pstEnv->pstAccountLimitCache)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach Account cache fail");
			return -1;
		}

		pstEnv->pstPasswdCardLimitCache = sht_attach(pstEnv->pstAuthMemory->sPasswdCardLimitSht, 
			                                   sizeof(pstEnv->pstAuthMemory->sPasswdCardLimitSht),
			                                  PASSWD_CARD_LIMIT_BUCKET_NUM, PASSWD_CARD_LIMIT_NUM, 
				                           sizeof(PasswdCardLimit));
		if(!pstEnv->pstPasswdCardLimitCache)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach Account cache fail");
			return -1;
		}

		pstEnv->pstSignInfoCacheCache = sht_attach(pstEnv->pstAuthMemory->sSignInfoCacheSht, 
			                                   sizeof(pstEnv->pstAuthMemory->sSignInfoCacheSht),
			                                  SIGN_INFO_CACHEBUCKET_NUM, SIGN_INFO_CACHENUM, 
				                           sizeof(SignInfoCache));
		if(!pstEnv->pstSignInfoCacheCache)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach Account cache fail");
			return -1;
		}

		pstEnv->pstWebSignClientCache = sht_attach(pstEnv->pstAuthMemory->sWebSignClientSht, 
			                                   sizeof(pstEnv->pstAuthMemory->sWebSignClientSht),
			                                  WEB_SIGN_CLIENT_BUCKET_NUM, WEB_SIGN_CLIENT_NUM, 
				                           sizeof(WebSignClient));
		if(!pstEnv->pstWebSignClientCache)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach Account cache fail");
			return -1;
		}
	}
	
	pstEnv->iAuthShmID = iShmID;
	return 0;
}


int auth_svr_init(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv)
{
	unsigned char ucEntity; 
	TDRDBMS stDBMS;
	
	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		printf("conf data error\n");
		return -1;
	}
	pstEnv->pstConf = (AUTH_SVRCONF *)pstAppCtx->stConfData.pszBuff;

	pstEnv->pstRunStat = (AUTH_SVRRUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
	if (NULL == pstEnv->pstRunStat)
	{
		pstEnv->pstRunStat = &gs_stStat;
	}

	pstEnv->uiDebugIP = (uint)inet_addr(pstEnv->pstConf->DebugIP);
	

	if (0 > tapp_get_category(NULL, (void *)(&pstEnv->pstLogCat)))	
	{		
		printf("tapp_get_category run fail\n");
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

	if(ss_han_create(g_szMetalib_proto_ss, &pstEnv->pstSSHan) < 0)
	{
		return -1;
	}

	pstEnv->pstRsa = rsa_from_private_key(IDRSAPrivateKey);
	if (NULL == pstEnv->pstRsa)
	{
		return -1;
	}

	pstEnv->pstAuthMeta = tdr_get_meta_by_name((LPTDRMETALIB)g_szMetalib_proto_cs, 
		                                         "AuthInfo");
	if (NULL == pstEnv->pstAuthMeta)
	{
		return -1;
	}

	pstEnv->pstSigMeta = tdr_get_meta_by_name((LPTDRMETALIB)g_szMetalib_proto_cs, 
		                                        "SigInfo");
	if (NULL == pstEnv->pstSigMeta)
	{
		return -1;
	}

	pstEnv->pstAuthRoleInfoMeta = tdr_get_meta_by_name((LPTDRMETALIB)g_szMetalib_proto_cs, 
		                                        "AccGetRoleInfo");
	if (NULL == pstEnv->pstAuthRoleInfoMeta)
	{
		return -1;
	}

	if (0 > taes_setkey(&pstEnv->stAESCtx, sig_key))
	{
		return -1;
	}
	
	// 初始化共享内存
	if(auth_init_shm(pstEnv) < 0)
	{
		return -1;
	}

	//printf("shm init ok, key : %d\n", pstEnv->pstConf->ResShmKey);

	// 初始化相关进程的ID
	GET_FUNC_ENTITY(ucEntity, pstAppCtx->iId);
	switch(ucEntity)
	{
	case FUNC_AUTH_SVR:
	
		GET_REGION_ENTITY_ID(pstEnv->iConnID, pstAppCtx->iId, FUNC_AUTH_CONN);
		break;
	
	default:
		return -1;
		break;
	}
	
	GET_REGION_ENTITY_ID(pstEnv->iInformerID, pstAppCtx->iId, FUNC_INFORMER_SVR);
	GET_REGION_ENTITY_ID(pstEnv->iAccountDBID, pstAppCtx->iId, FUNC_ACCOUNT_DB);
	GET_REGION_ENTITY_ID(pstEnv->iAccountCSID, pstAppCtx->iId, FUNC_ACCOUNT_CS_SVR);
	GET_REGION_ENTITY_ID(pstEnv->iServiceID, pstAppCtx->iId, FUNC_SERVICE_SVR);

	if (0 > torm_create_handle_from_dir(&pstEnv->hTorm, pstEnv->pstConf->DBMetaDir))
	{
		printf("torm_create_handle fail\n");
		return -1;
	}

	if (0 > tdr_load_metalib(&pstEnv->pstResLib, "../cfg/db_meta/role_db_meta.tdr"))
	{
		printf("load meta lib resdb_meta fail\n");
		return -1;
	}

	GET_REGION_ID(pstEnv->iRegionID, pstAppCtx->iId);

	memset(&stDBMS, 0, sizeof(stDBMS));
	STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
	STRNCPY(stDBMS.szDBMSCurDatabaseName, pstEnv->pstConf->IPLimitDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
	STRNCPY(stDBMS.szDBMSConnectionInfo, pstEnv->pstConf->IPLimitDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
	STRNCPY(stDBMS.szDBMSUser, pstEnv->pstConf->IPLimitDBUser, sizeof(stDBMS.szDBMSUser));
	STRNCPY(stDBMS.szDBMSPassword, pstEnv->pstConf->IPLimitDBPasswd, sizeof(stDBMS.szDBMSPassword));	
	stDBMS.iReconnectOpt = 1;	

	if (0 > tdr_open_dbhanlde(&pstEnv->hResDBHandle, &stDBMS, NULL))
	{
		return -1;
	}

	
	if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "IPBlock", 
			pstEnv->stIPLimit.sIPLimit[0], &pstEnv->stIPLimit.iIPLimit, sizeof(IPBLOCK), MAX_IP_LIMIT))
	{
		return -1;
	}

	srand((unsigned)time( NULL ));

	pstEnv->pstAppCtx = pstAppCtx;

	printf("auth_svr start\n");
	
	tlog_info(pstEnv->pstLogCat, 0, 0, "auth_svr start");
	tlog_info_dr(pstEnv->pstLogCat, 0, 0, (LPTDRMETA)pstAppCtx->stConfData.iMeta, 
		           pstAppCtx->stConfData.pszBuff, 
				pstAppCtx->stConfData.iLen, 0);	
	

	return 0;
}




int auth_svr_msg_send(AUTH_SVRENV *pstEnv, CSPKG *pstCSPkgRes, 
	                    const ConnCtx *pstConnCtx)
{
	return auth_svr_msg_send_with_frame_cmd(pstEnv,pstCSPkgRes,pstConnCtx, TFRAMEHEAD_CMD_STOP);
}

int ip_limit_check(TAPPCTX* pstAppCtx,AUTH_SVRENV* pstEnv, int iIP, const char* pszKey, ConnCtx *pstCtx)
{
	int i;
	int iTmpAddr;
	IPBLOCK *pstIPBlock;

	for (i=0; i<pstEnv->stIPLimit.iIPLimit; i++)
	{
		pstIPBlock = (IPBLOCK *)(&pstEnv->stIPLimit.sIPLimit[i]);
		iTmpAddr = iIP & pstIPBlock->Mask;

		if (iTmpAddr == (pstIPBlock->IP & pstIPBlock->Mask))
		{
			if (pstIPBlock->BlockEnd < 0 || pstIPBlock->BlockEnd > pstAppCtx->stCurr.tv_sec)
			{
				auth_svr_phony_respond(pstEnv, pszKey, AUTH_REASON_FREEZE, pstCtx,"");
				return -1;
			}
		}
	}
	return 0 ;
}

static int auth_plugin_sign_respond(AUTH_SVRENV* pstEnv,CSAUTHCLT *pstAuthClt,
								AUTHINFO *pstAuthInfo,ConnCtx *pstConnCtx)
{
	SignInfoCache* pstSignCache = NULL;
	
	pstSignCache = auth_sign_info_cache_find(pstEnv,pstAuthInfo->WebUin);
	if (pstSignCache && pstSignCache->iSignInfoStat == SIGN_INFO_STAT_READY &&
		pstEnv->pstAppCtx->stCurr.tv_sec <= pstSignCache->tTime + SIGN_INFO_CACHETIME  &&
		pstAuthInfo->AuthInfoType == AUTH_INFO_TYPE_GET_SIGN &&
		pstEnv->IP == pstSignCache->IP &&
		(0 == strncmp(pstSignCache->stAuthInfo.RandK1,
					pstAuthInfo->RandK1,sizeof(pstSignCache->stAuthInfo.RandK1))) &&
					
		(0 == strncmp(pstSignCache->stAuthInfo.AccountLoginName,
					pstAuthInfo->AccountLoginName,sizeof(pstAuthInfo->AccountLoginName)))		
		)
	{
		auth_svr_allowed_respond(pstEnv->pstAppCtx,pstEnv,
								pstSignCache->szMd5,&pstSignCache->stAccountAuthInfo,
								pstConnCtx);

		pstSignCache->iSignInfoStat = SIGN_INFO_STAT_GETED;

		tlog_debug(pstEnv->pstLogCat, 0, 0, "auth_plugin_sign_respond: webuin=%d acc=%s ip=%s",
			pstAuthInfo->WebUin,pstAuthInfo->AccountLoginName,
			inet_ntoa(*(struct in_addr *)&pstSignCache->IP));
	}
	else
	{
		return auth_svr_phony_respond(pstEnv, pstAuthInfo->RandK1, 
		                            AUTH_IFM_WEB_SIGN_AUTH_ERROR, pstConnCtx,
		                            pstAuthInfo->AccountLoginName);
	}
	return 0;
	UNUSED(pstAuthClt);	//add by paraunused.pl
}

static int auth_web_sign_ip_check(AUTH_SVRENV* pstEnv,ConnCtx *pstConnCtx)
{
	//检查ip是否有效
	int i = 0; 
	int iValid = 0;
	
	for (i = 0; i < pstEnv->pstConf->WebSignClientIPNum;i++)
	{
		if (pstEnv->IP == inet_addr(pstEnv->pstConf->WebSignClientIP[i]))
		{
			iValid = 1;
			break;
		}
	}
	
	if (!iValid)			
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "invalid web sign client iConnID:%u ip:%s port:%d connidx:%d",  
					pstEnv->iConnID,inet_ntoa(*(struct in_addr *)&pstEnv->IP),
					pstConnCtx->stExtraInfo.IPInfo.port,
					pstConnCtx->iConnIdx);
		return -1;
	}
	else
	{
		return 0;
	}	

	return 0;
}

int auth_svr_msg(TAPPCTX* pstAppCtx, AUTH_SVRENV* pstEnv, char *pszMsg, 
	               int iMsgLen, TFRAMEHEAD *pstFrameHead)
{
	CSPKG stCsPkgClt;
	CSAUTHCLT *pstAuthClt = &stCsPkgClt.Body.AuthClt;
	int iRet = 0, 
		iErr = 0;
	char szDec[RSA_ENC_LEN];
	TDRDATA stHost, stNet;
	AUTHINFO stAuthInfo;
	ConnCtx stConnCtx;
	short shVersion = 0;
	AccountLimit stDummy;
	AccountLimit *pstAccountLimit = NULL;
	

	//if (TFRAMEHEAD_CMD_START == pstFrameHead->Cmd)
	//{
		auth_connctx_assign(&stConnCtx, pstFrameHead, 0);
	//}

	if (TFRAMEHEAD_CMD_STOP == pstFrameHead->Cmd)
	{		
		auth_passwd_card_limit_flush(pstEnv,stConnCtx.iConnIdx);
		auth_web_sign_client_flush(pstEnv,stConnCtx.iConnIdx);
		return 0;
	}

	if ((TFRAMEHEAD_CMD_START != pstFrameHead->Cmd && TFRAMEHEAD_CMD_INPROC != pstFrameHead->Cmd )|| stConnCtx.iConnIdx < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "pstFrameHead->Cmd=%d, iConnIdx=%d", 
					pstFrameHead->Cmd, stConnCtx.iConnIdx);
		pstEnv->pstRunStat->BadCltMsg++;
		goto error;
	}

	pstEnv->IP = 0;

	if (stConnCtx.chExtraType == TFRAMEHEAD_EXTRA_IP)
	{
		pstEnv->IP = stConnCtx.stExtraInfo.IPInfo.ip;
	}
	else
	{
		//长连接的ip
		WebSignClient * pstWebSignClient = auth_web_sign_client_find(pstEnv,stConnCtx.iConnIdx);
		PasswdCardLimit* pstPasswdCardLimit ;
		
		if (pstWebSignClient)
		{
			pstEnv->IP = pstWebSignClient->IP;
		}
		else
		{
			pstPasswdCardLimit = auth_passwd_card_limit_find(pstEnv,stConnCtx.iConnIdx);
			if (pstPasswdCardLimit)
			{
				pstEnv->IP = pstPasswdCardLimit->IP;
			}
		}
	}

	stConnCtx.IP = pstEnv->IP;

	memcpy(&shVersion,pszMsg+8,2);
	shVersion = NTOHS(shVersion);
	iErr = net_cs_unpack(pstEnv->pstCSHan, pszMsg, iMsgLen, &stCsPkgClt, shVersion);
	
	//iErr = net_cs_unpack(pstEnv->pstCSHan, pszMsg, iMsgLen, &stCsPkgClt, 0);
	if(iErr < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "net_cs_unpack fail,iMsgLen:%d reason : %s version:%d", 
			         iMsgLen,tdr_error_string(iErr),(int)shVersion);
		pstEnv->pstRunStat->BadCltMsg++;
		goto error;
	}

	stConnCtx.iCltProtoVer = stCsPkgClt.Head.Ver;

	// 查accountrole的角色
	if (stCsPkgClt.Head.Cmd == ACC_GET_ROLE_CLT)
	{
		if (db_fetch_account_role(pstAppCtx,pstEnv,&stConnCtx, &stCsPkgClt.Body.AccGetRoleClt) < 0)
		{
			goto error;
		}
		return 0;
	}

	if (stCsPkgClt.Head.Cmd == HEARTBEAT_REQ)
	{
		PasswdCardLimit* pstPasswdCardLimit = auth_passwd_card_limit_find(pstEnv,stConnCtx.iConnIdx);
		if (pstPasswdCardLimit)
		{
			//do nothing		
			tlog_debug(pstEnv->pstLogCat, 0, 0, "heart beat from passwdcard client ip=%d,connidx=%d", pstFrameHead->ExtraInfo.IPInfo.ip,stConnCtx.iConnIdx);
		}
		else
		{
			WebSignClient *pstWebSignClient = NULL;

			if (TFRAMEHEAD_CMD_START == pstFrameHead->Cmd)
			{		
				//检查ip是否有效
				int iRet = 0; 

				iRet= auth_web_sign_ip_check(pstEnv,&stConnCtx);
				
				if (iRet)
				{				
					//断开连接
					tlog_error(pstEnv->pstLogCat, 0, 0, "auth_web_sign_ip_check fail:  ip:%s port:%d connidx:%d",  
								inet_ntoa(*(struct in_addr *)&pstEnv->IP),
								stConnCtx.stExtraInfo.IPInfo.port,
								stConnCtx.iConnIdx);
					
					auth_disconnect(pstEnv,&stConnCtx);
					return 0;
				}
				else
				{
					pstWebSignClient = auth_web_sign_client_insert(pstAppCtx,pstEnv,stConnCtx.iConnIdx,stConnCtx.stExtraInfo.IPInfo.ip);
					
				}
			}
			else
			{
				pstWebSignClient = auth_web_sign_client_find(pstEnv,stConnCtx.iConnIdx);
			}

			if (!pstWebSignClient)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "auth_web_sign_client_find fail:  ip:%s port:%d connidx:%d",  
								inet_ntoa(*(struct in_addr *)&pstEnv->IP),
								stConnCtx.stExtraInfo.IPInfo.port,
								stConnCtx.iConnIdx);
				//断开连接
				auth_disconnect(pstEnv,&stConnCtx);
			}
			else
			{
				stConnCtx.iID = stConnCtx.iConnIdx;
				pstWebSignClient->tTime = pstAppCtx->stCurr.tv_sec;
				
				tlog_debug(pstEnv->pstLogCat, 0, 0, "heart beat from sign_service client ip=%d,connidx=%d", pstWebSignClient->IP,stConnCtx.iConnIdx);
			}
		}
		
		return 0;
	}

	if (ip_limit_check(pstAppCtx, pstEnv, pstFrameHead->ExtraInfo.IPInfo.ip,stCsPkgClt.Body.AuthClt.RandK1, &stConnCtx))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "IP limit  IP=%d", pstFrameHead->ExtraInfo.IPInfo.ip);
		goto error;
	}

	pstEnv->pstRunStat->RecvCltMsg++;

	if (stCsPkgClt.Head.Cmd != AUTH_CLT)
	{
		goto error;
	}

	if (0 > rsa_private_decrypt(pstEnv->pstRsa, sizeof(pstAuthClt->Enc), 
		                          pstAuthClt->Enc, szDec))
	{
		char szRsaEncHex[576];		
		int iHexLen = 0;
	
		tlog_debug(pstEnv->pstLogCat, 0, 0, "rsa_private_decrypt fail ip:%s port:%d connidx:%d", 
							inet_ntoa(*(struct in_addr *)&pstEnv->IP),
							stConnCtx.stExtraInfo.IPInfo.port,
							stConnCtx.iConnIdx);
		
		iHexLen = sizeof(szRsaEncHex);
		
		Bin2Hex((unsigned char *)pstAuthClt->Enc,sizeof(pstAuthClt->Enc), szRsaEncHex,&iHexLen);
		

		tlog_debug(pstEnv->pstLogCat, 0, 0, "ras enc:iHexLen=%d szRsaEnc=%s",
					iHexLen,szRsaEncHex);
		
		goto error;
		UNUSED(HEX_value);
	}

	stHost.iBuff = sizeof(stAuthInfo);
	stHost.pszBuff = (char *)&stAuthInfo;
	stNet.iBuff = sizeof(szDec);
	stNet.pszBuff = szDec;
	if (0 > tdr_ntoh(pstEnv->pstAuthMeta, &stHost, &stNet, stConnCtx.iCltProtoVer))
	{
		goto error;
	}
	
	strlwr(stAuthInfo.AccountLoginName);
	
	
	tlog_info(pstEnv->pstLogCat, 0, 0, "iConnID:%u ip:%s port:%d connidx:%d login:%s flag:%d",  
		pstEnv->iConnID,inet_ntoa(*(struct in_addr *)&pstEnv->IP),
		stConnCtx.stExtraInfo.IPInfo.port,
		stConnCtx.iConnIdx,stAuthInfo.AccountLoginName,
		pstAuthClt->AuthFlag);

	if (pstEnv->pstConf->NoxFlag && !(pstAuthClt->AuthFlag & AUTH_FLAG_NOX))
	{
		//todo:nox
		//NoxFlag指定了nox验证模式 
		tlog_error(pstEnv->pstLogCat, 0, 0, "nox auth only, ip:%s port:%d connidx:%d",  
							inet_ntoa(*(struct in_addr *)&pstEnv->IP),
							stConnCtx.stExtraInfo.IPInfo.port,
							stConnCtx.iConnIdx);

		pstAuthClt->AuthFlag |= AUTH_FLAG_NOX;
	}

	if (pstAuthClt->AuthFlag & AUTH_FLAG_PLUGIN_SIGN)
	{
		auth_plugin_sign_respond(pstEnv,pstAuthClt,&stAuthInfo,&stConnCtx);
		return 0;
	}

	

	if (pstAuthClt->AuthFlag & AUTH_FLAG_WEB_SIGN)
	{
		WebSignClient * pstWebSignClient = NULL;
		SignInfoCache * pstSignCache = NULL;
		
		if (TFRAMEHEAD_CMD_START == pstFrameHead->Cmd)
		{		
			//检查ip是否有效
			int iRet = 0; 

			iRet= auth_web_sign_ip_check(pstEnv,&stConnCtx);
			
			if (iRet)
			{				
				//断开连接
				auth_disconnect(pstEnv,&stConnCtx);
				return 0;
			}
			else
			{
				pstWebSignClient = auth_web_sign_client_insert(pstAppCtx,pstEnv,stConnCtx.iConnIdx,pstEnv->IP);
				stConnCtx.iID = stConnCtx.iConnIdx;
			}
		}
		else
		{
			pstWebSignClient = auth_web_sign_client_find(pstEnv,stConnCtx.iConnIdx);
		}

		if (!pstWebSignClient)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "auth_web_sign_client_find fail: ip:%s port:%d connidx:%d",  
							inet_ntoa(*(struct in_addr *)&pstEnv->IP),
							stConnCtx.stExtraInfo.IPInfo.port,
							stConnCtx.iConnIdx);
			//断开连接
			auth_disconnect(pstEnv,&stConnCtx);
			return 0;
		}

		stConnCtx.iID = stConnCtx.iConnIdx;

		pstSignCache = auth_sign_info_cache_insert(pstEnv, stAuthInfo.AccountLoginName, 
							pstAuthClt->IP, stAuthInfo.WebUin);

		if (!pstSignCache)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "auth_sign_info_cache_insert fail,uin:%d SignIP:%s SignPort:%d connidx:%d CltIP:%d",  
							stAuthInfo.WebUin,
							inet_ntoa(*(struct in_addr *)&pstEnv->IP),
							stConnCtx.stExtraInfo.IPInfo.port,
							stConnCtx.iConnIdx,
							pstAuthClt->IP);
			//返回错误
			auth_svr_phony_respond(pstEnv,stAuthInfo.RandK1, 
								AUTH_IFM_WEB_SIGN_ERROR, &stConnCtx,
								stAuthInfo.AccountLoginName);
			return 0;
		}
		else
		{
			if (pstSignCache->iSignInfoStat == SIGN_INFO_STAT_READY)
			{
				//直接返回签名
				auth_web_sign_respond(pstEnv,stAuthInfo.RandK1,&pstSignCache->stAccountAuthInfo, 
										&stConnCtx,AUTH_IFM_WEB_SIGN_SUCC,
										pstSignCache->szEnc,
										pstSignCache->iEncLen);

				pstSignCache->IP = pstAuthClt->IP;
				
				return 0;
			}
			else
			{
				//过一遍数据库
			}
		}
	}
	
	STRNCPY(stDummy.szAccountName, stAuthInfo.AccountLoginName, sizeof(stDummy.szAccountName));

	if (pstFrameHead->ExtraType == TFRAMEHEAD_EXTRA_IP)	
	{
		stDummy.IP = pstFrameHead->ExtraInfo.IPInfo.ip;
		pstAccountLimit = sht_find(pstEnv->pstAccountLimitCache, &stDummy, AccountLimitHashCmp, AccountLimitHashCode);
	}
	
	if (pstAccountLimit && pstAccountLimit->iTryNum >= MAX_TRY_NUM && pstAppCtx->stCurr.tv_sec - pstAccountLimit->tTime <= ACCOUNT_LIMIT_TIME)
	{
		iRet = auth_svr_phony_respond(pstEnv, stAuthInfo.RandK1, 
			                            AUTH_REASON_FREEZE_TRYMAX, &stConnCtx,stAuthInfo.AccountLoginName);
	}
	else
	{
		iRet = db_fetch_account(pstAppCtx, pstEnv, &stAuthInfo, &stConnCtx,
		                                              pstAuthClt->ActiveFlag,pstAuthClt->ActiveCode,
		                                              pstAuthClt->AuthFlag,0,
		                                              pstAuthClt->CltType,
		                                              &pstAuthClt->PasswdCardAuthInfo);
	}

final:
	return iRet;

error:
	iRet = -1;
	auth_disconnect(pstEnv, &stConnCtx);
	goto final;
	return 0;
}

int auth_svr_proc(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv)
{
	int iBusy = 0;
	int iLoopNum = 0;
	TBUSADDR iMsgDst ;
	TBUSADDR iMsgSrc ;
	int iMsgLen;
	int iRet = 0;
	char *pszMsg;
	TFRAMEHEAD stFrameHead;
	int iFrameHeadLen ;

	//先收client的请求
	while (LOOP_NUM > iLoopNum)
	{
		iLoopNum++;
		
		iMsgDst = pstAppCtx->iId;
		iMsgSrc = TBUS_ADDR_ALL;	// 接收发到tbus的所有来源的数据
		iMsgLen = sizeof(pstEnv->szRecvBuffer);
		pszMsg = &pstEnv->szRecvBuffer[0];
		if(TBUS_SUCCESS != tbus_recv(pstAppCtx->iBus, &iMsgSrc, &iMsgDst, pszMsg, 
			(size_t *)&iMsgLen, 0))
		{
			//no msg
			break;
		}
		//printf("tbus_recv : src = %d, dst = %d\n", iMsgSrc, iMsgDst);
		iBusy = 1;

		if(iMsgSrc == pstEnv->iConnID)
		{
			iFrameHeadLen = sizeof(stFrameHead);
			iRet = tconnapi_decode(pszMsg, iMsgLen, &stFrameHead, &iFrameHeadLen);
			if (0 != iRet)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, 
					         "tconnapi_decode failed, iRet0x%x, Msglen:%d", iRet, iMsgLen);
				continue;
			}

			pszMsg += iFrameHeadLen;
			iMsgLen -= iFrameHeadLen;
			iRet = auth_svr_msg(pstAppCtx, pstEnv, pszMsg, iMsgLen, &stFrameHead);
		}
		else if(iMsgSrc == pstEnv->iAccountDBID)
		{
			iRet = auth_process_account_db_msg(pstAppCtx, pstEnv, pszMsg, iMsgLen);
		}
		else if(iMsgSrc == pstEnv->iInformerID || 
			      iMsgSrc == pstEnv->iAccountCSID ||
			      iMsgSrc == pstEnv->iServiceID)
		{
			iRet = auth_process_ss_msg(pstAppCtx, pstEnv, pszMsg, iMsgLen);
		}
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

int auth_svr_reload(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv)
{
	AUTH_SVRCONF *pstPreConf;
	AUTH_SVRCONF *pstCurrentConf = pstEnv->pstConf;

	UNUSED(pstEnv);
	
	pstPreConf = (AUTH_SVRCONF   *)pstAppCtx->stConfPrepareData.pszBuff;

	pstCurrentConf->AuthMode = pstPreConf->AuthMode;
	
	printf("auth_svr reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "auth_svr reload");

	return 0;
}

int auth_svr_fini(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv)
{
	UNUSED(pstAppCtx);
	
	if (pstEnv->pstRsa)
	{
		rsa_free(pstEnv->pstRsa);
	}

	if(pstEnv->pstSSHan)
	{
		ss_han_destroy(&pstEnv->pstSSHan);
	}

	//if (pstEnv->cClearShm)
	{
		tshmdt(pstEnv->pstAuthMemory);
		tshmclose(pstEnv->iAuthShmID, TSHMF_DELETE);
	}

	printf("auth_svr finish\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "auth_svr finish");
	return 0;
}

int auth_svr_tick(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv)
{
	UNUSED(pstAppCtx);
	
	auth_flush_account_limit_cache(pstAppCtx, pstEnv, 1000, 0);
	auth_flush_sign_info_cache(pstEnv, 100,0);
	return 0;
}

int auth_svr_stop(TAPPCTX *pstAppCtx, AUTH_SVRENV *pstEnv)
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

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)auth_svr_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_auth_svr;
	gs_stAppCtx.stConfData.pszMetaName = "auth_svrconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);
		
	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)auth_svr_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)auth_svr_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)auth_svr_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)auth_svr_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)auth_svr_stop;
	
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

/*
int auth_svr_msg(TAPPCTX* pstAppCtx, AUTH_SVRENV* pstEnv, char *pszMsg, int iMsgLen, TFRAMEHEAD *pstFrameHead)
{
	CSPKG stCsPkgClt;
	CSAUTHCLT *pstAuthClt = &stCsPkgClt.Body.AuthClt;
	CSPKG stCsPkgSvr;
	CSAUTHSVR *pstAuthSvr = &stCsPkgSvr.Body.AuthSvr;
	int iRet;
	int iCltProtoVer;
	char Dec[RSA_ENC_LEN];  
	char Enc[AUTH_LEN];
	char Key[KEY_LEN];
	AUTHINFO stAuthInfo;
	SIGINFO stSigInfo;
	TDRDATA stHost, stNet;
	char szMd5[16];
	AES stAes;
	int iLen;
	
	if (TFRAMEHEAD_CMD_START != pstFrameHead->Cmd)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "pstFrameHead->Cmd=%d", pstFrameHead->Cmd);
		pstEnv->pstRunStat->BadCltMsg++;
		return -1;
	}

	
	if (0 > net_cs_unpack(pstEnv->pstCSHan, pszMsg, iMsgLen, &stCsPkgClt, 0))
	{
		pstEnv->pstRunStat->BadCltMsg++;
		tlog_error(pstEnv->pstLogCat, 0, 0, "net_cs_unpack fail");
		return -1;
	}

	iCltProtoVer = stCsPkgClt.Head.Ver;

	pstEnv->pstRunStat->RecvCltMsg++;

	if (stCsPkgClt.Head.Cmd != AUTH_CLT)
	{
		return -1;
	}

	if (0 > rsa_private_decrypt(pstEnv->pstRsa, sizeof(pstAuthClt->Enc), pstAuthClt->Enc, Dec))
	{
		return -1;
	}

	stHost.iBuff = sizeof(stAuthInfo);
	stHost.pszBuff = (char *)&stAuthInfo;
	stNet.iBuff = sizeof(Dec);
	stNet.pszBuff = Dec;
	if (0 > tdr_ntoh(pstEnv->pstAuthMeta, &stHost, &stNet, iCltProtoVer))
	{
		return -1;
	}

	stCsPkgSvr.Head.Magic = PROTOCOL_MAGIC;
	stCsPkgSvr.Head.Cmd = AUTH_SVR;
	stCsPkgSvr.Head.Ver = TDR_METALIB_PROTO_CS_VERSION;

	//扰乱
	Enc[0] = (char)random();

	if (0 == strcmp(stAuthInfo.AccountLoginPasswd, "hello"))
	{
		Enc[1] = 0;
		
		//gamekey
		rand_str(Enc+2, KEY_LEN);
				
		stSigInfo.TimeStamp = pstAppCtx->stCurr.tv_sec;
		stSigInfo.Uin = 11111;
		stSigInfo.UinFlag = 0;
		STRNCPY(stSigInfo.BirthDay, "19761117", sizeof(stSigInfo.BirthDay));
		memcpy(stSigInfo.GameKey, Enc+2, KEY_LEN);

		iRet = (int)HTONL((long)stSigInfo.Uin);
		memcpy(Enc+2+KEY_LEN, &iRet, sizeof(int));

		stNet.iBuff = sizeof(pstEnv->szSendBuffer);
		stNet.pszBuff = pstEnv->szSendBuffer;
		stHost.iBuff = sizeof(stSigInfo);
		stHost.pszBuff = (char *)&stSigInfo;
		if (0 > tdr_hton(pstEnv->pstSigMeta, &stNet, &stHost, iCltProtoVer))
		{
			return -1;
		}

		iLen = sizeof(pstEnv->szRecvBuffer);
		if (0 > taes_encrypt(stNet.pszBuff, stNet.iBuff, &pstEnv->stAESCtx, 
					pstEnv->szRecvBuffer, &iLen))
		{
			return -1;
		}

		if (iLen > (int)sizeof(Enc) - 2 - KEY_LEN - (int)sizeof(int) -32)
		{
			return -1;
		}

		memcpy(Enc+2+KEY_LEN+sizeof(int), pstEnv->szRecvBuffer, iLen);
		
		Md5HashBuffer(szMd5, stAuthInfo.AccountLoginPasswd, strlen(stAuthInfo.AccountLoginPasswd));
		Md5HashBuffer(Key, szMd5, 16);
	}
	else
	{
		Enc[1] = 1;

		iLen = sizeof(Enc) - (2+KEY_LEN+sizeof(int)) - 32;

		//扰乱
		rand_str(Enc+2, iLen);

		memcpy(Key, stAuthInfo.RandK1, sizeof(Key));
	}

	if (0 > taes_setkey(&stAes, (unsigned char *)Key))
	{
		return -1;
	}

	pstAuthSvr->AuthLen = sizeof(pstAuthSvr->Auth);
	if (0 > taes_encrypt(Enc, KEY_LEN+2+iLen+sizeof(int), &stAes, pstAuthSvr->Auth, &pstAuthSvr->AuthLen))
	{
		return -1;
	}

	iRet = auth_svr_msg_send(pstEnv, &stCsPkgSvr, pstFrameHead, iCltProtoVer);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "auth_svr_msg_send fail %d", iRet);
		pstEnv->pstRunStat->SendCltMsgFail++;
	}

	return iRet;
}
*/

