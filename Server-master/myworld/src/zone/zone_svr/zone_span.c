/*
**  @file $RCSfile: zone_span.c,v $
**  general description of this module
**  $Id: zone_span.c,v 1.83 2014/07/14 06:07:56 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/07/14 06:07:56 $
**  @version $Revision: 1.83 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_clt.h"
#include "zone_attr.h"
#include "zone_package.h"
#include "zone_svr.h"
#include "zone_arm.h"
#include "zone_map.h"
#include "zone_safelock.h"
#include "zone_oplog.h"
#include "zone_player.h"
#include "zone_task.h"
#include "zone_combat.h"
#include "zone_move.h"
#include "zone_battle.h"
#include "zone_span.h"
#include "zone_db.h"
#include "zone_skill.h"
#include "zone_status.h"
#include "zone_spansvr.h"
#include "zone_shop.h"
#include "zone_collect.h"
#include "zone_misc.h"
#include "zone_pet.h"
#include "zone_machine.h"
#include "zone_tmp.h"
#include "zone_subject.h"
#include "zone_exchg.h"
#include "zone_stall.h"
#include "zone_autopworld.h"
#include "zone_sparta.h"
#include "zone_err.h"
#include "zone_range.h"
#include "zone_baoxiang.h"
#include "zone_coroutine.h"
#include "zone_clan.h"
#include "zone_clan_pet.h"
#include "zone_span_strong.h"
#include "zone_itemlimit.h"
#include "zone_boss.h"


//master->other 清理掉用户
int player_span_role_clean(ZONESVRENV *pstEnv, int iUin, tdr_ulonglong ullRoleID, int iDstID);

//Step 2 角色切线前数据回写请求
int player_span_chg_svr_update_req(ZONESVRENV *pstEnv, Player *pstPlayer);

//master: Step 3 登录线路请求
int player_span_login_req(ZONESVRENV *pstEnv, Player *pstPlayer,  SSSPANDSTDATA *pstSpanDstInfo, int iDstID);

//other: Step 3 登录请求处理
int player_span_login2(ZONESVRENV *pstEnv, Player *pstPlayer, SSSPANDSTDATA *pstSpanDstInfo, int iSrcID);

int player_span_chg_svr_update_in(ZONESVRENV *pstEnv, Player *pstPlayer);

// 被保护的RoleData数据,这部分数据不应该被覆盖
typedef struct tagProtectedData
{
	char szRoleName[ROLE_NAME_LEN];
	tdr_ulonglong ullTeamID;
	int iTeamIdx;
	CLANINFO stClanInfo;
	unsigned char ucBlackNum;
    	ROLEENEMYMEMBER astBlackMember[MAX_BLACK_NUM];
    	ROLEBUDDY stBuddy;
    	ROLEENEMY stEnemy;
}ProtectedData;

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
		return 1; // 查找到
	}

	iAlloced = tmempool_alloc(pstEnv->pstTeamPool);
	if(iAlloced < 0)
	{
		return -1; // 内存耗尽
	}

	pstFound = tmempool_get(pstEnv->pstTeamPool, iAlloced);
	memset(pstFound, 0, sizeof(*pstFound));

	pstFound->iTeamIdx = iAlloced;
	pstFound->stTeamInfo.TeamID = ullID;

	if(team_shadow_insert(pstEnv, pstFound) < 0)
	{
		return -1; // key冲突了
	}
	*ppstZoneTeam = pstFound;
	return 0; // 新建了一个
}

//根据Role的分配规则取出worldid
int role_world_id_get(Player *pstPlayer)
{
	unsigned short unWorldID;

	if(pstPlayer->stRoleData.WorldID > 0)
	{
		return pstPlayer->stRoleData.WorldID;
	}
	
	unWorldID = ((unsigned short *)&(pstPlayer->stRoleData.RoleID))[2]; 

	unWorldID = unWorldID << 6;
	unWorldID = unWorldID >> 9;
	return unWorldID;
}

//是不是角色所属主线服务器
int is_my_master_svr(int iID, Player *pstPlayer)
{
	unsigned char ucEntity;
	int iWorldID;

	GET_FUNC_ENTITY(ucEntity, iID);
	if(ucEntity != FUNC_ZONE_SVR)
	{
		return 0;
	}
	
	GET_WORLD_ID(iWorldID, iID);
	if(iWorldID == role_world_id_get(pstPlayer))
	{
		return 1;
	}

	return 1;
}

//master: 是否报名参加跨服战场
int is_player_apply_battle(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	if(!is_my_master_svr(pstEnv->pstAppCtx->iId, pstPlayer))
	{
		return 0;
	}
	
	if(pstPlayer->stOnline.stSpanOnline.iStat == SPAN_STAT_CONNECT)
	{
		if(pstPlayer->stOnline.stSpanOnline.stInfo.stConnect.iDstID == pstEnv->iBattlePveBattleID)
		{
			return 1;
		}
	}
	else if (pstPlayer->stOnline.stSpanOnline.stApply.iBattleApply == SPAN_APPLY_ING)
	{
		return 1;
	}
	
	return 0;
}

//master: 是否真身在跨服野外
int is_player_in_battle_pvp_svr(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	if(!is_my_master_svr(pstEnv->pstAppCtx->iId, pstPlayer))
	{
		return 0;
	}
	
	if(pstPlayer->stOnline.stSpanOnline.iStat != SPAN_STAT_CONNECT)
	{
		return 0;
	}

	if(pstPlayer->stOnline.stSpanOnline.stInfo.stConnect.iDstID == pstEnv->iBattlePvpID)
	{
		return 1;
	}
	
	return 0;
}

int is_player_in_span_svr(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	if(!is_my_master_svr(pstEnv->pstAppCtx->iId, pstPlayer))
	{
		return 0;
	}
	
	if(pstPlayer->stOnline.stSpanOnline.iStat != SPAN_STAT_CONNECT)
	{
		return 0;
	}
	
	return 1;
}

// 将战场中的角色名字 "服%d$%s",的$前缀去掉
int battle_rolename_remove_prefix(char *pszFullName)
{
	int iLen = 0;
	char *pszReal = strstr(pszFullName, BATTLE_NAME_SYMBOL);
	if(!pszReal)
	{
		return 0;
	}
	++pszReal; // 跳过那个$符号
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
	++pszReal1; // 跳过那个$符号
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

	++pszReal; // 跳过那个$符号
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
	snprintf(szBattleName,sizeof(szBattleName),"w%d"BATTLE_NAME_SYMBOL,unWorldID);

	if(strcmp(szBattleName, szPlayerBattleFix) == 0)
	{
		return 0;
	}

	return -1;
}

int player_shadow_action_request(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 CSACTIONRES *pstActionRes)
{
	SSPKG stPkg;
	SSBATTLEMIRRORSYNC *pstReq = &stPkg.Body.BattleMirrorSync;
	TDRDATA stHost, stNet;
	LPTDRMETA pstMeta = tdr_get_meta_by_name(pstEnv->pstNetHan->pstCSProtoLib,
	                                         "CSActionRes");
	if(!pstMeta || pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
	{
		return -1;
	}

	if(pstPlayer->stOnline.stSpanOnline.iStat != SPAN_STAT_CONNECT)
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
	            pstPlayer->stOnline.stSpanOnline.stInfo.stConnect.iDstID);
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

	pstActionRes->ID = pstPlayer->iMemID; // 要替换成本地的Memory ID,切忌
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	return z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg,
	                            0);
}

int span_set_routing(ZONESVRENV* pstEnv, int iConnBusID, int iConnIdx, int iBusID, int iID)
{
	TFRAMEHEAD stReq;

	memset(&stReq, 0, sizeof(stReq));
	stReq.Cmd = TFRAMEHEAD_CMD_SETROUTING;
	stReq.ID = 0x7fffffff; // 填充一个不可能的值,但又不能是-1
	stReq.ConnIdx = iConnIdx;
 	stReq.CmdData.SetRouting.BusID = iBusID;
 	stReq.CmdData.SetRouting.ID = iID;

	tlog_info(pstEnv->pstLogCat, 0, 0, "z_set_routing_req iConnIdx=%d,iBusID=%d",iConnIdx, iBusID);

 	return tconnapi_send(pstEnv->iTconnapiHandle, iConnBusID, NULL, 0,
 	                     &stReq);
}

void z_save_protected_data(const ROLEDATA *pstRoleData,ProtectedData *pstSavedData)
{
	// 队伍信息
	pstSavedData->ullTeamID = pstRoleData->MiscInfo.TeamID;
	pstSavedData->iTeamIdx = pstRoleData->MiscInfo.TeamIdx;

	// 公会信息
	pstSavedData->stClanInfo = pstRoleData->MiscInfo.ClanInfo;

	// 角色名
	STRNCPY(pstSavedData->szRoleName, pstRoleData->RoleName,
	        CCH(pstSavedData->szRoleName));

	// 关系系统
	pstSavedData->ucBlackNum = pstRoleData->MiscInfo.BlackNum;
	memcpy(pstSavedData->astBlackMember, pstRoleData->MiscInfo.BlackMember,
	       sizeof(pstSavedData->astBlackMember[0]) * pstSavedData->ucBlackNum);
	memcpy(&pstSavedData->stEnemy, &pstRoleData->MiscInfo.Enemy,
	       sizeof(pstSavedData->stEnemy));
	memcpy(&pstSavedData->stBuddy, &pstRoleData->Buddy,
	       sizeof(pstSavedData->stBuddy));
}

void z_restore_protected_data(const ProtectedData *pstSavedData,ROLEDATA *pstRoleData)
{
	// 队伍信息
	pstRoleData->MiscInfo.TeamID = pstSavedData->ullTeamID;
	pstRoleData->MiscInfo.TeamIdx = pstSavedData->iTeamIdx;

	// 公会信息
	pstRoleData->MiscInfo.ClanInfo = pstSavedData->stClanInfo;

	// 角色名
	STRNCPY(pstRoleData->RoleName, pstSavedData->szRoleName,
	        CCH(pstRoleData->RoleName));

	// 关系系统
	pstRoleData->MiscInfo.BlackNum = pstSavedData->ucBlackNum;
	memcpy(pstRoleData->MiscInfo.BlackMember, pstSavedData->astBlackMember,
	       sizeof(pstRoleData->MiscInfo.BlackMember[0]) *
	       pstSavedData->ucBlackNum);
	memcpy(&pstRoleData->MiscInfo.Enemy, &pstSavedData->stEnemy,
	       sizeof(pstRoleData->MiscInfo.Enemy));
	memcpy(&pstRoleData->Buddy, &pstSavedData->stBuddy,
	       sizeof(pstRoleData->Buddy));
}

//填充一下角色转移信息
int player_span_role_data(ZONESVRENV* pstEnv, Player* pstPlayer, SSSPANROLEDATA* pstSpanRoleData)
{
	pstSpanRoleData->RoleData = pstPlayer->stRoleData;
	pstSpanRoleData->ConnInfo.Ver = pstPlayer->stConnectInfo.chVer;
	pstSpanRoleData->ConnInfo.ExtraType= pstPlayer->stConnectInfo.chExtraType;
	memcpy(&pstSpanRoleData->ConnInfo.ExtraInfo, &pstPlayer->stConnectInfo.stExtraInfo, sizeof(pstSpanRoleData->ConnInfo.ExtraInfo));
	pstSpanRoleData->ConnInfo.TimeStampType = pstPlayer->stConnectInfo.chTimeStampType;
	pstSpanRoleData->ConnInfo.TimeStamp = pstPlayer->stConnectInfo.stTimeStamp;
	pstSpanRoleData->ConnInfo.ConnIdx = pstPlayer->stConnectInfo.iConnIdx;
	STRNCPY(pstSpanRoleData->AccountLoginName, pstPlayer->szAccount, CCH(pstSpanRoleData->AccountLoginName));
	pstSpanRoleData->CltProtoVer = pstPlayer->unCltProVer;
	pstSpanRoleData->AccSilenceEnd = pstPlayer->iAccSilenceEnd;
	pstSpanRoleData->GMLimitPerm = pstPlayer->GMLimitPerm;
	
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

//角色的zone_svr的BusID
int player_master_busid_get(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int iWorldID;
	int iBusID;
	
	iWorldID = role_world_id_get(pstPlayer);
	iBusID = region_world_func_inst_aton_busid(pstEnv->ucRegionID, iWorldID, FUNC_ZONE_SVR, 1);
	return iBusID;
}

//角色的zone_conn的BusID
int player_conn_busid_get(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int iWorldID;
	int iBusID;
	
	iWorldID = role_world_id_get(pstPlayer);
	iBusID = region_world_func_inst_aton_busid(pstEnv->ucRegionID, iWorldID, FUNC_ZONE_CONN, 1);
	return iBusID;
}

//是不是同一服的玩家。不支持跨区
int is_together_world(ZONESVRENV *pstEnv, Player *pstPlayer1, Player *pstPlayer2)
{
	int iWorldID;

	iWorldID = role_world_id_get(pstPlayer1);
	if(iWorldID == role_world_id_get(pstPlayer2))
	{
		return 1;
	}
	
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

//获得野外场景所在进程Bus ID
int span_map_dstid_get(ZONESVRENV *pstEnv, Player *pstPlayer, int iMapID)
{
	MapIndex *pstMapIndex;
	int iDstID = -1;
	
	pstMapIndex	= z_mappos_index(pstEnv->pstMapObj, NULL, iMapID);
	if( !pstMapIndex )
		return -1;

	//只有到跨服野外和本服，没其他多进程的场景
	if(pstMapIndex->stMapDef.Span	== 0)
	{
		iDstID = player_master_busid_get(pstEnv, pstPlayer);
	}	
	else
	{
		iDstID = pstEnv->iBattlePvpID;
	}
	
	return iDstID;
}

//跨服进程复制角色
Player* player_span_new_role(ZONESVRENV * pstEnv, SSSPANROLEDATA *pstSpanRoleData, int *piResult)
{
	int iUin=pstSpanRoleData->RoleData.Uin;
	UinCache *pstUinCache = NULL;
	UinCache stUinHashData;
	Player *pstPlayer;
	unsigned short unWorldID;
	NameMemID stNameMemID, * pstNameMemID;

	stUinHashData.iUin = iUin;
	pstUinCache = (UinCache *)sht_find(pstEnv->pstShtUinCache,&stUinHashData,UinHashCmp,UinHashCode);
	pstPlayer = player_get_by_uin(pstEnv, iUin);
	if(pstPlayer )
	{
		if(pstPlayer->stRoleData.RoleID != pstSpanRoleData->RoleData.RoleID)
		{
			//tlog_error(pstEnv->pstLogCat, iUin, 0, "player_battle_acc_login error the same acc");
			*piResult = -3;
			return NULL;
		}
		else if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
		{
			*piResult = -1;
			return NULL;
		}
	}
	else
	{
		if (pstEnv->pstPlayerPool->iUsed >= MAX_PLAYER_SOFT )
		{
			*piResult = -2;
			return NULL;
		}
		if(pstUinCache)
		{
			pstPlayer = player_alloc(pstEnv->pstAppCtx, pstEnv, iUin, pstSpanRoleData->AccountLoginName, pstUinCache);
		}
		else
		{
			pstPlayer = player_alloc(pstEnv->pstAppCtx, pstEnv, iUin, pstSpanRoleData->AccountLoginName, NULL);
		}
		
		if (NULL == pstPlayer)
		{
			tlog_error(pstEnv->pstLogCat, iUin, 0, "player_battle_acc_login error pstPlayerNew is NULL");
			*piResult = -1;
			return NULL;
		}

		pstPlayer->stRoleData.Uin = pstSpanRoleData->RoleData.Uin;
	}

	*piResult = 0;
	memset(&stNameMemID,0,sizeof(stNameMemID));

	//pstPlayer->eStatus = PLAYER_STATUS_ACC_LOGIN;
	//一个不存在的状态，保护Player, 不做任何事情
	pstPlayer->eStatus = PLAYER_STATUS_ROLE_NOTHING;
	
	pstPlayer->tActive = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstPlayer->stConnectInfo.chVer = pstSpanRoleData->ConnInfo.Ver;
	pstPlayer->stConnectInfo.chCmd = TFRAMEHEAD_CMD_INPROC;
	pstPlayer->stConnectInfo.chExtraType = pstSpanRoleData->ConnInfo.ExtraType;
	memcpy(&pstPlayer->stConnectInfo.stExtraInfo, &pstSpanRoleData->ConnInfo.ExtraInfo, sizeof(pstPlayer->stConnectInfo.stExtraInfo));
	pstPlayer->stConnectInfo.chTimeStampType = pstSpanRoleData->ConnInfo.TimeStampType;
	memcpy(&pstPlayer->stConnectInfo.stTimeStamp, &pstSpanRoleData->ConnInfo.TimeStamp, sizeof(pstPlayer->stConnectInfo.stTimeStamp));
	pstPlayer->stConnectInfo.iConnIdx = pstSpanRoleData->ConnInfo.ConnIdx;
	pstPlayer->stConnectInfo.iID = pstPlayer->iIdx;
	pstPlayer->stOnline.bConnLost = 0;
	pstPlayer->GMLimitPerm = pstSpanRoleData->GMLimitPerm;
	pstPlayer->iAccSilenceEnd = pstSpanRoleData->AccSilenceEnd;
	pstPlayer->unCltProVer = pstSpanRoleData->CltProtoVer;
	pstPlayer->stRoleData = pstSpanRoleData->RoleData;

	// 这里修改下玩家的名字
	stNameMemID.iID = pstPlayer->iMemID;
	unWorldID = role_world_id_get(pstPlayer);
	snprintf(stNameMemID.szName,sizeof(stNameMemID.szName),"w%d"BATTLE_NAME_SYMBOL"%s",unWorldID, pstPlayer->stRoleData.RoleName);
	memset(pstPlayer->stRoleData.RoleName,0,sizeof(pstPlayer->stRoleData.RoleName));
	STRNCPY( pstPlayer->stRoleData.RoleName, stNameMemID.szName, sizeof(stNameMemID.szName));	
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
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "sht insert name %d %s player fail", 
					pstPlayer->stRoleData.Uin,  pstPlayer->stRoleData.RoleName);
		}
	}

	return pstPlayer;
}

/*
	报名(自动组队，战场)等跨服系统前的虚假角色，用来处理一些排队逻辑
*/
int player_span_apply_req(ZONESVRENV *pstEnv, Player *pstPlayer, int iDstType)
{
	int iDstID = 0;
	SSPKG stSSPkg;
	SSSPANROLEAPPLYREQ *pstSpanApplyReq = &stSSPkg.Body.SpanRoleApplyReq;
	SpanOL *pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;

	switch(iDstType)
	{
		case SPAN_DST_TYPE_SPARTA:
			if(is_battle_sparta_svr(pstEnv))
			{
				return -1;
			}
			
			if(pstSpanOnline->stApply.iSpartaApply!= SPAN_APPLY_NONE)
			{
				//先限制的严一点，报过名就不让在报
				return -1;
			}
			
			iDstID = pstEnv->iBattleSpartaID;
			pstSpanOnline->stApply.iSpartaApply = SPAN_APPLY_ING;
			break;
		case SPAN_DST_TYPE_BATTLE:
			if(pstSpanOnline->stApply.iBattleApply!= SPAN_APPLY_NONE)
			{
				//先限制的严一点，报过名就不让在报
				return 0;
			}
			
			iDstID = pstEnv->iBattlePveBattleID;
			pstSpanOnline->stApply.iBattleApply = SPAN_APPLY_ING;
			break;
		default:
			return -1;
			break;
	}

	if (0 == iDstID)
	{
		return -1;
	}
	
	//已经在目的地了就不应该发起这个消息，而是直接报名
	if(iDstID == pstEnv->pstAppCtx->iId)
	{
		return 0;
	}

	pstSpanApplyReq->MsgSrcID = pstEnv->pstAppCtx->iId;
	pstSpanApplyReq->MsgDstID = iDstID;

	player_span_role_data(pstEnv, pstPlayer, &pstSpanApplyReq->SpanData);
	Z_SSHEAD_INIT(&stSSPkg.Head, SPAN_ROLE_APPLY_REQ);
	z_ssmsg_send(pstEnv, &stSSPkg, iDstID);
	return 0;
}

