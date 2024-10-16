#include "proto_comm.h"
#include "proto_cs.h"
#include "role_db_meta.h"
#include "tconnapi.h"
#include "mud_svr.h"
#include "mud_clt.h"
#include "mud_db.h"
#include "comm/shtable_shm.h"
#include "libdirty.h"
#include "mud_player.h"
#include "comm_misc.h"
#include "apexproxy/ApexProxyLib.h"
#include "mud_zh_name.h"

//move  >> mud_svr.h
#define MUD_PLAYER_ACTIVE_TIMER 500

// 禁言检查
static int player_check_silence(MudPlayer *pstPlayer);

// 言论审查
static int player_check_illegal_words(MudPlayer *pstPlayer, char *pszContent,
                                      int iType);

// 私聊
static int player_private_chat(MudPlayer *pstPlayer, const CSCHATREQ *pstChat,
                               CSPKG *pstResPkg);

static int player_friend_chat(MudPlayer *pstPlayer, const CSCHATREQ *pstChat,
                               CSPKG *pstResPkg);

// 公会聊天
static int player_clan_chat(MudPlayer *pstPlayer, const CSCHATREQ *pstChatReq,
                            CSPKG *pstResPkg);

// 世界聊天
static int player_world_chat(MudPlayer *pstPlayer, const CSCHATREQ *pstChatReq,
                             CSPKG *pstResPkg);

// 名字到Player Pool Index的映射哈希表用的辅助函数
static int Name2PlayerIdxHashPred(const void *pv1, const void *pv2);
static unsigned int Name2PlayerIdxHashGen(const void* pvData);


// 检查pstOther是否在pstPlayer的黑名单中
static inline int player_check_blacklist(MudPlayer *pstPlayer,
                                         MudPlayer *pstOther)
{
	int i = pstPlayer->stRoleData.iBadGuyNum;
	while(i--)
	{
		if(pstPlayer->stRoleData.astBadGuys[i].RoleID ==
		   pstOther->stRoleData.ullRoleID)
			return -1;
	}
	return 0;
}

// 检查pstOther是否在pstPlayer的好友名单中
static inline int player_check_friendlist(MudPlayer *pstPlayer,const char* pszName)
{
	int i = pstPlayer->stRoleData.iFriendNum;
	while(i--)
	{
		if (strcmp(pstPlayer->stRoleData.astFriends[i].RoleName, pszName) == 0)
			return 1;
	}
	return 0;
}

int L2PHashCmp(const void *pv1, const void *pv2)
{
	const LoginName2MemID *pstLhs = pv1;
	const LoginName2MemID *pstRhs = pv2;

	return strcmp(pstLhs->szLoginName, pstRhs->szLoginName);
}

unsigned int L2PHashCode(const void *pvData)
{
	const LoginName2MemID *pstElement = pvData;
	
	return sht_get_code(pstElement->szLoginName);
}

int UinHashCmp(const void * pv1 , const void *pv2)
{
	MudUinCache *pstUinHashData1 = (MudUinCache *)pv1;
	MudUinCache *pstUinHashData2 = (MudUinCache *)pv2;

	return pstUinHashData1->iUin - pstUinHashData2->iUin;
}

unsigned int UinHashCode(const void* pvData)
{
	MudUinCache *pstUinShtData = (MudUinCache *)pvData;
	
	return (unsigned int)pstUinShtData->iUin;
}

MudPlayer* z_idx_player(int iIdx)
{
	MUD_SVRENV* pstEnv = this_env();
	TMEMBLOCK *pstMemBlock;
	MudPlayer *pstPlayer;

	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIdx);
	if (!pstMemBlock->fValid)
		return NULL;
	
	pstPlayer = (MudPlayer *)pstMemBlock->szData;
	
	return pstPlayer;
}  

LEVELUP *z_get_levelup(int iLevel)
{
	MUD_SVRENV * pstEnv = this_env();
	
	if (iLevel < 1 || iLevel >= MAX_LEVEL ||iLevel >= pstEnv->pstMudObj->iLevelUpDef)
	{
		return NULL;
	}

	return  (LEVELUP *)pstEnv->pstMudObj->sLevelUpDef[iLevel-1];
}

int mud_player_free(MudPlayer *pstPlayer,char* pszReason)
{
	MUD_SVRENV* pstEnv = this_env();
	MudUinCache stUinCache;
	MudUinCache *pstUinCache;
	int iUin = pstPlayer->stRoleData.iUin;

	if (!pszReason)
	{
		pszReason = "unknown";
	}

	tlog_info(pstEnv->pstLogCat, iUin, 0, "uin=%d player free for %s", iUin,pszReason);

	//表示的确登录了角色
	if (pstPlayer->stOnline.tRoleLogin > 0)
	{
		OP_LOG_META stOpLog;
		OPMUDROLELOGOUT * pstRoleLogOut = &stOpLog.OpLog.OpMudRoleLogout;

		stOpLog.OpLogType = OP_LOG_MUD_ROLE_LOGOUT;

		pstRoleLogOut->Uin = pstPlayer->stRoleData.iUin;
		pstRoleLogOut->RoleID = pstPlayer->stRoleData.ullRoleID;
		STRNCPY(pstRoleLogOut->RoleName, pstPlayer->stRoleData.szRoleName, sizeof(pstRoleLogOut->RoleName));
		STRNCPY(pstRoleLogOut->AccountLoginName, pstPlayer->szAccount, CCH(pstRoleLogOut->AccountLoginName));
		STRNCPY(pstRoleLogOut->IP, inet_ntoa(*(struct in_addr *)&pstPlayer->stConnectInfo.stExtraInfo.IPInfo.ip), CCH(pstRoleLogOut->IP));
		pstRoleLogOut->Reason = pstPlayer->iRoleLogoutReason;
		pstRoleLogOut->Duration = pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.tRoleLogin;
		
		m_oplog_send(&stOpLog);
	}	
	


	//if (0 > z_id_clean(pstEnv, pstPlayer->iMemID))
	//{
	//	tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0,
	//             "%d player clean id %d fail", pstPlayer->stRoleData.iUin,
	//             pstPlayer->iMemID);
	//	assert(0);
	//}

	{
		LoginName2MemID stDummy;
		STRNCPY(stDummy.szLoginName, pstPlayer->szAccount,
		        CCH(stDummy.szLoginName));
		if(!sht_remove(pstEnv->pstMudShtL2P, &stDummy, L2PHashCmp, L2PHashCode))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,
			           "can not remove MudShtL2P %s", stDummy.szLoginName);
		}
	}
	
	tmempool_free(pstEnv->pstPlayerPool, pstPlayer->iIdx);
	
	stUinCache.iUin = iUin;
	pstUinCache = sht_find(pstEnv->pstShtUinCache, &stUinCache, UinHashCmp,
	                       UinHashCode);


		
	if (pstUinCache)
	{
		pstUinCache->iOnline = 0;
		return 0;
	}
	else
	{
		tlog_error(pstEnv->pstLogCat, iUin, 0,
		           "mud_player_free %d cache not exist", iUin);
		return -1;
	}

	return 0;
}

MudPlayer* m_loginname_player(const char *pszAccount)
{
	MudPlayer *pstPlayer; 
	LoginName2MemID stKey;
	LoginName2MemID *pstTarget = NULL;
	MUD_SVRENV* pstEnv = this_env();

	STRNCPY(stKey.szLoginName, pszAccount, CCH(stKey.szLoginName));
	pstTarget = sht_find(pstEnv->pstMudShtL2P, &stKey, L2PHashCmp, L2PHashCode);
	if (NULL == pstTarget)
	{
		return NULL;
	}

	pstPlayer = tmempool_get(pstEnv->pstPlayerPool, pstTarget->iID);
	if (NULL == pstPlayer)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "get player by uin error");
		return NULL;
	}
	
	if (0 == strcmp(pstPlayer->szAccount, pszAccount))
	{
		return pstPlayer;
	}
	else
	{
		return NULL;
	}
}

static int mud_washout_uincache()
{
	MUD_SVRENV* pstEnv = this_env();
	int i;
	int iWashoutCount = 0;
	int iMax = pstEnv->pstShtUinCache->iMax;
	MudUinCache *pstUinCache;
	int iValid;

	for (i=0; i<iMax; i++)
	{
		if (pstEnv->iUinCacheWashoutPos >= iMax)
		{
			pstEnv->iUinCacheWashoutPos = 0;
		}
		pstUinCache = sht_pos(pstEnv->pstShtUinCache,
		                      pstEnv->iUinCacheWashoutPos, &iValid);
		pstEnv->iUinCacheWashoutPos++;

		if (pstUinCache && iValid && 0 == pstUinCache->iOnline)
		{
			iWashoutCount++;
			sht_remove_by_pos(pstEnv->pstShtUinCache,
			                  pstEnv->iUinCacheWashoutPos - 1);

			if (iWashoutCount >= MAX_MUD_PLAYER)
			{
				break;
			}
		}
	}

	if (MAX_MUD_PLAYER > iWashoutCount)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_washout_uincache washout %d",
		           iWashoutCount);
	}

	return iWashoutCount;
}

int mud_heartbeat_to_region(int iUin)
{
	MUD_SVRENV* pstEnv = this_env();
	int iSrcID = pstEnv->pstAppCtx->iId;
	int iDstID = pstEnv->iRegionGateID;
	char szBuff[sizeof(SSPKG)];
	TDRDATA stNet;
	SSPKG stPkgReq;
	SSMUDHEARTBEAT *pstSSHeartBeat = &stPkgReq.Body.MudHeartBeat;

	pstSSHeartBeat->Uin = iUin;
	Z_SSHEAD_INIT(&stPkgReq.Head, MUD_HEARTBEAT);
	
	stNet.pszBuff = szBuff;
	stNet.iBuff = sizeof(szBuff);
	if (0 > ss_pack(pstEnv->pstSSHan, &stNet, &stPkgReq, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack_err");
		return -1;
	}
	
	if (0 > tbus_forward(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, stNet.pszBuff, stNet.iBuff, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail");
		return -1;
	}

	return 0;
}

int m_player_leave_chat_room_info(MudPlayer *pstPlayer,unsigned char bRoom)
{
	MUD_SVRENV* pstEnv = this_env();
	MudChatRoom* pstMudChatRoom = pstEnv->pstMudChatRoom;
	CSPKG stResPkg;
	CSMUDCHATROOMLEAVEINFO*pstRes = &stResPkg.Body.MudChatRoomLeaveInfo;
	ChatRoomInfo *pstChatRoom = NULL;
	int i = 0;

	if (bRoom >= MAX_MUD_CHAT_ROOM)
	{
		return -1;
	}

	pstChatRoom = &pstMudChatRoom->astChatRoom[bRoom];

	for(i=0; i<pstChatRoom->iPlayerNum; i++)
	{
		if (pstPlayer->iIdx == pstChatRoom->aiPlayerIdx[i])
		{
			break;
		}		
	}

	pstChatRoom->iPlayerNum--;

	if ( i < pstChatRoom->iPlayerNum)
	{
		pstChatRoom->aiPlayerIdx[i] = pstChatRoom->aiPlayerIdx[pstChatRoom->iPlayerNum];
	}

	pstPlayer->stOnline.bChatroom = 0xFF;

	pstRes->RoleID = pstPlayer->stRoleData.ullRoleID;

	STRNCPY(pstRes->RoleName, pstPlayer->stRoleData.szRoleName, CCH(pstRes->RoleName));
	
	Z_CSHEAD_INIT(&stResPkg.Head, MUD_CHAT_ROOM_LEAVE_INFO);
	mud_room_broadcast(pstChatRoom, &stResPkg);
	return 0;
}

int m_player_logout(MudPlayer* pstPlayer)
{
	pstPlayer->eStatus = MUD_PLAYER_STATUS_ROLE_LOGOUT_REQ;
	
	m_player_leave_chat_room_info(pstPlayer,pstPlayer->stOnline.bChatroom);
	
	
	db_update_shadow_req(pstPlayer, &pstPlayer->stShadowData);
	db_buddy_select_req(pstPlayer);

	tlog_info(
		this_env()->pstLogCat, 0, 0, 
		"role logout begin uin = %d, rolid = %llu,reason = %d.", 
		pstPlayer->stRoleData.iUin,
		pstPlayer->stRoleData.ullRoleID,
		pstPlayer->iRoleLogoutReason);

	return 0;
}

int mud_player_close_connection(TFRAMEHEAD *pstFrameHead)
{
	MUD_SVRENV* pstEnv = this_env();
	MudPlayer *pstPlayer;
	
	if (0 > pstFrameHead->ID)
	{
		return -1;
	}

	pstPlayer = tmempool_get(pstEnv->pstPlayerPool,pstFrameHead->ID);
	if (NULL == pstPlayer)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "player_close_connection id=%d player null", pstFrameHead->ID);
		return -1;
	}

	if (0 == pstPlayer->stOnline.bConnLost)
	{
		pstPlayer->stOnline.bConnLost = 1;

		if (pstPlayer->eStatus == MUD_PLAYER_STATUS_ROLE_LOGIN)
		{
			pstPlayer->iRoleLogoutReason = ROLE_LOGOUT_CLOSE;
			m_player_logout(pstPlayer);
		}
		else
		{
			mud_player_free(pstPlayer," player close conn");
		}
	}
	tlog_debug(pstEnv->pstLogCat, 0, 0, "frame stop reason=%d",
			           pstFrameHead->CmdData.Stop.Reason);

	return 0;
}

MudPlayer *mud_player_get_by_uin(int iUin)
{
	MUD_SVRENV* pstEnv = this_env();
	MudPlayer *pstPlayer;
	MudUinCache stUinHashData;
	MudUinCache *pstUinHashData;
	
	stUinHashData.iUin = iUin;
	pstUinHashData = sht_find(pstEnv->pstShtUinCache, &stUinHashData, UinHashCmp,
	                          UinHashCode);
	if (!pstUinHashData || 0 == pstUinHashData->iOnline)
	{
		return NULL;
	}

	pstPlayer = tmempool_get(pstEnv->pstPlayerPool, pstUinHashData->iIdx);
	if (NULL == pstPlayer)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "get player by uin error");
		return NULL;
	}
	
	if (pstPlayer->stRoleData.iUin == iUin)
	{
		return pstPlayer;
	}
	else
	{
		return NULL;
	}
}

int mud_player_kickoff_text_notify(MudPlayer *pstPlayer,  char *pszMsg)
{	
	CSPKG stPkg;
	CSKICKOFFINFO *pstKickoffInfo;
		
	if (!pszMsg)
		return 0;

	pstKickoffInfo = &stPkg.Body.KickoffInfo;
	STRNCPY(pstKickoffInfo->Msg, pszMsg, sizeof(pstKickoffInfo->Msg));

	Z_CSHEAD_INIT(&stPkg.Head, KICKOFF_INFO);
	return mud_cltmsg_send(pstPlayer, &stPkg);
}

