

#include "zone_clt.h"
#include "zone_db.h"
#include "zone_safelock.h"
#include "zone_player.h"
#include "zone_span.h"
#include "zone_task.h"
#include "zone_err.h"

int player_check_grabsafe(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if (	pstPlayer->iGrabTime > 0 &&
		pstEnv->pstAppCtx->stCurr.tv_sec < pstPlayer->iGrabTime)
	{
		return -1;
		
	}
	return 0;
}

int player_check_safelock_in(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if (	(pstPlayer->stOnline.cSafeLockFalg & SAFE_LOCK_ACTIVE_FALG) !=0 &&
		(pstPlayer->stOnline.cSafeLockFalg & SAFE_LOCK_PASS_FLAG) ==0)
	{
		return -1;
	}
	return 0;
	UNUSED(pstEnv);
}


int player_check_safelock(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if (player_check_grabsafe(pstEnv, pstPlayer) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
				ZONEERR_SAFE201,GRAB_SAFE_TIME);
		return -1;
	}
	
	if (player_check_safelock_in(pstEnv, pstPlayer) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SAFE202);		
		return -1;
	}
	return 0;
}

int player_safelock_login(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if (!(pstPlayer->stOnline.cMiscFlag &MISC_FLAG_SEND_LOCK_SAFE))
	{
		ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;
		pstPlayer->stOnline.cSafeLockFalg= 0;
		
		if (pstMisc->LockPassWD[0]!=0)
		{
			pstPlayer->stOnline.cSafeLockFalg |= SAFE_LOCK_ACTIVE_FALG;
		}

		player_lock_safe_info(pstEnv, pstPlayer, pstPlayer->stOnline.cSafeLockFalg);
		
		pstPlayer->stOnline.cMiscFlag |=MISC_FLAG_SEND_LOCK_SAFE;
	}

	if (!(pstPlayer->stOnline.cSafeLockFalg & SAFE_LOCK_ACTIVE_FALG))
	{
		pstPlayer->UinFlag &= ~UIN_FLAG_SAFE_PASSWD;
	}
	
	if (pstPlayer->UinFlag & UIN_FLAG_SAFE_PASSWD ||
		pstPlayer->stOnline.cSafeLockFalg & SAFE_LOCK_ACTIVE_FALG)
	{
		pstPlayer->UinFlag |= UIN_FLAG_SAFE_PASSWD;
		
		task_player_trace(pstEnv,pstPlayer,TASK_OBJ_SET_SAFE_PWD);
	}

	//借个地方
	if (pstPlayer->UinFlag & UIN_FLAG_SAFE_EMAIL)
	{
		task_player_trace(pstEnv,pstPlayer,TASK_OBJ_BIND_SAFE_EMAIL);
	}
	
	return 0;
}

int player_lock_safe_info(ZONESVRENV* pstEnv, Player *pstPlayer, int iFlag)
{
	CSPKG stPak;
	SAFELOCKSVR *pstSafeLockSvr = &stPak.Body.SafeLockSvr;

	pstSafeLockSvr->LockFlag = iFlag;
	
	Z_CSHEAD_INIT(&stPak.Head, SAFE_LOCK_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPak);
	return 0;
}

static int player_chg_safepwd_notify_acc(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	ACCSAFEPWD stPwd;

	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ,
		             "AccSafePwd", pstPlayer->stRoleData.Uin, NULL);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	sprintf(&pstUpdateReq->OperInfo.WhereDef[0], "where Uin=%d", pstPlayer->stRoleData.Uin);
	pstUpdateReq->DoInsert = 0;

	stPwd.Uin = pstPlayer->stRoleData.Uin;
	stPwd.SafePwd[0] = 0;
	if (pstPlayer->stRoleData.MiscInfo.LockPassWD[0] != 0)
	{
		STRNCPY(stPwd.SafePwd, pstPlayer->stRoleData.MiscInfo.LockPassWD, sizeof(stPwd.SafePwd));
	}
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stPwd;
	stMetaData.iBuff = sizeof(stPwd);
	
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

