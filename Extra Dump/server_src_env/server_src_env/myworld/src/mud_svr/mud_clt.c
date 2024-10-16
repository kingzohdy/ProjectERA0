/*
**  @file $RCSfile: mud_clt.c,v $
**  general description of this module
**  $Id: mud_clt.c,v 1.43 2013/05/30 02:39:19 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2013/05/30 02:39:19 $
**  @version $Revision: 1.43 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "proto_comm.h"
#include "proto_cs.h"
#include "role_db_meta.h"
#include "tconnapi.h"
#include "mud_svr.h"
#include "mud_clt.h"
#include "mud_player.h"
#include "libdirty.h"
#include "mud_zh_name.h"


int mud_player_close_connection(TFRAMEHEAD *pstFrameHead);

static int send_sys_str(MudPlayer *pstPlayer, int iSysType, const char *pszStr);



int mud_cltmsg_send(MudPlayer *pstPlayer, const CSPKG *pstCSPkgRes)
{
	MUD_SVRENV *pstEnv = this_env();
	int iRet;
	TDRDATA stNet;
	
	if(pstPlayer->stOnline.bConnLost)
	{
		return -1;
	}
	
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = &pstEnv->szSendBuffer[0];
	iRet = net_cs_pack(pstEnv->pstNetHan, &stNet, (CSPKG *)pstCSPkgRes,
	                   pstPlayer->unCltProVer);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0,
		           "uin=%d cmd=%d prover=%d cs pack fail ret=%d",
				   pstPlayer->stRoleData.iUin, pstCSPkgRes->Head.Cmd,
				   pstPlayer->unCltProVer, iRet);
		return -1;
	}

	tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0,
	           "uin=%d s-->c msg:headlen=%d,bodylen=%d",
	           pstPlayer->stRoleData.iUin,
	           ntohs(*(short*)(stNet.pszBuff + 4)),
	           ntohs(*(short*)(stNet.pszBuff + 6)));
	tlog_debug_dr(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0,
	              pstEnv->pstNetHan->pstCSProtoPkgMeta, 
	              pstCSPkgRes, sizeof(CSPKG), pstPlayer->unCltProVer);

	pstEnv->stFrameHeadTemp.Cmd = pstPlayer->stConnectInfo.chCmd;
	pstEnv->stFrameHeadTemp.ID = pstPlayer->stConnectInfo.iID;
	pstEnv->stFrameHeadTemp.ConnIdx = pstPlayer->stConnectInfo.iConnIdx;

	iRet = tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iMudConnID,
	                     stNet.pszBuff, stNet.iBuff,
	                     &pstEnv->stFrameHeadTemp);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0,
		           "uin=%d cmd=%d  tconnapi_send fail ret=%d",
		           pstPlayer->stRoleData.iUin, pstCSPkgRes->Head.Cmd, iRet);
	}
	
	return iRet; 
}

int mud_cltmsg_send_stop(MudPlayer *pstPlayer,  CSPKG *pstCSPkgRes)
{
	MUD_SVRENV *pstEnv = this_env();
	int iRet;
	TDRDATA stNet;

	if (pstPlayer->stOnline.bConnLost)
	{
		return -1;
	}
	
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = &pstEnv->szSendBuffer[0];
	iRet = net_cs_pack(pstEnv->pstNetHan, &stNet, pstCSPkgRes, pstPlayer->unCltProVer);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0, "uin=%d cmd=%d prover=%d cs pack fail ret=%d",
					pstPlayer->stRoleData.iUin, pstCSPkgRes->Head.Cmd, pstPlayer->unCltProVer, iRet);
		return -1;
	}

	tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0, "uin=%d s-->c stop msg:", pstPlayer->stRoleData.iUin);
	tlog_debug_dr(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0, pstEnv->pstNetHan->pstCSProtoPkgMeta, 
					pstCSPkgRes, sizeof(CSPKG), pstPlayer->unCltProVer);

	//info tconnd stop
	pstEnv->stFrameHeadTemp.Cmd = TFRAMEHEAD_CMD_STOP;
	pstEnv->stFrameHeadTemp.CmdData.Stop.Reason = STOP_REASON_LOGOUT;
	pstEnv->stFrameHeadTemp.ID = pstPlayer->stConnectInfo.iID;
	pstEnv->stFrameHeadTemp.ConnIdx = pstPlayer->stConnectInfo.iConnIdx;

	iRet = tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iMudConnID, stNet.pszBuff, stNet.iBuff, &pstEnv->stFrameHeadTemp);

	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0, "uin=%d cmd=%d	tconnapi_send fail ret=%d",
					pstPlayer->stRoleData.iUin, pstCSPkgRes->Head.Cmd, iRet);
	}

	pstPlayer->stOnline.bConnLost = 1;

	//恢复全局变量初始配置
	pstEnv->stFrameHeadTemp.CmdData.Stop.Reason = 0;
	
	return iRet; 
}

int mud_cltmsg_broadcast_except(CSPKG *pstCSPkgRes, MudPlayer *pstPlayerExcept)
{
	MUD_SVRENV *pstEnv = this_env();
	MudPlayer *pstPlayer = NULL;
	TMEMBLOCK *pstMemBlock;
	TFRAMECMDINPROC *pstInproc;
	int i;
	int iRet;
	TDRDATA stNet;
	int fSend = 0;

	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = &pstEnv->szSendBuffer[0];
	iRet = net_cs_pack(pstEnv->pstNetHan, &stNet, pstCSPkgRes, 0);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "mud_cltmsg_broadcast cmd=%d cs pack fail ret=%d",
					pstCSPkgRes->Head.Cmd, iRet);
		return -1;
	}

	pstInproc = &pstEnv->stFrameHeadTemp.CmdData.InProc;
	pstInproc->Flag = BROADCAST_FLAG_GROUP;
	pstEnv->stFrameHeadTemp.ID = 0;
	pstEnv->stFrameHeadTemp.ConnIdx = 0;
	pstEnv->stFrameHeadTemp.Cmd = TFRAMEHEAD_CMD_INPROC;
	pstInproc->NoEnc = 1;

	for (i=0; i<MAX_MUD_PLAYER; i++)
	{
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (MudPlayer *)pstMemBlock->szData;
		if (MUD_PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus || pstPlayer->stOnline.bConnLost || pstPlayer == pstPlayerExcept)
		{
			continue;
		}

		if (pstInproc->Count >= TFRAMEHEAD_MAX_BATCH_IDENT)
		{
			fSend = 1;
			iRet = tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iMudConnID, stNet.pszBuff, stNet.iBuff, &pstEnv->stFrameHeadTemp);
			if (0 > iRet)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "cmd=%d  tconnapi_send fail ret=%d",
							pstCSPkgRes->Head.Cmd, iRet);
			}
			
			pstInproc->Count = 0;
		}
		
		pstInproc->Idents[pstInproc->Count].ConnIdx = pstPlayer->stConnectInfo.iConnIdx;
		pstInproc->Idents[pstInproc->Count].ID = pstPlayer->stConnectInfo.iID;
		pstInproc->Count++;
	}

	if (pstInproc->Count > 0)
	{
		fSend = 1;
		iRet = tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iMudConnID, stNet.pszBuff, stNet.iBuff, &pstEnv->stFrameHeadTemp);
		if (0 > iRet)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "cmd=%d  tconnapi_send fail ret=%d",
						pstCSPkgRes->Head.Cmd, iRet);
		}
	}

	if(fSend)
	{
		tlog_debug(pstEnv->pstLogCat, 0, 0, "s-->c broad cast msg:headlen=%d,bodylen=%d", ntohs(*(short*)(stNet.pszBuff+4)), ntohs(*(short*)(stNet.pszBuff+6)));
		tlog_debug_dr(pstEnv->pstLogCat, 0, 0, pstEnv->pstNetHan->pstCSProtoPkgMeta, 
						pstCSPkgRes, sizeof(CSPKG), 0);
	}

	//恢复全局变量初始配置
	memset(pstInproc, 0 , offsetof(TFRAMECMDINPROC, Idents));

	return 0;
}

int mud_cltmsg_limit_broadcast(MudPlayer **apstPlayer, int iNum, CSPKG *pstCSPkgRes)
{
	MUD_SVRENV *pstEnv = this_env();
	MudPlayer *pstPlayer = NULL;
	TFRAMECMDINPROC *pstInproc;
	int i;
	int iRet;
	TDRDATA stNet;
	int fSend = 0;

	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = &pstEnv->szSendBuffer[0];
	iRet = net_cs_pack(pstEnv->pstNetHan, &stNet, pstCSPkgRes, 0);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "mud_cltmsg_limit_broadcast cmd=%d cs pack fail ret=%d",
					pstCSPkgRes->Head.Cmd, iRet);
		return -1;
	}

	pstInproc = &pstEnv->stFrameHeadTemp.CmdData.InProc;
	pstInproc->Flag = BROADCAST_FLAG_GROUP;
	pstEnv->stFrameHeadTemp.ID = 0;
	pstEnv->stFrameHeadTemp.ConnIdx = 0;
	pstEnv->stFrameHeadTemp.Cmd = TFRAMEHEAD_CMD_INPROC;
	pstInproc->NoEnc = 1;

	for (i=0; i<iNum; i++)
	{
		pstPlayer = apstPlayer[i];
		if (MUD_PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus || pstPlayer->stOnline.bConnLost)
		{
			continue;
		}

		if (pstInproc->Count >= TFRAMEHEAD_MAX_BATCH_IDENT)
		{
			fSend = 1;
			iRet = tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iMudConnID, stNet.pszBuff, stNet.iBuff, &pstEnv->stFrameHeadTemp);
			if (0 > iRet)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "cmd=%d  tconnapi_send fail ret=%d",
							pstCSPkgRes->Head.Cmd, iRet);
			}
			
			pstInproc->Count = 0;
		}
		
		pstInproc->Idents[pstInproc->Count].ConnIdx = pstPlayer->stConnectInfo.iConnIdx;
		pstInproc->Idents[pstInproc->Count].ID = pstPlayer->stConnectInfo.iID;
		pstInproc->Count++;
	}

	if (pstInproc->Count > 0)
	{
		fSend = 1;
		iRet = tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iMudConnID, stNet.pszBuff, stNet.iBuff, &pstEnv->stFrameHeadTemp);
		if (0 > iRet)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "cmd=%d  tconnapi_send fail ret=%d",
						pstCSPkgRes->Head.Cmd, iRet);
		}
	}

	if(fSend)
	{
		tlog_debug(pstEnv->pstLogCat, 0, 0, "s-->c limit %d broad cast msg:headlen=%d,bodylen=%d", iNum, ntohs(*(short*)(stNet.pszBuff+4)), ntohs(*(short*)(stNet.pszBuff+6)));
		tlog_debug_dr(pstEnv->pstLogCat, 0, 0, pstEnv->pstNetHan->pstCSProtoPkgMeta, 
						pstCSPkgRes, sizeof(CSPKG), 0);
	}

	//恢复全局变量初始配置
	memset(pstInproc, 0 , offsetof(TFRAMECMDINPROC, Idents));

	return 0;
}

int mud_cltmsg_broadcast(CSPKG *pstCSPkgRes)
{
	MUD_SVRENV *pstEnv = this_env();
	MudPlayer *pstPlayer = NULL;
	TMEMBLOCK *pstMemBlock;
	TFRAMECMDINPROC *pstInproc;
	int i;
	int iRet;
	TDRDATA stNet;
	int fSend = 0;

	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = &pstEnv->szSendBuffer[0];
	iRet = net_cs_pack(pstEnv->pstNetHan, &stNet, pstCSPkgRes, 0);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "mud_cltmsg_broadcast cmd=%d cs pack fail ret=%d",
					pstCSPkgRes->Head.Cmd, iRet);
		return -1;
	}

	pstInproc = &pstEnv->stFrameHeadTemp.CmdData.InProc;
	pstInproc->Flag = BROADCAST_FLAG_GROUP;
	pstEnv->stFrameHeadTemp.ID = 0;
	pstEnv->stFrameHeadTemp.ConnIdx = 0;
	pstEnv->stFrameHeadTemp.Cmd = TFRAMEHEAD_CMD_INPROC;
	pstInproc->NoEnc = 1;

	for (i=0; i<MAX_MUD_PLAYER; i++)
	{
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (MudPlayer *)pstMemBlock->szData;
		if (MUD_PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus || pstPlayer->stOnline.bConnLost)
		{
			continue;
		}

		if (pstInproc->Count >= TFRAMEHEAD_MAX_BATCH_IDENT)
		{
			fSend = 1;
			iRet = tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iMudConnID, stNet.pszBuff, stNet.iBuff, &pstEnv->stFrameHeadTemp);
			if (0 > iRet)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "cmd=%d  tconnapi_send fail ret=%d",
							pstCSPkgRes->Head.Cmd, iRet);
			}
			
			pstInproc->Count = 0;
		}
		if (CHAT_RES == pstCSPkgRes->Head.Cmd && m_is_player_chat_channel_silence(pstPlayer,CHAT_WORLD))
		{
			continue;
		}
		if (0xFF == pstPlayer->stOnline.bChatroom)
		{
			continue;
		}
			
		
		pstInproc->Idents[pstInproc->Count].ConnIdx = pstPlayer->stConnectInfo.iConnIdx;
		pstInproc->Idents[pstInproc->Count].ID = pstPlayer->stConnectInfo.iID;
		pstInproc->Count++;
	}

	if (pstInproc->Count > 0)
	{
		fSend = 1;
		iRet = tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iMudConnID, stNet.pszBuff, stNet.iBuff, &pstEnv->stFrameHeadTemp);
		if (0 > iRet)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "cmd=%d  tconnapi_send fail ret=%d",
						pstCSPkgRes->Head.Cmd, iRet);
		}
	}

	if(fSend)
	{
		tlog_debug(pstEnv->pstLogCat, 0, 0, "s-->c broad cast msg:headlen=%d,bodylen=%d", ntohs(*(short*)(stNet.pszBuff+4)), ntohs(*(short*)(stNet.pszBuff+6)));
		tlog_debug_dr(pstEnv->pstLogCat, 0, 0, pstEnv->pstNetHan->pstCSProtoPkgMeta, 
						pstCSPkgRes, sizeof(CSPKG), 0);
	}

	//恢复全局变量初始配置
	memset(pstInproc, 0 , offsetof(TFRAMECMDINPROC, Idents));

	return 0;
}

int mud_room_broadcast(ChatRoomInfo *pstChatRomm, CSPKG *pstCSPkgRes)
{
	MudPlayer *pstTmpPlayer;
	MudPlayer *apstPlayer[MAX_CHAT_ROOM_PLAYER];
	int i;
	int iPlayerNum = 0;

	if(NULL == pstChatRomm || pstChatRomm->iPlayerNum <= 0)
	{
		return -1;
	}

	iPlayerNum = 0;
	for(i=0; i<pstChatRomm->iPlayerNum; i++)
	{
		pstTmpPlayer = z_idx_player(pstChatRomm->aiPlayerIdx[i]);
		if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != MUD_PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}

		apstPlayer[iPlayerNum++] = pstTmpPlayer;
	}

	if(!iPlayerNum)
	{
		return -1;
	}
	
	return mud_cltmsg_limit_broadcast(apstPlayer, iPlayerNum, pstCSPkgRes);
}

int mud_room_broadcast_except(ChatRoomInfo *pstChatRomm, CSPKG *pstCSPkgRes, MudPlayer *pstPlayerExcept)
{
	MudPlayer *pstTmpPlayer;
	MudPlayer *apstPlayer[MAX_CHAT_ROOM_PLAYER];
	int i;
	int iPlayerNum = 0;

	if(NULL == pstChatRomm || pstChatRomm->iPlayerNum <= 0)
	{
		return -1;
	}

	iPlayerNum = 0;
	for(i=0; i<pstChatRomm->iPlayerNum; i++)
	{
		pstTmpPlayer = z_idx_player(pstChatRomm->aiPlayerIdx[i]);
		if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != MUD_PLAYER_STATUS_ROLE_LOGIN || pstTmpPlayer == pstPlayerExcept)
		{
			continue;
		}

		apstPlayer[iPlayerNum++] = pstTmpPlayer;
	}

	if(!iPlayerNum)
	{
		return -1;
	}
	
	return mud_cltmsg_limit_broadcast(apstPlayer, iPlayerNum, pstCSPkgRes);
}


int mud_dispatch_clt_msg(MUD_SVRENV *pstEnv, char *pszMsg, int iMsgLen,
                         TFRAMEHEAD *pstFrameHead)
{
	int iVersion = 0;
	MudPlayer *pstPlayer = NULL;
	MudCmdMap *pstCmdMap = NULL;
	TMEMBLOCK *pstMemBlock = NULL;
	int iCmd;

	/*handle special event of connection*/
	switch(pstFrameHead->Cmd)
	{
	case TFRAMEHEAD_CMD_START:	
		/* how to handle start depond on clt/server connect mode
		{
			iRet = player_open_connection(pstAppCtx, pstEnv, pstFrameHead);
		}*/
		break;
	case TFRAMEHEAD_CMD_INPROC:
		if (0 > pstFrameHead->ID)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "frame head id=%d",
			           pstFrameHead->ID);
			return -1;
		}

		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool,
		                               pstFrameHead->ID);
		if (pstMemBlock->fValid && pstFrameHead->ID == pstMemBlock->iIdx)
		{
			pstPlayer = (MudPlayer *)pstMemBlock->szData;
		}
		else
		{
			pstPlayer = NULL;
		}

		if (pstPlayer && MUD_PLAYER_STATUS_ACC_NOLOGIN < pstPlayer->eStatus)
		{
			iVersion = pstPlayer->unCltProVer;
		}
		break;
	case TFRAMEHEAD_CMD_STOP:
		return 	mud_player_close_connection(pstFrameHead);
		break;
	default:
		break;
	}/*switch(pstFrameHead->chCmd)*/

	if(net_cs_unpack(pstEnv->pstNetHan, pszMsg, iMsgLen,
	                 &pstEnv->stCSReq, iVersion) < 0)
	{
		return -1;
	}

	if (pstPlayer)
	{
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0,
		           "c-->s player %d cmd %d", pstPlayer->stRoleData.iUin,pstEnv->stCSReq.Head.Cmd);
	}

	tlog_debug_dr(pstEnv->pstLogCat, 0, 0,
	              pstEnv->pstNetHan->pstCSProtoPkgMeta, 
				  &pstEnv->stCSReq, sizeof(pstEnv->stCSReq), 0);

	assert(pstEnv->stCSReq.Head.Cmd < MAX_CS_CMD);

	iCmd = pstEnv->stCSReq.Head.Cmd;
	pstCmdMap = &pstEnv->astCmdMap[iCmd];

	if(pstPlayer && 0 <= pstCmdMap->cStateLimit &&
	   pstPlayer->eStatus != pstCmdMap->cStateLimit)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.iUin, 0,
		           "recv clt uin=%d cmd=%d state=%d", pstPlayer->stRoleData.iUin,
		           pstEnv->stCSReq.Head.Cmd, pstPlayer->eStatus);
		return -1;
	}

	if(pstCmdMap->cStateLimit == MUD_PLAYER_STATUS_ROLE_LOGIN && !pstPlayer)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "player null");
		return -1;
	}

	//control cmd handle by flag
	if (pstCmdMap->cCtrlFlag)
	{
		if (pstPlayer)
		{
			pstPlayer->tActive = CURR;
		}
		pstFrameHead->TimeStamp.TimeVal.sec += this_env()->pstAppCtx->iAdjustTime;

		return pstCmdMap->pfnCmdHandle(pstEnv, pstPlayer, pstFrameHead,
		                               &pstEnv->stCSReq);
	}
	else
	{
		return -1;
	}

	return 0;
}

