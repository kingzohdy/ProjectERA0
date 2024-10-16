/*
**  @file $RCSfile: zone_player.c,v $
**  general description of this module
**  $Id: zone_player.c,v 1.2037 2014/07/19 07:27:26 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/07/19 07:27:26 $
**  @version $Revision: 1.2037 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_player.h"
#include "tsec/tsecbasedef.h"
#include "tconnapi.h"
#include "comm/shtable.h"
#include "zone_clt.h"
#include "zone_db.h"
#include "zone_map.h"
#include "zone_move.h"
#include "mapmask.h"
#include "zone_gm.h"
#include "zone_act_status.h"
#include "zone_attr.h"
#include "zone_script.h"
#include "zone_skill.h"
#include "zone_status.h"
#include "zone_oplog.h"
#include "zone_package.h"
#include "zone_err.h"
#include "libdirty.h"
#include "zone_combat.h"
#include "zone_ai.h"
#include "zone_team.h"
#include "zone_tmp.h"
#include "zone_task.h"
#include "zone_relation.h"
#include "zone_npc.h"
#include "zone_combat.h"
#include "zone_pet.h"
#include "zone_exchg.h"
#include "zone_clan.h"
#include "zone_mail.h"
#include "zone_stall.h"
#include "zone_clan.h"
#include "zone_clan_construction.h"
#include "zone_clan_advance.h"
#include "zone_designation.h"
#include "zone_respact.h"
#include "zone_fairy.h"
#include "zone_clan.h"
#include "zone_shop.h"
#include "zone_range.h"
#include "zone_autoteam.h"
#include "zone_secondaryskill.h"
#include "zone_relation.h"
#include "zone_machine.h"
#include "zone_script.h"
#include "zone_misc.h"
#include "comm_misc.h"
#include "zone_roll.h"
#include "zone_collect.h"
#include "zone_battle.h"
#include "zone_diffpworld.h"
#include "zone_spansvr.h"
#include "zone_machine_view.h"
#include "zone_safelock.h"
#include "zone_combat.h"
#include "zone_arm.h"
#include "zone_vip.h"
#include "zone_city.h"
#include "zone_ai.h"
#include "zone_clan_city.h"
#include "zone_rune.h"
#include "auth_svr.h"
#include "grav_rsa.h"
#include "zone_aas.h"
#include "zone_unreal.h"
#include "zone_strongpoint.h"
#include "zone_trans.h"
#include "zone_span.h"
#include "zone_genius.h"
#include "zone_baoxiang.h"
#include "zone_boss.h"
#include "zone_city.h"
#include "zone_sparta.h"
#include "zone_itemlimit.h"
#include "zone_busy.h"
#include "zone_pworld.h"
#include "zone_zh_name.h"
#include "zone_second_pwd.h"
#include "zone_syncrelation.h"
#include "zone_store.h"
#include "zone_syncrelation.h"
#include "zone_losttotem.h"
#include "zone_ploy.h"
#include "zone_clan_pet.h"
#include "zone_dianfeng.h"
#include "zone_shangjin.h"
#include "zone_span_strong.h"

extern int g_iDesigVar;
#define BASE_XP 50
#define NORMAL_RERIVER_RATE 20
#define HUILIU_DAY 2
#define PERFECTION_REVIVE_GOLD 50

// 没有领取
#define NOT_GIFT 0
// 领了一次  不能在领取下次了
#define ONCE_GIFT_NOT_NEXT 1

extern int g_iOptype;
extern int g_iOpsubtype;
extern ZONESVRENV gs_stEnv;

// 死亡赦免令
#define ITEM_DIE_PARDEN_ID 5050500
// 死亡赦免令(绑定)
#define ITEM_DIE_PARDEN_BIND_ID 5050510
#define ZUOLAO_CENTER_X 30900
#define ZUOLAO_CENTER_Y 19400
#define ZUOLAO_RADIUS 2200

#define ZJ_RONGYAN_TOP_LEVEL 9
#define ZJ_RONGYAN_FIRST_LEVEL 1

#define BOSS_BAOXIANG_LIMIT_LEVEL 10

typedef struct tagDieDropGrid
{
	int iListType;
	int iGridIdx;
	ROLEGRID stGrid;
}DieDropGrid;

extern ScriptEnv stSEnv;
extern RSA_PRIVATE_KEY * IDRSAPrivateKey;


int g_iVimRecoverInterval = VIM_RECOVER_INTERVAL;
int g_dartcar_allow = 0;

// 跨服野外复活坐标
#define MAX_SPAN_MAP_CAMP 8
static int gszSpanMapRevivePos[][2] = 
{
	{6174,42436},    //  第一阵营复活坐标
	{43890,4912},    //    ...
	{6226,6747},     //    ...
	{44075,43710},   //    ...
	{25481,48739},   //    ...
	{25329,2115},    //    ...
	{4672,25590},    //    ...
	{48455,25603},   //  第八阵营复活坐标
};


//static int player_silence_check(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_new_role_gift_info(ZONESVRENV* pstEnv, Player *pstPlayer);
int self_battle_info_tick(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_die_battle_info_chg(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneAni *pstAtker);

int in_auto_revive_pworld(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_pk_time_clear(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_target_action(ZONESVRENV* pstEnv, Player *Player);
int player_revive_svr1(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, int iHp, int iMp);
int player_die_drop_app( ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstKiller, DIEDROP *pstDrop);
REVIVEDEF* z_find_revive_def(ZONESVRENV* pstEnv, int iLevel);


int ass_notify_player(ZONESVRENV* pstEnv, Player *pstPlayer, AasTime*pstAasTime)
{

	if(pstAasTime->tNotify <= AAS_HOUR*3)
	{
		if (pstAasTime->tNotify == AAS_HOUR*3)
		{
			//补填身份证
			player_aas_id_card_notify(pstEnv, pstPlayer);
			
			//收益开始减半
			pstPlayer->bAasIncomeLimt = 50;

			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PLAYER101, (int)pstAasTime->tNotify/AAS_HOUR);

			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, 
				ZONEERR_PLAYER102);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PLAYER101, (int)pstAasTime->tNotify/AAS_HOUR);
		}
	}
	else if (pstAasTime->tNotify < AAS_HOUR*5)
	{
		//补填身份证
		player_aas_id_card_notify(pstEnv, pstPlayer);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PLAYER102);
	}
	else
	{
		//收益为0
		pstPlayer->bAasIncomeLimt = 100;
		//补填身份证
		player_aas_id_card_notify(pstEnv, pstPlayer);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PLAYER103);
	}

	return 0;
}

int CheckNotify(ZONESVRENV* pstEnv, Player *pstPlayer, AasTime *pstAasTime)
{
	time_t tNotify;


	if(PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
	{
		return 0;
	}

	/*在线通知计算*/
	if ( pstAasTime->tNotify > pstAasTime->tOnline )
	{   
	    pstAasTime->tNotify = 0;
	}

	if ( pstAasTime->tOnline > AAS_HOUR*5+AAS_QUARTER_HOUR )
	{
		tNotify = pstAasTime->tOnline/AAS_QUARTER_HOUR*AAS_QUARTER_HOUR;
	}
	else if (pstAasTime->tOnline >= AAS_HOUR*3 + AAS_HALF_HOUR && pstAasTime->tOnline < AAS_HOUR*5+AAS_QUARTER_HOUR)
	{
		tNotify = pstAasTime->tOnline/AAS_HALF_HOUR*AAS_HALF_HOUR;
	}
	else
	{
		tNotify = pstAasTime->tOnline/AAS_HOUR*AAS_HOUR;
	}

	if ( tNotify > pstAasTime->tNotify )
	{
	 	pstAasTime->tNotify = tNotify;
		return 1;
	}
	else
	{
	 	return 0;
	}
	
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int CheckAasOnline(ZONESVRENV* pstEnv, Player *pstPlayer, AasTime*pstAasTime)
{
	if (pstAasTime->tActive < pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		pstAasTime->tOnline  += pstEnv->pstAppCtx->stCurr.tv_sec-pstAasTime->tActive;
	}
	pstAasTime->tActive = pstEnv->pstAppCtx->stCurr.tv_sec;

	if (CheckNotify(pstEnv, pstPlayer, pstAasTime))
	{
		ass_notify_player(pstEnv, pstPlayer, pstAasTime);
	}
	
	return 0;	
}

DIEPUNISHDEF  *z_find_diepunish_def(ZONESVRENV* pstEnv, int iType)
{
	DIEPUNISHDEF stDef;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.PKType = iType;
	iIndex = bsearch_int(&stDef, pstObj->sDiePunishDef, pstObj->iDiePunishDef, DIE_PUNISH_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (DIEPUNISHDEF *)pstObj->sDiePunishDef[iIndex];
	}
	
	return NULL;	
}

int send_to_aas(ZONESVRENV* pstEnv, int iUin, unsigned char ucType)
{
	int iSrcID = pstEnv->pstAppCtx->iId;
	int iDstID = pstEnv->iRegionGateID;
	char szBuff[sizeof(SSPKG)];
	TDRDATA stNet;
	SSPKG stPkgReq;
	SSAASREQ *pstSSAasReq = &stPkgReq.Body.AasReq;
	
	pstSSAasReq->Uin = iUin;
	pstSSAasReq->SvrID = iSrcID;
	pstSSAasReq->ActType = ucType;

	Z_SSHEAD_INIT(&stPkgReq.Head, AAS_GAME_REQ);
	
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
	UNUSED(pstEnv);
}

int z_heartbeat_to_region(ZONESVRENV* pstEnv, int iUin)
{
	int iSrcID = pstEnv->pstAppCtx->iId;
	int iDstID = pstEnv->iRegionGateID;
	char szBuff[sizeof(SSPKG)];
	TDRDATA stNet;
	SSPKG stPkgReq;
	SSZONEHEARTBEAT *pstSSHeartBeat = &stPkgReq.Body.ZoneHeartBeat;

	pstSSHeartBeat->Uin = iUin;
	Z_SSHEAD_INIT(&stPkgReq.Head, ZONE_HEARTBEAT);
	
	stNet.pszBuff = szBuff;
	stNet.iBuff = sizeof(szBuff);
	if (0 > ss_pack(pstEnv->pstSSHan, &stNet, &stPkgReq, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack_err");
		return -1;
	}
	
	if (0 > tbus_send(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, stNet.pszBuff, stNet.iBuff, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail");
		return -1;
	}

	return 0;
}

Player *player_get_by_uin(ZONESVRENV* pstEnv, int iUin)
{
	Player *pstPlayer;
	UinCache stUinHashData;
	UinCache *pstUinHashData;
	
	stUinHashData.iUin = iUin;
	pstUinHashData = (UinCache *)sht_find(pstEnv->pstShtUinCache, &stUinHashData, UinHashCmp, UinHashCode);
	if (!pstUinHashData || 0 == pstUinHashData->iOnline)
	{
		return NULL;
	}

	pstPlayer = (Player *)tmempool_get(pstEnv->pstPlayerPool, pstUinHashData->iIdx);
	if (NULL == pstPlayer)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "get player by uin error");
		return NULL;
	}
	
	if (pstPlayer->stRoleData.Uin == iUin)
	{
		return pstPlayer;
	}
	else
	{
		return NULL;
	}
}

int player_kickoff(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iErrno, ...)
{
	const char* pszErr;
	va_list marker;
	char szMsg[256];

	if (iErrno > -1)
	{
		va_start( marker, iErrno );
		pszErr	=	ZONE_GET_ERR(iErrno, 0);
		if( !pszErr )
		{
			snprintf(szMsg, sizeof(szMsg), "SysNo : %d", iErrno );
		}
		else
		{
			vsnprintf(szMsg, sizeof(szMsg), pszErr, marker);
		}

		szMsg[sizeof(szMsg) - 1] = 0;
		va_end(marker);

		player_kickoff_text_notify( pstAppCtx, pstEnv, pstPlayer, szMsg);
	}
	
	tlog_info(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "kick off player uin=%d", 
				pstPlayer->stRoleData.Uin);

	//will be asyn process, first should update role data succ
	pstPlayer->ucRoleLogoutReason = ROLE_LOGOUT_KICK;

	if( pstPlayer->stOnline.stSpanOnline.iStat == SPAN_STAT_CONNECT)
	{
		return player_span_role_kick(pstEnv, pstPlayer);
	}
	else if( pstPlayer->stOnline.stSpanOnline.iStat == SPAN_STAT_HOLD ||
		pstPlayer->stOnline.stSpanOnline.iStat == SPAN_STAT_APPLY)
	{
		return z_free_player(pstAppCtx, pstEnv, pstPlayer);
	}
	else if (PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus)
	{
		pstPlayer->eStatus = PLAYER_STATUS_ROLE_LOGOUT_REQ;
		tlog_info(pstEnv->pstLogCat, 0, 0, "Status=Role_LogOut_Req Uin=%d AccName=%s RoleName=%s",
				pstPlayer->stRoleData.Uin, pstPlayer->szAccount, pstPlayer->stRoleData.RoleName);

		if(is_my_master_svr(pstAppCtx->iId, pstPlayer))
		{
			z_db_role_update(pstAppCtx, pstEnv, pstPlayer,0);
		}
		else
		{
			z_db_role_update(pstAppCtx, pstEnv, pstPlayer, SPAN_SVR_ROLE_KICK);
		}
		
		return 0;	
	}
	else if (PLAYER_STATUS_ROLE_LOGOUT_REQ == pstPlayer->eStatus)
	{
		if ((int)(pstPlayer->stRoleData.LastLogout + LOGOUT_CHECK_TIMER) < pstAppCtx->stCurr.tv_sec)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "kick timer out Uin=%d AccName=%s RoleName=%s",
				pstPlayer->stRoleData.Uin, pstPlayer->szAccount, pstPlayer->stRoleData.RoleName);

			if(is_my_master_svr(pstAppCtx->iId, pstPlayer))
			{
				z_db_role_update(pstAppCtx, pstEnv, pstPlayer,0);
			}
			else
			{
				z_db_role_update(pstAppCtx, pstEnv, pstPlayer, SPAN_SVR_ROLE_KICK);
			}
		}
		return 0;
	}
	else
	{
		if (pstPlayer->stOnline.bWaitRoleLogout)
		{
			if ((int)(pstPlayer->stRoleData.LastLogout + LOGOUT_CHECK_TIMER) < pstAppCtx->stCurr.tv_sec)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "kick timer out Uin=%d AccName=%s RoleName=%s",
					pstPlayer->stRoleData.Uin, pstPlayer->szAccount, pstPlayer->stRoleData.RoleName);

				if(is_my_master_svr(pstAppCtx->iId, pstPlayer))
				{
					z_db_role_update(pstAppCtx, pstEnv, pstPlayer,0);
				}
				else
				{
					z_db_role_update(pstAppCtx, pstEnv, pstPlayer, SPAN_SVR_ROLE_KICK);
				}
			}
			return 0;
		}
		
		//info tconnd stop
		pstEnv->stFrameHeadTemp.Cmd = TFRAMEHEAD_CMD_STOP;
		pstEnv->stFrameHeadTemp.CmdData.Stop.Reason = STOP_REASON_KICKOFF;
		pstEnv->stFrameHeadTemp.ID = z_get_player_connectid(pstEnv,pstPlayer);//pstPlayer->stConnectInfo.iID;
		pstEnv->stFrameHeadTemp.ConnIdx = pstPlayer->stConnectInfo.iConnIdx;

		//tlog_info(pstEnv->pstLogCat, 0, 0, "TFRAMEHEAD_CMD_STOP ConnIdx:%d",pstEnv->stFrameHeadTemp.ConnIdx);
		if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
		{
			if(pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN )
			{
				tconnapi_send(pstEnv->iTconnapiHandle, player_conn_busid_get(pstEnv, pstPlayer),
					pstEnv->szSendBuffer, 0, &pstEnv->stFrameHeadTemp);
			}
		}
		else
		{
			tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iZoneConnID, pstEnv->szSendBuffer, 0, &pstEnv->stFrameHeadTemp);
		}

		//恢复全局变量的初始设置
		pstEnv->stFrameHeadTemp.CmdData.Stop.Reason = 0;

		z_free_player(pstAppCtx, pstEnv, pstPlayer);
				
		return 0;
	}
}

int player_disconnect( ZONESVRENV * pstEnv, Player * pstPlayer )
{
	if (pstPlayer->stOnline.bConnLost)
	{
		return 0;
	}
	
	//info tconnd stop
	pstEnv->stFrameHeadTemp.Cmd = TFRAMEHEAD_CMD_STOP;
	pstEnv->stFrameHeadTemp.CmdData.Stop.Reason = STOP_REASON_KICKOFF;
	pstEnv->stFrameHeadTemp.ID = pstPlayer->stConnectInfo.iID;
	pstEnv->stFrameHeadTemp.ConnIdx = pstPlayer->stConnectInfo.iConnIdx;

	//tlog_info(pstEnv->pstLogCat, 0, 0, "TFRAMEHEAD_CMD_STOP ConnIdx:%d",pstEnv->stFrameHeadTemp.ConnIdx);
	if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		if( pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN )
		{
			tconnapi_send(pstEnv->iTconnapiHandle, player_conn_busid_get(pstEnv, pstPlayer), 
			pstEnv->szSendBuffer, 0, &pstEnv->stFrameHeadTemp);
		}
	}
	else
	{
		tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iZoneConnID, 
		pstEnv->szSendBuffer, 0, &pstEnv->stFrameHeadTemp);
	}

	//恢复全局变量的初始设置
	pstEnv->stFrameHeadTemp.CmdData.Stop.Reason = 0;
	pstPlayer->stOnline.bConnLost = 1;

	pstPlayer->ucRoleLogoutReason = ROLE_LOGOUT_CLOSE;
	pstPlayer->stRoleData.LastLogout = pstEnv->pstAppCtx->stCurr.tv_sec;
	return 0;
}

int player_kickoff_text_notify(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, 
	Player *pstPlayer, char *pszMsg)
{
	CSPKG stPkg;
	CSKICKOFFINFO *pstKickoffInfo;
		
	if (!pszMsg)
		return 0;

	if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		if( pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN )
		{
			return 0;
		}
	}
	
	pstKickoffInfo = &stPkg.Body.KickoffInfo;
	STRNCPY(pstKickoffInfo->Msg, pszMsg, sizeof(pstKickoffInfo->Msg));

	Z_CSHEAD_INIT(&stPkg.Head, KICKOFF_INFO);
	return z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int player_delay_kickoff(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, char *pszMsg)
{
	if (pszMsg)
	{
		player_kickoff_text_notify(pstAppCtx, pstEnv, pstPlayer, pszMsg);
	}
	
	player_switch_role_delay_logout(pstEnv, pstPlayer, ROLE_LOGOUT_KICK);

	player_disconnect( pstEnv, pstPlayer );
	
	return 0;
}

int player_acc_login_err(ZONESVRENV* pstEnv, TFRAMEHEAD *pstFrameHead, short nResult)
{
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
	memcpy(&stFrameHead.ExtraInfo, &pstFrameHead->ExtraInfo, sizeof(stFrameHead.ExtraInfo));

	//tlog_info(pstEnv->pstLogCat, 0, 0, "TFRAMEHEAD_CMD_STOP ConnIdx:%d",pstFrameHead->ConnIdx);
	
	return tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iZoneConnID, stNet.pszBuff, stNet.iBuff, &stFrameHead);
}

//此函数与认证相关，目前没有使用，等运营时考虑
int player_open_connection(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TFRAMEHEAD *pstFrameHead)
{
	int iUin;
	int iRet = 0;
	Player *pstPlayer;

	iUin = 0;
		
	tlog_debug(pstEnv->pstLogCat, iUin, 0,  "player(%d) open conncetion(index:%d) clientaddr(%s:%d)",
		iUin, pstFrameHead->ConnIdx, 
		inet_ntoa(*(struct in_addr *)&pstFrameHead->ExtraInfo.IPInfo.ip),
		ntohs(pstFrameHead->ExtraInfo.IPInfo.port));

	/*check player whether have been login  before*/
	pstPlayer = player_get_by_uin(pstEnv, iUin);
	if (NULL != pstPlayer)
	{
		/*one player have open two connection, so kickoff player at preconnection*/
		tlog_error(pstEnv->pstLogCat, iUin, 0, "player(%d) have opened one connection(index:%d) address(%s:%d), so kickoff it",
			iUin, pstFrameHead->ConnIdx, 
			inet_ntoa(*(struct in_addr *)&pstFrameHead->ExtraInfo.IPInfo.ip),
			ntohs(pstFrameHead->ExtraInfo.IPInfo.port));

		player_kickoff(pstAppCtx, pstEnv, pstPlayer, ZONEERR_KICK205, inet_ntoa(*(struct in_addr *)&pstFrameHead->ExtraInfo.IPInfo.ip));

		if(pstFrameHead->ExtraInfo.IPInfo.ip == pstPlayer->stConnectInfo.stExtraInfo.IPInfo.ip)
		{
			player_acc_login_err(pstEnv, pstFrameHead, LOGIN_AGAIN_SAMEIP);	
		}
		else
		{
			player_acc_login_err(pstEnv, pstFrameHead, LOGIN_REP);
		}

		return -1;
	}/*if (NULL != pstPlayer)*/

	/*At end, If failed to process , then recycle */
	if (0 != iRet)
	{
		//
	}

	return iRet;
}

void player_switch_role_delay_logout( ZONESVRENV* pstEnv, Player * pstPlayer,
	int iReason )
{
	pstPlayer->ucRoleLogoutReason = (unsigned char)iReason;

	if (pstPlayer->stOnline.bWaitRoleLogout)
	{
		return;
	}

	//这个情况是不应该出现的
	if( pstPlayer->stOnline.stSpanOnline.iStat == SPAN_STAT_CONNECT)
	{
		player_span_role_kick(pstEnv, pstPlayer);
		return ;
	}
	//跨服进程HOLD状态
	else if( pstPlayer->stOnline.stSpanOnline.iStat == SPAN_STAT_HOLD)
	{
		z_free_player(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		return ;
	}
	//跨服战场进程报名状态
	else if(pstPlayer->stOnline.stSpanOnline.iStat == SPAN_STAT_APPLY)
	{
		if(is_battle_sparta_svr(pstEnv))
		{
			sparta_role_logout_clean(pstEnv, pstPlayer);
		}
		else
		{
			z_exit_pworld_queue(pstEnv, pstPlayer, pstPlayer->stOnline.stPworldQueue.iPworldID, 0);
			z_free_player(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}
		return ;
	}
	
	switch( pstPlayer->eStatus )
	{
	case PLAYER_STATUS_ROLE_LOGIN:
		if(pstPlayer->stOnline.State & CS_STAT_PREPARE)
		{
			pstPlayer->stOnline.State &= ~CS_STAT_PREPARE;
		}
		// 踢下线得时候把状态改下 add ouyang by 20110627
		// pstPlayer->eStatus = PLAYER_STATUS_ROLE_LOGOUT_REQ;
		
		pstPlayer->stOnline.bWaitRoleLogout = 1;
		pstPlayer->stOnline.tLogoutBegin = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstPlayer->stRoleData.LastLogout = pstEnv->pstAppCtx->stCurr.tv_sec;
		break;
	//case PLAYER_STATUS_ROLE_ON_BATTLE:
	//	pstPlayer->stOnline.tLogoutBegin = pstEnv->pstAppCtx->stCurr.tv_sec;
	//	pstPlayer->stRoleData.LastLogout = pstEnv->pstAppCtx->stCurr.tv_sec;
	case PLAYER_STATUS_ROLE_LOGOUT_REQ:
		break;

	default:
		
		z_free_player(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		break;
	}

	return ;
}

int player_close_connection(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TFRAMEHEAD *pstFrameHead)
{
	Player *pstPlayer;
	
	if (0 > pstFrameHead->ID)
	{
		return -1;
	}

	pstPlayer = tmempool_get(pstEnv->pstPlayerPool,pstFrameHead->ID);
	if (NULL == pstPlayer)
	{
		tlog_info(pstEnv->pstLogCat, 0, 0, "player_close_connection id=%d player null, reason=%d", 
					pstFrameHead->ID, pstFrameHead->CmdData.Stop.Reason);
		return -1;
	}
	tlog_info(pstEnv->pstLogCat, 0, 0, "player uin %d accname %s idx %d close connect, reason %d", 
				pstPlayer->stRoleData.Uin, pstPlayer->szAccount, pstPlayer->iIdx, pstFrameHead->CmdData.Stop.Reason);

	if (0 == pstPlayer->stOnline.bConnLost)
	{
		pstPlayer->stOnline.bConnLost = 1;

		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "uin=%d close connect, reason=%d", 
					pstPlayer->stRoleData.Uin, pstFrameHead->CmdData.Stop.Reason);

		player_switch_role_delay_logout( pstEnv, pstPlayer, ROLE_LOGOUT_CLOSE );
	}

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

#if defined(ZONE_ID_TRACKED)
Player *player_alloc_tracked(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, int iUin,
                             char *pszAccount, UinCache *pstUinCache,
                             const char *pszFile, int iLine)
#else
Player *player_alloc(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, int iUin,
                     char *pszAccount, UinCache *pstUinCache)
#endif
{
	UinCache stUinHashData;
	UinCache *pstUinHashData;
	Player *pstPlayer;
	int iIdx;

	if (NULL == pstUinCache)
	{
		if (sht_is_full(pstEnv->pstShtUinCache))
		{
			if (0 >= z_washout_uincache(pstEnv))
			{
				return NULL;
			}
		}

		memset(&stUinHashData, 0, sizeof(stUinHashData));
		stUinHashData.iUin = iUin;
		stUinHashData.iIdx = -1;
		stUinHashData.iOnline = 1;
		stUinHashData.tLastLogin = pstAppCtx->stCurr.tv_sec;
		pstUinHashData = (UinCache *)sht_insert_unique(pstEnv->pstShtUinCache,&stUinHashData,UinHashCmp,UinHashCode);
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
	pstPlayer->iMagic = PLAYER_SHM_MAGIC;
	pstPlayer->iMemID = z_get_mem_id(pstAppCtx, pstEnv, iIdx, OBJ_PLAYER);
#if defined(ZONE_ID_TRACKED)
	tlog_info(pstEnv->pstLogCat, 0, 0,
	          "Allocate id, From:%s(%d): Type=Player, DefID=N/A, MemID=%d, ID=%d",
	          pszFile, iLine, iIdx, pstPlayer->iMemID);
#endif
	pstPlayer->iIdx = iIdx;
	pstPlayer->iUinCachePos = (long int)((char *)pstUinHashData + offsetof(SHITEM, szData) - 
							(char *)pstEnv->pstShtUinCache - pstEnv->pstShtUinCache->iDataOff)/pstEnv->pstShtUinCache->iDataUnitMax;
	pstUinHashData->iIdx = iIdx;

	tlog_debug(pstEnv->pstLogCat, iUin, 0, "account(%s) uin=%d mem idx=%d alloc cachepos=%d get mem id %d", pszAccount, iUin, iIdx, pstPlayer->iUinCachePos, pstPlayer->iMemID);

	// 建立Login Name -> Player的映射
	{
		LoginName2MemID stDummy, *pstInserted = NULL;
		stDummy.iID = pstPlayer->iMemID;
		STRNCPY(stDummy.szLoginName, pstPlayer->szAccount,
		        CCH(stDummy.szLoginName));

		pstInserted = sht_insert_unique(pstEnv->pstShtL2P, &stDummy, L2PHashCmp,
		                                L2PHashCode);
		if(pstInserted)
		{
			memcpy(pstInserted, &stDummy, sizeof(stDummy));
		}
		else
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
			           "sht insert account %s,Uin:%u player fail",
			           pstPlayer->szAccount,
			           pstPlayer->stRoleData.Uin);
		}
	}

	
	return pstPlayer;
}

int player_acc_login(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSACCLOGINREQ *pstAccLogin = &pstCsPkg->Body.AccLoginReq;
	int iUin=pstAccLogin->Uin;
	Player *pstPlayerNew;
	CSPKG *pstCSRes = &pstEnv->stCSRes;
	CSACCLOGINRES *pstAccLoginRes;
	UinCache *pstUinCache;
	UinCache stUinHashData;
	char szMsg[DESC_LEN];
	TFRAMECMDSTART *pstStart;
	int iMinCltVer;
	int iMaxCltVer;
	int iDeQuick = 0;
	int iAlloc = 0;

	if (pstFrameHead->Cmd != TFRAMEHEAD_CMD_START)
	{
		return -1;
	}

	pstStart = &pstFrameHead->CmdData.Start;

	if (0 == pstEnv->pstConf->NotUinAuth)
	{
		if (pstStart->AuthType != AUTH_JC || pstStart->AuthData.AuthJC.Uin != pstAccLogin->Uin)
		{
			return -1;
		}
	}

	//server关闭过程中最后2分钟禁止登录
	if (pstEnv->cShutdown && pstEnv->ucShutdownCount <= 12)
	{
		player_acc_login_err(pstEnv, pstFrameHead, LOGIN_NOSERVICE);
		return -1;
	}
	
	if( pstEnv->iInstMode != INST_MODE_ZONE_SVR )
	{
		player_acc_login_err(pstEnv, pstFrameHead, LOGIN_ERR);
		return -1;
	}
	
	/*check clt proto ver*/
	if ( (0!= pstEnv->pstConf->MaxCltProtoVer && pstAccLogin->CltProtoVer > pstEnv->pstConf->MaxCltProtoVer) ||
		 (0!= pstEnv->pstConf->MinCltProtoVer && pstAccLogin->CltProtoVer < pstEnv->pstConf->MinCltProtoVer))
	{	
		tlog_info(pstEnv->pstLogCat, 0, 0, "client ip=%s port=%d proto ver %d",
					inet_ntoa(*(struct in_addr *)&pstFrameHead->ExtraInfo.IPInfo.ip), ntohs(pstFrameHead->ExtraInfo.IPInfo.port),
					pstAccLogin->CltProtoVer);
		player_acc_login_err(pstEnv, pstFrameHead, LOGIN_BADCLT);
		return -1;
	}

	/*check clt ver*/
	iMinCltVer = version_to_uint(pstEnv->pstConf->MinCltVer);
	iMaxCltVer = version_to_uint(pstEnv->pstConf->MaxCltVer);
	
	if ( (0!= iMaxCltVer && pstAccLogin->CltVer > iMaxCltVer) ||
		 (0!= iMinCltVer && pstAccLogin->CltVer < iMinCltVer))
	{	
		tlog_info(pstEnv->pstLogCat, 0, 0, "client ip=%s port=%d ver %s",
					inet_ntoa(*(struct in_addr *)&pstFrameHead->ExtraInfo.IPInfo.ip), ntohs(pstFrameHead->ExtraInfo.IPInfo.port),
					uint_to_version(pstAccLogin->CltVer));
		player_acc_login_err(pstEnv, pstFrameHead, LOGIN_BADCLT);
		return -1;
	}

	/*check black white ip*/
	if (black_white_check(pstEnv->pstBlackWhiteH, pstFrameHead->ExtraInfo.IPInfo.ip))
	{
		player_acc_login_err(pstEnv, pstFrameHead, LOGIN_NOSERVICE);
		return -1;
	}

	tlog_info(pstEnv->pstLogCat, iUin, 0, "client acc=%s uin=%d ip=%s port=%d ConnIdx=%d acc login",
				pstAccLogin->AccountLoginName, iUin, 
				inet_ntoa(*(struct in_addr *)&pstFrameHead->ExtraInfo.IPInfo.ip), 
				ntohs(pstFrameHead->ExtraInfo.IPInfo.port), pstFrameHead->ConnIdx);

	/*check login frequency limit*/
	stUinHashData.iUin = iUin;
	pstUinCache = (UinCache *)sht_find(pstEnv->pstShtUinCache,&stUinHashData,UinHashCmp,UinHashCode);
	if (pstUinCache && (pstUinCache->tLastLogin <= pstAppCtx->stCurr.tv_sec && pstUinCache->tLastLogin + pstEnv->pstConf->LoginInterval >= pstAppCtx->stCurr.tv_sec))
	{
		player_acc_login_err(pstEnv, pstFrameHead, LOGIN_FREQUENCY);

		return -1;
	}

	/*check player whether have been login  before*/
	pstPlayerNew = player_get_by_uin(pstEnv, iUin);
	if( pstPlayerNew != NULL )
	{
		//废除之前的顶号可接管
		{
			// 顶替一个已经登陆的客户端
			
			/*one player have open two connection, so kickoff player at preconnection*/
			tlog_info(pstEnv->pstLogCat, iUin, 0, "player(%d) have opened one connection(index:%d) address(%s:%d), so kickoff it",
				iUin, pstFrameHead->ConnIdx, 
				inet_ntoa(*(struct in_addr *)&pstFrameHead->ExtraInfo.IPInfo.ip),
				ntohs(pstFrameHead->ExtraInfo.IPInfo.port));

			sprintf(szMsg, LJY_KICK2, 
				inet_ntoa(*(struct in_addr *)&pstFrameHead->ExtraInfo.IPInfo.ip));
			/*
			if (pstEnv->pstConf->CountryVer == COUNTRY_VER_KOREA)
			{
				if (pstPlayerNew->eStatus == PLAYER_STATUS_ROLE_LOGIN)
				{
					z_send_sys_str(pstAppCtx, pstEnv, pstPlayerNew, SYS_MIDDLE2, szMsg);
				}
			}
			else*/
			{
				player_delay_kickoff(pstAppCtx, pstEnv, pstPlayerNew, szMsg);
			}

			//because kick the pre account maybe asyn, so stop this connect too
			if(pstFrameHead->ExtraInfo.IPInfo.ip == pstPlayerNew->stConnectInfo.stExtraInfo.IPInfo.ip)
			{
				player_acc_login_err(pstEnv, pstFrameHead, LOGIN_AGAIN_SAMEIP);	
			}
			else
			{
				pstPlayerNew->iGrabTime = GRAB_SAFE_TIME*60+pstEnv->pstAppCtx->stCurr.tv_sec;
				player_acc_login_err(pstEnv, pstFrameHead, LOGIN_REP);
			}

			return -1;
		}
	}
	else
	{
		/*alloc place for player at new connection*/
		if (pstEnv->pstPlayerPool->iUsed >= MAX_PLAYER_SOFT && 
			(0 == ((UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN) & pstStart->AuthData.AuthJC.UinFlag)))
		{
			player_acc_login_err(pstEnv, pstFrameHead, LOGIN_SVRBUSY);
			return -1;
		}
		if(pstUinCache)
		{
			pstPlayerNew = player_alloc(pstAppCtx, pstEnv, iUin, pstAccLogin->AccountLoginName, pstUinCache);
			iAlloc = 1;
		}
		else
		{
			pstPlayerNew = player_alloc(pstAppCtx, pstEnv, iUin, pstAccLogin->AccountLoginName, NULL);
			iAlloc = 1;
		}
		if (NULL == pstPlayerNew)
		{
			player_acc_login_err(pstEnv, pstFrameHead, LOGIN_SVRBUSY);

			tlog_error(pstEnv->pstLogCat, iUin , 0, "recv Uin=%d acc login, but player_alloc NULL",
						iUin);
			return -1;
		}

		pstPlayerNew->stRoleData.Uin = pstAccLogin->Uin;
		pstPlayerNew->iGrabTime = 0;
	}

	// 更新帐号禁言结束时间
	pstPlayerNew->iAccSilenceEnd = pstStart->AuthData.AuthJC.AccSilenceEnd;
	
	pstPlayerNew->unCltProVer = pstAccLogin->CltProtoVer;	

	if (0 == pstPlayerNew->stOnline.bWaitRoleLogout)
	{
		pstPlayerNew->eStatus = PLAYER_STATUS_ACC_LOGIN;
		tlog_info(pstEnv->pstLogCat, 0, 0, "Status=AccLogin Uin=%d AccName=%s Idx %d memid=%d", 
			pstPlayerNew->stRoleData.Uin, pstPlayerNew->szAccount, pstPlayerNew->iIdx, pstPlayerNew->iMemID);
	}
	else 
	{
		if (ROLE_LOGOUT_CLOSE == pstPlayerNew->ucRoleLogoutReason)
		{
			pstPlayerNew->ucRoleLogoutReason = ROLE_LOGOUT_GRAB;
			tlog_info(pstEnv->pstLogCat, 0, 0, "Status=AccLogin grab Uin=%d AccName=%s Idx %d memid=%d", 
				pstPlayerNew->stRoleData.Uin, pstPlayerNew->szAccount, pstPlayerNew->iIdx, pstPlayerNew->iMemID);
		}
	}
	
	pstPlayerNew->tActive = pstAppCtx->stCurr.tv_sec;

	/*store fream head info to connect info*/
	pstPlayerNew->stConnectInfo.chVer = pstFrameHead->Ver;
	pstPlayerNew->stConnectInfo.chCmd = TFRAMEHEAD_CMD_START;
	pstPlayerNew->stConnectInfo.chExtraType = pstFrameHead->ExtraType;
	memcpy(&pstPlayerNew->stConnectInfo.stExtraInfo, &pstFrameHead->ExtraInfo, sizeof(pstPlayerNew->stConnectInfo.stExtraInfo));
	pstPlayerNew->stConnectInfo.chTimeStampType = pstFrameHead->TimeStampType;
	memcpy(&pstPlayerNew->stConnectInfo.stTimeStamp, &pstFrameHead->TimeStamp, sizeof(pstPlayerNew->stConnectInfo.stTimeStamp));
	pstPlayerNew->stConnectInfo.iConnIdx = pstFrameHead->ConnIdx;
	pstPlayerNew->stConnectInfo.iID = pstPlayerNew->iIdx;
	pstPlayerNew->stOnline.bConnLost = 0;
	pstPlayerNew->UinFlag = pstStart->AuthData.AuthJC.UinFlag;

	if ((pstPlayerNew->UinFlag & UIN_FLAG_QUICK) && 
		iDeQuick)
	{
		pstPlayerNew->UinFlag &= ~UIN_FLAG_QUICK;
	}

	if (pstPlayerNew->UinFlag & UIN_FLAG_QUICK)
	{
		pstPlayerNew->iUinStatus = QUICK_STATUS_QUICK;
	}
	pstPlayerNew->iCltType = pstAccLogin->CltType;
	STRNCPY(pstPlayerNew->aMacAdd, pstAccLogin->MacAdd, sizeof(pstPlayerNew->aMacAdd));

	if (iAlloc)
	{
		z_acc_login_oplog(pstAppCtx, pstEnv,iUin, pstAccLogin->AccountLoginName, 1,pstPlayerNew);
	}
	
	Z_CSHEAD_INIT(&pstCSRes->Head, ACC_LOGIN_RES);
	pstAccLoginRes = &pstCSRes->Body.AccLoginRes;
	pstAccLoginRes->Result = LOGIN_SUCC;
	pstAccLoginRes->SvrProtoVer = TDR_METALIB_PROTO_CS_VERSION;

	if (0 > z_cltmsg_send(pstEnv, pstPlayerNew, pstCSRes))
	{
		player_acc_login_err(pstEnv, pstFrameHead, LOGIN_SVRBUSY);

		tlog_error(pstEnv->pstLogCat, iUin , 0, "recv Uin=%d acc login, but z_cltmsg_send fail",
					iUin);

		if (0 == pstPlayerNew->stOnline.bWaitRoleLogout)
		{
			z_free_player(pstAppCtx, pstEnv,pstPlayerNew);
		}
		return -1;
	}
	
	pstPlayerNew->stConnectInfo.chCmd = TFRAMEHEAD_CMD_INPROC;

	//防沉迷
	player_acc_login_aas_check(pstEnv, pstPlayerNew, pstStart->AuthData.AuthJC.BirthDay);
	
	z_heartbeat_to_region(pstEnv, pstPlayerNew->stRoleData.Uin);

	// 记录帐号最后一次登录时间与IP
	z_account_db_update_archive(pstEnv, pstPlayerNew);

	{
		SSCMDDATA stData;
		
		stData.AccUin = pstPlayerNew->stRoleData.Uin;
		z_cmd_to_mud(pstEnv,NULL, NULL,SS_CMD_ACC_NOTICE, &stData);
	}

	pstPlayerNew->GMLimitPerm = z_check_gm_ip(pstAppCtx, pstEnv, pstPlayerNew, pstAccLogin->MacAdd);

	if (pstUinCache && pstUinCache->cGetRoleList)
	{
		if (0 == pstPlayerNew->stOnline.bWaitRoleLogout)
		{
			pstPlayerNew->eStatus = PLAYER_STATUS_ROLE_LIST;
			tlog_info(pstEnv->pstLogCat, 0, 0, "Status=RoleList Uin=%d AccName=%s", 
					pstPlayerNew->stRoleData.Uin, pstPlayerNew->szAccount);
		}
		
		return z_send_rolelist(pstEnv, pstPlayerNew, pstUinCache);
	}
	else
	{
		//send role list req
		return z_db_role_list(pstAppCtx, pstEnv, pstPlayerNew);
	}
	UNUSED(pstPlayer);	//add by paraunused.pl
}

int player_role_new(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSROLENEWREQ *pstNewReq = &pstCsPkg->Body.RoleNewReq;
	CSPKG *pstPkgRes = &pstEnv->stCSRes;
	CSROLENEWRES *pstRoleNewRes = &pstPkgRes->Body.RoleNewRes;

	if( pstEnv->iInstMode != INST_MODE_ZONE_SVR )
	{
		return 0;
	}

	//不可以创建角色
	if(pstEnv->pstConf->NoCreateRole)
	{
		memset(pstRoleNewRes, 0, sizeof(*pstRoleNewRes));
		pstRoleNewRes->Result = NEW_NOCREATE;

		Z_CSHEAD_INIT(&pstPkgRes->Head, ROLE_NEW_RES);
		z_cltmsg_send(pstEnv, pstPlayer, pstPkgRes);

		return -1;
	}
	
	if (NULL == pstPlayer || pstPlayer->stOnline.cCheckName)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "player_role_new player null");
		return -1;
	}

	pstPlayer->tActive = pstAppCtx->stCurr.tv_sec;

	if (pstNewReq->Career < CAREER_WARRIOR || pstNewReq->Career > CAREER_CHURCH ||
		pstNewReq->Gender < GENDER_MAN || pstNewReq->Gender > GENDER_WOMAN || 
		pstNewReq->Head > HEAD_MAX || pstNewReq->Face > FACE_MAX || pstNewReq->Hair > HAIR_MAX )
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player new req error",
					pstPlayer->stRoleData.Uin);
		return -1;
	}

	if (0 == pstEnv->pstConf->CountryVer || COUNTRY_VER_MALAI == pstEnv->pstConf->CountryVer)
	{
		if (CheckCharacter(pstNewReq->RoleName) || ((CheckDirty(pstNewReq->RoleName , 0x1f) & 0x1f )))
		{
			memset(pstRoleNewRes, 0, sizeof(*pstRoleNewRes));
			pstRoleNewRes->Result = NEW_NAME_CHECK;
			STRNCPY(pstRoleNewRes->RoleInfo.RoleName, pstNewReq->RoleName, sizeof(pstRoleNewRes->RoleInfo.RoleName));

			Z_CSHEAD_INIT(&pstPkgRes->Head, ROLE_NEW_RES);
			z_cltmsg_send(pstEnv, pstPlayer, pstPkgRes);
			
			return -1;
		}
	}
	else 
	{
		if (CheckCharacter1(pstNewReq->RoleName))
		{
			memset(pstRoleNewRes, 0, sizeof(*pstRoleNewRes));
			pstRoleNewRes->Result = NEW_NAME_CHECK;
			STRNCPY(pstRoleNewRes->RoleInfo.RoleName, pstNewReq->RoleName, sizeof(pstRoleNewRes->RoleInfo.RoleName));

			Z_CSHEAD_INIT(&pstPkgRes->Head, ROLE_NEW_RES);
			z_cltmsg_send(pstEnv, pstPlayer, pstPkgRes);
			
			return -1;
		}
	}

	return z_db_role_insert(pstAppCtx, pstEnv, pstPlayer, pstNewReq);
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

int player_acc_share_init(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	memset(&pstPlayer->stAccShareData.stAccShare, 0, sizeof(pstPlayer->stAccShareData.stAccShare));
	pstPlayer->stAccShareData.iDBQureyTime = 0;
	pstPlayer->stAccShareData.iStat = ACC_SHARE_INIT;


	z_db_acc_share_select(pstEnv->pstAppCtx,pstEnv,pstPlayer);
	return 0;
}

int player_acc_share_final(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	pstPlayer->stAccShareData.iStat = ACC_SHARE_FINAL;

	z_player_awardexp_calculate(pstEnv, pstPlayer);
	db_select_shadow_req (pstEnv,pstPlayer,pstPlayer->stRoleData.RoleID);

	if (pstPlayer->stAccShareData.stAccShare.GrabTime > pstPlayer->iGrabTime)
	{
		pstPlayer->iGrabTime = pstPlayer->stAccShareData.stAccShare.GrabTime;
	}

	return 0;
}

int player_use_offline_exp(ZONESVRENV *pstEnv,Player *pstPlayer, tdr_longlong *piExp)
{
	AccShareData* pstData = &pstPlayer->stAccShareData;
	int iAward = 0;
	
	if (ACC_SHARE_FINAL != pstData->iStat)
	{
		return -1;
	}

	iAward = min(2* (*piExp),pstData->stAccShare.OfflineExp);
	if (iAward > 0)
	{
		*piExp += iAward;
		//z_player_awardexp_change(pstEnv,pstPlayer,-iAward,TRUE);

		z_player_awardexp_change(pstEnv,pstPlayer,-iAward,TRUE);	
	}

	return 0;
	
}

int player_role_login(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	UinCache stUinHashData;
	UinCache *pstUinCache=NULL;
	int i;
	//是否是预创建角色
	if( pstEnv->pstConf->PreCreateRole == 1 )
	{
		CSPKG *pstPkg = &pstEnv->stCSRes;
		CSROLELOGINRES *pstRoleLoginRes = &pstPkg->Body.RoleLoginRes;
		unsigned short   unWorldID;
		GET_WORLD_ID(unWorldID,pstEnv->pstAppCtx->iId);
		memset(pstRoleLoginRes,0,sizeof(*pstRoleLoginRes));
		Z_CSHEAD_INIT(&pstPkg->Head, ROLE_LOGIN_RES);
		pstRoleLoginRes->Result = LOGIN_PRECREATEROLE;
		pstRoleLoginRes->EnterFlag = 0;
		pstRoleLoginRes->WorldID = unWorldID;
		pstRoleLoginRes->RoleMemID = pstPlayer->iMemID;
		pstRoleLoginRes->MaxLevel = pstEnv->pstConf->MaxLevel;
		if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat != GLOBAL_FINI)
		{
			pstRoleLoginRes->LevelFeng.LevelFeng = 0;
			pstRoleLoginRes->LevelFeng.LevelFengEnd = 0;
		}
		else
		{
			pstRoleLoginRes->LevelFeng = pstEnv->pstGlobalObj->stGlobal.LevFeng;
		}

		if (pstPlayer)
		{
			pstRoleLoginRes->IP = pstPlayer->stConnectInfo.stExtraInfo.IPInfo.ip;
		}
		pstRoleLoginRes->PingCheck = pstEnv->pstConf->PingCheck;
		z_cltmsg_send(pstEnv, pstPlayer, pstPkg);
		return -1;
	}
	if( pstEnv->iInstMode != INST_MODE_ZONE_SVR )
	{
		return 0;
	}
		
	if (NULL == pstPlayer)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "player null");
		return -1;
	}

	stUinHashData.iUin = pstPlayer->stRoleData.Uin;
	pstUinCache = (UinCache *)sht_find(pstEnv->pstShtUinCache,&stUinHashData,UinHashCmp,UinHashCode);
	if (!pstUinCache)
	{
		return -1;
	}

	for (i=0; i<pstUinCache->ucRoleNum; i++)
	{
		if (pstUinCache->astRoleListInfo[i].RoleID == pstCsPkg->Body.RoleLoginReq.RoleID)
		{
			/*隐藏角色*/
			if (i>=4)
			{
				return -1;
			}

			/*被删除的角色*/
			if (pstUinCache->astRoleListInfo[i].DelTime > 0)
			{
				return -1;
			}

		}
		
		pstPlayer->stAccShareData.iAccMaxRoleLevel = 
			max(pstPlayer->stAccShareData.iAccMaxRoleLevel,pstUinCache->astRoleListInfo[i].Level); 
	}

	pstPlayer->tActive = pstAppCtx->stCurr.tv_sec;
	if( pstPlayer->stOnline.bWaitRoleLogout )
	{
		return -1;		
	}
	else
	{
		if (pstPlayer->eStatus != PLAYER_STATUS_ROLE_LIST)
		{
			return -1;
		}
		
		// 真实登陆
		/*
		if( pstCsPkg->Body.RoleLoginReq.RoleID == pstPlayer->stRoleData.RoleID &&
			(pstPlayer->stOnline.eStatusAssistant&PLAYER_STATUS_BATTLE_LOGIN_READY) )
		{
			SSBATTLECAMPINFO stBattleCampInfo = pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;
			int eStatusAssistant = pstPlayer->stOnline.eStatusAssistant;

			memset(&pstPlayer->stOnline, 0, sizeof(pstPlayer->stOnline));


			pstPlayer->stOnline.eStatusAssistant = eStatusAssistant;
			pstPlayer->stOnline.stPworldQueue.stBattleCampInfo = stBattleCampInfo;
		}
		else
		*/
		{
			memset(&pstPlayer->stOnline, 0, sizeof(pstPlayer->stOnline));
		}
		
		memcpy(&pstPlayer->stOnline.stConnectTime, &pstFrameHead->TimeStamp.TimeVal, 
				sizeof(pstPlayer->stOnline.stConnectTime));

		pstPlayer->stOnline.iWorldCityBuffFlag = -1;

		
		//韩国版本 或者台湾版本登录时要弹出二级密码界面
		if(COUNTRY_VER_KOREA  == pstEnv->pstConf->CountryVer || COUNTRY_VER_CHINA_TAIWAN == pstEnv->pstConf->CountryVer)
		{
			pstPlayer->stOnline.iRoleID = pstCsPkg->Body.RoleLoginReq.RoleID;
			pstPlayer->stOnline.iSecondPwdCmd = 1;// 1代表登录操作
			
			//发起获得二级密码请求
			if(0 > z_select_role_secondpwd(pstEnv, pstPlayer))
			{
				return -1;
			}
		}
		else
		{
			if(z_db_role_select(pstAppCtx, pstEnv, pstPlayer, &pstCsPkg->Body.RoleLoginReq)< 0)
			{
				return -1;
			}
		}
	}
	pstPlayer->stOnline.stFrameHead = *pstFrameHead;
	
	return 0;
}

int player_role_logout(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	UinCache *pstUinCache;
	int iValid;
//	int i;

	//先更新rolelist
	pstUinCache = (UinCache *)sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos, &iValid);
	if (NULL == pstUinCache || 0 == iValid || pstUinCache->iUin != pstPlayer->stRoleData.Uin)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
					"player_role_logout Uin=%d uincache", pstPlayer->stRoleData.Uin);
		return -1;
	}

	player_sparta_team_chg_clean(pstEnv, pstPlayer);
		
	//异步模式，更新完数据再退出
	player_switch_role_delay_logout( pstEnv, pstPlayer, ROLE_LOGOUT_CLTREQ );

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstFrameHead);	//add by paraunused.pl
	UNUSED(pstCsPkg);	//add by paraunused.pl
}

int player_cancel_role_logout(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	UNUSED( pstAppCtx );
	UNUSED( pstEnv );
	UNUSED( pstFrameHead );
	UNUSED( pstCsPkg );

	if (1 == pstPlayer->stOnline.bWaitRoleLogout && pstPlayer->ucRoleLogoutReason == ROLE_LOGOUT_CLTREQ)
	{
		pstPlayer->stOnline.bWaitRoleLogout = 0;
		pstPlayer->ucRoleLogoutReason = 0;
	}
	
	return 0;
}

int player_acc_logout_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG *pstCSPkgRes = &pstEnv->stCSRes;
	CSACCLOGOUTRES *pstAccLogoutRes = &pstCSPkgRes->Body.AccLogoutRes;
	
	pstAccLogoutRes->Uin = pstPlayer->stRoleData.Uin;

	pstPlayer->eStatus = PLAYER_STATUS_ACC_LOGOUT;

			
	pstAccLogoutRes->Result = LOGOUT_SUCC;

	if (MISC_FLAG_LOGOUT_FOR_MUD & pstPlayer->stOnline.cMiscFlag)
	{
		pstAccLogoutRes->Result = LOGOUT_FOR_MUD_SUCC;
	}

	tlog_info(pstEnv->pstLogCat, 0, 0, "Status=RoleAccLogOut Uin=%d AccName=%s Result=%d", 
					pstPlayer->stRoleData.Uin, pstPlayer->szAccount,pstAccLogoutRes->Result);
	
	Z_CSHEAD_INIT(&pstCSPkgRes->Head, ACC_LOGOUT_RES);
	z_cltmsg_send_stop(pstEnv, pstPlayer, pstCSPkgRes);

	z_free_player(pstAppCtx, pstEnv, pstPlayer);
	return 0;
}

int player_acc_logout(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	
	if (NULL == pstPlayer )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "player null");
		return -1;
	}

	pstPlayer->ucRoleLogoutReason = ROLE_LOGOUT_ACC;
	
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_LOGOUT_FOR_MUD;	
	if (LOGOUT_FOR_MUD == pstCsPkg->Body.AccLogoutReq.LogoutFor)
	{
		pstPlayer->stOnline.cMiscFlag &= MISC_FLAG_LOGOUT_FOR_MUD;
	}
	
	switch( pstPlayer->eStatus )
	{
	case PLAYER_STATUS_ROLE_LOGIN:
		player_switch_role_delay_logout(pstEnv, pstPlayer, ROLE_LOGOUT_ACC);
		break;

	case PLAYER_STATUS_ROLE_LOGOUT_REQ:
		break;

	default:
		if (pstPlayer->stOnline.bWaitRoleLogout)
		{
			break;
		}
		
		player_acc_logout_res(pstAppCtx,pstEnv,pstPlayer);		
		
		break;
	}
	
	return 0;
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

int player_heartbeat(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	struct timeval stCltTime;
	struct timeval stConnectTime;
	CSTIME *pstCSTime = &pstCsPkg->Body.HeartBeatReq.SvrTime;
	CSPKG stPkg;
	CSHEARTBEATRES *pstRes = &stPkg.Body.HeartBeatRes;
	

	if (NULL == pstPlayer)
	{
		tlog_info(pstEnv->pstLogCat, 0, 0, "player uin:%d heartbeat null",
			pstFrameHead->CmdData.Start.AuthData.AuthJC.Uin);

		player_acc_login_err(pstEnv, pstFrameHead, LOGIN_NOSERVICE);
		return -1;
	}

	tlog_debug(pstEnv->pstLogCat, 0, 0, "player heartbeat req Uin:%d,eStatus:%d",
					pstPlayer->stRoleData.Uin,
					pstPlayer->eStatus);

	if (PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus)
	{
		stCltTime.tv_sec = pstCSTime->BenchTimeSec;
		stCltTime.tv_usec = pstCSTime->BenchTimeUSec;

		stConnectTime.tv_sec = pstFrameHead->TimeStamp.TimeVal.sec;
		stConnectTime.tv_usec = pstFrameHead->TimeStamp.TimeVal.usec;

		z_adjust_time(&stCltTime, &stConnectTime, pstPlayer);

		pstRes->CltTime = *pstCSTime;
		pstRes->ConnectTime.BenchTimeSec = pstFrameHead->TimeStamp.TimeVal.sec;
		pstRes->ConnectTime.BenchTimeUSec = pstFrameHead->TimeStamp.TimeVal.usec;

		Z_CSHEAD_INIT(&stPkg.Head, HEARTBEAT_RES);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	if(INST_MODE_ZONE_SVR == pstEnv->iInstMode)
	{	
		if(PLAYER_STATUS_ROLE_NOTHING != pstPlayer->eStatus)
		{
			pstPlayer->tActive = pstAppCtx->stCurr.tv_sec;
		}
	}
	else if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
	{
		pstPlayer->tActive = pstAppCtx->stCurr.tv_sec;
		player_span_heartbeat_req(pstEnv, pstPlayer);
	}

	return 0;
}

int player_role_del_level(ZONESVRENV* pstEnv,tdr_ulonglong ullDelRoleID,Player *pstPlayer)
{
	UinCache stUinHashData;
	UinCache *pstUinCache=NULL;
	int i;
	
	stUinHashData.iUin = pstPlayer->stRoleData.Uin;
	pstUinCache = (UinCache *)sht_find(pstEnv->pstShtUinCache,&stUinHashData,UinHashCmp,UinHashCode);
	if (!pstUinCache)
	{
		return -1;
	}

	for (i=0; i<pstUinCache->ucRoleNum; i++)
	{
		if (pstUinCache->astRoleListInfo[i].RoleID == ullDelRoleID)
		{
			return pstUinCache->astRoleListInfo[i].Level;
		}
	}
	return -1;
}

int player_role_del(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	int iLevel;
	if( pstEnv->iInstMode != INST_MODE_ZONE_SVR )
	{
		return 0;
	}
		
	if (NULL == pstPlayer)
	{
		return -1;
	}
	
	if (PLAYER_STATUS_ROLE_LIST != pstPlayer->eStatus)
	{
		return -1;
	}

	if(0 < shopping_delete_user_all_traderecord( pstAppCtx,  pstEnv,pstCsPkg->Body.RoleDelReq.RoleID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
						ZONEERR_PLAYER104);
		return -1;
	}
	
	if(z_clan_leader_id( pstEnv, pstCsPkg->Body.RoleDelReq.RoleID ) )
	{
		CSPKG *pstPkg = &pstEnv->stCSRes;
		CSROLEDELRES *pstDelRes = &pstPkg->Body.RoleDelRes;

		pstDelRes->RoleID = pstCsPkg->Body.RoleDelReq.RoleID;
		pstDelRes->Result = DEL_ERR_CLAN_LEADER;
		Z_CSHEAD_INIT(&pstPkg->Head, ROLE_DEL_RES);
		z_cltmsg_send(pstEnv, pstPlayer, pstPkg);
		return -1;
	}

	iLevel = player_role_del_level(pstEnv,pstCsPkg->Body.RoleDelReq.RoleID,pstPlayer);
	if (iLevel <= 0) return -1;

	
	//linzhongkai   韩国版本或者台湾版本 登录时要弹出二级密码界面
	if(COUNTRY_VER_KOREA  == pstEnv->pstConf->CountryVer || COUNTRY_VER_CHINA_TAIWAN == pstEnv->pstConf->CountryVer)
	{		
		pstPlayer->stOnline.iRoleID = pstCsPkg->Body.RoleDelReq.RoleID;
		pstPlayer->stOnline.iSecondPwdCmd = 2;// 2代表删除操作
		
		//发起获得二级密码请求
		return z_select_role_secondpwd(pstEnv, pstPlayer);
	}
	else
	{
		// 真正要删除的时候才更新金卷
		if (iLevel < DELETE_ROLE_LEVEL_LIMIT)
		{
			return z_db_get_roledetail_req(pstAppCtx, pstEnv, pstPlayer, &pstCsPkg->Body.RoleDelReq);
		}
		else
		{
			return z_role_del_respite(pstEnv, pstPlayer, &pstCsPkg->Body.RoleDelReq);
		}
	}
	
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

int z_new_player_gift_fittings(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}


int z_new_player_gift(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	//ROLEGRID stRoleGrid;
	//CSWEAR stCSWear;
	LEVELUP *pstLevelUp = NULL;

	pstLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);

	z_player_add_gift(pstAppCtx, pstEnv, pstPlayer);
	
	if (pstPlayer->stRoleData.Career== CAREER_WARRIOR  )
	{
		if (pstLevelUp )
		{
			for (i=0; i<(int)(sizeof(pstLevelUp->WarriorSkillList)/sizeof(pstLevelUp->WarriorSkillList[0])); i++)
			{
				if (pstLevelUp->WarriorSkillList[i] <= 0)
				{
					break;
				}
				z_add_skill(pstAppCtx, pstEnv, pstPlayer, NULL, pstLevelUp->WarriorSkillList[i], 1, 1);
			}
		}
		
	}
	else if(pstPlayer->stRoleData.Career== CAREER_GUNER)
	{
		if (pstLevelUp )
		{
			for (i=0; i<(int)(sizeof(pstLevelUp->GunerSkillList)/sizeof(pstLevelUp->GunerSkillList[0])); i++)
			{
				if (pstLevelUp->GunerSkillList[i] <= 0)
				{
					break;
				}
				z_add_skill(pstAppCtx, pstEnv, pstPlayer, NULL, pstLevelUp->GunerSkillList[i], 1, 1);
			}
		}

	}
	else if (pstPlayer->stRoleData.Career== CAREER_MAGICOR)
	{
		if (pstLevelUp )
		{
			for (i=0; i<(int)(sizeof(pstLevelUp->MagicorSkillList)/sizeof(pstLevelUp->MagicorSkillList[0])); i++)
			{
				if (pstLevelUp->MagicorSkillList[i] <= 0)
				{
					break;
				}
				z_add_skill(pstAppCtx, pstEnv, pstPlayer, NULL, pstLevelUp->MagicorSkillList[i], 1, 1);
			}
		}
		
	}

	else if (pstPlayer->stRoleData.Career== CAREER_CHURCH)
	{
		if (pstLevelUp )
		{
			for (i=0; i<(int)(sizeof(pstLevelUp->ChurchSkillList)/sizeof(pstLevelUp->ChurchSkillList[0])); i++)
			{
				if (pstLevelUp->ChurchSkillList[i] <= 0)
				{
					break;
				}
				z_add_skill(pstAppCtx, pstEnv, pstPlayer, NULL, pstLevelUp->ChurchSkillList[i], 1, 1);
			}
		}
		
	}
	
	// 出生获得默认配件
	z_new_player_gift_fittings(pstAppCtx, pstEnv, pstPlayer);
	
	// 出生获得3个栏位 骑术
	pstPlayer->stRoleData.MiscInfo.RideList.MaxNum = 4;
	ride_list_chg( pstEnv, pstPlayer);

	pstPlayer->stRoleData.MiscInfo.PetSkillCollectTime = pstAppCtx->stCurr.tv_sec/3600;
	pstPlayer->stOnline.stDyn.stMidAttrList.uiDayExpMax = get_player_dayexplimit(pstAppCtx, pstEnv, pstPlayer);

	return 0;
}


int online_award(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSONLINEAWARDSVR *pstOnlineAwardSvr = &stPkg.Body.OnlineAwardSvr;
	ONLINEAWARDDEF *pstOnlineAwardDef;

	if (pstPlayer->stRoleData.MiscInfo.OnlineAwardIdx < 0)
	{
		return -1;
	}
	
	pstOnlineAwardDef = find_online_award_def(pstEnv, 
					pstPlayer->stRoleData.MiscInfo.OnlineAwardIdx, ONLINE_AWARD_DEF_DATA);
	if (pstOnlineAwardDef)
	{
		pstOnlineAwardSvr->AwardItemID = pstOnlineAwardDef->AwardItemID;
		pstOnlineAwardSvr->AwardTime = pstPlayer->stRoleData.MiscInfo.OnlineAwardEnd;
	}
	else
	{
		pstPlayer->stRoleData.MiscInfo.OnlineAwardIdx = -1;
		pstPlayer->stRoleData.MiscInfo.OnlineAwardEnd = 0;
		pstOnlineAwardSvr->AwardItemID = 0;
		pstOnlineAwardSvr->AwardTime = 0;
	}

	Z_CSHEAD_INIT(&stPkg.Head, ONLINE_AWARD_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_online_award(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						     TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSONLINEAWARDCLT *pstClt = &pstCsPkg->Body.OnlineAwardClt;
	//ITEMDEF *pstItemDef = z_find_itemdef(pstEnv, pstClt->AwardItemID);
	ROLEGRID stRoleGrid;
	int iRet = -1;
	ONLINEAWARDDEF *pstOnlineAwardDef;
	int iPreAwardTime;
	ITEMDEF *pstItemDef;

	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
		return -1;
	}

	if (pstPlayer->stOnline.State & CS_STAT_STALL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER105);
		return 0;
	}

	if(pstPlayer->stOnline.stExchgOnline.bExchgStat > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER106);
		return 0;
	}
	
	pstOnlineAwardDef = find_online_award_def(pstEnv, 
					pstPlayer->stRoleData.MiscInfo.OnlineAwardIdx, ONLINE_AWARD_DEF_DATA);

	if (!pstOnlineAwardDef || pstOnlineAwardDef->AwardItemID != pstClt->AwardItemID)
	{
		return -1;
	}

	iPreAwardTime = pstOnlineAwardDef->OnlineTime;
	pstItemDef = z_find_itemdef( pstEnv, pstOnlineAwardDef->AwardItemID);
	if (!pstItemDef)
	{
		return -1;
	}

	if (0 == item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid,pstClt->AwardItemID))
	{
		stRoleGrid.GridData.RoleItem.Num = 1;
		if (0 == package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid,  1,0))
		{
			package_add(pstEnv->pstAppCtx , pstEnv , pstPlayer, &stRoleGrid, 1,0, IMPITEM_OPTYPE_GIFT);
			iRet =  0;
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
		}
	}

	if (iRet == 0)
	{
		pstPlayer->stRoleData.MiscInfo.OnlineAwardIdx ++;
		pstOnlineAwardDef = find_online_award_def(pstEnv, 
					pstPlayer->stRoleData.MiscInfo.OnlineAwardIdx, ONLINE_AWARD_DEF_DATA);
		if (pstOnlineAwardDef)
		{
			pstPlayer->stRoleData.MiscInfo.OnlineAwardEnd = (pstOnlineAwardDef->OnlineTime -iPreAwardTime) * 60;
		}
		online_award(pstEnv,pstPlayer);
		z_role_gift_log(pstEnv, pstPlayer, pstPlayer->stRoleData.RoleName , GIFT_TYPE_CONTI,pstItemDef->Name, pstOnlineAwardDef->AwardItemID,0);
		return 0;
	}
	else
	{
		return -1;
	}
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstFrameHead);	//add by paraunused.pl
}


NEWROLEGIFT *new_role_gift_find(ZONESVRENV* pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	NEWROLEGIFT stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.ID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sNewRoleGiftDef, pstObj->iNewRoleGiftDef, NEW_ROLE_GIFT_DEF_SIZE, &iEqu);
	if (iEqu)
	{
		return (NEWROLEGIFT*)pstObj->sNewRoleGiftDef[iIndex];
		
	}
	return NULL;
}

CONTCOVERGIFT *cont_cover_gift_find(ZONESVRENV* pstEnv, int iDay)
{
	int iIndex;
	int iEqu;
	CONTCOVERGIFT stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.Day = iDay;
	iIndex = bsearch_int(&stDef, pstObj->sContCoverGiftDef, pstObj->iContCoverGiftDef, CONT_COVER_DEF_SIZE, &iEqu);
	if (iEqu)
	{
		return (CONTCOVERGIFT*)pstObj->sContCoverGiftDef[iIndex];
		
	}
	return NULL;
}

// 角色登录时调用
int player_new_role_gift_init(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	NEWROLEGIFTINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.NewRoleGiftInfo; 
	NEWROLEGIFT *pstDef;

	// 新角色
	if (	pstInfo->NewRole &&
		pstInfo->ID == 0)
	{
		pstDef = new_role_gift_find( pstEnv, 1);
		if (!pstDef)
		{
			pstInfo->NewRole = 0;
			return 0;
		}
		
		pstInfo->ID = 1;
		pstInfo->Sec = pstDef->Sec;
		pstInfo->End = pstEnv->pstAppCtx->stCurr.tv_sec+pstDef->Sec;
		// todo  通知客户端:
		player_new_role_gift_info(pstEnv, pstPlayer);
		return 0;
	}

	// 新角色退出之后登陆
	if (	pstInfo->NewRole &&
		pstInfo->ID > 0)
	{
		pstDef = new_role_gift_find( pstEnv, pstInfo->ID);
		if (!pstDef)
		{			
			pstInfo->NewRole = 0;
			return 0;		
		}
		pstInfo->End = pstEnv->pstAppCtx->stCurr.tv_sec+pstInfo->Sec;

		// todo  通知客户端:
		player_new_role_gift_info(pstEnv, pstPlayer);
	}
	
	return 0;
}

// 角色退出时调用
int player_logout_new_gift(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	NEWROLEGIFTINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.NewRoleGiftInfo; 

	if (	pstInfo->NewRole &&
		pstInfo->ID > 0)
	{
		pstInfo->Sec = pstInfo->End-pstEnv->pstAppCtx->stCurr.tv_sec;
		if (pstInfo->Sec < 0)
		{
			pstInfo->Sec = 0;
		}
	}
	return 0;
}

int player_role_selbirth(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	int i;
	int iSelMapID;
	ZONEIDX stIdx;
	BirthMapSort astBirMapSort[MAX_BIRTHMAP_DEF];
	ZONEMAPINST *pstMapInst = NULL;
	BIRTHMAPDEF *pstBirthDef = NULL;
	RESPOS stBirthPos;
	ROLEPACKAGE *pstRolePackage;
	MapIndex *pstMapIdx;
	int iMapPos;
	int iCameraDir, iDir;
	TASKRUNONE *pstTaskRunOne;
	TASKDEF *pstTaskDef = NULL;
	CSELECTBIRTHLOCC *pstSelectBirth = &pstCsPkg->Body.SelectBirthLocC;
	ONLINEAWARDDEF *pstOnlineAwardDef;

	if( pstEnv->iInstMode != INST_MODE_ZONE_SVR )
	{
		return 0;
	}
	
	if (NULL == pstPlayer)
	{
		return -1;
	}

	if (0 == pstSelectBirth->MapID)
	{
		pstPlayer->eStatus = PLAYER_STATUS_ROLE_LIST;
		tlog_info(pstEnv->pstLogCat, 0, 0, "Status=RoleList Uin=%d AccName=%s", 
							pstPlayer->stRoleData.Uin, pstPlayer->szAccount);
		return 0;
	}

	pstRolePackage = &pstPlayer->stRoleData.Package;
	iSelMapID = pstSelectBirth->MapID;
	
	for (i=0; i<pstEnv->pstZoneObj->iBirthMapDef; i++)
	{
		pstBirthDef = (BIRTHMAPDEF *)pstEnv->pstZoneObj->sBirthMapDef[i];

		stIdx.iID = pstBirthDef->MapID;
		stIdx.iPos = -1;
		pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
		if (NULL == pstMapInst)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "map %d no map inst", stIdx.iID);
			return -1;
		}

		if (pstSelectBirth->MapID == (int)pstBirthDef->MapID && pstMapInst->iPlayerCount < NEW_VALLEGE_DISP*5)
			break;

		astBirMapSort[i].iMap = pstBirthDef->MapID;
		astBirMapSort[i].iPlayerNum = pstMapInst->iPlayerCount;
		astBirMapSort[i].DefIdx = i;
	}

	if (i >= pstEnv->pstZoneObj->iBirthMapDef)
	{
		qsort(astBirMapSort, pstEnv->pstZoneObj->iBirthMapDef, sizeof(astBirMapSort[0]), trl_comp_func);

		pstSelectBirth->MapID = astBirMapSort[0].iMap;
		pstBirthDef = (BIRTHMAPDEF *)pstEnv->pstZoneObj->sBirthMapDef[astBirMapSort[0].DefIdx];

	}
	if( pstBirthDef == NULL )
	{
		return -1;
	}
	if ( 0 >= pstBirthDef->PosNum)
	{
		tlog_debug(pstEnv->pstLogCat, 0, 0, "birth map %d pos num <= 0", pstBirthDef->MapID);
		return -1;
	}
	
	i=RAND1(pstBirthDef->PosNum);

	iCameraDir = pstBirthDef->PosList[i].CameraDir;
	iDir = pstBirthDef->PosList[i].direction;
	
	iMapPos	=	-1;
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &iMapPos, pstSelectBirth->MapID);

	pstPlayer->stRoleData.Map = pstSelectBirth->MapID;
	pstPlayer->stOnline.bDir = pstBirthDef->PosList[i].direction ;
	stBirthPos.X = pstBirthDef->PosList[i].X;
	stBirthPos.Y = pstBirthDef->PosList[i].Y;

	stIdx.iID = pstSelectBirth->MapID;
	stIdx.iPos = -1;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &stBirthPos, &stBirthPos, 600, 0, &pstPlayer->stRoleData.Pos);

	//角色背包
	pstRolePackage->SparPkg.MaxNum = INIT_SPAR_PKG;
	pstRolePackage->SparPkg.Num = 0;
	
	pstRolePackage->EtStashPkg.MaxNum = INIT_ET_STASH_PKG;
	pstRolePackage->EtSundPkg.StuffNum = 0;
	pstRolePackage->EtSundPkg.MaxNum = INIT_ET_SUND_PKG;
	pstRolePackage->TaskPackage.MaxNum = INIT_TASK_PACKAGE;
	pstRolePackage->SundriesPackage.MaxNum = INIT_SUNDRIES_PACKAGE;
	pstRolePackage->SundriesPackage.BaseNum = INIT_SUNDRIES_PACKAGE;
	pstRolePackage->StashPackage.MaxNum = INIT_STASH_PACKAGE;
	pstRolePackage->StashPackage.BaseNum = INIT_STASH_PACKAGE;
	pstRolePackage->StuffPackage.MaxNum = INIT_STUFF_PACKAGE;
	pstRolePackage->StuffPackage.BaseNum = INIT_STUFF_PACKAGE;
	pstRolePackage->PetPackage.MaxNum= NEW_PET_LIST_NUM;
	//pstRolePackage->ShoppingPackage.MaxNum =  MAX_SHOPPING_PACKAGE; // 商品背包200个格子
	pstRolePackage->ShoppingPackage.MaxNum =  INIT_SHOPPING_PKG_NUM; // 最大上限100,先放20个格子

	pstPlayer->stRoleData.MiscInfo.Flag = SHOW_FASHION_FLAG|SHOW_HEAD_FLAG|NEW_ROLE_FLAG|SHOW_BACK_FLAG|SHOW_FACE_FLAG|PLAYER_CLAN_SKILL_RESET|SHOW_WEDDING_DSN_FLAG|WEDDING_DSN_FLAG_ONCE;

	//  出生获取摆摊格子
	pstPlayer->stRoleData.MiscInfo.StallGridNum = STALL_MAX_ITEM_PAGE;

	pstPlayer->stRoleData.MiscInfo.NewRoleGiftInfo.NewRole = 1;
	pstPlayer->stRoleData.MiscInfo.NewRoleGiftInfo.ID = 0;

	// 出生10个精灵格子
	memset(&pstRolePackage->FairyPackage, 0, sizeof(pstRolePackage->FairyPackage));
	pstRolePackage->FairyPackage.FairyMaxNum = FAIRY_MAX_NUM;

	// 出生10个精灵格子
	pstRolePackage->QiYueInfo.QiYueMax = QIYUE_MAX_NUM;

	//出生获得任务
	pstTaskDef = task_def_find(pstEnv, BIRTH_PLAYER_TASK, NULL);
	if (pstTaskDef)
	{
		pstTaskRunOne = task_add_taskrun(pstEnv, pstPlayer, pstTaskDef);
		if (pstTaskRunOne)
		{
			pstTaskRunOne->TaskStat = TASK_STAT_FINI;
		}
	}

	// 新手在线奖励
	pstOnlineAwardDef = find_online_award_def(pstEnv, 0, ONLINE_AWARD_DEF_DATA);
	if (pstOnlineAwardDef)
	{
		pstPlayer->stRoleData.MiscInfo.OnlineAwardIdx = 0;
		pstPlayer->stRoleData.MiscInfo.OnlineAwardEnd = pstOnlineAwardDef->OnlineTime * 60;
	}

	// 出生BCt历史最高记录为0
	pstPlayer->stRoleData.MiscInfo.BCTHistoryTop = 0;

	// 每天在线时间
	pstPlayer->stRoleData.MiscInfo.ODTime.OndaySec = 0;
	pstPlayer->stRoleData.MiscInfo.ODTime.UpTime = pstAppCtx->stCurr.tv_sec;
	
	z_role_login(pstAppCtx, pstEnv, pstPlayer, iCameraDir, iDir);

	//z_rolenew_oplog(pstAppCtx, pstEnv, pstPlayer);

	//强制出生地提示
	if (pstSelectBirth->MapID != iSelMapID)
	{
		MapIndex *pstSelMapIdx;

		pstSelMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, iSelMapID);
		if(pstSelMapIdx)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PLAYER107, pstSelMapIdx->stMapDef.Name, pstMapIdx->stMapDef.Name);
		}
	}
	
	return 0;
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

int z_player_move(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)

{
	int i, iMapIdx0, iX0, iY0;
	MapIndex *pstMapIdx;
	ZONEMAPINST * pstMapInst;
	enum MaskBits enMask;
	TMAPMASK *pstMask;
	char cPassC;
	RESPOS  *pstLastPos;
	struct timeval stCltTime;
	struct timeval stConnectTime;
	long long llRet;
	ZONEIDX stIdx;
	ZONE_SVRCONF *pstConf = pstEnv->pstConf;
	CSMOVEC *pstMove = &pstCsPkg->Body.MoveReq;
	ROLETMPSYS *pstTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys; 

	pstPlayer->tActive = pstAppCtx->stCurr.tv_sec;

	if (pstMove->MoveSeq - pstPlayer->stOnline.iPosSeq < 0) {
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "stop player %d error move seq %d  %d\n", 
					pstPlayer->stRoleData.Uin, pstMove->MoveSeq, pstPlayer->stOnline.iPosSeq);
		return 0;
	}

 	if ((CS_STAT_SIT|CS_STAT_GAOJI_SIT) & pstPlayer->stOnline.State)
	{
		player_sit_break(pstAppCtx, pstEnv, pstPlayer);
	}

	stConnectTime.tv_sec = pstFrameHead->TimeStamp.TimeVal.sec;
	stConnectTime.tv_usec = pstFrameHead->TimeStamp.TimeVal.usec;

	stCltTime.tv_sec = pstMove->SvrTime.BenchTimeSec;
	stCltTime.tv_usec = pstMove->SvrTime.BenchTimeUSec;

	llRet = z_compare_time(&stCltTime, &stConnectTime);
	if (llRet > pstConf->BenchTimeMoveMax || llRet < pstConf->BenchTimeMoveMin)
	{
		z_adjust_time(&stCltTime, &stConnectTime, pstPlayer);

		tlog_info(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
			"stop %d player z_compare_time clt and connect time %lld, CheckCltTimeFlag=%d", 
			pstPlayer->stRoleData.Uin, llRet, pstPlayer->stOnline.cCheckCltTimeFlag);
		z_player_stop(pstAppCtx, pstEnv,  pstPlayer, 1, &stCltTime);
		return 0;
	}

	if (pstPlayer->stOnline.cCheckCltTimeFlag)
	{
		if (pstPlayer->stOnline.stLastCltTime.tv_sec !=0)
		{
			llRet =  z_compare_time(&pstPlayer->stOnline.stLastCltTime, &stCltTime);
			if (llRet < -200)
			{
				tlog_info(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
					"stop %d player z_compare_time lastclttime and clttime fail, ret=%lld, CheckCltTimeFlag=%d", 
					pstPlayer->stRoleData.Uin, llRet, pstPlayer->stOnline.cCheckCltTimeFlag);
				z_player_stop(pstAppCtx, pstEnv,  pstPlayer, 1, &stCltTime);
				return 0;
			}
			else if (llRet > 0)
			{
				TV_CLONE(pstPlayer->stOnline.stLastCltTime, stCltTime);
			}
		}
		else
		{
			TV_CLONE(pstPlayer->stOnline.stLastCltTime, stCltTime);
		}

		if (pstPlayer->stOnline.stLastCltMoveTime.tv_sec !=0)
		{
			llRet = z_compare_time(&pstPlayer->stOnline.stLastCltMoveTime, &stCltTime);
			if (llRet < 0)
			{
				tlog_info(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
					"stop %d player z_compare_time stLastCltMoveTime and clttime fail, ret=%lld, CheckCltTimeFlag=%d", 
					pstPlayer->stRoleData.Uin, llRet, pstPlayer->stOnline.cCheckCltTimeFlag);
				z_player_stop(pstAppCtx, pstEnv,  pstPlayer, 1, &stCltTime);
				return 0;
			}
		}

		TV_CLONE(pstPlayer->stOnline.stLastCltMoveTime, stCltTime);
	}
	else
		return 0;

	/* 发包频率由zone_conn检测，客户端自行控制
	if (pstMove->MoveType != CS_MOVE_TURN)
	{
		llRet = z_compare_time(&pstPlayer->stOnline.stLastCltMoveTime, &stCltTime);
		if (pstPlayer->stOnline.stLastCltMoveTime.tv_sec != 0 && llRet < pstConf->BenchTimeMoveInterval)
		{
			tlog_info(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "stop %d player move interval %lld", pstPlayer->stRoleData.Uin, llRet);
			z_player_stop(pstAppCtx, pstEnv,  pstPlayer, 1, &stCltTime);
			return 0;
		}

		TV_CLONE(pstPlayer->stOnline.stLastCltMoveTime, stCltTime);
	}*/
	
	if (!(pstPlayer->stOnline.State & CS_STAT_HITCH_RIDE))
	{
		z_adjust_time(&stCltTime, &stConnectTime, pstPlayer);
	}

	if (pstMove->MoveType == CS_MOVE_WALK)
	{
		if (0 == pstMove->Data.Walk.Total)
		{
			return 0;
		}
		
		if (0 == z_player_act_status(pstAppCtx, pstEnv, pstPlayer, RES_ACTION_WALK))
		{
			if (!(pstPlayer->stOnline.State & CS_STAT_DEAD))
			{
				z_player_stop(pstAppCtx, pstEnv,  pstPlayer, 1, &stCltTime);
			}
			return -1;
		}
	}

	iMapIdx0 = pstPlayer->stRoleData.Map;

	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr_bytime(pstAppCtx, pstEnv, pstPlayer, 1, &stCltTime, 20);
	}

	if (iMapIdx0 != pstPlayer->stRoleData.Map) 
	{
		return 0;
	}
	
	if (pstPlayer->stOnline.cLastPathPoints)
	{
		cPassC = z_point_on_recordpath(pstMove->x, pstMove->y, pstPlayer->stOnline.cLastPath, pstPlayer->stOnline.cLastPathPoints, pstPlayer->stOnline.stTurnPoints, 
					pstPlayer->stOnline.cFuturePoints);
		if (!cPassC)
		{
			tlog_info(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "stop %d player move point not in old path", pstPlayer->stRoleData.Uin);
			return z_player_stop(pstAppCtx, pstEnv,  pstPlayer, 1, NULL);
		}

		pstLastPos = &pstPlayer->stOnline.stTurnPoints[(pstPlayer->stOnline.cLastPath+ pstPlayer->stOnline.cLastPathPoints)%RECORD_TURN];
		if (pstMove->x == pstLastPos->X && pstMove->y == pstLastPos->Y)
		{
			TV_CLONE(pstPlayer->stOnline.stLastMoveTv, stCltTime);
		}
	}
	else
	{
		TV_CLONE(pstPlayer->stOnline.stLastMoveTv, stCltTime);
		
		if (pstMove->x != pstPlayer->stRoleData.Pos.X || pstMove->y != pstPlayer->stRoleData.Pos.Y)
		{
			if (0 > z_trust_clt_pos(pstAppCtx, pstEnv, pstPlayer, pstMove->x, pstMove->y, &stCltTime, 0))
			{
				tlog_info(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "stop %d player map=%d move point (%d,%d) not in old path(%d,%d)", 
						pstPlayer->stRoleData.Uin, pstPlayer->stRoleData.Map, pstMove->x, pstMove->y, pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y);
			
				return z_player_stop(pstAppCtx, pstEnv, pstPlayer, 1, &stCltTime);
			}
		}
		cPassC = 0;
	}
	
	pstPlayer->stOnline.iPosSeq = pstMove->MoveSeq;
	
	if (pstMove->MoveType == CS_MOVE_TURN)
	{
		z_player_check_stop(pstAppCtx, pstEnv, pstPlayer, pstMove, &stCltTime);
	
		return 0;
	}

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst) return -1;
	pstMapIdx =  z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx) return -1;

	if (CS_STAT_AUTOFLY & pstPlayer->stOnline.State)
	{
		
	}
	else
	{
		//if (CS_STAT_FLY & pstPlayer->stOnline.State)
		//{
		//	enMask = maskMagic;
		//}
		//else 
		//{
			enMask = maskStop;
		//}
		
		iX0 = pstMove->x;
		iY0 = pstMove->y;

		pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);

		if( !(pstTmpSys->PatrolID > 0 && pstTmpSys->TmpItemID > 0) )
		{
			for (i = 0; i < pstMove->Data.Walk.Total; i++) 
			{
				if (!IsPixelLineThrough(pstMask, iX0, iY0, pstMove->Data.Walk.Points[i].x, pstMove->Data.Walk.Points[i].y, enMask))
				{
					tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "stop mask %d player map=%d PixelLineThrough fail (%d,%d)->(%d,%d)", 
								pstPlayer->stRoleData.Uin, pstPlayer->stRoleData.Map, iX0, iY0, pstMove->Data.Walk.Points[i].x, pstMove->Data.Walk.Points[i].y);
					return z_player_stop(pstAppCtx, pstEnv,  pstPlayer, 1, NULL);
				}

				if (!dyn_msk_check(pstEnv, pstMapInst, pstMask, iX0, iY0, 
						pstMove->Data.Walk.Points[i].x, pstMove->Data.Walk.Points[i].y, 0,DYN_MSK_STOP))
				{
					tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "stop dyn mask %d player map=%d PixelLineThrough fail (%d,%d)->(%d,%d)", 
								pstPlayer->stRoleData.Uin, pstPlayer->stRoleData.Map, iX0, iY0, pstMove->Data.Walk.Points[i].x, pstMove->Data.Walk.Points[i].y);
					return z_player_stop(pstAppCtx, pstEnv,  pstPlayer, 1, NULL);
				}
				
				iX0 = pstMove->Data.Walk.Points[i].x;
				iY0 = pstMove->Data.Walk.Points[i].y;
			}
		}
	}

	
	if (z_add_path(pstAppCtx, pstEnv, pstPlayer, pstMove, cPassC) < 0)
		return z_player_stop(pstAppCtx, pstEnv,  pstPlayer, 1, NULL);
	
	if (pstPlayer->stOnline.cFuturePoints && !(pstPlayer->stOnline.State & CS_STAT_HITCH_RIDE))
		z_player_move_notify(pstAppCtx, pstEnv,  pstPlayer, pstMove->MoveType);
	
	return 0;
}

int player_move(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	int i;
	int iTmpSeq;	
	Player *pstTmpPlayer;
	CSMOVEC *pstMove = &pstCsPkg->Body.MoveReq;

	player_sham_dead_clear(pstEnv, pstPlayer);
	
	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOCLT))
	{
		return -1;
	}

	if (pstPlayer->stOnline.State & CS_STAT_HITCH_RIDE)
	{
		return -1;
	}

	
	//乘坐机械
	if(is_player_ride_on_machine(pstPlayer))
	{
		Monster *pstMon = z_id_monster(pstAppCtx, pstEnv, pstPlayer->stOnline.stMachineInfo.iMachineMonID);

		//燃料不足也不可以移动
		if(pstMon && pstMon->iPatrolID > 0)
		{
			return -1;
		}
	}
	
	z_player_move(pstAppCtx, pstEnv, pstPlayer, pstFrameHead, pstCsPkg);
	
	if ((pstPlayer->stOnline.State & (CS_STAT_RIDE_ON | CS_STAT_FLY)) && 
		pstPlayer->stOnline.stRideOnInfo.iPassengerNum)
	{
		//复制路径
		for(i = 0; i< pstPlayer->stOnline.stRideOnInfo.iPassengerNum; i++)
		{
			pstTmpPlayer = z_id_player(pstAppCtx, pstEnv, pstPlayer->stOnline.stRideOnInfo.PassengerID[i]);
			if (pstTmpPlayer && pstTmpPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
			{
				iTmpSeq = pstTmpPlayer->stOnline.iPosSeq;
				pstMove->MoveSeq = pstTmpPlayer->stOnline.iPosSeq;
				
				z_player_move(pstAppCtx, pstEnv, pstTmpPlayer, pstFrameHead, pstCsPkg);

				pstTmpPlayer->stOnline.iPosSeq = iTmpSeq;
			}
		}
	}

	return 0;
}

/* 由数据表GMIPMacLimit控制*/
int z_check_gm_ip(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,Player *pstPlayer, char *pszMac)
{
	int i;
	//unsigned int iIp;

	if (RES_LOAD_BIN != pstEnv->pstConf->ResloadType)
	{
		return UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN;
	}

	if (0 == pstEnv->stGmLimt.iInitFlag)
	{
		return 0;
	}

	for (i=0; i<pstEnv->stGmLimt.iGMIP; i++)
	{
		/*iIp = (int)inet_addr(pstEnv->stGmLimt.astGmIPMacLimit[i].IP);
		if (iIp == pstPlayer->stConnectInfo.stExtraInfo.IPInfo.ip &&
			0 == strcasecmp(pszMac, pstEnv->stGmLimt.astGmIPMacLimit[i].Mac))*/
		if ( 0 == strcasecmp(pszMac, pstEnv->stGmLimt.astGmIPMacLimit[i].Mac) )	
		{
			return pstEnv->stGmLimt.astGmIPMacLimit[i].Perm;
		}
	}

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}

int z_check_chat(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, char* pszChat, char cType)
{
	int iRet;

	if ((CHAT_ITEM == cType || CHAT_SPAN== cType)&&
		(player_check_safelock(pstEnv,  pstPlayer) < 0))
	{
		return -1;
	}
	
	iRet	=	z_gm_cmd(pstAppCtx, pstEnv, pstPlayer, pszChat);
	if (0 == iRet)
	{
		// OPLOG : 记录每一条GM命令
		z_gm_cmd_oplog(pstEnv, pstPlayer, pszChat);
		return -1;
	}
	else if (2 == iRet)
	{
		return -1;
	}

	// gm 牛x
	if (	pstPlayer->stRoleData.Perm > 0 ||
		(pstPlayer->UinFlag & UIN_FLAG_GM_ALL) )
	{
		return 0;
	}

	if (CHAT_WORLD == cType)
	{
		if (pstPlayer->stOnline.tChatWorldCoolEnd > pstAppCtx->stCurr.tv_sec)
		{
			z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CHAT_WORLD, CHAT_WORLD_COOL/1000);
			return -1;
		}

		if (pstPlayer->stRoleData.Level < CHAT_WORLD_LEVEL)
		{
			z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CHAT_LEVEL, CHAT_WORLD_LEVEL);
			return -1;
		}
	}
	else
	{
		long long  llMs;
		struct timeval stDiff;
		int iSec = CHAT_COMM_COOL/1000;
		if (CHAT_COMM_COOL%1000 > 0)
			iSec ++;
		
		TV_DIFF(stDiff, pstAppCtx->stCurr,pstPlayer->stOnline.stChatCommCool);
		//避免溢出加个判断 
		if( stDiff.tv_sec > iSec)
		{
			stDiff.tv_sec = iSec;
		}
		else if( stDiff.tv_sec < -iSec)
		{
			stDiff.tv_sec = -iSec;
		}
		TV_TO_MS(llMs, stDiff);
		
		if (llMs + CLT_TIME_WAVE*10 < CHAT_COMM_COOL)
		{
			z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CHAT_COMM);
			return -1;
		}
	}

	if ( DirtyReplace( pszChat ) > 0 )
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_DIRTY);
	}

	return 0;
}

int player_silence_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iRet = 0;
	const int kCurr = pstEnv->pstAppCtx->stCurr.tv_sec;

	if(pstPlayer->iAccSilenceEnd < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_PLAYER108);
		goto keep_silence;
	}

	if(pstPlayer->iAccSilenceEnd > kCurr)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, 
								ZONEERR_PLAYER109, pstPlayer->iAccSilenceEnd - kCurr);
		goto keep_silence;
	}

	if(pstPlayer->stRoleData.SilenceEnd < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_PLAYER110);
		goto keep_silence;
	}
	
	if(pstPlayer->stRoleData.SilenceEnd > kCurr)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, 
							ZONEERR_PLAYER111, pstPlayer->stRoleData.SilenceEnd - kCurr);
		goto keep_silence;
	}

final:
	return iRet;

keep_silence:
	iRet = -1;
	goto final;
	return 0;
}

int z_sendpkg_playerview_filtered(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, 
	                                Player *pstPlayer, int iSelf, CSPKG *pstPkg,
	                                int iImportant)
{
	Player *pstVPlayer;
	int i, iIndex;
	char cType;
	TMEMBLOCK *pstMemBlock;
	TDRDATA stNet;
	int iRet;
	int iCount = 0;
	Player *apstPlayers[MAX_PLAYER];

	if (iSelf) z_cltmsg_send(pstEnv, pstPlayer, pstPkg);

	//坐在机械上，需要用机械的视野
	if(is_player_ride_on_machine(pstPlayer))
	{
		Monster *pstMachine = machine_get_by_player(pstEnv, pstPlayer);
		if(pstMachine)
		{
			z_sendpkg_monview_except(pstAppCtx, pstEnv, pstMachine, pstPkg, 0, pstPlayer);
		}

		return 0;
	}

	if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE)
	{
		ZONEIDX stIdx;
		ZONEMAPINST *pstMapInst;

		stIdx.iID = pstPlayer->stRoleData.Map;
		stIdx.iPos = pstPlayer->stOnline.iMapPos;
		pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
		if (pstMapInst && pstMapInst->bBroadcast && pstPkg->Head.Cmd != CHAT_RES)
		{
			ZONEPWORLDINST *pstPworldInst;
			pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj,
				                                 pstMapInst->iPworldInstID);
			if (pstPworldInst)
			{
				return pworld_map_broadcast_except(pstAppCtx, pstEnv, pstPworldInst,
					                                         pstMapInst, iImportant, pstPkg, pstPlayer);
			}
		}
	}

	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = &pstEnv->szSendBuffer[0];
	iRet = net_cs_pack(pstEnv->pstNetHan, &stNet, pstPkg, 0);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
			         "z_sendpkg_playerview cmd=%d cs pack fail ret=%d",
			         pstPkg->Head.Cmd, iRet);
		return -1;
	}

	for (i = 0; i < pstPlayer->stOnline.iVAnimate; i++) {
		iIndex = z_id_indextype(pstAppCtx, pstEnv, 
			                      pstPlayer->stOnline.aiVAnimate[i], &cType);
		if (iIndex < 0) continue;

		switch(cType)
		{
			case OBJ_PLAYER:
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					continue;
				}
				pstVPlayer = (Player *)pstMemBlock->szData;
				
				if (PLAYER_STATUS_ROLE_LOGIN != pstVPlayer->eStatus)	  
				{
					continue;
				}

				// 在黑名单中的不给发
				if(0 <= player_black_find_byid(&pstVPlayer->stRoleData.MiscInfo, 
					                             pstPlayer->stRoleData.RoleID))
				{
					continue;
				}

				if((pstPlayer->stOnline.State & CS_STAT_QIANXIN) && 
					 (0 == player_team_same(pstEnv, pstPlayer, pstVPlayer)))
				{
					continue;
				}

				if (!iImportant && pstPlayer->stOnline.iVAnimate > VISIBLE_ANIMATE_VALVE &&
					  !z_pos_in_view_half(&pstPlayer->stRoleData.Pos,
					                      &pstVPlayer->stRoleData.Pos))
				{
					continue;
				}
				
				apstPlayers[iCount++] = pstVPlayer;
				break;
			case OBJ_MONSTER:
				{
					Monster *pstVMon;
					
					pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
					if (!pstMemBlock->fValid)
					{
						continue;
					}
					pstVMon = (Monster *)pstMemBlock->szData;

					if (is_mon_machine(pstVMon))
					{
						machine_msg_broadcast_except(pstEnv, pstVMon, pstPkg, pstPlayer->iMemID);
					}
				}
				break;
			default:
				break;
		}
		
	}

	if (iCount > 0)
	{
		return z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
		                                iCount, pstPkg);
	}
	return 0;
}

int player_chat_to_mud(ZONESVRENV* pstEnv, Player *pstPlayer, CSCHATREQ *pstChat, const char* pstRoleName)
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
		if (pstPlayer)
		{
			pstSSChat->ToData.ClanInfo = pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId;
		}
	}
	else if(pstChat->Type == CHAT_PRIVATE)
	{
		pstSSChat->ChatType = SS_CHAT_PRIVATE;
		STRNCPY(pstSSChat->ToData.ToName, pstChat->ToName, 
			CCH(pstSSChat->ToData.ToName));
	}
	else
	{
		return -1;
	}

	pstSSChat->LinkLen = pstChat->LinkLen;
	if (pstPlayer)
	{
		STRNCPY(pstSSChat->From, pstPlayer->stRoleData.RoleName,
	        			sizeof(pstSSChat->From));
	}
	else
	{
		STRNCPY(pstSSChat->From, pstRoleName,sizeof(pstSSChat->From));
	}
	
	STRNCPY(pstSSChat->Content, pstChat->Content, sizeof(pstSSChat->Content));
	if (pstSSChat->LinkLen > 0)
	{
		memcpy(pstSSChat->LinkContent, pstChat->LinkContent, pstChat->LinkLen);
	}

	Z_SSHEAD_INIT(&stPkg.Head, SS_CHAT);
	z_ssmsg_send(pstEnv, &stPkg, pstEnv->iMudSvrID);

	return 0;
}

int player_chat_to_all_svr(ZONESVRENV* pstEnv, Player *pstPlayer, CSCHATREQ *pstChat)
{
	SSPKG stPkg;
	SSCHAT *pstSSChat = &stPkg.Body.ChatInfo;

	pstSSChat->ChatType = SS_CHAT_SPAN;
	pstSSChat->LinkLen = pstChat->LinkLen;
	STRNCPY(pstSSChat->From, pstPlayer->stRoleData.RoleName,
	        		sizeof(pstSSChat->From));
	STRNCPY(pstSSChat->Content, pstChat->Content, sizeof(pstSSChat->Content));
	if (pstSSChat->LinkLen > 0)
	{
		memcpy(pstSSChat->LinkContent, pstChat->LinkContent, pstChat->LinkLen);
	}
	pstSSChat->WorldID = pstPlayer->stRoleData.WorldID;
	Z_SSHEAD_INIT(&stPkg.Head, SS_CHAT);
	z_ssmsg_send(pstEnv, &stPkg, pstEnv->iRegionGateID);

	return 0;
}

static int player_chat_proxy_request(ZONESVRENV* pstEnv, Player *pstPlayer,
                                     CSPKG *pstCsPkg, int *pfDispatched)
{
	CSCHATREQ *pstChat = &pstCsPkg->Body.ChatReq;

	*pfDispatched = 1;
	switch(pstChat->Type)
	{
	case CHAT_PRIVATE:
		//if(strstr(pstChat->ToName, BATTLE_NAME_SYMBOL) == NULL)
		//{
			battle_rolename_remove_prefix(pstChat->ToName);
			return battle_proxy_request(pstEnv, pstPlayer, pstCsPkg);
		//}
		//else
		//{
		//	*pfDispatched = 0;
		//}
		break;

	case CHAT_SAY:
		*pfDispatched = 0;
		break;

	case CHAT_FRIEND:
		battle_rolename_remove_prefix(pstChat->ToName);
		return battle_proxy_request(pstEnv, pstPlayer, pstCsPkg);
		break;
	case CHAT_WINDOW:
		battle_rolename_remove_prefix(pstChat->ToName);
		return battle_proxy_request(pstEnv, pstPlayer, pstCsPkg);
		break;
	case CHAT_TEAM:
		battle_proxy_request(pstEnv, pstPlayer, pstCsPkg);
		break;
	case CHAT_CLAN:
		battle_proxy_request(pstEnv, pstPlayer, pstCsPkg);
		break;
	case CHAT_CLAN_GROUP:
		battle_proxy_request(pstEnv, pstPlayer, pstCsPkg);
		break;
	case CHAT_WORLD:
		battle_proxy_request(pstEnv, pstPlayer, pstCsPkg);
		break;
	case CHAT_ITEM:
		{
			ITEMDEF *pstItemDef = NULL;
			ROLEITEM *pstRoleItem = NULL;
			RESULT *pstResult = NULL;
			int iPos = -1;
			
			if(package_get_item(pstEnv, pstPlayer, pstChat->ListType,
			                    pstChat->GridIdx, &pstItemDef, &pstRoleItem,
			                    &iPos) < 0)
			{
				return -1;
			}
			if(!pstItemDef)
			{
				return -1;
			}
			pstResult = z_item_get_result(pstItemDef, RESULT_ITEM_CHAT);
			if(!pstResult)
			{
				return -1;
			}

			if(package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,
			               pstItemDef->ItemID, 1,  pstChat->ListType, &iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_CHAT) < 0)
			{
				return -1;
			}
			
			//完成使用物品任务
			if(pstPlayer->stOnline.cMiscFlag & MISC_FLAG_USE_ITEM)
			{
				task_use_item_fini_trace(pstEnv, pstPlayer, pstItemDef);
			}
			
			return battle_proxy_request(pstEnv, pstPlayer, pstCsPkg);
		}
		break;
	case CHAT_SPAN:
		{
			ITEMDEF *pstItemDef = NULL;
			ROLEITEM *pstRoleItem = NULL;
			RESULT *pstResult = NULL;
			int iPos = -1;
			
			if(package_get_item(pstEnv, pstPlayer, pstChat->ListType,
			                    pstChat->GridIdx, &pstItemDef, &pstRoleItem,
			                    &iPos) < 0)
			{
				return -1;
			}
			if(!pstItemDef)
			{
				return -1;
			}
			pstResult = z_item_get_result(pstItemDef, RESULT_SPAN_CHAT);
			if(!pstResult)
			{
				return -1;
			}

			if(package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,
			               pstItemDef->ItemID, 1,  pstChat->ListType, &iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_CHAT) < 0)
			{
				return -1;
			}
			
			return battle_proxy_request(pstEnv, pstPlayer, pstCsPkg);
		}
		break;
	case CHAT_BATTLE:
		*pfDispatched = 0;
		break;
	case CHAT_SPAN_MAP:
		*pfDispatched = 0;
		break;
	case CHAT_SPAN_MAP_WORLD:
		*pfDispatched = 0;
		break;
	default:
		return -1;
		break;
	}

	return 0;
}


int player_chat(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	ZoneTeam *pstTeam;
	CSPKG *pstResPkg = &pstEnv->stCSRes;
	CSCHATRES *pstChatRes = &pstResPkg->Body.ChatRes;
	CSCHATREQ *pstChat = &pstCsPkg->Body.ChatReq;
	Player *pstOther = NULL;
	ZoneClan *pstClan = NULL;
	CSPKG stEcho;
	int iVip = 0;
	
	if(player_silence_check(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}

	if (z_check_chat(pstAppCtx, pstEnv, pstPlayer, pstChat->Content, pstChat->Type) < 0)
	{
		return -1;
	}

	iVip |= z_player_check_vip(pstEnv,pstPlayer);
	
	//xxoo后的回音
	stEcho.Body.ChatReq = *pstChat;
	stEcho.Body.ChatReq.Vip = iVip;
	stEcho.Body.ChatReq.IsCluster = 0;
	Z_CSHEAD_INIT(&stEcho.Head, CHAT_REQ);
	
	if(pstPlayer->stOnline.stSpanOnline.iStat == SPAN_STAT_CONNECT)
	{
		if(pstChat->Type == CHAT_WORLD  &&
			pstEnv->iInstMode == INST_MODE_ZONE_SVR)
		{
			z_cltmsg_send(pstEnv, pstPlayer, &stEcho);
		}
	}
	else
	{
		if(pstChat->Type == CHAT_WORLD &&
			pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
		{

		}
		else if(pstChat->Type != CHAT_PRIVATE &&
			pstChat->Type != CHAT_SPAN)
		{
			z_cltmsg_send(pstEnv, pstPlayer, &stEcho);
		}
	}
	
	// 如果是battle_svr,转发给zone_svr处理
	if(pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		int fDispatched = 0;
		int iRet = player_chat_proxy_request(pstEnv, pstPlayer, pstCsPkg,
		                                     &fDispatched);
		if(iRet < 0)
		{
			return -1;
		}
		if(fDispatched)
		{
			return iRet;
		}
	}

	pstChatRes->Type = pstChat->Type;
	pstChatRes->LinkLen = pstChat->LinkLen;
	STRNCPY(pstChatRes->From, pstPlayer->stRoleData.RoleName, sizeof(pstChatRes->From));
	STRNCPY(pstChatRes->Content, pstChat->Content, sizeof(pstChatRes->Content));
	pstChatRes->OnlineType = ROLE_GAME_ONLINE;
	pstChatRes->IsCluster = 0;
	pstChatRes->WorldID = pstPlayer->stRoleData.WorldID;
	if (pstChatRes->LinkLen > 0)
	{
		memcpy(pstChatRes->LinkContent, pstChat->LinkContent, pstChat->LinkLen);
	}

	pstChatRes->Vip = iVip;
	Z_CSHEAD_INIT(&pstResPkg->Head, CHAT_RES);

	if (ROLE_MUD_ONLINE== pstChat->OnlineType)
	{
		player_chat_to_mud(pstEnv, pstPlayer, pstChat, NULL);
		return 0;
	}
	switch(pstChat->Type)
	{
	case CHAT_SAY:
		z_sendpkg_playerview_filtered(pstAppCtx, pstEnv, pstPlayer, 0,
		                              pstResPkg, 0);
		break;
		
	case CHAT_TEAM:
		pstTeam = player_team_get(pstEnv, pstPlayer);
		if (NULL == pstTeam)
		{
			return -1;
		}
		if(pstTeam)
		{
			player_team_broadcast_except(pstEnv, pstTeam, pstResPkg, pstPlayer);
		}
		break;
		
	case CHAT_PRIVATE:
		if(NULL == pstChat->ToName)
		{
			return -1;
		}
		pstOther = z_name_player(pstAppCtx, pstEnv, pstChat->ToName);
		if (NULL == pstOther)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PLAYER112, pstChat->ToName);
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_CURR, ZONEERR_PLAYER112, pstChat->ToName);
			return -1;
		}

		// gm 关闭耳朵
		if (pstOther->stRoleData.MiscInfo.Flag & EAR_SWITCH)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PLAYER113);
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_CURR, ZONEERR_PLAYER113);
			return -1;
		}

		if(0 <= player_black_find_byid(&pstOther->stRoleData.MiscInfo,
			                             pstPlayer->stRoleData.RoleID))
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PLAYER114, pstChat->ToName);
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_CURR, ZONEERR_PLAYER114, pstChat->ToName);
			return -1;
		}
		z_cltmsg_send(pstEnv, pstPlayer, &stEcho);
		z_cltmsg_send(pstEnv, pstOther, pstResPkg);
		
		break;

	case CHAT_FRIEND:
		if(NULL == pstChat->ToName)
		{
			return -1;
		}
		pstOther = z_name_player(pstAppCtx, pstEnv, pstChat->ToName);
		if (NULL == pstOther)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PLAYER115, pstChat->ToName);
			
			return -1;
		}
		if (0 <= player_black_find_byid(&pstPlayer->stRoleData.MiscInfo, pstOther->stRoleData.RoleID))
		{
			STRNCPY(pstChatRes->From, pstChat->ToName, sizeof(pstChatRes->From));
			STRNCPY(pstChatRes->Content, "不能和黑名单中玩家好友聊天", sizeof(pstChatRes->Content));
			z_cltmsg_send(pstEnv, pstPlayer, pstResPkg);
			return -1;
		}
		
		if (0 <= player_black_find_byid(&pstOther->stRoleData.MiscInfo, pstPlayer->stRoleData.RoleID))
		{
			STRNCPY(pstChatRes->From, pstChat->ToName, sizeof(pstChatRes->From));
			STRNCPY(pstChatRes->Content, "你在对方黑名单中", sizeof(pstChatRes->Content));
			z_cltmsg_send(pstEnv, pstPlayer, pstResPkg);
			return -1;
		}
		z_cltmsg_send(pstEnv, pstOther, pstResPkg);
		break;
	case CHAT_WINDOW:
		if(NULL == pstChat->ToName)
		{
			return -1;
		}
		
		pstOther = z_name_player(pstAppCtx, pstEnv, pstChat->ToName);
		if (NULL == pstOther)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PLAYER115, pstChat->ToName);
			
			return -1;
		}
		
		z_cltmsg_send(pstEnv, pstOther, pstResPkg);
		break;
	case CHAT_CLAN:
		pstClan = player_get_clan(pstEnv, pstPlayer);
		if (pstClan)
		{
			clan_chat_transmit(pstEnv,pstClan,&pstResPkg->Body.ChatRes);
			player_clan_broadcast_except(pstEnv, pstClan, pstResPkg, 0, pstPlayer);
		}
		break;
	case CHAT_CLAN_GROUP:
		pstClan = player_get_clan(pstEnv, pstPlayer);
		if (pstClan)
		{
			player_clan_group_broadcast_except(pstEnv, pstClan, pstResPkg, pstPlayer);
		}
		break;
		
	case CHAT_WORLD:
		z_cltmsg_broadcast(pstAppCtx, pstEnv, pstResPkg);
		break;
	case CHAT_ITEM:
		{
			ITEMDEF *pstItemDef = NULL;
			ROLEITEM *pstRoleItem = NULL;
			RESULT *pstResult = NULL;
			unsigned char ucEntity = 0;
			int iPos = -1;

			GET_FUNC_ENTITY(ucEntity, pstEnv->iLastSrc);
			if(ucEntity == FUNC_BATTLE_SVR) // 检查消息来源
			{
				// 消息来自battle_svr,道具已经消耗过了,此时只要直接广播消息
				// 就可以了
				z_cltmsg_broadcast(pstAppCtx, pstEnv, pstResPkg);
				break;
			}

			if (0 > package_get_item(pstEnv, pstPlayer, pstChat->ListType, 
								pstChat->GridIdx, &pstItemDef, &pstRoleItem, &iPos))
			{
				return -1;
			}
			if( !pstItemDef )
			{
				return -1;
			}
			pstResult = z_item_get_result(pstItemDef, RESULT_ITEM_CHAT);
			if(pstResult == NULL)
			{
				return -1;
			}

			if (0 > package_dec(pstAppCtx, pstEnv, pstPlayer, pstItemDef->ItemID, 1,  
				pstChat->ListType, &iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_CHAT))
			{
				return -1;
			}

			//完成使用物品任务
			if(pstPlayer->stOnline.cMiscFlag & MISC_FLAG_USE_ITEM)
			{
				task_use_item_fini_trace(pstEnv, pstPlayer, pstItemDef);
			}
	
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PLAYER153, pstItemDef->Name);
			
			z_cltmsg_broadcast(pstAppCtx, pstEnv, pstResPkg);
			break;
		}
	case CHAT_BATTLE:
		normal_battle_camp_broadcast_except(pstEnv, pstPlayer, pstResPkg, pstPlayer);
		break;
	case CHAT_SPAN:
		{
			ITEMDEF *pstItemDef = NULL;
			ROLEITEM *pstRoleItem = NULL;
			RESULT *pstResult = NULL;
			unsigned char ucEntity = 0;
			int iPos = -1;

			GET_FUNC_ENTITY(ucEntity, pstEnv->iLastSrc);
			if(ucEntity == FUNC_BATTLE_SVR) // 检查消息来源
			{
				// 消息来自battle_svr,道具已经消耗过了,此时只要直接广播消息
				// 就可以了
				player_chat_to_all_svr(pstEnv, pstPlayer, pstChat);
				z_cltmsg_broadcast(pstAppCtx, pstEnv, pstResPkg);
				break;
			}

			if (0 > package_get_item(pstEnv, pstPlayer, pstChat->ListType, 
								pstChat->GridIdx, &pstItemDef, &pstRoleItem, &iPos))
			{
				return -1;
			}
			if( !pstItemDef )
			{
				return -1;
			}
			
			pstResult = z_item_get_result(pstItemDef, RESULT_SPAN_CHAT);
			if(pstResult == NULL)
			{
				return -1;
			}

			if (0 > package_dec(pstAppCtx, pstEnv, pstPlayer, pstItemDef->ItemID, 1,  
				pstChat->ListType, &iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_CHAT))
			{
				return -1;
			}
	
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PLAYER153, pstItemDef->Name);
			
			player_chat_to_all_svr(pstEnv, pstPlayer, pstChat);
			z_cltmsg_broadcast(pstAppCtx, pstEnv, pstResPkg);
			break;
		}
	case CHAT_SPAN_MAP:
		span_map_broadcast(pstEnv, pstPlayer,pstResPkg,0);
		break;
	case CHAT_SPAN_MAP_WORLD:
		span_map_broadcast(pstEnv, pstPlayer,pstResPkg,1);
		break;
	default:
		return -1;
		break;
	}

	TV_ADD_MS(pstPlayer->stOnline.stChatCommCool, pstAppCtx->stCurr, CHAT_COMM_COOL);
	if (CHAT_WORLD == pstChat->Type)
	{
		pstPlayer->stOnline.tChatWorldCoolEnd = pstAppCtx->stCurr.tv_sec + CHAT_WORLD_COOL/1000;
	}
	
	z_role_chat_oplog( pstEnv, pstPlayer, pstChat );
	return 0;
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

int player_opt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	memcpy(&pstPlayer->stRoleData.ClientOpt, &pstCsPkg->Body.ClientOpt, sizeof(pstPlayer->stRoleData.ClientOpt));
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstFrameHead);	//add by paraunused.pl
}


// 检查宠物强化 ，老账号没有NextStrenLvl字段
int player_check_pet_stren(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	PETPACKAGE *pstPetPkg = &pstPlayer->stRoleData.Package.PetPackage;
	for(i=0; i<pstPetPkg->Num; i++)
	{
		if (	pstPetPkg->RolePets[i].NextStrenLvl == 0 && 
			pstPetPkg->RolePets[i].StrengthenLevel < MAX_PET_STRENGTHEN_LEVEL)
		{
			pstPetPkg->RolePets[i].NextStrenLvl  = pstPetPkg->RolePets[i].StrengthenLevel+1;
		}
	}
	return 0;
	UNUSED(pstEnv);
}

// 这几个东西可能经常会变     在这里预防一下
// 艹他妹妹 老他妈的改来改去
static int player_pkg_grid_check(Player *pstPlayer)
{
	if (pstPlayer->stRoleData.Package.EtSundPkg.MaxNum < INIT_ET_SUND_PKG)
		pstPlayer->stRoleData.Package.EtSundPkg.MaxNum = INIT_ET_SUND_PKG;

	if (pstPlayer->stRoleData.MiscInfo.StallGridNum != STALL_MAX_ITEM_PAGE)
		pstPlayer->stRoleData.MiscInfo.StallGridNum = STALL_MAX_ITEM_PAGE;

	if (pstPlayer->stRoleData.Package.StashPackage.MaxNum < INIT_STASH_PACKAGE)
		pstPlayer->stRoleData.Package.StashPackage.MaxNum = INIT_STASH_PACKAGE;

	int i;
	SUNDRIESPACKAGE *pstPkg = &pstPlayer->stRoleData.Package.SundriesPackage;
	ROLEGRID *pstGrid;

	for (i=0; i<pstPkg->Num; i++)
	{
		pstGrid = &pstPkg->RoleGrids[i];
		if (pstGrid->Type == GRID_ITEM_ITEM)
		{
			continue;
		}

		pstGrid->GridData.RoleArm.ArmSkill.Num = pstGrid->GridData.RoleArm.ArmSkill.Max;
	}
	
	return 0;
}
int player_ltrInfo(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkgRes;
	CSROLELTRINFO *pstCSRoleLtrInfo;
	pstCSRoleLtrInfo = &stPkgRes.Body.LtrInfoSvr;
	pstCSRoleLtrInfo->RoleLtrData = pstPlayer->stRoleData.MiscInfo.LtrInfo;
	Z_CSHEAD_INIT(&stPkgRes.Head, LTRINFO_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);
	return 0;
}

int player_pkg_check_spar(Player *pstPlayer)
{
	/*SPARPKG *pstSparPkg = &pstPlayer->stRoleData.Package.SparPkg;

	if (pstSparPkg->MaxNum < INIT_SPAR_PKG)
	{
		pstSparPkg->MaxNum = INIT_SPAR_PKG;
	}*/
	return 0;
	UNUSED(pstPlayer);	//add by paraunused.pl
}

int player_package_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
		
	pstPackageInfo->Type = PACKAGE_INFO_ALL;

	fairy_evolve_update_time(pstEnv, pstPlayer);
	player_pkg_grid_check(pstPlayer);
	player_pkg_check_spar(pstPlayer);
	player_check_pet_stren(pstEnv, pstPlayer);
	pet_role_login_check(pstEnv, pstPlayer);
	clan_pet_player_login_check( pstEnv, pstPlayer, 0);
	
	memcpy(&pstPackageInfo->PackageInfoData.PackageAll, &pstPlayer->stRoleData.Package, sizeof(pstPackageInfo->PackageInfoData.PackageAll));
	Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
	if (0 <= z_cltmsg_send(pstEnv, pstPlayer, &stPkg))
	{
		pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_PACKAGE_ALL;

		pet_package_info(pstEnv, pstPlayer);

		// 摊位格子数
		stall_msg_to_clt(pstEnv, pstPlayer, SVR_STALL_GRID_NUM);

		fairy_up_online_by_login(pstEnv, pstPlayer);
		
		if (	pstPlayer->stRoleData.MiscInfo.CampFyID > 0 && 
			pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
		{
			ROLEFAIRY *pstRoleFairy = get_pack_fairy_by_id(pstPlayer,pstPlayer->stRoleData.MiscInfo.CampFyID);
			if (pstRoleFairy)
			{
				set_fairy_camp(pstEnv, pstPlayer, pstRoleFairy);
				player_add_xp(pstEnv, pstPlayer, pstPlayer->stRoleData.MiscInfo.CampFyXP, 0,1);
			}
			pstPlayer->stRoleData.MiscInfo.CampFyXP = 0;
			pstPlayer->stRoleData.MiscInfo.CampFyID = 0;
		}

		if (pstPlayer->stRoleData.MiscInfo.Flag & NEW_ROLE_FLAG)
		{
			z_new_player_gift(pstEnv->pstAppCtx, pstEnv, pstPlayer);
			pstPlayer->stRoleData.MiscInfo.Flag &= ~NEW_ROLE_FLAG;
		}

		// 天赋
		genius_list_notify(pstEnv, pstPlayer);

		if (pstPlayer->stRoleData.MiscInfo.LostTotemInfo.ItemWid > 0 && 
			pstPlayer->stRoleData.MiscInfo.LostTotemInfo.MonID > 0)
		{
			player_losttotem_active_notify(pstEnv, pstPlayer, 1,
					pstPlayer->stRoleData.MiscInfo.LostTotemInfo.ItemWid);
		}
	}
	
	return 0;
}

int is_status_consume( Player *pstPlayer)
{
	ZoneStatus *pstStutas;
	
	pstStutas = z_player_find_status(pstPlayer, STATUS_ID_JIZOU);
	if (pstStutas)
	{
		return 1;
	}

	pstStutas = z_player_find_status(pstPlayer, STATUS_ID_QIANXIN);
	if (pstStutas)
	{
		return 1;
	}
	return 0;
}

int z_pet_active(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;
	ROLEPET *pstRolePet;
	PETDEF *pstPetDef;

	for (i=0; i<pstPetPackage->Num; i++)
	{
		pstRolePet = &pstPetPackage->RolePets[i];

		pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
		if (!pstPetDef)
		{
			continue;
		}

		if (pstRolePet->StatFlag & PET_STAT_FIGHT)
		{
			pet_love_val_chg(pstEnv, pstPlayer, pstRolePet, -PET_FIGHT_DEC_LOVEVAL);

			z_pet_check_status(pstAppCtx, pstEnv, pstPlayer, pstRolePet);
		}
	}

	return 0;
}


/*int z_pet_change_age_by_figter(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{

	int i;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;
	ROLEPET *pstRolePet;
	int iOldAge;
	PETDEF *pstPetDef;

	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	CSPETCHGDATA *pstPetChgDataList;

	pstPetSvr->Num = 0;

	if (!(pstPlayer->stOnline.State & CS_STAT_FIGHT))
	{
		return -1;
	}
	
	for (i=0; i<pstPetPackage->ShiNum; i++)
	{
		pstRolePet = &pstPetPackage->RoleShiPets[i];

		if (!(pstRolePet->StatFlag & PET_STAT_FIGHT))
		{
			continue;
		}

		pstPetDef = pet_find_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
		if (!pstPetDef)
		{
			continue;
		}
		
		iOldAge = pstRolePet->Age;
		pstRolePet->Age += PET_AGE_ADD_SPD_FIGTER;

		if ((int)pstRolePet->Age >= pstPetDef->MaxAge*PET_AGE_UNIT)
		{
			pstRolePet->Age = pstPetDef->MaxAge*PET_AGE_UNIT;
			pet_set_stat(pstEnv, pstPlayer, pstRolePet, PET_STAT_DEAD);
		}

		if (iOldAge/PET_AGE_UNIT != (int)(pstRolePet->Age/PET_AGE_UNIT))
		{
			pstPetChgDataList = &pstPetSvr->PetChgDataList[pstPetSvr->Num];
			pstPetChgDataList->AttrNum = 0;
			pstPetChgDataList->Wid = pstRolePet->WID;
			pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].Type = PET_CHG_AGE;
			pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].AttrValue.ChgAge = pstRolePet->Age;
			pstPetChgDataList->AttrNum ++;
			
			pstPetSvr->Num ++;
		}
	}

	if (pstPetSvr->Num > 0)
	{
		Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	
	return 0;
}*/

/*
int z_pet_change_power(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;
	ROLEPET *pstRolePet;
	PETDEF *pstPetDef = NULL;
	PETLEVELDEF *pstPetLevelDef = NULL;
	PetPowerOnline *pstPoweronline = NULL;
	int iAddPower;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	CSPETCHGDATA *pstPetChgDataList;
	int iOldPower;
	
	memset(pstPetSvr, 0, sizeof(CSPETSVR));

	for ( i=0; i<pstPetPackage->ShiNum; i++)
	{
		pstRolePet = &pstPetPackage->RoleShiPets[i];
		if (pstRolePet->StatFlag & PET_STAT_DEAD)
		{
			continue;
		}

		if (pstRolePet->Power >= pstRolePet->MaxPower)
		{
			continue;
		}

		pstPetDef = pet_find_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
		if (!pstPetDef)
		{
			continue;
		}

		pstPetLevelDef =  pet_find_level_def(pstEnv, pstRolePet->Level, NULL);
		if (!pstPetLevelDef)
		{
			return -1;
		}

		pstPoweronline = pet_find_poweronline(pstPlayer, pstRolePet);
		if (!pstPoweronline)
		{
			continue;
		}

		iAddPower = pstPetLevelDef->HpRecover*(1+pstPoweronline->nPetPowerRestoreMul/100.0)+pstPoweronline->nPetPowerRestoreAdd;
		if (iAddPower == 0)
		{
			continue;
		}

		iOldPower = pstRolePet->Power;
		
		pstRolePet->Power += iAddPower;
		if (pstRolePet->Power > pstRolePet->MaxPower)
		{
			pstRolePet->Power = pstRolePet->MaxPower;
		}

		if ((unsigned int )iOldPower != pstRolePet->Power)
		{
			pstPetChgDataList = &pstPetSvr->PetChgDataList[pstPetSvr->Num];
			pstPetChgDataList->Wid = pstRolePet->WID;
			pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].Type = PET_CHG_POWER;
			pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].AttrValue.ChgPower.Power = pstRolePet->Power;
			pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].AttrValue.ChgPower.XieDaiPos= get_pet_shi_xiedaipos(pstEnv, pstPlayer, pstRolePet) ;
			pstPetChgDataList->AttrNum ++;
			
			pstPetSvr->Num ++;
		}
		
	}

	for ( i=0; i<pstPetPackage->XianNum; i++)
	{
		pstRolePet = &pstPetPackage->RoleXianPets[i];

		if (pstRolePet->Power >= pstRolePet->MaxPower)
		{
			continue;
		}

		pstPetDef = pet_find_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
		if (!pstPetDef)
		{
			continue;
		}

		pstPetLevelDef =  pet_find_level_def(pstEnv, pstRolePet->Level, NULL);
		if (!pstPetLevelDef)
		{
			return -1;
		}

		pstPoweronline = pet_find_poweronline(pstPlayer, pstRolePet);
		if (!pstPoweronline)
		{
			continue;
		}

		iAddPower = pstPetLevelDef->MpRecover *(1+pstPoweronline->nPetPowerRestoreMul/100.0)+pstPoweronline->nPetPowerRestoreAdd;
		if (iAddPower == 0)
		{
			continue;
		}

		iOldPower = pstRolePet->Power;
		
		pstRolePet->Power += iAddPower;
		if (pstRolePet->Power > pstRolePet->MaxPower)
		{
			pstRolePet->Power = pstRolePet->MaxPower;
		}

		if ((unsigned int )iOldPower != pstRolePet->Power)
		{
			pstPetChgDataList = &pstPetSvr->PetChgDataList[pstPetSvr->Num];
			pstPetChgDataList->Wid = pstRolePet->WID;
			pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].Type = PET_CHG_POWER;
			pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].AttrValue.ChgPower.Power= pstRolePet->Power;
			pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].AttrValue.ChgPower.XieDaiPos = -1; 
			pstPetChgDataList->AttrNum ++;
			
			pstPetSvr->Num ++;
		}
		
	}

	if (pstPetSvr->Num > MAX_PET_PACKAGE || pstPetSvr->Num <= 0)
	{
		return -1;
	}

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}	



int z_pet_add_skillcollectnum_fight(ZONESVRENV* pstEnv, Player *pstPlayer, int iRate)
{
	int i;
	SKILLDEF *pstSkillDef;
	PlayerChagAttrVal stChgVal;
	ZoneSkill *pstZoneSkill;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;
	
	stChgVal.AttrNum = 0;
	for (i=0; i<pstZoneSkillList->nSkillNum; i++)
	{
		pstZoneSkill = &pstZoneSkillList->astZoneSkills[i];

		if (0 == pstZoneSkill->ullPetWID)
		{
			continue;
		}

		pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill);
		if (NULL == pstSkillDef)
		{
			continue;
		}

		if (pstSkillDef->CollectNumAddNum == 0 || pstZoneSkill->stSkillData.CollectNum >= pstSkillDef->CollectNumMax)
		{
			continue;
		}	

		pstZoneSkill->stSkillData.CollectNum += iRate*pstSkillDef->CollectNumAddNum;
		if (pstZoneSkill->stSkillData.CollectNum > pstSkillDef->CollectNumMax)
		{
			pstZoneSkill->stSkillData.CollectNum = pstSkillDef->CollectNumMax;
		}

		if (stChgVal.AttrNum >= MAX_ATTR_VAL)
		{
			continue;
		}

		stChgVal.AttrList[stChgVal.AttrNum].AttrID = ATTR_ID_COLLECTNUM;
		stChgVal.AttrList[stChgVal.AttrNum].Value.CollectNum.SkillID = pstZoneSkill->stSkillData.SkillID;
		stChgVal.AttrList[stChgVal.AttrNum].Value.CollectNum.CollectNum = pstZoneSkill->stSkillData.CollectNum;
		stChgVal.AttrNum++;	
	}

	if (stChgVal.AttrNum > 0)
	{
		z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);
	}
	
	return 0;
}

int z_pet_add_skillcollectnum_nofight(ZONESVRENV* pstEnv, Player *pstPlayer, int iRate)
{
	SKILLDEF *pstSkillDef;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;
	ROLEPET *pstRolePet;
	int i,j;
	PETSKILLONE *pstSkill;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	CSPETCHGDATA *pstPetChgDataList;
	int iChange;
	
	memset(pstPetSvr, 0, sizeof(CSPETSVR));	
	for (i=0; i<pstPetPackage->XianNum; i++)
	{
		pstRolePet = &pstPetPackage->RoleXianPets[i];
		if (0 == (PET_STAT_FIGHT & pstRolePet->StatFlag))
		{
			continue;
		}

		iChange = 0;
		pstPetChgDataList = &pstPetSvr->PetChgDataList[pstPetSvr->Num];
		
		for (j=0; j<pstRolePet->PetSkills.SkillNum; j++)
		{
			pstSkill = &pstRolePet->PetSkills.Skills[j];
			pstSkillDef = z_find_skill(pstEnv, pstSkill->SkillID, pstSkill->SkillLevel, NULL);
			if (!pstSkillDef)
			{
				continue;
			}

			if (pstSkillDef->CollectNumAddNum == 0 || (int)pstSkill->SkillCollectNum >= pstSkillDef->CollectNumMax)
			{
				continue;
			}	

			pstSkill->SkillCollectNum += pstSkillDef->CollectNumAddNum*iRate;
			if ((int)pstSkill->SkillCollectNum > pstSkillDef->CollectNumMax)
			{
				pstSkill->SkillCollectNum = pstSkillDef->CollectNumMax;
			}
				
			iChange = 1;
		}

		if (iChange == 1)
		{
			pstPetChgDataList->Wid = pstRolePet->WID;
			pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].Type = PET_CHG_SKILL;
			memcpy(&pstPetChgDataList->AttrChgList[pstPetChgDataList->AttrNum].AttrValue.ChgPetSkills,&pstRolePet->PetSkills,sizeof(pstRolePet->PetSkills));
			pstPetChgDataList->AttrNum ++;
			
			pstPetSvr->Num ++;
		}
	}

	if (pstPetSvr->Num > 0)
	{
		Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	return 0;
}

int z_pet_add_skillcollectnum(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iTimeHour;
	int iRate;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	
	iTimeHour = pstAppCtx->stCurr.tv_sec/3600;
	if (pstMiscInfo->PetSkillCollectTime == iTimeHour || pstAppCtx->stCurr.tv_sec%3600 > 30)	
	{
		return 0;
	}
	
	iRate = iTimeHour - pstMiscInfo->PetSkillCollectTime;
	pstMiscInfo->PetSkillCollectTime = iTimeHour;

	z_pet_add_skillcollectnum_fight(pstEnv, pstPlayer, iRate);
	z_pet_add_skillcollectnum_nofight(pstEnv, pstPlayer, iRate);
	
	return 0;
}

int z_pet_add_skillcollect_of_fight(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, Player * pstPlayer)
{
	int i;
	SKILLDEF *pstSkillDef;
	PlayerChagAttrVal stChgVal;
	ZoneSkill *pstZoneSkill;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;
	
	if (!(pstPlayer->stOnline.State & CS_STAT_FIGHT))
	{
		return 0;
	}
	
	stChgVal.AttrNum = 0;
	for (i=0; i<pstZoneSkillList->nSkillNum; i++)
	{
		pstZoneSkill = &pstZoneSkillList->astZoneSkills[i];

		if (0 == pstZoneSkill->ullPetWID)
		{
			continue;
		}

		pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill);
		if (NULL == pstSkillDef || 0 == pstSkillDef->FightCollectNumAddNum)
		{
			continue;
		}

		if (pstZoneSkill->stSkillData.CollectNum >= pstSkillDef->CollectNumMax)
		{
			continue;
		}	

		pstZoneSkill->stSkillData.CollectNum += pstSkillDef->FightCollectNumAddNum;
		if (pstZoneSkill->stSkillData.CollectNum > pstSkillDef->CollectNumMax)
		{
			pstZoneSkill->stSkillData.CollectNum = pstSkillDef->CollectNumMax;
		}

		if (stChgVal.AttrNum >= MAX_ATTR_VAL)
		{
			continue;
		}

		stChgVal.AttrList[stChgVal.AttrNum].AttrID = ATTR_ID_COLLECTNUM;
		stChgVal.AttrList[stChgVal.AttrNum].Value.CollectNum.SkillID = pstZoneSkill->stSkillData.SkillID;
		stChgVal.AttrList[stChgVal.AttrNum].Value.CollectNum.CollectNum = pstZoneSkill->stSkillData.CollectNum;
		stChgVal.AttrNum++;	
	}

	if (stChgVal.AttrNum > 0)
	{
		z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);
	}
	
	return 0;
}
*/
/*
int z_player_add_vigor(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iTimeDay;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	int iOldVigor = pstPlayer->stRoleData.RoleDetail.VigorCurr;
	int iVigorMax = pstPlayer->stOnline.stDyn.stMidAttrList.unVigorMax;
	int iVigor;
	PlayerChagAttrVal stChgVal;
	AttrChgVal *pstVal;

	//stChgVal.AttrNum = 0;
	iTimeDay = pstEnv->pstAppCtx->stCurr.tv_sec/(3600*24);
	if (pstMiscInfo->VigorTime == iTimeDay || pstEnv->pstAppCtx->stCurr.tv_sec%(3600*24) > 30)	
	{
		return 0;
	}

	if (iOldVigor == iVigorMax)
	{
		return 0;
	}
	
	pstMiscInfo->VigorTime = iTimeDay;

	iVigor = (iOldVigor*90/100.0)+500;
	if (iVigor > iVigorMax)
	{
		iVigor = iVigorMax;
	}

	if (iVigor - iOldVigor > 0)
	{
		pstVal = &stChgVal.AttrList[0];
		pstVal->AttrID = ATTR_ID_VIGOR;
		pstVal->nFlag = 0;
		pstVal->Value.Vigor = iVigor - iOldVigor;
		stChgVal.AttrNum = 1;

		z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);
	}

	return 0;
}
*/

int z_player_addexp_test(ZONESVRENV* pstEnv,Player *pstPlayer, tdr_longlong *piExp)
{
	long long llTmpExp1 = 0;
	long long llTmpExp2 = 0;
	long long llLimitExp = 0;
	ROLEMISCINFO *pstRoleMisc = &pstPlayer->stRoleData.MiscInfo;
	LEVELUP *pstLevelUp;
	int iFlag = 0;

	if(NULL == piExp)
	{
		return -1;
	}

	if(IsSameDay(pstRoleMisc->DayExpTime, pstEnv->pstAppCtx->stCurr.tv_sec) == 0)
	{
		pstPlayer->stOnline.stDyn.stMidAttrList.uiDayExpMax = get_player_dayexplimit(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	if (0 == (MISC_FLAG_EXPNODAYLIMIT & pstPlayer->stOnline.cMiscFlag))
	{
		if ((long long)pstRoleMisc->DayExpCount + *piExp >= pstPlayer->stOnline.stDyn.stMidAttrList.uiDayExpMax)
		{
			if(!(pstPlayer->stOnline.iGameNotifyFlag & PLAYER_DAY_EXP_WARNING))
			{
				char szBuff[128];
				
				pstPlayer->stOnline.iGameNotifyFlag |= PLAYER_DAY_EXP_WARNING;
				snprintf(szBuff, CCH(szBuff), "RoleName=%s, RoleID=%llu, DayExp=%d, DayExp Warning", 
					pstPlayer->stRoleData.RoleName, pstPlayer->stRoleData.RoleID, pstRoleMisc->DayExpCount);
				z_game_notify(pstEnv, szBuff, CCH(szBuff));
			}
		}
	}
	
	if (pstPlayer->stOnline.stDyn.stMidAttrList.uiDayExpMax &&
		(0 == (MISC_FLAG_EXP_NOLEVEL_LIMIT & pstPlayer->stOnline.cMiscFlag) &&
			0 == (MISC_FLAG_EXPNODAYLIMIT & pstPlayer->stOnline.cMiscFlag)))
	{
		if ((long long)pstRoleMisc->DayExpCount + *piExp >= pstPlayer->stOnline.stDyn.stMidAttrList.uiDayExpMax)
		{
			llTmpExp1 = (long long)pstPlayer->stOnline.stDyn.stMidAttrList.uiDayExpMax - pstRoleMisc->DayExpCount;
			if (0 > llTmpExp1)
			{
				llTmpExp1 = 0;
			}
			
			iFlag = 1;
		}	
	}

	if (pstPlayer->stRoleData.Level < pstEnv->iLevelStandStart)
	{
		int iLevelTmp = pstPlayer->stRoleData.Level;

		while(iLevelTmp < pstEnv->iLevelStandStart)
		{
			pstLevelUp = z_get_levelup(pstEnv, iLevelTmp);
			if(NULL == pstLevelUp)
			{
				return -1;
			}

			llLimitExp += pstLevelUp->LevelNextExp;
			
			iLevelTmp++;
		}

		pstLevelUp = z_get_levelup(pstEnv, pstEnv->iLevelStandStart);
		if(NULL == pstLevelUp)
		{
			return -1;
		}

		llLimitExp += pstLevelUp->LevelNextExp*1.0*pstLevelUp->ExpLeiji;
	}
	else
	{
		pstLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
		if(NULL == pstLevelUp)
		{
			return -1;
		}

		llLimitExp += pstLevelUp->LevelNextExp*1.0*pstLevelUp->ExpLeiji;
	}

	if( 0 != llLimitExp &&
		 (0 == (MISC_FLAG_EXP_NOLEVEL_LIMIT & pstPlayer->stOnline.cMiscFlag) && 
		 	0 == (MISC_FLAG_EXPNODAYLIMIT & pstPlayer->stOnline.cMiscFlag)) &&
		((long long)pstPlayer->stRoleData.RoleDetail.Exp + *piExp >= llLimitExp))
	{
		int iTmpFlag;
		if (pstLevelUp->Level >= pstEnv->pstConf->MaxLevel ||
			(pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat == GLOBAL_FINI &&
				(pstEnv->pstGlobalObj->stGlobal.LevFeng.LevelFeng > 0 && pstPlayer->stRoleData.Level >= pstEnv->pstGlobalObj->stGlobal.LevFeng.LevelFeng)))
		{
			iTmpFlag = 3;
		}
		else
		{
			iTmpFlag = 2;
		}
		
		llTmpExp2 = llLimitExp - pstPlayer->stRoleData.RoleDetail.Exp;
		if (0 > llTmpExp2)
		{
			llTmpExp2 = 0;
		}

		if (iFlag)
		{
			if (llTmpExp2 <= llTmpExp1)
			{
				iFlag = iTmpFlag;
			}
		}
		else
		{
			iFlag = iTmpFlag;
		}
	}
	

	/*if( pstLevelUp->Level != CAREERCHG_LEVEL && 
		((long long)pstPlayer->stRoleData.RoleDetail.Exp + *piExp >= llLimitExp) &&
		pstLevelUp->Level != MAX_LEVEL)
	{
		
		llTmpExp2 = llLimitExp - pstPlayer->stRoleData.RoleDetail.Exp;
		if (0 > llTmpExp2)
		{
			llTmpExp2 = 0;
		}

		if (iFlag)
		{
			if (llTmpExp2 < llTmpExp1)
			{
				iFlag = 2;
			}
		}
		else
		{
			iFlag = 2;
		}
	}*/
	

	if (iFlag)
	{
		if (pstPlayer->stRoleData.Level < pstEnv->iLevelStandStart)
		{
			*piExp = 1;
				
			return 0;
		}
		
		if (1 == iFlag)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER116);
			*piExp = llTmpExp1;
		}
		else if (2 == iFlag)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER117);
			*piExp = llTmpExp2;
		}
		else if(3 == iFlag)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER118);
			*piExp = llTmpExp2;
		}
		return -iFlag;
	}

	return 0;
}

int exit_pworld_clean_xp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	PlayerChagAttrVal stChgVal;
	AttrChgVal *pstVal;

	stChgVal.AttrNum = 0;
	pstVal = &stChgVal.AttrList[stChgVal.AttrNum];
	pstVal->AttrID = ATTR_ID_XP;
	pstVal->nFlag = 1;
	pstVal->Value.XP = pstPlayer->stRoleData.RoleDetail.XP;
	stChgVal.AttrNum++;

	z_attr_chg(pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);
	return 0;
}

int player_mon_pkmode_make_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, CSMONPKMODECHG *pstMonPkModeChg)
{
	Player *pstOwner = NULL;

	memset(pstMonPkModeChg,0,sizeof(CSMONPKMODECHG));
	pstMonPkModeChg->Index = pstMon->iDefIdx;
	pstMonPkModeChg->PKInfoType = pstMon->iPKType;
	pstMonPkModeChg->PkID = pstMon->ullPKID;

	if(pstMon->iPKType == MON_PK_TYPE_PLAYER)
	{
		pstOwner = z_id_player(pstAppCtx, pstEnv, pstMon->ullPKID);
		if (pstOwner)
		{
			pstMonPkModeChg->OwnerPKMode = pstOwner->stOnline.ePkMode;
			pstMonPkModeChg->OwnerClanID = pstOwner->stRoleData.MiscInfo.ClanInfo.ClanId.GID;
		}
	}
	
	return 0;
}
int player_mon_pk_mode_action(ZONESVRENV* pstEnv,Monster *pstMon)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstActionRes->ID = pstMon->iID;
	pstActionRes->ActionID = ACTION_MON_PK_MODE_CHG;

	if(pstMon->iPKType != MON_PK_TYPE_PLAYER)
	{
		return -1;
	}

	player_mon_pkmode_make_view(pstEnv->pstAppCtx, pstEnv, pstMon, &pstActionRes->Data.MonPkModeChg);
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_monview(pstEnv->pstAppCtx, pstEnv, pstMon, &stPkg, 0);
	
	return 0;
}
int player_mon_pk_mode_change(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	int iTotemID;
	Monster *pstTotem;
	Monster *pstMachine;
	ZoneSkill *pstZoneSkill;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;
	
	for (i=0; i<pstZoneSkillList->nSkillNum; i++)
	{
		pstZoneSkill = &pstZoneSkillList->astZoneSkills[i];
		if (pstZoneSkill->bTotem > 0)
		{
			iTotemID = pstZoneSkill->iTotemHead;
			while(iTotemID > 0)
			{
				pstTotem = z_id_monster(pstEnv->pstAppCtx, pstEnv, iTotemID);
				if (NULL == pstTotem)
				{
					break;
				}
				iTotemID = pstTotem->iTotemNext;
				
				player_mon_pk_mode_action(pstEnv,pstTotem);
			}
		}
	}

	//todo: 佣兵后面是不是也需要搞一下

	pstMachine = machine_get_by_player(pstEnv, pstPlayer);
	if(pstMachine && is_player_driver_machine(pstEnv, pstPlayer))
	{
		player_mon_pk_mode_action(pstEnv, pstMachine);
	}
	
	return 0;
}

int player_pk_mode_action(ZONESVRENV* pstEnv, Player *pstPlayer, int fBroadcast)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_PK_MODE_CHG;

	pstActionRes->Data.PkModeChg.Mode = pstPlayer->stOnline.ePkMode;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	
	if(fBroadcast)
	{
		z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	}
	else
	{
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	player_mon_pk_mode_change(pstEnv, pstPlayer);

	return 0;
}

int player_pk_val_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_PK_VAL_CHG;

	pstActionRes->Data.PkValChg.PKVal = pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	
	return 0;
}

int player_pkval_chg_outpress(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i, j;
	SearchArea stSArea;
	AREADYN *pstArea;
	char cType;
	TMEMBLOCK *pstMemBlock;
	Npc *pstNpc;
	ZONEIDX stZoneIdx;
	ZONEMAPINST *pstMapInst;
	int iIndex;

	stZoneIdx.iID = pstPlayer->stRoleData.Map;
	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stZoneIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	if (z_pos_search_area(pstEnv->pstAppCtx, pstEnv,  pstMapInst, &pstPlayer->stRoleData.Pos, &stSArea) < 0) 
		return -1;
		
	for (i = 0; i < stSArea.iNum; i++) 
	{
		pstArea = stSArea.pastArea[i];

		// 出压力表事件会导致区域内memid变动
		for (j =pstArea->iAnimate-1; j >=0; j--)
             {
			iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv,  pstArea->aiAnimate[j], &cType);

			if (iIndex < 0) continue;

			if (cType == OBJ_NPC)
			{
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					continue;
				}
				
				pstNpc = (Npc *)pstMemBlock->szData;
				mon_press_delete(&pstNpc->stMon, pstPlayer->iMemID);
				mon_ai_evt_outpress(pstEnv, &pstNpc->stMon);
			}
		}
       }
	return 0;
}

int player_pk_val_chg(ZONESVRENV* pstEnv, Player *pstPlayer, int iVal)
{
	int iOldPKVal = pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal;
	
	if(pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal + iVal <= 0)
	{
		pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal = 0;
	}
	else
	{
		pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal += iVal;
	}

	if(pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal > MAX_PK_VAL)
	{
		pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal = MAX_PK_VAL;
	}

	if (iOldPKVal != pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal)
	{
		Monster *pstMachine = machine_get_by_player(pstEnv, pstPlayer);

		if(pstMachine)
		{
			machine_pkval_update( pstEnv, pstMachine);
		}
		
		range_single_set(pstEnv, pstPlayer, RANGE_MAIN_SINGLE_SUB_PK, pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal);
		player_pk_val_action(pstEnv, pstPlayer);
	}

	if (iOldPKVal >= PLAYER_PK_FRENZY &&
		pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal < PLAYER_PK_FRENZY)
	{
		player_pkval_chg_outpress(pstEnv, pstPlayer);
	}

	if(iOldPKVal < PLAYER_PK_FRENZY && pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal >= PLAYER_PK_FRENZY)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_PLAYER154);
	}
	else if(iVal > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_PLAYER155, iVal/10, pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal/10);
	}
	return 0;
}

int player_hp_action(ZONESVRENV* pstEnv, Player *pstPlayer, int iOldHP, int iOldMP, int iApplyID)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_HP_CHG;

	pstActionRes->Data.HPChg.ApplyID = iApplyID;
		
	pstActionRes->Data.HPChg.ChgHP = pstPlayer->stRoleData.RoleDetail.HPCurr - iOldHP;
	pstActionRes->Data.HPChg.CurrHP = pstPlayer->stRoleData.RoleDetail.HPCurr;
	pstActionRes->Data.HPChg.CurrMaxHP = pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax;

	pstActionRes->Data.HPChg.ChgMP = pstPlayer->stRoleData.RoleDetail.MPCurr - iOldMP;
	pstActionRes->Data.HPChg.CurrMP = pstPlayer->stRoleData.RoleDetail.MPCurr;
	pstActionRes->Data.HPChg.CurrMaxMP = pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	
	return 0;
}

int mon_weixing_tick(ZONESVRENV* pstEnv, Monster *pstMon)
{
	int i;
	int iCount = 0;
	Player * apstPlayers[MAX_MON_FOLLOW];
	Player * pstOther;
	
	if (pstMon->iFollow <= 0)
	{
		return 0;
	}

	for (i=pstMon->iFollow-1; i>=0; i--)
	{
		pstOther = z_id_player(pstEnv->pstAppCtx,pstEnv,pstMon->astFollow[i]);
		if (pstOther && pstOther->eStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			apstPlayers[iCount++] = pstOther;
		}
		else
		{
			pstMon->iFollow --;
			if (i != pstMon->iFollow)
			{
				pstMon->astFollow[i]= pstMon->astFollow[pstMon->iFollow];
			}
		}
	}

	if (iCount > 0)
	{
		MapIndex* pstMapIdx;
		CSPKG stPkg;
		CSWEIXINGSVR *pstSvr = &stPkg.Body.WeiXingSvr;

		pstSvr->WeiXingOp = WEIXING_SVR_POS;
		pstSvr->WeiXingData.WeiXingPos.Map = GET_MAPID(pstMon->stMap.iID);
		pstSvr->WeiXingData.WeiXingPos.MapLine  = 0;
		
		pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, pstMon->stMap.iID);
		if (pstMapIdx  && pstMapIdx->stMapDef.MapLine > 0)
		{
			pstSvr->WeiXingData.WeiXingPos.MapLine = pstMapIdx->stMapDef.MapLine;
		}
		
		pstSvr->WeiXingData.WeiXingPos.X = pstMon->stCurrPos.X;
		pstSvr->WeiXingData.WeiXingPos.Y = pstMon->stCurrPos.Y;
		Z_CSHEAD_INIT(&stPkg.Head, WEI_XING_SVR);
		
		return z_cltmsg_limit_broadcast( pstEnv->pstAppCtx,pstEnv,apstPlayers,iCount, &stPkg );
	}
	return 0;
}

int player_weixing_tick(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	WeiXingInfo *pstWxinfo = &pstPlayer->stOnline.stWeiXing;
	int iCount = 0;
	Player * apstPlayers[MAX_WEI_XING];
	Player * pstOther;
	
	for (i=pstWxinfo->cWeiXingNum-1; i>=0; i--)
	{
		pstOther = z_id_player(pstEnv->pstAppCtx,pstEnv,pstWxinfo->aiWeiXingMemID[i]);
		if (pstOther && pstOther->eStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			apstPlayers[iCount++] = pstOther;
		}
		else
		{
			pstWxinfo->cWeiXingNum --;
			if (i != pstWxinfo->cWeiXingNum)
			{
				pstWxinfo->aiWeiXingMemID[i] = pstWxinfo->aiWeiXingMemID[pstWxinfo->cWeiXingNum];
			}
		}
	}

	if (iCount > 0)
	{
		MapIndex* pstMapIdx;
		CSPKG stPkg;
		CSWEIXINGSVR *pstSvr = &stPkg.Body.WeiXingSvr;

		pstSvr->WeiXingOp = WEIXING_SVR_POS;
		pstSvr->WeiXingData.WeiXingPos.Map = GET_MAPID(pstPlayer->stRoleData.Map);
		pstSvr->WeiXingData.WeiXingPos.MapLine  = 0;
		
		pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, pstPlayer->stRoleData.Map);
		if (pstMapIdx  && pstMapIdx->stMapDef.MapLine > 0)
		{
			pstSvr->WeiXingData.WeiXingPos.MapLine = pstPlayer->stRoleData.MiscInfo.MapLine;
		}
		
		pstSvr->WeiXingData.WeiXingPos.X = pstPlayer->stRoleData.Pos.X;
		pstSvr->WeiXingData.WeiXingPos.Y = pstPlayer->stRoleData.Pos.Y;
		Z_CSHEAD_INIT(&stPkg.Head, WEI_XING_SVR);
		
		return z_cltmsg_limit_broadcast( pstEnv->pstAppCtx,pstEnv,apstPlayers,iCount, &stPkg );
	}

	return 0;
}

int role_login_pkval_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iDeltaTime;
	
	if(pstPlayer->stRoleData.MiscInfo.LogoutPkValTime <= 0)
	{
		pstPlayer->stRoleData.MiscInfo.LogoutPkValTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	iDeltaTime = pstEnv->pstAppCtx->stCurr.tv_sec -
	             pstPlayer->stRoleData.LastLogout;
	if(iDeltaTime <= 0)
	{
		return -1;
	}

	player_pk_val_chg(pstEnv, pstPlayer, -iDeltaTime/720*10);
	pstPlayer->stRoleData.MiscInfo.LogoutPkValTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	return 0;
}

// 6分钟减一点pkval
int z_check_player_pkval(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	RESULT *pstVipResult;
	STATUSRESULT stResult;
	int iVal;

	pstPlayer->stRoleData.MiscInfo.LogoutPkValTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	if(pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal == 0)
	{
		if(pstPlayer->stRoleData.MiscInfo.DecPkValTime)
		{
			pstPlayer->stRoleData.MiscInfo.DecPkValTime = 0;
		}
		
		return 0;
	}
	
	pstPlayer->stRoleData.MiscInfo.DecPkValTime += 10;
	if(pstPlayer->stRoleData.MiscInfo.DecPkValTime >= 60)
	{
		iVal = 5000/pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal*0.8;
		if(iVal <= 0)
		{
			iVal = 1;
		}

		z_get_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_DEC_PKVAL_QUICK, &stResult, 0);
		pstVipResult = star_get_result(pstEnv, pstPlayer, STAR_RESULT_PKVAL);
		if (pstVipResult)
		{
			stResult.ResultVal1 += pstVipResult->ResultVal1;
		}
		
		if (stResult.ResultVal1 > 0)
		{
			iVal = iVal*(100+stResult.ResultVal1)/100.0;
		}
		
		player_pk_val_chg(pstEnv, pstPlayer, -iVal);
		pstPlayer->stRoleData.MiscInfo.DecPkValTime = 0;
	}

	return 0;
}

int vim_role_login(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	PlayerChagAttrVal stChgVal;
	AttrChgVal *pstVal = NULL;
	int iMul = 0, iDeltaTime = 0;

	if(pstPlayer->bAasIncomeLimt > 0)
	{
		pstPlayer->stRoleData.MiscInfo.LastRestoreVimTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		return 0;
	}
	
	if(pstPlayer->stRoleData.RoleDetail.VIMCurr >=
	   pstPlayer->stOnline.stDyn.stMidAttrList.uiVimMax)
	{
		pstPlayer->stRoleData.MiscInfo.LastRestoreVimTime = 
			pstEnv->pstAppCtx->stCurr.tv_sec;
		return 0;
	}

	stChgVal.AttrNum = 0;
	/*
	if(!pstPlayer->stRoleData.MiscInfo.LastRestoreVimTime ||
	   !IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec,
	              pstPlayer->stRoleData.LastLogout))
	{
		// 过了一天, 全部回复
		pstVal = &stChgVal.AttrList[stChgVal.AttrNum++];
		pstVal->AttrID = ATTR_ID_VIM;
		pstVal->nFlag = 0;
		pstVal->Value.Vim = pstPlayer->stOnline.stDyn.stMidAttrList.uiVimMax;
		goto final;
	}
	*/

	iDeltaTime = pstEnv->pstAppCtx->stCurr.tv_sec -
	             pstPlayer->stRoleData.LastLogout;
	if(iDeltaTime < 0)
	{
		return -1;
	}
	iMul = iDeltaTime / (g_iVimRecoverInterval 
	                  ? g_iVimRecoverInterval : iDeltaTime + 1);
	if(iMul)
	{
		pstVal = &stChgVal.AttrList[stChgVal.AttrNum++];
		pstVal->AttrID = ATTR_ID_VIM;
		pstVal->nFlag = 0;
		pstVal->Value.Vim = iMul * VIM_RECOVER_OFFLINE;
	}

//final:
	if(stChgVal.AttrNum > 0)
	{
		z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);

		pstPlayer->stRoleData.MiscInfo.LastRestoreVimTime =
			pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	return 0;
}

int z_player_add_vim(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	PlayerChagAttrVal stChgVal;
	AttrChgVal *pstVal;
	int iDeltaTime = 0;

	if(pstPlayer->bAasIncomeLimt > 0)
	{
		pstPlayer->stRoleData.MiscInfo.LastRestoreVimTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		return 0;
	}
	
	stChgVal.AttrNum = 0;
	
	if(pstPlayer->stRoleData.RoleDetail.VIMCurr >=
	   pstPlayer->stOnline.stDyn.stMidAttrList.uiVimMax)
	{
		pstPlayer->stRoleData.MiscInfo.LastRestoreVimTime = 
			pstAppCtx->stCurr.tv_sec;
		return 0;
	}

	if(pstPlayer->stRoleData.MiscInfo.LastRestoreVimTime)
	{
		// 时间是连续的
		iDeltaTime += pstAppCtx->stCurr.tv_sec -
		              pstPlayer->stRoleData.MiscInfo.LastRestoreVimTime;
	}
	else
	{
		iDeltaTime += g_iVimRecoverInterval;
	}

	if(iDeltaTime >= g_iVimRecoverInterval)
	{
		int iRemain = iDeltaTime % (g_iVimRecoverInterval 
		                         ? g_iVimRecoverInterval : iDeltaTime);
		int iMul = iDeltaTime / (g_iVimRecoverInterval 
		                         ? g_iVimRecoverInterval : iDeltaTime);

		pstVal = &stChgVal.AttrList[stChgVal.AttrNum++];
		pstVal->AttrID = ATTR_ID_VIM;
		pstVal->nFlag = 0;
		pstVal->Value.Vim = iMul * VIM_RECOVER_ONLINE;

		pstPlayer->stRoleData.MiscInfo.LastRestoreVimTime =
			pstAppCtx->stCurr.tv_sec - iRemain;
	}

	if (stChgVal.AttrNum > 0)
	{
		z_attr_chg(pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);
	}

	return 0;
}

int player_item_use_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	ITEMUSECOOLLIST *pstItemCoolList= &pstPlayer->stRoleData.MiscInfo.ItemUseCoolList;
	CSPKG stPkg;
	CSITEMCOOLINFO *pstItemCoolInfo = &stPkg.Body.ItemCoolInfo;

	pstItemCoolInfo->ItemSelfCoolNum = 	pstItemCoolList->ItemSelfCoolNum;
	for (i=0; i<pstItemCoolInfo->ItemSelfCoolNum; i++)
	{
		pstItemCoolInfo->ItemSelfCools[i].ItemID = pstItemCoolList->ItemSelfCools[i].ItemID;
		pstItemCoolInfo->ItemSelfCools[i].Left = pstItemCoolList->ItemSelfCools[i].CoolEnd.BenchTimeSec - pstEnv->pstAppCtx->stCurr.tv_sec + 1;
		if (0 > pstItemCoolInfo->ItemSelfCools[i].Left)
		{
			pstItemCoolInfo->ItemSelfCools[i].Left = 0;
		}
	}

	pstItemCoolInfo->ItemSpeCoolNum = pstItemCoolList->ItemSpeCoolNum;
	for (i=0; i<pstItemCoolInfo->ItemSpeCoolNum; i++)
	{
		pstItemCoolInfo->ItemSpeCools[i].ItemCoolType = pstItemCoolList->ItemSpeCools[i].ItemCoolType;
		pstItemCoolInfo->ItemSpeCools[i].Left = pstItemCoolList->ItemSpeCools[i].CoolEnd.BenchTimeSec - pstEnv->pstAppCtx->stCurr.tv_sec + 1;
		if (0 > pstItemCoolInfo->ItemSpeCools[i].Left)
		{
			pstItemCoolInfo->ItemSpeCools[i].Left = 0;
		}
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, ITEM_COOL_INFO);

	return z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
}


static int pet_recover(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	ROLEPET *pstRolePet;
	PetOnline *pstPetOnline = &pstPlayer->stOnline.stPetOnline;

	for(i=0; i <pstPetOnline->bFightShiNum; i++)
	{
		pstRolePet = z_find_role_pet(pstPlayer, pstPetOnline->astFightShi[i].stRolePetOnline.ullWID, NULL);
		if (pstRolePet && (pstRolePet->StatFlag & PET_STAT_FIGHT))
		{
			pet_add_power(pstEnv, pstPlayer, pstRolePet, pstRolePet->MaxHP*5/100.0);
			break;
		}
	}

	return 0;
}

OFFLINEEXPPARAMETER* z_get_offline_exp_parameter(ZONESVRENV* pstEnv,int iType)
{

	
	if (iType < 1 || iType > pstEnv->pstZoneObj->iOfflineExpParameter)
	{
		return NULL;
	}

	return  (OFFLINEEXPPARAMETER*)pstEnv->pstZoneObj->sOfflineExpParameter[iType-1];

}

int player_offline_exp_tick(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	LEVELUP *pstLevel = NULL;
	OFFLINEEXPPARAMETER *pstPara = NULL;
	int iSec = 60;
	int iExp = 0;
	
	if (ACC_SHARE_FINAL != pstPlayer->stAccShareData.iStat)
	{
		return -1;
	}

	pstLevel = z_get_levelup(pstEnv, pstPlayer->stAccShareData.iAccMaxRoleLevel);
	if (!pstLevel)
	{
		tlog_error(pstEnv->pstLogCat,pstPlayer->stRoleData.Uin,0,
			"LevelUPDef get fail level=%d",pstPlayer->stAccShareData.iAccMaxRoleLevel);

		return -1;
	}

	if((pstPlayer->stOnline.cSecenOverLay & SCENEOVERLAY_MAP) ||
		pstPlayer->stRoleData.Map == BEACH_MAP_ID) 
	{
		pstPara = z_get_offline_exp_parameter(pstEnv,OFFLINE_EXP_CITY);
	}
	else
	{
		return -1;
	}
		
	if (pstPara && pstPara->Parameter1 != 0 )
	{
		//公式:M*sec/参数1
		iExp = (longlong ) pstLevel->ContributeExp * iSec / pstPara->Parameter1;
		if (iExp < 1)
		{
			iExp = 1;
		}
		
		//发协议
		z_player_awardexp_change(pstEnv,pstPlayer,iExp,TRUE);	

	}	
	
	

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_check_refind_skill(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i = 0;
	int j = 0;
	LEVELUP *pstNextLevelUp;

	for (j=1; j<=pstPlayer->stRoleData.Level; j++)
	{
		pstNextLevelUp = z_get_levelup(pstEnv, j);
		if (!pstNextLevelUp)
		{
			continue;
		}

		switch(pstPlayer->stRoleData.Career)
		{
		case CAREER_WARRIOR:
		case CAREER_WARRIOR_SINGLEHAND:
		case CAREER_WARRIOR_DOUBLEHAND:	
			for (i=0; i<(int)(sizeof(pstNextLevelUp->WarriorSkillList)/sizeof(pstNextLevelUp->WarriorSkillList[0])); i++)
			{
				if (pstNextLevelUp->WarriorSkillList[i] <= 0)
				{
					break;
				}
				z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstNextLevelUp->WarriorSkillList[i], 1, 1);
			}
			
			break;
		case CAREER_MAGICOR:
		case CAREER_MAGICOR_SINGLEHAND:
		case CAREER_MAGICOR_DOUBLEHAND:
			for (i=0; i<(int)(sizeof(pstNextLevelUp->MagicorSkillList)/sizeof(pstNextLevelUp->MagicorSkillList[0])); i++)
			{
				if (pstNextLevelUp->MagicorSkillList[i] <= 0)
				{
					break;
				}
				z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstNextLevelUp->MagicorSkillList[i], 1, 1);
			}
			
			break;
		case CAREER_GUNER:
		case CAREER_GUNER_SINGLEHAND:
		case CAREER_GUNER_DOUBLEHAND:
			for (i=0; i<(int)(sizeof(pstNextLevelUp->GunerSkillList)/sizeof(pstNextLevelUp->GunerSkillList[0])); i++)
			{
				if (pstNextLevelUp->GunerSkillList[i] <= 0)
				{
					break;
				}
				z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstNextLevelUp->GunerSkillList[i], 1, 1);
			}
			break;
		case CAREER_CHURCH:
		case CAREER_CHURCH_SINGLEHAND:
		case CAREER_CHURCH_DOUBLEHAND:	
			for (i=0; i<(int)(sizeof(pstNextLevelUp->ChurchSkillList)/sizeof(pstNextLevelUp->ChurchSkillList[0])); i++)
			{
				if (pstNextLevelUp->ChurchSkillList[i] <= 0)
				{
					break;
				}
				z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstNextLevelUp->ChurchSkillList[i], 1, 1);
			}
			break;	
		}
	}

	return 0;
}


int z_active_player_sec(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iCount = ++pstPlayer->stOnline.iSecCount;
	int iHP, iHPChg = 0, iMPChg = 0;
	PlayerChagAttrVal stChgVal;
	AttrChgVal *pstVal;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;
	ROLEDATA *pstRoleData =  &pstPlayer->stRoleData;
	int iOldHP = pstPlayer->stRoleData.RoleDetail.HPCurr;
	int iOldMP = pstPlayer->stRoleData.RoleDetail.MPCurr;
	MapIndex *pstMapIdx;
	MapIndex *pstMapIdx1;
	ZONEMAPINST* pstMapInst;
	ZONEPWORLDINST *pstPworldInst = NULL;
	ZONEIDX stIdx;

	if (pstPlayer->stOnline.bWaitRoleLogout)
	{
		return 0;
	}

	if (pstPlayer->stOnline.iDropTmpID != 0 && 
		pstPlayer->stOnline.tDropTmpItemEnd >= pstAppCtx->stCurr.tv_sec)
	{
		if (pstPlayer->stOnline.iDropTmpID == (int)pstRoleTmpSys->TmpItemID)
		{
			tmp_item_drop(pstEnv, pstPlayer, 1);
		}
		
		pstPlayer->stOnline.iDropTmpID = 0;
		pstPlayer->stOnline.tDropTmpItemEnd = 0;
	}

	stChgVal.AttrNum = 0;
	
	// 因为有限时任务以秒计算，所以不能10秒一次。
	task_tick(pstEnv, pstPlayer);

	z_player_tick_qiecuo(pstEnv, pstPlayer);

	// 精力值回复也提高精度吧.
	z_player_add_vim(pstAppCtx, pstEnv, pstPlayer);

	// 清理隔天信息 包括(FB  任务等)
	player_clean_diffsameday_info(pstEnv, pstPlayer);

	player_team_follow_sec(pstEnv, pstPlayer);

	if (0 == iCount%10)
	{
		
		player_daoba_sec(pstEnv ,pstPlayer);
		player_update_online_sec(pstEnv,pstPlayer,0);
		update_cont_online_time(pstEnv, pstPlayer, 0);
		player_updata_oneday_online_time(pstEnv,&pstPlayer->stRoleData.MiscInfo.RoleGiftInfo,0);
		
		player_update_onlinegiftgroup(pstEnv,pstPlayer);
		
		//z_check_ride_use(pstEnv,pstPlayer);
		//z_pet_add_skillcollectnum(pstAppCtx, pstEnv, pstPlayer);
		//z_player_add_vigor(pstEnv, pstPlayer);

		z_check_player_package(pstAppCtx, pstEnv, pstPlayer);

		//z_player_add_vim(pstAppCtx, pstEnv, pstPlayer);

		z_check_player_pkval(pstEnv, pstPlayer);

		package_cangbaotu_tick( pstEnv, pstPlayer);

		z_player_check_map_timelimit(pstEnv, pstPlayer);

		player_map_ore_info(pstEnv, pstPlayer);
		strong_glory_player_tick(pstEnv, pstPlayer);

		z_player_level_remain_tm_update(pstEnv, pstPlayer);

		if (pstPlayer->stRoleData.RoleDetail.HPCurr == 0 && 
			(0 == (CS_STAT_DEAD & pstPlayer->stOnline.State)))
		{
			pstPlayer->stOnline.State |= CS_STAT_DEAD;
			tlog_error(pstEnv->pstLogCat, 0, 0, "player uin %d hp 0 not die", pstPlayer->stRoleData.Uin);
		}
	}

	if (0 == iCount%60)
	{
		pei_heti_tick(pstEnv, pstPlayer);
		player_des_del_by_time(pstEnv, pstPlayer);
	}

	z_active_player_stall(pstAppCtx, pstEnv, pstPlayer);

	self_battle_info_tick(pstEnv,pstPlayer);

	//player_collectskill_add_slot( pstEnv, pstPlayer,0,COLLECTSKILL_ADDTYPE_TIME);
	//player_collectskill_dec_slot( pstEnv, pstPlayer);
	
	// 精灵tick
	fairy_diath_tick(pstEnv,pstPlayer);
	if (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_PACKAGE_ALL)
	{
		fairy_evolve_tick(pstEnv, pstPlayer);
	}

	if (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_LIFE_TIME_TRACE)
	{
		task_player_trace(pstEnv, pstPlayer, TASK_OBJ_PLAYER_LIFE_TIME);
	}

	//宠物
	pet_shi_active(pstEnv, pstPlayer);

	player_world_city_tick(pstEnv,pstPlayer);
	
	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_RANDOM_RUN))
	{
		z_svr_player_move(pstEnv, pstPlayer);
	}
	
	if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
	{
		relation_player_tick(pstEnv, pstPlayer);

		if(0 == iCount%20)
		{
			if(pstPlayer->stOnline.stSpanOnline.stApply.iBattleApply == SPAN_APPLY_ING ||
				pstPlayer->stOnline.stSpanOnline.stApply.iSpartaApply == SPAN_APPLY_ING)
			{
				player_span_apply_heartbeat_req(pstEnv, pstPlayer);
			}
		}

		if (pstEnv->pstConf->CountryVer == COUNTRY_VER_KOREA)
		{
			if (0 == iCount%600)
			{
				z_role_snapshot_oplog(pstAppCtx, pstEnv, pstPlayer);
			}
		}
		else
		{
			if (0 == iCount%1800)
			{
				z_role_snapshot_oplog(pstAppCtx, pstEnv, pstPlayer);
			}
		}
	}
	else if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
	{
		if (0 == iCount%5)
		{
			player_pvp_svr_member_info_sync_req(pstEnv, pstPlayer);
		}

		if(0 == iCount%20)
		{
			player_span_heartbeat_req(pstEnv, pstPlayer);
		}
	}
	
	if (pstRoleData->MiscInfo.OnlineAwardEnd > 0 &&  pstRoleData->MiscInfo.OnlineAwardIdx >= 0)
		pstRoleData->MiscInfo.OnlineAwardEnd --;


	if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
	{
		if ((0 == (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_PACKAGE_ALL)) 
		&& (pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.tRoleLogin > 1))
		{
			player_safelock_login(pstEnv, pstPlayer);
			player_package_info(pstEnv, pstPlayer);
			player_item_use_info(pstEnv, pstPlayer);
			player_ltrInfo(pstEnv,pstPlayer);
			if(is_player_ride_on_machine(pstPlayer))
			{
				machine_item_action(pstEnv, pstPlayer);
			}
			
			player_map_bigore_info(pstEnv, pstPlayer);

			if(pstPlayer->bAasFlag)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, 
					ZONEERR_PLAYER119);
			}

			star_stardata_chg(pstEnv, pstPlayer, 0);
		}
		
		if ((0 == (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_TASK_ALL)) 
			&& (pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.tRoleLogin > 2))
		{
			task_info_get(pstEnv, pstPlayer);
			pworld_limit_info_get(pstEnv, pstPlayer);
			battle_limit_info_get(pstEnv, pstPlayer);
		}

		if ((0 == (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_CLANID_SNAPSHOT)) 
			&& (pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.tRoleLogin > 2))
		{
			player_clan_id_snapshot(pstEnv, pstPlayer);
		}

		if ((0 == (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_SECONDARY_SKILL_ALL)) 
			&& (pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.tRoleLogin > 3))
		{
			player_secondary_skill_info(pstEnv, pstPlayer);
		}
		// 发下战场的商城数据
		if ((0 == (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_SEND_SHOPDEF)) 
			&& (pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.tRoleLogin > 3))
		{
			CSSHOPPINGFRESHRECORDREQ stShoppingFreshRecordReq;
			memset(&stShoppingFreshRecordReq,0,sizeof(stShoppingFreshRecordReq));
			shopping_fresh_shopdef_req(pstAppCtx, pstEnv, pstPlayer, &stShoppingFreshRecordReq);
			//pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_SEND_SHOPDEF;
		}

		if ((0 == (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_MAIL_SNAP)) 
			&& (pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.tRoleLogin > 4))
		{
			player_mail_head_snapshot( pstEnv, pstPlayer );
		}
		
		if ((0 == (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_CLAN_SNAP)) 
			&& (pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.tRoleLogin > 5))
		{
			clan_role_login(pstEnv, pstPlayer);
			if(player_get_clan(pstEnv, pstPlayer))
			{
				player_clan_info(pstEnv, pstPlayer);
				player_clan_diplomatism_snapshot(pstEnv, pstPlayer);
				player_clan_city_info(pstEnv, pstPlayer);
				player_clan_member_snapshot(pstEnv, pstPlayer);
				player_clan_advance(pstEnv, pstPlayer);
				player_clan_color_info(pstEnv, pstPlayer); // 公会头顶颜色信息
				login_check_chg_clanname(pstEnv, pstPlayer);
				player_clan_group_info(pstEnv, pstPlayer);
			}
		}

		if ((0 == (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_BULLETIN_SNAP)) 
			&& (pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.tRoleLogin > 6))
		{
			player_bulletin_snapshot(pstEnv, pstPlayer);
		}

		if ((0 == (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_DESIGNATION_SNAP)) 
			&& (pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.tRoleLogin > 7))
		{
			player_designation_info(pstEnv, pstPlayer);
		}

		if ((0 == (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_RESPACT_SNAP)) 
			&& (pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.tRoleLogin > 8))
		{
			player_respact_info(pstEnv, pstPlayer);

			//技能补偿, 暂时挂在这里，不放在check_data是因为消息的原因
			z_check_refind_skill(pstEnv, pstPlayer);
		}
	
	}
	

	//-------------player dead-----------------------------------------------

	z_player_check_status(pstAppCtx, pstEnv, pstPlayer);

	
	if (pstPlayer->stOnline.State & CS_STAT_DEAD)
	{
		return 0;
	}

	if (pstPlayer->stOnline.stWeiXing.cWeiXingNum > 0)
	{
		player_weixing_tick(pstEnv, pstPlayer);
	}

	//晕下马
	if( z_player_find_status(pstPlayer, STATUS_ID_TIAOZHANYUN) || z_player_find_status(pstPlayer, STATUS_ID_FAINT))
	{
		if( ( CS_STAT_RIDE_ON | CS_STAT_FLY | CS_STAT_HITCH_RIDE  ) & pstPlayer->stOnline.State)
		{
			z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
		}
	}

	//限时副本到时间 则回城
	if (pstPlayer->stRoleData.MiscInfo.PworldMisc.OutPworldTime != 0 &&
		pstPlayer->stRoleData.MiscInfo.PworldMisc.OutPworldTime < pstAppCtx->stCurr.tv_sec)
	{
		if (GET_PWORLDID(pstPlayer->stRoleData.Map) == pstPlayer->stRoleData.MiscInfo.PworldMisc.OutPworldID)
		{
			RESPOS stExitPos;
	
			stExitPos.X = pstPlayer->stRoleData.ExitPos_x;
			stExitPos.Y = pstPlayer->stRoleData.ExitPos_y;
	
			z_player_change_map(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstPlayer->stRoleData.ExitMap, &stExitPos, RAND1(180) , 0);
		}

		pstPlayer->stRoleData.MiscInfo.PworldMisc.OutPworldID = 0;
		pstPlayer->stRoleData.MiscInfo.PworldMisc.OutPworldTime = 0;
	}

	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos	=	pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);

	if (pstMapInst)
	{
		if (pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.stLastSearchTv.tv_sec > 1)
		{
			z_player_search_view(pstAppCtx, pstEnv, pstMapInst, &pstPlayer->stRoleData.Pos, pstPlayer);
		}

		pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
		if (pstMapIdx)
		{
			if (0 == iCount%3) //3
			{
				if(pstMapIdx->stMapDef.CtrlFlag & MAP_CTRL_RESTORE_HPMMP)
				{
					iHPChg += 20;
					iMPChg += 20;
				}
			}
			
			if (pstMapIdx->stMapDef.ReferMapID > 0)
			{
				pstMapIdx1 = z_mappos_index(pstEnv->pstMapObj, &pstMapIdx->stMapDef.ReferMapIDPos, pstMapIdx->stMapDef.ReferMapID);
			}
			else
			{
				pstMapIdx1 = pstMapIdx;
			}

			if (pstMapIdx1)
			{
				z_check_proc(pstEnv, pstPlayer, pstMapIdx1, NULL, 0);
			}
		}

		pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
		if (pstPworldInst && pstRoleData->MiscInfo.InPworldSafeTime>0)
		{
			pstRoleData->MiscInfo.InPworldSafeTime--;
		}
	}
	
	if (0 < pstPlayer->stOnline.tFightStatEnd && pstPlayer->stOnline.tFightStatEnd < pstAppCtx->stCurr.tv_sec)
	{
		memset(pstPlayer->stOnline.abFollowAngel, 0, sizeof(pstPlayer->stOnline.abFollowAngel));
		SET_PLAYER_NOFIGHT(pstPlayer);
		z_send_player_fight(pstEnv, pstPlayer, CLEAN_FIGHT_STATUS);
	}

	if (0 == iCount%10)
	{
		machine_item_update(pstEnv, pstPlayer);
	}//3

	//z_player_check_status 里面有加血加魔
	iOldHP = pstPlayer->stRoleData.RoleDetail.HPCurr;
	iOldMP = pstPlayer->stRoleData.RoleDetail.MPCurr;
	
	if (0 == iCount%3) //3
	{
		fairy_add_xp_by_sec(pstEnv,pstPlayer);
		
		z_pet_active( pstAppCtx,  pstEnv, pstPlayer);

		iHPChg += pstDyn->stHealChg.unArmHPAdd;
		iMPChg += pstDyn->stHealChg.unArmMPAdd;

		if (!(pstPlayer->stOnline.State & CS_STAT_FIGHT)) 
		{
			iHPChg += pstDyn->stHealChg.unNoFightRestoreHPAdd;
			iMPChg += pstDyn->stHealChg.unNoFightRestoreMPAdd;

			if (	(/*(pstPlayer->stOnline.cSecenOverLay & SCENEOVERLAY_SAFE) ||*/
				(pstPlayer->stOnline.cSecenOverLay & SCENEOVERLAY_MAP)) &&
				pstPlayer->stOnline.stQiecuoInfo.iMemID <= 0)
			{
				//iHPChg += pstDyn->stMidAttrList.uiHPMax*5/100.0;
				//iMPChg += pstDyn->stMidAttrList.uiMPMax*5/100.0;

				// 宠物
				pet_recover(pstEnv, pstPlayer);
			}

			if(!z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_CANNOT_ADDHP))
			{
				int iTmp;
				
				iTmp = pstDyn->stMidAttrList.unTili*0.5;
				iHPChg += iTmp*(10000.0+pstDyn->stHealChg.iNoFightRestoreHPMul)/10000.0;
				if(iHPChg <= 0)
				{
					iHPChg = 1;
				}
			}

			if(!z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_CANNOT_ADDMP))
			{
				int iTmp;
				
				iTmp = pstDyn->stMidAttrList.unJingshen*0.25;
				iMPChg += iTmp*(10000.0+pstDyn->stHealChg.iNoFightRestoreMPMul)/10000.0;
				if(iMPChg <= 0)
				{
					iMPChg = 1;
				}
			}
		}
		
		if (pstPlayer->stOnline.State & (CS_STAT_SIT|CS_STAT_GAOJI_SIT))
		{
			iHPChg += pstDyn->stMidAttrList.uiSitRestoreHP;
			iMPChg += pstDyn->stMidAttrList.uiSitRestoreMP;
		}
		else if(!(pstPlayer->stOnline.State & CS_STAT_FIGHT))
		{
			iHPChg += pstDyn->stMidAttrList.uiLevelUpAddHp;
			iMPChg += pstDyn->stMidAttrList.uiLevelUpAddMp;
		}

		if (0 > iHPChg) //不会因状态减血死亡
		{
			iHP = pstPlayer->stRoleData.RoleDetail.HPCurr;
			if (iHP > 1)
			{
				if ((iHP + iHPChg) < 1)
				{
					iHPChg = -(iHP -1);
				}
				
				pstVal = &stChgVal.AttrList[stChgVal.AttrNum];
				pstVal->AttrID = ATTR_ID_HP;
				pstVal->nFlag = 1;
				pstVal->Value.HP = -iHPChg;
				stChgVal.AttrNum++;
			}
		}
		else if (0 < iHPChg)
		{
			if (pstPlayer->stRoleData.RoleDetail.HPCurr < pstDyn->stMidAttrList.uiHPMax)
			{
				pstVal = &stChgVal.AttrList[stChgVal.AttrNum];
				pstVal->AttrID = ATTR_ID_HP;
				pstVal->nFlag = 0;
				pstVal->Value .HP = iHPChg;
				stChgVal.AttrNum++;
			}
		}

		if (0 > iMPChg)
		{
			if (pstPlayer->stRoleData.RoleDetail.MPCurr > 0)
			{
				pstVal = &stChgVal.AttrList[stChgVal.AttrNum];
				pstVal->AttrID = ATTR_ID_MP;
				pstVal->nFlag = 1;
				pstVal->Value.MP = -iMPChg;
				stChgVal.AttrNum++;
			}
			
		}
		else if (0 < iMPChg)
		{
			if (pstPlayer->stRoleData.RoleDetail.MPCurr < pstDyn->stMidAttrList.uiMPMax)
			{
				pstVal = &stChgVal.AttrList[stChgVal.AttrNum];
				pstVal->AttrID = ATTR_ID_MP;
				pstVal->nFlag = 0;
				pstVal->Value.MP = iMPChg;
				stChgVal.AttrNum++;
			}
		}

	}
	
	if (pstRoleTmpSys->TmpEnerge > 0 && pstRoleTmpSys->TmpItemID > 0 )
	{
		ITEMDEF *pstItemDef;

		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
		if (pstItemDef && ITEM_TMP_MACHINE == pstItemDef->ItemTmp.Type && pstItemDef->ItemTmp.EnergeDec > 0)
		{
			if (pstRoleTmpSys->TmpEnerge > pstItemDef->ItemTmp.EnergeDec)
			{
				pstRoleTmpSys->TmpEnerge -= pstItemDef->ItemTmp.EnergeDec;
			}
			/*else
			{
				pstRoleTmpSys->TmpEnerge = 0;
			}*/
			
			pstVal = &stChgVal.AttrList[stChgVal.AttrNum];
			pstVal->AttrID = ATTR_ID_TMPENERGE;
			if( pstRoleTmpSys->TmpEnerge >0 )
			{
				pstVal->Value.Energe = pstRoleTmpSys->TmpEnerge ;
			}
			else
			{
				pstVal->Value.Energe = 0;
			}
			stChgVal.AttrNum++;
		}
	}

	/*消耗移动力*/
	//player_status_consume( pstEnv,&stChgVal,pstPlayer);
	
	if (stChgVal.AttrNum > 0)
	{
		z_attr_chg(pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);
	}

	if (iOldHP != (int)pstPlayer->stRoleData.RoleDetail.HPCurr ||
		iOldMP != (int)pstPlayer->stRoleData.RoleDetail.MPCurr)
	{
		player_hp_action(pstEnv, pstPlayer, iOldHP, iOldMP, 0);
	}

	if (CS_STAT_COLLECT & pstPlayer->stOnline.State)
	{
		player_collect_award(pstEnv, pstPlayer);
	}

	if(pstPlayer->stOnline.iSelectTarget != pstPlayer->stOnline.iOldSelectTarget)
	{
		pstPlayer->stOnline.iOldSelectTarget = pstPlayer->stOnline.iSelectTarget;
		player_target_action(pstEnv, pstPlayer);
	}

	player_beach_tick(pstEnv,pstPlayer);
	player_luck_tick(pstEnv,pstPlayer);

	
	
	return 0;
}

int player_check_uin_cache(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	UinCache *pstUinCache = NULL;
	int iValid = 0;

	if (PLAYER_STATUS_ROLE_LIST != pstPlayer->eStatus) return -1;

	pstUinCache = sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos,&iValid);
	if(!pstUinCache || !iValid ||pstUinCache->iUin != pstPlayer->stRoleData.Uin)
	{
		return -1;
	}

	z_check_uin_cache(pstEnv,pstUinCache,pstPlayer);
	return 0;
}

int z_active_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iCount)
{
	static struct timeval stRoundTv;
	static struct timeval stRoundStartTv;
	static unsigned int iActivePos = 0, iRound = 0;
	int i;
	struct timeval stSub;
	long long  llMs=0;
	Player *pstPlayer;
	TMEMBLOCK *pstMemBlock;

	if (0 == stRoundTv.tv_sec)
	{
		llMs = PLAYER_ACTIVE_TIMER;
	}
	else
	{
		TV_DIFF(stSub, pstAppCtx->stCurr, stRoundTv);
		TV_TO_MS(llMs, stSub);
	}

	if (0 > llMs)
	{
		TV_CLONE(stRoundTv, pstAppCtx->stCurr);
		TV_CLONE(stRoundStartTv, pstAppCtx->stCurr);
	}
		
	if (llMs < PLAYER_ACTIVE_TIMER)
	{
		return 0;
	}

	if (0 == iActivePos)
	{
		TV_CLONE(stRoundStartTv, pstAppCtx->stCurr);
	}

	for (i = 0; i < iCount; i++) 
	{
		if (iActivePos >= MAX_PLAYER) 
		{
			break;
		}
		
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, iActivePos);
		iActivePos++;

		if (!pstMemBlock->fValid)
		{
			continue;
		}
		
		pstPlayer = (Player *)pstMemBlock->szData;

		assert(pstPlayer->iMagic == PLAYER_SHM_MAGIC);
		
		if (PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus && DIRTY_IMPORTANT == pstPlayer->stOnline.cDataDirty)
		{
			if (0 == z_db_role_update(pstAppCtx, pstEnv, pstPlayer,0))
			{
				pstPlayer->stOnline.cDataDirty = 0;
			}
		}

		if (PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus) 
		{
			if (pstPlayer->stOnline.cMoving)
			{
				z_player_move_curr(pstAppCtx, pstEnv,  pstPlayer, 1);
			}
			else if (pstPlayer->stOnline.stLastSpeed0.tv_sec)
			{
				if (pstPlayer->stOnline.stMspdInfo.MoveSpdCurr == 0)
				{
					struct timeval stDiff;
					long long  llMs;

					TV_DIFF(stDiff, pstAppCtx->stCurr, pstPlayer->stOnline.stLastSpeed0);
					TV_TO_MS(llMs, stDiff);

					if (llMs > 0)
					{
						TV_ADD_MS(pstPlayer->stOnline.stLastMoveTv, pstPlayer->stOnline.stLastMoveTv, llMs);
						pstPlayer->stOnline.stLastSpeed0 = pstAppCtx->stCurr;
					}
				}
				else
				{
					pstPlayer->stOnline.stLastSpeed0.tv_sec = 0;
				}
			}
		}
		else if (PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus && !pstPlayer->stOnline.cMoving) 
		{
			z_player_stop_by_patrol_end( pstEnv, pstPlayer);
		}
							
		if (iActivePos % 2 == iRound % 2) 
		{
			if (PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus)
			{
				z_active_player_sec(pstAppCtx, pstEnv, pstPlayer);
				star_stone_check_endtime(pstEnv, pstPlayer);
			}	
		}
		
		//0.5秒弄一次
		player_atk_by_lightning(pstAppCtx, pstEnv, pstPlayer);
		
		//十秒钟
		if (iActivePos % 20 == iRound % 20) 
		{
			// 防沉迷提醒
			if (pstPlayer->bAasFlag)  
			{
				CheckAasOnline(pstEnv, pstPlayer, &pstPlayer->stAasTime);
			}

			player_check_uin_cache(pstEnv, pstPlayer);
		}

		//一分钟
		if (iActivePos % 120 == iRound % 120)
		{
			//离线补偿经验
			player_offline_exp_tick(pstAppCtx,pstEnv,pstPlayer);
		}

		//5分钟
		if (iActivePos % 600 == iRound % 600) 
		{
			if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
			{
				z_heartbeat_to_region(pstEnv, pstPlayer->stRoleData.Uin);

				if (pstPlayer->bAasFlag)  
				{
					
					send_to_aas(pstEnv, pstPlayer->stRoleData.Uin, AAS_GAME_ACTIVE);
				}
			}
		}
	
		if (1 == pstPlayer->stOnline.bWaitRoleLogout)
		{
			if (pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.tLogoutBegin >= PLAYER_DELAY_LOGOUT_TIME)
			{
				pstPlayer->eStatus = PLAYER_STATUS_ROLE_LOGOUT_REQ;
				tlog_info(pstEnv->pstLogCat, 0, 0, "Status=RoleLogOutReq Uin=%d AccName=%s", 
							pstPlayer->stRoleData.Uin, pstPlayer->szAccount);

				if(!is_my_master_svr(pstAppCtx->iId, pstPlayer))
				{
					z_db_role_update(pstEnv->pstAppCtx, pstEnv, pstPlayer, SPAN_SVR_ROLE_KICK);
				}
				else
				{
					z_db_role_update(pstAppCtx, pstEnv, pstPlayer,0);
				}
				
				pstPlayer->stOnline.bWaitRoleLogout = 2;
			}
		}

		if( pstPlayer->ucRoleLogoutReason 
			&& ((int)(pstPlayer->stRoleData.LastLogout + LOGOUT_CHECK_TIMER) < pstAppCtx->stCurr.tv_sec))
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
						"RoleName=%s player role logout timer out, logout reason=%d, delta secs = %d", 
						pstPlayer->stRoleData.RoleName, 
						pstPlayer->ucRoleLogoutReason,
						pstAppCtx->stCurr.tv_sec - pstPlayer->stRoleData.LastLogout );
			
			if(!is_my_master_svr(pstAppCtx->iId, pstPlayer))
			{
				z_db_role_update(pstEnv->pstAppCtx, pstEnv, pstPlayer, SPAN_SVR_ROLE_KICK);
			}
			else
			{
				z_db_role_update(pstAppCtx, pstEnv, pstPlayer,0);
			}
		}
		else if (pstPlayer->tActive + pstEnv->pstConf->CheckIdle < pstAppCtx->stCurr.tv_sec)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "RoleName %s Active=%d, CheckIdle=%d, Curr=%d",
						pstPlayer->stRoleData.RoleName, pstPlayer->tActive, pstEnv->pstConf->CheckIdle,
						pstAppCtx->stCurr.tv_sec);

			if (pstPlayer->tActive + pstEnv->pstConf->CheckIdle*30 < pstAppCtx->stCurr.tv_sec)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "idle kick");
				player_kickoff(pstAppCtx, pstEnv, pstPlayer, ZONEERR_KICK206);
				player_disconnect( pstEnv, pstPlayer );
			}
		}
	}
	if (iActivePos < MAX_PLAYER)
	{
		return 0;
	}
	
	iActivePos = 0;
	iRound++;
	TV_CLONE(stRoundTv, pstAppCtx->stCurr);

	TV_DIFF(stSub, pstAppCtx->stCurr, stRoundStartTv);
	TV_TO_MS(llMs, stSub);
	if (llMs > PLAYER_ACTIVE_TIMER)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "active player loop %lld ms", llMs);
	}
	
	return 0;
}

int z_touch_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i;
	Player *pstPlayer;

	for (i=0; i<MAX_PLAYER; i++)
	{
		pstPlayer = tmempool_get_bypos(pstEnv->pstPlayerPool, i);
		if (pstPlayer)
		{
			assert(pstPlayer->iMagic == PLAYER_SHM_MAGIC);
			pstPlayer->tActive = pstAppCtx->stCurr.tv_sec;
		}
	}

	return 0;
}


int player_daily_award_check_condition(ZONESVRENV* pstEnv, Player *pstPlayer, int iCondition)
{
	if (iCondition & DAILY_COND_TYPE_STRONG)
	{
		if (strong_get_by_player(pstEnv, pstPlayer) == NULL)
		{
			return -1;
		}
	}
	else if (iCondition & DAILY_COND_TYPE_CITY_OWNER)
	{
		if (is_world_city_owner(pstEnv, pstPlayer) == 0)
		{
			return -1;
		}
	}
	else
	{
		return 0;
	}
	return 0;
}

int player_action(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	//CSPKG *pstCSPkgRes = &pstEnv->stCSRes;
	//CSACTIONRES *pstActionRes = &pstCSPkgRes->Body.ActionRes;
	//CSACTIONSTATE *pstActionStat= &pstActionRes->Data.Stat;
	CSACTIONREQ *pstAction = &pstCsPkg->Body.ActionReq;
		
	switch(pstAction->ActionID)
	{
	/*
	case ACTION_STAND:
		pstPlayer->stOnline.State &=	~CS_STAT_SIT;
		pstPlayer->stOnline.State &=	~CS_STAT_RUN;
		pstPlayer->stOnline.State &=	~CS_STAT_WALK;
		break;
	case ACTION_RUN:
		if (0 == z_player_act_status(pstAppCtx, pstEnv, pstPlayer, RES_ACTION_WALK))
		{
			return -1;
		}

		pstPlayer->stOnline.State &=	~CS_STAT_SIT;
		pstPlayer->stOnline.State &=	~CS_STAT_WALK;
		pstPlayer->stOnline.State |=	CS_STAT_RUN;
		break;
	case ACTION_SIT:
		if (0 == z_player_act_status(pstAppCtx, pstEnv, pstPlayer, RES_ACTION_SIT))
		{
			return -1;
		}
		
		pstPlayer->stOnline.State &=	~CS_STAT_RUN;
		pstPlayer->stOnline.State &=	~CS_STAT_WALK;
		pstPlayer->stOnline.State |=	CS_STAT_SIT;
		break;
	*/
	case ACTION_CMD_RET:
		if (CLIENT_CMD_AUTO_MOVE == pstAction->Data.CmdRet.CmdType)
		{
			AutoMove *pstAutoMove = &pstPlayer->stOnline.stAutoMoveInfo;

			// 严格判断到终点才删除
			if( pstAutoMove->iMapID == pstPlayer->stRoleData.Map &&
				0 == z_trust_clt_pos(pstAppCtx, pstEnv, pstPlayer, pstAutoMove->iX, pstAutoMove->iY, &pstAppCtx->stCurr, 1))
			{
				if(pstPlayer->stOnline.cMiscFlag & MISC_FLAG_TMPITEM_PATROL_TRACE)
				{
					task_player_trace(pstEnv, pstPlayer, TASK_OBJ_TMPITEM_PATROL_END);
				}

				if (pstAutoMove->iTmpItemID != 0)
				{
					//弄到脚本里去删除了
					tmp_item_del(pstEnv,pstPlayer);
					pstAutoMove->iTmpItemID = 0;
				}

				pstAutoMove->iMapID = 0;

				{
					CSPKG stPkg;
					CSACTIONRES *pstAction = &stPkg.Body.ActionRes;

					memset(pstAction,0,sizeof(*pstAction));

					pstAction->ID = pstPlayer->iMemID;
					pstAction->ActionID = ACTION_CLIENT_CMD;
					
					pstAction->Data.ClientCmd.CmdType = CLIENT_CMD_AUTO_MOVE;
					pstAction->Data.ClientCmd.CmdData.AutoMove.MapID = pstAutoMove->iMapID;
					pstAction->Data.ClientCmd.CmdData.AutoMove.X = pstAutoMove->iX;
					pstAction->Data.ClientCmd.CmdData.AutoMove.Y = pstAutoMove->iY;
					pstAction->Data.ClientCmd.CmdData.AutoMove.Stop = 1;

					Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
					z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
				}

				memset(pstAutoMove,0,sizeof(*pstAutoMove));
				
			}
			else
			{
				z_player_stop(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, NULL);
			}
		}
		break;
	case ACTION_TAKE_DAILY_GOAL_AWARD:
		{
			DAILYACTIVEINFO* pstInfo = &pstPlayer->stRoleData.MiscInfo.DailyActive;
			CSACTIONTAKEDAILYGOALAWARD* pstTake = &pstAction->Data.TakeDailyGoalAward;
			DAILYGOALAWARD* pstAward = NULL;
			
			if (pstTake->TakeAward > 4)
			{
				if (pstInfo->TaskNum < 4)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE, ZONEERR_PLAYER156);
					return -1;	
				}
			}
			else
			{
				if (pstTake->TakeAward > pstInfo->TaskNum)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE, ZONEERR_PLAYER156);
					return -1;
				}
			}
			
			if (pstInfo->TakeAwardFlag & (1 << pstTake->TakeAward))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE, ZONEERR_PLAYER157);
				return -1;
			}
			
			pstAward = z_find_daily_goal_award(pstEnv,pstTake->TakeAward);
			if (!pstAward)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE, ZONEERR_PLAYER158);
				return -1;
			}

			// 每日目标条件检查
			if (player_daily_award_check_condition(pstEnv, pstPlayer,pstAward->Condi) < 0)
			{
				return -1;
			}

			{
				int i = 0;
				int iRoleGrid = 0;
				int iBindGoldSum = 0;
				ROLEGRID astRoleGrid[MAX_DAILY_GOAL_AWARD_ITEM*2];
				
				for (i=0; i<(int)(sizeof(pstAward->ItemID)/sizeof(pstAward->ItemID[0])); i++)
				{
					if (iRoleGrid >= MAX_DAILY_GOAL_AWARD_ITEM)
					{
						break;
					}
					
					if (0 == pstAward->ItemID[i])
					{
						break;
					}

					if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &astRoleGrid[iRoleGrid],pstAward->ItemID[i]))
					{
						return -1;
					}

					astRoleGrid[iRoleGrid].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;

					if (GRID_ITEM_ITEM == astRoleGrid[iRoleGrid].Type)
					{
						astRoleGrid[iRoleGrid].GridData.RoleItem.Num =  pstAward->ItemNum[i];
					}
					
					iRoleGrid++;
				}

				if (iRoleGrid > 0)
				{
					if (0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, astRoleGrid, iRoleGrid,0))
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
										ZONEERR_NOSPACE);
						return -1;
					}
				}

				//pstAward->Gold 这个策划理解是绑定银币
				if (pstAward->Gold > 0)
				{
					if (0 > package_add_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, pstAward->Gold))
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
											ZONEERR_NOMORE_MONEY1);
						return -1;
					}
				}

				//pstAward->BindGold 这个策划理解是金券
				if (pstAward->BindGold > 0)
				{
					//获取金券=ceiling（角色等级*填表系数/10000，5）
					//iBindGoldSum =(( (int)(ceil((pstPlayer->stRoleData.Level*pstAward->BindGold)/10000.0)+4))/5*5);

					//客户端一样的公式
					iBindGoldSum = ceil( pstPlayer->stRoleData.Level * pstAward->BindGold / 50000.0) * 5;
					if(iBindGoldSum < 0)
					{
						iBindGoldSum = 0;
					}
					
					if (0 > package_add_money_test(pstEnv, pstPlayer, ATTR_ID_BINDGODCOIN, iBindGoldSum))
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
											ZONEERR_NOMORE_MONEY2);
						return -1;
					}
				}

				pstInfo->TakeAwardFlag |= (1 << pstTake->TakeAward);
				
				if (iRoleGrid > 0)
				{
					package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, astRoleGrid, iRoleGrid,0, IMPITEM_OPTYPE_DAY);
				}								

				if (pstAward->Gold > 0)
				{
					package_add_money(pstEnv, pstPlayer, ATTR_ID_BULL, pstAward->Gold);
					z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_DAY, 0, pstAward->Gold, 0, "今日目标");
				}

				if (pstAward->BindGold > 0)
				{
					package_add_money(pstEnv, pstPlayer, ATTR_ID_BINDGODCOIN, iBindGoldSum);
					z_bindgold_create_oplog(pstEnv, pstPlayer, BINDGOLD_OPTYPE_DAYGOLD, 0, iBindGoldSum,
								"今日目标");
				}

				z_daily_goal_award_oplog(pstEnv,pstPlayer,pstTake->TakeAward);
			}
			player_daily_goal_info(pstEnv,pstPlayer);
		}
		break;
	case ACTION_RUNE_LEVEL_UP:
			return player_rune_level_up(pstEnv,pstPlayer,&pstAction->Data.RuneLevelUp);
		break;
		
	case ACTION_RESET_RUNE_EX:
			return player_rune_reset_ex(pstEnv,pstPlayer,&pstAction->Data.ResetRuneEx);
		break;
	case ACTION_RUNNE_SKILL_CHG:
			return player_rune_skill_chg(pstEnv,pstPlayer,&pstAction->Data.RunneSkillChg);
		break;
		
	default:
		return -1;
		break;
	}
	/*
	pstPlayer->stOnline.bDir = pstAction->Dir;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_STATE;
	pstActionStat->Dir = pstPlayer->stOnline.bDir;
	pstActionStat->x = pstPlayer->stRoleData.Pos.X;
	pstActionStat->y = pstPlayer->stRoleData.Pos.Y;
	pstActionStat->State = pstPlayer->stOnline.State;

	Z_CSHEAD_INIT(&pstCSPkgRes->Head, ACTION_RES);

	z_sendpkg_playerview(pstAppCtx, pstEnv, pstPlayer, 0, pstCSPkgRes, 0);
	*/

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pstFrameHead);	//add by paraunused.pl
	UNUSED(pstCsPkg);	//add by paraunused.pl
}

int z_check_op_dist(ZONESVRENV* pstEnv, Player *pstPlayer,  int iMap, RESPOS* pstPos, int iOpDist)
{
	int iDist;

	if( iMap!=pstPlayer->stRoleData.Map)
		return -1;

	iDist	=	z_distance(&pstPlayer->stRoleData.Pos, pstPos);

	if( iDist>iOpDist )
		return -1;
	else
		return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int player_dialog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	int iIndex;
	char cType;
	MONSTERDEF *pstMonDef;
	Npc *pstNpc = NULL;
	int iProc = 0;
	int iSelect;
	char sSelect[20];
	//ScriptIndex *pstScpIdx;
	DialogOpt *pstOpt;
	CSDLGC *pstCSDlgC = &pstCsPkg->Body.DlgC;
	int iFlag = 0;
	int iRet = 0;
	ROLEITEM *pstRoleItem = NULL;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	int iClose = 0;
	
	if (pstCSDlgC->Type == DLG_TYPE_CLOSE) 
	{
		if(!(pstPlayer->stOnline.State & CS_STAT_DIALOG))
		{
			return 0;
		}
		
		pstPlayer->stOnline.cDialogStatOld = pstPlayer->stOnline.cDialogStat;
		pstPlayer->stOnline.cDialogStat = DIALOG_STAT_BEGIN;
		pstPlayer->stOnline.State &= ~CS_STAT_DIALOG;
		return 0;
	}

	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NODIALOG) ||
		z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOCLT))
	{
		return -1;
	}
	
	if (0 == z_player_act_status(pstAppCtx, pstEnv, pstPlayer, RES_ACTION_DIALOG))
	{
		return -1;
	}

	//普通乘客不可点击对话
	if(is_player_normal_passenger_machine(pstEnv, pstPlayer))
	{
		return -1;
	}

	if (pstPlayer->stOnline.cMoving)
		z_player_move_curr(pstAppCtx, pstEnv, pstPlayer, 0);

	
	//完成任务，接任务等回调到脚本时需要特殊处理
	if (0 == pstCSDlgC->NpcID)
	{
		char cTmp;
		
		iFlag = 1;

		cTmp = pstPlayer->stOnline.cDialogStat;
		pstPlayer->stOnline.cDialogStat = pstPlayer->stOnline.cDialogStatOld;
		pstPlayer->stOnline.cDialogStatOld = cTmp;
		pstCSDlgC->NpcID = pstPlayer->stOnline.iDialogID;
	}

	iIndex = z_id_indextype(pstAppCtx, pstEnv, pstCSDlgC->NpcID, &cType);
	if (0 > iIndex)
	{
		goto _error;
	}

	switch (cType) 
	{
		case OBJ_NPC:
			pstNpc = tmempool_get(pstEnv->pstNpcPool, iIndex);
			if (!pstNpc )
			{
				goto _error;
			}

			pstMonDef = z_get_mon_def(pstEnv, &pstNpc->stMon);
			if(NULL == pstMonDef)
			{
				goto _error;
			}

			if (0 == pstNpc->stMon.cOwnerIDType && pstNpc->stMon.iOwnerID > 0)
			{
				if (pstNpc->stMon.iOwnerID != pstPlayer->iMemID)
				{
					iClose = 1;
					goto _error;
				}
			}

			if (pstNpc->stMon.cOwnerIDType && pstNpc->stMon.iOwnerID != pstPlayer->iMemID)
			{
				ZoneTeam *pstZoneTeam = player_team_get(pstEnv, pstPlayer);

				if (NULL == pstZoneTeam || pstZoneTeam->stTeamInfo.TeamID != pstNpc->stMon.ullOwnerApplyID)
				{
					iClose = 1;
					goto _error;
				}
			}
			
			//pstScpIdx = &pstNpc->stScript;
			stSEnv.pstNpc = pstNpc;

			if (pstNpc->szScriptName[0] == 0)
			{
				iClose = 1;
				goto _error;
			}

			//阵营npc
			if (pstNpc->stMon.iCampIdx > 0 && pstNpc->stMon.iCampIdx != pstPlayer->stOnline.stWarOL.cCampIdx)
			{
				goto _error;
			}

			if (pstNpc->stMon.cMoving)
			{
				z_monster_move_curr(pstAppCtx, pstEnv, &pstNpc->stMon, 0);
			}
			
			break;
		default:
			return -1;
	}

	if (0 > z_check_op_dist(pstEnv, pstPlayer, pstNpc->stMon.stMap.iID,
	                        &pstNpc->stMon.stCurrPos, OP_DIST + pstNpc->stMon.unRadius))
	{
		goto _error;
	}

	tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player dialog input %s",
				pstPlayer->stRoleData.Uin, pstCSDlgC->Dlg.Input);

	if (pstPlayer->stOnline.iDialogID != pstCSDlgC->NpcID || pstPlayer->stOnline.cDialogStat == DIALOG_STAT_BEGIN) 
	{
		iProc = SCRIPT_PROC_DIALOG;
		pstPlayer->stOnline.iDialogID = pstCSDlgC->NpcID;
		pstPlayer->stOnline.cUIType = -1;
	}
	else if (pstPlayer->stOnline.cDialogStat == DIALOG_STAT_NEXT) iProc = pstPlayer->stOnline.stSData.shNext;
	else if (pstPlayer->stOnline.cDialogStat == DIALOG_STAT_OPT) {
		if (get_script_input(pstCSDlgC->Dlg.Input, "Select", sSelect, sizeof(sSelect)) < 0) {
			goto _error;
		}

		iSelect = atoi(sSelect);
		pstOpt = &pstPlayer->stOnline.stSData.stOpt;
		if( iSelect<0 || iSelect >= pstOpt->shNum) goto _error;
		iProc = pstOpt->ashProc[iSelect];
		pstPlayer->stOnline.stDynamicOpt.iSelectedOpt = iSelect; // 记录选择的是哪个选项.

	
		if(!(pstMonDef->MiscType & MON_MISC_TYPE_NPC_MOVE))
		{
			pstNpc->stMon.tFreezeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + 10;
			if (pstNpc->stMon.cMoving)
			{
				z_mon_stop(pstEnv, &pstNpc->stMon); 
			}
		}
	}
	else if (pstPlayer->stOnline.cDialogStat == DIALOG_STAT_COMMIT_OPT) {
		char szNum[32],szListType[32], szGridIdx[32];
		int  iPos,i;
		ITEMDEF *pstItemDef;
		DialogCommitOpt* pstOpt = &pstPlayer->stOnline.stSData.stCommitOpt;

		pstOpt->iNum = 0;
		memset(pstOpt->astGrids,0,sizeof(pstOpt->astGrids));
		
		if (get_script_input(pstCSDlgC->Dlg.Input, "Num", szNum, sizeof(szNum)) < 0) {
			//旧版本?
			if (get_script_input(pstCSDlgC->Dlg.Input, "ListType", szListType, sizeof(szListType)) < 0) {
				goto _error;
			}

			if (get_script_input(pstCSDlgC->Dlg.Input, "GridIdx", szGridIdx, sizeof(szGridIdx)) < 0) {
				goto _error;
			}
			pstOpt->iNum = 1;

			pstOpt->astGrids[0].iListType= atoi(szListType);
			pstOpt->astGrids[0].iGridIdx= atoi(szGridIdx);
		}
		else
		{
			char szListTypeKey[32];
			char szGridIdxKey[32];
			
			pstOpt->iNum = atoi(szNum);
			if (pstOpt->iNum < 0 || pstOpt->iNum > MAX_DIALOG_COMMIT_NUM)
			{
				goto _error;
			}
			
			for (i = 0; i < pstOpt->iNum ; i++)
			{
				snprintf(szListTypeKey,sizeof(szListTypeKey),"ListType_%d",i);
				snprintf(szGridIdxKey,sizeof(szGridIdxKey),"GridIdx_%d",i);
				
				if (get_script_input(pstCSDlgC->Dlg.Input, szListTypeKey, szListType, sizeof(szListType)) < 0) {
					goto _error;
				}

				if (get_script_input(pstCSDlgC->Dlg.Input, szGridIdxKey, szGridIdx, sizeof(szGridIdx)) < 0) {
					goto _error;
				}
				pstOpt->astGrids[i].iListType = atoi(szListType);
				pstOpt->astGrids[i].iGridIdx = atoi(szGridIdx);
			}
		}

		if ( pstOpt->iNum < 1 || pstOpt->iNum > MAX_DIALOG_COMMIT_NUM)
		{
			goto _error;
		}
		
		for (i = 0; i < pstOpt->iNum ; i++)
		{
			//get item
			if(0 > package_get_item(pstEnv, pstPlayer, pstOpt->astGrids[i].iListType, pstOpt->astGrids[i].iGridIdx, &pstItemDef, &pstRoleItem, &iPos))
			{
				goto _error;
			}
		}

		iProc = pstPlayer->stOnline.stSData.stCommitOpt.shProc;
		if(iProc <= 0) goto _error;

		if(!(pstMonDef->MiscType & MON_MISC_TYPE_NPC_MOVE))
		{
			pstNpc->stMon.tFreezeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + 10;
			if (pstNpc->stMon.cMoving)
			{
				z_mon_stop(pstEnv, &pstNpc->stMon); 
			}
		}
	}

	if (iFlag)
	{
		char cTmp;
		
		cTmp = pstPlayer->stOnline.cDialogStat;
		pstPlayer->stOnline.cDialogStat = pstPlayer->stOnline.cDialogStatOld;
		pstPlayer->stOnline.cDialogStatOld = cTmp;
	}

	stSEnv.pstDlgC = pstCSDlgC;
	stSEnv.pstPlayer = pstPlayer;
	stSEnv.pstMonster = NULL;
	stSEnv.pstPworldInst = NULL;
	stSEnv.pstMapinst = NULL;
	stSEnv.pstKiller = NULL;
	stSEnv.stDlgPkg.Body.DlgS.DlgStyleType = DLG_STYLE_NORMAL;

	if (pstNpc)
	{
		MONSTERDEF *pstMonDef;
		
		pstMonDef = z_get_mon_def(pstEnv, &pstNpc->stMon);

		if (pstEnv->pstConf->CountryVer > 0)
		{
			g_dartcar_allow = 1;
		}
		else
		{
			if (pstMonDef && 0 == strcmp(pstMonDef->Name, "传送门"))
			{
				g_dartcar_allow = 1;
			}
		}
	}
	z_exec_script(pstAppCtx, pstEnv, pstPlayer, pstNpc->szScriptName, &pstNpc->stScriptIdx, iProc);
	g_dartcar_allow = 0;

_final:

	return iRet;
_error:
	
	if (iFlag)
	{
		char cTmp;
		
		cTmp = pstPlayer->stOnline.cDialogStat;
		pstPlayer->stOnline.cDialogStat = pstPlayer->stOnline.cDialogStatOld;
		pstPlayer->stOnline.cDialogStatOld = cTmp;
	}

	if (iClose)
	{
		pstActionRes->ID =  pstPlayer->iMemID;
		pstActionRes->ActionID = ACTION_CLOSE_DIALOG;
		pstActionRes->Data.CloseDialog.Type = 0;
		
		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

		pstPlayer->stOnline.cDialogStatOld = pstPlayer->stOnline.cDialogStat;
		pstPlayer->stOnline.cDialogStat = DIALOG_STAT_BEGIN;
		pstPlayer->stOnline.State &= ~CS_STAT_DIALOG;
	}
	
	iRet = -1;
	goto _final;
		
	return 0;
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

int mon_view_player_check(ZONESVRENV* pstEnv, Monster *pstMon)
{
	int  iIndex, i;
	char cType;
	Player *pstVPlayer = NULL;
	TMEMBLOCK *pstMemBlock;
	
	for (i = 0; i < pstMon->iVPlayer;) 
	{
		iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstMon->aiVPlayer[i], &cType);
		if (iIndex < 0)
		{
			mon_press_delete(pstMon, pstMon->aiVPlayer[i]);
			
			if (bdelete_int(&pstMon->aiVPlayer[i], &pstMon->aiVPlayer, &pstMon->iVPlayer, sizeof(int)) == 0)i++;
			continue;
		}
		
		if (cType == OBJ_PLAYER)
		{
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				mon_press_delete(pstMon, pstMon->aiVPlayer[i]);
				
				if (bdelete_int(&pstMon->aiVPlayer[i], &pstMon->aiVPlayer, &pstMon->iVPlayer, sizeof(int)) == 0)i++;
				continue;
			}

			pstVPlayer = (Player *)pstMemBlock->szData;
			if ( !(pstVPlayer->stOnline.State & CS_STAT_DEAD) )
			{
				return 0;
			}
		
		}
		else
		{
			if (bdelete_int(&pstMon->aiVPlayer[i], &pstMon->aiVPlayer, &pstMon->iVPlayer, sizeof(int)) == 0) i++;
			continue;
		}
		
		i++;
	}

	return 1;
}

int team_all_die_destory_fb(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	//队伍全灭销毁FB
	ZONEPWORLDINST *pstPworldInst = NULL;
	ZONEPWORLD *pstZonePworld;
	Player *pstTeamMember;
	int i;
	ZONEMAPINST *pstMapInst; 
	ZONEIDX stIdx;
	
	if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
	{
		return -1;
	}
		
	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst)
	{
		return -1;
	}

	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (NULL == pstPworldInst )
	{
		return -1;
	}
	if(pstPworldInst->iType != PWORLD_TYPE_TEAM)
	{
		return -1;
	}
	
	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if (!pstZonePworld || !(pstZonePworld->stPworldDef.CtrlFlag & ALL_DIE_DESTORY))
	{
		return -1;
	}

	for (i=0; i<pstPworldInst->iPlayer; i++)
	{
		pstTeamMember = z_id_player(pstEnv->pstAppCtx, pstEnv, 
						pstPworldInst->aiPlayerID[i]);
		if (!pstTeamMember || pstTeamMember == pstPlayer)
		{
			continue;
		}

		if ( !(pstTeamMember->stOnline.State & CS_STAT_DEAD))
		{
			break;
		}
	}

	if (i >= pstPworldInst->iPlayer)
	{
		z_in_destroy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstPworldInst);
	}
	return 0;
}

static int player_die_punish_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	MapIndex *pstMapIndex;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	ZoneStatus *pstStauts;

	// 处理死亡赦免令
	pstStauts = z_player_find_status(pstPlayer, DIE_REMIT_BUFFID);
	if (pstStauts)
	{
		if (pstStauts->unOverlay == 1)
		{
			z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, DIE_REMIT_BUFFID, 1, 0);
		}
		else if (pstStauts->unOverlay > 1)
		{
			pstStauts->unOverlay --;
			z_player_send_status(pstEnv->pstAppCtx, pstEnv, pstPlayer );		
		}
		return 1;
	}

	stIdx.iID	 =	pstPlayer->stRoleData.Map;
	stIdx.iPos =	pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst( pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return 0;
	}
	
	pstMapIndex = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIndex)
	{
		return 0;
	}

	if(pstMapIndex->stMapDef.MapID == BASE_CITY_MAPID)
	{
		return 1;
	}

	if (pstMapIndex->stMapDef.NotDiePunish)
	{
		return 1;
	}
	
	return 0;
}

int is_both_lucky_attendee(Player* pstAtkee,Player* pstAtker)
{
	if ((pstAtkee->stOnline.cMiscFlag & MISC_FLAG_LUCK) &&
		(pstAtker->stOnline.cMiscFlag & MISC_FLAG_LUCK) )
	{
		return 1;
	}
	return 0;
}

int is_both_clan_spy(ZONESVRENV* pstEnv,Player* pstAtkee,Player* pstAtker)
{
	//不是在公会城市的都不算
	if (GET_MAPID(pstAtkee->stRoleData.Map) != CLAN_CITY_MAP_ID)
	{
		return 0;
	}

	if (pstAtkee->stRoleData.Map != pstAtker->stRoleData.Map)
	{
		return 0;
	}
	
	long long iAtkeeSpy = ((pstAtkee->stOnline.cMiscFlag & MISC_FLAG_SPY) && 
						pstAtkee->stRoleData.MiscInfo.DailyInfo.SpyInfo.TargetClanID) ||
					(pstAtkee->stOnline.cMiscFlag & MISC_FLAG_ANTI_SPY);

	long long iAtkerSpy = ((pstAtker->stOnline.cMiscFlag & MISC_FLAG_SPY) && 
						pstAtker->stRoleData.MiscInfo.DailyInfo.SpyInfo.TargetClanID) ||
					(pstAtker->stOnline.cMiscFlag & MISC_FLAG_ANTI_SPY);

 	if (iAtkerSpy && iAtkeeSpy)
	{
		return 1;
	}
	
	{
		ZONEPWORLDINST* pstPworldInst = NULL;
		pstPworldInst = z_find_player_pworld_inst(pstEnv,pstAtkee);

		if (pstPworldInst && pstPworldInst->iClanSpyFlag)
		{
			return 1;
		}
		
	}
	return 0;
}


int z_add_atker_pkval(ZONESVRENV* pstEnv, Player *pstAtkee, ZoneAni *pstAtker)
{	
	Player *pstAtkPlayer;
	MapIndex *pstMapIndex;
	int iPKVal;
	ZoneClan *pstAtkerClan = NULL;

	if(!pstAtker || !pstAtkee)
	{
		return 0;
	}

	//地图限制
	pstMapIndex = z_mappos_index(pstEnv->pstMapObj, NULL, pstAtkee->stRoleData.Map);
	if(NULL == pstMapIndex)
	{
		return 0;
	}

	if(pstMapIndex->stMapDef.CtrlFlag & MAP_CTRL_NO_PK_VAL)
	{
		return 0;
	}

	/*
	//攻城战时期，攻城战地图杀人不违法
	if(pstMapIndex->stMapDef.MapID == WORLD_CITY_MAPID &&
		is_in_siege_city_fast(pstEnv))
	{
		return 0;
	}
	*/

	//杀红名
	if(is_bad_palyer(pstAtkee) || is_in_pk_time_player(pstEnv, pstAtkee))
	{
		return 0;
	}
	
	if(pstAtker->iType == OBJ_MONSTER && is_mon_machine(pstAtker->stObj.pstMon))
	{
		pstAtkPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAtker->stObj.pstMon->iSkillAtkerID);
	}
	else
	{
		pstAtkPlayer = get_atker_player(pstEnv,  pstAtker);
	}

	if(NULL == pstAtkPlayer || pstAtkee == pstAtkPlayer)
	{
		return 0;
	}

	if(pstAtkee->stRoleData.Map == BASE_CITY_MAPID || 
		(pstAtkPlayer && pstAtkPlayer->stRoleData.Map == BASE_CITY_MAPID))
	{
		return 0;
	}

	if (z_is_qiecuo(pstEnv, pstAtkPlayer, pstAtkee))
	{
		return 0;
	}

	if(CLAN_CITY_MAP_ID == GET_MAPID(pstAtkPlayer->stRoleData.Map))
	{
		if(is_in_my_clan_city(pstEnv, pstAtkPlayer))
		{
			return 0;
		}
	}
	else
	{
		//非中立地图不加PK 值(不限主动制模式的且可PK 视为中立地图)
		if(pstMapIndex->stMapDef.MapLimit.PKMode != 0 || pstMapIndex->stMapDef.MapLimit.PK == 0)
		{
			return 0;
		}
	}

	pstAtkerClan = player_get_clan(pstEnv, pstAtkPlayer);
	//敌对公会之间不加PK值
	if(pstAtkerClan && clan_relation_by_player(pstEnv, pstAtkee, pstAtkerClan) < 0)
	{
		return 0;
	}

	if (is_both_lucky_attendee(pstAtkee,pstAtkPlayer))
	{
		return 0;
	}

	//刺探活动不增加PK值
	if (is_both_clan_spy(pstEnv, pstAtkee,pstAtkPlayer))
	{
		return 0;
	}
	
	iPKVal = KILL_PLAYER_ADD_PKVAL;

	player_pk_val_chg(pstEnv, pstAtkPlayer, iPKVal);
	
	if(pstAtkPlayer->stRoleData.MiscInfo.PkInfo.PkVal >= PLAYER_PK_FRENZY)
	{
		player_pk_time_clear(pstEnv, pstAtkPlayer);
	}

	return 0;
}

void player_die_task_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	TASKINFO *pstRoleTaskInfo = &pstPlayer->stRoleData.MiscInfo.TaskInfo;
	TASKRUN *pstTaskRun = &pstRoleTaskInfo->TaskRun;
	TASKDEF *pstTaskDef;
	int i;
	
	for (i=pstTaskRun->TaskRunNum-1; i>=0; i--)
	{
		pstTaskDef = task_def_find(pstEnv, pstTaskRun->TaskRuns[i].TaskID, &pstTaskRun->TaskRuns[i].TaskIdx);
		if(pstTaskDef && pstTaskRun->TaskRuns[i].TaskStat == TASK_STAT_RUN)
		{
			 if(pstTaskDef->TaskFlag & TASK_ROLE_DIE_FAIL)
			 {
				pstTaskRun->TaskRuns[i].TaskStat = TASK_STAT_FAIL;
				task_chg(pstEnv, pstPlayer, &pstTaskRun->TaskRuns[i]);
			 }
		}
	}
	
	return ;
}

int get_player_pkval_type(Player *pstPlayer)
{
	if(pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal >= PLAYER_PK_PUBLIC_ENEMY)
	{
		return PK_VAL_TYPE_BLACK;
	}
	else if(pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal >= PLAYER_PK_FRENZY)
	{
		return PK_VAL_TYPE_RED;
	}	
	else
	{
		return PK_VAL_TYPE_NORMAL;
	}
		
	return 0;
}

int get_pkval_type(int iVal)
{
	if(iVal >= PLAYER_PK_PUBLIC_ENEMY)
	{
		return PK_VAL_TYPE_BLACK;
	}
	else if(iVal >= PLAYER_PK_FRENZY)
	{
		return PK_VAL_TYPE_RED;
	}	
	else
	{
		return PK_VAL_TYPE_NORMAL;
	}
		
	return 0;
}

int die_drop_item(ZONESVRENV* pstEnv, Player *pstPlayer, DIEDROP *pstDrop, 
	DieDropGrid *pstDieDropGrid, int iDropNum, int iSec)
{
	int i = 0;
	ZoneBootyGrid stBootyGrid;
	CSPKG stPkg;
	CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
	ROLEPACKAGECHG *pstPackageChg = &pstPackageInfo->PackageInfoData.PackageChg;
	
	pstPackageInfo->Type = PACKAGE_INFO_CHG;
	pstPackageChg->ItemChg.ChgNum = 0;
	pstPackageChg->ArmChg.ChgNum = 0;

	rand_position_init();

	for(i = 0; i<iDropNum; i++)
	{
		switch(pstDieDropGrid[i].iListType)
		{
			case LIST_TYPE_WEAR:
				z_wear_del(pstEnv, pstPlayer, &pstDieDropGrid[i].stGrid.GridData.RoleArm, pstPackageChg);
				break;
			case LIST_TYPE_SUNDRIES:
				{
					int iPos;
					ITEMDEF *pstItemDef;
					ROLEITEM *pstRoleItem;
					
					if( 0 > package_get_item(pstEnv, pstPlayer, pstDieDropGrid[i].iListType, pstDieDropGrid[i].iGridIdx, 
								&pstItemDef, &pstRoleItem, &iPos))
					{
						continue;
					}
					
					if(0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstDieDropGrid[i].stGrid.GridData.RoleItem.DefID, 
							pstDieDropGrid[i].stGrid.GridData.RoleItem.Num, pstDieDropGrid[i].iListType, &iPos,PACKAGE_DEC_NONE, 0))
					{
						continue;
					}
				}
				break;
			default:
				continue;
				break;
		}

		//掉落显示
		pstDrop->Items[pstDrop->Count].ItemID = pstDieDropGrid[i].stGrid.GridData.RoleItem.DefID;
		pstDrop->Items[pstDrop->Count].Num = pstDieDropGrid[i].stGrid.GridData.RoleItem.Num;
		pstDrop->Count++;
			
		//掉落
		memset(&stBootyGrid, 0, sizeof(stBootyGrid));
		
		stBootyGrid.bViewType = BOOTY_DROP_VIEW_ALL;
		stBootyGrid.bCollectTime = iSec;
		
		memcpy(&stBootyGrid.stGrid, &pstDieDropGrid[i].stGrid, sizeof(stBootyGrid.stGrid));

		dropitem_create(pstEnv, &stBootyGrid, &pstPlayer->stRoleData.Pos, pstPlayer->stRoleData.Map, pstPlayer->iMemID);
	}

	
	if (pstPackageChg->ArmChg.ChgNum > 0)
	{
		Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

		z_update_avatar(pstEnv, pstPlayer);
	}

	return 0;
}


int player_spy_die_drop_item(ZONESVRENV* pstEnv, Player *pstPlayer, DIEDROP *pstDrop, 
							int iItemRate,Player *pstKiller)
{
	int i;
	ITEMDEF *pstItemDef;
	ZoneBootyGrid stBootyGrid;
	int iRand = 0;	
	ROLEGRID * pstRoleGrid = &stBootyGrid.stGrid;

	iRand = RAND1(10000);

	if (iRand >= iItemRate)
	{
		return 0;
	}

	//掉落

	memset(&stBootyGrid, 0, sizeof(stBootyGrid));
	
	stBootyGrid.bViewType = BOOTY_DROP_VIEW_ALL;
	stBootyGrid.bCollectTime = 0;

	for (i = 0; i < pstDrop->Count; i++)
	{
		
		
		//package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstDrop->Items[i].ItemID, 
							//pstDrop->Items[i].Num, LIST_TYPE_TASK, 0,PACKAGE_DEC_NONE, 0);		
		pstEnv->iRealItemDecNum = 1;
		if (pstEnv->iRealItemDecNum > 0)
		{
			pstItemDef = z_find_itemdef(pstEnv,  pstDrop->Items[i].ItemID);
			if (!pstItemDef)
			{
				continue;
			}
			
			if (0 > item_create(pstEnv->pstAppCtx, pstEnv, pstRoleGrid,  pstDrop->Items[i].ItemID))
			{
				continue;
			}

			pstRoleGrid->GridData.RoleItem.Num = pstEnv->iRealItemDecNum;
			pstRoleGrid->GridData.RoleItem.DefID = pstItemDef->ItemID;
			
			//package_add(pstEnv->pstAppCtx, pstEnv, pstKiller, &stRoleGrid, 1,0);

			dropitem_create(pstEnv, &stBootyGrid, &pstPlayer->stRoleData.Pos, pstPlayer->stRoleData.Map, 0);
			
		}
	}



	return 0;
	UNUSED(pstKiller);	//add by paraunused.pl
}

/*
	角色死亡掉落规则
	1.封印的加上掉落的最多3个
	2.封印的最多1个
*/

int player_die_drop_item(ZONESVRENV* pstEnv, Player *pstPlayer, DIEDROP *pstDrop, 
							int iWearRate,int iItemRate,Player *pstKiller, FENGYINARM *pstFengYinArm)
{
	int i;
	int iDropFlag = 0;
	int iDropNum = 0, iCount = 0;
	DieDropGrid astDieDropGrid[DIE_DROP_ITEM_NUM];
	unsigned char sMap[MAX_SUNDRIES_PACKAGE + MAX_WEAR];
	ROLEPACKAGE *pstPackage = &pstPlayer->stRoleData.Package;
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	ITEMDEF *pstItemDef;
	OPROLEDIEDROP stDrop;
	int iFengyin = 0;	//封印装备

	//找身上
	for(i = 0; i<pstRoleWear->ArmNum; i++)
	{
		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)&pstRoleWear->RoleArms[i]);
		if(NULL == pstItemDef)
		{
			continue;
		}

		if(arm_is_fashion(pstItemDef))
		{
			continue;
		}
		
		if(pstItemDef->DieDropFlag == DIE_DROP_TYPE_DROP)
		{
			if(iDropNum+iFengyin >= 1 /*DIE_DROP_ITEM_NUM*/)
			{
				break;
			}

			// 必掉的 肯定不会是绑定的
			if (pstRoleWear->RoleArms[i].InstFlag & INST_ITEM_BIND)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, 
							"LIST_TYPE_WEAR:arm inst bind and pstItemDef->DieDropFlag == DIE_DROP_TYPE_DROP");
				continue;
			}
			
			astDieDropGrid[iDropNum].iListType = LIST_TYPE_WEAR;
			astDieDropGrid[iDropNum].iGridIdx = pstRoleWear->RoleArms[i].GridIdx;
			astDieDropGrid[iDropNum].stGrid.Type = GRID_ITEM_ARM;
			astDieDropGrid[iDropNum].stGrid.GridData.RoleArm = pstRoleWear->RoleArms[i];
			iDropNum++;
		}
		else
		{
			if (pstEnv->pstConf->CountryVer == COUNTRY_VER_KOREA)
			{
				if (pstRoleWear->RoleArms[i].InstFlag & INST_ITEM_BIND)
				{
					continue;
				}
			}
			sMap[iCount++] = i;
		}
	}

	//掉落身上的
	if(	iCount > 0 &&
		iDropNum+iFengyin < 1 /*DIE_DROP_ITEM_NUM*/ && 
		RAND1(100) < iWearRate)
	{
		int iIdx;

		iDropFlag = 1;
		iIdx = RAND1(iCount);

		if (	pstRoleWear->RoleArms[sMap[iIdx]].InstFlag&INST_ITEM_BIND)
		{
			if (iFengyin < MAX_FENGYIN_ARM)
			{
				// todo 封印
				if (0 == arm_fengyin_in(pstEnv, pstPlayer, pstRoleWear->RoleArms[sMap[iIdx]].GridIdx, 
										pstKiller,LIST_TYPE_WEAR))
				{
					iFengyin++;
					if (pstFengYinArm->Num < MAX_FENGYIN_ARM)
					{
						pstFengYinArm->Items[pstFengYinArm->Num++] = 
										pstRoleWear->RoleArms[sMap[iIdx]].DefID;
					}
				}
			}
		}
		else
		{
			astDieDropGrid[iDropNum].iListType = LIST_TYPE_WEAR;
			astDieDropGrid[iDropNum].iGridIdx  = pstRoleWear->RoleArms[sMap[iIdx]].GridIdx;
			astDieDropGrid[iDropNum].stGrid.Type = GRID_ITEM_ARM;
			astDieDropGrid[iDropNum].stGrid.GridData.RoleArm = pstRoleWear->RoleArms[sMap[iIdx]];
			iDropNum++;
		}
		
	}

	iCount = 0;
	//杂物包
	if(iDropNum+iFengyin < 1 /*DIE_DROP_ITEM_NUM*/)
	{
		ROLEGRID *pstGrid;
		SUNDRIESPACKAGE *pstSundries = &pstPackage->SundriesPackage;
		
		for(i = 0; i<pstSundries->Num; i++)
		{
			pstGrid = &pstSundries->RoleGrids[i];
			
			pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstGrid->GridData.RoleItem);
			if(NULL == pstItemDef)
			{
				continue;
			}

			if(arm_is_fashion(pstItemDef))
			{
				continue;
			}
			
			if(pstItemDef->DieDropFlag == DIE_DROP_TYPE_DROP)
			{
				if(iDropNum+iFengyin >= 1 /*DIE_DROP_ITEM_NUM*/ )
				{
					break;
				}

				// 必掉的 肯定不会是绑定的
				if (pstGrid->GridData.RoleItem.InstFlag & INST_ITEM_BIND)
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, 
								"LIST_TYPE_SUNDRIES:arm inst bind and pstItemDef->DieDropFlag == DIE_DROP_TYPE_DROP");
					continue;
				}
				
				astDieDropGrid[iDropNum].iListType = LIST_TYPE_SUNDRIES;
				astDieDropGrid[iDropNum].iGridIdx = pstGrid->GridData.RoleItem.GridIdx;
				astDieDropGrid[iDropNum].stGrid = *pstGrid;
				iDropNum++;
			}
			else
			{
				// 绑定的item不掉
				if (	(pstGrid->GridData.RoleItem.InstFlag & INST_ITEM_BIND) &&
					pstGrid->Type == GRID_ITEM_ITEM)
				{
					continue;
				}

				if (pstEnv->pstConf->CountryVer == COUNTRY_VER_KOREA)
				{
					if (pstGrid->Type == GRID_ITEM_ARM && (pstGrid->GridData.RoleArm.InstFlag & INST_ITEM_BIND))
					{
						continue;
					}
				}

				sMap[iCount++] = i;
			}
		}
	}

	//掉背包一个格子
	if(	iCount > 0 && 
		iDropFlag == 0 &&
		iDropNum+iFengyin < 1 /*DIE_DROP_ITEM_NUM*/ && 
		RAND1(100) < iItemRate)
	{
		int iIdx;
		ROLEGRID*pstGrid;
		
		iIdx = RAND1(iCount);
		pstGrid = &pstPackage->SundriesPackage.RoleGrids[sMap[iIdx]];
		if (	pstGrid->GridData.RoleItem.InstFlag&INST_ITEM_BIND)
		{
			if (iFengyin < MAX_FENGYIN_ARM && pstGrid->Type == GRID_ITEM_ARM)
			{
				if (0 == arm_fengyin_in(pstEnv, pstPlayer, pstGrid->GridData.RoleItem.GridIdx, pstKiller,LIST_TYPE_SUNDRIES))
				{
					iFengyin ++;	
					if (pstFengYinArm->Num < MAX_FENGYIN_ARM)
					{
						pstFengYinArm->Items[pstFengYinArm->Num++] = 
												pstGrid->GridData.RoleItem.DefID;
					}
				}
			}
		}
		else
		{
			astDieDropGrid[iDropNum].iListType = LIST_TYPE_SUNDRIES;
			astDieDropGrid[iDropNum].iGridIdx = pstPackage->SundriesPackage.RoleGrids[sMap[iIdx]].GridData.RoleItem.GridIdx;
			astDieDropGrid[iDropNum].stGrid = pstPackage->SundriesPackage.RoleGrids[sMap[iIdx]];
			iDropNum++;
		}
		
	}

	if(iDropNum == 0)
	{
		return 0;
	}

	// ----------死亡掉落日志  start --------------
	stDrop.DropNum = 0;
	for (i=0; i<iDropNum; i++)
	{
		if (stDrop.DropNum >= DIE_DROP_ITEM_NUM) 
			break;
		stDrop.DropItem[stDrop.DropNum++]  = astDieDropGrid[i].stGrid;
	}
	
	stDrop.Uin = pstPlayer->stRoleData.Uin;
	stDrop.RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
	STRNCPY(stDrop.RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, sizeof(stDrop.RoleInfo.RoleName));
	STRNCPY(stDrop.LoginName, pstPlayer->szAccount, sizeof(stDrop.LoginName));
	z_role_die_drop_oplog(pstEnv->pstAppCtx, pstEnv, pstPlayer,&stDrop);
	//-------- 死亡掉落日志  end-------------

	die_drop_item(pstEnv, pstPlayer, pstDrop, astDieDropGrid, iDropNum, 5);
	
	return 0;
}

int player_die_drop(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstAtker, 
					DIEDROP *pstDrop,FENGYINARM *pstFengYinArm)
{
	int iMoney = 0;
	DIEPUNISHDEF *pstDiePunishDef;
	LEVELUP *pstLevelUp;
	long long uExp = pstPlayer->stRoleData.RoleDetail.Exp;
	int iDropExpMul;
	int iDropMoneyMul;
	int iDropArmDurMul;
	int iDropItemRate;
	int iDropWearRate;


	if (pstAtker && is_both_lucky_attendee(pstPlayer, pstAtker))
	{
		return 0;//宏福活动死亡不惩罚
	}

	if (pstAtker && is_both_clan_spy(pstEnv, pstPlayer, pstAtker))
	{
		//刺探活动中的相关惩罚
		int iAwardID = 0;
		CLANSPYAWARDDROPDEF *pstAward = NULL;
		/*if ((pstAtker->stOnline.cMiscFlag & MISC_FLAG_SPY) &&
			pstAtker->stRoleData.MiscInfo.DailyInfo.SpyInfo.TargetClanID)*/
		if ((pstAtker->stOnline.cMiscFlag & MISC_FLAG_SPY) &&
			pstAtker->stRoleData.MiscInfo.DailyInfo.SpyInfo.TargetClanID)
		{
			if ((pstPlayer->stOnline.cMiscFlag & MISC_FLAG_SPY) &&
				pstPlayer->stRoleData.MiscInfo.DailyInfo.SpyInfo.TargetClanID)				
			{
				iAwardID = SPY_KILL_SPY;
			}
			else if (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_ANTI_SPY)
			{
				iAwardID = SPY_KILL_ANTI;
			}
		}

		if (pstAtker->stOnline.cMiscFlag & MISC_FLAG_ANTI_SPY) 
		{
			if ((pstPlayer->stOnline.cMiscFlag & MISC_FLAG_SPY) &&
				pstPlayer->stRoleData.MiscInfo.DailyInfo.SpyInfo.TargetClanID)
			{
				iAwardID = ANTI_KILL_SPY;
			}
			else if (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_ANTI_SPY)
			{
				iAwardID = ANTI_KILL_ANTI;
			}
		}

		if (iAwardID)
		{
			pstAward = z_find_clan_spy_award_drop_def(pstEnv,iAwardID);
		}

		if (pstAward)
		{
			if (pstAward->AwardLimit > 
				pstAtker->stRoleData.MiscInfo.DailyInfo.SpyInfo.AwardNum)
			{
				if (pstAward->AwardContri > 0)
				{
					ZoneClan* pstZoneClan = player_get_clan(pstEnv,pstAtker);
					if (pstZoneClan)
					{
						g_iOptype = OP_OPTYPE_CITAN;
						g_iOpsubtype = pstAward->DefID;
						clan_resource_inc(pstEnv, pstZoneClan, pstAtker,
								 CLAN_OWNED_CONTRIBUT, pstAward->AwardContri);
						g_iOptype = 0;
						g_iOpsubtype = 0;
					}
				}
				if (pstAward->AwardMoney > 0)
				{
					package_add_money(pstEnv, pstAtker, ATTR_ID_BULL, pstAward->AwardMoney);
					z_money_create_oplog(pstEnv, pstAtker, MONEY_OPTYPE_CITAN, pstAward->DefID, pstAward->AwardMoney, 0, "刺探");					
				}
				if (pstAward->AwardContri > 0 || pstAward->AwardMoney > 0)
				{
					pstAtker->stRoleData.MiscInfo.DailyInfo.SpyInfo.AwardNum++;
				}
			}

			if (pstAtker && pstAward->DropItemID && pstAward->DropItemNum && 
				pstAward->DropItemRate && 
				!(pstPlayer->stOnline.cMiscFlag & MISC_FLAG_SPY_FINI))
			{
				int iNum = pstAward->DropItemNum;
				DIEDROP stDrop;

				memset(&stDrop,0,sizeof(stDrop));
				
				if (-1 == pstAward->DropItemNum)
				{
					iNum = 10000;
				}

				stDrop.Items[0].ItemID = pstAward->DropItemID ;
				stDrop.Items[0].Num = iNum;
				stDrop.Count++;

				player_spy_die_drop_item(pstEnv,pstPlayer, &stDrop, pstAward->DropItemRate,pstAtker);
			}
		}
		
		return 0;
	}

	pstDiePunishDef = z_find_diepunish_def(pstEnv, get_player_pkval_type(pstPlayer));
	if(NULL == pstDiePunishDef)
	{
		return 0;
	}

	if(pstAtker)
	{
		iDropExpMul = pstDiePunishDef->PKDropExp;
		iDropMoneyMul = pstDiePunishDef->PKDropMoney;
		iDropArmDurMul = pstDiePunishDef->PKDropArmDur;
		iDropItemRate = pstDiePunishDef->PKDropItemRate;
		iDropWearRate = pstDiePunishDef->PKDropWearRate;
	}
	else
	{	
		iDropExpMul = pstDiePunishDef->DropExp;
		iDropMoneyMul = pstDiePunishDef->DropMoney;
		iDropArmDurMul = pstDiePunishDef->DropArmDur;
		iDropItemRate = pstDiePunishDef->DropItemRate;
		iDropWearRate = pstDiePunishDef->DropWearRate;
	}

	// 死亡装备耐久惩罚
	if(iDropArmDurMul > 0) 
	{
		player_die_wear(pstEnv->pstAppCtx, pstEnv, pstPlayer, iDropArmDurMul);
	}
	pstDrop->ArmEndure = iDropArmDurMul;

	//只掉耐久
	if(pstPlayer->stRoleData.Level < DIE_NO_PUNISH_LEVEL)
	{
		return 0;
	}

	// 死亡经验惩罚
		
	pstLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	if (pstLevelUp && iDropExpMul > 0)
	{
		// 如果经验到了最小值了   减不下去了
		int iDecExp = pstLevelUp->LevelNextExp * (iDropExpMul / 100.0) ;
		if (uExp < iDecExp)
		{
			iDecExp = uExp;
		}

		if (iDecExp > 0)
		{
			//宠物天赋,玩家死亡惩罚
			pet_native_skill_result(pstEnv,pstPlayer,RESULT_NATIVE_OWNER_DIE_EXP_MAP, &iDecExp,NULL);
			player_dec_exp(pstEnv, pstPlayer, iDecExp);
		}
		
		pstDrop->Exp = iDecExp;
	}
	


	//银币
	if(iDropMoneyMul > 0)
	{
		iMoney = pstPlayer->stRoleData.RoleDetail.Money* (iDropMoneyMul /100.0);
		if (iMoney == 0 && pstPlayer->stRoleData.RoleDetail.Money> 0)
		{
			iMoney = 1;
		}
		
		if(iMoney > 0)
		{
			//宠物天赋,玩家死亡惩罚
			pet_native_skill_result(pstEnv,pstPlayer,RESULT_NATIVE_OWNER_DIE_MONEY_MAP, &iMoney,NULL);
			
			package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, iMoney);
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_PK, 0,  0, iMoney, "PK");

			pstDrop->Money= iMoney;

			if (pstEnv->pstConf->CountryVer != COUNTRY_VER_KOREA)
			{
				//击杀者获得50%
				if(pstAtker)
				{
					iMoney = iMoney * 50.0/100;
					if (iMoney == 0)
						iMoney = 1;
					package_add_money(pstEnv, pstAtker, ATTR_ID_MONEY, iMoney);
					z_money_create_oplog(pstEnv, pstAtker, MONEY_OPTYPE_PK, 0,  0, iMoney, "PK");
				}
			}
		}
	}
	
	//物品
	if(pstPlayer->stRoleData.Level >= 60 && ( iDropItemRate > 0 || iDropWearRate > 0))
	{
		if(pstAtker)
		{
			player_die_drop_item(pstEnv, pstPlayer, pstDrop, iDropWearRate, 
									iDropItemRate,pstAtker,pstFengYinArm);
		}
		else
		{
			player_die_drop_item(pstEnv, pstPlayer, pstDrop, iDropWearRate, 
									iDropItemRate,NULL,pstFengYinArm);
		}
	}
	
	return 0;
}

int player_die_revive_data(ZONESVRENV* pstEnv, Player *pstPlayer, DIESVR *pstDieSvr)
{
	tdr_ulonglong iMoney = 0;
	REVIVEDEF *pstReviveDef;
	GLOBAGOLDRATE *pstGoldRate = &pstEnv->pstGlobalObj->stGlobal.GoldRate;
	REVIVEINFO *pstReviveInfo = &pstPlayer->stRoleData.MiscInfo.ReviveInfo;

	//免费赠送次数
	if(!IsSameDay(pstReviveInfo->DayTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstReviveInfo->DayReviveNum = 0;
		pstReviveInfo->PerfectReviveMoney = 0;
		pstReviveInfo->DayPerfectReviveNum = 0;
		pstReviveInfo->DayTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	//两小时重置
	if(pstReviveInfo->PerfectTime + 7200 < pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		pstReviveInfo->PerfectReviveNum = 0;
		pstReviveInfo->ReviveNum = 0;
		pstReviveInfo->PerfectReviveMoney = 0;
	}

	pstReviveInfo->PerfectReviveMoney = 0;
	pstReviveInfo->PerfectTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstDieSvr->Revive.ReviveNum = pstReviveInfo->ReviveNum;
	pstDieSvr->Revive.PerfectReviveNum = pstReviveInfo->PerfectReviveNum;
	pstDieSvr->Revive.DayReviveNum = pstReviveInfo->DayReviveNum;
	pstDieSvr->Revive.DayPerfectReviveNum = pstReviveInfo->DayPerfectReviveNum;
	pstDieSvr->Revive.PerfectReviveMoney = 0;

	pstReviveDef = z_find_revive_def(pstEnv, pstPlayer->stRoleData.Level);
	if(!pstReviveDef)
	{
		return -1;
	}

	if(pstReviveInfo->DayPerfectReviveNum >= pstReviveDef->DayPerfectReviveTimes)
	{	
		if(pstReviveInfo->PerfectReviveNum+1 <= pstReviveDef->PerfectReviveTimes)
		{
			iMoney = pstReviveDef->PerfectRevive.MoneyVal;
		}
		else
		{
			if(pstReviveInfo->PerfectReviveNum+1 <= pstReviveDef->PerfectIncreaseNum)
			{
				iMoney = pstReviveDef->PerfectRevive.MoneyVal+
					pstReviveDef->IncreaseVal*(pstReviveInfo->PerfectReviveNum+1-pstReviveDef->PerfectReviveTimes);
			}
			else
			{
				iMoney = pstReviveDef->PerfectRevive.MoneyVal+
					pstReviveDef->IncreaseVal*(pstReviveInfo->PerfectReviveNum+1-pstReviveDef->PerfectReviveTimes);
				iMoney = iMoney*
					pow(pstReviveDef->PerfectIncreaseVal2, pstReviveInfo->PerfectReviveNum+1-pstReviveDef->PerfectIncreaseNum);

				if(pstReviveInfo->PerfectReviveNum+1 >= pstReviveDef->PerfectFixedNum)
				{
					tdr_ulonglong ullSysMoney;

					if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
					{
						ullSysMoney = pstReviveDef->PerfectFixedMul*pstGoldRate->GoldRate;
					}
					else
					{
						ullSysMoney = pstReviveDef->PerfectFixedMul*pstPlayer->stOnline.iSpanGoldRate;
					}
					
					if(iMoney > ullSysMoney)
					{
						iMoney = ullSysMoney;
					}
				}
			}
		}

		if(iMoney > 0x7FFFFFFF)
		{
			iMoney = 0x7FFFFFFF;
		}

		pstReviveInfo->PerfectReviveMoney = iMoney;
		pstDieSvr->Revive.PerfectReviveMoney = pstReviveInfo->PerfectReviveMoney;
	}
	
	return 0;
}

int player_login_die_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	DIESVR *pstDieSvr = &stPkg.Body.DieSvr;
	MISCDIEINFO *pstDieInfo = &pstPlayer->stRoleData.MiscInfo.DieInfo;
	Player *pstPlayerKill;

	memset(pstDieSvr, 0, sizeof(*pstDieSvr));
	
	STRNCPY(pstDieSvr->KillerName, pstDieInfo->KillerName, sizeof(pstDieSvr->KillerName));
	pstDieSvr->End = pstDieInfo->End;
	pstDieSvr->DieDrop = pstDieInfo->DieDrop;
	player_die_revive_data(pstEnv, pstPlayer, pstDieSvr);

	
	pstPlayerKill = z_name_player(pstEnv->pstAppCtx,  pstEnv, pstDieInfo->KillerName);
	if (pstPlayerKill)
	{
		pstDieSvr->DaoBaInfo = pstPlayer->stRoleData.MiscInfo.DaoBaInfo;
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, DIE_INFO_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int get_pworld_autorevive_time(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iAutoTime = 0;
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;
	ZONEPWORLDINST* pstPworldInst = NULL;
	ZONEPWORLD *pstZonePworld;
	BATTLEDEF *pstBattleDef;
	
	if(0 == in_auto_revive_pworld(pstEnv, pstPlayer))
	{
		return 0;
	}

	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos =	pstPlayer->stOnline.iMapPos;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
		return 0;


	pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if(NULL == pstPworldInst)
		return 0;

	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if (NULL == pstZonePworld)
	{
		return 0;
	}

	iAutoTime = 0;
	if(pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_DOUBLE_MONSTER)
	{
		iAutoTime = 3;
	}
	else if(pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_MACHINE_WAR)
	{
		iAutoTime = 3;
	}
	else if(pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_SPARTA)
	{
		iAutoTime = 3;
	}
	else if (pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_TANK)
	{
		iAutoTime = 3;
	}
	else
	{
		pstBattleDef = z_find_battledef(pstEnv, pstZonePworld->stPworldDef.BattleID);
		if(NULL == pstBattleDef)
		{
			return 0;
		}

		if(pstBattleDef->FightType == BATTLE_FIGHT_TYPE_XUKONG)
		{
			if(pstEnv->pstAppCtx->stCurr.tv_sec >= pstPworldInst->tStart)
			{
				iAutoTime = 20 - (pstEnv->pstAppCtx->stCurr.tv_sec - pstPworldInst->tStart)%20;
			}
		}
		else if(pstBattleDef->FightType == BATTLE_FIGHT_TYPE_CLAN_WAR)
		{
			if(pstEnv->pstAppCtx->stCurr.tv_sec >= pstPworldInst->tStart)
			{
				//自动复活时间为15秒
				iAutoTime = 15 - (pstEnv->pstAppCtx->stCurr.tv_sec - pstPworldInst->tStart)%15;
			}
		}
		else
		{
			if(pstEnv->pstAppCtx->stCurr.tv_sec >= pstPworldInst->tStart)
			{
				iAutoTime = 30 - (pstEnv->pstAppCtx->stCurr.tv_sec - pstPworldInst->tStart)%30;
			}
		}
	}
		
	return iAutoTime;
}

int is_nixi_revive_player(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZONEPWORLD *pstZonePworld;
	int iMapID = GET_MAPID(pstPlayer->stRoleData.Map);
	
	pstZonePworld = z_find_pworld2(pstEnv, pstPlayer->stRoleData.Map);
	
	if(pstPlayer->stOnline.iDieNum > 0 &&
		pstPlayer->stOnline.iDieNum % 10 == 0)
	{
		if(iMapID == WORLD_CITY_MAPID && is_in_siege_city_fast(pstEnv))
		{
			return 1;
		}

		if(pstZonePworld && is_battle_pworld(pstZonePworld))
		{
			return 1;
		}
		
		if(strong_player_is_in_pworld(pstEnv, pstPlayer))
		{
			return 1;
		}
	}
	
	return 0;
}

int player_die_punish(ZONESVRENV* pstEnv, Player *pstPlayer,ZoneAni *pstAtker, int iDaoba)
{
	CSPKG stPkg;
	DIESVR *pstDieSvr = &stPkg.Body.DieSvr;
	Player *pstKiller = NULL;
	MISCDIEINFO *pstDieInfo = &pstPlayer->stRoleData.MiscInfo.DieInfo;

	memset(pstDieSvr, 0, sizeof(*pstDieSvr));
	memset(pstDieInfo, 0, sizeof(*pstDieInfo));

	pstPlayer->stOnline.iDieNum++;

	//击杀人
	pstKiller = get_atker_player(pstEnv,  pstAtker);
	if(pstKiller)
	{
		STRNCPY(pstDieSvr->KillerName, pstKiller->stRoleData.RoleName, sizeof(pstDieSvr->KillerName));

		pstKiller->stOnline.iDieNum = 0;

		player_die_drop_app( pstEnv, pstPlayer, pstKiller, &pstDieSvr->DieDrop);
	}
	//非人即怪
	else
	{
		MONSTERDEF *pstMonDef;
		
		if(pstAtker->iType == OBJ_MONSTER)
		{
			pstMonDef =  z_get_mon_def(pstEnv, pstAtker->stObj.pstMon);
			STRNCPY(pstDieSvr->KillerName, pstMonDef->Name, sizeof(pstDieSvr->KillerName));
		}
		else
		{
			//离奇死亡。。。
		}
	}
	strncpy(pstDieInfo->KillerName,pstDieSvr->KillerName,sizeof(pstDieInfo->KillerName));

	if (WORLD_CITY_MAPID == pstPlayer->stRoleData.Map)
	{
		pstDieSvr->End = pstDieInfo->End = pstEnv->pstAppCtx->stCurr.tv_sec+30;
	}
	else
	{	
		pstDieSvr->End = pstDieInfo->End = pstEnv->pstAppCtx->stCurr.tv_sec+5*60;
	}
	
	
	if (player_die_punish_check(pstEnv, pstPlayer) == 0)
	{
		player_die_drop(pstEnv, pstPlayer, pstKiller, &pstDieSvr->DieDrop, &pstDieSvr->FengYinArm);
	}
	
	pstDieSvr->AutoReviveTime = get_pworld_autorevive_time(pstEnv, pstPlayer);
	if(pstDieSvr->AutoReviveTime > 0)
	{
		pstPlayer->stOnline.tAutoReviveTime = pstEnv->pstAppCtx->stCurr.tv_sec + pstDieSvr->AutoReviveTime;
	}

	if(is_nixi_revive_player(pstEnv, pstPlayer))
	{
		pstDieSvr->NiXi = 1;
	}

	player_die_revive_data(pstEnv, pstPlayer, pstDieSvr);
	if (iDaoba && pstKiller)
	{
		pstDieSvr->DaoBaInfo = pstPlayer->stRoleData.MiscInfo.DaoBaInfo;	
	}

	if (CS_STAT_DEAD & pstPlayer->stOnline.State)
	{
		Z_CSHEAD_INIT(&stPkg.Head, DIE_INFO_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	pstDieInfo->DieDrop = pstDieSvr->DieDrop;
	return 0;
}

int mon_kill_player_notify(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneAni *pstAtker)
{
	Monster *pstMon;
	
	if(pstAtker->iType != OBJ_MONSTER)
	{
		return -1;
	}

	pstMon = pstAtker->stObj.pstMon;
	pstMon->iKillPlayerNum++;
	
	if(pstMon->iDefIdx == 207001 ||
		pstMon->iDefIdx == 200030 ||
		pstMon->iDefIdx == 230013)
	{
		MONSTERDEF *pstMonDef;
		
		pstMonDef = z_get_mon_def(pstEnv, pstMon);
		if(!pstMonDef)
		{
			return -1;
		}
		
		switch(pstMon->iKillPlayerNum)
		{
			case 10:
			//case 1:
				z_sys_strf_broadcast(pstEnv, SYS_SVR, 
					ZONEERR_BROAD333, 
					pstPlayer->stRoleData.RoleName, pstMonDef->Name);
				break;
			case 50:
			//case 2:
				z_sys_strf_broadcast(pstEnv, SYS_SVR, 
					ZONEERR_BROAD334, 
					pstMonDef->Name);
				break;
			case 100:
			//case 3:
				z_sys_strf_broadcast(pstEnv, SYS_SVR, 
					ZONEERR_BROAD335, 
					pstMonDef->Name);
				break;
			case 200:
			//case 4:
				z_sys_strf_broadcast(pstEnv, SYS_SVR, 
					ZONEERR_BROAD336, 
					pstMonDef->Name, pstPlayer->stRoleData.RoleName);
				break;
			case 300:
			//case 5:
				z_sys_strf_broadcast(pstEnv, SYS_SVR, 
					ZONEERR_BROAD337, 
					pstMonDef->Name);
				break;
			case 500:
			//case 6:
				z_sys_strf_broadcast(pstEnv, SYS_SVR, 
					ZONEERR_BROAD338, 
					pstMonDef->Name);
				break;
			default:
				break;
		}
	}

	return 0;
}

int player_die_drop_app( ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstKiller, DIEDROP *pstDrop)
{
	ZONEMAPINST* pstMapInst;
	MapIndex* pstMapIdx;
	int iMap = GET_MAPID(pstPlayer->stRoleData.Map);
	ZONEIDX stIdx;
	SUNDRIESPACKAGE *pstSund = &pstPlayer->stRoleData.Package.SundriesPackage;
	DieDropGrid stDropGrid;
	int i;
	
	if (!pstKiller)
	{
		return 0;
	}

	stIdx.iID = iMap;
	stIdx.iPos = -1;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return 0;
	}

	pstMapIdx =  z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return 0;
	}

	if (pstMapIdx->stMapDef.AppDropItemID <= 0)
	{
		return 0;
	}

	for (i=0; i<pstSund->Num; i++)
	{
		if (pstSund->RoleGrids[i].GridData.RoleItem.DefID == pstMapIdx->stMapDef.AppDropItemID &&
			!(pstSund->RoleGrids[i].GridData.RoleItem.InstFlag&INST_ITEM_BIND))
		{
			stDropGrid.iListType = LIST_TYPE_SUNDRIES;
			stDropGrid.iGridIdx = pstSund->RoleGrids[i].GridData.RoleItem.GridIdx;
			stDropGrid.stGrid = pstSund->RoleGrids[i];
			stDropGrid.stGrid.GridData.RoleItem.Num = 1;
			break;
		}
	}

	if (i>=pstSund->Num)
		return 0;

	die_drop_item(pstEnv, pstPlayer, pstDrop, &stDropGrid, 1,1);
	
	//package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,  pstMapIdx->stMapDef.AppDropItemID,
	//			1, LIST_TYPE_SUNDRIES, NULL,PACKAGE_DEC_UNBIND, 0);
	return 0;
}

int player_die(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ZoneAni *pstAtker, int iNotify)
{
	CSPKG stPkg;
	PlayerChagAttrVal stChgVal;
	ZONEPWORLDINST *pstPworldInst;
	CSACTIONRES *pstActRes = &stPkg.Body.ActionRes;
	Player *pstAtkPlayer = NULL;	
	Player *pstQiecuo;
	int iNixi = 0;
	int iNewDaoba = 0;

	pstAtkPlayer = get_atker_player(pstEnv,  pstAtker);

	z_oplog_event_begin(pstEnv);
	
	if (pstPlayer->stOnline.cMoving) 
	{
		z_player_move_stop(pstAppCtx, pstEnv,  pstPlayer);
	}

	if (pstPlayer->stOnline.State & CS_STAT_FIGHT)
	{
		memset(pstPlayer->stOnline.abFollowAngel, 0, sizeof(pstPlayer->stOnline.abFollowAngel));
		SET_PLAYER_NOFIGHT(pstPlayer);
		z_send_player_fight(pstEnv, pstPlayer, CLEAN_FIGHT_STATUS);
	}
	
	pstPlayer->stOnline.State &= ~(CS_STAT_WALK|CS_STAT_SIT|CS_STAT_ATK|CS_STAT_MOVE_ATK|CS_STAT_PREPARE|
									CS_STAT_MOVE_PREPARE|CS_STAT_FIGHT );	
	pstPlayer->stOnline.State |= CS_STAT_DEAD;

	pstPlayer->stOnline.iKillerObjType = OBJ_UNKNOW;
	pstPlayer->stOnline.iKillerMemID = 0;

	if(pstPlayer->stOnline.stQiecuoInfo.iMemID > 0)
	{
		pstQiecuo = z_id_player(pstAppCtx, pstEnv, pstPlayer->stOnline.stQiecuoInfo.iMemID);
		if (pstQiecuo)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "qiecuo err:skillid=%d ", pstQiecuo->stOnline.stAtkInfo.unSkillID);
		}
	}
	
	if (pstAtker)
	{
		Player* pstPlayerKiller = NULL;
		
		if(pstAtker->iType == OBJ_MONSTER && is_mon_machine(pstAtker->stObj.pstMon))
		{
			pstPlayerKiller = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAtker->stObj.pstMon->iSkillAtkerID);
		}
		else
		{
			pstPlayerKiller = get_atker_player(pstEnv,  pstAtker);
		}

		if (pstPlayerKiller)
		{
			pstPlayer->stOnline.iKillerObjType = OBJ_PLAYER;
			pstPlayer->stOnline.iKillerMemID = pstPlayerKiller->iMemID;
			pstPlayer->stOnline.stDieClean.cDieClean = 1;
			pstPlayer->stOnline.stDieClean.iMemID = pstPlayerKiller->iMemID;
		}
		else
		{
			if (pstAtker->iType == OBJ_MONSTER)
			{
				pstPlayer->stOnline.iKillerObjType = OBJ_MONSTER;
				pstPlayer->stOnline.iKillerMemID = pstAtker->stObj.pstMon->iID;
				
				// 称号
				player_desig_get(pstEnv, pstPlayer, GET_DESIG_MON_KILL); 
			}
		}
	}

	strong_player_die(pstEnv, pstPlayer, pstAtkPlayer, &iNixi);
	span_strong_player_die(pstEnv, pstPlayer, pstAtkPlayer);
	player_die_tip_by_saprte_range(pstEnv, pstPlayer, pstAtkPlayer);

	iNewDaoba = player_die_up_daoba(pstEnv, pstPlayer, pstAtkPlayer);

	player_team_follow_cancel_svr(pstEnv, pstPlayer);
	
	team_all_die_destory_fb(pstEnv, pstPlayer);

	player_trace_act_break_prepare(pstEnv, pstPlayer, BRK_PRE_ACT_DIE);
	
	tlog_debug(pstEnv->pstLogCat, 0, 0, "Vanimate num %d", pstPlayer->stOnline.iVAnimate);

	player_view_mon_delpress(pstEnv, pstPlayer);

	player_xukong_energy_clear(pstEnv, pstPlayer);

	stChgVal.AttrNum = 1;
	stChgVal.AttrList[0].AttrID = ATTR_ID_MP;
	stChgVal.AttrList[0].nFlag = 1;
	stChgVal.AttrList[0].Value.MP = pstPlayer->stRoleData.RoleDetail.MPCurr;
	z_attr_chg(pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);

	player_die_status_clear(pstAppCtx, pstEnv, pstPlayer);
	pstPlayer->stOnline.stDieClean.cDieClean = 0;
	pstPlayer->stOnline.stDieClean.iMemID = 0;

	z_skill_clear(pstPlayer);

	if( (CS_STAT_RIDE_ON | CS_STAT_FLY | CS_STAT_HITCH_RIDE) & pstPlayer->stOnline.State)
	{
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
	}

	tmp_item_drop( pstEnv,  pstPlayer,1);
	//tmp_item_del(pstEnv, pstPlayer);
	player_del_totem(pstEnv, pstPlayer);

	//存活时间任务追踪
	if (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_LIFE_TIME_TRACE)
	{
		task_player_trace(pstEnv, pstPlayer, TASK_OBJ_PLAYER_LIFE_TIME);
	}

	if (CS_STAT_COLLECT & pstPlayer->stOnline.State)
	{
		player_collect_break(pstEnv, pstPlayer, z_id_monster(pstAppCtx, pstEnv, pstPlayer->stOnline.iCollectTarget));
	}

	if ((CS_STAT_SIT|CS_STAT_GAOJI_SIT) & pstPlayer->stOnline.State)
	{
		player_sit_break(pstAppCtx, pstEnv, pstPlayer);
	}

	if(pstAtker)
	{
		z_add_atker_pkval(pstEnv, pstPlayer, pstAtker);
		z_add_tmpenemy(pstEnv, pstPlayer, pstAtker);
	}
	
	//清理犯罪状态要在PK值判定之后
	player_pk_time_clear(pstEnv, pstPlayer);

	player_die_pet_add_power(pstEnv, pstPlayer);

	if(pstAtker)
	{
		world_city_player_die(pstEnv, pstPlayer, pstAtker);
		player_die_battle_info_chg(pstEnv, pstPlayer, pstAtker);
	}
	else
	{
		player_die_battle_info_chg(pstEnv, pstPlayer, NULL);
	}

	//杀人 任务 追踪
	if(pstAtkPlayer && (pstAtkPlayer->stOnline.cMiscFlag & MISC_FLAG_KILL_PLAYER_TRACE))
	{
		task_hit_player_trace(pstEnv, pstAtkPlayer, pstPlayer);
	}
	
	//任务失败检查
	player_die_task_check(pstEnv, pstPlayer);

	//怪物杀人计数
	mon_kill_player_notify(pstEnv, pstPlayer, pstAtker);

	// 调用副本角色死亡脚本
	if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE)
	{
		char szName[64];
		pstPworldInst	=	z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);

		if(pstPworldInst)
		{
			//不管是不是评价副本都记录下
			pworld_val_trace_player_die(pstPworldInst);
		}
		
		if (pstPworldInst &&
			pstPworldInst->stPworldIdx.iID == BCT_PWORLD_ID)
		{
			pstPworldInst->iDownTm = 0;
		}
		
		if (pstPworldInst && 
			//pstPworldInst->ullPworldWID == pstPlayer->stRoleData.MiscInfo.PworldWID &&
			(pstPworldInst->cPworldScriptFlag & PWORLD_SCRIPT_PROC_ROLE_DIE_EVENT))
		{
			stSEnv.pstPlayer = pstPlayer;
			stSEnv.pstPworldInst = pstPworldInst;
			stSEnv.pstMonster = NULL;
			stSEnv.pstNpc = NULL;
			stSEnv.pstRoleItem = NULL;
			stSEnv.pstMapinst = NULL;
			stSEnv.pstKiller = NULL;
			memset (&stSEnv.stCsPackageUse, 0, sizeof(stSEnv.stCsPackageUse));

			snprintf(szName, sizeof(szName), "pworld%d.mac", pstPworldInst->stPworldIdx.iID);
			szName[sizeof(szName) - 1] = 0;

			z_exec_script(pstAppCtx, pstEnv, NULL, szName, &pstPworldInst->stScriptIdx, PWORLD_SCRIPT_PROC_ROLE_DIE);
		}

	}

	if(pstAtker)
	{
		player_die_punish(pstEnv, pstPlayer,pstAtker, iNewDaoba);
	}
	else
	{
		player_die_punish(pstEnv, pstPlayer,NULL, 0);
	}
	
	if(pstAtker)
	{
		z_role_die_oplog(pstEnv, pstPlayer, pstAtker);
	}
	
	if (!(CS_STAT_DEAD & pstPlayer->stOnline.State))
	{
		memset(&pstPlayer->stRoleData.MiscInfo.DieInfo, 0, 
			sizeof(pstPlayer->stRoleData.MiscInfo.DieInfo));
	}
	
	if (iNotify && (CS_STAT_DEAD & pstPlayer->stOnline.State))
	{
		pstActRes->ID = pstPlayer->iMemID;
		pstActRes->ActionID = ACTION_DEAD;
		pstActRes->Data.Dead.x = pstPlayer->stRoleData.Pos.X;
		pstActRes->Data.Dead.y = pstPlayer->stRoleData.Pos.Y;
		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);

		z_sendpkg_playerview(pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 1);
	}
	else
	{
		//z_player_move_notify(pstAppCtx, pstEnv, pstPlayer, CS_MOVE_TURN);
	}

	//pstPlayer->stOnline.iPosSeq += 50;

	poly_pworld_player_die(pstEnv, pstPlayer, pstAtker);

	sparta_player_die(pstEnv, pstPlayer, pstAtkPlayer);

	return 0;
}


int player_revive_attr_chg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, int iHpAdd, int iMpAdd)
{
	PlayerChagAttrVal stChgVal;

	stChgVal.AttrNum = 2;
	stChgVal.AttrList[0].AttrID = ATTR_ID_HP;
	stChgVal.AttrList[0].nFlag = 0;
	stChgVal.AttrList[0].Value.HP = iHpAdd;
	stChgVal.AttrList[1].AttrID = ATTR_ID_MP;
	stChgVal.AttrList[1].nFlag = 0;
	stChgVal.AttrList[1].Value.MP = iMpAdd;
	z_attr_chg(pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);
	return 0;
}

int player_revive_action(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer)
{
	CSPKG *pstPkg = &pstEnv->stCSRes;
	CSACTIONRES *pstActRes = &pstPkg->Body.ActionRes;

	if (CS_STAT_DEAD & pstPlayer->stOnline.State)
	{
		return 0;
	}

	pstActRes->ID = pstPlayer->iMemID;
	pstActRes->ActionID = ACTION_REVIVE;
	pstActRes->Data.Revive.x = pstPlayer->stRoleData.Pos.X;
	pstActRes->Data.Revive.y = pstPlayer->stRoleData.Pos.Y;
	pstActRes->Data.Revive.HP = pstPlayer->stRoleData.RoleDetail.HPCurr;
	Z_CSHEAD_INIT(&pstPkg->Head, ACTION_RES);
	z_sendpkg_playerview(pstAppCtx, pstEnv, pstPlayer, 1, pstPkg, 1);
	return 0;
}

int player_revive_svr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer)
{
	pstPlayer->stOnline.iReviverID = 0;
	pstPlayer->stOnline.State &= ~CS_STAT_DEAD;

	player_revive_attr_chg(	pstAppCtx, pstEnv, pstPlayer,
							pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax,
							pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax);
	player_revive_action(pstAppCtx, pstEnv, pstPlayer);
	return 0;

}

int player_revive_svr1(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, int iHp, int iMp)
{
	pstPlayer->stOnline.iReviverID = 0;
	pstPlayer->stOnline.State &= ~CS_STAT_DEAD;

	player_revive_attr_chg(	pstAppCtx, pstEnv, pstPlayer,iHp,iMp);
	player_revive_action(pstAppCtx, pstEnv, pstPlayer);
	return 0;
}

int player_in_pworld_lift(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZONEPWORLDINST *pstZonePworldInst;
	ZONEPWORLD *pstZonePworld;
	BATTLEDEF *pstBattleDef;
	ZONEMAPINST* pstMapInst;
	ZONEIDX stIdx;

	if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
	{
		return 0;
	}

	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos =	pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return 0;
	}

	pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (NULL == pstZonePworldInst)
	{
		return 0;
	}

	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
	if (NULL == pstZonePworld)
	{
		return 0;
	}

	if(pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_SPAN_WORLD )
	{
		return 1;
	}

	if(!is_normal_battle_pworld(pstZonePworld))
	{
		return 0;
	}

	pstBattleDef = z_find_battledef(pstEnv, pstZonePworld->stPworldDef.BattleID);
	if(NULL == pstBattleDef)
	{
		return 0;
	}

	if(pstBattleDef->FightType == BATTLE_FIGHT_TYPE_CAMP_LIFT || 
		pstBattleDef->FightType == BATTLE_FIGHT_TYPE_RAND_LIFT)
	{
		return 1;
	}
	else
	{
		return 2;
	}
	
	return 0;
}

int get_normal_battle_dynrevive_pos(ZONESVRENV* pstEnv, BATTLEDEF *pstBattleDef, Player *pstPlayer, RESPOS * pstPos)
{
	int i, iDist;
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;
	RESPOS stDynPos;
	
	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos =	pstPlayer->stOnline.iMapPos;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
		return -1;

	for(i=0; i< BATTLE_DYN_REVIVEPOS_NUM; i++)
	{
		if(pstMapInst->aiDynReviveCamp[i] != pstPlayer->stOnline.stWarOL.cCampIdx)
		{
			continue;
		}

		stDynPos = pstBattleDef->DynRevivePs[i];

		iDist = z_distance(&stDynPos, &pstPlayer->stRoleData.Pos);
		if( iDist <= z_distance(pstPos, &pstPlayer->stRoleData.Pos))
		{
			*pstPos = stDynPos;
		}
	}

	return 0;
}

static int get_pworld_revive_pos(ZONESVRENV* pstEnv, MapIndex *pstMapIdx, Player *pstPlayer, RESPOS * pstPos)
{
	int iIdx = pstPlayer->stOnline.stWarOL.cCampIdx;
	int iPloyType = pstPlayer->stOnline.stWarOL.cPloyType;
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;
	
	if (!pstMapIdx || !pstPlayer || !pstPos)
	{
		return -1;
	}

	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos	=	pstPlayer->stOnline.iMapPos;
	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv,  &stIdx);
	if(!pstMapInst)
	{
		return -1;
	}

	switch(iPloyType)
	{
		case PWORLD_POLY_SPAN_WORLD:
			{
				if(iIdx <= 0 || iIdx > REVIVEPOS_NUM)
				{
					return -1;
				}
				
				*pstPos = pstMapIdx->stMapDef.RevivePs[iIdx-1];
			}
			break;
		case PWORLD_POLY_DOUBLE_MONSTER:
		case PWORLD_POLY_TANK:
			{
				if(iIdx <= 0 || iIdx > REVIVEPOS_NUM)
				{
					return -1;
				}
				
				*pstPos = pstMapIdx->stMapDef.RevivePs[iIdx-1];
			}
			break;
		case PWORLD_POLY_MACHINE_WAR:
			{
				int iRand = RAND1(2);
				RESPOS stPos;
				
				stPos = pstMapIdx->stMapDef.RevivePs[iRand];

				z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &stPos, &stPos, 1500, 0, pstPos);
			}
			break;
		case PWORLD_POLY_BATTLE:
			{
				BATTLEDEF *pstBattleDef;
				ZONEPWORLD *pstPworld;
				ZONEIDX stIdx;

				stIdx.iID = GET_PWORLDID(pstPlayer->stRoleData.Map);
				stIdx.iPos = -1;
				
				pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);	
				if(NULL == pstPworld || !is_normal_battle_pworld(pstPworld))
				{
					return -1;
				}

				pstBattleDef = z_find_battledef(pstEnv, pstPworld->stPworldDef.BattleID);
				if(NULL == pstBattleDef)
				{
					return -1;
				}

				if(pstBattleDef->FightType == BATTLE_FIGHT_TYPE_AOSHAN)
				{
					*pstPos = pstMapIdx->stMapDef.RevivePs[iIdx-1];
					get_normal_battle_dynrevive_pos(pstEnv, pstBattleDef, pstPlayer, pstPos);
				}
				else if(pstBattleDef->FightType == BATTLE_FIGHT_TYPE_RAND_LIFT)
				{
					int iRand = (int)(RAND1(REVIVEPOS_NUM));
					
					*pstPos = pstMapIdx->stMapDef.RevivePs[iRand];
				}
				else if(pstBattleDef->FightType == BATTLE_FIGHT_TYPE_CAMP_LIFT)
				{
					*pstPos = pstMapIdx->stMapDef.RevivePs[iIdx-1];
				}
				else if(pstBattleDef->FightType == BATTLE_FIGHT_TYPE_XUKONG)
				{
					*pstPos = pstMapIdx->stMapDef.RevivePs[iIdx-1];
				}
				else if(pstBattleDef->FightType == BATTLE_FIGHT_TYPE_CLAN_WAR)
				{
					*pstPos = pstMapIdx->stMapDef.RevivePs[iIdx-1];
				}
				else if(pstBattleDef->FightType == BATTLE_FIGHT_TYPE_CLAN_WAR)
				{
					//跨服公会战复活点
					*pstPos = pstMapIdx->stMapDef.RevivePs[iIdx-1];
				}
			}
			break;
		case PWORLD_POLY_SPARTA:
			{
				if(iIdx == 1)
				{
					*pstPos = pstMapIdx->stMapDef.RevivePs[0];
				}
				else
				{
					*pstPos = pstMapIdx->stMapDef.RevivePs[1];
				}
			}
			break;
		
		default:
			{
				ZONEPWORLD *pstPworld;
				pstPworld = z_find_pworld2(pstEnv, pstPlayer->stRoleData.Map);	
				if(pstPworld && pstPworld->stPworldDef.StrongMapID > 0)
				{
					// 普通据点和跨服据点
					if (strong_get_enter_pos(pstEnv,pstPworld, pstMapIdx,pstPos, pstPlayer,1) <0)
					{
						return -1;
					}
					/*if (strong_get_map_pos(pstEnv,pstPworld->stPworldDef.StrongMapID, 
						pstMapIdx,pstPos, pstPlayer,1) < 0)
					{
						return -1;
					}*/
				}
				else
				{
					*pstPos = pstMapIdx->stMapDef.RevivePs[0];
				}
			}
			break;
	}

	if (pstPos->X <= 0 || pstPos->Y <= 0)
	{
		return -1;
	}

	if (!z_valid_pos(pstEnv->pstMapObj, pstMapIdx->stMapDef.MapID, NULL, pstPos))
	{
		return -1;
	}
	
	return 0;
}

int in_auto_revive_pworld(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;
	ZONEPWORLDINST* pstPworldInst = NULL;
	ZONEPWORLD *pstZonePworld;
	BATTLEDEF *pstBattleDef;
	
	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos =	pstPlayer->stOnline.iMapPos;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
		return 0;


	pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if(NULL == pstPworldInst)
		return 0;


	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if(NULL == pstZonePworld)
	{
		return 0;
	}

	if(is_normal_battle_pworld(pstZonePworld))
	{
		pstBattleDef = z_find_battledef(pstEnv, pstZonePworld->stPworldDef.BattleID);
		if(NULL == pstBattleDef)
		{
			return 0;
		}

		if(pstBattleDef->FightType == BATTLE_FIGHT_TYPE_AOSHAN ||
			pstBattleDef->FightType == BATTLE_FIGHT_TYPE_CLAN_WAR || //自动复活副本
			pstBattleDef->FightType == BATTLE_FIGHT_TYPE_XUKONG)
		{
			return 1;
		}
	}
	else if(pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_DOUBLE_MONSTER ||
		pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_MACHINE_WAR ||
		pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_TANK)
	{
		return 1;
	}
	else if(pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_SPARTA)
	{
		return 1;
	}

	return 0;
}

REVIVEDEF* z_find_revive_def(ZONESVRENV* pstEnv, int iLevel)
{
	REVIVEDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Level = iLevel;
	iIndex = bsearch_int(&stDef, pstObj->sReviveDef, pstObj->iReviveDef, REVIVE_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (REVIVEDEF *)&pstObj->sReviveDef[iIndex];
	}
	
	return NULL;
}

int player_revive(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	ZONEMAPINST* pstMapInst;
	ZONEIDX stIdx;
	int iMapIndex;
	MapIndex *pstMapIdx=NULL;
	RESPOS stPos;
	int iHPAdd = 0, iMPAdd = 0,iMoneyAdd=0, iExpAdd=0;
	int iPosChg = 0;
	CSREVIVE *pstRevive = &pstCsPkg->Body.Revive;
	CSREVIVEACCEPTSKILL *pstSkillRevive;
	int iProtect = 1;
	int iRet = 0;
	int iRetResult = 0;
	int iNoChgMap = 0;//原地复活,未change map
	ZONEPWORLD *pstZonePworld;
	REVIVEINFO *pstReviveInfo = &pstPlayer->stRoleData.MiscInfo.ReviveInfo;
	int iToMap;
				
	if (!(pstPlayer->stOnline.State & CS_STAT_DEAD))
	{
		return -1;
	}

	//战场倒计时复活
	if(in_auto_revive_pworld(pstEnv, pstPlayer) && 
		pstRevive->ReviveType == REVIVE_NORMAL)
	{
		if(pstPlayer->stOnline.tAutoReviveTime > pstAppCtx->stCurr.tv_sec + 2)
		{
			return -1;
		}
	}

	if(pstRevive->ReviveType == REVIVE_NIXI &&
		!is_nixi_revive_player(pstEnv, pstPlayer))
	{
		pstRevive->ReviveType = REVIVE_NORMAL;
	}

	switch(pstRevive->ReviveType)
	{
		case REVIVE_NORMAL:
			if (pstPlayer->stRoleData.Level >= 20)
			{
				iHPAdd = (pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax * NORMAL_RERIVER_RATE) / 100;
				iMPAdd = (pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax * NORMAL_RERIVER_RATE) / 100;
			}
			else
			{
				iHPAdd = pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax ;
				iMPAdd = pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax ;
			}

			iRet = player_in_pworld_lift(pstEnv, pstPlayer);
			if (iRet > 0)
			{
				// 生存大考验普通复活满血 加1204状态
				if(iRet == 1)
				{
					iProtect = 0;
				}
				
				iHPAdd = (pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax ) ;
				iMPAdd = (pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax ) ;
			}
			
			iPosChg = 1;
			//清零完美复活次数
			pstReviveInfo->PerfectReviveNum = 0;
			pstReviveInfo->ReviveNum = 0;
			break;
		case REVIVE_ACCEPT_SKILL:
			pstSkillRevive = &pstRevive->Data.AcceptSkill;
			if (pstSkillRevive->HelperRoleID != pstPlayer->stOnline.iReviverID || pstSkillRevive->SkillID != pstPlayer->stOnline.iReviverSkillID)
			{
				return -1;
			}

			iHPAdd = pstPlayer->stOnline.iReviverHP;
			iMoneyAdd = pstPlayer->stOnline.iReviverMoney;
			pstPlayer->stOnline.iReviverMoney = 0;
			pstPlayer->stRoleData.MiscInfo.DieInfo.DieDrop.Money = 0;
			iExpAdd = pstPlayer->stOnline.iReviverExp;
			pstPlayer->stOnline.iReviverExp = 0;
			pstPlayer->stRoleData.MiscInfo.DieInfo.DieDrop.Exp = 0;

			
			iPosChg = 1;
			break;
		case REVIVE_PERFECT:
			{
				tdr_ulonglong iMoney;
				REVIVEDEF *pstReviveDef;

				if (pstRevive->Flag)
				{
					if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_GODCOIN, PERFECTION_REVIVE_GOLD))
					{
						char szBuff[32];
				
						if (money_type_to_string(ATTR_ID_GODCOIN, szBuff, sizeof(szBuff)) < 0 )
						{
							strcpy(szBuff, "金币");
						}

						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN_PET2, szBuff);
						return -1;
					}

					package_dec_money(pstEnv, pstPlayer,ATTR_ID_GODCOIN, PERFECTION_REVIVE_GOLD);
					godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
											pstPlayer->stRoleData.Level,GODCOIN_OPER_FLAG_REVIVE,0,0,0,
											PERFECTION_REVIVE_GOLD,"完美复活消耗");
			
				}
				else
				{
					pstReviveDef = z_find_revive_def(pstEnv, pstPlayer->stRoleData.Level);
					if(!pstReviveDef)
					{
						return -1;
					}

					if(pstReviveInfo->DayPerfectReviveNum >= pstReviveDef->DayPerfectReviveTimes)
					{
						iMoney = pstReviveInfo->PerfectReviveMoney;

						if (0 > package_dec_money_test(pstEnv, pstPlayer, pstReviveDef->PerfectRevive.MoneyType, iMoney))
						{
							char szBuff[32];
							if (money_type_to_string(pstReviveDef->PerfectRevive.MoneyType, szBuff, sizeof(szBuff)) < 0 )
							{
								strcpy(szBuff, "金钱");
							}

							z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOENOUGH, szBuff);
							return -1;
						}

						package_dec_money(pstEnv, pstPlayer, pstReviveDef->PerfectRevive.MoneyType, iMoney);
						z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_REVIVE_PERFECT, 0, 0, iMoney, "完美复活消耗");
					}

					if(pstReviveInfo->DayPerfectReviveNum >= pstReviveDef->DayPerfectReviveTimes)
					{
						pstReviveInfo->PerfectReviveNum++;
					}
					else
					{
						pstReviveInfo->DayPerfectReviveNum++;
					}
				}
				
				iHPAdd = pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax;
				iMPAdd = pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax;
				iPosChg = 0;
			}
			break;
		case REVIVE_STAND:
			{
				tdr_ulonglong iMoney;
				REVIVEDEF *pstReviveDef;

				pstReviveDef = z_find_revive_def(pstEnv, pstPlayer->stRoleData.Level);
				if(!pstReviveDef)
				{
					return -1;
				}

				if(pstReviveInfo->DayReviveNum >= pstReviveDef->DayReviveTimes)
				{
					if(pstReviveInfo->ReviveNum+1 <= pstReviveDef->ReviveTimes)
					{
						iMoney = pstReviveDef->StandRevive.MoneyVal;
					}
					else
					{
						if(pstReviveInfo->ReviveNum +1 <= pstReviveDef->StandIncreaseNum)
						{
							iMoney = pstReviveDef->StandRevive.MoneyVal+
								pstReviveDef->StandIncreaseVal*(pstReviveInfo->ReviveNum+1-pstReviveDef->ReviveTimes);
						}
						else
						{
							iMoney = pstReviveDef->StandRevive.MoneyVal+
								pstReviveDef->StandIncreaseVal*(pstReviveInfo->ReviveNum+1-pstReviveDef->ReviveTimes);
							iMoney = iMoney*
								pow(pstReviveDef->StandIncreaseVal2, pstReviveInfo->ReviveNum +1-pstReviveDef->StandIncreaseNum);
						}
						
					}

					if(iMoney > 0x7FFFFFFF)
					{
						iMoney = 0x7FFFFFFF;
					}

					if (0 > package_dec_money_test(pstEnv, pstPlayer, pstReviveDef->StandRevive.MoneyType, iMoney))
					{
						char szBuff[32];
				
						if (money_type_to_string(pstReviveDef->StandRevive.MoneyType, szBuff, sizeof(szBuff)) < 0 )
						{
							strcpy(szBuff, "金钱");
						}

						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOENOUGH, szBuff);
						return -1;
					}

					iRet = package_dec_money(pstEnv, pstPlayer, pstReviveDef->StandRevive.MoneyType, iMoney);
					z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_REVIVE_STAND, 0, iMoney-iRet, iRet, "原地复活消耗");
				}
				
				iHPAdd = (pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax*pstReviveDef->HPMPRate)/100;
				iMPAdd = (pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax*pstReviveDef->HPMPRate)/100;
				iPosChg = 0;
				
				if(pstReviveInfo->DayReviveNum >= pstReviveDef->DayReviveTimes)
				{
					pstReviveInfo->ReviveNum++;
				}
				else
				{
					pstReviveInfo->DayReviveNum++;
				}
			}
			break;
		case REVIVE_NIXI:
			if(!is_nixi_revive_player(pstEnv, pstPlayer))
			{
				return -1;
			}
			
			iHPAdd = pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax;
			iMPAdd = pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax;
			iPosChg = 0;
			break;
		default:
			return -1;
			break;
	}

	//在自己公会城市复活，满血满魔10秒无敌
	if (GET_MAPID(pstPlayer->stRoleData.Map) == CLAN_CITY_MAP_ID &&
				is_in_my_clan_city(pstEnv, pstPlayer))
	{
		iProtect = 0;
		iHPAdd = pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax;
		iMPAdd = pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax;
	}

	if(GET_MAPID(pstPlayer->stRoleData.Map) == SPARTA_MAP_ID)
	{
		iHPAdd = pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax;
		iMPAdd = pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax;
	}
	
	pstPlayer->stOnline.iReviverID = 0;
	pstPlayer->stOnline.State &= ~CS_STAT_DEAD;

	player_revive_attr_chg(pstAppCtx, pstEnv, pstPlayer, iHPAdd, iMPAdd);
	if(iMoneyAdd>0)
	{
		package_add_money(pstEnv, pstPlayer, ATTR_ID_MONEY, iMoneyAdd);
	}
	if(iExpAdd >0)
	{
		player_add_exp(pstEnv,pstPlayer,iExpAdd,0);
	}

	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos	=	pstPlayer->stOnline.iMapPos;
	pstMapInst	=	z_find_map_inst(pstAppCtx, pstEnv,  &stIdx);
	if (NULL == pstMapInst)
	{
		//return -1;
		iRetResult = -1;
		goto _final;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		//return -1;
		iRetResult = -1;
		goto _final;
	}
	
	// 据点完美无保护
	if (	pstRevive->ReviveType == REVIVE_NORMAL || 
		(pstRevive->ReviveType == REVIVE_PERFECT && !strong_player_is_in_pworld(pstEnv, pstPlayer)))
	{
		if (!iProtect)
		{
			z_player_apply_status( pstEnv->pstAppCtx, pstEnv, pstPlayer, 
				1204, 1, 10, 1, 0);
		}
		else
		{
			if(!(pstMapIdx->stMapDef.CtrlFlag & MAP_CTRL_NO_AREAPROTECTED))
			{
				z_player_apply_status( pstEnv->pstAppCtx, pstEnv, pstPlayer, 
					STATUS_ID_AREAPROTECTED, 1, 10, 1, 0);
			}
		}
	}
	else if(pstRevive->ReviveType == REVIVE_NIXI)
	{
		z_player_apply_status( pstEnv->pstAppCtx, pstEnv, pstPlayer, 
				3191, 1, 10, 1, 0);
		z_player_apply_status( pstEnv->pstAppCtx, pstEnv, pstPlayer, 
				1204, 1, 5, 1, 0);

		player_revive_attr_chg(pstAppCtx, pstEnv, pstPlayer, 
			pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax, 0);
	}

	//天降横福活动有buff是切地图清除
	z_player_check_status(pstAppCtx, pstEnv, pstPlayer);
	if (iPosChg)
	{
		pstZonePworld = z_find_pworld2(pstEnv, pstPlayer->stRoleData.Map);
		if(pstRevive->ReviveType==REVIVE_ACCEPT_SKILL )
		{
			stPos = pstPlayer->stOnline.stReviverPos;
			if (stPos.X <= 0 || stPos.Y <= 0 || !z_valid_pos(pstEnv->pstMapObj, pstPlayer->stRoleData.Map, NULL, &stPos))
			{
				iRetResult = -1;
				goto _final;
			}

			iToMap = pstPlayer->stRoleData.Map;
			if (player_trans_check(pstEnv, pstPlayer, iToMap, 0) < 0)
			{
				iToMap = BASE_CITY_MAPID;
				stPos.X = BASE_CITY_REVIVE_POS_X;
				stPos.Y = BASE_CITY_REVIVE_POS_Y;
			}
			
			z_player_change_map(pstAppCtx,  pstEnv, pstPlayer, iToMap, &stPos, RAND1(180), 0);
		}
		else if (pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal >= PLAYER_PK_PUBLIC_ENEMY &&
				!pstMapIdx->stMapDef.NotDiePunish)
		{
			stPos.X = BLACK_NAME_REVIVE_POS_X;
			stPos.Y = BLACK_NAME_REVIVE_POS_Y;
			g_iDesigVar = 2;
			player_chg_map_by_mapid(pstEnv, pstPlayer,BLACK_NAME_REVIVE_MAP,&stPos);
			g_iDesigVar = 0;
		}
		else if (	GET_MAPID(pstPlayer->stRoleData.Map) == CLAN_CITY_MAP_ID &&
				!is_in_my_clan_city(pstEnv, pstPlayer))
		{
			player_chg_map_by_mapid(pstEnv, pstPlayer,BASE_CITY_MAPID,NULL);
		}
		else if (pstMapIdx->stMapDef.ReviveMap > 0 && pstMapIdx->stMapDef.ReviveMap < PWORLD_MAPID_BASE)
		{
			int iReviveMap = pstMapIdx->stMapDef.ReviveMap;

			stPos = pstMapIdx->stMapDef.RevivePs[0];

			// 幻境boss地图,随机3个点复活
			if (pstMapIdx->stMapDef.UnrealLayer > 0)
			{
				stPos = pstMapIdx->stMapDef.RevivePs[RAND1(REVIVEPOS_NUM)];
			}
			
			if(WORLD_CITY_MAPID == GET_MAPID(pstPlayer->stRoleData.Map))
			{
				world_city_revive_pos_get(pstEnv, pstPlayer, pstMapIdx, &iReviveMap, &stPos);
			}

			if (player_trans_check(pstEnv, pstPlayer, iReviveMap, 0) < 0)
			{
				iReviveMap = BASE_CITY_MAPID;
				stPos.X = BASE_CITY_REVIVE_POS_X;
				stPos.Y = BASE_CITY_REVIVE_POS_Y;	
			}

			//跨服野外，按阵营复活点来复活
			if(0 == player_if_in_span_map(pstEnv,pstPlayer))
			{
				span_map_revive_pos(pstPlayer->stOnline.stWarOL.cCampIdx,&stPos);
			}

			player_chg_map_by_mapid(pstEnv, pstPlayer,iReviveMap,&stPos);
		}
		else if (pstMapIdx->stMapDef.ReviveMap >= PWORLD_MAPID_BASE)  // 副本内
		{
			if (get_pworld_revive_pos(pstEnv, pstMapIdx, pstPlayer, &stPos) < 0)
			{
				iRetResult = -1;
				goto _final;
			}
			
			iMapIndex = (pstPlayer->stRoleData.Map & 0xffff0000) | pstMapIdx->stMapDef.ReviveMap;
			if (stPos.X <= 0 || stPos.Y <= 0 || !z_valid_pos(pstEnv->pstMapObj, iMapIndex, NULL, &stPos))
			{
				iRetResult = -1;
				goto _final;
			}
			z_player_change_map(pstAppCtx,  pstEnv, pstPlayer, iMapIndex, &stPos, RAND1(180), 0);
		}
		else //原地
		{
			stIdx.iID	=	pstPlayer->stRoleData.Map;
			stIdx.iPos	=	pstPlayer->stOnline.iMapPos;
			pstMapInst	=	z_find_map_inst(pstAppCtx, pstEnv,  &stIdx);
			if (pstMapInst)
			{
				z_player_search_view(pstAppCtx, pstEnv,  pstMapInst, &pstPlayer->stOnline.stLastSearchPos, pstPlayer);
			}
			iNoChgMap = 1;
		}
	}
	else
	{
		stIdx.iID	=	pstPlayer->stRoleData.Map;
		stIdx.iPos	=	pstPlayer->stOnline.iMapPos;
		pstMapInst	=	z_find_map_inst(pstAppCtx, pstEnv,  &stIdx);
		if (pstMapInst)
		{
			z_player_search_view(pstAppCtx, pstEnv,  pstMapInst, &pstPlayer->stOnline.stLastSearchPos, pstPlayer);
		}
		iNoChgMap = 1;
	}

	pstPlayer->stOnline.iKillerObjType = OBJ_UNKNOW;

	//补充调用enter map的脚本
	if (iNoChgMap)
	{
		int i = 0;
			
		//游戏事件
		for (i = 0; i < pstEnv->pstShtEvent->iMax; i++) 
		{
			GameEventInst *pstEvent = NULL;
			SHITEM * pstItem = SHT_GET_ITEM(pstEnv->pstShtEvent, i);
			if ( !pstItem->fValid )
			{
		            continue;
		       }
			
			pstEvent = (GameEventInst *)pstItem->szData;

			if(pstEvent->cScriptFlag & GAME_EVENT_SCRIPT_PROC_ENTER_MAP_MAP)
			{
				stSEnv.pstEvent= pstEvent;
				stSEnv.pstMonster = NULL;
				stSEnv.pstNpc = NULL;
				stSEnv.pstPlayer = pstPlayer;
				stSEnv.pstMapinst = NULL;
				stSEnv.pstKiller = NULL;
							
				z_exec_script(pstAppCtx, pstEnv, NULL, pstEvent->szScriptName, &pstEvent->stScriptIdx, GAME_EVENT_SCRIPT_PROC_ENTER_MAP);

			}
		}
	}

_final:
	
	player_revive_action(pstAppCtx, pstEnv, pstPlayer);

	player_hp_action(pstEnv, pstPlayer, 
		pstPlayer->stRoleData.RoleDetail.HPCurr, pstPlayer->stRoleData.RoleDetail.MPCurr, 0);

	z_player_send_status(pstAppCtx, pstEnv, pstPlayer);

	player_revive_pet_update(pstEnv, pstPlayer);
	
	return  iRetResult;
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

int player_adjust_level(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int iCheckFlag)
{
	LEVELUP *pstLevelUp;
	int iLevel;
	CSPKG *pstPkg = &pstEnv->stCSRes;
	CSPLAYERATTRVAL *pstAttrVal = &pstPkg->Body.AttrChg;
	CSACTIONRES *pstActionRes = &pstPkg->Body.ActionRes;
	int iOldLv = pstPlayer->stRoleData.Level;
	int iOldExp = pstPlayer->stRoleData.RoleDetail.Exp;
	ROLEDETAIL *pstRoleDetail = &pstPlayer->stRoleData.RoleDetail;

	iLevel = pstPlayer->stRoleData.Level;
	if (iLevel < 1 || iLevel >= pstEnv->pstConf->MaxLevel  || iLevel >= MAX_LEVEL ||iLevel >= pstEnv->pstZoneObj->iLevelUpDef)
	{
		return -1;
	}

	pstLevelUp = (LEVELUP *)pstEnv->pstZoneObj->sLevelUpDef[iLevel-1];
	if (iCheckFlag && (!pstLevelUp->LevelStand || pstRoleDetail->Exp < (int)pstLevelUp->LevelNextExp) )
	{
		return -1;
	}

	//特定地图不可以使用技能
	if (iCheckFlag && (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE) ) 
	{
		ZONEPWORLD *pstPworld;
		ZONEIDX stIdx;

		stIdx.iID = GET_PWORLDID(pstPlayer->stRoleData.Map);
		stIdx.iPos = -1;
		
		pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);	
		if (pstPworld && (pstPworld->stPworldDef.CtrlFlag & PWORLD_NO_ADJUST_LEVEL)) 
		{	
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_PLAYER159, pstPworld->stPworldDef.Name);
			return -1;
		}
	}

	if (	pstPlayer->stRoleData.Level == CAREERCHG_LEVEL &&
		(pstPlayer->stRoleData.Career&0xF0) == 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER160);
		return -1;
	}

	if (!pstEnv->pstConf->IsMatchWorld)
	{
		if (genius_check_level_up( pstEnv, pstPlayer, pstLevelUp) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER161, 
						pstLevelUp->FzMod);
			return -1;
		}
	}
	

	if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat != GLOBAL_FINI ||
				(pstEnv->pstGlobalObj->stGlobal.LevFeng.LevelFeng > 0 && pstPlayer->stRoleData.Level >= pstEnv->pstGlobalObj->stGlobal.LevFeng.LevelFeng))
	{
		int iFengLeft;

		iFengLeft = (pstEnv->pstGlobalObj->stGlobal.LevFeng.LevelFengEnd - pstAppCtx->stCurr.tv_sec)/3600;
		if (iFengLeft <= 0)
		{
			iFengLeft = 1;
		}
		
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER162, iFengLeft);
		return -1;
	}

	pstRoleDetail->Exp -= pstLevelUp->LevelNextExp;
	pstPlayer->stRoleData.Level++;
	pstPlayer->stRoleData.MiscInfo.LevelRemainInfo.LevelRemainDay = 0;
	
	player_update_cache_level(pstEnv, pstPlayer);

	package_cangbaotu_check(pstEnv,pstPlayer);
	package_cangbaotu_send(pstEnv,pstPlayer);

	pstAttrVal->AttrNum = 0;
	z_level_up_attr(pstEnv,pstPlayer, pstLevelUp);

	z_update_all_attr(pstAppCtx, pstEnv, pstPlayer);
	z_level_up_attrlist(pstEnv, pstPlayer, pstAttrVal);

	Z_CSHEAD_INIT(&pstPkg->Head, ATTR_CHG);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkg);

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_LEVEL;
	pstActionRes->Data.Level = pstPlayer->stRoleData.Level;
	Z_CSHEAD_INIT(&pstPkg->Head, ACTION_RES);
	z_sendpkg_playerview(pstAppCtx, pstEnv, pstPlayer, 1, pstPkg, 0);

	if (pstPlayer->stRoleData.Level >=15 && pstPlayer->stRoleData.Level <= 60)
	{
		if (pstPlayer->stRoleData.Level%5 == 0)
		{
			//player_auto_mate(pstEnv, pstPlayer);
		}
	}

	if(iOldLv <= 39)
	{
		PlayerChagAttrVal stAttr;

		stAttr.AttrNum = 0;
		stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_VIM;
		stAttr.AttrList[stAttr.AttrNum].Value.Vim =
			z_get_player_vimmax(pstAppCtx, pstEnv, pstPlayer);
		stAttr.AttrList[stAttr.AttrNum].nFlag = 0;
		stAttr.AttrNum++;
		z_attr_chg(pstAppCtx, pstEnv, pstPlayer, &stAttr, 1);
	}

	if (iOldLv < LUCKY_LEVEL &&
		pstPlayer->stRoleData.Level >= LUCKY_LEVEL)
	{
		player_login_lucky_ui( pstEnv, pstPlayer);
	}

	if (iOldLv < GC_PLOY_LVL &&
			pstPlayer->stRoleData.Level >= GC_PLOY_LVL)
	{
		ploy_gc_player_login(pstEnv, pstPlayer);
	}

	// 60级开启天赋
	if (GENIUS_OPEN_LVL == pstPlayer->stRoleData.Level)
	{
		genius_player_open(pstEnv, pstPlayer, 1);
	}

	relation_dbbuddy_chg(pstEnv, pstPlayer, 0,1);
	player_clan_update_mem(pstEnv, pstPlayer);

	z_role_lvup_snapshot_oplog(pstEnv, pstPlayer, iOldLv, iOldExp,0);
	return 0;
}

int player_adjust_attr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSPLAYERATTRVAL *pstAttr)
{
	LEVELUP *pstLevelUp;
	int iLevel;
	//int i;
	
	//int iVal;
	char sMap[MAX_ATTR_VAL];
	PlayerChagAttrVal stAttrValChg;
	//ROLEDETAIL *pstRoleDetail = &pstPlayer->stRoleData.RoleDetail;
	//int iCareer = pstPlayer->stRoleData.Career;
	
	stAttrValChg.AttrNum = 0;
	iLevel = pstPlayer->stRoleData.Level;
	if (iLevel < 1 || pstAttr->AttrNum <= 0 || iLevel >= pstEnv->pstConf->MaxLevel  || iLevel >= MAX_LEVEL ||iLevel >= pstEnv->pstZoneObj->iLevelUpDef)
	{
		return -1;
	}

	pstLevelUp = (LEVELUP *)pstEnv->pstZoneObj->sLevelUpDef[iLevel-1];
	memset(sMap, 0, sizeof(sMap));
	
	z_attr_chg(pstAppCtx, pstEnv, pstPlayer, &stAttrValChg, 1);
	
	return 0;
}

int player_adjust_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSADJUSTUI *pstUI)
{
	switch(pstUI->UIType)
	{
		case UI_TYPE_STASH:
			close_stash(pstAppCtx, pstEnv, pstPlayer);
			break;
		case UI_TYPE_NPCSHOP:
			shop_vip_open(pstEnv, pstPlayer);
			break;
		default:
			return -1;
			break;
	}
	
	return 0;
}

int player_adjust_lock(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSLOCK *pstLock)
{
	CSPKG *pstPkgRes = &pstEnv->stCSRes;
	CSACTIONRES *pstActionRes = &pstPkgRes->Body.ActionRes;

	memcpy(&pstPlayer->stOnline.stLock, pstLock, sizeof(pstPlayer->stOnline.stLock));

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_LOCK;
	memcpy(&pstActionRes->Data.Lock, pstLock, sizeof(pstActionRes->Data.Lock));

	Z_CSHEAD_INIT(&pstPkgRes->Head, ACTION_RES);
	z_sendpkg_playerview(pstAppCtx, pstEnv, pstPlayer, 0, pstPkgRes, 0);
	
	return 0;
}
/*
int player_adjust_amm(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSAMM *pstCSAmm)
{
	int i;
	int iPos = 0;
	ITEMDEF *pstItemDef;
	ITEMDEF *pstWeapon;
	ATKCHG *pstAtkChg;
	AMMMAP *pstAmmMap = &pstPlayer->stRoleData.Wear.AmmMap;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	AMMPACKAGE *pstAmmPak = &pstPlayer->stRoleData.Package.AmmPackage;
	CSPKG *pstPkgRes = &pstEnv->stCSRes;
	CSACTIONRES *pstActionRes = &pstPkgRes->Body.ActionRes;
	
	for (i=0; i<pstAmmPak->Num; i++)
	{
		if (pstAmmPak->RoleItems[i].DefID == (int)pstCSAmm->AmmID)
		{
			iPos = i;
			break;
		}
	}

	if (i == pstAmmPak->Num)
	{
		return -1;
	}

	pstItemDef = z_get_itemdef_roleitem(pstEnv, pstAmmPak->RoleItems+iPos);
	if (NULL == pstItemDef ||  ITEM_TYPE_AMM != pstItemDef->ItemType)
	{
		return -1;
	}

	for (i=0; i<MAX_ARM_ON; i++)
	{
		pstAtkChg = pstDyn->astAtkChg+i;
		if (0 == pstAtkChg->iArmID)
		{
			break;
		}

		pstWeapon = (ITEMDEF *)pstEnv->pstZoneObj->sItemDef[pstAtkChg->iArmDefPos];
		if ((int)pstWeapon->ItemID != pstAtkChg->iArmID)
		{
			continue;
		}

		if (0 == pstWeapon->WeaponAmmNeed)
		{
			continue;
		}

		if (pstWeapon->WeaponAmmType == pstItemDef->AmmType)
		{
			if (pstAtkChg->iAmmID == (int)pstCSAmm->AmmID)
			{
				return 0;
			}
			
			if (0 != pstWeapon->WearPos2) //组合武器放在右手位
			{
				pstAmmMap->AmmRight = pstCSAmm->AmmID;
			}
			else
			{
				switch(pstWeapon->WearPos1)
				{
					case WEAR_POS_LEFT:
						pstAmmMap->AmmLeft = pstCSAmm->AmmID;
						break;
					case WEAR_POS_RIGHTHAND:
						pstAmmMap->AmmRight = pstCSAmm->AmmID;
						break;
					case WEAR_POS_SHOULDER:
						pstAmmMap->AmmShoulder = pstCSAmm->AmmID;
						break;
					default:
						return -1;
						break;
				}
			}

			pstAtkChg->iAmmID = pstCSAmm->AmmID;
			pstAtkChg->iAmmPos = iPos;
			pstAtkChg->unBaseAmmAtk = pstItemDef->AmmAtk;
			
			pstActionRes->ID = pstPlayer->iMemID;
			pstActionRes->ActionID = ACTION_AMM;
			memcpy(&pstActionRes->Data.Amm, pstAmmMap, sizeof(pstActionRes->Data.Amm));

			Z_CSHEAD_INIT(&pstPkgRes->Head, ACTION_RES);
			z_cltmsg_send(pstEnv, pstPlayer, pstPkgRes);
			
			return 0;
		}
	}
		
	return -1;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}*/

int player_show_head(ZONESVRENV* pstEnv, Player *pstPlayer, int iShowFlag)
{
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
		
	if (iShowFlag)
	{
		pstMiscInfo->Flag |= SHOW_HEAD_FLAG;
	}
	else
	{
		pstMiscInfo->Flag &= ~SHOW_HEAD_FLAG;
	}

	z_update_avatar(pstEnv, pstPlayer);
	
	return 0;
}

//lzk   显示面饰
int player_show_face(ZONESVRENV* pstEnv, Player *pstPlayer, int iShowFlag)
{
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
		
	if (iShowFlag)
	{
		pstMiscInfo->Flag |= SHOW_FACE_FLAG;
	}
	else
	{
		pstMiscInfo->Flag &= ~SHOW_FACE_FLAG;
	}

	z_update_avatar(pstEnv, pstPlayer);
	
	return 0;

}

int player_show_fashion(ZONESVRENV* pstEnv, Player *pstPlayer, int iShowFlag)
{
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
		
	if (iShowFlag)
	{
		pstMiscInfo->Flag |= SHOW_FASHION_FLAG;
	}
	else
	{
		pstMiscInfo->Flag &= ~SHOW_FASHION_FLAG;
	}

	z_update_avatar(pstEnv, pstPlayer);
	
	return 0;
}

int player_show_back(ZONESVRENV* pstEnv, Player *pstPlayer, int iShowFlag)
{
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
		
	if (iShowFlag)
	{
		pstMiscInfo->Flag |= SHOW_BACK_FLAG;
	}
	else
	{
		pstMiscInfo->Flag &= ~SHOW_BACK_FLAG;
	}

	z_update_avatar(pstEnv, pstPlayer);
	
	return 0;
}

int player_select_fashion_suit(ZONESVRENV* pstEnv, Player *pstPlayer, CSSELECTFASHIONSUIT *pstSelect)
{
	ROLEAVATARPROF *pstRoleAvatar = &pstPlayer->stRoleData.AvatarProf;
		
	if (pstSelect->SelectIdx > pstRoleAvatar->AddFashion)
	{
		return -1;
	}
	
	pstRoleAvatar->SelectFashion = pstSelect->SelectIdx;
	z_update_avatar(pstEnv, pstPlayer);

	z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	return 0;
}

int player_show_wedding_dsn(ZONESVRENV* pstEnv, Player *pstPlayer, int iShowFlag)
{
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
		
	if (iShowFlag)
	{
		pstMiscInfo->Flag |= SHOW_WEDDING_DSN_FLAG;
	}
	else
	{
		pstMiscInfo->Flag &= ~SHOW_WEDDING_DSN_FLAG;
	}

	wedding_dsn_action(pstEnv, pstPlayer);

	return 0;
}

int player_adjust(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSADJUSTC *pstAdjust = &pstCsPkg->Body.AdjustC;

	switch(pstAdjust->Type)
	{
		case ADJUST_TYPE_LEVEL:
			return player_adjust_level(pstAppCtx, pstEnv, pstPlayer,1);
			break;
		case ADJUST_TYPE_ATTR:
			return player_adjust_attr(pstAppCtx, pstEnv, pstPlayer, &pstCsPkg->Body.AdjustC.AdjustData.Attr);
			break;
		/*case ADJUST_TYPE_FITTINGS:
			return player_adjust_fittings(pstAppCtx, pstEnv, pstPlayer, &pstCsPkg->Body.AdjustC.AdjustData.Fittings);
			break;*/
		case ADJUST_TYPE_UI:
			return player_adjust_ui(pstAppCtx, pstEnv, pstPlayer, &pstCsPkg->Body.AdjustC.AdjustData.UI);
			break;
		case ADJUST_TYPE_LOCK:
			return player_adjust_lock(pstAppCtx, pstEnv, pstPlayer, &pstCsPkg->Body.AdjustC.AdjustData.Lock);
			break;
		case ADJUST_TYPE_WEAR:
			if (pstCsPkg->Body.AdjustC.AdjustData.Wear.PetWID > 0)
			{
				//return pet_wear(pstAppCtx, pstEnv, pstPlayer, &pstCsPkg->Body.AdjustC.AdjustData.Wear);
			}
			else if (pstCsPkg->Body.AdjustC.AdjustData.Wear.PetWID == 0)
			{
				return player_wear(pstAppCtx, pstEnv, pstPlayer, &pstCsPkg->Body.AdjustC.AdjustData.Wear);
			}
			
			break;
		case ADJUST_TYPE_UNWEAR:
			if (pstCsPkg->Body.AdjustC.AdjustData.UnWear.PetWID == 0)
			{
				return player_unwear(pstAppCtx, pstEnv, pstPlayer, &pstCsPkg->Body.AdjustC.AdjustData.UnWear);
			}
			else if (pstCsPkg->Body.AdjustC.AdjustData.UnWear.PetWID > 0)
			{
				//return pet_unwear(pstAppCtx, pstEnv, pstPlayer, &pstCsPkg->Body.AdjustC.AdjustData.UnWear);
			}
			
			break;
		case ADJUST_TYPE_AMM:
			//return player_adjust_amm(pstAppCtx, pstEnv, pstPlayer, &pstCsPkg->Body.AdjustC.AdjustData.Amm);
			break;
		case ADJUST_SHOW_HEAD:
			player_show_head(pstEnv, pstPlayer, pstCsPkg->Body.AdjustC.AdjustData.ShowHead);
			break;
		case ADJUST_SHOW_FASHION:
			player_show_fashion(pstEnv, pstPlayer, pstCsPkg->Body.AdjustC.AdjustData.ShowFashion);
			break;
		case ADJUST_SHOW_BACK:
			 player_show_back( pstEnv, pstPlayer, pstCsPkg->Body.AdjustC.AdjustData.ShowBack);
			break;
		case ADJUST_SELECT_FASHION_SUIT:
			player_select_fashion_suit(pstEnv, pstPlayer, &pstCsPkg->Body.AdjustC.AdjustData.SelectFashionSuit);
			break;
		case ADJUST_SHOW_FACE:
			player_show_face(pstEnv, pstPlayer, pstCsPkg->Body.AdjustC.AdjustData.ShowFace);
			break;
		case ADJUST_SHOW_WEDDING_DSN:
			player_show_wedding_dsn(pstEnv, pstPlayer, pstCsPkg->Body.AdjustC.AdjustData.ShowWeddingDsn);
			break;
		default:
			return -1;
			break;
	}
	
	return 0;
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

int player_totem_chg_map(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	int iTotemID;
	MONSTERDEF *pstMonDef;
	Monster *pstTotem;
	ZoneSkill *pstZoneSkill;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;
	
	for (i=0; i<pstZoneSkillList->nSkillNum; i++)
	{
		pstZoneSkill = &pstZoneSkillList->astZoneSkills[i];
		if (pstZoneSkill->bTotem > 0)
		{
			iTotemID = pstZoneSkill->iTotemHead;
			while(iTotemID > 0)
			{
				pstTotem = z_id_monster(pstEnv->pstAppCtx, pstEnv, iTotemID);
				if (NULL == pstTotem)
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "totem %d err", iTotemID);
					break;;
				}
				iTotemID = pstTotem->iTotemNext;

				pstMonDef = z_get_mon_def(pstEnv, pstTotem);
				if(pstMonDef && (pstMonDef->MiscType & MON_MISC_TYPE_BUB_CHGMAP))
				{
					continue;
				}

				//怪物死亡后 tLeaveMap == 0  表示已经z_monster_leave_map 过了
				if (MONSTER_DIE != pstTotem->cStat || pstTotem->tLeaveMap > 0)
				{
					z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstTotem);
				}
				
				z_free_monster(pstEnv->pstAppCtx, pstEnv, pstTotem);
			}
		}
	}

	for(i=pstPlayer->stOnline.stBubOnline.iBubNum-1; i>=0; i--)
	{
		pstTotem = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stBubOnline.iBubID[i]);
		if(NULL == pstTotem)
		{
			continue;
		}

		pstMonDef = z_get_mon_def(pstEnv, pstTotem);
		if(pstMonDef && (pstMonDef->MiscType & MON_MISC_TYPE_BUB_CHGMAP))
		{
			continue;
		}

		//怪物死亡后 tLeaveMap == 0  表示已经z_monster_leave_map 过了
		if (MONSTER_DIE != pstTotem->cStat || pstTotem->tLeaveMap > 0)
		{
			z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstTotem);
		}
		
		z_free_monster(pstEnv->pstAppCtx, pstEnv, pstTotem);
	}

	return 0;
}

int player_del_totem(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	int iTotemID;
	Monster *pstTotem;
	ZoneSkill *pstZoneSkill;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;
	
	for (i=0; i<pstZoneSkillList->nSkillNum; i++)
	{
		pstZoneSkill = &pstZoneSkillList->astZoneSkills[i];
		if (pstZoneSkill->bTotem > 0)
		{
			iTotemID = pstZoneSkill->iTotemHead;
			while(iTotemID > 0)
			{
				pstTotem = z_id_monster(pstEnv->pstAppCtx, pstEnv, iTotemID);
				if (NULL == pstTotem)
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "totem %d err", iTotemID);
					break;;
				}
				
				iTotemID = pstTotem->iTotemNext;

				//怪物死亡后 tLeaveMap == 0  表示已经z_monster_leave_map 过了
				if (MONSTER_DIE != pstTotem->cStat || pstTotem->tLeaveMap > 0)
				{
					z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstTotem);
				}
				
				z_free_monster(pstEnv->pstAppCtx, pstEnv, pstTotem);
			}
		}

		if (0 != pstZoneSkill->bTotem)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "%d skill totem %d", pstZoneSkill->stSkillData.SkillID,
						pstZoneSkill->bTotem);
		}

		pstZoneSkill->bTotem = 0;
		pstZoneSkill->iTotemHead = 0;
	}

	for(i=pstPlayer->stOnline.stBubOnline.iBubNum-1; i>=0; i--)
	{
		pstTotem = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stBubOnline.iBubID[i]);
		if(NULL == pstTotem)
		{
			continue;
		}

		//怪物死亡后 tLeaveMap == 0  表示已经z_monster_leave_map 过了
		if (MONSTER_DIE != pstTotem->cStat || pstTotem->tLeaveMap > 0)
		{
			z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstTotem);
		}
		
		z_free_monster(pstEnv->pstAppCtx, pstEnv, pstTotem);
	}
	
	return 0;
}

int player_totem_info_del(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstTotem)
{
	Monster *pstMonTmp;
	int iTotemID;
	
	//图腾链表
	iTotemID = pstPlayer->stOnline.iTotemHead;
	while(iTotemID > 0)
	{
		if (iTotemID == pstTotem->iID)
		{
			break;
		}
		
		pstMonTmp = z_id_monster(pstEnv->pstAppCtx, pstEnv, iTotemID);
		if (NULL == pstMonTmp)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "totem %d error", iTotemID);
			return -1;
		}

		iTotemID = pstMonTmp->iPlayerTotemNext;
		if (pstMonTmp->iPlayerTotemNext == pstTotem->iID	)
		{
			pstMonTmp->iPlayerTotemNext = pstTotem->iPlayerTotemNext;
		}
	}
	
	if (pstPlayer->stOnline.iTotemHead == pstTotem->iID)
	{
		pstPlayer->stOnline.iTotemHead = pstTotem->iPlayerTotemNext;
	}
	
	pstPlayer->stOnline.bTotemNum--;

	return 0;
}

int z_send_player_viewinfo(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSENTERVIEW *pstEnterView = &pstPkg->Body.EnterView;

	z_make_player_view(pstAppCtx, pstEnv, pstPlayer, pstEnterView);
	
	Z_CSHEAD_INIT(&pstPkg->Head, ENTER_VIEW);
	z_sendpkg_playerview(pstAppCtx, pstEnv, pstPlayer, 0, pstPkg, 0);
	
	return 0;
}

int player_auto_fly_move(ZONESVRENV* pstEnv, Player *pstPlayer, RESPOS *pstPos)
{
	RESPOS stPos;
	ZONEMAPINST* pstMapInst;
	MapIndex* pstMapIdx;
	ZONEIDX stIdx;

	memcpy(&stPos, &pstPlayer->stRoleData.Pos, sizeof(stPos));

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst  =   z_find_map_inst(pstEnv->pstAppCtx, pstEnv,  &stIdx);
	if(!pstMapInst)
		return -1;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if(!pstMapIdx)
		return -1;

	if (z_valid_pos(pstEnv->pstMapObj, pstMapInst->iMapInstID, &pstMapInst->iMapPos, pstPos))
	{
		pstPlayer->stRoleData.Pos	=	*pstPos;
	}
	else
	{
		//在副本里一个不可以下落的地方下来
		if(!(pstPlayer->stOnline.State & CS_STAT_AUTOFLY) && 
			pstPlayer->stRoleData.Map > PWORLD_MAPID_BASE)
		{
			player_pworld_exit_chgmap(pstEnv, pstPlayer);
			return -1;
		}
		
		pstPlayer->stRoleData.Pos.X = pstPlayer->stRoleData.ExitPos_x;
		pstPlayer->stRoleData.Pos.Y = pstPlayer->stRoleData.ExitPos_y;
	}

	z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstPlayer->iMemID, &stPos, 0);

	pstPlayer->stOnline.cLastPathPoints = 0;
	pstPlayer->stOnline.cWalkPath = pstPlayer->stOnline.cLastPath = pstPlayer->stOnline.cCurrPoint;
	pstPlayer->stOnline.cFuturePoints = 0;
	SET_PLAYER_NOMOVE(pstPlayer);

	if(z_pos_search_pos(pstMapIdx, &pstPlayer->stRoleData.Pos, &stPos))
	{
            	z_player_search_view(pstEnv->pstAppCtx, pstEnv, pstMapInst, &stPos, pstPlayer);
	}

	if(!(pstPlayer->stOnline.State & CS_STAT_AUTOFLY))
	{
		z_send_player_viewinfo(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	return 0;
}


int player_auto_fly(ZONESVRENV* pstEnv, Player *pstPlayer, int iAutoFlyPath)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;
	
	if (CS_STAT_AUTOFLY & pstPlayer->stOnline.State)
	{
		return -1;
	}
	
	if (pstPlayer->stOnline.State &  CS_STAT_STALL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER120);
		return -1;
	}

	if (pstPlayer->stOnline.State &  CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER121);
		return -1;
	}

	if (pstPlayer->stOnline.stExchgOnline.bExchgStat != EXCHG_NO)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER122);
		return -1;
	}

	if(0 >= z_get_player_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER123);
		return -1;
	}

	if(is_player_ride_on_machine(pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER124);
		return -1;
	}
		
	z_player_del_wushuang_status(pstEnv->pstAppCtx, pstEnv, pstPlayer);

	if( (CS_STAT_RIDE_ON | CS_STAT_FLY | CS_STAT_HITCH_RIDE) & pstPlayer->stOnline.State)	
	{
		z_ride_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	if (pstPlayer->stRoleData.MiscInfo.TmpSys.TmpItemID > 0)
	{	
		tmp_item_drop(pstEnv, pstPlayer,1);			
	}

	if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
	{
		pstPlayer->stRoleData.ExitMap = pstPlayer->stRoleData.Map;
		pstPlayer->stRoleData.ExitPos_x = pstPlayer->stRoleData.Pos.X;
		pstPlayer->stRoleData.ExitPos_y = pstPlayer->stRoleData.Pos.Y;
	}
	
	pstPlayer->stOnline.State |= CS_STAT_AUTOFLY;
	pstPlayer->stRoleData.MiscInfo.AutoFlyPath = iAutoFlyPath;

	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_AUTOFLY;
	pstAction->Data.AutoFly.AutoPathIdx = iAutoFlyPath;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);

	z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);

	player_view_mon_delpress(pstEnv, pstPlayer);

	return 0;
}

int player_auto_fly_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;
	CSAUTOFLYCLT *pstAutoFlyClt = &pstCsPkg->Body.AutoFlyClt;

	if (0 == (CS_STAT_AUTOFLY & pstPlayer->stOnline.State))
	{
		return -1;
	}

	if(pstAutoFlyClt->AutoFlyEnd)
	{
		pstPlayer->stOnline.State &= ~CS_STAT_AUTOFLY;
		pstPlayer->stRoleData.MiscInfo.AutoFlyPath = 0;

		if(0 == player_auto_fly_move(pstEnv, pstPlayer, &pstAutoFlyClt->EndPos))
		{
			pstAction->ID = pstPlayer->iMemID;
			pstAction->ActionID = ACTION_AUTOFLY;
			pstAction->Data.AutoFly.AutoPathIdx = 0;

			Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
			z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
		}
		
		z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}
	else
	{
		player_auto_fly_move(pstEnv, pstPlayer, &pstAutoFlyClt->EndPos);
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

int chg_name_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,char *pszNewName,tdr_ulonglong ullRoleID,char *pszOldName,int iFlag)
{
	CSPKG stPkg;
	CSCHGNAMESVR *pstChgNameSvr = &stPkg.Body.ChgNameSvr;
	int ret,ret2;
	int i;
	ROLEBUDDYMEMBER * pstSpouseMember;

	Z_CSHEAD_INIT(&stPkg.Head, CHGNAME_SVR);
	pstChgNameSvr->Flag = iFlag;
	STRNCPY(pstChgNameSvr->NewName, pszNewName, sizeof(pstChgNameSvr->NewName));
	STRNCPY(pstChgNameSvr->OldName, pszOldName, sizeof(pstChgNameSvr->OldName));
	pstChgNameSvr->WID = ullRoleID;

	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	//lzk   名字改变了，有情侣或伴侣的头顶的信息也要更新
	ret = find_lover_byID(&pstPlayer->stRoleData.Buddy, ullRoleID);
	ret2 = find_spouse_byID(&pstPlayer->stRoleData.Buddy , ullRoleID);
	if(ret >= 0 || ret2 >= 0)
	{
		for(i=0;i<pstPlayer->stRoleData.Buddy.SpouseNum;i++)
		{
			pstSpouseMember = &pstPlayer->stRoleData.Buddy.Spouse[i];
			if(pstSpouseMember->RoleID == ullRoleID)
			{
				if (strcmp(pstSpouseMember->RoleName, pszNewName)  != 0)
				{
					STRNCPY(pstSpouseMember->RoleName, pszNewName, sizeof(pstSpouseMember->RoleName));
					wedding_dsn_action(pstEnv, pstPlayer);
				}
				break;
			}
		}
	}
	
	return 0;
}

int chg_clan_name_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, CLANID *pstClanID)
{
	CSPKG stPkg;
	CSCHGNAMESVR *pstChgNameSvr = &stPkg.Body.ChgNameSvr;

	pstChgNameSvr->Flag = CHG_CLAN_NAME;
	STRNCPY(pstChgNameSvr->NewName, pstClanID->Name, sizeof(pstChgNameSvr->NewName));
	pstChgNameSvr->WID = pstClanID->GID;
	pstChgNameSvr->ID = pstClanID->Id;

	Z_CSHEAD_INIT(&stPkg.Head, CHGNAME_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int chg_name_check_select_res(ZONESVRENV* pstEnv, Player *pstPlayer,TORMSELECTRES *pstSelectRes, char *pszName)
{
	NAMEID stNameID;
	ROLEBUDDYMEMBER *pstMember;
	ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;
	int i;

	if (1 != pstSelectRes->SelectResult.ResultNum)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
					"select uin=%d role num=%d", pstPlayer->stRoleData.Uin,
					pstSelectRes->SelectResult.ResultNum);
		return -1;
	}

	if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stNameID, sizeof(stNameID)))
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
					"uin=%d torm_fetch_one_meta fail", pstPlayer->stRoleData.Uin);
		return -1;
	}

	if (strcmp(stNameID.Name, pszName) == 0)
	{
		return 0;
	}

	for (i=0;i<pstMisc->BlackNum; i++)
	{
		if (strcmp(pstMisc->BlackMember[i].RoleName, pszName) == 0)
		{
			STRNCPY(pstMisc->BlackMember[i].RoleName, stNameID.Name, sizeof(pstMisc->BlackMember[i].RoleName));
			chg_name_to_clt(pstEnv, pstPlayer,stNameID.Name, pstMisc->BlackMember[i].RoleID, pszName,CHG_ROLE_NAME);
			return 0;
		}
	}

	for (i=0; i<pstMisc->Enemy.EnemyNum; i++)
	{
		if (strcmp(pstMisc->Enemy.Enemys[i].RoleName, pszName) == 0)
		{
			STRNCPY(pstMisc->Enemy.Enemys[i].RoleName, stNameID.Name, sizeof(pstMisc->Enemy.Enemys[i].RoleName));
			chg_name_to_clt(pstEnv, pstPlayer,stNameID.Name, pstMisc->Enemy.Enemys[i].RoleID, pszName,CHG_ROLE_NAME);
			return 0;
		}
	}

	pstMember = relation_buddy_online_find_by_name(&pstPlayer->stRoleData.Buddy, pszName);
	if (!pstMember)
	{
		return -1;
	}

	STRNCPY(pstMember->RoleName, stNameID.Name, sizeof(pstMember->RoleName));
	chg_name_to_clt(pstEnv, pstPlayer,stNameID.Name, pstMember->RoleID, pszName,CHG_ROLE_NAME);
	return 0;
}


int login_chg_clanname_check(ZONESVRENV* pstEnv, Player *pstPlayer, CLANID *pstClanID)
{
	ClanHash stClanHash;
	ClanHash *pstClanHash;
	ZoneClan *pstClan;

	if (pstEnv->pstClanObj->stClanObjInfo.iClanStat != CLAN_STAT_FINI)
	{
		return 0;
	}
	
	stClanHash.uiClanId = pstClanID->Id;
	pstClanHash = sht_find(pstEnv->pstShtClan, &stClanHash, ClanHashCmp, ClanHashCode);
	if (NULL == pstClanHash )
	{
		return -1;
	}

	if (0 > pstClanHash->iIdx || pstClanHash->iIdx >= pstEnv->pstClanObj->iClanNum )
	{
		return -1;
	}

	pstClan = &pstEnv->pstClanObj->astZoneClan[pstClanHash->iIdx];

	if (strcmp(pstClan->stClan.ClanProf.Name, pstClanID->Name) != 0)
	{
		STRNCPY(pstClanID->Name, pstClan->stClan.ClanProf.Name, sizeof(pstClanID->Name));

		chg_clan_name_to_clt(pstEnv, pstPlayer, pstClanID);
	}

	return 0;
}

int chg_clanname_check_select_res(ZONESVRENV* pstEnv, Player *pstPlayer,TORMSELECTRES *pstSelectRes, char *pszName)
{
	CLAN stClan;

	if (1 != pstSelectRes->SelectResult.ResultNum)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
					"select uin=%d role num=%d", pstPlayer->stRoleData.Uin,
					pstSelectRes->SelectResult.ResultNum);
		return -1;
	}

	if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stClan, sizeof(stClan)))
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
					"uin=%d torm_fetch_one_meta fail", pstPlayer->stRoleData.Uin);
		return -1;
	}

	if (strcmp(stClan.ClanProf.Name, pszName) == 0)
	{
		return 0;
	}

	STRNCPY(pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Name, stClan.ClanProf.Name, 
				sizeof(pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Name));

	chg_name_to_clt(pstEnv, pstPlayer,stClan.ClanProf.Name, pstPlayer->stRoleData.RoleID, pszName,CHG_CLAN_NAME);
	
	return 0;
}

/*static int role_name_check_select_req(ZONESVRENV* pstEnv, Player *pstPlayer,  tdr_ulonglong ullRoleID, char*pszName)
{
	NAMEID stNameID;
	DBAsyn stDBAsyn;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;

	stDBAsyn.iCmd = DB_CHG_NAME_SELECT;
	strncpy(stDBAsyn.AsynCmd.stChgRoleName.szName, pszName, sizeof(stDBAsyn.AsynCmd.stChgRoleName.szName));
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, "NameID", 
						pstPlayer->stRoleData.Uin, &stDBAsyn);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	snprintf(pstSelectReq->OperInfo.WhereDef,sizeof(pstSelectReq->OperInfo.WhereDef),"where GID=%llu", ullRoleID);
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 0;

	memset(&stNameID, 0, sizeof(stNameID));

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stNameID;
	stMetaData.iBuff = sizeof(stNameID);
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}
	return 0;
}*/


int login_check_chg_clanname(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CLANID *pstClanID = &pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId;
	if (pstClanID->GID > 0)
	{
		if (strstr(pstClanID->Name, "*"))
		{
			login_chg_clanname_check(pstEnv, pstPlayer, pstClanID);
		}
	}
	return 0;
}


int login_check_chg_rolename(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	/*int i;
	ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;
	ROLEBUDDY *pstBuddy =  &pstPlayer->stRoleData.Buddy;
	

	for (i=0; i<pstMisc->BlackNum; i++)
	{
		//if (strstr(pstMisc->BlackMember[i].RoleName, "*") )
		{
			if (role_name_check_select_req(pstEnv, pstPlayer, pstMisc->BlackMember[i].RoleID,pstMisc->BlackMember[i].RoleName) < 0)
			{
				return -1;
			}
		}
	}

	for (i=0; i<pstMisc->Enemy.EnemyNum; i++)
	{
		//if (strstr(pstMisc->Enemy.Enemys[i].RoleName, "*") )
		{
			if (role_name_check_select_req(pstEnv, pstPlayer, pstMisc->Enemy.Enemys[i].RoleID,pstMisc->Enemy.Enemys[i].RoleName) < 0)
			{
				return -1;
			}
		}
	}

	for (i=0;i<pstBuddy->BrotherNum; i++)
	{
		//if (strstr(pstBuddy->Brothers[i].RoleName, "*"))
		{
			if (role_name_check_select_req(pstEnv, pstPlayer, pstBuddy->Brothers[i].RoleID, pstBuddy->Brothers[i].RoleName) < 0)
			{
				return -1;
			}
		}
	}

	for (i=0;i<pstBuddy->FriendNum; i++)
	{
		//if (strstr(pstBuddy->Friends[i].RoleName, "*"))
		{
			if (role_name_check_select_req(pstEnv, pstPlayer, pstBuddy->Friends[i].RoleID,pstBuddy->Friends[i].RoleName) < 0)
			{
				return -1;
			}
		}
	}

	for (i=0;i<pstBuddy->MasterNum; i++)
	{
		//if (strstr(pstBuddy->Master[i].RoleName, "*"))
		{
			if (role_name_check_select_req(pstEnv, pstPlayer, pstBuddy->Master[i].RoleID,pstBuddy->Master[i].RoleName) < 0)
			{
				return -1;
			}
		}
	}

	for (i=0;i<pstBuddy->PrenticeNum; i++)
	{
		//if (strstr(pstBuddy->Prentices[i].RoleName, "*"))
		{
			if (role_name_check_select_req(pstEnv, pstPlayer, pstBuddy->Prentices[i].RoleID,pstBuddy->Prentices[i].RoleName) < 0)
			{
				return -1;
			}
		}
	}

	for (i=0;i<pstBuddy->SpouseNum; i++)
	{
		//if (strstr(pstBuddy->Spouse[i].RoleName, "*"))
		{
			if (role_name_check_select_req(pstEnv, pstPlayer, pstBuddy->Spouse[i].RoleID,pstBuddy->Spouse[i].RoleName) < 0)
			{
				return -1;
			}
		}
	}*/

	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}


int chg_name_insert_nameid(ZONESVRENV* pstEnv,tdr_ulonglong ullRoleID, int iUin, const char* pstOldName)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	NAMEID stNameID;

	stNameID.GID = z_get_gid(pstEnv->pstAppCtx, pstEnv);
	stNameID.NameType = 65;
	STRNCPY(stNameID.Name, pstOldName, sizeof(stNameID.Name));
	stNameID.NewTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_INSERT_REQ, "NameID", 
						iUin, NULL);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stNameID;
	stMetaData.iBuff = sizeof(stNameID);
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}
	
	return 0;

	UNUSED(ullRoleID);	//add by paraunused.pl
}

 int chg_name_update_req(ZONESVRENV* pstEnv,tdr_ulonglong ullRoleID, int iUin, ChgRoleName *pstChg)
{
	DBAsyn stDBAsyn;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	CHGNAMEID stChgNameID;
	Player *pstPlayer;

	pstPlayer = z_name_player(pstEnv->pstAppCtx,  pstEnv, pstChg->szOldName);
	if (pstPlayer)
	{
		if (	pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN ||
			pstPlayer->stOnline.stSpanOnline.stApply.iBattleApply == SPAN_APPLY_ING ||
			INST_MODE_ZONE_SVR != pstEnv->iInstMode )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, 
								ZONEERR_PLAYER163);
			return -1;
		}
	}

	stChgNameID.GID = ullRoleID;
	STRNCPY(stChgNameID.Name, pstChg->szName, sizeof(stChgNameID.Name));
	
	stDBAsyn.iCmd = DB_CHG_NAME_UP;
	stDBAsyn.AsynCmd.stChgRoleName = *pstChg;
	
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, "ChgNameID", 
						iUin, &stDBAsyn);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stChgNameID;
	stMetaData.iBuff = sizeof(stChgNameID);
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}
	
	return 0;
}


int chg_rolename_update_relation(ZONESVRENV* pstEnv, tdr_ulonglong ullRoleID,  char *pszNewName)
{
	int i,j;
	ROLEMISCINFO *pstMisc;
	ROLEBUDDY *pstBuddy;
	ROLEBUDDYMEMBER *pstMember;
	Player *pstRelationPlayer;
	TMEMBLOCK* pstMemBlock;
	int k;
	ROLEBUDDYMEMBER * pstSpouseMember;

	for (i=0; i<MAX_PLAYER; i++)
	{
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstRelationPlayer = (Player *)pstMemBlock->szData;
		if (PLAYER_STATUS_ROLE_LOGIN != pstRelationPlayer->eStatus)
		{
			continue;
		}

		pstMisc = &pstRelationPlayer->stRoleData.MiscInfo;
		pstBuddy = &pstRelationPlayer->stRoleData.Buddy;
		
		for (j=0; j<pstMisc->BlackNum;j++)
		{
			if (pstMisc->BlackMember[j].RoleID == ullRoleID)
			{
				STRNCPY(pstMisc->BlackMember[j].RoleName, pszNewName, sizeof(pstMisc->BlackMember[j].RoleName));
				break;
			}
		}

		for (j=0; j<pstMisc->Enemy.EnemyNum; j++)
		{
			if (pstMisc->Enemy.Enemys[j].RoleID == ullRoleID)
			{
				STRNCPY(pstMisc->Enemy.Enemys[j].RoleName, pszNewName, sizeof(pstMisc->Enemy.Enemys[j].RoleName));
				break;
			}
		}

		for(k=0;k<pstBuddy->SpouseNum;k++)
		{
			pstSpouseMember = &pstBuddy->Spouse[k];
			if(pstSpouseMember->RoleID == ullRoleID)
			{
				if (strcmp(pstSpouseMember->RoleName, pszNewName)  != 0)
				{
					STRNCPY(pstSpouseMember->RoleName, pszNewName, sizeof(pstSpouseMember->RoleName));
					wedding_dsn_action(pstEnv, pstRelationPlayer);
				}
				break;
			}
		}

		pstMember = relation_buddy_online_find_by_roleid(pstBuddy, ullRoleID);
		if (pstMember)
		{
			STRNCPY(pstMember->RoleName, pszNewName, sizeof(pstMember->RoleName));
		}
		
	}
	
	return 0;

}

static int chg_clanname_update_relation(ZONESVRENV* pstEnv, char *pszOldName, char *pszNewName)
{
	int i,j;
	Player *pstRelationPlayer;
	TMEMBLOCK* pstMemBlock;
	BUDDY *pstBuddy;

	for (i=0; i<MAX_PLAYER; i++)
	{
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstRelationPlayer = (Player *)pstMemBlock->szData;
		if (PLAYER_STATUS_ROLE_LOGIN != pstRelationPlayer->eStatus)
		{
			continue;
		}

		if (pstRelationPlayer->stOnline.bGetBuddyStat != BUDDY_GET_FINI)
		{
			continue;
		}

		pstBuddy = &pstRelationPlayer->stOnline.stRelation.Buddy;
		for (j=0; j<pstBuddy->BuddyNum; j++)
		{
			if (0 == strcmp(pszOldName, pstBuddy->BuddyInfos[j].ClanName))
			{
				STRNCPY(pstBuddy->BuddyInfos[j].ClanName, pszNewName, sizeof(pstBuddy->BuddyInfos[j].ClanName));
				pstRelationPlayer->stOnline.bRelationUpdateFlag = 2;
				break;
			}	
		}
		
	}
	
	return 0;

}

int chg_name_up_sht(ZONESVRENV* pstEnv,int iUin,tdr_ulonglong RoleID, char *pszNewName,char *pszOldName,int iMemID)
{
	int i;
	UinCache *pstUinCache;
	UinCache stUinHashData;
	NameMemID stNameMemID;
	NameMemID *pstNameMemID;
	Player *pstPlayer;
	
	stUinHashData.iUin = iUin;
	pstUinCache = (UinCache *)sht_find(pstEnv->pstShtUinCache,&stUinHashData,UinHashCmp,UinHashCode);
	if (!pstUinCache)
	{
		return -1;
	}
	
	for (i=0; i<pstUinCache->ucRoleNum; i++)
	{
		if (pstUinCache->astRoleListInfo[i].RoleID == RoleID)
		{
			STRNCPY(pstUinCache->astRoleListInfo[i].RoleName, pszNewName, sizeof(pstUinCache->astRoleListInfo[i].RoleName));
			break;
		}
	}

	pstPlayer = player_get_by_uin(pstEnv, iUin);
	if (pstPlayer && pstPlayer->eStatus == PLAYER_STATUS_ROLE_LIST)
	{
		z_send_rolelist(pstEnv,pstPlayer, pstUinCache);
	}
	
	STRNCPY(stNameMemID.szName, pszOldName, sizeof(stNameMemID.szName));
	if (sht_remove(pstEnv->pstShtNameMemID, &stNameMemID, NameMemIDHashCmp, NameMemIDHashCode))
	{
		
		stNameMemID.iID = iMemID;
		
		STRNCPY(stNameMemID.szName, pszNewName, sizeof(stNameMemID.szName));
		pstNameMemID = sht_insert_unique(pstEnv->pstShtNameMemID, &stNameMemID, NameMemIDHashCmp, NameMemIDHashCode);
		if (pstNameMemID)
		{
			memcpy(pstNameMemID, &stNameMemID, sizeof(*pstNameMemID));
		}
	}
	return 0;
}

int chg_role_name_up_team(ZONESVRENV* pstEnv, Player *pstPlayer,  char *pszNewName)
{
	ZoneTeam *pstTeam;
	//int i;
	TEAMMEMBER *pstMember;
	
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if (pstTeam)
	{
		pstMember = player_team_get_member(pstTeam, pstPlayer->stRoleData.RoleName, NULL);
		if (pstMember)
		{
			strncpy(pstMember->MemberName, pszNewName, sizeof(pstMember->MemberName));
		}
	}
	return 0;
}

int chg_role_name_up_db(ZONESVRENV* pstEnv,int iUin,tdr_ulonglong ullRoleID,
							const char* pszOldName, const char* pszNewName, ACCOUNTROLENAME *pstAccRoleName)
{
	// 更新AccountRole 
	z_account_db_update_name(pstEnv, iUin, pstAccRoleName);
	// 更新RoleDrop
	z_db_role_drop_update_name(pstEnv, ullRoleID, pszNewName);
	// 更新ShadowRoleData
	//z_db_shadowroledata_up_name(pstEnv, ullRoleID, pszNewName);
	// 更新TopRange
	range_up_role_name(pstEnv, ullRoleID, pszOldName, pszNewName);
	//更新金币银币寄售
	shop_chg_rolename(pstEnv, ullRoleID, pszNewName);

	//更新巅峰活动
	dianfeng_update_name(pstEnv,pszOldName,pszNewName);

	//更新终极熔岩巨兽召唤者名字
	zj_rongyan_update_call_name(pstEnv,pszOldName,pszNewName);

	return 0;
}

int chg_role_name_notify(ZONESVRENV* pstEnv, tdr_ulonglong ullRoleID, const char* pszNew, const char* pszOld)
{
	CSPKG stPkg;
	CSCHGNAMESVR *pstChgNameSvr = &stPkg.Body.ChgNameSvr;


	pstChgNameSvr->Flag = CHG_ROLE_NAME;
	pstChgNameSvr->WID = ullRoleID;
	pstChgNameSvr->ID = 0;
	STRNCPY(pstChgNameSvr->NewName, pszNew, sizeof(pstChgNameSvr->NewName));
	STRNCPY(pstChgNameSvr->OldName, pszOld, sizeof(pstChgNameSvr->OldName));
	
	Z_CSHEAD_INIT(&stPkg.Head, CHGNAME_SVR);
	z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
	return 0;
}

static int chg_role_name(ZONESVRENV* pstEnv, Player *pstPlayer,  char *pszNewName)
{
	ROLEDATA *pstRoleData = &pstPlayer->stRoleData;
	ZoneClan * pstClan;
	int i;
	ACCOUNTROLENAME stAccRoleName;
	BAICENGHISTORY *pstHis = &pstEnv->pstGlobalObj->stGlobal.Misc.BaiCengHistory;
	char szOldName[ROLE_NAME_LEN];
	ROLESCHOOL *pstRoleSchool = &pstPlayer->stRoleData.SyncRelation.School;
	Player *pstTmpPlayer;

	// 工会
	pstClan = player_get_clan( pstEnv, pstPlayer );
	if (pstClan)
	{
		if (strcmp(pstClan->stClan.ClanBrief.Leader.RoleName, pstRoleData->RoleName) == 0)
		{
			strncpy(pstClan->stClan.ClanBrief.Leader.RoleName, pszNewName, 
				sizeof(pstClan->stClan.ClanBrief.Leader.RoleName));
		}

		for (i=0; i<pstClan->stClan.ClanProf.MemNum; i++)
		{
			if (strcmp(pstClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName, pstRoleData->RoleName) == 0)
			{
				strncpy(pstClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName, pszNewName, 
					sizeof(pstClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName));
				
			}
		}

		for (i=0; i<pstClan->stClan.ClanBrief.AssistNum; i++)
		{
			if (strcmp(pstClan->stClan.ClanBrief.Assists[i].RoleName, pstRoleData->RoleName)  == 0)
			{
				strncpy(pstClan->stClan.ClanBrief.Assists[i].RoleName,pszNewName, 
					sizeof(pstClan->stClan.ClanBrief.Assists[i].RoleName));
			}
		}

		// 公会捐赠记录
		for (i=0; i<pstClan->stClan.ClanAdvance.ClanDonateLogInfo.Num; i++)
		{
			if (strcmp(pstClan->stClan.ClanAdvance.ClanDonateLogInfo.Log[i].MemberName, pstRoleData->RoleName)  == 0)
			{
				strncpy(pstClan->stClan.ClanAdvance.ClanDonateLogInfo.Log[i].MemberName,pszNewName, 
					sizeof(pstClan->stClan.ClanAdvance.ClanDonateLogInfo.Log[i].MemberName));
			}
		}

		// 公会领取记录
		for (i=0; i<pstClan->stClan.ClanAdvance.Resource.GetInfoNum; i++)
		{
			if (strcmp(pstClan->stClan.ClanAdvance.Resource.GetResInfos[i].Name, pstRoleData->RoleName)  == 0)
			{
				strncpy(pstClan->stClan.ClanAdvance.Resource.GetResInfos[i].Name,pszNewName, 
					sizeof(pstClan->stClan.ClanAdvance.Resource.GetResInfos[i].Name));
			}
		}

		// 公会福利
		if (strcmp(pstClan->stClan.ClanBoon.Assign.AssignGlod.Name, pstRoleData->RoleName) == 0)
		{
			strncpy(pstClan->stClan.ClanBoon.Assign.AssignGlod.Name,pszNewName, 
					sizeof(pstClan->stClan.ClanBoon.Assign.AssignGlod.Name));	
		}

		for (i=0; i<pstClan->stClan.ClanBoon.Assign.Office; i++)
		{
			if (strcmp(pstClan->stClan.ClanBoon.Assign.AssignBindGlod[i].Name, pstRoleData->RoleName) == 0)
			{
				strncpy(pstClan->stClan.ClanBoon.Assign.AssignBindGlod[i].Name,pszNewName, 
						sizeof(pstClan->stClan.ClanBoon.Assign.AssignBindGlod[i].Name));	
			}
		}
		// 写回数据库
		clan_set_dirty_notify(pstClan, CACHE_FLAG_IMM);
		
	}

	for (i=0; i<BAICENG_CENG_NUM; i++)
	{
		if (strcmp(pstHis->BaiCengHistoryEntrys[i].Name, pstRoleData->RoleName)  == 0)
		{
			strncpy(pstHis->BaiCengHistoryEntrys[i].Name,pszNewName, 
					sizeof(pstHis->BaiCengHistoryEntrys[i].Name));	
			pstEnv->pstGlobalObj->stGlobalObjInfo.iDirty = 2;
		}
	}
	// 更新公会宠物
	clan_pet_player_chg_name(pstEnv, pstPlayer, pszNewName);

	// 更新队伍信息
	chg_role_name_up_team(pstEnv, pstPlayer,  pszNewName);

	player_mail_chgname(pstEnv, pstPlayer->stRoleData.RoleName, pszNewName);

	// 更新共享内存
	chg_name_up_sht(pstEnv,pstRoleData->Uin,pstRoleData->RoleID, pszNewName,pstRoleData->RoleName,pstPlayer->iMemID);

	// 更新relation
	chg_rolename_update_relation(pstEnv, pstPlayer->stRoleData.RoleID,  pszNewName);

	STRNCPY(szOldName, pstPlayer->stRoleData.RoleName, sizeof(szOldName));

	// 全服广播
	chg_role_name_notify(pstEnv,  pstPlayer->stRoleData.RoleID, pszNewName, pstPlayer->stRoleData.RoleName);

	wedding_dsn_action(pstEnv, pstPlayer);

	span_strong_chg_name( pstEnv, 0, pstPlayer->stRoleData.RoleName, pszNewName);

	// 立即更新roledata
	STRNCPY(pstPlayer->stRoleData.RoleName, pszNewName, sizeof(pstPlayer->stRoleData.RoleName));
	pstPlayer->stRoleData.MiscInfo.NameChangeTimes = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstPlayer->stOnline.cDataDirty = DIRTY_IMPORTANT;

	// 以下的pstPlayer为改名之后的
	store_process_role_chg_name(pstEnv, pstPlayer);

	if(pstRoleSchool->JobType == ROLE_SCHOOL_TYPE_TEACHER)
	{
		ROLETEACHER *pstRoleTeacher = &pstRoleSchool->Data.RoleTeacher;

		for(i = 0; i < pstRoleTeacher->Num; i++)
		{
			pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleTeacher->Students[i].RoleName);
			if(pstTmpPlayer)
			{
				ROLESTUDENT *pstRoleStudent = &pstTmpPlayer->stRoleData.SyncRelation.School.Data.RoleStudent;
				
				strncpy(pstRoleStudent->Teacher.RoleName, pstPlayer->stRoleData.RoleName, 
						sizeof(pstRoleStudent->Teacher.RoleName));
			}
			else
			{
				DBAsyn stDBAsyn;

				stDBAsyn.AsynCmd.stSyncRelationSelect.Uin = pstPlayer->stRoleData.Uin;
				stDBAsyn.AsynCmd.stSyncRelationSelect.ullRoleID = pstPlayer->stRoleData.RoleID;
				stDBAsyn.AsynCmd.stSyncRelationSelect.iType =  3;
				strncpy(stDBAsyn.AsynCmd.stSyncRelationSelect.szNewName, 
						pstPlayer->stRoleData.RoleName,
						sizeof(stDBAsyn.AsynCmd.stSyncRelationSelect.szNewName));

				role_sync_relation_select_req(pstEnv, pstPlayer, pstRoleTeacher->Students[i].RoleID, &stDBAsyn);
			}
		}
	}
	else if(pstRoleSchool->JobType == ROLE_SCHOOL_TYPE_STUDENT)
	{
		ROLESTUDENT *pstRoleStudent = &pstRoleSchool->Data.RoleStudent;
		
		pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleStudent->Teacher.RoleName);
		if(pstTmpPlayer)
		{
			ROLESTUDENTINFO *pstRoleStudentInfo;
			
			pstRoleStudentInfo = school_teacher_get_stuinfo(pstEnv, pstTmpPlayer, pstPlayer->stRoleData.RoleID);
			if(pstRoleStudentInfo)
			{
				strncpy(pstRoleStudentInfo->RoleName, pstPlayer->stRoleData.RoleName, 
						sizeof(pstRoleStudentInfo->RoleName));
			}
		}
		else
		{
			DBAsyn stDBAsyn;

			stDBAsyn.AsynCmd.stSyncRelationSelect.Uin = pstPlayer->stRoleData.Uin;
			stDBAsyn.AsynCmd.stSyncRelationSelect.ullRoleID = pstPlayer->stRoleData.RoleID;
			stDBAsyn.AsynCmd.stSyncRelationSelect.iType =  4;
			strncpy(stDBAsyn.AsynCmd.stSyncRelationSelect.szNewName, 
					pstPlayer->stRoleData.RoleName,
					sizeof(stDBAsyn.AsynCmd.stSyncRelationSelect.szNewName));

			role_sync_relation_select_req(pstEnv, pstPlayer, pstRoleStudent->Teacher.RoleID, &stDBAsyn);
		}
	}

	if (szOldName[0] != '*')
	{
		package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, ITEM_CHGNAME_ID, 1, -1, NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_USE);
	}

	stAccRoleName.RoleWID = pstPlayer->stRoleData.RoleID;
	STRNCPY(stAccRoleName.RoleName, pstPlayer->stRoleData.RoleName, 
		      CCH(stAccRoleName.RoleName));

	chg_role_name_up_db(pstEnv, pstPlayer->stRoleData.Uin, pstPlayer->stRoleData.RoleID,
							szOldName, stAccRoleName.RoleName,	&stAccRoleName);

	player_kickoff (pstEnv->pstAppCtx, pstEnv, pstPlayer, ZONEERR_KICK207);
	return 0;
}

int chg_name_update_res(ZONESVRENV* pstEnv, Player *pstPlayer, TORMUPDATERES *pstUpdateRes, char *pszNewName)
{
	// 修改数据
	return chg_role_name(pstEnv, pstPlayer, pszNewName);
	UNUSED(pstUpdateRes);	//add by paraunused.pl
}

static int player_chg_name_check(ZONESVRENV* pstEnv, Player *pstPlayer, CSCHGNAMECLT *pstChgNameClt)
{
	int i;
	CLANINFO *pstClanInfo = &pstPlayer->stRoleData.MiscInfo.ClanInfo;
	//int iFlag;

	if (pstPlayer->stOnline.State & CS_STAT_DEAD)
	{
		return -1;
	}

	/*if (pstChgNameClt->Flag == CHG_ROLE_NAME)
	{
		iFlag = UI_TYPE_CHG_ROLE_NAME;
	}
	else if(pstChgNameClt->Flag == CHG_CLAN_NAME)
	{
		iFlag = UI_TYPE_CHG_CLAN_NAME;
	}
	else
	{
		return -1;
	}

	if (check_npc_ui(pstEnv, pstPlayer, iFlag) < 0)
	{
		return -1;
	}*/

	if (pstEnv->pstClanObj->stClanObjInfo.iClanStat != CLAN_STAT_FINI)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_BUSY);
		return -1;
	}

	if (pstPlayer->stOnline.bGetBuddyStat != BUDDY_GET_FINI)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_BUSY);
		return -1;
	}

	if (	pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN ||
		pstPlayer->stOnline.stSpanOnline.stApply.iBattleApply == SPAN_APPLY_ING ||
		INST_MODE_ZONE_SVR != pstEnv->iInstMode )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv,  pstPlayer, SYS_MIDDLE2, 
							ZONEERR_PLAYER125);
		return -1;
	}

	if (pstChgNameClt->Flag == CHG_CLAN_NAME)
	{
		ZoneClan *pstZoneClan = NULL;
		ClanObj * pstClanObj = NULL;
		
		pstZoneClan =  player_get_clan(pstEnv, pstPlayer);
		pstClanObj = pstEnv->pstClanObj;

		// 无工会
		if (pstZoneClan == NULL || pstClanInfo->ClanId.GID <= 0)
		{
			return -1;
		}

		// 名字没有星星
		if (strstr(pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Name, "*") == NULL)
		{
			//z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER126);
			//return -1;
		}
		
		// 不是会长
		if (strcmp(pstZoneClan->stClan.ClanBrief.Leader.RoleName, pstPlayer->stRoleData.RoleName) != 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER127);
			return -1;
		}

		if (strcmp(pstClanInfo->ClanId.Name, pstChgNameClt->NewName) == 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER128);
			return -1;
		}

		// 重名
		for( i = 0; i < pstClanObj->iClanNum; i++ )
		{
			const CLAN * pstClan = &pstClanObj->astZoneClan[i].stClan;

			if( strcmp( pstClan->ClanProf.Name, pstChgNameClt->NewName ) == 0 )
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER129);
				return -1;
			}
		}
	}
	else if(pstChgNameClt->Flag == CHG_ROLE_NAME)
	{
		if (strcmp(pstPlayer->stRoleData.RoleName, pstChgNameClt->NewName) == 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER130);
			return -1;
		}

		if (!(pstPlayer->stOnline.cMiscFlag & MISC_FLAG_MAIL_FETCHED))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_BUSY);
			return -1;
		}

		if (pstPlayer->stRoleData.MiscInfo.RoleTradeRecordInfo.TradeSellNum > 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER801);	
			return -1;
		}

		if (pstPlayer->stRoleData.MiscInfo.RoleTradeRecordInfo.TradeBuyNum > 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER802);	
			return -1;
		}
		
		/*不再数星星
		// 名字没有星星
		if (strstr(pstPlayer->stRoleData.RoleName, "*") == NULL)
		{
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, "角色名没有被系统修改过");
			return -1;
		}     
		*/

		if (pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stRoleData.MiscInfo.NameChangeTimes < 7*24*3600)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER131);
			return -1;
		}

		if (pstPlayer->stRoleData.RoleName[0] != '*')
		{
			if (package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, ITEM_CHGNAME_ID, 1, -1, NULL,PACKAGE_DEC_NONE))
			{
				return -1;
			}
		}
	}
	else
	{
		return -1;
	}

	if ((0 == pstEnv->pstConf->CountryVer && CheckCharacter(pstChgNameClt->NewName)) ||
		(CheckDirty(pstChgNameClt->NewName , 0x1f) & 0x1f ) )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_DIRTY1);
		return -1;
	}
	
	return 0;
}

int player_chg_rolename(ZONESVRENV* pstEnv, Player *pstPlayer, CSCHGNAMECLT *pstChgNameClt)
{

	ChgRoleName stChg;
	
	memset (&stChg, 0, sizeof(stChg));
	STRNCPY(stChg.szName, pstChgNameClt->NewName, sizeof(stChg.szName));
	
	if (player_chg_name_check(pstEnv, pstPlayer, pstChgNameClt) < 0)
	{
		return -1;
	}

	//   检查NameID表是否名字重复
	//   异步处理
	//   update能成功说明没有重名
	return chg_name_update_req(pstEnv, pstPlayer->stRoleData.RoleID, pstPlayer->stRoleData.Uin,&stChg);
	
}

int chg_clan_name(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan, char *pszNewName)
{
	int i, j;
	Player *pstClanPlayer;
	CLANID *pstClanID;
	ClanHash stClanHash;
	ClanHash *pstClanHash;
	ZoneClan *pstEnemyClan;
	ZoneClan *pstFriendClan;
	CSPKG stPkg;
	CSCHGNAMESVR *pstChgNameSvr = &stPkg.Body.ChgNameSvr;
	
	for (i=0; i<pstZoneClan->stClan.ClanProf.MemNum; i++)
	{
		pstClanPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName);
		if (pstClanPlayer)
		{
			pstClanID = &pstClanPlayer->stRoleData.MiscInfo.ClanInfo.ClanId;
			STRNCPY(pstClanID->Name, pszNewName, sizeof(pstClanID->Name));
		}
	}

	for (i=0; i<pstZoneClan->stClan.ClanInner.ClanEnemyNum; i++)
	{
		stClanHash.uiClanId = pstZoneClan->stClan.ClanInner.ClanEnemys[i].ClanId.Id;
		pstClanHash = sht_find(pstEnv->pstShtClan, &stClanHash, ClanHashCmp, ClanHashCode);
		if (NULL == pstClanHash )
		{
			return -1;
		}

		if (0 > pstClanHash->iIdx || pstClanHash->iIdx >= pstEnv->pstClanObj->iClanNum )
		{
			return -1;
		}

		pstEnemyClan = &pstEnv->pstClanObj->astZoneClan[pstClanHash->iIdx];
		for (j=0; j<pstEnemyClan->stClan.ClanInner.ClanEnemyNum; j++)
		{
			if (pstEnemyClan->stClan.ClanInner.ClanEnemys[j].ClanId.GID ==
			    pstZoneClan->stClan.ClanProf.GID)
			{
				STRNCPY(pstEnemyClan->stClan.ClanInner.ClanEnemys[j].ClanId.Name,
				        pszNewName, 
				        sizeof(pstEnemyClan->stClan.ClanInner.ClanEnemys[j].ClanId.Name));
				pstEnemyClan->stZoneClanInfo.iDirty = CACHE_FLAG_IMM;
				break;
			}
		}
	}

	for (i=0; i<pstZoneClan->stClan.ClanInner.ClanFriendNum; i++)
	{
		stClanHash.uiClanId = pstZoneClan->stClan.ClanInner.ClanFriends[i].ClanId.Id;
		pstClanHash = sht_find(pstEnv->pstShtClan, &stClanHash, ClanHashCmp, ClanHashCode);
		if (NULL == pstClanHash )
		{
			return -1;
		}

		if (0 > pstClanHash->iIdx || pstClanHash->iIdx >= pstEnv->pstClanObj->iClanNum )
		{
			return -1;
		}

		pstFriendClan = &pstEnv->pstClanObj->astZoneClan[pstClanHash->iIdx];
		for (j=0; j<pstFriendClan->stClan.ClanInner.ClanFriendNum; j++)
		{
			if (pstFriendClan->stClan.ClanInner.ClanFriends[j].ClanId.GID ==
			    pstZoneClan->stClan.ClanProf.GID)
			{
				STRNCPY(pstFriendClan->stClan.ClanInner.ClanFriends[j].ClanId.Name,
				           pszNewName, 
					    sizeof(pstFriendClan->stClan.ClanInner.ClanFriends[j].ClanId.Name));
				pstFriendClan->stZoneClanInfo.iDirty = CACHE_FLAG_IMM;
				break;
			}
		}
	}

	chg_clanname_update_relation(pstEnv, pstZoneClan->stClan.ClanProf.Name, pszNewName);

	range_up_clanname(pstEnv, 0, pstZoneClan->stClan.ClanProf.Name, pszNewName, 0);

	span_strong_chg_name( pstEnv, 1, pstZoneClan->stClan.ClanProf.Name, pszNewName);

	STRNCPY(pstChgNameSvr->OldName, pstZoneClan->stClan.ClanProf.Name, sizeof(pstChgNameSvr->OldName));
	pstChgNameSvr->Flag = CHG_CLAN_NAME;
	pstChgNameSvr->WID = pstZoneClan->stClan.ClanProf.GID;
	pstChgNameSvr->ID = pstZoneClan->stClan.ClanProf.Id;
	STRNCPY(pstChgNameSvr->NewName, pszNewName, sizeof(pstChgNameSvr->NewName));
		
	Z_CSHEAD_INIT(&stPkg.Head, CHGNAME_SVR);
	z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);

	STRNCPY(pstZoneClan->stClan.ClanProf.Name, pszNewName, sizeof(pstZoneClan->stClan.ClanProf.Name));
	pstZoneClan->stZoneClanInfo.iDirty = CACHE_FLAG_IMM;

	return 0;
}

int player_chg_clanname(ZONESVRENV* pstEnv, Player *pstPlayer, CSCHGNAMECLT *pstChgNameClt)
{
	ZoneClan * pstClan;
	
	if (player_chg_name_check(pstEnv, pstPlayer, pstChgNameClt) < 0)
	{
		return -1;
	}

	pstClan = player_get_clan( pstEnv, pstPlayer );
	if (!pstClan)
	{
		return -1;
	}

	chg_clan_name(pstEnv, pstClan, pstChgNameClt->NewName);

	return 0;
}


int player_chg_name(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{

	 CSCHGNAMECLT *pstChgNameClt = &pstCsPkg->Body.ChgNameClt; 

	if( pstEnv->iInstMode != INST_MODE_ZONE_SVR )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
		return 0;
	}
	
	 switch(pstChgNameClt->Flag)
	 {
	case CHG_ROLE_NAME:
		return player_chg_rolename(pstEnv, pstPlayer, pstChgNameClt);
		break;

	case CHG_CLAN_NAME:
		return player_chg_clanname(pstEnv, pstPlayer, pstChgNameClt);
		break;

	default:
		return -1;
		break;
	 }
	

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstFrameHead);	//add by paraunused.pl
}


int player_arm_bull_rate(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i, j, iRet = 0;
	ITEMDEF * pstItemDef = NULL;
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;

	i = pstRoleWear->ArmNum;
	while(i--)
	{
		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)(pstRoleWear->RoleArms+i));
		
		j = MAX_ITEM_RESULT;
		while(j--)
		{
			if(pstItemDef->Result[j].ResultID == RESULT_ADD_BULL_DROP)
			{
				iRet += pstItemDef->Result[j].ResultVal1;
			}
		}
	}

	return iRet;

	/*
	for( i = 0; i < pstRoleWear->ArmNum; i++ )
	{
		ITEMDEF * pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)(pstRoleWear->RoleArms+i));
		if( !pstItemDef )
			continue;

		if (pstItemDef->WearPos1 == WEAR_POS_QIYUE)
		{
			pstPlayer->stOnline.bQiYueArmIdx = i;
			return z_qiyue_id_from_itemdef( pstItemDef );
		}
	}
	*/

	return 0;
}

int player_jump(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOSKILL))
	{
		z_send_sys_str_msg(pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER132);
		return -1;
	}
	
	pstActionRes->ActionID = ACTION_JUMP;
	pstActionRes->ID = pstPlayer->iMemID;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstAppCtx, pstEnv, pstPlayer, 0, &stPkg, 0);

	return 0;
	UNUSED(pstFrameHead);	//add by paraunused.pl
	UNUSED(pstCsPkg);	//add by paraunused.pl
}

//全服状态
int z_role_svr_status(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	WorldEvent *pstWorldEvent = pstEnv->pstWorldEvent;

	for(i= 0; i <pstWorldEvent->iStatusNum; i++)
	{
		GameEventInst stEventInst, *pstEvent;
		int iTime;
		
		stEventInst.iEventID = pstWorldEvent->stStatus[i].EventIdx;
		pstEvent = sht_find(pstEnv->pstShtEvent, &stEventInst, EventHashCmp, EventHashCode);
		if (!pstEvent)
		{
			continue;
		}

		iTime = pstWorldEvent->stStatus[i].iTime - (pstEnv->pstAppCtx->stCurr.tv_sec - pstEvent->tEventStart);
		
		if(iTime > 0)
		{
			z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstPlayer,
								pstWorldEvent->stStatus[i].iStatusID, 
								pstWorldEvent->stStatus[i].iLevel, 
								iTime, 1, 0);
		}
	}

	return 0;
}

int player_bulletin_snapshot(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSBULLETINSNAPSHOT *pstRes = 
		&stPkg.Body.BulletinSvr.BulletinSvrData.BulletinSnapshot;
	const BulletinBoard *pstBoard = pstEnv->pstBulletinBoard;
	int i = 0;

	pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_BULLETIN_SNAP;

	if(pstBoard->iNum <= 0)
	{
		return 0;
	}

	// jerry modify
	pstRes->Num = 0;
	for (i=0; i<pstBoard->iNum; i++)
	{
		const BULLETINENTRY *pstSrc = NULL;
		BULLETINENTRY *pstDst = NULL;
		// 没到激活时间的不发
		if (pstBoard->astBulletinArray[i].iActiveStart > pstEnv->pstAppCtx->stCurr.tv_sec &&
			pstBoard->astBulletinArray[i].iFlag == 0)
		{
			continue;
		}

		pstSrc = &pstBoard->astBulletinArray[i].stCore;
		pstDst = pstRes->BulletinArray + pstRes->Num;

		*pstDst = *pstSrc;
		//pstDst->ID = pstSrc->ID;
		//pstDst->RollInterval = pstSrc->RollInterval;
		//STRNCPY(pstDst->Text, pstSrc->Text, CCH(pstDst->Text));
		pstRes->Num++;
	}

	if (0 == pstRes->Num)
		return 0;

	Z_CSHEAD_INIT(&stPkg.Head, BULLETIN_SVR);
	stPkg.Body.BulletinSvr.Operation = BULLETIN_SVR_SNAPSHOT;
	return z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
}

int z_bulletin_chg_notify(ZONESVRENV* pstEnv, const BULLETINENTRY *pstChgData,
	                        int fAddOrDel)
{
	CSPKG stPkg;

	if(fAddOrDel)
	{
		BULLETINENTRY *pstRes = 
			&stPkg.Body.BulletinSvr.BulletinSvrData.BulletinAdded;

		*pstRes = *pstChgData;
		//pstRes->ID = pstChgData->ID;
		//pstRes->RollInterval = pstChgData->RollInterval;
		//STRNCPY(pstRes->Text, pstChgData->Text, CCH(pstRes->Text));

		stPkg.Body.BulletinSvr.Operation = BULLETIN_SVR_CHG_ADD;
	}
	else
	{
		stPkg.Body.BulletinSvr.BulletinSvrData.BulletinDeletedID = pstChgData->ID;
		stPkg.Body.BulletinSvr.Operation = BULLETIN_SVR_CHG_DEL;
	}

	Z_CSHEAD_INIT(&stPkg.Head, BULLETIN_SVR);
	return z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
}

int player_watch_role(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstTarget)
{
	CSPKG stPkg;
	CSWATCHPLAYERSVR *pstRes = &stPkg.Body.WatchPlayerSvr;

	if( INST_MODE_ZONE_SVR == pstEnv->iInstMode &&
		pstTarget->eStatus != PLAYER_STATUS_ROLE_LOGIN &&
		pstTarget->eStatus != PLAYER_STATUS_ROLE_NOTHING)
	{
		return -1;
	}
	
	pstRes->PlayerMemID = pstTarget->iMemID;
	pstRes->RoleLevel = pstTarget->stRoleData.Level;
	pstRes->RoleGender = pstTarget->stRoleData.Gender;
	pstRes->RoleCareer = pstTarget->stRoleData.Career;
	pstRes->RoleHair = pstTarget->stRoleData.Hair;
	pstRes->RoleHead = pstTarget->stRoleData.Head;
	pstRes->RoleFace = pstTarget->stRoleData.Face;
	STRNCPY(pstRes->RoleName, pstTarget->stRoleData.RoleName,
		      CCH(pstRes->RoleName));
	pstRes->RoleWear = pstTarget->stRoleData.Wear;
	pstRes->SelectFashion = pstTarget->stRoleData.AvatarProf.SelectFashion;
	pstRes->ClanWID = pstTarget->stRoleData.MiscInfo.ClanInfo.ClanId.GID;
	z_get_watched_attr(pstEnv->pstAppCtx, pstEnv, pstTarget, &pstRes->RoleAttrVal);

	//基础数值白色字参考标准
	pstRes->RoleBaseAttrVal = pstTarget->stOnline.stBaseAttrList;

	pstRes->RoleMiscFlag =pstTarget->stRoleData.MiscInfo.Flag;
	pstRes->SparPkg = pstTarget->stRoleData.Package.SparPkg;
	pstRes->WorldID  = pstTarget->stRoleData.WorldID;
	pstRes->Genius = genius_get_mod( pstEnv, pstTarget);
	pstRes->DaoBaInfo = pstTarget->stRoleData.MiscInfo.DaoBaInfo;
	pstRes->GeniusInfo = pstTarget->stRoleData.MiscInfo.GeniusInfo;
	
	Z_CSHEAD_INIT(&stPkg.Head, WATCH_PLAYER_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	// 称号
	player_desig_get( pstEnv, pstPlayer, GET_DESIG_WATCH);
	if (pstPlayer != pstTarget)
	{
		player_desig_get( pstEnv, pstTarget, GET_DESIG_WATCH);
	}
	return 0;
}

int player_watch(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						     TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	Player *pstTarget = NULL;
	int iEqual = 0;
	CSWATCHPLAYERCLT *pstReq = &pstCsPkg->Body.WatchPlayerClt;


	UNUSED(pstFrameHead);

	bsearch_int(&pstReq->PlayerMemID, pstPlayer->stOnline.aiVAnimate, 
	            pstPlayer->stOnline.iVAnimate,
	            sizeof(pstPlayer->stOnline.aiVAnimate[0]), &iEqual);
	if(!iEqual)
	{
		return -1;
	}

	pstTarget = z_id_player(pstAppCtx, pstEnv, pstReq->PlayerMemID);
	if(!pstTarget || pstTarget == pstPlayer)
	{
		return -1;
	}

	return player_watch_role(pstEnv, pstPlayer, pstTarget);
}

int player_watch_friend(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	Player *pstTarget = NULL;
	int iBusID;
	CSWATCHFRIENDCLT *pstReq = &pstCsPkg->Body.WatchFriendClt;

	UNUSED(pstFrameHead);

	/*if(!relation_buddy_online_find_by_name(&pstPlayer->stRoleData.Buddy,
	                                       pstReq->FriendName))
	{
		// 不是好友
		return -1;
	}*/

	/*
	if(pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "跨服不允许观察好友");
		return -1;
	}*/

	if(pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		char szName[NAME_LEN];
		unsigned short unWorldID;

		if((strstr(pstReq->FriendName, BATTLE_NAME_SYMBOL) == NULL))
		{
			unWorldID = role_world_id_get(pstPlayer);
			snprintf(szName,sizeof(szName),"w%d"BATTLE_NAME_SYMBOL"%s",
					unWorldID,
					pstReq->FriendName);

			STRNCPY(pstReq->FriendName, szName, CCH(pstReq->FriendName));
		}

		pstTarget = z_name_player(pstAppCtx, pstEnv, pstReq->FriendName);
		if (!pstTarget || PLAYER_STATUS_ROLE_LOGIN != pstTarget->eStatus)
		{
			SSPKG stSSPkg;
			SSCMDREQ* pstCmdReq = &stSSPkg.Body.CmdReq;

			iBusID = player_master_busid_get(pstEnv, pstPlayer);
			
			memset(pstCmdReq,0,sizeof(*pstCmdReq));
			pstCmdReq->Cmd = SS_CMD_WATCH_PROXY;
			STRNCPY(pstCmdReq->szName, pstPlayer->stRoleData.RoleName, sizeof(pstCmdReq->szName));
			battle_rolename_remove_prefix(pstCmdReq->szName);
			STRNCPY(pstCmdReq->szReceiver, pstReq->FriendName, sizeof(pstCmdReq->szReceiver));
			battle_rolename_remove_prefix(pstCmdReq->szReceiver);
			
			Z_SSHEAD_INIT(&stSSPkg.Head, SS_CMD_REQ);
			z_ssmsg_send(pstEnv, &stSSPkg, iBusID);
		}
		else
		{
			return player_watch_role(pstEnv, pstPlayer, pstTarget);
		}

		return 0;
	}
	else
	{
		pstTarget = z_name_player(pstAppCtx, pstEnv, pstReq->FriendName);
		if(!pstTarget)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER133, pstReq->FriendName);
			return -1;
		}

		player_watch_role(pstEnv, pstPlayer, pstTarget);
	}
	
	return 0;
}

int player_weixing_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,Player *pstWxPlayer,Monster *pstMon,char cOp)
{
	CSPKG stPkg;
	CSWEIXINGSVR *pstWeiXingSvr = &stPkg.Body.WeiXingSvr;
	MapIndex* pstMapIdx;
	
	pstWeiXingSvr->WeiXingOp = cOp;
	if (cOp == WEIXING_SVR_POS)
	{
		pstWeiXingSvr->WeiXingData.WeiXingPos.MapLine  = 0;
		if (pstWxPlayer)
		{
			pstWeiXingSvr->WeiXingData.WeiXingPos.Map = GET_MAPID(pstWxPlayer->stRoleData.Map);
			pstWeiXingSvr->WeiXingData.WeiXingPos.X = pstWxPlayer->stRoleData.Pos.X;
			pstWeiXingSvr->WeiXingData.WeiXingPos.Y = pstWxPlayer->stRoleData.Pos.Y;
			pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, 
										pstWeiXingSvr->WeiXingData.WeiXingPos.Map);
			if (pstMapIdx  && pstMapIdx->stMapDef.MapLine > 0)
			{
				pstWeiXingSvr->WeiXingData.WeiXingPos.MapLine = pstWxPlayer->stRoleData.MiscInfo.MapLine;
			}
		
		}
		else if (pstMon)
		{
			pstWeiXingSvr->WeiXingData.WeiXingPos.Map = GET_MAPID(pstMon->stMap.iID);
			pstWeiXingSvr->WeiXingData.WeiXingPos.X = pstMon->stCurrPos.X;
			pstWeiXingSvr->WeiXingData.WeiXingPos.Y = pstMon->stCurrPos.Y;
			pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, 
										pstWeiXingSvr->WeiXingData.WeiXingPos.Map);
			if (pstMapIdx  && pstMapIdx->stMapDef.MapLine > 0)
			{
				pstWeiXingSvr->WeiXingData.WeiXingPos.MapLine = 0;
			}
		}
		else
		{
			return -1;
		}
	}
	else if (cOp == WEIXING_SVR_APPEAL)
	{
		pstWeiXingSvr->WeiXingData.WeiXingAppeal.MemID = pstWxPlayer->iMemID;
	}
	else
	{
		return -1;
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, WEI_XING_SVR);
	return z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
}

int player_weixing_req_check(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,Player *pstWxPlayer)
{
	WeiXingInfo *pstWxInfo;
	int i;
	MapIndex* pstMapIdx2;
	MapIndex* pstMapIdx1;
	int iReferMapID1;
	int iReferMapID2;

	pstWxInfo = &pstWxPlayer->stOnline.stWeiXing;
	if (pstWxPlayer == pstPlayer)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER164);
		return -1;
	}

	for (i=0; i<pstWxInfo->cWeiXingNum; i++)
	{
		if (pstWxInfo->aiWeiXingMemID[i] == pstPlayer->iMemID)
		{
			return -1;
		}
	}

	if (pstWxInfo->cWeiXingNum >= MAX_WEI_XING)
	{
		z_send_sys_str_msg(pstAppCtx,pstEnv,pstPlayer,SYS_MIDDLE2,ZONEERR_PLAYER134);
		return -1;
	}

	if (	pstPlayer->stRoleData.Map != pstWxPlayer->stRoleData.Map &&
		pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE )
	{
		z_send_sys_str_msg(pstAppCtx,pstEnv,pstPlayer,SYS_MIDDLE2,
						ZONEERR_PLAYER135);
		return -1;
	}

	if (	pstPlayer->stRoleData.Map != pstWxPlayer->stRoleData.Map &&
		pstWxPlayer->stRoleData.Map >= PWORLD_MAPID_BASE )
	{
		z_send_sys_str_msg(pstAppCtx,pstEnv,pstPlayer,SYS_MIDDLE2,
						ZONEERR_PLAYER135);
		return -1;
	}

	// 以下是判断: 两个角色是否是在不同线的同一地图
	pstMapIdx1 = z_mappos_index(pstEnv->pstMapObj, NULL, pstPlayer->stRoleData.Map);
	if (!pstMapIdx1 )
		return -1;

	pstMapIdx2 = z_mappos_index(pstEnv->pstMapObj, NULL, pstWxPlayer->stRoleData.Map);
	if (!pstMapIdx2 )
		return -1;

	if (pstMapIdx1->stMapDef.MapLine == 0 || pstMapIdx2->stMapDef.MapLine== 0)
	{
		return 0;
	}

	iReferMapID1 = pstMapIdx1->stMapDef.ReferMapID>0?
				   	pstMapIdx1->stMapDef.ReferMapID:pstMapIdx1->stMapDef.MapID;
	iReferMapID2 = pstMapIdx2->stMapDef.ReferMapID>0?
					pstMapIdx2->stMapDef.ReferMapID:pstMapIdx2->stMapDef.MapID;
	
	if (	iReferMapID2 == iReferMapID1 &&
		pstPlayer->stRoleData.MiscInfo.MapLine != pstWxPlayer->stRoleData.MiscInfo.MapLine)
	{
		z_send_sys_str_msg(pstAppCtx,pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_PLAYER136);
		return -1;
	}
	
	return 0;
}


int player_unweixing(Player *pstPlayer, Player *pstWxPlayer, Monster *pstMon)
{
	int i;


	if (pstWxPlayer)
	{
		WeiXingInfo *pstWxInfo = &pstWxPlayer->stOnline.stWeiXing;
		for (i=0; i<pstWxInfo->cWeiXingNum; i++)
		{
			if (pstWxInfo->aiWeiXingMemID[i] == pstPlayer->iMemID)
			{
				pstWxInfo->cWeiXingNum --;
				if (i != pstWxInfo->cWeiXingNum)
				{
					pstWxInfo->aiWeiXingMemID[i] = pstWxInfo->aiWeiXingMemID[pstWxInfo->cWeiXingNum];
				}
				break;
			}
		}
	}
	else if (pstMon)
	{
		for (i=0; i<pstMon->iFollow; i++)
		{
			if (pstMon->astFollow[i] == pstPlayer->iMemID)
			{
				pstMon->iFollow --;
				if (i !=pstMon->iFollow)
				{
					pstMon->astFollow[i] =pstMon->astFollow[pstMon->iFollow];
				}
				break;
			}
		}
	}
	else
	{
		return -1;
	}
	
	return 0;
}

int player_weixing(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						     TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	
	int iFlag = pstCsPkg->Body.WeiXingClt.Flag;
	WeiXingInfo *pstWxInfo;
	ZoneTeam *pstTeam;
	TEAMMEMBER * pstCaptain;
	Player *pstWxPlayer;
	Monster *pstMon;
	CSWEIXINGCLT *pstClt = &pstCsPkg->Body.WeiXingClt;
	int i;
	
	if (pstClt->Type == 0)
	{
		if( 	INST_MODE_ZONE_SVR == pstEnv->iInstMode &&
		(strstr(pstCsPkg->Body.WeiXingClt.RoleName, BATTLE_NAME_SYMBOL) != NULL))
		{
			if( iFlag == PLAYER_WEIXING &&
				0>player_check_same_world(pstEnv,pstCsPkg->Body.WeiXingClt.RoleName))
			{
				z_send_sys_str_msg(pstAppCtx,pstEnv,pstPlayer,SYS_TIP, ZONEERR_PLAYER137);
				return -1;
			}
			battle_rolename_remove_prefix(pstCsPkg->Body.WeiXingClt.RoleName);
		}

		if(is_battle_pvp_svr(pstEnv))
		{
			char szName[NAME_LEN];
			unsigned short unWorldID;

			if((strstr(pstCsPkg->Body.WeiXingClt.RoleName, BATTLE_NAME_SYMBOL) == NULL))
			{
				unWorldID = role_world_id_get(pstPlayer);
				snprintf(szName,sizeof(szName),"w%d"BATTLE_NAME_SYMBOL"%s",
						unWorldID,
						pstCsPkg->Body.WeiXingClt.RoleName);
				STRNCPY(pstCsPkg->Body.WeiXingClt.RoleName, szName, CCH(pstCsPkg->Body.WeiXingClt.RoleName));
			}
		}
	}


	if (iFlag == PLAYER_WEIXING) 	// 尾行
	{
		if (pstClt->Type == 0)
		{
			pstWxPlayer = z_name_player(pstAppCtx, pstEnv, pstCsPkg->Body.WeiXingClt.RoleName);
			if(!pstWxPlayer || pstWxPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER165);
				return -1;
			}

			if ( player_weixing_req_check(pstAppCtx, pstEnv, pstPlayer,pstWxPlayer) < 0)
			{
				return -1;
			}
			pstWxInfo = &pstWxPlayer->stOnline.stWeiXing;
			pstWxInfo->aiWeiXingMemID[pstWxInfo->cWeiXingNum] = pstPlayer->iMemID;
			pstWxInfo->cWeiXingNum ++;

			return player_weixing_to_clt(pstEnv, pstPlayer,pstWxPlayer,NULL,WEIXING_SVR_POS);
		}
		else
		{
			pstMon = z_id_monster(pstAppCtx, pstEnv, pstCsPkg->Body.WeiXingClt.MemID);
			if (	!pstMon ||
				!(is_my_convoy_car(pstPlayer,pstMon) ||
				is_my_team_convoy_car(pstEnv, pstPlayer,pstMon)))
			{
				z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER138);
				return -1;
			}

			for (i=0; i<pstMon->iFollow; i++)
			{
				if (pstMon->astFollow[i] == pstPlayer->iMemID)
				{
					z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER139);
					return -1;
				}
			}

			if (pstMon->iFollow >= MAX_MON_FOLLOW)
			{
				return -1;
			}
			pstMon->astFollow[pstMon->iFollow ++] = pstPlayer->iMemID;

			return player_weixing_to_clt(pstEnv, pstPlayer,NULL,pstMon,WEIXING_SVR_POS);
		}
	}
	else	 if ( iFlag == PLAYER_UNWEIXING )		// 解除尾行
	{
		if (pstClt->Type == 0)
		{
			pstWxPlayer = z_name_player(pstAppCtx, pstEnv, pstCsPkg->Body.WeiXingClt.RoleName);
			return player_unweixing(pstPlayer, pstWxPlayer,NULL);
		}
		else
		{
			pstMon = z_id_monster(pstAppCtx, pstEnv, pstCsPkg->Body.WeiXingClt.MemID);
			return player_unweixing(pstPlayer, NULL,pstMon);
		}
		
	}
	else if (iFlag == PLAYER_APPEAL_WEIXING)		// 队长要求尾行
	{
		CSPKG stPkg;
		CSWEIXINGSVR *pstWeiXingSvr = &stPkg.Body.WeiXingSvr;
	
		pstTeam = player_team_get(pstEnv, pstWxPlayer);
		if(!pstTeam)
		{
			return -1;
		}

		pstCaptain = player_team_get_captain( pstTeam );
		if (!pstCaptain || strcmp(pstCaptain->MemberName, pstWxPlayer->stRoleData.RoleName) != 0)
		{
			return -1;
		}

		pstWeiXingSvr->WeiXingOp = WEIXING_SVR_APPEAL;
		pstWeiXingSvr->WeiXingData.WeiXingAppeal.MemID = pstWxPlayer->iMemID;
		STRNCPY(pstWeiXingSvr->WeiXingData.WeiXingAppeal.RoleName, 
					pstWxPlayer->stRoleData.RoleName, 
					sizeof(pstWeiXingSvr->WeiXingData.WeiXingAppeal.RoleName));
		Z_CSHEAD_INIT(&stPkg.Head, WEI_XING_SVR);

		return player_team_broadcast(pstEnv, pstTeam, &stPkg);
	}
	return 0;
	UNUSED(pstFrameHead);
}

int mon_weixing_clear(ZONESVRENV* pstEnv, Monster *pstMon)
{
	int i;
	int iCount = 0;
	Player * apstPlayers[MAX_MON_FOLLOW];
	Player * pstOther;

	for (i=pstMon->iFollow-1; i>=0; i--)
	{
		pstOther = z_id_player(pstEnv->pstAppCtx,pstEnv,pstMon->astFollow[i]);
		if (pstOther && pstOther->eStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			apstPlayers[iCount++] = pstOther;
		}
	}
	// 把尾行的数据整理掉
	pstMon->iFollow = 0;

	if (iCount > 0)
	{
		CSPKG stPkg;
		CSWEIXINGSVR *pstSvr = &stPkg.Body.WeiXingSvr;

		pstSvr->WeiXingOp = WEIXING_DESTROY;
		pstSvr->WeiXingData.WeiXingPos.Map = GET_MAPID(pstMon->stMap.iID);
		pstSvr->WeiXingData.WeiXingPos.X = pstMon->stCurrPos.X;
		pstSvr->WeiXingData.WeiXingPos.Y = pstMon->stCurrPos.Y;
		Z_CSHEAD_INIT(&stPkg.Head, WEI_XING_SVR);
		z_cltmsg_limit_broadcast( pstEnv->pstAppCtx,pstEnv,apstPlayers,iCount, &stPkg );
	}
	return 0;
}

int player_weixing_clear(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	WeiXingInfo *pstWxinfo = &pstPlayer->stOnline.stWeiXing;
	int iCount = 0;
	Player * apstPlayers[MAX_WEI_XING+1];
	Player * pstOther;
	
	for (i=pstWxinfo->cWeiXingNum-1; i>=0; i--)
	{
		pstOther = z_id_player(pstEnv->pstAppCtx,pstEnv,pstWxinfo->aiWeiXingMemID[i]);
		if (pstOther && pstOther->eStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			apstPlayers[iCount++] = pstOther;
		}
		else
		{
			pstWxinfo->cWeiXingNum --;
			if (i != pstWxinfo->cWeiXingNum)
			{
				pstWxinfo->aiWeiXingMemID[i] = pstWxinfo->aiWeiXingMemID[pstWxinfo->cWeiXingNum];
			}
		}
	}
	// 把尾行的数据整理掉
	pstWxinfo->cWeiXingNum = 0;

	// 把自己的也发下,自己也不会跟谁了
	apstPlayers[iCount++] = pstPlayer;
	
	if (iCount > 0)
	{
		CSPKG stPkg;
		CSWEIXINGSVR *pstSvr = &stPkg.Body.WeiXingSvr;

		pstSvr->WeiXingOp = WEIXING_DESTROY;
		pstSvr->WeiXingData.WeiXingPos.Map = GET_MAPID(pstPlayer->stRoleData.Map);
		pstSvr->WeiXingData.WeiXingPos.X = pstPlayer->stRoleData.Pos.X;
		pstSvr->WeiXingData.WeiXingPos.Y = pstPlayer->stRoleData.Pos.Y;
		Z_CSHEAD_INIT(&stPkg.Head, WEI_XING_SVR);
		z_cltmsg_limit_broadcast( pstEnv->pstAppCtx,pstEnv,apstPlayers,iCount, &stPkg );
	}
	
	return 0;
}
ONLINEAWARDDEF* find_online_award_def(ZONESVRENV* pstEnv, int iIdx, uint iUnit)
{
	ZoneObj *pstZoneObj = pstEnv->pstZoneObj;
	UNUSED(iUnit);

	if (iIdx >= pstZoneObj->iOnlineAwardDef || iIdx < 0)
	{
		return NULL;
	}

	//return (ONLINEAWARDDEF*)(pstZoneObj->sOnlineAwardDef + iIdx*iUnit);
	return (ONLINEAWARDDEF*)(pstZoneObj->sOnlineAwardDef + iIdx);
}

int player_goto_ignore_refer_line(ZONESVRENV* pstEnv, Player *pstPlayer, int iMap,int X, int Y,int iMapPos)
{
	int iRet = 0;
	
	pstEnv->iMiscFlag |= GAME_FLAG_IGNORE_REFER_LINE_MAP;
	iRet = player_goto(pstEnv,pstPlayer,iMap,X,Y,iMapPos);
	pstEnv->iMiscFlag &= ~GAME_FLAG_IGNORE_REFER_LINE_MAP;
	return iRet;
}

int player_goto(ZONESVRENV* pstEnv, Player *pstPlayer, int iMap,int X, int Y,int iMapPos)
{
	RESPOS stPos;
	MapIndex *pstMapIdx;
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;

	stPos.X = X;
	stPos.Y = Y;
	stIdx.iID	=	iMap;
	stIdx.iPos	=	iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx)
	{
		return -1;
	}

	if (NULL == z_pos_area_dyn(pstMapIdx, pstEnv->pstAreaDynPool, pstMapInst, &stPos))
	{
		return -1;
	}

	if (0 == z_valid_pos(pstEnv->pstMapObj, iMap, &pstMapInst->iMapPos, &stPos))
	{
		return -1;
	}

	return z_player_change_map(pstEnv->pstAppCtx, pstEnv, pstPlayer, iMap, &stPos, RAND1(180), 0);
}

int player_pk_mode_chg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iMode)
{
	if(iMode < PK_MODE_NORMAL || iMode > PK_MODE_ENEMY_CLAN)
	{
		return 0;
	}
	
	pstPlayer->stOnline.ePkMode = iMode;
	pstPlayer->stRoleData.MiscInfo.PkInfo.PkMode = iMode;
	pstPlayer->stRoleData.MiscInfo.PkInfo.SavePkMode = -1;

	player_totem_update_enemy(pstEnv, pstPlayer);

	return player_pk_mode_action(pstEnv, pstPlayer, 0);
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int player_pk_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSPKCLT *pstReq = &pstCsPkg->Body.PKClt;

	UNUSED(pstFrameHead);

	switch(pstReq->PKCltType)
	{
		case PK_MODE_CHG:
			if(0 == player_if_in_span_map(pstEnv,pstPlayer))
			{
				//在跨服野外就不让切换模式了
				return 0;
			}
			
			return player_pk_mode_chg(pstAppCtx, pstEnv, pstPlayer, pstReq->PKCltData.PKMode);
			break;
		default:
			break;
	}

	return 0;
}

int player_exit_fb_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID)
{
	ZONEPWORLDINST *pstZonePworldInst;
	ZONEPWORLD *pstZonePworld;
	//int iCamp;

	// 在fb中就退出   不再就不管
	if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER166);
		return -1;
	}

	pstZonePworldInst = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);
	if (NULL == pstZonePworldInst )
	{
		return -1;
	}

	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
	if (NULL == pstZonePworld)
	{
		return -1;
	}

	player_pworld_exit_chgmap(pstEnv, pstPlayer);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(iPworldID);
}


int self_battle_info_tick(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZONEPWORLDINST *pstZonePworldInst;
	ZONEPWORLD *pstZonePworld;
	BATTLEDEF *pstBattleDef;
	ZONEMAPINST* pstMapInst;
	ZONEIDX stIdx;

	if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
	{
		return -1;
	}

	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos =	pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (NULL == pstZonePworldInst)
	{
		return -1;
	}

	if(pstZonePworldInst->bOver == 1)
	{
		return -1;
	}

	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
	if (NULL == pstZonePworld)
	{
		return -1;
	}

	//pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_SPAN_WORLD 这暂时不知道具体战场

	if(!is_normal_battle_pworld(pstZonePworld))
	{
		return -1;
	}

	pstBattleDef = z_find_battledef(pstEnv, pstZonePworld->stPworldDef.BattleID);
	if(NULL == pstBattleDef)
	{
		return -1;
	}

	switch(pstBattleDef->FightType)
	{
		case BATTLE_FIGHT_TYPE_AOSHAN:
			//目前无事可做
			return 0;
			break;
		case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		case BATTLE_FIGHT_TYPE_RAND_LIFT:
			{
				int iOldCry = pstPlayer->stOnline.stWarOL.iCrystal;
				int iOldOre = pstPlayer->stOnline.stWarOL.iOre;
				int iOldMaxLife = pstPlayer->stOnline.stWarOL.iMaxLife;
				
				if (pstPlayer->stOnline.State & CS_STAT_DEAD)
				{
					pstPlayer->stOnline.stWarOL.iDieSec ++;
					if (pstPlayer->stOnline.stWarOL.iDieSec >= 10)
					{
						pstPlayer->stOnline.stWarOL.iDieSec = 0;
						pstPlayer->stOnline.stWarOL.iCrystal --;
						pstPlayer->stOnline.stWarOL.iOre -= 2;

						if (pstPlayer->stOnline.stWarOL.iCrystal < 0)
							pstPlayer->stOnline.stWarOL.iCrystal = 0;
						if (pstPlayer->stOnline.stWarOL.iOre < 0)
							pstPlayer->stOnline.stWarOL.iOre = 0;
					}		
				}
				else
				{
					pstPlayer->stOnline.stWarOL.iCurrLife ++;
					if (pstPlayer->stOnline.stWarOL.iMaxLife < pstPlayer->stOnline.stWarOL.iCurrLife)
					{
						pstPlayer->stOnline.stWarOL.iMaxLife = pstPlayer->stOnline.stWarOL.iCurrLife;
					}
				}

				if (	iOldCry != pstPlayer->stOnline.stWarOL.iCrystal ||
					iOldOre != pstPlayer->stOnline.stWarOL.iOre	||
					iOldMaxLife != pstPlayer->stOnline.stWarOL.iMaxLife)
				{
					normal_battle_svr_info(pstEnv,pstPlayer, pstZonePworld,pstZonePworldInst, 0, 0);
				}
			}
			break;
		case BATTLE_FIGHT_TYPE_CLAN_WAR:
			{
				//目前无事可做
/*
				int iOldCry = pstPlayer->stOnline.stWarOL.iCrystal;
				int iOldOre = pstPlayer->stOnline.stWarOL.iOre;
				int iOldMaxLife = pstPlayer->stOnline.stWarOL.iMaxLife;
				
				if (pstPlayer->stOnline.State & CS_STAT_DEAD)
				{
					pstPlayer->stOnline.stWarOL.iDieSec ++;
					if (pstPlayer->stOnline.stWarOL.iDieSec >= 10)
					{
						pstPlayer->stOnline.stWarOL.iDieSec = 0;
						pstPlayer->stOnline.stWarOL.iCrystal --;
						pstPlayer->stOnline.stWarOL.iOre -= 2;

						if (pstPlayer->stOnline.stWarOL.iCrystal < 0)
							pstPlayer->stOnline.stWarOL.iCrystal = 0;
						if (pstPlayer->stOnline.stWarOL.iOre < 0)
							pstPlayer->stOnline.stWarOL.iOre = 0;
					}		
				}
				else
				{
					pstPlayer->stOnline.stWarOL.iCurrLife ++;
					if (pstPlayer->stOnline.stWarOL.iMaxLife < pstPlayer->stOnline.stWarOL.iCurrLife)
					{
						pstPlayer->stOnline.stWarOL.iMaxLife = pstPlayer->stOnline.stWarOL.iCurrLife;
					}
				}

				if (	iOldCry != pstPlayer->stOnline.stWarOL.iCrystal ||
					iOldOre != pstPlayer->stOnline.stWarOL.iOre	||
					iOldMaxLife != pstPlayer->stOnline.stWarOL.iMaxLife)
				{
					normal_battle_svr_info(pstEnv,pstPlayer, pstZonePworld,pstZonePworldInst, 0, 0);
				}

*/			
			}
			break;
		default:
			return -1;
			break;
	}

	/*
	if (	pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_CAMP_LIFT ||
		pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_RAND_LIFT ||
		pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_SPAN_WORLD)
	{
		int iOldCry = pstPlayer->stOnline.stWarOL.iCrystal;
		int iOldOre = pstPlayer->stOnline.stWarOL.iOre;
		int iOldMaxLife = pstPlayer->stOnline.stWarOL.iMaxLife;
		
		if (pstPlayer->stOnline.State & CS_STAT_DEAD)
		{
			pstPlayer->stOnline.stWarOL.iDieSec ++;
			if (pstPlayer->stOnline.stWarOL.iDieSec >= 10)
			{
				pstPlayer->stOnline.stWarOL.iDieSec = 0;
				pstPlayer->stOnline.stWarOL.iCrystal --;
				pstPlayer->stOnline.stWarOL.iOre -= 2;

				if (pstPlayer->stOnline.stWarOL.iCrystal < 0)
					pstPlayer->stOnline.stWarOL.iCrystal = 0;
				if (pstPlayer->stOnline.stWarOL.iOre < 0)
					pstPlayer->stOnline.stWarOL.iOre = 0;
			}		
		}
		else
		{
			pstPlayer->stOnline.stWarOL.iCurrLife ++;
			if (pstPlayer->stOnline.stWarOL.iMaxLife < pstPlayer->stOnline.stWarOL.iCurrLife)
			{
				pstPlayer->stOnline.stWarOL.iMaxLife = pstPlayer->stOnline.stWarOL.iCurrLife;
			}
		}

		if (	iOldCry != pstPlayer->stOnline.stWarOL.iCrystal ||
			iOldOre != pstPlayer->stOnline.stWarOL.iOre	||
			iOldMaxLife != pstPlayer->stOnline.stWarOL.iMaxLife)
		{
			z_ploy_info_chg(pstEnv, pstPlayer, SELF_RES_CHG);
		}
	}
	*/
	
	return 0;
}



int lift_ploy_pworld_msg(ZONESVRENV* pstEnv,Player *pstDie, Player *pstAtker, ZONEPWORLDINST *pstZonePworldInst, ZONEMAPINST *pstMapInst, int iFlag)
{
	WarFieldOL *pstDieOL = &pstDie->stOnline.stWarOL;
	WarFieldOL *pstAtkOL= &pstAtker->stOnline.stWarOL;
	char szMsg[64] = {0};
	char szName[ROLE_NAME_LEN];
	char szAtkerName[ROLE_NAME_LEN];

	if (iFlag == 0)
	{
		STRNCPY(szName, pstDie->stRoleData.RoleName, CCH(szName));
		battle_rolename_remove_prefix(szName);

		STRNCPY(szAtkerName, pstAtker->stRoleData.RoleName, CCH(szAtkerName));
		battle_rolename_remove_prefix(szAtkerName);
		
		if (pstDieOL->iCurrKill > SURVIVE_SUPERGOD_NUM)
		{
			snprintf(szMsg, sizeof(szMsg),LJY_SYSMSG8, 
					szAtkerName, szName);
		}
		else if (pstDieOL->iCurrKill >= SURVIVE_GOD_NUM)
		{
			snprintf(szMsg, sizeof(szMsg),LJY_SYSMSG9, 
					szAtkerName, szName);
		}
		else if (pstDieOL->iCurrKill >= SURVIVE_MON_NUM)
		{
			snprintf(szMsg, sizeof(szMsg),LJY_SYSMSG10, 
					szAtkerName, szName);
		}
		else if (pstDieOL->iCurrKill >= SURVIVE_MAN_NUM)
		{
			snprintf(szMsg, sizeof(szMsg),LJY_SYSMSG11, 
					szAtkerName, szName);
		}
		else
		{
			szMsg[0] = 0;
		}
	}
	else if (iFlag == 1)
	{
		STRNCPY(szName, pstAtker->stRoleData.RoleName, CCH(szName));
		battle_rolename_remove_prefix(szName);
		
		if (pstAtkOL->iCurrKill > 20)
		{
			snprintf(szMsg, sizeof(szMsg),LJY_SYSMSG12, szName);
		}
		else if (pstAtkOL->iCurrKill >= 15)
		{
			snprintf(szMsg, sizeof(szMsg),LJY_SYSMSG13, szName);
		}
		else if (pstAtkOL->iCurrKill >= 10)
		{
			snprintf(szMsg, sizeof(szMsg),LJY_SYSMSG14, szName);
		}
		else if (pstAtkOL->iCurrKill >= 5)
		{
			snprintf(szMsg, sizeof(szMsg),LJY_SYSMSG15, szName);
		}
		else
		{
			szMsg[0] = 0;
		}

		if(pstAtkOL->iCurrKill == 100)
		{
			z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstAtker, 3190, 
									3, 3600, 1, 0);
		}
		else if(pstAtkOL->iCurrKill == 50)
		{
			z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstAtker, 3190, 
									2, 3600, 1, 0);
		}
		else if(pstAtkOL->iCurrKill == 20)
		{
			z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstAtker, 3190, 
									1, 3600, 1, 0);
		}
	}
	else if (iFlag == 2)
	{
		STRNCPY(szName, pstAtker->stRoleData.RoleName, CCH(szName));
		battle_rolename_remove_prefix(szName);
		
		if (pstAtkOL->iKillNum == 30)
		{
			snprintf(szMsg, sizeof(szMsg),LJY_SYSMSG16, szName);
		}
		else if (pstAtkOL->iKillNum == 20)
		{
			snprintf(szMsg, sizeof(szMsg),LJY_SYSMSG17, szName);
		}
		else if (pstAtkOL->iKillNum == 10)
		{
			snprintf(szMsg, sizeof(szMsg),LJY_SYSMSG18, szName);
		}
		else
		{
			szMsg[0] = 0;
		}
	}
	

	if (szMsg[0])
	{
		CSPKG stPkg;
		CSSYSMSG *pstSysMsg = &stPkg.Body.SysMsg;


		pstSysMsg->Type = SYS_CHAT;
		pstSysMsg->Times = 0;
		pstSysMsg->Interval = 0;
		pstSysMsg->LinkLen = 0;
		STRNCPY(pstSysMsg->Msg, szMsg, sizeof(szMsg));

		Z_CSHEAD_INIT(&stPkg.Head, SYS_MSG);
		pworld_map_broadcast(pstEnv->pstAppCtx, pstEnv, pstZonePworldInst, pstMapInst, 99, &stPkg);
	}
	return 0;

}

//分配积分
int player_clan_war_battle_assign_fraction(ZONESVRENV* pstEnv, ZONEPWORLD *pstZonePworld, 
                        ZONEPWORLDINST *pstPworldInst, Player *pstPlayer)
{
	int i;
	Player *pstAssit;
	Player *apstPlayer[MAX_VISIBLE_ANIMATE];
	int iPlayerNum;
	int iFraction = 0;
	int iAssitFraction = 0;

	iPlayerNum = 0;
	for (i = 0; i < pstPlayer->stOnline.iVAnimate; i++) 
	{
		pstAssit = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.aiVAnimate[i]);
		if(NULL == pstAssit || pstAssit->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}

		if(!is_same_camp_player(pstPlayer, pstAssit))
		{
			continue;
		}

		if(CS_STAT_DEAD & pstAssit->stOnline.State)
		{
			continue;
		}
		
		apstPlayer[iPlayerNum++] = pstAssit;
	}

	//计算杀人的积分
	iFraction = ceil(9.0/(iPlayerNum+1));
	if(iFraction <=0)
	{
		iFraction = 1;
	}

	//杀人者的积分
	player_clan_war_battle_add_fraction(pstPworldInst, pstPlayer, iFraction);
	
	normal_battle_svr_info(pstEnv, pstPlayer, pstZonePworld,pstPworldInst, 0, 0);

	//计算助攻的积分
	iAssitFraction = ceil(5.0/(iPlayerNum+1));
	if(iAssitFraction <=0)
	{
		iFraction = 1;
	}

	for(i=0; i<iPlayerNum; i++)
	{
		//助攻数
		apstPlayer[i]->stOnline.stWarOL.iAssistKill++;

		//助攻积分
		player_clan_war_battle_add_fraction(pstPworldInst, apstPlayer[i], iAssitFraction);

		normal_battle_svr_info(pstEnv, apstPlayer[i], pstZonePworld,pstPworldInst, 0, 0);
	}

	return 0;
}

int player_lift_battle_assign_kill(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLD *pstZonePworld, ZONEPWORLDINST *pstPworldInst)
{
	int i;
	Player *pstAssit;
	
	for (i = 0; i < pstPlayer->stOnline.iVAnimate; i++) 
	{
		pstAssit = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.aiVAnimate[i]);
		if(NULL == pstAssit || pstAssit->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}

		if(!is_same_camp_player(pstPlayer, pstAssit))
		{
			continue;
		}

		if(CS_STAT_DEAD & pstAssit->stOnline.State)
		{
			continue;
		}
		
		pstAssit->stOnline.stWarOL.iAssistKill++;
		normal_battle_svr_info(pstEnv, pstAssit, pstZonePworld,pstPworldInst, 0, 0);
	}

	return 0;
}


int player_die_clan_war_battle_info(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstKiller,
	ZONEPWORLD *pstZonePworld, ZONEPWORLDINST *pstPworldInst, ZONEMAPINST *pstMapInst)
{
	int iCry = 0;
	int iOre = 0;
	PworldCampPlayerInfo *pstCampInfo = NULL;
	
	if (pstPlayer->stOnline.stWarOL.iCrystal > 0)
	{
		iCry = pstPlayer->stOnline.stWarOL.iCrystal *10.0/100;
		if (iCry == 0)
			iCry = 1;
	}
	if (pstPlayer->stOnline.stWarOL.iOre > 0)
	{
		iOre = pstPlayer->stOnline.stWarOL.iOre *10.0/100;
		if (iOre == 0)
			iOre = 1;
	}
		
	if (pstKiller)
	{
		char szName[ROLE_NAME_LEN];
		pstKiller->stOnline.stWarOL.iCrystal += iCry;
		pstKiller->stOnline.stWarOL.iOre += iOre;
		
		pstKiller->stOnline.stWarOL.iKillNum ++;		//	杀人数
		pstKiller->stOnline.stWarOL.iCurrKill ++;		//  	当前连杀数
		if (pstKiller->stOnline.stWarOL.iContinueKill < pstKiller->stOnline.stWarOL.iCurrKill)
		{
			pstKiller->stOnline.stWarOL.iContinueKill = pstKiller->stOnline.stWarOL.iCurrKill;	// 最大连杀数
		}

		if (pstKiller->stOnline.stWarOL.cCampIdx > 0 && 
			pstKiller->stOnline.stWarOL.cCampIdx <=MAX_CAMP_NUM)
		{
			pstCampInfo = &pstPworldInst->stPworldCamp.stCampInfo[pstKiller->stOnline.stWarOL.cCampIdx-1];
		}

		if (pstCampInfo)
		{
			pstCampInfo->iKillNum++;
			pstCampInfo->CampVal = pstCampInfo->iKillNum + pstCampInfo->iPlayerNum;
		}

		//助攻
		//player_clan_war_battle_assign_kill(pstEnv, pstKiller, pstZonePworld,pstPworldInst);
		//积分:
		player_clan_war_battle_assign_fraction(pstEnv, pstZonePworld, pstPworldInst,pstKiller);


		normal_battle_svr_info(pstEnv, pstKiller, pstZonePworld,pstPworldInst, 0, 0);

		// 终结连杀消息
		lift_ploy_pworld_msg(pstEnv,pstPlayer,pstKiller, pstPworldInst, pstMapInst, 0);

		STRNCPY(szName, pstKiller->stRoleData.RoleName, CCH(szName));
		battle_rolename_remove_prefix(szName);
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER167, szName);

		// 连杀消息和击杀数消息
		lift_ploy_pworld_msg(pstEnv,pstPlayer,pstKiller, pstPworldInst, pstMapInst,1);
		lift_ploy_pworld_msg(pstEnv,pstPlayer,pstKiller, pstPworldInst, pstMapInst, 2);

		STRNCPY(szName, pstPlayer->stRoleData.RoleName, CCH(szName));
		battle_rolename_remove_prefix(szName);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstKiller, SYS_MIDDLE2, ZONEERR_PLAYER168, szName);
	}

	pstPlayer->stOnline.stWarOL.iDieNum ++;
	pstPlayer->stOnline.stWarOL.iCurrKill = 0;
	pstPlayer->stOnline.stWarOL.iCurrLife = 0;
	pstPlayer->stOnline.stWarOL.iCrystal -= iCry;
	pstPlayer->stOnline.stWarOL.iOre -= iOre;
	pstPlayer->stOnline.stWarOL.iDieSec = 0;
	
	normal_battle_svr_info(pstEnv,pstPlayer, pstZonePworld,pstPworldInst, 0, 0);

	return 0;
}


int player_die_lift_battle_info(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstKiller,
	ZONEPWORLD *pstZonePworld, ZONEPWORLDINST *pstPworldInst, ZONEMAPINST *pstMapInst)
{
	int iCry = 0;
	int iOre = 0;
	
	if (pstPlayer->stOnline.stWarOL.iCrystal > 0)
	{
		iCry = pstPlayer->stOnline.stWarOL.iCrystal *10.0/100;
		if (iCry == 0)
			iCry = 1;
	}
	if (pstPlayer->stOnline.stWarOL.iOre > 0)
	{
		iOre = pstPlayer->stOnline.stWarOL.iOre *10.0/100;
		if (iOre == 0)
			iOre = 1;
	}
		
	if (pstKiller)
	{
		char szName[ROLE_NAME_LEN];
		pstKiller->stOnline.stWarOL.iCrystal += iCry;
		pstKiller->stOnline.stWarOL.iOre += iOre;
		
		pstKiller->stOnline.stWarOL.iKillNum ++;		//	杀人数
		pstKiller->stOnline.stWarOL.iCurrKill ++;		//  	当前连杀数
		if (pstKiller->stOnline.stWarOL.iContinueKill < pstKiller->stOnline.stWarOL.iCurrKill)
		{
			pstKiller->stOnline.stWarOL.iContinueKill = pstKiller->stOnline.stWarOL.iCurrKill;	// 最大连杀数
		}

		//助攻
		player_lift_battle_assign_kill(pstEnv, pstKiller, pstZonePworld,pstPworldInst);

		normal_battle_svr_info(pstEnv, pstKiller, pstZonePworld,pstPworldInst, 0, 0);

		// 终结连杀消息
		lift_ploy_pworld_msg(pstEnv,pstPlayer,pstKiller, pstPworldInst, pstMapInst, 0);

		STRNCPY(szName, pstKiller->stRoleData.RoleName, CCH(szName));
		battle_rolename_remove_prefix(szName);
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER167, szName);

		// 连杀消息和击杀数消息
		lift_ploy_pworld_msg(pstEnv,pstPlayer,pstKiller, pstPworldInst, pstMapInst,1);
		lift_ploy_pworld_msg(pstEnv,pstPlayer,pstKiller, pstPworldInst, pstMapInst, 2);

		STRNCPY(szName, pstPlayer->stRoleData.RoleName, CCH(szName));
		battle_rolename_remove_prefix(szName);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstKiller, SYS_MIDDLE2, ZONEERR_PLAYER168, szName);
	}

	pstPlayer->stOnline.stWarOL.iDieNum ++;
	pstPlayer->stOnline.stWarOL.iCurrKill = 0;
	pstPlayer->stOnline.stWarOL.iCurrLife = 0;
	pstPlayer->stOnline.stWarOL.iCrystal -= iCry;
	pstPlayer->stOnline.stWarOL.iOre -= iOre;
	pstPlayer->stOnline.stWarOL.iDieSec = 0;
	
	normal_battle_svr_info(pstEnv,pstPlayer, pstZonePworld,pstPworldInst, 0, 0);

	return 0;
}

int player_die_aoshan_battle_info(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstKiller,
	ZONEPWORLD *pstZonePworld, ZONEPWORLDINST *pstPworldInst, ZONEMAPINST *pstMapInst)
{
	if (pstKiller)
	{
		char szName[ROLE_NAME_LEN];
		//杀人数
		pstKiller->stOnline.stWarOL.iKillNum ++;	
		//当前连杀数
		pstKiller->stOnline.stWarOL.iCurrKill ++;		

		//最大连杀数
		if (pstKiller->stOnline.stWarOL.iContinueKill < pstKiller->stOnline.stWarOL.iCurrKill)
		{
			pstKiller->stOnline.stWarOL.iContinueKill = pstKiller->stOnline.stWarOL.iCurrKill;	
		}

		normal_battle_svr_info(pstEnv, pstKiller, pstZonePworld,pstPworldInst, 0, 0);

		// 终结连杀消息
		lift_ploy_pworld_msg(pstEnv,pstPlayer,pstKiller, pstPworldInst, pstMapInst, 0);
		
		STRNCPY(szName, pstKiller->stRoleData.RoleName, CCH(szName));
		battle_rolename_remove_prefix(szName);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER167, szName);

		// 连杀消息和击杀数消息
		lift_ploy_pworld_msg(pstEnv,pstPlayer,pstKiller, pstPworldInst, pstMapInst,1);
		lift_ploy_pworld_msg(pstEnv,pstPlayer,pstKiller, pstPworldInst, pstMapInst, 2);

		STRNCPY(szName, pstPlayer->stRoleData.RoleName, CCH(szName));
		battle_rolename_remove_prefix(szName);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstKiller, SYS_MIDDLE2, ZONEERR_PLAYER168, szName);

		//分配助攻
		player_aoshan_battle_assign_fraction(pstEnv, pstZonePworld, pstPworldInst, pstKiller);
	}

	pstPlayer->stOnline.stWarOL.iDieNum++;
	pstPlayer->stOnline.stWarOL.iCurrKill = 0;
	
	normal_battle_svr_info(pstEnv,pstPlayer, pstZonePworld,pstPworldInst, 0, 0);
	return 0;
}

int player_die_xukong_battle_info(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstKiller,
	ZONEPWORLD *pstZonePworld, ZONEPWORLDINST *pstPworldInst, ZONEMAPINST *pstMapInst)
{
	if (pstKiller)
	{
		char szName[ROLE_NAME_LEN];
		//杀人数
		pstKiller->stOnline.stWarOL.iKillNum ++;	
		//当前连杀数
		pstKiller->stOnline.stWarOL.iCurrKill ++;		

		//最大连杀数
		if (pstKiller->stOnline.stWarOL.iContinueKill < pstKiller->stOnline.stWarOL.iCurrKill)
		{
			pstKiller->stOnline.stWarOL.iContinueKill = pstKiller->stOnline.stWarOL.iCurrKill;	
		}

		normal_battle_svr_info(pstEnv, pstKiller, pstZonePworld,pstPworldInst, 0, 0);

		// 终结连杀消息
		lift_ploy_pworld_msg(pstEnv,pstPlayer,pstKiller, pstPworldInst, pstMapInst, 0);
		
		STRNCPY(szName, pstKiller->stRoleData.RoleName, CCH(szName));
		battle_rolename_remove_prefix(szName);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER167, szName);

		// 连杀消息和击杀数消息
		lift_ploy_pworld_msg(pstEnv,pstPlayer,pstKiller, pstPworldInst, pstMapInst,1);
		lift_ploy_pworld_msg(pstEnv,pstPlayer,pstKiller, pstPworldInst, pstMapInst, 2);

		STRNCPY(szName, pstPlayer->stRoleData.RoleName, CCH(szName));
		battle_rolename_remove_prefix(szName);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstKiller, SYS_MIDDLE2, ZONEERR_PLAYER168, szName);

		//分配助攻
		player_lift_battle_assign_kill(pstEnv, pstKiller, pstZonePworld, pstPworldInst);
	}

	pstPlayer->stOnline.stWarOL.iDieNum++;
	pstPlayer->stOnline.stWarOL.iCurrKill = 0;
	
	normal_battle_svr_info(pstEnv,pstPlayer, pstZonePworld,pstPworldInst, 0, 0);
	return 0;
}


int player_die_battle_info_chg(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneAni *pstAtker)
{
	ZONEPWORLDINST *pstZonePworldInst;
	ZONEPWORLD *pstZonePworld;
	Player *pstKiller = NULL;
	BATTLEDEF *pstBattleDef;
	ZONEMAPINST *pstMapInst;
	ZONEIDX stIdx;

	if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
	{
		return -1;
	}

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (NULL == pstZonePworldInst)
	{
		return -1;
	}

	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
	if (NULL == pstZonePworld || !is_normal_battle_pworld(pstZonePworld))
	{
		return -1;
	}

	pstBattleDef = z_find_battledef(pstEnv, pstZonePworld->stPworldDef.BattleID);
	if(NULL == pstBattleDef)
	{
		return -1;
	}

	pstKiller = get_atker_player(pstEnv,  pstAtker);

	switch(pstBattleDef->FightType)
	{
		case BATTLE_FIGHT_TYPE_AOSHAN:
			return player_die_aoshan_battle_info(pstEnv, pstPlayer, pstKiller, pstZonePworld, pstZonePworldInst, pstMapInst);
			break;
		case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		case BATTLE_FIGHT_TYPE_RAND_LIFT:
			return player_die_lift_battle_info(pstEnv, pstPlayer, pstKiller, pstZonePworld, pstZonePworldInst, pstMapInst);
			break;
		case BATTLE_FIGHT_TYPE_XUKONG:
			player_die_xukong_battle_info(pstEnv, pstPlayer, pstKiller, pstZonePworld, pstZonePworldInst, pstMapInst);
			break;
		case BATTLE_FIGHT_TYPE_CLAN_WAR:
			return player_die_clan_war_battle_info(pstEnv, pstPlayer, pstKiller, pstZonePworld, pstZonePworldInst, pstMapInst);
			break;
		default:
			return 0;
			break;
	}
			
	return 0;
}

int role_online_to_mud(ZONESVRENV* pstEnv, Player *pstPlayer, int iOnlineType)
{
	SSPKG stSSPkg;
	SSROLEONLINE *pstRoleOnlie = &stSSPkg.Body.RoleOnline;
	
	pstRoleOnlie->RoleID = pstPlayer->stRoleData.RoleID;
	pstRoleOnlie->Online = iOnlineType;
	STRNCPY(pstRoleOnlie->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstRoleOnlie->RoleName));
	pstRoleOnlie->AccUin = pstPlayer->stRoleData.Uin;
	pstRoleOnlie->IP = pstPlayer->stConnectInfo.stExtraInfo.IPInfo.ip;
	
	Z_SSHEAD_INIT(&stSSPkg.Head, ROLE_ONLINE);
	z_ssmsg_send(pstEnv, &stSSPkg, pstEnv->iMudSvrID);

	if (ROLE_GAME_ONLINE == iOnlineType)
	{
		pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_APEXON;
	}
	else
	{
		pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_APEXON;
	}

	return 0;
}


int player_updata_oneday_online_time(ZONESVRENV* pstEnv, ROLEGIFTINFO *pstRoleGiftInfo, int iLogin)
{
	
	int iSec;
	if (!IsSameDay(pstRoleGiftInfo->OnlineTMUpTM, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstRoleGiftInfo->OneDayOnlineTM = 0;
	}
	else
	{
		iSec =  pstEnv->pstAppCtx->stCurr.tv_sec - pstRoleGiftInfo->OnlineTMUpTM;
		if (!iLogin )
		{
			if (iSec > 0 && iSec <= 20)
				pstRoleGiftInfo->OneDayOnlineTM += iSec;
		}
	}

	pstRoleGiftInfo->OnlineTMUpTM = pstEnv->pstAppCtx->stCurr.tv_sec;

	
	return 0;
}


int role_gift_info(ZONESVRENV* pstEnv, Player *pstPlayer, int iLogin)
{
	CSPKG stCSPkg;
	CSGIFTSVR *pstGiftSvr = &stCSPkg.Body.GiftSvr;
	ROLEGIFTINFO *pstRoleGiftInfo = &pstPlayer->stRoleData.MiscInfo.RoleGiftInfo;
	GIFTINFO *pstGiftInfo;
	int i;

	// 清除过时的记录
	for (i=pstRoleGiftInfo->OneDayNum-1; i>=0; i--)
	{
		pstGiftInfo = &pstRoleGiftInfo->GiftInfoOneDay[i];
		if (	pstGiftInfo->GiftID > 0 && 
			IsSameDay(pstGiftInfo->GiftTM, pstEnv->pstAppCtx->stCurr.tv_sec) == 0)
		{
			pstRoleGiftInfo->OneDayNum --;
			if (i != pstRoleGiftInfo->OneDayNum)
			{
				memcpy(pstGiftInfo, &pstRoleGiftInfo->GiftInfoOneDay[(int)pstRoleGiftInfo->OneDayNum], sizeof(*pstGiftInfo));
			}
		}
	}

	player_updata_oneday_online_time(pstEnv, pstRoleGiftInfo,iLogin);

	memcpy(&pstGiftSvr->GiftInfo, pstRoleGiftInfo, sizeof(pstGiftSvr->GiftInfo));
	pstGiftSvr->PreCreateGift = pstPlayer->stRoleData.CtrlFlag;
	pstGiftSvr->UserType = 0;
	if (pstPlayer->stOnline.stVipBarInfo.iVipBarID)
		pstGiftSvr->UserType = USER_TYPE_INTERNET_BAR;	// 黑网吧用户

	Z_CSHEAD_INIT(&stCSPkg.Head, GIFT_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stCSPkg);
	return 0;
}

int role_cont_online_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stCSPkg;
	CSONLINEGIFTSVR *pstOnlineGiftSvr = &stCSPkg.Body.OnlineGiftSvr;
	
	pstOnlineGiftSvr->ContOnlineInfo = pstPlayer->stRoleData.MiscInfo.ContOnlineInfo;

	Z_CSHEAD_INIT(&stCSPkg.Head, CONTI_GIFT_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stCSPkg);
	return 0;
}

LVGIFT *find_lvgift(ZONESVRENV* pstEnv, int iLV)
{

	LVGIFT stLvGift;
	int iEqu,iLvGiftIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stLvGift.LV = iLV;
	iLvGiftIndex = bsearch_int(&stLvGift, pstObj->sLVGift, pstObj->iLVGift, GIFT_LV_DATA, &iEqu);
	if (iEqu)
	{
		return (LVGIFT *)&pstObj->sLVGift[iLvGiftIndex];
	}
	
	return NULL;
}

ONEDAYGIFT * find_onedaygift(ZONESVRENV* pstEnv, int iGiftID)
{
	ONEDAYGIFT stOneDayGift;
	int iEqu,iOneDayGiftIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stOneDayGift.GiftID = iGiftID;
	iOneDayGiftIndex = bsearch_int(&stOneDayGift, pstObj->sOneDayGift, pstObj->iOneDayGift, GIFT_ONEDAY_DATA, &iEqu);
	if (iEqu)
	{
		return (ONEDAYGIFT *)&pstObj->sOneDayGift[iOneDayGiftIndex];
	}
	return NULL;
}

int player_gift_lv(ZONESVRENV* pstEnv, Player *pstPlayer, int iLv)
{
	LVGIFT * pstLvGift;
	ROLEGIFTINFO *pstGiftInfo = &pstPlayer->stRoleData.MiscInfo.RoleGiftInfo;
	int i,j,iIdx;
	GIFTITEMS*pstGiftItems;
	ROLEGRID stRoleGrid[ONEDAY_GIFT_ITEM_MAX];
	ITEMDEF *pstItemDef = NULL;

	iIdx = -1;

	if (pstPlayer->stRoleData.Level < iLv)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER169);
		return -1;
	}

	if (0 == iLv)
	{
/*
		if (ROLE_PRE_FLAG != pstPlayer->stRoleData.CtrlFlag)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER170);
			return -1;
		}

*/
		//预创建礼包不在这里领了
		return 0;
	}
	else
	{
		for (i=0; i<MAX_LV_GIFT; i++)
		{
			if (pstGiftInfo->GiftLvList[i] == 0 )
				break;

			if (pstGiftInfo->GiftLvList[i] == iLv)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER171, iLv);
				return -1;
			}
		}

		iIdx = i;
		if (iIdx >= MAX_LV_GIFT)
			return -1;
	}

	pstLvGift = find_lvgift(pstEnv, iLv);
	if (!pstLvGift)
		return -1;

	if (package_add_money_test(pstEnv, pstPlayer, pstLvGift->MoneyType,pstLvGift->Money) != 0 )
	{
		//package_add_money(pstEnv, pstPlayer, iType,pstAward->Num);
		return -1;
	}
	j = 0;
	for (i=0; i<CAREER_MAX; i++)
	{
		pstGiftItems = &pstLvGift->GiftItems[i];
		//if ( pstGiftItems->Career == pstPlayer->stRoleData.Career ||
		//	 pstGiftItems->Career == 0)
		if ( (pstGiftItems->Career&0x0F) == (pstPlayer->stRoleData.Career&0x0F) ||
			 pstGiftItems->Career == 0)		
		{
			for (j=0; j<ONEDAY_GIFT_ITEM_MAX; j++)
			{
				if (  	pstGiftItems->GiftItems[j].ItemID <= 0 || 
					pstGiftItems->GiftItems[j].ItemNum <= 0)
				{
					break;
				}

				//判断物品看是否有转职限制
				pstItemDef = z_find_itemdef(pstEnv, pstGiftItems->GiftItems[j].ItemID);
				if (NULL == pstItemDef)
				{
					return -1;
				}
				// 转职后有对职业的要求
				if ( (CAREER_NON != pstItemDef->WearPre.Career && (pstPlayer->stRoleData.Career&0x0F) != (pstItemDef->WearPre.Career&0x0F)) ||
				        (CAREER_NON != (pstItemDef->WearPre.Career&0xF0) && (pstPlayer->stRoleData.Career&0xF0) != (pstItemDef->WearPre.Career&0xF0)) )
				{
					return -1;
				}
				
				if (0 == item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid[j], pstGiftItems->GiftItems[j].ItemID ) )
				{
						stRoleGrid[j].GridData.RoleItem.Num = pstGiftItems->GiftItems[j].ItemNum;
						stRoleGrid[j].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
				}
				else
				{
					return -1;
				}
			}
			break;
		}
	}

	// 策划填表有问题
	if (i >= CAREER_MAX)
		return -1;

	if (j > 0)
	{
		if (package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid[0], j,0) <0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
			return -1;
		}

		package_add(pstEnv->pstAppCtx , pstEnv , pstPlayer, &stRoleGrid[0], j,0, IMPITEM_OPTYPE_GIFT);
	}

	if (pstLvGift->Money > 0)
	{
		package_add_money(pstEnv, pstPlayer, pstLvGift->MoneyType,pstLvGift->Money) ;
		if (pstLvGift->MoneyType == ATTR_ID_BULL)
		{
			z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_LVGIFT, 0,  pstLvGift->Money, 0, "等级礼包");
		}
		else if (pstLvGift->MoneyType == ATTR_ID_MONEY)
		{
			z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_LVGIFT, 0,  0, pstLvGift->Money, "等级礼包");
		}
	}

	// j等于0的情况也是策划填表有问题  也记录一下
	if (0 == iLv)
	{
		pstPlayer->stRoleData.CtrlFlag = 0;
	}
	else
	{
		pstGiftInfo->GiftLvList[iIdx] = iLv;
	}

	// 通知一下客户端
	role_gift_info(pstEnv, pstPlayer, 0);
	z_role_gift_log(pstEnv, pstPlayer, pstPlayer->stRoleData.RoleName, GIFT_TYPE_LV, pstLvGift->Name, pstLvGift->LV,0);
	return 0;
}


int player_gift_oneday(ZONESVRENV* pstEnv, Player *pstPlayer, int iGiftID)
{
	ROLEGIFTINFO *pstGiftInfo = &pstPlayer->stRoleData.MiscInfo.RoleGiftInfo;
	ROLEGRID stRoleGrid[ONEDAY_GIFT_ITEM_MAX];
	ONEDAYGIFT *pstOneDayGift;
	int i, j;
	int iIdx = -1;
	int iLv = pstPlayer->stRoleData.Level;
	int iCareer = pstPlayer->stRoleData.Career;
	GIFTITEMS *pstGiftItems;
	ITEMDEF *pstItemDef = NULL;
	time_t tCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	ZoneClan* pstZoneClan = NULL;
	ONEDAYGIFT stDummyGift;
	int iContriExp = 0;
	int iClean = 0;
	
	pstOneDayGift = find_onedaygift(pstEnv, iGiftID);
	if (!pstOneDayGift)
		return -1;

	stDummyGift = *pstOneDayGift;

	if (pstGiftInfo->OneDayNum > MAX_ONEDAY_GIFT)
		return -1;

	// 判断同一天有没有领取过本次奖励
	for (i=0; i<pstGiftInfo->OneDayNum; i++)
	{
		if(pstGiftInfo->GiftInfoOneDay[i].GiftID == iGiftID)
		{
			if (IsSameDay(pstGiftInfo->GiftInfoOneDay[i].GiftTM, pstEnv->pstAppCtx->stCurr.tv_sec))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER172);
				return -1;
			}
			else
			{
				
				iIdx = i;
			}
		}
		else
		{
			if (pstOneDayGift->GiftCond.UserType == USER_TYPE_CLAN && pstOneDayGift->GiftCond.ClanLevel > 0)
			{
				ONEDAYGIFT *pstDayGiftDone = find_onedaygift(pstEnv, pstGiftInfo->GiftInfoOneDay[i].GiftID);
				if (pstDayGiftDone && 
					pstDayGiftDone->GiftCond.UserType == USER_TYPE_CLAN && 
					pstDayGiftDone->GiftCond.ClanLevel > 0 &&
					IsSameDay(pstGiftInfo->GiftInfoOneDay[i].GiftTM, pstEnv->pstAppCtx->stCurr.tv_sec))
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER173);
					return -1;
				}
			}
		}
	}

	if (pstOneDayGift->GiftCond.LVMin > 0 && iLv < pstOneDayGift->GiftCond.LVMin)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER174, pstOneDayGift->GiftCond.LVMin);
		return -1;
	}

	if (pstOneDayGift->GiftCond.LVMax > 0 && iLv > pstOneDayGift->GiftCond.LVMax)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER175, pstOneDayGift->GiftCond.LVMax);
		return -1;
	}

	// 每天在线时间要求
	if (pstOneDayGift->GiftCond.OnlineTM > 0)
	{
		player_updata_oneday_online_time(pstEnv, pstGiftInfo, 0);

		// 服务器容忍60秒误差
		if ((pstGiftInfo->OneDayOnlineTM+60) <pstOneDayGift->GiftCond.OnlineTM * 60)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER176, 
						pstOneDayGift->GiftCond.OnlineTM);
			return -1;
		}
	}

	// 其他条件暂时无开发需求
	if (pstOneDayGift->GiftCond.OnlineCon > 0)
	{
		// 连续上线天数要求
	}

	if (pstOneDayGift->GiftCond.PloyNum > 0)
	{
		// 完成活动次数要求
	}

	// 网吧用户要求
	if (	pstOneDayGift->GiftCond.UserType == USER_TYPE_INTERNET_BAR &&
		pstPlayer->stOnline.stVipBarInfo.iVipBarID <= 0)
	{
		return -1;
	}

	// 公会成员要求
	if (pstOneDayGift->GiftCond.UserType == USER_TYPE_CLAN)
	{		
		CLANMEM* pstMem = NULL;
		LEVELUP *pstLevelUP = NULL;
		
		pstZoneClan = player_get_clan(pstEnv,pstPlayer);
		if (!pstZoneClan)
		{
			return -1;
		}

		pstMem = clan_id_mem(pstZoneClan,pstPlayer->stRoleData.RoleID,  &pstPlayer->stOnline.iClanMemIdx);

		if (!pstMem)
		{
			return -1;
		}


		if (pstOneDayGift->GiftCond.ClanLevel > 0 &&
			pstOneDayGift->GiftCond.ClanLevel != pstZoneClan->stClan.ClanProf.Level)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER177);
			
			return -1;
		}

		if (1 == pstZoneClan->stClan.ClanProf.Level)
		{
			CLANLEVELDEF *pstDef =  z_clan_level_def(pstEnv, pstZoneClan->stClan.ClanProf.Level);
			if (pstDef && (unsigned int)pstDef->WorkingCost.Money > pstZoneClan->stClan.ClanProf.Money)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER178);
				return -1;
			}
		}		

		if (pstOneDayGift->GiftCond.ClanTouch > 0 )
		{
			if (pstZoneClan->stClan.ClanCore.YesterdayClanTouchNum < 1)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER179);
				return -1;
			}

			if (pstPlayer->stRoleData.Level < CLAN_GIFT_TOUCH_EXP_LEVEL)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER180,CLAN_GIFT_TOUCH_EXP_LEVEL);
				return -1;
			}
	

		}

		 if (pstOneDayGift->GiftCond.TotalContri > 0 &&
			pstMem->TotalContribute < (unsigned int )pstOneDayGift->GiftCond.TotalContri)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER181,
				pstOneDayGift->GiftCond.ClanCityLevel);
			return -1;
		}

 		if (pstOneDayGift->GiftCond.ClanCityLevel > 0 &&
			pstZoneClan->stClan.ClanCity.Level < pstOneDayGift->GiftCond.ClanCityLevel)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER182,
				pstOneDayGift->GiftCond.ClanCityLevel);
			return -1;
		}

		pstOneDayGift = &stDummyGift;
		pstLevelUP = z_get_levelup(pstEnv,pstPlayer->stRoleData.Level);
		if (NULL == pstLevelUP)
		{
			return -1;
		}

		iContriExp = pstLevelUP->ContributeExp;

		if (pstOneDayGift->Exp > 0)
		{
			
			//if (pstOneDayGift->GiftCond.ClanTouch)
			{
				//摸宝额外奖励经验：floor（M/120000,1)*摸宝总计次数
				int iUnit = max((int)(iContriExp / 120000.0),1);
				
				pstOneDayGift->Exp = iUnit * pstZoneClan->stClan.ClanCore.YesterdayClanTouchNum * 10;
			}
			//else
			{
				//普通的奖励
				/*
				
				基本经验：floor（M/80，1）
				公会等级影响：floor（M/400,1)*公会等级
				--公会安定度影响：floor（M/100,1)*当前安定度/10000
				今日可领取经验：基本经验 + 公会等级影响经验 + --安定度影响的经验
				*/

				int iUnit = max((int)(iContriExp / 80.0),1) +  
				max((int)(iContriExp / 400.0),1) * pstZoneClan->stClan.ClanProf.Level;
				//(int)(max((int)(iContriExp / 100.0),1) * pstZoneClan->stClan.ClanProf.StableValue / 10000.0);
				
				pstOneDayGift->Exp += iUnit;
			}
			{
				tdr_longlong llExp = pstOneDayGift->Exp;
				if (z_player_addexp_test(pstEnv,pstPlayer, &llExp) < 0)
				{
					return -1;
				}
			}
		}

		if (pstOneDayGift->ClanContri > 0)
		{
			// 	今日可领取贡献度：公会等级×10
			pstOneDayGift->ClanContri = pstZoneClan->stClan.ClanProf.Level * 10;

			// 昨天驱逐入侵提交过50分
			//if (pstPlayer->stRoleData.MiscInfo.OldDailyInfo.ClanEveryDayPworldScore.Value > 0  &&
				//IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec - 86400,
				//pstPlayer->stRoleData.MiscInfo.OldDailyInfo.ClanEveryDayPworldScore.Time))
			{
				pstOneDayGift->ClanContri += 30*pstPlayer->stRoleData.MiscInfo.OldDailyInfo.ClanEveryDayPworldScore.Value;
				iClean = 1;
			}

			//安定度影响
			if (pstZoneClan->stClan.ClanProf.StableValue > 9000)
			{
				pstOneDayGift->ClanContri += 20;
			}
			else if (pstZoneClan->stClan.ClanProf.StableValue > 7000)
			{
				pstOneDayGift->ClanContri += 10;
			}
			else if (pstZoneClan->stClan.ClanProf.StableValue > 5000)
			{
				pstOneDayGift->ClanContri += 5;
			}
			else if (pstZoneClan->stClan.ClanProf.StableValue > 0)
			{
				pstOneDayGift->ClanContri += 1;
			}

			if ((unsigned long long)pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute >= (unsigned)MAX_CLAN_OWNED_CONTRIBUT)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
				           ZONEERR_NOMORE_CONTRI);
				return -1;
			}
		}
	}

	

	// 每天连续在线时间要求
	if (pstOneDayGift->GiftCond.ContOnlineTM > 0)
	{
		// 服务器容忍60秒误差
		if (IsSameDay(tCurr, pstPlayer->stRoleData.LastLogin))     // 同一天
		{
			if ((int)(tCurr - pstPlayer->stRoleData.LastLogin+60) < pstOneDayGift->GiftCond.ContOnlineTM*60)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER183,  
							pstOneDayGift->GiftCond.ContOnlineTM);
				return -1;
			}
		}
		else
		{
			struct tm curr1;
			localtime_r(&tCurr, &curr1);
			
			if ((curr1.tm_hour-6)*3600 + curr1.tm_min*60 +curr1.tm_sec+60 < pstOneDayGift->GiftCond.ContOnlineTM*60)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER184,  
							pstOneDayGift->GiftCond.ContOnlineTM);
				return -1;
			}
		}
	}

	if (package_add_money_test(pstEnv, pstPlayer, pstOneDayGift->MoneyType,pstOneDayGift->Money) != 0 )
	{
		return -1;
	}
	
	j = 0;
	for (i=0; i<CAREER_MAX; i++)
	{
		pstGiftItems = &pstOneDayGift->GiftItems[i];
		if ( pstGiftItems->Career == 0 ||
			 (iCareer&0x0F) == (pstGiftItems->Career&0x0F) )
		{
			for (j=0; j<ONEDAY_GIFT_ITEM_MAX; j++)
			{
				if (  	pstGiftItems->GiftItems[j].ItemID <= 0 || 
					pstGiftItems->GiftItems[j].ItemNum <= 0)
				{
					break;
				}

				//判断物品看是否有转职限制
				pstItemDef = z_find_itemdef(pstEnv,pstGiftItems->GiftItems[j].ItemID);
				if (NULL == pstItemDef)
				{
					return -1;
				}
				// 转职后有对职业的要求
				if ( (CAREER_NON != pstItemDef->WearPre.Career && (pstPlayer->stRoleData.Career&0x0F) != (pstItemDef->WearPre.Career&0x0F)) ||
				        (CAREER_NON != (pstItemDef->WearPre.Career&0xF0) && (pstPlayer->stRoleData.Career&0xF0) != (pstItemDef->WearPre.Career&0xF0)) )
				{
					return -1;
				}
				
				if (0 == item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid[j], pstGiftItems->GiftItems[j].ItemID ) )
				{
						stRoleGrid[j].GridData.RoleItem.Num = pstGiftItems->GiftItems[j].ItemNum;
						stRoleGrid[j].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
				}
				else
				{
					return -1;
				}
			}
			
			break;
		}
	}

	if (i >= CAREER_MAX)
		return -1;

	if (j > 0)
	{
		if (package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid[0], j,0) <0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
			return -1;
		}

		package_add(pstEnv->pstAppCtx , pstEnv , pstPlayer, &stRoleGrid[0], j,0, IMPITEM_OPTYPE_GIFT);
	}

	if (iIdx > 0)
	{
		pstGiftInfo->GiftInfoOneDay[iIdx].GiftTM = tCurr;
	}
	else
	{
		if (pstGiftInfo->OneDayNum < MAX_ONEDAY_GIFT)
		{
			pstGiftInfo->GiftInfoOneDay[(int)pstGiftInfo->OneDayNum].GiftID = iGiftID;
			pstGiftInfo->GiftInfoOneDay[(int)pstGiftInfo->OneDayNum].GiftTM = tCurr;
			pstGiftInfo->OneDayNum ++;
		}
	}

	if (pstOneDayGift->Money > 0)
	{
		package_add_money(pstEnv, pstPlayer, pstOneDayGift->MoneyType,pstOneDayGift->Money);
		if (pstOneDayGift->MoneyType == ATTR_ID_BULL)
		{
			z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ONEDAYGIFT, 0,  pstOneDayGift->Money, 0, "每日礼包");
		}
		else if (pstOneDayGift->MoneyType == ATTR_ID_MONEY)
		{
			z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ONEDAYGIFT, 0,  0, pstOneDayGift->Money, "每日礼包");
		}
	}

	if (pstOneDayGift->ClanContri > 0 && pstZoneClan)
	{
		g_iOptype = OP_OPTYPE_GIFT;
		g_iOpsubtype = pstOneDayGift->GiftID;
		clan_resource_inc(pstEnv, pstZoneClan, pstPlayer, CLAN_OWNED_CONTRIBUT,pstOneDayGift->ClanContri);
		g_iOptype = 0;
		g_iOpsubtype = 0;
	}

	if (pstOneDayGift->Exp)
	{
		player_add_exp(pstEnv,pstPlayer,pstOneDayGift->Exp,0);
	}

	if (iClean)
	{
		pstPlayer->stRoleData.MiscInfo.OldDailyInfo.ClanEveryDayPworldScore.Value = 0;
	}

	//完成公会礼包任务
	if(pstOneDayGift->GiftCond.UserType == USER_TYPE_CLAN)
	{
		task_clan_gift_trace(pstEnv,pstPlayer);

		//成长之路
		{
			ACHIEVEMENTCOND stCond;

			memset(&stCond,0,sizeof(stCond));
			
			stCond.Type = COND_CLAN_GIFT;

			z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
		}
	}
	
	// 通知一下客户端
	role_gift_info(pstEnv, pstPlayer, 0);
	z_role_gift_log(pstEnv, pstPlayer, pstPlayer->stRoleData.RoleName, GIFT_TYPE_ONEDAY, pstOneDayGift->Name, pstOneDayGift->GiftID,0);
	return 0;
}

int player_conti_day_gift(ZONESVRENV* pstEnv, Player *pstPlayer, int iGiftID)
{
	CONTONLINEGIFTDEF *pstDef;
	MISCCONTONLINEINFO *pstContOnlineInfo = &pstPlayer->stRoleData.MiscInfo.ContOnlineInfo;
	int i,j;
	CONTONLINEINFO *pstInfo = NULL;
	ROLEGRID stRoleGrid[ONEDAY_GIFT_ITEM_MAX];
	int iCareer = pstPlayer->stRoleData.Career;
	GIFTITEMS *pstGiftItems;
	ITEMDEF *pstItemDef = NULL;

	pstDef = 	z_find_contonlinedef(pstEnv, iGiftID);
	if (!pstDef)
	{
		return -1;
	}

	pstInfo = NULL;
	for (i=0; i<pstContOnlineInfo->Num; i++)
	{
		pstInfo = &pstContOnlineInfo->ContOnlineInfo[i];
		if (!pstInfo)
		{
			return -1;
		}
		
		if (pstInfo->GiftID == iGiftID)
		{
			break;
		}
		pstInfo = NULL;
	}

	if (!pstInfo || i>=pstContOnlineInfo->Num )
	{
		return -1;
	}

	if (!pstInfo->IsGift )
		return -1;

	if (package_add_money_test(pstEnv, pstPlayer, pstDef->MoneyType,pstDef->Money) != 0 )
	{
		return -1;
	}

	j = 0;
	for (i=0; i<CAREER_MAX; i++)
	{
		pstGiftItems = &pstDef->GiftItems[i];
		if ( pstGiftItems->Career == 0 ||
			 (iCareer&0x0F) == (pstGiftItems->Career&0x0F) )
		{
			for (j=0; j<ONEDAY_GIFT_ITEM_MAX; j++)
			{
				if (  	pstGiftItems->GiftItems[j].ItemID <= 0 || 
					pstGiftItems->GiftItems[j].ItemNum <= 0)
				{
					break;
				}

				//判断物品看是否有转职限制
				pstItemDef = z_find_itemdef(pstEnv,pstGiftItems->GiftItems[j].ItemID);
				if (NULL == pstItemDef)
				{
					return -1;
				}
				// 转职后有对职业的要求
				if ( (CAREER_NON != pstItemDef->WearPre.Career && (pstPlayer->stRoleData.Career&0x0F) != (pstItemDef->WearPre.Career&0x0F)) ||
				        (CAREER_NON != (pstItemDef->WearPre.Career&0xF0) && (pstPlayer->stRoleData.Career&0xF0) != (pstItemDef->WearPre.Career&0xF0)) )
				{
					return -1;
				}
				
				if (0 == item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid[j], pstGiftItems->GiftItems[j].ItemID ) )
				{
						stRoleGrid[j].GridData.RoleItem.Num = pstGiftItems->GiftItems[j].ItemNum;
						stRoleGrid[j].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
				}
				else
				{
					return -1;
				}
			}
			
			break;
		}
	}

	if (i >= CAREER_MAX)
		return -1;

	if (j > 0)
	{
		if (package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid[0], j,0) <0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
			return -1;
		}

		package_add(pstEnv->pstAppCtx , pstEnv , pstPlayer, &stRoleGrid[0], j,0,IMPITEM_OPTYPE_GIFT);
	}

	if (pstDef->Money > 0)
	{
		package_add_money(pstEnv, pstPlayer, pstDef->MoneyType,pstDef->Money);
		if (pstDef->MoneyType == ATTR_ID_BULL)
		{
			z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ONEDAYGIFT, 0,  pstDef->Money, 0, "每日礼包");
		}
		else if (pstDef->MoneyType == ATTR_ID_MONEY)
		{
			z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_ONEDAYGIFT, 0,  0, pstDef->Money, "每日礼包");
		}
	}
	
	pstInfo->ContDay = 0;
	pstInfo->UpDayTm = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstInfo->IsGift = 0;
	pstInfo->NextFlag = NOT_GIFT;
	if (!pstDef->IsNext)
		pstInfo->NextFlag = ONCE_GIFT_NOT_NEXT;

	role_cont_online_info(pstEnv, pstPlayer);
	z_role_gift_log(pstEnv, pstPlayer, pstPlayer->stRoleData.RoleName, GIFT_TYPE_CONTI, pstDef->Name, pstDef->GiftID,0);	
	return 0;
}


int player_acc_gift_req(ZONESVRENV* pstEnv, Player *pstPlayer, int iID)
{
	SSACCGIFTLISTREQ stReq;

	stReq.Uin = pstPlayer->stRoleData.Uin;
	stReq.ID = iID;
	stReq.RoleID = pstPlayer->stRoleData.RoleID;
	STRNCPY(stReq.Pass9Account, pstPlayer->szAccount, sizeof(stReq.Pass9Account));
	STRNCPY(stReq.RoleName, pstPlayer->stRoleData.RoleName, sizeof(stReq.RoleName));
	
	shopping_ss_send(pstEnv->pstAppCtx,pstEnv,  ACC_GIFT_LIST_REQ,(char *)&stReq,sizeof(stReq));
	return 0;
}

int player_acc_gift_insert_req(ZONESVRENV* pstEnv, Player *pstPlayer, int iID)
{
	SSACCGIFTINSERTREQ stReq;
	int iLv = 0;//预创建礼包的等级为0
	LVGIFT * pstLvGift;
	int i;
	

	pstLvGift = find_lvgift(pstEnv, iLv);
	if (!pstLvGift)
	{
		return -1;
	}

	memset(&stReq,0,sizeof(stReq));
	

	stReq.Uin = pstPlayer->stRoleData.Uin;
	stReq.ID = iID;
	stReq.RoleID = pstPlayer->stRoleData.RoleID;
	STRNCPY(stReq.Pass9Account, pstPlayer->szAccount, sizeof(stReq.Pass9Account));
	STRNCPY(stReq.RoleName, pstPlayer->stRoleData.RoleName, sizeof(stReq.RoleName));
	stReq.AccGift.Uin = stReq.Uin;
	stReq.AccGift.ID = 0;
	stReq.AccGift.RoleID = stReq.RoleID;
	
	STRNCPY(stReq.AccGift.Name,pstLvGift->Name,sizeof(stReq.AccGift.Name));
	STRNCPY(stReq.AccGift.GiftDesc,pstLvGift->GiftDesc,sizeof(stReq.AccGift.GiftDesc));
	
	for(i = 0; i < ONEDAY_GIFT_ITEM_MAX;i++)
	{
		if (i >= MAX_ACC_GIFT_LIST)
		{
			break;
		}
		
		if (0 == pstLvGift->GiftItems[0].GiftItems[i].ItemID)
		{
			break;
		}
		
		stReq.AccGift.GiftItems[i] = pstLvGift->GiftItems[0].GiftItems[i];
	}
	
	shopping_ss_send(pstEnv->pstAppCtx,pstEnv,  ACC_GIFT_INSERT_REQ,(char *)&stReq,sizeof(stReq));
	return 0;
}

int player_acc_giftinfo_req_broadcast(ZONESVRENV* pstEnv)
{
	SSACCGIFTLISTREQ stReq;
	Player *pstPlayer=NULL;
	int i;
	TMEMBLOCK *pstMemBlock;

	for (i=0; i<MAX_PLAYER; i++)
	{
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (Player *)pstMemBlock->szData;
		if ( (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus ) )
		{
			continue;
		}
		stReq.Uin = pstPlayer->stRoleData.Uin;
		stReq.ID = 0;
		stReq.RoleID = pstPlayer->stRoleData.RoleID;
		STRNCPY(stReq.Pass9Account, pstPlayer->szAccount, sizeof(stReq.Pass9Account));
		STRNCPY(stReq.RoleName, pstPlayer->stRoleData.RoleName, sizeof(stReq.RoleName));
		shopping_ss_send(pstEnv->pstAppCtx,pstEnv,  ACC_GIFT_LIST_REQ,(char *)&stReq,sizeof(stReq));
	}
	
	return 0;
}

int player_prop_gift_req(ZONESVRENV* pstEnv, Player *pstPlayer, int iID,tdr_ulonglong llCashUp)
{
	SSPROPGIFTLISTREQ stReq;

	stReq.ID = iID;
	stReq.CashUp = llCashUp;
	stReq.Uin = pstPlayer->stRoleData.Uin;
	stReq.RoleID = pstPlayer->stRoleData.RoleID;
	STRNCPY(stReq.Pass9Account, pstPlayer->szAccount, sizeof(stReq.Pass9Account));
	STRNCPY(stReq.RoleName, pstPlayer->stRoleData.RoleName, sizeof(stReq.RoleName));

	shopping_ss_send(pstEnv->pstAppCtx,pstEnv,  PROP_GIFT_REQ,(char *)&stReq,sizeof(stReq));
	return 0;
}

int player_onlinegiftgroup_req(ZONESVRENV* pstEnv, Player *pstPlayer, int iID)
{
	int i;
	GIFTGROUPS *pstGiftGroups = &pstPlayer->stRoleData.MiscInfo.GiftGroups;  
	GIFTGROUP *pstGift = NULL;
	ONLINEGIFTGROUP *pstDef;
	
	for (i=0; i<pstGiftGroups->Num; i++)
	{
		if (pstGiftGroups->GiftGroups[i].ID == iID)
		{
			pstGift = &pstGiftGroups->GiftGroups[i];
			break;
		}
	}

	if (!pstGift ||pstGift->GetFlag)
	{
		return -1;
	}

	pstDef = z_find_onlinegroupgift(pstEnv, pstGift->ID);
	if (!pstDef)
		return -1;
	
	for (i=pstDef->OneNum-1; i>=0; i--)
	{
		if (pstGift->ContDay >= pstDef->GiftOnes[i].Day)
		{
			pstGift->GetFlag = 1;
			
			// 如果是最后一个可以领取的奖励的了  领完从头开始
			// 这里注释掉 不从头开始了
			/*if(i == pstDef->OneNum-1)
			{
				pstGift->ContDay = 0;
			}*/
			
			giftgroup_res_to_clt( pstEnv, pstPlayer,&pstDef->GiftOnes[i], iID);
			z_role_gift_log(pstEnv, pstPlayer, pstPlayer->stRoleData.RoleName, GIFT_TYPE_CONTI_GROUP, pstDef->Name, pstDef->ID,0);	
			return 0;
		}	
	}
	return 0;
}


int player_new_role_gift_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	
		
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstRes = &stPkg.Body.PlayerMiscSvr;
	CSNEWGIFTINFO *pstInfoRes = &pstRes->PlayerMiscSvrData.NewGiftInfo;
	pstRes->PlayerMiscType = PLAYER_SVR_NEW_GIFT_INFO;

	pstInfoRes->NewRoleGiftInfo = pstPlayer->stRoleData.MiscInfo.NewRoleGiftInfo;
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_new_role_gift_req(ZONESVRENV* pstEnv, Player *pstPlayer, int iID)
{
	NEWROLEGIFTINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.NewRoleGiftInfo; 
	NEWROLEGIFT *pstDef;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	ROLEGRID stGrid[MAX_NEW_ROLE_GIFT_ITEM];
	int i;
	int iNum = 0;

	if (pstInfo->ID != iID)
	{
		return -1;
	}

	if (pstInfo->NewRole == 0)
	{
		return -1;
	}
	
	pstDef = new_role_gift_find( pstEnv, iID);
	if (!pstDef)
	{
		return -1;
	}

	// 误差10秒
	if (pstInfo->End - iCurr > 10)
	{
		return -1;
	}

	for (i=0; i<MAX_NEW_ROLE_GIFT_ITEM; i++)
	{
		if (pstDef->Items[i].ItemID <=0 || pstDef->Items[i].ItemNum <= 0)
		{
			break;
		}

		if (item_create(pstEnv->pstAppCtx, pstEnv, &stGrid[iNum], pstDef->Items[i].ItemID) < 0)
		{
			return -1;
		}	

		stGrid[iNum].GridData.RoleItem.Num = pstDef->Items[i].ItemNum;
		stGrid[iNum].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
		iNum ++;
	}

	if (iNum > 0)
	{
		if (package_add_test(pstEnv->pstAppCtx,pstEnv,pstPlayer, &stGrid[0], iNum, 0) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
			return -1;
		}
		else
		{
			package_add(pstEnv->pstAppCtx,pstEnv,pstPlayer, &stGrid[0], iNum, 0, 0);	
		}
	}

	pstDef = new_role_gift_find( pstEnv, iID+1);
	if (pstDef)
	{
		pstInfo->ID = iID+1;
		pstInfo->End = iCurr+pstDef->Sec;
		pstInfo->Sec = pstDef->Sec;
	}
	else
	{
		pstInfo->NewRole = 0;
	}

	player_new_role_gift_info(pstEnv, pstPlayer);
	return 0;
}

int player_cont_cover_gift_notify(ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID, int iNum)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstRes = &stPkg.Body.PlayerMiscSvr;
	CSCONTCOVERGIFTINFO *pstInfoRes = &pstRes->PlayerMiscSvrData.ContCoverGiftInfo;
	pstRes->PlayerMiscType = PLAYER_SVR_CONT_COVER_INFO;

	pstInfoRes->ItemID = iItemID;
	pstInfoRes->ItemNum = iNum;
	pstInfoRes->ContCoverGiftInfo = pstPlayer->stRoleData.MiscInfo.ContCoverGiftInfo;
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_cont_cover_up(ZONESVRENV* pstEnv, Player *pstPlayer, int iNotify)
{
	CONTCOVERGIFTINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.ContCoverGiftInfo;
	int iChg = 0;

	if (pstEnv->pstAppCtx->stCurr.tv_sec - pstInfo->PreTime > 48*60*60)
	{
		pstInfo->Num = 0;
		pstInfo->PreTime = 0;
		iChg = 1;	
	}
	else
	{
		// 隔2天了清0
		if (IsSameDay(pstInfo->PreTime+48*60*60, pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			pstInfo->Num = 0;
			pstInfo->PreTime = 0;
			iChg = 1;
		}

		// 隔天了通知客户端可以领取
		else if (IsSameDay(pstInfo->PreTime+24*60*60, pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			iChg  = 1;	
		}
	}
	
	if (iChg || iNotify)
		player_cont_cover_gift_notify(pstEnv, pstPlayer, 0,0);
	return 0;
}

int player_cont_cover_gift_req(ZONESVRENV* pstEnv, Player *pstPlayer, int iDay)
{
	CONTCOVERGIFTINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.ContCoverGiftInfo;
	//int iNum = pstInfo->Num;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	CONTCOVERGIFT *pstDef;
	ROLEGRID stGrid;
	int iIdx = -1;
	int i;
	int iRate = 0;
	int iCountRate = 0;

	if (IsSameDay(pstInfo->PreTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		return -1;
	}
	
	if (iDay != pstInfo->Num +1)
	{
		return -1;
	}
	
	if (iDay > pstObj->iContCoverGiftDef)
	{
		iDay = pstObj->iContCoverGiftDef;
	}

	pstDef = cont_cover_gift_find(pstEnv,  iDay);
	if (!pstDef)
	{
		return 0;
	}

	for (i=0; i<MAX_CONT_COVER_GIFT_ITEM; i++)
	{
		if (pstDef->Items[i].ItemID <= 0 || pstDef->Items[i].Num <= 0)
		{
			break;
		}

		iCountRate += pstDef->Items[i].Rate;
	}

	if (iCountRate <= 0)
	{
		return -1;
	}

	iRate  = RAND1(iCountRate);
	iCountRate = 0;
	
	for (i=0; i<MAX_CONT_COVER_GIFT_ITEM; i++)
	{
		if (pstDef->Items[i].ItemID <= 0 || pstDef->Items[i].Num <= 0)
		{
			break;
		}

		iCountRate += pstDef->Items[i].Rate;
		if (iRate < iCountRate)
		{
			iIdx = i;
			break;
		}
	}

	if (iIdx < 0 || iIdx >= MAX_CONT_COVER_GIFT_ITEM)
	{
		return -1;
	}
	
	if (item_create(pstEnv->pstAppCtx, pstEnv, &stGrid, pstDef->Items[iIdx].ItemID) < 0)
	{
		return -1;
	}	

	stGrid.GridData.RoleItem.Num = pstDef->Items[iIdx].Num;
	stGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;

	if (package_add_test(pstEnv->pstAppCtx,pstEnv,pstPlayer, &stGrid, 1, 0) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
		return -1;
	}
	else
	{
		package_add(pstEnv->pstAppCtx,pstEnv,pstPlayer, &stGrid, 1, 0, IMPITEM_OPTYPE_CONT_COVER_GIFT);	
	}

	pstInfo->Num++;	
	pstInfo->PreTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstInfo->PreItemID = pstDef->Items[iIdx].ItemID;

	player_cont_cover_gift_notify(pstEnv, pstPlayer, pstDef->Items[iIdx].ItemID, pstDef->Items[iIdx].Num);
	return 0;
}

int player_gift(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						     TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{	
	CSGIFTCLT *pstGiftClt = &pstCsPkg->Body.GiftClt;
	UNUSED(pstFrameHead);
	UNUSED(pstAppCtx);

	if( pstEnv->iInstMode != INST_MODE_ZONE_SVR )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER140);
		return 0;
	}

	if (pstPlayer->stOnline.State & CS_STAT_STALL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER105);
		return 0;
	}

	if(pstPlayer->stOnline.stExchgOnline.bExchgStat > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER106);
		return 0;
	}
	
	if (pstGiftClt->GiftType == GIFT_TYPE_LV)
	{
		return player_gift_lv(pstEnv, pstPlayer, pstGiftClt->CltData.LV);
	}
	else if (pstGiftClt->GiftType == GIFT_TYPE_ONEDAY)
	{
		return player_gift_oneday(pstEnv, pstPlayer, pstGiftClt->CltData.GiftID);
	}
	else if (pstGiftClt->GiftType == GIFT_TYPE_CONTI)
	{
		return player_conti_day_gift(pstEnv, pstPlayer, pstGiftClt->CltData.ContGiftID);
	}
	else if (pstGiftClt->GiftType == GIFT_TYPE_ACCGIFT)
	{
		if (pstGiftClt->CltData.AccGiftID <= 0)
			return -1;
		return player_acc_gift_req(pstEnv, pstPlayer, pstGiftClt->CltData.AccGiftID);
		
	}
	else if (pstGiftClt->GiftType == GIFT_TYPE_ACCGIFT_RELOAD)
	{
		return player_acc_gift_req(pstEnv, pstPlayer, 0);
	}
	else if (pstGiftClt->GiftType == GIFT_TYPE_PROPGIFT)
	{
		if (pstGiftClt->CltData.PropGiftIDInfo.GroupID <=0)
			return -1;
		return player_prop_gift_req(pstEnv, pstPlayer, pstGiftClt->CltData.PropGiftIDInfo.GroupID,pstGiftClt->CltData.PropGiftIDInfo.CashUp);
	}
	else if (pstGiftClt->GiftType == GIFT_TYPE_CONTI_GROUP)
	{
		if (	pstGiftClt->CltData.ContiGroup.ID <= 0 ||
			pstGiftClt->CltData.ContiGroup.Day <=0)
			return -1;
		return player_onlinegiftgroup_req(pstEnv, pstPlayer, pstGiftClt->CltData.ContiGroup.ID);
	}
	else if (pstGiftClt->GiftType == GIFT_TYPE_CDKEY)
	{
		if (pstGiftClt->CltData.CDkey[0]  == 0)
			return -1;
		
		return player_cdkey_req(pstEnv, pstPlayer, pstGiftClt->CltData.CDkey);
	}
	else if (pstGiftClt->GiftType == GIFT_TYPE_NEW_ROLE)
	{
		return player_new_role_gift_req(pstEnv, pstPlayer, pstGiftClt->CltData.ID);
	}
	else if (pstGiftClt->GiftType == GIFT_TYPE_CONT_COVER)
	{
		return player_cont_cover_gift_req( pstEnv, pstPlayer, pstGiftClt->CltData.ContinueDay);		
	}
	else
	{
		return -1;
	}
	
	return 0;
}



int player_action_res_ex(ZONESVRENV *pstEnv, Player *pstPlayer, CSPKG *pstPkg)
{

	/*if( pstPkg->Body.ActionRes.ActionID == ACTION_TEAM &&
		pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN )
	{
		return -1;
	}*/
		
	Z_CSHEAD_INIT(&pstPkg->Head, ACTION_RES);
	
	if(pstPlayer->eStatus == PLAYER_STATUS_ROLE_NOTHING)
	{
		return player_shadow_action_request(pstEnv, pstPlayer,
		                                    &pstPkg->Body.ActionRes);
	}
	else
	{
		if(z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, pstPkg, 0) < 0)
		{
			return -1;
		}
	}
	
	return 0;
}

int player_career_chg(TAPPCTX*pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer, int  CarrerChgType,int CheckUIFlag)
{
	// 参数1: 转职的职业类型 请参考tagCAREER_TYPE
	// 参数2: 积累数技能槽的上限
	// 参数3: 积累数技能槽的增加方式
	// 参数4: 返回值0为成功,<0为失败 -2表示已经转职过了

	int iCareer = 0;
	int iCollectSkill_SlotMax = 1000;
	int iCollectSkill_AddType = 0;
	int iLevel = 0;
	LEVELUP *pstLevelUp = NULL;
	int iNum = 0;

	iCareer = CarrerChgType;
	
	if (pstEnv->iInstMode != INST_MODE_ZONE_SVR)
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_BATTLE_SVR_OP);
		return -1;
	}

	if (pstEnv->pstConf->IsMatchWorld)
	{
		return -1;
	}

	/*if (CheckUIFlag && check_npc_ui(pstEnv, pstPlayer, UI_TYPE_CAREER_CHG) < 0)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, "你离NPC太远了");
		return -1;
	}*/
	iNum =z_get_pak_item_num(pstEnv, &pstPlayer->stRoleData.Package, CAREERCHG_ITEMID, -1,0,PACKAGE_DEC_NONE);
	if (iNum <= 0 )
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_PLAYER141);
		return 0;
	}
		

	if( pstPlayer->stRoleData.Career > CAREER_CHURCH )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_PLAYER142);
		return -1;
	}
		
	if(pstPlayer->stRoleData.Level != CAREERCHG_LEVEL )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_PLAYER143);
		return -1;
	}

	iLevel = pstPlayer->stRoleData.Level;
	pstLevelUp = (LEVELUP *)pstEnv->pstZoneObj->sLevelUpDef[iLevel-1];
	if(pstPlayer->stRoleData.RoleDetail.Exp <  (int) pstLevelUp->LevelNextExp)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_PLAYER144);
		return -1;
	}
		
			
      if( CAREER_WARRIOR_SINGLEHAND != iCareer && CAREER_WARRIOR_DOUBLEHAND != iCareer &&
   	   CAREER_MAGICOR_SINGLEHAND != iCareer && CAREER_MAGICOR_DOUBLEHAND != iCareer &&
   	   CAREER_GUNER_SINGLEHAND     != iCareer && CAREER_GUNER_DOUBLEHAND     != iCareer &&
   	   CAREER_CHURCH_SINGLEHAND   != iCareer && CAREER_CHURCH_DOUBLEHAND   != iCareer )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_PLAYER145);
		return -1;
	}
  
	if( pstPlayer->stRoleData.Career == CAREER_WARRIOR && 
		CAREER_WARRIOR_SINGLEHAND != iCareer && 
		CAREER_WARRIOR_DOUBLEHAND != iCareer  )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_PLAYER145);
		return -1;
	}
	if( pstPlayer->stRoleData.Career == CAREER_MAGICOR && 
		CAREER_MAGICOR_SINGLEHAND != iCareer && 
		CAREER_MAGICOR_DOUBLEHAND != iCareer  )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_PLAYER145);
		return -1;
	}
	if( pstPlayer->stRoleData.Career == CAREER_GUNER && 
		CAREER_GUNER_SINGLEHAND != iCareer && 
		CAREER_GUNER_DOUBLEHAND != iCareer )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_PLAYER145);
		return -1;
	}
	if(pstPlayer->stRoleData.Career == CAREER_CHURCH && 
		CAREER_CHURCH_SINGLEHAND != iCareer && 
		CAREER_CHURCH_DOUBLEHAND != iCareer)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_PLAYER145);
		return -1;
	}

	if( iCareer == CAREER_WARRIOR_SINGLEHAND )
	{
		iCollectSkill_AddType = COLLECTSKILL_ADDTYPE_AKEE;
	}
	else if( iCareer == CAREER_WARRIOR_DOUBLEHAND )
	{
		iCollectSkill_AddType = COLLECTSKILL_ADDTYPE_AKER;
	}
	else if( iCareer == CAREER_MAGICOR_DOUBLEHAND ||
			 iCareer == CAREER_MAGICOR_SINGLEHAND ||
			 iCareer == CAREER_CHURCH_SINGLEHAND  ||
			 iCareer == CAREER_CHURCH_DOUBLEHAND   )
	{
		iCollectSkill_AddType = COLLECTSKILL_ADDTYPE_TIME;
	}
	else if( iCareer == CAREER_GUNER_SINGLEHAND	  ||
			 iCareer == CAREER_GUNER_DOUBLEHAND    )
	{
		iCollectSkill_AddType = COLLECTSKILL_ADDTYPE_VAL;
	}
	
	//转职了
	pstPlayer->stRoleData.Career = iCareer;
	pstPlayer->stRoleData.MiscInfo.CollectSkillInfo.CollectSkill_SlotMax = iCollectSkill_SlotMax;
	pstPlayer->stRoleData.MiscInfo.CollectSkillInfo.CollectSkill_AddType = iCollectSkill_AddType;

	//relation_dbbuddy_chg(pstEnv, pstPlayer, 0,1);
	
	player_update_cache_career(pstEnv, pstPlayer);

	relation_dbbuddy_chg(pstEnv, pstPlayer, 0,1);

	//记录一条转职的日志
	z_role_lvup_snapshot_oplog(pstEnv, pstPlayer, pstPlayer->stRoleData.Level, pstPlayer->stRoleData.RoleDetail.Exp,1);

	//下发个ACTION
	player_career_chg_action(pstEnv, pstPlayer);

	//转职成功扣道具
	//CAREERCHG_ITEMID
	package_dec(pstAppCtx, pstEnv, pstPlayer, CAREERCHG_ITEMID, iNum, -1, 
				NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_CAREER);
	
	//升一级
	player_adjust_level(pstAppCtx, pstEnv, pstPlayer,0);

	
	//成长之路
	{
		ACHIEVEMENTCOND stCond;
	
		memset(&stCond,0,sizeof(stCond));
		
		stCond.Type = COND_CAREER_CHG;
	
		z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
	}

	// 转职称号
	//player_desig_get(pstEnv, pstPlayer, GET_DESIG_CHG_CAREER);
	return 0;
	UNUSED(CheckUIFlag);	//add by paraunused.pl
}


int player_match_chg_career(TAPPCTX*pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer, MATCHCHGCAREER *pstChg)
{
	int iCareer = 0;
	RESULT *pstResult;
	ITEMDEF *pstItemDef;
	int iPos;
	ROLEITEM *pstRoleItem;
	int i;

	iCareer = pstChg->Career;
	if (!pstEnv->pstConf->IsMatchWorld)
	{
		return -1;
	}

	if (0 > package_get_item(pstEnv, pstPlayer, LIST_TYPE_SUNDRIES, pstChg->GridIdx, &pstItemDef, &pstRoleItem, &iPos))
	{
		return -1;
	}

	pstResult = z_item_get_result(pstItemDef, RESULT_ITEM_BISAI);
	if(!pstResult)
	{
		return -1;
	}

	if( pstPlayer->stRoleData.Career == CAREER_WARRIOR )
	{
		if (	CAREER_WARRIOR_SINGLEHAND != iCareer && 
			CAREER_WARRIOR_DOUBLEHAND != iCareer  )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_PLAYER145);
			return -1;
		}
		
	}
	else if (pstPlayer->stRoleData.Career == CAREER_MAGICOR)
	{
		if (	CAREER_MAGICOR_SINGLEHAND != iCareer && 
			CAREER_MAGICOR_DOUBLEHAND != iCareer )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_PLAYER145);
			return -1;
		}
	}
	else if (pstPlayer->stRoleData.Career == CAREER_GUNER)
	{
		if (	CAREER_GUNER_SINGLEHAND != iCareer && 
			CAREER_GUNER_DOUBLEHAND != iCareer  )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_PLAYER145);
			return -1;
		}
	}
	else if (pstPlayer->stRoleData.Career == CAREER_CHURCH)
	{
		if (	CAREER_CHURCH_SINGLEHAND != iCareer && 
			CAREER_CHURCH_DOUBLEHAND != iCareer )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_PLAYER145);
			return -1;
		}
	}
	else
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_PLAYER145);
		return -1;
	}
	
	
	pstPlayer->stRoleData.Career = iCareer;
	player_update_cache_career(pstEnv, pstPlayer);

	package_dec(pstAppCtx, pstEnv, pstPlayer, pstItemDef->ItemID, 1, LIST_TYPE_SUNDRIES, &iPos,
				PACKAGE_DEC_NONE, IMPITEM_OPTYPE_CAREER);
	player_career_chg_action(pstEnv, pstPlayer);

	for (i=pstPlayer->stRoleData.Level; i<80; i++)
	{
		player_adjust_level(pstAppCtx, pstEnv, pstPlayer,0);	
	}
	return 0;
}

int player_select_target(Player *pstPlayer, int iTargetID)
{
	pstPlayer->stOnline.iSelectTarget = iTargetID;
	return 0;
}

int player_drop_clan_tem(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	package_use_clan_item(pstEnv, pstPlayer, NULL);
	return 0;
}


int player_clan_everyday_pworld_enter(ZONESVRENV *pstEnv,Player *pstPlayer,CSPLAYERCLANEVERYDAYPWORLDENTER *pReq)
{
	clan_everyday_pworld_enter(pstEnv,pstPlayer);
	return 0;
	UNUSED(pReq);	//add by paraunused.pl
}

int player_clan_machine_pworld_enter(ZONESVRENV *pstEnv,Player *pstPlayer,CSPLAYERCLANMACHINEPWORLDENTER *pReq)
{
	clan_machine_pworld_enter(pstEnv,pstPlayer, pReq->ClanID);
	return 0;
	UNUSED(pReq);	//add by paraunused.pl
}

int player_award_by_def(ZONESVRENV *pstEnv,Player *pstPlayer,int iAwardDefID,
						char * pszTip,int iTipType,int iErrTip,int iTest)
{
	PLAYERAWARDDEF* pstDef = NULL;
	PlayerAwardDefTail* pstTail = NULL;
	int i = 0;	
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	PLAYERAWARDDEF* pstAward = NULL;
	ROLEGRID stRoleGrid[PLAYER_AWARD_ITEM_MAX];
	int iItemNum = 0;
	char szTip[512] = {0};
	
	char szMoneyTip1[64] = {0};
	char szMoneyTip2[64] = {0};
	
	char *pszMoneyTip = szMoneyTip1;
	char *pszMoneyTipTmp = szMoneyTip2;	

	
	char szExpTip[64] = {0};
	char szItemIip1[64] = {0};
	char szItemIip2[64] = {0};
	
	char *pszItemTip = szItemIip1;
	char *pszItemTipTmp = szItemIip2;

	pstDef = z_find_player_award_def(pstEnv,iAwardDefID);
	if (!pstDef)
	{
		if (iErrTip)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_PLAYER146);
		}
		return -1;
	}

	pstTail = z_get_player_award_tail(pstDef);

	if (!pstTail)
	{
		return -1;
	}

	if (iTest)
	{
		int iRet = 0;
		char * pszTmp = NULL;
		for( i = 0; i < pstTail->iPosCount; i++)
		{
			int idx = i + pstTail->iPosStart;
			
			char szName[32] = {0};
			
			pstAward = (PLAYERAWARDDEF*)pstObj->sPlayerAwardDef[idx];
			
			if (pstAward->Type == PLAYER_AWARD_TYPE_BULL)
			{
				money_type_to_string(ATTR_ID_BULL,szName,sizeof(szName));
				iRet = package_add_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, pstAward->Value);
				if (0 >  iRet)
				{
					if (iErrTip)
					{					
						
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,  SYS_TIP, ZONEERR_PLAYER147,szName);				
					}
					return -1;
				}
				
				pszTmp = pszMoneyTip;
				pszMoneyTip = pszMoneyTipTmp;
				pszMoneyTipTmp = pszTmp;
				
				snprintf(pszMoneyTip,sizeof(szMoneyTip1),"%s %s:%d",pszMoneyTipTmp,szName,pstAward->Value);
			}
			else if (pstAward->Type == PLAYER_AWARD_TYPE_MONEY)
			{
				money_type_to_string(ATTR_ID_MONEY,szName,sizeof(szName));
				iRet = package_add_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, pstAward->Value);
				if (0 >  iRet)
				{
					if (iErrTip)
					{
						
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,  SYS_TIP, ZONEERR_PLAYER147,szName);				
					}
					return -1;
				}
				pszTmp = pszMoneyTip;
				pszMoneyTip = pszMoneyTipTmp;
				pszMoneyTipTmp = pszTmp;
				
				snprintf(pszMoneyTip,sizeof(szMoneyTip1),"%s %s:%d",pszMoneyTipTmp,szName,pstAward->Value);
			}
			else if (pstAward->Type == PLAYER_AWARD_TYPE_EXP)
			{
				long long iExp = pstAward->Value;
				iRet = z_player_addexp_test(pstEnv, pstPlayer, &iExp);
				if (0 >  iRet)
				{
					return -1;
				}

				snprintf(szExpTip,sizeof(szExpTip)," %s:%d",LJY_EXP, pstAward->Value);
				
			}
			else if (pstAward->Type == PLAYER_AWARD_TYPE_BIND_GOD_COIN)
			{
				money_type_to_string(ATTR_ID_BINDGODCOIN,szName,sizeof(szName));
				iRet = package_add_money_test(pstEnv, pstPlayer, ATTR_ID_BINDGODCOIN, pstAward->Value);
				if (0 >  iRet)
				{
					if (iErrTip)
					{
						
						z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer,  SYS_TIP, ZONEERR_PLAYER147,szName);				
					}
					return -1;
				}
				pszTmp = pszMoneyTip;
				pszMoneyTip = pszMoneyTipTmp;
				pszMoneyTipTmp = pszTmp;
				
				snprintf(pszMoneyTip,sizeof(szMoneyTip1),"%s %s:%d",pszMoneyTipTmp,szName,pstAward->Value);
			}
			else if (pstAward->Type == PLAYER_AWARD_TYPE_ITEM)
			{
				ITEMDEF *pstItemDef  = z_find_itemdef(pstEnv,  pstAward->ItemID);
				if ((iItemNum < PLAYER_AWARD_ITEM_MAX) && 
					(0 == item_create(pstEnv->pstAppCtx,pstEnv, &stRoleGrid[iItemNum], pstAward->ItemID)) &&
					pstItemDef)
				{
					stRoleGrid[iItemNum].GridData.RoleItem.Num = pstAward->Value;
					
					if(pstItemDef->MaxPile && stRoleGrid[iItemNum].GridData.RoleItem.Num > pstItemDef->MaxPile)
					{
						stRoleGrid[iItemNum].GridData.RoleItem.Num = pstItemDef->MaxPile;
					}
					
					iItemNum++;



					pszTmp = pszItemTip;
					pszItemTip = pszItemTipTmp;
					pszItemTipTmp = pszTmp;
					
					snprintf(pszItemTip,sizeof(szItemIip1),"%s %sx%d",pszItemTipTmp,
								pstItemDef->Name,pstAward->Value);
				}				
			}		
		}

		if (iItemNum > 0)
		{
			iRet =package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid[0],  iItemNum,0);
			if (iRet < 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_NOSPACE);
				return -1;
			}
		}
	}

	if (iItemNum > 0)
	{
		package_add(pstEnv->pstAppCtx , pstEnv , pstPlayer, &stRoleGrid[0], iItemNum,0, IMPITEM_OPTYPE_GIFT);
	}

	for( i = 0; i < pstTail->iPosCount; i++)
	{
		int idx = i + pstTail->iPosStart;
		
		pstAward = (PLAYERAWARDDEF*)pstObj->sPlayerAwardDef[idx];
		
		if (pstAward->Type == PLAYER_AWARD_TYPE_BULL)
		{
			package_add_money(pstEnv, pstPlayer, ATTR_ID_BULL, pstAward->Value);	
			z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_AWARD_DEF, iAwardDefID,  pstAward->Value, 0, "奖励定义表奖励");
		}
		else if (pstAward->Type == PLAYER_AWARD_TYPE_MONEY)
		{
			package_add_money(pstEnv, pstPlayer, ATTR_ID_MONEY, pstAward->Value);	
			z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_AWARD_DEF, iAwardDefID,  0, pstAward->Value, "奖励定义表奖励");
		}
		else if (pstAward->Type == PLAYER_AWARD_TYPE_EXP)
		{
			player_add_exp(pstEnv,pstPlayer,pstAward->Value,0);
		}
		else if (pstAward->Type == PLAYER_AWARD_TYPE_BIND_GOD_COIN)
		{
			package_add_money(pstEnv, pstPlayer, ATTR_ID_BINDGODCOIN, pstAward->Value);
			z_bindgold_create_oplog(pstEnv, pstPlayer, BINDGOLD_OPTYPE_AWARD_DEF, iAwardDefID, pstAward->Value, "奖励定义表奖励");
		}
		else if (pstAward->Type == PLAYER_AWARD_TYPE_ITEM)
		{
			continue;
		}
	}

	if(pszTip && (0 != *pszTip))
	{
		snprintf(szTip,sizeof(szTip),"%s%s%s%s",pszTip,szExpTip,pszMoneyTip,pszItemTip);
		z_send_sys_str(pstEnv->pstAppCtx,pstEnv,pstPlayer,iTipType,szTip);	
	}
	return 0;
}


int player_take_cow_award(ZONESVRENV *pstEnv,Player *pstPlayer,CSPLAYERTAKECOWAWARD *pReq)
{
	COWFINALRANGE *pstRange = NULL;
	int iNow = pstEnv->pstAppCtx->stCurr.tv_sec;
	int i = 0;
	COWFINALRANGEMEMBER *pstMember = NULL;
	int iFound = 0;
	COWFINALAWARDDEF *pstAward = NULL;

	pstRange = &pstEnv->pstGlobalObj->stGlobal.Misc.CowFinalRange;
	iNow -= pstRange->RangeTime;
	if (iNow > COW_FINAL_AWARD_VALID_TIME || iNow < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_PLAYER185);
		return 0;
	}

	for(i = 0; i < pstRange->Num; i++)
	{
		pstMember = &pstRange->Members[i];
		if ((tdr_ulonglong)pstMember->RoleID == pstPlayer->stRoleData.RoleID)
		{
			iFound = 1;
			break;
		}
	}

	if(!iFound || !pstMember)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2,ZONEERR_PLAYER186);
		return 0;		
	}

	if(pstMember->TakeAward)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2,ZONEERR_PLAYER187);
		return 0;		
	}

	pstAward = z_find_cow_final_award_def(pstEnv,pstMember->Range);
	if (!pstAward)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_PLAYER188);
		return 0;			
	}

	if (0 == player_award_by_def(pstEnv,pstPlayer,pstAward->PlayerAwardDefID,
				pstAward->AwardTip,pstAward->AwardTipType,1,1))
	{
		pstMember->TakeAward = 1;
		player_action_cow_final_range(pstEnv,pstPlayer,1);
	}

	return 0;
	UNUSED(pReq);	//add by paraunused.pl
}

int player_take_hang_hai_award(ZONESVRENV *pstEnv,Player *pstPlayer,CSPLAYERTAKEHANGHAIAWARD *pReq)
{
	HANGHAIRANGE *pstRange = NULL;
	int iNow = pstEnv->pstAppCtx->stCurr.tv_sec;
	int i = 0;
	HANGHAIRANGEMEMBER *pstMember = NULL;
	int iFound = 0;
	HANGHAIAWARDDEF *pstAward = NULL;

	pstRange = &pstEnv->pstGlobalObj->stGlobal.Misc.HangHaiRange;
	iNow -= pstRange->RangeTime;
	if (iNow > HANG_HAI_AWARD_VALID_TIME || iNow < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_PLAYER185);
		return 0;
	}

	for(i = 0; i < pstRange->Num; i++)
	{
		pstMember = &pstRange->Members[i];
		if ((tdr_ulonglong)pstMember->RoleID == pstPlayer->stRoleData.RoleID)
		{
			iFound = 1;
			break;
		}
	}

	if(!iFound || !pstMember)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2,ZONEERR_PLAYER186);
		return 0;		
	}

	if(pstMember->TakeAward)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2,ZONEERR_PLAYER187);
		return 0;		
	}


	pstAward =z_find_hang_hai_award_def(pstEnv,pstMember->Range);
	if (!pstAward)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_PLAYER188);
		return 0;			
	}

	if (0 == player_award_by_def(pstEnv,pstPlayer,pstAward->PlayerAwardDefID,
				pstAward->AwardTip,pstAward->AwardTipType,1,1))
	{
		pstMember->TakeAward = 1;
		player_action_hang_hai_range(pstEnv,pstPlayer,1);
	}

	return 0;
	UNUSED(pReq);	//add by paraunused.pl
}


int autoteam_in_pworld_del_saft_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer)
{
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;

	pstMiscInfo->InPworldSafeTime = 0;

	z_player_del_status(pstAppCtx, pstEnv, pstPlayer, ENTER_PWORLD_SAFE_STATUSID, 1, 0);
	
	return 0;
}

int player_week_charm_notify(ZONESVRENV* pstEnv, Player *pstPlayer, int iFlag, int iNum)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	WEEKCHARMINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.WeekCharmInfo;
	
	if (!is_same_week(pstInfo->ClearTm,pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstInfo->Charm = 0;
		pstInfo->ClearTm = pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	if (iFlag == 0)
	{
		pstInfo->Charm += iNum;
	}
	else
	{
		if (pstInfo->Charm > iNum)
		{
			pstInfo->Charm -= iNum;
		}
		else
		{
			pstInfo->Charm = 0;
		}
	}
	
	pstMiscSvr->PlayerMiscType = PLAYER_WEEK_CHARM;
	pstMiscSvr->PlayerMiscSvrData.WeekCharm.Charm = pstPlayer->stRoleData.MiscInfo.WeekCharmInfo.Charm;
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int player_charm_chg(ZONESVRENV* pstEnv, Player* pstPlayer, int iFlag, int iNum)
{
	PlayerChagAttrVal stAttr;
	WEEKCHARMINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.WeekCharmInfo;
	
	stAttr.AttrNum = 0;
	stAttr.AttrList[stAttr.AttrNum].nFlag = iFlag;
	stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_CHARM;
	stAttr.AttrList[stAttr.AttrNum].Value.Charm = iNum;
	stAttr.AttrNum++;	

	z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stAttr, 1);

	player_week_charm_notify( pstEnv, pstPlayer, iFlag, iNum);

	range_single_set(pstEnv, pstPlayer, RANGE_MAIN_SINGLE_SUB_CHARM, pstPlayer->stRoleData.RoleDetail.WhiteJade);
	range_single_set(pstEnv, pstPlayer, RANGE_MAIN_SINGLE_SUB_CHARM_WEEK, pstInfo->Charm);
	return 0;
}


int player_gold_rate_notify(ZONESVRENV* pstEnv, Player* pstPlayer, int iLogin)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstRes = &stPkg.Body.PlayerMiscSvr;
	GOLDRATERES *pstRateRes = &pstRes->PlayerMiscSvrData.GoldRateRes;
	pstRes->PlayerMiscType = PLAYER_GOLD_RATE;

	pstRateRes->GoldRate = pstEnv->pstGlobalObj->stGlobal.GoldRate.GoldRate;

	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);

	if (iLogin)
	{
		if (pstPlayer)
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	else
	{
		z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);	
	}
	return 0;
}

int player_charm_res(ZONESVRENV* pstEnv, Player* pstSend, Player *pstAccept, CHARMREQ *pstCharmReq, int iGold)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstRes = &stPkg.Body.PlayerMiscSvr;
	CHARMRES *pstCharmRes = &pstRes->PlayerMiscSvrData.CharmRes;
	pstRes->PlayerMiscType = PLAYER_CHARM_RES;

	pstCharmRes->Flag = pstCharmReq->Flag;
	pstCharmRes->Num = pstCharmReq->Num;
	pstCharmRes->Type = pstCharmReq->Type;
	strncpy(pstCharmRes->Send, pstSend->stRoleData.RoleName, sizeof(pstCharmRes->Send));
	strncpy(pstCharmRes->Accept, pstAccept->stRoleData.RoleName, sizeof(pstCharmRes->Accept));
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);

	/*if (iGold < 500)
	{
		z_cltmsg_send(pstEnv, pstAccept, &stPkg);
	}
	else*/
	{
		if (pstEnv->pstConf->CountryVer == COUNTRY_VER_KOREA)
		{
			z_cltmsg_send(pstEnv, pstSend, &stPkg);
			z_cltmsg_send(pstEnv, pstAccept, &stPkg);
		}
		else
		{
			z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
		}
	}
	return 0;
	UNUSED(iGold);	//add by paraunused.pl
}

int player_charm_req(ZONESVRENV* pstEnv, Player* pstPlayer, CHARMREQ *pstCharmReq)
{
	Player *pstAccept = z_name_player(pstEnv->pstAppCtx, pstEnv, pstCharmReq->Accept);
	int iGold = 0;
	int iCharm = 0;

	if (!pstAccept)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_EXG8);
		return -1;
	}

	if (pstAccept == pstPlayer)
	{
		return -1;
	}

	if (pstCharmReq->Num <= 0 || pstCharmReq->Num > 10000)
	{
		return -1;
	}

	if (pstEnv->pstConf->CountryVer == COUNTRY_VER_KOREA)
	{
		if (pstCharmReq->Type == 0)
		{
			iGold = pstCharmReq->Num*100;
			iCharm = pstCharmReq->Num*1;
		}	
		else if (pstCharmReq->Type == 1)
		{
			iGold = pstCharmReq->Num*1000;
			iCharm = pstCharmReq->Num*15;
		}
		else if (pstCharmReq->Type == 2)
		{
			iGold = pstCharmReq->Num*10000;
			iCharm = pstCharmReq->Num*180;
		}
		else
		{
			return -1;
		}

		if (0 > package_dec_money_test( pstEnv, pstPlayer, ATTR_ID_MONEY, iGold))
		{
			return -1;
		}

		package_dec_money( pstEnv, pstPlayer, ATTR_ID_MONEY, iGold);
	}
	else
	{
		if (pstCharmReq->Type == 0)
		{
			iGold = pstCharmReq->Num*1;
			iCharm = pstCharmReq->Num*1;
		}	
		else if (pstCharmReq->Type == 1)
		{
			iGold = pstCharmReq->Num*10;
			iCharm = pstCharmReq->Num*15;
		}
		else if (pstCharmReq->Type == 2)
		{
			iGold = pstCharmReq->Num*100;
			iCharm = pstCharmReq->Num*180;
		}
		else
		{
			return -1;
		}

		if (0 > package_dec_money_test( pstEnv, pstPlayer, ATTR_ID_GODCOIN, iGold))
		{
			return -1;
		}

		package_dec_money( pstEnv, pstPlayer, ATTR_ID_GODCOIN, iGold);
		
		godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
						pstPlayer->stRoleData.Level, GODCOIN_OPER_FLAG_SONGHUA,0,pstCharmReq->Type,pstCharmReq->Num,
						iGold,"送花");
	}

	// 通知魅力变化
	player_charm_chg(pstEnv, pstAccept, pstCharmReq->Flag,iCharm);
	 
	// 给导师送康乃馨加兄弟会声望
	if (pstCharmReq->Type == 1)
	{
		if (	pstPlayer->stRoleData.SyncRelation.School.JobType== ROLE_SCHOOL_TYPE_STUDENT &&
			pstPlayer->stRoleData.SyncRelation.School.Data.RoleStudent.Teacher.RoleID ==
			pstAccept->stRoleData.RoleID)
		{
			player_add_school_val( pstEnv, pstAccept, 50*pstCharmReq->Num);	
		}
	}

	task_za_charm_fini(pstEnv, pstPlayer,pstCharmReq);
	
	// 各种广播
	player_charm_res(pstEnv, pstPlayer, pstAccept, pstCharmReq, iGold);
	return 0;
}



// 学生登记请求
int player_reg_req(ZONESVRENV* pstEnv, Player* pstPlayer, REGREQ *pstReq)
{
	int i;
	SVRREGINFO *pstSvrInfo = &pstEnv->pstZoneObj->stRegInfo;
		CSPKG stPkg;
	CSPLAYERMISCSVR *pstRes = &stPkg.Body.PlayerMiscSvr;
	REGRES *pstRegRes = &pstRes->PlayerMiscSvrData.RegRes;
	pstRes->PlayerMiscType = PLAYER_REG_RES;

	if ((CheckDirty(pstReq->Msg, 0x1f) & 0x1f ) )
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_DIRTY);
		return -1;
	}
	if ((CheckDirty(pstReq->Msg, 0x0f) & 0x0f ) )
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_DIRTY);
		return -1;
	}
	
	if (is_school_student(pstEnv, pstPlayer))
	{
		return -1;
	}
	
	if (pstSvrInfo->Num >= 100)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE, ZONEERR_PLAYER_301);
		return -1;
	}

	for (i=0; i<pstSvrInfo->Num; i++)
	{
		if (pstSvrInfo->RegInfo[i].RoleWID == pstPlayer->stRoleData.RoleID)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE, ZONEERR_PLAYER_302);
			return -1;
		}
	}

	pstSvrInfo->RegInfo[pstSvrInfo->Num ].RegTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstSvrInfo->RegInfo[pstSvrInfo->Num ].RoleWID = pstPlayer->stRoleData.RoleID;
	STRNCPY(pstSvrInfo->RegInfo[pstSvrInfo->Num ].Name, pstPlayer->stRoleData.RoleName, 
			sizeof(pstSvrInfo->RegInfo[pstSvrInfo->Num ].Name));
	STRNCPY(pstSvrInfo->RegInfo[pstSvrInfo->Num ].Msg, pstReq->Msg, 
			sizeof(pstSvrInfo->RegInfo[pstSvrInfo->Num ].Msg));
	pstSvrInfo->Num ++;




	pstRegRes->Succ = 0;
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_reg_info_notify(ZONESVRENV* pstEnv, Player* pstPlayer, int iNpcID)
{
	int i;
	SVRREGINFO *pstSvrInfo = &pstEnv->pstZoneObj->stRegInfo;
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstRes = &stPkg.Body.PlayerMiscSvr;
	CSSVRREGINFO *pstRegRes = &pstRes->PlayerMiscSvrData.SvrRegInfo;
	Player *pstTmp;
	pstRes->PlayerMiscType = PLAYER_SVR_REG_INFO;
	
	
	pstRegRes->Num = 0;
	pstRegRes->NpcID = iNpcID;
	
	for (i=0; i<pstSvrInfo->Num; i++)
	{
		pstTmp = z_name_player(pstEnv->pstAppCtx, pstEnv, pstSvrInfo->RegInfo[i].Name);
		if(!pstTmp)
		{
			// 发的都是在线的
			continue;
		}
		
		pstRegRes->RegInfoOne[i].RegOne = pstSvrInfo->RegInfo[i];
		pstRegRes->RegInfoOne[i].Lvl = pstTmp->stRoleData.Level;
		pstRegRes->RegInfoOne[i].Gender = pstTmp->stRoleData.Gender;
		pstRegRes->RegInfoOne[i].Career = pstTmp->stRoleData.Career;
		pstRegRes->RegInfoOne[i].ClanWID = pstTmp->stRoleData.MiscInfo.ClanInfo.ClanId.GID;

		pstRegRes->Num ++;
	}

	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_reg_delete(ZONESVRENV* pstEnv, Player* pstPlayer)
{
	int i;
	SVRREGINFO *pstSvrInfo = &pstEnv->pstZoneObj->stRegInfo;

	for (i=0; i<pstSvrInfo->Num; i++)
	{
		if (pstSvrInfo->RegInfo[i].RoleWID == pstPlayer->stRoleData.RoleID)
		{
			pstSvrInfo->Num --;
			if (i != pstSvrInfo->Num)
			{
				pstSvrInfo->RegInfo[i] = pstSvrInfo->RegInfo[pstSvrInfo->Num];
			}

			return 0;
		}
	}
	return 0;
}

int player_reg_delete_by_time(ZONESVRENV* pstEnv)
{
	int i;
	SVRREGINFO *pstSvrInfo = &pstEnv->pstZoneObj->stRegInfo;

	for (i=pstSvrInfo->Num-1; i>=0; i--)
	{
		if (pstSvrInfo->RegInfo[i].RegTime+24*60*60 < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			pstSvrInfo->Num --;
			if (i != pstSvrInfo->Num)
			{
				pstSvrInfo->RegInfo[i] = pstSvrInfo->RegInfo[pstSvrInfo->Num];
			}
		}
	}
	return 0;
}


#define MAX_BUCHANG_DAY 30


BUCHANGDEF *buchang_def_find(ZONESVRENV* pstEnv, int iDay)
{

	BUCHANGDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Day = iDay;
	iIndex = bsearch_int(&stDef, pstObj->sBuChangDef, pstObj->iBuChangDef, BUCHANG_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (BUCHANGDEF *)&pstObj->sBuChangDef[iIndex];
	}
	
	return NULL;
}

ARMBUCHANGDEF *arm_buchang_def_find(ZONESVRENV* pstEnv, int iLvl, int iCareer)
{

	ARMBUCHANGDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Lvl = iLvl;
	stDef.Career = iCareer;
	iIndex = bsearch_int(&stDef, pstObj->sArmBuChangDef, pstObj->iArmBuChangDef, ARM_BUCHANG_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ARMBUCHANGDEF *)&pstObj->sArmBuChangDef[iIndex];
	}
	
	return NULL;
}

int player_buchang_req(ZONESVRENV* pstEnv, Player *pstPlayer, BUCHANGREQ *pstBuChangReq)
{
	LEVELBUFFEXPINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.LevelBuffExpInfo;
	int iSucc = 0;
	int iDay = pstInfo->BuchangInfo.Day;
	BUCHANGDEF *pstDef;
	int i;
	ROLEGRID stGridTmp[MAX_BUCHANG_ITEM];
	int iNum = 0;
	ITEMDEF *pstItemDef;
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstRes = &stPkg.Body.PlayerMiscSvr;
	BUCHANGRES *pstBuchangRes = &pstRes->PlayerMiscSvrData.BuChangRes;

	if(pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_BATTLE_SVR_OP);
		goto end;
	}

	if (pstInfo->BuchangInfo.Flag == 1)
	{
		goto end;
	}
	
	if (iDay != pstBuChangReq->Day)
	{
		goto end;
	}

	if (iDay > MAX_BUCHANG_DAY)
	{
		iDay = MAX_BUCHANG_DAY;
	}

	pstDef = buchang_def_find(pstEnv, iDay);
	if (!pstDef)
	{
		goto end;
	}

	for (i=0; i<MAX_BUCHANG_ITEM; i++)
	{
		if (	pstDef->GiftItem[i].ItemID <= 0 ||
			pstDef->GiftItem[i].ItemNum <= 0)
		{
			break;
		}

		pstItemDef = z_find_itemdef(pstEnv,pstDef->GiftItem[i].ItemID);
		if (!pstItemDef)
			break;
		
		memset(&stGridTmp[iNum], 0, sizeof(stGridTmp[iNum]));
		if(0 == item_create(pstEnv->pstAppCtx, pstEnv, &stGridTmp[iNum], pstDef->GiftItem[i].ItemID))
		{
			stGridTmp[iNum].GridData.RoleItem.Num = pstDef->GiftItem[i].ItemNum;
			stGridTmp[iNum].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
			
			if (pstItemDef->Result[0].ResultID == RESULT_ITEM_EXP_BUCHANG)
			{
				stGridTmp[iNum].GridData.RoleItem.InstVal1 = 
						pstPlayer->stAccShareData.stAccShare.OfflineExp*(pstDef->Modulus/100.0f);

				if (stGridTmp[iNum].GridData.RoleItem.InstVal1 <= 0)
				{
					continue;
				}
			}
		}
		else
		{
			goto end;
		}

		iNum ++;
	}

	if (iNum > 0)
	{
		if(0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stGridTmp[0], iNum, 0))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FENJIE1);
			goto end;
		}

		package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stGridTmp[0], iNum, 0, IMPITEM_OPTYPE_BUCHANG);
	}

	if (pstDef->Bull > 0)
	{
		package_add_money(pstEnv, pstPlayer, ATTR_ID_BULL, pstDef->Bull);
	}

	iSucc = 1;
	pstInfo->BuchangInfo.Flag = 1;
	
end:
	
	pstRes->PlayerMiscType = PLAYER_BUCHANG_RES;
	pstBuchangRes->Succ = iSucc;
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int player_take_clan_war_award_in(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	CLANWARAWARDINFO *pstAwardInfo;
	PlayerChagAttrVal stAttr;
	int i;
	

	pstAwardInfo = &pstPlayer->stRoleData.MiscInfo.ClanWarSelfInfo.AwardInfo;

	stAttr.AttrNum = 0;

	for (i = 0; i < MAX_ATTR_VAL;i++)
	{
		if (i >= pstAwardInfo->AwardAttrNum)
		{
			break;
		}
		
		if (pstAwardInfo->AwardAttr[i].AttrID < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "award attr err,id=%d value=%d",
					pstAwardInfo->AwardAttr[i].AttrID,pstAwardInfo->AwardAttr[i].Value.HP);
			continue;
		}
		stAttr.AttrList[stAttr.AttrNum].AttrID = pstAwardInfo->AwardAttr[i].AttrID;
		stAttr.AttrList[stAttr.AttrNum].nFlag = 0;
		stAttr.AttrList[stAttr.AttrNum].Value = pstAwardInfo->AwardAttr[i].Value;
		stAttr.AttrNum++;
	}
	
	if (stAttr.AttrNum > 0)
	{
		z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stAttr, 1);
	}

	memset(pstAwardInfo,0,sizeof(*pstAwardInfo));

	player_action_clan_war_award_info(pstEnv,pstPlayer);
	
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}

int player_take_clan_war_award(ZONESVRENV *pstEnv,Player *pstPlayer,CSPLAYERTAKECLANWARAWARD *pReq)
{
	
	return player_take_clan_war_award_in(pstEnv,pstPlayer);
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(pReq);	//add by paraunused.pl
}

static int z_account_db_update_uin_flag(ZONESVRENV* pstEnv, Player *pstPlayer,int iUinFlag)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	ACCOUNTUINFLAG stAccoutUinFlag;
	

	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ,
		             "AccountUinFlag", pstPlayer->stRoleData.Uin, NULL);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	stAccoutUinFlag.Uin = pstPlayer->stRoleData.Uin;

	stAccoutUinFlag.UinFlag = pstPlayer->UinFlag | iUinFlag;

	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stAccoutUinFlag;
	stMetaData.iBuff = sizeof(stAccoutUinFlag);
	
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		                      pstEnv->iAccountDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if(0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iAccountDBID))
	{
		return -1;
	}

	return 0;	
}

static int z_region_db_wanyou_mobile_update_req( ZONESVRENV* pstEnv,Player *pstPlayer)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq =  &pstPkg->Body.UpdateReq;
	//char szCallBackBuf[1024];
	//int  iOffset = 0;
	LJY_MEMBER_PROFILE stMemberProfile;
	

	memset(&stMemberProfile,0,sizeof(stMemberProfile));
	
	STRNCPY(stMemberProfile.mobile,pstPlayer->stOnline.szSmsMobile,sizeof(stMemberProfile.mobile));
	STRNCPY(stMemberProfile.username,pstPlayer->szAccount,sizeof(stMemberProfile.username));


	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,-1,&pstPkg->Head, 
								TORM_UPDATE_REQ,DB_UPDATE_MOBILE_RES, 
			               			"ljy_member_profile", 
			               			NULL, 
			               			0);// update 修改点1
	
	pstUpdateReq->DoInsert = 0; //update失败不做插入处理
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	snprintf(pstUpdateReq->OperInfo.WhereDef,
	         sizeof(pstUpdateReq->OperInfo.WhereDef),
	         "where username = '%s'",
		       stMemberProfile.username);

	iDst = pstEnv->iRegionDBID;
	
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,&stMemberProfile, 
			sizeof(stMemberProfile),&iDst,pstEnv->iRoleDBProtoVer);
	
}

int player_precreate_gift_verify(ZONESVRENV *pstEnv,Player *pstPlayer,CSPLAYERPRECREATEGIFTVERIFY *pstReq)
{
	int iRet = 0;
	UinCache * pstUinCache = NULL;
	int iValid = 0;
	
	//todo:sms
	if (pstReq->OpType == PRECREATE_GIFT_VERIFY_OP_GET_SMS)
	{
		if (0 == pstReq->Data.GetSmsMobile[0])
		{
			iRet = PRECREATE_GIFT_VERIFY_RESULT_NO_MOBILE;
			
			goto final;
		}

		if (pstEnv->pstConf->PreCreateSms)
		{
		
			if (pstPlayer->stOnline.iSmsTimeOut > pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				iRet = PRECREATE_GIFT_VERIFY_RESULT_TOO_FAST;
				
				goto final;
			}
		}

		if (pstPlayer->UinFlag & UIN_FLAG_PRECREATE_GIFT)
		{
			iRet = PRECREATE_GIFT_VERIFY_RESULT_DUP;
			
			goto final;
		}

		pstUinCache = (UinCache *)sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos, &iValid);
		if (NULL == pstUinCache || 0 == iValid || pstUinCache->iUin != pstPlayer->stRoleData.Uin)
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
						" Uin=%d uincache",  pstPlayer->stRoleData.Uin);
			
			iRet = PRECREATE_GIFT_VERIFY_RESULT_NO_ROLE;
			
			goto final;
		}
		
		if (pstUinCache->ucRoleNum < 1)
		{
			iRet = PRECREATE_GIFT_VERIFY_RESULT_NO_ROLE;
			
			goto final;
		}


		
		STRNCPY(pstPlayer->stOnline.szSmsMobile,pstReq->Data.GetSmsMobile,sizeof(pstPlayer->stOnline.szSmsMobile));
		if (pstEnv->pstConf->PreCreateSms)
		{
			if (0 > player_action_precreate_gift_verify(pstEnv,pstPlayer,ACTION_PRECREATE_GIFT_VERIFY_OP_SN,
										0))
			{
				iRet = PRECREATE_GIFT_VERIFY_RESULT_SMS_SEND_FAIL;
				
				goto final;
			}
			else
			{
				return 0;
			}
		}
	}
	else if(pstReq->OpType == PRECREATE_GIFT_VERIFY_OP_VERIFY)
	{
		if (pstEnv->pstConf->PreCreateSms)
		{
			if (pstPlayer->stOnline.iSmsTimeOut < pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				iRet = PRECREATE_GIFT_VERIFY_RESULT_TIMEOUT;
				
				goto final;
			}

			if (pstPlayer->stOnline.iSmsCode != pstReq->Data.Verify ||
				pstPlayer->stOnline.iSmsCode == 0 ||
				0 == pstPlayer->stOnline.szSmsMobile[0] )
			{
				iRet = PRECREATE_GIFT_VERIFY_RESULT_VERIFY_FAIL;
				
				goto final;
			}
		}

		if (pstPlayer->UinFlag & UIN_FLAG_PRECREATE_GIFT)
		{
			iRet = PRECREATE_GIFT_VERIFY_RESULT_DUP;
			
			goto final;
		}

		//改手机号
		z_region_db_wanyou_mobile_update_req(pstEnv,pstPlayer);
		
		//改db中的uinflag
		z_account_db_update_uin_flag(pstEnv,pstPlayer,UIN_FLAG_PRECREATE_GIFT);
		
		//加礼包,丢给 bill_svr
		player_acc_gift_insert_req(pstEnv, pstPlayer,0);

		pstPlayer->UinFlag |= UIN_FLAG_PRECREATE_GIFT;

		//通知各个zone
		{
			SSCMDREQ stCmdReq;
			SSCMDREQ* pstCmdReq = &stCmdReq;

			
			memset(pstCmdReq,0,sizeof(*pstCmdReq));
			pstCmdReq->Cmd = SS_CMD_UIN_FLAG_CHANGE;
		
			STRNCPY(pstCmdReq->Data.UinFlagChange.AccName, pstPlayer->szAccount,
				sizeof(pstCmdReq->Data.UinFlagChange.AccName)) ;
			
			pstCmdReq->Data.UinFlagChange.Type = ACC_SET_UIN_FLAG_TYPE_NOTICE;
			pstCmdReq->Data.UinFlagChange.UinFlag = UIN_FLAG_PRECREATE_GIFT;
			pstCmdReq->Data.UinFlagChange.Uin = pstPlayer->stRoleData.Uin;

			//Z_SSHEAD_INIT(&pstEnv->stSSPkg.Head, SS_CMD_REQ);
			//z_ssmsg_send(pstEnv, &pstEnv->stSSPkg, pstEnv->iRegionGateID);

			z_send_to_region_msg(pstEnv->pstAppCtx,pstEnv,SS_CMD_REQ,(char *)pstCmdReq,sizeof(*pstCmdReq));
			
			//ss_sendto_region_pkg(pstAppCtx, pstEnv,SS_CMD_REQ, (char *)pstCmdReq,sizeof(*pstCmdReq));
		}

		iRet = PRECREATE_GIFT_VERIFY_RESULT_SUCC;
		pstPlayer->stOnline.iSmsCode = 0;
		
	}
	else
	{
		return -1;
	}


final:	
	
	player_action_precreate_gift_verify(pstEnv,pstPlayer,ACTION_PRECREATE_GIFT_VERIFY_OP_RESULT,
										iRet);
	return 0;
	
}

int player_misc_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSPLAYERMISCCLT *pstReq = &pstCsPkg->Body.PlayerMiscClt;

	UNUSED(pstFrameHead);

	if (!pstPlayer)
	{
		return -1;
	}

	if (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
	{
		if(pstReq->PlayerMiscType != PLAYER_PRECREATE_GIFT_VERIFY &&
		pstReq->PlayerMiscType != PLAYER_WORLD_BOSS_INFO_REQ)
		{
			return -1;
		}
	}
	
	switch(pstReq->PlayerMiscType)
	{
		case PLAYER_MATCH_CHG_CAREER:
			player_match_chg_career(pstAppCtx,pstEnv, pstPlayer, &pstReq->PlayerMiscCltData.MatchChgCareer);
			break;
		case PLAYER_CAREER_CHG_CLT:
			player_career_chg(pstAppCtx, pstEnv, pstPlayer, pstReq->PlayerMiscCltData.CarrerChgType,1);
			break;
		case PLAYER_CLEAN_SAFETIME_STATUSID_CLT:
			autoteam_in_pworld_del_saft_status(pstAppCtx, pstEnv, pstPlayer);
			break;
		case PLAYER_CHOOSE_CLT:
			player_choose(pstEnv,pstPlayer,&pstReq->PlayerMiscCltData.ChooseC);
			break;
		case PLAYER_OFFLINE_EXP_CLT:
			//player_offline_exp(pstEnv,pstPlayer);
			break;
		case PLAYER_SELECT_TARGET_CLT:
			player_select_target(pstPlayer, pstReq->PlayerMiscCltData.TargetID);
			break;
		case PLAYER_DROP_CLAN_ITEM:
			player_drop_clan_tem(pstEnv,pstPlayer);
			break;
		case PLAYER_CLAN_EVERYDAY_PWORLD_ENTER:
			player_clan_everyday_pworld_enter(pstEnv,pstPlayer,&pstReq->PlayerMiscCltData.ClanEverydayPworldEnter);
			break;
		case PLAYER_CLAN_MACHINE_PWORLD_ENTER:
			player_clan_machine_pworld_enter(pstEnv,pstPlayer,&pstReq->PlayerMiscCltData.ClanMachinePworldEnter);
			break;
		case PLAYER_SHAM_DEAD_BRK:
			player_sham_dead_clear(pstEnv, pstPlayer);
			break;
		case PLAYER_TAKE_COW_AWARD:
			player_take_cow_award(pstEnv,pstPlayer,&pstReq->PlayerMiscCltData.TakeCowAward);
			break;
		case PLAYER_EXP_BUFF_AWARD:
			player_level_buff_award(pstEnv, pstPlayer);
			break;
		case PLAYER_BAOXIANG_OPEN:
			player_baoxiang_open(pstEnv, pstPlayer, &pstReq->PlayerMiscCltData.BaoXiangOpen);
			break;
		case PLAYER_WORLD_BOSS_INFO_REQ:
			if (INST_MODE_ZONE_SVR != pstEnv->iInstMode)
			{
				return battle_proxy_request(pstEnv, pstPlayer, pstCsPkg);
			}
			
			boss_info_req(pstEnv, pstPlayer, &pstReq->PlayerMiscCltData.BossInfoReq);
			break;
		case PLAYER_CHARM_REQ:
			player_charm_req(pstEnv, pstPlayer,  &pstReq->PlayerMiscCltData.CharmReq);
			break;
		case PLAYER_REG_REQ:
			player_reg_req(pstEnv,  pstPlayer, &pstReq->PlayerMiscCltData.RegReq);
			break;
		case PLAYER_BUCHANG_REQ:
			player_buchang_req( pstEnv, pstPlayer, &pstReq->PlayerMiscCltData.BuChangReq);
			player_level_buff_award(pstEnv, pstPlayer);
			break;
		case PLAYER_MT_GIFT_AWARD:
			player_mt_gift_award(pstEnv, pstPlayer);
			break;
		case PLAYER_TAKE_HANG_HAI_AWARD:
			player_take_hang_hai_award(pstEnv,pstPlayer,&pstReq->PlayerMiscCltData.TakeHangHaiAward);
			break;
		case PLAYER_YANGHAIBANG_ADD:
			player_yanghaibang_add(pstEnv,pstPlayer,&pstReq->PlayerMiscCltData.YangHaiBangAdd);
			break;
		case PLAYER_YANGHAIBANG_AWARD:
			player_yanghaibang_award(pstEnv,pstPlayer);
			break;
		 case PLAYER_RONGYAN_BOSS_SELECT:
		 	player_rongyan_boss(pstEnv,pstPlayer,&pstReq->PlayerMiscCltData.RongYanBossSelect);
		 	break;
   		case PLAYER_RONGYAN_BEGIN_SELECT:
			player_rongyan_begin(pstEnv,pstPlayer,&pstReq->PlayerMiscCltData.RongYanBeginSelect);
			break;
		case PLAYER_TAKE_CLAN_WAR_AWARD:
			player_take_clan_war_award(pstEnv,pstPlayer,&pstReq->PlayerMiscCltData.TakeClanWarAward);
			break;
		case PLAYER_PAOSHANG_UI_SELECT:
			player_paoshang_clt_select(pstEnv,pstPlayer,&pstReq->PlayerMiscCltData.PaoShangUiSelect);
			break;
			
		case PLAYER_PRECREATE_GIFT_VERIFY:
			player_precreate_gift_verify(pstEnv,pstPlayer,&pstReq->PlayerMiscCltData.PrecreateGiftVerify);
			break;

		case PLAYER_DIANFENG_GROUP_INFO_C:
			player_clt_need_which_one_group_info(pstEnv,pstPlayer,&pstReq->PlayerMiscCltData.DianFengGroupInfoC);
			break;
		case PLAYER_DIANFENG_JING_CAI:
			player_dianfeng_guess(pstEnv, pstPlayer,&pstReq->PlayerMiscCltData.DianFengJingCai);
			break;
		case PLAYER_DIANFENG_BAOMING_CLT:
			player_dianfeng_baoming(pstEnv,pstPlayer, &pstReq->PlayerMiscCltData.DianFengBaoMing);
			break;
		case PLAYER_DIANFENG_TRANSMIT_CLT:
			player_dianfeng_transmit(pstEnv, pstPlayer,&pstReq->PlayerMiscCltData.DianFengTransmit);
			break;
		case PLAYER_DIANFENG_IF_GUESS_CLT:
			player_dianfeng_if_guess(pstEnv, pstPlayer,&pstReq->PlayerMiscCltData.DianFengIfGuess);
			break;
		case PLAYER_ZJ_RONGYAN_CALL:
			player_call_zj_rongyan(pstEnv, pstPlayer,&pstReq->PlayerMiscCltData.ZJRongYanCall);
			break;
		case PLAYER_ZJ_RONGYAN_BOSS_SELECT:
			player_zj_rongyan_boss(pstEnv,pstPlayer,&pstReq->PlayerMiscCltData.ZJRongYanBossSelect);
			break;
		case PLAYER_SCULPT_CHG_CLT:
			player_sculpt_chg_clt(pstEnv,pstPlayer,&pstReq->PlayerMiscCltData.SculptChgClt);
			break;
		case PLAYER_NEW_PEOPLE_CLT:
			player_new_people_column_clt(pstEnv,pstPlayer,&pstReq->PlayerMiscCltData.NewPeopleClt);
			break;
		case PLAYER_SHORTCUT_SKILL_CLEAR:
			player_clear_shortcut_skill_clt(pstEnv,pstPlayer,&pstReq->PlayerMiscCltData.ClearShortCutSkill);
			break;
		default: 
			break; 
	}

	return 0;
}

static int gprs_svr(ZONESVRENV* pstEnv, Player *pstPlayer,Player *pstGprsPlayer,ROLEENEMYMEMBER *pstEnemyMember)
{
	CSPKG stPak;
	GPRSSVR *pstGPRSSvr = &stPak.Body.GPRSSvr;
	ZoneClan *pstClan;// = player_get_clan(pstEnv, pstPlayer);

	pstGPRSSvr->Gender = pstGprsPlayer->stRoleData.Gender;
	pstGPRSSvr->Hair = pstGprsPlayer->stRoleData.Hair;
	pstGPRSSvr->Head = pstGprsPlayer->stRoleData.Head;
	pstGPRSSvr->Face = pstGprsPlayer->stRoleData.Face;
	pstGPRSSvr->Career = pstGprsPlayer->stRoleData.Career;
	pstGPRSSvr->Level = pstGprsPlayer->stRoleData.Level;
	pstGPRSSvr->GPRSEnd = pstEnemyMember->GPRSEnd;
	pstGPRSSvr->X = pstGprsPlayer->stRoleData.Pos.X;
	pstGPRSSvr->Y = pstGprsPlayer->stRoleData.Pos.Y;
	pstGPRSSvr->Map = pstGprsPlayer->stRoleData.Map;
	pstGPRSSvr->Team = pstGprsPlayer->stRoleData.MiscInfo.TeamID > 0?1:0;
	strncpy(pstGPRSSvr->RoleName, pstEnemyMember->RoleName, sizeof(pstGPRSSvr->RoleName));
	pstGPRSSvr->ClanName[0] = 0;
	pstClan = player_get_clan(pstEnv, pstGprsPlayer);
	if (pstClan)
	{
		strncpy(pstGPRSSvr->ClanName,pstClan->stClan.ClanProf.Name, sizeof(pstGPRSSvr->ClanName));
	}

	Z_CSHEAD_INIT(&stPak.Head, GPRS_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPak);
	return 0;
}


int player_gprs(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	ROLEENEMYMEMBER *pstEnemyMember;
	Player *pstGprsPlayer;
	UNUSED(pstFrameHead);

	pstGprsPlayer = z_name_player(pstAppCtx, pstEnv, pstCsPkg->Body.GPRSClt.RoleName);
	if (!pstGprsPlayer)
		return -1;

	pstEnemyMember = relation_enemy_online_find(&pstPlayer->stRoleData.MiscInfo.Enemy, 
													pstGprsPlayer->stRoleData.RoleID);
	if (!pstEnemyMember)
		return -1;

	// 点击定位选项
	if (pstCsPkg->Body.GPRSClt.Flag == 0)
	{
		if (pstEnemyMember->GPRSEnd < pstAppCtx->stCurr.tv_sec)
		{
			if (package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,GPRS_ITEMID, 1, -1, NULL,
				PACKAGE_DEC_NONE, IMPITEM_OPTYPE_GPRS) < 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER189);
				return -1;
			}

			// 定位仪30分钟失效
			pstEnemyMember->GPRSEnd = pstAppCtx->stCurr.tv_sec + 30*60;
		}
	}
	else
	{
		if (pstEnemyMember->GPRSEnd < pstAppCtx->stCurr.tv_sec)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER190);
			return -1;
		}
	}

	gprs_svr(pstEnv, pstPlayer,pstGprsPlayer,pstEnemyMember);
	return 0;
}


int player_choose(ZONESVRENV* pstEnv, Player *pstPlayer,CSCHOOSEC* pstCsChooseC)
{
	char szScript[RES_NAME_LEN];
	int iProcIndex = -1;
	int iNpcIndex = -1;
	Npc* pstNpc = NULL;
	int iProc = 0;
	char cType = 0;
	ScriptIndex* pstScriptInd = NULL;

	memset(szScript,0,sizeof(szScript));
	
	if (BIND_TYPE_UNKNOWN == pstPlayer->stOnline.stChooseInfo.cChooseBindType ||
		pstPlayer->stOnline.stChooseInfo.cChooseBindType != 	pstCsChooseC->BindType ||
		pstPlayer->stOnline.stChooseInfo.iChooseMemID != pstCsChooseC->MemID)
	{
		return -1;//验证不通过
	}
	switch (pstCsChooseC->ChooseType)
	{
		case CHOOSE_TYPE_CLOSE:
			goto _final;
			break;
			
		case CHOOSE_TYPE_YES:
			iProcIndex = 0;
			break;

		case CHOOSE_TYPE_NO:
			iProcIndex = 1;
			break;
	}

	if (iProcIndex < 0 || iProcIndex >= MAX_CHOOSE_NUM)
	{
		goto _final;
	}
	
	iProc = pstPlayer->stOnline.stChooseInfo.aiChooseProc[iProcIndex];	
	
	switch (pstPlayer->stOnline.stChooseInfo.cChooseBindType)
	{
		case BIND_TYPE_NAME:
			stSEnv.pstMonster = NULL;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPlayer = pstPlayer;
			stSEnv.pstPworldInst = NULL;
			stSEnv.pstRoleItem = NULL;
			stSEnv.iItemDefID = 0;
			stSEnv.pstMapinst = NULL;
			stSEnv.pstKiller = NULL;

			if (0 == pstPlayer->stOnline.stChooseInfo.szScript[0])
			{
				goto _final;
			}

			pstScriptInd = get_script_ind(pstPlayer->stOnline.stChooseInfo.szScript);
			if ( !pstScriptInd )
			{
				//printf("get_script_ind fail\n");
				tlog_error(pstEnv->pstLogCat, 0, 0, "get_script_ind fail");
				goto _final;
			}
			
			z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstPlayer->stOnline.stChooseInfo.szScript, pstScriptInd, iProc);
			break;
		case BIND_TYPE_ITEM:
			
			stSEnv.pstMonster = NULL;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPlayer = pstPlayer;
			stSEnv.pstPworldInst = NULL;
			stSEnv.pstRoleItem = NULL;
			stSEnv.iItemDefID = pstCsChooseC->MemID;
			stSEnv.pstMapinst = NULL;
			stSEnv.pstKiller = NULL;
			
			snprintf(szScript, sizeof(szScript), "item%d.mac", pstCsChooseC->MemID);				
			pstScriptInd = get_script_ind(szScript);
			if ( !pstScriptInd )
			{
				//printf("get_script_ind fail\n");
				tlog_error(pstEnv->pstLogCat, 0, 0, "get_script_ind fail");
				goto _final;
			}
			
			z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, szScript, pstScriptInd, iProc);
			
			break;
		case BIND_TYPE_NPC:
			iNpcIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstCsChooseC->MemID, &cType);
			if (0 > iNpcIndex)
			{
				goto _final;
			}

			pstNpc = tmempool_get(pstEnv->pstNpcPool, iNpcIndex);
			if (!pstNpc )
			{
				goto _final;
			}

			if (pstNpc->szScriptName[0] == 0)
			{
				goto _final;
			}

			if (0 > z_check_op_dist(pstEnv, pstPlayer, pstNpc->stMon.stMap.iID,
			                        &pstNpc->stMon.stCurrPos, OP_DIST + pstNpc->stMon.unRadius))
			{
				goto _final;
			}

			stSEnv.pstPlayer = pstPlayer;
			stSEnv.pstNpc = pstNpc;
			stSEnv.pstMonster = NULL;
			stSEnv.pstPworldInst = NULL;
			stSEnv.pstMapinst = NULL;
			stSEnv.pstKiller = NULL;
			
			z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstNpc->szScriptName, &pstNpc->stScriptIdx, iProc);
			
			break;
		case BIND_TYPE_EVENT:
			{
				GameEventInst stEvent;
				GameEventInst *pstEvent = NULL;


				stEvent.iEventID = pstCsChooseC->MemID;
				pstEvent = sht_find(pstEnv->pstShtEvent, &stEvent, EventHashCmp, EventHashCode);
				if (pstEvent)
				{
					stSEnv.pstPlayer = pstPlayer;
					stSEnv.pstNpc = NULL;
					stSEnv.pstMonster = NULL;
					stSEnv.pstPworldInst = NULL;
					stSEnv.pstEvent = pstEvent;
					stSEnv.pstMapinst = NULL;
					stSEnv.pstKiller = NULL;
					
					z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstEvent->szScriptName, &pstEvent->stScriptIdx, iProc);
				}
			}
			break;
		default:
			goto _final;
			break;
	}
	
_final:

	tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player choose input %d %d %d",
						pstPlayer->stRoleData.Uin, pstCsChooseC->BindType,pstCsChooseC->MemID,pstCsChooseC->ChooseType);

	pstPlayer->stOnline.stChooseInfo.iChooseLevel--;
	if (pstPlayer->stOnline.stChooseInfo.iChooseLevel < 1)
	{
		//置空
		pstPlayer->stOnline.stChooseInfo.cChooseBindType = BIND_TYPE_UNKNOWN;
		pstPlayer->stOnline.stChooseInfo.iChooseMemID = 0;
		memset(pstPlayer->stOnline.stChooseInfo.aiChooseProc,0,sizeof(pstPlayer->stOnline.stChooseInfo.aiChooseProc));
		memset(pstPlayer->stOnline.stChooseInfo.szScript,0,sizeof(pstPlayer->stOnline.stChooseInfo.szScript));
	}
	
	return 0;
}

int z_player_script_event(ZONESVRENV* pstEnv, Player *pstPlayer,int iProc)
{
	unsigned char ucFlag = 0;
	ScriptIndex *pstScriptInd = get_script_ind(PLAYER_EVENT_SCRIPT_NAME);
	if (NULL == pstScriptInd)
	{
		return 0;
	}

	if (PLAYER_EVENT_SCRIPT_PROC_LEVEL == iProc)
	{
		ucFlag = PLAYER_EVENT_SCRIPT_PROC_LEVEL_MAP;
	}

	stSEnv.pstMonster = NULL;
	stSEnv.pstNpc = NULL;
	stSEnv.pstPlayer = pstPlayer;
	stSEnv.pstPworldInst = NULL;
	stSEnv.pstMapinst = NULL;
	stSEnv.pstKiller = NULL;
	
	if (pstEnv->ucPlayerScriptFlag & ucFlag)
	{
		z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, PLAYER_EVENT_SCRIPT_NAME, pstScriptInd, iProc);		
	}
	return 0;
}



CONTONLINEGIFTDEF *z_find_contonlinedef(ZONESVRENV* pstEnv, int iGiftID)
{
	CONTONLINEGIFTDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.GiftID = iGiftID;
	iIndex = bsearch_int(&stDef, pstObj->sContOnlineDef, pstObj->iContOnlineDef, CONT_ONLINE_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (CONTONLINEGIFTDEF *)&pstObj->sContOnlineDef[iIndex];
	}
	return NULL;
}


int player_comp_contonlineinfo(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	int i,j;
	CONTONLINEGIFTDEF *pstDef;
	MISCCONTONLINEINFO *pstContOnlineInfo = &pstPlayer->stRoleData.MiscInfo.ContOnlineInfo;

	for ( i=pstContOnlineInfo->Num -1; i>=0; i--)
	{
		// 上次记录在线时间和当前登录时间不是同一天 清除在线时间 从零开始
		if (!IsSameDay(pstContOnlineInfo->ContOnlineInfo[i].UpOnlineTm, pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			pstContOnlineInfo->ContOnlineInfo[i].OnlineTm = 0;
		}

		// 登录的时候更新记录时间
		pstContOnlineInfo->ContOnlineInfo[i].UpOnlineTm = pstEnv->pstAppCtx->stCurr.tv_sec;
		
		pstDef = z_find_contonlinedef( pstEnv, pstContOnlineInfo->ContOnlineInfo[i].GiftID );
		if ( !pstDef )
		{
			pstContOnlineInfo->Num --;
			if ( i != pstContOnlineInfo->Num )
			{
				memcpy( &pstContOnlineInfo->ContOnlineInfo[i], 
						 &pstContOnlineInfo->ContOnlineInfo[pstContOnlineInfo->Num],
						 sizeof(pstContOnlineInfo->ContOnlineInfo[0]));
			}
		}	
	}

	for ( i=0; i<pstObj->iContOnlineDef; i++ )
	{
		pstDef = 	(CONTONLINEGIFTDEF *)pstObj->sContOnlineDef[i];
		for ( j=0; j<pstContOnlineInfo->Num; j++ )
		{
			if (pstDef->GiftID == pstContOnlineInfo->ContOnlineInfo[j].GiftID)
			{
				break;
			}
		}
		
		if ( j>=pstContOnlineInfo->Num )
		{
			if (pstContOnlineInfo->Num >= MAX_CONTGIFT)
			{
				return 0;
			}
			
			memset(&pstContOnlineInfo->ContOnlineInfo[pstContOnlineInfo->Num], 0,
					sizeof(pstContOnlineInfo->ContOnlineInfo[pstContOnlineInfo->Num]));
			pstContOnlineInfo->ContOnlineInfo[pstContOnlineInfo->Num].GiftID = pstDef->GiftID;
			pstContOnlineInfo->ContOnlineInfo[pstContOnlineInfo->Num].UpOnlineTm = pstEnv->pstAppCtx->stCurr.tv_sec;
			pstContOnlineInfo->Num ++;
		}
	}

	update_cont_online_time(pstEnv, pstPlayer, 1);
	role_cont_online_info(pstEnv, pstPlayer);
	return 0;
}


int update_cont_online_time(ZONESVRENV* pstEnv, Player *pstPlayer, int iLogin)
{
	MISCCONTONLINEINFO *pstContOnlineInfo = &pstPlayer->stRoleData.MiscInfo.ContOnlineInfo;
	int i;
	CONTONLINEINFO *pstInfo;
	int iSend = 0;
	int iSec;

	for (i=0; i<pstContOnlineInfo->Num; i++)
	{
		CONTONLINEGIFTDEF *pstDef = z_find_contonlinedef( pstEnv, pstContOnlineInfo->ContOnlineInfo[i].GiftID );
		if (!pstDef)
			continue;

		pstInfo = &pstContOnlineInfo->ContOnlineInfo[i];
		if (!IsSameDay(pstInfo->UpOnlineTm, pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			pstInfo->OnlineTm = 0;
		}
		else
		{
			iSec = pstEnv->pstAppCtx->stCurr.tv_sec - pstInfo->UpOnlineTm;
			if (iSec > 0 && iSec <= 20)
			{
				pstInfo->OnlineTm += iSec;
				if (!IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstInfo->UpDayTm))
				{
					if (	pstInfo->UpDayTm == 0 ||
						IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstInfo->UpDayTm+24*60*60))
					{
						if (pstInfo->OnlineTm >= pstDef->Hour*60)
						{
							pstInfo->ContDay ++;
							pstInfo->UpDayTm = pstEnv->pstAppCtx->stCurr.tv_sec;
						}
					}
					else
					{
						// 如果是可以领奖的话   不改变
						if (!pstInfo->IsGift)
						{
							pstInfo->ContDay = 0;
							pstInfo->UpDayTm = 0;
						}
					}
				}
			}
			
		}

		pstInfo->UpOnlineTm = pstEnv->pstAppCtx->stCurr.tv_sec;

		// 可以领取了  通知客户端
		if (pstInfo->ContDay >= pstDef->Day && pstInfo->NextFlag != ONCE_GIFT_NOT_NEXT && !pstInfo->IsGift)
		{
			pstInfo->IsGift = 1;
			iSend = 1;
		}
	}
	
	if (iSend && !iLogin)
	{
		role_cont_online_info(pstEnv, pstPlayer);
	}
	return 0;
}


int clean_oneday_gift_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLEGIFTINFO *pstRoleGiftInfo = &pstPlayer->stRoleData.MiscInfo.RoleGiftInfo;
	CSPKG stCSPkg;
	CSGIFTSVR *pstGiftSvr = &stCSPkg.Body.GiftSvr;

	pstRoleGiftInfo->OneDayNum = 0;
	pstRoleGiftInfo->OneDayOnlineTM = 0;
	memset(&pstRoleGiftInfo->GiftInfoOneDay[0], 0, sizeof(pstRoleGiftInfo->GiftInfoOneDay[0]));
	pstRoleGiftInfo->OnlineTMUpTM = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	memcpy(&pstGiftSvr->GiftInfo, pstRoleGiftInfo, sizeof(pstGiftSvr->GiftInfo));
	pstGiftSvr->PreCreateGift = pstPlayer->stRoleData.CtrlFlag;
	pstGiftSvr->UserType = 0;
	if (pstPlayer->stOnline.stVipBarInfo.iVipBarID)
		pstGiftSvr->UserType = USER_TYPE_INTERNET_BAR;
	
	Z_CSHEAD_INIT(&stCSPkg.Head, GIFT_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stCSPkg);
	return 0;
}

// 清理隔天信息   任务   副本 等等
int player_clean_diffsameday_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;

	int i,j;
	TASKINFO *pstTaskInfo = &pstPlayer->stRoleData.MiscInfo.TaskInfo;
	TASKONEDAY *pstTaskOneDay = &pstTaskInfo->TaskOneDay;
	TASKCYCLEONEDAY *pstTaskCycleOneDay = &pstTaskInfo->TaskCycleOneDay;
	GROUPONEDAY *pstGroupOneDay = &pstTaskInfo->GroupOneDay;
	TASKRECYCLE *pstTaskRecycle = &pstTaskInfo->TaskRecycle;
	TASKRUN *pstTaskRun = &pstTaskInfo->TaskRun;
	TASKCYCLEWEEK *pstWeek = &pstTaskInfo->CycleWeek;
	int iClean = 0;
	GROUPWEEK *pstGroupWeek = &pstTaskInfo->GroupWeek;

	if (IsSameDay(pstMisc->CleanDiffTm, iCurr))
	{
		return 0;
	}

	pstMisc->CleanDiffTm = iCurr;

	// 清理任务信息
	for (i=pstTaskOneDay->TaskNum-1; i>=0; i--)
	{
		if (0 == IsSameDay(pstTaskOneDay->TaskOneDayInfo[i].Time, pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			iClean = 1;
			pstTaskOneDay->TaskNum--;
			if (i != pstTaskOneDay->TaskNum)
			{
				pstTaskOneDay->TaskOneDayInfo[i] = pstTaskOneDay->TaskOneDayInfo[pstTaskOneDay->TaskNum];
			}
		}
	}

	for (i=pstWeek->CycleNum-1; i>=0; i--)
	{
		if (0==IsSameWeek(pstWeek->CycleWeekInfo[i].Time, pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			iClean = 1;
			pstWeek->CycleNum --;
			if (i != pstWeek->CycleNum)
			{
				pstWeek->CycleWeekInfo[i] = pstWeek->CycleWeekInfo[pstWeek->CycleNum];
			}
		}
	}

	for (i=pstTaskCycleOneDay->TaskCycleNum-1; i>=0; i--)
	{
		if (0 == IsSameDay(pstTaskCycleOneDay->TaskCycleOneDayInfo[i].Time, pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			iClean = 1;
			pstTaskCycleOneDay->TaskCycleNum--;
			if (i != pstTaskCycleOneDay->TaskCycleNum)
			{
				pstTaskCycleOneDay->TaskCycleOneDayInfo[i] = pstTaskCycleOneDay->TaskCycleOneDayInfo[pstTaskCycleOneDay->TaskCycleNum];
			}
		}
	}

	for (i=pstGroupOneDay->GroupNum -1; i>=0; i--)
	{
		if (0 == IsSameDay(pstGroupOneDay->GroupOneDayInfo[i].Time, pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			iClean = 1;
			pstGroupOneDay->GroupNum --;
			if (i != pstGroupOneDay->GroupNum)
			{
				pstGroupOneDay->GroupOneDayInfo[i] = pstGroupOneDay->GroupOneDayInfo[pstGroupOneDay->GroupNum];
			}
		}
	}

	for (i=pstGroupWeek->GroupNum-1; i>=0; i--)
	{
		if (0==IsSameWeek(pstGroupWeek->GroupWeekInfo[i].Time, pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			iClean = 1;
			pstGroupWeek->GroupNum--;
			if (i != pstGroupWeek->GroupNum)
			{
				pstGroupWeek->GroupWeekInfo[i] = pstGroupWeek->GroupWeekInfo[pstGroupWeek->GroupNum];
			}
		}
	}

	for (i=pstTaskRecycle->TaskRecycleNum-1; i>=0;i--)
	{
		for (j=0; j<pstTaskRun->TaskRunNum; j++)
		{
			if (pstTaskRecycle->TaskRecycles[i].TaskRecycleID == pstTaskRun->TaskRuns[j].CycleID)
			{
				break;
			}
		}

		// 没有任务了  环组从第一环开始。。
		if (j>=pstTaskRun->TaskRunNum)
		{
			taskcycle_run_del(pstPlayer, pstTaskRecycle->TaskRecycles[i].TaskRecycleID);
			iClean = 1;
		}
		/*else
		{
			pstTaskRecycle->TaskRecycles[i].GetTime = iCurr;
			pstCycDef = taskcycle_def_find(pstEnv,pstTaskRecycle->TaskRecycles[i].TaskRecycleID,NULL);
			if (pstCycDef && !pstCycDef->OtherDayFlag)
			{
				pstTaskRecycle->TaskRecycles[i].TaskRecycleIdx=0;	
				iClean = 1;
			}
		}*/
	}

	if (iClean)
	{
		task_info_all(pstEnv, pstPlayer);
	}

	// 清理副本信息
	pworld_limit_info_get(pstEnv, pstPlayer);

	// 清理每天奖励信息
	clean_oneday_gift_info(pstEnv, pstPlayer);

	battle_limit_info_get(pstEnv, pstPlayer);

	// 清理精灵刻印信息
	fairy_clean_keyin_info(pstEnv, pstPlayer);

	if (pstPlayer->stRoleData.MiscInfo.GoldRaffle != 0)
	{
		pstPlayer->stRoleData.MiscInfo.GoldRaffle = 0;
		player_gold_raffle_num_notify(pstEnv,pstPlayer);
	}

	player_daily_goal_info(pstEnv,pstPlayer);

	player_full_vig(pstEnv, pstPlayer);

	pstPlayer->stRoleData.MiscInfo.DailyUseBull = 0;
	player_daily_bull_action(pstEnv, pstPlayer);

	player_pworld_off_exp(pstEnv, pstPlayer, 1);

	memset(&pstPlayer->stRoleData.MiscInfo.MiscWar, 0, 
			sizeof(pstPlayer->stRoleData.MiscInfo.MiscWar));

	player_beach_info_update(pstEnv, pstPlayer);

	// 清除今日进入据点ID
	if (pstPlayer->stRoleData.MiscInfo.RoleStrongInfo1.StrongID != 0)
	{
		pstPlayer->stRoleData.MiscInfo.RoleStrongInfo1.StrongID = 0;
		strong_enter_id_chg( pstEnv, pstPlayer, 0);
	}

	player_busy_day_update(pstEnv, pstPlayer);

	pstPlayer->stRoleData.MiscInfo.LostTotemInfo.Num = 0;
	player_mt_gift_info(pstEnv, pstPlayer);

	player_cont_cover_up( pstEnv, pstPlayer, 0);

	player_zhengyi_limit_chg(pstEnv, pstPlayer);

	player_life_brand_limit_chg(pstEnv, pstPlayer);

	// 更新每天能获得的传承点
	pstPlayer->stRoleData.MiscInfo.SchoolInfo.DaySchoolVal = 0;
	pstPlayer->stRoleData.MiscInfo.SchoolInfo.DayTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	player_chuancheng_notify(pstEnv, pstPlayer);

	clan_pet_info_notify(pstEnv, pstPlayer);

	if (!IsSameWeek(pstPlayer->stRoleData.MiscInfo.MztInfo.PreTime, iCurr))
	{
		pstPlayer->stRoleData.MiscInfo.MztInfo.PreTime = iCurr;
		pstPlayer->stRoleData.MiscInfo.MztInfo.Layer = 0;

		clan_pet_clear_gaoji_sec(pstEnv, pstPlayer);
	}

	player_sj_item_clear(pstEnv, pstPlayer);

	unreal_check_exp_layer(pstEnv,pstPlayer);
	return 0;
}

int player_movie_start_notify(ZONESVRENV* pstEnv, Player *pstPlayer, char *szFilePath)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstPlayerMiscSvr = &stPkg.Body.PlayerMiscSvr;
	CSMOVIESTARTS *pstMovieStartS = &pstPlayerMiscSvr->PlayerMiscSvrData.MovieStartS;

	memset(pstPlayerMiscSvr,0,sizeof(CSPLAYERMISCSVR));

	//让玩家停止走动
	z_player_stop(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, NULL);
	
	pstPlayerMiscSvr->PlayerMiscType = PLAYER_MOVIE_START_SVR;
	snprintf(pstMovieStartS->MovieFilePath,sizeof(pstMovieStartS->MovieFilePath),
		"%s",szFilePath);
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_view_mon_delpress(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	Monster *pstMon;
	RESPOS stSearchPos;
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;
	MapIndex *pstMapIdx;
	AtkTargetList  *pstTargetList;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx) 
		return -1;
	
	for (i=pstPlayer->stOnline.iVAnimate-1; i>=0; i--)
	{
		pstMon = z_id_monster2(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.aiVAnimate[i]);
		if (NULL == pstMon || pstMon->bSubType >= MON_SUB_TYPE_TMP || pstMon->cStat != MONSTER_ALIVE )
		{
			continue;
		}

		if (0 > mon_press_delete_player(pstEnv, pstMon, pstPlayer))
		{
			continue;
		}

		pstTargetList = &pstMon->stAtkTargets;
		if (pstTargetList->iTargetCur == pstPlayer->iMemID)
		{
			pstTargetList->iTargetCur  = 0;
		}

		if (z_pos_search_pos(pstMapIdx, &pstMon->stCurrPos, &stSearchPos)) {
			z_mon_search_view(pstEnv->pstAppCtx, pstEnv,  pstMapInst, &stSearchPos, pstMon);
		}

		//怪物归位
		if ( 0 == pstTargetList->PressNum &&
		     0 >= pstTargetList->iDefyID &&
		     0 >= pstTargetList->iMarkedID)
		{
			if (pstMon->unTraceDist > 0 && pstMon->bAtkType == ATK_TYPE_INITIATIVE &&  pstMon->bMoveType != MOVE_TYPE_NO)
			{
				mon_ai_home(pstEnv, pstMon);
			}
		}
	}

	 pstPlayer->stOnline.iHitTarget = 0;

	return 0;
}


int player_boss_info_insert(Player *pstPlayer, Monster *pstMon)
{
	AtkBossInfo *pstAtkBossInfo = &pstPlayer->stOnline.stAtkBossInfo;
	MONSTERDEF *pstMonDef;

	pstMonDef = z_get_mon_def(&gs_stEnv, pstMon);
	if (NULL == pstMonDef)
	{
		return -1;
	}
	
	if(pstMon->bSubType != MON_SUB_TYPE_BOSS && (0 == (MON_MISC_TYPE_CAMP_TASK & pstMonDef->MiscType)))
	{
		return -1;
	}

	pstAtkBossInfo->iMemID = pstMon->iID;
	return 0;
}

int player_boss_info_del(Player *pstPlayer, Monster *pstMon)
{
	AtkBossInfo *pstAtkBossInfo = &pstPlayer->stOnline.stAtkBossInfo;
	MONSTERDEF *pstMonDef;

	pstMonDef = z_get_mon_def(&gs_stEnv, pstMon);
	if (NULL == pstMonDef)
	{
		return -1;
	}
	
	if(pstMon->bSubType != MON_SUB_TYPE_BOSS && (0 == (MON_MISC_TYPE_CAMP_TASK & pstMonDef->MiscType)))
	{
		return -1;
	}

	if(pstAtkBossInfo->iMemID != pstMon->iID)
	{
		return -1;
	}
	
	pstAtkBossInfo->iMemID = 0;
	return 0;
}

int is_in_player_boss_info(Player *pstPlayer, Monster *pstMon)
{
	AtkBossInfo *pstAtkBossInfo = &pstPlayer->stOnline.stAtkBossInfo;
	MONSTERDEF *pstMonDef;

	pstMonDef = z_get_mon_def(&gs_stEnv, pstMon);
	if (NULL == pstMonDef)
	{
		return -1;
	}
	
	if(pstMon->bSubType != MON_SUB_TYPE_BOSS && (0 == (MON_MISC_TYPE_CAMP_TASK & pstMonDef->MiscType)))
	{
		return -1;
	}
	
	if(pstAtkBossInfo->iMemID != pstMon->iID)
	{
		return 0;
	}
	
	return 1;
}

Monster * new_bub_mon(ZONESVRENV* pstEnv, Player *pstPlayer , ZONEMAPINST *pstMapInst, MapIndex *pstMapIdx, 
		int iMonID, int iLifeTime)
{
	Monster stMonster;
	MONSTERDEF *pstMonDef;
	Monster *pstFind;
	RESPOS stPos;
	int iPoint, iFaceAngel = 0;
	
	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos=	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.tRefresh	=	pstEnv->pstAppCtx->stCurr.tv_sec;
	
	stMonster.iOwnerID = pstPlayer->iMemID;
	mon_set_pkinfo(&stMonster, MON_PK_TYPE_PLAYER, pstPlayer->iMemID);
	snprintf(stMonster.szOwner,sizeof(stMonster.szOwner),"%s",pstPlayer->stRoleData.RoleName);
	stMonster.szOwner[sizeof(stMonster.szOwner) - 1] = 0;
		
	//设置起始朝向
	iPoint = (pstPlayer->stOnline.cLastPath + pstPlayer->stOnline.cLastPathPoints - 1+RECORD_TURN)%RECORD_TURN;
	if(iPoint >= 0)
	{
		iFaceAngel = FaceAngleBetween(pstPlayer->stOnline.stTurnPoints[iPoint].X, pstPlayer->stOnline.stTurnPoints[iPoint].Y, 
								pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y);
	}
	SET_RANPOS_DIR(iFaceAngel, 0, 0);

	pstMonDef = z_get_mon_def(pstEnv, &stMonster);
	if (NULL == pstMonDef || pstMonDef->SubType != MON_SUB_TYPE_TOTEM)
	{
		return NULL;
	}
	
	z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstPlayer->stRoleData.Pos, &pstPlayer->stRoleData.Pos, 400, 400, &stPos);

	stMonster.bDirection = RAND1(180);
	stMonster.stInitPos = stPos;
	pstFind =	z_new_monster(pstEnv->pstAppCtx, pstEnv, &stMonster, pstMonDef);
	if( !pstFind )
		return NULL;

	pstFind->cFirstBorn = 1;
	if(iLifeTime > 0)
	{
		pstFind->tLifeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + iLifeTime;
	}
	if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstFind, NULL))
	{
		tmempool_free(pstEnv->pstMonsterPool, pstFind->iMIndex);
		return NULL;
	}
	
	pstFind->cFirstBorn = 0;

	return pstFind;
}

int player_call_bub(ZONESVRENV* pstEnv, Player *pstPlayer, int iMonID, int iLifeTime)
{
	MapIndex *pstMapIdx;
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;
	Monster *pstBub;
	
	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	if(pstPlayer->stOnline.stBubOnline.iBubNum >= MAX_PLAYER_BUB)
	{
		return -1;
	}

	pstBub = new_bub_mon(pstEnv, pstPlayer, pstMapInst, pstMapIdx, iMonID, iLifeTime);
	if(NULL == pstBub)
	{
		return -1;
	}

	pstPlayer->stOnline.stBubOnline.iBubID[pstPlayer->stOnline.stBubOnline.iBubNum++] = pstBub->iID;

	pstBub->iPlayerTotemNext = pstPlayer->stOnline.iTotemHead;
	pstPlayer->stOnline.iTotemHead = pstBub->iID;
	pstPlayer->stOnline.bTotemNum++;
	
	return pstBub->iID;
}

int player_bub_insert_enemy_mon(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMon)
{
	int i;
	Monster *pstBub;
	
	if(!is_player_enemy_mon(pstEnv, pstPlayer, pstMon))
	{
		return 0;
	}

	for(i=0; i<pstPlayer->stOnline.stBubOnline.iBubNum; i++)
	{
		pstBub = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stBubOnline.iBubID[i]);
		if(NULL == pstBub)
		{
			continue;
		}

		mon_press_insert_mon(pstEnv, pstBub, pstMon);
	}

	return 0;
}

int player_bub_insert_enemy_player(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstOtherPlayer)
{
	int i;
	Monster *pstBub;
	MONSTERDEF *pstMonDef;
	
	if(!is_player_enemy_player(pstEnv, pstPlayer, pstOtherPlayer))
	{
		return 0;
	}

	for(i=0; i<pstPlayer->stOnline.stBubOnline.iBubNum; i++)
	{
		pstBub = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stBubOnline.iBubID[i]);
		if(NULL == pstBub)
		{
			continue;
		}

		pstMonDef = z_get_mon_def(pstEnv, pstBub);
		if (NULL == pstMonDef)
		{
			return -1;
		}

		mon_press_insert_player(pstEnv, pstBub, pstMonDef, pstOtherPlayer);
	}

	return 0;
}

/*
int z_player_oneoffexp_send(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_OFFLINE_EXP;
	pstActionRes->Data.OfflineExpInfo.Exp = pstPlayer->stRoleData.MiscInfo.OfflineExpInfo.OneOffExp;
	pstActionRes->Data.OfflineExpInfo.Type = CSOFFLINE_EXP_ONE_OFF;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);


	return 0;
}
*/
int z_player_awardexp_change(ZONESVRENV* pstEnv, Player *pstPlayer,int iChange, int iSend)
{
	LEVELUP * pstLevel = NULL;	
	AccShareData* pstAccShareData = &pstPlayer->stAccShareData;
	int iOld;

	if (ACC_SHARE_FINAL != pstAccShareData->iStat)
	{
		return -1;
	}
//	char szMsg[256];
	iOld = pstAccShareData->stAccShare.OfflineExp;
	
	pstAccShareData->stAccShare.OfflineExp += iChange;
	if ( pstAccShareData->stAccShare.OfflineExp < 0)
	{
		pstAccShareData->stAccShare.OfflineExp = 0;
	}

	//最大值判断
	pstLevel = z_get_levelup(pstEnv, pstAccShareData->iAccMaxRoleLevel);
	if (pstLevel && 
		pstAccShareData->stAccShare.OfflineExp > pstLevel->MaxOfflineExp)
	{
		pstAccShareData->stAccShare.OfflineExp = pstLevel->MaxOfflineExp;
	}

	//发协议
	if (iSend)
	{
		CSPKG stPkg;
		CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
		
		
		pstActionRes->ID = pstPlayer->iMemID;
		pstActionRes->ActionID = ACTION_OFFLINE_EXP;
		pstActionRes->Data.OfflineExpInfo.Exp = pstAccShareData->stAccShare.OfflineExp;
		pstActionRes->Data.OfflineExpInfo.Type = CSOFFLINE_EXP_REMAIN;
		pstActionRes->Data.OfflineExpInfo.MaxExp = pstAccShareData->stAccShare.OfflineExp;
		if (pstLevel)
		{
			pstActionRes->Data.OfflineExpInfo.MaxExp = pstLevel->MaxOfflineExp;
		}
		
		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);		

		iChange = pstAccShareData->stAccShare.OfflineExp - iOld;
		if (iChange < 0)
		{	
			pstPlayer->stOnline.iUsedOfflineExp = -iChange;
			/*
			snprintf(szMsg, sizeof(szMsg), "你使用了%d点离线补偿经验", -iChange);
			szMsg[sizeof(szMsg) - 1] = 0;

			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, szMsg);
			*/
		}
	}
	
	return 0;
}



int z_player_awardexp_calculate(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	OFFLINEEXPPARAMETER* pstPara = NULL;
	int iExp = 0;
	int iSec = 0;
	longlong iMinuteExp = 0;
	LEVELUP* pstLevel;
	ACCSHARE* pstAccShare = &pstPlayer->stAccShareData.stAccShare;

	if (ACC_SHARE_FINAL != pstPlayer->stAccShareData.iStat)
	{
		return -1;
	}


	if ( 0 ==  pstAccShare->LastLogout)
	{
		return 0;
	}


	iSec = pstEnv->pstAppCtx->stCurr.tv_sec - pstAccShare->LastLogout;
	if (iSec < 0)
	{
		tlog_error(pstEnv->pstLogCat,pstPlayer->stRoleData.Uin,0,
			"time error iSec = %d",iSec);
		iSec = 0;
	}

	iSec -= ZONE_OFFLINE_EXP_SEC;
	
	if (iSec < 0)
	{
		iSec = 0;
	}

	pstLevel = z_get_levelup(pstEnv, pstPlayer->stAccShareData.iAccMaxRoleLevel);
	if (!pstLevel)
	{
		tlog_error(pstEnv->pstLogCat,pstPlayer->stRoleData.Uin,0,
			"LevelUPDef get fail level=%d",pstPlayer->stAccShareData.iAccMaxRoleLevel);
		
		iSec = 0;
		return -1;
	}

	pstPara = z_get_offline_exp_parameter(pstEnv,OFFLINE_EXP_ZONE);
		
	if (pstPara && pstPara->Parameter1 != 0 && iSec > 0)
	{
		//公式:M*sec/参数1
		iMinuteExp = (longlong) pstLevel->ContributeExp * 60 / pstPara->Parameter1;
		if (iMinuteExp < 1)
		{
			iMinuteExp = 1;
		}
		
		iExp = (int)(iMinuteExp * (iSec /60.0f)) ;
		
		tlog_debug(pstEnv->pstLogCat,pstPlayer->stRoleData.Uin,0,
			"level = %d,sec = %d, exp = %d",pstPlayer->stAccShareData.iAccMaxRoleLevel,iSec,iExp);
		
	}	

	
	
	//发协议
	z_player_awardexp_change(pstEnv,pstPlayer,iExp,TRUE);	

	
	return 0;
}

/*
int player_offline_exp(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iExp = pstPlayer->stRoleData.MiscInfo.OfflineExpInfo.OneOffExp;

	if (iExp > 0)
	{
		z_player_addexp_test(pstEnv, pstPlayer, &iExp);
		player_add_exp(pstEnv,pstPlayer,iExp,0);
		pstPlayer->stRoleData.MiscInfo.OfflineExpInfo.OneOffExp -= iExp;
	}
	
	z_player_oneoffexp_send(pstEnv,pstPlayer);

	return 0;
}
*/

int is_bad_mon(ZONESVRENV* pstEnv, Monster *pstMon)
{
	if(!is_mon_machine(pstMon))
	{
		return 0;
	}
	
	if(get_pkval_type(this_machine(pstEnv, pstMon)->stMachineInfo.PKVal) > PK_VAL_TYPE_NORMAL)
	{
		return 1;
	}

	return 0;
}

int is_in_pk_time_mon(ZONESVRENV* pstEnv, Monster *pstMon)
{
	if(!is_mon_machine(pstMon))
	{
		return 0;
	}
	
	if(this_machine(pstEnv, pstMon)->stMachineInfo.PkTime > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return 1;
	}

	return 0;
}


int is_bad_palyer(Player *pstPlayer)
{
	if(get_player_pkval_type(pstPlayer) > PK_VAL_TYPE_NORMAL)
	{
		return 1;
	}

	return 0;
}

int is_in_pk_time_player(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if(pstPlayer->stOnline.tPkEndTime > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return 1;
	}

	return 0;
}

int player_pk_time_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_PK_TIME;
	pstActionRes->Data.PKTime.PkTime = 0;
	if(pstPlayer->stOnline.tPkEndTime > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		pstActionRes->Data.PKTime.PkTime = pstPlayer->stOnline.tPkEndTime - pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);

	return 0;
}

int is_player_pk_atk_player(ZONESVRENV* pstEnv, Player *pstAtker, Player *pstAtkee, int fNoHurt)
{
	ZoneClan *pstAtkerClan = player_get_clan(pstEnv, pstAtker);
	MapIndex *pstMapIndex;
	
	//地图限制
	pstMapIndex = z_mappos_index(pstEnv->pstMapObj, NULL, pstAtker->stRoleData.Map);
	if(NULL == pstMapIndex)
	{
		return 0;
	}

	if(pstMapIndex->stMapDef.MapID == BASE_CITY_MAPID)
	{
		return 0;
	}

	//非中立地图不犯罪
	if(pstMapIndex->stMapDef.MapLimit.PKMode != 0 || pstMapIndex->stMapDef.MapLimit.PK == 0)
	{
		return 0;
	}

	if(pstMapIndex->stMapDef.CtrlFlag & MAP_CTRL_NO_PK_VAL)
	{
		return 0;
	}

	//攻城战时期，攻城战地图杀人不违法
	if(pstMapIndex->stMapDef.MapID == WORLD_CITY_MAPID &&
		is_in_siege_city_fast(pstEnv))
	{
		return 0;
	}

	if(pstAtker == pstAtkee)
	{
		return 0;
	}
	
	if(z_is_qiecuo(pstEnv, pstAtker, pstAtkee))
	{
		return 0;
	}
	
	//不可以攻击的敌人
	if(!z_player_hit_player_filter(pstEnv, pstAtker, pstAtkee))
	{
		if(is_in_pk_time_player(pstEnv, pstAtkee) || is_bad_palyer(pstAtkee))
		{
			if(fNoHurt)
			{
				return 1;
			}
			else
			{
				return -1;
			}
		}
	}
	else
	{
		//敌对公会之间不加PK值
		if(pstAtkerClan && clan_relation_by_player(pstEnv, pstAtkee, pstAtkerClan) < 0)
		{
			return 0;
		}

		if(is_in_pk_time_player(pstEnv, pstAtkee) || is_bad_palyer(pstAtkee))
		{
			return 0;
		}

		return 1;
	}

	return 0;
}

int is_player_pk_atk_mon(ZONESVRENV* pstEnv, Player *pstAtker, Monster *pstAtkee)
{
	MapIndex *pstMapIndex;
	
	//地图限制
	pstMapIndex = z_mappos_index(pstEnv->pstMapObj, NULL, pstAtker->stRoleData.Map);
	if(NULL == pstMapIndex)
	{
		return 0;
	}

	//非中立地图不犯罪
	if(pstMapIndex->stMapDef.MapLimit.PKMode != 0 || pstMapIndex->stMapDef.MapLimit.PK == 0)
	{
		return 0;
	}

	if(pstMapIndex->stMapDef.CtrlFlag & MAP_CTRL_NO_PK_VAL)
	{
		return 0;
	}

	//攻城战时期，攻城战地图杀人不违法
	if(pstMapIndex->stMapDef.MapID == WORLD_CITY_MAPID &&
		is_in_siege_city_fast(pstEnv))
	{
		return 0;
	}
	
	if(pstAtkee->iPKType == MON_PK_TYPE_TEAM ||
		pstAtkee->iPKType == MON_PK_TYPE_CLAN)
	{
		if(is_player_enemy_mon(pstEnv, pstAtker, pstAtkee))
		{
			return 1;
		}
	}
	
	return 0;
}

int player_set_pktime_check(ZONESVRENV* pstEnv, Player *pstAtker, ZoneAni *pstAtkee, int fNoHurt)
{	
	if(pstAtkee->iType == OBJ_PLAYER)
	{
		if(0 >= is_player_pk_atk_player(pstEnv, pstAtker, pstAtkee->stObj.pstPlayer, fNoHurt))
		{
			return -1;
		}
	}
	else if(pstAtkee->iType == OBJ_PET)
	{
		if(0 >= is_player_pk_atk_player(pstEnv, pstAtker, pstAtkee->stObj.stZonePet.pstOwner, fNoHurt))
		{
			return -1;
		}
	}
	else if(pstAtkee->iType == OBJ_MONSTER)
	{
		if(pstAtkee->stObj.pstMon->iPKType ==  MON_PK_TYPE_PLAYER)
		{
			Player *pstAtkeePlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAtkee->stObj.pstMon->ullPKID);
			if(NULL == pstAtkeePlayer)
			{
				return -1;
			}
			
			if(is_in_pk_time_mon(pstEnv, pstAtkee->stObj.pstMon) || is_bad_mon(pstEnv, pstAtkee->stObj.pstMon))
			{
				if(!is_player_enemy_mon(pstEnv, pstAtker, pstAtkee->stObj.pstMon) &&
					-1 == is_player_pk_atk_player(pstEnv, pstAtker, pstAtkeePlayer, fNoHurt))
				{
					return 0;
				}
				else
				{
					return -1;
				}
			}
			else
			{
				if(0 >= is_player_pk_atk_player(pstEnv, pstAtker, pstAtkeePlayer, fNoHurt))
				{
					return -1;
				}
			}
		}
		else
		{
			if(!is_player_pk_atk_mon(pstEnv, pstAtker, pstAtkee->stObj.pstMon))
			{
				return -1;
			}
		}
	}

	return 0;
}

int player_set_pktime(ZONESVRENV* pstEnv, Player *pstAtker, ZoneAni *pstAtkee, int fNoHurt)
{
	if(is_bad_palyer(pstAtker))
	{
		return 0;
	}

	if (pstAtkee->iType == OBJ_PLAYER)
	{
		if (is_both_lucky_attendee(pstAtkee->stObj.pstPlayer, pstAtker))
		{
			return 0;
		}

		if (is_both_clan_spy(pstEnv, pstAtkee->stObj.pstPlayer, pstAtker))
		{
			return 0;
		}
	}

	if(0 > player_set_pktime_check(pstEnv, pstAtker, pstAtkee, fNoHurt))
	{
		return 0;
	}
	
	pstAtker->stOnline.tPkEndTime = pstEnv->pstAppCtx->stCurr.tv_sec + 60;
	return player_pk_time_action(pstEnv, pstAtker);
}

int mon_set_pktime(ZONESVRENV* pstEnv, Monster *pstAtker, ZoneAni *pstAtkee, int fNoHurt)
{
	Player *pstAtkerPlayer;

	if(is_bad_mon(pstEnv, pstAtker))
	{
		return 0;
	}

	if(pstAtker->iPKType != MON_PK_TYPE_PLAYER)
	{
		return 0;
	}
	
	pstAtkerPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAtker->ullPKID);
	if(NULL == pstAtkerPlayer)
	{
		return 0;
	}

	if(!is_mon_machine(pstAtker))
	{
		return player_set_pktime(pstEnv, pstAtkerPlayer, pstAtkee, fNoHurt);
	}

	if(0 > player_set_pktime_check(pstEnv, pstAtkerPlayer, pstAtkee, fNoHurt))
	{
		return 0;
	}

	pstAtkerPlayer->stOnline.tPkEndTime = pstEnv->pstAppCtx->stCurr.tv_sec + 60;
	player_pk_time_action(pstEnv, pstAtkerPlayer);

	this_machine(pstEnv, pstAtker)->stMachineInfo.PkTime = pstEnv->pstAppCtx->stCurr.tv_sec + 60;
	return machine_pk_time_action(pstEnv, pstAtker);
}

int atker_set_pktime(ZONESVRENV* pstEnv, ZoneAni *pstAtker, ZoneAni *pstAtkee, int fNoHurt)
{
	if(pstAtker->iType == OBJ_PLAYER)
	{
		player_set_pktime(pstEnv, pstAtker->stObj.pstPlayer, pstAtkee, fNoHurt);
	}
	else if(pstAtker->iType == OBJ_PET)
	{
		player_set_pktime(pstEnv, pstAtker->stObj.stZonePet.pstOwner, pstAtkee, fNoHurt);
	}
	else if(pstAtker->iType == OBJ_MONSTER)
	{
		mon_set_pktime(pstEnv, pstAtker->stObj.pstMon, pstAtkee, fNoHurt);
	}
	
	return 0;
}

int player_pk_time_clear(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if(!is_in_pk_time_player(pstEnv, pstPlayer))
	{
		return 0;
	}

	pstPlayer->stOnline.tPkEndTime = 0;
	
	return player_pk_time_action(pstEnv, pstPlayer);
}

int z_totem_update_enemy(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstTotem)
{
	int i;
	AtkTargetList *pstTargetList = &pstTotem->stAtkTargets;

	for(i=pstTargetList->PressNum -1; i>=0; i--)
	{
		int iIndex;
		char cType;
		Player *pstOtherPlayer;
		Monster *pstMon;
		
		iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstTargetList->astPressEntrys[i].iTargetID, &cType);
		if (iIndex == -1)
		{
			continue;
		}

		switch(cType)
		{
			case OBJ_PLAYER:
				pstOtherPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstTargetList->astPressEntrys[i].iTargetID);
				//清理主人非敌对的目标
				if (pstOtherPlayer && !is_player_enemy_player(pstEnv, pstPlayer, pstOtherPlayer))
				{
					mon_press_delete_player(pstEnv, pstTotem, pstOtherPlayer);
				}
				break;
			case OBJ_MONSTER:
				pstMon =	z_id_monster(pstEnv->pstAppCtx, pstEnv, pstTargetList->astPressEntrys[i].iTargetID);
				//清理主人非敌对的目标
				if (pstMon && !is_player_enemy_mon(pstEnv, pstPlayer, pstMon))
				{
					mon_press_delete_mon(pstEnv, pstTotem, pstMon);
				}
				break;
			default:
				break;
		}
	}
	
	return 0;
}

int player_totem_update_enemy(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	Monster *pstMonTmp;
	int iTotemID;
	
	//图腾链表
	iTotemID = pstPlayer->stOnline.iTotemHead;
	while(iTotemID > 0)
	{
		pstMonTmp = z_id_monster(pstEnv->pstAppCtx, pstEnv, iTotemID);
		if (NULL == pstMonTmp)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "totem %d error", iTotemID);
			return -1;
		} 

		iTotemID = pstMonTmp->iPlayerTotemNext;	
		z_totem_update_enemy(pstEnv, pstPlayer, pstMonTmp);
	}

	return 0;
}

int player_is_bianshen(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	ZoneStatus *pstZoneStatus;
	STATUSDEF *pstStatusDef;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;

	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		
		if(pstStatusDef->ModeID > 0)
		{
			return 1;
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		
		if(pstStatusDef->ModeID > 0)
		{
			return 1;
		}
	}

	for (i=0; i<pstStatusList->bNoLimitStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		
		if(pstStatusDef->ModeID > 0)
		{
			return 1;
		}
	}

	return 0;
}

int is_can_bianshen(ZONESVRENV* pstEnv, Player *pstPlayer,int iSend, int iIgnoreRide)
{
	// 摆摊、其他无双、临时技能状态、自动飞行状态（骑龙和飞行器）、骑乘等
	int iRet = 0;
	
	if ((pstPlayer->stOnline.cMiscFlag & MISC_FLAG_WUSHUANG) ||
		player_is_bianshen(pstEnv, pstPlayer))
	{
		if (iSend)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER191);
		}
		goto final;
	}

	if (pstPlayer->stOnline.State &CS_STAT_STALL)
	{
		if (iSend)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER192);
		}
		
		goto final;
	}

	if (pstPlayer->stOnline.State &CS_STAT_AUTOFLY)
	{
		if (iSend)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER193);
		}
		goto final;
	}

	if (!iIgnoreRide)
	{
		if (pstPlayer->stOnline.State &(CS_STAT_RIDE_ON|CS_STAT_RIDE_PREPARE))
		{
			if (iSend)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER194);
			}
			goto final;
		}
	}
	
	if (is_player_ride_on_machine(pstPlayer))
	{
		if (iSend)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER194);
		}
		goto final;	
	}

	if (is_tmp_machine(pstEnv,pstPlayer))
	{
		if (iSend)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER194);
		}
		goto final;
	}

	iRet = 1;

final:
		
	return iRet;
}


int player_motion_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_MOTION;

	pstActionRes->Data.Motion.MotionID = pstPlayer->stOnline.stMotionInfo.MotionID;
	pstActionRes->Data.Motion.TargetID = pstPlayer->stOnline.stMotionInfo.TargetID;
		
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);

	return 0;
}

int player_motion_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	int i;
	SHITEM *pstItem;
	GameEventInst *pstEvent;
	CSMOTIONCLT *pstMotionClt = &pstCsPkg->Body.Motion;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;
	
	UNUSED(pstAppCtx);
	UNUSED(pstFrameHead);

	pstPlayer->stOnline.stMotionInfo.MotionID = pstMotionClt->MotionID;
	pstPlayer->stOnline.stMotionInfo.TargetID = pstMotionClt->TargetID; 

	if(pstPlayer->stOnline.stMotionInfo.MotionID <= 0)
	{
		goto _final;
	}
	
	//任务追踪
	for (i=0; i<pstTaskRun->TaskRunNum; i++)
	{
		TASKDEF *pstTaskDef = task_def_find(pstEnv, pstTaskRun->TaskRuns[i].TaskID, &pstTaskRun->TaskRuns[i].TaskIdx);
		if(NULL == pstTaskDef)
		{
			continue;
		}

		if (pstTaskDef->TaskScript & TASK_SCRIPT_PROC_MOTION_MAP)
		{	
			ScriptIndex *pstScriptInd;
			char szScript[32];	
		
			stSEnv.pstMonster = NULL;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPlayer = pstPlayer;
			stSEnv.iMemID = pstPlayer->stOnline.stMotionInfo.TargetID;
			stSEnv.iMotionID = pstPlayer->stOnline.stMotionInfo.MotionID;
			stSEnv.pstMapinst = NULL;
			stSEnv.pstKiller = NULL;
			
			snprintf(szScript, sizeof(szScript), "task%d.mac", pstTaskDef->ID);				
			pstScriptInd = get_script_ind(szScript);

			if ( !pstScriptInd )
			{
				continue;
			}
			
			z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, szScript, pstScriptInd, TASK_SCRIPT_PROC_MOTION);

			stSEnv.iMemID = 0;
			stSEnv.iMotionID = 0;
		}
	}

	//游戏时间追踪
	for (i = 0; i < pstEnv->pstShtEvent->iMax; i++) 
	{
		pstItem = SHT_GET_ITEM(pstEnv->pstShtEvent, i);
		if ( !pstItem->fValid )
		{
	            continue;
	       }
		
		pstEvent = (GameEventInst *)pstItem->szData;

		if(pstEvent->cScriptFlag & GAME_EVENT_SCRIPT_PROC_MOTION_MAP)
		{
			stSEnv.pstEvent= pstEvent;
			stSEnv.pstMonster = NULL;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPlayer = pstPlayer;
			stSEnv.iMemID = pstPlayer->stOnline.stMotionInfo.TargetID;
			stSEnv.iMotionID = pstPlayer->stOnline.stMotionInfo.MotionID;
			stSEnv.pstMapinst = NULL;
			stSEnv.pstKiller = NULL;
						
			z_exec_script(pstAppCtx, pstEnv, NULL, pstEvent->szScriptName, &pstEvent->stScriptIdx, GAME_EVENT_SCRIPT_PROC_MOTION);
			
			stSEnv.pstEvent= NULL;
			stSEnv.iMemID = 0;
			stSEnv.iMotionID = 0;
		}
	}

_final:

	return player_motion_action(pstEnv, pstPlayer);
}

int player_target_action(ZONESVRENV* pstEnv, Player *Player)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	
	pstActionRes->ID = Player->iMemID;
	pstActionRes->ActionID = ACTION_TARGET;
	pstActionRes->Data.Target.TargetID = Player->stOnline.iSelectTarget;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, Player, 0, &stPkg, 0);
	
	return 0;
}


// 玩家当前是否检查动态阻挡
int player_is_check_dyn_mask(Player *pstPlayer)
{
	if (pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		return 0;
	}
	
	if (pstPlayer->stOnline.State & CS_STAT_STALL)
	{
		return 0;
	}

	return 1;
}

int player_leave_beach(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;
	BEACHINFO *pstBeachInfo = &pstPlayer->stRoleData.MiscInfo.BeachInfo;
	int iSec;
	
	if (pstPlayer->stRoleData.Map != BEACH_MAP_ID)
	{
		return -1;
	}

	memset(pstAction,0,sizeof(*pstAction));

	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_FAKE_STATUS;

	memset(&pstAction->Data.FakeStatus,0,sizeof(pstAction->Data.FakeStatus));
	
	pstAction->Data.FakeStatus.ID = BEACH_STATUS_ID;
	pstAction->Data.FakeStatus.Level = 1;
	pstAction->Data.FakeStatus.OpType = FAKE_STATUS_OP_DEL;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	iSec = pstBeachInfo->Sec % BEACH_EXP_INTERVAL;
	if ( iSec > 0)
	{
		pstBeachInfo->Sec += BEACH_EXP_INTERVAL - iSec;
	}

	pstPlayer->stOnline.iBeachMapSec = 0;
		
	return 0;
}

int player_enter_beach(ZONESVRENV *pstEnv,Player *pstPlayer,int iOldMapID)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;
	BEACHINFO *pstBeachInfo = &pstPlayer->stRoleData.MiscInfo.BeachInfo;
	
	if (pstPlayer->stRoleData.Map != BEACH_MAP_ID)
	{
		if (iOldMapID == BEACH_MAP_ID)
		{
			z_update_avatar(pstEnv,pstPlayer);
		}
		return -1;
	}

	memset(pstAction,0,sizeof(*pstAction));

	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_FAKE_STATUS;

	memset(&pstAction->Data.FakeStatus,0,sizeof(pstAction->Data.FakeStatus));
	
	pstAction->Data.FakeStatus.ID = BEACH_STATUS_ID;
	pstAction->Data.FakeStatus.Level = 1;
	pstAction->Data.FakeStatus.OpType = FAKE_STATUS_OP_ADD;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	if (!IsSameDay(pstBeachInfo->EnterTime,	pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstBeachInfo->EnterTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstBeachInfo->Sec = 0;
	}

	pstPlayer->stOnline.iBeachMapSec = 0;

	z_update_avatar(pstEnv,pstPlayer);

	wedding_celebration_to_clt(pstEnv,pstPlayer);
	
	return 0;
}

//在地图上广播玩家的位置
int player_broadcast_map_pos(ZONESVRENV *pstEnv,Player* pstPlayer,int iShowType)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;
	ZONEIDX  stIdx;
	ZONEMAPINST * pstMapInst = NULL;


	memset(pstAction,0,sizeof(*pstAction));

	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_MAP_POS;
	
	STRNCPY(pstAction->Data.MapPos.Name,pstPlayer->stRoleData.RoleName,sizeof(pstAction->Data.MapPos.Name));
	pstAction->Data.MapPos.ShowType = iShowType;
	pstAction->Data.MapPos.Pos = pstPlayer->stRoleData.Pos;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	
	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (pstMapInst)
	{
		map_broadcast(pstEnv->pstAppCtx,pstEnv,pstMapInst,&stPkg);
	}
	return 0;
}


int lucky_open_ui_action(ZONESVRENV* pstEnv, int iType)
{
	/*int i;
	Player *pstPlayer;
	CSPKG stPkg;
	CSACTIVEGAMESVR *pstActiveGameSvr = &stPkg.Body.ActiveGameSvr;
	CSACTIVEGAMEUIINFO *pstActiveUIInfo = &pstActiveGameSvr->Data.UIInfo;

	pstActiveGameSvr->Type = iType;
	pstActiveUIInfo->UIType = ACTIVE_UI_TYPE_LUCKY;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTIVE_GAME_SVR);
	for (i=0; i<MAX_PLAYER; i++)
	{
		pstPlayer = (Player *)tmempool_get_bypos(pstEnv->pstPlayerPool, i);
				
		if (pstPlayer && PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus &&
			pstPlayer->stRoleData.Level >= LUCKY_LEVEL)
		{
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);	
		}
	}*/

	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(iType);	//add by paraunused.pl
}

int player_login_lucky_ui(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	/*if (LUCKY_LEVEL > pstPlayer->stRoleData.Level)
	{
		return 0;
	}
	
	if (pstEnv->iMiscFlag &GAME_FLAG_LUCK)
	{
		CSPKG stPkg;
		CSACTIVEGAMESVR *pstActiveGameSvr = &stPkg.Body.ActiveGameSvr;
		CSACTIVEGAMEUIINFO *pstActiveUIInfo = &pstActiveGameSvr->Data.UIInfo;

		pstActiveGameSvr->Type = ACTIVE_GAME_SVR_TYPE_UI_INFO;
		pstActiveUIInfo->UIType = ACTIVE_UI_TYPE_LUCKY;
		
		Z_CSHEAD_INIT(&stPkg.Head, ACTIVE_GAME_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}*/
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}


//宏福活动,1秒1次
int player_luck_tick(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	if (pstEnv->iMiscFlag & GAME_FLAG_LUCK)
	{
		//携带者地图广播位置
		if (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_MAP_BROADCAST)
		{		
			player_broadcast_map_pos(pstEnv,pstPlayer,1);
			
			if (0 == pstEnv->iLuckyTipCount % 300)
			{
				//宏福buff携带者提示				
				ZONEIDX stIdx;
				MapIndex* pstMapIndex;

				stIdx.iPos = pstPlayer->stOnline.iMapPos;
				stIdx.iID = pstPlayer->stRoleData.Map;
		
				pstMapIndex =	z_mappos_index(pstEnv->pstMapObj, &stIdx.iPos, stIdx.iID);
				if(pstMapIndex )
				{
					//"XX玩家在XX地点获得了幸运BUFF，大家快去抢啊！"
					z_sys_strf_broadcast(pstEnv, SYS_CHAT, ZONEERR_BROAD340,
					pstPlayer->stRoleData.RoleName,pstMapIndex->stMapDef.Name);
				}
				
			}
			
			pstEnv->iLuckyTipCount++;
			
		}

		
	}
	else
	{
		//pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_LUCK;
	}
	return 0;
}


//一秒一次
int player_beach_tick(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	BEACHINFO *pstBeachInfo = &pstPlayer->stRoleData.MiscInfo.BeachInfo;
	LEVELUP *pstLevel = NULL;
	int iUseVig = 0;
	//ACTIVEHELPER * pstActiveHelper = NULL;

/*
	if (1 == pstEnv->cBeachClosed)
	{
		return -1;
	}
*/
	
	if (pstPlayer->stRoleData.Map != BEACH_MAP_ID )
	{
		if(pstPlayer->stOnline.cMiscFlag & MISC_FLAG_IN_BEACH)
		{
			pstPlayer->stOnline.cMiscFlag &=  ~MISC_FLAG_IN_BEACH;
			player_beach_buff_action(pstEnv, pstPlayer, 0);
		}
		
		return -1;
	}

/*
	if (pstEnv->pstConf->BeachActiveHelperID > 0)
	{
		pstActiveHelper = z_find_active_help_def(pstEnv, pstEnv->pstConf->BeachActiveHelperID, NULL);
	}
	
	if (pstActiveHelper)
	{
		if (autoteam_check_active_running(pstEnv,pstActiveHelper) < 0)
		{
			return -1;
		}
	}
*/
	pstLevel = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);

	if (!pstLevel)
	{
		return -1;
	}

	{
		ZONEIDX stIdx;
		MapIndex * pstMapIdx = NULL;
		TMAPMASK * pstMask = NULL;
		ZONEMAPINST *pstMapInst = NULL; 
		long long llOldFlag = 0;

		stIdx.iID = pstPlayer->stRoleData.Map;
		stIdx.iPos = pstPlayer->stOnline.iMapPos;
		pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx,pstEnv, &stIdx);
				
		if (NULL == pstMapInst)
		{			
			return -1;
		}

		pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);

		if (NULL == pstMapIdx)
		{			
			return -1;
		}

		pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask +pstMapIdx->iMaskOrigin);
		llOldFlag = pstPlayer->stOnline.cMiscFlag;
		if(IsPixelLoMagic(pstMask, pstPlayer->stRoleData.Pos.X,pstPlayer->stRoleData.Pos.Y))
		{	
			pstPlayer->stOnline.cMiscFlag |=  MISC_FLAG_IN_BEACH;
		}
		else
		{
			pstPlayer->stOnline.cMiscFlag &=  ~MISC_FLAG_IN_BEACH;
		}

		if (llOldFlag != pstPlayer->stOnline.cMiscFlag)
		{
			if (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_IN_BEACH)
			{
				int iLeft = (BEACH_EXP_HIGH - pstBeachInfo->Sec + 50) / 60;

				player_beach_buff_action(pstEnv, pstPlayer, 1);
				
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_CHAT,ZONEERR_PLAYER195);

				if (iLeft > 0)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE,ZONEERR_PLAYER196,iLeft);
				}
				else
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE, ZONEERR_PLAYER197);
				}
			}
			else
			{
				player_beach_buff_action(pstEnv, pstPlayer, 0);
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_CHAT, ZONEERR_PLAYER198);
			}
		}
	}

	pstPlayer->stOnline.iBeachMapSec++;
	
	if ( 0 == pstPlayer->stOnline.iBeachMapSec % BEACH_EXP_INTERVAL)
	{
		//int iPara = pstEnv->pstConf->BeachExpNormal;
		int iPara = 0;
		longlong iExp = 0;
		longlong iStealExp = 0;
		int iBeachExpMul = 0;
		int iResultBeach = 0;
	
		if(pstPlayer->stOnline.cMiscFlag & MISC_FLAG_IN_BEACH)
		{		
			if (pstPlayer->stRoleData.MiscInfo.TmpSys.TmpItemID > 0)
			{
				ITEMDEF * pstDef = z_find_itemdef(pstEnv, pstPlayer->stRoleData.MiscInfo.TmpSys.TmpItemID);
				if (pstDef)
				{
					RESULT* pstResult = z_item_get_result(pstDef,RESULT_BEACH_EXP_MUL);
					if (pstResult)
					{
						iBeachExpMul = 1;
						iResultBeach = pstResult->ResultVal1;
					}
				}
			}
			
			if (pstBeachInfo->Sec <= BEACH_EXP_HIGH)
			{
				iPara = pstEnv->pstConf->BeachExpHigh;
			}
			else
			{
				if(iBeachExpMul == 1)
				{
					iPara = pstEnv->pstConf->BeachExpNormal;
				}
			}
		
		}
		
		if (iPara > 0)
		{
			int iMul = pstPlayer->stOnline.stDyn.iBeachExpMul;
			int iDec = 0;
			STATUSRESULT stResultDec;
			STATUSRESULT stResultMul;
			STATUSRESULT stResultSteal;
			STATUSRESULT* pstResultDec = &stResultDec;
			STATUSRESULT* pstResultMul = &stResultMul;
			STATUSRESULT* pstResultSteal = &stResultSteal;
			Player* pstExpPlayer = pstPlayer;
			RIDEDEF* pstRideDef = NULL;
			int iLvl = pstPlayer->stRoleData.Level;
			int iImmune = 0;

			z_get_player_status_result(pstEnv,pstPlayer,STATUS_RESULT_BEACH_EXP_DEC,pstResultDec,0);
			z_get_player_status_result(pstEnv,pstPlayer,STATUS_RESULT_BEACH_EXP_MUL,pstResultMul,0);
			z_get_player_status_result(pstEnv,pstPlayer,STATUS_RESULT_BEACH_EXP_STEAL,pstResultSteal,0);

			{

				ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;				
			
				if (pstRoleTmpSys->TmpItemID)
				{
					ITEMDEF* pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
					
					if (pstItemDef)
					{
						int k = 0;
						
						for (k = 0; k < MAX_ITEM_RESULT; k++)
						{
							RESULT* pstTmpItemResult = pstItemDef->Result + k;
							if (0 == pstTmpItemResult->ResultID)
							{
								break;
							}							
							if (RESULT_BEACH_EXP_MUL_IMMUNE == pstTmpItemResult->ResultID)
							{
								iImmune = 1;
								break;								
							}
							
						}						
					}
					
				}
			}

			// 消耗活力
			// ceiling（（等级+10）/（9-72/等级），1）
			iUseVig = 1.0*(iLvl+10)/(9-72.0f/iLvl)+0.99f;
			if (iUseVig >  (int)pstPlayer->stRoleData.RoleDetail.GreenJade)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE, ZONEERR_PLAYER805);
				return 0;
			}
			else
			{
				player_add_vig(pstEnv,pstPlayer,iUseVig, 1, 1);

				int tSec = 0;
				int tSec2 = 0;
				
				tSec = pstBeachInfo->Sec;
				pstBeachInfo->Sec += BEACH_EXP_INTERVAL;
				tSec2 = pstBeachInfo->Sec;

				if (pstBeachInfo->Sec <= BEACH_EXP_HIGH)
				{
					pet_native_add_money(pstEnv, pstPlayer);
				}

				if(tSec2 >= BEACH_EXP_HIGH && tSec < BEACH_EXP_HIGH)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE, ZONEERR_PLAYER197);
					task_player_trace(pstEnv,pstPlayer,TASK_OBJ_BEACH_TIME_OVER);
					if(iBeachExpMul == 1)
					{
						player_beach_low_exp_ui_to_clt(pstEnv, pstPlayer);
					}
				}
	
				player_beach_info_action(pstEnv, pstPlayer);
				player_beach_buff_action(pstEnv, pstPlayer, 1);
			}

			if(iBeachExpMul == 1)
			{
				iMul += iResultBeach;
			}
			
			if ( pstPlayer->stOnline.RideItemID > 0)
			{
				pstRideDef = z_find_ride_def(pstEnv, pstPlayer->stOnline.RideItemID, pstPlayer->stOnline.RideLevel);
				if(pstRideDef)
				{
					int i = 0;
					for(i = 0;i < MAX_RIDE_RESULT; i++)
					{
						if (0 == pstRideDef->Result[i].ResultID)
						{
							break;
						}
						else if (RESULT_BEACH_EXP_MUL == pstRideDef->Result[i].ResultID)
						{
							iMul += pstRideDef->Result[i].ResultVal1;
						}
					}
				}	
			}

			if (pstResultMul)
			{
				iMul += pstResultMul->ResultVal1;
			}

			if (pstResultDec && 0 == iImmune)
			{
				iDec = pstResultDec->ResultVal1;
			}

			iExp = (pstLevel->ContributeExp*1.0 / iPara) * ( BEACH_EXP_INTERVAL * (100 + iMul) / 100.0);
					
			if (iDec > 0)
			{
				iExp -= iExp * iDec /100.0f;
			}

			if (pstResultSteal)
			{
				ZoneStatus* pstStaus = z_is_player_status_result(pstEnv,pstPlayer,STATUS_RESULT_BEACH_EXP_STEAL);
				if (pstStaus)
				{
					pstExpPlayer = z_id_player(pstEnv->pstAppCtx,pstEnv,pstStaus->iApplyID);
					if (pstExpPlayer)
					{
						if ( 0 == iImmune)
						{
							iStealExp = iExp;						
							iExp /= 2;
							
							iStealExp *= (pstResultSteal->ResultVal1 / 100.0);
							if (pstExpPlayer->stRoleData.Level < pstPlayer->stRoleData.Level)
							{
								LEVELUP * pstExpLevel = z_get_levelup(pstEnv, pstExpPlayer->stRoleData.Level);
								if (pstExpLevel && pstLevel && pstLevel->ContributeExp != 0)
								{
									iStealExp *= (pstExpLevel->ContributeExp * 1.0 / pstLevel->ContributeExp);
								}
							}
						}
					}
					else
					{
						z_player_del_status(pstEnv->pstAppCtx,pstEnv,pstPlayer, pstStaus->unStatusID, 1,pstStaus->iApplyID);
					}
				}
				else
				{
					pstExpPlayer = NULL; 
				}
			}
			
			if (iStealExp > 0 && pstExpPlayer)
			{
				z_player_addexp_test(pstEnv,pstExpPlayer, &iStealExp);

				if (iStealExp > 0)
				{
					PlayerChagAttrVal stChgVal;
					AttrChgVal *pstVal;
				

					stChgVal.AttrNum = 0;					
				
					pstVal = &stChgVal.AttrList[stChgVal.AttrNum++];
					pstVal->AttrID = ATTR_ID_EXP;
					pstVal->nFlag = 0;
					pstVal->Value.Exp = iStealExp;
					
					if (stChgVal.AttrNum > 0)
					{
						z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstExpPlayer, &stChgVal, 2);
					}
				}
			}
			
			if (iExp > 0)
			{
				RESULT *pstVipResult;

				pstVipResult = star_get_result(pstEnv, pstPlayer, STAR_RESULT_SHATAN_EXP);
				if (pstVipResult)
				{
					iExp = iExp * (1.0 + pstVipResult->ResultVal1/100.0);
				}
				
				z_player_addexp_test(pstEnv,pstPlayer, &iExp);

				if (iExp > 0)
				{
					PlayerChagAttrVal stChgVal;
					AttrChgVal *pstVal;
				

					stChgVal.AttrNum = 0;					
				
					pstVal = &stChgVal.AttrList[stChgVal.AttrNum++];
					pstVal->AttrID = ATTR_ID_EXP;
					pstVal->nFlag = 0;
					pstVal->Value.Exp = iExp;
					
					if (stChgVal.AttrNum > 0)
					{
						z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 2);
					}
				}
			}
			
		}
		else
		{
			//tlog_error(pstEnv->pstLogCat, 0, 0, "parameter error iPara=%d", iPara);
		}
	}

	player_beach_info_update(pstEnv, pstPlayer);

	/*
	if (!IsSameDay(pstBeachInfo->EnterTime,	pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstBeachInfo->EnterTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstBeachInfo->Sec = 0;
		pstPlayer->stOnline.iBeachMapSec = 0;
	}
	*/

		
	return 0;
}

int player_world_city_debuff_clear(ZONESVRENV *pstEnv,Player *pstPlayer)
{


	if (pstPlayer->stOnline.iWorldCityBuffFlag & WORLD_CITY_BUFF_FLAG_DEBUFF)
	{ 
		Monster* pstMachine = machine_get_by_player(pstEnv, pstPlayer);
		if(pstMachine)
		{
			if ( 0 == z_mon_del_status(pstEnv->pstAppCtx,pstEnv,pstMachine,WORLD_CITY_DEBUFF_ID, 0))
			{
				z_mon_send_status(pstEnv->pstAppCtx, pstEnv, pstMachine);
			}
		}			
		
		pstPlayer->stOnline.iWorldCityBuffFlag &= ~WORLD_CITY_BUFF_FLAG_DEBUFF;
	}




	return 0;
}

int player_world_city_buff_clear(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	if ( pstPlayer->stOnline.iWorldCityBuffFlag)
	{
		if (pstPlayer->stOnline.iWorldCityBuffFlag & WORLD_CITY_BUFF_FLAG_CONTROL)
		{
			z_player_del_status(pstEnv->pstAppCtx,pstEnv,pstPlayer,WORLD_CITY_CONTROL_BUFF_ID, 1,0);
		}
		
		if (pstPlayer->stOnline.iWorldCityBuffFlag & WORLD_CITY_BUFF_FLAG_BUILD)
		{ 
			z_player_del_status(pstEnv->pstAppCtx,pstEnv,pstPlayer,WORLD_CITY_BUILD_BUFF_ID, 1,0);
		}

		if (pstPlayer->stOnline.iWorldCityBuffFlag & WORLD_CITY_BUFF_FLAG_DEBUFF)
		{ 
			Monster* pstMachine = machine_get_by_player(pstEnv, pstPlayer);
			if(pstMachine)
			{
				if ( 0 == z_mon_del_status(pstEnv->pstAppCtx,pstEnv,pstMachine,WORLD_CITY_DEBUFF_ID, 0))
				{
					z_mon_send_status(pstEnv->pstAppCtx, pstEnv, pstMachine);
				}
			}			
		}

		pstPlayer->stOnline.iWorldCityBuffFlag = 0;
		pstPlayer->stOnline.iWorldCityBuildSerial = 0;

		

	}
	return 0;
}


//3秒一次
int player_world_city_tick(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	if (WORLD_CITY_MAPID != pstPlayer->stRoleData.Map)
	{
		player_world_city_buff_clear(pstEnv,pstPlayer);
		return 0;
	}
	
	//19点到20点,踢人
	if(WORLD_CITY_STAT_CLEAR == pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity.CityCore.Stat &&
		!is_world_city_owner(pstEnv,pstPlayer))
	{
		MapIndex *pstMapIdx;
		RESPOS stExitPos;

		if(pstPlayer->stOnline.State & CS_STAT_HITCH_RIDE)
		{
			z_ride_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}

		pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, BASE_CITY_MAPID);
		if (NULL == pstMapIdx)
		{
			return 0;
		}

		pstPlayer->stRoleData.ExitMap = BASE_CITY_MAPID;
		pstPlayer->stRoleData.ExitPos_x = pstMapIdx->stMapDef.RevivePs[0].X;
		pstPlayer->stRoleData.ExitPos_y= pstMapIdx->stMapDef.RevivePs[0].Y;
		stExitPos.X = pstPlayer->stRoleData.ExitPos_x;
		stExitPos.Y = pstPlayer->stRoleData.ExitPos_y;
		
		z_player_change_map(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
						pstPlayer->stRoleData.ExitMap, &stExitPos, RAND1(180) , 0);

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2,
							ZONEERR_PLAYER199);
		return 0;
	}

	//战斗阶段,buff
	if (is_in_siege_city_fast(pstEnv))
	{
		player_world_city_buff_update(pstEnv,pstPlayer);
		
		if(!IsSameDay(pstPlayer->stRoleData.MiscInfo.EnterCityTime, pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			pstPlayer->stRoleData.MiscInfo.EnterCityTime = pstEnv->pstAppCtx->stCurr.tv_sec;
			pstPlayer->stRoleData.MiscInfo.CityFightTime = 0;
		}

		pstPlayer->stRoleData.MiscInfo.CityFightTime++;
	}
	else
	{
		player_world_city_buff_clear(pstEnv,pstPlayer);
	}
	return 0;
}

int player_enter_world_city(ZONESVRENV *pstEnv,Player *pstPlayer,int iOldMap,RESPOS* pstOldPos)
{
	int iMode;
	if (WORLD_CITY_MAPID != pstPlayer->stRoleData.Map)
	{
		player_pk_mode_chg(pstEnv->pstAppCtx, pstEnv, 
				pstPlayer, pstPlayer->stRoleData.MiscInfo.PkInfo.SavePkMode);
		return 0;
	}
	
	memset(&pstPlayer->stOnline.stCityFightOL, 0, sizeof(pstPlayer->stOnline.stCityFightOL));

	if (is_in_siege_city_fast(pstEnv))
	{
		player_world_city_buff_update(pstEnv,pstPlayer);

		if(!IsSameDay(pstPlayer->stRoleData.MiscInfo.EnterCityTime, pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			pstPlayer->stRoleData.MiscInfo.EnterCityTime = pstEnv->pstAppCtx->stCurr.tv_sec;
			pstPlayer->stRoleData.MiscInfo.CityFightTime = 0;
		}

		world_city_fight_range_svr_info(pstEnv, pstPlayer, 0);
	}

	iMode = pstPlayer->stRoleData.MiscInfo.PkInfo.PkMode;
	player_pk_mode_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, PK_MODE_CLAN);
	pstPlayer->stRoleData.MiscInfo.PkInfo.SavePkMode = iMode;
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER200);
	 	
	pstPlayer->stOnline.iWorldCityBuildSerial = 0;

	pstPlayer->stRoleData.ExitMap = iOldMap;
	pstPlayer->stRoleData.ExitPos_x = pstOldPos->X;
	pstPlayer->stRoleData.ExitPos_y = pstOldPos->Y;
	
	return 0;
}



int player_leave_world_city(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	if (WORLD_CITY_MAPID != pstPlayer->stRoleData.Map)
	{
		return 0;
	}

	player_world_city_buff_clear(pstEnv,pstPlayer);

	pstPlayer->stOnline.iWorldCityBuildSerial = 0;
	
	return 0;
}




int player_world_city_buff_update(ZONESVRENV *pstEnv,Player *pstPlayer)
{	
	if (!is_world_city_owner(pstEnv,pstPlayer))
	{
		//在内城
		if (pstPlayer->stOnline.iWorldCityProcEventFlag & WORLD_CITY_PROC_EVENT_IN) 
		{
			if (!(pstPlayer->stOnline.iWorldCityBuffFlag & WORLD_CITY_BUFF_FLAG_DEBUFF))
			{ 
				Monster* pstMachine = machine_get_by_player(pstEnv, pstPlayer);
				if(pstMachine && !is_city_build_mon(pstMachine))
				{
					if ( 0 == z_mon_apply_status(pstEnv->pstAppCtx,pstEnv,pstMachine,WORLD_CITY_DEBUFF_ID, 1,86400,1,0))
					{
						pstPlayer->stOnline.iWorldCityBuffFlag |= WORLD_CITY_BUFF_FLAG_DEBUFF;
					}
				}			
			}
		}
		else
		{
			if (pstPlayer->stOnline.iWorldCityBuffFlag & WORLD_CITY_BUFF_FLAG_DEBUFF)
			{ 
				Monster* pstMachine = machine_get_by_player(pstEnv, pstPlayer);
				if(pstMachine)
				{
					if ( 0 == z_mon_del_status(pstEnv->pstAppCtx,pstEnv,pstMachine,WORLD_CITY_DEBUFF_ID, 0))
					{
						z_mon_send_status(pstEnv->pstAppCtx, pstEnv, pstMachine);
					}
				}			
				pstPlayer->stOnline.iWorldCityBuffFlag &= ~WORLD_CITY_BUFF_FLAG_DEBUFF;
			}
		}

		if (pstPlayer->stOnline.iWorldCityBuffFlag & WORLD_CITY_BUFF_FLAG_CONTROL)
		{
			z_player_del_status(pstEnv->pstAppCtx,pstEnv,pstPlayer,WORLD_CITY_CONTROL_BUFF_ID, 1,0);
			pstPlayer->stOnline.iWorldCityBuffFlag &= ~WORLD_CITY_BUFF_FLAG_CONTROL;
			pstPlayer->stOnline.iWorldCityBuildSerial = 0;
		}
		
		if (pstPlayer->stOnline.iWorldCityBuffFlag & WORLD_CITY_BUFF_FLAG_BUILD)
		{
			z_player_del_status(pstEnv->pstAppCtx,pstEnv,pstPlayer,WORLD_CITY_BUILD_BUFF_ID, 1,0);
			pstPlayer->stOnline.iWorldCityBuffFlag &= ~WORLD_CITY_BUFF_FLAG_BUILD;
			pstPlayer->stOnline.iWorldCityBuildSerial = 0;
		}
	}
	else
	{
		//在内城
		if (pstPlayer->stOnline.iWorldCityProcEventFlag & WORLD_CITY_PROC_EVENT_IN) 
		{
			if (!(pstPlayer->stOnline.iWorldCityBuffFlag & WORLD_CITY_BUFF_FLAG_DEBUFF))
			{ 
				Monster* pstMachine = machine_get_by_player(pstEnv, pstPlayer);
				if(pstMachine && ! is_city_build_mon(pstMachine))
				{
					if ( 0 == z_mon_apply_status(pstEnv->pstAppCtx,pstEnv,pstMachine,WORLD_CITY_DEBUFF_ID, 1,86400,1,0))
					{
						pstPlayer->stOnline.iWorldCityBuffFlag |= WORLD_CITY_BUFF_FLAG_DEBUFF;
					}
				}			
			}
		}
		else
		{
			if (pstPlayer->stOnline.iWorldCityBuffFlag & WORLD_CITY_BUFF_FLAG_DEBUFF)
			{ 
				Monster* pstMachine = machine_get_by_player(pstEnv, pstPlayer);
				if(pstMachine)
				{
					if ( 0 == z_mon_del_status(pstEnv->pstAppCtx,pstEnv,pstMachine,WORLD_CITY_DEBUFF_ID, 0))
					{
						z_mon_send_status(pstEnv->pstAppCtx, pstEnv, pstMachine);
					}
				}			
				pstPlayer->stOnline.iWorldCityBuffFlag &= ~WORLD_CITY_BUFF_FLAG_DEBUFF;
			}
		}
		
		if (pstPlayer->stOnline.iWorldCityBuildSerial != pstEnv->iWorldCityBuildSerial)
		{			
			int iContrlNum = city_build_control_num_get_byclan(pstEnv,pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity.CityCore.ClanGID);
			int iBuildNum = city_build_buff_num_get_byclan(pstEnv,pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity.CityCore.ClanGID);
			int iDel = 0;

			pstPlayer->stOnline.iWorldCityBuildSerial = pstEnv->iWorldCityBuildSerial;

			
	
			if (iContrlNum > 0)
			{	
				//在内城或 在外城同时增幅器有7个
				if ((pstPlayer->stOnline.iWorldCityProcEventFlag & WORLD_CITY_PROC_EVENT_IN) ||
					((pstPlayer->stOnline.iWorldCityProcEventFlag & WORLD_CITY_PROC_EVENT_OUT) && iBuildNum >= 7))
				{
					
					if (0 == z_player_apply_status(pstEnv->pstAppCtx, pstEnv,pstPlayer,WORLD_CITY_CONTROL_BUFF_ID,iContrlNum,86400,1,0))
					{
						pstPlayer->stOnline.iWorldCityBuffFlag |= WORLD_CITY_BUFF_FLAG_CONTROL;					
					}
					else
					{
						pstPlayer->stOnline.iWorldCityBuildSerial--;
					}
				}
				else
				{
					iDel = 1;
				}
			}
			else
			{
				iDel = 1;
			}

			if (iDel == 1 && (pstPlayer->stOnline.iWorldCityBuffFlag & WORLD_CITY_BUFF_FLAG_CONTROL))
			{
				z_player_del_status(pstEnv->pstAppCtx,pstEnv,pstPlayer,WORLD_CITY_CONTROL_BUFF_ID, 1,0);
				pstPlayer->stOnline.iWorldCityBuffFlag &= ~WORLD_CITY_BUFF_FLAG_CONTROL;
			}
			
			iDel = 0;

			if (iBuildNum > 0)
			{
				//在外城
				if (pstPlayer->stOnline.iWorldCityProcEventFlag & WORLD_CITY_PROC_EVENT_OUT)
				{		
					if (0 == z_player_apply_status(pstEnv->pstAppCtx, pstEnv,pstPlayer,WORLD_CITY_BUILD_BUFF_ID,iBuildNum,86400,1,0))
					{
						pstPlayer->stOnline.iWorldCityBuffFlag |= WORLD_CITY_BUFF_FLAG_BUILD;
					}
					else
					{
						pstPlayer->stOnline.iWorldCityBuildSerial--;
					}
				}
				else
				{
					iDel = 1;
				}
			}
			else
			{
				iDel = 1;
			}

			if (1 == iDel  && (pstPlayer->stOnline.iWorldCityBuffFlag & WORLD_CITY_BUFF_FLAG_BUILD))
			{
				z_player_del_status(pstEnv->pstAppCtx,pstEnv,pstPlayer,WORLD_CITY_BUILD_BUFF_ID, 1,0);
				pstPlayer->stOnline.iWorldCityBuffFlag &= ~WORLD_CITY_BUFF_FLAG_BUILD;
			}
		}
		 
	}
	

	return 0;
}


int open_chg_name_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID,int iType)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	UNUSED(pstAppCtx);

	pstActionRes->ActionID = ACTION_UI;
	pstActionRes->Data.UI.UIType = iType;
	pstActionRes->ID = iNpcID;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int player_ui(ZONESVRENV* pstEnv,Player *pstPlayer,Npc* pstNpc,int iType)
{
	pstPlayer->stOnline.cUIType = iType;

	pstNpc->stMon.tFreezeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + 30;
	if (pstNpc->stMon.cMoving)
	{
		z_mon_stop(pstEnv, &pstNpc->stMon); 
	}

	switch(iType)
	{
	case  UI_TYPE_STASH:
		return open_stash(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstNpc->stMon.iID);
		break;
	case UI_TYPE_CHANGE_COIN_INTO:
		return open_coin_to_bull_ui(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstNpc->stMon.iID);
		break;
	case UI_TYPE_SKILL:
		return open_skill_ui(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstNpc->stMon.iID);
		break;
	case UI_TYPE_DIATHESIS:
		return open_diathesis_ui(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstNpc->stMon.iID);
		break;
	case UI_TYPE_PET:
		return open_pet_clt_ui(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstNpc->stMon.iID);
		break;
	case UI_TYPE_FIT:
		return open_skill_fitting_ui(pstEnv->pstAppCtx, pstEnv, 
			pstPlayer, pstNpc->stMon.iID,UI_TYPE_FIT);
		break;
	case UI_TYPE_CLAN_CREATE:
	case UI_TYPE_CLAN_DISBAND:
	case UI_TYPE_CLAN_MANIFESTO:
	case UI_TYPE_CLAN_SEARCH:
	case UI_TYPE_CLAN_RESEARCH:
	case UI_TYPE_CLAN_MAKING:
	case UI_TYPE_CLAN_STORAGE:
	case UI_TYPE_CLAN_FACTORY:
	case UI_TYPE_UPGRADE_CLAN:
	case UI_TYPE_UPGRADE_CLAN_CITY:
	case UI_TYPE_UPGRADE_TECH_CENTER:
	case UI_TYPE_UPGRADE_COLLECTION_CENTER:
	case UI_TYPE_UPGRADE_FACTORY:
	case UI_TYPE_UPGRADE_FARM:
	case UI_TYPE_UPGRADE_WALL:
		return clan_open_generic_ui(pstEnv, pstPlayer, pstNpc->stMon.iID, iType);
		break;
	case UI_TYPE_CHG_ROLE_NAME:
	case UI_TYPE_CHG_CLAN_NAME:
		return  open_chg_name_ui(pstEnv->pstAppCtx, 
					pstEnv, pstPlayer, pstNpc->stMon.iID,iType);
		break;
		
	case UI_TYPE_ARM_STAR:
	case UI_TYPE_ARM_FIGHT:
	case UI_TYPE_ARM_LEVEL:
	case UI_TYPE_ARM_IDENTIFIABILITY:
	case UI_TYPE_GEM_LEVEL_UP:
	case UI_TYPE_GEM_INLAY:
	case UI_TYPE_GEM_PICK:
	case UI_TYPE_ARM_BIND:
	case UI_TYPE_ARM_UNBIND:
	case UI_TYPE_ARM_MOVE:	
	case UI_TYPE_ARM_WASH:
		return open_arm_ui(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstNpc->stMon.iID, iType);
		break;
	case UI_TYPE_SKILL_FITTING_INLAY:
		return open_skill_fitting_ui(pstEnv->pstAppCtx, pstEnv, 
			pstPlayer, pstNpc->stMon.iID,UI_TYPE_SKILL_FITTING_INLAY);
	case UI_TYPE_SKILL_FITTING_PICK:
		return open_skill_fitting_ui(pstEnv->pstAppCtx, pstEnv, 
			pstPlayer, pstNpc->stMon.iID,UI_TYPE_SKILL_FITTING_PICK);
	case UI_TYPE_PET_WASH_GENIUS:
	case UI_TYPE_PET_WASH_SKILL_GRID:
	case UI_TYPE_PET_SKILL_LEARN:
	case UI_TYPE_PET_SKILL_UP:
	case UI_TYPE_PET_ADD_ATTR:
	case UI_TYPE_PET_ADD_SKILL_GRID:
	case UI_TYPE_PET_STRENGTHEN:
	case UI_TYPE_PET_SKILL_RESET:
		return open_pet_ui(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstNpc->stMon.iID, iType);
	case UI_TYPE_GENIUS:
		return open_genius_ui(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstNpc->stMon.iID);
		break;
	case UI_TYPE_BCT:
		return open_bct_ui(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstNpc->stMon.iID);
		break;
	case UI_TYPE_MINING_RECIPE:
	case UI_TYPE_HERBALISM_RECIPE:
	case UI_TYPE_RESOLVE_RECIPE:
	case UI_TYPE_BLACKSMITHING_RECIPE:
	case UI_TYPE_ALCHEMY_RECIPE:
	case UI_TYPE_ENCHANT_RECIPE:
	case UI_TYPE_ENGINEERING_RECIPE:
	case UI_TYPE_CAREER_CHG:
		return open_ui(pstEnv, pstPlayer, pstNpc->stMon.iID, iType);
		break;
	case UI_TYPE_SAFEPWD_CANCEL:
	case UI_TYPE_SAFEPWD_MODIFY:
		return open_ui(pstEnv, pstPlayer, pstNpc->stMon.iID, iType);
		break;
	case UI_TYPE_ARM_UNFENGYIN:
		return open_ui(pstEnv, pstPlayer, pstNpc->stMon.iID, iType);
		break;

	case UI_TYPE_MAP_TRANS:
		return 0;//不需要发协议
		break;
	case UI_TYPE_WORLD_CITY:
		return player_open_world_city_ui_info(pstEnv, pstPlayer, pstNpc->stMon.iID);
		break;
	case UI_TYPE_UNREAL:
		return open_ui(pstEnv, pstPlayer, pstNpc->stMon.iID, iType);
		break;
	case UI_TYPE_SPY_TRANS:
		return open_ui(pstEnv, pstPlayer, pstNpc->stMon.iID, iType);
		break;

	case UI_TYPE_CLAN_DONATE_LOG:
		return open_ui(pstEnv, pstPlayer, pstNpc->stMon.iID, iType);
		break;
	case UI_TYPE_CLAN_GET_RES_INFO:
		return player_open_res_info_ui(pstEnv, pstPlayer, pstNpc->stMon.iID, iType);
		break;
	case UI_TYPE_SET_BANNER:
		return strong_open_set_banner_ui(pstEnv, pstPlayer,  pstNpc);
		break;
	case UI_TYPE_SET_BANNER_XUANYAN:
		return strong_open_set_banner_xuanyan_ui(pstEnv, pstPlayer,  pstNpc);
		break;
	case UI_TYPE_RESET_RONGLU:
		return open_ui(pstEnv, pstPlayer, pstNpc->stMon.iID, iType);
		break;
	case UI_TYPE_ADDATTR_MOVE:
		open_ui(pstEnv, pstPlayer, pstNpc->stMon.iID, iType);
		break;
	case UI_TYPE_ATTR_RESET:
		open_ui(pstEnv, pstPlayer, pstNpc->stMon.iID, iType);
		break;
	case UI_TYPE_PET_MOVE_STAR:
		open_ui(pstEnv, pstPlayer, pstNpc->stMon.iID, iType);
		break;
	case UI_TYPE_PET_EAT:
	case UI_TYPE_PET_STEP_UP:
	case UI_TYPE_CLAN_BOON:
	case UI_TYPE_ADDATTR_MOVE_GAOJI:
	case UI_TYPE_STUDENT_REG:
	case UI_TYPE_ARM_STEP_UP:
	case UI_TYPE_ARM_SKILL_MOVE_HIGH:
	case UI_TYPE_ARM_SKILL_MOVE_LOW:
	case UI_TYPE_PET_IMP_UP:
	case UI_TYPE_HOME:
		open_ui(pstEnv, pstPlayer, pstNpc->stMon.iID, iType);
		break;
	default:
		return open_ui(pstEnv, pstPlayer, pstNpc->stMon.iID, iType);
		break;
	}

	return 0;
}


int player_beach_check_chair(ZONESVRENV *pstEnv,Player *pstPlayer,int iRange)
{
	int i = 0;
	int iMemID = 0;
	char cType = 0;
	int iIndex = 0; 
	TMEMBLOCK* pstMemBlock = NULL;
	Player* pstTmpOther = NULL;
	int iDis = 0;
	ITEMDEF* pstItemDef = NULL;
	RESULT* pstResult = NULL;
	DropItem* pstDropItem = NULL;
		
	for(i = 0 ; i < pstPlayer->stOnline.iVAnimate; i++)
	{
		iMemID = pstPlayer->stOnline.aiVAnimate[i];
		iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, iMemID, &cType);
		if (iIndex < 0 )
		{
			continue;
		}
		switch (cType) 
		{
		case OBJ_PLAYER:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				continue;
			}
			
			pstTmpOther = (Player*)pstMemBlock->szData;

			if (0 == pstTmpOther->stRoleData.MiscInfo.TmpSys.TmpItemID)
			{
				continue;
			}

			iDis = z_distance(&pstPlayer->stRoleData.Pos,&pstTmpOther->stRoleData.Pos);
			if (iDis > iRange)
			{
				continue;
			}

			pstItemDef = z_find_itemdef(pstEnv, pstTmpOther->stRoleData.MiscInfo.TmpSys.TmpItemID);
			if (!pstItemDef)
			{
				continue;
			}

			pstResult = z_item_get_result(pstItemDef,RESULT_BEACH_EXP_MUL);

			if (pstResult)
			{
				return -1;
			}
			
			break;

		case OBJ_ITEM:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstItemPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				continue;
			}
			
			pstDropItem = (DropItem*)pstMemBlock->szData;

			iDis = z_distance(&pstPlayer->stRoleData.Pos,&pstDropItem->stPos);
			if (iDis > iRange)
			{
				continue;
			}

			pstItemDef = z_find_itemdef(pstEnv, pstDropItem->iDefIdx);
			if (!pstItemDef)
			{
				continue;
			}

			pstResult = z_item_get_result(pstItemDef,RESULT_BEACH_EXP_MUL);

			if (pstResult)
			{
				return -1;
			}
			
			break;
		default:
			break;
		}
			
	}
	
	for(i = 0 ; i < pstPlayer->stOnline.iVInanimate; i++)
	{
		iMemID = pstPlayer->stOnline.aiVInanimate[i];
		iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, iMemID, &cType);
		if (iIndex < 0 )
		{
			continue;
		}
		switch (cType) 
		{
		case OBJ_PLAYER:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				continue;
			}
			
			pstTmpOther = (Player*)pstMemBlock->szData;

			if (0 == pstTmpOther->stRoleData.MiscInfo.TmpSys.TmpItemID)
			{
				continue;
			}

			iDis = z_distance(&pstPlayer->stRoleData.Pos,&pstTmpOther->stRoleData.Pos);
			if (iDis > iRange)
			{
				continue;
			}

			pstItemDef = z_find_itemdef(pstEnv, pstTmpOther->stRoleData.MiscInfo.TmpSys.TmpItemID);
			if (!pstItemDef)
			{
				continue;
			}

			pstResult = z_item_get_result(pstItemDef,RESULT_BEACH_EXP_MUL);

			if (pstResult)
			{
				return -1;
			}
			
			break;

		case OBJ_ITEM:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstItemPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				continue;
			}
			
			pstDropItem = (DropItem*)pstMemBlock->szData;

			iDis = z_distance(&pstPlayer->stRoleData.Pos,&pstDropItem->stPos);
			if (iDis > iRange)
			{
				continue;
			}

			pstItemDef = z_find_itemdef(pstEnv, pstDropItem->iDefIdx);
			if (!pstItemDef)
			{
				continue;
			}

			pstResult = z_item_get_result(pstItemDef,RESULT_BEACH_EXP_MUL);

			if (pstResult)
			{
				return -1;
			}
			
			break;
		default:
			break;
		}
			
	}
	return 0;
}


int player_gold_raffle_num_notify(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	CSPKG stPkg;
	CSGOLDRAFFLESVR *pstNotify = &stPkg.Body.GoldRaffleSvr;

	pstNotify->RaffleNum = pstPlayer->stRoleData.MiscInfo.GoldRaffle;

	Z_CSHEAD_INIT(&stPkg.Head, GOLD_RAFFLE_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;	
}

int player_is_zuolao(Player *pstPlayer)
{
	RESPOS stPos;

	if(g_SpousePos == 0)
	{
		if (pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal < PLAYER_PK_PUBLIC_ENEMY)
		{
			return 0;
		}
	}
	
	if (GET_MAPID(pstPlayer->stRoleData.Map) != 1000)
	{
		return 0;
	}

	stPos.X = ZUOLAO_CENTER_X;
	stPos.Y = ZUOLAO_CENTER_Y;
	if (z_distance(&pstPlayer->stRoleData.Pos, &stPos) <= ZUOLAO_RADIUS)
	{
		return 1;
	}
	return 0;
}

MEDIAGIFTDEF * find_mediagiftdef(ZONESVRENV* pstEnv, int iMediaGiftID)
{
	MEDIAGIFTDEF stMediaGiftDef;
	int iEqu,iMediaGiftIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stMediaGiftDef.MediaGiftID = iMediaGiftID;
	iMediaGiftIndex = bsearch_int(&stMediaGiftDef, pstObj->sMediaGiftDef, pstObj->iMediaGiftDef, GIFT_MEDIA_DATA, &iEqu);
	if (iEqu)
	{
		return (MEDIAGIFTDEF *)&pstObj->sMediaGiftDef[iMediaGiftIndex];
	}
	
	return NULL;
}

LTRGIFTDEF* find_ltrgiftdef(ZONESVRENV* pstEnv, int iLtrID)
{
	LTRGIFTDEF stLtrGiftDef;
	int iEqu,iLtrGiftIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stLtrGiftDef.LtrID= iLtrID;
	iLtrGiftIndex = bsearch_int(&stLtrGiftDef, pstObj->sLtrGiftDef, pstObj->iLtrGiftDef, GIFT_LTR_DATA, &iEqu);
	if (iEqu)
	{
		return (LTRGIFTDEF *)&pstObj->sLtrGiftDef[iLtrGiftIndex];
	}
	
	return NULL;
}

int player_mediagift_use_check(ZONESVRENV * pstEnv,Player * pstPlayer,int iMediaGiftID,int selectOneItemIndex, ROLEITEM *pstRoleItem)
{
	MEDIAGIFTDEF * pstMediaGiftDef;
	int i;
	int iItems=0;
	ROLEGRID stGridTmp[MEDIAGIFT_OPTIONS_ITEM_MAX+1];
	int isNSelOne=1;//是否有N选1项
	pstMediaGiftDef = find_mediagiftdef(pstEnv, iMediaGiftID);
	if(!pstMediaGiftDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PLAYER201);
		return -1;
	}
	//N选1数组第一个组有值，则表示此礼包有N选1项
	if(pstMediaGiftDef->SelectOneGift.GiftItems[0].ItemID > 0)
	{
		if(selectOneItemIndex == 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PLAYER202);
			return -1;
		}
		if(!(selectOneItemIndex >= 1 && selectOneItemIndex <= MEDIAGIFT_OPTIONS_ITEM_MAX && pstMediaGiftDef->SelectOneGift.GiftItems[selectOneItemIndex-1].ItemID > 0))
		{
			return -1;
		}
	}
	else
	{
		isNSelOne = 0;
	}
		
	memset(stGridTmp,0,sizeof(stGridTmp));
	//必得道具
	for(i=0;i<MEDIAGIFT_OPTIONS_ITEM_MAX;i++)
	{
		if(pstMediaGiftDef->SelectAllGift.GiftItems[i].ItemID > 0)
		{
			if(0 == item_create(pstEnv->pstAppCtx, pstEnv, &stGridTmp[i], pstMediaGiftDef->SelectAllGift.GiftItems[i].ItemID))
			{
				stGridTmp[iItems].GridData.RoleItem.Num = pstMediaGiftDef->SelectAllGift.GiftItems[iItems].ItemNum;
				if (is_bind_roleitem(pstRoleItem))
				{
					stGridTmp[iItems].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
				}
				iItems++;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			break;
		}
	}
	//N选一
	if(isNSelOne)
	{
		if(0 == item_create(pstEnv->pstAppCtx, pstEnv, &stGridTmp[iItems], pstMediaGiftDef->SelectOneGift.GiftItems[selectOneItemIndex-1].ItemID))
		{
			stGridTmp[iItems].GridData.RoleItem.Num = pstMediaGiftDef->SelectOneGift.GiftItems[selectOneItemIndex-1].ItemNum;
			if (is_bind_roleitem(pstRoleItem))
			{
				stGridTmp[iItems].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
			}
			iItems++;
		}
		else
		{
			return -1;
		}
	}
	
	if(0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, stGridTmp, iItems, 0))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NOSPACE);
		return -1;
	}
	return 0;
}

int player_mediagift_use(ZONESVRENV * pstEnv,Player * pstPlayer,int iMediaGiftID,int selectOneItemIndex,ROLEITEM *pstRoleItem,char * szItemName)
{
	MEDIAGIFTDEF * pstMediaGiftDef;
	int i;
	int iItems=0;
	int isNSelOne = 0;
	ROLEGRID stGridTmp[MEDIAGIFT_OPTIONS_ITEM_MAX+1];
	pstMediaGiftDef = find_mediagiftdef(pstEnv, iMediaGiftID);
		
	if(!pstMediaGiftDef)
	{
		return -1;
	}
	if(pstMediaGiftDef->SelectOneGift.GiftItems[0].ItemID > 0)
	{
		isNSelOne = 1;
	}

	memset(stGridTmp,0,sizeof(stGridTmp));
	//必得道具
	for(i=0;i<MEDIAGIFT_OPTIONS_ITEM_MAX;i++)
	{
		if(pstMediaGiftDef->SelectAllGift.GiftItems[i].ItemID > 0)
		{
			if(0 == item_create(pstEnv->pstAppCtx, pstEnv, &stGridTmp[i], pstMediaGiftDef->SelectAllGift.GiftItems[i].ItemID))
			{
				stGridTmp[iItems].GridData.RoleItem.Num = pstMediaGiftDef->SelectAllGift.GiftItems[iItems].ItemNum;
				if (is_bind_roleitem(pstRoleItem))
				{
					stGridTmp[iItems].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
				}
				iItems++;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			break;
		}
	}
	//N选一
	if(isNSelOne)
	{
		if(0 == item_create(pstEnv->pstAppCtx, pstEnv, &stGridTmp[iItems], pstMediaGiftDef->SelectOneGift.GiftItems[selectOneItemIndex-1].ItemID))
		{
			stGridTmp[iItems].GridData.RoleItem.Num = pstMediaGiftDef->SelectOneGift.GiftItems[selectOneItemIndex-1].ItemNum;
			if (is_bind_roleitem(pstRoleItem))
			{
				stGridTmp[iItems].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
			}
			iItems++;
		}
		else
		{
			return -1;
		}
	}
	if(0 > package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, stGridTmp,  iItems,0, IMPITEM_OPTYPE_GIFT))
	{
		return -1;
	}
	
	//日志
	{
		int iSelAllGiftItemsLen;
		GIFTITEM * pstSelOneGiftItem = NULL;
		GIFTITEM * pstSelAllGiftItem;
		if(isNSelOne)
		{
			iSelAllGiftItemsLen = iItems -1;
			pstSelOneGiftItem = &pstMediaGiftDef->SelectOneGift.GiftItems[selectOneItemIndex-1];
		}
		else
		{
			iSelAllGiftItemsLen = iItems;
		}
		pstSelAllGiftItem = pstMediaGiftDef->SelectAllGift.GiftItems;
		z_role_media_gift_log(pstEnv,pstPlayer,szItemName,pstRoleItem->DefID,pstSelOneGiftItem,pstSelAllGiftItem,iSelAllGiftItemsLen);
		//GIFTITEM astSelAllGiftItem[iSelAllGiftItemsLen];
		//memcpy(astSelAllGiftItem,pstMediaGiftDef->SelectAllGift.GiftItems,iSelAllGiftItemsLen*sizeof(GIFTITEM));
		//z_role_media_gift_log(pstEnv,pstPlayer,szItemName,iUseItemID,iMediaGiftID,pstSelOneGiftItem,astSelAllGiftItem,iSelAllGiftItemsLen);
	}
	return 0;
}

LTRGIFTDEF * find_ltrgiftdef_bytimes(ZONESVRENV * pstEnv,Player * pstPlayer,int iLtrGiftID,int iItemID)
{
	int i,iLastRangeTimes,iMaxTime;
	LTRGIFTDEF * pstLtrGiftDef;
	ROLELTRINFO * pstRoleLtrInfo;
	ROLELTRINFODETAIL * pstRoleLtrInfoDetail;
	pstLtrGiftDef = find_ltrgiftdef(pstEnv, iLtrGiftID);
	if(!pstLtrGiftDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv,pstPlayer,SYS_MIDDLE, ZONEERR_PLAYER148);
		return NULL;
	}
	iMaxTime = pstLtrGiftDef->MaxTime;	//此礼包一个循环最大次数
	if(0 > iMaxTime)
	{
		return NULL;
	}
	pstRoleLtrInfo = &pstPlayer->stRoleData.MiscInfo.LtrInfo;
	for(i=0;i<pstRoleLtrInfo->Count;i++)
	{
		pstRoleLtrInfoDetail = &pstRoleLtrInfo->RoleLtrInfoDetail[i];
		if(iItemID == pstRoleLtrInfoDetail->ItemID)
		{
			if(pstRoleLtrInfoDetail->Num >= iMaxTime)
			{
				break;
			}
			//for(;RoleLtrInfoDetail.Num >= pstLtrGiftDef->RangeTime;)
			while(pstRoleLtrInfoDetail->Num >= pstLtrGiftDef->RangeTime)
			{
				iLastRangeTimes = pstLtrGiftDef->RangeTime;
				pstLtrGiftDef = find_ltrgiftdef(pstEnv,pstLtrGiftDef->NextLtrGift.LtrID);
				if(!pstLtrGiftDef)
				{
					return NULL;
				}
				if(pstLtrGiftDef->RangeTime > iMaxTime || pstLtrGiftDef->RangeTime <= iLastRangeTimes)
				{
					return NULL;
				}
			}
			break;
		}
	}
	return pstLtrGiftDef;
}

int player_ltrinfo_chg(ZONESVRENV * pstEnv,Player * pstPlayer,int iLtrGiftID,int iItemID)
{
	int i;
	int flag = 0;	//标识此礼包ID在数组【pstRoleLtrInfo->RoleLtrInfoDetail】中是否存在
	CSPKG stPkgRes;
	CSROLELTRINFO *pstCSRoleLtrInfo;
	ROLELTRINFODETAIL RoleLtrInfoDetail;
	ROLELTRINFO * pstRoleLtrInfo = &pstPlayer->stRoleData.MiscInfo.LtrInfo;
	LTRGIFTDEF * pstLtrGiftDef = find_ltrgiftdef(pstEnv, iLtrGiftID);
	if(!pstLtrGiftDef)
	{
		return -1;
	}
	
	for(i=0;i<pstRoleLtrInfo->Count;i++)
	{
		if(iItemID == pstRoleLtrInfo->RoleLtrInfoDetail[i].ItemID)
		{
			RoleLtrInfoDetail = pstRoleLtrInfo->RoleLtrInfoDetail[i];
			if(RoleLtrInfoDetail.Num >= pstLtrGiftDef->MaxTime)
			{
				RoleLtrInfoDetail.Num = 0;
			}
			RoleLtrInfoDetail.Num++;
			pstRoleLtrInfo->Count--;
			flag = 1;
			memmove(&pstRoleLtrInfo->RoleLtrInfoDetail[i], &pstRoleLtrInfo->RoleLtrInfoDetail[i+1], 
				   (pstRoleLtrInfo->Count - i) * sizeof(pstRoleLtrInfo->RoleLtrInfoDetail[0]));
			break;
		}
	}
	
	if(!flag)
	{
		if(pstRoleLtrInfo->Count >= MAX_ROLE_LTRINFO_NUM)
		{
			//数组已满，将最老的数据(即第一个顶出去)
			pstRoleLtrInfo->Count  = MAX_ROLE_LTRINFO_NUM - 1;
			memmove(&pstRoleLtrInfo->RoleLtrInfoDetail[0], &pstRoleLtrInfo->RoleLtrInfoDetail[1], 
				   (MAX_ROLE_LTRINFO_NUM - 1) * sizeof(pstRoleLtrInfo->RoleLtrInfoDetail[0]));
		}
		RoleLtrInfoDetail.ItemID = iItemID;
		RoleLtrInfoDetail.Num = 1;
	}
	
	if (!(RoleLtrInfoDetail.Num >= pstLtrGiftDef->MaxTime && pstLtrGiftDef->EverLoop))
	{
		pstRoleLtrInfo->RoleLtrInfoDetail[pstRoleLtrInfo->Count].ItemID = RoleLtrInfoDetail.ItemID;
		pstRoleLtrInfo->RoleLtrInfoDetail[pstRoleLtrInfo->Count].Num = RoleLtrInfoDetail.Num;
		pstRoleLtrInfo->Count++;
	}
		
	pstCSRoleLtrInfo = &stPkgRes.Body.LtrInfoSvr;
	memcpy(&pstCSRoleLtrInfo->RoleLtrData,pstRoleLtrInfo,sizeof(pstCSRoleLtrInfo->RoleLtrData));
	Z_CSHEAD_INIT(&stPkgRes.Head, LTRINFO_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);
	return 0;
}

int player_ltrgift_use_check(ZONESVRENV * pstEnv,Player * pstPlayer,int iLtrGiftID,ROLEITEM *pstRoleItem)
{
	int i;
	ROLEGRID stGridTmp;
	LTRGIFTITEM * pstLtrGiftItem;
	LTRGIFTDEF * pstLtrGiftDef;
	ROLELTRINFODETAIL RoleLtrInfoDetail;
	ROLELTRINFO * pstRoleLtrInfo = &pstPlayer->stRoleData.MiscInfo.LtrInfo;
	
	pstLtrGiftDef = find_ltrgiftdef_bytimes(pstEnv,pstPlayer,iLtrGiftID, pstRoleItem->DefID);
	if(!pstLtrGiftDef)
	{
		return -1;
	}
	for(i=0; i<MEDIAGIFT_OPTIONS_ITEM_MAX; i++)
	{	
		pstLtrGiftItem = &pstLtrGiftDef->LtrGiftItems.GiftItems[i];
		if(pstLtrGiftItem->ItemID > 0)
		{
			if(pstLtrGiftItem->Rate <= 0)
			{
				continue;
			}
			if(0 == item_create(pstEnv->pstAppCtx, pstEnv, &stGridTmp, pstLtrGiftItem->ItemID))
			{
				stGridTmp.GridData.RoleItem.Num = pstLtrGiftItem->ItemNum;
				if (is_bind_roleitem(pstRoleItem))
				{
					stGridTmp.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
				}

				if(0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stGridTmp, 1, 0))
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NOSPACE);
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
			break;
		}
	}

	for(i=0;i<pstRoleLtrInfo->Count;i++)
	{
		if(pstRoleItem->DefID == pstRoleLtrInfo->RoleLtrInfoDetail[i].ItemID)
		{
			RoleLtrInfoDetail = pstRoleLtrInfo->RoleLtrInfoDetail[i];
			if((RoleLtrInfoDetail.Num >= pstLtrGiftDef->MaxTime || RoleLtrInfoDetail.Num == 0) && 0 == pstLtrGiftDef->EverLoop)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PLAYER149, pstLtrGiftDef->MaxTime);
				return -1;
			}
			
			break;
		}
	}
	
	return 0;
}

int player_ltrgift_use(ZONESVRENV * pstEnv,Player * pstPlayer,int iLtrGiftID,ROLEITEM *pstRoleItem,char * szItemName)
{
	int i,iRateCount=0;
	int iRnd;
	int iSelIdx = -1;
	int iCurrRate=0;
	ROLEGRID stGridTmp;
	LTRGIFTDEF * pstLtrGiftDef;
	pstLtrGiftDef = find_ltrgiftdef_bytimes(pstEnv,pstPlayer,iLtrGiftID,pstRoleItem->DefID);
	if(!pstLtrGiftDef)
	{
		return -1;
	}
	for(i=0;i<MEDIAGIFT_OPTIONS_ITEM_MAX;i++)
	{
		if(pstLtrGiftDef->LtrGiftItems.GiftItems[i].ItemID >0)
		{
			iRateCount += pstLtrGiftDef->LtrGiftItems.GiftItems[i].Rate;
		}
		else
		{
			break;
		}
	}

	iRnd = (int )RAND1(iRateCount);
	for(i=0;i<MEDIAGIFT_OPTIONS_ITEM_MAX;i++)
	{
		if(pstLtrGiftDef->LtrGiftItems.GiftItems[i].ItemID >0)
		{
			iCurrRate+= pstLtrGiftDef->LtrGiftItems.GiftItems[i].Rate;
			if(iRnd < iCurrRate)
			{
				iSelIdx = i;
				if(0 == item_create(pstEnv->pstAppCtx, pstEnv, &stGridTmp, pstLtrGiftDef->LtrGiftItems.GiftItems[iSelIdx].ItemID))
				{
					stGridTmp.GridData.RoleItem.Num = pstLtrGiftDef->LtrGiftItems.GiftItems[iSelIdx].ItemNum;
					if (is_bind_roleitem(pstRoleItem))
					{
						stGridTmp.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
					}
					package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stGridTmp, 1, 0, IMPITEM_OPTYPE_GIFT);
					player_ltrinfo_chg(pstEnv,pstPlayer,iLtrGiftID,pstRoleItem->DefID);
				}
				break;
			}
		}
		else
		{
			break;
		}
	}
	
	//日志
	if(iSelIdx >= 0)
	{
		ROLELTRINFO * pstRoleLtrInfo = &pstPlayer->stRoleData.MiscInfo.LtrInfo;
		ROLELTRINFODETAIL * pstRoleLtrInfoDetail;
		LTRGIFTDEF * pstLtrGiftDef = find_ltrgiftdef(pstEnv, iLtrGiftID);
		GIFTITEM stGiftItem;
		int iUseNum = pstLtrGiftDef->MaxTime;
		for(i=0;i<pstRoleLtrInfo->Count;i++)
		{
			pstRoleLtrInfoDetail = &pstRoleLtrInfo->RoleLtrInfoDetail[i];
			if(pstRoleItem->DefID == pstRoleLtrInfoDetail->ItemID)
			{
				iUseNum = pstRoleLtrInfoDetail->Num;
				break;
			}
		}
		stGiftItem.ItemID = pstLtrGiftDef->LtrGiftItems.GiftItems[iSelIdx].ItemID;
		stGiftItem.ItemNum = pstLtrGiftDef->LtrGiftItems.GiftItems[iSelIdx].ItemNum;
		z_role_ltr_gift_log(pstEnv,pstPlayer,szItemName, pstRoleItem->DefID,iUseNum,&stGiftItem);
	}
	
	return 0;
}

int player_daily_goal_info(ZONESVRENV * pstEnv,Player * pstPlayer)
{
	DAILYACTIVEINFO* pstInfo = &pstPlayer->stRoleData.MiscInfo.DailyActive;
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;
	
	if (!IsSameDay(pstInfo->MinLevelTime,pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstInfo->MinLevelTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstInfo->MinLevel = pstPlayer->stRoleData.Level;
		pstInfo->TaskNum = 0;
		pstInfo->TakeAwardFlag = 0;
	}

	

	memset(pstAction,0,sizeof(*pstAction));

	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_DAILY_GOAL_INFO;
	
	pstAction->Data.DailyGoalInfo.MinLevel = pstInfo->MinLevel;
	pstAction->Data.DailyGoalInfo.TaskNum = pstInfo->TaskNum;
	pstAction->Data.DailyGoalInfo.TakeAwardFlag = pstInfo->TakeAwardFlag;
	 

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
			
	return 0;
}


int player_dyn_active_helper_info(ZONESVRENV * pstEnv,Player * pstPlayer,int iID,int iStatus)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;

	memset(pstAction,0,sizeof(*pstAction));

	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_DYN_ACTIVE_INFO;
	
	pstAction->Data.DynActiveInfo.ActiveHelpID = iID;
	pstAction->Data.DynActiveInfo.Stauts = iStatus;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	if (iID == CLAN_TOUCH_ACTIVE_HELP_ID)
	{
		CSACTIVEGAMESVR *pstActiveGameSvr = &stPkg.Body.ActiveGameSvr;
		CSACTIVEGAMEUIINFO *pstActiveUIInfo = &pstActiveGameSvr->Data.UIInfo;

		int iType = ACTIVE_GAME_SVR_TYPE_UI_CLOSE;
		
		memset(pstActiveGameSvr,0,sizeof(*pstActiveGameSvr));

		

		if (iStatus == ACTIVE_STATUS_RUNNING)
		{
			iType = ACTIVE_GAME_SVR_TYPE_UI_INFO;
		}
		
		pstActiveGameSvr->Type = iType;
		pstActiveUIInfo->UIType = ACTIVE_UI_TYPE_CLAN_TOUCH;
		
		Z_CSHEAD_INIT(&stPkg.Head, ACTIVE_GAME_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	
	return 0;
}

int all_player_dny_active_helper_info(ZONESVRENV * pstEnv,int iID,int iStatus)
{
	int i = 0;
	for (i=0; i<MAX_PLAYER; i++)
	{
		TMEMBLOCK* pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		Player * pstPlayer = NULL;
		
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (Player *)pstMemBlock->szData;		
		
		if(PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus ) 
		{
			player_dyn_active_helper_info(pstEnv,pstPlayer,iID,iStatus);
		}
	}
	return 0;
}

ROLEARM * player_get_wear_rolearm_by_grididx(Player * pstPlayer,int gridIdx)
{
	int i;
	ROLEARM * pstRoleArm = NULL;
	ROLEWEAR * pstRoleWear = &pstPlayer->stRoleData.Wear; 
	for(i=0;i<pstRoleWear->ArmNum;i++)
	{
		if(pstRoleWear->RoleArms[i].GridIdx == gridIdx)
		{
			pstRoleArm = pstRoleWear->RoleArms + i;
			break;
		}
	}
	if(i == pstRoleWear->ArmNum)
	{
		return NULL;
	}
	return pstRoleArm;
}


// 是否魔法职业
int player_is_magic_career(int iCareer)
{
	switch (iCareer)
	{
	case CAREER_MAGICOR:
	case CAREER_MAGICOR_SINGLEHAND:
	case CAREER_MAGICOR_DOUBLEHAND:
	case CAREER_CHURCH:
	case CAREER_CHURCH_SINGLEHAND:
	case CAREER_CHURCH_DOUBLEHAND:
		return 1;
	default:
		return 0;
	}
	
	return 0;
}

// 去informer_svr查找信息。
int z_ss_req_get_account(TAPPCTX * pstAppCtx, ZONESVRENV* pstEnv,
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
	iDst = pstEnv->iRegionGateID;


	
	if(tbus_forward(pstAppCtx->iBus, &iSrc, &iDst, stNet.pszBuff, stNet.iBuff, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"tbus_forward fail.");
		return -1;
	}
	return 0;
}


int z_ss_fill_id_card_req(ZONESVRENV* pstEnv,Player* pstPlayer,
                              CSAASIDINFO * pstInfo)
{
	SSPKG stPkg;
	TDRDATA stNet;
	int iSrc, iDst;
	SSFILLIDCARDREQ *pstFillIdCardReq = &stPkg.Body.FillIdCardReq;	

	if (pstPlayer->bAasFlag <= 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"acc=%s is not a aas account",pstPlayer->szAccount);
		return -1;
	}

	STRNCPY(pstFillIdCardReq->eMail,pstInfo->eMail,sizeof(pstFillIdCardReq->eMail));
	STRNCPY(pstFillIdCardReq->Name,pstInfo->Name,sizeof(pstFillIdCardReq->Name));
	STRNCPY(pstFillIdCardReq->IDCard,pstInfo->IDCard,sizeof(pstFillIdCardReq->IDCard));
	
	STRNCPY(pstFillIdCardReq->Pass9Account,pstPlayer->szAccount,
			sizeof(pstFillIdCardReq->Pass9Account));

	pstFillIdCardReq->Uin = pstPlayer->stRoleData.Uin;

	stPkg.Head.Cmd = SS_FILL_ID_CARD_REQ;
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

	iSrc = pstEnv->pstAppCtx->iId;
	iDst = pstEnv->iRegionGateID;


	
	if(tbus_forward(pstEnv->pstAppCtx->iBus, &iSrc, &iDst, stNet.pszBuff, stNet.iBuff, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"tbus_forward fail.");
		return -1;
	}
	return 0;
}


int z_ss_sms_req(TAPPCTX * pstAppCtx, ZONESVRENV* pstEnv,
                              Player* pstPlayer,int iSN)
{
	SSPKG stPkg;
	TDRDATA stNet;
	int iSrc, iDst;
	SSSMSREQ *pstSmsReq = &stPkg.Body.SmsReq;	

	//sms
	if (!pstPlayer->stOnline.szSmsMobile[0])
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"mobile err.");
		return -1;
	}
	
	STRNCPY(pstSmsReq->Mobile,pstPlayer->stOnline.szSmsMobile,sizeof(pstSmsReq->Mobile));
	STRNCPY(pstSmsReq->Pass9Account,pstPlayer->szAccount,sizeof(pstSmsReq->Pass9Account));	
	pstSmsReq->Uin = pstPlayer->stRoleData.Uin;
	snprintf(pstSmsReq->Text,sizeof(pstSmsReq->Text),"零纪元短信验证密码: %d 序号: %d",
				pstPlayer->stOnline.iSmsCode,iSN);

	stPkg.Head.Cmd = SS_SMS_REQ;
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
	iDst = pstEnv->iRegionGateID;


	
	if(tbus_forward(pstAppCtx->iBus, &iSrc, &iDst, stNet.pszBuff, stNet.iBuff, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"tbus_forward fail.");
		return -1;
	}
	return 0;
}

static int z_account_db_update_quick(ZONESVRENV* pstEnv, Player *pstPlayer,
									const INFORMERACCOUNT *pstIAccount,
									SelectInfo *pstExtra)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	ACCOUNTQUICKUPDATE stAccoutQuick;

	if (!(pstPlayer->UinFlag & UIN_FLAG_QUICK) || !(pstExtra->AuthFlag & AUTH_FLAG_DEQUICK))
	{
		return 0;
	}

	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ,
		             "AccountQuickUpdate", pstPlayer->stRoleData.Uin, NULL);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	stAccoutQuick.Uin = pstPlayer->stRoleData.Uin;

	memcpy(stAccoutQuick.PasswdHash, pstIAccount->PasswdHash,sizeof(stAccoutQuick.PasswdHash));
	memcpy(stAccoutQuick.Pass9Account, pstIAccount->Pass9Account,sizeof(stAccoutQuick.Pass9Account));
	memcpy(stAccoutQuick.BirthDay, pstIAccount->BirthDay,sizeof(stAccoutQuick.BirthDay));
	stAccoutQuick.Gender = pstIAccount->Gender;

	pstPlayer->UinFlag &= (~UIN_FLAG_QUICK);
	
	stAccoutQuick.QuickStatus = QUICK_STATUS_REG;
	stAccoutQuick.UinFlag = pstPlayer->UinFlag;
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stAccoutQuick;
	stMetaData.iBuff = sizeof(stAccoutQuick);
	
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		                      pstEnv->iAccountDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if(0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iAccountDBID))
	{
		return -1;
	}

	return 0;	
}

static int z_account_db_update_birthday(ZONESVRENV* pstEnv, Player *pstPlayer,
									const INFORMERACCOUNT *pstIAccount)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	ACCOUNTBIRTHDAYUPDATE stAccoutUpdate;

	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ,
		             "AccountBirthdayUpdate", pstPlayer->stRoleData.Uin, NULL);
	
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	stAccoutUpdate.Uin = pstPlayer->stRoleData.Uin;

	memcpy(stAccoutUpdate.BirthDay, pstIAccount->BirthDay,sizeof(stAccoutUpdate.BirthDay));
	stAccoutUpdate.Gender = pstIAccount->Gender;
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stAccoutUpdate;
	stMetaData.iBuff = sizeof(stAccoutUpdate);
	
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		                      pstEnv->iAccountDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if(0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iAccountDBID))
	{
		return -1;
	}

	return 0;	
}


// 从informer_svr获取的account信息
int z_ss_get_account_res(ZONESVRENV * pstEnv,
	                            const SSGETACCOUNTRES *pstGetAccountRes)
{
	int iRet = 0;
	const INFORMERACCOUNT *pstIAccount = &pstGetAccountRes->Account;
	SelectInfo *pstExtra = (SelectInfo *)(pstGetAccountRes->ExtraData);
	Player *pstPlayer = NULL;

	pstPlayer =  player_get_by_uin(pstEnv, pstExtra->Uin);

	if (!pstPlayer)
	{
		return -1;
	}

	// 从informer_svr获取的account信息如果存在，就将他插入数据，如果不存在，
	// 进发给客户端一个虚假的回应信息，防止客户端猜测密码。
	if( pstGetAccountRes->Found == 0 )
	{
		iRet = z_account_db_update_quick(pstEnv,pstPlayer, pstIAccount, 
			                               pstExtra);
		if (0 == iRet)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PLAYER150);
		}
	}
	else
	{		
	 	//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, "注册帐号失败");
		iRet = pstGetAccountRes->Found;
	}

	{
		
		CSPKG stPkg;
		CSACTIONRES *pstAction = &stPkg.Body.ActionRes; 	
		
		memset(pstAction,0,sizeof(*pstAction));
		
		pstAction->ID = pstPlayer->iMemID;
		pstAction->ActionID = ACTION_ACC_DE_QUICK;
		
		pstAction->Data.AccDeQuick.Result = iRet;
		
		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	
	return iRet;
}

static int z_db_req_select_account(ZONESVRENV *pstEnv, 
	                               Player* pstPlayer, 
	                               SelectInfo* pstSelectInfo)
{
	TORMPKG stPkg;
	TORMSELECTREQ *pstSelectReq = &stPkg.Body.SelectReq;
	DBAsyn stDBAsyn;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &stPkg;
	ACCOUNT stAccount;
		
	stDBAsyn.iCmd = DB_SELECT_ACCOUNT_RES;
	stDBAsyn.AsynCmd.stSelectInfo = *pstSelectInfo;
	memset(&stAccount,0,sizeof(stAccount));

	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &stPkg.Head, TORM_SELECT_REQ, 
			              "Account", pstPlayer->stRoleData.Uin, 
			              &stDBAsyn);
	
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	snprintf(pstSelectReq->OperInfo.WhereDef, 
		       sizeof(pstSelectReq->OperInfo.WhereDef),
		       "where Pass9Account=\'%s\'",
		       pstSelectInfo->stAuthInfo.AccountLoginName);

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stAccount;
	stMetaData.iBuff = sizeof(stAccount);

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		                      pstEnv->iAccountDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if(0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iAccountDBID))
	{
		return -1;
	}

	

	return 0;
}


int auth_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSAUTHCLT* pstAuthClt = &pstCsPkg->Body.AuthClt;
	SelectInfo stInfo;
	char szDec[RSA_ENC_LEN];
	TDRDATA stHost, stNet;
	RSA* pstRsa = NULL;
	LPTDRMETA pstAuthMeta = NULL;

	memset(&stInfo,0,sizeof(stInfo));

	pstRsa = rsa_from_private_key(IDRSAPrivateKey);

	pstAuthMeta = tdr_get_meta_by_name(pstEnv->pstNetHan->pstCSProtoLib, 
		                                         "AuthInfo");

	if (!pstRsa || !pstAuthMeta)
	{
		goto error;
	}

	stInfo.AuthFlag = pstAuthClt->AuthFlag;
	stInfo.ActiveFlag = 2;
	stInfo.Uin = pstPlayer->stRoleData.Uin;
	stInfo.CltType = pstAuthClt->CltType;

	if (0 > rsa_private_decrypt(pstRsa, sizeof(pstAuthClt->Enc), 
		                          pstAuthClt->Enc, szDec))
	{
		goto error;
	}

	stHost.iBuff = sizeof(stInfo.stAuthInfo);
	stHost.pszBuff = (char *)&stInfo.stAuthInfo;
	stNet.iBuff = sizeof(szDec);
	stNet.pszBuff = szDec;
	
	if (0 > tdr_ntoh(pstAuthMeta, &stHost, &stNet, pstPlayer->unCltProVer))
	{
		goto error;
	}
	
	strlwr(stInfo.stAuthInfo.AccountLoginName);

	z_db_req_select_account(pstEnv,pstPlayer,&stInfo);
	
	return 0;
	
error:
	return -1;
	UNUSED(pstFrameHead);	//add by paraunused.pl
	UNUSED(pstAppCtx);	//add by paraunused.pl
}


int z_ss_fill_id_card_res(ZONESVRENV * pstEnv,
	                            const SSFILLIDCARDRES *pstFillIdCardRes)
{
	int iRet = 0;
	const INFORMERACCOUNT *pstIAccount = &pstFillIdCardRes->Account;
	Player *pstPlayer = NULL;

	pstPlayer =  player_get_by_uin(pstEnv, pstFillIdCardRes->Uin);

	if (!pstPlayer)
	{
		return -1;
	}

	if (strcmp(pstPlayer->szAccount, pstIAccount->Pass9Account))
	{
		return -1;
	}

	if( pstFillIdCardRes->Result == 0 )
	{
		iRet = z_account_db_update_birthday(pstEnv,pstPlayer, pstIAccount);
		if (0 == iRet)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PLAYER151);
			player_add_id_card_succ(pstEnv,pstPlayer);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER152);
		}
	}
	else
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER152);
	}

	return iRet;
}


int z_ss_sms_res(ZONESVRENV * pstEnv,
	                            const SSSMSRES *pstSmsRes)
{
	tlog_info(pstEnv->pstLogCat, 0, 0,  "acc=%s uin=%d ret=%d",
		pstSmsRes->Pass9Account,pstSmsRes->Uin,pstSmsRes->Result);
	
	return 0;
}

int achievement_addattr(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	ACHIEVEMENTINFO* pstInfo = &pstPlayer->stRoleData.MiscInfo.AchievementInfo;
	int i = 0;
	ACHIEVEMENTDEF* pstDef = NULL;
	AchievementDefTail *pstTail = NULL;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	
	for(i = 0; i < MAX_ACHIEVEMENT; i++)
	{
		if (i >= pstInfo->Num)
		{
			break;
		}
		
		if (pstInfo->AchievementID[i] == 0)
		{
			break;
		}
		
		pstDef = z_find_achievement_def(pstEnv,pstInfo->AchievementID[i]);
		if (!pstDef)
		{
			continue;
		}
		pstTail = z_get_achievement_tail(pstDef);
		if (!pstTail || pstTail->iLevel > pstPlayer->stRoleData.Level ||
			!pstTail->iValid)
		{
			continue;
		}

		z_attr_result(pstEnv, pstDyn, NULL, &pstDef->Result);
	}
	
	return 0;
}

int z_player_achievement_done(ZONESVRENV* pstEnv,Player* pstPlayer,int iID)
{	
	int iIdx = 0;
	int iPos = 0;
	ACHIEVEMENTINFO* pstInfo = &pstPlayer->stRoleData.MiscInfo.AchievementInfo;

	if (iID >= MAX_ACHIEVEMENT)
	{
		return -1;
	}

	iIdx = iID / 8;
	iPos = iID % 8;
	
	if (pstPlayer->stOnline.szAchievementFlag[iIdx] & (1 << iPos))
	{
		return 0;
	}

	if (pstInfo->Num >= MAX_ACHIEVEMENT)
	{
		return 0;
	}

	pstInfo->AchievementID[pstInfo->Num] = iID;
	pstInfo->Num++;

	z_update_all_attr(pstEnv->pstAppCtx,pstEnv,pstPlayer);

	{
		CSPKG stPkg;
		CSACTIONRES *pstAction = &stPkg.Body.ActionRes;		
		
		memset(pstAction,0,sizeof(*pstAction));
		
		pstAction->ID = pstPlayer->iMemID;
		pstAction->ActionID = ACTION_ACHIEVEMENT_NEW;
		
		pstAction->Data.AchievementNew.AchievementID = iID;
		
		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}


	pstPlayer->stOnline.szAchievementFlag[iIdx] |= (1 << iPos);
	return 0;
}

int z_player_achievement_online(ZONESVRENV* pstEnv, Player* pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;	
	ACHIEVEMENTINFO* pstInfo = &pstPlayer->stRoleData.MiscInfo.AchievementInfo;
	int i = 0;
	ACHIEVEMENTDEF* pstDef = NULL;
	int iRemove = 0; //是否重复要删除
	
	memset(pstAction,0,sizeof(*pstAction));

	pstInfo->Num = min(pstInfo->Num,MAX_ACHIEVEMENT);

	memset(pstPlayer->stOnline.szAchievementFlag,0,sizeof(pstPlayer->stOnline.szAchievementFlag));

	for(i = 0; i < MAX_ACHIEVEMENT; i++)
	{
		if (i >= pstInfo->Num)
		{
			break;
		}
		
		if (pstInfo->AchievementID[i] == 0)
		{
			break;
		}
		
		pstDef = z_find_achievement_def(pstEnv,pstInfo->AchievementID[i]);
		if (pstDef && pstDef->AchievementID < MAX_ACHIEVEMENT)
		{
			int iIdx = pstDef->AchievementID  / 8;
			int iPos = pstDef->AchievementID  % 8;

			if (pstPlayer->stOnline.szAchievementFlag[iIdx] & (1 << iPos))
			{
				iRemove = 1;
			}
			else
			{			
				pstPlayer->stOnline.szAchievementFlag[iIdx] |= (1 << iPos);
			}
		}

		if (!pstDef || 1 == iRemove)
		{
			if (pstInfo->Num - 1 > i)
			{
				pstInfo->AchievementID[i] = pstInfo->AchievementID[pstInfo->Num - 1];
			}
			pstInfo->Num--;
			i--;

			iRemove = 0;
		}
	}

	z_update_all_attr(pstEnv->pstAppCtx,pstEnv,pstPlayer);
	
	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_ACHIEVEMENT_LIST;
	
	pstAction->Data.AchievementList.Num = pstInfo->Num;
	memcpy(pstAction->Data.AchievementList.AchievementID,pstInfo->AchievementID,sizeof(pstAction->Data.AchievementList.AchievementID));
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int z_player_achievement_cond(ZONESVRENV* pstEnv, Player* pstPlayer,ACHIEVEMENTCOND* pstCond)
{
	CONDIDMAP* pstIDMap = z_find_cond_id_map(pstEnv,pstCond->Type);
	ACHIEVEMENTDEF *pstDef = NULL;
	AchievementDefTail* pstTail = NULL;
	ACHIEVEMENTDEF *pstAchievementDef = NULL;
	
	int i = 0;
	int iIdx = 0;
	int iPos = 0;
	int iCmpType = 0;
	int iDone = 0;
	
	if (!pstIDMap)
	{
		return -1;
	}

	pstDef = z_find_achievement_def(pstEnv,pstIDMap->iAchievementID);

	if(!pstDef)
	{
		return -1;
	}
	pstTail = z_get_achievement_tail(pstDef);
	
	if (!pstTail)
	{
		return -1;
	}

	for(i = 0; i < MAX_ACHIEVEMENT_TAIL_ID; i++)
	{
		
		if (i >= pstTail->iNum )
		{
			break;
		}

		if (0 >= pstTail->aiID[i] || pstTail->aiID[i] >= MAX_ACHIEVEMENT)
		{
			continue;
		}

		iIdx = pstTail->aiID[i] / 8;
		iPos = pstTail->aiID[i] % 8;
		
		if (pstPlayer->stOnline.szAchievementFlag[iIdx] & (1 << iPos))
		{
			continue;
		}
		
		pstAchievementDef = z_find_achievement_def(pstEnv,pstTail->aiID[i]);
		if (!pstAchievementDef)
		{
			continue;
		}
		
		if (pstAchievementDef->Cond.Type != pstCond->Type)
		{
			continue;
		}

		switch (pstCond->Type)
		{
		
		case COND_TASK_ACCEPT:
		case COND_TASK_DONE:
		case COND_SKILL_USE:
		
			iCmpType = 0;
			break;
			
		case COND_FAIRY_LEVEL:
		case COND_FAIRY_STRENGTHEN:
		case COND_LIFESKILL_LEVEL:
			iCmpType = 2;
			break;

		case COND_RIDE_GET:
			iCmpType = 3;
			break;
		case COND_PET_GET:
			
			iCmpType = 4;
			break;
		default:
			iCmpType = 1;
			break;
		}
		
		iDone = 0;
		
		switch (iCmpType)
		{
		case 0:
			if (pstAchievementDef->Cond.Val1 == pstCond->Val1 &&
				pstAchievementDef->Cond.Val2 == pstCond->Val2 &&
				pstAchievementDef->Cond.Val3 == pstCond->Val3)
			{
				iDone = 1;
			}
			break;
		case 1:
			if (pstAchievementDef->Cond.Val1 <= pstCond->Val1 &&
				pstAchievementDef->Cond.Val2 <= pstCond->Val2 &&
				pstAchievementDef->Cond.Val3 <= pstCond->Val3)
			{
				iDone = 1;
			}
			break;
		case 2:
			if (pstAchievementDef->Cond.Val1 <= pstCond->Val1 &&
				pstAchievementDef->Cond.Val2 == pstCond->Val2 &&
				pstAchievementDef->Cond.Val3 == pstCond->Val3)
			{
				iDone = 1;
			}
			break;
		case 3:
			if (pstAchievementDef->Cond.Val1 <= pstCond->Val1 &&
				pstAchievementDef->Cond.Val2 <= pstCond->Val2 &&
				pstAchievementDef->Cond.Val3 == pstCond->Val3)
			{
				iDone = 1;
			}
			break;
		case 4:
			if (pstAchievementDef->Cond.Val1 == 0)
			{
				if (pstAchievementDef->Cond.Val1 <= pstCond->Val1 &&
					pstAchievementDef->Cond.Val2 == pstCond->Val2 &&
					pstAchievementDef->Cond.Val3 == pstCond->Val3)
				{
					iDone = 1;
				}
			}
			else
			{
				if (pstAchievementDef->Cond.Val1 == pstCond->Val1 &&
					pstAchievementDef->Cond.Val2 == pstCond->Val2 &&
					pstAchievementDef->Cond.Val3 == pstCond->Val3)
				{
					iDone = 1;
				}

			}
		default:
			break;
		}
		

		if (iDone)
		{
			z_player_achievement_done(pstEnv,pstPlayer,pstTail->aiID[i]);
		}
		
	}
		

	return 0;
}

int player_yesterday_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;		
	
	memset(pstAction,0,sizeof(*pstAction));

	if (!IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec,
				pstPlayer->stRoleData.MiscInfo.DailyInfo.ClanEveryDayPworldScore.Time))
	{
		int iHistory = pstPlayer->stRoleData.MiscInfo.OldDailyInfo.ClanEveryDayPworldScore.Value;

		if (pstPlayer->stRoleData.MiscInfo.DailyInfo.ClanEveryDayPworldScore.Value > 0)
		{
			iHistory++;
		}
		
		pstPlayer->stRoleData.MiscInfo.OldDailyInfo.ClanEveryDayPworldScore = pstPlayer->stRoleData.MiscInfo.DailyInfo.ClanEveryDayPworldScore;
		memset(&pstPlayer->stRoleData.MiscInfo.DailyInfo,0,
			sizeof(pstPlayer->stRoleData.MiscInfo.DailyInfo));

		pstPlayer->stRoleData.MiscInfo.OldDailyInfo.ClanEveryDayPworldScore.Value = iHistory;
	}
	
	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_YESTERDAY_INFO;
	
	pstAction->Data.YesterdayInfo.ClanEveryDayPworldScore = 0;

	//if (IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec - 86400,
				//pstPlayer->stRoleData.MiscInfo.OldDailyInfo.ClanEveryDayPworldScore.Time))
	{
		pstAction->Data.YesterdayInfo.ClanEveryDayPworldScore = 
			pstPlayer->stRoleData.MiscInfo.OldDailyInfo.ClanEveryDayPworldScore.Value;
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int all_player_yesterday_info(ZONESVRENV * pstEnv)
{
	int i = 0;
	for (i=0; i<MAX_PLAYER; i++)
	{
		TMEMBLOCK* pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		Player * pstPlayer = NULL;
		
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (Player *)pstMemBlock->szData;		
		
		if(PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus ) 
		{
			player_yesterday_info(pstEnv,pstPlayer);
		}
	}
	return 0;
}

int player_get_vig(ZONESVRENV * pstEnv, Monster *pstDieMon, Player*pstKiller)
{
	ZoneTeam *pstTeam;
	ZONEPWORLD *pstPworld;
	int i;
	TEAMMEMBER *pstCap;
	int iLevel = 0;
	MONSTERDEF *pstMonDef;
	Player *pstCapPlayer;
	Player *pstTmp;
	int iCapAdd = 0;
	int iVaildNum = 0;
	int iVig = 0;
	DYNATTRLIST *pstDyn = NULL;

	if (!pstDieMon || !pstKiller)
	{
		return 0;
	}
	
	pstPworld = z_find_pworld2(pstEnv,pstKiller->stRoleData.Map);
	if (pstPworld && !pstPworld->stPworldDef.IsGetVig)
	{
		return 0;
	}
	
	pstTeam = player_team_get(pstEnv, pstKiller);
	if (!pstTeam || pstTeam->stTeamInfo.MemberNum < 3)
	{
		return 0;
	}

	pstCap = player_team_get_captain(pstTeam);
	if (!pstCap)
	{
		return 0;
	}

	pstCapPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstCap->MemberName);
	if (!pstCapPlayer)
	{
		return 0;
	}

	pstMonDef = z_get_mon_def(pstEnv, pstDieMon);
	if (NULL == pstMonDef ||pstMonDef->Vig <= 0)
	{
		return 0;
	}

	for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		iLevel += pstTeam->stTeamInfo.TeamMember[i].Level;
	}

	iLevel = iLevel/pstTeam->stTeamInfo.MemberNum;

	if (abs(iLevel-pstDieMon->stMonDynAttr.Level) > 10)
	{
		return 0;
	}

	for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		
		pstTmp = z_name_player(pstEnv->pstAppCtx, pstEnv, 
								pstTeam->stTeamInfo.TeamMember[i].MemberName);
		if (!pstTmp)
		{
			continue;
		}

		//  不同地图不加
		if (pstTmp->stRoleData.Map != pstKiller->stRoleData.Map)
		{
			continue;
		}

		if (pstTmp->stOnline.State & CS_STAT_DEAD)
		{
			continue;
		}

		// 队长单独给活力值
		if (pstCapPlayer == pstTmp)
		{
			iCapAdd = 1;
		}
		else
		{
			iVig = pstMonDef->Vig;
			pstDyn =  &pstTmp->stOnline.stDyn;
			pstTmp->stOnline.iVigorEx = 0;
			if (pstDyn->iClanSkillVigorMul && iVig > 0)
			{
				int iRand = RAND1(100);
				if (iRand < pstDyn->iClanSkillVigorMul)
				{
					iVig += pstDyn->iClanSkillVigorVal;
					pstTmp->stOnline.iVigorEx = pstDyn->iClanSkillVigorVal;
				}
			}
			//队员获得活力值公式：基础活力
			player_add_vig(pstEnv, pstTmp,iVig, 0, 1);

			// 队员数量，除开队长
			iVaildNum ++;
		}
	}

	if (iCapAdd)
	{
		// 队长获得活力值公式：基础活力*（队员数量*0.25+1）
		iVig = pstMonDef->Vig *(1.0*iVaildNum*25/100+1);
		
		pstDyn =  &pstCapPlayer->stOnline.stDyn;
		if (pstDyn->iClanSkillVigorMul && iVig > 0)
		{
			int iRand = RAND1(100);
			if (iRand < pstDyn->iClanSkillVigorMul)
			{
				iVig += pstDyn->iClanSkillVigorVal;
				pstCapPlayer->stOnline.iVigorEx = pstDyn->iClanSkillVigorVal;
			}
		}
		
		player_add_vig(pstEnv, pstCapPlayer,iVig, 0, 1);
	}
	return 0;
}

int player_full_vig(ZONESVRENV * pstEnv,  Player*pstPlayer)
{
	int iVig = pstPlayer->stOnline.stDyn.stMidAttrList.uiVigorMax*0.5;

	if (iVig > 0)
	{
		player_add_vig(pstEnv, pstPlayer,iVig, 0, 1);
	}
	return 0;
}

int player_login_up_vig(ZONESVRENV * pstEnv,  Player*pstPlayer)
{
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	

	// 不是同一天加满vig
	if (IsSameDay(pstPlayer->stRoleData.LastLogout, iCurr) == 0)
	{
		player_full_vig(pstEnv,  pstPlayer);	
	}
	return 0;
}

int player_action_count_down(ZONESVRENV * pstEnv,  Player*pstPlayer,int iSec,char* pszTitle)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;		
	
	memset(pstAction,0,sizeof(*pstAction));
	
	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_COUNT_DOWN;
	
	pstAction->Data.CountDown.Sec = iSec;
	STRNCPY(pstAction->Data.CountDown.Title,pszTitle,sizeof(pstAction->Data.CountDown.Title));
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}


// 是否矿场地图
int is_mine_map(ZONESVRENV* pstEnv, int iMapID)
{
	MapIndex *pstMapIdx; 

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, iMapID);
	if (pstMapIdx && pstMapIdx->stMapDef.EnterTime > 0)
	{
		return 1;
	}
	
	return 0;
}


//坐牢,海滩,彩虹岛,勇士岛,中控中心,幻境不让用
int player_use_trans_item_map_check(ZONESVRENV * pstEnv,  Player*pstPlayer,int iTip)
{
	if (player_is_zuolao(pstPlayer) ||
		BEACH_MAP_ID == pstPlayer->stRoleData.Map ||
		RAINBOW_MAP_ID == pstPlayer->stRoleData.Map ||
		WARRIOR_MAP_ID == pstPlayer->stRoleData.Map ||
		WORLD_CITY_MAPID == pstPlayer->stRoleData.Map ||
		is_unreal_boss_map(pstEnv,pstPlayer->stRoleData.Map) ||
		is_mine_map(pstEnv, pstPlayer->stRoleData.Map))
	{
		if (iTip)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER203);
		}
		return -1;
	}
	return 0;
}

int player_action_view_flag_chg(ZONESVRENV * pstEnv,  Player*pstPlayer)
{
	CSPKG stPkg;
	ZoneTeam *pstTeam = NULL;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;		
	
	memset(pstAction,0,sizeof(*pstAction));
	
	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_VIEW_FLAG_CHG;
	
	pstAction->Data.ViewFlagChg.ViewFlag = 0;

	
	// 队伍信息
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if (pstTeam)
	{		
		TEAMMEMBER * pstTeamMember = player_team_get_captain(pstTeam);
		if (pstTeamMember && 0 == strcmp(pstTeamMember->MemberName, pstPlayer->stRoleData.RoleName))
		{
			pstAction->Data.ViewFlagChg.ViewFlag |= VIEW_FLAG_CAPTAIN;
		}
	}

	// 安全区
	if (pstPlayer->stOnline.cSecenOverLay > 0)
	{
		pstAction->Data.ViewFlagChg.ViewFlag |= VIEW_FLAG_SAFE;
	}

	//刺探活动
	if (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_SPY)
	{
		ZONEPWORLDINST *pstZonePworldInstSelf = NULL;
			
		pstZonePworldInstSelf = z_find_player_pworld_inst(pstEnv,pstPlayer);	
		
		if (pstZonePworldInstSelf &&
			pstZonePworldInstSelf->iType == PWORLD_TYPE_CLAN &&
			pstZonePworldInstSelf->stPworldTypeData.stClan.iID ==
				pstPlayer->stRoleData.MiscInfo.DailyInfo.SpyInfo.TargetClanID)
		{
			pstAction->Data.ViewFlagChg.ViewFlag |= VIEW_FLAG_SPY;
		}
	}

	if (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_ANTI_SPY)
	{
		ZONEPWORLDINST *pstZonePworldInstSelf = NULL;
			
		pstZonePworldInstSelf = z_find_player_pworld_inst(pstEnv,pstPlayer);	
		
		if (pstZonePworldInstSelf &&
			pstZonePworldInstSelf->iType == PWORLD_TYPE_CLAN &&
			pstZonePworldInstSelf->stPworldTypeData.stClan.iID ==
				(int)pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id)
		{
			pstAction->Data.ViewFlagChg.ViewFlag |= VIEW_FLAG_ANTI_SPY;
		}
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	return 0;
}


int player_is_dartcar_limit(Player*pstPlayer)
{
	if (	pstPlayer->stOnline.State & 
		(CS_STAT_STALL|CS_STAT_RECIPE_PREPARE |
		CS_STAT_GEM_FENJIE_PRE|CS_STAT_ARM_FENJIE_PRE|
		CS_STAT_RIDE_PREPARE| CS_STAT_FAIRY_CAMPAIGN_PRE |
		CS_STAT_PREPARE | CS_STAT_AUTOFLY ))
	{
		return -1;
	}

	if (pstPlayer->stOnline.stExchgOnline.bExchgStat  > 0)
	{
		return -1;
	}

	if (pstPlayer->stOnline.stQiecuoInfo.iMemID > 0)
	{
		return -1;
	}
	return 0;
}


int player_action_clan_machine_pworld(ZONESVRENV * pstEnv,  Player*pstPlayer,ZONEPWORLDINST *pstPworldInst)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;	
	int iNow = pstEnv->pstAppCtx->stCurr.tv_sec;
	PWORLDROUNDMON *pstRound = NULL;
	PWORLDROUNDMONINFO *pstMonInfo = NULL;
	ZONEPWORLD *pstZonePworld = NULL;
	
	pstRound = &pstPworldInst->stWaveRound;
	if (pstRound->iCurrRound >= 0 && pstRound->iCurrRound < pstRound->iTotalRound)
	{
		pstMonInfo = &pstRound->aRoundMonInfo[pstRound->iCurrRound];
	}
	
	if (pstRound->iCurrRound == 0 &&  pstMonInfo && pstMonInfo->iRefreshTime == 0)
	{
		return 0;
	}

	//lzk
	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if(NULL == pstZonePworld)
	{
		return 0;
	}
	
	memset(pstAction,0,sizeof(*pstAction));
	
	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_CLAN_MACHINE_PWORLD;
	
	pstAction->Data.ClanMachinePworld.WaveNum = pstRound->iCurrRound + 1;
	if (pstMonInfo)
	{
		pstAction->Data.ClanMachinePworld.WaveTimer = pstMonInfo->iRefreshTime - iNow;
	}
	
	if (pstAction->Data.ClanMachinePworld.WaveTimer < 0)
	{
		pstAction->Data.ClanMachinePworld.WaveTimer = 0;
	}

	//副本id传递   lzk
	pstAction->Data.ClanMachinePworld.PworldDefId = pstZonePworld->stPworldDef.PworldID;
	
	//设置副本结束标识
	if(pstRound->iTotalRound == 0)
	{
		pstAction->Data.ClanMachinePworld.Flag = 1;
	}
	
	pstRound = &pstPworldInst->stTimerRound;
	if (pstRound->iCurrRound >= 0 && pstRound->iCurrRound < pstRound->iTotalRound)
	{
		pstMonInfo = &pstRound->aRoundMonInfo[pstRound->iCurrRound];
	}
	
	if (pstMonInfo)
	{
		pstAction->Data.ClanMachinePworld.RaiderTimer = pstMonInfo->iRefreshTime - iNow;
		if (pstAction->Data.ClanMachinePworld.RaiderTimer < 0)
		{
			pstAction->Data.ClanMachinePworld.RaiderTimer = 0;
		}
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_action_cow_final_range(ZONESVRENV * pstEnv,  Player*pstPlayer,int iOpType)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;	
	COWFINALRANGE *pstRange = NULL;

	
	memset(pstAction,0,sizeof(*pstAction));
	
	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_COW_FINAL_RANGE;

	pstRange = &pstEnv->pstGlobalObj->stGlobal.Misc.CowFinalRange;
	
	pstAction->Data.CowFinalRange.OpType = iOpType;
	memcpy(&pstAction->Data.CowFinalRange.CowFinalRange,pstRange,sizeof(pstAction->Data.CowFinalRange.CowFinalRange));
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


//lzk  打开航海大亨奖励面板iOpType:0表示打开 1表示更新
int player_action_hang_hai_range(ZONESVRENV * pstEnv,  Player*pstPlayer,int iOpType)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;	
	HANGHAIRANGE *pstRange = NULL;

	
	memset(pstAction,0,sizeof(*pstAction));
	
	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_HANG_HAI_RANGE;

	pstRange = &pstEnv->pstGlobalObj->stGlobal.Misc.HangHaiRange;
	
	pstAction->Data.HangHaiRange.OpType = iOpType;
	memcpy(&pstAction->Data.HangHaiRange.HangHaiRange,pstRange,sizeof(pstAction->Data.HangHaiRange.HangHaiRange));
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int player_contribute_dec(ZONESVRENV* pstEnv, Player *pstPlayer,
                      unsigned int nValue,int iTest)
{
	CLANMEM *pstMem = NULL;
	ZoneClan* pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
	{
		return -1;
	}

	pstMem = clan_id_mem(pstClan,pstPlayer->stRoleData.RoleID, NULL);
	if (!pstMem)
	{
		return -1;
	}

	if (pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute < nValue)
	{
		return -1;
	}
	
	if (iTest)
	{
		return 0;
	}
	
	pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute -= 	nValue;
	pstMem->Contribute = pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute;
	
	player_clan_mem_chg_broadcast(pstEnv, pstClan, pstMem,CLAN_MEMBER_CHG_MOD);
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER204,nValue);
		
	return 0;
}


int player_action_clan_donate_log(ZONESVRENV * pstEnv,  Player*pstPlayer,CLANDONATELOGENTRY *pstLog)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;		
	
	memset(pstAction,0,sizeof(*pstAction));
	
	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_CLAN_DONATE_LOG;
	
	pstAction->Data.ClanDonateLog.ClanDonateLogEntry = *pstLog;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_action_clan_skill_info(ZONESVRENV * pstEnv,  Player* pstPlayer,CLANSKILLINFO* pstInfo)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;		
	
	memset(pstAction,0,sizeof(*pstAction));
	
	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_CLAN_SKILL_INFO;
	
	pstAction->Data.ClanSkillInfo.ClanSkillInfo = *pstInfo;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_action_player_clan_skill_info(ZONESVRENV * pstEnv,  Player*pstPlayer,PLAYERCLANSKILLINFO* pstInfo)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;		
	
	memset(pstAction,0,sizeof(*pstAction));
	
	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_PLAYER_CLAN_SKILL_INFO;
	
	pstAction->Data.PlayerClanSkillInfo.ClanSkillInfo = *pstInfo;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_uin_flag_chg(ZONESVRENV* pstEnv, Player *pstPlayer,int iUinFlag,int iType)
{
	if (iType == ACC_SET_UIN_FLAG_TYPE_NOTICE)
	{
		pstPlayer->UinFlag |= iUinFlag;
	}
	
	if (pstPlayer->UinFlag & UIN_FLAG_SAFE_EMAIL)
	{
		task_player_trace(pstEnv,pstPlayer,TASK_OBJ_BIND_SAFE_EMAIL);
	}
	
	return 0;
}

int player_beach_info_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_BEACH_INFO;

	pstActionRes->Data.BeachInfo.Sec = pstPlayer->stRoleData.MiscInfo.BeachInfo.Sec;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int player_beach_info_update(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	BEACHINFO *pstBeachInfo = &pstPlayer->stRoleData.MiscInfo.BeachInfo;
	
	if (!IsSameDay(pstBeachInfo->EnterTime,	pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstBeachInfo->EnterTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstBeachInfo->Sec = 0;
		pstPlayer->stOnline.iBeachMapSec = 0;

		player_beach_info_action(pstEnv, pstPlayer);
	}

	return 0;
}

int player_beach_buff_action(ZONESVRENV* pstEnv, Player *pstPlayer, int fIn)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_BEACH_BUFF;
	
	player_beach_info_update(pstEnv, pstPlayer);
	pstActionRes->Data.BeachBuff.In = fIn;
	pstActionRes->Data.BeachBuff.BuffSec = pstPlayer->stRoleData.MiscInfo.BeachInfo.Sec;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

//获得当前可达到的最高等级
int get_curr_max_level(ZONESVRENV* pstEnv)
{
	GLOBAL *pstGlobal = &pstEnv->pstGlobalObj->stGlobal;

	if(is_fini_global(pstEnv) && pstGlobal->LevFeng.LevelFeng > 0)
	{
		return pstGlobal->LevFeng.LevelFeng;
	}
	else if(pstEnv->pstConf->MaxLevel > 0)
	{
		return pstEnv->pstConf->MaxLevel;
	}
	else
	{
		return MAX_LEVEL;
	}
	
	return 0;
}


int player_daoba_chg(ZONESVRENV* pstEnv, Player *pstPlayer, int iSelf)
{
	CSPKG stPkg;
	DAOBAINFO *pstDaoba = &pstPlayer->stRoleData.MiscInfo.DaoBaInfo;

	stPkg.Body.ActionRes.ID = pstPlayer->iMemID;
	stPkg.Body.ActionRes.ActionID = ACTION_DAOBA_INFO;
	stPkg.Body.ActionRes.Data.DaoBa.ClanWid = pstDaoba->ClanWID;
	stPkg.Body.ActionRes.Data.DaoBa.End = pstDaoba->End;
	stPkg.Body.ActionRes.Data.DaoBa.Name[0] = 0;

	if (pstDaoba->Name[0] != 0)
	{
		strncpy(stPkg.Body.ActionRes.Data.DaoBa.Name, pstDaoba->Name, 
			sizeof(stPkg.Body.ActionRes.Data.DaoBa.Name));
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	if (iSelf)
	{
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		return 0;
	}
	return z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg,0);
}

int player_die_up_daoba(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstKiller)
{
	ZoneClan *pstClan;
	ZoneClan *pstkillClan;
	DAOBAINFO *pstDaoba = &pstPlayer->stRoleData.MiscInfo.DaoBaInfo;
	DAOBAINFO *pstDaobaKill;
//	int iMapID = GET_MAPID(pstPlayer->stRoleData.Map);
	
	if (!pstKiller)
		return 0;

	/*if (BASE_CITY_MAPID == iMapID)
	{
		return 0;
	}*/

	// 切磋不用管
	if (pstPlayer->stOnline.stQiecuoInfo.iMemID == pstKiller->iMemID)
	{
		return 0;
	}

 	// 副本不用管
	if (	pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE || 
		GET_MAPID(pstPlayer->stRoleData.Map) == WORLD_CITY_MAPID) 
	{
		return 0;
	}

	pstDaobaKill = &pstKiller->stRoleData.MiscInfo.DaoBaInfo;
	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
		return 0;

	// 清理攻击者的
	if (	pstDaobaKill->ClanID > 0 &&
		pstDaobaKill->End > (unsigned int)pstEnv->pstAppCtx->stCurr.tv_sec &&
		pstDaobaKill->ClanID == pstClan->stClan.ClanProf.Id)
	{
		memset(pstDaobaKill, 0, sizeof(*pstDaobaKill));
		player_daoba_chg(pstEnv,pstKiller, 0);
	}

	pstkillClan = player_get_clan(pstEnv, pstKiller);
	if (!pstkillClan)
		return 0;

	if (pstClan == pstkillClan)
		return 0;

	if (!strong_get_by_player(pstEnv,pstPlayer))
	{
		return 0;
	}

	if (!strong_get_by_player(pstEnv,pstKiller))
	{
		return 0;
	}

	pstDaoba->ClanID = pstkillClan->stClan.ClanProf.Id;
	pstDaoba->ClanWID = pstkillClan->stClan.ClanProf.GID;
	pstDaoba->End = pstEnv->pstAppCtx->stCurr.tv_sec + 7*24*60*60;
	strncpy(pstDaoba->Name,pstKiller->stRoleData.RoleName, sizeof(pstDaoba->Name));
	player_daoba_chg( pstEnv,pstPlayer, 0);
	
	return 1;
}

int player_make_daoba_view(ZONESVRENV* pstEnv ,CSPLAYER *pstCSPlayer , Player *pstVPlayer)
{
	DAOBAINFO *pstDaoba = &pstVPlayer->stRoleData.MiscInfo.DaoBaInfo;

	if (pstDaoba->End <= (unsigned int)pstEnv->pstAppCtx->stCurr.tv_sec)
		return 0;
	
	pstCSPlayer->DaoBa.End = pstDaoba->End;
	pstCSPlayer->DaoBa.ClanWid = pstDaoba->ClanWID;
	pstCSPlayer->DaoBa.Name[0] = 0;

	if (pstDaoba->Name[0] != 0)
		strncpy(pstCSPlayer->DaoBa.Name, pstDaoba->Name, sizeof(pstCSPlayer->DaoBa.Name));
	return 0;
}

int player_daoba_sec(ZONESVRENV* pstEnv ,Player *pstPlayer)
{
	DAOBAINFO *pstDaoba = &pstPlayer->stRoleData.MiscInfo.DaoBaInfo;

	if (pstDaoba->End <= 0)
		return 0;

	if (pstDaoba->End <= (unsigned int)pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		memset(pstDaoba, 0, sizeof(*pstDaoba));	
		player_daoba_chg(pstEnv, pstPlayer, 0);
		return 0;
	}

	// 没工会了
	if(pstEnv->iInstMode == INST_MODE_ZONE_SVR)
	{
		if (!z_id_clan( pstEnv, pstDaoba->ClanID))
		{
			memset(pstDaoba, 0, sizeof(*pstDaoba));	
			player_daoba_chg(pstEnv, pstPlayer, 0);
			return 0;	
		}
	}
	return 0;
}


int player_die_tip_by_saprte_range(ZONESVRENV* pstEnv ,Player *pstPlayer, Player *pstKiller)
{
	if (pstEnv->iInstMode != INST_MODE_ZONE_SVR)
	{
		return 0;
	}
	
	if (!pstKiller)
	{
		return 0;
	}

	if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE)
	{	
		return 0;
	}	

	if (! player_sparta_in_top_ten(pstEnv,pstPlayer))
	{
		return 0;
	}

	if (pstEnv->pstConf->CountryVer != COUNTRY_VER_KOREA)
	{
		z_sys_strf_broadcast(pstEnv, SYS_SHOW, 
				ZONEERR_PLAYER803, pstPlayer->stRoleData.RoleName, pstKiller->stRoleData.RoleName);
	}
	
	// 临时性的
	/*CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;

	pstSysMsg->Type = SYS_SHOW;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;

	sprintf(pstSysMsg->Msg,
		"荣耀殿堂十大战神之%s被英勇的%s击败!",
		pstPlayer->stRoleData.RoleName, pstKiller->stRoleData.RoleName);
	pstSysMsg->Msg[sizeof(pstSysMsg->Msg) - 1] = 0;

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
	*/
	return 0;
}

// 
int player_is_huiliu(Player *pstPlayer)
{
	if (	pstPlayer->stRoleData.MiscInfo.LevelBuffExpInfo.BuchangInfo.Day >= HUILIU_DAY && 
		pstPlayer->stRoleData.MiscInfo.LevelBuffExpInfo.BuchangInfo.Flag == 0)
	{
		return 1;
	}
	
	return 0;
}


//登录 判定是不是老玩家回流
int player_login_verdict_huiliu(ZONESVRENV* pstEnv ,Player *pstPlayer)
{
	int iCurr =  pstEnv->pstAppCtx->stCurr.tv_sec;
	int iDay;

	if (pstPlayer->stRoleData.LastLogout <= 0)
	{
		return 0;
	}

	iDay = ( iCurr - (int)pstPlayer->stRoleData.LastLogout)/(3600*24); 	// 天

	if (iDay >= HUILIU_DAY )
	{
		pstPlayer->stRoleData.MiscInfo.LevelBuffExpInfo.BuchangInfo.Day = iDay;
		pstPlayer->stRoleData.MiscInfo.LevelBuffExpInfo.BuchangInfo.Flag = 0;		// 没有领取
	}
	return 0;
}



RONGYANJUSHOUDEF *z_find_rongyanjushou_def(ZONESVRENV* pstEnv, int iLevel)
{
	RONGYANJUSHOUDEF  stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Level = iLevel;
	iIndex = bsearch_int(&stDef, pstObj->sRongYanJuShouDef, pstObj->iRongYanJuShouDef, RONGYANJUSHOU_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (RONGYANJUSHOUDEF *)&pstObj->sRongYanJuShouDef[iIndex];
	}
	
	return NULL;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(iLevel);	//add by paraunused.pl
}

RONGYANMONDYNATTRDEF *z_find_rongyanmondynattr_def(ZONESVRENV* pstEnv, int iLevel)
{	
	RONGYANMONDYNATTRDEF  stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Level = iLevel;
	iIndex = bsearch_int(&stDef, pstObj->sRongYanMonDynAttrDef, pstObj->iRongYanMonDynAttrDef, RONGYANMONDYNATTR_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (RONGYANMONDYNATTRDEF *)&pstObj->sRongYanMonDynAttrDef[iIndex];
	}  
	return NULL;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(iLevel);	//add by paraunused.pl
}

//lzk 客户端开始选择的倍数索引记录到team中,并刷怪
int player_rongyan_begin(ZONESVRENV* pstEnv,Player* pstPlayer,CSRONGYANBEGINSELECT *pReq)
{
	RONGYANJUSHOUDEF *pstRongYanJuShouDef; 
	CSRONGYANUIINFO stRongYanUiInfo;
	int iMonID;
	RESPOS stPos;
	int Ret = 0 ;
	int iRongYanTeamCapId;
	int iMonMemId = 0;

	ZoneTeam *pstTeam;
	TEAMMEMBER *pstCaptain;
	Player *pstCaptainPlayer = NULL;
	int iPinJunLevel = 0;
	//int i;

	int iNum;
	int iItemID;
	int iRet;
	ITEMDEC stDec;
	ITEMDEF *pstItemDef;

	int k;
	int TeamMemberNum;
	Player *pstMemberPlayer = NULL;
	
	//只能在各野外出安全区意外的中立地图使用
	if(1 != is_player_zhongli_area(pstEnv,pstPlayer))
	{
		return -1;
	}
	
	//--------------只有队长能操作判断begin------------
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(pstTeam == NULL)
	{
		//没有队伍
		return -1;
	}

	pstCaptain = player_team_get_captain(pstTeam);
	if(pstCaptain == NULL)
	{
		//没有队长
		return -1;
	}
	
	pstCaptainPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,pstCaptain->MemberName);
	if(pstCaptainPlayer == NULL)
	{
		//队长不在线
		return -1;
	}
	
	//判断队长是不是角色本人
	if(strcmp(pstCaptain->MemberName, pstPlayer->stRoleData.RoleName) != 0)
	{
		return -1;
	}	
	//-----------------------end--------------------------------

	//假设正在打怪了，使用物品，不可操作
	if(1 ==  pstTeam->iRongYanBegin)
	{
        return -1;
	}
	
	pstRongYanJuShouDef = z_find_rongyanjushou_def(pstEnv,1);
	if(!pstRongYanJuShouDef)
	{
		return -1;
	}
	
	//以下是扣除道具------------begin----------------
	//iBeginSelectMul   只能取1,2,3,4
	iNum = pstRongYanJuShouDef->RongYan[pReq->iBeginSelectMul-1].ItemNum;
	
	if(iNum < 0)
	{
		return -1;
	}
	
	iItemID = RONGYAN_HAIXIN_ITEM_ID;
	if (iNum > z_get_pak_item_num(pstEnv, &pstPlayer->stRoleData.Package, iItemID, -1,0,PACKAGE_DEC_NONE))
	{
		pstItemDef = z_find_itemdef(pstEnv, iItemID);
		if (NULL == pstItemDef)
		{
			return -1;
		}

		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_MISC_306,pstItemDef->Name);
		return -1;
	}

	stDec.ID = iItemID;
	stDec.Num = iNum;
	stDec.BindNum = 0;
	
	iRet = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, iItemID, iNum, -1,
			NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_USE);
	if (iRet > 0)
		stDec.BindNum = iRet;
	
	if (iRet >= 0)
	{
		z_role_item_dec_oplog(pstEnv,pstPlayer, &stDec,OP_ITEM_DEC_USE);
	}
	//--------------------------扣除物品end  -----------------

	
	//算队伍中的平均级别--------begin-----------------
	TeamMemberNum = pstTeam->stTeamInfo.MemberNum;
	if( TeamMemberNum <= 0)
	{
			return -1;
	}
	
	iPinJunLevel = player_rongyan_level(pstEnv,pstPlayer,pstTeam);
	if(-1 == iPinJunLevel)
	{
		return -1;
	}
	//-----------------------end--------------------------------

	
	iMonID = pstRongYanJuShouDef->MonID;
	//队长id
	iRongYanTeamCapId = pstPlayer->iMemID;

	//刷怪坐标
	stPos.X = pstCaptain->Pos.X;
	stPos.Y = pstCaptain->Pos.Y;
	pstTeam->stRongYanPos.X = pstCaptain->Pos.X;
	pstTeam->stRongYanPos.Y = pstCaptain->Pos.Y;
	//给后面刷假boss 世界播报 取地图名用
	pstTeam->stRongYanZoneIdx.iID = pstCaptainPlayer->stRoleData.Map;
	pstTeam->stRongYanZoneIdx.iPos = pstCaptainPlayer->stOnline.iMapPos;
	Ret = player_rongyan_new_mon(pstEnv, pstPlayer,pstRongYanJuShouDef->MonLifeTime,iMonID,&stPos,iRongYanTeamCapId,&iMonMemId,iPinJunLevel,&pstTeam->stRongYanZoneIdx);
	if(Ret < 0)
	{
		rongyan_all_status_clean(pstEnv,pstTeam,TeamMemberNum);
		return -1;
	}

	//在队伍中记录相关
	pstTeam->iRongYanMonId = iMonMemId;
	pstTeam->iRongYanCurLevel = pstRongYanJuShouDef->Level;
	pstTeam->iRongYanStatus = 1;
	//1,2,3,4   iBeginSelectMul   取值情况
	pstTeam->iRongYanSelectMul = pReq->iBeginSelectMul;
	pstTeam->iRongYanBegin = 1;//活动开始
	
	//通知客户端弹出融焰移动信息框
	stRongYanUiInfo.BeginSelectMul = pReq->iBeginSelectMul;
	stRongYanUiInfo.CurLayer = pstRongYanJuShouDef->Level;

	//每个队员都发move 框
	for(k = 0;k<TeamMemberNum;k++)
	{
		pstMemberPlayer =  z_name_player(pstEnv->pstAppCtx, pstEnv,pstTeam->stTeamInfo.TeamMember[k].MemberName);
		if(NULL == pstMemberPlayer)
		{
			continue;
		}

		player_rongyan_move_ui(pstEnv,pstMemberPlayer,&stRongYanUiInfo);
	}

	return 0;
}


//lzk  客户端进行boss选择，CSRongYanBossSelect.iSelect 0代表没选，1代表选择 
int player_rongyan_boss(ZONESVRENV* pstEnv,Player* pstPlayer,CSRONGYANBOSSSELECT *pReq)
{
	RONGYANJUSHOUDEF *pstRongYanJuShouDef; 
	RONGYANJUSHOUDEF *pstTempRongYanJuShouDef;
	int iMonID,iFalseMonID;
	int iLevel;
	ZoneTeam *pstTeam;
	TEAMMEMBER *pstCaptain;
	int RandOdds;
	RESPOS stPos;
	int iRongYanTeamCapId;
	CSRONGYANUIINFO stRongYanUiInfo;
	int iMonMemId = 0;
	int iPinJunLevel = 0;
	//int i;
	Player *pstCaptainPlayer = NULL;
	int iBossSelect = pReq->iSelect;
	int k;
	int TeamMemberNum;
	Player *pstMemberPlayer = NULL;
	CSRONGYANBOSSINFO stRongYanBossInfo;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	
	int Ret = 0 ;

	//跨服状态无法进行
	if(pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
	   z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
	   return -1;
	}
	
	//--------------只有队长能操作判断begin------------
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(pstTeam == NULL)
	{
		//没有队伍
		return -1;
	}

	pstCaptain = player_team_get_captain(pstTeam);
	if(pstCaptain == NULL)
	{
		//没有队长
		return -1;
	}
	
	pstCaptainPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,pstCaptain->MemberName);
	if(pstCaptainPlayer == NULL)
	{
		//队长不在线
		return -1;
	}
	
	//判断队长是不是角色本人
	if(strcmp(pstCaptain->MemberName, pstPlayer->stRoleData.RoleName) != 0)
	{
		return -1;
	}
	
	//算队伍中的平均级别
	TeamMemberNum = pstTeam->stTeamInfo.MemberNum;
	if( TeamMemberNum <= 0)
	{
			return -1;
	}
	
	iPinJunLevel = player_rongyan_level(pstEnv,pstPlayer,pstTeam);
	if(-1 == iPinJunLevel)
	{
		return -1;
	}
	
	//-----------------------end--------------------------------

	iLevel = pstTeam->iRongYanCurLevel +1;
	if(iLevel > 10 || iLevel < 0)
	{
		return -1;
	}

	if (pstTeam->iRongYanStatus)
	{
		return -1;
	}

	iRongYanTeamCapId = pstCaptain->MemID;
	stPos.X = pstTeam->stRongYanPos.X;
	stPos.Y = pstTeam->stRongYanPos.Y;
	
	pstRongYanJuShouDef = z_find_rongyanjushou_def(pstEnv,iLevel);
	if(!pstRongYanJuShouDef)
	{
		return -1;
	}
		
	RandOdds = RAND1(10000);
	//刷假boss
	if(RandOdds >= pstRongYanJuShouDef->Odds)
	{
		pstTempRongYanJuShouDef = z_find_rongyanjushou_def(pstEnv,0);
		iFalseMonID = pstTempRongYanJuShouDef->MonID;
		Ret = player_rongyan_new_mon(pstEnv, pstPlayer,pstTempRongYanJuShouDef->MonLifeTime,iFalseMonID,&stPos,iRongYanTeamCapId,&iMonMemId,iPinJunLevel,&pstTeam->stRongYanZoneIdx);
		if(Ret < 0)
		{
			rongyan_all_status_clean(pstEnv,pstTeam,TeamMemberNum);
			return -1;
		}
		
		z_team_sys_strf(pstEnv, pstTeam, SYS_TEAM, ZONEERR_RONGYAN1);

		//先记录打真怪的最后一层给打完假怪后世界播报用
		pstTeam->iRongYanOldLevel = pstTeam->iRongYanCurLevel;
		
		pstTeam->iRongYanCurLevel = pstTempRongYanJuShouDef->Level;
		pstTeam->iRongYanMonId = iMonMemId;
		pstTeam->iRongYanStatus = 1;

		//每个队员都关闭boss 框
		stRongYanBossInfo.BeginSelectMul = 0;
		stRongYanBossInfo.CurLayer = 0;

		for(k = 0;k<TeamMemberNum;k++)
		{
			pstMemberPlayer =  z_name_player(pstEnv->pstAppCtx, pstEnv,pstTeam->stTeamInfo.TeamMember[k].MemberName);
			if(NULL == pstMemberPlayer)
			{
				continue;
			}

			player_rongyan_boss_ui(pstEnv, pstMemberPlayer,&stRongYanBossInfo);
		}

		//以下是为了得到地图名并世界播报----------begin--------------
		stIdx.iID = pstTeam->stRongYanZoneIdx.iID;
		stIdx.iPos = pstTeam->stRongYanZoneIdx.iPos;
		
		pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
		if (NULL == pstMapInst)
		{
			return -1;
		}

		pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
		if (NULL == pstMapIdx)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,  "pstMapIdx NULL");
			return -1;
		}

		if (pstEnv->pstConf->CountryVer != COUNTRY_VER_KOREA)
		{
			z_sys_strf_broadcast(pstEnv, SYS_SHOW,ZONEERR_TEAM317,pstCaptain->MemberName,pstMapIdx->stMapDef.Name,pstTeam->iRongYanOldLevel);
		}
		//-----------------------end------------------------------
	}
	else
	{
		//刷真boss
		iMonID = pstRongYanJuShouDef->MonID;
		Ret = player_rongyan_new_mon(pstEnv, pstPlayer,pstRongYanJuShouDef->MonLifeTime,iMonID,&stPos,iRongYanTeamCapId,&iMonMemId,iPinJunLevel,&pstTeam->stRongYanZoneIdx);
		if(Ret < 0)
		{
			rongyan_all_status_clean(pstEnv,pstTeam,TeamMemberNum);
			return -1;
		}
		
		pstTeam->iRongYanCurLevel = iLevel;
		pstTeam->iRongYanMonId = iMonMemId;
		pstTeam->iRongYanStatus = 1;

    	//通知客户端弹出融焰移动信息框
		stRongYanUiInfo.BeginSelectMul = pstTeam->iRongYanSelectMul;
		stRongYanUiInfo.CurLayer = iLevel;

		//每个队员都发move 框
		for(k = 0;k<TeamMemberNum;k++)
		{
			pstMemberPlayer =  z_name_player(pstEnv->pstAppCtx, pstEnv,pstTeam->stTeamInfo.TeamMember[k].MemberName);
			if(NULL == pstMemberPlayer)
			{
				continue;
			}

			player_rongyan_move_ui(pstEnv,pstMemberPlayer,&stRongYanUiInfo);
		}
	}

	pstTeam->tRongYanBossSelectTimeEnd = 0;
	return 0;
	UNUSED(iBossSelect);
}

int player_rongyan_move_ui(ZONESVRENV* pstEnv, Player *pstPlayer,CSRONGYANUIINFO *pstRongYanUiInfo)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	CSRONGYANUIINFO *pstRongYanUI = &pstMiscSvr->PlayerMiscSvrData.RongYanUIInfo;
	
	pstMiscSvr->PlayerMiscType = PLAYER_RONGYAN_MOVE_UI;
	pstRongYanUI->BeginSelectMul = pstRongYanUiInfo->BeginSelectMul;
	pstRongYanUI->CurLayer = pstRongYanUiInfo->CurLayer;

	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int player_rongyan_boss_ui(ZONESVRENV* pstEnv, Player *pstPlayer,CSRONGYANBOSSINFO *pstRongYanBossInfo)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	CSRONGYANBOSSINFO *pstRongYanBoss = &pstMiscSvr->PlayerMiscSvrData.RongYanBossInfo;
	
	pstMiscSvr->PlayerMiscType = PLAYER_RONGYAN_BOSS_TRUE_FALSE_UI;
	pstRongYanBoss->BeginSelectMul = pstRongYanBossInfo->BeginSelectMul;
	pstRongYanBoss->CurLayer = pstRongYanBossInfo->CurLayer;

	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int player_rongyan_new_mon(ZONESVRENV* pstEnv, Player *pstPlayer,int tLifeTime,int iMonID,RESPOS *pstPos,int iTeamCapId,int *MemID,int iPinJunLevel,ZONEIDX *pstIdx)
{
	ZONEIDX stIdx;
	Monster stMonster;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	Monster *pstFind;
	MONSTERDEF *pstMonDef;
	char szMsg[256];
	
	//以下是刷怪 ---------------- begin ----------------
	stIdx.iID = pstIdx->iID;
	stIdx.iPos = pstIdx->iPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,  "pstMapIdx NULL");
		return -1;
	}

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	

	pstMonDef = z_get_mon_def(pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		sprintf(szMsg, "pstMonDef NULL  %d", iMonID);
		tlog_error(pstEnv->pstLogCat, 0, 0,  szMsg);
		return -1;
	}

	stMonster.bDirection = RAND1(180);

	//刷坐标
	memcpy(&stMonster.stInitPos,pstPos,sizeof(stMonster.stInitPos));
	memcpy(&stMonster.stCurrPos,pstPos,sizeof(stMonster.stCurrPos));
	
	pstFind =	z_new_monster(pstEnv->pstAppCtx, pstEnv, &stMonster, pstMonDef);
	if( !pstFind )
	{
		sprintf(szMsg, "NEW MON FAIL  %d", iMonID);
		tlog_error(pstEnv->pstLogCat, 0, 0,  szMsg);
		return -1;
	}
	pstFind->cFirstBorn = 1;
	pstFind->cOwnerIDType = 1; 
	pstFind->tLifeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + tLifeTime;

	if(0 > player_rongyanmon_dynattr(pstEnv, pstFind,iPinJunLevel))
	{
		return -1;
	}
	
	if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstFind,NULL))
	{
		tmempool_free(pstEnv->pstMonsterPool, pstFind->iMIndex);
		sprintf(szMsg, "z_refresh_mon FAIL  %d", iMonID);
		tlog_error(pstEnv->pstLogCat, 0, 0,  szMsg);
		return -1;
	}
	*MemID = pstFind->iID;
	pstFind->cFirstBorn = 0;
	//---------------- end ----------------

	//怪物记录队长id
	pstFind->iRongYanTeamCapId = iTeamCapId;

	return 0;
	UNUSED(pstPlayer);	//add by paraunused.pl
}

//融焰怪修正属性
int player_rongyanmon_dynattr(ZONESVRENV * pstEnv, Monster* pstMon,int iPinJunLevel)
{
	RONGYANMONDYNATTRDEF *pstRongYanMonDynAttrDef;
	pstRongYanMonDynAttrDef = z_find_rongyanmondynattr_def(pstEnv,iPinJunLevel);
	if(NULL == pstRongYanMonDynAttrDef)
	{
		return -1;
	}
	
	pstMon->stMonDynAttr.Level = pstRongYanMonDynAttrDef->Level;
	pstMon->stMonDynAttr.IceDefMul = pstRongYanMonDynAttrDef->IceDefMul;
	pstMon->stMonDynAttr.PhsicDefMul = pstRongYanMonDynAttrDef->PhsicDefMul;
	pstMon->stMonDynAttr.HitMul = pstRongYanMonDynAttrDef->HitMul;
	pstMon->stMonDynAttr.FleeMul = pstRongYanMonDynAttrDef->FleeMul;
	pstMon->stMonDynAttr.IceHurtDefMul= pstRongYanMonDynAttrDef->IceHurtDefMul;
	pstMon->stMonDynAttr.PhsicHurtDefMul= pstRongYanMonDynAttrDef->PhsicHurtDefMul;
	pstMon->stMonDynAttr.HPMul = pstRongYanMonDynAttrDef->DynHPMul;
	pstMon->stMonDynAttr.DynHurtMul = pstRongYanMonDynAttrDef->DynHurtMul; 
	pstMon->stMonDynAttr.IceHurtMul = pstRongYanMonDynAttrDef->IceHurtMul;
	pstMon->stMonDynAttr.PhsicHurtMul = pstRongYanMonDynAttrDef->PhsicHurtMul;
	pstMon->stMonDynAttr.ElementAtkMul = pstRongYanMonDynAttrDef->ElementAtkMul;
	pstMon->stMonDynAttr.ElementDefMul = pstRongYanMonDynAttrDef->ElementDefMul;
	return 0;
}

int player_rongyan_level(ZONESVRENV * pstEnv,Player *pstPlayer,ZoneTeam *pstTeam)
{
	
	int TeamMemberNum;
	int iPinJunLevel=0;
	int MemberLevel;
	int iMaxLevel=0;
	int i;
	int iTempLevel;
	double dPinJun = 0.0;
	
	//算队伍中的平均级别
	TeamMemberNum = pstTeam->stTeamInfo.MemberNum;
	if( TeamMemberNum <= 0)
	{
			return -1;
	}
	
	for(i = 0;i<TeamMemberNum;i++)
	{
		MemberLevel = pstTeam->stTeamInfo.TeamMember[i].Level;
		if(MemberLevel >= iMaxLevel)
		{
			iMaxLevel = MemberLevel;
		}
		
		iPinJunLevel += MemberLevel;
	}

	//原来的算法
	//iPinJunLevel = iPinJunLevel/TeamMemberNum;

	//平均级别要四舍五入
	dPinJun = (double)iPinJunLevel;
	iPinJunLevel = (int)floor(dPinJun/TeamMemberNum+0.5);

	iTempLevel = iMaxLevel-iPinJunLevel;
	if(iTempLevel>RONGYAN_LEVEL_SUB)
	{
		iPinJunLevel = iMaxLevel - RONGYAN_LEVEL_SUB;
	}
	
	//-----------------------end--------------------------------
	return iPinJunLevel;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}

int player_clan_war_award_mail(ZONESVRENV * pstEnv,  Player*pstPlayer)
{

	CLANWARSELFINFO *pstClanWarSelfInfo;

	pstClanWarSelfInfo = &pstPlayer->stRoleData.MiscInfo.ClanWarSelfInfo;

	//先直接给
	player_take_clan_war_award_in(pstEnv,pstPlayer);
	
	//todo:奖励的内容以邮件方式发
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}

int player_action_clan_war_award_info(ZONESVRENV * pstEnv,  Player*pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;	
	CLANWARSELFINFO *pstClanWarSelfInfo;


	pstClanWarSelfInfo = &pstPlayer->stRoleData.MiscInfo.ClanWarSelfInfo;

	
	memset(pstAction,0,sizeof(*pstAction));
	
	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_CLAN_WAR_AWARD_INFO;
	
	pstAction->Data.ClanWarAwardInfo.AwardInfo = pstClanWarSelfInfo->AwardInfo;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int player_action_clan_battle_apply_info(ZONESVRENV * pstEnv,  Player*pstPlayer,int iOpType)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;	
	ZoneClan* pstClan;
	ClanBattleApplyData *pstApplyData ;


	pstClan = player_get_clan(pstEnv,pstPlayer);

	if (!pstClan)
	{
		return -1;
	}

	pstApplyData = &pstClan->stZoneClanInfo.stBattleApplyData;

	if (!pstApplyData->iBattleID)
	{
		return -1;
	}

	
	memset(pstAction,0,sizeof(*pstAction));
	
	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_CLAN_BATTLE_APPLY_INFO;
	
	pstAction->Data.ClanBattleApplyInfo.BattleID = pstApplyData->iBattleID;
	pstAction->Data.ClanBattleApplyInfo.OpType = iOpType;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}



PAOSHANGDEF *z_find_paoshang_def(ZONESVRENV* pstEnv, int ID)
{
	PAOSHANGDEF  stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.PaoShangID = ID;
	iIndex = bsearch_int(&stDef, pstObj->sPaoShangDef, pstObj->iPaoShangDef, PAOSHANG_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (PAOSHANGDEF *)&pstObj->sPaoShangDef[iIndex];
	}
	
	return NULL;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(ID);	//add by paraunused.pl
}

PAOSHANGITEMDEF *z_find_paoshangitem_def(ZONESVRENV* pstEnv, int ItemID)
{	
	PAOSHANGITEMDEF  stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.ItemID = ItemID;
	iIndex = bsearch_int(&stDef, pstObj->sPaoShangItemDef, pstObj->iPaoShangItemDef, PAOSHANG_ITEM_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (PAOSHANGITEMDEF *)&pstObj->sPaoShangItemDef[iIndex];
	}  
	return NULL;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(ItemID);	//add by paraunused.pl
}

int player_open_paoshang_ui(ZONESVRENV* pstEnv, Player *pstPlayer,PAOSHANGGLOBALDATA *pstPaoShangGlo,PAOSHANGPLAYER *pstPaoShangPl)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	CSPAOSHANGUIINFO *pstPaoShangUiInfo = &pstMiscSvr->PlayerMiscSvrData.PaoShangUIInfo;
	int i;
	int k;
	int w;	
	int UserSellPrice;
	int HaveItemNum;
	int iRitemId;
	
	pstMiscSvr->PlayerMiscType = PLAYER_OPEN_PAOSHANG_UI_SVR;

	pstPaoShangUiInfo->YinPiao = pstPaoShangPl->YinPiao;
	pstPaoShangUiInfo->JiaoPiao = pstPaoShangPl->JiaoPiao;
	pstPaoShangUiInfo->RightNum = pstPaoShangPl->YiHaveItemNum;
	pstPaoShangUiInfo->LeftNum = pstPaoShangGlo->iSellNum;
	pstPaoShangUiInfo->iNpcPaoShangID = pstPlayer->stOnline.iPaoShangNpcID;

	//跑商商店左边
	for(k=0;k<pstPaoShangUiInfo->LeftNum;k++)
	{
		pstPaoShangUiInfo->PaoShangLeftUi[k].LeftItemID = pstPaoShangGlo->stPaoShangLeftData[k].iLeftItemID;
		pstPaoShangUiInfo->PaoShangLeftUi[k].UserBuyPrice = pstPaoShangGlo->stPaoShangLeftData[k].iUserBuyPrice;
		pstPaoShangUiInfo->PaoShangLeftUi[k].BuyRemainNum = pstPaoShangGlo->stPaoShangLeftData[k].iBuyRemainNum;
	}

	//跑商商店右边
	for(w=0;w<pstPaoShangUiInfo->RightNum;w++)
	{
		iRitemId = pstPaoShangUiInfo->PaoShangRightUi[w].RightItemID = pstPaoShangPl->PaoShangYiHave[w].ItemID;
		pstPaoShangUiInfo->PaoShangRightUi[w].CostPrice = pstPaoShangPl->PaoShangYiHave[w].CostPrice;
		HaveItemNum = pstPaoShangUiInfo->PaoShangRightUi[w].HaveItemNum = pstPaoShangPl->PaoShangYiHave[w].ItemNum;

		//用户身上物品可以卖出的价格
		for(i=0;i<pstPaoShangGlo->ItemTotalNum;i++)
		{
			if(pstPaoShangGlo->stPaoShangRightData[i].iRightItemID == iRitemId)
			{
				UserSellPrice = pstPaoShangUiInfo->PaoShangRightUi[w].UserSellPrice = pstPaoShangGlo->stPaoShangRightData[i].iUserSellPrice;
				break;
			}
		}

		if(i>=pstPaoShangGlo->ItemTotalNum)
		{
			return -1;
		}

		//总价
		pstPaoShangUiInfo->PaoShangRightUi[w].SellTotalPrice = HaveItemNum*UserSellPrice;
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_paoshang_clt_select(ZONESVRENV *pstEnv,Player* pstPlayer,CSPAOSHANGUISELECT *pReq)
{
	ZoneObj *pstZoneObj = pstEnv->pstZoneObj;
	
	
	PAOSHANGINFO *pstPaoShangInfo = &pstZoneObj->stPaoShangInfo;
	int iNpcNum = pstPaoShangInfo->iNpcNum;
	PAOSHANGPLAYER *pstPaoShangPl = &pstPlayer->stRoleData.MiscInfo.PaoShang;
	
	int k;
	int iPaoShaId = pstPlayer->stOnline.iPaoShangID;
	int iPaoShangNpcID = pstPlayer->stOnline.iPaoShangNpcID;
	int ret;
	PAOSHANGGLOBALDATA *pstPaoShangGlo = NULL;
	Npc *pstNpc = NULL;

	if (!pstZoneObj)
	{
		return -1;
	}

	if(NULL == pstPaoShangPl)
	{
		return -1;
	}

	/*以下是判断距离********************/
	if (pstPlayer->stOnline.cMoving)
			z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);

	

	
	pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, iPaoShangNpcID);
	if(!pstNpc)
	{
		return -1;
	}

	if (0 > z_check_op_dist(pstEnv, pstPlayer, pstNpc->stMon.stMap.iID,
	                        &pstNpc->stMon.stCurrPos, OP_DIST*2 + pstNpc->stMon.unRadius))
	{
		return -1;
	}
	/*******************end****************/

	//找出跑商id在全局的结构体
	for(k=0;k<iNpcNum;k++)
	{
		pstPaoShangGlo = &pstPaoShangInfo->stPaoShangGloData[k];
		if(pstPaoShangGlo == NULL)
		{
			continue;
		}
		
		if(pstPaoShangGlo->iPaoShangID == iPaoShaId)
		{
			break;
		}
			
	}

	if(k>=iNpcNum)
	{
		return -1;
	}

	//判断价格是否改变
	ret = player_paoshang_price_if_change(pstEnv,pstPlayer,pReq,pstPaoShangGlo,pstPaoShangPl);
	if(0 != ret)
	{
		return -1;
	}
	
	switch(pReq->BuySellType)
	{
		case PAOSHANG_BUY:
			{
				if(pstPaoShangPl->YinPiao >= pstPaoShangPl->JiaoPiao)
				{
					z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER_303);
					return 0;
				}

				player_paoshang_user_buy(pstEnv,pstPlayer,pReq,pstPaoShangGlo,pstPaoShangPl);
				break;
			}
		case PAOSHANG_SELL:
			{
				player_paoshang_user_sell(pstEnv,pstPlayer,pReq,pstPaoShangGlo,pstPaoShangPl);
				break;
			}
		case PAOSHANG_ALL_SELL:
			{
			    player_paoshang_user_all_sell(pstEnv,pstPlayer,pReq,pstPaoShangGlo,pstPaoShangPl);
				break;
			}
		default:
			return -1;
			break;
	}
	return 0;
}

//判断跑商活动价格是否改变
//返回1表示价格已经变动 ，返回0表示未变动，返回-1表示出错
int player_paoshang_price_if_change(ZONESVRENV *pstEnv,Player* pstPlayer,CSPAOSHANGUISELECT *pReq,PAOSHANGGLOBALDATA *pstPaoShangGlo,PAOSHANGPLAYER *pstPaoShangPl)
{
	int j;
	int iTotNum;
	int iSellItemNum;
	int iItemID = pReq->ItemID;
	int iPrice = pReq->Price;
	int iType = pReq->BuySellType;
	
	//记得初始化ItemTotalNum
	iTotNum = pstPaoShangGlo->ItemTotalNum;
	iSellItemNum = pstPaoShangGlo->iSellNum;

	//卖
	if(PAOSHANG_SELL == iType || PAOSHANG_ALL_SELL == iType)
	{
		//判断价格是否改变
		for(j=0;j<iTotNum;j++)
		{
			//先判断右边是否有
			if(pstPaoShangGlo->stPaoShangRightData[j].iRightItemID == iItemID)
			{
				if(pstPaoShangGlo->stPaoShangRightData[j].iUserSellPrice != iPrice)
				{
					z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER_304);
					player_open_paoshang_ui(pstEnv,pstPlayer,pstPaoShangGlo,pstPaoShangPl);
					return 1;
				}
				else
				{
					return 0;
				}
			}
		}

		if(j>=iTotNum)
		{
			return -1;
		}
	}
	else if(PAOSHANG_BUY == iType)
	{
	//买
		//判断价格是否改变
		for(j=0;j<iSellItemNum;j++)
		{
			if(pstPaoShangGlo->stPaoShangLeftData[j].iLeftItemID == iItemID)
			{
				if(pstPaoShangGlo->stPaoShangLeftData[j].iUserBuyPrice != iPrice)
				{
					z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER_305);
					player_open_paoshang_ui(pstEnv,pstPlayer,pstPaoShangGlo,pstPaoShangPl);
					return 1;
				}
				else
				{
					return 0;
				}
			}
		}

		if(j>=iSellItemNum)
		{
			return -1;
		}
	}
	else
	{
		return 0;
	}

 	return 0;
}

int player_paoshang_user_buy(ZONESVRENV *pstEnv,Player* pstPlayer,CSPAOSHANGUISELECT *pReq,PAOSHANGGLOBALDATA *pstPaoShangGlo,PAOSHANGPLAYER *pstPaoShangPl)
{
	UNUSED(pstEnv);
	
	int w;
	int iYinPiao;
	int i;
	int j;
	
	//记得初始化
	int iSellItemNum = pstPaoShangGlo->iSellNum;
	int iYiHaveItemNum = pstPaoShangPl->YiHaveItemNum;
	int iLimitNum = 0;

	if(pReq->Num <= 0)
	{
		return -1;
	}

	for(j=0;j<iYiHaveItemNum;j++)
	{
		iLimitNum+=pstPaoShangPl->PaoShangYiHave[j].ItemNum;
	}

	iLimitNum = iLimitNum+pReq->Num;
	if(iLimitNum > 16)
	{
		//z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "你的货仓已经装不下更多东西了，可将贸易商品出售给贸易商人以清空货仓");
		return 0;
	}
	
	for(i=0;i<iSellItemNum;i++)
	{
		if(pReq->ItemID == pstPaoShangGlo->stPaoShangLeftData[i].iLeftItemID)
		{
			if(pstPaoShangGlo->stPaoShangLeftData[i].iBuyRemainNum == 0)
			{
				z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PLAYER_306);
				player_open_paoshang_ui(pstEnv,pstPlayer,pstPaoShangGlo,pstPaoShangPl);
				return 0;
			}
			
			if(pReq->Num > pstPaoShangGlo->stPaoShangLeftData[i].iBuyRemainNum)
			{
				//客户端购买的数量超于卖家还剩的
				return -1;
			}
			else
			{
				iYinPiao = (pReq->Num)*(pReq->Price);
				if(iYinPiao > pstPaoShangPl->YinPiao)
				{
					//银票不足
					return -1;
				}

				pstPaoShangPl->YinPiao -= iYinPiao;
				pstPaoShangGlo->stPaoShangLeftData[i].iBuyRemainNum-= pReq->Num;

				//假设身上已有，就在身上叠加
				for(w=0;w<iYiHaveItemNum;w++)
				{
					if(pReq->ItemID == pstPaoShangPl->PaoShangYiHave[w].ItemID)
					{
						pstPaoShangPl->PaoShangYiHave[w].ItemNum += pReq->Num;
						pstPaoShangPl->PaoShangYiHave[w].CostPrice += iYinPiao;
						break;
					}
				}

				//身上没有。重新分配
				if(w>=iYiHaveItemNum)
				{
					pstPaoShangPl->YiHaveItemNum++;
					pstPaoShangPl->PaoShangYiHave[pstPaoShangPl->YiHaveItemNum-1].CostPrice = iYinPiao;
					pstPaoShangPl->PaoShangYiHave[pstPaoShangPl->YiHaveItemNum-1].ItemID = pReq->ItemID;
					pstPaoShangPl->PaoShangYiHave[pstPaoShangPl->YiHaveItemNum-1].ItemNum = pReq->Num;
				}
				z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PLAYER_307,iYinPiao,pReq->Num);
			}
			break;
		}
	}

	player_open_paoshang_ui(pstEnv,pstPlayer,pstPaoShangGlo,pstPaoShangPl);
	return 0;
}

int player_paoshang_user_sell(ZONESVRENV *pstEnv,Player* pstPlayer,CSPAOSHANGUISELECT *pReq,PAOSHANGGLOBALDATA *pstPaoShangGlo,PAOSHANGPLAYER *pstPaoShangPl)
{
	UNUSED(pstEnv);
	UNUSED(pstPaoShangGlo);
		
	int w;
	int iYiHaveItemNum = pstPaoShangPl->YiHaveItemNum;
	if(0 >= iYiHaveItemNum)
	{
		return -1;
	}
	tdr_longlong llprice = 0;
	int reNum = pReq->Num;
	int iPinJuPrice = 0;
	
	//身上是否有该物品
	for (w=iYiHaveItemNum-1; w>=0; w--)
	{
		if(pReq->ItemID == pstPaoShangPl->PaoShangYiHave[w].ItemID)
		{
			if(pstPaoShangPl->PaoShangYiHave[w].ItemNum <= reNum)
			{
				//身上该物品没有那么多
				reNum = pstPaoShangPl->PaoShangYiHave[w].ItemNum;
				llprice = reNum*(pReq->Price);
				pstPaoShangPl->PaoShangYiHave[w].ItemNum = 0;
				pstPaoShangPl->PaoShangYiHave[w].CostPrice = 0;
				pstPaoShangPl->PaoShangYiHave[w].ItemID = 0;

				pstPaoShangPl->YiHaveItemNum--;
				if (w != pstPaoShangPl->YiHaveItemNum)
				{
					memmove(&pstPaoShangPl->PaoShangYiHave[w], &pstPaoShangPl->PaoShangYiHave[pstPaoShangPl->YiHaveItemNum],
						sizeof(pstPaoShangPl->PaoShangYiHave[w]));
				}
			}
		    else
		   	{
		   		llprice = reNum*(pReq->Price);
				iPinJuPrice = (pstPaoShangPl->PaoShangYiHave[w].CostPrice)/(pstPaoShangPl->PaoShangYiHave[w].ItemNum);
				pstPaoShangPl->PaoShangYiHave[w].ItemNum -= reNum;
				pstPaoShangPl->PaoShangYiHave[w].CostPrice -= (iPinJuPrice*reNum);
		    }
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PLAYER_308,reNum,llprice);
			break;
		}		
	}
	
	//身上没有该物品
	if(w < 0)
	{
		return -1;
	}

	pstPaoShangPl->YinPiao += llprice;

	if(pstPaoShangPl->YiHaveItemNum == 0 && pstPaoShangPl->YinPiao <= 5000)
	{
		//出现可以兑换额度的选项
		pstPaoShangPl->ExchangeFlag = 0;
		pstPaoShangPl->YiExchangeYinPiao = 0;
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PLAYER_309);
	}
	
	if(pstPaoShangPl->YinPiao >= pstPaoShangPl->JiaoPiao)
	{
		task_paoshang_fini(pstEnv, pstPlayer,pstPaoShangPl->YinPiao);
	}

	player_open_paoshang_ui(pstEnv,pstPlayer,pstPaoShangGlo,pstPaoShangPl);
	return 0;
	UNUSED(pstPaoShangGlo);	//add by paraunused.pl
}

int player_paoshang_user_all_sell(ZONESVRENV *pstEnv,Player* pstPlayer,CSPAOSHANGUISELECT *pReq,PAOSHANGGLOBALDATA *pstPaoShangGlo,PAOSHANGPLAYER *pstPaoShangPl)
{
	UNUSED(pstEnv);
	UNUSED(pReq);
	
	int j;
	int w;
	int iUserSellPrice = 0;
	tdr_longlong llprice = 0;
	int iTotNum = pstPaoShangGlo->ItemTotalNum;
	int iYiHaveItemNum = pstPaoShangPl->YiHaveItemNum;
	if(0 == iYiHaveItemNum)
	{
		return -1;
	}

	//全部卖出			
	for(w=0;w<iYiHaveItemNum;w++)
	{
		for(j=0;j<iTotNum;j++)
		{
			if(pstPaoShangPl->PaoShangYiHave[w].ItemID == pstPaoShangGlo->stPaoShangRightData[j].iRightItemID)
			{
				iUserSellPrice = pstPaoShangGlo->stPaoShangRightData[j].iUserSellPrice;
				iUserSellPrice =  iUserSellPrice*(pstPaoShangPl->PaoShangYiHave[w].ItemNum);
				llprice += iUserSellPrice;
				pstPaoShangPl->PaoShangYiHave[j].ItemNum = 0;
				pstPaoShangPl->PaoShangYiHave[j].CostPrice = 0;
				pstPaoShangPl->PaoShangYiHave[j].ItemID = 0;
				break;
			}
		}
	}

	pstPaoShangPl->YiHaveItemNum = 0;
	//卖出的价格叠加在银票额上
	pstPaoShangPl->YinPiao += llprice;

	z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PLAYER_310,llprice);

	if(pstPaoShangPl->YinPiao <= 5000)
	{
		//出现可以兑换额度的选项
		pstPaoShangPl->ExchangeFlag = 0;
		pstPaoShangPl->YiExchangeYinPiao = 0;
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PLAYER_309);
	}
	
	if(pstPaoShangPl->YinPiao >= pstPaoShangPl->JiaoPiao)
	{
		task_paoshang_fini(pstEnv, pstPlayer,pstPaoShangPl->YinPiao);
	}
	
	player_open_paoshang_ui(pstEnv,pstPlayer,pstPaoShangGlo,pstPaoShangPl);
	return 0;
}

//登录发的背包货仓信息
int player_paoshang_login_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int w;
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	CSPAOSHANGUIINFO *pstPaoShangUiInfo = &pstMiscSvr->PlayerMiscSvrData.PaoShangUIInfo;
	PAOSHANGPLAYER *pstPaoShangPl = &pstPlayer->stRoleData.MiscInfo.PaoShang;
	if(NULL == pstPaoShangPl)
	{
		return -1;
	}
	
	pstMiscSvr->PlayerMiscType = PLAYER_OPEN_PAOSHANG_UI_SVR;
	
	pstPaoShangUiInfo->RightNum = pstPaoShangPl->YiHaveItemNum;
	//约定左边道具数量为0时。此消息为登录时发的背包仓库信息
	pstPaoShangUiInfo->LeftNum = 0;
	pstPaoShangUiInfo->iNpcPaoShangID = pstPlayer->stOnline.iPaoShangNpcID;
	pstPaoShangUiInfo->YinPiao = pstPaoShangPl->YinPiao;
	pstPaoShangUiInfo->JiaoPiao = pstPaoShangPl->JiaoPiao;
	
	for(w=0;w<pstPaoShangUiInfo->RightNum;w++)
	{
		pstPaoShangUiInfo->PaoShangRightUi[w].RightItemID = pstPaoShangPl->PaoShangYiHave[w].ItemID;
		pstPaoShangUiInfo->PaoShangRightUi[w].HaveItemNum = pstPaoShangPl->PaoShangYiHave[w].ItemNum;
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

//lzk  
//跑商整点刷新物品价格    和    初始化内存
int player_paoshang_init(ZONESVRENV* pstEnv)
{	
	ZoneObj *pstZoneObj = pstEnv->pstZoneObj;
	if (!pstZoneObj)
	{
		return -1;
	}
	
	PAOSHANGINFO *pstPaoShangInfo = &pstZoneObj->stPaoShangInfo;
	PAOSHANGGLOBALDATA *pstPaoShangGlo = NULL;	
	PAOSHANGDEF *pstPaoShangDef = NULL;
	PAOSHANGITEMDEF *pstPaoShangItemDef = NULL;
	int iSellNum;
	int ItemTotalNum;
	int iLeftItemID;
	int iSellPrice = 0;
	int iRightItemID;
	int iRightSellPrice = 0;
	int iTempSellPrice = 0;
	int k;
	int i;
	int j;
	int w;
		
	//npc跑商的数量
	pstPaoShangInfo->iNpcNum = pstEnv->pstZoneObj->iPaoShangDef;
	//记录刷新时间
	pstPaoShangInfo->PaoShangRefTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	for(k=0;k<pstPaoShangInfo->iNpcNum;k++)
	{
		pstPaoShangDef = (PAOSHANGDEF *)&pstZoneObj->sPaoShangDef[k];
		
		pstPaoShangGlo = &pstPaoShangInfo->stPaoShangGloData[k];
				
		pstPaoShangGlo->iPaoShangID = pstPaoShangDef->PaoShangID;
		iSellNum = pstPaoShangGlo->iSellNum = pstPaoShangDef->Num;
		//考虑一下
		ItemTotalNum = pstPaoShangGlo->ItemTotalNum = pstEnv->pstZoneObj->iPaoShangItemDef;

		//跑商界面左边
		for(i=0;i<iSellNum;i++)
		{
			iLeftItemID = pstPaoShangGlo->stPaoShangLeftData[i].iLeftItemID = pstPaoShangDef->PaoShangItem[i];
	
			pstPaoShangItemDef = z_find_paoshangitem_def(pstEnv,iLeftItemID);
			if(NULL == pstPaoShangItemDef)
			{
				continue;
			}

			pstPaoShangGlo->stPaoShangLeftData[i].iBuyRemainNum = pstPaoShangItemDef->SellNum;
			iSellPrice = abs(pstPaoShangItemDef->SellPriceMax - pstPaoShangItemDef->SellPriceMin);
			iSellPrice = RAND1(iSellPrice+1);
			pstPaoShangGlo->stPaoShangLeftData[i].iUserBuyPrice = pstPaoShangItemDef->SellPriceMin + iSellPrice;
		}

		//跑商界面右边
		for(j=0;j<ItemTotalNum;j++)
		{
			pstPaoShangItemDef = (PAOSHANGITEMDEF *)&pstEnv->pstZoneObj->sPaoShangItemDef[j];
					
			iRightItemID = pstPaoShangGlo->stPaoShangRightData[j].iRightItemID = pstPaoShangItemDef->ItemID;
			iRightSellPrice = abs(pstPaoShangItemDef->BuyPriceMax - pstPaoShangItemDef->BuyPriceMin);
			iRightSellPrice = RAND1(iRightSellPrice+1);
			iTempSellPrice = pstPaoShangGlo->stPaoShangRightData[j].iUserSellPrice = pstPaoShangItemDef->BuyPriceMin + iRightSellPrice;

			//若商人本身也出售 这件物品，就会是当前售价的一半
			for(w=0;w<iSellNum;w++)
			{	
				if(pstPaoShangGlo->stPaoShangLeftData[w].iLeftItemID == iRightItemID)
				{
					pstPaoShangGlo->stPaoShangRightData[j].iUserSellPrice = iTempSellPrice/2;
					break;
				}
			}
		}
	}
	
	return 0;
}

//lzk  清空跑商相关变量
int player_paoshang_clean(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	PAOSHANGPLAYER *pstPaoShangPl = &pstPlayer->stRoleData.MiscInfo.PaoShang;
	if(NULL == pstPaoShangPl)
	{
		return -1;
	}

	//身上的跑商相关变量清空
	memset(pstPaoShangPl,0,sizeof(*pstPaoShangPl));
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl


}

int player_action_precreate_gift_verify(ZONESVRENV * pstEnv,  Player* pstPlayer,int iOpType,int iRet)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;		
	CSACTIONPRECREATEGIFTVERIFY *pstVerify = &pstAction->Data.PrecreateGiftVerify;
	
	memset(pstAction,0,sizeof(*pstAction));
	
	pstAction->ID = pstPlayer->iMemID;
	pstAction->ActionID = ACTION_PRECREATE_GIFT_VERIFY;

	if (iOpType == ACTION_PRECREATE_GIFT_VERIFY_OP_SN)
	{
		//todo:sms
		pstEnv->iSmsSN++;
		if (pstEnv->iSmsSN >= 100)
		{
			pstEnv->iSmsSN = 1;
		}
		pstPlayer->stOnline.iSmsCode = 100000 + RAND1(900000);
		pstPlayer->stOnline.iSmsTimeOut = pstEnv->pstAppCtx->stCurr.tv_sec + PRECREATE_GIFT_VERIFY_TIMEOUT;

		if (0 > z_ss_sms_req(pstEnv->pstAppCtx,pstEnv,pstPlayer,pstEnv->iSmsSN))
		{
			return -1;
		}
		
		pstVerify->Data.NewSN = pstEnv->iSmsSN;

		//todo:要注释掉
		if (pstEnv->pstConf->ResloadType == RES_LOAD_DB)
		{
			//z_gm_smscode(pstEnv->pstAppCtx, pstEnv,pstPlayer,"");
		}
		
	}
	else if (iOpType == ACTION_PRECREATE_GIFT_VERIFY_OP_OPEN)
	{
		pstVerify->Data.Open = 0;
	}
	else if (iOpType == ACTION_PRECREATE_GIFT_VERIFY_OP_RESULT)
	{
		pstVerify->Data.Result = iRet;
	}
	else
	{
		return -1;
	}

	pstVerify->OpType = iOpType;
	
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_beach_low_exp_ui_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	CSBEACHLOWEXPUI *pstBeachLowExpUI= &pstMiscSvr->PlayerMiscSvrData.BeachLowExpUI;

	pstMiscSvr->PlayerMiscType = PLAYER_BEACH_LOW_EXP_SVR;
	
	pstBeachLowExpUI->iReq = 0;
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int z_player_level_remain_tm_update(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	LEVELREMAININFO *pstLevelRemainInfo = &pstPlayer->stRoleData.MiscInfo.LevelRemainInfo;

	if (!IsSameDay(pstLevelRemainInfo->RecordTime,	pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstLevelRemainInfo->RecordTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstLevelRemainInfo->flag = 0;
	}
	else
	{
		if(pstLevelRemainInfo->flag == 0 && pstPlayer->stRoleData.MiscInfo.ODTime.OndaySec >= LEVEL_REMAIN_TM)
		{
			pstLevelRemainInfo->LevelRemainDay++;
			pstLevelRemainInfo->flag = 1;
		}
	}
	
	return 0;
}

int player_boss_baoxiang_if_touch(ZONESVRENV* pstEnv,Player *pstPlayer,tdr_ulonglong WID)
{
	BOSSBAOXIANGINFO *pstBossBaoXiangInfo = &pstPlayer->stRoleData.MiscInfo.BossBaoXiangInfo;
	BAOXIANG *pstBaoXiang = NULL;
	int i = 0;
	int iNum = pstBossBaoXiangInfo->BaoXiangNum;

	if(iNum > MAX_PLAYER_BOSS_BXIANG_NUM || iNum < 0)
	{
		return -1;
	}

	for(i=0;i<iNum;i++)
	{
		pstBaoXiang = &pstBossBaoXiangInfo->BaoXiang[i];

		if(pstBaoXiang->WID == WID)
		{
			return 1;
		}
	}

	return 0;
}

int player_boss_baoxiang_touch_set(ZONESVRENV* pstEnv,Player *pstPlayer,tdr_ulonglong WID)
{
	BOSSBAOXIANGINFO *pstBossBaoXiangInfo = &pstPlayer->stRoleData.MiscInfo.BossBaoXiangInfo;
	int iNum = pstBossBaoXiangInfo->BaoXiangNum;
	BAOXIANG stBXiang;
	memset(&stBXiang,0,sizeof(stBXiang));

	if(iNum > MAX_PLAYER_BOSS_BXIANG_NUM || iNum < 0)
	{
		return -1;
	}

	stBXiang.WID = WID;
	if(pstBossBaoXiangInfo->BaoXiangNum >= MAX_PLAYER_BOSS_BXIANG_NUM)
	{
		memmove(&pstBossBaoXiangInfo->BaoXiang[0],&pstBossBaoXiangInfo->BaoXiang[1],sizeof(pstBossBaoXiangInfo->BaoXiang[0])*(MAX_PLAYER_BOSS_BXIANG_NUM-1));
		memcpy(&pstBossBaoXiangInfo->BaoXiang[MAX_PLAYER_BOSS_BXIANG_NUM-1],&stBXiang,sizeof(pstBossBaoXiangInfo->BaoXiang[MAX_PLAYER_BOSS_BXIANG_NUM-1]));
	}
	else
	{
		memcpy(&pstBossBaoXiangInfo->BaoXiang[pstBossBaoXiangInfo->BaoXiangNum],&stBXiang,sizeof(pstBossBaoXiangInfo->BaoXiang[pstBossBaoXiangInfo->BaoXiangNum]));
		pstBossBaoXiangInfo->BaoXiangNum++;
	}

	return 1;
}

int player_zj_rongyan_ui(ZONESVRENV* pstEnv, Player *pstPlayer,ZJRONGYANINFO *pstZJRongYanInfo)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	ZJRONGYANINFO *pstZJRongYanUI = &pstMiscSvr->PlayerMiscSvrData.ZJRongYanInfo;
	
	pstMiscSvr->PlayerMiscType = PLAYER_ZJ_RONGYAN_SVR;
	memcpy(pstZJRongYanUI,pstZJRongYanInfo,sizeof(*pstZJRongYanUI));	

	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

ZJRONGYANJUSHOUDEF *z_find_zjrongyanjushou_def(ZONESVRENV* pstEnv, int iLevel)
{
	ZJRONGYANJUSHOUDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Level = iLevel;
	iIndex = bsearch_int(&stDef, pstObj->sZJRongYanJuShouDef, pstObj->iZJRongYanJuShouDef, ZJ_RONGYANJUSHOU_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ZJRONGYANJUSHOUDEF *)&pstObj->sZJRongYanJuShouDef[iIndex];
	}
	
	return NULL;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(iLevel);	//add by paraunused.pl
}

int player_call_zj_rongyan(ZONESVRENV* pstEnv,Player *pstPlayer, ZJRONGYANCALL *pstZJRongYanCall)
{
	ZJRONGYANJUSHOUDEF *pstZJRongYanJuShouDef;
	ITEMDEC stDec;
	ITEMDEF *pstItemDef;
	int iRet;
	ZJRONGYANINFO stZJRongYanInfo;
	ZJRONGYAN *pstZJRongYan;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	tdr_longlong llExp = 0;

	time_t now;
	struct tm *pstnow;

	now = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstnow = localtime(&now);

	//超级熔岩巨兽，限定开放时间是12:00-24:00之间可以使用
	if(pstnow->tm_hour < 12)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RONGYAN21);
		return -1;
	}

	if(is_fini_global(pstEnv) == 0)
	{
		return 0;
	}

	ZJRONGYANGLOBAL *pstZJRongYanGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.ZJRongYanGlobal;

	//只能在各野外出安全区意外的中立地图使用
	if(1 != is_player_zhongli_area(pstEnv,pstPlayer))
	{
		return -1;
	}

	pstZJRongYanJuShouDef = z_find_zjrongyanjushou_def(pstEnv,ZJ_RONGYAN_FIRST_LEVEL);
	if(!pstZJRongYanJuShouDef)
	{
		return -1;
	}

	//服务器每天最多召唤3只
	if(pstZJRongYanGlobal->Num >= MAX_ZJRONGYAN_NUM || pstZJRongYanGlobal->Num < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RONGYAN18);
		return -1;
	} 

	//同一时间只能存在一只判断,已经有在倒计时5分钟的，进行boss选择也不能召唤了
	if(pstZJRongYanGlobal->ZJRongYan.Status == 1 || pstZJRongYanGlobal->ZJRongYan.Status == 2 ||
	    pstZJRongYanGlobal->ZJRongYan.Status == 4)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RONGYAN19);
		return -1;
	}

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,  "pstMapIdx NULL");
		return -1;
	}
	
	pstItemDef = z_find_itemdef(pstEnv, ZJ_RONGYAN_ITEM_ID);
	if (NULL == pstItemDef)
	{
		return -1;
	}

	//以下是扣除道具------------begin----------------
	if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, ZJ_RONGYAN_ITEM_ID, 1, -1, NULL,PACKAGE_DEC_NONE))
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_MISC_306,pstItemDef->Name);
		return -1;
	}

	stDec.ID = ZJ_RONGYAN_ITEM_ID;
	stDec.Num = 1;
	stDec.BindNum = 0;
	
	iRet = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, ZJ_RONGYAN_ITEM_ID, 1, -1,
			NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_USE);
	if (iRet > 0)
		stDec.BindNum = iRet;
	
	if (iRet >= 0)
	{
		z_role_item_dec_oplog(pstEnv,pstPlayer, &stDec,OP_ITEM_DEC_USE);
	}
	//--------------------------扣除物品end  -----------------

	stZJRongYanInfo.Type = 0;
	stZJRongYanInfo.ExpMul = pstZJRongYanJuShouDef->ExpMul;
	
	llExp = player_get_exp_by_zj_rongyan(pstEnv,pstPlayer->stRoleData.Level,pstZJRongYanJuShouDef->Level);
	if(llExp <= 0)
	{
		stZJRongYanInfo.CurExp = 0;
	}
	else
	{
		stZJRongYanInfo.CurExp = 3*llExp;
	}
	stZJRongYanInfo.Level = pstZJRongYanJuShouDef->MonLevel;
	stZJRongYanInfo.NextExpMul = pstZJRongYanJuShouDef->ExpMul*2;
	stZJRongYanInfo.DaoJiShi = 300;

	player_zj_rongyan_ui(pstEnv, pstPlayer,&stZJRongYanInfo);

	pstZJRongYan = &pstZJRongYanGlobal->ZJRongYan;
	memset(pstZJRongYan,0,sizeof(*pstZJRongYan));
	pstZJRongYan->CallTime = pstEnv->pstAppCtx->stCurr.tv_sec + 300;
	pstZJRongYan->NotifyTime = pstEnv->pstAppCtx->stCurr.tv_sec + 60;
	pstZJRongYan->stPos = pstPlayer->stRoleData.Pos;
	pstZJRongYan->Map = pstPlayer->stRoleData.Map;
	pstZJRongYan->iMapPos = pstPlayer->stOnline.iMapPos;
	STRNCPY(pstZJRongYan->CallName,pstPlayer->stRoleData.RoleName,sizeof(pstZJRongYan->CallName));
	STRNCPY(pstZJRongYan->MapName,pstMapIdx->stMapDef.Name,sizeof(pstZJRongYan->MapName));
	pstZJRongYan->NotifyMin = 5;
	pstZJRongYan->Level = pstZJRongYanJuShouDef->Level;
	pstZJRongYan->Status = 1;

	z_sys_strf_broadcast(pstEnv, SYS_SVR, ZONEERR_RONGYAN16,pstZJRongYan->CallName,pstZJRongYan->NotifyMin ,pstZJRongYan->MapName);
	return 0;
	UNUSED(pstZJRongYanCall);	//add by paraunused.pl
}

int zj_rongyan_tick(ZONESVRENV* pstEnv)
{
	if(is_fini_global(pstEnv) == 0)
	{
		return 0;
	}

	ZJRONGYANGLOBAL *pstZJRongYanGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.ZJRongYanGlobal;
	ZJRONGYAN *pstZJRongYan;
	tdr_ulonglong WID = 0;
	ZONEIDX stIdx;

	if(!IsSameDay(pstZJRongYanGlobal->Time ,pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstZJRongYanGlobal->Num = 0;
		pstZJRongYanGlobal->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	pstZJRongYan = &pstZJRongYanGlobal->ZJRongYan;
	if(pstZJRongYan->CallTime != 0)
	{
		if(pstZJRongYan->NotifyTime != 0 && pstZJRongYan->NotifyTime < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			pstZJRongYan->NotifyTime = pstEnv->pstAppCtx->stCurr.tv_sec+60;
			pstZJRongYan->NotifyMin--;
			if(pstZJRongYan->NotifyMin > 0)
			{
				z_sys_strf_broadcast(pstEnv, SYS_SVR, ZONEERR_RONGYAN16,pstZJRongYan->CallName,pstZJRongYan->NotifyMin,pstZJRongYan->MapName);
			}
			else
			{
				pstZJRongYan->NotifyTime = 0;
				pstZJRongYan->NotifyMin = 0;
			}
		}
	
		if( pstZJRongYan->CallTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			stIdx.iID = pstZJRongYan->Map;
			stIdx.iPos = pstZJRongYan->iMapPos;
			ZJRONGYANJUSHOUDEF *pstZJRongYanJuShouDef = NULL;

			pstZJRongYanJuShouDef = z_find_zjrongyanjushou_def(pstEnv,pstZJRongYan->Level);
			if(!pstZJRongYanJuShouDef)
			{
				return -1;
			}

			if(0 == player_zj_rongyan_new_mon(pstEnv,pstZJRongYanJuShouDef->MonLifeTime,pstZJRongYanJuShouDef->MonID,&pstZJRongYan->stPos,&WID,&stIdx))
			{
				z_sys_strf_broadcast(pstEnv, SYS_SVR, ZONEERR_RONGYAN20,pstZJRongYan->MapName);
				pstZJRongYan->ZJRongYanWID = WID;
				pstZJRongYan->CallTime = 0;
				pstZJRongYan->NotifyTime = 0;
				pstZJRongYan->NotifyMin = 0;
				pstZJRongYan->tBossSelectTm = 0;
				pstZJRongYan->Status = 2;
				pstZJRongYan->tMonLifeTime = pstEnv->pstAppCtx->stCurr.tv_sec + pstZJRongYanJuShouDef->MonLifeTime;
				pstZJRongYanGlobal->Num++;
			}
		}
	}

	//防止服务器重启或其他情况导致终极熔岩巨兽没了，从此不能再召唤的情况
	if((pstZJRongYan->tMonLifeTime != 0) && (pstZJRongYan->tMonLifeTime + 900 < pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		if(pstZJRongYan->Status == 1 || pstZJRongYan->Status == 2 || pstZJRongYan->Status == 4)
		{
			pstZJRongYan->Status = 0;
			pstZJRongYan->tMonLifeTime = 0;
		}
	}

	if(pstZJRongYan->Status == 4)
	{
		if(pstZJRongYan->tBossSelectTm != 0 && pstZJRongYan->tBossSelectTm < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			CSZJRONGYANBOSSSELECT stReq;
			stReq.iSelect = 0;
			player_zj_rongyan_boss(pstEnv,NULL,&stReq);
		}
	}
	
	return 0;
}

int player_zj_rongyan_new_mon(ZONESVRENV* pstEnv,int tLifeTime,int iMonID,RESPOS *pstPos,tdr_ulonglong *WID,ZONEIDX *pstIdx)
{
	ZONEIDX stIdx;
	Monster stMonster;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	Monster *pstFind;
	MONSTERDEF *pstMonDef;
	char szMsg[256];
	
	//以下是刷怪 ---------------- begin ----------------
	stIdx.iID = pstIdx->iID;
	stIdx.iPos = pstIdx->iPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,  "pstMapIdx NULL");
		return -1;
	}

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	

	pstMonDef = z_get_mon_def(pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		sprintf(szMsg, "pstMonDef NULL  %d", iMonID);
		tlog_error(pstEnv->pstLogCat, 0, 0,  szMsg);
		return -1;
	}

	stMonster.bDirection = RAND1(180);

	//刷坐标
	memcpy(&stMonster.stInitPos,pstPos,sizeof(stMonster.stInitPos));
	memcpy(&stMonster.stCurrPos,pstPos,sizeof(stMonster.stCurrPos));
	
	pstFind =	z_new_monster(pstEnv->pstAppCtx, pstEnv, &stMonster, pstMonDef);
	if( !pstFind )
	{
		sprintf(szMsg, "NEW MON FAIL  %d", iMonID);
		tlog_error(pstEnv->pstLogCat, 0, 0,  szMsg);
		return -1;
	}
	pstFind->cFirstBorn = 1;
	pstFind->cOwnerIDType = 1; 
	pstFind->tLifeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + tLifeTime;

	if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstFind,NULL))
	{
		tmempool_free(pstEnv->pstMonsterPool, pstFind->iMIndex);
		sprintf(szMsg, "z_refresh_mon FAIL  %d", iMonID);
		tlog_error(pstEnv->pstLogCat, 0, 0,  szMsg);
		return -1;
	}

	*WID = pstFind->WID;
	pstFind->cFirstBorn = 0;
	//---------------- end ----------------

	return 0;
}

int zj_rongyan_mon_if_have_by_wid(ZONESVRENV* pstEnv,tdr_ulonglong WID)
{
	if(is_fini_global(pstEnv) == 0)
	{
		return -1;
	}

	ZJRONGYANGLOBAL *pstZJRongYanGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.ZJRongYanGlobal;
	if(pstZJRongYanGlobal->ZJRongYan.ZJRongYanWID == WID)
	{
		return 1;
	}
	
	return -1;
}

int mon_zj_rongyan_die(ZONESVRENV* pstEnv,Monster* pstMon)
{
	if(is_fini_global(pstEnv) == 0)
	{
		return -1;
	}

	ZJRONGYANGLOBAL *pstZJRongYanGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.ZJRongYanGlobal;
	ZJRONGYAN *pstZJRongYan;
	int iLevel = 0;
	ZJRONGYANJUSHOUDEF  *pstZJRongYanJuShouDef = NULL;
	Player *pstPlayer = NULL;
	int iExist = 0;

	pstZJRongYan = &pstZJRongYanGlobal->ZJRongYan;
	pstZJRongYan->Status = 3;
	pstZJRongYan->tMonLifeTime = 0;

	iLevel = pstZJRongYan->Level;

	pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,pstZJRongYan->CallName);
	if(pstPlayer != NULL)
	{
		iExist = 1;
	}

	if(iLevel >= ZJ_RONGYAN_FIRST_LEVEL && iLevel < ZJ_RONGYAN_TOP_LEVEL)
	{
		z_sys_strf_broadcast(pstEnv, SYS_SVR, ZONEERR_RONGYAN17,pstZJRongYan->CallName,iLevel);
		mon_zj_rongyan_die_player_add_exp(pstEnv,pstMon);

		//不做选择的话，3分钟后自动选择真假boss
		pstZJRongYan->tBossSelectTm = pstEnv->pstAppCtx->stCurr.tv_sec + 180;
		//4表示boss选择
		pstZJRongYan->Status = 4;

		if(1 == iExist)
		{
			zj_rongyan_close_dlg(pstEnv, pstPlayer);
			
			CSZJRONGYANBOSSINFO stZJRongYanBossInfo;
			memset(&stZJRongYanBossInfo,0,sizeof(stZJRongYanBossInfo));

			iLevel++;
			pstZJRongYanJuShouDef = z_find_zjrongyanjushou_def(pstEnv,iLevel);
			if(!pstZJRongYanJuShouDef)
			{
				stZJRongYanBossInfo.NextExpMul = 0;
			}
			else
			{
				stZJRongYanBossInfo.NextExpMul = pstZJRongYanJuShouDef->ExpMul;
			}
			
			stZJRongYanBossInfo.Type = 0;
			player_zj_rongyan_boss_select_ui(pstEnv, pstPlayer,&stZJRongYanBossInfo);
		}
	}
	else if(iLevel == ZJ_RONGYAN_TOP_LEVEL)
	{
		z_sys_strf_broadcast(pstEnv, SYS_SVR, ZONEERR_RONGYAN17,pstZJRongYan->CallName,iLevel);
		mon_zj_rongyan_die_player_add_exp(pstEnv,pstMon);
		if(1 == iExist)
		{
			zj_rongyan_close_dlg(pstEnv, pstPlayer);
		}
	}
	else if(iLevel == 0)
	{
		//假怪
		if(1 == iExist)
		{
			zj_rongyan_close_dlg(pstEnv, pstPlayer);
		}
	}
	else if(iLevel == -2)
	{
		//在限定时间内没有将怪物击杀  ,zone_svr.c中mon_active  将pstZJRongYan->Level赋值为-2
		if(pstMon->iHP > 0)
		{
			if(1 == iExist)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RONGYAN3);
				zj_rongyan_close_dlg(pstEnv, pstPlayer);
			}
		}
	}
	else
	{
		return -1;
	}

	return 0;
}

int mon_zj_rongyan_die_player_add_exp(ZONESVRENV* pstEnv,Monster* pstMon)
{
	Player *pstVPlayer = NULL;
	int iZJret = 0; 
	tdr_longlong llExp = 0;
	ZJRONGYAN *pstZJRongYan = NULL;
	int i, j;
	SearchArea stSArea;
	AREADYN *pstArea;
	ZONEMAPINST* pstMapInst;
	ZONEIDX stIdx;
	int flag = 0;

	if(is_fini_global(pstEnv) == 0)
	{
		return -1;
	}

	ZJRONGYANGLOBAL *pstZJRongYanGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.ZJRongYanGlobal;
	pstZJRongYan = &pstZJRongYanGlobal->ZJRongYan;
	
	stIdx.iID = pstZJRongYan->Map;
	stIdx.iPos = pstZJRongYan->iMapPos;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	
	if (z_pos_search_area(pstEnv->pstAppCtx, pstEnv, pstMapInst, &pstMon->stCurrPos, &stSArea) < 0) 
	{
		return -1;
	}
		
	for (i = 0; i < stSArea.iNum; i++) 
	{
		pstArea = stSArea.pastArea[i];
		for (j = 0; j < pstArea->iAnimate; j++) 
		{
			pstVPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstArea->aiAnimate[j]);
			if (NULL == pstVPlayer)
			{
				continue;
			}

			if (PLAYER_STATUS_ROLE_LOGIN != pstVPlayer->eStatus)
			{
				continue;
			}

			if (!z_pos_in_view(&pstVPlayer->stRoleData.Pos, &pstMon->stCurrPos))
			{
				continue;
			}

			if(pstMon->WID != pstVPlayer->stRoleData.MiscInfo.ZJRongYanMisc.WID)
			{	
				continue;
			}
			
			iZJret  = zj_rongyan_mon_if_have_by_wid(pstEnv,pstMon->WID);
			if(iZJret > 0)
			{
				pstZJRongYan = &pstZJRongYanGlobal->ZJRongYan;
				flag =  0;

				if(0 == strcmp(pstZJRongYan->CallName,pstVPlayer->stRoleData.RoleName))
				{
					flag = 1;
				}
		
				llExp = player_get_exp_by_zj_rongyan(pstEnv,pstVPlayer->stRoleData.Level,pstZJRongYan->Level);
				if(llExp < 0)
				{
					continue;
				}

				if(flag == 1)
				{
					llExp = llExp * 3;
				}

				pstVPlayer->stOnline.cMiscFlag |= MISC_FLAG_EXPNODAYLIMIT;
				player_add_exp(pstEnv,pstVPlayer,llExp,0);
				pstVPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_EXPNODAYLIMIT;
			}
		}
	}

	return 0;
}

tdr_longlong player_get_exp_by_zj_rongyan(ZONESVRENV* pstEnv,int iPlayerLevel,int iZJRongYanLevel)
{
	tdr_longlong llExp = 0;
	LEVELUP *pstLevelUP = NULL;
	ZJRONGYANJUSHOUDEF  *pstZJRongYanJuShouDef = NULL;

	pstLevelUP = z_get_levelup(pstEnv, iPlayerLevel);
	if (NULL == pstLevelUP)
	{
		return -1;
	}

	pstZJRongYanJuShouDef = z_find_zjrongyanjushou_def(pstEnv,iZJRongYanLevel);
	if(!pstZJRongYanJuShouDef)
	{
		return -1;
	}

	llExp = (pstLevelUP->ContributeExp/10000.0)*375.0*(pstZJRongYanJuShouDef->ExpMul);

	return llExp;
}

int player_login_zj_rongyan_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if(is_fini_global(pstEnv) == 0)
	{
		return -1;
	}

	ZJRONGYANGLOBAL *pstZJRongYanGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.ZJRongYanGlobal;
	ZJRONGYAN *pstZJRongYan = &pstZJRongYanGlobal->ZJRongYan;
	
	CSZJRONGYANBOSSINFO stZJRongYanBossInfo;
	memset(&stZJRongYanBossInfo,0,sizeof(stZJRongYanBossInfo));

	if(0 != strcmp(pstZJRongYan->CallName,pstPlayer->stRoleData.RoleName))
	{
		zj_rongyan_close_dlg(pstEnv, pstPlayer);
		stZJRongYanBossInfo.Type = 1;
		stZJRongYanBossInfo.NextExpMul = 0;
		player_zj_rongyan_boss_select_ui(pstEnv, pstPlayer,&stZJRongYanBossInfo);
		return -1;
	}

	ZJRONGYANJUSHOUDEF  *pstZJRongYanJuShouDef = NULL;
	
	if(pstZJRongYan->Status == 1 || pstZJRongYan->Status == 2)
	{
		if(pstZJRongYan->Level >= ZJ_RONGYAN_FIRST_LEVEL && pstZJRongYan->Level <= ZJ_RONGYAN_TOP_LEVEL)
		{
			tdr_longlong llexp = 0;
			ZJRONGYANINFO stZJRongYanInfo;
			memset(&stZJRongYanInfo,0,sizeof(stZJRongYanInfo));
			
			pstZJRongYanJuShouDef = z_find_zjrongyanjushou_def(pstEnv,pstZJRongYan->Level);
			if(pstZJRongYanJuShouDef)
			{
				stZJRongYanInfo.ExpMul = pstZJRongYanJuShouDef->ExpMul;
				stZJRongYanInfo.Level = pstZJRongYanJuShouDef->MonLevel;
				stZJRongYanInfo.NextExpMul = pstZJRongYanJuShouDef->ExpMul*2;
			}

			llexp = player_get_exp_by_zj_rongyan(pstEnv,pstPlayer->stRoleData.Level,pstZJRongYan->Level);
			if(llexp <= 0)
			{	
				stZJRongYanInfo.CurExp = 0;
			}
			else
			{
				stZJRongYanInfo.CurExp = 3*llexp;
			}

			if((pstZJRongYan->CallTime != 0) && (pstZJRongYan->CallTime > pstEnv->pstAppCtx->stCurr.tv_sec))
			{
				stZJRongYanInfo.DaoJiShi = pstZJRongYan->CallTime - pstEnv->pstAppCtx->stCurr.tv_sec;
				stZJRongYanInfo.Type = 0;
			}
			else
			{
				stZJRongYanInfo.Type = 1;	
			}
	
			player_zj_rongyan_ui(pstEnv, pstPlayer,&stZJRongYanInfo);	
		}
	}
	else if(pstZJRongYan->Status == 4)
	{
		int iLevel = 0;
		iLevel = pstZJRongYan->Level;
		iLevel++;
		if((iLevel > ZJ_RONGYAN_FIRST_LEVEL) && (iLevel <= ZJ_RONGYAN_TOP_LEVEL))
		{
			pstZJRongYanJuShouDef = z_find_zjrongyanjushou_def(pstEnv,iLevel);
			if(pstZJRongYanJuShouDef)
			{
				stZJRongYanBossInfo.NextExpMul = pstZJRongYanJuShouDef->ExpMul;
			}
			stZJRongYanBossInfo.Type = 0;
			player_zj_rongyan_boss_select_ui(pstEnv, pstPlayer,&stZJRongYanBossInfo);
		}
	}
	else
	{
		zj_rongyan_close_dlg(pstEnv, pstPlayer);
		stZJRongYanBossInfo.Type = 1;
		stZJRongYanBossInfo.NextExpMul = 0;
		player_zj_rongyan_boss_select_ui(pstEnv, pstPlayer,&stZJRongYanBossInfo);
	}

	return 0;
}

int player_if_call_zj_rongyan(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if(is_fini_global(pstEnv) == 0 )
	{
		return -1;
	}

	ZJRONGYANGLOBAL *pstZJRongYanGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.ZJRongYanGlobal;
	ZJRONGYAN *pstZJRongYan = &pstZJRongYanGlobal->ZJRongYan;
	
	if(pstZJRongYan->Status == 3 || pstZJRongYan->Status == 0)
	{
		return -1;
	}
	
	if(0 == strcmp(pstZJRongYan->CallName,pstPlayer->stRoleData.RoleName))
	{
		return 1;
	}

	return -1;
}

int zj_rongyan_update_call_name(ZONESVRENV* pstEnv, const char* pszOldName, const char* pszNewName)
{
	if(is_fini_global(pstEnv) == 0)
	{
		return -1;
	}

	ZJRONGYANGLOBAL *pstZJRongYanGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.ZJRongYanGlobal;
	ZJRONGYAN *pstZJRongYan;
	
	pstZJRongYan = &pstZJRongYanGlobal->ZJRongYan;

	if(!pstZJRongYan->CallName)
	{
		return -1;
	}

	if(0 == strcmp(pstZJRongYan->CallName,pszOldName))	
	{
		STRNCPY(pstZJRongYan->CallName,pszNewName,sizeof(pstZJRongYan->CallName));
	}	

	return 0;
}

int zj_rongyan_close_dlg(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZJRONGYANINFO stZJRongYanInfo;
	memset(&stZJRongYanInfo,0,sizeof(stZJRongYanInfo));

	//2表示关闭倒数召唤提示窗口和等级奖励窗口
	stZJRongYanInfo.Type = 2;	
	player_zj_rongyan_ui(pstEnv, pstPlayer,&stZJRongYanInfo);
	return 0;
}

int player_zj_rongyan_boss(ZONESVRENV* pstEnv,Player* pstPlayer,CSZJRONGYANBOSSSELECT *pReq)
{
	if(is_fini_global(pstEnv) == 0)
	{
		return -1;
	}

	ZJRONGYANGLOBAL *pstZJRongYanGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.ZJRongYanGlobal;
	ZJRONGYAN *pstZJRongYan;
	int iLevel = 0;
	ZJRONGYANJUSHOUDEF	*pstZJRongYanJuShouDef = NULL;
	tdr_ulonglong WID = 0;
	ZONEIDX stIdx;
	int RandOdds;
	ZJRONGYANJUSHOUDEF *pstTempZJRongYanJuShouDef;
	int iMonId = 0;
	int tLifeTm = 0;
	int iFlag = 0;
	Player *pstPlayer2 = NULL;
	int iExist = 0;
	CSZJRONGYANBOSSINFO stZJRongYanBossInfo;

	pstZJRongYan = &pstZJRongYanGlobal->ZJRongYan;

	if(pstZJRongYan->Status != 4)
	{
		return -1;
	}

	iLevel = pstZJRongYan->Level;
	//1~8才有boss选择
	if(iLevel >= ZJ_RONGYAN_TOP_LEVEL || iLevel < ZJ_RONGYAN_FIRST_LEVEL)
	{
		return -1;
	}

	pstPlayer2 = z_name_player(pstEnv->pstAppCtx, pstEnv,pstZJRongYan->CallName);
	if(pstPlayer2 != NULL)
	{
		iExist = 1;
	}

	iLevel++;
	pstZJRongYanJuShouDef = z_find_zjrongyanjushou_def(pstEnv,iLevel);
	if(!pstZJRongYanJuShouDef)
	{
		return -1;
	}

	RandOdds = RAND1(10000);
	if(RandOdds < pstZJRongYanJuShouDef->Odds)
	{
		iMonId = pstZJRongYanJuShouDef->MonID;
		tLifeTm = pstZJRongYanJuShouDef->MonLifeTime;
		iFlag = 1;
	}
	else
	{
		pstTempZJRongYanJuShouDef = z_find_zjrongyanjushou_def(pstEnv,0);
		if(!pstTempZJRongYanJuShouDef)
		{
			return -1;
		}			

		//刷假怪
		iMonId = pstTempZJRongYanJuShouDef->MonID;
		tLifeTm = pstTempZJRongYanJuShouDef->MonLifeTime;
	}		

	stIdx.iID = pstZJRongYan->Map;
	stIdx.iPos = pstZJRongYan->iMapPos;
	if(0 == player_zj_rongyan_new_mon(pstEnv,tLifeTm,iMonId ,&pstZJRongYan->stPos,&WID,&stIdx))
	{
		pstZJRongYan->ZJRongYanWID = WID;
		pstZJRongYan->tMonLifeTime =  pstEnv->pstAppCtx->stCurr.tv_sec + tLifeTm;
		pstZJRongYan->Status = 2;
		pstZJRongYan->tBossSelectTm = 0;
		
		if(iFlag == 1)
		{
			pstZJRongYan->Level++;

			if(1 == iExist)
			{
				tdr_longlong llexp = 0;
				ZJRONGYANINFO stZJRongYanInfo;
				memset(&stZJRongYanInfo,0,sizeof(stZJRongYanInfo));
				
				llexp = player_get_exp_by_zj_rongyan(pstEnv,pstPlayer2->stRoleData.Level,pstZJRongYan->Level);
				if(llexp <= 0)
				{	
					stZJRongYanInfo.CurExp = 0;
				}
				else
				{
					stZJRongYanInfo.CurExp = 3*llexp;
				}

				stZJRongYanInfo.Type = 1;	
				stZJRongYanInfo.ExpMul = pstZJRongYanJuShouDef->ExpMul;
				stZJRongYanInfo.Level = pstZJRongYanJuShouDef->MonLevel;
				stZJRongYanInfo.NextExpMul = pstZJRongYanJuShouDef->ExpMul*2;

				player_zj_rongyan_ui(pstEnv, pstPlayer2,&stZJRongYanInfo);
			}
		}
		else
		{
			if(1 == iExist)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer2, SYS_MIDDLE2, ZONEERR_RONGYAN1);
				z_sys_strf_broadcast(pstEnv, SYS_SVR,ZONEERR_TEAM317,pstZJRongYan->CallName,pstZJRongYan->MapName,pstZJRongYan->Level);
				zj_rongyan_close_dlg(pstEnv, pstPlayer2);
			
				stZJRongYanBossInfo.Type = 1;
				stZJRongYanBossInfo.NextExpMul = 0;
				player_zj_rongyan_boss_select_ui(pstEnv, pstPlayer2,&stZJRongYanBossInfo);
			}
			pstZJRongYan->Level = 0;
		}
	}
	
	return 0;
}

int player_zj_rongyan_boss_select_ui(ZONESVRENV* pstEnv, Player *pstPlayer,CSZJRONGYANBOSSINFO *pstZJRongYanBossInfo)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	CSZJRONGYANBOSSINFO *pstZJRongYanBossInfoUI = &pstMiscSvr->PlayerMiscSvrData.ZJRongYanBossInfo;
	
	pstMiscSvr->PlayerMiscType = PLAYER_ZJ_RONGYAN_TRUE_FALSE_UI;
	memcpy(pstZJRongYanBossInfoUI ,pstZJRongYanBossInfo,sizeof(*pstZJRongYanBossInfoUI));

	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

//iType 如果为0表示所有，1表示需要同服才能收到
int span_map_broadcast(ZONESVRENV *pstEnv, Player *pstPlayer, CSPKG *pstPkg,int iType)
{
	int i;
	Player *pstElement = NULL;
	TMEMBLOCK *pstMemBlock;
	int iCount = 0;
	Player *apstPlayers[MAX_PLAYER];
	
	for (i=0; i<MAX_PLAYER; i++)
	{
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstElement = (Player *)pstMemBlock->szData;
		if (pstElement == NULL || PLAYER_STATUS_ROLE_LOGIN != pstElement->eStatus)
		{
			continue;
		}

		if(pstElement == pstPlayer)
		{
			continue;
		}

		if(0 > player_if_in_span_map(pstEnv,pstElement))
		{
			continue;
		}

		if(1 == iType)
		{
			if(pstPlayer->stRoleData.WorldID != pstElement->stRoleData.WorldID)
			{
				continue;
			}
		}

		apstPlayers[iCount++] = pstElement;	
	}

	if (iCount > 0)
	{
		return z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
		                                iCount, pstPkg);
	}

	return 0;
}

int player_sculpt_chg_clt(ZONESVRENV* pstEnv,Player* pstPlayer,CSSCULPTCHGCLT *pReq)
{
	int iSucc = 1;
	ITEMDEC stDec;
	ITEMDEF *pstItemDef;
	int iRet;
	CSSCULPTCHGSVR stSculptChgSvr;
	memset(&stSculptChgSvr,0,sizeof(stSculptChgSvr));
	int i;
	UinCache *pstUinCache;
	int iValid;

	if(pReq->Head > HEAD_MAX || pReq->Face > FACE_MAX || pReq->Hair > HAIR_MAX)
	{
		iSucc = 0;
		goto final;
	}

	pstItemDef = z_find_itemdef(pstEnv, SCULPT_CHG_ITEM_ID);
	if (NULL == pstItemDef)
	{
		iSucc = 0;
		goto final;
	}

	//以下是扣除道具------------begin----------------
	if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, SCULPT_CHG_ITEM_ID, 1, -1, NULL,PACKAGE_DEC_NONE))
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_MISC_306,pstItemDef->Name);
		iSucc = 0;
		goto final;
	}

	stDec.ID = SCULPT_CHG_ITEM_ID;
	stDec.Num = 1;
	stDec.BindNum = 0;
	
	iRet = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, SCULPT_CHG_ITEM_ID, 1, -1,
			NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_USE);
	if (iRet > 0)
		stDec.BindNum = iRet;
	
	if (iRet >= 0)
	{
		z_role_item_dec_oplog(pstEnv,pstPlayer, &stDec,OP_ITEM_DEC_USE);
	}
	//--------------------------扣除物品end  -----------------

	pstPlayer->stRoleData.Head = pReq->Head;
	pstPlayer->stRoleData.Face = pReq->Face;
	pstPlayer->stRoleData.Hair = pReq->Hair;

	sculpt_chg_action(pstEnv, pstPlayer);

	pstPlayer->stOnline.cDataDirty = DIRTY_IMPORTANT;

	pstUinCache = (UinCache *)sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos, &iValid);
	if (pstUinCache && iValid)
	{
		for (i=0; i<pstUinCache->ucRoleNum; i++)
		{
			if (pstUinCache->astRoleListInfo[i].RoleID == pstPlayer->stRoleData.RoleID)
			{
				pstUinCache->astRoleListInfo[i].Head = pstPlayer->stRoleData.Head;
				pstUinCache->astRoleListInfo[i].Face = pstPlayer->stRoleData.Face;
				pstUinCache->astRoleListInfo[i].Hair = pstPlayer->stRoleData.Hair;
				break;
			}
		}
	}

final:
    stSculptChgSvr.Res = iSucc;
	player_sculpt_chg_svr(pstEnv, pstPlayer,&stSculptChgSvr);
	return 0;
}

int player_sculpt_chg_svr(ZONESVRENV* pstEnv, Player *pstPlayer,CSSCULPTCHGSVR *pstSculptChgSvr)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	CSSCULPTCHGSVR *pstCSSculptChgSvr = &pstMiscSvr->PlayerMiscSvrData.SculptChgSvr;
	
	pstMiscSvr->PlayerMiscType = PLAYER_SCULPT_CHG_SVR;
	memcpy(pstCSSculptChgSvr,pstSculptChgSvr,sizeof(*pstCSSculptChgSvr));

	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int sculpt_chg_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	
	pstActionRes->ActionID = ACTION_SCULPT_CHG;

	pstActionRes->Data.SculptChg.Head = pstPlayer->stRoleData.Head;
	pstActionRes->Data.SculptChg.Face = pstPlayer->stRoleData.Face;
	pstActionRes->Data.SculptChg.Hair = pstPlayer->stRoleData.Hair;

	pstActionRes->ID = pstPlayer->iMemID;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	return 0;
}

int player_word_pwd_ui(ZONESVRENV* pstEnv, Player *pstPlayer,CSWORDPWDUI *pstWordPwdUI)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	CSWORDPWDUI *pstCSWordPwdUI = &pstMiscSvr->PlayerMiscSvrData.WordPwdUI;
	
	pstMiscSvr->PlayerMiscType = PLAYER_WORD_PWD_UI;
	memcpy(pstCSWordPwdUI,pstWordPwdUI,sizeof(*pstCSWordPwdUI));	

	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

WORDPWDDEF *z_find_word_pwd_def(ZONESVRENV* pstEnv, int iID)
{
	WORDPWDDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.ID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sWordPwdDef, pstObj->iWordPwdDef, WORD_PWD_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (WORDPWDDEF *)&pstObj->sWordPwdDef[iIndex];
	}
	
	return NULL;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(iID);	//add by paraunused.pl
}

int player_if_in_span_map(ZONESVRENV *pstEnv,Player *pstElement)
{
	MapIndex* pstMapIndex = NULL;
	ZONEMAPINST* pstMapInst = NULL;
	ZONEIDX stIdx;
	
	stIdx.iID = pstElement->stRoleData.Map;
	stIdx.iPos = pstElement->stOnline.iMapPos;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if(!pstMapInst)
	{
		return -1;
	}	

	pstMapIndex	= z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if( !pstMapIndex)
	{
		return -1;
	}
	
	if(!(pstMapIndex->stMapDef.CtrlFlag & MAP_CTRL_PVP_LOAD_MAP))
	{
		return -1;
	}

	return 0;
}

int player_enter_span_map(ZONESVRENV *pstEnv,Player *pstPlayer,int iOldMap,RESPOS* pstOldPos)
{
	int iMode;
	if(0 > player_if_in_span_map(pstEnv,pstPlayer))
	{
		return 0;
	}
	
	iMode = pstPlayer->stRoleData.MiscInfo.PkInfo.PkMode;
	player_pk_mode_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, PK_MODE_CAMP);
	pstPlayer->stRoleData.MiscInfo.PkInfo.SavePkMode = iMode;
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPAN_MAP1);

	pstPlayer->stRoleData.ExitMap = iOldMap;
	pstPlayer->stRoleData.ExitPos_x = pstOldPos->X;
	pstPlayer->stRoleData.ExitPos_y = pstOldPos->Y;
	
	return 0;
}

int player_enter_span_map_notify(ZONESVRENV* pstEnv, Player *pstPlayer,CSENTERSPANMAPNOTIFY *pstSpanMapNotify)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	CSENTERSPANMAPNOTIFY *pstNotify = &pstMiscSvr->PlayerMiscSvrData.EnterSpanMapNotify;
	
	pstMiscSvr->PlayerMiscType = PLAYER_ENTER_SPAN_MAP_NOTIFY;
	memcpy(pstNotify,pstSpanMapNotify,sizeof(*pstNotify));	

	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_enter_span_map_notify_msg(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	if(0 > player_if_in_span_map(pstEnv,pstPlayer))
	{
		return 0;
	}
	
	CSENTERSPANMAPNOTIFY stSpanMapNotify;
	memset(&stSpanMapNotify,0,sizeof(stSpanMapNotify));
	//跨服野外自己的阵营索引值
	stSpanMapNotify.CampIdx = pstPlayer->stOnline.stWarOL.cCampIdx;
	player_enter_span_map_notify(pstEnv, pstPlayer,&stSpanMapNotify);
	
	return 0;
}

//判断玩家是否有权能摸取宝箱
int player_baoxiang_if_have_limit_touch(ZONESVRENV* pstEnv,Player *pstPlayer,tdr_ulonglong WID,int iLevel)
{
	BAOXIANGLIMITINFO *pstBaoXiangLimitInfo = &pstPlayer->stRoleData.MiscInfo.BaoXiangLimitInfo;
	BXLIMITINFO *pstBXLimitInfo = NULL;
	int i = 0;
	int iNum = pstBaoXiangLimitInfo->LimitNum;
	int iMinLevel = 0;

	if(iNum > MAX_BXIANG_LIMIT_NUM || iNum < 0)
	{
		return -1;
	}

	for(i=0;i<iNum;i++)
	{
		pstBXLimitInfo = &pstBaoXiangLimitInfo->BXLimitInfo[i];

		if(pstBXLimitInfo->WID == WID)
		{
			iMinLevel = iLevel - BOSS_BAOXIANG_LIMIT_LEVEL;
			//boss掉落的宝箱，只有等级>=boss等级-10的玩家才能摸
			if(pstPlayer->stRoleData.Level >= iMinLevel)
			{
				return 1;
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONE_BOSS_BAOXING_1,iMinLevel);
				return -1;
			}
		}
	}

	return -1;
}

int player_baoxiang_touch_limit_set(ZONESVRENV* pstEnv,Player *pstPlayer,tdr_ulonglong WID)
{
	BAOXIANGLIMITINFO *pstBaoXiangLimitInfo = &pstPlayer->stRoleData.MiscInfo.BaoXiangLimitInfo;
	int iNum = pstBaoXiangLimitInfo->LimitNum;
	BXLIMITINFO stBXLimitInfo;
	memset(&stBXLimitInfo,0,sizeof(stBXLimitInfo));

	if(iNum > MAX_BXIANG_LIMIT_NUM || iNum < 0)
	{
		return -1;
	}

	//已经记录了的，不记
	if(0 < player_baoxiang_if_have_limit_touch(pstEnv,pstPlayer,WID,BOSS_BAOXIANG_LIMIT_LEVEL))
	{
		return -1;
	}
	
	stBXLimitInfo.WID = WID;
	if(pstBaoXiangLimitInfo->LimitNum >= MAX_BXIANG_LIMIT_NUM)
	{
		memmove(&pstBaoXiangLimitInfo->BXLimitInfo[0],&pstBaoXiangLimitInfo->BXLimitInfo[1],sizeof(pstBaoXiangLimitInfo->BXLimitInfo[0])*(MAX_BXIANG_LIMIT_NUM-1));
		memcpy(&pstBaoXiangLimitInfo->BXLimitInfo[MAX_BXIANG_LIMIT_NUM-1],&stBXLimitInfo,sizeof(pstBaoXiangLimitInfo->BXLimitInfo[MAX_BXIANG_LIMIT_NUM-1]));
	}
	else
	{
		memcpy(&pstBaoXiangLimitInfo->BXLimitInfo[pstBaoXiangLimitInfo->LimitNum],&stBXLimitInfo,sizeof(pstBaoXiangLimitInfo->BXLimitInfo[pstBaoXiangLimitInfo->LimitNum]));
		pstBaoXiangLimitInfo->LimitNum++;
	}

	return 1;
}

int z_add_tmpenemy(ZONESVRENV* pstEnv, Player *pstAtkee, ZoneAni *pstAtker)
{
	Player *pstAtkPlayer;
	MapIndex *pstMapIndex;
	ZONEPWORLD *pstZonePworld;

	if(!pstAtker || !pstAtkee)
	{
		return 0;
	}
	
	pstMapIndex = z_mappos_index(pstEnv->pstMapObj, NULL, pstAtkee->stRoleData.Map);
	if(NULL == pstMapIndex)
	{
		return 0;
	}
	
	//攻城战时期，攻城战地图杀人不添加
	if(pstMapIndex->stMapDef.MapID == WORLD_CITY_MAPID &&
		is_in_siege_city_fast(pstEnv))
	{
		return 0;
	}
	
	if(pstAtker->iType == OBJ_MONSTER && is_mon_machine(pstAtker->stObj.pstMon))
	{
		pstAtkPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAtker->stObj.pstMon->iSkillAtkerID);
	}
	else
	{
		pstAtkPlayer = get_atker_player(pstEnv,  pstAtker);
	}

	if(NULL == pstAtkPlayer || pstAtkee == pstAtkPlayer)
	{
		return 0;
	}

	//时间城不添加
	if(pstAtkee->stRoleData.Map == BASE_CITY_MAPID || pstAtkPlayer->stRoleData.Map == BASE_CITY_MAPID)
	{
		return 0;
	}

	//末世巅峰pk副本不添加
	if((GET_PWORLDID(pstAtkee->stRoleData.Map) == DIANFENG_PWORLD_ID) ||
		(GET_PWORLDID(pstAtkPlayer->stRoleData.Map) == DIANFENG_PWORLD_ID))
	{
		return 0;
	}

	//轰炸大战不添加
	if(player_is_in_tank_pworld(pstEnv, pstAtkPlayer)
		|| player_is_in_tank_pworld(pstEnv, pstAtkee))
	{
		return 0;
	}

	//切磋不添加
	if (z_is_qiecuo(pstEnv, pstAtkPlayer, pstAtkee))
	{
		return 0;
	}

	//天降横幅不添加仇人
	if (is_both_lucky_attendee(pstAtkee,pstAtkPlayer))
	{
		return 0;
	}

	//刺探活动不添加仇人
	if (is_both_clan_spy(pstEnv, pstAtkee,pstAtkPlayer))
	{
		return 0;
	}

	//在跨服野外不添加仇人
	if(0 == player_if_in_span_map(pstEnv,pstAtkee))
	{
		return 0;
	}

	//据点战和跨服据点战不添加
	if(strong_player_is_in_pworld(pstEnv, pstAtkee))
	{
		return 0;
	}

	//战场不添加
	pstZonePworld = z_find_pworld2(pstEnv, pstAtkee->stRoleData.Map);
	if(pstZonePworld && is_battle_pworld(pstZonePworld))
	{
		return 0;
	}

	//末世荣耀殿堂不添加
	if(pstZonePworld && pstZonePworld->stPworldDef.PolyType == PWORLD_POLY_SPARTA)
	{
		return 0;
	}

	// 通知客户端加仇人
	player_relation_addtmpenemy_req(pstEnv, pstAtkee, pstAtkPlayer->stRoleData.RoleName);

	return 0;
}

//跨服野外按阵营复活的坐标
int span_map_revive_pos(int iCamp,RESPOS *pstPos)
{
	assert(iCamp > 0 && iCamp <= MAX_SPAN_MAP_CAMP);

	RESPOS stPos;
	stPos.X = gszSpanMapRevivePos[iCamp-1][0];
	stPos.Y = gszSpanMapRevivePos[iCamp-1][1];

	*pstPos = stPos;
	return 0;
}

