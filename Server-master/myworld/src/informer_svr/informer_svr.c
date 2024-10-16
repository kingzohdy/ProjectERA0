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
#include "informer_svr_conf_desc.h"
#include "version.h"
#include "tbus/tbus.h"
#include "comm_misc.h"
#include "cs_net.h"
#include "entity.h"
#include "md5.h"
#include "../auth_svr/auth_svr.h"
#include "tormapi/torm_protocol_api.h"
#include "ifm/informer_lib.h"
#include "comm/shtable.h"
#include "file_lock.h"

#define MAX_THREAD 256
#define LOCAL_BUS_ADDR_START 10
#define CHANNEL_SIZE 1024*256
#define MAX_MSG_SIZE 64000
#define CHANNEL_APPEND 128
#define UNUSED(arg) ((void)arg)
#define CCH(arr)    (sizeof(arr)/sizeof(arr[0]))
#define IFM_MESSAGE_ID_LEN  4
#define IFM_PARAS_NUM_MAX    11
#define IFM_PARAS_BUFFER_LEN    10240
#define CS_MAX_MSG CS_MAX_BODY+sizeof(CSPKGHEAD)+sizeof(TFRAMEHEAD)

//hash 的节点数 1:3的索引查找比
#define IFM_MAX_NUM  50000
//每秒3千的请求量 cache 10秒的数据请求量 
#define IFM_BUCKET_NUM (IFM_MAX_NUM * 3)
// 超过10秒的数据就从cache中删除,能cache 3分钟
#define IFM_MAX_CACHE_FLUSH_TIME  3*60
// 每次扫描的节点数
#define PRE_SCAN_BUCKET_NUM  3000

#define OBJ_MEM_VER 0x0002

#define SCAN_TIME_INTERVAL   1
enum {
	DB_RES_UPDATE_PASSWD,
	DB_RES_UPDATE_ACCOUNT,
	DB_RES_FREEZE_ACCOUNT,
	DB_RES_FREE_ACCOUNT
};
// 哈希表相关数据
typedef struct tagIfmAuthBucket
{
	SSGETACCOUNTRES stRes;
	//unsigned long long  ullIfmSeq; 
	char AccountLoginName[ACCOUNT_LOGIN_NAME_LEN];
	int tTime;//淘汰时间
}IfmAuthBucket;
typedef struct tagIfmAuthMemory
{
	int iVersion;
	char sIfmMem[SHT_SIZE(IFM_BUCKET_NUM, IFM_MAX_NUM, sizeof(IfmAuthBucket))];
}IfmAuthMemory;

typedef struct tagChannelAppend
{
	char sAppend[CHANNEL_APPEND];
}ChannelAppend;

typedef struct tagThreadMgr
{
	pid_t tid;
	int iIdx;
	LPTBUSCHANNEL	pstChannel0;
	LPTBUSCHANNEL	pstChannel1;
	char cIsExit;
	char cIsValid;
	void * pstEnv;
	ChannelAppend stChannelAppend;//记录了以下回包方向的数据
}ThreadMgr;

struct taginformer_svrEnv
{
	INFORMER_SVRCONF *pstConf;
	INFORMER_SVRRUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	TAPPCTX *pstAppCtx;
	pthread_mutex_t stLogMutex;
	int iThread;
	ThreadMgr astThread[MAX_THREAD];

	int iAuthID;      // 主 auth_svr的ID 
	//int iAuthSlaveID; // 辅
	//int iAuthBakID;   // 备用
	int iRegionGateID;
	LPSSHANDLE pstSSHan;
	char szRecvBuffer[CS_MAX_MSG];
	char szSendBuffer[CS_MAX_MSG];
	int iLastSrcID; // 上一次接收数据源

	int iAccountDBProtoVer;//访问db的服务器的协议版本
	int iAccountDBID;//accountdb的busid
	TORMPROTOCOLHANDLE hTorm;//Torm操作的句柄
	int   iTormSeq; //控制下发包的次序
	unsigned long long    ullIfmSeq;//控制下到ifm的包的请求次序
	SSPKG stSSPkg;//用于发送消息到auth

	//用来记录auth来的请求
	LPSHTABLE pstIfmAuthCache;
	IfmAuthMemory *pstIfmAuthMemory; //开辟动态内存来存下临时数据	

	int* piProExitFlag; // 进程退出的标志位
	
};

typedef struct taginformer_svrEnv		INFORMER_SVRENV;
typedef struct taginformer_svrEnv		*LINFORMER_SVRENV;

static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_informer_svr[];
extern unsigned char g_szMetalib_proto_ss[];

INFORMER_SVRENV gs_stEnv;
static INFORMER_SVRRUN_STATUS gs_stStat;

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
			filelock_lock(FILE_LOCK_WRITE,0,1, 1);\
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
			filelock_unlock(0,1); \
		  }while(0)
// 根据路由信息回送数据
static int ss_backward_pkg(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,
                           SSPKG *pstPkg, int iCmd);

// 回复生产模式的帐号信息
static int ss_produce_account_respond(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,
	                                    const SSGETACCOUNTREQ * pstReq);

// 回复开发模式的帐号信息
static int ss_develop_account_respond(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,
	                                    const SSGETACCOUNTREQ * pstReq);

static int ss_get_account_req(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,
	                            const SSGETACCOUNTREQ * pstGetAccountReq);

// 处理 auth_svr 进程发送过来的信息。
static int ifm_process_auth_msg(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,
	                              const char *pszMsg, int iMsgLen);

// 处理 9c 发送过来的信息。
static int ifm_process_9c_msg(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread);

// 窥视ExtraData中的密码信息，仅仅在开发模式中使用
static const char * ifm_peek_passwd(const void *pszExtraData, int iExtraDataLen);

//9c发送过来的数据 要插入到db
static int ifm_db_insert_produce_account(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,
	                                   const INFORMERACCOUNT *pstIfmAccount,unsigned long long  ullSeq);

static int ifm_response2pass9(INFORMER_SVRENV * pstEnv,int imsgid,unsigned long long ullSeq,
					int iRet ,const char* service_name);

int store_appendinfo(TAPPCTX *pstAppCtx, ChannelAppend *pstChannelAppend);

static int ifm_process_9c_active(INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,char *paras[],uint32_t para_count);

/* 建立一小块共享内存 */
int ifm_init_shm(INFORMER_SVRENV *pstEnv, int iKey)
{
	int iShmID = 0;
	int iSize = 0;

	iSize = (pstEnv->pstConf->MutiThreadNum+1) *sizeof(int);
	/* 每个int 表示进程是否退出的标志位 0,表示不退出 1表示退出 */
	iShmID = shmget(iKey, iSize, 0666|IPC_CREAT|IPC_EXCL);
	if(iShmID < 0)
	{
		iShmID = shmget(iKey, iSize, 0666);
	}
	if(iShmID < 0)
	{
		printf("shmget fail\n");
		return -1;
	}

	pstEnv->piProExitFlag = (int *)shmat(iShmID, NULL, 0);
	if( (void *) -1  == pstEnv->piProExitFlag )
	{
		printf("shmat fail\n");
	}

	/* 把值初始化成0 */
	memset(pstEnv->piProExitFlag,0,iSize);

	return 0;
}
int ifm_set_process_exit_flag(INFORMER_SVRENV *pstEnv,unsigned  int iIndex)
{
	if( iIndex   < (unsigned int )(pstEnv->pstConf->MutiThreadNum+1) )
	{
		pstEnv->piProExitFlag[iIndex] = 1;
		
		return 0;
	}

	return -1;
}

int ifm_get_process_exit_flag(INFORMER_SVRENV *pstEnv,unsigned  int iIndex)
{
	if( iIndex   < (unsigned int )(pstEnv->pstConf->MutiThreadNum+1) )
	{
		return pstEnv->piProExitFlag[iIndex];
	}

	return -1;
}
// 哈希表相关函数 以序列号做hash的索引
static int ifm_mem_hash_cmp(const void * pv1 , const void *pv2)
{
	const IfmAuthBucket * pszBucket1 = (IfmAuthBucket * )pv1;
	const IfmAuthBucket * pszBucket2 = (IfmAuthBucket * )pv2;

	//return pszBucket1->ullIfmSeq==pszBucket2->ullIfmSeq ;
	return strcmp(pszBucket1->AccountLoginName, pszBucket2->AccountLoginName);
}
static unsigned int ifm_mem_hash_code(const void* pv)
{
	const IfmAuthBucket * pszBucket = (IfmAuthBucket * )pv;
	
	//return (unsigned int)pszBucket->ullIfmSeq;
	return sht_get_code(pszBucket->AccountLoginName);
}
// 将account信息放入缓存中。
static int ifm_mem_insert_cache(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv,
                                      const SSGETACCOUNTRES *pstRes, char* AccountLoginName )
{
	IfmAuthBucket *pstPos, stDummy;
	UNUSED(pstAppCtx);

	STRNCPY(stDummy.AccountLoginName , AccountLoginName,sizeof(stDummy.AccountLoginName ));
		
	pstPos = (IfmAuthBucket *)sht_insert_unique(pstEnv->pstIfmAuthCache, &stDummy, ifm_mem_hash_cmp,
		                        ifm_mem_hash_code);
	if(pstPos)
	{
		pstPos->tTime = time(NULL);
		pstPos->stRes = *pstRes;
		STRNCPY(pstPos->AccountLoginName , AccountLoginName,sizeof(pstPos->AccountLoginName ));
		return 0;
	}
	
	return -1;
}
// 从cache中删除一个account数据
static void ifm_mem_remove_cache(INFORMER_SVRENV* pstEnv, 
                                     char*   AccountLoginName)
{
	IfmAuthBucket stDummy;

	STRNCPY(stDummy.AccountLoginName , AccountLoginName,sizeof(stDummy.AccountLoginName ));
	sht_remove(pstEnv->pstIfmAuthCache, &stDummy, ifm_mem_hash_cmp, 
		         ifm_mem_hash_code);
}
// 获取高速缓存中的account信息。
static const SSGETACCOUNTRES * ifm_mem_cached_account(INFORMER_SVRENV *pstEnv,
                                         char * AccountLoginName)
{
	IfmAuthBucket *pstPos, stDummy;

	STRNCPY(stDummy.AccountLoginName , AccountLoginName,sizeof(stDummy.AccountLoginName ));

	pstPos = (IfmAuthBucket *)sht_find(pstEnv->pstIfmAuthCache, &stDummy, ifm_mem_hash_cmp, 
		                ifm_mem_hash_code);
	
	return pstPos ? &pstPos->stRes: NULL;
}
// 清洗缓存
static int ifm_mem_flush_cache(TAPPCTX *pstAppCtx, INFORMER_SVRENV* pstEnv, int iCount)
{
	int i;
	static int iIfmCheckPos = 0;
	LPSHITEM pstItem = NULL;
	IfmAuthBucket *pstBucket = NULL;
	static int iScanTime = 0;

	static int iScanLogTime = 0;
	UNUSED(pstAppCtx);
	if(sht_is_empty(pstEnv->pstIfmAuthCache))
	{
		return 0;
	}

	//每秒稍描一次
	if( time(NULL) - iScanTime >   1)
	{
		iScanTime = time(NULL);
	}
	else
	{
		return 0;
	}

	if( time(NULL) - iScanLogTime > 60 )
	{
		iScanLogTime = time(NULL);
		MutiLog(MUTI_LOGLEVEL_INFO, "ifm_mem_flush_cache: iBucket: %d,iMax: %d iItem: %d", 
		pstEnv->pstIfmAuthCache->iItem,
		pstEnv->pstIfmAuthCache->iMax,	
		pstEnv->pstIfmAuthCache->iItem);
	}
	

	for(i = 0; i < iCount; i++) 
	{
		if(iIfmCheckPos >= pstEnv->pstIfmAuthCache->iMax) 
		{
			break;
		}
		
		pstItem = SHT_GET_ITEM(pstEnv->pstIfmAuthCache, iIfmCheckPos++);
		if(!pstItem->fValid)
		{
			continue;
		}
		
		pstBucket = (IfmAuthBucket *)pstItem->szData;

		if(  time(NULL) - pstBucket->tTime > IFM_MAX_CACHE_FLUSH_TIME)
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "timeout remove name: %s", pstBucket->AccountLoginName);
			ifm_mem_remove_cache(pstEnv, pstBucket->AccountLoginName);
		}
	}
	
	if (iIfmCheckPos < pstEnv->pstIfmAuthCache->iMax)
	{
		return 0;
	}
	
	iIfmCheckPos = 0;
	return 0;
}

