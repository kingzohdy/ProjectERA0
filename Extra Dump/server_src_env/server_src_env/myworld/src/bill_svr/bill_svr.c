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
#include "bill_svr_conf_desc.h"
#include "version.h"
#include "tbus/tbus.h"
#include "comm_misc.h"
#include "entity.h"
#include "tormapi/torm_protocol_api.h"
#include "../zone/zone_svr/role_db_meta.h"
#include "comm/shtable.h"

#define REMOVE_HASH_NODE_TIMEOUT   8 
#define OVERDUE_HASH_NODE_TIMEOUT  5 
// »ñÈ¡Ò»¸öÊý×éµÄ³ÉÔ±ÊýÁ¿
#define CCH( arr ) (sizeof(arr)/sizeof(arr[0]))
unsigned int g_uiBillLockScanNum;
unsigned int g_uiCDKeyScanNum;

#define MAX_MSG 64000*2
#define UNUSED( param ) ((void)(param))

//³åÖµËÍ»ý·Ö
#define QUERY_PROPSGIFT_BY_INFORMER     1
//ÓÃ»§²éÑ¯×Ô¼ºµÄ»ý·ÖÁìÈ¡Çé¿ö
#define QUERY_PROPSGIFT_BY_USER_QUERY     2
//ÓÃ»§ÁìÈ¡×Ô¼ºµÄ»ý·Ö
#define QUERY_PROPSGIFT_BY_USER_SET     3
//9c³åÖµËÍÀñcs½Ó¿Ú
#define CASH_PROPSGIFT_BY_CS    4

// ½ÇÉ«µÇÂ¼²éÑ¯½ÇÉ«ÕËºÅÀñ°üÐÅÏ¢ 
#define QUERY_ACC_GIFT_INFO 5
#define INSERT_ACC_GIFT_INFO 6



enum  DB_BILL_CMD{
	DB_SELECT_BILL_CASH,
	DB_SELECT_BILL_QUERY,
	DB_SELECT_PROPS,
	DB_UPDATE_BILL,
	DB_UPDATE_PROPS,
	DB_INSERT_BILL_CASH,
	DB_INSERT_PROPS,
	DB_DELETE_BILL,
	DB_DELETE_PROPS,
	DB_SELECT_BILL_DEC,
	DB_UPDATE_BILL_DEC,
	DB_SELECT_BILL_CASH_RECORD,
	DB_INSERT_BILL_CASH_RECORD,
	DB_SELECT_USER_QUERY_BILL,
	DB_INSERT_BILL_DEC_RECORD,
	DB_SELECT_USER_FREEZE_BILL,
	DB_UPDATE_USER_FREEZE_BILL,
	DB_INSERT_BILL_ADD,
	DB_SELECT_COUNT_PROPSDEF,
	DB_SELECT_PROPSDEF,
	DB_SELECT_PROPSGIFT,
	DB_UPDATE_PROPSGIFT,

	DB_SELECT_ACCGIFT,
	DB_UPDATE_ACCGIFT,
	DB_INSERT_PROPSGIFT,

	DB_SELECT_PROPGIFT_CLT,
	DB_UPDATE_PROPGIFT_CLT,

	DB_SELECT_CDKEYTYPEDEF,
	DB_SELECT_CDKEYDEF_COUNT,
	DB_SELECT_CDKEYDEF,
	DB_UPDATE_CDKEY_DEF,
	DB_SELECT_ACCOUNT_UIN,
	DB_INSERT_GODCOIN_CONSUME_RECORD,
	DB_SELECT_CDKEYDEF_GET,
	DB_INSERT_CDKEY_GET,
	DB_INSERT_ACCGIFT,
};
// ²»×ö¹²ÏíÄÚ´æµÄ´¦Àí,Ö»Åª¶¯Ì¬ÄÚ´æ.
// ²»¿¼ÂÇcoredumpµÄÇé¿ö(ÎÈ¶¨ºóÕâ²¿·ÖºÜÉÙÐÞ¸Ä)
/* Çó¹º¼ÇÂ¼±í */
struct tagBillLock
{
	char Pass9Account[ACCOUNT_LOGIN_NAME_LEN];
    	unsigned int StartTime;                       	/*   ¹Òµ¥µÄ¿ªÊ¼Ê±¼ä */
};
struct tagBillLock;
typedef struct tagBillLock                         	BILLLOCK;
typedef struct tagBillLock                         	*LPTBILLLOCK;

// Í¬Ê±Ö§³Ö3W ÈËµÄ²Ù×÷,Ã¿ÃëÉÔÃè1Ç§¸ö½Úµã,30ÃëÉ¨Íê
#define MAX_BILLHASH_BUCKET   30000
typedef struct tagBillHashMemory
{
	char sMem[SHT_SIZE(MAX_BILLHASH_BUCKET, MAX_BILLHASH_BUCKET, sizeof(BILLLOCK))];
}BillHashMemory;

struct tagCDKeyLock
{
	char CDKey[CDKEY_LEN];
	unsigned int StartTime;   
};
typedef struct tagCDKeyLock CDKEYLOCK;
#define MAX_CDKEY_BUCKET 10000 // 10000Ó¦¸Ã×ã¹»ÁË
typedef struct tagCDKeyHashMem
{
	char sMem[SHT_SIZE(MAX_CDKEY_BUCKET, MAX_CDKEY_BUCKET, sizeof(CDKEYLOCK))];
}CDKeyHashMem;


//»ñÈ¡×îÐÂ10ÌõµÄ ÖØÖµËÍÀñ°üµÄÅäÖÃ 
//#define NEW_PROPSDEF_RECORD_MAX   10
#define PROPOS_DEF_DATA 2048
char sWatchPropsDef[PROPOS_DEF_DATA - sizeof(PROPSDEF)];

struct tagbill_svrEnv
{
	BILL_SVRCONF *pstConf;
	BILL_SVRRUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;

	TAPPCTX *pstAppCtx;
	int iRegionID;
	int iBillDBID;
	int iAccountDBID;
	LPSSHANDLE pstSSHan;
	TORMPROTOCOLHANDLE hTorm;//Torm²Ù×÷µÄ¾ä±ú
	int iDBProtoVer;
	int iTormSeq;
	char szRecvBuffer[MAX_MSG];
	char szSendBuffer[MAX_MSG];

	LPSHTABLE pstBillHash;
	BillHashMemory stBillHashMemory;

	LPSHTABLE pstCDKeyHash;
	CDKeyHashMem stCDKeyHashMem;


	//³åÖµÊ±¼ä¶Ï»ñÈ¡Àñ°ü
	time_t tPropsDef;
	int iPropsDef;
	char sPropsDef[NEW_PROPSDEF_RECORD_MAX][PROPOS_DEF_DATA];

	int iDefNum;
	PROPSDEF stPropsDef[NEW_PROPSDEF_RECORD_MAX];

};

typedef struct tagbill_svrEnv		BILL_SVRENV;
typedef struct tagbill_svrEnv		*LBILL_SVRENV;


static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_bill_svr[];
extern unsigned char g_szMetalib_proto_ss[];

extern int trl_comp_func(const void* pv1, const void* pv2);

BILL_SVRENV gs_stEnv;
static BILL_SVRRUN_STATUS gs_stStat;

int bill_ss_backward_pkg(BILL_SVRENV *pstEnv,SSPKG *pstPkg, int iCmd,char *Pass9Account);
int bill_db_select_req(BILL_SVRENV *pstEnv,void *pstData,char *szCallBackBuffer,int iLen,int iFlag);
int bill_db_insert_res_dec(BILL_SVRENV *pstEnv,TORMPKG *pstPkg);
int bill_db_select_count_propsdef_req(BILL_SVRENV *pstEnv);
int bill_db_update_req(BILL_SVRENV *pstEnv,void *pstData,char *szCallBackBuffer,int iLen,int iFlag);
int bill_db_select_req_accgift(BILL_SVRENV *pstEnv,char *pstData,int iLen,int Uin,int OperType);
int bill_db_update_res_accgift(BILL_SVRENV *pstEnv,TORMPKG *pstPkg);
int bill_db_insert_req(BILL_SVRENV *pstEnv,void *pstData,char *szCallBackBuffer,int iLen,int iFlag);
int bill_db_select_req_propgift_clt(BILL_SVRENV *pstEnv,char *pstData,int iLen,int Uin);
int bill_db_update_res_propsgift_clt(BILL_SVRENV *pstEnv,TORMPKG *pstPkg);
int bill_propsgift_clt(BILL_SVRENV *pstEnv,SSPROPGIFTLISTREQ *pstReq,int iRetCode);
int bill_db_propsgift2region(BILL_SVRENV *pstEnv, PROPSGIFT  *pstPropsGift, const char* pszPass9);
int bill_db_select_cdkey_type(BILL_SVRENV *pstEnv,char *pstData,int iLen);
int bill_ss_backward_pkg_cdkey(BILL_SVRENV *pstEnv,	int iCmd, int iRet, 
									const char* pszCDKey,	CDKEYTYPEDEF *pstDef, const char* pszRoleName,int iUnLock);
int bill_db_update_res_cdkey_def(BILL_SVRENV *pstEnv,TORMPKG *pstPkg);
int bill_process_accountdb_req(BILL_SVRENV *pstEnv, char *Pass9Account,char *szCallBackBuffer,int iLen );
int bill_props_gift_add_cash_to_region(BILL_SVRENV *pstEnv,SSCASHPROPSGIFTINFOREQ* pstReq);
int bill_db_insert_res_cdkey_get(BILL_SVRENV *pstEnv,TORMPKG *pstPkg);
int bill_db_insert_res_acc_gift(BILL_SVRENV *pstEnv,TORMPKG *pstOrmPkg);

static  int hash_cmp_string(const void * pv1 , const void *pv2)
{
	BILLLOCK * pszBucket1 = (BILLLOCK * )pv1;
	BILLLOCK * pszBucket2 = (BILLLOCK * )pv2;

	return strcmp(pszBucket1->Pass9Account, pszBucket2->Pass9Account);
}
static unsigned int hash_code_string(const void* pv)
{
	BILLLOCK *psBillLock = (BILLLOCK *)pv;
	
	return sht_get_code(psBillLock->Pass9Account);
}

static  int hash_cmp_string_cdkey(const void * pv1 , const void *pv2)
{
	CDKEYLOCK * pszBucket1 = (CDKEYLOCK * )pv1;
	CDKEYLOCK * pszBucket2 = (CDKEYLOCK * )pv2;

	return strcmp(pszBucket1->CDKey, pszBucket2->CDKey);
}

static unsigned int hash_code_string_cdkey(const void* pv)
{
	CDKEYLOCK *psLock = (CDKEYLOCK *)pv;
	
	return sht_get_code(psLock->CDKey);
}

CDKEYLOCK *z_find_cdkey_lock(BILL_SVRENV* pstEnv, const char*pszCdKey)
{
	CDKEYLOCK *pstPos = NULL;
	CDKEYLOCK stLock;

	memset(&stLock,0,sizeof(stLock));
	STRNCPY(stLock.CDKey, pszCdKey, CCH(stLock.CDKey));
	pstPos = (CDKEYLOCK *)sht_find(pstEnv->pstCDKeyHash, &stLock, hash_cmp_string_cdkey, hash_code_string_cdkey);
	return pstPos;
}

void* z_remove_cdkey_lock(BILL_SVRENV* pstEnv, const char*pszCdKey)
{
	CDKEYLOCK *pstLock;
	CDKEYLOCK stLock;

	memset(&stLock,0,sizeof(stLock));
	STRNCPY(stLock.CDKey, pszCdKey, CCH(stLock.CDKey));

	pstLock = sht_remove(pstEnv->pstCDKeyHash, &stLock, hash_cmp_string_cdkey, hash_code_string_cdkey);
	if( pstLock != NULL  )
	{
		memset(pstLock,0,sizeof(CDKEYLOCK));
	}
	return pstLock;
}

int  z_insert_cdkey_lock(BILL_SVRENV *pstEnv, const char*pszCdKey)
{
	CDKEYLOCK stDummy;
	CDKEYLOCK *pstPos;
	CDKEYLOCK *pstOld;
	CDKEYLOCK stCdkeyLock;

	memset(&stCdkeyLock,0,sizeof(stCdkeyLock));
	STRNCPY(stCdkeyLock.CDKey, pszCdKey, CCH(stCdkeyLock.CDKey));
	stCdkeyLock.StartTime = time(NULL);
	
	pstOld = z_find_cdkey_lock(pstEnv, pszCdKey);
	if (NULL != pstOld)
	{
		memcpy( pstOld, &stCdkeyLock,sizeof(CDKEYLOCK));	
	}
	else
	{
		memset(&stDummy,0,sizeof(stDummy));	
		STRNCPY(stDummy.CDKey, pszCdKey, CCH(stDummy.CDKey));
		pstPos = (CDKEYLOCK *)sht_insert_unique(pstEnv->pstCDKeyHash, &stDummy, hash_cmp_string_cdkey,
			                        hash_code_string_cdkey);
		if(pstPos)
		{
			memcpy( pstPos, &stCdkeyLock,sizeof(CDKEYLOCK));
		}
	}
	return 0;
}



BILLLOCK * z_find_bill_lock( BILL_SVRENV* pstEnv, char *Pass9Account)
{
	BILLLOCK *pstPos = NULL;
	BILLLOCK  stDummy;
	memset(&stDummy,0,sizeof(stDummy));
	STRNCPY(stDummy.Pass9Account, Pass9Account, CCH(stDummy.Pass9Account));
	pstPos = (BILLLOCK *)sht_find(pstEnv->pstBillHash, &stDummy, hash_cmp_string, hash_code_string);

	return  pstPos;
}
void* z_remove_bill_lock(BILL_SVRENV* pstEnv, char *Pass9Account)
{
	BILLLOCK *pstBillLock;
	BILLLOCK  stDummy;
	memset(&stDummy,0,sizeof(stDummy));
	STRNCPY(stDummy.Pass9Account, Pass9Account, CCH(stDummy.Pass9Account));
	
	pstBillLock = sht_remove(pstEnv->pstBillHash, &stDummy, hash_cmp_string, hash_code_string);
	if( pstBillLock != NULL  )
	{
		// Çå¿ÕÏÂÊý¾Ý
		memset(pstBillLock,0,sizeof(BILLLOCK));
	}
	
	return pstBillLock;
}


int  z_insert_bill_lock(BILL_SVRENV *pstEnv,char * Pass9Account)
{
	BILLLOCK stDummy;
	BILLLOCK *pstPos;
	BILLLOCK *pBillLockOld;

	BILLLOCK stBillLock;
	memset(&stBillLock,0,sizeof(stBillLock));
	STRNCPY(stBillLock.Pass9Account, Pass9Account, CCH(stBillLock.Pass9Account));
	stBillLock.StartTime = time(NULL);
	
	// ÓÐÀÏµÄÊý¾Ý,¾ÍÖ±½Ó¿½±´¸øËû
	pBillLockOld = z_find_bill_lock(pstEnv,stBillLock.Pass9Account);
	if( NULL!= pBillLockOld )
	{
		/* ÐÞ¸ÄµÄÊý¾Ý */
		memcpy( pBillLockOld, &stBillLock,sizeof(BILLLOCK));		
	}
	else
	{
		memset(&stDummy,0,sizeof(stDummy));	
		STRNCPY(stDummy.Pass9Account, stBillLock.Pass9Account, CCH(stDummy.Pass9Account));
		pstPos = (BILLLOCK *)sht_insert_unique(pstEnv->pstBillHash, &stDummy, hash_cmp_string,
			                        hash_code_string);
		if(pstPos)
		{
			memcpy( pstPos, &stBillLock,sizeof(BILLLOCK));
		}
	}

	return 0;
}


int z_bill_cdkey_lock_tick(BILL_SVRENV* pstEnv,unsigned int uiPreScanNum)
{
	unsigned int i =0;
	int iValid = 0;
	CDKEYLOCK *pstCDkeyLock;
	time_t    tCurr = time(NULL);

	for (i=0; i<uiPreScanNum; i++)
	{
		g_uiCDKeyScanNum = g_uiCDKeyScanNum%pstEnv->pstCDKeyHash->iMax;
		
		pstCDkeyLock = (CDKEYLOCK *)sht_pos(pstEnv->pstCDKeyHash, g_uiCDKeyScanNum, &iValid);
		g_uiCDKeyScanNum++;
		if (NULL == pstCDkeyLock || !iValid )
		{
			continue;
		}

		// ÏÂÃæÊÇÒª»ØÐ´µÄÊý¾Ý
		if( tCurr - pstCDkeyLock->StartTime > REMOVE_HASH_NODE_TIMEOUT)
		{
			z_remove_cdkey_lock(pstEnv, pstCDkeyLock->CDKey);
		}		
	}
	return 0;
}

int z_bill_lock_tick(BILL_SVRENV* pstEnv,unsigned int uiPreScanNum)
{
	unsigned int i =0;
	int iValid = 0;
	BILLLOCK *pstBillLock;
	time_t    tCurr;

	tCurr = time(NULL);
	for (i=0; i<uiPreScanNum; i++)
	{		
		g_uiBillLockScanNum = g_uiBillLockScanNum%pstEnv->pstBillHash->iMax;
		
		pstBillLock = (BILLLOCK *)sht_pos(pstEnv->pstBillHash, g_uiBillLockScanNum, &iValid);
		if (NULL == pstBillLock || !iValid )
		{
			g_uiBillLockScanNum++;
			continue;
		}

		g_uiBillLockScanNum++;

		// ÏÂÃæÊÇÒª»ØÐ´µÄÊý¾Ý
		if( tCurr - pstBillLock->StartTime > REMOVE_HASH_NODE_TIMEOUT)
		{
			z_remove_bill_lock(pstEnv, pstBillLock->Pass9Account);
		}		
	}

	return 0;
}

int bill_svr_init(TAPPCTX *pstAppCtx, BILL_SVRENV *pstEnv)
{
	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		return -1;
	}
	pstEnv->pstConf = (BILL_SVRCONF *)pstAppCtx->stConfData.pszBuff;

	pstEnv->pstAppCtx = pstAppCtx;
	pstEnv->iTormSeq = 0;
	pstEnv->iDBProtoVer = TDR_METALIB_ROLE_DB_META_VERSION;
	pstEnv->pstRunStat = (BILL_SVRRUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
	pstEnv->iPropsDef = 0;
	if (NULL == pstEnv->pstRunStat)
	{
		pstEnv->pstRunStat = &gs_stStat;
	}

	if(ss_han_create(g_szMetalib_proto_ss, &pstEnv->pstSSHan) < 0)
	{
		return -1;
	}

	/* Ö»ºÍregionÍ¨ÐÅ */
	GET_REGION_ENTITY_ID(pstEnv->iRegionID, pstEnv->pstAppCtx->iId, FUNC_REGION_GATE);
	GET_REGION_ENTITY_ID(pstEnv->iBillDBID, pstEnv->pstAppCtx->iId, FUNC_BILL_DB);
	GET_REGION_ENTITY_ID(pstEnv->iAccountDBID, pstEnv->pstAppCtx->iId, FUNC_ACCOUNT_DB);
	
	/*if (0 > tconnapi_initialize(pstAppCtx->pszGCIMKey, pstAppCtx->iBusinessID))
	{
		printf("tbus gcim key %s init fail\n", pstAppCtx->pszGCIMKey);
		return -1;
	}*/

	if (0 > torm_create_handle_from_dir(&pstEnv->hTorm, pstEnv->pstConf->DBMetaDir))
	{
		printf("torm_create_handle fail\n");
		return -1;
	}
		
	if (0 > tapp_get_category(NULL, (void *)(&pstEnv->pstLogCat)))	
	{		
		printf("tapp_get_category run fail\n");
		return -1;	
	}	

	g_uiCDKeyScanNum = g_uiBillLockScanNum = 0;
	pstEnv->pstBillHash= sht_init(pstEnv->stBillHashMemory.sMem, 
	                             			sizeof(pstEnv->stBillHashMemory.sMem),
								MAX_BILLHASH_BUCKET, 
								MAX_BILLHASH_BUCKET, 
								sizeof(BILLLOCK));
	if(!pstEnv->pstBillHash)
	{
		printf("bill_svr_init BillHash fail \n");
		return -1;
	}

	pstEnv->pstCDKeyHash = sht_init(pstEnv->stCDKeyHashMem.sMem, 
	                             			sizeof(pstEnv->stCDKeyHashMem.sMem),
								MAX_CDKEY_BUCKET, 
								MAX_CDKEY_BUCKET, 
								sizeof(CDKEYLOCK));
	
	if (NULL == pstEnv->pstCDKeyHash)
	{
		printf("bill_svr_init CDKeyHash fail \n");
		return -1;
	}

	//°Ñ³åÖµËÍÀñ°üµÄÅäÖÃÅª³öÀ´
	bill_db_select_count_propsdef_req(pstEnv);
	
	printf("bill_svr start\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "bill_svr start");

	return 0;
}
static void bill_db_init_torm_head(	TAPPCTX *pstAppCtx, 
	                      					BILL_SVRENV *pstEnv,
                              				  	TORMPKGHEAD * pstHead,
                              				  	int iCmd, 
                              				  	int iCallBackCmd,
	                       					const char *pszMetaName, 
	                      					const char * pstCallBackData, 
	                      					int iContextLen)
{
	pstHead->HeadComm.InstID = pstAppCtx->iId;
	pstHead->HeadComm.Magic = TORM_MAGIC;
       pstHead->HeadCmd.Cmd = iCmd;
	
	if (pszMetaName)
	{
	       //Èç¹ûÓÐÔªµÄÃû×Ö °ÑÔªµÄÃû×Ö¸³Öµ
		STRNCPY(pstHead->HeadCmd.MetaNameVer.MetaName, pszMetaName, 
			      sizeof(pstHead->HeadCmd.MetaNameVer.MetaName));
	}
	pstHead->HeadCmd.MetaNameVer.MetaVer = 0;
	
	//×öÒ»¸ö·¢°üµÄË³ÐòµÝÔö
	pstEnv->iTormSeq++;
	if (pstEnv->iTormSeq <= 0)
	{
		pstEnv->iTormSeq = 1;
	}
	pstHead->HeadApp.AppCmd = 0;
	pstHead->HeadApp.Seq = pstEnv->iTormSeq;
	pstHead->HeadApp.ObjID = 0;//¶à½ø³ÌÕâÀï±íÊ¾ÓÃ½ø³ÌidÀ´±íÊ¾

	//°Ñ³¤¶ÈºÍÄÚÈÝ¶¼¼ÇÂ¼ÏÂ
	pstHead->AsynCallBackDataLen = 0;
	memcpy(pstHead->AsynCallBackData +pstHead->AsynCallBackDataLen,&iCallBackCmd, sizeof(iCallBackCmd));
	pstHead->AsynCallBackDataLen += sizeof(iCallBackCmd);
	if(pstCallBackData != NULL)
	{
		if(iContextLen > 0)
		{
			memcpy(pstHead->AsynCallBackData +pstHead->AsynCallBackDataLen,pstCallBackData, iContextLen);
			pstHead->AsynCallBackDataLen +=iContextLen;
		}
	}
}
static int bill_db_torm_send(TAPPCTX* pstAppCtx, BILL_SVRENV* pstEnv,TORMPKG *pstPkg)
{
	TDRDATA stData;
	int iProtoVer;
	int iSrc = 0;
	int iDst = 0;

	iProtoVer = pstEnv->iDBProtoVer;
	stData.pszBuff = pstEnv->szSendBuffer;
	stData.iBuff = sizeof(pstEnv->szSendBuffer);
	if(0 != torm_pack_msg(pstEnv->hTorm, &stData, pstPkg, iProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_pack_msg error");
		return -1;
	}
	
	iSrc = pstAppCtx->iId;
	iDst = pstEnv->iBillDBID;
	if(tbus_forward(pstAppCtx->iBus, &iSrc, &iDst, stData.pszBuff, stData.iBuff, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,"tbus_forward auth_svr fail.");
	}
	
	return 0;
}
static int bill_db_send_req(TAPPCTX *pstAppCtx, BILL_SVRENV *pstEnv,
	                     TORMPKG *pstTormPkg, const void *pvData, int iDataLen)
{
	int iRet = 0;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pvData;
	stMetaData.iBuff = iDataLen;

	iRet =  torm_make_req_msg(pstEnv->hTorm, &stNetData, pstTormPkg, &stMetaData, pstEnv->iDBProtoVer);
	if(0 !=iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail %s",torm_error_string(iRet));
		return -1;
	}
	
	if (0 > bill_db_torm_send(pstAppCtx, pstEnv,pstTormPkg))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "bill_db_torm_send err");
		return -1;
	}

	return 0;	
}

PROPSDEF * bill_get_propsdef(BILL_SVRENV* pstEnv,unsigned int GroupID)
{
	int i = 0;
	PROPSDEF *pstPropsDef = NULL;
		
	for ( i=0; i<pstEnv->iPropsDef && i<NEW_PROPSDEF_RECORD_MAX; i++ )
	{
		pstPropsDef = (PROPSDEF *)pstEnv->sPropsDef[i];
		if(pstPropsDef->GroupID == GroupID)
		{
			return pstPropsDef;
		}
	}
		
	return NULL;
}

PROPSDEF * bill_db_props_check_date(BILL_SVRENV* pstEnv)
{
	int i = 0;
	time_t tTime;
	PROPSDEF *pstPropsDef = NULL;
	char szStartDate[DB_TIME_LEN];
	char szEndDate[DB_TIME_LEN];
	time_t tStartDate;
	time_t tEndDate;

	tTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		
	for ( i=0; i<pstEnv->iPropsDef && i<NEW_PROPSDEF_RECORD_MAX; i++ )
	{
		pstPropsDef = (PROPSDEF *)pstEnv->sPropsDef[i];
		
		memset(szStartDate,0,sizeof(szStartDate));
		memset(szEndDate,0,sizeof(szEndDate)); 

		snprintf(szStartDate,sizeof(szStartDate),"%s000000",
			pstPropsDef->BeginDate);
		tStartDate = string2time(szStartDate);
		if( tStartDate <= 0 && i==0)
		{
			// Ä¬ÈÏÈç¹ûÃ»ÓÐÌîÈÕÆÚ
			return NULL;
		}
		else if(tStartDate <= 0)
		{
			break;
		}
		
		snprintf(szEndDate,sizeof(szEndDate),"%s000000",
			pstPropsDef->EndDate);
		tEndDate = string2time(szEndDate);
		if( tEndDate <= 0)
		{
			break;
		}
		// °ÑÈÕÆÚ¼ÓÒ»Ìì´¦Àí,±íÌî 20110302-20110302±íÊ¾Ò»Ìì
		tEndDate = tEndDate + 24*60*60;
              // ÔÚÕâ¸öÈÕÆÚÀï,ÔÙ¿´¿´ÊÇ²»ÊÇÕâ¸öÊ±¼ä¶ÎÀï
		if( tTime >=tStartDate && tTime < tEndDate )
		{
			return pstPropsDef;
		}
	}
			
	return NULL;
}


int bill_db_get_curr_propsdef(BILL_SVRENV* pstEnv)
{
	int i = 0;
	time_t tTime;
	PROPSDEF *pstPropsDef = NULL;
	char szStartDate[DB_TIME_LEN+6];
	char szEndDate[DB_TIME_LEN+6];
	time_t tStartDate;
	time_t tEndDate;
	
	pstEnv->iDefNum = 0;
	tTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		
	for ( i=0; i<pstEnv->iPropsDef && i<NEW_PROPSDEF_RECORD_MAX; i++ )
	{
		pstPropsDef = (PROPSDEF *)pstEnv->sPropsDef[i];
		
		memset(szStartDate,0,sizeof(szStartDate));
		memset(szEndDate,0,sizeof(szEndDate)); 

		snprintf(szStartDate,sizeof(szStartDate),"%s000000",
			pstPropsDef->BeginDate);
		tStartDate = string2time(szStartDate);
		if( tStartDate <= 0 && i==0)
		{
			return 0;
		}
		else if(tStartDate <= 0)
		{
			return 0;
		}
		
		snprintf(szEndDate,sizeof(szEndDate),"%s000000",
			pstPropsDef->EndDate);
		tEndDate = string2time(szEndDate);
		if( tEndDate <= 0)
		{
			return 0;
		}
		// °ÑÈÕÆÚ¼ÓÒ»Ìì´¦Àí,±íÌî 20110302-20110302±íÊ¾Ò»Ìì
		tEndDate = tEndDate + 24*60*60;
              // ÔÚÕâ¸öÈÕÆÚÀï,ÔÙ¿´¿´ÊÇ²»ÊÇÕâ¸öÊ±¼ä¶ÎÀï
		if( tTime >=tStartDate && tTime < tEndDate )
		{
			//return pstPropsDef;
			pstEnv->stPropsDef[pstEnv->iDefNum++] = *pstPropsDef;
		}
	}
			
	return 0;
}

PROPSDEF *bill_db_get_propsdef(BILL_SVRENV* pstEnv, unsigned int GroupID)
{
	int i;

	for(i=0; i<pstEnv->iDefNum; i++)
	{
		if (pstEnv->stPropsDef[i].GroupID == GroupID)
		{
			return &pstEnv->stPropsDef[i];
		}
	}

	return NULL;
}

//³åÖµ»ý·ÖËÍÀñ°ü¶¨Òå
int bill_db_select_count_propsdef_req(BILL_SVRENV *pstEnv)
{
	TORMPKG stOrmPkg;
	TORMSELECTCOUNTREQ *pstSelectCountReq = &stOrmPkg.Body.SelectCountReq;
	PROPSDEF stPropsDef;

	pstEnv->iPropsDef = 0;
	memset(&stPropsDef,0,sizeof(stPropsDef));

	bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stOrmPkg.Head, TORM_SELECT_COUNT_REQ,DB_SELECT_COUNT_PROPSDEF, 
				               "PropsDef", NULL, 0);
		
	pstSelectCountReq->WhereDef[0] = 0;
	snprintf(pstSelectCountReq->WhereDef,
	         sizeof(pstSelectCountReq->WhereDef),
	         "where now() between DATE_SUB(CAST(BeginDate as datetime),INTERVAL 1 month) and  DATE_ADD(CAST(EndDate as datetime), INTERVAL 1 month)");
			
	return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stOrmPkg,&stPropsDef, sizeof(PROPSDEF));

}
int bill_db_select_count_propsdef_res(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
	TORMSELECTCOUNTRES *pstSelectCountRes;
	int iRecordNum = 0;
	int iCallBackCmd = 0;	
	int iOffset = 0;
	/* ¿ªÊ¼È¡Êý¾ÝÁË */
	TORMPKG stPkg;
	TORMSELECTREQ *pstSelectReq = &stPkg.Body.SelectReq;
	PROPSDEF stPropsDef;

	
	pstSelectCountRes = &pstPkg->Body.SelectCountRes;
	if (TORM_ERR_IS_ERROR(pstSelectCountRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "select count res error errno=%d, errmsg=%s", 
					pstSelectCountRes->ErrNo,torm_error_string(pstSelectCountRes->ErrNo));
		return -1;
	}
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);
	if( DB_SELECT_COUNT_PROPSDEF != iCallBackCmd )
	{
		return -1;
	}
	
	iRecordNum = pstSelectCountRes->RowsCount;
	
	bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_SELECT_REQ,DB_SELECT_PROPSDEF, 
		               "PropsDef", pstPkg->Head.AsynCallBackData+iOffset, pstPkg->Head.AsynCallBackDataLen-iOffset);
	/*	
	// iRecordNumÎªÒ»¸öÆ«ÒÆÖµ   ´ÓiRecordNum¿ªÊ¼È¡¼ÇÂ¼
	if( iRecordNum  > NEW_PROPSDEF_RECORD_MAX )
	{
		iRecordNum = iRecordNum - NEW_PROPSDEF_RECORD_MAX;
	}
	else
	{
		iRecordNum = 0;
	}
	*/

	iRecordNum = 0;
	
	//È¡×îÐÂµÄ10ÌõÅäÖÃ
	pstSelectReq->OffSet = iRecordNum;
	pstSelectReq->Limit = NEW_PROPSDEF_RECORD_MAX;
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	snprintf(pstSelectReq->OperInfo.WhereDef,
	         sizeof(pstSelectReq->OperInfo.WhereDef),
	         "where now() between DATE_SUB(CAST(BeginDate as datetime),INTERVAL 1 month) and  DATE_ADD(CAST(EndDate as datetime), INTERVAL 1 month)");	
	
	return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,&stPropsDef, sizeof(PROPSDEF));

}