int mud_player_kickoff(MudPlayer *pstPlayer, char *pszMsg)
{
	MUD_SVRENV* pstEnv = this_env();
	
	tlog_info(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0,
	          "kick off player uin=%d", pstPlayer->stRoleData.iUin);

	mud_player_kickoff_text_notify(pstPlayer, pszMsg);

	pstPlayer->iRoleLogoutReason = ROLE_LOGOUT_KICK;

	if (MUD_PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus)
	{
		m_player_logout(pstPlayer);
		return 0;	
	}
	else if (MUD_PLAYER_STATUS_ROLE_LOGOUT_REQ == pstPlayer->eStatus)
	{
		return 0;
	}
	else
	{
		//info tconnd stop
		pstEnv->stFrameHeadTemp.Cmd = TFRAMEHEAD_CMD_STOP;
		pstEnv->stFrameHeadTemp.CmdData.Stop.Reason = STOP_REASON_KICKOFF;
		pstEnv->stFrameHeadTemp.ID = pstPlayer->stConnectInfo.iID;
		pstEnv->stFrameHeadTemp.ConnIdx = pstPlayer->stConnectInfo.iConnIdx;
			
		tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iMudConnID,
		              pstEnv->szSendBuffer, 0, &pstEnv->stFrameHeadTemp);

		//恢复全局变量的初始设置
		pstEnv->stFrameHeadTemp.CmdData.Stop.Reason = 0;

		mud_player_free(pstPlayer,"kickoff");
				
		return 0;
	}
}

int mud_player_disconnect(MudPlayer * pstPlayer)
{
	MUD_SVRENV* pstEnv = this_env();
	
	if (pstPlayer->stOnline.bConnLost)
	{
		return 0;
	}
	
	//info tconnd stop
	pstEnv->stFrameHeadTemp.Cmd = TFRAMEHEAD_CMD_STOP;
	pstEnv->stFrameHeadTemp.CmdData.Stop.Reason = STOP_REASON_KICKOFF;
	pstEnv->stFrameHeadTemp.ID = pstPlayer->stConnectInfo.iID;
	pstEnv->stFrameHeadTemp.ConnIdx = pstPlayer->stConnectInfo.iConnIdx;
		
	tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iMudConnID, 
	              pstEnv->szSendBuffer, 0, &pstEnv->stFrameHeadTemp);

	//恢复全局变量的初始设置
	pstEnv->stFrameHeadTemp.CmdData.Stop.Reason = 0;
	pstPlayer->stOnline.bConnLost = 1;
	pstPlayer->iRoleLogoutReason = ROLE_LOGOUT_CLOSE;

	pstPlayer->stRoleData.tLastLogout = pstEnv->pstAppCtx->stCurr.tv_sec;
	return 0;
}

int mud_player_acc_login_err(const TFRAMEHEAD *pstFrameHead, short nResult)
{
	MUD_SVRENV* pstEnv = this_env();
	CSPKG stPkg;
	CSPKG *pstCSRes = &stPkg;
	CSACCLOGINRES *pstAccLoginRes = &pstCSRes->Body.AccLoginRes;
	TFRAMEHEAD stFrameHead;
	TDRDATA stNet;

	tlog_debug(pstEnv->pstLogCat, 0, 0, "payer login err result=%d", nResult);

	Z_CSHEAD_INIT(&pstCSRes->Head, ACC_LOGIN_RES);
	pstAccLoginRes->Result = nResult;
	pstAccLoginRes->SvrProtoVer = TDR_METALIB_PROTO_CS_VERSION;
	
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = &pstEnv->szSendBuffer[0];
	if (0 > net_cs_pack(pstEnv->pstNetHan, &stNet, pstCSRes, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "net cs pack error");
		return -1;
	}
	
	memset(&stFrameHead, 0, sizeof(stFrameHead));
	stFrameHead.Cmd = TFRAMEHEAD_CMD_STOP;
	stFrameHead.CmdData.Stop.Reason = STOP_REASON_LIMIT;
	stFrameHead.ConnIdx = pstFrameHead->ConnIdx;
	stFrameHead.ID = -1;
	stFrameHead.ExtraType = pstFrameHead->ExtraType;
	memcpy(&stFrameHead.ExtraInfo, &pstFrameHead->ExtraInfo,
	       sizeof(stFrameHead.ExtraInfo));

	return tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iMudConnID,
	                     stNet.pszBuff, stNet.iBuff, &stFrameHead);
}

MudPlayer *mud_player_alloc(int iUin, const char *pszAccount,
                            MudUinCache *pstUinCache)
{
	MUD_SVRENV* pstEnv = this_env();
	TAPPCTX* pstAppCtx = pstEnv->pstAppCtx;
	
	MudUinCache stUinHashData;
	MudUinCache *pstUinHashData;
	MudPlayer *pstPlayer;
	int iIdx;

	if (NULL == pstUinCache)
	{
		if (sht_is_full(pstEnv->pstShtUinCache))
		{
			if (0 >= mud_washout_uincache())
			{
				return NULL;
			}
		}

		memset(&stUinHashData, 0, sizeof(stUinHashData));
		stUinHashData.iUin = iUin;
		stUinHashData.iIdx = -1;
		stUinHashData.iOnline = 1;
		stUinHashData.tLastLogin = pstAppCtx->stCurr.tv_sec;
		pstUinHashData = sht_insert_unique(pstEnv->pstShtUinCache,
		                                   &stUinHashData, UinHashCmp,
		                                   UinHashCode);
		if (!pstUinHashData)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_insert_unique fail");
			return NULL;
		}
		memcpy(pstUinHashData, &stUinHashData, sizeof(*pstUinHashData));
	}
	else
	{
		pstUinCache->tLastLogin = pstAppCtx->stCurr.tv_sec;
		pstUinCache->iOnline = 1;
		pstUinHashData = pstUinCache;
	}

	iIdx = tmempool_alloc(pstEnv->pstPlayerPool);
	if (0 > iIdx)
	{
		sht_remove(pstEnv->pstShtUinCache,&stUinHashData,UinHashCmp,UinHashCode);

		tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_alloc fail");
		return NULL;
	}

	pstPlayer = tmempool_get(pstEnv->pstPlayerPool, iIdx);
	memset(pstPlayer, 0, sizeof(*pstPlayer));

	STRNCPY(pstPlayer->szAccount, pszAccount, sizeof(pstPlayer->szAccount));
	pstPlayer->iMagic = MUD_PLAYER_SHM_MAGIC;
	//pstPlayer->iMemID = z_get_mem_id(pstAppCtx, pstEnv, iIdx, OBJ_PLAYER);
	pstPlayer->iIdx = iIdx;
	pstPlayer->iUinCachePos = (long int)((char *)pstUinHashData + offsetof(SHITEM, szData) - 
							   (char *)pstEnv->pstShtUinCache -
							   pstEnv->pstShtUinCache->iDataOff) /
							   pstEnv->pstShtUinCache->iDataUnitMax;
	pstUinHashData->iIdx = iIdx;

	tlog_debug(pstEnv->pstLogCat, iUin, 0, "account(%s) uin=%d mem idx=%d alloc cachepos=%d get PoolIdx id %d", pszAccount, iUin, iIdx, pstPlayer->iUinCachePos, pstPlayer->iIdx);

	{
		LoginName2MemID stDummy, *pstInserted = NULL;
		stDummy.iID = pstPlayer->iIdx;
		STRNCPY(stDummy.szLoginName, pstPlayer->szAccount,
		        CCH(stDummy.szLoginName));

		pstInserted = sht_insert_unique(pstEnv->pstMudShtL2P, &stDummy, L2PHashCmp,
		                                L2PHashCode);
		if(pstInserted)
		{
			memcpy(pstInserted, &stDummy, sizeof(stDummy));
		}
		else
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,
			           "sht insert account %s,player fail",
			           pstPlayer->szAccount);
		}
	}

	return pstPlayer;
}

int role_online_to_zone(MudPlayer *pstPlayer, int iOnlineType)
{
	SSPKG stSSPkg;
	SSROLEONLINE *pstRoleOnlie = &stSSPkg.Body.RoleOnline;
	
	pstRoleOnlie->RoleID = pstPlayer->stRoleData.ullRoleID;
	pstRoleOnlie->Online = iOnlineType;
	STRNCPY(pstRoleOnlie->RoleName, pstPlayer->stRoleData.szRoleName, sizeof(pstRoleOnlie->RoleName));
	pstRoleOnlie->AccUin = pstPlayer->stRoleData.iUin;
	
	Z_SSHEAD_INIT(&stSSPkg.Head, ROLE_ONLINE);
	mud_ssmsg_send(&stSSPkg, this_env()->iZoneSvrID);

	return 0;
}

int mud_role_online(MudPlayer *pstPlayer, int iOnlineType)
{
	CSPKG stPkg;
	CSONLINESVR *pstOnline = &stPkg.Body.OnlineInfo;
		
	//上线通知
	pstOnline->RoleID = pstPlayer->stRoleData.ullRoleID;
	pstOnline->Online = iOnlineType;
	STRNCPY(pstOnline->RoleName, pstPlayer->stRoleData.szRoleName, sizeof(pstOnline->RoleName));
	Z_CSHEAD_INIT(&stPkg.Head, ONLINE_INFO);
	mud_cltmsg_broadcast_except(&stPkg, pstPlayer);

	//to zone
	m_pkg_to_zone(&stPkg,NULL, PKG_TRANS_ALL,0);
	
	return 0;
}

int mud_relation_online_info(MudPlayer *pstPlayer)
{
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONCHGLIST *pstRelationChgList = &pstRelationSvr->RelationSvrData.RelationChgList;
	CSRELATIONCHG *pstRelationChg;
	SSPKG stSSPkg;
	SSRELATIONONLINEREQ *pstRelationReq = &stSSPkg.Body.RelationOnlineReq;
	MudRoleData *pstRoleData = &pstPlayer->stRoleData;
	MudPlayer *pstOther;
	int i;
	int iIsOnline;
	
	//通知一部分个人关系信息
	pstRelationSvr->OpType = RELATION_OP_TYPE_CHGLIST;
	pstRelationChgList->Num = 0;

	//不在线的去zone_svr找一下
	pstRelationReq->BuddyNum = 0;
	STRNCPY(pstRelationReq->RoleName, pstPlayer->stRoleData.szRoleName, 
					sizeof(pstRelationReq->RoleName));

	for (i=0; i<pstRoleData->iBadGuyNum; i++)
	{
		pstRelationChg = &pstRelationChgList->ChgList[pstRelationChgList->Num++];
		pstRelationChg->RoleID = pstRoleData->astBadGuys[i].RoleID;
		pstRelationChg->RelationType = RELATION_TYPE_BLACK;
		pstRelationChg->RelationAttrNum = 0;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_NAME;
		STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Name, pstRoleData->astBadGuys[i].RoleName, 
					sizeof(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Name));
		pstRelationChg->RelationAttrNum++;

		pstOther = mud_name_player(pstRoleData->astBadGuys[i].RoleName);
		if (pstOther)
			iIsOnline = ROLE_MUD_ONLINE;
		else
		{
			iIsOnline = ROLE_OFFLINE;

			pstRelationReq->BuddyInfo[pstRelationReq->BuddyNum].RoleID = pstRoleData->astBadGuys[i].RoleID;
			STRNCPY(pstRelationReq->BuddyInfo[pstRelationReq->BuddyNum].RoleName, 
						pstRoleData->astBadGuys[i].RoleName, sizeof(pstRelationReq->BuddyInfo[0].RoleName));
			pstRelationReq->BuddyInfo[pstRelationReq->BuddyNum].RelationType = RELATION_TYPE_BLACK;
			
			pstRelationReq->BuddyNum++;
		}
			
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_ONLINE;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.IsOnline = iIsOnline;
		pstRelationChg->RelationAttrNum++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_HEAD;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= (pstRoleData->astBadGuys[i].Head<<16);
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= (pstRoleData->astBadGuys[i].Hair<<8);
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= pstRoleData->astBadGuys[i].Face;
		pstRelationChg->RelationAttrNum++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_CAREER;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.IsOnline =  pstRoleData->astBadGuys[i].Career;
		pstRelationChg->RelationAttrNum++;
		
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_GENDER;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.IsOnline =  pstRoleData->astBadGuys[i].Gender;
		pstRelationChg->RelationAttrNum++;
	}

	for (i=0; i<pstRoleData->iFriendNum; i++)
	{
		pstRelationChg = &pstRelationChgList->ChgList[pstRelationChgList->Num++];
		pstRelationChg->RoleID = pstRoleData->astFriends[i].RoleID;
		pstRelationChg->RelationType = pstRoleData->astFriends[i].RelationType;
		
		pstRelationChg->RelationAttrNum = 0;
		
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_NAME;
		STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Name, 
					pstRoleData->astFriends[i].RoleName, sizeof(pstRoleData->astFriends[i].RoleName));
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_BUDDYVAL;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.BuddyVal = pstRoleData->astFriends[i].BuddyVal;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_GENDER;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Gender = pstRoleData->astFriends[i].Gender;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_MEMNAME;
		STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.MemName, 
					pstRoleData->astFriends[i].MemName, sizeof(pstRoleData->astFriends[i].MemName));
		pstRelationChg->RelationAttrNum ++;

		pstOther = mud_name_player(pstRoleData->astFriends[i].RoleName);
		if (pstOther)
		{
			iIsOnline = ROLE_MUD_ONLINE;

			pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_HEAD;
			pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= (pstOther->stRoleData.iHead<<16);
			pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= (pstOther->stRoleData.iHair<<8);
			pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= pstOther->stRoleData.iFace;
			pstRelationChg->RelationAttrNum ++;

			pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_CAREER;
			pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Career = pstOther->stRoleData.iCareer;
			pstRelationChg->RelationAttrNum ++;

			pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_LEVEL;
			pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Level = pstOther->stRoleData.iLevel;
			pstRelationChg->RelationAttrNum ++;

			pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_EMOTION;
			STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Emotion,
				pstOther->stRoleData.Emotion, sizeof(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Emotion));
			pstRelationChg->RelationAttrNum ++;

		}
		else
		{
			iIsOnline = ROLE_OFFLINE;
			
			pstRelationReq->BuddyInfo[pstRelationReq->BuddyNum].RoleID = pstRoleData->astFriends[i].RoleID;
			STRNCPY(pstRelationReq->BuddyInfo[pstRelationReq->BuddyNum].RoleName, 
						pstRoleData->astFriends[i].RoleName, sizeof(pstRelationReq->BuddyInfo[0].RoleName));
			pstRelationReq->BuddyInfo[pstRelationReq->BuddyNum].RelationType = pstRoleData->astFriends[i].RelationType;
			
			pstRelationReq->BuddyNum++;

		
		}	

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_ONLINE;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.IsOnline = iIsOnline;
		pstRelationChg->RelationAttrNum ++;

		

	}
	
	for (i = 0; i < pstRelationChgList->Num; i ++)
	{
		pstRelationChg = &pstRelationChgList->ChgList[i];
		if (pstRelationChg->RelationAttrNum > RELATION_ATTR_MAX)
		{
			pstRelationChg->RelationAttrNum = RELATION_ATTR_MAX;
		}
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	mud_cltmsg_send(pstPlayer, &stPkg);

	if(pstRelationReq->BuddyNum)
	{
		Z_SSHEAD_INIT(&stSSPkg.Head, RELATION_ONLINE_REQ);
		mud_ssmsg_send(&stSSPkg, this_env()->iZoneSvrID);
	}
	
	return 0;
}

