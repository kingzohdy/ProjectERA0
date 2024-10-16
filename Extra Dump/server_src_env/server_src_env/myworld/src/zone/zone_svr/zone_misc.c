
#include "zone_move.h"
#include "zone_map.h"
#include "zone_clt.h"
#include "tconnapi.h"
#include "zone_act_status.h"
#include "zone_npc.h"
#include "zone_ai.h"
#include "zone_attr.h"
#include "zone_team.h"
#include "zone_pet.h"
#include "zone_script.h"
#include "zone_clan.h"
#include "zone_status.h"
#include "zone_player.h"
#include "zone_fairy.h"
#include "mapmask.h"
#include "zone_package.h"
#include "zone_tmp.h"
#include "zone_skill.h"
#include "zone_machine.h"
#include "zone_oplog.h"
#include "zone_combat.h"
#include "zone_fairy.h"
#include "zone_secondaryskill.h"
#include "zone_misc.h"
#include "zone_mail.h"
#include "zone_shop.h"
#include "zone_itemlimit.h"
#include "zone_arm.h"
#include "zone_range.h"
#include "zone_levfeng.h"
#include "zone_err.h"
#include "zone_zh_name.h"
#include "zone_span_strong.h"
#include "zone_genius.h"
#include "zone_dianfeng.h"

int player_up_giftgroup(ZONESVRENV* pstEnv,Player *pstPlayer, GIFTGROUP *pstGiftGroup);
int invite_player_qiecuo(ZONESVRENV* pstEnv,  Player *pstPlayer,TESTINVITE *pstTestInvite)
{
	CSPKG stPak;
	INVITESVR *pstInviteSvr =  &stPak.Body.InviteSvr;
	Player *pstTestPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTestInvite->RoleName);
	if (!pstTestPlayer)
	{
		return -1;
	}

	if (GET_MAPID(pstTestPlayer->stRoleData.Map) != 1000)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_MISC1);
		return 0;
	}
	
	if (	pstPlayer->stOnline.stQiecuoInfo.iMemID > 0 ||
		pstPlayer->stOnline.stQiecuoInfo.iReqID > 0 ||
		pstPlayer->stOnline.stQiecuoInfo.iAcceptTm > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_MISC2);
		return 0;
	}
	
	if (	pstTestPlayer->stOnline.stQiecuoInfo.iMemID > 0 ||
		pstTestPlayer->stOnline.stQiecuoInfo.iReqID > 0 ||
		pstTestPlayer->stOnline.stQiecuoInfo.iAcceptTm > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_MISC3);
		return 0;
	}

	if (pstTestPlayer->stOnline.State & CS_STAT_STALL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_MISC4);
		return 0;
	}

	if(pstTestPlayer->stOnline.stExchgOnline.bExchgStat > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_MISC5);
		return 0;
	}

	if (pstTestPlayer->stOnline.cMiscFlag&MISC_FLAG_WUSHUANG)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_MISC6);
		return 0;
	}

	if ((pstTestPlayer->stOnline.State & CS_STAT_DEAD))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_MISC7);
		return 0;
	}
	
	pstTestPlayer->stOnline.stQiecuoInfo.iReqID = pstPlayer->iMemID;
	pstPlayer->stOnline.stQiecuoInfo.iReqID = pstTestPlayer->iMemID;
	pstInviteSvr->Cmd = TEST_SVR_INFO;
	strncpy(	pstInviteSvr->InviteTestSvr.TestInvite.RoleName, 
			pstPlayer->stRoleData.RoleName, 
			sizeof(pstInviteSvr->InviteTestSvr.TestInvite.RoleName));
	
	Z_CSHEAD_INIT(&stPak.Head, INVITE_TEST_SVR);
	z_cltmsg_send(pstEnv, pstTestPlayer, &stPak);
	return 0;
}


int player_qiecuo_res(ZONESVRENV* pstEnv,  Player *pstPlayer, Player *pstOther, RESPOS*pstPos, int iFlag)
{
	CSPKG stPak;
	INVITESVR *pstInviteSvr = &stPak.Body.InviteSvr;

	pstInviteSvr->Cmd = TEST_SVR_RES;
	pstInviteSvr->InviteTestSvr.TestSvrRes.TestRes.Flag = iFlag;
	pstInviteSvr->InviteTestSvr.TestSvrRes.x = pstPos->X;
	pstInviteSvr->InviteTestSvr.TestSvrRes.y = pstPos->Y;
	strncpy(	pstInviteSvr->InviteTestSvr.TestSvrRes.TestRes.RoleName,
			pstPlayer->stRoleData.RoleName, 
			sizeof(pstInviteSvr->InviteTestSvr.TestSvrRes.TestRes.RoleName));
	
	Z_CSHEAD_INIT(&stPak.Head, INVITE_TEST_SVR);
	z_cltmsg_send(pstEnv, pstOther, &stPak);
	return 0;
}

int invite_player_qiecuo_res(ZONESVRENV* pstEnv,  Player *pstPlayer, TESTRES *pstTestCltRes)
{
	Player *pstTestPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTestCltRes->RoleName);
	if (!pstTestPlayer)
	{
		return -1;
	}

	if (	pstTestPlayer->stOnline.stQiecuoInfo.iReqID <= 0 ||
		pstPlayer->stOnline.stQiecuoInfo.iReqID <= 0)
	{
		goto final;
	}

	// 同意
	if (pstTestCltRes->Flag == 0)
	{
		/*if (pstTestPlayer->stOnline.iTestMemID > 0)
		{
			z_sys_strf( pstEnv,pstPlayer, SYS_MIDDLE2,
						"%s 已经在切磋中", 
						pstTestPlayer->stRoleData.RoleName);
			goto final;
		}*/

		if (0 > z_check_op_dist(pstEnv, pstPlayer, pstTestPlayer->stRoleData.Map, &pstTestPlayer->stRoleData.Pos, 3500))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MISC107);
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTestPlayer, SYS_MIDDLE2, ZONEERR_MISC8);
			goto final;
		}

		if (player_qiecuo_check(pstEnv, pstTestPlayer) < 0)
		{
			goto final;
		}
		
		pstTestPlayer->stOnline.stQiecuoInfo.iAcceptTm = 
		pstPlayer->stOnline.stQiecuoInfo.iAcceptTm = 
		pstEnv->pstAppCtx->stCurr.tv_sec;
		
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstTestPlayer, SYS_MIDDLE2,
				     	ZONEERR_MISC9, 
					pstPlayer->stRoleData.RoleName);
		
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MISC10);

		if(is_player_ride_on_machine(pstTestPlayer))
		{
			player_ride_machine_brk(pstEnv, pstTestPlayer, 1);
		}

		if(is_player_ride_on_machine(pstPlayer))
		{
			player_ride_machine_brk(pstEnv, pstPlayer, 1);
		}
		
		return 0;
	}
	// 拒绝
	else
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstTestPlayer, SYS_MIDDLE2,
					ZONEERR_MISC11, 
					pstPlayer->stRoleData.RoleName);
		player_qiecuo_res(pstEnv,  pstPlayer, pstTestPlayer, &pstTestPlayer->stRoleData.Pos,pstTestCltRes->Flag);
		player_qiecuo_res(pstEnv,  pstTestPlayer, pstPlayer, &pstTestPlayer->stRoleData.Pos,pstTestCltRes->Flag);
		goto final;
	}

	

final:
	memset(&pstTestPlayer->stOnline.stQiecuoInfo, 0, sizeof(pstTestPlayer->stOnline.stQiecuoInfo));
	memset(&pstPlayer->stOnline.stQiecuoInfo, 0, sizeof(pstPlayer->stOnline.stQiecuoInfo));
	return 0;
}


int player_qiecuo_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	Player *pstOther;
	if( pstEnv->iInstMode != INST_MODE_ZONE_SVR )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
		goto final;
	}

	if (GET_MAPID(pstPlayer->stRoleData.Map)	!= 1000)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_MISC12);
		goto final;
	}

	if (pstPlayer->stOnline.State & CS_STAT_STALL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_MISC13);
		goto final;
	}

	if(pstPlayer->stOnline.stExchgOnline.bExchgStat > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_MISC14);
		goto final;
	}
	
	if ((pstPlayer->stOnline.State & CS_STAT_DEAD))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_MISC15);
		goto final;
	}

	if (pstPlayer->stOnline.cMiscFlag&MISC_FLAG_WUSHUANG)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_MISC16);
		goto final;
	}
	
	return 0;

final:
	if (pstPlayer->stOnline.stQiecuoInfo.iReqID > 0)
	{
		pstOther = z_id_player(pstEnv->pstAppCtx, pstEnv,pstPlayer->stOnline.stQiecuoInfo.iReqID);
		if (pstOther)
		{
			z_qiecuo_invite_cancel(pstEnv, pstOther);
			memset(&pstOther->stOnline.stQiecuoInfo, 0, sizeof(pstOther->stOnline.stQiecuoInfo));
		}
		memset(&pstPlayer->stOnline.stQiecuoInfo, 0, sizeof(pstPlayer->stOnline.stQiecuoInfo));	
	}
	return -1;
}

int player_qiecuo(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						     TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{	
	INVITECLT *pstInviteClt = &pstCsPkg->Body.InviteClt;
	UNUSED(pstFrameHead);
	UNUSED(pstAppCtx);
	
	if (player_qiecuo_check(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}

	if (pstInviteClt->Cmd == TEST_CLT_INVITE)
	{
		return invite_player_qiecuo(pstEnv,  pstPlayer,&pstInviteClt->InviteTestClt.TestInvite);
	}
	else if(pstInviteClt->Cmd == TEST_CLT_RES)
	{
		return invite_player_qiecuo_res(pstEnv,  pstPlayer, &pstInviteClt->InviteTestClt.TestCltRes);
	}
	else
	{
		return -1;
	}
}

int z_qiecuo_invite_cancel(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPak;
	INVITESVR *pstInviteSvr =  &stPak.Body.InviteSvr;

	pstInviteSvr->Cmd = TEST_SVR_CANCEL;
	pstInviteSvr->InviteTestSvr.TestMemID = pstPlayer->stOnline.stQiecuoInfo.iReqID;
	
	Z_CSHEAD_INIT(&stPak.Head, INVITE_TEST_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPak);
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MISC17);
	return 0;
}

int player_logout_qiecuo(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	Player *pstOther;

	// 在切磋下  取消邀请
	if (pstPlayer->stOnline.stQiecuoInfo.iReqID > 0)
	{
		pstOther = z_id_player(pstEnv->pstAppCtx, pstEnv,pstPlayer->stOnline.stQiecuoInfo.iReqID);
		if (pstOther)
		{
			z_qiecuo_invite_cancel(pstEnv, pstOther);
			memset(&pstOther->stOnline.stQiecuoInfo, 0, sizeof(pstOther->stOnline.stQiecuoInfo));
		}
		memset(&pstPlayer->stOnline.stQiecuoInfo, 0, sizeof(pstPlayer->stOnline.stQiecuoInfo));	
	}

	// 切磋下 判定失败
	if (pstPlayer->stOnline.stQiecuoInfo.iMemID > 0)
	{
		pstOther = z_id_player(pstEnv->pstAppCtx, pstEnv,pstPlayer->stOnline.stQiecuoInfo.iMemID);
		if (pstOther)
		{
			z_player_qiecuo_end(pstEnv, pstOther, 1);
		}
		memset(&pstPlayer->stOnline.stQiecuoInfo, 0, sizeof(pstPlayer->stOnline.stQiecuoInfo));	
	}
	return 0;
}


int z_player_clean_badbuff(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	ZoneStatus *pstZoneStatus;
	STATUSDEF *pstStatusDef;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;
	int iFlag = 0;

	for (i=(int)pstStatusList->bBadStatusNum-1; i>=0; i--)	
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
				
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (pstStatusDef && 0 == pstStatusDef->DeadClear)
		{
			continue;
		}

		iFlag = 1;
		z_player_status_clear(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstStatusList->astBadStatus, i, &pstStatusList->bBadStatusNum);
	}


	if (iFlag)
	{
		z_player_send_status(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);

	return 0;
}

int z_player_qiecuo_end(ZONESVRENV* pstEnv, Player *pstPlayer, int iVictory)
{
	role_test_action(pstEnv, pstPlayer, 1,iVictory);
	memset(&pstPlayer->stOnline.stQiecuoInfo, 0, sizeof(pstPlayer->stOnline.stQiecuoInfo));	
	if (iVictory == 0)
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MISC18);
	else
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MISC19);

	// 切磋结束     清除DEBUFF 
	z_player_clean_badbuff(pstEnv, pstPlayer);

	player_totem_update_enemy(pstEnv, pstPlayer);
	
	return 0;
}

