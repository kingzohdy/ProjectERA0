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
#include "libdirty.h"
#include "ptgenimg.h"
#include "entity.h"
#include "tbus/tbus.h"
#include "tormapi/torm_protocol_api.h"
#include "mud_player.h"
#include "mud_db.h"
#include "op_log_meta.h"
#include "apexproxy/ApexProxyLib.h"
#include "mud_zh_name.h"

#define MUD_STOP_INTERVAL_CHECK 20
#define MUD_STOP_PATCH 50

static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_mud_svr[];
extern unsigned char g_szMetalib_proto_cs[];
extern unsigned char g_szMetalib_proto_ss[];

extern int tlog_init_cfg_from_file(TLOGCONF* a_pstConf, const char *a_pszPath);
extern unsigned char g_szMetalib_op_log_meta[];


MUD_SVRENV gs_stEnv;
static MUD_SVRRUN_STATUS gs_stStat;

static int ClanMemCacheHashPred(const void *pv1, const void *pv2)
{
	const ClanMemberCache *pstLhs = pv1, *pstRhs = pv2;
	return pstLhs->iClanID - pstRhs->iClanID;
}

static unsigned int ClanMemCacheHashGen(const void* pvData)
{
	const ClanMemberCache *pstCache = pvData;
	return (unsigned int)pstCache->iClanID;
}

const ClanMemberCache* clan_mem_cached_find(const CLANID *pstClanID)
{
	ClanMemberCache stKey, *pstFound = NULL;

	bzero(&stKey, sizeof(stKey));
	stKey.iClanID = pstClanID->Id;
	pstFound = sht_find(this_env()->pstShtClanMemberCache, &stKey,
	                    ClanMemCacheHashPred, ClanMemCacheHashGen);
	return pstFound;
}

int clan_mem_cached_add(const MudPlayer *pstPlayer)
{
	PlayerIdxEntries *pstPool = this_env()->pstPlayerIdxEntries;
	ClanMemberCache stKey, *pstFound = NULL;

	bzero(&stKey, sizeof(stKey));
	stKey.iClanID = pstPlayer->stRoleData.stClanInfo.Id;
	pstFound = sht_find(this_env()->pstShtClanMemberCache, &stKey,
	                    ClanMemCacheHashPred, ClanMemCacheHashGen);
	if(!pstFound)
	{
		pstFound = sht_insert_unique(this_env()->pstShtClanMemberCache, &stKey,
		                             ClanMemCacheHashPred, ClanMemCacheHashGen);
		if(!pstFound)
		{
			return -1; // 内存耗尽
		}
		pstFound->iClanID = stKey.iClanID;
		pstFound->iMemberEntry = -1;
		pstFound->iMemberNum = 0;
	}

	{
		// 取出一个结点:
		int iAllocated = pstPool->iHead;
		pstPool->iHead = pstPool->astEntries[pstPool->iHead].iNext;

		// 插入头部:
		pstPool->astEntries[iAllocated].iValue = pstPlayer->iIdx;
		pstPool->astEntries[iAllocated].iNext = pstFound->iMemberEntry;
		pstFound->iMemberEntry = iAllocated;
		++(pstFound->iMemberNum);
	}
	return 0;
}

int clan_mem_cached_del(const MudPlayer *pstPlayer)
{
	PlayerIdxEntries *pstPool = this_env()->pstPlayerIdxEntries;
	ClanMemberCache stKey, *pstFound = NULL;

	bzero(&stKey, sizeof(stKey));
	stKey.iClanID = pstPlayer->stRoleData.stClanInfo.Id;
	pstFound = sht_find(this_env()->pstShtClanMemberCache, &stKey,
	                    ClanMemCacheHashPred, ClanMemCacheHashGen);
	if(!pstFound)
	{
		return -1;
	}

	{
		// 取出结点:
		int i = -1, iPrev = -1, iFreed = -1;
		for(i = pstFound->iMemberEntry; i != -1;
		    i = pstPool->astEntries[i].iNext)
		{
			if(pstPool->astEntries[i].iValue == pstPlayer->iIdx)
			{
				iFreed = i;
				break;
			}
			iPrev = i;
		}

		if(i < 0)
		{
			return -1; // 没有找到结点
		}
		if(i == pstFound->iMemberEntry)
		{
			pstFound->iMemberEntry = 
				pstPool->astEntries[pstFound->iMemberEntry].iNext;
		}
		else
		{
			pstPool->astEntries[iPrev].iNext = pstPool->astEntries[i].iNext;
		}

		// 放回去:
		pstPool->astEntries[iFreed].iValue = -1;
		pstPool->astEntries[iFreed].iNext = pstPool->iHead;
		pstPool->iHead = iFreed;
		--(pstFound->iMemberNum);
	}

	if(!pstFound->iMemberNum)
	{
		// 没有结点了,删除缓存
		if(!sht_remove(this_env()->pstShtClanMemberCache, &stKey,
		               ClanMemCacheHashPred, ClanMemCacheHashGen))
		{
			return -1;
		}
	}
	return 0;
}