int bill_db_select_res_propsdef(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
	int iCallBackCmd = 0;	
	int iOffset = 0;
	TORMSELECTRES * pstSelectRes;
	PROPSDEF 	stPropsDef;
	int  i = 0;
	
	pstSelectRes = &pstPkg->Body.SelectRes;
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);

	if( DB_SELECT_PROPSDEF != iCallBackCmd )
	{
		return -1;
	}

	if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			         "Select Bill info from db failed. DBErr = %d, Msg = %s.", 
			         pstSelectRes->DBMSErrNo,
			         torm_error_string(pstSelectRes->ErrNo));
		return -1;
	}
	else if(pstSelectRes->SelectResult.ResultNum == 0)
	{
		return 0;
	}

	pstEnv->iPropsDef = 0;

	for(i=0; i<pstSelectRes->SelectResult.ResultNum ; i++)
	{
		memset(&stPropsDef,0,sizeof(stPropsDef));
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stPropsDef, sizeof(PROPSDEF)))
		{
			continue;
		}
		/*Êý¾Ý¶¼ÏÈÅª³öÀ´,µÈÉ¨ÃècacheµÄÊ±ºòÎÒÃÇÔÙÀ´×öÉ¾³ý²Ù×÷,ÇåÎúµã*/
		if( pstEnv->iPropsDef < NEW_PROPSDEF_RECORD_MAX )
		{
			memcpy(pstEnv->sPropsDef[pstEnv->iPropsDef],
						&stPropsDef,sizeof(stPropsDef));
			pstEnv->iPropsDef++;
		}
	}
	if (pstEnv->iPropsDef > 1)
	{
		qsort(pstEnv->sPropsDef[0], pstEnv->iPropsDef, PROPOS_DEF_DATA, trl_comp_func);
	}
	return 0;
}



int str_lower(char *p1,int iLen)
{
	int i;
	char c;

	for(i=0; i<iLen; i++)
	{
		c = p1[i];
		if(c >= 'A' && c <= 'Z')
		{
			p1[i]+=('a'-'A');
		}
	}
	return 0;
}

int cdkey_type_check(BILL_SVRENV *pstEnv, SSCDKEYREQ *pstReq, CDKEYTYPEDEF *pstDef)
{
	int iLen = strlen(pstDef->KeyType);
	time_t t;
	int i;
	
	char szDefLower[CDKEY_TYPE_LEN];
	char szReqLower[CDKEY_LEN];
	strcpy(szDefLower, pstDef->KeyType);
	strcpy(szReqLower, pstReq->CDKey);
	str_lower(szDefLower,strlen(szDefLower));
	str_lower(szReqLower,strlen(szReqLower));
	
	for(i=0; i<iLen; i++)
	{
		if (szDefLower[i] != szReqLower[i])
		{
			return -1;
		}
	}

	if (pstDef->EndTime != 0)
	{
		if (tdr_tdrdatetime_to_utctime(&t,pstDef->EndTime) != 0)
		{
			return -1;
		}

		if (pstEnv->pstAppCtx->stCurr.tv_sec > t)
		{
			return CDKEY_FAIL_OVERDUE;
		}
	}

	if (pstDef->LvlLowLimit >0)
	{
		if (pstReq->RoleLvl < pstDef->LvlLowLimit)
		{
			return CDKEY_FAIL_LEVEL_LOW;
		}
	}

	if (pstDef->LvlHighLimt > 0)
	{
		if (pstReq->RoleLvl > pstDef->LvlHighLimt)
		{
			return CDKEY_FAIL_LEVEL_HIGH;
		}
	}
	
	return 0;
}


int bill_db_select_cdkey(BILL_SVRENV *pstEnv,SSCDKEYREQ *pstSSReq, CDKEYTYPEDEF*pstDef, int iCmd)
{
	char szCallBackBuffer[MAX_MSG];
	int   iOffset = 0;
	CDKEYDEF stCDKeyDef;

	memset(szCallBackBuffer,0,sizeof(szCallBackBuffer));
	memcpy(szCallBackBuffer+iOffset,pstSSReq, sizeof(SSCDKEYREQ) );
	iOffset += sizeof(SSCDKEYREQ);

	memcpy(szCallBackBuffer+iOffset,pstDef, sizeof(CDKEYTYPEDEF) );
	iOffset += sizeof(CDKEYTYPEDEF);

	memset(&stCDKeyDef, 0, sizeof(stCDKeyDef));
	STRNCPY(stCDKeyDef.CDKey, pstSSReq->CDKey, sizeof(stCDKeyDef.CDKey));
	
	return bill_db_select_req(pstEnv,&stCDKeyDef,szCallBackBuffer,iOffset,iCmd);
}

int bill_db_select_res_cdkey_get(BILL_SVRENV *pstEnv,const TORMPKG *pstPkg)
{
	int iCallBackCmd = 0;	
	int iOffset = 0;
	const TORMSELECTRES * pstSelectRes = &pstPkg->Body.SelectRes;
	SSCDKEYREQ *pstReq = NULL;
	CDKEYTYPEDEF *pstTypeDef;
	int iLogErr = 0;
	int iRet = CDKEY_FAIL_SYS;
	CDKEYGET stGet;

	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "function:bill_db_select_res_cdkey_get error!errcode=%d",-1);
		return -1;
	}

	iCallBackCmd = *(int*)pstPkg->Head.AsynCallBackData;
	iOffset += sizeof(int);
	pstReq = (SSCDKEYREQ*)(pstPkg->Head.AsynCallBackData+iOffset);
	iOffset += sizeof(SSCDKEYREQ);
       pstTypeDef = (CDKEYTYPEDEF*)(pstPkg->Head.AsynCallBackData+iOffset);
	iOffset += sizeof(CDKEYTYPEDEF);

	if( DB_SELECT_CDKEYDEF_GET != iCallBackCmd )
	{
		iLogErr = -2;
		goto unlock;
	}

	if (pstSelectRes->SelectResult.ResultNum > 0)
	{	
		//iLogErr = -4;
		iRet = CDKEY_FAIL_USE;
		goto unlock;
	}

	strncpy(stGet.CDKey, pstReq->CDKey, sizeof(stGet.CDKey));
	stGet.RoleID = pstReq->RoleID;
	stGet.SvrID = pstReq->SvrID;

	// ²åÈë¼ÇÂ¼ todo:	
	if (bill_db_insert_req(pstEnv,&stGet, (char*)(pstPkg->Head.AsynCallBackData+4),
						iOffset-4,DB_INSERT_CDKEY_GET) < 0)
	{
		goto unlock;	
	}
	
	return 0;
unlock:
	bill_ss_backward_pkg_cdkey(pstEnv, SS_CDKEY_RES, iRet, pstReq->CDKey,NULL,pstReq->RoleName,1);
	if (iLogErr < 0)
		tlog_error(pstEnv->pstLogCat, 0, 0, "function:bill_db_select_res_cdkey_get error!errcode=%d",iLogErr);
	return -1;
	

}

int bill_db_select_res_cdkey_def(BILL_SVRENV *pstEnv,const TORMPKG *pstPkg)
{
	int iCallBackCmd = 0;	
	int iOffset = 0;
	const TORMSELECTRES * pstSelectRes = &pstPkg->Body.SelectRes;
	CDKEYDEF stDef;
	int iRet = CDKEY_FAIL_SYS;
	int iLogErr = 0;
	SSCDKEYREQ *pstReq = NULL;
	CDKEYTYPEDEF *pstTypeDef;

	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "function:bill_db_select_res_cdkey_def error!errcode=%d",-1);
		return -1;
	}

	iCallBackCmd = *(int*)pstPkg->Head.AsynCallBackData;
	iOffset += sizeof(int);
	pstReq = (SSCDKEYREQ*)(pstPkg->Head.AsynCallBackData+iOffset);
	iOffset += sizeof(SSCDKEYREQ);
       pstTypeDef = (CDKEYTYPEDEF*)(pstPkg->Head.AsynCallBackData+iOffset);
	iOffset += sizeof(CDKEYTYPEDEF);

	if( DB_SELECT_CDKEYDEF != iCallBackCmd )
	{
		iLogErr = -2;
		goto unlock;
	}

	if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
	{
		iLogErr = -3;
		goto unlock;
	}

	if (pstSelectRes->SelectResult.ResultNum != 1)
	{	
		iRet = CDKEY_FAIL_VAILD;
		goto unlock;
	}

	if (torm_fetch_one_meta(pstEnv->hTorm, (char *)&stDef, sizeof(stDef)) < 0)
	{
		iLogErr = -5;
		goto unlock;
	}

	if (pstTypeDef->SubType == 1)
	{
		if (bill_db_select_cdkey(pstEnv,pstReq, pstTypeDef,DB_SELECT_CDKEYDEF_GET) < 0)
		{
			iLogErr = -5;
			goto unlock;
		}
		return 0;
	}

	if (stDef.RoleID != 0 || stDef.State != 0 ||stDef.SvrID != 0)
	{
		//iLogErr = -6;
		iRet = CDKEY_FAIL_USE;
		goto unlock;
	}

	stDef.RoleID = pstReq->RoleID;
	stDef.State = 1;
	stDef.SvrID = pstReq->SvrID;

	if (bill_db_update_req(	pstEnv,&stDef,(char*)(pstPkg->Head.AsynCallBackData+4),
						iOffset-4, DB_UPDATE_CDKEY_DEF) < 0)
	{
		iLogErr = -7;
		goto unlock;
	}
	return 0;
	
unlock:
	bill_ss_backward_pkg_cdkey(pstEnv, SS_CDKEY_RES, iRet, pstReq->CDKey,NULL,pstReq->RoleName,1);
	if (iLogErr < 0)
		tlog_error(pstEnv->pstLogCat, 0, 0, "function:bill_db_select_res_cdkey_def error!errcode=%d",iLogErr);
	return -1;

}

int bill_db_select_res_cdkey_type(BILL_SVRENV *pstEnv,const TORMPKG *pstPkg)
{
	int iCallBackCmd = 0;	
	int iOffset = 0;
	const TORMSELECTRES * pstSelectRes = &pstPkg->Body.SelectRes;
	int i;
	CDKEYTYPEDEF stDef;
	int iCheck;
	int iRet = CDKEY_FAIL_SYS;
	int iLogErr = 0;
	SSCDKEYREQ *pstReq = NULL;
	
	iCallBackCmd = *(int*)pstPkg->Head.AsynCallBackData;
	iOffset += sizeof(int);
	pstReq = (SSCDKEYREQ*)(pstPkg->Head.AsynCallBackData+iOffset);
	iOffset += sizeof(SSCDKEYREQ);
	
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		iLogErr = -1;
		goto unlock;
	}
	
	if( DB_SELECT_CDKEYTYPEDEF != iCallBackCmd )
	{
		iLogErr = -2;
		goto unlock;
	}

	if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
	{
		iLogErr = -3;
		goto unlock;
	}

	if (pstSelectRes->SelectResult.ResultNum < 1)
	{	
		iRet = CDKEY_FAIL_VAILD;
		iLogErr = -4;
		goto unlock;
	}

	for (i=0; i<pstSelectRes->SelectResult.ResultNum; i++)
	{
		if (torm_fetch_one_meta(pstEnv->hTorm, (char *)&stDef, sizeof(stDef)) < 0)
		{
			continue;
		}

		// Î¢ÐÅÀñ°üLimitNum×Ö¶ÎÌî0
		if (stDef.SubType == 1 && stDef.LimitNum != 0)
		{
			goto unlock;
		}

		iCheck = cdkey_type_check(pstEnv, pstReq, &stDef);
		// ÕÒµ½ÁË È¥²écdkeydef
		if (iCheck == 0)
		{	
			if (stDef.LimitNum == 0)
			{
				if (bill_db_select_cdkey(pstEnv,pstReq, &stDef,DB_SELECT_CDKEYDEF) < 0)
				{
					iLogErr = -5;
					goto unlock;
				}
			}
			else
			{
				if (bill_db_select_cdkey(pstEnv,pstReq, &stDef,DB_SELECT_CDKEYDEF_COUNT) < 0)
				{
					iLogErr = -6;
					goto unlock;
				}
			}
			return 0;
		}
		// ÕÒµ½µ«ÊÇ¹ýÆÚÁË
		else if (iCheck == CDKEY_FAIL_OVERDUE ||
			iCheck == CDKEY_FAIL_LEVEL_HIGH ||
			iCheck == CDKEY_FAIL_LEVEL_LOW)
		{
			iRet = iCheck;
			//iLogErr = -7;
			goto unlock;
		}
	}

	// Ã»ÓÐÕÒµ½  ÎÞÐ§µÄcdkey
	if (i >= pstSelectRes->SelectResult.ResultNum)
	{
		iRet = CDKEY_FAIL_VAILD;
		//iLogErr = -8;
		goto unlock;
	}
	return 0;
	
unlock:
	bill_ss_backward_pkg_cdkey(pstEnv, SS_CDKEY_RES, iRet, pstReq->CDKey,NULL,pstReq->RoleName,1);
	if(iLogErr < 0)
		tlog_error(pstEnv->pstLogCat, 0, 0, "function:bill_db_select_res_cdkey_type error!errcode=%d",iLogErr);
	return -1;
}


int bill_db_select_res_propgift_clt(BILL_SVRENV *pstEnv,const TORMPKG *pstPkg)
{
	int iCallBackCmd = 0;	
	int iOffset = 0;
	const TORMSELECTRES * pstSelectRes = &pstPkg->Body.SelectRes;
	int i;
	int k;
	SSPKG stSSPkg;
	SSPROPGIFTLISTRES *pstRes = &stSSPkg.Body.PropGiftListRes;
	PROPSGIFT *pstPropsGift;
	SSPROPGIFTLISTREQ *pstReq = NULL;

	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}

	iCallBackCmd = *(int*)pstPkg->Head.AsynCallBackData;
	iOffset += sizeof(int);
	//iOperType = *(int*)(pstPkg->Head.AsynCallBackData+iOffset);
	//iOffset += sizeof(int);
	pstReq = (SSPROPGIFTLISTREQ*)(pstPkg->Head.AsynCallBackData+iOffset);

	if (iCallBackCmd != DB_SELECT_PROPGIFT_CLT)
	{
		if (pstReq->ID > 0)
		{
			goto unlock;
		}
		return -1;
	}

	if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
	{
		if (pstReq->ID > 0)
		{
			goto unlock;
		}
		return -1;
	}

	memset(pstRes,0,sizeof(*pstRes));

	pstRes->Uin = pstReq->Uin;
	pstRes->RoleID = pstReq->RoleID;
	pstRes->ID = pstReq->ID;
	STRNCPY(pstRes->RoleName, pstReq->RoleName, sizeof(pstRes->RoleName));

	if (pstReq->ID == 0)
	{
		for (i = 0; i < pstEnv->iPropsDef; i ++)
		{
			PROPSDEF * pstPropsDef = (PROPSDEF *)pstEnv->sPropsDef[i];
			
			pstRes->PropGiftInfo.PropsDef[pstRes->PropGiftInfo.DefCount] = *pstPropsDef;
			pstRes->PropGiftInfo.DefCount++;
		}
					
		if(	pstSelectRes->SelectResult.ResultNum <= 0 ||
			pstSelectRes->SelectResult.ResultNum > PROPS_LIMIT_MONTH)
		{
			//do nothing
		}
		else
		{
			
			for (i=0; i<pstSelectRes->SelectResult.ResultNum; i++)
			{
				pstPropsGift = &pstRes->PropGiftInfo.PropsGift[pstRes->PropGiftInfo.Count];
				if (torm_fetch_one_meta(pstEnv->hTorm, (char *)pstPropsGift, sizeof(PROPSGIFT)) < 0)
				{
					continue;
				}
				
				for (k = 0; k < pstEnv->iPropsDef; k ++)
				{
					PROPSDEF * pstPropsDef = (PROPSDEF *)pstEnv->sPropsDef[k];
					
					if (pstPropsDef->GroupID == pstPropsGift->GroupID)
					{
						pstRes->PropGiftInfo.Count++;
						break;
					}
				}
				
			}		
			


			
		}

		if (pstRes->PropGiftInfo.DefCount > 0)
		{
			// ²»ÐèÒª½âËø
			bill_ss_backward_pkg(pstEnv,&stSSPkg,PROP_GIFT_RES,NULL); 
		}
	}
	else
	{
		if (pstSelectRes->SelectResult.ResultNum != 1)
		{
			goto unlock;
		}
		else
		{
			PROPSGIFT stPropsGift;
			char CallBackData[TORM_MAX_ASYNCALLBACK_DATA_LEN]; 
			int iFound = 0;
			
			if (torm_fetch_one_meta(pstEnv->hTorm, (char *)&stPropsGift, sizeof(stPropsGift)) < 0)
			{
				goto unlock;
			}

			if (stPropsGift.GiftGetNum >= PROPS_LIMIT_NUM || stPropsGift.GiftGetNum < 0 || stPropsGift.CashNow < pstReq->CashUp)
			{
				goto unlock;
			}


			for (i = 0; i < stPropsGift.GiftGetNum; i++)
			{
				if (stPropsGift.GiftGetInfo[i].CashUp == pstReq->CashUp)
				{
					goto unlock;
				}
			}

			
			for (i = 0; i < pstEnv->iPropsDef; i ++)
			{
				PROPSDEF * pstPropsDef = (PROPSDEF *)pstEnv->sPropsDef[i];
				if(pstPropsDef->GroupID == (unsigned int)pstReq->ID && 
					pstPropsDef->CashUp == pstReq->CashUp)
				{
					time_t tTime = pstEnv->pstAppCtx->stCurr.tv_sec;
					char szStartDate[DB_TIME_LEN+6];
					char szEndDate[DB_TIME_LEN+6];
					time_t tStartDate;
					time_t tEndDate;
					
					iFound = 1;

					memset(szStartDate,0,sizeof(szStartDate));
					memset(szEndDate,0,sizeof(szEndDate)); 

					snprintf(szStartDate,sizeof(szStartDate),"%s000000",
						pstPropsDef->BeginDate);
					tStartDate = string2time(szStartDate);

					snprintf(szEndDate,sizeof(szEndDate),"%s000000",
						pstPropsDef->EndDate);
					tEndDate = string2time(szEndDate);

					if( tTime >=tStartDate && tTime < tEndDate )
					{
						//todo:ÅÐ¶Ï¹ýÆÚµÄå?
					}

				}
			}

			if (!iFound)
			{
				goto unlock;
			}
			
			stPropsGift.GiftGetInfo[stPropsGift.GiftGetNum].CashUp = pstReq->CashUp;
			stPropsGift.GiftGetInfo[stPropsGift.GiftGetNum].GetTime = pstEnv->pstAppCtx->stCurr.tv_sec;
			stPropsGift.GiftGetNum++;
			
			//stPropsGift.GetFlag = 1; // flag=99-¿ÉÒÔÁìÈ¡    0-²»¿ÉÒÔÁìÈ¡ 1-ÒÑ¾­ÁìÈ¡
			// todo:update
			iOffset = 0;
			memcpy(CallBackData+iOffset,pstReq,sizeof(*pstReq));
			iOffset += sizeof(*pstReq);
			memcpy(CallBackData+iOffset,&stPropsGift,sizeof(stPropsGift));
			iOffset += sizeof(stPropsGift);
			bill_db_update_req(pstEnv,&stPropsGift,CallBackData,iOffset,DB_UPDATE_PROPGIFT_CLT);
			
		}
	}
	return 0;

unlock:
	
	if (pstReq->Pass9Account[0] != 0)
	{
		bill_propsgift_clt(pstEnv, pstReq,PROPS_GIFT_LIST_RET_UPDATE);
		z_remove_bill_lock(pstEnv, pstReq->Pass9Account);
	}
		
	return -1;
}

int bill_db_select_res_accgift(BILL_SVRENV *pstEnv,const TORMPKG *pstPkg)
{
	int iCallBackCmd = 0;	
	int iOperType = 0;
	int iOffset = 0;
	const TORMSELECTRES * pstSelectRes = &pstPkg->Body.SelectRes;
	int i;
	SSPKG stSSPkg;
	SSACCGIFTLISTRES *pstAccGiftListRes = &stSSPkg.Body.AccGiftListRes;
	ACCGIFT *pstAccGift;
	SSACCGIFTLISTREQ *pstAccGiftListReq ;
	SSACCGIFTINSERTREQ *pstAccGiftInsertReq ;
	char * pszAccount;

	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}

	iCallBackCmd = *(int*)pstPkg->Head.AsynCallBackData;
	iOffset += sizeof(int);
	iOperType = *(int*)(pstPkg->Head.AsynCallBackData+iOffset);
	iOffset += sizeof(int);
	pstAccGiftListReq = (SSACCGIFTLISTREQ*)(pstPkg->Head.AsynCallBackData+iOffset);
	pstAccGiftInsertReq = (SSACCGIFTINSERTREQ*)pstAccGiftListReq;

	pszAccount = pstAccGiftListReq->Pass9Account;
	if (iOperType == INSERT_ACC_GIFT_INFO)
	{
		pszAccount = pstAccGiftInsertReq->Pass9Account;
	}

	if (iCallBackCmd != DB_SELECT_ACCGIFT)
	{
		//if (pstAccGiftListReq->ID > 0)
		{
			goto unlock;
		}
		return -1;
	}

	if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
	{
		//if (pstAccGiftReq->ID > 0)
		{
			goto unlock;
		}
		return -1;
	}
	
	if (iOperType == QUERY_ACC_GIFT_INFO)
	{
		pstAccGiftListRes->Uin = pstAccGiftListReq->Uin;
		pstAccGiftListRes->RoleID = pstAccGiftListReq->RoleID;
		pstAccGiftListRes->ID = pstAccGiftListReq->ID;
		STRNCPY(pstAccGiftListRes->RoleName, pstAccGiftListReq->RoleName, sizeof(pstAccGiftListRes->RoleName));

		if (pstAccGiftListReq->ID == 0)
		{
			if(	pstSelectRes->SelectResult.ResultNum <= 0 ||
				pstSelectRes->SelectResult.ResultNum > MAX_ACC_GIFT)
			{
				return -1;
			}
			else
			{
				pstAccGiftListRes->Count = 0;
				for (i=0; i<pstSelectRes->SelectResult.ResultNum; i++)
				{
					pstAccGift = &pstAccGiftListRes->AccGifts[pstAccGiftListRes->Count];
					if (torm_fetch_one_meta(pstEnv->hTorm, (char *)pstAccGift, sizeof(ACCGIFT)) < 0)
					{
						continue;
					}
					pstAccGiftListRes->Count ++;
				}

				if (pstAccGiftListRes->Count > 0)
				{
					// ²»ÐèÒª½âËø
					bill_ss_backward_pkg(pstEnv,&stSSPkg,ACC_GIFT_LIST_RES,NULL); 
				}
				
			}
		}
		else
		{
			if (pstSelectRes->SelectResult.ResultNum != 1)
			{
				goto unlock;
			}
			else
			{
				ACCGIFT stAccGift;
				char CallBackData[TORM_MAX_ASYNCALLBACK_DATA_LEN]; 
				
				if (torm_fetch_one_meta(pstEnv->hTorm, (char *)&stAccGift, sizeof(ACCGIFT)) < 0)
				{
					goto unlock;
				}
				stAccGift.Flag = 1;
				tdr_utctime_to_tdrdatetime (&stAccGift.GetGiftTime, time(NULL));
				// todo:update
				iOffset = 0;
				memcpy(CallBackData+iOffset,&iOperType,sizeof(iOperType));
				iOffset += sizeof(iOperType);
				memcpy(CallBackData+iOffset,pstAccGiftListReq,sizeof(*pstAccGiftListReq));
				iOffset += sizeof(*pstAccGiftListReq);
				memcpy(CallBackData+iOffset,&stAccGift,sizeof(stAccGift));
				iOffset += sizeof(stAccGift);
				bill_db_update_req(pstEnv,&stAccGift,CallBackData,iOffset,DB_UPDATE_ACCGIFT);
			}
		}
		return 0;
	}
	else if (iOperType == INSERT_ACC_GIFT_INFO)
	{
		//acc gift insert
		//bill_ss_backward_pkg(pstEnv,&stSSPkg,ACC_GIFT_INSERT_RES,NULL); 
		if (pstSelectRes->SelectResult.ResultNum != 0)
		{
			//ÒÑ´æÔÚ,²»²åÈë
			goto unlock;
		}
		else
		{
			ACCGIFT *pstAccGift;
			char CallBackData[TORM_MAX_ASYNCALLBACK_DATA_LEN]; 
			
			pstAccGift = &pstAccGiftInsertReq->AccGift;

			pstAccGift->ID = 0;
			pstAccGift->Flag = 0;
			pstAccGift->GetGiftTime = 0;
			tdr_utctime_to_tdrdatetime (&pstAccGift->SetGiftTime, time(NULL));
			
			iOffset = 0;
			memcpy(CallBackData+iOffset,&iOperType,sizeof(iOperType));
			iOffset += sizeof(iOperType);
			memcpy(CallBackData+iOffset,pstAccGiftInsertReq,sizeof(*pstAccGiftInsertReq));
			iOffset += sizeof(*pstAccGiftInsertReq);
			memcpy(CallBackData+iOffset,pstAccGift,sizeof(*pstAccGift));
			iOffset += sizeof(*pstAccGift);
			bill_db_insert_req(pstEnv,pstAccGift,CallBackData,iOffset,DB_INSERT_ACCGIFT);
		}

	}

unlock:
	if (pszAccount &&  *pszAccount != 0)
	{
		z_remove_bill_lock(pstEnv, pszAccount);
	}
		
	return -1;
}

int propsgift_data_fill(PROPSGIFT *pstPropsGift, PROPSDEF *pstPropsDef,SSCASHBILLREQ *pstCashBillReq)
{
//	int i;
	pstPropsGift->GroupID = pstPropsDef->GroupID;
	
	//sprintf(&pstPropsGift->PropsGiftDesc[0], "%s µ½%s ÆÚ¼ä,ÀÛ¼Æ³äÖµ´ïµ½%llu µã", 
			//pstPropsDef->BeginDate,pstPropsDef->EndDate,pstPropsDef->PropsLimit.CashUp);

	pstPropsGift->Uin = pstCashBillReq->Uin;
	pstPropsGift->CashNow = pstCashBillReq->Points;
/*
	pstPropsGift->GetFlag = 0;
	if (pstPropsGift->CashNow >= pstPropsDef->PropsLimit.CashUp)
	{
		pstPropsGift->GetFlag = 99;
	}
	
	for (i=0; i<PROPS_GIFT_ITEM_NUM; i++)
	{
		pstPropsGift->PropsGiftItem[i] = 
				pstPropsDef->PropsLimit.PropsGiftItem[i];
	}
	*/
	return 0;
}

tdr_ulonglong get_propsgift_max_cash(PROPSGIFT *pstPropsGift, int iNum)
{
	tdr_ulonglong  ullCash = 0;
	int i;

	if (iNum <= 0)
	{
		return 0;
	}
	
	ullCash = pstPropsGift[0].CashNow;
	for (i=1; i<iNum; i++)
	{
		if (ullCash < pstPropsGift[i].CashNow)
		{
			ullCash = pstPropsGift[i].CashNow;
		}
	}

	return ullCash;
}