int mud_send_role_list(MudPlayer *pstPlayer, const MudUinCache *pstRoleList)
{
	//MUD_SVRENV *pstEnv = this_env();
	CSPKG stPkg;
	CSROLELIST *pstRes = &stPkg.Body.RoleList;
	int i;

	for(i = 0; i < pstRoleList->ucRoleNum; i++)
	{
		ROLEINFO *pstDst = &pstRes->RoleList[i];
		const ROLELISTINFO *pstSrc = &pstRoleList->astRoleListInfo[i];
		
		pstDst->Career = pstSrc->Career;
		pstDst->Gender = pstSrc->Gender;
		pstDst->RoleID = pstSrc->RoleID;
		STRNCPY(pstDst->RoleName, pstSrc->RoleName, sizeof(pstSrc->RoleName));
		pstDst->Head = pstSrc->Head;
		pstDst->Face = pstSrc->Face;
		pstDst->Level = pstSrc->Level;
		pstDst->Map = pstSrc->Map;
		pstDst->Hair = pstSrc->Hair;
		pstDst->LastLogin = pstSrc->LastLogin;
		memcpy(&pstDst->Avatar, &pstSrc->AvatarProf, sizeof(pstDst->Avatar));

		pstPlayer->iAccMaxLevel = max(pstPlayer->iAccMaxLevel,pstSrc->Level);
	}
	pstRes->RoleNum = pstRoleList->ucRoleNum;

	Z_CSHEAD_INIT(&stPkg.Head, ROLE_LIST);
	return mud_cltmsg_send(pstPlayer, &stPkg);
}

