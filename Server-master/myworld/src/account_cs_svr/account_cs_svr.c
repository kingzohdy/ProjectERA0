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
#include "account_cs_svr_conf_desc.h"
#include "version.h"
#include "cs_net.h"
#include "comm_misc.h"
#include "entity.h"
#include "tbus/tbus.h"
#include "tconnapi.h"
#include "proto_ss.h"
#include "comm/tmempool.h"
#include "zone_svr.h"
#include "apps/tormapi/torm_protocol_api.h"

#define LOOP_NUM 100
#define MAX_MSG 64000*2

//#define UNUSED(arg) ((void)(arg))
#pragma pack(1)

struct tagaccount_cs_svrEnv
{
	ACCOUNT_CS_SVRCONF *pstConf;
	ACCOUNT_CS_SVRRUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;

	unsigned long int iTconnapiHandle;
	//LPNETHANDLE pstCSHan;
	LPSSHANDLE pstSSHan;

	LPTDRMETALIB pstProtoLib;
	LPTDRMETA pstProtoMeta;
	TORMPROTOCOLHANDLE hTorm;
	
	// 连接进程ID
	int iAuthSvrID;
	//int iAuthBakID;
	//int iAuthSlaveID;
	int iAccountCSConnId;
	int iAccountDBID;
	int iAccountDBProtoVer;
	int iRegionSvrID;

	ZoneObj *pstZoneObj;
	int iShmID;
	LPSHTABLE pstShtNameMemID; 

	char cIsAttach;

	SSPKG stSSPkg;
	TORMPKG stTormPkg;
	TFRAMEHEAD stIncomingFrameHead;
	TFRAMEHEAD stOutcomingFrameHead;

	char szRecvBuffer[MAX_MSG];
	char szSendBuffer[MAX_MSG];
	
};

typedef struct tagaccount_cs_svrEnv		ACCOUNT_CS_SVRENV;
typedef struct tagaccount_cs_svrEnv		*LACCOUNT_CS_SVRENV;


static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_account_cs_svr[];
extern unsigned char g_szMetalib_proto_ss[];

ACCOUNT_CS_SVRENV gs_stEnv;
static ACCOUNT_CS_SVRRUN_STATUS gs_stStat;

typedef struct tagConnCtx
{
	int iConnIdx;                        
	TIPINFO stIPInfo; 	
}ConnCtx;

typedef union tagTormCallbackExtra
{
	char szLoginName[INFORMER_ACCOUNT_LOGIN_NAME_LEN];
	ACCSETFREEZEREQ stAccSetFreezeReq;
	ACCSETSILENCEREQ stAccSetSilenceReq;
	ACCSETUINFLAGREQ stAccSetUinFlagReq;
	BILLRECORD stQueryBillRecordReq;
	SETACCSAFELOCKREQ stSetAccSafeLockReq;
}TormCallbackExtra;

typedef struct tagTormCallbackCtx
{
	int iCmd;
	ConnCtx stConnInfo;
	int iRealCmd;
	TormCallbackExtra stData;
}TormCallbackCtx;

enum {
	DB_RES_REAL = 0,
	DB_RES_ACCOUNT_INFO,
	DB_RES_ACCOUNT_LITE,
	DB_RES_ACCOUNT_ROLELIST,
	DB_RES_ACCOUNT_ROLE_UPDATE_DELTIME,
	DB_RES_ACCOUNT_FREEZE,
	DB_RES_ACCOUNT_SILENCE,
	DB_RES_ACCOUNT_ROLE_UPDATE_NAME,
	DB_RES_ACCOUNT_UIN_FLAG,
	DB_RES_SELECT_BILL_RECORD,
	DB_RES_SELECT_ACCOUNT_PASSPWD,
	DB_RES_SET_PASSPWD,
	
};

#pragma pack() 

// 处理connd发来的消息
static int acc_cs_process_conn_msg(TAPPCTX *pstAppCtx,
                                   ACCOUNT_CS_SVRENV *pstEnv, char *pszMsg,
                                   int iMsgLen);

// 处理auth_svr发来的消息
static int acc_cs_process_auth_msg(TAPPCTX *pstAppCtx,
                                   ACCOUNT_CS_SVRENV *pstEnv, char *pszMsg,
                                   int iMsgLen);

// 处理account_db发来的消息
static int acc_cs_process_db_msg(TAPPCTX *pstAppCtx,
                                 ACCOUNT_CS_SVRENV *pstEnv, char *pszMsg,
                                 int iMsgLen);

// 从帐号名精确的查找帐号信息
static int db_get_account_info_req(TAPPCTX *pstAppCtx,
                                   ACCOUNT_CS_SVRENV *pstEnv,
                                   const char *pszLoginName, int iRealCmd,
                                   const void *pvData, int iDataLen);

// 从帐号名精确的查找帐号信息
static int ss_get_account_info(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
                               const ACCGETINFOREQ *pstReq);

// 恢复被删除的角色
static int ss_restore_role(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
                           const SSRESTOREROLEREQ *pstReq);

// 设置冻结状态													
static int ss_set_account_freeze(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
			                     const ACCSETFREEZEREQ *pstReq);

// 设置禁言状态
static int ss_set_account_silence(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
			                      const ACCSETSILENCEREQ *pstReq);

// 设置Account 的UinFlag
static int ss_set_account_uin_flag(TAPPCTX *pstAppCtx,
                                   ACCOUNT_CS_SVRENV *pstEnv,
			                       const ACCSETUINFLAGREQ *pstReq);

// 模糊查找帐号角色列表
static int ss_find_account_rolelist(TAPPCTX *pstAppCtx,
                                    ACCOUNT_CS_SVRENV *pstEnv,
                                    const ACCFINDROLELISTREQ *pstReq);

// 模糊查找帐号信息
static int ss_find_account(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
                           const ACCFINDREQ *pstReq);

static int db_set_account_uin_flag_req(TAPPCTX *pstAppCtx,
                                       ACCOUNT_CS_SVRENV *pstEnv,
			                           int iUin,
			                           const ACCSETUINFLAGREQ *pstReq);

static int db_will_account_uin_flag_req(TAPPCTX *pstAppCtx,
                                        ACCOUNT_CS_SVRENV *pstEnv,
	                                    TORMPKG *pstPkg,
	                                    TormCallbackCtx *pstCallbackCtx);

static void db_init_torm_head(TAPPCTX* pstAppCtx, TORMPKGHEAD *pstHead,
                              int iCmd, char *pszMetaName, int iObID);

static void db_set_callback_ctx(TORMPKGHEAD *pstHead, int iCmd, int iRealCmd,
                                const TFRAMEHEAD *pstFH, const void *pvData,
                                int iDataLen);

static int db_torm_send(TAPPCTX* pstAppCtx, ACCOUNT_CS_SVRENV* pstEnv, 
	                    TORMPKG *pstPkg, int iDstID);

static int db_send_req(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv, 
	                   const void *pvData, int iDataLen);

static int db_find_account_req(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
                               int iByWhat, int iIndex, const char *pszPattern);

static int db_will_account_silence_req(TAPPCTX *pstAppCtx,
                                       ACCOUNT_CS_SVRENV *pstEnv,
                                       TORMPKG *pstPkg,
                                       TormCallbackCtx *pstCallbackCtx);

static int db_will_account_freeze_req(TAPPCTX *pstAppCtx,
                                      ACCOUNT_CS_SVRENV *pstEnv, 
                                      TORMPKG *pstPkg,
                                      TormCallbackCtx *pstCallbackCtx);

static int db_set_account_freeze_req(TAPPCTX *pstAppCtx,
                                     ACCOUNT_CS_SVRENV *pstEnv,
			                         int iUin, const ACCSETFREEZEREQ *pstReq);

static int db_set_account_silence_req(TAPPCTX *pstAppCtx,
                                      ACCOUNT_CS_SVRENV *pstEnv,
			                          int iUin, const ACCSETSILENCEREQ *pstReq);

static int db_account_lite_res(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv, 
	                           TORMPKG *pstPkg,
	                           TormCallbackCtx *pstCallbackCtx);

static int db_account_info_res(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv, 
	                           TORMPKG *pstPkg,
	                           TormCallbackCtx *pstCallbackCtx);

static int db_account_rolelist_res(TAPPCTX *pstAppCtx,
                                   ACCOUNT_CS_SVRENV *pstEnv, TORMPKG *pstPkg, 
	                               TormCallbackCtx *pstCallbackCtx);

static int db_account_role_update_deltime_res(TAPPCTX *pstAppCtx, 
	                                          ACCOUNT_CS_SVRENV *pstEnv, 
	                                          TORMPKG *pstPkg, 
	                                          TormCallbackCtx *pstCallbackCtx);

static int db_account_silence_res(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv, 
	                              TORMPKG *pstPkg,
	                              TormCallbackCtx *pstCallbackCtx);

static int db_account_freeze_res(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv, 
	                             TORMPKG *pstPkg,
	                             TormCallbackCtx *pstCallbackCtx);

static int db_account_uin_flag_res(TAPPCTX *pstAppCtx,
                                   ACCOUNT_CS_SVRENV *pstEnv, 
	                               TORMPKG *pstPkg,
	                               TormCallbackCtx *pstCallbackCtx);

static int db_l2u_res(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv, 
	                  TORMPKG *pstPkg, TormCallbackCtx *pstCallbackCtx);

static int conn_send_pkg(ACCOUNT_CS_SVRENV *pstEnv, ConnCtx *pstConnCtx, 
	                     SSPKG *pstSSPkg);

static int ss_sendto_auth_pkg(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv, 
	                          SSPKG *pstSSPkg);

// 查询点券的请求
static int cs_query_bill_point(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
                                   BILLRECORD *pstQeuryBillRecordReq);

int ss_sendto_region_pkg(TAPPCTX* pstAppCtx, ACCOUNT_CS_SVRENV* pstEnv, 
					     int iCmd, char *szData, int iLen);


static inline void _ss_init_head(SSPKGHEAD *pstHead, int iCmd)
{
	pstHead->Cmd = iCmd;
	pstHead->HeadLen = sizeof(SSPKGHEAD);
	pstHead->Magic = PROTOCOL_MAGIC;
	pstHead->Ver = TDR_METALIB_PROTO_SS_VERSION;
}

static void db_init_torm_head(TAPPCTX* pstAppCtx, TORMPKGHEAD *pstHead,
                              int iCmd, char *pszMetaName, int iObID)
{
	pstHead->HeadComm.InstID = pstAppCtx->iId;
	pstHead->HeadComm.Magic = TORM_MAGIC;
	pstHead->HeadCmd.Cmd = iCmd;

	STRNCPY(pstHead->HeadCmd.MetaNameVer.MetaName, pszMetaName, 
		      sizeof(pstHead->HeadCmd.MetaNameVer.MetaName));
	pstHead->HeadCmd.MetaNameVer.MetaVer = TDR_METALIB_ROLE_DB_META_VERSION;
	pstHead->HeadApp.AppCmd = 0;
	pstHead->HeadApp.Seq = 0;
	pstHead->HeadApp.ObjID = iObID;
	pstHead->AsynCallBackDataLen = 0;
}

static void db_set_callback_ctx(TORMPKGHEAD *pstHead, int iCmd, int iRealCmd,
                                const TFRAMEHEAD *pstFH, const void *pvData,
                                int iDataLen)
{
	TormCallbackCtx *pstCtx = (TormCallbackCtx *)pstHead->AsynCallBackData;
	pstHead->AsynCallBackDataLen = 0;

	pstCtx->iCmd = iCmd;
	pstHead->AsynCallBackDataLen += sizeof(pstCtx->iCmd);

	pstCtx->stConnInfo.iConnIdx = pstFH->ConnIdx;
	pstCtx->stConnInfo.stIPInfo = pstFH->ExtraInfo.IPInfo;
	pstHead->AsynCallBackDataLen += sizeof(pstCtx->stConnInfo);

	pstCtx->iRealCmd = iRealCmd;
	pstHead->AsynCallBackDataLen += sizeof(pstCtx->iRealCmd);

	if (pvData && iDataLen)
	{
		memcpy(&pstCtx->stData, pvData, iDataLen);
		pstHead->AsynCallBackDataLen += iDataLen;
	}
}