MudRoleData* role_to_mud(const ROLEDATA *pstFrom, MudRoleData *pstTo)
{
	pstTo->iPerm = pstFrom->Perm;
	pstTo->iUin = pstFrom->Uin;
	pstTo->ullRoleID = pstFrom->RoleID;
	STRNCPY(pstTo->szRoleName, pstFrom->RoleName, CCH(pstTo->szRoleName));

	// 时间记录
	pstTo->tBlockEnd = pstFrom->BlockEnd;
	pstTo->tCreateTime = pstFrom->CreateTime;
	pstTo->tLastLogin = pstFrom->LastLogin;
	pstTo->tLastLogout = pstFrom->LastLogout;
	pstTo->tSilenceEnd = pstFrom->SilenceEnd;

	// 外观属性
	pstTo->iGender = pstFrom->Gender;
	pstTo->iCareer = pstFrom->Career;
	pstTo->iLevel = pstFrom->Level;
	pstTo->iHead = pstFrom->Head;
	pstTo->iFace = pstFrom->Face;
	pstTo->iHair = pstFrom->Hair;
	pstTo->stAvatar.Num = pstFrom->AvatarProf.Num;
	pstTo->stAvatar.QiYue = pstFrom->AvatarProf.QiYue;
	memcpy(pstTo->stAvatar.Arms,
	       pstFrom->AvatarProf.Arms,
	       sizeof(pstFrom->AvatarProf.Arms[0]) * pstFrom->AvatarProf.Num);

	// 所有物
	pstTo->stWear.AmmMap = pstFrom->Wear.AmmMap;
	pstTo->stWear.ArmNum = pstFrom->Wear.ArmNum;
	memcpy(pstTo->stWear.RoleArms, pstFrom->Wear.RoleArms,
	       sizeof(pstFrom->Wear.RoleArms[0]) * pstFrom->Wear.ArmNum);

	// 关系
	pstTo->iFriendNum = pstFrom->Buddy.FriendNum;
	pstTo->iFriendMax = pstFrom->Buddy.FirendMax;
	memcpy(pstTo->astFriends, pstFrom->Buddy.Friends,
	       sizeof(pstFrom->Buddy.Friends[0]) *
	       pstFrom->Buddy.FriendNum);
	pstTo->iBadGuyNum = pstFrom->MiscInfo.BlackNum;
	memcpy(pstTo->astBadGuys, pstFrom->MiscInfo.BlackMember,
	       sizeof(pstFrom->MiscInfo.BlackMember[0]) *
	       pstFrom->MiscInfo.BlackNum);
	pstTo->stClanInfo = pstFrom->MiscInfo.ClanInfo.ClanId;

	//属性
	memcpy(&pstTo->AttrVal,&pstFrom->MiscInfo.AttrVal,sizeof(pstTo->AttrVal));
	pstTo->BaseAttrVal = pstFrom->MiscInfo.BaseAttrVal;

	//心情
	memcpy(pstTo->Emotion,pstFrom->Buddy.Emotion,CCH(pstTo->Emotion));
	pstTo->RoleMiscFlag = pstFrom->MiscInfo.Flag;
	
	return pstTo;
}

ROLEDATA* mud_to_role(const MudRoleData *pstFrom, ROLEDATA *pstTo)
{
	// TODO : 日~后再说
	UNUSED(pstFrom);
	UNUSED(pstTo);
	return NULL;
}

int mud_report_dir(TAPPCTX* pstAppCtx, MUD_SVRENV* pstEnv,unsigned short sWeihu)
{

	SSPKG *pstSSReq = &pstEnv->stSSReq;
	SSDIRREPORT *pstReport;
	TDRDATA stNet;

	Z_SSHEAD_INIT(&pstSSReq->Head, MUD_DIR_REPORT);
	pstReport = &pstSSReq->Body.MudDirReport;
	memset(pstReport, 0, sizeof(*pstReport));

	pstReport->WorldID = pstEnv->unWorldID;
	pstReport->PlayerMaxNum = pstEnv->pstPlayerPool->iMax;
	pstReport->PlayerNum = pstEnv->pstPlayerPool->iUsed;
	//pstReport->IP = pstEnv->iConnIP;
	STRNCPY(pstReport->IP, pstEnv->pstConf->ConnIP, sizeof(pstReport->IP));
	pstReport->Port = pstEnv->unPort;
	pstReport->RegionID = pstEnv->ucRegionID;
	pstReport->WeiHuFlag = sWeihu;
	pstReport->IP_Bak1 = (int)inet_addr(pstEnv->pstConf->ConnIP_bak1);
	pstReport->IP_Bak2 = (int)inet_addr(pstEnv->pstConf->ConnIP_bak2);
		

	stNet.pszBuff = pstEnv->szSendBuffer;
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	if (0 > ss_pack(pstEnv->pstSSHan, &stNet, pstSSReq, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "report dir ss_pack_err");
		return -1;
	}

	tnet_send(pstEnv->iDirSvrSock, stNet.pszBuff, stNet.iBuff, 0);
	tnet_send(pstEnv->iDirBakSock, stNet.pszBuff, stNet.iBuff, 0);

	return 0;
	UNUSED(pstAppCtx);
}

int reg_mud_clt_cmd(MUD_SVRENV* pstEnv, unsigned short unCmd,
                    CmdHandle pfnCmdHandle, char cStateLimit)
{
	MudCmdMap *pstCmdMap;
	
	assert(unCmd < MAX_CS_CMD);
	
	pstCmdMap = &pstEnv->astCmdMap[unCmd];
	pstCmdMap->cCtrlFlag = 1;
	pstCmdMap->cStateLimit = cStateLimit;
	pstCmdMap->pfnCmdHandle = pfnCmdHandle;

	return 0;
}

static int unreg_mud_clt_cmd(MUD_SVRENV *pstEnv, unsigned short unCmd)
{
	MudCmdMap *pstCmdMap;
	
	if (unCmd >= MAX_CS_CMD )
	{
		return -1;
	}
	
	pstCmdMap = &pstEnv->astCmdMap[unCmd];
	pstCmdMap->cCtrlFlag = 0;
	pstCmdMap->pfnCmdHandle = NULL;

	return 0;
}

static void init_player_idx_entries(PlayerIdxEntries *pstList)
{
	int i = 0;
	SListEntry *pstEntries = pstList->astEntries;

	pstList->iHead = 0;
	for(i = 0; i < (int)CCH(pstList->astEntries); ++i)
	{
		pstEntries[i].iValue = -1;
		pstEntries[i].iNext = i + 1;
	}
	pstEntries[CCH(pstList->astEntries) - 1].iNext = -1;
}