// 生成密钥
static inline void ifm_gen_key(char *psKey, const char * pszPasswd)
{
	char szTmp[KEY_LEN];

	Md5HashBuffer(szTmp, pszPasswd, strlen(pszPasswd));
	Md5HashBuffer(psKey, szTmp, sizeof(szTmp));
}
static int ifm_send2channel(TAPPCTX *pstAppCtx,INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread, 
								char *pszBuff, int iBuff)
{	
	char szMsg[MAX_MSG_SIZE];
	int iLen = 0;
	int iOffset = 0;
	int iRet = 0;
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	
	memset(szMsg,0,sizeof(szMsg));
	iLen = sizeof(szMsg) - sizeof(ChannelAppend);
	if(iBuff > iLen)
	{
		return -1;
	}
	memcpy(szMsg,&pstThread->stChannelAppend,sizeof(ChannelAppend));
	
	iOffset += sizeof(ChannelAppend);
	memcpy(szMsg+iOffset,pszBuff,iBuff);
	iOffset += iBuff;

	iRet = tbus_channel_send(pstThread->pstChannel1, szMsg, iOffset, 0);
	if (0 != iRet)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "tbus_channel_send error Ret %d", iRet);
		return -1;
	}
	return 0;
}
// 根据路由信息回送数据
static int ss_backward_pkg(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,
                           SSPKG *pstPkg, int iCmd)
{
	TDRDATA stNet;
	int iRet = 0;
	UNUSED(pstAppCtx);
	
	pstPkg->Head.Cmd = iCmd;
	pstPkg->Head.Magic = PROTOCOL_MAGIC; 
	//pstPkg->Head.Ver = TDR_METALIB_PROTO_SS_VERSION;
	//pstPkg->Head.HeadLen = sizeof(pstPkg->Head);

	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = pstEnv->szSendBuffer;
	if(ss_pack(pstEnv->pstSSHan, &stNet, pstPkg, 0) < 0)
	{
		return -1;
	}
	
	iRet = ifm_send2channel(pstAppCtx,pstEnv,pstThread, stNet.pszBuff, stNet.iBuff);
	if (0 != iRet)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "ifm_send2channel error Ret %d", iRet);
		return -1;
	}
	return 0;
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


// 回复生产模式的帐号信息
static int ss_produce_account_respond(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,
	                                    const SSGETACCOUNTREQ * pstReq)
{
	/* 反向激活 */
	char *paras[12] = {NULL};
	char message_id[IFM_MESSAGE_ID_LEN] ="104";
	char loginname[50] = {0};
	char password[17] = {0};
	char ip[16] = "";
	SSGETACCOUNTRES stRes;
	SSGETACCOUNTRES *pstRes;
	SelectInfo * pstExtraInfo;
	int iRet = 0;
	unsigned long long ullSeq=0;
	UNUSED(pstThread);

	memset(password,0,sizeof(password));
	
	STRNCPY(loginname, pstReq->LoginName,   CCH(loginname));
	snprintf(password,sizeof(password),"%s",ifm_peek_passwd(pstReq->ExtraData, pstReq->ExtraDataLen));
	pstExtraInfo = (SelectInfo *)pstReq->ExtraData;

	MutiLog(MUTI_LOGLEVEL_INFO, "ss_produce_account_respond account %s,ActiveFlag:%d", loginname,pstExtraInfo->ActiveFlag);
	
	if( pstExtraInfo->ActiveFlag == 1)
	{
		/* 激活码的逻辑 */
		char *paras[12] = {NULL};
		char message_id[IFM_MESSAGE_ID_LEN] ="109";
		char ActiveCode[100] = {0};
		char Sequence[20]={0};

		memset(ActiveCode,0,sizeof(ActiveCode));
		STRNCPY(ActiveCode,pstExtraInfo->ActiveCode,CCH(ActiveCode));
		
		pstEnv->ullIfmSeq++;
		if( (int )pstEnv->ullIfmSeq<= 0  )
		{
			pstEnv->ullIfmSeq = 1;	
		}	

		pstRes = (SSGETACCOUNTRES *)ifm_mem_cached_account(pstEnv, loginname);
		if( pstRes == NULL)
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "active fail,no find account %s", loginname);
			return -1;
		}		
		memcpy(&ullSeq,pstRes->ExtraData+pstRes->ExtraDataLen+sizeof(ChannelAppend),sizeof(ullSeq));

		//把连接的相关数据更新下
		memcpy(pstRes->ExtraData,pstExtraInfo,sizeof(SelectInfo));

		
		snprintf(Sequence,sizeof(Sequence),"%llu",ullSeq);
		paras[0] = message_id;
		paras[1] = Sequence;
		paras[2] = loginname;
		paras[3] = ActiveCode;

		MutiLog(MUTI_LOGLEVEL_INFO, "post 109 request loginname=[%s] Sequence=[%s] ActiveCode=[%s]", 
				loginname, Sequence,ActiveCode);
		
		iRet = ifm_post_request(pstEnv->pstConf->ClientServiceName, pstEnv->ullIfmSeq, paras, 4) ;
		if (iRet != 0)
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "post request failed, loginname=[%s] message_id=[%s]", loginname, message_id);
			return -1;
		}	

		/*// test code 
		char Sequence1[12]={0};
		char Sequence2[12]={0};
		char Sequence3[12]={0};
		char Sequence4[12]={0};
		char Sequence5[12]={0};
		paras[4] = Sequence1;
		paras[5] = Sequence2;
		paras[6] = Sequence3;
		uint32_t para_count = 3;
		memset(Sequence,0,sizeof(Sequence));
		snprintf(Sequence,sizeof(Sequence), "%s","0");
		ifm_process_9c_active(pstEnv,pstThread,paras,para_count);*/
		
	}
	else if(pstExtraInfo->ActiveFlag == 2 )
	{
		// 4.1.	反向注册
		// 如果cache里有,必须要先删除掉
		if( ifm_mem_cached_account(pstEnv, loginname) )
		{
			MutiLog(MUTI_LOGLEVEL_ERROR,"LoginName:%s, had cached! ifm_mem_remove_cache !  ",pstReq->LoginName);
		}

		ifm_mem_remove_cache(pstEnv, loginname);

		memset(message_id,0,sizeof(message_id));
		snprintf(message_id,sizeof(message_id),"103");
		paras[0] = message_id;
		paras[1] = loginname;
		paras[2] = password;
		paras[3] = pstExtraInfo->stAuthInfo.CertID;
		paras[4] = pstExtraInfo->stAuthInfo.RealName;
		paras[5] = pstExtraInfo->stAuthInfo.eMail;
		paras[6] = pstExtraInfo->stAuthInfo.Mobile;
		
		memset(ip,0,sizeof(ip));
		snprintf( ip,sizeof(ip),"%s",
			inet_ntoa(*(struct in_addr *)&pstExtraInfo->stConnCtx.stExtraInfo.IPInfo.ip) );
		paras[7] = ip;
		
		// post request
		pstEnv->ullIfmSeq++;
		if( (int )pstEnv->ullIfmSeq<= 0  )
		{
			pstEnv->ullIfmSeq = 1;	
		}
		iRet = ifm_post_request(pstEnv->pstConf->ClientServiceName, pstEnv->ullIfmSeq, paras, 8) ;
		if (iRet != 0)
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "post request failed, loginname=[%s] message_id=[%s]", loginname, message_id);
			return -1;
		}
	
		//把数据存入到内存里面
		memset(&stRes,0,sizeof(stRes));
		STRNCPY(stRes.Account.Pass9Account, pstReq->LoginName,   CCH(stRes.Account.Pass9Account));
		ifm_gen_key(stRes.Account.PasswdHash, 
			ifm_peek_passwd(pstReq->ExtraData, pstReq->ExtraDataLen) );
		stRes.ExtraDataLen = pstReq->ExtraDataLen;
		memcpy(stRes.ExtraData, pstReq->ExtraData, stRes.ExtraDataLen);
		//追加一个记录数据
		if( stRes.ExtraDataLen+sizeof(ChannelAppend) < sizeof(stRes.ExtraData) )
		{
			memcpy(stRes.ExtraData+stRes.ExtraDataLen,&pstThread->stChannelAppend,sizeof(ChannelAppend));
			//stRes.ExtraDataLen += sizeof(ChannelAppend);
		}

		if( 0 > ifm_mem_insert_cache(pstAppCtx, pstEnv, &stRes,  stRes.Account.Pass9Account) )
		{
			MutiLog(MUTI_LOGLEVEL_ERROR,"LoginName:%s, ifm_mem_insert_cache fail! ",pstReq->LoginName);
		}

		/*// test code
		char Sequence[12]={0};
		char Sequence1[12]={0};
		char Sequence2[12]={0};
		char Sequence3[12]={0};
		char Sequence4[12]={0};
		char Sequence5[12]={0};
		uint32_t para_count = 3;
		memset(Sequence,0,sizeof(Sequence));
		snprintf(Sequence,sizeof(Sequence),"%s", "40");
		snprintf(Sequence5,sizeof(Sequence5),"%s", "140");
		paras[0] = message_id;
		paras[1] = Sequence;
		paras[2] = Sequence5;
		paras[3] = loginname;
		paras[4] = Sequence1;
		paras[5] = Sequence2;
		paras[6] = Sequence3;
		
		ifm_process_9c_active(pstEnv,pstThread,paras,para_count);*/
	}
	else
	{
		// 如果cache里有,必须要先删除掉
		if( ifm_mem_cached_account(pstEnv, loginname) )
		{
			MutiLog(MUTI_LOGLEVEL_ERROR,"LoginName:%s, had cached! ifm_mem_remove_cache !  ",pstReq->LoginName);
		}

		ifm_mem_remove_cache(pstEnv, loginname);
		
		paras[0] = message_id;
		paras[1] = loginname;
		paras[2] = password;

		memset(ip,0,sizeof(ip));
		snprintf( ip,sizeof(ip),"%s",
			inet_ntoa(*(struct in_addr *)&pstExtraInfo->stConnCtx.stExtraInfo.IPInfo.ip) );
		paras[3] = ip;
		

		// post request
		pstEnv->ullIfmSeq++;
		/* 做下保护,不要大于int的自然数,用户hash的比较*/
		if( (int )pstEnv->ullIfmSeq<= 0  )
		{
			pstEnv->ullIfmSeq = 1;	
		}
		iRet = ifm_post_request(pstEnv->pstConf->ClientServiceName, pstEnv->ullIfmSeq, paras, 4) ;
		if (iRet != 0)
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "post request failed, loginname=[%s] message_id=[%s]", loginname, message_id);
			return -1;
		}
	
		//把数据存入到内存里面
		memset(&stRes,0,sizeof(stRes));
		STRNCPY(stRes.Account.Pass9Account, pstReq->LoginName,   CCH(stRes.Account.Pass9Account));
		ifm_gen_key(stRes.Account.PasswdHash, 
			ifm_peek_passwd(pstReq->ExtraData, pstReq->ExtraDataLen) );
		stRes.ExtraDataLen = pstReq->ExtraDataLen;
		memcpy(stRes.ExtraData, pstReq->ExtraData, stRes.ExtraDataLen);
		//追加一个记录数据
		if( stRes.ExtraDataLen+sizeof(ChannelAppend) < sizeof(stRes.ExtraData) )
		{
			memcpy(stRes.ExtraData+stRes.ExtraDataLen,&pstThread->stChannelAppend,sizeof(ChannelAppend));
			//stRes.ExtraDataLen += sizeof(ChannelAppend);
		}

		if( 0 > ifm_mem_insert_cache(pstAppCtx, pstEnv, &stRes,  stRes.Account.Pass9Account) )
		{
			MutiLog(MUTI_LOGLEVEL_ERROR,"LoginName:%s, ifm_mem_insert_cache fail! ",pstReq->LoginName);
		}

		/*// test code 
		char Sequence[12]={0};
		char Sequence1[12]={0};
		char Sequence2[12]={0};
		char Sequence3[12]={0};
		char Sequence4[12]={0};
		char Sequence5[12]={0};
		uint32_t para_count = 3;
		memset(Sequence,0,sizeof(Sequence));
		snprintf(Sequence,sizeof(Sequence),"%s", "40");
		snprintf(Sequence5,sizeof(Sequence5),"%s", "140");
		paras[0] = message_id;
		paras[1] = Sequence;
		paras[2] = Sequence5;
		paras[3] = loginname;
		paras[4] = Sequence1;
		paras[5] = Sequence2;
		paras[6] = Sequence3;
		
		ifm_process_9c_active(pstEnv,pstThread,paras,para_count);*/
	}

	
	return 0;
}