static int db_torm_send(TAPPCTX* pstAppCtx, ACCOUNT_CS_SVRENV* pstEnv, 
	                    TORMPKG *pstPkg, int iDstID)
{
	TDRDATA stData;
	int iSrc;
	int iDst;

	stData.pszBuff = pstEnv->szSendBuffer;
	stData.iBuff = sizeof(pstEnv->szSendBuffer);
	if (torm_pack_msg(pstEnv->hTorm, &stData, pstPkg, 0) != 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_pack_msg error");
		return -1;
	}
	
	iSrc = pstAppCtx->iId;
	iDst = iDstID;
	if (tbus_send(pstAppCtx->iBus, &iSrc, &iDst, stData.pszBuff,
	              stData.iBuff, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail");
		return -1;
	}

	return 0;
}

static int db_send_req(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv, 
	                   const void *pvData, int iDataLen)
{
	int iRet = 0;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pvData;
	stMetaData.iBuff = iDataLen;

	iRet = torm_make_req_msg(pstEnv->hTorm, &stNetData, &pstEnv->stTormPkg, 
	                         &stMetaData, 0);
	if (iRet != 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail. reason : %s\n",
		           torm_error_string(iRet));
		return -1;
	}

	if (db_torm_send(pstAppCtx, pstEnv, &pstEnv->stTormPkg,
	                 pstEnv->iAccountDBID) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "db_torm_send fail.");
		return -1;
	}
	return 0;
}


static int db_get_account_passpwd_res(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv, 
	                           						TORMPKG *pstPkg, TormCallbackCtx *pstCallbackCtx)
{
//	int iRet = 0;
	SSPKG *pstSSPkg = &pstEnv->stSSPkg;
	GETACCSAFELOCKRES *pstRes = &pstSSPkg->Body.CsCtrL.CsCmd.GetAccSafeLockRes;
	const TORMSELECTRES *pstDBRes = &pstPkg->Body.SelectRes;
	UNUSED(pstAppCtx);
	bzero(pstRes, sizeof(*pstRes));

	if (TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			       "Select account info from db failed. dberr = %d, msg = %s.", 
			       pstDBRes->DBMSErrNo, torm_error_string(pstDBRes->ErrNo));
		return -1;
	}

	if(pstDBRes->SelectResult.ResultNum !=1)
		return -1;
	
	ACCOUNT stAccInfo;
	if (torm_fetch_one_meta(pstEnv->hTorm, (char *)&stAccInfo, 
		                    			sizeof(stAccInfo)) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"torm_fetch_one_meta fail");
		return -1;
	}

	pstRes->SafeLockPassWd[0] = 0;
	if (stAccInfo.SafePwd[0] != 0)
	{
		STRNCPY(pstRes->SafeLockPassWd, stAccInfo.SafePwd, sizeof(pstRes->SafeLockPassWd));
	}
	STRNCPY(pstRes->AccName, stAccInfo.Pass9Account, sizeof(pstRes->AccName));

	_ss_init_head(&pstEnv->stSSPkg.Head, CS_CTRL);
	pstEnv->stSSPkg.Body.CsCtrL.Cmd = GET_ACC_SAFELOCK_RES;
	return conn_send_pkg(pstEnv, &pstCallbackCtx->stConnInfo, &pstEnv->stSSPkg);
}

static int db_up_acc_passwd_req(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv, 
	                           					TORMPKG *pstPkg, TormCallbackCtx *pstCallbackCtx)
{
	ACCOUNT stAccInfo;
//	SSPKG *pstSSPkg = &pstEnv->stSSPkg;
	const TORMSELECTRES *pstDBRes = &pstPkg->Body.SelectRes;
	UNUSED(pstAppCtx);
	
	if (TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			       "Select account info from db failed. dberr = %d, msg = %s.", 
			       pstDBRes->DBMSErrNo,
			       torm_error_string(pstDBRes->ErrNo));
		return -1;
	}

	if (pstDBRes->SelectResult.ResultNum != 1)
	{
		return -1;
	}

	if (torm_fetch_one_meta(pstEnv->hTorm, (char *)&stAccInfo,
	                        			sizeof(stAccInfo)) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"torm_fetch_one_meta fail");
		return -1;
	}

	STRNCPY(stAccInfo.SafePwd, pstCallbackCtx->stData.stSetAccSafeLockReq.SafeLockPassWd, 
			sizeof(stAccInfo.SafePwd));

	pstEnv->stIncomingFrameHead.ConnIdx =
		pstCallbackCtx->stConnInfo.iConnIdx;
	pstEnv->stIncomingFrameHead.ExtraInfo.IPInfo =
		pstCallbackCtx->stConnInfo.stIPInfo;

	
	{
		TORMUPDATEREQ *pstDBReq = &pstEnv->stTormPkg.Body.UpdateReq;
		ACCSAFEPWD stPwd;

		db_init_torm_head(pstAppCtx, &pstEnv->stTormPkg.Head, TORM_UPDATE_REQ,"AccSafePwd", 0);
		db_set_callback_ctx(
			&pstEnv->stTormPkg.Head,
			DB_RES_SET_PASSPWD,
			0,
			&pstEnv->stIncomingFrameHead,
			stAccInfo.Pass9Account,
			sizeof(stAccInfo.Pass9Account));

		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDBReq->OperInfo.WhereDef[0] = 0;
		pstDBReq->DoInsert = 0;
		
		stPwd.Uin = stAccInfo.Uin;
		STRNCPY(stPwd.SafePwd, pstCallbackCtx->stData.stSetAccSafeLockReq.SafeLockPassWd, sizeof(stPwd.SafePwd));
		
		return db_send_req(pstAppCtx, pstEnv, &stPwd, sizeof(stPwd));
	}
}

// Login Name -> Uin
static int db_l2u_res(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv, 
	                  TORMPKG *pstPkg, TormCallbackCtx *pstCallbackCtx)
{
	switch(pstCallbackCtx->iRealCmd)
	{
	case DB_RES_REAL:
		return db_account_info_res(pstAppCtx, pstEnv, pstPkg, pstCallbackCtx);

	case DB_RES_ACCOUNT_FREEZE:
		return db_will_account_freeze_req(pstAppCtx, pstEnv, pstPkg,
		                                  pstCallbackCtx);

	case DB_RES_ACCOUNT_SILENCE:
		return db_will_account_silence_req(pstAppCtx, pstEnv, pstPkg,
		                                   pstCallbackCtx);

	case DB_RES_ACCOUNT_UIN_FLAG:
		return db_will_account_uin_flag_req(pstAppCtx, pstEnv, pstPkg,
		                                    pstCallbackCtx);
	case DB_RES_SELECT_ACCOUNT_PASSPWD:
		return db_get_account_passpwd_res(pstAppCtx, pstEnv, pstPkg, pstCallbackCtx);
		break;
	case DB_RES_SET_PASSPWD:
		return  db_up_acc_passwd_req(pstAppCtx, pstEnv, pstPkg, pstCallbackCtx);
		break;

	default:
		return -1;
	}
	return 0;
}

static int db_account_info_res(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv, 
	                           TORMPKG *pstPkg, TormCallbackCtx *pstCallbackCtx)
{
	int iRet = 0;
	SSPKG *pstSSPkg = &pstEnv->stSSPkg;
	ACCGETINFORES *pstRes = &pstSSPkg->Body.CsCtrL.CsCmd.GetAccInfoRes;
	const TORMSELECTRES *pstDBRes = &pstPkg->Body.SelectRes;

	UNUSED(pstAppCtx);

	bzero(pstRes, sizeof(*pstRes));

	if (TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			       "Select account info from db failed. dberr = %d, msg = %s.", 
			       pstDBRes->DBMSErrNo, torm_error_string(pstDBRes->ErrNo));
		goto error;
	}

	if(pstDBRes->SelectResult.ResultNum > 0)
	{
		ACCOUNT stAccInfo;
		
		if (torm_fetch_one_meta(pstEnv->hTorm, (char *)&stAccInfo, 
			                    sizeof(stAccInfo)) < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,	"torm_fetch_one_meta fail");
			goto error;
		}

		pstRes->Found = 1;
		pstRes->AccountInfo.Uin = stAccInfo.Uin;
		pstRes->AccountInfo.UinFlag = stAccInfo.UinFlag;
		pstRes->AccountInfo.SilenceEndTime = stAccInfo.SilenceEndTime;
		pstRes->AccountInfo.FreezeStartTime = stAccInfo.FreezeStartTime;
		pstRes->AccountInfo.FreezeEndTime = stAccInfo.FreezeEndTime;
		pstRes->AccountInfo.Gender = stAccInfo.Gender;
		pstRes->AccountInfo.LastLoginIP = stAccInfo.LastLoginIP;
		pstRes->AccountInfo.LastLoginTime = stAccInfo.LastLoginTime;
		pstRes->AccountInfo.AccCreateTime = stAccInfo.AccountCreateTime;

		// jerry modify   start
		pstRes->AccountInfo.SilenceStartTime = stAccInfo.SilenceStartTime;
		// end

		STRNCPY(pstRes->AccountInfo.FreezeReason, stAccInfo.FreezeReason,
		        CCH(pstRes->AccountInfo.FreezeReason));
		STRNCPY(pstRes->AccountInfo.RegisterTs, stAccInfo.RegisterTs,
		        CCH(pstRes->AccountInfo.RegisterTs));
		STRNCPY(pstRes->AccountInfo.Pass9Account, stAccInfo.Pass9Account,
			    CCH(pstRes->AccountInfo.Pass9Account));
		STRNCPY(pstRes->AccountInfo.BirthDay, stAccInfo.BirthDay,
			    CCH(pstRes->AccountInfo.BirthDay));
		pstRes->AccountInfo.LastLoginWorldID = stAccInfo.LastLoginWorldID;
	}

	_ss_init_head(&pstEnv->stSSPkg.Head, CS_CTRL);
	pstEnv->stSSPkg.Body.CsCtrL.Cmd = GET_ACCOUNT_INFO_RES;
	iRet = conn_send_pkg(pstEnv, &pstCallbackCtx->stConnInfo, &pstEnv->stSSPkg);
final:
	return iRet;

error:
	iRet = -1;
	pstRes->Found = 0;
	goto final;
	return 0;
}

/*
static int db_account_info_id_res(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv, 
	                                TORMPKG *pstPkg, 
	                                TormCallbackCtx *pstCallbackCtx)
{
	int i = 0, iRet = 0;
	SSPKG *pstSSPkg = &pstEnv->stSSPkg;
	ACCGETIDRES *pstRes = &pstSSPkg->Body.CsCtrL.CsCmd.GetAccIDRes;
	const TORMSELECTRES *pstDBRes = &pstPkg->Body.SelectRes;

	UNUSED(pstAppCtx);

	bzero(pstRes, sizeof(*pstRes));

	if (TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			         "Select account info from db failed. dberr = %d, msg = %s.", 
			         pstDBRes->DBMSErrNo,
			         torm_error_string(pstDBRes->ErrNo));
		goto error;
	}

	i = pstDBRes->SelectResult.ResultNum;
	while(i--)
	{
		ACCOUNT stAccInfo;
		
		if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stAccInfo, 
			                     sizeof(stAccInfo)) < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,	"torm_fetch_one_meta fail");
			goto error;
		}

		STRNCPY(pstRes->AccountIDs[pstRes->ResNum].LoginName,
		        stAccInfo.Pass9Account, CCH(stAccInfo.Pass9Account));
		pstRes->ResNum++;
	}

final:
	_ss_init_head(&pstEnv->stSSPkg.Head, CS_CTRL);
	pstEnv->stSSPkg.Body.CsCtrL.Cmd = GET_ACCOUNT_ID_RES;
	iRet = conn_send_pkg(pstEnv, &pstCallbackCtx->stConnInfo, &pstEnv->stSSPkg);
	return iRet;

error:
	iRet = -1;
	pstRes->ResNum = 0;
	goto final;
	return 0;
}
*/