static int send_sys_str(MudPlayer *pstPlayer, int iSysType, const char *pszStr)
{
	CSPKG *pstPkg = &this_env()->stCSRes;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;

	pstSysMsg->Type = iSysType;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
	STRNCPY(pstSysMsg->Msg, pszStr, sizeof(pstSysMsg->Msg));

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	return mud_cltmsg_send(pstPlayer, pstPkg);
}

int mud_sys_strf(MudPlayer *pstPlayer, int iType, const char *pszFormat, ...)
{
	char szBuffer[SYS_MSG_LEN];
	va_list valist;
	
	va_start(valist, pszFormat);
	vsnprintf(szBuffer, CCH(szBuffer), pszFormat, valist);
	szBuffer[SYS_MSG_LEN -1] = 0;
	va_end(valist);

	return send_sys_str(pstPlayer, iType, szBuffer);
}

int mud_ssmsg_send(SSPKG *pstSSPkg, int iDstID)	
{
	MUD_SVRENV *pstEnv = this_env();
	TAPPCTX* pstAppCtx = pstEnv->pstAppCtx;
	int iSrcID;
	TDRDATA szNet;
	char szBuff[sizeof(SSPKG)];
	
	szNet.iBuff = sizeof(szBuff);
	szNet.pszBuff = szBuff;

	if (0 > ss_pack(pstEnv->pstSSHan, &szNet, pstSSPkg,  0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -1;
	}

	iSrcID = pstAppCtx->iId;
	if (0 > tbus_send(pstAppCtx->iBus, &iSrcID, &iDstID, szNet.pszBuff, szNet.iBuff, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail");
		return -1;
	}

	tlog_debug(pstEnv->pstLogCat, 0, 0,
	           "m-->z msg:headlen=%d,bodylen=%d",
	           ntohs(*(short*)(szNet.pszBuff + 4)),
	           ntohs(*(short*)(szNet.pszBuff + 6)));
	
	tlog_debug_dr(pstEnv->pstLogCat, 0, 0,
	              pstEnv->pstSSHan->pstSSProtoPkgMeta, 
	              pstSSPkg, sizeof(*pstSSPkg), 0);
	
	return 0;
}


// 广播至指定公会
int mud_cltmsg_broadcast_clan(const CSPKG *pstPkg, const CLANID *pstClanID)
{
	MudPlayer *astOutgoingList[CLAN_MAX_MEM];
	int i = -1, iCount = 0;
	const SListEntry *astEntries = this_env()->pstPlayerIdxEntries->astEntries;
	const ClanMemberCache *pstFound = clan_mem_cached_find(pstClanID);
	
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

		//成员屏蔽了公会聊天
		if (pstPkg->Head.Cmd == CHAT_RES && m_is_player_chat_channel_silence(pstPlayer, CHAT_CLAN))
		{
			continue;
		}

		astOutgoingList[iCount++] = pstPlayer;
	}

	return mud_cltmsg_limit_broadcast(astOutgoingList, iCount, (CSPKG*)pstPkg);
	/*
	MudPlayer *astOutgoingList[CLAN_MAX_MEM];
	int i = MAX_MUD_PLAYER, iCount = 0;
	while(i--)
	{
		MudPlayer *pstPlayer = NULL;
		TMEMBLOCK *pstMemBlock = 
			TMEMPOOL_GET_PTR_BYPOS(this_env()->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (MudPlayer *)pstMemBlock->szData;
		if(pstPlayer->eStatus != MUD_PLAYER_STATUS_ROLE_LOGIN ||
		   pstPlayer->stOnline.bConnLost)
		{
			continue;
		}

		if(pstPlayer->stRoleData.stClanInfo.GID == pstClanID->GID)
		{
			astOutgoingList[iCount++] = pstPlayer;
		}
	}

	return mud_cltmsg_limit_broadcast(astOutgoingList, iCount, (CSPKG*)pstPkg);
	*/
}

int mud_enter_chat_room_req(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                 TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{

	CSMUDENTERCHATROOMREQ *pstReq = &pstCsPkg->Body.MudEnterChatRoomReq;
	ChatRoomInfo *pstRoom = NULL;
	CSPKG stResPkg;
	CSMUDENTERCHATROOMRES *pstRes = &stResPkg.Body.MudEnterChatRoomRes;
	
	BYTE bRoom = (BYTE) (pstReq->ChatRoomIdx - 1);
	if (bRoom >= MAX_MUD_CHAT_ROOM)
	{
		mud_sys_strf(pstPlayer, SYS_TIP, "无效的房间号");
		goto failed;
	}


	pstRoom = &pstEnv->pstMudChatRoom->astChatRoom[bRoom];

	if (pstRoom->iPlayerNum >= MAX_CHAT_ROOM_PLAYER)
	{
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD1);
		goto failed;
		
	}

	if (0xFF != pstPlayer->stOnline.bChatroom)
	{
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD2);
		goto failed;
	}

	if (0 > m_player_enter_chat_room(pstPlayer, bRoom))
	{
		goto failed;
	}

	
	return 0;
	UNUSED(pstFrameHead);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstCsPkg);	//add by paraunused.pl
	