static int player_safelock_active(ZONESVRENV* pstEnv, Player *pstPlayer, SAFELOCKCLT *pstSafeLockClt )
{
	ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;
	
	if (	pstSafeLockClt->SafeLock.LockActive[0] == 0 )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SAFE203);
		return -1;
	}

	if (	pstMisc->LockPassWD[0] != 0 ||
		(pstPlayer->stOnline.cSafeLockFalg & SAFE_LOCK_ACTIVE_FALG))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SAFE204);
		return -1;
	}
	strncpy(pstMisc->LockPassWD, pstSafeLockClt->SafeLock.LockActive, sizeof(pstMisc->LockPassWD));
	pstPlayer->stOnline.cSafeLockFalg |= SAFE_LOCK_ACTIVE_FALG;
	pstPlayer->stOnline.cSafeLockFalg |= SAFE_LOCK_PASS_FLAG;
	player_lock_safe_info(pstEnv, pstPlayer, pstPlayer->stOnline.cSafeLockFalg);

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SAFE205);

	/*通知account*/
	player_chg_safepwd_notify_acc(pstEnv, pstPlayer);

	
	pstPlayer->UinFlag |= UIN_FLAG_SAFE_PASSWD;

	task_player_trace(pstEnv,pstPlayer,TASK_OBJ_SET_SAFE_PWD);
	return 0;
}

static int player_safelock_pass(ZONESVRENV* pstEnv, Player *pstPlayer, SAFELOCKCLT *pstSafeLockClt)
{
	ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;
	
	if (pstMisc->LockPassWD[0] == 0 )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SAFE206);
		return -1;
	}

	if (pstPlayer->stOnline.cSafeLockFalg & SAFE_LOCK_PASS_FLAG)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SAFE207);
		return -1;
	}

	if (strcmp(pstMisc->LockPassWD, pstSafeLockClt->SafeLock.LockPass) == 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SAFE208);
		pstPlayer->stOnline.cSafeLockFalg |= SAFE_LOCK_PASS_FLAG;
		player_lock_safe_info(pstEnv, pstPlayer, pstPlayer->stOnline.cSafeLockFalg);
		
		if ((pstPlayer->eStatus == PLAYER_STATUS_ROLE_NOTHING) &&
		 	(pstPlayer->stOnline.stSpanOnline.iStat == SPAN_STAT_CONNECT))
		{
			SSPKG stPkg;
			SSSPANSAFELOCK *pstSpanSafeLock = &stPkg.Body.SpanSafeLock;

			pstSpanSafeLock->Uin = pstPlayer->stRoleData.Uin;
			pstSpanSafeLock->RoleID = pstPlayer->stRoleData.RoleID;
			pstSpanSafeLock->SafeLockFalg = pstPlayer->stOnline.cSafeLockFalg;

			Z_SSHEAD_INIT(&stPkg.Head, SS_SPAN_SAFELOCK);
			z_ssmsg_send(pstEnv, &stPkg, pstPlayer->stOnline.stSpanOnline.stInfo.stConnect.iDstID);
		}
		
		return 0;
	}
	else
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SAFE302);
		return -1;
	}
	return 0;
}

int player_span_safelock(ZONESVRENV* pstEnv, SSSPANSAFELOCK *pstSpanSafeLock)
{
	Player *pstPlayer = player_get_by_uin(pstEnv, pstSpanSafeLock->Uin);

	if(!pstPlayer || 
	   pstPlayer->stRoleData.RoleID != pstSpanSafeLock->RoleID ||
	   pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		return -1;
	}

	pstPlayer->stOnline.cSafeLockFalg = pstSpanSafeLock->SafeLockFalg;
	
	return 0;
}

static int player_safelock_modify(ZONESVRENV* pstEnv, Player *pstPlayer, SAFELOCKCLT *pstSafeLockClt)
{
	ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;
	if (	pstMisc->LockPassWD[0] == 0 || 
		((pstPlayer->stOnline.cSafeLockFalg & SAFE_LOCK_ACTIVE_FALG) == 0))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SAFE303);
		return -1;
	}

	/*if ((pstPlayer->stOnline.cSafeLockFalg & SAFE_LOCK_PASS_FLAG) == 0)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "请先解锁");
		return -1;	
	}*/

	if (strcmp(pstSafeLockClt->SafeLock.LockModify.OldPassWD, pstMisc->LockPassWD))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SAFE302);
		return -1;
	}
	
	if(pstSafeLockClt->SafeLock.LockModify.NewPassWD[0]==0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SAFE305);
		return -1;
	}

	strncpy(pstMisc->LockPassWD, pstSafeLockClt->SafeLock.LockModify.NewPassWD, sizeof(pstMisc->LockPassWD));
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SAFE306);

	/*通知account*/
	player_chg_safepwd_notify_acc(pstEnv, pstPlayer);

	return 0;
}