int mud_relation_role_logout(MudPlayer *pstPlayer)
{
	CSPKG stPkg;
	CSONLINESVR *pstOnline = &stPkg.Body.OnlineInfo;
	
	//下线通知
	pstOnline->RoleID = pstPlayer->stRoleData.ullRoleID;
	pstOnline->Online = 0;
	STRNCPY(pstOnline->RoleName, pstPlayer->stRoleData.szRoleName, sizeof(pstOnline->RoleName));
	Z_CSHEAD_INIT(&stPkg.Head, ONLINE_INFO);
	mud_cltmsg_broadcast(&stPkg);

	//to zone
	m_pkg_to_zone(&stPkg, NULL,PKG_TRANS_ALL,0);
	
	return 0;
}

int chatroom_info_to_clt(MudPlayer *pstPlayer)
{	
	MUD_SVRENV* pstEnv = this_env();
	CSPKG *pstResPkg = &pstEnv->stCSRes;
	CSCHATROOMINFO*pstRes = &pstResPkg->Body.MudEnterChatRoomRes.ChatRoomInfo;
	MudPlayer *pstTmpPlayer;
	MudChatRoom* pstMudChatRoom = pstEnv->pstMudChatRoom;
	ChatRoomInfo *pstChatRoomInfo;
	int i;

	if(pstPlayer->eStatus != MUD_PLAYER_STATUS_ROLE_LOGIN || 
		pstPlayer->stOnline.bChatroom >= MAX_MUD_CHAT_ROOM)
	{
		return -1;
	}

	pstChatRoomInfo = &pstMudChatRoom->astChatRoom[pstPlayer->stOnline.bChatroom];
	pstRes->ChatRoomIdx = pstPlayer->stOnline.bChatroom + 1;
	
	pstRes->PlayerNum = 0;
	for(i=0; i<pstChatRoomInfo->iPlayerNum; i++)
	{
		pstTmpPlayer = z_idx_player(pstChatRoomInfo->aiPlayerIdx[i]);
		if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != MUD_PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}

		pstRes->ChatRoomRoleInfo[pstRes->PlayerNum].RoleID = pstPlayer->stRoleData.ullRoleID;
		pstRes->ChatRoomRoleInfo[pstRes->PlayerNum].Career = pstPlayer->stRoleData.iCareer;
		pstRes->ChatRoomRoleInfo[pstRes->PlayerNum].Level = pstPlayer->stRoleData.iLevel;
		pstRes->ChatRoomRoleInfo[pstRes->PlayerNum].Gender = pstPlayer->stRoleData.iGender;
		STRNCPY(pstRes->ChatRoomRoleInfo[pstRes->PlayerNum].RoleName, pstPlayer->stRoleData.szRoleName, 
					CCH(pstRes->ChatRoomRoleInfo[pstRes->PlayerNum].RoleName));
		
		pstRes->PlayerNum++;
	}

	Z_CSHEAD_INIT(&pstResPkg->Head, MUD_ENTER_CHAT_ROOM_RES);
	
	return mud_cltmsg_send(pstPlayer, pstResPkg);
}



int mud_acc_login_req(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                      TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	TAPPCTX* pstAppCtx = pstEnv->pstAppCtx;
	char szMsg[DESC_LEN];
	CSACCLOGINREQ *pstAccLogin = &pstCsPkg->Body.AccLoginReq;
	int iUin= pstAccLogin->Uin;
	MudPlayer *pstPlayerNew;
	MudUinCache *pstUinCache;
	MudUinCache stUinHashData;
	CSPKG *pstCSRes = &pstEnv->stCSRes;
	CSACCLOGINRES *pstAccLoginRes;
	TFRAMECMDSTART *pstStart;

	UNUSED(pstPlayer);

	if (pstFrameHead->Cmd != TFRAMEHEAD_CMD_START)
	{
		return -1;
	}

	pstStart = &pstFrameHead->CmdData.Start;

	if (0 == pstEnv->pstConf->NotUinAuth)
	{
		if(pstStart->AuthType != AUTH_JC ||
		   pstStart->AuthData.AuthJC.Uin != pstAccLogin->Uin)
		{
			return -1;
		}
	}

	//server关闭过程中禁止登录
	if (pstEnv->cShutdown)
	{
		mud_player_acc_login_err(pstFrameHead, LOGIN_NOSERVICE);
		return -1;
	}
	
	if ( (0!= pstEnv->pstConf->MaxCltProtoVer && pstAccLogin->CltProtoVer > pstEnv->pstConf->MaxCltProtoVer) ||
		 (0!= pstEnv->pstConf->MinCltProtoVer && pstAccLogin->CltProtoVer < pstEnv->pstConf->MinCltProtoVer))
	{	
		tlog_debug(pstEnv->pstLogCat, 0, 0, "client ip=%s port=%d proto ver %d",
					inet_ntoa(*(struct in_addr *)&pstFrameHead->ExtraInfo.IPInfo.ip), ntohs(pstFrameHead->ExtraInfo.IPInfo.port),
					pstAccLogin->CltProtoVer);
		mud_player_acc_login_err(pstFrameHead, LOGIN_BADCLT);
		return -1;
	}

	
	if (black_white_check(pstEnv->pstBlackWhiteH, pstFrameHead->ExtraInfo.IPInfo.ip))
	{
		mud_player_acc_login_err(pstFrameHead, LOGIN_NOSERVICE);
		return -1;
	}

	tlog_info(pstEnv->pstLogCat, iUin, 0, "client uin=%d ip=%s port=%d ConnIdx=%d acc login",iUin, 
				inet_ntoa(*(struct in_addr *)&pstFrameHead->ExtraInfo.IPInfo.ip), 
				ntohs(pstFrameHead->ExtraInfo.IPInfo.port), pstFrameHead->ConnIdx);
	
	stUinHashData.iUin = iUin;
	pstUinCache = (MudUinCache *)sht_find(pstEnv->pstShtUinCache,&stUinHashData,UinHashCmp,UinHashCode);
	if (pstUinCache && (pstUinCache->tLastLogin <= pstAppCtx->stCurr.tv_sec && pstUinCache->tLastLogin + pstEnv->pstConf->LoginInterval >= pstAppCtx->stCurr.tv_sec))
	{
		mud_player_acc_login_err(pstFrameHead, LOGIN_FREQUENCY);

		return -1;
	}

	
	pstPlayerNew = mud_player_get_by_uin(iUin);
	if( pstPlayerNew != NULL )
	{
		sprintf(szMsg, LJY_MUD34, 
			inet_ntoa(*(struct in_addr *)&pstFrameHead->ExtraInfo.IPInfo.ip));
		mud_player_kickoff(pstPlayerNew, szMsg);
		mud_player_disconnect(pstPlayerNew);
					
		if (0 == (pstUinCache && pstUinCache->cGetRoleList))
		{
			mud_player_acc_login_err(pstFrameHead, LOGIN_SVRBUSY);

			
		}

		if(pstFrameHead->ExtraInfo.IPInfo.ip == pstPlayerNew->stConnectInfo.stExtraInfo.IPInfo.ip)
		{
			mud_player_acc_login_err( pstFrameHead, LOGIN_AGAIN_SAMEIP);	
		}
		else
		{
			mud_player_acc_login_err(pstFrameHead, LOGIN_REP);
		}
		return -1;
	}
	else
	{
		if (pstEnv->pstPlayerPool->iUsed >= MAX_MUD_PLAYER_SOFT && (0 == pstStart->AuthData.AuthJC.UinFlag))
		{
			mud_player_acc_login_err(pstFrameHead, LOGIN_SVRBUSY);
			return -1;
		}
		if (pstUinCache)
		{
			pstPlayerNew = mud_player_alloc(iUin, pstAccLogin->AccountLoginName, pstUinCache);
		}
		else
		{
			pstPlayerNew = mud_player_alloc(iUin, pstAccLogin->AccountLoginName, NULL);
		}
		if (NULL == pstPlayerNew)
		{
			mud_player_acc_login_err(pstFrameHead, LOGIN_SVRBUSY);

			tlog_error(pstEnv->pstLogCat, iUin , 0, "recv Uin=%d acc login, but player_alloc NULL",
						iUin);
			return -1;
		}

		pstPlayerNew->stRoleData.iUin = pstAccLogin->Uin;
	}

	// 挂机禁言应该也生效吧
	pstPlayerNew->iAccSilenceEnd = pstStart->AuthData.AuthJC.AccSilenceEnd;
	
	pstPlayerNew->unCltProVer = pstAccLogin->CltProtoVer;	
	
	pstPlayerNew->eStatus = MUD_PLAYER_STATUS_ACC_LOGIN;
	pstPlayerNew->tActive = pstAppCtx->stCurr.tv_sec;
	pstPlayerNew->tLastTickSec = pstAppCtx->stCurr.tv_sec;

	
	pstPlayerNew->stConnectInfo.chVer = pstFrameHead->Ver;
	pstPlayerNew->stConnectInfo.chCmd = TFRAMEHEAD_CMD_START;
	pstPlayerNew->stConnectInfo.chExtraType = pstFrameHead->ExtraType;
	memcpy(&pstPlayerNew->stConnectInfo.stExtraInfo, &pstFrameHead->ExtraInfo, sizeof(pstPlayerNew->stConnectInfo.stExtraInfo));
	pstPlayerNew->stConnectInfo.chTimeStampType = pstFrameHead->TimeStampType;
	memcpy(&pstPlayerNew->stConnectInfo.stTimeStamp, &pstFrameHead->TimeStamp, sizeof(pstPlayerNew->stConnectInfo.stTimeStamp));
	pstPlayerNew->stConnectInfo.iConnIdx = pstFrameHead->ConnIdx;
	pstPlayerNew->stConnectInfo.iID = pstPlayerNew->iIdx;
	pstPlayerNew->stOnline.bConnLost = 0;

	STRNCPY(pstPlayerNew->aMacAdd,pstAccLogin->MacAdd,CCH(pstPlayerNew->aMacAdd));
	
	Z_CSHEAD_INIT(&pstCSRes->Head, ACC_LOGIN_RES);
	pstAccLoginRes = &pstCSRes->Body.AccLoginRes;
	pstAccLoginRes->Result = LOGIN_SUCC;
	pstAccLoginRes->SvrProtoVer = TDR_METALIB_PROTO_CS_VERSION;

	if(mud_cltmsg_send(pstPlayerNew, pstCSRes) < 0)
	{
		mud_player_acc_login_err(pstFrameHead, LOGIN_SVRBUSY);

		tlog_error(pstEnv->pstLogCat, iUin , 0, "recv Uin=%d acc login, but z_cltmsg_send fail", iUin);

		mud_player_free(pstPlayerNew,"send failed");
		return -1;
	}
	
	pstPlayerNew->stConnectInfo.chCmd = TFRAMEHEAD_CMD_INPROC;

	mud_heartbeat_to_region(pstPlayerNew->stRoleData.iUin);
	
	{
		OP_LOG_META stOpLog;
		OPMUDACCLOGIN *pstAccLogin = &stOpLog.OpLog.OpMudAccLogin;
		
		stOpLog.OpLogType = OP_LOG_MUD_ACC_LOGIN;
		pstAccLogin->Uin = iUin;
		STRNCPY(pstAccLogin->AccountLoginName, pstPlayerNew->szAccount, sizeof(pstAccLogin->AccountLoginName));
		pstAccLogin->LogTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		m_oplog_send(&stOpLog);
	}
	
	if (pstUinCache && pstUinCache->cGetRoleList)
	{
		pstPlayerNew->eStatus = MUD_PLAYER_STATUS_ROLE_LIST;
	
		//return z_send_rolelist(pstEnv, pstPlayerNew, pstUinCache);
		return mud_send_role_list(pstPlayerNew, pstUinCache);
	}
	else
	{
		//send role list req
		//return z_db_role_list(pstAppCtx, pstEnv, pstPlayerNew);
		return db_select_role_list_req(pstPlayerNew, iUin);
	}

	return 0;
}

int mud_acc_logout_req(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                       TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{

	CSPKG *pstCSPkgRes = &pstEnv->stCSRes;
	CSACCLOGOUTRES *pstAccLogoutRes = &pstCSPkgRes->Body.AccLogoutRes;
	
	if (NULL == pstPlayer )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "player null");
		return -1;
	}

	pstPlayer->iRoleLogoutReason = ROLE_LOGOUT_ACC;
	
	switch(pstPlayer->eStatus)
	{
		case MUD_PLAYER_STATUS_ROLE_LOGIN:
			m_player_logout(pstPlayer);
			break;

		case MUD_PLAYER_STATUS_ROLE_LOGOUT_REQ:
			break;

		default:
			pstAccLogoutRes->Uin = pstPlayer->stRoleData.iUin;

			pstPlayer->eStatus = MUD_PLAYER_STATUS_ACC_LOGOUT;
					
			pstAccLogoutRes->Result = LOGOUT_SUCC;
			Z_CSHEAD_INIT(&pstCSPkgRes->Head, ACC_LOGOUT_RES);
			mud_cltmsg_send_stop(pstPlayer, pstCSPkgRes);
			
			mud_player_free(pstPlayer,"logout");
			break;
	}
	
	return 0;
	UNUSED(pstFrameHead);
	UNUSED(pstCsPkg);
}

int mud_role_login_req(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                       TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	const CSMUDROLELOGINREQ *pstReq = &pstCsPkg->Body.MudRoleLoginReq;
	MudUinCache *pstUinCache;
	MudUinCache stUinHashData;
	int i = 0;
	int iFound = 0;

	if(!pstPlayer)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "mud player null");
		return -1;
	}
	
	if(pstPlayer->eStatus != MUD_PLAYER_STATUS_ROLE_LIST)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "mud player miss status : MUD_PLAYER_STATUS_ROLE_LIST");
		return -1;
	}

	stUinHashData.iUin = pstPlayer->stRoleData.iUin;
	pstUinCache = (MudUinCache *)sht_find(pstEnv->pstShtUinCache,&stUinHashData,UinHashCmp,UinHashCode);
	if (!pstUinCache)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "uin cache null");
		return -1;
	}

	for (i=0; i<pstUinCache->ucRoleNum; i++)
	{
		if (pstUinCache->astRoleListInfo[i].RoleID == pstCsPkg->Body.RoleLoginReq.RoleID)
		{
			iFound = 1;
			
		}
		pstPlayer->iAccMaxLevel = max(pstPlayer->iAccMaxLevel,pstUinCache->astRoleListInfo[i].Level);
	}

	if (0 == iFound)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "login roleid invalid,uin=%d,roleid=%lld",
						pstPlayer->stRoleData.iUin,
						pstCsPkg->Body.RoleLoginReq.RoleID);
		return -1;
	}
	
	memset(&pstPlayer->stOnline, 0, sizeof(pstPlayer->stOnline));
	
	memcpy(&pstPlayer->stOnline.stConnectTime,
	       &pstFrameHead->TimeStamp.TimeVal, 
	       sizeof(pstPlayer->stOnline.stConnectTime));

	if(db_select_role_req(pstPlayer, pstReq->RoleID) < 0)
	{
		return -1;
	}
	
	tlog_info(
		pstEnv->pstLogCat, 0, 0, 
		"role login begin uin = %d, rolid = %lld", 
		pstPlayer->stRoleData.iUin,
		pstReq->RoleID);

	return 0;
}