// 回复开发帐号信息
static int ss_develop_account_respond(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,
	                                    const SSGETACCOUNTREQ * pstReq)
{
	SSPKG stPkg;
	SSGETACCOUNTRES *pstRes = &stPkg.Body.GetAccountRes;
	int iAuthFlag;
	
	bzero(pstRes, sizeof(*pstRes));
	
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

	if (0 == ifm_peek_auth_flag(pstReq->ExtraData,pstReq->ExtraDataLen,&iAuthFlag) )
	{
		if (iAuthFlag & AUTH_FLAG_QUICK)
		{
			pstRes->Account.UinFlag |= UIN_FLAG_QUICK;
		}
	}


	return ss_backward_pkg(pstAppCtx, pstEnv,pstThread, &stPkg, INFORMER_GET_ACCOUNT_RES);
}


static int ss_get_account_req(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,
	                            const SSGETACCOUNTREQ * pstGetAccountReq)
{
	int iRet = -1;
	
	switch(pstEnv->pstConf->AuthMode)
	{
	case IFM_MODE_DEVELOP:
		iRet = ss_develop_account_respond(pstAppCtx, pstEnv,pstThread, pstGetAccountReq);
		break;

	case IFM_MODE_PRODUCE:
		iRet = ss_produce_account_respond(pstAppCtx, pstEnv,pstThread, pstGetAccountReq);
		break;

	default:
		break;
	}
	return iRet;
}

// 处理 auth_svr 进程发送过来的信息。
static int ifm_process_auth_msg(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,
	                              const char *pszMsg, int iMsgLen)
{
	SSPKG stPkg;
	int iRet = 0;

	iRet = ss_unpack(pstEnv->pstSSHan, (char *)pszMsg, iMsgLen, &stPkg, 0);
	if(iRet < 0)
	{
		return -1;
	}
	
	switch(stPkg.Head.Cmd)
	{
	case INFORMER_GET_ACCOUNT_REQ:
		iRet = ss_get_account_req(pstAppCtx, pstEnv,pstThread,&stPkg.Body.GetAccountReq);
		break;
	default:
		return -1;
		break;
	}
	
	return iRet;
}

int informer_init_memory(INFORMER_SVRENV *pstEnv)
{
	pstEnv->pstIfmAuthMemory =	(IfmAuthMemory *) malloc(sizeof(IfmAuthMemory));
	if( (void *) -1  == pstEnv->pstIfmAuthMemory ||
		pstEnv->pstIfmAuthMemory == NULL )
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "new fail");
		return -1;
	}
	pstEnv->pstIfmAuthMemory->iVersion = OBJ_MEM_VER;
	
	pstEnv->pstIfmAuthCache = sht_init(pstEnv->pstIfmAuthMemory->sIfmMem, 
			                                 sizeof(pstEnv->pstIfmAuthMemory->sIfmMem),
												               IFM_BUCKET_NUM, IFM_MAX_NUM, 
												               sizeof(IfmAuthBucket));
	if(!pstEnv->pstIfmAuthCache)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "init_memory  ifm cache fail");
		return -1;
	}
	
	return 0;
}

static int ifm_db_torm_send(TAPPCTX* pstAppCtx, INFORMER_SVRENV* pstEnv, ThreadMgr *pstThread,TORMPKG *pstPkg,
	                      int iDstID)
{
	TDRDATA stData;
	int iProtoVer=0;
	int iRet = 0;

	if (iDstID == pstEnv->iAccountDBID)
	{
		iProtoVer = pstEnv->iAccountDBProtoVer;
	}

	stData.pszBuff = pstEnv->szSendBuffer;
	stData.iBuff = sizeof(pstEnv->szSendBuffer);
	if(0 != torm_pack_msg(pstEnv->hTorm, &stData, pstPkg, iProtoVer))
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "torm_pack_msg error");
		return -1;
	}
	
	iRet = ifm_send2channel(pstAppCtx,pstEnv,pstThread, stData.pszBuff, stData.iBuff);	
	if (0 != iRet)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "ifm_send2channel error Ret %d", iRet);
	}
	return 0;
}
static int ifm_db_send_req(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv, ThreadMgr *pstThread,
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
		MutiLog(MUTI_LOGLEVEL_ERROR, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > ifm_db_torm_send(pstAppCtx, pstEnv, pstThread,pstTormPkg, pstEnv->iAccountDBID))
	{
		return -1;
	}

	return 0;	
}
static void ifm_db_init_torm_head(	TAPPCTX *pstAppCtx, 
	                      					INFORMER_SVRENV *pstEnv,
                              				  	TORMPKGHEAD * pstHead,
                              				  	int iCmd, 
                              				  	int iCallBackCmd,
	                       					const char *pszMetaName, 
	                      					const char * pstCallBackData, 
	                      					int iContextLen,
	                      					unsigned long long  ullSeq)
{
	pstHead->HeadComm.InstID = pstAppCtx->iId;
	pstHead->HeadComm.Magic = TORM_MAGIC;
		
       pstHead->HeadCmd.Cmd = iCmd;
	if (pszMetaName)
	{
	       //如果有元的名字 把元的名字赋值
		STRNCPY(pstHead->HeadCmd.MetaNameVer.MetaName, pszMetaName, 
			      sizeof(pstHead->HeadCmd.MetaNameVer.MetaName));
	}
	pstHead->HeadCmd.MetaNameVer.MetaVer = 0;
	
	//做一个发包的顺序递增
	pstEnv->iTormSeq++;
	if (pstEnv->iTormSeq <= 0)
	{
		pstEnv->iTormSeq = 1;
	}
	pstHead->HeadApp.AppCmd = 0;
	pstHead->HeadApp.Seq = pstEnv->iTormSeq;
	pstHead->HeadApp.ObjID = 0;//多进程这里表示用进程id来表示

	//把长度和内容都记录下
	pstHead->AsynCallBackDataLen = 0;
	memcpy(pstHead->AsynCallBackData +pstHead->AsynCallBackDataLen,&iCallBackCmd, sizeof(iCallBackCmd));
	pstHead->AsynCallBackDataLen += sizeof(iCallBackCmd);
	memcpy(pstHead->AsynCallBackData +pstHead->AsynCallBackDataLen,&ullSeq, sizeof(ullSeq));
	pstHead->AsynCallBackDataLen += sizeof(ullSeq);
	
	if(pstCallBackData != NULL)
	{
		if(iContextLen > 0)
		{
			memcpy(pstHead->AsynCallBackData +pstHead->AsynCallBackDataLen,pstCallBackData, iContextLen);
			pstHead->AsynCallBackDataLen +=iContextLen;
		}
	}
}


static int ifm_db_req_insert_account(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv, ThreadMgr *pstThread,
	                               const ACCOUNT * pstAccount,unsigned long long  ullSeq)
{
	TORMPKG stPkg;
	TORMINSERTREQ *pstInsertReq = &stPkg.Body.InsertReq;
		
	ifm_db_init_torm_head(pstAppCtx, pstEnv,&stPkg.Head, TORM_INSERT_REQ, TORM_INSERT_RES,
			               "Account", (char *)pstAccount, (int)sizeof(ACCOUNT),ullSeq);

	//设置包体的信息
	pstInsertReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstInsertReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstInsertReq->OperInfo.WhereDef[0] = 0;

	return ifm_db_send_req(pstAppCtx, pstEnv,pstThread, &stPkg, pstAccount, sizeof(ACCOUNT));
}
static int ifm_db_insert_produce_account(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,
	                                   const INFORMERACCOUNT *pstIfmAccount,unsigned long long  ullSeq)
{
	ACCOUNT stAccount;

	memset(&stAccount,0,sizeof(stAccount));
	// 将从9c获取的数据插入数据库
	stAccount.Uin = 0;
	stAccount.UinFlag = pstIfmAccount->UinFlag;
	STRNCPY(stAccount.Pass9Account, pstIfmAccount->Pass9Account, 
		      sizeof(stAccount.Pass9Account));
	memcpy(stAccount.PasswdHash, pstIfmAccount->PasswdHash, 
		     sizeof(stAccount.PasswdHash));
	STRNCPY(stAccount.BirthDay, pstIfmAccount->BirthDay, 
		      sizeof(stAccount.BirthDay));
	memcpy(&stAccount.Gender, &pstIfmAccount->Gender, 1);
	STRNCPY(stAccount.RegisterTs,pstIfmAccount->RegisterTs,
		       sizeof(stAccount.RegisterTs));
	stAccount.SilenceEndTime = 0;
	memset(stAccount.FreezeReason,0,sizeof(stAccount.FreezeReason));
	stAccount.FreezeStartTime = 0;
	stAccount.FreezeEndTime = 0;
	stAccount.AccountCreateTime = time(NULL);
		
	return ifm_db_req_insert_account(pstAppCtx, pstEnv,pstThread, &stAccount,ullSeq);
}