int bill_db_select_res_propsgift(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
	TORMSELECTRES * pstSelectRes;
	char CallBackData[TORM_MAX_ASYNCALLBACK_DATA_LEN]; 
	int iCallBackCmd = 0;	
	int iOffset = 0;
	SSCASHBILLREQ *pstCashBillReq = NULL;
	PROPSGIFT  stPropsGift;
//	PROPSDEF* pstPropsdef = NULL;
	int i = 0;
//	int j = 0;
	int iOperType = 0;
	int iToClt = 0;

	memset(&stPropsGift,0,sizeof(stPropsGift));
	memset(CallBackData,0,sizeof(CallBackData));
	
	pstSelectRes = &pstPkg->Body.SelectRes;
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);
	
	memcpy(&iOperType,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iOperType));
	iOffset += sizeof(iOperType);

	if( DB_SELECT_PROPSGIFT != iCallBackCmd )
	{
		return -1;
	}

	// ÏÈÕâÃ´Ð´×Å ¹¦ÄÜÊµÏÖÁË ÔÚÖØ¹¹¡£
	if( QUERY_PROPSGIFT_BY_INFORMER == iOperType )
	{
		bill_db_get_curr_propsdef(pstEnv);
		if (pstEnv->iDefNum <= 0)
			return -1;
		
		pstCashBillReq = (SSCASHBILLREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(SSCASHBILLREQ);

	
		if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
		{
			return -1;
		}
		else if(pstSelectRes->SelectResult.ResultNum == 0 )
		{
			for (i=0; i<pstEnv->iDefNum; i++)
			{
				propsgift_data_fill(&stPropsGift, &pstEnv->stPropsDef[i], pstCashBillReq);
				//iToClt = 0;
				iToClt = 1;
				//if (pstCashBillReq->Points >= pstEnv->stPropsDef[i].PropsLimit.CashUp)
					//iToClt = 1;
				
				iOffset = 0;
				memcpy(CallBackData+iOffset,&iOperType,sizeof(iOperType));
				iOffset += sizeof(iOperType);
				
				memcpy(CallBackData+iOffset,pstCashBillReq,sizeof(SSCASHBILLREQ));
				iOffset += sizeof(SSCASHBILLREQ);
				memcpy(CallBackData+iOffset,&stPropsGift,sizeof(stPropsGift));
				iOffset += sizeof(stPropsGift);
				memcpy(CallBackData+iOffset,&iToClt,sizeof(iToClt));
				iOffset += sizeof(iToClt);
				
				bill_db_insert_req(pstEnv,&stPropsGift,CallBackData,iOffset,DB_INSERT_PROPSGIFT);
			}
		}
		else
		{
			/*
			int iGiftNum = 0;
			PROPSGIFT  stPropsGifts[NEW_PROPSDEF_RECORD_MAX];

			// ÕýÏò¼ì²é
			for (i=0; i<pstSelectRes->SelectResult.ResultNum; i++)
			{
				if (torm_fetch_one_meta(pstEnv->hTorm, (char *)&stPropsGifts[iGiftNum], sizeof(PROPSGIFT))< 0)
				{
					continue;
				}
			
				pstPropsdef = bill_db_get_propsdef(pstEnv, stPropsGifts[iGiftNum].GroupID);
				if (!pstPropsdef)
				{
					continue;
				}

				stPropsGifts[iGiftNum].CashNow += pstCashBillReq->Points;
				iToClt = 0;
				if (stPropsGifts[iGiftNum].GetFlag == 0)
				{
					if (stPropsGifts[iGiftNum].CashNow >= pstPropsdef->PropsLimit.CashUp)
					{
						stPropsGifts[iGiftNum].GetFlag = 99;
						iToClt = 1;
					}
				}

				iOffset = 0;
				memcpy(CallBackData+iOffset,&iOperType,sizeof(iOperType));
				iOffset += sizeof(iOperType);
				
				memcpy(CallBackData+iOffset,pstCashBillReq,sizeof(SSCASHBILLREQ));
				iOffset += sizeof(SSCASHBILLREQ);
				
				memcpy(CallBackData+iOffset,&stPropsGifts[iGiftNum],sizeof(stPropsGifts[iGiftNum]));
				iOffset += sizeof(stPropsGifts[iGiftNum]);

				memcpy(CallBackData+iOffset,&iToClt,sizeof(iToClt));
					iOffset += sizeof(iToClt);
			
				bill_db_update_req(pstEnv,&stPropsGifts[iGiftNum],CallBackData,iOffset,DB_UPDATE_PROPSGIFT);	
				iGiftNum ++;
				if (iGiftNum >= pstSelectRes->SelectResult.ResultNum)
				{
					break;
				}
			}

			// ·´Ïò¼ì²é
			for (i=0; i<pstEnv->iDefNum; i++)
			{
				for (j=0; j<iGiftNum; j++)
				{
					if (pstEnv->stPropsDef[i].PropsID == stPropsGifts[j].PropsDefID)
					{
						break;
					}
				}

				if (j>=iGiftNum)
				{
					propsgift_data_fill(&stPropsGift, &pstEnv->stPropsDef[i], pstCashBillReq);
					stPropsGift.CashNow = get_propsgift_max_cash(stPropsGifts, iGiftNum);
					iToClt = 0;
					if (stPropsGift.CashNow >= pstEnv->stPropsDef[i].PropsLimit.CashUp)
					{
						stPropsGift.GetFlag = 99;
						iToClt = 1;
					}

					iOffset = 0;
					memcpy(CallBackData+iOffset,&iOperType,sizeof(iOperType));
					iOffset += sizeof(iOperType);
				
					memcpy(CallBackData+iOffset,pstCashBillReq,sizeof(SSCASHBILLREQ));
					iOffset += sizeof(SSCASHBILLREQ);

					memcpy(CallBackData+iOffset,&stPropsGift,sizeof(stPropsGift));
					iOffset += sizeof(stPropsGift);

					memcpy(CallBackData+iOffset,&iToClt,sizeof(iToClt));
					iOffset += sizeof(iToClt);
					bill_db_insert_req(pstEnv,&stPropsGift,CallBackData,iOffset,DB_INSERT_PROPSGIFT);
				}
			}*/
		}
	}
#if 0
	else if(QUERY_PROPSGIFT_BY_USER_QUERY == iOperType )
	{
		SSPKG stSSPkg;
		SSBILLQUERYPROPSGIFTREQ* pstBillQueryPropsGiftReq;
		pstBillQueryPropsGiftReq = (SSBILLQUERYPROPSGIFTREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(SSBILLQUERYPROPSGIFTREQ);

		
		if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
		{
			
		}
		else if(pstSelectRes->SelectResult.ResultNum == 0 )
		{
			
		}
		else if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stPropsGift, sizeof(stPropsGift))
			 < 0)
		{
			
		}
		else
		{
			// ÓÃ»§²éÑ¯ÔÝÊ±²»»ØÐ´
			for(i=0;i<PROPS_ACCOUNT_ACCEPT_NUM; i++)
			{
				// ÅäÖÃÒÑ¾­Ã»ÓÐÁË
				if( pstEnv->iPropsDef>0 &&
					NULL == bill_get_propsdef(pstEnv,stPropsGift.PropsGiftInfo[i].PropsID) )
				{				
					stPropsGift.PropsGiftInfo[i].GetFlag = 0;
					stPropsGift.PropsGiftInfo[i].MoneyTotal = 0;
					stPropsGift.PropsGiftInfo[i].PropsID = 0;
					stPropsGift.PropsGiftInfo[i].FreshTime = 0;
				}
			}
		}
		//³ö´íºÍÃ»ÓÐ¼ÇÂ¼¶¼Òª·µ»Ø
		memset(&stSSPkg,0,sizeof(stSSPkg));

		stSSPkg.Body.BillQueryPropsGiftRes.Uin = pstBillQueryPropsGiftReq->Uin;
		stSSPkg.Body.BillQueryPropsGiftRes.PropsGift = stPropsGift;

		bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_QUERY_PROPSGIFT_RES,pstBillQueryPropsGiftReq->Pass9Account); 
		
	}
	else if(QUERY_PROPSGIFT_BY_USER_SET == iOperType )
	{
		SSPKG stSSPkg;
		SSBILLSETPROPSGIFTREQ* pstSSBillSetPropsGiftReq;

		memset(&stSSPkg,0,sizeof(stSSPkg));
		pstSSBillSetPropsGiftReq = (SSBILLSETPROPSGIFTREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(SSBILLSETPROPSGIFTREQ);

		
		if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
		{
			stSSPkg.Body.BillSetPropsGiftRes.Uin = pstSSBillSetPropsGiftReq->Uin;
			stSSPkg.Body.BillSetPropsGiftRes.Result = 1;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_SET_PROPSGIFT_RES,pstSSBillSetPropsGiftReq->Pass9Account); 

			return -1;
		}
		else if(pstSelectRes->SelectResult.ResultNum == 0 )
		{
			stSSPkg.Body.BillSetPropsGiftRes.Uin = pstSSBillSetPropsGiftReq->Uin;
			stSSPkg.Body.BillSetPropsGiftRes.Result = 1;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_SET_PROPSGIFT_RES,pstSSBillSetPropsGiftReq->Pass9Account); 

			return -1;
		}
		else if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stPropsGift, sizeof(stPropsGift))
			 < 0)
		{
			stSSPkg.Body.BillSetPropsGiftRes.Uin = pstSSBillSetPropsGiftReq->Uin;
			stSSPkg.Body.BillSetPropsGiftRes.Result = 1;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_SET_PROPSGIFT_RES,pstSSBillSetPropsGiftReq->Pass9Account); 

			return -1;
		}

		if(pstSSBillSetPropsGiftReq->GetType == 1)
		{
			if(pstSSBillSetPropsGiftReq->Index<0 || pstSSBillSetPropsGiftReq->Index>=PROPS_LIMIT_NUM)
			{
				stSSPkg.Body.BillSetPropsGiftRes.Uin = pstSSBillSetPropsGiftReq->Uin;
				stSSPkg.Body.BillSetPropsGiftRes.Result = 1;
				bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_SET_PROPSGIFT_RES,pstSSBillSetPropsGiftReq->Pass9Account); 
				return -1;
			}
			
			pstPropsdef = bill_get_propsdef(pstEnv,pstSSBillSetPropsGiftReq->PropsID);
			if( NULL == pstPropsdef )
			{
				stSSPkg.Body.BillSetPropsGiftRes.Uin = pstSSBillSetPropsGiftReq->Uin;
				stSSPkg.Body.BillSetPropsGiftRes.Result = 1;
				bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_SET_PROPSGIFT_RES,pstSSBillSetPropsGiftReq->Pass9Account); 
				return -1;
			}
			for(i=0;i<PROPS_ACCOUNT_ACCEPT_NUM;i++)
			{
				if( stPropsGift.PropsGiftInfo[i].PropsID == pstSSBillSetPropsGiftReq->PropsID)
				{
					break;
				}
			}
			if(i>=PROPS_ACCOUNT_ACCEPT_NUM )
			{
				stSSPkg.Body.BillSetPropsGiftRes.Uin = pstSSBillSetPropsGiftReq->Uin;
				stSSPkg.Body.BillSetPropsGiftRes.Result = 1;
				bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_SET_PROPSGIFT_RES,pstSSBillSetPropsGiftReq->Pass9Account); 

				return -1;
			}

			
			if( pstPropsdef->PropsLimit[pstSSBillSetPropsGiftReq->Index].RepeatFlag &&
				stPropsGift.PropsGiftInfo[i].GetFlag & (1<<pstSSBillSetPropsGiftReq->Index) )
			{
				//ÒÑ¾­ÁìÈ¡¹ýÁË,¸ÃÀñ°üÖ»ÄÜÁìÈ¡Ò»´Î
				stSSPkg.Body.BillSetPropsGiftRes.Uin = pstSSBillSetPropsGiftReq->Uin;
				stSSPkg.Body.BillSetPropsGiftRes.Result = 2;
				bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_SET_PROPSGIFT_RES,pstSSBillSetPropsGiftReq->Pass9Account); 

				return -1;
			}

			if(stPropsGift.PropsGiftInfo[i].MoneyTotal < pstPropsdef->PropsLimit[pstSSBillSetPropsGiftReq->Index].CashUp )
			{
				//³åµÄÇ®²»¹»
				stSSPkg.Body.BillSetPropsGiftRes.Uin = pstSSBillSetPropsGiftReq->Uin;
				stSSPkg.Body.BillSetPropsGiftRes.Result = 1;
				bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_SET_PROPSGIFT_RES,pstSSBillSetPropsGiftReq->Pass9Account); 
				return -1;
			}
			stPropsGift.PropsGiftInfo[i].MoneyTotal -= pstPropsdef->PropsLimit[pstSSBillSetPropsGiftReq->Index].CashUp;
			if( pstPropsdef->PropsLimit[pstSSBillSetPropsGiftReq->Index].RepeatFlag)
			{
				stPropsGift.PropsGiftInfo[i].GetFlag |= (1<<pstSSBillSetPropsGiftReq->Index);
			}
			
			
		}
		else if(pstSSBillSetPropsGiftReq->GetType == 2)
		{
			if(pstSSBillSetPropsGiftReq->Index<0 || pstSSBillSetPropsGiftReq->Index>=PROPS_GIFT_ITEM_NUM)
			{
				stSSPkg.Body.BillSetPropsGiftRes.Uin = pstSSBillSetPropsGiftReq->Uin;
				stSSPkg.Body.BillSetPropsGiftRes.Result = 1;
				bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_SET_PROPSGIFT_RES,pstSSBillSetPropsGiftReq->Pass9Account); 
				return -1;
			}
			if( stPropsGift.PropsGiftItem[pstSSBillSetPropsGiftReq->Index].ItemID ==0 ||
				stPropsGift.PropsGiftItem[pstSSBillSetPropsGiftReq->Index].ItemNum ==0 ||
				stPropsGift.PropsGiftItem[pstSSBillSetPropsGiftReq->Index].ItemID != pstSSBillSetPropsGiftReq->GiftItem.ItemID )
			{
				//Êý¾Ý´íÎó²»ÄÜÁìÈ¡
				stSSPkg.Body.BillSetPropsGiftRes.Uin = pstSSBillSetPropsGiftReq->Uin;
				stSSPkg.Body.BillSetPropsGiftRes.Result = 1;
				bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_SET_PROPSGIFT_RES,pstSSBillSetPropsGiftReq->Pass9Account); 
				return -1;
			}
			// ¿ªÊ¼ÁìÈ¡
			stPropsGift.PropsGiftItem[pstSSBillSetPropsGiftReq->Index].ItemID = 0;
			stPropsGift.PropsGiftItem[pstSSBillSetPropsGiftReq->Index].ItemNum = 0;
		}

		// °ÑÒ»Ð©¹ýÆÚµÄÊý¾ÝÇåÀíÏÂ
		for(i=0;i<PROPS_ACCOUNT_ACCEPT_NUM; i++)
		{
			// ÅäÖÃÒÑ¾­Ã»ÓÐÁË
			if( pstEnv->iPropsDef>0 &&
				NULL == bill_get_propsdef(pstEnv,stPropsGift.PropsGiftInfo[i].PropsID) )
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, 
			         "Propsdef PropsID(%d) is destroy|Pass9Account|%s|PropsID|%d|GetFlag|%d|MoneyTotal|%llu", 
			         stPropsGift.PropsGiftInfo[i].PropsID,
			         stPropsGift.Pass9Account,
			         stPropsGift.PropsGiftInfo[i].PropsID, 
			         stPropsGift.PropsGiftInfo[i].GetFlag,
			         stPropsGift.PropsGiftInfo[i].MoneyTotal);
				
				stPropsGift.PropsGiftInfo[i].GetFlag = 0;
				stPropsGift.PropsGiftInfo[i].MoneyTotal = 0;
				stPropsGift.PropsGiftInfo[i].PropsID = 0;
				stPropsGift.PropsGiftInfo[i].FreshTime = 0;
			}
		}
		
		
		iOffset = 0;
		memcpy(CallBackData+iOffset,&iOperType,sizeof(iOperType));
		iOffset += sizeof(iOperType);
		memcpy(CallBackData+iOffset,pstSSBillSetPropsGiftReq,sizeof(SSBILLSETPROPSGIFTREQ));
		iOffset += sizeof(SSBILLSETPROPSGIFTREQ);
		memcpy(CallBackData+iOffset,&stPropsGift,sizeof(stPropsGift));
		iOffset += sizeof(stPropsGift);
		bill_db_update_req(pstEnv,&stPropsGift,CallBackData,iOffset,DB_UPDATE_PROPSGIFT);
		
	}
	else if( CASH_PROPSGIFT_BY_CS == iOperType )
	{
		SSPKG stSSPkg;
		SSCASHPROPSGIFTINFOREQ* pstSSCashPropsGiftInfoReq;

		memset(&stSSPkg,0,sizeof(stSSPkg));
		pstSSCashPropsGiftInfoReq = (SSCASHPROPSGIFTINFOREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(SSCASHPROPSGIFTINFOREQ);

		
		if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
		{
			STRNCPY(stSSPkg.Body.CashPropsGiftInfoRes.Pass9Account,pstSSCashPropsGiftInfoReq->Pass9Account,
				CCH(stSSPkg.Body.CashPropsGiftInfoRes.Pass9Account));
			stSSPkg.Body.CashPropsGiftInfoRes.Result = 1;
			stSSPkg.Body.CashPropsGiftInfoRes.CsConnCtx = pstSSCashPropsGiftInfoReq->CsConnCtx;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_CASH_PROPSGIFT_RES,pstSSCashPropsGiftInfoReq->Pass9Account); 

			return -1;
		}
		else if(pstSelectRes->SelectResult.ResultNum == 0 )
		{
			//×ö²åÈë²Ù×÷
			STRNCPY(stPropsGift.Pass9Account,pstSSCashPropsGiftInfoReq->Pass9Account,
				CCH(stPropsGift.Pass9Account));
		}
		else if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stPropsGift, sizeof(stPropsGift))
			 < 0)
		{
			STRNCPY(stSSPkg.Body.CashPropsGiftInfoRes.Pass9Account,pstSSCashPropsGiftInfoReq->Pass9Account,
				CCH(stSSPkg.Body.CashPropsGiftInfoRes.Pass9Account));
			stSSPkg.Body.CashPropsGiftInfoRes.Result = 1;
			stSSPkg.Body.CashPropsGiftInfoRes.CsConnCtx = pstSSCashPropsGiftInfoReq->CsConnCtx;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_CASH_PROPSGIFT_RES,pstSSCashPropsGiftInfoReq->Pass9Account); 

			return -1;
		}

		for(i=0;i<PROPS_GIFT_ITEM_NUM;i++)
		{
			if(pstSSCashPropsGiftInfoReq->GiftItem.ItemID == stPropsGift.PropsGiftItem[i].ItemID)
			{
				stPropsGift.PropsGiftItem[i].ItemNum += pstSSCashPropsGiftInfoReq->GiftItem.ItemNum;
				break;
			}
		}
		if(i>= PROPS_GIFT_ITEM_NUM)
		{
			for(i=0;i<PROPS_GIFT_ITEM_NUM;i++)
			{
				if( stPropsGift.PropsGiftItem[i].ItemID == 0)
				{
					stPropsGift.PropsGiftItem[i].ItemID = pstSSCashPropsGiftInfoReq->GiftItem.ItemID;
					stPropsGift.PropsGiftItem[i].ItemNum = pstSSCashPropsGiftInfoReq->GiftItem.ItemNum;
					break;
				}
			}
		}
		
		//Ä¬ÈÏ°ÑµÚÒ»¸²¸Çµô
		if(i>= PROPS_GIFT_ITEM_NUM)
		{
			stPropsGift.PropsGiftItem[0].ItemID = pstSSCashPropsGiftInfoReq->GiftItem.ItemID;
			stPropsGift.PropsGiftItem[0].ItemNum = pstSSCashPropsGiftInfoReq->GiftItem.ItemNum;
		}

		
		iOffset = 0;
		memcpy(CallBackData+iOffset,&iOperType,sizeof(iOperType));
		iOffset += sizeof(iOperType);
		memcpy(CallBackData+iOffset,pstSSCashPropsGiftInfoReq,sizeof(SSCASHPROPSGIFTINFOREQ));
		iOffset += sizeof(SSCASHPROPSGIFTINFOREQ);
		memcpy(CallBackData+iOffset,&stPropsGift,sizeof(stPropsGift));
		iOffset += sizeof(stPropsGift);
		bill_db_update_req(pstEnv,&stPropsGift,CallBackData,iOffset,DB_UPDATE_PROPSGIFT);
	}

#endif
	
	return 0;
}
int bill_db_select_req_propsgift(BILL_SVRENV *pstEnv,char *pstData,int iLen,int Uin,int OperType)
{
	PROPSGIFT stPropsGift;
	char szCallBackBuffer[MAX_MSG];
	int   iOffset = 0;

	/*if( QUERY_PROPSGIFT_BY_INFORMER == OperType && NULL == bill_db_props_check_date(pstEnv) )
	{
		//²»ÔÙÊ±¼ä¶ÎÀï²»´¦Àí
		//return -1;
	}*/

	bill_db_get_curr_propsdef(pstEnv);
	if (pstEnv->iDefNum <= 0)
		return -1;
	
	memset(&stPropsGift,0,sizeof(stPropsGift));
	memset(szCallBackBuffer,0,sizeof(szCallBackBuffer));

	memcpy(szCallBackBuffer+iOffset,&OperType,sizeof(OperType) );
	iOffset += sizeof(OperType);
	memcpy(szCallBackBuffer+iOffset,pstData,iLen );
	iOffset += iLen;

	stPropsGift.Uin = Uin;
	
	bill_db_select_req(pstEnv,&stPropsGift,szCallBackBuffer,iOffset,DB_SELECT_PROPSGIFT);

	return 0;

}
int bill_db_update_res_propsgift(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
 	int iCallBackCmd = 0;	
	int iOffset = 0;
	TORMUPDATERES * pstRes;
	//SSBILLFREEZEPOINTRES *pstBillFreezePointRes;
	int iOperType = 0;
	
	pstRes = &pstPkg->Body.UpdateRes;
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);
	memcpy(&iOperType,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iOperType));
	iOffset += sizeof(iOperType);

	if( DB_UPDATE_PROPSGIFT != iCallBackCmd )
	{
		return -1;
	}

	if(QUERY_PROPSGIFT_BY_USER_SET == iOperType)
	{
		SSPKG stSSPkg;
		SSBILLSETPROPSGIFTREQ* pstSSBillSetPropsGiftReq;
		PROPSGIFT  *pstPropsGift;

		memset(&stSSPkg,0,sizeof(stSSPkg));
		
		pstSSBillSetPropsGiftReq = (SSBILLSETPROPSGIFTREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(SSBILLSETPROPSGIFTREQ);
		pstPropsGift = (PROPSGIFT *)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(PROPSGIFT);
		
		if (TORM_ERR_IS_ERROR(pstRes->ErrNo))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, 
				         "update Bill info from db failed. DBErr = %d, uin = %d, Msg = %s.", 
				         pstRes->DBMSErrNo,
				         pstSSBillSetPropsGiftReq->Uin,
				         torm_error_string(pstRes->ErrNo));
			
			stSSPkg.Body.BillSetPropsGiftRes.Uin = pstSSBillSetPropsGiftReq->Uin;
			stSSPkg.Body.BillSetPropsGiftRes.Result = 3;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_SET_PROPSGIFT_RES,pstSSBillSetPropsGiftReq->Pass9Account); 
			
			return -1;
		}

		if(pstRes->AffectRows < 1)
		{
			/* ÑÏ¸ñÅÐ¶Ï */
			stSSPkg.Body.BillSetPropsGiftRes.Uin = pstSSBillSetPropsGiftReq->Uin;
			stSSPkg.Body.BillSetPropsGiftRes.Result = 2;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_SET_PROPSGIFT_RES,pstSSBillSetPropsGiftReq->Pass9Account);
			return -1;
		}

		stSSPkg.Body.BillSetPropsGiftRes.Uin = pstSSBillSetPropsGiftReq->Uin;
		stSSPkg.Body.BillSetPropsGiftRes.Result = 0;
		stSSPkg.Body.BillSetPropsGiftRes.PropsGift = *pstPropsGift;
		STRNCPY(stSSPkg.Body.BillSetPropsGiftRes.Pass9Account,pstSSBillSetPropsGiftReq->Pass9Account,
			CCH(stSSPkg.Body.BillSetPropsGiftRes.Pass9Account));
		stSSPkg.Body.BillSetPropsGiftRes.PropsID = pstSSBillSetPropsGiftReq->PropsID;
		stSSPkg.Body.BillSetPropsGiftRes.BillSetPropsGiftReq = *pstSSBillSetPropsGiftReq;
		
		bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_SET_PROPSGIFT_RES,pstSSBillSetPropsGiftReq->Pass9Account);
			
	}
	else if(QUERY_PROPSGIFT_BY_INFORMER == iOperType)
	{
		SSCASHBILLREQ stCashBillReq;
		PROPSGIFT  stPropsGift;
		int iToClt;
		
		if(pstRes->AffectRows != 1)
		{
			return -1;
		}

		stCashBillReq = *(SSCASHBILLREQ*)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(SSCASHBILLREQ);
		
		stPropsGift  = *(PROPSGIFT*)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(PROPSGIFT);
		
		iToClt =  *(int*)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(int);
		
		if (iToClt)
		{
			bill_db_propsgift2region(pstEnv, &stPropsGift, stCashBillReq.Pass9Account);
		}
	}
	else if(CASH_PROPSGIFT_BY_CS == iOperType)
	{
		SSPKG stSSPkg;
		SSCASHPROPSGIFTINFOREQ* pstSSCashPropsGiftInfoReq;
		PROPSGIFT  *pstPropsGift;
		
		memset(&stSSPkg,0,sizeof(stSSPkg));
		
		
		pstSSCashPropsGiftInfoReq = (SSCASHPROPSGIFTINFOREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(SSCASHPROPSGIFTINFOREQ);
		pstPropsGift = (PROPSGIFT *)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(PROPSGIFT);
		
		if (TORM_ERR_IS_ERROR(pstRes->ErrNo))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, 
				         "update Bill info from db failed. DBErr = %d, Name = %s, Msg = %s.", 
				         pstRes->DBMSErrNo,
				         //pstBillFreezePointRes->BillRecord.Pass9Account,
				         pstSSCashPropsGiftInfoReq->Pass9Account,
				         torm_error_string(pstRes->ErrNo));
			
			STRNCPY(stSSPkg.Body.CashPropsGiftInfoRes.Pass9Account, pstSSCashPropsGiftInfoReq->Pass9Account,
				CCH(stSSPkg.Body.CashPropsGiftInfoRes.Pass9Account));
			stSSPkg.Body.CashPropsGiftInfoRes.Result = 3;
			stSSPkg.Body.CashPropsGiftInfoRes.CsConnCtx = pstSSCashPropsGiftInfoReq->CsConnCtx;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_CASH_PROPSGIFT_RES,pstSSCashPropsGiftInfoReq->Pass9Account); 
			
			return -1;
		}

		if(pstRes->AffectRows < 1)
		{
			/* ÑÏ¸ñÅÐ¶Ï */
			STRNCPY(stSSPkg.Body.CashPropsGiftInfoRes.Pass9Account, pstSSCashPropsGiftInfoReq->Pass9Account,
				CCH(stSSPkg.Body.CashPropsGiftInfoRes.Pass9Account));
			stSSPkg.Body.CashPropsGiftInfoRes.Result = 3;
			stSSPkg.Body.CashPropsGiftInfoRes.CsConnCtx = pstSSCashPropsGiftInfoReq->CsConnCtx;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_CASH_PROPSGIFT_RES,pstSSCashPropsGiftInfoReq->Pass9Account); 
			return -1;
		}

		STRNCPY(stSSPkg.Body.CashPropsGiftInfoRes.Pass9Account, pstSSCashPropsGiftInfoReq->Pass9Account,
				CCH(stSSPkg.Body.CashPropsGiftInfoRes.Pass9Account));
		stSSPkg.Body.CashPropsGiftInfoRes.Result = 0;
		stSSPkg.Body.CashPropsGiftInfoRes.CsConnCtx = pstSSCashPropsGiftInfoReq->CsConnCtx;
		bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_CASH_PROPSGIFT_RES,pstSSCashPropsGiftInfoReq->Pass9Account); 
	}

	
 	return 0;
}

int bill_process_region_cash_bill(BILL_SVRENV *pstEnv,SSCASHBILLREQ *pstCashBillReq)
{
	char szCallBackBuffer[MAX_MSG];
	int   iOffset = 0;

	memset(szCallBackBuffer,0,sizeof(szCallBackBuffer));
	
	memcpy(szCallBackBuffer+iOffset,pstCashBillReq,sizeof(SSCASHBILLREQ) );
	iOffset += sizeof(SSCASHBILLREQ);
	
	bill_db_select_req(pstEnv,pstCashBillReq,szCallBackBuffer,iOffset,DB_SELECT_BILL_CASH_RECORD);

	return 0;
}


int bill_db_select_req_cash(BILL_SVRENV *pstEnv,SSCASHBILLREQ *pstCashBillReq)
{
	BILL stBill;
	char szCallBackBuffer[MAX_MSG];
	int   iOffset = 0;

	memset(&stBill,0,sizeof(stBill));
	memset(szCallBackBuffer,0,sizeof(szCallBackBuffer));
	
	memcpy(szCallBackBuffer+iOffset,pstCashBillReq,sizeof(SSCASHBILLREQ) );
	iOffset += sizeof(SSCASHBILLREQ);
	
	stBill.Uin = pstCashBillReq->Uin;
	bill_db_select_req(pstEnv,&stBill,szCallBackBuffer,iOffset,DB_SELECT_BILL_CASH);

	return 0;
}

int bill_process_region_get_propsdef(BILL_SVRENV *pstEnv,SSBILLQUERYPROPSDEFREQ* pstBillQueryPropsDefReq)
{	
	int i=0;
	SSPKG stSSPkg;
	memset(&stSSPkg,0,sizeof(stSSPkg));
	UNUSED(pstBillQueryPropsDefReq);

	for(i=0;i<NEW_PROPSDEF_RECORD_MAX;i++)
	{
		memcpy(&stSSPkg.Body.BillQueryPropsDefRes.PropsDef[i],pstEnv->sPropsDef[i],sizeof(PROPSDEF));
	}

	bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_QUERY_PROPSDEF_RES,NULL); 

	return 0;
}

int bill_process_region_query_bill(BILL_SVRENV *pstEnv,SSQUERYBILLREQ *pstQueryBillReq)
{
	BILL stBill;
	char szCallBackBuffer[MAX_MSG];
	int   iOffset = 0;

	memset(&stBill,0,sizeof(stBill));
	memset(szCallBackBuffer,0,sizeof(szCallBackBuffer));
	
	memcpy(szCallBackBuffer+iOffset,pstQueryBillReq,sizeof(SSQUERYBILLREQ) );
	iOffset += sizeof(SSQUERYBILLREQ);
	
	stBill.Uin = pstQueryBillReq->Uin;
	bill_db_select_req(pstEnv,&stBill,szCallBackBuffer,iOffset,DB_SELECT_BILL_QUERY);

	return 0;

}

int bill_process_region_dec_bill(BILL_SVRENV *pstEnv,    SSDECTIANYUANREQ *pstDecTianYuanReq)
{
	BILL stBill;
	char szCallBackBuffer[MAX_MSG];
	int   iOffset = 0;

	memset(&stBill,0,sizeof(stBill));
	memset(szCallBackBuffer,0,sizeof(szCallBackBuffer));
	
	memcpy(szCallBackBuffer+iOffset,pstDecTianYuanReq,sizeof(SSDECTIANYUANREQ) );
	iOffset += sizeof(SSDECTIANYUANREQ);
	

	stBill.Uin = pstDecTianYuanReq->Uin;
	bill_db_select_req(pstEnv,&stBill,szCallBackBuffer,iOffset,DB_SELECT_BILL_DEC);

	return 0;

}

int bill_process_region_user_query_bill(BILL_SVRENV *pstEnv,SSQUERYTIANYUANREQ *pstQueryTianYuanReq)
{
	BILL stBill;
	char szCallBackBuffer[MAX_MSG];
	int   iOffset = 0;

	memset(&stBill,0,sizeof(stBill));
	memset(szCallBackBuffer,0,sizeof(szCallBackBuffer));
	
	memcpy(szCallBackBuffer+iOffset,pstQueryTianYuanReq,sizeof(SSQUERYTIANYUANREQ) );
	iOffset += sizeof(SSQUERYTIANYUANREQ);
	

	stBill.Uin = pstQueryTianYuanReq->Uin;
	bill_db_select_req(pstEnv,&stBill,szCallBackBuffer,iOffset,DB_SELECT_USER_QUERY_BILL);

	return 0;

}

int bill_process_region_freeze_bill(BILL_SVRENV *pstEnv,SSBILLFREEZEPOINTREQ *pstBillFreezePointReq)
{
	BILL stBill;
	char szCallBackBuffer[MAX_MSG];
	int   iOffset = 0;

	memset(&stBill,0,sizeof(stBill));
	memset(szCallBackBuffer,0,sizeof(szCallBackBuffer));
	
	memcpy(szCallBackBuffer+iOffset,pstBillFreezePointReq,sizeof(SSQUERYTIANYUANREQ) );
	iOffset += sizeof(SSQUERYTIANYUANREQ);

	stBill.Uin = pstBillFreezePointReq->Uin;
	bill_db_select_req(pstEnv,&stBill,szCallBackBuffer,iOffset,DB_SELECT_USER_FREEZE_BILL);

	return 0;

}


