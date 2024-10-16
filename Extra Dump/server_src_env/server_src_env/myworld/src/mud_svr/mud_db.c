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

#include "mud_svr_conf_desc.h"
#include "mud_svr.h"
#include "mud_clt.h"
#include "tapp/tapp.h"
#include "tdr/tdr.h"
#include "tlog/tlog.h"
#include "comm/tmempool.h"
#include "tconnapi.h"
#include "comm/shtable_shm.h"
#include "tresloader.h"
#include "entity.h"
#include "tbus/tbus.h"
#include "tormapi/torm_protocol_api.h"
#include "mud_player.h"
#include "mud_db.h"

#pragma pack(1)
enum tagDBResCode
{
	DB_ROLE_SELECT_RES,
	DB_ROLE_LIST_SELECT_RES,
	DB_SHADOW_SELECT_RES,
	DB_SHADOW_UPDATE_RES,
	DB_BUDDY_SELECT_RES,
	DB_BUDDY_UPDATE_RES,
	DB_ACC_SHARE_SELECT_RES,
	DB_ACC_SHARE_UPDATE_RES,
};

enum tagDBDest
{
	TO_ROLE_DB,
	TO_MISC_DB,
};

typedef struct tagDBExtra DBExtra;

struct tagDBExtra{
	int iCmd;
	int iRes;
};
#pragma pack()

static int db_select_role_res(MudPlayer *pstPlayer, const TORMPKG *pstDBPkg,
                              const DBExtra *pstExtra);

static int db_select_role_list_res(MudPlayer *pstPlayer, const TORMPKG *pstDBPkg,
                                   const DBExtra *pstExtra);

static int db_select_shadow_res(MudPlayer *pstPlayer, const TORMPKG *pstDBPkg,
                                const DBExtra *pstExtra);

static int db_update_shadow_res(MudPlayer *pstPlayer, const TORMPKG *pstDBPkg,
                                const DBExtra *pstExtra);

static int db_send_req(TORMPKG *pstPkg, const void *pvKey, int iKeyLen,
                       int iToWhere);

static int db_torm_send(TORMPKG *pstPkg, int iDstID, int iProtoVer);

static void db_init_torm_head(const char *pszMetaName, const DBExtra *pstExtra,
                              int iDBCmd, int iUin, TORMPKGHEAD *pstHead);

int db_select_acc_share_req(MudPlayer *pstPlayer, tdr_ulonglong ullRoleID);

static int db_select_acc_share_res(MudPlayer *pstPlayer, const TORMPKG *pstDBPkg,
                                const DBExtra *pstExtra);



static inline void init_shadow_data(const MudRoleData *pstFrom,
                                    SHADOWACCDATA* pstTo)
{
	pstTo->Uin = pstFrom->iUin;
}

int mud_torm_exchg_ver()
{
	MUD_SVRENV *pstEnv = this_env();
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	
	db_init_torm_head(NULL, NULL, TORM_EXGVER_REQ, -1, &pstPkg->Head);
	pstPkg->Body.ExgVerReq.Version = TDR_METALIB_TORM_NET_VERSION;

	db_send_req(pstPkg, &pstPkg, sizeof(*pstPkg), TO_ROLE_DB);
	db_send_req(pstPkg, &pstPkg, sizeof(*pstPkg), TO_MISC_DB);
	
	return 0;
}

int db_buddy_update_res(MudPlayer *pstPlayer, const TORMPKG *pstDBPkg,
                                const DBExtra *pstExtra)
{
	const TORMUPDATERES *pstRes = &pstDBPkg->Body.UpdateRes;

	UNUSED(pstExtra);

	if(TORM_ERR_IS_ERROR(pstRes->ErrNo))
	{
		tlog_error(
			this_env()->pstLogCat, 0, 0, 
			"Update shadow data to db failed. code = %d, uin = %d, msg = %s.", 
			pstRes->DBMSErrNo,
			pstPlayer->stRoleData.iUin,
			torm_error_string(pstRes->ErrNo));
		return -1;
	}

	pstPlayer->iBuddyDirty = 0;

	return 0;
}