int mud_load_shm()
{
	MUD_SVRENV *pstEnv = this_env();
	int iShmID;
	int iIsExist=0;
	int iReload;
			
	iShmID = shmget(pstEnv->pstConf->ObjShmKey, sizeof(MudObj), 0666 | IPC_CREAT | IPC_EXCL);
	if( iShmID<0 )
	{
		iIsExist = 1;
		iShmID = shmget(pstEnv->pstConf->ObjShmKey, sizeof(MudObj), 0666);
	}
	
	if(iShmID < 0)
	{
		return -1;
	}
	
	if (pstEnv->pstMudObj)
	{
		shmdt(pstEnv->pstMudObj);
	}

	pstEnv->pstMudObj =	(MudObj *)shmat(iShmID, NULL, 0);
	
	if((void *) -1  == pstEnv->pstMudObj)
	{
		return -1;
	}

	memset(pstEnv->pstMudObj->sMudChatRoom, 0,sizeof(pstEnv->pstMudObj->sMudChatRoom));
			
	if (!iIsExist)
	{
		pstEnv->pstMudObj->iVersion = MUD_OBJ_SHM_VER;

		pstEnv->pstMudObj->iLevelUpDef = 0;
		pstEnv->pstMudObj->tLevelUpDef= 0;

		pstEnv->pstMudObj->iOfflineExpParameter = 0;
		pstEnv->pstMudObj->tOfflineExpParameter = 0;



		pstEnv->pstPlayerIdxEntries = 
			(PlayerIdxEntries *)pstEnv->pstMudObj->sPlayerIdxEntries;

		init_player_idx_entries(pstEnv->pstPlayerIdxEntries);

		// 初始化Name -> Player Pool Index 映射哈希表
		pstEnv->pstShtName2PlayerIdx = sht_init(
			pstEnv->pstMudObj->sName2PlayerIdxSht,
			sizeof(pstEnv->pstMudObj->sName2PlayerIdxSht),
			NAME2PLAYERIDX_BUCKET,
			MAX_MUD_PLAYER,
			sizeof(Name2PlayerIdx));
		if(!pstEnv->pstShtName2PlayerIdx)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,
			           "sht_init Name2PlayerIdx hash fail");
			return -1;
		}

		pstEnv->pstMudShtL2P = sht_init(
			pstEnv->pstMudObj->sMudL2PSht,
			sizeof(pstEnv->pstMudObj->sMudL2PSht),
			NAME2PLAYERIDX_BUCKET,
			MAX_MUD_PLAYER,
			sizeof(LoginName2MemID));
		if(!pstEnv->pstMudShtL2P)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,
			           "sht_init pstMudShtL2P hash fail");
			return -1;
		}

		// 初始化公会成员缓存
		pstEnv->pstShtClanMemberCache = sht_init(
			pstEnv->pstMudObj->sClanMemberCacheSht,
			sizeof(pstEnv->pstMudObj->sClanMemberCacheSht),
			CLAN_MEMBER_CACHE_BUCKET,
			CLAN_MAX_NUM_HARD,
			sizeof(ClanMemberCache));
		if(!pstEnv->pstShtClanMemberCache)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,
			           "sht_init ClanMemberCache hash fail");
			return -1;
		}
	}
	else
	{
		if (MUD_OBJ_SHM_VER != pstEnv->pstMudObj->iVersion)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "shm ver=%d", pstEnv->pstMudObj->iVersion);
			return -1;
		}

		pstEnv->pstPlayerIdxEntries = 
			(PlayerIdxEntries *)pstEnv->pstMudObj->sPlayerIdxEntries;

		//Attach Name -> Player Pool Index 映射哈希表
		pstEnv->pstShtName2PlayerIdx = sht_attach(
			pstEnv->pstMudObj->sName2PlayerIdxSht,
			sizeof(pstEnv->pstMudObj->sName2PlayerIdxSht),
			NAME2PLAYERIDX_BUCKET,
			MAX_MUD_PLAYER,
			sizeof(Name2PlayerIdx));
		if(!pstEnv->pstShtName2PlayerIdx)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,
			           "sht_attach Name2PlayerIdx hash fail");
			return -1;
		}

		pstEnv->pstMudShtL2P = sht_attach(
			pstEnv->pstMudObj->sMudL2PSht,
			sizeof(pstEnv->pstMudObj->sMudL2PSht),
			NAME2PLAYERIDX_BUCKET,
			MAX_MUD_PLAYER,
			sizeof(LoginName2MemID));
		if(!pstEnv->pstMudShtL2P)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,
			           "sht_init pstMudShtL2P hash fail");
			return -1;
		}

		//Attach 公会成员缓存
		pstEnv->pstShtClanMemberCache = sht_attach(
			pstEnv->pstMudObj->sClanMemberCacheSht,
			sizeof(pstEnv->pstMudObj->sClanMemberCacheSht),
			CLAN_MEMBER_CACHE_BUCKET,
			CLAN_MAX_NUM_HARD,
			sizeof(ClanMemberCache));
		if(!pstEnv->pstShtClanMemberCache)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,
			           "sht_attach ClanMemberCache hash fail");
			return -1;
		}
	}

	pstEnv->pstMudChatRoom = (MudChatRoom *)pstEnv->pstMudObj->sMudChatRoom;
	pstEnv->iObjShmID = iShmID;
	
	//load bin
	if(0 > load_resfile_one(
		RES_DIR,
		"LevelUP",
		pstEnv->pstMudObj->sLevelUpDef[0],
		&pstEnv->pstMudObj->iLevelUpDef,
		&pstEnv->pstMudObj->tLevelUpDef,
		LEVELUP_DEF_DATA,
		MAX_LEVEL,
		&iReload))
	{
		return -1;
	}

	if(0 > load_resfile_one(
		RES_DIR,
		"OfflineExpParameter",
		pstEnv->pstMudObj->sOfflineExpParameter[0],
		&pstEnv->pstMudObj->iOfflineExpParameter,
		&pstEnv->pstMudObj->tOfflineExpParameter,
		OFFLINE_EXP_PARAMETER_DATA,
		MAX_OFFLINE_EXP_PARAMETER,
		&iReload))
	{
		return -1;
	}
	
	return 0;
}

