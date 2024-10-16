/*
**  @file $RCSfile: zone_spansvr.c,v $
**  general description of this module
**  $Id: zone_spansvr.c,v 1.96 2012/12/13 03:21:31 eric Exp $
**  @author $Author: eric $
**  @date $Date: 2012/12/13 03:21:31 $
**  @version $Revision: 1.96 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#if 0
#include "zone_spansvr.h"
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
#include "zone_city.h"
#include "zone_vipbar.h"


// ��������RoleData����,�ⲿ�����ݲ�Ӧ�ñ�Battle_svr��RoleData���ݸ���
typedef struct tagProtectedRoleData
{
	char szRoleName[ROLE_NAME_LEN];
	tdr_ulonglong ullTeamID;
	int iTeamIdx;
	CLANINFO stClanInfo;
	unsigned char ucBlackNum;
    ROLEENEMYMEMBER astBlackMember[MAX_BLACK_NUM];
    ROLEBUDDY stBuddy;
    ROLEENEMY stEnemy;
}ProtectedRoleData;

/*
 * Battle Serverר�еĹ���
 */
static int WID2TeamHashCmp(const void *pv1 , const void *pv2);
static unsigned int WID2TeamHashCode(const void* pvData);
void role_save_protected_data(const ROLEDATA *pstRoleData,ProtectedRoleData *pstSavedData);
void role_restore_protected_data(const ProtectedRoleData *pstSavedData,ROLEDATA *pstRoleData);

static int WID2TeamHashCmp(const void *pv1 , const void *pv2)
{
	const WID2Team *pstLhs = pv1, *pstRhs = pv2;

	if(pstLhs->ullID > pstRhs->ullID )
	{
		return 1;
	}
	else if(pstLhs->ullID < pstRhs->ullID)
	{
		return -1;
	}
	
	return 0;
}

static unsigned int WID2TeamHashCode(const void* pvData)
{
	int i = 0;
	unsigned int nCode = 0;
	const WID2Team *pstElement = pvData;
	unsigned int anBuf[sizeof(pstElement->ullID)/sizeof(unsigned int)];

	memset(anBuf, 0, sizeof(anBuf));
	memcpy(anBuf, &pstElement->ullID, sizeof(pstElement->ullID));

	i = CCH(anBuf);
	while(i--)
	{
		nCode += anBuf[i];
	}
	return nCode;
}



//ս������ҵ�½
int player_battle_acc_login(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, SSBATTLEPLAYERENTERPWORLDREQ *pstBattlePlayerEnterPworldReq)
{
	int iUin=pstBattlePlayerEnterPworldReq->RoleData.Uin;
	Player *pstPlayerNew;
	UinCache *pstUinCache = NULL;
	UinCache stUinHashData;
	char szMsg[DESC_LEN];
	TFRAMEHEAD *pstFrameHead  = &pstBattlePlayerEnterPworldReq->Framehead;
	NameMemID stNameMemID, * pstNameMemID;
	unsigned short   unWorldID;
	ROLEDATA stRoleData;
	
	memset(&stNameMemID,0,sizeof(stNameMemID));
	
	//server�رչ����н�ֹ��¼
	if (pstEnv->cShutdown)
	{
		tlog_error(pstEnv->pstLogCat, iUin, 0, "player_battle_acc_login error system");
		return BATTLE_ERR_SYSTEM;
	}
	
	/*check login frequency limit*/
	stUinHashData.iUin = iUin;
	pstUinCache = (UinCache *)sht_find(pstEnv->pstShtUinCache,&stUinHashData,UinHashCmp,UinHashCode);
	/*check player whether have been login  before*/
	pstPlayerNew = player_get_by_uin(pstEnv, iUin);
	if( pstPlayerNew  )
	{
		if(pstPlayerNew->stRoleData.RoleID != pstBattlePlayerEnterPworldReq->RoleData.RoleID)
		{
			z_sys_strf( pstEnv, pstPlayerNew, SYS_MIDDLE2, "ս���Ѿ�����ͬ���ʺŵ�½�� %s", pstPlayerNew->szAccount);
			tlog_error(pstEnv->pstLogCat, iUin, 0, "player_battle_acc_login error the same acc");
			return BATTLE_ERR_ACC_MUL_ROLE;
		}
		else if( pstPlayerNew->eStatus != PLAYER_STATUS_ROLE_LOGIN )
		{
			
		}
		else
		{
			// �����ݻ���½����,���������ߵ�(��ҵ�����,���ܽ�ս������?)
			memset(szMsg,0,sizeof(szMsg));
			snprintf(szMsg, sizeof(szMsg),"������ip��ַΪ%s���û�ʹ�������ʺŵ�¼", 
					inet_ntoa(*(struct in_addr *)&pstFrameHead->ExtraInfo.IPInfo.ip));
			player_battle_now_kickoff(pstAppCtx, pstEnv, pstPlayerNew, szMsg);
			tlog_error(pstEnv->pstLogCat, iUin, 0, "player_battle_acc_login error,kickoff");
			return BATTLE_ERR_DELAY_LOGIN;
		}
	}
	else
	{
		/*alloc place for player at new connection*/
		if (pstEnv->pstPlayerPool->iUsed >= MAX_PLAYER_SOFT )
		{
			tlog_error(pstEnv->pstLogCat, iUin, 0, "player_battle_acc_login error iUsed is Max");
			return BATTLE_ERR_PALYER_UPLIMIT;
		}
		if(pstUinCache)
		{
			pstPlayerNew = player_alloc(pstAppCtx, pstEnv, iUin, pstBattlePlayerEnterPworldReq->AccountLoginName, pstUinCache);
		}
		else
		{
			pstPlayerNew = player_alloc(pstAppCtx, pstEnv, iUin, pstBattlePlayerEnterPworldReq->AccountLoginName, NULL);
		}
		if (NULL == pstPlayerNew)
		{
			tlog_error(pstEnv->pstLogCat, iUin, 0, "player_battle_acc_login error pstPlayerNew is NULL");
			return BATTLE_ERR_PALYER_UPLIMIT;
		}

		pstPlayerNew->stRoleData.Uin = pstBattlePlayerEnterPworldReq->RoleData.Uin;
	}

	
	// �����ʺŽ��Խ���ʱ��
	//pstPlayerNew->iAccSilenceEnd = pstFrameHead->CmdData.Start.AuthData.AuthJC.AccSilenceEnd;
	pstPlayerNew->iAccSilenceEnd = pstBattlePlayerEnterPworldReq->AccSilenceEnd;
	
	pstPlayerNew->unCltProVer = pstBattlePlayerEnterPworldReq->CltProtoVer;	

	pstPlayerNew->eStatus = PLAYER_STATUS_ACC_LOGIN;

	pstBattlePlayerEnterPworldReq->eStatusAssistant = pstBattlePlayerEnterPworldReq->eStatusAssistant%PLAYER_STATUS_BATTLE_TEAM_SYNC;
	pstPlayerNew->stOnline.eStatusAssistant |= pstBattlePlayerEnterPworldReq->eStatusAssistant;	
	if(pstPlayerNew->stOnline.stPworldQueue.stBattleCampInfo.ZoneBusID > 0 &&
		pstPlayerNew->stOnline.eStatusAssistant < PLAYER_STATUS_BATTLE_TEAM_SYNC)
	{
		player_do_statusassitant_sync(pstEnv,pstPlayerNew);
	}
	
	pstPlayerNew->stOnline.cMiscFlag = pstBattlePlayerEnterPworldReq->MiscFlag;
	pstPlayerNew->stOnline.ZoneMemID =  pstBattlePlayerEnterPworldReq->ZoneMemID;

	// �̳ǵ�����Ҫ�����·���		
	pstPlayerNew->tActive = pstAppCtx->stCurr.tv_sec;

	/*store fream head info to connect info*/
	pstPlayerNew->stConnectInfo.chVer = pstFrameHead->Ver;
	pstPlayerNew->stConnectInfo.chCmd = TFRAMEHEAD_CMD_START;
	pstPlayerNew->stConnectInfo.chExtraType = pstFrameHead->ExtraType;
	memcpy(&pstPlayerNew->stConnectInfo.stExtraInfo, &pstFrameHead->ExtraInfo, sizeof(pstPlayerNew->stConnectInfo.stExtraInfo));
	pstPlayerNew->stConnectInfo.stExtraInfo.IPInfo.ip = pstFrameHead->ExtraInfo.IPInfo.ip;
	
	pstPlayerNew->stConnectInfo.chTimeStampType = pstFrameHead->TimeStampType;
	memcpy(&pstPlayerNew->stConnectInfo.stTimeStamp, &pstFrameHead->TimeStamp, sizeof(pstPlayerNew->stConnectInfo.stTimeStamp));
	pstPlayerNew->stConnectInfo.iConnIdx = pstFrameHead->ConnIdx;
	pstPlayerNew->stConnectInfo.iID = pstPlayerNew->iIdx;
	pstPlayerNew->stOnline.iZoneConnectID = pstBattlePlayerEnterPworldReq->ConnectID;
	pstPlayerNew->GMLimitPerm = pstBattlePlayerEnterPworldReq->GMLimitPerm;

	pstPlayerNew->stOnline.stVipBarInfo = pstBattlePlayerEnterPworldReq->VipBarInfo;
	
	/*tlog_info(pstEnv->pstLogCat, 0, 0, "acc sync Uin=%d AccName=%s,ConnIdx=%d,iIdx=%d,connid=%d,reqconnid=%d,status=%d", 
				pstPlayerNew->stRoleData.Uin, pstPlayerNew->szAccount,pstFrameHead->ConnIdx,
				pstPlayerNew->iIdx,pstPlayerNew->stConnectInfo.iID,
				pstBattlePlayerEnterPworldReq->ConnectID,
				pstPlayerNew->stOnline.eStatusAssistant);*/
	
	pstPlayerNew->stOnline.bConnLost = 0;
	pstPlayerNew->UinFlag = pstFrameHead->CmdData.Start.AuthData.AuthJC.UinFlag;
		
	pstPlayerNew->stConnectInfo.chCmd = TFRAMEHEAD_CMD_INPROC;

	//������
	pstPlayerNew->bAasFlag = pstBattlePlayerEnterPworldReq->AasInfo.AasFlag;
	pstPlayerNew->bAasIncomeLimt = 0;
	pstPlayerNew->stAasTime.tOnline = pstBattlePlayerEnterPworldReq->AasInfo.OnlineTime;
	pstPlayerNew->stAasTime.tOffline = pstBattlePlayerEnterPworldReq->AasInfo.OffTime;
	pstPlayerNew->stAasTime.tNotify = 0;
	pstPlayerNew->stAasTime.tActive = pstAppCtx->stCurr.tv_sec;
	
	// ��ɫ����
	memcpy(&stRoleData,&pstPlayerNew->stRoleData,sizeof(pstPlayerNew->stRoleData));
	memcpy(&pstPlayerNew->stRoleData,&pstBattlePlayerEnterPworldReq->RoleData,sizeof(pstPlayerNew->stRoleData));
	if( stRoleData.MiscInfo.TeamID> 0)
	{
		pstPlayerNew->stRoleData.MiscInfo.TeamID = stRoleData.MiscInfo.TeamID;
		pstPlayerNew->stRoleData.MiscInfo.TeamIdx = stRoleData.MiscInfo.TeamIdx;
	}
	//���½�ɫ�����ս��������
	pstPlayerNew->stOnline.stPworldQueue.stBattleCampInfo = pstBattlePlayerEnterPworldReq->BattleCampInfo;
	// ��ʱ��������
	memcpy(&pstPlayerNew->stOnline.stConnectTime,&pstFrameHead->TimeStamp.TimeVal, 
		sizeof(pstPlayerNew->stOnline.stConnectTime));

	if(!(pstBattlePlayerEnterPworldReq->eStatusAssistant&PLAYER_STATUS_BATTLE_LOGIN_ON))
	{
		PlayerSyncOnline stSyncInfo;
		
		memcpy(&stSyncInfo,
			pstBattlePlayerEnterPworldReq->ExtData,sizeof(stSyncInfo));
		pstPlayerNew->stOnline.stDyn = stSyncInfo.stDyn;
		
	}
	else
	{
		PlayerSyncOnline stSyncInfo;

		if(pstBattlePlayerEnterPworldReq->ExtLen == sizeof(stSyncInfo))
		{
			memcpy(&stSyncInfo, pstBattlePlayerEnterPworldReq->ExtData, sizeof(stSyncInfo));
		
			memset(&pstPlayerNew->stOnline.stDyn,0,sizeof(DYNATTRLIST));
			pstPlayerNew->stOnline.stFairyOnline = stSyncInfo.stFairyOnline;
			pstPlayerNew->stOnline.stHeti = stSyncInfo.stHeti;
			pstPlayerNew->stOnline.stPetOnline = stSyncInfo.stPetOnline;
			pstPlayerNew->stOnline.cMiscFlag = stSyncInfo.ullMiscFlag;
			pstPlayerNew->stOnline.cSafeLockFalg = stSyncInfo.cSafeLockFalg;
		}
	}

	if( pstEnv->iInstMode == INST_MODE_BATTLE_SVR )
	{
		// �����޸�����ҵ�����
		stNameMemID.iID = pstPlayerNew->iMemID;
		GET_WORLD_ID(unWorldID,pstPlayerNew->stOnline.stPworldQueue.stBattleCampInfo.ZoneBusID);
		
		snprintf(stNameMemID.szName,sizeof(stNameMemID.szName),"��%d"BATTLE_NAME_SYMBOL"%s",
					unWorldID,
					pstPlayerNew->stRoleData.RoleName);
		memset(pstPlayerNew->stRoleData.RoleName,0,sizeof(pstPlayerNew->stRoleData.RoleName));
		STRNCPY( pstPlayerNew->stRoleData.RoleName, stNameMemID.szName, sizeof(stNameMemID.szName));	
		pstNameMemID = sht_find(pstEnv->pstShtNameMemID, &stNameMemID, NameMemIDHashCmp, NameMemIDHashCode);
		if (pstNameMemID)
		{
			memcpy(pstNameMemID, &stNameMemID, sizeof(*pstNameMemID));
		}
		else
		{
			pstNameMemID = sht_insert_unique(pstEnv->pstShtNameMemID, &stNameMemID, NameMemIDHashCmp, NameMemIDHashCode);
			if (pstNameMemID)
			{
				memcpy(pstNameMemID, &stNameMemID, sizeof(*pstNameMemID));
			}
			else
			{
				tlog_error(pstEnv->pstLogCat, pstPlayerNew->stRoleData.Uin, 0, "sht insert name %d player fail", 
						pstPlayerNew->stRoleData.Uin);
			}
		}

		if(is_battle_pve_autoteam_svr(pstEnv))
		{
			if(0!=player_team_activation_player(pstEnv, pstPlayerNew,pstBattlePlayerEnterPworldReq))
			{
				return BATTLE_ERR_NO_TEAM;
			}
		}
	}

	// ��uin�Ľ�ɫ���ݳ�ʼ����
	player_battle_uin_role_cache_info(pstAppCtx, pstEnv, pstPlayerNew);

	
		
	return 0;
}
int player_battle_role_login(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iCameraDir, int iDir)
{
	int iOldMap = pstPlayer->stRoleData.Map;
	int iRet = 0;
	ZoneTeam *pstZoneTeam = NULL;
	SSBATTLECAMPINFO *pstBattleCampInfo = &pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;
	
	pstPlayer->stOnline.bDir = (unsigned char)iDir;
	// ���ص����ݶ�Ҫ�������״̬,��Ϊ���ߵ����ݶ���һ��
	//pstPlayer->stOnline.cMiscFlag &= ~ MISC_FLAG_SEND_SHOPDEF;
	pstPlayer->stOnline.cMiscFlag &= ~ MISC_FLAG_PACKAGE_ALL;
	pstPlayer->tActive =  pstAppCtx->stCurr.tv_sec;

	// ��uin�Ľ�ɫ���ݳ�ʼ����
	player_battle_uin_role_cache_info(pstAppCtx, pstEnv, pstPlayer);
	
	if( pstEnv->iInstMode == INST_MODE_BATTLE_SVR )
	{
		
	}
	else if( pstEnv->iInstMode == INST_MODE_ZONE_SVR )
	{
		pstPlayer->stRoleData.Map = pstPlayer->stRoleData.ExitMap;
		pstPlayer->stRoleData.Pos.X = pstPlayer->stRoleData.ExitPos_x;
		pstPlayer->stRoleData.Pos.Y = pstPlayer->stRoleData.ExitPos_y;
		//��������Ӫ����
		memset (&pstPlayer->stOnline.stWarOL, 0, sizeof(pstPlayer->stOnline.stWarOL));
		//memset (&pstPlayer->stOnline.stPworldQueue, 0, sizeof(pstPlayer->stOnline.stPworldQueue));
		z_del_battle_player_num(pstEnv, pstBattleCampInfo->BattleBusID, pstBattleCampInfo->PworldWID);
	}
	else 
	{
		return BATTLE_ERR_SYSTEM;
	}
	
	iRet = z_init_role(pstAppCtx, pstEnv, pstPlayer,1);
	if( iRet != 0 )
	{
		return BATTLE_ERR_SYSTEM;
	}
	
	// ���������Ϣ
	z_skill_online(pstAppCtx, pstEnv, pstPlayer);

	// ����״̬��Ϣ
	z_status_online(pstAppCtx, pstEnv, pstPlayer,0);

	if (z_player_find_status(pstPlayer, STATUS_ID_QIANXIN))
	{
		pstPlayer->stOnline.State |= CS_STAT_QIANXIN;
	}
	
	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode  )
	{
		if( z_role_login_notify( pstAppCtx, pstEnv, pstPlayer, iCameraDir,ROLE_LOGIN_FROM_WORLD_TO_BATTLE ) < 0 )
		{
			return BATTLE_ERR_SYSTEM;
		}
		z_ploy_info_chg(pstEnv, pstPlayer, SELF_RES_CHG);
	}
	else if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
	{
		if( pstEnv->cShutdown > 1)
		{
			//ϵͳ�ر���,���·��û�entermap
			return 0;
		}
		else if( z_role_login_notify( pstAppCtx, pstEnv, pstPlayer, iCameraDir,ROLE_LOGIN_FROM_BATTLE_TO_WORLD ) < 0 )
		{
			return BATTLE_ERR_SYSTEM;
		}
	}
	else
	{
		return BATTLE_ERR_SYSTEM;
	}
		
	//�����ֱ�Ӹ��
	if (pstPlayer->stRoleData.MiscInfo.AutoFlyPath > 0)
	{
		pstPlayer->stRoleData.MiscInfo.AutoFlyPath = 0;
		pstPlayer->stOnline.State &= ~CS_STAT_AUTOFLY;
	}
	
	pstPlayer->eStatus = PLAYER_STATUS_ROLE_LOGIN;
	tlog_info(pstEnv->pstLogCat, 0, 0, "Status=RoleLogin Uin=%d AccName=%s RoleName=%s", 
			pstPlayer->stRoleData.Uin, pstPlayer->szAccount, pstPlayer->stRoleData.RoleName);
	
	pstPlayer->stOnline.cCheckCltTimeFlag = 1;

	player_package_info(pstEnv, pstPlayer);

	if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
	{
		if(is_battle_pve_autoteam_svr(pstEnv))
		{
			if(0 != autoteam_op_trans(pstEnv, pstPlayer, pstBattleCampInfo->PworldDefID))
			{
				//z_send_sys_str(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, "�Զ���Ӵ���ʧ��,�������Զ����");
				//autoteam_notify_clt_clean(pstEnv, pstPlayer);
				return BATTLE_ERR_SYSTEM;
			}
		}
		else if(is_battle_pve_battle_svr(pstEnv))
		{
			if(0 != z_player_enter_pworld_inst(pstEnv, pstPlayer, pstBattleCampInfo->PworldDefID))
			{
				return BATTLE_ERR_SYSTEM;
			}	
		}
		else if(is_battle_pvp_svr(pstEnv))
		{
			if(0 > player_enter_battle_pvp_map(pstEnv, pstPlayer))
			{
				return BATTLE_ERR_SYSTEM;
			}
		}
		else
		{
			return BATTLE_ERR_SYSTEM;
		}
	}
	else
	{
		if (0 > z_player_entermap(pstAppCtx, pstEnv, pstPlayer, pstPlayer->stRoleData.Map, &pstPlayer->stRoleData.Pos, 1))
		{ 
			return BATTLE_ERR_SYSTEM;
		}
	}

	tmp_item_check(pstEnv, pstPlayer, iOldMap);

	if (iOldMap != pstPlayer->stRoleData.Map)
	{
		player_package_check_mapitem(pstEnv, pstPlayer);
	}

	if ( (pstPlayer->stOnline.State & CS_STAT_FLY) ||  (pstPlayer->stOnline.State & CS_STAT_RIDE_ON) )
	{
		z_player_ride_action(pstEnv, pstPlayer, 1);
	}

	task_check_trace(pstEnv, pstPlayer);

	z_role_svr_status(pstEnv, pstPlayer);

	//Ұ���ս���������ᵼ��TeamID = 0��Ӧ��ֻ���Զ������Ҫ
	if(is_battle_pve_autoteam_svr(pstEnv))
	{
		//���һ�¶���
		pstZoneTeam = player_team_get(pstEnv, pstPlayer);
		if(pstZoneTeam)
		{
			player_team_sync(pstEnv, pstZoneTeam);
			player_team_all_action(pstEnv, pstZoneTeam);
		}
	}
	
	/*tlog_info(pstEnv->pstLogCat, 0, 0, "watch out clan [%s][%s][%llu][%d]",
	          pstPlayer->stRoleData.RoleName,
	          pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Name,
	          pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID,
	          pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id);*/

	// ��ս��״̬�ĵ� 
	SET_PLAYER_NOFIGHT(pstPlayer);
	z_send_player_fight(pstEnv, pstPlayer, CLEAN_FIGHT_STATUS);

	if(INST_MODE_ZONE_SVR == pstEnv->iInstMode)
	{
		player_battle_hello2battle_req(pstAppCtx, pstEnv, &pstPlayer->stOnline.stFrameHead,pstPlayer);
		pworld_limit_info_get(pstEnv, pstPlayer);
	}
	else if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
	{
		
		SSPKG stSSPkg;
		SSCMDREQ* pstCmdReq = &stSSPkg.Body.CmdReq;
		int iBusID;
		
		iBusID = pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.ZoneBusID ;
		
		memset(pstCmdReq,0,sizeof(*pstCmdReq));
		pstCmdReq->Cmd = SS_CMD_BATTLE_MEMID_NOTICE;
		STRNCPY(pstCmdReq->szName, pstPlayer->stRoleData.RoleName, sizeof(pstCmdReq->szName));
		battle_rolename_remove_prefix(pstCmdReq->szName);
		STRNCPY(pstCmdReq->szReceiver, pstPlayer->stRoleData.RoleName, sizeof(pstCmdReq->szReceiver));

		pstCmdReq->Data.BattleMemidNotice.BattleMemID = pstPlayer->iMemID;
		
		Z_SSHEAD_INIT(&stSSPkg.Head, SS_CMD_REQ);
		z_ssmsg_send(pstEnv, &stSSPkg, iBusID);
		
	}

	//vip ����
	//player_vip_bar_check(pstEnv,pstPlayer);
	
	return 0;
}