static int ifm_process_9c_active(INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,char *paras[],uint32_t para_count)
{
	int iMessageId = 0;
	int iResult = 0;
	unsigned long long   ullSeq = 0;
	unsigned long long ullSeqTmp = 0;
	INFORMERACCOUNT stIfmAccount;
	SSGETACCOUNTRES *pstRes;
	SSPKG stPkg;
	SelectInfo * pstExtraInfo=NULL;
	
	if(para_count < 3 )
	{
		return -1;
	}
	/* 消息类型 */
	iMessageId = atoi(paras[0]);
	/* 错误码*/
	iResult = atoi(paras[1]);

	if( 104 == iMessageId  ||
		103 == iMessageId )
	{
		/* 交易流水号 */
		ullSeq = atoull(paras[2]);
		/* 成功获取信息,则需要把数据写入db */
	       stIfmAccount.UinFlag = 0;

		STRNCPY(stIfmAccount.Pass9Account, paras[3],CCH(stIfmAccount.Pass9Account));
		/* 生日 */
	       STRNCPY(stIfmAccount.BirthDay,paras[4], CCH(stIfmAccount.BirthDay));
		/* 性别 */
		memcpy(&stIfmAccount.Gender,paras[5], 1);
		/* 注册时间 */
		STRNCPY(stIfmAccount.RegisterTs,paras[6], CCH(stIfmAccount.RegisterTs));
	}
	else if( 109 == iMessageId )
	{
		/* 反向激活码 */
		STRNCPY(stIfmAccount.Pass9Account, paras[2],CCH(stIfmAccount.Pass9Account));
	}
	else
	{
		return -1;
	}

	/* 找到寄存的内存*/
	pstRes = (SSGETACCOUNTRES *)ifm_mem_cached_account(pstEnv, stIfmAccount.Pass9Account);
	if(pstRes == NULL )
	{
		MutiLog(MUTI_LOGLEVEL_ERROR,"ifm_useid:%s,seq:%llu,no cache account ",
			stIfmAccount.Pass9Account,
			ullSeq);
		ifm_mem_remove_cache(pstEnv,stIfmAccount.Pass9Account);
		return -1;
	}
	
	if( 0 !=  iResult )
	{
		pstRes->Found = iResult;
		STRNCPY(stIfmAccount.Pass9Account, pstRes->Account.Pass9Account,CCH(stIfmAccount.Pass9Account));
	}
	else
	{
		pstRes->Found = 0;
		/* 登陆名/用户编号 */
		//STRNCPY(stIfmAccount.Pass9Account, paras[3],CCH(stIfmAccount.Pass9Account));		
	}

	MutiLog(MUTI_LOGLEVEL_INFO,"active info messageid:%s,result:%s,seq:%s,account:%s,birthday:%s,Gender:%s,registerTs:%s",
			paras[0],paras[1],paras[2],paras[3],paras[4],paras[5],paras[6]);
	
	if( strcmp(stIfmAccount.Pass9Account,pstRes->Account.Pass9Account) != 0 )
	{
		/* 两个用户名不对*/
		MutiLog(MUTI_LOGLEVEL_ERROR,"account cache err messageid:%s,result:%s,seq:%s,account:%s,birthday:%s,Gender:%s,registerTs:%s",
			paras[0],paras[1],paras[2],paras[3],paras[4],paras[5],paras[6]);
		ifm_mem_remove_cache(pstEnv,stIfmAccount.Pass9Account);
		return -1;
	}

	if( 104 == iMessageId  ||
		103 == iMessageId  )
	{
		/* 密码 */
		memcpy(stIfmAccount.PasswdHash, pstRes->Account.PasswdHash,16);
		/* 把数据返回给auth_svr */
		pstRes->Account = stIfmAccount;
	}
	else if(109 == iMessageId)
	{
		memcpy(&ullSeqTmp,pstRes->ExtraData+pstRes->ExtraDataLen+sizeof(ChannelAppend),sizeof(ullSeqTmp));
		if( ullSeqTmp == 0)
		{
			pstRes->Found = 20;//用户详细数据被清理掉了
		}
	}
	
	stPkg.Body.GetAccountRes = *pstRes;

	if( pstRes->ExtraDataLen+sizeof(ChannelAppend) < sizeof(pstRes->ExtraData) )
	{
		memcpy(&pstThread->stChannelAppend,pstRes->ExtraData+pstRes->ExtraDataLen,sizeof(ChannelAppend));
	}
	
	
	pstExtraInfo = (SelectInfo * )pstRes->ExtraData;
	
	ss_backward_pkg(pstEnv->pstAppCtx, pstEnv,pstThread, &stPkg, INFORMER_GET_ACCOUNT_RES);
	
	if(iResult == 40 && (104 == iMessageId || 103 == iMessageId ) )
	{
		// 存下流水号
		memcpy(pstRes->ExtraData+pstRes->ExtraDataLen+sizeof(ChannelAppend),&ullSeq,sizeof(ullSeq));
	}
	else
	{
		ifm_mem_remove_cache(pstEnv,stIfmAccount.Pass9Account);
	}
	return 0;
		
}
static int ifm_process_9c_create_account(INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,char *paras[],uint32_t para_count)
{
	int iMessageId = 0;
	unsigned long long  ullSeq = 0;
	INFORMERACCOUNT stIfmAccount;

	if(para_count < 6 )
	{
		return -1;
	}
	/* 成功获取信息,则需要把数据写入db */
       stIfmAccount.UinFlag = 0;
	/* 消息类型 */
	iMessageId = atoi(paras[0]);
	/* 交易流水号 */
	ullSeq = atoull(paras[1]);
	/* 登陆名/用户编号 */
	STRNCPY(stIfmAccount.Pass9Account, paras[2],CCH(stIfmAccount.Pass9Account));
	/* 密码 */
	//memcpy(stIfmAccount.PasswdHash, paras[3],16);	
	ifm_gen_key(stIfmAccount.PasswdHash, paras[3] );
	/* 生日 */
       STRNCPY(stIfmAccount.BirthDay,paras[4], CCH(stIfmAccount.BirthDay));
	/* 性别 */
	memcpy(&stIfmAccount.Gender,paras[5], 1);
	/* 注册时间 */
	STRNCPY(stIfmAccount.RegisterTs,paras[6], CCH(stIfmAccount.RegisterTs));

	/* 把数据插入db*/
	ifm_db_insert_produce_account(pstEnv->pstAppCtx,pstEnv,pstThread,&stIfmAccount,ullSeq);

	MutiLog(MUTI_LOGLEVEL_INFO, "ifm_process_9c_create_account account = %s", stIfmAccount.Pass9Account);
	return 0;
}

static int ifm_process_9c_update_passwd(INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,char *paras[],uint32_t para_count)
{
	int iMessageId = 0;
	unsigned long long   ullSeq = 0;
	INFORMERACCOUNT stIfmAccount;
	TORMPKG stPkg;
	TORMUPDATEREQ *pstDBReq = &stPkg.Body.UpdateReq;
	UPDATEPASSWD stUpdatePasswd;

	if(para_count < 2 )
	{
		return -1;
	}
	/* 成功获取信息,则需要把数据写入db */
       stIfmAccount.UinFlag = 0;
	/* 消息类型 */
	iMessageId = atoi(paras[0]);
	/* 交易流水号 */
	ullSeq = atoull(paras[1]);
	/* 登陆名/用户编号 */
	STRNCPY(stIfmAccount.Pass9Account, paras[2],CCH(stIfmAccount.Pass9Account));
	/* 密码 */
	//memcpy(stIfmAccount.PasswdHash, paras[3],16);	
	ifm_gen_key(stIfmAccount.PasswdHash, paras[3] );
	/* 生效时间 */
       //STRNCPY(stIfmAccount.Effect_ts,paras[4], CCH(stIfmAccount.Effect_ts));
	

	/* 做下update 操作*/
	stUpdatePasswd.Uin = 0;
	memcpy(stUpdatePasswd.PasswdHash, stIfmAccount.PasswdHash,16);	
	ifm_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_UPDATE_REQ, DB_RES_UPDATE_PASSWD,
			               "UpdatePasswd", (char *)&stIfmAccount, 
			              sizeof(INFORMERACCOUNT),ullSeq);

	pstDBReq->DoInsert = 0;
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	snprintf(pstDBReq->OperInfo.WhereDef,
	         sizeof(pstDBReq->OperInfo.WhereDef),
	         "where Pass9Account = '%s'", 
		       stIfmAccount.Pass9Account);

	MutiLog(MUTI_LOGLEVEL_INFO, "update account info account = %s, cmd = %d", 
			         stIfmAccount.Pass9Account,
			         DB_RES_UPDATE_PASSWD);
	return ifm_db_send_req(pstEnv->pstAppCtx, pstEnv,pstThread, &stPkg,(void *)&stUpdatePasswd, (int)sizeof(stUpdatePasswd));
}
static int ifm_process_9c_update_account(INFORMER_SVRENV *pstEnv, ThreadMgr *pstThread,char *paras[],uint32_t para_count)
{
	int iMessageId = 0;
	unsigned long long  ullSeq = 0;
	INFORMERACCOUNT stIfmAccount;
	TORMPKG stPkg;
	TORMUPDATEREQ *pstDBReq = &stPkg.Body.UpdateReq;
	UPDATEACCOUNT stUpdateAccount;

	if( para_count < 3)
	{
		return -1;
	}
	/* 成功获取信息,则需要把数据写入db */
       stIfmAccount.UinFlag = 0;
	/* 消息类型 */
	iMessageId = atoi(paras[0]);
	/* 交易流水号 */
	ullSeq = atoull(paras[1]);
	/* 登陆名/用户编号 */
	STRNCPY(stIfmAccount.Pass9Account, paras[2],CCH(stIfmAccount.Pass9Account));
	/* 生日 */
       STRNCPY(stIfmAccount.BirthDay,paras[3], CCH(stIfmAccount.BirthDay));
	/* 性别 */
	memcpy(&stIfmAccount.Gender,paras[4], 1);
	/* 生效时间 */
       //STRNCPY(stIfmAccount.Effect_ts,paras[5], CCH(stIfmAccount.Effect_ts));

	MutiLog(MUTI_LOGLEVEL_INFO, "ifm_process_9c_update_account account = %s", stIfmAccount.Pass9Account);
	/* 做下update 操作*/
	stUpdateAccount.Uin = 0;
	STRNCPY(stUpdateAccount.BirthDay,paras[3], CCH(stUpdateAccount.BirthDay));
	memcpy(&stUpdateAccount.Gender,paras[4], 1);
	ifm_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_UPDATE_REQ, DB_RES_UPDATE_ACCOUNT,
			               "UpdateAccount",  (char *)&stIfmAccount, 
			              sizeof(INFORMERACCOUNT),ullSeq);

	pstDBReq->DoInsert = 0;
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	snprintf(pstDBReq->OperInfo.WhereDef,
	         sizeof(pstDBReq->OperInfo.WhereDef),
	         "where Pass9Account = '%s'", 
		       stIfmAccount.Pass9Account);

	return ifm_db_send_req(pstEnv->pstAppCtx, pstEnv,pstThread,&stPkg, &stUpdateAccount, sizeof(UPDATEACCOUNT));
}