int bill_process_region_msg(BILL_SVRENV *pstEnv,char *pszMsg,int iMsgLen )
{
	SSPKG stPkgReq;
	
	if( ss_unpack(pstEnv->pstSSHan, pszMsg, iMsgLen, &stPkgReq, 0) < 0 )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_unpack fail");
		return -1;
	}

	if( CASH_BILL_REQ == stPkgReq.Head.Cmd  )
	{
		//×öÒ»¸öËøµÄ´¦Àí
		if( NULL != z_find_bill_lock(  pstEnv, stPkgReq.Body.CashBillReq.Pass9Account) )
		{
			SSPKG stSSPkg;
			stSSPkg.Body.CashBillRes.Result = 99;
			stSSPkg.Body.CashBillRes.CashBillReq = stPkgReq.Body.CashBillReq;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,CASH_BILL_RES,NULL);
			return 0;
		}
		else
		{
			// ¼ÇÂ¼ÏÂËø
			z_insert_bill_lock(pstEnv,stPkgReq.Body.CashBillReq.Pass9Account);

			bill_process_accountdb_req(pstEnv, stPkgReq.Body.CashBillReq.Pass9Account,
							(char *)&stPkgReq,sizeof(stPkgReq.Head)+sizeof(stPkgReq.Body.CashBillReq));
			/* ³äÖµÇëÇó */
			//bill_process_region_cash_bill(pstEnv,&stPkgReq.Body.CashBillReq); 
		}
	}
	else if( QUERY_BILL_REQ == stPkgReq.Head.Cmd )
	{
		//×öÒ»¸öËøµÄ´¦Àí
		if( NULL != z_find_bill_lock(  pstEnv, stPkgReq.Body.QueryBillReq.Pass9Account) )
		{
			/* ÐèÒª»Ø¸ö³ö´í°ü */
			SSPKG stSSPkg;
			memset(&stSSPkg,0,sizeof(stSSPkg));
			stSSPkg.Body.QueryBillRes.Result = 99;
			stSSPkg.Body.QueryBillRes.MessageId = stPkgReq.Body.QueryBillReq.MessageId;
			stSSPkg.Body.QueryBillRes.Sequence = stPkgReq.Body.QueryBillReq.Sequence;
			stSSPkg.Body.QueryBillRes.QueryTs = time(NULL);
			bill_ss_backward_pkg(pstEnv,&stSSPkg,QUERY_BILL_RES,	NULL); 
			return 0;
		}
		else
		{
			// ¼ÇÂ¼ÏÂËø
			z_insert_bill_lock(pstEnv,stPkgReq.Body.QueryBillReq.Pass9Account);
			/* ²éÑ¯Óà¶î */
			//bill_process_region_query_bill(pstEnv,&stPkgReq.Body.QueryBillReq);
			bill_process_accountdb_req(pstEnv, stPkgReq.Body.QueryBillReq.Pass9Account,
							(char *)&stPkgReq,sizeof(stPkgReq.Head)+sizeof(stPkgReq.Body.QueryBillReq));
		}
	}
	else if(DEC_BILL_REQ == stPkgReq.Head.Cmd )
	{

		//×öÒ»¸öËøµÄ´¦Àí
		if( NULL != z_find_bill_lock(  pstEnv, stPkgReq.Body.DecTianYuanReq.Pass9Account) )
		{
			SSPKG stSSPkg;
			memset(&stSSPkg,0,sizeof(stSSPkg));
			stSSPkg.Body.DecTianYuanRes.Result = 1; /* ¿ÛÌìÔªÊ§°Ü */
			stSSPkg.Body.DecTianYuanRes.Uin = stPkgReq.Body.DecTianYuanReq.Uin;
			STRNCPY(stSSPkg.Body.DecTianYuanRes.BillRecord.Pass9Account,
				stPkgReq.Body.DecTianYuanReq.Pass9Account,
				CCH(stSSPkg.Body.DecTianYuanRes.BillRecord.Pass9Account));
			stSSPkg.Body.DecTianYuanRes.BillRecord.Uin = stPkgReq.Body.DecTianYuanReq.Uin;
			stSSPkg.Body.DecTianYuanRes.OperType = 	stPkgReq.Body.DecTianYuanReq.OperType;
			stSSPkg.Body.DecTianYuanRes.TianYuan = stPkgReq.Body.DecTianYuanReq.TianYuan;
			stSSPkg.Body.DecTianYuanRes.ExtraData = stPkgReq.Body.DecTianYuanReq.ExtraData;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,DEC_BILL_RES,NULL); 
			return 0;
		}
		else
		{
			// ¼ÇÂ¼ÏÂËø
			z_insert_bill_lock(pstEnv,stPkgReq.Body.DecTianYuanReq.Pass9Account);
			/* ¿ÛÇ®µÄÂß¼­ */
			bill_process_region_dec_bill(pstEnv,&stPkgReq.Body.DecTianYuanReq);
		}
	}
	else if(USER_QUERY_BILL_REQ == stPkgReq.Head.Cmd)
	{
		//×öÒ»¸öËøµÄ´¦Àí
		if( NULL != z_find_bill_lock(  pstEnv, stPkgReq.Body.QueryTianYuanReq.Pass9Account) )
		{
			/* ÐèÒª»Ø¸ö³ö´í°ü */
			SSPKG stSSPkg;
			memset(&stSSPkg,0,sizeof(stSSPkg));
			stSSPkg.Body.QueryTianYuanRes.Result = 1;
			stSSPkg.Body.QueryTianYuanRes.Uin = stPkgReq.Body.QueryTianYuanReq.Uin;
			stSSPkg.Body.QueryTianYuanRes.ExtraData = stPkgReq.Body.QueryTianYuanReq.ExtraData;
			stSSPkg.Body.QueryTianYuanRes.OperType = stPkgReq.Body.QueryTianYuanReq.OperType;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,USER_QUERY_BILL_RES,NULL); 
			return 0;
		}
		else
		{
			// ¼ÇÂ¼ÏÂËø
			z_insert_bill_lock(pstEnv,stPkgReq.Body.QueryTianYuanReq.Pass9Account);
			/* ²éÑ¯Óà¶î */
			bill_process_region_user_query_bill(pstEnv,&stPkgReq.Body.QueryTianYuanReq);
		}
	}
	else if(BILL_FREEZE_REQ == stPkgReq.Head.Cmd)
	{
		//×öÒ»¸öËøµÄ´¦Àí
		if( NULL != z_find_bill_lock(  pstEnv, stPkgReq.Body.BillFreezePointReq.Pass9Account) )
		{
			SSPKG stSSPkg;
			memset(&stSSPkg.Body.BillFreezePointRes,0,sizeof(stSSPkg.Body.BillFreezePointRes));
			stSSPkg.Body.BillFreezePointRes.Uin = stPkgReq.Body.BillFreezePointReq.Uin;
			stSSPkg.Body.BillFreezePointRes.OperType = stPkgReq.Body.BillFreezePointReq.OperType;
			stSSPkg.Body.BillFreezePointRes.ExtraData = stPkgReq.Body.BillFreezePointReq.ExtraData;
			STRNCPY(stSSPkg.Body.BillFreezePointRes.BillRecord.Pass9Account,stPkgReq.Body.BillFreezePointReq.Pass9Account,
				CCH(stPkgReq.Body.BillFreezePointReq.Pass9Account));
			stSSPkg.Body.BillFreezePointRes.BillRecord.Uin = stPkgReq.Body.BillFreezePointReq.Uin;
			/* ÐèÒª»Ø¸ö³ö´í°ü */
			stSSPkg.Body.BillFreezePointRes.Result = 1;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_FREEZE_RES,NULL); 
			return 0;
		}
		else
		{
			// ¼ÇÂ¼ÏÂËø
			z_insert_bill_lock(pstEnv,stPkgReq.Body.BillFreezePointReq.Pass9Account);
			/* ¶³½á»ò½â¶³ */
			bill_process_region_freeze_bill(pstEnv,&stPkgReq.Body.BillFreezePointReq);
		}
	}
	else if(BILL_QUERY_PROPSDEF_REQ == stPkgReq.Head.Cmd)
	{
		bill_process_region_get_propsdef(pstEnv,&stPkgReq.Body.BillQueryPropsDefReq);
	}
	else if(BILL_QUERY_PROPSGIFT_REQ == stPkgReq.Head.Cmd)
	{
		bill_db_select_req_propsgift(pstEnv,(char *)&stPkgReq.Body.BillQueryPropsGiftReq,sizeof(SSBILLQUERYPROPSGIFTREQ),
			stPkgReq.Body.BillQueryPropsGiftReq.Uin,QUERY_PROPSGIFT_BY_USER_QUERY);
	}
	else if(BILL_SET_PROPSGIFT_REQ == stPkgReq.Head.Cmd)
	{
		if( NULL != z_find_bill_lock(  pstEnv, stPkgReq.Body.BillSetPropsGiftReq.Pass9Account) )
		{
			SSPKG stSSPkg;
			memset(&stSSPkg.Body.BillSetPropsGiftRes,0,sizeof(stSSPkg.Body.BillSetPropsGiftRes));
			stSSPkg.Body.BillSetPropsGiftRes.Uin = stPkgReq.Body.BillSetPropsGiftReq.Uin;
			stSSPkg.Body.BillSetPropsGiftRes.Result = 1;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_SET_PROPSGIFT_RES,NULL); 
			return 0;
		}
		else
		{
			// ¼ÇÂ¼ÏÂËø
			z_insert_bill_lock(pstEnv,stPkgReq.Body.BillSetPropsGiftReq.Pass9Account);

			bill_db_select_req_propsgift(pstEnv,(char *)&stPkgReq.Body.BillSetPropsGiftReq,sizeof(SSBILLSETPROPSGIFTREQ),
			stPkgReq.Body.BillSetPropsGiftReq.Uin,QUERY_PROPSGIFT_BY_USER_SET);
		}
	}
	else if(BILL_CASH_PROPSGIFT_REQ == stPkgReq.Head.Cmd)
	{
		bill_props_gift_add_cash_to_region(pstEnv,&stPkgReq.Body.CashPropsGiftInfoReq);
		/*
		if( NULL != z_find_bill_lock(  pstEnv, stPkgReq.Body.CashPropsGiftInfoReq.Pass9Account) )
		{
			SSPKG stSSPkg;
			memset(&stSSPkg.Body.CashPropsGiftInfoRes,0,sizeof(stSSPkg.Body.CashPropsGiftInfoRes));
			STRNCPY(stSSPkg.Body.CashPropsGiftInfoRes.Pass9Account,stPkgReq.Body.CashPropsGiftInfoReq.Pass9Account,
				CCH(stSSPkg.Body.CashPropsGiftInfoRes.Pass9Account));
			stSSPkg.Body.CashPropsGiftInfoRes.Result = 1;
			stSSPkg.Body.CashPropsGiftInfoRes.CsConnCtx = stPkgReq.Body.CashPropsGiftInfoReq.CsConnCtx;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_CASH_PROPSGIFT_RES,NULL); 
			return 0;
		}
		else
		{
			// ¼ÇÂ¼ÏÂËø
			z_insert_bill_lock(pstEnv,stPkgReq.Body.CashPropsGiftInfoReq.Pass9Account);

			//bill_db_select_req_propsgift(pstEnv,(char *)&stPkgReq.Body.CashPropsGiftInfoReq,sizeof(SSCASHPROPSGIFTINFOREQ),
			//stPkgReq.Body.CashPropsGiftInfoReq.Uin,CASH_PROPSGIFT_BY_CS);
			bill_process_accountdb_req(pstEnv, stPkgReq.Body.CashPropsGiftInfoReq.Pass9Account,
							(char *)&stPkgReq,sizeof(stPkgReq.Head)+sizeof(stPkgReq.Body.CashPropsGiftInfoReq));
		}
		*/
	}
	else if(ACC_GIFT_LIST_REQ == stPkgReq.Head.Cmd)
	{
		// ²éÑ¯²»ÓÃ¼ÓËøÁË
		if (stPkgReq.Body.AccGiftListReq.ID > 0)
		{
			// ¼ÓËø todo	
			if( NULL != z_find_bill_lock(  pstEnv, stPkgReq.Body.AccGiftListReq.Pass9Account) )
			{
				return 0;
			}
			else
			{
				z_insert_bill_lock(pstEnv,stPkgReq.Body.AccGiftListReq.Pass9Account);
			}
		}
		bill_db_select_req_accgift(pstEnv,(char *)&stPkgReq.Body.AccGiftListReq,sizeof(stPkgReq.Body.AccGiftListReq),
									stPkgReq.Body.AccGiftListReq.Uin,QUERY_ACC_GIFT_INFO);
	}
	else if(ACC_GIFT_INSERT_REQ == stPkgReq.Head.Cmd)
	{
		// ²éÑ¯²»ÓÃ¼ÓËøÁË
		//if (stPkgReq.Body.AccGiftInsertReq.ID > 0)
		{
			// ¼ÓËø todo	
			if( NULL != z_find_bill_lock(  pstEnv, stPkgReq.Body.AccGiftInsertReq.Pass9Account) )
			{
				return 0;
			}
			else
			{
				z_insert_bill_lock(pstEnv,stPkgReq.Body.AccGiftInsertReq.Pass9Account);
			}
		}
		bill_db_select_req_accgift(pstEnv,(char *)&stPkgReq.Body.AccGiftInsertReq,sizeof(stPkgReq.Body.AccGiftInsertReq),
									stPkgReq.Body.AccGiftInsertReq.Uin,INSERT_ACC_GIFT_INFO);
	}
	else if (PROP_GIFT_REQ == stPkgReq.Head.Cmd)
	{
		if (stPkgReq.Body.PropGiftListReq.ID > 0)
		{
			int i;
			int iFound = 0;

			for (i = 0;i < pstEnv->iPropsDef; i++)
			{
				PROPSDEF *pstPropsDef = (PROPSDEF *)pstEnv->sPropsDef[i];
				if(pstPropsDef->GroupID == (unsigned int)stPkgReq.Body.PropGiftListReq.ID && 
					pstPropsDef->CashUp == stPkgReq.Body.PropGiftListReq.CashUp)
				{
					iFound = 1;
					break;
				}
			}

			if (!iFound)
			{
				bill_propsgift_clt(pstEnv, &stPkgReq.Body.PropGiftListReq,PROPS_GIFT_LIST_RET_NO_DEF);
				return 0;
			}				
			
			// ¼ÓËø
			if( NULL != z_find_bill_lock(  pstEnv, stPkgReq.Body.PropGiftListReq.Pass9Account) )
			{
				bill_propsgift_clt(pstEnv, &stPkgReq.Body.PropGiftListReq,PROPS_GIFT_LIST_RET_LOCK);
				return 0;
			}
			else
			{
				z_insert_bill_lock(pstEnv,stPkgReq.Body.PropGiftListReq.Pass9Account);
				if( NULL == z_find_bill_lock(  pstEnv, stPkgReq.Body.PropGiftListReq.Pass9Account) )
				{
					//Ã»Ëø³É¹¦
					bill_propsgift_clt(pstEnv, &stPkgReq.Body.PropGiftListReq,PROPS_GIFT_LIST_RET_LOCK);
					return 0; 
				}
			}

			
		}
		bill_db_select_req_propgift_clt(pstEnv,(char*)&stPkgReq.Body.PropGiftListReq,
									sizeof(stPkgReq.Body.PropGiftListReq),
									stPkgReq.Body.PropGiftListReq.Uin);
	}
	else if(SS_CDKEY_REQ == stPkgReq.Head.Cmd)
	{
		SSCDKEYREQ *pstReq = &stPkgReq.Body.SSCDKeyReq;
		
		if (NULL != z_find_cdkey_lock(pstEnv,pstReq->CDKey))
		{
			bill_ss_backward_pkg_cdkey(pstEnv,	SS_CDKEY_RES, CDKEY_FAIL_MANY,NULL,NULL,pstReq->RoleName,0);
			return 0;
		}
		else
		{
			z_insert_cdkey_lock(pstEnv,pstReq->CDKey);
			if (bill_db_select_cdkey_type(pstEnv,(char*)pstReq,sizeof(SSCDKEYREQ)) < 0)
			{
				bill_ss_backward_pkg_cdkey(pstEnv,	SS_CDKEY_RES, CDKEY_FAIL_SYS,NULL,NULL,pstReq->RoleName,1);	
			}
		}
	}
	else if(GODCOIN_CONSUME_LOG_REQ == stPkgReq.Head.Cmd)
	{
		SSGODCOINCONSUMELOGREQ *pstGodCoinConsumeLogReq = &stPkgReq.Body.GodCoinConsumeLogReq;

		/* ²åÈëÒ»¸ö¿ÛÇ®µÄ¼ÇÂ¼ */
		bill_db_insert_req(pstEnv,pstGodCoinConsumeLogReq,(char*)pstGodCoinConsumeLogReq,
									sizeof(SSGODCOINCONSUMELOGREQ),DB_INSERT_GODCOIN_CONSUME_RECORD);
	}
	return 0;
}

int bill_db_select_cdkey_type(BILL_SVRENV *pstEnv,char *pstData,int iLen)
{
	char szCallBackBuffer[MAX_MSG];
	int   iOffset = 0;

	memset(szCallBackBuffer,0,sizeof(szCallBackBuffer));
	memcpy(szCallBackBuffer+iOffset,pstData,iLen );
	iOffset += iLen;

	return bill_db_select_req(pstEnv,NULL,szCallBackBuffer,iOffset,DB_SELECT_CDKEYTYPEDEF);
}

int bill_db_select_req_propgift_clt(BILL_SVRENV *pstEnv,char *pstData,int iLen,int Uin)
{
	PROPSGIFT stPropGift;
	char szCallBackBuffer[MAX_MSG];
	int   iOffset = 0;
	SSPROPGIFTLISTREQ *pstReq = (SSPROPGIFTLISTREQ *)pstData;

	memset(&stPropGift,0,sizeof(stPropGift));
	memset(szCallBackBuffer,0,sizeof(szCallBackBuffer));

	//memcpy(szCallBackBuffer+iOffset,&OperType,sizeof(OperType) );
	//iOffset += sizeof(OperType);
	memcpy(szCallBackBuffer+iOffset,pstData,iLen );
	iOffset += iLen;

	stPropGift.Uin = Uin;
	stPropGift.GroupID = pstReq->ID;

	bill_db_select_req(pstEnv,&stPropGift,szCallBackBuffer,iOffset,DB_SELECT_PROPGIFT_CLT);
	return 0;
}

int bill_db_select_req_accgift(BILL_SVRENV *pstEnv,char *pstData,int iLen,int Uin,int OperType)
{
	ACCGIFT stAccGift;
	char szCallBackBuffer[MAX_MSG];
	int   iOffset = 0;

	memset(&stAccGift,0,sizeof(stAccGift));
	memset(szCallBackBuffer,0,sizeof(szCallBackBuffer));

	memcpy(szCallBackBuffer+iOffset,&OperType,sizeof(OperType) );
	iOffset += sizeof(OperType);
	memcpy(szCallBackBuffer+iOffset,pstData,iLen );
	iOffset += iLen;
	
	stAccGift.Uin = Uin;
	bill_db_select_req(pstEnv,&stAccGift,szCallBackBuffer,iOffset,DB_SELECT_ACCGIFT);

	return 0;
}


int bill_ss_backward_pkg_cdkey(BILL_SVRENV *pstEnv,	int iCmd, int iRet, 
									const char* pszCDKey,	CDKEYTYPEDEF *pstDef,const char *pszRoleName, int iUnLock)
{
	SSPKG stSSPkg;
	TDRDATA stNet;
	int iSrc = 0;
	int iDst = 0;


	stSSPkg.Body.SSCDKeyRes.CDKey[0] = 0;
	if( NULL != pszCDKey && iUnLock)
	{
		STRNCPY(stSSPkg.Body.SSCDKeyRes.CDKey, pszCDKey, sizeof(stSSPkg.Body.SSCDKeyRes.CDKey));
		z_remove_cdkey_lock(pstEnv, pszCDKey);
	}
	
	stSSPkg.Head.Cmd = iCmd;
	stSSPkg.Head.Magic = PROTOCOL_MAGIC; 
	stSSPkg.Head.Ver = TDR_METALIB_PROTO_SS_VERSION;
	stSSPkg.Head.HeadLen = sizeof(stSSPkg.Head);

	stSSPkg.Body.SSCDKeyRes.Succ = iRet;
	strncpy(stSSPkg.Body.SSCDKeyRes.RoleName, pszRoleName, sizeof(stSSPkg.Body.SSCDKeyRes.RoleName));
	memset(stSSPkg.Body.SSCDKeyRes.Items, 0, sizeof(stSSPkg.Body.SSCDKeyRes));
	
	if (pstDef)
	{
		memcpy(stSSPkg.Body.SSCDKeyRes.Items,pstDef->Items, sizeof(stSSPkg.Body.SSCDKeyRes.Items));
	}
	
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = pstEnv->szSendBuffer;

	if(ss_pack(pstEnv->pstSSHan, &stNet, &stSSPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "update cdkey ss_pack err");
		return -1;
	}

	iSrc = pstEnv->pstAppCtx->iId;
	iDst = 0;
	if ( tbus_backward(pstEnv->pstAppCtx->iBus, &iSrc, &iDst, stNet.pszBuff, stNet.iBuff, 0) != 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "update cdkey tbus_backward err");
		return -1;
	}
	
	return 0;
	
}

int bill_ss_backward_pkg(BILL_SVRENV *pstEnv,SSPKG *pstPkg, int iCmd,char *Pass9Account)
{
	
	TDRDATA stNet;
	int iRet = 0;
	int iSrc = 0;
	int iDst = 0;
	int iErr = 0;


	// Í³Ò»½âÏÂËø
	if( NULL != Pass9Account )
	{
		z_remove_bill_lock(pstEnv, Pass9Account);
	}
	
	pstPkg->Head.Cmd = iCmd;
	pstPkg->Head.Magic = PROTOCOL_MAGIC; 
	pstPkg->Head.Ver = TDR_METALIB_PROTO_SS_VERSION;
	pstPkg->Head.HeadLen = sizeof(pstPkg->Head);

	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = pstEnv->szSendBuffer;
	if((iErr = ss_pack(pstEnv->pstSSHan, &stNet, pstPkg, 0)) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail, reason : %s",
			         tdr_error_string(iErr));
		
		return -1;
	}

	iSrc = pstEnv->pstAppCtx->iId;
	iDst = 0;
	iRet = tbus_backward(pstEnv->pstAppCtx->iBus, &iSrc, &iDst, stNet.pszBuff, stNet.iBuff, 0);
	if (0 != iRet)
	{
		tlog_error(pstEnv->pstLogCat,0,0, "tbus_backward error ret %d", iRet);
	}
		
	return 0;
}


static int bill_db_select_where(BILL_SVRENV *pstEnv, int Uin, char* pstWhere,int iLen)
{
	int i;
	int iOffset = 0;
	int ID;

	snprintf(pstWhere,iLen,"where  Uin = %d &&", Uin);
	iOffset += strlen(pstWhere) ;
	
	for (i=0; i<pstEnv->iDefNum; i++)
	{
		ID = pstEnv->stPropsDef[i].GroupID;
		if (i == pstEnv->iDefNum -1)
		{
			snprintf(pstWhere+iOffset,iLen-iOffset,"GroupID=%d", ID);
		}
		else
		{
			snprintf(pstWhere+iOffset,iLen-iOffset,"GroupID=%d ||", ID);
		}
		iOffset += (strlen(pstWhere)-iOffset);
	}
	
	return 0;
}

int bill_db_select_req(BILL_SVRENV *pstEnv,void *pstData,char *szCallBackBuffer,int iLen,int iFlag)
{
	TORMPKG stPkg;
	TORMSELECTREQ *pstDBReq = &stPkg.Body.SelectReq;

	pstDBReq->OffSet = 0;
	pstDBReq->Limit = 0;
	
	if( iFlag == DB_SELECT_BILL_CASH || 
		iFlag == DB_SELECT_BILL_QUERY || 
		iFlag == DB_SELECT_BILL_DEC ||
		iFlag == DB_SELECT_USER_QUERY_BILL ||
		iFlag == DB_SELECT_USER_FREEZE_BILL)
	{
		BILL *pstBill;
		pstBill = (BILL *)pstData;
		if(iFlag == DB_SELECT_BILL_CASH)
		{
			bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_SELECT_REQ,DB_SELECT_BILL_CASH, 
				               "Bill", szCallBackBuffer, iLen);
		}
		else if(iFlag == DB_SELECT_BILL_QUERY)
		{
			bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_SELECT_REQ,DB_SELECT_BILL_QUERY, 
		               "Bill", szCallBackBuffer, iLen);
		}
		else if(iFlag == DB_SELECT_BILL_DEC)
		{
			bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_SELECT_REQ,DB_SELECT_BILL_DEC, 
		               "Bill", szCallBackBuffer, iLen);
		}
		else if(iFlag == DB_SELECT_USER_QUERY_BILL)
		{
			bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_SELECT_REQ,DB_SELECT_USER_QUERY_BILL, 
		               "Bill", szCallBackBuffer, iLen);
		}
		else if (iFlag ==  DB_SELECT_USER_FREEZE_BILL)
		{
			bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_SELECT_REQ,DB_SELECT_USER_FREEZE_BILL, 
		               "Bill", szCallBackBuffer, iLen);		
		}
		
		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		snprintf(pstDBReq->OperInfo.WhereDef,
		         sizeof(pstDBReq->OperInfo.WhereDef),
		         "where Uin = '%d'", 
			       pstBill->Uin);
		
		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,pstBill, sizeof(BILL));		
	}
	else if(iFlag == DB_SELECT_BILL_CASH_RECORD)
	{
		/* Èç¹ûÊÇ³äÖµÊ×ÏÈÒªÏÈÈ·¶¨ÏÂ ¸ÃÁ÷Ë®ÊÇ·ñÒÑ¾­³äÖµ¹ý*/
		SSCASHBILLREQ *pstCashBillReq;
		ADDBILLRECORD  stAddBillRecord;

		memset(&stAddBillRecord,0,sizeof(stAddBillRecord));
		
		pstCashBillReq = (SSCASHBILLREQ *)pstData;
		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_SELECT_REQ,DB_SELECT_BILL_CASH_RECORD, 
	               "AddBillRecord", szCallBackBuffer, iLen);
		
		//stAddBillRecord.MessageId = pstCashBillReq->MessageId;
		//stAddBillRecord.Sequence = pstCashBillReq->Sequence;
		stAddBillRecord.Uin =pstCashBillReq->Uin;
		stAddBillRecord.ServerID = pstCashBillReq->ServerID;
		
		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		snprintf(pstDBReq->OperInfo.WhereDef,
		         sizeof(pstDBReq->OperInfo.WhereDef),
		         "where Uin = %d and Sequence =%s ", 
		         	stAddBillRecord.Uin,
			       stAddBillRecord.Sequence );
		
		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,&stAddBillRecord, sizeof(ADDBILLRECORD));
	}
	else if( iFlag == DB_SELECT_PROPSGIFT )
	{
		PROPSGIFT *pstPropsGift;
		pstPropsGift = (PROPSGIFT *)pstData;
		
		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_SELECT_REQ,DB_SELECT_PROPSGIFT, 
		               "PropsGift", szCallBackBuffer, iLen);
		
		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstDBReq->Limit = PROPS_LIMIT_MONTH;

		bill_db_select_where(pstEnv, pstPropsGift->Uin,
							pstDBReq->OperInfo.WhereDef, 
							sizeof(pstDBReq->OperInfo.WhereDef));
		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,pstPropsGift, sizeof(PROPSGIFT));	
	}
	else if (iFlag == DB_SELECT_ACCGIFT)
	{
		ACCGIFT *pstAccGift = NULL;
		SSACCGIFTLISTREQ *pstAccGiftListReq;
		SSACCGIFTINSERTREQ *pstAccGiftInsertReq = NULL;		
		int iOpType;
		
		pstAccGift = (ACCGIFT*)pstData;
		iOpType = *((int*)szCallBackBuffer);


		pstAccGiftListReq = (SSACCGIFTLISTREQ *)(szCallBackBuffer+sizeof(int));
		
		if (iOpType == INSERT_ACC_GIFT_INFO)
		{
			pstAccGiftInsertReq = (SSACCGIFTINSERTREQ *)(szCallBackBuffer+sizeof(int));
			pstAccGift = &pstAccGiftInsertReq->AccGift;
		}
		
		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_SELECT_REQ,DB_SELECT_ACCGIFT, 
		               "AccGift", szCallBackBuffer, iLen);

		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		// acc gift insert
		
		if (iOpType == INSERT_ACC_GIFT_INFO)
		{
			//²éÑ¯Ö¸¶¨Ãû×ÖµÄÀñ°ü
			if (pstAccGift)
			{
				snprintf(pstDBReq->OperInfo.WhereDef,
				        		 sizeof(pstDBReq->OperInfo.WhereDef),
				         		"where Uin = %u && Name = '%s'", 
					       	pstAccGift->Uin, pstAccGift->Name);
			}
			else
			{
				//return -1;
			}
		}
		else
		{
			if (pstAccGiftListReq->ID == 0)
			{
				pstDBReq->Limit = MAX_ACC_GIFT;
				snprintf(pstDBReq->OperInfo.WhereDef,
			        		 sizeof(pstDBReq->OperInfo.WhereDef),
			         		"where Uin = %u && Flag=0 && (RoleID = 0 || RoleID = %llu) order by ID desc", 
				       	pstAccGift->Uin,pstAccGiftListReq->RoleID);
				/*ÅÐ¶ÏÊ±¼ä¶¨Ê±·¢²¼
				tdr_datetime_t t;
				tdr_utctime_to_tdrdatetime (&t, time(NULL));
				char *pszTime = tdr_tdrdatetime_to_str(&t);
				snprintf(pstDBReq->OperInfo.WhereDef,
			        	sizeof(pstDBReq->OperInfo.WhereDef),
			         	"where Uin = %u && Flag=0 && (RoleID = 0 || RoleID = %llu) && SetGiftTime <= '%s' order by ID desc", 
				       	pstAccGift->Uin,pstAccGiftListReq->RoleID,pszTime);
				*/       	
			}
			else
			{
				snprintf(pstDBReq->OperInfo.WhereDef,
			        		 sizeof(pstDBReq->OperInfo.WhereDef),
			         		"where Uin = %u && Flag = 0 && ID = %d && (RoleID = 0 || RoleID = %llu)", 
				       	pstAccGift->Uin, pstAccGiftListReq->ID, pstAccGiftListReq->RoleID);
			}
		}
		
		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,pstAccGift, sizeof(ACCGIFT));	
	}
	else if (DB_SELECT_PROPGIFT_CLT == iFlag)
	{
		PROPSGIFT *pstPropsGift;
		SSPROPGIFTLISTREQ *pstPropGiftListReq;
		
		pstPropsGift = (PROPSGIFT*)pstData;
		pstPropGiftListReq = (SSPROPGIFTLISTREQ *)(szCallBackBuffer);
		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_SELECT_REQ,iFlag, 
		               "PropsGift", szCallBackBuffer, iLen);

		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		if (pstPropGiftListReq->ID == 0)
		{
			pstDBReq->Limit = PROPS_LIMIT_MONTH;
			snprintf(pstDBReq->OperInfo.WhereDef,
		        		 sizeof(pstDBReq->OperInfo.WhereDef),
		         		"where Uin = %d order by LastCashTime desc", 
			       	pstPropGiftListReq->Uin);
		}
		else
		{
			snprintf(pstDBReq->OperInfo.WhereDef,
		        		 sizeof(pstDBReq->OperInfo.WhereDef),
		         		"where Uin = %u && GroupID=%d", 
			       	pstPropsGift->Uin, pstPropGiftListReq->ID);
		}
		
		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,pstPropsGift, sizeof(PROPSGIFT));	
	}
	else if(DB_SELECT_CDKEYTYPEDEF == iFlag)
	{
		CDKEYTYPEDEF stCDkeyType;
		//SSCDKEYREQ *pstReq = (SSCDKEYREQ *)(szCallBackBuffer);
		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_SELECT_REQ,iFlag, 
		               "CDKeyTypeDef", szCallBackBuffer, iLen);

		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		snprintf(pstDBReq->OperInfo.WhereDef,
		        	sizeof(pstDBReq->OperInfo.WhereDef),
		         	"where KeyType!=''");

		memset(&stCDkeyType, 0, sizeof(stCDkeyType));
		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,&stCDkeyType, sizeof(stCDkeyType));	
	}
	else if(DB_SELECT_CDKEYDEF_COUNT == iFlag)
	{
		CDKEYDEF *pstCDKeyDef;
		int iOffset = 0;
		SSCDKEYREQ *pstSSReq;
		CDKEYTYPEDEF *pstTypeDef;
		TORMPKG stPkg;
		TORMSELECTCOUNTREQ *pstSelectCountReq;
		
		pstCDKeyDef = (CDKEYDEF*)pstData;
		pstSSReq = (SSCDKEYREQ *)(szCallBackBuffer);
		iOffset += sizeof(SSCDKEYREQ);
		pstTypeDef = (CDKEYTYPEDEF *)(szCallBackBuffer+iOffset);

		
		pstSelectCountReq = &stPkg.Body.SelectCountReq;
		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_SELECT_COUNT_REQ,iFlag, 
		               "CDKeyDef", szCallBackBuffer, iLen);

		snprintf(pstSelectCountReq->WhereDef,
		        	sizeof(pstSelectCountReq->WhereDef),
		         	"where CDKey like '%s%c' &&  RoleID =%llu", 
			      pstTypeDef->KeyType,'%',
			      pstSSReq->RoleID);
		
		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,pstCDKeyDef, sizeof(CDKEYDEF));	
	}
	else if (DB_SELECT_CDKEYDEF == iFlag)
	{
		CDKEYDEF *pstCDKeyDef;		
		SSCDKEYREQ *pstSSReq = (SSCDKEYREQ *)(szCallBackBuffer);

		pstCDKeyDef = (CDKEYDEF*)pstData;

		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_SELECT_REQ,iFlag, 
		               "CDKeyDef", szCallBackBuffer, iLen);

		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstDBReq->Limit = 1;
		snprintf(pstDBReq->OperInfo.WhereDef,
	        		 sizeof(pstDBReq->OperInfo.WhereDef),
	         		"where CDKey = '%s'", 
		       	pstSSReq->CDKey);
		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,pstCDKeyDef, sizeof(CDKEYDEF));	
	}
	else if(DB_SELECT_CDKEYDEF_GET == iFlag)
	{
		SSCDKEYREQ *pstSSReq = (SSCDKEYREQ *)(szCallBackBuffer);
		CDKEYGET stGet;
		
		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_SELECT_REQ,iFlag, 
		               "CDKeyGet", szCallBackBuffer, iLen);	

		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstDBReq->Limit = 1;
		snprintf(pstDBReq->OperInfo.WhereDef,
	        		 sizeof(pstDBReq->OperInfo.WhereDef),
	         		"where CDKey = '%s' && RoleID =%llu", 
		       	pstSSReq->CDKey, pstSSReq->RoleID);

		memset(&stGet, 0, sizeof(stGet));
		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,&stGet, sizeof(stGet));	
	}

	return 0;
}

