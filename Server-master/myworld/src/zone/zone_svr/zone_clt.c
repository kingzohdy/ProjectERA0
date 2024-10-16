/*
**  @file $RCSfile: zone_clt.c,v $
**  general description of this module
**  $Id: zone_clt.c,v 1.142 2014/03/14 13:29:16 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2014/03/14 13:29:16 $
**  @version $Revision: 1.142 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_clt.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "role_db_meta.h"
#include "zone_player.h"
#include "tconnapi.h"
#include "zone_err.h"
#include "zone_team.h"
#include "zone_package.h"
#include "zone_map.h"
#include "comm_misc.h"
#include "entity.h"
#include "zone_spansvr.h"
#include "zone_span.h"

//iBattleSvrBusID为0表示设置回bus状态
int z_set_routing_req(ZONESVRENV* pstEnv, int iConnIdx, int iNewID,
                      int iZoneSvrBusID, int iZoneConnBusID,int iBattleSvrBusID,
                      const void *pvExtraData, int iExtraDataLen)
{
	/*TFRAMEHEAD stReq;

	memset(&stReq, 0, sizeof(stReq));
	stReq.Cmd = TFRAMEHEAD_CMD_SETROUTING;
	stReq.ID = 0x7fffffff; // 填充一个不可能的值,但又不能是-1
	stReq.ConnIdx = iConnIdx;
 	stReq.CmdData.SetRouting.ZoneBusID = iZoneSvrBusID;
	stReq.CmdData.SetRouting.BattleBusID = iBattleSvrBusID;
 	stReq.CmdData.SetRouting.ID = iNewID;

 	if(pvExtraData && iExtraDataLen > 0)
 	{
 		stReq.CmdData.SetRouting.ExtraDataLen = iExtraDataLen;
 		memcpy(stReq.CmdData.SetRouting.ExtraData, pvExtraData, iExtraDataLen);
 	}


	tlog_info(pstEnv->pstLogCat, 0, 0, "z_set_routing_req iConnIdx=%d,iNewID=%d",iConnIdx, iNewID);

 	return tconnapi_send(pstEnv->iTconnapiHandle, iZoneConnBusID, NULL, 0,
 	                     &stReq);
 	                     */
 	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(iConnIdx);	//add by paraunused.pl
	UNUSED(iNewID);	//add by paraunused.pl
	UNUSED(iZoneSvrBusID);	//add by paraunused.pl
	UNUSED(iZoneConnBusID);	//add by paraunused.pl
	UNUSED(iBattleSvrBusID);	//add by paraunused.pl
	UNUSED(pvExtraData);	//add by paraunused.pl
	UNUSED(iExtraDataLen);	//add by paraunused.pl
}

int z_set_routing_res(ZONESVRENV* pstEnv, const TFRAMEHEAD *pstRes)
{
	/*
	Player *pstPlayer =NULL;
	int Uin = 0;

	if(	pstRes->CmdData.SetRouting.ID >=0 &&
		pstRes->CmdData.SetRouting.ExtraDataLen > 0)
	{
		int  iOffset = 0;
		int  iCmd = 0;
		memcpy(&iCmd,pstRes->CmdData.SetRouting.ExtraData+iOffset,sizeof(iCmd));
		iOffset += sizeof(iCmd);
		memcpy(&Uin,pstRes->CmdData.SetRouting.ExtraData+iOffset,sizeof(Uin));
		iOffset += sizeof(Uin);

		if(iCmd == 1)
		{
			pstPlayer = player_get_by_uin(pstEnv, Uin);
			if( pstPlayer == NULL )
			{
				return -1;
			}
			player_battle_enter_map_req(pstEnv->pstAppCtx,pstEnv,pstPlayer);
		}
		else if(iCmd == 2)
		{
			
		}
		else if(iCmd == 3)
		{
			
		}
		tlog_info(pstEnv->pstLogCat, 0, 0, "z_set_routing_res cmd=%d,connidx:%d",iCmd,pstRes->CmdData.SetRouting.ID);
	}
	*/
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstRes);	//add by paraunused.pl
}

int z_set_limits_req(ZONESVRENV* pstEnv, int iZoneConnBusID, int fEnabled,
                     int iParam)
{
	TFRAMEHEAD stReq;

	memset(&stReq, 0, sizeof(stReq));
	stReq.Cmd = TFRAMEHEAD_CMD_SETLIMITS;
	stReq.ID = 0;
	stReq.ConnIdx = 0;
 	stReq.CmdData.SetLimits.Enabled = fEnabled;
 	stReq.CmdData.SetLimits.Param = iParam;

 	return tconnapi_send(pstEnv->iTconnapiHandle, iZoneConnBusID, NULL, 0,
 	                     &stReq);
}

int cmd_stat_recv(ZONESVRENV* pstEnv, CSPKG *pstPkg, int iMsgLen)
{
	int i;
	RECVCLTMSGDETAIL *pstDetail = &pstEnv->pstRunStat->RecvCltMsgDetail;
	
	for (i=0; i<pstDetail->RecvCmdNum; i++)
	{
		if (pstPkg->Head.Cmd == pstDetail->RecvCmd[i].Cmd)
		{
			pstDetail->RecvCmd[i].Num++;
			pstDetail->RecvCmd[i].Bytes += iMsgLen;
			return 0;
		}
	}

	if (pstDetail->RecvCmdNum >= MAX_RECV_CMD)
	{
		return -1;
	}

	pstDetail->RecvCmd[pstDetail->RecvCmdNum].Cmd = pstPkg->Head.Cmd;
	pstDetail->RecvCmd[pstDetail->RecvCmdNum].Bytes = iMsgLen;
	pstDetail->RecvCmd[pstDetail->RecvCmdNum].Num = 1;
	pstDetail->RecvCmdNum++;
	
	return 0;
}

int subcmd_stat_send(SENDCMD *pstSendCmd, CSPKG *pstPkg, int iMsgLen, int iGroupNum)
{
	int i;
	int iSubCmd = -1;

	switch(pstPkg->Head.Cmd)
	{
	case ACTION_RES:
		iSubCmd = pstPkg->Body.ActionRes.ActionID;
		break;
	case BATTLE_LIMIT_INFO:
		iSubCmd = pstPkg->Body.BattleLimit.Type;
		break;
	case ACTIVE_GAME_SVR:
		iSubCmd = pstPkg->Body.ActiveGameSvr.Type;
		break;
	case STRONG_SVR:
		iSubCmd = pstPkg->Body.StrongSvr.Cmd;
		break;
	case PLOY_FB_INFO_SVR:
		iSubCmd = pstPkg->Body.PolyFBInfo.Type;
		break;
	default:
		break;
	}

	if (iSubCmd >= 0)
	{
		for (i = 0; i<pstSendCmd->SubCmdNum; i++)
		{
			if (iSubCmd == pstSendCmd->SubCmd[i].Cmd)
			{
				pstSendCmd->SubCmd[i].Num += iGroupNum;
				pstSendCmd->SubCmd[i].Bytes += iMsgLen*iGroupNum;
				
				return 0;
			}
		}

		if (pstSendCmd->SubCmdNum >= MAX_SEND_SUB_CMD)
		{
			return -1;
		}

		pstSendCmd->SubCmd[pstSendCmd->SubCmdNum].Cmd = iSubCmd;
		pstSendCmd->SubCmd[pstSendCmd->SubCmdNum].Num = iGroupNum;
		pstSendCmd->SubCmd[pstSendCmd->SubCmdNum].Bytes = iMsgLen*iGroupNum;

		pstSendCmd->SubCmdNum++;
	}
	
	return 0;
}