static int db_account_rolelist_res(TAPPCTX *pstAppCtx,
                                   ACCOUNT_CS_SVRENV *pstEnv, TORMPKG *pstPkg, 
	                               TormCallbackCtx *pstCallbackCtx)
{
	int i = 0, iRet = 0;
	SSPKG *pstSSPkg = &pstEnv->stSSPkg;
	ACCFINDROLELISTRES *pstRes =
		&pstSSPkg->Body.CsCtrL.CsCmd.FindAccountRoleListRes;
	ACCOUNTROLE stAccRole;
	const TORMSELECTRES *pstDBRes = &pstPkg->Body.SelectRes;

	UNUSED(pstAppCtx);

	if (TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			       "Select account info from db failed. dberr = %d, msg = %s.", 
			       pstDBRes->DBMSErrNo, torm_error_string(pstDBRes->ErrNo));
		goto error;
	}

	bzero(pstRes, sizeof(*pstRes));
	i = pstDBRes->SelectResult.ResultNum;
	while(i--)
	{
		SSACCOUNTROLE *pstCurr = &pstRes->RoleArray[pstRes->ResNum++];
		
		if (torm_fetch_one_meta(pstEnv->hTorm, (char *)&stAccRole,
		                        sizeof(stAccRole)) < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,	"torm_fetch_one_meta fail");
			goto error;
		}

		pstCurr->RoleWID = stAccRole.RoleWID;
		pstCurr->Uin = stAccRole.Uin;
		pstCurr->RegionID = stAccRole.RegionID;
		pstCurr->ServerID = stAccRole.ServerID;
		pstCurr->DelTime = stAccRole.DelTime;
		pstCurr->CreateTime = stAccRole.CreateTime;
		pstCurr->OnlineFlag = stAccRole.OnlineFlag;
		STRNCPY(pstCurr->MacAddr,stAccRole.MacAddr,sizeof(pstCurr->MacAddr));
		
		STRNCPY(pstCurr->Pass9Account, stAccRole.Pass9Account,
		        sizeof(pstCurr->Pass9Account));
		STRNCPY(pstCurr->RoleName, stAccRole.RoleName, sizeof(pstCurr->RoleName));
	}

final:
	_ss_init_head(&pstEnv->stSSPkg.Head, CS_CTRL);
	pstEnv->stSSPkg.Body.CsCtrL.Cmd = FIND_ACCOUNT_ROLELIST_RES;
	iRet = conn_send_pkg(pstEnv, &pstCallbackCtx->stConnInfo, &pstEnv->stSSPkg);
	return iRet;

error:
	iRet = -1;
	pstRes->ResNum = 0;
	goto final;
	return 0;
}


static int db_account_role_update_deltime_res(TAPPCTX *pstAppCtx, 
	                                          ACCOUNT_CS_SVRENV *pstEnv, 
	                                          TORMPKG *pstPkg, 
	                                          TormCallbackCtx *pstCallbackCtx)
{
	SSPKG *pstSSPkg = &pstEnv->stSSPkg;
	SSRESTOREROLERES *pstRes = &pstSSPkg->Body.CsCtrL.CsCmd.RestoreRoleRes;
	TORMUPDATERES *pstDBRes = &pstPkg->Body.UpdateRes;
	
	UNUSED(pstAppCtx);

	if (TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			       "Update account info from db failed. dberr = %d, msg = %s.", 
			       pstDBRes->DBMSErrNo, torm_error_string(pstDBRes->ErrNo));
		pstRes->Succeeded = 0;
	}
	else
	{
		pstRes->Succeeded = 1;
	}
	
	_ss_init_head(&pstEnv->stSSPkg.Head, CS_CTRL);
	pstEnv->stSSPkg.Body.CsCtrL.Cmd = RESTORE_ROLE_RES;
	conn_send_pkg(pstEnv, &pstCallbackCtx->stConnInfo, &pstEnv->stSSPkg);
	return !pstRes->Succeeded ? -1 : 0;
}

static int db_account_silence_res(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv, 
	                              TORMPKG *pstPkg, 
	                              TormCallbackCtx *pstCallbackCtx)
{
	SSPKG *pstSSPkg = &pstEnv->stSSPkg;
	ACCSETSILENCERES *pstRes = &pstSSPkg->Body.CsCtrL.CsCmd.SetAccSilenceRes;
	TORMUPDATERES *pstDBRes = &pstPkg->Body.UpdateRes;
	
	UNUSED(pstAppCtx);

	if (TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			     "Update account silence from db failed. dberr = %d, msg = %s.", 
			       pstDBRes->DBMSErrNo,
			       torm_error_string(pstDBRes->ErrNo));
		pstRes->Succeeded = 0;
	}
	else
	{
		pstRes->Succeeded = 1;
	}
	
	_ss_init_head(&pstEnv->stSSPkg.Head, CS_CTRL);
	pstEnv->stSSPkg.Body.CsCtrL.Cmd = SET_ACCOUNT_SILENCE_RES;
	conn_send_pkg(pstEnv, &pstCallbackCtx->stConnInfo, &pstEnv->stSSPkg);

	// ACCOUNT数据变化了,他妈的通知一下auth_svr
	_ss_init_head(&pstEnv->stSSPkg.Head, UPDATE_ACCOUNT_REP);
	STRNCPY(pstEnv->stSSPkg.Body.UpdateAccountRep.LoginName,
		    pstCallbackCtx->stData.szLoginName,
		    sizeof(pstEnv->stSSPkg.Body.UpdateAccountRep.LoginName));
	ss_sendto_auth_pkg(pstAppCtx, pstEnv, &pstEnv->stSSPkg);
	return !pstRes->Succeeded ? -1 : 0;
}

static int db_account_freeze_res(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv, 
	                             TORMPKG *pstPkg,
	                             TormCallbackCtx *pstCallbackCtx)
{
	SSPKG *pstSSPkg = &pstEnv->stSSPkg;
	ACCSETFREEZERES *pstRes = &pstSSPkg->Body.CsCtrL.CsCmd.SetAccFreezeRes;
	TORMUPDATERES *pstDBRes = &pstPkg->Body.UpdateRes;
	
	UNUSED(pstAppCtx);

	if (TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			      "Update account freeze from db failed. dberr = %d, msg = %s.", 
			       pstDBRes->DBMSErrNo, torm_error_string(pstDBRes->ErrNo));
		pstRes->Succeeded = 0;
	}
	else
	{
		pstRes->Succeeded = 1;
	}

	_ss_init_head(&pstEnv->stSSPkg.Head, CS_CTRL);
	pstEnv->stSSPkg.Body.CsCtrL.Cmd = SET_ACCOUNT_FREEZE_RES;
	conn_send_pkg(pstEnv, &pstCallbackCtx->stConnInfo, &pstEnv->stSSPkg);

	// ACCOUNT数据变化了,他妈的通知一下auth_svr
	_ss_init_head(&pstEnv->stSSPkg.Head, UPDATE_ACCOUNT_REP);
	STRNCPY(pstEnv->stSSPkg.Body.UpdateAccountRep.LoginName,
		    pstCallbackCtx->stData.szLoginName,
		    sizeof(pstEnv->stSSPkg.Body.UpdateAccountRep.LoginName));
	ss_sendto_auth_pkg(pstAppCtx, pstEnv, &pstEnv->stSSPkg);
	return !pstRes->Succeeded ? -1 : 0;
}

static int db_set_acc_passwd_res(TAPPCTX *pstAppCtx,ACCOUNT_CS_SVRENV *pstEnv, 
	                               				TORMPKG *pstPkg,TormCallbackCtx *pstCallbackCtx)
{
	SSPKG *pstSSPkg = &pstEnv->stSSPkg;
	SETACCSAFELOCKRES *pstRes = &pstSSPkg->Body.CsCtrL.CsCmd.SetAccSafeLockRes;
	TORMUPDATERES *pstDBRes = &pstPkg->Body.UpdateRes;
	UNUSED(pstAppCtx);

	if (TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			      "Update account freeze from db failed. dberr = %d, msg = %s.", 
			       pstDBRes->DBMSErrNo, torm_error_string(pstDBRes->ErrNo));
		return -1;
	}

	STRNCPY(pstRes->AccName, pstCallbackCtx->stData.szLoginName, sizeof(pstRes->AccName));
	_ss_init_head(&pstEnv->stSSPkg.Head, CS_CTRL);
	pstEnv->stSSPkg.Body.CsCtrL.Cmd = SET_ACC_SAFELOCK_RES;
	conn_send_pkg(pstEnv, &pstCallbackCtx->stConnInfo, &pstEnv->stSSPkg);
	return 0;
}

static int db_account_uin_flag_res_in(TAPPCTX *pstAppCtx,
                                   ACCOUNT_CS_SVRENV *pstEnv, 
	                               int iSucceeded,
	                               TormCallbackCtx *pstCallbackCtx)
{
	SSPKG *pstSSPkg = &pstEnv->stSSPkg;
	ACCSETUINFLAGRES *pstRes = &pstSSPkg->Body.CsCtrL.CsCmd.SetAccountUinFlagRes;

	UNUSED(pstAppCtx);


	pstRes->Succeeded = iSucceeded;

	_ss_init_head(&pstEnv->stSSPkg.Head, CS_CTRL);
	pstEnv->stSSPkg.Body.CsCtrL.Cmd = SET_ACCOUNT_UIN_FLAG_RES;
	conn_send_pkg(pstEnv, &pstCallbackCtx->stConnInfo, &pstEnv->stSSPkg);

	// 

	// ACCOUNT数据变化了,他妈的通知一下auth_svr
	_ss_init_head(&pstEnv->stSSPkg.Head, UPDATE_ACCOUNT_REP);
	STRNCPY(pstEnv->stSSPkg.Body.UpdateAccountRep.LoginName,
		    pstCallbackCtx->stData.szLoginName,
		    sizeof(pstEnv->stSSPkg.Body.UpdateAccountRep.LoginName));
	ss_sendto_auth_pkg(pstAppCtx, pstEnv, &pstEnv->stSSPkg);

	//通知各个zone
	{
		SSCMDREQ* pstCmdReq = &pstEnv->stSSPkg.Body.CmdReq;
		
		//_ss_init_head(&pstEnv->stSSPkg.Head, SS_CMD_REQ);

		
		memset(pstCmdReq,0,sizeof(*pstCmdReq));
		pstCmdReq->Cmd = SS_CMD_UIN_FLAG_CHANGE;
	
		STRNCPY(pstCmdReq->Data.UinFlagChange.AccName, pstCallbackCtx->stData.szLoginName,
			sizeof(pstCmdReq->Data.UinFlagChange.AccName)) ;
		
		pstCmdReq->Data.UinFlagChange.Type = pstCallbackCtx->stData.stAccSetUinFlagReq.Type;
		pstCmdReq->Data.UinFlagChange.UinFlag = pstCallbackCtx->stData.stAccSetUinFlagReq.UinFlag;
		pstCmdReq->Data.UinFlagChange.Uin = 0;
		
		ss_sendto_region_pkg(pstAppCtx, pstEnv,SS_CMD_REQ, (char *)pstCmdReq,sizeof(*pstCmdReq));
	}
	
	return !pstRes->Succeeded ? -1 : 0;
}