int bill_db_insert_req(BILL_SVRENV *pstEnv,void *pstData,char *szCallBackBuffer,int iLen,int iFlag)
{
	TORMPKG stPkg;
	TORMINSERTREQ *pstDBReq = &stPkg.Body.InsertReq;

	if( DB_INSERT_BILL_CASH == iFlag)
	{
		BILL *pstBill;
		pstBill = (BILL *)pstData;
		if(pstBill->Uin == 0 )
		{
			tlog_error(pstEnv->pstLogCat,0,0,"bill_db_insert_req Bill fail key is null");
			return -1;
		}		
		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_INSERT_REQ,DB_INSERT_BILL_CASH, 
				               "Bill", szCallBackBuffer, iLen);
		
		//pstDBReq->DoInsert = 0;
		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstDBReq->OperInfo.WhereDef[0] = 0;

		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,pstBill, sizeof(BILL));
	}
	else if(DB_INSERT_BILL_ADD == iFlag )
	{
		BILL *pstBill;
		pstBill = (BILL *)pstData;		
		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_INSERT_REQ,DB_INSERT_BILL_ADD, 
				               "Bill", szCallBackBuffer, iLen);
		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstDBReq->OperInfo.WhereDef[0] = 0;
		
		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,pstBill, sizeof(BILL));
	}
	else if( DB_INSERT_PROPSGIFT == iFlag)
	{
		PROPSGIFT *pstPropGift;
		pstPropGift = (PROPSGIFT *)pstData;
		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_INSERT_REQ,DB_INSERT_PROPSGIFT, 
				               "PropsGift", szCallBackBuffer, iLen);

		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstDBReq->OperInfo.WhereDef[0] = 0;
		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,pstPropGift, sizeof(*pstPropGift));
		
	}
	else if(DB_INSERT_BILL_CASH_RECORD== iFlag )
	{
		SSCASHBILLREQ *pstCashBillReq;
		ADDBILLRECORD stAddBillRecord;
		
		pstCashBillReq = (SSCASHBILLREQ *)pstData;		
		memset(&stAddBillRecord,0,sizeof(stAddBillRecord));

		//stAddBillRecord.Sequence = pstCashBillReq->Sequence;
		//stAddBillRecord.MessageId = pstCashBillReq->MessageId;
		stAddBillRecord.Uin = pstCashBillReq->Uin;
		stAddBillRecord.ServerID = pstCashBillReq->ServerID;
		stAddBillRecord.Amount = pstCashBillReq->Amount;
		stAddBillRecord.Points = pstCashBillReq->Points;
		stAddBillRecord.Bonus = pstCashBillReq->Bonus;
		stAddBillRecord.PropsID = pstCashBillReq->PropsID;
		//stAddBillRecord.LogTime= time(NULL);
		tdr_utctime_to_tdrdatetime (&stAddBillRecord.LogTime, time(NULL));
		
		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_INSERT_REQ,DB_INSERT_BILL_CASH_RECORD, 
				               "AddBillRecord", szCallBackBuffer, iLen);
		
		//pstDBReq->DoInsert = 0;
		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstDBReq->OperInfo.WhereDef[0] = 0;

		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,&stAddBillRecord, sizeof(ADDBILLRECORD));			
	}
	else if( DB_INSERT_BILL_DEC_RECORD == iFlag )
	{
		SSDECTIANYUANRES *pstDecTianYuanRes;
		DECBILLRECORD stDecBillRecord;

		memset(&stDecBillRecord,0,sizeof(stDecBillRecord));
		pstDecTianYuanRes = (SSDECTIANYUANRES *)pstData;

		/*
		if( DATATYPE_POINT_BY_SHOPPING == pstDecTianYuanRes->ExtraData.DataType )
		{
			CSSHOPPINGBUYREQ *pstShoppingBuyReq;
			pstShoppingBuyReq = (CSSHOPPINGBUYREQ *)(pstDecTianYuanRes->ExtraData.ExtraData);
			stDecBillRecord.RoleID = pstShoppingBuyReq->RoleID;

			stDecBillRecord.ShopID = pstShoppingBuyReq->ShoppingID;
			stDecBillRecord.ShopNum = pstShoppingBuyReq->ShoppingNum;
			stDecBillRecord.GiftFlag = pstShoppingBuyReq->GiftFlag;
			stDecBillRecord.GiftToRoleID = pstShoppingBuyReq->GiftToRoleID;
		}*/
		
		stDecBillRecord.OperType = pstDecTianYuanRes->OperType;
		stDecBillRecord.Uin = pstDecTianYuanRes->Uin;
		stDecBillRecord.WorldID = pstDecTianYuanRes->WorldID;

		if (pstDecTianYuanRes->ExtraData.DataType == DATATYPE_POINT_BY_EXCHANGE)
		{
			CSSHOPPINGEXCHANGEREQ *pstExchangeReq = (CSSHOPPINGEXCHANGEREQ *)pstDecTianYuanRes->ExtraData.ExtraData;
			
			stDecBillRecord.RoleID = pstExchangeReq->RoleID;
			stDecBillRecord.RoleLevel = pstExchangeReq->RoleLevel;
		}
		stDecBillRecord.GodPointNum = pstDecTianYuanRes->TianYuan;
		tdr_utctime_to_tdrdatetime (&stDecBillRecord.LogTime, time(NULL));
		
		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_INSERT_REQ,DB_INSERT_BILL_DEC_RECORD, 
				               "DecBillRecord", szCallBackBuffer, iLen);
		
		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstDBReq->OperInfo.WhereDef[0] = 0;

		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,&stDecBillRecord, sizeof(DECBILLRECORD));		
	}
	else if( DB_INSERT_GODCOIN_CONSUME_RECORD == iFlag )
	{
		SSGODCOINCONSUMELOGREQ *pstGodCoinConsumeLogReq;
		GODCOINCONSUMERECORD stGodCoinConsumeRecord;

		memset(&stGodCoinConsumeRecord,0,sizeof(stGodCoinConsumeRecord));
		pstGodCoinConsumeLogReq = (SSGODCOINCONSUMELOGREQ *)pstData;
 
		stGodCoinConsumeRecord.Uin = pstGodCoinConsumeLogReq->Uin;
		stGodCoinConsumeRecord.RoleID = pstGodCoinConsumeLogReq->RoleID;
		stGodCoinConsumeRecord.RoleLevel = pstGodCoinConsumeLogReq->RoleLevel;
		stGodCoinConsumeRecord.WorldID = pstGodCoinConsumeLogReq->WorldID;
		stGodCoinConsumeRecord.OperType = pstGodCoinConsumeLogReq->OperType;
		stGodCoinConsumeRecord.OperSubType = pstGodCoinConsumeLogReq->OperSubType;
		stGodCoinConsumeRecord.GodCoin = pstGodCoinConsumeLogReq->GodCoin;
		stGodCoinConsumeRecord.ShopID = pstGodCoinConsumeLogReq->ShopID;
		stGodCoinConsumeRecord.ShopNum = pstGodCoinConsumeLogReq->ShopNum;
		STRNCPY(stGodCoinConsumeRecord.OperDesc,pstGodCoinConsumeLogReq->OperDesc,sizeof(stGodCoinConsumeRecord.OperDesc));
		tdr_utctime_to_tdrdatetime (&stGodCoinConsumeRecord.LogTime, time(NULL));
		
		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_INSERT_REQ,DB_INSERT_GODCOIN_CONSUME_RECORD, 
				               "GodCoinConsumeRecord", szCallBackBuffer, iLen);
		
		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstDBReq->OperInfo.WhereDef[0] = 0;

		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg, &stGodCoinConsumeRecord, sizeof(stGodCoinConsumeRecord));		
	}
	else if (DB_INSERT_CDKEY_GET == iFlag)
	{
		CDKEYGET *pstGet;
		pstGet = (CDKEYGET *)pstData;		
		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_INSERT_REQ,DB_INSERT_CDKEY_GET, 
				               "CDKeyGet", szCallBackBuffer, iLen);
		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstDBReq->OperInfo.WhereDef[0] = 0;
		
		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,pstGet, sizeof(*pstGet));		
	}
	else if (DB_INSERT_ACCGIFT == iFlag)
	{	
		ACCGIFT *pstAccGift;
		pstAccGift = (ACCGIFT *)pstData;	
		
		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_INSERT_REQ,DB_INSERT_ACCGIFT, 
				               "AccGift", szCallBackBuffer, iLen);
		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstDBReq->OperInfo.WhereDef[0] = 0;
		
		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg, pstAccGift, sizeof(*pstAccGift));		
	}


	return 0;
}

int bill_db_update_req(BILL_SVRENV *pstEnv,void *pstData,char *szCallBackBuffer,int iLen,int iFlag)
{
	TORMPKG stPkg;
	TORMUPDATEREQ *pstDBReq = &stPkg.Body.UpdateReq;

	if( DB_UPDATE_BILL == iFlag ||
		DB_UPDATE_BILL_DEC == iFlag ||
		DB_UPDATE_USER_FREEZE_BILL == iFlag )
	{
		BILL *pstBill;
		pstBill = (BILL *)pstData;
		if( DB_UPDATE_BILL == iFlag )
		{

			bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_UPDATE_REQ,DB_UPDATE_BILL, 
					               "Bill", szCallBackBuffer, iLen);
			
		}
		else if( DB_UPDATE_BILL_DEC == iFlag )
		{
			bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_UPDATE_REQ,DB_UPDATE_BILL_DEC, 
					               "Bill", szCallBackBuffer, iLen);
			
		}
		else if(DB_UPDATE_USER_FREEZE_BILL == iFlag)
		{
			bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_UPDATE_REQ,DB_UPDATE_USER_FREEZE_BILL, 
					               "Bill", szCallBackBuffer, iLen);
			
		}
		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		snprintf(pstDBReq->OperInfo.WhereDef,
		         sizeof(pstDBReq->OperInfo.WhereDef),
		         "where Uin = %d", pstBill->Uin);

		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,pstBill, sizeof(BILL));
	}
	else if(DB_UPDATE_PROPSGIFT == iFlag )
	{
		PROPSGIFT *pstPropsGift;
		pstPropsGift = (PROPSGIFT *)pstData;

		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_UPDATE_REQ,DB_UPDATE_PROPSGIFT, 
					               "PropsGift", szCallBackBuffer, iLen);

		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstDBReq->DoInsert =  0;//²»´æÔÚ×ö²åÈë²Ù×÷
		pstDBReq->OperInfo.WhereDef[0] = 0;
		snprintf(pstDBReq->OperInfo.WhereDef,
		         sizeof(pstDBReq->OperInfo.WhereDef),
		         "where GroupID = '%u' and Uin = %u", 
			       pstPropsGift->GroupID,pstPropsGift->Uin);

		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,pstPropsGift, sizeof(PROPSGIFT));
		
	}
	else if (DB_UPDATE_ACCGIFT == iFlag)
	{
		ACCGIFT *pstAccgift = (ACCGIFT *)pstData;
		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_UPDATE_REQ,DB_UPDATE_ACCGIFT, 
					               "AccGift", szCallBackBuffer, iLen);

		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstDBReq->DoInsert =  0;
		pstDBReq->OperInfo.WhereDef[0] = 0;
		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,pstAccgift, sizeof(*pstAccgift));
	}
	else if (DB_UPDATE_PROPGIFT_CLT == iFlag)
	{
		PROPSGIFT *pstPropsgift = (PROPSGIFT *)pstData;
		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_UPDATE_REQ,iFlag, 
					               "PropsGift", szCallBackBuffer, iLen);

		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstDBReq->DoInsert =  0;
		pstDBReq->OperInfo.WhereDef[0] = 0;

		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,pstPropsgift, sizeof(*pstPropsgift));	

	}
	else if (DB_UPDATE_CDKEY_DEF == iFlag)
	{
		CDKEYDEF *pstDef = (CDKEYDEF *)pstData;
		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_UPDATE_REQ,iFlag, 
					               "CDKeyDef", szCallBackBuffer, iLen);
		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstDBReq->DoInsert =  0;
		pstDBReq->OperInfo.WhereDef[0] = 0;

		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,pstDef, sizeof(*pstDef));	
		
	}
	
	return 0;
}

int bill_db_delete_req(BILL_SVRENV *pstEnv,void *pstData,char *szCallBackBuffer,int iLen,int iFlag)
{
	UNUSED(pstEnv);
	UNUSED(pstData);
	UNUSED(szCallBackBuffer);
	UNUSED(&iLen);
	UNUSED(&iFlag);
	/*TORMPKG stPkg;
	TORMDELETEREQ *pstDBReq = &stPkg.Body.DeleteReq;

	if( iFlag == DB_DELETE_PROPS)
	{
		PROPS *pstProps;
		pstProps = (PROPS *)pstData;
		bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_DELETE_REQ,DB_DELETE_PROPS, 
				               "Bill", szCallBackBuffer, iLen);
		
		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
		snprintf(pstDBReq->OperInfo.WhereDef,
		         sizeof(pstDBReq->OperInfo.WhereDef),
		         "where seqID=%llu and Pass9Account = '%s' and PropsID=%llu", 
		         	pstProps->seqID,
			       pstProps->Pass9Account,
			       pstProps->PropsID);

		return bill_db_send_req(pstEnv->pstAppCtx, pstEnv,&stPkg,pstProps, sizeof(PROPS));
	}*/
	
	return 0;
}
int bill_db_select_res_dec(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
	int iCallBackCmd = 0;	
	int iOffset = 0;
	TORMSELECTRES * pstSelectRes;
	unsigned long long  ullTotalTianYuan = 0;
	int iTmp = 0;
	unsigned long long ullPoint = 0;
	time_t  tCurr = time(NULL);
	char AsynCallBackData[TORM_MAX_ASYNCALLBACK_DATA_LEN]; 
	BILL stBill;
	SSDECTIANYUANREQ *pstSSDecTianYuanReq;
	SSPKG stSSPkg;
	
	memset(AsynCallBackData,0,sizeof(AsynCallBackData));
	
	pstSelectRes = &pstPkg->Body.SelectRes;
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);

	if( DB_SELECT_BILL_DEC != iCallBackCmd )
	{
		return -1;
	}
	
	memset(&stBill,0,sizeof(stBill));
	pstSSDecTianYuanReq = (SSDECTIANYUANREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
	iOffset += sizeof(SSDECTIANYUANREQ);

	/* ÐèÒª»Ø¸ö³ö´í°ü */
	memset(&stSSPkg.Body.DecTianYuanRes, 0, sizeof(stSSPkg.Body.DecTianYuanRes));		
	if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			         "Select Bill info from db failed. DBErr = %d, Uin = %u, Msg = %s.", 
			         pstSelectRes->DBMSErrNo,
			         pstSSDecTianYuanReq->Uin,
			         torm_error_string(pstSelectRes->ErrNo));

		stSSPkg.Body.DecTianYuanRes.Result = BILL_DEC_ERR_SYSTEM;/* ÏµÍ³·±Ã¦ */
	}
	else if(pstSelectRes->SelectResult.ResultNum == 0 &&
			OPERATE_POINT_ADD == pstSSDecTianYuanReq->OperType )
	{
		/* Èç¹ûÊÇzone_svrÀ´¼ÓµãÈ¯µÄ»°,ºóÃæÀ´´¦Àí*/
		stSSPkg.Body.DecTianYuanRes.Result = BILL_DEC_NO_CASH;/* »¹Ã»ÓÐ³äÖµ¼ÇÂ¼ */		
	}
	else if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stBill, sizeof(stBill))
		 < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"torm_fetch_one_meta fail,Uin = %u,",
			pstSSDecTianYuanReq->Uin);
		stSSPkg.Body.DecTianYuanRes.Result = BILL_DEC_ERR_SYSTEM; /* ½âÂëÊ§°Ü  */
	}

	if( tCurr >= stBill.FreezeLastTime +  MAX_TRADE_SHOW_TIME*3600 )
	{
		/* ³¬¹ýÁË24Ð¡Ê±ÁË */
		stBill.FreezePoint = 0;
		stBill.FreezeLastTime = 0;
	}
	if( 0 != stSSPkg.Body.DecTianYuanRes.Result && 
		BILL_DEC_NO_CASH != stSSPkg.Body.DecTianYuanRes.Result )
	{
		stSSPkg.Body.DecTianYuanRes.Uin = pstSSDecTianYuanReq->Uin;
		STRNCPY(stSSPkg.Body.DecTianYuanRes.BillRecord.Pass9Account,
		pstSSDecTianYuanReq->Pass9Account,CCH(stSSPkg.Body.DecTianYuanRes.BillRecord.Pass9Account));
		stSSPkg.Body.DecTianYuanRes.BillRecord.Uin = stBill.Uin;
		stSSPkg.Body.DecTianYuanRes.BillRecord.ServerID = stBill.ServerID;
		stSSPkg.Body.DecTianYuanRes.BillRecord.Amount = stBill.Amount;
		stSSPkg.Body.DecTianYuanRes.BillRecord.Points = stBill.Points;
		stSSPkg.Body.DecTianYuanRes.BillRecord.Bonus = stBill.Bonus;
		stSSPkg.Body.DecTianYuanRes.BillRecord.FreezePoint = stBill.FreezePoint;
		stSSPkg.Body.DecTianYuanRes.BillRecord.FreezeLastTime = stBill.FreezeLastTime;
		stSSPkg.Body.DecTianYuanRes.OperType = 	pstSSDecTianYuanReq->OperType;
		stSSPkg.Body.DecTianYuanRes.TianYuan = pstSSDecTianYuanReq->TianYuan;
		stSSPkg.Body.DecTianYuanRes.ExtraData = pstSSDecTianYuanReq->ExtraData;
			
		bill_ss_backward_pkg(pstEnv,&stSSPkg,DEC_BILL_RES,pstSSDecTianYuanReq->Pass9Account);
		return -1;
	}

 	ullPoint = stBill.Bonus + stBill.Points;
	if(tCurr < stBill.FreezeLastTime +  MAX_TRADE_SHOW_TIME*3600 )
	{
		/* ÓÐ¶³½áµÄÇ®ÔÚÕâÀï */
		if( ullPoint > stBill.FreezePoint )
		{
			ullPoint = ullPoint - stBill.FreezePoint;
		}
		else
		{
			ullPoint = 0;
		}
	}
	if( OPERATE_POINT_DEC == pstSSDecTianYuanReq->OperType)
	{
		if( DATATYPE_POINT_BY_SHOPPING != pstSSDecTianYuanReq->ExtraData.DataType)
		{
			// ÏÈ½â¶³´¦ÀíÏÂ
			if( stBill.FreezePoint >=  pstSSDecTianYuanReq->TianYuan )
			{
				stBill.FreezePoint -= pstSSDecTianYuanReq->TianYuan;
				ullPoint +=   pstSSDecTianYuanReq->TianYuan;
			}
			else
			{
				ullPoint +=   stBill.FreezePoint;
				stBill.FreezePoint = 0;
				tlog_error(pstEnv->pstLogCat, 0, 0,	"please check your code, something is error! Uin:%u",
						stBill.Uin);
			}
		}

	
		if(ullPoint < pstSSDecTianYuanReq->TianYuan )
		{
			stSSPkg.Body.DecTianYuanRes.Result = BILL_DEC_NO_ENOUGH_CASH;/* Óà¶î²»×ã */
			stSSPkg.Body.DecTianYuanRes.Uin = pstSSDecTianYuanReq->Uin;
			STRNCPY(stSSPkg.Body.DecTianYuanRes.BillRecord.Pass9Account,
			pstSSDecTianYuanReq->Pass9Account,CCH(stSSPkg.Body.DecTianYuanRes.BillRecord.Pass9Account));
			stSSPkg.Body.DecTianYuanRes.BillRecord.Uin = stBill.Uin;
			stSSPkg.Body.DecTianYuanRes.BillRecord.ServerID = stBill.ServerID;
			stSSPkg.Body.DecTianYuanRes.BillRecord.Amount = stBill.Amount;
			stSSPkg.Body.DecTianYuanRes.BillRecord.Points = stBill.Points;
			stSSPkg.Body.DecTianYuanRes.BillRecord.Bonus = stBill.Bonus;
			stSSPkg.Body.DecTianYuanRes.BillRecord.FreezePoint = stBill.FreezePoint;
			stSSPkg.Body.DecTianYuanRes.BillRecord.FreezeLastTime = stBill.FreezeLastTime;			
			stSSPkg.Body.DecTianYuanRes.ExtraData = pstSSDecTianYuanReq->ExtraData;
			stSSPkg.Body.DecTianYuanRes.OperType = 	pstSSDecTianYuanReq->OperType;
			stSSPkg.Body.DecTianYuanRes.TianYuan = pstSSDecTianYuanReq->TianYuan;
				
			bill_ss_backward_pkg(pstEnv,&stSSPkg,DEC_BILL_RES,pstSSDecTianYuanReq->Pass9Account);
			return -1;
		}

		/* ¿ªÊ¼¿ÛÇ® */
		ullTotalTianYuan  = pstSSDecTianYuanReq->TianYuan;
		if( ullTotalTianYuan > 0 &&
			stBill.Bonus > 0 && 
			stBill.Bonus >= ullTotalTianYuan)
		{
			stBill.Bonus -= ullTotalTianYuan;
			ullTotalTianYuan = 0;
		}
		else if( ullTotalTianYuan > 0 &&
			stBill.Bonus > 0 && 
			stBill.Bonus < ullTotalTianYuan)
		{
			ullTotalTianYuan -= stBill.Bonus;
			stBill.Bonus = 0;
		}

		if( ullTotalTianYuan > 0 &&
			stBill.Points > 0 && 
			stBill.Points >= ullTotalTianYuan)
		{
			stBill.Points -= ullTotalTianYuan;
			ullTotalTianYuan = 0;
		}
		else if( ullTotalTianYuan > 0 &&
			stBill.Points > 0 && 
			stBill.Points < ullTotalTianYuan)
		{
			ullTotalTianYuan -= stBill.Points;
			stBill.Points = 0;
		}
		//¼ÇÂ¼ÏûºÄ×ÜÁ¿
		stBill.UsedTotalPoints += pstSSDecTianYuanReq->TianYuan;

		if(ullTotalTianYuan != 0 )
		{
			stSSPkg.Body.DecTianYuanRes.Result = BILL_DEC_NO_ENOUGH_CASH;/* Óà¶î²»×ã */
			stSSPkg.Body.DecTianYuanRes.Uin = pstSSDecTianYuanReq->Uin;
			STRNCPY(stSSPkg.Body.DecTianYuanRes.BillRecord.Pass9Account,
			pstSSDecTianYuanReq->Pass9Account,CCH(stSSPkg.Body.DecTianYuanRes.BillRecord.Pass9Account));
			stSSPkg.Body.DecTianYuanRes.BillRecord.Uin = stBill.Uin;
			stSSPkg.Body.DecTianYuanRes.BillRecord.ServerID = stBill.ServerID;
			stSSPkg.Body.DecTianYuanRes.BillRecord.Amount = stBill.Amount;
			stSSPkg.Body.DecTianYuanRes.BillRecord.Points = stBill.Points;
			stSSPkg.Body.DecTianYuanRes.BillRecord.Bonus = stBill.Bonus;
			stSSPkg.Body.DecTianYuanRes.BillRecord.FreezePoint = stBill.FreezePoint;
			stSSPkg.Body.DecTianYuanRes.BillRecord.FreezeLastTime = stBill.FreezeLastTime;			
			stSSPkg.Body.DecTianYuanRes.ExtraData = pstSSDecTianYuanReq->ExtraData;
			stSSPkg.Body.DecTianYuanRes.OperType = 	pstSSDecTianYuanReq->OperType;
			stSSPkg.Body.DecTianYuanRes.TianYuan = pstSSDecTianYuanReq->TianYuan;				
			bill_ss_backward_pkg(pstEnv,&stSSPkg,DEC_BILL_RES,pstSSDecTianYuanReq->Pass9Account);
			return -1;
		}
		/* °ÑÊý¾Ý°üÏÈ¼Ä´æÏÂ */			
		stSSPkg.Body.DecTianYuanRes.Result = 0;
		stSSPkg.Body.DecTianYuanRes.Uin = pstSSDecTianYuanReq->Uin;
		stSSPkg.Body.DecTianYuanRes.TianYuan = pstSSDecTianYuanReq->TianYuan;
		STRNCPY(stSSPkg.Body.DecTianYuanRes.BillRecord.Pass9Account,
		pstSSDecTianYuanReq->Pass9Account,CCH(stSSPkg.Body.DecTianYuanRes.BillRecord.Pass9Account));
		stSSPkg.Body.DecTianYuanRes.BillRecord.Uin = stBill.Uin;
		stSSPkg.Body.DecTianYuanRes.BillRecord.ServerID = stBill.ServerID;
		stSSPkg.Body.DecTianYuanRes.BillRecord.Amount = stBill.Amount;
		stSSPkg.Body.DecTianYuanRes.BillRecord.Points = stBill.Points;
		stSSPkg.Body.DecTianYuanRes.BillRecord.Bonus = stBill.Bonus;
		stSSPkg.Body.DecTianYuanRes.BillRecord.FreezePoint = stBill.FreezePoint;
		stSSPkg.Body.DecTianYuanRes.BillRecord.FreezeLastTime = stBill.FreezeLastTime;	
		stSSPkg.Body.DecTianYuanRes.ExtraData = pstSSDecTianYuanReq->ExtraData;
		stSSPkg.Body.DecTianYuanRes.OperType = 	pstSSDecTianYuanReq->OperType;
		stSSPkg.Body.DecTianYuanRes.WorldID = pstSSDecTianYuanReq->WorldID;

		iTmp = 0;
		iTmp += sizeof(iCallBackCmd);
		memcpy(AsynCallBackData+iTmp,&stSSPkg.Body.DecTianYuanRes,sizeof(SSDECTIANYUANRES));
		iTmp += sizeof(SSDECTIANYUANRES) ;
		
		pstPkg->Head.AsynCallBackDataLen  = iTmp;
		memcpy(pstPkg->Head.AsynCallBackData,AsynCallBackData,iTmp);
		
		/* ÐèÒª×ö¸öupdate²Ù×÷ */		
		bill_db_update_req(pstEnv,&stBill,pstPkg->Head.AsynCallBackData+sizeof(iCallBackCmd),
			pstPkg->Head.AsynCallBackDataLen-sizeof(iCallBackCmd),DB_UPDATE_BILL_DEC);			
	}
	else if( OPERATE_POINT_ADD == pstSSDecTianYuanReq->OperType)
	{
		if(pstSelectRes->SelectResult.ResultNum >0  )
		{
			// ÕâÀïÊÇupdate
			stBill.Points += pstSSDecTianYuanReq->TianYuan;

			// °ÑÊý¾Ý°üÏÈ¼Ä´æÏÂ 		
			stSSPkg.Body.DecTianYuanRes.Result = 0;
			stSSPkg.Body.DecTianYuanRes.Uin = pstSSDecTianYuanReq->Uin;
			stSSPkg.Body.DecTianYuanRes.TianYuan = pstSSDecTianYuanReq->TianYuan;
			STRNCPY(stSSPkg.Body.DecTianYuanRes.BillRecord.Pass9Account,
			pstSSDecTianYuanReq->Pass9Account,CCH(stSSPkg.Body.DecTianYuanRes.BillRecord.Pass9Account));
			stSSPkg.Body.DecTianYuanRes.BillRecord.Uin = stBill.Uin;
			stSSPkg.Body.DecTianYuanRes.BillRecord.ServerID = stBill.ServerID;
			stSSPkg.Body.DecTianYuanRes.BillRecord.Amount = stBill.Amount;
			stSSPkg.Body.DecTianYuanRes.BillRecord.Points = stBill.Points;
			stSSPkg.Body.DecTianYuanRes.BillRecord.Bonus = stBill.Bonus;
			stSSPkg.Body.DecTianYuanRes.BillRecord.FreezePoint = stBill.FreezePoint;
			stSSPkg.Body.DecTianYuanRes.BillRecord.FreezeLastTime = stBill.FreezeLastTime;	
			stSSPkg.Body.DecTianYuanRes.ExtraData = pstSSDecTianYuanReq->ExtraData;
			stSSPkg.Body.DecTianYuanRes.OperType = 	pstSSDecTianYuanReq->OperType;


			iTmp = 0;
			iTmp += sizeof(iCallBackCmd);
			memcpy(AsynCallBackData+iTmp,&stSSPkg.Body.DecTianYuanRes,sizeof(SSDECTIANYUANRES));
			iTmp += sizeof(SSDECTIANYUANRES) ;
			
			pstPkg->Head.AsynCallBackDataLen  = iTmp;
			memcpy(pstPkg->Head.AsynCallBackData,AsynCallBackData,iTmp);
			
			//ÐèÒª×ö¸öupdate²Ù×÷ 	
			bill_db_update_req(pstEnv,&stBill,pstPkg->Head.AsynCallBackData+sizeof(iCallBackCmd),
				pstPkg->Head.AsynCallBackDataLen-sizeof(iCallBackCmd),DB_UPDATE_BILL_DEC);	
		}
		else if( 0  == pstSelectRes->SelectResult.ResultNum &&
			      BILL_DEC_NO_CASH == stSSPkg.Body.DecTianYuanRes.Result)
		{
			memset(&stBill,0,sizeof(stBill));
			// ÕâÀïÊÇinsertÂß¼­,Ö»²åÈëÁ½¸öÖµ,ÆäËû¶¼ÊÇ0
			stBill.Uin = pstSSDecTianYuanReq->Uin;
			stBill.Bonus = pstSSDecTianYuanReq->TianYuan;

			// °ÑÊý¾Ý°üÏÈ¼Ä´æÏÂ 		
			stSSPkg.Body.DecTianYuanRes.Result = 0;
			stSSPkg.Body.DecTianYuanRes.Uin = pstSSDecTianYuanReq->Uin;
			stSSPkg.Body.DecTianYuanRes.TianYuan = pstSSDecTianYuanReq->TianYuan;
			STRNCPY(stSSPkg.Body.DecTianYuanRes.BillRecord.Pass9Account,
			pstSSDecTianYuanReq->Pass9Account,CCH(stSSPkg.Body.DecTianYuanRes.BillRecord.Pass9Account));
			stSSPkg.Body.DecTianYuanRes.BillRecord.Uin = stBill.Uin;
			stSSPkg.Body.DecTianYuanRes.BillRecord.ServerID = stBill.ServerID;
			stSSPkg.Body.DecTianYuanRes.BillRecord.Amount = stBill.Amount;
			stSSPkg.Body.DecTianYuanRes.BillRecord.Points = stBill.Points;
			stSSPkg.Body.DecTianYuanRes.BillRecord.Bonus = stBill.Bonus;
			stSSPkg.Body.DecTianYuanRes.BillRecord.FreezePoint = stBill.FreezePoint;
			stSSPkg.Body.DecTianYuanRes.BillRecord.FreezeLastTime = stBill.FreezeLastTime;	
			stSSPkg.Body.DecTianYuanRes.ExtraData = pstSSDecTianYuanReq->ExtraData;
			stSSPkg.Body.DecTianYuanRes.OperType = 	pstSSDecTianYuanReq->OperType;


			iTmp = 0;
			iTmp += sizeof(iCallBackCmd);
			memcpy(AsynCallBackData+iTmp,&stSSPkg.Body.DecTianYuanRes,sizeof(SSDECTIANYUANRES));
			iTmp += sizeof(SSDECTIANYUANRES) ;
			
			pstPkg->Head.AsynCallBackDataLen  = iTmp;
			memcpy(pstPkg->Head.AsynCallBackData,AsynCallBackData,iTmp);		
			
			bill_db_insert_req(pstEnv,&stBill,pstPkg->Head.AsynCallBackData+sizeof(iCallBackCmd),
					pstPkg->Head.AsynCallBackDataLen-sizeof(iCallBackCmd),DB_INSERT_BILL_ADD);
		}
	}

	/* Ð´ÈÕÖ¾ */
	
	return 0;
}
int bill_db_select_res_log_cash(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
	int iCallBackCmd = 0;	
	int iOffset = 0;
	TORMSELECTRES * pstSelectRes;
	SSCASHBILLREQ *pstCashBillReq = NULL;
	ADDBILLRECORD stAddBillRecord;
	SSPKG stSSPkg;
	
	pstSelectRes = &pstPkg->Body.SelectRes;
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);

	if( DB_SELECT_BILL_CASH_RECORD != iCallBackCmd )
	{
		return -1;
	}
	
	memset(&stAddBillRecord,0,sizeof(stAddBillRecord));
	
	pstCashBillReq = (SSCASHBILLREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
	iOffset += sizeof(SSCASHBILLREQ);

	if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
	{
		SSPKG stSSPkg;
		
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			         "Select Bill info from db failed. DBErr = %d, Uin = %d, Msg = %s.", 
			         pstSelectRes->DBMSErrNo,
			         pstCashBillReq->Uin,
			         torm_error_string(pstSelectRes->ErrNo));

		/* ÐèÒª»Ø¸ö³ö´í°ü */
		stSSPkg.Body.CashBillRes.Result = 99;
		stSSPkg.Body.CashBillRes.CashBillReq = *pstCashBillReq;
		bill_ss_backward_pkg(pstEnv,&stSSPkg,CASH_BILL_RES,pstCashBillReq->Pass9Account);

		return -1;
	}
	else if(pstSelectRes->SelectResult.ResultNum == 0)
	{
		/* Ã»ÓÐ¸ÃÁ÷Ë®µÄ³äÖµ¼ÇÂ¼ */
		bill_db_select_req_cash(pstEnv,pstCashBillReq);
		return 0;
	}
	else if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stAddBillRecord, sizeof(stAddBillRecord))
		 < 0)
	{	
		SSPKG stSSPkg;
		tlog_error(pstEnv->pstLogCat, 0, 0,	"torm_fetch_one_meta fail,Name = %s,",
			pstCashBillReq->Pass9Account);
		
		stSSPkg.Body.CashBillRes.Result = 99;
		stSSPkg.Body.CashBillRes.CashBillReq = *pstCashBillReq;
		bill_ss_backward_pkg(pstEnv,&stSSPkg,CASH_BILL_RES,pstCashBillReq->Pass9Account);
		
		return -1;
	}

	/* ÓÐÒ»ÌõÁ÷Ë®idÏàÍ¬µÄ¼ÇÂ¼ */
	/*
	if( stAddBillRecord.Sequence == pstCashBillReq->Sequence)
	{
		// ÒÑ¾­³äÖµ¹ýÁË
		SSPKG stSSPkg;
		stSSPkg.Body.CashBillRes.Result = 0;
		stSSPkg.Body.CashBillRes.CashBillReq = *pstCashBillReq;
		bill_ss_backward_pkg(pstEnv,&stSSPkg,CASH_BILL_RES,pstCashBillReq->Pass9Account);
		
		tlog_error(pstEnv->pstLogCat, 0, 0,	
			"|1001|ÏàÍ¬µÄÁ÷Ë®|messageid|%u|sequence|%llu|account|%s|serverid|%u|amount|%llu|point|%llu|bonus|%llu|Uin|%u|",
			pstCashBillReq->MessageId,
			pstCashBillReq->Sequence,
			pstCashBillReq->Pass9Account,
			pstCashBillReq->ServerID,
			pstCashBillReq->Amount,
			pstCashBillReq->Points,
			pstCashBillReq->Bonus,
			pstCashBillReq->Uin);		
		return 0;
	}*/

	tlog_error(pstEnv->pstLogCat, 0, 0,	"bill_db_select_res_log_cash  system error Name = %s,",
			pstCashBillReq->Pass9Account);
	
	
	stSSPkg.Body.CashBillRes.Result = 99;
	stSSPkg.Body.CashBillRes.CashBillReq = *pstCashBillReq;
	bill_ss_backward_pkg(pstEnv,&stSSPkg,CASH_BILL_RES,pstCashBillReq->Pass9Account);
		
	return 0;
}