int db_buddy_select_res(MudPlayer *pstPlayer, const TORMPKG *pstDBPkg,const DBExtra *pstExtra)
{
	const TORMSELECTRES* pstRes = &pstDBPkg->Body.SelectRes;
	int i = 0;
	ROLEDATABUDDY stRoleBuddy;
	stRoleBuddy.RoleID = 0;
	
	UNUSED(pstExtra);

	if(TORM_ERR_IS_ERROR(pstRes->ErrNo))
	{
		tlog_error(
			this_env()->pstLogCat, 0, 0, 
			"db_buddy_select_req failed. code = %d, uin = %d, msg = %s.", 
			pstRes->DBMSErrNo,
			pstPlayer->stRoleData.iUin,
			torm_error_string(pstRes->ErrNo));
		return -1;
	}

	for(i = 0; i < pstRes->SelectResult.ResultNum; i++)
	{

		if(torm_fetch_one_meta(this_env()->hTorm, (char *)&stRoleBuddy,
		                       sizeof(stRoleBuddy)) < 0)
		{
			tlog_error(this_env()->pstLogCat, pstPlayer->stRoleData.iUin, 0, 
					   "torm_fetch_one_meta faild, uin = %d",
					   pstPlayer->stRoleData.iUin);
			continue;
		}

		break;
	}

	if (stRoleBuddy.RoleID)
	{
		stRoleBuddy.Buddy.FriendNum = pstPlayer->stRoleData.iFriendNum;
		stRoleBuddy.Buddy.FriendNum = min (stRoleBuddy.Buddy.FriendNum ,pstPlayer->stRoleData.iFriendMax);
		memcpy(stRoleBuddy.Buddy.Friends, pstPlayer->stRoleData.astFriends,
			stRoleBuddy.Buddy.FriendNum * sizeof(stRoleBuddy.Buddy.Friends[0]));

		if (pstPlayer->stRoleData.EmotionAlterTime)
		{
			STRNCPY(stRoleBuddy.Buddy.Emotion,pstPlayer->stRoleData.Emotion,
				CCH(stRoleBuddy.Buddy.Emotion));
			stRoleBuddy.Buddy.EmotionAlterTime = pstPlayer->stRoleData.EmotionAlterTime;
			pstPlayer->stRoleData.EmotionAlterTime = 0;
		}
		db_buddy_update_req(pstPlayer,&stRoleBuddy);
	}

	return 0;
}