// ���״̬��check�����ڵ��ú���ǰ��ʵ
// zone_svr����
int player_battle_enter_check(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,Player *pstPlayer)
{
	if( PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus &&
		PLAYER_STATUS_ROLE_ON_BATTLE != pstPlayer->eStatus )
	{
		z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "�ǵ�½̬���ܽ�����������");
		return -1;
	}

	if (pstPlayer->stOnline.State & CS_STAT_GAOJI_SIT  ||
		pstPlayer->stOnline.State & CS_STAT_SIT  )
	{
		player_sit_break(pstAppCtx,pstEnv,pstPlayer);        
	} 

	if( pstPlayer->stRoleData.Map > PWORLD_MAPID_BASE )
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					"�����в����Խ�����������");
		return -1;
	}

	if (pstPlayer->stOnline.stExchgOnline.bExchgStat > 0)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					"����״̬�����Խ�����������");
		return -1;
	}
	
	// �����������
	player_weixing_clear(pstEnv, pstPlayer);

	//�������ϵó˿�
	if (pstPlayer->stOnline.stRideOnInfo.iPassengerNum && 
		(pstPlayer->stOnline.State & (CS_STAT_RIDE_ON | CS_STAT_FLY)))
	{
		int i;
		Player *pstTmpPlayer = NULL;

		for(i = 0; i<pstPlayer->stOnline.stRideOnInfo.iPassengerNum; i++)
		{
			pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stRideOnInfo.PassengerID[i]);
			if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				continue;
			}

			//��������
			z_hitch_ride_clear(pstEnv, pstTmpPlayer);
		}
	}
	//��˵�����
	else if (pstPlayer->stOnline.State & CS_STAT_HITCH_RIDE)
	{
		z_ride_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}
	
	z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
	
	if (pstPlayer->stOnline.State & CS_STAT_DEAD ||
		pstPlayer->stOnline.State & CS_STAT_ATK   ||
		pstPlayer->stOnline.State & CS_STAT_MOVE_ATK ||
		pstPlayer->stOnline.State & CS_STAT_PREPARE ||
		pstPlayer->stOnline.State & CS_STAT_MOVE_PREPARE ||
		pstPlayer->stOnline.State & CS_STAT_FIGHT ||
		pstPlayer->stOnline.State & CS_STAT_COLLECT||
		pstPlayer->stOnline.State & CS_STAT_FLY ||
		pstPlayer->stOnline.State & CS_STAT_RIDE_PREPARE ||  
		//pstPlayer->stOnline.State & CS_STAT_RIDE_ON || 
		pstPlayer->stOnline.State & CS_STAT_STALL ||      
		pstPlayer->stOnline.State & CS_STAT_CAMPAIGN_PRE ||  
		pstPlayer->stOnline.State & CS_STAT_HITCH_RIDE ||
		pstPlayer->stOnline.State &CS_STAT_FAIRY_CAMPAIGN_PRE
	)
	{
		z_send_sys_str(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "��ǰ״̬���ܽ�����������");
		return -1;
	} 

	pstPlayer->stOnline.cDialogStatOld = pstPlayer->stOnline.cDialogStat;
	pstPlayer->stOnline.cDialogStat = DIALOG_STAT_BEGIN;
	pstPlayer->stOnline.State &= ~CS_STAT_DIALOG;

	if(is_player_ride_on_machine(pstPlayer))
	{
		player_ride_machine_brk(pstEnv, pstPlayer, 1);
	}

	return 0;   
}

int player_battle_exit_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,Player *pstPlayer)
{
	SSPKG stPkg;
	SSBATTLEEXITREQ *pstBattleExitReq = &stPkg.Body.BattleExitReq;
	SSBATTLECAMPINFO *pstBattleCampInfo = &pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;

	if( !(pstBattleCampInfo->BattleBusID > 0 &&
		(pstPlayer->stOnline.eStatusAssistant&PLAYER_STATUS_BATTLE_LOGIN_READY) &&
		(pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN ||
		pstPlayer->eStatus == PLAYER_STATUS_ROLE_LIST)) )
	{
		return 0;
	}

	STRNCPY(pstBattleExitReq->AccountLoginName, pstPlayer->szAccount, 
			CCH(pstBattleExitReq->AccountLoginName));
	pstBattleExitReq->BattleCampInfo = *pstBattleCampInfo;
	pstBattleExitReq->RoleID = pstPlayer->stRoleData.RoleID;
	pstBattleExitReq->Uin = pstPlayer->stRoleData.Uin;
	STRNCPY(pstBattleExitReq->RoleName, pstPlayer->stRoleData.RoleName, CCH(pstBattleExitReq->RoleName));
	
	Z_SSHEAD_INIT(&stPkg.Head, BATTLE_EXIT_REQ);
	z_ssmsg_send(pstEnv, &stPkg, pstBattleCampInfo->BattleBusID);

	//������������
	pstPlayer->stOnline.eStatusAssistant = 0;
	player_do_statusassitant_sync(pstEnv,pstPlayer);
	memset(pstBattleCampInfo,0,sizeof(*pstBattleCampInfo));
	return 0;
	UNUSED(pstAppCtx);
	
}

//iOperType 0��ʾЭ����������,1��ʾ�����ڲ�����,��ֹautoteam_reject��ѭ������
int player_battle_exit(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,int Uin,int iOperType)
{
	Player *pstPlayer = NULL;
	SSPKG stPkg;
	SSBATTLECAMPINFO *pststBattleCampInfo;
	SSBATTLEEXITRES *pstBattleExitRes = &stPkg.Body.BattleExitRes;
	
	pstPlayer = player_get_by_uin(pstEnv, Uin);
	if( pstPlayer == NULL )
	{
		return -1;
	}
	pststBattleCampInfo = &pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;


	
	autoteam_exit_trans(pstEnv, pstPlayer);

	z_exit_pworld_queue(pstEnv, pstPlayer, pstPlayer->stOnline.stPworldQueue.iPworldID, 1);
	
	//ȡ��pvp�����
	memset(pstBattleExitRes,0,sizeof(SSBATTLEEXITRES));
	pstBattleExitRes->Result = 0;
	pstBattleExitRes->RoleID = pstPlayer->stRoleData.RoleID;
	pstBattleExitRes->Uin = pstPlayer->stRoleData.Uin;
	
	
	Z_SSHEAD_INIT(&stPkg.Head, BATTLE_EXIT_RES);
	z_ssmsg_send(pstEnv, &stPkg, pststBattleCampInfo->ZoneBusID);
	return 0;
	UNUSED(pstAppCtx);
	UNUSED(&iOperType);
}

int player_battle_exit_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,SSBATTLEEXITRES *pstBattleExitRes )
{
	Player *pstPlayer = NULL;
	SSBATTLECAMPINFO *pstBattleCampInfo = NULL;
	
	pstPlayer = player_get_by_uin(pstEnv, pstBattleExitRes->Uin);
	if( pstPlayer == NULL )
	{
		return -1;
	}

	if(pstBattleExitRes->Result == BATTLE_ERR_EXIT_TEAM)
	{
		player_team_exit(pstEnv, pstPlayer,NULL);
		return 0;
	}

	
	pstBattleCampInfo = &pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;

	if(pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN &&
		(pstPlayer->stOnline.eStatusAssistant&PLAYER_STATUS_BATTLE_LOGIN_READY))
	{
		//ս��,�Զ���������Ϣ����
		pstPlayer->stOnline.eStatusAssistant = 0;
		player_do_statusassitant_sync(pstEnv,pstPlayer);
		memset(pstBattleCampInfo,0,sizeof(SSBATTLECAMPINFO));
	}

	//vip bar ��״̬�ͻ��˻ظ���ʾ
	//if (pstPlayer->stOnline.stVipBarInfo.iVipBarID)
	//{
		//player_vip_bar_buff_chg_num(pstEnv,pstPlayer,FAKE_STATUS_OP_ADD,0);
	//}
	
	return 0;
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
}