int bill_db_select_res_user_freeze_bill(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
	int iCallBackCmd = 0;	
	int iOffset = 0;
	TORMSELECTRES * pstSelectRes;
	time_t tCurr = time(NULL);
	char AsynCallBackData[TORM_MAX_ASYNCALLBACK_DATA_LEN]; 
	int iTmp = 0;
	SSBILLFREEZEPOINTREQ *pstBillFreezePointReq;
	BILL 	stBill;
	unsigned long long ullPoint = 0;
	SSPKG stSSPkg;
	
	pstSelectRes = &pstPkg->Body.SelectRes;
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);

	if( DB_SELECT_USER_FREEZE_BILL != iCallBackCmd )
	{
		return -1;
	}
	
	memset(&stBill,0,sizeof(stBill));
	
	pstBillFreezePointReq = (SSBILLFREEZEPOINTREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
	iOffset += sizeof(SSQUERYTIANYUANREQ);

	memset(&stSSPkg.Body.BillFreezePointRes,0,sizeof(stSSPkg.Body.BillFreezePointRes));
	stSSPkg.Body.BillFreezePointRes.Uin = pstBillFreezePointReq->Uin;
	stSSPkg.Body.BillFreezePointRes.OperType = pstBillFreezePointReq->OperType;
	stSSPkg.Body.BillFreezePointRes.ExtraData = pstBillFreezePointReq->ExtraData;
	STRNCPY(stSSPkg.Body.BillFreezePointRes.BillRecord.Pass9Account,pstBillFreezePointReq->Pass9Account,
		CCH(pstBillFreezePointReq->Pass9Account));
	stSSPkg.Body.DecTianYuanRes.BillRecord.Uin = pstBillFreezePointReq->Uin;
	if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			         "Select Bill info from db failed. DBErr = %d, Name = %s, Msg = %s.", 
			         pstSelectRes->DBMSErrNo,
			         pstBillFreezePointReq->Pass9Account,
			         torm_error_string(pstSelectRes->ErrNo));

		/* ÐèÒª»Ø¸ö³ö´í°ü */
		stSSPkg.Body.BillFreezePointRes.Result = 1;
		bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_FREEZE_RES,pstBillFreezePointReq->Pass9Account);

		return -1;
	}
	else if(pstSelectRes->SelectResult.ResultNum == 0)
	{
		/* Ã»ÓÐ¸ÃÁ÷Ë®µÄ³äÖµ¼ÇÂ¼ */
		stSSPkg.Body.BillFreezePointRes.Result = 1;
		bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_FREEZE_RES,pstBillFreezePointReq->Pass9Account);
		return 0;
	}
	else if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stBill, sizeof(BILL))
		 < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"torm_fetch_one_meta fail,Name = %s,",
			pstBillFreezePointReq->Pass9Account);
		/* ÐèÒª»Ø¸ö³ö´í°ü */
		stSSPkg.Body.BillFreezePointRes.Result = 1;
		bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_FREEZE_RES,pstBillFreezePointReq->Pass9Account);
		
		return -1;
	}

	if(tCurr >= stBill.FreezeLastTime + MAX_TRADE_SHOW_TIME * 3600  )
	{
		stBill.FreezeLastTime = 0;
		stBill.FreezePoint = 0;
	}
	
	if( OPERATE_POINT_FREEZE == pstBillFreezePointReq->OperType )
	{
		ullPoint = stBill.Points + stBill.Bonus;
		if( ullPoint >= stBill.FreezePoint )
		{
			ullPoint = ullPoint - stBill.FreezePoint;
		}
		else
		{
			ullPoint = 0;
		}

		if( ullPoint >=  pstBillFreezePointReq->Point )
		{
			stBill.FreezePoint += pstBillFreezePointReq->Point ;
			stBill.FreezeLastTime = tCurr;
		}
		else
		{
			stSSPkg.Body.BillFreezePointRes.Result = 1;
		}
	}
	else if( OPERATE_POINT_UNFREEZE == pstBillFreezePointReq->OperType )
	{
		if( stBill.FreezePoint >= pstBillFreezePointReq->Point )
		{
			stBill.FreezePoint = stBill.FreezePoint - pstBillFreezePointReq->Point;
		}
		else
		{
			stBill.FreezePoint = 0;
			stBill.FreezeLastTime = 0;
		}
	}
	
	stSSPkg.Body.BillFreezePointRes.Uin = pstBillFreezePointReq->Uin;
	stSSPkg.Body.BillFreezePointRes.OperType = pstBillFreezePointReq->OperType;
	stSSPkg.Body.BillFreezePointRes.ExtraData = pstBillFreezePointReq->ExtraData;
	STRNCPY(stSSPkg.Body.BillFreezePointRes.BillRecord.Pass9Account, pstBillFreezePointReq->Pass9Account,
		CCH(stSSPkg.Body.BillFreezePointRes.BillRecord.Pass9Account));
	stSSPkg.Body.BillFreezePointRes.BillRecord.Uin = stBill.Uin;
	stSSPkg.Body.BillFreezePointRes.BillRecord.ServerID = stBill.ServerID;
	stSSPkg.Body.BillFreezePointRes.BillRecord.Amount = stBill.Amount;
	stSSPkg.Body.BillFreezePointRes.BillRecord.Points = stBill.Points;
	stSSPkg.Body.BillFreezePointRes.BillRecord.Bonus = stBill.Bonus;
	stSSPkg.Body.BillFreezePointRes.BillRecord.FreezePoint = stBill.FreezePoint;
	stSSPkg.Body.BillFreezePointRes.BillRecord.FreezeLastTime = stBill.FreezeLastTime;
	if( 0 != stSSPkg.Body.BillFreezePointRes.Result )
	{
		bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_FREEZE_RES,pstBillFreezePointReq->Pass9Account);
	}
	
	/* ÐèÒª×ö¸öupdate²Ù×÷ */		
	memset(AsynCallBackData,0,sizeof(AsynCallBackData));
	iTmp = 0;
	iTmp += sizeof(iCallBackCmd);
	memcpy(AsynCallBackData+iTmp,&stSSPkg.Body.BillFreezePointRes,sizeof(SSBILLFREEZEPOINTRES));
	iTmp += sizeof(SSBILLFREEZEPOINTRES) ;

	pstPkg->Head.AsynCallBackDataLen  = iTmp;
	memcpy(pstPkg->Head.AsynCallBackData,AsynCallBackData,iTmp);
	
	/* ÐèÒª×ö¸öupdate²Ù×÷ */		
	bill_db_update_req(pstEnv,&stBill,pstPkg->Head.AsynCallBackData+sizeof(iCallBackCmd),
		pstPkg->Head.AsynCallBackDataLen-sizeof(iCallBackCmd),DB_UPDATE_USER_FREEZE_BILL);	
	
	
	return 0;
}

int bill_db_select_res_user_query_bill(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
	int iCallBackCmd = 0;	
	int iOffset = 0;
	TORMSELECTRES * pstSelectRes;
	time_t tCurr = time(NULL);
	SSPKG stSSPkg;
	SSQUERYTIANYUANREQ *pstQueryTianYuanReq;
	BILL 	stBill;

	memset(&stSSPkg.Body.QueryTianYuanRes,0,sizeof(stSSPkg.Body.QueryTianYuanRes));
	memset(&stBill,0,sizeof(stBill));
	
	pstSelectRes = &pstPkg->Body.SelectRes;
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);

	if( DB_SELECT_USER_QUERY_BILL != iCallBackCmd )
	{
		return -1;
	}
	
	pstQueryTianYuanReq = (SSQUERYTIANYUANREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
	iOffset += sizeof(SSQUERYTIANYUANREQ);

	stSSPkg.Body.QueryTianYuanRes.ExtraData = pstQueryTianYuanReq->ExtraData;
	stSSPkg.Body.QueryTianYuanRes.OperType = pstQueryTianYuanReq->OperType;
	if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			         "Select Bill info from db failed. DBErr = %d, Name = %s, Msg = %s.", 
			         pstSelectRes->DBMSErrNo,
			         pstQueryTianYuanReq->Pass9Account,
			         torm_error_string(pstSelectRes->ErrNo));

		/* ÐèÒª»Ø¸ö³ö´í°ü */
		stSSPkg.Body.QueryTianYuanRes.Result = 1;
		stSSPkg.Body.QueryTianYuanRes.Uin = pstQueryTianYuanReq->Uin;
		bill_ss_backward_pkg(pstEnv,&stSSPkg,USER_QUERY_BILL_RES,pstQueryTianYuanReq->Pass9Account);

		return -1;
	}
	else if(pstSelectRes->SelectResult.ResultNum == 0)
	{
		/* Ã»ÓÐ¸ÃÁ÷Ë®µÄ³äÖµ¼ÇÂ¼ */
		stSSPkg.Body.QueryTianYuanRes.Result = 0;
		stSSPkg.Body.QueryTianYuanRes.Uin = pstQueryTianYuanReq->Uin;
		bill_ss_backward_pkg(pstEnv,&stSSPkg,USER_QUERY_BILL_RES,pstQueryTianYuanReq->Pass9Account);
		return 0;
	}
	else if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stBill, sizeof(BILL))
		 < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"torm_fetch_one_meta fail,Name = %s,",
			pstQueryTianYuanReq->Pass9Account);
		/* ÐèÒª»Ø¸ö³ö´í°ü */
		stSSPkg.Body.QueryTianYuanRes.Result = 1;
		stSSPkg.Body.QueryTianYuanRes.Uin = pstQueryTianYuanReq->Uin;
		bill_ss_backward_pkg(pstEnv,&stSSPkg,USER_QUERY_BILL_RES,pstQueryTianYuanReq->Pass9Account);
		
		return -1;
	}

	
	stSSPkg.Body.QueryTianYuanRes.Result = 0;
	stSSPkg.Body.QueryTianYuanRes.Uin = pstQueryTianYuanReq->Uin;
	STRNCPY(stSSPkg.Body.QueryTianYuanRes.BillRecord.Pass9Account ,pstQueryTianYuanReq->Pass9Account,
				CCH(stSSPkg.Body.QueryTianYuanRes.BillRecord.Pass9Account ));
	stSSPkg.Body.BillFreezePointRes.BillRecord.Uin = stBill.Uin;
	stSSPkg.Body.QueryTianYuanRes.BillRecord.ServerID = stBill.ServerID;
	stSSPkg.Body.QueryTianYuanRes.BillRecord.Amount = stBill.Amount;
	stSSPkg.Body.QueryTianYuanRes.BillRecord.Points = stBill.Points;
	stSSPkg.Body.QueryTianYuanRes.BillRecord.Bonus = stBill.Bonus;
	if(tCurr > stBill.FreezeLastTime + MAX_TRADE_SHOW_TIME*3600 )
	{
		stBill.FreezePoint = 0;
		stBill.FreezeLastTime = 0;
	}
	stSSPkg.Body.QueryTianYuanRes.BillRecord.FreezePoint = stBill.FreezePoint;
	stSSPkg.Body.QueryTianYuanRes.BillRecord.FreezeLastTime  = stBill.FreezeLastTime;
	stSSPkg.Body.QueryTianYuanRes.OperType = pstQueryTianYuanReq->OperType;
		
	bill_ss_backward_pkg(pstEnv,&stSSPkg,USER_QUERY_BILL_RES,pstQueryTianYuanReq->Pass9Account);

	
	return 0;
}
int bill_db_select_res(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
	int iCallBackCmd = 0;	
	int iOffset = 0;
	TORMSELECTRES * pstSelectRes;
	
	pstSelectRes = &pstPkg->Body.SelectRes;
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);

	if( DB_SELECT_BILL_CASH == iCallBackCmd )
	{
		BILL stBill;
		SSCASHBILLREQ *pstCashBillReq;
		
		memset(&stBill,0,sizeof(stBill));
		pstCashBillReq = (SSCASHBILLREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(SSCASHBILLREQ);
		if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
		{
			SSPKG stSSPkg;
			
			tlog_error(pstEnv->pstLogCat, 0, 0, 
				         "Select Bill info from db failed. DBErr = %d, Name = %s, Msg = %s.", 
				         pstSelectRes->DBMSErrNo,
				         pstCashBillReq->Pass9Account,
				         torm_error_string(pstSelectRes->ErrNo));
			/* ÐèÒª»Ø¸ö³ö´í°ü */
			stSSPkg.Body.CashBillRes.Result = 99;
			stSSPkg.Body.CashBillRes.CashBillReq = *pstCashBillReq;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,CASH_BILL_RES,pstCashBillReq->Pass9Account);
			return -1;
		}
		
		if(pstSelectRes->SelectResult.ResultNum == 0)
		{
			/* ²åÈëbill²Ù×÷ */
			stBill.Uin = pstCashBillReq->Uin;
			stBill.Sequence = pstCashBillReq->Sequence;
			stBill.ServerID = pstCashBillReq->ServerID;
			stBill.Amount = pstCashBillReq->Amount;
			stBill.Bonus = pstCashBillReq->Bonus;
			stBill.Points = pstCashBillReq->Points;
			stBill.CashTotalPoints += pstCashBillReq->Points;
			bill_db_insert_req(pstEnv,&stBill,pstPkg->Head.AsynCallBackData+sizeof(iCallBackCmd),
					pstPkg->Head.AsynCallBackDataLen-sizeof(iCallBackCmd),DB_INSERT_BILL_CASH);

			return 0;
		}
		else if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stBill, sizeof(stBill))
			 < 0)
		{
			SSPKG stSSPkg;
			
			tlog_error(pstEnv->pstLogCat, 0, 0,	"torm_fetch_one_meta fail,Name = %s,",
				pstCashBillReq->Pass9Account);
			/*  ÕâÀïÐèÒª¸ö´íÎó»Ø°ü */
			stSSPkg.Body.CashBillRes.Result = 99;
			stSSPkg.Body.CashBillRes.CashBillReq = *pstCashBillReq;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,CASH_BILL_RES,pstCashBillReq->Pass9Account);			
			return -1;
		}
		if(stBill.Sequence  == pstCashBillReq->Sequence) 
		{
			/*×îÐÂÒ»´ÎµÄÁ÷Ë®idÒ»Ñù,Ö±½Ó·µ»Ø */
			SSPKG stSSPkg;
			stSSPkg.Body.CashBillRes.Result = 0; /* Í¨Öª9c³åÖµ³É¹¦*/
			stSSPkg.Body.CashBillRes.CashBillReq = *pstCashBillReq;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,CASH_BILL_RES,pstCashBillReq->Pass9Account);

			tlog_error(pstEnv->pstLogCat, 0, 0,	
				"|1001|ÏàÍ¬µÄÁ÷Ë®|messageid|%u|sequence|%llu|account|%s|serverid|%u|amount|%llu|point|%llu|bonus|%llu|",
				pstCashBillReq->MessageId,
				pstCashBillReq->Sequence,
				pstCashBillReq->Pass9Account,
				pstCashBillReq->ServerID,
				pstCashBillReq->Amount,
				pstCashBillReq->Points,
				pstCashBillReq->Bonus);
			return -1;
		}
		/* ÐèÒª×ö¸öupdate²Ù×÷ */
		stBill.Sequence = pstCashBillReq->Sequence;
		stBill.Amount += pstCashBillReq->Amount;
		stBill.Bonus += pstCashBillReq->Bonus;
		stBill.Points += pstCashBillReq->Points;	
		stBill.CashTotalPoints += pstCashBillReq->Points;
		bill_db_update_req(pstEnv,&stBill,pstPkg->Head.AsynCallBackData+sizeof(iCallBackCmd),
			pstPkg->Head.AsynCallBackDataLen-sizeof(iCallBackCmd),DB_UPDATE_BILL);		
	}
	else if( DB_SELECT_BILL_QUERY == iCallBackCmd )
	{
		/* ²éÑ¯Óà¶îµÄÁ÷³Ì */
		BILL stBill;
		SSQUERYBILLREQ *pstQueryBillReq;
		SSPKG stSSPkg;
		
		memset(&stBill,0,sizeof(stBill));
		pstQueryBillReq = (SSQUERYBILLREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(SSQUERYBILLREQ);
		if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
		{
			SSPKG stSSPkg;
			
			tlog_error(pstEnv->pstLogCat, 0, 0, 
				         "Select Bill info from db failed. DBErr = %d, Name = %s, Msg = %s.", 
				         pstSelectRes->DBMSErrNo,
				         pstQueryBillReq->Pass9Account,
				         torm_error_string(pstSelectRes->ErrNo));
			/* ÐèÒª»Ø¸ö³ö´í°ü */
			memset(&stSSPkg,0,sizeof(stSSPkg));
			stSSPkg.Body.QueryBillRes.Result = 99;
			stSSPkg.Body.QueryBillRes.MessageId = pstQueryBillReq->MessageId;
			stSSPkg.Body.QueryBillRes.Sequence = pstQueryBillReq->Sequence;
			stSSPkg.Body.QueryBillRes.QueryTs = time(NULL);
			bill_ss_backward_pkg(pstEnv,&stSSPkg,QUERY_BILL_RES,pstQueryBillReq->Pass9Account);
			return -1;
		}
		
		if(pstSelectRes->SelectResult.ResultNum == 0)
		{
			SSPKG stSSPkg;
			memset(&stSSPkg,0,sizeof(stSSPkg));
			stSSPkg.Body.QueryBillRes.Result = 3;//ÕËºÅ²»´æÔÚ
			stSSPkg.Body.QueryBillRes.MessageId = pstQueryBillReq->MessageId;
			stSSPkg.Body.QueryBillRes.Sequence = pstQueryBillReq->Sequence;
			stSSPkg.Body.QueryBillRes.QueryTs = time(NULL);
			bill_ss_backward_pkg(pstEnv,&stSSPkg,QUERY_BILL_RES,pstQueryBillReq->Pass9Account);
			return 0;
		}
		else if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stBill, sizeof(stBill))
			 < 0)
		{
			SSPKG stSSPkg;
			tlog_error(pstEnv->pstLogCat, 0, 0,	"torm_fetch_one_meta fail,Name = %s,",
				pstQueryBillReq->Pass9Account);
			/*  ÕâÀïÐèÒª¸ö´íÎó»Ø°ü */
			memset(&stSSPkg,0,sizeof(stSSPkg));
			stSSPkg.Body.QueryBillRes.Result = 99;
			stSSPkg.Body.QueryBillRes.MessageId = pstQueryBillReq->MessageId;
			stSSPkg.Body.QueryBillRes.Sequence = pstQueryBillReq->Sequence;
			stSSPkg.Body.QueryBillRes.QueryTs = time(NULL);
			bill_ss_backward_pkg(pstEnv,&stSSPkg,QUERY_BILL_RES,pstQueryBillReq->Pass9Account);			
			return -1;
		}
		
		
		memset(&stSSPkg,0,sizeof(stSSPkg));
		stSSPkg.Body.QueryBillRes.Result = 0;
		stSSPkg.Body.QueryBillRes.MessageId = pstQueryBillReq->MessageId;
		stSSPkg.Body.QueryBillRes.Sequence = pstQueryBillReq->Sequence;
		stSSPkg.Body.QueryBillRes.QueryTs = time(NULL);
		stSSPkg.Body.QueryBillRes.Amount = stBill.Amount;
		stSSPkg.Body.QueryBillRes.Bonus =  stBill.Bonus;
		stSSPkg.Body.QueryBillRes.Points = stBill.Points;
		bill_ss_backward_pkg(pstEnv,&stSSPkg,QUERY_BILL_RES,pstQueryBillReq->Pass9Account);
	}
	else if(DB_SELECT_BILL_DEC == iCallBackCmd)
	{
		bill_db_select_res_dec(pstEnv,pstPkg);
	}
	else if(DB_SELECT_BILL_CASH_RECORD == iCallBackCmd )
	{
		bill_db_select_res_log_cash(pstEnv,pstPkg);
	}
	else if(DB_SELECT_USER_QUERY_BILL == iCallBackCmd )
	{
		bill_db_select_res_user_query_bill(pstEnv,pstPkg);
	}
	else if(DB_SELECT_USER_FREEZE_BILL == iCallBackCmd)
	{
		bill_db_select_res_user_freeze_bill(pstEnv,pstPkg);
	}
	else if(DB_SELECT_PROPSDEF == iCallBackCmd)
	{
		bill_db_select_res_propsdef(pstEnv,pstPkg);
	}
	else if(DB_SELECT_PROPSGIFT == iCallBackCmd)
	{
		bill_db_select_res_propsgift(pstEnv,pstPkg);
	}
	else if (DB_SELECT_ACCGIFT == iCallBackCmd)
	{
		bill_db_select_res_accgift(pstEnv,pstPkg);
	}
	else if (DB_SELECT_PROPGIFT_CLT == iCallBackCmd)
	{
		bill_db_select_res_propgift_clt(pstEnv,pstPkg)	;
	}
	else if (DB_SELECT_CDKEYTYPEDEF == iCallBackCmd)
	{
		 bill_db_select_res_cdkey_type(pstEnv,pstPkg);
	}
	else if (DB_SELECT_CDKEYDEF == iCallBackCmd)
	{
		bill_db_select_res_cdkey_def(pstEnv,pstPkg);
	}
	else if (DB_SELECT_CDKEYDEF_GET == iCallBackCmd)
	{
		bill_db_select_res_cdkey_get(pstEnv,pstPkg);
	}
	return 0;
}



int bill_db_propsgift2region(BILL_SVRENV *pstEnv, PROPSGIFT  *pstPropsGift, const char* pszPass9)
{
	SSPKG stPkg;
	SSPROPGIFTBILLRES *pstPropGiftBillRes = &stPkg.Body.PropGiftBillRes;
	TDRDATA stNet;
//	int iDst;

	stPkg.Head.Cmd = PROP_GIFT_BILL_RES;
	stPkg.Head.HeadLen = sizeof(SSPKGHEAD);
	stPkg.Head.Magic = PROTOCOL_MAGIC;
	stPkg.Head.Ver = TDR_METALIB_PROTO_SS_VERSION;

	pstPropGiftBillRes->PropsInfoType = PROPS_GIFT_BILL_INFO_TYPE_GIFT;
	pstPropGiftBillRes->PropsGiftBillInfo.PropsGift = *pstPropsGift;

	STRNCPY(pstPropGiftBillRes->Pass9Account, pszPass9, sizeof(pstPropGiftBillRes->Pass9Account));

	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = pstEnv->szSendBuffer;
	if(ss_pack(pstEnv->pstSSHan, &stNet, &stPkg, 0) < 0)
	{
		return -1;
	}
	
	tbus_send(pstEnv->pstAppCtx->iBus,  &pstEnv->pstAppCtx->iId, &pstEnv->iRegionID, stNet.pszBuff, stNet.iBuff, 0);

	return 0;
}