// TORM 消息分派
int mud_dispatch_db_msg(MUD_SVRENV *pstEnv, const char *pszMsg, int iMsgLen,
                        int iMsgSrc)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	MudPlayer *pstPlayer = NULL;
	int iUin, iProtoVer, iRet;
	TDRDATA stData;
	DBExtra stDBExtra;

	if(iMsgSrc == pstEnv->iRoleDBID)
	{
		iProtoVer = pstEnv->iRoleDBProtoVer;
	}
	else if(iMsgSrc == pstEnv->iMiscDBID)
	{
		iProtoVer = pstEnv->iMiscDBProtoVer;
	}
	else
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "unknown db msg source %d",
		           iMsgSrc);
		return -1;
	}

	stData.pszBuff = (char *)pszMsg;
	stData.iBuff = iMsgLen;
	iRet =  torm_attach_res_msg(pstEnv->hTorm, &stData, pstPkg, iProtoVer);
	if(iRet < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "torm_attach_res_msg error:%s, iProtoVer=%d",
		           torm_error_string(iRet), iProtoVer);
		return -1;
	}

	iUin = pstPkg->Head.HeadApp.ObjID;
	if(iUin > 0)
	{
		pstPlayer = mud_player_get_by_uin(iUin);
	}

	if(pstPkg->Head.AsynCallBackDataLen > 0 &&
	   pstPkg->Head.AsynCallBackDataLen <= (int)sizeof(stDBExtra))
	{
		memcpy(&stDBExtra, pstPkg->Head.AsynCallBackData,
		       pstPkg->Head.AsynCallBackDataLen);
	}
	else
	{
		stDBExtra.iCmd = 0;
	}

	tlog_debug(pstEnv->pstLogCat, 0, 0, "recv db msg uin=%d,cmd=%d,isrc=%d",
	           iUin, pstPkg->Head.HeadCmd.Cmd, iMsgSrc);

	switch(pstPkg->Head.HeadCmd.Cmd)
	{
	case TORM_EXGVER_RES:
		if (iMsgSrc == pstEnv->iRoleDBID)
		{
			pstEnv->iRoleDBProtoVer = pstPkg->Body.ExgVerRes.Version;
		}
		else if(iMsgSrc == pstEnv->iMiscDBID)
		{
			pstEnv->iMiscDBProtoVer = pstPkg->Body.ExgVerRes.Version;
		}
		iRet = 0;
		break;
	case TORM_SELECT_RES:
		if(!pstPlayer) // pstPlayer为NULL就逻辑错误了
		{
			iRet = -1;
			break;
		}
		switch(stDBExtra.iCmd)
		{
		case DB_ROLE_SELECT_RES:
			iRet = db_select_role_res(pstPlayer, pstPkg, &stDBExtra);
			break;
		case DB_ROLE_LIST_SELECT_RES:
			iRet = db_select_role_list_res(pstPlayer, pstPkg, &stDBExtra);
			break;
		case DB_SHADOW_SELECT_RES:
			iRet = db_select_shadow_res(pstPlayer, pstPkg, &stDBExtra);
			break;
		case DB_BUDDY_SELECT_RES:
			iRet = db_buddy_select_res(pstPlayer, pstPkg, &stDBExtra);
			break;
		case DB_ACC_SHARE_SELECT_RES:
			iRet = db_select_acc_share_res(pstPlayer, pstPkg, &stDBExtra);
			break;
		default:
			iRet = -1;
			break;
		}
		break;
	case TORM_UPDATE_RES:
		if(!pstPlayer)
		{
			iRet = -1;
			break;
		}
		switch(stDBExtra.iCmd)
		{
		case DB_SHADOW_UPDATE_RES:
			iRet = db_update_shadow_res(pstPlayer, pstPkg, &stDBExtra);
			break;
		
		case DB_BUDDY_UPDATE_RES:
			iRet = db_buddy_update_res(pstPlayer, pstPkg, &stDBExtra);
			break;
		default:
			iRet = -1;
			break;
		}
		break;
	case TORM_INSERT_RES:
		break;
	default:
		break;
	}
	return iRet;
}

static int db_select_role_res(MudPlayer *pstPlayer, const TORMPKG *pstDBPkg,
                              const DBExtra *pstExtra)
{
	MUD_SVRENV *pstEnv = this_env();
	const TORMSELECTRES *pstRes = &pstDBPkg->Body.SelectRes;
	ROLEDATA stStub;

	UNUSED(pstExtra);

	if(TORM_ERR_IS_ERROR(pstRes->ErrNo))
	{
		tlog_error(
			pstEnv->pstLogCat, 0, 0, 
			"Select role data from db failed. code = %d, uin = %d, msg = %s.", 
			pstRes->DBMSErrNo,
			pstPlayer->stRoleData.iUin,
			torm_error_string(pstRes->ErrNo));
		return -1;
	}
	
	if (pstRes->SelectResult.ResultNum != 1)
	{
		tlog_error(
			pstEnv->pstLogCat, 0, 0, 
			"Select role data ResultNum = %d, uin = %d", 
			pstRes->SelectResult.ResultNum,
			pstPlayer->stRoleData.iUin);
			
		return -1;
	}
	
	if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stStub, sizeof(stStub)) < 0)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0, 
				   "torm_fetch_one_meta faild, uin = %d",
				   pstPlayer->stRoleData.iUin);
		return -1;
	}

	role_to_mud(&stStub, &pstPlayer->stRoleData);

	return db_select_acc_share_req(pstPlayer, pstPlayer->stRoleData.ullRoleID);
}

int db_select_role_req(MudPlayer *pstPlayer, tdr_ulonglong ullRoleID)
{	
	TORMPKG *pstPkg = &this_env()->stOrmPkg;
	TORMSELECTREQ *pstDBReq = &pstPkg->Body.SelectReq;
	ROLEDATA stKey;
	DBExtra stDBExtra;
		
	stDBExtra.iCmd = DB_ROLE_SELECT_RES;
	db_init_torm_head("RoleData", &stDBExtra, TORM_SELECT_REQ,
	                  pstPlayer->stRoleData.iUin, &pstPkg->Head);
		
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OperInfo.WhereDef[0] = 0;
	pstDBReq->OffSet = 0;
	pstDBReq->Limit = 0;

	memset(&stKey, 0, sizeof(stKey));
	stKey.Uin = pstPlayer->stRoleData.iUin;
	stKey.RoleID = ullRoleID;
		
	if(db_send_req(pstPkg, &stKey, sizeof(stKey), TO_ROLE_DB) < 0)
	{
		return -1;
	}
	


	pstPlayer->eStatus = MUD_PLAYER_STATUS_ROLE_LOGIN_REQ;
	return 0;
}