int z_player_tick_qiecuo(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if (	pstPlayer->stOnline.stQiecuoInfo.iAcceptTm > 0 &&
		pstPlayer->stOnline.stQiecuoInfo.iAcceptTm +3 < pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		Player *pstTestPlayer = NULL;
		// 进入切磋
		if (player_qiecuo_check(pstEnv, pstPlayer) < 0)
		{
			return 0;
		}
		
		pstTestPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stQiecuoInfo.iReqID);
		if (pstTestPlayer)
		{
			if (player_qiecuo_check(pstEnv, pstTestPlayer) < 0)
			{
				return 0;
			}
			
			pstTestPlayer->stOnline.stQiecuoInfo.iMemID = pstPlayer->iMemID;
			pstPlayer->stOnline.stQiecuoInfo.iMemID = pstTestPlayer->iMemID;
			pstTestPlayer->stOnline.stQiecuoInfo.stTestPos = 
						pstPlayer->stOnline.stQiecuoInfo.stTestPos = 
						pstTestPlayer->stRoleData.Pos;
			role_test_action(pstEnv, pstPlayer, 0,0);
			role_test_action(pstEnv, pstTestPlayer, 0,0);
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv,pstTestPlayer, SYS_MIDDLE2,ZONEERR_MISC20);
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MISC20);
			pstTestPlayer->stOnline.stQiecuoInfo.iAcceptTm = 0;
			pstTestPlayer->stOnline.stQiecuoInfo.iReqID = 0;

			player_qiecuo_res(pstEnv,  pstPlayer, pstTestPlayer, &pstTestPlayer->stRoleData.Pos,0);
			player_qiecuo_res(pstEnv,  pstTestPlayer, pstPlayer, &pstTestPlayer->stRoleData.Pos,0);
		}
		pstPlayer->stOnline.stQiecuoInfo.iAcceptTm = 0;
		pstPlayer->stOnline.stQiecuoInfo.iReqID = 0;
	}

	if (pstPlayer->stOnline.stQiecuoInfo.iMemID <= 0)
	{
		return -1;
	}

	// 离开切磋区域了
	if (0 > z_check_op_dist(pstEnv, pstPlayer, pstPlayer->stRoleData.Map,&pstPlayer->stOnline.stQiecuoInfo.stTestPos, 4000))
	{
		if (pstPlayer->stOnline.stQiecuoInfo.iLeaveTm <= 0)
		{
			pstPlayer->stOnline.stQiecuoInfo.iLeaveTm = pstEnv->pstAppCtx->stCurr.tv_sec;
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MISC21);
		}
		else
		{
			if (	pstPlayer->stOnline.stQiecuoInfo.iLeaveTm > 0 &&
				pstPlayer->stOnline.stQiecuoInfo.iLeaveTm + 5 < pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				Player *pstQcPlayer;
				pstQcPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stQiecuoInfo.iMemID);
				if (pstQcPlayer)
					z_player_qiecuo_end(pstEnv, pstQcPlayer, 1);
				z_player_qiecuo_end(pstEnv, pstPlayer, 0);
			}
		}
	}
	else
	{
		pstPlayer->stOnline.stQiecuoInfo.iLeaveTm = 0;
	}


	// 判断切磋
	if (pstPlayer->stRoleData.RoleDetail.HPCurr <= 1)
	{
		Player *pstTmp = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stQiecuoInfo.iMemID);
		if (pstTmp)
			z_player_qiecuo_end(pstEnv, pstTmp, 1);
		z_player_qiecuo_end(pstEnv, pstPlayer, 0);
	}	
	return 0;
}

int is_in_qiecuo(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if(pstPlayer->stOnline.stQiecuoInfo.iAcceptTm > 0)
	{
		return 1;
	}

	if(pstPlayer->stOnline.stQiecuoInfo.iMemID > 0)
	{
		return 1;
	}
		
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int z_is_qiecuo(ZONESVRENV* pstEnv, Player *pstAtker, Player *pstAtkee)
{
	if (!pstAtker || !pstAtkee)
	{
		return 0;
	}

	if (	pstAtker->stOnline.stQiecuoInfo.iMemID > 0 &&
		pstAtker->stOnline.stQiecuoInfo.iMemID == pstAtkee->iMemID &&
		pstAtkee->stOnline.stQiecuoInfo.iMemID == pstAtker->iMemID)
	{
		return 1;
	}
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int role_test_action(ZONESVRENV* pstEnv, Player *pstPlayer,int iFlag,int iVictory)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	Player *pstQieCuoPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stQiecuoInfo.iMemID);

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_TEST;

	pstActionRes->Data.RoleTest.Flag = iFlag;
	pstActionRes->Data.RoleTest.Victory = iVictory;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);

	// 进入切磋只要发给两个人
	if (iFlag == 0)
	{
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		if (pstQieCuoPlayer)
		{
			z_cltmsg_send(pstEnv, pstQieCuoPlayer, &stPkg);
		}
	}
	else
	{
		z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	}
	
	
	return 0;
}

int acc_gift_info_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, SSACCGIFTLISTRES *pstRes)
{
	CSPKG stPak;
	CSACCGIFTSVR *pstAccGiftSvr = &stPak.Body.AccGiftSvr;

	pstAccGiftSvr->Type = SVR_ACC_GIFT_TYPE_ALL;
	pstAccGiftSvr->AccGiftData.AccGiftInfo.Count = pstRes->Count;
	memcpy(pstAccGiftSvr->AccGiftData.AccGiftInfo.AccGifts, pstRes->AccGifts, 
			sizeof(pstAccGiftSvr->AccGiftData.AccGiftInfo.AccGifts));

	Z_CSHEAD_INIT(&stPak.Head, ACCGIFT_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPak);
	return 0;
}

int props_gift_info_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, SSPROPGIFTLISTRES *pstRes)
{
	CSPKG stPak;
	CSPROPGIFTSVR *pstPropGiftSvr = &stPak.Body.PropGiftSvr;

	pstPropGiftSvr->Type = SVR_PROP_GIFT_TYPE_ALL;
	memcpy(&pstPropGiftSvr->PropGiftData.GiftInfo,&pstRes->PropGiftInfo,
		sizeof(pstPropGiftSvr->PropGiftData.GiftInfo));

	Z_CSHEAD_INIT(&stPak.Head, PROPS_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPak);
	return 0;
}

int props_gift_get_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, SSPROPGIFTLISTRES *pstRes)
{
	CSPKG stPak;
	CSPROPGIFTSVR *pstPropGiftSvr = &stPak.Body.PropGiftSvr;

	pstPropGiftSvr->Type = SVR_PROP_GIFT_TYPE_ONE;
	pstPropGiftSvr->PropGiftData.GiftRes = pstRes->PropGiftInfo.PropsGift[0];

	Z_CSHEAD_INIT(&stPak.Head, PROPS_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPak);
	return 0;
}

int acc_gift_get_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, SSACCGIFTLISTRES *pstRes)
{
	CSPKG stPak;
	CSACCGIFTSVR *pstAccGiftSvr = &stPak.Body.AccGiftSvr;

	pstAccGiftSvr->Type = SVR_ACC_GIFT_TYPE_ONE;
	pstAccGiftSvr->AccGiftData.AccGiftRes.ID = pstRes->ID;

	Z_CSHEAD_INIT(&stPak.Head, ACCGIFT_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPak);
	return 0;
}

int player_propsgift_res(ZONESVRENV* pstEnv, SSPKG *pstSSPkg)
{
	SSPROPGIFTLISTRES *pstRes = &pstSSPkg->Body.PropGiftListRes;
	Player *pstPlayer = NULL;

	pstPlayer =  player_get_by_uin(pstEnv, pstRes->Uin);
	if (pstRes->ID == 0)
	{
		// 全部信息
		//if (pstPlayer && pstRes->Count > 0)
		if (pstPlayer)
		{
			props_gift_info_to_clt(pstEnv, pstPlayer, pstRes);
		}
	}
	else
	{
		int i;
		int iMail = 0;
		int iNum = 0;
		ROLEGRID stRoleGrid[PROPS_GIFT_ITEM_NUM];

		if (pstRes->RetCode == PROPS_GIFT_LIST_RET_SUCC)
		{

			if (pstRes->PropGiftInfo.DefCount > 0)
			{
				for (i=0; i< PROPS_GIFT_ITEM_NUM; i++)
				{
					if (pstRes->PropGiftInfo.PropsDef[0].PropsGiftItem[i].ItemID == 0)
					{
						break;
					}

					if (0 == item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid[i], pstRes->PropGiftInfo.PropsDef[0].PropsGiftItem[i].ItemID ) )
					{
						stRoleGrid[i].GridData.RoleItem.Num = pstRes->PropGiftInfo.PropsDef[0].PropsGiftItem[i].ItemNum;
						stRoleGrid[i].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
						iNum ++;
					}
				}
			}
			
			if (pstPlayer && pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN && pstPlayer->stRoleData.RoleID == pstRes->RoleID)
			{
				if (package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid[0], iNum,0) ==0)
				{
					package_add(pstEnv->pstAppCtx , pstEnv , pstPlayer, &stRoleGrid[0], iNum,0, IMPITEM_OPTYPE_GIFT);
				}
				else
				{
					iMail = 1;
				}

				props_gift_get_to_clt(pstEnv, pstPlayer, pstRes);
			}
			else
			{
				iMail = 1;
			}

			// 发邮件给pstRes->RoleName
			if (iMail)
			{
				player_sysmail_fast_send(pstEnv, pstRes->RoleName, LJY_MAIL9,
										LJY_MAIL10,
										stRoleGrid, iNum, 0,NULL, 0);
			}

			if(pstPlayer)
			{
				z_role_gift_log(pstEnv, pstPlayer, pstRes->RoleName, GIFT_TYPE_PROPGIFT, "充值奖励", pstRes->ID,pstRes->PropGiftInfo.PropsDef[0].CashUp);
			}
			else
			{
				z_role_gift_log(pstEnv, NULL, pstRes->RoleName, GIFT_TYPE_PROPGIFT, "充值奖励", pstRes->ID,pstRes->PropGiftInfo.PropsDef[0].CashUp);
			}
		}
		else if (pstRes->RetCode == PROPS_GIFT_LIST_RET_LOCK)
		{
			if (pstPlayer)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2 , ZONEERR_MISC22);
			}
		}
		else if (pstRes->RetCode == PROPS_GIFT_LIST_RET_NO_DEF)
		{
			if (pstPlayer)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2 , ZONEERR_MISC23);
			}
		}
		else if (pstRes->RetCode == PROPS_GIFT_LIST_RET_UPDATE)
		{
			if (pstPlayer)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2 , ZONEERR_MISC24);
			}
		}
		else
		{
			if (pstPlayer)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2 , ZONEERR_UNKNOWN);
			}
		}
		
	}
	return 0;
}

int player_accgift_res(ZONESVRENV* pstEnv, SSPKG *pstSSPkg)
{
	SSACCGIFTLISTRES *pstRes = &pstSSPkg->Body.AccGiftListRes;
	Player *pstPlayer = NULL;
	ITEMDEF *pstItemDef;

	pstPlayer =  player_get_by_uin(pstEnv, pstRes->Uin);
	if (pstRes->ID == 0)
	{
		// 全部信息
		if (pstPlayer && pstRes->Count > 0)
		{
			acc_gift_info_to_clt(pstEnv, pstPlayer, pstRes);
		}
	}
	else
	{
		int i;
		int iMail = 0;
		int iNum = 0;
		ROLEGRID stRoleGrid[MAX_ACC_GIFT_LIST];

		tlog_info(pstEnv->pstLogCat, 0, 0, "Uin=%d resid %d is recv gift msg!", 
				 pstRes->Uin, pstRes->ID);

		for (i=0; i<MAX_ACC_GIFT_LIST; i++)
		{
			if (pstRes->AccGifts[0].GiftItems[i].ItemID == 0 )
			{
				if (i == 0)
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "gift ItemID=0! ");
					return -1;
				}
				break;
			}
			
			pstItemDef = z_find_itemdef(pstEnv, pstRes->AccGifts[0].GiftItems[i].ItemID);
			if (!pstItemDef)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "gift pstItemDef=NULL ItemID=%d! Uin=%d", 
							pstRes->Uin, pstRes->AccGifts[0].GiftItems[i].ItemID);
				return -1;
			}
			
			if (0 == item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid[i], pstRes->AccGifts[0].GiftItems[i].ItemID ) )
			{
				if (pstItemDef->Result[0].ResultID == RESULT_ITEM_SWITCH)
				{
					stRoleGrid[i].GridData.RoleItem.InstVal1 = pstRes->AccGifts[0].GiftItems[i].ItemNum;
				}
				else
				{
					stRoleGrid[i].GridData.RoleItem.Num = pstRes->AccGifts[0].GiftItems[i].ItemNum;
				}
				stRoleGrid[i].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
				iNum ++;
			}
			else
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "gift item_create fail ItemID=%d! Uin=%d", 
							pstRes->AccGifts[0].GiftItems[i].ItemID, pstRes->Uin);
				return -1;
			}
		}
		
		if (pstPlayer && pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN && pstPlayer->stRoleData.RoleID == pstRes->RoleID)
		{
			if (package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid[0], iNum,0) ==0)
			{
				package_add(pstEnv->pstAppCtx , pstEnv , pstPlayer, &stRoleGrid[0], iNum,0, IMPITEM_OPTYPE_GIFT);
			}
			else
			{
				iMail = 1;
			}

			acc_gift_get_to_clt(pstEnv, pstPlayer, pstRes);
			
			//领取完特殊礼包后，再重新拉取8条数据
			player_acc_gift_req(pstEnv, pstPlayer, 0);
		}
		else
		{
			iMail = 1;
		}

		// 发邮件给pstRes->RoleName
		if (iMail)
		{
			player_sysmail_fast_send(pstEnv, pstRes->RoleName, pstRes->AccGifts[0].Name,
									LJY_MAIL10,
									stRoleGrid, iNum, 0,NULL, 0);
		}

		if(pstPlayer)
		{
			z_role_gift_log(pstEnv, pstPlayer, pstRes->RoleName, GIFT_TYPE_ACCGIFT, pstRes->AccGifts[0].Name, pstRes->ID,0);
		}
		else
		{
			z_role_gift_log(pstEnv, NULL, pstRes->RoleName, GIFT_TYPE_ACCGIFT, pstRes->AccGifts[0].Name, pstRes->ID,0);
		}

		tlog_info(pstEnv->pstLogCat, 0, 0, "Uin=%d resid %d recv gift end ! mail %d", 
				pstRes->Uin, pstRes->ID, iMail);
	}
	return 0;
}

int player_accgift_insert_res(ZONESVRENV* pstEnv, SSPKG *pstSSPkg)
{
	SSACCGIFTINSERTRES *pstRes = &pstSSPkg->Body.AccGiftInsertRes;
	
	tlog_info(pstEnv->pstLogCat, 0, 0, "Uin=%d acc=%s gift=%s ", 
				pstRes->Uin, pstRes->Pass9Account , pstRes->AccGift.Name);
	
	return 0;
}