int cmd_stat_send(ZONESVRENV* pstEnv, CSPKG *pstPkg, int iMsgLen, int iGroupNum)
{
	int i;
	SENDCLTMSGDETAIL *pstDetail = &pstEnv->pstRunStat->SendCltMsgDetail;

	pstEnv->pstRunStat->SendCltMsg += iGroupNum;
	pstEnv->pstRunStat->SendCltBytes += iGroupNum*iMsgLen;
	
	for (i=0; i<pstDetail->SendCmdNum; i++)
	{
		if (pstPkg->Head.Cmd == pstDetail->SendCmd[i].SendCmd.Cmd)
		{
			pstDetail->SendCmd[i].SendCmd.Num += iGroupNum;
			pstDetail->SendCmd[i].SendCmd.Bytes += iMsgLen*iGroupNum;

			subcmd_stat_send(&pstDetail->SendCmd[i], pstPkg, iMsgLen, iGroupNum);
			return 0;
		}
	}

	if (pstDetail->SendCmdNum >= MAX_SEND_CMD)
	{
		return -1;
	}

	pstDetail->SendCmd[pstDetail->SendCmdNum].SendCmd.Cmd = pstPkg->Head.Cmd;
	pstDetail->SendCmd[pstDetail->SendCmdNum].SendCmd.Bytes = iMsgLen*iGroupNum;
	pstDetail->SendCmd[pstDetail->SendCmdNum].SendCmd.Num = iGroupNum;
	pstDetail->SendCmdNum++;

	subcmd_stat_send(&pstDetail->SendCmd[pstDetail->SendCmdNum], pstPkg, iMsgLen, iGroupNum);
	
	return 0;
}


int z_clt_msg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, char *pszMsg, int iMsgLen, TFRAMEHEAD *pstFrameHead)
{
	int iVersion = 0;
	Player *pstPlayer = NULL;
	CmdMap *pstCmdMap;
	TMEMBLOCK *pstMemBlock;
	int iCmd;
	int iRet;

	if (pstEnv->cShutdown > 1)
	{
		return 0;
	}

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
			tlog_error(pstEnv->pstLogCat, 0, 0, "frame head id=%d", pstFrameHead->ID);
			return -1;
		}

		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, pstFrameHead->ID);
		if (pstMemBlock->fValid && pstFrameHead->ID == pstMemBlock->iIdx)
		{
			pstPlayer = (Player *)pstMemBlock->szData;
		}
		else
		{
			pstPlayer = NULL;
		}

		if (pstPlayer && PLAYER_STATUS_ACC_NOLOGIN < pstPlayer->eStatus)
		{
			iVersion = pstPlayer->unCltProVer;
		}
		break;
	case TFRAMEHEAD_CMD_STOP:
		//tlog_info(pstEnv->pstLogCat, 0, 0, "TFRAMEHEAD_CMD_STOP ConnIdx:%d",pstFrameHead->ConnIdx);
		return 	player_close_connection(pstAppCtx, pstEnv, pstFrameHead);
		break;
	case TFRAMEHEAD_CMD_SETROUTING:
		return z_set_routing_res(pstEnv, pstFrameHead);
	default:
		break;
	}/*switch(pstFrameHead->chCmd)*/

	iRet = net_cs_unpack(pstEnv->pstNetHan, pszMsg, iMsgLen, &pstEnv->stCSReq, iVersion);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "cs unpack fail %d, version=%d, cmd=%d", 
			iRet, iVersion, NTOHS(*(uint16_t *)(pszMsg+2)));
		return -1;
	}

	if (pstEnv->pstConf->PackStat)
	{
		cmd_stat_recv(pstEnv, &pstEnv->stCSReq, iMsgLen);
	}

	if (pstPlayer)
	{
		pstPlayer->tActive = pstAppCtx->stCurr.tv_sec;
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "c-->s player %d", pstPlayer->stRoleData.Uin);
	}

	tlog_debug_dr(pstEnv->pstLogCat, 0, 0, pstEnv->pstNetHan->pstCSProtoPkgMeta, 
					&pstEnv->stCSReq, sizeof(pstEnv->stCSReq), 0);

	assert(pstEnv->stCSReq.Head.Cmd < MAX_CS_CMD);

	iCmd = pstEnv->stCSReq.Head.Cmd;
	pstCmdMap = &pstEnv->astCmdMap[iCmd];

	if (pstPlayer && 0 <= pstCmdMap->cStateLimit && pstPlayer->eStatus != pstCmdMap->cStateLimit)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "recv clt uin=%d cmd=%d state=%d",
					pstPlayer->stRoleData.Uin, pstEnv->stCSReq.Head.Cmd, pstPlayer->eStatus);
		return -1;
	}

	if (pstCmdMap->cStateLimit == PLAYER_STATUS_ROLE_LOGIN && NULL == pstPlayer)
	{
		tlog_info(pstEnv->pstLogCat, 0, 0, "player null");
		return -1;
	}

	if (pstPlayer && pstPlayer->stOnline.bWaitRoleLogout)
	{
		switch( iCmd )
		{
		case ACC_LOGIN_REQ:
		case ROLE_LOGIN_REQ:
		case HEARTBEAT_REQ:
		case CANCEL_ROLE_LOGOUT:
		case CHAT_REQ:
		case AUTO_FLY_CLT:
		case TEAM_CLT:
		case RELATION_CLT:	
			break;

		default:
			return -1;
			break;
		}
	}

	//control cmd handle by flag
	if (pstCmdMap->cCtrlFlag)
	{
		/*if (pstPlayer)
		{
			pstPlayer->tActive = pstAppCtx->stCurr.tv_sec;
		}*/

		pstFrameHead->TimeStamp.TimeVal.sec += pstAppCtx->iAdjustTime;
		if(	pstPlayer)
		{
			pstPlayer->stOnline.stFrameHead = *pstFrameHead;
		}
		return pstCmdMap->pfnCmdHandle(pstAppCtx, pstEnv, pstPlayer, pstFrameHead, &pstEnv->stCSReq);
	}
	else
	{
		return -1;
	}

	return 0;
}

int z_clt_msg_filter_item(ZONESVRENV *pstEnv, ROLEITEM *pstRoleItem)
{
	ITEMDEF *pstItemDef;

	pstItemDef = z_get_itemdef_roleitem(pstEnv, pstRoleItem);
	if (pstItemDef && RESULT_TREASURE_PIC == pstItemDef->Result[0].ResultID)
	{
		pstRoleItem->InstVal2 = 0;
		pstRoleItem->InstVal3 = 0;
	}
	
	return 0;
}

int z_clt_msg_filter_grid(ZONESVRENV *pstEnv, ROLEGRID *pstRoleGrid)
{
	switch(pstRoleGrid->Type)
	{
		case GRID_ITEM_ITEM:
			z_clt_msg_filter_item(pstEnv, &pstRoleGrid->GridData.RoleItem);
			break;
		default:
			break;
	}
	
	return 0;
}