static int db_account_uin_flag_res(TAPPCTX *pstAppCtx,
                                   ACCOUNT_CS_SVRENV *pstEnv, 
	                               TORMPKG *pstPkg,
	                               TormCallbackCtx *pstCallbackCtx)
{
	TORMUPDATERES *pstDBRes = &pstPkg->Body.UpdateRes;
	
	UNUSED(pstAppCtx);

	if (TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			      "Update account freeze from db failed. dberr = %d, msg = %s.", 
			       pstDBRes->DBMSErrNo, torm_error_string(pstDBRes->ErrNo));
		return db_account_uin_flag_res_in(pstAppCtx,pstEnv,0, pstCallbackCtx);
	}
	else
	{
		return db_account_uin_flag_res_in(pstAppCtx,pstEnv,1, pstCallbackCtx);
	}


}

static int db_account_role_update_name_res(TAPPCTX *pstAppCtx,
	                                       ACCOUNT_CS_SVRENV *pstEnv, 
	                                       TORMPKG *pstPkg,
	                                       TormCallbackCtx *pstCallbackCtx)
{
	SSPKG *pstSSPkg = &pstEnv->stSSPkg;
	TORMUPDATERES *pstDBRes = &pstPkg->Body.UpdateRes;
	ACCSETROLENAMERES *pstRes = &pstSSPkg->Body.CsCtrL.CsCmd.SetAccRoleNameRes;
	
	UNUSED(pstAppCtx);

	if (TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			    "Update account rolename from db failed. dberr = %d, msg = %s.", 
			       pstDBRes->DBMSErrNo, torm_error_string(pstDBRes->ErrNo));
		pstRes->Succeeded = 0;
	}
	else
	{
		pstRes->Succeeded = 1;
	}

	_ss_init_head(&pstEnv->stSSPkg.Head, CS_CTRL);
	pstEnv->stSSPkg.Body.CsCtrL.Cmd = SET_ACCOUNT_ROLENAME_RES;
	conn_send_pkg(pstEnv, &pstCallbackCtx->stConnInfo, &pstEnv->stSSPkg);
	return !pstRes->Succeeded ? -1 : 0;
}

static int conn_send_pkg(ACCOUNT_CS_SVRENV *pstEnv, ConnCtx *pstConnCtx, 
	                       SSPKG *pstSSPkg)
{
	TFRAMEHEAD *pstFH = &pstEnv->stOutcomingFrameHead;
	TDRDATA stNet;
	
	bzero(pstFH, sizeof(*pstFH));
	pstFH->Cmd = TFRAMEHEAD_CMD_STOP;
	pstFH->TimeStampType = 0;
	pstFH->ID = -1;
	pstFH->ConnIdx = pstConnCtx->iConnIdx;
	memcpy(&pstFH->ExtraInfo.IPInfo, &pstConnCtx->stIPInfo,
	       sizeof(pstFH->ExtraInfo.IPInfo));
	pstFH->ExtraType = TFRAMEHEAD_EXTRA_IP;

	stNet.pszBuff = pstEnv->szSendBuffer;
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	if (ss_pack(pstEnv->pstSSHan, &stNet, pstSSPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -1;
	}
	
	if (tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iAccountCSConnId, 
		              stNet.pszBuff, stNet.iBuff, pstFH) != 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tconnapi_send fail");
		return -1;
	}
	return 0;
}

static int ss_sendto_auth_pkg(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv, 
	                            SSPKG *pstSSPkg)
{
	int iSrc, iDst;
	TDRDATA stNet;

	stNet.pszBuff = pstEnv->szSendBuffer;
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	if (ss_pack(pstEnv->pstSSHan, &stNet, pstSSPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -1;
	}

	iSrc = pstAppCtx->iId;
	iDst = pstEnv->iAuthSvrID;
	if (tbus_forward(pstAppCtx->iBus, &iSrc, &iDst, stNet.pszBuff, stNet.iBuff,
	                 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"tbus_forward auth_svr fail.");
	}
	else
	{
		goto out;
	}
	
	/*iSrc = pstAppCtx->iId;
	iDst = pstEnv->iAuthSlaveID;
	if(tbus_forward(pstAppCtx->iBus, &iSrc, &iDst, stNet.pszBuff, stNet.iBuff, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"tbus_forward auth_slave_svr fail.");
	}
	else
	{
		goto out;
	}
	
	iSrc = pstAppCtx->iId;
	iDst = pstEnv->iAuthBakID;
	if(tbus_forward(pstAppCtx->iBus, &iSrc, &iDst, stNet.pszBuff, stNet.iBuff, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"tbus_forward auth_bak_svr fail.");
	}
	else
	{
		goto out;
	}*/
	
out:
	return 0;
}

int ss_sendto_region_pkg(TAPPCTX* pstAppCtx, ACCOUNT_CS_SVRENV* pstEnv, 
					     int iCmd, char *szData, int iLen)
{
	SSPKG stPkg;
	TDRDATA stNet;
	int iSrc, iDst;

	memcpy(&stPkg.Body,szData,iLen);

	stPkg.Head.Cmd = iCmd;
	stPkg.Head.Magic = PROTOCOL_MAGIC; 
	stPkg.Head.Ver = TDR_METALIB_PROTO_SS_VERSION;
	stPkg.Head.HeadLen = sizeof(stPkg.Head);

	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = pstEnv->szSendBuffer;
	if (ss_pack(pstEnv->pstSSHan, &stNet, &stPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"ss_pack fail.");
		return -1;
	}

	iSrc = pstAppCtx->iId;
	iDst = pstEnv->iRegionSvrID;
	//iDst = pstEnv->iAuthSvrID;
	tlog_info(pstEnv->pstLogCat, 0, 0, "region_svr [%s]", tbus_addr_ntoa(iDst));
	
	if (tbus_forward(pstAppCtx->iBus, &iSrc, &iDst, stNet.pszBuff, stNet.iBuff,
	                 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"tbus_forward fail.");
		return -1;
	}
	return 0;
}


static int db_get_account_info_req(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
                                   const char *pszLoginName, int iRealCmd,
                                   const void *pvData, int iDataLen)
{
	TORMSELECTREQ *pstDBReq = &pstEnv->stTormPkg.Body.SelectReq;
	ACCOUNT stAccInfo;
	
	db_init_torm_head(pstAppCtx, &pstEnv->stTormPkg.Head, TORM_SELECT_REQ, "Account", 0);
	db_set_callback_ctx(
		&pstEnv->stTormPkg.Head,
		DB_RES_ACCOUNT_INFO,
		iRealCmd,
		&pstEnv->stIncomingFrameHead,
		pvData,
		iDataLen);

	pstDBReq->OffSet = 0;
	pstDBReq->Limit = 0;
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	snprintf(pstDBReq->OperInfo.WhereDef, CCH(pstDBReq->OperInfo.WhereDef),
	         "WHERE Pass9Account=\'%s\'", pszLoginName);

	bzero(&stAccInfo, sizeof(stAccInfo));
	STRNCPY(stAccInfo.Pass9Account, pszLoginName, CCH(stAccInfo.Pass9Account));

	return db_send_req(pstAppCtx, pstEnv, &stAccInfo, sizeof(stAccInfo));
}

// 从Login Name精确的查找帐号信息
static int ss_get_account_info(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
                               const ACCGETINFOREQ *pstReq)
{
	return db_get_account_info_req(
		pstAppCtx,
		pstEnv,
		pstReq->AccIDInfo.LoginName,
		0,
		NULL,
		0);
}

static int db_restore_role_req(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
                               const SSRESTOREROLEREQ *pstReq)
{
	TORMUPDATEREQ *pstDBReq = &pstEnv->stTormPkg.Body.UpdateReq;
	ACCOUNTROLEDELTIME stAccRoleDelTime;

	db_init_torm_head(pstAppCtx, &pstEnv->stTormPkg.Head, TORM_UPDATE_REQ,
		              "AccountRoleDelTime", 0);
	db_set_callback_ctx(
		&pstEnv->stTormPkg.Head, 
	    DB_RES_ACCOUNT_ROLE_UPDATE_DELTIME,
	    0,
	    &pstEnv->stIncomingFrameHead,
	    NULL,
	    0);

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.WhereDef[0] = 0;
	pstDBReq->DoInsert = 0;

	stAccRoleDelTime.RoleWID = pstReq->RoleID;
	stAccRoleDelTime.DelTime = 0;
	return db_send_req(pstAppCtx, pstEnv, &stAccRoleDelTime, 
		               sizeof(stAccRoleDelTime));
}

// 恢复被删除的角色
static int ss_restore_role(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
                           const SSRESTOREROLEREQ *pstReq)
{
	return db_restore_role_req(pstAppCtx, pstEnv, pstReq);
}

static int db_set_account_freeze_req(TAPPCTX *pstAppCtx,
                                     ACCOUNT_CS_SVRENV *pstEnv,
			                         int iUin, const ACCSETFREEZEREQ *pstReq)
{
	TORMUPDATEREQ *pstDBReq = &pstEnv->stTormPkg.Body.UpdateReq;
	ACCOUNTFREEZEENDTIME stAccFreezeEndTime;

	db_init_torm_head(pstAppCtx, &pstEnv->stTormPkg.Head, TORM_UPDATE_REQ,
		              "AccountFreezeEndTime", 0);
	db_set_callback_ctx(
		&pstEnv->stTormPkg.Head,
		DB_RES_ACCOUNT_FREEZE,
		0,
		&pstEnv->stIncomingFrameHead,
		pstReq->AccIDInfo.LoginName,
		strlen(pstReq->AccIDInfo.LoginName) + 1);

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.WhereDef[0] = 0;
	pstDBReq->DoInsert = 0;
	
	stAccFreezeEndTime.Uin = iUin;
	stAccFreezeEndTime.FreezeStartTime = 0;
	STRNCPY(stAccFreezeEndTime.FreezeReason, pstReq->FreezeReason,
	       sizeof(stAccFreezeEndTime.FreezeReason));
	if (pstReq->Enable)
	{
		stAccFreezeEndTime.FreezeEndTime = ((pstReq->FreezeTime < 0) ?
			-1 : pstAppCtx->stCurr.tv_sec + pstReq->FreezeTime);
		stAccFreezeEndTime.FreezeStartTime = pstAppCtx->stCurr.tv_sec ;
	}
	else
	{
		stAccFreezeEndTime.FreezeEndTime = 0;
		stAccFreezeEndTime.FreezeStartTime = 0;
	}
	return db_send_req(pstAppCtx, pstEnv, &stAccFreezeEndTime, 
		               sizeof(stAccFreezeEndTime));
}

static int db_will_account_freeze_req(TAPPCTX *pstAppCtx,
                                      ACCOUNT_CS_SVRENV *pstEnv, 
	                                  TORMPKG *pstPkg,
	                                  TormCallbackCtx *pstCallbackCtx)
{
	int iRet = 0;
	ACCOUNT stAccInfo;
	SSPKG *pstSSPkg = &pstEnv->stSSPkg;
	ACCSETFREEZERES *pstRes = &pstSSPkg->Body.CsCtrL.CsCmd.SetAccFreezeRes;
	const TORMSELECTRES *pstDBRes = &pstPkg->Body.SelectRes;

	UNUSED(pstAppCtx);

	bzero(pstRes, sizeof(*pstRes));
	if (TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			       "Select account info from db failed. dberr = %d, msg = %s.", 
			       pstDBRes->DBMSErrNo,
			       torm_error_string(pstDBRes->ErrNo));
		goto error;
	}

	if (!pstDBRes->SelectResult.ResultNum)
	{
		goto error;
	}

	if (torm_fetch_one_meta(pstEnv->hTorm, (char *)&stAccInfo,
	                        sizeof(stAccInfo)) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"torm_fetch_one_meta fail");
		goto error;
	}

	pstEnv->stIncomingFrameHead.ConnIdx =
		pstCallbackCtx->stConnInfo.iConnIdx;
	pstEnv->stIncomingFrameHead.ExtraInfo.IPInfo =
		pstCallbackCtx->stConnInfo.stIPInfo;
	return db_set_account_freeze_req(pstAppCtx, pstEnv, stAccInfo.Uin,
	                                 &pstCallbackCtx->stData.stAccSetFreezeReq);