failed:
	memset(pstRes,0,sizeof(*pstRes));
	pstRes->Result = 1;

	Z_CSHEAD_INIT(&stResPkg.Head, MUD_ENTER_CHAT_ROOM_RES);
	mud_cltmsg_send(pstPlayer, &stResPkg);
	
	return -1;
	
}

int mud_out_chat_room_req(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                 TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{

	//ChatRoomInfo *pstRoom = NULL;	//comment by paraunused.pl
	CSPKG stResPkg;
	CSMUDOUTCHATROOMRES *pstRes = &stResPkg.Body.MudOutChatRoomRes;
	int iRet = -1;
	
	BYTE bRoom = pstPlayer->stOnline.bChatroom;
	if (bRoom >= MAX_MUD_CHAT_ROOM)
	{
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD3);
		goto fini;
	}


	m_player_leave_chat_room_info(pstPlayer,pstPlayer->stOnline.bChatroom);

	iRet = 0;

	
	UNUSED(pstFrameHead);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstCsPkg);	//add by paraunused.pl
	
fini:
	memset(pstRes,0,sizeof(*pstRes));
	pstRes->Result = iRet;

	Z_CSHEAD_INIT(&stResPkg.Head, MUD_OUT_CHAT_ROOM_RES);
	mud_cltmsg_send(pstPlayer, &stResPkg);
	
	return -1;
	
}


int mud_chat_room_list_req(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                 TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSPKG *pstResPkg = &pstEnv->stCSRes;
	CSMUDCHATROOMLISTRES *pstRes = &pstResPkg->Body.MudChatRoomListRes;
	MudChatRoom* pstMudChatRoom = pstEnv->pstMudChatRoom;
	int i = 0;

	pstRes->RoomNum = MAX_MUD_CHAT_ROOM;
	for (i = 0; i < MAX_MUD_CHAT_ROOM; i++)
	{
		pstRes->RoomPlayerNum[i] = pstMudChatRoom->astChatRoom[i].iPlayerNum;
	}

	Z_CSHEAD_INIT(&pstResPkg->Head, MUD_CHAT_ROOM_LIST_RES);
	mud_cltmsg_send(pstPlayer, pstResPkg);
	
	return 0;
	UNUSED(pstFrameHead);	//add by paraunused.pl
	UNUSED(pstCsPkg);	//add by paraunused.pl
}