int z_clt_msg_filter(ZONESVRENV *pstEnv, CSPKG *pstCSPkgRes, Player *pstPlayer)
{
	int i;
	
	CSPACKAGEINFO *pstPackageInfo;
	ROLEPACKAGE *pstPackageAll;
	ROLEPACKAGECHG *pstPackageChg;
	CSBOOTYINFO *pstBooty;
	CSSHOPPINGRES *pstShoppingSvr = NULL;
	CSMAILSVR *pstMailSvr;
	MAIL *pstMail;

	CSEXCHGSVR *pstExchgSvr;
		
	switch(pstCSPkgRes->Head.Cmd)
	{
		case PACKAGE_INFO:
			pstPackageInfo = &pstCSPkgRes->Body.PackageInfo;
			switch(pstPackageInfo->Type)
			{
				case PACKAGE_INFO_ALL:
					pstPackageAll = &pstPackageInfo->PackageInfoData.PackageAll;
					for (i=0; i<pstPackageAll->SundriesPackage.Num; i++)
					{
						z_clt_msg_filter_grid(pstEnv, &pstPackageAll->SundriesPackage.RoleGrids[i]);
					}
					
					break;

				case PACKAGE_INFO_CHG:
					pstPackageChg = &pstPackageInfo->PackageInfoData.PackageChg;
					for (i=0; i<pstPackageChg->ItemChg.ChgNum; i++)
					{
						z_clt_msg_filter_item(pstEnv, &pstPackageChg->ItemChg.Items[i].Item);
					}
						
					break;

				case PACKAGE_INFO_BOOTY:
					pstBooty = &pstPackageInfo->PackageInfoData.PackageBooty;
					for (i=0; i<pstBooty->Num; i++)
					{
						z_clt_msg_filter_grid(pstEnv, &pstBooty->Items[i]);
					}
					break;
					
				default:
					break;
			}
			break;
		case MAIL_SVR:
			pstMailSvr = &pstCSPkgRes->Body.MailSvr;
			switch(pstMailSvr->Operation)
			{
				case MAIL_SVR_CONTENT:
					pstMail = &pstMailSvr->MailSvrData.MailContent;
					for (i=0; i<pstMail->Detail.ItemNum; i++)
					{
						z_clt_msg_filter_grid(pstEnv, &pstMail->Detail.Items[i]);
					}
					break;
					
				default:
					break;
			}
				
			break;
		case EXCHG_SVR:
			pstExchgSvr = &pstCSPkgRes->Body.ExchgSvr;
			for (i=0; i<pstExchgSvr->ExchgEntiy.ItemNum; i++)
			{
				z_clt_msg_filter_grid(pstEnv, &pstExchgSvr->ExchgEntiy.ItemEntiy[i].RoleGrid);
			}
			break;
		case SHOPPING_SVR:
			pstShoppingSvr = &pstCSPkgRes->Body.ShoppingSvr;
			if( SHOPPING_QUERY_TIANYUAN_RES == pstShoppingSvr->Type )
			{
				pstShoppingSvr->ShoppingResData.ShoppingQueryTianYuanRes.Amount = 0;
			}
			break;
		case ACTION_RES:
			if (pstPlayer && 
				PLAYER_STATUS_ROLE_ON_BATTLE == pstPlayer->eStatus &&
				pstPlayer->iMemID == pstCSPkgRes->Body.ActionRes.ID &&
				pstPlayer->iBattleMemID)
			{
				pstCSPkgRes->Body.ActionRes.ID = pstPlayer->iBattleMemID;
			}
			break;
		default:
			break;
	}
	
	return 0;
}
int z_get_player_connectid(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	if( INST_MODE_ZONE_SVR == pstEnv ||
		pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN ||
		(pstPlayer->stOnline.eStatusAssistant&PLAYER_STATUS_BATTLE_LOGIN_ON) )
	{
		return pstPlayer->stConnectInfo.iID;
	}
	else if(pstPlayer->stOnline.iZoneConnectID)
	{
		return pstPlayer->stOnline.iZoneConnectID;
	}
	else 
	{
		return pstPlayer->stConnectInfo.iID;
	}
}

int  z_cltmsg_send_trace(ZONESVRENV *pstEnv, Player *pstPlayer, CSPKG *pstCSPkgRes, const char *pszFile, int iLine)
{
	int iRet;
	TDRDATA stNet;
	
	if (!pstPlayer || pstPlayer->stOnline.bConnLost)
	{
		return -1;
	}

	z_clt_msg_filter(pstEnv, pstCSPkgRes,pstPlayer);
	
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = &pstEnv->szSendBuffer[0];
	iRet = net_cs_pack(pstEnv->pstNetHan, &stNet, pstCSPkgRes, pstPlayer->unCltProVer);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "From:%s(%d):uin=%d cmd=%d prover=%d cs pack fail ret=%d",
					pszFile, iLine, pstPlayer->stRoleData.Uin, pstCSPkgRes->Head.Cmd, pstPlayer->unCltProVer, iRet);
		return -1;
	}

	tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "uin=%d s-->c msg:headlen=%d,bodylen=%d", pstPlayer->stRoleData.Uin, ntohs(*(short*)(stNet.pszBuff+4)), ntohs(*(short*)(stNet.pszBuff+6)) );
	tlog_debug_dr(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, pstEnv->pstNetHan->pstCSProtoPkgMeta, 
					pstCSPkgRes, sizeof(CSPKG), pstPlayer->unCltProVer);

	pstEnv->stFrameHeadTemp.CmdData.InProc.Flag = BROADCAST_FLAG_NO;
	pstEnv->stFrameHeadTemp.Cmd = pstPlayer->stConnectInfo.chCmd;
	pstEnv->stFrameHeadTemp.ID = z_get_player_connectid(pstEnv,pstPlayer);//pstPlayer->stConnectInfo.iID;
	pstEnv->stFrameHeadTemp.ConnIdx = pstPlayer->stConnectInfo.iConnIdx;

	pstEnv->stFrameHeadTemp.PkgHeadCmd = pstCSPkgRes->Head.Cmd;

	if(!is_my_master_svr(pstEnv->pstAppCtx->iId, pstPlayer))
	{
		int iConnID = player_conn_busid_get(pstEnv, pstPlayer);
		iRet = tconnapi_send(pstEnv->iTconnapiHandle, iConnID, stNet.pszBuff, stNet.iBuff, &pstEnv->stFrameHeadTemp);
		if (0 > iRet)
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "uin=%d cmd=%d  tconnapi_send fail ret=%d",
						pstPlayer->stRoleData.Uin, pstCSPkgRes->Head.Cmd, iRet);
		}
		if(pstEnv->stFrameHeadTemp.ID == 0)
		{
			tlog_info(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "battle iConnIdx:%d,connid:%d", 
				pstPlayer->stConnectInfo.iConnIdx,
				iConnID);
		}

		if (pstEnv->pstConf->PackStat)
		{
			cmd_stat_send(pstEnv, pstCSPkgRes, stNet.iBuff, 1);
		}
	}
	else 
	{
		//角色接管， Login之前 过滤掉游戏内的七七八八的消息。
		if(pstPlayer->ucRoleLogoutReason == ROLE_LOGOUT_GRAB &&
			pstPlayer->stOnline.bWaitRoleLogout)
		{
			if(pstCSPkgRes->Head.Cmd > ACC_LOGOUT_RES &&
				pstCSPkgRes->Head.Cmd != SELECT_BIRTH_LOCATION_SVR)
			{
				return -1;
			}
		}
		iRet = tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iZoneConnID, stNet.pszBuff, stNet.iBuff, &pstEnv->stFrameHeadTemp);
		if (0 > iRet)
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "uin=%d cmd=%d  tconnapi_send fail ret=%d",
						pstPlayer->stRoleData.Uin, pstCSPkgRes->Head.Cmd, iRet);
		}

		if (pstEnv->pstConf->PackStat)
		{
			cmd_stat_send(pstEnv, pstCSPkgRes, stNet.iBuff, 1);
		}
	}
	
	return iRet; 
}


int z_cltmsg_send_stop(ZONESVRENV *pstEnv, Player *pstPlayer,  CSPKG *pstCSPkgRes)
{
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
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "uin=%d cmd=%d prover=%d cs pack fail ret=%d",
					pstPlayer->stRoleData.Uin, pstCSPkgRes->Head.Cmd, pstPlayer->unCltProVer, iRet);
		return -1;
	}

	tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "uin=%d s-->c stop msg:", pstPlayer->stRoleData.Uin);
	tlog_debug_dr(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, pstEnv->pstNetHan->pstCSProtoPkgMeta, 
					pstCSPkgRes, sizeof(CSPKG), pstPlayer->unCltProVer);

	//info tconnd stop
	pstEnv->stFrameHeadTemp.CmdData.InProc.Flag = BROADCAST_FLAG_NO;
	pstEnv->stFrameHeadTemp.Cmd = TFRAMEHEAD_CMD_STOP;
	pstEnv->stFrameHeadTemp.CmdData.Stop.Reason = STOP_REASON_LOGOUT;
	pstEnv->stFrameHeadTemp.ID = z_get_player_connectid(pstEnv,pstPlayer);//pstPlayer->stConnectInfo.iID;
	pstEnv->stFrameHeadTemp.ConnIdx = pstPlayer->stConnectInfo.iConnIdx;
	pstEnv->stFrameHeadTemp.PkgHeadCmd = pstCSPkgRes->Head.Cmd;

	//tlog_info(pstEnv->pstLogCat, 0, 0, "TFRAMEHEAD_CMD_STOP ConnIdx:%d",pstEnv->stFrameHeadTemp.ConnIdx);
	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		if( pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			int iConnID = player_conn_busid_get(pstEnv, pstPlayer);
			iRet = tconnapi_send(pstEnv->iTconnapiHandle, iConnID, stNet.pszBuff, stNet.iBuff, &pstEnv->stFrameHeadTemp);
		}
	}
	else
	{
		iRet = tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iZoneConnID, stNet.pszBuff, stNet.iBuff, &pstEnv->stFrameHeadTemp);
	}

	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "uin=%d cmd=%d	tconnapi_send fail ret=%d",
					pstPlayer->stRoleData.Uin, pstCSPkgRes->Head.Cmd, iRet);
	}

	pstPlayer->stOnline.bConnLost = 1;

	//恢复全局变量初始配置
	pstEnv->stFrameHeadTemp.CmdData.Stop.Reason = 0;
	
	return iRet; 
}