int player_battle_enter_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,TFRAMEHEAD *pstFrameHead,
				Player *pstPlayer,int eStatusAssistant,int iBattleBusID)
{
	int iRet = 0;
	SSPKG stPkg;
	SSBATTLEPLAYERENTERPWORLDREQ *pstBattlePlayerEnterPworldReq = &stPkg.Body.BattlePlayerEnterPworldReq;
	SSBATTLECAMPINFO *pstBattleCampInfo = &pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;

	if( eStatusAssistant&PLAYER_STATUS_BATTLE_LOGIN_ON )
	{
		if (pstPlayer->stOnline.cMoving) 
		{
			pstPlayer->stOnline.cMoving = 0;
			z_player_move_stop(pstAppCtx, pstEnv,  pstPlayer);
		}

		iRet = player_battle_enter_check(pstAppCtx,pstEnv,pstPlayer);
		if( iRet != 0 )
		{
			return -1;
		}
	}
	if(!pstEnv->pstConf->BattleFlag )
	{
		pstPlayer->stRoleData.MiscInfo.SpanSvrTeamID = 0;
		pstPlayer->stRoleData.MiscInfo.SpanSvrTeamIdx= 0;
		pstBattleCampInfo->BattleBusID = 0;
		pstBattleCampInfo->ZoneBusID = 0;
		pstPlayer->stOnline.eStatusAssistant = 0;
	}
	if(pstPlayer->eStatus == PLAYER_STATUS_ROLE_ON_BATTLE )
	{
		return 0;
	}
	
	if( pstBattleCampInfo->BattleBusID > 0 &&
		pstBattleCampInfo->BattleBusID != iBattleBusID)
	{
		//��½����ͬ����Ҫ��֪�˵�
		player_battle_exit_req(pstAppCtx,pstEnv,pstPlayer);
	}
	
	//��ֵ�����Ϣ 
	pstBattleCampInfo->BattleBusID = iBattleBusID;
	pstBattleCampInfo->ZoneBusID = pstEnv->pstAppCtx->iId;
	pstBattleCampInfo->ZoneConnID = pstEnv->iZoneConnID;
	
	// ��ʼ����
	memset(pstBattlePlayerEnterPworldReq,0,sizeof(SSBATTLEPLAYERENTERPWORLDREQ));

	// ��¼����ҵĽ�ս��ʱ�ĵ�ͼ������
	if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE && !(pstPlayer->stOnline.State & CS_STAT_AUTOFLY))
	{
		pstPlayer->stRoleData.ExitMap = pstPlayer->stRoleData.Map;
		pstPlayer->stRoleData.ExitPos_x = pstPlayer->stRoleData.Pos.X ;
		pstPlayer->stRoleData.ExitPos_y = pstPlayer->stRoleData.Pos.Y;
		player_exit_info_check(pstEnv, pstPlayer);
	}



	// �Ȱ����ݻ�д��,ʹroledata����Ϊ����	
	if( (eStatusAssistant&PLAYER_STATUS_BATTLE_LOGIN_ON) &&
		pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN )
	{
		//״̬���
		if(pstPlayer->stOnline.cMiscFlag & MISC_FLAG_STATUS_CHGMAP_CLEAN)
		{
			pstPlayer->stOnline.iOldChangeMapID = pstPlayer->stRoleData.Map;
			Player_clean_matching_status(pstEnv->pstAppCtx, pstEnv, pstPlayer,STATUS_CHG_MAP_CLEAN,MISC_FLAG_STATUS_CHGMAP_CLEAN);
			pstPlayer->stOnline.iOldChangeMapID = 0;
		}
		//vip bar ��״̬�ͻ��˲���ʾ
		//if (pstPlayer->stOnline.stVipBarInfo.iVipBarID)
		//{
			//player_vip_bar_buff_chg_num(pstEnv,pstPlayer,FAKE_STATUS_OP_DEL,0);
		//}

		pstBattlePlayerEnterPworldReq->VipBarInfo = pstPlayer->stOnline.stVipBarInfo;
		
		z_db_role_update( pstAppCtx,  pstEnv, pstPlayer,0);
	}

	pstFrameHead->TimeStamp.TimeVal = pstPlayer->stOnline.stConnectTime;
	
	pstBattlePlayerEnterPworldReq->BattleCampInfo = pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;
	memcpy(&pstBattlePlayerEnterPworldReq->RoleData,&pstPlayer->stRoleData,sizeof(pstPlayer->stRoleData));
	memcpy(&pstBattlePlayerEnterPworldReq->Framehead,pstFrameHead,sizeof(TFRAMEHEAD));
	pstBattlePlayerEnterPworldReq->Framehead.ExtraType = TFRAMEHEAD_EXTRA_IP;
	pstBattlePlayerEnterPworldReq->Framehead.ExtraInfo = pstPlayer->stConnectInfo.stExtraInfo;
		
	pstBattlePlayerEnterPworldReq->CltProtoVer = pstPlayer->unCltProVer;
	STRNCPY(pstBattlePlayerEnterPworldReq->AccountLoginName, pstPlayer->szAccount, 
			CCH(pstBattlePlayerEnterPworldReq->AccountLoginName));
	pstBattlePlayerEnterPworldReq->MiscFlag = pstPlayer->stOnline.cMiscFlag;
	pstBattlePlayerEnterPworldReq->ZoneMemID = pstPlayer->iMemID;
	pstBattlePlayerEnterPworldReq->GMLimitPerm = pstPlayer->GMLimitPerm;

	//��������Ϣ
	pstBattlePlayerEnterPworldReq->AasInfo.AasFlag = pstPlayer->bAasFlag;
	pstBattlePlayerEnterPworldReq->AasInfo.OnlineTime = pstPlayer->stAasTime.tOnline;
	pstBattlePlayerEnterPworldReq->AasInfo.OffTime = pstPlayer->stAasTime.tOffline;

	//��״̬����ȥ
	pstPlayer->stOnline.eStatusAssistant |= eStatusAssistant;
	//׼���͵�¼����״̬���⣬��ֹͬʱ����
	if(eStatusAssistant&PLAYER_STATUS_BATTLE_LOGIN_ON )
	{
		pstPlayer->stOnline.eStatusAssistant |= eStatusAssistant;
		pstPlayer->stOnline.eStatusAssistant &= ~PLAYER_STATUS_BATTLE_LOGIN_READY;
	}
	else if(eStatusAssistant&PLAYER_STATUS_BATTLE_LOGIN_READY )
	{
		pstPlayer->stOnline.eStatusAssistant &= ~PLAYER_STATUS_BATTLE_LOGIN_ON;
	}
	pstBattlePlayerEnterPworldReq->eStatusAssistant = pstPlayer->stOnline.eStatusAssistant;
	pstBattlePlayerEnterPworldReq->ConnectID = pstPlayer->stConnectInfo.iID;

	//����
	pstBattlePlayerEnterPworldReq->AccSilenceEnd = pstPlayer->iAccSilenceEnd;

	//��Ҫ�õ���online��Ϣ
	{
		PlayerSyncOnline stSyncInfo;
		
		stSyncInfo.stDyn = pstPlayer->stOnline.stDyn;
		stSyncInfo.stFairyOnline = pstPlayer->stOnline.stFairyOnline;
		stSyncInfo.stHeti = pstPlayer->stOnline.stHeti;
		stSyncInfo.stPetOnline = pstPlayer->stOnline.stPetOnline;
		stSyncInfo.ullMiscFlag = pstPlayer->stOnline.cMiscFlag;
		stSyncInfo.cSafeLockFalg = pstPlayer->stOnline.cSafeLockFalg;

		memcpy(pstBattlePlayerEnterPworldReq->ExtData, &stSyncInfo, sizeof(stSyncInfo));
		pstBattlePlayerEnterPworldReq->ExtLen = sizeof(stSyncInfo);
		
	}
	
	
	//�ڴ�״̬�޸�
	if( (eStatusAssistant&PLAYER_STATUS_BATTLE_LOGIN_ON) &&
		pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN )
	{
		pstPlayer->eStatus = PLAYER_STATUS_ROLE_ON_BATTLE;
	}
	else if( (eStatusAssistant&PLAYER_STATUS_BATTLE_LOGIN_READY ||
			  eStatusAssistant == PLAYER_STATUS_BATTLE_NOUSE) &&
				pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN )
	{
		
	}
	else
	{
		if(pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "����ս��ʧ��");
		}
		return -1;
	}
	
	Z_SSHEAD_INIT(&stPkg.Head, BATTLE_ENTER_REQ);
	z_ssmsg_send(pstEnv, &stPkg, pstBattleCampInfo->BattleBusID);

	return 0;
}

// battle����
int player_battle_enter(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, SSBATTLEPLAYERENTERPWORLDREQ *pstBattlePlayerEnterPworldReq)
{	
	int iRet = 0;
	SSPKG stPkg;
	SSBATTLEPLAYERENTERPWORLDRES *pstBattlePlayerEnterPworldRes = &stPkg.Body.BattlePlayerEnterPworldRes;

	memset(pstBattlePlayerEnterPworldRes,0,sizeof(SSBATTLEPLAYERENTERPWORLDRES));
	
	iRet = player_battle_acc_login( pstAppCtx,  pstEnv, pstBattlePlayerEnterPworldReq);

	pstBattlePlayerEnterPworldRes->Result = iRet;
	pstBattlePlayerEnterPworldRes->Uin = pstBattlePlayerEnterPworldReq->RoleData.Uin;
	pstBattlePlayerEnterPworldRes->RoleID = pstBattlePlayerEnterPworldReq->RoleData.RoleID;
	
	Z_SSHEAD_INIT(&stPkg.Head, BATTLE_ENTER_RES);
	z_ssmsg_send(pstEnv, &stPkg, pstBattlePlayerEnterPworldReq->BattleCampInfo.ZoneBusID);

	if(pstBattlePlayerEnterPworldRes->Result > 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "enter pworld req |Result|%d|Uin|%d|RoleID|%llu|pword|%d|%d|%llu|from|%s",
			pstBattlePlayerEnterPworldRes->Result,
			pstBattlePlayerEnterPworldRes->Uin,
			pstBattlePlayerEnterPworldRes->RoleID,
			pstBattlePlayerEnterPworldReq->BattleCampInfo.PworldDefID,
			pstBattlePlayerEnterPworldReq->BattleCampInfo.PworldInstID,
			pstBattlePlayerEnterPworldReq->BattleCampInfo.PworldWID,
			tbus_addr_ntoa(pstBattlePlayerEnterPworldReq->BattleCampInfo.ZoneBusID));
	}
		
	return 0;
}

int player_battle_enter_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,SSBATTLEPLAYERENTERPWORLDRES *pstBattlePlayerEnterPworldRes)
{	
	Player *pstPlayer = NULL;
	int i;
	
	pstPlayer = player_get_by_uin(pstEnv, pstBattlePlayerEnterPworldRes->Uin);
	if( pstPlayer == NULL )
	{
		return -1;
	}

	if(pstBattlePlayerEnterPworldRes->Result != 0 &&
       pstBattlePlayerEnterPworldRes->Result != BATTLE_ERR_NO_TEAM)
	{
		player_battle_exit_req(pstAppCtx,pstEnv,pstPlayer);	
		if( pstBattlePlayerEnterPworldRes->Result == BATTLE_ERR_ACC_MUL_ROLE)
		{
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "ս���Ѿ�����ͬ���ʺŵ�½��");
		}
		else 
		{
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "׼������ս��ʧ��");
		}
	}
	if(pstPlayer->eStatus == PLAYER_STATUS_ROLE_ON_BATTLE)
	{
		if(  pstBattlePlayerEnterPworldRes->Result != 0 ||
			 pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.BattleBusID == 0 )
		{
			// ���������û���
			char szBuff[128];
			int  iOffset = 0;
			int  iCmd = 0;

			memset(szBuff,0,sizeof(szBuff));
			iCmd = 2;
			memcpy(szBuff+iOffset,&iCmd,sizeof(iCmd));
			iOffset += sizeof(iCmd);
			memcpy(szBuff+iOffset,&pstPlayer->stRoleData.Uin,sizeof(pstPlayer->stRoleData.Uin));
			iOffset += sizeof(pstPlayer->stRoleData.Uin);
			z_set_routing_req( pstEnv, pstPlayer->stConnectInfo.iConnIdx, pstPlayer->stConnectInfo.iID,
				 pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.ZoneBusID,
	         	pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.ZoneConnID,
	         	0, szBuff, iOffset);
			
			if( pstPlayer->eStatus == PLAYER_STATUS_ROLE_ON_BATTLE )
			{
				// ��״̬�Ļ���
				pstPlayer->eStatus =PLAYER_STATUS_ROLE_LOGIN;
				tlog_info(pstEnv->pstLogCat, 0, 0, "Status=RoleLogin Uin=%d AccName=%s RoleName=%s", 
				pstPlayer->stRoleData.Uin, pstPlayer->szAccount, pstPlayer->stRoleData.RoleName);
			}
			
			if (pstBattlePlayerEnterPworldRes->Result == BATTLE_ERR_PWORLD_CLOSE)
			{
				// ս�������Ѿ��������  ����ȴ�����
				for (i=0; i<MAX_PWORLD_WAIT_QUEUE; i++)
				{
					if (pstEnv->pstPworldQueue->stWaitQueue[i].iPworldInstID == 0)
					{
						continue;
					}
					
					if (pstEnv->pstPworldQueue->stWaitQueue[i].iBattleBusID == 
						pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.BattleBusID )
					{
						z_clear_pworld_wait_queue(pstEnv, i, pstEnv->pstPworldQueue->stWaitQueue[i].iPworldInstID);
						break;
					}
				}
				
			}			
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "����ս��ʧ��");

			player_battle_notify_clean_player(pstEnv,pstPlayer->stRoleData.Uin,
						pstPlayer->stRoleData.RoleID,
						&pstPlayer->stOnline.stPworldQueue.stBattleCampInfo,
						BATTLE_OPER_CLEAN_PLAYER,
						pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.BattleBusID);
				
			return -1;
		}

	
		// ֪ͨ����������뿪�����ͼ��
		z_player_leave_map(pstAppCtx, pstEnv, pstPlayer, 1);
		player_battle_set_routing_req(pstAppCtx,pstEnv,pstPlayer,0);
	}
	
	return 0;
}

