/*
**  @file $RCSfile: zone_db.c,v $
**  general description of this module
**  $Id: zone_db.c,v 1.658 2014/06/25 09:30:14 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2014/06/25 09:30:14 $
**  @version $Revision: 1.658 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_svr.h"
#include "role_db_meta.h"
#include "zone_clt.h"
#include "zone_db.h"
#include "zone_player.h"
#include "tconnapi.h"
#include "zone_map.h"
#include "zone_attr.h"
#include "zone_skill.h"
#include "zone_status.h"
#include "zone_oplog.h"
#include "zone_team.h"
#include "zone_tmp.h"
#include "zone_task.h"
#include "zone_relation.h"
#include "zone_package.h"
#include "zone_clan.h"
#include "zone_mail.h"
#include "zone_exchg.h"
#include "zone_stall.h"
#include "zone_pet.h"
#include "zone_combat.h"
#include "zone_fairy.h"
#include "zone_shop.h"
#include "zone_range.h"
#include "zone_autoteam.h"
#include "zone_machine.h"
#include "zone_misc.h"
#include "zone_battle.h"
#include "zone_gm.h"
#include "zone_spansvr.h"
#include "zone_safelock.h"
#include "zone_city.h"
#include "zone_clan_city.h"
#include "zone_clan_construction.h"
#include "zone_vipbar.h"
#include "zone_rune.h"
#include "zone_unreal.h"
#include "zone_subject.h"
#include "zone_strongpoint.h"
#include "zone_autopworld.h"
#include "zone_span.h"
#include "zone_arm.h"
#include "zone_ronglu.h"
#include "zone_genius.h"
#include "zone_weather.h"
#include "zone_busy.h"
#include "zone_err.h"
#include "zone_clanboon.h"
#include "zone_sparta.h"
#include "zone_second_pwd.h"
#include "zone_syncrelation.h"
#include "zone_losttotem.h"
#include "zone_syncrelation.h"
#include "zone_store.h"
#include "zone_attr.h"
#include "zone_ploy.h"
#include "zone_dianfeng.h"
#include "zone_clan_pet.h"
#include "zone_designation.h"
#include "zone_shangjin.h"
#include "zone_span_strong.h"
#include "zone_home.h"
#include "zone_zh_name.h"
#include "zone_shikong.h"

extern int g_iDesigVar;

//从跨服野外内的副本出来会找不到Map，默认的去向
#define SPAN_MAP_ID 4500
#define SPAN_MAP_EXIT_MAP 1000
#define SPAN_MAP_EXIT_POS_X 28437
#define SPAN_MAP_EXIT_POS_Y 7687


int z_account_db_update_lastlogout(ZONESVRENV* pstEnv, Player *pstPlayer);
int z_db_role_del_in( ZONESVRENV* pstEnv,  tdr_ulonglong ullRoleID, int iUin);
int z_cs_del_role_update( ZONESVRENV * pstEnv, const ROLEDELINFO* pstRoleDelInfo ,DBAsyn *pstDbsyn);
int z_account_db_del(ZONESVRENV* pstEnv, int iUin,tdr_ulonglong ullRoleID);
int z_account_db_update_online(ZONESVRENV* pstEnv, Player *pstPlayer,int iOnlineFlag);
int z_account_db_update_worldid(ZONESVRENV* pstEnv, Player *pstPlayer);
int z_db_role_drop_select_res(ZONESVRENV* pstEnv, Player *pstPlayer, TORMSELECTRES *pstSelectRes, DropObjUpdate *pstDropUpdate);
static int z_select_account_safepwd(ZONESVRENV* pstEnv, Player *pstPlayer);
int z_acc_safepwd_res(ZONESVRENV* pstEnv, Player *pstPlayer, TORMSELECTRES *pstSelectRes,DBAsyn *pstDbsyn);
int z_db_acc_share_select_res(ZONESVRENV* pstEnv, Player *pstPlayer, TORMSELECTRES *pstSelectRes);
int z_account_db_role_logout(ZONESVRENV* pstEnv, Player *pstPlayer);
int z_account_db_update_firstlogin(ZONESVRENV* pstEnv, Player *pstPlayer);

void z_init_torm_head(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKGHEAD *pstHead, int iCmd, 
						char *pszMetaName, int  iUin, DBAsyn *pstDBAsyn)
{
	pstHead->HeadComm.InstID = pstAppCtx->iId;
	pstHead->HeadComm.Magic = TORM_MAGIC;
	pstHead->HeadCmd.Cmd = iCmd;

	if (pszMetaName)
	{
		STRNCPY(pstHead->HeadCmd.MetaNameVer.MetaName, pszMetaName, sizeof(pstHead->HeadCmd.MetaNameVer.MetaName));
	}
	pstHead->HeadCmd.MetaNameVer.MetaVer=0;
	pstHead->HeadApp.AppCmd = 0;
	pstEnv->iTormSeq++;
	if (pstEnv->iTormSeq <= 0)
	{
		pstEnv->iTormSeq = 1;
	}
	
	pstHead->HeadApp.Seq = pstEnv->iTormSeq;
	pstHead->HeadApp.ObjID = iUin;
	pstHead->AsynCallBackDataLen = 0;

	if (pstDBAsyn)
	{
		memcpy(pstHead->AsynCallBackData, &pstDBAsyn->iCmd, sizeof(pstDBAsyn->iCmd));
		pstHead->AsynCallBackDataLen += sizeof(pstDBAsyn->iCmd);
		
		switch(pstDBAsyn->iCmd)
		{
		case DB_ROLE_INSERT:
			memcpy(pstHead->AsynCallBackData+pstHead->AsynCallBackDataLen, &pstDBAsyn->AsynCmd.stRoleListInfo, sizeof(pstDBAsyn->AsynCmd.stRoleListInfo));
			pstHead->AsynCallBackDataLen += sizeof(pstDBAsyn->AsynCmd.stRoleListInfo);
			break;
		case DB_CHECK_NAME:
			memcpy(pstHead->AsynCallBackData+pstHead->AsynCallBackDataLen, &pstDBAsyn->AsynCmd.stNewPlayer, sizeof(pstDBAsyn->AsynCmd.stNewPlayer));
			pstHead->AsynCallBackDataLen += sizeof(pstDBAsyn->AsynCmd.stNewPlayer);
			break;
		case DB_ROLE_DELETE:
		case DB_ROLE_DELETE_RESPITE:
		case DB_SELECT_ACC_SAFEPWD:
		case DB_ROLE_DELETE_REALLY:
		case DB_ROLE_UPDATE_UNDEL:
		case DB_UNFENGYIN_SELECT_NAMEID:
		case DB_ROLE_SECOND_PWD_SELECT:
		
			memcpy(pstHead->AsynCallBackData+pstHead->AsynCallBackDataLen, &pstDBAsyn->AsynCmd.ullRoleID, sizeof(pstDBAsyn->AsynCmd.ullRoleID));
			pstHead->AsynCallBackDataLen += sizeof(pstDBAsyn->AsynCmd.ullRoleID);
			break;
		case DB_BUDDY_CHG:
			memcpy(pstHead->AsynCallBackData+pstHead->AsynCallBackDataLen, &pstDBAsyn->AsynCmd.stBuddyChg, sizeof(pstDBAsyn->AsynCmd.stBuddyChg));
			pstHead->AsynCallBackDataLen += sizeof(pstDBAsyn->AsynCmd.stBuddyChg);
			break;
		case DB_CLAN_DELETE:
		case DB_CLAN_UPDATE:
			memcpy(pstHead->AsynCallBackData+pstHead->AsynCallBackDataLen, &pstDBAsyn->AsynCmd.nClanID, sizeof(pstDBAsyn->AsynCmd.nClanID));
			pstHead->AsynCallBackDataLen += sizeof(pstDBAsyn->AsynCmd.nClanID);
			break;
		case DB_MAIL_COUNT_SELECT:
		    case DB_MAIL_UPDATE:
		    case DB_MAIL_SELECT:
		    case DB_MAIL_DELETE:
		    case DB_MAIL_ROLE_COUNT_SELECT:
			memcpy(pstHead->AsynCallBackData+pstHead->AsynCallBackDataLen, &pstDBAsyn->AsynCmd.stActiveMailboxInfo, sizeof(pstDBAsyn->AsynCmd.stActiveMailboxInfo));
			pstHead->AsynCallBackDataLen += sizeof(pstDBAsyn->AsynCmd.stActiveMailboxInfo);
			break;	

		case DB_CHG_NAME_UP:
		case DB_CHG_NAME_SELECT:
		case DB_CS_CHG_NAME_UP_ROLEDATA:
			memcpy(pstHead->AsynCallBackData+pstHead->AsynCallBackDataLen, &pstDBAsyn->AsynCmd.stChgRoleName, 
					sizeof(pstDBAsyn->AsynCmd.stChgRoleName));
			pstHead->AsynCallBackDataLen += sizeof(pstDBAsyn->AsynCmd.stChgRoleName);
			break;

		case DB_UPDATE_ROLE_LIST:
			memcpy(pstHead->AsynCallBackData+pstHead->AsynCallBackDataLen,
				     &pstDBAsyn->AsynCmd.iUin, sizeof(pstDBAsyn->AsynCmd.iUin));
			pstHead->AsynCallBackDataLen += sizeof(pstDBAsyn->AsynCmd.iUin);
			break;
		case DB_CS_DEL_ROLE_SELECT:
			memcpy(pstHead->AsynCallBackData+pstHead->AsynCallBackDataLen,
				     &pstDBAsyn->AsynCmd.stCsDelRole, sizeof(pstDBAsyn->AsynCmd.stCsDelRole));
			pstHead->AsynCallBackDataLen += sizeof(pstDBAsyn->AsynCmd.stCsDelRole);
			break;
		case DB_RANGE_UPDATE:
			memcpy(pstHead->AsynCallBackData+pstHead->AsynCallBackDataLen,
				     &pstDBAsyn->AsynCmd.stRangeUpdate, sizeof(pstDBAsyn->AsynCmd.stRangeUpdate));
			pstHead->AsynCallBackDataLen += sizeof(pstDBAsyn->AsynCmd.stRangeUpdate);
			break;
		case DB_CS_DEL_ROLE_UPDATE:
			memcpy(pstHead->AsynCallBackData,pstDBAsyn, sizeof(*pstDBAsyn));
			break;
		case DB_ROLE_DROP_SELECT:
			memcpy(pstHead->AsynCallBackData+pstHead->AsynCallBackDataLen,
				     &pstDBAsyn->AsynCmd.stDropObjUpdate, sizeof(pstDBAsyn->AsynCmd.stDropObjUpdate));
			pstHead->AsynCallBackDataLen += sizeof(pstDBAsyn->AsynCmd.stDropObjUpdate);
			break;
		case DB_ROLEDETAIL_SELECT_RES:
		case DB_ROLEDETAIL_UPDATE_RES:
			memcpy(pstHead->AsynCallBackData+pstHead->AsynCallBackDataLen,
				     &pstDBAsyn->AsynCmd.stDetailUpdate, sizeof(pstDBAsyn->AsynCmd.stDetailUpdate));
			pstHead->AsynCallBackDataLen += sizeof(pstDBAsyn->AsynCmd.stDetailUpdate);
			break;
		case DB_SELECT_WATCH_RES:
			memcpy(pstHead->AsynCallBackData+pstHead->AsynCallBackDataLen,
				     &pstDBAsyn->AsynCmd.stWatchRole, sizeof(pstDBAsyn->AsynCmd.stWatchRole));
			pstHead->AsynCallBackDataLen += sizeof(pstDBAsyn->AsynCmd.stWatchRole);
			break;
		case DB_ROLE_BASE_SELECT:
			memcpy(pstHead->AsynCallBackData+pstHead->AsynCallBackDataLen,
				     &pstDBAsyn->AsynCmd.stRoleBaseSelect, sizeof(pstDBAsyn->AsynCmd.stRoleBaseSelect));
			pstHead->AsynCallBackDataLen += sizeof(pstDBAsyn->AsynCmd.stRoleBaseSelect);
			break;
		case DB_ROLE_SYNC_RELATION_SELECT:
			memcpy(pstHead->AsynCallBackData+pstHead->AsynCallBackDataLen,
				     &pstDBAsyn->AsynCmd.stSyncRelationSelect, sizeof(pstDBAsyn->AsynCmd.stSyncRelationSelect));
			pstHead->AsynCallBackDataLen += sizeof(pstDBAsyn->AsynCmd.stSyncRelationSelect);
			break;
		case DB_ROLE_MISC_INFO_SELECT:
			memcpy(pstHead->AsynCallBackData+pstHead->AsynCallBackDataLen,
					 &pstDBAsyn->AsynCmd.stDBMiscInfo, sizeof(pstDBAsyn->AsynCmd.stDBMiscInfo));
			pstHead->AsynCallBackDataLen += sizeof(pstDBAsyn->AsynCmd.stDBMiscInfo);
			break;
		case DB_ROLE_BUDDY_SELECT:
			memcpy(pstHead->AsynCallBackData+pstHead->AsynCallBackDataLen,
					 &pstDBAsyn->AsynCmd.stDBRoleBuddy, sizeof(pstDBAsyn->AsynCmd.stDBRoleBuddy));
			pstHead->AsynCallBackDataLen += sizeof(pstDBAsyn->AsynCmd.stDBRoleBuddy);
			break;
		case DB_ROLE_SKILL_INFO_SELECT:
			memcpy(pstHead->AsynCallBackData+pstHead->AsynCallBackDataLen,
					 &pstDBAsyn->AsynCmd.stDBRoleSkillInfo, sizeof(pstDBAsyn->AsynCmd.stDBRoleSkillInfo));
			pstHead->AsynCallBackDataLen += sizeof(pstDBAsyn->AsynCmd.stDBRoleSkillInfo);
			break;
		default:
			break;
		}
	}

	return;
}

int z_torm_send(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg, int iDstID)
{
	TDRDATA stData;
	int iSrc;
	int iDst;
	int iProtoVer;
	int iRet;

	if (iDstID == pstEnv->iRoleDBID)
	{
		iProtoVer = pstEnv->iRoleDBProtoVer;
	}
	else
	{
		iProtoVer = pstEnv->iMiscDBProtoVer;
	}

	stData.pszBuff = pstEnv->szSendBuffer;
	stData.iBuff = sizeof(pstEnv->szSendBuffer);
	if(0 != torm_pack_msg(pstEnv->hTorm, &stData, pstPkg, iProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_pack_msg error");
		return -1;
	}
	
	iSrc = pstAppCtx->iId;
	iDst = iDstID;

	iRet = tbus_send(pstAppCtx->iBus, &iSrc, &iDst, stData.pszBuff, stData.iBuff, 0);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail %s dst %s cmd=%d meta=%s",
			 tbus_error_string(iRet), tbus_addr_ntoa(iDstID), pstPkg->Head.HeadCmd.Cmd, pstPkg->Head.HeadCmd.MetaNameVer.MetaName);
		return -1;
	}
	else
	{
		return 0;
	}
}

int z_torm_exchg_ver(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	
	z_init_torm_head(pstAppCtx, pstEnv, &pstPkg->Head, TORM_EXGVER_REQ, NULL, -1, NULL);
	pstPkg->Body.ExgVerReq.Version = TDR_METALIB_TORM_NET_VERSION;

	z_torm_send(pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID);
	z_torm_send(pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID);
	z_torm_send(pstAppCtx, pstEnv, pstPkg, pstEnv->iRegionDBID);
	
	return 0;
}

int z_role_cache_update(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	UinCache *pstUinCache;
	UinCache stUinHashData;

	stUinHashData.iUin = pstPlayer->stRoleData.Uin;
	pstUinCache = (UinCache *)sht_find(pstEnv->pstShtUinCache,&stUinHashData,UinHashCmp,UinHashCode);
	if (!pstUinCache)
	{
		return -1;
	}

	for (i = 0; i < pstUinCache->ucRoleNum; i++)
	{
		if (pstUinCache->astRoleListInfo[i].RoleID != pstPlayer->stRoleData.RoleID)
		{
			continue;
		}
		
		pstUinCache->astRoleListInfo[i].Map = GET_MAPID(pstPlayer->stRoleData.Map);
		pstUinCache->astRoleListInfo[i].LastLogin = pstPlayer->stRoleData.LastLogin;
	}

	return 0;
}


int z_check_uin_cache(ZONESVRENV* pstEnv,UinCache *pstUinCache,Player *pstPlayer)
{
	int iChg = 0;
	int i;
	time_t t = pstEnv->pstAppCtx->stCurr.tv_sec-DEL_ROLE_PROTECT_TM*60;
	//ROLEDELINFO stInfo;
	CSROLEDELREQ stRoleDelReq;

	for (i=pstUinCache->ucRoleNum-1; i>=0; i--)
	{
		if (	pstUinCache->astRoleListInfo[i].DelTime > 0 &&
			(time_t)pstUinCache->astRoleListInfo[i].DelTime < t &&
			pstUinCache->astRoleListInfo[i].CtrlFlag == 0 )
		{
			// 删除角色 
			//stInfo.CtrlFlag = ROLE_DEL_FLAG;
			//stInfo.DelTime = pstUinCache->astRoleListInfo[i].DelTime;
			//stInfo.RoleID = pstUinCache->astRoleListInfo[i].RoleID;
			//z_db_del_time_update( pstEnv, pstUinCache->astRoleListInfo[i].Uin, &stInfo,DB_ROLE_DELETE_REALLY);
			stRoleDelReq.RoleID = pstUinCache->astRoleListInfo[i].RoleID;
			z_db_get_roledetail_req(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleDelReq);
			iChg = 1;
		}
	}
	return iChg;
}

int set_show_flag(unsigned char *p, int iFlag)
{
	unsigned char c = 0;
	
	if (SHOW_FASHION_FLAG & iFlag)
	{
		c |= SHOW_FASHION_FLAG;
	}
	if (SHOW_HEAD_FLAG & iFlag)
	{
		c |= SHOW_HEAD_FLAG;
	}
	if (SHOW_BACK_FLAG & iFlag)
	{
		c |= 4;
	}
	if (SHOW_FACE_FLAG & iFlag)
	{
		c |= 8;
	}
	if (SHOW_WEDDING_DSN_FLAG & iFlag)
	{
		c |= 16;
	}
	if (CLEAR_SHORTCUT_COLUMN_SKILL_FLAG & iFlag)
	{
		c |= 32;
	}
	
	*p = c;
	return 0;
}

int z_send_rolelist(ZONESVRENV* pstEnv, Player *pstPlayer, UinCache *pstUinCache)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSROLELIST *pstRoleList;
	int i;
	time_t t = pstEnv->pstAppCtx->stCurr.tv_sec-DEL_ROLE_PROTECT_TM*60;
	int iMaxLevel = 0;
	
	Z_CSHEAD_INIT(&pstPkg->Head, ROLE_LIST);
	
	pstRoleList = &pstPkg->Body.RoleList;
	pstRoleList->RoleNum = 0;
	pstRoleList->SvrTime.BenchTimeSec = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstRoleList->SvrTime.BenchTimeUSec = pstEnv->pstAppCtx->stCurr.tv_usec;

	if (pstUinCache->ucRoleNum > ROLE_MAX_NUM) return -1;

	z_check_uin_cache(pstEnv,pstUinCache,pstPlayer);

	for (i=0; i<pstUinCache->ucRoleNum; i++)
	{
		if (	pstUinCache->astRoleListInfo[i].DelTime > 0 &&
			(time_t)pstUinCache->astRoleListInfo[i].DelTime < t &&
			pstUinCache->astRoleListInfo[i].CtrlFlag == 0 )
		{
			continue;
		}
		
		iMaxLevel = max(pstUinCache->astRoleListInfo[i].Level,iMaxLevel);
		
		pstRoleList->RoleList[i].Career = pstUinCache->astRoleListInfo[i].Career;
		pstRoleList->RoleList[i].Gender= pstUinCache->astRoleListInfo[i].Gender;
		pstRoleList->RoleList[i].RoleID= pstUinCache->astRoleListInfo[i].RoleID;
		STRNCPY(pstRoleList->RoleList[i].RoleName, pstUinCache->astRoleListInfo[i].RoleName,
				sizeof(pstRoleList->RoleList[i].RoleName));
		pstRoleList->RoleList[i].Head = pstUinCache->astRoleListInfo[i].Head;
		pstRoleList->RoleList[i].Face = pstUinCache->astRoleListInfo[i].Face;
		pstRoleList->RoleList[i].Level = pstUinCache->astRoleListInfo[i].Level;
		pstRoleList->RoleList[i].Map = pstUinCache->astRoleListInfo[i].Map;
		pstRoleList->RoleList[i].Hair = pstUinCache->astRoleListInfo[i].Hair;
		pstRoleList->RoleList[i].LastLogin = pstUinCache->astRoleListInfo[i].LastLogin;
		pstRoleList->RoleList[i].DelTime = pstUinCache->astRoleListInfo[i].DelTime;
		memcpy(&pstRoleList->RoleList[i].Avatar, &pstUinCache->astRoleListInfo[i].AvatarProf, 
				sizeof(pstRoleList->RoleList[i].Avatar));
		
		pstRoleList->RoleNum++;
	}

	pstPlayer->stAccShareData.iAccMaxRoleLevel = iMaxLevel;

	//预创建并且还没有领取过礼包
	if (pstEnv->pstConf->PreCreateRole &&
		pstRoleList->RoleNum > 0 &&
		!(pstPlayer->UinFlag & UIN_FLAG_PRECREATE_GIFT))
	{
		//todo:sms
		player_action_precreate_gift_verify(pstEnv,pstPlayer,
								ACTION_PRECREATE_GIFT_VERIFY_OP_OPEN,0);
	}
	
	return z_cltmsg_send(pstEnv, pstPlayer, pstPkg);
}

int z_db_role_list_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TORMSELECTRES *pstSelectRes)
{
	int i;
	ROLELISTINFO *pstRoleListInfo;
	UinCache *pstUinCache;
	int iValid;

	pstUinCache = (UinCache *)sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos, &iValid);
	if (NULL == pstUinCache || 0 == iValid || pstUinCache->iUin != pstPlayer->stRoleData.Uin)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
					"uin %d get cache pos=%d error", pstPlayer->stRoleData.Uin, pstPlayer->iUinCachePos);
		return -1;
	}

	pstUinCache->cGetRoleList = 1;
	pstUinCache->ucRoleNum = 0;
	for(i=0; i<pstSelectRes->SelectResult.ResultNum; i++)
	{
		if (pstUinCache->ucRoleNum >= ROLE_MAX_NUM)
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
					"z_db_role_list_res uin=%d role list num %d", pstPlayer->stRoleData.Uin,
					pstSelectRes->SelectResult.ResultNum);
			break;
		}
		
		pstRoleListInfo = &pstUinCache->astRoleListInfo[pstUinCache->ucRoleNum];
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)pstRoleListInfo, sizeof(*pstRoleListInfo)))
		{
			continue;
		}

		if ( 0 == pstRoleListInfo->LastLogin && 0 == pstRoleListInfo->Map)
		{
			z_birth_avatar(pstAppCtx, pstEnv, &pstRoleListInfo->AvatarProf, 
							pstRoleListInfo->Career, pstRoleListInfo->Gender, pstRoleListInfo->ApperalSelect);
		}
			
		pstUinCache->ucRoleNum++;
	}

	pstPlayer->eStatus = PLAYER_STATUS_ROLE_LIST;
	tlog_info(pstEnv->pstLogCat, 0, 0, "Status=RoleList Uin=%d AccName=%s", pstPlayer->stRoleData.Uin, pstPlayer->szAccount);

	return z_send_rolelist(pstEnv, pstPlayer, pstUinCache);
}

int z_db_update_role_list_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, 
	                            TORMSELECTRES *pstSelectRes, int iUin)
{
	
	int i;
	ROLELISTINFO *pstRoleListInfo;
	UinCache stDummy, *pstUinCache;
	UNUSED(pstAppCtx);

	stDummy.iUin = iUin;
	pstUinCache = sht_find(pstEnv->pstShtUinCache, &stDummy, UinHashCmp,
		                     UinHashCode);
	if(!pstUinCache)
	{
		return -1;
	}

	pstUinCache->cGetRoleList = 1;
	pstUinCache->ucRoleNum = 0;
	for(i = 0; i < pstSelectRes->SelectResult.ResultNum; i++)
	{
		if (pstUinCache->ucRoleNum >= ROLE_MAX_NUM)
		{
			tlog_error(pstEnv->pstLogCat, iUin, 0, 
					       "z_db_role_list_res uin=%d role list num %d",
					       iUin,
					       pstSelectRes->SelectResult.ResultNum);
			break;
		}
		
		pstRoleListInfo = &pstUinCache->astRoleListInfo[pstUinCache->ucRoleNum];
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, 
			                         (char *)pstRoleListInfo,
			                         sizeof(*pstRoleListInfo)))
		{
			continue;
		}
		
		pstUinCache->ucRoleNum++;
	}

	return 0;
}

int z_check_player_pkg(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLEPACKAGE *pstPkg = &pstPlayer->stRoleData.Package;
	ROLEWEAR *pstWear = &pstPlayer->stRoleData.Wear;
	int i;
	ROLEITEM *pstRoleItem;
	ITEMDEF *pstItemDef = NULL;
	int t2;
	ROLEGRID *pstRoleGrid;
	ROLEARM *pstRoleArm;
	
	time_t t1 = string2time("20120308000000");

	// 这句不注释了 策划说这样可以
	if (pstEnv->pstAppCtx->stCurr.tv_sec < t1)
	{
		return 0;
	}

	for (i=0; i<pstPkg->SundriesPackage.Num; i++)
	{
		pstRoleGrid = &pstPkg->SundriesPackage.RoleGrids[i];
		pstRoleItem = &pstRoleGrid->GridData.RoleItem;
		if (pstRoleItem->DefID == 5052801)
		{
			pstItemDef = z_get_itemdef_roleitem(pstEnv, pstRoleItem);
			if (pstItemDef && pstItemDef->ValidTime > 0)
			{
				if (pstRoleItem->ValidEnd  == 0)
				{
					pstRoleItem->ValidEnd = pstEnv->pstAppCtx->stCurr.tv_sec + pstItemDef->ValidTime*3600;	
				}
				else
				{
					t2 = pstRoleItem->ValidEnd - pstItemDef->ValidTime*3600;
					if (t2 > 0 && t2 < t1)
					{
						pstRoleItem->ValidEnd = pstEnv->pstAppCtx->stCurr.tv_sec + pstItemDef->ValidTime*3600;	
					}
				}
				
			}
		}

		if (pstRoleGrid->Type == GRID_ITEM_ARM)
		{
			pstRoleArm = &pstRoleGrid->GridData.RoleArm;
			if (pstRoleArm->StrengthenCount < 0)
			{
				pstRoleArm->StrengthenCount = 0;
			}
		}
	}

	for (i=0; i<pstPkg->StashPackage.Num; i++)
	{
		pstRoleGrid = &pstPkg->StashPackage.RoleGrids[i];
		pstRoleItem = &pstRoleGrid->GridData.RoleItem;
		if (pstRoleItem->DefID == 5052801)
		{
			pstItemDef = z_get_itemdef_roleitem(pstEnv, pstRoleItem);
			if (pstItemDef && pstItemDef->ValidTime > 0)
			{
				if (pstRoleItem->ValidEnd  == 0)
				{
					pstRoleItem->ValidEnd = pstEnv->pstAppCtx->stCurr.tv_sec + pstItemDef->ValidTime*3600;	
				}
				else
				{
					t2 = pstRoleItem->ValidEnd - pstItemDef->ValidTime*3600;
					if (t2 > 0 && t2 < t1)
					{
						pstRoleItem->ValidEnd = pstEnv->pstAppCtx->stCurr.tv_sec + pstItemDef->ValidTime*3600;	
					}
				}
				
			}
		}

		if (pstRoleGrid->Type == GRID_ITEM_ARM)
		{
			pstRoleArm = &pstRoleGrid->GridData.RoleArm;
			if (pstRoleArm->StrengthenCount < 0)
			{
				pstRoleArm->StrengthenCount = 0;
			}
		}
	}

	for (i=0; i<pstWear->ArmNum; i++)
	{
		pstRoleArm = &pstWear->RoleArms[i];
		if (pstRoleArm->StrengthenCount < 0)
		{
			pstRoleArm->StrengthenCount = 0;
		}
	}
	
	return 0;
}

int z_check_player_data( ZONESVRENV* pstEnv, Player *pstPlayer)
{
	SKILLINFO *pstSkillInfo = &pstPlayer->stRoleData.SkillInfo;	
	int i = 0;
	SKILLDEF* pstOriginDef = NULL;
	SKILLDEF* pstSkillDef = NULL;
	SKILLDATA* pstSkillData = NULL;
	ITEMUSECOOLLIST *pstItemUseCoolList = NULL;
	ITEMDEF* pstItemDef = NULL;
	ITEMSELFCOOL* pstCool = NULL;
	struct timeval stTmp,stDiff,stSpecTimeEnd;
	long long  llMs = 0;
	ITEMSPECOOL* pstSpecCool = NULL;
	long long llMaxErr = 0;

	memset(&stTmp,0,sizeof(stTmp));
	memset(&stSpecTimeEnd,0,sizeof(stSpecTimeEnd));

	//检查师生关系数据
	school_check_data(pstEnv, pstPlayer);
	
	//检查技能冷却时间
	for (i = 0; i < pstSkillInfo->SkillNum; i++)
	{
		pstSkillData = &pstSkillInfo->SkillList[i];
		pstSkillDef = NULL;
	
		pstOriginDef = z_find_skill(pstEnv, pstSkillData->SkillID, pstSkillData->SkillLevel,NULL);
		if (!pstOriginDef)
		{
			continue;
		}		
		if (pstSkillData->RuneSlotEnabled &&
			pstSkillData->RuneSlot > 0 &&
			pstSkillData->RuneSlot < RUNE_TYPE_NUM)
		{
			switch (pstSkillData->RuneSlot)
			{
			case RUNE_RED:
				pstSkillDef = z_find_skill(pstEnv,
				                           pstOriginDef->RedRuneTransferedID,
				                           pstOriginDef->Level, NULL);
				break;

			case RUNE_BLUE:
				pstSkillDef = z_find_skill(pstEnv,
				                           pstOriginDef->BlueRuneTransferedID,
				                           pstOriginDef->Level, NULL);
				break;			

			case RUNE_YELLOW:
				pstSkillDef = z_find_skill(pstEnv,
				                           pstOriginDef->YellowRuneTransferedID,
				                           pstOriginDef->Level, NULL);
				break;

			default:
				pstSkillDef = NULL;
				break;
			}
		}
		
		if (!pstSkillDef)
		{
			pstSkillDef = pstOriginDef;
		}

		//冷却时间比较,大于某值,设置回默认冷却时间
		if (pstSkillData->CoolLeft > pstSkillDef->SelfCool * MAX_COOL_TIME_ERR_MUL) 
		{
			pstSkillData->CoolLeft = pstSkillDef->SelfCool;
		}

	}
	
	//检查物品冷却时间,自冷却	
	pstItemUseCoolList = &pstPlayer->stRoleData.MiscInfo.ItemUseCoolList;
	for (i=0; i<pstItemUseCoolList->ItemSelfCoolNum;i++)
	{
		pstCool = &pstItemUseCoolList->ItemSelfCools[i];
		if (!pstCool->ItemID)
		{
			continue;
		}

		pstItemDef = z_find_itemdef(pstEnv,pstCool->ItemID);
		if (!pstItemDef)
		{
			continue;
		}
		stTmp.tv_sec = pstCool->CoolEnd.BenchTimeSec;
		stTmp.tv_usec = pstCool->CoolEnd.BenchTimeUSec;

		TV_DIFF(stDiff, stTmp, pstEnv->pstAppCtx->stCurr);
		TV_TO_MS(llMs, stDiff);

		//冷却时间大于最大允许误差,重置
		if (llMs > MAX_COOL_TIME_ERR_MUL * pstItemDef->SelfCool * 100)
		{
			llMaxErr = max(llMs,llMaxErr);
			llMs = pstItemDef->SelfCool * 100;
			memset(&stTmp,0,sizeof(stTmp));
			
			TV_ADD_MS(stTmp, pstEnv->pstAppCtx->stCurr, llMs);
			
			pstCool->CoolEnd.BenchTimeSec = stTmp.tv_sec;
			pstCool->CoolEnd.BenchTimeUSec = stTmp.tv_usec;			
		}

	}
	
	//物品指定冷却
	if (llMaxErr > 0)
	{
		for (i=0; i<pstItemUseCoolList->ItemSpeCoolNum;i++)
		{
			pstSpecCool = &pstItemUseCoolList->ItemSpeCools[i];
			
			stSpecTimeEnd.tv_sec = pstSpecCool->CoolEnd.BenchTimeSec;
			stSpecTimeEnd.tv_usec = pstSpecCool->CoolEnd.BenchTimeUSec;

			TV_ADD_MS(stTmp, stSpecTimeEnd, -llMaxErr);
			
			pstSpecCool->CoolEnd.BenchTimeSec = stTmp.tv_sec;
			pstSpecCool->CoolEnd.BenchTimeUSec = stTmp.tv_usec;

		}
	}

	z_check_player_pkg(pstEnv, pstPlayer);
	genius_player_check(pstEnv, pstPlayer);
	return 0;
}

//0正常登陆,1表示跨服登陆
int z_init_role(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int iOperType)
{
	int iMapInstID;
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;
	ROLETMPSYS *pstTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;
//	int iCurr = pstAppCtx->stCurr.tv_sec;

	/*if (pstPlayer->stRoleData.MiscInfo.MapTimeLimit.EndTime > iCurr)
	{
		pstPlayer->stOnline.stMapOL.iEndTime = pstPlayer->stRoleData.MiscInfo.MapTimeLimit.EndTime;
	}*/

	z_pre_create_award_money( pstEnv, pstPlayer);
	
	//检查角色数据,技能冷却时间
	z_check_player_data(pstEnv,pstPlayer);

	if( iOperType && INST_MODE_ZONE_SVR == pstEnv->iInstMode)
	{
		//不更新登陆时间
	}
	else
	{
		pstPlayer->stRoleData.LastLogin = pstAppCtx->stCurr.tv_sec;
		pstPlayer->stOnline.tRoleLogin = pstAppCtx->stCurr.tv_sec;
		pstPlayer->stOnline.tRoleLogout = pstPlayer->stRoleData.LastLogout;
	}
	if (pstPlayer->stRoleData.MiscInfo.CleanDiffTm == 0)
		pstPlayer->stRoleData.MiscInfo.CleanDiffTm = pstAppCtx->stCurr.tv_sec;

	//坐骑
	pstPlayer->stOnline.RideItemID = pstPlayer->stRoleData.MiscInfo.RideItemID;
	pstPlayer->stOnline.RideLevel = pstPlayer->stRoleData.MiscInfo.RideLevel;

	
	pstPlayer->stOnline.ePkMode = pstPlayer->stRoleData.MiscInfo.PkInfo.PkMode;

	//state 初始化
	pstPlayer->stOnline.State = 0;
	if (pstPlayer->stRoleData.RoleDetail.HPCurr == 0)
	{
		pstPlayer->stOnline.State |= CS_STAT_DEAD;
	}
	
	if (pstPlayer->stRoleData.MiscInfo.Flag & ROLE_LOGOUT_FLY)
	{
		pstPlayer->stOnline.State |= CS_STAT_FLY;
		pstPlayer->stOnline.stRideOnInfo.iOwnerID = pstPlayer->iMemID;
	}
	else if (pstPlayer->stRoleData.MiscInfo.Flag & ROLE_LOGOUT_RIDE)
	{
		pstPlayer->stOnline.State |= CS_STAT_RIDE_ON;
		pstPlayer->stOnline.stRideOnInfo.iOwnerID = pstPlayer->iMemID;
	}
	
	if (pstPlayer->stRoleData.MiscInfo.AutoFlyPath > 0)
	{
		pstPlayer->stRoleData.MiscInfo.AutoFlyPath = 0;
			
		pstPlayer->stRoleData.Map = pstPlayer->stRoleData.ExitMap;
		pstPlayer->stRoleData.Pos.X = pstPlayer->stRoleData.ExitPos_x;
		pstPlayer->stRoleData.Pos.Y = pstPlayer->stRoleData.ExitPos_y;
	}

	if (pstTmpSys->TmpItemID > 0)
	{
		pstPlayer->stOnline.bDir = pstTmpSys->Dir;
	}
	else if (0 == pstPlayer->stOnline.bDir)
	{
		pstPlayer->stOnline.bDir = RAND1(180);
	}

	if(pstPlayer->stRoleData.WorldID <= 0)
	{
		pstPlayer->stRoleData.WorldID = pstEnv->unWorldID;
	}
	
	pstPlayer->stOnline.iPosSeq = 1;
	pstPlayer->stOnline.stAtkInfo.stPilotEnd.tv_sec = pstAppCtx->stCurr.tv_sec;
	pstPlayer->stOnline.stAtkInfo.stPilotEnd.tv_usec = pstAppCtx->stCurr.tv_usec;

	//死亡或者map inst 不存在的情况
	iMapInstID = pstPlayer->stRoleData.Map;

	stIdx.iID = iMapInstID;
	stIdx.iPos = -1;
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);

	 // 只有副本地图才会找不到inst
	 // 所以一定是副本退出来的
	if (!pstMapInst)
	{
		if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
		{
			//没找到战场副本地图,就是个悲剧
			return -1;
		}
		//找副本的退出点
		stIdx.iID = pstPlayer->stRoleData.ExitMap;
		stIdx.iPos = -1;
		pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
		if (pstMapInst)
		{
			pstPlayer->stRoleData.Map = pstPlayer->stRoleData.ExitMap;
			pstPlayer->stRoleData.Pos.X=  pstPlayer->stRoleData.ExitPos_x;
			pstPlayer->stRoleData.Pos.Y = pstPlayer->stRoleData.ExitPos_y;
		}
		else
		{
			if(pstPlayer->stRoleData.ExitMap == SPAN_MAP_ID)
			{
				//在跨服野外中的绿野凶林内下线之类的，找不到去向
				pstPlayer->stRoleData.Map = SPAN_MAP_EXIT_MAP;
				pstPlayer->stRoleData.Pos.X=  SPAN_MAP_EXIT_POS_X;
				pstPlayer->stRoleData.Pos.Y = SPAN_MAP_EXIT_POS_Y;
			}
			else
			{
				z_get_safe_mappos(pstAppCtx, pstEnv, &pstPlayer->stRoleData.Map,&pstPlayer->stRoleData.Pos);
			}
		}

		if ((pstPlayer->stOnline.State & CS_STAT_DEAD) &&
			pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal >= PLAYER_PK_PUBLIC_ENEMY)
		{
			pstPlayer->stRoleData.Map = BLACK_NAME_REVIVE_MAP;
			pstPlayer->stRoleData.Pos.X=  BLACK_NAME_REVIVE_POS_X;
			pstPlayer->stRoleData.Pos.Y = BLACK_NAME_REVIVE_POS_Y;
			g_iDesigVar = 2;
		}
		// 记一下副本地图ID
		pstPlayer->stOnline.iOldMap = iMapInstID;
	}
	return 0;
}