static int ifm_process_9c_freeze_account(INFORMER_SVRENV *pstEnv, ThreadMgr *pstThread,char *paras[],uint32_t para_count)
{
	int iMessageId = 0;
	unsigned long long   ullSeq = 0;
	ACCOUNTFREEZEENDTIME stAccFreezeEndTime;
	char Pass9Account[INFORMER_ACCOUNT_LOGIN_NAME_LEN];
	INFORMERACCOUNT stIfmAccount;
	TORMPKG stPkg;
	TORMUPDATEREQ *pstDBReq = &stPkg.Body.UpdateReq;
	
	memset(Pass9Account,0,sizeof(Pass9Account));
	memset(&stAccFreezeEndTime,0,sizeof(stAccFreezeEndTime));
	memset(&stIfmAccount,0,sizeof(stIfmAccount));
	
	if( para_count < 3)
	{
		return -1;
	}
	stAccFreezeEndTime.Uin = 0;
	/* 消息类型 */
	iMessageId = atoi(paras[0]);
	/* 交易流水号 */
	ullSeq = atoull(paras[1]);
	/* 登陆名/用户编号 */
	STRNCPY(Pass9Account, paras[2],CCH(Pass9Account));
	/* 封停原因 */
	STRNCPY(stAccFreezeEndTime.FreezeReason, paras[3],CCH(stAccFreezeEndTime.FreezeReason));
	/* 生效时间 */
	stAccFreezeEndTime.FreezeStartTime = string2time(paras[4]);
	/* 解封时间 */
	stAccFreezeEndTime.FreezeEndTime = string2time(paras[5]);
	
	/* 做下update 操作*/


	STRNCPY(stIfmAccount.Pass9Account, paras[2],CCH(stIfmAccount.Pass9Account));
	ifm_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_UPDATE_REQ, DB_RES_FREEZE_ACCOUNT,
			               "AccountFreezeEndTime",  (char *)&stIfmAccount, 
			              sizeof(INFORMERACCOUNT),ullSeq);

	pstDBReq->DoInsert = 0;
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	snprintf(pstDBReq->OperInfo.WhereDef,
	         sizeof(pstDBReq->OperInfo.WhereDef),
	         "where Pass9Account = '%s'", 
		       Pass9Account);

	MutiLog(MUTI_LOGLEVEL_INFO,"9c_freeze_account req iMessageId:%s,ullSeq:%s,Pass9Account:%s",
			paras[0],paras[1],paras[2]);

	return ifm_db_send_req(pstEnv->pstAppCtx, pstEnv,pstThread,&stPkg, &stAccFreezeEndTime, sizeof(ACCOUNTFREEZEENDTIME));
}


static int ifm_process_9c_free_account(INFORMER_SVRENV *pstEnv, ThreadMgr *pstThread,char *paras[],uint32_t para_count)
{
	int iMessageId = 0;
	unsigned long long   ullSeq = 0;
	ACCOUNTFREEZEENDTIME stAccFreezeEndTime;
	char Pass9Account[INFORMER_ACCOUNT_LOGIN_NAME_LEN];
	INFORMERACCOUNT stIfmAccount;
	TORMPKG stPkg;
	TORMUPDATEREQ *pstDBReq = &stPkg.Body.UpdateReq;
	
	memset(Pass9Account,0,sizeof(Pass9Account));
	memset(&stAccFreezeEndTime,0,sizeof(stAccFreezeEndTime));
	memset(&stIfmAccount,0,sizeof(stIfmAccount));
	
	if( para_count < 3)
	{
		return -1;
	}
	stAccFreezeEndTime.Uin = 0;
	/* 消息类型 */
	iMessageId = atoi(paras[0]);
	/* 交易流水号 */
	ullSeq = atoull(paras[1]);
	/* 登陆名/用户编号 */
	STRNCPY(Pass9Account, paras[2],CCH(Pass9Account));
	/* 封停原因 */
	STRNCPY(stAccFreezeEndTime.FreezeReason, paras[3],CCH(stAccFreezeEndTime.FreezeReason));
	/* 生效时间 */
	stAccFreezeEndTime.FreezeStartTime = string2time(paras[4]);
	/* 解封时间 */
	stAccFreezeEndTime.FreezeEndTime = stAccFreezeEndTime.FreezeStartTime;
	
	/* 做下update 操作*/
	MutiLog(MUTI_LOGLEVEL_INFO, "ifm_process_9c_free_account account = %s", Pass9Account);

	STRNCPY(stIfmAccount.Pass9Account, paras[2],CCH(stIfmAccount.Pass9Account));
	ifm_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_UPDATE_REQ, DB_RES_FREE_ACCOUNT,
			               "AccountFreezeEndTime",  (char *)&stIfmAccount, 
			              sizeof(INFORMERACCOUNT),ullSeq);

	pstDBReq->DoInsert = 0;
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	snprintf(pstDBReq->OperInfo.WhereDef,
	         sizeof(pstDBReq->OperInfo.WhereDef),
	         "where Pass9Account = '%s'", 
		       Pass9Account);

	return ifm_db_send_req(pstEnv->pstAppCtx, pstEnv,pstThread,&stPkg, &stAccFreezeEndTime, sizeof(ACCOUNTFREEZEENDTIME));
}

static int ifm_process_9c_cash_bill(INFORMER_SVRENV *pstEnv, ThreadMgr *pstThread,char *paras[],uint32_t para_count)
{
	SSPKG stPkg;
	SSCASHBILLREQ *pstReq;

	pstReq = &stPkg.Body.CashBillReq;
	
	if( para_count < 3)
	{
		return -1;
	}
	
	/* 消息类型 */
	pstReq->MessageId = atoi(paras[0]);
	/* 交易流水号 */
	pstReq->Sequence = atoull(paras[1]);
	/* 登陆名/用户编号 */
	STRNCPY(pstReq->Pass9Account, paras[2],CCH(pstReq->Pass9Account));
	/* 充值到的服务器编号 */
	pstReq->ServerID = atoi(paras[3]);
	pstReq->Amount = atoull(paras[4]);
	pstReq->Points = atoull(paras[5]);
	pstReq->Bonus =  atoull(paras[6]);
	pstReq->PropsID = atoull(paras[7]);
	STRNCPY(pstReq->PropsName,paras[8],CCH(pstReq->PropsName));
	pstReq->Uin = 0;

	/* 起始的数据没有ChannelAppend */
	memset(&pstThread->stChannelAppend,0,sizeof(ChannelAppend));
	ss_backward_pkg(pstEnv->pstAppCtx, pstEnv,pstThread, &stPkg, CASH_BILL_REQ);

	MutiLog(MUTI_LOGLEVEL_INFO, "ifm_process_9c_cash_bill account = %s Points:%lld ", pstReq->Pass9Account,pstReq->Points);
	
	return 0;
}

static int ifm_process_9c_query_bill(INFORMER_SVRENV *pstEnv, ThreadMgr *pstThread,char *paras[],uint32_t para_count)
{
	SSPKG stPkg;
	SSQUERYBILLREQ *pstReq;

	pstReq = &stPkg.Body.QueryBillReq;
	
	if( para_count < 3)
	{
		return -1;
	}
	
	/* 消息类型 */
	pstReq->MessageId = atoi(paras[0]);
	/* 交易流水号 */
	pstReq->Sequence = atoull(paras[1]);
	/* 登陆名/用户编号 */
	STRNCPY(pstReq->Pass9Account, paras[2],CCH(pstReq->Pass9Account));
	/* 服务器编号 */
	pstReq->ServerID = atoi(paras[3]);
	
	/* 起始的数据没有ChannelAppend */
	memset(&pstThread->stChannelAppend,0,sizeof(ChannelAppend));
	ss_backward_pkg(pstEnv->pstAppCtx, pstEnv,pstThread, &stPkg, QUERY_BILL_REQ);

	MutiLog(MUTI_LOGLEVEL_INFO, "ifm_process_9c_query_bill account = %s ", pstReq->Pass9Account);
	
	return 0;
}

static int ifm_process_9c_cmds(INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,char *paras[],uint32_t para_count)
{
	int iRet = 1;
	int iMessageID = 0;
	
	if(para_count > 0  || para_count <= IFM_PARAS_NUM_MAX )
	{
		iMessageID = atoi(paras[0]);
		switch(iMessageID)	
		{
			case 104: /* 反向激活 */
			case 109: /* 反向激活码 */
			case 103: /* 反向注册返回 */
				iRet = ifm_process_9c_active(pstEnv,pstThread,paras,para_count);
				break;
			case 1: /* 3.1.	创建账号 */
				iRet = ifm_process_9c_create_account(pstEnv,pstThread,paras,para_count);
				break;
			case 2: /* 3.2.	修改密码 */
				iRet = ifm_process_9c_update_passwd(pstEnv,pstThread,paras,para_count);
				break;
			case 3:/* 3.3.	修改账号信息 */
				iRet = ifm_process_9c_update_account(pstEnv,pstThread,paras,para_count);
				break;
			case 8: /* 3.8.	封停账号 */
				iRet = ifm_process_9c_freeze_account(pstEnv,pstThread,paras,para_count);
				break;
			case 9:/* 3.9.	解封账号 */
				iRet = ifm_process_9c_free_account(pstEnv,pstThread,paras,para_count);
				break;
			case 10:/* 3.10.	账号充值 */
				iRet = ifm_process_9c_cash_bill(pstEnv,pstThread,paras,para_count);
				break;
			case 11:/* 3.11.	查询余额 */
				iRet = ifm_process_9c_query_bill(pstEnv,pstThread,paras,para_count);
				break;
		       default:
			   	iRet = 1;
			   	break;
		}
	}	
	return 0;
}

static int ifm_process_9c_msg(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread)
{
	uint32_t para_count=0;
	unsigned long long  ullSeq = 0;
	int i = 0;
	int iRet = 0;
	char *paras[IFM_PARAS_NUM_MAX] = {NULL};
	char tmp_para[IFM_PARAS_NUM_MAX][IFM_PARAS_BUFFER_LEN];
	static int iScanTime=0;
	int iCurrTime = time(NULL);
	int iCount=1000;
	UNUSED(pstAppCtx);

	if(iCurrTime < iScanTime+SCAN_TIME_INTERVAL)
	{
		return 0;
	}

	for( i=0; i<IFM_PARAS_NUM_MAX;i++)
	{
		memset(tmp_para[i],0,IFM_PARAS_BUFFER_LEN);
		paras[i] = tmp_para[i];
	}
	para_count = 11;
	ullSeq = 0;	
	
	if( pstThread->iIdx == 0 )
	{
		setenv("IFM_CONFIG_FILE_PATH",pstEnv->pstConf->ClientServiceChdir,1);
		while(iRet == 0 &&  iCount>0)
		{
			iCount--;

			for( i=0; i<IFM_PARAS_NUM_MAX;i++)
			{
				memset(tmp_para[i],0,IFM_PARAS_BUFFER_LEN);
				paras[i] = tmp_para[i];
			}
			para_count = 11;
			ullSeq = 0;	
			
			iRet = ifm_get_response(pstEnv->pstConf->ClientServiceName, &ullSeq, paras, &para_count);
			MutiLog(MUTI_LOGLEVEL_DEBUG,	"ifm_process_9c_msg,idx:%d,iRet=%d,iCount=%d",
				 pstThread->iIdx,iRet,iCount);
				
			if (iRet == 0)
			{
				 ifm_process_9c_cmds(pstEnv,pstThread,paras,para_count);
			}
			else
			{
				iScanTime = iCurrTime;
			}
		}
	}
	else if( pstThread->iIdx == 1 )
	{
		setenv("IFM_CONFIG_FILE_PATH",pstEnv->pstConf->ServerServiceChdir,1);
		while(iRet == 0 &&  iCount>0)
		{
			iCount--;

			for( i=0; i<IFM_PARAS_NUM_MAX;i++)
			{
				memset(tmp_para[i],0,IFM_PARAS_BUFFER_LEN);
				paras[i] = tmp_para[i];
			}
			para_count = 11;
			ullSeq = 0;	
			
			iRet = ifm_get_request(pstEnv->pstConf->ServerServiceName, &ullSeq, paras, &para_count);
			MutiLog(MUTI_LOGLEVEL_DEBUG,	"ifm_process_9c_msg,idx:%d,iRet=%d,iCount=%d",
				 pstThread->iIdx,iRet,iCount);	
			if (iRet == 0)
			{
			     ifm_process_9c_cmds(pstEnv,pstThread,paras,para_count);
			}
			else
			{
				iScanTime = iCurrTime;
			}
		}
	}
	else if( pstThread->iIdx == 2)
	{
		setenv("IFM_CONFIG_FILE_PATH",pstEnv->pstConf->ChargeServiceChdir,1);
		while(iRet == 0 &&  iCount>0)
		{
			iCount--;

			for( i=0; i<IFM_PARAS_NUM_MAX;i++)
			{
				memset(tmp_para[i],0,IFM_PARAS_BUFFER_LEN);
				paras[i] = tmp_para[i];
			}
			para_count = 11;
			ullSeq = 0;	
			
			iRet = ifm_get_request(pstEnv->pstConf->ChargeServiceName, &ullSeq, paras, &para_count);
			MutiLog(MUTI_LOGLEVEL_DEBUG,	"ifm_process_9c_msg,idx:%d,iRet=%d,iCount=%d",
				 pstThread->iIdx,iRet,iCount);	
			if (iRet == 0)
			{
			     ifm_process_9c_cmds(pstEnv,pstThread,paras,para_count);
			}
			else
			{
				iScanTime = iCurrTime;
			}
		}
	}

	return 0;    

}