static int db_select_role_list_res(MudPlayer *pstPlayer, const TORMPKG *pstDBPkg,
                                   const DBExtra *pstExtra)
{
	MUD_SVRENV *pstEnv = this_env();
	int i = 0;
	const TORMSELECTRES *pstRes = &pstDBPkg->Body.SelectRes;
	ROLELISTINFO *pstRoleListInfo = NULL;
	MudUinCache *pstUinCache = NULL;
	int iValid = 0;

	UNUSED(pstExtra);

	pstUinCache = sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos,
	                      &iValid);
	if(!pstUinCache || !iValid ||
	   pstUinCache->iUin != pstPlayer->stRoleData.iUin)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0, 
				   "uin %d get cache pos=%d error", pstPlayer->stRoleData.iUin,
				   pstPlayer->iUinCachePos);
		return -1;
	}

	pstUinCache->cGetRoleList = 1;
	pstUinCache->ucRoleNum = 0;
	for(i = 0; i < pstRes->SelectResult.ResultNum; i++)
	{
		if(pstUinCache->ucRoleNum >= ROLE_MAX_NUM)
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0, 
					   "db_select_role_list_res uin = %d role list num %d",
					   pstPlayer->stRoleData.iUin,
					   pstRes->SelectResult.ResultNum);
			break;
		}
		
		pstRoleListInfo = &pstUinCache->astRoleListInfo[pstUinCache->ucRoleNum];
		if(torm_fetch_one_meta(pstEnv->hTorm, (char *)pstRoleListInfo,
		                       sizeof(*pstRoleListInfo)) < 0)
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0, 
					   "torm_fetch_one_meta faild, uin = %d",
					   pstPlayer->stRoleData.iUin);
			continue;
		}

		pstUinCache->ucRoleNum++;
	}

	pstPlayer->eStatus = MUD_PLAYER_STATUS_ROLE_LIST;

	return mud_send_role_list(pstPlayer, pstUinCache);
}

int db_select_role_list_req(MudPlayer *pstPlayer, int iUin)
{
	TORMPKG *pstPkg = &this_env()->stOrmPkg;
	TORMSELECTREQ *pstDBReq = &pstPkg->Body.SelectReq;
	ROLELISTINFO stKey;
	DBExtra stDBExtra;
	
	if(MUD_PLAYER_STATUS_ACC_LOGIN != pstPlayer->eStatus)
	{
		return -1;
	}

	stDBExtra.iCmd = DB_ROLE_LIST_SELECT_RES;
	db_init_torm_head("RoleListInfo", &stDBExtra, TORM_SELECT_REQ,
	                  pstPlayer->stRoleData.iUin, &pstPkg->Head);
	pstDBReq->OffSet = 0;
	pstDBReq->Limit = ROLE_MAX_NUM;
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OperInfo.WhereDef[0] = 0;
	snprintf(pstDBReq->OperInfo.WhereDef,
	         CCH(pstDBReq->OperInfo.WhereDef),
	         "where Uin=%d && CtrlFlag=0 order by Level DESC,LastLogin DESC",
	         iUin);

	stKey.Uin = iUin;
	if(db_send_req(pstPkg, &stKey, sizeof(stKey), TO_ROLE_DB) < 0)
	{
		return -1;
	}

	pstPlayer->eStatus = MUD_PLAYER_STATUS_ROLE_LIST_REQ;
	return 0;
}