void z_add_roledetail(Player *pstPlayer, CSPLAYERATTRVAL *pstAttrVal)
{
	ROLEDETAIL *pstRoleDetail = &pstPlayer->stRoleData.RoleDetail;
		
	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_SKILLPOINT;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.SkillPoint = pstRoleDetail->SkillPoint;
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_MONEY;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.Money = pstRoleDetail->Money;
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_BULL;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.Bull = pstRoleDetail->Bull;
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_BINDGODCOIN;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.BindGodCoin = pstRoleDetail->BindGodCoin;
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_VIPPOINT;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.VipPoint = pstRoleDetail->VipPoint;
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_GODCOIN;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.VipPoint = pstRoleDetail->GodCoin;
	pstAttrVal->AttrNum++;	

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_BATTLE_GLORY;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.BattleGlory = pstRoleDetail->BlueJade;
	pstAttrVal->AttrNum++;	

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_AOSHAN_BRAND;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.AoShanBrand = pstRoleDetail->BlueGem;
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_LIFT_BRAND;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.LiftBrand = pstRoleDetail->GreenGem;
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_SPARTA_GLORY;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.SpartaGlory = pstRoleDetail->SpartaGlory;
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_CHARM;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.Charm = pstRoleDetail->WhiteJade;
	pstAttrVal->AttrNum++;

	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_SCHOOL_VAL;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.SchoolVal = pstRoleDetail->SchoolVal;
	pstAttrVal->AttrNum++;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].AttrID = ATTR_ID_CLAN_WAR_BRAND;
	pstAttrVal->AttrList[pstAttrVal->AttrNum].Value.ClanWarBrand = pstRoleDetail->ClanWarBrand;
	pstAttrVal->AttrNum++;

	return ;
}

int z_check_bind_skill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	int iInValidSkillNum;
	int aiInValidSkills[MAX_ROLE_SKILL];
	SKILLDEF *pstSkillDef;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;

	iInValidSkillNum = 0;
	for (i=0; i<pstZoneSkillList->nSkillNum; i++)
	{
		pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkillList->astZoneSkills+i);
		if (pstSkillDef && 1 == pstSkillDef->BindSkill) //状态绑定
		{
			if (NULL == z_find_bindskill_status(pstEnv, pstPlayer, pstSkillDef->SkillID))
			{
				aiInValidSkills[iInValidSkillNum] =  pstSkillDef->SkillID;
				iInValidSkillNum++;
			}
		}
	}

	for (i=0; i<iInValidSkillNum; i++)
	{
		z_del_skill(pstAppCtx, pstEnv, pstPlayer, aiInValidSkills[i], 0);
	}
	
	return 0;
}

int z_add_role_detail(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							CSROLEDETAIL *pstRoleDetail)
{
	int i;
	CSSKILLLIST *pstCSSkillList = &pstRoleDetail->SkillList;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;
	DIATHESISINFO *pstDiaThesisInfo = &pstPlayer->stRoleData.DiathesisInfo;
	//FITTINGSINFO *pstFittingsInfo = &pstPlayer->stRoleData.FittingsInfo;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	
	pstRoleDetail->Gender = pstPlayer->stRoleData.Gender;
	pstRoleDetail->Career = pstPlayer->stRoleData.Career;
	pstRoleDetail->Head = pstPlayer->stRoleData.Head;
	pstRoleDetail->Face  = pstPlayer->stRoleData.Face;
	pstRoleDetail->Hair = pstPlayer->stRoleData.Hair;
	pstRoleDetail->Level = pstPlayer->stRoleData.Level;
	pstRoleDetail->AutoFlyPath = pstPlayer->stRoleData.MiscInfo.AutoFlyPath;
	pstRoleDetail->RoleID = pstPlayer->stRoleData.RoleID;

	//PK
	pstRoleDetail->PkInfo.PkMode = pstPlayer->stOnline.ePkMode;
	pstRoleDetail->PkInfo.PkVal = pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal;
	pstRoleDetail->PkInfo.PkTime = 0;
	set_show_flag(&pstRoleDetail->ShowFlag, pstMiscInfo->Flag);
	
	// 装备增加技能等级
	z_arm_skill_update(pstEnv, pstPlayer);
	
	//穿戴的装备信息
	memcpy(&pstRoleDetail->Wear, &pstPlayer->stRoleData.Wear, sizeof(pstRoleDetail->Wear));

	//骑术
	memcpy(&pstRoleDetail->RideList, &pstMiscInfo->RideList, sizeof(pstRoleDetail->RideList));

	//技能信息
	pstCSSkillList->SkillNum = pstZoneSkillList->nSkillNum;
	for (i=0; i<pstZoneSkillList->nSkillNum; i++)
	{
		memcpy(&pstCSSkillList->Skills[i], &pstZoneSkillList->astZoneSkills[i].stSkillData,
				sizeof(pstCSSkillList->Skills[i]));
	}

	//素质信息
	//pstRoleDetail->DiathesisList.DiathesisNum  = pstDiaThesisInfo->DiathesisNum;
	memcpy(&pstRoleDetail->DiathesisList, pstDiaThesisInfo, sizeof(DIATHESISINFO));
		
	//配件技能信息
	//pstRoleDetail->FittingsList.FittingsNum = pstFittingsInfo->FittingsNum;
	//memcpy(pstRoleDetail->FittingsList.FittingsList, pstFittingsInfo->FittingsList, sizeof(pstRoleDetail->FittingsList.FittingsList[0])*pstFittingsInfo->FittingsNum);

	//状态信息
	z_status_fill(pstAppCtx, pstEnv, pstPlayer, &pstRoleDetail->StatusList);

	//检查绑定技能
	z_check_bind_skill(pstAppCtx, pstEnv, pstPlayer);

	memcpy(&pstRoleDetail->TmpSys, &pstPlayer->stRoleData.MiscInfo.TmpSys, sizeof(pstRoleDetail->TmpSys));

	pstRoleDetail->PlayerAttrVal.AttrNum = 0;

        z_add_roledetail(pstPlayer, &pstRoleDetail->PlayerAttrVal);

	//属性值计算
	z_calc_all_attr(pstAppCtx, pstEnv, pstPlayer, &pstRoleDetail->PlayerAttrVal);

	return 0;
}

int cs_chg_pos_up_cache(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, CHGPOSUPCACHEREQ* pstReq)
{
	int i;
	UinCache *pstUinCache;
	UinCache stUinHashData;
	Player *pstPlayer;
	
	stUinHashData.iUin = pstReq->Uin;
	pstUinCache = (UinCache *)sht_find(pstEnv->pstShtUinCache,&stUinHashData,UinHashCmp,UinHashCode);
	if (!pstUinCache)
	{
		return -1;
	}
	
	for (i=0; i<pstUinCache->ucRoleNum; i++)
	{
		if (strcmp(pstUinCache->astRoleListInfo[i].RoleName, pstReq->RoleName) == 0 )
		{
			if (pstUinCache->astRoleListInfo[i].Map == pstReq->Map)
				return 0;
			
			pstUinCache->astRoleListInfo[i].Map = pstReq->Map;
			pstPlayer = player_get_by_uin(pstEnv, pstReq->Uin);
			if (pstPlayer && pstPlayer->eStatus == PLAYER_STATUS_ROLE_LIST)
			{
				z_send_rolelist(pstEnv,pstPlayer, pstUinCache);
			}
			break;
		}
	}

	return 0;
	UNUSED(pstAppCtx);
}

int z_new_role_birth(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i, iIndex;
	BirthMapSort astBirMapSort[MAX_BIRTHMAP_DEF];
	BirthMapSort astTmpBirMapSort[MAX_BIRTHMAP_DEF];
	CSPKG *pstPkg = &pstEnv->stCSRes;
	CSELECTBIRTHLOCS *pstSelectBirth = &pstPkg->Body.SelectBirthLocS;
	BIRTHMAPDEF *pstBirthDef;
	ZONEMAPINST *pstMapInst;
	ZONEIDX stIdx;

	if (0 >= pstEnv->pstZoneObj->iBirthMapDef)
	{
		return -1;
	}
	
	for (i=0; i<pstEnv->pstZoneObj->iBirthMapDef; i++)
	{
		pstBirthDef = (BIRTHMAPDEF *)pstEnv->pstZoneObj->sBirthMapDef[i];
		astBirMapSort[i].iMap = pstBirthDef->MapID;

		stIdx.iID = pstBirthDef->MapID;
		stIdx.iPos = -1;
		pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
		if (NULL == pstMapInst)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "map %d no map inst", stIdx.iID);
			return -1;
		}

		astBirMapSort[i].iPlayerNum = pstMapInst->iPlayerCount;
	}

	qsort(astBirMapSort, pstEnv->pstZoneObj->iBirthMapDef, sizeof(astBirMapSort[0]), trl_comp_func);

	for (iIndex=0; iIndex<pstEnv->pstZoneObj->iBirthMapDef; iIndex++)
	{
		if (astBirMapSort[iIndex].iPlayerNum > NEW_VALLEGE_DISP)
		{
			break;
		}
	}

	if (iIndex > 1 && (astBirMapSort[iIndex-1].iPlayerNum == astBirMapSort[0].iPlayerNum))
	{
		for (i=0; i<iIndex; i++)
		{
			astTmpBirMapSort[i] =  astBirMapSort[i];
		}
	}
	else
	{
		for (i=0; i<iIndex; i++)
		{
			astTmpBirMapSort[iIndex -i -1] =  astBirMapSort[i];
		}
	}
	
	for (i=iIndex; i<pstEnv->pstZoneObj->iBirthMapDef; i++)
	{
		astTmpBirMapSort[i] = astBirMapSort[i];
	}

	pstSelectBirth->BirthMapNum = pstEnv->pstZoneObj->iBirthMapDef;
	for (i=0; i<pstSelectBirth->BirthMapNum; i++)
	{
		pstSelectBirth->MapIDList[i] = astTmpBirMapSort[i].iMap;

		if (astTmpBirMapSort[i].iPlayerNum > NEW_VALLEGE_DISP*5)
		{
			pstSelectBirth->Busy[i] = DIR_FULL;
		}
		else if (astTmpBirMapSort[i].iPlayerNum > NEW_VALLEGE_DISP*2)
		{
			pstSelectBirth->Busy[i] = DIR_BUSY;
		}
		else if (astTmpBirMapSort[i].iPlayerNum > NEW_VALLEGE_DISP)
		{
			pstSelectBirth->Busy[i] = DIR_NORMAL;
		}
		else
		{
			pstSelectBirth->Busy[i] = DIR_EXCELLENCE;
		}
	}

	Z_CSHEAD_INIT(&pstPkg->Head, SELECT_BIRTH_LOCATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkg);	
	
	return 0;
}

int z_check_cool(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	SKILLDATA *pstSkillData;
	SKILLINFO *pstSkillInfo = &pstPlayer->stRoleData.SkillInfo;
	ITEMUSECOOLLIST *pstItemUseCoolList = &pstPlayer->stRoleData.MiscInfo.ItemUseCoolList;
	STATUSINFO *pstStatusInfo =  &pstPlayer->stRoleData.StatusInfo;

	UNUSED(pstEnv);

	for (i=0; i<(int)pstSkillInfo->SkillNum; i++)
	{
		pstSkillData = &pstSkillInfo->SkillList[i];
		pstSkillData->CoolLeft = 0;
	}
	
	pstItemUseCoolList->ItemSpeCoolNum = 0;
	pstItemUseCoolList->ItemSelfCoolNum = 0;

	pstStatusInfo->BadStatusNum = 0;
	
	return 0;
}


static int role_login_restore_status(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	ZONEPWORLD *pstPworld;
	ZONEIDX stIdx;
		
	if (pstPlayer->stOnline.iOldMap == pstPlayer->stRoleData.Map)
		return 0;

	if (pstPlayer->stOnline.iOldMap < PWORLD_MAPID_BASE)
		return 0;
	stIdx.iID = GET_PWORLDID(pstPlayer->stOnline.iOldMap);
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);	
	if (pstPworld)
	{
		// 还原状态
		if (pstPworld->stPworldDef.CtrlFlag & PWORLD_CLEAN_STATUS)
		{
			pstPlayer->stOnline.iCleanStatuFlag = 1;
			z_status_online(pstEnv->pstAppCtx, pstEnv, pstPlayer ,1);
			pstPlayer->stOnline.iCleanStatuFlag = 0;
		}
	}

	return 0;
}

static int role_login_revive(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	ZONEPWORLD *pstPworld;
	ZONEIDX stIdx;
		
	if (pstPlayer->stOnline.iOldMap == pstPlayer->stRoleData.Map)
		return 0;

	if (pstPlayer->stOnline.iOldMap < PWORLD_MAPID_BASE)
		return 0;

	stIdx.iID = GET_PWORLDID(pstPlayer->stOnline.iOldMap);
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);	
	if (pstPworld)
	{
		// 还原状态
		if (pstPworld->stPworldDef.CtrlFlag & PWORLD_EXIT_REVIVER ||
			is_normal_battle_pworld(pstPworld))
		{
			if (pstPlayer->stOnline.State & CS_STAT_DEAD)
			{
				player_revive_svr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
			}
		}
	}
	return 0;
}

int is_role_login_valid_pworldinst(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLD *pstPworld,
	ZONEPWORLDINST *pstPworldInst)
{
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	ZoneClan *pstClan;
	
	// 掉线不能在进
	if (pstPworld->stPworldDef.OutLineNotIn == 1)
	{
		return 0;
	}

	if(pstPworld->stPworldDef.PolyType == PWORLD_POLY_BCT)
	{
		if (pstPlayer->stOnline.State & CS_STAT_DEAD)
		{
			return 0;
		}
	}

	if (pstPworld->stPworldDef.PworldID == UNREAL_PWORLD_ID)
	{
		return 0;
	}

	if (CLAN_CITY_MAP_ID == GET_MAPID(pstPlayer->stRoleData.Map))
	{
		if (!is_in_my_clan_city(pstEnv, pstPlayer))
		{
			return 0;
		}
	}

	if (pstPworld->stPworldDef.StrongMapID > 0)
	{
		pstClan = player_get_clan(pstEnv,  pstPlayer);
		if (!pstClan)
		{
			return 0;
		}	

		if(!is_in_strong_group_player(pstClan, pstPlayer,pstPworld->stPworldDef.StrongMapID))
		{
			return 0;
		}
	}
	
	//WID检查
	if (pstPworldInst->ullPworldWID != pstMiscInfo->PworldWID)
	{
		return 0;
	}
	//队伍副本有效检查
	else if (pstPworldInst && (PWORLD_TYPE_TEAM == pstPworldInst->iType))
	{
		ZONEPWORLDINST *pstTeamPworldInst;
		ZoneTeam *pstZoneTeam;
		
		pstZoneTeam = player_team_get(pstEnv, pstPlayer);
		if(pstZoneTeam)
		{
			pstTeamPworldInst = z_find_id_pworld_inst_byteam(pstEnv, pstZoneTeam, pstPworldInst->stPworldIdx.iID);
			if(NULL == pstTeamPworldInst || pstTeamPworldInst->ullPworldWID != pstPworldInst->ullPworldWID)
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	//单人副本
	else if( pstPworldInst && (PWORLD_TYPE_SINGLE == pstPworldInst->iType) )
	{
		if(pstPworldInst->stPworldTypeData.stSingle.ullRoleID != pstPlayer->stRoleData.RoleID)
		{
			return 0;
		}
	}
	
	return 1;
}

int is_battle_runaway(ZONESVRENV* pstEnv, ZONEPWORLD *pstPworld, ZONEPWORLDINST *pstPworldInst)
{

	BATTLEDEF *pstBattleDef;
	
	if (!is_battle_pworld(pstPworld))
	{
		return 0;
	}
	
	if(pstPworldInst->tEnd && pstPworldInst->tEnd < pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return 0;
	}

	if(pstPworldInst->bOver)
	{
		return 0;
	}

	//如果是跨服公会战不加逃跑debuf
	pstBattleDef = z_find_battledef(pstEnv, pstPworld->stPworldDef.BattleID);	
	if(pstBattleDef && pstBattleDef->FightType == BATTLE_FIGHT_TYPE_CLAN_WAR)
	{
		return 0;
	}

	return 1;
}

int z_role_login_pworld_check(ZONESVRENV* pstEnv, Player *pstPlayer, CSENTERWORLD *pstEnter)
{
	ZONEPWORLDINST *pstPworldInst = NULL;
	ZONEPWORLD *pstPworld = NULL;
	int fRunAway = 0;
	ZONEMAPINST* pstMapInst;
	ZONEIDX stIdx;

	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos =	pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	
	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (NULL == pstPworldInst)
	{
		goto _clean;
	}

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if(NULL == pstPworld)
	{
		goto _clean;
	}

	//战场相关检查(逃跑判定)
	if(is_battle_runaway(pstEnv, pstPworld, pstPworldInst))
	{
		fRunAway = 1;
	}
	
	if(!is_role_login_valid_pworldinst(pstEnv, pstPlayer, pstPworld, pstPworldInst))
	{
		goto _clean;
	}

	pstEnter->EnterMap.PworldInfoFlag = 1;
	pstEnter->EnterMap.PworldInfo.PworldInstID = pstPworldInst->iPworldInstID;
	pstEnter->EnterMap.PworldInfo.SelectGrade = pstPworldInst->stPworldselect.PworldGrade;
	pstEnter->EnterMap.PworldInfo.StartTime = pstPworldInst->tStart;

	if (pstPworldInst->iType == PWORLD_TYPE_CLAN)
	{
		ZoneClan* pstZoneClan = z_id_clan(pstEnv,  pstPworldInst->stPworldTypeData.stClan.iID);
		if (pstZoneClan)
		{
			player_clan_city_update(pstEnv,pstPlayer,pstZoneClan);
		}
	}

	//个人进入副本时间限制
	if (pstPworld->stPworldDef.PlayerInTime > 0)
	{
		if (pstPworld->stPworldDef.PworldID == pstPlayer->stRoleData.MiscInfo.PworldMisc.OutPworldID)
		{
			pstEnter->EnterMap.PworldInfo.LeftTime = pstPlayer->stRoleData.MiscInfo.PworldMisc.OutPworldTime - 
											pstEnv->pstAppCtx->stCurr.tv_sec;
		}
		else		//防范一下 
		{
			pstPlayer->stRoleData.MiscInfo.PworldMisc.OutPworldID = pstPworld->stPworldDef.PworldID;
			
			pstPlayer->stRoleData.MiscInfo.PworldMisc.OutPworldTime = pstEnv->pstAppCtx->stCurr.tv_sec + 
															pstPworld->stPworldDef.PlayerInTime;
			
			pstEnter->EnterMap.PworldInfo.LeftTime = pstPlayer->stRoleData.MiscInfo.PworldMisc.OutPworldTime - 
															pstEnv->pstAppCtx->stCurr.tv_sec;
		}
		
		if (0 > pstEnter->EnterMap.PworldInfo.LeftTime)
		{
			pstEnter->EnterMap.PworldInfo.LeftTime = 0;
		}
	}
	//大地之心倒计时
	else if(pstPworld->stPworldDef.PolyType == PWORLD_POLY_BCT)
	{	
		if (pstPworldInst->iDownTm > 0)
		{
			pstEnter->EnterMap.PworldInfo.LeftTime = pstPworldInst->iDownTm - pstEnv->pstAppCtx->stCurr.tv_sec;
			if (0 > pstEnter->EnterMap.PworldInfo.LeftTime)
			{
				pstEnter->EnterMap.PworldInfo.LeftTime = 0;
			}
		}
		else
		{
			pstEnter->EnterMap.PworldInfo.LeftTime = 0;
		}
		
		// 还在bct  发给客户端bct信息
		pstPlayer->stOnline.stBctOnline.iBctFlag = 2;
	}
	// 末世巅峰倒计时
	else if(pstPworld->stPworldDef.PolyType == PWORLD_POLY_DIANFENG)
	{
		if (pstPworldInst->iDownTm > 0)
		{
			pstEnter->EnterMap.PworldInfo.LeftTime = pstPworldInst->iDownTm - pstEnv->pstAppCtx->stCurr.tv_sec;
			if (0 > pstEnter->EnterMap.PworldInfo.LeftTime)
			{
				pstEnter->EnterMap.PworldInfo.LeftTime = 0;
			}
		}
		else
		{
			pstEnter->EnterMap.PworldInfo.LeftTime = 0;
		}
	}
	//副本自己身倒计时
	else if (pstPworldInst->tEnd > 0)
	{
		pstEnter->EnterMap.PworldInfo.LeftTime = pstPworldInst->tEnd - pstEnv->pstAppCtx->stCurr.tv_sec;
		if (0 > pstEnter->EnterMap.PworldInfo.LeftTime)
		{
			pstEnter->EnterMap.PworldInfo.LeftTime = 0;
		}
	}
	else
	{
		pstEnter->EnterMap.PworldInfo.LeftTime = -1;
	}

	return 0;
		
_clean:

	pstPlayer->stOnline.stBctOnline.iBctFlag = 1;
	//pstPlayer->stRoleData.MiscInfo.PworldWID = 0;
	//pstPlayer->stRoleData.MiscInfo.PworldInstID = 0;		
	pstPlayer->stRoleData.Map = pstPlayer->stRoleData.ExitMap;
	pstPlayer->stRoleData.Pos.X = pstPlayer->stRoleData.ExitPos_x;
	pstPlayer->stRoleData.Pos.Y = pstPlayer->stRoleData.ExitPos_y;

	return fRunAway;
}


int z_db_update_acc_share_req(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	ACCSHARE* pstShare = &pstPlayer->stAccShareData.stAccShare;

	if (pstPlayer->stAccShareData.iStat != ACC_SHARE_FINAL)
	{
		return -1;
	}

	pstShare->LastLogout = pstPlayer->stRoleData.LastLogout;
	pstShare->GrabTime = pstPlayer->iGrabTime;

	z_init_torm_head( pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, 
		"AccShare", pstPlayer->stRoleData.Uin, NULL );
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pstShare;
	stMetaData.iBuff = sizeof(*pstShare);
	
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		pstEnv->iAccountDBProtoVer))
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

static int db_select_shadow_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TORMSELECTRES *pstSelectRes)
{

	const TORMSELECTRES *pstRes = pstSelectRes;
	ACCSHARE* pstShare = &pstPlayer->stAccShareData.stAccShare;

	if (ACC_SHARE_FINAL != pstPlayer->stAccShareData.iStat)
	{
		return -1;
	}
	
	if(pstRes->SelectResult.ResultNum > 0)
	{
		SHADOWACCDATA stShadow;
		int iChange = 0;
		if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stShadow,
	                       sizeof(stShadow)) < 0)
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
					"uin %d get cache pos=%d error", pstPlayer->stRoleData.Uin, pstPlayer->iUinCachePos);
			return -1;
		}
		
		iChange = stShadow.MudExp - pstShare->TakeMudExp;
		
		pstShare->TakeMudExp = stShadow.MudExp;
		
		z_player_awardexp_change(pstEnv, pstPlayer, iChange,TRUE);
		z_db_update_acc_share_req(pstEnv,pstPlayer);
		//stShadow.MudExp = 0;
		//z_db_update_shadow_req(pstEnv, pstPlayer, &stShadow);
	}

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int db_select_shadow_req(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong ullRoleID)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstDBReq = &pstPkg->Body.SelectReq;
	SHADOWACCDATA stKey;
	DBAsyn stDBAsyn;
	TDRDATA stNetData;
	TDRDATA stMetaData;
		
	stDBAsyn.iCmd = DB_SHADOW_SELECT_RES;
	stDBAsyn.AsynCmd.ullRoleID = ullRoleID;

	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, "ShadowAccData", 
						pstPlayer->stRoleData.Uin, &stDBAsyn);
		
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OperInfo.WhereDef[0] = 0;
	pstDBReq->OffSet = 0;
	pstDBReq->Limit = 0;

	memset(&stKey, 0, sizeof(stKey));
	stKey.Uin = pstPlayer->stRoleData.Uin;


	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stKey;
	stMetaData.iBuff = sizeof(stKey);
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
}