static int init_cmd_map(MUD_SVRENV* pstEnv)
{
	int i = 0;
	
	memset(pstEnv->astCmdMap, 0, sizeof(pstEnv->astCmdMap));

	reg_mud_clt_cmd(pstEnv, ACC_LOGIN_REQ, mud_acc_login_req, -1);
	reg_mud_clt_cmd(pstEnv, ACC_LOGOUT_REQ, mud_acc_logout_req, -1);
	reg_mud_clt_cmd(pstEnv, MUD_ROLE_LOGIN_REQ, mud_role_login_req,
	                MUD_PLAYER_STATUS_ROLE_LIST);
	reg_mud_clt_cmd(pstEnv, MUD_ROLE_LOGOUT_REQ, mud_role_logout_req,
	                MUD_PLAYER_STATUS_ROLE_LOGIN);
	reg_mud_clt_cmd(pstEnv, CHAT_REQ, mud_chat_req,
	                MUD_PLAYER_STATUS_ROLE_LOGIN);

	reg_mud_clt_cmd(pstEnv, HEARTBEAT_REQ, mud_player_heartbeat,
	                -1);
	reg_mud_clt_cmd(pstEnv, MUD_ENTER_CHAT_ROOM_REQ, mud_enter_chat_room_req,
					MUD_PLAYER_STATUS_ROLE_LOGIN);
	reg_mud_clt_cmd(pstEnv, MUD_CHAT_ROOM_LIST_REQ, mud_chat_room_list_req,
					MUD_PLAYER_STATUS_ROLE_LOGIN);
	reg_mud_clt_cmd(pstEnv, MUD_CHAT_CHANNEL_SET_REQ, mud_chat_channel_set_req,
					MUD_PLAYER_STATUS_ROLE_LOGIN);
	reg_mud_clt_cmd(pstEnv, MUD_OFFLINE_EXP_TAKE_REQ, mud_offline_exp_take_req,
					MUD_PLAYER_STATUS_ROLE_LOGIN);
	reg_mud_clt_cmd(pstEnv, WATCH_FRIEND_CLT, watch_player_clt,
					MUD_PLAYER_STATUS_ROLE_LOGIN);
	reg_mud_clt_cmd(pstEnv, RELATION_CLT, m_relation_clt,
					MUD_PLAYER_STATUS_ROLE_LOGIN);
	
	reg_mud_clt_cmd(pstEnv, MUD_OUT_CHAT_ROOM_REQ, mud_out_chat_room_req,
					MUD_PLAYER_STATUS_ROLE_LOGIN);

	
	
	// 把蛋疼的协议屏蔽了
	for(i = 0; i < pstEnv->pstConf->CmdLimitNum; i++)
	{
		unreg_mud_clt_cmd(pstEnv, pstEnv->pstConf->CmdLimitIds[i]);
	}
	return 0;
}

int z_tick_update()
{
	MUD_SVRENV *pstEnv = this_env();
	int i;
	MudPlayer *pstPlayer;

	for (i=0; i<pstEnv->iUpdateCount; i++)
	{
		if (pstEnv->iUpdatePos >= MAX_MUD_PLAYER)
		{
			pstEnv->iUpdatePos = 0;
		}
		
		pstPlayer = (MudPlayer *)tmempool_get_bypos(pstEnv->pstPlayerPool, pstEnv->iUpdatePos);
		pstEnv->iUpdatePos++;

		if (pstPlayer && MUD_PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus)
		{
			if (pstPlayer->stShadowData.iCacheFlag == CACHE_DIRTY)
			{
				db_update_shadow_req(pstPlayer, &pstPlayer->stShadowData);
			}
			
			if (pstPlayer->iBuddyDirty)
			{
				db_buddy_select_req(pstPlayer);
			}
		}
	}

	return 0;
}

static int init_pref(TAPPCTX *pstAppCtx, MUD_SVRENV *pstEnv)
{
	TDRDATA stInput;
	
	if(!pstAppCtx->stConfData.pszBuff ||
	   !pstAppCtx->stConfData.iMeta ||
	   !pstAppCtx->pszConfFile)
	{
		return -1;
	}

	memset(pstEnv, 0, sizeof(*pstEnv));
	
	pstEnv->pstAppCtx = pstAppCtx;
	pstEnv->pstConf = (MUD_SVRCONF *)pstAppCtx->stConfData.pszBuff;

	if (pstAppCtx->iNoLoadConf)
	{
		stInput.pszBuff	=	(char*)pstEnv->pstConf;
		stInput.iBuff	=	(int) sizeof(*(pstEnv->pstConf));

		if(tdr_input_file((LPTDRMETA)pstAppCtx->stConfData.iMeta, &stInput,
		                  pstAppCtx->pszConfFile, 0, 0) < 0)
		{
			return -1;
		}
	}

	pstEnv->pstConf = (MUD_SVRCONF *)pstAppCtx->stConfData.pszBuff;
	return 0;
}