int z_cltmsg_broadcast_condition(ZONESVRENV *pstEnv, CSPKG *pstCSPkgRes, int (*condition)(ZONESVRENV *,Player *,void *))
{
	int i;
	Player *pstPlayer = NULL;
	TMEMBLOCK *pstMemBlock;
	int iRet;
	TDRDATA stNet;
	int iCount = 0;
	Player *apstPlayers[MAX_PLAYER];

//	UNUSED(pstAppCtx);

	//晕，这种方式无法做协议兼容，要求广播消息暂时不做版本兼容，先备忘
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = &pstEnv->szSendBuffer[0];
	iRet = net_cs_pack(pstEnv->pstNetHan, &stNet, pstCSPkgRes, 0);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_cltmsg_broadcast cmd=%d cs pack fail ret=%d",
					pstCSPkgRes->Head.Cmd, iRet);
		return -1;
	}


	//发到mud
	z_pkg_to_mud(pstEnv,pstCSPkgRes,NULL,0,PKG_TRANS_ALL);

	for (i=0; i<MAX_PLAYER; i++)
	{
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (Player *)pstMemBlock->szData;
		/*if( CHAT_RES == pstCSPkgRes->Head.Cmd &&
			PLAYER_STATUS_ROLE_ON_BATTLE == pstPlayer->eStatus)
		{
			
		}
		else*/ if ( !(*condition)(pstEnv,pstPlayer,NULL))
			{
				continue;
			}
		
		apstPlayers[iCount++] = pstPlayer;	
	}

	if (iCount > 0)
	{
		return z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
		                                iCount, pstCSPkgRes);
	}

	return 0;
}

int z_cltmsg_broadcast(TAPPCTX* pstAppCtx, ZONESVRENV *pstEnv, CSPKG *pstCSPkgRes)
{
	return z_cltmsg_broadcast_except(pstAppCtx, pstEnv, pstCSPkgRes, NULL);
}

int z_cltmsg_broadcast_except(TAPPCTX* pstAppCtx, ZONESVRENV *pstEnv, CSPKG *pstCSPkgRes, Player *pstPlayerExcept)
{
	int i;
	Player *pstPlayer = NULL;
	TMEMBLOCK *pstMemBlock;
	int iRet;
	TDRDATA stNet;
	int iCount = 0;
	Player *apstPlayers[MAX_PLAYER];

	UNUSED(pstAppCtx);

	//晕，这种方式无法做协议兼容，要求广播消息暂时不做版本兼容，先备忘
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = &pstEnv->szSendBuffer[0];
	iRet = net_cs_pack(pstEnv->pstNetHan, &stNet, pstCSPkgRes, 0);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_cltmsg_broadcast cmd=%d cs pack fail ret=%d",
					pstCSPkgRes->Head.Cmd, iRet);
		return -1;
	}


	//发到mud
	z_pkg_to_mud(pstEnv,pstCSPkgRes,NULL,0,PKG_TRANS_ALL);

	for (i=0; i<MAX_PLAYER; i++)
	{
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (Player *)pstMemBlock->szData;
		/*if( CHAT_RES == pstCSPkgRes->Head.Cmd &&
			PLAYER_STATUS_ROLE_ON_BATTLE == pstPlayer->eStatus)
		{
			
		}
		else*/ if ( 	((PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus) && 
					(PLAYER_STATUS_ROLE_NOTHING != pstPlayer->eStatus)) ||
					pstPlayer == pstPlayerExcept)
			{
				continue;
			}
		
		apstPlayers[iCount++] = pstPlayer;	
	}

	if (iCount > 0)
	{
		return z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
		                                iCount, pstCSPkgRes);
	}
	return 0;
}

TFRAMEHEAD *spansvr_broadcast_info_get(SpanSvrBroadcastGroup *pstBroadcastGroup, int iConnID, int iCmd)
{
	int i;

	for(i=0; i<pstBroadcastGroup->iNum; i++)
	{
		if(pstBroadcastGroup->astGroupInfo[i].iConnID == iConnID)
		{
			return &pstBroadcastGroup->astGroupInfo[i].stFrameHeadTemp;
		}
	}

	if(pstBroadcastGroup->iNum < MAX_SPANSVR_BROADCAST_GROUP)
	{
		TFRAMEHEAD *pstFrameHeadTemp;
		TFRAMECMDINPROC *pstInproc;
			
		pstBroadcastGroup->astGroupInfo[pstBroadcastGroup->iNum].iConnID = iConnID;

		pstFrameHeadTemp =  &pstBroadcastGroup->astGroupInfo[pstBroadcastGroup->iNum].stFrameHeadTemp;
		pstInproc = &pstFrameHeadTemp->CmdData.InProc;
		pstInproc->Flag = BROADCAST_FLAG_GROUP;
		
		pstFrameHeadTemp->ID = 0;
		pstFrameHeadTemp->ConnIdx = 0;
		pstFrameHeadTemp->Cmd = TFRAMEHEAD_CMD_INPROC;
		pstFrameHeadTemp->PkgHeadCmd = iCmd;
		pstInproc->NoEnc = 1;
		
		pstBroadcastGroup->iNum++;

		return &pstBroadcastGroup->astGroupInfo[pstBroadcastGroup->iNum-1].stFrameHeadTemp;
	}
	
	return NULL;
}

int z_spansvr_cltmsg_limit_broadcast(ZONESVRENV *pstEnv, Player **apstPlayer, int iNum, CSPKG *pstCSPkgRes)
{
	int i;
	int iConnID;
	Player *pstPlayer = NULL;
	TFRAMECMDINPROC *pstInproc;
	int iRet;
	TDRDATA stNet;
	int fSend = 0;
	SpanSvrBroadcastGroup stBroadcastGroup;
	TFRAMEHEAD *pstFrameHeadTemp;

	//晕，这种方式无法做协议兼容，要求广播消息暂时不做版本兼容，先备忘
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = &pstEnv->szSendBuffer[0];
	iRet = net_cs_pack(pstEnv->pstNetHan, &stNet, pstCSPkgRes, 0);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_cltmsg_broadcast cmd=%d cs pack fail ret=%d",
					pstCSPkgRes->Head.Cmd, iRet);
		return -1;
	}

	memset(&stBroadcastGroup, 0, sizeof(stBroadcastGroup));
	for (i=0; i<iNum; i++)
	{
		pstPlayer = apstPlayer[i];

		if(pstPlayer->stOnline.bConnLost)
		{
			continue;
		}
		
		{
			if((PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus && PLAYER_STATUS_ROLE_NOTHING != pstPlayer->eStatus)||
			   	pstPlayer->stOnline.bConnLost)
			{
				continue;
			}
		}

		iConnID = player_conn_busid_get(pstEnv, pstPlayer);
		pstFrameHeadTemp = spansvr_broadcast_info_get(&stBroadcastGroup, iConnID, pstCSPkgRes->Head.Cmd);
		if(!pstFrameHeadTemp)
		{
			z_cltmsg_send(pstEnv, pstPlayer,  pstCSPkgRes);	
			
			continue;
		}

		pstInproc = &pstFrameHeadTemp->CmdData.InProc;
		if (pstInproc->Count >= TFRAMEHEAD_MAX_BATCH_IDENT)
		{
			fSend = 1;

			iRet = tconnapi_send(pstEnv->iTconnapiHandle, iConnID, stNet.pszBuff, stNet.iBuff, pstFrameHeadTemp);
			if (0 > iRet)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "cmd=%d  tconnapi_send fail ret=%d",
							pstCSPkgRes->Head.Cmd, iRet);
			}

			if (pstEnv->pstConf->PackStat)
			{
				cmd_stat_send(pstEnv, pstCSPkgRes, stNet.iBuff, pstInproc->Count);
			}
			
			pstInproc->Count = 0;
		}
		
		pstInproc->Idents[pstInproc->Count].ConnIdx = pstPlayer->stConnectInfo.iConnIdx;
		pstInproc->Idents[pstInproc->Count].ID = pstPlayer->stConnectInfo.iID;
		pstInproc->Count++;
	}

	for(i=0; i<stBroadcastGroup.iNum; i++)
	{		
		pstFrameHeadTemp =  &stBroadcastGroup.astGroupInfo[i].stFrameHeadTemp;
		pstInproc = &pstFrameHeadTemp->CmdData.InProc;
		if (pstInproc->Count > 0)
		{
			fSend = 1;
			
			iRet = tconnapi_send(pstEnv->iTconnapiHandle, stBroadcastGroup.astGroupInfo[i].iConnID, stNet.pszBuff, stNet.iBuff, pstFrameHeadTemp);
			if (0 > iRet)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "cmd=%d  tconnapi_send fail ret=%d",
							pstCSPkgRes->Head.Cmd, iRet);
			}

			if (pstEnv->pstConf->PackStat)
			{
				cmd_stat_send(pstEnv, pstCSPkgRes, stNet.iBuff, pstInproc->Count);
			}
		}
	}

	if(fSend)
	{
		tlog_debug(pstEnv->pstLogCat, 0, 0, "s-->c limit %d broad cast msg:headlen=%d,bodylen=%d", iNum, ntohs(*(short*)(stNet.pszBuff+4)), ntohs(*(short*)(stNet.pszBuff+6)));
		tlog_debug_dr(pstEnv->pstLogCat, 0, 0, pstEnv->pstNetHan->pstCSProtoPkgMeta, 
						pstCSPkgRes, sizeof(CSPKG), 0);
	}

	return 0;
}