// 报名进程处理
int player_span_apply(ZONESVRENV * pstEnv, SSSPANROLEAPPLYREQ *pstSpanRoleApplyReq)
{
	int iRet;
	Player *pstPlayer;
	SSPKG stSSPkg;
	SSSPANROLEAPPLYRES *pstSpanApplyRes = &stSSPkg.Body.SpanRoleApplyRes;
	SpanOL *pstSpanOnline;

	//这个消息不会在Zone\跨服野外 处理
	if(INST_MODE_BATTLE_SVR != pstEnv->iInstMode)
	{		
		return -1;
	}
	
	if (pstEnv->cShutdown)
	{
		iRet = ROLE_APPLY_ERR_SYS;
		goto _final;
	}

	pstPlayer = player_span_new_role(pstEnv, &pstSpanRoleApplyReq->SpanData, &iRet);
	if(!pstPlayer)
	{
		if(iRet == -3)
		{
			iRet = ROLE_APPLY_ERR_SAME;
		}
		else if(iRet == -2)
		{
			iRet = ROLE_APPLY_ERR_FULL;
		}
		else
		{
			iRet = ROLE_APPLY_ERR_SYS;
		}
		goto _final;
	}
	
	iRet = ROLE_APPLY_SUCC;

	pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
	pstSpanOnline->iStat = SPAN_STAT_APPLY;
	pstSpanOnline->stInfo.stApply.ullRoleID = pstSpanRoleApplyReq->SpanData.RoleData.RoleID;
_final:

	pstSpanApplyRes->Result = iRet;
	pstSpanApplyRes->RoleID = pstSpanRoleApplyReq->SpanData.RoleData.RoleID;
	pstSpanApplyRes->Uin = pstSpanRoleApplyReq->SpanData.RoleData.Uin;
	pstSpanApplyRes->MsgDstID = pstSpanRoleApplyReq->MsgSrcID;
	pstSpanApplyRes->MsgSrcID = pstEnv->pstAppCtx->iId;
	
	Z_SSHEAD_INIT(&stSSPkg.Head, SPAN_ROLE_APPLY_RES);
	z_ssmsg_send(pstEnv, &stSSPkg, pstSpanRoleApplyReq->MsgSrcID);
	
	return 0;
}

//报名消息返回
int player_span_apply_res(ZONESVRENV *pstEnv, SSSPANROLEAPPLYRES *pstSpanApplyRes)
{	
	Player *pstPlayer;
	SpanOL *pstSpanOnline;

	pstPlayer = player_get_by_uin(pstEnv, pstSpanApplyRes->Uin);
	if(!pstPlayer)
	{
		return -1;
	}

	if(pstSpanApplyRes->Result == ROLE_APPLY_SUCC)
	{
		return 0;
	}

	pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
	if(pstEnv->iBattleSpartaID == pstSpanApplyRes->MsgSrcID)
	{
		pstSpanOnline->stApply.iSpartaApply = 0;
	}
	else if(pstEnv->iBattlePveBattleID == pstSpanApplyRes->MsgSrcID)
	{
		pstSpanOnline->stApply.iBattleApply = 0;
	}

	if(pstSpanApplyRes->Result == ROLE_APPLY_ERR_SAME)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
							ZONEERR_SPAN203);
	}
	else if(pstSpanApplyRes->Result == ROLE_APPLY_ERR_FULL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
							ZONEERR_SPAN204);
	}
	
	return 0;
}