static int init_communicator(TAPPCTX *pstAppCtx, MUD_SVRENV *pstEnv)
{
	if (pstAppCtx->iTimer >= 500)
	{
		pstEnv->iUpdateCount = MAX_MUD_PLAYER/(pstEnv->pstConf->UpdateInterval*1000.0/pstAppCtx->iTimer) +1;
		printf("update count=%d\n", pstEnv->iUpdateCount);
		tlog_info(pstEnv->pstLogCat, 0, 0, "update count=%d", pstEnv->iUpdateCount);
	}
	else
	{
		printf("timer=%d\n", pstAppCtx->iTimer);
		return -1;	
	}
	
	if(tconnapi_initialize(pstAppCtx->pszGCIMKey, pstAppCtx->iBusinessID) < 0)
	{
		printf("tbus gcim key %s init fail\n", pstAppCtx->pszGCIMKey);
		return -1;
	}
	
	if(tconnapi_create(pstAppCtx->iId, &pstEnv->iTconnapiHandle) < 0)
	{
		printf("tbus id %s init fail\n", pstAppCtx->pszId);
		return -1;
	}
	
	if(net_han_create_c(g_szMetalib_proto_cs, &pstEnv->pstNetHan) < 0)
	{
		return -1;
	}

	if(ss_han_create(g_szMetalib_proto_ss, &pstEnv->pstSSHan) < 0)
	{
		return -1;
	}

	GET_WORLD_ENTITY_ID(pstEnv->iMudConnID, pstAppCtx->iId, FUNC_MUD_CONN);
	GET_WORLD_ENTITY_ID(pstEnv->iZoneSvrID, pstAppCtx->iId, FUNC_ZONE_SVR);
	GET_WORLD_ENTITY_ID(pstEnv->iRoleDBID, pstAppCtx->iId, FUNC_ROLE_DB);
	GET_WORLD_ENTITY_ID(pstEnv->iMiscDBID, pstAppCtx->iId, FUNC_MISC_DB);
	GET_REGION_ENTITY_ID(pstEnv->iRegionGateID, pstEnv->pstAppCtx->iId,
	                     FUNC_REGION_GATE);
	GET_WORLD_ENTITY_ID(pstEnv->iMudCtrlID, pstAppCtx->iId, FUNC_MUD_CTRL);


	GET_WORLD_ID(pstEnv->unWorldID, pstAppCtx->iId);
	GET_REGION_ID(pstEnv->ucRegionID, pstAppCtx->iId);

	pstEnv->pstBlackWhiteH = NULL;
	if(pstEnv->pstConf->BlackWhiteCfgPath[0] &&
	   black_white_init(pstEnv->pstConf->BlackWhiteCfgPath,
	                    &pstEnv->pstBlackWhiteH) < 0)
	{
		printf("%s black_white_init fail\n", pstEnv->pstConf->BlackWhiteCfgPath);		
	}

	pstEnv->iDirSvrSock = tnet_connect(pstEnv->pstConf->DirMasterUrl, 0);
	if(pstEnv->iDirSvrSock < 0)
	{
		perror("tnet connect dir master error");
		return -1;
	}

	pstEnv->iDirBakSock = tnet_connect(pstEnv->pstConf->DirBakUrl, 0);
	if(pstEnv->iDirBakSock < 0)
	{
		perror("tnet connect dir bak error");
		return -1;
	}

	//pstEnv->iConnIP =(int)inet_addr(pstEnv->pstConf->ConnIP);
	pstEnv->unPort = HTONS(pstEnv->pstConf->ConnPort);

	if(torm_create_handle_from_dir(&pstEnv->hTorm, pstEnv->pstConf->DBMetaDir)
	   < 0)
	{
		printf("torm_create_handle fail\n");
		return -1;
	}

	pstEnv->iRoleDBProtoVer = 0;
	pstEnv->iMiscDBProtoVer = 0;
	mud_torm_exchg_ver();

	return 0;
}

int m_oplog_init()
{
	MUD_SVRENV* pstEnv = this_env();
	char szPath[PATH_MAX];

	snprintf(szPath, sizeof(szPath), "../cfg/op_log_%s_66.xml", tbus_addr_ntoa( (TBUSADDR) pstEnv->pstAppCtx->iId) );
	szPath[sizeof(szPath) - 1] = 0;
	
	if (0 > tlog_init_cfg_from_file(&pstEnv->stOpLogConf, szPath))	
	{		
		printf("load op log cfg file fail\n");
		return -1;	
	}	

	if (0 > tlog_init(&pstEnv->stOpLogCtx, &pstEnv->stOpLogConf))
	{		
		printf("init op log cfg file fail\n");
		return -1;
	}

	pstEnv->pstOpLogCat = tlog_get_category(&pstEnv->stOpLogCtx, "oplog_net");
	if (NULL == pstEnv->pstOpLogCat)
	{		
		printf("load op log cfg filefail\n");
		return -1;
	}

	pstEnv->pstLogicLogMeta = tdr_get_meta_by_name((LPTDRMETALIB)g_szMetalib_op_log_meta, "op_log_meta");	
	if (NULL == pstEnv->pstLogicLogMeta)
	{
		return -1;
	}

	return 0;
}

int m_oplog_fini()
{
	MUD_SVRENV* pstEnv = this_env();
	tlog_fini(&pstEnv->stOpLogCtx);
	return 0;
}

int m_oplog_send(OP_LOG_META *pstOpLog)
{
	TDRDATA stHost;	
	TDRDATA stNet;
	char sBuff[2*sizeof(OP_LOG_META)];
	MUD_SVRENV* pstEnv = this_env();

	stNet.iBuff = sizeof(sBuff);
	stNet.pszBuff = sBuff;
	stHost.iBuff = sizeof(OP_LOG_META);
	stHost.pszBuff = (char *)pstOpLog;	

	if (0 > tdr_hton(pstEnv->pstLogicLogMeta, &stNet, &stHost, TDR_METALIB_OP_LOG_META_VERSION))
	{
		return -1;
	}	

	tlog_info_bin(pstEnv->pstOpLogCat, 0, 0, 0, TDR_METALIB_OP_LOG_META_VERSION, stNet.pszBuff, stNet.iBuff);
	
	return 0;
}