int m_player_enter_chat_room_info(MudPlayer *pstPlayer,unsigned char bRoom)
{
	MUD_SVRENV* pstEnv = this_env();
	MudChatRoom* pstMudChatRoom = pstEnv->pstMudChatRoom;
	CSPKG stResPkg;
	CSMUDCHATROOMENTERINFO *pstRes = &stResPkg.Body.MudChatRoomEnterInfo;
	ChatRoomInfo *pstChatRoom = NULL;


	if (bRoom >= MAX_MUD_CHAT_ROOM)
	{
		return -1;
	}

	pstChatRoom = &pstMudChatRoom->astChatRoom[bRoom];

	pstRes->RoleInfo.RoleID = pstPlayer->stRoleData.ullRoleID;
	pstRes->RoleInfo.Career = pstPlayer->stRoleData.iCareer;
	pstRes->RoleInfo.Level = pstPlayer->stRoleData.iLevel;
	pstRes->RoleInfo.Gender = pstPlayer->stRoleData.iGender;
	STRNCPY(pstRes->RoleInfo.RoleName, pstPlayer->stRoleData.szRoleName, CCH(pstRes->RoleInfo.RoleName));
	
	Z_CSHEAD_INIT(&stResPkg.Head, MUD_CHAT_ROOM_ENTER_INFO);
	mud_room_broadcast(pstChatRoom, &stResPkg);
	return 0;
}



int m_player_enter_chat_room(MudPlayer *pstPlayer,unsigned char bRoom)
{
	MUD_SVRENV* pstEnv = this_env();
	CSPKG stResPkg;
	CSMUDENTERCHATROOMRES *pstRes = &stResPkg.Body.MudEnterChatRoomRes;
	CSCHATROOMINFO *pstCsChatRoomInfo = &pstRes->ChatRoomInfo;
	MudPlayer *pstTmpPlayer = NULL;
	MudChatRoom* pstMudChatRoom = pstEnv->pstMudChatRoom;
	ChatRoomInfo *pstChatRoom = NULL;
	int i = 0;

	if (bRoom >= MAX_MUD_CHAT_ROOM)
	{
		return -1;
	}

	pstPlayer->stOnline.bChatroom = bRoom;
	pstChatRoom = &pstMudChatRoom->astChatRoom[pstPlayer->stOnline.bChatroom];
	pstCsChatRoomInfo->ChatRoomIdx = pstPlayer->stOnline.bChatroom+1;
	pstCsChatRoomInfo->PlayerNum = 0;

	if (pstChatRoom->iPlayerNum >= MAX_CHAT_ROOM_PLAYER)
	{
		return -1;
	}
	
	for(i=0; i<pstChatRoom->iPlayerNum; i++)
	{
		pstTmpPlayer = z_idx_player(pstChatRoom->aiPlayerIdx[i]);
		if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != MUD_PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}

		pstCsChatRoomInfo->ChatRoomRoleInfo[pstCsChatRoomInfo->PlayerNum].RoleID = pstTmpPlayer->stRoleData.ullRoleID;
		pstCsChatRoomInfo->ChatRoomRoleInfo[pstCsChatRoomInfo->PlayerNum].Career = pstTmpPlayer->stRoleData.iCareer;
		pstCsChatRoomInfo->ChatRoomRoleInfo[pstCsChatRoomInfo->PlayerNum].Level = pstTmpPlayer->stRoleData.iLevel;
		pstCsChatRoomInfo->ChatRoomRoleInfo[pstCsChatRoomInfo->PlayerNum].Gender = pstTmpPlayer->stRoleData.iGender;
		STRNCPY(pstCsChatRoomInfo->ChatRoomRoleInfo[pstCsChatRoomInfo->PlayerNum].RoleName, pstTmpPlayer->stRoleData.szRoleName, 
					CCH(pstCsChatRoomInfo->ChatRoomRoleInfo[pstCsChatRoomInfo->PlayerNum].RoleName));
		
		pstCsChatRoomInfo->PlayerNum++;
	}
	m_player_enter_chat_room_info(pstPlayer,bRoom);

	pstChatRoom->aiPlayerIdx[pstChatRoom->iPlayerNum] = pstPlayer->iIdx;
	pstChatRoom->iPlayerNum++;
	pstRes->Result = 0;

	Z_CSHEAD_INIT(&stResPkg.Head, MUD_ENTER_CHAT_ROOM_RES);
	mud_cltmsg_send(pstPlayer, &stResPkg);
	return 0;
}

BYTE m_player_get_default_chat_room()
{
	MUD_SVRENV* pstEnv = this_env();
	MudChatRoom* pstMudChatRoom = pstEnv->pstMudChatRoom;
	ChatRoomInfo *pstChatRoom = NULL;
	int iMax = 0;
	BYTE bRoom = 0xFF;
	BYTE bMaxRoom = 0xFF;
	int i = 0;


	for(i = MAX_MUD_CHAT_ROOM - 1; i >= 0; i--)
	{
		pstChatRoom = &pstMudChatRoom->astChatRoom[i];
		if (pstChatRoom->iPlayerNum >= MAX_CHAT_ROOM_PLAYER)
		{
			continue;
		}
		bRoom = (BYTE)i;
		if (iMax < pstChatRoom->iPlayerNum)
		{
			iMax = pstChatRoom->iPlayerNum;
			bMaxRoom = (BYTE)i;
		}		
	}

	if (bMaxRoom != 0xFF)
	{
		bRoom = bMaxRoom;
	}

	return bRoom;
}

OFFLINEEXPPARAMETER* m_get_offline_exp_parameter(int iType)
{
	
	MUD_SVRENV * pstEnv = this_env();
	
	if (iType < 1 || iType > pstEnv->pstMudObj->iOfflineExpParameter)
	{
		return NULL;
	}

	return  (OFFLINEEXPPARAMETER* )pstEnv->pstMudObj->sOfflineExpParameter[iType-1];
}

int m_clan_snapshot(MudPlayer* pstPlayer)
{
	CSPKG stPkg;
	CSCLANMEMBERS *pstMems = &stPkg.Body.ClanSvr.ClanSvrData.ClanMemberSnapshot;
	int i = 0;
	const SListEntry *astEntries = this_env()->pstPlayerIdxEntries->astEntries;
	const ClanMemberCache *pstFound = clan_mem_cached_find(&pstPlayer->stRoleData.stClanInfo);	
	SSCMDDATA stData;

	memset(pstMems,0,sizeof(*pstMems));
	
	pstMems->ID = pstPlayer->stRoleData.stClanInfo;
	pstMems->MemNum = 0;
	
	
	if(!pstFound)
	{
		return -1;
	}

	for(i = pstFound->iMemberEntry; i != -1; i = astEntries[i].iNext)
	{
		MudPlayer *pstMember = astEntries[i].iValue < 0 ?
		   NULL : tmempool_get(this_env()->pstPlayerPool, astEntries[i].iValue);

		if (!pstMember)
		{
			continue;
		}
		if (pstMems->MemNum >= CLAN_MAX_MEM)
		{
			break;
		}

		pstMems->Members[pstMems->MemNum ].Career = pstMember->stRoleData.iCareer;
		pstMems->Members[pstMems->MemNum ].Level = pstMember->stRoleData.iLevel;
		pstMems->Members[pstMems->MemNum ].Online = ROLE_MUD_ONLINE;
		pstMems->Members[pstMems->MemNum ].RoleMember.RoleID = pstMember->stRoleData.ullRoleID;
		STRNCPY(pstMems->Members[pstMems->MemNum ].RoleMember.RoleName,pstMember->stRoleData.szRoleName,
			CCH(pstMems->Members[pstMems->MemNum ].RoleMember.RoleName));

		pstMems->MemNum++;
	}

	stPkg.Body.ClanSvr.Operation = CLAN_SVR_MEMBER_SNAPSHOT;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	mud_cltmsg_send(pstPlayer,&stPkg);
	//让zone发公会成员
	stData.ClanID = pstPlayer->stRoleData.stClanInfo;
	m_cmd_to_zone(pstPlayer->stRoleData.szRoleName,NULL, SS_CMD_CLAN_ONLINE_MEMBER, &stData);
	return 0;
}

int get_player_clan_mem(MudPlayer* pstPlayer,CLANMEM *pstMem)
{
	memset(pstMem,0,sizeof(*pstMem));
	pstMem->Career = pstPlayer->stRoleData.iCareer;
	pstMem->Level = pstPlayer->stRoleData.iLevel;
	pstMem->Online = ROLE_MUD_ONLINE;
	pstMem->RoleMember.RoleID = pstPlayer->stRoleData.ullRoleID;
	STRNCPY(pstMem->RoleMember.RoleName,
		pstPlayer->stRoleData.szRoleName,
		CCH(pstMem->RoleMember.RoleName));
	
	return 0;
}

int clan_mem_online_to_zone(MudPlayer* pstPlayer)
{
	CSPKG stPkg;
	CSCLANMEMBERCHG *pstMemChg = &stPkg.Body.ClanSvr.ClanSvrData.ClanMemberChg;

	pstMemChg->Code = CLAN_MEMBER_CHG_MOD;
	get_player_clan_mem(pstPlayer,&pstMemChg->MemInfo);
	
	stPkg.Body.ClanSvr.Operation = CLAN_SVR_MEMBER_CHG;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	m_pkg_to_zone(&stPkg, NULL,PKG_TRANS_CLAN,pstPlayer->stRoleData.stClanInfo.Id);
	return 0;
}

int mud_role_login_res(MudPlayer *pstPlayer)
{
	MUD_SVRENV* pstEnv = this_env();
	CSPKG stResPkg;
	CSMUDROLELOGINRES *pstRes = &stResPkg.Body.MudRoleLoginRes;
	LEVELUP *pstLevel = NULL;
	CSPKG stEnterResPkg;
	CSMUDENTERCHATROOMRES *pstEnterRes = &stEnterResPkg.Body.MudEnterChatRoomRes;

	Name2PlayerIdx stNameMemID, *pstNameMemID;
	OFFLINEEXPPARAMETER* pstPara = NULL;
	int iSendFull = 0;
	int iValidSec = 0;

	memset(pstRes, 0, sizeof(*pstRes));
	memset(pstEnterRes, 0, sizeof(*pstEnterRes));
	
	pstPlayer->eStatus = MUD_PLAYER_STATUS_ROLE_LOGIN;
	
	stNameMemID.iIdx = pstPlayer->iIdx;
	STRNCPY(stNameMemID.szName, pstPlayer->stRoleData.szRoleName, sizeof(stNameMemID.szName));
	pstNameMemID = sht_insert_unique(pstEnv->pstShtName2PlayerIdx, &stNameMemID, 
										Name2PlayerIdxHashPred, Name2PlayerIdxHashGen);
	if (pstNameMemID)
	{
		memcpy(pstNameMemID, &stNameMemID, sizeof(*pstNameMemID));
	}
	else
	{

		pstNameMemID = sht_find(pstEnv->pstShtName2PlayerIdx, &stNameMemID, 
										Name2PlayerIdxHashPred, Name2PlayerIdxHashGen);
		if (pstNameMemID)
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0, 
				"sht insert player fail name = %s, uin = %d idx = %d, find name = %s idx = %d",
				stNameMemID.szName,
				pstPlayer->stRoleData.iUin,
				pstPlayer->iIdx,
				pstNameMemID->szName,
				pstNameMemID->iIdx);
		}
		else
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0, 
				"sht insert player fail name = %s, uin = %d, find = %d",
				stNameMemID.szName,
				pstPlayer->stRoleData.iUin,
				pstNameMemID);
		}
	}
	
	

	pstRes->RoleDetail.RoleID = pstPlayer->stRoleData.ullRoleID;
	pstRes->RoleDetail.Gender = pstPlayer->stRoleData.iGender;
	pstRes->RoleDetail.Career = pstPlayer->stRoleData.iCareer;
	pstRes->RoleDetail.Level = pstPlayer->stRoleData.iLevel;
	pstRes->RoleDetail.Head = pstPlayer->stRoleData.iHead;
	pstRes->RoleDetail.Face = pstPlayer->stRoleData.iFace;
	pstRes->RoleDetail.Hair = pstPlayer->stRoleData.iHair;
	
	pstRes->RoleDetail.Wear = pstPlayer->stRoleData.stWear;
	
	pstPlayer->stOnline.bChatroom = 0xFF;
	pstPlayer->iMaxOfflineExp = 0;
	pstPlayer->iZoneOfflineExp = 0;
	pstPlayer->stOnline.tRoleLogin = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	pstPlayer->iInvalidTime = MUD_OFFLINE_EXP_SEC;
	pstLevel = z_get_levelup(pstPlayer->iAccMaxLevel);
	if (pstLevel)
	{
		pstPlayer->iMaxOfflineExp = pstLevel->MaxOfflineExp;
		pstPlayer->iContributeExp = pstLevel->ContributeExp;
	}
	else
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0, 
			"LevelUPDef get fail uin = %d,level = %d",
			pstPlayer->stRoleData.iUin,
			pstPlayer->iAccMaxLevel);
	}

	if (pstPlayer->stAccShare.LastLogout == 0)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0, 
			"AccShare uin = %d,LastLogout = %d",
			pstPlayer->stRoleData.iUin,
			pstPlayer->stAccShare.LastLogout);
	}
	//计算iZoneOfflineExp
	pstPlayer->iZoneOfflineExp = pstPlayer->stRoleData.iZoneRemainExp + 
								(pstPlayer->stShadowData.stCore.MudExp - pstPlayer->stRoleData.iTakeMudExp);
	if ( pstPlayer->stAccShare.LastLogout > 0)
	{

		pstPara = m_get_offline_exp_parameter(OFFLINE_EXP_ZONE);
		iValidSec = pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stAccShare.LastLogout - ZONE_OFFLINE_EXP_SEC;
		
		if (pstPara && pstPara->Parameter1 != 0 && iValidSec > 0)
		{
			pstPlayer->iZoneOfflineExp += (int)pstPlayer->iContributeExp * ( iValidSec) / pstPara->Parameter1;
		}
		if (pstPlayer->iZoneOfflineExp > pstPlayer->iMaxOfflineExp)
		{
			pstPlayer->iZoneOfflineExp = pstPlayer->iMaxOfflineExp;
		}
	}

	
	
	if (pstEnv->pstAppCtx->stCurr.tv_sec < pstPlayer->stAccShare.LastLogout)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0, 
			"last logout time err,uin = %d,logout = %d, now = %d",
			pstPlayer->stRoleData.iUin,
			pstPlayer->stAccShare.LastLogout,
			pstEnv->pstAppCtx->stCurr.tv_sec );
	}
	
	if (pstPlayer->iZoneOfflineExp + pstPlayer->iOnlineMudExp >= pstPlayer->iMaxOfflineExp)
	{
		pstPlayer->iOnlineMudExp = pstPlayer->iMaxOfflineExp - pstPlayer->iZoneOfflineExp;
		iSendFull = 1;
	}
	
	pstRes->MaxOfflineExp = pstPlayer->iMaxOfflineExp;	
	pstRes->OfflineExp = pstPlayer->iZoneOfflineExp + pstPlayer->iOnlineMudExp;	

	Z_CSHEAD_INIT(&stResPkg.Head, MUD_ROLE_LOGIN_RES);
	mud_cltmsg_send(pstPlayer, &stResPkg);

	//好友上线 通知
	mud_role_online(pstPlayer, ROLE_MUD_ONLINE);

	//通知zone_svr
	//role_online_to_zone(pstPlayer, ROLE_MUD_ONLINE);

	mud_relation_online_info(pstPlayer);

	// 如果是公会成员,要入缓存
	if(pstPlayer->stRoleData.stClanInfo.GID != 0ULL)
	{
		clan_mem_cached_add(pstPlayer);
		// 通知上线
		clan_mem_online_to_zone(pstPlayer);
	}

	/*
	bChatRoom = m_player_get_default_chat_room();
	if (bChatRoom < MAX_MUD_CHAT_ROOM)
	{
		m_player_enter_chat_room(pstPlayer,bChatRoom);	
	}
	else
	{
		mud_sys_strf(pstPlayer, SYS_TIP, "所有房间已满");
		pstEnterRes->Result = 2;

		Z_CSHEAD_INIT(&stResPkg.Head, MUD_ENTER_CHAT_ROOM_RES);
		mud_cltmsg_send(pstPlayer, &stResPkg);
	}
	*/
	
	//发送公会成员列表
	m_clan_snapshot(pstPlayer);

	//心情
	m_send_emotion(pstPlayer);

	if (iSendFull)
	{
		m_player_offline_exp_full(pstPlayer, iSendFull);
	}

	tlog_info(
		pstEnv->pstLogCat, 0, 0, 
		"role login done uin = %d, rolid = %llu", 
		pstPlayer->stRoleData.iUin,
		pstPlayer->stRoleData.ullRoleID);

	{
		OP_LOG_META stOpLog;
		OPMUDROLELOGIN *pstRoleLogin = &stOpLog.OpLog.OpMudRoleLogin;

		stOpLog.OpLogType = OP_LOG_MUD_ROLE_LOGIN;

		pstRoleLogin->Uin = pstPlayer->stRoleData.iUin;
		pstRoleLogin->RoleID = pstPlayer->stRoleData.ullRoleID;
		STRNCPY(pstRoleLogin->RoleName, pstPlayer->stRoleData.szRoleName, CCH(pstRoleLogin->RoleName));
		STRNCPY(pstRoleLogin->AccountLoginName, pstPlayer->szAccount, CCH(pstRoleLogin->AccountLoginName));
		STRNCPY(pstRoleLogin->MacAddr, pstPlayer->aMacAdd, CCH(pstRoleLogin->MacAddr));
		STRNCPY(pstRoleLogin->IP, inet_ntoa(*(struct in_addr *)&pstPlayer->stConnectInfo.stExtraInfo.IPInfo.ip), CCH(pstRoleLogin->IP));
		pstRoleLogin->LogTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		
		m_oplog_send(&stOpLog);
	}	

	{
		SSCMDDATA stData;
		
		stData.AccUin = pstPlayer->stRoleData.iUin;
		m_cmd_to_zone(NULL, NULL,SS_CMD_ACC_NOTICE, &stData);
	}

	return 0;
}