int z_role_login_notify( TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, 
	Player *pstPlayer, int iCameraDir ,short LoginSrc)
{
	CSPKG *pstPkg = &pstEnv->stCSRes;
	CSROLELOGINRES *pstRoleLoginRes = &pstPkg->Body.RoleLoginRes;
	CSENTERWORLD *pstEnter = &pstRoleLoginRes->Enter;
	MapIndex *pstMapIndex;
	MapIndex *pstMapIndex1;
	int iPos;	
	int fRunAway = 0;
	int iCurr = pstAppCtx->stCurr.tv_sec;
	
	Z_CSHEAD_INIT(&pstPkg->Head, ROLE_LOGIN_RES);
	pstRoleLoginRes->Result = LOGIN_SUCC;
	pstRoleLoginRes->EnterFlag = 1;
	pstRoleLoginRes->LoginFlag = LoginSrc;
	pstRoleLoginRes->CtrlFlag = pstPlayer->stRoleData.CtrlFlag;
	pstRoleLoginRes->MaxLevel = pstEnv->pstConf->MaxLevel;

	pstRoleLoginRes->LevelFengChg = 0;
	if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat != GLOBAL_FINI ||
		pstEnv->pstConf->NoLevelFeng)
	{
		pstRoleLoginRes->LevelFeng.LevelFeng = 0;
		pstRoleLoginRes->LevelFeng.LevelFengEnd = 0;
	}
	else
	{
		pstRoleLoginRes->LevelFeng = pstEnv->pstGlobalObj->stGlobal.LevFeng;
		if (pstEnv->pstGlobalObj->stGlobal.Misc.LevFengBefore > 0 &&
			pstPlayer->stRoleData.Level == pstEnv->pstGlobalObj->stGlobal.Misc.LevFengBefore)
		{
			pstRoleLoginRes->LevelFengChg = 1;
		}
	}

	//if( pstEnv->iInstMode == INST_MODE_BATTLE_SVR )
	//{
	//	GET_WORLD_ID(unWorldID,pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.ZoneBusID);
	//}
	//else
	//{
	//	GET_WORLD_ID(unWorldID,pstEnv->pstAppCtx->iId);
	//}
	pstRoleLoginRes->WorldID = pstEnv->unWorldID;
	
	pstRoleLoginRes->RoleMemID = pstPlayer->iMemID;
	pstRoleLoginRes->IP = pstPlayer->stConnectInfo.stExtraInfo.IPInfo.ip;
	pstRoleLoginRes->PingCheck = pstEnv->pstConf->PingCheck;

	pstEnter->EnterMap.PworldInfoFlag = 0;

	if (pstEnv->pstConf->CloseLine)
	{
		pstEnter->SvrConfig |=  SVR_CONFIG_CLOSE_LINE;
	}

	if (pstEnv->pstConf->UnFyUse)
	{
		pstEnter->SvrConfig |=  SVR_CONFIG_UNFY_USE_JINQUAN;
	}
	
	// iOldmap:从那张地图退出的
	if (pstPlayer->stOnline.iOldMap <=0)
	{
		pstPlayer->stOnline.iOldMap = pstPlayer->stRoleData.Map;
	}
	
	if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE) //在副本中
	{
		//没计算属性之前不能加状态
		fRunAway = z_role_login_pworld_check(pstEnv, pstPlayer, pstEnter);
	}
	else
	{
		// iBctFlag=1表示 进入点不是副本
		// iBctFlag=2表示 进入点是Bct
		pstPlayer->stOnline.stBctOnline.iBctFlag = 1;
		if(pstPlayer->stRoleData.MiscInfo.AutoFlyPath > 0)
		{
			pstPlayer->stRoleData.Map = pstPlayer->stRoleData.ExitMap;
			pstPlayer->stRoleData.Pos.X = pstPlayer->stRoleData.ExitPos_x;
			pstPlayer->stRoleData.Pos.Y = pstPlayer->stRoleData.ExitPos_y;
		}

		// 校正分线
		if (pstEnv->pstConf->CloseLine)
		{
			iPos = -1;
			pstMapIndex1 = z_mappos_index(pstEnv->pstMapObj, &iPos, pstPlayer->stRoleData.Map);
			if (pstMapIndex1 && is_refer_line_map(pstMapIndex1))
			{
				if (pstMapIndex1->stMapDef.ReferMapID > 0)
				{
					pstPlayer->stRoleData.Map = pstMapIndex1->stMapDef.ReferMapID;
				}	
			}
		}
	}

	//尽可能的登录进去
	if(0 > z_role_login_entermap_test(pstAppCtx, pstEnv, pstPlayer))
	{
		return -1;
	}
	
	iPos = -1;
	pstMapIndex = z_mappos_index(pstEnv->pstMapObj, &iPos, pstPlayer->stRoleData.Map);
	if (pstMapIndex)
	{
		pstEnter->EnterMap.Effect = pstMapIndex->stMapDef.EffectID;
	}
	else
	{
		z_get_safe_mappos(pstAppCtx, pstEnv, &pstPlayer->stRoleData.Map, &pstPlayer->stRoleData.Pos);
		tlog_error(pstEnv->pstLogCat, 0, 0, "map %d no map index", pstEnter->EnterMap.Map);

		iPos = -1;
		pstMapIndex = z_mappos_index(pstEnv->pstMapObj, &iPos, pstPlayer->stRoleData.Map);
		if (NULL == pstMapIndex)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "safe map pos error");
			return -1;
		}

		pstEnter->EnterMap.Effect = pstMapIndex->stMapDef.EffectID;
	}

	if ((0 == (pstPlayer->stOnline.State & CS_STAT_AUTOFLY)) && !z_valid_pos(pstEnv->pstMapObj, pstPlayer->stRoleData.Map, &iPos, &pstPlayer->stRoleData.Pos))
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player %d no valid pos:(map=%d,x=%d,y=%d)", 
					pstPlayer->stRoleData.Uin, pstPlayer->stRoleData.Map, pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y);

		z_get_safe_mappos(pstAppCtx, pstEnv, &pstPlayer->stRoleData.Map, &pstPlayer->stRoleData.Pos);

		iPos = -1;
		pstMapIndex = z_mappos_index(pstEnv->pstMapObj, &iPos, pstPlayer->stRoleData.Map);
		if (NULL == pstMapIndex)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "safe map pos error");
			return -1;
		}

		pstEnter->EnterMap.Effect = pstMapIndex->stMapDef.EffectID;
	}

	if (pstMapIndex->stMapDef.ReferMapID > 0)
	{
		pstEnter->EnterMap.Map = pstMapIndex->stMapDef.ReferMapID;
	}
	else
	{
		pstEnter->EnterMap.Map = pstMapIndex->stMapDef.MapID;
	}
	pstEnter->EnterMap.RealMap = pstMapIndex->stMapDef.MapID;
	
	pstEnter->EnterMap.x = pstPlayer->stRoleData.Pos.X;
	pstEnter->EnterMap.y = pstPlayer->stRoleData.Pos.Y;
	pstEnter->EnterMap.InitMoveSeq = pstPlayer->stOnline.iPosSeq;
	pstEnter->EnterMap.Dir = pstPlayer->stOnline.bDir;
	pstEnter->EnterMap.CameraDir = iCameraDir;
	pstEnter->EnterMap.Span = map_span_type_get(pstEnv);

	// 猪世界地图玩家时间限制
	player_chg_map_timelimit(pstEnv, pstPlayer,pstMapIndex,&pstEnter->EnterMap.MapEndTime, 1);

	pstEnter->ClintOptLen = pstPlayer->stRoleData.ClientOpt.OptLen;
	if (pstEnter->ClintOptLen > MAX_OPT_LEN)
	{
		pstEnter->ClintOptLen = MAX_OPT_LEN;
	}
	memcpy(pstEnter->ClintOptContent, pstPlayer->stRoleData.ClientOpt.OptContent, pstEnter->ClintOptLen);

	wedding_dsn_once_flag(pstEnv, pstPlayer);
	z_add_role_detail(pstAppCtx, pstEnv, pstPlayer, &pstEnter->RoleDetail);
	pstEnter->RoleDetailFlag = 1;
	pstEnter->RoleDetail.State = pstPlayer->stOnline.State;

	pstPlayer->stOnline.stMspdInfo.MoveSpdCurr = z_get_player_mspd(pstAppCtx, pstEnv, pstPlayer);
	pstEnter->MoveSpeed = pstPlayer->stOnline.stMspdInfo.MoveSpdCurr;
	
	pstRoleLoginRes->BenchTime.BenchTimeSec = pstPlayer->stOnline.stConnectTime.sec;
	pstRoleLoginRes->BenchTime.BenchTimeUSec = pstPlayer->stOnline.stConnectTime.usec;

	pstRoleLoginRes->Perm = z_player_get_perm(pstEnv, pstPlayer);

	z_cltmsg_send(pstEnv, pstPlayer, pstPkg);

	z_update_avatar(pstEnv, pstPlayer);
	
	player_rune_num_check(pstEnv,pstPlayer);
	player_dyn_active_helper_info(pstEnv,pstPlayer,CLAN_BANISH_ACTIVE_HELP_ID,ACTIVE_STATUS_INIT);
	z_player_achievement_online(pstEnv, pstPlayer);

	if (INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
	{
		return 0;
	}

	task_check_trace(pstEnv, pstPlayer);

	//在发送一下数值
	role_login_attr_chg(pstEnv, pstPlayer, &pstEnter->RoleDetail.PlayerAttrVal);

	//player_career_chg_action(pstEnv,pstPlayer);

	if (pstPlayer->stOnline.stBctOnline.iBctFlag == 2)
	{
		bct_info(pstEnv, pstPlayer);
		bct_layer_history_notify(pstEnv, pstPlayer, pstPlayer->stRoleData.MiscInfo.BCTInfo.CurrLayer);
	}
	else if (pstPlayer->stOnline.stBctOnline.iBctFlag == 1)
	{
		ZONEPWORLD *pstPworld;
		ZONEIDX stIdx;

		stIdx.iID = GET_PWORLDID(pstPlayer->stOnline.iOldMap);
		stIdx.iPos = -1;
		
		pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);	
		if (pstPworld)
		{
			// 不在副本中了,奖励还是要给的
			if (pstPworld->stPworldDef.PolyType == PWORLD_POLY_BCT)
			{
				bct_exit_info(pstEnv, pstPlayer);
				memset(&pstPlayer->stRoleData.MiscInfo.BCTInfo,0,sizeof(pstPlayer->stRoleData.MiscInfo.BCTInfo));
			}

			if (pstPworld->stPworldDef.CtrlFlag&PWORLD_EXIT_CLEAN_XP)
			{
				fairy_camp_clean_xp(pstEnv, pstPlayer);
			}
		}
	}

	role_login_restore_status(pstEnv,pstPlayer);
	role_login_revive(pstEnv,pstPlayer);

	pstPlayer->stOnline.stBctOnline.iBctFlag = 0;

	if (pstPlayer->stOnline.State & CS_STAT_DEAD)
	{
		player_login_die_info(pstEnv, pstPlayer);
	}

	if ( (pstPlayer->stOnline.State & CS_STAT_FLY) ||  (pstPlayer->stOnline.State & CS_STAT_RIDE_ON) )
	{
		z_player_ride_action(pstEnv, pstPlayer, 1);
	}

	//战场逃跑buff
	if(fRunAway)
	{
		z_player_apply_status( pstEnv->pstAppCtx, pstEnv, pstPlayer, 
					STATUS_ID_RUNAWAY, 1, RUNAWAY_DEBUFF_TIME, 1, 0);
	}
	
	//clan_role_login(pstEnv, pstPlayer);
	player_update_online_sec(pstEnv,pstPlayer,1);
	player_comp_contonlineinfo(pstEnv, pstPlayer);
	player_login_cmp_onlinegroup(pstEnv,pstPlayer);
	bct_self_top(pstEnv, pstPlayer);
	online_award(pstEnv, pstPlayer);
	//player_safelock_login(pstEnv, pstPlayer);
	vim_role_login(pstEnv, pstPlayer);
	role_login_pkval_check(pstEnv, pstPlayer);
	player_acc_gift_req(pstEnv, pstPlayer, 0);
	player_prop_gift_req(pstEnv, pstPlayer, 0,0);
	player_trust_order_list( pstEnv, pstPlayer );

	//player_login_level_buff_info(pstEnv, pstPlayer);
	task_login_check_trace(pstEnv,pstPlayer);

	player_pworld_off_exp(pstEnv, pstPlayer, 1);
	
	if (player_team_get(pstEnv, pstPlayer))
	{
	}
	else
	{
		player_leave_team_clean_buff(pstEnv, pstPlayer, STATUS_RESULT_QIXINXIELI);
	}

	package_cangbaotu_send( pstEnv, pstPlayer);
	player_gold_raffle_num_notify(pstEnv,pstPlayer);
	player_world_city_info(pstEnv, pstPlayer);
	world_city_master_notify(pstEnv, pstPlayer, 1);
	player_battle_limit_time_action(pstEnv, pstPlayer);
	player_clan_everyday_pworld_action(pstEnv, pstPlayer);
	player_clan_active_ui_info_clan_spy(pstEnv,pstPlayer,ACTIVE_GAME_SVR_TYPE_UI_INFO);
	player_clan_active_ui_info_clan_machine_pworld(pstEnv,pstPlayer,ACTIVE_GAME_SVR_TYPE_UI_INFO);
	if (ROLE_LOGIN_FROM_BATTLE_TO_WORLD != LoginSrc)
	{
		player_vip_bar_check(pstEnv,pstPlayer);
	}
	role_gift_info(pstEnv, pstPlayer,1);	// 放在检查网吧用户之后
	player_daily_goal_info(pstEnv,pstPlayer);
	pstPlayer->stOnline.stHeti.PetWID = is_login_pet_heti(pstEnv,pstPlayer);	// login检查一下宠物合体
	pstPlayer->stOnline.cClanPos = get_world_city_clan_pos(pstEnv, pstPlayer);
	dyn_active_status_update(pstEnv,pstPlayer);
	unreal_save_info(pstEnv, pstPlayer);
	player_login_single_pworld_update(pstEnv, pstPlayer);
	player_yesterday_info(pstEnv, pstPlayer);
	player_relation_max_chg(pstEnv, pstPlayer);
	machine_dart_car_login(pstEnv, pstPlayer);
	ronglu_player_login_check(pstEnv, pstPlayer);
	losttotem_player_login_check(pstEnv, pstPlayer);
	task_login_check_dart_car(pstEnv, pstPlayer);
	player_daily_bull_action(pstEnv, pstPlayer);
	subject_player_login(pstEnv, pstPlayer);
	player_color_font_limit_action(pstEnv, pstPlayer);
	player_motion_limit_action(pstEnv, pstPlayer);

	strong_svr_strong_info(pstEnv, pstPlayer, 1);
	//strong_player_login(pstEnv, pstPlayer);
	player_login_lucky_ui(pstEnv, pstPlayer);
	
	player_action_player_clan_skill_info(pstEnv,pstPlayer, &pstPlayer->stRoleData.MiscInfo.ClanSkillInfo);

	spar_hole_check(pstEnv,pstPlayer);
	weather_player_login(pstEnv, pstPlayer);

	player_beach_info_update(pstEnv, pstPlayer);
	player_beach_info_action(pstEnv, pstPlayer);
	player_busy_info_action(pstEnv, pstPlayer);
	strong_glory_player_login(pstEnv, pstPlayer);
	clan_dart_login(pstEnv, pstPlayer);

	// 公会福利信息
	clan_boon_player_login(pstEnv, pstPlayer);
	//role_login_sparta(pstEnv, pstPlayer);
	sparta_login_check(pstEnv, pstPlayer);

	// 角色刀疤通知
	player_daoba_chg(pstEnv, pstPlayer, 0);

	player_new_role_gift_init(pstEnv, pstPlayer);
	player_cont_cover_up( pstEnv, pstPlayer, 1);

	if (!IsSameDay(iCurr, pstPlayer->stRoleData.LastLogout))
	{
		pstPlayer->stRoleData.MiscInfo.RoleStrongInfo1.StrongID = 0;	
	}
	strong_enter_id_chg( pstEnv, pstPlayer, pstPlayer->stRoleData.MiscInfo.RoleStrongInfo1.StrongID);

	school_role_login(pstEnv, pstPlayer);
	
	player_mt_gift_info(pstEnv, pstPlayer);
	player_gold_rate_notify(pstEnv, pstPlayer, 1);
	player_yanghaibang_login_check(pstEnv, pstPlayer);
	player_login_level_buff_info(pstEnv, pstPlayer);
	player_action_clan_war_award_info(pstEnv, pstPlayer);
	player_action_clan_battle_apply_info(pstEnv,pstPlayer,1);
	player_team_rongyan_login_check(pstEnv,pstPlayer);

	player_zhengyi_limit_chg(pstEnv, pstPlayer);
	player_life_brand_limit_chg(pstEnv, pstPlayer);
	player_chuancheng_notify(pstEnv,pstPlayer);

	player_paoshang_login_check(pstEnv,pstPlayer);

	player_login_check_pkgbug(pstEnv, pstPlayer);

	player_week_charm_notify(pstEnv, pstPlayer, 0, 0);

	clan_player_skill_reset_return_contri(pstEnv,pstPlayer);
	ploy_gc_player_login(pstEnv, pstPlayer);

	clan_spend_notify(pstEnv, pstPlayer,NULL);
	clan_pet_info_notify( pstEnv, pstPlayer);
	unreal_check_exp_layer( pstEnv,  pstPlayer);

	player_sj_item_clear( pstEnv, pstPlayer);

	clan_pet_gaoji_sec_notify( pstEnv, pstPlayer);
	player_login_zj_rongyan_check(pstEnv, pstPlayer);

	span_strong_occ_info_notify( pstEnv, pstPlayer);
	home_player_login_in(pstEnv, pstPlayer);
	home_login_check(pstEnv, pstPlayer);
	return 0;
}

int z_role_login(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iCameraDir, int iDir)
{
	NameMemID stNameMemID, * pstNameMemID;
	int iOldMap = pstPlayer->stRoleData.Map, i = 0;

	pstPlayer->stOnline.bDir = (unsigned char)iDir;
	z_init_role(pstAppCtx, pstEnv, pstPlayer,0);

	// 构建附魔的bitmap
	pstPlayer->stOnline.nExtendedResultBitMap = 0;
	for(i = 0; i < pstPlayer->stRoleData.Wear.ArmNum; ++i)
	{
		z_wear_extended_bit_set(pstEnv, pstPlayer, i);
	}
	
	//把藏宝图的初始数据化下
	package_cangbaotu_check( pstEnv,  pstPlayer);
	
	// 计算机能信息
	z_skill_online(pstAppCtx, pstEnv, pstPlayer);

	// 计算状态信息
	z_status_online(pstAppCtx, pstEnv, pstPlayer, 0);

	if (z_player_find_status(pstPlayer, STATUS_ID_QIANXIN))
	{
		pstPlayer->stOnline.State |= CS_STAT_QIANXIN;
	}

	player_login_verdict_huiliu(pstEnv ,pstPlayer);

	if( z_role_login_notify( pstAppCtx, pstEnv, pstPlayer, iCameraDir,ROLE_LOGIN_FROM_DB_TO_WORLD ) < 0 )
	{
		return -1;
	}
	
	if (iOldMap != pstPlayer->stRoleData.Map)
	{
		if (pstPlayer->stRoleData.MiscInfo.AutoFlyPath > 0)
		{
			pstPlayer->stRoleData.MiscInfo.AutoFlyPath = 0;
			pstPlayer->stOnline.State &= ~CS_STAT_AUTOFLY;
		}
	}

	if (0 > z_player_entermap(pstAppCtx, pstEnv, pstPlayer, pstPlayer->stRoleData.Map, &pstPlayer->stRoleData.Pos, 1))
	{
		player_kickoff(pstAppCtx, pstEnv, pstPlayer, ZONEERR_KICK201);
		return -1;
	}

	pstPlayer->eStatus = PLAYER_STATUS_ROLE_LOGIN;
	tlog_info(pstEnv->pstLogCat, 0, 0, "Status=RoleLogin Uin=%d AccName=%s RoleName=%s", 
			pstPlayer->stRoleData.Uin, pstPlayer->szAccount, pstPlayer->stRoleData.RoleName);
	
	pstPlayer->stOnline.cCheckCltTimeFlag = 1;

	stNameMemID.iID = pstPlayer->iMemID;
	STRNCPY(stNameMemID.szName, pstPlayer->stRoleData.RoleName, sizeof(stNameMemID.szName));
	pstNameMemID = sht_insert_unique(pstEnv->pstShtNameMemID, &stNameMemID, NameMemIDHashCmp, NameMemIDHashCode);
	if (pstNameMemID)
	{
		memcpy(pstNameMemID, &stNameMemID, sizeof(*pstNameMemID));
	}
	else
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "sht insert name %d player fail", pstPlayer->stRoleData.Uin);
	}

	if (iOldMap != pstPlayer->stRoleData.Map)
	{
		player_package_check_mapitem(pstEnv, pstPlayer);
		//迷之塔 去除特定坏buf
		poly_mzt_del_status(pstEnv, pstPlayer,iOldMap,MZT_THREE_LAYER_BAD_STATUS);
	}

	//通知类的尽量放到z_role_login_notify 中，因为角色接管也需要获得通知
	
	tmp_item_check(pstEnv, pstPlayer, iOldMap);
	relation_role_login(pstEnv, pstPlayer);
	mail_role_login(pstEnv, pstPlayer);
	z_role_svr_status(pstEnv, pstPlayer);
	z_game_event_role_login(pstEnv, pstPlayer);
	login_check_chg_rolename(pstEnv, pstPlayer);
	role_online_to_mud(pstEnv, pstPlayer, ROLE_GAME_ONLINE);
	z_account_db_update_online(pstEnv, pstPlayer,1);
	z_account_db_update_firstlogin(pstEnv, pstPlayer);
	z_select_account_safepwd(pstEnv, pstPlayer);
	player_login_up_vig(pstEnv, pstPlayer);

	store_player_login(pstEnv, pstPlayer);

	z_role_loging_oplog(pstAppCtx, pstEnv, pstPlayer);
	//通知类的尽量放到z_role_login_notify 中，因为角色接管也需要获得通知

	if(pstPlayer->stRoleData.MiscInfo.PkTime > 0)
	{
		pstPlayer->stOnline.tPkEndTime = pstEnv->pstAppCtx->stCurr.tv_sec + pstPlayer->stRoleData.MiscInfo.PkTime;
		player_pk_time_action(pstEnv, pstPlayer);
	}

	//闪避时间，赋值一下，防止计算时候溢出
	//pstPlayer->stOnline.stSkillMissTime = pstEnv->pstAppCtx->stCurr;

	//会标
	player_clan_map_pos_info(pstEnv, pstPlayer);

	player_acc_share_init(pstEnv,pstPlayer);

	z_role_snapshot_oplog(pstAppCtx, pstEnv, pstPlayer);

	player_dianfeng_login_check(pstEnv,pstPlayer);
	wedding_login_check(pstEnv, pstPlayer);
	
	return 0;
}