error:
	_ss_init_head(&pstEnv->stSSPkg.Head, CS_CTRL);
	pstEnv->stSSPkg.Body.CsCtrL.Cmd = SET_ACCOUNT_FREEZE_RES;
	pstRes->Succeeded = 0;
	iRet = conn_send_pkg(pstEnv, &pstCallbackCtx->stConnInfo, &pstEnv->stSSPkg);
	return 0;
}

// 设置冻结状态													
static int ss_set_account_freeze(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
			                     const ACCSETFREEZEREQ *pstReq)
{
	return db_get_account_info_req(
		pstAppCtx,
		pstEnv,
	    pstReq->AccIDInfo.LoginName,
	    DB_RES_ACCOUNT_FREEZE,
	    pstReq,
	    sizeof(*pstReq));
}

static int db_set_account_silence_req(TAPPCTX *pstAppCtx,
                                      ACCOUNT_CS_SVRENV *pstEnv,
			                          int iUin, const ACCSETSILENCEREQ *pstReq)
{
	TORMUPDATEREQ *pstDBReq = &pstEnv->stTormPkg.Body.UpdateReq;
	ACCOUNTSILENCEENDTIME stAccSilenceEndTime;

	db_init_torm_head(pstAppCtx, &pstEnv->stTormPkg.Head, TORM_UPDATE_REQ,
		              "AccountSilenceEndTime", 0);
	db_set_callback_ctx(
		&pstEnv->stTormPkg.Head,
		DB_RES_ACCOUNT_SILENCE,
		0,
		&pstEnv->stIncomingFrameHead,
		pstReq->AccIDInfo.LoginName,
		strlen(pstReq->AccIDInfo.LoginName) + 1);

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.WhereDef[0] = 0;
	pstDBReq->DoInsert = 0;

	stAccSilenceEndTime.Uin = iUin;
	if (pstReq->Enable)
	{
		stAccSilenceEndTime.SilenceEndTime = ((pstReq->SilenceTime < 0) ?
			-1 : pstAppCtx->stCurr.tv_sec + pstReq->SilenceTime);
		
		// jerry modify start
		stAccSilenceEndTime.SilenceStartTime = pstAppCtx->stCurr.tv_sec;
		// end
		
	}
	else
	{
		stAccSilenceEndTime.SilenceEndTime = 0;

		// jerry modify
		stAccSilenceEndTime.SilenceStartTime = 0;
		// end
	}
	return db_send_req(pstAppCtx, pstEnv, &stAccSilenceEndTime, 
		               sizeof(stAccSilenceEndTime));
}

static int db_will_account_silence_req(TAPPCTX *pstAppCtx,
                                       ACCOUNT_CS_SVRENV *pstEnv, 
	                                   TORMPKG *pstPkg,
	                                   TormCallbackCtx *pstCallbackCtx)
{
	int iRet = 0;
	ACCOUNT stAccInfo;
	SSPKG *pstSSPkg = &pstEnv->stSSPkg;
	ACCSETSILENCERES *pstRes = &pstSSPkg->Body.CsCtrL.CsCmd.SetAccSilenceRes;
	const TORMSELECTRES *pstDBRes = &pstPkg->Body.SelectRes;

	UNUSED(pstAppCtx);

	bzero(pstRes, sizeof(*pstRes));
	if (TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			       "Select account info from db failed. dberr = %d, msg = %s.", 
			       pstDBRes->DBMSErrNo,
			       torm_error_string(pstDBRes->ErrNo));
		goto error;
	}

	if (!pstDBRes->SelectResult.ResultNum)
	{
		goto error;
	}

	if (torm_fetch_one_meta(pstEnv->hTorm, (char *)&stAccInfo,
	                        sizeof(stAccInfo)) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"torm_fetch_one_meta fail");
		goto error;
	}

	pstEnv->stIncomingFrameHead.ConnIdx =
		pstCallbackCtx->stConnInfo.iConnIdx;
	pstEnv->stIncomingFrameHead.ExtraInfo.IPInfo =
		pstCallbackCtx->stConnInfo.stIPInfo;
	return db_set_account_silence_req(pstAppCtx, pstEnv, stAccInfo.Uin,
	                                  &pstCallbackCtx->stData.stAccSetSilenceReq);

error:
	_ss_init_head(&pstEnv->stSSPkg.Head, CS_CTRL);
	pstEnv->stSSPkg.Body.CsCtrL.Cmd = SET_ACCOUNT_SILENCE_RES;
	pstRes->Succeeded = 0;
	iRet = conn_send_pkg(pstEnv, &pstCallbackCtx->stConnInfo, &pstEnv->stSSPkg);
	return 0;
}

// 设置禁言状态
static int ss_set_account_silence(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
			                      const ACCSETSILENCEREQ *pstReq)
{
	return db_get_account_info_req(
		pstAppCtx,
		pstEnv,
	    pstReq->AccIDInfo.LoginName,
	    DB_RES_ACCOUNT_SILENCE,
	    pstReq,
	    sizeof(*pstReq));
}

// 设置AccountRoleName
static int ss_set_account_rolename(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
			                       const ACCSETROLENAMEREQ *pstReq)
{
	TORMUPDATEREQ *pstDBReq = &pstEnv->stTormPkg.Body.UpdateReq;
	ACCOUNTROLENAME stDummy;

	db_init_torm_head(pstAppCtx, &pstEnv->stTormPkg.Head, TORM_UPDATE_REQ,
		              "AccountRoleName", 0);
	db_set_callback_ctx(
		&pstEnv->stTormPkg.Head,
		DB_RES_ACCOUNT_ROLE_UPDATE_NAME,
		0,
	    &pstEnv->stIncomingFrameHead,
	    NULL,
	    0);

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.WhereDef[0] = 0;
	pstDBReq->DoInsert = 0;

	stDummy.RoleWID = pstReq->RoleID;
	STRNCPY(stDummy.RoleName, pstReq->RoleName, sizeof(stDummy.RoleName));
	return db_send_req(pstAppCtx, pstEnv, &stDummy, sizeof(stDummy));	
}

static int db_set_account_uin_flag_req(TAPPCTX *pstAppCtx,
                                       ACCOUNT_CS_SVRENV *pstEnv,
			                           int iUin, const ACCSETUINFLAGREQ *pstReq)
{
	TORMUPDATEREQ *pstDBReq = &pstEnv->stTormPkg.Body.UpdateReq;
	ACCOUNTUINFLAG stDummy;

	db_init_torm_head(pstAppCtx, &pstEnv->stTormPkg.Head, TORM_UPDATE_REQ,
		              "AccountUinFlag", 0);
	db_set_callback_ctx(
		&pstEnv->stTormPkg.Head,
		DB_RES_ACCOUNT_UIN_FLAG,
		0,
	    &pstEnv->stIncomingFrameHead,
		pstReq->AccIDInfo.LoginName,
		strlen(pstReq->AccIDInfo.LoginName) + 1);

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.WhereDef[0] = 0;
	pstDBReq->DoInsert = 0;

	stDummy.Uin = iUin;
	stDummy.UinFlag = pstReq->UinFlag;
	return db_send_req(pstAppCtx, pstEnv, &stDummy, sizeof(stDummy));
}

static int db_will_account_uin_flag_req(TAPPCTX *pstAppCtx,
                                        ACCOUNT_CS_SVRENV *pstEnv,
	                                    TORMPKG *pstPkg,
	                                    TormCallbackCtx *pstCallbackCtx)
{
	int iRet = 0;
	ACCOUNT stAccInfo;
	SSPKG *pstSSPkg = &pstEnv->stSSPkg;
	ACCSETUINFLAGRES *pstRes = &pstSSPkg->Body.CsCtrL.CsCmd.SetAccountUinFlagRes;
	const TORMSELECTRES *pstDBRes = &pstPkg->Body.SelectRes;

	UNUSED(pstAppCtx);

	bzero(pstRes, sizeof(*pstRes));
	if (TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			       "Select account info from db failed. dberr = %d, msg = %s.", 
			       pstDBRes->DBMSErrNo,
			       torm_error_string(pstDBRes->ErrNo));
		goto error;
	}

	if (!pstDBRes->SelectResult.ResultNum)
	{
		goto error;
	}

	if (torm_fetch_one_meta(pstEnv->hTorm, (char *)&stAccInfo,
	                        sizeof(stAccInfo)) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"torm_fetch_one_meta fail");
		goto error;
	}

	pstEnv->stIncomingFrameHead.ConnIdx =
		pstCallbackCtx->stConnInfo.iConnIdx;
	pstEnv->stIncomingFrameHead.ExtraInfo.IPInfo =
		pstCallbackCtx->stConnInfo.stIPInfo;
	return db_set_account_uin_flag_req(pstAppCtx, pstEnv, stAccInfo.Uin,
	                                &pstCallbackCtx->stData.stAccSetUinFlagReq);

error:
	_ss_init_head(&pstEnv->stSSPkg.Head, CS_CTRL);
	pstEnv->stSSPkg.Body.CsCtrL.Cmd = SET_ACCOUNT_UIN_FLAG_RES;
	pstRes->Succeeded = 0;
	iRet = conn_send_pkg(pstEnv, &pstCallbackCtx->stConnInfo, &pstEnv->stSSPkg);
	return 0;
}

// 设置Account 的UinFlag
static int ss_set_account_uin_flag(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
			                       const ACCSETUINFLAGREQ *pstReq)
{
	if (pstReq->Type == ACC_SET_UIN_FLAG_TYPE_NOTICE)
	{
		TORMPKGHEAD stHead;
		TormCallbackCtx stCallbackCtx;
		
		db_set_callback_ctx(
			&stHead,
			DB_RES_ACCOUNT_INFO,
			DB_RES_ACCOUNT_UIN_FLAG,
			&pstEnv->stIncomingFrameHead,
			pstReq,
			sizeof(*pstReq));		

		if (stHead.AsynCallBackDataLen > 0)
		{
			memcpy(&stCallbackCtx, stHead.AsynCallBackData, 
				   stHead.AsynCallBackDataLen);
		}
		
		db_account_uin_flag_res_in(pstAppCtx,pstEnv,1, &stCallbackCtx);
		return 0;
	}
	else
	{
		return db_get_account_info_req(
			pstAppCtx,
			pstEnv,
		    pstReq->AccIDInfo.LoginName,
		    DB_RES_ACCOUNT_UIN_FLAG,
		    pstReq,
		    sizeof(*pstReq));
	}
	return 0;
}