int mud_chat_channel_set_req(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                 TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSPKG *pstResPkg = &pstEnv->stCSRes;
	CSMUDCHATCHANNELSETREQ *pstReq = &pstCsPkg->Body.MudChatChannelSetReq;
	CSMUDCHATCHANNELSETRES *pstRes = &pstResPkg->Body.MudChatChannelSetRes;

	if (pstReq->MudChatChannelInfo.Silence)
	{
		pstPlayer->iChatSilenceFlag |= 1 << pstReq->MudChatChannelInfo.ChatType;
	}
	else
	{
		pstPlayer->iChatSilenceFlag &= ~(1 << pstReq->MudChatChannelInfo.ChatType);
	}

	pstRes->MudChatChannelInfo.ChatType = pstReq->MudChatChannelInfo.ChatType;
	pstRes->MudChatChannelInfo.Silence = pstReq->MudChatChannelInfo.Silence;
	
	Z_CSHEAD_INIT(&pstResPkg->Head, MUD_CHAT_CHANNEL_SET_RES);
	mud_cltmsg_send(pstPlayer, pstResPkg);
	
	return 0;
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

int mud_offline_exp_take_req(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                 TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSPKG *pstResPkg = &pstEnv->stCSRes;
	CSMUDOFFLINEEXPTAKEREQ *pstReq = &pstCsPkg->Body.MudOfflineExpTakeReq;
	CSMUDOFFLINEEXPTAKERES *pstRes = &pstResPkg->Body.MudOfflineExpTakeRes;
	MudPlayer* pstTarget = NULL;
	int iExp = 0;
	int i = 0;
	int iFriend = 0;


	pstTarget = mud_name_player(pstReq->RoleName);
	if (!pstTarget)
	{
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD4);
		return -1;
	}

	for(i = 0; i < pstPlayer->stRoleData.iFriendNum; i++)
	{
		if ( 0 == strcmp(pstReq->RoleName,pstPlayer->stRoleData.astFriends[i].RoleName))
		{
			iFriend = 1;
			break;
		}
	}

	if (0 == iFriend)
	{
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD5);
		return -1;
	}


	if(0 == m_is_player_offline_exp_full(pstTarget))
	{
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD6);
		return -1;
	}

	if (IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstPlayer->stShadowData.stCore.TakeTime))
	{
		if (pstPlayer->stShadowData.stCore.DayTimes >= MAX_DAY_OFFLINE_EXP_TAKE)
		{
			mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD7);
			return -1;
		}
	}
	else
	{
		pstPlayer->stShadowData.stCore.DayTimes = 0;
	}

	pstPlayer->stShadowData.stCore.TakeTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstPlayer->stShadowData.stCore.DayTimes++;
	
	iExp = (int)pstTarget->iMaxOfflineExp * 0.03f;
	iExp = min(iExp,(int)pstPlayer->iMaxOfflineExp * 0.03f);
	
	m_player_offline_exp_change(pstTarget, -iExp,0);
	iExp += 0.01f * pstPlayer->iMaxOfflineExp;
	m_player_offline_exp_change(pstPlayer, iExp,0);

	pstRes->Result = 0;
	Z_CSHEAD_INIT(&pstResPkg->Head, MUD_OFFLINE_EXP_TAKE_RES);
	mud_cltmsg_send(pstPlayer, pstResPkg);

	{
		OP_LOG_META stOpLog;
		OPMUDTAKEEXP *pstTakeExp = &stOpLog.OpLog.OpMudTakeExp;
		
		stOpLog.OpLogType = OP_LOG_MUD_TAKE_EXP;
		pstTakeExp->TakeExp = iExp;
		pstTakeExp->Uin = pstPlayer->stRoleData.iUin;
		pstTakeExp->RoleInfo.RoleID = pstPlayer->stRoleData.ullRoleID;
		STRNCPY(pstTakeExp->RoleInfo.RoleName,pstPlayer->stRoleData.szRoleName,CCH(pstTakeExp->RoleInfo.RoleName));
		pstTakeExp->Exp = pstPlayer->iZoneOfflineExp + pstPlayer->iOnlineMudExp;
		STRNCPY(pstTakeExp->Pass9Account, pstPlayer->szAccount, sizeof(pstTakeExp->Pass9Account));

		pstTakeExp->TargetRemainExp = pstTarget->iZoneOfflineExp + pstTarget->iOnlineMudExp;
		pstTakeExp->TargetRoleInfo.RoleID = pstTarget->stRoleData.ullRoleID;
		STRNCPY(pstTakeExp->TargetRoleInfo.RoleName,pstTarget->stRoleData.szRoleName,CCH(pstTakeExp->TargetRoleInfo.RoleName));
		
		pstTakeExp->LogTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		m_oplog_send(&stOpLog);
	}
	
	return 0;
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

int m_relation_find_by_id(MudPlayer* pstPlayer,tdr_ulonglong RoleID, int iType)
{
	int i = 0;
	if (RELATION_TYPE_FRIEND == iType)
	{
		for (i = 0; i < pstPlayer->stRoleData.iFriendNum;i++)
		{
			if ( RoleID == pstPlayer->stRoleData.astFriends[i].RoleID)
			{
				return i;
			}
		}
	}
	else if (RELATION_TYPE_BLACK == iType)
	{
		for (i = 0; i < pstPlayer->stRoleData.iBadGuyNum;i++)
		{
			if ( RoleID == pstPlayer->stRoleData.astBadGuys[i].RoleID)
			{
				return i;
			}
		}
	}
	return -1;
}

int m_relation_find_by_name(MudPlayer* pstPlayer,char* pszName, int iType)
{
	int i = 0;
	if (RELATION_TYPE_FRIEND == iType)
	{
		for (i = 0; i < pstPlayer->stRoleData.iFriendNum;i++)
		{
			if ( 0 == strcmp(pszName,pstPlayer->stRoleData.astFriends[i].RoleName))
			{
				return 1;
			}
		}
	}
	else if (RELATION_TYPE_BLACK == iType)
	{
		for (i = 0; i < pstPlayer->stRoleData.iBadGuyNum;i++)
		{
			if ( 0 == strcmp(pszName,pstPlayer->stRoleData.astBadGuys[i].RoleName))
			{
				return 1;
			}
		}
	}
	return 0;
}

int m_relation_addfriend_req(MudPlayer* pstPlayer,CSRELATIONCLT *pstClt)
{
	ADDFRIENDREQ* pstReq = &pstClt->RelationCltData.AddRelationReq.AddRelationData.AddFriend;

	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;


	// 加自己
	if ( 0 == strcmp(pstPlayer->stRoleData.szRoleName, pstReq->Name))
	{
		mud_sys_strf(pstPlayer,SYS_MIDDLE2, "不能添加自己为好友");
		return -1;
	}
	

	// 在自己黑名单中
	if (m_relation_find_by_name(pstPlayer, pstReq->Name,RELATION_TYPE_BLACK))
	{
		mud_sys_strf(pstPlayer,SYS_MIDDLE2, LJY_MUD8);
		return -1;
	}
	
	

	// 已经在好友名单中
	if (m_relation_find_by_name(pstPlayer, pstReq->Name,RELATION_TYPE_FRIEND))
	{
		mud_sys_strf(pstPlayer,SYS_MIDDLE2, LJY_MUD9);
		return -1;
	}

	if (pstPlayer->stRoleData.iFriendNum >= pstPlayer->stRoleData.iFriendMax)
	{
		mud_sys_strf(pstPlayer,SYS_MIDDLE2, LJY_MUD10);
		return -1;
	}

	// 避免客户端总是弹出加用友面板
	if (0 == strcmp(pstPlayer->szAddFriendName, pstReq->Name))
	{
		mud_sys_strf(pstPlayer,SYS_MIDDLE2, LJY_MUD11);
		return -1;
	}
		//对方在mud
	if (pstReq->OnlineType != ROLE_GAME_ONLINE)
	{
		// 对方不在线
		MudPlayer *pstOther = mud_name_player(pstReq->Name);
		if (NULL == pstOther)
		{
			char szMsg[128];
			snprintf(szMsg,sizeof(szMsg),LJY_MUD12, pstReq->Name);
			mud_sys_strf(pstPlayer,SYS_MIDDLE2, szMsg);
			return -1;
		}
	}
	
	STRNCPY(pstPlayer->szAddFriendName,pstReq->Name, CCH(pstPlayer->szAddFriendName));
	
	pstRelationSvr->OpType = RELATION_OP_TYPE_ADDFRIEND_INFO;
	STRNCPY(pstRelationSvr->RelationSvrData.RelationAddFriendInfo.ReqName, 
		pstPlayer->stRoleData.szRoleName, sizeof(pstRelationSvr->RelationSvrData.RelationAddFriendInfo.ReqName));
	STRNCPY(pstRelationSvr->RelationSvrData.RelationAddFriendInfo.ReqInfo, 
		pstReq->ReqInfo, sizeof(pstRelationSvr->RelationSvrData.RelationAddFriendInfo.ReqInfo));
	pstRelationSvr->RelationSvrData.RelationAddFriendInfo.OnlineType = ROLE_MUD_ONLINE;
	
	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);

	m_cltmsg_send_by_name(&stPkg, pstReq->Name, pstReq->OnlineType);
		
	return 0;
}