static int db_select_shadow_res(MudPlayer *pstPlayer, const TORMPKG *pstDBPkg,
                                const DBExtra *pstExtra)
{
	MUD_SVRENV *pstEnv = this_env();
	const TORMSELECTRES *pstRes = &pstDBPkg->Body.SelectRes;

	UNUSED(pstExtra);

	if(TORM_ERR_IS_ERROR(pstRes->ErrNo))
	{
		tlog_error(
			pstEnv->pstLogCat, 0, 0, 
			"Select shadow data from db failed. code = %d, uin = %d, msg = %s.", 
			pstRes->DBMSErrNo,
			pstPlayer->stRoleData.iUin,
			torm_error_string(pstRes->ErrNo));
		return -1;
	}
	
	if(!pstRes->SelectResult.ResultNum)
	{
		// 角色第一次使用挂机吧，新建一个shadow数据
		init_shadow_data(&pstPlayer->stRoleData, &pstPlayer->stShadowData.stCore);
	}
	else
	{
		if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&pstPlayer->stShadowData.stCore,
	                       sizeof(pstPlayer->stShadowData.stCore)) < 0)
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0, 
					   "torm_fetch_one_meta faild, uin = %d",
					   pstPlayer->stRoleData.iUin);
			return -1;
		}
	}

	pstPlayer->stShadowData.iCacheFlag = CACHE_CLEAN;

	// TODO(niko) 状态切换

	return mud_role_login_res(pstPlayer);
}

static int db_select_acc_share_res(MudPlayer *pstPlayer, const TORMPKG *pstDBPkg,
                                const DBExtra *pstExtra)
{
	MUD_SVRENV *pstEnv = this_env();
	const TORMSELECTRES *pstRes = &pstDBPkg->Body.SelectRes;

	UNUSED(pstExtra);

	if(TORM_ERR_IS_ERROR(pstRes->ErrNo))
	{
		tlog_error(
			pstEnv->pstLogCat, 0, 0, 
			"Select acc share data from db failed. code = %d, uin = %d, msg = %s.", 
			pstRes->DBMSErrNo,
			pstPlayer->stRoleData.iUin,
			torm_error_string(pstRes->ErrNo));
		return -1;
	}
	
	if(!pstRes->SelectResult.ResultNum)
	{
		tlog_error(
			pstEnv->pstLogCat, 0, 0, 
			"Select acc share data from db ResultNum=0", 
			pstRes->DBMSErrNo,
			pstPlayer->stRoleData.iUin,
			torm_error_string(pstRes->ErrNo));
		return -1;
	}
	else
	{
		if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&pstPlayer->stAccShare,
	                       sizeof(pstPlayer->stAccShare)) < 0)
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0, 
					   "torm_fetch_one_meta faild, uin = %d",
					   pstPlayer->stRoleData.iUin);
			return -1;
		}
	}


	pstPlayer->stRoleData.iZoneRemainExp = pstPlayer->stAccShare.OfflineExp;
	pstPlayer->stRoleData.iTakeMudExp = pstPlayer->stAccShare.TakeMudExp;

	return db_select_shadow_req(pstPlayer,pstPlayer->stRoleData.iUin);
}


int db_select_acc_share_req(MudPlayer *pstPlayer, tdr_ulonglong ullRoleID)
{
	TORMPKG *pstPkg = &this_env()->stOrmPkg;
	TORMSELECTREQ *pstDBReq = &pstPkg->Body.SelectReq;
	ACCSHARE stKey;
	DBExtra stDBExtra;
		
	stDBExtra.iCmd = DB_ACC_SHARE_SELECT_RES;
	db_init_torm_head("AccShare", &stDBExtra, TORM_SELECT_REQ,
	                  pstPlayer->stRoleData.iUin, &pstPkg->Head);
		
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OperInfo.WhereDef[0] = 0;
	pstDBReq->OffSet = 0;
	pstDBReq->Limit = 0;

	memset(&stKey, 0, sizeof(stKey));
	stKey.Uin = pstPlayer->stRoleData.iUin;
		
	if(db_send_req(pstPkg, &stKey, sizeof(stKey), TO_MISC_DB) < 0)
	{
		return -1;
	}
	return 0;
	UNUSED(ullRoleID);	//add by paraunused.pl
}