// 模糊查找帐号角色列表
static int ss_find_account_rolelist(TAPPCTX *pstAppCtx,
                                    ACCOUNT_CS_SVRENV *pstEnv,
                                    const ACCFINDROLELISTREQ *pstReq)
{
	TORMSELECTREQ *pstDBReq = &pstEnv->stTormPkg.Body.SelectReq;
	ACCOUNTROLE stAccRole;

	db_init_torm_head(pstAppCtx, &pstEnv->stTormPkg.Head, TORM_SELECT_REQ, 
		                "AccountRole", 0);
	db_set_callback_ctx(
		&pstEnv->stTormPkg.Head,
		DB_RES_ACCOUNT_ROLELIST,
		0,
		&pstEnv->stIncomingFrameHead,
		NULL,
		0);

	pstDBReq->OffSet = pstReq->Index * ROLE_MAX_RES;
	pstDBReq->Limit = ROLE_MAX_RES;
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;

	switch(pstReq->ByWhat)
	{
	case FIND_ACCOUNT_ROLELIST_BY_LOGINNAME:
		if (pstReq->ServerID < 0)
		{
			snprintf(pstDBReq->OperInfo.WhereDef,
				     sizeof(pstDBReq->OperInfo.WhereDef),
				     "WHERE Pass9Account LIKE \'%s\'",
				     pstReq->Key.LoginNamePattern);
		}
		else
		{
			snprintf(pstDBReq->OperInfo.WhereDef,
				     sizeof(pstDBReq->OperInfo.WhereDef),
				     "WHERE Pass9Account LIKE \'%s\' AND ServerID = %d",
				     pstReq->Key.LoginNamePattern,
				     pstReq->ServerID);
		}
		break;

	case FIND_ACCOUNT_ROLELIST_BY_ROLENAME:
		if (pstReq->ServerID < 0)
		{
			snprintf(pstDBReq->OperInfo.WhereDef,
				     sizeof(pstDBReq->OperInfo.WhereDef),
				     "WHERE RoleName LIKE \'%s\'",
				     pstReq->Key.RoleNamePattern);
		}
		else
		{
			snprintf(pstDBReq->OperInfo.WhereDef,
				     sizeof(pstDBReq->OperInfo.WhereDef),
				     "WHERE RoleName LIKE \'%s\' AND ServerID = %d",
				     pstReq->Key.RoleNamePattern,
				     pstReq->ServerID);
		}
		break;

	default:
		return -1;
		break;
	}

	bzero(&stAccRole, sizeof(stAccRole));
	return db_send_req(pstAppCtx, pstEnv, &stAccRole, sizeof(stAccRole));
}

static int db_find_account_req(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
                               int iIndex, int iByWhat, const char *pszPattern)
{
	TORMSELECTREQ *pstDBReq = &pstEnv->stTormPkg.Body.SelectReq;
	ACCOUNT stDummy;

	db_init_torm_head(pstAppCtx, &pstEnv->stTormPkg.Head, TORM_SELECT_REQ, 
		              "Account", 0);
	db_set_callback_ctx(
		&pstEnv->stTormPkg.Head,
		DB_RES_ACCOUNT_LITE,
		0,
		&pstEnv->stIncomingFrameHead,
		NULL,
		0);

	pstDBReq->OffSet = iIndex * ROLE_MAX_RES;
	pstDBReq->Limit = ACC_MAX_RES;
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;

	switch(iByWhat)
	{
	case FIND_ACCOUNT_BY_LOGINNAME:
		snprintf(pstDBReq->OperInfo.WhereDef,
		         sizeof(pstDBReq->OperInfo.WhereDef),
		         "WHERE Pass9Account LIKE \'%s\'", pszPattern);
		break;

	case FIND_ACCOUNT_BY_ROLENAME:
		snprintf(pstDBReq->OperInfo.WhereDef,
		         sizeof(pstDBReq->OperInfo.WhereDef),
	             "natural join "
	             "( SELECT Uin FROM AccountRole WHERE RoleName LIKE \'%s\' ) b",
	             pszPattern);
		break;

	default:
		return -1;
		break;
	}

	bzero(&stDummy, sizeof(stDummy));
	return db_send_req(pstAppCtx, pstEnv, &stDummy, sizeof(stDummy));
}

static int db_account_lite_res(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv, 
	                           TORMPKG *pstPkg, TormCallbackCtx *pstCallbackCtx)
{
	int i = 0, iRet = 0;
	SSPKG *pstSSPkg = &pstEnv->stSSPkg;
	ACCFINDRES *pstRes = &pstSSPkg->Body.CsCtrL.CsCmd.FindAccountRes;
	const TORMSELECTRES *pstDBRes = &pstPkg->Body.SelectRes;

	UNUSED(pstAppCtx);

	bzero(pstRes, sizeof(*pstRes));

	if (TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			         "Select account info from db failed. dberr = %d, msg = %s.", 
			         pstDBRes->DBMSErrNo,
			         torm_error_string(pstDBRes->ErrNo));
		goto error;
	}

	i = pstDBRes->SelectResult.ResultNum;
	while(i--)
	{
		ACCOUNT stStub;
		SSACCLITEINFO *pstCurr = &pstRes->AccountLiteArray[pstRes->ResNum++];
		
		if (torm_fetch_one_meta(pstEnv->hTorm, (char *)&stStub,
		                        sizeof(stStub)) < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,	"torm_fetch_one_meta fail");
			goto error;
		}

		pstCurr->FreezeEndTime = stStub.FreezeEndTime;
		pstCurr->FreezeStartTime = stStub.FreezeStartTime;
		pstCurr->LastLoginIP = stStub.LastLoginIP;
		pstCurr->LastLoginTime = stStub.LastLoginTime;
		pstCurr->SilenceEndTime = stStub.SilenceEndTime;
		pstCurr->AccCreateTime = stStub.AccountCreateTime;
		STRNCPY(pstCurr->Pass9Account, stStub.Pass9Account,
		        sizeof(pstCurr->Pass9Account));
		pstCurr->Uin = stStub.Uin;
		STRNCPY(pstCurr->FreezeReason,stStub.FreezeReason,sizeof(pstCurr->FreezeReason));
	}

final:
	_ss_init_head(&pstEnv->stSSPkg.Head, CS_CTRL);
	pstEnv->stSSPkg.Body.CsCtrL.Cmd = FIND_ACCOUNT_RES;
	iRet = conn_send_pkg(pstEnv, &pstCallbackCtx->stConnInfo, &pstEnv->stSSPkg);
	return iRet;

error:
	iRet = -1;
	pstRes->ResNum = 0;
	goto final;
	return 0;
}


// 模糊查找帐号信息
static int ss_find_account(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
                           const ACCFINDREQ *pstReq)
{
	switch(pstReq->ByWhat)
	{
	case FIND_ACCOUNT_BY_LOGINNAME:
		return db_find_account_req(pstAppCtx, pstEnv, pstReq->Index,
		                           pstReq->ByWhat,
		                           pstReq->Key.LoginNamePattern);
		break;

	case FIND_ACCOUNT_BY_ROLENAME:
		return db_find_account_req(pstAppCtx, pstEnv, pstReq->Index,
		                           pstReq->ByWhat,
		                           pstReq->Key.RoleNamePattern);
		break;

	default:
		return -1;
		break;
	}
	return 0;
}


// 消息转发到auth
int ss_ip_limit_msg_trans(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv, int iCmd)
{
	SSCSCTRL * pstCSCtrl = &pstEnv->stSSPkg.Body.CsCtrL;
	ASYNREQ stAsyn;

	stAsyn.Cmd = pstCSCtrl->Cmd;
	stAsyn.ConnIdx = pstEnv->stIncomingFrameHead.ConnIdx;
	stAsyn.IPInfo = pstEnv->stIncomingFrameHead.ExtraInfo.IPInfo;

	if (iCmd == IP_LIMIT_REQ)
	{
		stAsyn.DataLen = sizeof(pstCSCtrl->CsCmd.IPLimitReq);
		memcpy(stAsyn.Data,&pstCSCtrl->CsCmd.IPLimitReq,stAsyn.DataLen);
	}
	else if (iCmd == GET_IP_LIMIT_REQ)
	{
		stAsyn.DataLen = sizeof(pstCSCtrl->CsCmd.GetIPLimitReq);
		memcpy(stAsyn.Data,&pstCSCtrl->CsCmd.GetIPLimitReq,stAsyn.DataLen);
	}
	else
	{
		return -1;
	}
	
	

	pstCSCtrl->Cmd = ZONE_REQ;
	memcpy(&pstEnv->stSSPkg.Body.CsCtrL.CsCmd.ZoneReq, &stAsyn, sizeof(stAsyn));
	_ss_init_head(&pstEnv->stSSPkg.Head, CS_CTRL);

	if (ss_sendto_auth_pkg(pstAppCtx, pstEnv, &pstEnv->stSSPkg) < 0)
	{
		return -1;
	}
	return 0;
}
int ss_cash_props_gift_bill(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
		CASHPROPSGIFTINFOREQ *pCashPropsGiftInfoReq)
{
	//ASYNREQ stAsyn;
	SSCASHPROPSGIFTINFOREQ stCashPropsGiftInfoReq;

	memset(&stCashPropsGiftInfoReq,0,sizeof(stCashPropsGiftInfoReq));
	
	//stAsyn.Cmd = CS_CASH_PROPS_BILL_RES;
	stCashPropsGiftInfoReq.CsConnCtx.ConnIdx = pstEnv->stIncomingFrameHead.ConnIdx;
	stCashPropsGiftInfoReq.CsConnCtx.IPInfo = pstEnv->stIncomingFrameHead.ExtraInfo.IPInfo;

	STRNCPY(stCashPropsGiftInfoReq.Pass9Account,pCashPropsGiftInfoReq->Pass9Account,
			CCH(stCashPropsGiftInfoReq.Pass9Account));
	stCashPropsGiftInfoReq.AddCash = pCashPropsGiftInfoReq->AddCash;
	stCashPropsGiftInfoReq.Uin = pCashPropsGiftInfoReq->Uin;
	stCashPropsGiftInfoReq.GroupID = pCashPropsGiftInfoReq->GroupID;
	
	ss_sendto_region_pkg(pstAppCtx,pstEnv,  BILL_CASH_PROPSGIFT_REQ,(char *)&stCashPropsGiftInfoReq,sizeof(SSCASHPROPSGIFTINFOREQ));

	return 0;
}
// 处理connd发来的消息


static int acc_cs_process_conn_msg(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
	                               char *pszMsg, int iMsgLen)
{
	int iRet = 0;
	const SSCSCTRL * pstCSCtrl = &pstEnv->stSSPkg.Body.CsCtrL;

	if (pstEnv->stIncomingFrameHead.Cmd != TFRAMEHEAD_CMD_START)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "pstFrameHead->Cmd=%d", 
			         pstEnv->stIncomingFrameHead.Cmd);
		goto error;
	}

	if (ss_unpack(pstEnv->pstSSHan, pszMsg, iMsgLen, &pstEnv->stSSPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "bad ss package.");
		goto error;
	}

	if (pstEnv->stSSPkg.Head.Cmd != CS_CTRL)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "bad ss package.");
		goto error;
	}

	tlog_info_dr(pstEnv->pstLogCat, 0, 0, pstEnv->pstSSHan->pstSSProtoPkgMeta, 
					&pstEnv->stSSPkg, sizeof(pstEnv->stSSPkg), 0);
		
	switch(pstCSCtrl->Cmd)
	{
	case GET_IP_LIMIT_REQ:
		iRet = ss_ip_limit_msg_trans(pstAppCtx, pstEnv, GET_IP_LIMIT_REQ);
		break;

	case IP_LIMIT_REQ:
		iRet = ss_ip_limit_msg_trans(pstAppCtx, pstEnv, IP_LIMIT_REQ);
		break;

	case GET_ACCOUNT_INFO_REQ:
		iRet = ss_get_account_info(pstAppCtx, pstEnv, 
		                           &pstCSCtrl->CsCmd.GetAccInfoReq);
		break;

	case RESTORE_ROLE_REQ:
		iRet = ss_restore_role(pstAppCtx, pstEnv,
		                       &pstCSCtrl->CsCmd.RestoreRoleReq);
		break;

	case SET_ACCOUNT_FREEZE_REQ:
		iRet = ss_set_account_freeze(pstAppCtx, pstEnv, 
			                         &pstCSCtrl->CsCmd.SetAccFreezeReq);
		break;

	case SET_ACCOUNT_SILENCE_REQ:
		iRet = ss_set_account_silence(pstAppCtx, pstEnv, 
		                              &pstCSCtrl->CsCmd.SetAccSilenceReq);
		break;

	case SET_ACCOUNT_ROLENAME_REQ:
		iRet = ss_set_account_rolename(pstAppCtx, pstEnv,
		                               &pstCSCtrl->CsCmd.SetAccRoleNameReq);
		break;

	case SET_ACCOUNT_UIN_FLAG_REQ:
		iRet = ss_set_account_uin_flag(pstAppCtx, pstEnv,
		                               &pstCSCtrl->CsCmd.SetAccountUinFlagReq);
		break;

	case FIND_ACCOUNT_ROLELIST_REQ:
		iRet = ss_find_account_rolelist(pstAppCtx, pstEnv,
		                              &pstCSCtrl->CsCmd.FindAccountRoleListReq);
		break;

	case FIND_ACCOUNT_REQ:
		iRet = ss_find_account(pstAppCtx, pstEnv,
		                       &pstCSCtrl->CsCmd.FindAccountReq);
		break;
	case CS_QUERY_BILL_REQ:
		iRet = cs_query_bill_point(pstAppCtx, pstEnv,
		                       (BILLRECORD *)&pstCSCtrl->CsCmd.QeuryBillRecordReq);
		break;
	case CS_CASH_PROPS_BILL_REQ:
		iRet = ss_cash_props_gift_bill(pstAppCtx, pstEnv,(CASHPROPSGIFTINFOREQ *)&pstCSCtrl->CsCmd.CashPropsGiftInfoReq);
		break;
	case GET_ACC_SAFELOCK_REQ:
		iRet = db_get_account_info_req(pstAppCtx, pstEnv,pstCSCtrl->CsCmd.GetAccSafeLockReq.AccName,
										DB_RES_SELECT_ACCOUNT_PASSPWD,NULL, 0);
		break;
	case SET_ACC_SAFELOCK_REQ:
		{
			const SETACCSAFELOCKREQ *pstReq = &pstCSCtrl->CsCmd.SetAccSafeLockReq;
			iRet = db_get_account_info_req(pstAppCtx, pstEnv,pstReq->AccName,
										DB_RES_SET_PASSPWD,(void*)pstReq, sizeof(*pstReq));
		}
		
		break;
	default:
		tlog_error(pstEnv->pstLogCat, 0, 0, "bad ss package.");
		goto error;
		break;
	}