int props_gift_bill_add_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, SSPROPGIFTBILLRES *pstRes)
{
	CSPKG stPak;
	CSPROPGIFTSVR *pstPropGiftSvr = &stPak.Body.PropGiftSvr;

	if (pstRes->PropsInfoType == PROPS_GIFT_BILL_INFO_TYPE_CASH)
	{
		pstPropGiftSvr->Type = SVR_PROP_GIFT_TYPE_ADD;
		pstPropGiftSvr->PropGiftData.PropsGiftAddCash.GroupID = pstRes->PropsGiftBillInfo.CashInfo.GroupID;
		pstPropGiftSvr->PropGiftData.PropsGiftAddCash.AddCash = pstRes->PropsGiftBillInfo.CashInfo.AddCash;
	}
	else if (pstRes->PropsInfoType == PROPS_GIFT_BILL_INFO_TYPE_GIFT)
	{
		pstPropGiftSvr->Type = SVR_PROP_GIFT_TYPE_NOTICE;
		pstPropGiftSvr->PropGiftData.GiftNotice = pstRes->PropsGiftBillInfo.PropsGift;
	}

	Z_CSHEAD_INIT(&stPak.Head, PROPS_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPak);
	return 0;
}

int player_propsgift_bill_res(ZONESVRENV* pstEnv, SSPKG *pstSSPkg)
{
	SSPROPGIFTBILLRES *pstRes = &pstSSPkg->Body.PropGiftBillRes;  
	Player *pstPlayer = z_loginname_player(pstEnv->pstAppCtx, pstEnv, pstRes->Pass9Account);

	if (pstPlayer)
	{
		props_gift_bill_add_to_clt(pstEnv, pstPlayer, pstRes);
	}
	
	return 0;
}

static int player_cdkey_res_clt(ZONESVRENV* pstEnv,SSCDKEYRES *pstRes,Player *pstPlayer)
{
	CSPKG stPak;
	CSCDKEYSVR *pstSvr = &stPak.Body.CDKeySvr;

	pstSvr->Succ = pstRes->Succ;

	//z_sys_strf(pstEnv,  pstPlayer, SYS_MIDDLE2, "%d", pstSvr->Succ);

	Z_CSHEAD_INIT(&stPak.Head, CDKEY_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPak);
	return 0;
}

int player_ss_cdkey_res(ZONESVRENV* pstEnv, SSPKG *pstSSPkg)
{
	SSCDKEYRES *pstRes = &pstSSPkg->Body.SSCDKeyRes;
	int i;
	int iNum = 0;
	ROLEGRID stRoleGrid[CDKEY_ITEM_NUM];
	Player *pstPlayer = NULL;
	char szName[64];

	pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRes->RoleName);
	if (pstPlayer)
	{
		player_cdkey_res_clt(pstEnv,pstRes,pstPlayer);
	}
	
	if (pstRes->Succ == CDKEY_FAIL_NO)
	{
		for (i=0; i<CDKEY_ITEM_NUM; i++)
		{
			if (pstRes->Items[i].ItemID <= 0)
			{
				break;
			}

			if (0 == item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid[i],pstRes->Items[i].ItemID ) )
			{
				stRoleGrid[i].GridData.RoleItem.Num = pstRes->Items[i].ItemNum;
				stRoleGrid[i].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
				iNum ++;
			}
		}

		 	// 发邮件
	 	if (iNum > 0)
	 	{
	 		char szMsg[256];
	 
	 		snprintf(szMsg, sizeof(szMsg),LJY_MAIL11,pstRes->RoleName,pstRes->CDKey);
	 		
	 		player_sysmail_fast_send(pstEnv, pstRes->RoleName, LJY_MAIL12,
	 									szMsg,stRoleGrid, iNum, 0,LJY_MAIL13, 0);
	 	}

		snprintf(szName,sizeof(szName),"CDKey:%s",pstRes->CDKey);
		if(pstPlayer)
		{
			z_role_gift_log(pstEnv, pstPlayer, pstRes->RoleName, GIFT_TYPE_CDKEY, szName, 0,0);
		}
		else
		{
			z_role_gift_log(pstEnv, NULL, pstRes->RoleName, GIFT_TYPE_CDKEY, szName, 0,0);
		}
		
	}
	return 0;
}


// 更新在线时间  有一点点误差
int player_update_online_sec(ZONESVRENV* pstEnv,Player *pstPlayer,int iLogin)
{
	int iSec;
	ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;

	if (!IsSameDay(pstMisc->ODTime.UpTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstMisc->ODTime.OndaySec = 0;

		memset(&pstPlayer->stRoleData.MiscInfo.SpanStrongMiscInfo.Award, 0, 
				sizeof(pstPlayer->stRoleData.MiscInfo.SpanStrongMiscInfo.Award));
	}
	else
	{
		iSec =  pstEnv->pstAppCtx->stCurr.tv_sec - pstMisc->ODTime.UpTime;
		if (!iLogin)
		{
			if (iSec > 0 && iSec <= 20)
				pstMisc->ODTime.OndaySec += iSec;
		}
	}

	pstMisc->ODTime.UpTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	//完成一天在线XX时长任务
	if( pstPlayer->stOnline.cMiscFlag & MISC_FLAG_ONLINE_TIME)
	{
		task_online_time_trace(pstEnv, pstPlayer, pstMisc->ODTime.OndaySec);
	}

	span_strong_player_money_award(pstEnv, pstPlayer);
	return 0;
}


ONLINEGIFTGROUP *z_find_onlinegroupgift(ZONESVRENV* pstEnv, int iID)
{
	ONLINEGIFTGROUP stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.ID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sOnlineGiftGroup, pstObj->iOnlineGiftGroup, MAX_ONLINE_GROUP_DATA, &iEqu);
	if (iEqu)
	{
		return (ONLINEGIFTGROUP *)&pstObj->sOnlineGiftGroup[iIndex];
	}
	return NULL;
}

void onlinegiftgroup2role(ZONESVRENV* pstEnv,ONLINEGIFTGROUP *pstDef, GIFTGROUP *pstGiftGroup)
{
	pstGiftGroup->ID = pstDef->ID;
	pstGiftGroup->Time = pstDef->Time;
	pstGiftGroup->UpDayTime = 0;
	pstGiftGroup->GetFlag = 0;
	pstGiftGroup->ContDay = 0;
	UNUSED(pstEnv);
}

int player_login_cmp_onlinegroup(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	int i,j;
	GIFTGROUPS *pstGiftGroups = &pstPlayer->stRoleData.MiscInfo.GiftGroups;  
	ZoneObj *pstZoneObj = pstEnv->pstZoneObj;
	ONLINEGIFTGROUP *pstDef;
	GIFTGROUP *pstGiftGroup;
	ONEDAYTIME *pstOneDayTime = &pstPlayer->stRoleData.MiscInfo.ODTime;

	// 正向检查
	for (i=pstGiftGroups->Num -1; i>=0; i--)
	{
		pstGiftGroup = &pstGiftGroups->GiftGroups[i];
		pstDef = z_find_onlinegroupgift(pstEnv, pstGiftGroups->GiftGroups[i].ID);
		if (!pstDef)
		{
			pstGiftGroups->Num --;
			if (i != pstGiftGroups->Num)
			{
				memcpy(&pstGiftGroups->GiftGroups[i], 
						&pstGiftGroups->GiftGroups[pstGiftGroups->Num], 
						sizeof(pstGiftGroups->GiftGroups[0]));
			}
			continue;
		}

		
		// 检查角色身上和数据库差异
		// 时间变了   删掉
		if (pstGiftGroups->GiftGroups[i].Time != pstDef->Time)
		{
			pstGiftGroups->Num --;
			if (i != pstGiftGroups->Num)
			{
				memcpy(&pstGiftGroups->GiftGroups[i], 
						&pstGiftGroups->GiftGroups[pstGiftGroups->Num], 
						sizeof(pstGiftGroups->GiftGroups[0]));
			}
		}
	}

	// 反向检查
	for (i=0; i<pstZoneObj->iOnlineGiftGroup; i++)
	{
		pstDef = (ONLINEGIFTGROUP*)pstZoneObj->sOnlineGiftGroup[i];
		for (j=0; j<pstGiftGroups->Num; j++)
		{
			if (pstDef->ID == pstGiftGroups->GiftGroups[j].ID)
			{
				break;
			}
		}

		// 添加一条记录
		if (j>=pstGiftGroups->Num)
		{
			if (pstGiftGroups->Num < MAX_GIFT_GROUP_DEF)
			{
				onlinegiftgroup2role(pstEnv,pstDef, &pstGiftGroups->GiftGroups[pstGiftGroups->Num]);
				pstGiftGroups->Num++;
			}
		}
	}

	// 上线更新下
	for (i=0; i<pstGiftGroups->Num; i++)
	{
		if (!IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstGiftGroups->GiftGroups[i].UpDayTime))
		{
			if (	pstGiftGroups->GiftGroups[i].UpDayTime == 0 ||
				IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstGiftGroups->GiftGroups[i].UpDayTime+24*60*60))
			{
				if (pstOneDayTime->OndaySec >= pstGiftGroups->GiftGroups[i].Time*60)
				{
					player_up_giftgroup(pstEnv,pstPlayer, &pstGiftGroups->GiftGroups[i]);
				}
			}
			else
			{
				// 不是连续的天了
				pstGiftGroups->GiftGroups[i].ContDay  = 0;
				pstGiftGroups->GiftGroups[i].UpDayTime = 0;
				pstGiftGroups->GiftGroups[i].GetFlag = 0;
			}
		}
	}

	// 符合条件的通知客户端
	for (i=0; i<pstGiftGroups->Num; i++)
	{
		pstDef = z_find_onlinegroupgift(pstEnv, pstGiftGroups->GiftGroups[i].ID);
		if (!pstDef)
			continue;
		
		for (j = pstDef->OneNum-1; j>=0; j--)
		{
			if (	pstGiftGroups->GiftGroups[i].ContDay >= pstDef->GiftOnes[j].Day &&
				pstGiftGroups->GiftGroups[i].GetFlag == 0 )
			{
				giftgroup_add_to_clt(pstEnv, pstPlayer,&pstDef->GiftOnes[j],pstDef->ID);
				break;
			}
		}	
	}
	return 0;
}


int giftgroup_add_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,GIFTONE *pstGiftOne,int iID)
{
	CSPKG stPak;
	CSONLINEGIFTGROUPSVR *pstOnlineGiftGroupSvr  = &stPak.Body.OnlineGiftGroupSvr;

	pstOnlineGiftGroupSvr->Type = SVR_GIFT_GROUP_TYPE_ONE;
	pstOnlineGiftGroupSvr->GiftGroupData.GiftGroupOne.ID = iID;
	
	pstOnlineGiftGroupSvr->GiftGroupData.GiftGroupOne.Day = pstGiftOne->Day;
	Z_CSHEAD_INIT(&stPak.Head, ONLINEGIFT_GROUP_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPak);
	return 0;

}

int giftgroup_res_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,GIFTONE *pstGiftOne,int iID)
{
	CSPKG stPak;
	CSONLINEGIFTGROUPSVR *pstOnlineGiftGroupSvr  = &stPak.Body.OnlineGiftGroupSvr;

	int iMail = 0;
	int iNum = 0;
	ROLEGRID stRoleGrid[3];

	if (pstGiftOne->ItemID1 > 0 && pstGiftOne->ItemNum1 > 0)
	{
		if (0 == item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid[iNum], pstGiftOne->ItemID1) )
		{
			stRoleGrid[iNum].GridData.RoleItem.Num = pstGiftOne->ItemNum1;
			stRoleGrid[iNum].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
			iNum ++;
		}
	}
	
	if (pstGiftOne->ItemID2 > 0 && pstGiftOne->ItemNum2 > 0)
	{
		if (0 == item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid[iNum], pstGiftOne->ItemID2) )
		{
			stRoleGrid[iNum].GridData.RoleItem.Num = pstGiftOne->ItemNum2;
			stRoleGrid[iNum].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
			iNum ++;
		}
	}

	if (pstGiftOne->ItemID3 > 0 && pstGiftOne->ItemNum3 > 0)
	{
		if (0 == item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid[iNum], pstGiftOne->ItemID3) )
		{
			stRoleGrid[iNum].GridData.RoleItem.Num = pstGiftOne->ItemNum3;
			stRoleGrid[iNum].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
			iNum ++;
		}
	}

	if (package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid[0], iNum,0) ==0)
	{
		package_add(pstEnv->pstAppCtx , pstEnv , pstPlayer, &stRoleGrid[0], iNum,0, IMPITEM_OPTYPE_GIFT);
	}
	else
	{
		iMail = 1;
	}
	
	if (iMail)
	{
		player_sysmail_fast_send(pstEnv, pstPlayer->stRoleData.RoleName, LJY_MAIL14,
								LJY_MAIL15,
								stRoleGrid, iNum, 0,NULL, 0);
	}

	pstOnlineGiftGroupSvr->Type = SVR_GIFT_GROUP_TYPE_RES;
	pstOnlineGiftGroupSvr->GiftGroupData.GiftGroupRes.ID = iID;
	pstOnlineGiftGroupSvr->GiftGroupData.GiftGroupRes.Day = pstGiftOne->Day;
	Z_CSHEAD_INIT(&stPak.Head, ONLINEGIFT_GROUP_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPak);
	return 0;

}

int player_onlinegiftgroup_info(ZONESVRENV* pstEnv,Player *pstPlayer, GIFTGROUP *pstGiftGroup)
{
	int i;
	ONLINEGIFTGROUP *pstDef = z_find_onlinegroupgift(pstEnv, pstGiftGroup->ID);

	if (!pstDef)
		return -1;

	for (i=pstDef->OneNum-1; i>=0 ;i--)
	{
		if (pstGiftGroup->ContDay >= pstDef->GiftOnes[i].Day)
		{
			giftgroup_add_to_clt(pstEnv, pstPlayer,&pstDef->GiftOnes[i],pstDef->ID);
			break;
		}
	}
	return 0;
}