int player_battle_set_routing_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,Player *pstPlayer,int iOperType)
{
	SSPKG stPkg;
	SSSETBATTLEROUTINREQ *pstSetBattleRoutinReq = &stPkg.Body.SetBattleRoutinReq;

	memset(pstSetBattleRoutinReq,0,sizeof(SSSETBATTLEROUTINREQ));

	pstSetBattleRoutinReq->Uin = pstPlayer->stRoleData.Uin;
	pstSetBattleRoutinReq->RoleID = pstPlayer->stRoleData.RoleID;
	pstSetBattleRoutinReq->Opertype = iOperType;
	pstSetBattleRoutinReq->ConnID = pstPlayer->iIdx;

	Z_SSHEAD_INIT(&stPkg.Head, SET_BATTLE_ROUTING_REQ);
	z_ssmsg_send(pstEnv, &stPkg, pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.BattleBusID);
	return 0;
	UNUSED(pstAppCtx);
}

int player_battle_set_routing(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,SSSETBATTLEROUTINREQ *pstSetBattleRoutinReq)
{
	Player *pstPlayer =NULL;
	SSPKG stPkg;
	SSSETBATTLEROUTINRES *pstSetBattleRoutinRes = &stPkg.Body.SetBattleRoutinRes;

	pstPlayer = player_get_by_uin(pstEnv, pstSetBattleRoutinReq->Uin);
	if( pstPlayer == NULL )
	{
		return -1;
	}
	memset(pstSetBattleRoutinRes,0,sizeof(SSSETBATTLEROUTINRES));

	pstSetBattleRoutinRes->Uin = pstSetBattleRoutinReq->Uin;
	pstSetBattleRoutinRes->RoleID = pstSetBattleRoutinReq->RoleID;
	pstSetBattleRoutinRes->ConnID = pstPlayer->stConnectInfo.iID;
	pstSetBattleRoutinRes->Opertype = pstSetBattleRoutinRes->Opertype;
		
	Z_SSHEAD_INIT(&stPkg.Head, SET_BATTLE_ROUTING_RES);
	z_ssmsg_send(pstEnv, &stPkg, pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.ZoneBusID);
	return 0;
	UNUSED(pstAppCtx);
}

int player_battle_set_routing_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,SSSETBATTLEROUTINRES *pstSetBattleRoutinRes)
{
	Player *pstPlayer =NULL;
	char szBuff[128];
	int  iOffset = 0;
	int  iCmd = 0;

	pstPlayer = player_get_by_uin(pstEnv, pstSetBattleRoutinRes->Uin);
	if( pstPlayer == NULL )
	{
		return -1;
	}
	
	memset(szBuff,0,sizeof(szBuff));
	iCmd = 1;
	memcpy(szBuff+iOffset,&iCmd,sizeof(iCmd));
	iOffset += sizeof(iCmd);
	memcpy(szBuff+iOffset,&pstSetBattleRoutinRes->Uin,sizeof(pstSetBattleRoutinRes->Uin));
	iOffset += sizeof(pstSetBattleRoutinRes->Uin);
	
	z_set_routing_req( pstEnv, pstPlayer->stConnectInfo.iConnIdx, pstSetBattleRoutinRes->ConnID,
			 			pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.ZoneBusID,
                      	pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.ZoneConnID,
                      	pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.BattleBusID,
                      	szBuff, iOffset);
	
	return 0;
	UNUSED(pstAppCtx);
}
int player_battle_enter_map_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,Player *pstPlayer)
{
	int iRet = 0;
	SSPKG stPkg;
	SSBATTLEENTERPWORLDMAPREQ *pstBattleEnterPworldMapReq = &stPkg.Body.BattleEnterPworldMapReq;

	if (pstPlayer->stOnline.cMoving) 
	{
		pstPlayer->stOnline.cMoving = 0;
		z_player_move_stop(pstAppCtx, pstEnv,  pstPlayer);
	}
	iRet = player_battle_enter_check(pstAppCtx,pstEnv,pstPlayer);
	if( iRet != 0 )
	{
		return -1;
	}

	//��ʼ����
	memset(pstBattleEnterPworldMapReq,0,sizeof(SSBATTLEENTERPWORLDMAPREQ));

	pstBattleEnterPworldMapReq->Uin = pstPlayer->stRoleData.Uin;
	pstBattleEnterPworldMapReq->RoleID = pstPlayer->stRoleData.RoleID;

	Z_SSHEAD_INIT(&stPkg.Head, BATTLE_ENTER_PWORLD);
	z_ssmsg_send(pstEnv, &stPkg, pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.BattleBusID);

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
} 
int player_battle_enter_map(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,SSBATTLEENTERPWORLDMAPREQ *pstBattleEnterPworldMapReq)
{
	int iRet = 0;
	Player *pstPlayer = NULL;
	
	pstPlayer = player_get_by_uin(pstEnv, pstBattleEnterPworldMapReq->Uin);
	if( pstPlayer == NULL )
	{
		return -1;
	}
	//�޸���״̬
	pstPlayer->eStatus = PLAYER_STATUS_ROLE_LOGIN;
	tlog_info(pstEnv->pstLogCat, 0, 0, "Status=RoleLogin Uin=%d AccName=%s RoleName=%s", 
			pstPlayer->stRoleData.Uin, pstPlayer->szAccount, pstPlayer->stRoleData.RoleName);

	if(pstPlayer->stRoleData.RoleID != pstBattleEnterPworldMapReq->RoleID )
	{
		z_sys_strf( pstEnv, pstPlayer, SYS_MIDDLE2, "ս���Ѿ�����ͬ���ʺŵ�������ɫ��½�� %s", pstPlayer->szAccount);
		iRet =  BATTLE_ERR_SYSTEM;
	}
	else
	{
		iRet = player_battle_role_login(pstAppCtx, pstEnv, pstPlayer,  RAND1(180),0 );
	}
	if( iRet != 0 )
	{
		// Ҫ����roledata
		if(is_battle_pve_autoteam_svr(pstEnv))
		{
			pstPlayer->stOnline.eStatusAssistant |= PLAYER_STATUS_BATTLE_GOTO_ZONESVR;
	
			z_db_role_update(pstAppCtx, pstEnv, pstPlayer, BATTLE_OPER_USER_LOGOUT_PVE_PWORLD);
			
			pstPlayer->eStatus = PLAYER_STATUS_ACC_LOGIN;
			pstPlayer->stOnline.eStatusAssistant = PLAYER_STATUS_BATTLE_LOGIN_READY;
			pstPlayer->stOnline.eStatusAssistant |= PLAYER_STATUS_BATTLE_TEAM_SYNC;
			player_do_statusassitant_sync(pstEnv,pstPlayer);
		}
		else
		{
			z_db_role_update(pstAppCtx, pstEnv, pstPlayer,BATTLE_OPER_CLEAN_BATTLE_STATUS);
		}
		
	}
	tlog_info(pstEnv->pstLogCat, 0, 0, "enter map req |Result|%d|Uin|%d|RoleID|%llu",
		iRet,
		pstBattleEnterPworldMapReq->Uin,
		pstBattleEnterPworldMapReq->RoleID);
	
	return 0;
} 

int player_battle_hello2battle_req(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, TFRAMEHEAD *pstFrameHead,Player * pstPlayer)
{
	SSBATTLECAMPINFO *pstBattleCampInfo = &pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;

	if( (pstPlayer->stOnline.eStatusAssistant&PLAYER_STATUS_BATTLE_LOGIN_READY)&&
		(PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus ||
		 PLAYER_STATUS_ROLE_LIST == pstPlayer->eStatus) &&
		pstBattleCampInfo->BattleBusID>0 )
	{
		player_battle_enter_req(pstAppCtx,pstEnv,pstFrameHead,
				pstPlayer,PLAYER_STATUS_BATTLE_LOGIN_READY,pstBattleCampInfo->BattleBusID);
	}
	return 0;
}

int player_battle_hello2zone_req(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, Player * pstPlayer)
{
	SSPKG stPkg;
	SSBATTLEPLAYERHELLOREQ *pstBattlePlayerHelloReq = &stPkg.Body.BattlePlayerHelloReq;

	memset(pstBattlePlayerHelloReq,0,sizeof(SSBATTLEPLAYERHELLOREQ));

	pstBattlePlayerHelloReq->Uin = pstPlayer->stRoleData.Uin;
	pstBattlePlayerHelloReq->RoleID = pstPlayer->stRoleData.RoleID;
	pstBattlePlayerHelloReq->BattleCampInfo = pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;
	
	Z_SSHEAD_INIT(&stPkg.Head, BATTLE_PLAYER_HELLO_REQ);
	z_ssmsg_send(pstEnv, &stPkg, pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.ZoneBusID);

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int player_battle_hello2zone(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, SSBATTLEPLAYERHELLOREQ *pstBattlePlayerHelloReq)
{
	Player *pstPlayer = NULL;
			
	pstPlayer = player_get_by_uin(pstEnv, pstBattlePlayerHelloReq->Uin);
	if( pstPlayer == NULL )
	{
		player_battle_notify_clean_player(pstEnv,pstBattlePlayerHelloReq->Uin,
										pstBattlePlayerHelloReq->RoleID,
										&pstBattlePlayerHelloReq->BattleCampInfo,
										BATTLE_OPER_CLEAN_PLAYER,
										pstBattlePlayerHelloReq->BattleCampInfo.BattleBusID);
		return -1;
	}

	if( pstPlayer->eStatus != PLAYER_STATUS_ROLE_ON_BATTLE ||
		pstPlayer->stRoleData.RoleID  != pstBattlePlayerHelloReq->RoleID)
	{
		player_battle_notify_clean_player(pstEnv,pstBattlePlayerHelloReq->Uin,
										pstBattlePlayerHelloReq->RoleID,
										&pstBattlePlayerHelloReq->BattleCampInfo,
										BATTLE_OPER_CLEAN_PLAYER,
										pstBattlePlayerHelloReq->BattleCampInfo.BattleBusID);
		return -1;
	}

	// ϵͳ�ر���,���������޸���
	if (pstEnv->cShutdown > 1)
	{
		return 0;
	}
	// �ʱ���ս�������ݳ�5��,ȷ�����ݵ���ȷ����
	pstPlayer->tActive = pstAppCtx->stCurr.tv_sec + 5;
	player_battle_notify_clean_player(pstEnv,pstBattlePlayerHelloReq->Uin,
										pstBattlePlayerHelloReq->RoleID,
										&pstBattlePlayerHelloReq->BattleCampInfo,
										BATTLE_OPER_OK,
										pstBattlePlayerHelloReq->BattleCampInfo.BattleBusID);
	return 0;
}


int player_battle_notify_clean_player(ZONESVRENV* pstEnv,int Uin,unsigned long long ullRoleID, 
					SSBATTLECAMPINFO *pstBattleCampInfo,int OperType,int BattleBusID)
{
	SSPKG stPkg;
	SSBATTLEUPDATEROLEDATARES *pstBattleUpdateRoleDataRes = &stPkg.Body.BattleUpdateRoleDataRes;

	memset(pstBattleUpdateRoleDataRes,0,sizeof(SSBATTLEUPDATEROLEDATARES));
	
	pstBattleUpdateRoleDataRes->RoleID = ullRoleID;
	pstBattleUpdateRoleDataRes->Uin = Uin;
	memcpy(&pstBattleUpdateRoleDataRes->BattleCampInfo, pstBattleCampInfo,sizeof(SSBATTLECAMPINFO));

	pstBattleUpdateRoleDataRes->OperType = OperType;
	Z_SSHEAD_INIT(&stPkg.Head, BATTLE_UPDATE_ROLEDATA_RES);
	z_ssmsg_send(pstEnv, &stPkg, BattleBusID);

	tlog_info(pstEnv->pstLogCat, 0, 0, "update roledata msg|OperType|%d|Uin|%d|RoleID|%llu",
			OperType,
			pstBattleUpdateRoleDataRes->Uin,
			pstBattleUpdateRoleDataRes->RoleID
			);
	return 0;
}


int player_battle_update_roledate(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,SSBATTLEUPDATEROLEDATAREQ *pstBattleUpdateRoleDataReq )
{
	Player *pstPlayer  = player_get_by_uin(pstEnv, pstBattleUpdateRoleDataReq->RoleData.Uin);
	if( pstPlayer == NULL )
	{
		player_battle_notify_clean_player(pstEnv,pstBattleUpdateRoleDataReq->RoleData.Uin,
										pstBattleUpdateRoleDataReq->RoleData.RoleID,
										&pstBattleUpdateRoleDataReq->BattleCampInfo,
										BATTLE_OPER_CLEAN_PLAYER,
										pstBattleUpdateRoleDataReq->BattleCampInfo.BattleBusID);
		return -1;
	}

	if( pstPlayer->eStatus != PLAYER_STATUS_ROLE_ON_BATTLE ||
		pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.PworldDefID != pstBattleUpdateRoleDataReq->BattleCampInfo.PworldDefID ||
		pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.PworldInstID != pstBattleUpdateRoleDataReq->BattleCampInfo.PworldInstID ||
		pstPlayer->stRoleData.RoleID  != pstBattleUpdateRoleDataReq->RoleData.RoleID)
	{
		player_battle_notify_clean_player(pstEnv,pstBattleUpdateRoleDataReq->RoleData.Uin,
								pstBattleUpdateRoleDataReq->RoleData.RoleID,
								&pstBattleUpdateRoleDataReq->BattleCampInfo,
								BATTLE_OPER_CLEAN_PLAYER,
								pstBattleUpdateRoleDataReq->BattleCampInfo.BattleBusID);
		return -1;
	}
	
	if( pstBattleUpdateRoleDataReq->OperType != BATTLE_OPER_CLEAN_BATTLE_STATUS &&
		pstBattleUpdateRoleDataReq->RoleData.Level == 0)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "battle role data error!!!");		
		player_battle_notify_clean_player(pstEnv,pstBattleUpdateRoleDataReq->RoleData.Uin,
								pstBattleUpdateRoleDataReq->RoleData.RoleID,
								&pstBattleUpdateRoleDataReq->BattleCampInfo,
								BATTLE_OPER_CLEAN_PLAYER,
								pstBattleUpdateRoleDataReq->BattleCampInfo.BattleBusID);
		return -1;
	}
	
	// �����½�ɫ������
	if( pstBattleUpdateRoleDataReq->OperType != BATTLE_OPER_CLEAN_BATTLE_STATUS &&
		pstBattleUpdateRoleDataReq->RoleData.Level > 0 )
	{
		// ע��˴������޸ĵ��൱���������:
		// 1. ��ɫ��
		// 2. ������Ϣ,��Ϊ����ʵ������zone����
		// 3. ������Ϣ
		ProtectedRoleData stSavedData;
		role_save_protected_data(&pstPlayer->stRoleData, &stSavedData);
		memcpy(&pstPlayer->stRoleData, &pstBattleUpdateRoleDataReq->RoleData,
		       sizeof(ROLEDATA));
		role_restore_protected_data(&stSavedData, &pstPlayer->stRoleData);
	}

	
	if( BATTLE_OPER_OK != pstBattleUpdateRoleDataReq->OperType)
	{
		char szBuff[128];
		int  iOffset = 0;
		int  iCmd = 0;

		memset(szBuff,0,sizeof(szBuff));
		iCmd = 3;
		memcpy(szBuff+iOffset,&iCmd,sizeof(iCmd));
		iOffset += sizeof(iCmd);
		memcpy(szBuff+iOffset,&pstPlayer->stRoleData.Uin,sizeof(pstPlayer->stRoleData.Uin));
		iOffset += sizeof(pstPlayer->stRoleData.Uin);
		z_set_routing_req( pstEnv, pstPlayer->stConnectInfo.iConnIdx, pstPlayer->stConnectInfo.iID,
			pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.ZoneBusID,
            pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.ZoneConnID,
            0,szBuff, iOffset);


		if(BATTLE_OPER_USER_LOGOUT_PVE_PWORLD != pstBattleUpdateRoleDataReq->OperType)
		{
			pstPlayer->stOnline.eStatusAssistant = 0;
			player_do_statusassitant_sync(pstEnv,pstPlayer);
		}	
		pstPlayer->eStatus = PLAYER_STATUS_ROLE_LOGIN;

		if(pstBattleUpdateRoleDataReq->OperType == BATTLE_OPER_USER_LOGOUT_PVE_PWORLD ||
			pstBattleUpdateRoleDataReq->OperType == BATTLE_OPER_USER_LOGOUT)
		{
			PlayerSyncOnline stSyncInfo;

			if(pstBattleUpdateRoleDataReq->ExtLen == sizeof(stSyncInfo))
			{
				memcpy(&stSyncInfo, pstBattleUpdateRoleDataReq->ExtData, sizeof(stSyncInfo));
				pstPlayer->stOnline.stFairyOnline = stSyncInfo.stFairyOnline;
				pstPlayer->stOnline.stHeti = stSyncInfo.stHeti;
				pstPlayer->stOnline.stPetOnline = stSyncInfo.stPetOnline;
				pstPlayer->stOnline.cMiscFlag = stSyncInfo.ullMiscFlag;
				pstPlayer->stOnline.cSafeLockFalg = stSyncInfo.cSafeLockFalg;
			}
		}

		if( /*BATTLE_OPER_CLEAN_BATTLE_STATUS != pstBattleUpdateRoleDataReq->OperType  &&*/
			BATTLE_OPER_ROLE_LOGOUT != pstBattleUpdateRoleDataReq->OperType )
		{
			player_battle_role_login(pstAppCtx, pstEnv, pstPlayer, RAND1(180), 0);
			if (pstEnv->cShutdown > 1)
			{
				// zone_svr�ر�ʱ,kick offս�����Լ�������ҷ���,
				// ֮�������kickoff����
				player_kickoff(pstAppCtx, pstEnv, pstPlayer, "ϵͳ�ر�");
			}
		}
		
		if( BATTLE_OPER_ROLE_LOGOUT == pstBattleUpdateRoleDataReq->OperType )
		{
			// Ҫ���ؽ�ɫ�б�,��Ҫ���ϻ�д,�����ؽ�ɫ�б�
			pstPlayer->stOnline.tLogoutBegin = pstAppCtx->stCurr.tv_sec - PLAYER_DELAY_LOGOUT_TIME;
			pstPlayer->ucRoleLogoutReason = ROLE_LOGOUT_CLTREQ;
			//pstPlayer->stOnline.bWaitRoleLogout = 1;
			pstPlayer->stOnline.bWaitRoleLogout = 2;
			pstPlayer->eStatus = PLAYER_STATUS_ROLE_LOGOUT_REQ;
			z_db_role_update(pstAppCtx, pstEnv, pstPlayer,BATTLE_OPER_ROLE_LOGOUT);
		}
		else if(BATTLE_OPER_USER_LOGOUT == pstBattleUpdateRoleDataReq->OperType &&
			   pstPlayer->stRoleData.MiscInfo.SpanSvrTeamID == 0 &&
			   pstPlayer->stRoleData.MiscInfo.PworldInstID == 0 )
		{
			pstPlayer->stOnline.eStatusAssistant = 0;
		}

		if(BATTLE_OPER_USER_LOGOUT_PVE_PWORLD == pstBattleUpdateRoleDataReq->OperType)
		{
			
		}
		else
		{
			//��battle_svr������û�����
			player_battle_notify_clean_player(pstEnv,pstBattleUpdateRoleDataReq->RoleData.Uin,
									pstBattleUpdateRoleDataReq->RoleData.RoleID,
									&pstBattleUpdateRoleDataReq->BattleCampInfo,
									BATTLE_OPER_CLEAN_PLAYER,
									pstBattleUpdateRoleDataReq->BattleCampInfo.BattleBusID);

			autoteam_notify_clt_clean(pstEnv, pstPlayer);
		}

		return 0;
	}

	//�����»�дʱ��
	pstPlayer->tActive = pstAppCtx->stCurr.tv_sec;
	
	player_battle_notify_clean_player(pstEnv,pstBattleUpdateRoleDataReq->RoleData.Uin,
								pstBattleUpdateRoleDataReq->RoleData.RoleID,
								&pstBattleUpdateRoleDataReq->BattleCampInfo,
								BATTLE_OPER_OK,
								pstBattleUpdateRoleDataReq->BattleCampInfo.BattleBusID);
	return 0;
}