final:
	return iRet;

error:
	iRet = -1;
	goto final;
	return 0;
}

// 处理auth_svr发来的消息
static int acc_cs_process_auth_msg(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
	                               char *pszMsg, int iMsgLen)
{
	// TODO :
	UNUSED(pstAppCtx);
	SSPKG stZoneRes;
	SSPKG stPkg;
	TDRDATA stNet;
	char szBuff[sizeof(SSPKG)];
	stNet.pszBuff = szBuff;
	stNet.iBuff = sizeof(szBuff);
	SSCSCTRL *pstCtrl;
	TFRAMEHEAD stFrameHead;
	
	if (ss_unpack(pstEnv->pstSSHan, pszMsg, iMsgLen, &stZoneRes, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_unpack");
		return -1;
	}

	pstCtrl = &stZoneRes.Body.CsCtrL;
	if (pstCtrl->Cmd == ZONE_RES)
	{
		if (pstCtrl->CsCmd.ZoneRes.AsynReq.Cmd == IP_LIMIT_REQ)
		{
			stPkg.Body.CsCtrL.Cmd = IP_LIMIT_RES;
		}
		else if (pstCtrl->CsCmd.ZoneRes.AsynReq.Cmd == GET_IP_LIMIT_REQ)
		{
			stPkg.Body.CsCtrL.Cmd = GET_IP_LIMIT_RES;
			if (pstCtrl->CsCmd.ZoneRes.AsynReq.DataLen != sizeof(ZONEGETIPLIMITRES))
			{
				return -1;
			}
			memcpy(&stPkg.Body.CsCtrL.CsCmd.GetIPLimitRes,
					pstCtrl->CsCmd.ZoneRes.AsynReq.Data, sizeof(ZONEGETIPLIMITRES));
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

	stPkg.Head.Cmd = stZoneRes.Head.Cmd;
	stPkg.Head.HeadLen = sizeof(SSPKGHEAD);
	stPkg.Head.Magic = PROTOCOL_MAGIC;
	stPkg.Head.Ver = TDR_METALIB_PROTO_SS_VERSION;
	
	if (ss_pack(pstEnv->pstSSHan, &stNet, &stPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack");
		return -1;
	}

	
	memset(&stFrameHead, 0,sizeof(TFRAMEHEAD));
	stFrameHead.Cmd = TFRAMEHEAD_CMD_STOP;
	stFrameHead.TimeStampType = 0;
	stFrameHead.ID = -1;
	stFrameHead.ConnIdx = stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.ConnIdx;
	memcpy(&stFrameHead.ExtraInfo.IPInfo,
	       &stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.IPInfo,sizeof(TIPINFO));
	stFrameHead.ExtraType = TFRAMEHEAD_EXTRA_IP;
	
	if (tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iAccountCSConnId,
	                  stNet.pszBuff, stNet.iBuff, &stFrameHead) != 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tconnapi_send fail");
		return -1;
	}
	return 0;
}
static int cs_query_bill_point(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
			BILLRECORD *pstQeuryBillRecordReq)
{
	
	TORMSELECTREQ *pstDBReq = &pstEnv->stTormPkg.Body.SelectReq;
	BILL stBillInfo;
	
	db_init_torm_head(pstAppCtx, &pstEnv->stTormPkg.Head, TORM_SELECT_REQ, 
		              "Bill", 0);
	db_set_callback_ctx(
		&pstEnv->stTormPkg.Head,
		DB_RES_SELECT_BILL_RECORD,
		0,
		&pstEnv->stIncomingFrameHead,
		(void *)pstQeuryBillRecordReq,
		sizeof(BILLRECORD));

	pstDBReq->OffSet = 0;
	pstDBReq->Limit = 0;
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	snprintf(pstDBReq->OperInfo.WhereDef, CCH(pstDBReq->OperInfo.WhereDef),
	         "WHERE Uin=%u", pstQeuryBillRecordReq->Uin);

	bzero(&stBillInfo, sizeof(stBillInfo));
	//STRNCPY(stBillInfo.Pass9Account, pstQeuryBillRecordReq->Pass9Account, 
	//	CCH(stBillInfo.Pass9Account));
	stBillInfo.Uin = pstQeuryBillRecordReq->Uin;
	return db_send_req(pstAppCtx, pstEnv, &stBillInfo, sizeof(stBillInfo));
}

static int db_query_bill_res(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv, 
	                                 TORMPKG *pstPkg, 
	                                 TormCallbackCtx *pstCallbackCtx)
{
	int iRet = 0;
	SSPKG *pstSSPkg = &pstEnv->stSSPkg;
	BILL stBillInfo;
	BILLRECORD *pstBillRes =
		&pstSSPkg->Body.CsCtrL.CsCmd.QeuryBillRecordRes;
	const TORMSELECTRES *pstDBRes = &pstPkg->Body.SelectRes;

	UNUSED(pstAppCtx);
	if (TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			         "Select Bill info from db failed. dberr = %d, msg = %s.", 
			         pstDBRes->DBMSErrNo,
			         torm_error_string(pstDBRes->ErrNo));
		return -1;
	}

	bzero(pstBillRes, sizeof(*pstBillRes));
	if (pstDBRes->SelectResult.ResultNum == 1)
	{
		//有数据
		memset(&stBillInfo,0,sizeof(stBillInfo));
		if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stBillInfo, sizeof(BILL))
			 < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,	"torm_fetch_one_meta fail");
			return -1;
		}
		//STRNCPY(pstBillRes->Pass9Account, stBillInfo.Pass9Account,
		//        CCH(pstBillRes->Pass9Account));
		pstBillRes->Uin = stBillInfo.Uin;
		pstBillRes->ServerID = stBillInfo.ServerID;
		pstBillRes->Amount = stBillInfo.Amount;
		pstBillRes->Points = stBillInfo.Points;
		pstBillRes->Bonus = stBillInfo.Bonus;
		pstBillRes->FreezePoint = stBillInfo.FreezePoint;
		pstBillRes->FreezeLastTime = stBillInfo.FreezeLastTime;		
		
	}
	else if (pstDBRes->SelectResult.ResultNum == 0)
	{
		//没有用户记录,就是0
		STRNCPY(pstBillRes->Pass9Account,
		        pstCallbackCtx->stData.stQueryBillRecordReq.Pass9Account,
		        sizeof(pstBillRes->Pass9Account));
	}

	_ss_init_head(&pstEnv->stSSPkg.Head, CS_CTRL);
	pstEnv->stSSPkg.Body.CsCtrL.Cmd = CS_QUERY_BILL_RES;
	iRet = conn_send_pkg(pstEnv, &pstCallbackCtx->stConnInfo, &pstEnv->stSSPkg);
	return iRet;
}

// 处理account_db发来的消息
static int acc_cs_process_db_msg(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
	                             char *pszMsg, int iMsgLen)
{
	TDRDATA stData;
	TORMPKG stPkg;
	TORMPKG *pstPkg = &stPkg;
	TormCallbackCtx stCallbackCtx;
	int iRet = 0;
	int iProtoVer = pstEnv->iAccountDBProtoVer;

	stData.pszBuff = (char *)pszMsg;
	stData.iBuff = iMsgLen;
	iRet =  torm_attach_res_msg(pstEnv->hTorm, &stData, pstPkg, iProtoVer);
	if (iRet < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			       "torm_attach_res_msg error:%s, iProtoVer=%d", 
		           torm_error_string(iRet), iProtoVer);
		return -1;
	}

	if (pstPkg->Head.AsynCallBackDataLen > 0)
	{
		memcpy(&stCallbackCtx, pstPkg->Head.AsynCallBackData, 
			   pstPkg->Head.AsynCallBackDataLen);
	}
	else
	{
		stCallbackCtx.iCmd = -1;
	}

	tlog_info(pstEnv->pstLogCat, 0,0, "get msg from db cmd=%d, asyn cmd=%d",
	          pstPkg->Head.HeadCmd.Cmd, stCallbackCtx.iCmd);

	switch(pstPkg->Head.HeadCmd.Cmd)
	{
	case TORM_SELECT_RES:
		switch(stCallbackCtx.iCmd)
		{
		case DB_RES_ACCOUNT_INFO:
			iRet = db_l2u_res(pstAppCtx, pstEnv, pstPkg, &stCallbackCtx);
			break;

		case DB_RES_ACCOUNT_LITE:
			iRet = db_account_lite_res(pstAppCtx, pstEnv, pstPkg,
			                           &stCallbackCtx);
			break;

		case DB_RES_ACCOUNT_ROLELIST:
			iRet = db_account_rolelist_res(pstAppCtx, pstEnv, pstPkg,
			                               &stCallbackCtx);
			break;
		case DB_RES_SELECT_BILL_RECORD:
			iRet = db_query_bill_res(pstAppCtx, pstEnv, pstPkg,
			                               &stCallbackCtx);
			break;
	
		}
		break;

	case TORM_EXGVER_RES:
		pstEnv->iAccountDBProtoVer = pstPkg->Body.ExgVerRes.Version;
		break;
		
	case TORM_UPDATE_RES:
		switch(stCallbackCtx.iCmd)
		{
		case DB_RES_ACCOUNT_ROLE_UPDATE_DELTIME:
			iRet = db_account_role_update_deltime_res(pstAppCtx, pstEnv, pstPkg,
			                                          &stCallbackCtx);
			break;

		case DB_RES_ACCOUNT_SILENCE:
			iRet = db_account_silence_res(pstAppCtx, pstEnv, pstPkg,
			                              &stCallbackCtx);
			break;

		case DB_RES_ACCOUNT_FREEZE:
			iRet = db_account_freeze_res(pstAppCtx, pstEnv, pstPkg,
			                             &stCallbackCtx);
			break;

		case DB_RES_ACCOUNT_ROLE_UPDATE_NAME:
			iRet = db_account_role_update_name_res(pstAppCtx, pstEnv, pstPkg, 
			                                       &stCallbackCtx);
			break;

		case DB_RES_ACCOUNT_UIN_FLAG:
			iRet = db_account_uin_flag_res(pstAppCtx, pstEnv, pstPkg,
			                               &stCallbackCtx);
			break;
		case DB_RES_SET_PASSPWD:
			return db_set_acc_passwd_res(pstAppCtx,pstEnv, pstPkg,&stCallbackCtx);
			break;
		}
		break;

	default:
		break;
	}

	return iRet;
}