int player_up_giftgroup(ZONESVRENV* pstEnv,Player *pstPlayer, GIFTGROUP *pstGiftGroup)
{
	int i;
	ONLINEGIFTGROUP *pstDef = z_find_onlinegroupgift(pstEnv, pstGiftGroup->ID);

	if (!pstDef)
		return -1;

	pstGiftGroup->ContDay ++;
	pstGiftGroup->UpDayTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	for (i=pstDef->OneNum-1; i>=0 ;i--)
	{
		// 更新getflag
	//	if (pstGiftGroup->ContDay == pstDef->GiftOnes[i].Day)
		if (pstGiftGroup->ContDay >= pstDef->GiftOnes[i].Day)
		{
			pstGiftGroup->GetFlag = 0;
			break;
		}
	}
	return 0;
	UNUSED(pstPlayer);
}

int player_update_onlinegiftgroup(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	int i;
	GIFTGROUPS *pstGiftGroups = &pstPlayer->stRoleData.MiscInfo.GiftGroups;  
	ONEDAYTIME *pstOneDayTime = &pstPlayer->stRoleData.MiscInfo.ODTime;
	int iNotify = 0;
//	ONLINEGIFTGROUP *pstDef;

	for (i=0; i<pstGiftGroups->Num; i++)
	{
		iNotify = 0;
		if (!IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstGiftGroups->GiftGroups[i].UpDayTime))
		{
			if (	pstGiftGroups->GiftGroups[i].UpDayTime == 0 ||
				IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstGiftGroups->GiftGroups[i].UpDayTime+24*60*60))
			{
				if (pstOneDayTime->OndaySec >= pstGiftGroups->GiftGroups[i].Time*60)
				{
					player_up_giftgroup(pstEnv,pstPlayer, &pstGiftGroups->GiftGroups[i]);
					iNotify = 1;
				}
			}
			else
			{
				// 不是连续的天了
				pstGiftGroups->GiftGroups[i].ContDay  = 0;
				pstGiftGroups->GiftGroups[i].UpDayTime = 0;
				pstGiftGroups->GiftGroups[i].GetFlag = 0;
			}
		}

		if (iNotify)
		{
			player_onlinegiftgroup_info(pstEnv,pstPlayer, &pstGiftGroups->GiftGroups[i]);
		}
	}
	return 0;
}


int player_cdkey_req(ZONESVRENV* pstEnv, Player *pstPlayer, const char* pszCDKey)
{
	SSCDKEYREQ stReq;
	stReq.RoleID = pstPlayer->stRoleData.RoleID;
	stReq.SvrID = pstEnv->unWorldID;
	stReq.RoleLvl = pstPlayer->stRoleData.Level;
	STRNCPY(stReq.CDKey, pszCDKey, sizeof(stReq.CDKey));
	STRNCPY(stReq.RoleName, pstPlayer->stRoleData.RoleName, sizeof(stReq.RoleName));
	stReq.IP = pstPlayer->stConnectInfo.stExtraInfo.IPInfo.ip;
	STRNCPY(stReq.Pass9Account,pstPlayer->szAccount,sizeof(stReq.Pass9Account));

	shopping_ss_send(pstEnv->pstAppCtx,pstEnv,  SS_CDKEY_REQ,(char *)&stReq,sizeof(stReq));
	return 0;
}

int player_grade_d_pre_brk(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	//采集怪物
	if(CS_STAT_COLLECT & pstPlayer->stOnline.State)
	{
		int iIndex;
		
		iIndex = z_id_index(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.iCollectTarget, OBJ_MONSTER);
		if (iIndex >=0) 
		{
			TMEMBLOCK *pstMemBlock;
			Monster *pstMon = NULL;
			MONSTERDEF *pstMonsterDef = NULL;
				
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				return -1;
			}		
			
			pstMon = (Monster *)pstMemBlock->szData;
			if(!pstMon)
			{
				return -1;
			}
			pstMonsterDef = z_get_mon_def(pstEnv, pstMon);
			if(!pstMonsterDef)
			{
				return -1;
			}

			if(pstMonsterDef->MiscType & MON_MISC_TYPE_COLLECT_BREAK)
			{
				player_collect_break(pstEnv, pstPlayer, NULL);
			}
		}
	}
	
	return 0;
}

int player_grade_c_pre_brk(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	//移动引导技能
	if((pstPlayer->stOnline.State & CS_STAT_MOVE_ATK) &&
		pstPlayer->stOnline.stAtkInfo.unPilot > 0)
	{
		z_skill_break(pstEnv->pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstPlayer->stOnline.stAtkInfo.unSkillID);
	}

	return 0;
}

int player_grade_b_pre_brk(ZONESVRENV* pstEnv, Player *pstPlayer)
{	
	//采集怪物
	if(CS_STAT_COLLECT & pstPlayer->stOnline.State)
	{
		int iIndex;
		
		iIndex = z_id_index(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.iCollectTarget, OBJ_MONSTER);
		if (iIndex >=0) 
		{
			player_collect_break(pstEnv, pstPlayer, NULL);
		}
	}

	//召唤坐骑
	if(CS_STAT_RIDE_PREPARE & pstPlayer->stOnline.State)
	{
		z_ride_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	//出征宠物
	if(CS_STAT_CAMPAIGN_PRE & pstPlayer->stOnline.State)
	{
		z_pet_campaign_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	//出征精灵
	if (pstPlayer->stOnline.State & CS_STAT_FAIRY_CAMPAIGN_PRE)
	{
		fairy_camp_pre_brk(pstEnv, pstPlayer, pstPlayer->stOnline.stFairyOnline.ullWID);
		
		pstPlayer->stOnline.State &= ~CS_STAT_FAIRY_CAMPAIGN_PRE;
		pstPlayer->stOnline.stFairyOnline.ullWID = 0;
		pstPlayer->stOnline.stFairyOnline.uiCampPreTime = 0;
	}

	//物品使用吟唱
	if(pstPlayer->stOnline.stItemUseInfo.tPreTime > 0)
	{
		package_use_pre_brk(pstEnv, pstPlayer);
	}

	//生活制造吟唱
	if(pstPlayer->stOnline.State & CS_STAT_RECIPE_PREPARE)
	{
		z_recipe_brk(pstEnv, pstPlayer);
	}

	//技能吟唱
	if(pstPlayer->stOnline.State & CS_STAT_PREPARE)
	{
		z_skill_break(pstEnv->pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstPlayer->stOnline.stAtkInfo.unSkillID);
	}

	//引导技能
	if((pstPlayer->stOnline.State & CS_STAT_ATK) &&
		pstPlayer->stOnline.stAtkInfo.unPilot > 0)
	{
		z_skill_break(pstEnv->pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstPlayer->stOnline.stAtkInfo.unSkillID);
	}

	//机械下车吟唱
	if(is_player_ride_on_machine(pstPlayer) &&
		pstPlayer->stOnline.stMachineInfo.tLeavePreTime > 0)
	{
		player_machine_leave_pre_brk(pstEnv, pstPlayer);
	}

	if(pstPlayer->stOnline.State & CS_STAT_ARM_FENJIE_PRE)
	{
		arm_fenjie_pre_brk(pstEnv, pstPlayer,0);
	}

	
	if(pstPlayer->stOnline.State & CS_STAT_GEM_FENJIE_PRE)
	{
		arm_fenjie_pre_brk(pstEnv, pstPlayer,1);
	}
	return 0;
}

int player_grade_a_pre_brk(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	//采集战利品
	if(CS_STAT_COLLECT & pstPlayer->stOnline.State)
	{
		int iIndex;
		
		iIndex = z_id_index(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.iCollectTarget, OBJ_ITEM);
		if (iIndex >=0) 
		{
			player_collect_break(pstEnv, pstPlayer, NULL);
		}
	}

	return 0;
}

int player_trace_act_break_prepare(ZONESVRENV* pstEnv, Player *pstPlayer, int iActType)
{	
	switch(iActType)
	{
		//D类打断动作，特殊的怪
		case BRK_PRE_ACT_SPE_MON:
			goto _grade_d;
			break;
		//A类打断动作, 只会打断级别A的读条
		case BRK_PRE_ACT_BEATK:
			goto _grade_a;
			break;
		//B类打断动作，打断级别B，级别C的读条
		case BRK_PRE_ACT_MOVE:
			goto _grade_b;
			break;
		//C类打断动作是最低的都包含
		case BRK_PRE_ACT_SELF_NOPRE_ATK:
	 	case BRK_PRE_ACT_BEATK_CTRLSKILL:
		case BRK_PRE_ACT_BEATK_CTRLSTATUS:
		case BRK_PRE_ACT_STALL:
		case BRK_PRE_ACT_DIE:
		case BRK_PRE_ACT_LOGOUT:
		case BRK_PRE_ACT_CHG_MAP:
		case BRK_PRE_ACT_SHAM_DEAD:
		case BRK_PRE_ACT_SPAN_SVR:
			goto _grade_c;
			break;
		default:
			return -1;
			break;
	}
	
_grade_d:
	player_grade_d_pre_brk(pstEnv, pstPlayer);
	return 0;

_grade_c:

	player_grade_c_pre_brk(pstEnv, pstPlayer);

_grade_b:

	player_grade_b_pre_brk(pstEnv, pstPlayer);
	
_grade_a:

	player_grade_a_pre_brk(pstEnv, pstPlayer);
	
	return 0;
}

int player_pworld_offexp_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{	
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	LEVELUP *pstLevelUP = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);

	if(NULL == pstLevelUP)
	{
		return -1;
	}
	
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_OFFLINE_EXP;
	pstActionRes->Data.OfflineExpInfo.Exp = pstPlayer->stRoleData.MiscInfo.OfflineExpInfo.PworldOffExp;
	pstActionRes->Data.OfflineExpInfo.Type = CSOFFLINE_EXP_PWORLD;
	pstActionRes->Data.OfflineExpInfo.MaxExp = pstLevelUP->MaxPworldOffExp; 
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_pworld_offexp_change(ZONESVRENV* pstEnv, Player *pstPlayer, int iExp)
{
	LEVELUP *pstLevelUP = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	OFFLINEEXPINFO *pstOffExpInfo = &pstPlayer->stRoleData.MiscInfo.OfflineExpInfo;

	if(NULL == pstLevelUP || iExp == 0)
	{
		return -1;
	}

	if(pstOffExpInfo->PworldOffExp + iExp > pstLevelUP->MaxPworldOffExp)
	{
		pstOffExpInfo->PworldOffExp = pstLevelUP->MaxPworldOffExp;
	}
	else
	{
		pstOffExpInfo->PworldOffExp += iExp;
	}

	if(pstOffExpInfo->PworldOffExp < 0)
	{
		pstOffExpInfo->PworldOffExp = 0;
	}
	
	player_pworld_offexp_action(pstEnv, pstPlayer);
	return 0;
}

int player_pworld_award_offexp(ZONESVRENV* pstEnv, Player *pstPlayer, int iAwardExp)
{
	OFFLINEEXPINFO *pstOffExpInfo = &pstPlayer->stRoleData.MiscInfo.OfflineExpInfo;

	if(pstOffExpInfo->PworldOffExp >= iAwardExp)
	{
		player_pworld_offexp_change(pstEnv, pstPlayer, -iAwardExp);
		return iAwardExp;
	}
	else
	{
		int iExp = pstOffExpInfo->PworldOffExp;
		player_pworld_offexp_change(pstEnv, pstPlayer, -pstOffExpInfo->PworldOffExp);
		return iExp;
	}
}

int player_pworld_max_expmul_get(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	int iMax = 0;
	PWORLDDEF *pstPworldDef;

	for(i = 0; i<pstEnv->pstMapObj->iPworld; i++)
	{
		pstPworldDef = (PWORLDDEF *)pstEnv->pstMapObj->sPworld[i];
		if(pstPworldDef->OffExpMul <= 0)
		{
			continue;
		}

		if(pstPlayer->stRoleData.Level >= pstPworldDef->MinLv)
		{
			iMax += pstPworldDef->OffExpMul;
		}
	}

	return iMax;
}

int player_pworld_off_exp(ZONESVRENV* pstEnv, Player *pstPlayer, int iNotify)
{
	OFFLINEEXPINFO *pstOffExpInfo = &pstPlayer->stRoleData.MiscInfo.OfflineExpInfo;

	if(pstOffExpInfo->UsePworldMulTime == 0)
	{
		pstOffExpInfo->UsePworldMulTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	else if(!IsSameDay(pstOffExpInfo->UsePworldMulTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		time_t tTime;
		double iExp, iDay = 0, iMul = 0;
		int iMaxPworldExpMul = player_pworld_max_expmul_get(pstEnv, pstPlayer);
		int iSec = pstEnv->pstAppCtx->stCurr.tv_sec -pstOffExpInfo->UsePworldMulTime;
		LEVELUP *pstLevelUP = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);

		iMul = iMaxPworldExpMul -pstOffExpInfo->UsePworldMul;
		if(iMul < 0)
		{
			iMul = 0;
		}
		
		if(iSec > 86400)
		{
			iDay += iSec/86400;
			if(iDay > 0)
			{
				tTime = pstOffExpInfo->UsePworldMulTime + iDay*86400;
				if(IsSameDay(tTime, pstEnv->pstAppCtx->stCurr.tv_sec))
				{
					iDay = iDay -1;
				}
				
				iMul += iDay*iMaxPworldExpMul;
			}
		}

		if(pstLevelUP && iMul > 0)
		{
			iExp = (ulonglong )pstLevelUP->ContributeExp*iMul/10000.0;
			player_pworld_offexp_change(pstEnv, pstPlayer, iExp);
		}

		pstOffExpInfo->UsePworldMul = 0;
		pstOffExpInfo->UsePworldMulTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	if(iNotify)
	{
		player_pworld_offexp_action(pstEnv, pstPlayer);
	}

	return 0;
}

int player_pworld_offmul_chg(ZONESVRENV* pstEnv, Player *pstPlayer, int iMul)
{
	OFFLINEEXPINFO *pstOffExpInfo = &pstPlayer->stRoleData.MiscInfo.OfflineExpInfo;
	
	player_pworld_off_exp( pstEnv, pstPlayer, 1);

	pstOffExpInfo->UsePworldMul += iMul;
	if(pstOffExpInfo->UsePworldMul < 0)
	{
		pstOffExpInfo->UsePworldMul = 0;
	}
	
	return 0;
}

ATKERHURTMULDEF *z_find_atker_hurtmul_def(ZONESVRENV *pstEnv, int iAtkerType)
{
	int iIndex;
	int iEqu;
	ATKERHURTMULDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.AtkerType = iAtkerType;
	iIndex = bsearch_int(&stDef, pstObj->sAtkerHurtMulDef, pstObj->iAtkerHurtMulDef, ATKER_HURT_MUL_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ATKERHURTMULDEF *)pstObj->sAtkerHurtMulDef[iIndex];
	}
	
	return NULL;	
}

int atker_type_get(ZoneAni *pstAni, SKILLDEF *pstSkillDef)
{
	if(pstAni->iType == OBJ_PLAYER)
	{
		if(pstSkillDef && pstSkillDef->BindSkill == BINDSKILL_TYPE_CALN_ITEM)
		{
			return ATKER_HURT_TYPE_SKILL_ITEM;
		}
		else if(pstSkillDef && pstSkillDef->FairySkill)
		{
			return ATKER_HURT_TYPE_FAIRY;
		}
		else
		{
			return ATKER_HURT_TYPE_PLAYER;
		}
	}
	else if(pstAni->iType == OBJ_MONSTER)
	{
		Monster *pstMon = pstAni->stObj.pstMon;
		
		if(is_mon_machine(pstMon))
		{
			return ATKER_HURT_TYPE_MACHINE;
		}
		else if(pstMon->bSubType == MON_SUB_TYPE_MINI ||
			pstMon->bSubType == MON_SUB_TYPE_SOFT ||
			pstMon->bSubType == MON_SUB_TYPE_WEAK ||
			pstMon->bSubType == MON_SUB_TYPE_NORM)
		{
			return ATKER_HURT_TYPE_NORMAL_MON;
		}
		else if(pstMon->bSubType == MON_SUB_TYPE_JIN)
		{
			return ATKER_HURT_TYPE_JING_MON;
		}
		else if(pstMon->bSubType == MON_SUB_TYPE_JI)
		{
			return ATKER_HURT_TYPE_JI;
		}
		else if(pstMon->bSubType == MON_SUB_TYPE_XI ||
			pstMon->bSubType == MON_SUB_TYPE_BOSS  ||
			pstMon->bSubType == MON_SUB_TYPE_HARD)
		{
			return ATKER_HURT_TYPE_BOSS_MON;
		}
		else if(pstMon->bSubType == MON_SUB_TYPE_TOTEM)
		{
			return ATKER_HURT_TYPE_TOTEM;
		}
	}
	else if(pstAni->iType == OBJ_PET)
	{
		return ATKER_HURT_TYPE_PET;
	}

	return -1;
}

int atker_hurt_mul_get(ZONESVRENV *pstEnv, AtkerInfo *pstAtkerInfo, AtkeeInfo *pstAtkeeInfo)
{
	int iAtkerType;
	ATKERHURTMULDEF *pstAtkerHurtMulDef;
	ZoneAni *pstAni = &pstAtkeeInfo->stAtkee;

	iAtkerType = atker_type_get(&pstAtkerInfo->stAtker, pstAtkerInfo->pstSkillDef);
	if(iAtkerType <= 0)
	{
		return 100;
	}

	pstAtkerHurtMulDef = z_find_atker_hurtmul_def(pstEnv, iAtkerType);
	if(NULL == pstAtkerHurtMulDef)
	{
		return 100;
	}

	if(pstAni->iType == OBJ_PLAYER)
	{
		return pstAtkerHurtMulDef->PlayerMul;
	}
	else if(pstAni->iType == OBJ_MONSTER)
	{
		Monster *pstMon = pstAni->stObj.pstMon;
		
		if(is_mon_machine(pstMon))
		{
			return pstAtkerHurtMulDef->Machine;
		}
		else if(pstMon->bSubType == MON_SUB_TYPE_MINI ||
			pstMon->bSubType == MON_SUB_TYPE_SOFT ||
			pstMon->bSubType == MON_SUB_TYPE_WEAK ||
			pstMon->bSubType == MON_SUB_TYPE_NORM)
		{
			return pstAtkerHurtMulDef->NormalMonMul;
		}
		else if(pstMon->bSubType == MON_SUB_TYPE_JIN)
		{
			return pstAtkerHurtMulDef->JingMonMul;
		}
		else if(pstMon->bSubType == MON_SUB_TYPE_JI)
		{
			return pstAtkerHurtMulDef->Ji;
		}
		else if(pstMon->bSubType == MON_SUB_TYPE_XI ||
			 pstMon->bSubType == MON_SUB_TYPE_HARD ||
			pstMon->bSubType == MON_SUB_TYPE_BOSS)
		{
			return pstAtkerHurtMulDef->BossMonMul;
		}
		else if(pstMon->bSubType == MON_SUB_TYPE_CLAN_FACTORY)
		{
			return pstAtkerHurtMulDef->Factory;
		}
		else if(pstMon->bSubType == MON_SUB_TYPE_RONGLU)
		{
			return pstAtkerHurtMulDef->RongLuMul;
		}
	}
	else if(pstAni->iType == OBJ_PET)
	{
		return pstAtkerHurtMulDef->PetMul;
	}
	
	return 100;
}


int raffle_get_rand_index(ZONESVRENV* pstEnv, RNDAWDDEF *pstRndDef)
{
	int i,j;
	int iRate = 0;
	int iRnd;
	for(i=0; i<RAND_ITEM_NUM; i++)
	{
		iRate += pstRndDef->RandIAwards[i].Rate;
	}

	iRnd = (int)RAND1(iRate);

	iRate = 0;

	for (i=0; i<RAND_ITEM_NUM; i++)
	{
		iRate += pstRndDef->RandIAwards[i].Rate;
		if (iRnd < iRate)
		{
			if (world_item_limit_check(pstEnv, pstRndDef->RandIAwards[i].ItemID) < 0)
			{
				int iMaxIdx = 0;
				int iMaxRate = pstRndDef->RandIAwards[0].Rate;
				for (j=1; j<RAND_ITEM_NUM; j++)
				{
					if (iMaxRate < pstRndDef->RandIAwards[j].Rate)	
					{
						iMaxRate = pstRndDef->RandIAwards[j].Rate;
						iMaxIdx = j;
					}
				}

				// 最高概率的就不记录了，直接返回
				return iMaxIdx;
			}

			world_item_limit_add(pstEnv, pstRndDef->RandIAwards[i].ItemID, pstRndDef->RandIAwards[i].Num);
			return i;
		}
	}
	return -1;
}

int player_raffle_res(ZONESVRENV* pstEnv, Player *pstPlayer, int iIdx,int iType)
{
	CSPKG stPak;
	CSRAFFLESVR *pstRaffleSvr = &stPak.Body.CsRaffleSvr;

	pstRaffleSvr->Idx = iIdx;
	pstRaffleSvr->Type = iType;

	Z_CSHEAD_INIT(&stPak.Head, RAFFLE_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPak);
	return 0;
}

int player_raffle_start(ZONESVRENV* pstEnv, Player *pstPlayer,CSRAFFLECLT *pstRaffleClt)
{
	RNDAWDDEF *pstRndDef;
	int iIdx = -1;
	
	pstRndDef = z_find_rndawddef(pstEnv, pstRaffleClt->Idx);
	if (!pstRndDef)
	{
		return -1;
	}

	if (pstRndDef->Type != pstRaffleClt->Type)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "player_raffle pstRndDef->Type != pstRaffleClt->Type error ");
		return -1;
	}

	if (package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BINDGODCOIN, pstRndDef->GoldUse) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOMONEY1);
		return -1;
	}
	
	if (pstRaffleClt->Type == RND_AWD_TYPE_BCT)
	{
		if (pstPlayer->stOnline.stBctOnline.iRaffleNum <= 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "player_raffle_start_bct RaffleNum error");
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_MISC25);
			goto final;
		}
		pstPlayer->stOnline.stBctOnline.iRaffleNum --;
	}
	else
	{
		if (pstPlayer->stRoleData.MiscInfo.GoldRaffle >= GOLD_RAFFLE_MAX_NUM)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_MISC26);
			tlog_error(pstEnv->pstLogCat, 0, 0, "player_raffle_start_gold GoldRaffle error ");
			goto final;
		}
		pstPlayer->stRoleData.MiscInfo.GoldRaffle ++;
	}

	pstPlayer->stOnline.stBctOnline.iRndDefIdx =  pstRaffleClt->Idx;
	iIdx = raffle_get_rand_index(pstEnv,pstRndDef);
	if(iIdx == -1)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "player_raffle_start_bct iIdx error ");
		goto final;
	}

	// 掉线后会变成0,所以这里加1
	pstPlayer->stOnline.stBctOnline.iItemIdx = iIdx+1;
	package_dec_money(pstEnv, pstPlayer, ATTR_ID_BINDGODCOIN, pstRndDef->GoldUse);
	z_bindgold_consume_oplog(pstEnv, pstPlayer, BINDGOLD_OPTYPE_RAFFLE,
								pstRaffleClt->Type, pstRndDef->GoldUse, "转盘抽奖");