int player_battle_update_roledate_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,Player *pstPlayer,int OperType)
{
	ZoneTeam *pstZoneTeam;
	SSPKG stPkg;
	SSBATTLEUPDATEROLEDATAREQ *pstBattleUpdateRoleDataReq = &stPkg.Body.BattleUpdateRoleDataReq;

	memset(pstBattleUpdateRoleDataReq,0,sizeof(SSBATTLEUPDATEROLEDATAREQ));
	
	if (OperType!=BATTLE_OPER_OK) 
	{
		if(pstPlayer->stOnline.cMoving)
		{
			z_player_move_stop(pstAppCtx, pstEnv,  pstPlayer);
			pstPlayer->stOnline.cMoving = 0;
		}
		z_change_battle_map_check(pstAppCtx, pstEnv, pstPlayer);
	}
	
	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if (pstZoneTeam)
	{
		pstPlayer->stRoleData.MiscInfo.SpanSvrTeamID = pstZoneTeam->stTeamInfo.TeamID;
		pstPlayer->stRoleData.MiscInfo.SpanSvrTeamIdx = pstZoneTeam->iTeamIdx;
	}
	else
	{
		pstPlayer->stRoleData.MiscInfo.SpanSvrTeamID = 0;
		pstPlayer->stRoleData.MiscInfo.SpanSvrTeamIdx = 0;
		pstPlayer->stRoleData.MiscInfo.PworldInstID = 0;
	}

	pstBattleUpdateRoleDataReq->OperType = OperType;
	pstBattleUpdateRoleDataReq->BattleCampInfo = pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;
	memcpy(&pstBattleUpdateRoleDataReq->RoleData,&pstPlayer->stRoleData,sizeof(ROLEDATA));

	pstBattleUpdateRoleDataReq->ExtLen = 0;
	if(OperType == BATTLE_OPER_USER_LOGOUT_PVE_PWORLD ||
		OperType == BATTLE_OPER_USER_LOGOUT)
	{
		PlayerSyncOnline stSyncInfo;
		
		stSyncInfo.stFairyOnline = pstPlayer->stOnline.stFairyOnline;
		stSyncInfo.stHeti = pstPlayer->stOnline.stHeti;
		stSyncInfo.stPetOnline = pstPlayer->stOnline.stPetOnline;
		stSyncInfo.ullMiscFlag = pstPlayer->stOnline.cMiscFlag;
		stSyncInfo.cSafeLockFalg = pstPlayer->stOnline.cSafeLockFalg;

		memcpy(pstBattleUpdateRoleDataReq->ExtData, &stSyncInfo, sizeof(stSyncInfo));
		pstBattleUpdateRoleDataReq->ExtLen = sizeof(stSyncInfo);
	}
	
	Z_SSHEAD_INIT(&stPkg.Head, BATTLE_UPDATE_ROLEDATA_REQ);
	z_ssmsg_send(pstEnv, &stPkg, pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.ZoneBusID);
	
	tlog_info(pstEnv->pstLogCat, 0, 0, "update roledate req |OperType|%d|Uin|%d|RoleID|%llu",
		OperType,
		pstBattleUpdateRoleDataReq->RoleData.Uin,
		pstBattleUpdateRoleDataReq->RoleData.RoleID);

	if( BATTLE_OPER_OK != OperType )
	{
		z_db_role_logout_res(pstAppCtx, pstEnv, pstPlayer);
		pstPlayer->eStatus = PLAYER_STATUS_ROLE_LOGOUT_BATTLE;
	}
		
	return 0;
}
int player_battle_update_roledate_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,SSBATTLEUPDATEROLEDATARES *pstBattleUpdateRoleDataRes )
{
	Player *pstPlayer = NULL;
	
	pstPlayer = player_get_by_uin(pstEnv, pstBattleUpdateRoleDataRes->Uin);
	if( pstPlayer == NULL )
	{
		return -1;
	}
		
	if( BATTLE_OPER_CLEAN_PLAYER == pstBattleUpdateRoleDataRes->OperType)
	{
		//autoteam_exit_trans(pstEnv, pstPlayer);
		if (pstPlayer->stOnline.stAutoTeamInfo.tStart)
		{
			autoteam_reject(pstEnv, pstPlayer,0);
		}
		z_player_leave_map(pstAppCtx, pstEnv, pstPlayer, 1);
		// �������� ������û����������
		if( PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus )
		{
			z_db_role_logout_res(pstAppCtx, pstEnv, pstPlayer);
			pstPlayer->eStatus = PLAYER_STATUS_ROLE_LOGOUT_BATTLE;
		}
		z_free_player(pstAppCtx, pstEnv, pstPlayer);
	}
	else if( BATTLE_OPER_KICKOF == pstBattleUpdateRoleDataRes->OperType)
	{
		char szMsg[DESC_LEN];
	
		autoteam_exit_trans(pstEnv, pstPlayer);
		z_player_leave_map(pstAppCtx, pstEnv, pstPlayer, 1);
		
		memset(szMsg,0,sizeof(szMsg));
		snprintf(szMsg,sizeof(szMsg), "�������û�ʹ�������ʺŵ�¼");
		player_battle_now_kickoff(pstAppCtx, pstEnv, pstPlayer, szMsg);
	}
	else
	{
		// �������û��Ļʱ��
		pstPlayer->tActive = pstAppCtx->stCurr.tv_sec;
	}
	
	tlog_info(pstEnv->pstLogCat, 0, 0, "update roledate res |OperType|%d|Uin|%d|RoleID|%llu",
		pstBattleUpdateRoleDataRes->OperType,
		pstBattleUpdateRoleDataRes->Uin,
		pstBattleUpdateRoleDataRes->RoleID);
	return 0;
}
// ս���������߼�
int player_battle_now_kickoff(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, char *pszMsg)
{
	if( 1 == pstPlayer->stOnline.bWaitRoleLogout)
	{
		return 0;
	}
	
	if (pszMsg)
	{
		player_kickoff_text_notify(pstAppCtx, pstEnv, pstPlayer, pszMsg);
	}
	
	player_switch_role_delay_logout(pstEnv, pstPlayer, ROLE_LOGOUT_KICK);

	player_disconnect( pstEnv, pstPlayer );

	// Ҫ���������������д��zone_svr
	z_db_role_update(pstAppCtx, pstEnv, pstPlayer,BATTLE_OPER_KICKOF);
	
	return 0;
}
// ��������˳�ս��,�س̾�,
int player_battle_logout(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int OperType;
	ZoneTeam *pstZoneTeam = NULL;

	if( pstEnv->iInstMode != INST_MODE_BATTLE_SVR )
	{
		return -1;
	}

	if (pstPlayer->stOnline.stAutoTeamInfo.tStart)
	{
		autoteam_reject(pstEnv, pstPlayer,0);
	}
	
	z_change_battle_map_check(pstAppCtx, pstEnv, pstPlayer);
	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if(pstZoneTeam && is_battle_pve_autoteam_svr(pstEnv))
	{
		OperType = BATTLE_OPER_USER_LOGOUT_PVE_PWORLD;
	}
	else
	{
		OperType = BATTLE_OPER_USER_LOGOUT;
	}
		

	if(BATTLE_OPER_USER_LOGOUT_PVE_PWORLD == OperType)
	{
		pstPlayer->stOnline.eStatusAssistant |= PLAYER_STATUS_BATTLE_GOTO_ZONESVR;
	}

	//vip bar ��״̬
	//if (pstPlayer->stOnline.stVipBarInfo.iVipBarID)
	//{
		//player_vip_bar_buff(pstEnv,pstPlayer,FAKE_STATUS_OP_DEL);
	//}
	
	z_db_role_update(pstAppCtx, pstEnv, pstPlayer,OperType);
	
	if(BATTLE_OPER_USER_LOGOUT == OperType)
	{
		pstPlayer->eStatus = PLAYER_STATUS_ROLE_LOGOUT_BATTLE;
	}
	else if(BATTLE_OPER_USER_LOGOUT_PVE_PWORLD == OperType)
	{
		pstPlayer->eStatus = PLAYER_STATUS_ACC_LOGIN;
		

		pstPlayer->stOnline.eStatusAssistant = PLAYER_STATUS_BATTLE_LOGIN_READY;
		pstPlayer->stOnline.eStatusAssistant |= PLAYER_STATUS_BATTLE_TEAM_SYNC;
		player_do_statusassitant_sync(pstEnv,pstPlayer);
	}

	return 0;
}
int player_do_statusassitant(ZONESVRENV* pstEnv,SSPLAYERSTATUSASSITANTSYNC *pstPlayerStatusassitantSync)
{
	Player *pstPlayer = NULL;
	SSBATTLECAMPINFO *pststBattleCampInfo;

	pstPlayer = player_get_by_uin(pstEnv, pstPlayerStatusassitantSync->Uin);
	if( pstPlayer == NULL )
	{
		return -1;
	}

	if(pstPlayer->stRoleData.RoleID != pstPlayerStatusassitantSync->RoleID)
	{
		return -1;
	}

	pststBattleCampInfo = &pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;

	if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
	{
		//zonesvr��������ͬ��״̬
		if(pstPlayerStatusassitantSync->eStatusAssistant != pstPlayer->stOnline.eStatusAssistant )
		{
			if(pstPlayerStatusassitantSync->OperType == DO_STATUSASSITANT_SYNC)
			{
				pstPlayer->stOnline.eStatusAssistant = pstPlayerStatusassitantSync->eStatusAssistant;
			}
			//player_do_battle_statusassitant(pstEnv,pstPlayer,DO_STATUSASSITANT_SYNC);
		}
	}
	else if(INST_MODE_ZONE_SVR == pstEnv->iInstMode)
	{
		if(pstPlayerStatusassitantSync->eStatusAssistant != pstPlayer->stOnline.eStatusAssistant )
		{
			if(pstPlayerStatusassitantSync->OperType == DO_STATUSASSITANT_SYNC)
			{
				pstPlayer->stOnline.eStatusAssistant = pstPlayerStatusassitantSync->eStatusAssistant;
				if(pststBattleCampInfo->BattleBusID <=0 && pstPlayer->stOnline.eStatusAssistant)
				{
					player_battle_enter_req(pstEnv->pstAppCtx,pstEnv,&pstPlayer->stOnline.stFrameHead,
						pstPlayer,PLAYER_STATUS_BATTLE_LOGIN_READY,pstEnv->iBattlePveAutoteamID);
				}
				if(pstPlayer->stOnline.eStatusAssistant==0)
				{
					pstPlayer->stRoleData.MiscInfo.SpanSvrTeamID = 0;
					pstPlayer->stRoleData.MiscInfo.SpanSvrTeamIdx= 0;
				}
			}
			//player_do_zone_statusassitant(pstEnv,pstPlayer,DO_STATUSASSITANT_SYNC);
		}
	}
	
	return 0;
}