int m_reject_add(MudPlayer *pstPlayer,CSADDFRIENDRES* pstRes)
{
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONREJECT *pstRelationReject = &pstRelationSvr->RelationSvrData.RelationReject;
	char szMsg[128];

	sprintf(szMsg, LJY_MUD13, pstPlayer->stRoleData.szRoleName);	
	m_send_tip_by_name(szMsg,pstRes->ReqName,pstRes->OnlineType);

	sprintf(szMsg, LJY_MUD14,pstRes->ReqName);
	mud_sys_strf(pstPlayer, SYS_MIDDLE2, szMsg);

	pstRelationSvr->OpType = RELATION_OP_TYPE_REJECT;
	pstRelationReject->Type = RELATION_TYPE_FRIEND;
	pstRelationReject->RoleID = pstPlayer->stRoleData.ullRoleID;
	STRNCPY(pstRelationReject->RejectName, pstPlayer->stRoleData.szRoleName,
		sizeof(pstRelationReject->RejectName));

	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	m_cltmsg_send_by_name(&stPkg,pstRes->ReqName,pstRes->OnlineType);
	return 0;
}

int m_friend_detail(MudPlayer* pstPlayer,char* pszName,int iOnlineType)
{
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONCHG *pstRelationChg = &pstRelationSvr->RelationSvrData.RelationChg;

	memset(pstRelationChg, 0, sizeof(*pstRelationChg));
	
	pstRelationSvr->OpType = RELATION_OP_TYPE_CHG;
	pstRelationChg->RelationAttrNum = 0;
	pstRelationChg->RelationType = RELATION_TYPE_FRIEND;
	pstRelationChg->RoleID = pstPlayer->stRoleData.ullRoleID;

	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_NAME;
	STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Name,
		pstPlayer->stRoleData.szRoleName, sizeof(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Name));
	pstRelationChg->RelationAttrNum ++;
	
	//if (iType == RELATION_TYPE_FRIEND || iType == RELATION_TYPE_MASTER || iType == RELATION_TYPE_PRENTICE ||
		//iType == RELATION_TYPE_SPOUSE || iType == RELATION_TYPE_BROTHER || iType == RELATION_TYPE_TMPFRIEND)
	{
		// head字段 高16位表示head         接下来8位表示hair          最后8为表示face
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_HEAD;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= (pstPlayer->stRoleData.iHead<<16);
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= (pstPlayer->stRoleData.iHair<<8);
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= pstPlayer->stRoleData.iFace;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_CAREER;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Career = pstPlayer->stRoleData.iCareer;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_LEVEL;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Level = pstPlayer->stRoleData.iLevel;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_EMOTION;
		STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Emotion,
			pstPlayer->stRoleData.Emotion, sizeof(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Emotion));
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_GENDER;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Gender = pstPlayer->stRoleData.iGender;
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

		//if (pstOther->stRoleData.MiscInfo.ClanInfo.ClanId.Id > 0)
		//{
		//	relation_claninfo_to_clt(pstEnv, pstPlayer, pstOther);
		//}

	}
	
	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);

	m_cltmsg_send_by_name(&stPkg,pszName, iOnlineType);

	//公会信息
	
	pstRelationSvr->OpType = RELATION_OP_TYPE_CHG_CLAN;
	pstRelationSvr->RelationSvrData.RelationClanInfo.RoleID = pstPlayer->stRoleData.ullRoleID;
	pstRelationSvr->RelationSvrData.RelationClanInfo.RelationClan.ClanID = pstPlayer->stRoleData.stClanInfo.Id;
	pstRelationSvr->RelationSvrData.RelationClanInfo.RelationClan.ClanName[0] = 0;
	if (pstPlayer->stRoleData.stClanInfo.Id > 0)
	{
		STRNCPY(pstRelationSvr->RelationSvrData.RelationClanInfo.RelationClan.ClanName, 
				  pstPlayer->stRoleData.stClanInfo.Name,
				  sizeof(pstRelationSvr->RelationSvrData.RelationClanInfo.RelationClan.ClanName));
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);	
	m_cltmsg_send_by_name(&stPkg,pszName, iOnlineType);
	
	return 0;
}

int m_del_black(MudPlayer *pstPlayer, char *pszName)
{

	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONDELRES *pstRelationDelRes = &pstRelationSvr->RelationSvrData.RelationDelRes; 	
	int i = 0;
	
	pstPlayer->stRoleData.iBadGuyNum = min(pstPlayer->stRoleData.iBadGuyNum,MAX_BLACK_NUM);
	for( i = 0; i < pstPlayer->stRoleData.iBadGuyNum;i++)
	{
		if ( 0 ==  strcmp(pstPlayer->stRoleData.astBadGuys[i].RoleName,pszName))
		{
			break;
		}
	}

	if (i >= pstPlayer->stRoleData.iBadGuyNum)
	{
		return 0;
	}

	pstRelationSvr->OpType = RELATION_OP_TYPE_DELRELATION_RES;	
	pstRelationDelRes->Type = RELATION_TYPE_BLACK;
	pstRelationDelRes->RoleID = pstPlayer->stRoleData.astBadGuys[i].RoleID;
	
	pstPlayer->stRoleData.iBadGuyNum--;

	memcpy(&pstPlayer->stRoleData.astBadGuys[i],
		&pstPlayer->stRoleData.astBadGuys[pstPlayer->stRoleData.iBadGuyNum],
		sizeof(pstPlayer->stRoleData.astBadGuys[i]));


		
	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	mud_cltmsg_send(pstPlayer, &stPkg);
	return 0;
}

// 添加好友 pstPlayer添加pstOther为好友
int m_friend_add(MudPlayer *pstPlayer, ROLEBUDDYMEMBER *pstOtherMember, int iOnlineType)
{
	
	ROLEBUDDYMEMBER *pstRoleBuddyMember = NULL; 
	MudPlayer* pstOther = NULL;

	char szMsg[128];

	if (m_relation_find_by_name(pstPlayer, pstOtherMember->RoleName,RELATION_TYPE_FRIEND))
	{
		return -1;
	}

	if (pstPlayer->stRoleData.iFriendNum >= pstPlayer->stRoleData.iFriendMax)
	{
		mud_sys_strf(pstPlayer, SYS_MIDDLE2, LJY_MUD15);
		return -1;
	}

	pstRoleBuddyMember = &pstPlayer->stRoleData.astFriends[pstPlayer->stRoleData.iFriendNum];
	pstPlayer->stRoleData.iFriendNum++;
	pstPlayer->iBuddyDirty = 1;

	memcpy(pstRoleBuddyMember,pstOtherMember,sizeof(*pstRoleBuddyMember));
	
	pstRoleBuddyMember->RelationType = RELATION_TYPE_FRIEND;
	pstRoleBuddyMember->Gender = pstOtherMember->Gender;

	snprintf(szMsg,sizeof(szMsg),LJY_MUD16,pstPlayer->stRoleData.szRoleName);
	m_send_tip_by_name(szMsg,pstOtherMember->RoleName,iOnlineType);

	// 删除黑名单
	m_del_black(pstPlayer,pstOtherMember->RoleName);

	if (iOnlineType == ROLE_GAME_ONLINE)
	{
		//zone发详细好友信息给pstPlayer
		m_cmd_to_zone(pstPlayer->stRoleData.szRoleName,pstOtherMember->RoleName,SS_CMD_FRIEND_DETAIL,NULL);
	}
	else
	{
		pstOther = mud_name_player(pstOtherMember->RoleName);
		if (pstOther)
		{
			m_friend_detail(pstOther,pstPlayer->stRoleData.szRoleName,iOnlineType);
		}
	}

	return 0;
}