int mud_role_logout_req(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                      TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	UNUSED(pstEnv);
	UNUSED(pstFrameHead);
	UNUSED(pstCsPkg);
	
	pstPlayer->iRoleLogoutReason = ROLE_LOGOUT_CLTREQ;
	m_player_logout(pstPlayer);

	return 0;
}

int mud_role_logout_res(MudPlayer *pstPlayer)
{
	MUD_SVRENV* pstEnv = this_env();
	Name2PlayerIdx stNameMemID;
	MudUinCache *pstUinCache;
	int iValid;
	CSPKG *pstCSPkgRes = &pstEnv->stCSRes;
	CSACCLOGOUTRES *pstAccLogoutRes = &pstCSPkgRes->Body.AccLogoutRes;

	m_player_leave_chat_room_info(pstPlayer,pstPlayer->stOnline.bChatroom);

	//通知好友下线
	mud_role_online(pstPlayer, ROLE_OFFLINE);

	//通知zone_svr
	//role_online_to_zone(pstPlayer, ROLE_OFFLINE);

	// 如果是公会成员,则清除缓存
	if(pstPlayer->stRoleData.stClanInfo.GID != 0ULL)
	{
		clan_mem_cached_del(pstPlayer);
	}

	STRNCPY(stNameMemID.szName, pstPlayer->stRoleData.szRoleName, sizeof(stNameMemID.szName));
	if (NULL == sht_remove(pstEnv->pstShtName2PlayerIdx, &stNameMemID, 
							Name2PlayerIdxHashPred, Name2PlayerIdxHashGen))
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0, 
			"sht_remove %d player fail, status = %d, reason = %d",
			pstPlayer->stRoleData.iUin,
			pstPlayer->eStatus,
			pstPlayer->iRoleLogoutReason );
		assert(0);
	}

	tlog_info(
		pstEnv->pstLogCat, 0, 0, 
		"role logout done uin = %d, rolid = %llu,reason = %d, status = %d", 
		pstPlayer->stRoleData.iUin,
		pstPlayer->stRoleData.ullRoleID,
		pstPlayer->iRoleLogoutReason,
		pstPlayer->eStatus);

	
	if (ROLE_LOGOUT_CLTREQ != pstPlayer->iRoleLogoutReason)
	{
		OP_LOG_META stOpLog;
		OPMUDACCLOGOUT *pstAccLogout = &stOpLog.OpLog.OpMudAccLogout;
		
		stOpLog.OpLogType = OP_LOG_MUD_ACC_LOGOUT;
		
		pstAccLogout->Uin = pstPlayer->stRoleData.iUin;
		STRNCPY(pstAccLogout->AccountLoginName, pstPlayer->szAccount, sizeof(pstAccLogout->AccountLoginName));
		pstAccLogout->LogTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		m_oplog_send(&stOpLog);
	}

	if (ROLE_LOGOUT_KICK == pstPlayer->iRoleLogoutReason)
	{
		//info tconnd stop

		if (0 == pstPlayer->stOnline.bConnLost)
		{
			pstEnv->stFrameHeadTemp.Cmd = TFRAMEHEAD_CMD_STOP;
			pstEnv->stFrameHeadTemp.CmdData.Stop.Reason = STOP_REASON_KICKOFF;
			pstEnv->stFrameHeadTemp.ID = pstPlayer->stConnectInfo.iID;
			pstEnv->stFrameHeadTemp.ConnIdx = pstPlayer->stConnectInfo.iConnIdx;
				
			tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iMudConnID, pstEnv->szSendBuffer, 0, &pstEnv->stFrameHeadTemp);

			//恢复全局变量的初始设置
			pstEnv->stFrameHeadTemp.CmdData.Stop.Reason = 0;
		}

		mud_player_free(pstPlayer,"logout kick");
	}
	else if (ROLE_LOGOUT_ACC == pstPlayer->iRoleLogoutReason)
	{
		pstPlayer->eStatus = MUD_PLAYER_STATUS_ACC_LOGOUT;

		pstAccLogoutRes->Uin = pstPlayer->stRoleData.iUin;
		pstAccLogoutRes->Result = LOGOUT_SUCC;
		Z_CSHEAD_INIT(&pstCSPkgRes->Head, ACC_LOGOUT_RES);
		
		mud_cltmsg_send_stop(pstPlayer, pstCSPkgRes);

		mud_player_free(pstPlayer,"logout acc");

	}
	else if (ROLE_LOGOUT_CLOSE == pstPlayer->iRoleLogoutReason)
	{
		mud_player_free(pstPlayer,"logout close");
	}
	else if (ROLE_LOGOUT_CLTREQ == pstPlayer->iRoleLogoutReason)
	{
		pstUinCache = (MudUinCache *)sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos, &iValid);
		if (NULL == pstUinCache || 0 == iValid || pstUinCache->iUin != pstPlayer->stRoleData.iUin)
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0, 
						"z_db_role_logout_res Uin=%d cache",  pstPlayer->stRoleData.iUin);
			return -1;
		}
		
		pstPlayer->eStatus = MUD_PLAYER_STATUS_ROLE_LIST;
		pstPlayer->iRoleLogoutReason = 0;
		
		mud_send_role_list(pstPlayer, pstUinCache);
	}
	else
	{
		;
	}



	
	return 0;	
}

int mud_gm(MudPlayer* pstPlayer,char* pszConent)
{
	char* pcBegin = NULL;
	char* pcGM = NULL;
	int iLen = 0;
	int iExp = 0;
	CSPKG stPkg;
	CSCHATREQ stChatReq;
	
	pcBegin = pszConent + 2;
	pcGM = "exp";
	iLen = strlen(pcGM);
	if ( 0 == strncmp(pcBegin,pcGM,iLen ))
	{
		pcBegin += iLen;
		iExp = atoi(pcBegin);
		m_player_offline_exp_change(pstPlayer,iExp,0);
		return 0;
	}
	
	pcGM = "clanchat";
	iLen = strlen(pcGM);
	if ( 0 == strncmp(pcBegin,pcGM,iLen ))
	{
		pcBegin += iLen + 1;
		memset(&stPkg.Body.ChatReq,0,sizeof(stPkg.Body.ChatReq));
		stPkg.Body.ChatReq.OnlineType = ROLE_MUD_ONLINE;
		stPkg.Head.Cmd = CHAT_REQ;
		stPkg.Body.ChatReq.Type = CHAT_CLAN;
		STRNCPY(stPkg.Body.ChatReq.Content,pcBegin,CCH(stPkg.Body.ChatReq.Content));

		memset(&stChatReq,0,sizeof(stChatReq));
		stChatReq.OnlineType = ROLE_MUD_ONLINE;
		stChatReq.Type = CHAT_CLAN;
		STRNCPY(stChatReq.Content,pcBegin,CCH(stPkg.Body.ChatReq.Content));

		player_clan_chat(pstPlayer, &stChatReq, &stPkg);
		return 0;
	}
	
	pcGM = "cleartimes";
	iLen = strlen(pcGM);
	if ( 0 == strncmp(pcBegin,pcGM,iLen ))
	{
		pstPlayer->stShadowData.stCore.DayTimes = 0;
	}
	
	pcGM = "shutdown";
	iLen = strlen(pcGM);
	if (0 == strncmp(pcBegin,pcGM,iLen ))
	{
		int iTime = 0;
		pcBegin += iLen + 1;
		iTime = atoi(pcBegin);
		
		if( iTime < 1 )
		{
			iTime = 30;
		}
		
		this_env()->cShutdown = 1;
		this_env()->cClearShm = 1;
		this_env()->ucShutdownCount = iTime/MUD_SHUTDOWN_NOTIFY_TIMER;
		this_env()->tShutdownStart = this_env()->pstAppCtx->stCurr.tv_sec;
		tos_send_signal( this_env()->pstAppCtx->pszPidFile, this_env()->pstAppCtx->pszApp, SIGUSR1, NULL );
	}
	return 0;
}

int chat_mud_room(MudPlayer* pstPlayer,CSPKG *pstResPkg)
{

	
	if ( 0 == strncmp(pstResPkg->Body.ChatRes.Content,"//",2))	
	{
		if ( 0 != pstPlayer->stRoleData.iPerm || this_env()->iOpenGM)
		{
			mud_gm(pstPlayer,pstResPkg->Body.ChatRes.Content);
		}
		else
		{
			if (!this_env()->iGetZoneGm)
			{
				SSINFOGETREQ stReq;
				stReq.ReqBody.ChatRes = pstResPkg->Body.ChatRes;
				m_info_get_to_zone(&stReq, INFO_GET_GM);
				return 0;
			}
		}
		
	}
	Z_CSHEAD_INIT(&pstResPkg->Head, CHAT_RES);
	mud_room_broadcast(fast_get_room(pstPlayer), pstResPkg);

	return 0;
}

int mud_chat_req(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                 TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSPKG *pstResPkg = &pstEnv->stCSRes;
	CSCHATRES *pstChatRes = &pstResPkg->Body.ChatRes;
	CSCHATREQ *pstChat = &pstCsPkg->Body.ChatReq;
	CSPKG stEcho;
	int iSec = 0;
	int iResult = -1;

	UNUSED(pstFrameHead);

	// 禁言检查
	if(player_check_silence(pstPlayer) < 0)
	{
		return -1;
	}

	// 反动言论过滤
	if(player_check_illegal_words(pstPlayer, pstChat->Content, pstChat->Type) < 0)
	{
		return -1;
	}
	//xx后的回响
	stEcho.Body.ChatReq = *pstChat;
	Z_CSHEAD_INIT(&stEcho.Head, CHAT_REQ);
	

	pstChatRes->Type = pstChat->Type;
	pstChatRes->LinkLen = pstChat->LinkLen;
	STRNCPY(pstChatRes->From, pstPlayer->stRoleData.szRoleName,
	        sizeof(pstChatRes->From));
	STRNCPY(pstChatRes->Content, pstChat->Content, sizeof(pstChatRes->Content));
	if (pstChatRes->LinkLen > 0)
	{
		memcpy(pstChatRes->LinkContent, pstChat->LinkContent, pstChat->LinkLen);
	}
	pstChatRes->OnlineType = ROLE_MUD_ONLINE;

	if (m_is_player_chat_channel_silence(pstPlayer,pstChat->Type))
	{
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD19);
		return -1;
	}

	if (ROLE_GAME_ONLINE == pstChat->OnlineType)
	{
		mud_chat_to_zone(pstPlayer,pstChat);
		return 0;
	}

	switch(pstChat->Type)
	{
	case CHAT_MUD_ROOM:
		iResult = chat_mud_room(pstPlayer,pstResPkg);
		break;
		
	case CHAT_PRIVATE:
		iResult = player_private_chat(pstPlayer, pstChat, pstResPkg);
		break;
		
	case CHAT_CLAN:
		iResult = player_clan_chat(pstPlayer, pstChat, pstResPkg);
		break;

	case CHAT_WORLD:
		iResult = player_world_chat(pstPlayer, pstChat, pstResPkg);
		break;

	case CHAT_FRIEND:
		iResult = player_friend_chat(pstPlayer, pstChat, pstResPkg);
		break;

	case CHAT_ITEM: // 不能吹喇叭
	case CHAT_TEAM: // 没有组队频道
		return -1;
		break;

	default:
		return -1;
		break;
	}

	// mud先这样写着      估计这东西用不着
	iSec = CHAT_COMM_COOL/1000;
	if (CHAT_COMM_COOL%1000 > 0)
		iSec ++;
	
	pstPlayer->stOnline.tChatCommCoolEnd = CURR + iSec;

	if(pstChat->Type == CHAT_WORLD)
	{
		pstPlayer->stOnline.tChatWorldCoolEnd = CURR + CHAT_WORLD_COOL/1000;
	}
	if ( 0 == iResult)
	{
		mud_cltmsg_send(pstPlayer, &stEcho);
	}

	return 0;
}