static int z_select_account_safepwd(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;
	ACCSAFEPWD stAccSafePwd;
	
	stDBAsyn.iCmd = DB_SELECT_ACC_SAFEPWD;
	stDBAsyn.AsynCmd.ullRoleID = pstPlayer->stRoleData.RoleID;
	
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, "AccSafePwd", pstPlayer->stRoleData.Uin, &stDBAsyn);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	sprintf(pstSelectReq->OperInfo.WhereDef, "where Uin=%d", pstPlayer->stRoleData.Uin);
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 1;

	memset(&stAccSafePwd, 0, sizeof(stAccSafePwd));
	stAccSafePwd.Uin = pstPlayer->stRoleData.Uin;
			
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stAccSafePwd;
	stMetaData.iBuff = sizeof(stAccSafePwd);

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iAccountDBID))
	{
		return -1;
	}
	return 0;
}


//author:linzhognkai    获得二级密码
int z_select_role_secondpwd(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;
	ROLESECONDPWD stRoleSecondPwd;
	
	stDBAsyn.iCmd = DB_ROLE_SECOND_PWD_SELECT;
	stDBAsyn.AsynCmd.ullRoleID = pstPlayer->stOnline.iRoleID;
	
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, "RoleSecondPwd", pstPlayer->stRoleData.Uin, &stDBAsyn);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	sprintf(pstSelectReq->OperInfo.WhereDef, "where RoleID=%llu", pstPlayer->stOnline.iRoleID);
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 1;

	memset(&stRoleSecondPwd, 0, sizeof(stRoleSecondPwd));
	stRoleSecondPwd.RoleID = pstPlayer->stOnline.iRoleID;
			
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRoleSecondPwd;
	stMetaData.iBuff = sizeof(stRoleSecondPwd);

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}
	return 0;

}

//linzhongkai    把从数据库中得到的数据复制到pstPlayer->stOnline.cSecondPwd
int z_role_secondpwd_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, TORMSELECTRES *pstSelectRes,DBAsyn *pstDbsyn)
{
	ROLESECONDPWD stRoleSecondPwd;

	if (1 != pstSelectRes->SelectResult.ResultNum)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
					"z_role_secondpwd_res uin=%d ", pstPlayer->stRoleData.Uin);
		return -1;
	}

	if (pstPlayer->stOnline.iRoleID != pstDbsyn->AsynCmd.ullRoleID)
	{
		return -1;
	}

	if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stRoleSecondPwd, sizeof(stRoleSecondPwd)))
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
					"uin=%d torm_fetch_one_meta fail", pstPlayer->stRoleData.Uin);
		return -1;
	}

	if (strcmp(stRoleSecondPwd.SecondPwd, pstPlayer->stOnline.cSecondPwd))
	{
		STRNCPY(pstPlayer->stOnline.cSecondPwd,stRoleSecondPwd.SecondPwd, SECOND_PWD_LEN);
	}

	player_send_to_clt_popup_dialog(pstAppCtx,pstEnv,pstPlayer,pstDbsyn->AsynCmd.ullRoleID);	
	
	return 0;

}


int z_db_role_login_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TORMSELECTRES *pstSelectRes)
{
	CSPKG stPkg;
       CSKICKOFFINFO *pstKickoffInfo;
	ROLEDATA stRoleData;

	if (1 != pstSelectRes->SelectResult.ResultNum)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
					"select uin=%d role num=%d", pstPlayer->stRoleData.Uin,
					pstSelectRes->SelectResult.ResultNum);
		return -1;
	}

	memset(&stRoleData, 0, sizeof(stRoleData));
	if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stRoleData, sizeof(stRoleData)))
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
					"uin=%d torm_fetch_one_meta fail", pstPlayer->stRoleData.Uin);
		return -1;
	}

	pstPlayer->stRoleData = stRoleData;

	if(pstPlayer->stRoleData.BlockEnd > pstAppCtx->stCurr.tv_sec ||
		pstPlayer->stRoleData.BlockEnd < 0)
       {
               pstKickoffInfo = &stPkg.Body.KickoffInfo;
               STRNCPY(pstKickoffInfo->Msg, LJY_DB701, sizeof(pstKickoffInfo->Msg));

               Z_CSHEAD_INIT(&stPkg.Head, KICKOFF_INFO);
               z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
               return -1;
       }

	

	//新角色的处理
	if (!pstEnv->pstConf->CloseLine)
	{
		if (0 == pstPlayer->stRoleData.Map && 0 == pstPlayer->stRoleData.LastLogin)
		{
			return z_new_role_birth(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}	
	}
	else
	{
		if (0 == pstPlayer->stRoleData.Map && 0 == pstPlayer->stRoleData.LastLogin)
		{
			stPkg.Body.SelectBirthLocC.MapID = 2000;
			return player_role_selbirth(pstAppCtx, pstEnv, pstPlayer, NULL, &stPkg);
		}	
	}
	
	z_role_login(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0, 0);
	return 0;
}

int z_acc_safepwd_res(ZONESVRENV* pstEnv, Player *pstPlayer, TORMSELECTRES *pstSelectRes,DBAsyn *pstDbsyn)
{
	ACCSAFEPWD stAccSafePwd;

	if (1 != pstSelectRes->SelectResult.ResultNum)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
					"z_acc_safepwd_res uin=%d ", pstPlayer->stRoleData.Uin);
		return -1;
	}

	if (pstPlayer->stRoleData.RoleID != pstDbsyn->AsynCmd.ullRoleID)
	{
		return -1;
	}

	if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stAccSafePwd, sizeof(stAccSafePwd)))
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
					"uin=%d torm_fetch_one_meta fail", pstPlayer->stRoleData.Uin);
		return -1;
	}

	if (	/*stAccSafePwd.SafePwd[0] != 0 &&*/
		strcmp(stAccSafePwd.SafePwd, pstPlayer->stRoleData.MiscInfo.LockPassWD) &&
		pstPlayer->stRoleData.Level >= SAFELOCK_LEVEL)
	{
		STRNCPY(pstPlayer->stRoleData.MiscInfo.LockPassWD, 
					stAccSafePwd.SafePwd, SAFE_LOCK_PASSWD_LEN);
	}

	player_safelock_login(pstEnv, pstPlayer);
	return 0;
}

int z_db_acc_share_insert_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TORMPKG *pstTormPkg)
{
	AccShareData* pstData = &pstPlayer->stAccShareData;
	TORMINSERTRES *pstInsertRes = &pstTormPkg->Body.InsertRes;

	if (pstData->iStat != ACC_SHARE_INS)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
					"z_db_acc_share_insert_res state error Uin=%d state=%d",  pstPlayer->stRoleData.Uin,
					pstData->iStat);
		return -1;
	}

	if (TORM_ERR_IS_ERROR(pstInsertRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
					"z_db_acc_share_insert_res fail Uin=%d",  pstPlayer->stRoleData.Uin);
		
		pstData->iStat = ACC_SHARE_FAIL;
	}
	else
	{
		player_acc_share_final(pstEnv,pstPlayer);
	}

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}


int z_db_role_insert_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TORMPKG *pstTormPkg, ROLELISTINFO *pstRoleListInfo1)
{
	CSPKG *pstPkg = &pstEnv->stCSRes;
	CSROLENEWRES *pstNewRes = &pstPkg->Body.RoleNewRes;
	TORMINSERTRES *pstInsertRes = &pstTormPkg->Body.InsertRes;
	UinCache *pstUinCache;
	int iValid;
	ROLELISTINFO *pstRoleListInfo2;
	ACCOUNTROLE stAccRole;

	memset(pstNewRes, 0, sizeof(*pstNewRes));
	if (TORM_ERR_IS_ERROR(pstInsertRes->ErrNo))
	{
		if (TORM_ERR_MAKE_SVR_ERROR(TORM_ERROR_QUERY_DUPKEY) == (unsigned int)pstInsertRes->ErrNo)
		{
			pstNewRes->Result = NEW_DUP; 
		}
		else
		{
			pstNewRes->Result = NEW_ERR;
		}
	}
	else
	{
		pstNewRes->RoleInfo.Career = pstRoleListInfo1->Career;
		pstNewRes->RoleInfo.Gender = pstRoleListInfo1->Gender;
		pstNewRes->RoleInfo.RoleID = pstRoleListInfo1->RoleID;
		pstNewRes->RoleInfo.Head = pstRoleListInfo1->Head;
		pstNewRes->RoleInfo.Face = pstRoleListInfo1->Face;
		pstNewRes->RoleInfo.Level = pstRoleListInfo1->Level;
		pstNewRes->RoleInfo.Map= pstRoleListInfo1->Map;
		pstNewRes->RoleInfo.Hair = pstRoleListInfo1->Hair;
		STRNCPY(pstNewRes->RoleInfo.RoleName, pstRoleListInfo1->RoleName, sizeof(pstNewRes->RoleInfo.RoleName));

		z_birth_avatar(pstAppCtx, pstEnv, &pstRoleListInfo1->AvatarProf, 
				pstRoleListInfo1->Career, pstRoleListInfo1->Gender, pstRoleListInfo1->ApperalSelect);

		memcpy(&pstNewRes->RoleInfo.Avatar, &pstRoleListInfo1->AvatarProf, sizeof(pstNewRes->RoleInfo.Avatar));
		
		//update Player role list
		pstUinCache = (UinCache *)sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos, &iValid);
		if (NULL == pstUinCache || 0 == iValid || pstUinCache->iUin != pstPlayer->stRoleData.Uin)
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
						"z_db_role_insert_res Uin=%d uincache",  pstPlayer->stRoleData.Uin);
			return -1;
		}
		if (pstUinCache->ucRoleNum >= ROLE_USER_NUM)
		{
			return -1;
		}
		
		pstRoleListInfo2 = &pstUinCache->astRoleListInfo[pstUinCache->ucRoleNum];
		memcpy(pstRoleListInfo2, pstRoleListInfo1, sizeof(*pstRoleListInfo1));
		
		pstUinCache->ucRoleNum++;

		memset(&stAccRole,0,sizeof(stAccRole));
		
		stAccRole.RoleWID = pstNewRes->RoleInfo.RoleID;
		STRNCPY(stAccRole.RoleName, pstNewRes->RoleInfo.RoleName, 
			    CCH(stAccRole.RoleName));
		STRNCPY(stAccRole.Pass9Account, pstPlayer->szAccount, 
			    CCH(stAccRole.Pass9Account));
		stAccRole.Uin = pstPlayer->stRoleData.Uin;
		stAccRole.DelTime = 0;
		stAccRole.CreateTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		stAccRole.OnlineFlag = 0;
		stAccRole.Level = 1;
		GET_REGION_ID(stAccRole.RegionID, pstAppCtx->iId);
		GET_WORLD_ID(stAccRole.ServerID, pstAppCtx->iId);
		
		z_account_db_insert(pstEnv, pstPlayer, &stAccRole);
		z_rolenew_oplog(pstAppCtx, pstEnv, pstPlayer,pstNewRes->RoleInfo.RoleID, 
						pstNewRes->RoleInfo.Career, pstNewRes->RoleInfo.Gender,pstNewRes->RoleInfo.RoleName);

		//预创建的角色,并且还没有领取或礼包
		if (pstEnv->pstConf->PreCreateRole &&
			!(pstPlayer->UinFlag & UIN_FLAG_PRECREATE_GIFT))
		{
			//stRoleData.CtrlFlag = ROLE_PRE_FLAG;
			//todo:sms
			player_action_precreate_gift_verify(pstEnv,pstPlayer,
								ACTION_PRECREATE_GIFT_VERIFY_OP_OPEN,0);
		}
	}

	

	Z_CSHEAD_INIT(&pstPkg->Head, ROLE_NEW_RES);
	return z_cltmsg_send(pstEnv, pstPlayer, pstPkg);
}

int z_pre_create_award_money(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int a;
	
	if ((pstPlayer->stRoleData.CtrlFlag & ROLE_PRE_FLAG) &&
		pstEnv->pstConf->PreCreateBindGodCoin > 0)
	{
		// 预创建角色第一次登录
		if (pstPlayer->stRoleData.LastLogin  <= 0)
		{
			a = 	pstEnv->pstGlobalObj->stGlobal.Misc.NewWorldFlags.FirstStartupTime - pstPlayer->stRoleData.CreateTime;

			a = a/3600*pstEnv->pstConf->PreCreateBindGodCoin;

			if (a > 0)
			{
				package_add_money(pstEnv,  pstPlayer, ATTR_ID_BINDGODCOIN, a);	
			}
		}
	}
	return 0;
}

int z_db_checkname_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TORMPKG *pstTormPkg, unsigned char cCheckName)
{
	
	UinCache *pstUinCache;
	int iValid;
	CSPKG *pstCSPkg = &pstEnv->stCSRes;
	CSROLENEWRES *pstNewRes = &pstCSPkg->Body.RoleNewRes;

	TORMINSERTREQ *pstInsertReq = &pstTormPkg->Body.InsertReq;
	TORMINSERTRES *pstInsertRes = &pstTormPkg->Body.InsertRes;

	ROLEDATA stRoleData;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;
	NewPlayerInfo *pstNewPlayerInfo = &stDBAsyn.AsynCmd.stNewPlayer;
	ROLELISTINFO *pstRoleListInfo = &stDBAsyn.AsynCmd.stRoleListInfo;

	UNUSED(&cCheckName);
	
	pstUinCache = (UinCache *)sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos, &iValid);
	if (NULL == pstUinCache || 0 == iValid || pstUinCache->iUin != pstPlayer->stRoleData.Uin)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
					"z_db_checkname_res Uin=%d uincache",  pstPlayer->stRoleData.Uin);
		return -1;
	}
	if (pstUinCache->ucRoleNum >= ROLE_USER_NUM)
	{
		return -1;
	}

	if (TORM_ERR_IS_ERROR(pstInsertRes->ErrNo))
	{
		memset(pstNewRes, 0, sizeof(*pstNewRes));
		
		if (TORM_ERR_MAKE_SVR_ERROR(TORM_ERROR_QUERY_DUPKEY) == (unsigned int)pstInsertRes->ErrNo)
		{
			pstNewRes->Result = NEW_DUP; 
		}
		else
		{
			pstNewRes->Result = NEW_ERR;
		}

		Z_CSHEAD_INIT(&pstCSPkg->Head, ROLE_NEW_RES);

		return z_cltmsg_send(pstEnv, pstPlayer, pstCSPkg);
	}

	if (pstTormPkg->Head.AsynCallBackDataLen-sizeof(int) != sizeof(*pstNewPlayerInfo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "check name fail");
		return -1;
	}
	
	memcpy(pstNewPlayerInfo, pstTormPkg->Head.AsynCallBackData+sizeof(int), pstTormPkg->Head.AsynCallBackDataLen-sizeof(int));
	memset(&stRoleData, 0, sizeof(stRoleData));
	stRoleData.Career = pstNewPlayerInfo->stNew.Career;
	stRoleData.CreateTime = pstAppCtx->stCurr.tv_sec;
	stRoleData.Gender = pstNewPlayerInfo->stNew.Gender;
	stRoleData.Head = pstNewPlayerInfo->stNew.Head;
	stRoleData.Face = pstNewPlayerInfo->stNew.Face;
	stRoleData.Hair = pstNewPlayerInfo->stNew.Hair;
	STRNCPY(stRoleData.RoleName, pstNewPlayerInfo->stNew.RoleName, sizeof(stRoleData.RoleName));
	stRoleData.RoleID = pstNewPlayerInfo->ullGid;
	stRoleData.Uin = pstPlayer->stRoleData.Uin;
	stRoleData.Level = 1;
	stRoleData.RoleDetail.HPCurr =  z_get_career_basehp(pstEnv, stRoleData.Career, stRoleData.Level);
	stRoleData.RoleDetail.MPCurr = z_get_career_basemp(pstEnv, stRoleData.Career, stRoleData.Level);
	stRoleData.RoleDetail.VIMCurr = z_role_vimmax(pstEnv, stRoleData.Level);
	stRoleData.RoleDetail.SpeMovePower = NEW_SPEMOVE;
	stRoleData.RoleDetail.GreenJade = z_get_max_vig(pstEnv, 1);
	stRoleData.RoleDetail.VigorCurr = 0;
	stRoleData.ApperalSelect = pstNewPlayerInfo->stNew.ApperalSelect;
	stRoleData.Buddy.FirendMax = INIT_MAX_BUDDY_FRIEND;
	stRoleData.WorldID = pstEnv->unWorldID;

	if (pstEnv->pstConf->PreCreateRole)
	{
		stRoleData.CtrlFlag = ROLE_PRE_FLAG;
	}

	//set some info in pkg, in real env, this info should tdr_hton
	stDBAsyn.iCmd = DB_ROLE_INSERT;
	memset(pstRoleListInfo, 0, sizeof(*pstRoleListInfo));
	pstRoleListInfo->Career = stRoleData.Career;
	pstRoleListInfo->Gender = stRoleData.Gender;
	pstRoleListInfo->Face  = stRoleData.Face;
	pstRoleListInfo->RoleID = stRoleData.RoleID;
	pstRoleListInfo->Head = stRoleData.Head;
	STRNCPY(pstRoleListInfo->RoleName, stRoleData.RoleName, sizeof(pstRoleListInfo->RoleName));
	pstRoleListInfo->Uin = stRoleData.Uin;
	pstRoleListInfo->Level = 1;
	pstRoleListInfo->Hair = stRoleData.Hair;
	pstRoleListInfo->ApperalSelect = stRoleData.ApperalSelect;
		
	z_init_torm_head(pstAppCtx, pstEnv, &pstTormPkg->Head, TORM_INSERT_REQ, "RoleData", pstPlayer->stRoleData.Uin, &stDBAsyn);
	pstInsertReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstInsertReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstInsertReq->OperInfo.WhereDef[0] = 0;
		
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRoleData;
	stMetaData.iBuff = sizeof(stRoleData);

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstTormPkg, &stMetaData, pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}

	if (0 > z_torm_send(pstAppCtx, pstEnv, pstTormPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}

	return 0;
}

// 更新NameID表
int z_db_name_id_update( ZONESVRENV * pstEnv, const NAMEIDTYPE * pstNameIDType )
{
	DBAsyn stDBAsyn;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;

	z_init_torm_head( pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, 
		"NameIDType", -1, &stDBAsyn );
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pstNameIDType;
	stMetaData.iBuff = sizeof(*pstNameIDType);
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

/*更新其他数据库*/
int z_role_del_up_else(ZONESVRENV* pstEnv, tdr_ulonglong ullRoleID,int iCareer, char *pszName)
{
	NAMEIDTYPE stNameIDType;
	
	shopping_del_traderecord_by_del_role(pstEnv,ullRoleID);
	
	//更新排行榜
	range_delete_role(pstEnv, ullRoleID, iCareer);

	// 做邮件相关的处理
	mail_role_on_delete(pstEnv, pszName);

	// 更新NameID，标记被删除的角色名
	bzero( &stNameIDType, sizeof(stNameIDType) );
	stNameIDType.GID = ullRoleID;
	stNameIDType.NameType = 64 | ROLE_NAME;
	z_db_name_id_update( pstEnv, &stNameIDType );
	
	clan_global_del_mem_slow(pstEnv, ullRoleID);
	return 0;
}

// iQuite--彻底删除 1-是 0-否
int z_role_del_update(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong ullRoleID, int iQuite)
{
	int i;
	int iValid;
	ROLELISTINFO *pstRoleListInfo;
	UinCache *pstUinCache;
	OPROLEDEL stOpRoleDel;
	int iCareer = CAREER_NON;
	char szRoleName[ROLE_NAME_LEN] = "";

	memset(&stOpRoleDel, 0, sizeof(stOpRoleDel));
	stOpRoleDel.RoleID = ullRoleID;
	STRNCPY( stOpRoleDel.RoleName, "NULL", CCH(stOpRoleDel.RoleName) );
	
	pstUinCache = (UinCache *)sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos, &iValid);
	if (NULL == pstUinCache || 0 == iValid || pstUinCache->iUin != pstPlayer->stRoleData.Uin)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
					"z_db_role_delete_res Uin=%d uincache",  pstPlayer->stRoleData.Uin);
	}
	else
	{
		//更新rolelist
		for (i=pstUinCache->ucRoleNum-1; i>=0; i--)
		{
			pstRoleListInfo = &pstUinCache->astRoleListInfo[i];
			if (pstRoleListInfo->RoleID == ullRoleID)
			{
				STRNCPY(szRoleName, pstRoleListInfo->RoleName, CCH(szRoleName));
				iCareer = pstRoleListInfo->Career;

				stOpRoleDel.Career = iCareer;
				stOpRoleDel.Level = pstRoleListInfo->Level;
				STRNCPY( stOpRoleDel.RoleName, pstRoleListInfo->RoleName, CCH(stOpRoleDel.RoleName) );

				if(iQuite)
				{
					pstUinCache->ucRoleNum--;
					if (i != pstUinCache->ucRoleNum)
					{
						memmove(pstRoleListInfo, &pstUinCache->astRoleListInfo[i+1], 
									sizeof(*pstRoleListInfo)*(pstUinCache->ucRoleNum-i));
					}
				}
				else
				{
					pstRoleListInfo->DelTime = pstEnv->pstAppCtx->stCurr.tv_sec;
					pstRoleListInfo->CtrlFlag = 0;
				}
				
				break;
			}
		}
	}

	if (iQuite)
	{
		if (iCareer > CAREER_NON)
		{
			z_role_del_up_else(pstEnv,ullRoleID,iCareer, szRoleName);
			z_role_del_oplog(pstEnv, pstPlayer, &stOpRoleDel);
		}
		else
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,	"z_role_del_update:iCareer<=0");
		}
	}
	return 0;
}

ROLELISTINFO *z_get_one_rolelistinfo(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong ullRoleID,int *piRet)
{
	int i;
	int iValid;
	UinCache *pstUinCache;

	if(piRet)
		*piRet = ERROR_UINCACHE_FIND_ROLE;
	
	pstUinCache = (UinCache *)sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos, &iValid);
	if (NULL == pstUinCache || 0 == iValid || pstUinCache->iUin != pstPlayer->stRoleData.Uin)
	{
		if(piRet)
			*piRet = ERROR_FIND_UINCACHE;
		return NULL;
	}

	for (i=pstUinCache->ucRoleNum-1; i>=0; i--)
	{
		if (pstUinCache->astRoleListInfo[i].RoleID == ullRoleID)
		{
			if(piRet)
				*piRet = 0;
			return &pstUinCache->astRoleListInfo[i];
		}
	}
	return NULL;
}

int z_db_role_undel_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
								TORMPKG *pstTormPkg, tdr_ulonglong ullRoleID)
{
	TORMUPDATERES *pstUpdateRes = &pstTormPkg->Body.UpdateRes;

	if (TORM_ERR_IS_ERROR(pstUpdateRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
				"z_db_role_undel_res error Uin=%d errno=%d",
				pstPlayer->stRoleData.Uin,
				pstUpdateRes->ErrNo );
	}
	else
	{
		int iRet;
		ROLELISTINFO *pstRoleListInfo = z_get_one_rolelistinfo(pstEnv, pstPlayer,ullRoleID,&iRet);
		if (iRet == ERROR_FIND_UINCACHE)
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
						"z_db_role_undel_res:ERROR_FIND_UINCACHE");
		}
		else if (iRet == ERROR_UINCACHE_FIND_ROLE)
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
						"z_db_role_undel_res:ERROR_UINCACHE_FIND_ROLE");
		}
		
		if (pstRoleListInfo)
		{
			pstRoleListInfo->DelTime = 0;
			pstRoleListInfo->CtrlFlag = 0;
		}
		
		z_db_role_list(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		z_account_db_update_deltime(pstEnv, pstPlayer, ullRoleID,0);
	}
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_db_delete_respite_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
								TORMPKG *pstTormPkg, tdr_ulonglong ullRoleID, int iQuite)
{
	//CSPKG *pstPkg = &pstEnv->stCSRes;
	//CSROLEDELRES *pstDelRes = &pstPkg->Body.RoleDelRes;
	TORMUPDATERES *pstUpdateRes = &pstTormPkg->Body.UpdateRes;

	UNUSED( pstAppCtx );

	//pstDelRes->RoleID = ullRoleID;

	if (TORM_ERR_IS_ERROR(pstUpdateRes->ErrNo))
	{
		//pstDelRes->Result = DEL_ERR;
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
				"db delete error Uin=%d errno=%d",
				pstPlayer->stRoleData.Uin,
				pstUpdateRes->ErrNo );
	}
	else
	{
		//pstDelRes->Result = DEL_SUCC;
		z_role_del_update(pstEnv, pstPlayer, ullRoleID,iQuite);
		z_db_role_list(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}
	
	//Z_CSHEAD_INIT(&pstPkg->Head, ROLE_DEL_RES);
	//return z_cltmsg_send(pstEnv, pstPlayer, pstPkg);
	return 0;
}

int z_db_role_delete_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TORMPKG *pstTormPkg, tdr_ulonglong ullRoleID )
{
	CSPKG *pstPkg = &pstEnv->stCSRes;
	CSROLEDELRES *pstDelRes = &pstPkg->Body.RoleDelRes;
	TORMDELETERES *pstDeleteRes = &pstTormPkg->Body.DeleteRes;

	UNUSED( pstAppCtx );

	pstDelRes->RoleID = ullRoleID;

	if (TORM_ERR_IS_ERROR(pstDeleteRes->ErrNo))
	{
		pstDelRes->Result = DEL_ERR;

		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
				"db delete error Uin=%d errno=%d",
				pstPlayer->stRoleData.Uin,
				pstDeleteRes->ErrNo );
	}
	else
	{
		pstDelRes->Result = DEL_SUCC;
		z_role_del_update(pstEnv, pstPlayer, ullRoleID,1);
	}
	
	Z_CSHEAD_INIT(&pstPkg->Head, ROLE_DEL_RES);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkg);

	//10级前把名字也删除掉
	z_db_role_del_name_req(pstEnv, pstPlayer,  ullRoleID);

	// 删除account表中记录
	z_account_db_del(pstEnv, pstPlayer->stRoleData.Uin, ullRoleID);

	// 删除角色在拉一次角色列表
	// 合服后角色有可能多余8个
	z_db_role_list(pstAppCtx, pstEnv, pstPlayer);
	return 0;
}

int z_role_logout_team_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZoneTeam * pstZoneTeam;
	
	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if( INST_MODE_ZONE_SVR == pstEnv->iInstMode &&
		pstZoneTeam && 
		pstPlayer == player_team_get_captain_player(pstEnv, pstZoneTeam) &&
		!(pstPlayer->stOnline.eStatusAssistant&PLAYER_STATUS_BATTLE_LOGIN_READY) )
	{
		int i, iIdx = -1, iFirstOnlineIdx = -1;
		char szMsg[512];
		Player *pstCaptain = NULL;
		TEAMMEMBER *pstTeamMember;
		
		for(i = pstZoneTeam->stTeamInfo.MemberNum - 1; i >= 0; --i)
		{
			pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[i];
			pstCaptain = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeamMember->MemberName);
			if (NULL == pstCaptain)
			{
				continue;
			}

			if(pstCaptain == pstPlayer)
			{
				iIdx = i;
			}
			else
			{
				iFirstOnlineIdx= i;
			}
		}

		if(iFirstOnlineIdx < 0 || iIdx < 0)
			return 0;

		pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[iIdx];
		pstTeamMember->Position = TEAM_POSITION_MEMBER;

		pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[iFirstOnlineIdx];
		pstTeamMember->Position = TEAM_POSITION_CAPTAIN;
		
		sprintf(szMsg, LJY_TEAM9, pstTeamMember->MemberName);
		player_team_broadcast_tip(pstEnv, pstZoneTeam, szMsg);

		snprintf(pstZoneTeam->stTeamInfo.TeamName, sizeof(pstZoneTeam->stTeamInfo.TeamName),
				LJY_TEAM3, pstTeamMember->MemberName);

		autopworld_invite_enter(pstEnv, NULL, pstZoneTeam->iAutoPworldID, 1, pstZoneTeam, 1);	
		player_team_adjust_captain_pos(pstEnv, pstZoneTeam);
		player_team_all_action(pstEnv, pstZoneTeam);
		if(pstCaptain)
		{
			//队长的副本加入队伍副本
			team_member_pworld_update(pstEnv, pstZoneTeam, pstCaptain, 1);
		}
	}

	return 0;
}