int account_cs_svr_attach(ACCOUNT_CS_SVRENV *pstEnv)
{
	int iShmID;
	
	iShmID = shmget(pstEnv->pstConf->ResShmKey, sizeof(ZoneObj), 0666);
	if (iShmID < 0)
	{
		return -1;
	}

	if (pstEnv->pstZoneObj)
	{
		shmdt(pstEnv->pstZoneObj);
	}
	
	pstEnv->iShmID = iShmID;
	pstEnv->pstZoneObj = (ZoneObj *)shmat(iShmID, NULL, 0);
	if (!pstEnv->pstZoneObj)
	{
		return -1;
	}

	if (pstEnv->pstZoneObj->iVersion != OBJ_SHM_VER)
	{
		return -1;
	}

	pstEnv->pstShtNameMemID = sht_attach(pstEnv->pstZoneObj->sNameSht,
		                                 sizeof(pstEnv->pstZoneObj->sNameSht),
	                                     PLAYER_BUCKET, MAX_PLAYER,
	                                     sizeof(NameMemID));
	if (pstEnv->pstShtNameMemID == NULL)
	{
		return -1;
	}
	return 0;
}

int account_cs_svr_init(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv)
{
	if (pstAppCtx->stConfData.pszBuff == NULL ||
		pstAppCtx->stConfData.iMeta == 0 ||
		pstAppCtx->pszConfFile == NULL)
	{
		return -1;
	}
	pstEnv->pstConf = (ACCOUNT_CS_SVRCONF *)pstAppCtx->stConfData.pszBuff;

	pstEnv->pstRunStat = 
		(ACCOUNT_CS_SVRRUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
	if (pstEnv->pstRunStat == NULL)
	{
		pstEnv->pstRunStat = &gs_stStat;
	}

	if (tapp_get_category(NULL, (void *)(&pstEnv->pstLogCat)) < 0)
	{		
		printf("tapp_get_category run fail\n");
		return -1;	
	}	

	GET_WORLD_ENTITY_ID(pstEnv->iAccountDBID, pstAppCtx->iId, FUNC_ACCOUNT_DB);
	GET_WORLD_ENTITY_ID(pstEnv->iAccountCSConnId, pstAppCtx->iId, 
		                  FUNC_ACCOUNT_CS_CONN);
	//GET_WORLD_ENTITY_ID(pstEnv->iAuthBakID, pstAppCtx->iId, FUNC_AUTH_BAK);
	GET_WORLD_ENTITY_ID(pstEnv->iAuthSvrID, pstAppCtx->iId, FUNC_AUTH_SVR);
	//GET_WORLD_ENTITY_ID(pstEnv->iAuthSlaveID, pstAppCtx->iId, FUNC_AUTH_SLAVE);

	GET_WORLD_ENTITY_ID(pstEnv->iRegionSvrID, pstAppCtx->iId, FUNC_REGION_GATE);
	
	if (tconnapi_initialize(pstAppCtx->pszGCIMKey, pstAppCtx->iBusinessID) < 0)
	{
		printf("tbus gcim key %s init fail\n", pstAppCtx->pszGCIMKey) ;
		return -1;
	}
	
	if (tconnapi_create(pstAppCtx->iId, &pstEnv->iTconnapiHandle) < 0)
	{
		printf("tbus id %s init fail\n", pstAppCtx->pszId);
		return -1;
	}

	pstEnv->cIsAttach = 0;
	
	if (ss_han_create(g_szMetalib_proto_ss, &pstEnv->pstSSHan) < 0)
	{
		printf("ss_han_create fail\n");
		return -1;
	}

	if (torm_create_handle_from_dir(&pstEnv->hTorm,
		                            pstEnv->pstConf->DBMetaDir) < 0)
	{
		printf("torm_create_handle fail\n");
		return -1;
	}
	
	printf("account_cs_svr start\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "account_cs_svr start");

	return 0;
}
// 处理account_db发来的消息
static int acc_cs_process_region_msg(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv,
	                               char *pszMsg, int iMsgLen)
{
	UNUSED(pstAppCtx);
	SSPKG stPkgRes;
	
	//ASYNREQ stAsyn;
	SSPKG stPkg;
	SSCSCTRL *  pstCSCtrl = &stPkg.Body.CsCtrL;
	TDRDATA stNet;
	char szBuff[sizeof(SSPKG)];
	stNet.pszBuff = szBuff;
	stNet.iBuff = sizeof(szBuff);
	CASHPROPSGIFTINFORES stCashPropsGiftInfoRes;

	TFRAMEHEAD stFrameHead;
	
	if (ss_unpack(pstEnv->pstSSHan, pszMsg, iMsgLen, &stPkgRes, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_unpack fail");
		return -1;
	}
	if (stPkgRes.Head.Cmd != BILL_CASH_PROPSGIFT_RES)
	{
		return -1;
	}
	
	//stAsyn = stPkgRes.Body.CashPropsGiftInfoRes.AsynReq;

	memset(&stCashPropsGiftInfoRes, 0, sizeof(stCashPropsGiftInfoRes));
	stCashPropsGiftInfoRes.Result = stPkgRes.Body.CashPropsGiftInfoRes.Result;
	STRNCPY(stCashPropsGiftInfoRes.Pass9Account,
	        stPkgRes.Body.CashPropsGiftInfoRes.Pass9Account,
	        CCH(stCashPropsGiftInfoRes.Pass9Account));
		
	stPkg.Head.Cmd = CS_CTRL;
	stPkg.Head.HeadLen = sizeof(SSPKGHEAD);
	stPkg.Head.Magic = PROTOCOL_MAGIC;
	stPkg.Head.Ver = TDR_METALIB_PROTO_SS_VERSION;

	pstCSCtrl->Cmd = CS_CASH_PROPS_BILL_RES;
	memcpy(&pstCSCtrl->CsCmd.CashPropsGiftInfoRes,
	       (char *)&stCashPropsGiftInfoRes,
	       sizeof(stCashPropsGiftInfoRes));
	
	if (ss_pack(pstEnv->pstSSHan, &stNet, &stPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack");
		return -1;
	}

	
	memset(&stFrameHead, 0,sizeof(TFRAMEHEAD));
	stFrameHead.Cmd = TFRAMEHEAD_CMD_STOP;
	stFrameHead.TimeStampType = 0;
	stFrameHead.ID = -1;
	stFrameHead.ConnIdx = stPkgRes.Body.CashPropsGiftInfoRes.CsConnCtx.ConnIdx;
	memcpy(&stFrameHead.ExtraInfo.IPInfo,
			&stPkgRes.Body.CashPropsGiftInfoRes.CsConnCtx.IPInfo,sizeof(TIPINFO));
	stFrameHead.ExtraType = TFRAMEHEAD_EXTRA_IP;
	
	if (tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iAccountCSConnId,
	                  stNet.pszBuff, stNet.iBuff, &stFrameHead) != 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tconnapi_send fail");
		return -1;
	}

	return 0;
}

int account_cs_svr_proc(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv)
{
	int iBusy=0;
	int iLoopNum = 0;
	TBUSADDR iMsgDst ;
	TBUSADDR iMsgSrc ;
	int iMsgLen;
	int iRet = 0;
	char *pszMsg;
	int iFrameHeadLen ;
	
	while(iLoopNum < LOOP_NUM)
	{
		iLoopNum++;
		
		iMsgDst = pstAppCtx->iId;
		iMsgSrc = TBUS_ADDR_ALL;	
		iMsgLen = sizeof(pstEnv->szRecvBuffer);
		pszMsg = &pstEnv->szRecvBuffer[0];
		if (tbus_recv(pstAppCtx->iBus, &iMsgSrc, &iMsgDst, pszMsg,
		              (size_t *)&iMsgLen, 0) != TBUS_SUCCESS)
		{
			break;
		}
		iBusy = 1;

		if (iMsgSrc == pstEnv->iAccountCSConnId)
		{
			iFrameHeadLen = sizeof(pstEnv->stIncomingFrameHead);
			iRet = tconnapi_decode(pszMsg, iMsgLen,
			                       &pstEnv->stIncomingFrameHead,
			                       &iFrameHeadLen);
			if (iRet != 0)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, 
					       "tconnapi_decode failed, iRet0x%x, Msglen:%d", iRet,
					       iMsgLen);
				continue;
			}

			pszMsg += iFrameHeadLen;
			iMsgLen -= iFrameHeadLen;

			iRet = acc_cs_process_conn_msg(pstAppCtx, pstEnv, pszMsg, iMsgLen);
		}
		else if(iMsgSrc == pstEnv->iAuthSvrID)
		{
			iRet = acc_cs_process_auth_msg(pstAppCtx, pstEnv, pszMsg, iMsgLen);
		}
		else if( iMsgSrc == pstEnv->iAccountDBID)
		{
			iRet = acc_cs_process_db_msg(pstAppCtx, pstEnv, pszMsg, iMsgLen);
		}
		else if(iMsgSrc == pstEnv->iRegionSvrID)
		{
			iRet = acc_cs_process_region_msg(pstAppCtx, pstEnv, pszMsg, iMsgLen);
		}
		else
		{
			// bite me.
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

int account_cs_svr_reload(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv)
{
	ACCOUNT_CS_SVRCONF *pstPreConf;

	pstPreConf = (ACCOUNT_CS_SVRCONF *)pstAppCtx->stConfPrepareData.pszBuff;
	
	printf("account_cs_svr reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "account_cs_svr reload");
	return 0;
}

int account_cs_svr_fini(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv)
{
	UNUSED(pstAppCtx);

	printf("account_cs_svr finish\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "account_cs_svr finish");
	return 0;
}

int account_cs_svr_tick(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv)
{
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);

	return 0;
}

int account_cs_svr_stop(TAPPCTX *pstAppCtx, ACCOUNT_CS_SVRENV *pstEnv)
{
	int iReady = 0;

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
	void* pvArg	= &gs_stEnv;

	memset(&gs_stAppCtx, 0, sizeof(gs_stAppCtx));
	memset(&gs_stEnv, 0, sizeof(gs_stEnv));

	gs_stAppCtx.argc = argc;
	gs_stAppCtx.argv = argv;

	gs_stAppCtx.pfnInit	= (PFNTAPPFUNC)account_cs_svr_init;

	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_account_cs_svr;
	gs_stAppCtx.stConfData.pszMetaName = "account_cs_svrconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);

	iRet = tapp_def_init(&gs_stAppCtx, pvArg);
	if (iRet < 0)
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	gs_stAppCtx.pfnFini	= (PFNTAPPFUNC)account_cs_svr_fini;
	gs_stAppCtx.pfnProc	= (PFNTAPPFUNC)account_cs_svr_proc;
	gs_stAppCtx.pfnTick	= (PFNTAPPFUNC)account_cs_svr_tick;
	gs_stAppCtx.pfnReload = (PFNTAPPFUNC)account_cs_svr_reload;
	gs_stAppCtx.pfnStop = (PFNTAPPFUNC)account_cs_svr_stop;

	iRet = tapp_def_mainloop(&gs_stAppCtx, pvArg);
	tapp_def_fini(&gs_stAppCtx, pvArg);
	return iRet;
}