int player_do_statusassitant_sync(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
	{
		player_do_battle_statusassitant(pstEnv,pstPlayer,DO_STATUSASSITANT_SYNC);
		
	}
	else if(INST_MODE_ZONE_SVR == pstEnv->iInstMode)
	{
		player_do_zone_statusassitant(pstEnv,pstPlayer,DO_STATUSASSITANT_SYNC);
	}
	
	return 0;
}

int player_do_battle_statusassitant(ZONESVRENV* pstEnv,Player *pstPlayer,int OperType)
{
	SSPKG stPkg;
	SSPLAYERSTATUSASSITANTSYNC *pstPlayerStatusassitantRes = &stPkg.Body.SSPlayerStatusassitantRes;
	SSBATTLECAMPINFO *pststBattleCampInfo = &pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;
	
	memset(pstPlayerStatusassitantRes,0,sizeof(SSPLAYERSTATUSASSITANTSYNC));
	
	pstPlayerStatusassitantRes->Uin = pstPlayer->stRoleData.Uin;
	pstPlayerStatusassitantRes->RoleID = pstPlayer->stRoleData.RoleID;
	pstPlayerStatusassitantRes->OperType = OperType;
	pstPlayerStatusassitantRes->eStatusAssistant = pstPlayer->stOnline.eStatusAssistant;
	pstPlayerStatusassitantRes->DataLen = 0;
	
	Z_SSHEAD_INIT(&stPkg.Head, BATTLE_STATUSASSISTANT_RES);
	z_ssmsg_send(pstEnv, &stPkg, pststBattleCampInfo->ZoneBusID);

	return 0;
}

int player_do_zone_statusassitant(ZONESVRENV* pstEnv,Player *pstPlayer,int OperType)
{
	SSPKG stPkg;
	SSPLAYERSTATUSASSITANTSYNC *pstPlayerStatusassitantReq = &stPkg.Body.SSPlayerStatusassitantReq;
	SSBATTLECAMPINFO *pststBattleCampInfo = &pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;
	
	memset(pstPlayerStatusassitantReq,0,sizeof(SSPLAYERSTATUSASSITANTSYNC));
	
	pstPlayerStatusassitantReq->Uin = pstPlayer->stRoleData.Uin;
	pstPlayerStatusassitantReq->RoleID = pstPlayer->stRoleData.RoleID;
	pstPlayerStatusassitantReq->OperType = OperType;
	pstPlayerStatusassitantReq->eStatusAssistant = pstPlayer->stOnline.eStatusAssistant;
	pstPlayerStatusassitantReq->DataLen = 0;
	
	Z_SSHEAD_INIT(&stPkg.Head, BATTLE_STATUSASSISTANT_REQ);
	if(pststBattleCampInfo->BattleBusID <=0)
	{
		z_ssmsg_send(pstEnv, &stPkg, pstEnv->iBattlePveAutoteamID);
	}
	else
	{
		z_ssmsg_send(pstEnv, &stPkg, pststBattleCampInfo->BattleBusID);
	}
	return 0;
}

int player_do_battle_check_offline(ZONESVRENV* pstEnv,TEAMMEMBER *pstTeamMember,int eStatusAssistant)
{
	SSPKG stPkg;
	SSPLAYERSTATUSASSITANTSYNC *pstPlayerStatusassitantRes = &stPkg.Body.SSPlayerStatusassitantRes;

	if(INST_MODE_BATTLE_SVR != pstEnv->iInstMode)
	{
		return 0;
	}
	memset(pstPlayerStatusassitantRes,0,sizeof(SSPLAYERSTATUSASSITANTSYNC));
	
	pstPlayerStatusassitantRes->Uin = pstTeamMember->Uin;
	pstPlayerStatusassitantRes->RoleID = pstTeamMember->RoleID;
	pstPlayerStatusassitantRes->OperType = DO_STATUSASSITANT_SYNC;
	pstPlayerStatusassitantRes->eStatusAssistant = eStatusAssistant;
	pstPlayerStatusassitantRes->DataLen = 0;
	
	Z_SSHEAD_INIT(&stPkg.Head, BATTLE_STATUSASSISTANT_RES);
	z_ssmsg_send(pstEnv, &stPkg, pstTeamMember->ZoneBusID);

	return 0;
}

int player_battle_uin_role_cache_info(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	ROLELISTINFO *pstRoleListInfo = NULL;
	UinCache *pstUinCache;
	int iValid;

	pstUinCache = (UinCache *)sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos, &iValid);
	if (NULL == pstUinCache || 0 == iValid || pstUinCache->iUin != pstPlayer->stRoleData.Uin)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
					"uin %d get cache pos=%d error", pstPlayer->stRoleData.Uin, pstPlayer->iUinCachePos);
		return -1;
	}

	for (i=pstUinCache->ucRoleNum-1; i>=0; i--)
	{
		pstRoleListInfo = &pstUinCache->astRoleListInfo[i];
		if (pstRoleListInfo->RoleID == pstPlayer->stRoleData.RoleID)
		{
			break;
		}
		pstRoleListInfo = NULL;
	}
	
	if (pstUinCache->ucRoleNum >= ROLE_MAX_NUM)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
				"z_db_role_list_res uin=%d role list num %d", pstPlayer->stRoleData.Uin,
				pstUinCache->ucRoleNum);
		return -1;
	}
	
	if(NULL == pstRoleListInfo)
	{
		pstRoleListInfo = &pstUinCache->astRoleListInfo[pstUinCache->ucRoleNum];
		pstUinCache->ucRoleNum++;
	}
	
	memset(pstRoleListInfo,0,sizeof(ROLELISTINFO));	
	pstRoleListInfo->Uin = pstPlayer->stRoleData.Uin;
	pstRoleListInfo->RoleID = pstPlayer->stRoleData.RoleID;
	memcpy(pstRoleListInfo->RoleName,pstPlayer->stRoleData.RoleName,sizeof(pstRoleListInfo->RoleName));
	pstRoleListInfo->Gender = pstPlayer->stRoleData.Gender;
	pstRoleListInfo->Career = pstPlayer->stRoleData.Career;
	pstRoleListInfo->Head = pstPlayer->stRoleData.Head;
	pstRoleListInfo->Level = pstPlayer->stRoleData.Level;
	pstRoleListInfo->AvatarProf = pstPlayer->stRoleData.AvatarProf;
	pstRoleListInfo->Face = pstPlayer->stRoleData.Face;
	pstRoleListInfo->LastLogin = pstPlayer->stRoleData.LastLogin;
	pstRoleListInfo->Map = pstPlayer->stRoleData.Map;
	pstRoleListInfo->Hair = pstPlayer->stRoleData.Hair;
	pstRoleListInfo->DelTime = pstPlayer->stRoleData.DelTime;
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int player_battle2zone_action_request(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 CSACTIONRES *pstActionRes)
{
	SSPKG stPkg;
	SSBATTLEMIRRORSYNC *pstReq = &stPkg.Body.BattleMirrorSync;
	TDRDATA stHost, stNet;
	LPTDRMETA pstMeta = tdr_get_meta_by_name(pstEnv->pstNetHan->pstCSProtoLib,
	                                         "CSActionRes");
	if(!pstMeta)
	{
		return -1;
	}

	pstReq->Cmd = MIRROR_SYNC_CMD_ACTION_RES;
	pstReq->DataLen = sizeof(pstPlayer->stRoleData.Uin);
	memcpy(pstReq->Data, &pstPlayer->stRoleData.Uin,
	       sizeof(pstPlayer->stRoleData.Uin));

	stHost.pszBuff = (char *)pstActionRes;
	stHost.iBuff   = sizeof(*pstActionRes);

	stNet.pszBuff  = pstReq->Data + pstReq->DataLen;
	stNet.iBuff    = sizeof(pstReq->Data) - pstReq->DataLen;

	if(tdr_hton(pstMeta, &stNet, &stHost, 0) < 0)
	{
		return -1;
	}
	pstReq->DataLen += stNet.iBuff;

	Z_SSHEAD_INIT(&stPkg.Head, BATTLE_MIRROR_SYNC);
	return z_ssmsg_send(pstEnv, &stPkg,
	            pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.ZoneBusID);
}


int player_shadow_action_request(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 CSACTIONRES *pstActionRes)
{
	SSPKG stPkg;
	SSBATTLEMIRRORSYNC *pstReq = &stPkg.Body.BattleMirrorSync;
	TDRDATA stHost, stNet;
	LPTDRMETA pstMeta = tdr_get_meta_by_name(pstEnv->pstNetHan->pstCSProtoLib,
	                                         "CSActionRes");
	if(!pstMeta || pstPlayer->eStatus != PLAYER_STATUS_ROLE_ON_BATTLE)
	{
		return -1;
	}

	pstReq->Cmd = MIRROR_SYNC_CMD_ACTION_RES;
	pstReq->DataLen = sizeof(pstPlayer->stRoleData.Uin);
	memcpy(pstReq->Data, &pstPlayer->stRoleData.Uin,
	       sizeof(pstPlayer->stRoleData.Uin));

	stHost.pszBuff = (char *)pstActionRes;
	stHost.iBuff   = sizeof(*pstActionRes);

	stNet.pszBuff  = pstReq->Data + pstReq->DataLen;
	stNet.iBuff    = sizeof(pstReq->Data) - pstReq->DataLen;

	if(tdr_hton(pstMeta, &stNet, &stHost, 0) < 0)
	{
		return -1;
	}
	pstReq->DataLen += stNet.iBuff;

	Z_SSHEAD_INIT(&stPkg.Head, BATTLE_MIRROR_SYNC);
	return z_ssmsg_send(pstEnv, &stPkg,
	            pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.BattleBusID);
}

int player_shadow_action_res(ZONESVRENV *pstEnv, Player *pstPlayer,
                             char *pszRaw, int iLen)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	TDRDATA stHost, stNet;
	LPTDRMETA pstMeta = tdr_get_meta_by_name(
		pstEnv->pstNetHan->pstCSProtoLib, "CSActionRes");
	if(!pstMeta)
	{
		return -1;
	}

	stHost.pszBuff = (char *)pstActionRes;
	stHost.iBuff   = sizeof(*pstActionRes);

	stNet.pszBuff  = pszRaw;
	stNet.iBuff    = iLen;

	if(tdr_ntoh(pstMeta, &stHost, &stNet, 0) < 0)
	{
		return -1;
	}

	pstActionRes->ID = pstPlayer->iMemID; // Ҫ�滻�ɱ��ص�Memory ID,�м�
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	return z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg,
	                            0);
}
int player_check_same_world(ZONESVRENV* pstEnv,char *pszFullName)
{
	int iLen = 0;
	char szBattleName[256];
	char szPlayerBattleFix[256];
	unsigned short unWorldID = 0;
	
	char *pszReal = strstr(pszFullName, BATTLE_NAME_SYMBOL);
	if(!pszReal)
	{
		return 0;
	}

	++pszReal; // �����Ǹ�$����
	iLen = strlen(pszReal);
	

	memset(szBattleName,0,sizeof(szBattleName));
	memset(szPlayerBattleFix,0,sizeof(szPlayerBattleFix));

	if( strlen(pszFullName)-iLen>0 &&
		strlen(pszFullName)-iLen < sizeof(szPlayerBattleFix))
	{
		memcpy(szPlayerBattleFix,pszFullName,strlen(pszFullName)-iLen);
	}
	else
	{
		return 0;
	}

	GET_WORLD_ID(unWorldID,pstEnv->pstAppCtx->iId);
	snprintf(szBattleName,sizeof(szBattleName),"��%d"BATTLE_NAME_SYMBOL,unWorldID);

	if(strcmp(szBattleName, szPlayerBattleFix) == 0)
	{
		return 0;
	}

	return -1;
}

int player_battle_get_memid(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode  &&
		pstPlayer->eStatus!=PLAYER_STATUS_ROLE_LOGIN)
	{
		return pstPlayer->stOnline.ZoneMemID;
	}
	else
	{
		return pstPlayer->iMemID;
	}
}
// ��ս���еĽ�ɫ���� "��%d$%s",��$ǰ׺ȥ��
int battle_rolename_remove_prefix(char *pszFullName)
{
	int iLen = 0;
	char *pszReal = strstr(pszFullName, BATTLE_NAME_SYMBOL);
	if(!pszReal)
	{
		return 0;
	}
	++pszReal; // �����Ǹ�$����
	iLen = strlen(pszReal);
	memmove(pszFullName, pszReal, iLen + 1);
	return iLen;
}

int player_check_same_world_name(char *pszName1,char *pszName2)
{
	int iLen1 = 0;
	int iLen2 = 0;
	char szPlayerFix2[256];
	char szPlayerFix1[256];
	char *pszReal1;
	char *pszReal2;
	
	pszReal1 = strstr(pszName1, BATTLE_NAME_SYMBOL);
	if(!pszReal1)
	{
		return 0;
	}
	pszReal2 = strstr(pszName2, BATTLE_NAME_SYMBOL);
	if(!pszReal2)
	{
		return 0;
	}
	++pszReal1; // �����Ǹ�$����
	iLen1 = strlen(pszReal1);
	++pszReal2; 
	iLen2 = strlen(pszReal2);
	

	memset(szPlayerFix2,0,sizeof(szPlayerFix2));
	memset(szPlayerFix1,0,sizeof(szPlayerFix1));

	if( strlen(pszName1)-iLen1>0 &&
		strlen(pszName1)-iLen1 < sizeof(szPlayerFix1))
	{
		memcpy(szPlayerFix1,pszName1,strlen(pszName1)-iLen1);
	}
	else
	{
		return 0;
	}

	if( strlen(pszName2)-iLen2>0 &&
		strlen(pszName2)-iLen2 < sizeof(szPlayerFix2))
	{
		memcpy(szPlayerFix2,pszName2,strlen(pszName2)-iLen2);
	}
	else
	{
		return 0;
	}
	
	if(strcmp(szPlayerFix1, szPlayerFix2) == 0)
	{
		return 0;
	}

	return -1;
}
void role_save_protected_data(const ROLEDATA *pstRoleData,ProtectedRoleData *pstSavedData)
{
	// ������Ϣ
	pstSavedData->ullTeamID = pstRoleData->MiscInfo.TeamID;
	pstSavedData->iTeamIdx = pstRoleData->MiscInfo.TeamIdx;

	// ������Ϣ
	pstSavedData->stClanInfo = pstRoleData->MiscInfo.ClanInfo;

	// ��ɫ��
	STRNCPY(pstSavedData->szRoleName, pstRoleData->RoleName,
	        CCH(pstSavedData->szRoleName));

	// ��ϵϵͳ
	pstSavedData->ucBlackNum = pstRoleData->MiscInfo.BlackNum;
	memcpy(pstSavedData->astBlackMember, pstRoleData->MiscInfo.BlackMember,
	       sizeof(pstSavedData->astBlackMember[0]) * pstSavedData->ucBlackNum);
	memcpy(&pstSavedData->stEnemy, &pstRoleData->MiscInfo.Enemy,
	       sizeof(pstSavedData->stEnemy));
	memcpy(&pstSavedData->stBuddy, &pstRoleData->Buddy,
	       sizeof(pstSavedData->stBuddy));
}