//退出要处理的七七八八的都丢到这里
int z_db_role_logout_pre(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iMoney = pstPlayer->stRoleData.RoleDetail.Money;
	LEVELUP *pstLevelUP = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	Monster *pstBiaoChe = mon_biaoche_get_by_player(pstEnv, pstPlayer);

	if(pstBiaoChe)
	{	
		pstBiaoChe->ullPKID = 0;
	}
	
	if(pstLevelUP && pstLevelUP->MaxPackageMoney > 0)
	{
		if(iMoney > pstLevelUP->MaxPackageMoney)
		{
			int iDecMoney = iMoney-pstLevelUP->MaxPackageMoney;
			package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, iDecMoney);
			package_add_money(pstEnv, pstPlayer, ATTR_ID_BULL, iDecMoney);

			z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_LEVLIMIT, 0,  iDecMoney, 0, "等级限制");
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_LEVLIMIT, 0,  0, iDecMoney, "等级限制");
		}
	}
	
	tmp_item_drop(pstEnv, pstPlayer,1);
	z_role_logout_team_check(pstEnv, pstPlayer);
	player_trace_act_break_prepare(pstEnv, pstPlayer, BRK_PRE_ACT_LOGOUT);

	//乘客 
	if (pstPlayer->stOnline.State & CS_STAT_HITCH_RIDE)
	{
		z_hitch_ride_clear(pstEnv, pstPlayer);
	}
	//骑乘上有乘客
	else if((pstPlayer->stOnline.State & (CS_STAT_RIDE_ON | CS_STAT_FLY)) && 
				pstPlayer->stOnline.stRideOnInfo.iPassengerNum) 
	{
		int i;
		Player *pstTmpPlayer;

		for(i = 0; i<pstPlayer->stOnline.stRideOnInfo.iPassengerNum; i++)
		{
			pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stRideOnInfo.PassengerID[i]);
			if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				continue;
			}

			//客人下马
			z_hitch_ride_clear(pstEnv, pstTmpPlayer);
		}
	}

	//下机械
	player_ride_machine_brk(pstEnv, pstPlayer, 1);
	
	pet_fuse_cancel(pstEnv, pstPlayer);

	player_del_totem(pstEnv, pstPlayer);

	//离开排队队列
	if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
	{
		z_account_db_update_worldid(pstEnv, pstPlayer);
		z_exit_pworld_queue(pstEnv, pstPlayer, pstPlayer->stOnline.stPworldQueue.iPworldID, 0);
		player_span_apply_queue_clean(pstEnv, pstPlayer);
	}

	//删除 释放的标记类状态
	z_del_apply_unique_status(pstEnv->pstAppCtx, pstEnv, pstPlayer);

	// 处理交易
	if (pstPlayer->stOnline.stExchgOnline.bExchgStat != EXCHG_NO)
	{
		exchg_logout(pstEnv, pstPlayer);
	}

	player_logout_new_gift( pstEnv, pstPlayer);

	player_logout_up_emotion(pstEnv, pstPlayer);

	// 处理切磋
	player_logout_qiecuo(pstEnv, pstPlayer);

	// 处理组队跟随
	player_logout_cancel_follow(pstEnv, pstPlayer);
	
	// 处理尾行
	pstPlayer->stOnline.stWeiXing.cWeiXingNum = 0;

	// 处理随身仓库
	if (pstPlayer->stOnline.iCallNpcMemID)
	{
		Npc *pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.iCallNpcMemID);
		if (pstNpc)
		{
			z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, &pstNpc->stMon);
			z_free_monster(pstEnv->pstAppCtx, pstEnv, &pstNpc->stMon);
		}
	}

	//处理摆摊
	if (CS_STAT_STALL & pstPlayer->stOnline.State)
	{
		player_stall_clear(pstEnv, pstPlayer);
	}

	world_city_master_notify(pstEnv, pstPlayer, 0);
	z_status_offline(pstEnv->pstAppCtx,pstEnv,pstPlayer);
	player_vip_bar_buff(pstEnv,pstPlayer,FAKE_STATUS_OP_DEL);

	// 队伍跟随下线取消
	player_team_follow_logout(pstEnv, pstPlayer);
	subject_player_logout(pstEnv, pstPlayer);
//	pstPlayer->stRoleData.MiscInfo.MapTimeLimit.EndTime = pstPlayer->stOnline.stMapOL.iEndTime;

	player_reg_delete(pstEnv, pstPlayer);
	school_role_logout(pstEnv, pstPlayer);

	//处理时空深渊副本
	shikong_exit_pworld_queue(pstEnv, pstPlayer, pstPlayer->stOnline.stShiKongQueueInfo.iPworldID, 1);
	
	z_player_leave_map(pstEnv->pstAppCtx, pstEnv, pstPlayer, 2);
	
	return 0;
}

int z_db_role_logout_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	UinCache *pstUinCache;
	int iValid;
	ROLETRUSTDATA stTrustHash;
	NameMemID stNameMemID;
	
	if( !(INST_MODE_BATTLE_SVR == pstEnv->iInstMode &&
		 	pstPlayer->stOnline.eStatusAssistant&PLAYER_STATUS_BATTLE_GOTO_ZONESVR)  )
	{
		STRNCPY(stNameMemID.szName, pstPlayer->stRoleData.RoleName, sizeof(stNameMemID.szName));
		if (NULL == sht_remove(pstEnv->pstShtNameMemID, &stNameMemID, NameMemIDHashCmp, NameMemIDHashCode))
		{
			/*tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
				"sht_remove %d player fail, status = %d, reason = %d",
				pstPlayer->stRoleData.Uin,
				pstPlayer->eStatus,
				pstPlayer->ucRoleLogoutReason );*/
		}
	}

	pstPlayer->eStatus = PLAYER_STATUS_ROLE_LOGOUT;
	tlog_info(pstEnv->pstLogCat, 0, 0, "Status=RoleLogOut Uin=%d AccName=%s RoleName=%s", 
			pstPlayer->stRoleData.Uin, pstPlayer->szAccount, pstPlayer->stRoleData.RoleName);

	//删除委托挂单信息
	stTrustHash.RoleID = pstPlayer->stRoleData.RoleID;
	sht_remove(pstEnv->pstShtTrust,&stTrustHash,TrustHashCmp, TrustHashCode);

	//宠物buff状态
	//pet_statust_time_update(pstEnv, pstPlayer, 1);

	//更新cache地图ID
	z_role_cache_update(pstEnv, pstPlayer);

	// 处理OOXX的退出处理
	if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
	{
		wedding_pursue_logout(pstEnv, pstPlayer);
		relation_role_logout(pstEnv, pstPlayer);
		mail_role_logout( pstEnv, pstPlayer );
		clan_role_logout( pstEnv, pstPlayer );
		player_logout_save_watch_cache(pstEnv, pstPlayer);

		// update accountrole 
		z_account_db_role_logout(pstEnv, pstPlayer);

		z_account_db_update_lastlogout(pstEnv,pstPlayer);

		role_online_to_mud(pstEnv, pstPlayer, ROLE_OFFLINE);

		z_role_snapshot_oplog(pstAppCtx, pstEnv, pstPlayer);

		z_rolelogout_oplog(pstAppCtx, pstEnv, pstPlayer);

		if (ROLE_LOGOUT_KICK == pstPlayer->ucRoleLogoutReason)
		{
			//info tconnd stop

			if (0 == pstPlayer->stOnline.bConnLost)
			{
				pstEnv->stFrameHeadTemp.Cmd = TFRAMEHEAD_CMD_STOP;
				pstEnv->stFrameHeadTemp.CmdData.Stop.Reason = STOP_REASON_KICKOFF;
				pstEnv->stFrameHeadTemp.ID = z_get_player_connectid(pstEnv,pstPlayer);//pstPlayer->stConnectInfo.iID;
				pstEnv->stFrameHeadTemp.ConnIdx = pstPlayer->stConnectInfo.iConnIdx;

				//tlog_info(pstEnv->pstLogCat, 0, 0, "TFRAMEHEAD_CMD_STOP ConnIdx:%d",pstEnv->stFrameHeadTemp.ConnIdx);
				tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iZoneConnID, pstEnv->szSendBuffer, 0, &pstEnv->stFrameHeadTemp);

				//恢复全局变量的初始设置
				pstEnv->stFrameHeadTemp.CmdData.Stop.Reason = 0;
			}

			z_free_player(pstAppCtx, pstEnv, pstPlayer);
		}
		else if (ROLE_LOGOUT_ACC == pstPlayer->ucRoleLogoutReason)
		{
			player_acc_logout_res(pstAppCtx,pstEnv,pstPlayer);
		}
		else if (ROLE_LOGOUT_CLOSE == pstPlayer->ucRoleLogoutReason)
		{
			z_free_player(pstAppCtx, pstEnv, pstPlayer);
		}
		else if (ROLE_LOGOUT_CLTREQ == pstPlayer->ucRoleLogoutReason || ROLE_LOGOUT_GRAB == pstPlayer->ucRoleLogoutReason)
		{
			pstUinCache = (UinCache *)sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos, &iValid);
			if (NULL == pstUinCache || 0 == iValid || pstUinCache->iUin != pstPlayer->stRoleData.Uin)
			{
				tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
							"z_db_role_logout_res Uin=%d cache",  pstPlayer->stRoleData.Uin);
				return -1;
			}
							
			if (pstPlayer->stOnline.bWaitRoleLogout || pstPlayer->stOnline.stSpanOnline.iStat == SPAN_STAT_CONNECT)
			{
				pstPlayer->stOnline.bWaitRoleLogout = 0;

				pstPlayer->eStatus = PLAYER_STATUS_ROLE_LIST;
				tlog_info(pstEnv->pstLogCat, 0, 0, "Status=RoleList Uin=%d AccName=%s RoleName=%s", 
						pstPlayer->stRoleData.Uin, pstPlayer->szAccount, pstPlayer->stRoleData.RoleName);
				
				if (ROLE_LOGOUT_CLTREQ == pstPlayer->ucRoleLogoutReason)
				{
					z_send_rolelist(pstEnv, pstPlayer, pstUinCache);
				}
			}

			pstPlayer->ucRoleLogoutReason = 0;
		}
		else
		{
			;
		}
	}
	
	return 0;
}

int z_db_set_asyn(ASYNREQ * pstAsynReq, ChgRoleName *pstChg)
{
	
	ZONECHGROLENAMERES stRes;
	STRNCPY(stRes.ChgRoleName, pstChg->szName, sizeof(stRes.ChgRoleName));
	
	pstAsynReq->Cmd = pstChg->Cmd;
	pstAsynReq->ConnIdx = pstChg->ConnIdx;
	pstAsynReq->IPInfo = pstChg->IPInfo;
	pstAsynReq->DataLen = sizeof(stRes);
	memcpy(pstAsynReq->Data, &stRes, pstAsynReq->DataLen);
	return 0;
}


int z_db_chg_name_updata_roledata(ZONESVRENV* pstEnv, ChgRoleName *pstChg)
{
	DBAsyn stDBAsyn;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	
	CHGROLEDATA stChgRoleData;

	stChgRoleData.RoleID = pstChg->RoleID;
	stChgRoleData.Uin = pstChg->Uin;
	STRNCPY(stChgRoleData.RoleName, pstChg->szName, sizeof(stChgRoleData.RoleName));
	

	stDBAsyn.iCmd = DB_CS_CHG_NAME_UP_ROLEDATA;
	stDBAsyn.AsynCmd.stChgRoleName = *pstChg;
	
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, "ChgRoleData", 
						pstChg->Uin, &stDBAsyn);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stChgRoleData;
	stMetaData.iBuff = sizeof(stChgRoleData);
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}
	
	return 0;
}

int chg_name_up_clan(ZONESVRENV* pstEnv, ChgRoleName *pstChg)
{
	int i,j;
	ZoneClan *pstClan = NULL;
	ClanObj *pstClanObj = pstEnv->pstClanObj;

	for(i = 0; i < pstClanObj->iClanNum; i++)
	{
		pstClan = &pstClanObj->astZoneClan[i];
		for ( j=0; j<pstClan->stClan.ClanProf.MemNum; j++)
		{
			if (pstClan->stClan.ClanCore.ClanMem[j].RoleMember.RoleID == (tdr_ulonglong)pstChg->RoleID)
			{
				break;
			}
		}

		if (j < pstClan->stClan.ClanProf.MemNum)
		{
			break;
		}
		pstClan = NULL;
	}

	if (!pstClan || 
		i>=pstClanObj->iClanNum)
	{
		return 0;
	}	

	
	if (strcmp(pstClan->stClan.ClanBrief.Leader.RoleName, pstChg->szOldName) == 0)
	{
		strncpy(pstClan->stClan.ClanBrief.Leader.RoleName, pstChg->szName, 
			sizeof(pstClan->stClan.ClanBrief.Leader.RoleName));
	}

	for (i=0; i<pstClan->stClan.ClanProf.MemNum; i++)
	{
		if (strcmp(pstClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName, pstChg->szOldName) == 0)
		{
			strncpy(pstClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName, pstChg->szName, 
				sizeof(pstClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName));
			
		}
	}

	for (i=0; i<pstClan->stClan.ClanBrief.AssistNum; i++)
	{
		if (strcmp(pstClan->stClan.ClanBrief.Assists[i].RoleName, pstChg->szOldName)  == 0)
		{
			strncpy(pstClan->stClan.ClanBrief.Assists[i].RoleName,pstChg->szName, 
				sizeof(pstClan->stClan.ClanBrief.Assists[i].RoleName));
		}
	}

	// 写回数据库
	clan_set_dirty_notify(pstClan, CACHE_FLAG_IMM);

	return 0;
}

int z_db_chg_name_update_roledata_res(ZONESVRENV* pstEnv, TORMPKG *pstPkg,DBAsyn *pstDBAsyn)
{
	ACCOUNTROLENAME stAccRoleName;
	ASYNREQ stAsynReq;
	ChgRoleName *pstChg = &pstDBAsyn->AsynCmd.stChgRoleName;
	//ROLEDROPNAME stRoleDropName;
	
	if (TORM_ERR_IS_ERROR(pstPkg->Body.UpdateRes.ErrNo))
	{
		return -1;
	}

	player_mail_chgname(pstEnv, pstChg->szOldName, pstChg->szName);
	// 更新公会
	chg_name_up_clan(pstEnv, pstChg);
	chg_rolename_update_relation(pstEnv, pstChg->RoleID,  pstChg->szName);
	chg_name_up_sht(pstEnv,pstChg->Uin,pstChg->RoleID, pstChg->szName, pstChg->szOldName,-1);

	z_db_set_asyn(&stAsynReq, &pstDBAsyn->AsynCmd.stChgRoleName);
	z_msg_to_cs(pstEnv, &stAsynReq);

	// 全服广播
	chg_role_name_notify(pstEnv,  pstChg->RoleID, pstChg->szName, pstChg->szOldName);

	// 更新AccountRole
	stAccRoleName.RoleWID = pstChg->RoleID;
	STRNCPY(stAccRoleName.RoleName, pstChg->szName, 
		      CCH(stAccRoleName.RoleName));

	chg_role_name_up_db(pstEnv, pstChg->Uin, pstChg->RoleID,
							pstChg->szOldName, pstChg->szName,	&stAccRoleName);
	/*z_account_db_update_name(pstEnv, pstChg->Uin, &stAccRoleName);
	z_db_role_drop_update_name(pstEnv, pstChg->RoleID, pstChg->szName);

	// 更新ShadowRoleData
	z_db_shadowroledata_up_name(pstEnv, pstChg->RoleID, pstChg->szName);
	// 更新TopRange
	range_up_role_name(pstEnv, pstChg->RoleID, pstChg->szName);*/

	return 0;
}


int z_err_res_cs(ZONESVRENV* pstEnv, int iConnIdx, TIPINFO *pstIpInfo, int iErrReason)
{
	int iErr;
	int iSrcID;
	int iDstID;
	TDRDATA szNet;
	char szBuff[sizeof(SSPKG)];
	SSPKG stPkgRes;
	ASYNREQ *pstAsyn = &stPkgRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq;      

	ERRRES stErrRes;
	stErrRes.ErrReason = iErrReason;

	stPkgRes.Head.Cmd = CS_CTRL;
	stPkgRes.Head.Magic = PROTOCOL_MAGIC;
	stPkgRes.Head.Ver = TDR_METALIB_PROTO_SS_VERSION;
	stPkgRes.Head.HeadLen = sizeof(stPkgRes.Head);
	stPkgRes.Body.CsCtrL.Cmd = ZONE_RES;
	stPkgRes.Body.CsCtrL.CsCmd.ZoneRes.Result = 0;

	pstAsyn->Cmd = ERR_RES;
	pstAsyn->ConnIdx = iConnIdx;
	pstAsyn->IPInfo = *pstIpInfo;
	pstAsyn->DataLen = sizeof(stErrRes);
	memcpy(&pstAsyn->Data, &stErrRes, sizeof(stErrRes));

	szNet.iBuff = sizeof(szBuff);
	szNet.pszBuff = szBuff;

	iErr = ss_pack(pstEnv->pstSSHan, &szNet, &stPkgRes, 0);
	if(iErr < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail, reason : %s",
			         tdr_error_string(iErr));
		return -1;
	}

	iSrcID = pstEnv->pstAppCtx->iId;
	iDstID = pstEnv->iCsSvrID;
	if (0 > tbus_send(pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, szNet.pszBuff, szNet.iBuff, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail");
		return -1;
	}

	return 0;
}

int z_db_chg_name_up_nameid_res(ZONESVRENV* pstEnv, Player *pstPlayer, TORMPKG *pstPkg,DBAsyn *pstDBAsyn)
{

	ASYNREQ stAsynReq;
	ChgRoleName *pstChg = &pstDBAsyn->AsynCmd.stChgRoleName;

	if (TORM_ERR_IS_ERROR(pstPkg->Body.UpdateRes.ErrNo))
	{
		if (TORM_ERR_MAKE_SVR_ERROR(TORM_ERROR_QUERY_DUPKEY) == (unsigned int)pstPkg->Body.UpdateRes.ErrNo)
		{
			if(pstPlayer && pstChg->Cmd == 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_ROLENAME_DUP);
			}

			// 給cs_svr 返回重名错误码
			if (pstChg->Cmd > 0)
			{
				z_err_res_cs(pstEnv, pstChg->ConnIdx,&pstChg->IPInfo, ERR_DUP);
			}
		}
		else
		{
			if(pstPlayer && pstChg->Cmd == 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BUSY);
			}

			if (pstChg->Cmd > 0)
			{
				z_err_res_cs(pstEnv, pstChg->ConnIdx,&pstChg->IPInfo, ERR_SYS);
			}
		}
		return -1;
	}

	// 更新完之后插入一条原名字的记录
	if (chg_name_insert_nameid(pstEnv,pstChg->RoleID, pstChg->Uin, pstChg->szOldName) < 0)
	{
		//return -1;
	}
	
	if (pstChg->Cmd > 0) 	// 来自cs_svr
	{
		if (pstPlayer && strcmp(pstPlayer->stRoleData.RoleName, pstChg->szOldName) == 0 &&
			PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus)
		{
			if (chg_name_update_res(pstEnv, pstPlayer, NULL, pstChg->szName) >= 0 )
			{
				z_db_set_asyn(&stAsynReq, &pstDBAsyn->AsynCmd.stChgRoleName);
				z_msg_to_cs(pstEnv, &stAsynReq);
			}
		}
		else
		{
			// 异步更新roledata
			z_db_chg_name_updata_roledata(pstEnv, pstChg);
		}
	}
	else					// ui 界面
	{
		if (!pstPlayer ||PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
		{
			return -1;	
		}

		return chg_name_update_res(pstEnv, pstPlayer, NULL, pstChg->szName);
	}
	return 0;
}

int z_cs_del_role_delete(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,DBAsyn *pstDbsyn,ROLEDATA *pstRoleData)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMDELETEREQ *pstDeleteReq = &pstPkg->Body.DeleteReq;
	ROLEDATA stRoleData;
	TDRDATA stNetData;
	TDRDATA stMetaData;

	pstDbsyn->iCmd = DB_CS_DEL_ROLE_DELETE;
	pstDbsyn->AsynCmd.stCsDelRole.RoleID = pstRoleData->RoleID;
	pstDbsyn->AsynCmd.stCsDelRole.Uin = pstRoleData->Uin;
	z_init_torm_head(pstAppCtx, pstEnv, &pstPkg->Head, TORM_DELETE_REQ, "RoleData", pstRoleData->Uin, pstDbsyn);
	pstDeleteReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDeleteReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDeleteReq->OperInfo.WhereDef[0] = 0;
		
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRoleData;
	stMetaData.iBuff = sizeof(stRoleData);
	
	memset(&stRoleData, 0, sizeof(stRoleData));
	stRoleData.RoleID = pstRoleData->RoleID;
	stRoleData.Uin = pstRoleData->Uin;
		
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}

	//z_account_db_del(pstEnv, pstRoleData->Uin, pstRoleData->RoleID);

	return 0;

}



int z_cs_del_role_select_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,TORMSELECTRES *pstSelectRes,DBAsyn *pstDbsyn)
{
	ROLEDATA stRoleData;
	Player stPlayer;
	ROLEDELINFO stRoleDel;
	

	if (pstSelectRes->SelectResult.ResultNum != 1)
	{
		return -1;
	}

	if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stRoleData, sizeof(stRoleData)))
	{
		return -1;
	}

	if (strcmp(stRoleData.RoleName, pstDbsyn->AsynCmd.stCsDelRole.szName)!= 0)
	{
		return -1;
	}
	pstDbsyn->AsynCmd.stCsDelRole.Uin = stPlayer.stRoleData.Uin = stRoleData.Uin;
	pstDbsyn->AsynCmd.stCsDelRole.RoleID = stPlayer.stRoleData.RoleID = stRoleData.RoleID;

	// 小于10 直接删除
	if (stRoleData.Level < DELETE_ROLE_LEVEL_LIMIT)
	{
		z_db_role_del_name_req(pstEnv, NULL,  stRoleData.RoleID);
		z_cs_del_role_delete(pstAppCtx, pstEnv,pstDbsyn,&stRoleData);
		z_account_db_del(pstEnv, stRoleData.Uin, stRoleData.RoleID);
	}
	else
	{
		stRoleDel.RoleID = stRoleData.RoleID;
		stRoleDel.DelTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		stRoleDel.CtrlFlag = ROLE_DEL_FLAG;
		z_cs_del_role_update( pstEnv, &stRoleDel ,pstDbsyn);
		z_account_db_update_deltime(pstEnv, &stPlayer, stRoleDel.RoleID, stRoleDel.DelTime);
	}

	return 0;
}

int z_del_role_res_cs(ZONESVRENV* pstEnv,DBAsyn *pstDbAsyn)
{
	int iSrcID;
	int iDstID;
	TDRDATA szNet;
	char szBuff[sizeof(SSPKG)];
	SSPKG stPkgRes;
	ASYNREQ *pstAsyn = &stPkgRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq;
	ZONEDELROLEREQ stDelRoleReq;

	stDelRoleReq.RoleID = pstDbAsyn->AsynCmd.stCsDelRole.RoleID;
	stDelRoleReq.Uin = pstDbAsyn->AsynCmd.stCsDelRole.Uin;
	strcpy(stDelRoleReq.RoleName, pstDbAsyn->AsynCmd.stCsDelRole.szName);
	pstAsyn->Cmd = DEL_ROLE_REQ;
	pstAsyn->ConnIdx = pstDbAsyn->AsynCmd.stCsDelRole.ConnIdx;
	pstAsyn->IPInfo = pstDbAsyn->AsynCmd.stCsDelRole.IPInfo;
	memcpy(pstAsyn->Data, &stDelRoleReq, sizeof(stDelRoleReq));
	pstAsyn->DataLen = sizeof(stDelRoleReq);
	
	stPkgRes.Head.Cmd = CS_CTRL;
	stPkgRes.Head.Magic = PROTOCOL_MAGIC;
	stPkgRes.Head.Ver = TDR_METALIB_PROTO_SS_VERSION;
	stPkgRes.Head.HeadLen = sizeof(stPkgRes.Head);
	stPkgRes.Body.CsCtrL.Cmd = ZONE_RES;
	//memcpy(&stPkgRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq,&stAsyn,sizeof(stAsyn));
	stPkgRes.Body.CsCtrL.CsCmd.ZoneRes.Result = 1;
	
	szNet.iBuff = sizeof(szBuff);
	szNet.pszBuff = szBuff;

	if (0 > ss_pack(pstEnv->pstSSHan, &szNet, &stPkgRes,  0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -1;
	}

	iSrcID = pstEnv->pstAppCtx->iId;
	iDstID = pstEnv->iCsSvrID;
	if (0 > tbus_send( pstEnv->pstAppCtx->iBus, &iSrcID, &iDstID, szNet.pszBuff, szNet.iBuff, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail");
		return -1;
	}
	
	return 0;

}


int cs_del_role_up_sht(ZONESVRENV* pstEnv,int iUin,tdr_ulonglong RoleID, char *pszName)
{
	int i;
	UinCache *pstUinCache;
	UinCache stUinHashData;
	NameMemID stNameMemID;
//	NameMemID *pstNameMemID;
	
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
			pstUinCache->ucRoleNum --;
			if (i != pstUinCache->ucRoleNum)
			{
				memcpy(&pstUinCache->astRoleListInfo[i], &pstUinCache->astRoleListInfo[pstUinCache->ucRoleNum],
						sizeof(pstUinCache->astRoleListInfo[0]));
			}
			break;
		}
	}

	STRNCPY(stNameMemID.szName, pszName, sizeof(stNameMemID.szName));
	sht_remove(pstEnv->pstShtNameMemID, &stNameMemID, NameMemIDHashCmp, NameMemIDHashCode);

	return 0;
}

int z_cs_role_delete_res(ZONESVRENV* pstEnv,  TORMPKG *pstTormPkg, DBAsyn *pstDbAsyn)
{
	
	TORMDELETERES *pstDeleteRes = &pstTormPkg->Body.DeleteRes;
	NAMEIDTYPE stNameIDType;
	Player *pstPlayer;
//	ZONEDELROLEREQ stDelRole;


	if (TORM_ERR_IS_ERROR(pstDeleteRes->ErrNo))
	{
		return -1;
	}
	else
	{
		stNameIDType.GID = pstDbAsyn->AsynCmd.stCsDelRole.RoleID;
		stNameIDType.NameType = 64 | ROLE_NAME;
		z_db_name_id_update( pstEnv, &stNameIDType );
		//z_role_del_oplog(pstEnv, pstPlayer, &stOpRoleDel);

		
		z_del_role_res_cs(pstEnv,pstDbAsyn);
		cs_del_role_up_sht(pstEnv,pstDbAsyn->AsynCmd.stCsDelRole.Uin,
							pstDbAsyn->AsynCmd.stCsDelRole.RoleID, pstDbAsyn->AsynCmd.stCsDelRole.szName);

		pstPlayer = player_get_by_uin(pstEnv, pstDbAsyn->AsynCmd.stCsDelRole.Uin);
		if (pstPlayer && pstPlayer->eStatus == PLAYER_STATUS_ROLE_LIST)
		{
			CSPKG *pstPkg = &pstEnv->stCSRes;
			CSROLEDELRES *pstDelRes = &pstPkg->Body.RoleDelRes;

			pstDelRes->RoleID = pstPlayer->stRoleData.RoleID;
			pstDelRes->Result = 0;
			Z_CSHEAD_INIT(&pstPkg->Head, ROLE_DEL_RES);
			z_cltmsg_send(pstEnv, pstPlayer, pstPkg);
		}

	}
	return 0;
}

int z_cs_del_role_up_res(ZONESVRENV* pstEnv,  TORMPKG *pstTormPkg, DBAsyn *pstDbAsyn)
{
	
	TORMUPDATERES*pstUpRes = &pstTormPkg->Body.UpdateRes;
	NAMEIDTYPE stNameIDType;
	//ZONEDELROLEREQ stDelRole;


	if (TORM_ERR_IS_ERROR(pstUpRes->ErrNo))
	{
		return -1;
	}
	else
	{
		stNameIDType.GID = pstDbAsyn->AsynCmd.stCsDelRole.RoleID;
		stNameIDType.NameType = 64 | ROLE_NAME;
		z_db_name_id_update( pstEnv, &stNameIDType );
		//z_role_del_oplog(pstEnv, pstPlayer, &stOpRoleDel);

		
		z_del_role_res_cs(pstEnv,pstDbAsyn);
		cs_del_role_up_sht(pstEnv,pstDbAsyn->AsynCmd.stCsDelRole.Uin,
							pstDbAsyn->AsynCmd.stCsDelRole.RoleID, pstDbAsyn->AsynCmd.stCsDelRole.szName);
	}
	return 0;
}