LPSHTABLE mud_attach_uincache()
{
	MUD_SVRENV* pstEnv = this_env();
	LPSHTABLE pstShtTable;
	int iShmID;
	 
	pstShtTable = sht_create_shm(MUD_UIN_CACHE*3, MUD_UIN_CACHE,
	                             sizeof(MudUinCache),
	                             pstEnv->pstConf->UinCacheShmKey);
	iShmID = shmget(pstEnv->pstConf->UinCacheShmKey, 0, 0666 );
	if (0 > iShmID)
	{
		return NULL;
	}

	pstEnv->iUinCacheShmID = iShmID;

	return pstShtTable;
}

int mud_attach_player()
{
	MUD_SVRENV* pstEnv = this_env();
	int iShmID;
	int iPoolSize;
	int iShmSize;
	MudPlayerShm *pvAddr;
	int iIsExist=0;
	int iRet;
	int iOffSet;
	int iPlayerShmKey = pstEnv->pstConf->PlayerShmKey;

	iOffSet = offsetof(MudPlayerShm, sData);
	iPoolSize = TMEMPOOL_CALC(MAX_MUD_PLAYER, MUD_PLAYER_DATA);
	iShmSize	=	iPoolSize + iOffSet;
	iShmID	=	shmget(iPlayerShmKey, iShmSize, 0666 | IPC_CREAT | IPC_EXCL);

	if( iShmID<0 )
	{
		iIsExist = 1;
		iShmID = shmget(iPlayerShmKey, iShmSize, 0666);
	}

	if( iShmID<0 )
		return -1;

	if (pstEnv->pstPlayerShm)
	{
		shmdt(pstEnv->pstPlayerShm);
	}

	pvAddr = (MudPlayerShm *)shmat(iShmID, NULL, 0);
	
	if( (void *) -1  == pvAddr )
		return -1;

	pstEnv->pstPlayerShm = pvAddr;
	pstEnv->iPlayerShmID = iShmID;

	if( iIsExist )
	{
		if (MUD_PLAYER_SHM_VER != pvAddr->iShmVersion)
		{
			printf("player shm ver=%d, but now shm ver=%d\n",
			       pvAddr->iShmVersion, MUD_PLAYER_SHM_VER);
			shmdt(pvAddr);
			return -1;
		}
		
		iRet = tmempool_attach(&pstEnv->pstPlayerPool,
		                       MAX_MUD_PLAYER, MUD_PLAYER_DATA, pvAddr->sData,
		                       iPoolSize);
	}
	else
	{
		pvAddr->iShmVersion = MUD_PLAYER_SHM_VER;
		iRet = tmempool_init(&pstEnv->pstPlayerPool, MAX_MUD_PLAYER,
		                     MUD_PLAYER_DATA, pvAddr->sData, iPoolSize);
	}

	if(iRet < 0)
	{
		shmdt(pvAddr);
	}

	return iRet;
	
}



int mud_active_player_sec(MudPlayer *pstPlayer)
{
	MUD_SVRENV* pstEnv = this_env();
	int iCount = ++pstPlayer->stOnline.iSecCount;
	OFFLINEEXPPARAMETER* pstPara = NULL;
	int iZone = 0;
	int iMud = 0;
	int iSec = 60;
	SHADOWACCDATA *pstShadowData = NULL;

	if(0 == iCount % 60)
	{
		iSec = pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->tLastTickSec;
		iSec = min(iSec,60);
		if (pstPlayer->iInvalidTime > 0)
		{
			pstPlayer->iInvalidTime -= iSec;
			pstPlayer->iInvalidTime = max(0,pstPlayer->iInvalidTime);
		}
		if (pstPlayer->iInvalidTime > 0)
		{
			return -1;
		}
		pstPlayer->tLastTickSec = pstEnv->pstAppCtx->stCurr.tv_sec;
		if (iSec < 1)
		{

			tlog_error(pstEnv->pstLogCat,pstPlayer->stRoleData.iUin,0,
				"time error iSec = %d",iSec);
			return -1;
		}
		
		pstShadowData = &pstPlayer->stShadowData.stCore;

		if (!m_is_player_offline_exp_full(pstPlayer))
		{
			pstPara = m_get_offline_exp_parameter(OFFLINE_EXP_ZONE);
			if (pstPara && pstPara->Parameter1 != 0 && iSec > 0 )
			{
				//公式:M/参数1,小于1则为1
				iZone = (longlong)pstPlayer->iContributeExp * iSec / pstPara->Parameter1;
				if( iZone < 1)
				{
					iZone = 1;
				}
				
				iZone = min(iZone,pstPlayer->iMaxOfflineExp - pstPlayer->iOnlineMudExp - pstPlayer->iZoneOfflineExp);
			}

			pstPara = m_get_offline_exp_parameter(OFFLINE_EXP_MUD);
			if (pstPara && pstPara->Parameter1 != 0)
			{
				iMud = (longlong)pstPlayer->iContributeExp * iSec / pstPara->Parameter1;
				if (iMud < 1)
				{
					iMud = 1;
				}
			}
		}

		if (iZone + iMud > 0)
		{
			m_player_offline_exp_change(pstPlayer,iMud,iZone);
		}

	}
	
	return 0;
}

int mud_active_player(int iCount)
{
	MUD_SVRENV* pstEnv = this_env();
	TAPPCTX* pstAppCtx = pstEnv->pstAppCtx;
	int i, iMs;
	MudPlayer *pstPlayer;
	TMEMBLOCK *pstMemBlock;
	struct timeval stSub;
	static struct timeval stRoundTv;
	static struct timeval stRoundStartTv;
	static unsigned int iActivePos = 0, iRound = 0;

	if (0 == stRoundTv.tv_sec)
	{
		iMs = MUD_PLAYER_ACTIVE_TIMER;
	}
	else
	{
		TV_DIFF(stSub, pstAppCtx->stCurr, stRoundTv);
		TV_TO_MS(iMs, stSub);
	}
		
	if (iMs < MUD_PLAYER_ACTIVE_TIMER)
	{
		return 0;
	}

	if (0 == iActivePos)
	{
		TV_CLONE(stRoundStartTv, pstAppCtx->stCurr);
	}

	for (i = 0; i < iCount; i++) 
	{
		if (iActivePos >= MAX_MUD_PLAYER) 
		{
			break;
		}
		
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, iActivePos);
		iActivePos++;

		if (!pstMemBlock->fValid)
		{
			continue;
		}
		
		pstPlayer = (MudPlayer *)pstMemBlock->szData;

		assert(pstPlayer->iMagic == MUD_PLAYER_SHM_MAGIC);

		// 跟新shadow数据
		if(pstPlayer->eStatus == MUD_PLAYER_STATUS_ROLE_LOGIN &&
		   pstPlayer->stShadowData.iCacheFlag == CACHE_DIRTY)
		{
			db_update_shadow_req(pstPlayer, &pstPlayer->stShadowData);
		}

		//更新buddy
		if (pstPlayer->iBuddyDirty)
		{
			db_buddy_select_req(pstPlayer);
		}

		//每秒
		if (iActivePos % 2 == iRound % 2) 
		{
			if (MUD_PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus)
			{
				mud_active_player_sec(pstPlayer);
			}	
		}

		//每5分钟
		if (iActivePos % 600 == iRound % 600) 
		{
			mud_heartbeat_to_region(pstPlayer->stRoleData.iUin);
		}
				
		if (pstPlayer->tActive + pstEnv->pstConf->CheckIdle < CURR)
		{
			tlog_error(
				pstEnv->pstLogCat, 0, 0,
				"player %d Active=%d, CheckIdle=%d, Curr=%d",
				pstPlayer->stRoleData.iUin,
				pstPlayer->tActive,
				pstEnv->pstConf->CheckIdle,
				pstAppCtx->stCurr.tv_sec);
			mud_player_kickoff(pstPlayer, LJY_MUD35);
		}
	}
	if (iActivePos < MAX_MUD_PLAYER)
	{
		return 0;
	}
	
	iActivePos = 0;
	iRound++;
	TV_CLONE(stRoundTv, pstAppCtx->stCurr);

	TV_DIFF(stSub, pstAppCtx->stCurr, stRoundStartTv);
	TV_TO_MS(iMs, stSub);
	if (iMs > MUD_PLAYER_ACTIVE_TIMER)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "active player loop %d ms", iMs);
	}
	
	return 0;
}

// 禁言检查
static int player_check_silence(MudPlayer *pstPlayer)
{
	if(pstPlayer->iAccSilenceEnd < 0)
	{
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD20);
		goto keep_silence;
	}

	if(pstPlayer->iAccSilenceEnd > CURR)
	{
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD21, 
			         pstPlayer->iAccSilenceEnd - CURR);
		goto keep_silence;
	}

	if(pstPlayer->stRoleData.tSilenceEnd < 0)
	{
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD22);
		goto keep_silence;
	}
	
	if(pstPlayer->stRoleData.tSilenceEnd > CURR)
	{
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD23, 
			         pstPlayer->stRoleData.tSilenceEnd - CURR);
		goto keep_silence;
	}

	return 0;

keep_silence:
	return -1;
}

// 言论审查
static int player_check_illegal_words(MudPlayer *pstPlayer, char *pszContent,
                                      int iType)
{
	if(iType == CHAT_WORLD)
	{
		if(pstPlayer->stOnline.tChatWorldCoolEnd > CURR)
		{
			mud_sys_strf(pstPlayer, SYS_TIP,
			             LJY_MUD24,
			             CHAT_WORLD_COOL/1000);
			return -1;
		}

		if(pstPlayer->stRoleData.iLevel < CHAT_WORLD_LEVEL)
		{
			mud_sys_strf(pstPlayer, SYS_TIP,
			             LJY_MUD25,
			             CHAT_WORLD_LEVEL);
			return -1;
		}
	}
	else
	{
		if (pstPlayer->stOnline.tChatCommCoolEnd > CURR)
		{
			mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD26);
			return -1;
		}
	}

	if(DirtyReplace(pszContent) > 0 )
	{
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD27);
	}

	return 0;
}

int mud_chat_to_zone(MudPlayer *pstPlayer, const CSCHATREQ *pstChat)
{
	SSPKG stPkg;
	SSCHAT *pstSSChat = &stPkg.Body.ChatInfo;

	if(pstChat->Type == CHAT_FRIEND)
	{
		pstSSChat->ChatType = SS_CHAT_FRIEND;
		STRNCPY(pstSSChat->ToData.ToName, pstChat->ToName, 
			CCH(pstSSChat->ToData.ToName));
	}
	else if (pstChat->Type == CHAT_CLAN)
	{
		pstSSChat->ChatType = SS_CHAT_CLAN;
		pstSSChat->ToData.ClanInfo = pstPlayer->stRoleData.stClanInfo;
	}
	else if (pstChat->Type == CHAT_PRIVATE)
	{
		pstSSChat->ChatType = SS_CHAT_PRIVATE;
		STRNCPY(pstSSChat->ToData.ToName, pstChat->ToName, 
			CCH(pstSSChat->ToData.ToName));
	}
	else
	{
		tlog_debug(this_env()->pstLogCat,0, 0, "chat to mud type=%d invalid", pstChat->Type);
		return -1;
	}

	pstSSChat->LinkLen = pstChat->LinkLen;
	STRNCPY(pstSSChat->From, pstPlayer->stRoleData.szRoleName,
	        sizeof(pstSSChat->From));
	STRNCPY(pstSSChat->Content, pstChat->Content, sizeof(pstSSChat->Content));
	if (pstSSChat->LinkLen > 0)
	{
		memcpy(pstSSChat->LinkContent, pstChat->LinkContent, pstChat->LinkLen);
	}

	Z_SSHEAD_INIT(&stPkg.Head, SS_CHAT);
	mud_ssmsg_send(&stPkg, this_env()->iZoneSvrID);

	return 0;
}

// 私聊
static int player_private_chat(MudPlayer *pstPlayer, const CSCHATREQ *pstChat,
                               CSPKG *pstResPkg)
{
	MudPlayer *pstReceiver = NULL;
	pstReceiver = mud_name_player(pstChat->ToName);
	if(!pstReceiver ||
	   pstReceiver->eStatus != MUD_PLAYER_STATUS_ROLE_LOGIN)
	{
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD28, pstChat->ToName);
		return -1;
	}

	if(player_check_blacklist(pstReceiver, pstPlayer) < 0)
	{
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD29, pstChat->ToName);
		return -1;
	}

	if (m_is_player_chat_channel_silence(pstReceiver, CHAT_PRIVATE))
	{
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD30, pstChat->ToName);
		return -1;
	}

	Z_CSHEAD_INIT(&pstResPkg->Head, CHAT_RES);
	return mud_cltmsg_send(pstReceiver, pstResPkg);
}

// 公会聊天
static int player_clan_chat(MudPlayer *pstPlayer, const CSCHATREQ *pstChatReq,
                            CSPKG *pstResPkg)
{
	if(pstPlayer->stRoleData.stClanInfo.GID == 0 ||
	   pstPlayer->stRoleData.stClanInfo.Id == 0 ||
	   pstPlayer->stRoleData.stClanInfo.Name[0] == 0 )
	{
		// 不是公会成员，不许公会聊天
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD31);
		return -1;
	}

	// 广播给Mud的公会成员
	Z_CSHEAD_INIT(&pstResPkg->Head, CHAT_RES);
	mud_cltmsg_broadcast_clan(pstResPkg, &pstPlayer->stRoleData.stClanInfo);

	// 转发给Zone的公会成员
	mud_chat_to_zone(pstPlayer, pstChatReq);
	return 0;
}

// 世界聊天
static int player_world_chat(MudPlayer *pstPlayer, const CSCHATREQ *pstChatReq,
                             CSPKG *pstResPkg)
{
	UNUSED(pstChatReq);

	// 仅仅广播:是否要广播到Zone?
	Z_CSHEAD_INIT(&pstResPkg->Head, CHAT_RES);
	mud_cltmsg_broadcast_except(pstResPkg, pstPlayer);
	return 0;
}

static int player_friend_chat(MudPlayer *pstPlayer, const CSCHATREQ *pstChat,
                               CSPKG *pstResPkg)
{
	MudPlayer *pstReceiver = NULL;
		
	if (0 == player_check_friendlist(pstPlayer, pstChat->ToName))
	{
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD32,
		             pstChat->ToName);
		return -1;
	}

	pstReceiver = mud_name_player(pstChat->ToName);
	if(NULL == pstReceiver)
	{
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD33, pstChat->ToName);
		return -1;
	}

	if(pstReceiver->eStatus != MUD_PLAYER_STATUS_ROLE_LOGIN)
	{
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD33, pstChat->ToName);
		return -1;
	}

	if(player_check_blacklist(pstReceiver, pstPlayer) < 0)
	{
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD29, pstChat->ToName);
		return -1;
	}

	Z_CSHEAD_INIT(&pstResPkg->Head, CHAT_RES);
	return mud_cltmsg_send(pstReceiver, pstResPkg);
}