/*
	跨服步骤 : Step1
	预先站位，对于报名类的跨服也必须经过此步骤，
	不可直接跨服，便于一些逻辑能顺序执行完
*/
int player_span_hold_req(ZONESVRENV *pstEnv, Player *pstPlayer, SSSPANDSTDATA *pstSpanDstInfo)
{
	int iDstID = 0;
	SSPKG stSSPkg;
	SSSPANROLEHOLDREQ *pstSpanHoldReq = &stSSPkg.Body.SpanRoleHoldReq;
	SpanOL *pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;

	//if(0 > z_player_goto_check(pstEnv, pstPlayer, 0, NULL))
	//{
	//	return -1;
	//}

	if(pstPlayer->stOnline.bWaitRoleLogout > 0)
	{
		return -1;
	}
	
	switch(pstSpanDstInfo->DstType)
	{
		
		case SPAN_DST_TYPE_SPARTA:
			iDstID = pstEnv->iBattleSpartaID;
			break;
		case SPAN_DST_TYPE_BATTLE:
			iDstID = pstEnv->iBattlePveBattleID;
			break;
		case SPAN_DST_TYPE_MAP:
			iDstID = span_map_dstid_get(pstEnv, pstPlayer, pstSpanDstInfo->Data.Map.MapID);
			if(iDstID <= 0)
			{
				return -1;
			}
			break;
		default:
			break;
	}

	if (0 == iDstID)
	{
		return -1;
	}

	if(iDstID == pstEnv->pstAppCtx->iId)
	{
		return -1;
	}

	if(pstSpanOnline->iStat > 0)
	{
		//已经在申请切线了,如果是同一个线路就放行
		if(pstSpanOnline->iStat == SPAN_STAT_HOLD_REQ)
		{
			if(pstSpanOnline->stInfo.stHoldReq.iDstID != iDstID)
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}

	pstSpanOnline->iStat = SPAN_STAT_HOLD_REQ;
	pstSpanOnline->stInfo.stHoldReq.iDstID = iDstID;
	pstSpanOnline->stInfo.stHoldReq.ullRoleID = pstPlayer->stRoleData.RoleID;
	pstSpanOnline->stInfo.stHoldReq.stDstData = *pstSpanDstInfo;

	pstSpanHoldReq->MsgSrcID = pstEnv->pstAppCtx->iId;
	pstSpanHoldReq->MsgDstID = iDstID;
	pstSpanHoldReq->DstData = *pstSpanDstInfo;
	player_span_role_data(pstEnv, pstPlayer, &pstSpanHoldReq->SpanData);
	Z_SSHEAD_INIT(&stSSPkg.Head, SPAN_ROLE_HOLD_REQ);
	z_ssmsg_send(pstEnv, &stSSPkg, iDstID);
	return 0;
}

/*
	跨服步骤 : Step1
	目的地预站位&& 验证是否合法
*/
int player_span_hold(ZONESVRENV * pstEnv, SSSPANROLEHOLDREQ *pstSpanRoleHoldReq)
{
	Player *pstPlayer;
	SSPKG stSSPkg;
	SpanOL *pstSpanOnline;
	SSSPANROLEHOLDRES *pstSpanHoldRes = &stSSPkg.Body.SpanRoleHoldRes;
	int iRet = ROLE_HOLD_SUCC;

	if (pstEnv->cShutdown)
	{
		iRet = ROLE_HOLD_ERR_SYS;
		goto _final;
	}

	pstPlayer = player_get_by_uin(pstEnv, pstSpanRoleHoldReq->SpanData.RoleData.Uin);
	if(pstPlayer)
	{
		if(is_my_master_svr(pstEnv->pstAppCtx->iId, pstPlayer))
		{
			iRet = ROLE_HOLD_SUCC;
			goto _final;
		}
	}

	//验证
	switch(pstSpanRoleHoldReq->DstData.DstType)
	{
		case SPAN_DST_TYPE_SPARTA:
			{
				//Player *pstPlayer;
				ZONEPWORLDINST *pstPworldInst;
				ZONEPWORLD *pstPworld;

				if(!is_battle_sparta_svr(pstEnv))
				{
					return -1;
				}

				pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstSpanRoleHoldReq->DstData.Data.Sparta.PworldInstID);
				if(NULL == pstPworldInst)
				{
					iRet = ROLE_HOLD_ERR_SYS;
					goto _final;
				}

				pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
				if (!pstPworld)
				{
					iRet = ROLE_HOLD_ERR_SYS;
					goto _final;	
				}

				if (pstPworld->stPworldDef.StrongMapID > 0)
				{
					pstPlayer = player_span_new_role(pstEnv, &pstSpanRoleHoldReq->SpanData, &iRet);
					if(!pstPlayer)
					{
						if(iRet == -3)
						{
							iRet = ROLE_HOLD_ERR_SAME;
						}
						else if(iRet == -2)
						{
							iRet = ROLE_HOLD_ERR_FULL;
						}
						else
						{
							iRet = ROLE_HOLD_ERR_SYS;
						}
						goto _final;
					}
				}
				else
				{
					pstPlayer = player_get_by_uin(pstEnv, pstSpanRoleHoldReq->SpanData.RoleData.Uin);
					if(!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
					{
						iRet = ROLE_HOLD_ERR_SYS;
						goto _final;
					}
				}
				

				

				if(pstPworldInst->ullPworldWID != pstSpanRoleHoldReq->DstData.Data.Sparta.PworldWID)
				{
					iRet = ROLE_HOLD_ERR_SYS;
					goto _final;
				}

				//TODO: 验证
			}
			break;

		case SPAN_DST_TYPE_BATTLE:
			{
				RolePworldQueue *pstRoleQueue;
				ZONEPWORLDINST *pstPworldInst;
				SSSPANPWORLDINFO *pstSpanPworldInfo = &pstSpanRoleHoldReq->DstData.Data.Battle;

				if(!is_battle_pve_battle_svr(pstEnv))
				{
					return -1;
				}
				
				pstPlayer = player_get_by_uin(pstEnv, pstSpanRoleHoldReq->SpanData.RoleData.Uin);
				if(!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
				{
					iRet = ROLE_HOLD_ERR_SYS;
					goto _final;
				}

				pstRoleQueue = z_get_player_online_queue(pstEnv, pstPlayer, pstSpanPworldInfo->PworldID);
				if(NULL == pstRoleQueue || (pstRoleQueue->iQueueType != PWORLD_WAIT_QUEUE &&
					pstRoleQueue->iQueueType != PWORLD_CLAN_QUEUE))
				{
					iRet = ROLE_HOLD_ERR_SYS;
					goto _final;
				}

				pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstRoleQueue->iPworldInstID);
				if(NULL == pstPworldInst)
				{
					iRet = ROLE_HOLD_ERR_SYS;
					goto _final;
				}

				if (battle_enter_up_time_limit(pstEnv,pstPlayer, 1) < 0)
				{
					iRet = ROLE_HOLD_ERR_SYS;
					goto _final;
				}
			}
			break;
		case SPAN_DST_TYPE_MAP:
			{
				int iDstID;
				MapIndex *pstMapIdx;
				SSSPANMAPINFO *pstSpanMapInfo = &pstSpanRoleHoldReq->DstData.Data.Map;

				pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, pstSpanMapInfo->MapID);
				if (NULL == pstMapIdx)
				{
					iRet = ROLE_HOLD_ERR_SYS;
					goto _final;
				}

				if (!z_valid_pos(pstEnv->pstMapObj, pstSpanMapInfo->MapID, NULL, &pstSpanMapInfo->Pos))
				{
					iRet = ROLE_HOLD_ERR_SYS;
					goto _final;
				}

				pstPlayer = player_span_new_role(pstEnv, &pstSpanRoleHoldReq->SpanData, &iRet);
				if(!pstPlayer)
				{
					if(iRet == -3)
					{
						iRet = ROLE_HOLD_ERR_SAME;
					}
					else if(iRet == -2)
					{
						iRet = ROLE_HOLD_ERR_FULL;
					}
					else
					{
						iRet = ROLE_HOLD_ERR_SYS;
					}
					goto _final;
				}

				iDstID = span_map_dstid_get(pstEnv, pstPlayer, pstSpanMapInfo->MapID);
				if(iDstID != pstEnv->pstAppCtx->iId)
				{
					iRet = ROLE_HOLD_ERR_SYS;
					goto _final;
				}

			}
			break;
	}

	if (pstPlayer)
	{
		pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
		pstSpanOnline->iStat = SPAN_STAT_HOLD;
		pstSpanOnline->stInfo.stHold.iSrcID = pstSpanRoleHoldReq->MsgSrcID;
		pstSpanOnline->stInfo.stHold.ullRoleID = pstSpanRoleHoldReq->SpanData.RoleData.RoleID;
	}
	
_final:

	pstSpanHoldRes->MsgDstID = pstSpanRoleHoldReq->MsgSrcID;
	pstSpanHoldRes->MsgSrcID = pstEnv->pstAppCtx->iId;
	pstSpanHoldRes->Result = iRet;
	pstSpanHoldRes->RoleID = pstSpanRoleHoldReq->SpanData.RoleData.RoleID;
	pstSpanHoldRes->Uin = pstSpanRoleHoldReq->SpanData.RoleData.Uin;

	Z_SSHEAD_INIT(&stSSPkg.Head, SPAN_ROLE_HOLD_RES);
	z_ssmsg_send(pstEnv, &stSSPkg, pstSpanRoleHoldReq->MsgSrcID);
	return 0;
}

/*
	跨服步骤 : Step1
	目的地预站位返回消息，站位位置成功则转入Step2 更新角色数据
*/
int player_span_hold_res(ZONESVRENV * pstEnv, SSSPANROLEHOLDRES *pstSpanRoleHoldRes)
{
	Player *pstPlayer;
	SpanOL *pstSpanOnline;
	
	pstPlayer = player_get_by_uin(pstEnv, pstSpanRoleHoldRes->Uin);
	if(!pstPlayer)
	{
		return -1;
	}

	//验证一下消息是否是最新的请求
	pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
	if(pstSpanOnline->iStat != SPAN_STAT_HOLD_REQ ||
		pstSpanOnline->stInfo.stHoldReq.iDstID != pstSpanRoleHoldRes->MsgSrcID)
	{
		return -1;
	}

	if(pstSpanRoleHoldRes->RoleID != pstSpanOnline->stInfo.stHoldReq.ullRoleID)
	{
		return -1;
	}

	//出错就清理切线状态，正常游戏
	if(pstSpanRoleHoldRes->Result != ROLE_HOLD_SUCC)
	{
		goto _clear;
	}

	if(pstPlayer->stOnline.bWaitRoleLogout  > 0)
	{
		goto _clear;
	}

	//防止一些复制的情况
	//if(0 > z_player_goto_check(pstEnv, pstPlayer, 0, NULL))
	//{
	//	player_span_role_clean(pstEnv, pstSpanRoleHoldRes->Uin, pstSpanRoleHoldRes->RoleID, pstSpanRoleHoldRes->MsgSrcID);
	//	goto _clear;
	//}

	//偷了个懒， UPDATE 和 HOLD_REQ 是同样的一个结构体，不在赋值了
	pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
	pstSpanOnline->iStat = SPAN_STAT_UPDATE;
	//清理报名状态，如果不是进战场应该还需要退一下
	if(pstSpanOnline->stApply.iBattleApply == SPAN_APPLY_ING)
	{
		pstSpanOnline->stApply.iBattleApply = SPAN_APPLY_NONE;
	}

	if(pstSpanOnline->stApply.iSpartaApply == SPAN_APPLY_ING)
	{
		pstSpanOnline->stApply.iSpartaApply = SPAN_APPLY_NONE;
	}

	//清理下七七八八的状态
	player_span_state_clean(pstEnv, pstPlayer);

	// 切换地图,先清状态,再加状态
	if(pstPlayer->stOnline.cMiscFlag & MISC_FLAG_STATUS_CHGMAP_CLEAN)
	{
		Player_clean_matching_status(pstEnv->pstAppCtx, pstEnv, pstPlayer,STATUS_CHG_MAP_CLEAN,MISC_FLAG_STATUS_CHGMAP_CLEAN);
	}

	if(pstSpanOnline->stInfo.stUpdate.iDstID == pstEnv->iBattleSpartaID)
	{
		if (pstPlayer->stOnline.State & CS_STAT_DEAD)
		{	
			player_revive_svr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}
		else
		{
			player_status_clean_debuff(pstEnv, pstPlayer);
			player_revive_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer,
							pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax,
							pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax);
		}
	}

	//这个时候要离开地图
	z_player_leave_map(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1);
	
	// 需要立即回写角色数据，回写之后再操作
	player_span_chg_svr_update_req(pstEnv, pstPlayer);
	return 0;
	
_clear:
	if(pstPlayer)
	{
		if(pstSpanRoleHoldRes->Result == ROLE_HOLD_ERR_SAME)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
								ZONEERR_SPAN205);
		}
		
		//登录游戏登录不进来，可能是跨服副本关闭了。或者满员
		// TODO: 登录到ExitMapID
	}

	pstSpanOnline->iStat = 0;
	memset(&pstSpanOnline->stInfo, 0, sizeof(pstSpanOnline->stInfo));
	return 0;
}

//更新Roldata 同时也是hello 包
int player_span_role_update_req(ZONESVRENV *pstEnv, Player *pstPlayer, int iOpType)
{
	PlayerSyncOnline stSyncInfo;
	SSPKG stSSPkg;
	SSSPANROLEUPDATEREQ *pstUpdateReq = &stSSPkg.Body.SpanRoleUpdateReq;

	if(iOpType == CHG_SVR_ROLE_UPDATE)
	{
		pstUpdateReq->Type = SPAN_UPDATE_REQ_CHG_SVR;
		pstUpdateReq->DataDirty = 1;
	}
	else if(iOpType == SPAN_SVR_ROLE_KICK)
	{
		pstUpdateReq->Type = SPAN_UPDATE_REQ_KICK;
		pstUpdateReq->DataDirty = 0;
	}
	else
	{
		pstUpdateReq->Type = SPAN_UPDATE_REQ_NONE;
		pstUpdateReq->DataDirty = 0;
	}

	pstUpdateReq->Uin = pstPlayer->stRoleData.Uin;
	pstUpdateReq->RoleData = pstPlayer->stRoleData;	
	pstUpdateReq->MsgDstID = player_master_busid_get(pstEnv, pstPlayer);
	pstUpdateReq->MsgSrcID = pstEnv->pstAppCtx->iId;
	pstUpdateReq->Reason = pstPlayer->ucRoleLogoutReason;
		
	stSyncInfo.stDyn = pstPlayer->stOnline.stDyn;
	stSyncInfo.stFairyOnline = pstPlayer->stOnline.stFairyOnline;
	stSyncInfo.stHeti = pstPlayer->stOnline.stHeti;
	stSyncInfo.stPetOnline = pstPlayer->stOnline.stPetOnline;
	stSyncInfo.ullMiscFlag = pstPlayer->stOnline.cMiscFlag;
	stSyncInfo.cSafeLockFalg = pstPlayer->stOnline.cSafeLockFalg;
	stSyncInfo.iSkillSeq = pstPlayer->stOnline.stAtkInfo.iSkillSeq;
	stSyncInfo.iMoveSeq = pstPlayer->stOnline.iPosSeq;
	stSyncInfo.iSpanGoldRate = pstPlayer->stOnline.iSpanGoldRate;
	
	pstUpdateReq->ExtLen = sizeof(stSyncInfo);
	memcpy(pstUpdateReq->ExtData, &stSyncInfo, sizeof(stSyncInfo));
	
	Z_SSHEAD_INIT(&stSSPkg.Head, SPAN_ROLE_UPDATE_REQ);
	z_ssmsg_send(pstEnv, &stSSPkg, player_master_busid_get(pstEnv, pstPlayer));
	return 0;
}