int z_db_unfengyin_select_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
									TORMSELECTRES *pstSelectRes)
{
	NAMEID stNameID;
	UnFengYin *pstUnFengYin = &pstPlayer->stOnline.stUnFengYin;
	CSPKG stPkg;
	CSARMSVR  *pstArmSvr = &stPkg.Body.ArmSvr;

	if (	pstUnFengYin->szRoleName[0]!=0 )
	{
		return -1;
	}

	memset(pstArmSvr,0,sizeof(*pstArmSvr));
	if ( pstSelectRes->SelectResult.ResultNum == 1)
	{
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stNameID, sizeof(stNameID)))
		{
			return -1;
		}

		STRNCPY(pstUnFengYin->szRoleName, stNameID.Name, sizeof(pstUnFengYin->szRoleName));
		STRNCPY(pstArmSvr->RoleName, stNameID.Name, sizeof(pstArmSvr->RoleName));
	}
	else if (pstSelectRes->SelectResult.ResultNum == 0)
	{
		// 角色删除了,找不到
	}
	else
	{
		return -1;
	}

	pstArmSvr->ArmSvrType = ARM_UNFENGYIN_PRE;
	pstArmSvr->Flag = 1;
	
	Z_CSHEAD_INIT(&stPkg.Head, ARM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_db_select_account_res(ZONESVRENV* pstEnv, Player *pstPlayer, 
									TORMSELECTRES *pstSelectRes, SelectInfo *pstSelectInfo)
{
	if ( pstSelectRes->SelectResult.ResultNum == 1)
	{
		
		CSPKG stPkg;
		CSACTIONRES *pstAction = &stPkg.Body.ActionRes; 	
		
		memset(pstAction,0,sizeof(*pstAction));
		
		pstAction->ID = pstPlayer->iMemID;
		pstAction->ActionID = ACTION_ACC_DE_QUICK;
		
		pstAction->Data.AccDeQuick.Result = AUTH_IFM_ACCOUNT_EXSIT;
		
		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	else
	{
		z_ss_req_get_account(pstEnv->pstAppCtx,pstEnv,pstSelectInfo);
	}
	return 0;
}


int z_db_msg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, char *pszMsg, int iMsgLen, int iMsgSrc)
{
	TDRDATA stData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	Player *pstPlayer = NULL;
	TORMSELECTRES *pstSelectRes;
	TORMSELECTCOUNTRES *pstSelectCountRes;
	TORMUPDATERES *pstUpdateRes;
	int iUin;
	int iProtoVer=0;
	int iRet;
	DBAsyn stDBAsyn;

	if (iMsgSrc == pstEnv->iRoleDBID)
	{
		iProtoVer = pstEnv->iRoleDBProtoVer;
	}
	else if(iMsgSrc == pstEnv->iMiscDBID)
	{
		iProtoVer = pstEnv->iMiscDBProtoVer;
	}
	else if(iMsgSrc == pstEnv->iAccountDBID)
	{
		iProtoVer = pstEnv->iAccountDBProtoVer;
	}
	
	stData.pszBuff = pszMsg;
	stData.iBuff = iMsgLen;
	iRet =  torm_attach_res_msg(pstEnv->hTorm, &stData, pstPkg, iProtoVer);
	if (0 >iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_attach_res_msg error:%s, iProtoVer=%d", torm_error_string(iRet), iProtoVer);
		return -1;
	}

	iUin = pstPkg->Head.HeadApp.ObjID;
	if (0 <= iUin)
	{
		// 这个 pstPlayer == NULL 的请自己控制下
		pstPlayer = player_get_by_uin(pstEnv, iUin);
	}

	if (pstPkg->Head.AsynCallBackDataLen > 0 && pstPkg->Head.AsynCallBackDataLen <= (int)sizeof(stDBAsyn))
	{
		memcpy(&stDBAsyn, pstPkg->Head.AsynCallBackData, pstPkg->Head.AsynCallBackDataLen);
	}
	else
	{
		stDBAsyn.iCmd = 0;
	}

	tlog_debug(pstEnv->pstLogCat, 0, 0, "recv db msg uin=%d,cmd=%d,isrc=%d", iUin, pstPkg->Head.HeadCmd.Cmd, iMsgSrc);

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
		else if(iMsgSrc == pstEnv->iAccountDBID)
		{
			pstEnv->iAccountDBProtoVer = pstPkg->Body.ExgVerRes.Version;
		}
		else if (iMsgSrc == pstEnv->iRegionDBID)
		{
			pstEnv->iRegionDBProtoVer = pstPkg->Body.ExgVerRes.Version;
		}
		//tlog_debug(pstEnv->pstLogCat, 0, 0, "recv torm exchg ver res, ver=%d", pstEnv->iRoleDBProtoVer);
		break;
	case TORM_SELECT_RES:	
		pstSelectRes = &pstPkg->Body.SelectRes;

		if(stDBAsyn.iCmd == DB_UPDATE_ROLE_LIST)
		{
			return z_db_update_role_list_res(pstAppCtx, pstEnv, pstSelectRes, 
				                               stDBAsyn.AsynCmd.iUin);
		}

		if(stDBAsyn.iCmd == DB_ROLE_DROP_SELECT)
		{
			if(pstPlayer)
			{
				return z_db_role_drop_select_res(pstEnv, pstPlayer, pstSelectRes, 
				                               &stDBAsyn.AsynCmd.stDropObjUpdate);
			}
			
			return -1;
		}

		if (stDBAsyn.iCmd == DB_STORE_SELECT)
		{
			return store_select_res(pstEnv, pstSelectRes);
		}

		if (stDBAsyn.iCmd == DB_HOME_SELECT)
		{
			return home_select_res(pstEnv, pstSelectRes);
		}

		if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
		{
			if (DB_ROLE_SELECT == stDBAsyn.iCmd && pstPlayer && 
				PLAYER_STATUS_ROLE_LOGIN_REQ == pstPlayer->eStatus)
			{
				pstPlayer->eStatus = PLAYER_STATUS_ROLE_LIST;
			}
			
			tlog_error(pstEnv->pstLogCat, iUin, 0, "select res error Uin=%d, cmd=%d, errno=%d, dberrno=%d", 
						iUin,stDBAsyn.iCmd, pstSelectRes->ErrNo, pstSelectRes->DBMSErrNo);
			return -1;
		}

		switch(stDBAsyn.iCmd)
		{
		//case DB_STORE_SELECT:
		//	store_select_res(pstEnv, pstSelectRes);
		//	break;
		case DB_REGION_GLOBAL_SELECT:
			span_global_res(pstEnv, pstSelectRes);
			break;
		case DB_SELECT_WATCH_RES:
			if (pstPlayer && pstPlayer->stRoleData.RoleID == stDBAsyn.AsynCmd.stWatchRole.ullRoleID)
			{
				z_db_watch_select_res(pstAppCtx,  pstEnv, pstPlayer, pstSelectRes, 
										stDBAsyn.AsynCmd.stWatchRole.ullPetWid);	
			}
			break;
		case DB_UNFENGYIN_SELECT_NAMEID:
			if (	!pstPlayer || 
				pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN ||
				stDBAsyn.AsynCmd.ullRoleID != pstPlayer->stRoleData.RoleID)
			{
				return -1;
			}
			z_db_unfengyin_select_res(pstAppCtx, pstEnv, pstPlayer, pstSelectRes);
			
			break;
		case DB_CS_DEL_ROLE_SELECT:
			z_cs_del_role_select_res(pstAppCtx, pstEnv,pstSelectRes,&stDBAsyn);
			break;
		case DB_ROLE_LIST:
			if (pstPlayer && (PLAYER_STATUS_ROLE_LIST_REQ == pstPlayer->eStatus ||
							PLAYER_STATUS_ROLE_LIST == pstPlayer->eStatus))
			{
				z_db_role_list_res(pstAppCtx, pstEnv, pstPlayer, pstSelectRes);
			}
			else
			{
				return -1;
			}
			break;
		case DB_SELECT_PLAYER_ROLE_LIST:
			// pstPlayer有可能是NULL
			if (pstPlayer)
			{
				shopping_db_select_role_res(pstAppCtx,pstEnv, pstPlayer, pstPkg);
			}
			else
			{
				shopping_db_select_role_res(pstAppCtx,pstEnv, NULL, pstPkg);
			}
			break;

		case DB_SELECT_TRADE_SELL_RES:
			shopping_db_tradesell_select_res(pstAppCtx,pstEnv, pstPkg);
			break;	
		case DB_SELECT_TRADE_BUY_RES:
			shopping_db_tradebuy_select_res(pstAppCtx,pstEnv, pstPkg);
			break;	
		case DB_SELECT_SHOPDEF_RES:
			shopping_db_shopdef_select_res(pstAppCtx,pstEnv, pstPkg);
			break;
		case DB_SELECT_SHOPMSGDEF_RES:
			shopping_db_shopmsgdef_select_res(pstAppCtx,pstEnv, pstPkg);
			break;
		case DB_SELECT_SHOPLIMITDEF_RES:
			shopping_db_shoplimitdef_select_res(pstAppCtx,pstEnv, pstPkg);
			break;
		case DB_ROLE_SELECT:
			if (NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN_REQ != pstPlayer->eStatus || 1 != pstSelectRes->SelectResult.ResultNum)
			{
				return -1;
			}
			
			z_db_role_login_res(pstAppCtx, pstEnv, pstPlayer, pstSelectRes);
			
			break;
		case DB_ROLEDETAIL_SELECT_RES:
			if (NULL == pstPlayer || 1 != pstSelectRes->SelectResult.ResultNum)
			{
				return -1;
			}
			
			z_db_get_roledetail_res(pstAppCtx, pstEnv, pstPlayer, pstSelectRes,&stDBAsyn);
			
			break;
		case DB_BUDDY_ASYN_INIT:
			if (NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
			{
				return -1;
			}
			
			relation_init_res(pstEnv, pstPlayer, pstSelectRes);
			break;
		case DB_BUDDY_CHG:
			relation_db_buddy_chg(pstEnv, pstSelectRes, &stDBAsyn.AsynCmd.stBuddyChg);
			break;
		case DB_GLOBAL_SELECT:
			z_global_res(pstEnv, pstSelectRes);
			break;
		case DB_CLAN_SELECT:
			clan_select_res(pstEnv, pstSelectRes);
			break;
		case DB_RANGE_SELECT:
			range_select_res(pstEnv, pstSelectRes);
			break;
		case DB_MAIL_SELECT:
			mail_select_res(pstEnv, pstSelectRes, &stDBAsyn.AsynCmd.stActiveMailboxInfo);
			break;
		case DB_CHG_NAME_SELECT:
			if (NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus )
			{
				return -1;
			}
			chg_name_check_select_res(pstEnv, pstPlayer,pstSelectRes, stDBAsyn.AsynCmd.stChgRoleName.szName);
			break;
		case DB_CHG_CLANNAME_SELECT:
			//chg_clanname_check_select_res(pstEnv, pstPlayer,pstSelectRes, stDBAsyn.AsynCmd.szName);
			break;

		case DB_SHADOW_SELECT_RES:
			if(pstPlayer)
			{
				db_select_shadow_res(pstAppCtx, pstEnv, pstPlayer,pstSelectRes);
			}
			break;
		case DB_SELECT_ACC_SAFEPWD:
			if (NULL == pstPlayer )
			{
				return -1;
			}

			z_acc_safepwd_res(pstEnv, pstPlayer, pstSelectRes,&stDBAsyn);
			break;
			
		case DB_ACC_SHARE_SELECT:
			if (NULL == pstPlayer )
			{
				return -1;
			}
			
			z_db_acc_share_select_res(pstEnv, pstPlayer, pstSelectRes);
			
			break;
		case DB_GMLIMIT_SELECT:
			z_gm_limit_res(pstEnv, pstSelectRes);
			break;

		case DB_SELECT_VIP_BAR_DEF_RES:
			shopping_db_vip_bar_def_select_res(pstAppCtx,pstEnv, pstPkg);
			break;

		case DB_SELECT_VIP_BAR_LEVEL_DEF_RES:
			shopping_db_vip_bar_level_def_select_res(pstAppCtx,pstEnv, pstPkg);
			break;

		case DB_SELECT_VIP_BAR_NUM_DEF_RES:
			shopping_db_vip_bar_num_def_select_res(pstAppCtx,pstEnv, pstPkg);
			break;	
		case DB_SELECT_ACCOUNT_RES:
			if (NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus )
			{
				return -1;
			}
			z_db_select_account_res(pstEnv, pstPlayer,pstSelectRes, &stDBAsyn.AsynCmd.stSelectInfo);
			break;
		case DB_ROLE_SECOND_PWD_SELECT:
			if(NULL == pstPlayer)
			{
				return -1;
			}
			z_role_secondpwd_res(pstAppCtx,pstEnv, pstPlayer, pstSelectRes,&stDBAsyn);
			break;
		case DB_ROLE_BASE_SELECT:
			if (NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus )
			{
				return -1;
			}

			role_base_db_res(pstEnv, pstPlayer, pstSelectRes, &stDBAsyn);
			break;
		case DB_ROLE_SYNC_RELATION_SELECT:
			role_sync_relation_select_res(pstEnv, pstSelectRes, &stDBAsyn);
			break;
		case DB_ROLE_MISC_INFO_SELECT:
			z_db_misc_info_res(pstEnv, pstSelectRes, &stDBAsyn);
			break;
		case DB_ROLE_BUDDY_SELECT:
			z_db_role_buddy_info_res(pstEnv, pstSelectRes, &stDBAsyn);
			break;
		case DB_ROLE_SKILL_INFO_SELECT:
			z_db_role_skill_info_res(pstEnv, pstSelectRes, &stDBAsyn);
			break;
		default:
			return -1;
			break;
		}

		break;
	case TORM_UPDATE_RES:
		switch( stDBAsyn.iCmd )
		{
		case DB_HOME_UPDATE_RES:
			home_update_res(pstEnv, pstPkg, &stDBAsyn);
			break;
		case DB_STORE_UPDATE_RES:
			store_update_res(pstEnv, pstPkg, &stDBAsyn);
			break;
		case DB_CHG_NAME_UP:
			if(pstPlayer)
			{
				z_db_chg_name_up_nameid_res(pstEnv, pstPlayer, pstPkg,&stDBAsyn);
			}
			else
			{
				z_db_chg_name_up_nameid_res(pstEnv, NULL, pstPkg,&stDBAsyn);
			}
			break;
			
		case DB_CS_DEL_ROLE_UPDATE:
			z_cs_del_role_up_res(pstEnv,  pstPkg, &stDBAsyn);
			break;
		case DB_ROLE_UPDATE:
			{
				pstUpdateRes = &pstPkg->Body.UpdateRes;
				if (NULL == pstPlayer || 
					(PLAYER_STATUS_ROLE_LOGOUT_REQ != pstPlayer->eStatus && 
					  PLAYER_STATUS_ROLE_LOGOUT_BATTLE != pstPlayer->eStatus ) ||
					TORM_ERR_IS_ERROR(pstUpdateRes->ErrNo))
				{
					tlog_error(pstEnv->pstLogCat, iUin, 0, "Uin %d player status=%d update errno=%d, DBErrno=%d",
								iUin, pstPlayer?pstPlayer->eStatus:-1, pstUpdateRes->ErrNo, pstUpdateRes->DBMSErrNo);
					return -1;
				}
						
				z_db_role_logout_res(pstAppCtx, pstEnv, pstPlayer);
			}
			break;

		case DB_GLOBAL_UPDATE:
			z_global_update_res( pstAppCtx, pstEnv, pstPkg );
			break;

		case DB_CLAN_UPDATE:
			clan_update_res( pstAppCtx, pstEnv, pstPkg, stDBAsyn.AsynCmd.nClanID );
			break;

		case DB_RANGE_UPDATE:
			range_update_res(pstEnv, pstPkg, &stDBAsyn);
			break;

		case DB_MAIL_UPDATE:
			mail_update_res( pstAppCtx, pstEnv, pstPkg, &stDBAsyn.AsynCmd.stActiveMailboxInfo );
			break;
		
		case DB_CS_CHG_NAME_UP_ROLEDATA:
			z_db_chg_name_update_roledata_res(pstEnv, pstPkg,&stDBAsyn);
			break;

		case DB_ROLE_DELETE_RESPITE:
			if (NULL == pstPlayer)
			{
				tlog_error(pstEnv->pstLogCat, iUin, 0, "Uin %d delete res player null", iUin);
				return -1;
			}
			z_db_delete_respite_res(pstAppCtx, pstEnv, pstPlayer, pstPkg, 
										stDBAsyn.AsynCmd.ullRoleID, 0);
			break;
		case DB_ROLE_DELETE_REALLY:
			if (pstPlayer)
			{
				z_db_delete_respite_res(pstAppCtx, pstEnv, pstPlayer, 
										pstPkg, stDBAsyn.AsynCmd.ullRoleID, 1);
			}
			break;
		case DB_ROLE_UPDATE_UNDEL:
			if (pstPlayer)
			{
				z_db_role_undel_res(pstAppCtx, pstEnv, pstPlayer, 
								pstPkg, stDBAsyn.AsynCmd.ullRoleID);
			}
			break;
		case DB_UPDATE_TRADE_SELL_RES:
			if (pstPlayer)
			{
				
			}
			break;
		case DB_UPDATE_TRADE_BUY_RES:
			if (pstPlayer)
			{
				
			}
			break;	
		case DB_UPDATE_SHOPLIMITDEF_RES:
			shopping_db_shoplimitdef_update_res( pstAppCtx,  pstEnv,pstPkg);
			break;
		case DB_ROLEDETAIL_UPDATE_RES:
			pstUpdateRes = &pstPkg->Body.UpdateRes;
			if(pstUpdateRes && pstPlayer)
			{
				z_db_set_roledetail_res(pstEnv, pstPlayer, pstUpdateRes, &stDBAsyn);
			}
			break;
		
		default:
			return -1;
			break;
		}
		
		break;	
	case TORM_INSERT_RES:
		
		switch(stDBAsyn.iCmd )
		{
		case DB_HOME_INSERT_RES:
			home_db_insert_res(pstEnv, pstPlayer,&stDBAsyn, &pstPkg->Body.InsertRes);
			break;
		case DB_STORE_INSERT_RES:
			store_db_insert_res(pstEnv, pstPlayer,&stDBAsyn, &pstPkg->Body.InsertRes);
			break;
		case DB_INSERT_TRADE_SELL_RES:
			if(pstPlayer)
			{
				shopping_db_tradesell_insert_res(pstAppCtx,pstEnv, pstPlayer, pstPkg);
			}
			else
			{
				shopping_db_tradesell_insert_res(pstAppCtx,pstEnv, NULL, pstPkg);
			}
			break;	
		case DB_INSERT_TRADE_BUY_RES:
			if(pstPlayer)
			{
				shopping_db_tradebuy_insert_res(pstAppCtx,pstEnv, pstPlayer, pstPkg);
			}
			else
			{
				shopping_db_tradebuy_insert_res(pstAppCtx,pstEnv, NULL, pstPkg);
			}
			break;	
		case  DB_CHECK_NAME:
			if (NULL == pstPlayer)
			{
				tlog_error(pstEnv->pstLogCat, iUin, 0, "Uin %d insert res player null", iUin);
				return -1;
			}
			
			{
				if (pstPlayer->stOnline.cCheckName)
				{
					z_db_checkname_res(pstAppCtx, pstEnv, pstPlayer, pstPkg, pstPlayer->stOnline.cCheckName);
					pstPlayer->stOnline.cCheckName = 0;
				}
				else
				{
					return -1;
				}
			}
			break;
		case DB_ROLE_INSERT:
			if (NULL == pstPlayer)
			{
				tlog_error(pstEnv->pstLogCat, iUin, 0, "Uin %d insert res player null", iUin);
				return -1;
			}
			
			{
				if (PLAYER_STATUS_ROLE_LIST != pstPlayer->eStatus)
				{
					return -1;
				}
				
				z_db_role_insert_res(pstAppCtx, pstEnv, pstPlayer, pstPkg, &stDBAsyn.AsynCmd.stRoleListInfo);
			}
			break;
		case DB_ACC_SHARE_INSERT:
			if (NULL == pstPlayer)
			{
				tlog_error(pstEnv->pstLogCat, iUin, 0, "Uin %d insert res player null", iUin);
				return -1;
			}

			z_db_acc_share_insert_res(pstAppCtx,pstEnv,pstPlayer, pstPkg);
			break;
		default:
			break;
		}
	case TORM_DELETE_RES:
		
		switch( stDBAsyn.iCmd )
		{
		case DB_HOME_DELETE_RES:
			home_db_delete_res(pstEnv,&stDBAsyn, &pstPkg->Body.DeleteRes);
			break;
		case DB_STORE_DELETE_RES:
			store_db_delete_res( pstEnv,&stDBAsyn, &pstPkg->Body.DeleteRes);
			break;
		case DB_CS_DEL_ROLE_DELETE:
			return z_cs_role_delete_res(pstEnv,  pstPkg, &stDBAsyn);
			break;
		case DB_ROLE_DELETE:
			if (NULL == pstPlayer)
			{
				tlog_error(pstEnv->pstLogCat, iUin, 0, "Uin %d delete res player null", iUin);
				return -1;
			}
			z_db_role_delete_res(pstAppCtx, pstEnv, pstPlayer, pstPkg, stDBAsyn.AsynCmd.ullRoleID);
			break;
		case DB_ROLE_DEL_NAMEID_RES:
			if(pstPlayer)
			{
				z_db_role_del_name_res(pstAppCtx, pstEnv, pstPlayer, pstPkg, stDBAsyn.AsynCmd.ullRoleID);
			}
			break;
		case DB_CLAN_DELETE:
			clan_delete_res( pstAppCtx, pstEnv, pstPkg, stDBAsyn.AsynCmd.nClanID );
			break;

		case DB_MAIL_DELETE:
			mail_delete_res( pstAppCtx, pstEnv, pstPkg, &stDBAsyn.AsynCmd.stActiveMailboxInfo );
			break;
		case DB_DELETE_TRADE_SELL_RES:
			if(pstPlayer)
			{
				shopping_db_tradesell_delete_res(pstAppCtx,pstEnv, pstPlayer, pstPkg);
			}
			else
			{
				shopping_db_tradesell_delete_res(pstAppCtx,pstEnv, NULL, pstPkg);
			}
			break;	
		case DB_DELETE_TRADE_BUY_RES:
			if(pstPlayer)
			{
				shopping_db_tradebuy_delete_res(pstAppCtx,pstEnv, pstPlayer, pstPkg);
			}
			else
			{
				shopping_db_tradebuy_delete_res(pstAppCtx,pstEnv, NULL, pstPkg);
			}
			break;	
		case DB_RANGE_DELETE:
			range_delete_res(pstEnv, pstPkg);
			break;

		default:
			return -1;
			break;
		}
		
		break;

	case TORM_SELECT_COUNT_RES:
		
		switch( stDBAsyn.iCmd )
		{
		case DB_HOME_COUNT:
			home_db_count_res(pstEnv, &pstPkg->Body.SelectCountRes);
			break;
		case DB_STORE_COUNT:
			store_db_count_res(pstEnv, &pstPkg->Body.SelectCountRes);
			break;
		case DB_CLAN_COUNT_SELECT:
			pstSelectCountRes = &pstPkg->Body.SelectCountRes;

			if (TORM_ERR_IS_ERROR(pstSelectCountRes->ErrNo))
			{
				tlog_error(pstEnv->pstLogCat, iUin, 0, "clan select count res error errno=%d", 
							pstSelectCountRes->ErrNo);
				return -1;
			}
			
			clan_count_res(pstEnv, pstSelectCountRes);
			break;
			
		case DB_RANGE_COUNT_SELECT:
			pstSelectCountRes = &pstPkg->Body.SelectCountRes;

			if (TORM_ERR_IS_ERROR(pstSelectCountRes->ErrNo))
			{
				tlog_error(pstEnv->pstLogCat, iUin, 0, "range select count res error errno=%d", 
							pstSelectCountRes->ErrNo);
				return -1;
			}
			
			range_count_res(pstEnv, pstSelectCountRes);
			break;

		case DB_MAIL_ROLE_COUNT_SELECT:
			mail_role_count_res( pstEnv, &pstPkg->Body.SelectCountRes, &stDBAsyn.AsynCmd.stActiveMailboxInfo );
			break;
		case DB_SELECT_COUNT_TRADE_SELL_RES:
			shopping_db_tradesell_count_res(pstAppCtx,pstEnv, pstPkg);
			break;
		case DB_SELECT_COUNT_TRADE_BUY_RES:
			shopping_db_tradebuy_count_res(pstAppCtx,pstEnv, pstPkg);
			break;		
		case DB_SELECT_COUNT_SHOPDEF_RES:
			shopping_db_shopdef_count_res(pstAppCtx,pstEnv, pstPkg);
			break;
		case DB_SELECT_COUNT_SHOPMSGDEF_RES:
			shopping_db_shopmsgdef_count_res(pstAppCtx,pstEnv, pstPkg);
			break;
		case DB_SELECT_COUNT_SHOPLIMITDEF_RES:
			shopping_db_shoplimitdef_count_res(pstAppCtx,pstEnv, pstPkg);
			break;
			
		case DB_SELECT_COUNT_VIP_BAR_DEF_RES:
			shopping_db_vip_bar_def_count_res(pstAppCtx,pstEnv, pstPkg);
			break;
			


		case DB_SELECT_COUNT_VIP_BAR_LEVEL_DEF_RES:
			shopping_db_vip_bar_level_def_count_res(pstAppCtx,pstEnv, pstPkg);
			break;
			

		case DB_SELECT_COUNT_VIP_BAR_NUM_DEF_RES:
			shopping_db_vip_bar_num_def_count_res(pstAppCtx,pstEnv, pstPkg);
			break;
			
	
		default:
			return -1;
			break;
		}
		break;
		
	default:
		return -1;
		break;
	}
	
	return 0;
}

int z_db_role_list(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iRet;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	ROLELISTINFO stRoleListInfo;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;

	// 有可能在rolelist状态删角色
	if (PLAYER_STATUS_ACC_LOGIN != pstPlayer->eStatus &&
		PLAYER_STATUS_ROLE_LIST != pstPlayer->eStatus)
	{
		return -1;
	}

	stDBAsyn.iCmd = DB_ROLE_LIST;
	z_init_torm_head(pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, "RoleListInfo", pstPlayer->stRoleData.Uin, &stDBAsyn);
	
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = ROLE_MAX_NUM;
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;

	// 取没删除  或者删除没有到期的角色
	sprintf(pstSelectReq->OperInfo.WhereDef, "where Uin=%d && CtrlFlag!=%d order by Level DESC,LastLogin DESC", 
										pstPlayer->stRoleData.Uin, ROLE_DEL_FLAG);

	//因为Uin是分表因子，所以这个字段还是要赋值
	stRoleListInfo.Uin = pstPlayer->stRoleData.Uin;
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRoleListInfo;
	stMetaData.iBuff = sizeof(stRoleListInfo);
	if (0 != (iRet = torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iRoleDBProtoVer)))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg err %d", iRet);
		return -1;
	}

	if (0 > z_torm_send(pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}

	if(pstPlayer->eStatus == PLAYER_STATUS_ACC_LOGIN)
	{
		pstPlayer->eStatus = PLAYER_STATUS_ROLE_LIST_REQ;
		tlog_info(pstEnv->pstLogCat, 0, 0, "Status=RoleListReq Uin=%d AccName=%s", 
					pstPlayer->stRoleData.Uin, pstPlayer->szAccount);
	}
	return 0;
}

int z_db_role_list_x(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iUin)
{
	int iRet;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	ROLELISTINFO stRoleListInfo;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;

	stDBAsyn.iCmd = DB_UPDATE_ROLE_LIST;
	stDBAsyn.AsynCmd.iUin = iUin;
	z_init_torm_head(pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, 
		               "RoleListInfo", -1, &stDBAsyn);
	
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = ROLE_MAX_NUM;
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	sprintf(pstSelectReq->OperInfo.WhereDef, "where Uin=%d and DelTime=0", iUin);

	//因为Uin是分表因子，所以这个字段还是要赋值
	stRoleListInfo.Uin = iUin;
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRoleListInfo;
	stMetaData.iBuff = sizeof(stRoleListInfo);
	if (0 != (iRet = torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, 
		                                 &stMetaData, pstEnv->iRoleDBProtoVer)))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg err %d", iRet);
		return -1;
	}

	if (0 > z_torm_send(pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}
	
	return 0;
}

tdr_ulonglong z_get_gid(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	unsigned short unTmp;
	unsigned char cInstance;
	tdr_ulonglong ullGid = 0;

	GET_INSTANCE_ENTITY(cInstance, pstAppCtx->iId);
	memcpy(&ullGid, &pstAppCtx->stCurr.tv_sec, sizeof(int));
	unTmp = (pstEnv->ucRegionID << 10) + (pstEnv->unWorldID << 3) + cInstance;
	memcpy( ((char *)(&ullGid)+sizeof(int)), &unTmp, sizeof(unTmp)); 
	memcpy( ((char *)(&ullGid)+sizeof(ullGid)-sizeof(pstEnv->unGidSeq)), &pstEnv->unGidSeq, sizeof(pstEnv->unGidSeq)); 
	pstEnv->unGidSeq++;
	
	return ullGid;
}