static int Name2PlayerIdxHashPred(const void *pv1, const void *pv2)
{
	const Name2PlayerIdx *pstLhs = pv1, *pstRhs = pv2;
	return strcmp(pstLhs->szName, pstRhs->szName);
}

static unsigned int Name2PlayerIdxHashGen(const void* pvData)
{
	const Name2PlayerIdx *pstMapping = pvData;
	return sht_get_code(pstMapping->szName);
}

// 通过名字获取Player
MudPlayer* mud_name_player(const char* pszName)
{
	Name2PlayerIdx stKey, *pstMapping = NULL;
	void *pvRes = NULL;

	if (0 == *pszName)
	{
		return NULL;
	}
	STRNCPY(stKey.szName, pszName, CCH(stKey.szName));
	pstMapping = sht_find(this_env()->pstShtName2PlayerIdx,
	                      &stKey,
	                      Name2PlayerIdxHashPred,
	                      Name2PlayerIdxHashGen);
	if(!pstMapping)
	{
		return NULL;
	}
	pvRes = tmempool_get(this_env()->pstPlayerPool, pstMapping->iIdx);
	if(!pvRes)
	{
		return NULL;
	}
	else
	{
		int *piMagic = pvRes;
		if(!piMagic || *piMagic != MUD_PLAYER_SHM_MAGIC)
		{
			return NULL; // 内存错乱了
		}
	}
	return pvRes;
}

int relation_online_res(SSRELATIONONLINERES *pstRes)
{
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONCHGLIST *pstRelationChgList = &pstRelationSvr->RelationSvrData.RelationChgList;
	CSRELATIONCHG *pstRelationChg;
	MudPlayer *pstPlayer;
	int i;
	
	pstPlayer = mud_name_player(pstRes->RoleName);
	if(NULL == pstPlayer || pstPlayer->eStatus != MUD_PLAYER_STATUS_ROLE_LOGIN)
	{
		return -1;
	}

	pstRelationSvr->OpType = RELATION_OP_TYPE_CHGLIST;
	pstRelationChgList->Num = 0;

	for(i = 0; i<pstRes->BuddyNum; i++)
	{
		pstRelationChg = &pstRelationChgList->ChgList[pstRelationChgList->Num++];
		pstRelationChg->RoleID = pstRes->BuddyInfo[i].RoleID;
		pstRelationChg->RelationType = pstRes->BuddyInfo[i].RelationType;
		pstRelationChg->RelationAttrNum = 0;
		
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_ONLINE;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.IsOnline = ROLE_GAME_ONLINE;
		pstRelationChg->RelationAttrNum++;
	}
	
	for (i = 0; i < pstRelationChgList->Num; i ++)
	{
		pstRelationChg = &pstRelationChgList->ChgList[i];
		if (pstRelationChg->RelationAttrNum > RELATION_ATTR_MAX)
		{
			pstRelationChg->RelationAttrNum = RELATION_ATTR_MAX;
		}
	}

	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	mud_cltmsg_send(pstPlayer, &stPkg);

	return 0;
}

int m_is_player_chat_channel_silence(MudPlayer *pstPlayer, int iChannel)
{
	if (pstPlayer->iChatSilenceFlag & (1 << iChannel ))
	{
		return 1;
	}
	return 0;
}

int mud_ss_chat(SSCHAT *pstChat)
{
	MUD_SVRENV* pstEnv = this_env();
	CSPKG *pstResPkg = &pstEnv->stCSRes;
	CSCHATRES *pstChatRes = &pstResPkg->Body.ChatRes;
	MudPlayer *pstPlayer;

	Z_CSHEAD_INIT(&pstResPkg->Head, CHAT_RES);
	
	pstChatRes->LinkLen = pstChat->LinkLen;
	STRNCPY(pstChatRes->From, pstChat->From, sizeof(pstChatRes->From));
	STRNCPY(pstChatRes->Content, pstChat->Content, sizeof(pstChatRes->Content));
	if (pstChatRes->LinkLen > 0)
	{
		memcpy(pstChatRes->LinkContent, pstChat->LinkContent, pstChat->LinkLen);
	}
	pstChatRes->OnlineType = ROLE_GAME_ONLINE;

	switch(pstChat->ChatType)
	{
	case SS_CHAT_FRIEND:
		pstPlayer = mud_name_player(pstChat->ToData.ToName);
		if(!pstPlayer || pstPlayer->eStatus != MUD_PLAYER_STATUS_ROLE_LOGIN)
		{
			return -1;
		}
		pstChatRes->Type = CHAT_FRIEND;
		return mud_cltmsg_send(pstPlayer, pstResPkg);
		break;

	case SS_CHAT_CLAN:
		pstChatRes->Type = CHAT_CLAN;
		return mud_cltmsg_broadcast_clan(pstResPkg, &pstChat->ToData.ClanInfo);
		break;
		
	case SS_CHAT_PRIVATE:
		pstPlayer = mud_name_player(pstChat->ToData.ToName);
		if(!pstPlayer || pstPlayer->eStatus != MUD_PLAYER_STATUS_ROLE_LOGIN)
		{
			char szMsg[128];
			//给zone提示
			sprintf(szMsg, "聊天对象%s 不在线或不存在", pstChat->ToData.ToName);
			m_send_tip_by_name(szMsg, pstChat->From,ROLE_GAME_ONLINE);
			return -1;
		}
		pstChatRes->Type = CHAT_PRIVATE;
		if (m_is_player_chat_channel_silence(pstPlayer, CHAT_PRIVATE))
		{
			char szMsg[128];
			//给zone提示关闭了频道
			sprintf(szMsg, "聊天对象%s 关闭了此频道",pstChat->ToData.ToName);
			m_send_tip_by_name(szMsg, pstChat->From,ROLE_GAME_ONLINE);
			return -1;
		}
		else
		{
			//给zone回显
			CSPKG stEcho;
			memset(&stEcho,0,sizeof(stEcho));
			stEcho.Body.ChatReq.Res = 1;//必须的,客户端需要
			STRNCPY(stEcho.Body.ChatReq.ToName , pstChat->ToData.ToName,sizeof(stEcho.Body.ChatReq.ToName));
			STRNCPY(stEcho.Body.ChatReq.Content , pstChat->Content,sizeof(stEcho.Body.ChatReq.Content));
			stEcho.Body.ChatReq.OnlineType = ROLE_MUD_ONLINE;
			stEcho.Body.ChatReq.LinkLen = pstChat->LinkLen;
			memcpy(stEcho.Body.ChatReq.LinkContent,pstChat->LinkContent,sizeof(stEcho.Body.ChatReq.LinkContent));
			stEcho.Body.ChatReq.Type = pstChat->ChatType;
			Z_CSHEAD_INIT(&stEcho.Head, CHAT_REQ);
			m_pkg_to_zone(&stEcho, pstChat->From,PKG_TRANS_PLAYER,0);
			
			return mud_cltmsg_send(pstPlayer, pstResPkg);
		}
		break;

	default:
		return -1;
		break;
	}
	
	return 0;
}

int mud_region_msg(const char *pszMsg, int iMsgLen)
{
	MUD_SVRENV* pstEnv = this_env();
	SSPKG *pstSSPkg = &pstEnv->stSSReq;
	MudPlayer *pstPlayer = NULL;

	if(ss_unpack(pstEnv->pstSSHan, (char *)pszMsg, iMsgLen, pstSSPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_unpack fail");
		return -1;
	}

	switch(pstSSPkg->Head.Cmd)
	{
	case KICK_OFF_ACC:
		pstPlayer = mud_player_get_by_uin(pstSSPkg->Body.KickOffAccount.Uin);
		if(pstPlayer)
		{
			mud_player_kickoff(pstPlayer, LJY_MUD36);
		}
		break;
	default:
		break;
	}

	return 0;
}



int mud_relation_online_query(SSRELATIONONLINEREQ *pstReq)
{
	MudPlayer *pstPlayerTmp;
	int i;
	SSPKG stPkg;
	SSRELATIONONLINERES *pstRes = &stPkg.Body.RelationOnlineRes;

	pstRes->BuddyNum = 0;
	STRNCPY(pstRes->RoleName, pstReq->RoleName, sizeof(pstRes->RoleName));
	
	for(i=0; i < pstReq->BuddyNum; i++)
	{
		pstPlayerTmp = mud_name_player(pstReq->BuddyInfo[i].RoleName);
		if(NULL == pstPlayerTmp || 
			pstPlayerTmp->eStatus != MUD_PLAYER_STATUS_ROLE_LOGIN || 
			pstPlayerTmp->stRoleData.ullRoleID != pstReq->BuddyInfo[i].RoleID)
		{
			continue;
		}

		pstRes->BuddyInfo[pstRes->BuddyNum].RoleID = pstPlayerTmp->stRoleData.ullRoleID;
		pstRes->BuddyInfo[pstRes->BuddyNum].RelationType = pstReq->BuddyInfo[i].RelationType;
		pstRes->BuddyNum++;
	}

	if(pstRes->BuddyNum > 0)
	{
		Z_SSHEAD_INIT(&stPkg.Head, RELATION_ONLINE_RES);
		mud_ssmsg_send(&stPkg, this_env()->iZoneSvrID);
	}

	return 0;
}

int m_ss_pkg_trans(SSPKGTRANS *pstTrans)
{
	CSPKG stPkg;
	MudPlayer* pstPlayer = NULL;	
	CLANID stClan;

	memset(&stClan,0,sizeof(stClan));
	Z_CSHEAD_INIT(&stPkg.Head, pstTrans->Cmd);
	stPkg.Body = pstTrans->Body;
	switch (pstTrans->Type)
	{
	case PKG_TRANS_ALL:
		mud_cltmsg_broadcast(&stPkg);
		break;
	case PKG_TRANS_FRIEND:
		mud_cltmsg_broadcast(&stPkg);
		break;
	case PKG_TRANS_PLAYER:
		pstPlayer = mud_name_player(pstTrans->szName);
		if (pstPlayer)
		{
			mud_cltmsg_send(pstPlayer,&stPkg);
		}
		break;
	case PKG_TRANS_CLAN:
		stClan.Id = pstTrans->ClanID;
		mud_cltmsg_broadcast_clan(&stPkg,&stClan);
		break;
	}
	
	return 0;
}

int mud_apex_login(SSROLEONLINE* pstSS)
{
	MUD_SVRENV* pstEnv = this_env();
	
	if (0 != pstEnv->pstConf->NoApex)
	{
		return 0;
	}
	
	if (pstSS->Online == ROLE_OFFLINE)
	{
		s_sendData_ToApex('G', pstSS->AccUin, pstSS->RoleName, strlen(pstSS->RoleName)+1);
		
	}
	else if (pstSS->Online == ROLE_GAME_ONLINE)
	{
		char szIP[10];

		assert(sizeof(szIP) > sizeof(pstSS->IP)+1);
		szIP[0] = 0x01;
		memcpy(&szIP[1], (char *)&pstSS->IP, sizeof(pstSS->IP));
		
		s_sendData_ToApex('L', pstSS->AccUin, pstSS->RoleName, strlen(pstSS->RoleName)+1);
		s_sendData_ToApex('S', pstSS->AccUin, szIP, sizeof(pstSS->IP)+1);
	}
	
	return 0;
}

int role_online(SSROLEONLINE* pstSS)	
{
	CSPKG stPkg;
	CSONLINESVR *pstOnline = &stPkg.Body.OnlineInfo;
	MudUinCache stUinHashData;
	MudUinCache *pstUinCache= NULL;
	
	pstOnline->RoleID = pstSS->RoleID;
	pstOnline->Online = pstSS->Online;
	STRNCPY(pstOnline->RoleName, pstSS->RoleName, sizeof(pstOnline->RoleName));
	
	Z_CSHEAD_INIT(&stPkg.Head, ONLINE_INFO);
	mud_cltmsg_broadcast(&stPkg);

	if (pstSS->Online != ROLE_OFFLINE)
	{
		stUinHashData.iUin = pstSS->AccUin;
		pstUinCache = (MudUinCache *)sht_find(this_env()->pstShtUinCache,&stUinHashData,UinHashCmp,UinHashCode);
		if (pstUinCache)
		{
			pstUinCache->cGetRoleList = 0;
		}
	}

	mud_apex_login(pstSS);

	return 0;
}

//  pstOther详细信息    发送给好友pszReceiver
int m_relation_chg_clt_detail(MudPlayer *pstOther,char * pszReceiver, int iToZone)
{
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONCHG *pstRelationChg = &pstRelationSvr->RelationSvrData.RelationChg;

	memset(pstRelationChg, 0, sizeof(*pstRelationChg));
	
	pstRelationSvr->OpType = RELATION_OP_TYPE_CHG;
	pstRelationChg->RelationAttrNum = 0;
	pstRelationChg->RelationType = RELATION_TYPE_FRIEND;
	pstRelationChg->RoleID = pstOther->stRoleData.ullRoleID;

	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_NAME;
	STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Name,
		pstOther->stRoleData.szRoleName, sizeof(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Name));
	pstRelationChg->RelationAttrNum ++;
	
	//if (iType == RELATION_TYPE_FRIEND || iType == RELATION_TYPE_MASTER || iType == RELATION_TYPE_PRENTICE ||
		//iType == RELATION_TYPE_SPOUSE || iType == RELATION_TYPE_BROTHER || iType == RELATION_TYPE_TMPFRIEND)
	{
		// head字段 高16位表示head         接下来8位表示hair          最后8为表示face
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_HEAD;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= (pstOther->stRoleData.iHead<<16);
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= (pstOther->stRoleData.iHair<<8);
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= pstOther->stRoleData.iFace;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_CAREER;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Career = pstOther->stRoleData.iCareer;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_LEVEL;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Level = pstOther->stRoleData.iLevel;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_EMOTION;
		STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Emotion,
			pstOther->stRoleData.Emotion, sizeof(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Emotion));
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_GENDER;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Gender = pstOther->stRoleData.iGender;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_BUDDYVAL;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.BuddyVal = 0;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_MEMNAME;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.MemName[0] = 0;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_ONLINE;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.IsOnline = ROLE_MUD_ONLINE;
		pstRelationChg->RelationAttrNum ++;

	}
	
	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);

	if (iToZone)
	{
		m_pkg_to_zone(&stPkg,pszReceiver,PKG_TRANS_PLAYER,0);
	}
	else
	{
		MudPlayer *pstPlayer = mud_name_player(pszReceiver);
		if (pstPlayer)
		{
			mud_cltmsg_send(pstPlayer, &stPkg);
		}
	}


	return 0;
} 