int z_cltmsg_limit_broadcast(TAPPCTX* pstAppCtx, ZONESVRENV *pstEnv, Player **apstPlayer, int iNum, CSPKG *pstCSPkgRes)
{
	int i;
	Player *pstPlayer = NULL;
	TFRAMECMDINPROC *pstInproc;
	int iRet;
	TDRDATA stNet;
	int fSend = 0;

	if (0 >= iNum)
	{
		return 0;
	}

	if(pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		return z_spansvr_cltmsg_limit_broadcast(pstEnv, apstPlayer, iNum, pstCSPkgRes);
	}

	//晕，这种方式无法做协议兼容，要求广播消息暂时不做版本兼容，先备忘
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = &pstEnv->szSendBuffer[0];
	iRet = net_cs_pack(pstEnv->pstNetHan, &stNet, pstCSPkgRes, 0);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_cltmsg_broadcast cmd=%d cs pack fail ret=%d",
					pstCSPkgRes->Head.Cmd, iRet);
		return -1;
	}

	pstInproc = &pstEnv->stFrameHeadTemp.CmdData.InProc;
	pstInproc->Flag = BROADCAST_FLAG_GROUP;
	pstEnv->stFrameHeadTemp.ID = 0;
	pstEnv->stFrameHeadTemp.ConnIdx = 0;
	pstEnv->stFrameHeadTemp.Cmd = TFRAMEHEAD_CMD_INPROC;
	pstEnv->stFrameHeadTemp.PkgHeadCmd = pstCSPkgRes->Head.Cmd;
	pstInproc->NoEnc = 1;
	pstInproc->Count = 0;

	for (i=0; i<iNum; i++)
	{
		pstPlayer = apstPlayer[i];
		if(pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
		{
			if( (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus &&
				PLAYER_STATUS_ROLE_NOTHING != pstPlayer->eStatus) ||
			   pstPlayer->stOnline.bConnLost)
			{
				continue;
			}
		}
		else if(pstEnv->iInstMode == INST_MODE_ZONE_SVR)
		{
			if((PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus &&
			    PLAYER_STATUS_ROLE_NOTHING != pstPlayer->eStatus) ||
			   pstPlayer->stOnline.bConnLost || pstPlayer->ucRoleLogoutReason == ROLE_LOGOUT_GRAB)
			{
				continue;
			}
		}
	
		
		if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
		{
			z_cltmsg_send(pstEnv, pstPlayer,  pstCSPkgRes);		
			continue;
		}

		if (pstInproc->Count >= TFRAMEHEAD_MAX_BATCH_IDENT)
		{
			fSend = 1;
			
			iRet = tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iZoneConnID, stNet.pszBuff, stNet.iBuff, &pstEnv->stFrameHeadTemp);
			if (0 > iRet)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "cmd=%d  tconnapi_send fail ret=%d",
							pstCSPkgRes->Head.Cmd, iRet);
			}

			if (pstEnv->pstConf->PackStat)
			{
				cmd_stat_send(pstEnv, pstCSPkgRes, stNet.iBuff, pstInproc->Count);
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
		
		iRet = tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iZoneConnID, stNet.pszBuff, stNet.iBuff, &pstEnv->stFrameHeadTemp);
		if (0 > iRet)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "cmd=%d  tconnapi_send fail ret=%d",
						pstCSPkgRes->Head.Cmd, iRet);
		}

		if (pstEnv->pstConf->PackStat)
		{
			cmd_stat_send(pstEnv, pstCSPkgRes, stNet.iBuff, pstInproc->Count);
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
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_send_sysmsg(TAPPCTX* pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer, int iSysType, int iErrno, ...)
{
	const char* pszErr;
	va_list marker;
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	
	va_start( marker, iErrno );
	pszErr	=	ZONE_GET_ERR(iErrno, 0);

	pstSysMsg->Type = iSysType;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
		
	if( !pszErr )
	{
		snprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), "SysNo : %d", iErrno );
	}
	else
	{
		vsnprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), pszErr, marker);
	}

	pstSysMsg->Msg[sizeof(pstSysMsg->Msg) - 1] = 0;
	va_end(marker);
	
	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkg);
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

//此函数不利于国际化，以后国际化版本再说
int z_send_sys_str(TAPPCTX* pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer, int iSysType, char *pszStr)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;

	pstSysMsg->Type = iSysType;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
	STRNCPY(pstSysMsg->Msg, pszStr, sizeof(pstSysMsg->Msg));

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkg);

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_send_sys_str_msg(TAPPCTX* pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer, int iSysType, int iErrno, ...)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	const char* pszErr;
	va_list marker;

	va_start( marker, iErrno );
	pszErr	=	ZONE_GET_ERR(iErrno, 0);

	pstSysMsg->Type = iSysType;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;

	if( !pszErr )
	{
		snprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), "SysNo : %d", iErrno );
	}
	else
	{
		vsnprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), pszErr, marker);
	}

	pstSysMsg->Msg[sizeof(pstSysMsg->Msg) - 1] = 0;
	va_end(marker);
	
	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkg);

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_send_sys_str_mud(TAPPCTX* pstAppCtx, ZONESVRENV *pstEnv, char *pszName, int iSysType, int iErrno, ...)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	const char* pszErr;
	va_list marker;

	va_start( marker, iErrno );
	pszErr	=	ZONE_GET_ERR(iErrno, 0);

	if (!pszName)
	{
		return -1;
	}
	pstSysMsg->Type = iSysType;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;

	if( !pszErr )
	{
		snprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), "SysNo : %d", iErrno );
	}
	else
	{
		vsnprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), pszErr, marker);
	}

	pstSysMsg->Msg[sizeof(pstSysMsg->Msg) - 1] = 0;
	va_end(marker);

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);

	z_pkg_to_mud(pstEnv,pstPkg,pszName,0,PKG_TRANS_PLAYER);

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_sys_strf( ZONESVRENV *pstEnv, Player *pstPlayer, int iSysType, const char *pszStr, ...)
{
	char szMsg[SYS_MSG_LEN];
	va_list valist;
	
	va_start( valist, pszStr );
	vsnprintf(szMsg, sizeof(szMsg), pszStr, valist);
	szMsg[SYS_MSG_LEN -1] = 0;
	va_end(valist);

	z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, iSysType, szMsg);

	return 0;
}