int db_select_shadow_req(MudPlayer *pstPlayer, tdr_ulonglong ullRoleID)
{
	TORMPKG *pstPkg = &this_env()->stOrmPkg;
	TORMSELECTREQ *pstDBReq = &pstPkg->Body.SelectReq;
	SHADOWACCDATA stKey;
	DBExtra stDBExtra;
		
	stDBExtra.iCmd = DB_SHADOW_SELECT_RES;
	db_init_torm_head("ShadowAccData", &stDBExtra, TORM_SELECT_REQ,
	                  pstPlayer->stRoleData.iUin, &pstPkg->Head);
		
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OperInfo.WhereDef[0] = 0;
	pstDBReq->OffSet = 0;
	pstDBReq->Limit = 0;

	memset(&stKey, 0, sizeof(stKey));
	stKey.Uin = pstPlayer->stRoleData.iUin;
		
	if(db_send_req(pstPkg, &stKey, sizeof(stKey), TO_MISC_DB) < 0)
	{
		return -1;
	}
	return 0;
	UNUSED(ullRoleID);	//add by paraunused.pl
}

int db_buddy_select_req(MudPlayer *pstPlayer)
{
	TORMPKG *pstPkg = &this_env()->stOrmPkg;
	TORMSELECTREQ *pstDBReq = &pstPkg->Body.SelectReq;
	ROLEDATABUDDY stKey;
	DBExtra stDBExtra;

	if (0 == pstPlayer->iBuddyDirty)
	{
		return 0;
	}
		
	stDBExtra.iCmd = DB_BUDDY_SELECT_RES;
	db_init_torm_head("RoleDataBuddy", &stDBExtra, TORM_SELECT_REQ,
	                  pstPlayer->stRoleData.iUin, &pstPkg->Head);
		
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_WITHINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OperInfo.WhereDef[0] = 0;
	pstDBReq->OffSet = 0;
	pstDBReq->Limit = 0;

	memset(&stKey, 0, sizeof(stKey));
	stKey.RoleID = pstPlayer->stRoleData.ullRoleID;
		
	if(db_send_req(pstPkg, &stKey, sizeof(stKey), TO_ROLE_DB) < 0)
	{
		return -1;
	}
	return 0;
}

static int db_update_shadow_res(MudPlayer *pstPlayer, const TORMPKG *pstDBPkg,
                                const DBExtra *pstExtra)
{
	const TORMUPDATERES *pstRes = &pstDBPkg->Body.UpdateRes;

	UNUSED(pstExtra);

	if(TORM_ERR_IS_ERROR(pstRes->ErrNo))
	{
		tlog_error(
			this_env()->pstLogCat, 0, 0, 
			"Update shadow data to db failed. code = %d, uin = %d, msg = %s.", 
			pstRes->DBMSErrNo,
			pstPlayer->stRoleData.iUin,
			torm_error_string(pstRes->ErrNo));
		return -1;
	}

	pstPlayer->stShadowData.iCacheFlag = CACHE_CLEAN;

	if(MUD_PLAYER_STATUS_ROLE_LOGOUT_REQ == pstPlayer->eStatus)
	{
		return mud_role_logout_res(pstPlayer);
	}
	
	return 0;
}

int db_buddy_update_req(MudPlayer* pstPlayer,ROLEDATABUDDY* pstRoleBuddy)
{
	TORMPKG *pstPkg = &this_env()->stOrmPkg;
	TORMUPDATEREQ *pstDBReq = &pstPkg->Body.UpdateReq;
	DBExtra stDBExtra;

	pstRoleBuddy->RoleID = pstPlayer->stRoleData.ullRoleID;
	
	stDBExtra.iCmd = DB_BUDDY_UPDATE_RES;
	db_init_torm_head("RoleDataBuddy", &stDBExtra, TORM_UPDATE_REQ,
	                  pstPlayer->stRoleData.iUin, &pstPkg->Head);
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OperInfo.WhereDef[0] = 0;
	pstDBReq->DoInsert = 0;

	if(db_send_req(pstPkg, pstRoleBuddy, sizeof(*pstRoleBuddy),TO_ROLE_DB) < 0)
	{
		return -1;
	}
	return 0;
}