static int ss_sendto_auth_bus_pkg(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv, char * pszBuff,int iBuff)
{
	int iSrc, iDst;
	iSrc = pstAppCtx->iId;
	iDst = pstEnv->iAuthID;
	
	if(tbus_send(pstAppCtx->iBus, &iSrc, &iDst, pszBuff, iBuff, 0) < 0)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR,	"tbus_forward auth_svr fail.");
	}
	else
	{
		goto out;
	}
	/*iSrc = pstAppCtx->iId;
	iDst = pstEnv->iAuthSlaveID;
	if(tbus_send(pstAppCtx->iBus, &iSrc, &iDst, pszBuff, iBuff, 0) < 0)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR,	"tbus_forward auth_slave_svr fail.");
	}
	else
	{
		goto out;
	}
	
	iSrc = pstAppCtx->iId;
	iDst = pstEnv->iAuthBakID;
	if(tbus_send(pstAppCtx->iBus, &iSrc, &iDst, pszBuff, iBuff, 0) < 0)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR,	"tbus_forward auth_bak_svr fail.");
	}
	else
	{
		goto out;
	}*/
out:
	
	return 0;		
}
static int ss_sendto_region_bus_pkg(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv, char * pszBuff,int iBuff,SSPKG *pstPkg)
{
	int iSrc, iDst;
	char szAddr[64];
	UNUSED(pstEnv);
	UNUSED(pstPkg);

	memset(szAddr,0,sizeof(szAddr));
	
	iSrc = pstAppCtx->iId;
	iDst=pstEnv->iRegionGateID;
	if(tbus_send(pstAppCtx->iBus, &iSrc, &iDst, pszBuff, iBuff, 0) < 0)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR,	"ss_sendto_region_bus_pkg  fail.");
	}
	
	return 0;		
}

static int ss_sendto_auth_pkg(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv, ThreadMgr *pstThread,
	                            SSPKG *pstSSPkg)
{
	int iRet = 0;
	TDRDATA stNet;
	UNUSED(pstAppCtx);

	stNet.pszBuff = pstEnv->szSendBuffer;
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	if(ss_pack(pstEnv->pstSSHan, &stNet, pstSSPkg, 0) < 0)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "ss_pack fail");
		return -1;
	}

	iRet = ifm_send2channel(pstAppCtx,pstEnv,pstThread, stNet.pszBuff, stNet.iBuff);	
	if (0 != iRet)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "ifm_send2channel error Ret %d", iRet);
	}
	return 0;
}

static inline void ss_init_head(SSPKGHEAD *pstHead, int iCmd)
{
	pstHead->Cmd = iCmd;
	pstHead->HeadLen = sizeof(SSPKGHEAD);
	pstHead->Magic = PROTOCOL_MAGIC;
	pstHead->Ver = TDR_METALIB_PROTO_SS_VERSION;
}

static int ifm_response2pass9(INFORMER_SVRENV * pstEnv,int imsgid,unsigned long long  ullSeq,
								int Result,const char* Service_name)
{
	char message_id[4] = {0};
	char errcode[5] = {0};
	char Sequence[20] = {0};
	int para_count = 0;
	char *paras[IFM_PARAS_NUM_MAX];
	int iRet = 0;
	UNUSED(pstEnv);
	
	memset(errcode,0,sizeof(errcode));
	snprintf(errcode,sizeof(errcode),"%.4d",Result);
	memset(message_id,0,sizeof(message_id));
	snprintf(message_id,sizeof(message_id),"%d",imsgid);
	memset(Sequence,0,sizeof(Sequence));
	snprintf(Sequence,sizeof(Sequence),"%llu",ullSeq);
	para_count = 3;
	paras[0] = message_id;
	paras[1] = Sequence;
	paras[2] = errcode;

	//post response
	iRet = ifm_post_response( Service_name, ullSeq, paras, para_count);
	if ( iRet != 0)
	{
	       MutiLog(MUTI_LOGLEVEL_ERROR, "post response failed message_id=[%s]", message_id);
	}

	return 0;
}

static int ifm_db_insert_res(TAPPCTX * pstAppCtx, INFORMER_SVRENV * pstEnv, ThreadMgr *pstThread,
	                               TORMPKG * pstPkg)
{
	ACCOUNT stAccount;
	TORMINSERTRES * pstInsertRes = NULL;
	int iCallBackCmd = 0;
	unsigned long long  ullSeq = 0;
	int iOffset = 0;

	memset(&stAccount,0,sizeof(stAccount));
	pstInsertRes = &pstPkg->Body.InsertRes;
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);
	memcpy(&ullSeq,pstPkg->Head.AsynCallBackData+iOffset,sizeof(ullSeq));
	iOffset += sizeof(ullSeq);
	memcpy(&stAccount, pstPkg->Head.AsynCallBackData+iOffset,  sizeof(stAccount) );
	iOffset += sizeof(stAccount);

	if (TORM_ERR_IS_ERROR(pstInsertRes->ErrNo))
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, 
			         "Insert account info in db failed. DBErr = %d, Name = %s, Msg = %s.", 
			         pstInsertRes->DBMSErrNo,
			         stAccount.Pass9Account,
			         torm_error_string(pstInsertRes->ErrNo));
		if( pstInsertRes->DBMSErrNo == 1062)
		{
			ifm_response2pass9(pstEnv,1,ullSeq,0,pstEnv->pstConf->ServerServiceName);
		}
		else
		{
			ifm_response2pass9(pstEnv,1,ullSeq,99,pstEnv->pstConf->ServerServiceName);
		}
		
		return -1;
	}
	stAccount.Uin = (int)(pstInsertRes->LastInsertID);
	//需要给三个auth服务器发送更新内存数据的信息
	ss_init_head(&pstEnv->stSSPkg.Head, UPDATE_ACCOUNT_REP);
	STRNCPY(pstEnv->stSSPkg.Body.UpdateAccountRep.LoginName,
		      stAccount.Pass9Account,
		      sizeof(pstEnv->stSSPkg.Body.UpdateAccountRep.LoginName));

	ss_sendto_auth_pkg(pstAppCtx, pstEnv, pstThread,&pstEnv->stSSPkg);
	ifm_response2pass9(pstEnv,1,ullSeq,0,pstEnv->pstConf->ServerServiceName);
	return 0;
}

static int ifm_db_update_res(TAPPCTX * pstAppCtx, INFORMER_SVRENV * pstEnv, ThreadMgr *pstThread,
	                               TORMPKG *pstPkg)
{	TORMUPDATERES *pstUpdateRes = NULL;
	TORMPKGHEAD *pstHead = NULL;   
	int iCallBackCmd = 0;
	INFORMERACCOUNT *pstIfmAccount;
	int iOffset  = 0;
	unsigned long long  ullSeq = 0;
	int iRet = 0;
	UNUSED(pstThread);
	UNUSED(pstAppCtx);

	pstHead = &pstPkg->Head;
	pstUpdateRes = &pstPkg->Body.UpdateRes;
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);
	memcpy(&ullSeq,pstPkg->Head.AsynCallBackData+iOffset,sizeof(ullSeq));
	iOffset += sizeof(ullSeq);
	pstIfmAccount = (INFORMERACCOUNT *)(pstHead->AsynCallBackData+iOffset);
	iOffset += sizeof(INFORMERACCOUNT);
	if (TORM_ERR_IS_ERROR(pstUpdateRes->ErrNo))
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, 
			         "Insert account info in db failed. DBErr = %d, Name = %s, Msg = %s.", 
			         pstUpdateRes->DBMSErrNo,
			         pstIfmAccount->Pass9Account,
			         torm_error_string(pstUpdateRes->ErrNo));
		iRet = -1;
	}
	switch(iCallBackCmd)
	{
		case DB_RES_UPDATE_PASSWD:
			if( iRet != 0 )
			{
				ifm_response2pass9(pstEnv,2,ullSeq,99,pstEnv->pstConf->ServerServiceName);
				return -1;
			}
			ifm_response2pass9(pstEnv,2,ullSeq,0,pstEnv->pstConf->ServerServiceName);
			break;
		case DB_RES_UPDATE_ACCOUNT:
			if( iRet != 0 )
			{
				ifm_response2pass9(pstEnv,3,ullSeq,99,pstEnv->pstConf->ServerServiceName);
				return -1;
			}
			ifm_response2pass9(pstEnv,3,ullSeq,0,pstEnv->pstConf->ServerServiceName);
			break;
		case DB_RES_FREEZE_ACCOUNT:
			if( iRet != 0 )
			{
				ifm_response2pass9(pstEnv,8,ullSeq,99,pstEnv->pstConf->ServerServiceName);
				return -1;
			}
			ifm_response2pass9(pstEnv,8,ullSeq,0,pstEnv->pstConf->ServerServiceName);
			break;

		case DB_RES_FREE_ACCOUNT:
			if( iRet != 0 )
			{
				ifm_response2pass9(pstEnv,9,ullSeq,99,pstEnv->pstConf->ServerServiceName);
				return -1;
			}
			ifm_response2pass9(pstEnv,9,ullSeq,0,pstEnv->pstConf->ServerServiceName);
			break;
		default:
			break;
	}

	//需要给auth服务器发送更新内存数据的信息
	ss_init_head(&pstEnv->stSSPkg.Head, UPDATE_ACCOUNT_REP);
	STRNCPY(pstEnv->stSSPkg.Body.UpdateAccountRep.LoginName,
		      pstIfmAccount->Pass9Account,
		      sizeof(pstEnv->stSSPkg.Body.UpdateAccountRep.LoginName));

	ss_sendto_auth_pkg(pstAppCtx, pstEnv, pstThread,&pstEnv->stSSPkg);

	MutiLog(MUTI_LOGLEVEL_INFO, "update account info account = %s, cmd = %d", 
			         pstIfmAccount->Pass9Account,
			         iCallBackCmd);
	
	return 0;
}

// 处理account_db发送的信息，多数是回调信息。
static int ifm_process_torm_msg(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,

	                                     const char * pszMsg, int iMsgLen)

