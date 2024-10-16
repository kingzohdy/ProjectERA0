#include "zone_svr.h"
#include "comm/tmempool.h"
#include "zone_player.h"
#include "tconnapi.h"
#include "zone_clt.h"
#include "zone_db.h"
#include "version.h"
#include "comm/shtable_shm.h"
#include "zone_ctrl.h"
#include "tresloader.h"
#include "zone_map.h"
#include "zone_move.h"
#include "zone_gm.h"
#include "zone_err.h"
#include "zone_act_status.h"
#include "zone_combat.h"
#include "zone_npc.h"
#include "zone_script.h"
#include "zone_status.h"
#include "zone_oplog.h"
#include "zone_package.h"
#include "libdirty.h"
#include "zone_ai.h"
#include "zone_attr.h"
#include "zone_trans.h"
#include "zone_team.h"
#include "zone_script.h"
#include "ptgenimg.h"
#include "zone_tmp.h"
#include "zone_task.h"
#include "zone_relation.h"
#include "zone_skill.h"
#include "zone_pet.h"
#include "zone_clan.h"
#include "zone_exchg.h"
#include "zone_clan.h"
#include "zone_mail.h"
#include "zone_stall.h"
#include "zone_arm.h"
#include "zone_status.h"
#include "zone_respact.h"
#include "zone_designation.h"
#include "zone_fairy.h"
#include "zone_shop.h"
#include "zone_range.h"
#include "zone_autoteam.h"
#include "zone_secondaryskill.h"
#include "zone_machine.h"
#include "zone_misc.h"
#include "zone_rune.h"
#include "zone_roll.h"
#include "zone_collect.h"
#include "zone_battle.h"
#include "zone_itemlimit.h"
#include "zone_val.h"
#include "zone_booty.h"
#include "zone_diffpworld.h"
#include "zone_spansvr.h"
#include "zone_safelock.h"
#include "zone_city.h"
#include "zone_clan_city.h"
#include "zone_vipbar.h"
#include "zone_unreal.h"
#include "zone_addattr.h"
#include "zone_shangjin.h"
#include "zone_aas.h"
#include "zone_subject.h"
#include "zone_gay.h"
#include "zone_strongpoint.h"
#include "zone_autopworld.h"
#include "zone_span.h"
#include "zone_ronglu.h"
#include "zone_genius.h"
#include "zone_boss.h"
#include "zone_levfeng.h"
#include "zone_apex.h"
#include "zone_weather.h"
#include "zone_busy.h"
#include "zone_sparta.h"
#include "zone_pworld.h"
#include "zone_vip.h"
#include "zone_clanboon.h"
#include "zone_sparta.h"
#include "zone_second_pwd.h"
#include "zone_back.h"
#include "zone_syncrelation.h"
#include "zone_baoxiang.h"
#include "zone_store.h"
#include "zone_zh_name.h"
#include "zone_clan_pet.h"
#include "zone_dianfeng.h"
#include "zone_span_strong.h"
#include "zone_home.h"
#include "zone_shikong.h"

//#include "cs.pb-c.h"

#define LOOP_NUM 100
#define DIR_REPORT_TIMER 15
#define STOP_INTERVAL_CHECK 20
#define STOP_PATCH 50

#define GD_CODE_LEN 4

#define UIN_CACHE MAX_PLAYER*10

#define BULLETIN_ACTIVE_TIMER 1000


ZONESVRENV gs_stEnv;
static ZONE_SVRRUN_STATUS gs_stStat;
time_t g_tDefCheckTime = 0;

int z_silence_account(ZONESVRENV* pstEnv, 
	                    const ZONECTLSILENCEACCOUNTREQ *pstCtlReq);

int z_block_account(ZONESVRENV* pstEnv,
	                  const ZONECTLBLOCKACCOUNTREQ *pstCtlReq);


static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_zone_svr[];
extern unsigned char g_szMetalib_proto_cs[];
extern unsigned char g_szMetalib_proto_ss[];

extern int iStatusSeq;

extern ScriptEnv stSEnv;

int UinHashCmp( const void * pv1 , const void *pv2 )
{
	UinCache *pstUinHashData1 = (UinCache *)pv1;
	UinCache *pstUinHashData2 = (UinCache *)pv2;

	return pstUinHashData1->iUin - pstUinHashData2->iUin;
}

unsigned int UinHashCode(const void* pvData)
{
	UinCache *pstUinShtData = (UinCache *)pvData;
	
	return (unsigned int)pstUinShtData->iUin;
}

int EventHashCmp( const void * pv1 , const void *pv2 )
{
	GameEventInst *pstHashData1 = (GameEventInst *)pv1;
	GameEventInst *pstHashData2 = (GameEventInst *)pv2;

	return pstHashData1->iEventID - pstHashData2->iEventID;
}

unsigned int EventHashCode(const void* pvData)
{
	GameEventInst *pstEventShtData = (GameEventInst *)pvData;
	
	return (unsigned int)pstEventShtData->iEventID;
}

int NameMemIDHashCmp( const void * pv1 , const void *pv2 )
{
	NameMemID *pstNameMemID1 = (NameMemID *)pv1;
	NameMemID *pstNameMemID2 = (NameMemID *)pv2;

	return strcmp(pstNameMemID1->szName, pstNameMemID2->szName);
}

unsigned int NameMemIDHashCode(const void* pvData)
{
	NameMemID *pstNameMemID = (NameMemID *)pvData;
	
	return sht_get_code(pstNameMemID->szName);
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

LPSHTABLE z_attach_uincache(ZONESVRENV* pstEnv)
{
	LPSHTABLE pstShtTable;
	int iShmID;
	 
	pstShtTable = sht_create_shm(UIN_CACHE*3, UIN_CACHE, sizeof(UinCache), pstEnv->pstConf->UinCacheShmKey);
	iShmID	=	shmget(pstEnv->pstConf->UinCacheShmKey, 0, 0666 );
	if (0 > iShmID)
	{
		return NULL;
	}

	pstEnv->iUinCacheShmID = iShmID;

	return pstShtTable;
}

int z_free_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	UinCache stUinCache;
	UinCache *pstUinCache;
	int iUin = pstPlayer->stRoleData.Uin;
	NameMemID stNameMemID;
	UNUSED(pstAppCtx);

	if ( pstPlayer->bAasFlag )
	{
		send_to_aas(pstEnv, pstPlayer->stRoleData.Uin, AAS_GAME_END);
	}

	tlog_debug(pstEnv->pstLogCat, iUin, 0, "uin=%d player free", iUin);
	z_acc_login_oplog(pstAppCtx, pstEnv,iUin, pstPlayer->szAccount, 0,pstPlayer);

	memset(&stNameMemID,0,sizeof(stNameMemID));
	STRNCPY(stNameMemID.szName, pstPlayer->stRoleData.RoleName, sizeof(stNameMemID.szName));
	if (NULL == sht_remove(pstEnv->pstShtNameMemID, &stNameMemID, NameMemIDHashCmp, NameMemIDHashCode))
	{
		/*tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
			"sht_remove %d player fail, status = %d, reason = %d",
			pstPlayer->stRoleData.Uin,
			pstPlayer->eStatus,
			pstPlayer->ucRoleLogoutReason );*/
	}
		
	if (0 > z_id_clean(pstEnv, pstPlayer->iMemID))
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player clean id %d fail", pstPlayer->stRoleData.Uin, pstPlayer->iMemID);
		assert(0);
	}

	// 取消Login Name -> Player的映射
	{
		LoginName2MemID stDummy;
		STRNCPY(stDummy.szLoginName, pstPlayer->szAccount,
		        CCH(stDummy.szLoginName));
		if(!sht_remove(pstEnv->pstShtL2P, &stDummy, L2PHashCmp, L2PHashCode))
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
			           "can not remove LoginName2MemID uin:%d, memid:%d",
			           pstPlayer->stRoleData.Uin,
			           pstPlayer->iMemID);
		}
	}
	
	tmempool_free(pstEnv->pstPlayerPool, pstPlayer->iIdx);
	
	stUinCache.iUin = iUin;
	pstUinCache = (UinCache *)sht_find(pstEnv->pstShtUinCache, &stUinCache, UinHashCmp, UinHashCode);
	if (pstUinCache)
	{
		pstUinCache->iOnline = 0;
		return 0;
	}
	else
	{
		tlog_error(pstEnv->pstLogCat, iUin, 0, "z_free_player %d cache not exist", iUin);
		assert(0);
		return -1;
	}
}

int z_free_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon)
{
	ZoneObj *pstObj;
	int iID;
	UNUSED(pstAppCtx);

	pstObj = pstEnv->pstZoneObj;
	iID = pstMon->iID;

	if (pstMon->iOwnerID > 0)
	{
		Player *pstPlayer;

		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->iOwnerID);

		if (pstPlayer)
		{
			z_del_totem(pstEnv, pstPlayer, NULL, pstMon);
		}
	}

	/*
	if(pstMon->bSubType == MON_SUB_TYPE_JIN || pstMon->bSubType == MON_SUB_TYPE_BOSS)
	{
		tlog_info(pstEnv->pstLogCat, 0, 0,	"monster free: DefID %d, MemID %d, Map %d",
			pstMon->iDefIdx, pstMon->iID, pstMon->stMap.iID);
	}
	*/
	
	if (pstMon->cType == OBJ_NPC)
	{
		tmempool_free(pstEnv->pstNpcPool, pstMon->iMIndex);
	}
	else if (pstMon->cType == OBJ_MONSTER)
	{
		tmempool_free(pstEnv->pstMonsterPool, pstMon->iMIndex);
	}
	else
		return -1;

	z_id_clean(pstEnv, iID);
	
	return 0;
}

int z_check_uincache(ZONESVRENV* pstEnv)
{
	int i;
	int iValid;
	Player *pstPlayer;
	LPSHTABLE pstShtUinCache;
	UinCache *pstUinCache;
	
    //should check uin cache and player mempool 约束
    
    for (i=0; i<MAX_PLAYER; i++)
    {
		pstPlayer = tmempool_get_bypos(pstEnv->pstPlayerPool, i);
		if (pstPlayer)
		{
			if (NULL == player_get_by_uin(pstEnv, pstPlayer->stRoleData.Uin))
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "uin=%d in player pool, but not in uin cache", pstPlayer->stRoleData.Uin);
			}
		}
    }

	pstShtUinCache = pstEnv->pstShtUinCache;
	for (i=0; i<pstShtUinCache->iMax; i++)
	{
		pstUinCache = (UinCache *)sht_pos(pstShtUinCache, i, &iValid);
		if (NULL == pstUinCache || !iValid || 0 == pstUinCache->iOnline)
		{
			continue;
		}

		if (NULL == tmempool_get(pstEnv->pstPlayerPool, pstUinCache->iIdx))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "uin=%d in uin cache, but not in player pool ", pstUinCache->iUin);
		}
	}
    
	return 0;
}

//最简单的淘汰，随机
int z_washout_uincache(ZONESVRENV *pstEnv)
{
	int i;
	int iWashoutCount = 0;
	int iMax = pstEnv->pstShtUinCache->iMax;
	UinCache *pstUinCache;
	int iValid;

	for (i=0; i<iMax; i++)
	{
		if (pstEnv->iUinCacheWashoutPos >= iMax)
		{
			pstEnv->iUinCacheWashoutPos = 0;
		}
		pstUinCache = sht_pos(pstEnv->pstShtUinCache, pstEnv->iUinCacheWashoutPos, &iValid);
		pstEnv->iUinCacheWashoutPos++;

		if (pstUinCache && iValid && 0 == pstUinCache->iOnline)
		{
			iWashoutCount++;
			sht_remove_by_pos(pstEnv->pstShtUinCache, pstEnv->iUinCacheWashoutPos-1);

			if (iWashoutCount >= MAX_PLAYER)
			{
				break;
			}
		}
	}

	if (MAX_PLAYER > iWashoutCount)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_washout_uincache washout %d", iWashoutCount);
	}

	return iWashoutCount;
}

Player * z_name_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, const char *pszName)
{
	NameMemID stNameMemID;
	NameMemID *pstNameMemID;

	STRNCPY(stNameMemID.szName, pszName, sizeof(stNameMemID.szName));
	pstNameMemID = sht_find(pstEnv->pstShtNameMemID, &stNameMemID, NameMemIDHashCmp, NameMemIDHashCode);
	if (pstNameMemID)
	{
		return z_id_player(pstAppCtx, pstEnv, pstNameMemID->iID);
	}
	else
	{
		return NULL;
	}
}

Player* z_loginname_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,
                           const char *pszAccount)
{
	LoginName2MemID stKey;
	LoginName2MemID *pstTarget = NULL;

	STRNCPY(stKey.szLoginName, pszAccount, CCH(stKey.szLoginName));
	pstTarget = sht_find(pstEnv->pstShtL2P, &stKey, L2PHashCmp, L2PHashCode);
	return !pstTarget ? NULL : z_id_player(pstAppCtx, pstEnv, pstTarget->iID);
}

GAMEEVENTDEF *z_find_event_def(ZONESVRENV *pstEnv, int iEventID, int *pIidx)
{
	return (GAMEEVENTDEF *)z_find_def(pstEnv->pstZoneObj->sGameEventDef, iEventID, GAME_EVENT_DEF_DATA, 
									pstEnv->pstZoneObj->iGameEventDef, pIidx);
}

ACTIVEHELPER *z_find_active_help_def(ZONESVRENV *pstEnv, int iActiveID, int *pIidx)
{
	return (ACTIVEHELPER *)z_find_def(pstEnv->pstZoneObj->sActiveHelp, iActiveID, ACTIVE_HELP_DATA, 
									pstEnv->pstZoneObj->iActiveHelp, pIidx);
}

void *z_find_def(void* pBase, unsigned int uiKey, int iUnit, int iUnitNum, int *piIdx)
{
	void *pDef;
	int iIndex;
	int iEqu;

	if (piIdx && *piIdx >= 0 && *piIdx < iUnitNum)
	{
		pDef = pBase + (*piIdx)*iUnit;
		if (*((unsigned int *)pDef) == uiKey)
		{
			return pDef;
		}
	}

	iIndex = bsearch_int(&uiKey, pBase, iUnitNum, iUnit, &iEqu);
	if (iEqu)
	{
		if (piIdx)
		{
			*piIdx = iIndex;
		}
		
		return  (pBase + iIndex*iUnit);
	}
	
	return NULL;
}


int z_attach_player(ZONESVRENV* pstEnv)
{
	int iShmID;
	int iPoolSize;
	int iShmSize;
	PlayerShm *pvAddr;
	int iIsExist=0;
	int iRet;
	int iOffSet;
	int iPlayerShmKey = pstEnv->pstConf->PlayerShmKey;

	iOffSet = offsetof(PlayerShm, sData);
	iPoolSize = TMEMPOOL_CALC(MAX_PLAYER, PLAYER_DATA);
	iShmSize	=	iPoolSize + iOffSet;
	iShmID	=	shmget(iPlayerShmKey, iShmSize, 0666 | IPC_CREAT | IPC_EXCL);

	if( iShmID<0 )
	{
		iIsExist	=	1;
		iShmID		=	shmget(iPlayerShmKey, iShmSize, 0666);
	}

	if( iShmID<0 )
		return -1;

	if (pstEnv->pstPlayerShm)
	{
		shmdt(pstEnv->pstPlayerShm);
	}

	pvAddr	=	(PlayerShm *)shmat(iShmID, NULL, 0);
	
	if( (void *) -1  == pvAddr )
		return -1;

	pstEnv->pstPlayerShm = pvAddr;
	pstEnv->iPlayerShmID = iShmID;

	if( iIsExist )
	{
		if (PLAYER_SHM_VER != pvAddr->iShmVersion)
		{
			printf("player shm ver=%d, but now shm ver=%d\n", pvAddr->iShmVersion, PLAYER_SHM_VER);
			shmdt(pvAddr);
			return -1;
		}
		
		iRet	=	tmempool_attach(&pstEnv->pstPlayerPool, MAX_PLAYER, PLAYER_DATA, pvAddr->sData, iPoolSize);
	}
	else
	{
		pvAddr->iShmVersion = PLAYER_SHM_VER;
		iRet	=	tmempool_init(&pstEnv->pstPlayerPool, MAX_PLAYER, PLAYER_DATA, pvAddr->sData, iPoolSize);
	}

	if( iRet<0 )
	{
		shmdt(pvAddr);
	}

	return iRet;
	
}

int z_reg_clt_cmd(ZONESVRENV* pstEnv, unsigned short unCmd, CmdHandle pfnCmdHandle, char cStateLimit)
{
	CmdMap *pstCmdMap;
	
	assert(unCmd < MAX_CS_CMD);
	
	pstCmdMap = &pstEnv->astCmdMap[unCmd];
	pstCmdMap->cCtrlFlag = 1;
	pstCmdMap->cStateLimit = cStateLimit;
	pstCmdMap->pfnCmdHandle = pfnCmdHandle;

	return 0;
}

int z_unreg_clt_cmd(ZONESVRENV* pstEnv, unsigned short unCmd)
{
	CmdMap *pstCmdMap;
	
	if (unCmd >= MAX_CS_CMD )
	{
		return -1;
	}
	
	pstCmdMap = &pstEnv->astCmdMap[unCmd];
	pstCmdMap->cCtrlFlag = 0;
	pstCmdMap->pfnCmdHandle = NULL;

	return 0;
}


int z_init_cmd_map(ZONESVRENV* pstEnv)
{
	int i;
	
	memset(pstEnv->astCmdMap, 0, sizeof(pstEnv->astCmdMap));

	z_reg_clt_cmd(pstEnv, ACC_LOGIN_REQ, player_acc_login, -1);
	z_reg_clt_cmd(pstEnv, ROLE_NEW_REQ, player_role_new, -1);
	z_reg_clt_cmd(pstEnv, ROLE_LOGIN_REQ, player_role_login, -1);
	z_reg_clt_cmd(pstEnv, ROLE_LOGOUT_REQ, player_role_logout, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, ACC_LOGOUT_REQ, player_acc_logout, -1);
	z_reg_clt_cmd(pstEnv, HEARTBEAT_REQ, player_heartbeat, -1);
	z_reg_clt_cmd(pstEnv, ROLE_DEL_REQ, player_role_del, PLAYER_STATUS_ROLE_LIST);
	z_reg_clt_cmd(pstEnv, SELECT_BIRTH_LOCATION_CLT, player_role_selbirth, PLAYER_STATUS_ROLE_LOGIN_REQ);
	z_reg_clt_cmd(pstEnv, MOVE_CLT, player_move, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, ACTION_REQ, player_action, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, CHAT_REQ, player_chat, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, CLT_OPT, player_opt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, SKILL_PREPARE, player_prepare, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, SKILL_BRK, player_skill_brk, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, SKILL_ATK, player_atk, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, SKILL_HIT, player_ballistic_hit, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, REVIVE, player_revive, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, DLG_CLT, player_dialog, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, ADJUST_CLT, player_adjust, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, PACKAGE_REQ, player_package_req, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, TRANS_SELECT, player_trans_select, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, RELATION_CLT, player_relation, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, TEAM_CLT, player_team, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, SHOP_CLT, player_shop, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, TASK_CLT, player_task, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, AUTO_FLY_CLT, player_auto_fly_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, COLLECT_CLT, player_collect, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, LEARN_CLT, player_skill_learn, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, RIDE_PRE, player_ride_prepare, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, RIDE_ON, player_ride_on, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, RIDE_BRK, player_ride_brk, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, PET_CLT, pet_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, EXCHG_CLT, player_exchg, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, DYN_PWORLD_CLT, player_enter_dyn_pworld, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, TRUST_CLT, player_trust, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, CLAN_CLT, player_clan, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, MAIL_CLT, player_mail, PLAYER_STATUS_ROLE_LOGIN);
	//z_reg_clt_cmd(pstEnv, FITTINGSUPDATE_CLT, player_fittings_update, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, STALL_CLT, player_stall, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, ARM_CLT, player_arm, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, CANCEL_ROLE_LOGOUT, player_cancel_role_logout, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, CHGNAME_CLT, player_chg_name, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, CANCEL_STATUS, player_cancel_status, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, PLAYER_JUMP, player_jump, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, RIDE_CLT, player_ride_clt, PLAYER_STATUS_ROLE_LOGIN);
	//z_reg_clt_cmd(pstEnv, WATCH_PLAYER_CLT, player_watch, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, ONLINE_AWARD_CLT, player_online_award, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, WEI_XING_CLT, player_weixing, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, DESIGNATION_CLT, player_designation, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, WATCH_FRIEND_CLT, player_watch_friend,  PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, FAIRY_CLT, player_fairy_op, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, SHOPPING_CLT, player_shopping_req, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, PK_CLT, player_pk_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, PWORLD_CLT, pworld_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, CHANGE_COIN_INTO, player_coin_to_bull, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, GIFT_CLT, player_gift, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, RANGE_CLT, player_range, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, BCT_CLT, bct_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, SECONDARY_SKILL_CLT, player_secondary_skill_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, EXTEND_ITEM_CLT, player_extend_item, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, PREPAREATK_CLT, z_prepareatk_mon_skill, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, INVITE_TEST_CLT, player_qiecuo, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, PLAYER_MISC_CLT, player_misc_clt, -1);//这里放开,里面判断
	z_reg_clt_cmd(pstEnv, GPRS_CLT, player_gprs, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, SAFE_LOCK_CLT, player_safelock, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, RIDE_MACHINE_CLT, player_machine_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, MAP_CLT, map_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, KEYIN_ITEM_CLT, player_keyin_item, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, RUNE_CLT, player_rune, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, ROLL_CLT, player_roll_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, LEAVE_CLT, leave_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, MOTION_CLT, player_motion_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, VAL_CLT, val_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, DIFF_PWORLD_CLT, diff_pworld_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, ROLE_UNDEL_REQ, player_undel, PLAYER_STATUS_ROLE_LIST);
	z_reg_clt_cmd(pstEnv, RAFFLE_CLT, player_raffle, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, CITY_CLT, world_city_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, ACTIVE_GAME_CLT, active_game_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, UNREAL_CLT, unreal_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, AUTH_CLT, auth_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, SJ_TASK_CLT, player_shangjin_task, PLAYER_STATUS_ROLE_LOGIN);	
	z_reg_clt_cmd(pstEnv, AAS_CLT, aas_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, SUBJECT_CLT, subject_clt_op, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, GAY_CLT, player_gay_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, STRONG_CLT, strong_clt_op, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, AUTOPWORLD_CLT, autopworld_clt_op, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, ACTIVE_CLT, active_clt_op, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, RONGLU_CLT, ronglu_clt_op, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, GUNIUS_CLT, genius_clt_op, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, APEX_CLT, apex_clt, -1);
	z_reg_clt_cmd(pstEnv, BUSY_CLT, player_busy_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, STARDATA_CLT, player_star_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, BOON_CLT, clan_boon_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, SPARTA_CLT, player_sparta_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, EXTEND_ITEM_SAVE, player_extend_item_save, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, SECOND_PWD_CLT, player_secondpwd, -1);
	z_reg_clt_cmd(pstEnv, BACK_CLT, player_back_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, SCHOOL_CLT, player_school_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, CARD_BOX_CLT, player_cardbox_clt, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, STORE_CLT, store_op, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, CLAN_PET_CLT, clan_pet_op, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, SPAN_STRONG_CLT, span_strong_clt_op, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, HOME_CLT, home_op, PLAYER_STATUS_ROLE_LOGIN);
	z_reg_clt_cmd(pstEnv, SHIKONG_CLT, shikong_op, PLAYER_STATUS_ROLE_LOGIN);
	
	
	for (i=0; i<pstEnv->pstConf->CmdLimitNum; i++)
	{
		z_unreg_clt_cmd(pstEnv, pstEnv->pstConf->CmdLimitIds[i]);
	}
	
	return 0;
}

int z_show_mem_use(ZONESVRENV* pstEnv)
{
	printf("player memsize=%ld, uin cache memsize=%ld, ZoneObj memsize=%ld, MapObj memsize=%ld, mon init %d, dyn area %d\n",
			(long int)(pstEnv->pstPlayerPool->iSize+offsetof(PlayerShm, sData)), (long int)SHT_SIZE(UIN_CACHE*3, UIN_CACHE, sizeof(UinCache)),
			(long int)sizeof(ZoneObj), (long int)sizeof(MapObj), pstEnv->pstMonsterPool->iUsed, pstEnv->pstAreaDynPool->iUsed);

	printf("ZoneObj:table def:%ld,Item:%ld,Mon:%ld,Npc:%ld,Mail:%ld,Clan:%ld\n",
		(long int)(offsetof(ZoneObj,iShopGetStat)-offsetof(ZoneObj, iInitRes)), 
		(long int)sizeof(pstEnv->pstZoneObj->sItem), 
		(long int)sizeof(pstEnv->pstZoneObj->sMonster),
		(long int)sizeof(pstEnv->pstZoneObj->sNpc),
		(long int)sizeof(pstEnv->pstZoneObj->sMail),
		(long int)sizeof(pstEnv->pstZoneObj->sClan));

	printf("MapObj:table def:%ld,Msk:%ld,AreaDyn:%ld,DynMsk:%ld,MapInst:%ld,PworldInst=%ld\n",
		(long int)(offsetof(MapObj,iArea)-offsetof(MapObj, iLastID)),
		(long int)sizeof(pstEnv->pstMapObj->sMask),
		(long int)sizeof(pstEnv->pstMapObj->sAreaDyn),
		(long int)sizeof(pstEnv->pstMapObj->sDynMsk),
		(long int)sizeof(pstEnv->pstMapObj->sMapInst),
		(long int)sizeof(pstEnv->pstMapObj->sPworldInst));
	
	tlog_info(pstEnv->pstLogCat, 0, 0, "player memsize=%ld, uin cache memsize=%ld, ZoneObj memsize=%ld, MapObj memsize=%ld, mon init=%d, dyn area %d",
			(long int)(pstEnv->pstPlayerPool->iSize+offsetof(PlayerShm, sData)), (long int)SHT_SIZE(UIN_CACHE*3, UIN_CACHE, sizeof(UinCache)),
			(long int)sizeof(ZoneObj), (long int)sizeof(MapObj), pstEnv->pstMonsterPool->iUsed, pstEnv->pstAreaDynPool->iUsed);

	tlog_info(pstEnv->pstLogCat, 0, 0, "ZoneObj:table def:%ld,Item:%ld,Mon:%ld,Npc:%ld,Mail:%ld,Clan:%ld",
		(long int)(offsetof(ZoneObj,iShopGetStat)-offsetof(ZoneObj, iInitRes)), 
		(long int)sizeof(pstEnv->pstZoneObj->sItem), 
		(long int)sizeof(pstEnv->pstZoneObj->sMonster),
		(long int)sizeof(pstEnv->pstZoneObj->sNpc),
		(long int)sizeof(pstEnv->pstZoneObj->sMail),
		(long int)sizeof(pstEnv->pstZoneObj->sClan));

	tlog_info(pstEnv->pstLogCat, 0, 0, "MapObj:table def:%ld,Msk:%ld,AreaDyn:%ld,DynMsk:%ld,MapInst:%ld,PworldInst=%ld",
		(long int)(offsetof(MapObj,iArea)-offsetof(MapObj, iLastID)),
		(long int)sizeof(pstEnv->pstMapObj->sMask),
		(long int)sizeof(pstEnv->pstMapObj->sAreaDyn),
		(long int)sizeof(pstEnv->pstMapObj->sDynMsk),
		(long int)sizeof(pstEnv->pstMapObj->sMapInst),
		(long int)sizeof(pstEnv->pstMapObj->sPworldInst));

	return 0;
}


SUITSET *z_find_suitset(ZONESVRENV* pstEnv, int iSuitSetID)
{
	SUITSET  stSuitSet;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stSuitSet.SuitSetID = iSuitSetID;
	iIndex = bsearch_int(&stSuitSet, pstObj->sSuitSetDef, pstObj->iSuitSetDef, SUITSET_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (SUITSET *)pstObj->sSuitSetDef[iIndex];
	}
	
	return NULL;	
}

LEVELUP *z_get_levelup(ZONESVRENV* pstEnv, int iLevel)
{
	//int iLevel;

	//iLevel = pstPlayer->stRoleData.Level;
	//if (iLevel < 1 || iLevel > pstEnv->pstConf->MaxLevel  || iLevel > MAX_LEVEL ||iLevel > pstEnv->pstZoneObj->iLevelUpDef)
	if (iLevel < 1 || iLevel > MAX_LEVEL ||iLevel > pstEnv->pstZoneObj->iLevelUpDef)
	{
		return NULL;
	}

	return  (LEVELUP *)pstEnv->pstZoneObj->sLevelUpDef[iLevel-1];
	
}

ADDATTRDEF *z_find_addattr(ZONESVRENV* pstEnv, int iAddAttrID)
{
	ADDATTRDEF  stAddAttr;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stAddAttr.AddAttrID = iAddAttrID;
	iIndex = bsearch_int(&stAddAttr, pstObj->sAddAttrDef, pstObj->iAddAttrDef, ADDATTR_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ADDATTRDEF *)pstObj->sAddAttrDef[iIndex];
	}
	
	return NULL;	
}

//根据monster 编号规则获得怪物类型
int z_monster_num_type(unsigned int uiMonsterID)
{
	int iType;
	char szMonID[16];

	if (uiMonsterID > PWORLD_LEVEL_MON_BASE)
	{
		return MON_TYPE_MONSTER;
	}

	szMonID[0] = 0;
	sprintf(szMonID, "%u", uiMonsterID);
	iType = szMonID[0] - '0';

	return iType;
}

int z_pre_monster(ZONESVRENV* pstEnv)
{
	int i;
	MONSTERDEF *pstMonsterDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	for (i=0; i<pstObj->iMonsterDef; i++)
	{
		pstMonsterDef = (MONSTERDEF *)pstObj->sMonsterDef[i];

		pstMonsterDef->Type = z_monster_num_type(pstMonsterDef->MonsterID);
		if (pstMonsterDef->Type < MON_TYPE_PET || pstMonsterDef->Type > MON_TYPE_NPC)
		{
			printf("monster %d def Type = %d  error\n", pstMonsterDef->MonsterID, pstMonsterDef->Type);
			return -1;
		}
	}
	
	return 0;
}

int z_pre_online_award(ZONESVRENV* pstEnv)
{
	int i;
	int iPreTime = -1;
	ONLINEAWARDDEF *pstOnlineAwardDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	for (i=0; i<pstObj->iOnlineAwardDef; i++)
	{
		pstOnlineAwardDef = (ONLINEAWARDDEF *)pstObj->sOnlineAwardDef[i];
		if ((int)(pstOnlineAwardDef->OnlineTime) <= iPreTime)
		{
			printf("OnlineAwardDef is error\n");
			return -1;
		}
		iPreTime = pstOnlineAwardDef->OnlineTime;
	}
	return 0;
}

/*
int z_check_hatchegg_rate(PETDEF *pstPetDef)
{
	int i;
	int iCount=0;

	if (pstPetDef->GenEggs[0].Rate == 0)
	{
		return 0;
	}
	
	for (i=0; i<MAX_PET_GENEGG_ID; i++)
	{
		iCount += pstPetDef->GenEggs[i].Rate;
	}

	if (iCount != 100)
	{
		return -1;
	}
	return 0;
}
*/


int z_check_overlaytype(ZONESVRENV* pstEnv)
{
	int i;
	OVERLAYTYPERELATIONDEF *pstDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	for (i=0; i<pstObj->iOverlayTypeRelationDef; i++)
	{
		pstDef = (OVERLAYTYPERELATIONDEF *)pstObj->sOverlayTypeRelationDef[i];

		if (pstDef->OverlayFlag < OVERLAY_FLAG_COEXIST || pstDef->OverlayFlag > OVERLAY_FLAG_SAME_ADDTIME)
		{
			printf("overlaytype %d %d %d err\n", pstDef->OverlayType1, pstDef->OverlayType2, pstDef->OverlayFlag);
			return -1;
		}
	}
	
	return 0;
}

int z_pre_pet(ZONESVRENV* pstEnv)
{
	int i,j;
	int iRate;
	PETDEF *pstPetDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	//洗资质
	for (i=0; i<pstObj->iPetDef; i++)
	{
		pstPetDef = (PETDEF *)pstObj->sPetDef[i];

		iRate = 0;
		for(j=0; j<MAX_PET_GENIUS;j++)
		{
			if(pstPetDef->NormalWashGenius[j] == 0 )
			{
				continue;
			}

			iRate+= pstPetDef->NormalWashGenius[j] ;
		}
		if (iRate != 10000)
		{
			printf("Pet def %d NormalWashGenius rate != 10000\n", pstPetDef->PetID);
			return -1;
		}
	}

	//性格
	/*pstNormalTrait->iTraitNum = 0;
	for (i=0; i<pstObj->iPetTraitDef; i++)
	{
		pstPetTraitDef = (PETTRAITDEF *)pstObj->sPetTraitDef[i];

		if (pstNormalTrait->iTraitNum >= MAX_PET_TRAIT_DEF)
		{
			break;
		}

		if(pstPetTraitDef->TraitType == PET_NORMAL_TRAIT )
		{
			pstNormalTrait->TraitID[pstNormalTrait->iTraitNum++] = pstPetTraitDef->TraitID;
		}
	}*/
	
	return 0;
}

SkillDefTail* z_get_skill_tail(SKILLDEF *pstSkillDef)
{
	long int iPtr;
	
	iPtr = (long int) pstSkillDef;
	iPtr += sizeof(*pstSkillDef);

	return (SkillDefTail*) iPtr;
}


int z_pre_skill(ZONESVRENV* pstEnv)
{
	int i;
	SKILLDEF *pstSkillDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	SkillDefTail* pstTail = NULL;

	for (i=0; i<pstObj->iSkillDef; i++)
	{
		pstSkillDef = (SKILLDEF *)pstObj->sSkillDef[i];
		pstTail = z_get_skill_tail(pstSkillDef);
		memset(pstTail,0,sizeof(*pstTail));
		
		

	}

#if 0
	int i;
	SKILLDEF *pstSkillDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	for (i=0; i<pstObj->iSkillDef; i++)
	{
		pstSkillDef = (SKILLDEF *)pstObj->sSkillDef[i];

		if (STATUS_BIND_SKILL_MIN <= pstSkillDef->SkillID && STATUS_BIND_SKILL_MAX >= pstSkillDef->SkillID)
		{
			pstSkillDef->BindSkill = BINDSKILL_TYPE_STATUS;
		}
		else if (TMP_SKILL_MIN <= pstSkillDef->SkillID && TMP_SKILL_MAX >= pstSkillDef->SkillID)
		{
			pstSkillDef->BindSkill = BINDSKILL_TYPE_TMP;
		}
		else
		{
			pstSkillDef->BindSkill = BINDSKILL_TYPE_NONE;
		}

		/*
		if (pstSkillDef->FittingsGridRowNum < 1)
		{
			pstSkillDef->FittingsGridRowNum = 1;
			pstSkillDef->FittingsGridNum = 0;

			//return -1;
		}
		*/

	}

#endif
	
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

AchievementDefTail* z_get_achievement_tail(ACHIEVEMENTDEF* pstDef)
{
	long int iPtr;
	
	iPtr = (long int) pstDef;
	iPtr += sizeof(*pstDef);

	return (AchievementDefTail*) iPtr;
}

PworldWaveMonDefTail* z_get_pworld_wave_mon_tail(PWORLDWAVEMONDEF* pstDef)
{
	long int iPtr;
	
	iPtr = (long int) pstDef;
	iPtr += sizeof(*pstDef);

	return (PworldWaveMonDefTail*) iPtr;
}

PlayerAwardDefTail* z_get_player_award_tail(PLAYERAWARDDEF* pstDef)
{
	long int iPtr;
	
	iPtr = (long int) pstDef;
	iPtr += sizeof(*pstDef);

	return (PlayerAwardDefTail*) iPtr;
}

ClanAwardDefTail* z_get_clan_award_tail(CLANAWARDDEF* pstDef)
{
	long int iPtr;
	
	iPtr = (long int) pstDef;
	iPtr += sizeof(*pstDef);

	return (ClanAwardDefTail*) iPtr;
}


int z_pre_clan_spy_award_drop(ZONESVRENV* pstEnv)
{
	int i;
	CLANSPYAWARDDROPDEF* pstDef = NULL;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	int iFlag = 0;

	for (i=0; i< pstObj->iClanSpyAwardDropDef; i++)
	{		
		pstDef = (CLANSPYAWARDDROPDEF*)pstObj->sClanSpyAwardDropDef[i];

		if (pstDef->DefID == ANTI_KILL_ANTI ||
			pstDef->DefID == ANTI_KILL_SPY ||
			pstDef->DefID == SPY_KILL_SPY ||
			pstDef->DefID == SPY_KILL_ANTI )
		{
			iFlag |= 1 << i;
		}
			

	}

	if (i > 0 && 
		(
		(iFlag & ANTI_KILL_ANTI) == 0 ||
		(iFlag & ANTI_KILL_SPY) == 0 ||
		(iFlag & SPY_KILL_SPY) == 0 ||
		(iFlag & SPY_KILL_ANTI) == 0
		))
	{
		printf("ClanSpyAwardDropDef check failed, iFlag = %d\n", iFlag);
		return -1;
	}
	return 0;
}

int z_pre_clan_spy_trans(ZONESVRENV* pstEnv)
{
	int i;
	CLANSPYTRANSDEF* pstDef = NULL;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	int iFound = 0;

	for (i=0; i<pstObj->iClanSpyTransDef; i++)
	{		
		pstDef = (CLANSPYTRANSDEF*)pstObj->sClanSpyTransDef[i];

		if (SPY_TASK_NPC_TRANS_DEFID == pstDef->DefID)
		{
			iFound = 1;
			break;
		}

	}

	if ((0 == iFound) && i > 0)
	{
		printf("ClanSpyTransDef check failed, need SPY_TASK_NPC_TRANS_DEFID DefID = %d\n", SPY_TASK_NPC_TRANS_DEFID);
		return -1;
	}
	return 0;
}

int z_pre_pworld_wave_mon_def(ZONESVRENV* pstEnv)
{
	int i;
	PWORLDWAVEMONDEF* pstDef = NULL;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	PworldWaveMonDefTail* pstTail = NULL;

	for (i=0; i<pstObj->iPworldWaveMonDef; i++)
	{
		
		pstDef = (PWORLDWAVEMONDEF*)pstObj->sPworldWaveMonDef[i];
		pstTail = z_get_pworld_wave_mon_tail(pstDef);

		memset(pstTail,0,sizeof(*pstTail));

		pstTail->iClanAwardDefStart = -1;
		pstTail->iMonPosDefStart = -1;

	}
	return 0;
}

int z_pre_pworld_wave_mon_pos_def(ZONESVRENV* pstEnv)
{
	int i;
	PWORLDWAVEMONPOSDEF* pstDef = NULL;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	PWORLDWAVEMONDEF* pstWaveDef = NULL;
	PworldWaveMonDefTail* pstTail = NULL;

	for (i=0; i<pstObj->iPworldWaveMonPosDef; i++)
	{
		
		pstDef = (PWORLDWAVEMONPOSDEF*)pstObj->sPworldWaveMonPosDef[i];

		pstWaveDef = z_find_pworld_wave_mon_def(pstEnv,pstDef->WaveMonDefID);
		if (!pstWaveDef)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "WaveMonDefID err:%d",pstDef->WaveMonDefID);
			continue;
		}
		
		pstTail = z_get_pworld_wave_mon_tail(pstWaveDef);
		
		if (-1 == pstTail->iMonPosDefStart)
		{
			pstTail->iMonPosDefStart = i;
		}

		pstTail->iMonPosDefCount++;
		pstTail->iTotalMonNum += pstDef->Num;

	}
	return 0;
}

int z_pre_pworld_wave_mon_clan_award_def(ZONESVRENV* pstEnv)
{
	int i;
	PWORLDWAVEMONCLANAWARDDEF* pstDef = NULL;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	PWORLDWAVEMONDEF* pstWaveDef = NULL;
	PworldWaveMonDefTail* pstTail = NULL;

	for (i=0; i<pstObj->iPworldWaveMonClanAwardDef; i++)
	{
		
		pstDef = (PWORLDWAVEMONCLANAWARDDEF*)pstObj->sPworldWaveMonClanAwardDef[i];

		pstWaveDef = z_find_pworld_wave_mon_def(pstEnv,pstDef->WaveMonDefID);
		if (!pstWaveDef)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "WaveMonDefID err:%d",pstDef->WaveMonDefID);
			continue;
		}
		
		pstTail = z_get_pworld_wave_mon_tail(pstWaveDef);
		
		if (-1 == pstTail->iClanAwardDefStart)
		{
			pstTail->iClanAwardDefStart = i;
		}

		pstTail->iClanAwardDefCount++;

	}
	return 0;
}

int z_pre_pworld_machine_buff_shop(ZONESVRENV* pstEnv)
{
	int i;
	PWORLDMACHINEBUFFSHOPDEF* pstDef = NULL;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	for (i=0; i<pstObj->iPworldMachineBuffShopDef; i++)
	{
		
		pstDef = (PWORLDMACHINEBUFFSHOPDEF*)pstObj->sPworldMachineBuffShopDef[i];

		//todo:	

	}
	return 0;
}

int z_pre_player_award(ZONESVRENV* pstEnv)
{
	int i;
	PLAYERAWARDDEF* pstDef = NULL;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	int iDefID = -1;
	int k = 0;
	PlayerAwardDefTail* pstTail = NULL;
	int iStart = 0;
	int iCount = 0;

	for (i=0; i<pstObj->iPlayerAwardDef; i++)
	{
		
		pstDef = (PLAYERAWARDDEF*)pstObj->sPlayerAwardDef[i];
		pstTail = z_get_player_award_tail(pstDef);

		if (!pstTail)
		{
			continue;
		}
		
		if (iDefID != pstDef->PlayerAwardDefID)
		{
			iCount = 0;
			iDefID = pstDef->PlayerAwardDefID;
			for (k = i; k < pstObj->iPlayerAwardDef;k++)
			{
				PLAYERAWARDDEF* pstDefTmp = (PLAYERAWARDDEF*)pstObj->sPlayerAwardDef[k];
				if (iDefID != pstDefTmp->PlayerAwardDefID)
				{
					break;
				}
				iCount++;
			}
			iDefID = pstDef->PlayerAwardDefID;

			iStart = i;
		}

		pstTail->iPosCount = iCount;
		pstTail->iPosStart = iStart;

	}
	return 0;
}

int z_pre_cow_final_award(ZONESVRENV* pstEnv)
{
	int i;
	COWFINALAWARDDEF* pstDef = NULL;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	for (i=0; i<pstObj->iCowFinalAwardDef; i++)
	{
		
		pstDef = (COWFINALAWARDDEF*)pstObj->sCowFinalAwardDef[i];

		//todo:	

	}
	return 0;
}

int z_pre_clan_skill(ZONESVRENV* pstEnv)
{
	int i;
	CLANSKILLDEF* pstDef = NULL;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	for (i=0; i<pstObj->iClanSkillDef; i++)
	{
		
		pstDef = (CLANSKILLDEF*)pstObj->sClanSkillDef[i];

		//todo:	

	}
	return 0;
}

int z_pre_clan_pay(ZONESVRENV* pstEnv)
{
	int i;
	CLANPAYDEF* pstDef = NULL;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	for (i=0; i<pstObj->iClanPayDef; i++)
	{
		
		pstDef = (CLANPAYDEF*)pstObj->sClanPayDef[i];

		//todo:	

	}
	return 0;
}

int z_pre_pet_love_level(ZONESVRENV* pstEnv)
{
	int i;
	PETLOVELEVELDEF* pstDef = NULL;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	for (i=0; i<pstObj->iPetLoveLevelDef; i++)
	{
		
		pstDef = (PETLOVELEVELDEF*)pstObj->sPetLoveLevelDef[i];

		//todo:	

	}
	return 0;
}

int z_pre_lucky_map(ZONESVRENV* pstEnv)
{
	int i;
	LUCKYMAPDEF* pstDef = NULL;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	pstEnv->iLuckyRandSum = 0;

	for (i=0; i<pstObj->iLuckyMapDef; i++)
	{
		
		pstDef = (LUCKYMAPDEF*)pstObj->sLuckyMapDef[i];

		pstEnv->iLuckyRandSum += pstDef->RandProb;

	}
	
	return 0;
}

int z_pre_clan_award(ZONESVRENV* pstEnv)
{
	int i;
	CLANAWARDDEF* pstDef = NULL;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	int iDefID = -1;
	int k = 0;
	ClanAwardDefTail* pstTail = NULL;
	int iStart = 0;
	int iCount = 0;

	for (i=0; i<pstObj->iClanAwardDef; i++)
	{
		
		pstDef = (CLANAWARDDEF*)pstObj->sClanAwardDef[i];
		pstTail = z_get_clan_award_tail(pstDef);

		if (!pstTail)
		{
			continue;
		}
		
		if (iDefID != pstDef->ClanAwardDefID)
		{
			iCount = 0;
			iDefID = pstDef->ClanAwardDefID;
			for (k = i; k < pstObj->iClanAwardDef;k++)
			{
				CLANAWARDDEF* pstDefTmp = (CLANAWARDDEF*)pstObj->sClanAwardDef[k];
				if (iDefID != pstDefTmp->ClanAwardDefID)
				{
					break;
				}
				iCount++;
			}
			iDefID = pstDef->ClanAwardDefID;

			iStart = i;
		}

		pstTail->iPosCount = iCount;
		pstTail->iPosStart = iStart;

	}
	return 0;
	
}

int z_pre_clan_war_award(ZONESVRENV* pstEnv)
{
	int i;
	CLANWARAWARDDEF* pstDef = NULL;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	for (i=0; i<pstObj->iClanWarAwardDef; i++)
	{
		
		pstDef = (CLANWARAWARDDEF*)pstObj->sClanWarAwardDef[i];

		//todo:	

	}
	return 0;
}

int z_pre_achievement(ZONESVRENV* pstEnv)
{
	int i;
	ACHIEVEMENTDEF* pstDef = NULL;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	SKILLDEF *pstSkillDef = NULL;
	int iIdx = 0;
	AchievementDefTail* pstTail = NULL;
	ACHIEVEMENTCATEGORYDEF* pstCategory = NULL;
	CONDIDMAP *pstIDMap = NULL;
	ACHIEVEMENTDEF* pstMapDef = NULL;
	AchievementDefTail* pstMapTail = NULL;


	for(i = 0; i < pstEnv->pstCondIdMapHash->iMax;i++)
	{
		CondIdMapBucket *pstPos = NULL;
		int iValid = 0;


		pstPos = (CondIdMapBucket *)sht_pos(pstEnv->pstCondIdMapHash, i, &iValid);

		if( iValid && pstPos)
		{	
			sht_remove_by_pos(pstEnv->pstCondIdMapHash, i);
		}
	}

	for (i=0; i<pstObj->iAchievementDef; i++)
	{
		int k = 0;
		
		pstDef = (ACHIEVEMENTDEF*)pstObj->sAchievementDef[i];

		if (pstDef->AchievementID >= MAX_ACHIEVEMENT)
		{
			printf("AchievementDef ID %d >= MAX_ACHIEVEMENT\n", pstDef->AchievementID);
			return -1;
		}

		pstCategory = z_find_achievement_category_def(pstEnv,pstDef->CategoryID);

		if (!pstCategory)
		{
			continue;
		}
		
		pstTail = z_get_achievement_tail(pstDef);

		if (!pstTail)
		{
			continue;
		}

		pstTail->iNum = 0;
		pstTail->iLevel = pstCategory->Level;

		for (k = 0; k < MAX_CATEGORY_ACHIEVEMENT; k++)
		{
			if (pstCategory->AchievementID[k] == pstDef->AchievementID)
			{
				pstTail->iValid = 1;
				break;
			}
		}

		if (!pstTail->iValid)
		{
			continue;
		}

		pstIDMap = z_find_cond_id_map(pstEnv,pstDef->Cond.Type);
		if (!pstIDMap)
		{
			CONDIDMAP stIDMap;

			stIDMap.iAchievementID = pstDef->AchievementID;
			
			hash_insert_cond_id_map(pstEnv,pstDef->Cond.Type,(char*) &stIDMap,sizeof(stIDMap));

			pstIDMap = z_find_cond_id_map(pstEnv,pstDef->Cond.Type);
		}

		if (!pstIDMap)
		{
			printf("CONDIDMAP error\n");
			return -1;
		}

		pstMapDef = z_find_achievement_def(pstEnv,pstIDMap->iAchievementID);
		if (pstMapDef)
		{
			pstMapTail = z_get_achievement_tail(pstMapDef);
		}
		
		if (pstMapTail)
		{
			if (pstMapTail->iNum >= MAX_ACHIEVEMENT_TAIL_ID)
			{
				printf("pstMapTail->iNum >= MAX_ACHIEVEMENT_TAIL_ID\n");
				return -1;
			}
			pstMapTail->aiID[pstMapTail->iNum] = pstDef->AchievementID;
			pstMapTail->iNum++;
		}

		if (pstDef->Cond.Type == COND_SKILL_USE && 
			pstDef->Cond.Val1) 
		{
			int k = 0;
			for (k = pstDef->Cond.Val2; k <= pstDef->Cond.Val3;k++)
			{
				pstSkillDef = z_find_skill(pstEnv,pstDef->Cond.Val1,k,&iIdx);

				if (pstSkillDef)
				{
					SkillDefTail *pstTail = z_get_skill_tail(pstSkillDef);
					if (pstTail)
					{
						pstTail->iAchievementID = pstDef->AchievementID;
					}
				}
			}
		}
		
		

	}
	return 0;
}


int z_pre_status(ZONESVRENV* pstEnv)
{
	int i, j;
	unsigned short ResultID;
	StatusResultMap *pstMap;
	STATUSDEF *pstStatusDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	SKILLDEF *pstSkillDef;

	for (i=0; i<pstObj->iStatusDef; i++)
	{
		pstStatusDef = (STATUSDEF *)pstObj->sStatusDef[i];
		pstMap = (StatusResultMap *)((char *)(pstStatusDef)+sizeof(*pstStatusDef));

		memset(pstMap, 0, sizeof(*pstMap));
		for (j=0; j<(int)(sizeof(pstStatusDef->Result)/sizeof(pstStatusDef->Result[0])); j++)
		{
			ResultID = pstStatusDef->Result[j].ResultID;
			if (0 == ResultID)
			{
				break;
			}

			if (ResultID >= MAX_STATUS_RESULT_VAL)
			{
				printf("status def %d result id >= %d\n", pstStatusDef->StatusID, MAX_STATUS_RESULT_VAL);
				return -1;
			}

			SET_STATUS_RESULTMAP(ResultID, pstMap->sMap);
			pstMap->sMapIdx[ResultID] = j;
		}

		if(pstStatusDef->BindSkill.SkillID && 
			(pstStatusDef->BindSkill.Trig  == STATUS_TRIG_NONE || 
			(pstStatusDef->BindSkill.Trig & (STATUS_TRIG_START | STATUS_TRIG_END))))
		{
			pstSkillDef = z_find_skill(pstEnv, pstStatusDef->BindSkill.SkillID, pstStatusDef->BindSkill.Level, NULL);
			if(NULL == pstSkillDef)
			{
				continue;
			}

			if(pstStatusDef->BindSkill.Trig & STATUS_TRIG_START )
			{
				for(j=0; j<MAX_SKILL_RESULT; j++)
				{
					if(pstSkillDef->SkillResult[j].SkillResultID == 0)
					{
						break;
					}
					
					if(pstSkillDef->SkillResult[j].SkillResultID == RESULT_STATUS ||pstSkillDef->SkillResult[j].SkillResultID == RESULT_SELF_STATUS)
					{
						printf("status def %d bindskill result add status\n", pstStatusDef->StatusID);
						return -1;
					}
				}
			}
		}
	}
	
	return 0;
}

int z_pre_pworld(ZONESVRENV* pstEnv)
{
	int i;
	PWORLDDEF *pstPworldDef;

	pstEnv->iMaxPworldExpMul = 0;
	for(i = 0; i<pstEnv->pstMapObj->iPworld; i++)
	{
		pstPworldDef = (PWORLDDEF *)pstEnv->pstMapObj->sPworld[i];
		if(pstPworldDef->PlayerInTime && pstPworldDef->NotEnterAgain  == 0)
		{
			printf("Pworld def %d  PlayerInTime=%d ,NotEnterAgain=%d  error\n", pstPworldDef->PworldID, pstPworldDef->PlayerInTime, pstPworldDef->NotEnterAgain);
			return -1;
		}

		pstEnv->iMaxPworldExpMul += pstPworldDef->OffExpMul;
	}

	return 0;
}

int z_addattr_group_def_check(ZONESVRENV* pstEnv, ADDATTRGROUPDEF *pstAddAttrGroupDef)
{
	int i, j;
	int iRate, iLevelRate;

	//关键字总数
	iRate = 0;
	for(i = 0; i<MAX_RAND_ADD_ATTR_SUM; i++)
	{
		if (pstAddAttrGroupDef->AddAttSum[i].Val == 0)
		{
			break;
		}
		
		iRate += pstAddAttrGroupDef->AddAttSum[i].Rate;
	}
	if (iRate != 100)
	{
		printf("AddAttrDef def  groupid =%d AddAttrSum Rate != 100\n", pstAddAttrGroupDef->GroupID);
		return -1;
	}

	//关键字
	iRate = 0;
	for(i = 0; i<MAX_RAND_ADD_ATTR; i++)
	{
		if (pstAddAttrGroupDef->AddAtt[i].AddAttrID == 0)
		{
			break;
		}
		
		iRate += pstAddAttrGroupDef->AddAtt[i].Rate;

		iLevelRate = 0;
		for(j = 0; j<MAX_RAND_ADD_ATTR_LEVEL; j++)
		{
			if (pstAddAttrGroupDef->AddAtt[i].AddAttrLevel[j].Val == 0)
			{
				break;
			}
			
			iLevelRate += pstAddAttrGroupDef->AddAtt[i].AddAttrLevel[j].Rate;
		}

		if (iLevelRate != 100)
		{
			printf("AddAttrDef def groupid=%d   AddAttr=%d AddAttrLevel Rate != 100\n", pstAddAttrGroupDef->GroupID, pstAddAttrGroupDef->AddAtt[i].AddAttrID);
			return -1;
		}
	}
	if (iRate != 100)
	{
		printf("AddAttrDef def  groupid=%d AddAttr Rate != 100\n", pstAddAttrGroupDef->GroupID);
		return -1;
	}

	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int z_check_addattr(ZONESVRENV* pstEnv)
{
	int i;
	ADDATTRGROUPDEF *pstAddAttrGroupDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	for(i = 0; i<pstObj->iAddAttrGroupDef; i++)
	{
		pstAddAttrGroupDef = (ADDATTRGROUPDEF *)pstObj->sAddAttrGroupDef[i];
		if (0 > z_addattr_group_def_check(pstEnv, pstAddAttrGroupDef))
		{
			return -1;
		}
	}

	return 0;	
}

int z_pre_task(ZONESVRENV* pstEnv)
{
	int i, j;
	int iRet = 0;
	TASKDEF *pstTaskDef;
	TASKDEF *pstPreTaskDef;
	SKILLDEF *pstSkillDef;
	ITEMDEF *pstItemDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	for (i=0; i<pstObj->iTaskDef; i++)
	{
		pstTaskDef = (TASKDEF *)pstObj->sTaskDef[i];
		if (pstTaskDef->TaskGetLimit.PreTaskID > 0)
		{
			pstPreTaskDef = task_def_find(pstEnv, pstTaskDef->TaskGetLimit.PreTaskID, NULL);
			if (NULL == pstPreTaskDef)	
			{
				printf("task %d pre task %d err\n", pstTaskDef->ID, pstTaskDef->TaskGetLimit.PreTaskID);
				return -1;
			}

			pstPreTaskDef->Flag = 1;
		}

		if(pstTaskDef->TaskItem && pstTaskDef->ID != 3010)
		{
			pstItemDef = z_find_itemdef(pstEnv, pstTaskDef->TaskItem);
			if(pstItemDef && pstItemDef->SellPrice > 0)
			{
				printf("Task %d TaskItem %d  SellPrice %d\n", 
					pstTaskDef->ID, pstTaskDef->TaskItem, pstItemDef->SellPrice);
				return -1;
			}
		}

		for(j=0; j<MAX_TASK_CLEAR_ITEM; j++)
		{
			if( pstTaskDef->TaskClearItemID[j] <= 0)
			{
				continue;
			}

			pstItemDef = z_find_itemdef(pstEnv, pstTaskDef->TaskClearItemID[j]);
			if(pstItemDef && pstItemDef->SellPrice > 0)
			{
				printf("Task %d TaskItem %d  SellPrice %d\n", 
					pstTaskDef->ID, pstTaskDef->TaskClearItemID[j], pstItemDef->SellPrice);
				return -1;
			}
		}
	}

	for (i=0; i<pstObj->iSkillDef; i++)
	{
		pstSkillDef = (SKILLDEF *)pstObj->sSkillDef[i];
		if (pstSkillDef->Learn.TaskID > 0)
		{
			pstPreTaskDef = task_def_find(pstEnv, pstSkillDef->Learn.TaskID, NULL);
			if (NULL == pstPreTaskDef)	
			{
				printf("skill %d pre task %d err\n", pstSkillDef->SkillID, pstSkillDef->Learn.TaskID);
				return -1;
			}

			pstPreTaskDef->Flag = 1;
		}
	}

	sj_task_pre(pstEnv);

	return iRet;
}

int z_pre_taskcycle(ZONESVRENV* pstEnv)
{
	int i, j,k;
	TASKDEF *pstTaskDef;
	TASKRECYCLEDEF *pstTaskCycleDef;
	TaskCycleRandTaskID *pstTaskCycleRandTask;

	for (i=0; i<pstEnv->pstZoneObj->iTaskCycleDef; i++)
	{
		pstTaskCycleDef = (TASKRECYCLEDEF *)pstEnv->pstZoneObj->sTaskCycleDef[i];

		for (j=i+1; j<pstEnv->pstZoneObj->iTaskCycleDef; j++)
		{
			TASKRECYCLEDEF *pstTmpDef =  (TASKRECYCLEDEF *)pstEnv->pstZoneObj->sTaskCycleDef[j];
			if (	pstTaskCycleDef->GroupID > 0 &&
				pstTaskCycleDef->GroupID == pstTmpDef->GroupID &&
				pstTaskCycleDef->GroupLimit != pstTmpDef->GroupLimit)
			{
				printf("TaskCycleDef GroupLimit err\n");
				return -1;
			}
		}

		pstTaskCycleRandTask = (TaskCycleRandTaskID *)((char *)pstTaskCycleDef+sizeof(*pstTaskCycleDef));
		pstTaskCycleRandTask->iNum = 0;
	}

	for (i=0; i<pstEnv->pstZoneObj->iTaskDef; i++)
	{
		pstTaskDef = (TASKDEF *)pstEnv->pstZoneObj->sTaskDef[i];

		for (k=0;k<MAX_TASK_TASKCYCLE;k++)
		{
			if (0 == pstTaskDef->TaskRecycleIDs[k])
			{
				break;
			}

			pstTaskCycleDef = taskcycle_def_find(pstEnv, pstTaskDef->TaskRecycleIDs[k], NULL);
			if (NULL == pstTaskCycleDef)
			{
				printf("task def %d no taskcycle def %d\n", pstTaskDef->ID, pstTaskDef->TaskRecycleIDs[k]);
				return -1;
			}

			for (j=0; j<pstTaskCycleDef->CycleNum; j++)
			{
				if (pstTaskCycleDef->TaskRecycles[j].TaskID == pstTaskDef->ID)
				{
					break;
				}
			}

			if (j < pstTaskCycleDef->CycleNum)
			{
				continue;	
			}

			if (pstTaskCycleDef->HideAccType == HIDE_ACC_TYPE_ONE_RAND)
			{
				for (j=0; j<MAX_HIDE_TASK_IN_CYC; j++)
				{
					if (pstTaskCycleDef->HideTaskLists[j] == (int)pstTaskDef->ID)
					{
						break;
					}
				}

				if (j < MAX_HIDE_TASK_IN_CYC)
				{
					continue;
				}
			}
			
			pstTaskCycleRandTask = (TaskCycleRandTaskID *)((char *)pstTaskCycleDef+sizeof(*pstTaskCycleDef));

			if (pstTaskCycleRandTask->iNum >= (int)(sizeof(pstTaskCycleRandTask->aiTaskID)/sizeof(pstTaskCycleRandTask->aiTaskID[0])) )
			{
				return -1;
			}

			pstTaskCycleRandTask->aiTaskID[pstTaskCycleRandTask->iNum++] = pstTaskDef->ID;
		}
	}

	for (i=0; i<pstEnv->pstZoneObj->iTaskCycleDef; i++)
	{
		pstTaskCycleDef = (TASKRECYCLEDEF *)pstEnv->pstZoneObj->sTaskCycleDef[i];

		if (0 >= pstTaskCycleDef->CycleNum || pstTaskCycleDef->CycleNum > MAX_TASK_RECYCLE)
		{
			printf("taskcycle %d cyclenum=%d\n", pstTaskCycleDef->ID, pstTaskCycleDef->CycleNum);
			return -1;
		}

		/*pstTaskCycleRandTask = (TaskCycleRandTaskID *)((char *)pstTaskCycleDef+sizeof(*pstTaskCycleDef));
		if (0 >= pstTaskCycleRandTask->iNum)
		{
			printf("taskcycle %d rand num=0\n", pstTaskCycleDef->ID);
			return -1;
		}*/
	}
	
	return 0;
}
int check_fittings_pick(ZONESVRENV* pstEnv)
{
	char *pAddr;
	int iCount;
	int iUint;
	int i;
	int j;
	FITTINGSDEF *pstFittingsDef = NULL;
	ITEMDEF *pstItemDef = NULL;

	pAddr = pstEnv->pstZoneObj->sFittingsDef[0];
	iCount = pstEnv->pstZoneObj->iFittingsDef;
	iUint = FITTINGS_DATA;
		
	for (i=0; i<iCount; i++)
	{
		pstFittingsDef = (FITTINGSDEF *)(pAddr + i*iUint);
		
		if (pstFittingsDef->FittingsID < 1 || pstFittingsDef->Level < 1 ||
			pstFittingsDef->FittingsResult[0].SkillResultID == 0 || 
			pstFittingsDef->Type >= MAX_FITTINGS_TYPE )
		{
			printf("FittingsDef %d level %d error, please check database FittingsDef tab\n", pstFittingsDef->FittingsID, pstFittingsDef->Level);
			return -1;
		}

		if(pstFittingsDef->PickItemID > 0 )
		{
			pstItemDef = z_find_itemdef(pstEnv, pstFittingsDef->PickItemID);
			if(pstItemDef == NULL )
			{
				printf("FittingsDef %d PickItemID %d error, please check database FittingsDef tab\n", pstFittingsDef->FittingsID,pstFittingsDef->PickItemID);
				return -1;
			}
			/* 再判断下result */
			for (j=0; j<MAX_ITEM_RESULT; j++)
			{
				if(pstItemDef->Result[j].ResultID == RESULT_FITTINGS_INLAY && 
				      pstItemDef->Result[j].ResultVal1 == pstFittingsDef->FittingsID )
				{
					break;
				}
			}
			if( j >= MAX_ITEM_RESULT )
			{
				/* 没有匹配的物品*/	
				printf("FittingsDef %d PickItemID %d error, please check database FittingsDef tab\n", pstFittingsDef->FittingsID,pstFittingsDef->PickItemID);
				return -1;
			}
		}
	}

	return 0;
}

int z_pre_map_ore(ZONESVRENV* pstEnv)
{
	int i, j;
	int iNum;
	MAPOREDEF *pstMapOreDef;
	MapObj *pstObj = pstEnv->pstMapObj;

	for (i=0; i<pstObj->iMapOreDef; i++)
	{
		pstMapOreDef = (MAPOREDEF *)pstObj->sMapOreDef[i];
		
		iNum = 0;
		for(j=0; j<MAX_MAP_ORE_LIST; j++)
		{
			iNum += pstMapOreDef->OreList[j].Num;
		}

		if(iNum > MAX_MAP_ORE)
		{
			printf("MapOreDef MapID = %d  OreNum > MAX_MAP_ORE\n", pstMapOreDef->MapID);
			return -1;
		}
	}

	return 0;
}

int z_pre_rune_addattr(ZONESVRENV* pstEnv)
{
	int i;
	int iRate = 0;
	RUNEADDATTRDEF *pstRuneAddAttrDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	for (i=0; i<pstObj->iRuneAddAttrDef; i++)
	{
		pstRuneAddAttrDef = (RUNEADDATTRDEF *)pstObj->sRuneAddAttrDef[i];
		iRate += pstRuneAddAttrDef->ResetRate;
	}

	if(pstObj->iRuneAddAttrDef > 0 && iRate != 1000000)
	{
		printf("RuneAddAttrDef  ResetRate != 10000000 \n");
		return -1;
	}

	return 0;
}

int z_pre_recipe(ZONESVRENV* pstEnv)
{
	int i, j, k;
	int iRate;
	RECIPEDEF *pstRecipeDef;
	RECIPEPRODUCE *pstProduceList;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	for (i=0; i<pstObj->iRecipeDef; i++)
	{
		pstRecipeDef = (RECIPEDEF *)pstObj->sRecipeDef[i];
		for(j=0; j<MAX_RECIPE_GET_ITEM_LIST; j++)
		{
			pstProduceList = &pstRecipeDef->ProduceList[j];
			
			iRate = 0;
			for(k=0; k<MAX_RECIPE_GET_ITEM; k++)
			{
				iRate += pstProduceList->GetItem[k].Rate;
			}
			/*
			if (iRate > 0 && iRate != 10000)
			{
				printf("SkillType %d RecipeID %d idx=%d  Rate != 100\n", pstRecipeDef->SkillType, pstRecipeDef->RecipeID, j);
				return -1;
			}
			*/
		}
	}

	return 0;
}

int z_check_fairy(ZONESVRENV* pstEnv)
{
	int i,j;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	FAIRYDEF *pstDef;

	for (i=0; i<pstObj->iFairyDef; i++)
	{
		pstDef = (FAIRYDEF *)pstObj->sFairyDef[i];
		for (j=0; j<pstDef->PNum; j++)
		{
			if (pstDef->PItems[j] <= 0)
			{
				printf("FairyDef->PNum is error\n");
				return -1;
			}
		}
	}
	return 0;
}

int z_check_pet_rand_skill(ZONESVRENV* pstEnv)
{
	
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	DIATHESISDEF *pstDef;
	int i;
	pstEnv->iPetRndSkill = 0;

	for (i=0; i<pstObj->iDiathesisDef; i++)
	{
		pstDef = (DIATHESISDEF *)pstObj->sDiathesisDef[i];
		if (	pstDef->PetSkillType & (PET_LEARN_SKILL_TYPE_LINGWU))	// 可领悟技能
		{
			if (pstDef->Level > 10)
			{
				printf("pstDef->PetSkillType == 1 && pstDef->Level >= 3 is error\n");
				return -1;
			}
			if (pstEnv->iPetRndSkill >= PET_RND_SKILL_MAX)
			{
				printf("pstEnv->iPetRndSkill >= PET_RND_SKILL_MAX is error\n");
				return -1;
			}
			pstEnv->astPetRndSkill[pstEnv->iPetRndSkill ].iID = pstDef->DiathesisID;
			pstEnv->astPetRndSkill[pstEnv->iPetRndSkill ].iLevel = pstDef->Level;
			pstEnv->astPetRndSkill[pstEnv->iPetRndSkill ].iGroupID = pstDef->GroupSkill;
			pstEnv->astPetRndSkill[pstEnv->iPetRndSkill ].iRate = 100;
			if (pstDef->ResetRate > 0)
			{
				pstEnv->astPetRndSkill[pstEnv->iPetRndSkill ].iRate = pstDef->ResetRate;
			}
			pstEnv->iPetRndSkill ++;
		}
	}

	/*if (pstEnv->iPetRndSkill < 10)
	{
		printf("pstEnv->iPetRndSkill is error\n");
		return -1;
	}*/
	return 0;
}

int z_check_fenjie(ZONESVRENV* pstEnv)
{
	int i,j;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	ARMFENJIEDEF*pstDef;

	for (i=0; i<pstObj->iArmFenJieDef; i++)
	{
		pstDef = (ARMFENJIEDEF *)pstObj->sArmFenJieDef[i];
		if (pstDef->Level < 0 || pstDef->Important < 0)
		{
			printf("arm fen jie def %u, %u err\n", pstDef->Level, pstDef->Important);
			return -1;
		}

		for (j=0; j<MAX_ARMFENJIE_ITEM; j++)
		{
			if (pstDef->GetItem[j].ItemID !=0 && NULL == z_find_itemdef(pstEnv, pstDef->GetItem[j].ItemID))
			{
				printf("arm fen jie def %u, %u err\n", pstDef->Level, pstDef->Important);
				return -1;
			}

			if ((pstDef->GetItem[j].ItemID == 0 && pstDef->GetItem[j].Rate > 0) ||
				(pstDef->GetItem[j].ItemID !=0 && pstDef->GetItem[j].Num == 0))
			{
				printf("arm fen jie def %u, %u err\n", pstDef->Level, pstDef->Important);
				return -1;
			}
		}
	}

	return 0;
}

int z_check_offline_exp_parameter(ZONESVRENV* pstEnv)
{
	int i;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	OFFLINEEXPPARAMETER* pstDef;

	for (i=0; i<pstObj->iOfflineExpParameter; i++)
	{
		pstDef = (OFFLINEEXPPARAMETER *)pstObj->sOfflineExpParameter[i];
		if (0 == pstDef->Parameter1)//防止除0
		{
			printf("OfflineExpParameter->Parameter1 is error\n");
			return -1;
		}	
	}
	return 0;
}

int z_check_onlinegiftgroup(ZONESVRENV* pstEnv)
{
	int i,j;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	ONLINEGIFTGROUP *pstDef;
	int iDay;

	for (i=0; i<pstObj->iOnlineGiftGroup; i++)
	{
		pstDef = (ONLINEGIFTGROUP *)pstObj->sOnlineGiftGroup[i];
		if (pstDef->OneNum <= 0 ||pstDef->OneNum >MAX_GIFT_GROUP)
		{
			printf("onlinegiftgroup:OneNum=%d is error\n",pstDef->OneNum);
			return -1;
		}

		
		for (j=0; j<pstDef->OneNum; j++)
		{
			iDay = pstDef->GiftOnes[j].Day;
			if (pstDef->GiftOnes[j].Day <= 0)
			{
				printf("onlinegiftgroup:OneNum is error\n");
				return -1;
			}

			if (j != pstDef->OneNum-1)
			{
				if (iDay >= pstDef->GiftOnes[j+1].Day)
				{
					printf("onlinegiftgroup:OneNum order is error\n");
					return -1;
				}
			}
		}
		
	}
	return 0;
}

int z_pre_levelup(ZONESVRENV* pstEnv)
{
	int i;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	LEVELUP  *pstLevelUP;
	
	for (i=0; i<pstObj->iLevelUpDef; i++)
	{
		pstLevelUP = (LEVELUP  *)pstObj->sLevelUpDef[i];
		if (pstLevelUP->LevelStand)
		{
			pstEnv->iLevelStandStart = pstLevelUP->Level;
			break;
		}
	}
	
	return 0;
}

int z_check_npcshop(ZONESVRENV* pstEnv)
{
	int i, j;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	NPCSELLLIST *pstSellList;
	ITEMDEF *pstItemDef;

	for (i=0; i<pstObj->iSellDef; i++)
	{
		pstSellList = (NPCSELLLIST *)pstObj->sSellDef[i];
		for (j=0; j<(int)pstSellList->Count; j++)
		{
			if (0 == pstSellList->Goods[j].GoodsID)
			{
				break;
			}
			
			pstItemDef = z_find_itemdef(pstEnv, pstSellList->Goods[j].GoodsID);
			if (pstItemDef && pstItemDef->SellPrice > pstItemDef->BuyPrice && pstItemDef->ClanContri <= 0 &&
				pstItemDef->ExchgItemID <= 0 && pstItemDef->MoneyUses[0].ID == 0)
			{
				return -1;
			}
		}
	}
	
	return 0;
}

int z_load_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int iShmID;
	int iReload;
	int iIsExist=0;
			
	iShmID	=	shmget(pstEnv->pstConf->ResShmKey, sizeof(ZoneObj), 0666 | IPC_CREAT | IPC_EXCL);
	if( iShmID<0 )
	{
		iIsExist	=	1;
		iShmID		=	shmget(pstEnv->pstConf->ResShmKey, sizeof(ZoneObj), 0666);
	}
	
	if( iShmID<0 )
		return -1;
	
	if (pstEnv->pstZoneObj)
	{
		shmdt(pstEnv->pstZoneObj);
	}

	pstEnv->pstZoneObj =	(ZoneObj *)shmat(iShmID, NULL, 0);
	
	if( (void *) -1  == pstEnv->pstZoneObj )
		return -1;

	if (!iIsExist)
	{
		memset(pstEnv->pstZoneObj, 0, offsetof(ZoneObj, iInitRes));
		
		pstEnv->pstZoneObj->iVersion = OBJ_SHM_VER;
		
		pstEnv->pstZoneObj->iSIdxNum = 0;
		
		if (0 > tmempool_init(&pstEnv->pstItemPool, MAX_ITEM, DROP_ITEM_DATA, 
								pstEnv->pstZoneObj->sItem, sizeof(pstEnv->pstZoneObj->sItem)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_init item fail");
			return -1;
		}
		
		if (0 > tmempool_init(&pstEnv->pstMonsterPool, MAX_MONSTER, MONSTER_DATA, 
								pstEnv->pstZoneObj->sMonster, sizeof(pstEnv->pstZoneObj->sMonster)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_init monster fail");
			return -1;
		}

		if (0 > tmempool_init(&pstEnv->pstNpcPool, MAX_NPC, NPC_DATA, 
								pstEnv->pstZoneObj->sNpc, sizeof(pstEnv->pstZoneObj->sNpc)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_init npc fail");
			return -1;
		}

		if (0 > tmempool_init(&pstEnv->pstScriptMiniPool, SCRIPT_MINI_NUM, SCRIPT_MINI_SIZE, 
								pstEnv->pstZoneObj->sScriptMini, sizeof(pstEnv->pstZoneObj->sScriptMini)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_init ScriptMini fail");
			return -1;
		}

		if (0 > tmempool_init(&pstEnv->pstScriptMiddlePool, SCRIPT_MIDDLE_NUM, SCRIPT_MIDDLE_SIZE, 
								pstEnv->pstZoneObj->sScriptMiddle, sizeof(pstEnv->pstZoneObj->sScriptMiddle)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_init ScriptMiddle fail");
			return -1;
		}

		if (0 > tmempool_init(&pstEnv->pstScriptLargePool, SCRIPT_LARGE_NUM, SCRIPT_LARGE_SIZE, 
								pstEnv->pstZoneObj->sScriptLarge, sizeof(pstEnv->pstZoneObj->sScriptLarge)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_init ScriptLarge fail");
			return -1;
		}

		if (0 > tmempool_init(&pstEnv->pstScriptHugePool, SCRIPT_HUGE_NUM, SCRIPT_HUGE_SIZE, 
								pstEnv->pstZoneObj->sScriptHuge, sizeof(pstEnv->pstZoneObj->sScriptHuge)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_init ScriptHuge fail");
			return -1;
		}

		if (0 > tmempool_init(&pstEnv->pstTeamPool, MAX_TEAM, TEAM_DATA, 
								pstEnv->pstZoneObj->sTeam, sizeof(pstEnv->pstZoneObj->sTeam)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_init team fail");
			return -1;
		}

		if (0 > tmempool_init(&pstEnv->pstMailPool, MAIL_MAX_TOTAL_NUM, ZONE_MAIL_DATA,
								pstEnv->pstZoneObj->sMail, sizeof(pstEnv->pstZoneObj->sMail)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_init mail fail");
			return -1;
		}

		pstEnv->pstShtNameMemID = sht_init(pstEnv->pstZoneObj->sNameSht, sizeof(pstEnv->pstZoneObj->sNameSht),
											PLAYER_BUCKET, MAX_PLAYER, sizeof(NameMemID));
		if (NULL == pstEnv->pstShtNameMemID)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  NameMemID fail");
			return -1;
		}

		pstEnv->pstShtL2P = sht_init(pstEnv->pstZoneObj->sL2PSht, sizeof(pstEnv->pstZoneObj->sL2PSht),
									 PLAYER_BUCKET, MAX_PLAYER, sizeof(LoginName2MemID));
		if (NULL == pstEnv->pstShtL2P)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  LoginName2MemID fail");
			return -1;
		}

		pstEnv->pstShtClan = sht_init(pstEnv->pstZoneObj->sClanSht, sizeof(pstEnv->pstZoneObj->sClanSht),
											CLAN_BUCKET, CLAN_MAX_NUM_HARD, sizeof(ClanHash));
		if (NULL == pstEnv->pstShtClan)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  clan hash fail");
			return -1;
		}

		pstEnv->pstShtTrust = sht_init(pstEnv->pstZoneObj->sTrustSht, sizeof(pstEnv->pstZoneObj->sTrustSht),
											TRUST_BUCKET, MAX_PLAYER, sizeof(ROLETRUSTDATA));
		if (NULL == pstEnv->pstShtTrust)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  trust hash fail");
			return -1;
		}

		pstEnv->pstShtMailbox = sht_init(pstEnv->pstZoneObj->sMailboxSht, sizeof(pstEnv->pstZoneObj->sMailboxSht),
			MAILBOX_BUCKET, MAIL_MAX_MAILBOX, sizeof(MailboxHash) );
		if (NULL == pstEnv->pstShtMailbox)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  mailbox hash fail");
			return -1;
		}

		pstEnv->pstShtEvent = sht_init(pstEnv->pstZoneObj->sEventSht, sizeof(pstEnv->pstZoneObj->sEventSht),
											EVENT_BUCKET, MAX_GAME_EVENT, sizeof(GameEventInst));
		if (NULL == pstEnv->pstShtEvent)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  game event hash fail");
			return -1;
		}

		pstEnv->pstShopHash = sht_init(pstEnv->pstZoneObj->stShopMemory.sMem, 
                             			sizeof(pstEnv->pstZoneObj->stShopMemory.sMem),
							MAX_BUCKET_SHOP_RECORD, 
							MAX_SHOP_DEF_LINES, 
							sizeof(ShopBucket));
		if (NULL == pstEnv->pstShopHash)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  game shop hash fail");
			return -1;
		}		
		pstEnv->pstShopLimitDefHash = sht_init(pstEnv->pstZoneObj->stShopLimitDefMemory.sMem, 
                             			sizeof(pstEnv->pstZoneObj->stShopLimitDefMemory.sMem),
							MAX_BUCKET_SHOP_LIMIT_RECORD, 
							MAX_SHOP_DEF_LINES, 
							sizeof(ShopLimitBucket));
		if (NULL == pstEnv->pstShopLimitDefHash)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  game shop limit hash fail");
			return -1;
		}

		pstEnv->pstVipBarHash = sht_init(pstEnv->pstZoneObj->stVipBarMemory.sMem, 
                             			sizeof(pstEnv->pstZoneObj->stVipBarMemory.sMem),
							MAX_BUCKET_VIP_BAR_RECORD, 
							MAX_VIP_BAR_DEF_LINES, 
							sizeof(VipBarBucket));
		if (NULL == pstEnv->pstVipBarHash)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  game vipbar hash fail");
			return -1;
		}		

		pstEnv->pstVipBarLevelHash = sht_init(pstEnv->pstZoneObj->stVipBarLevelMemory.sMem, 
                             			sizeof(pstEnv->pstZoneObj->stVipBarLevelMemory.sMem),
							MAX_BUCKET_VIP_BAR_LEVEL_RECORD, 
							MAX_VIP_BAR_LEVEL_DEF_LINES, 
							sizeof(VipBarLevelBucket));
		if (NULL == pstEnv->pstVipBarLevelHash)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  game vipbarlevel hash fail");
			return -1;
		}		

		pstEnv->pstVipBarNumHash = sht_init(pstEnv->pstZoneObj->stVipBarNumMemory.sMem, 
                             			sizeof(pstEnv->pstZoneObj->stVipBarNumMemory.sMem),
							MAX_BUCKET_VIP_BAR_NUM_RECORD, 
							MAX_VIP_BAR_NUM_DEF_LINES, 
							sizeof(VipBarNumBucket));
		if (NULL == pstEnv->pstVipBarNumHash)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  game vipbarnum hash fail");
			return -1;
		}		

		
		pstEnv->pstVipBarOnlineHash = sht_init(pstEnv->pstZoneObj->stVipBarOnlineMemory.sMem, 
												sizeof(pstEnv->pstZoneObj->stVipBarOnlineMemory.sMem),
									MAX_BUCKET_VIP_BAR_ONLINE_RECORD, 
									MAX_VIP_BAR_ONLINE_LINES, 
									sizeof(VipBarOnlineBucket));
		if (NULL == pstEnv->pstVipBarOnlineHash)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  game vipbaronline hash fail");
			return -1;
		}	

		pstEnv->pstDynActiveStatusHash = sht_init(pstEnv->pstZoneObj->stDynActiveStatusMemory.sMem, 
                             			sizeof(pstEnv->pstZoneObj->stDynActiveStatusMemory.sMem),
							MAX_BUCKET_DYN_ACTIVE_STATUS_RECORD, 
							MAX_DYN_ACTIVE_STATUS_LINES, 
							sizeof(DynActiveStatusBucket));
		if (NULL == pstEnv->pstDynActiveStatusHash)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  game dynactivestatus hash fail");
			return -1;
		}		

		pstEnv->pstCondIdMapHash = sht_init(pstEnv->pstZoneObj->stCondIdMapMemory.sMem, 
                             			sizeof(pstEnv->pstZoneObj->stCondIdMapMemory.sMem),
							MAX_BUCKET_COND_ID_MAP_RECORD, 
							MAX_COND_ID_MAP_LINES, 
							sizeof(CondIdMapBucket));
		if (NULL == pstEnv->pstCondIdMapHash)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  game condidmap hash fail");
			return -1;
		}		
						
		pstEnv->pstShtWID2Team = sht_init(pstEnv->pstZoneObj->sWID2TeamSht,
		                               sizeof(pstEnv->pstZoneObj->sWID2TeamSht),
							              TEAM_BUCKET, MAX_TEAM,
							              sizeof(WID2Team));
		if(NULL == pstEnv->pstShtWID2Team)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  team hash fail");
			return -1;
		}

		pstEnv->pstWatchHash = sht_init(pstEnv->pstZoneObj->sWatchSht,
		                               				sizeof(pstEnv->pstZoneObj->sWatchSht),
							             	 	WATCH_BUCKET, MAX_WATCH_ROLE,
							             	 	sizeof(WATCHONE));
		if (NULL == pstEnv->pstWatchHash)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  watch hash fail");
			return -1;
		}

		pstEnv->pstShtStore = sht_init(pstEnv->pstZoneObj->sStoreSht,
		                               			sizeof(pstEnv->pstZoneObj->sStoreSht),
							              STORE_BUCKET, MAX_STORE_NUM,
							              sizeof(StoreHash));
		if (NULL == pstEnv->pstShtStore)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  ShtStore hash fail");
			return -1;
		}

		pstEnv->pstRegionGlobalInfo = (RegionGlobalInfo*)pstEnv->pstZoneObj->sRegionGlobal;
		memset(pstEnv->pstRegionGlobalInfo, 0, sizeof(*pstEnv->pstRegionGlobalInfo));

		pstEnv->pstStoreObj = (StoreObj *)pstEnv->pstZoneObj->sStore;
		pstEnv->pstStoreObj->iStoreNum = 0;
		memset(&pstEnv->pstStoreObj->stObjStoreInfo, 0, sizeof(pstEnv->pstStoreObj->stObjStoreInfo));

		pstEnv->pstHomeObj = (HomeObj *)pstEnv->pstZoneObj->sHome;
		pstEnv->pstHomeObj->iHomeNum = 0;
		memset(&pstEnv->pstHomeObj->stObjHomeInfo, 0, sizeof(pstEnv->pstHomeObj->stObjHomeInfo));

		pstEnv->pstGlobalObj = (GlobalObj *)pstEnv->pstZoneObj->sGlobal;
		memset(&pstEnv->pstGlobalObj->stGlobalObjInfo, 0, sizeof(pstEnv->pstGlobalObj->stGlobalObjInfo));

		pstEnv->pstClanObj = (ClanObj *)pstEnv->pstZoneObj->sClan;
		pstEnv->pstClanObj->iClanNum = 0;
		memset(&pstEnv->pstClanObj->stClanObjInfo, 0, sizeof(pstEnv->pstClanObj->stClanObjInfo));

		pstEnv->pstPostoffice = (Postoffice *)pstEnv->pstZoneObj->sPostoffice;
		pstEnv->pstPostoffice->nMailboxNum = 0;
		memset(&pstEnv->pstPostoffice->stInfo, 0, sizeof(pstEnv->pstPostoffice->stInfo));

		pstEnv->pstBulletinBoard = (BulletinBoard *)pstEnv->pstZoneObj->sBulletinBoard;
		pstEnv->pstBulletinBoard->iNum = 0;

		pstEnv->pstAddAttrFastList = (AddAttrFastList *)pstEnv->pstZoneObj->sAddAttrFastList;
		
		memset(&pstEnv->pstAddAttrFastList->astRandList, 0, sizeof(pstEnv->pstAddAttrFastList->astRandList));
		memset(&pstEnv->pstAddAttrFastList->astWashList, 0, sizeof(pstEnv->pstAddAttrFastList->astWashList));

		pstEnv->pstSJTaskFastList = (SJTaskFastList *)pstEnv->pstZoneObj->sSJTaskFastList;
		memset(pstEnv->pstSJTaskFastList, 0, sizeof(*pstEnv->pstSJTaskFastList));


		pstEnv->pstSpartaQueue = (SpartaQueue *)pstEnv->pstZoneObj->sSpartaData;
		memset(pstEnv->pstSpartaQueue, 0, sizeof(*pstEnv->pstSpartaQueue));

		pstEnv->pstShiKongQueue = (ShiKongQueue *)pstEnv->pstZoneObj->sShiKongData;
		memset(pstEnv->pstShiKongQueue, 0, sizeof(*pstEnv->pstShiKongQueue));

		range_init(pstEnv);
		weather_init(pstEnv);

		//只在维护更新时或reload zone_svr时拉取特权网吧表
		z_load_vip_bar_data(pstEnv);
		
	}
	else
	{
		if (OBJ_SHM_VER != pstEnv->pstZoneObj->iVersion)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "shm ver=%d", pstEnv->pstZoneObj->iVersion);
			return -1;
		}

		if (0 > tmempool_attach(&pstEnv->pstItemPool, MAX_ITEM, DROP_ITEM_DATA, 
								pstEnv->pstZoneObj->sItem, sizeof(pstEnv->pstZoneObj->sItem)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_init item fail");
			return -1;
		}
		
		if (0 > tmempool_attach(&pstEnv->pstMonsterPool, MAX_MONSTER, MONSTER_DATA, 
								pstEnv->pstZoneObj->sMonster, sizeof(pstEnv->pstZoneObj->sMonster)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_attach monster fail");
			return -1;
		}

		if (0 > tmempool_attach(&pstEnv->pstNpcPool, MAX_NPC, NPC_DATA, 
								pstEnv->pstZoneObj->sNpc, sizeof(pstEnv->pstZoneObj->sNpc)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_attach npc fail");
			return -1;
		}

		if (0 > tmempool_attach(&pstEnv->pstScriptMiniPool, SCRIPT_MINI_NUM, SCRIPT_MINI_SIZE, 
								pstEnv->pstZoneObj->sScriptMini, sizeof(pstEnv->pstZoneObj->sScriptMini)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_attach ScriptMini fail");
			return -1;
		}

		if (0 > tmempool_attach(&pstEnv->pstScriptMiddlePool, SCRIPT_MIDDLE_NUM, SCRIPT_MIDDLE_SIZE, 
								pstEnv->pstZoneObj->sScriptMiddle, sizeof(pstEnv->pstZoneObj->sScriptMiddle)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_attach ScriptMiddle fail");
			return -1;
		}

		if (0 > tmempool_attach(&pstEnv->pstScriptLargePool, SCRIPT_LARGE_NUM, SCRIPT_LARGE_SIZE, 
								pstEnv->pstZoneObj->sScriptLarge, sizeof(pstEnv->pstZoneObj->sScriptLarge)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_attach ScriptLarge fail");
			return -1;
		}

		if (0 > tmempool_attach(&pstEnv->pstScriptHugePool, SCRIPT_HUGE_NUM, SCRIPT_HUGE_SIZE, 
								pstEnv->pstZoneObj->sScriptHuge, sizeof(pstEnv->pstZoneObj->sScriptHuge)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_attach ScriptHuge fail");
			return -1;
		}

		if (0 > tmempool_attach(&pstEnv->pstTeamPool, MAX_TEAM, TEAM_DATA, 
								pstEnv->pstZoneObj->sTeam, sizeof(pstEnv->pstZoneObj->sTeam)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_attach team fail");
			return -1;
		}

		if (0 > tmempool_attach(&pstEnv->pstMailPool, MAIL_MAX_TOTAL_NUM, ZONE_MAIL_DATA,
								pstEnv->pstZoneObj->sMail, sizeof(pstEnv->pstZoneObj->sMail)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_attach mail fail");
			return -1;
		}

		pstEnv->pstShtNameMemID = sht_attach(pstEnv->pstZoneObj->sNameSht, sizeof(pstEnv->pstZoneObj->sNameSht),
											PLAYER_BUCKET, MAX_PLAYER, sizeof(NameMemID));
		if (NULL == pstEnv->pstShtNameMemID)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach  NameMemID fail");
			return -1;
		}

		pstEnv->pstShtL2P = sht_attach(pstEnv->pstZoneObj->sL2PSht, sizeof(pstEnv->pstZoneObj->sL2PSht),
									   PLAYER_BUCKET, MAX_PLAYER, sizeof(LoginName2MemID));
		if (NULL == pstEnv->pstShtNameMemID)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach  LoginName2MemID fail");
			return -1;
		}

		pstEnv->pstShtClan = sht_attach(pstEnv->pstZoneObj->sClanSht, sizeof(pstEnv->pstZoneObj->sClanSht),
											CLAN_BUCKET, CLAN_MAX_NUM_HARD, sizeof(ClanHash));
		if (NULL == pstEnv->pstShtClan)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach  clan hash fail");
			return -1;
		}

		pstEnv->pstShtTrust= sht_attach(pstEnv->pstZoneObj->sTrustSht, sizeof(pstEnv->pstZoneObj->sTrustSht),
											TRUST_BUCKET, MAX_PLAYER, sizeof(ROLETRUSTDATA));
		if (NULL == pstEnv->pstShtTrust)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach  trust hash fail");
			return -1;
		}

		pstEnv->pstShtMailbox = sht_attach(pstEnv->pstZoneObj->sMailboxSht, sizeof(pstEnv->pstZoneObj->sMailboxSht),
			MAILBOX_BUCKET, MAIL_MAX_MAILBOX, sizeof(MailboxHash) );
		if (NULL == pstEnv->pstShtMailbox)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach  mailbox hash fail");
			return -1;
		}

		pstEnv->pstShtEvent = sht_attach(pstEnv->pstZoneObj->sEventSht, sizeof(pstEnv->pstZoneObj->sEventSht),
											EVENT_BUCKET, MAX_GAME_EVENT, sizeof(GameEventInst));
		if (NULL == pstEnv->pstShtEvent)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach  game event hash fail");
			return -1;
		}

		pstEnv->pstShopHash = sht_attach(pstEnv->pstZoneObj->stShopMemory.sMem, 
                             			sizeof(pstEnv->pstZoneObj->stShopMemory.sMem),
							MAX_BUCKET_SHOP_RECORD, 
							MAX_SHOP_DEF_LINES, 
							sizeof(ShopBucket));
		if (NULL == pstEnv->pstShopHash)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach  game shop hash fail");
			return -1;
		}
		pstEnv->pstShopLimitDefHash = sht_attach(pstEnv->pstZoneObj->stShopLimitDefMemory.sMem, 
                             			sizeof(pstEnv->pstZoneObj->stShopLimitDefMemory.sMem),
							MAX_BUCKET_SHOP_RECORD, 
							MAX_SHOP_DEF_LINES, 
							sizeof(ShopLimitBucket));
		if (NULL == pstEnv->pstShopLimitDefHash)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach  game shop hash fail");
			return -1;
		}

		pstEnv->pstVipBarHash = sht_attach(pstEnv->pstZoneObj->stVipBarMemory.sMem, 
                             			sizeof(pstEnv->pstZoneObj->stVipBarMemory.sMem),
							MAX_BUCKET_VIP_BAR_RECORD, 
							MAX_VIP_BAR_DEF_LINES, 
							sizeof(VipBarBucket));
		if (NULL == pstEnv->pstVipBarHash)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach  game vipbar hash fail");
			return -1;
		}

		pstEnv->pstVipBarLevelHash = sht_attach(pstEnv->pstZoneObj->stVipBarLevelMemory.sMem, 
                             			sizeof(pstEnv->pstZoneObj->stVipBarLevelMemory.sMem),
							MAX_BUCKET_VIP_BAR_LEVEL_RECORD, 
							MAX_VIP_BAR_LEVEL_DEF_LINES, 
							sizeof(VipBarLevelBucket));
		if (NULL == pstEnv->pstVipBarLevelHash)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach  game vipbarlevel hash fail");
			return -1;
		}

		pstEnv->pstVipBarNumHash = sht_attach(pstEnv->pstZoneObj->stVipBarNumMemory.sMem, 
                             			sizeof(pstEnv->pstZoneObj->stVipBarNumMemory.sMem),
							MAX_BUCKET_VIP_BAR_NUM_RECORD, 
							MAX_VIP_BAR_NUM_DEF_LINES, 
							sizeof(VipBarNumBucket));
		if (NULL == pstEnv->pstVipBarNumHash)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach  game vipbarnum hash fail");
			return -1;
		}		

		
		pstEnv->pstVipBarOnlineHash = sht_attach(pstEnv->pstZoneObj->stVipBarOnlineMemory.sMem, 
														sizeof(pstEnv->pstZoneObj->stVipBarOnlineMemory.sMem),
											MAX_BUCKET_VIP_BAR_ONLINE_RECORD, 
											MAX_VIP_BAR_ONLINE_LINES, 
											sizeof(VipBarOnlineBucket));
		if (NULL == pstEnv->pstVipBarOnlineHash)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach  game vipbaronline hash fail");
			return -1;
		}

		
		pstEnv->pstDynActiveStatusHash = sht_attach(pstEnv->pstZoneObj->stDynActiveStatusMemory.sMem, 
                             			sizeof(pstEnv->pstZoneObj->stDynActiveStatusMemory.sMem),
							MAX_BUCKET_DYN_ACTIVE_STATUS_RECORD, 
							MAX_DYN_ACTIVE_STATUS_LINES, 
							sizeof(DynActiveStatusBucket));
		if (NULL == pstEnv->pstDynActiveStatusHash)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach  game dynactivestatus hash fail");
			return -1;
		}

		pstEnv->pstCondIdMapHash = sht_attach(pstEnv->pstZoneObj->stCondIdMapMemory.sMem, 
                             			sizeof(pstEnv->pstZoneObj->stCondIdMapMemory.sMem),
							MAX_BUCKET_COND_ID_MAP_RECORD, 
							MAX_COND_ID_MAP_LINES, 
							sizeof(CondIdMapBucket));
		if (NULL == pstEnv->pstCondIdMapHash)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach  game condidmap hash fail");
			return -1;
		}

		pstEnv->pstShtWID2Team = sht_attach(pstEnv->pstZoneObj->sWID2TeamSht,
		                               sizeof(pstEnv->pstZoneObj->sWID2TeamSht),
									        TEAM_BUCKET, MAX_TEAM,
									        sizeof(WID2Team));

		
		if(NULL == pstEnv->pstShtWID2Team)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach  WID2Team fail");
			return -1;
		}

		pstEnv->pstWatchHash = sht_attach(pstEnv->pstZoneObj->sWatchSht,
		                              					sizeof(pstEnv->pstZoneObj->sWatchSht),
									        	WATCH_BUCKET, MAX_WATCH_ROLE,
									        	sizeof(WATCHONE));

		if (pstEnv->pstWatchHash == NULL)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach watch hash fail");
			return -1;
		}

		pstEnv->pstShtStore = sht_attach(pstEnv->pstZoneObj->sStoreSht,
		                               			sizeof(pstEnv->pstZoneObj->sStoreSht),
							              STORE_BUCKET, MAX_STORE_NUM,
							              sizeof(StoreHash));
		if (NULL == pstEnv->pstShtStore)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_attach  ShtStore hash fail");
			return -1;
		}
	}

	pstEnv->iZoneObjShmID = iShmID;

	if (RANGE_STAT_FAIL == pstEnv->pstZoneObj->stRange.iState)
	{
		range_init(pstEnv);
	}
	
	pstEnv->pstClanObj = (ClanObj *)pstEnv->pstZoneObj->sClan;
	if (CLAN_STAT_FAIL == pstEnv->pstClanObj->stClanObjInfo.iClanStat)
	{
		memset(&pstEnv->pstClanObj->stClanObjInfo, 0, sizeof(pstEnv->pstClanObj->stClanObjInfo));
		pstEnv->pstShtClan = sht_init(pstEnv->pstZoneObj->sClanSht, sizeof(pstEnv->pstZoneObj->sClanSht),
											CLAN_BUCKET, CLAN_MAX_NUM_HARD, sizeof(ClanHash));
		if (NULL == pstEnv->pstShtClan)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  clan hash fail");
			return -1;
		}
	}

	pstEnv->pstStoreObj = (StoreObj *)pstEnv->pstZoneObj->sStore;
	if (pstEnv->pstStoreObj->stObjStoreInfo.iStat != STORE_STAT_FINI)
	{
		pstEnv->pstStoreObj->iStoreNum = 0;
		memset(&pstEnv->pstStoreObj->stObjStoreInfo, 0, sizeof(pstEnv->pstStoreObj->stObjStoreInfo));	
		pstEnv->pstShtStore = sht_init(pstEnv->pstZoneObj->sStoreSht,
		                               			sizeof(pstEnv->pstZoneObj->sStoreSht),
							              STORE_BUCKET, MAX_STORE_NUM,
							              sizeof(StoreHash));
		if (NULL == pstEnv->pstShtStore)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "sht_init  ShtStore hash fail");
			return -1;
		}
		pstEnv->pstStoreObj->stObjStoreInfo.iStat = STORE_STAT_INIT;
	}

	// 家园数据不大,没必要哈希了
	pstEnv->pstHomeObj = (HomeObj*)pstEnv->pstZoneObj->sHome;
	if (pstEnv->pstHomeObj->stObjHomeInfo.iStat != HOME_STAT_FINI)
	{
		pstEnv->pstHomeObj->iHomeNum = 0;
		memset(&pstEnv->pstHomeObj->stObjHomeInfo, 0, sizeof(pstEnv->pstHomeObj->stObjHomeInfo));
	}

	pstEnv->pstRegionGlobalInfo = (RegionGlobalInfo*)pstEnv->pstZoneObj->sRegionGlobal;
	if (pstEnv->pstRegionGlobalInfo->stStat.iStat != REGION_GLOBAL_STAT_FINI)
	{
		memset(pstEnv->pstRegionGlobalInfo, 0, sizeof(*pstEnv->pstRegionGlobalInfo));	
	}
	
	pstEnv->pstGlobalObj = (GlobalObj *)pstEnv->pstZoneObj->sGlobal;
	if (GLOBAL_STAT_FAIL == pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat)
	{
		memset(&pstEnv->pstGlobalObj->stGlobalObjInfo, 0, sizeof(pstEnv->pstGlobalObj->stGlobalObjInfo));
	}
	pstEnv->pstGlobalObj->stGlobalObjInfo.iStrongStat = 0;

	pstEnv->pstPostoffice = (Postoffice *)pstEnv->pstZoneObj->sPostoffice;
	pstEnv->pstBulletinBoard = (BulletinBoard *)pstEnv->pstZoneObj->sBulletinBoard;
	pstEnv->pstWorldEvent = (WorldEvent *)pstEnv->pstZoneObj->sWorldEvent;
	pstEnv->pstPworldQueue = (WorldPworldQueue *)pstEnv->pstZoneObj->sPworldQueue;
	pstEnv->pstAddAttrFastList = (AddAttrFastList *)pstEnv->pstZoneObj->sAddAttrFastList;
	pstEnv->pstSJTaskFastList = (SJTaskFastList *)pstEnv->pstZoneObj->sSJTaskFastList;
	pstEnv->pstSpartaQueue = (SpartaQueue *)pstEnv->pstZoneObj->sSpartaData;
	pstEnv->pstShiKongQueue = (ShiKongQueue *)pstEnv->pstZoneObj->sShiKongData;
	
	if (RES_LOAD_BIN == pstEnv->pstConf->ResloadType)
	{	
		if (0 > load_resfile_one(RES_DIR,  "MonsterDef", pstEnv->pstZoneObj->sMonsterDef[0], &pstEnv->pstZoneObj->iMonsterDef,
								&pstEnv->pstZoneObj->tMonsterDef, MONSTER_DEF_DATA, MAX_MONSTER_DEF, &iReload))
		{
			return -1;
		}
		if (0 > z_pre_monster(pstEnv))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR,  "MonsterStateSkillDef", pstEnv->pstZoneObj->sMonsterStateSkillDef[0], &pstEnv->pstZoneObj->iMonsterStateSkillDef,
								&pstEnv->pstZoneObj->tMonsterStateSkillDef, MONSTERSTATESKILL_DEF_DATA, MAX_MONSTERSTATESKILL_DEF, &iReload))
		{
			return -1;
		}
		
		if (0 > load_resfile_one(RES_DIR, "AddAttrDef", pstEnv->pstZoneObj->sAddAttrDef[0], &pstEnv->pstZoneObj->iAddAttrDef, 
								&pstEnv->pstZoneObj->tAddAttrDef, ADDATTR_DEF_DATA, MAX_ADDATTR_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "SuitSet", pstEnv->pstZoneObj->sSuitSetDef[0], &pstEnv->pstZoneObj->iSuitSetDef, 
								&pstEnv->pstZoneObj->tSuitSetDef, SUITSET_DEF_DATA, MAX_SUITSET_DEF, &iReload))
		{
			return -1;
		}
		
		if (load_resfile_one(RES_DIR, "ItemDef", pstEnv->pstZoneObj->sItemDef[0], &pstEnv->pstZoneObj->iItemDef, 
								&pstEnv->pstZoneObj->tItemDef,ITEM_DEF_DATA, MAX_ITEM_DEF, &iReload))
		{
			return -1;
		}
		if (0 > pre_item(pstEnv->pstZoneObj->sItemDef[0], pstEnv->pstZoneObj->iItemDef, ITEM_DEF_DATA))
		{
			return -1;
		}
		
		if (0 > load_resfile_one(RES_DIR, "NpcSellList", pstEnv->pstZoneObj->sSellDef[0], &pstEnv->pstZoneObj->iSellDef, 
								&pstEnv->pstZoneObj->tSellDef, SELL_DEF_DATA, MAX_SELL_DEF, &iReload))
		{
			return -1;
		}
		
		if (0 > load_resfile_one(RES_DIR, "MapTransDef", pstEnv->pstZoneObj->sMapTransDef[0], &pstEnv->pstZoneObj->iMapTransDef, 
								&pstEnv->pstZoneObj->tMapTransDef, MAP_TRANS_DEF_DATA, MAX_MAP_TRANS_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "BirthMapDef", pstEnv->pstZoneObj->sBirthMapDef[0], &pstEnv->pstZoneObj->iBirthMapDef, 
								&pstEnv->pstZoneObj->tBirthMapDef, BIRTHMAP_DEF_DATA, MAX_BIRTHMAP_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ActionStatus", pstEnv->pstZoneObj->sActionStatusDef[0], &pstEnv->pstZoneObj->iActionStatusDef, 
								&pstEnv->pstZoneObj->tActionStatusDef, ACTION_STATUS_DEF_DATA, RES_STATUS_MAX, &iReload))
		{
			return -1;
		}

		if (0 > z_act_status_init(pstAppCtx, pstEnv))
		{
			printf("init act_status fail\n");
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "LevelUP", pstEnv->pstZoneObj->sLevelUpDef[0], &pstEnv->pstZoneObj->iLevelUpDef, 
								&pstEnv->pstZoneObj->tLevelUpDef, LEVELUP_DEF_DATA, MAX_LEVEL, &iReload))
		{
			return -1;
		}

		if (0 > z_pre_levelup(pstEnv))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ArmLevelHurtDef", pstEnv->pstZoneObj->sArmLevelHurtDef[0], &pstEnv->pstZoneObj->iArmLevelHurtDef, 
								    &pstEnv->pstZoneObj->tArmLevelHurtDef, ARM_LEVEL_HURT_DATA, MAX_ARM_LEVEL_HURT, &iReload))
		{
			return -1;
		}
		
		if (0 > load_resfile_one(RES_DIR, "SkillDef", pstEnv->pstZoneObj->sSkillDef[0], &pstEnv->pstZoneObj->iSkillDef, 
								    &pstEnv->pstZoneObj->tSkillDef, SKILL_DATA, MAX_SKILL, &iReload))
		{
			return -1;
		}

		if (0 > z_pre_skill(pstEnv))
		{
			return -1;
		}
		
		if (0 > load_resfile_one(RES_DIR, "DiathesisDef", pstEnv->pstZoneObj->sDiathesisDef[0], &pstEnv->pstZoneObj->iDiathesisDef, 
								    &pstEnv->pstZoneObj->tDiathesisDef, DIATHESIS_DATA, MAX_DIATHESIS_DEF, &iReload))
		{
			return -1;
		}

		if ( z_check_pet_rand_skill(pstEnv) < 0)
		{
			return -1;
		}	
		
		if (0 > load_resfile_one(RES_DIR, "FittingsDef", pstEnv->pstZoneObj->sFittingsDef[0], &pstEnv->pstZoneObj->iFittingsDef, 
								    &pstEnv->pstZoneObj->tFittingsDef, FITTINGS_DATA, MAX_SYS_FITTINGS, &iReload))
		{
			return -1;
		}
		
		if (0 > load_resfile_one(RES_DIR, "StatusDef", pstEnv->pstZoneObj->sStatusDef[0], &pstEnv->pstZoneObj->iStatusDef, 
								    &pstEnv->pstZoneObj->tStatusDef, STATUS_DATA, MAX_STATUS, &iReload))
		{
			return -1;
		}
		if (0 > z_pre_status(pstEnv))
		{
			return -1;
		}
		
		if (0 > load_resfile_one(RES_DIR, "OverlayTypeRelationDef", pstEnv->pstZoneObj->sOverlayTypeRelationDef[0], &pstEnv->pstZoneObj->iOverlayTypeRelationDef, 
								    &pstEnv->pstZoneObj->tOverlayTypeRelationDef, OVERLAYTYPE_RELATION_DATA, MAX_OVERLAYTYPE_RELATION, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "StatusOverlayDef", pstEnv->pstZoneObj->sStatusOverlayDef[0], &pstEnv->pstZoneObj->iStatusOverlayDef, 
								    &pstEnv->pstZoneObj->tStatusOverlayDef, STATUS_OVERLAY_DATA, MAX_STATUS_OVERLAY, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "MonDynAttrDef", pstEnv->pstZoneObj->sMonDynAttrDef[0], &pstEnv->pstZoneObj->iMonDynAttrDef, 
								    &pstEnv->pstZoneObj->tMonDynAttrDef, MON_DYN_ATTR_DATA, MAX_LEVEL, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "PatrolDef", pstEnv->pstZoneObj->sPatrolDef[0], &pstEnv->pstZoneObj->iPatrolDef, 
								    &pstEnv->pstZoneObj->tPatrolDef, PATROL_DATA, MAX_PATROL, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "TaskDef", pstEnv->pstZoneObj->sTaskDef[0], &pstEnv->pstZoneObj->iTaskDef, 
								    &pstEnv->pstZoneObj->tTaskDef, TASK_DATA, MAX_TASK, &iReload))
		{
			return -1;
		}
		if (0 > z_pre_task(pstEnv))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "TaskRecycleDef", pstEnv->pstZoneObj->sTaskCycleDef[0], &pstEnv->pstZoneObj->iTaskCycleDef, 
								    &pstEnv->pstZoneObj->tTaskCycleDef, TASK_CYCLE_DATA, MAX_TASK_CYCLE, &iReload))
		{
			return -1;
		}

		if (0 > z_pre_taskcycle(pstEnv))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "NpcInfoDef", pstEnv->pstZoneObj->sNpcInfoDef[0], &pstEnv->pstZoneObj->iNpcInfoDef, 
								    &pstEnv->pstZoneObj->tNpcInfoDef, NPC_INFO_DATA, MAX_NPC_INFO, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "PetDef", pstEnv->pstZoneObj->sPetDef[0], &pstEnv->pstZoneObj->iPetDef, 
								    &pstEnv->pstZoneObj->tPetDef, PET_DATA, MAX_PET, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "PetLevelDef", pstEnv->pstZoneObj->sPetLevelDef[0], &pstEnv->pstZoneObj->iPetLevelDef, 
								    &pstEnv->pstZoneObj->tPetLevelDef, PETLEVEL_DATA, MAX_PETLEVEL, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "PetTraitDef", pstEnv->pstZoneObj->sPetTraitDef[0], &pstEnv->pstZoneObj->iPetTraitDef, 
								    &pstEnv->pstZoneObj->tPetTraitDef, PET_TRAIT_DEF_DATA, MAX_PET_TRAIT_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "PetStrengthenDef", pstEnv->pstZoneObj->sPetStrengthenDef[0], &pstEnv->pstZoneObj->iPetStrengthenDef, 
								    &pstEnv->pstZoneObj->tPetStrengthenDef, PET_STRENGTHEN_DEF_DATA, MAX_PET_STRENGTHEN_DEF, &iReload))
		{
			return -1;
		}

		if (0 > z_pre_pet(pstEnv))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "MonDynExpLimitDef", pstEnv->pstZoneObj->sMonDynExpLimitDef[0], &pstEnv->pstZoneObj->iMonDynExpLimitDef, 
								    &pstEnv->pstZoneObj->tMonDynExpLimitDef, MONDYNEXPLIMIT_DATA, MAX_MONDYNEXPLIMIT, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "QiYueDef", pstEnv->pstZoneObj->sQiYueDef[0], &pstEnv->pstZoneObj->iQiYueDef, 
								    &pstEnv->pstZoneObj->tQiYueDef, QIYUE_DEF_DATA, MAX_QIYUE, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ArmGradeDef", pstEnv->pstZoneObj->sArmGradeDef[0], &pstEnv->pstZoneObj->iArmGradeDef, 
								    &pstEnv->pstZoneObj->tArmGradeDef, ARMGRADE_DATA, MAX_ARMGRADE_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ArmRandDef", pstEnv->pstZoneObj->sArmRandDef[0], &pstEnv->pstZoneObj->iArmRandDef, 
								    &pstEnv->pstZoneObj->tArmRandDef, ARMRAND_DATA, MAX_ARMRAND_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ArmHideAttrDef", pstEnv->pstZoneObj->sArmHideAttrDef[0], &pstEnv->pstZoneObj->iArmHideAttrDef, 
								    &pstEnv->pstZoneObj->tArmHideAttrDef, ARMHIDEATTR_DATA, MAX_ARMHIDEATTR_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "BirthItemDef", pstEnv->pstZoneObj->sBirthItemDef[0], &pstEnv->pstZoneObj->iBirthItemDef, 
								    &pstEnv->pstZoneObj->tBirthItemDef, BIRTH_ITEM_DEF_DATA, MAX_BIRTH_ITEM_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "MonsterSpeechDef", pstEnv->pstZoneObj->sMonsterSpeechDef[0], &pstEnv->pstZoneObj->iMonsterSpeechDef, 
								    &pstEnv->pstZoneObj->tMonsterSpeechDef, MONSTER_SPEECH_DEF_DATA, MAX_MONSTER_SPEECH_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ArmAddAttrDef", pstEnv->pstZoneObj->sArmAddAttrDef[0], &pstEnv->pstZoneObj->iArmAddAttrDef, 
								    &pstEnv->pstZoneObj->tArmAddAttrDef, ARM_ADDATTR_DEF_DATA, MAX_ARM_ADDATTR_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "OnlineAwardDef", pstEnv->pstZoneObj->sOnlineAwardDef[0], &pstEnv->pstZoneObj->iOnlineAwardDef, 
								    &pstEnv->pstZoneObj->tOnlineAwardDef, ONLINE_AWARD_DEF_DATA, MAX_ONLINE_AWARD_DEF, &iReload))
		{
			return -1;
		}

		if (z_pre_online_award(pstEnv) < 0)
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "TaskLimitGroup", pstEnv->pstZoneObj->sTaskLimitGroup[0], &pstEnv->pstZoneObj->iTaskLimitGroup, 
								    &pstEnv->pstZoneObj->tTaskLimitGroup, TASK_LIMIT_GROUP_DATA, MAX_TASK_LIMIT_GROUP, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ClanCityBuildingLevelDef", pstEnv->pstZoneObj->sClanCityBuildingLevelDef[0], &pstEnv->pstZoneObj->iClanCityBuildingLevelDef, 
								    &pstEnv->pstZoneObj->tClanCityBuildingLevelDef, CLAN_CITY_BUILDING_LEVEL_DEF_DATA, MAX_CLAN_CITY_BUILDING_LEVEL_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ClanCityLevelDef", pstEnv->pstZoneObj->sClanCityLevelDef[0], &pstEnv->pstZoneObj->iClanCityLevelDef, 
								    &pstEnv->pstZoneObj->tClanCityLevelDef, CLAN_CITY_LEVEL_DEF_DATA, MAX_CLAN_CITY_LEVEL_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ClanLevelDef", pstEnv->pstZoneObj->sClanLevelDef[0], &pstEnv->pstZoneObj->iClanLevelDef, 
								    &pstEnv->pstZoneObj->tClanLevelDef, CLAN_LEVEL_DEF_DATA, MAX_CLAN_LEVEL_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "MonsterColorDef", pstEnv->pstZoneObj->sMonsterColorDef[0], &pstEnv->pstZoneObj->iMonsterColorDef, 
								    &pstEnv->pstZoneObj->tMonsterColorDef, MONSTER_COLOR_DEF_DATA, MAX_MONSTER_COLOR_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "GameEventDef", pstEnv->pstZoneObj->sGameEventDef[0], &pstEnv->pstZoneObj->iGameEventDef, 
								    &pstEnv->pstZoneObj->tGameEventDef, GAME_EVENT_DEF_DATA, MAX_GAME_EVENT_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "DesignationDef", pstEnv->pstZoneObj->sDesignationDef[0], &pstEnv->pstZoneObj->iDesignationDef, 
								    &pstEnv->pstZoneObj->tDesignationDef, DESIGNATION_DEF_DATA, MAX_DESIGNATION_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "RespactPartyDef", pstEnv->pstZoneObj->sRespactPartyDef[0], &pstEnv->pstZoneObj->iRespactPartyDef, 
								 &pstEnv->pstZoneObj->tRespactPartyDef, RESPACT_PARTY_DEF_DATA, MAX_RESPACT_PARTY_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "RespactPhaseDef", pstEnv->pstZoneObj->sRespactPhaseDef[0], &pstEnv->pstZoneObj->iRespactPhaseDef, 
								 &pstEnv->pstZoneObj->tRespactPhaseDef, RESPACT_PHASE_DEF_DATA, MAX_RESPACT_PHASE_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "RideDef", pstEnv->pstZoneObj->sRideDef[0], &pstEnv->pstZoneObj->iRideDef, 
								 &pstEnv->pstZoneObj->tRideDef, RIDE_DEF_DATA, MAX_RIDE_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "FairyDef", pstEnv->pstZoneObj->sFairyDef[0], &pstEnv->pstZoneObj->iFairyDef, 
								 &pstEnv->pstZoneObj->tFairyDef, FAIRY_DEF_DATA, MAX_FAIRY_DEF, &iReload))
		{
			return -1;
		}

		if (z_check_fairy(pstEnv) < 0)
		{
			return -1;
		}
	
		if (0 > load_resfile_one(RES_DIR, "AddAttrBaseDef", pstEnv->pstZoneObj->sAddAttrBaseDef[0], &pstEnv->pstZoneObj->iAddAttrBaseDef, 
								 &pstEnv->pstZoneObj->tAddAttrBaseDef, ADD_ATTR_BASE_DEF_DATA, MAX_ADD_ATTR_BASE_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "AddAttrGroupDef", pstEnv->pstZoneObj->sAddAttrGroupDef[0], &pstEnv->pstZoneObj->iAddAttrGroupDef, 
								 &pstEnv->pstZoneObj->tAddAttrGroupDef, ADD_ATTR_GROUP_DEF_DATA, MAX_ADD_ATTR_GROUP_DEF, &iReload))
		{
			return -1;
		}
		if (0> z_check_addattr(pstEnv))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "LVGift", pstEnv->pstZoneObj->sLVGift[0], &pstEnv->pstZoneObj->iLVGift, 
								 &pstEnv->pstZoneObj->tLVGift, GIFT_LV_DATA, MAX_GIFT_LV_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "OneDayGift", pstEnv->pstZoneObj->sOneDayGift[0], &pstEnv->pstZoneObj->iOneDayGift, 
								 &pstEnv->pstZoneObj->tOneDayGift, GIFT_ONEDAY_DATA, MAX_GIFT_ONEDAY_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "FixedAddAttr", pstEnv->pstZoneObj->sFixedAddAttr[0], &pstEnv->pstZoneObj->iFixedAddAttr, 
								 &pstEnv->pstZoneObj->tFixedAddAttr, FIXED_ADD_ATTR_DATA, MAX_FIXED_ADD_ATTR, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "RndAwdDef", pstEnv->pstZoneObj->sRndAwdDef[0], &pstEnv->pstZoneObj->iRndAwdDef, 
								 &pstEnv->pstZoneObj->tRndAwdDef, RAND_AWARD_DATA, MAX_RAND_AWARD_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "BCTDef", pstEnv->pstZoneObj->sBCTDef[0], &pstEnv->pstZoneObj->iBCTDef, 
								 &pstEnv->pstZoneObj->tBCTDef, BCT_DEF_DATA, MAX_BCT_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "SecondarySkillDef", pstEnv->pstZoneObj->sSecondarySkillDef[0], &pstEnv->pstZoneObj->iSecondarySkillDef, 
								 &pstEnv->pstZoneObj->tSecondarySkillDef, SECONDARY_SKILL_DEF_DATA, MAX_SECONDARY_SKILL_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "RecipeDef", pstEnv->pstZoneObj->sRecipeDef[0], &pstEnv->pstZoneObj->iRecipeDef, 
								 &pstEnv->pstZoneObj->tRecipeDef, RECIPE_DEF_DATA, MAX_RECIPE_DEF, &iReload))
		{
			return -1;
		}

		if(0> z_pre_recipe(pstEnv))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ActiveHelper", pstEnv->pstZoneObj->sActiveHelp[0], &pstEnv->pstZoneObj->iActiveHelp, 
								 &pstEnv->pstZoneObj->tActiveHelp, ACTIVE_HELP_DATA, MAX_ACTIVE_HELP, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "MonBootyDef", pstEnv->pstZoneObj->sMonBootyDef[0], &pstEnv->pstZoneObj->iMonBootyDef, 
								 &pstEnv->pstZoneObj->tMonBootyDef, MON_BOOTY_DEF_DATA, MAX_MON_BOOTY_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "MonBootyGroupDef", pstEnv->pstZoneObj->sMonBootyGroupDef[0], &pstEnv->pstZoneObj->iMonBootyGroupDef, 
								 &pstEnv->pstZoneObj->tMonBootyGroupDef, MON_BOOTY_GROUP_DEF_DATA, MAX_MON_BOOTY_GROUP_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "CangBaoTuDef", pstEnv->pstZoneObj->sCangBaoTuDef[0], &pstEnv->pstZoneObj->iCangBaoTuDef, 
								 &pstEnv->pstZoneObj->tCangBaoTuDef, CANGBAOTU_DEF_DATA, MAX_CANGBAOTU_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "MonMachineDef", pstEnv->pstZoneObj->sMonMachineDef[0], &pstEnv->pstZoneObj->iMonMachineDef, 
								 &pstEnv->pstZoneObj->tMonMachineDef, MON_MACHINE_DEF_DATA, MAX_MON_MACHINE_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ClanTechDef", pstEnv->pstZoneObj->sClanTechDef[0], &pstEnv->pstZoneObj->iClanTechDef, 
								 &pstEnv->pstZoneObj->tClanTechDef, CLAN_TECH_DEF_DATA, MAX_CLAN_TECH_DEF, &iReload))
		{
			return -1;
		}
		
		if (0 > load_resfile_one(RES_DIR, "ClanProductDef", pstEnv->pstZoneObj->sClanProductDef[0], &pstEnv->pstZoneObj->iClanProductDef, 
								 &pstEnv->pstZoneObj->tClanProductDef, CLAN_PRODUCT_DEF_DATA, MAX_CLAN_PRODUCT_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "AttrContrastDef", pstEnv->pstZoneObj->sAttrContrastDef[0], &pstEnv->pstZoneObj->iAttrContrastDef, 
								 &pstEnv->pstZoneObj->tAttrContrastDef, ATTR_CONTRAST_DEF_DATA, MAX_ATTR_CONTRAST_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "LevelAttrDef", pstEnv->pstZoneObj->sLevelAttrDef[0], &pstEnv->pstZoneObj->iLevelAttrDef, 
								 &pstEnv->pstZoneObj->tLevelAttrDef, LEVEL_ATTR_DEF_DATA, MAX_LEVEL_ATTR_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "DiePunishDef", pstEnv->pstZoneObj->sDiePunishDef[0], &pstEnv->pstZoneObj->iDiePunishDef, 
								 &pstEnv->pstZoneObj->tDiePunishDef, DIE_PUNISH_DEF_DATA, MAX_DIE_PUNISH_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ContOnlineGiftDef", pstEnv->pstZoneObj->sContOnlineDef[0], &pstEnv->pstZoneObj->iContOnlineDef, 
								 &pstEnv->pstZoneObj->tContOnlineDef, CONT_ONLINE_DEF_DATA, MAX_CONT_ONLINE_DEF, &iReload))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iContOnlineDef > MAX_CONTGIFT)
			return -1;

		if (0 > load_resfile_one(RES_DIR, "BattleDef", pstEnv->pstZoneObj->sBattleDef[0], &pstEnv->pstZoneObj->iBattleDef, 
								 &pstEnv->pstZoneObj->tBattleDef,BATTLE_DEF_DATA, MAX_BATTLE_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "FairyStarStuffDef", pstEnv->pstZoneObj->sFairyStuffDef[0], &pstEnv->pstZoneObj->iFairyStuffDef, 
								 &pstEnv->pstZoneObj->tFairyStuffDef,FAIRY_STUFF_DEF_DATA, MAX_FAIRY_STUFF_DEF, &iReload))
		{
			return -1;
		}

		
		if (0 > load_resfile_one(RES_DIR, "FairyStarDef", pstEnv->pstZoneObj->sFairyStarDef[0], &pstEnv->pstZoneObj->iFairyStarDef, 
								 &pstEnv->pstZoneObj->tFairyStarDef,FAIRY_STAR_DATA, MAX_FAIRY_STAR_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "PetResolveDef", pstEnv->pstZoneObj->sPetResolveDef[0], &pstEnv->pstZoneObj->iPetResolveDef, 
								 &pstEnv->pstZoneObj->tPetResolveDef, PET_RESOLVE_DEF_DATA, MAX_PET_RESOLVE_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "PetSkillGridDef", pstEnv->pstZoneObj->sPetSkillGridDef[0], &pstEnv->pstZoneObj->iPetSkillGridDef, 
								 &pstEnv->pstZoneObj->tPetSkillGridDef, PET_SKILL_GRID_DEF_DATA, MAX_PET_SKILL_GRID_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "PetNativeSkillDef", pstEnv->pstZoneObj->sPetNativeSkillDef[0], &pstEnv->pstZoneObj->iPetNativeSkillDef, 
								 &pstEnv->pstZoneObj->tPetNativeSkillDef, PET_NATIVE_SKILL_DEF_DATA, MAX_PET_NATIVE_SKILL_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "PetFuseDef", pstEnv->pstZoneObj->sPetFuseDef[0], &pstEnv->pstZoneObj->iPetFuseDef, 
								 &pstEnv->pstZoneObj->tPetFuseDef, PET_FUSE_DEF_DATA, MAX_PET_FUSE_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "OfflineExpParameter", pstEnv->pstZoneObj->sOfflineExpParameter[0], &pstEnv->pstZoneObj->iOfflineExpParameter, 
								 &pstEnv->pstZoneObj->tOfflineExpParameter, OFFLINE_EXP_PARAMETER_DATA, MAX_OFFLINE_EXP_PARAMETER, &iReload))
		{
			return -1;
		}

		if (z_check_offline_exp_parameter(pstEnv) < 0)
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "OnlineGiftGroup", pstEnv->pstZoneObj->sOnlineGiftGroup[0], &pstEnv->pstZoneObj->iOnlineGiftGroup, 
								 &pstEnv->pstZoneObj->tOnlineGiftGroup, MAX_ONLINE_GROUP_DATA, MAX_GIFT_GROUP_DEF, &iReload))
		{
			return -1;
		}

		if (z_check_onlinegiftgroup(pstEnv) < 0)
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "WorldItemLimitDef", pstEnv->pstZoneObj->sWorldItemLimitDef[0], &pstEnv->pstZoneObj->iWorldItemLimitDef, 
								 &pstEnv->pstZoneObj->tWorldItemLimitDef, WORLD_ITEM_LIMIT_DEF_DATA, MAX_WORLD_ITEM_LIMIT_DEF, &iReload))
		{
			return -1;
		}

		//更新一下，针对reloadres, reloadall
		global_item_limit_update(pstEnv);

		if (0 > load_resfile_one(RES_DIR, "WorldBootyGroupLimitDef", pstEnv->pstZoneObj->sWorldBootyGroupLimitDef[0], &pstEnv->pstZoneObj->iWorldBootyGroupLimitDef, 
								 &pstEnv->pstZoneObj->tWorldBootyGroupLimitDef, WORLD_BOOTY_GROUP_LIMIT_DEF_DATA, MAX_WORLD_BOOTY_GROUP_LIMIT_DEF, &iReload))
		{
			return -1;
		}

		//更新一下，针对reloadres, reloadall
		global_booty_group_limit_update(pstEnv);

		if (0 > load_resfile_one(RES_DIR, "DiffPworldDef", pstEnv->pstZoneObj->sDiffPworldDef[0], &pstEnv->pstZoneObj->iDiffPworldDef, 
								 &pstEnv->pstZoneObj->tDiffPworldDef, DIFF_PWORLD_DEF_DATA, MAX_DIFF_PWORLD_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "MonEventDef", pstEnv->pstZoneObj->sMonEventDef[0], &pstEnv->pstZoneObj->iMonEventDef, 
								 &pstEnv->pstZoneObj->tMonEventDef, MON_EVENT_DEF_DATA, MAX_MON_EVENT_DEF, &iReload))
		{
			return -1;
		}
		global_mon_event_update(pstEnv);

		if (0 > load_resfile_one(RES_DIR, "PworldBootyGroupDef", pstEnv->pstZoneObj->sPworldBootyGroupDef[0], &pstEnv->pstZoneObj->iPworldBootyGroupDef, 
								 &pstEnv->pstZoneObj->tPworldBootyGroupDef, PWORLD_BOOTY_GROUP_DEF_DATA, MAX_PWORLD_BOOTY_GROUP_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "PworldAwardMulDef", pstEnv->pstZoneObj->sPworldAwardMulDef[0], &pstEnv->pstZoneObj->iPworldAwardMulDef, 
								 &pstEnv->pstZoneObj->tPworldAwardMulDef, PWORLD_AWARD_MUL_DEF_DATA, MAX_PWORLD_AWARD_MUL_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "AtkerHurtMulDef", pstEnv->pstZoneObj->sAtkerHurtMulDef[0], &pstEnv->pstZoneObj->iAtkerHurtMulDef, 
								 &pstEnv->pstZoneObj->tAtkerHurtMulDef, ATKER_HURT_MUL_DEF_DATA, MAX_ATKER_HURT_MUL_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "WorldCityBuildDef", pstEnv->pstZoneObj->sWorldCityBuildDef[0], &pstEnv->pstZoneObj->iWorldCityBuildDef, 
								 &pstEnv->pstZoneObj->tWorldCityBuildDef, WORLD_CITY_BUILD_DEF_DATA, MAX_WORLD_CITY_BUILD_DEF, &iReload))
		{
			return -1;
		}
		global_world_city_update(pstEnv);

		if (0 > load_resfile_one(RES_DIR, "StarStoneDef", pstEnv->pstZoneObj->sStarStoneDef[0], &pstEnv->pstZoneObj->iStarStoneDef, 
								 &pstEnv->pstZoneObj->tStarStoneDef, MAX_STARSTONE_DEF_DATA, MAX_STARSTONE_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "StarFuncDef", pstEnv->pstZoneObj->sStarFuncDef[0], &pstEnv->pstZoneObj->iStarFuncDef, 
								 &pstEnv->pstZoneObj->tStarFuncDef, MAX_STARFUNC_DEF_DATA, MAX_STARFUNC_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "CityProductDef", pstEnv->pstZoneObj->sCityProductDef[0], &pstEnv->pstZoneObj->iCityProductDef, 
								 &pstEnv->pstZoneObj->tCityProductDef, CITY_PRODUCT_DEF_DATA, MAX_CITY_PRODUCT_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ArmFenJieDef", pstEnv->pstZoneObj->sArmFenJieDef[0], &pstEnv->pstZoneObj->iArmFenJieDef, 
								    &pstEnv->pstZoneObj->tArmFenJieDef, ARMFENJIE_DATA, MAX_ARMFENJIE_DEF, &iReload))
		{
			return -1;
		}

		if (0 > z_check_fenjie(pstEnv))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "MediaGiftDef", pstEnv->pstZoneObj->sMediaGiftDef[0], &pstEnv->pstZoneObj->iMediaGiftDef, 
								    &pstEnv->pstZoneObj->tMediaGiftDef, GIFT_MEDIA_DATA, MAX_GIFT_MEDIA_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "LtrGiftDef", pstEnv->pstZoneObj->sLtrGiftDef[0], &pstEnv->pstZoneObj->iLtrGiftDef, 
								    &pstEnv->pstZoneObj->tLtrGiftDef, GIFT_LTR_DATA, MAX_GIFT_LTR_DEF, &iReload))
		{
			return -1;
		}
		
		if (0 > load_resfile_one(RES_DIR, "BattleGloryDef", pstEnv->pstZoneObj->sBattleGlory[0], &pstEnv->pstZoneObj->iBattleGlory, 
								    &pstEnv->pstZoneObj->tBattleGlory, BATTLE_GLORY_DEF_DATA, MAX_BATTLE_GLORY_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "GemNewDef", pstEnv->pstZoneObj->sGemNewDef[0], &pstEnv->pstZoneObj->iGemNewDef, 
								    &pstEnv->pstZoneObj->tGemNewDef, GEM_NEW_DEF_DATA, MAX_GEM_NEW_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "GemAttrStarDef", pstEnv->pstZoneObj->sGemAttrStarDef[0], &pstEnv->pstZoneObj->iGemAttrStarDef, 
								    &pstEnv->pstZoneObj->tGemAttrStarDef, GEM_ATTR_STAR_DEF_DATA, MAX_GEM_ATTR_STAR_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "GemLevelStarDef", pstEnv->pstZoneObj->sGemLevelStarDef[0], &pstEnv->pstZoneObj->iGemLevelStarDef, 
								    &pstEnv->pstZoneObj->tGemLevelStarDef, GEM_LEVEL_STAR_DEF_DATA, MAX_GEM_LEVEL_STAR_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ArmRecastDef", pstEnv->pstZoneObj->sArmRecastDef[0], &pstEnv->pstZoneObj->iArmReCastDef, 
								    &pstEnv->pstZoneObj->tArmRecastDef, ARM_RECAST_DEF_DATA, MAX_ARM_RECAST_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "UseDef", pstEnv->pstZoneObj->sUseDef[0], &pstEnv->pstZoneObj->iUseDef, 
								    &pstEnv->pstZoneObj->tUseDef, USE_DEF_DATA, MAX_USE_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "DailyGoalAward", pstEnv->pstZoneObj->sDailyGoalAward[0], &pstEnv->pstZoneObj->iDailyGoalAward, 
								 &pstEnv->pstZoneObj->tDailyGoalAward, DAILY_GOAL_AWARD_DATA, MAX_DAILY_GOAL_AWARD, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "RuneAddAttrDef", pstEnv->pstZoneObj->sRuneAddAttrDef[0], &pstEnv->pstZoneObj->iRuneAddAttrDef, 
								 &pstEnv->pstZoneObj->tRuneAddAttrDef, RUNE_ADDATTR_DEF_DATA, MAX_RUNE_ADDATTR_DEF, &iReload))
		{
			return -1;
		}

		if(0 > z_pre_rune_addattr(pstEnv))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ArmStarDef", pstEnv->pstZoneObj->sArmStarDef[0], &pstEnv->pstZoneObj->iArmStarDef, 
								 &pstEnv->pstZoneObj->tArmStarDef, ARM_STAR_DEF_DATA, MAX_ARM_STAR_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "UnrealMapDef", pstEnv->pstZoneObj->sUnrealMapDef[0], &pstEnv->pstZoneObj->iUnrealMapDef, 
								 &pstEnv->pstZoneObj->tUnrealMapDef, UNREAL_MAP_DEF_SIZE, MAX_UNREAL_MAP_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "UnrealDef", pstEnv->pstZoneObj->sUnrealDef[0], &pstEnv->pstZoneObj->iUnrealDef, 
								 &pstEnv->pstZoneObj->tUnrealDef, UNREAL_DEF_SIZE, MAX_UNREAL_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "PetWashGeniusDef", pstEnv->pstZoneObj->sPetWashGeniusDef[0], &pstEnv->pstZoneObj->iPetWashGeniusDef, 
								 &pstEnv->pstZoneObj->tPetWashGeniusDef, PET_WASH_GENIUS_DEF_DATA, MAX_PET_WASH_GENIUS_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "NewAddAttrDef", pstEnv->pstZoneObj->sNewAddAttrDef[0], &pstEnv->pstZoneObj->iNewAddAttrDef, 
								 &pstEnv->pstZoneObj->tNewAddAttrDef, NEW_ADD_ATTR_DEF_DATA, MAX_NEW_ADD_ATTR_DEF, &iReload))
		{
			return -1;
		}

		if(0 > add_attr_pre(pstEnv))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "AddAttrRandDef", pstEnv->pstZoneObj->sAddAttrRandDef[0], &pstEnv->pstZoneObj->iAddAttrRandDef, 
								 &pstEnv->pstZoneObj->tAddAttrRandDef, ADD_ATTR_RAND_DEF_DATA, MAX_ADD_ATTR_RAND_DEF, &iReload))
		{
			return -1;
		}


		if (0 > load_resfile_one(RES_DIR, "AchievementCategoryDef", pstEnv->pstZoneObj->sAchievementCategoryDef[0], &pstEnv->pstZoneObj->iAchievementCategoryDef, 
								 &pstEnv->pstZoneObj->tAchievementCategoryDef, ACHIEVEMENT_CATEGORY_DEF_DATA, MAX_ACHIEVEMENT_CATEGORY_DEF, &iReload))
		{
			return -1;
		}
		
		if (0 > load_resfile_one(RES_DIR, "AchievementDef", pstEnv->pstZoneObj->sAchievementDef[0], &pstEnv->pstZoneObj->iAchievementDef, 
								 &pstEnv->pstZoneObj->tAchievementDef, ACHIEVEMENT_DEF_DATA, MAX_ACHIEVEMENT_DEF, &iReload))
		{
			return -1;
		}		

		if (0 > z_pre_achievement(pstEnv))
		{
			return -1;
		}


		if (0 > load_resfile_one(RES_DIR, "AddAttrWashDef", pstEnv->pstZoneObj->sAddAttrWashDef[0], &pstEnv->pstZoneObj->iAddAttrWashDef, 
								 &pstEnv->pstZoneObj->tAddAttrWashDef, ADD_ATTR_WASH_DEF_DATA, MAX_ADD_ATTR_WASH_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "SparDef", pstEnv->pstZoneObj->sSparDef[0], &pstEnv->pstZoneObj->iSparDef, 
								 &pstEnv->pstZoneObj->tSparDef, SPAR_DEF_SIZE, MAX_SPAR_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ItemStarRandDef", pstEnv->pstZoneObj->sItemStarRandDef[0], &pstEnv->pstZoneObj->iItemStarRandDef, 
								 &pstEnv->pstZoneObj->tItemStarRandDef, ITEM_STAR_RAND_DEF_DATA, MAX_ITEM_STAR_RAND_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "SJTaskDef", pstEnv->pstZoneObj->sSJTaskDef[0], &pstEnv->pstZoneObj->iSJTaskDef, 
								 &pstEnv->pstZoneObj->tSJTaskDef, SJ_TASK_DEF_DATA, MAX_SJ_TASK_DEF, &iReload))
		{
			return -1;
		}


		if (0 > load_resfile_one(RES_DIR, "ClanSpyAwardDropDef", pstEnv->pstZoneObj->sClanSpyAwardDropDef[0], &pstEnv->pstZoneObj->iClanSpyAwardDropDef, 
								 &pstEnv->pstZoneObj->tClanSpyAwardDropDef, CLAN_SPY_AWARD_DROP_DEF_DATA, MAX_CLAN_SPY_AWARD_DROP_DEF, &iReload))
		{
			return -1;
		}		

		if (0 > z_pre_clan_spy_award_drop(pstEnv))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ClanSpyTransDef", pstEnv->pstZoneObj->sClanSpyTransDef[0], &pstEnv->pstZoneObj->iClanSpyTransDef, 
								 &pstEnv->pstZoneObj->tClanSpyTransDef, CLAN_SPY_TRANS_DEF_DATA, MAX_CLAN_SPY_TRANS_DEF, &iReload))
		{
			return -1;
		}		

		if (0 > z_pre_clan_spy_trans(pstEnv))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "DartCarDef", pstEnv->pstZoneObj->sDartCarDef[0], &pstEnv->pstZoneObj->iDartCarDef, 
								 &pstEnv->pstZoneObj->tDartCarDef, DART_CAR_DEF_DATA, MAX_DART_CAR_DEF, &iReload))
		{
			return -1;
		}


		if (0 > load_resfile_one(RES_DIR, "PworldWaveMonDef", pstEnv->pstZoneObj->sPworldWaveMonDef[0], &pstEnv->pstZoneObj->iPworldWaveMonDef, 
								 &pstEnv->pstZoneObj->tPworldWaveMonDef, PWORLD_WAVE_MON_DEF_DATA, MAX_PWORLD_WAVE_MON_DEF, &iReload))
		{
			return -1;
		}		

		if (0 > z_pre_pworld_wave_mon_def(pstEnv))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "PworldWaveMonPosDef", pstEnv->pstZoneObj->sPworldWaveMonPosDef[0], &pstEnv->pstZoneObj->iPworldWaveMonPosDef, 
								 &pstEnv->pstZoneObj->tPworldWaveMonPosDef, PWORLD_WAVE_MON_POS_DEF_DATA, MAX_PWORLD_WAVE_MON_POS_DEF, &iReload))
		{
			return -1;
		}		

		if (0 > z_pre_pworld_wave_mon_pos_def(pstEnv))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "PworldWaveMonClanAwardDef", pstEnv->pstZoneObj->sPworldWaveMonClanAwardDef[0], &pstEnv->pstZoneObj->iPworldWaveMonClanAwardDef, 
								 &pstEnv->pstZoneObj->tPworldWaveMonClanAwardDef, PWORLD_WAVE_MON_CLAN_AWARD_DEF_DATA, MAX_PWORLD_WAVE_MON_CLAN_AWARD_DEF, &iReload))
		{
			return -1;
		}		

		if (0 > z_pre_pworld_wave_mon_clan_award_def(pstEnv))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "SubJectDef", pstEnv->pstZoneObj->sSubJectDef[0], &pstEnv->pstZoneObj->iSubJectDef, 
								 &pstEnv->pstZoneObj->tSubJectDef, SUBJECT_DEF_DATA, MAX_SUBJECT_DEF, &iReload))
		{
			return -1;
		}	

		if (subject_db_pre(pstEnv) < 0)
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "SubJectTypeDef", pstEnv->pstZoneObj->sSubJectTypeDef[0], &pstEnv->pstZoneObj->iSubJectTypeDef, 
								 &pstEnv->pstZoneObj->tSubJectTypeDef, SUBJECT_TYPE_DEF_DATA, MAX_SUBJECT_TYPE_DEF, &iReload))
		{
			return -1;
		}

		if (subject_type_db_pre(pstEnv) < 0)
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "GayDef", pstEnv->pstZoneObj->sGayDef[0], &pstEnv->pstZoneObj->iGayDef, 
								 &pstEnv->pstZoneObj->tGayDef, GAY_DEF_DATA, MAX_GAY_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "RangDsnDef", pstEnv->pstZoneObj->sRangDsnDef[0], &pstEnv->pstZoneObj->iRangDsnDef, 
								 &pstEnv->pstZoneObj->tRangDsnDef, RANGDSN_DEF_DATA, MAX_RANGDSN_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "PworldMachineBuffShopDef", pstEnv->pstZoneObj->sPworldMachineBuffShopDef[0], &pstEnv->pstZoneObj->iPworldMachineBuffShopDef, 
								 &pstEnv->pstZoneObj->tPworldMachineBuffShopDef, PWORLD_MACHINE_BUFF_SHOP_DEF_DATA, MAX_PWORLD_MACHINE_BUFF_SHOP_DEF, &iReload))
		{
			return -1;
		}		

		if (0 > z_pre_pworld_machine_buff_shop(pstEnv))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "PlayerAwardDef", pstEnv->pstZoneObj->sPlayerAwardDef[0], &pstEnv->pstZoneObj->iPlayerAwardDef, 
								 &pstEnv->pstZoneObj->tPlayerAwardDef, PLAYER_AWARD_DEF_DATA, MAX_PLAYER_AWARD_DEF, &iReload))
		{
			return -1;
		}		

		if (0 > z_pre_player_award(pstEnv))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "CowFinalAwardDef", pstEnv->pstZoneObj->sCowFinalAwardDef[0], &pstEnv->pstZoneObj->iCowFinalAwardDef, 
								 &pstEnv->pstZoneObj->tCowFinalAwardDef, COW_FINAL_AWARD_DEF_DATA, MAX_COW_FINAL_AWARD_DEF, &iReload))
		{
			return -1;
		}		

		z_pre_cow_final_award(pstEnv);

		if (0 > load_resfile_one(RES_DIR, "GmTrade", pstEnv->pstZoneObj->sGmTrade[0], &pstEnv->pstZoneObj->iGmTrade, 
								 &pstEnv->pstZoneObj->tGmTrade, GM_TRADE_DATA, MAX_GM_TRADE, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "StrongPointDef", pstEnv->pstZoneObj->sStrongPointDef[0], &pstEnv->pstZoneObj->iStrongPointDef, 
								 &pstEnv->pstZoneObj->tStrongPointDef, STRONG_POINT_DEF_DATA, MAX_STRONG_POINT_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ClanSkillDef", pstEnv->pstZoneObj->sClanSkillDef[0], &pstEnv->pstZoneObj->iClanSkillDef, 
								 &pstEnv->pstZoneObj->tClanSkillDef, CLAN_SKILL_DEF_DATA, MAX_CLAN_SKILL_DEF, &iReload))
		{
			return -1;
		}		

		z_pre_clan_skill(pstEnv);


		if (0 > load_resfile_one(RES_DIR, "BannerDef", pstEnv->pstZoneObj->sBannerDef[0], &pstEnv->pstZoneObj->iBannerDef, 
								 &pstEnv->pstZoneObj->tBannerDef, BANNER_DEF_DATA, MAX_BANNER_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ReviveDef", pstEnv->pstZoneObj->sReviveDef[0], &pstEnv->pstZoneObj->iReviveDef, 
								 &pstEnv->pstZoneObj->tReviveDef, REVIVE_DEF_DATA, MAX_REVIVE_DEF, &iReload))
		{
			return -1;

		}

		if (0 > load_resfile_one(RES_DIR, "ClanPayDef", pstEnv->pstZoneObj->sClanPayDef[0], &pstEnv->pstZoneObj->iClanPayDef, 
								 &pstEnv->pstZoneObj->tClanPayDef, CLAN_PAY_DEF_DATA, MAX_CLAN_PAY_DEF, &iReload))
		{
			return -1;
		}		

		z_pre_clan_pay(pstEnv);

		if (0 > load_resfile_one(RES_DIR, "LevelExpDef", pstEnv->pstZoneObj->sLevelExpDef[0], &pstEnv->pstZoneObj->iLevelExpDef, 
								 &pstEnv->pstZoneObj->tLevelExpDef, LEVEL_EXP_DEF_DATA, MAX_LEVEL_EXP_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "LevelBuffExpDef", pstEnv->pstZoneObj->sLevelBuffExpDef[0], &pstEnv->pstZoneObj->iLevelBuffExpDef, 
								 &pstEnv->pstZoneObj->tLevelBuffExpDef, LEVEL_BUFF_EXP_DEF_DATA, MAX_LEVEL_BUFF_EXP_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "RidePinZhiDef", pstEnv->pstZoneObj->sRidePinZhiDef[0], &pstEnv->pstZoneObj->iRidePinZhiDef, 
								 &pstEnv->pstZoneObj->tRidePinZhiDef, RIDE_PINZHI_DEF_DATA, MAX_RIDE_PINZHI_DEF, &iReload))
		{
			return -1;
		}
		
		if (0 > load_resfile_one(RES_DIR, "MapAwardOreDef", pstEnv->pstZoneObj->sMapAwardOreDef[0], &pstEnv->pstZoneObj->iMapAwardOreDef, 
								 &pstEnv->pstZoneObj->tMapAwardOreDef, MAP_AWARD_ORE_DEF_DATA, MAX_MAP_AWARD_ORE_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "RongluDef", pstEnv->pstZoneObj->sRongluDef[0], &pstEnv->pstZoneObj->iRongluDef, 
								 &pstEnv->pstZoneObj->tRongluDef, RONGLU_DEF_DATA, MAX_RONGLU_DEF, &iReload))
		{
			return -1;
		}

		if (ronglu_check_pre( pstEnv) < 0)
		{
			return -1;
		}
		
		if (0 > load_resfile_one(RES_DIR, "PetLoveLevelDef", pstEnv->pstZoneObj->sPetLoveLevelDef[0], &pstEnv->pstZoneObj->iPetLoveLevelDef, 
								 &pstEnv->pstZoneObj->tPetLoveLevelDef, PET_LOVE_LEVEL_DEF_DATA, MAX_PET_LOVE_LEVEL_DEF, &iReload))
		{
			return -1;
		}	
		z_pre_pet_love_level(pstEnv);

		if (0 > load_resfile_one(RES_DIR, "GeniusCareerDef", pstEnv->pstZoneObj->sGeniusCareerDef[0], &pstEnv->pstZoneObj->iGeniusCareerDef, 
								 &pstEnv->pstZoneObj->tGeniusCareerDef, GENIUS_CAREER_DEF_DATA, MAX_GENIUS_CAREER_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "GeniusAttrDef", pstEnv->pstZoneObj->sGeniusAttrDef[0], &pstEnv->pstZoneObj->iGeniusAttrDef, 
								 &pstEnv->pstZoneObj->tGeniusAttrDef, GENIUS_ATTR_DEF_DATA, MAX_GENIUS_ATTR_DEF, &iReload))
		{
			return -1;
		}	
		
		if (0 > load_resfile_one(RES_DIR, "BaoXiangDef", pstEnv->pstZoneObj->sBaoXiangDef[0], &pstEnv->pstZoneObj->iBaoXiangDef, 
								 &pstEnv->pstZoneObj->tBaoXiangDef, BAOXIANG_DEF_DATA, MAX_BAOXIANG_DEF, &iReload))
		{
			return -1;
		}	

		//更新一下，针对reloadres, reloadall
		global_baoxiang_update(pstEnv);

		if (0 > load_resfile_one(RES_DIR, "WorldCityFightAwardDef", pstEnv->pstZoneObj->sWorldCityFightAwardDef[0], &pstEnv->pstZoneObj->iWorldCityFightAwardDef, 
								 &pstEnv->pstZoneObj->tWorldCityFightAwardDef, WORLD_CITY_FIGHT_AWARD_DEF_DATA, MAX_WORLD_CITY_FIGHT_AWARD_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "LevelFeng", pstEnv->pstZoneObj->sLevelFengDef[0], &pstEnv->pstZoneObj->iLevelFengDef, 
								 &pstEnv->pstZoneObj->tLevelFengDef, LEVEL_FENG_DEF_DATA, MAX_LEVEL_FENG_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "ArmLevelUPDef", pstEnv->pstZoneObj->sArmLevelUPDef[0], &pstEnv->pstZoneObj->iArmLevelUPDef, 
								 &pstEnv->pstZoneObj->tArmLevelUPDef, ARM_LEVELUP_DEF_DATA, MAX_ARM_LEVELUP_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "WorldBossDef", pstEnv->pstZoneObj->sWorldBossDef[0], &pstEnv->pstZoneObj->iWorldBossDef, 
								 &pstEnv->pstZoneObj->tWorldBossDef,WORLD_BOSS_DEF_DATA, MAX_WORLD_BOSS_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "LuckyMapDef", pstEnv->pstZoneObj->sLuckyMapDef[0], &pstEnv->pstZoneObj->iLuckyMapDef, 
								 &pstEnv->pstZoneObj->tLuckyMapDef, LUCKY_MAP_DEF_DATA, MAX_LUCKY_MAP_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "BusyDef", pstEnv->pstZoneObj->sBusyDef[0], &pstEnv->pstZoneObj->iBusyDef, 
								 &pstEnv->pstZoneObj->tBusyDef, BUSY_DEF_DATA, MAX_BUSY_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "BusyUseDef", pstEnv->pstZoneObj->sBusyUseDef[0], &pstEnv->pstZoneObj->iBusyUseDef, 
								 &pstEnv->pstZoneObj->tBusyUseDef, BUSY_USE_DEF_DATA, MAX_BUSY_USE_DEF, &iReload))
		{
			return -1;
		}		

		if (0 > load_resfile_one(RES_DIR, "CareerHurtDef", pstEnv->pstZoneObj->sCareerHurtDef[0], &pstEnv->pstZoneObj->iCareerHurtDef, 
								 &pstEnv->pstZoneObj->tCareerHurtDef, CAREER_HURT_DEF_DATA, MAX_CAREER_HURT_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "GeniusAddDef", pstEnv->pstZoneObj->sGeniusAddDef[0], &pstEnv->pstZoneObj->iGeniusAddDef, 
								 &pstEnv->pstZoneObj->tGeniusAddDef, GENIUS_ADD_DEF_DATA, MAX_GENIUS_ADD_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "HighWashUseDef", pstEnv->pstZoneObj->sHighWashUseDef[0], &pstEnv->pstZoneObj->iHighWashUseDef, 
								 &pstEnv->pstZoneObj->tHighWashUseDef, HIGHWASH_USE_DEF_DATA, MAX_HIGHWASH_USE_DEF, &iReload))
		{
			return -1;
		}

		z_pre_lucky_map(pstEnv);

		if (0 > load_resfile_one(RES_DIR, "PetGeniusDef", pstEnv->pstZoneObj->sPetGeniusDef[0], &pstEnv->pstZoneObj->iPetGeniusDef, 
								 &pstEnv->pstZoneObj->tPetGeniusDef, PET_GENIUS_DEF_DATA, MAX_PET_GENIUS_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "WeDef", pstEnv->pstZoneObj->sWeDef[0], &pstEnv->pstZoneObj->iWeDef, 
								 &pstEnv->pstZoneObj->tWeDef, WE_DEF_DATA, MAX_WE_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ExchgItemDef", pstEnv->pstZoneObj->sExchgItemDef[0], &pstEnv->pstZoneObj->iExchgItemDef, 
								 &pstEnv->pstZoneObj->tExchgItemDef, EXCHG_ITEM_DEF_DATA, MAX_EXCHG_ITEM_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "SpartaAwardDef", pstEnv->pstZoneObj->sSpartaAwardDef[0], &pstEnv->pstZoneObj->iSpartaAwardDef, 
								 &pstEnv->pstZoneObj->tSpartaAwardDef, SPARTA_AWARD_DEF_DATA, MAX_SPARTA_AWARD_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "SpartaLevelDef", pstEnv->pstZoneObj->sSpartaLevelDef[0], &pstEnv->pstZoneObj->iSpartaLevelDef, 
								 &pstEnv->pstZoneObj->tSpartaLevelDef, SPARTA_LEVEL_DEF_DATA, MAX_SPARTA_LEVEL_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "MapRandTransmit", pstEnv->pstZoneObj->sMapRandTransmitDef[0], &pstEnv->pstZoneObj->iMapRandTransmitDef, 
								 &pstEnv->pstZoneObj->tMapRandTransmitDef, MAP_RAND_TRANS_DATA, MAX_MAP_RAND_TRANS_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "SchoolLevelDef", pstEnv->pstZoneObj->sSchoolLevelDef[0], &pstEnv->pstZoneObj->iSchoolLevelDef, 
								 &pstEnv->pstZoneObj->tSchoolLevelDef, SCHOOL_LEVEL_DEF_DATA, MAX_SCHOOL_LEVEL_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "SchoolPworldDef", pstEnv->pstZoneObj->sSchoolPworldDef[0], &pstEnv->pstZoneObj->iSchoolPworldDef, 
								 &pstEnv->pstZoneObj->tSchoolPworldDef, SCHOOL_PWORLD_DEF_DATA, MAX_SCHOOL_PWORLD_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "SchoolXinDeDef", pstEnv->pstZoneObj->sSchoolXinDeDef[0], &pstEnv->pstZoneObj->iSchoolXinDeDef, 
								 &pstEnv->pstZoneObj->tSchoolXinDeDef, SCHOOL_XINDE_DEF_DATA, MAX_SCHOOL_XINDE_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "BackLevelDef", pstEnv->pstZoneObj->sBackLevelDef[0], &pstEnv->pstZoneObj->iBackLevelDef, 
								 &pstEnv->pstZoneObj->tBackLevelDef, BACK_LEVEL_DEF_DATA, MAX_BACK_LEVEL_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "XuKongPosDef", pstEnv->pstZoneObj->sXuKongPosDef[0], &pstEnv->pstZoneObj->iXuKongPosDef, 
								 &pstEnv->pstZoneObj->tXuKongPosDef, XUKONG_POS_DEF_DATA, MAX_XUKONG_POS_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "NewRoleGift", pstEnv->pstZoneObj->sNewRoleGiftDef[0], &pstEnv->pstZoneObj->iNewRoleGiftDef, 
								 &pstEnv->pstZoneObj->tNewRoleGiftDef, NEW_ROLE_GIFT_DEF_SIZE, MAX_NEW_ROLE_GIFT_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ContCoverGift", pstEnv->pstZoneObj->sContCoverGiftDef[0], &pstEnv->pstZoneObj->iContCoverGiftDef, 
								 &pstEnv->pstZoneObj->tContCoverGiftDef, CONT_COVER_DEF_SIZE, MAX_CONT_COVER_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "CardBoxDef", pstEnv->pstZoneObj->sCardBoxDef[0], &pstEnv->pstZoneObj->iCardBoxDef, 
								 &pstEnv->pstZoneObj->tCardBoxDef, CARD_BOX_DEF_DATA, MAX_CARD_BOX_DEF, &iReload))
		{
			return -1;
		}

		global_cardbox_update(pstEnv);

		if (0 > load_resfile_one(RES_DIR, "CardBoxGroupDef", pstEnv->pstZoneObj->sCardBoxGroupDef[0], &pstEnv->pstZoneObj->iCardBoxGroupDef, 
								 &pstEnv->pstZoneObj->tCardBoxGroupDef, CARD_BOX_GROUP_DEF_DATA, MAX_CARD_BOX_GROUP_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ArmBuChangDef", pstEnv->pstZoneObj->sArmBuChangDef[0], &pstEnv->pstZoneObj->iArmBuChangDef, 
								 &pstEnv->pstZoneObj->tArmBuChangDef, ARM_BUCHANG_DEF_DATA, MAX_ARM_BUCHANG_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "BuChangDef", pstEnv->pstZoneObj->sBuChangDef[0], &pstEnv->pstZoneObj->iBuChangDef, 
								 &pstEnv->pstZoneObj->tBuChangDef, BUCHANG_DEF_DATA, MAX_BUCHANG_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "PworldBoxDef", pstEnv->pstZoneObj->sPworldBoxDef[0], &pstEnv->pstZoneObj->iPworldBoxDef, 
								 &pstEnv->pstZoneObj->tPworldBoxDef, PWORLD_BOX_DEF_DATA, MAX_PWORLD_BOX_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "YangHaiBangDef", pstEnv->pstZoneObj->sYangHaiBangDef[0], &pstEnv->pstZoneObj->iYangHaiBangDef, 
								 &pstEnv->pstZoneObj->tYangHaiBangDef, YANGHAIBANG_DEF_DATA, MAX_YANGHAIBANG_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "HangHaiAwardDef", pstEnv->pstZoneObj->sHangHaiAwardDef[0], &pstEnv->pstZoneObj->iHangHaiAwardDef, 
								 &pstEnv->pstZoneObj->tHangHaiAwardDef, HANG_HAI_AWARD_DEF_DATA, MAX_HANG_HAI_AWARD_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "StoreGridDef", pstEnv->pstZoneObj->sStoreGridDef[0], &pstEnv->pstZoneObj->iStoreGridDef, 
								 &pstEnv->pstZoneObj->tStoreGridDef, STORE_GRID_DEF_DATA, MAX_STORE_GRID_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "ArmStepUpDef", pstEnv->pstZoneObj->sArmStepUpDef[0], &pstEnv->pstZoneObj->iArmStepUpDef, 
								 &pstEnv->pstZoneObj->tArmStepUpDef, ARM_STEP_UP_DEF_DATA, MAX_ARM_STEP_UP_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "ArmReSetDef", pstEnv->pstZoneObj->sArmReSetDef[0], &pstEnv->pstZoneObj->iArmReSetDef, 
								 &pstEnv->pstZoneObj->tArmReSetDef, ARM_RESET_DEF_DATA, MAX_ARM_RESET_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "ArmGradeValDef", pstEnv->pstZoneObj->sArmGradeValDef[0], &pstEnv->pstZoneObj->iArmGradeValDef, 
								 &pstEnv->pstZoneObj->tArmGradeValDef, ARM_VAL_DEF_DATA, MAX_ARM_VAL_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "RongYanJuShouDef", pstEnv->pstZoneObj->sRongYanJuShouDef[0], &pstEnv->pstZoneObj->iRongYanJuShouDef, 
								 &pstEnv->pstZoneObj->tRongYanJuShouDef, RONGYANJUSHOU_DEF_DATA, MAX_RONGYANJUSHOU_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "RongYanMonDynAttrDef", pstEnv->pstZoneObj->sRongYanMonDynAttrDef[0], &pstEnv->pstZoneObj->iRongYanMonDynAttrDef, 
								 &pstEnv->pstZoneObj->tRongYanMonDynAttrDef, RONGYANMONDYNATTR_DEF_DATA, MAX_RONGYANMONDYNATTR_DEF, &iReload))
		{
			return -1;
		}	
		

		if (0 > load_resfile_one(RES_DIR, "ClanAwardDef", pstEnv->pstZoneObj->sClanAwardDef[0], &pstEnv->pstZoneObj->iClanAwardDef, 
								 &pstEnv->pstZoneObj->tClanAwardDef, CLAN_AWARD_DEF_DATA, MAX_CLAN_AWARD_DEF, &iReload))
		{
			return -1;
		}		

		z_pre_clan_award(pstEnv);

		if (0 > load_resfile_one(RES_DIR, "ClanWarAwardDef", pstEnv->pstZoneObj->sClanWarAwardDef[0], &pstEnv->pstZoneObj->iClanWarAwardDef, 
								 &pstEnv->pstZoneObj->tClanWarAwardDef, CLAN_WAR_AWARD_DEF_DATA, MAX_CLAN_WAR_AWARD_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "ArmSkillNumDef", pstEnv->pstZoneObj->sArmSkillNumDef[0], &pstEnv->pstZoneObj->iArmSkillNumDef, 
								 &pstEnv->pstZoneObj->tArmSkillNumDef, ARM_SKILL_NUM_DEF_DATA, MAX_ARM_SKILL_NUM_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "ArmSkillAttrDef", pstEnv->pstZoneObj->sArmSkillAttrDef[0], &pstEnv->pstZoneObj->iArmSkillAttrDef, 
								 &pstEnv->pstZoneObj->tArmSkillAttrDef, ARM_SKILL_ATTR_DEF_DATA, MAX_ARM_SKILL_ATTR_DEF, &iReload))
		{
			return -1;
		}	

		if (arm_skill_fast_init(pstEnv) < 0)
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "GeniusMdfDef", pstEnv->pstZoneObj->sGeniusMdfDef[0], &pstEnv->pstZoneObj->iGeniusMdfDef, 
								 &pstEnv->pstZoneObj->tGeniusMdfDef, GENIUS_MDF_DEF_DATA, MAX_GENIUS_MDF_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "PaoShangDef", pstEnv->pstZoneObj->sPaoShangDef[0], &pstEnv->pstZoneObj->iPaoShangDef, 
								 &pstEnv->pstZoneObj->tPaoShangDef, PAOSHANG_DEF_DATA, MAX_PAOSHANG_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "PaoShangItemDef", pstEnv->pstZoneObj->sPaoShangItemDef[0], &pstEnv->pstZoneObj->iPaoShangItemDef, 
								 &pstEnv->pstZoneObj->tPaoShangItemDef, PAOSHANG_ITEM_DEF_DATA, MAX_PAOSHANG_ITEM_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "PetMoneyDef", pstEnv->pstZoneObj->sPetMoneyDef[0], &pstEnv->pstZoneObj->iPetMoneyDef, 
								 &pstEnv->pstZoneObj->tPetMoneyDef, PET_MONEY_DEF_DATA, MAX_PET_MONEY_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "PetImpUpAttrDef", pstEnv->pstZoneObj->sPetImpUpAttrDef[0], &pstEnv->pstZoneObj->iPetImpUpAttrDef, 
								 &pstEnv->pstZoneObj->tPetImpUpAttrDef, PET_IMP_UP_ATTR_DEF_DATA, MAX_PET_IMP_UP_ATTR_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "PetImpUpDef", pstEnv->pstZoneObj->sPetImpUpDef[0], &pstEnv->pstZoneObj->iPetImpUpDef, 
								 &pstEnv->pstZoneObj->tPetImpUpDef, PET_IMP_UP_DEF_DATA, MAX_PET_IMP_UP_DEF, &iReload))
		{
			return -1;
		}	

		
		if (0 > load_resfile_one(RES_DIR, "GemPagDef", pstEnv->pstZoneObj->sGemPagDef[0], &pstEnv->pstZoneObj->iGemPagDef, 
									 &pstEnv->pstZoneObj->tGemPagDef, GEM_PAG_DEF_DATA, MAX_GEM_PAG_DEF, &iReload))
		{
			return -1;
		}
			

		if (0 > load_resfile_one(RES_DIR, "PetOpAwardDef", pstEnv->pstZoneObj->sPetOpAwardDef[0], &pstEnv->pstZoneObj->iPetOpAwardDef, 
								 &pstEnv->pstZoneObj->tPetOpAwardDef, PET_OP_AWARD_DEF_DATA, MAX_PET_OP_AWARD_DEF, &iReload))
		{
			return -1;
		}	
		z_pre_clan_war_award(pstEnv);

		if (0 > load_resfile_one(RES_DIR, "ArmTeshuDef", pstEnv->pstZoneObj->sArmTeshuDef[0], &pstEnv->pstZoneObj->iArmTeshuDef, 
								 &pstEnv->pstZoneObj->tArmTeshuDef, ARM_TESHU_DEF_DATA, MAX_ARM_TESHU_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "ZJRongYanJuShouDef", pstEnv->pstZoneObj->sZJRongYanJuShouDef[0], &pstEnv->pstZoneObj->iZJRongYanJuShouDef, 
								 &pstEnv->pstZoneObj->tZJRongYanJuShouDef, ZJ_RONGYANJUSHOU_DEF_DATA, MAX_ZJ_RONGYANJUSHOU_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "SpanStrongDef", pstEnv->pstZoneObj->sSpanStrongDef[0], &pstEnv->pstZoneObj->iSpanStrongDef, 
								 &pstEnv->pstZoneObj->tSpanStrongDef, SPAN_STRONG_DEF_SIZE, MAX_SPAN_STRONG_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "WordPwdDef", pstEnv->pstZoneObj->sWordPwdDef[0], &pstEnv->pstZoneObj->iWordPwdDef, 
								 &pstEnv->pstZoneObj->tWordPwdDef, WORD_PWD_DEF_DATA, MAX_WORD_PWD_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "HomeLvlDef", pstEnv->pstZoneObj->sHomeLvlDef[0], &pstEnv->pstZoneObj->iHomeLvlDef, 
								 &pstEnv->pstZoneObj->tHomeLvlDef, HOME_LVL_DEF_DATA, MAX_HOME_LVL_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "HomeMonLvlDef", pstEnv->pstZoneObj->sHomeMonLvlDef[0], &pstEnv->pstZoneObj->iHomeMonLvlDef, 
								 &pstEnv->pstZoneObj->tHomeMonLvlDef, HOME_MON_LVL_DEF_DATA, MAX_HOME_MON_LVL_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "HomeMonSegDef", pstEnv->pstZoneObj->sHomeMonSegDef[0], &pstEnv->pstZoneObj->iHomeMonSegDef, 
								 &pstEnv->pstZoneObj->tHomeMonSegDef, HOME_MON_SEG_DEF_DATA, MAX_HOME_MON_SEG_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "HomeFarmDef", pstEnv->pstZoneObj->sHomeFarmDef[0], &pstEnv->pstZoneObj->iHomeFarmDef, 
								 &pstEnv->pstZoneObj->tHomeFarmDef, HOME_FARM_DEF_DATA, MAX_HOME_FARM_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ArmSkillRecastDef", pstEnv->pstZoneObj->sArmSkillRecastDef[0], &pstEnv->pstZoneObj->iArmSkillRecastDef, 
								 &pstEnv->pstZoneObj->tArmSkillRecastDef, SKILL_ARM_RECAST_DEF_DATA, MAX_SKILL_ARM_RECAST_DEF, &iReload))
		{
			return -1;
		}	

		if (0 > load_resfile_one(RES_DIR, "HomeSoilDef", pstEnv->pstZoneObj->sHomeSoilDef[0], &pstEnv->pstZoneObj->iHomeSoilDef, 
								 &pstEnv->pstZoneObj->tHomeSoilDef, HOME_SOIL_DEF_DATA, MAX_HOME_SOIL_DEF, &iReload))
		{
			return -1;
		}
	}
	else
	{
		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "MonsterDef", pstEnv->pstZoneObj->sMonsterDef[0], &pstEnv->pstZoneObj->iMonsterDef, MONSTER_DEF_DATA, MAX_MONSTER_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iMonsterDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sMonsterDef[0], pstEnv->pstZoneObj->iMonsterDef, MONSTER_DEF_DATA, trl_comp_func);
		}
		if (0 > z_pre_monster(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "MonsterStateSkillDef", pstEnv->pstZoneObj->sMonsterStateSkillDef[0],
				&pstEnv->pstZoneObj->iMonsterStateSkillDef, MONSTERSTATESKILL_DEF_DATA, MAX_MONSTERSTATESKILL_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iMonsterStateSkillDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sMonsterStateSkillDef[0], pstEnv->pstZoneObj->iMonsterStateSkillDef, MONSTERSTATESKILL_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "AddAttrDef", pstEnv->pstZoneObj->sAddAttrDef[0], &pstEnv->pstZoneObj->iAddAttrDef, ADDATTR_DEF_DATA, MAX_ADDATTR_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iAddAttrDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sAddAttrDef[0], pstEnv->pstZoneObj->iAddAttrDef, ADDATTR_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "SuitSet", pstEnv->pstZoneObj->sSuitSetDef[0], &pstEnv->pstZoneObj->iSuitSetDef, SUITSET_DEF_DATA, MAX_SUITSET_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iSuitSetDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sSuitSetDef[0], pstEnv->pstZoneObj->iSuitSetDef, SUITSET_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ItemDef", pstEnv->pstZoneObj->sItemDef[0], &pstEnv->pstZoneObj->iItemDef, ITEM_DEF_DATA, MAX_ITEM_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iItemDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sItemDef[0], pstEnv->pstZoneObj->iItemDef, ITEM_DEF_DATA, trl_comp_func);
		}
		if (0 > pre_item(pstEnv->pstZoneObj->sItemDef[0], pstEnv->pstZoneObj->iItemDef, ITEM_DEF_DATA))
		{
			return -1;
		}
		if (0 > check_item(pstEnv->pstZoneObj->sItemDef[0], pstEnv->pstZoneObj->iItemDef,ITEM_DEF_DATA,
							pstEnv->pstZoneObj->sSuitSetDef[0], pstEnv->pstZoneObj->iSuitSetDef, SUITSET_DEF_DATA))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "NpcSellList", pstEnv->pstZoneObj->sSellDef[0], &pstEnv->pstZoneObj->iSellDef, SELL_DEF_DATA, MAX_SELL_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iSellDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sSellDef[0], pstEnv->pstZoneObj->iSellDef, SELL_DEF_DATA, trl_comp_func);
		} 

		if (0 > z_check_npcshop(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "MapTransDef", pstEnv->pstZoneObj->sMapTransDef[0], &pstEnv->pstZoneObj->iMapTransDef, MAP_TRANS_DEF_DATA, MAX_MAP_TRANS_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iMapTransDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sMapTransDef[0], pstEnv->pstZoneObj->iMapTransDef, MAP_TRANS_DEF_DATA, trl_comp_func);
		} 

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "BirthMapDef", pstEnv->pstZoneObj->sBirthMapDef[0], &pstEnv->pstZoneObj->iBirthMapDef, 
								BIRTHMAP_DEF_DATA, MAX_BIRTHMAP_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iBirthMapDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sBirthMapDef[0], pstEnv->pstZoneObj->iBirthMapDef, BIRTHMAP_DEF_DATA, trl_comp_func);
		} 

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ActionStatus", pstEnv->pstZoneObj->sActionStatusDef[0], &pstEnv->pstZoneObj->iActionStatusDef, 
								ACTION_STATUS_DEF_DATA, RES_STATUS_MAX))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iActionStatusDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sActionStatusDef[0], pstEnv->pstZoneObj->iActionStatusDef, ACTION_STATUS_DEF_DATA, trl_comp_func);
		} 

		if (0 > z_act_status_init(pstAppCtx, pstEnv))
		{
			printf("init act_status fail\n");
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "LevelUP", pstEnv->pstZoneObj->sLevelUpDef[0], &pstEnv->pstZoneObj->iLevelUpDef, 
								LEVELUP_DEF_DATA, MAX_LEVEL))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iLevelUpDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sLevelUpDef[0], pstEnv->pstZoneObj->iLevelUpDef, LEVELUP_DEF_DATA, trl_comp_func);
		} 

		if (0 > z_pre_levelup(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ArmLevelHurtDef", pstEnv->pstZoneObj->sArmLevelHurtDef[0], &pstEnv->pstZoneObj->iArmLevelHurtDef, 
								    ARM_LEVEL_HURT_DATA, MAX_ARM_LEVEL_HURT))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iArmLevelHurtDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sArmLevelHurtDef[0], pstEnv->pstZoneObj->iArmLevelHurtDef, ARM_LEVEL_HURT_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "SkillDef", pstEnv->pstZoneObj->sSkillDef[0], &pstEnv->pstZoneObj->iSkillDef, 
								    SKILL_DATA, MAX_SKILL))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iSkillDef > 1) 
		{
			qsort(pstEnv->pstZoneObj->sSkillDef[0], pstEnv->pstZoneObj->iSkillDef, SKILL_DATA, trl_comp_func);
		}
		if (0 > check_skilldef(pstEnv->pstZoneObj->sSkillDef[0], pstEnv->pstZoneObj->iSkillDef, SKILL_DATA))
		{
			return -1;
		}
		if (0 > z_pre_skill(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "DiathesisDef", pstEnv->pstZoneObj->sDiathesisDef[0], &pstEnv->pstZoneObj->iDiathesisDef, 
								     DIATHESIS_DATA, MAX_DIATHESIS_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iDiathesisDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sDiathesisDef[0], pstEnv->pstZoneObj->iDiathesisDef, DIATHESIS_DATA, trl_comp_func);
		}
		if (0 > check_diathesis(pstEnv->pstZoneObj->sDiathesisDef[0], pstEnv->pstZoneObj->iDiathesisDef, DIATHESIS_DATA))
		{
			return -1;
		}

		if ( z_check_pet_rand_skill(pstEnv) < 0)
		{
			return -1;
		}	

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "FittingsDef", pstEnv->pstZoneObj->sFittingsDef[0], &pstEnv->pstZoneObj->iFittingsDef, 
								    FITTINGS_DATA, MAX_SYS_FITTINGS))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iFittingsDef > 1) 
		{
			qsort(pstEnv->pstZoneObj->sFittingsDef[0], pstEnv->pstZoneObj->iFittingsDef, FITTINGS_DATA, trl_comp_func);
		}
		if (0 > check_fittings(pstEnv->pstZoneObj->sFittingsDef[0], pstEnv->pstZoneObj->iFittingsDef, FITTINGS_DATA))
		{
			return -1;
		}
		if( 0 > check_fittings_pick(pstEnv) )
		{
			return -1;
		}


		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "StatusDef", pstEnv->pstZoneObj->sStatusDef[0], &pstEnv->pstZoneObj->iStatusDef, 
								    STATUS_DATA, MAX_STATUS))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iStatusDef > 1) 
		{
			qsort(pstEnv->pstZoneObj->sStatusDef[0], pstEnv->pstZoneObj->iStatusDef, STATUS_DATA, trl_comp_func);
		}
		if (0 > z_pre_status(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "OverlayTypeRelationDef", pstEnv->pstZoneObj->sOverlayTypeRelationDef[0], &pstEnv->pstZoneObj->iOverlayTypeRelationDef, 
								    OVERLAYTYPE_RELATION_DATA, MAX_OVERLAYTYPE_RELATION))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iOverlayTypeRelationDef > 1) 
		{
			qsort(pstEnv->pstZoneObj->sOverlayTypeRelationDef[0], pstEnv->pstZoneObj->iOverlayTypeRelationDef, OVERLAYTYPE_RELATION_DATA, trl_comp_func);
		}

		if (0 > z_check_overlaytype(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "StatusOverlayDef", pstEnv->pstZoneObj->sStatusOverlayDef[0], &pstEnv->pstZoneObj->iStatusOverlayDef, 
								    STATUS_OVERLAY_DATA, MAX_STATUS_OVERLAY))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iStatusOverlayDef > 1) 
		{
			qsort(pstEnv->pstZoneObj->sStatusOverlayDef[0], pstEnv->pstZoneObj->iStatusOverlayDef, STATUS_OVERLAY_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "MonDynAttrDef", pstEnv->pstZoneObj->sMonDynAttrDef[0], &pstEnv->pstZoneObj->iMonDynAttrDef, 
								     MON_DYN_ATTR_DATA, MAX_LEVEL))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iMonDynAttrDef > 1) 
		{
			qsort(pstEnv->pstZoneObj->sMonDynAttrDef[0], pstEnv->pstZoneObj->iMonDynAttrDef, MON_DYN_ATTR_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PatrolDef", pstEnv->pstZoneObj->sPatrolDef[0], &pstEnv->pstZoneObj->iPatrolDef, 
								     PATROL_DATA, MAX_PATROL))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iPatrolDef > 1) 
		{
			qsort(pstEnv->pstZoneObj->sPatrolDef[0], pstEnv->pstZoneObj->iPatrolDef, PATROL_DATA, trl_comp_func);
		}
		if (0 > check_patrol(pstEnv->pstZoneObj->sPatrolDef[0], pstEnv->pstZoneObj->iPatrolDef, PATROL_DATA))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "TaskDef", pstEnv->pstZoneObj->sTaskDef[0], &pstEnv->pstZoneObj->iTaskDef, 
								   TASK_DATA, MAX_TASK))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iTaskDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sTaskDef[0], pstEnv->pstZoneObj->iTaskDef, TASK_DATA, trl_comp_func);
		}
		if (0 > z_pre_task(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "TaskRecycleDef", pstEnv->pstZoneObj->sTaskCycleDef[0], &pstEnv->pstZoneObj->iTaskCycleDef, 
								    TASK_CYCLE_DATA, MAX_TASK_CYCLE))
		{
			return -1;
		}	
		if (pstEnv->pstZoneObj->iTaskCycleDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sTaskCycleDef[0], pstEnv->pstZoneObj->iTaskCycleDef, TASK_CYCLE_DATA, trl_comp_func);
		}
		if (0 > z_pre_taskcycle(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "NpcInfoDef", pstEnv->pstZoneObj->sNpcInfoDef[0], &pstEnv->pstZoneObj->iNpcInfoDef, 
								    NPC_INFO_DATA, MAX_NPC_INFO))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iNpcInfoDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sNpcInfoDef[0], pstEnv->pstZoneObj->iNpcInfoDef, NPC_INFO_DATA, trl_comp_func);
		}

		
		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PetDef", pstEnv->pstZoneObj->sPetDef[0], &pstEnv->pstZoneObj->iPetDef, 
								    PET_DATA, MAX_PET))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iPetDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sPetDef[0], pstEnv->pstZoneObj->iPetDef, PET_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PetLevelDef", pstEnv->pstZoneObj->sPetLevelDef[0], &pstEnv->pstZoneObj->iPetLevelDef, 
								    PETLEVEL_DATA, MAX_PETLEVEL))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iPetLevelDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sPetLevelDef[0], pstEnv->pstZoneObj->iPetLevelDef, PETLEVEL_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PetTraitDef", pstEnv->pstZoneObj->sPetTraitDef[0], &pstEnv->pstZoneObj->iPetTraitDef, 
								    PET_TRAIT_DEF_DATA, MAX_PET_TRAIT_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iPetTraitDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sPetTraitDef[0], pstEnv->pstZoneObj->iPetTraitDef, PET_TRAIT_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PetStrengthenDef", pstEnv->pstZoneObj->sPetStrengthenDef[0], &pstEnv->pstZoneObj->iPetStrengthenDef, 
								    PET_STRENGTHEN_DEF_DATA, MAX_PET_STRENGTHEN_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iPetStrengthenDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sPetStrengthenDef[0], pstEnv->pstZoneObj->iPetStrengthenDef, PET_STRENGTHEN_DEF_DATA, trl_comp_func);
		}

		if (0 > z_pre_pet(pstEnv))
		{
			return -1;
		}
		
		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "MonDynExpLimitDef", pstEnv->pstZoneObj->sMonDynExpLimitDef[0], &pstEnv->pstZoneObj->iMonDynExpLimitDef, 
								    MONDYNEXPLIMIT_DATA, MAX_MONDYNEXPLIMIT))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iMonDynExpLimitDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sMonDynExpLimitDef[0], pstEnv->pstZoneObj->iMonDynExpLimitDef, MONDYNEXPLIMIT_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "QiYueDef", pstEnv->pstZoneObj->sQiYueDef[0], &pstEnv->pstZoneObj->iQiYueDef, 
								    QIYUE_DEF_DATA, MAX_QIYUE))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iQiYueDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sQiYueDef[0], pstEnv->pstZoneObj->iQiYueDef, QIYUE_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ArmGradeDef", pstEnv->pstZoneObj->sArmGradeDef[0], &pstEnv->pstZoneObj->iArmGradeDef, 
								    ARMGRADE_DATA, MAX_ARMGRADE_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iArmGradeDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sArmGradeDef[0], pstEnv->pstZoneObj->iArmGradeDef, ARMGRADE_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ArmRandDef", pstEnv->pstZoneObj->sArmRandDef[0], &pstEnv->pstZoneObj->iArmRandDef, 
								    ARMRAND_DATA, MAX_ARMRAND_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iArmRandDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sArmRandDef[0], pstEnv->pstZoneObj->iArmRandDef, ARMRAND_DATA, trl_comp_func);
		}


		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ArmHideAttrDef", pstEnv->pstZoneObj->sArmHideAttrDef[0], &pstEnv->pstZoneObj->iArmHideAttrDef, 
								    ARMHIDEATTR_DATA, MAX_ARMHIDEATTR_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iArmHideAttrDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sArmHideAttrDef[0], pstEnv->pstZoneObj->iArmHideAttrDef, ARMHIDEATTR_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "BirthItemDef", pstEnv->pstZoneObj->sBirthItemDef[0], &pstEnv->pstZoneObj->iBirthItemDef, 
								    BIRTH_ITEM_DEF_DATA, MAX_BIRTH_ITEM_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iBirthItemDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sBirthItemDef[0], pstEnv->pstZoneObj->iBirthItemDef, BIRTH_ITEM_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "MonsterSpeechDef", pstEnv->pstZoneObj->sMonsterSpeechDef[0], &pstEnv->pstZoneObj->iMonsterSpeechDef, 
								    MONSTER_SPEECH_DEF_DATA, MAX_MONSTER_SPEECH_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iMonsterSpeechDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sMonsterSpeechDef[0], pstEnv->pstZoneObj->iMonsterSpeechDef, MONSTER_SPEECH_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ArmAddAttrDef", pstEnv->pstZoneObj->sArmAddAttrDef[0], &pstEnv->pstZoneObj->iArmAddAttrDef, 
								    ARM_ADDATTR_DEF_DATA, MAX_ARM_ADDATTR_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iArmAddAttrDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sArmAddAttrDef[0], pstEnv->pstZoneObj->iArmAddAttrDef, ARM_ADDATTR_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "OnlineAwardDef", pstEnv->pstZoneObj->sOnlineAwardDef[0], &pstEnv->pstZoneObj->iOnlineAwardDef, 
								    ONLINE_AWARD_DEF_DATA, MAX_ONLINE_AWARD_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iOnlineAwardDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sOnlineAwardDef[0], pstEnv->pstZoneObj->iOnlineAwardDef, ONLINE_AWARD_DEF_DATA, trl_comp_func);
		}

		if (z_pre_online_award(pstEnv) < 0)
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "TaskLimitGroup", pstEnv->pstZoneObj->sTaskLimitGroup[0], &pstEnv->pstZoneObj->iTaskLimitGroup, 
								    TASK_LIMIT_GROUP_DATA, MAX_TASK_LIMIT_GROUP))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iTaskLimitGroup> 1)
		{
			qsort(pstEnv->pstZoneObj->sTaskLimitGroup[0], pstEnv->pstZoneObj->iTaskLimitGroup, TASK_LIMIT_GROUP_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ClanCityBuildingLevelDef", pstEnv->pstZoneObj->sClanCityBuildingLevelDef[0], &pstEnv->pstZoneObj->iClanCityBuildingLevelDef, 
								           CLAN_CITY_BUILDING_LEVEL_DEF_DATA, MAX_CLAN_CITY_BUILDING_LEVEL_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iClanCityBuildingLevelDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sClanCityBuildingLevelDef[0], pstEnv->pstZoneObj->iClanCityBuildingLevelDef, CLAN_CITY_BUILDING_LEVEL_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ClanCityLevelDef", pstEnv->pstZoneObj->sClanCityLevelDef[0], &pstEnv->pstZoneObj->iClanCityLevelDef, 
								           CLAN_CITY_LEVEL_DEF_DATA, MAX_CLAN_CITY_LEVEL_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iClanCityLevelDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sClanCityLevelDef[0], pstEnv->pstZoneObj->iClanCityLevelDef, CLAN_CITY_LEVEL_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ClanLevelDef", pstEnv->pstZoneObj->sClanLevelDef[0], &pstEnv->pstZoneObj->iClanLevelDef, 
								           CLAN_LEVEL_DEF_DATA, MAX_CLAN_LEVEL_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iClanLevelDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sClanLevelDef[0], pstEnv->pstZoneObj->iClanLevelDef, CLAN_LEVEL_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "MonsterColorDef", pstEnv->pstZoneObj->sMonsterColorDef[0], &pstEnv->pstZoneObj->iMonsterColorDef, 
								           MONSTER_COLOR_DEF_DATA, MAX_MONSTER_COLOR_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iMonsterColorDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sMonsterColorDef[0], pstEnv->pstZoneObj->iMonsterColorDef, MONSTER_COLOR_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "GameEventDef", pstEnv->pstZoneObj->sGameEventDef[0], &pstEnv->pstZoneObj->iGameEventDef, 
								           GAME_EVENT_DEF_DATA, MAX_GAME_EVENT_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iGameEventDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sGameEventDef[0], pstEnv->pstZoneObj->iGameEventDef, GAME_EVENT_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "DesignationDef", pstEnv->pstZoneObj->sDesignationDef[0], &pstEnv->pstZoneObj->iDesignationDef, 
								           DESIGNATION_DEF_DATA, MAX_DESIGNATION_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iDesignationDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sDesignationDef[0], pstEnv->pstZoneObj->iDesignationDef, DESIGNATION_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "RespactPartyDef", pstEnv->pstZoneObj->sRespactPartyDef[0], &pstEnv->pstZoneObj->iRespactPartyDef, 
							   RESPACT_PARTY_DEF_DATA, MAX_RESPACT_PARTY_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iRespactPartyDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sRespactPartyDef[0], pstEnv->pstZoneObj->iRespactPartyDef, RESPACT_PARTY_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "RespactPhaseDef", pstEnv->pstZoneObj->sRespactPhaseDef[0], &pstEnv->pstZoneObj->iRespactPhaseDef, 
							   RESPACT_PHASE_DEF_DATA, MAX_RESPACT_PHASE_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iRespactPhaseDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sRespactPhaseDef[0], pstEnv->pstZoneObj->iRespactPhaseDef, RESPACT_PHASE_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "RideDef", pstEnv->pstZoneObj->sRideDef[0], &pstEnv->pstZoneObj->iRideDef, 
							   RIDE_DEF_DATA, MAX_RIDE_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iRideDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sRideDef[0], pstEnv->pstZoneObj->iRideDef, RIDE_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "FairyDef", pstEnv->pstZoneObj->sFairyDef[0], &pstEnv->pstZoneObj->iFairyDef, 
							   FAIRY_DEF_DATA, MAX_FAIRY_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iFairyDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sFairyDef[0], pstEnv->pstZoneObj->iFairyDef, FAIRY_DEF_DATA, trl_comp_func);
		}

		if (z_check_fairy(pstEnv) < 0)
		{
			return -1;
		}
		
		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "AddAttrBaseDef", pstEnv->pstZoneObj->sAddAttrBaseDef[0], &pstEnv->pstZoneObj->iAddAttrBaseDef, 
							   ADD_ATTR_BASE_DEF_DATA, MAX_ADD_ATTR_BASE_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iAddAttrBaseDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sAddAttrBaseDef[0], pstEnv->pstZoneObj->iAddAttrBaseDef, ADD_ATTR_BASE_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "AddAttrGroupDef", pstEnv->pstZoneObj->sAddAttrGroupDef[0], &pstEnv->pstZoneObj->iAddAttrGroupDef, 
							   ADD_ATTR_GROUP_DEF_DATA, MAX_ADD_ATTR_GROUP_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iAddAttrGroupDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sAddAttrGroupDef[0], pstEnv->pstZoneObj->iAddAttrGroupDef, ADD_ATTR_GROUP_DEF_DATA, trl_comp_func);
		}
		if (0> z_check_addattr(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "LVGift", pstEnv->pstZoneObj->sLVGift[0], &pstEnv->pstZoneObj->iLVGift, 
							   GIFT_LV_DATA, MAX_GIFT_LV_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iLVGift> 1)
		{
			qsort(pstEnv->pstZoneObj->sLVGift[0], pstEnv->pstZoneObj->iLVGift, GIFT_LV_DATA, trl_comp_func);
		}


		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "OneDayGift", pstEnv->pstZoneObj->sOneDayGift[0], &pstEnv->pstZoneObj->iOneDayGift, 
							   GIFT_ONEDAY_DATA, MAX_GIFT_ONEDAY_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iOneDayGift> 1)
		{
			qsort(pstEnv->pstZoneObj->sOneDayGift[0], pstEnv->pstZoneObj->iOneDayGift, GIFT_ONEDAY_DATA, trl_comp_func);
		}


		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "FixedAddAttr", pstEnv->pstZoneObj->sFixedAddAttr[0], &pstEnv->pstZoneObj->iFixedAddAttr, 
							   FIXED_ADD_ATTR_DATA, MAX_FIXED_ADD_ATTR))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iFixedAddAttr> 1)
		{
			qsort(pstEnv->pstZoneObj->sFixedAddAttr[0], pstEnv->pstZoneObj->iFixedAddAttr, FIXED_ADD_ATTR_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "RndAwdDef", pstEnv->pstZoneObj->sRndAwdDef[0], &pstEnv->pstZoneObj->iRndAwdDef, 
							   RAND_AWARD_DATA, MAX_RAND_AWARD_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iRndAwdDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sRndAwdDef[0], pstEnv->pstZoneObj->iRndAwdDef, RAND_AWARD_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "BCTDef", pstEnv->pstZoneObj->sBCTDef[0], &pstEnv->pstZoneObj->iBCTDef, 
							   BCT_DEF_DATA, MAX_BCT_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iBCTDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sBCTDef[0], pstEnv->pstZoneObj->iBCTDef, BCT_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "SecondarySkillDef", pstEnv->pstZoneObj->sSecondarySkillDef[0], &pstEnv->pstZoneObj->iSecondarySkillDef, 
							   SECONDARY_SKILL_DEF_DATA, MAX_SECONDARY_SKILL_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iSecondarySkillDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sSecondarySkillDef[0], pstEnv->pstZoneObj->iSecondarySkillDef, SECONDARY_SKILL_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "RecipeDef", pstEnv->pstZoneObj->sRecipeDef[0], &pstEnv->pstZoneObj->iRecipeDef, 
							   RECIPE_DEF_DATA, MAX_RECIPE_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iRecipeDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sRecipeDef[0], pstEnv->pstZoneObj->iRecipeDef, RECIPE_DEF_DATA, trl_comp_func);
		}
		
		if(0> z_pre_recipe(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ActiveHelper", pstEnv->pstZoneObj->sActiveHelp[0], &pstEnv->pstZoneObj->iActiveHelp, 
								 ACTIVE_HELP_DATA, MAX_ACTIVE_HELP))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iActiveHelp> 1)
		{
			qsort(pstEnv->pstZoneObj->sActiveHelp[0], pstEnv->pstZoneObj->iActiveHelp, ACTIVE_HELP_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "MonBootyDef", pstEnv->pstZoneObj->sMonBootyDef[0], &pstEnv->pstZoneObj->iMonBootyDef, 
							   MON_BOOTY_DEF_DATA, MAX_MON_BOOTY_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iMonBootyDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sMonBootyDef[0], pstEnv->pstZoneObj->iMonBootyDef, MON_BOOTY_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "MonBootyGroupDef", pstEnv->pstZoneObj->sMonBootyGroupDef[0], &pstEnv->pstZoneObj->iMonBootyGroupDef, 
							   MON_BOOTY_GROUP_DEF_DATA, MAX_MON_BOOTY_GROUP_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iMonBootyGroupDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sMonBootyGroupDef[0], pstEnv->pstZoneObj->iMonBootyGroupDef, MON_BOOTY_GROUP_DEF_DATA, trl_comp_func);
		}

		
		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "CangBaoTuDef", pstEnv->pstZoneObj->sCangBaoTuDef[0], &pstEnv->pstZoneObj->iCangBaoTuDef, 
							   CANGBAOTU_DEF_DATA, MAX_CANGBAOTU_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iCangBaoTuDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sCangBaoTuDef[0], pstEnv->pstZoneObj->iCangBaoTuDef, CANGBAOTU_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "MonMachineDef", pstEnv->pstZoneObj->sMonMachineDef[0], &pstEnv->pstZoneObj->iMonMachineDef, 
							   MON_MACHINE_DEF_DATA, MAX_MON_MACHINE_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iMonMachineDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sMonMachineDef[0], pstEnv->pstZoneObj->iMonMachineDef, MON_MACHINE_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ClanTechDef", pstEnv->pstZoneObj->sClanTechDef[0], &pstEnv->pstZoneObj->iClanTechDef, 
							   CLAN_TECH_DEF_DATA, MAX_CLAN_TECH_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iClanTechDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sClanTechDef[0], pstEnv->pstZoneObj->iClanTechDef, CLAN_TECH_DEF_DATA, trl_comp_func);
		}
		
		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ClanProductDef", pstEnv->pstZoneObj->sClanProductDef[0], &pstEnv->pstZoneObj->iClanProductDef, 
							   CLAN_PRODUCT_DEF_DATA, MAX_CLAN_PRODUCT_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iClanProductDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sClanProductDef[0], pstEnv->pstZoneObj->iClanProductDef, CLAN_PRODUCT_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "AttrContrastDef", pstEnv->pstZoneObj->sAttrContrastDef[0], &pstEnv->pstZoneObj->iAttrContrastDef, 
							   ATTR_CONTRAST_DEF_DATA, MAX_ATTR_CONTRAST_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iAttrContrastDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sAttrContrastDef[0], pstEnv->pstZoneObj->iAttrContrastDef, ATTR_CONTRAST_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "LevelAttrDef", pstEnv->pstZoneObj->sLevelAttrDef[0], &pstEnv->pstZoneObj->iLevelAttrDef, 
							   LEVEL_ATTR_DEF_DATA, MAX_LEVEL_ATTR_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iLevelAttrDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sLevelAttrDef[0], pstEnv->pstZoneObj->iLevelAttrDef, LEVEL_ATTR_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "DiePunishDef", pstEnv->pstZoneObj->sDiePunishDef[0], &pstEnv->pstZoneObj->iDiePunishDef, 
							   DIE_PUNISH_DEF_DATA, MAX_DIE_PUNISH_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iDiePunishDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sDiePunishDef[0], pstEnv->pstZoneObj->iDiePunishDef, DIE_PUNISH_DEF_DATA, trl_comp_func);
		}


		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ContOnlineGiftDef", pstEnv->pstZoneObj->sContOnlineDef[0], &pstEnv->pstZoneObj->iContOnlineDef, 
							   CONT_ONLINE_DEF_DATA, MAX_CONT_ONLINE_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iContOnlineDef > MAX_CONTGIFT)
			return -1;
		
		if (pstEnv->pstZoneObj->iContOnlineDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sContOnlineDef[0], pstEnv->pstZoneObj->iContOnlineDef, 
					CONT_ONLINE_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "BattleDef", pstEnv->pstZoneObj->sBattleDef[0], &pstEnv->pstZoneObj->iBattleDef, 
							   BATTLE_DEF_DATA, MAX_BATTLE_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iBattleDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sBattleDef[0], pstEnv->pstZoneObj->iBattleDef, 
					BATTLE_DEF_DATA, trl_comp_func);
		}


		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "FairyStarStuffDef", pstEnv->pstZoneObj->sFairyStuffDef[0], &pstEnv->pstZoneObj->iFairyStuffDef, 
							   FAIRY_STUFF_DEF_DATA, MAX_FAIRY_STUFF_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iFairyStuffDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sFairyStuffDef[0], pstEnv->pstZoneObj->iFairyStuffDef, 
					FAIRY_STUFF_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "FairyStarDef", pstEnv->pstZoneObj->sFairyStarDef[0], &pstEnv->pstZoneObj->iFairyStarDef, 
							   FAIRY_STAR_DATA, MAX_FAIRY_STAR_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iFairyStarDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sFairyStarDef[0], pstEnv->pstZoneObj->iFairyStarDef, 
					FAIRY_STAR_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PetResolveDef", pstEnv->pstZoneObj->sPetResolveDef[0], &pstEnv->pstZoneObj->iPetResolveDef, 
							   PET_RESOLVE_DEF_DATA, MAX_PET_RESOLVE_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iPetResolveDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sPetResolveDef[0], pstEnv->pstZoneObj->iPetResolveDef, 
					PET_RESOLVE_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PetSkillGridDef", pstEnv->pstZoneObj->sPetSkillGridDef[0], &pstEnv->pstZoneObj->iPetSkillGridDef, 
							   PET_SKILL_GRID_DEF_DATA, MAX_PET_SKILL_GRID_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iPetSkillGridDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sPetSkillGridDef[0], pstEnv->pstZoneObj->iPetSkillGridDef, 
					PET_SKILL_GRID_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PetNativeSkillDef", pstEnv->pstZoneObj->sPetNativeSkillDef[0], &pstEnv->pstZoneObj->iPetNativeSkillDef, 
							   PET_NATIVE_SKILL_DEF_DATA, MAX_PET_NATIVE_SKILL_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iPetNativeSkillDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sPetNativeSkillDef[0], pstEnv->pstZoneObj->iPetNativeSkillDef, 
					PET_NATIVE_SKILL_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PetFuseDef", pstEnv->pstZoneObj->sPetFuseDef[0], &pstEnv->pstZoneObj->iPetFuseDef, 
							   PET_FUSE_DEF_DATA, MAX_PET_FUSE_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iPetFuseDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sPetFuseDef[0], pstEnv->pstZoneObj->iPetFuseDef, 
					PET_FUSE_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "OfflineExpParameter", pstEnv->pstZoneObj->sOfflineExpParameter[0], &pstEnv->pstZoneObj->iOfflineExpParameter,
								OFFLINE_EXP_PARAMETER_DATA, MAX_OFFLINE_EXP_PARAMETER))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iOfflineExpParameter > 1)
		{
			qsort(pstEnv->pstZoneObj->sOfflineExpParameter[0], pstEnv->pstZoneObj->iOfflineExpParameter, 
					OFFLINE_EXP_PARAMETER_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "OnlineGiftGroup", pstEnv->pstZoneObj->sOnlineGiftGroup[0], &pstEnv->pstZoneObj->iOnlineGiftGroup,
								MAX_ONLINE_GROUP_DATA, MAX_GIFT_GROUP_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iOnlineGiftGroup > 1)
		{
			qsort(pstEnv->pstZoneObj->sOnlineGiftGroup[0], pstEnv->pstZoneObj->iOnlineGiftGroup, 
					MAX_ONLINE_GROUP_DATA, trl_comp_func);
		}

		if (z_check_onlinegiftgroup(pstEnv) < 0)
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "WorldItemLimitDef", pstEnv->pstZoneObj->sWorldItemLimitDef[0], &pstEnv->pstZoneObj->iWorldItemLimitDef,
								WORLD_ITEM_LIMIT_DEF_DATA, MAX_WORLD_ITEM_LIMIT_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iWorldItemLimitDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sWorldItemLimitDef[0], pstEnv->pstZoneObj->iWorldItemLimitDef, 
					WORLD_ITEM_LIMIT_DEF_DATA, trl_comp_func);
		}

		//更新一下，针对reloadres, reloadall
		global_item_limit_update(pstEnv);

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "WorldBootyGroupLimitDef", pstEnv->pstZoneObj->sWorldBootyGroupLimitDef[0], &pstEnv->pstZoneObj->iWorldBootyGroupLimitDef,
								WORLD_BOOTY_GROUP_LIMIT_DEF_DATA, MAX_WORLD_BOOTY_GROUP_LIMIT_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iWorldBootyGroupLimitDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sWorldBootyGroupLimitDef[0], pstEnv->pstZoneObj->iWorldBootyGroupLimitDef, 
					WORLD_BOOTY_GROUP_LIMIT_DEF_DATA, trl_comp_func);
		}

		//更新一下，针对reloadres, reloadall
		global_booty_group_limit_update(pstEnv);

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "DiffPworldDef", pstEnv->pstZoneObj->sDiffPworldDef[0], &pstEnv->pstZoneObj->iDiffPworldDef,
								DIFF_PWORLD_DEF_DATA, MAX_DIFF_PWORLD_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iDiffPworldDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sDiffPworldDef[0], pstEnv->pstZoneObj->iDiffPworldDef, 
					DIFF_PWORLD_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "MonEventDef", pstEnv->pstZoneObj->sMonEventDef[0], &pstEnv->pstZoneObj->iMonEventDef,
								MON_EVENT_DEF_DATA, MAX_MON_EVENT_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iMonEventDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sMonEventDef[0], pstEnv->pstZoneObj->iMonEventDef, 
					MON_EVENT_DEF_DATA, trl_comp_func);
		}
		global_mon_event_update(pstEnv);

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PworldBootyGroupDef", pstEnv->pstZoneObj->sPworldBootyGroupDef[0], &pstEnv->pstZoneObj->iPworldBootyGroupDef,
								PWORLD_BOOTY_GROUP_DEF_DATA, MAX_PWORLD_BOOTY_GROUP_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iPworldBootyGroupDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sPworldBootyGroupDef[0], pstEnv->pstZoneObj->iPworldBootyGroupDef, 
					PWORLD_BOOTY_GROUP_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PworldAwardMulDef", pstEnv->pstZoneObj->sPworldAwardMulDef[0], &pstEnv->pstZoneObj->iPworldAwardMulDef,
								PWORLD_AWARD_MUL_DEF_DATA, MAX_PWORLD_AWARD_MUL_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iPworldAwardMulDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sPworldAwardMulDef[0], pstEnv->pstZoneObj->iPworldAwardMulDef, 
					PWORLD_AWARD_MUL_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "AtkerHurtMulDef", pstEnv->pstZoneObj->sAtkerHurtMulDef[0], &pstEnv->pstZoneObj->iAtkerHurtMulDef,
								ATKER_HURT_MUL_DEF_DATA, MAX_ATKER_HURT_MUL_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iAtkerHurtMulDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sAtkerHurtMulDef[0], pstEnv->pstZoneObj->iAtkerHurtMulDef, 
					ATKER_HURT_MUL_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "WorldCityBuildDef", pstEnv->pstZoneObj->sWorldCityBuildDef[0], &pstEnv->pstZoneObj->iWorldCityBuildDef,
								WORLD_CITY_BUILD_DEF_DATA, MAX_WORLD_CITY_BUILD_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iWorldCityBuildDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sWorldCityBuildDef[0], pstEnv->pstZoneObj->iWorldCityBuildDef, 
					WORLD_CITY_BUILD_DEF_DATA, trl_comp_func);
		}
		global_world_city_update(pstEnv);


		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "StarStoneDef", pstEnv->pstZoneObj->sStarStoneDef[0], &pstEnv->pstZoneObj->iStarStoneDef,
								MAX_STARSTONE_DEF_DATA, MAX_STARSTONE_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iStarStoneDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sStarStoneDef[0], pstEnv->pstZoneObj->iStarStoneDef, 
					MAX_STARSTONE_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "StarFuncDef", pstEnv->pstZoneObj->sStarFuncDef[0], &pstEnv->pstZoneObj->iStarFuncDef,
								MAX_STARFUNC_DEF_DATA, MAX_STARFUNC_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iStarFuncDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sStarFuncDef[0], pstEnv->pstZoneObj->iStarFuncDef, 
					MAX_STARFUNC_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "CityProductDef", pstEnv->pstZoneObj->sCityProductDef[0], &pstEnv->pstZoneObj->iCityProductDef,
								CITY_PRODUCT_DEF_DATA, MAX_CITY_PRODUCT_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iCityProductDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sCityProductDef[0], pstEnv->pstZoneObj->iCityProductDef, 
					CITY_PRODUCT_DEF_DATA, trl_comp_func);
		}
		
		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ArmFenJieDef", pstEnv->pstZoneObj->sArmFenJieDef[0], &pstEnv->pstZoneObj->iArmFenJieDef,
								ARMFENJIE_DATA, MAX_ARMFENJIE_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iArmFenJieDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sArmFenJieDef[0], pstEnv->pstZoneObj->iArmFenJieDef, 
					ARMFENJIE_DATA, trl_comp_func);
		}

		if (0 > z_check_fenjie(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "BattleGloryDef", pstEnv->pstZoneObj->sBattleGlory[0], &pstEnv->pstZoneObj->iBattleGlory,
								BATTLE_GLORY_DEF_DATA, MAX_BATTLE_GLORY_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iBattleGlory > 1)
		{
			qsort(pstEnv->pstZoneObj->sBattleGlory[0], pstEnv->pstZoneObj->iBattleGlory, 
					BATTLE_GLORY_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "GemNewDef", pstEnv->pstZoneObj->sGemNewDef[0], &pstEnv->pstZoneObj->iGemNewDef,
								GEM_NEW_DEF_DATA, MAX_GEM_NEW_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iGemNewDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sGemNewDef[0], pstEnv->pstZoneObj->iGemNewDef, 
					GEM_NEW_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "GemAttrStarDef", pstEnv->pstZoneObj->sGemAttrStarDef[0], &pstEnv->pstZoneObj->iGemAttrStarDef,
								GEM_ATTR_STAR_DEF_DATA, MAX_GEM_ATTR_STAR_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iGemAttrStarDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sGemAttrStarDef[0], pstEnv->pstZoneObj->iGemAttrStarDef, 
					GEM_ATTR_STAR_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "GemLevelStarDef", pstEnv->pstZoneObj->sGemLevelStarDef[0], &pstEnv->pstZoneObj->iGemLevelStarDef,
								GEM_LEVEL_STAR_DEF_DATA, MAX_GEM_LEVEL_STAR_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iGemLevelStarDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sGemLevelStarDef[0], pstEnv->pstZoneObj->iGemLevelStarDef, 
					GEM_LEVEL_STAR_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "MediaGiftDef", pstEnv->pstZoneObj->sMediaGiftDef[0], &pstEnv->pstZoneObj->iMediaGiftDef,
								GIFT_MEDIA_DATA, MAX_GIFT_MEDIA_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iMediaGiftDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sMediaGiftDef[0], pstEnv->pstZoneObj->iMediaGiftDef, 
					GIFT_MEDIA_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "LtrGiftDef", pstEnv->pstZoneObj->sLtrGiftDef[0], &pstEnv->pstZoneObj->iLtrGiftDef,
								GIFT_LTR_DATA, MAX_GIFT_LTR_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iLtrGiftDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sLtrGiftDef[0], pstEnv->pstZoneObj->iLtrGiftDef, 
					GIFT_LTR_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ArmRecastDef", pstEnv->pstZoneObj->sArmRecastDef[0], &pstEnv->pstZoneObj->iArmReCastDef,
								ARM_RECAST_DEF_DATA, MAX_ARM_RECAST_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iArmReCastDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sArmRecastDef[0], pstEnv->pstZoneObj->iArmReCastDef, 
					ARM_RECAST_DEF_DATA, trl_comp_func);
		}


		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "UseDef", pstEnv->pstZoneObj->sUseDef[0], &pstEnv->pstZoneObj->iUseDef,
								USE_DEF_DATA, MAX_USE_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iUseDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sUseDef[0], pstEnv->pstZoneObj->iUseDef, 
					USE_DEF_DATA, trl_comp_func);
		}

		
		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "DailyGoalAward", pstEnv->pstZoneObj->sDailyGoalAward[0], &pstEnv->pstZoneObj->iDailyGoalAward,
										DAILY_GOAL_AWARD_DATA, MAX_DAILY_GOAL_AWARD))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iDailyGoalAward > 1)
		{
			qsort(pstEnv->pstZoneObj->sDailyGoalAward[0], pstEnv->pstZoneObj->iDailyGoalAward, 
					DAILY_GOAL_AWARD_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "RuneAddAttrDef", pstEnv->pstZoneObj->sRuneAddAttrDef[0], &pstEnv->pstZoneObj->iRuneAddAttrDef,
										RUNE_ADDATTR_DEF_DATA, MAX_RUNE_ADDATTR_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iRuneAddAttrDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sRuneAddAttrDef[0], pstEnv->pstZoneObj->iRuneAddAttrDef, 
					RUNE_ADDATTR_DEF_DATA, trl_comp_func);
		}

		if(0 > z_pre_rune_addattr(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ArmStarDef", pstEnv->pstZoneObj->sArmStarDef[0], &pstEnv->pstZoneObj->iArmStarDef,
										ARM_STAR_DEF_DATA, MAX_ARM_STAR_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iArmStarDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sArmStarDef[0], pstEnv->pstZoneObj->iArmStarDef, 
					ARM_STAR_DEF_DATA, trl_comp_func);
		}


		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "UnrealMapDef", pstEnv->pstZoneObj->sUnrealMapDef[0], &pstEnv->pstZoneObj->iUnrealMapDef,
										UNREAL_MAP_DEF_SIZE, MAX_UNREAL_MAP_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iUnrealMapDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sUnrealMapDef[0], pstEnv->pstZoneObj->iUnrealMapDef, 
					UNREAL_MAP_DEF_SIZE, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "UnrealDef", pstEnv->pstZoneObj->sUnrealDef[0], &pstEnv->pstZoneObj->iUnrealDef,
										UNREAL_DEF_SIZE, MAX_UNREAL_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iUnrealDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sUnrealDef[0], pstEnv->pstZoneObj->iUnrealDef, 
					UNREAL_DEF_SIZE, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PetWashGeniusDef", pstEnv->pstZoneObj->sPetWashGeniusDef[0], &pstEnv->pstZoneObj->iPetWashGeniusDef,
										PET_WASH_GENIUS_DEF_DATA, MAX_PET_WASH_GENIUS_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iPetWashGeniusDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sPetWashGeniusDef[0], pstEnv->pstZoneObj->iPetWashGeniusDef, 
					PET_WASH_GENIUS_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "NewAddAttrDef", pstEnv->pstZoneObj->sNewAddAttrDef[0], &pstEnv->pstZoneObj->iNewAddAttrDef,
										NEW_ADD_ATTR_DEF_DATA, MAX_NEW_ADD_ATTR_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iNewAddAttrDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sNewAddAttrDef[0], pstEnv->pstZoneObj->iNewAddAttrDef, 
					NEW_ADD_ATTR_DEF_DATA, trl_comp_func);
		}

		if(0 > add_attr_pre(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "AddAttrRandDef", pstEnv->pstZoneObj->sAddAttrRandDef[0], &pstEnv->pstZoneObj->iAddAttrRandDef,
										ADD_ATTR_RAND_DEF_DATA, MAX_ADD_ATTR_RAND_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iAddAttrRandDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sAddAttrRandDef[0], pstEnv->pstZoneObj->iAddAttrRandDef, 
					ADD_ATTR_RAND_DEF_DATA, trl_comp_func);
		}


		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "AchievementCategoryDef", pstEnv->pstZoneObj->sAchievementCategoryDef[0], &pstEnv->pstZoneObj->iAchievementCategoryDef,
								ACHIEVEMENT_CATEGORY_DEF_DATA, MAX_ACHIEVEMENT_CATEGORY_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iAchievementCategoryDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sAchievementCategoryDef[0], pstEnv->pstZoneObj->iAchievementCategoryDef, 
					ACHIEVEMENT_CATEGORY_DEF_DATA, trl_comp_func);
		}
		
		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "AchievementDef", pstEnv->pstZoneObj->sAchievementDef[0], &pstEnv->pstZoneObj->iAchievementDef,
								ACHIEVEMENT_DEF_DATA, MAX_ACHIEVEMENT_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iAchievementDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sAchievementDef[0], pstEnv->pstZoneObj->iAchievementDef, 
					ACHIEVEMENT_DEF_DATA, trl_comp_func);
		}

		if ( 0 > z_pre_achievement(pstEnv))
		{
			return -1;
		}


		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "AddAttrWashDef", pstEnv->pstZoneObj->sAddAttrWashDef[0], &pstEnv->pstZoneObj->iAddAttrWashDef,
										ADD_ATTR_WASH_DEF_DATA, MAX_ADD_ATTR_WASH_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iAddAttrWashDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sAddAttrWashDef[0], pstEnv->pstZoneObj->iAddAttrWashDef, 
					ADD_ATTR_WASH_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "SparDef", pstEnv->pstZoneObj->sSparDef[0], &pstEnv->pstZoneObj->iSparDef,
										SPAR_DEF_SIZE, MAX_SPAR_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iSparDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sSparDef[0], pstEnv->pstZoneObj->iSparDef, 
					SPAR_DEF_SIZE, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ItemStarRandDef", pstEnv->pstZoneObj->sItemStarRandDef[0], &pstEnv->pstZoneObj->iItemStarRandDef,
										ITEM_STAR_RAND_DEF_DATA, MAX_ITEM_STAR_RAND_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iItemStarRandDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sItemStarRandDef[0], pstEnv->pstZoneObj->iItemStarRandDef, 
					ITEM_STAR_RAND_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "SJTaskDef", pstEnv->pstZoneObj->sSJTaskDef[0], &pstEnv->pstZoneObj->iSJTaskDef,
										SJ_TASK_DEF_DATA, MAX_SJ_TASK_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iSJTaskDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sSJTaskDef[0], pstEnv->pstZoneObj->iSJTaskDef, 
					SJ_TASK_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ClanSpyAwardDropDef", pstEnv->pstZoneObj->sClanSpyAwardDropDef[0], &pstEnv->pstZoneObj->iClanSpyAwardDropDef,
								CLAN_SPY_AWARD_DROP_DEF_DATA, MAX_CLAN_SPY_AWARD_DROP_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iClanSpyAwardDropDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sClanSpyAwardDropDef[0], pstEnv->pstZoneObj->iClanSpyAwardDropDef, 
					CLAN_SPY_AWARD_DROP_DEF_DATA, trl_comp_func);
		}

		if ( 0 > z_pre_clan_spy_award_drop(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ClanSpyTransDef", pstEnv->pstZoneObj->sClanSpyTransDef[0], &pstEnv->pstZoneObj->iClanSpyTransDef,
								CLAN_SPY_TRANS_DEF_DATA, MAX_CLAN_SPY_TRANS_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iClanSpyTransDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sClanSpyTransDef[0], pstEnv->pstZoneObj->iClanSpyTransDef, 
					CLAN_SPY_TRANS_DEF_DATA, trl_comp_func);
		}

		if ( 0 > z_pre_clan_spy_trans(pstEnv))
		{
			return -1;
		}


		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "DartCarDef", pstEnv->pstZoneObj->sDartCarDef[0], &pstEnv->pstZoneObj->iDartCarDef,
										DART_CAR_DEF_DATA, MAX_DART_CAR_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iDartCarDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sDartCarDef[0], pstEnv->pstZoneObj->iDartCarDef, 
					DART_CAR_DEF_DATA, trl_comp_func);
		}
		

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PworldWaveMonDef", pstEnv->pstZoneObj->sPworldWaveMonDef[0], &pstEnv->pstZoneObj->iPworldWaveMonDef,
								PWORLD_WAVE_MON_DEF_DATA, MAX_PWORLD_WAVE_MON_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iPworldWaveMonDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sPworldWaveMonDef[0], pstEnv->pstZoneObj->iPworldWaveMonDef, 
					PWORLD_WAVE_MON_DEF_DATA, trl_comp_func);
		}

		if ( 0 > z_pre_pworld_wave_mon_def(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PworldWaveMonPosDef", pstEnv->pstZoneObj->sPworldWaveMonPosDef[0], &pstEnv->pstZoneObj->iPworldWaveMonPosDef,
								PWORLD_WAVE_MON_POS_DEF_DATA, MAX_PWORLD_WAVE_MON_POS_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iPworldWaveMonPosDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sPworldWaveMonPosDef[0], pstEnv->pstZoneObj->iPworldWaveMonPosDef, 
					PWORLD_WAVE_MON_POS_DEF_DATA, trl_comp_func);
		}

		if ( 0 > z_pre_pworld_wave_mon_pos_def(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PworldWaveMonClanAwardDef", pstEnv->pstZoneObj->sPworldWaveMonClanAwardDef[0], &pstEnv->pstZoneObj->iPworldWaveMonClanAwardDef,
								PWORLD_WAVE_MON_CLAN_AWARD_DEF_DATA, MAX_PWORLD_WAVE_MON_CLAN_AWARD_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iPworldWaveMonClanAwardDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sPworldWaveMonClanAwardDef[0], pstEnv->pstZoneObj->iPworldWaveMonClanAwardDef, 
					PWORLD_WAVE_MON_CLAN_AWARD_DEF_DATA, trl_comp_func);
		}

		if ( 0 > z_pre_pworld_wave_mon_clan_award_def(pstEnv))
		{
			return -1;
		}


		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "SubJectDef", pstEnv->pstZoneObj->sSubJectDef[0], &pstEnv->pstZoneObj->iSubJectDef,
								SUBJECT_DEF_DATA, MAX_SUBJECT_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iSubJectDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sSubJectDef[0], pstEnv->pstZoneObj->iSubJectDef, 
					SUBJECT_DEF_DATA, trl_comp_func);
		}

		if (subject_db_pre(pstEnv) < 0)
		{
			return -1;
		}


		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "SubJectTypeDef", pstEnv->pstZoneObj->sSubJectTypeDef[0], &pstEnv->pstZoneObj->iSubJectTypeDef,
								SUBJECT_TYPE_DEF_DATA, MAX_SUBJECT_TYPE_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iSubJectTypeDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sSubJectTypeDef[0], pstEnv->pstZoneObj->iSubJectTypeDef, 
					SUBJECT_TYPE_DEF_DATA, trl_comp_func);
		}

		if (subject_type_db_pre(pstEnv) < 0)
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "GayDef", pstEnv->pstZoneObj->sGayDef[0], &pstEnv->pstZoneObj->iGayDef,
								GAY_DEF_DATA, MAX_GAY_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iGayDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sGayDef[0], pstEnv->pstZoneObj->iGayDef, 
					GAY_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "RangDsnDef", pstEnv->pstZoneObj->sRangDsnDef[0], &pstEnv->pstZoneObj->iRangDsnDef,
								RANGDSN_DEF_DATA, MAX_RANGDSN_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iRangDsnDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sRangDsnDef[0], pstEnv->pstZoneObj->iRangDsnDef, 
					RANGDSN_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PworldMachineBuffShopDef", pstEnv->pstZoneObj->sPworldMachineBuffShopDef[0], &pstEnv->pstZoneObj->iPworldMachineBuffShopDef,
								PWORLD_MACHINE_BUFF_SHOP_DEF_DATA, MAX_PWORLD_MACHINE_BUFF_SHOP_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iPworldMachineBuffShopDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sPworldMachineBuffShopDef[0], pstEnv->pstZoneObj->iPworldMachineBuffShopDef, 
					PWORLD_MACHINE_BUFF_SHOP_DEF_DATA, trl_comp_func);
		}

		if ( 0 > z_pre_pworld_machine_buff_shop(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PlayerAwardDef", pstEnv->pstZoneObj->sPlayerAwardDef[0], &pstEnv->pstZoneObj->iPlayerAwardDef,
								PLAYER_AWARD_DEF_DATA, MAX_PLAYER_AWARD_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iPlayerAwardDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sPlayerAwardDef[0], pstEnv->pstZoneObj->iPlayerAwardDef, 
					PLAYER_AWARD_DEF_DATA, trl_comp_func);
		}

		if ( 0 > z_pre_player_award(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "CowFinalAwardDef", pstEnv->pstZoneObj->sCowFinalAwardDef[0], &pstEnv->pstZoneObj->iCowFinalAwardDef,
								COW_FINAL_AWARD_DEF_DATA, MAX_COW_FINAL_AWARD_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iCowFinalAwardDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sCowFinalAwardDef[0], pstEnv->pstZoneObj->iCowFinalAwardDef, 
					COW_FINAL_AWARD_DEF_DATA, trl_comp_func);
		}

		if ( 0 > z_pre_cow_final_award(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "GmTrade", pstEnv->pstZoneObj->sGmTrade[0], &pstEnv->pstZoneObj->iGmTrade,
								GM_TRADE_DATA, MAX_GM_TRADE))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iGmTrade > 1)
		{
			qsort(pstEnv->pstZoneObj->sGmTrade[0], pstEnv->pstZoneObj->iGmTrade, 
					GM_TRADE_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "StrongPointDef", pstEnv->pstZoneObj->sStrongPointDef[0], &pstEnv->pstZoneObj->iStrongPointDef,
								STRONG_POINT_DEF_DATA, MAX_STRONG_POINT_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iStrongPointDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sStrongPointDef[0], pstEnv->pstZoneObj->iStrongPointDef, 
					STRONG_POINT_DEF_DATA, trl_comp_func);
		}


		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ClanSkillDef", pstEnv->pstZoneObj->sClanSkillDef[0], &pstEnv->pstZoneObj->iClanSkillDef,
								CLAN_SKILL_DEF_DATA, MAX_CLAN_SKILL_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iClanSkillDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sClanSkillDef[0], pstEnv->pstZoneObj->iClanSkillDef, 
					CLAN_SKILL_DEF_DATA, trl_comp_func);
		}

		if ( 0 > z_pre_clan_skill(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "BannerDef", pstEnv->pstZoneObj->sBannerDef[0], &pstEnv->pstZoneObj->iBannerDef,
								BANNER_DEF_DATA, MAX_BANNER_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iBannerDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sBannerDef[0], pstEnv->pstZoneObj->iBannerDef, 
					BANNER_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ReviveDef", pstEnv->pstZoneObj->sReviveDef[0], &pstEnv->pstZoneObj->iReviveDef,
								REVIVE_DEF_DATA, MAX_REVIVE_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iReviveDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sReviveDef[0], pstEnv->pstZoneObj->iReviveDef, 
					REVIVE_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ClanPayDef", pstEnv->pstZoneObj->sClanPayDef[0], &pstEnv->pstZoneObj->iClanPayDef,
								CLAN_PAY_DEF_DATA, MAX_CLAN_PAY_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iClanPayDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sClanPayDef[0], pstEnv->pstZoneObj->iClanPayDef, 
					CLAN_PAY_DEF_DATA, trl_comp_func);
		}

		if ( 0 > z_pre_clan_pay(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "LevelExpDef", pstEnv->pstZoneObj->sLevelExpDef[0], &pstEnv->pstZoneObj->iLevelExpDef,
								LEVEL_EXP_DEF_DATA, MAX_LEVEL_EXP_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iLevelExpDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sLevelExpDef[0], pstEnv->pstZoneObj->iLevelExpDef, 
					LEVEL_EXP_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "LevelBuffExpDef", pstEnv->pstZoneObj->sLevelBuffExpDef[0], &pstEnv->pstZoneObj->iLevelBuffExpDef,
								LEVEL_BUFF_EXP_DEF_DATA, MAX_LEVEL_BUFF_EXP_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iLevelBuffExpDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sLevelBuffExpDef[0], pstEnv->pstZoneObj->iLevelBuffExpDef, 
					LEVEL_BUFF_EXP_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "RidePinZhiDef", pstEnv->pstZoneObj->sRidePinZhiDef[0], &pstEnv->pstZoneObj->iRidePinZhiDef,
								RIDE_PINZHI_DEF_DATA, MAX_RIDE_PINZHI_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iRidePinZhiDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sRidePinZhiDef[0], pstEnv->pstZoneObj->iRidePinZhiDef, 
					RIDE_PINZHI_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "MapAwardOreDef", pstEnv->pstZoneObj->sMapAwardOreDef[0], &pstEnv->pstZoneObj->iMapAwardOreDef,
								MAP_AWARD_ORE_DEF_DATA, MAX_MAP_AWARD_ORE_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iMapAwardOreDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sMapAwardOreDef[0], pstEnv->pstZoneObj->iMapAwardOreDef, 
					MAP_AWARD_ORE_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PetLoveLevelDef", pstEnv->pstZoneObj->sPetLoveLevelDef[0], &pstEnv->pstZoneObj->iPetLoveLevelDef,
								PET_LOVE_LEVEL_DEF_DATA, MAX_PET_LOVE_LEVEL_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iPetLoveLevelDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sPetLoveLevelDef[0], pstEnv->pstZoneObj->iPetLoveLevelDef, 
					PET_LOVE_LEVEL_DEF_DATA, trl_comp_func);
		}

		if ( 0 > z_pre_pet_love_level(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "RongluDef", pstEnv->pstZoneObj->sRongluDef[0], &pstEnv->pstZoneObj->iRongluDef,
								RONGLU_DEF_DATA, MAX_RONGLU_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iRongluDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sRongluDef[0], pstEnv->pstZoneObj->iRongluDef, 
					RONGLU_DEF_DATA, trl_comp_func);
		}

		if (ronglu_check_pre( pstEnv) < 0)
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "GeniusCareerDef", pstEnv->pstZoneObj->sGeniusCareerDef[0], &pstEnv->pstZoneObj->iGeniusCareerDef,
								GENIUS_CAREER_DEF_DATA, MAX_GENIUS_CAREER_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iGeniusCareerDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sGeniusCareerDef[0], pstEnv->pstZoneObj->iGeniusCareerDef, 
					GENIUS_CAREER_DEF_DATA, trl_comp_func);
		}
		
		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "GeniusAttrDef", pstEnv->pstZoneObj->sGeniusAttrDef[0], &pstEnv->pstZoneObj->iGeniusAttrDef,
								GENIUS_ATTR_DEF_DATA, MAX_GENIUS_ATTR_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iGeniusAttrDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sGeniusAttrDef[0], pstEnv->pstZoneObj->iGeniusAttrDef, 
					GENIUS_ATTR_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "BaoXiangDef", pstEnv->pstZoneObj->sBaoXiangDef[0], &pstEnv->pstZoneObj->iBaoXiangDef,
								BAOXIANG_DEF_DATA, MAX_BAOXIANG_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iBaoXiangDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sBaoXiangDef[0], pstEnv->pstZoneObj->iBaoXiangDef, 
					BAOXIANG_DEF_DATA, trl_comp_func);
		}
		//更新一下，针对reloadres, reloadall
		global_baoxiang_update(pstEnv);

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "WorldCityFightAwardDef", pstEnv->pstZoneObj->sWorldCityFightAwardDef[0], &pstEnv->pstZoneObj->iWorldCityFightAwardDef,
								WORLD_CITY_FIGHT_AWARD_DEF_DATA, MAX_WORLD_CITY_FIGHT_AWARD_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iWorldCityFightAwardDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sWorldCityFightAwardDef[0], pstEnv->pstZoneObj->iWorldCityFightAwardDef, 
					WORLD_CITY_FIGHT_AWARD_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "LevelFeng", pstEnv->pstZoneObj->sLevelFengDef[0], &pstEnv->pstZoneObj->iLevelFengDef,
								LEVEL_FENG_DEF_DATA, MAX_LEVEL_FENG_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iLevelFengDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sLevelFengDef[0], pstEnv->pstZoneObj->iLevelFengDef, 
					LEVEL_FENG_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ArmLevelUPDef", pstEnv->pstZoneObj->sArmLevelUPDef[0], &pstEnv->pstZoneObj->iArmLevelUPDef,
								ARM_LEVELUP_DEF_DATA, MAX_ARM_LEVELUP_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iArmLevelUPDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sArmLevelUPDef[0], pstEnv->pstZoneObj->iArmLevelUPDef, 
					ARM_LEVELUP_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "WorldBossDef", pstEnv->pstZoneObj->sWorldBossDef[0], &pstEnv->pstZoneObj->iWorldBossDef,
								WORLD_BOSS_DEF_DATA, MAX_WORLD_BOSS_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iWorldBossDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sWorldBossDef[0], pstEnv->pstZoneObj->iWorldBossDef, 
					WORLD_BOSS_DEF_DATA, trl_comp_func);
		}
		

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "LuckyMapDef", pstEnv->pstZoneObj->sLuckyMapDef[0], &pstEnv->pstZoneObj->iLuckyMapDef,
								LUCKY_MAP_DEF_DATA, MAX_LUCKY_MAP_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iLuckyMapDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sLuckyMapDef[0], pstEnv->pstZoneObj->iLuckyMapDef, 
					LUCKY_MAP_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "BusyDef", pstEnv->pstZoneObj->sBusyDef[0], &pstEnv->pstZoneObj->iBusyDef,
								BUSY_DEF_DATA, MAX_BUSY_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iBusyDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sBusyDef[0], pstEnv->pstZoneObj->iBusyDef, 
					BUSY_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "BusyUseDef", pstEnv->pstZoneObj->sBusyUseDef[0], &pstEnv->pstZoneObj->iBusyUseDef,
								BUSY_USE_DEF_DATA, MAX_BUSY_USE_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iBusyUseDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sBusyUseDef[0], pstEnv->pstZoneObj->iBusyUseDef, 
					BUSY_USE_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ItemIDDef", pstEnv->pstZoneObj->sItemIDDef[0], &pstEnv->pstZoneObj->iItemIDDef,
								ITEM_ID_DEF_DATA, MAX_ITEM_ID_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iItemIDDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sItemIDDef[0], pstEnv->pstZoneObj->iItemIDDef, 
					ITEM_ID_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "CareerHurtDef", pstEnv->pstZoneObj->sCareerHurtDef[0], &pstEnv->pstZoneObj->iCareerHurtDef,
								CAREER_HURT_DEF_DATA, MAX_CAREER_HURT_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iCareerHurtDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sCareerHurtDef[0], pstEnv->pstZoneObj->iCareerHurtDef, 
					CAREER_HURT_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "GeniusAddDef", pstEnv->pstZoneObj->sGeniusAddDef[0], &pstEnv->pstZoneObj->iGeniusAddDef,
								GENIUS_ADD_DEF_DATA, MAX_GENIUS_ADD_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iGeniusAddDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sGeniusAddDef[0], pstEnv->pstZoneObj->iGeniusAddDef, 
					GENIUS_ADD_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "HighWashUseDef", pstEnv->pstZoneObj->sHighWashUseDef[0], &pstEnv->pstZoneObj->iHighWashUseDef,
								HIGHWASH_USE_DEF_DATA, MAX_HIGHWASH_USE_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iHighWashUseDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sHighWashUseDef[0], pstEnv->pstZoneObj->iHighWashUseDef, 
					HIGHWASH_USE_DEF_DATA, trl_comp_func);
		}

		if ( 0 > z_pre_lucky_map(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PetGeniusDef", pstEnv->pstZoneObj->sPetGeniusDef[0], &pstEnv->pstZoneObj->iPetGeniusDef,
								PET_GENIUS_DEF_DATA, MAX_PET_GENIUS_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iPetGeniusDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sPetGeniusDef[0], pstEnv->pstZoneObj->iPetGeniusDef, 
					PET_GENIUS_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "WeDef", pstEnv->pstZoneObj->sWeDef[0], &pstEnv->pstZoneObj->iWeDef,
								WE_DEF_DATA, MAX_WE_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iWeDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sWeDef[0], pstEnv->pstZoneObj->iWeDef, 
					WE_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ExchgItemDef", pstEnv->pstZoneObj->sExchgItemDef[0], &pstEnv->pstZoneObj->iExchgItemDef,
								EXCHG_ITEM_DEF_DATA, MAX_EXCHG_ITEM_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iExchgItemDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sExchgItemDef[0], pstEnv->pstZoneObj->iExchgItemDef, 
					EXCHG_ITEM_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "SpartaAwardDef", pstEnv->pstZoneObj->sSpartaAwardDef[0], &pstEnv->pstZoneObj->iSpartaAwardDef,
								SPARTA_AWARD_DEF_DATA, MAX_SPARTA_AWARD_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iSpartaAwardDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sSpartaAwardDef[0], pstEnv->pstZoneObj->iSpartaAwardDef, 
					SPARTA_AWARD_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "SpartaLevelDef", pstEnv->pstZoneObj->sSpartaLevelDef[0], &pstEnv->pstZoneObj->iSpartaLevelDef,
								SPARTA_LEVEL_DEF_DATA, MAX_SPARTA_LEVEL_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iSpartaLevelDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sSpartaLevelDef[0], pstEnv->pstZoneObj->iSpartaLevelDef, 
					SPARTA_LEVEL_DEF_DATA, trl_comp_func);
		}


		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "MapRandTransmit", pstEnv->pstZoneObj->sMapRandTransmitDef[0], &pstEnv->pstZoneObj->iMapRandTransmitDef,
									MAP_RAND_TRANS_DATA, MAX_MAP_RAND_TRANS_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iMapRandTransmitDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sMapRandTransmitDef[0], pstEnv->pstZoneObj->iMapRandTransmitDef, 
						MAP_RAND_TRANS_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "SchoolLevelDef", pstEnv->pstZoneObj->sSchoolLevelDef[0], &pstEnv->pstZoneObj->iSchoolLevelDef,
									SCHOOL_LEVEL_DEF_DATA, MAX_SCHOOL_LEVEL_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iSchoolLevelDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sSchoolLevelDef[0], pstEnv->pstZoneObj->iSchoolLevelDef, 
						SCHOOL_LEVEL_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "SchoolPworldDef", pstEnv->pstZoneObj->sSchoolPworldDef[0], &pstEnv->pstZoneObj->iSchoolPworldDef,
									SCHOOL_PWORLD_DEF_DATA, MAX_SCHOOL_PWORLD_DEF))
		{
			return -1;
		}
		if (pstEnv->pstZoneObj->iSchoolPworldDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sSchoolPworldDef[0], pstEnv->pstZoneObj->iSchoolPworldDef, 
						SCHOOL_PWORLD_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "SchoolXinDeDef", pstEnv->pstZoneObj->sSchoolXinDeDef[0], &pstEnv->pstZoneObj->iSchoolXinDeDef,
									SCHOOL_XINDE_DEF_DATA, MAX_SCHOOL_XINDE_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iSchoolXinDeDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sSchoolXinDeDef[0], pstEnv->pstZoneObj->iSchoolXinDeDef, 
						SCHOOL_XINDE_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "BackLevelDef", pstEnv->pstZoneObj->sBackLevelDef[0], &pstEnv->pstZoneObj->iBackLevelDef,
									BACK_LEVEL_DEF_DATA, MAX_BACK_LEVEL_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iBackLevelDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sBackLevelDef[0], pstEnv->pstZoneObj->iBackLevelDef, 
						BACK_LEVEL_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "XuKongPosDef", pstEnv->pstZoneObj->sXuKongPosDef[0], &pstEnv->pstZoneObj->iXuKongPosDef,
									XUKONG_POS_DEF_DATA, MAX_XUKONG_POS_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iXuKongPosDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sXuKongPosDef[0], pstEnv->pstZoneObj->iXuKongPosDef, 
						XUKONG_POS_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "NewRoleGift", pstEnv->pstZoneObj->sNewRoleGiftDef[0], &pstEnv->pstZoneObj->iNewRoleGiftDef,
									NEW_ROLE_GIFT_DEF_SIZE, MAX_NEW_ROLE_GIFT_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iNewRoleGiftDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sNewRoleGiftDef[0], pstEnv->pstZoneObj->iNewRoleGiftDef, 
						NEW_ROLE_GIFT_DEF_SIZE, trl_comp_func);
		}


		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ContCoverGift", pstEnv->pstZoneObj->sContCoverGiftDef[0], &pstEnv->pstZoneObj->iContCoverGiftDef,
									CONT_COVER_DEF_SIZE, MAX_CONT_COVER_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iContCoverGiftDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sContCoverGiftDef[0], pstEnv->pstZoneObj->iContCoverGiftDef, 
						CONT_COVER_DEF_SIZE, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "CardBoxDef", pstEnv->pstZoneObj->sCardBoxDef[0], &pstEnv->pstZoneObj->iCardBoxDef,
									CARD_BOX_DEF_DATA, MAX_CARD_BOX_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iCardBoxDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sCardBoxDef[0], pstEnv->pstZoneObj->iCardBoxDef, 
						CARD_BOX_DEF_DATA, trl_comp_func);
		}

		global_cardbox_update(pstEnv);

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "CardBoxGroupDef", pstEnv->pstZoneObj->sCardBoxGroupDef[0], &pstEnv->pstZoneObj->iCardBoxGroupDef,
									CARD_BOX_GROUP_DEF_DATA, MAX_CARD_BOX_GROUP_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iCardBoxGroupDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sCardBoxGroupDef[0], pstEnv->pstZoneObj->iCardBoxGroupDef, 
						CARD_BOX_GROUP_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ArmBuChangDef", pstEnv->pstZoneObj->sArmBuChangDef[0], &pstEnv->pstZoneObj->iArmBuChangDef,
									ARM_BUCHANG_DEF_DATA, MAX_ARM_BUCHANG_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iArmBuChangDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sArmBuChangDef[0], pstEnv->pstZoneObj->iArmBuChangDef, 
					ARM_BUCHANG_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "BuChangDef", pstEnv->pstZoneObj->sBuChangDef[0], &pstEnv->pstZoneObj->iBuChangDef,
									BUCHANG_DEF_DATA, MAX_BUCHANG_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iBuChangDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sBuChangDef[0], pstEnv->pstZoneObj->iBuChangDef, 
					BUCHANG_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PworldBoxDef", pstEnv->pstZoneObj->sPworldBoxDef[0], &pstEnv->pstZoneObj->iPworldBoxDef,
									PWORLD_BOX_DEF_DATA, MAX_PWORLD_BOX_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iPworldBoxDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sPworldBoxDef[0], pstEnv->pstZoneObj->iPworldBoxDef, 
					PWORLD_BOX_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "YangHaiBangDef", pstEnv->pstZoneObj->sYangHaiBangDef[0], &pstEnv->pstZoneObj->iYangHaiBangDef,
									YANGHAIBANG_DEF_DATA, MAX_YANGHAIBANG_DEF))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "HangHaiAwardDef", pstEnv->pstZoneObj->sHangHaiAwardDef[0], &pstEnv->pstZoneObj->iHangHaiAwardDef,
									HANG_HAI_AWARD_DEF_DATA, MAX_HANG_HAI_AWARD_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iYangHaiBangDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sYangHaiBangDef[0], pstEnv->pstZoneObj->iYangHaiBangDef, 
					YANGHAIBANG_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "StoreGridDef", pstEnv->pstZoneObj->sStoreGridDef[0], &pstEnv->pstZoneObj->iStoreGridDef,
									STORE_GRID_DEF_DATA, MAX_STORE_GRID_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iStoreGridDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sStoreGridDef[0], pstEnv->pstZoneObj->iStoreGridDef, 
					STORE_GRID_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ArmStepUpDef", pstEnv->pstZoneObj->sArmStepUpDef[0], &pstEnv->pstZoneObj->iArmStepUpDef,
									ARM_STEP_UP_DEF_DATA, MAX_ARM_STEP_UP_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iArmStepUpDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sArmStepUpDef[0], pstEnv->pstZoneObj->iArmStepUpDef, 
					ARM_STEP_UP_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ArmReSetDef", pstEnv->pstZoneObj->sArmReSetDef[0], &pstEnv->pstZoneObj->iArmReSetDef,
									ARM_RESET_DEF_DATA, MAX_ARM_RESET_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iArmReSetDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sArmReSetDef[0], pstEnv->pstZoneObj->iArmReSetDef, 
					ARM_RESET_DEF_DATA, trl_comp_func);
		}


		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ArmGradeValDef", pstEnv->pstZoneObj->sArmGradeValDef[0], &pstEnv->pstZoneObj->iArmGradeValDef,
									ARM_VAL_DEF_DATA, MAX_ARM_VAL_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iArmGradeValDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sArmGradeValDef[0], pstEnv->pstZoneObj->iArmGradeValDef, 
					ARM_VAL_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "RongYanJuShouDef", pstEnv->pstZoneObj->sRongYanJuShouDef[0], &pstEnv->pstZoneObj->iRongYanJuShouDef,
									RONGYANJUSHOU_DEF_DATA, MAX_RONGYANJUSHOU_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iRongYanJuShouDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sRongYanJuShouDef[0], pstEnv->pstZoneObj->iRongYanJuShouDef, 
					RONGYANJUSHOU_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "RongYanMonDynAttrDef", pstEnv->pstZoneObj->sRongYanMonDynAttrDef[0], &pstEnv->pstZoneObj->iRongYanMonDynAttrDef,
									RONGYANMONDYNATTR_DEF_DATA, MAX_RONGYANMONDYNATTR_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iRongYanMonDynAttrDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sRongYanMonDynAttrDef[0], pstEnv->pstZoneObj->iRongYanMonDynAttrDef, 
					RONGYANMONDYNATTR_DEF_DATA, trl_comp_func);
		}

	

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ClanAwardDef", pstEnv->pstZoneObj->sClanAwardDef[0], &pstEnv->pstZoneObj->iClanAwardDef,
								CLAN_AWARD_DEF_DATA, MAX_CLAN_AWARD_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iClanAwardDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sClanAwardDef[0], pstEnv->pstZoneObj->iClanAwardDef, 
					CLAN_AWARD_DEF_DATA, trl_comp_func);
		}

		if ( 0 > z_pre_clan_award(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ClanWarAwardDef", pstEnv->pstZoneObj->sClanWarAwardDef[0], &pstEnv->pstZoneObj->iClanWarAwardDef,
								CLAN_WAR_AWARD_DEF_DATA, MAX_CLAN_WAR_AWARD_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iClanWarAwardDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sClanWarAwardDef[0], pstEnv->pstZoneObj->iClanWarAwardDef, 
					CLAN_WAR_AWARD_DEF_DATA, trl_comp_func);
		}

		if ( 0 > z_pre_clan_war_award(pstEnv))
		{
			return -1;
		}


		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ArmSkillNumDef", pstEnv->pstZoneObj->sArmSkillNumDef[0], &pstEnv->pstZoneObj->iArmSkillNumDef,
								ARM_SKILL_NUM_DEF_DATA, MAX_ARM_SKILL_NUM_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iArmSkillNumDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sArmSkillNumDef[0], pstEnv->pstZoneObj->iArmSkillNumDef, 
					ARM_SKILL_NUM_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ArmSkillAttrDef", pstEnv->pstZoneObj->sArmSkillAttrDef[0], &pstEnv->pstZoneObj->iArmSkillAttrDef,
								ARM_SKILL_ATTR_DEF_DATA, MAX_ARM_SKILL_ATTR_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iArmSkillAttrDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sArmSkillAttrDef[0], pstEnv->pstZoneObj->iArmSkillAttrDef, 
					ARM_SKILL_ATTR_DEF_DATA, trl_comp_func);
		}

		if (arm_skill_fast_init(pstEnv) < 0)
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "GeniusMdfDef", pstEnv->pstZoneObj->sGeniusMdfDef[0], &pstEnv->pstZoneObj->iGeniusMdfDef,
								GENIUS_MDF_DEF_DATA, MAX_GENIUS_MDF_DEF))
		{
			return -1;
		}

		if (pstEnv->pstZoneObj->iGeniusMdfDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sGeniusMdfDef[0], pstEnv->pstZoneObj->iGeniusMdfDef, 
					GENIUS_MDF_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PaoShangDef", pstEnv->pstZoneObj->sPaoShangDef[0], &pstEnv->pstZoneObj->iPaoShangDef,
									PAOSHANG_DEF_DATA, MAX_PAOSHANG_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iPaoShangDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sPaoShangDef[0], pstEnv->pstZoneObj->iPaoShangDef, 
					PAOSHANG_DEF_DATA, trl_comp_func);
		}

        	if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PaoShangItemDef", pstEnv->pstZoneObj->sPaoShangItemDef[0], &pstEnv->pstZoneObj->iPaoShangItemDef,
									PAOSHANG_ITEM_DEF_DATA, MAX_PAOSHANG_ITEM_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iPaoShangItemDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sPaoShangItemDef[0], pstEnv->pstZoneObj->iPaoShangItemDef, 
					PAOSHANG_ITEM_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PetMoneyDef", pstEnv->pstZoneObj->sPetMoneyDef[0], &pstEnv->pstZoneObj->iPetMoneyDef,
									PET_MONEY_DEF_DATA, MAX_PET_MONEY_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iPetMoneyDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sPetMoneyDef[0], pstEnv->pstZoneObj->iPetMoneyDef, 
					PET_MONEY_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PetImpUpAttrDef", pstEnv->pstZoneObj->sPetImpUpAttrDef[0], 
								&pstEnv->pstZoneObj->iPetImpUpAttrDef,PET_IMP_UP_ATTR_DEF_DATA, MAX_PET_IMP_UP_ATTR_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iPetImpUpAttrDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sPetImpUpAttrDef[0], pstEnv->pstZoneObj->iPetImpUpAttrDef, 
					PET_IMP_UP_ATTR_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PetImpUpDef", pstEnv->pstZoneObj->sPetImpUpDef[0],
								&pstEnv->pstZoneObj->iPetImpUpDef,PET_IMP_UP_DEF_DATA, MAX_PET_IMP_UP_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iPetImpUpDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sPetImpUpDef[0], pstEnv->pstZoneObj->iPetImpUpDef, 
					PET_IMP_UP_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PetOpAwardDef", pstEnv->pstZoneObj->sPetOpAwardDef[0], 
							&pstEnv->pstZoneObj->iPetOpAwardDef, PET_OP_AWARD_DEF_DATA, MAX_PET_OP_AWARD_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iPetOpAwardDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sPetOpAwardDef[0], pstEnv->pstZoneObj->iPetOpAwardDef, 
					PET_OP_AWARD_DEF_DATA, trl_comp_func);
		}	
		
		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "GemPagDef", pstEnv->pstZoneObj->sGemPagDef[0], &pstEnv->pstZoneObj->iGemPagDef,GEM_PAG_DEF_DATA, MAX_GEM_PAG_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iGemPagDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sGemPagDef[0], pstEnv->pstZoneObj->iGemPagDef, 
					GEM_PAG_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ArmTeshuDef", pstEnv->pstZoneObj->sArmTeshuDef[0], 
								&pstEnv->pstZoneObj->iArmTeshuDef,
								ARM_TESHU_DEF_DATA, MAX_ARM_TESHU_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iArmTeshuDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sArmTeshuDef[0], pstEnv->pstZoneObj->iArmTeshuDef, 
					ARM_TESHU_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ZJRongYanJuShouDef", pstEnv->pstZoneObj->sZJRongYanJuShouDef[0], 
								&pstEnv->pstZoneObj->iZJRongYanJuShouDef,
								ZJ_RONGYANJUSHOU_DEF_DATA, MAX_ZJ_RONGYANJUSHOU_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iZJRongYanJuShouDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sZJRongYanJuShouDef[0], pstEnv->pstZoneObj->iZJRongYanJuShouDef, 
					ZJ_RONGYANJUSHOU_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "SpanStrongDef", pstEnv->pstZoneObj->sSpanStrongDef[0], 
								&pstEnv->pstZoneObj->iSpanStrongDef,
								SPAN_STRONG_DEF_SIZE, MAX_SPAN_STRONG_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iSpanStrongDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sSpanStrongDef[0], pstEnv->pstZoneObj->iSpanStrongDef, 
					SPAN_STRONG_DEF_SIZE, trl_comp_func);
		}

		
		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "WordPwdDef", pstEnv->pstZoneObj->sWordPwdDef[0], 
										&pstEnv->pstZoneObj->iWordPwdDef,
										WORD_PWD_DEF_DATA, MAX_WORD_PWD_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iWordPwdDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sWordPwdDef[0], pstEnv->pstZoneObj->iWordPwdDef, 
					WORD_PWD_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "HomeLvlDef", pstEnv->pstZoneObj->sHomeLvlDef[0], 
										&pstEnv->pstZoneObj->iHomeLvlDef,
										HOME_LVL_DEF_DATA, MAX_HOME_LVL_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iHomeLvlDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sHomeLvlDef[0], pstEnv->pstZoneObj->iHomeLvlDef, 
					HOME_LVL_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "HomeMonLvlDef", pstEnv->pstZoneObj->sHomeMonLvlDef[0], 
										&pstEnv->pstZoneObj->iHomeMonLvlDef,
										HOME_MON_LVL_DEF_DATA, MAX_HOME_MON_LVL_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iHomeMonLvlDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sHomeMonLvlDef[0], pstEnv->pstZoneObj->iHomeMonLvlDef, 
					HOME_MON_LVL_DEF_DATA, trl_comp_func);
		}


		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "HomeMonSegDef", pstEnv->pstZoneObj->sHomeMonSegDef[0], 
										&pstEnv->pstZoneObj->iHomeMonSegDef,
										HOME_MON_SEG_DEF_DATA, MAX_HOME_MON_SEG_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iHomeMonSegDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sHomeMonSegDef[0], pstEnv->pstZoneObj->iHomeMonSegDef, 
					HOME_MON_SEG_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "HomeFarmDef", pstEnv->pstZoneObj->sHomeFarmDef[0], 
										&pstEnv->pstZoneObj->iHomeFarmDef,
										HOME_FARM_DEF_DATA, MAX_HOME_FARM_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iHomeFarmDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sHomeFarmDef[0], pstEnv->pstZoneObj->iHomeFarmDef, 
					HOME_FARM_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ArmSkillRecastDef", pstEnv->pstZoneObj->sArmSkillRecastDef[0], 
										&pstEnv->pstZoneObj->iArmSkillRecastDef,
										SKILL_ARM_RECAST_DEF_DATA, MAX_SKILL_ARM_RECAST_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iArmSkillRecastDef> 1)
		{
			qsort(pstEnv->pstZoneObj->sArmSkillRecastDef[0], pstEnv->pstZoneObj->iArmSkillRecastDef, 
					SKILL_ARM_RECAST_DEF_DATA, trl_comp_func);
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "HomeSoilDef", pstEnv->pstZoneObj->sHomeSoilDef[0], 
												&pstEnv->pstZoneObj->iHomeSoilDef,
												HOME_SOIL_DEF_DATA, MAX_HOME_SOIL_DEF))
		{
			return -1;
		}
		
		if (pstEnv->pstZoneObj->iHomeSoilDef > 1)
		{
			qsort(pstEnv->pstZoneObj->sHomeSoilDef[0], pstEnv->pstZoneObj->iHomeSoilDef, 
					HOME_SOIL_DEF_DATA, trl_comp_func);
		}
			
		pstEnv->tCheckDB = pstAppCtx->stCurr.tv_sec;
		
	}

	pstEnv->pstZoneObj->iShopGetStat = 0;
	z_load_shopdef_data(pstEnv);
		
	dyn_active_status_init(pstEnv);

	//跑商
	player_paoshang_init(pstEnv);
	return 0;
}

ACHIEVEMENTCATEGORYDEF* z_find_achievement_category_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	ACHIEVEMENTCATEGORYDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.CategoryID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sAchievementCategoryDef, pstObj->iAchievementCategoryDef, ACHIEVEMENT_CATEGORY_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ACHIEVEMENTCATEGORYDEF*)pstObj->sAchievementCategoryDef[iIndex];
		
	}
	return NULL;
}

CLANSPYAWARDDROPDEF* z_find_clan_spy_award_drop_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	CLANSPYAWARDDROPDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.DefID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sClanSpyAwardDropDef, pstObj->iClanSpyAwardDropDef, CLAN_SPY_AWARD_DROP_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (CLANSPYAWARDDROPDEF*)pstObj->sClanSpyAwardDropDef[iIndex];		
	}
	return NULL;
}

CLANSPYTRANSDEF* z_find_clan_spy_trans_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	CLANSPYTRANSDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.DefID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sClanSpyTransDef, pstObj->iClanSpyTransDef, CLAN_SPY_TRANS_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (CLANSPYTRANSDEF*)pstObj->sClanSpyTransDef[iIndex];		
	}
	return NULL;
}

PWORLDWAVEMONDEF* z_find_pworld_wave_mon_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	PWORLDWAVEMONDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.WaveMonDefID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sPworldWaveMonDef, pstObj->iPworldWaveMonDef, PWORLD_WAVE_MON_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (PWORLDWAVEMONDEF*)pstObj->sPworldWaveMonDef[iIndex];		
	}
	return NULL;
}

PWORLDWAVEMONPOSDEF* z_find_pworld_wave_mon_pos_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	PWORLDWAVEMONPOSDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.WaveMonDefID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sPworldWaveMonPosDef, pstObj->iPworldWaveMonPosDef, PWORLD_WAVE_MON_POS_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (PWORLDWAVEMONPOSDEF*)pstObj->sPworldWaveMonPosDef[iIndex];		
	}
	return NULL;
}

PWORLDWAVEMONCLANAWARDDEF* z_find_pworld_wave_mon_clan_award_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	PWORLDWAVEMONCLANAWARDDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.WaveMonDefID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sPworldWaveMonClanAwardDef, pstObj->iPworldWaveMonClanAwardDef, PWORLD_WAVE_MON_CLAN_AWARD_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (PWORLDWAVEMONCLANAWARDDEF*)pstObj->sPworldWaveMonClanAwardDef[iIndex];		
	}
	return NULL;
}

PWORLDMACHINEBUFFSHOPDEF* z_find_pworld_machine_buff_shop_def(ZONESVRENV *pstEnv, int iID,int iLevel)
{
	int iIndex;
	int iEqu;
	PWORLDMACHINEBUFFSHOPDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.StatusID = iID;
	stDef.Level = iLevel;
	iIndex = bsearch_int(&stDef, pstObj->sPworldMachineBuffShopDef, pstObj->iPworldMachineBuffShopDef, PWORLD_MACHINE_BUFF_SHOP_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (PWORLDMACHINEBUFFSHOPDEF*)pstObj->sPworldMachineBuffShopDef[iIndex];		
	}
	return NULL;
}

PLAYERAWARDDEF* z_find_player_award_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	PLAYERAWARDDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.PlayerAwardDefID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sPlayerAwardDef, pstObj->iPlayerAwardDef, PLAYER_AWARD_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (PLAYERAWARDDEF*)pstObj->sPlayerAwardDef[iIndex];		
	}
	return NULL;
}



COWFINALAWARDDEF* z_find_cow_final_award_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	COWFINALAWARDDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.CowFinalAwardDefID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sCowFinalAwardDef, pstObj->iCowFinalAwardDef, COW_FINAL_AWARD_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (COWFINALAWARDDEF*)pstObj->sCowFinalAwardDef[iIndex];		
	}
	return NULL;
}

HANGHAIAWARDDEF* z_find_hang_hai_award_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	HANGHAIAWARDDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.HangHaiAwardDefID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sHangHaiAwardDef, pstObj->iHangHaiAwardDef, HANG_HAI_AWARD_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (HANGHAIAWARDDEF *)pstObj->sHangHaiAwardDef[iIndex];		
	}
	return NULL;
}

CLANSKILLDEF* z_find_clan_skill_def(ZONESVRENV *pstEnv, int iID,int iLevel)
{
	int iIndex;
	int iEqu;
	CLANSKILLDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.SkillID = iID;
	stDef.Level = iLevel;
	iIndex = bsearch_int(&stDef, pstObj->sClanSkillDef, pstObj->iClanSkillDef, CLAN_SKILL_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (CLANSKILLDEF*)pstObj->sClanSkillDef[iIndex];		
	}
	return NULL;
}

CLANPAYDEF* z_find_clan_pay_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	CLANPAYDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.KeepTimes = min(pstObj->iClanPayDef,iID);
	iIndex = bsearch_int(&stDef, pstObj->sClanPayDef, pstObj->iClanPayDef, CLAN_PAY_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (CLANPAYDEF*)pstObj->sClanPayDef[iIndex];		
	}
	return NULL;
}

PETLOVELEVELDEF* z_find_pet_love_level_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	PETLOVELEVELDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.LoveLevel = iID;
	iIndex = bsearch_int(&stDef, pstObj->sPetLoveLevelDef, pstObj->iPetLoveLevelDef, PET_LOVE_LEVEL_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (PETLOVELEVELDEF*)pstObj->sPetLoveLevelDef[iIndex];		
	}
	return NULL;
}

LUCKYMAPDEF* z_find_lucky_map_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	LUCKYMAPDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.ID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sLuckyMapDef, pstObj->iLuckyMapDef, LUCKY_MAP_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (LUCKYMAPDEF*)pstObj->sLuckyMapDef[iIndex];		
	}
	return NULL;
}

CLANAWARDDEF* z_find_clan_award_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	CLANAWARDDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.ClanAwardDefID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sClanAwardDef, pstObj->iClanAwardDef, CLAN_AWARD_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (CLANAWARDDEF*)pstObj->sClanAwardDef[iIndex];		
	}
	return NULL;
}

CLANWARAWARDDEF* z_find_clan_war_award_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	CLANWARAWARDDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.ClanWarAwardDefID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sClanWarAwardDef, pstObj->iClanWarAwardDef, CLAN_WAR_AWARD_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (CLANWARAWARDDEF*)pstObj->sClanWarAwardDef[iIndex];		
	}
	return NULL;
}

ACHIEVEMENTDEF* z_find_achievement_def(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	ACHIEVEMENTDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.AchievementID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sAchievementDef, pstObj->iAchievementDef, ACHIEVEMENT_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ACHIEVEMENTDEF*)pstObj->sAchievementDef[iIndex];
		
	}
	return NULL;
}


DAILYGOALAWARD* z_find_daily_goal_award(ZONESVRENV *pstEnv, int iID)
{
	int iIndex;
	int iEqu;
	DAILYGOALAWARD stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.AwardID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sDailyGoalAward, pstObj->iDailyGoalAward, DAILY_GOAL_AWARD_DATA, &iEqu);
	if (iEqu)
	{
		return (DAILYGOALAWARD*)pstObj->sDailyGoalAward[iIndex];
		
	}
	return NULL;
}


//清除MapIndex 以外的其他变量
int z_init_mapobj(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	UNUSED(pstAppCtx);
	pstEnv->pstMapObj->tPworldDef = 0;
	pstEnv->pstMapObj->iPworld = 0;
	memset(pstEnv->pstMapObj->sPworld, 0, sizeof(pstEnv->pstMapObj->sPworld));
	pstEnv->pstMapObj->tMapMonsterDef = 0;
	pstEnv->pstMapObj->iMapMonsterDef = 0;
	pstEnv->pstMapObj->tMapNpcDef = 0;
	pstEnv->pstMapObj->iMapNpcDef = 0;
	pstEnv->pstMapObj->tProcDef = 0;
	pstEnv->pstMapObj->iProcDef = 0;
	pstEnv->pstMapObj->iMapInst = 0;
	
	return 0;
}

int z_get_mem_id(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iIndex, char cType)
{
	int *piMemID;
	IDIndex *pII;
	UNUSED(pstAppCtx);
	
	piMemID = &pstEnv->pstZoneObj->iMemID;
	while (1) {
		++(*piMemID);
		if ((*piMemID) <= 0 || (*piMemID) >= ID_INDEX)
		{
			*piMemID = 1;
		}
		
		pII = &pstEnv->pstZoneObj->stIndex[(*piMemID)];
		if (pII->iID == 0) {
			pII->iID = *piMemID;
			pII->iIndex = iIndex;
			pII->cType = cType;
			break;
		}
	}
	return *piMemID;
}

int z_id_index(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iID, char cType)
{
	IDIndex *pII;
	ZoneObj*pstObj = pstEnv->pstZoneObj;
	UNUSED(pstAppCtx);
	
	if (iID <= 0)
		return -1;

	pII = &pstObj->stIndex[iID % ID_INDEX];
	if (pII->iID == iID && pII->cType == cType)
		return pII->iIndex;
	
	return -1;
}

int z_id_clean(ZONESVRENV* pstEnv, int iID)
{
	ZoneObj *pstObj;
	IDIndex *pII;
	int iRet;

	if( iID<=0 )
		return -1;

	pstObj	=	pstEnv->pstZoneObj;

	pII = &pstObj->stIndex[iID % ID_INDEX];

	if (pII->iID == iID)
	{
		pII->iID = 0;
		iRet	=	0;
	}
	else
	{
		iRet	=	-1;
	}

	return iRet;
}

int z_id_indextype(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iID, char *cType)
{
	IDIndex *pII;
	ZoneObj*pstObj = pstEnv->pstZoneObj;
	int iRet;
	UNUSED(pstAppCtx);

	
	if (iID <= 0)
	{
		tlog_info(pstEnv->pstLogCat, 0, 0, "ID %d", iID);
		return -1;
	}

	pII = &pstObj->stIndex[iID % ID_INDEX];

	if (pII->iID != iID)
	{
		//tlog_error(pstEnv->pstLogCat, 0, 0, "ID %d", iID);
		return -1;
	}

	iRet	=	0;

	switch( pII->cType )
	{
		case OBJ_PLAYER:
			if( pII->iIndex<0)
				iRet	=	-1;
			break;
		case OBJ_MONSTER:
			if( pII->iIndex<0)
				iRet	=	-1;
			break;
		case OBJ_NPC:
			if( pII->iIndex<0)
				iRet	=	-1;
			break;
		case OBJ_ITEM:
			if( pII->iIndex<0)
				iRet	=	-1;
			break;
		default:
			iRet = -1;
			break;
	}

	if( iRet<0 )
	{
		pII->iID	=	0;
		tlog_error(pstEnv->pstLogCat, 0, 0, "ID %d", iID);
		return -1;
	}

	*cType = pII->cType;

	return pII->iIndex;
}

Player* z_id_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iID)
{
	int iIndex;
	TMEMBLOCK *pstMemBlock;
	Player *pstPlayer;

	iIndex = z_id_index(pstAppCtx, pstEnv, iID, OBJ_PLAYER);
	if (iIndex < 0) return NULL;

	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
	if (!pstMemBlock->fValid)
		return NULL;
	
	pstPlayer = (Player *)pstMemBlock->szData;
	
	if (pstPlayer->iMemID != iID) return NULL;
	else return pstPlayer;
}

Npc* z_id_npc(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iID)
{
	int iIndex;
	TMEMBLOCK *pstMemBlock;
	Npc *pstNpc;

	iIndex = z_id_index(pstAppCtx, pstEnv, iID, OBJ_NPC);
	if (iIndex < 0) return NULL;

	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
	if (!pstMemBlock->fValid)
		return NULL;
	
	pstNpc = (Npc *)pstMemBlock->szData;
	
	if (pstNpc->stMon.iID != iID) return NULL;
	else return pstNpc;
}

Monster* z_id_monster2(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iID)
{
	int iIndex;
	TMEMBLOCK *pstMemBlock;
	Monster *pstMon;
	char cType;

	iIndex = z_id_indextype(pstAppCtx, pstEnv, iID, &cType);
	if (iIndex < 0) return NULL;

	if (cType == OBJ_MONSTER)
	{
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
		if (!pstMemBlock->fValid)
			return NULL;
		
		pstMon = (Monster *)pstMemBlock->szData;
	}
	else if (cType == OBJ_NPC)
	{
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
		if (!pstMemBlock->fValid)
			return NULL;
		
		pstMon = &((Npc *)pstMemBlock->szData)->stMon;
	}
	else
	{
		return NULL;
	}

	if (pstMon->iID != iID) return NULL;
	else return pstMon;
}

Monster* z_id_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iID)
{
	int iIndex;
	TMEMBLOCK *pstMemBlock;
	Monster *pstMon;

	iIndex = z_id_index(pstAppCtx, pstEnv, iID, OBJ_MONSTER);
	if (iIndex < 0) return NULL;

	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
	if (!pstMemBlock->fValid)
		return NULL;
	
	pstMon = (Monster *)pstMemBlock->szData;
	
	if (pstMon->iID != iID) return NULL;
	else return pstMon;
}

DropItem* z_id_dropitem(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iID)
{
	int iIndex;
	TMEMBLOCK *pstMemBlock;
	DropItem *pstItem;

	iIndex = z_id_index(pstAppCtx, pstEnv, iID, OBJ_ITEM);
	if (iIndex < 0) return NULL;

	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstItemPool, iIndex);
	if (!pstMemBlock->fValid)
		return NULL;
	
	pstItem = (DropItem *)pstMemBlock->szData;
	
	if (pstItem->iID != iID) return NULL;
	else return pstItem;
}

int z_check_id_index(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	IDIndex *pII;
	Player* pstPlayer;
	Monster* pstMon;
	Npc* pstNpc;
	DropItem *pstItem;
	TMEMBLOCK *pstMemBlock;
	ZoneObj* pstObj = pstEnv->pstZoneObj;
		
	int i;
	int iPlayer=0;
	int iMon=0;
	int iNpc=0;
	int iItem=0;
	UNUSED(pstAppCtx);
	
	for(i=0; i<ID_INDEX; i++)
	{
		pII = &pstObj->stIndex[i];

		if( !pII->iID )
			continue;

		switch( pII->cType )
		{
			case OBJ_PLAYER:
				if( pII->iIndex<0)
				{
					iPlayer++;
					pII->iID	=	0;
					break;
				}

				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, pII->iIndex);
				if (!pstMemBlock->fValid)
				{
					iPlayer++;
					pII->iID	=	0;
					break;
				}

				pstPlayer	=	(Player*) pstMemBlock->szData;

				if( pstPlayer->iMemID!= pII->iID )
				{
					iPlayer++;
					pII->iID	=	0;
					break;
				}

				break;

			case OBJ_MONSTER:
				if( pII->iIndex<0)
				{
					iMon++;
					pII->iID	=	0;
					break;
				}

				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, pII->iIndex);
				if (!pstMemBlock->fValid)
				{
					iMon++;
					pII->iID	=	0;
					break;
				}

				pstMon = (Monster *)pstMemBlock->szData;

				if( pstMon->iID != pII->iID )
				{
					iMon++;
					pII->iID	=	0;
					break;
				}

				break;

			case OBJ_NPC:
				if( pII->iIndex<0)
				{
					iNpc++;
					pII->iID	=	0;
					break;
				}

				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, pII->iIndex);
				if (!pstMemBlock->fValid)
				{
					iNpc++;
					pII->iID	=	0;
					break;
				}

				pstNpc = (Npc *)pstMemBlock->szData;

				if( pstNpc->stMon.iID != pII->iID )
				{
					iNpc++;
					pII->iID	=	0;
					break;
				}

				break;

			case OBJ_ITEM:
				if( pII->iIndex<0)
				{
					iItem++;
					pII->iID	=	0;
					break;
				}

				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstItemPool, pII->iIndex);
				if (!pstMemBlock->fValid)
				{
					iItem++;
					pII->iID	=	0;
					break;
				}

				pstItem = (DropItem *)pstMemBlock->szData;

				if( pstItem->iID != pII->iID )
				{
					iItem++;
					pII->iID	=	0;
					break;
				}

				break;	

			default:
				break;
		}
	}

	printf("CheckIDIndex: Player:%d Mon:%d Npc:%d Item:%d\n", iPlayer, iMon, iNpc, iItem);
	tlog_info(pstEnv->pstLogCat, 0, 0, "CheckIDIndex: Player:%d Mon:%d Npc:%d Item:%d", iPlayer, iMon, iNpc, iItem);

	if (iPlayer > 0 || iMon > 0 || iNpc > 0 || iItem > 0)
	{
		char szNotify[256];

		snprintf(szNotify, sizeof(szNotify), "CheckIDIndex: Player:%d Mon:%d Npc:%d Item:%d", iPlayer, iMon, iNpc, iItem);
		z_game_notify(pstEnv, szNotify, CCH(szNotify));
	}

	return 0;
}


int z_load_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iForce)
{
	int iShmID;
	int iReload;
	int iIsExist=0;
		
	iShmID	=	shmget(pstEnv->pstConf->MapShmKey, sizeof(MapObj), 0666 | IPC_CREAT | IPC_EXCL);
	if( iShmID<0 )
	{
		iIsExist	=	1;
		iShmID		=	shmget(pstEnv->pstConf->MapShmKey, sizeof(MapObj), 0666);
	}

	if( iShmID<0 )
		return -1;

	if (pstEnv->pstMapObj)
	{
		shmdt(pstEnv->pstMapObj);
	}

	pstEnv->pstMapObj =	(MapObj *)shmat(iShmID, NULL, 0);
	
	if( (void *) -1  == pstEnv->pstMapObj )
		return -1;

	if (!iIsExist )
	{
		pstEnv->pstMapObj->iVersion = MAP_SHM_VER;
	}
	else
	{
		if (MAP_SHM_VER != pstEnv->pstMapObj->iVersion)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "shm ver=%d", pstEnv->pstMapObj->iVersion);
			return -1;
		}
	}

	if (!iIsExist)
	{
		z_init_mapobj(pstAppCtx, pstEnv);
		pstEnv->pstMapObj->tMapDef = 0;
		pstEnv->pstMapObj->iMapIndex = 0;
		memset(&pstEnv->pstMapObj->sMapIdx, 0, sizeof(pstEnv->pstMapObj->sMapIdx));
		if (0 > tmempool_init(&pstEnv->pstAreaDynPool, MAX_AREA_DYN, AREA_DYN_DATA, 
								pstEnv->pstMapObj->sAreaDyn, sizeof(pstEnv->pstMapObj->sAreaDyn)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_init AreaDyn fail");
			return -1;
		}

		if (0 > tmempool_init(&pstEnv->pstDynMskPool, MAX_DYN_MSK, DYN_MSK_DATA, 
								pstEnv->pstMapObj->sDynMsk, sizeof(pstEnv->pstMapObj->sDynMsk)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_init DynMsk fail");
			return -1;
		}

		if (0 > tmempool_init(&pstEnv->pstMapInstPool, MAX_MAP_INST, MAP_INST_DATA, 
								pstEnv->pstMapObj->sMapInst, sizeof(pstEnv->pstMapObj->sMapInst)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_init map inst fail");
			return -1;
		}

		if (0 > tmempool_init(&pstEnv->pstPworldInstPool, MAX_PWORLD_INST, PWORLD_INST_DATA, 
								pstEnv->pstMapObj->sPworldInst, sizeof(pstEnv->pstMapObj->sPworldInst)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_init pworld inst fail");
			return -1;
		}

	}
	else
	{
		if (0 > tmempool_attach(&pstEnv->pstAreaDynPool, MAX_AREA_DYN, AREA_DYN_DATA, 
								pstEnv->pstMapObj->sAreaDyn, sizeof(pstEnv->pstMapObj->sAreaDyn)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_attach AreaDyn fail");
			return -1;
		}

		if (0 > tmempool_attach(&pstEnv->pstDynMskPool, MAX_DYN_MSK, DYN_MSK_DATA, 
								pstEnv->pstMapObj->sDynMsk, sizeof(pstEnv->pstMapObj->sDynMsk)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_attach DynMsk fail");
			return -1;
		}

		if (0 > tmempool_attach(&pstEnv->pstMapInstPool, MAX_MAP_INST, MAP_INST_DATA, 
								pstEnv->pstMapObj->sMapInst, sizeof(pstEnv->pstMapObj->sMapInst)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_attach map inst fail");
			return -1;
		}

		if (0 > tmempool_attach(&pstEnv->pstPworldInstPool, MAX_PWORLD_INST, PWORLD_INST_DATA, 
								pstEnv->pstMapObj->sPworldInst, sizeof(pstEnv->pstMapObj->sPworldInst)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_attach pworld inst fail");
			return -1;
		}
	}
	
	pstEnv->iMapObjShmID = iShmID;

	if (RES_LOAD_BIN == pstEnv->pstConf->ResloadType)
	{
		if (0 > load_resfile_one(RES_DIR,"PworldDef", pstEnv->pstMapObj->sPworld[0], &pstEnv->pstMapObj->iPworld, 
								&pstEnv->pstMapObj->tPworldDef, PWORLD_DATA, MAX_PWORLD_DEF, &iReload))
		{
			return -1;
		}

		if (0 > z_pre_pworld(pstEnv))
		{
			return -1;
		}
		
		pstEnv->cReladPworld = iReload;

		if (0 > load_resfile_one(RES_DIR, "MapDef", pstEnv->pstMapObj->sMapIdx[0], &pstEnv->pstMapObj->iMapIndex, 
								&pstEnv->pstMapObj->tMapDef, MAP_INDEX_DATA, MAX_MAP_INDEX, &iReload))
		{
			return -1;
		}

		if (0 > z_mapdef_pre(pstEnv))
		{
			return -1;
		}
		
		pstEnv->cReloadMap = iReload;

		if (0 > load_resfile_one(RES_DIR, "MapMonster", pstEnv->pstMapObj->sMapMonsterDef[0], &pstEnv->pstMapObj->iMapMonsterDef, 
								&pstEnv->pstMapObj->tMapMonsterDef, MAPMONSTER_DEF_DATA, MAX_MAPMONSTER_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "MapNpc", pstEnv->pstMapObj->sMapNpcDef[0], &pstEnv->pstMapObj->iMapNpcDef, 
								&pstEnv->pstMapObj->tMapNpcDef, MAPNPC_DEF_DATA, MAX_MAPNPC_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "ProcEvent", pstEnv->pstMapObj->sProcDef[0], &pstEnv->pstMapObj->iProcDef, 
								&pstEnv->pstMapObj->tProcDef, PROC_DEF_DATA, MAX_PROC_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "MapOrePositionDef", pstEnv->pstMapObj->sMapOrePositionDef[0], &pstEnv->pstMapObj->iMapOrePositionDef, 
								&pstEnv->pstMapObj->tMapOrePositionDef, MAP_ORE_POSITION_DEF_DATA, MAX_MAP_ORE_POSITION_DEF, &iReload))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "MapOreDef", pstEnv->pstMapObj->sMapOreDef[0], &pstEnv->pstMapObj->iMapOreDef, 
								&pstEnv->pstMapObj->tMapOreDef, MAP_ORE_DEF_DATA, MAX_MAP_ORE_DEF, &iReload))
		{
			return -1;
		}

		if(0 > z_pre_map_ore(pstEnv))
		{
			return -1;
		}

		if (0 > load_resfile_one(RES_DIR, "MapTmpItemDef", pstEnv->pstMapObj->sMapTmpItemDef[0], &pstEnv->pstMapObj->iMapTmpItemDef, 
								 &pstEnv->pstMapObj->tMapTmpItemDef, MAP_TMP_ITEM_DEF_DATA, MAX_MAP_TMP_ITEM_DEF, &iReload))
		{
			return -1;
		}
	}
	else
	{
		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "PworldDef", pstEnv->pstMapObj->sPworld[0], &pstEnv->pstMapObj->iPworld, 
								PWORLD_DATA, MAX_PWORLD_DEF))
		{
			return -1;
		}
		if (pstEnv->pstMapObj->iPworld > 1)
		{
			qsort(pstEnv->pstMapObj->sPworld[0], pstEnv->pstMapObj->iPworld, PWORLD_DATA, trl_comp_func);
		} 

		if (0 > z_pre_pworld(pstEnv))
		{
			return -1;
		}
		
		pstEnv->cReladPworld = 1;

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "MapDef", pstEnv->pstMapObj->sMapIdx[0], &pstEnv->pstMapObj->iMapIndex, MAP_INDEX_DATA, MAX_MAP_INDEX))
		{
			return -1;
		}
		if (pstEnv->pstMapObj->iMapIndex > 1)
		{
			qsort(pstEnv->pstMapObj->sMapIdx[0], pstEnv->pstMapObj->iMapIndex, MAP_INDEX_DATA, trl_comp_func);
		} 

		if (0 > z_mapdef_pre(pstEnv))
		{
			return -1;
		}

		pstEnv->cReloadMap = 1;

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "MapMonster", pstEnv->pstMapObj->sMapMonsterDef[0], &pstEnv->pstMapObj->iMapMonsterDef, MAPMONSTER_DEF_DATA, MAX_MAPMONSTER_DEF))
		{
			return -1;
		}
		if (pstEnv->pstMapObj->iMapMonsterDef > 1)
		{
			qsort(pstEnv->pstMapObj->sMapMonsterDef[0], pstEnv->pstMapObj->iMapMonsterDef, MAPMONSTER_DEF_DATA, trl_comp_func);
		} 

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "MapNpc", pstEnv->pstMapObj->sMapNpcDef[0], &pstEnv->pstMapObj->iMapNpcDef, MAPNPC_DEF_DATA, MAX_MAPNPC_DEF))
		{
			return -1;
		}
		if (pstEnv->pstMapObj->iMapNpcDef > 1)
		{
			qsort(pstEnv->pstMapObj->sMapNpcDef[0], pstEnv->pstMapObj->iMapNpcDef, MAPNPC_DEF_DATA, trl_comp_func);
		} 

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "ProcEvent", pstEnv->pstMapObj->sProcDef[0], &pstEnv->pstMapObj->iProcDef, PROC_DEF_DATA, MAX_PROC_DEF))
		{
			return -1;
		}

		if (pstEnv->pstMapObj->iProcDef > 1)
		{
			qsort(pstEnv->pstMapObj->sProcDef[0], pstEnv->pstMapObj->iProcDef, PROC_DEF_DATA, trl_comp_func);
		} 

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "MapOrePositionDef", pstEnv->pstMapObj->sMapOrePositionDef[0], 
			&pstEnv->pstMapObj->iMapOrePositionDef, MAP_ORE_POSITION_DEF_DATA, MAX_MAP_ORE_POSITION_DEF))
		{
			return -1;
		}
		if (pstEnv->pstMapObj->iMapOrePositionDef > 1)
		{
			qsort(pstEnv->pstMapObj->sMapOrePositionDef[0], pstEnv->pstMapObj->iMapOrePositionDef, MAP_ORE_POSITION_DEF_DATA, trl_comp_func);
		} 

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "MapOreDef", pstEnv->pstMapObj->sMapOreDef[0], 
			&pstEnv->pstMapObj->iMapOreDef, MAP_ORE_DEF_DATA, MAX_MAP_ORE_DEF))
		{
			return -1;
		}
		if (pstEnv->pstMapObj->iMapOreDef > 1)
		{
			qsort(pstEnv->pstMapObj->sMapOreDef[0], pstEnv->pstMapObj->iMapOreDef, MAP_ORE_DEF_DATA, trl_comp_func);
		} 

		if(0 > z_pre_map_ore(pstEnv))
		{
			return -1;
		}

		if (0 > load_resdb_one(pstEnv->pstResLib, pstEnv->hResDBHandle, "MapTmpItemDef", pstEnv->pstMapObj->sMapTmpItemDef[0], &pstEnv->pstMapObj->iMapTmpItemDef,
								MAP_TMP_ITEM_DEF_DATA, MAX_MAP_TMP_ITEM_DEF))
		{
			return -1;
		}

		if (pstEnv->pstMapObj->iMapTmpItemDef > 1)
		{
			qsort(pstEnv->pstMapObj->sMapTmpItemDef[0], pstEnv->pstMapObj->iMapTmpItemDef, 
					MAP_TMP_ITEM_DEF_DATA, trl_comp_func);
		}
		
	}

	if (iForce)
	{
		pstEnv->cReloadMap = 1;
		pstEnv->cReladPworld = 1;
	}

	return z_init_map(pstAppCtx, pstEnv);
}

int mon_hp_action(ZONESVRENV* pstEnv, Monster *pstMon, int iOldHP, int iApplyID)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	
	pstActionRes->ID = pstMon->iID;
	pstActionRes->ActionID = ACTION_HP_CHG;
	
	memset(&pstActionRes->Data.HPChg, 0, sizeof(pstActionRes->Data.HPChg));

	pstActionRes->Data.HPChg.ApplyID = iApplyID;
	
	pstActionRes->Data.HPChg.ChgHP = pstMon->iHP  - iOldHP;
	pstActionRes->Data.HPChg.CurrHP = pstMon->iHP;
	pstActionRes->Data.HPChg.CurrMaxHP = pstMon->iMaxHP;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_monview(pstEnv->pstAppCtx, pstEnv, pstMon, &stPkg, 0);
	
	return 0;
}

int mon_machine_active(ZONESVRENV* pstEnv, Monster *pstMon, int iActivePos, int iRound)
{
	if (iActivePos % 6 == iRound % 6) //每3秒
	{
		MONMACHINEDEF *pstMonMachineDef;

		pstMonMachineDef = z_find_mon_machine_def(pstEnv, pstMon->iDefIdx);
		if (pstMonMachineDef && pstMonMachineDef->EnergyDec)
		{
			z_machine_attr_chg(pstEnv, pstMon, MON_MACHINE_ATTR_ENERGY, -pstMonMachineDef->EnergyDec);
		}
	}

	if (pstEnv->pstAppCtx->stCurr.tv_sec - pstMon->stLastSearchTv.tv_sec > 1)
	{
		ZONEMAPINST* pstMapInst;
		
		pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
		if (pstMapInst)
		{
		        z_mon_search_view(pstEnv->pstAppCtx, pstEnv, pstMapInst, &pstMon->stCurrPos, pstMon);
		}
	}

	return 0;
}

int mon_map_pos(ZONESVRENV* pstEnv,Monster* pstMon,int iShowType)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;
	ZONEMAPINST * pstMapInst = NULL;
	MONSTERDEF* pstMonDef = NULL;
	

	pstMonDef = z_get_mon_def(pstEnv,pstMon);
	if (!pstMonDef)
	{
		return -1;
	}

	memset(pstAction,0,sizeof(*pstAction));

	pstAction->ID = pstMon->iID;
	pstAction->ActionID = ACTION_MAP_POS;
	
	STRNCPY(pstAction->Data.MapPos.Name,pstMonDef->Name,sizeof(pstAction->Data.MapPos.Name));
	pstAction->Data.MapPos.ShowType = iShowType;
	pstAction->Data.MapPos.Pos = pstMon->stCurrPos;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	

	
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if (pstMapInst)
	{
		map_broadcast(pstEnv->pstAppCtx,pstEnv,pstMapInst,&stPkg);
	}
	return 0;
}

int mon_active(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TMEMBLOCK *pstMemBlock, int iActivePos, int iRound)
{
	Monster *pstMon;
	ZONEMAPINST* pstMapInst = NULL;
	int iHPChg, iOldHP;
	int iTmp;
	int fOldMonInFight = 0;
	ZONEPWORLDINST* pstPworldInst = NULL;
	int iZJret = 0; 

	if (!pstMemBlock->fValid)
	{
		return 0;
	}
	
	pstMon = (Monster *)pstMemBlock->szData;
	fOldMonInFight = GET_MON_IS_FIGHTING(pstMon);
		
	if (pstMon->bDelayScript & MON_SCRIPT_PROC_NEW_MAP)
	{
		char szName[128];
		
		pstMon->bDelayScript &= ~MON_SCRIPT_PROC_NEW_MAP;

		stSEnv.pstMonster = pstMon;
		stSEnv.pstNpc = NULL;
		stSEnv.pstPlayer = NULL;
		pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
		if (pstMapInst)
		{
			pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
		}		
		stSEnv.pstPworldInst = pstPworldInst;
		stSEnv.pstMapinst = NULL;
		stSEnv.pstKiller = NULL;

		snprintf(szName, sizeof(szName), "monster%d.mac", pstMon->iDefIdx);
		szName[sizeof(szName) - 1] = 0;
					
		z_exec_script(pstAppCtx, pstEnv, NULL, szName, &pstMon->stScriptIdx, MON_SCRIPT_PROC_NEW);
		
		return 0;
	}

	if (pstMon->tLifeEnd > 0 && pstMon->cStat !=MONSTER_DIE &&  pstAppCtx->stCurr.tv_sec >= pstMon->tLifeEnd)
	{
		MONSTERDEF *pstMonDef;
		ZoneAni stAni;

		task_fail_by_dartcar_die(pstEnv, pstMon);
		pstMonDef = z_get_mon_def(pstEnv, pstMon);
		if (pstMonDef)
		{
			pstMon->cFirstAtkerType = 0;
			pstMon->ullFirstAtkerWid = 0;
			pstMon->iFirstAtkerID = 0; //没有战利品
			pstMon->iFirstAtkerMemID = 0;
			pstMon->szOwner[0] = 0;
			pstMon->cRefresh = 0;
			stAni.iType = OBJ_UNKNOW;

			//lzk   融焰怪死亡
			if(pstMon->iRongYanTeamCapId > 0)
			{
				Player *pstRongYanPlayer = NULL;
				ZoneTeam *pstRongYanTeam = NULL;
				pstRongYanPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->iRongYanTeamCapId);
				if(NULL == pstRongYanPlayer)
				{
					return -1;
				}
				
				pstRongYanTeam = player_team_get(pstEnv, pstRongYanPlayer);
				if(NULL == pstRongYanTeam)
				{
					return -1;
				}

				pstRongYanTeam->iRongYanCurLevel = -2;
			}
			
			iZJret	= zj_rongyan_mon_if_have_by_wid(pstEnv,pstMon->WID);
			if(iZJret > 0)
			{
				ZJRONGYANGLOBAL *pstZJRongYanGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.ZJRongYanGlobal;
				pstZJRongYanGlobal->ZJRongYan.Level = -2;
			}
			
			z_mon_die(pstAppCtx, pstEnv, pstMon, pstMonDef, &stAni, 1, 1);
		}
		else
		{
			z_free_monster(pstAppCtx, pstEnv, pstMon);
		}
		return 0;
	}

	if (pstMon->cStat == MONSTER_DIE)
	{
		mon_weixing_clear(pstEnv, pstMon);
		if (pstAppCtx->stCurr.tv_sec >= pstMon->tRefresh)
		{
			// 对连锁怪物的处理
			int fLinkedAllDie = 1;
		 	if(pstMon->iLinkedNext >= 0 && pstMon->iLinkedPrev >= 0)
		 	{
				Monster *pstLinkedMon = NULL;

				for(pstLinkedMon = tmempool_get(pstEnv->pstMonsterPool,
				                                pstMon->iLinkedNext);
				    pstLinkedMon != pstMon;
				    pstLinkedMon = tmempool_get(pstEnv->pstMonsterPool,
				                                pstLinkedMon->iLinkedNext))
				{
					if(!pstLinkedMon)
					{
						break;
					}
					
					if(pstLinkedMon->cStat != MONSTER_DIE)					
					{
						fLinkedAllDie = 0;
						break;
					}
				}
			}

			if (pstMon->iFormDefID)
			{
				if (0 > z_mon_resile(pstEnv, pstMon))
				{
					//还原不了就清除掉。
					z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);
					return 0;
				}
			}
			else
			{	// 尝试变异
				if (0 > z_mon_aberrants( pstEnv, pstMon))
				{
					//还原不了就清除掉。
					z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);
					return 0;
				}
			}

			if(fLinkedAllDie)
			{
				z_refresh_mon(pstAppCtx, pstEnv, pstMon,NULL);
			}
		}
		else if (pstMon->tLeaveMap && pstMon->tLeaveMap < pstAppCtx->stCurr.tv_sec)
		{
			pstMon->tLeaveMap = 0;
			z_monster_leave_map(pstAppCtx, pstEnv, pstMon);
		}

		if (pstMon->cMonMiscFlag & MON_MISC_FLAG_MAP_SHOW)
		{
			PworldCampMonInfo  stMonInfo;
			int i;
			//小地图上显示
			stMonInfo.iCamp = pstMon->iCampIdx;
			stMonInfo.iDefID = pstMon->iDefIdx;
			stMonInfo.iHP = 0;
			stMonInfo.iMemID = pstMon->iID;
			stMonInfo.stPos = pstMon->stCurrPos;
			stMonInfo.iDieTime = pstAppCtx->stCurr.tv_sec;
			stMonInfo.iRangePos = 2;//第三名
			
			pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
			if (pstMapInst)
			{
				mon_action_map_show(pstEnv,&stMonInfo,pstMapInst,2,1, NULL);
				pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
			}

			if (pstPworldInst && pstMon->iCampIdx > 0 && 
				pstMon->iCampIdx <= pstPworldInst->stPworldCamp.iCampNum)
			{
				PworldCampPlayerInfo *pstPworldCampInfo;
				for (i = 0; i < pstPworldInst->stPworldCamp.iCampNum;i++)
				{
					if (i == (pstMon->iCampIdx - 1))
					{
						continue;
					}
					pstPworldCampInfo = &pstPworldInst->stPworldCamp.stCampInfo[i];
					if (pstPworldCampInfo->stMapShowMon.iDieTime )
					{
						stMonInfo.iRangePos = 1;//第二名
					}
				}
			}

			if (pstPworldInst && pstMon->iCampIdx > 0 && 
				pstMon->iCampIdx <= pstPworldInst->stPworldCamp.iCampNum)
			{
				PworldCampPlayerInfo *pstPworldCampInfo;
				pstPworldCampInfo = &pstPworldInst->stPworldCamp.stCampInfo[pstMon->iCampIdx - 1];
				if (pstPworldCampInfo->stMapShowMon.iDefID == stMonInfo.iDefID)
				{
					pstPworldCampInfo->stMapShowMon = stMonInfo;
				}
			}
		}
		
		return 0;
	}

	if (pstMon->cMoving)
	{
		z_monster_move_curr(pstAppCtx, pstEnv, pstMon, 1);
	}

	if(pstMon->stAtkTargets.iTargetCur != pstMon->stAtkTargets.iOldTargetID)
	{	
		pstMon->stAtkTargets.iOldTargetID = pstMon->stAtkTargets.iTargetCur;
		mon_target_action(pstEnv, pstMon);
	}

	//被击触发技能，看看状态减了血没
	iOldHP = pstMon->iHP;
	z_check_mon_status(pstAppCtx, pstEnv, pstMon);

	//有可能怪物死了(z_check_mon_status 会对自己放技能)
	if(pstMon->iHP == 0)
	{
		return 0;
	}

	mon_booty_owner_tick(pstEnv, pstMon);

	if (iActivePos % 2 == iRound % 2) //每1秒
	{
		mon_weixing_tick(pstEnv, pstMon);
		//宏福活动
		if (pstMon->cMonMiscFlag & MON_MISC_FLAG_MAP_POS)
		{
			mon_map_pos(pstEnv,pstMon,1);

			if (0 == pstEnv->iLuckyTipCount % 300) //每5分钟		
			{
				ZONEIDX stIdx;
				MapIndex* pstMapIndex;
				MONSTERDEF* pstMonDef;

				stIdx = pstMon->stMap;

				pstMapIndex =	z_mappos_index(pstEnv->pstMapObj, &stIdx.iPos, stIdx.iID);
				pstMonDef = z_get_mon_def(pstEnv,pstMon);
	 			if(pstMapIndex && pstMonDef)
				{			
					z_sys_strf_broadcast(pstEnv,SYS_CHAT, ZONEERR_BROAD344,
						pstMapIndex->stMapDef.Name,pstMonDef->Name);
						
				}

			}
			pstEnv->iLuckyTipCount++;
		}
		else if (pstMon->cMonMiscFlag & MON_MISC_FLAG_MAP_SHOW)
		{
			PworldCampMonInfo  stMonInfo;
			//小地图上显示
			stMonInfo.iCamp = pstMon->iCampIdx;
			stMonInfo.iDefID = pstMon->iDefIdx;
			stMonInfo.iHP = pstMon->iHP;
			stMonInfo.iMemID = pstMon->iID;
			stMonInfo.stPos = pstMon->stCurrPos;

			pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
			if (pstMapInst)
			{
				mon_action_map_show(pstEnv,&stMonInfo,pstMapInst,2,1, NULL);
				pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
			}

			if (pstPworldInst && pstMon->iCampIdx > 0 && 
				pstMon->iCampIdx <= pstPworldInst->stPworldCamp.iCampNum)
			{
				PworldCampPlayerInfo *pstPworldCampInfo;
				pstPworldCampInfo = &pstPworldInst->stPworldCamp.stCampInfo[pstMon->iCampIdx - 1];
			
				if (pstPworldCampInfo->stMapShowMon.iDefID == stMonInfo.iDefID)
				{
					pstPworldCampInfo->stMapShowMon = stMonInfo;
				}
			}
		}
	}

	if (iActivePos % 6 == iRound % 6) //每3秒
	{
		//更新global相关记录
		world_city_build_mon_update(pstEnv, pstMon);
	}
	
	if (pstMon->bAiStat == AI_STAT_HOME)
	{
		return mon_ai_home_process(pstEnv, pstMon);
	}

	//怪物机械无需后续行为
	if(is_mon_machine(pstMon))
	{
		if (pstMon->iPatrolID > 0)
		{
			return mon_ai_tick(pstEnv, pstMon);
		}
		
		return mon_machine_active(pstEnv, pstMon, iActivePos, iRound);
	}

	if(iActivePos % 4 == iRound % 4) //每2秒
	{
		mon_factory_tick(pstEnv, pstMon);
	}
	
	iHPChg = 0;
	if (iActivePos % 6 == iRound % 6) //每3秒
	{	
		mon_ore_update(pstEnv, pstMon);
		
		if (pstMon->cMonScriptFlag & MON_SCRIPT_PROC_TIMER_MAP)
		{
			char szName[128];
			
			//pstMon->bDelayScript &= ~MON_SCRIPT_PROC_TIMER_MAP;

			stSEnv.pstMonster = pstMon;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPlayer = NULL;
			
			pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
			if (pstMapInst)
			{
				pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
			}		
			stSEnv.pstPworldInst = pstPworldInst;
			stSEnv.pstMapinst = NULL;
			stSEnv.pstKiller = NULL;

			snprintf(szName, sizeof(szName), "monster%d.mac", pstMon->iDefIdx);
			szName[sizeof(szName) - 1] = 0;
			
			
			z_exec_script(pstAppCtx, pstEnv, NULL, szName, &pstMon->stScriptIdx, MON_SCRIPT_PROC_TIMER);
			
			if (!pstMemBlock->fValid)
			{
				return 0;
			}
		}

		/*
		// 检查有没有禁止回血的Status
		if(!z_is_mon_status_result(pstEnv, pstMon, STATUS_RESULT_CANNOT_ADDHP))
		{
			z_get_mon_status_result(pstEnv, pstMon, STATUS_RESULT_HPADD, &stResult);
			iHPChg += pstMon->iMaxHP*(stResult.ResultVal1/100.0)+stResult.ResultVal2;
		}

		iVal = z_get_mon_status_result(pstEnv, pstMon, STATUS_RESULT_HPDEC, &stResult);
		iHPChg += (iVal+stResult.ResultVal2);

		*/
	}
	
	if (iActivePos % 2 == iRound % 2) //每1秒
	{
		if (pstMon->cMonScriptFlag & MON_SCRIPT_PROC_TIMER2_MAP)
		{
			char szName[128];
			

			stSEnv.pstMonster = pstMon;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPlayer = NULL;
			
			pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
			if (pstMapInst)
			{
				pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
			}		
			stSEnv.pstPworldInst = pstPworldInst;
			stSEnv.pstMapinst = NULL;
			stSEnv.pstKiller = NULL;

			snprintf(szName, sizeof(szName), "monster%d.mac", pstMon->iDefIdx);
			szName[sizeof(szName) - 1] = 0;
			
			
			z_exec_script(pstAppCtx, pstEnv, NULL, szName, &pstMon->stScriptIdx, MON_SCRIPT_PROC_TIMER2);
			
			if (!pstMemBlock->fValid)
			{
				return 0;
			}
		}
	}
	
	if (pstMon->iHP < iOldHP)
	{
		AIEvt stEvt;

		stEvt.iEvtID = AI_EVT_BEATK;
		stEvt.stEvtDetail.stBeAtk.iHurt = iOldHP - pstMon->iHP;
		stEvt.stEvtDetail.stBeAtk.iSkillPressRate = -100;
		stEvt.stEvtDetail.stBeAtk.stAtker.iType = OBJ_UNKNOW;

		mon_ai_stat(pstEnv, pstMon, &stEvt);
	}
	
	iOldHP = pstMon->iHP;
	iTmp = pstAppCtx->stCurr.tv_sec - pstMon->tLastPressUpdate;

	if(pstMon->cType == OBJ_NPC)
	{
		if(pstMon->bMoveType == MOVE_TYPE_NO)
		{
			//return 0;
		}
	}
	else if(pstMon->iVPlayer <= 0 && pstMon->stAtkTargets.PressNum <= 0  && pstMon->bMoveType != MOVE_TYPE_FORCED)
	{
		//非巡逻怪物看不到人就回家
		if (0 == pstMon->iPatrolID && !is_mon_bub(pstMon))
		{
			if (iTmp > 2 && iTmp < 5 && 
				pstMon->unViewDist > 0 &&
				z_distance(&pstMon->stCurrPos, &pstMon->stInitPos) >= pstMon->unViewDist)
			{
				mon_ai_home(pstEnv, pstMon);
			}
			
			return 0;
		}
	}

	//不可移动的主动怪，不停的搜索视野
	if(pstMon->bMoveType == MOVE_TYPE_NO && pstMon->bAtkType == ATK_TYPE_INITIATIVE)
	{
		pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
		if (pstMapInst)
		{
		        z_mon_search_view(pstAppCtx, pstEnv, pstMapInst, &pstMon->stCurrPos, pstMon);
		}
	}
	else if ( (0 < pstMon->iOwnerID || is_friend_mon(pstMon)) && pstAppCtx->stCurr.tv_sec - pstMon->stLastSearchTv.tv_sec > 0)
	{
		pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
		if (pstMapInst)
		{
		        z_mon_search_view(pstAppCtx, pstEnv, pstMapInst, &pstMon->stCurrPos, pstMon);
		}
	}
	else if(pstAppCtx->stCurr.tv_sec - pstMon->stLastSearchTv.tv_sec > 15)
	{
		pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
		if (pstMapInst)
		{
		        z_mon_search_view(pstAppCtx, pstEnv, pstMapInst, &pstMon->stCurrPos, pstMon);
		}
	}
	
	if (iTmp >= 2)
	{
		mon_press_update_out(pstEnv, pstMon);
	}

	if (z_is_mon_status_result(pstEnv, pstMon, STATUS_RESULT_RANDOM_RUN))
	{
		if (iActivePos % 2 == iRound % 2) //每秒
		{
			z_svr_mon_move(pstEnv, pstMon);
		}
	}
	else
	{
		mon_ai_tick(pstEnv, pstMon);
	}

	if (pstMon->iHP != iOldHP)
	{
		mon_hp_action(pstEnv, pstMon, iOldHP, 0);
	}

	// 战斗计数/清除
	if(GET_MON_IS_FIGHTING(pstMon))
	{
		pstMon->iCombatTimeCount++;
	}
	else
	{
		pstMon->iCombatTimeCount = 0;
	}

	// 从战斗状态进入非战斗状态了
	if(fOldMonInFight && GET_MON_IS_FIGHTING(pstMon) != fOldMonInFight)
	{
		//佣兵不会清除状态
		if(!is_mon_bub(pstMon))
		{
			mon_out_fight_status_clear(pstAppCtx, pstEnv, pstMon);
		}

		memset(pstMon->abFollowAngel, 0, sizeof(pstMon->abFollowAngel));
	}

	return 0;
}

int z_active_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iCount)
{
	static struct timeval stRoundTv;
	static struct timeval stRoundStartTv;
	static unsigned int iActivePos = 0, iRound = 0;
	int i;
	struct timeval stSub;
	long long  llMs;
	TMEMBLOCK *pstMemBlock;

	if (0 == stRoundTv.tv_sec)
	{
		llMs = MONSTER_ACTIVE_TIMER;
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
		
	if (llMs < MONSTER_ACTIVE_TIMER)
	{
		return 0;
	}

	if (0 == iActivePos)
	{
		TV_CLONE(stRoundStartTv, pstAppCtx->stCurr);
	}

	for (i = 0; i < iCount; i++) 
	{
//		int fOldMonInFight = 0;

		if (iActivePos >= MAX_MONSTER) 
		{
			break;
		}
		
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstMonsterPool, iActivePos);
		iActivePos++;

		if (!pstMemBlock->fValid)
		{
			continue;
		}

		mon_active(pstAppCtx, pstEnv, pstMemBlock, iActivePos, iRound);
	}

	if (iActivePos < MAX_MONSTER)
	{
		return 0;
	}
	
	iActivePos = 0;
	iRound++;
	TV_CLONE(stRoundTv, pstAppCtx->stCurr);

	TV_DIFF(stSub, pstAppCtx->stCurr, stRoundStartTv);
	TV_TO_MS(llMs, stSub);
	if (llMs > MONSTER_ACTIVE_TIMER)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "active monster loop %lld ms", llMs);
	}
	
	return 0;
}

int z_check_event_script(ZONESVRENV* pstEnv, GameEventInst *pstEvent)
{
	
	ScriptIndex *pstScriptInd;
	UNUSED(pstEnv);
	
	pstScriptInd = &pstEvent->stScriptIdx;
	if (pstEvent->szScriptName[0] == 0)
	{
		return 0;
	}
	
	if (0 == check_script_proc(pstEvent->szScriptName, pstScriptInd, GAME_EVENT_SCRIPT_PROC_TIMER))
	{
		pstEvent->cScriptFlag |= GAME_EVENT_SCRIPT_PROC_TIMER_MAP;
	}

 	if (0 == check_script_proc(pstEvent->szScriptName, pstScriptInd, GAME_EVENT_SCRIPT_PROC_TIMER2))
	{
		pstEvent->cScriptFlag |= GAME_EVENT_SCRIPT_PROC_TIMER2_MAP;
	}

	if (0 == check_script_proc(pstEvent->szScriptName, pstScriptInd, GAME_EVENT_SCRIPT_PROC_START))
	{
		pstEvent->cScriptFlag |= GAME_EVENT_SCRIPT_PROC_START_MAP;
	}

	if (0 == check_script_proc(pstEvent->szScriptName, pstScriptInd, GAME_EVENT_SCRIPT_PROC_END))
	{
		pstEvent->cScriptFlag |= GAME_EVENT_SCRIPT_PROC_END_MAP;
	}

	if (0 == check_script_proc(pstEvent->szScriptName, pstScriptInd, GAME_EVENT_SCRIPT_PROC_ROLE_LOGIN))
	{
		pstEvent->cScriptFlag |= GAME_EVENT_SCRIPT_PROC_ROLE_LOGIN_MAP;
	}

	if (0 == check_script_proc(pstEvent->szScriptName, pstScriptInd, GAME_EVENT_SCRIPT_PROC_MOTION))
	{
		pstEvent->cScriptFlag |= GAME_EVENT_SCRIPT_PROC_MOTION_MAP;
	}

	if (0 == check_script_proc(pstEvent->szScriptName, pstScriptInd, GAME_EVENT_SCRIPT_PROC_ENTER_MAP))
	{
		pstEvent->cScriptFlag |= GAME_EVENT_SCRIPT_PROC_ENTER_MAP_MAP;
	}

	return 0;
}

int autoteam_check_active_running(ZONESVRENV* pstEnv, ACTIVEHELPER *pstActiveHelper)
{
	int i, iWeekFlag, iLen,iLen2;
	struct tm stCurr;
	int iCurrDate, iCurrTime;
	char sTmp[20];
	int iTimeFlag;
	int iStartTime, iEndTime;

	if(pstActiveHelper->Disabled)
	{
		return -1;
	}

	localtime_r(&pstEnv->pstAppCtx->stCurr.tv_sec, &stCurr);
	strftime( sTmp, sizeof(sTmp), "%Y%m%d", &stCurr);
	iCurrDate = atoi(sTmp);
	
	if (8 == strlen(pstActiveHelper->StartDate) && iCurrDate < atoi(pstActiveHelper->StartDate))
	{
		return -1;
	}

	if (8 == strlen(pstActiveHelper->EndDate) && iCurrDate > atoi(pstActiveHelper->EndDate))
	{
		return -1;
	}

	if (pstActiveHelper->ReCycleType)
	{
		iWeekFlag = 0;
		iLen = strlen(pstActiveHelper->WeekDay);
		for (i=0; i<iLen; i++)
		{
			if (pstActiveHelper->WeekDay[i] == '1')
			{
				iWeekFlag |= (1 << (iLen - 1 - i));
			}
		}

		if( !(iWeekFlag & (1 << (6 - stCurr.tm_wday))) )
		{
			return -1;
		}
	}

	strftime( sTmp, sizeof(sTmp), "%H0%M", &stCurr);
	iCurrTime = atoi(sTmp);

	iTimeFlag = 0;
	for (i=0; i<MAX_ACTIVE_DAY_TIME; i++)
	{
		if (pstActiveHelper->ActiveDayTime[i].StartTime[0] == '-')
		{
			break;
		}

		iStartTime = 0;
		iEndTime = 24000;	
		iLen2 = strlen(pstActiveHelper->ActiveDayTime[i].StartTime);
		if (iLen2 == 4 || iLen2 == 5)
		{
			STRNCPY(sTmp, pstActiveHelper->ActiveDayTime[i].StartTime, sizeof(sTmp));
			if (iLen2 == 4)
			{
				sTmp[1] = '0';
			}
			else
			{
				sTmp[2] = '0';
			}
			iStartTime = atoi(sTmp);
		}
		iLen2 = strlen(pstActiveHelper->ActiveDayTime[i].EndTime);
		if (iLen2 == 4 || iLen2 == 5)
		{
			STRNCPY(sTmp, pstActiveHelper->ActiveDayTime[i].EndTime, sizeof(sTmp));
			if (iLen2 == 4)
			{
				sTmp[1] = '0';
			}
			else
			{
				sTmp[2] = '0';
			}
			iEndTime = atoi(sTmp);
		}
		if (iCurrTime >= iStartTime && iCurrTime < iEndTime)
		{
			iTimeFlag = 1;
			break;
		}
	}

	if (0 == iTimeFlag)
	{
		return -1;
	}
	
	return iStartTime;
}

int z_event_check_active_runing(ZONESVRENV* pstEnv, GAMEEVENTDEF *pstEventDef)
{ 
	struct tm stnow;
	time_t tStartTime = -1;

	localtime_r(&pstEnv->pstAppCtx->stCurr.tv_sec, &stnow);

	if(pstEventDef->Disabled)
	{
		return -1;
	}
	
	//采用活动助手时间
	if(pstEventDef->ReferActiveID > 0)
	{
		ACTIVEHELPER * pstActiveHelper;
		int iActiveStartTime;
		
		pstActiveHelper = z_find_active_help_def(pstEnv, pstEventDef->ReferActiveID, NULL);
		if(pstActiveHelper == NULL)
		{
			return -1;
		}

		iActiveStartTime = autoteam_check_active_running(pstEnv, pstActiveHelper);
		//如果活动开启，返回开启时间 格式11030 -> 11:30
		if(iActiveStartTime < 0)
		{
			return -1;
		}

		//开始时间
		stnow.tm_hour = iActiveStartTime/1000;
		stnow.tm_min = iActiveStartTime%1000;
		stnow.tm_sec = 0;
		tStartTime = mktime(&stnow);
	}
	else
	{
		int iweekday;
		time_t tEndTime;
		int iCurrDate;
		char sTmp[20];
		
		strftime(sTmp, sizeof(sTmp), "%Y%m%d", &stnow);
		iCurrDate = atoi(sTmp);
		
		if (8 == strlen(pstEventDef->StartDate) && iCurrDate < atoi(pstEventDef->StartDate))
		{
			return -1;
		}

		if (8 == strlen(pstEventDef->EndDate) && iCurrDate > atoi(pstEventDef->EndDate))
		{
			return -1;
		}
			
		//月份限制
		if (pstEventDef->EventMonth != 0 && stnow.tm_mon+1 != pstEventDef->EventMonth)
		{
			return -1;;
		}

		//日期限制
		if (pstEventDef->EventDay != 0 && stnow.tm_mday != pstEventDef->EventDay)
		{
			return -1;
		}

		//星期限制tm_wday 0~6 
		if (stnow.tm_wday == 0)
		{
			iweekday = 7;
		}
		else
		{
			iweekday = stnow.tm_wday;
		}
		
		if (pstEventDef->EventWeekDay != 0 && iweekday != pstEventDef->EventWeekDay)
		{
			return -1;;
		}

		//开始时间
		stnow.tm_hour =pstEventDef->EventHour;
		stnow.tm_min = pstEventDef->EventMinute;
		stnow.tm_sec = 0;
		tStartTime = mktime(&stnow);

		if(pstEventDef->EventHour > 0)
		{
			//结束时间
			tEndTime = tStartTime+ pstEventDef->EventDuration*60;
			
			//	判断是否在事件存在时间内
			if (pstEnv->pstAppCtx->stCurr.tv_sec < tStartTime ||
				pstEnv->pstAppCtx->stCurr.tv_sec >= tEndTime)
			{
				return -1;
			}
		}
	}

	return tStartTime;
}

int z_event_start(ZONESVRENV* pstEnv, GAMEEVENTDEF *pstEventDef, int iStartTime)
{
	GameEventInst stEventInst, *pstEvent;
	ScriptIndex *pstScriptInd;

	stEventInst.iEventID = pstEventDef->EventID;
	pstEvent = sht_insert_unique(pstEnv->pstShtEvent, &stEventInst, EventHashCmp, EventHashCode);
	if (!pstEvent)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "event %d insert hash fail", pstEventDef->EventID);
		return -1;;
	}

	bzero(pstEvent, sizeof(*pstEvent));
	pstEvent->bStat = GAME_EVENT_START;
	pstEvent->iEventID = pstEventDef->EventID;
	pstEvent->tEventStart = iStartTime;
	pstEvent->iFlag = 0;

	//事件脚本
	STRNCPY(pstEvent->szScriptName, pstEventDef->EventScriptName, sizeof(pstEvent->szScriptName));
	trim(pstEvent->szScriptName);

	if (pstEvent->szScriptName[0] != 0)
	{
		pstScriptInd = get_script_ind(pstEvent->szScriptName);
		if (!pstScriptInd)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "event %d script %s cant be available", pstEventDef->EventID, pstEventDef->EventScriptName);
			return -1;
		}
		memcpy(&pstEvent->stScriptIdx, pstScriptInd, sizeof(pstEvent->stScriptIdx));
	}

	z_check_event_script(pstEnv, pstEvent);

	return 0;
}

int z_event_def_check(ZONESVRENV* pstEnv)
{
	int i;
	time_t tStartTime;
	GAMEEVENTDEF *pstEventDef;
	GameEventInst stEventInst, *pstEvent;
	SUBJECT *pstSubJect = &pstEnv->pstZoneObj->stSubJect;
	
	
	for(i = 0; i<pstEnv->pstZoneObj->iGameEventDef; i++)
	{
		pstEventDef = (GAMEEVENTDEF *)pstEnv->pstZoneObj->sGameEventDef[i];

		tStartTime = z_event_check_active_runing(pstEnv, pstEventDef);
		
		stEventInst.iEventID = pstEventDef->EventID;
		pstEvent = sht_find(pstEnv->pstShtEvent, &stEventInst, EventHashCmp, EventHashCode);
		if(pstEvent)
		{
			if(tStartTime < 0 /*|| pstEvent->tEventStart != tStartTime*/)
			{
				// 答题活动题目出完才结束
				if (pstEventDef->EventID != SUBJECT_EVENT_ID)
				{
					z_game_event_del (pstEnv, pstEvent);
					continue;
				}
			}
		}
		else
		{
			if(pstEventDef->Disabled)
			{
				continue;
			}

			if (pstEventDef->FyLvl > 0 && !pstEnv->pstConf->NoLevelFeng)
			{
				int iFyLvl = level_get_curr_fy(pstEnv);
				
				if (iFyLvl < 0 || iFyLvl <= pstEventDef->FyLvl)
				{
					continue;
				}
			}
			
			if(tStartTime > 0)
			{
				z_event_start(pstEnv, pstEventDef, tStartTime);
				if (pstEventDef->EventID == SUBJECT_EVENT_ID)
				{
					memset(pstSubJect, 0, sizeof(*pstSubJect));
				}
				continue;
			}
		}
	}

	return 0;
}

int z_game_event_to_xml(ZONESVRENV* pstEnv)
{
	int i;
	int iNetID;
	GameEventInst *pstEvent;
	LPSHITEM pstItem = NULL;
	LPTDRMETA pstMeta = NULL;
	SVREVENT stSvrEvent;
	char szID[64];
	char szFileName[256];
	
	if (sht_is_empty(pstEnv->pstShtEvent))
	{
		return 0;
	}

	stSvrEvent.EventNum = 0;
	for (i = 0; i < pstEnv->pstShtEvent->iMax; i++) 
	{
		
		pstItem = SHT_GET_ITEM(pstEnv->pstShtEvent, i);
		if ( !pstItem->fValid )
		{
	            continue;
	        }
		
		pstEvent = (GameEventInst *)pstItem->szData;

		stSvrEvent.GameEvents[stSvrEvent.EventNum].EventID = pstEvent->iEventID;
		stSvrEvent.GameEvents[stSvrEvent.EventNum].EventEndTime = pstEvent->tEventStart;
		stSvrEvent.GameEvents[stSvrEvent.EventNum].Flag = pstEvent->iFlag;
		stSvrEvent.GameEvents[stSvrEvent.EventNum].PworldInstID = pstEvent->iPworldInstID;
		stSvrEvent.GameEvents[stSvrEvent.EventNum].PworldWID = pstEvent->iPworldWID;
		stSvrEvent.GameEvents[stSvrEvent.EventNum].LuckyRandID = pstEvent->iLuckyRandID;
		
		//事件变量
		memcpy(&stSvrEvent.GameEvents[stSvrEvent.EventNum].EventScriptVar[0] , 
				&pstEvent->EventScriptVar[0], 
				sizeof(stSvrEvent.GameEvents[stSvrEvent.EventNum].EventScriptVar));	
		stSvrEvent.EventNum ++;
	}

	szID[0] = 0;
	GET_WORLD_ENTITY_ID(iNetID, pstEnv->pstAppCtx->iId, FUNC_ZONE_SVR);
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));
	snprintf(szFileName, sizeof(szFileName), "../cfg/game_event_%s.xml", szID);
	
	pstMeta = tdr_get_meta_by_name(pstEnv->pstResLib, "SvrEvent");

	return res_mem_to_xml(pstMeta, szFileName,(char *)&stSvrEvent, sizeof(stSvrEvent));
}

int z_load_event_xml(ZONESVRENV* pstEnv)
{
	int i;
	int iNetID;
	FILE *fp;
	LPTDRMETA pstMeta = NULL;
	TDRDATA stHost;
	SVREVENT stSvrEvent;
	char szID[64];
	char szFilePath[256];
	GameEventInst *pstEvent, stEvent;
	
	szID[0] = 0;
	GET_WORLD_ENTITY_ID(iNetID, pstEnv->pstAppCtx->iId, FUNC_ZONE_SVR);
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) iNetID), sizeof(szID));
	snprintf(szFilePath, sizeof(szFilePath), "../cfg/game_event_%s.xml", szID);

	//没备份就不读
	fp = fopen(szFilePath, "r");
	if (NULL == fp)
	{
		return -1;
	}
	fclose(fp);
	
	pstMeta = tdr_get_meta_by_name(pstEnv->pstResLib, "SvrEvent");
		
	stHost.pszBuff = (char*)&stSvrEvent;
	stHost.iBuff = sizeof(stSvrEvent);

	if( tdr_input_file(pstMeta, &stHost, szFilePath, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME) < 0)
	{
		return -1;
	}

	for (i = 0; i < stSvrEvent.EventNum; i++) 
	{		
		stEvent.iEventID = stSvrEvent.GameEvents[i].EventID;
		pstEvent = sht_find(pstEnv->pstShtEvent, &stEvent, EventHashCmp, EventHashCode);
		if (!pstEvent)
		{
			continue;
		}

		if (pstEvent->tEventStart != stSvrEvent.GameEvents[i].EventEndTime)
		{
			continue;
		}

		memcpy(&pstEvent->EventScriptVar[0], 
				&stSvrEvent.GameEvents[i].EventScriptVar[0], 
				sizeof(pstEvent->EventScriptVar));	
		
		pstEvent->iFlag = stSvrEvent.GameEvents[i].Flag;
		pstEvent->iPworldInstID = stSvrEvent.GameEvents[i].PworldInstID;
		pstEvent->iPworldWID = stSvrEvent.GameEvents[i].PworldWID;
		pstEvent->iLuckyRandID = stSvrEvent.GameEvents[i].LuckyRandID;
		
	}

	
	return 0;
}

int z_game_notify(ZONESVRENV* pstEnv, char *pstStr, int iLen)
{
	FILE *fp;
	char szID[64];
	char szFilePath[128];
	char szBuff[512];
	int iRegionID = pstEnv->ucRegionID;

	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) pstEnv->pstAppCtx->iId), sizeof(szID));
	snprintf(szFilePath, sizeof(szFilePath), "../cfg/game_notify_%s.txt", szID);

	//没备份就不读
	fp = fopen(szFilePath, "a");
	if (NULL == fp)
	{
		return -1;
	}

	//防范一下
	pstStr[iLen-1] = 0;
	
	snprintf(szBuff, CCH(szBuff), "%s\t%d\t%u\t%s\n", 
		time2string(pstEnv->pstAppCtx->stCurr.tv_sec), iRegionID, (unsigned int)pstEnv->unWorldID, pstStr);
	fwrite(szBuff, strlen(szBuff), 1, fp);
	
	fclose(fp);
	return 0;
}

int z_load_global_xml(ZONESVRENV* pstEnv)
{
	FILE *fp;
	char szID[64];
	char szFilePath[256];
	LPTDRMETA pstMeta = NULL;
	TDRDATA stHost;
	GLOBAL stGlobal;
	
	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) pstEnv->pstAppCtx->iId), sizeof(szID));
	snprintf(szFilePath, sizeof(szFilePath), "../cfg/global_%s.xml", szID);

	//没备份就不读
	fp = fopen(szFilePath, "r");
	if (NULL == fp)
	{
		memset(pstEnv->pstGlobalObj, 0, sizeof(*pstEnv->pstGlobalObj));
		goto _final;
	}
	fclose(fp);
	
	pstMeta = tdr_get_meta_by_name(pstEnv->pstResLib, "Global");
		
	stHost.pszBuff = (char*)&stGlobal;
	stHost.iBuff = sizeof(stGlobal);

	if( tdr_input_file(pstMeta, &stHost, szFilePath, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME) < 0)
	{
		memset(pstEnv->pstGlobalObj, 0, sizeof(*pstEnv->pstGlobalObj));
		goto _final;
	}

	
	memcpy(&pstEnv->pstGlobalObj->stGlobal, &stGlobal, sizeof(pstEnv->pstGlobalObj->stGlobal));

_final:
	
	pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat = GLOBAL_STAT_FINI;

	global_world_limit_update(pstEnv);
	global_world_ore_refresh(pstEnv);
	
	return 0;
}

GameEventInst * z_get_game_event(ZONESVRENV* pstEnv,int iEventID)
{
	int i = 0;
	for (i = 0; i < pstEnv->pstShtEvent->iMax; i++) 
	{
		GameEventInst * pstEvent = NULL;
		SHITEM *pstItem = SHT_GET_ITEM(stSEnv.pstEnv->pstShtEvent, i);
		if ( !pstItem->fValid )
		{
				continue;
		}
		
		pstEvent = (GameEventInst *)pstItem->szData;

		if (iEventID == pstEvent->iEventID)
		{
			return pstEvent;
		}
	}

	return NULL;
}

int z_global_to_xml(ZONESVRENV* pstEnv)
{
	LPTDRMETA pstMeta = NULL;
	char szID[64];
	char szFileName[256];
	
	if(!is_fini_global(pstEnv))
	{
		return 0;
	}

	szID[0] = 0;
	STRNCPY(szID, tbus_addr_ntoa((TBUSADDR) pstEnv->pstAppCtx->iId), sizeof(szID));
	snprintf(szFileName, sizeof(szFileName), "../cfg/global_%s.xml", szID);
	
	pstMeta = tdr_get_meta_by_name(pstEnv->pstResLib, "Global");

	return res_mem_to_xml(pstMeta, szFileName,
		(char *)&pstEnv->pstGlobalObj->stGlobal, sizeof(pstEnv->pstGlobalObj->stGlobal));
}

int z_world_event_clear(ZONESVRENV* pstEnv, int iEventID)
{
	int i, j;
	WorldEvent *pstWorldEvent = pstEnv->pstWorldEvent;
	Player *pstPlayer = NULL;
	TMEMBLOCK *pstMemBlock;
	ZONEPWORLD *pstPworld;
	ZONEIDX stIdx;
	ZONEPWORLDINST *pstTmpPworldInst;
	PworldApplyQueue *pstApplyQueue;
	BATTLEDEF *pstBattleDef;

	//事件相关状态
	for(i = pstWorldEvent->iStatusNum-1; i>=0; i--)
	{
		if(pstWorldEvent->stStatus[i].EventIdx != iEventID)
		{
			continue;
		}

		for (j=0; j<MAX_PLAYER; j++)
		{
			pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(stSEnv.pstEnv->pstPlayerPool, j);
			if (!pstMemBlock->fValid)
			{
				continue;
			}

			pstPlayer = (Player *)pstMemBlock->szData;
			//if (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus || pstPlayer->stOnline.bConnLost || pstPlayer->stOnline.bWaitRoleLogout)
			if (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus )
			{
				continue;
			}

			z_player_del_status_level( stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, 
				pstWorldEvent->stStatus[i].iStatusID, pstWorldEvent->stStatus[i].iLevel, 1, 0);
		}

		pstWorldEvent->iStatusNum--;
		if(i !=pstWorldEvent->iStatusNum)
		{
			memcpy(&pstWorldEvent->stStatus[i], 
						&pstWorldEvent->stStatus[pstWorldEvent->iStatusNum], sizeof(pstWorldEvent->stStatus[0]));
		}
	}

	//事件相关公告
	for(i = pstWorldEvent->iBulletinNum-1; i>=0; i--)
	{
		if(pstWorldEvent->stBulletin[i].EventIdx != iEventID)
		{
			continue;
		}
		
		z_del_bulletin(pstEnv, pstWorldEvent->stBulletin[i].iBulletinID);

		pstWorldEvent->iBulletinNum--;
		if(i !=pstWorldEvent->iBulletinNum)
		{
			memcpy(&pstWorldEvent->stBulletin[i], 
						&pstWorldEvent->stBulletin[pstWorldEvent->iBulletinNum], sizeof(pstWorldEvent->stBulletin[0]));
		}
	}


	//事件活动副本相关
	for(i = pstWorldEvent->iPworldNum-1; i>=0; i--)
	{
		if(pstWorldEvent->stPworld[i].EventIdx != iEventID)
		{
			continue;
		}

		stIdx.iID = pstWorldEvent->stPworld[i].iPworldID;
		stIdx.iPos = -1;
		
		pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
		if (NULL == pstPworld)
		{
			continue;
		}

		if(pstPworld->stPworldDef.PolyType == PWORLD_POLY_DOUBLE_MONSTER || 
			pstPworld->stPworldDef.PolyType == PWORLD_POLY_MACHINE_WAR ||
			pstPworld->stPworldDef.PolyType == PWORLD_POLY_TANK)
		{
			int iPworldInstID = pstPworld->iPworldInstHead;
			
			while(iPworldInstID > 0)
			{
				pstTmpPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, iPworldInstID);
				if (NULL == pstTmpPworldInst)
				{
					break;
				}

				if(pstTmpPworldInst->bOver == 0)
				{
					poly_pworld_end(pstEnv, pstTmpPworldInst);
				}

				iPworldInstID = pstTmpPworldInst->iNextPworldInstID;
			}
		}
		else
		{
			while(pstPworld->iPworldInstHead>0)
			{

				pstTmpPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstPworld->iPworldInstHead);
				if(pstTmpPworldInst == NULL)
				{
					//出问题了就归零
					pstPworld->iPworldInstHead = 0;
					pstPworld->iPworldInstNum = 0;
					
					tlog_error(pstEnv->pstLogCat, 0, 0, "PLOY Pworld %d error", pstPworld->iPworldInstHead);
					break;
				}

				z_in_destroy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstTmpPworldInst);
			}
		}
		
		pstWorldEvent->iPworldNum--;
		if (i !=pstWorldEvent->iPworldNum)
		{
			memcpy(&pstWorldEvent->stPworld[i], 
						&pstWorldEvent->stPworld[pstWorldEvent->iPworldNum], sizeof(pstWorldEvent->stPworld[0]));
		}
	}

	for(i = pstWorldEvent->iQueueNum-1; i>=0; i--)
	{
		if(pstWorldEvent->stPworldQueue[i].EventIdx != iEventID)
		{
			continue;
		}
		
		pstApplyQueue = get_pworld_apply_queue(pstEnv, pstWorldEvent->stPworldQueue[i].iPworldID);
		if(pstApplyQueue)
		{
			z_clear_pworld_apply_queue(pstEnv, pstApplyQueue);
		}

		pstWorldEvent->iQueueNum--;
		if (i !=pstWorldEvent->iQueueNum)
		{
			memcpy(&pstWorldEvent->stPworldQueue[i], 
						&pstWorldEvent->stPworldQueue[pstWorldEvent->iQueueNum], sizeof(pstWorldEvent->stPworldQueue[0]));
		}
	}

	for(i = pstWorldEvent->iBattleNum-1; i>=0; i--)
	{
		if(pstWorldEvent->stBattle[i].EventIdx != iEventID)
		{
			continue;
		}

		pstBattleDef = z_find_battledef(pstEnv, pstWorldEvent->stBattle[i].iBattleD);
		if(pstBattleDef)
		{
			for(j = 0; j<MAX_BATTLE_PWORLD_QUEUE; j++)
			{
				pstApplyQueue = get_pworld_apply_queue(pstEnv, pstBattleDef->BattleQueue[j].PworldID);
				if(pstApplyQueue)
				{
					z_clear_pworld_apply_queue(pstEnv, pstApplyQueue);
				}
			}
		}

		pstWorldEvent->iBattleNum--;
		if (i !=pstWorldEvent->iBattleNum)
		{
			memcpy(&pstWorldEvent->stBattle[i], 
						&pstWorldEvent->stBattle[pstWorldEvent->iBattleNum], sizeof(pstWorldEvent->stBattle[0]));
		}
	}

	return 0;
}

int z_game_event_del(ZONESVRENV* pstEnv, GameEventInst *pstEvent)
{
	GameEventInst stEvent;

	pstEvent->bStat = GAME_EVENT_END;

	//清理事件相关信息
	z_world_event_clear(pstEnv, pstEvent->iEventID);
	
	stSEnv.pstEvent= pstEvent;
	stSEnv.pstMonster = NULL;
	stSEnv.pstNpc = NULL;
	stSEnv.pstPlayer = NULL;
	stSEnv.pstPworldInst = NULL;
	stSEnv.pstMapinst = NULL;
	stSEnv.pstKiller = NULL;
				
	z_exec_script(pstEnv->pstAppCtx, pstEnv, NULL, pstEvent->szScriptName, &pstEvent->stScriptIdx, GAME_EVENT_SCRIPT_PROC_END);

	stEvent.iEventID = pstEvent->iEventID;
	sht_remove(pstEnv->pstShtEvent, &stEvent, EventHashCmp, EventHashCode);

	stSEnv.pstEvent= NULL;
	
	return 0;
}

int z_clear_all_event(ZONESVRENV* pstEnv)
{
	int i;
	GameEventInst *pstEvent;
	LPSHITEM pstItem = NULL;
	
	if (sht_is_empty(pstEnv->pstShtEvent))
	{
		return 0;
	}

	for (i = 0; i < pstEnv->pstShtEvent->iMax; i++) 
	{
		
		pstItem = SHT_GET_ITEM(pstEnv->pstShtEvent, i);
		if ( !pstItem->fValid )
		{
	            continue;
	        }
		
		pstEvent = (GameEventInst *)pstItem->szData;

		z_game_event_del (pstEnv, pstEvent);
	}
	pstEnv->iMiscFlag = 0;
	
	return 0;
}

int z_game_event_role_login(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	GameEventInst *pstEvent;
	LPSHITEM pstItem = NULL;
	
	for (i = 0; i < pstEnv->pstShtEvent->iMax; i++) 
	{
		pstItem = SHT_GET_ITEM(pstEnv->pstShtEvent, i);
		if ( !pstItem->fValid )
		{
	            continue;
	        }
		
		pstEvent = (GameEventInst *)pstItem->szData;

		if (pstEvent->cScriptFlag & GAME_EVENT_SCRIPT_PROC_ROLE_LOGIN_MAP && pstEvent->bStat == GAME_EVENT_RUN)
		{
			stSEnv.pstEvent= pstEvent;
			stSEnv.pstMonster = NULL;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPlayer = pstPlayer;
			stSEnv.pstPworldInst = NULL;
			stSEnv.pstMapinst = NULL;
			stSEnv.pstKiller = NULL;
						
			z_exec_script(pstEnv->pstAppCtx, pstEnv, NULL, pstEvent->szScriptName, &pstEvent->stScriptIdx, GAME_EVENT_SCRIPT_PROC_ROLE_LOGIN);
			
			stSEnv.pstEvent= NULL;
		}
	}

	return 0;
}

int z_active_game_event(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iCount)
{
	static unsigned int iActivePos = 0, iTimeCount = 0;
	static time_t tEventToXmlTime = 0;
	int i;
	GameEventInst *pstEvent;
	LPSHITEM pstItem = NULL;

	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		return 0;
	}
	
	iTimeCount++;

	//检查事件，控制开启和关闭
	z_event_def_check(pstEnv);

	if (tEventToXmlTime == 0)
	{
		tEventToXmlTime = pstAppCtx->stCurr.tv_sec;
	}

	if (tEventToXmlTime < pstAppCtx->stCurr.tv_sec)
	{
		//每5分钟备份实例变量
		z_game_event_to_xml(pstEnv);
		tEventToXmlTime = pstAppCtx->stCurr.tv_sec + EVENT_TO_XML_TIMER;
	}

	if (sht_is_empty(pstEnv->pstShtEvent))
	{
		return 0;
	}

	for (i = 0; i < iCount; i++) 
	{
		if (iActivePos >= (unsigned int)pstEnv->pstShtEvent->iMax) 
		{
			break;
		}
		
		pstItem = SHT_GET_ITEM(pstEnv->pstShtEvent, iActivePos++);
		if ( !pstItem->fValid )
		{
	            continue;
	        }
		
		pstEvent = (GameEventInst *)pstItem->szData;

		if (pstEvent->bStat == GAME_EVENT_START)
		{
			pstEvent->bStat = GAME_EVENT_RUN;

			stSEnv.pstEvent= pstEvent;
			stSEnv.pstMonster = NULL;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPlayer = NULL;
			stSEnv.pstPworldInst = NULL;
			stSEnv.pstMapinst = NULL;
			stSEnv.pstKiller = NULL;
						
			z_exec_script(pstAppCtx, pstEnv, NULL, pstEvent->szScriptName, &pstEvent->stScriptIdx, GAME_EVENT_SCRIPT_PROC_START);
			
			stSEnv.pstEvent= NULL;
			continue;
		}

		if (pstEvent->bStat == GAME_EVENT_RUN)
		{
			subject_check_stat(pstEnv,pstEvent);
		}

		// z_event_def_check 来控制事件的结束
		/*
		if (pstEvent->tEventEnd <= pstAppCtx->stCurr.tv_sec)
		{
			z_game_event_del (pstEnv, pstEvent);
			continue;
		}
		*/

		if (iTimeCount % 10 == 0) //每10秒
		{
			if (pstEvent->cScriptFlag & GAME_EVENT_SCRIPT_PROC_TIMER_MAP && pstEvent->bStat == GAME_EVENT_RUN)
			{
				stSEnv.pstEvent= pstEvent;
				stSEnv.pstMonster = NULL;
				stSEnv.pstNpc = NULL;
				stSEnv.pstPlayer = NULL;
				stSEnv.pstPworldInst = NULL;
				stSEnv.pstMapinst = NULL;
				stSEnv.pstKiller = NULL;
							
				z_exec_script(pstAppCtx, pstEnv, NULL, pstEvent->szScriptName, &pstEvent->stScriptIdx, GAME_EVENT_SCRIPT_PROC_TIMER);
				
				stSEnv.pstEvent= NULL;
				continue;
			}
		}

		if (iTimeCount % 2 == 0) 
		{
			if (pstEvent->cScriptFlag & GAME_EVENT_SCRIPT_PROC_TIMER2_MAP && pstEvent->bStat == GAME_EVENT_RUN)
			{
				stSEnv.pstEvent= pstEvent;
				stSEnv.pstMonster = NULL;
				stSEnv.pstNpc = NULL;
				stSEnv.pstPlayer = NULL;
				stSEnv.pstPworldInst = NULL;
				stSEnv.pstMapinst = NULL;
				stSEnv.pstKiller = NULL;
							
				z_exec_script(pstAppCtx, pstEnv, NULL, pstEvent->szScriptName, &pstEvent->stScriptIdx, GAME_EVENT_SCRIPT_PROC_TIMER2);
				
				stSEnv.pstEvent= NULL;
				continue;
			}
		}
		
	}
	
	if (iActivePos < (unsigned int)pstEnv->pstShtEvent->iMax)
	{
		return 0;
	}
	
	iActivePos = 0;
	
	return 0;
}
/*
int z_gen_image(ZONESVRENV* pstEnv, void* buf, int* len, char *pszCode, int iCodeLen)
{
	GdCache *pstGdCache;
	ZoneObj *pstZoneObj = pstEnv->pstZoneObj;
	
	if (pstZoneObj->iGdCacheHead != pstZoneObj->iGdCacheTail)
	{
		pstGdCache = (GdCache *)pstZoneObj->sGdCache[pstZoneObj->iGdCacheHead];
		pstZoneObj->iGdCacheHead = (pstZoneObj->iGdCacheHead + 1) % MAX_GD_CACHE;

		if (*len < pstGdCache->iLen)
		{
			return -1;
		}
		else
		{
			memcpy(buf, pstGdCache->sData, pstGdCache->iLen);
			* len = pstGdCache->iLen;
			STRNCPY(pszCode, pstGdCache->szStr, iCodeLen);
		}
	}
	else
	{
		if (iCodeLen < GD_CODE_LEN)
		{
			return -1;
		}
		
		ptg_gen_rand_code(pszCode, GD_CODE_LEN);	
		
		return ptg_gen_image(pszCode, GD_CODE_LEN, buf, len);
	}

	return 0;
}
*/
/*
int z_fill_gd_cache(ZONESVRENV* pstEnv, int iCount)
{
	int i;
	int iIdx;
	GdCache *pstGdCache;
	ZoneObj *pstZoneObj = pstEnv->pstZoneObj;

	for (i=0; i<iCount; i++)
	{
		iIdx = (pstZoneObj->iGdCacheTail + 1) % MAX_GD_CACHE;
		if (iIdx == pstZoneObj->iGdCacheHead)
		{
			return 0;
		}
		
		pstGdCache = (GdCache *)pstZoneObj->sGdCache[iIdx];
		ptg_gen_rand_code(pstGdCache->szStr, GD_CODE_LEN);	

		pstGdCache->iLen = sizeof(pstGdCache->sData);
		if (0 == ptg_gen_image(pstGdCache->szStr, GD_CODE_LEN, pstGdCache->sData, &pstGdCache->iLen))
		{
			pstZoneObj->iGdCacheTail = iIdx;
		}
	}
	
	return 0;
}
*/

int item_scr_check(ZONESVRENV* pstEnv, char *pAddr, int iCount, int iUnit)
{
	int i;
	ITEMDEF *pstItemDef;

	for (i=0; i<iCount; i++)
	{
		pstItemDef = (ITEMDEF *)(pAddr + i*iUnit);
		
		z_check_item_script(pstEnv, pstItemDef);
	}

	return 0;
}

int init_item_scr(ZONESVRENV* pstEnv)
{
	
	if (0 > item_scr_check(pstEnv, pstEnv->pstZoneObj->sItemDef[0], pstEnv->pstZoneObj->iItemDef, ITEM_DEF_DATA))
	{
			return -1;
	}
	return 0;
	
}

int task_scr_check(ZONESVRENV* pstEnv, char *pAddr, int iCount, int iUnit)
{
	int i;
	TASKDEF *pstTaskDef;

	for (i=0; i<iCount; i++)
	{
		pstTaskDef = (TASKDEF *)(pAddr + i*iUnit);
		
		z_check_task_script(pstEnv, pstTaskDef);
	}

	return 0;
}

int init_task_scr(ZONESVRENV* pstEnv)
{
	
	if (0 > task_scr_check(pstEnv, pstEnv->pstZoneObj->sTaskDef[0], pstEnv->pstZoneObj->iTaskDef, TASK_DATA))
	{
			return -1;
	}
	return 0;
}

int z_gm_limit_get(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;
	GMIPMACLIMIT stGMIPMacLimit;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;

	memset(&pstEnv->stGmLimt, 0, sizeof(pstEnv->stGmLimt));

	stDBAsyn.iCmd = DB_GMLIMIT_SELECT;
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, "GMIPMacLimit", -1, &stDBAsyn);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = MAX_IP_LIMIT;

	memset(&stGMIPMacLimit, 0, sizeof(stGMIPMacLimit));

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stGMIPMacLimit;
	stMetaData.iBuff = sizeof(stGMIPMacLimit);
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
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_gm_limit_res(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes)
{
	int i;
	GMIPMACLIMIT stGMIPMacLimit;

	for(i = 0; i < pstSelectRes->SelectResult.ResultNum; i++)
	{
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stGMIPMacLimit,
		                            sizeof(stGMIPMacLimit)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "gm limit res err");
			continue;
		}

		memcpy(&pstEnv->stGmLimt.astGmIPMacLimit[pstEnv->stGmLimt.iGMIP++],
			&stGMIPMacLimit, sizeof(pstEnv->stGmLimt.astGmIPMacLimit[0]));
	}

	pstEnv->stGmLimt.iInitFlag = 1;
	
	return 0;
}


int z_init(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	TDRDATA stInput;
	TDRDBMS stDBMS;
	char szErrorMsg[2048]={0};
	unsigned char ucEntity;
				
	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		return -1;
	}

	//memset(pstEnv, 0, sizeof(*pstEnv));
	
	pstEnv->pstAppCtx = pstAppCtx;
	pstEnv->pstConf = (ZONE_SVRCONF *)pstAppCtx->stConfData.pszBuff;

	if (pstAppCtx->iNoLoadConf)
	{
		stInput.pszBuff	=	(char*)pstEnv->pstConf;
		stInput.iBuff	=	(int) sizeof(*(pstEnv->pstConf));

		if( tdr_input_file((LPTDRMETA)pstAppCtx->stConfData.iMeta, &stInput, pstAppCtx->pszConfFile, 0, 0)<0 )
		{
			return -1;
		}
	}

	pstEnv->pstRunStat = (ZONE_SVRRUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
	if (NULL == pstEnv->pstRunStat)
	{
		pstEnv->pstRunStat = &gs_stStat;
	}

	if (0 > tapp_get_category(NULL, (void *)(&pstEnv->pstLogCat)))	
	{		
		printf("tapp_get_category run fail\n");
		return -1;	
	}	

	if (pstAppCtx->iTimer >= 500)
	{
		pstEnv->iUpdateCount = MAX_PLAYER/(pstEnv->pstConf->UpdateInterval*1000.0/pstAppCtx->iTimer) +1;
		printf("update count=%d\n", pstEnv->iUpdateCount);
		tlog_info(pstEnv->pstLogCat, 0, 0, "update count=%d", pstEnv->iUpdateCount);

		pstEnv->iBenchCount = MAX_PLAYER/(BENCH_INTERVAL*1000.0/pstAppCtx->iTimer);
	}
	else
	{
		printf("timer=%d\n", pstAppCtx->iTimer);
		return -1;	
	}

	GET_FUNC_ENTITY(ucEntity, pstAppCtx->iId);
	switch(ucEntity)
	{
		case FUNC_BATTLE_SVR:
			pstEnv->iInstMode = INST_MODE_BATTLE_SVR;
			break;
		case FUNC_ZONE_SVR:
			pstEnv->iInstMode = INST_MODE_ZONE_SVR;
			break;
		default:
			printf("Func ID %d error", ucEntity);
			return -1;
			break;
	}
	
	//计算关联进程的bus id
	GET_WORLD_ENTITY_ID(pstEnv->iZoneConnID, pstAppCtx->iId, FUNC_ZONE_CONN);
	GET_WORLD_ENTITY_ID(pstEnv->iRoleDBID, pstAppCtx->iId, FUNC_ROLE_DB);
	GET_WORLD_ENTITY_ID(pstEnv->iMiscDBID, pstAppCtx->iId, FUNC_MISC_DB);
	GET_WORLD_ENTITY_ID(pstEnv->iZoneCtrlID, pstAppCtx->iId, FUNC_ZONE_CTRL);
	GET_WORLD_ENTITY_ID(pstEnv->iCsSvrID, pstAppCtx->iId, FUNC_CS_SVR);
	GET_WORLD_ENTITY_ID(pstEnv->iMudSvrID, pstAppCtx->iId, FUNC_MUD_SVR);
	GET_WORLD_ID(pstEnv->unWorldID, pstAppCtx->iId);
	GET_REGION_ID(pstEnv->ucRegionID, pstAppCtx->iId);

	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		GET_REGION_ENTITY_ID_BY_BATTLE(pstEnv->iRegionGateID, pstAppCtx->iId, FUNC_REGION_GATE);
		GET_REGION_ENTITY_ID_BY_BATTLE(pstEnv->iRegionDBID, pstAppCtx->iId, FUNC_REGION_DB);
		GET_REGION_ENTITY_ID_BY_BATTLE(pstEnv->iAccountDBID, pstAppCtx->iId, FUNC_ACCOUNT_DB);
		
		GET_REGION_ENTITY_ID_BY_BATTLE(pstEnv->iBattleSpartaID, pstAppCtx->iId, FUNC_BATTLE_SVR);
		GET_REGION_ENTITY_ID_BY_BATTLE(pstEnv->iBattlePveBattleID, pstAppCtx->iId, FUNC_BATTLE_SVR);
		GET_REGION_ENTITY_ID_BY_BATTLE(pstEnv->iBattlePvpID, pstAppCtx->iId, FUNC_BATTLE_SVR);
	}
	else
	{
		GET_REGION_ENTITY_ID(pstEnv->iRegionGateID, pstAppCtx->iId, FUNC_REGION_GATE);
		GET_REGION_ENTITY_ID(pstEnv->iRegionDBID, pstAppCtx->iId, FUNC_REGION_DB);
		GET_REGION_ENTITY_ID(pstEnv->iAccountDBID, pstAppCtx->iId, FUNC_ACCOUNT_DB);

		GET_REGION_ENTITY_ID(pstEnv->iBattleSpartaID, pstAppCtx->iId, FUNC_BATTLE_SVR);
		GET_REGION_ENTITY_ID(pstEnv->iBattlePveBattleID, pstAppCtx->iId, FUNC_BATTLE_SVR);
		GET_REGION_ENTITY_ID(pstEnv->iBattlePvpID, pstAppCtx->iId, FUNC_BATTLE_SVR);
	}
	//处理下三个跨服的相关id
	pstEnv->iBattleSpartaID = (pstEnv->iBattleSpartaID&0x00ffffff) | (BATTLE_SPARTA_INSTANCE<<24);
	pstEnv->iBattlePveBattleID = (pstEnv->iBattlePveBattleID&0x00ffffff) | (BATTLE_PVE_BATTLE_INSTANCE<<24);
	pstEnv->iBattlePvpID = (pstEnv->iBattlePvpID&0x00ffffff) | (BATTLE_PVP_INSTANCE<<24);
	

	printf("region id=%d, svr type=%d world id=%d\n", pstEnv->ucRegionID, 
											pstEnv->iInstMode,
											pstEnv->unWorldID);

	if (0 > tconnapi_initialize(pstAppCtx->pszGCIMKey, pstAppCtx->iBusinessID))
	{
		printf("tbus gcim key %s init fail\n", pstAppCtx->pszGCIMKey);
		return -1;
	}
	
	if (0 > tconnapi_create(pstAppCtx->iId, &pstEnv->iTconnapiHandle))
	{
		printf("tbus id %s init fail\n", pstAppCtx->pszId);
		return -1;
	}
	
	if (0 > net_han_create_c(g_szMetalib_proto_cs, &pstEnv->pstNetHan))
	{
		return -1;
	}

	if (0 > ss_han_create(g_szMetalib_proto_ss, &pstEnv->pstSSHan))
	{
		return -1;
	}

	pstEnv->pstBlackWhiteH = NULL;
	if (pstEnv->pstConf->BlackWhiteCfgPath[0] && 0 > black_white_init(pstEnv->pstConf->BlackWhiteCfgPath, &pstEnv->pstBlackWhiteH))
	{
		printf("%s black_white_init fail\n", pstEnv->pstConf->BlackWhiteCfgPath);		
	}

	if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
	{
		pstEnv->iDirSvrSock = tnet_connect(pstEnv->pstConf->DirMasterUrl, 0);
		if (0 > pstEnv->iDirSvrSock)
		{
			perror("tnet connect dir master error");
			return -1;
		}

		pstEnv->iDirBakSock = tnet_connect(pstEnv->pstConf->DirBakUrl, 0);
		if (0 > pstEnv->iDirBakSock)
		{
			perror("tnet connect dir bak error");
			return -1;
		}
	}

	//pstEnv->iConnIP =(int)inet_addr(pstEnv->pstConf->ConnIP);
	pstEnv->unPort = HTONS(pstEnv->pstConf->ConnPort);

	InitDirtySystem("../cfg/dirty/dirtyword.txt");
	
	srand(pstAppCtx->stCurr.tv_sec);

	z_init_search_func(pstEnv->astSearchFuns);

	if (0 > z_oplog_init(pstAppCtx, pstEnv))
	{
		return -1;
	}

	if (0 > tdr_load_metalib(&pstEnv->pstResLib, "../cfg/resdb_meta.tdr"))
	{
		printf("load meta lib resdb_meta fail\n");
		return -1;
	}

	if (RES_LOAD_DB == pstEnv->pstConf->ResloadType)
	{
		memset(&stDBMS, 0, sizeof(stDBMS));
		STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
		STRNCPY(stDBMS.szDBMSCurDatabaseName, pstEnv->pstConf->ResDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
		STRNCPY(stDBMS.szDBMSConnectionInfo, pstEnv->pstConf->ResDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
		STRNCPY(stDBMS.szDBMSUser, pstEnv->pstConf->ResDBUser, sizeof(stDBMS.szDBMSUser));
		STRNCPY(stDBMS.szDBMSPassword, pstEnv->pstConf->ResDBPasswd, sizeof(stDBMS.szDBMSPassword));	
		stDBMS.iReconnectOpt = 1;	
		if (0 > tdr_open_dbhanlde(&pstEnv->hResDBHandle, &stDBMS, &szErrorMsg[0]))
		{
			printf("tdr_open_dbhanlde fail: %s", szErrorMsg);
			return -1;
		}
	}

	if (0 > torm_create_handle_from_dir(&pstEnv->hTorm, pstEnv->pstConf->DBMetaDir))
	{
		printf("torm_create_handle fail\n");
		return -1;
	}

	//装载资源
	if (0 > z_load_res(pstAppCtx, pstEnv))
	{
		printf("z_load_res fail\n");
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_load_res fail");
		return -1;
	}

	if (0 > init_script_sys(pstAppCtx, pstEnv))
	{
		printf("init_script_sys fail\n");
		return -1;
	}

	if (0 > init_item_scr(pstEnv))
	{
		printf("init_item_scr fail\n");
		return -1;
	}

	if (0 > init_task_scr(pstEnv))
	{
		printf("init_task_scr fail\n");
		return -1;
	}

	if (0 > z_init_player_scr(pstEnv))
	{
		printf("z_init_player_scr fail\n");
		return -1;
	}

	if (0 > z_init_cmd_map(pstEnv))
	{
		return -1;
	}

	if (0 > z_attach_player(pstEnv))
	{
		printf("attach player shm fail\n");
		tlog_error(pstEnv->pstLogCat,0,0,"attach player fail");
		return -1;
	}

	pstEnv->pstShtUinCache = z_attach_uincache(pstEnv);
	if (NULL == pstEnv->pstShtUinCache)
	{
		printf("attach uin cache shm fail\n");
		tlog_error(pstEnv->pstLogCat,0,0,"attach uin cache shm fail");
		return -1;
	}

	//装载地图
	if (0 > z_load_map(pstAppCtx, pstEnv, 0))
	{
		printf("z_load_map fail\n");
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_load_map fail");
		return -1;
	}

	z_check_uincache(pstEnv);
	z_check_id_index(pstAppCtx, pstEnv);

	z_sort_gm_cmd();
	if (0 > err_init("../cfg/res/mmogerr.xml", NULL, 0))
	{
		printf("err_init fail\n");
		return -1;
	}

	pstEnv->pstShtRangeName = sht_create(RANGE_ONE_NUM, RANGE_ONE_NUM, sizeof(RangeHashName), NULL);
	if (NULL == pstEnv->pstShtRangeName)
	{
		printf("range name hash fail\n");
		return -1;
	}

	
	// 载入交易表数据
	if (0 > z_shopping_init(pstAppCtx, pstEnv))
	{
		return -1;
	}

	if (store_init(pstEnv) < 0)
	{
		return -1;
	}

	if (span_global_init(pstEnv) < 0)
	{
		return -1;
	}

	if (home_init( pstEnv) < 0)
	{
		return -1;
	}

	printf("zone server start, version:%llu.%llu.%llu  build at %llu, pid=%d\n", 
		TAPP_GET_MAJOR(pstAppCtx->uiVersion),
		TAPP_GET_MINOR(pstAppCtx->uiVersion),
		TAPP_GET_REV(pstAppCtx->uiVersion),
		TAPP_GET_BUILD(pstAppCtx->uiVersion),
		getpid());
	
	tlog_info(pstEnv->pstLogCat, 0, 0, "zone server start, version:%lld.%lld.%lld  build at %lld, pid=%d\n", 
		TAPP_GET_MAJOR(pstAppCtx->uiVersion),
		TAPP_GET_MINOR(pstAppCtx->uiVersion),
		TAPP_GET_REV(pstAppCtx->uiVersion),
		TAPP_GET_BUILD(pstAppCtx->uiVersion),
		getpid());
	tlog_info_dr(pstEnv->pstLogCat, 0, 0, (LPTDRMETA)pstAppCtx->stConfData.iMeta, 
				pstEnv->pstConf, sizeof(*(pstEnv->pstConf)), 0);

	z_show_mem_use(pstEnv);

	pstEnv->stStart = pstAppCtx->stCurr;

	iStatusSeq = rand();
	pstEnv->unGidSeq = (unsigned short)iStatusSeq;
	pstEnv->iRoleDBProtoVer = 0;
	if(INST_MODE_ZONE_SVR == pstEnv->iInstMode)
	{
		z_torm_exchg_ver(pstAppCtx, pstEnv);
	}

	//先更新所有共享内存中的player/monster/npc
	z_touch_player(pstAppCtx, pstEnv);
	z_active_player(pstAppCtx, pstEnv, MAX_PLAYER);
	z_active_monster(pstAppCtx, pstEnv, MAX_MONSTER);
	z_active_game_event(pstAppCtx, pstEnv, MAX_GAME_EVENT);
	//z_active_npc(pstAppCtx, pstEnv, MAX_NPC);	

	if(INST_MODE_ZONE_SVR == pstEnv->iInstMode)
	{
		//检查事件，控制开启和关闭
		z_event_def_check(pstEnv);
		
		//载入事件变量
		z_load_event_xml(pstEnv);

		//装载GMIPMacLimit
		z_gm_limit_get(pstAppCtx, pstEnv);
	}

	pstEnv->iOpRoleNumTm = pstAppCtx->stCurr.tv_sec;
	pstEnv->iShopBuyTipTime = pstAppCtx->stCurr.tv_sec;

	//同步mud时间
	z_sync_mud_time(pstEnv);

	if (GLOBAL_STAT_FINI == pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat)
	{
		span_map_camp_init(pstEnv);
	}
	return 0;
}

int z_active_item(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iCount)
{
	static struct timeval stRoundTv;
	static struct timeval stRoundStartTv;
	static unsigned int iActivePos = 0, iRound = 0;
	int i;
	struct timeval stSub;
	long long llMs=0;
	DropItem *pstDropItem;
	TMEMBLOCK *pstMemBlock;

	if (0 == stRoundTv.tv_sec)
	{
		llMs = ITEM_ACTIVE_TIMER;
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
		
	if (llMs < ITEM_ACTIVE_TIMER)
	{
		return 0;
	}

	if (0 == iActivePos)
	{
		TV_CLONE(stRoundStartTv, pstAppCtx->stCurr);
	}

	for (i = 0; i < iCount; i++) 
	{
		if (iActivePos >= MAX_ITEM) 
		{
			break;
		}
		
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstItemPool, iActivePos);
		iActivePos++;

		if (!pstMemBlock->fValid)
		{
			continue;
		}
		
		pstDropItem = (DropItem *)pstMemBlock->szData;

		if (0 != pstDropItem->unRefreshTime && DROP_ITEM_NOSTAY == pstDropItem->cStat && 
			pstDropItem->iLeaveMapTime + pstDropItem->unRefreshTime < pstAppCtx->stCurr.tv_sec)
		{
			z_item_refresh(pstEnv, pstDropItem,1);
			continue;
		}

		if (DROP_ITEM_NOSTAY == pstDropItem->cStat)
		{
			continue;
		}

		if (pstDropItem->iStayTime > 0 && pstDropItem->stDropTime.tv_sec + pstDropItem->iStayTime < pstAppCtx->stCurr.tv_sec)
		{
			tlog_debug(pstEnv->pstLogCat, 0, 0, "drop item %d %d disappear", pstDropItem->iID, pstDropItem->iDefIdx);

			if (0 == pstDropItem->unRefreshTime)
			{
				z_item_clean(pstEnv, pstDropItem);
			}
			else
			{
				z_item_leave_map(pstAppCtx, pstEnv, pstDropItem);
			}

			continue;
		}
	}
	if (iActivePos < MAX_ITEM)
	{
		return 0;
	}
	
	iActivePos = 0;
	iRound++;
	TV_CLONE(stRoundTv, pstAppCtx->stCurr);

	TV_DIFF(stSub, pstAppCtx->stCurr, stRoundStartTv);
	TV_TO_MS(llMs, stSub);
	if (llMs > ITEM_ACTIVE_TIMER)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "active item loop %lld ms", llMs);
	}
	
	return 0;
}

int z_sendto_cs_svr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, SSPKG *pstSSPkg,int iResult)
{	
	int iSrcID;
	int iDstID;
	TDRDATA szNet;
	char szBuff[sizeof(SSPKG)];
	SSPKG stPkgRes;
	SSCSCTRL *pstCsCtrl = &pstSSPkg->Body.CsCtrL;
	
	stPkgRes.Head.Cmd = pstSSPkg->Head.Cmd;
	stPkgRes.Head.Magic = pstSSPkg->Head.Magic;
	stPkgRes.Head.Ver = pstSSPkg->Head.Ver;
	stPkgRes.Head.HeadLen = sizeof(stPkgRes.Head);
	stPkgRes.Body.CsCtrL.Cmd = ZONE_RES;
	memcpy(&stPkgRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq,&pstCsCtrl->CsCmd.ZoneReq,sizeof(pstCsCtrl->CsCmd.ZoneReq));
	stPkgRes.Body.CsCtrL.CsCmd.ZoneRes.Result = iResult;
	
	szNet.iBuff = sizeof(szBuff);
	szNet.pszBuff = szBuff;

	if (0 > ss_pack(pstEnv->pstSSHan, &szNet, &stPkgRes,  0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -1;
	}

	iSrcID = pstAppCtx->iId;
	iDstID = pstEnv->iCsSvrID;
	if (0 > tbus_send(pstAppCtx->iBus, &iSrcID, &iDstID, szNet.pszBuff, szNet.iBuff, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail");
		return -1;
	}
	
	return 0;
}

//int block_player_of_online(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, char *szRoleName,int iTime)
int block_player_of_online(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, 
                           ZONEBLOCKPLAYERREQ *pstReq)
{
	Player *pszPlayer = NULL;
	
	pszPlayer = z_name_player(pstAppCtx, pstEnv, pstReq->RoleName);
	if(pszPlayer == NULL)
	{
		return -1;
	}

	pszPlayer->stRoleData.BlockEnd = (pstReq->Time < 0) ? -1 :
		pstAppCtx->stCurr.tv_sec + pstReq->Time;

	pszPlayer->stRoleData.BlockStart = pstAppCtx->stCurr.tv_sec;
	
	if(pstReq->Time > 0)
	{
		player_kickoff(pstAppCtx, pstEnv, pszPlayer, ZONEERR_KICK208, pstReq->Time);	
	}
	else
	{
		player_kickoff(pstAppCtx, pstEnv, pszPlayer, ZONEERR_KICK209);	
	}
	
	return 0;
}


int silence_player_of_online(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,
	                           ZONESILENCEROLEREQ *pstReq)
{
	Player *pstPlayer = NULL;
	
	pstPlayer = z_name_player(pstAppCtx, pstEnv, pstReq->RoleName);
	if(pstPlayer == NULL)
	{
		return -1;
	}

	pstPlayer->stRoleData.SilenceEnd = (pstReq->Time < 0) ? -1 :
		pstAppCtx->stCurr.tv_sec + (int)pstReq->Time;

	pstPlayer->stRoleData.SilenceStart = pstAppCtx->stCurr.tv_sec;

	if(pstReq->Time > 0)
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_SVR201, pstReq->Time);
	}
	else if(pstReq->Time < 0)
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_SVR202);
	}
	else
	{
		pstPlayer->stRoleData.SilenceStart = 0;
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_SVR203);
	}
	

	return 0;
}

int player_kick_mud(ZONESVRENV* pstEnv, const char *pszLoginName)
{
	SSPKG stSSPkg;
	SSKICKACCNAME *pstKickAcc = &stSSPkg.Body.SSKickAccName;
	
	STRNCPY(pstKickAcc->AccName, pszLoginName, sizeof(pstKickAcc->AccName));
	
	Z_SSHEAD_INIT(&stSSPkg.Head, SS_KICK_ACCNAME);
	z_ssmsg_send(pstEnv, &stSSPkg, pstEnv->iMudSvrID);
	
	return 0;
}

int z_block_account(ZONESVRENV* pstEnv, const ZONECTLBLOCKACCOUNTREQ *pstCtlReq)
{
	Player *pstPlayer = NULL;
	SSPKG stSSPkg;
	ZONECTLBLOCKACCOUNTRES *pstCtlRes = 
		&stSSPkg.Body.CsCtrL.CsCmd.CtlBlockAccountRes;
	
	pstPlayer = z_loginname_player(pstEnv->pstAppCtx, pstEnv,
	                               pstCtlReq->Core.LoginName);
	if(!pstPlayer)
	{
		pstCtlRes->Core.Succeeded = 0;
		goto final;
	}
	else
	{
		pstCtlRes->Core.Succeeded = 1;
	}

	if(pstCtlReq->Core.BlockTime < 0)
	{
		player_kickoff(pstEnv->pstAppCtx, pstEnv, pstPlayer, ZONEERR_KICK210);
	}
	else
	{
		player_kickoff(pstEnv->pstAppCtx, pstEnv, pstPlayer, ZONEERR_KICK211, pstCtlReq->Core.BlockTime);
	}
	
final:
	ss_init_head(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_BLOCK_ACCOUNT_RES;
	pstCtlRes->Extra = pstCtlReq->Extra;
	ss_sendto_cs_svr(pstEnv, &stSSPkg);

	player_kick_mud(pstEnv, pstCtlReq->Core.LoginName);
	
	return 0;
}

int z_silence_account(ZONESVRENV* pstEnv, 
	                    const ZONECTLSILENCEACCOUNTREQ *pstCtlReq)
{
	Player *pstPlayer = NULL;
	SSPKG stSSPkg;
	ZONECTLSILENCEACCOUNTRES *pstCtlRes = 
		&stSSPkg.Body.CsCtrL.CsCmd.CtlSilenceAccountRes;

	pstPlayer = z_loginname_player(pstEnv->pstAppCtx, pstEnv,
	                               pstCtlReq->Core.LoginName);
	if(!pstPlayer)
	{
		pstCtlRes->Core.Succeeded = 0;
		goto final;
	}
	else
	{
		pstCtlRes->Core.Succeeded = 1;
	}

	if(pstCtlReq->Core.Enable)
	{
		pstPlayer->iAccSilenceEnd = (pstCtlReq->Core.SilenceTime < 0) ? -1 :
			pstEnv->pstAppCtx->stCurr.tv_sec + pstCtlReq->Core.SilenceTime;
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_SVR205, pstCtlReq->Core.SilenceTime);
	}
	else
	{
		pstPlayer->iAccSilenceEnd = 0;
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_SVR204);
	}

final:
	ss_init_head(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_SILENCE_ACCOUNT_RES;
	pstCtlRes->Extra = pstCtlReq->Extra;
	ss_sendto_cs_svr(pstEnv, &stSSPkg);
	return 0;
}

int z_get_bulletin_list(ZONESVRENV* pstEnv, 
	                      const ZONECTLGETBULLETINLISTREQ *pstCtlReq)
{
	int i = 0;
	SSPKG stSSPkg;
	ZONECTLGETBULLETINLISTRES *pstCtlRes = 
		&stSSPkg.Body.CsCtrL.CsCmd.CtlGetBulletinListRes;
	ZONEGETBULLETINLISTRES *pstRes = &pstCtlRes->Core;
	const int kCurr = pstEnv->pstAppCtx->stCurr.tv_sec;

	i = pstEnv->pstBulletinBoard->iNum;
	pstRes->Num = pstEnv->pstBulletinBoard->iNum;
	while(i--)
	{
		BULLETINENTRY *pstDst = &pstRes->BulletinArray[i].Core;
		const BULLETINENTRY *pstSrc = 
			&pstEnv->pstBulletinBoard->astBulletinArray[i].stCore;

		pstDst->ID = pstSrc->ID;
		pstDst->RollInterval = pstSrc->RollInterval;
		STRNCPY(pstDst->Text, pstSrc->Text, sizeof(pstDst->Text));

		if (pstEnv->pstBulletinBoard->astBulletinArray[i].iFlag)
		{
			pstRes->BulletinArray[i].ActiveTime = 
				pstEnv->pstBulletinBoard->astBulletinArray[i].iActiveEnd - kCurr;
		}
		else
		{
			pstRes->BulletinArray[i].ActiveTime = pstEnv->pstBulletinBoard->astBulletinArray[i].iActiveEnd;
		}
		
		if (0 > pstRes->BulletinArray[i].ActiveTime)
		{
			pstRes->BulletinArray[i].ActiveTime = 0;
		}

		pstRes->BulletinArray[i].ActiveTimeStart = pstEnv->pstBulletinBoard->astBulletinArray[i].iActiveStart;
	}
	
	ss_init_head(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_GET_BULLETIN_LIST_RES;
	pstCtlRes->Extra = pstCtlReq->Extra;
	ss_sendto_cs_svr(pstEnv, &stSSPkg);
	return 0;
}

int z_gen_bulletin_id()
{
	static int iN = 0;
	int iCurr = time(NULL);
	int iRet = 0;

	iN = (iN + 1) % 10807;
	iRet = (((rand() * iN) & 0x00ffff00) << 8) | ((iCurr + iN) & 0x0000ffff);

	return iRet;
}

int z_add_bulletin(ZONESVRENV* pstEnv, const SSBULLETIN *pstReq)
{
	BulletinBoard *pstBoard = pstEnv->pstBulletinBoard;
	const int kCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	ZoneBulletin *pstNew = NULL;

	if(pstBoard->iNum >= (int)(CCH(pstBoard->astBulletinArray)))
	{
		return -1;
	}

	// 结束时间比当前时间还小就不发了
	/*if (	pstReq->ActiveTimeStart != 0 && 
		pstReq->ActiveTimeStart + pstReq->ActiveTime < kCurr)
	{
		return -1;
	}*/

	pstNew = &pstBoard->astBulletinArray[pstEnv->pstBulletinBoard->iNum++];
	memset(pstNew, 0, sizeof(*pstNew));

	pstNew->iActiveEnd 			= kCurr + pstReq->ActiveTime;		// pstReq->ActiveTime 持续时间
	pstNew->iActiveStart 			= pstReq->ActiveTimeStart;		// 开始时间
	pstNew->stCore.ID 			= pstReq->Core.ID;
	pstNew->stCore.RollInterval 	= pstReq->Core.RollInterval;
	pstNew->stCore.IconID 		= pstReq->Core.IconID;
	pstNew->stCore.Type 			= pstReq->Core.Type;
	pstNew->stCore.TStart 			= pstNew->iActiveStart;			// 客户端要知道的开始时间
	STRNCPY(pstNew->stCore.Text, pstReq->Core.Text, CCH(pstNew->stCore.Text));

	// 还没到时间的  不发给客户端
	if (	pstNew->iActiveStart > 0 &&
		pstNew->iActiveStart > kCurr)
	{
		pstNew->iFlag = 0;  									// 0表示还没有发送
		pstNew->iActiveEnd = 	pstReq->ActiveTime;				// 借用iActiveEnd 保存一下持续时间
		return 0;
	}
	else
	{
		// 立即广播到所有玩家
		pstNew->iFlag = 1;
		return z_bulletin_chg_notify(pstEnv, &pstNew->stCore, 1);
	}	
}

static int ss_add_bulletin(ZONESVRENV* pstEnv, 
	                         const ZONECTLADDBULLETINREQ *pstCtlReq)
{
	SSPKG stSSPkg;
	ZONECTLADDBULLETINRES *pstCtlRes = 
		&stSSPkg.Body.CsCtrL.CsCmd.CtlAddBulletinRes;

	if(z_add_bulletin(pstEnv, &pstCtlReq->Core.Bulletin) < 0)
	{
		pstCtlRes->Core.Succeeded = 0;
	}
	else
	{
		pstCtlRes->Core.Succeeded = 1;
	}

	ss_init_head(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_ADD_BULLETIN_RES;
	pstCtlRes->Extra = pstCtlReq->Extra;
	ss_sendto_cs_svr(pstEnv, &stSSPkg);
	return 0;
}

int z_del_bulletin(ZONESVRENV* pstEnv, int iID)
{
	BulletinBoard *pstBoard = pstEnv->pstBulletinBoard;
	int i = 0;

	for(i = 0; i < pstBoard->iNum; i++)
	{
		if(iID == pstBoard->astBulletinArray[i].stCore.ID)
		{
			break;
		}
	}

	if(i >= pstBoard->iNum)
	{
		return -1;
	}

	// 广播到所有玩家
	z_bulletin_chg_notify(pstEnv, &pstBoard->astBulletinArray[i].stCore, 0);

	if(i < pstBoard->iNum - 1)
	{
		memcpy(pstBoard->astBulletinArray + i,
			     pstBoard->astBulletinArray + pstBoard->iNum - 1,
			     sizeof(pstBoard->astBulletinArray[0]));
	}
	pstEnv->pstBulletinBoard->iNum--;
	
	return 0;
}

int ss_del_bulletin(ZONESVRENV* pstEnv, const ZONECTLDELBULLETINREQ *pstCtlReq)
{
	SSPKG stSSPkg;
	ZONECTLDELBULLETINRES *pstCtlRes = 
		&stSSPkg.Body.CsCtrL.CsCmd.CtlDelBulletinRes;

	if(z_del_bulletin(pstEnv, pstCtlReq->Core.ID) < 0)
	{
		pstCtlRes->Core.Succeeded = 0;
	}
	else
	{
		pstCtlRes->Core.Succeeded = 1;
	}

	ss_init_head(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_DEL_BULLETIN_RES;
	pstCtlRes->Extra = pstCtlReq->Extra;
	ss_sendto_cs_svr(pstEnv, &stSSPkg);
	return 0;
}

int ss_update_rolelist(ZONESVRENV* pstEnv, 
	                     const ZONEUPDATEROLELISTCACHEREQ *pstCtlReq)
{
	UinCache stDummy, *pstHashData;

	stDummy.iUin = pstCtlReq->Uin;
	pstHashData = sht_find(pstEnv->pstShtUinCache, &stDummy, UinHashCmp,
		                     UinHashCode);
	if(!pstHashData)
	{
		return 0;
	}

	return z_db_role_list_x(pstEnv->pstAppCtx, pstEnv, pstCtlReq->Uin);
}

int z_active_bulletin(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i;
	ZoneBulletin *pstBulletin = NULL;
	BulletinBoard *pstBoard = pstEnv->pstBulletinBoard;
	
	for (i=pstBoard->iNum-1; i>=0; i--)
	{
		pstBulletin = pstBoard->astBulletinArray + i;

		// 激活公告时间到了 
		if (	pstBulletin->iActiveStart <= pstAppCtx->stCurr.tv_sec &&
			pstBulletin->iFlag == 0)
		{
			// 添加一条记录
			pstBulletin->iFlag = 1;
			pstBulletin->iActiveEnd = pstBulletin->iActiveEnd+pstAppCtx->stCurr.tv_sec;
			z_bulletin_chg_notify(pstEnv, &pstBulletin->stCore, 1);
		}

		// 时间到期了 删除
		else if (pstBulletin->iActiveEnd <= pstAppCtx->stCurr.tv_sec &&
				pstBulletin->iFlag != 0)
		{
			z_bulletin_chg_notify(pstEnv, &pstBulletin->stCore, 0);
			if(i < pstBoard->iNum - 1)
			{
				memcpy(pstBoard->astBulletinArray + i,
					     pstBoard->astBulletinArray + pstBoard->iNum - 1,
					     sizeof(pstBoard->astBulletinArray[0]));
			}
			pstBoard->iNum--;
		}
	}
	
	/*
	static struct timeval stRoundTv;
	static struct timeval stRoundStartTv;
	static  int iActivePos = 0, iRound = 0;
	int i;
	struct timeval stSub;
	long long llMs=0;
	ZoneBulletin *pstBulletin = NULL;
	BulletinBoard *pstBoard = pstEnv->pstBulletinBoard;
	const int kCurr = pstAppCtx->stCurr.tv_sec;
	int iDel=0;

	

	if(pstBoard->iNum <= 0)
	{
		return 0;
	}
	
	if (0 == stRoundTv.tv_sec)
	{
		llMs = BULLETIN_ACTIVE_TIMER;
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
		
	if (llMs < BULLETIN_ACTIVE_TIMER)
	{
		return 0;
	}

	if (0 == iActivePos)
	{
		TV_CLONE(stRoundStartTv, pstAppCtx->stCurr);
	}

	for (i = 0; i < iCount; i++) 
	{
		iDel=0;
		if (iActivePos >= pstBoard->iNum) 
		{
			break;
		}

		pstBulletin = pstBoard->astBulletinArray + iActivePos;

		// 激活公告时间到了 
		if (	pstBulletin->iActiveStart <= kCurr &&
			pstBulletin->iFlag == 0)
		{
			// 添加一条记录
			pstBulletin->iFlag = 1;
			pstBulletin->iActiveEnd = pstBulletin->iActiveEnd+kCurr;
			z_bulletin_chg_notify(pstEnv, &pstBulletin->stCore, 1);
		}

		// 时间到期了 删除
		else if (pstBulletin->iActiveEnd <= kCurr &&
				pstBulletin->iFlag != 0)
		{
			z_bulletin_chg_notify(pstEnv, &pstBulletin->stCore, 0);
			if(iActivePos < pstBoard->iNum - 1)
			{
				memcpy(pstBoard->astBulletinArray + iActivePos,
					     pstBoard->astBulletinArray + pstBoard->iNum - 1,
					     sizeof(pstBoard->astBulletinArray[0]));
			}
			pstBoard->iNum--;
			iDel = 1;
		}

		if (!iDel)
			iActivePos++;
	}
	
	if (iActivePos < pstBoard->iNum)
	{
		return 0;
	}
	
	iActivePos = 0;
	iRound++;
	TV_CLONE(stRoundTv, pstAppCtx->stCurr);

	TV_DIFF(stSub, pstAppCtx->stCurr, stRoundStartTv);
	TV_TO_MS(llMs, stSub);
	if (llMs > BULLETIN_ACTIVE_TIMER)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "active clan loop %lld ms", llMs);
	}*/
	
	return 0;
}

// SSSS
int ss_sendto_cs_svr(ZONESVRENV *pstEnv, SSPKG *pstSSPkg)
{
	int iSrc, iDst;
	TDRDATA stNet;

	stNet.pszBuff = pstEnv->szSendBuffer;
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	if(ss_pack(pstEnv->pstSSHan, &stNet, pstSSPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -1;
	}

	iSrc = pstEnv->pstAppCtx->iId;
	iDst = pstEnv->iCsSvrID;
	if(tbus_forward(pstEnv->pstAppCtx->iBus, &iSrc, &iDst, stNet.pszBuff, 
		              stNet.iBuff, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"tbus_forward cs_svr fail.");
		return -1;
	}
	return 0;
}

void ss_init_head(SSPKGHEAD *pstHead, int iCmd)
{
	pstHead->Cmd = iCmd;
	pstHead->HeadLen = sizeof(SSPKGHEAD);
	pstHead->Magic = PROTOCOL_MAGIC;
	pstHead->Ver = TDR_METALIB_PROTO_SS_VERSION;
}

int z_msg_to_cs(ZONESVRENV* pstEnv, ASYNREQ *pstAsynReq)
{
	int iErr;
	int iSrcID;
	int iDstID;
	TDRDATA szNet;
	char szBuff[sizeof(SSPKG)];
	SSPKG stPkgRes;
	ASYNREQ *pstAsyn = &stPkgRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq;        

	stPkgRes.Head.Cmd = CS_CTRL;
	stPkgRes.Head.Magic = PROTOCOL_MAGIC;
	stPkgRes.Head.Ver = TDR_METALIB_PROTO_SS_VERSION;
	stPkgRes.Head.HeadLen = sizeof(stPkgRes.Head);
	stPkgRes.Body.CsCtrL.Cmd = ZONE_RES;
	stPkgRes.Body.CsCtrL.CsCmd.ZoneRes.Result = 1;
	memcpy(pstAsyn, pstAsynReq, sizeof(*pstAsynReq));

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

int z_send_result_to_cs(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
                        ASYNREQ *pstAsyn, const void *pvRes, int iResLen)
{
	
	ASYNREQ stAsynReq;
	UNUSED(pstAppCtx);
	
	if((int)sizeof(stAsynReq.Data) < iResLen)
	{
		return -1;
	}
	
	memcpy(stAsynReq.Data, pvRes, iResLen);
	stAsynReq.DataLen = iResLen;

	stAsynReq.Cmd = pstAsyn->Cmd;
	stAsynReq.ConnIdx = pstAsyn->ConnIdx;
	stAsynReq.IPInfo = pstAsyn->IPInfo;
	z_msg_to_cs(pstEnv, &stAsynReq);
	return 0;
}

int z_shop_cgi_to_cs(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, SSPKG *pstSSPkg, SHOPCGIRES *pstRes)
{
	ASYNREQ stAsynReq;
	SSCSCTRL *pstCsCtrl = &pstSSPkg->Body.CsCtrL;
	
	stAsynReq.Cmd = pstCsCtrl->CsCmd.ZoneReq.Cmd;
	stAsynReq.ConnIdx = pstCsCtrl->CsCmd.ZoneReq.ConnIdx;
	stAsynReq.IPInfo = pstCsCtrl->CsCmd.ZoneReq.IPInfo;

	stAsynReq.DataLen = sizeof(*pstRes);
	if ((int)sizeof(stAsynReq.Data) < stAsynReq.DataLen)
	{
		return -1;
	}
		
	memcpy(stAsynReq.Data, pstRes, stAsynReq.DataLen);
	
	z_msg_to_cs(pstEnv, &stAsynReq);
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_get_role_info_to_cs(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,SSPKG *pstSSPkg, char *pszRoleName, int iCmd)
{
	Player *pszPlayer = NULL;
	
	ZONEGETROLEDETAILRES stGetRoleDetailRes;
	ROLEDETAIL *pstRoleDetail = &stGetRoleDetailRes.RoleDetailInfo;
	ZONEGETROLESKILLINFORES stGetRoleSkillRes;
	ZONEGETROLEDIATHESISRES stGetRoleDiathsisRes;
	ZONEGETROLEITEMLISTRES stGetRoleItemRes;
	//ZONEGETROLEFITTINGSRES stGetRoleFittingRes;
	ZONEGETROLETASKRES stGetRoleTaskRes;
	ZONECHGROLEPOSRES stChgRolePosRes;
	ZONEGETROLERIDEINFORES stGetRoleRideRes;
	GETROLEQIYUELISTRES stGetRoleQiYueListRes;
	ZONEGETROLEPETINFORES stGetRolePetRes;
	ZONEGETROLEPACKAGERES stGetRolePackageRes;
	ZONEGETPACKAGEITEMINFORES stGetPackageItemInfoRes;	
		
	ROLEDATA *pstRoleData;
	SSCSCTRL *pstCsCtrl = &pstSSPkg->Body.CsCtrL;
	ASYNREQ stAsynReq;

	pszPlayer = z_name_player(pstAppCtx, pstEnv, pszRoleName);
	if(pszPlayer == NULL)
	{
		return -1;
	}

	pstRoleData = &pszPlayer->stRoleData;

	if (iCmd == GET_ROLE_DETAIL_REQ)
	{
		memcpy(pstRoleDetail, &pstRoleData->RoleDetail, sizeof(*pstRoleDetail));
		pstRoleDetail->StashMoney = pstRoleData->Package.StashPackage.Money;
		stGetRoleDetailRes.StashMoney = pstRoleData->Package.StashPackage.Money;
		stGetRoleDetailRes.StashBull = pstRoleData->Package.StashPackage.Bull;
		//pstRoleDetail->Bull =  pstRoleData->Package.StashPackage.Bull;

		stGetRoleDetailRes.Block = 0;
		stGetRoleDetailRes.BlockStart = pstRoleData->BlockStart;

		//stGetRoleDetailRes.Silence = 0;
		if (pstRoleData->SilenceEnd > 0)
		{
			if (pstRoleData->SilenceEnd  - pstAppCtx->stCurr.tv_sec>0)
			{
				stGetRoleDetailRes.Silence = pstRoleData->SilenceEnd- pstAppCtx->stCurr.tv_sec ;
			}
			else
			{
				stGetRoleDetailRes.Silence = 0;
			}
		}
		else
		{
			stGetRoleDetailRes.Silence = pstRoleData->SilenceEnd;
		}
		stGetRoleDetailRes.SilenceStart = pstRoleData->SilenceStart;

		stGetRoleDetailRes.IsOnline = 1;
		stGetRoleDetailRes.MapID = pstRoleData->Map;
		stGetRoleDetailRes.X = pstRoleData->Pos.X;
		stGetRoleDetailRes.Y = pstRoleData->Pos.Y;
		stGetRoleDetailRes.Perm = pstRoleData->Perm;
		stGetRoleDetailRes.CreateTime = pstRoleData->CreateTime;
		stGetRoleDetailRes.DelTime = pstRoleData->DelTime;
		stGetRoleDetailRes.Uin = pstRoleData->Uin;
		stGetRoleDetailRes.RoleID = pstRoleData->RoleID;
		stGetRoleDetailRes.Level = pstRoleData->Level;
		stGetRoleDetailRes.Career = pstRoleData->Career;
		stGetRoleDetailRes.Gender = pstRoleData->Gender;
		stGetRoleDetailRes.ClanID = pstRoleData->MiscInfo.ClanInfo.ClanId.Id;

		stGetRoleDetailRes.PreLoginTime = pstRoleData->LastLogin;
		stGetRoleDetailRes.LastLogoutTime = pstRoleData->LastLogout;
		STRNCPY(stGetRoleDetailRes.ClanName, pstRoleData->MiscInfo.ClanInfo.ClanId.Name, 
					sizeof(stGetRoleDetailRes.ClanName));
		STRNCPY(stGetRoleDetailRes.RoleName, pstRoleData->RoleName, sizeof(stGetRoleDetailRes.RoleName));

		stGetRoleDetailRes.AttrVal = pstRoleData->MiscInfo.AttrVal;
		stAsynReq.DataLen = sizeof(stGetRoleDetailRes);
		stGetRoleDetailRes.GeniusInfo = pstRoleData->MiscInfo.GeniusInfo;

		if ((int)sizeof(stAsynReq.Data) < stAsynReq.DataLen)
		{
			return -1;
		}
		
		memcpy(stAsynReq.Data, &stGetRoleDetailRes, stAsynReq.DataLen);
	}
	else if (iCmd == GET_ROLE_SKILLINFO_REQ)
	{
		z_skill_roledata(pstAppCtx, pstEnv, pszPlayer);
		memcpy(&stGetRoleSkillRes.RoleSkillInfo, &pstRoleData->SkillInfo, sizeof(pstRoleData->SkillInfo));
		stAsynReq.DataLen = sizeof(stGetRoleSkillRes);

		if ((int)sizeof(stAsynReq.Data) < stAsynReq.DataLen)
		{
			return -1;
		}
		memcpy(stAsynReq.Data, &stGetRoleSkillRes, stAsynReq.DataLen);
	}
	else if (iCmd == GET_ROLE_DIATHESIS_REQ)
	{
		stGetRoleDiathsisRes.ZoneDiathesisInfo = pstRoleData->DiathesisInfo;
		stAsynReq.DataLen = sizeof(stGetRoleDiathsisRes);

		if ((int)sizeof(stAsynReq.Data) < stAsynReq.DataLen)
		{
			return -1;
		}
		memcpy(stAsynReq.Data, &stGetRoleDiathsisRes, stAsynReq.DataLen);
	}

		/*else if (iCmd == GET_ROLE_FITTING_REQ)
	{
	stGetRoleFittingRes.RoleFittingsInfo = pstRoleData->FittingsInfo;
		stAsynReq.DataLen = sizeof(stGetRoleFittingRes);

		if (sizeof(stAsynReq.Data) < sizeof(stAsynReq.DataLen))
		{
			return -1;
		}
		memcpy(stAsynReq.Data, &stGetRoleFittingRes, stAsynReq.DataLen);
	}*/
	else if (GET_ROLE_TASK_REQ == iCmd)
	{
		stGetRoleTaskRes.TaskInfo = pstRoleData->MiscInfo.TaskInfo;
		stAsynReq.DataLen = sizeof(stGetRoleTaskRes);

		if ((int)sizeof(stAsynReq.Data) < stAsynReq.DataLen)
		{
			return -1;
		}
		memcpy(stAsynReq.Data, &stGetRoleTaskRes, stAsynReq.DataLen);
	}
	else if (GET_ROLE_ITEMLIST_REQ == iCmd)
	{
		stGetRoleItemRes.ZoneRolePackage = pstRoleData->Package;
		stGetRoleItemRes.ZoneRoleWear = pstRoleData->Wear;
		
		stAsynReq.DataLen = sizeof(stGetRoleItemRes);

		if ((int)sizeof(stAsynReq.Data) < stAsynReq.DataLen)
		{
			return -1;
		}
		memcpy(stAsynReq.Data, &stGetRoleItemRes, stAsynReq.DataLen);
	}
 
	else if(CHG_ROLE_POS_REQ == iCmd)
	{
		//player_goto(pstEnv, pstPlayer, stChgRolePosReq.Map, stChgRolePosReq.X, stChgRolePosReq.Y);
		strncpy(stChgRolePosRes.RoleName, pstRoleData->RoleName, sizeof(stChgRolePosRes.RoleName));

		stAsynReq.DataLen = sizeof(stChgRolePosRes);
		memcpy(stAsynReq.Data, &stChgRolePosRes, stAsynReq.DataLen);
		
	}
	else if (iCmd == GET_ROLE_RIDE_REQ)
	{
		memcpy(&stGetRoleRideRes.RoleRideInfo, &pstRoleData->MiscInfo.RideList, sizeof(pstRoleData->MiscInfo.RideList));
		stAsynReq.DataLen = sizeof(stGetRoleRideRes);

		if ((int)sizeof(stAsynReq.Data) < stAsynReq.DataLen)
		{
			return -1;
		}
		memcpy(stAsynReq.Data, &stGetRoleRideRes, stAsynReq.DataLen);
	}
	else if (iCmd == GET_ROLE_PACKAGE_REQ)
	{
		ZONEGETROLEPACKAGEREQ stPackageReq;

		memcpy(&stPackageReq, &pstSSPkg->Body.CsCtrL.CsCmd.ZoneReq.Data,
			       sizeof(stPackageReq));
		
		stGetRolePackageRes.ZoneRolePackageData.ListType = stPackageReq.ListType;
		stGetRolePackageRes.ZoneRolePackageData.MaxNum = package_maxnum_by_type(pstRoleData, stPackageReq.ListType);

		stAsynReq.DataLen = sizeof(stGetRolePackageRes);
		if ((int)sizeof(stAsynReq.Data) < stAsynReq.DataLen)
		{
			return -1;
		}
		memcpy(stAsynReq.Data, &stGetRolePackageRes, stAsynReq.DataLen);
	}
	else if (iCmd == GET_ROLE_PACKAGE_ITEM_REQ)
	{
		ZONEGETPACKAGEITEMINFOREQ stGetPackageItemInfoReq;

		memcpy(&stGetPackageItemInfoReq, &pstSSPkg->Body.CsCtrL.CsCmd.ZoneReq.Data,
			       sizeof(stGetPackageItemInfoReq));
		
		stGetPackageItemInfoRes.ListType = stGetPackageItemInfoReq.ListType;
		package_item_info_by_type(pstRoleData, stGetPackageItemInfoReq.ListType,  &stGetPackageItemInfoRes.GetPackageItemInfo);
		
		stAsynReq.DataLen = sizeof(stGetPackageItemInfoRes);
		if ((int)sizeof(stAsynReq.Data) < stAsynReq.DataLen)
		{
			return -1;
		}
		memcpy(stAsynReq.Data, &stGetPackageItemInfoRes, stAsynReq.DataLen);
	}	
	else if (iCmd == GET_ROLE_QIYUELIST_REQ)
	{
		memcpy(&stGetRoleQiYueListRes.QiYueInfo, &pstRoleData->Package.QiYueInfo, sizeof(stGetRoleQiYueListRes.QiYueInfo));
		stAsynReq.DataLen = sizeof(stGetRoleQiYueListRes);

		if ((int)sizeof(stAsynReq.Data) < stAsynReq.DataLen)
		{
			return -1;
		}
		memcpy(stAsynReq.Data, &stGetRoleQiYueListRes, stAsynReq.DataLen);
	}
	else if (iCmd == GET_ROLE_PET_REQ)
	{
		memcpy(&stGetRolePetRes.RolePetInfo, &pstRoleData->Package.PetPackage, sizeof(stGetRolePetRes.RolePetInfo));
		stAsynReq.DataLen = sizeof(stGetRolePetRes);

		if ((int)sizeof(stAsynReq.Data) < stAsynReq.DataLen)
		{
			return -1;
		}
		memcpy(stAsynReq.Data, &stGetRolePetRes, stAsynReq.DataLen);
	}
	else if(iCmd == GET_ROLE_DESIGNATION_LIST_REQ)
	{
		ZONEGETROLEDESIGNATIONLISTRES stRes;

		stRes.Succeeded = 1;
		stRes.Designations = pstRoleData->MiscInfo.DesignationInfo;
	
		stAsynReq.DataLen = sizeof(stRes);
		if (sizeof(stAsynReq.Data) < sizeof(stRes))
		{
			stRes.Succeeded = 0;
		}
		memcpy(stAsynReq.Data, &stRes, stAsynReq.DataLen);
	}
	else if(iCmd == GET_ROLE_RESPACT_LIST_REQ)
	{
		ZONEGETROLERESPACTLISTRES stRes;

		stRes.Succeeded = 1;
		stRes.RespactInfo = pstRoleData->MiscInfo.RespactInfo;
	
		stAsynReq.DataLen = sizeof(stRes);
		if (sizeof(stAsynReq.Data) < sizeof(stRes))
		{
			stRes.Succeeded = 0;
		}
		memcpy(stAsynReq.Data, &stRes, stAsynReq.DataLen);
	}
	else if (iCmd == GET_ROLE_FRIEND_REQ)
	{
		ZONEGETFIRENDRES stRes;

		memcpy(&stRes.RoleBuddy, &pstRoleData->Buddy, sizeof(stRes.RoleBuddy));
		
		stAsynReq.DataLen = sizeof(stRes);
		if (sizeof(stAsynReq.Data) < sizeof(stRes))
		{
			return -1;
		}
		memcpy(stAsynReq.Data, &stRes, stAsynReq.DataLen);
	
	}
	else if (iCmd == GET_ROLE_BLACK_REQ)
	{
		ZONEGETBLACKRES stRes;
		
		stRes.BlackNum = pstRoleData->MiscInfo.BlackNum;
		stRes.EnemyNum = pstRoleData->MiscInfo.Enemy.EnemyNum;
		memcpy(stRes.Blacks, pstRoleData->MiscInfo.BlackMember, sizeof(stRes.Blacks));	
		memcpy(stRes.Enemys, pstRoleData->MiscInfo.Enemy.Enemys, sizeof(stRes.Enemys));

		stAsynReq.DataLen = sizeof(stRes);
		if (sizeof(stAsynReq.Data) < sizeof(stRes))
		{
			return -1;
		}
		memcpy(stAsynReq.Data, &stRes, stAsynReq.DataLen);
		
	}
	else if (iCmd == GET_LIFT_SKILL_REQ)
	{
		GETLIFTSKILLRES stRes;
		strncpy(stRes.RoleName, pstRoleData->RoleName, sizeof(stRes.RoleName));
		stRes.LiftSkillInfo = pstRoleData->MiscInfo.SecondarySkillMiscInfo;

		stAsynReq.DataLen = sizeof(stRes);
		if (sizeof(stAsynReq.Data) < sizeof(stRes))
		{
			return -1;
		}

		memcpy(stAsynReq.Data, &stRes, stAsynReq.DataLen);
	}
	else
	{
		return -1;
	}

	stAsynReq.Cmd = pstCsCtrl->CsCmd.ZoneReq.Cmd;
	stAsynReq.ConnIdx = pstCsCtrl->CsCmd.ZoneReq.ConnIdx;
	stAsynReq.IPInfo = pstCsCtrl->CsCmd.ZoneReq.IPInfo;

	z_msg_to_cs(pstEnv, &stAsynReq);
	return 0;
}

int z_chg_role_name_to_cs(ZONESVRENV* pstEnv, ZONECHGROLENAMEREQ *pstChg, SSPKG *pstSSPkg)
{
	Player *pstPlayer;
	SSCSCTRL *pstCsCtrl = &pstSSPkg->Body.CsCtrL;
	ChgRoleName stChgName;
	
	/*if ( CheckCharacter(pstChg->ChgRoleName) ||
		(CheckDirty(pstChg->ChgRoleName , 0x1f) & 0x1f ) )
	{
		return -1;
	}*/

	stChgName.Cmd = pstCsCtrl->CsCmd.ZoneReq.Cmd;
	stChgName.Uin = pstChg->Uin;
	stChgName.RoleID = pstChg->RoleID;
	stChgName.ConnIdx = pstCsCtrl->CsCmd.ZoneReq.ConnIdx;
	memcpy(&stChgName.IPInfo, &pstCsCtrl->CsCmd.ZoneReq.IPInfo, sizeof(stChgName.IPInfo));
	STRNCPY(stChgName.szName, pstChg->ChgRoleName, sizeof(stChgName.szName));
	STRNCPY(stChgName.szOldName, pstChg->OldRoleName, sizeof(stChgName.szOldName));
		
	pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstChg->OldRoleName);
	if (pstPlayer )
	{
		/*if (pstPlayer->stRoleData.RoleID != pstChg->RoleID || 
			pstPlayer->stRoleData.Uin != pstChg->Uin )*/
		//在线不让改，先封住	
		{
			return -1;
		}
		//chg_name_update_req(pstEnv, pstPlayer->stRoleData.RoleID, pstPlayer->stRoleData.Uin, &stChgName);
	}
	
	chg_name_update_req(pstEnv,pstChg->RoleID, pstChg->Uin, &stChgName);
	
	return 0;
}

int z_cs_select_roledata(ZONESVRENV* pstEnv, ZONEDELROLEREQ *pstDelRoleReq, ASYNREQ *pstAsynReq)
{

	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	ROLEDATA stRoleData;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;
		
	stDBAsyn.iCmd = DB_CS_DEL_ROLE_SELECT;
	stDBAsyn.AsynCmd.stCsDelRole.Cmd = DEL_ROLE_REQ;
	stDBAsyn.AsynCmd.stCsDelRole.ConnIdx = pstAsynReq->ConnIdx;
	stDBAsyn.AsynCmd.stCsDelRole.IPInfo = pstAsynReq->IPInfo;
	stDBAsyn.AsynCmd.stCsDelRole.RoleID = pstDelRoleReq->RoleID;
	stDBAsyn.AsynCmd.stCsDelRole.Uin = pstDelRoleReq->Uin;
	STRNCPY(stDBAsyn.AsynCmd.stCsDelRole.szName, pstDelRoleReq->RoleName, 
		sizeof(stDBAsyn.AsynCmd.stCsDelRole.szName));
	
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, "RoleData", -1, &stDBAsyn);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 0;

	memset(&stRoleData, 0, sizeof(stRoleData));
	STRNCPY(stRoleData.RoleName, pstDelRoleReq->RoleName, sizeof(stRoleData.RoleName));
	//stRoleData.Uin = pstPlayer->stRoleData.Uin;
	//stRoleData.RoleID = pstRoleLoginReq->RoleID;
	sprintf(pstSelectReq->OperInfo.WhereDef,"where RoleName='%s'", stRoleData.RoleName);
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRoleData;
	stMetaData.iBuff = sizeof(stRoleData);

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

int z_cs_kick_off(ZONESVRENV* pstEnv ,const char *pszRoleName)
{
	Player *pstPlayer;

	pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pszRoleName);
	if (pstPlayer)
	{
		player_kickoff(pstEnv->pstAppCtx, pstEnv, pstPlayer, ZONEERR_KICK212);
	}
	return 0;
}

int z_cs_del_role(ZONESVRENV* pstEnv, ZONEDELROLEREQ *pstDelRoleReq, ASYNREQ *pstAsynReq)
{
	Player *pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstDelRoleReq->RoleName);

	if (pstPlayer)
	{
		
		return -1;
	}

	return z_cs_select_roledata(pstEnv, pstDelRoleReq,pstAsynReq);
	
}

int z_cs_ip_limit(ZONESVRENV* pstEnv, ZONEIPLIMITREQ *pstIpLimit)
{
	int i;
	Player *pstPlayer;
	
	for (i=0; i<MAX_PLAYER; i++)
	{
		pstPlayer = (Player *)tmempool_get_bypos(pstEnv->pstPlayerPool, i);

		if (pstPlayer && ((int)pstPlayer->stConnectInfo.stExtraInfo.IPInfo.ip&pstIpLimit->Mask) == 
						(pstIpLimit->IP&pstIpLimit->Mask))
		{
			player_kickoff(pstEnv->pstAppCtx, pstEnv, pstPlayer, ZONEERR_KICK213);
		}
	}
	return 0;
}

static int z_cs_set_perm(ZONESVRENV* pstEnv, ZONESETROLEPERMREQ *pstReq)
{
	
	Player *pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstReq->RoleName);
	if (!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		return -1;
	}

	pstPlayer->stRoleData.Perm = pstReq->Perm;
	return 0;
}

static int z_cs_set_acc_uinflag(ZONESVRENV* pstEnv, ACCSETUINFLAGREQ *pstReq)
{
	
	Player *pstPlayer = z_loginname_player(pstEnv->pstAppCtx, pstEnv,pstReq->AccIDInfo.LoginName);
	if (!pstPlayer)
	{
		return -1;
	}

	pstPlayer->UinFlag = pstReq->UinFlag;
	
	if (pstPlayer->UinFlag & UIN_FLAG_SAFE_EMAIL)
	{
		task_player_trace(pstEnv,pstPlayer,TASK_OBJ_BIND_SAFE_EMAIL);
	}
	
	return 0;
}

static int z_set_role_up_uincach(ZONESVRENV* pstEnv, ZONESETROLEDETAILREQ *pstReq)
{
	UinCache *pstUinCache;
	UinCache stUinHashData;
	int i;
	
	stUinHashData.iUin = pstReq->Uin;
	pstUinCache = (UinCache *)sht_find(pstEnv->pstShtUinCache,&stUinHashData,UinHashCmp,UinHashCode);
	if (pstUinCache)
	{
		for (i=0; i<pstUinCache->ucRoleNum; i++)
		{
			if (pstUinCache->astRoleListInfo[i].RoleID == pstReq->RoleID)
			{
				pstUinCache->astRoleListInfo[i].Level = pstReq->Level;
				pstUinCache->astRoleListInfo[i].Map = pstReq->MapID;
				break;
			}
		}
	}
	
	return 0;
}

int z_ss_force_msg_req(ZONESVRENV* pstEnv, const FORCEMSGREQ*pstCtlReq)
{
	SSBULLETIN stReq;
	struct tm stTm;
	time_t tTime;

	memset(&stReq, 0, sizeof(stReq));
	stReq.ActiveTime = pstCtlReq->TLife;
	if (NULL == strptime(pstCtlReq->TStart, "%Y-%m-%d %H:%M:%S", &stTm))
	{
		stReq.ActiveTimeStart = 0;
	}
	else
	{
		tTime = mktime(&stTm);
		if (tTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			stReq.ActiveTimeStart = 0;
		}
		else
		{
			stReq.ActiveTimeStart = tTime;
		}
	}
	stReq.Core.IconID = pstCtlReq->IconID;
	stReq.Core.ID = z_gen_bulletin_id();
	stReq.Core.RollInterval = pstCtlReq->Interval;
	stReq.Core.Type =0;
	if (pstCtlReq->Type == 0)
		stReq.Core.Type  =1;
	else if(pstCtlReq->Type == 1)
		stReq.Core.Type = 2;
	
	strncpy(stReq.Core.Text, pstCtlReq->Msg, sizeof(stReq.Core.Text));
	if(z_add_bulletin(pstEnv, &stReq) < 0)
	{
		return -1;
	}
	return 0;
}

int ss_reload_server(ZONESVRENV *pstEnv,
                              const ZONECTLRELOADSERVERREQ *pstReq)
{
	int iRet = 0;
	SSPKG stSSPkg;
	ZONECTLRELOADSERVERRES *pstRes = 
		&stSSPkg.Body.CsCtrL.CsCmd.CtlReloadServerRes;

	pstRes->Core.Succeeded = 0;

	iRet = z_reload_self();

	if (0 == iRet)
	{
		pstRes->Core.Succeeded = 1;
	}
	

	ss_init_head(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_RELOAD_SERVER_RES;
	pstRes->Extra = pstReq->Extra;
	ss_sendto_cs_svr(pstEnv, &stSSPkg);
	return 0;
}

int z_cs_svr_msg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, char *pszMsg, int iMsgLen)
{
	SSPKG stSSPkg;
	ZONEBLOCKPLAYERREQ stBlockReq;
	ZONESILENCEROLEREQ stSilenceReq;
	ZONEGETROLEDETAILREQ stGetRoleInfoReq;
	ZONEGETROLESKILLINFOREQ stGetRoleSkillReq;
	ZONEGETROLETASKREQ stGetRoleTaskReq;
	ZONEGETROLEDIATHESISREQ stGetRoleDiathsisReq;
	ZONEGETROLEITEMLISTREQ stGetRoleItemReq;
	ZONECHGROLENAMEREQ stChgRoleNameReq;
	ZONECHGROLEPOSREQ stChgRolePosReq;
	ZONEGETROLERIDEINFOREQ stGetRoleRideReq;
	GETROLEQIYUELISTREQ stGetRoleQiYueListReq;
	ZONEGETROLEPETINFOREQ stGetRolePetReq;
	ZONEGETROLEPACKAGEREQ stGetRolePackageReq;
	ZONEGETPACKAGEITEMINFOREQ stGetPackageItemInfoReq ;

	ZONEROLELOGOUTREQ stRoleLogoutReq;
	ZONEDELROLEREQ stDelRoleReq;
	ZONEGETFIRENDREQ stGetFriendReq;
	ZONEGETBLACKREQ  stGetBlackReq;
	ZONEIPLIMITREQ stIpLimitReq;
	Player *pstPlayer;
	ASYNREQ *pstAsynReq;

	int iRet = 0, iErr;

	iErr = ss_unpack(pstEnv->pstSSHan, pszMsg, iMsgLen, &stSSPkg, 0);
	if(iErr < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_unpack fail, reason : %s\n",
			         tdr_error_string(iErr));
		return -1;
	}

	if(stSSPkg.Head.Cmd != CS_CTRL)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "stSSPkg.Head.Cmd err");
		return -1;
	}

	switch(stSSPkg.Body.CsCtrL.Cmd)
	{
	case ZONE_REQ:
		break;
	
	case CTL_SEND_MAIL_REQ:
		return ss_mail_sys_send(pstEnv,
		                        &stSSPkg.Body.CsCtrL.CsCmd.CtlSendMailReq,
		                        MAIL_FLAG_SYSTEM);
		break;

	case CTL_DEL_ROLE_MAIL_REQ:
		return ss_mail_delete(pstEnv, &stSSPkg.Body.CsCtrL.CsCmd.CtlDelMailReq);
		break;

	case CTL_BLOCK_ACCOUNT_REQ:
		return z_block_account(pstEnv, 
			                   &stSSPkg.Body.CsCtrL.CsCmd.CtlBlockAccountReq);
		break;

	case CTL_SILENCE_ACCOUNT_REQ:
		return z_silence_account(pstEnv, 
			                   &stSSPkg.Body.CsCtrL.CsCmd.CtlSilenceAccountReq);
		break;

	case CTL_GET_BULLETIN_LIST_REQ:
		return z_get_bulletin_list(pstEnv, 
			                  &stSSPkg.Body.CsCtrL.CsCmd.CtlGetBulletinListReq);
		break;

	case CTL_ADD_BULLETIN_REQ:
		return ss_add_bulletin(pstEnv, 
			                   &stSSPkg.Body.CsCtrL.CsCmd.CtlAddBulletinReq);
		break;

	case CTL_DEL_BULLETIN_REQ:
		return ss_del_bulletin(pstEnv, 
			                   &stSSPkg.Body.CsCtrL.CsCmd.CtlDelBulletinReq);
		break;

	case CTL_GET_CLAN_ALL_INFO_REQ:
		return ss_clan_all_info(pstEnv,
		                       &stSSPkg.Body.CsCtrL.CsCmd.CtlGetClanAllInfoReq);
		break;

	case CTL_DEL_CLAN_MEMBER_REQ:
		return ss_clan_del_member(pstEnv,
		                        &stSSPkg.Body.CsCtrL.CsCmd.CtlDelClanMemberReq);
		break;

	case CTL_DISBAND_CLAN_REQ:
		return ss_clan_disband(pstEnv,
		                       &stSSPkg.Body.CsCtrL.CsCmd.CtlDisbandClanReq);
		break;

	case CTL_CLEAR_CLAN_DIPLOMATISM_REQ:
		return ss_clan_clear_diplomatism(pstEnv,
		                 &stSSPkg.Body.CsCtrL.CsCmd.CtlClearClanDiplomatismReq);
		break;

	case UPDATE_ROLELIST_CACHE_REQ:
		return ss_update_rolelist(pstEnv, 
			                 &stSSPkg.Body.CsCtrL.CsCmd.UpdateRoleListCacheReq);
		break;

	case CTL_DEL_CLAN_DIPLOMATISM_REQ:
		return ss_clan_del_diplomatism(pstEnv,
		                   &stSSPkg.Body.CsCtrL.CsCmd.CltDelClanDiplomatismReq);
		break;

	case CTL_EXCHANGE_CLAN_LEADER_REQ:
		return ss_clan_exchange_leader(pstEnv,
		                   &stSSPkg.Body.CsCtrL.CsCmd.CltExchangeClanLeaderReq);
		break;
	case CHG_POS_UP_CACHE_REQ:
		return cs_chg_pos_up_cache(pstAppCtx, pstEnv, &stSSPkg.Body.CsCtrL.CsCmd.ChgPosUpCacheReq);
		break;
		
	case CTL_SET_CLAN_BASE_ATTR_REQ:
		return ss_set_clan_base_attr(pstEnv,
		                 &stSSPkg.Body.CsCtrL.CsCmd.CtlSetClanBaseAttrReq);
		break;
	case CTL_RELOAD_SERVER_REQ:
		return ss_reload_server(pstEnv,
		                 &stSSPkg.Body.CsCtrL.CsCmd.CtlReloadServerReq);
		break;


	default:
		tlog_error(pstEnv->pstLogCat, 0, 0, "stSSPkg.Body.CsCtrL.Cmd err");
		return -1;
		break;
	}

	pstAsynReq = &stSSPkg.Body.CsCtrL.CsCmd.ZoneReq;
	
	switch(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Cmd)
	{
	case CTL_SHOP_CGI_REQ:
		{
			SHOPCGIRES stRes;
			shop_cgi_req(pstEnv, (SHOPCGIREQ*)(pstAsynReq->Data),  &stRes);
			z_shop_cgi_to_cs(pstAppCtx, pstEnv, &stSSPkg, &stRes);
		}
		break;
		
	case FORCE_MSG_REQ:
		if (sizeof(FORCEMSGREQ) != pstAsynReq->DataLen)
		{
			return -1;
		}

		if (z_ss_force_msg_req(pstEnv, (FORCEMSGREQ*)(pstAsynReq->Data)) == 0)
		{
			z_sendto_cs_svr(pstAppCtx, pstEnv, &stSSPkg,1);
		}
		break;
	case SET_ROLE_DETAIL_REQ:
		if (sizeof(ZONESETROLEDETAILREQ) != pstAsynReq->DataLen)
		{
			return -1;
		}

		z_set_role_up_uincach(pstEnv, (ZONESETROLEDETAILREQ *)(pstAsynReq->Data));
		break;
	case SET_ACCOUNT_UIN_FLAG_REQ:
		if (sizeof(ACCSETUINFLAGREQ) != pstAsynReq->DataLen)
		{
			return -1;
		}

		z_cs_set_acc_uinflag(pstEnv, (ACCSETUINFLAGREQ *)(pstAsynReq->Data));
		break;
	case SET_ROLE_PERM_REQ:
		if (sizeof(ZONESETROLEPERMREQ) != pstAsynReq->DataLen)
		{
			return -1;
		}

		if (z_cs_set_perm(pstEnv, (ZONESETROLEPERMREQ*)(pstAsynReq->Data)))
		{
			return -1;
		}

		if(z_sendto_cs_svr(pstAppCtx, pstEnv, &stSSPkg,1) < 0)
		{
			return -1;
		}	
		
		break;
	case IP_LIMIT_REQ:
		if (pstAsynReq->DataLen != sizeof(ZONEIPLIMITREQ))
		{
			return -1;
		}
		memcpy (&stIpLimitReq, pstAsynReq->Data, sizeof(stIpLimitReq));
		 z_cs_ip_limit(pstEnv, &stIpLimitReq);
		
		
		break;
	case DEL_ROLE_REQ:	// cs_svr 删除角色
		if (pstAsynReq->DataLen != sizeof(ZONEDELROLEREQ))
		{
			return -1;
		}
		memcpy(&stDelRoleReq, pstAsynReq->Data, sizeof(stDelRoleReq));
		
		z_cs_del_role(pstEnv, &stDelRoleReq, pstAsynReq);
		break;
	case SET_ROLE_LOGOUT_REQ:
		if (pstAsynReq->DataLen != sizeof(ZONEROLELOGOUTREQ))
		{
			return -1;
		}

		memcpy(&stRoleLogoutReq, pstAsynReq->Data, sizeof(stRoleLogoutReq));
		z_cs_kick_off(pstEnv ,stRoleLogoutReq.RoleName);
		if(z_sendto_cs_svr(pstAppCtx, pstEnv, &stSSPkg,iRet) < 0)
		{
			return -1;
		}	
		

		break;
	case BLOCKPLAYER_REQ:
		if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(ZONEBLOCKPLAYERREQ))
		{
			return -1;
		}
		
		memcpy(&stBlockReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data, 
			     sizeof(ZONEBLOCKPLAYERREQ));
		// 封 在线角色
		if(stBlockReq.Type == 0)		
		{
			iRet = block_player_of_online(pstAppCtx, pstEnv, &stBlockReq);
			if(z_sendto_cs_svr(pstAppCtx, pstEnv, &stSSPkg,iRet) < 0)
			{
				return -1;
			}	
		}
		
		break;
		
	case SILENCE_ROLE_REQ:
		if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(ZONESILENCEROLEREQ))
		{
			return -1;
		}
		
		memcpy(&stSilenceReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data,
			     sizeof(ZONESILENCEROLEREQ));
		// 禁言在线角色
		iRet = silence_player_of_online(pstAppCtx, pstEnv,  &stSilenceReq);
		if(z_sendto_cs_svr(pstAppCtx, pstEnv, &stSSPkg,iRet) < 0)
		{
			return -1;
		}
		
		break;

		// 获取在线角色信息
	case GET_ROLE_DETAIL_REQ:
		if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(ZONEGETROLEDETAILREQ))
		{
			return -1;
		}
		memcpy(&stGetRoleInfoReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data, sizeof(stGetRoleInfoReq));
		return  z_get_role_info_to_cs(pstAppCtx, pstEnv,&stSSPkg, stGetRoleInfoReq.RoleName, GET_ROLE_DETAIL_REQ);
		break;

		// 跳转在线角色
	case CHG_ROLE_POS_REQ:
		if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(stChgRolePosReq))
		{
			return -1;
		}

		memcpy(&stChgRolePosReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data, sizeof(stChgRolePosReq));

		pstPlayer = z_name_player(pstAppCtx,pstEnv,stChgRolePosReq.RoleName);
		if (NULL == pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			return -1;
		}
		
		if (player_goto(pstEnv, pstPlayer, stChgRolePosReq.Map, stChgRolePosReq.X, stChgRolePosReq.Y,pstPlayer->stOnline.iMapPos) == 0)
		{
			return  z_get_role_info_to_cs(pstAppCtx, pstEnv,&stSSPkg, stChgRolePosReq.RoleName,CHG_ROLE_POS_REQ);
		}
		
		break;
		// 获取在线角色技能信息
	case GET_ROLE_SKILLINFO_REQ:
		if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(ZONEGETROLEDETAILREQ))
		{
			return -1;
		}

		memcpy(&stGetRoleSkillReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data, sizeof(stGetRoleSkillReq));
		return  z_get_role_info_to_cs(pstAppCtx, pstEnv,&stSSPkg, stGetRoleSkillReq.RoleName, GET_ROLE_SKILLINFO_REQ);
		break;

		// 获取角色素质信息
	case GET_ROLE_DIATHESIS_REQ:
		if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(ZONEGETROLEDIATHESISREQ))
		{
			return -1;
		}
		memcpy(&stGetRoleDiathsisReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data, sizeof(stGetRoleDiathsisReq));
		return  z_get_role_info_to_cs(pstAppCtx, pstEnv,&stSSPkg, stGetRoleDiathsisReq.RoleName, GET_ROLE_DIATHESIS_REQ);
		break;

		// 获取配件信息
	/*case GET_ROLE_FITTING_REQ:
		if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(ZONEGETROLEFITTINGSREQ))
		{
			return -1;
		}
		memcpy(&stGetRoleFittingReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data, sizeof(stGetRoleFittingReq));
		return  z_get_role_info_to_cs(pstAppCtx, pstEnv,&stSSPkg, stGetRoleFittingReq.RoleName, GET_ROLE_FITTING_REQ);
		break;*/

		// 获取任务信息
	case GET_ROLE_TASK_REQ:
		if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(ZONEGETROLETASKREQ))
		{
			return -1;
		}
		memcpy(&stGetRoleTaskReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data, sizeof(stGetRoleTaskReq));
		return  z_get_role_info_to_cs(pstAppCtx, pstEnv,&stSSPkg, stGetRoleTaskReq.RoleName, GET_ROLE_TASK_REQ);
		break;
		
		// 获取道具信息
	case GET_ROLE_ITEMLIST_REQ:
		if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(ZONEGETROLEITEMLISTREQ))
		{
			return -1;
		}
		memcpy(&stGetRoleItemReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data, sizeof(stGetRoleItemReq));

		return  z_get_role_info_to_cs(pstAppCtx, pstEnv,&stSSPkg, stGetRoleItemReq.RoleName, GET_ROLE_ITEMLIST_REQ);
		break;
		
		// 角色改名
	case CHG_ROLE_NAME_REQ:
		if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(ZONECHGROLENAMEREQ))
		{
			return -1;
		}

		memcpy(&stChgRoleNameReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data, sizeof(stChgRoleNameReq));

		return z_chg_role_name_to_cs(pstEnv, &stChgRoleNameReq, &stSSPkg);
		break;
		// 获取在线角色骑乘信息
	case GET_ROLE_RIDE_REQ:
		if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(ZONEGETROLERIDEINFOREQ))
		{
			return -1;
		}

		memcpy(&stGetRoleRideReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data, sizeof(stGetRoleRideReq));
		return  z_get_role_info_to_cs(pstAppCtx, pstEnv,&stSSPkg, stGetRoleRideReq.RoleName, GET_ROLE_RIDE_REQ);
		break;
	case GET_ROLE_PACKAGE_REQ:
		if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(ZONEGETROLEPACKAGEREQ))
		{
			return -1;
		}

		memcpy(&stGetRolePackageReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data, sizeof(stGetRolePackageReq));
		return  z_get_role_info_to_cs(pstAppCtx, pstEnv,&stSSPkg, stGetRolePackageReq.RoleName, GET_ROLE_PACKAGE_REQ);
		break;
	case GET_ROLE_PACKAGE_ITEM_REQ:
		if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(ZONEGETPACKAGEITEMINFOREQ))
		{
			return -1;
		}

		memcpy(&stGetPackageItemInfoReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data, sizeof(stGetPackageItemInfoReq));
		return  z_get_role_info_to_cs(pstAppCtx, pstEnv,&stSSPkg, stGetPackageItemInfoReq.RoleName, GET_ROLE_PACKAGE_ITEM_REQ);
		break;		
	case GET_ROLE_QIYUELIST_REQ:
		if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(GETROLEQIYUELISTREQ))
		{
			return -1;
		}

		memcpy(&stGetRoleQiYueListReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data, sizeof(stGetRoleQiYueListReq));
		return  z_get_role_info_to_cs(pstAppCtx, pstEnv,&stSSPkg, stGetRoleQiYueListReq.RoleName, GET_ROLE_QIYUELIST_REQ);
		break;	
	case GET_ROLE_PET_REQ:
		if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(ZONEGETROLEPETINFOREQ))
		{
			return -1;
		}

		memcpy(&stGetRolePetReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data, sizeof(stGetRolePetReq));
		return  z_get_role_info_to_cs(pstAppCtx, pstEnv,&stSSPkg, stGetRolePetReq.RoleName, GET_ROLE_PET_REQ);
		break;
	case GET_ROLE_FRIEND_REQ:
		if (stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(stGetFriendReq))
		{
			return -1;
		}
		memcpy(&stGetFriendReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data, sizeof(stGetFriendReq));
		return  z_get_role_info_to_cs(pstAppCtx, pstEnv,&stSSPkg, stGetFriendReq.RoleName, GET_ROLE_FRIEND_REQ);
		break;
	case GET_ROLE_BLACK_REQ:
		if (stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(stGetBlackReq))
		{
			return -1;
		}
		memcpy(&stGetBlackReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data, sizeof(stGetBlackReq));
		return  z_get_role_info_to_cs(pstAppCtx, pstEnv,&stSSPkg, stGetBlackReq.RoleName, GET_ROLE_BLACK_REQ);
		break;
	

		// 增加一个称号
	case ADD_ROLE_DESIGNATION_REQ:
		{
			ZONEADDROLEDESIGNATIONREQ stReq;
			ZONEADDROLEDESIGNATIONRES stRes;

			if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(stReq))
			{
				return -1;
			}

			memcpy(&stReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data,
			       sizeof(stReq));

			stRes.Succeeded = 0;
			pstPlayer = z_name_player(pstAppCtx, pstEnv, stReq.RoleName);
			if(pstPlayer && pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
			{
				if(player_designation_add(pstEnv, pstPlayer,
				                          stReq.DesignationID) == 0)
				{
					stRes.Succeeded = 1;
				}
			}

			return z_send_result_to_cs(
				pstAppCtx,
				pstEnv,
				&stSSPkg.Body.CsCtrL.CsCmd.ZoneReq,
			    &stRes,
			    sizeof(stRes));
		}
		break;

		// 删除一个称号
	case DEL_ROLE_DESIGNATION_REQ:
		{
			ZONEDELROLEDESIGNATIONREQ stReq;
			ZONEDELROLEDESIGNATIONRES stRes;

			if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(stReq))
			{
				return -1;
			}

			memcpy(&stReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data,
			       sizeof(stReq));

			stRes.Succeeded = 0;
			pstPlayer = z_name_player(pstAppCtx, pstEnv, stReq.RoleName);
			if(pstPlayer && pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
			{
				if(player_designation_del(pstEnv, pstPlayer,
				                          stReq.DesignationID) == 0)
				{
					stRes.Succeeded = 1;
				}
			}

			return z_send_result_to_cs(
				pstAppCtx,
				pstEnv,
				&stSSPkg.Body.CsCtrL.CsCmd.ZoneReq,
			    &stRes,
			    sizeof(stRes));
		}
		break;

	case GET_ROLE_DESIGNATION_LIST_REQ:
		{
			ZONEGETROLEDESIGNATIONLISTREQ stReq;
			if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(stReq))
			{
				return -1;
			}

			memcpy(&stReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data, sizeof(stReq));
			return z_get_role_info_to_cs(pstAppCtx, pstEnv, &stSSPkg,
			                             stReq.RoleName,
			                             GET_ROLE_DESIGNATION_LIST_REQ);
		}
		break;

		// 设置/添加一个声望
	case SET_ROLE_RESPACT_REQ:
		{
			ZONESETROLERESPACTREQ stReq;
			ZONESETROLERESPACTRES stRes;

			if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(stReq))
			{
				return -1;
			}

			memcpy(&stReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data,
			       sizeof(stReq));

			stRes.Succeeded = 0;
			pstPlayer = z_name_player(pstAppCtx, pstEnv, stReq.RoleName);
			if(pstPlayer)
			{
				if(player_respact_inc(pstEnv, pstPlayer, stReq.RespactPartyID,
				                      stReq.RespactValue, 0) == 0)
				{
					stRes.Succeeded = 1;
				}
			}

			return z_send_result_to_cs(
				pstAppCtx,
				pstEnv,
				&stSSPkg.Body.CsCtrL.CsCmd.ZoneReq,
			    &stRes,
			    sizeof(stRes));
		}
		break;

		// 删除一个声望信息
	case DEL_ROLE_RESPACT_REQ:
		{
			ZONEDELROLERESPACTREQ stReq;
			ZONEDELROLERESPACTRES stRes;

			if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(stReq))
			{
				return -1;
			}

			memcpy(&stReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data,
			       sizeof(stReq));

			stRes.Succeeded = 0;
			pstPlayer = z_name_player(pstAppCtx, pstEnv, stReq.RoleName);
			if(pstPlayer && pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
			{
				if(player_respact_del(pstEnv, pstPlayer,
				                      stReq.RespactPartyID) == 0)
				{
					stRes.Succeeded = 1;
				}
			}

			return z_send_result_to_cs(
				pstAppCtx,
				pstEnv,
				&stSSPkg.Body.CsCtrL.CsCmd.ZoneReq,
			    &stRes,
			    sizeof(stRes));
		}
		break;

	case GET_ROLE_RESPACT_LIST_REQ:
		{
			ZONEGETROLERESPACTLISTREQ stReq;
			if(stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(stReq))
			{
				return -1;
			}

			memcpy(&stReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data, sizeof(stReq));
			return z_get_role_info_to_cs(pstAppCtx, pstEnv, &stSSPkg,
			                             stReq.RoleName,
			                             GET_ROLE_RESPACT_LIST_REQ);
		}
		break;
	case GET_LIFT_SKILL_REQ:
		{
			GETLIFTSKILLREQ stReq;
			if (stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.DataLen != sizeof(stReq))
			{
				return -1;
			}

			memcpy(&stReq, stSSPkg.Body.CsCtrL.CsCmd.ZoneReq.Data, sizeof(stReq));
			return z_get_role_info_to_cs(pstAppCtx, pstEnv, &stSSPkg,
				                             stReq.RoleName,
				                             GET_LIFT_SKILL_REQ);
		}
		break;

	default:
		break;
		
	}
	return 0;
}

int z_world_tick(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iBusy)
{
	//先大致估算处理100个消息< 5 ms
	
	if (pstEnv->cShutdown > 1)
	{
		return 0;
	}
	
	if (iBusy)
	{
		z_active_player(pstAppCtx, pstEnv, 30);
		z_active_monster(pstAppCtx, pstEnv, 300);
		z_active_team(pstAppCtx, pstEnv, 3);
		z_active_pworld(pstAppCtx, pstEnv, 12);
		z_active_mapinst(pstAppCtx, pstEnv, 30);
		z_active_item(pstAppCtx, pstEnv, 300);
		z_active_clan(pstAppCtx, pstEnv, 5);
		z_active_mail(pstAppCtx, pstEnv, 5);
		//z_active_bulletin(pstAppCtx, pstEnv, 5);
		z_active_npc(pstAppCtx, pstEnv, 30);
		z_active_store( pstAppCtx, pstEnv, 5);
		z_active_home( pstAppCtx, pstEnv, 5);
	}
	else
	{
		z_active_player(pstAppCtx, pstEnv, 120);
		z_active_monster(pstAppCtx, pstEnv, 1200);
		z_active_team(pstAppCtx, pstEnv, 12);
		z_active_pworld(pstAppCtx, pstEnv, 30);
		z_active_mapinst(pstAppCtx, pstEnv, 60);
		z_active_item(pstAppCtx, pstEnv, 1200);
		z_active_clan(pstAppCtx, pstEnv, 20);
		z_active_mail(pstAppCtx, pstEnv, 20);
		//z_active_bulletin(pstAppCtx, pstEnv, 20);
		z_active_npc(pstAppCtx, pstEnv, 120);
		z_active_store( pstAppCtx, pstEnv, 20);
		z_active_home( pstAppCtx, pstEnv, 20);
		//z_fill_gd_cache(pstEnv, 2);
	}

	return 0; 
}


int z_ss_coroutine_rpc_req(TAPPCTX * pstAppCtx, ZONESVRENV* pstEnv,
	COROUTINERPC *pstCoroutineRpc)
{
	SSPKG stPkg;
	
	stPkg.Body.CoroutineRpc = *pstCoroutineRpc;

	Z_SSHEAD_INIT(&stPkg.Head, SS_COROUTINE_RPC);
	z_ssmsg_send(pstEnv, &stPkg, pstCoroutineRpc->DstID);	

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_ss_chat(ZONESVRENV* pstEnv, SSCHAT *pstChat)
{
	CSPKG *pstResPkg = &pstEnv->stCSRes;
	CSCHATRES *pstChatRes = &pstResPkg->Body.ChatRes;

	Z_CSHEAD_INIT(&pstResPkg->Head, CHAT_RES);
	
	pstChatRes->LinkLen = pstChat->LinkLen;
	STRNCPY(pstChatRes->From, pstChat->From, sizeof(pstChatRes->From));
	STRNCPY(pstChatRes->Content, pstChat->Content, sizeof(pstChatRes->Content));
	if (pstChatRes->LinkLen > 0)
	{
		memcpy(pstChatRes->LinkContent, pstChat->LinkContent, pstChat->LinkLen);
	}

	pstChatRes->OnlineType = ROLE_MUD_ONLINE;
	pstChatRes->WorldID = pstChat->WorldID;

	switch(pstChat->ChatType)
	{
	case SS_CHAT_FRIEND:
		{
			Player *pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
			                                  pstChat->ToData.ToName);
			if(!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				return -1;
			}
			pstChatRes->Type = CHAT_FRIEND;
			return z_cltmsg_send(pstEnv, pstPlayer, pstResPkg);
		}
		break;
	case SS_CHAT_CLAN:
		{
			ZoneClan *pstZoneClan = z_clanid_clan(
				pstEnv,
				&pstChat->ToData.ClanInfo);
			pstChatRes->Type = CHAT_CLAN;
			if(!pstZoneClan)
			{
				return -1;
			}
			return player_clan_broadcast_except(pstEnv, pstZoneClan, pstResPkg,
			                                    0, NULL);
		}
		break;
	case SS_CHAT_PRIVATE:
		{
			CSPKG stEcho;
			Player *pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
			                                  pstChat->ToData.ToName);
			if(!pstPlayer || 
				(pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN && pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING))
			{
				z_send_sys_str_mud(pstEnv->pstAppCtx, pstEnv, pstChat->From, SYS_MIDDLE2, ZONEERR_MUD104, pstChat->ToData.ToName);
				return -1;
			}
			//回显
			
			memset(&stEcho,0,sizeof(stEcho));
			stEcho.Body.ChatReq.Res = 1;//必须的,客户端需要
			STRNCPY(stEcho.Body.ChatReq.ToName , pstChat->ToData.ToName,sizeof(stEcho.Body.ChatReq.ToName));
			STRNCPY(stEcho.Body.ChatReq.Content , pstChat->Content,sizeof(stEcho.Body.ChatReq.Content));
			stEcho.Body.ChatReq.OnlineType = ROLE_GAME_ONLINE;
			stEcho.Body.ChatReq.LinkLen = pstChat->LinkLen;
			memcpy(stEcho.Body.ChatReq.LinkContent,pstChat->LinkContent,sizeof(stEcho.Body.ChatReq.LinkContent));
			stEcho.Body.ChatReq.Type = pstChat->ChatType;
			Z_CSHEAD_INIT(&stEcho.Head, CHAT_REQ);
			z_pkg_to_mud(pstEnv, &stEcho, pstChat->From, 0, PKG_TRANS_PLAYER);
			
			pstChatRes->Type = CHAT_PRIVATE;
			return z_cltmsg_send(pstEnv, pstPlayer, pstResPkg);
		}
		break;
	case SS_CHAT_SPAN:
		{
			pstChatRes->Type = CHAT_SPAN;
			pstChatRes->OnlineType = ROLE_GAME_ONLINE;
			
			return z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, pstResPkg);
		}
		break;
	default:
		return -1;
		break;
	}
	
	return 0;
}

int z_ss_pkg_trans(ZONESVRENV* pstEnv, SSPKG *pstPkg)
{
	SSPKGTRANS* pstTrans = &pstPkg->Body.PkgTansInfo;
	CSPKG stCs;
	Player* pstPlayer = NULL;
	ZoneClan *pstZoneClan = NULL;

	Z_CSHEAD_INIT(&stCs.Head, pstTrans->Cmd);
	stCs.Body = pstPkg->Body.PkgTansInfo.Body;
	if (CHAT_RES == pstTrans->Cmd)
	{
		stCs.Body.ChatRes.OnlineType = ROLE_MUD_ONLINE;
	}
	switch(pstTrans->Type)
	{
	case PKG_TRANS_ALL:
		z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stCs);
		break;
	case PKG_TRANS_FRIEND:
		break;
	case PKG_TRANS_CLAN:
		pstZoneClan = z_id_clan(pstEnv, pstTrans->ClanID);
		if (pstZoneClan)
		{
			player_clan_broadcast(pstEnv, pstZoneClan, &stCs, 0);
		}
		break;
	case PKG_TRANS_PLAYER:
		pstPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pstTrans->szName);
		if (pstPlayer)
		{
			z_cltmsg_send(pstEnv,pstPlayer,&stCs);
		}
		break;
	case PKG_TRANS_SVR:
		break;
	default:
		break;
	}


	
	return 0;
}

int z_sync_mud_time(ZONESVRENV* pstEnv)
{
	SSCMDDATA stData;
	stData.SyncTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	z_cmd_to_mud(pstEnv,NULL, NULL,SS_CMD_SYNC_TIME,&stData);
	return 0;
}


int z_ss_cmd_oplog_proxy(ZONESVRENV *pstEnv,OP_LOG_META* pstOplogProxy)
{
	return z_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, pstOplogProxy,pstOplogProxy->OpLogFrom);
}


int z_ss_cmd_req(ZONESVRENV* pstEnv, SSPKG *pstPkg)
{
	SSCMDREQ* pstReq = &pstPkg->Body.CmdReq;
	Player* pstPlayer = NULL;
	
	switch(pstReq->Cmd)
	{
	case SS_CMD_FRIEND_DETAIL://发好友的详细信息	
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,pstReq->szName);
		if (pstPlayer)
		{
			relation_chg_clt_detail_mud(pstEnv,pstPlayer,pstReq->szReceiver,RELATION_TYPE_FRIEND);
		}
		break;
		
	case SS_CMD_FRIEND_ADD:
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,pstReq->szName);
		if (pstPlayer)
		{
			relation_friend_add_mud(pstEnv,pstPlayer,pstReq->szReceiver,
				&pstReq->Data.FriendAdd.BuddyMember, &pstReq->Data.FriendAdd.BuddyInfo);
		}
		break;
		
	case SS_CMD_CLAN_ONLINE_MEMBER:
		clan_online_member_to_mud(pstEnv,pstReq->szReceiver,&pstReq->Data.ClanID);
		break;

	case SS_CMD_SYNC_TIME:
		if (SYNC_TIME_SEC <= abs(pstEnv->pstAppCtx->stCurr.tv_sec - pstReq->Data.SyncTime))
		{
			z_sync_mud_time(pstEnv);
		}
		break;
		
	case SS_CMD_OPLOG_PROXY:
		z_ss_cmd_oplog_proxy(pstEnv,&pstReq->Data.OplogProxy);
		break;
		
	case SS_CMD_ACC_NOTICE:
		{
			pstPlayer = player_get_by_uin(pstEnv, pstReq->Data.AccUin);
			if (!pstPlayer)
			{
				return -1;
			}
						
			//player_kickoff(pstAppCtx, pstEnv, pstPlayer, "您的帐号登录了引力空间");
			
			//反踢MUD	
			{
				SSCMDDATA stData;

				stData.KickOffAccUin = pstPlayer->stRoleData.Uin;
				z_cmd_to_mud(pstEnv,NULL,NULL,SS_CMD_KICK_OFF,&stData);
			}
		}
		break;

	case SS_CMD_MAIL_PROXY:
		ss_mail_sys_send(pstEnv, &pstReq->Data.MailProxy.MailReq, pstReq->Data.MailProxy.Flags);
		break;

	case SS_CMD_RANGE_PROXY:
		range_set(pstEnv, &pstReq->Data.RangeProxy);
		break;

	case SS_CMD_WATCH_PROXY:
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstReq->szName);
		if (pstPlayer)
		{
			Player *pstPlayerTarget;

			pstPlayerTarget = z_name_player(pstEnv->pstAppCtx, pstEnv, pstReq->szReceiver);

			if (NULL == pstPlayerTarget)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SVR206, pstReq->szReceiver);
			}
			else
			{
				player_watch_role(pstEnv, pstPlayer, pstPlayerTarget);
			}
		}
		break;

	case SS_CMD_SYNC_VIP_BAR_INFO:
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstReq->szName);
		if (pstPlayer)
		{
			pstPlayer->stOnline.stVipBarInfo = pstReq->Data.SyncVipBarInfo.VipBarInfo;
		}
		break;
		
	case SS_CMD_BATTLE_MEMID_NOTICE:
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstReq->szName);
		if (pstPlayer)
		{
			pstPlayer->iBattleMemID = pstReq->Data.BattleMemidNotice.BattleMemID;
		}
		break;
	case SS_CMD_RANGE_WATCH_PROXY:
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstReq->szName);
		if (pstPlayer)
		{
			Player *pstPlayerTarget;

			pstPlayerTarget = z_name_player(pstEnv->pstAppCtx, pstEnv, pstReq->szReceiver);
			if (pstPlayerTarget)
			{
				return player_watch_role(pstEnv, pstPlayer, pstPlayerTarget);
			}
			else
			{
				 return player_watch_offline_player(pstEnv, pstPlayer, pstReq->szReceiver, 0);
			}
		}
		break;
	case SS_CMD_UIN_FLAG_CHANGE:
		pstPlayer = z_loginname_player(pstEnv->pstAppCtx, pstEnv, pstReq->Data.UinFlagChange.AccName);
		if (pstPlayer)
		{
			UINFLAGCHANGE* pstUinFlagChange = &pstReq->Data.UinFlagChange; 
			player_uin_flag_chg(pstEnv,pstPlayer, pstUinFlagChange->UinFlag, pstUinFlagChange->Type);
			
		}
		break;
	case SS_CMD_BATLLE_HEART_BEAT:
		{
			BATLLEHEARTBEAT *pstBatlleHeartBeat = &pstReq->Data.BatlleHeartBeat;
			z_battle_heart_beat(pstEnv,pstBatlleHeartBeat);
		}
		break;

	default:
		break;
	}
	
	return 0;
}

int z_ss_info_get_req(ZONESVRENV* pstEnv, SSPKG *pstPkg)
{
	SSPKG stPkg;
	SSINFOGETREQ* pstReq = &pstPkg->Body.InfoGetReq;
	SSINFOGETRES* pstRes = &stPkg.Body.InfoGetRes;
	Player* pstPlayer = NULL;
	
	pstRes->Type = pstReq->Type;
	pstRes->ReqBody = pstReq->ReqBody;
	pstRes->Result = -1;
	switch(pstReq->Type)
	{
	case INFO_GET_GM:
		pstRes->Result = 0;
		if (RES_LOAD_BIN == pstEnv->pstConf->ResloadType)
		{
			pstRes->ResBody.GM = 0;
		}
		else if (RES_LOAD_DB== pstEnv->pstConf->ResloadType)
		{
			//限制普通用户
			if(pstEnv->cLimitPerm)
			{
				pstRes->ResBody.GM = 0;
			}
			else
			{
				pstRes->ResBody.GM = 1;
			}
		}
		break;
	case INFO_GET_BUDDY_MEMBER:
		pstPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pstReq->ReqBody.BuddyMember.AddFriendRes.ReqName);
		if (pstPlayer)
		{
			pstRes->Result = 0;
			relation_get_role_member(pstPlayer,&pstRes->ResBody.BuddyMemberRes.BuddyMember);
		}
		break;
	default:
		return -1;
		break;
	}
	Z_SSHEAD_INIT(&stPkg.Head, SS_INFO_GET_RES);
	z_ssmsg_send(pstEnv, &stPkg, pstEnv->iMudSvrID);
	
	return 0;
}

int z_ss_info_get_res(ZONESVRENV* pstEnv, SSPKG *pstPkg)
{
	SSINFOGETRES* pstRes = &pstPkg->Body.InfoGetRes;
	Player* pstPlayer = NULL;
	
	switch(pstRes->Type)
	{
	case INFO_GET_GM:
		break;
	case INFO_GET_BUDDY_MEMBER:
		pstPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pstRes->ReqBody.BuddyMember.Name);
		if (pstPlayer && 0 == pstRes->Result)
		{
			player_relation_addfriend_res_mud(pstEnv,pstPlayer,&pstRes->ReqBody.BuddyMember.AddFriendRes,
				&pstRes->ResBody.BuddyMemberRes.BuddyMember, &pstRes->ResBody.BuddyMemberRes.BuddyInfo);
		}
		break;
	default:
		return -1;
		break;
	}
	return 0;
}

int z_mud_msg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, char *pszMsg, int iMsgLen)
{
	SSPKG stPkgRes;
	
	if(ss_unpack(pstEnv->pstSSHan, pszMsg, iMsgLen, &stPkgRes, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "aas_svr ss_unpack fail");
		return -1;
	}

	if( RELATION_ONLINE_REQ  == stPkgRes.Head.Cmd )
	{
		relation_online_query(pstAppCtx,pstEnv,  &stPkgRes.Body.RelationOnlineReq);
	}
	else if(ROLE_ONLINE == stPkgRes.Head.Cmd )
	{
		CSPKG stPkg;
		CSONLINESVR *pstOnline = &stPkg.Body.OnlineInfo;

		pstOnline->RoleID = stPkgRes.Body.RoleOnline.RoleID;
		pstOnline->Online = stPkgRes.Body.RoleOnline.Online;
		STRNCPY(pstOnline->RoleName, stPkgRes.Body.RoleOnline.RoleName, sizeof(pstOnline->RoleName));
		
		Z_CSHEAD_INIT(&stPkg.Head, ONLINE_INFO);
		z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
	}
	else if(SS_CHAT == stPkgRes.Head.Cmd)
	{
		z_ss_chat(pstEnv, &stPkgRes.Body.ChatInfo);
	}
	else if( RELATION_ONLINE_RES  == stPkgRes.Head.Cmd )
	{
		ss_relation_online_res(pstAppCtx, pstEnv, &stPkgRes.Body.RelationOnlineRes);
	}
	else if (SS_PKG_TRANS == stPkgRes.Head.Cmd)
	{
		z_ss_pkg_trans(pstEnv,&stPkgRes);
	}
	else if (SS_CMD_REQ == stPkgRes.Head.Cmd)
	{
		z_ss_cmd_req(pstEnv,&stPkgRes);
	}
	else if (SS_INFO_GET_REQ == stPkgRes.Head.Cmd)
	{
		z_ss_info_get_req(pstEnv,&stPkgRes);
	}
	else if (SS_INFO_GET_RES == stPkgRes.Head.Cmd)
	{
		z_ss_info_get_res(pstEnv,&stPkgRes);
	}
	else if (SS_APEX == stPkgRes.Head.Cmd)
	{
		z_ss_apex_get_res(pstEnv,&stPkgRes);
	}

	tlog_debug(pstEnv->pstLogCat, 0, 0,
	           "m-->z msg:headlen=%d,bodylen=%d",
	           stPkgRes.Head.HeadLen,
	           stPkgRes.Head.BodyLen);
	
	tlog_debug_dr(pstEnv->pstLogCat, 0, 0,
	              pstEnv->pstSSHan->pstSSProtoPkgMeta, 
	              &stPkgRes, sizeof(stPkgRes), 0);
	
	return 0;
}


int z_ss_msg_trans(ZONESVRENV* pstEnv,SSPKG *pstSsPkg)
{
	CSPKG *pstPkg = &pstSsPkg->Body.SSMsgTrans.CSPkg;

	z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, pstPkg);
	return 0;
}

int z_region_msg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, char *pszMsg, int iMsgLen)
{
	Player *pstPlayer = NULL;
	SSPKG stPkgRes;
	SSAASRES *pstSSAasRes = &stPkgRes.Body.AasRes;
	
	if(ss_unpack(pstEnv->pstSSHan, pszMsg, iMsgLen, &stPkgRes, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_unpack fail");
		return -1;
	}
	switch(stPkgRes.Head.Cmd)
	{
		case AAS_GAME_RES:
			{
				if (stPkgRes.Head.Cmd != AAS_GAME_RES)
				{
					return -1;
				}

				pstPlayer = player_get_by_uin(pstEnv, pstSSAasRes->Uin);
				if (!pstPlayer)
				{
					return -1;
				}

				pstPlayer->stAasTime.tOnline  = pstSSAasRes->tOnline;
				pstPlayer->stAasTime.tOffline = pstSSAasRes->tOffline;
				pstPlayer->stAasTime.tActive = pstAppCtx->stCurr.tv_sec;
				pstPlayer->stAasTime.tNotify = 0;

				tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "uin = %d  aas start   online = %d offline = %d", 
								pstPlayer->stRoleData.Uin, pstPlayer->stAasTime.tOnline, pstPlayer->stAasTime.tOffline);	
			}
			break;
		case DEC_BILL_RES:
			{
				shopping_operate_point_res(pstAppCtx, pstEnv, &stPkgRes.Body.DecTianYuanRes);
			}
			break;
		case USER_QUERY_BILL_RES:
			{
				shopping_query_dianquan_res(pstAppCtx,pstEnv,  &stPkgRes.Body.QueryTianYuanRes);
			}
			break;
		case BILL_FREEZE_RES:
			{
				
			}
			break;
		case KICK_OFF_ACC:
			{
				pstPlayer = player_get_by_uin(pstEnv, stPkgRes.Body.KickOffAccount.Uin);
				if (!pstPlayer)
				{
					return -1;
				}
							
				//player_kickoff(pstAppCtx, pstEnv, pstPlayer, "您的帐号登录了引力空间");
				
				//反踢MUD				
				z_heartbeat_to_region(pstEnv, pstPlayer->stRoleData.Uin);
				
			}
			break;
		case BILL_QUERY_PROPSDEF_RES:
			//shopping_ss_query_propsdef_res(pstAppCtx,pstEnv, &stPkgRes.Body.BillQueryPropsDefRes);
			break;
		case BILL_QUERY_PROPSGIFT_RES:
			//shopping_query_propsgift_res(pstAppCtx,pstEnv, &stPkgRes.Body.BillQueryPropsGiftRes);
			break;
		case BILL_SET_PROPSGIFT_RES:
			//shopping_set_propsgift_res(pstAppCtx,pstEnv, &stPkgRes.Body.BillSetPropsGiftRes);
			break;	
		case ACC_GIFT_LIST_RES:
			player_accgift_res(pstEnv, &stPkgRes);
			break;
		case ACC_GIFT_INSERT_RES:
			player_accgift_insert_res(pstEnv, &stPkgRes);
			break;
		case PROP_GIFT_RES:
			player_propsgift_res(pstEnv, &stPkgRes);
			break;
		case PROP_GIFT_BILL_RES:
			player_propsgift_bill_res(pstEnv,  &stPkgRes);
		case SS_CDKEY_RES:
			player_ss_cdkey_res(pstEnv, &stPkgRes);
			break;
			
		case INFORMER_GET_ACCOUNT_RES:
			z_ss_get_account_res(pstEnv,&stPkgRes.Body.GetAccountRes);
			break;
			
		case SS_SMS_RES:
			z_ss_sms_res(pstEnv,&stPkgRes.Body.SmsRes);
			break;
			
		case SS_FILL_ID_CARD_RES:
			z_ss_fill_id_card_res(pstEnv,&stPkgRes.Body.FillIdCardRes);
			break;
		case SS_CMD_REQ:
			z_ss_cmd_req(pstEnv,&stPkgRes);
			break;
		case SS_CHAT:
			z_ss_chat(pstEnv, &stPkgRes.Body.ChatInfo);
			break;
		case SS_MSG_TRANS:
			 z_ss_msg_trans(pstEnv,&stPkgRes);
			break;
		default:		
			break;
	}
	
	return 0;
}

/*
//battle处理从服来的协议
int z_battle_msg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,int iMsgSrc, char *pszMsg, int iMsgLen)
{
	SSPKG stPkgRes;

	if(ss_unpack(pstEnv->pstSSHan, pszMsg, iMsgLen, &stPkgRes, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "aas_svr ss_unpack fail");
		return -1;
	}

	switch(stPkgRes.Head.Cmd)
	{
	case BATTLE_ENTER_REQ:
		player_battle_enter(pstAppCtx, pstEnv,
		                           &stPkgRes.Body.BattlePlayerEnterPworldReq);
		break;
	case BATTLE_ENTER_PWORLD:
		player_battle_enter_map(pstAppCtx, pstEnv,
		                        &stPkgRes.Body.BattleEnterPworldMapReq);
		break;
	case BATTLE_UPDATE_ROLEDATA_RES:
		player_battle_update_roledate_res(pstAppCtx, pstEnv,
		                                &stPkgRes.Body.BattleUpdateRoleDataRes);
		break;
	case BATTLE_MIRROR_SYNC:
		battle_mirror_sync_respond(pstAppCtx, pstEnv, iMsgSrc,
		                           &stPkgRes.Body.BattleMirrorSync);
		break;
	case ZONE_PROXY_ROUTINE:
		zone_proxy_respond(pstAppCtx, pstEnv, &stPkgRes.Body.BattleProxy);
		break;
	case TEAM_SHADOW_EXIT_REQ:
		team_shadow_exit(pstAppCtx, pstEnv,iMsgSrc, &stPkgRes.Body.TeamShadowExitReq);
		break;
	case SET_BATTLE_ROUTING_REQ:
		player_battle_set_routing(pstAppCtx, pstEnv,&stPkgRes.Body.SetBattleRoutinReq);
		break;
	case BATTLE_EXIT_REQ:
		player_battle_exit(pstAppCtx,pstEnv,stPkgRes.Body.BattleExitReq.Uin,0);
		break;
	case BATTLE_STATUSASSISTANT_REQ:
		player_do_statusassitant(pstEnv,&stPkgRes.Body.SSPlayerStatusassitantReq);
		break;
	case BATTLE_SHOPPING_REQ:
		shopping_battle_buy(pstAppCtx,pstEnv, &stPkgRes.Body.BattleBuyReq);
		break;
	case SS_CMD_REQ:
		z_ss_cmd_req(pstEnv, &stPkgRes);
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
	default:		
		break;
	} 
		
	return 0;
}

int z_zonesvr_msg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,int iMsgSrc, char *pszMsg, int iMsgLen)
{
	SSPKG stPkgRes;
	UNUSED(&iMsgSrc);
	
	if(ss_unpack(pstEnv->pstSSHan, pszMsg, iMsgLen, &stPkgRes, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_unpack fail");
		return -1;
	}

	switch(stPkgRes.Head.Cmd)
	{
	case BATTLE_ENTER_RES:
		player_battle_enter_res(pstAppCtx,pstEnv,&stPkgRes.Body.BattlePlayerEnterPworldRes);
		break;
	case BATTLE_UPDATE_ROLEDATA_REQ:
		player_battle_update_roledate(pstAppCtx, pstEnv,&stPkgRes.Body.BattleUpdateRoleDataReq );
		break;
	case BATTLE_PLAYER_HELLO_REQ:
		player_battle_hello2zone(pstAppCtx, pstEnv, &stPkgRes.Body.BattlePlayerHelloReq);
		break;
	case BATTLE_PROXY_ROUTINE:
		battle_proxy_respond(pstAppCtx, pstEnv, &stPkgRes.Body.BattleProxy);
		break;
	case TEAM_SHADOW_EXIT_RES:
		team_shadow_exit_res(pstAppCtx, pstEnv,&stPkgRes.Body.TeamShadowExitRes);
		break;
	case TEAM_SHADOW_SYNC_RES:
		team_shadow_sync_res(pstAppCtx, pstEnv,&stPkgRes.Body.TeamShadowSyncRes);
		break;
	case SET_BATTLE_ROUTING_RES:
		player_battle_set_routing_res(pstAppCtx,pstEnv,&stPkgRes.Body.SetBattleRoutinRes);
		break;
	case BATTLE_EXIT_RES:
		player_battle_exit_res(pstAppCtx,pstEnv,&stPkgRes.Body.BattleExitRes );
		break;
	case BATTLE_STATUSASSISTANT_RES:
		player_do_statusassitant(pstEnv,&stPkgRes.Body.SSPlayerStatusassitantRes);
		break;	
	case BATTLE_SHOPPING_RES:
		shopping_battle_buy_res(pstAppCtx,pstEnv,&stPkgRes.Body.BattleBuyRes);
		break;
	case BATTLE_TEAM_SUCC_REQ:
		team_create_succ(pstEnv,&stPkgRes.Body.BattleTeamSuccReq);
		break;
	case SS_CMD_REQ:
		z_ss_cmd_req(pstEnv, &stPkgRes);
		break;
	case BATTLE_TEAM_MEMBER_SYNC:
		player_pvp_svr_member_info_sync(pstEnv, &stPkgRes.Body.BattleMemberInfo);
		break;
	case BATTLE_MIRROR_SYNC:
		zone_mirror_sync_respond(pstAppCtx, pstEnv, iMsgSrc, &stPkgRes.Body.BattleMirrorSync);
		break;
	default:		
		break;
	}
		
	return 0;
}
*/
 
int z_proc(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int iLoopNum = 0;
	TBUSADDR iMsgDst ;
	TBUSADDR iMsgSrc ;
	int iMsgLen;
	int iRet = 0;
	int iBusy = 0;
	unsigned char ucEntity = 0;
	char *pszMsg;

	while (LOOP_NUM > iLoopNum)
	{
		iLoopNum++;
		
		iMsgDst = pstAppCtx->iId;
		iMsgSrc = TBUS_ADDR_ALL;	// 接收发到tbus的所有来源的数据
		iMsgLen = sizeof(pstEnv->szRecvBuffer);
		pszMsg = &pstEnv->szRecvBuffer[0];
		if ( TBUS_SUCCESS != tbus_recv(pstAppCtx->iBus, &iMsgSrc, &iMsgDst, 
			(void *)pszMsg, (size_t *)&iMsgLen, 0) )
		{
			//no msg
			break;
		}
		iBusy = 1;
		pstEnv->iLastSrc = iMsgSrc;

		GET_FUNC_ENTITY(ucEntity, iMsgSrc);
		//if (iMsgSrc == pstEnv->iZoneConnID)
		if (FUNC_ZONE_CONN == ucEntity)
		{
			TFRAMEHEAD stFrameHead;
			int iFrameHeadLen = sizeof(stFrameHead);
			
			iRet = tconnapi_decode(pszMsg, iMsgLen, &stFrameHead, &iFrameHeadLen);
			if (0 != iRet)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "tconnapi_decode failed, iRet0x%x, Msglen:%d", iRet, iMsgLen);
				continue;
			}

			

			pszMsg += iFrameHeadLen;
			iMsgLen -= iFrameHeadLen;

			/*Msg *pstMsg ;
			int iCmd = (int)ntohs(*(unsigned short*)(pszMsg));
			int iHLen = (int)ntohs(*(unsigned short*)(pszMsg+2));
			int iBLen = (int)ntohs(*(unsigned short*)(pszMsg+4));
			pszMsg += 6;
			iMsgLen -= 6;
			pstMsg = msg__unpack(NULL,iMsgLen,pszMsg);*/

			
			iRet = z_clt_msg(pstAppCtx, pstEnv, pszMsg, iMsgLen, &stFrameHead);
			pstEnv->pstRunStat->RecvCltMsg++;	
			pstEnv->pstRunStat->RecvCltBytes += iMsgLen;
		}
		else if (iMsgSrc == pstEnv->iRoleDBID || 
			       iMsgSrc == pstEnv->iMiscDBID ||
			       iMsgSrc == pstEnv->iAccountDBID ||
			       iMsgSrc == pstEnv->iRegionDBID)
		{
			iRet = z_db_msg(pstAppCtx, pstEnv, pszMsg, iMsgLen, iMsgSrc);
			pstEnv->pstRunStat->RecvDBMsg++;	
			pstEnv->pstRunStat->RecvDBBytes += iMsgLen;
		}
		else if (iMsgSrc == pstEnv->iZoneCtrlID)
		{
			iRet = z_ctrl_msg(pstAppCtx, pstEnv, pszMsg, iMsgLen);
		}
		else if(iMsgSrc == pstEnv->iCsSvrID)
		{
			iRet = z_cs_svr_msg(pstAppCtx, pstEnv, pszMsg, iMsgLen);
			pstEnv->pstRunStat->RecvCSMsg++;	
			pstEnv->pstRunStat->RecvCSBytes += iMsgLen;
		}
		else if (iMsgSrc == pstEnv->iRegionGateID)
		{
			iRet = z_region_msg(pstAppCtx, pstEnv, pszMsg, iMsgLen);
			pstEnv->pstRunStat->RecvRegionMsg++;	
			pstEnv->pstRunStat->RecvRegionBytes += iMsgLen;
		}
		else if (iMsgSrc == pstEnv->iMudSvrID)
		{
			iRet = z_mud_msg(pstAppCtx, pstEnv, pszMsg, iMsgLen);
			pstEnv->pstRunStat->RecvMudMsg++;	
			pstEnv->pstRunStat->RecvMudBytes += iMsgLen;
		}
		else
		{
			switch(ucEntity)
			{
				case FUNC_BATTLE_SVR:
				case FUNC_ZONE_SVR:
					iRet = ss_zone_msg(pstAppCtx, pstEnv, iMsgSrc, pszMsg, iMsgLen);
					pstEnv->pstRunStat->RecvBattleMsg++;	
					pstEnv->pstRunStat->RecvBattleBytes += iMsgLen;
					
					break;
				default:
					tlog_error(pstEnv->pstLogCat, 0, 0, "unspported msg source(%s)", tbus_addr_ntoa(iMsgSrc));
					break;
			}
	
			
		}
	}

	//不放在z_tick处理是为了能更均匀
	z_world_tick(pstAppCtx, pstEnv, iBusy);

	if (iBusy)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int z_report_dir(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iWeihuFlag)
{
	SSPKG *pstSSReq = &pstEnv->stSSReq;
	SSDIRREPORT *pstReport;
	TDRDATA stNet;
	TMEMBLOCK *pstMemBlock;
	int i,j;
	Player *pstPlayer;
	int iMap;
	int iLen;
	int iFind;
	int iCount = 0;
	MAPROLENUM stMapRoleNum[100];
	char szBuff[32];
	UNUSED(pstAppCtx);
	
	Z_SSHEAD_INIT(&pstSSReq->Head, DIR_REPORT);
	pstReport = &pstSSReq->Body.DirReport;

	memset(&stMapRoleNum, 0, sizeof(stMapRoleNum));
	memset(pstReport, 0, sizeof(*pstReport));
	pstReport->WorldID = pstEnv->unWorldID;
	pstReport->PlayerMaxNum = pstEnv->pstPlayerPool->iMax;
	pstReport->PlayerNum = pstEnv->pstPlayerPool->iUsed;
	//pstReport->IP = pstEnv->iConnIP;
	STRNCPY(pstReport->IP, pstEnv->pstConf->ConnIP, sizeof(pstReport->IP));
	pstReport->WeiHuFlag = iWeihuFlag;
	pstReport->Port = pstEnv->unPort;
	pstReport->RegionID = pstEnv->ucRegionID;
	pstReport->IP_Bak1 = (int)inet_addr(pstEnv->pstConf->ConnIP_bak1);
	pstReport->IP_Bak2 = (int)inet_addr(pstEnv->pstConf->ConnIP_bak2);

	for(i = 0; i < MAX_PLAYER; i++)
	{
		iFind = 0;
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (Player *)pstMemBlock->szData;
		if (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
		{
			continue;
		}

		if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
		{
			iMap = pstPlayer->stRoleData.Map;
		}
		else
		{
			iMap =  GET_MAPID(pstPlayer->stRoleData.Map);
		}

		for (j=0; j<(int)(sizeof(stMapRoleNum)/sizeof(stMapRoleNum[0])); j++)
		{
			if (stMapRoleNum[j].MapID == iMap)
			{
				iFind = 1;
				break;
			}	
		}

		if (iFind)
		{
			stMapRoleNum[j].RoleNum ++;
		}
		else
		{
			if (iCount >= (int)(sizeof(stMapRoleNum)/sizeof(stMapRoleNum[0])))
			{
				continue;
			}
			
			stMapRoleNum[iCount].MapID = iMap;
			stMapRoleNum[iCount].RoleNum = 1;
			iCount ++;
		}
		
	}

	for (i=0; i<iCount; i++)
	{
		memset(szBuff, 0, sizeof(szBuff));
		iLen = strlen(pstReport->MapInfo);
		if (iLen+32 >= MAX_MAP_INFO)
			break;
		snprintf(szBuff,sizeof(szBuff),"%d_%d,",stMapRoleNum[i].MapID, stMapRoleNum[i].RoleNum);
		strncpy(&pstReport->MapInfo[iLen],szBuff,strlen(szBuff));
	}

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
}

int z_tick_update(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i;
	Player *pstPlayer;

	for (i=0; i<pstEnv->iUpdateCount; i++)
	{
		if (pstEnv->iUpdatePos >= MAX_PLAYER)
		{
			pstEnv->iUpdatePos = 0;
		}
		pstPlayer = (Player *)tmempool_get_bypos(pstEnv->pstPlayerPool, pstEnv->iUpdatePos);
		pstEnv->iUpdatePos++;

		if (pstPlayer && (PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus || 
						 (pstPlayer->eStatus == PLAYER_STATUS_ROLE_NOTHING &&
		 					pstPlayer->stOnline.stSpanOnline.iStat == SPAN_STAT_CONNECT)))
		{
			z_db_role_update(pstAppCtx, pstEnv, pstPlayer,0);
		}
	}

	return 0;
}

int z_tick_bench(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i;
	Player *pstPlayer;
	int iState;
	CSPKG *pstPkg = &pstEnv->stCSRes;
	ADJUSTTIME *pstAdjustTime = &pstPkg->Body.AdjustTime;

	for (i=0; i<pstEnv->iBenchCount; i++)
	{
		if (pstEnv->iBenchPos >= MAX_PLAYER)
		{
			pstEnv->iBenchPos = 0;
		}
		pstPlayer = (Player *)tmempool_get_bypos(pstEnv->pstPlayerPool, pstEnv->iBenchPos);
		pstEnv->iBenchPos++;
		if (pstPlayer && PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus)
		{
			if (pstPlayer->stOnline.cCheckCltTimeFlag != 1)
			{
				continue;
			}
			
			iState = pstPlayer->stOnline.State;

			//角色在移动、战斗时不调整
			if (pstPlayer->stOnline.cMoving || pstPlayer->stOnline.tFightStatEnd > pstAppCtx->stCurr.tv_sec)
			{
				continue;
			}
			
			pstAdjustTime->BenchTimeMs = z_bench_time(pstPlayer);

			if (0 != pstAdjustTime->BenchTimeMs)
			{
				Z_CSHEAD_INIT(&pstPkg->Head, ADJUST_TIME);
				z_cltmsg_send(pstEnv, pstPlayer, pstPkg);
			}
		}
	}
	
	return 0;
}

int z_global_get(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;
	GLOBAL stGlobal;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	GlobalObjInfo *pstGlobalObjInfo = &pstEnv->pstGlobalObj->stGlobalObjInfo;

	if(pstEnv->iInstMode == INST_MODE_ZONE_SVR)
	{
		stDBAsyn.iCmd = DB_GLOBAL_SELECT;
		z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, "Global", -1, &stDBAsyn);
		pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstSelectReq->OperInfo.WhereDef[0] = 0;
		pstSelectReq->OffSet = 0;
		pstSelectReq->Limit = 0;

		memset(&stGlobal, 0, sizeof(stGlobal));

		stNetData.pszBuff = pstEnv->szSendBuffer;
		stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
		stMetaData.pszBuff = (char *)&stGlobal;
		stMetaData.iBuff = sizeof(stGlobal);
		if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iMiscDBProtoVer))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
			return -1;
		}
		
		if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
		{
			return -1;
		}

		pstGlobalObjInfo->iGlobalStat = GLOBAL_STAT_REQ;
		pstGlobalObjInfo->tReq = pstAppCtx->stCurr.tv_sec;
	}
	else
	{
		z_load_global_xml(pstEnv);
	}

	return 0;
}

int z_global_res(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes)
{
	GLOBAL stGlobal;
	ClanObj *pstClanObj = pstEnv->pstClanObj;
	
	if (1 == pstSelectRes->SelectResult.ResultNum)
	{
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stGlobal, sizeof(stGlobal)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,	"torm_fetch_one_meta fail");
			pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat = GLOBAL_STAT_INIT;
			return -1;
		}

		memset(pstEnv->pstGlobalObj, 0, sizeof(*pstEnv->pstGlobalObj));
		memcpy(&pstEnv->pstGlobalObj->stGlobal, &stGlobal, sizeof(pstEnv->pstGlobalObj->stGlobal));
	}
	else
	{
		memset(pstEnv->pstGlobalObj, 0, sizeof(*pstEnv->pstGlobalObj));
		pstEnv->pstGlobalObj->stGlobalObjInfo.iDirty = 1;
	}

	pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat = GLOBAL_STAT_FINI;
	shop_cgi_init(pstEnv);

	if (pstClanObj->iClanNum > 0 && pstClanObj->stClanObjInfo.iClanStat == CLAN_STAT_FINI)
	{
		if (pstClanObj->astZoneClan[pstClanObj->iClanNum-1].stClan.ClanProf.Id > pstEnv->pstGlobalObj->stGlobal.ClanLastIdx)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "max clan id %d, global last clan idx %d", pstClanObj->astZoneClan[pstClanObj->iClanNum-1].stClan.ClanProf.Id,
						pstEnv->pstGlobalObj->stGlobal.ClanLastIdx);

			pstEnv->pstGlobalObj->stGlobal.ClanLastIdx = pstClanObj->astZoneClan[pstClanObj->iClanNum-1].stClan.ClanProf.Id + 1;
		}
	}

	// 设置第一次启动服务器的时间
	if(!pstEnv->pstGlobalObj->stGlobal.Misc.NewWorldFlags.FirstStartupTime)
	{
		pstEnv->pstGlobalObj->stGlobal.Misc.NewWorldFlags.FirstStartupTime
			= pstEnv->pstAppCtx->stCurr.tv_sec;
		pstEnv->pstGlobalObj->stGlobalObjInfo.iDirty = 2;

		tlog_info(pstEnv->pstLogCat, 0, 0,
		          "new world first startup, set current time: %d",
		          pstEnv->pstAppCtx->stCurr.tv_sec);		
	}
	else
	{
		tlog_info(pstEnv->pstLogCat, 0, 0,
		          "world first startup time: %d",
		    pstEnv->pstGlobalObj->stGlobal.Misc.NewWorldFlags.FirstStartupTime);
	}

	if (pstEnv->pstGlobalObj->stGlobal.Misc.RangeSparteTm <= 0)
	{
		pstEnv->pstGlobalObj->stGlobal.Misc.RangeSparteTm =	pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	if (pstEnv->pstGlobalObj->stGlobal.Misc.WeekCharmTm <= 0)
	{
		pstEnv->pstGlobalObj->stGlobal.Misc.WeekCharmTm =	pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	if (pstEnv->pstGlobalObj->stGlobal.GoldRate.GoldRate == 0)
	{
		pstEnv->pstGlobalObj->stGlobal.GoldRate.GoldRate = DEFAULT_GOLD_RATE;
	}

	// 检测新服持续时间
	if(pstEnv->pstConf->NewWorldPersistedTime > 0)
	{
		if(pstEnv->pstGlobalObj->stGlobal.Misc.NewWorldFlags.FirstStartupTime +
		   pstEnv->pstConf->NewWorldPersistedTime >
		   pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			tlog_info(pstEnv->pstLogCat, 0, 0,
			          "this world is in new world stat, set zone_conn now.");

			if(z_set_limits_req(pstEnv, pstEnv->iZoneConnID, 1, 0) >= 0)
			{
				pstEnv->pstGlobalObj->stGlobalObjInfo.iConnLimitsEnabled = 1;
			}
		}
	}

	global_world_limit_update(pstEnv);
	global_world_ore_refresh(pstEnv);

	level_feng_update(pstEnv);
	span_map_camp_init(pstEnv);
	
	return 0;
}

int z_global_update_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, 
	TORMPKG *pstTormPkg )
{
	TORMUPDATERES *pstUpdateRes = &pstTormPkg->Body.UpdateRes;
	UNUSED( pstAppCtx );
	

	if (TORM_ERR_IS_ERROR(pstUpdateRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_global_update_res:DB update global failed." );
		return -1;
	}

	pstEnv->pstGlobalObj->stGlobalObjInfo.iDirty = 0;
	return 0;
}

int z_global_update(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	UNUSED(pstAppCtx);

	if(pstEnv->iInstMode == INST_MODE_ZONE_SVR)
	{
		z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, "Global", -1, NULL);
		pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
		pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstUpdateReq->OperInfo.WhereDef[0] = 0;
		pstUpdateReq->DoInsert = 1;

		stNetData.pszBuff = pstEnv->szSendBuffer;
		stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
		stMetaData.pszBuff = (char *)(&(pstEnv->pstGlobalObj->stGlobal));
		stMetaData.iBuff = sizeof(pstEnv->pstGlobalObj->stGlobal);
		if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iMiscDBProtoVer))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
			return -1;
		}
		
		if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
		{
			return -1;
		}	
	}
	else
	{
		z_global_to_xml(pstEnv);
	}

	pstEnv->pstGlobalObj->stGlobalObjInfo.iDirty = 0;

	return 0;
}

// 请求数据库更新Global数据
int z_global_update_req( TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv )
{
	
	DBAsyn stDBAsyn;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	UNUSED(pstAppCtx);

	if(pstEnv->iInstMode == INST_MODE_ZONE_SVR)
	{
		stDBAsyn.iCmd = DB_GLOBAL_UPDATE;
		z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, 
			"Global", -1, &stDBAsyn);
		pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstUpdateReq->OperInfo.WhereDef[0] = 0;
		pstUpdateReq->DoInsert = 1;

		stNetData.pszBuff = pstEnv->szSendBuffer;
		stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
		stMetaData.pszBuff = (char *)(&(pstEnv->pstGlobalObj->stGlobal));
		stMetaData.iBuff = sizeof(pstEnv->pstGlobalObj->stGlobal);
		if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iMiscDBProtoVer))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
			return -1;
		}
		
		if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
		{
			return -1;
		}
	}
	else
	{
		z_global_to_xml(pstEnv);
		pstEnv->pstGlobalObj->stGlobalObjInfo.iDirty = 0;
	}
	
	return 0;
}

int z_new_world_persisted_time(ZONESVRENV *pstEnv, int iPreTime, int iNowTime)
{
	// 时间为0, 尝试清除新服状态
	if(!iNowTime && pstEnv->pstGlobalObj->stGlobalObjInfo.iConnLimitsEnabled)
	{
		if(z_set_limits_req(pstEnv, pstEnv->iZoneConnID, 0, 0) >= 0)
		{
			pstEnv->pstGlobalObj->stGlobalObjInfo.iConnLimitsEnabled = 0;
			return 0;
		}
		return -1;
	}

	if(iNowTime > iPreTime)
	{
		// 时间延长了, 可能重新设置新服状态
		if(!pstEnv->pstGlobalObj->stGlobalObjInfo.iConnLimitsEnabled &&
		   (pstEnv->pstGlobalObj->stGlobal.Misc.NewWorldFlags.FirstStartupTime +
		    iNowTime > pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			tlog_info(pstEnv->pstLogCat, 0, 0,
			          "this world is in new world stat, set zone_conn now.");

			if(z_set_limits_req(pstEnv, pstEnv->iZoneConnID, 1, 0) >= 0)
			{
				pstEnv->pstGlobalObj->stGlobalObjInfo.iConnLimitsEnabled = 1;
			}
		}
	}
	else
	{
		// 时间缩短了, 可能清除新服状态
		if(pstEnv->pstGlobalObj->stGlobalObjInfo.iConnLimitsEnabled &&
		   (pstEnv->pstGlobalObj->stGlobal.Misc.NewWorldFlags.FirstStartupTime +
		    iNowTime <= pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			tlog_info(pstEnv->pstLogCat, 0, 0,
			          "this world is NOT in new world stat, unset zone_conn now.");

			if(z_set_limits_req(pstEnv, pstEnv->iZoneConnID, 0, 0) >= 0)
			{
				pstEnv->pstGlobalObj->stGlobalObjInfo.iConnLimitsEnabled = 0;
			}
		}
	}
	return 0;
}

int z_tick_global(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	static int iRetry = 0;
	static int iCount = 0;
	GlobalObjInfo *pstGlobalObjInfo = &pstEnv->pstGlobalObj->stGlobalObjInfo;

	iCount++;

	switch(pstGlobalObjInfo->iGlobalStat)
	{
		case GLOBAL_STAT_INIT:
			if (iRetry > 6)
			{
				pstGlobalObjInfo->iGlobalStat = GLOBAL_STAT_FAIL;
			}
			else
			{
				z_global_get(pstAppCtx, pstEnv);
			}
			break;
		case GLOBAL_STAT_REQ:
			if (pstAppCtx->stCurr.tv_sec - pstGlobalObjInfo->tReq > 5)
			{
				iRetry++;
				pstGlobalObjInfo->iGlobalStat = GLOBAL_STAT_INIT;
			}
			break;
		default:
			break;
	}

	if (GLOBAL_STAT_FINI != pstGlobalObjInfo->iGlobalStat)
	{
		return 0;
	}

	if(pstEnv->pstGlobalObj->stGlobalObjInfo.iConnLimitsEnabled &&
	   (pstEnv->pstGlobalObj->stGlobal.Misc.NewWorldFlags.FirstStartupTime +
	    pstEnv->pstConf->NewWorldPersistedTime <=
	    pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		tlog_info(pstEnv->pstLogCat, 0, 0,
		          "this world is NOT in new world stat, unset zone_conn now.");

		if(z_set_limits_req(pstEnv, pstEnv->iZoneConnID, 0, 0) >= 0)
		{
			pstEnv->pstGlobalObj->stGlobalObjInfo.iConnLimitsEnabled = 0;
		}
	}
	   

	if (2 == pstGlobalObjInfo->iDirty)
	{
		z_global_update(pstAppCtx, pstEnv);
	}
	else if (iCount%600 == 0 && 1 == pstGlobalObjInfo->iDirty) //定时
	{
		z_global_update(pstAppCtx, pstEnv);
	}

	if(pstGlobalObjInfo->iWorldCityStat == GLOBAL_WORLDCITY_INIT)
	{
		ClanObj *pstClanObj = pstEnv->pstClanObj;
		
		if(pstClanObj->stClanObjInfo.iClanStat == CLAN_STAT_FINI)
		{
			global_world_city_init(pstEnv);
			pstGlobalObjInfo->iWorldCityStat = GLOBAL_WORLDCITY_FINI;
		}
	}

	if (pstGlobalObjInfo->iStrongStat == 0)
	{
		pstGlobalObjInfo->iStrongStat = 1;
		strong_global_init(pstEnv);
	}

	if (pstGlobalObjInfo->iBossStat == 0)
	{
		boss_data_init(pstEnv);
		pstGlobalObjInfo->iBossStat = 1;
	}

	if (iCount%3 == 0)
	{
		strong_tick(pstEnv);
	}

	if (iCount%5 == 0)
	{
		 boss_fresh_tick( pstEnv);
		 weather_tick(pstEnv);
	}

	if (iCount%10 == 0)
	{
		player_reg_delete_by_time( pstEnv);
	}
	return 0;
}

//处理系统全局秒级事务
int z_tick(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	static time_t tLast = 0;

	if (pstEnv->cShutdown > 1)
	{
		return 0;
	}

	if (0 == tLast)
	{
		tLast = pstAppCtx->stCurr.tv_sec;
	}

	if (0 == IsSameDay(tLast, pstAppCtx->stCurr.tv_sec))
	{
		tLast = pstAppCtx->stCurr.tv_sec;
		
		z_check_id_index(pstAppCtx, pstEnv);
		z_check_pworld_inst(pstAppCtx, pstEnv);
		z_check_map_inst(pstAppCtx, pstEnv);
		z_check_area_dyn(pstAppCtx, pstEnv);
		tlog_info(pstEnv->pstLogCat, 0, 0, "mail pool %d, dyn msk pool %d", 
			pstEnv->pstMailPool->iUsed, pstEnv->pstDynMskPool->iUsed);

		pstEnv->iWarningFlag = 0;
	}
	
	if (pstEnv->tDirReport + DIR_REPORT_TIMER < pstAppCtx->stCurr.tv_sec)
	{
		pstEnv->tDirReport = pstAppCtx->stCurr.tv_sec;
		if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
		{
			z_report_dir(pstAppCtx, pstEnv,0);
		}
	}

	z_active_game_event(pstAppCtx, pstEnv, MAX_GAME_EVENT);

	//if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
	//{
		z_pworld_queue_tick(pstEnv);
	//}

	sparta_tick(pstEnv);
	
	z_tick_update(pstAppCtx, pstEnv);

	//调整客户端时间
	z_tick_bench(pstAppCtx, pstEnv);

	//全局产出限制周期更新
	world_limit_tick(pstEnv);

	if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
	{
		range_tick(pstEnv);
		z_shopdef_tick(pstAppCtx, pstEnv, 500);
		z_shopping_trade_tick(pstAppCtx, pstEnv, 180);
		shop_buy_tip(pstEnv);
	}

	z_active_bulletin(pstAppCtx, pstEnv);

	pstEnv->pstRunStat->Player = pstEnv->pstPlayerPool->iUsed;
	
	z_tick_global(pstAppCtx, pstEnv);
	span_global_tick( pstEnv);
	level_feng_update(pstEnv);
	
	if (RES_LOAD_DB == pstEnv->pstConf->ResloadType && pstAppCtx->stCurr.tv_sec-pstEnv->tCheckDB > 30)
	{
		tdr_keep_dbmsconnection(pstEnv->hResDBHandle);
		pstEnv->tCheckDB = pstAppCtx->stCurr.tv_sec;
	}

	if ( !IsSameDay(pstEnv->iLastWorldTick,pstAppCtx->stCurr.tv_sec))
	{
		dyn_active_status_init(pstEnv);
		dyn_active_status_update_all(pstEnv);

		clan_touch_check_all(pstEnv);

		
		clan_init_touch_status(pstEnv);
		all_player_yesterday_info(pstEnv);
	}

	pstEnv->iLastWorldTick = pstAppCtx->stCurr.tv_sec;


	//if (pstEnv->iOpRoleNumTm + 60 < pstAppCtx->stCurr.tv_sec)
	//{
	//	pstEnv->iOpRoleNumTm = pstAppCtx->stCurr.tv_sec;
	//	z_map_role_num_log(pstEnv);
	//}

	//整点刷跑商价格和数量
	if (pstEnv->pstZoneObj->stPaoShangInfo.PaoShangRefTime/3600 != pstEnv->pstAppCtx->stCurr.tv_sec/3600)
	{
		player_paoshang_init(pstEnv);
	}
	
	player_dianfeng_tick(pstEnv);

	z_wedding_tick(pstEnv);
    zj_rongyan_tick(pstEnv);
	new_people_tick(pstEnv);
	shikong_tick(pstEnv);
	
	return 0;
}

int z_reload_self()
{
	traise(SIGUSR2);
	
	return 0;
}

/*不要直接调用此函数，pstPreConf是tapp进行初始化的
    使用SIGUSR2 信号，调用z_reload_self 函数
*/
int z_reload(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i;
	ZONE_SVRCONF *pstPreConf = (ZONE_SVRCONF   *)pstAppCtx->stConfPrepareData.pszBuff;
	ZONE_SVRCONF *pstNowConf = pstEnv->pstConf;

	if(pstNowConf->NewWorldPersistedTime != pstPreConf->NewWorldPersistedTime)
	{
		z_new_world_persisted_time(pstEnv, pstNowConf->NewWorldPersistedTime,
		                           pstPreConf->NewWorldPersistedTime);

		pstNowConf->NewWorldPersistedTime = pstPreConf->NewWorldPersistedTime;
	}

	pstNowConf->NoCreateRole = pstPreConf->NoCreateRole;
	pstNowConf->PingCheck = pstPreConf->PingCheck;
	
	pstNowConf->CmdLimitNum = pstPreConf->CmdLimitNum;
	for (i=0; i<pstPreConf->CmdLimitNum; i++)
	{
		pstNowConf->CmdLimitIds[i] = pstPreConf->CmdLimitIds[i];
	}
	z_init_cmd_map(pstEnv);

	if (pstNowConf->UpdateInterval != pstPreConf->UpdateInterval)
	{
		pstNowConf->UpdateInterval = pstPreConf->UpdateInterval;
		pstEnv->iUpdateCount = MAX_PLAYER/(pstEnv->pstConf->UpdateInterval*1000.0/pstAppCtx->iTimer);
		printf("update count=%d\n", pstEnv->iUpdateCount);
		tlog_info(pstEnv->pstLogCat, 0, 0, "update count=%d", pstEnv->iUpdateCount);
	}

	InitDirtySystem("../cfg/dirty/dirtyword.txt");

	//经验倍率
	pstNowConf->ExpMul = pstPreConf->ExpMul;
	pstNowConf->PreCreateRole = pstPreConf->PreCreateRole;
	pstNowConf->LoginInterval = pstPreConf->LoginInterval;
	pstNowConf->MaxLevel = pstPreConf->MaxLevel;
	STRNCPY(pstNowConf->MinCltVer, pstPreConf->MinCltVer, sizeof(pstNowConf->MinCltVer));
	STRNCPY(pstNowConf->MaxCltVer, pstPreConf->MaxCltVer, sizeof(pstNowConf->MaxCltVer));

	STRNCPY(pstNowConf->BlackWhiteCfgPath, pstPreConf->BlackWhiteCfgPath, sizeof(pstNowConf->BlackWhiteCfgPath));

	if (pstEnv->pstBlackWhiteH)
	{
		black_white_fini(&pstEnv->pstBlackWhiteH);
	}

	pstEnv->pstBlackWhiteH = NULL;
	if (pstNowConf->BlackWhiteCfgPath[0] && 0 > black_white_init(pstNowConf->BlackWhiteCfgPath, &pstEnv->pstBlackWhiteH))
	{
		printf("init black-white file:%s fail\n", pstNowConf->BlackWhiteCfgPath);;		
		tlog_info(pstEnv->pstLogCat, 0, 0, "init black-white file:%s fail", pstNowConf->BlackWhiteCfgPath);
	}

	// 先把要回写的都处理下
	z_shopdef_tick(pstAppCtx, pstEnv, MAX_SHOP_DEF_LINES);

	load_script_dir(pstEnv);

	z_load_res(pstAppCtx, pstEnv);

	init_item_scr(pstEnv);

	//任务脚本
	init_task_scr(pstEnv);

	z_init_player_scr(pstEnv);

	redef_all_mon(pstEnv);

	//装载GMIPMacLimit
	memset(&pstEnv->stGmLimt, 0, sizeof(pstEnv->stGmLimt));
	z_gm_limit_get(pstAppCtx, pstEnv);
	player_acc_giftinfo_req_broadcast(pstEnv);

	//只在维护更新时或reload zone_svr时拉取特权网吧表
	z_load_vip_bar_data(pstEnv);
	
	tlog_info(pstEnv->pstLogCat, 0, 0, "z_reload");
	
	return 0;
}



int z_kickoff_all(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i;
	Player *pstPlayer;
	int iCount = 0;
	
	for (i=0; i<MAX_PLAYER; i++)
	{
		pstPlayer = (Player *)tmempool_get_bypos(pstEnv->pstPlayerPool, i);

		if (pstPlayer)
		{
			/*
			if ()
			{
				if ( ( pstEnv->cShutdown >2) && 
				 ((pstAppCtx->stCurr.tv_sec-pstPlayer->tActive) > STOP_INTERVAL_CHECK*2) )
				{
					z_free_player(pstAppCtx, pstEnv, pstPlayer);
				}
			}
			else*/
			{
				if(INST_MODE_ZONE_SVR == pstEnv->iInstMode)
				{
					player_kickoff(pstAppCtx, pstEnv, pstPlayer, ZONEERR_KICK214);
				}
				else
				{
					player_kickoff(pstAppCtx, pstEnv, pstPlayer, -1);
				}
				iCount++;

				//一次少搞一些，避免bus 通道拥塞和db瓶颈
				if (iCount > STOP_PATCH) 
				{
					usleep(STOP_PATCH*2*1000); // 2ms存一个player
					iCount = 0;
					z_proc(pstAppCtx, pstEnv);
				}
			}
		}
	}

	return 0;
}

int sec2str(char *pszBuff, int iLen, int iTimeSec)
{
	int iMinute, iSec;

	pszBuff[0] = 0;
	iMinute = iTimeSec/60;
	//iSec = iTimeSec%60;
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
			snprintf(pszBuff, iLen, "%d%s", iMinute, LJY_MINUTE);
		}
	}
	else
	{
		snprintf(pszBuff, iLen, "%d%s", iSec, LJY_SECOND);
	}
	
	return 0;
}

int z_stop(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i;
	Player *pstPlayer;
	CSSYSMSG *pstMsg;
	CSPKG *pstCSRes = &pstEnv->stCSRes;

	switch( pstEnv->cShutdown )
	{
	case 0:
		pstEnv->cShutdown = 2;
		return 0;
		break;
	case 1: // 阶段1
		{
			if (0 == pstEnv->ucShutdownCount)
			{
				pstEnv->cShutdown++; // 通知完成，转入阶段2
				return 0;
			}
			
			if (pstAppCtx->stCurr.tv_sec - pstEnv->tShutdownStart >= SHUTDOWN_NOTIFY_TIMER)
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

					sec2str(szBuff, sizeof(szBuff), pstEnv->ucShutdownCount*SHUTDOWN_NOTIFY_TIMER);
					
					snprintf(pstMsg->Msg, sizeof(pstMsg->Msg), LJY_SYSMSG20, szBuff);
					pstMsg->Msg[sizeof(pstMsg->Msg) - 1] = 0;
					
					z_cltmsg_broadcast(pstAppCtx, pstEnv, pstCSRes);
				}
			}
		}
		return 0;
		break;
	case 2: // 阶段2 尝试踢出所有玩家
		{
			z_report_dir(pstAppCtx, pstEnv,1);
		
			tlog_info(pstEnv->pstLogCat, 0, 0, "kick off all player begin");
			z_kickoff_all(pstAppCtx, pstEnv);
			pstEnv->cShutdown++;
			pstEnv->tShutdownStart = pstAppCtx->stCurr.tv_sec;
		}
		return 0;
		break;

	case 3: // 阶段3 尝试再踢
		{
			if (pstAppCtx->stCurr.tv_sec - pstEnv->tShutdownStart > STOP_INTERVAL_CHECK)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "kick off all player %d left", pstEnv->pstPlayerPool->iUsed);
				pstEnv->tShutdownStart = pstAppCtx->stCurr.tv_sec;
				z_kickoff_all(pstAppCtx, pstEnv);
			}
			
			for (i=0; i<MAX_PLAYER; i++)
			{
				pstPlayer = (Player *)tmempool_get_bypos(pstEnv->pstPlayerPool, i);
				if (pstPlayer)
				{
					return 0;
				}
			}
			tlog_info(pstEnv->pstLogCat, 0, 0, "kick off all player end");
			pstEnv->cShutdown++;
		}
		return 0;
		break;

	case 4: // 阶段4 尝试处理所有CLAN事务
		{
			tlog_info(pstEnv->pstLogCat, 0, 0, "store all clan start");
			if(INST_MODE_ZONE_SVR == pstEnv->iInstMode )
			{
				z_db_clan_fin( pstAppCtx, pstEnv );
			}
			pstEnv->cShutdown++;
			pstEnv->tShutdownStart = pstAppCtx->stCurr.tv_sec;
		}
		return 0;
		break;
		
	case 5: // 阶段5 尝试再处理所有CLAN事务
		{
			if(INST_MODE_ZONE_SVR == pstEnv->iInstMode )
			{
				if (pstAppCtx->stCurr.tv_sec - pstEnv->tShutdownStart > STOP_INTERVAL_CHECK)
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "clan not clean");
					pstEnv->tShutdownStart = pstAppCtx->stCurr.tv_sec;
					z_db_clan_fin( pstAppCtx, pstEnv );
				}

				if( !z_db_clan_is_allclean( pstAppCtx, pstEnv ) )
				{
					return 0;
				}
				tlog_info(pstEnv->pstLogCat, 0, 0, "store all clan end");
			}
			pstEnv->cShutdown++;
		}
		return 0;
		break;

	case 6: // 阶段6 尝试保存Global
		{
			//if(INST_MODE_ZONE_SVR == pstEnv->iInstMode )
			{
				tlog_info(pstEnv->pstLogCat, 0, 0, "store global start, GlobalStat=%d", pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat);

				if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat == GLOBAL_STAT_FINI)
				{
					// 强制更新数据库
					pstEnv->pstGlobalObj->stGlobalObjInfo.iDirty = 1;
					z_global_update_req( pstAppCtx, pstEnv );
				}
			}
			
			pstEnv->cShutdown++;
			pstEnv->tShutdownStart = pstAppCtx->stCurr.tv_sec;
		}
		return 0;
		break;

	case 7: // 阶段7 尝试再保存Global
		{
			//if(INST_MODE_ZONE_SVR == pstEnv->iInstMode )
			if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat == GLOBAL_STAT_FINI)
			{
				if (pstAppCtx->stCurr.tv_sec - pstEnv->tShutdownStart > STOP_INTERVAL_CHECK)
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "global not clean");
					pstEnv->tShutdownStart = pstAppCtx->stCurr.tv_sec;
					z_global_update_req( pstAppCtx, pstEnv );
				}

				if( pstEnv->pstGlobalObj->stGlobalObjInfo.iDirty != 0 )
				{
					return 0;
				}
			}

			tlog_info(pstEnv->pstLogCat, 0, 0, "store global end");

			pstEnv->cShutdown++;
			pstEnv->tShutdownStart = pstAppCtx->stCurr.tv_sec;
		}
		return 0;
		break;

      case 8:// 阶段8把商城的数据都回写下
      		{
			z_shopdef_tick(pstAppCtx, pstEnv, MAX_SHOP_DEF_LINES);
			pstEnv->cShutdown++;
      		}
	       return 0;
      		break;
	case 9: //排行榜回写
		if (pstEnv->pstZoneObj->stRange.iState == RANGE_STAT_DB_FINI)
		{
			if(INST_MODE_ZONE_SVR == pstEnv->iInstMode )
			{
				range_do_chg(pstEnv);
			}
			pstEnv->cShutdown++;
		}
		else
		{
			pstEnv->cShutdown++;
		}
		return 0;
		break;
	case 10: //排行榜回写完成
		if (pstEnv->pstZoneObj->stRange.iState < RANGE_STAT_DB_FINI)
		{
			pstEnv->cShutdown++;
		}
		else if (pstEnv->pstZoneObj->stRange.iState == RANGE_STAT_DB_FINI)
		{
			pstEnv->cShutdown++;
		}
		else
		{
			if(INST_MODE_ZONE_SVR == pstEnv->iInstMode )
			{
				range_tick(pstEnv);
			}
		}
		return 0;
		break;
	case 11: // 阶段11 尝试保存所有邮件
		{
			if(INST_MODE_ZONE_SVR == pstEnv->iInstMode )
			{
				tlog_info(pstEnv->pstLogCat, 0, 0, "store all mail start");
				z_db_mail_fin( pstAppCtx, pstEnv );
			}
			pstEnv->cShutdown++;
			pstEnv->tShutdownStart = pstAppCtx->stCurr.tv_sec;
		}
		return 0;
		break;

	case 12: // 阶段12 尝试再保存所有邮件
		{
			if(INST_MODE_ZONE_SVR == pstEnv->iInstMode )
			{
				if (pstAppCtx->stCurr.tv_sec - pstEnv->tShutdownStart > STOP_INTERVAL_CHECK)
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "mail not clean DeleteNum=%d, DeleteList=%d", 
						pstEnv->pstPostoffice->stInfo.iDeleteNum,
						pstEnv->pstPostoffice->stInfo.iDeleteList);
					pstEnv->tShutdownStart = pstAppCtx->stCurr.tv_sec;
					z_db_mail_fin( pstAppCtx, pstEnv );
				}

				z_active_mail(pstAppCtx, pstEnv, 10);
				if( !z_db_mail_is_allclean( pstAppCtx, pstEnv ) )
				{
					return 0;
				}
				tlog_info(pstEnv->pstLogCat, 0, 0, "store all mail end");
			}
			pstEnv->cShutdown++;
		}
		return 0;
		break;	
	case 13:
		if(INST_MODE_ZONE_SVR == pstEnv->iInstMode )
		{
			z_db_store_finish( pstEnv);
		}
		pstEnv->cShutdown++;
		pstEnv->tShutdownStart = pstAppCtx->stCurr.tv_sec;
		break;
	case 14:
		if(INST_MODE_ZONE_SVR == pstEnv->iInstMode )
		{
			if (pstAppCtx->stCurr.tv_sec - pstEnv->tShutdownStart > STOP_INTERVAL_CHECK)
			{
				z_db_store_finish( pstEnv);
				pstEnv->tShutdownStart = pstAppCtx->stCurr.tv_sec;
			}

			z_active_store(pstAppCtx, pstEnv, 10);
			if( !z_db_store_is_allclean(pstEnv))
			{
				return 0;
			}
			tlog_info(pstEnv->pstLogCat, 0, 0, "store all end");
		}
		pstEnv->cShutdown++;
		break;
	case 15:
		if(INST_MODE_ZONE_SVR == pstEnv->iInstMode )
		{
			z_db_home_finish( pstEnv);
		}
		pstEnv->cShutdown++;
		pstEnv->tShutdownStart = pstAppCtx->stCurr.tv_sec;
		break;

	default:
		return -1;
		break;
	}

	return 0;
}

int z_fini(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int iRet;
	
	tlog_info(pstEnv->pstLogCat, 0, 0, "zone svr fini");
	
	z_check_uincache(pstEnv);
	z_check_id_index(pstAppCtx, pstEnv);
	z_check_map(pstAppCtx, pstEnv);

	z_game_event_to_xml(pstEnv);

	err_release();

	if (pstEnv->pstBlackWhiteH)
	{
		black_white_fini(&pstEnv->pstBlackWhiteH);
	}

	if (pstEnv->pstShtRangeName)
	{
		sht_destroy(&pstEnv->pstShtRangeName);
	}
	
	sht_destroy(&pstEnv->pstShtUinCache);
	tconnapi_free(&pstEnv->iTconnapiHandle);
	net_han_destroy(&pstEnv->pstNetHan);
	ss_han_destroy(&pstEnv->pstSSHan);

	z_oplog_fini(pstAppCtx, pstEnv);

	if (pstEnv->cClearShm)
	{
		//删除player shm
		iRet = tshmdt(pstEnv->pstPlayerShm);
		iRet = tshmclose(pstEnv->iPlayerShmID, TSHMF_DELETE);

		//删除uin cache shm
		iRet = tshmdt(pstEnv->pstShtUinCache);
		iRet = tshmclose(pstEnv->iUinCacheShmID, TSHMF_DELETE);

		//删除ZoneObj shm
		iRet = tshmdt(pstEnv->pstZoneObj);
		iRet = tshmclose(pstEnv->iZoneObjShmID, TSHMF_DELETE);

		//删除MapObj shm
		iRet = tshmdt(pstEnv->pstMapObj);
		iRet = tshmclose(pstEnv->iMapObjShmID, TSHMF_DELETE);
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

	gs_stAppCtx.iIdleCount = 5;
	gs_stAppCtx.iIdleSleep = 5; 
	gs_stAppCtx.iTimer = 1000;

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)z_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_zone_svr;
	gs_stAppCtx.stConfData.pszMetaName = "zone_svrconf";
	
       gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);

	gettimeofday(&gs_stAppCtx.stCurr, NULL);
		
	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)z_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)z_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)z_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)z_reload;
	gs_stAppCtx.pfnStop  =  (PFNTAPPFUNC)z_stop;

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

int z_init_player_scr(ZONESVRENV* pstEnv)
{
	ScriptIndex *pstScriptInd = NULL;
	pstEnv->ucPlayerScriptFlag = 0;
	
	pstScriptInd = get_script_ind(PLAYER_EVENT_SCRIPT_NAME);
	if (NULL == pstScriptInd)
	{
		return 0;
	}

	if (0 == check_script_proc(PLAYER_EVENT_SCRIPT_NAME, pstScriptInd, PLAYER_EVENT_SCRIPT_PROC_LEVEL))
	{
		pstEnv->ucPlayerScriptFlag |= PLAYER_EVENT_SCRIPT_PROC_LEVEL_MAP;
	}
	return 0;	
}

int is_battle_sparta_svr(ZONESVRENV* pstEnv)
{
	unsigned char ucInstance = 0;
	GET_INSTANCE_ENTITY(ucInstance,pstEnv->pstAppCtx->iId);
	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode && 
		ucInstance == BATTLE_SPARTA_INSTANCE)
	{
		return 1;
	}
	return 0;
}
int is_battle_pve_battle_svr(ZONESVRENV* pstEnv)
{
	unsigned char ucInstance = 0;
	GET_INSTANCE_ENTITY(ucInstance,pstEnv->pstAppCtx->iId);
	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode && 
		ucInstance == BATTLE_PVE_BATTLE_INSTANCE)
	{
		return 1;
	}
	return 0;
}
int is_battle_pvp_svr(ZONESVRENV* pstEnv)
{
	unsigned char ucInstance = 0;
	GET_INSTANCE_ENTITY(ucInstance,pstEnv->pstAppCtx->iId);
	if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode && 
	   ucInstance == BATTLE_PVP_INSTANCE)
	{
		return 1;
	}
	return 0;
}

int  hash_insert_dyn_active_status(ZONESVRENV *pstEnv,unsigned int uiKey,char* pszData,unsigned int iSize)
{
	int iRet = 0;
	DynActiveStatusBucket *pstPos, stDummy;
	DYNACTIVESTATUS *pstDynActiveStatusOld = NULL;

	if( iSize > sizeof(DynActiveStatusBucket))
	{
		return -1;
	}

	pstDynActiveStatusOld = z_find_dyn_active_status(pstEnv,uiKey);
	if( NULL!= pstDynActiveStatusOld )
	{
		if( memcmp(pszData,pstDynActiveStatusOld,sizeof(DYNACTIVESTATUS)) != 0   ) 
		{
			/* 修改的数据 */
			memcpy( pstDynActiveStatusOld, pszData,sizeof(DYNACTIVESTATUS));
			iRet = 1;
		}
	}
	else
	{
		iRet = 1;
	}
	
	// 不管有没有 都做插入操作
	stDummy.uiKey = uiKey;
	pstPos = (DynActiveStatusBucket *)sht_insert_unique(pstEnv->pstDynActiveStatusHash, &stDummy, hash_cmp_int,
		                        hash_code_int);
	if(pstPos)
	{
		pstPos->uiKey = uiKey;
		memcpy( pstPos->szData, pszData,iSize);
	}
	
	return iRet;
}

DYNACTIVESTATUS * z_find_dyn_active_status(ZONESVRENV *pstEnv,unsigned int uiKey)
{
	DynActiveStatusBucket *pstPos, stDummy;

	stDummy.uiKey = uiKey;
	pstPos = (DynActiveStatusBucket *)sht_find(pstEnv->pstDynActiveStatusHash, &stDummy, hash_cmp_int, hash_code_int);

	if( pstPos != NULL )
	{
		return (DYNACTIVESTATUS *)pstPos->szData;
	}
	
	return  NULL;
}

int dyn_active_status_init(ZONESVRENV *pstEnv)
{

	int i = 0;
	for (i=0; i<pstEnv->pstZoneObj->iActiveHelp; i++)
	{
		ACTIVEHELPER * pstActiveHelper = (ACTIVEHELPER *)pstEnv->pstZoneObj->sActiveHelp[i];
		if (ACTIVE_HELPER_CLAN == pstActiveHelper->Type &&
			WORLD_CITY_ACTIVE_HELP_ID == pstActiveHelper->ActiveID)
		{
			DYNACTIVESTATUS stActive;

			memset(&stActive,0,sizeof(stActive));

			stActive.ActiveHelperID = pstActiveHelper->ActiveID;
			
			hash_insert_dyn_active_status(pstEnv,stActive.ActiveHelperID,(char*)&stActive,sizeof(stActive));
		}

	}
	
	return 0;
}

int dyn_active_status_update(ZONESVRENV *pstEnv,Player* pstPlayer)
{
	int i = 0;
	int iValid = 0;
	ZoneClan* pstZoneClan = NULL;
	
	for(i = 0; i < pstEnv->pstDynActiveStatusHash->iMax;i++)
	{
		DynActiveStatusBucket *pstPos;


		pstPos = (DynActiveStatusBucket *)sht_pos(pstEnv->pstDynActiveStatusHash, i, &iValid);

		if( iValid && pstPos)
		{
			DYNACTIVESTATUS* pstStatus = (DYNACTIVESTATUS *)pstPos->szData;

			player_dyn_active_helper_info(pstEnv,pstPlayer,pstStatus->ActiveHelperID,pstStatus->Status);
			
		}
	}

	pstZoneClan = player_get_clan(pstEnv,pstPlayer);
	if (pstZoneClan)
	{
		player_dyn_active_helper_info(pstEnv,pstPlayer,CLAN_TOUCH_ACTIVE_HELP_ID,pstZoneClan->stZoneClanInfo.iClanTouchStatus);
	}
	else
	{
		player_dyn_active_helper_info(pstEnv,pstPlayer,CLAN_TOUCH_ACTIVE_HELP_ID,ACTIVE_STATUS_INIT);
	}
	return 0;
}

int dyn_active_status_update_all(ZONESVRENV *pstEnv)
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
			dyn_active_status_update(pstEnv,pstPlayer);
		}
		
	}
	return 0;
}

int dyn_active_status_ch(ZONESVRENV *pstEnv,int iID,int iStatus)
{
	DYNACTIVESTATUS	*pstActive = z_find_dyn_active_status(pstEnv, iID);
	if (!pstActive)
	{
		return -1;
	}
	
	if (pstActive->Status != iStatus)
	{
		pstActive->Status = iStatus;
		all_player_dny_active_helper_info(pstEnv,pstActive->ActiveHelperID,iStatus);
	}

	return 0;
}

int  hash_insert_cond_id_map(ZONESVRENV *pstEnv,unsigned int uiKey,char* pszData,unsigned int iSize)
{
	int iRet = 0;
	CondIdMapBucket *pstPos, stDummy;
	CONDIDMAP *pstCondIdMapOld = NULL;

	if( iSize > sizeof(CondIdMapBucket))
	{
		return -1;
	}

	pstCondIdMapOld = z_find_cond_id_map(pstEnv,uiKey);
	if( NULL!= pstCondIdMapOld )
	{
		if( memcmp(pszData,pstCondIdMapOld,sizeof(CONDIDMAP)) != 0   ) 
		{
			/* 修改的数据 */
			memcpy( pstCondIdMapOld, pszData,sizeof(CONDIDMAP));
			iRet = 1;
		}
	}
	else
	{
		iRet = 1;
	}
	
	// 不管有没有 都做插入操作
	stDummy.uiKey = uiKey;
	pstPos = (CondIdMapBucket *)sht_insert_unique(pstEnv->pstCondIdMapHash, &stDummy, hash_cmp_int,
		                        hash_code_int);
	if(pstPos)
	{
		pstPos->uiKey = uiKey;
		memcpy( pstPos->szData, pszData,iSize);
	}
	
	return iRet;
}

CONDIDMAP * z_find_cond_id_map(ZONESVRENV *pstEnv,unsigned int uiKey)
{
	CondIdMapBucket *pstPos, stDummy;

	stDummy.uiKey = uiKey;
	pstPos = (CondIdMapBucket *)sht_find(pstEnv->pstCondIdMapHash, &stDummy, hash_cmp_int, hash_code_int);

	if( pstPos != NULL )
	{
		return (CONDIDMAP *)pstPos->szData;
	}
	
	return  NULL;
}

int z_wedding_tick(ZONESVRENV* pstEnv)
{
	if(is_fini_global(pstEnv) == 0)
	{
		return 0;
	}
	
	WEDDINGGLOBAL *pstWeddingGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.WeddingGlobal;
	CELEBRATIONINFOS *pstCelebrationInfos = NULL;
	TRAVELINFOS *pstTravelInfos = NULL;

	if(pstWeddingGlobal->CelebrationNum > 0)
	{
		//只有一对而已
		pstCelebrationInfos = &pstWeddingGlobal->CelebrationInfos[pstWeddingGlobal->CelebrationNum-1];
		if(pstCelebrationInfos->CelebrationEndTime != 0  && 
			pstCelebrationInfos->CelebrationEndTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			z_sys_strf_broadcast(pstEnv, SYS_SVR, ZONEERR_WEDDING45,pstCelebrationInfos->FirstCRoleName,pstCelebrationInfos->SecondCRoleName);
			pstWeddingGlobal->CelebrationNum = 0;
		}
	}

	if(pstWeddingGlobal->TravelNum > 0)
	{
		//只有一对而已
		pstTravelInfos = &pstWeddingGlobal->TravelInfos[pstWeddingGlobal->TravelNum-1];
		if((pstTravelInfos->TravelEndFlag == 1) ||
			(pstTravelInfos->EndTime != 0 && pstTravelInfos->EndTime <= pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			z_sys_strf_broadcast(pstEnv, SYS_SVR, ZONEERR_WEDDING46,pstTravelInfos->FirstTRoleName,pstTravelInfos->SecondTRoleName);
			pstWeddingGlobal->TravelNum = 0;
		}
	}
	
	return 0;
}