//Step 2 角色切线前数据回写请求
int player_span_chg_svr_update_req(ZONESVRENV *pstEnv, Player *pstPlayer)
{	
	if(is_my_master_svr(pstEnv->pstAppCtx->iId, pstPlayer))
	{
		//主线无需再请求更新数据
		pstPlayer->stOnline.cDataDirty = DIRTY_IMPORTANT;	
		return player_span_chg_svr_update_in(pstEnv, pstPlayer);
	}

	z_db_role_update(pstEnv->pstAppCtx, pstEnv, pstPlayer, CHG_SVR_ROLE_UPDATE);
	return 0;
}

//master: Step 2 角色切线前数据回写数据库完成消息
int player_span_chg_svr_update_in(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	SSPKG stSSPkg;
	SSSPANROLEUPDATERES *pstUpdateRes = &stSSPkg.Body.SpanRoleUpdateRes;
	SpanOL *pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;

	//主线切线
	if(pstSpanOnline->iStat == SPAN_STAT_UPDATE)
	{
		//可以直接切线了，直接Step 3
		return player_span_login_req(pstEnv, pstPlayer, 
			&pstSpanOnline->stInfo.stUpdate.stDstData, pstSpanOnline->stInfo.stUpdate.iDstID);
		
	}
	//分线切线
	else if(pstSpanOnline->iStat == SPAN_STAT_CONNECT)
	{
		//通知当前分线把角色清理掉
		pstUpdateRes->MsgSrcID = pstEnv->pstAppCtx->iId;
		pstUpdateRes->MsgDstID = pstSpanOnline->stInfo.stConnect.iDstID;
		pstUpdateRes->Uin = pstPlayer->stRoleData.Uin;
		pstUpdateRes->RoleID = pstPlayer->stRoleData.RoleID;
		pstUpdateRes->Result = SPAN_ROLE_UPDATE_RES_CHG_SVR;
		Z_SSHEAD_INIT(&stSSPkg.Head, SPAN_ROLE_UPDATE_RES);
		z_ssmsg_send(pstEnv, &stSSPkg, pstSpanOnline->stInfo.stConnect.iDstID);
	}
	else
	{
		return -1;
	}

	return 0;
}

//Step 2 角色切线前数据回写后返回消息，清理用户
int player_span_chg_svr_update_res(ZONESVRENV *pstEnv, SSSPANROLEUPDATERES *pstUpdateRes)
{
	Player *pstPlayer;
	SpanOL *pstSpanOnline;
	SSPKG stSSPkg;
	SSSPANCHGSVR *pstChgSvr = &stSSPkg.Body.SpanChgSvr;

	pstPlayer = player_get_by_uin(pstEnv, pstUpdateRes->Uin);
	if(!pstPlayer)
	{
		return -1;
	}

	//防止出错，只会是分线才清理用户
	if(is_my_master_svr(pstEnv->pstAppCtx->iId, pstPlayer))
	{
		return -1;
	}

	//确保是正确的消息
	pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
	if(pstSpanOnline->iStat != SPAN_STAT_UPDATE ||
		pstSpanOnline->stInfo.stUpdate.ullRoleID != pstUpdateRes->RoleID)
	{
		return -1;
	}

	pstChgSvr->MsgDstID = player_master_busid_get(pstEnv, pstPlayer);
	pstChgSvr->MsgSrcID = pstEnv->pstAppCtx->iId;
	pstChgSvr->Uin = pstPlayer->stRoleData.Uin;
	pstChgSvr->RoleID = pstPlayer->stRoleData.RoleID;
	pstChgSvr->DstID = pstSpanOnline->stInfo.stUpdate.iDstID;
	pstChgSvr->DstData = pstSpanOnline->stInfo.stHoldReq.stDstData;
	Z_SSHEAD_INIT(&stSSPkg.Head, SPAN_CHG_SVR);
	z_ssmsg_send(pstEnv, &stSSPkg, player_master_busid_get(pstEnv, pstPlayer));

	//暴力退出
	z_player_leave_map(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1);
	z_free_player(pstEnv->pstAppCtx, pstEnv, pstPlayer);

	return 0;
}

//master: Step 3 登录线路请求
int player_span_login_req(ZONESVRENV *pstEnv, Player *pstPlayer,  SSSPANDSTDATA *pstSpanDstInfo, int iDstID)
{
	SpanOL *pstSpanOnline;
	PlayerSyncOnline stSyncInfo;
	ZoneTeam *pstZoneTeam;
	SSPKG stSSPkg;
	SSSPANROLELOGINREQ *pstLoginReq = &stSSPkg.Body.SpanRoleLoginReq;

	if(iDstID == pstEnv->pstAppCtx->iId)
	{
		return player_span_login2(pstEnv, pstPlayer, pstSpanDstInfo, iDstID);
	}

	//实际上是为了处理Online->RoleData
	z_db_role_update(pstEnv->pstAppCtx,  pstEnv, pstPlayer,0);

	pstLoginReq->MsgDstID = iDstID;
	pstLoginReq->MsgSrcID = pstEnv->pstAppCtx->iId;
	pstLoginReq->DstData = *pstSpanDstInfo;
	pstLoginReq->RoleData = pstPlayer->stRoleData;

	//online 数据同步
	stSyncInfo.stDyn = pstPlayer->stOnline.stDyn;
	stSyncInfo.stFairyOnline = pstPlayer->stOnline.stFairyOnline;
	stSyncInfo.stHeti = pstPlayer->stOnline.stHeti;
	stSyncInfo.stPetOnline = pstPlayer->stOnline.stPetOnline;
	stSyncInfo.ullMiscFlag = pstPlayer->stOnline.cMiscFlag;
	stSyncInfo.cSafeLockFalg = pstPlayer->stOnline.cSafeLockFalg;
	stSyncInfo.iSkillSeq = pstPlayer->stOnline.stAtkInfo.iSkillSeq;
	stSyncInfo.iMoveSeq = pstPlayer->stOnline.iPosSeq;
	stSyncInfo.iSpanGoldRate = pstEnv->pstGlobalObj->stGlobal.GoldRate.GoldRate;
	stSyncInfo.iSpanMapCamp = 0;

	if(is_fini_global(pstEnv) > 0)
	{
		SPANMAPGLOBAL *pstSpanMapGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.SpanMapGlobal;
		if(pstSpanMapGlobal->iCampIdx > 0)
		{
			stSyncInfo.iSpanMapCamp = pstSpanMapGlobal->iCampIdx;
		}
	}
	
	pstLoginReq->ExtLen = sizeof(stSyncInfo);
	memcpy(pstLoginReq->ExtData, &stSyncInfo, sizeof(stSyncInfo));

	//VIP
	pstLoginReq->VipBarInfo = pstPlayer->stOnline.stVipBarInfo;
	
	Z_SSHEAD_INIT(&stSSPkg.Head, SPAN_ROLE_LOGIN_REQ);
	z_ssmsg_send(pstEnv, &stSSPkg, iDstID);

	//这个时候应该就算conncet 状态了，如果进入不成功也算，少一个状态逻辑少点。
	pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
	pstSpanOnline->iStat = SPAN_STAT_CONNECT;
	pstSpanOnline->stInfo.stConnect.iDstID = iDstID;
	pstSpanOnline->stInfo.stConnect.ullRoleID = pstPlayer->stRoleData.RoleID;
	pstPlayer->eStatus = PLAYER_STATUS_ROLE_NOTHING;

	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if(pstZoneTeam)
	{
		player_team_span_sync_req(pstEnv, pstPlayer, pstZoneTeam);
	}

	return 0;
}

//修改客户端MEMID
int player_memid_action(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSROLECHGMEMID *pstChg = &stPkg.Body.RoleChgMem;

	pstChg->MemID = pstPlayer->iMemID;

	Z_CSHEAD_INIT(&stPkg.Head, ROLE_CHG_MEMID);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

//other: Step 3 登录请求处理
int player_span_login2(ZONESVRENV *pstEnv, Player *pstPlayer, SSSPANDSTDATA *pstSpanDstInfo, int iSrcID)
{
	pstPlayer->eStatus = PLAYER_STATUS_ROLE_LOGIN;
	memset(&pstPlayer->stOnline.stSpanOnline, 0, sizeof(pstPlayer->stOnline.stSpanOnline));;
	
	z_init_role(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1);

	// 计算机能信息
	z_skill_online(pstEnv->pstAppCtx, pstEnv, pstPlayer);

	// 计算状态信息
	z_status_online(pstEnv->pstAppCtx, pstEnv, pstPlayer,0);

	if (z_player_find_status(pstPlayer, STATUS_ID_QIANXIN))
	{
		pstPlayer->stOnline.State |= CS_STAT_QIANXIN;
	}

	pstPlayer->stOnline.cCheckCltTimeFlag = 2;

	//通知客户端修改MemID
	player_memid_action(pstEnv, pstPlayer);

	z_player_achievement_online(pstEnv, pstPlayer);
	
	z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);

	if(is_my_master_svr(pstEnv->pstAppCtx->iId, pstPlayer))
	{
		clan_pet_player_login_check(pstEnv,pstPlayer, 1);
	}

	//设置Conn 路由
	if(0 > span_set_routing(pstEnv, player_conn_busid_get(pstEnv, pstPlayer), 
			pstPlayer->stConnectInfo.iConnIdx, pstEnv->pstAppCtx->iId, pstPlayer->iIdx))
	{
		goto _error;
	}

	//进入地图
	switch(pstSpanDstInfo->DstType)
	{
		case SPAN_DST_TYPE_SPARTA:
			{
				ZONEPWORLDINST *pstPworldInst;
				ZONEPWORLD *pstPworld;
				
				if(!is_battle_sparta_svr(pstEnv))
				{
					goto _error;
				}

				pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstSpanDstInfo->Data.Sparta.PworldInstID);
				if(NULL == pstPworldInst)
				{
					goto _error;
				}

				pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
				if (NULL == pstPworld)
				{
					goto _error;
				}
				
				if (0 > z_player_enter_pworld_by_inst(pstEnv, pstPlayer, pstPworld, pstPworldInst))
				{
					goto _error;
				}
			}
			break;
		case SPAN_DST_TYPE_BATTLE:
			if(!is_battle_pve_battle_svr(pstEnv))
			{
				goto _error;
			}

			if(0 > z_player_enter_pworld_inst(pstEnv, pstPlayer, pstSpanDstInfo->Data.Battle.PworldID))
			{
				goto _error;
			}
			break;
		case SPAN_DST_TYPE_MAP:
			{
				int iDstID;
				iDstID = span_map_dstid_get(pstEnv, pstPlayer, pstSpanDstInfo->Data.Map.MapID);
				if(iDstID != pstEnv->pstAppCtx->iId)
				{
					goto _error;
				}

				if (0 > z_player_change_map2(pstEnv->pstAppCtx, pstEnv, pstPlayer,
					pstSpanDstInfo->Data.Map.MapID, &pstSpanDstInfo->Data.Map.Pos, RAND1(180), 0))
				{ 
					goto _error;
				}
			}
			break;
		default:
			break;
	}

	SET_PLAYER_NOFIGHT(pstPlayer);
	z_send_player_fight(pstEnv, pstPlayer, CLEAN_FIGHT_STATUS);
	return 0;
_error:
	player_kickoff(pstEnv->pstAppCtx, pstEnv, pstPlayer, -1);
	return 0;
	UNUSED(iSrcID);	//add by paraunused.pl
}