int z_ssmsg_send(ZONESVRENV *pstEnv, SSPKG *pstSSPkg, int iDstID)	
{
	TAPPCTX* pstAppCtx = pstEnv->pstAppCtx;
	int iSrcID;
	TDRDATA szNet;
	char szBuff[sizeof(SSPKG)];
	unsigned char ucEntity;
	
	szNet.iBuff = sizeof(szBuff);
	szNet.pszBuff = szBuff;

	//GET_FUNC_ENTITY(ucEntity, iDstID);
	memcpy(&ucEntity ,((char *)&(iDstID))+2,1);
	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode 
		&& ucEntity == FUNC_MUD_SVR)
	{
		return 0;
	}

	if (0 > ss_pack(pstEnv->pstSSHan, &szNet, pstSSPkg,  0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail,cmd=%d", pstSSPkg->Head.Cmd);
		return -1;
	}

	iSrcID = pstAppCtx->iId;
	if (0 > tbus_send(pstAppCtx->iBus, &iSrcID, &iDstID, szNet.pszBuff, szNet.iBuff, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail,iDstID=%s,Cmd=%d",
			tbus_addr_ntoa(iDstID),pstSSPkg->Head.Cmd);
		return -1;
	}
	
	if (pstEnv->iMudSvrID == iDstID)
	{
		tlog_debug(pstEnv->pstLogCat, 0, 0,
		           "z-->m msg:headlen=%d,bodylen=%d",
		           ntohs(*(short*)(szNet.pszBuff + 4)),
		           ntohs(*(short*)(szNet.pszBuff + 6)));
		
		tlog_debug_dr(pstEnv->pstLogCat, 0, 0,
		              pstEnv->pstSSHan->pstSSProtoPkgMeta, 
		              pstSSPkg, sizeof(*pstSSPkg), 0);
	}
	else
	{
		tlog_debug(pstEnv->pstLogCat, 0, 0,
		           "s-->s (%d-->%d) msg:headlen=%d,bodylen=%d",
		           iSrcID,
		           iDstID,
		           ntohs(*(short*)(szNet.pszBuff + 4)),
		           ntohs(*(short*)(szNet.pszBuff + 6)));
		
		tlog_debug_dr(pstEnv->pstLogCat, 0, 0,
		              pstEnv->pstSSHan->pstSSProtoPkgMeta, 
		              pstSSPkg, sizeof(*pstSSPkg), 0);
	}



	
	return 0;
}

int z_team_sys_strf(ZONESVRENV *pstEnv, ZoneTeam *pstTeam, int iSysType, int iErrno, ...)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	const char* pszErr;
	va_list marker;

	va_start( marker, iErrno );
	pszErr	=	ZONE_GET_ERR(iErrno, 0);
	
	pstSysMsg->Type = iSysType;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;

	if( !pszErr )
	{
		snprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), "SysNo : %d", iErrno );
	}
	else
	{
		vsnprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), pszErr, marker);
	}

	pstSysMsg->Msg[sizeof(pstSysMsg->Msg) - 1] = 0;
	va_end(marker);
	

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	player_team_broadcast(pstEnv, pstTeam, pstPkg);

	return 0;
}

int z_team_sys_strf2(ZONESVRENV *pstEnv, ZoneTeam *pstTeam, int iSysType, char *pszStr, ...)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	char szMsg[SYS_MSG_LEN];
	va_list valist;
	
	va_start( valist, pszStr );
	vsnprintf(szMsg, sizeof(szMsg), pszStr, valist);
	szMsg[SYS_MSG_LEN -1] = 0;
	va_end(valist);

	pstSysMsg->Type = iSysType;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
	STRNCPY(pstSysMsg->Msg, szMsg, sizeof(pstSysMsg->Msg));

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	player_team_broadcast(pstEnv, pstTeam, pstPkg);

	return 0;
}

int z_sys_strf_broadcast(ZONESVRENV *pstEnv, int iSysType, int iErrno, ...)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	const char* pszErr;
	va_list marker;

	va_start( marker, iErrno );
	pszErr	=	ZONE_GET_ERR(iErrno, 0);

	pstSysMsg->Type = iSysType;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;

	if( !pszErr )
	{
		snprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), "SysNo : %d", iErrno );
	}
	else
	{
		vsnprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), pszErr, marker);
	}

	pstSysMsg->Msg[sizeof(pstSysMsg->Msg) - 1] = 0;
	va_end(marker);

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);

	z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);

	return 0;
}

int z_sys_strf_map_broadcast(ZONESVRENV *pstEnv, ZONEMAPINST * pstMapInst, int iSysType, int iErrno, ...)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	const char* pszErr;
	va_list marker;

	va_start( marker, iErrno );
	pszErr	=	ZONE_GET_ERR(iErrno, 0);

	pstSysMsg->Type = iSysType;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
	if( !pszErr )
	{
		snprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), "SysNo : %d", iErrno );
	}
	else
	{
		vsnprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), pszErr, marker);
	}

	pstSysMsg->Msg[sizeof(pstSysMsg->Msg) - 1] = 0;
	va_end(marker);

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	map_broadcast(pstEnv->pstAppCtx, pstEnv, pstMapInst, &stPkg);

	return 0;
}


int z_send_to_region_msg(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, int iCmd,char *szData,int iLen)
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
	if(ss_pack(pstEnv->pstSSHan, &stNet, &stPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"ss_pack fail.cmd=%d", iCmd);
		return -1;
	}

	iSrc = pstAppCtx->iId;
	iDst = pstEnv->iRegionGateID;
	if(tbus_forward(pstAppCtx->iBus, &iSrc, &iDst, stNet.pszBuff, stNet.iBuff, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"tbus_forward fail.");
		return -1;
	}
	return 0;
}

int battle_proxy_respond(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
                         SSBATTLEPROXY *pstRes)
{
	//CSPKG stPkg;
	int iRet = 0;
	CmdMap *pstCmdMap = NULL;
	Player *pstPlayer = player_get_by_uin(pstEnv, pstRes->Uin);

	if(!pstPlayer || 
	   pstPlayer->stRoleData.RoleID != pstRes->RoleID ||
	   pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "proxy uin fail: %d", pstRes->Uin);
		return -1;
	}

	iRet = net_cs_unpack(pstEnv->pstNetHan, pstRes->Data, pstRes->DataLen,
	                     &pstEnv->stCSReq, pstPlayer->unCltProVer);
	if(iRet < 0)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
		           "uin=%d cmd=%d prover=%d proxy respond cs pack fail ret=%d",
		           pstPlayer->stRoleData.Uin, pstEnv->stCSReq.Head.Cmd,
		           pstPlayer->unCltProVer, iRet);
		return -1;
	}

	tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
	           "c-->b-[Proxy]->s player %d", pstPlayer->stRoleData.Uin);
	tlog_debug_dr(pstEnv->pstLogCat, 0, 0, pstEnv->pstNetHan->pstCSProtoPkgMeta, 
	              &pstEnv->stCSReq, sizeof(pstEnv->stCSReq), 0);

	assert(pstEnv->stCSReq.Head.Cmd < MAX_CS_CMD);
	pstCmdMap = &pstEnv->astCmdMap[pstEnv->stCSReq.Head.Cmd];

	if(!pstCmdMap->pfnCmdHandle)
	{ 
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
		           "bad proxy cmd: %d", pstEnv->stCSReq.Head.Cmd);
		return -1;
	}
	return pstCmdMap->pfnCmdHandle(pstAppCtx, pstEnv, pstPlayer, NULL, // Maybe crash
	                               &pstEnv->stCSReq);
}