int z_db_role_del_name_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TORMPKG *pstTormPkg, tdr_ulonglong ullRoleID )
{
	CSPKG *pstPkg = &pstEnv->stCSRes;
	CSROLEDELRES *pstDelRes = &pstPkg->Body.RoleDelRes;
	TORMDELETERES *pstDeleteRes = &pstTormPkg->Body.DeleteRes;

	UNUSED( pstAppCtx );

	pstDelRes->RoleID = ullRoleID;

	if (TORM_ERR_IS_ERROR(pstDeleteRes->ErrNo))
	{
		pstDelRes->Result = DEL_ERR;

		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
				"db delete NameID Uin=%d ullRoleID:%llu,errno=%d",
				pstPlayer->stRoleData.Uin,
				ullRoleID,
				pstDeleteRes->ErrNo );
	}
	
	return 0;
}
int z_db_role_del_name_req(ZONESVRENV* pstEnv, Player *pstPlayer,  tdr_ulonglong ullRoleID)
{
	NAMEID stNameID;
	DBAsyn stDBAsyn;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	int iUin = 0;
	
	stDBAsyn.iCmd = DB_ROLE_DEL_NAMEID_RES;
	stDBAsyn.AsynCmd.ullRoleID = ullRoleID;
	if (pstPlayer)
		iUin = pstPlayer->stRoleData.Uin;
	
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_DELETE_REQ, "NameID", 
						iUin, &stDBAsyn);
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
}

int z_db_check_name(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, char *pszName, tdr_ulonglong ullGid, void *pvAsynData, int iAsynData, int iUin, int iNameType)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMINSERTREQ *pstInsertReq = &pstPkg->Body.InsertReq;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	NAMEID stNameID;
		
	z_init_torm_head(pstAppCtx, pstEnv, &pstPkg->Head, TORM_INSERT_REQ, "NameID", iUin, NULL);
	pstInsertReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstInsertReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstInsertReq->OperInfo.WhereDef[0] = 0;
		
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stNameID;
	stMetaData.iBuff = sizeof(stNameID);

	memset(&stNameID, 0, sizeof(stNameID));
	stNameID.GID = ullGid;
	STRNCPY(stNameID.Name, pszName, sizeof(stNameID.Name));
	stNameID.NameType = iNameType;
	stNameID.NewTime = pstAppCtx->stCurr.tv_sec;
	
	//set some info in pkg, in real env, this info should tdr_hton
	assert((int)sizeof(pstPkg->Head.AsynCallBackData) >= iAsynData);
	pstPkg->Head.AsynCallBackDataLen = iAsynData;
	memcpy(pstPkg->Head.AsynCallBackData, pvAsynData, iAsynData);
	
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}

	if (0 > z_torm_send(pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}

	return 0;

}

int z_db_unfengyin_select_nameid(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,
									tdr_ulonglong ullGid,int iUin,tdr_ulonglong ullFind)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;
	NAMEID stNameID;
	
	stDBAsyn.iCmd = DB_UNFENGYIN_SELECT_NAMEID;
	stDBAsyn.AsynCmd.ullRoleID = ullGid;
	
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, "NameID", iUin, &stDBAsyn);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	sprintf(pstSelectReq->OperInfo.WhereDef, "where GID=%llu", ullFind);
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 1;

	memset(&stNameID, 0, sizeof(stNameID));
	stNameID.GID = ullFind;
			
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stNameID;
	stMetaData.iBuff = sizeof(stNameID);

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_db_acc_share_insert(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{

	DBAsyn stDBAsyn;
	AccShareData* pstData = &pstPlayer->stAccShareData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMINSERTREQ *pstInsertReq = &pstPkg->Body.InsertReq;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	

	tlog_info(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
				"z_db_acc_share_insert uin=%d AccShare status=%d", pstPlayer->stRoleData.Uin,
				pstData->iStat);

	pstData->iStat = ACC_SHARE_INS;

	memset(&pstData->stAccShare, 0, sizeof(pstData->stAccShare));
	pstData->stAccShare.LastLogout = pstAppCtx->stCurr.tv_sec;
	pstData->stAccShare.OfflineExp = 0;
	pstData->stAccShare.TakeMudExp = 0;
	pstData->stAccShare.Uin = pstPlayer->stRoleData.Uin;
	pstData->stAccShare.GrabTime = 0;


	stDBAsyn.iCmd = DB_ACC_SHARE_INSERT;
	
	z_init_torm_head(pstAppCtx, pstEnv, &pstPkg->Head, TORM_INSERT_REQ, "AccShare", pstData->stAccShare.Uin, &stDBAsyn);
	pstInsertReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstInsertReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstInsertReq->OperInfo.WhereDef[0] = 0;
		
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&pstData->stAccShare;
	stMetaData.iBuff = sizeof(pstData->stAccShare);
	
	
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}

	if (0 > z_torm_send(pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}

	return 0;	
}

int z_db_role_insert(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSROLENEWREQ *pstRoleNewReq)
{
	UinCache *pstUinCache;
	int iValid;
	DBAsyn stDBAsyn;
	NewPlayerInfo *pstNewPlayerInfo = &stDBAsyn.AsynCmd.stNewPlayer;
	
	if (PLAYER_STATUS_ROLE_LIST != pstPlayer->eStatus)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
					"z_db_role_insert uin=%d player status=%d", pstPlayer->stRoleData.Uin,
					pstPlayer->eStatus);
		return -1;
	}

	pstUinCache = (UinCache *)sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos, &iValid);
	if (NULL == pstUinCache || 0 == iValid || pstUinCache->iUin != pstPlayer->stRoleData.Uin)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
					"z_db_role_insert_res Uin=%d uincache",  pstPlayer->stRoleData.Uin);
		return -1;
	}

	if (pstUinCache->ucRoleNum >= ROLE_USER_NUM)
	{
		return -1;
	}

	pstPlayer->stOnline.cCheckName = 1;

	//先去nameid_svr 检查名字是否重复
	stDBAsyn.iCmd = DB_CHECK_NAME;
	memcpy(&pstNewPlayerInfo->stNew, pstRoleNewReq, sizeof(pstNewPlayerInfo->stNew));
	pstNewPlayerInfo->ullGid = z_get_gid(pstAppCtx, pstEnv);
	
	return z_db_check_name(pstAppCtx, pstEnv,  pstRoleNewReq->RoleName, pstNewPlayerInfo->ullGid, &stDBAsyn, 
							sizeof(*pstNewPlayerInfo)+sizeof(int), pstPlayer->stRoleData.Uin, ROLE_NAME);
}

int z_db_role_update(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int OperType)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;

	if (pstPlayer->iMagic != PLAYER_SHM_MAGIC)
	{
		return -1;
	}

	pstPlayer->stRoleData.LastLogout = pstAppCtx->stCurr.tv_sec;
	pstPlayer->stRoleData.OLCountTime += pstAppCtx->stCurr.tv_sec-pstPlayer->stRoleData.LastLogin;

	//记录下回写的版本号
	pstPlayer->stRoleData.DataVersion = TDR_METALIB_ROLE_DB_META_VERSION;
	pstPlayer->stRoleData.RoleDetail.StashMoney = pstPlayer->stRoleData.Package.StashPackage.Money;

	STRNCPY(pstPlayer->stRoleData.MacAddr,pstPlayer->aMacAdd,sizeof(pstPlayer->stRoleData.MacAddr));
	
	//退出前要做的事
	if(pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGOUT_REQ &&
		pstPlayer->stOnline.bLogoutPre == 0)
	{
		//防止分线信息，在主线更新的时候被错误处理
		if(pstPlayer->stOnline.stSpanOnline.iStat != SPAN_STAT_CONNECT)
		{
			z_db_role_logout_pre(pstEnv, pstPlayer);
			pstPlayer->stOnline.bLogoutPre = 1;
		}
	}
	
	if (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus &&
		PLAYER_STATUS_ROLE_LOGOUT_REQ != pstPlayer->eStatus &&
		PLAYER_STATUS_ROLE_NOTHING != pstPlayer->eStatus )
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "z_db_role_update %d player status %d",
					pstPlayer->stRoleData.Uin, pstPlayer->eStatus);
		return -1;
	}

       // 最初的回写数据
	if( PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus ||
		PLAYER_STATUS_ROLE_LOGOUT_REQ == pstPlayer->eStatus )
	{
		//防止分线信息，向主线更新的时候被错误处理
		if(pstPlayer->stOnline.stSpanOnline.iStat != SPAN_STAT_CONNECT)
		{
			if(pstPlayer->stOnline.tPkEndTime > pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				pstPlayer->stRoleData.MiscInfo.PkTime = pstPlayer->stOnline.tPkEndTime - pstEnv->pstAppCtx->stCurr.tv_sec;
			}
			else
			{
				pstPlayer->stRoleData.MiscInfo.PkTime = 0;
			}
			
			if (pstPlayer->stOnline.State & CS_STAT_FLY)
			{
				pstPlayer->stRoleData.MiscInfo.Flag |= ROLE_LOGOUT_FLY;
				pstPlayer->stRoleData.MiscInfo.RideItemID = pstPlayer->stOnline.RideItemID;
				pstPlayer->stRoleData.MiscInfo.RideLevel = pstPlayer->stOnline.RideLevel;
			}
			else if  (pstPlayer->stOnline.State & CS_STAT_RIDE_ON)
			{
				pstPlayer->stRoleData.MiscInfo.Flag |= ROLE_LOGOUT_RIDE;
				pstPlayer->stRoleData.MiscInfo.RideItemID = pstPlayer->stOnline.RideItemID;
				pstPlayer->stRoleData.MiscInfo.RideLevel = pstPlayer->stOnline.RideLevel;
			}
			else
			{
				pstPlayer->stRoleData.MiscInfo.Flag &= ~ROLE_LOGOUT_RIDE;
				pstPlayer->stRoleData.MiscInfo.Flag &= ~ROLE_LOGOUT_FLY;
				pstPlayer->stRoleData.MiscInfo.RideItemID = 0;
				pstPlayer->stRoleData.MiscInfo.RideLevel = 0;
			}

			z_skill_roledata(pstAppCtx,  pstEnv, pstPlayer);
			z_status_roledata(pstAppCtx, pstEnv, pstPlayer, 0);
			fairy_evolve_logout(pstEnv, pstPlayer);
		}
	}

	//分线逻辑
	if(!is_my_master_svr(pstAppCtx->iId, pstPlayer))
	{
		player_span_role_update_req(pstEnv, pstPlayer, OperType);
	}
	else
	{
		stNetData.pszBuff = pstEnv->szSendBuffer;
		stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
		stMetaData.pszBuff = (char *)&pstPlayer->stRoleData;
		stMetaData.iBuff = sizeof(pstPlayer->stRoleData);

		stDBAsyn.iCmd = DB_ROLE_UPDATE;
		z_init_torm_head(pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, "RoleData", pstPlayer->stRoleData.Uin, &stDBAsyn);

		if (PLAYER_STATUS_ROLE_LOGOUT_REQ == pstPlayer->eStatus)
		{
			pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		}
		else
		{
			pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
		}
		
		pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstUpdateReq->OperInfo.WhereDef[0] = 0;
		pstUpdateReq->DoInsert = 0;
			
		if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iRoleDBProtoVer))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
			return -1;
		}

		if (0 > z_torm_send(pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
		{
			return -1;
		}
	}

	z_db_update_acc_share_req(pstEnv,pstPlayer);
	
	return 0;
}

int z_db_acc_share_select(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	ACCSHARE stAccShare;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;

	if(pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		return -1;
	}

	if (ACC_SHARE_INIT != pstPlayer->stAccShareData.iStat)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "AccShare select state error Uin=%d AccName=%s state=%d", 
							pstPlayer->stRoleData.Uin, pstPlayer->szAccount,pstPlayer->stAccShareData.iStat);
		return -1;
	}
	
	
	stDBAsyn.iCmd = DB_ACC_SHARE_SELECT;
	z_init_torm_head(pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, "AccShare", pstPlayer->stRoleData.Uin, &stDBAsyn);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	//sprintf(pstSelectReq->OperInfo.WhereDef, "where Uin=%d", pstPlayer->stRoleData.Uin);
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 0;

	memset(&stAccShare, 0, sizeof(stAccShare));
	stAccShare.Uin = pstPlayer->stRoleData.Uin;
		
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stAccShare;
	stMetaData.iBuff = sizeof(stAccShare);

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}

	pstPlayer->stAccShareData.iStat = ACC_SHARE_GETING;
	pstPlayer->stAccShareData.iDBQureyTime = pstAppCtx->stCurr.tv_sec;
	
	tlog_info(pstEnv->pstLogCat, 0, 0, "AccShare select Uin=%d AccName=%s", 
							pstPlayer->stRoleData.Uin, pstPlayer->szAccount);
	
	return 0;
}

int z_db_role_select(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSROLELOGINREQ *pstRoleLoginReq)
{
	
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	ROLEDATA stRoleData;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;
		
	stDBAsyn.iCmd = DB_ROLE_SELECT;
	z_init_torm_head(pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, "RoleData", pstPlayer->stRoleData.Uin, &stDBAsyn);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	sprintf(pstSelectReq->OperInfo.WhereDef, "where RoleID=%llu and CtrlFlag!=%d", pstRoleLoginReq->RoleID, ROLE_DEL_FLAG);
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 0;

	memset(&stRoleData, 0, sizeof(stRoleData));
	stRoleData.Uin = pstPlayer->stRoleData.Uin;
	stRoleData.RoleID = pstRoleLoginReq->RoleID;
		
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRoleData;
	stMetaData.iBuff = sizeof(stRoleData);

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}

	pstPlayer->eStatus = PLAYER_STATUS_ROLE_LOGIN_REQ;
	tlog_info(pstEnv->pstLogCat, 0, 0, "Status=RoleLoginReq Uin=%d AccName=%s", 
							pstPlayer->stRoleData.Uin, pstPlayer->szAccount);
	
	return 0;
}

int z_db_del_time_update( ZONESVRENV * pstEnv,int iUin, const ROLEDELINFO* pstRoleDelInfo,int iCmd)
{
	DBAsyn stDBAsyn;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;

	stDBAsyn.iCmd = iCmd;
	stDBAsyn.AsynCmd.ullRoleID = pstRoleDelInfo->RoleID;
	z_init_torm_head( pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, 
		"RoleDelInfo", iUin, &stDBAsyn );
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pstRoleDelInfo;
	stMetaData.iBuff = sizeof(*pstRoleDelInfo);
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}

	return 0;
}

int z_cs_del_role_update( ZONESVRENV * pstEnv, const ROLEDELINFO* pstRoleDelInfo ,DBAsyn *pstDbsyn)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;

	pstDbsyn->iCmd = DB_CS_DEL_ROLE_UPDATE;
	
	z_init_torm_head( pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, 
		"RoleDelInfo", pstDbsyn->AsynCmd.stCsDelRole.Uin, pstDbsyn);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pstRoleDelInfo;
	stMetaData.iBuff = sizeof(*pstRoleDelInfo);
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}

	return 0;
}

int z_role_del_respite(ZONESVRENV* pstEnv, Player *pstPlayer, CSROLEDELREQ *pstRoleDelReq)
{
	int iValid;
	int i;
	UinCache *pstUinCache;
	ROLELISTINFO *pstRoleListInfo;
	ROLEDELINFO stRoleDel;

	pstUinCache = (UinCache *)sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos, &iValid);
	if (NULL == pstUinCache || 0 == iValid || pstUinCache->iUin != pstPlayer->stRoleData.Uin)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
					"z_db_role_delete_res Uin=%d uincache",  pstPlayer->stRoleData.Uin);
		return 0;
	}

	for (i=pstUinCache->ucRoleNum-1; i>=0; i--)
	{
		pstRoleListInfo = &pstUinCache->astRoleListInfo[i];
		if (pstRoleListInfo->RoleID == pstRoleDelReq->RoleID)
		{
			if (pstRoleListInfo->Level < DELETE_ROLE_LEVEL_LIMIT)
			{
				return 0;
			}

			stRoleDel.RoleID = pstRoleDelReq->RoleID;
			stRoleDel.DelTime = pstEnv->pstAppCtx->stCurr.tv_sec;
			stRoleDel.CtrlFlag = 0;
			z_db_del_time_update( pstEnv, pstPlayer->stRoleData.Uin, &stRoleDel,DB_ROLE_DELETE_RESPITE);
			return 1;
		}
	}

	return 0;
}

ROLELISTINFO * z_get_rolelistinfo_by_roleid(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong RoleID)
{
	int iValid;
	int i;
	UinCache *pstUinCache;
	ROLELISTINFO *pstRoleListInfo;

	pstUinCache = (UinCache *)sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos, &iValid);
	if (NULL == pstUinCache || 0 == iValid || pstUinCache->iUin != pstPlayer->stRoleData.Uin)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
					"z_db_role_delete_res Uin=%d uincache",  pstPlayer->stRoleData.Uin);
		return 0;
	}

	for (i=pstUinCache->ucRoleNum-1; i>=0; i--)
	{
		pstRoleListInfo = &pstUinCache->astRoleListInfo[i];
		if (pstRoleListInfo->RoleID == RoleID)
		{
			return pstRoleListInfo;
		}
	}

	return NULL;
}

//获取角色的详细数据
int z_db_get_roledetail_req(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSROLEDELREQ *pstRoleDelReq)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	CHGROLEDETAIL stChgRoleDetail;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;
	ROLELISTINFO *pstRoleListInfo = NULL;

	if (PLAYER_STATUS_ROLE_LIST != pstPlayer->eStatus)
	{
		return -1;
	}

	pstRoleListInfo = z_get_rolelistinfo_by_roleid(pstEnv, pstPlayer, pstRoleDelReq->RoleID);

	memset(&stDBAsyn,0,sizeof(stDBAsyn));
	stDBAsyn.iCmd = DB_ROLEDETAIL_SELECT_RES;
	stDBAsyn.AsynCmd.stDetailUpdate.ullRoleID = pstRoleDelReq->RoleID;
	stDBAsyn.AsynCmd.stDetailUpdate.iUin = pstPlayer->stRoleData.Uin;
	stDBAsyn.AsynCmd.stDetailUpdate.RoleInfo.RoleID = pstRoleDelReq->RoleID;
	STRNCPY(stDBAsyn.AsynCmd.stDetailUpdate.RoleInfo.RoleName,pstRoleListInfo->RoleName,
			CCH(pstRoleListInfo->RoleName));
	STRNCPY(stDBAsyn.AsynCmd.stDetailUpdate.szAccount,pstPlayer->szAccount,
			CCH(stDBAsyn.AsynCmd.stDetailUpdate.szAccount));
	
	
	z_init_torm_head(pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, 
		               "ChgRoleDetail", pstPlayer->stRoleData.Uin, &stDBAsyn);
	
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	sprintf(pstSelectReq->OperInfo.WhereDef, "where RoleID=%llu && CtrlFlag!=%d", pstRoleDelReq->RoleID, ROLE_DEL_FLAG);
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 0;

	memset(&stChgRoleDetail, 0, sizeof(stChgRoleDetail));
	stChgRoleDetail.Uin = pstPlayer->stRoleData.Uin;
	stChgRoleDetail.RoleID = pstRoleDelReq->RoleID;
	
		
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stChgRoleDetail;
	stMetaData.iBuff = sizeof(stChgRoleDetail);

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}
	
	return 0;
}
int z_db_get_roledetail_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TORMSELECTRES *pstSelectRes,DBAsyn *pstDbsyn)
{
    	CHGROLEDETAIL stChgRoleDetail;
	int iHas = 0;
	//unsigned int GodCoin = 0;
	
	memset(&stChgRoleDetail,0,sizeof(stChgRoleDetail));
	if (1 != pstSelectRes->SelectResult.ResultNum)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
					"select roledetail uin=%d role num=%d", pstPlayer->stRoleData.Uin,
					pstSelectRes->SelectResult.ResultNum);
		return -1;
	}

	if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stChgRoleDetail, sizeof(stChgRoleDetail)))
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
					"uin=%d torm_fetch_one_meta fail", pstPlayer->stRoleData.Uin);
		return -1;
	}

	if (PLAYER_STATUS_ROLE_LIST != pstPlayer->eStatus ||
		pstDbsyn->AsynCmd.stDetailUpdate.ullRoleID != stChgRoleDetail.RoleID )
	{
		return -1;
	}

	if (stChgRoleDetail.RoleDetail.GodCoin > 0)
	{
		iHas = 1;
	}

	
	{
		CSROLEDELREQ stRoleDelReq;
		stRoleDelReq.RoleID = pstDbsyn->AsynCmd.stDetailUpdate.ullRoleID;
	  	z_db_role_del(pstAppCtx, pstEnv, pstPlayer,&stRoleDelReq,iHas);
	}
	/*if(stChgRoleDetail.RoleDetail.GodCoin <= 0)
	{
		//接着删除
		CSROLEDELREQ stRoleDelReq;
		stRoleDelReq.RoleID = pstDbsyn->AsynCmd.stDetailUpdate.ullRoleID;
	   	 z_db_role_del(pstAppCtx, pstEnv, pstPlayer,&stRoleDelReq);
	}
	else
	{
		//先把金券清0
		GodCoin = stChgRoleDetail.RoleDetail.GodCoin;
		stChgRoleDetail.RoleDetail.GodCoin = 0;
		z_db_set_roledetail_req(pstAppCtx,pstEnv,pstPlayer,&stChgRoleDetail,GodCoin,&pstDbsyn->AsynCmd.stDetailUpdate);
	}*/
	return 0;
}

int z_db_set_roledetail_req(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, 
				Player *pstPlayer, CHGROLEDETAIL *pstChgRoleDetail,unsigned int GodCoin,RoleDetailUpdate *pstDetailUpdate)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;
	
	if (PLAYER_STATUS_ROLE_LIST != pstPlayer->eStatus)
	{
		return -1;
	}
	
	memset(&stDBAsyn,0,sizeof(stDBAsyn));	
	stDBAsyn.iCmd = DB_ROLEDETAIL_UPDATE_RES;
	stDBAsyn.AsynCmd.stDetailUpdate = *pstDetailUpdate;
	stDBAsyn.AsynCmd.stDetailUpdate.GodCoin= GodCoin;
	
	z_init_torm_head(pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, 
		               "ChgRoleDetail", pstPlayer->stRoleData.Uin, &stDBAsyn);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;
	snprintf(pstUpdateReq->OperInfo.WhereDef, sizeof(pstUpdateReq->OperInfo.WhereDef),
				"where RoleID=%llu && CtrlFlag!=%d", pstChgRoleDetail->RoleID, ROLE_DEL_FLAG);
		
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pstChgRoleDetail;
	stMetaData.iBuff = sizeof(CHGROLEDETAIL);

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}
	
	return 0;
}

int z_db_set_roledetail_res(ZONESVRENV* pstEnv, Player *pstPlayer,TORMUPDATERES *pstUpdateRes, DBAsyn *pstDBAsyn)
{
	CSROLEDELREQ stRoleDelReq;

	memset(&stRoleDelReq,0,sizeof(stRoleDelReq));
	if (TORM_ERR_IS_ERROR(pstUpdateRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "update roledetail failed." );
		return -1;
	}

	if (PLAYER_STATUS_ROLE_LIST != pstPlayer->eStatus)
	{
		return -1;
	}

	if (1 != pstUpdateRes->AffectRows)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
					"update roledetail uin=%d role num=%d", pstPlayer->stRoleData.Uin,
					pstUpdateRes->AffectRows);
		return -1;
	}
	//接着删除

	stRoleDelReq.RoleID = pstDBAsyn->AsynCmd.stDetailUpdate.ullRoleID;
    	z_db_role_del(pstEnv->pstAppCtx, pstEnv, pstPlayer,&stRoleDelReq,0);

	/*
	//加金券
	shopping_operate_point_req(pstEnv->pstAppCtx,pstEnv,pstPlayer->szAccount,pstPlayer->stRoleData.Uin,
							pstDBAsyn->AsynCmd.stDetailUpdate.GodCoin,	&pstDBAsyn->AsynCmd.stDetailUpdate,sizeof(RoleDetailUpdate),
							DATATYPE_POINT_BY_DELROLE,OPERATE_POINT_ADD);	

	z_role_shopping_exchange_oplog(pstEnv, pstDBAsyn->AsynCmd.stDetailUpdate.ullRoleID, 
									pstDBAsyn->AsynCmd.stDetailUpdate.RoleInfo.RoleName,
								pstPlayer->szAccount,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.Level,
								pstDBAsyn->AsynCmd.stDetailUpdate.GodCoin,0,0,1);
	*/
	
	return 0;
}

/*这个函数才是完完全全删除角色*/
int z_db_role_del_in( ZONESVRENV* pstEnv,  tdr_ulonglong ullRoleID, int iUin)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMDELETEREQ *pstDeleteReq = &pstPkg->Body.DeleteReq;
	ROLEDATA stRoleData;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;
	
	stDBAsyn.iCmd = DB_ROLE_DELETE;
	stDBAsyn.AsynCmd.ullRoleID = ullRoleID;
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_DELETE_REQ, 
						"RoleData", iUin, &stDBAsyn);
	pstDeleteReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDeleteReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDeleteReq->OperInfo.WhereDef[0] = 0;
		
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRoleData;
	stMetaData.iBuff = sizeof(stRoleData);
	
	memset(&stRoleData, 0, sizeof(stRoleData));
	stRoleData.RoleID = ullRoleID;
	stRoleData.Uin = iUin;
		
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}
	return 0;
}

int z_db_role_del(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSROLEDELREQ *pstRoleDelReq,int iHasGodCoin)
{
	int iLevel;
	if (PLAYER_STATUS_ROLE_LIST != pstPlayer->eStatus)
	{
		return -1;
	}

	iLevel  = player_role_del_level(pstEnv, pstRoleDelReq->RoleID, pstPlayer);
	if (iHasGodCoin || iLevel >= DELETE_ROLE_LEVEL_LIMIT)
	{
		UinCache stUinHashData;
		UinCache *pstUinCache=NULL;
		int i;
		ROLEDELINFO stInfo;
	
		stUinHashData.iUin = pstPlayer->stRoleData.Uin;
		pstUinCache = (UinCache *)sht_find(pstEnv->pstShtUinCache,&stUinHashData,UinHashCmp,UinHashCode);
		if (!pstUinCache)
		{
			return -1;
		}

		for (i=pstUinCache->ucRoleNum-1; i>=0; i--)
		{
			if ( pstUinCache->astRoleListInfo[i].RoleID == pstRoleDelReq->RoleID)
			{
				stInfo.CtrlFlag = ROLE_DEL_FLAG;
				stInfo.DelTime = pstUinCache->astRoleListInfo[i].DelTime;
				if (iHasGodCoin && iLevel < DELETE_ROLE_LEVEL_LIMIT)
				{
					stInfo.DelTime = pstAppCtx->stCurr.tv_sec;
				}
				stInfo.RoleID = pstUinCache->astRoleListInfo[i].RoleID;
				return z_db_del_time_update( pstEnv, pstUinCache->astRoleListInfo[i].Uin, &stInfo,DB_ROLE_DELETE_REALLY);
			}
		}
		return 0;
	}
	else
	{
		return z_db_role_del_in(pstEnv, pstRoleDelReq->RoleID, pstPlayer->stRoleData.Uin);
	}

	/*大于10级的在这里处理  
	if ( z_role_del_respite(pstEnv, pstPlayer, pstRoleDelReq) )
	{
		return 0;
	}
	*/
	
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_account_db_update_name(ZONESVRENV* pstEnv, int iUin, 
	                           const ACCOUNTROLENAME *pstAccRoleName)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;

	z_init_torm_head( pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, 
		"AccountRoleName", iUin, NULL );
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	snprintf(pstUpdateReq->OperInfo.WhereDef, sizeof(pstUpdateReq->OperInfo.WhereDef),
				"where RoleWID=%llu",pstAccRoleName->RoleWID);
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pstAccRoleName;
	stMetaData.iBuff = sizeof(*pstAccRoleName);
	
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iAccountDBID))
	{
		return -1;
	}
	return 0;	
}

int z_account_db_insert(ZONESVRENV* pstEnv, Player *pstPlayer, 
	                      const ACCOUNTROLE *pstAccRole)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMINSERTREQ *pstInsertReq = &pstPkg->Body.InsertReq;
	TDRDATA stNetData;
	TDRDATA stMetaData;

	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_INSERT_REQ, 
		               "AccountRole", pstPlayer->stRoleData.Uin, NULL);
	pstInsertReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstInsertReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstInsertReq->OperInfo.WhereDef[0] = 0;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pstAccRole;
	stMetaData.iBuff = sizeof(*pstAccRole);

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		                        pstEnv->iAccountDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iAccountDBID))
	{
		return -1;
	}
	
	return 0;
}

int z_account_db_del(ZONESVRENV* pstEnv, int iUin,tdr_ulonglong ullRoleID)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMDELETEREQ *pstDeleteReq = &pstPkg->Body.DeleteReq;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	ACCOUNTROLE stAccRole;

	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_DELETE_REQ, 
		               "AccountRole", iUin, NULL);
	pstDeleteReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstDeleteReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDeleteReq->OperInfo.WhereDef[0] = 0;

	memset(&stAccRole, 0, sizeof(stAccRole));
	stAccRole.RoleWID = ullRoleID;
		
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stAccRole;
	stMetaData.iBuff = sizeof(stAccRole);
		
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		                        pstEnv->iAccountDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iAccountDBID))
	{
		return -1;
	}
	
	return 0;
}