int bill_props_gift_add_cash_to_region(BILL_SVRENV *pstEnv,SSCASHPROPSGIFTINFOREQ* pstReq)
{
	SSPKG stPkg;
	SSPROPGIFTBILLRES *pstPropGiftBillRes = &stPkg.Body.PropGiftBillRes;
	TDRDATA stNet;
	int iRet = 0;

	stPkg.Head.Cmd = PROP_GIFT_BILL_RES;
	stPkg.Head.HeadLen = sizeof(SSPKGHEAD);
	stPkg.Head.Magic = PROTOCOL_MAGIC;
	stPkg.Head.Ver = TDR_METALIB_PROTO_SS_VERSION;

	pstPropGiftBillRes->PropsInfoType = PROPS_GIFT_BILL_INFO_TYPE_CASH;
	pstPropGiftBillRes->PropsGiftBillInfo.CashInfo.AddCash = pstReq->AddCash;
	pstPropGiftBillRes->PropsGiftBillInfo.CashInfo.GroupID = pstReq->GroupID;

	STRNCPY(pstPropGiftBillRes->Pass9Account, pstReq->Pass9Account, sizeof(pstPropGiftBillRes->Pass9Account));

	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = pstEnv->szSendBuffer;
	if(ss_pack(pstEnv->pstSSHan, &stNet, &stPkg, 0) == 0)
	{
		iRet = tbus_send(pstEnv->pstAppCtx->iBus,  &pstEnv->pstAppCtx->iId, &pstEnv->iRegionID, stNet.pszBuff, stNet.iBuff, 0);
	}
	else
	{
		iRet = -1;
	}
	
	

	{
		SSPKG stSSPkg;
		memset(&stSSPkg.Body.CashPropsGiftInfoRes,0,sizeof(stSSPkg.Body.CashPropsGiftInfoRes));
		STRNCPY(stSSPkg.Body.CashPropsGiftInfoRes.Pass9Account,pstReq->Pass9Account,
			CCH(stSSPkg.Body.CashPropsGiftInfoRes.Pass9Account));
		
		stSSPkg.Body.CashPropsGiftInfoRes.Result = iRet;
		stSSPkg.Body.CashPropsGiftInfoRes.CsConnCtx = pstReq->CsConnCtx;
		bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_CASH_PROPSGIFT_RES,NULL); 
	}

	return 0;
}


//static int tbus_get_dst_list(const int a_iHandle, unsigned int * a_piDstList, int * a_piVecCnt)


int bill_db_insert_res(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
	int iCallBackCmd = 0;	
	int iOffset = 0;
	TORMINSERTRES * pstRes;
	
	pstRes = &pstPkg->Body.InsertRes;
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);

	if( DB_INSERT_BILL_CASH == iCallBackCmd )
	{
		BILL stBill;
		SSCASHBILLREQ *pstCashBillReq;
		SSPKG stSSPkg;
		
		memset(&stBill,0,sizeof(stBill));
		pstCashBillReq = (SSCASHBILLREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(SSCASHBILLREQ);
		if (TORM_ERR_IS_ERROR(pstRes->ErrNo))
		{
			SSPKG stSSPkg;
			tlog_error(pstEnv->pstLogCat, 0, 0, 
				         "insert Bill info from db failed. DBErr = %d, Name = %s, Msg = %s.", 
				         pstRes->DBMSErrNo,
				         pstCashBillReq->Pass9Account,
				         torm_error_string(pstRes->ErrNo));
			/* ÐèÒª»Ø¸ö³ö´í°ü */
			
			stSSPkg.Body.CashBillRes.Result = 99;
			stSSPkg.Body.CashBillRes.CashBillReq = *pstCashBillReq;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,CASH_BILL_RES,pstCashBillReq->Pass9Account);
			
			tlog_error(pstEnv->pstLogCat, 0, 0,	
				"|1002|³åÖµ²åÈëÊ§°Ü|messageid|%u|sequence|%llu|account|%s|serverid|%u|amount|%llu|point|%llu|bonus|%llu|",
				pstCashBillReq->MessageId,
				pstCashBillReq->Sequence,
				pstCashBillReq->Pass9Account,
				pstCashBillReq->ServerID,
				pstCashBillReq->Amount,
				pstCashBillReq->Points,
				pstCashBillReq->Bonus);				
			return -1;
		}

		tlog_info(pstEnv->pstLogCat, 0, 0,	
			"|2001|³åÖµ²åÈë³É¹¦|messageid|%u|sequence|%llu|account|%s|serverid|%u|amount|%llu|point|%llu|bonus|%llu|",
			pstCashBillReq->MessageId,
			pstCashBillReq->Sequence,
			pstCashBillReq->Pass9Account,
			pstCashBillReq->ServerID,
			pstCashBillReq->Amount,
			pstCashBillReq->Points,
			pstCashBillReq->Bonus);			
		/* ²åÈë³É¹¦·µ»ØÒ»¸öÊý¾Ý°ü */
		
		stSSPkg.Body.CashBillRes.Result = 0;
		stSSPkg.Body.CashBillRes.CashBillReq = *pstCashBillReq;
		bill_ss_backward_pkg(pstEnv,&stSSPkg,CASH_BILL_RES,pstCashBillReq->Pass9Account);

		/* ²åÈëÁ÷Ë®dbÀïÃæÈ¥ */
		bill_db_insert_req(pstEnv,pstCashBillReq,pstPkg->Head.AsynCallBackData+sizeof(iCallBackCmd),
				pstPkg->Head.AsynCallBackDataLen-sizeof(iCallBackCmd),DB_INSERT_BILL_CASH_RECORD);

		//³åÖµËÍÀñ°ü
		bill_db_select_req_propsgift(pstEnv,(char *)pstCashBillReq,
					sizeof(SSCASHBILLREQ),pstCashBillReq->Uin,QUERY_PROPSGIFT_BY_INFORMER);
		
	}
	else if( DB_INSERT_BILL_ADD == iCallBackCmd )
	{
		bill_db_insert_res_dec(pstEnv,pstPkg);
	}
	else if(DB_INSERT_PROPS == iCallBackCmd )
	{
		SSCASHBILLREQ *pstCashBillReq;
		pstCashBillReq = (SSCASHBILLREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(SSCASHBILLREQ);
		if (TORM_ERR_IS_ERROR(pstRes->ErrNo))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, 
				         "insert props info from db failed. DBErr = %d, Name = %s, Msg = %s.", 
				         pstRes->DBMSErrNo,
				         pstCashBillReq->Pass9Account,
				         torm_error_string(pstRes->ErrNo));
		}
	}
	else if( DB_INSERT_BILL_CASH_RECORD == iCallBackCmd)
	{
		/*Á÷Ë®²åÈë·µ»Ø */
		SSCASHBILLREQ *pstCashBillReq;
		pstCashBillReq = (SSCASHBILLREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(SSCASHBILLREQ);		
		if (TORM_ERR_IS_ERROR(pstRes->ErrNo))
		{
			//²åÈëÊ§°Ü¼ÆÌõÈÕÖ¾
			tlog_error(pstEnv->pstLogCat, 0, 0, 
				         "insert cash log db failed. DBErr = %d, Name = %s, Msg = %s, Sequence=%llu", 
				         pstRes->DBMSErrNo,
				         pstCashBillReq->Pass9Account,
				         torm_error_string(pstRes->ErrNo),
				         pstCashBillReq->Sequence);
			tlog_error(pstEnv->pstLogCat, 0, 0,	
				"|1003|³åÖµ¼ÇÂ¼²åÈëÊ§°Ü,Òª²¹²å|messageid|%u|sequence|%llu|account|%s|serverid|%u|amount|%llu|point|%llu|bonus|%llu|",
				pstCashBillReq->MessageId,
				pstCashBillReq->Sequence,
				pstCashBillReq->Pass9Account,
				pstCashBillReq->ServerID,
				pstCashBillReq->Amount,
				pstCashBillReq->Points,
				pstCashBillReq->Bonus);	
		}
		else
		{
			tlog_info(pstEnv->pstLogCat, 0, 0,	
				"|2003|³åÖµ¼ÇÂ¼²åÈë³É¹¦|messageid|%u|sequence|%llu|account|%s|serverid|%u|amount|%llu|point|%llu|bonus|%llu|",
				pstCashBillReq->MessageId,
				pstCashBillReq->Sequence,
				pstCashBillReq->Pass9Account,
				pstCashBillReq->ServerID,
				pstCashBillReq->Amount,
				pstCashBillReq->Points,
				pstCashBillReq->Bonus);	
		}
	}
	else if( DB_INSERT_BILL_DEC_RECORD == iCallBackCmd )
	{
		SSDECTIANYUANRES *pstDecTianYuanRes;
				
		pstDecTianYuanRes = (SSDECTIANYUANRES *)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(SSDECTIANYUANRES);	

		if (TORM_ERR_IS_ERROR(pstRes->ErrNo))
		{
			//²åÈëÊ§°Ü¼ÆÌõÈÕÖ¾
			tlog_error(pstEnv->pstLogCat, 0, 0,	
				"|1006|¿Û·Ñ¼ÇÂ¼²åÈëÊ§°Ü,Òª²¹²å|Uin|%d|TianYuan|%llu|",
				pstDecTianYuanRes->Uin, pstDecTianYuanRes->TianYuan);
		}
	}
	else if (DB_INSERT_PROPSGIFT == iCallBackCmd)
	{
		SSCASHBILLREQ stCashBillReq;
		PROPSGIFT  stPropsGift;
		int iToClt ;
		
		iOffset += sizeof(int); // Õ¼Î»  iOpType Î´È¡...
		
		stCashBillReq = *(SSCASHBILLREQ*)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(SSCASHBILLREQ);
		
		stPropsGift  = *(PROPSGIFT*)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(PROPSGIFT);
		
		iToClt =  *(int*)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(int);
		if (iToClt)
		{
			bill_db_propsgift2region(pstEnv, &stPropsGift, stCashBillReq.Pass9Account);
		}
	}
	else if( DB_INSERT_GODCOIN_CONSUME_RECORD == iCallBackCmd )
	{
		SSGODCOINCONSUMELOGREQ *pstGodCoinConsumeLogReq;
		
		pstGodCoinConsumeLogReq = (SSGODCOINCONSUMELOGREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(SSGODCOINCONSUMELOGREQ);	
		
		if (TORM_ERR_IS_ERROR(pstRes->ErrNo))
		{
			//²åÈëÊ§°Ü¼ÆÌõÈÕÖ¾
			tlog_error(pstEnv->pstLogCat, 0, 0,	
				"|5001|ÏûºÄ½ðÈ¯²åÈëÊ§°Ü,Òª²¹²å|Uin|%u|RoleID|%llu|WorldID|%d|OperType|%d|OperSubType|%d|GodCoin|%llu|ShopID|%u|ShopNum|%u|Desc|%s|",
				pstGodCoinConsumeLogReq->Uin,
				pstGodCoinConsumeLogReq->RoleID,
				pstGodCoinConsumeLogReq->WorldID,
				pstGodCoinConsumeLogReq->OperType,
				pstGodCoinConsumeLogReq->OperSubType,
				pstGodCoinConsumeLogReq->GodCoin,
				pstGodCoinConsumeLogReq->ShopID,
				pstGodCoinConsumeLogReq->ShopNum,
				pstGodCoinConsumeLogReq->OperDesc);		

				tlog_error(pstEnv->pstLogCat, 0, 0, "insert res error errno=%d, errmsg=%s", 
					pstRes->ErrNo,torm_error_string(pstRes->ErrNo));
		}
		else
		{
			/*
			tlog_error(pstEnv->pstLogCat, 0, 0,	
				"|5001|ÏûºÄ½ð±Ò²åÈë³É¹¦|Uin|%u|RoleID|%llu|GodCoin|%llu|ItemID|%u|ItemNum|%u|Desc|%s|",
				pstGodCoinConsumeLogReq->Uin,
				pstGodCoinConsumeLogReq->RoleID,
				pstGodCoinConsumeLogReq->GodCoin,
				pstGodCoinConsumeLogReq->ShopID,
				pstGodCoinConsumeLogReq->ShopNum,
				pstGodCoinConsumeLogReq->OperDesc);*/
		}
		
	}
	else if (DB_INSERT_CDKEY_GET == iCallBackCmd)
	{
		bill_db_insert_res_cdkey_get(pstEnv,pstPkg);	
	}
	else if (DB_INSERT_ACCGIFT == iCallBackCmd)
	{
		bill_db_insert_res_acc_gift(pstEnv,pstPkg);	
	}
 	return 0;
}

int bill_db_update_res_freeze(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
 	int iCallBackCmd = 0;	
	int iOffset = 0;
	TORMUPDATERES * pstRes;
	BILL stBill;
	SSBILLFREEZEPOINTRES *pstBillFreezePointRes;
	SSPKG stSSPkg;
	
	pstRes = &pstPkg->Body.UpdateRes;
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);

	if( DB_UPDATE_USER_FREEZE_BILL != iCallBackCmd )
	{
		return -1;
	}

	memset(&stBill,0,sizeof(stBill));
	pstBillFreezePointRes = (SSBILLFREEZEPOINTRES *)(pstPkg->Head.AsynCallBackData+iOffset);
	iOffset += sizeof(SSBILLFREEZEPOINTRES);
	stSSPkg.Body.BillFreezePointRes = *pstBillFreezePointRes;
	
	if (TORM_ERR_IS_ERROR(pstRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			         "update Bill info from db failed. DBErr = %d, Name = %s, Msg = %s.", 
			         pstRes->DBMSErrNo,
			         pstBillFreezePointRes->BillRecord.Pass9Account,
			         torm_error_string(pstRes->ErrNo));

		
		stSSPkg.Body.BillFreezePointRes.Result = 1; /* ¶³½áÌìÔªÊ§°Ü */
		bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_FREEZE_RES,pstBillFreezePointRes->BillRecord.Pass9Account);
		
		return -1;
	}

	if(pstRes->AffectRows < 1)
	{
		/* ÑÏ¸ñÅÐ¶Ï */
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			         "update Bill AffectRows failed.  DBErr = %d, Name = %s, Msg = %s.", 
			         pstRes->DBMSErrNo,
			         pstBillFreezePointRes->BillRecord.Pass9Account,
			         torm_error_string(pstRes->ErrNo));

		stSSPkg.Body.BillFreezePointRes.Result = 1; /* ¿ÛÌìÔª»ØÐ´Ê§°Ü */
		bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_FREEZE_RES,pstBillFreezePointRes->BillRecord.Pass9Account);
		return -1;
	}
	
	/* ÎÒµÄÌì°¡,×ÜËã¿ÉÒÔ·µ»ØÁË */
	stSSPkg.Body.BillFreezePointRes.Result = 0;
	bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_FREEZE_RES,pstBillFreezePointRes->BillRecord.Pass9Account);	

 	return 0;
}

int bill_db_update_res_dec(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
 	int iCallBackCmd = 0;	
	int iOffset = 0;
	TORMUPDATERES * pstRes;
	BILL stBill;
	SSDECTIANYUANRES *pstDecTianYuanRes;
	SSPKG stSSPkg;
	tdr_ulonglong RoleID = 0;
		
	pstRes = &pstPkg->Body.UpdateRes;
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);

	if( DB_UPDATE_BILL_DEC != iCallBackCmd )
	{
		return -1;
	}


	memset(&stBill,0,sizeof(stBill));
	pstDecTianYuanRes = (SSDECTIANYUANRES *)(pstPkg->Head.AsynCallBackData+iOffset);
	iOffset += sizeof(SSDECTIANYUANRES);
	stSSPkg.Body.DecTianYuanRes = *pstDecTianYuanRes;

	if (pstDecTianYuanRes->ExtraData.DataType == DATATYPE_POINT_BY_EXCHANGE)
	{
		CSSHOPPINGEXCHANGEREQ *pstExchangeReq;
		pstExchangeReq = (CSSHOPPINGEXCHANGEREQ *)pstDecTianYuanRes->ExtraData.ExtraData;
		RoleID = pstExchangeReq->RoleID;
	}
	
	if (TORM_ERR_IS_ERROR(pstRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			         "update Bill info from db failed. DBErr = %d, Name = %s, Msg = %s.", 
			         pstRes->DBMSErrNo,
			         pstDecTianYuanRes->BillRecord.Pass9Account,
			         torm_error_string(pstRes->ErrNo));

		tlog_error(pstEnv->pstLogCat, 0, 0, 
					"|1999|¶Ò»»½ðÈ¯Ê§°Ü|Result|6|Uin|%d|OperType|%u|TianYuan|%llu|account|%s|serverid|%u|amount|%llu|point|%llu|bonus|%llu|FreezePoint|%llu|FreezeLastTime|%u|WorldID|%d|RoleID|%llu|",
					pstDecTianYuanRes->Uin,
					pstDecTianYuanRes->OperType,
					pstDecTianYuanRes->TianYuan,
					pstDecTianYuanRes->BillRecord.Pass9Account,
					pstDecTianYuanRes->BillRecord.ServerID,
					pstDecTianYuanRes->BillRecord.Amount,
					pstDecTianYuanRes->BillRecord.Points,
					pstDecTianYuanRes->BillRecord.Bonus,
					pstDecTianYuanRes->BillRecord.FreezePoint,
					pstDecTianYuanRes->BillRecord.FreezeLastTime,
					pstDecTianYuanRes->WorldID,
					RoleID);	
		
		stSSPkg.Body.DecTianYuanRes.Result = 6; /* ¿ÛÌìÔª»ØÐ´Ê§°Ü */
		bill_ss_backward_pkg(pstEnv,&stSSPkg,DEC_BILL_RES,pstDecTianYuanRes->BillRecord.Pass9Account);
		
		return -1;
	}

	if(pstRes->AffectRows < 1)
	{
		/* ÑÏ¸ñÅÐ¶Ï */
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			         "update Bill AffectRows failed.  DBErr = %d, Name = %s, Msg = %s.", 
			         pstRes->DBMSErrNo,
			         pstDecTianYuanRes->BillRecord.Pass9Account,
			         torm_error_string(pstRes->ErrNo));

		stSSPkg.Body.DecTianYuanRes.Result = 7; /* ¿ÛÌìÔª»ØÐ´Ê§°Ü */
		bill_ss_backward_pkg(pstEnv,&stSSPkg,DEC_BILL_RES,pstDecTianYuanRes->BillRecord.Pass9Account);
		return -1;
	}
	
	/* ÎÒµÄÌì°¡,×ÜËã¿ÉÒÔ·µ»ØÁË */
	bill_ss_backward_pkg(pstEnv,&stSSPkg,DEC_BILL_RES,pstDecTianYuanRes->BillRecord.Pass9Account);	

	if( OPERATE_POINT_ADD == pstDecTianYuanRes->OperType )
	{
		tlog_info(pstEnv->pstLogCat, 0, 0,	
		"|2002|¼ÓµãÈ¯³É¹¦|Result|%u|Uin|%d|OperType|%u|TianYuan|%llu|account|%s|serverid|%u|amount|%llu|point|%llu|bonus|%llu|FreezePoint|%llu|FreezeLastTime|%u|",
		pstDecTianYuanRes->Result,
		pstDecTianYuanRes->Uin,
		pstDecTianYuanRes->OperType,
		pstDecTianYuanRes->TianYuan,
		pstDecTianYuanRes->BillRecord.Pass9Account,
		pstDecTianYuanRes->BillRecord.ServerID,
		pstDecTianYuanRes->BillRecord.Amount,
		pstDecTianYuanRes->BillRecord.Points,
		pstDecTianYuanRes->BillRecord.Bonus,
		pstDecTianYuanRes->BillRecord.FreezePoint,
		pstDecTianYuanRes->BillRecord.FreezeLastTime);	
	}
	else
	{
		tlog_info(pstEnv->pstLogCat, 0, 0,	
		"|2002|¿ÛµãÈ¯³É¹¦|Result|%u|Uin|%d|OperType|%u|TianYuan|%llu|account|%s|serverid|%u|amount|%llu|point|%llu|bonus|%llu|FreezePoint|%llu|FreezeLastTime|%u|WorldID|%d|RoleID|%llu|",
					pstDecTianYuanRes->Result,
					pstDecTianYuanRes->Uin,
					pstDecTianYuanRes->OperType,
					pstDecTianYuanRes->TianYuan,
					pstDecTianYuanRes->BillRecord.Pass9Account,
					pstDecTianYuanRes->BillRecord.ServerID,
					pstDecTianYuanRes->BillRecord.Amount,
					pstDecTianYuanRes->BillRecord.Points,
					pstDecTianYuanRes->BillRecord.Bonus,
					pstDecTianYuanRes->BillRecord.FreezePoint,
					pstDecTianYuanRes->BillRecord.FreezeLastTime,
					pstDecTianYuanRes->WorldID,
					RoleID);		
	}

	/* ²åÈëÒ»¸ö¿ÛÇ®µÄ¼ÇÂ¼ */
	bill_db_insert_req(pstEnv,pstDecTianYuanRes,pstPkg->Head.AsynCallBackData+sizeof(iCallBackCmd),
			pstPkg->Head.AsynCallBackDataLen-sizeof(iCallBackCmd),DB_INSERT_BILL_DEC_RECORD);
 	return 0;
}
int bill_db_insert_res_dec(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
 	int iCallBackCmd = 0;	
	int iOffset = 0;
	TORMINSERTRES * pstInsertRes;
	BILL stBill;
	SSDECTIANYUANRES *pstDecTianYuanRes;
	SSPKG stSSPkg;
	
	pstInsertRes = &pstPkg->Body.InsertRes;
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);

	if( DB_INSERT_BILL_ADD != iCallBackCmd )
	{
		return -1;
	}

	memset(&stBill,0,sizeof(stBill));
	pstDecTianYuanRes = (SSDECTIANYUANRES *)(pstPkg->Head.AsynCallBackData+iOffset);
	iOffset += sizeof(SSDECTIANYUANRES);
	stSSPkg.Body.DecTianYuanRes = *pstDecTianYuanRes;
	
	if (TORM_ERR_IS_ERROR(pstInsertRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			         "insert Bill info from db failed. DBErr = %d, Name = %s, Msg = %s.", 
			         pstInsertRes->DBMSErrNo,
			         pstDecTianYuanRes->BillRecord.Pass9Account,
			         torm_error_string(pstInsertRes->ErrNo));

		/* ÐèÒª¼ÇÂ¼ÌõÈÕÖ¾ */
		stSSPkg.Body.DecTianYuanRes.Result = 1; /* ²åÈëÌìÔªÊ§°Ü*/
		bill_ss_backward_pkg(pstEnv,&stSSPkg,DEC_BILL_RES,pstDecTianYuanRes->BillRecord.Pass9Account);
		
		return -1;
	}
	
	/* ÎÒµÄÌì°¡,×ÜËã¿ÉÒÔ·µ»ØÁË */
	bill_ss_backward_pkg(pstEnv,&stSSPkg,DEC_BILL_RES,pstDecTianYuanRes->BillRecord.Pass9Account);	

	if( OPERATE_POINT_ADD == pstDecTianYuanRes->OperType )
	{
		tlog_info(pstEnv->pstLogCat, 0, 0,	
		"|2002|¼ÓµãÈ¯³É¹¦|Result|%u|Uin|%d|OperType|%u|TianYuan|%llu|account|%s|serverid|%u|amount|%llu|point|%llu|bonus|%llu|FreezePoint|%llu|FreezeLastTime|%u|",
		pstDecTianYuanRes->Result,
		pstDecTianYuanRes->Uin,
		pstDecTianYuanRes->OperType,
		pstDecTianYuanRes->TianYuan,
		pstDecTianYuanRes->BillRecord.Pass9Account,
		pstDecTianYuanRes->BillRecord.ServerID,
		pstDecTianYuanRes->BillRecord.Amount,
		pstDecTianYuanRes->BillRecord.Points,
		pstDecTianYuanRes->BillRecord.Bonus,
		pstDecTianYuanRes->BillRecord.FreezePoint,
		pstDecTianYuanRes->BillRecord.FreezeLastTime);	
	}
	else
	{
		tlog_info(pstEnv->pstLogCat, 0, 0,	
		"|2002|¿ÛµãÈ¯³É¹¦|Result|%u|Uin|%d|OperType|%u|TianYuan|%llu|account|%s|serverid|%u|amount|%llu|point|%llu|bonus|%llu|FreezePoint|%llu|FreezeLastTime|%u|",
		pstDecTianYuanRes->Result,
		pstDecTianYuanRes->Uin,
		pstDecTianYuanRes->OperType,
		pstDecTianYuanRes->TianYuan,
		pstDecTianYuanRes->BillRecord.Pass9Account,
		pstDecTianYuanRes->BillRecord.ServerID,
		pstDecTianYuanRes->BillRecord.Amount,
		pstDecTianYuanRes->BillRecord.Points,
		pstDecTianYuanRes->BillRecord.Bonus,
		pstDecTianYuanRes->BillRecord.FreezePoint,
		pstDecTianYuanRes->BillRecord.FreezeLastTime);	
	}

	/* ²åÈëÒ»¸ö¿ÛÇ®µÄ¼ÇÂ¼ */
	bill_db_insert_req(pstEnv,pstDecTianYuanRes,pstPkg->Head.AsynCallBackData+sizeof(iCallBackCmd),
			pstPkg->Head.AsynCallBackDataLen-sizeof(iCallBackCmd),DB_INSERT_BILL_DEC_RECORD);
 	return 0;
}
int bill_db_update_res(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
 	int iCallBackCmd = 0;	
	int iOffset = 0;
	TORMUPDATERES * pstRes;
	
	pstRes = &pstPkg->Body.UpdateRes;
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "bill_db_update_res error");
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);

	if( DB_UPDATE_BILL == iCallBackCmd )
	{
		BILL stBill;
		SSCASHBILLREQ *pstCashBillReq;
		SSPKG stSSPkg;
		
		memset(&stBill,0,sizeof(stBill));
		pstCashBillReq = (SSCASHBILLREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(SSCASHBILLREQ);
		if (TORM_ERR_IS_ERROR(pstRes->ErrNo))
		{
			SSPKG stSSPkg;
			
			tlog_error(pstEnv->pstLogCat, 0, 0, 
				         "insert Bill info from db failed. DBErr = %d, Name = %s, Msg = %s.", 
				         pstRes->DBMSErrNo,
				         pstCashBillReq->Pass9Account,
				         torm_error_string(pstRes->ErrNo));
			/* ÐèÒª»Ø¸ö³ö´í°ü */
			
			stSSPkg.Body.CashBillRes.Result = 99;
			stSSPkg.Body.CashBillRes.CashBillReq = *pstCashBillReq;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,CASH_BILL_RES,pstCashBillReq->Pass9Account);
			
			tlog_error(pstEnv->pstLogCat, 0, 0,	
				"|1005|³åÖµÐÞ¸ÄÊ§°Ü|messageid|%u|sequence|%llu|account|%s|serverid|%u|amount|%llu|point|%llu|bonus|%llu|",
				pstCashBillReq->MessageId,
				pstCashBillReq->Sequence,
				pstCashBillReq->Pass9Account,
				pstCashBillReq->ServerID,
				pstCashBillReq->Amount,
				pstCashBillReq->Points,
				pstCashBillReq->Bonus);				
			return -1;
		}
		
		tlog_info(pstEnv->pstLogCat, 0, 0,	
			"|2001|³åÖµ¸üÐÂ³É¹¦|messageid|%u|sequence|%llu|account|%s|serverid|%u|amount|%llu|point|%llu|bonus|%llu|",
			pstCashBillReq->MessageId,
			pstCashBillReq->Sequence,
			pstCashBillReq->Pass9Account,
			pstCashBillReq->ServerID,
			pstCashBillReq->Amount,
			pstCashBillReq->Points,
			pstCashBillReq->Bonus);
		/* ³É¹¦·µ»ØÒ»¸öÊý¾Ý°ü */
		
		stSSPkg.Body.CashBillRes.Result = 0;
		stSSPkg.Body.CashBillRes.CashBillReq = *pstCashBillReq;
		bill_ss_backward_pkg(pstEnv,&stSSPkg,CASH_BILL_RES,pstCashBillReq->Pass9Account);		

		/* ²åÈëÁ÷Ë®dbÀïÃæÈ¥ */
		bill_db_insert_req(pstEnv,pstCashBillReq,pstPkg->Head.AsynCallBackData+sizeof(iCallBackCmd),
				pstPkg->Head.AsynCallBackDataLen-sizeof(iCallBackCmd),DB_INSERT_BILL_CASH_RECORD);

		//³åÖµËÍÀñ°ü
		bill_db_select_req_propsgift(pstEnv,(char *)pstCashBillReq,
					sizeof(SSCASHBILLREQ),pstCashBillReq->Uin,QUERY_PROPSGIFT_BY_INFORMER);
				
	}
	else if( DB_UPDATE_BILL_DEC == iCallBackCmd)
	{
		bill_db_update_res_dec(pstEnv,pstPkg);
	}
	else if(DB_UPDATE_USER_FREEZE_BILL == iCallBackCmd)
	{
		bill_db_update_res_freeze(pstEnv,pstPkg);
	}
	else if( DB_UPDATE_PROPSGIFT == iCallBackCmd)
	{
		bill_db_update_res_propsgift(pstEnv,pstPkg);
	}
	else if (DB_UPDATE_ACCGIFT == iCallBackCmd)
	{
		bill_db_update_res_accgift(pstEnv,pstPkg);
	}
	else if (DB_UPDATE_PROPGIFT_CLT == iCallBackCmd)
	{
		bill_db_update_res_propsgift_clt(pstEnv,pstPkg);
	}
	else if (iCallBackCmd == DB_UPDATE_CDKEY_DEF)
	{
		bill_db_update_res_cdkey_def(pstEnv,pstPkg);
	}
	else
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "iCallBackCmd=%d", iCallBackCmd);
	}
	
 	return 0;
}


int bill_db_insert_res_cdkey_get(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
	int iCallBackCmd = 0;	
	int iOffset = 0;
	TORMUPDATERES * pstRes =  &pstPkg->Body.UpdateRes;
//	int iLogErr = -1;
	SSCDKEYREQ *pstSSReq;
	CDKEYTYPEDEF *pstTypeDef;
	
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		goto error;
	}

	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);
	pstSSReq = (SSCDKEYREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
	iOffset += sizeof(SSCDKEYREQ);
	pstTypeDef = (CDKEYTYPEDEF *)(pstPkg->Head.AsynCallBackData+iOffset);
	iOffset += sizeof(CDKEYTYPEDEF);

	if (TORM_ERR_IS_ERROR(pstRes->ErrNo))
	{
		goto error;
	}

	bill_ss_backward_pkg_cdkey(pstEnv,SS_CDKEY_RES, CDKEY_FAIL_NO, 
								pstSSReq->CDKey, pstTypeDef,pstSSReq->RoleName,1);
	return 0;

error:
	bill_ss_backward_pkg_cdkey(pstEnv,SS_CDKEY_RES, CDKEY_FAIL_SYS, 
								pstSSReq->CDKey, NULL,pstSSReq->RoleName,1);
	return -1;
}