int battle_proxy_request(ZONESVRENV* pstEnv, Player *pstPlayer, CSPKG *pstPkg)
{
	int iRet = 0;
	TDRDATA stNet;
	SSPKG stReq;

	stNet.iBuff = sizeof(stReq.Body.BattleProxy.Data);
	stNet.pszBuff = &stReq.Body.BattleProxy.Data[0];
	
	iRet = net_cs_pack(pstEnv->pstNetHan, &stNet, pstPkg, pstPlayer->unCltProVer);
	if(iRet < 0)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
		           "uin=%d cmd=%d prover=%d proxy request cs pack fail ret=%d",
		           pstPlayer->stRoleData.Uin, pstPkg->Head.Cmd,
		           pstPlayer->unCltProVer, iRet);
		return -1;
	}
	stReq.Body.BattleProxy.DataLen = stNet.iBuff;
	stReq.Body.BattleProxy.Uin = pstPlayer->stRoleData.Uin;
	stReq.Body.BattleProxy.RoleID = pstPlayer->stRoleData.RoleID;

	tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
	           "uin=%d b-[Proxy]->s-->c msg:headlen=%d,bodylen=%d",
	           pstPlayer->stRoleData.Uin,
	           ntohs(*(short*)(stNet.pszBuff + 4)),
	           ntohs(*(short*)(stNet.pszBuff + 6)));
	tlog_debug_dr(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
	              pstEnv->pstNetHan->pstCSProtoPkgMeta, pstPkg, sizeof(CSPKG),
	              pstPlayer->unCltProVer);

	Z_SSHEAD_INIT(&stReq.Head, BATTLE_PROXY_ROUTINE);
	return z_ssmsg_send(pstEnv, &stReq,
	              player_master_busid_get(pstEnv,  pstPlayer));
}
int zone_proxy_respond(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
                         SSBATTLEPROXY *pstRes)
{
	int iRet = 0;
	CmdMap *pstCmdMap = NULL;
	Player *pstPlayer = player_get_by_uin(pstEnv, pstRes->Uin);

	if(!pstPlayer || 
	   pstPlayer->stRoleData.RoleID != pstRes->RoleID)
	{
		if(pstPlayer)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "proxy uin fail: %d,%lld,%lld", pstRes->Uin,pstRes->RoleID,pstPlayer->stRoleData.RoleID);
		}
		return -1;
	}

	iRet = net_cs_unpack(pstEnv->pstNetHan, pstRes->Data, pstRes->DataLen,
	                     &pstEnv->stCSReq, pstPlayer->unCltProVer);
	if(iRet < 0)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
		           "uin=%d cmd=%d prover=%d proxy respond cs pack fail ret=%d",
		           pstPlayer->stRoleData.Uin, pstEnv->stCSReq.Head.Cmd,
		           pstPlayer->unCltProVer, iRet);
		return -1;
	}

	tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
	           "c-->s-[Proxy]->b player %d", pstPlayer->stRoleData.Uin);
	tlog_debug_dr(pstEnv->pstLogCat, 0, 0, pstEnv->pstNetHan->pstCSProtoPkgMeta, 
	              &pstEnv->stCSReq, sizeof(pstEnv->stCSReq), 0);


	assert(pstEnv->stCSReq.Head.Cmd < MAX_CS_CMD);
	pstCmdMap = &pstEnv->astCmdMap[pstEnv->stCSReq.Head.Cmd];

	if(!pstCmdMap->pfnCmdHandle)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
		           "bad proxy cmd: %d", pstEnv->stCSReq.Head.Cmd);
		return -1;
	}
	return pstCmdMap->pfnCmdHandle(pstAppCtx, pstEnv, pstPlayer, NULL, // Maybe crash
	                               &pstEnv->stCSReq);
}

int zone_proxy_request(ZONESVRENV* pstEnv, Player *pstPlayer, CSPKG *pstPkg)
{
	int iRet = 0;
	TDRDATA stNet;
	SSPKG stReq;

	if(pstPlayer->stOnline.stSpanOnline.stApply.iBattleApply != SPAN_APPLY_ING)
	{
		return -1;
	}

	stNet.iBuff = sizeof(stReq.Body.BattleProxy.Data);
	stNet.pszBuff = &stReq.Body.BattleProxy.Data[0];
	
	iRet = net_cs_pack(pstEnv->pstNetHan, &stNet, pstPkg, pstPlayer->unCltProVer);
	if(iRet < 0)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
		           "uin=%d cmd=%d prover=%d proxy request cs pack fail ret=%d",
		           pstPlayer->stRoleData.Uin, pstPkg->Head.Cmd,
		           pstPlayer->unCltProVer, iRet);
		return -1;
	}
	stReq.Body.BattleProxy.DataLen = stNet.iBuff;
	stReq.Body.BattleProxy.Uin = pstPlayer->stRoleData.Uin;
	stReq.Body.BattleProxy.RoleID = pstPlayer->stRoleData.RoleID;

	Z_SSHEAD_INIT(&stReq.Head, ZONE_PROXY_ROUTINE);
	return z_ssmsg_send(pstEnv, &stReq, pstEnv->iBattlePveBattleID);
}

int zone_proxy_sparta_request(ZONESVRENV* pstEnv, Player *pstPlayer, CSPKG *pstPkg)
{
	int iRet = 0;
	TDRDATA stNet;
	SSPKG stReq;

	if(pstPlayer->stOnline.stSpanOnline.stApply.iSpartaApply != SPAN_APPLY_ING)
	{
		return -1;
	}

	stNet.iBuff = sizeof(stReq.Body.BattleProxy.Data);
	stNet.pszBuff = &stReq.Body.BattleProxy.Data[0];
	
	iRet = net_cs_pack(pstEnv->pstNetHan, &stNet, pstPkg, pstPlayer->unCltProVer);
	if(iRet < 0)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
		           "uin=%d cmd=%d prover=%d proxy request cs pack fail ret=%d",
		           pstPlayer->stRoleData.Uin, pstPkg->Head.Cmd,
		           pstPlayer->unCltProVer, iRet);
		return -1;
	}
	stReq.Body.BattleProxy.DataLen = stNet.iBuff;
	stReq.Body.BattleProxy.Uin = pstPlayer->stRoleData.Uin;
	stReq.Body.BattleProxy.RoleID = pstPlayer->stRoleData.RoleID;

	Z_SSHEAD_INIT(&stReq.Head, ZONE_PROXY_ROUTINE);
	return z_ssmsg_send(pstEnv, &stReq, pstEnv->iBattleSpartaID);
}

// zone --> battle
int battle_mirror_sync_request(ZONESVRENV* pstEnv, int iBusID, int iCmd,
                               const void *pstRawData, int iDataLen)
{
	SSPKG stPkg;
	SSBATTLEMIRRORSYNC *pstReq = &stPkg.Body.BattleMirrorSync;

	pstReq->Cmd = iCmd;
	pstReq->DataLen = iDataLen;
	memcpy(pstReq->Data, pstRawData, iDataLen);

	Z_SSHEAD_INIT(&stPkg.Head, BATTLE_MIRROR_SYNC);
	return z_ssmsg_send(pstEnv, &stPkg, iBusID);
}

//battle->zone
int zone_mirror_sync_respond(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,
                               int iMsgSrc, SSBATTLEMIRRORSYNC *pstRes)
{
	UNUSED(pstAppCtx);
	UNUSED(iMsgSrc);

	switch(pstRes->Cmd)
	{
	case MIRROR_SYNC_CMD_ACTION_RES:
		{
			int iUin = 0;
			Player *pstPlayer = NULL;

			memcpy(&iUin, pstRes->Data, sizeof(iUin));
			pstPlayer = player_get_by_uin(pstEnv, iUin);
			if(!pstPlayer)
			{
				return -1;
			}

			// 放到里面解包,减少内存拷贝
			player_shadow_action_res(pstEnv, pstPlayer,
			                         pstRes->Data + sizeof(iUin),
			                         pstRes->DataLen - sizeof(iUin));
		}
		break;
	default:
		break;
	}

	return 0;
}

// battle received
int battle_mirror_sync_respond(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,
                               int iMsgSrc, SSBATTLEMIRRORSYNC *pstRes)
{
	UNUSED(pstAppCtx);

	switch(pstRes->Cmd)
	{
	case MIRROR_SYNC_CMD_IDLE:
		return -1;
		break;

		/*
		 * +---+---------- ~ --+
		 * |Uin| CSActionRes   |
		 * +---+---------- ~ --+
		 */
	case MIRROR_SYNC_CMD_ACTION_RES:
		{
			int iUin = 0;
			Player *pstPlayer = NULL;

			memcpy(&iUin, pstRes->Data, sizeof(iUin));
			pstPlayer = player_get_by_uin(pstEnv, iUin);
			if(!pstPlayer)
			{
				return -1;
			}

			// 放到里面解包,减少内存拷贝
			player_shadow_action_res(pstEnv, pstPlayer,
			                         pstRes->Data + sizeof(iUin),
			                         pstRes->DataLen - sizeof(iUin));
		}
		break;

		/*
		 * +--------+------------ ~ --+
		 * | TeamID |    TeamInfo     |
		 * +--------+------------ ~ --+
		*/
	/*
	case MIRROR_SYNC_CMD_TEAM:
		{
			tdr_ulonglong ullTeamID = 0ULL;

			memcpy(&ullTeamID, pstRes->Data, sizeof(ullTeamID));
			team_shadow_sync(pstEnv, iMsgSrc, ullTeamID,
			                         pstRes->Data + sizeof(ullTeamID),
			                         pstRes->DataLen - sizeof(ullTeamID));
		}
		break;

	case MIRROR_SYNC_CMD_TEAM_DESTROY:
		{
			if(pstRes->DataLen != sizeof(tdr_ulonglong))
			{
				return -1;
			}
			team_shadow_destroy_respond(pstEnv, iMsgSrc,
			                            *((const tdr_ulonglong*)pstRes->Data));
		}
		break;
	*/
	}

	return 0;
	UNUSED(iMsgSrc);	//add by paraunused.pl
}