static int init_misc(TAPPCTX *pstAppCtx, MUD_SVRENV *pstEnv)
{
	pstEnv->pstRunStat = (MUD_SVRRUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
	if (NULL == pstEnv->pstRunStat)
	{
		pstEnv->pstRunStat = &gs_stStat;
	}

	if(tapp_get_category(NULL, (void *)(&pstEnv->pstLogCat)) < 0)
	{		
		printf("tapp_get_category run fail\n");
		return -1;	
	}

	if (0 > m_oplog_init())
	{
		printf("m_oplog_init fail\n");
		tlog_error(pstEnv->pstLogCat, 0, 0, "m_oplog_init fail");
		return -1;
	}

	if(mud_attach_player() < 0)
	{
		printf("attach player shm fail\n");
		tlog_error(pstEnv->pstLogCat,0,0,"attach player fail");
		return -1;
	}

	pstEnv->pstShtUinCache = mud_attach_uincache();
	if(!pstEnv->pstShtUinCache)
	{
		printf("attach uin cache shm fail\n");
		tlog_error(pstEnv->pstLogCat,0,0,"attach uin cache shm fail");
		return -1;
	}

	if (0 > mud_load_shm())
	{
		printf("z_load_shm fail\n");
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_load_shm fail");
		return -1;
	}


	InitDirtySystem("../cfg/dirty/dirtyword.txt");

	
	return 0;
}

int mud_svr_init(TAPPCTX *pstAppCtx, MUD_SVRENV *pstEnv)
{
	SSCMDDATA stData;
	// STEP 1 : 预初始化
	if(init_pref(pstAppCtx, pstEnv) < 0)
	{
		printf("pref initialize fail.\n");
		return -1;
	}

	// STEP 2 : 杂项初始化
	if(init_misc(pstAppCtx, pstEnv) < 0)
	{
		printf("misc initialize fail.\n");
		return -1;
	}

	// STEP 3 : 通信相关初始化
	if(init_communicator(pstAppCtx, pstEnv) < 0)
	{
		printf("communicator initialize fail.\n");
		return -1;
	}

	// STEP 4 : 命令处理初始化
	if(init_cmd_map(pstEnv) < 0)
	{
		return -1;
	}

	if (0 == pstEnv->pstConf->NoApex)
	{
		if (0 != s_start(NULL))
		{
			printf("apex start fail\n");
			return -1;
		}
	}
	
	printf("mud_svr start\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "mud_svr start");

	stData.SyncTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	m_cmd_to_zone(NULL, NULL, SS_CMD_SYNC_TIME, &stData);
	
	return 0;
}

int mud_apex_get()
{
	char cMsgId;
	SSPKG stPkg;
	SSAPEX *pstApex = &stPkg.Body.Apex;
	unsigned int uUserID;
	int iRetLen;
	char sData[MAX_APEX_DATA];
	
	MUD_SVRENV *pstEnv = this_env();

	if (0 != pstEnv->pstConf->NoApex)
	{
		return 0;
	}

	if (0 != s_getData_FromApex(sData, sizeof(sData),
								&cMsgId, &uUserID, &iRetLen))
	{
		return -1;
	}

	pstApex->Uin = uUserID;

	switch (cMsgId)
	{
		case  'D':
			pstApex->ApexMsgType = APEX_MSG_DATA;
			pstApex->ApexMsg.ApexData.DataLen = iRetLen;
			if (pstApex->ApexMsg.ApexData.DataLen > MAX_APEX_DATA)
			{
				return -1;
			}

			memcpy(pstApex->ApexMsg.ApexData.Data, sData, pstApex->ApexMsg.ApexData.DataLen);
			
			Z_SSHEAD_INIT(&stPkg.Head, SS_APEX);
			mud_ssmsg_send(&stPkg, this_env()->iZoneSvrID);
			break;
		case 'K':
			pstApex->ApexMsgType = APEX_MSG_KICK;
			pstApex->ApexMsg.KickInfo.ApexData.DataLen = iRetLen;
			if (pstApex->ApexMsg.KickInfo.ApexData.DataLen > MAX_APEX_DATA)
			{
				return -1;
			}

			memcpy(pstApex->ApexMsg.KickInfo.ApexData.Data, sData, 
						pstApex->ApexMsg.KickInfo.ApexData.DataLen);
			
			Z_SSHEAD_INIT(&stPkg.Head, SS_APEX);
			mud_ssmsg_send(&stPkg, this_env()->iZoneSvrID);
			break;
		default:
			break;
	}
	
	return 0;
}

int mud_world_tick(int iBusy)
{
	MUD_SVRENV *pstEnv = this_env();

	if (pstEnv->cShutdown > 1)
	{
		return 0;
	}
	
	if (iBusy)
	{
		mud_active_player(20); 
	}
	else
	{
		mud_active_player(80); 
	}

	mud_apex_get();

	return 0;
}

int m_ctrl_msg( char *pszMsg, int iMsgLen)
{
	int iSrc;
	int iDst;
	SSZONECTRL *pstZoneCtrlReq;
	SSZONECTRL *pstZoneCtrlRes;
	TDRDATA stNet;
	MUD_SVRENV* pstEnv = this_env();
	TAPPCTX* pstAppCtx = pstEnv->pstAppCtx; 
	SSPKG *pstSSRes = &pstEnv->stSSRes;

	if (0 > ss_unpack(pstEnv->pstSSHan, pszMsg, iMsgLen, &pstEnv->stSSReq, 0))
	{
		return -1;
	}
			
	tlog_debug_dr(pstEnv->pstLogCat, 0, 0, pstEnv->pstSSHan->pstSSProtoPkgMeta, 
					&pstEnv->stSSReq, sizeof(pstEnv->stSSReq), 0);

	if (ZONE_CTRL != pstEnv->stSSReq.Head.Cmd)
	{
		return -1;
	}

	pstZoneCtrlReq = &pstEnv->stSSReq.Body.ZoneCtrl;

	tlog_info(pstEnv->pstLogCat, 0, 0, "recv zone ctrl cmd=%d", pstZoneCtrlReq->Cmd);

	Z_SSHEAD_INIT(&pstSSRes->Head, ZONE_CTRL);
	pstZoneCtrlRes = &pstSSRes->Body.ZoneCtrl;
	switch(pstZoneCtrlReq->Cmd)
	{
	case TEST_REQ:
		pstZoneCtrlRes->Cmd = TEST_RES;
		pstZoneCtrlRes->CtrlCmd.TestRes.Result = 0;
		break;
		
	case SHUTDOWN_REQ:
		pstZoneCtrlRes->Cmd = SHUTDOWN_RES;
		pstZoneCtrlRes->CtrlCmd.ShutDownRes.Result = 0;
		pstEnv->cShutdown = 1; // 转入关闭服务器第1阶段
		pstEnv->cClearShm = 1;
		pstEnv->ucShutdownCount = pstZoneCtrlReq->CtrlCmd.ShutDownReq.Time/MUD_SHUTDOWN_NOTIFY_TIMER;
		pstEnv->tShutdownStart = pstAppCtx->stCurr.tv_sec;
		tos_send_signal( pstAppCtx->pszPidFile, pstAppCtx->pszApp, SIGUSR1, NULL );
		break;
		
	case LOADSCRIPT_REQ:
	case RELOAD_REQ:
	default:
		break;
		
	}

	stNet.pszBuff = pstEnv->szSendBuffer;
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	if (0 > ss_pack(pstEnv->pstSSHan, &stNet, pstSSRes, 0))
	{
		return 0;
	}

	iSrc = pstAppCtx->iId;
	iDst = 0;
	tbus_backward(pstAppCtx->iBus, &iSrc, &iDst, stNet.pszBuff, stNet.iBuff, 0);

	return 0;
}

int mud_svr_proc(TAPPCTX *pstAppCtx, MUD_SVRENV *pstEnv)
{
	int iBusy=0;
	int iLoopNum = 0;
	TBUSADDR iMsgDst ;
	TBUSADDR iMsgSrc ;
	int iMsgLen;
	char *pszMsg;
	int iRet = 0;

	while (LOOP_NUM > iLoopNum)
	{
		iLoopNum++;
		
		iMsgDst = pstAppCtx->iId;
		iMsgSrc = TBUS_ADDR_ALL;
		iMsgLen = sizeof(pstEnv->szRecvBuffer);
		pszMsg = &pstEnv->szRecvBuffer[0];
		if ( TBUS_SUCCESS != tbus_recv(pstAppCtx->iBus, &iMsgSrc, &iMsgDst, 
			(void *)pszMsg, (size_t *)&iMsgLen, 0) )
		{
			break;
		}
		iBusy = 1;

		pstEnv->pstRunStat->RecvCltMsg++;
		if(iMsgSrc == pstEnv->iMudConnID)
		{
			TFRAMEHEAD stFrameHead;
			int iFrameHeadLen = sizeof(stFrameHead);
			
			iRet = tconnapi_decode(pszMsg, iMsgLen, &stFrameHead, &iFrameHeadLen);
			if (0 != iRet)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0,
				           "tconnapi_decode failed, iRet0x%x, Msglen:%d",
				           iRet, iMsgLen);
				continue;
			}

			pszMsg += iFrameHeadLen;
			iMsgLen -= iFrameHeadLen;
			iRet = mud_dispatch_clt_msg(pstEnv, pszMsg, iMsgLen, &stFrameHead);
		}
		else if(iMsgSrc == pstEnv->iRoleDBID || iMsgSrc == pstEnv->iMiscDBID)
		{
			iRet = mud_dispatch_db_msg(pstEnv, pszMsg, iMsgLen, iMsgSrc);
		}
		else if(iMsgSrc == pstEnv->iRegionGateID)
		{
			iRet = mud_region_msg(pszMsg, iMsgLen);
		}
		else if(iMsgSrc == pstEnv->iZoneSvrID)
		{
			iRet = mud_zone_msg(pszMsg, iMsgLen);
		}
		else if(iMsgSrc == pstEnv->iMudCtrlID)
		{
			iRet = m_ctrl_msg(pszMsg, iMsgLen);
		}
		else
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "unspported msg source(%s)",
			           tbus_addr_ntoa(iMsgSrc));
		}
	}

	mud_world_tick(iBusy);
	
	if(iBusy)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int mud_svr_reload(TAPPCTX *pstAppCtx, MUD_SVRENV *pstEnv)
{
	MUD_SVRCONF   *pstPreConf;
	
	pstPreConf = (MUD_SVRCONF   *)pstAppCtx->stConfPrepareData.pszBuff;
	
	printf("mud_svr reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "mud_svr reload");

	return 0;
}

int mud_svr_fini(TAPPCTX *pstAppCtx, MUD_SVRENV *pstEnv)
{

	int iRet;

	printf("mud_svr finish\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "mud_svr finish");

	if (pstEnv->pstBlackWhiteH)
	{
		black_white_fini(&pstEnv->pstBlackWhiteH);
	}
	
	sht_destroy(&pstEnv->pstShtUinCache);
	tconnapi_free(&pstEnv->iTconnapiHandle);
	net_han_destroy(&pstEnv->pstNetHan);
	ss_han_destroy(&pstEnv->pstSSHan);

	if (pstEnv->cClearShm)
	{
		//删除player shm
		iRet = tshmdt(pstEnv->pstPlayerShm);
		iRet = tshmclose(pstEnv->iPlayerShmID, TSHMF_DELETE);

		//删除uin cache shm
		iRet = tshmdt(pstEnv->pstShtUinCache);
		iRet = tshmclose(pstEnv->iUinCacheShmID, TSHMF_DELETE);

		//删除Obj shm
		iRet = tshmdt(pstEnv->pstMudObj);
		iRet = tshmclose(pstEnv->iObjShmID, TSHMF_DELETE);
	}

	m_oplog_fini();

	if (0 == pstEnv->pstConf->NoApex)
	{
		s_end();
	}
	
	return 0;
	UNUSED(pstAppCtx);
}

int mud_svr_tick(TAPPCTX *pstAppCtx, MUD_SVRENV *pstEnv)
{
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);

	z_tick_update();


	if (pstEnv->tDirReport + DIR_REPORT_TIMER < pstAppCtx->stCurr.tv_sec)
	{
		pstEnv->tDirReport = pstAppCtx->stCurr.tv_sec;
		mud_report_dir(pstAppCtx, pstEnv,0);
	}

	return 0;
}

int m_kickoff_all( MUD_SVRENV *pstEnv)
{
	int i = 0;
	TMEMBLOCK* pstMemBlock = NULL;
	MudPlayer* pstPlayer = NULL;
	int iCount = 0;
	
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
		mud_player_kickoff(pstPlayer,LJY_MUD41);
		
		iCount++;

		//一次少搞一些，避免bus 通道拥塞和db瓶颈
		if (iCount > MUD_STOP_PATCH) 
		{
			usleep(MUD_STOP_PATCH*1000); // 1ms存一个player
			iCount = 0;

		}
	}

	usleep(MUD_STOP_PATCH*1000);
	
	return 0;
}