int m_get_buddy_member(MudPlayer* pstPlayer,ROLEBUDDYMEMBER* pstBuddyMember,BUDDYINFO *pstBuddyInfo)
{
	assert(pstBuddyMember);
    memset(pstBuddyMember,0,sizeof(*pstBuddyMember));

	pstBuddyMember->Gender = pstPlayer->stRoleData.iGender;
	pstBuddyMember->RelationType = RELATION_TYPE_FRIEND;
	pstBuddyMember->RoleID = pstPlayer->stRoleData.ullRoleID;
	STRNCPY(pstBuddyMember->RoleName,pstPlayer->stRoleData.szRoleName,CCH(pstBuddyMember->RoleName));

	if (pstBuddyInfo)
	{
		pstBuddyInfo->RoleID = pstPlayer->stRoleData.ullRoleID;
		pstBuddyInfo->Head = pstPlayer->stRoleData.iHead;
		pstBuddyInfo->Career = pstPlayer->stRoleData.iCareer;
		pstBuddyInfo->Level = pstPlayer->stRoleData.iLevel;
		STRNCPY(pstBuddyInfo->Emotion, pstPlayer->stRoleData.Emotion, sizeof(pstBuddyInfo->Emotion));
		pstBuddyInfo->ClanID = pstPlayer->stRoleData.stClanInfo.Id;
		STRNCPY(pstBuddyInfo->ClanName, pstPlayer->stRoleData.stClanInfo.Name, sizeof(pstBuddyInfo->ClanName));
		pstBuddyInfo->Face = pstPlayer->stRoleData.iFace;
		pstBuddyInfo->Hair = pstPlayer->stRoleData.iHair;
	}
	
	return 0;
}



int m_relation_addfriend_res(MudPlayer* pstPlayer,CSADDFRIENDRES* pstRes, ROLEBUDDYMEMBER *pstBuddyMember)
{
	MudPlayer* pstOther = NULL;
	ROLEBUDDYMEMBER stTmpMember;
	SSINFOGETREQ stInfoGetReq;
	SSCMDDATA stData;
	// B(pstPlayer)同意A加自己为好友,


	if (0 == pstRes->ReqName[0])
	{
		return -1;
	}
	
	if (pstRes->OnlineType == ROLE_MUD_ONLINE)
	{
		pstOther = mud_name_player(pstRes->ReqName);
		if (pstOther)
		{
			pstOther->szAddFriendName[0] = 0;
		}
	}
	
	if (pstRes->AddOther)
	{
		char szMsg[128];

		sprintf(szMsg, "%s 拒绝了你的好友申请", pstPlayer->stRoleData.szRoleName);
		m_send_tip_by_name(szMsg, pstRes->ReqName,pstRes->OnlineType);

		return 0;
	}
	
	if (0 == pstRes->Accept)
	{
		m_reject_add(pstPlayer,pstRes);
		
		return 0;
	}

	if (pstRes->OnlineType == ROLE_GAME_ONLINE && !pstBuddyMember)
	{
		stInfoGetReq.ReqBody.BuddyMember.AddFriendRes = *pstRes;
		STRNCPY(stInfoGetReq.ReqBody.BuddyMember.Name,pstPlayer->stRoleData.szRoleName,
			CCH(stInfoGetReq.ReqBody.BuddyMember.Name));
		
		m_info_get_to_zone(&stInfoGetReq, INFO_GET_BUDDY_MEMBER);
		return 0;
	}

	if (pstOther)
	{
		pstBuddyMember = &stTmpMember;
		m_get_buddy_member(pstOther,pstBuddyMember,NULL);
	}

	if ( !pstBuddyMember)
	{
		return -1;
	}

	m_friend_add(pstPlayer,pstBuddyMember,pstRes->OnlineType) ;

	pstBuddyMember = &stTmpMember;
	m_get_buddy_member(pstPlayer,pstBuddyMember,NULL);
	if (pstOther && pstBuddyMember)
	{
		m_friend_add(pstOther, pstBuddyMember, ROLE_MUD_ONLINE);
	}
	else
	{
		//让zone中的other 加player为好友
		m_get_buddy_member(pstPlayer,&stData.FriendAdd.BuddyMember,&stData.FriendAdd.BuddyInfo);
		m_cmd_to_zone(pstPlayer->stRoleData.szRoleName, pstRes->ReqName,SS_CMD_FRIEND_ADD, &stData);
	}
	
	return 0;
}

int m_send_emotion(MudPlayer* pstPlayer)
{
	CSPKG* pstPkg = &this_env()->stCSRes;
	CSRELATIONSVR *pstRelationSvr = &pstPkg->Body.RelationSvr;
	CSEMOTIONRES *pstEmotionRes = &pstRelationSvr->RelationSvrData.EmotionRes;

	pstRelationSvr->OpType = RELATION_OP_TYPE_EMOTION_RES;
	memcpy(pstEmotionRes->Emotion, pstPlayer->stRoleData.Emotion, sizeof(pstEmotionRes->Emotion));
	Z_CSHEAD_INIT(&pstPkg->Head, RELATION_SVR);
	mud_cltmsg_send(pstPlayer,pstPkg);
	return 0;
}

int m_relation_emotion(MudPlayer* pstPlayer,CSRELATIONCLT *pstClt)
{
	CSEMOTIONREQ* pstReq = &pstClt->RelationCltData.EmotionReq;

	
	if( CheckDirty(pstReq->Emotion, 0xf) & 0xf )
	{
		mud_sys_strf(pstPlayer, SYS_MIDDLE2,LJY_MUD17);
		return -1;
	}

	STRNCPY(pstPlayer->stRoleData.Emotion,pstReq->Emotion,CCH(pstPlayer->stRoleData.Emotion));
	pstPlayer->stRoleData.EmotionAlterTime = this_env()->pstAppCtx->stCurr.tv_sec;
	pstPlayer->iBuddyDirty = 1;


	
	m_send_emotion(pstPlayer);
	return 0;
}

int m_relation_delrelation_req(MudPlayer* pstPlayer,CSRELATIONCLT *pstClt)
{
	CSDELRELATIONREQ* pstReq = &pstClt->RelationCltData.DelRelationReq;
	
	int iIdx;

	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONDELRES *pstRelationDelRes = &pstRelationSvr->RelationSvrData.RelationDelRes;  
	char *pszBuff = LJY_MUD18;

	pstRelationSvr->OpType = RELATION_OP_TYPE_DELRELATION_RES;
	
	if (pstReq->Type ==  RELATION_TYPE_FRIEND)
	{
		iIdx = m_relation_find_by_id(pstPlayer,pstReq->RoleID,RELATION_TYPE_FRIEND);
		if (0 > iIdx)
		{
			mud_sys_strf(pstPlayer, SYS_TIP, pszBuff);
			return -1;
		}

		pstPlayer->stRoleData.iFriendNum--;
		if (iIdx < pstPlayer->stRoleData.iFriendNum)
		{
			memcpy(&pstPlayer->stRoleData.astFriends[iIdx], 
				&pstPlayer->stRoleData.astFriends[pstPlayer->stRoleData.iFriendNum],
				sizeof(pstPlayer->stRoleData.astFriends[0]));
		}

	
		pstRelationDelRes->Type = RELATION_TYPE_FRIEND;
		pstRelationDelRes->RoleID = pstReq->RoleID;
			
		Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
		mud_cltmsg_send(pstPlayer, &stPkg);
		
		pstPlayer->iBuddyDirty = 1;

		return 0;
	}	
	
	return -1;

}