void role_restore_protected_data(const ProtectedRoleData *pstSavedData,ROLEDATA *pstRoleData)
{
	// ������Ϣ
	pstRoleData->MiscInfo.TeamID = pstSavedData->ullTeamID;
	pstRoleData->MiscInfo.TeamIdx = pstSavedData->iTeamIdx;

	// ������Ϣ
	pstRoleData->MiscInfo.ClanInfo = pstSavedData->stClanInfo;

	// ��ɫ��
	STRNCPY(pstRoleData->RoleName, pstSavedData->szRoleName,
	        CCH(pstRoleData->RoleName));

	// ��ϵϵͳ
	pstRoleData->MiscInfo.BlackNum = pstSavedData->ucBlackNum;
	memcpy(pstRoleData->MiscInfo.BlackMember, pstSavedData->astBlackMember,
	       sizeof(pstRoleData->MiscInfo.BlackMember[0]) *
	       pstSavedData->ucBlackNum);
	memcpy(&pstRoleData->MiscInfo.Enemy, &pstSavedData->stEnemy,
	       sizeof(pstRoleData->MiscInfo.Enemy));
	memcpy(&pstRoleData->Buddy, &pstSavedData->stBuddy,
	       sizeof(pstRoleData->Buddy));
}


ZoneTeam* team_shadow_by_id(ZONESVRENV *pstEnv, tdr_ulonglong ullID)
{
	WID2Team stDummy, *pstFound = NULL;

	stDummy.ullID = ullID;
	pstFound = sht_find(pstEnv->pstShtWID2Team, &stDummy, WID2TeamHashCmp,
	                    WID2TeamHashCode);
	if(!pstFound)
	{
		return NULL;
	}

	return tmempool_get(pstEnv->pstTeamPool, pstFound->iIdx);
}

int team_shadow_insert(ZONESVRENV *pstEnv, const ZoneTeam *pstZoneTeam)
{
	WID2Team stDummy;
	WID2Team *pstInserted = NULL;

	if( INST_MODE_BATTLE_SVR != pstEnv->iInstMode)
	{
		return -1;
	}
	stDummy.ullID = pstZoneTeam->stTeamInfo.TeamID;
	stDummy.iIdx  = pstZoneTeam->iTeamIdx;

	pstInserted = sht_insert_unique(pstEnv->pstShtWID2Team, &stDummy,
	                                WID2TeamHashCmp, WID2TeamHashCode);
	if(!pstInserted)
	{
		return -1;
	}
	*pstInserted = stDummy;
	return 0;
}

int team_shadow_delete(const ZONESVRENV *pstEnv, tdr_ulonglong ullID)
{
	WID2Team stDummy;
	const WID2Team *pstRemoved = NULL;

	stDummy.ullID = ullID;
	stDummy.iIdx  = -1;
	
	pstRemoved = sht_remove(pstEnv->pstShtWID2Team, &stDummy,
	                        WID2TeamHashCmp, WID2TeamHashCode);
	if(!pstRemoved)
	{
		return -1;
	}

	tmempool_free(pstEnv->pstTeamPool, pstRemoved->iIdx);
	return 0;
}

int team_shadow_find_or_new(ZONESVRENV *pstEnv, tdr_ulonglong ullID,
                                   ZoneTeam **ppstZoneTeam)
{
	int iAlloced = -1;
	ZoneTeam *pstFound = team_shadow_by_id(pstEnv, ullID);

	if(pstFound)
	{
		*ppstZoneTeam = pstFound;
		return 1; // ���ҵ�
	}

	iAlloced = tmempool_alloc(pstEnv->pstTeamPool);
	if(iAlloced < 0)
	{
		return -1; // �ڴ�ľ�
	}

	pstFound = tmempool_get(pstEnv->pstTeamPool, iAlloced);
	memset(pstFound, 0, sizeof(*pstFound));

	pstFound->iTeamIdx = iAlloced;
	pstFound->stTeamInfo.TeamID = ullID;

	if(team_shadow_insert(pstEnv, pstFound) < 0)
	{
		return -1; // key��ͻ��
	}
	*ppstZoneTeam = pstFound;
	return 0; // �½���һ��
}

int team_shadow_destroy_respond(ZONESVRENV *pstEnv, int iMsgSrc,
                                tdr_ulonglong ullID)
{
	int i=0;
	Player *pstPlayer;
	ZoneTeam *pstZoneTeam = team_shadow_by_id(pstEnv, ullID);
	if(!pstZoneTeam)
	{
		return -1;
	}

	if( (pstZoneTeam->stAutoTeamInfo.AutoTeamID >0 &&
		pstZoneTeam->stAutoTeamInfo.tStart == 0) ||
		(pstZoneTeam->eStatusAssistant&PLAYER_STATUS_BATTLE_TEAM_SYNC))
	{
		return -1;
	}

	if(is_battle_pve_autoteam_svr(pstEnv))
	{
		for(i=0;i<pstZoneTeam->stTeamInfo.MemberNum && i<MAX_TEAM_MEMBER;i++)
		{
			pstPlayer = player_get_by_uin(pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].Uin);
			if( pstPlayer == NULL )
			{
				continue;
			}
			if(pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
			{
				return -1;
			}
		}

		autoteam_team_chg_reject(pstEnv, pstZoneTeam, 1);
	}
	
	return team_shadow_delete(pstEnv, ullID);
	UNUSED(&iMsgSrc);
}

int team_shadow_exit_req(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam)
{
	SSPKG stPkg;
	SSTEAMSHADOWEXITREQ *pstTeamShadowExitReq = &stPkg.Body.TeamShadowExitReq;

	if(pstZoneTeam->BattleBusID<=0)
	{
		return 0;
	}
	
	pstTeamShadowExitReq->TeamID = pstZoneTeam->stTeamInfo.TeamID;
	pstTeamShadowExitReq->TeamIdx = pstZoneTeam->iTeamIdx;

	
	Z_SSHEAD_INIT(&stPkg.Head, TEAM_SHADOW_EXIT_REQ);
	z_ssmsg_send(pstEnv, &stPkg, pstZoneTeam->BattleBusID);

	pstZoneTeam->BattleBusID = 0;
	pstZoneTeam->eStatusAssistant = 0;
	
	return 0;
}

int team_shadow_exit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,
                      int iMsgSrc, SSTEAMSHADOWEXITREQ *pstTeamShadowExitReq)
{
	SSPKG stPkg;
	SSTEAMSHADOWEXITRES *pstTeamShadowExitRes = &stPkg.Body.TeamShadowExitRes;
	ZoneTeam *pstFound = team_shadow_by_id(pstEnv, pstTeamShadowExitReq->TeamID);
	if(!pstFound)
	{
		goto fini;
	}
	team_shadow_destroy_respond(pstEnv, iMsgSrc,pstTeamShadowExitReq->TeamID);
	
fini:
	pstTeamShadowExitRes->Result = 0;
	pstTeamShadowExitRes->TeamID = pstTeamShadowExitReq->TeamID;
	pstTeamShadowExitRes->TeamIdx = pstTeamShadowExitReq->TeamIdx;
	
	Z_SSHEAD_INIT(&stPkg.Head, TEAM_SHADOW_EXIT_RES);
	z_ssmsg_send(pstEnv, &stPkg, iMsgSrc);
	
	return 0;
	UNUSED(pstAppCtx);
}

int team_shadow_exit_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,SSTEAMSHADOWEXITRES *pstTeamShadowExitRes)
{
	ZoneTeam *pstZoneTeam = NULL;

	if(pstTeamShadowExitRes->Result != 0)
	{
		pstZoneTeam = z_idx_team(pstEnv, pstTeamShadowExitRes->TeamIdx, pstTeamShadowExitRes->TeamID);
		if(pstZoneTeam)
		{
			pstZoneTeam->BattleBusID = 0;
			pstZoneTeam->eStatusAssistant = 0;
		}
	}
	return 0;
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	UNUSED(pstTeamShadowExitRes);
}

int team_shadow_sync_zone(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam,int eStatus,int iBusID)
{
	if( pstZoneTeam->BattleBusID>0 && 
		iBusID != pstZoneTeam->BattleBusID )
	{
		//����ȡ��
		team_shadow_exit_req(pstEnv, pstZoneTeam);
	}

	pstZoneTeam->ZoneBusID = pstEnv->pstAppCtx->iId;
	pstZoneTeam->BattleBusID = iBusID;
	pstZoneTeam->eStatusAssistant |= eStatus;
	team_shadow_sync_req(pstEnv, pstZoneTeam->BattleBusID,pstZoneTeam);

	return 0;
}


int team_shadow_sync_req(ZONESVRENV *pstEnv, int iBusID,
                                    ZoneTeam *pstZoneTeam)
{
	int i = 0;
	SSPKG stPkg;
	SSBATTLEMIRRORSYNC *pstReq = &stPkg.Body.BattleMirrorSync;
	TDRDATA stHost, stNet;
	Player *pstOne = NULL;
	SSBATTLECAMPINFO *pstBattleCampInfo = NULL;
	LPTDRMETA pstMeta = tdr_get_meta_by_name(
		pstEnv->pstNetHan->pstCSProtoLib, "TeamInfo");
	if(!pstMeta)
	{
		return -1;
	}

	if(pstZoneTeam->eStatusAssistant&PLAYER_STATUS_BATTLE_TEAM_SYNC)
	{
		return -1;
	}

	//���Ұ�⣬����ǰվλ�ã�ֻͬ���������ȥ���ڵ������ز���
	if(iBusID != pstEnv->iBattlePvpID)
	{
		for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
		{
			pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
			if (!pstOne)
			{
				continue;
			}
			pstBattleCampInfo = &pstOne->stOnline.stPworldQueue.stBattleCampInfo;
			if( pstBattleCampInfo->BattleBusID == 0 && 
				pstOne->eStatus == PLAYER_STATUS_ROLE_LOGIN )
			{
				player_battle_enter_req(pstEnv->pstAppCtx,pstEnv,&pstOne->stOnline.stFrameHead,
									pstOne,PLAYER_STATUS_BATTLE_LOGIN_READY,pstEnv->iBattlePveAutoteamID);
				
			}
		}
	}
	
	pstReq->Cmd = MIRROR_SYNC_CMD_TEAM;
	pstReq->DataLen = 0;
	memcpy(pstReq->Data, &pstZoneTeam->stTeamInfo.TeamID, sizeof(pstZoneTeam->stTeamInfo.TeamID));
	pstReq->DataLen += sizeof(pstZoneTeam->stTeamInfo.TeamID);
	memcpy(pstReq->Data+pstReq->DataLen, &pstZoneTeam->iTeamIdx, sizeof(pstZoneTeam->iTeamIdx));
	pstReq->DataLen += sizeof(pstZoneTeam->iTeamIdx);
	memcpy(pstReq->Data+pstReq->DataLen, &pstZoneTeam->eStatusAssistant, sizeof(pstZoneTeam->eStatusAssistant));
	pstReq->DataLen += sizeof(pstZoneTeam->eStatusAssistant);
	

	stHost.pszBuff = (char *)&pstZoneTeam->stTeamInfo;
	stHost.iBuff   = sizeof(pstZoneTeam->stTeamInfo);

	stNet.pszBuff  = pstReq->Data + pstReq->DataLen;
	stNet.iBuff    = sizeof(pstReq->Data) - pstReq->DataLen;

	if(tdr_hton(pstMeta, &stNet, &stHost, 0) < 0)
	{
		return -1;
	}
	pstReq->DataLen += stNet.iBuff;

	Z_SSHEAD_INIT(&stPkg.Head, BATTLE_MIRROR_SYNC);
	return z_ssmsg_send(pstEnv, &stPkg, iBusID);
}
int team_shadow_sync(ZONESVRENV *pstEnv, int iMsgSrc,
                             tdr_ulonglong ullTeamID, char *pszRaw, int iLen)
{
	int i = 0, fFound = 0;
	int j = 0;
	ZoneTeam *pstZoneTeam = NULL;
	unsigned short unWorldID = 0;
	TDRDATA stHost, stNet;
	int iErr = 0;
	int iTeamIdx = 0;
	int eStatusAssistant = 0; //�����״̬
	int iOffset = 0;
	SSPKG stPkg;
	SSTEAMSHADOWSYNCRES *pstTeamShadowSyncRes = &stPkg.Body.TeamShadowSyncRes;
	Player *pstPlayer = NULL;
	TEAMINFO stTeamInfo;
	
	LPTDRMETA pstMeta = tdr_get_meta_by_name(
		pstEnv->pstNetHan->pstCSProtoLib, "TeamInfo");
	
	if(!pstMeta)
	{
		return -1;
	}
	
	memcpy(&iTeamIdx,pszRaw+iOffset,sizeof(int));
	iOffset += sizeof(int);
	memcpy(&eStatusAssistant,pszRaw+iOffset,sizeof(int));
	iOffset += sizeof(int);

	if(iLen<iOffset)
	{
		return -1;
	}

	fFound = team_shadow_find_or_new(pstEnv, ullTeamID, &pstZoneTeam);
	if(fFound < 0 || !pstZoneTeam)
	{
		team_shadow_delete(pstEnv, ullTeamID);
	}
	fFound = team_shadow_find_or_new(pstEnv, ullTeamID, &pstZoneTeam);
	if(fFound < 0 || !pstZoneTeam)
	{
		team_shadow_delete(pstEnv, ullTeamID);
		pstTeamShadowSyncRes->Result = BATTLE_ERR_SYSTEM;
		pstTeamShadowSyncRes->eStatusAssistant = 0;
		goto fini;
	}
	else if(fFound == 1)
	{
		stTeamInfo = pstZoneTeam->stTeamInfo;
	}
	else if(fFound == 0)
	{
		if(eStatusAssistant&PLAYER_STATUS_BATTLE_TEAM_SYNC)
		{
			pstTeamShadowSyncRes->Result = BATTLE_ERR_SYSTEM;
			pstTeamShadowSyncRes->eStatusAssistant = 0;
			goto fini;
		}
		else
		{
			pstZoneTeam->eStatusAssistant = 0;
		}
	}

	pstZoneTeam->ZoneBusID = iMsgSrc;
	
	if( (pstZoneTeam->stAutoTeamInfo.AutoTeamID >0 &&
		pstZoneTeam->stAutoTeamInfo.tStart == 0) ||
		(pstZoneTeam->eStatusAssistant&PLAYER_STATUS_BATTLE_TEAM_SYNC))
	{
		pstTeamShadowSyncRes->Result = BATTLE_ERR_TEAM_NO_SYNC;
		pstTeamShadowSyncRes->eStatusAssistant = pstZoneTeam->eStatusAssistant;
	}
	else 
	{
		// ���
		stHost.pszBuff = (char *)&pstZoneTeam->stTeamInfo;
		stHost.iBuff   = sizeof(pstZoneTeam->stTeamInfo);

		stNet.pszBuff  = pszRaw+iOffset;
		stNet.iBuff    = iLen-iOffset;

		iErr = tdr_ntoh(pstMeta, &stHost, &stNet, 0);
		if(iErr < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tdr_ntoh fail, reason : %s",
				       tdr_error_string(iErr));
			return -1;
		}

		pstZoneTeam->iZoneTeamIdx = iTeamIdx;
		pstZoneTeam->ullZoneTeamID = ullTeamID;
		
		// �ѽ�ɫ���ָĹ���װ���Ǳ��ؽ�ɫ
		GET_WORLD_ID(unWorldID, iMsgSrc);
		i = pstZoneTeam->stTeamInfo.MemberNum;
		while(i--)
		{
			char szTmp[ROLE_NAME_LEN];

			snprintf(szTmp, CCH(szTmp), "��%d"BATTLE_NAME_SYMBOL"%s",
			         unWorldID, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
			STRNCPY(pstZoneTeam->stTeamInfo.TeamMember[i].MemberName, szTmp,
			        CCH(pstZoneTeam->stTeamInfo.TeamMember[i].MemberName));

			pstPlayer = player_get_by_uin(pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].Uin);
			if(pstPlayer)
			{
				pstZoneTeam->stTeamInfo.TeamMember[i].MemID = pstPlayer->iMemID;
			}
			else
			{
				pstZoneTeam->stTeamInfo.TeamMember[i].MemID = 0;
			}
			
			if( pstPlayer )
			{
				if(pstPlayer->stRoleData.RoleID != pstZoneTeam->stTeamInfo.TeamMember[i].RoleID)
				{
					team_shadow_delete(pstEnv, ullTeamID);
					pstTeamShadowSyncRes->Result = BATTLE_ERR_ACC_MUL_ROLE;
					goto fini;
				}

				if( pstPlayer->stOnline.eStatusAssistant&PLAYER_STATUS_BATTLE_TEAM_SYNC)
				{
					team_shadow_delete(pstEnv, ullTeamID);
					pstTeamShadowSyncRes->Result = BATTLE_ERR_SYSTEM;
					pstTeamShadowSyncRes->eStatusAssistant = 0;
					goto fini;
				}
				
				if( pstPlayer->stRoleData.MiscInfo.TeamID > 0 &&
					pstPlayer->stRoleData.MiscInfo.TeamID != pstZoneTeam->stTeamInfo.TeamID)
				{
					if(!is_battle_pvp_svr(pstEnv))
					{
						player_team_exit(pstEnv, pstPlayer,NULL);
					}
				}
				pstPlayer->stRoleData.MiscInfo.TeamID = pstZoneTeam->stTeamInfo.TeamID;
			}
		}

		if(is_battle_pvp_svr(pstEnv))
		{
			pstTeamShadowSyncRes->Result = 0;	
			goto fini;
		}
		
		//�¼����
		i = pstZoneTeam->stTeamInfo.MemberNum;
		while( i-- && (fFound == 1) )
		{
			pstPlayer = player_get_by_uin(pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].Uin);
			if(!pstPlayer)
			{
				continue;
			}
			for(j=0;j<stTeamInfo.MemberNum;j++)
			{
				if(pstPlayer->stRoleData.Uin == stTeamInfo.TeamMember[j].Uin)
				{
					break;
				}
			}
			
			//�¼������
			if (j>=stTeamInfo.MemberNum && pstZoneTeam->stAutoTeamInfo.tStart)
			{
				if(autoteam_team_add_player_check(pstEnv, pstZoneTeam, pstPlayer)<0)
				{
					autoteam_team_chg_reject(pstEnv, pstZoneTeam, 3);
				}
				else
				{
					autoteam_team_chg_allow(pstEnv, pstZoneTeam);
				}
			}
		}

		//����������
		if(stTeamInfo.MemberNum > pstZoneTeam->stTeamInfo.MemberNum)
		{
			i = stTeamInfo.MemberNum;
			while( i-- && (fFound == 1) )
			{
				pstPlayer = player_get_by_uin(pstEnv, stTeamInfo.TeamMember[i].Uin);
				if(!pstPlayer)
				{
					continue;
				}
				
				for(j=0;j<pstZoneTeam->stTeamInfo.MemberNum;j++)
				{
					if(pstPlayer->stRoleData.Uin == pstZoneTeam->stTeamInfo.TeamMember[j].Uin)
					{
						break;
					}
				}

				if (j>=pstZoneTeam->stTeamInfo.MemberNum )
				{
					autoteam_notify_clt_clean(pstEnv, pstPlayer);
					if(pstPlayer->stRoleData.MiscInfo.TeamID == pstZoneTeam->stTeamInfo.TeamID)
					{
						pstPlayer->stRoleData.MiscInfo.TeamID = 0;
					}
				}
				
			}	
		}

		if(stTeamInfo.MemberNum != pstZoneTeam->stTeamInfo.MemberNum)
		{
			autoteam_team_chg_allow(pstEnv, pstZoneTeam);
		}
		autoteam_refresh_team(pstEnv, pstZoneTeam,1);
		pstTeamShadowSyncRes->Result = 0;	
		
	}
	