final:
	player_raffle_res(pstEnv, pstPlayer, iIdx,pstRaffleClt->Type);	
	return 0;
}

int player_raffle_end(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	RNDAWDDEF *pstRndDef;
	RANDAWARD *pstAward;
	ITEMDEF *pstItemDef;
	int iRet;
	int iOpType;
	
	pstRndDef = z_find_rndawddef(pstEnv, pstPlayer->stOnline.stBctOnline.iRndDefIdx);
	if (!pstRndDef || pstRndDef->RndIdx != pstPlayer->stOnline.stBctOnline.iRndDefIdx)
	{
		return -1;
	}

	if (	pstPlayer->stOnline.stBctOnline.iItemIdx <=0 || 
		pstPlayer->stOnline.stBctOnline.iItemIdx >RAND_ITEM_NUM)
	{
		return -1;
	}

	if (pstRndDef->Type == RND_AWD_TYPE_BCT)
	{
		iOpType = OP_BCT_END;
	}
	else
	{
		iOpType = OP_RAFFLE_GOLD;
	}
	
	pstAward = &pstRndDef->RandIAwards[pstPlayer->stOnline.stBctOnline.iItemIdx-1];
	// 金钱||布尔
	if (pstAward->Type == 0 ||pstAward->Type == 1)
	{
		int iType;
		char szMoneyName[32];
		if(pstAward->Type == 0)
		{
			iType = ATTR_ID_MONEY;
		}
		else
		{
			iType = ATTR_ID_BULL;
		}
		money_type_to_string(iType,szMoneyName,sizeof(szMoneyName));
		
		if (pstAward->Num > 0)
		{
			if (package_add_money_test(pstEnv, pstPlayer, iType,pstAward->Num) == 0 )
			{
				package_add_money(pstEnv, pstPlayer, iType,pstAward->Num);
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_MISC27,
							pstAward->Num, szMoneyName);
				{
					ROLEGRID stRoleGrid;
					memset(&stRoleGrid, 0, sizeof(stRoleGrid));
					stRoleGrid.GridData.RoleItem.DefID = BOOTY_MONEY_ID;
					stRoleGrid.GridData.RoleItem.Flag = pstAward->Num;
					z_role_pickitem_oplog( pstEnv,pstPlayer, &stRoleGrid, 1, iOpType);		
				}

				if (iType == ATTR_ID_MONEY)
				{
					z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_RAFFLE, 0, 0, pstAward->Num, "抽奖");
				}
				else
				{
					z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_RAFFLE, 0, pstAward->Num, 0, "抽奖");
				}
			}
		}
	}
	// 道具
	else if(pstAward->Type == 2)
	{
		if(pstAward->ItemID > 0)
		{
			iRet = accord_player_item(pstEnv, pstPlayer, pstAward->ItemID, pstAward->Num, 1,iOpType,
									LJY_ACCORD_ITEM1,LJY_ACCORD_ITEM2,1);
			if (iRet == 0)
			{
				pstItemDef = z_find_itemdef(pstEnv, pstAward->ItemID);
				if (pstItemDef)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_MISC27, 
								pstAward->Num, pstItemDef->Name);

				}
			}
		}
	}
	else
	{
		return -1;
	}

	pstPlayer->stOnline.stBctOnline.iRndDefIdx = 0;
	pstPlayer->stOnline.stBctOnline.iItemIdx = 0;
	
	return 0;
}