int z_account_db_update_deltime(ZONESVRENV* pstEnv, Player *pstPlayer,
	                              				tdr_ulonglong ullRoleID, int iDelTime)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	ACCOUNTROLEDELTIME stAccDelTime;

	z_init_torm_head( pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, 
		"AccountRoleDelTime", pstPlayer->stRoleData.Uin, NULL );
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	stAccDelTime.RoleWID = ullRoleID;
	stAccDelTime.DelTime = iDelTime;
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stAccDelTime;
	stMetaData.iBuff = sizeof(stAccDelTime);
	
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		pstEnv->iAccountDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iAccountDBID))
	{
		return -1;
	}

	return 0;	
}

int z_account_db_update_firstlogin(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iFlag = 0;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	ACCOUNTROLEFIRSTLOGIN stRoleFistLogin;
	ROLEMISCINFO *pstRoleMisc = &pstPlayer->stRoleData.MiscInfo;

	if (0 != pstRoleMisc->FirstLoginTime && 0 != pstRoleMisc->SecondLoginTime)
	{
		return 0;
	}

	if (0 != pstRoleMisc->FirstLoginTime && 0 == pstRoleMisc->SecondLoginTime &&
		pstEnv->pstAppCtx->stCurr.tv_sec - pstRoleMisc->FirstLoginTime >= 24*3600)
	{
		pstRoleMisc->SecondLoginTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		iFlag = 1;
	}
	
	if (0 == pstRoleMisc->FirstLoginTime)
	{
		pstRoleMisc->FirstLoginTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		iFlag = 1;
	}

	if (0 == iFlag)
	{
		return 0;
	}

	z_init_torm_head( pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, 
		"AccountRoleFirstLogin", pstPlayer->stRoleData.Uin, NULL );
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	stRoleFistLogin.RoleWID = pstPlayer->stRoleData.RoleID;
	stRoleFistLogin.FirstLoginTime = pstRoleMisc->FirstLoginTime;
	stRoleFistLogin.SecondLoginTime = pstRoleMisc->SecondLoginTime;
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRoleFistLogin;
	stMetaData.iBuff = sizeof(stRoleFistLogin);
	
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		pstEnv->iAccountDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iAccountDBID))
	{
		return -1;
	}
	
	return 0;
}

int z_account_db_update_online(ZONESVRENV* pstEnv, Player *pstPlayer,int iOnlineFlag)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	//ACCOUNTROLEDELTIME stAccDelTime;
	ACCOUNTROLEONLINE stAccOnline;

	z_init_torm_head( pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, 
		"AccountRoleOnline", pstPlayer->stRoleData.Uin, NULL );
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	stAccOnline.RoleWID = pstPlayer->stRoleData.RoleID;
	stAccOnline.OnlineFlag = iOnlineFlag;
	STRNCPY(stAccOnline.MacAddr,pstPlayer->aMacAdd,sizeof(stAccOnline.MacAddr));
	stAccOnline.LastLoginIP = pstPlayer->stConnectInfo.stExtraInfo.IPInfo.ip;
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stAccOnline;
	stMetaData.iBuff = sizeof(stAccOnline);
	
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		pstEnv->iAccountDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iAccountDBID))
	{
		return -1;
	}

	return 0;	
}

int z_account_db_role_logout(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	ACCOUNTROLELOGOUT stAccLogout;

	z_init_torm_head( pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, 
		"AccountRoleLogout", pstPlayer->stRoleData.Uin, NULL );
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	stAccLogout.RoleWID = pstPlayer->stRoleData.RoleID;
	stAccLogout.OnlineFlag = 0;
	stAccLogout.Level = pstPlayer->stRoleData.Level;
	stAccLogout.LastLogout = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stAccLogout;
	stMetaData.iBuff = sizeof(stAccLogout);
	
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		pstEnv->iAccountDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iAccountDBID))
	{
		return -1;
	}

	return 0;	
}


int z_account_db_update_lastlogout(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	ACCOUNTLASTLOGOUT stLogout;

	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ,
		             "AccountLastLogout", pstPlayer->stRoleData.Uin, NULL);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	stLogout.Uin = pstPlayer->stRoleData.Uin;
	stLogout.LastLogout = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stLogout;
	stMetaData.iBuff = sizeof(stLogout);
	
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


int z_account_db_update_archive(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	ACCOUNTARCHIVE stAccArchive;

	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ,
		             "AccountArchive", pstPlayer->stRoleData.Uin, NULL);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	stAccArchive.Uin = pstPlayer->stRoleData.Uin;
	stAccArchive.LastLoginTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	stAccArchive.LastLoginIP = pstPlayer->stConnectInfo.stExtraInfo.IPInfo.ip;
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stAccArchive;
	stMetaData.iBuff = sizeof(stAccArchive);
	
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

int z_account_db_update_worldid(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	//ACCOUNTARCHIVE stAccArchive;
	ACCOUNTROLEWORLDID stAccoutWorldID;

	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ,
		             "AccountRoleWorldID", pstPlayer->stRoleData.Uin, NULL);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	stAccoutWorldID.Uin = pstPlayer->stRoleData.Uin;
	stAccoutWorldID.LastLoginWorldID = pstEnv->unWorldID;
	//stAccArchive.LastLoginTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	//stAccArchive.LastLoginIP = pstPlayer->stConnectInfo.stExtraInfo.IPInfo.ip;
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stAccoutWorldID;
	stMetaData.iBuff = sizeof(stAccoutWorldID);
	
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



// RoleDropName
int z_db_role_drop_update_name(ZONESVRENV* pstEnv, tdr_ulonglong ullWID, const char * pszName)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	ROLEDROPNAME stRoleDropName;

	stRoleDropName.RoleID = ullWID;
	STRNCPY(stRoleDropName.RoleName, pszName, sizeof(stRoleDropName.RoleName));
	
	z_init_torm_head( pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, 
		"RoleDropName", 0, NULL);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRoleDropName;
	stMetaData.iBuff = sizeof(stRoleDropName);
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		pstEnv->iRoleDBProtoVer))
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

// 
/*
int z_db_shadowroledata_up_name(ZONESVRENV* pstEnv, tdr_ulonglong ullWID, const char * pszName)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	SHADOWROLEDATANAME stSDName;

	stSDName.RoleID = ullWID;
	STRNCPY(stSDName.RoleName, pszName, sizeof(stSDName.RoleName));
	
	z_init_torm_head( pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, 
		"ShadowRoleDataName", 0, NULL);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stSDName;
	stMetaData.iBuff = sizeof(stSDName);
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		pstEnv->iRoleDBProtoVer))
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
*/
int z_db_role_drop_update(ZONESVRENV* pstEnv, ROLEDROP *pstDropUpdate)
{
	DBAsyn stDBAsyn;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;

	stDBAsyn.iCmd = DB_ROLE_DROP_UPDATE;
	
	z_init_torm_head( pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, 
		"RoleDrop", pstDropUpdate->Uin, &stDBAsyn );
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;

	//没有就插入一个
	pstUpdateReq->DoInsert = 1;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pstDropUpdate;
	stMetaData.iBuff = sizeof(*pstDropUpdate);
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		pstEnv->iRoleDBProtoVer))
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

int z_db_role_drop_select(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, DropObjUpdate *pstDropUpdate)
{
	
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	ROLEDROP stRoleDrop;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;
		
	stDBAsyn.iCmd = DB_ROLE_DROP_SELECT;
	stDBAsyn.AsynCmd.stDropObjUpdate = *pstDropUpdate;
	
	z_init_torm_head(pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, "RoleDrop", pstPlayer->stRoleData.Uin, &stDBAsyn);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	sprintf(pstSelectReq->OperInfo.WhereDef, "where RoleID=%llu", pstPlayer->stRoleData.RoleID);
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 0;

	memset(&stRoleDrop, 0, sizeof(stRoleDrop));
	stRoleDrop.Uin = pstPlayer->stRoleData.Uin;
	stRoleDrop.RoleID = pstPlayer->stRoleData.RoleID;
		
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRoleDrop;
	stMetaData.iBuff = sizeof(stRoleDrop);

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}
	
	return 0;
}

int z_db_acc_share_select_res(ZONESVRENV* pstEnv, Player *pstPlayer, TORMSELECTRES *pstSelectRes)
{	
	int iDiff = 0;
	
	//检查状态
	iDiff = pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stAccShareData.iDBQureyTime;
	if(pstPlayer->stAccShareData.iStat != ACC_SHARE_GETING || 
		pstPlayer->stAccShareData.iDBQureyTime == 0 || 
		iDiff > 15)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
						"uin=%d roleid = %llu acc_share_select state error state=%d time diff=%d", pstPlayer->stRoleData.Uin, pstPlayer->stRoleData.RoleID,
						pstPlayer->stAccShareData.iStat,iDiff);
		return -1;
	}
		
	if ( pstSelectRes->SelectResult.ResultNum <= 0)
	{
		z_db_acc_share_insert(pstEnv->pstAppCtx,pstEnv,pstPlayer);
		
		return 0;
	}
	else
	{
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&pstPlayer->stAccShareData.stAccShare, sizeof(pstPlayer->stAccShareData.stAccShare)))
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
						"uin=%d roleid = %llu torm_fetch_one_meta AccShare fail", pstPlayer->stRoleData.Uin, pstPlayer->stRoleData.RoleID);

			pstPlayer->stAccShareData.iStat = ACC_SHARE_FAIL;
			return -1;
		}
	}

	player_acc_share_final(pstEnv,pstPlayer);
	
	return 0;
}

int z_db_role_drop_select_res(ZONESVRENV* pstEnv, Player *pstPlayer, TORMSELECTRES *pstSelectRes, DropObjUpdate *pstDropUpdate)
{	
	ROLEDROP stDropUpdate;
	ROLEDROPDATA *pstDropData;

	//检查 player 状态、roleid 
	if(pstPlayer->stRoleData.RoleID != pstDropUpdate->ullRoleID || 
		pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		return -1;
	}
		
	if ( pstSelectRes->SelectResult.ResultNum <= 0)
	{
		stDropUpdate.Uin = pstPlayer->stRoleData.Uin;
		stDropUpdate.RoleID = pstPlayer->stRoleData.RoleID;
		STRNCPY(stDropUpdate.RoleName, pstPlayer->stRoleData.RoleName, 
		      CCH(stDropUpdate.RoleName));

		stDropUpdate.DropData.GridNum = 0;
		stDropUpdate.DropData.PetNum = 0;
	}
	else
	{
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stDropUpdate, sizeof(stDropUpdate)))
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
						"uin=%d roleid = %llu torm_fetch_one_meta delobj fail", pstPlayer->stRoleData.Uin, pstPlayer->stRoleData.RoleID);
			return -1;
		}
	}

	pstDropData = &stDropUpdate.DropData;
	
	switch(pstDropUpdate->iType)
	{
		case ROLE_DROP_GRID:
			if (pstDropData->GridNum >= MAX_ROLE_DROP_GRID)
			{
				pstDropData->GridNum--;
				if(MAX_ROLE_DROP_GRID > 1)
				{
					memmove(&pstDropData->DropGrids[0], &pstDropData->DropGrids[1], 
							(MAX_ROLE_DROP_GRID-1)*sizeof(pstDropData->DropGrids[0]));
				}
			}

			pstDropData->DropGrids[pstDropData->GridNum].RestoreFlag = 0;
			pstDropData->DropGrids[pstDropData->GridNum].Grid = pstDropUpdate->stData.stGrid;
			pstDropData->GridNum++;
			break;
		case ROLE_DROP_PET:
			if (pstDropData->PetNum >= MAX_ROLE_DROP_PET)
			{
				pstDropData->PetNum--;
				if(MAX_ROLE_DROP_PET > 1)
				{
					memmove(&pstDropData->DropPets[0], &pstDropData->DropPets[1], 
							(MAX_ROLE_DROP_PET-1)*sizeof(pstDropData->DropPets[0]));
				}
			}

			pstDropData->DropPets[pstDropData->PetNum].RestoreFlag = 0;
			pstDropData->DropPets[pstDropData->PetNum].Pet = pstDropUpdate->stData.stPet;
			pstDropData->PetNum++;
			break;
		default:
			return -1;
			break;
	}
	
	z_db_role_drop_update(pstEnv, &stDropUpdate);
	
	return 0;
}

//丢弃重要物品等记录
int z_role_drop_record(ZONESVRENV* pstEnv, Player *pstPlayer, int iType, void *pstData)
{
	DropObjUpdate stDrop;

	stDrop.iType = iType;
	stDrop.ullRoleID = pstPlayer->stRoleData.RoleID;
	
	switch(iType)
	{
		case ROLE_DROP_GRID:
			stDrop.stData.stGrid = *(ROLEGRID *)pstData;
			break;
		case ROLE_DROP_PET:
			stDrop.stData.stPet = *(ROLEPET *)pstData;
			break;
		default:
			return -1;
			break;
	}

	return z_db_role_drop_select(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stDrop);
}

static int player_undel_check( ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong ullRoleID)
{
	int i;
	UinCache *pstUinCache;
	int iValid;
	ROLELISTINFO *pstInfo = NULL;
	
	pstUinCache = (UinCache *)sht_pos(pstEnv->pstShtUinCache, pstPlayer->iUinCachePos, &iValid);
	if (pstUinCache && iValid)
	{
		for (i=0; i<pstUinCache->ucRoleNum; i++)
		{
			if (pstUinCache->astRoleListInfo[i].RoleID == ullRoleID)
			{
				pstInfo = &pstUinCache->astRoleListInfo[i];
				break;
			}
		}
	}
	
	if (!pstInfo || pstInfo->DelTime <= 0 || pstInfo->CtrlFlag == ROLE_DEL_FLAG)
	{
		return -1;
	} 

	
	return 0;
}



int player_undel(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	ROLEDELINFO stRoleDel;
	tdr_ulonglong ullRoleID = pstCsPkg->Body.CsUnDel.RoleID;

	if (player_undel_check(pstEnv,pstPlayer,ullRoleID) < 0) return -1;

	stRoleDel.RoleID = ullRoleID;
	stRoleDel.DelTime = 0;
	stRoleDel.CtrlFlag = 0;

	z_db_del_time_update( pstEnv, pstPlayer->stRoleData.Uin, &stRoleDel,DB_ROLE_UPDATE_UNDEL);
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

//从数据库中获取MiscInfo
int z_get_role_misc_info(ZONESVRENV* pstEnv,DBMiscInfo *pstDBMiscInfo,tdr_ulonglong ullRoleID)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;
	MISCROLEINFO stMiscRoleInfo;
	
	memset(&stDBAsyn,0,sizeof(stDBAsyn));
	stDBAsyn.iCmd = DB_ROLE_MISC_INFO_SELECT;
	memcpy(&stDBAsyn.AsynCmd.stDBMiscInfo,pstDBMiscInfo,sizeof(stDBAsyn.AsynCmd.stDBMiscInfo));
	
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, "MiscRoleInfo", -1, &stDBAsyn);
	
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	sprintf(pstSelectReq->OperInfo.WhereDef, "where RoleID=%llu",ullRoleID);
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 1;

	memset(&stMiscRoleInfo, 0, sizeof(stMiscRoleInfo));
	stMiscRoleInfo.RoleID = ullRoleID;
			
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stMiscRoleInfo;
	stMetaData.iBuff = sizeof(stMiscRoleInfo);

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}
	return 0;
}

int z_db_misc_info_res(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes,DBAsyn *pstDbsyn)
{
	if(pstDbsyn == NULL)
	{
		return -1;
	}
	
	DBMiscInfo *pstDBMiscInfo = NULL;
	pstDBMiscInfo = &pstDbsyn->AsynCmd.stDBMiscInfo;
	
	if(NULL == pstDBMiscInfo)
	{
		return -1;
	}
	
	switch(pstDBMiscInfo->iCmd)
	{
		case DB_MISC_INFO_DSN_UPDATE:
			z_db_misc_info_dsn_res(pstEnv,pstSelectRes,pstDBMiscInfo);
			break;
		case DB_MISC_INFO_WEDDINGINFO_UPDATE:
			z_db_misc_info_wedding_res(pstEnv,pstSelectRes,pstDBMiscInfo);
			break;
		default:
			break;
	}

	return 0;
}


int z_db_misc_info_dsn_res(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes,DBMiscInfo *pstDBMiscInfo)
{
	MISCROLEINFO stMiscRoleInfo;
	ROLEDESIGNATIONS *pstInfo = NULL;
	unsigned int uiDsn = 0;
	DESIGNATIONENTRY* pstDsn;
	const DESIGNATIONDEF* pstDsnDef;
	unsigned int i = 0;
	MiscInfoDsn *pstMiscInfoDsn = NULL;
	
	if (1 != pstSelectRes->SelectResult.ResultNum)
	{
		return -1;
	}

	if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stMiscRoleInfo, sizeof(stMiscRoleInfo)))
	{
		return -1;
	}
	
	pstMiscInfoDsn = &pstDBMiscInfo->AsynMiscInfo.stMiscInfoDsn;
	
	if(NULL == pstMiscInfoDsn)
	{
		return -1;
	}
	
	if (stMiscRoleInfo.RoleID != pstMiscInfoDsn->ullRoleID)
	{
		return -1;
	}
	
	uiDsn = pstMiscInfoDsn->uiDsn;
	if(!uiDsn)
	{
		return -1;
	}
	
	pstDsnDef = z_designation_def(pstEnv, uiDsn);
	if(!pstDsnDef)
	{
		return -1;
	}
	
	pstInfo = &stMiscRoleInfo.MiscInfo.DesignationInfo;
	
	for(i=0;i<pstInfo->Num;i++)
	{
		//如果称号已有，延长时间
		if(pstInfo->Designations[i].ID == uiDsn)
		{
			if (pstDsnDef && pstDsnDef->DesTime > 0)
			{
				pstInfo->Designations[i].EndTime = pstEnv->pstAppCtx->stCurr.tv_sec+pstDsnDef->DesTime;
			}
			break;
		}
	}
	
	if(i>=pstInfo->Num)
	{
		if(pstInfo->Num >= MAX_ROLE_DESIGNATION)
		{
			return -1;
		}
		
		//如果称号没有，就加
		pstDsn = &pstInfo->Designations[pstInfo->Num++];
		pstDsn->ID = uiDsn;
		pstDsn->LastUseTime = 0;
		pstDsn->EndTime = 0;
		if (pstDsnDef && pstDsnDef->DesTime > 0)
		{
			pstDsn->EndTime = pstEnv->pstAppCtx->stCurr.tv_sec + pstDsnDef->DesTime;
		}
	}

	pstInfo->CurrentDesignationID = uiDsn;
	
	z_db_update_misc_info(pstEnv, &stMiscRoleInfo);
	
	return 0;
}

//更新回数据库
int z_db_update_misc_info(ZONESVRENV* pstEnv, MISCROLEINFO *pstMiscRoleInfo)
{
	TDRDATA stMetaData;
	TDRDATA stNetData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ,
		             "MiscRoleInfo", -1, NULL);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	sprintf(&pstUpdateReq->OperInfo.WhereDef[0], "where RoleID=%llu", pstMiscRoleInfo->RoleID);
	pstUpdateReq->DoInsert = 0;
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pstMiscRoleInfo;
	stMetaData.iBuff = sizeof(*pstMiscRoleInfo);
	
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		                      pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if(0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}
	return 0;
}

int z_db_misc_info_wedding_res(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes,DBMiscInfo *pstDBMiscInfo)
{
	MISCROLEINFO stMiscRoleInfo;
	WEDDINGINFO *pstInfo = NULL;
	int iType = 0;
	MiscInfoWedding *pstMiscInfoWedding = NULL;
	
	if (1 != pstSelectRes->SelectResult.ResultNum)
	{
		return -1;
	}

	if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stMiscRoleInfo, sizeof(stMiscRoleInfo)))
	{
		return -1;
	}
	
	pstMiscInfoWedding = &pstDBMiscInfo->AsynMiscInfo.stMiscInfoWedding;
	
	if(NULL == pstMiscInfoWedding)
	{
		return -1;
	}
	
	if (stMiscRoleInfo.RoleID != pstMiscInfoWedding->ullRoleID)
	{
		return -1;
	}
	
	pstInfo = &stMiscRoleInfo.MiscInfo.WeddingInfo;
	
	iType = pstMiscInfoWedding->type;
	
	switch(iType)
	{
		case DB_MISC_WEDDING_SET_NULL:
			memset(pstInfo,0,sizeof(*pstInfo));
			break;
		default:
			break;
	}

	z_db_update_misc_info(pstEnv, &stMiscRoleInfo);
	return 0;
}

//从数据库中获取RoleData表中的Buddy
int z_get_role_buddy(ZONESVRENV* pstEnv,DBRoleBuddy *pstDBRoleBuddy,tdr_ulonglong ullRoleID)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;
	ROLEDATABUDDY stRoleBuddy;
	
	memset(&stDBAsyn,0,sizeof(stDBAsyn));
	stDBAsyn.iCmd = DB_ROLE_BUDDY_SELECT;
	memcpy(&stDBAsyn.AsynCmd.stDBRoleBuddy,pstDBRoleBuddy,sizeof(stDBAsyn.AsynCmd.stDBRoleBuddy));
	
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, "RoleDataBuddy", -1, &stDBAsyn);
	
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	sprintf(pstSelectReq->OperInfo.WhereDef, "where RoleID=%llu",ullRoleID);
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 1;

	memset(&stRoleBuddy, 0, sizeof(stRoleBuddy));
	stRoleBuddy.RoleID = ullRoleID;
			
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRoleBuddy;
	stMetaData.iBuff = sizeof(stRoleBuddy);

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}
	return 0;
}
	
//更新buddy回数据库
int z_db_update_role_buddy_info(ZONESVRENV* pstEnv, ROLEDATABUDDY* pstRoleBuddy)
{
	TDRDATA stMetaData;
	TDRDATA stNetData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ,
		             "RoleDataBuddy", -1, NULL);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	sprintf(&pstUpdateReq->OperInfo.WhereDef[0], "where RoleID=%llu", pstRoleBuddy->RoleID);
	pstUpdateReq->DoInsert = 0;
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pstRoleBuddy;
	stMetaData.iBuff = sizeof(*pstRoleBuddy);
	
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		                      pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if(0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}
	return 0;
}

int z_db_role_buddy_info_res(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes,DBAsyn *pstDbsyn)
{
	ROLEBUDDY *pstRoleBuddy = NULL;
	ROLEDATABUDDY stRoleBuddy;
	DBRoleBuddy *pstDBRoleBuddy = &pstDbsyn->AsynCmd.stDBRoleBuddy;
	int itype;

	if (1 != pstSelectRes->SelectResult.ResultNum)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
					"role_base_db_res RoleID =%llu ", pstDBRoleBuddy->ullRoleID);
		return -1;
	}

	if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stRoleBuddy, sizeof(stRoleBuddy)))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
					"RoleID=%llu torm_fetch_one_meta fail", pstDBRoleBuddy->ullRoleID);
		return -1;
	}
	
	if(pstDBRoleBuddy->ullRoleID != stRoleBuddy.RoleID)
	{
		return -1;
	}

	pstRoleBuddy = &stRoleBuddy.Buddy;
	
	itype = pstDBRoleBuddy->type;
	
	switch(itype)
	{
		case DB_WEDDING_REMOVE_RELATION:
			z_db_wedding_remove_relation(pstEnv,pstRoleBuddy,pstDBRoleBuddy->ullOtherRoleID);
			break;
		case DB_WEDDING_REMOVE_FRIEND:
			z_db_wedding_remove_friend(pstEnv,pstRoleBuddy,pstDBRoleBuddy->ullOtherRoleID);
			break;
		default:
			break;
	}
	
	z_db_update_role_buddy_info(pstEnv, &stRoleBuddy);
	return 0;
}

int z_db_wedding_remove_relation(ZONESVRENV* pstEnv,ROLEBUDDY *pstRoleBuddy,tdr_ulonglong ullOtherRoleID)
{
	//双方头顶信息删除身份关系相关的显示
	pstRoleBuddy->SpouseNum = 0;
	
	int ret;
	ret = online_info_find_friend_byID(pstRoleBuddy, ullOtherRoleID);
	if(ret < 0)
	{
		return -1;
	}
	
	//好友列表中，删除相应对象的关系标志
	pstRoleBuddy->Friends[ret].IntimateColorType = INTIMATE_COLOR_FRIEND;
	
	return 0;
}

int z_db_wedding_remove_friend(ZONESVRENV* pstEnv,ROLEBUDDY *pstRoleBuddy,tdr_ulonglong ullOtherRoleID)
{
	int ret;
	ret = online_info_find_friend_byID(pstRoleBuddy, ullOtherRoleID);
	if(ret < 0)
	{
		return -1;
	}
	
	pstRoleBuddy->Friends[ret].BuddyVal = 0;
	return 0;
}

//从数据库中获取RoleData中的SkillInfo
int z_get_role_skill_info(ZONESVRENV* pstEnv,DBRoleSkillInfo *pstDBRoleSkillInfo,tdr_ulonglong ullRoleID)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;
	ROLESKILLINFO stRoleSkillInfo;
	
	memset(&stDBAsyn,0,sizeof(stDBAsyn));
	stDBAsyn.iCmd = DB_ROLE_SKILL_INFO_SELECT;
	memcpy(&stDBAsyn.AsynCmd.stDBRoleSkillInfo,pstDBRoleSkillInfo,sizeof(stDBAsyn.AsynCmd.stDBRoleSkillInfo));
	
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, "RoleSkillInfo", -1, &stDBAsyn);
	
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	sprintf(pstSelectReq->OperInfo.WhereDef, "where RoleID=%llu",ullRoleID);
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 1;

	memset(&stRoleSkillInfo, 0, sizeof(stRoleSkillInfo));
	stRoleSkillInfo.RoleID = ullRoleID;
			
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRoleSkillInfo;
	stMetaData.iBuff = sizeof(stRoleSkillInfo);

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}
	return 0;
}

//更新skillinfo   回数据库
int z_db_update_skill_info(ZONESVRENV* pstEnv, ROLESKILLINFO *pstRoleSkillInfo)
{
	TDRDATA stMetaData;
	TDRDATA stNetData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ,
		             "RoleSkillInfo", -1, NULL);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	sprintf(&pstUpdateReq->OperInfo.WhereDef[0], "where RoleID=%llu", pstRoleSkillInfo->RoleID);
	pstUpdateReq->DoInsert = 0;
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pstRoleSkillInfo;
	stMetaData.iBuff = sizeof(*pstRoleSkillInfo);
	
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		                      pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if(0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}
	return 0;
}

int z_db_role_skill_info_res(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes,DBAsyn *pstDbsyn)
{
	if(pstDbsyn == NULL)
	{
		return -1;
	}
	
	DBRoleSkillInfo *pstDBRoleSkillInfo = NULL;
	pstDBRoleSkillInfo = &pstDbsyn->AsynCmd.stDBRoleSkillInfo;
	
	if(NULL == pstDBRoleSkillInfo)
	{
		return -1;
	}
	
	switch(pstDBRoleSkillInfo->iCmd)
	{
		case DB_SKILL_INFO_DELETE:
			z_db_skill_info_del(pstEnv,pstSelectRes,&pstDBRoleSkillInfo->AsynRoleSkill.stSkillInfoDelete);
			break;
		default:
			break;
	}
	
	return 0;
}

int z_db_skill_info_del(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes,SkillInfoDelete *pstSkillInfoDelete)
{
	ROLESKILLINFO stRoleSkillInfo;
	SKILLINFO *pstSkillInfo = NULL;
	int i;
	int iSkillNum = 0;
	int j;
	int flag = 0;
	
	if (1 != pstSelectRes->SelectResult.ResultNum)
	{
		return -1;
	}

	if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stRoleSkillInfo, sizeof(stRoleSkillInfo)))
	{
		return -1;
	}
	
	if (stRoleSkillInfo.RoleID != pstSkillInfoDelete->ullRoleID)
	{
		return -1;
	}

	if(pstSkillInfoDelete->iDeleteNum == 0 || pstSkillInfoDelete->iDeleteNum > 6)
	{
		return -1;
	}
	
	pstSkillInfo = &stRoleSkillInfo.SkillInfo;
	
	for(j=0;j<pstSkillInfoDelete->iDeleteNum;j++)
	{	
		iSkillNum = pstSkillInfo->SkillNum;
		for(i=iSkillNum-1;i>=0;i--)
		{
			if(pstSkillInfo->SkillList[i].SkillID == pstSkillInfoDelete->stSkillInfoDeleteData[j].SkillID)
			{
				pstSkillInfo->SkillNum--;
				if(pstSkillInfo->SkillNum != i)
				{
					memcpy(&pstSkillInfo->SkillList[i],&pstSkillInfo->SkillList[pstSkillInfo->SkillNum],sizeof(pstSkillInfo->SkillList[i]));
				}

				flag = 1;
				break;
			}
		}
	}

	if(flag == 1)
	{
		z_db_update_skill_info(pstEnv, &stRoleSkillInfo);
	}
	
	return 0;
}