//other: Step 3 登录请求处理
int player_span_login(ZONESVRENV *pstEnv, SSSPANROLELOGINREQ *pstLoginReq)
{
	Player *pstPlayer;
	SpanOL *pstSpanOnline;
	PlayerSyncOnline stSyncInfo;
	
	pstPlayer = player_get_by_uin(pstEnv, pstLoginReq->RoleData.Uin);
	if(!pstPlayer)
	{
		player_span_clean_logout_req(pstEnv, pstLoginReq->RoleData.Uin, 
			pstLoginReq->RoleData.RoleID, pstLoginReq->MsgSrcID);
		return -1;
	}

	if(!is_my_master_svr(pstEnv->pstAppCtx->iId, pstPlayer))
	{
		char szName[ROLE_NAME_LEN];	
		
		//确保是正确的消息
		pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
		if(pstSpanOnline->iStat != SPAN_STAT_HOLD ||
			pstSpanOnline->stInfo.stHold.ullRoleID != pstLoginReq->RoleData.RoleID ||
			pstSpanOnline->stInfo.stHold.iSrcID != pstLoginReq->MsgSrcID)
		{
			//这个情况暂时让角色卡着。
			return -1;
		}

		tlog_info(pstEnv->pstLogCat, 0, 0, "Status=AccLogin Uin=%d AccName=%s ", 
			pstPlayer->stRoleData.Uin, pstPlayer->szAccount);


		STRNCPY(szName, pstPlayer->stRoleData.RoleName, CCH(szName));
		pstPlayer->stRoleData = pstLoginReq->RoleData;
		STRNCPY(pstPlayer->stRoleData.RoleName, szName, CCH(pstPlayer->stRoleData.RoleName));
	}

	if(pstLoginReq->ExtLen == sizeof(stSyncInfo))
	{
		memcpy(&stSyncInfo, pstLoginReq->ExtData, sizeof(stSyncInfo));
		pstPlayer->stOnline.stFairyOnline = stSyncInfo.stFairyOnline;
		pstPlayer->stOnline.stHeti = stSyncInfo.stHeti;
		pstPlayer->stOnline.stPetOnline = stSyncInfo.stPetOnline;
		pstPlayer->stOnline.cMiscFlag = stSyncInfo.ullMiscFlag;
		pstPlayer->stOnline.cSafeLockFalg = stSyncInfo.cSafeLockFalg;
		pstPlayer->stOnline.stAtkInfo.iSkillSeq = stSyncInfo.iSkillSeq;
		pstPlayer->stOnline.iPosSeq = stSyncInfo.iMoveSeq;
		pstPlayer->stOnline.iSpanGoldRate = stSyncInfo.iSpanGoldRate;
		
		if(is_battle_pvp_svr(pstEnv))
		{
			pstPlayer->stOnline.stWarOL.cCampIdx = stSyncInfo.iSpanMapCamp;
		}
	}
	
	return player_span_login2(pstEnv, pstPlayer, &pstLoginReq->DstData, pstLoginReq->MsgSrcID);
}

//master : 处理分线切线
int player_span_chg_svr(ZONESVRENV *pstEnv, SSSPANCHGSVR *pstChgSvr)
{
	Player *pstPlayer;
	SpanOL *pstSpanOnline;

	pstPlayer = player_get_by_uin(pstEnv, pstChgSvr->Uin);
	if(!pstPlayer || pstPlayer ->stRoleData.RoleID != pstChgSvr->RoleID)
	{
		return -1;
	}

	if(!is_my_master_svr(pstEnv->pstAppCtx->iId, pstPlayer))
	{
		return -1;
	}

	//确保是正确的消息
	pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
	if(pstSpanOnline->iStat != SPAN_STAT_CONNECT ||
		pstSpanOnline->stInfo.stConnect.ullRoleID != pstChgSvr->RoleID)
	{
		return -1;
	}

	player_span_login_req(pstEnv, pstPlayer, &pstChgSvr->DstData, pstChgSvr->DstID);
	return 0;
}

//master->other T用户下线
int player_span_role_kick(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	SpanOL *pstSpanOnline;
	SSPKG stSSPkg;
	SSSPANROLEUPDATERES *pstUpdateRes = &stSSPkg.Body.SpanRoleUpdateRes;
	
	pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
	if(pstSpanOnline->iStat != SPAN_STAT_CONNECT)
	{
		return -1;
	}

	pstUpdateRes->MsgDstID = pstSpanOnline->stInfo.stConnect.iDstID;
	pstUpdateRes->MsgSrcID = pstEnv->pstAppCtx->iId;
	pstUpdateRes->Uin = pstPlayer->stRoleData.Uin;
	pstUpdateRes->RoleID	= pstPlayer->stRoleData.RoleID;
	pstUpdateRes->Result = SPAN_ROLE_UPDATE_RES_KICK;

	Z_SSHEAD_INIT(&stSSPkg.Head, SPAN_ROLE_UPDATE_RES);
	z_ssmsg_send(pstEnv, &stSSPkg, pstSpanOnline->stInfo.stConnect.iDstID);
	return 0;
}

//master->other 清理掉用户
int player_span_role_clean(ZONESVRENV *pstEnv, int iUin, tdr_ulonglong ullRoleID, int iDstID)
{
	SSPKG stSSPkg;
	SSSPANROLEUPDATERES *pstUpdateRes = &stSSPkg.Body.SpanRoleUpdateRes;

	pstUpdateRes->MsgDstID = iDstID;
	pstUpdateRes->MsgSrcID = pstEnv->pstAppCtx->iId;
	pstUpdateRes->Uin = iUin;
	pstUpdateRes->RoleID	= ullRoleID;
	pstUpdateRes->Result = SPAN_ROLE_UPDATE_RES_CLEAN;

	Z_SSHEAD_INIT(&stSSPkg.Head, SPAN_ROLE_UPDATE_RES);
	z_ssmsg_send(pstEnv, &stSSPkg, iDstID);
	return 0;
}