int m_relation_info_req(MudPlayer* pstPlayer,CSRELATIONCLT *pstClt)
{
	CSBUDDYINFOREQ* pstReq = &pstClt->RelationCltData.BuddyInfoReq;

	if (pstReq->OnlineType == ROLE_GAME_ONLINE)
	{
		//zone发详细好友信息给pstPlayer
		m_cmd_to_zone(pstPlayer->stRoleData.szRoleName,pstReq->ReqName,SS_CMD_FRIEND_DETAIL,NULL);
	}
	else
	{
		MudPlayer* pstOther = mud_name_player(pstReq->ReqName);
		if (pstOther)
		{
			m_friend_detail(pstOther,pstPlayer->stRoleData.szRoleName, ROLE_MUD_ONLINE);
		}
	}
	return 0;


}

int m_cltmsg_send_by_name(const CSPKG *pstCSPkgRes,char* pszName, int iOnlineType)
{
	if (ROLE_GAME_ONLINE == iOnlineType)
	{
		m_pkg_to_zone((CSPKG*)pstCSPkgRes,pszName,PKG_TRANS_PLAYER,0);
	}
	else
	{
		MudPlayer* pstPlayer = mud_name_player(pszName);
		if (pstPlayer)
		{
			mud_cltmsg_send(pstPlayer,pstCSPkgRes);
		}
	}
	return 0;
}

int m_send_tip_by_name(char *pszMsg, char* pszReceiver,int iOnlineType)
{
	CSPKG *pstPkg = &this_env()->stCSRes;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;

	pstSysMsg->Type = SYS_MIDDLE2;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
	STRNCPY(pstSysMsg->Msg, pszMsg, sizeof(pstSysMsg->Msg));

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	if (iOnlineType == ROLE_GAME_ONLINE)
	{
		if (*pszReceiver != 0)
		{
			m_pkg_to_zone(pstPkg,pszReceiver,PKG_TRANS_PLAYER,0);
		}
	}
	else
	{
		MudPlayer* pstPlayer = mud_name_player(pszReceiver);
		if (pstPlayer)
		{
			mud_cltmsg_send(pstPlayer,pstPkg);
		}
	}
	return 0;
}

int m_relation_clt(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                 TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSRELATIONCLT *pstRelationClt = &pstCsPkg->Body.RelationClt;
	//CSADDRELATIONDATA *pstAddRelationData = &pstRelationClt->RelationCltData.AddRelationReq.AddRelationData;	//comment by paraunused.pl

	switch(pstRelationClt->OpType)
	{
	case RELATION_OP_TYPE_ADDRELATION_REQ:
		if (pstRelationClt->RelationCltData.AddRelationReq.Type == RELATION_TYPE_FRIEND)
		{
			m_relation_addfriend_req(pstPlayer, pstRelationClt);
		}		
		break;
	case RELATION_OP_TYPE_ADDFRIEND_RES:
		m_relation_addfriend_res( pstPlayer,&pstRelationClt->RelationCltData.AddFriendRes,NULL);
		break;
	case RELATION_OP_TYPE_EMOTION_REQ:
		m_relation_emotion( pstPlayer, pstRelationClt);
		break;
	case RELATION_OP_TYPE_DELRELATION_REQ:
		m_relation_delrelation_req( pstPlayer, pstRelationClt);
		break;
	case RELATION_OP_TYPE_INFO_REQ:
		m_relation_info_req(pstPlayer, pstRelationClt);
		break;
	default:
		return -1;
		break;
	}
		
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

int watch_player_clt(MUD_SVRENV *pstEnv, MudPlayer *pstPlayer,
                 TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSPKG *pstResPkg = &pstEnv->stCSRes;
	CSWATCHFRIENDCLT *pstReq = &pstCsPkg->Body.WatchFriendClt;
	CSWATCHPLAYERSVR *pstRes = &pstResPkg->Body.WatchPlayerSvr;
	MudPlayer* pstTarget = mud_name_player(pstReq->FriendName);

	if (!pstTarget)
	{
		mud_sys_strf(pstPlayer, SYS_TIP, LJY_MUD4);
		return -1;
	}

	pstRes->PlayerMemID = 0;
	pstRes->RoleLevel = pstTarget->stRoleData.iLevel;
	pstRes->RoleGender = pstTarget->stRoleData.iGender;
	pstRes->RoleCareer = pstTarget->stRoleData.iCareer;
	pstRes->RoleHair = pstTarget->stRoleData.iHair;
	pstRes->RoleHead = pstTarget->stRoleData.iHead;
	pstRes->RoleFace = pstTarget->stRoleData.iFace;
	STRNCPY(pstRes->RoleName, pstTarget->stRoleData.szRoleName,
		      CCH(pstRes->RoleName));
	pstRes->RoleWear = pstTarget->stRoleData.stWear;
	pstRes->RoleAttrVal = pstTarget->stRoleData.AttrVal;

	//基础数值白色字参考标准
	pstRes->RoleBaseAttrVal = pstTarget->stRoleData.BaseAttrVal;

	pstRes->RoleMiscFlag = pstTarget->stRoleData.RoleMiscFlag;


	Z_CSHEAD_INIT(&pstResPkg->Head, WATCH_PLAYER_SVR);
	mud_cltmsg_send(pstPlayer, pstResPkg);
	
	return 0;
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

int m_pkg_to_zone(CSPKG *pstPkg,char *pszName,int iTransType,unsigned int uiClanID)
{
	SSPKG * pstSSPkg = &this_env()->stSSRes;
	SSPKGTRANS* pstTrans = &pstSSPkg->Body.PkgTansInfo;

	if (!IsTransPkgCmd(pstPkg->Head.Cmd))
	{
		tlog_error(this_env()->pstLogCat, 0, 0, "can't trans cmd=%d type=%d to zone_svr",
			pstTrans->Cmd, iTransType);
		return -1;
	}

	memset(pstTrans,0,sizeof(*pstTrans));
	
	pstTrans->Cmd = pstPkg->Head.Cmd;
	pstTrans->Type = iTransType;
	pstTrans->ClanID = uiClanID;

	if (pszName)
	{
		STRNCPY(pstTrans->szName, pszName, CCH(pstTrans->szName));
	}
	memcpy(&pstTrans->Body,&pstPkg->Body,sizeof(pstTrans->Body));

	Z_SSHEAD_INIT(&pstSSPkg->Head, SS_PKG_TRANS);
	mud_ssmsg_send(pstSSPkg, this_env()->iZoneSvrID);	

	return 0;
}

int m_info_get_to_zone(SSINFOGETREQ* pstReq, int iType)
{
	SSPKG * pstSSPkg = &this_env()->stSSRes;
	
	pstSSPkg->Body.InfoGetReq = *pstReq;
	pstSSPkg->Body.InfoGetReq.Type = iType;
	
	Z_SSHEAD_INIT(&pstSSPkg->Head, SS_INFO_GET_REQ);
	mud_ssmsg_send(pstSSPkg, this_env()->iZoneSvrID);
	return 0;
}


int m_cmd_to_zone(char *pszReceiver,char *pszName,int iCmd, SSCMDDATA* pstData)
{
	SSPKG * pstSSPkg = &this_env()->stSSRes;
	SSCMDREQ* pstReq = &pstSSPkg->Body.CmdReq;


	memset(pstReq,0,sizeof(*pstReq));
	pstReq->Cmd = iCmd;
	
	if (pszReceiver)
	{
		STRNCPY(pstReq->szReceiver,pszReceiver,CCH(pstReq->szReceiver));
	}

	if (pszName)
	{
		STRNCPY(pstReq->szName,pszName,CCH(pstReq->szName));
	}
	
	if (pstData)
	{
		pstReq->Data = *pstData;
	}
	

	Z_SSHEAD_INIT(&pstSSPkg->Head, SS_CMD_REQ);
	mud_ssmsg_send(pstSSPkg, this_env()->iZoneSvrID);
	return 0;
}