int bill_db_insert_res_acc_gift(BILL_SVRENV *pstEnv,TORMPKG *pstOrmPkg)
{
	int iCallBackCmd = 0;	
	int iOffset = 0;
	TORMUPDATERES * pstRes =  &pstOrmPkg->Body.UpdateRes;
//	int iLogErr = -1;
	SSACCGIFTINSERTREQ *pstSSReq;
	int iOperType = 0;
	SSPKG stPkg;
	SSACCGIFTINSERTRES *pstSSRes = &stPkg.Body.AccGiftInsertRes;
	
	
	if(pstOrmPkg->Head.AsynCallBackDataLen <= 0 )
	{
		goto error;
	}

	memcpy(&iCallBackCmd,pstOrmPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);
	
	memcpy(&iOperType,pstOrmPkg->Head.AsynCallBackData+iOffset,sizeof(iOperType));
	iOffset += sizeof(iOperType);
	
	pstSSReq = (SSACCGIFTINSERTREQ *)(pstOrmPkg->Head.AsynCallBackData+iOffset);
	iOffset += sizeof(*pstSSReq);


	pstSSRes->Uin = pstSSReq->Uin;
	pstSSRes->ID = pstSSReq->ID;
	pstSSRes->RoleID = pstSSReq->RoleID;
	STRNCPY(pstSSRes->Pass9Account,pstSSReq->Pass9Account,sizeof(pstSSRes->Pass9Account));
	STRNCPY(pstSSRes->RoleName,pstSSReq->RoleName,sizeof(pstSSRes->RoleName));
	
	pstSSRes->AccGift = pstSSReq->AccGift;
	

	if (TORM_ERR_IS_ERROR(pstRes->ErrNo))
	{
		goto error;
	}

	bill_ss_backward_pkg(pstEnv,&stPkg,ACC_GIFT_INSERT_RES,pstSSReq->Pass9Account);
		
	return 0;

error:
	
	return -1;
}


int bill_db_update_res_cdkey_def(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
	int iCallBackCmd = 0;	
	int iOffset = 0;
	TORMUPDATERES * pstRes =  &pstPkg->Body.UpdateRes;
	int iLogErr = -1;
	SSCDKEYREQ *pstSSReq;
	CDKEYTYPEDEF *pstTypeDef;

	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		goto error;
	}

	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);
	pstSSReq = (SSCDKEYREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
	iOffset += sizeof(SSCDKEYREQ);
	pstTypeDef = (CDKEYTYPEDEF *)(pstPkg->Head.AsynCallBackData+iOffset);
	iOffset += sizeof(CDKEYTYPEDEF);

	if( DB_UPDATE_CDKEY_DEF!= iCallBackCmd )
	{
		iLogErr = -2;
		goto error;
	}

	if (TORM_ERR_IS_ERROR(pstRes->ErrNo))
	{
		iLogErr = -3;
		goto error;
	}

	if(pstRes->AffectRows != 1)
	{
		iLogErr = -4;
		goto error;
	}

	bill_ss_backward_pkg_cdkey(pstEnv,SS_CDKEY_RES, CDKEY_FAIL_NO, 
								pstSSReq->CDKey, pstTypeDef,pstSSReq->RoleName,1);
	return 0;

error:
	bill_ss_backward_pkg_cdkey(pstEnv,SS_CDKEY_RES, CDKEY_FAIL_SYS, 
								pstSSReq->CDKey, NULL,pstSSReq->RoleName,1);
	tlog_error(pstEnv->pstLogCat, 0, 0, "function:bill_db_update_res_cdkey_def error! errcode=%d",iLogErr);
	return -1;
}

int bill_db_update_res_propsgift_clt(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
	int iCallBackCmd = 0;	
	int iOffset = 0;
	TORMUPDATERES * pstRes =  &pstPkg->Body.UpdateRes;
	SSPROPGIFTLISTREQ *pstReq;
	PROPSGIFT *pstPropGift;
	SSPKG stSSPkg;
	int i = 0;
	
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);
	pstReq = (SSPROPGIFTLISTREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
	iOffset += sizeof(SSPROPGIFTLISTREQ);
	pstPropGift = (PROPSGIFT *)(pstPkg->Head.AsynCallBackData+iOffset);
	

	if( DB_UPDATE_PROPGIFT_CLT!= iCallBackCmd )
	{
		goto unlock;
	}

	memset(&stSSPkg,0,sizeof(stSSPkg));
	if (TORM_ERR_IS_ERROR(pstRes->ErrNo))
	{
		goto unlock;
	}

	if(pstRes->AffectRows != 1)
	{
		goto unlock;
	}

	stSSPkg.Body.PropGiftListRes.Uin = pstReq->Uin;
	stSSPkg.Body.PropGiftListRes.ID = pstReq->ID;
	stSSPkg.Body.PropGiftListRes.RoleID = pstReq->RoleID;
	stSSPkg.Body.PropGiftListRes.PropGiftInfo.Count = 1;
	stSSPkg.Body.PropGiftListRes.PropGiftInfo.PropsGift[0] = *pstPropGift;
	//stSSPkg.Body.PropGiftListRes.Count = 1;
	//stSSPkg.Body.PropGiftListRes.PropGifts[0] = *pstPropGift;
	STRNCPY(stSSPkg.Body.PropGiftListRes.RoleName, pstReq->RoleName, 
					sizeof(stSSPkg.Body.PropGiftListRes.RoleName));

	for (i = 0; i < pstEnv->iPropsDef; i ++)
	{
		PROPSDEF * pstPropsDef = (PROPSDEF *)pstEnv->sPropsDef[i];
		if(pstPropsDef->GroupID == (unsigned int)pstReq->ID && 
			pstPropsDef->CashUp == pstReq->CashUp)
		{
			stSSPkg.Body.PropGiftListRes.PropGiftInfo.DefCount = 1;
			stSSPkg.Body.PropGiftListRes.PropGiftInfo.PropsDef[0] = *pstPropsDef;
			break;
		}
	}


	// ÕâÀïÐèÒª½âËø
	bill_ss_backward_pkg(pstEnv,&stSSPkg,PROP_GIFT_RES,pstReq->Pass9Account);

	bill_db_propsgift2region(pstEnv,pstPropGift,pstReq->Pass9Account);
	return 0;

unlock:
	
	if (pstReq->Pass9Account[0] != 0)
	{
		bill_propsgift_clt(pstEnv, pstReq,PROPS_GIFT_LIST_RET_UPDATE);
		z_remove_bill_lock(pstEnv, pstReq->Pass9Account);
	}
	return -1;
}

int bill_propsgift_clt(BILL_SVRENV *pstEnv,SSPROPGIFTLISTREQ *pstReq,int iRetCode)
{
	SSPKG stSSPkg;

	memset(&stSSPkg,0,sizeof(stSSPkg));

	//memset(&stSSPkg.Body.PropGiftListRes,0,sizeof(stSSPkg.Body.PropGiftListRes));

	stSSPkg.Body.PropGiftListRes.RetCode = iRetCode;
	stSSPkg.Body.PropGiftListRes.Uin = pstReq->Uin;
	stSSPkg.Body.PropGiftListRes.ID = pstReq->ID;
	stSSPkg.Body.PropGiftListRes.RoleID = pstReq->RoleID;

	STRNCPY(stSSPkg.Body.PropGiftListRes.RoleName, pstReq->RoleName, 
					sizeof(stSSPkg.Body.PropGiftListRes.RoleName));

	// ÕâÀïÐèÒª½âËø
	bill_ss_backward_pkg(pstEnv,&stSSPkg,PROP_GIFT_RES,pstReq->Pass9Account);

	return 0;

}




int bill_db_update_res_accgift(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
	int iCallBackCmd = 0;	
	int iOffset = 0;
	TORMUPDATERES * pstRes =  &pstPkg->Body.UpdateRes;
	SSACCGIFTLISTREQ *pstReq;
	ACCGIFT *pstAccGift;
	int iOperType = 0;
	SSPKG stSSPkg;
	
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "bill_db_update_res_accgift error");
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);
	memcpy(&iOperType,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iOperType));
	iOffset += sizeof(iOperType);
	pstReq = (SSACCGIFTLISTREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
	iOffset += sizeof(SSACCGIFTLISTREQ);
	pstAccGift = (ACCGIFT *)(pstPkg->Head.AsynCallBackData+iOffset);
	

	if( DB_UPDATE_ACCGIFT!= iCallBackCmd )
	{
		goto unlock;
	}

	memset(&stSSPkg,0,sizeof(stSSPkg));
	if (TORM_ERR_IS_ERROR(pstRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			         "update AccGift info from db failed. DBErr = %d, Name = %s, Msg = %s.", 
			         pstRes->DBMSErrNo,
			        pstReq->Pass9Account,
			         torm_error_string(pstRes->ErrNo));
		goto unlock;
	}

	if(pstRes->AffectRows != 1)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "bill_db_update_res error");
		goto unlock;
	}

	stSSPkg.Body.AccGiftListRes.Uin = pstReq->Uin;
	stSSPkg.Body.AccGiftListRes.ID = pstReq->ID;
	stSSPkg.Body.AccGiftListRes.RoleID = pstReq->RoleID;
	stSSPkg.Body.AccGiftListRes.Count = 1;
	stSSPkg.Body.AccGiftListRes.AccGifts[0] = *pstAccGift;
	STRNCPY(stSSPkg.Body.AccGiftListRes.RoleName, pstReq->RoleName, sizeof(stSSPkg.Body.AccGiftListRes.RoleName));

	// ÕâÀïÐèÒª½âËø
	bill_ss_backward_pkg(pstEnv,&stSSPkg,ACC_GIFT_LIST_RES,pstReq->Pass9Account);
	return 0;

unlock:
	if (pstReq->Pass9Account[0] != 0)
		z_remove_bill_lock(pstEnv, pstReq->Pass9Account);
	return -1;
}

int bill_db_delete_res(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
 	int iCallBackCmd = 0;	
	int iOffset = 0;
	TORMDELETERES * pstRes;
	UNUSED(pstEnv);
	
	pstRes = &pstPkg->Body.DeleteRes;
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);

	if( DB_DELETE_PROPS == iCallBackCmd )
	{
		return -1;
	}
	
 	return 0;
}


int bill_db_select_count_cdkeydef_res(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
	TORMSELECTCOUNTRES *pstSelectCountRes;
	int iRecordNum = 0;
	int iCallBackCmd = 0;	
	int iOffset = 0;

	CDKEYTYPEDEF stCDKeyTypeDef;
	SSCDKEYREQ stCDKeyReq;
	int iRet = CDKEY_FAIL_SYS;
	int iLogErr = 0;

	
	pstSelectCountRes = &pstPkg->Body.SelectCountRes;
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "function:bill_db_select_count_cdkeydef_res error!errcode=%d",-1);
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);
	memcpy(&stCDKeyReq,pstPkg->Head.AsynCallBackData+iOffset,sizeof(stCDKeyReq));
	iOffset += sizeof(stCDKeyReq);
	memcpy(&stCDKeyTypeDef,pstPkg->Head.AsynCallBackData+iOffset,sizeof(stCDKeyTypeDef));
	iOffset += sizeof(stCDKeyTypeDef);

	if (TORM_ERR_IS_ERROR(pstSelectCountRes->ErrNo))
	{
		iLogErr = -2;
		goto unlock;
	}
	
	if( DB_SELECT_CDKEYDEF_COUNT != iCallBackCmd )
	{
		iLogErr = -3;
		goto unlock;
	}

	iRecordNum = pstSelectCountRes->RowsCount;
	if (stCDKeyTypeDef.LimitNum != 0 && iRecordNum >= stCDKeyTypeDef.LimitNum)
	{
		//iLogErr = -4;
		// ½âËø   Í¨Öª¿Í»§¶Ë todo:
		iRet = CDKEY_FAIL_DUMP;
		goto unlock;
	}

	// select cdkey
	if (bill_db_select_cdkey(pstEnv,&stCDKeyReq, &stCDKeyTypeDef, DB_SELECT_CDKEYDEF) < 0)
	{
		iLogErr = -5;
		goto unlock;
	}
	return 0;

unlock:
	bill_ss_backward_pkg_cdkey(pstEnv,SS_CDKEY_RES, iRet, stCDKeyReq.CDKey, NULL, stCDKeyReq.RoleName,1);
	if (iLogErr < 0)
		tlog_error(pstEnv->pstLogCat, 0, 0, "function:bill_db_select_count_cdkeydef_res error!errcode=%d",iLogErr);
	return -1;
}


int bill_db_select_count_res(BILL_SVRENV *pstEnv,TORMPKG *pstPkg)
{
 	int iCallBackCmd = 0;	
	int iOffset = 0;
	TORMSELECTCOUNTRES * pstRes;
	UNUSED(pstEnv);
	
	pstRes = &pstPkg->Body.SelectCountRes;
	if(pstPkg->Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}
	memcpy(&iCallBackCmd,pstPkg->Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);

	if( DB_SELECT_COUNT_PROPSDEF == iCallBackCmd )
	{
		bill_db_select_count_propsdef_res(pstEnv,pstPkg);
	}
	else if(DB_SELECT_CDKEYDEF_COUNT == iCallBackCmd)
	{
		 bill_db_select_count_cdkeydef_res(pstEnv,pstPkg);
	}
	
 	return 0;
}

int bill_process_torm_msg(BILL_SVRENV *pstEnv,char *pszMsg,int iMsgLen )
{
	TDRDATA stData;
	TORMPKG stPkg;
	int iRet = 0;
	int iProtoVer = pstEnv->iDBProtoVer;
	
	memset(&stPkg,0,sizeof(stPkg));
	stData.pszBuff = (char *)pszMsg;
	stData.iBuff = iMsgLen;
	iRet =  torm_attach_res_msg(pstEnv->hTorm, &stData, &stPkg, iProtoVer);
	if (0 >iRet)
	{
		tlog_error(pstEnv->pstLogCat,0,0, 
			         "torm_attach_res_msg error:%s, iProtoVer=%d", 
		           torm_error_string(iRet), iProtoVer);
		return -1;
	}

	switch(stPkg.Head.HeadCmd.Cmd)
	{
	case TORM_EXGVER_RES:
		pstEnv->iDBProtoVer = stPkg.Body.ExgVerRes.Version;
		break;
	case TORM_SELECT_RES:	
		iRet = bill_db_select_res( pstEnv, &stPkg );
		break;
	case TORM_INSERT_RES:
		iRet = bill_db_insert_res( pstEnv, &stPkg );
		break;
	case TORM_UPDATE_RES:
		iRet = bill_db_update_res( pstEnv, &stPkg );
		break;
	case TORM_DELETE_RES:
		iRet = bill_db_delete_res( pstEnv, &stPkg );
		break;
	case TORM_SELECT_COUNT_RES:
		iRet = bill_db_select_count_res(pstEnv,&stPkg);
		break;

	default:
		iRet = -1;
		tlog_error(pstEnv->pstLogCat,0,0, "bad TORM cmd received:%d",stPkg.Head.HeadCmd.Cmd);
		break;
	}
	
	return 0;
}

// ÇëÇó»ñÈ¡accountÐÅÏ¢¡£
int bill_process_accountdb_req(BILL_SVRENV *pstEnv, char *Pass9Account,char *szCallBackBuffer,int iLen )
{
	ACCOUNT   	stAccount;
	TORMPKG 	stPkg;
	TORMSELECTREQ *pstDBReq = &stPkg.Body.SelectReq;
	int iRet = 0;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TDRDATA stData;
	int iProtoVer;
	int iSrc = 0;
	int iDst = 0;
	
	STRNCPY(stAccount.Pass9Account, Pass9Account,sizeof(stAccount.Pass9Account));
	bill_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,&stPkg.Head, TORM_SELECT_REQ,DB_SELECT_ACCOUNT_UIN, 
	       "Account", szCallBackBuffer, iLen);
	
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	snprintf(pstDBReq->OperInfo.WhereDef,
	         sizeof(pstDBReq->OperInfo.WhereDef),
	          "where Pass9Account=\'%s\'", 
		       Pass9Account);
		

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stAccount;
	stMetaData.iBuff = sizeof(stAccount);
	iRet =  torm_make_req_msg(pstEnv->hTorm, &stNetData, &stPkg, &stMetaData, pstEnv->iDBProtoVer);
	if(0 !=iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail %s",torm_error_string(iRet));
		return -1;
	}
	
	iProtoVer = pstEnv->iDBProtoVer;
	stData.pszBuff = pstEnv->szSendBuffer;
	stData.iBuff = sizeof(pstEnv->szSendBuffer);
	if(0 != torm_pack_msg(pstEnv->hTorm, &stData, &stPkg, iProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_pack_msg error");
		return -1;
	}
	
	iSrc = pstEnv->pstAppCtx->iId;
	iDst = pstEnv->iAccountDBID;
	if(tbus_forward(pstEnv->pstAppCtx->iBus, &iSrc, &iDst, stData.pszBuff, stData.iBuff, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,"tbus_forward auth_svr fail.");
	}

	return 0;
}


int bill_process_accountdb_torm_msg(BILL_SVRENV *pstEnv,char *pszMsg,int iMsgLen )
{
	TDRDATA stData;
	TORMPKG stPkg;
	int iRet = 0;
	int iProtoVer = pstEnv->iDBProtoVer;
	SSPKG stPkgReq;
	SSPKG stPkgRes;
	int iCallBackCmd = 0;	
	int iOffset = 0;
	TORMSELECTRES * pstSelectRes;
	ACCOUNT stAccount;

	memset(&stAccount,0,sizeof(stAccount));
	memset(&stPkg,0,sizeof(stPkg));
	stData.pszBuff = (char *)pszMsg;
	stData.iBuff = iMsgLen;
	iRet =  torm_attach_res_msg(pstEnv->hTorm, &stData, &stPkg, iProtoVer);
	if (0 >iRet)
	{
		tlog_error(pstEnv->pstLogCat,0,0, 
			         "torm_attach_res_msg error:%s, iProtoVer=%d", 
		           torm_error_string(iRet), iProtoVer);
		return -1;
	}
	if(stPkg.Head.HeadCmd.Cmd != TORM_SELECT_RES)
	{
		return -1;
	}	
	pstSelectRes = &stPkg.Body.SelectRes;
	if(stPkg.Head.AsynCallBackDataLen <= 0 )
	{
		return -1;
	}
	memcpy(&iCallBackCmd,stPkg.Head.AsynCallBackData+iOffset,sizeof(iCallBackCmd));
	iOffset += sizeof(iCallBackCmd);
	memcpy(&stPkgReq.Head,stPkg.Head.AsynCallBackData+iOffset,sizeof(stPkgReq.Head));
	iOffset += sizeof(stPkgReq.Head);
	
	if( DB_SELECT_ACCOUNT_UIN == iCallBackCmd &&
		CASH_BILL_REQ == stPkgReq.Head.Cmd )
	{
		memcpy(&stPkgReq.Body.CashBillReq,stPkg.Head.AsynCallBackData+iOffset,sizeof(stPkgReq.Body.CashBillReq));
		iOffset += sizeof(stPkgReq.Body.CashBillReq);
		
		if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo) ||
			pstSelectRes->SelectResult.ResultNum == 0 )
		{
			stPkgRes.Body.CashBillRes.Result = 99;
			stPkgRes.Body.CashBillRes.CashBillReq = stPkgReq.Body.CashBillReq;
			bill_ss_backward_pkg(pstEnv,&stPkgRes,CASH_BILL_RES,NULL);

			tlog_error(pstEnv->pstLogCat, 0, 0,	"bill_process_accountdb_torm_msg fail,Name = %s,",
							stPkgReq.Body.CashBillReq.Pass9Account);
			return -1;
		}
		else if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stAccount, sizeof(stAccount))< 0 )
		{
			stPkgRes.Body.CashBillRes.Result = 99;
			stPkgRes.Body.CashBillRes.CashBillReq = stPkgReq.Body.CashBillReq;
			bill_ss_backward_pkg(pstEnv,&stPkgRes,CASH_BILL_RES,NULL);

			tlog_error(pstEnv->pstLogCat, 0, 0,	"bill_process_accountdb_torm_msg fail,Name = %s,",
							stPkgReq.Body.CashBillReq.Pass9Account);
			return -1;
		}
		else if (0 != strcmp(stAccount.Pass9Account, stPkgReq.Body.CashBillReq.Pass9Account))
		{
			stPkgRes.Body.CashBillRes.Result = 99;
			stPkgRes.Body.CashBillRes.CashBillReq = stPkgReq.Body.CashBillReq;
			bill_ss_backward_pkg(pstEnv,&stPkgRes,CASH_BILL_RES,NULL);
			tlog_error(pstEnv->pstLogCat, 0, 0,	"cmp account fail, %s != %s",
							stAccount.Pass9Account,stPkgReq.Body.CashBillReq.Pass9Account);
			return -1;
		}
		
		/* ³äÖµÇëÇó */
		stPkgReq.Body.CashBillReq.Uin = stAccount.Uin;
		bill_process_region_cash_bill(pstEnv,&stPkgReq.Body.CashBillReq); 
		
	}
	else if( DB_SELECT_ACCOUNT_UIN == iCallBackCmd &&
			 QUERY_BILL_REQ == stPkgReq.Head.Cmd )
	{
		memcpy(&stPkgReq.Body.QueryBillReq,stPkg.Head.AsynCallBackData+iOffset,sizeof(stPkgReq.Body.QueryBillReq));
		iOffset += sizeof(stPkgReq.Body.QueryBillReq);
		
		if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo) ||
			pstSelectRes->SelectResult.ResultNum == 0 )
		{
			/* ÐèÒª»Ø¸ö³ö´í°ü */
			SSPKG stSSPkg;
			memset(&stSSPkg,0,sizeof(stSSPkg));
			stSSPkg.Body.QueryBillRes.Result = 99;
			stSSPkg.Body.QueryBillRes.MessageId = stPkgReq.Body.QueryBillReq.MessageId;
			stSSPkg.Body.QueryBillRes.Sequence = stPkgReq.Body.QueryBillReq.Sequence;
			stSSPkg.Body.QueryBillRes.QueryTs = time(NULL);
			bill_ss_backward_pkg(pstEnv,&stSSPkg,QUERY_BILL_RES,NULL); 

			tlog_error(pstEnv->pstLogCat, 0, 0,	"QueryBillReq fail,account = %s,",
							stPkgReq.Body.QueryBillReq.Pass9Account);
			return -1;
		}
		else if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stAccount, sizeof(stAccount))< 0 )
		{
			/* ÐèÒª»Ø¸ö³ö´í°ü */
			SSPKG stSSPkg;
			memset(&stSSPkg,0,sizeof(stSSPkg));
			stSSPkg.Body.QueryBillRes.Result = 99;
			stSSPkg.Body.QueryBillRes.MessageId = stPkgReq.Body.QueryBillReq.MessageId;
			stSSPkg.Body.QueryBillRes.Sequence = stPkgReq.Body.QueryBillReq.Sequence;
			stSSPkg.Body.QueryBillRes.QueryTs = time(NULL);
			bill_ss_backward_pkg(pstEnv,&stSSPkg,QUERY_BILL_RES,	NULL); 

			tlog_error(pstEnv->pstLogCat, 0, 0,	"QueryBillReq fail,account = %s,",
							stPkgReq.Body.QueryBillReq.Pass9Account);
			return -1;
		}
		else if (0 != strcmp(stAccount.Pass9Account, stPkgReq.Body.CashBillReq.Pass9Account))
		{
			/* ÐèÒª»Ø¸ö³ö´í°ü */
			SSPKG stSSPkg;
			memset(&stSSPkg,0,sizeof(stSSPkg));
			stSSPkg.Body.QueryBillRes.Result = 99;
			stSSPkg.Body.QueryBillRes.MessageId = stPkgReq.Body.QueryBillReq.MessageId;
			stSSPkg.Body.QueryBillRes.Sequence = stPkgReq.Body.QueryBillReq.Sequence;
			stSSPkg.Body.QueryBillRes.QueryTs = time(NULL);
			bill_ss_backward_pkg(pstEnv,&stSSPkg,QUERY_BILL_RES,	NULL); 
			
			tlog_error(pstEnv->pstLogCat, 0, 0,	"QueryBillReq fail,account = %s,",
							stPkgReq.Body.QueryBillReq.Pass9Account);
			return -1;
		}
		
		/* ²éÑ¯Óà¶î */
		stPkgReq.Body.QueryBillReq.Uin = stAccount.Uin;
		bill_process_region_query_bill(pstEnv,&stPkgReq.Body.QueryBillReq);
		
	}
	else if( DB_SELECT_ACCOUNT_UIN == iCallBackCmd &&
			 BILL_CASH_PROPSGIFT_REQ == stPkgReq.Head.Cmd )
	{
		memcpy(&stPkgReq.Body.QueryBillReq,stPkg.Head.AsynCallBackData+iOffset,sizeof(stPkgReq.Body.QueryBillReq));
		iOffset += sizeof(stPkgReq.Body.QueryBillReq);
		
		if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo) ||
			pstSelectRes->SelectResult.ResultNum == 0 )
		{
			/* ÐèÒª»Ø¸ö³ö´í°ü */
			SSPKG stSSPkg;
			memset(&stSSPkg.Body.CashPropsGiftInfoRes,0,sizeof(stSSPkg.Body.CashPropsGiftInfoRes));
			STRNCPY(stSSPkg.Body.CashPropsGiftInfoRes.Pass9Account,stPkgReq.Body.CashPropsGiftInfoReq.Pass9Account,
				CCH(stSSPkg.Body.CashPropsGiftInfoRes.Pass9Account));
			stSSPkg.Body.CashPropsGiftInfoRes.Result = 1;
			stSSPkg.Body.CashPropsGiftInfoRes.CsConnCtx = stPkgReq.Body.CashPropsGiftInfoReq.CsConnCtx;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_CASH_PROPSGIFT_RES,NULL); 


			tlog_error(pstEnv->pstLogCat, 0, 0,	"BILL_CASH_PROPSGIFT_REQ fail,account = %s,",
							stPkgReq.Body.CashPropsGiftInfoReq.Pass9Account);
			return -1;
		}
		else if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stAccount, sizeof(stAccount))< 0 )
		{
			/* ÐèÒª»Ø¸ö³ö´í°ü */
			SSPKG stSSPkg;
			memset(&stSSPkg.Body.CashPropsGiftInfoRes,0,sizeof(stSSPkg.Body.CashPropsGiftInfoRes));
			STRNCPY(stSSPkg.Body.CashPropsGiftInfoRes.Pass9Account,stPkgReq.Body.CashPropsGiftInfoReq.Pass9Account,
				CCH(stSSPkg.Body.CashPropsGiftInfoRes.Pass9Account));
			stSSPkg.Body.CashPropsGiftInfoRes.Result = 1;
			stSSPkg.Body.CashPropsGiftInfoRes.CsConnCtx = stPkgReq.Body.CashPropsGiftInfoReq.CsConnCtx;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_CASH_PROPSGIFT_RES,NULL); 

			tlog_error(pstEnv->pstLogCat, 0, 0,	"BILL_CASH_PROPSGIFT_REQ fail,account = %s,",
							stPkgReq.Body.CashPropsGiftInfoReq.Pass9Account);
			return -1;
		}
		else if (0 != strcmp(stAccount.Pass9Account, stPkgReq.Body.CashBillReq.Pass9Account))
		{
			/* ÐèÒª»Ø¸ö³ö´í°ü */
			SSPKG stSSPkg;
			memset(&stSSPkg.Body.CashPropsGiftInfoRes,0,sizeof(stSSPkg.Body.CashPropsGiftInfoRes));
			STRNCPY(stSSPkg.Body.CashPropsGiftInfoRes.Pass9Account,stPkgReq.Body.CashPropsGiftInfoReq.Pass9Account,
				CCH(stSSPkg.Body.CashPropsGiftInfoRes.Pass9Account));
			stSSPkg.Body.CashPropsGiftInfoRes.Result = 1;
			stSSPkg.Body.CashPropsGiftInfoRes.CsConnCtx = stPkgReq.Body.CashPropsGiftInfoReq.CsConnCtx;
			bill_ss_backward_pkg(pstEnv,&stSSPkg,BILL_CASH_PROPSGIFT_RES,NULL); 
			
			tlog_error(pstEnv->pstLogCat, 0, 0,	"BILL_CASH_PROPSGIFT_REQ fail,account = %s,",
							stPkgReq.Body.CashPropsGiftInfoReq.Pass9Account);
			return -1;
		}
		
		/* ²éÑ¯Óà¶î */
		stPkgReq.Body.CashPropsGiftInfoReq.Uin = stAccount.Uin;
		bill_db_select_req_propsgift(pstEnv,(char *)&stPkgReq.Body.CashPropsGiftInfoReq,sizeof(SSCASHPROPSGIFTINFOREQ),
			stPkgReq.Body.CashPropsGiftInfoReq.Uin,CASH_PROPSGIFT_BY_CS);
	}
	return 0;
}

int bill_svr_proc(TAPPCTX *pstAppCtx, BILL_SVRENV *pstEnv)
{
	int iBusy=0;
	int iLoopNum = 0;
	TBUSADDR iMsgDst ;
	TBUSADDR iMsgSrc ;
	int iMsgLen;
	char *pszMsg;

	while (100 > iLoopNum)
	{
		iLoopNum++;
		
		iMsgDst = pstAppCtx->iId;
		iMsgSrc = TBUS_ADDR_ALL;
		iMsgLen = sizeof(pstEnv->szRecvBuffer) ;
		pszMsg = (char *)(pstEnv->szRecvBuffer);	
		if ( TBUS_SUCCESS != tbus_recv(pstAppCtx->iBus, &iMsgSrc, &iMsgDst, 
			(void *)pszMsg, (size_t *)&iMsgLen, 0) )
		{
			break;
		}
		
		iBusy = 1;
		if( iMsgSrc ==  pstEnv->iRegionID )
		{
			bill_process_region_msg(pstEnv,pstEnv->szRecvBuffer,iMsgLen);
		}
		else if(iMsgSrc == pstEnv->iBillDBID )
		{
			bill_process_torm_msg(pstEnv,pszMsg,iMsgLen);
		}
		else if(iMsgSrc == pstEnv->iAccountDBID )
		{
			bill_process_accountdb_torm_msg(pstEnv,pszMsg,iMsgLen);
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

int bill_svr_reload(TAPPCTX *pstAppCtx, BILL_SVRENV *pstEnv)
{
	BILL_SVRCONF   *pstPreConf;
	
	pstPreConf = (BILL_SVRCONF   *)pstAppCtx->stConfPrepareData.pszBuff;
	
	printf("bill_svr reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "bill_svr reload");

	bill_db_select_count_propsdef_req(pstEnv);
	
	return 0;
}

int bill_svr_fini(TAPPCTX *pstAppCtx, BILL_SVRENV *pstEnv)
{
	UNUSED(pstAppCtx);
	printf("bill_svr finish\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "bill_svr finish");
	return 0;
}

int bill_svr_tick(TAPPCTX *pstAppCtx, BILL_SVRENV *pstEnv)
{
	UNUSED(pstAppCtx);

	z_bill_lock_tick( pstEnv,1000);
	z_bill_cdkey_lock_tick(pstEnv,1000);
	return 0;
}

int bill_svr_stop(TAPPCTX *pstAppCtx, BILL_SVRENV *pstEnv)
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

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)bill_svr_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_bill_svr;
	gs_stAppCtx.stConfData.pszMetaName = "bill_svrconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);
		
	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)bill_svr_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)bill_svr_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)bill_svr_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)bill_svr_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)bill_svr_stop;
	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	tapp_def_fini(&gs_stAppCtx, pvArg);
	
	return iRet;
}