int player_raffle(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						     TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{	
	CSRAFFLECLT *pstRaffleClt = &pstCsPkg->Body.CsRaffleClt;

	// start
	if (!pstRaffleClt->StartFlag)
	{
		return player_raffle_start(pstEnv, pstPlayer,pstRaffleClt);
	}
	// end
	else
	{
		return player_raffle_end(pstEnv, pstPlayer);
	}
	UNUSED(pstAppCtx);
	UNUSED(pstFrameHead);
	return 0;
}


/*
int player_arm_val_get(ZONESVRENV *pstEnv, ROLEGRID *pstRoleGrid)
{
	ITEMDEF *pstItemDef;
	int iVal = 0, iImportant = 0;

	pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstRoleGrid->GridData.RoleItem);
	if(pstItemDef ==NULL)
	{
		return -1;
	}

	iVal = pstRoleGrid->GridData.RoleArm.StarLv*10 + powf(3, pstItemDef->Important+1);
	
	return 0;
}
*/


int active_clt_trans(ZONESVRENV* pstEnv, Player *pstPlayer, ACTIVETRANS *pstTrans)
{
	ACTIVEHELPER *pstActiveHelper;
	
	pstActiveHelper = z_find_active_help_def(pstEnv, pstTrans->ActiveID, NULL);
	if (NULL == pstActiveHelper)
	{
		return -1;
	}

	if (	0 == pstActiveHelper->TransMapID || 
		0 ==  pstActiveHelper->TransPos.X || 
		0==  pstActiveHelper->TransPos.Y)
	{
		return -1;
	}

	if (z_player_goto_check( pstEnv, pstPlayer,  pstActiveHelper->TransMapID, &pstActiveHelper->TransPos) < 0)
	{
		return -1;
	}

	z_player_goto(pstEnv, pstPlayer,pstActiveHelper->TransMapID, &pstActiveHelper->TransPos);
	return 0;
}

int active_clt_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	ACTIVECLT *pstClt = &pstCsPkg->Body.ActiveClt;

	if (pstClt->Cmd == ACTIVE_CLT_OP_TRANS)
	{
		active_clt_trans(pstEnv, pstPlayer, &pstClt->Data.Trans);	
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

LEVELEXPDEF *z_find_level_exp_def(ZONESVRENV* pstEnv, int iLevel)
{
	LEVELEXPDEF  stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Level = iLevel;
	iIndex = bsearch_int(&stDef, pstObj->sLevelExpDef, pstObj->iLevelExpDef, LEVEL_EXP_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (LEVELEXPDEF *)&pstObj->sLevelExpDef[iIndex];
	}
	return NULL;
}

LEVELBUFFEXPDEF *z_find_level_buff_exp_def(ZONESVRENV* pstEnv, int iLevel)
{
	LEVELBUFFEXPDEF  stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Level = iLevel;
	iIndex = bsearch_int(&stDef, pstObj->sLevelBuffExpDef, pstObj->iLevelBuffExpDef, LEVEL_BUFF_EXP_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (LEVELBUFFEXPDEF *)&pstObj->sLevelBuffExpDef[iIndex];
	}
	return NULL;
}

//等级差经验补偿
int player_level_exp_mul_get(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iLevel;
	LEVELEXPDEF *pstLevelExpDef;
	RangeOneMem *pstRange;
	
	if(pstPlayer->stRoleData.Level <= 40 || pstPlayer->stRoleData.Level == 59)
	{
		return 0;
	}
	
	pstRange = range_find_onemem(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_LEVEL);
	if(!pstRange)
	{
		return 0;
	}

	if(pstRange->stLastRange.iNum <= 0)
	{
		return 0;
	}

	pstLevelExpDef = z_find_level_exp_def(pstEnv, pstRange->stLastRange.astRangeEntry[0].stRange.RangeData.Level);
	if(!pstLevelExpDef)
	{
		return 0;
	}

	iLevel = pstRange->stLastRange.astRangeEntry[0].stRange.RangeData.Level - pstPlayer->stRoleData.Level;
	if(iLevel <= 0)
	{
		return 0;
	}

	iLevel = iLevel > MAX_LEVEL_EXP_LIST? MAX_LEVEL_EXP_LIST : iLevel;
	return pstLevelExpDef->ExpMul[iLevel-1];
}

//经验值补偿
int player_world_level_exp(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_longlong *piExp)
{
	int iMul = 0;
	int iAddLevelExp = 0;
	GLOBAL *pstGlobal = &pstEnv->pstGlobalObj->stGlobal;
	
	if (pstEnv->pstConf->NoLevelFeng == 0 && pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat == GLOBAL_FINI &&
		pstGlobal->LevFeng.LevelFeng > 0 && pstPlayer->stRoleData.Level >= pstGlobal->LevFeng.LevelFeng)
	{
		iMul = level_feng_get_expdec(pstEnv, pstPlayer);
		*piExp = *piExp*iMul/100.0;
		return 0;
	}


	iMul = player_level_exp_mul_get(pstEnv, pstPlayer);
	if(iMul <= 0)
	{
		return 0;
	}
	
	iAddLevelExp = *piExp*iMul/100.0;
	if(iAddLevelExp <= 0)
	{
		return 0;
	}
	
	pstPlayer->stOnline.iAddWorldLevelExp = iAddLevelExp;
	*piExp += iAddLevelExp;
	
	return 0;
}

//离线经验一次性使用检查
int player_use_all_offline_exp_check(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	AccShareData* pstData = &pstPlayer->stAccShareData;
	
	if (ACC_SHARE_FINAL != pstData->iStat)
	{
		return -1;
	}

	if(pstData->stAccShare.OfflineExp <= 0)
	{
		return -1;
	}

	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

//离线经验一次性使用
int player_use_all_offline_exp(ZONESVRENV *pstEnv,Player *pstPlayer, int iAddMul)
{
	
	PlayerChagAttrVal stChgVal;
	AttrChgVal *pstVal;
	AccShareData* pstData = &pstPlayer->stAccShareData;
	long long iExp = 0;
	
	if (ACC_SHARE_FINAL != pstData->iStat)
	{
		return -1;
	}

	iExp = pstData->stAccShare.OfflineExp*((100+iAddMul)/100.0);
	if (iExp <= 0)
	{
		return 0;
	}

	stChgVal.AttrNum = 0;
	if(iExp > 0)
	{
		pstVal = &stChgVal.AttrList[stChgVal.AttrNum++];
		pstVal->AttrID = ATTR_ID_EXP;
		pstVal->nFlag = 0;
		pstVal->Value.Exp = iExp;
	}

	if (stChgVal.AttrNum > 0)
	{
		z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);
	}

	z_player_awardexp_change(pstEnv,pstPlayer,-iExp,TRUE);	
	return 0;
}

//活动补偿经验一次性使用检查
int player_use_all_pworld_offexp_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	OFFLINEEXPINFO *pstOffExpInfo = &pstPlayer->stRoleData.MiscInfo.OfflineExpInfo;

	if(pstOffExpInfo->PworldOffExp <= 0)
	{
		return -1;
	}
	
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

//活动补偿经验一次性使用
int player_use_all_pworld_offexp(ZONESVRENV* pstEnv, Player *pstPlayer, int iAddMul)
{
	PlayerChagAttrVal stChgVal;
	AttrChgVal *pstVal;
	long long iExp = 0;
	OFFLINEEXPINFO *pstOffExpInfo = &pstPlayer->stRoleData.MiscInfo.OfflineExpInfo;

	if(pstOffExpInfo->PworldOffExp <= 0)
	{
		return 0;
	}

	iExp = pstOffExpInfo->PworldOffExp *((100+iAddMul)/100.0);
	if (iExp <= 0)
	{
		return 0;
	}

	stChgVal.AttrNum = 0;
	if(iExp > 0)
	{
		pstVal = &stChgVal.AttrList[stChgVal.AttrNum++];
		pstVal->AttrID = ATTR_ID_EXP;
		pstVal->nFlag = 0;
		pstVal->Value.Exp = iExp;
	}

	if (stChgVal.AttrNum > 0)
	{
		z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);
	}
	player_pworld_offexp_change(pstEnv, pstPlayer, -pstOffExpInfo->PworldOffExp);
	return 0;
}

//经验BUFF 每跳的经验
int player_level_buff_exp_add(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iExp;
	PlayerChagAttrVal stChgVal;
	AttrChgVal *pstVal;

	iExp = pstPlayer->stRoleData.MiscInfo.LevelBuffExpInfo.Exp;
	if(0 > iExp)
	{
		return 0;
	}

	stChgVal.AttrNum = 0;
	pstVal = &stChgVal.AttrList[stChgVal.AttrNum++];
	pstVal->AttrID = ATTR_ID_EXP;
	pstVal->nFlag = 0;
	pstVal->Value.Exp = iExp;
	z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);
	return 0;
}