{
	TDRDATA stData;
	TORMPKG stPkg;
	int iRet = 0;
	int iProtoVer = pstEnv->iAccountDBProtoVer;
	
	memset(&stPkg,0,sizeof(stPkg));
	stData.pszBuff = (char *)pszMsg;
	stData.iBuff = iMsgLen;
	iRet =  torm_attach_res_msg(pstEnv->hTorm, &stData, &stPkg, iProtoVer);
	if (0 >iRet)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, 
			         "torm_attach_res_msg error:%s, iProtoVer=%d", 
		           torm_error_string(iRet), iProtoVer);
		return -1;
	}
	
	//只处理insert的返回
	switch(stPkg.Head.HeadCmd.Cmd)
	{
	case TORM_EXGVER_RES:
		pstEnv->iAccountDBProtoVer = stPkg.Body.ExgVerRes.Version;
		break;
	case TORM_INSERT_RES:
		iRet = ifm_db_insert_res(pstAppCtx, pstEnv,pstThread, &stPkg);
		break;
	case TORM_UPDATE_RES:
		iRet = ifm_db_update_res(pstAppCtx, pstEnv,pstThread, &stPkg);
	case TORM_SELECT_RES:		
	case TORM_DELETE_RES:
	case TORM_SELECT_COUNT_RES:
		break;

	default:
		iRet = -1;
		MutiLog(MUTI_LOGLEVEL_ERROR, "bad TORM cmd received %d.", stPkg.Head.HeadCmd.Cmd);
		break;
	}

	//MutiLog(MUTI_LOGLEVEL_INFO,"ifm_process_torm_msg db res ");
		
	return iRet;

}
static int ss_region_cash_bill_res(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,
								SSCASHBILLRES *pstCashBillRes)
{
	UNUSED(pstAppCtx);
	UNUSED(pstThread);
	
	if( pstCashBillRes->Result != 0 )
	{
		ifm_response2pass9(pstEnv,10,pstCashBillRes->CashBillReq.Sequence,99,pstEnv->pstConf->ChargeServiceName);
	}
	else
	{
		ifm_response2pass9(pstEnv,10,pstCashBillRes->CashBillReq.Sequence,0,pstEnv->pstConf->ChargeServiceName);
	}
	
	return 0;
}

static int ifm_region_query_bill_res(INFORMER_SVRENV * pstEnv,int imsgid,unsigned long long  ullSeq,
								int Result,const char* Service_name,SSQUERYBILLRES *pstQueryBillRes)
{
	char message_id[4] = {0};
	char errcode[5] = {0};
	char Sequence[20] = {0};
	char Points[20] = {0};
	char Bonus[20] = {0};
	char Amount[20] = {0};
	int para_count = 0;
	char *paras[IFM_PARAS_NUM_MAX];
	int iRet = 0;
	UNUSED(pstEnv);
	
	memset(errcode,0,sizeof(errcode));
	snprintf(errcode,sizeof(errcode),"%.4d",Result);
	memset(message_id,0,sizeof(message_id));
	snprintf(message_id,sizeof(message_id),"%d",imsgid);
	memset(Sequence,0,sizeof(Sequence));
	snprintf(Sequence,sizeof(Sequence),"%llu",ullSeq);
	memset(Points,0,sizeof(Points));
	snprintf(Points,sizeof(Points),"%llu",pstQueryBillRes->Points);	
	memset(Bonus,0,sizeof(Bonus));
	snprintf(Bonus,sizeof(Bonus),"%llu",pstQueryBillRes->Bonus);
	memset(Amount,0,sizeof(Amount));
	snprintf(Amount,sizeof(Amount),"%llu",pstQueryBillRes->Amount);

	
	para_count = 7;
	paras[0] = message_id;
	paras[1] = Sequence;
	paras[2] = errcode;
	paras[3] = time2string(time(NULL));
	paras[4] = Points;
	paras[5] = Bonus;
	paras[6] = Amount;

	//post response
	iRet = ifm_post_response( Service_name, ullSeq, paras, para_count);
	if ( iRet != 0)
	{
	       MutiLog(MUTI_LOGLEVEL_ERROR, "post response failed message_id=[%s]", message_id);
	}

	MutiLog(MUTI_LOGLEVEL_INFO,"query res messageid:%s,Sequence:%s,errcode:%s,time2string:%s,Points:%s,Bonus:%s,Amount:%s",
			paras[0],paras[1],paras[2],paras[3],paras[4],paras[5],paras[6]);
	
	return 0;
}


static int ss_region_query_bill_res(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,
								SSQUERYBILLRES *pstQueryBillRes)
{
	UNUSED(pstAppCtx);
	UNUSED(pstThread);
	
	if( pstQueryBillRes->Result !=0 )
	{
		ifm_response2pass9(pstEnv,11,pstQueryBillRes->Sequence,99,pstEnv->pstConf->ChargeServiceName);
	}
	else 
	{
		ifm_region_query_bill_res(pstEnv,11,pstQueryBillRes->Sequence,0,pstEnv->pstConf->ChargeServiceName,pstQueryBillRes);
	}
	
	return 0;
}

static int ifm_process_region_msg(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv,ThreadMgr *pstThread,
	                                     const char * pszMsg, int iMsgLen)
{
	SSPKG stPkg;
	int iRet = 0;

	if(ss_unpack(pstEnv->pstSSHan, (char *)pszMsg, iMsgLen, &stPkg, 0) < 0)
	{
		return -1;
	}
	
	switch(stPkg.Head.Cmd)
	{
	case CASH_BILL_RES:
		iRet = ss_region_cash_bill_res(pstAppCtx, pstEnv,pstThread,&stPkg.Body.CashBillRes);
		break;
	case QUERY_BILL_RES:
		iRet = ss_region_query_bill_res(pstAppCtx, pstEnv,pstThread,&stPkg.Body.QueryBillRes);
		break;

	case INFORMER_GET_ACCOUNT_REQ:
		iRet = ss_get_account_req(pstAppCtx, pstEnv,pstThread,&stPkg.Body.GetAccountReq);
		break;
		
	default:
		return -1;
		break;
	}
	
	return iRet;

}

void* thread_proc(void* pvArg)

{
	int iRet;
	ThreadMgr *pstThread;
	char szMsg[MAX_MSG_SIZE];
	int iLen;
	int iIdle = 0;
	INFORMER_SVRENV *pstEnv;

	pstThread = (ThreadMgr *)pvArg;
	pstEnv = (INFORMER_SVRENV *)pstThread->pstEnv;
	
	while(1)
	{
		if( pstEnv->piProExitFlag[0] )
		{
			ifm_set_process_exit_flag(pstEnv,pstThread->iIdx+1 );
			break;
		}

		// 只这个线程用到了内存
		if(pstThread->iIdx == 0)
		{
			ifm_mem_flush_cache(pstEnv->pstAppCtx, pstEnv,PRE_SCAN_BUCKET_NUM);
		}

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
				}
				/* 把sleep放if的外面,另外八idlesleep增加一倍*/
				usleep( (gs_stAppCtx.iIdleSleep*3)*1000);
			}
			else
			{
				MutiLog(MUTI_LOGLEVEL_ERROR, "tbus_channel_recv error Ret %d", iRet);
			}		

			//处理下9c来的数据
			ifm_process_9c_msg(pstEnv->pstAppCtx, pstEnv,pstThread);
			continue;

		}

		iIdle = 0;
		if (iLen <= (int)sizeof(ChannelAppend))
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "tbus_channel_recv error iLen %d", iLen);
			continue;
		}

		if(pstThread->iIdx == 0)
		{
			// 反向激活
			memcpy(&pstThread->stChannelAppend,szMsg,sizeof(ChannelAppend));
			
			iRet = ifm_process_auth_msg(pstEnv->pstAppCtx, pstEnv,pstThread,  &szMsg[sizeof(ChannelAppend)], iLen-sizeof(ChannelAppend));
			
		}
		else if( pstThread->iIdx == 1)
		{
			//正向激活
			memcpy(&pstThread->stChannelAppend,szMsg,sizeof(ChannelAppend));
			iRet = ifm_process_torm_msg(pstEnv->pstAppCtx, pstEnv, pstThread,  &szMsg[sizeof(ChannelAppend)], iLen-sizeof(ChannelAppend));
		}
		else if( pstThread->iIdx == 2 )
		{
			// 处理由region来的数据
			memcpy(&pstThread->stChannelAppend,szMsg,sizeof(ChannelAppend));
			iRet = ifm_process_region_msg(pstEnv->pstAppCtx, pstEnv, pstThread,  &szMsg[sizeof(ChannelAppend)], iLen-sizeof(ChannelAppend));
		}

		//处理下9c来的数据
		ifm_process_9c_msg(pstEnv->pstAppCtx, pstEnv,pstThread);				

	}


	return (void*)0;

}


int thread_init_one(INFORMER_SVRENV *pstEnv, ThreadMgr *pstThread, int iIdx)
{

	int iKey = 0;
	pid_t pid = 0;
	
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
	pstThread->pstEnv = pstEnv;

	/* 这里改成fork调用 */
	/*if( pthread_create(&pstThread->tid, NULL, thread_proc, pstThread)<0 )
	{
		return -1;
	}*/
	pid = fork();
	if( pid == 0 )
	{
		/* 子进程 */
		pstThread->tid = getpid();
		thread_proc(pstThread);
		return -1;
	}

	return 0;

}