int sec2str(char *pszBuff, int iLen, int iTimeSec)
{
	int iMinute, iSec;

	pszBuff[0] = 0;
	iMinute = iTimeSec/60;
	iSec = iTimeSec;

	if(iMinute >= 5)
	{
		/*
		if(iSec > 0)
		{
			snprintf(pszBuff, iLen, "%d分钟%d秒", iMinute, iSec);
		}
		else
		*/
		{
			snprintf(pszBuff, iLen, "%d%s", iMinute, LJY_MUD42);
		}
	}
	else
	{
		snprintf(pszBuff, iLen, "%d%s", iSec, LJY_MUD43);
	}
	
	return 0;
}

int mud_svr_stop(TAPPCTX *pstAppCtx, MUD_SVRENV *pstEnv)
{

	switch (pstEnv->cShutdown)
	{
	case 0: 		
		pstEnv->cShutdown = 2;
		return 0;
		break;
		
	case 1://阶段1
		{
			CSPKG* pstCSRes = &pstEnv->stCSRes;
			CSSYSMSG* pstMsg = NULL;
			
			if (0 == pstEnv->ucShutdownCount)
			{
				pstEnv->cShutdown++; // 通知完成，转入阶段2
				return 0;
			}
			
			if (pstAppCtx->stCurr.tv_sec - pstEnv->tShutdownStart >= MUD_SHUTDOWN_NOTIFY_TIMER)
			{
				pstEnv->tShutdownStart = pstAppCtx->stCurr.tv_sec;
				pstEnv->ucShutdownCount--;

				if (pstEnv->ucShutdownCount > 0)
				{
					char szBuff[256];

					Z_CSHEAD_INIT(&pstCSRes->Head, SYS_MSG);
					pstMsg = &pstCSRes->Body.SysMsg;
					pstMsg->Type = SYS_SVR;
					pstMsg->Times = 0;
					pstMsg->Interval = 0;
					pstMsg->LinkLen = 0;

					sec2str(szBuff, sizeof(szBuff), pstEnv->ucShutdownCount*MUD_SHUTDOWN_NOTIFY_TIMER);
					
					snprintf(pstMsg->Msg, sizeof(pstMsg->Msg), "服务器将在%s后关闭!", szBuff);
					pstMsg->Msg[sizeof(pstMsg->Msg) - 1] = 0;
					
					mud_cltmsg_broadcast(pstCSRes);
				}
			}
		}
		return 0;
		break;
		
	case 2: // 阶段2 尝试踢出所有玩家
		{
			mud_report_dir(pstAppCtx,pstEnv,1);			
		
			tlog_info(pstEnv->pstLogCat, 0, 0, "kick off all player begin");
			m_kickoff_all( pstEnv);
			pstEnv->cShutdown++;
			pstEnv->tShutdownStart = pstAppCtx->stCurr.tv_sec;
		}
		return 0;
		break;

	case 3: // 阶段3 尝试再踢
		{
			int i = 0;			
			
			if (pstAppCtx->stCurr.tv_sec - pstEnv->tShutdownStart > MUD_STOP_INTERVAL_CHECK)
			{
				pstEnv->tShutdownStart = pstAppCtx->stCurr.tv_sec;
				m_kickoff_all( pstEnv);
			}
			
			for (i=0; i<MAX_MUD_PLAYER; i++)
			{
				MudPlayer* pstPlayer = (MudPlayer*)tmempool_get_bypos(pstEnv->pstPlayerPool, i);
				if (pstPlayer)
				{
					tlog_info(pstEnv->pstLogCat, 0, 0, "kick off all player %d left", pstEnv->pstPlayerPool->iUsed);
					return 0;
				}
			}
			tlog_info(pstEnv->pstLogCat, 0, 0, "kick off all player end");
			pstEnv->cShutdown++;
		}
		return 0;
		break;
		
	default:
		return -1;
		break;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	int iRet;
	void* pvArg	=	&gs_stEnv;

	memset(&gs_stAppCtx, 0, sizeof(gs_stAppCtx));
	memset(&gs_stEnv, 0, sizeof(gs_stEnv));

	gs_stAppCtx.argc	=	argc;
	gs_stAppCtx.argv	=	argv;
	gs_stAppCtx.iTimer = 1000;

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)mud_svr_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_mud_svr;
	gs_stAppCtx.stConfData.pszMetaName = "mud_svrconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);
		
	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)mud_svr_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)mud_svr_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)mud_svr_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)mud_svr_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)mud_svr_stop;
	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	if (TAPP_EXIT_QUIT == tapp_is_exit())
	{
		gs_stEnv.cClearShm = 0;
	}
	else if (TAPP_EXIT_STOP == tapp_is_exit())
	{
		gs_stEnv.cClearShm = 1;
	}

	tapp_def_fini(&gs_stAppCtx, pvArg);
	
	return iRet;
}