//角色登录等级补偿BUFF信息
int player_login_level_buff_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iMul = 0;
	RangeOneMem *pstRange;
	LEVELBUFFEXPDEF *pstLevelBuffExpDef;
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	GLOBAL *pstGlobal;
	int iExp = 0;

	if(!is_fini_global(pstEnv))
	{
		return 0;
	}
	pstGlobal = &pstEnv->pstGlobalObj->stGlobal;

	if(IsSameDay(pstPlayer->stRoleData.MiscInfo.LevelBuffExpInfo.DayTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		return 0;
	}

	pstLevelBuffExpDef = z_find_level_buff_exp_def(pstEnv, pstPlayer->stRoleData.Level);
	if(!pstLevelBuffExpDef)
	{
		return 0;
	}
	
	pstRange = range_find_onemem(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_LEVEL);
	if(!pstRange)
	{
		return 0;
	}

	if(pstRange->stLastRange.iNum <= 0)
	{
		return 0;
	}

	if(pstLevelBuffExpDef->LevelAwardLimit > 0 &&
		pstPlayer->stRoleData.MiscInfo.LevelBuffExpInfo.AwardNum >= pstLevelBuffExpDef->LevelAwardLimit)
	{
		goto end;
	}

	if (pstEnv->pstConf->LevelExpBuffLimit > 0)
	{
		if(pstPlayer->stRoleData.Level > pstEnv->pstConf->LevelExpBuffLimit)
		{
			goto end;
		}
	}
	else
	{
		if(pstLevelBuffExpDef->DayLimit > 0)
		{
			int iDay;
			iDay = (pstEnv->pstAppCtx->stCurr.tv_sec - pstGlobal->Misc.NewWorldFlags.FirstStartupTime)/86400;
			if(pstLevelBuffExpDef->DayLimit > iDay)
			{
				goto end;
			}
		}
		
		if(pstPlayer->stRoleData.Level + 5 > pstRange->stLastRange.astRangeEntry[0].stRange.RangeData.Level)
		{
			goto end;
		}
	}

	if(0 > pstLevelBuffExpDef->Exp || 0 > pstLevelBuffExpDef->Time)
	{
		goto end;
	}

	iMul = player_level_exp_mul_get(pstEnv, pstPlayer);

	iExp = pstLevelBuffExpDef->Exp*(100+iMul)/100*pstLevelBuffExpDef->Time/10;
	
end:
	pstMiscSvr->PlayerMiscSvrData.ExpBuffInfo.Day = 0;
	pstMiscSvr->PlayerMiscType = PLAYER_EXP_BUFF_INFO;
	pstMiscSvr->PlayerMiscSvrData.ExpBuffInfo.Exp = iExp;

	if (player_is_huiliu(pstPlayer))
	{
		// 流失天数,客户端根据这个天数确认界面的回流奖励
		pstMiscSvr->PlayerMiscSvrData.ExpBuffInfo.Day = pstPlayer->stRoleData.MiscInfo.LevelBuffExpInfo.BuchangInfo.Day;
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_level_buff_award(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iMul = 0;
	RangeOneMem *pstRange;
	LEVELBUFFEXPDEF *pstLevelBuffExpDef;
	GLOBAL *pstGlobal;

	if(!is_fini_global(pstEnv))
	{
		return 0;
	}
	pstGlobal = &pstEnv->pstGlobalObj->stGlobal;

	if(IsSameDay(pstPlayer->stRoleData.MiscInfo.LevelBuffExpInfo.DayTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		return 0;
	}

	pstLevelBuffExpDef = z_find_level_buff_exp_def(pstEnv, pstPlayer->stRoleData.Level);
	if(!pstLevelBuffExpDef)
	{
		return 0;
	}
	
	pstRange = range_find_onemem(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_LEVEL);
	if(!pstRange)
	{
		return 0;
	}

	if(pstRange->stLastRange.iNum <= 0)
	{
		return 0;
	}

	if (pstEnv->pstConf->LevelExpBuffLimit > 0)
	{
		if(pstPlayer->stRoleData.Level > pstEnv->pstConf->LevelExpBuffLimit)
		{
			return 0;
		}
	}
	else
	{
		if(pstPlayer->stRoleData.Level + 5 > pstRange->stLastRange.astRangeEntry[0].stRange.RangeData.Level)
		{
			return 0;
		}

		if(pstLevelBuffExpDef->DayLimit > 0)
		{
			int iDay;
			iDay = (pstEnv->pstAppCtx->stCurr.tv_sec - pstGlobal->Misc.NewWorldFlags.FirstStartupTime)/86400;
			if(pstLevelBuffExpDef->DayLimit > iDay)
			{
				return 0;
			}
		}
	}

	if(pstLevelBuffExpDef->LevelAwardLimit > 0 &&
		pstPlayer->stRoleData.MiscInfo.LevelBuffExpInfo.AwardNum >= pstLevelBuffExpDef->LevelAwardLimit)
	{
		return 0;
	}

	if(0 > pstLevelBuffExpDef->Exp || 0 > pstLevelBuffExpDef->Time)
	{
		return 0;
	}

	iMul = player_level_exp_mul_get(pstEnv, pstPlayer);

	pstPlayer->stRoleData.MiscInfo.LevelBuffExpInfo.AwardNum++;

	pstPlayer->stRoleData.MiscInfo.LevelBuffExpInfo.DayTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstPlayer->stRoleData.MiscInfo.LevelBuffExpInfo.Exp = pstLevelBuffExpDef->Exp*(100+iMul)/100.0;
	z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
		WORLD_LEVEL_EXP_BUFF_ID, 1, pstLevelBuffExpDef->Time, 1, 0);

	return 0;
}

//广播世界最高等级
int world_max_level_broadcast(ZONESVRENV* pstEnv)
{
	int iMaxLevel;
	RangeOneMem *pstRange;
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;

	pstRange = range_find_onemem(pstEnv, RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_LEVEL);
	if(!pstRange)
	{
		return 0;
	}

	if(pstRange->stLastRange.iNum <= 0)
	{
		iMaxLevel = 0;
	}
	else
	{
		iMaxLevel = pstRange->stLastRange.astRangeEntry[0].stRange.RangeData.Level;
	}

	pstMiscSvr->PlayerMiscType = PLAYER_WORLD_MAX_LEVEL;
	pstMiscSvr->PlayerMiscSvrData.MaxLevel.Level = iMaxLevel;
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
	
	return 0;
}

int is_mt_gift_item(int iItemID)
{
	if(iItemID == MT_DAY_GIFT_ITEMID)
	{
		return 1;
	}
	
	return 0;
}

int player_mt_gift_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	
	pstMiscSvr->PlayerMiscType = PLAYER_MT_GIFT_INFO;
	pstMiscSvr->PlayerMiscSvrData.MTGiftInfo.EndTime = pstPlayer->stRoleData.MiscInfo.MTGiftInfo.EndTime;
	pstMiscSvr->PlayerMiscSvrData.MTGiftInfo.AwardTime = pstPlayer->stRoleData.MiscInfo.MTGiftInfo.AwardTime;
	pstMiscSvr->PlayerMiscSvrData.MTGiftInfo.AwardMoneyVal = MT_DAY_GIFT_MONEY_VAL;

	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int player_mt_gift_add_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if(pstPlayer->stRoleData.MiscInfo.MTGiftInfo.EndTime > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return -1;
	}

	return 0;
}

int player_mt_gift_add(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if(0 > player_mt_gift_add_check(pstEnv, pstPlayer))
	{
		return -1;
	}

	pstPlayer->stRoleData.MiscInfo.MTGiftInfo.EndTime  = pstEnv->pstAppCtx->stCurr.tv_sec + 29*24*60*60;
	pstPlayer->stRoleData.MiscInfo.MTGiftInfo.AwardTime = 0;
	player_mt_gift_info(pstEnv, pstPlayer);

	return 0;
}

int player_mt_gift_award(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if(pstPlayer->stRoleData.MiscInfo.MTGiftInfo.EndTime < pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return -1;
	}

	if(IsSameDay(pstPlayer->stRoleData.MiscInfo.MTGiftInfo.AwardTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		return -1;
	}

	pstPlayer->stRoleData.MiscInfo.MTGiftInfo.AwardTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	package_add_money(pstEnv, pstPlayer, ATTR_ID_BINDGODCOIN, MT_DAY_GIFT_MONEY_VAL);

	z_bindgold_create_oplog(pstEnv, pstPlayer, BINDGOLD_OPTYPE_MT_GIFT, 0,MT_DAY_GIFT_MONEY_VAL, "成长基金");
	//z_bindgold_consume_oplog(pstEnv, pstPlayer, BINDGOLD_OPTYPE_MT_GIFT, 0,
	//				MT_DAY_GIFT_MONEY_VAL,"成长基金");

	player_mt_gift_info(pstEnv, pstPlayer);

	return 0;
}

YANGHAIBANGDEF *z_find_yanghaibang_def(ZONESVRENV* pstEnv, int iLevel, int iDayNum)
{
	YANGHAIBANGDEF  stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Level = iLevel;
	stDef.DayNum = iDayNum;
	iIndex = bsearch_int(&stDef, pstObj->sYangHaiBangDef, pstObj->iYangHaiBangDef, YANGHAIBANG_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (YANGHAIBANGDEF *)&pstObj->sYangHaiBangDef[iIndex];
	}
	return NULL;
}

int player_yanghaibang_day_update(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	YANGHAIBANGINFO *pstYangHaiBangInfo = &pstPlayer->stRoleData.MiscInfo.YangHaiBangInfo;

	if(!IsSameDay(pstYangHaiBangInfo->DayTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstYangHaiBangInfo->DayTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstYangHaiBangInfo->Num = 0;
	}
	
	return 0;
}

int player_yanghaibang_open_ui(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	CSYANGHAIBANGUIINFO *pstYangHaiBangUIInfo = &pstMiscSvr->PlayerMiscSvrData.YangHaiBangUIInfo;
	YANGHAIBANGINFO *pstYangHaiBangInfo = &pstPlayer->stRoleData.MiscInfo.YangHaiBangInfo;

	pstMiscSvr->PlayerMiscType = PLAYER_YANGHAIBANG_UI_INFO;
	pstYangHaiBangUIInfo->EndTime = pstYangHaiBangInfo->EndTime;
	pstYangHaiBangUIInfo->Level = pstYangHaiBangInfo->Level;
	pstYangHaiBangUIInfo->Val = pstYangHaiBangInfo->Val;
	pstYangHaiBangUIInfo->tLifeEnd = pstYangHaiBangInfo->tLifeEnd;

	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

//lzk    给player_yanghaibang_login_check 用,玩家从一个不为0的 登录到另外一个不为0 的时，更新EndTime
/*
int player_yanghaibang_open_ui_login(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	CSYANGHAIBANGUIINFO *pstYangHaiBangUIInfo = &pstMiscSvr->PlayerMiscSvrData.YangHaiBangUIInfo;
	YANGHAIBANGINFO *pstYangHaiBangInfo = &pstPlayer->stRoleData.MiscInfo.YangHaiBangInfo;

	pstMiscSvr->PlayerMiscType = PLAYER_YANGHAIBANG_UI_INFO;
	pstYangHaiBangUIInfo->EndTime = pstYangHaiBangInfo->EndTime;
	pstYangHaiBangUIInfo->Level = 0;
	pstYangHaiBangUIInfo->Val = pstYangHaiBangInfo->Val;
	pstYangHaiBangUIInfo->tLifeEnd = pstYangHaiBangInfo->tLifeEnd;

	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

*/
int player_yanghaibang_award(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iDayNum;
	Npc *pstNpc;
	int iItemID;
	int AwardItemNum;//奖励道具的个数
	ROLEGRID stRoleGrid, stDummy;
	int iRes;
	int MaxVal; 
	YANGHAIBANGDEF *pstYangHaiBandDef; 
	ITEMDEF *pstItemDef;
	YANGHAIBANGINFO *pstYangHaiBangInfo = &pstPlayer->stRoleData.MiscInfo.YangHaiBangInfo;

	pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstYangHaiBangInfo->NpcID);
	if(!pstNpc)
	{
		return -1;
	}

	if(pstNpc->stMon.iOwnerID != pstPlayer->iMemID)
	{
		return -1;
	}

	//取珠时间未够
	if(pstYangHaiBangInfo->EndTime > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return -1;
	}

	if(pstYangHaiBangInfo->Level >MAX_HAIBANG_LEVEL)
	{
		return -1;
	}

	iDayNum = pstYangHaiBangInfo->Num;
	if(iDayNum > 3)
	{
		iDayNum = 3;
	}

	pstYangHaiBandDef = z_find_yanghaibang_def(pstEnv, pstPlayer->stRoleData.Level/10*10, iDayNum);
	if(!pstYangHaiBandDef)
	{
		return -1;
	}

	/*应满足以下两大条件才能取珠*/
	MaxVal = pstYangHaiBandDef->HaiBang[pstYangHaiBangInfo->Level-1].MaxVal;
	if(MaxVal > pstYangHaiBangInfo->Val)
	{
		//喂养程度未够   
		return 0;
	}

    iItemID = pstYangHaiBandDef->HaiBang[pstYangHaiBangInfo->Level-1].AwardItemID;
	AwardItemNum = pstYangHaiBandDef->HaiBang[pstYangHaiBangInfo->Level-1].AwardItemNum;

	/***********以下是添加绑定物品**************/
	if (0 == item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, iItemID))
	{
		stRoleGrid.GridData.RoleItem.Num = AwardItemNum;
		stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
		
		iRes=package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid,  1,0);
		if (iRes == -1)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_NOSPACE);
			return -1;
		}

		//op_log
		memcpy(&stDummy, &stRoleGrid, sizeof(stDummy));
		
		if ( 0 > package_add(pstEnv->pstAppCtx , pstEnv , pstPlayer, &stRoleGrid, 1,0, IMPITEM_OPTYPE_PICK))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_NOSPACE);
			return 0;
		}

		if(AwardItemNum - stRoleGrid.GridData.RoleItem.Num > 0)
		{
			stDummy.GridData.RoleItem.Num = AwardItemNum-stRoleGrid.GridData.RoleItem.Num;
			z_role_pickitem_oplog(pstEnv,pstPlayer, &stDummy, 1, OP_PICK);
		}

		//珍珠蚌生存时间为120分钟；每一次领取珍珠后，成熟期重新算
		pstNpc->stMon.tLifeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + 7200;

		//添加海蚌生存时间字段
		pstYangHaiBangInfo->tLifeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + 7200;

		pstYangHaiBangInfo->Val = pstYangHaiBangInfo->Val - MaxVal;

		pstYangHaiBangInfo->Level++;

		pstYangHaiBangInfo->EndTime = pstEnv->pstAppCtx->stCurr.tv_sec+600;

		pstItemDef = z_find_itemdef(pstEnv, iItemID);
		if (NULL == pstItemDef)
		{
			return -1;
		}

		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_MISC_301,AwardItemNum,pstItemDef->Name);

		if(pstYangHaiBangInfo->Level > MAX_HAIBANG_LEVEL)
		{
			/****以下是注：每个蚌壳可以领3次物品（珍珠），领完3次领完，NPC就消失了***/
			
            z_npc_die(pstEnv->pstAppCtx, pstEnv, pstNpc);
		}
		else
		{
			player_yanghaibang_open_ui(pstEnv,pstPlayer);
		}
		return 0;

	}

	player_yanghaibang_open_ui(pstEnv,pstPlayer);
	return -1;
}

int player_yanghaibang_login_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	Npc *pstNpc;
	YANGHAIBANGINFO *pstYangHaiBangInfo = &pstPlayer->stRoleData.MiscInfo.YangHaiBangInfo;
	if(pstYangHaiBangInfo->NpcID ==  0)
	{
		return -1;
	}
	
	pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstYangHaiBangInfo->NpcID);
	if(pstNpc && pstNpc->stMon.iDefIdx == 304128)
	{
		pstNpc->stMon.iOwnerID = pstPlayer->iMemID;
		//player_yanghaibang_open_ui_login(pstEnv,pstPlayer);		
	}
	else
	{
		pstYangHaiBangInfo->NpcID = 0;
		pstYangHaiBangInfo->Level = 0;
		pstYangHaiBangInfo->Val = 0;
		pstYangHaiBangInfo->YiWeiVal = 0;
		pstYangHaiBangInfo->EndTime = 0;
		pstYangHaiBangInfo->tLifeEnd = 0;

		//不弹出面板 当level 为0的时候。供客户端提示作用
		//player_yanghaibang_open_ui(pstEnv,pstPlayer);		
	}

	return 0;
}


int player_yanghaibang_start_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	Npc *pstNpc;
	YANGHAIBANGINFO *pstYangHaiBangInfo = &pstPlayer->stRoleData.MiscInfo.YangHaiBangInfo;
	RESPOS stPos,stTmpPos;
	int flag = 0;

	if (pstPlayer->stOnline.cMoving)
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);

	if (pstPlayer->stRoleData.Map != BEACH_MAP_ID )
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_MISC_302);
	       return -1;
	}

	stPos.X = pstPlayer->stRoleData.Pos.X;
	stPos.Y = pstPlayer->stRoleData.Pos.Y;
	
 	stTmpPos.X = 30928;
	stTmpPos.Y = 16795;

	//只能在以下坐标点10米以内刷海蚌npc
	if(z_distance(&stPos, &stTmpPos) > 1000)
	{
		stTmpPos.X = 30200;
		stTmpPos.Y = 22300;
		if(z_distance(&stPos, &stTmpPos) > 1000)
		{
			stTmpPos.X = 28300;
			stTmpPos.Y = 24700;
			if(z_distance(&stPos, &stTmpPos) > 1000)
			{
				stTmpPos.X = 31400;
				stTmpPos.Y = 17300;
				if(z_distance(&stPos, &stTmpPos) > 1000)
				{
					stTmpPos.X = 31600;
					stTmpPos.Y = 20300;
					if(z_distance(&stPos, &stTmpPos) > 1000)
					{
						flag = 1;
					}
				}
			}
		}
	}

	if(1 == flag)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_MISC_303);
		return -1;
	}

	pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstYangHaiBangInfo->NpcID);
	if(pstNpc)
	{
		if(pstNpc->stMon.iOwnerID == pstPlayer->iMemID)
		{
			return -2;
		}
	}
	
	return 0;
}

int player_yanghaibang_start(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	//MapIndex *pstMapIndex;
	Npc *pstNpc, stNpc; 
	RESPOS stPos,stTmpPos;
	int flag = 0;
	YANGHAIBANGINFO *pstYangHaiBangInfo = &pstPlayer->stRoleData.MiscInfo.YangHaiBangInfo;

	//获取玩家当前坐标
	if (pstPlayer->stOnline.cMoving)
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	
	if (pstPlayer->stRoleData.Map != BEACH_MAP_ID )
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_MISC_302);
	    return -1;
	}

	stPos.X = pstPlayer->stRoleData.Pos.X;
	stPos.Y = pstPlayer->stRoleData.Pos.Y;
	
 	stTmpPos.X = 30928;
	stTmpPos.Y = 16795;

	//只能在以下坐标点10米以内刷海蚌npc
	if(z_distance(&stPos, &stTmpPos) > 1000)
	{
		stTmpPos.X = 30200;
		stTmpPos.Y = 22300;
		if(z_distance(&stPos, &stTmpPos) > 1000)
		{
			stTmpPos.X = 28300;
			stTmpPos.Y = 24700;
			if(z_distance(&stPos, &stTmpPos) > 1000)
			{
				stTmpPos.X = 31400;
				stTmpPos.Y = 17300;
				if(z_distance(&stPos, &stTmpPos) > 1000)
				{
					stTmpPos.X = 31600;
					stTmpPos.Y = 20300;
					if(z_distance(&stPos, &stTmpPos) > 1000)
					{
						flag = 1;
					}
				}
			}
		}
	}

	if(1 == flag)
	{
		goto final;
	}
	
	player_yanghaibang_day_update(pstEnv, pstPlayer);
	
	pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstYangHaiBangInfo->NpcID);
	if(pstNpc)
	{
		//一只海靽在海滩上时。重新养一只的话。把原来那只删除
		if(pstNpc->stMon.iOwnerID == pstPlayer->iMemID)
		{
			z_npc_die(pstEnv->pstAppCtx, pstEnv, pstNpc);
			//pstYangHaiBangInfo->Num++;
		}
	}

	pstYangHaiBangInfo->NpcID = 0;
	pstYangHaiBangInfo->Level = 0;
	pstYangHaiBangInfo->Val = 0;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if(!pstMapInst)
	{
		return -1;
	}

	memset(&stNpc, 0, sizeof(stNpc));

	stNpc.stMon.iDefIdx = 304128;
	stNpc.stMon.stMap.iID = stNpc.stMon.iInitMap = pstMapInst->iMapInstID;
	stNpc.stMon.iInitMapPos = pstMapInst->iMapPos;
	stNpc.stMon.stMap.iPos = pstMapInst->iIndex;
	stNpc.stMon.cType = OBJ_NPC;
	stNpc.stMon.cStat = MONSTER_DIE;
	stNpc.stMon.bDirection = 0;
	stNpc.stMon.tRefresh = pstEnv->pstAppCtx->stCurr.tv_sec;

	//刷npc的坐标
	memcpy(&stNpc.stMon.stInitPos,&stPos,sizeof(stNpc.stMon.stInitPos));
	memcpy(&stNpc.stMon.stCurrPos,&stPos,sizeof(stNpc.stMon.stCurrPos));
	
	
	STRNCPY(stNpc.szScriptName, "npc304128.mac", sizeof(stNpc.szScriptName));
	trim(stNpc.szScriptName);

	if (stNpc.szScriptName[0] != 0)
	{
		ScriptIndex *pstScriptInd;
		
		pstScriptInd = get_script_ind(stNpc.szScriptName);
		if (!pstScriptInd)
		{
			return -1;
		}
		memcpy(&stNpc.stScriptIdx, pstScriptInd, sizeof(stNpc.stScriptIdx));
	}

	pstNpc = z_new_npc(pstEnv->pstAppCtx, pstEnv, &stNpc);
	if( !pstNpc )
	{
		return -1;
	}

	pstNpc->stMon.iOwnerID = pstPlayer->iMemID;
	pstNpc->stMon.cOwnerIDType = 0;	
	
	pstNpc->stMon.cFirstBorn = 1;
	pstNpc->stMon.tLifeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + 7200;

	if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, (Monster *)pstNpc, NULL))
	{
		return -1;
	}
	pstNpc->stMon.cFirstBorn = 0;
	z_check_npc_script(pstEnv, pstNpc);

	pstYangHaiBangInfo->NpcID = pstNpc->stMon.iID;
	pstYangHaiBangInfo->EndTime = pstEnv->pstAppCtx->stCurr.tv_sec+600;
	pstYangHaiBangInfo->Level	= 1;
	pstYangHaiBangInfo->Val = 0;
	pstYangHaiBangInfo->YiWeiVal = 0;

	//添加海蚌生存时间字段
	pstYangHaiBangInfo->tLifeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + 7200;

	pstYangHaiBangInfo->Num++;

	z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE, ZONEERR_MISC_304);
	return 0;