int thread_init(INFORMER_SVRENV *pstEnv)
{
	int i;
	pstEnv->iThread = pstEnv->pstConf->MutiThreadNum;
	if (MAX_THREAD < pstEnv->iThread)
	{
		pstEnv->iThread = MAX_THREAD;
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


int informer_svr_init(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv)
{
	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		return -1;
	}
	pstEnv->pstAppCtx = pstAppCtx;
	pstEnv->pstConf = (INFORMER_SVRCONF *)pstAppCtx->stConfData.pszBuff;
	pstEnv->ullIfmSeq = 0;
	pstEnv->pstRunStat = (INFORMER_SVRRUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
	if (NULL == pstEnv->pstRunStat)
	{
		pstEnv->pstRunStat = &gs_stStat;
	}

	if(ss_han_create(g_szMetalib_proto_ss, &pstEnv->pstSSHan) < 0)
	{
		return -1;
	}
	
	// 计算auth_svr的ID，共有三个:主、辅、备用
	GET_REGION_ENTITY_ID(pstEnv->iAuthID, pstAppCtx->iId, FUNC_AUTH_SVR);
	GET_REGION_ENTITY_ID(pstEnv->iRegionGateID, pstAppCtx->iId, FUNC_REGION_GATE);
	GET_REGION_ENTITY_ID(pstEnv->iAccountDBID, pstAppCtx->iId, FUNC_ACCOUNT_DB);
	
	if (0 > torm_create_handle_from_dir(&pstEnv->hTorm, pstEnv->pstConf->DBMetaDir))
	{
		printf("torm_create_handle fail\n");
		return -1;
	}

	if( 0 > informer_init_memory(pstEnv) )
	{
		printf("informer new memory fail\n");
		return -1;
	}
	
	//pthread_mutex_init(&pstEnv->stLogMutex, NULL);
	/* 初始化文件锁 进程安全*/
	filelock_init(pstEnv->pstConf->FileLock);

	/* 主进程初始化下共享内存*/
	ifm_init_shm(pstEnv,pstEnv->pstConf->ProcessExitKey);
	
	/* 初始化日志 */
	if (0 > tapp_get_category(NULL, (void *)(&pstEnv->pstLogCat)))	
	{		
		printf("tapp_get_category run fail\n");
		return -1;	
	}	

	if (0 > thread_init(pstEnv))
	{
		return -1;	
	}
	
	printf("informer_svr start\n");
	MutiLog(MUTI_LOGLEVEL_INFO, "informer_svr start");

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

int proc_multi_bus2channel(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv)
{
	int i;
	int iBusy;
	int iRet;
	int iSrc;
	int iDst;
	TDRDATA stData;
	char szMsg[MAX_MSG_SIZE];
	iBusy = 0;
	//char szAddr[64];
	
	for (i=0; i<pstEnv->iThread; i++)
	{
		iSrc = TBUS_ADDR_ALL;
		iDst = pstEnv->pstAppCtx->iId;
		stData.iBuff = sizeof(szMsg) - sizeof(ChannelAppend);
		stData.pszBuff = (char *)szMsg + sizeof(ChannelAppend);
		
		iRet = tbus_recv(pstAppCtx->iBus, &iSrc, &iDst, stData.pszBuff, (size_t *)&stData.iBuff, 0);
		if (0 == iRet)
		{
			SSPKG stPkg;
			iBusy = 1;

			if(ss_unpack(pstEnv->pstSSHan, stData.pszBuff, stData.iBuff, &stPkg, 0) < 0)
			{
				return -1;
			}
	
			if (0 != store_appendinfo(pstAppCtx, (ChannelAppend *)szMsg))
			{
				MutiLog(MUTI_LOGLEVEL_ERROR, "store_appendinfo error");
			}
			
			stData.iBuff += sizeof(ChannelAppend);
			stData.pszBuff = (char *)szMsg;

			pstEnv->iLastSrcID = iSrc;
			
			//反响激活
			if( iSrc == pstEnv->iAuthID )
			{
				

				// 反向激活， auth数据放到第一个线程里面去处理,
				iRet = tbus_channel_send(pstEnv->astThread[0].pstChannel0, 
									stData.pszBuff, stData.iBuff, 0);

			}
			else if( iSrc == pstEnv->iAccountDBID )
			{
				pstEnv->iLastSrcID = iSrc;
				// 正向激活, db来的数据都放到第二个线程里面来处理
				iRet = tbus_channel_send(pstEnv->astThread[1].pstChannel0, 
									stData.pszBuff, stData.iBuff, 0);
			}
			else
			{
				/* 取下function id*/
				unsigned char ucEntity = 0;
				GET_FUNC_ENTITY(ucEntity,iSrc);
				if(ucEntity == FUNC_REGION_GATE )
				{
					iRet = tbus_channel_send(pstEnv->astThread[2].pstChannel0, 
						stData.pszBuff, stData.iBuff, 0);
				}
							
				pstEnv->iLastSrcID = -1;
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
	
	return iBusy;	
}

static int channel2bus2torm(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv,
	                                     const char * pszMsg, int iMsgLen)
{

	TDRDATA stData;
	TORMPKG stPkg;
	int iRet = 0;
	int iProtoVer = pstEnv->iAccountDBProtoVer;
	int iSrc, iDst;
	
	stData.pszBuff = (char *)pszMsg;
	stData.iBuff = iMsgLen;
	iRet =  torm_attach_res_msg(pstEnv->hTorm, &stData, &stPkg, iProtoVer);
	if (0 >iRet)
	{
		return -1;
	}

	// 发送数据给torm
	switch(stPkg.Head.HeadCmd.Cmd)
	{
	case TORM_EXGVER_REQ:
	case TORM_INSERT_REQ:
	case TORM_UPDATE_REQ:		
	case TORM_DELETE_REQ:
	case TORM_SELECT_REQ:
	case TORM_SVRINFO_REQ:
	case TORM_SELECT_COUNT_REQ:
		iSrc = pstAppCtx->iId;
		iDst = pstEnv->iAccountDBID;
		if(tbus_forward(pstAppCtx->iBus, &iSrc, &iDst, pszMsg, iMsgLen, 0) < 0)
		{
			MutiLog(MUTI_LOGLEVEL_ERROR,	"tbus_forward auth_svr fail.");
		}
		break;
	default:
		iRet = -1;
		break;
	}
	
	return iRet;
}


// 次函数为测试充值送礼     乱搞的..
int test_propgift(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv)
{
	int iRet = 0;
//#if 0
	SSPKG stPkg;
	static int iQue = 0;
	int iSrc, iDst;
	TDRDATA stNet;
	
	
	stPkg.Head.Cmd = CASH_BILL_REQ;
	stPkg.Head.HeadLen = sizeof(SSPKGHEAD);
	stPkg.Head.Magic = PROTOCOL_MAGIC;
	stPkg.Head.Ver = TDR_METALIB_PROTO_SS_VERSION;
	
	iQue ++;
	iQue = iQue%0x7fffffff;
	stPkg.Body.CashBillReq.Amount = 0;
	stPkg.Body.CashBillReq.Points = 100;
	STRNCPY(stPkg.Body.CashBillReq.Pass9Account, "jerry", sizeof(stPkg.Body.CashBillReq.Pass9Account));
	stPkg.Body.CashBillReq.Sequence = iQue;

	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = pstEnv->szSendBuffer;
	if(ss_pack(pstEnv->pstSSHan, &stNet, &stPkg, 0) < 0)
	{
		return -1;
	}
	
	iSrc = pstAppCtx->iId;
	iDst=pstEnv->iRegionGateID;
	
	iRet = tbus_send(pstAppCtx->iBus, &iSrc, &iDst, stNet.pszBuff, stNet.iBuff, 0);
//#endif
	return iRet;
}

int channel2bus_one(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv, ThreadMgr *pstThread)
{
	int iLen;
	int iRet;
	char szMsg[MAX_MSG_SIZE];
	int iSrc, iDst;
	int iBusy = 0;

	iLen = sizeof(szMsg);
	iRet = tbus_channel_recv(pstThread->pstChannel0, szMsg, &iLen, 0);

	//test_propgift(pstAppCtx,pstEnv);
	
	if (0 == iRet)
	{
		SSPKG stPkg;
		
		MutiLog(MUTI_LOGLEVEL_INFO,  "channel2bus_one thread id:%d,recv iLen:%u ",pstThread->iIdx,iLen);
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
		
		// 主动发给 auth的数据包
		if(ss_unpack(pstEnv->pstSSHan,  &szMsg[sizeof(ChannelAppend)], iLen-sizeof(ChannelAppend), &stPkg, 0) >= 0)
		{
			int iSendType = 0;
			if( stPkg.Head.Cmd == UPDATE_ACCOUNT_REP)
			{
				iSendType = 1;
			}

			if( stPkg.Head.Cmd == CASH_BILL_REQ ||
				stPkg.Head.Cmd == QUERY_BILL_REQ )
			{
				iSendType = 2;
			}
			if (stPkg.Head.Cmd == INFORMER_GET_ACCOUNT_RES)
			{
				int iAuthFlag = 0;
				if (0 == ifm_peek_auth_flag(stPkg.Body.GetAccountRes.ExtraData,stPkg.Body.GetAccountRes.ExtraDataLen,&iAuthFlag) )
				{
					if (iAuthFlag & AUTH_FLAG_DEQUICK)
					{
						iSendType = 3;
					}
					else
					{
						iSendType = 1;
					}
				}
			}
			
			if(iSendType == 1)
			{
				
				// 发给三个auth服务器数据
				MutiLog(MUTI_LOGLEVEL_INFO, "ss_sendto_auth_bus_pkg cmd:%d ", stPkg.Head.Cmd);
				ss_sendto_auth_bus_pkg(pstAppCtx,pstEnv,&szMsg[sizeof(ChannelAppend)], iLen-sizeof(ChannelAppend));		
				return iBusy;
			}
			else if (iSendType == 2)
			{
				ss_sendto_region_bus_pkg(pstAppCtx,pstEnv,&szMsg[sizeof(ChannelAppend)], iLen-sizeof(ChannelAppend),&stPkg);		
				return iBusy;
			}
			else if (iSendType == 3)
			{
				iSrc = pstAppCtx->iId;
				iDst= 0;
				if(tbus_backward(pstAppCtx->iBus, &iSrc, &iDst, &szMsg[sizeof(ChannelAppend)], iLen-sizeof(ChannelAppend), 0) < 0)
				{
					MutiLog(MUTI_LOGLEVEL_ERROR,	"ss_sendto_region_bus_pkg  fail.");
				}
				return iBusy;
			}
		}
		// 主动发给torm的数据包
		if( 0 == channel2bus2torm( pstAppCtx,pstEnv,&szMsg[sizeof(ChannelAppend)], iLen-sizeof(ChannelAppend) ) )
		{
			return iBusy;
		}
		
		// 正常的回包信息,restore保证了iDst的存在
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
}

int proc_multi_channel2bus(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv)
{
	int i;
	int iBusy = 0;

	for (i=0; i<pstEnv->iThread; i++)
	{
		iBusy = channel2bus_one(pstAppCtx, pstEnv, &pstEnv->astThread[i]);
	}

	return iBusy;
}

int informer_svr_proc(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv)
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
	{
		return 0;
	}
	else
	{
		return -1;
	}

	return -1;
}

int informer_svr_reload(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv)
{
	
	INFORMER_SVRCONF   *pstPreConf;
	UNUSED(pstEnv);
	
	pstPreConf = (INFORMER_SVRCONF   *)pstAppCtx->stConfPrepareData.pszBuff;

	pstEnv->pstConf->AuthMode = pstPreConf->AuthMode;
	
	printf("informer_svr reload\n");
	MutiLog(MUTI_LOGLEVEL_INFO,  "informer_svr reload");

	return 0;
}

int channel2bus_one_recvall(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv, ThreadMgr* pstThread)
{
	while(channel2bus_one(pstAppCtx, pstEnv, pstThread))
	{
		;
	}

	return 0;
}

int informer_svr_fini(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv)
{
	int i = 0;
	//void* pvRet = NULL;
	ThreadMgr* pstThread  = NULL;
	int iCount;
	assert(pstAppCtx);

	for(i=0; i<pstEnv->iThread; i++)
	{
		pstThread	=	&pstEnv->astThread[i];

		if( pstThread->cIsValid )
		{
			/* 主进程退出 设置标志位 */
			ifm_set_process_exit_flag(pstEnv, 0);
		}
	}
	
	for(i=0; i<pstEnv->iThread; i++)
	{
		pstThread	=	&pstEnv->astThread[i];

		if( pstThread->cIsValid )
		{
			channel2bus_one_recvall(pstAppCtx, pstEnv, pstThread);	
			//pthread_join(pstThread->tid, &pvRet);

			iCount = 0;
			while(iCount < 100 )
			{
				if(	0 != ifm_get_process_exit_flag(pstEnv,pstThread->iIdx+1) )
				{
					break;	
				}
				iCount++;
				usleep(10000);
			}
			channel2bus_one_recvall(pstAppCtx, pstEnv, pstThread);	
		}
	}

	for (i=0; i<pstEnv->iThread; i++)
	{
		pstThread	=	&pstEnv->astThread[i];
		tbus_close_channel(&pstThread->pstChannel0);
		tbus_close_channel(&pstThread->pstChannel1);
	}

	printf("informer_svr stop\n");
	MutiLog(MUTI_LOGLEVEL_INFO, "informer_svr stop");

	//pthread_mutex_destroy(&pstEnv->stLogMutex);
	filelock_close();

	shmdt(pstEnv->piProExitFlag);
	return 0;
}

int informer_svr_tick(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv)
{
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	return 0;
}

int informer_svr_stop(TAPPCTX *pstAppCtx, INFORMER_SVRENV *pstEnv)
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

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)informer_svr_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_informer_svr;
	gs_stAppCtx.stConfData.pszMetaName = "informer_svrconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);
		
	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		//printf("Error: app Initialization failed.\n");
		printf("process(%d)  exit\n",getpid());
		return iRet;
	}

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)informer_svr_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)informer_svr_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)informer_svr_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)informer_svr_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)informer_svr_stop;
	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	tapp_def_fini(&gs_stAppCtx, pvArg);
	
	return iRet;
}