//matser:  ROLE_UPDATE处理
int player_span_role_update(ZONESVRENV *pstEnv, SSSPANROLEUPDATEREQ *pstUpdateReq)
{
	Player * pstPlayer;
	SpanOL *pstSpanOnline;
	PlayerSyncOnline stSyncInfo;
	ProtectedData stSavedData;
	
	pstPlayer = player_get_by_uin(pstEnv, pstUpdateReq->Uin);
	if(!pstPlayer)
	{
		goto _clean;
	}

	if(!is_my_master_svr(pstEnv->pstAppCtx->iId, pstPlayer))
	{
		return -1;
	}

	//确保是目前连接的分线的角色更新请求
	pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
	if(pstSpanOnline->iStat != SPAN_STAT_CONNECT ||
		pstSpanOnline->stInfo.stConnect.iDstID != pstUpdateReq->MsgSrcID ||
		pstSpanOnline->stInfo.stConnect.ullRoleID != pstUpdateReq->RoleData.RoleID)
	{
		return -1;
	}

	if(pstUpdateReq->DataDirty)
	{
		pstPlayer->stOnline.cDataDirty = DIRTY_IMPORTANT;
	}
	
	z_save_protected_data(&pstPlayer->stRoleData, &stSavedData);
	memcpy(&pstPlayer->stRoleData, &pstUpdateReq->RoleData, sizeof(ROLEDATA));
	z_restore_protected_data(&stSavedData, &pstPlayer->stRoleData);

	if(pstUpdateReq->MsgSrcID == pstEnv->iBattleSpartaID)
	{
		//每次都更新下排行榜
		range_sparta_set(pstEnv, pstPlayer, RANGE_MAIN_SPARTA_SUB_FIGHT);
	}
		
	if(pstUpdateReq->ExtLen == sizeof(stSyncInfo))
	{
		memcpy(&stSyncInfo, pstUpdateReq->ExtData, sizeof(stSyncInfo));
		pstPlayer->stOnline.stFairyOnline = stSyncInfo.stFairyOnline;
		pstPlayer->stOnline.stHeti = stSyncInfo.stHeti;
		pstPlayer->stOnline.stPetOnline = stSyncInfo.stPetOnline;
		pstPlayer->stOnline.cMiscFlag = stSyncInfo.ullMiscFlag;
		pstPlayer->stOnline.cSafeLockFalg = stSyncInfo.cSafeLockFalg;
		pstPlayer->stOnline.stAtkInfo.iSkillSeq = stSyncInfo.iSkillSeq;
		pstPlayer->stOnline.iPosSeq = stSyncInfo.iMoveSeq;
		pstPlayer->stOnline.iSpanGoldRate = stSyncInfo.iSpanGoldRate;
	}

	//分线更新，准备切换线路
	if(pstUpdateReq->Type == SPAN_UPDATE_REQ_CHG_SVR)
	{
		return player_span_chg_svr_update_in(pstEnv, pstPlayer);
	}
	//分线信息回写，并KICK分线下线
	else if(pstUpdateReq->Type == SPAN_UPDATE_REQ_KICK)
	{
		pstPlayer->ucRoleLogoutReason = pstUpdateReq->Reason;
		pstPlayer->eStatus = PLAYER_STATUS_ROLE_LOGOUT_REQ;
		z_db_role_update(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);

		pstPlayer->stOnline.bWaitRoleLogout = 2;
		memset(&pstPlayer->stOnline.stSpanOnline, 0, sizeof(pstPlayer->stOnline.stSpanOnline));

		// 计算技能信息
		z_skill_online(pstEnv->pstAppCtx, pstEnv, pstPlayer);

		// 计算状态信息
		z_status_online(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);

		//坐骑
		pstPlayer->stOnline.RideItemID = pstPlayer->stRoleData.MiscInfo.RideItemID;
		pstPlayer->stOnline.RideLevel = pstPlayer->stRoleData.MiscInfo.RideLevel;

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
	
		//设置Conn 路由
		span_set_routing(pstEnv, player_conn_busid_get(pstEnv, pstPlayer), 
				pstPlayer->stConnectInfo.iConnIdx, pstEnv->pstAppCtx->iId, pstPlayer->iIdx);
		
		goto _clean;
	}
	else
	{
		SSPKG stSSPkg;
		SSSPANROLEUPDATERES *pstUpdateRes = &stSSPkg.Body.SpanRoleUpdateRes;

		pstUpdateRes->MsgDstID = pstSpanOnline->stInfo.stConnect.iDstID;
		pstUpdateRes->MsgSrcID = pstEnv->pstAppCtx->iId;
		pstUpdateRes->Uin = pstUpdateReq->Uin;
		pstUpdateRes->RoleID	= pstUpdateReq->RoleData.RoleID;
		pstUpdateRes->Result = SPAN_ROLE_UPDATE_RES_SUCC;

		Z_SSHEAD_INIT(&stSSPkg.Head, SPAN_ROLE_UPDATE_RES);
		z_ssmsg_send(pstEnv, &stSSPkg, pstSpanOnline->stInfo.stConnect.iDstID);
	
		pstPlayer->tActive = pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	return 0;
_clean:
	player_span_role_clean(pstEnv, pstUpdateReq->Uin, pstUpdateReq->RoleData.RoleID, pstUpdateReq->MsgSrcID);
	return 0;
}


//回写请求返回消息
int player_span_role_update_res(ZONESVRENV *pstEnv, SSSPANROLEUPDATERES *pstUpdateRes)
{
	Player * pstPlayer;
	
	pstPlayer = player_get_by_uin(pstEnv, pstUpdateRes->Uin);
	if(!pstPlayer || pstPlayer->stRoleData.RoleID != pstUpdateRes->RoleID)
	{
		//T角色的时候如果角色不再战场，干脆也正常T下线好了
		if(pstUpdateRes->Result == SPAN_ROLE_UPDATE_RES_KICK)
		{
			player_span_clean_logout_req(pstEnv, pstUpdateRes->Uin, 
				pstUpdateRes->RoleID, pstUpdateRes->MsgSrcID);
		}
		
		return -1;
	}

	if(pstUpdateRes->Result == SPAN_ROLE_UPDATE_RES_KICK)
	{
		z_player_leave_map(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1);
		player_kickoff(pstEnv->pstAppCtx, pstEnv, pstPlayer, -1);
		//z_db_role_update(pstEnv->pstAppCtx, pstEnv, pstPlayer, SPAN_SVR_ROLE_KICK);
	}
	else if(pstUpdateRes->Result == SPAN_ROLE_UPDATE_RES_CHG_SVR)
	{
		player_span_chg_svr_update_res(pstEnv, pstUpdateRes);
	}
	else if(pstUpdateRes->Result == SPAN_ROLE_UPDATE_RES_CLEAN)
	{
		if(is_battle_sparta_svr(pstEnv))
		{
			sparta_role_logout_clean(pstEnv, pstPlayer);
		}
		else
		{
			//z_exit_pworld_queue(pstEnv, pstPlayer, pstPlayer->stOnline.stPworldQueue.iPworldID, 0);
			z_exit_pworld_queue(pstEnv, pstPlayer, pstPlayer->stOnline.stPworldQueue.iPworldID, 1);
			z_free_player(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}
	}
	else if(pstUpdateRes->Result == SPAN_ROLE_UPDATE_RES_SUCC)
	{
		pstPlayer->tActive = pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	
	return 0;
}

//分线影子队伍
int z_team_span_sync(ZONESVRENV * pstEnv, SSTEAMSYNC *pstSSTeamSync)
{
	int i, fFound;
	ZoneTeam *pstTeam;
	TEAMMEMBER* pstTeamMember;

	fFound = team_shadow_find_or_new(pstEnv, pstSSTeamSync->TeamID, &pstTeam);
	if(fFound < 0 || !pstTeam)
	{
		team_shadow_delete(pstEnv, pstSSTeamSync->TeamID);
	}
	
	fFound = team_shadow_find_or_new(pstEnv, pstSSTeamSync->TeamID, &pstTeam);
	if(fFound < 0 || !pstTeam)
	{
		team_shadow_delete(pstEnv, pstSSTeamSync->TeamID);
		return -1;
	}
	
	for(i = 0; i < pstSSTeamSync->MemberNum; i++)
	{
		pstTeamMember = &pstTeam->stTeamInfo.TeamMember[i];
		
		pstTeamMember->RoleID = pstSSTeamSync->TeamMember[i].RoleID;
		STRNCPY(pstTeamMember->MemberName, pstSSTeamSync->TeamMember[i].RoleName, sizeof(pstTeamMember->MemberName));
		pstTeamMember->Position = pstSSTeamSync->TeamMember[i].Position;
	}
	
	pstTeam->stTeamInfo.MemberNum = pstSSTeamSync->MemberNum;
	pstTeam->stTeamInfo.TeamBootyMode = pstSSTeamSync->TeamBootyMode;
	
	return 0;
}

//同步队伍信息到角色所在进程
int player_team_span_sync_req(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneTeam *pstZoneTeam)
{
	int i;
	SpanOL *pstSpanOnline;
	TEAMMEMBER *pstTeamMember;
	char szTmp[ROLE_NAME_LEN];
	SSPKG stPkg;
	SSTEAMSYNC *pstTeamSync = &stPkg.Body.TeamSync;

	if( INST_MODE_ZONE_SVR != pstEnv->iInstMode)
	{
		return -1;
	}

	pstTeamSync->TeamID = pstZoneTeam->stTeamInfo.TeamID;
	pstTeamSync->TeamBootyMode = pstZoneTeam->stTeamInfo.TeamBootyMode;
	pstTeamSync->MemberNum = 0;
	for(i = 0; i < pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[i];
		
		pstTeamSync->TeamMember[pstTeamSync->MemberNum].RoleID = pstTeamMember->RoleID;
		pstTeamSync->TeamMember[pstTeamSync->MemberNum].Position = pstTeamMember->Position;

		snprintf(szTmp, CCH(szTmp), "w%d"BATTLE_NAME_SYMBOL"%s",
			         pstEnv->unWorldID, pstTeamMember->MemberName);
		STRNCPY(pstTeamSync->TeamMember[pstTeamSync->MemberNum].RoleName, 
			szTmp, sizeof(pstTeamSync->TeamMember[pstTeamSync->MemberNum].RoleName));		
		pstTeamSync->MemberNum++;
	}
	
	//在非主线进程的角色需要同步队伍信息
	pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
	if(pstSpanOnline->iStat == SPAN_STAT_CONNECT)
	{
		pstTeamSync->MsgDstID = pstSpanOnline->stInfo.stConnect.iDstID;
		pstTeamSync->MsgSrcID = pstEnv->pstAppCtx->iId;
		Z_SSHEAD_INIT(&stPkg.Head, SS_TEAM_SYNC);
		z_ssmsg_send(pstEnv, &stPkg, pstSpanOnline->stInfo.stConnect.iDstID);
	}

	return 0;
}

//请求同步全队队伍信息
int z_team_all_span_sync_req(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam)
{
	int i;
	SpanOL *pstSpanOnline;
	Player *pstPlayer;
	TEAMMEMBER *pstTeamMember;
	char szTmp[ROLE_NAME_LEN];
	SSPKG stPkg;
	SSTEAMSYNC *pstTeamSync = &stPkg.Body.TeamSync;

	if( INST_MODE_ZONE_SVR != pstEnv->iInstMode)
	{
		return -1;
	}

	pstTeamSync->TeamID = pstZoneTeam->stTeamInfo.TeamID;
	pstTeamSync->TeamBootyMode = pstZoneTeam->stTeamInfo.TeamBootyMode;
	pstTeamSync->MemberNum = 0;
	for(i = 0; i < pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[i];
		
		pstTeamSync->TeamMember[pstTeamSync->MemberNum].RoleID = pstTeamMember->RoleID;
		pstTeamSync->TeamMember[pstTeamSync->MemberNum].Position = pstTeamMember->Position;

		snprintf(szTmp, CCH(szTmp), "w%d"BATTLE_NAME_SYMBOL"%s",
			         pstEnv->unWorldID, pstTeamMember->MemberName);
		STRNCPY(pstTeamSync->TeamMember[pstTeamSync->MemberNum].RoleName, 
			szTmp, sizeof(pstTeamSync->TeamMember[pstTeamSync->MemberNum].RoleName));	
		
		pstTeamSync->MemberNum++;
	}

	for(i = 0; i < pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[i];
		
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeamMember->MemberName);	
		if(!pstPlayer)
		{
			continue;
		}

		if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
		{
			continue;
		}

		//在非主线进程的角色需要同步队伍信息
		pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
		if(pstSpanOnline->iStat == SPAN_STAT_CONNECT)
		{
			pstTeamSync->MsgDstID = pstSpanOnline->stInfo.stConnect.iDstID;
			pstTeamSync->MsgSrcID = pstEnv->pstAppCtx->iId;
			Z_SSHEAD_INIT(&stPkg.Head, SS_TEAM_SYNC);
			z_ssmsg_send(pstEnv, &stPkg, pstSpanOnline->stInfo.stConnect.iDstID);
		}
	}

	return 0;
}

int player_pvp_svr_member_info_sync_req(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	SSPKG stPkg;
	SSBATTLEMEMBERINFO *pstMemberInfo = &stPkg.Body.BattleMemberInfo;
	
	if( INST_MODE_BATTLE_SVR != pstEnv->iInstMode)
	{
		return -1;
	}

	pstMemberInfo->Uin = pstPlayer->stRoleData.Uin;
	pstMemberInfo->RoleID = pstPlayer->stRoleData.RoleID;
	player_team_sync_member_info(pstEnv, &pstMemberInfo->Member, pstPlayer);
	battle_rolename_remove_prefix(pstMemberInfo->Member.MemberName);
			
	Z_SSHEAD_INIT(&stPkg.Head, BATTLE_TEAM_MEMBER_SYNC);
	z_ssmsg_send(pstEnv, &stPkg, player_master_busid_get(pstEnv, pstPlayer));
	return 0;
}

int player_pvp_svr_member_info_sync(ZONESVRENV* pstEnv, SSBATTLEMEMBERINFO *pstSSMemberInfo)
{
	Player *pstPlayer;
	ZoneTeam *pstTeam;
	TEAMMEMBER *pstTeamMember;
	int iOld;
	int iVote;
	
	pstPlayer = player_get_by_uin(pstEnv, pstSSMemberInfo->Uin);
	if( pstPlayer == NULL  || pstPlayer->stRoleData.RoleID != pstSSMemberInfo->RoleID)
	{
		return -1;
	}

	if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
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
	iVote = pstTeamMember->VoteFlag;
	*pstTeamMember = pstSSMemberInfo->Member;
	pstTeamMember->Position = iOld;
	pstTeamMember->VoteFlag = iVote;
	return 0;
}

//切地图是否跨服检查
int player_span_map_check(ZONESVRENV* pstEnv, Player *pstPlayer, MapIndex *pstMapIdx, RESPOS *pstPos)
{
	int iDstID;
	SSSPANDSTDATA stSpanDst;
	
	if(pstMapIdx->stMapDef.MapID < PWORLD_MAPID_BASE)
	{
		iDstID = span_map_dstid_get(pstEnv,  pstPlayer, pstMapIdx->stMapDef.MapID);
	}
	
	else
	{
		/*
		//暂时不让在跨服进程进副本，都在自己的zone
		if(!is_my_master_svr(pstEnv->pstAppCtx->iId, pstPlayer))
		{
			goto _error;
		}
		*/

		return 0;
	}
	
	
	if(iDstID == pstEnv->pstAppCtx->iId)
	{
		return 0;
	}

	stSpanDst.DstType = SPAN_DST_TYPE_MAP;
	stSpanDst.Data.Map.MapID = pstMapIdx->stMapDef.MapID;
	stSpanDst.Data.Map.Pos = *pstPos;

//_final:
	player_span_hold_req(pstEnv, pstPlayer, &stSpanDst);
	return -1;

/*
_error:

	stSpanDst.DstType = SPAN_DST_TYPE_MAP;
	stSpanDst.Data.Map.MapID = pstPlayer->stRoleData.ExitMap;
	stSpanDst.Data.Map.Pos.X = pstPlayer->stRoleData.ExitPos_x;
	stSpanDst.Data.Map.Pos.Y = pstPlayer->stRoleData.ExitPos_y;
	goto _final;
	
	return -1;
*/
}

//SS 线路心跳发起
int player_span_heartbeat_req(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	SSPKG stPkg;
	SSHEARTBEATREQ *pstSSHeartBeatReq = &stPkg.Body.HeartBeatReq;

	pstSSHeartBeatReq->Uin = pstPlayer->stRoleData.Uin;
	pstSSHeartBeatReq->RoleID = pstPlayer->stRoleData.RoleID;

	Z_SSHEAD_INIT(&stPkg.Head, SS_HEARTBEAT_REQ);
	z_ssmsg_send(pstEnv, &stPkg, player_master_busid_get(pstEnv, pstPlayer));
	return 0;
}

//SS 线路报名状态心跳发起
int player_span_apply_heartbeat_req(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	SSPKG stPkg;
	SSHEARTBEATREQ *pstSSHeartBeatReq = &stPkg.Body.HeartBeatReq;

	pstSSHeartBeatReq->Uin = pstPlayer->stRoleData.Uin;
	pstSSHeartBeatReq->RoleID = pstPlayer->stRoleData.RoleID;

	Z_SSHEAD_INIT(&stPkg.Head, SS_HEARTBEAT_REQ);

	if(pstPlayer->stOnline.stSpanOnline.stApply.iBattleApply == SPAN_APPLY_ING)
	{
		z_ssmsg_send(pstEnv, &stPkg, pstEnv->iBattlePveBattleID);
	}

	if(pstPlayer->stOnline.stSpanOnline.stApply.iSpartaApply == SPAN_APPLY_ING)
	{
		z_ssmsg_send(pstEnv, &stPkg, pstEnv->iBattleSpartaID);
	}
	
	return 0;
}

//SS 线路心跳
int player_span_heartbeat(ZONESVRENV *pstEnv, SSHEARTBEATREQ *pstSSHeartBeatReq, int iSrcID)
{
	Player *pstPlayer;
	SpanOL *pstSpanOnline;
	
	pstPlayer = player_get_by_uin(pstEnv, pstSSHeartBeatReq->Uin);
	if( pstPlayer == NULL  || pstPlayer->stRoleData.RoleID != pstSSHeartBeatReq->RoleID)
	{
		return -1;
	}

	pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
	if(pstSpanOnline->iStat == SPAN_STAT_CONNECT)
	{
		if(pstSpanOnline->stInfo.stConnect.ullRoleID != pstSSHeartBeatReq->RoleID ||
			pstSpanOnline->stInfo.stConnect.iDstID != iSrcID)
		{
			return -1;
		}
	}
	else if(pstSpanOnline->iStat == SPAN_STAT_APPLY)
	{
		if(pstSpanOnline->stInfo.stApply.ullRoleID != pstSSHeartBeatReq->RoleID)
		{
			return -1;
		}
	}

	pstPlayer->tActive = pstEnv->pstAppCtx->stCurr.tv_sec;
	return 0;
}

//跨服环境不存在角色时，干脆T掉本服人下线
int player_span_clean_logout_req(ZONESVRENV *pstEnv, int iUin, tdr_ulonglong ullRoleID, int iDstID)
{
	SSPKG stPkg;
	SSSPANCLEANLOGOUT *pstSSSpanCleanLogout = &stPkg.Body.SpanCleanLogout;

	pstSSSpanCleanLogout->Uin = iUin;
	pstSSSpanCleanLogout->RoleID = ullRoleID;

	Z_SSHEAD_INIT(&stPkg.Head, SS_SPAN_CLEAN_LOGOUT);
	z_ssmsg_send(pstEnv, &stPkg, iDstID);
	
	return 0;
}

//跨服环境不存在角色时，干脆T掉本服人下线
int player_span_clean_logout(ZONESVRENV *pstEnv, SSSPANCLEANLOGOUT *pstSSSpanCleanLogout, int iSrcID)
{
	Player *pstPlayer;
	SpanOL *pstSpanOnline;
	
	pstPlayer = player_get_by_uin(pstEnv, pstSSSpanCleanLogout->Uin);
	if( pstPlayer == NULL  || pstPlayer->stRoleData.RoleID != pstSSSpanCleanLogout->RoleID)
	{
		return -1;
	}

	pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
	if(pstSpanOnline->iStat != SPAN_STAT_CONNECT)
	{
		return -1;
	}

	if(pstSpanOnline->stInfo.stConnect.ullRoleID != pstSSSpanCleanLogout->RoleID ||
		pstSpanOnline->stInfo.stConnect.iDstID != iSrcID)
	{
		return -1;
	}

	pstPlayer->ucRoleLogoutReason = ROLE_LOGOUT_KICK;
	pstPlayer->eStatus = PLAYER_STATUS_ROLE_LOGOUT_REQ;
	z_db_role_update(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);

	pstPlayer->stOnline.bWaitRoleLogout = 2;
	memset(&pstPlayer->stOnline.stSpanOnline, 0, sizeof(pstPlayer->stOnline.stSpanOnline));
	return 0;
}

//下线强制清理排队报名占用的位置
int player_span_apply_queue_clean(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if(pstPlayer->stOnline.stSpanOnline.iStat == SPAN_STAT_CONNECT &&
		(pstPlayer->stOnline.stSpanOnline.stInfo.stConnect.iDstID == pstEnv->iBattlePveBattleID ||
		pstPlayer->stOnline.stSpanOnline.stInfo.stConnect.iDstID == pstEnv->iBattleSpartaID))
	{
		return -1;
	}
	
	if(pstPlayer->stOnline.stSpanOnline.stApply.iBattleApply == SPAN_APPLY_ING)
	{
		player_span_role_clean(pstEnv, pstPlayer->stRoleData.Uin, pstPlayer->stRoleData.RoleID, pstEnv->iBattlePveBattleID);
	}
	else if(pstPlayer->stOnline.stSpanOnline.stApply.iSpartaApply == SPAN_APPLY_ING)
	{
		player_span_role_clean(pstEnv, pstPlayer->stRoleData.Uin, pstPlayer->stRoleData.RoleID, pstEnv->iBattleSpartaID);
	}

	pstPlayer->stOnline.stSpanOnline.stApply.iBattleApply = SPAN_APPLY_NONE;
	pstPlayer->stOnline.stSpanOnline.stApply.iSpartaApply = SPAN_APPLY_NONE;
	return 0;
}

//通知master 清理排队状态
int player_span_exit_pworld_queue_req(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	SSPKG stPkg;
	SSEXITPWORLDQUEUE *pstExitPworldQueue = &stPkg.Body.ExitPworldQueue;

	if( INST_MODE_BATTLE_SVR != pstEnv->iInstMode)
	{
		return -1;
	}

	pstExitPworldQueue->Uin = pstPlayer->stRoleData.Uin;
	pstExitPworldQueue->RoleID = pstPlayer->stRoleData.RoleID;

	Z_SSHEAD_INIT(&stPkg.Head, SS_EXIT_PWORLD_QUEUE);
	z_ssmsg_send(pstEnv, &stPkg, player_master_busid_get(pstEnv, pstPlayer));
	
	return 0;
}

//通知master 清理公会的排队状态
int clan_span_exit_pworld_queue_req(ZONESVRENV* pstEnv, CLANID *pstClanID,int iBusID)
{
	SSPKG stPkg;
	SSEXITPWORLDQUEUE *pstExitPworldQueue = &stPkg.Body.ExitPworldQueue;

	if( INST_MODE_BATTLE_SVR != pstEnv->iInstMode)
	{
		return -1;
	}

	pstExitPworldQueue->Uin = 0;//用这个区分roleid的含义
	pstExitPworldQueue->RoleID = pstClanID->GID;

	Z_SSHEAD_INIT(&stPkg.Head, SS_EXIT_PWORLD_QUEUE);
	//player_master_busid_get(pstEnv, pstPlayer)
	z_ssmsg_send(pstEnv, &stPkg, iBusID);
	
	return 0;
}

//master:清理掉排队状态，不在发送心跳包
int player_span_exit_pworld_queue(ZONESVRENV* pstEnv, SSEXITPWORLDQUEUE *pstReq)
{
	Player *pstPlayer;
	SpanOL *pstSpanOnline;

	if (0 == pstReq->Uin)
	{
		//表示公会
		ZoneClan *pstClan = clan_find_by_wid(pstEnv, pstReq->RoleID);
		if (pstClan)
		{
			ClanBattleApplyData *pstBattleApplyData = &pstClan->stZoneClanInfo.stBattleApplyData;
			
			
			clan_battle_apply_info_action(pstEnv,pstClan,0);
			pstBattleApplyData->iBattleID = 0;
		}
		else
		{
			//log?
			return -1;
		}
			
	}
	else
	{
		pstPlayer = player_get_by_uin(pstEnv, pstReq->Uin);
		if( pstPlayer == NULL  || pstPlayer->stRoleData.RoleID != pstReq->RoleID)
		{
			return -1;
		}

		pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
		pstSpanOnline->stApply.iBattleApply = SPAN_APPLY_NONE;
		pstSpanOnline->stApply.iSpartaApply = SPAN_APPLY_NONE;
	}
	
	return 0;
}

//同步保护数据到角色所在分线
int player_protect_data_sync_req(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	char szTmp[ROLE_NAME_LEN];
	SpanOL *pstSpanOnline;
	SSPKG stPkg;
	SSPROTECTDATASYNC *pstProtectSync = &stPkg.Body.ProtectDataSync;
	
	if(!is_my_master_svr(pstEnv->pstAppCtx->iId, pstPlayer))
	{
		return -1;
	}

	pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
	if(pstSpanOnline->iStat != SPAN_STAT_CONNECT)
	{
		return -1;
	}

	pstProtectSync->RoleID = pstPlayer->stRoleData.RoleID;
	snprintf(szTmp, CCH(szTmp), "w%d"BATTLE_NAME_SYMBOL"%s", pstEnv->unWorldID, pstPlayer->stRoleData.RoleName);
	STRNCPY(pstProtectSync->RoleName, szTmp, sizeof(pstProtectSync->RoleName));

	//保护数据同步到分线todo: 公会、好友
	pstProtectSync->ProtectData.TeamID = pstPlayer->stRoleData.MiscInfo.TeamID;
	pstProtectSync->ProtectData.TeamIdx = pstPlayer->stRoleData.MiscInfo.TeamIdx;
	
	Z_SSHEAD_INIT(&stPkg.Head, SS_PROTECT_DATA_SYNC);
	z_ssmsg_send(pstEnv, &stPkg, pstSpanOnline->stInfo.stConnect.iDstID);
	
	return 0;
}

//分线入队出退需要处理的事情
int player_team_span_update(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneTeam * pstOldTeam)
{
	//老的队伍存在，要退出
	if(pstOldTeam)
	{
		int iIdx;
		TEAMMEMBER *pstTeamMember;
	
		player_team_memchg(pstEnv, pstPlayer, pstOldTeam, 0);
		pstTeamMember = player_team_get_member(pstOldTeam, pstPlayer->stRoleData.RoleName, &iIdx);
		if (pstTeamMember)
		{
			pstOldTeam->stTeamInfo.MemberNum--;
			if (iIdx != pstOldTeam->stTeamInfo.MemberNum)
			{
				memmove(pstTeamMember, pstTeamMember+1,  (pstOldTeam->stTeamInfo.MemberNum - iIdx) * sizeof(*pstTeamMember));
			}
		}
	}
	else
	{
		ZoneTeam * pstZoneTeam = player_team_get(pstEnv, pstPlayer);
		if(pstZoneTeam)
		{
			player_team_memchg(pstEnv, pstPlayer, pstZoneTeam, 1);
		}
	}

	return 0;
}

//分线保护数据同步(队伍 公会 都可以采用这种方式)
int player_protect_data_sync(ZONESVRENV* pstEnv, SSPROTECTDATASYNC *pstSSProtectDataSync)
{
	tdr_ulonglong ullOldTeamID;
	Player *pstPlayer;
	ZoneTeam *pstZoneTeam;

	pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstSSProtectDataSync->RoleName);
	if(!pstPlayer)
	{
		return -1;
	}

	pstZoneTeam = player_team_get(pstEnv, pstPlayer);

	//防错，只会是主线同步到分线
	if(is_my_master_svr(pstEnv->pstAppCtx->iId, pstPlayer))
	{
		return -1;
	}

	ullOldTeamID = pstPlayer->stRoleData.MiscInfo.TeamID;
	pstPlayer->stRoleData.MiscInfo.TeamID = pstSSProtectDataSync->ProtectData.TeamID;
	pstPlayer->stRoleData.MiscInfo.TeamIdx = pstSSProtectDataSync->ProtectData.TeamIdx;

	//队伍变化后需要进行的操作
	if(ullOldTeamID != pstPlayer->stRoleData.MiscInfo.TeamID)
	{
		player_team_span_update(pstEnv, pstPlayer, pstZoneTeam);
	}
	
	return 0;
}

//分线队伍解散
int z_team_span_destroy(ZONESVRENV* pstEnv, SSTEAMDESTROY *pstSSTeamDestroy)
{
	ZoneTeam *pstZoneTeam = team_shadow_by_id(pstEnv, pstSSTeamDestroy->TeamID);
	if(!pstZoneTeam)
	{
		return -1;
	}
	
	if( INST_MODE_BATTLE_SVR != pstEnv->iInstMode)
	{
		return -1;
	}

	return team_shadow_delete(pstEnv, pstSSTeamDestroy->TeamID);
}

//通知分线队伍解散
int z_team_span_destroy_req(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam)
{
	int i;
	SpanOL *pstSpanOnline;
	Player *pstPlayer;
	TEAMMEMBER *pstTeamMember;
	SSPKG stPkg;
	SSTEAMDESTROY *pstTeamDestroy = &stPkg.Body.TeamDestroy;

	if(INST_MODE_ZONE_SVR != pstEnv->iInstMode)
	{
		return -1;
	}

	for(i = 0; i < pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[i];
		
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeamMember->MemberName);	
		if(!pstPlayer)
		{
			continue;
		}

		if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING)
		{
			continue;
		}

		//在非主线进程的角色需要同步队伍信息
		pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
		if(pstSpanOnline->iStat == SPAN_STAT_CONNECT)
		{
			pstTeamDestroy->TeamID =  pstZoneTeam->stTeamInfo.TeamID;
		
			Z_SSHEAD_INIT(&stPkg.Head, SS_TEAM_DESTROY);
			z_ssmsg_send(pstEnv, &stPkg, pstSpanOnline->stInfo.stConnect.iDstID);
		}
	}
	
	return 0;
}