int normal_battle_camp_broadcast_except(ZONESVRENV *pstEnv, Player *pstPlayer, CSPKG *pstPkg, Player *pstExcept)
{
	int i = 0, iCount = 0;
	Player *apstPlayers[MAX_PWORLD_MEMBER];
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;
	ZONEPWORLDINST* pstPworldInst = NULL;
	
	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos =	pstPlayer->stOnline.iMapPos;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
		return -1;


	pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if(NULL == pstPworldInst)
		return -1;

	bzero(apstPlayers, sizeof(apstPlayers));
	i = pstPworldInst->iPlayer;
	while(i--)
	{
		Player *pstElement = z_id_player(pstEnv->pstAppCtx, pstEnv,
		                                 pstPworldInst->aiPlayerID[i]);
		if(NULL == pstElement || pstElement == pstExcept)
		{
			continue;
		}

		if(pstElement->stOnline.stWarOL.cCampIdx == pstPlayer->stOnline.stWarOL.cCampIdx)
		{
			apstPlayers[iCount++] = pstElement;
		}
	}

	if(iCount > 0)
	{
		if(z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
		                            iCount, pstPkg) < 0)
		{
			return -1;
		}
	}
	return iCount;
}

int normal_battle_camp_broadcast(ZONESVRENV *pstEnv, Player *pstPlayer, CSPKG *pstPkg)
{
	return normal_battle_camp_broadcast_except(pstEnv, pstPlayer, pstPkg, NULL);
}

int z_pkg_to_mud(ZONESVRENV *pstEnv,CSPKG *pstPkg,char *pszName,uint iClanID,int iTransType)
{
	SSPKG * pstSSPkg = &pstEnv->stSSRes;
	SSPKGTRANS* pstTrans = &pstSSPkg->Body.PkgTansInfo;

	pstTrans->Cmd = pstPkg->Head.Cmd;
	if (!IsTransPkgCmd(pstTrans->Cmd))
	{
		return -1;
	}


	if (ONLINE_INFO == pstTrans->Cmd)
	{
		return -1;
	}

	
	
	pstTrans->szName[0] = 0;
	pstTrans->Type = iTransType;
	pstTrans->ClanID = iClanID;
	if (pszName)
	{
		STRNCPY(pstTrans->szName, pszName, CCH(pstTrans->szName));
	}
	memcpy(&pstTrans->Body,&pstPkg->Body,sizeof(pstTrans->Body));
	if (CHAT_RES == pstTrans->Cmd)
	{
		pstTrans->Body.ChatRes.OnlineType = ROLE_GAME_ONLINE;
	}

	Z_SSHEAD_INIT(&pstSSPkg->Head, SS_PKG_TRANS);
	z_ssmsg_send(pstEnv, pstSSPkg, pstEnv->iMudSvrID);	



	return 0;
}

int z_pkg_all_world(ZONESVRENV *pstEnv,CSPKG *pstPkg)
{
	SSPKG * pstSSPkg = &pstEnv->stSSRes;
	SSMSGTRANS *pstSSMsgTrans = &pstSSPkg->Body.SSMsgTrans;

	pstSSMsgTrans->CSPkg = *pstPkg;

	Z_SSHEAD_INIT(&pstSSPkg->Head, SS_MSG_TRANS);
	z_ssmsg_send(pstEnv, pstSSPkg, pstEnv->iRegionGateID);	
	return 0;
}

int z_pkg_span(ZONESVRENV *pstEnv,CSPKG *pstPkg, int iBusID, int iTransType)
{
	SSPKG * pstSSPkg = &pstEnv->stSSRes;
	SSPKGTRANS* pstTrans = &pstSSPkg->Body.PkgTansInfo;

	pstTrans->Cmd = pstPkg->Head.Cmd;
	pstTrans->szName[0] = 0;
	pstTrans->Type = iTransType;
	pstTrans->ClanID = 0;

	memcpy(&pstTrans->Body,&pstPkg->Body,sizeof(pstTrans->Body));

	Z_SSHEAD_INIT(&pstSSPkg->Head, SS_PKG_TRANS);
	z_ssmsg_send(pstEnv, pstSSPkg, iBusID);	

	return 0;
}

int z_sys_strf_span_broadcast(ZONESVRENV *pstEnv, int iBusID, int iSysType, char *pszStr, ...)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	char szMsg[SYS_MSG_LEN];
	va_list valist;
	
	va_start( valist, pszStr );
	vsnprintf(szMsg, sizeof(szMsg), pszStr, valist);
	szMsg[SYS_MSG_LEN -1] = 0;
	va_end(valist);

	pstSysMsg->Type = iSysType;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
	STRNCPY(pstSysMsg->Msg, szMsg, sizeof(pstSysMsg->Msg));
	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	z_pkg_span(pstEnv, pstPkg, iBusID, PKG_TRANS_ALL);
	return 0;
}


// 战场工会广播包
int z_pkg_span_clan(ZONESVRENV *pstEnv,CSPKG *pstPkg, int iBusID, int iClanID)
{
	SSPKG * pstSSPkg = &pstEnv->stSSRes;
	SSPKGTRANS* pstTrans = &pstSSPkg->Body.PkgTansInfo;

	pstTrans->Cmd = pstPkg->Head.Cmd;
	pstTrans->szName[0] = 0;
	pstTrans->Type = PKG_TRANS_CLAN;
	pstTrans->ClanID = iClanID;

	memcpy(&pstTrans->Body,&pstPkg->Body,sizeof(pstTrans->Body));

	Z_SSHEAD_INIT(&pstSSPkg->Head, SS_PKG_TRANS);
	z_ssmsg_send(pstEnv, pstSSPkg, iBusID);	

	return 0;
}

// 战场公会广播消息
int z_sys_strf_span_broadcast_clan(ZONESVRENV *pstEnv, int iBusID, int iClanID,int iSysType, char *pszStr, ...)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	char szMsg[SYS_MSG_LEN];
	va_list valist;
	
	va_start( valist, pszStr );
	vsnprintf(szMsg, sizeof(szMsg), pszStr, valist);
	szMsg[SYS_MSG_LEN -1] = 0;
	va_end(valist);

	pstSysMsg->Type = iSysType;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
	STRNCPY(pstSysMsg->Msg, szMsg, sizeof(pstSysMsg->Msg));

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	z_pkg_span_clan(pstEnv,pstPkg, iBusID,iClanID);
	return 0;
}


int z_info_get_to_mud(ZONESVRENV *pstEnv,SSINFOGETREQ* pstReq, int iType)
{
	SSPKG * pstSSPkg = &pstEnv->stSSRes;
	
	pstSSPkg->Body.InfoGetReq = *pstReq;
	pstSSPkg->Body.InfoGetReq.Type = iType;
	
	Z_SSHEAD_INIT(&pstSSPkg->Head, SS_INFO_GET_REQ);
	z_ssmsg_send(pstEnv, pstSSPkg, pstEnv->iMudSvrID);	
	return 0;
}

int z_cmd_to_mud(ZONESVRENV *pstEnv,char *pszReceiver,char *pszName,int iCmd,SSCMDDATA* pstData)
{
	SSPKG * pstSSPkg = &pstEnv->stSSRes;
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
	z_ssmsg_send(pstEnv, pstSSPkg, pstEnv->iMudSvrID);	
	return 0;
}