static int player_safelock_cancel(ZONESVRENV* pstEnv, Player *pstPlayer, SAFELOCKCLT *pstSafeLockClt)
{
	ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;

	if ((pstPlayer->stOnline.cSafeLockFalg & SAFE_LOCK_ACTIVE_FALG) ==0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SAFE303);
		return -1;
	}

	if (strcmp(pstMisc->LockPassWD, pstSafeLockClt->SafeLock.LockCancel))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SAFE304);
		return -1;
	}

	pstPlayer->stOnline.cSafeLockFalg = 0;
	memset(pstMisc->LockPassWD, 0, sizeof(pstMisc->LockPassWD));

	player_lock_safe_info(pstEnv, pstPlayer, 0);
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SAFE309);

	player_chg_safepwd_notify_acc(pstEnv, pstPlayer);

	if (!(pstPlayer->stOnline.cSafeLockFalg & SAFE_LOCK_ACTIVE_FALG))
	{
		pstPlayer->UinFlag &= ~UIN_FLAG_SAFE_PASSWD;
	}
	
	return 0;
}

static int player_safelock_redo(ZONESVRENV* pstEnv, Player *pstPlayer, SAFELOCKCLT *pstSafeLockClt)
{
	ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;

	if (pstMisc->LockPassWD[0] == 0 )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SAFE303);
		return -1;
	}

	// 没有解锁,肯定不能重新锁上
	if (!(pstPlayer->stOnline.cSafeLockFalg & SAFE_LOCK_PASS_FLAG))
	{
		return -1;
	}

	pstPlayer->stOnline.cSafeLockFalg &= ~SAFE_LOCK_PASS_FLAG;
	player_lock_safe_info(pstEnv, pstPlayer, pstPlayer->stOnline.cSafeLockFalg);
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SAFE310);
	return 0;
	UNUSED(pstSafeLockClt);
}

int player_safelock(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	SAFELOCKCLT *pstSafeLockClt = &pstCsPkg->Body.SafeLockClt;
	UNUSED(pstFrameHead);

	if(pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		if(pstSafeLockClt->Cmd == SAFE_LOCK_PASS)
		{
			battle_proxy_request(pstEnv, pstPlayer, pstCsPkg);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
		}
		
		return -1;
	}
	
	if (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
	{
		if (pstSafeLockClt->Cmd != SAFE_LOCK_PASS)
		{
			return -1;
		}
		else if (!((pstPlayer->eStatus == PLAYER_STATUS_ROLE_NOTHING) &&
		 	(pstPlayer->stOnline.stSpanOnline.iStat == SPAN_STAT_CONNECT)))
		{
			return -1;
		}
	}

	if (!(pstPlayer->stOnline.cMiscFlag &MISC_FLAG_SEND_LOCK_SAFE))
	{
		return -1;
	}
	

	if (pstPlayer->stRoleData.Level < SAFELOCK_LEVEL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SAFE311,SAFELOCK_LEVEL);
		return -1;
	}

	if (player_check_grabsafe(pstEnv, pstPlayer) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
				ZONEERR_SAFE301,
				GRAB_SAFE_TIME);
		return -1;
	}

	if (pstSafeLockClt->Cmd == SAFE_LOCK_ACTIVE)
	{
		return player_safelock_active(pstEnv, pstPlayer, pstSafeLockClt );
	}
	else if(pstSafeLockClt->Cmd == SAFE_LOCK_PASS)
	{
		return player_safelock_pass(pstEnv, pstPlayer, pstSafeLockClt);
	}
	else if(pstSafeLockClt->Cmd == SAFE_LOCK_MODIFY)
	{
		return player_safelock_modify(pstEnv, pstPlayer, pstSafeLockClt);
	}
	else if(pstSafeLockClt->Cmd == SAFE_LOCK_CANCEL)
	{
		return player_safelock_cancel(pstEnv, pstPlayer, pstSafeLockClt);
	}
	else if (pstSafeLockClt->Cmd == SAFE_LOCK_REDO)
	{
		return  player_safelock_redo(pstEnv, pstPlayer, pstSafeLockClt);
	}
	else
	{
		return -1;
	}
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}