final:
	z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_MISC_303);
	return -1;
}

//lzk   喂养  
int player_yanghaibang_add(ZONESVRENV* pstEnv, Player *pstPlayer,CSYANGHAIBANGADD *pReq)
{
	int iNum = pReq->Num;
	YANGHAIBANGINFO *pstYangHaiBangInfo = &pstPlayer->stRoleData.MiscInfo.YangHaiBangInfo;
	Npc *pstNpc;
	int iRet= 0;
	
	pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstYangHaiBangInfo->NpcID);
	if(!pstNpc)
	{
		return -1;
	}

	if (!pstPlayer)
	{
		return -1;
	}

	if(iNum < 0 || iNum > 60)
	{
		return -1;
	}
	
	if(pstNpc->stMon.iOwnerID != pstPlayer->iMemID)
	{
		return -1;
	}
	
	if(pstYangHaiBangInfo->Level > MAX_HAIBANG_LEVEL)
	{
	    return -1;
	}
	else
	{
		iRet = player_yanghaibang_val(pstEnv,pstPlayer,iNum);
	}

	if(iRet < 0)
		return -1;
	
	player_yanghaibang_open_ui(pstEnv,pstPlayer);

	return 0;
}


//lzk   喂养海靽时，喂养程度的改变
int player_yanghaibang_val(ZONESVRENV* pstEnv, Player *pstPlayer,int iNum)
{
	YANGHAIBANGINFO *pstYangHaiBangInfo = &pstPlayer->stRoleData.MiscInfo.YangHaiBangInfo;
	YANGHAIBANGDEF *pstYangHaiBandDef;
	int iDayNum;
	int iItemID;
	int iRet;
	ITEMDEC stDec;
	ITEMDEF *pstItemDef;

	pstYangHaiBangInfo->YiWeiVal+=iNum;

	//已喂养
	if(pstYangHaiBangInfo->YiWeiVal > 60)
	{
		//喂养不能超过60
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_MISC_305);
		pstYangHaiBangInfo->YiWeiVal-=iNum;
		return 0;
	}

	//以下是扣除喂养的道具
	iDayNum = pstYangHaiBangInfo->Num;
	if(iDayNum > 3)
	{
		iDayNum = 3;
	}

	pstYangHaiBandDef = z_find_yanghaibang_def(pstEnv, pstPlayer->stRoleData.Level/10*10, iDayNum);
	if(!pstYangHaiBandDef)
	{
		return -1;
	}

	iItemID = pstYangHaiBandDef->HaiBang[pstYangHaiBangInfo->Level-1].NeedItemID;
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

	pstYangHaiBangInfo->Val = pstYangHaiBangInfo->Val+iNum;

	return 0;

}

//lzk     拼酒活动玩家输了，通知客户端进行屏幕模糊
int player_pinjiu_screen_vague(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	
	pstMiscSvr->PlayerMiscType = PLAYER_PINJIU_SCREEN_VAGUE;

	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

//打强盗分数显示
int player_daqiangdao_fenshu(ZONESVRENV* pstEnv, Player *pstPlayer,int iFenShu)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	CSDAQIANGDAOFENSHU *pstDaQiangDaoFenShu = &pstMiscSvr->PlayerMiscSvrData.DaQiangDaoFenShu;

	pstDaQiangDaoFenShu->iFenShu = iFenShu;
	pstMiscSvr->PlayerMiscType = PLAYER_DAQIAGNDAO_FENSHU;

	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

//职业转换前检查
int player_career_alter_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;
	CAREERALTERINFO *pstCareerAlterInfo = &pstPlayer->stRoleData.MiscInfo.CareerAlterInfo;
	int i;

	if(pstRoleWear->ArmNum > 0)
	{
		//转换职业前必须解除身上所有装备
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CARERR_ALTER4);
		return -1;
	}

	if(CS_STAT_RIDE_MON_MACHINE & pstPlayer->stOnline.State)
	{
		//请下机械再进行职业转换
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CARERR_ALTER2);
		return -1;
	}

	for (i=pstZoneSkillList->nSkillNum-1; i>=0; i--)
	{
		if (pstZoneSkillList->astZoneSkills[i].stSkillData.SkillID >= TMP_SKILL_MIN && 
			pstZoneSkillList->astZoneSkills[i].stSkillData.SkillID <= TMP_SKILL_MAX)
		{
			//有临时技能，不能职业转换
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CARERR_ALTER3);
			return -1;
		}
	}
	
	if(pstCareerAlterInfo->time != 0 && 
		pstCareerAlterInfo->time + ALTER_CAREER_LIMIT_TIME > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		//限制一周后才能职业转换
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CARERR_ALTER5);
		return -1;
	}
	
	return 0;
}

//职业转换
int player_career_alter(ZONESVRENV* pstEnv, Player *pstPlayer,int iCareer)
{
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	GENIUSINFO *pstGeniusInfo = &pstMiscInfo->GeniusInfo;
	int i,j;
	GENIUSCAREERDEF *pstDef = NULL;
	int k;
	int iSkillNum = 0;
	int iOldCareer = 0;
	int iTempOldCareer = 0;
	tdr_ulonglong ullRoleID;
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;
	DIANFENGJOININFO *pstDianFengJoinInfo = NULL;	
	DIANFENGINFO *pstDianFengInfo = NULL;
	CAREERALTERINFO *pstCareerAlterInfo = NULL;
	int m;
	UinCache *pstUinCache;
	int iValid;
	ZoneSkill *pstZoneSkill;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;	
	CSPKG stPkg;
	CSPLAYERATTRVAL *pstAttrVal = &stPkg.Body.AttrChg;
	
	if(0 > player_career_alter_check(pstEnv, pstPlayer))
	{
		return -1;
	}
	
	pstDef = genius_career_def_find(pstEnv, iCareer);
	if (pstDef)
	{
		//同等级转换职业基础天赋
		for (i=0; i<MAX_GENIUS_TYPE; i++)
		{
			if (pstDef->Types[i].Type <=  0)
				break;

			//只处理基础天赋就好，其他天赋各个职业都一样的
			if(pstDef->Types[i].Type != 1)
			{
				continue;
			}
			
			for (j=0; j<MAX_GENIUS_SUBTYPE; j++)
			{
				if (pstDef->Types[i].AttrIDs[j] <= 0)
					break;
				pstGeniusInfo->One[i].SubOne[j].AttrID = pstDef->Types[i].AttrIDs[j];
			}
		}
	}

	iSkillNum = pstZoneSkillList->nSkillNum;
	if(iSkillNum > 0)
	{
		//除结婚技能外，其余技能都清
		for(k=iSkillNum-1;k>=0;k--)
		{
			pstZoneSkill = &pstZoneSkillList->astZoneSkills[k];
			
			if(pstZoneSkill->stSkillData.SkillID == LIVE_SKILL_ID ||
				pstZoneSkill->stSkillData.SkillID == STRIVE_SKILL_ID)
			{
				continue;
			}

			pstZoneSkillList->nSkillNum--;
			if(k != pstZoneSkillList->nSkillNum)
			{
				memcpy(pstZoneSkill,&pstZoneSkillList->astZoneSkills[pstZoneSkillList->nSkillNum],sizeof(*pstZoneSkill));
			}
		}
	}

	iTempOldCareer = iOldCareer = pstPlayer->stRoleData.Career;
	pstPlayer->stRoleData.Career = iCareer;
	ullRoleID = pstPlayer->stRoleData.RoleID;
	pstMiscInfo->Flag |= CLEAR_SHORTCUT_COLUMN_SKILL_FLAG;
	pstCareerAlterInfo = &pstMiscInfo->CareerAlterInfo;
	pstCareerAlterInfo->time = pstEnv->pstAppCtx->stCurr.tv_sec;	

	pstUinCache = (UinCache *)sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos, &iValid);
	if (pstUinCache && iValid)
	{
		for (m=0; m<pstUinCache->ucRoleNum; m++)
		{
			if (pstUinCache->astRoleListInfo[m].RoleID == pstPlayer->stRoleData.RoleID)
			{
				pstUinCache->astRoleListInfo[m].Career = pstPlayer->stRoleData.Career;
				break;
			}
		}
	}

	//学习等级技能
	player_add_all_skill_by_now_level(pstEnv, pstPlayer);

	pstAttrVal->AttrNum = 0;
	//这里是为了排行榜信息与当前对应
	z_calc_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstAttrVal);
	//反正踢下线，不用发ATTR_CHG
	
	//排行榜去除原来的，再添加新的
	iTempOldCareer &= 0x0f;
	if (iTempOldCareer > CAREER_NON)
	{
		range_delete(pstEnv, RANGE_MAIN_CAREER, iTempOldCareer-1, ullRoleID);
	}
	range_career_set(pstEnv, pstPlayer, pstPlayer->stRoleData.Level);
	range_single_replace(pstEnv, pstPlayer);
	range_attr_replace(pstEnv, pstPlayer);

	if(0 < range_find(pstEnv, RANGE_MAIN_SPARTA, RANGE_MAIN_SPARTA_SUB_FIGHT, ullRoleID))
	{
		range_sparta_set(pstEnv, pstPlayer, RANGE_MAIN_SPARTA_SUB_FIGHT);
	}

	//末世巅峰清除冠军信息排名
	dianfeng_clear_champion_by_career(pstEnv,pstPlayer,iOldCareer);

	//末世巅峰处理
	if(0 == dianfeng_get_joininfo_by_career(pstEnv,&pstDianFengJoinInfo,pstDianFengGlobal,iOldCareer))
	{
		int i;
		int iMemNum = 0;
		GROUPINFO *pstGroupInfo = NULL;

		if(NULL == pstDianFengJoinInfo)
		{
			goto final;
		}

		if(pstDianFengJoinInfo->MemNum <= 0)
		{
			goto final;
		}
		
		iMemNum = min(pstDianFengJoinInfo->MemNum,MAX_DIANFENG_EVERY_GROUP_MEM_NUM);
		
		for(i=iMemNum-1;i>=0;i--)
		{
			pstGroupInfo = &pstDianFengJoinInfo->GroupInfo[i];
			if(NULL == pstGroupInfo)
			{
				continue;
			}

			if(pstGroupInfo->RoleID == pstPlayer->stRoleData.RoleID)
			{
				pstDianFengJoinInfo->MemNum--;
				if(i!=pstDianFengJoinInfo->MemNum)
				{
					memcpy(&pstDianFengJoinInfo->GroupInfo[i],&pstDianFengJoinInfo->GroupInfo[pstDianFengJoinInfo->MemNum],sizeof(pstDianFengJoinInfo->GroupInfo[i]));
				}
				
				pstDianFengInfo = &pstPlayer->stRoleData.MiscInfo.DianFengInfo;
				pstDianFengInfo->DianFengFlag &=~IF_DIANFENG_BAOMING;
			
				break;
			}
		}
	}

final:	
	player_kickoff (pstEnv->pstAppCtx, pstEnv, pstPlayer, ZONEERR_CARERR_ALTER1);
	return 0;
}

int player_clear_shortcut_skill_clt(ZONESVRENV* pstEnv, Player *pstPlayer,CSCLEARSHORTCUTSKILL *pstReq)
{
	if(pstReq->res == 1)
	{
		ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
		if(pstMiscInfo->Flag & CLEAR_SHORTCUT_COLUMN_SKILL_FLAG)
		{
			pstMiscInfo->Flag &=~CLEAR_SHORTCUT_COLUMN_SKILL_FLAG;		
		}
	}
	
	return 0;
}

//根据目前等级来添加所有该添加的等级技能
int player_add_all_skill_by_now_level(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	LEVELUP *pstNextLevelUp = NULL;
	int k,i;
	int iLevel = pstPlayer->stRoleData.Level;

	for(k=1;k<=iLevel;k++)
	{
		pstNextLevelUp = (LEVELUP *)pstEnv->pstZoneObj->sLevelUpDef[k-1];
		if(pstNextLevelUp == NULL)
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
			default:
				return -1;
				break;
		}
	}

	return 0;
}