int span_pkg_trans(ZONESVRENV* pstEnv, SSPKG *pstPkg)
{
	SSPKGTRANS* pstTrans = &pstPkg->Body.PkgTansInfo;
	CSPKG stCs;

	Z_CSHEAD_INIT(&stCs.Head, pstTrans->Cmd);
	stCs.Body = pstPkg->Body.PkgTansInfo.Body;
	
	switch(pstTrans->Type)
	{
	case PKG_TRANS_ALL:
		z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stCs);
		break;
	case PKG_TRANS_CLAN:
		{
			ZoneClan *pstClan = z_id_clan( pstEnv, pstTrans->ClanID);
			if (pstClan)
				player_clan_broadcast(pstEnv, pstClan, &stCs, 0);
		}
		
		break;
	default:
		break;
	}

	return 0;
}

int ss_zone_msg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iMsgSrc, char *pszMsg, int iMsgLen)
{
	SSPKG stPkgRes;

	if(ss_unpack(pstEnv->pstSSHan, pszMsg, iMsgLen, &stPkgRes, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "world_svr ss_unpack fail");
		return -1;
	}

	switch(stPkgRes.Head.Cmd)
	{
		//Old ......
		case BATTLE_SHOPPING_REQ:
			shopping_battle_buy(pstAppCtx,pstEnv, &stPkgRes.Body.BattleBuyReq);
			break;
		case BATTLE_SHOPPING_RES:
			shopping_battle_buy_res(pstAppCtx,pstEnv,&stPkgRes.Body.BattleBuyRes);
			break;
		case SS_CMD_REQ:
			z_ss_cmd_req(pstEnv, &stPkgRes);
			break;
		case BATTLE_MIRROR_SYNC:
			zone_mirror_sync_respond(pstAppCtx, pstEnv, iMsgSrc, &stPkgRes.Body.BattleMirrorSync);
			break;
		case ZONE_PROXY_ROUTINE:
			zone_proxy_respond(pstAppCtx, pstEnv, &stPkgRes.Body.BattleProxy);
			break;
		case BATTLE_TEAM_MEMBER_SYNC:
			player_pvp_svr_member_info_sync(pstEnv, &stPkgRes.Body.BattleMemberInfo);
			break;
		case BATTLE_APPLY_QUEUE:
			player_spansvr_battle_join_queue(pstEnv, &stPkgRes.Body.BattleApplyQueue);
			break;
		case FLUSH_BATTLE_QUEUE:
			ss_flush_battle_pworld_queue(pstEnv, &stPkgRes.Body.FlushBattleQueue);
			break;
		case FLUSH_BATTLE_QUEUE_TEAM_INFO:
			ss_flush_battle_queue_team_info(pstEnv, &stPkgRes.Body.FlushBattleQueueTeamInfo);
			break; 
		case BATTLE_PROXY_ROUTINE:
			battle_proxy_respond(pstAppCtx, pstEnv, &stPkgRes.Body.BattleProxy);
			break;

		//Span
		case SPAN_ROLE_APPLY_REQ:
			player_span_apply(pstEnv, &stPkgRes.Body.SpanRoleApplyReq);
			break;
		case SPAN_ROLE_APPLY_RES:
			player_span_apply_res(pstEnv, &stPkgRes.Body.SpanRoleApplyRes);
			break;
		case SPAN_ROLE_HOLD_REQ:
			player_span_hold(pstEnv, &stPkgRes.Body.SpanRoleHoldReq);
			break;
		case SPAN_ROLE_HOLD_RES:
			player_span_hold_res(pstEnv, &stPkgRes.Body.SpanRoleHoldRes);
			break;
		case SPAN_ROLE_UPDATE_REQ:
			player_span_role_update(pstEnv, &stPkgRes.Body.SpanRoleUpdateReq);
			break;
		case SPAN_ROLE_UPDATE_RES:
			player_span_role_update_res(pstEnv, &stPkgRes.Body.SpanRoleUpdateRes);
			break;
		case SPAN_CHG_SVR:
			player_span_chg_svr(pstEnv, &stPkgRes.Body.SpanChgSvr);
			break;
		case SPAN_ROLE_LOGIN_REQ:
			player_span_login(pstEnv, &stPkgRes.Body.SpanRoleLoginReq);
			break;
		case SS_TEAM_SYNC:
			z_team_span_sync(pstEnv, &stPkgRes.Body.TeamSync);
			break;
		case SS_HEARTBEAT_REQ:
			player_span_heartbeat(pstEnv, &stPkgRes.Body.HeartBeatReq, iMsgSrc);
			break;
		case SS_PROTECT_DATA_SYNC:
			player_protect_data_sync(pstEnv, &stPkgRes.Body.ProtectDataSync);
			break;
		case SS_TEAM_DESTROY:
			z_team_span_destroy(pstEnv, &stPkgRes.Body.TeamDestroy);
			break;
		case SS_EXIT_PWORLD_QUEUE:
			player_span_exit_pworld_queue(pstEnv, &stPkgRes.Body.ExitPworldQueue);
			break;
		case SS_SPAN_CLEAN_LOGOUT:
			player_span_clean_logout(pstEnv, &stPkgRes.Body.SpanCleanLogout, iMsgSrc);
			break;
		case SS_SPAN_SAFELOCK:
			player_span_safelock(pstEnv, &stPkgRes.Body.SpanSafeLock);
			break;
		case SPAN_SPARTA_APPLY_REQ:
			player_sparta_apply(pstEnv, &stPkgRes.Body.SpanSpartaApplyReq);
			break;
		case SPAN_SPARTA_INVITE:
			span_sparta_invite(pstEnv, &stPkgRes.Body.SpanSpartaInvite);
			break;
		case SS_PKG_TRANS:
			span_pkg_trans(pstEnv, &stPkgRes);
			break;
		case SS_COROUTINE_RPC:
			z_ss_coroutine_rpc(pstEnv,&stPkgRes.Body.CoroutineRpc);
			break;
		case SPAN_STRONG_REQ:
			span_strong_ss_req(pstEnv,&stPkgRes); 	// zone->battle
			break;
		case SPAN_STRONG_RES:
			span_strong_ss_res(pstEnv,&stPkgRes);	// battle->zone
			break;
		case SPAN_MAP_CAMP_REQ:                 // zone_svr -> battle_svr
			span_map_camp_ss_req(pstEnv,&stPkgRes,iMsgSrc); 
			break;
		case SPAN_MAP_CAMP_RES:                //   battle_svr -> zone_svr
			span_map_camp_ss_res(pstEnv,&stPkgRes); 
			break;
		case SS_WORLD_BOSS_DATA_SYNC:       // 跨服野外 -> zone_svr
			z_ss_world_boss_data_sync(pstEnv, &stPkgRes.Body.WorldBossDataSync);
			break;
		default:		
		break;
	} 
		
	return 0;
}