int db_update_shadow_req(MudPlayer *pstPlayer, const MudShadowData *pstShadowData)
{
	TORMPKG *pstPkg = &this_env()->stOrmPkg;
	TORMUPDATEREQ *pstDBReq = &pstPkg->Body.UpdateReq;
	DBExtra stDBExtra;

	/*
	if(pstShadowData->iCacheFlag == CACHE_CLEAN)
	{
		tlog_error(this_env()->pstLogCat, pstPlayer->stRoleData.iUin, 0, 
				   "don't update clean ShadowRoleData again, uin = %d",
				   pstPlayer->stRoleData.iUin);
		return -1;
	}
	*/
	
	stDBExtra.iCmd = DB_SHADOW_UPDATE_RES;
	db_init_torm_head("ShadowAccData", &stDBExtra, TORM_UPDATE_REQ,
	                  pstPlayer->stRoleData.iUin, &pstPkg->Head);
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OperInfo.WhereDef[0] = 0;
	pstDBReq->DoInsert = 1;

	if(db_send_req(pstPkg, &pstShadowData->stCore, sizeof(pstShadowData->stCore),
	               TO_MISC_DB) < 0)
	{
		return -1;
	}
	return 0;
}

static int db_send_req(TORMPKG *pstPkg, const void *pvKey, int iKeyLen,
                       int iToWhere)
{
	MUD_SVRENV *pstEnv = this_env();
	TDRDATA stNetData;
	TDRDATA stMetaData;
	int iDstID = 0, iProtoVer = 0;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pvKey;
	stMetaData.iBuff = iKeyLen;

	switch(iToWhere)
	{
	case TO_ROLE_DB:
		iDstID = pstEnv->iRoleDBID;
		iProtoVer = pstEnv->iRoleDBProtoVer;
		break;

	case TO_MISC_DB:
		iDstID = pstEnv->iMiscDBID;
		iProtoVer = pstEnv->iMiscDBProtoVer;
		break;

	default:
		return -1;
		break;
	}

	if(torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData,
	                     pstEnv->iRoleDBProtoVer) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}

	return db_torm_send(pstPkg, iDstID, iProtoVer);
}

static int db_torm_send(TORMPKG *pstPkg, int iDstID, int iProtoVer)
{
	MUD_SVRENV *pstEnv = this_env();
	TDRDATA stData;
	int iSrc = 0, iDst = 0;

	stData.pszBuff = pstEnv->szSendBuffer;
	stData.iBuff = sizeof(pstEnv->szSendBuffer);
	if(0 != torm_pack_msg(pstEnv->hTorm, &stData, pstPkg, iProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_pack_msg error");
		return -1;
	}
	
	iSrc = pstEnv->pstAppCtx->iId;
	iDst = iDstID;
	if(tbus_send(pstEnv->pstAppCtx->iBus, &iSrc, &iDst, stData.pszBuff,
	             stData.iBuff, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail");
		return -1;
	}
	return 0;
}

static void db_init_torm_head(const char *pszMetaName, const DBExtra *pstExtra,
                              int iDBCmd, int iUin, TORMPKGHEAD *pstHead)
{
	MUD_SVRENV *pstEnv = this_env();
	pstHead->HeadComm.InstID = pstEnv->pstAppCtx->iId;
	pstHead->HeadComm.Magic = TORM_MAGIC;
	pstHead->HeadCmd.Cmd = iDBCmd;

	if (pszMetaName)
	{
		STRNCPY(pstHead->HeadCmd.MetaNameVer.MetaName,
		        pszMetaName,
		        sizeof(pstHead->HeadCmd.MetaNameVer.MetaName));
	}
	pstHead->HeadCmd.MetaNameVer.MetaVer=0;
	pstHead->HeadApp.AppCmd = 0;
	pstEnv->iTormSeq++;
	if(pstEnv->iTormSeq <= 0)
	{
		pstEnv->iTormSeq = 1;
	}
	
	pstHead->HeadApp.Seq = pstEnv->iTormSeq;
	pstHead->HeadApp.ObjID = iUin;
	pstHead->AsynCallBackDataLen = 0;

	if(pstExtra)
	{
		memcpy(pstHead->AsynCallBackData, pstExtra, sizeof(pstExtra->iCmd));
		pstHead->AsynCallBackDataLen += sizeof(pstExtra->iCmd);

		//
		// TODO(all) 如有额外数据，在此添加
		//
		switch(pstExtra->iCmd)
		{
		case DB_ROLE_SELECT_RES:
			break;
		case DB_ROLE_LIST_SELECT_RES:
			break;
		default:
			break;
		}
	}
}