int clan_online_member_to_zone(char *pszName,CLANID *pstID)
{
	int i = 0;
	CSPKG stPkg;


	
	CSCLANMEMBERCHG *pstMemChg = &stPkg.Body.ClanSvr.ClanSvrData.ClanMemberChg;
	const SListEntry *astEntries = this_env()->pstPlayerIdxEntries->astEntries;


	const ClanMemberCache *pstFound = clan_mem_cached_find(pstID);

	if (0 == *pszName)
	{
		return -1;
	}
	
	if(!pstFound)
	{
		return -1;
	}

	for(i = pstFound->iMemberEntry; i != -1; i = astEntries[i].iNext)
	{
		MudPlayer *pstPlayer = astEntries[i].iValue < 0 ?
		   NULL : tmempool_get(this_env()->pstPlayerPool, astEntries[i].iValue);

		if (!pstPlayer)
		{
			continue;
		}

		pstMemChg->Code = (unsigned char)CLAN_MEMBER_CHG_MOD;
		get_player_clan_mem(pstPlayer,&pstMemChg->MemInfo);

		stPkg.Body.ClanSvr.Operation = CLAN_SVR_MEMBER_CHG;
		Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
		m_pkg_to_zone(&stPkg, pszName,PKG_TRANS_PLAYER,0);
	}
		
	return 0;
}

int m_set_time(unsigned int uiSec)
{
	MUD_SVRENV* pstEnv = this_env();
	int i = 0;
	TMEMBLOCK* pstMemBlock = NULL;
	MudPlayer* pstPlayer = NULL;

	pstEnv->pstAppCtx->iAdjustTime = uiSec - time(NULL);

	pstEnv->pstAppCtx->stCurr.tv_sec = time(NULL) + pstEnv->pstAppCtx->iAdjustTime;

	for (i=0; i<MAX_MUD_PLAYER; i++)
	{
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (MudPlayer *)pstMemBlock->szData;

		mud_player_kickoff(pstPlayer, LJY_MUD37);
	}

	pstEnv->tDirReport = pstEnv->pstAppCtx->stCurr.tv_sec;

	tlog_notice(pstEnv->pstLogCat, 0, 0, "set time: now=%d, adjust=%d", 
						pstEnv->pstAppCtx->stCurr.tv_sec, pstEnv->pstAppCtx->iAdjustTime);
	return 0;
}

int m_ss_cmd_req(SSCMDREQ* pstReq)
{

	MudPlayer* pstPlayer = NULL;
	switch(pstReq->Cmd)
	{
	case SS_CMD_FRIEND_DETAIL://发好友的详细信息	
		pstPlayer = mud_name_player(pstReq->szName);
		if (pstPlayer)
		{
			m_relation_chg_clt_detail(pstPlayer, pstReq->szReceiver,TRUE);
		}
		break;
		
	case SS_CMD_FRIEND_ADD:
		pstPlayer = mud_name_player(pstReq->szName);
		if (pstPlayer)
		{
			m_friend_add(pstPlayer, &pstReq->Data.FriendAdd.BuddyMember, ROLE_GAME_ONLINE);
		}
		break;
		
	case SS_CMD_CLAN_ONLINE_MEMBER:
		clan_online_member_to_zone(pstReq->szReceiver,&pstReq->Data.ClanID);
		break;
		
	case SS_CMD_SYNC_TIME:
		if (SYNC_TIME_SEC <= abs(this_env()->pstAppCtx->stCurr.tv_sec - pstReq->Data.SyncTime))
		{
			m_set_time(pstReq->Data.SyncTime);
		}
		break;
	case SS_CMD_ACC_NOTICE:
		{
			MudUinCache stUinHashData;
			MudUinCache *pstUinCache = NULL;
			
			stUinHashData.iUin = pstReq->Data.AccUin;
			pstUinCache = (MudUinCache *)sht_find(this_env()->pstShtUinCache,&stUinHashData,UinHashCmp,UinHashCode);
			if (pstUinCache)
			{
				pstUinCache->cGetRoleList = 0;
				pstPlayer = mud_player_get_by_uin(pstReq->Data.AccUin);
				if(pstPlayer)
				{
					mud_player_kickoff(pstPlayer, LJY_MUD38);
				}
			}
		}
		break;
	case SS_CMD_KICK_OFF:		
		{
			MudUinCache stUinHashData;
			MudUinCache *pstUinCache = NULL;
			
			stUinHashData.iUin = pstReq->Data.KickOffAccUin;
			pstUinCache = (MudUinCache *)sht_find(this_env()->pstShtUinCache,&stUinHashData,UinHashCmp,UinHashCode);
			if (pstUinCache)
			{
				pstUinCache->cGetRoleList = 0;
				pstPlayer = mud_player_get_by_uin(pstReq->Data.KickOffAccUin);
				if(pstPlayer)
				{
					mud_player_kickoff(pstPlayer, LJY_MUD39);
				}
			}
		}
		break;

		
	default:
		break;
	}
	
	return 0;
}

int m_ss_info_get_req(SSPKG* pstSSPkg)
{
	SSINFOGETREQ* pstReq = &pstSSPkg->Body.InfoGetReq;
	SSPKG stPkg;
	SSINFOGETRES* pstRes = &stPkg.Body.InfoGetRes;

	MudPlayer* pstPlayer = NULL;
	
	pstRes->Type = pstReq->Type;
	pstRes->ReqBody = pstReq->ReqBody;
	pstRes->Result = -1;

	
	switch(pstReq->Type)
	{
	case INFO_GET_BUDDY_MEMBER:
		pstPlayer = mud_name_player(pstReq->ReqBody.BuddyMember.AddFriendRes.ReqName);		
		if (pstPlayer)
		{
			pstRes->Result = 0;
			m_get_buddy_member(pstPlayer,&pstRes->ResBody.BuddyMemberRes.BuddyMember,&pstRes->ResBody.BuddyMemberRes.BuddyInfo);
			
		}		
		break;
	case INFO_GET_GM:
		break;
	default:
		break;
	}

	Z_SSHEAD_INIT(&stPkg.Head, SS_INFO_GET_RES);
	mud_ssmsg_send(&stPkg,this_env()->iZoneSvrID);
	
	return 0;
}

int m_ss_info_get_res(SSPKG* pstSSPkg)
{
	SSINFOGETRES* pstRes = &pstSSPkg->Body.InfoGetRes;
	MudPlayer* pstPlayer = NULL;
	CSPKG* pstCS = &this_env()->stCSRes;
	
	switch(pstRes->Type)
	{
	case INFO_GET_BUDDY_MEMBER:
		if (0 == pstRes->Result)
		{			
			pstPlayer = mud_name_player(pstRes->ReqBody.BuddyMember.Name);
			if (pstPlayer)
			{
				m_relation_addfriend_res(pstPlayer,&pstRes->ReqBody.BuddyMember.AddFriendRes, &pstRes->ResBody.BuddyMemberRes.BuddyMember);
			}
			
		}		
		break;
	case INFO_GET_GM:
		this_env()->iGetZoneGm = 1;
		if (0 == pstRes->Result)
		{
			this_env()->iOpenGM = pstRes->ResBody.GM;
		}
		pstPlayer = mud_name_player(pstRes->ReqBody.ChatRes.From);
		if (pstPlayer)
		{
			pstCS->Body.ChatRes = pstRes->ReqBody.ChatRes; 
			chat_mud_room(pstPlayer,pstCS);
		}
		break;
	default:
		break;
	}
	return 0;
}

int mud_apex_msg(SSPKG *pstSSPkg)
{
	MUD_SVRENV* pstEnv = this_env();
	SSAPEX *pstApex = &pstSSPkg->Body.Apex;

	if (0 != pstEnv->pstConf->NoApex)
	{
		return 0;
	}

	if (pstApex->ApexMsgType != APEX_MSG_DATA)
	{
		return 0;
	}

	s_sendData_ToApex('T', pstApex->Uin, pstApex->ApexMsg.ApexData.Data, 
						pstApex->ApexMsg.ApexData.DataLen);
	
	return 0;
}

int mud_kick_accname(SSPKG *pstSSPkg)
{
	MudPlayer *pstPlayer;

	pstPlayer = m_loginname_player(pstSSPkg->Body.SSKickAccName.AccName);
	if (pstPlayer)
	{
		mud_player_kickoff(pstPlayer, LJY_MUD40);
	}
	
	return 0;
}

int mud_zone_msg(const char *pszMsg, int iMsgLen)
{
	MUD_SVRENV* pstEnv = this_env();
	SSPKG *pstSSPkg = &pstEnv->stSSReq;

	
	SSPKGTRANS * pstTrans = &pstSSPkg->Body.PkgTansInfo;
	SSCMDREQ *pstReq = &pstSSPkg->Body.CmdReq;
	
	if(ss_unpack(pstEnv->pstSSHan, (char *)pszMsg, iMsgLen, pstSSPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_unpack fail");
		return -1;
	}

	
	tlog_debug(pstEnv->pstLogCat, 0, 0,
           "z-->m msg:headlen=%d,bodylen=%d",
           ntohs(*(short*)(pszMsg + 4)),
           ntohs(*(short*)(pszMsg + 6)));
	
	tlog_debug_dr(pstEnv->pstLogCat, 0, 0,
	              pstEnv->pstSSHan->pstSSProtoPkgMeta, 
	              pstSSPkg, sizeof(*pstSSPkg), 0);

	switch(pstSSPkg->Head.Cmd)
	{
	case SS_CHAT:
		return mud_ss_chat(&pstSSPkg->Body.ChatInfo);
		break;
	case RELATION_ONLINE_RES: 
		return relation_online_res(&pstSSPkg->Body.RelationOnlineRes);
		break;
	case ROLE_ONLINE:
		return role_online(&pstSSPkg->Body.RoleOnline);
		break;
	case RELATION_ONLINE_REQ:
		return mud_relation_online_query(&pstSSPkg->Body.RelationOnlineReq);
		break;
	case SS_PKG_TRANS:
		return m_ss_pkg_trans(pstTrans);
		break;
	case SS_CMD_REQ:
		return m_ss_cmd_req(pstReq);
		break;
	case SS_INFO_GET_REQ:
		return m_ss_info_get_req(pstSSPkg);
		break;
	case SS_INFO_GET_RES:
		return m_ss_info_get_res(pstSSPkg);
		break;
	case SS_APEX:
		return mud_apex_msg(pstSSPkg);
		break;
	case SS_KICK_ACCNAME:
		return mud_kick_accname(pstSSPkg);
		break;
			
	default:
			break;
	}


	return 0;
}

int mud_player_heartbeat(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                 TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	struct timeval stCltTime;
	struct timeval stConnectTime;
	CSTIME *pstCSTime = &pstCsPkg->Body.HeartBeatReq.SvrTime;
	CSPKG stPkg;
	CSHEARTBEATRES *pstRes = &stPkg.Body.HeartBeatRes;
	
	if (NULL == pstPlayer)
	{
		tlog_info(pstEnv->pstLogCat, 0, 0, "player heartbeat null");
		return -1;
	}
	/*
	if (MUD_PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
	{
		return 0;
	}
	*/

	stCltTime.tv_sec = pstCSTime->BenchTimeSec;
	stCltTime.tv_usec = pstCSTime->BenchTimeUSec;

	stConnectTime.tv_sec = pstFrameHead->TimeStamp.TimeVal.sec;
	stConnectTime.tv_usec = pstFrameHead->TimeStamp.TimeVal.usec;

	pstRes->CltTime = *pstCSTime;
	pstRes->ConnectTime.BenchTimeSec = pstFrameHead->TimeStamp.TimeVal.sec;
	pstRes->ConnectTime.BenchTimeUSec = pstFrameHead->TimeStamp.TimeVal.usec;

	Z_CSHEAD_INIT(&stPkg.Head, HEARTBEAT_RES);
	mud_cltmsg_send(pstPlayer, &stPkg);
		
	return 0;
	UNUSED(pstFrameHead);	//add by paraunused.pl
	UNUSED(pstCsPkg);	//add by paraunused.pl
}

int m_is_player_offline_exp_full(MudPlayer *pstPlayer)
{
	if (pstPlayer->iMaxOfflineExp > pstPlayer->iOnlineMudExp + pstPlayer->iZoneOfflineExp)
	{
		return 0;
	}
	return 1;
}

int m_player_offline_exp_full(MudPlayer *pstPlayer,int iType)
{
	CSPKG stPkg;
	CSMUDOFFLINEEXPFULLINFO *pstRes = &stPkg.Body.MudOfflineExpFullInfo;

	if (pstPlayer->iMaxOfflineExp < 1)
	{
		return 0;
	}

	pstRes->RoleID = pstPlayer->stRoleData.ullRoleID;
	STRNCPY(pstRes->RoleName, pstPlayer->stRoleData.szRoleName,CCH(pstRes->RoleName));
	pstRes->Type = (char)iType;

	Z_CSHEAD_INIT(&stPkg.Head, MUD_OFFLINE_EXP_FULL_INFO);
	//发给好友
	//m_player_cltmsg_friend(pstPlayer, &stPkg);
	mud_cltmsg_broadcast(&stPkg);
	return 0;
}

int m_player_offline_exp_change(MudPlayer *pstPlayer,int iMudExp,int iZoneExp)
{

	CSPKG stPkg;
	CSMUDROLEATTRCHG *pstRoleAttrChg = &stPkg.Body.MudRoleAttrChg;
	int iFull = m_is_player_offline_exp_full(pstPlayer);
	int iExp = pstPlayer->iZoneOfflineExp + pstPlayer->iOnlineMudExp;

	pstPlayer->iZoneOfflineExp += iZoneExp;

	if (iMudExp > 0)
	{
		iMudExp = min(iMudExp,pstPlayer->iMaxOfflineExp - pstPlayer->iZoneOfflineExp - pstPlayer->iOnlineMudExp);
	}

	pstPlayer->stShadowData.stCore.MudExp += iMudExp;
	pstPlayer->iOnlineMudExp += iMudExp;
	
	if (iExp == pstPlayer->iZoneOfflineExp + pstPlayer->iOnlineMudExp)
	{
		return 0;//没变
	}

	if (m_is_player_offline_exp_full(pstPlayer))
	{
		m_player_offline_exp_full(pstPlayer,1);
	}
	else
	{
		if (iFull)
		{
			m_player_offline_exp_full(pstPlayer,0);	
		}
	}
	
	pstPlayer->stShadowData.iCacheFlag = 1;

	pstRoleAttrChg->RoleID = pstPlayer->stRoleData.ullRoleID;
	pstRoleAttrChg->AttrType = MUD_ROLE_OFFLINE_EXP;
	pstRoleAttrChg->AttrData.RoleOfflineExp.OfflineExp = pstPlayer->iZoneOfflineExp + pstPlayer->iOnlineMudExp;

	Z_CSHEAD_INIT(&stPkg.Head, MUD_ROLE_ATTR_CHG);
	mud_cltmsg_send(pstPlayer, &stPkg);

	return 0;
}

int m_player_cltmsg_friend(MudPlayer *pstPlayer,CSPKG *pstCSPkgRes)
{
	int i = 0;
	MudPlayer* pstFriend = NULL;
	for (i = 0; i < pstPlayer->stRoleData.iFriendNum; i++)
	{
		pstFriend = mud_name_player(pstPlayer->stRoleData.astFriends[i].RoleName);
		if (!pstFriend)
		{
			continue;
		}
		if (pstFriend->eStatus == MUD_PLAYER_STATUS_ROLE_LOGIN)
		{
			mud_cltmsg_send(pstFriend, pstCSPkgRes);
		}
	}
	return 0;
}