//跨服是否允许检查
int player_span_state_check(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);
	
	if(0 > z_player_goto_check(pstEnv, pstPlayer, 0, NULL))
	{
		return -1;
	}

	if(subject_player_is_join(pstEnv, pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPAN201);
		return -1;
	}

	if(pstTeam && pstTeam->iAutoPworldID > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPAN202);
		return -1;
	}
	return 0;
}

//切换线路前清理下七七八八的状态
int player_span_state_clean(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);
	
	if(pstTeam)
	{
		TEAMMEMBER *pstTeamMember;
		
		pstTeamMember = player_team_get_member(pstTeam, pstPlayer->stRoleData.RoleName, NULL);
		if(pstTeamMember && pstTeamMember->EnterPworldFlag)
		{
			autopworld_invite_enter(pstEnv, NULL, pstTeam->iAutoPworldID, 1, pstTeam, 0);
		}
	}
		
	//统一打断一些吟唱
	player_trace_act_break_prepare(pstEnv, pstPlayer, BRK_PRE_ACT_SPAN_SVR);
	
	// 处理交易
	if (pstPlayer->stOnline.stExchgOnline.bExchgStat != EXCHG_NO)
	{
		exchg_logout(pstEnv, pstPlayer);
	}

	if(pstPlayer->stOnline.stCardBoxOL.iStat)
	{
		player_cardbox_close(pstEnv, pstPlayer);
	}
	
	// 清理跟随的玩家
	player_weixing_clear(pstEnv, pstPlayer);

	//清理马上得乘客
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

			//客人下马
			z_hitch_ride_clear(pstEnv, pstTmpPlayer);
		}
	}
	//搭乘的下马
	else if (pstPlayer->stOnline.State & CS_STAT_HITCH_RIDE)
	{
		z_ride_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}
	
	z_ride_break(pstEnv->pstAppCtx, pstEnv,  pstPlayer);

	//if (pstPlayer->stOnline.State & CS_STAT_FIGHT)
	{
		memset(pstPlayer->stOnline.abFollowAngel, 0, sizeof(pstPlayer->stOnline.abFollowAngel));
		SET_PLAYER_NOFIGHT(pstPlayer);
		z_send_player_fight(pstEnv, pstPlayer, CLEAN_FIGHT_STATUS);
	}

	if (pstPlayer->stOnline.State & CS_STAT_COLLECT)
	{
		player_collect_end(pstEnv, pstPlayer);
	}

	//处理摆摊
	if (CS_STAT_STALL & pstPlayer->stOnline.State)
	{
		player_stall_clear(pstEnv, pstPlayer);
	}

	pstPlayer->stOnline.cDialogStatOld = pstPlayer->stOnline.cDialogStat;
	pstPlayer->stOnline.cDialogStat = DIALOG_STAT_BEGIN;
	pstPlayer->stOnline.State &= ~CS_STAT_DIALOG;

	if(is_player_ride_on_machine(pstPlayer))
	{
		player_ride_machine_brk(pstEnv, pstPlayer, 1);
	}

	// 处理切磋
	player_logout_qiecuo(pstEnv, pstPlayer);

	// 处理组队跟随
	player_logout_cancel_follow(pstEnv, pstPlayer);

	// 队伍跟随下线取消
	player_team_follow_logout(pstEnv, pstPlayer);

	//答题退出
	subject_player_logout(pstEnv, pstPlayer);

	tmp_item_drop(pstEnv, pstPlayer,1);

	pet_fuse_cancel(pstEnv, pstPlayer);

	player_xukong_energy_clear(pstEnv, pstPlayer);

	return 0;   
}

//当前线路类型
int map_span_type_get(ZONESVRENV* pstEnv)
{
	if(INST_MODE_ZONE_SVR == pstEnv->iInstMode)
	{
		return MAP_SPAN_NONE;
	}
	else
	{
		if(is_battle_pve_battle_svr(pstEnv))
		{
			return MAP_SPAN_BATTLE;
		}
	}
	
	return MAP_SPAN_NONE;
}

int span_map_camp_init(ZONESVRENV* pstEnv)
{
	if(is_battle_pvp_svr(pstEnv))
	{
		int iNum = pstEnv->pstConf->SpanMapCampNum;
		if(iNum <= 0) return 0;
		int iWorldID = 0;
		int iBusID;
		int i = 0;

		SSPKG stPkg;
		SSSPANMAPCAMPRES *pstRes = &stPkg.Body.SpanMapCampRes;
		TDRDATA stNet;
		
		Z_SSHEAD_INIT(&stPkg.Head, SPAN_MAP_CAMP_RES);
		
		for(i=0;i<iNum;i++)
		{
			if(0 > get_world_by_camp(pstEnv,i,&iWorldID))
			{
				return -1;
			}

			pstRes->iCamp = i+1;

			stNet.iBuff = sizeof(pstEnv->szSendBuffer);
			stNet.pszBuff = pstEnv->szSendBuffer;
			if(ss_pack(pstEnv->pstSSHan, &stNet, &stPkg, 0) < 0)
			{
				continue;
			}
		
			iBusID = region_world_func_inst_aton_busid(pstEnv->ucRegionID, iWorldID, FUNC_ZONE_SVR, 1);
			
			tbus_send(pstEnv->pstAppCtx->iBus,  &pstEnv->pstAppCtx->iId, &iBusID, stNet.pszBuff, stNet.iBuff, 0);
		}
	}
	else
	{
		if( INST_MODE_ZONE_SVR != pstEnv->iInstMode)
		{
			return 0;
		}

		SSPKG stSSPkg;
		SSSPANMAPCAMPREQ *pstSpanMapCampReq = &stSSPkg.Body.SpanMapCampReq;
		pstSpanMapCampReq->req = 0;
		Z_SSHEAD_INIT(&stSSPkg.Head, SPAN_MAP_CAMP_REQ);
		z_ssmsg_send(pstEnv, &stSSPkg, pstEnv->iBattlePvpID);
	}

	return 0;
}

//battle  ->   zone
int span_map_camp_ss_res(ZONESVRENV * pstEnv,SSPKG *pstPkg)
{
	// 收到这个消息的必定是zone
	if( INST_MODE_ZONE_SVR != pstEnv->iInstMode || is_fini_global(pstEnv) == 0)
	{
		return 0;
	}

	SSSPANMAPCAMPRES *pstRes = &pstPkg->Body.SpanMapCampRes;
	SPANMAPGLOBAL *pstSpanMapGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.SpanMapGlobal;

	//if(pstRes->iCamp > 0)
	//{
		pstSpanMapGlobal->iCampIdx = pstRes->iCamp;
	//}
	
	return 0;
}

//zone -> battle -> zone
int span_map_camp_ss_req(ZONESVRENV * pstEnv,SSPKG *pstPkg,int iMsgSrc)
{
	if(!is_battle_pvp_svr(pstEnv))
	{
		return 0;
	}
	
	int iCampIdx = 0;

	if(get_span_map_camp_by_worldId(pstEnv,iMsgSrc,&iCampIdx) < 0)
	{
		return 0;
	}
	
	SSPKG stSSPkg;
	SSSPANMAPCAMPRES *pstSpanMapCampRes = &stSSPkg.Body.SpanMapCampRes;
	pstSpanMapCampRes->iCamp = iCampIdx;
	Z_SSHEAD_INIT(&stSSPkg.Head, SPAN_MAP_CAMP_RES);
	z_ssmsg_send(pstEnv, &stSSPkg, iMsgSrc);

	return 0;
	UNUSED(pstPkg);	//add by paraunused.pl
}

int get_span_map_camp_by_worldId(ZONESVRENV * pstEnv,int iMsgSrc,int *pCampIdx)
{
	int i = 0;
	int iNum = pstEnv->pstConf->SpanMapCampNum;
	if(iNum <= 0) return -1;
	int iCampWorldID = 0;
	int iWorldID = 0; 
	int k = 0;
	GET_WORLD_ID(iWorldID,iMsgSrc);
	
	for(i=0;i<iNum;i++)
	{
		if(0 > get_world_by_camp(pstEnv,i,&iCampWorldID))
		{
			return -1;
		}

		if(iCampWorldID == iWorldID)
		{
			k = i+1;
			*pCampIdx = k;
			return 0;
		}
	}
	
	return 0;
}

int get_world_by_camp(ZONESVRENV * pstEnv,int idx,int *iCampWorldID)
{
	switch(idx)
	{
		case 0:
			*iCampWorldID = pstEnv->pstConf->SpanMapCamp1;
			break;
		case 1:	
			*iCampWorldID = pstEnv->pstConf->SpanMapCamp2;
			break;
		case 2:
			*iCampWorldID = pstEnv->pstConf->SpanMapCamp3;
			break;
		case 3:	
			*iCampWorldID = pstEnv->pstConf->SpanMapCamp4;
			break;
		case 4:
			*iCampWorldID = pstEnv->pstConf->SpanMapCamp5;
			break;
		case 5:
			*iCampWorldID = pstEnv->pstConf->SpanMapCamp6;
			break;
		case 6:
			*iCampWorldID = pstEnv->pstConf->SpanMapCamp7;
			break;
		case 7:	
			*iCampWorldID = pstEnv->pstConf->SpanMapCamp8;
			break;	
		default:
			return -1;
			break;
	}
		
	return 0;
}

int z_ss_world_boss_data_sync(ZONESVRENV * pstEnv, SSWORLDBOSSDATASYNC *pstWorldBossDataSync)
{
	if( INST_MODE_ZONE_SVR != pstEnv->iInstMode)
	{
		return 0;
	}

	WORLDBOSSONE *pstOne = NULL;
	WORLDBOSS *pstWorldBoss = &pstEnv->pstZoneObj->stBoss;
	
	pstOne = boss_one_find(pstWorldBoss, pstWorldBossDataSync->iID);
	if(!pstOne)
	{
		return 0;
	}

	pstOne->iDayNum = pstWorldBossDataSync->iDayNum;
	pstOne->iFreshTime = pstWorldBossDataSync->iFreshTime;
	pstOne->iNextTime = pstWorldBossDataSync->iNextTime;
	pstOne->iStat = pstWorldBossDataSync->iStat;
	pstOne->iTip = pstWorldBossDataSync->iTip;
	
	return 0;
}