fini:	
	pstTeamShadowSyncRes->TeamID = ullTeamID;
	pstTeamShadowSyncRes->TeamIdx = iTeamIdx;
	
	Z_SSHEAD_INIT(&stPkg.Head, TEAM_SHADOW_SYNC_RES);
	z_ssmsg_send(pstEnv, &stPkg, iMsgSrc);
	return 0;
}


int team_shadow_sync_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,SSTEAMSHADOWSYNCRES *pstTeamShadowSyncRes)
{
	ZoneTeam *pstZoneTeam = NULL;
	TMEMBLOCK *pstMemBlock = NULL;
	
	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, pstTeamShadowSyncRes->TeamIdx);
	if (0 == pstMemBlock->fValid)
	{
		return -1;
	}
	pstZoneTeam= (ZoneTeam *)pstMemBlock->szData;

	if(pstZoneTeam->stTeamInfo.TeamID != pstTeamShadowSyncRes->TeamID)
	{
		return -1;
	}

	//��ս��״̬,�����Զ�����ܸı����״̬
	if(pstTeamShadowSyncRes->Result == BATTLE_ERR_TEAM_NO_SYNC)
	{
		pstZoneTeam->eStatusAssistant = pstTeamShadowSyncRes->eStatusAssistant;
		if(pstTeamShadowSyncRes->eStatusAssistant&PLAYER_STATUS_BATTLE_TEAM_SYNC)
		{
			player_team_destroy_in(pstEnv, pstZoneTeam, 2);
		}
	}
	else if( pstTeamShadowSyncRes->Result == BATTLE_ERR_ACC_MUL_ROLE)
	{
		player_team_broadcast_tip(pstEnv, pstZoneTeam, "�ж����Ա���ʺ��Ѿ���ս����");
		pstZoneTeam->eStatusAssistant = 0;
		pstZoneTeam->BattleBusID = 0;
	}
	else if(pstTeamShadowSyncRes->Result != 0)
	{
		pstZoneTeam->eStatusAssistant = 0;
		pstZoneTeam->BattleBusID = 0;
	}
	
	return 0;
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
}

int team_shadow_statusassitant_req(ZONESVRENV* pstEnv,ZoneTeam *pstZoneTeam)
{
	SSPKG stPkg;
	SSTEAMSHADOWSYNCRES *pstTeamShadowSyncRes = &stPkg.Body.TeamShadowSyncRes;

	if( pstZoneTeam->ullZoneTeamID > 0 &&
		pstZoneTeam->ZoneBusID > 0 )
	{
		pstTeamShadowSyncRes->Result = BATTLE_ERR_TEAM_NO_SYNC;
		pstTeamShadowSyncRes->eStatusAssistant = pstZoneTeam->eStatusAssistant;
		pstTeamShadowSyncRes->TeamIdx = pstZoneTeam->iZoneTeamIdx;
		pstTeamShadowSyncRes->TeamID = pstZoneTeam->ullZoneTeamID;
			
		Z_SSHEAD_INIT(&stPkg.Head, TEAM_SHADOW_SYNC_RES);
		z_ssmsg_send(pstEnv, &stPkg, pstZoneTeam->ZoneBusID);
	}
	return 0;
}

int team_create_succ_req(ZONESVRENV* pstEnv,ZoneTeam *pstZoneTeam,Player *pstPlayer)
{
	SSPKG stPkg;
	SSBATTLETEAMSUCCREQ *pstBattleTeamSuccReq = &stPkg.Body.BattleTeamSuccReq;
	SSBATTLECAMPINFO *pststBattleCampInfo = &pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;
	
	memset(pstBattleTeamSuccReq,0,sizeof(SSBATTLETEAMSUCCREQ));
	
	pstBattleTeamSuccReq->Uin = pstPlayer->stRoleData.Uin;
	pstBattleTeamSuccReq->RoleID = pstPlayer->stRoleData.RoleID;
	pstBattleTeamSuccReq->TeamID= pstZoneTeam->stTeamInfo.TeamID;
	pstBattleTeamSuccReq->TeamIdx= pstZoneTeam->iTeamIdx;

	
	Z_SSHEAD_INIT(&stPkg.Head, BATTLE_TEAM_SUCC_REQ);
	z_ssmsg_send(pstEnv, &stPkg, pststBattleCampInfo->ZoneBusID);

	return 0;
}
int team_create_succ(ZONESVRENV* pstEnv,SSBATTLETEAMSUCCREQ *pstBattleTeamSuccReq)
{
	Player *pstPlayer = NULL;

	pstPlayer = player_get_by_uin(pstEnv, pstBattleTeamSuccReq->Uin);
	if( pstPlayer == NULL )
	{
		return -1;
	}

	if(pstPlayer->stRoleData.RoleID != pstBattleTeamSuccReq->RoleID)
	{
		return -1;
	}

	if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		return -1;
	}

	pstPlayer->stRoleData.MiscInfo.SpanSvrTeamID = pstBattleTeamSuccReq->TeamID;
	pstPlayer->stRoleData.MiscInfo.SpanSvrTeamIdx= pstBattleTeamSuccReq->TeamIdx;
	
	return 0;
}

int team_shadow_chg_true_team(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam)
{
	Player *pstOne;
	int i;
	
	if( INST_MODE_BATTLE_SVR != pstEnv->iInstMode )
	{
		return -1;
	}

	pstZoneTeam->eStatusAssistant |= PLAYER_STATUS_BATTLE_TEAM_SYNC;
	team_shadow_statusassitant_req(pstEnv,pstZoneTeam);

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstOne)
		{
			pstOne->stOnline.eStatusAssistant |= PLAYER_STATUS_BATTLE_TEAM_SYNC;
			player_do_statusassitant_sync(pstEnv,pstOne);
			team_create_succ_req(pstEnv,pstZoneTeam,pstOne);
		}
	}

	return 0;
}

int is_player_in_battle_autoteam_svr(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	SSBATTLECAMPINFO *pstBattleCampInfo = &pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;
	
	if( INST_MODE_ZONE_SVR != pstEnv->iInstMode )
	{
		return 0;
	}
	
	if(pstBattleCampInfo->BattleBusID == pstEnv->iBattlePveAutoteamID)
	{
		return 1;
	}

	return 0;
}

int is_player_in_battle_pve_svr(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	SSBATTLECAMPINFO *pstBattleCampInfo = &pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;
	
	if( INST_MODE_ZONE_SVR != pstEnv->iInstMode )
	{
		return 0;
	}
	
	if(pstBattleCampInfo->BattleBusID == pstEnv->iBattlePveBattleID)
	{
		return 1;
	}

	return 0;
}

int is_player_in_battle_pvp_svr(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	SSBATTLECAMPINFO *pstBattleCampInfo = &pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;
	
	if( INST_MODE_ZONE_SVR != pstEnv->iInstMode )
	{
		return 0;
	}
	
	if(pstBattleCampInfo->BattleBusID == pstEnv->iBattlePvpID)
	{
		return 1;
	}

	return 0;
}

int player_enter_battle_pvp_map_req(ZONESVRENV* pstEnv, Player *pstPlayer, int iMapID)
{
	MapIndex *pstMapIdx;
	ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);
	SSBATTLECAMPINFO *pstBattleCampInfo = &pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;
	
	if(INST_MODE_ZONE_SVR != pstEnv->iInstMode ||
		pstEnv->pstConf->BattleFlag == 0)
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, iMapID);
	if (NULL == pstMapIdx || pstMapIdx->stMapDef.Span == 0)
	{
		return -1;
	}

	if(pstTeam)
	{	
		if(0 > player_battle_enter_req(pstEnv->pstAppCtx, pstEnv, &pstPlayer->stOnline.stFrameHead,
					pstPlayer, PLAYER_STATUS_BATTLE_LOGIN_READY, pstEnv->iBattlePvpID))
		{
			return -1;
		}
		
		team_shadow_sync_zone(pstEnv, pstTeam, PLAYER_STATUS_BATTLE_LOGIN_READY, pstEnv->iBattlePvpID);
	}
	
	pstBattleCampInfo->MapID = iMapID;
	if(0 > player_battle_enter_req(pstEnv->pstAppCtx, pstEnv, &pstPlayer->stOnline.stFrameHead,
					pstPlayer, PLAYER_STATUS_BATTLE_LOGIN_ON, pstEnv->iBattlePvpID))
	{
		return -1;
	}
	
	return 0;
}

int player_enter_battle_pvp_map(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	MapIndex *pstMapIdx;
	RESPOS stPos;
	SSBATTLECAMPINFO *pstBattleCampInfo = &pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, pstBattleCampInfo->MapID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}
		
	stPos = pstMapIdx->stMapDef.RevivePs[0];
	if (stPos.X <= 0 || stPos.Y <= 0 || !z_valid_pos(pstEnv->pstMapObj, pstBattleCampInfo->MapID, NULL, &stPos))
	{
		return -1;
	}

	if (0 > z_player_change_map(pstEnv->pstAppCtx, pstEnv, 
			pstPlayer, pstBattleCampInfo->MapID, &stPos, RAND1(180), 0))
	{ 
		return -1;
	}

	return 0;
}

int player_pvp_svr_member_info_sync_req(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	SSPKG stPkg;
	SSBATTLEMEMBERINFO *pstMemberInfo = &stPkg.Body.BattleMemberInfo;
	SSBATTLECAMPINFO *pststBattleCampInfo = &pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;
	
	if(!is_battle_pvp_svr(pstEnv))
	{
		return -1;
	}

	pstMemberInfo->Uin = pstPlayer->stRoleData.Uin;
	pstMemberInfo->RoleID = pstPlayer->stRoleData.RoleID;
	player_team_sync_member_info(pstEnv, &pstMemberInfo->Member, pstPlayer);
	battle_rolename_remove_prefix(pstMemberInfo->Member.MemberName);
			
	Z_SSHEAD_INIT(&stPkg.Head, BATTLE_TEAM_MEMBER_SYNC);
	z_ssmsg_send(pstEnv, &stPkg, pststBattleCampInfo->ZoneBusID);
	return 0;
}

int player_pvp_svr_member_info_sync(ZONESVRENV* pstEnv, SSBATTLEMEMBERINFO *pstSSMemberInfo)
{
	Player *pstPlayer;
	ZoneTeam *pstTeam;
	TEAMMEMBER *pstTeamMember;
	int iOld;
	
	pstPlayer = player_get_by_uin(pstEnv, pstSSMemberInfo->Uin);
	if( pstPlayer == NULL  || pstPlayer->stRoleData.RoleID != pstSSMemberInfo->RoleID)
	{
		return -1;
	}

	if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_ON_BATTLE)
	{
		return -1;
	}

	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(NULL == pstTeam)
	{
		return -1;
	}

	pstTeamMember = player_team_get_member(pstTeam, pstPlayer->stRoleData.RoleName, NULL);
	if (NULL == pstTeamMember)
	{
		return -1;
	}

	iOld = pstTeamMember->Position;
	*pstTeamMember = pstSSMemberInfo->Member;
	pstTeamMember->Position = iOld;
	return 0;
}
#endif
