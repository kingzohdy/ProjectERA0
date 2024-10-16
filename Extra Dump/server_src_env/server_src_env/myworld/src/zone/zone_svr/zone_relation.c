/*
**  @file $RCSfile: zone_relation.c,v $
**  general description of this module
**  $Id: zone_relation.c,v 1.183 2014/05/08 14:23:34 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/05/08 14:23:34 $
**  @version $Revision: 1.183 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_relation.h"
#include "zone_clt.h"
#include "zone_db.h"
#include "libdirty.h"
#include "zone_err.h"
#include "zone_clan.h"
#include "zone_mail.h"
#include "zone_spansvr.h"
#include "zone_player.h"
#include "zone_team.h"
#include "zone_gm.h"
#include "zone_designation.h"
#include "zone_span.h"
#include "zone_err.h"
#include "zone_task.h"
#include "zone_zh_name.h"
#include "zone_itemlimit.h"
#include "zone_skill.h"
#include "zone_package.h"
#include "zone_script.h"
#include "zone_move.h"
#include "zone_machine.h"
#include "zone_span.h"

int g_SpousePos = 0;


//#define CESHI_ 
#define EMOTION_ALTER_INTERVAL 3600
int relation_db_get(ZONESVRENV* pstEnv, tdr_ulonglong ullRoleID, int iUin, DBAsyn *pstDBAsyn);
int relation_self_emotion_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer);
int relation_claninfo_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstOther);

int player_black_find_byname(ROLEMISCINFO *pstRoleMisc, char *pszName)
{
	int i;

	for (i=0; i<pstRoleMisc->BlackNum; i++)
	{
		if (0 == strcmp(pstRoleMisc->BlackMember[i].RoleName, pszName))
		{
			return i;
		}
	}

	return -1;
}

int player_black_find_byid(ROLEMISCINFO *pstRoleMisc, tdr_ulonglong ullID)
{
	int i;

	for (i=0; i<pstRoleMisc->BlackNum; i++)
	{
		if (pstRoleMisc->BlackMember[i].RoleID == ullID)
		{
			return i;
		}
	}

	return -1;
}

int player_enemy_find_byname( const ROLEMISCINFO * pstMiscInfo, 
	const char * pszName )
{
	int i;

	for( i = 0; i < pstMiscInfo->Enemy.EnemyNum; i++ )
	{
		if( strcmp( pstMiscInfo->Enemy.Enemys[i].RoleName, pszName ) == 0 )
		{
			return i;
		}
	}
	
	return -1;
}

int online_info_find_friend_byname(ROLEBUDDY *pstRoleBuddy, char* pszName)
{
	int i;

	for (i=0; i<pstRoleBuddy->FriendNum; i++)
	{
		if (strcmp(pstRoleBuddy->Friends[i].RoleName, pszName) == 0)
		{
			return i;
		}
	}
	
	return -1;
}

int online_info_find_friend_byID(ROLEBUDDY *pstRoleBuddy, tdr_ulonglong ullID)
{
	int i;

	for (i=0; i<pstRoleBuddy->FriendNum; i++)
	{
		if (pstRoleBuddy->Friends[i].RoleID == ullID)
		{
			return i;
		}
	}
	
	return -1;
}



// 自动配对
int player_auto_mate(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	int iLevelDiff;
	TMEMBLOCK *pstMemBlock;
	Player *pstPlayerOnline;
	BUDDYINFO *pstBuddyInfo;
	Player *pstPlayerTmp = NULL;

	if(pstEnv->iInstMode != INST_MODE_ZONE_SVR)
	{
		return 0; // 非zone_svr不自动配对
	}

	for (i=0; i<MAX_PLAYER; i++)
	{
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayerOnline = (Player *)pstMemBlock->szData;
		if (PLAYER_STATUS_ROLE_LOGIN != pstPlayerOnline->eStatus)
		{
			continue;
		}

		// 剔除自己
		if (pstPlayerOnline == pstPlayer)
		{
			continue;
		}

		// 在自己的黑名单中
		if (0 <= player_black_find_byid(&pstPlayer->stRoleData.MiscInfo, pstPlayerOnline->stRoleData.RoleID))
		{
			continue;
		}

		// 在对方黑名单中
		if (0 <= player_black_find_byid(&pstPlayerOnline->stRoleData.MiscInfo, pstPlayer->stRoleData.RoleID))
		{
			continue;
		}

		// 在对方同伴列表中
		pstBuddyInfo = relation_buddy_db_find(&pstPlayerOnline->stOnline.stRelation.Buddy, pstPlayer->stRoleData.RoleID);
		if (pstBuddyInfo)
		{
			continue;
		}

		// 在自己同伴列表中
		pstBuddyInfo = relation_buddy_db_find(&pstPlayer->stOnline.stRelation.Buddy, pstPlayerOnline->stRoleData.RoleID);
		if (pstBuddyInfo)
		{
			continue;
		}

		// 查找仇人列表
		if (relation_enemy_online_find(&pstPlayer->stRoleData.MiscInfo.Enemy, pstPlayerOnline->stRoleData.RoleID))
		{
			continue;
		}

		if (relation_enemy_online_find(&pstPlayerOnline->stRoleData.MiscInfo.Enemy, pstPlayer->stRoleData.RoleID))
		{
			continue;
		}

		//  性别不符
		if (pstPlayer->stRoleData.Gender == pstPlayerOnline->stRoleData.Gender)
		{
			continue;
		}

		// 等级不符
		if (pstPlayer->stRoleData.Level != pstPlayerOnline->stRoleData.Level)
		{
			//  找到等级最相近的人
			iLevelDiff = abs(pstPlayer->stRoleData.Level - pstPlayerOnline->stRoleData.Level);
			if (iLevelDiff < 5 )
			{
				if (pstPlayerTmp == NULL)
				{
					pstPlayerTmp = pstPlayerOnline;
				}
				else
				{
					if (abs(pstPlayerTmp->stRoleData.Level - pstPlayer->stRoleData.Level) > iLevelDiff)
					{
						pstPlayerTmp = pstPlayerOnline;
					}
				}	
			}
		}
		else
		{
			pstPlayerTmp = pstPlayerOnline;
			break;
		}
	}

	if (pstPlayerTmp)
	{
		relation_chg_clt_detail(pstEnv, pstPlayer, pstPlayerTmp, RELATION_TYPE_TMPFRIEND);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_RELATION1, pstPlayerTmp->stRoleData.RoleName);

		relation_chg_clt_detail(pstEnv, pstPlayerTmp, pstPlayer, RELATION_TYPE_TMPFRIEND);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayerTmp, SYS_TIP, ZONEERR_RELATION1, pstPlayer->stRoleData.RoleName);
	}
	
	return 0;
}

int relation_friend_req_insert(Player *pstPlayer, int iID)
{
	int i;
	int iFriendReqListSize = (int)(sizeof(pstPlayer->stOnline.aiAddFriendReqList)/sizeof(pstPlayer->stOnline.aiAddFriendReqList[0]));

	//for (i=0; i<iFriendReqListSize; i++)
	for (i=0; i<pstPlayer->stOnline.bAddFriendReqNum; i++)
	{
		if (pstPlayer->stOnline.aiAddFriendReqList[i] == iID)
		{
			return -1;
		}
	}
	
	if (pstPlayer->stOnline.bAddFriendReqNum >= iFriendReqListSize)
	{
		memmove(&pstPlayer->stOnline.aiAddFriendReqList[0], &pstPlayer->stOnline.aiAddFriendReqList[1],
			(iFriendReqListSize-1)*sizeof(pstPlayer->stOnline.aiAddFriendReqList[0]));
		pstPlayer->stOnline.aiAddFriendReqList[iFriendReqListSize-1] = iID;
	}
	else
	{
		pstPlayer->stOnline.aiAddFriendReqList[pstPlayer->stOnline.bAddFriendReqNum] = iID;
		pstPlayer->stOnline.bAddFriendReqNum++;
	}
	
	return 0;
}

// 查找请求列表
int relation_friend_req_find(Player *pstPlayer, int iID)
{
	int i;
	
	for (i=0; i<pstPlayer->stOnline.bAddFriendReqNum; i++)
	{
		if (pstPlayer->stOnline.aiAddFriendReqList[i] == iID)
		{
			return i;
		}
	}

	return -1;
}

//  删除请求列表中的请求
int relation_friend_req_del_byidx(Player *pstPlayer, int iIdx)
{
	pstPlayer->stOnline.bAddFriendReqNum --;

	if (iIdx != pstPlayer->stOnline.bAddFriendReqNum)
	{
		pstPlayer->stOnline.aiAddFriendReqList[iIdx] = pstPlayer->stOnline.aiAddFriendReqList[pstPlayer->stOnline.bAddFriendReqNum];
	}
	
	return 0;
}

int player_relation_addTmpfriend_req(ZONESVRENV * pstEnv, Player * pstPlayer, const char *pszName)
{
	Player *pstOther;

	pstOther = z_name_player(pstEnv->pstAppCtx, pstEnv, pszName);
	if (NULL == pstOther || pstOther == pstPlayer)
	{
		return -1;
	}

	/*
	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		if( pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.ZoneBusID != 
			pstOther->stOnline.stPworldQueue.stBattleCampInfo.ZoneBusID)
		{
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "同服的玩家才能加为临时好友");
			return -1;
		}
	}
	*/

	if (0 <= player_black_find_byid(&pstOther->stRoleData.MiscInfo, pstPlayer->stRoleData.RoleID))
	{
		return -1;
	}

	if (0 <= player_black_find_byid(&pstPlayer->stRoleData.MiscInfo, pstOther->stRoleData.RoleID))
	{
		return -1;
	}

	if (relation_enemy_online_find(&pstPlayer->stRoleData.MiscInfo.Enemy, pstOther->stRoleData.RoleID))
	{
		return -1;
	}

	relation_chg_clt_detail(pstEnv, pstPlayer, pstOther, RELATION_TYPE_TMPFRIEND);

	//z_sys_strf( pstEnv, pstOther, SYS_TIP, "玩家%s 添加你为临时好友",pstPlayer->stRoleData.RoleName);
	
	return 0;
}

int player_relation_add_friend_req_mud(ZONESVRENV* pstEnv, Player *pstPlayer,  ADDFRIENDREQ  *pstAddFriendReq)
{
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;

	// 加自己
	if ( 0 == strcmp(pstPlayer->stRoleData.RoleName, pstAddFriendReq->Name))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION2);
		return -1;
	}
	
	// 在对方黑名单中
	/*if (0 <= player_black_find_byid(&pstOther->stRoleData.MiscInfo, pstPlayer->stRoleData.RoleID))
	{
		snprintf(szMsg,sizeof(szMsg),"你在%s黑名单中,不能加为好友", pstAddFriendReq->Name);
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, szMsg);
		return -1;
	}*/

	// 在自己黑名单中
	if (0 <= player_black_find_byname(&pstPlayer->stRoleData.MiscInfo, pstAddFriendReq->Name))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION3);
		return -1;
	}
	

	if (relation_enemy_online_find_byname(&pstPlayer->stRoleData.MiscInfo.Enemy, pstAddFriendReq->Name))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION4);
		return -1;
	}

	/*if (relation_enemy_online_find(&pstOther->stRoleData.MiscInfo.Enemy, pstPlayer->stRoleData.RoleID))
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "你在对方宿敌名单中");
		return -1;
	}*/

	// 已经在好友名单中
	if (relation_buddy_online_find_by_name(&pstPlayer->stRoleData.Buddy, pstAddFriendReq->Name))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION5);
		return -1;
	}

	if (pstPlayer->stRoleData.Buddy.FriendNum >= pstPlayer->stRoleData.Buddy.FirendMax)
	//if (pstPlayer->stRoleData.Buddy.FriendNum >= MAX_BUDDY_FRIEND)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION6);
		
		return -1;
	}

	// 避免客户端总是弹出加用友面板
	if (0 == strcmp(pstPlayer->stOnline.szAddFriendName, pstAddFriendReq->Name))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION7);
		return -1;
	}
	STRNCPY(pstPlayer->stOnline.szAddFriendName,pstAddFriendReq->Name, CCH(pstPlayer->stOnline.szAddFriendName));
	
	pstRelationSvr->OpType = RELATION_OP_TYPE_ADDFRIEND_INFO;
	STRNCPY(pstRelationSvr->RelationSvrData.RelationAddFriendInfo.ReqName, 
		pstPlayer->stRoleData.RoleName, sizeof(pstRelationSvr->RelationSvrData.RelationAddFriendInfo.ReqName));
	STRNCPY(pstRelationSvr->RelationSvrData.RelationAddFriendInfo.ReqInfo, 
		pstAddFriendReq->ReqInfo, sizeof(pstRelationSvr->RelationSvrData.RelationAddFriendInfo.ReqInfo));
	pstRelationSvr->RelationSvrData.RelationAddFriendInfo.OnlineType = ROLE_GAME_ONLINE;
	
	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_pkg_to_mud(pstEnv, &stPkg,pstPlayer->stRoleData.RoleName,0,PKG_TRANS_PLAYER);//发给mud上的player
	
	return 0;
}

int player_relation_addfriend_req(ZONESVRENV* pstEnv, Player *pstPlayer,  ADDFRIENDREQ  *pstAddFriendReq)
{
	Player *pstOther;
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;

	//对方在mud
	if (pstAddFriendReq->OnlineType == ROLE_MUD_ONLINE)
	{
		return player_relation_add_friend_req_mud(pstEnv, pstPlayer,  pstAddFriendReq);
	}

	// 对方不在线
	pstOther = z_name_player(pstEnv->pstAppCtx, pstEnv, pstAddFriendReq->Name);
	if (NULL == pstOther)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION8, pstAddFriendReq->Name);
		return -1;
	}

	// 加自己
	if (pstOther == pstPlayer)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION2);
		return -1;
	}
	
	// 在对方黑名单中
	/*if (0 <= player_black_find_byid(&pstOther->stRoleData.MiscInfo, pstPlayer->stRoleData.RoleID))
	{
		snprintf(szMsg,sizeof(szMsg),"你在%s黑名单中,不能加为好友", pstAddFriendReq->Name);
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, szMsg);
		return -1;
	}*/

	// 在自己黑名单中
	if (0 <= player_black_find_byid(&pstPlayer->stRoleData.MiscInfo, pstOther->stRoleData.RoleID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION3);
		return -1;
	}
	

	if (relation_enemy_online_find(&pstPlayer->stRoleData.MiscInfo.Enemy, pstOther->stRoleData.RoleID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION4);
		return -1;
	}

	/*if (relation_enemy_online_find(&pstOther->stRoleData.MiscInfo.Enemy, pstPlayer->stRoleData.RoleID))
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "你在对方宿敌名单中");
		return -1;
	}*/

	// 已经在好友名单中
	if (relation_buddy_online_find(&pstPlayer->stRoleData.Buddy, pstOther->stRoleData.RoleID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION5);
		return -1;
	}

	if (pstPlayer->stRoleData.Buddy.FriendNum >= pstPlayer->stRoleData.Buddy.FirendMax)
	//if (pstPlayer->stRoleData.Buddy.FriendNum >= MAX_BUDDY_FRIEND)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION6);
		
		return -1;
	}

	// 插入请求列表
	// 避免客户端总是弹出加用友面板
	if (relation_friend_req_insert(pstPlayer, pstOther->iMemID) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION7);
		return -1;
	}

	pstRelationSvr->OpType = RELATION_OP_TYPE_ADDFRIEND_INFO;
	STRNCPY(pstRelationSvr->RelationSvrData.RelationAddFriendInfo.ReqName, 
		pstPlayer->stRoleData.RoleName, sizeof(pstRelationSvr->RelationSvrData.RelationAddFriendInfo.ReqName));
	STRNCPY(pstRelationSvr->RelationSvrData.RelationAddFriendInfo.ReqInfo, 
		pstAddFriendReq->ReqInfo, sizeof(pstRelationSvr->RelationSvrData.RelationAddFriendInfo.ReqInfo));

	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_cltmsg_send(pstEnv, pstOther, &stPkg);
	
	return 0;
}

int relation_get_role_member( Player *pstOther,ROLEBUDDYMEMBER *pstRoleBuddyMember)
{
	memset(pstRoleBuddyMember, 0, sizeof(*pstRoleBuddyMember));
	pstRoleBuddyMember->RoleID = pstOther->stRoleData.RoleID;
	STRNCPY(pstRoleBuddyMember->RoleName, pstOther->stRoleData.RoleName, sizeof(pstRoleBuddyMember->RoleName));
	pstRoleBuddyMember->RelationType = RELATION_TYPE_FRIEND;
	pstRoleBuddyMember->Gender = pstOther->stRoleData.Gender;
	pstRoleBuddyMember->GroupID = 1;
	return 0;
}

// 添加好友 pstPlayer添加pstOther为好友
int relation_friend_add(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstOther)
{
	
	ROLEBUDDYMEMBER *pstRoleBuddyMember; 
	ROLEBUDDY *pstRoleBuddy = &pstPlayer->stRoleData.Buddy;
	BUDDY *pstBuddy = &pstPlayer->stOnline.stRelation.Buddy;
	BUDDYINFO *pstBuddyInfo;

	pstRoleBuddyMember = relation_buddy_online_find(pstRoleBuddy, pstOther->stRoleData.RoleID);
	if (pstRoleBuddyMember)
	{
		return -1;
	}

	if (pstRoleBuddy->FriendNum >= pstRoleBuddy->FirendMax)
	//if (pstRoleBuddy->FriendNum >= MAX_BUDDY_FRIEND)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_RELATION6);
		
		return -1;
	}

	pstRoleBuddyMember = &pstRoleBuddy->Friends[pstRoleBuddy->FriendNum++];
	relation_get_role_member(pstOther,pstRoleBuddyMember);

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE, ZONEERR_RELATION9, pstPlayer->stRoleData.RoleName);
	
	if (pstPlayer->stOnline.bGetBuddyStat != BUDDY_GET_FINI)
	{
		return 0;
	}

	if (pstBuddy->BuddyNum >= MAX_BUDDY)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "buddy num err");
		return -1;
	}

	pstBuddyInfo = &pstBuddy->BuddyInfos[pstBuddy->BuddyNum++];
	relation_buddyInfo_get(pstOther, pstBuddyInfo);

	relation_chg_clt_detail(pstEnv, pstPlayer, pstOther, RELATION_TYPE_FRIEND);	

	pstPlayer->stOnline.bRelationUpdateFlag = 2;

	// 称号
	player_desig_get(pstEnv, pstPlayer, GET_DESIG_FRIEND_NUM);

	//lzk   完成加好友特殊任务
	task_player_trace(pstEnv, pstPlayer, TASK_OBJ_ADD_FRIEND);
	return 0;
}

// 添加好友 pstPlayer添加pszOther为好友
int relation_friend_add_mud(ZONESVRENV* pstEnv, Player *pstPlayer,char *pszOther, ROLEBUDDYMEMBER *pstOtherBuddyMember,BUDDYINFO *pstOtherBuddyInfo)
{	
	ROLEBUDDYMEMBER *pstTmpBuddyMember; 
	ROLEBUDDY *pstRoleBuddy = &pstPlayer->stRoleData.Buddy;
	BUDDY *pstBuddy = &pstPlayer->stOnline.stRelation.Buddy;
	BUDDYINFO *pstBuddyInfo;

	if (!pstOtherBuddyMember || !pstOtherBuddyInfo)
	{
		return -1;
	}
	
	pstTmpBuddyMember = relation_buddy_online_find_by_name(pstRoleBuddy, pszOther);
	if (pstTmpBuddyMember)
	{
		return -1;
	}

	if (pstRoleBuddy->FriendNum >= pstRoleBuddy->FirendMax)
	//if (pstRoleBuddy->FriendNum >= MAX_BUDDY_FRIEND)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION6);
		
		return -1;
	}


	pstTmpBuddyMember = &pstRoleBuddy->Friends[pstRoleBuddy->FriendNum++];
	(*pstTmpBuddyMember) = (*pstOtherBuddyMember);

	z_send_sys_str_mud(pstEnv->pstAppCtx, pstEnv, pszOther, SYS_MIDDLE2, ZONEERR_MUD101, pstPlayer->stRoleData.RoleName);
	
	if (pstPlayer->stOnline.bGetBuddyStat != BUDDY_GET_FINI)
	{
		return 0;
	}

	if (pstBuddy->BuddyNum >= MAX_BUDDY)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "buddy num err");
		return -1;
	}

	pstBuddyInfo = &pstBuddy->BuddyInfos[pstBuddy->BuddyNum++];
	(*pstBuddyInfo) = (*pstOtherBuddyInfo);

	//mud中的pszother 发好友详细信息给pstPlayer
	z_cmd_to_mud(pstEnv,pstPlayer->stRoleData.RoleName, pszOther,SS_CMD_FRIEND_DETAIL,NULL);

	pstPlayer->stOnline.bRelationUpdateFlag = 2;

	//lzk   完成加好友特殊任务
	task_player_trace(pstEnv, pstPlayer, TASK_OBJ_ADD_FRIEND);
	
	return 0;
}

int player_reject_add_mud(ZONESVRENV* pstEnv, Player *pstPlayer, char* pszOther)
{
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONREJECT *pstRelationReject = &pstRelationSvr->RelationSvrData.RelationReject;

	z_send_sys_str_mud(pstEnv->pstAppCtx, pstEnv, pszOther, SYS_MIDDLE2, ZONEERR_MUD102, pstPlayer->stRoleData.RoleName);

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION10, pszOther);

	pstRelationSvr->OpType = RELATION_OP_TYPE_REJECT;
	pstRelationReject->Type = RELATION_TYPE_FRIEND;
	pstRelationReject->RoleID = pstPlayer->stRoleData.RoleID;
	STRNCPY(pstRelationReject->RejectName, pstPlayer->stRoleData.RoleName,
		sizeof(pstRelationReject->RejectName));

	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_pkg_to_mud(pstEnv,&stPkg,pszOther, 0,PKG_TRANS_PLAYER);
	return 0;
}

int player_reject_add(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstOther, int iReason)
{
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONREJECT *pstRelationReject = &pstRelationSvr->RelationSvrData.RelationReject;

	if (iReason == 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE2, ZONEERR_RELATION11, pstPlayer->stRoleData.RoleName);
	}
	else
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE2, ZONEERR_RELATION12, pstPlayer->stRoleData.RoleName);

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION10, pstOther->stRoleData.RoleName);
	}
	
	pstRelationSvr->OpType = RELATION_OP_TYPE_REJECT;
	pstRelationReject->Type = RELATION_TYPE_FRIEND;
	pstRelationReject->RoleID = pstPlayer->stRoleData.RoleID;
	STRNCPY(pstRelationReject->RejectName, pstPlayer->stRoleData.RoleName,
		sizeof(pstRelationReject->RejectName));

	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_cltmsg_send(pstEnv, pstOther, &stPkg);
	return 0;
}

int player_del_black_in(ZONESVRENV* pstEnv, Player *pstPlayer, int iIdx)
{
	ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONDELRES *pstRelationDelRes = &pstRelationSvr->RelationSvrData.RelationDelRes;  
	tdr_ulonglong RoleID = 0;

	if (iIdx < 0 || iIdx >= MAX_BLACK_NUM)
	{
		return -1;
	}

	RoleID = pstMisc->BlackMember[iIdx].RoleID;
	
	pstMisc->BlackNum --;
	if (iIdx != pstMisc->BlackNum)
	{
		memcpy (&pstMisc->BlackMember[iIdx], &pstMisc->BlackMember[pstMisc->BlackNum], 
					sizeof(pstMisc->BlackMember[0]));
	}

	pstRelationSvr->OpType = RELATION_OP_TYPE_DELRELATION_RES;
	
	pstRelationDelRes->Type = RELATION_TYPE_BLACK;
	pstRelationDelRes->RoleID = RoleID;
		
	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_del_black_by_name(ZONESVRENV* pstEnv, Player *pstPlayer, char *pszOther)
{
	int iIdx = -1;
	ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;
	
	iIdx = player_black_find_byname(pstMisc, pszOther);
	if (iIdx == -1)
	{
		return -1;
	}
	player_del_black_in(pstEnv,pstPlayer,iIdx);

	return 0;
}

int player_del_black(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstOther)
{
	int iIdx = -1;
	ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;
	
	iIdx = player_black_find_byid(pstMisc, pstOther->stRoleData.RoleID);
	if (iIdx == -1)
	{
		return -1;
	}

	player_del_black_in(pstEnv,pstPlayer,iIdx);
	return 0;
}

int player_del_enemy(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstEnemy)
{
	int i;
	int iIdx = -1;
	ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONDELRES *pstRelationDelRes = &pstRelationSvr->RelationSvrData.RelationDelRes;  
	tdr_ulonglong RoleID = 0;
	

	for (i=0; i<pstMisc->Enemy.EnemyNum; i++)
	{
		if (pstMisc->Enemy.Enemys[i].RoleID == pstEnemy->stRoleData.RoleID)
		{
			iIdx = i;
			break;
		}
	}

	if (iIdx < 0 || iIdx >= MAX_ENEMY) return 0;

	RoleID = pstMisc->Enemy.Enemys[iIdx].RoleID;
	
	pstMisc->Enemy.EnemyNum -- ;
	if (iIdx != pstMisc->Enemy.EnemyNum)
	{
		memcpy (&pstMisc->Enemy.Enemys[iIdx], &pstMisc->Enemy.Enemys[pstMisc->Enemy.EnemyNum], 
					sizeof(pstMisc->Enemy.Enemys[0]));
	}

	pstRelationSvr->OpType = RELATION_OP_TYPE_DELRELATION_RES;
	
	pstRelationDelRes->Type = RELATION_TYPE_ENEMY;
	pstRelationDelRes->RoleID = RoleID;
		
	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_relation_addfriend_res_mud(ZONESVRENV* pstEnv, Player *pstPlayer, CSADDFRIENDRES *pstAddFriendRes,ROLEBUDDYMEMBER *pstOtherBuddyMember,BUDDYINFO *pstOtherBuddyInfo)
{
	// B同意A加自己为好友

	SSINFOGETREQ stInfoGet;
	SSCMDDATA stData;
	
	if (pstAddFriendRes->AddOther)
	{
		z_send_sys_str_mud(pstEnv->pstAppCtx, pstEnv, pstAddFriendRes->ReqName, SYS_MIDDLE2, ZONEERR_MUD103, pstPlayer->stRoleData.RoleName);
		return -1;
	}
	
	if (0 == pstAddFriendRes->Accept)
	{
		player_reject_add_mud(pstEnv, pstPlayer, pstAddFriendRes->ReqName);
		return 0;
	}

	if(!pstOtherBuddyMember || !pstOtherBuddyInfo)
	{
		stInfoGet.ReqBody.BuddyMember.AddFriendRes = *pstAddFriendRes;
		STRNCPY(stInfoGet.ReqBody.BuddyMember.Name,pstPlayer->stRoleData.RoleName,
			CCH(stInfoGet.ReqBody.BuddyMember.Name));
		z_info_get_to_mud(pstEnv,&stInfoGet, INFO_GET_BUDDY_MEMBER);
		return 0;
	}
	
	if (relation_friend_add_mud(pstEnv, pstPlayer, pstAddFriendRes->ReqName,pstOtherBuddyMember,pstOtherBuddyInfo) == 0)
	{
		// 删除黑名单
		player_del_black_by_name(pstEnv,pstPlayer, pstAddFriendRes->ReqName);
	}
	//mud 中的A加B为好友
	relation_get_role_member(pstPlayer,&stData.FriendAdd.BuddyMember);
	relation_buddyInfo_get(pstPlayer, &stData.FriendAdd.BuddyInfo);
	z_cmd_to_mud(pstEnv, pstPlayer->stRoleData.RoleName,pstAddFriendRes->ReqName,SS_CMD_FRIEND_ADD,&stData);
	
	return 0;
}

int player_relation_addfriend_res(ZONESVRENV* pstEnv, Player *pstPlayer, CSADDFRIENDRES *pstAddFriendRes)
{
	// B同意A加自己为好友
	
	int iIdx;
	Player *pstOther; // A

	if (ROLE_MUD_ONLINE == pstAddFriendRes->OnlineType )//A在mud
	{
		return player_relation_addfriend_res_mud(pstEnv,pstPlayer,pstAddFriendRes,NULL,NULL);
	}

	pstOther = z_name_player(pstEnv->pstAppCtx, pstEnv, pstAddFriendRes->ReqName);
	if (NULL == pstOther)
	{
		return -1;
	}

	iIdx = relation_friend_req_find(pstOther, pstPlayer->iMemID);
	if (0 > iIdx)
	{
		return -1;
	}

	relation_friend_req_del_byidx(pstOther, iIdx);
	// 在自己黑名单中
	/*if (0 <= player_black_find_byid(&pstPlayer->stRoleData.MiscInfo, pstOther->stRoleData.RoleID))
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "目标在你的黑名单,无法增加好友,请按F键打开黑名单查看");
		return -1;
	}

	if (relation_enemy_online_find(&pstPlayer->stRoleData.MiscInfo.Enemy, pstOther->stRoleData.RoleID))
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "目标在你的宿敌名单,无法增加好友,请按F键打开宿敌名单查看");
		return -1;
	}

	if (pstAddFriendRes->AddOther)
	{
		char szMsg[128];

		sprintf(szMsg, "%s 拒绝了你的好友申请", pstPlayer->stRoleData.RoleName);
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE2, szMsg);
		return -1;
	}
	*/
	if (0 == pstAddFriendRes->Accept)
	{
		player_reject_add(pstEnv, pstPlayer, pstOther, pstAddFriendRes->Reason);
		return 0;
	}

	if (relation_friend_add(pstEnv, pstOther, pstPlayer) == 0)
	{
		// 删除黑名单
		player_del_black(pstEnv,pstOther, pstPlayer);
		player_del_enemy(pstEnv, pstOther, pstPlayer);
	}
	
	if (relation_friend_add(pstEnv, pstPlayer, pstOther) == 0)
	{
		// 删除黑名单
		player_del_black(pstEnv,pstPlayer, pstOther);
		player_del_enemy(pstEnv,pstPlayer, pstOther);
	}
	
	
	
	return 0;
}

void relation_buddyInfo_get(Player *pstPlayer, BUDDYINFO *pstBuddyInfo)
{
	pstBuddyInfo->RoleID = pstPlayer->stRoleData.RoleID;
	pstBuddyInfo->Head = pstPlayer->stRoleData.Head;
	pstBuddyInfo->Career = pstPlayer->stRoleData.Career;
	pstBuddyInfo->Level = pstPlayer->stRoleData.Level;
	STRNCPY(pstBuddyInfo->Emotion, pstPlayer->stRoleData.Buddy.Emotion, sizeof(pstBuddyInfo->Emotion));
	pstBuddyInfo->ClanID = pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id;
	STRNCPY(pstBuddyInfo->ClanName, pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Name, sizeof(pstBuddyInfo->ClanName));
	pstBuddyInfo->Face = pstPlayer->stRoleData.Face;
	pstBuddyInfo->Hair = pstPlayer->stRoleData.Hair;
	return;
}

int relation_db_info_chg(ZONESVRENV* pstEnv, ROLEBUDDYMEMBER *pstRoleBuddyMember, Player *pstPlayer)
{
	DBAsyn stDBAsyn;

	stDBAsyn.iCmd = DB_BUDDY_CHG;
	relation_buddyInfo_get(pstPlayer, &stDBAsyn.AsynCmd.stBuddyChg.stBuddyInfo);
	STRNCPY(stDBAsyn.AsynCmd.stBuddyChg.szName, pstRoleBuddyMember->RoleName, sizeof(stDBAsyn.AsynCmd.stBuddyChg.szName));
	relation_db_get(pstEnv, pstRoleBuddyMember->RoleID, -1, &stDBAsyn);
	
	return 0;
}

int relation_dbbuddy_chg(ZONESVRENV* pstEnv, Player *pstPlayer, int iFlag,int iModifyDB)
{
	int i;
	Player *pstPlayerOnline;
	ROLEBUDDYMEMBER *pstRoleBuddyMember;
	ROLEBUDDY *pstRoleBuddy = &pstPlayer->stRoleData.Buddy;
	BUDDYINFO * pstBuddyInfo;

	for (i=0; i<pstRoleBuddy->BrotherNum; i++)
	{
		pstRoleBuddyMember = &pstRoleBuddy->Brothers[i];
		pstRoleBuddyMember->Flag = 1;

		pstPlayerOnline = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleBuddyMember->RoleName);
		if (pstPlayerOnline)
		{
			if (pstPlayerOnline->stOnline.bGetBuddyStat != BUDDY_GET_FINI)
			{
				continue;
			}

			pstBuddyInfo = relation_buddy_db_find(&pstPlayerOnline->stOnline.stRelation.Buddy, pstPlayer->stRoleData.RoleID);

			if (pstBuddyInfo)
			{
				relation_buddyInfo_get(pstPlayer, pstBuddyInfo);
				pstPlayerOnline->stOnline.bRelationUpdateFlag = 1;

				//todo 通知客户端
				if (iFlag == 0)
				{
					relation_dbbuddy_chg_clt(pstEnv, pstPlayerOnline, pstBuddyInfo, RELATION_TYPE_BROTHER);
				}
				else if (iFlag == 1)
				{
					relation_claninfo_to_clt(pstEnv, pstPlayerOnline, pstPlayer);
				}
			}

			pstRoleBuddyMember->Flag = 0;
		}
		else
		{
			// 不在线   修改数据库
			if (iModifyDB)
			{
				relation_db_info_chg(pstEnv, pstRoleBuddyMember, pstPlayer);
				pstRoleBuddyMember->Flag = 0;
			}
				
		}
	}

	for (i=0; i<pstRoleBuddy->FriendNum; i++)
	{
		pstRoleBuddyMember = &pstRoleBuddy->Friends[i];
		pstRoleBuddyMember->Flag = 1;

		pstPlayerOnline = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleBuddyMember->RoleName);

		if (pstPlayerOnline)
		{
			if (pstPlayerOnline->stOnline.bGetBuddyStat != BUDDY_GET_FINI)
			{
				continue;
			}

			pstBuddyInfo = relation_buddy_db_find(&pstPlayerOnline->stOnline.stRelation.Buddy, pstPlayer->stRoleData.RoleID);

			if (pstBuddyInfo)
			{
				relation_buddyInfo_get(pstPlayer, pstBuddyInfo);
				pstPlayerOnline->stOnline.bRelationUpdateFlag = 1;

				if (iFlag == 0)
				{
					relation_dbbuddy_chg_clt(pstEnv, pstPlayerOnline, pstBuddyInfo, RELATION_TYPE_FRIEND);
				}
				else if (iFlag == 1)
				{
					relation_claninfo_to_clt(pstEnv, pstPlayerOnline, pstPlayer);
				}
			}
			pstRoleBuddyMember->Flag = 0;
		}
		else
		{
			if (iModifyDB)
			{
				relation_db_info_chg(pstEnv, pstRoleBuddyMember, pstPlayer);
				pstRoleBuddyMember->Flag = 0;
			}
		}
	}

	for (i=0; i<pstRoleBuddy->PrenticeNum; i++)
	{
		pstRoleBuddyMember = &pstRoleBuddy->Prentices[i];
		pstRoleBuddyMember->Flag = 1;

		pstPlayerOnline = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleBuddyMember->RoleName);

		if (pstPlayerOnline)
		{
			if (pstPlayerOnline->stOnline.bGetBuddyStat != BUDDY_GET_FINI)
			{
				continue;
			}

			pstBuddyInfo = relation_buddy_db_find(&pstPlayerOnline->stOnline.stRelation.Buddy, pstPlayer->stRoleData.RoleID);

			if (pstBuddyInfo)
			{
				relation_buddyInfo_get(pstPlayer, pstBuddyInfo);
				pstPlayerOnline->stOnline.bRelationUpdateFlag = 1;

				if (iFlag == 0)
				{
					relation_dbbuddy_chg_clt(pstEnv, pstPlayerOnline, pstBuddyInfo, RELATION_TYPE_PRENTICE);
				}
				else if (iFlag == 1)
				{
					relation_claninfo_to_clt(pstEnv, pstPlayerOnline, pstPlayer);
				}
			}
			pstRoleBuddyMember->Flag = 0;
		}
		else
		{
			if (iModifyDB)
			{
				relation_db_info_chg(pstEnv, pstRoleBuddyMember, pstPlayer);
				pstRoleBuddyMember->Flag = 0;
			}
		}
	}

	for (i=0; i<pstRoleBuddy->MasterNum; i++)
	{
		pstRoleBuddyMember = &pstRoleBuddy->Master[i];
		pstRoleBuddyMember->Flag = 1;

		pstPlayerOnline = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleBuddyMember->RoleName);

		if (pstPlayerOnline)
		{
			if (pstPlayerOnline->stOnline.bGetBuddyStat != BUDDY_GET_FINI)
			{
				continue;
			}

			pstBuddyInfo = relation_buddy_db_find(&pstPlayerOnline->stOnline.stRelation.Buddy, pstPlayer->stRoleData.RoleID);

			if (pstBuddyInfo)
			{
				relation_buddyInfo_get(pstPlayer, pstBuddyInfo);
				pstPlayerOnline->stOnline.bRelationUpdateFlag = 1;

				if (iFlag == 0)
				{
					relation_dbbuddy_chg_clt(pstEnv, pstPlayerOnline, pstBuddyInfo, RELATION_TYPE_MASTER);
				}
				else if (iFlag == 1)
				{
					relation_claninfo_to_clt(pstEnv, pstPlayerOnline, pstPlayer);
				}
			}
			pstRoleBuddyMember->Flag = 0;
		}
		else
		{
			if (iModifyDB)
			{
				relation_db_info_chg(pstEnv, pstRoleBuddyMember, pstPlayer);
				pstRoleBuddyMember->Flag = 0;
			}
		}
	}

/*
	for (i=0; i<pstRoleBuddy->SpouseNum; i++)
	{
		pstRoleBuddyMember = &pstRoleBuddy->Spouse[i];
		pstRoleBuddyMember->Flag = 1;

		pstPlayerOnline = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleBuddyMember->RoleName);

		if (pstPlayerOnline)
		{
			if (pstPlayerOnline->stOnline.bGetBuddyStat != BUDDY_GET_FINI)
			{
				continue;
			}

			pstBuddyInfo = relation_buddy_db_find(&pstPlayerOnline->stOnline.stRelation.Buddy, pstPlayer->stRoleData.RoleID);

			if (pstBuddyInfo)
			{
				relation_buddyInfo_get(pstPlayer, pstBuddyInfo);
				pstPlayerOnline->stOnline.bRelationUpdateFlag = 1;

				if (iFlag == 0)
				{
					relation_dbbuddy_chg_clt(pstEnv, pstPlayerOnline, pstBuddyInfo, RELATION_TYPE_SPOUSE);
				}
				else if (iFlag == 1)
				{
					relation_claninfo_to_clt(pstEnv, pstPlayerOnline, pstPlayer);
				}
			}
			pstRoleBuddyMember->Flag = 0;
		}
		else
		{
			if (iModifyDB)
			{
				relation_db_info_chg(pstEnv, pstRoleBuddyMember, pstPlayer);
				pstRoleBuddyMember->Flag = 0;
			}
		}
	}
	*/

	return 0;
}


int player_logout_up_emotion(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	//Player *pstPlayerOnline;
	ROLEBUDDYMEMBER *pstRoleBuddyMember;
	ROLEBUDDY *pstRoleBuddy = &pstPlayer->stRoleData.Buddy;
//	BUDDYINFO * pstBuddyInfo;

	for (i=0; i<pstRoleBuddy->BrotherNum; i++)
	{
		pstRoleBuddyMember = &pstRoleBuddy->Brothers[i];
		if (pstRoleBuddyMember->Flag == 1)
		{
			relation_db_info_chg(pstEnv, pstRoleBuddyMember, pstPlayer);
			pstRoleBuddyMember->Flag = 0;
		}
	}

	for (i=0; i<pstRoleBuddy->FriendNum; i++)
	{
		pstRoleBuddyMember = &pstRoleBuddy->Friends[i];
		if (pstRoleBuddyMember->Flag == 1)
		{
			relation_db_info_chg(pstEnv, pstRoleBuddyMember, pstPlayer);
			pstRoleBuddyMember->Flag = 0;
		}
	}

	for (i=0; i<pstRoleBuddy->PrenticeNum; i++)
	{
		pstRoleBuddyMember = &pstRoleBuddy->Prentices[i];
		if (pstRoleBuddyMember->Flag == 1)
		{
			relation_db_info_chg(pstEnv, pstRoleBuddyMember, pstPlayer);
			pstRoleBuddyMember->Flag = 0;
		}
	}
	
	for (i=0; i<pstRoleBuddy->MasterNum; i++)
	{
		pstRoleBuddyMember = &pstRoleBuddy->Master[i];
		if (pstRoleBuddyMember->Flag == 1)
		{
			relation_db_info_chg(pstEnv, pstRoleBuddyMember, pstPlayer);
			pstRoleBuddyMember->Flag = 0;
		}
	}

/*
	for (i=0; i<pstRoleBuddy->SpouseNum; i++)
	{
		pstRoleBuddyMember = &pstRoleBuddy->Spouse[i];
		if (pstRoleBuddyMember->Flag == 1)
		{
			relation_db_info_chg(pstEnv, pstRoleBuddyMember, pstPlayer);
			pstRoleBuddyMember->Flag = 0;
		}
	}
*/
	
	return 0;
}

int player_relation_emotion(ZONESVRENV* pstEnv, Player *pstPlayer, CSEMOTIONREQ *pstEmotionReq)
{
	ROLEBUDDY *pstRoleBuddy = &pstPlayer->stRoleData.Buddy;
	
	if (pstEnv->pstAppCtx->stCurr.tv_sec - pstRoleBuddy->EmotionAlterTime < EMOTION_ALTER_INTERVAL)
	{	
		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "心情签名1小时只能改一次");
		//return 0;
	}

	if( CheckDirty(pstEmotionReq->Emotion, 0xf) & 0xf )
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_DIRTY);
		return -1;
	}
		
	if (0 == strcmp(pstEmotionReq->Emotion, pstRoleBuddy->Emotion))
	{
		return 0;
	}
	else
	{
		STRNCPY(pstRoleBuddy->Emotion, pstEmotionReq->Emotion, sizeof(pstRoleBuddy->Emotion));
	}

	pstRoleBuddy->EmotionAlterTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	relation_self_emotion_to_clt(pstEnv, pstPlayer);

	relation_dbbuddy_chg(pstEnv, pstPlayer, 0,0);

	pstPlayer->stOnline.bRelationUpdateFlag = 1;
	
	return 0;
}

ROLEBUDDYMEMBER* player_find_friend_by_name(Player *pstPlayer, const char *pszName)
{
	int i;

	for (i=0; i<pstPlayer->stRoleData.Buddy.FriendNum; i++)
	{
		if (strcmp(pstPlayer->stRoleData.Buddy.Friends[i].RoleName, pszName) == 0)
		{
			return &pstPlayer->stRoleData.Buddy.Friends[i];
		}
	}
	return NULL;
}

ROLEBUDDYMEMBER *relation_buddy_online_find_by_name(ROLEBUDDY *pstRoleBuddy, char* szRoleName)
{
	int i;
	
	for (i=0; i<pstRoleBuddy->FriendNum; i++)
	{
		if (strcmp(pstRoleBuddy->Friends[i].RoleName, szRoleName) == 0)
		{
			return &pstRoleBuddy->Friends[i];
		}
	}

	for (i=0; i<pstRoleBuddy->PrenticeNum; i++)
	{
		if (strcmp(pstRoleBuddy->Prentices[i].RoleName, szRoleName) == 0)
		{
			return &pstRoleBuddy->Prentices[i];
		}
	}

	for (i=0; i<pstRoleBuddy->BrotherNum; i++)
	{
		if (strcmp(pstRoleBuddy->Brothers[i].RoleName, szRoleName) == 0)
		{
			return &pstRoleBuddy->Brothers[i];
		}
	}

	for (i=0; i<pstRoleBuddy->MasterNum; i++)
	{
		if (strcmp(pstRoleBuddy->Master[i].RoleName, szRoleName) == 0)
		{
			return &pstRoleBuddy->Master[i];
		}
	}


	for (i=0; i<pstRoleBuddy->SpouseNum; i++)
	{
		if (strcmp(pstRoleBuddy->Spouse[i].RoleName, szRoleName) == 0)
		{
			return &pstRoleBuddy->Spouse[i];
		}
	}

	return NULL;
}

ROLEBUDDYMEMBER *relation_buddy_online_find_by_roleid(ROLEBUDDY *pstRoleBuddy, tdr_ulonglong ullRoleID)
{
	int i;
	
	for (i=0; i<pstRoleBuddy->FriendNum; i++)
	{
		if (pstRoleBuddy->Friends[i].RoleID == ullRoleID)
		{
			return &pstRoleBuddy->Friends[i];
		}
	}

	for (i=0; i<pstRoleBuddy->PrenticeNum; i++)
	{
		if (pstRoleBuddy->Prentices[i].RoleID == ullRoleID)
		{
			return &pstRoleBuddy->Prentices[i];
		}
	}

	for (i=0; i<pstRoleBuddy->BrotherNum; i++)
	{
		if (pstRoleBuddy->Brothers[i].RoleID == ullRoleID)
		{
			return &pstRoleBuddy->Brothers[i];
		}
	}

	for (i=0; i<pstRoleBuddy->MasterNum; i++)
	{
		if (pstRoleBuddy->Master[i].RoleID == ullRoleID)
		{
			return &pstRoleBuddy->Master[i];
		}
	}

	
	for (i=0; i<pstRoleBuddy->SpouseNum; i++)
	{
		if (pstRoleBuddy->Spouse[i].RoleID == ullRoleID)
		{
			return &pstRoleBuddy->Spouse[i];
		}
	}
	
	
	return NULL;
}


// 修改备注姓名,此消息不会回复客户端, 客户端发过来之后直接修改备注姓名
static int player_relation_memname_req(ZONESVRENV* pstEnv, Player *pstPlayer, CSMEMNAMEREQ *pstMemNameReq)
{
	ROLEBUDDY *pstRoleBuddy = &pstPlayer->stRoleData.Buddy;
	
	ROLEBUDDYMEMBER *pstMember = relation_buddy_online_find_by_name(pstRoleBuddy , pstMemNameReq->Name);
	if (NULL == pstMember)
	{
		return -1;
	}

	if (strcmp(pstMember->MemName, pstMemNameReq->MEMNAME))
	{
		STRNCPY(pstMember->MemName, pstMemNameReq->MEMNAME, sizeof(pstMember->MemName));
	}
	else
	{
		return -1;
	}
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}


static int player_relation_delrelation_req(ZONESVRENV* pstEnv, Player *pstPlayer, CSDELRELATIONREQ*pstDelRelationReq)
{
	int iIdx;
	ROLEBUDDY *pstRoleBuddy = &pstPlayer->stRoleData.Buddy;
	BUDDYINFO *pstBuddyInfo = NULL;
	BUDDY *pstBuddy;
	ROLEENEMY *pstEnemy = &pstPlayer->stRoleData.MiscInfo.Enemy;
	ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONDELRES *pstRelationDelRes = &pstRelationSvr->RelationSvrData.RelationDelRes;  
	//char *pszBuff = "要删除的角色不存在";

	pstRelationSvr->OpType = RELATION_OP_TYPE_DELRELATION_RES;
	
	if (pstDelRelationReq->Type ==  RELATION_TYPE_FRIEND)
	{
		Player *pstFriendPlayer = NULL;
		int iIdx2 = 0;
		ROLEBUDDYMEMBER *pstBuddyMember = NULL;
		
		iIdx = online_info_find_friend_byID(pstRoleBuddy, pstDelRelationReq->RoleID);
		if (iIdx == -1)
		{
			//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, pszBuff);
			return -1;
		}

		if(pstRoleBuddy->Friends[iIdx].IntimateColorType == INTIMATE_COLOR_LOVERS)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING9);
			return -1;
		}
		
		if(pstRoleBuddy->Friends[iIdx].IntimateColorType == INTIMATE_COLOR_SPOUSE)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING10);
			return -1;
		}
		
		pstFriendPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pstRoleBuddy->Friends[iIdx].RoleName);
		
		if(NULL != pstFriendPlayer)
		{
			ROLEBUDDY *pstRoleBuddy2 = &pstFriendPlayer->stRoleData.Buddy;
			iIdx2 = online_info_find_friend_byID(pstRoleBuddy2, pstPlayer->stRoleData.RoleID);
			if(iIdx2 >= 0)
			{
				//对方亲密度清0
				pstBuddyMember = &pstRoleBuddy2->Friends[iIdx2];
				pstBuddyMember->BuddyVal = 1;
				relation_buddyval_change(pstEnv, pstFriendPlayer, pstBuddyMember, -1);
				//要不要提示一下，比较好呢？
			}
		}
		else
		{			
			DBRoleBuddy stDBRoleBuddy;
			tdr_ulonglong ullRoleID2 = pstRoleBuddy->Friends[iIdx].RoleID;
			memset(&stDBRoleBuddy,0,sizeof(stDBRoleBuddy));
		
			stDBRoleBuddy.ullRoleID = ullRoleID2;
			stDBRoleBuddy.type = DB_WEDDING_REMOVE_FRIEND;
			stDBRoleBuddy.ullOtherRoleID = pstPlayer->stRoleData.RoleID;
			
			z_get_role_buddy(pstEnv,&stDBRoleBuddy,ullRoleID2);
		}

		pstRoleBuddy->FriendNum --;
		if (iIdx != pstRoleBuddy->FriendNum)
		{
			memcpy(&pstRoleBuddy->Friends[iIdx], &pstRoleBuddy->Friends[pstRoleBuddy->FriendNum], sizeof(pstRoleBuddy->Friends[0]));
		}

		if (pstPlayer->stOnline.bGetBuddyStat != BUDDY_GET_FINI)
		{
			return 0;
		}
		
		pstBuddy = &pstPlayer->stOnline.stRelation.Buddy;
		pstBuddyInfo = relation_buddy_db_find(pstBuddy, pstDelRelationReq->RoleID);
		if (pstBuddyInfo == NULL)
		{
			return -1;
		}
		
		pstBuddy->BuddyNum --;
		if (pstBuddyInfo != &pstBuddy->BuddyInfos[pstBuddy->BuddyNum])
		{
			memcpy(pstBuddyInfo, &pstBuddy->BuddyInfos[pstBuddy->BuddyNum], sizeof(*pstBuddyInfo));
		}

		pstRelationDelRes->Type = RELATION_TYPE_FRIEND;
		pstRelationDelRes->RoleID = pstDelRelationReq->RoleID;
			
		Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		
		pstPlayer->stOnline.bRelationUpdateFlag = 2;
	}
	else if (pstDelRelationReq->Type == RELATION_TYPE_ENEMY)
	{
		ROLEENEMYMEMBER *pstEnemyMemBer;

		pstEnemyMemBer = relation_enemy_online_find(pstEnemy , pstDelRelationReq->RoleID);
		if (!pstEnemyMemBer)
		{
			//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, pszBuff);
			return -1;
		}

		pstEnemy->EnemyNum --;
		if (pstEnemyMemBer != &pstEnemy->Enemys[pstEnemy->EnemyNum])
		{
			memcpy(pstEnemyMemBer, &pstEnemy->Enemys[pstEnemy->EnemyNum], sizeof(pstEnemy->Enemys[0]));
		}

		pstRelationDelRes->Type = RELATION_TYPE_ENEMY;
		pstRelationDelRes->RoleID = pstDelRelationReq->RoleID;
			
		Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	else if (pstDelRelationReq->Type == RELATION_TYPE_BLACK)
	{
		iIdx = player_black_find_byid(pstMisc, pstDelRelationReq->RoleID);
		if (iIdx == -1)
		{
			//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, pszBuff);
			return -1;
		}

		pstMisc->BlackNum --;
		if (iIdx != pstMisc->BlackNum)
		{
			memcpy (&pstMisc->BlackMember[iIdx], &pstMisc->BlackMember[pstMisc->BlackNum], sizeof(pstMisc->BlackMember[0]));
		}

		pstRelationDelRes->Type = RELATION_TYPE_BLACK;
		pstRelationDelRes->RoleID = pstDelRelationReq->RoleID;
			
		Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	else 
	{
		return -1;
	}
	
	return 0;
}

// 加 黑名单  必须对方在线    pstPlayer添加pstOther为黑名单
int player_relation_addblack_req(ZONESVRENV* pstEnv, Player *pstPlayer, ADDBLACKREQ *pstAddBlackReq)
{
	Player *pstOther;
	ROLEMISCINFO *pstMisc;
	int iIdx;
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	BUDDYINFO *pstBuddyInfo;
	BUDDY *pstBuddy = &pstPlayer->stOnline.stRelation.Buddy;
	ROLEBUDDYMEMBER *pstBuddyMember;
	ROLEBUDDY *pstRoleBuddy2 = NULL;
	int iIdx2;
	
	pstMisc = &pstPlayer->stRoleData.MiscInfo;

	// 不在线
	pstOther = z_name_player(pstEnv->pstAppCtx, pstEnv, pstAddBlackReq->Name);
	if (NULL == pstOther)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION13, pstAddBlackReq->Name);
		return -1;
	}
	
	// 加自己
	if (pstOther == pstPlayer)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION14);
		return -1;
	}

	// 已经在黑名单中
	iIdx = player_black_find_byname(pstMisc, pstAddBlackReq->Name);
	if (iIdx >=0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION15);
		return -1;
	}

	if (pstMisc->BlackNum >= MAX_BLACK_NUM)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION16);
		return -1;
		//pstMisc->BlackNum --;
		//memmove(&pstMisc->BlackMember[0], &pstMisc->BlackMember[1], (pstMisc->BlackNum)*sizeof(pstMisc->BlackMember[0]));
		
	}

	pstBuddyMember =  relation_buddy_online_find_by_name(&pstPlayer->stRoleData.Buddy, pstAddBlackReq->Name);
	if (pstBuddyMember)
	{
		if (pstBuddyMember->RelationType == RELATION_TYPE_FRIEND)
		{
			if(pstBuddyMember->IntimateColorType == INTIMATE_COLOR_LOVERS)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING76);
				return -1;
			}

			if(pstBuddyMember->IntimateColorType == INTIMATE_COLOR_SPOUSE)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING77);
				return -1;
			}
			
			// 在好友列表中添加黑名单   删除好友信息
			iIdx = online_info_find_friend_byname(&pstPlayer->stRoleData.Buddy, pstAddBlackReq->Name);
			if (iIdx >= 0)
			{	
				pstPlayer->stRoleData.Buddy.FriendNum --;
				if(iIdx != pstPlayer->stRoleData.Buddy.FriendNum)
				{
					memcpy (&pstPlayer->stRoleData.Buddy.Friends[iIdx], &pstPlayer->stRoleData.Buddy.Friends[pstPlayer->stRoleData.Buddy.FriendNum], sizeof(pstPlayer->stRoleData.Buddy.Friends[0]));
				}

				pstRelationSvr->OpType = RELATION_OP_TYPE_DELRELATION_RES;
				pstRelationSvr->RelationSvrData.RelationDelRes.RoleID = pstOther->stRoleData.RoleID;
				pstRelationSvr->RelationSvrData.RelationDelRes.Type = RELATION_TYPE_FRIEND;
				
				Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
				z_cltmsg_send(pstEnv, pstPlayer, &stPkg);


				pstBuddyInfo = relation_buddy_db_find(pstBuddy, pstOther->stRoleData.RoleID);
				if (pstBuddyInfo)
				{
					pstBuddy->BuddyNum --;
					if (pstBuddyInfo != &pstBuddy->BuddyInfos[pstBuddy->BuddyNum])
					{
						memcpy(pstBuddyInfo, &pstBuddy->BuddyInfos[pstBuddy->BuddyNum], sizeof(*pstBuddyInfo));
					}

					pstPlayer->stOnline.bRelationUpdateFlag = 2;
				}
			}
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION17, pstBuddyMember->RoleName);
			return -1;
		}
	}

	pstMisc->BlackMember[pstMisc->BlackNum].RoleID = pstOther->stRoleData.RoleID;
	pstMisc->BlackMember[pstMisc->BlackNum].Head = pstOther->stRoleData.Head;
	pstMisc->BlackMember[pstMisc->BlackNum].Hair = pstOther->stRoleData.Hair;
	pstMisc->BlackMember[pstMisc->BlackNum].Career = pstOther->stRoleData.Career;
	pstMisc->BlackMember[pstMisc->BlackNum].Gender = pstOther->stRoleData.Gender;
	pstMisc->BlackMember[pstMisc->BlackNum].Face = pstOther->stRoleData.Face;
	STRNCPY(pstMisc->BlackMember[pstMisc->BlackNum].RoleName, pstOther->stRoleData.RoleName, 
				sizeof(pstMisc->BlackMember[pstMisc->BlackNum].RoleName));
	pstMisc->BlackNum ++;

	// 检查邮件
	//mail_check_clean_notify( pstEnv, pstPlayer, pstAddBlackReq->Name );
	
	if (pstPlayer->stOnline.bGetBuddyStat != BUDDY_GET_FINI)
	{
		return 0;
	}
	
	relation_chg_clt_detail(pstEnv, pstPlayer, pstOther, RELATION_TYPE_BLACK);	

	pstRoleBuddy2 = &pstOther->stRoleData.Buddy;
	iIdx2 = online_info_find_friend_byID(pstRoleBuddy2, pstPlayer->stRoleData.RoleID);
	if(iIdx2 >= 0)
	{
		//对方亲密度清0
		pstBuddyMember = &pstRoleBuddy2->Friends[iIdx2];
		pstBuddyMember->BuddyVal = 1;
		relation_buddyval_change(pstEnv, pstOther, pstBuddyMember, -1);
	}

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE, ZONEERR_RELATION18, pstPlayer->stRoleData.RoleName);
	return 0;
}


int player_relation_addtmpenemy_req(ZONESVRENV* pstEnv, Player *pstPlayer, const char* pszName)
{
	ROLEENEMYMEMBER *pstEnemyMember = NULL;
	ROLEENEMY *pstEnemy = &pstPlayer->stRoleData.MiscInfo.Enemy;
	ROLEBUDDYMEMBER *pstBuddyMember;
	Player *pstEnemyPlayer;

	pstEnemyPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pszName);
	if (!pstEnemyPlayer)
		return -1;
	
	pstEnemyMember = relation_enemy_online_find(pstEnemy , pstEnemyPlayer->stRoleData.RoleID);
	if(pstEnemyMember)
	{
		return -1;
	}

	if (pstEnemyPlayer->stRoleData.RoleID == pstPlayer->stRoleData.RoleID)
	{
		return -1;
	}

	pstBuddyMember = relation_buddy_online_find(&pstPlayer->stRoleData.Buddy,pstEnemyPlayer->stRoleData.RoleID);
	if (pstBuddyMember)
	{
		// 现在好友这些可以和凶手名单共存
		//return -1;
	}
	
	relation_chg_clt_detail(pstEnv, pstPlayer, pstEnemyPlayer, RELATION_TYPE_TMPENEMY);
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstEnemyPlayer, SYS_MIDDLE, ZONEERR_RELATION23, pstPlayer->stRoleData.RoleName);
	
	return 0;
}

// 加不在线宿敌  特殊处理
int player_add_enmey_outline(ZONESVRENV* pstEnv, Player *pstPlayer, ADDENEMYREQ *pstAddEnemyReq)
{
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONCHG *pstRelationChg = &pstRelationSvr->RelationSvrData.RelationChg;
	memset(pstRelationChg, 0, sizeof(*pstRelationChg));
	
	pstRelationSvr->OpType = RELATION_OP_TYPE_CHG;
	pstRelationChg->RelationAttrNum = 0;
	pstRelationChg->RelationType = RELATION_TYPE_ENEMY;
	pstRelationChg->RoleID = pstAddEnemyReq->RoleID;

	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_NAME;
	STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Name,
				pstAddEnemyReq->Name, 
				sizeof(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Name));
	pstRelationChg->RelationAttrNum ++;

	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_ONLINE;
	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.IsOnline = 0;
	pstRelationChg->RelationAttrNum ++;

	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;

}

// 加仇人名单       不需要在线 pstPlayer添加pstOther为仇人
int player_relation_addenemy_req(ZONESVRENV* pstEnv, Player *pstPlayer, ADDENEMYREQ *pstAddEnemyReq)
{
	ROLEENEMYMEMBER *pstEnemyMember = NULL;
	ROLEENEMY *pstEnemy = &pstPlayer->stRoleData.MiscInfo.Enemy;

	ROLEBUDDYMEMBER *pstBuddyMember;
	Player *pstEnemyPlayer;

	// 已经在仇人名单中
	pstEnemyMember = relation_enemy_online_find(pstEnemy , pstAddEnemyReq->RoleID);
	if(pstEnemyMember)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION24);
		return -1;
	}
	
	pstEnemyPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstAddEnemyReq->Name);
	//if (!pstEnemyPlayer)
		//return -1;

	// 加自己
	if (pstAddEnemyReq->RoleID == pstPlayer->stRoleData.RoleID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION19);
		return -1;
	}

	// 在好友列表
	pstBuddyMember = relation_buddy_online_find(&pstPlayer->stRoleData.Buddy, pstAddEnemyReq->RoleID);
	if (pstBuddyMember)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION20);
		return -1;
	}

	// 仇人名单已满
	if (pstEnemy->EnemyNum >= MAX_ENEMY)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION21);
		return -1;
	}

	pstEnemy->Enemys[pstEnemy->EnemyNum].RoleID = pstAddEnemyReq->RoleID;
	STRNCPY(pstEnemy->Enemys[pstEnemy->EnemyNum].RoleName, pstAddEnemyReq->Name, 
			sizeof(pstEnemy->Enemys[pstEnemy->EnemyNum].RoleName));
	pstEnemy->Enemys[pstEnemy->EnemyNum].GPRSEnd = 0;
	if (pstEnemyPlayer)
	{
		pstEnemy->Enemys[pstEnemy->EnemyNum].Career = pstEnemyPlayer->stRoleData.Career;
		pstEnemy->Enemys[pstEnemy->EnemyNum].Face = pstEnemyPlayer->stRoleData.Face;
		pstEnemy->Enemys[pstEnemy->EnemyNum].Gender = pstEnemyPlayer->stRoleData.Gender;
		pstEnemy->Enemys[pstEnemy->EnemyNum].Hair = pstEnemyPlayer->stRoleData.Hair;
		pstEnemy->Enemys[pstEnemy->EnemyNum].Head = pstEnemyPlayer->stRoleData.Head;

		relation_chg_clt_detail(pstEnv, pstPlayer, pstEnemyPlayer, RELATION_TYPE_ENEMY);	
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstEnemyPlayer, SYS_MIDDLE2, ZONEERR_RELATION25, pstPlayer->stRoleData.RoleName);
	}
	else
	{
		// 不在线  只给客户端名字 roleid  在不在线
		player_add_enmey_outline(pstEnv, pstPlayer, pstAddEnemyReq);

		/*char szMsg[64];
		snprintf(szMsg, sizeof(szMsg), "玩家 %s  把你加入宿敌名单", pstPlayer->stRoleData.RoleName);
		player_sysmail_fast_send(pstEnv,pstAddEnemyReq->Name,
								"系统提示",szMsg,NULL,0,0,NULL);*/

	}
	pstEnemy->EnemyNum ++;

	// 检查邮件
	//mail_check_clean_notify( pstEnv, pstPlayer, pstAddEnemyReq->Name );

	return 0;
}

//  查看装备请求
int player_relation_avatar_req(ZONESVRENV* pstEnv, Player *pstPlayer, CSAVATARREQ *pstAvatarReq)
{

	Player *pstOther = NULL;
	ROLEBUDDYMEMBER *pstMemBer = NULL;
	ROLEBUDDY *pstRoleBuddy = &pstPlayer->stRoleData.Buddy;

	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONAVATARINFO *pstAvatarInfo = &pstRelationSvr->RelationSvrData.RelationAvatarInfo; 
	
	pstOther = z_name_player(pstEnv->pstAppCtx, pstEnv, pstAvatarReq->Name);
	if (pstOther == NULL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION22, pstAvatarReq->Name);
		return -1;
	}

	if (pstOther == pstPlayer)
	{
		return -1;
	}

	pstMemBer = relation_buddy_online_find_by_name(pstRoleBuddy, pstAvatarReq->Name);
	if (pstMemBer == NULL)
	{
		return -1;
	}

	pstRelationSvr->OpType = RELATION_OP_TYPE_AVATAR_RES;
	pstAvatarInfo->RoleID = pstOther->stRoleData.RoleID;
	memcpy (&pstAvatarInfo->RoleAvatar, &pstOther->stRoleData.AvatarProf, sizeof(pstAvatarInfo->RoleAvatar));

	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

static int relation_proxy_request(ZONESVRENV* pstEnv, Player *pstPlayer,
                                  CSPKG *pstCsPkg)
{
	CSRELATIONCLT *pstRelationClt = &pstCsPkg->Body.RelationClt;
	CSADDRELATIONDATA *pstAddRelationData =
		&pstRelationClt->RelationCltData.AddRelationReq.AddRelationData;

	switch(pstRelationClt->OpType)
	{
	case RELATION_OP_TYPE_ADDRELATION_REQ:
		{
			char *pszRoleName = NULL;
			//Player *pstOther = NULL;

			switch(pstRelationClt->RelationCltData.AddRelationReq.Type)
			{
			case RELATION_TYPE_FRIEND:
				pszRoleName = pstAddRelationData->AddFriend.Name;
				break;

			case RELATION_TYPE_BLACK:
				pszRoleName = pstAddRelationData->AddBlack.Name;
				break;

			case RELATION_TYPE_ENEMY:
				pszRoleName = pstAddRelationData->AddEnemy.Name;
				break;

			case RELATION_TYPE_TMPFRIEND:
				pszRoleName = pstAddRelationData->AddTmpFriend.Name;
				break;

			case RELATION_TYPE_TMPENEMY:
				pszRoleName = pstAddRelationData->AddTmpEnemy.Name;
				break;

			default:
				return -1;
				break;
			}
			if(!pszRoleName)
			{
				return -1;
			}
			if(0 > player_check_same_world_name(pstPlayer->stRoleData.RoleName,pszRoleName))
			{
				if(pstRelationClt->RelationCltData.AddRelationReq.Type != RELATION_TYPE_TMPFRIEND)
				{
					if(pstRelationClt->RelationCltData.AddRelationReq.Type == RELATION_TYPE_FRIEND)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_RELATION27);
					}
					else if(pstRelationClt->RelationCltData.AddRelationReq.Type == RELATION_TYPE_BLACK)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_RELATION28);
					}
					
				}
				return -1;
			}
			/*if(pszRoleName)
			{
 				pstOther = z_name_player(pstEnv->pstAppCtx, pstEnv, pszRoleName);
			}
			if(pstOther &&
			   pstOther->stOnline.stPworldQueue.stBattleCampInfo.PworldCampID !=
			   pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.PworldCampID)
			{
				z_sys_strf(pstEnv, pstPlayer, SYS_TIP,
				           "你指定的角色与你非同一阵营,因此无法进行指定的操作");
				return -1;
			}*/
			
			battle_rolename_remove_prefix(pszRoleName);
		}
		break;

	case RELATION_OP_TYPE_ADDFRIEND_RES:
		battle_rolename_remove_prefix(
			pstRelationClt->RelationCltData.AddFriendRes.ReqName);
		break;

	case RELATION_OP_TYPE_MEMNAME_REQ:
		battle_rolename_remove_prefix(
			pstRelationClt->RelationCltData.MemNameReq.Name);
		break;

	case RELATION_OP_TYPE_AVATAR_REQ:
		battle_rolename_remove_prefix(
			pstRelationClt->RelationCltData.AvatarReq.Name);
		break;

	case RELATION_OP_TYPE_EMOTION_REQ:
	case RELATION_OP_TYPE_DELRELATION_REQ:
	case RELATION_OP_TYPE_INFO_REQ:
		break;
	case RELATION_OP_TYPE_MSG_GROUP:
		break;
	default:
		return -1;
		break;
	}

	return battle_proxy_request(pstEnv, pstPlayer, pstCsPkg);
}


static int player_relation_info_req(ZONESVRENV* pstEnv, Player *pstPlayer, const char * pszReqName)
{
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	ROLEBUDDYMEMBER *pstMemBer;
	ZoneTeam *pstTeam;
	
	Player *pstReqPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pszReqName);
	if (!pstReqPlayer)
	{
		return -1;
	}

	pstMemBer = player_find_friend_by_name( pstPlayer, pszReqName);
	if (!pstMemBer)
	{
		return -1;
	}

	memset(pstRelationSvr, 0, sizeof(*pstRelationSvr));
	pstRelationSvr->OpType = RELATION_OP_TYPE_INFO_RES;
	pstRelationSvr->RelationSvrData.BuddyInfoRes.Career = pstReqPlayer->stRoleData.Career;
	pstRelationSvr->RelationSvrData.BuddyInfoRes.Gender = pstReqPlayer->stRoleData.Gender;
	pstRelationSvr->RelationSvrData.BuddyInfoRes.Face = pstReqPlayer->stRoleData.Face;
	pstRelationSvr->RelationSvrData.BuddyInfoRes.Hair = pstReqPlayer->stRoleData.Hair;
	pstRelationSvr->RelationSvrData.BuddyInfoRes.Head = pstReqPlayer->stRoleData.Head;
	pstRelationSvr->RelationSvrData.BuddyInfoRes.Level = pstReqPlayer->stRoleData.Level;
	strncpy(	pstRelationSvr->RelationSvrData.BuddyInfoRes.RoleName,
			pszReqName, sizeof(pstRelationSvr->RelationSvrData.BuddyInfoRes.RoleName));
	if (pstReqPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id > 0)
	{
		STRNCPY(pstRelationSvr->RelationSvrData.BuddyInfoRes.ClanName, 
				  pstReqPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Name,
				  sizeof(pstRelationSvr->RelationSvrData.BuddyInfoRes.ClanName));
		pstRelationSvr->RelationSvrData.BuddyInfoRes.ClanWid = 
								pstReqPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID;
	}

	if (pstReqPlayer->stRoleData.Buddy.Emotion[0] != 0)
	{
		strcpy(pstRelationSvr->RelationSvrData.BuddyInfoRes.Emotion, pstReqPlayer->stRoleData.Buddy.Emotion);
	}

	pstRelationSvr->RelationSvrData.BuddyInfoRes.Map = GET_MAPID(pstReqPlayer->stRoleData.Map);
	pstRelationSvr->RelationSvrData.BuddyInfoRes.Pos = pstReqPlayer->stRoleData.Pos;
	pstRelationSvr->RelationSvrData.BuddyInfoRes.FriendVal = pstMemBer->BuddyVal;
	pstRelationSvr->RelationSvrData.BuddyInfoRes.Charm = pstReqPlayer->stRoleData.RoleDetail.WhiteJade;
	pstRelationSvr->RelationSvrData.BuddyInfoRes.Span = 0;
	
	if(pstReqPlayer->stOnline.stSpanOnline.iStat == SPAN_STAT_CONNECT)
	{
		pstRelationSvr->RelationSvrData.BuddyInfoRes.Span = 1;
	}

	pstTeam = player_team_get(pstEnv, pstReqPlayer);
	if (pstTeam)
	{
		pstRelationSvr->RelationSvrData.BuddyInfoRes.TeamRoleNum = pstTeam->stTeamInfo.MemberNum;
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_relation_set_group(ZONESVRENV* pstEnv, Player *pstPlayer,CSSETGROUPREQ *pstSet)
{
	ROLEBUDDYMEMBER *pstMemBer;
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONCHG *pstRelationChg = &pstRelationSvr->RelationSvrData.RelationChg;

	pstMemBer = player_find_friend_by_name( pstPlayer, pstSet->RoleName);
	if (!pstMemBer)
		return -1;

	if (pstSet->GroupID <= 0 || pstSet->GroupID > 5)
	{
		return -1;
	}

	pstMemBer->GroupID = pstSet->GroupID;

	memset(pstRelationChg, 0, sizeof(*pstRelationChg));	
	pstRelationSvr->OpType = RELATION_OP_TYPE_CHG;
	pstRelationChg->RelationAttrNum = 0;
	pstRelationChg->RelationType = RELATION_TYPE_FRIEND;
	pstRelationChg->RoleID = pstMemBer->RoleID;

	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_NAME;
	STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Name,
				pstMemBer->RoleName, 
				sizeof(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Name));
	pstRelationChg->RelationAttrNum ++;

	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_GROUP;
	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.GroupID = pstSet->GroupID;
	pstRelationChg->RelationAttrNum ++;

	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_relation_max_chg(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONMAXRES *pstBuddyMax = &pstRelationSvr->RelationSvrData.BuddyMax;

	if (pstPlayer->stRoleData.Buddy.FirendMax < INIT_MAX_BUDDY_FRIEND)
		pstPlayer->stRoleData.Buddy.FirendMax = INIT_MAX_BUDDY_FRIEND;
	if (pstPlayer->stRoleData.Buddy.FirendMax > MAX_BUDDY_FRIEND)
		pstPlayer->stRoleData.Buddy.FirendMax = MAX_BUDDY_FRIEND; 

	#ifdef CESHI_
		pstPlayer->stRoleData.Buddy.FirendMax = 20;
	#endif
	
	pstRelationSvr->OpType = RELATION_OP_TYPE_MAX_FRIEND;
	pstBuddyMax->FriendNum = pstPlayer->stRoleData.Buddy.FirendMax;

	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


// 好友消息群发
int relation_friend_msg_group(ZONESVRENV* pstEnv, Player *pstPlayer,FRIENDMSGGROUP *pstMsgGroup)
{
	int i,j;
	int iPlayer = 0;
	Player *apstFriend[MAX_BUDDY_FRIEND];
	ROLEBUDDY *pstBuddy = &pstPlayer->stRoleData.Buddy;
	Player *pstTmp;
	CSCHATREQ *pstChat = &pstMsgGroup->ChatReq;
	CSPKG *pstResPkg = &pstEnv->stCSRes;
	CSCHATRES *pstChatRes = &pstResPkg->Body.ChatRes;
	int iVip = 0;
	
	
	if (pstMsgGroup->iGroup <= 0)
		return -1;

	for (i=0; i<pstBuddy->FriendNum; i++)
	{
		for (j=0; j<pstMsgGroup->iGroup; j++)
		{
			if (pstMsgGroup->Groups[j] == pstBuddy->Friends[i].GroupID)
			{
				break;
			}
		}

		if (j<pstMsgGroup->iGroup)
		{
			pstTmp = z_name_player(pstEnv->pstAppCtx, pstEnv, pstBuddy->Friends[i].RoleName);
			if (pstTmp)
			{
				apstFriend[iPlayer++] = pstTmp;
			}
		}
	}

	if(player_silence_check(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}

	if (z_check_chat(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstChat->Content, pstChat->Type) < 0)
	{
		return -1;
	}

	iVip |= z_player_check_vip(pstEnv,pstPlayer);

	// to zone_svr
	if (iPlayer > 0)	
	{
		memset(pstChatRes, 0, sizeof(*pstChatRes));

		pstChatRes->Vip = iVip;
		pstChatRes->IsCluster = 1;
		pstChatRes->Type = pstChat->Type;
		STRNCPY(pstChatRes->From, pstPlayer->stRoleData.RoleName, sizeof(pstChatRes->From));
		STRNCPY(pstChatRes->Content, pstChat->Content, sizeof(pstChatRes->Content));

		if (pstChat->LinkLen > 0)
		{
			pstChatRes->LinkLen = pstChat->LinkLen;
			memcpy(pstChatRes->LinkContent, pstChat->LinkContent, pstChat->LinkLen);
		}
			
		pstChatRes->OnlineType = ROLE_GAME_ONLINE;
		
		Z_CSHEAD_INIT(&pstResPkg->Head, CHAT_RES);
		z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv,apstFriend, iPlayer, pstResPkg);
	}

	// to mud_svr
	if (pstMsgGroup->iMud > 0)
	{
		pstChat->Vip = iVip;
		pstChat->IsCluster = 1;
		pstChat->OnlineType = ROLE_GAME_ONLINE;
	
		for (i=0; i<pstBuddy->FriendNum; i++)
		{
			for (j=0; j<pstMsgGroup->iMud; j++)
			{
				if (pstBuddy->Friends[i].RoleID == pstMsgGroup->Muds[j])
				{
					STRNCPY(pstChat->ToName, pstBuddy->Friends[i].RoleName, sizeof(pstChat->ToName));
					player_chat_to_mud(pstEnv, NULL,pstChat, pstPlayer->stRoleData.RoleName);
					continue;
				}
			}
		}
	}
	
	return 0;
}

int player_relation(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                    TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSRELATIONCLT *pstRelationClt = &pstCsPkg->Body.RelationClt;
	CSADDRELATIONDATA *pstAddRelationData = &pstRelationClt->RelationCltData.AddRelationReq.AddRelationData;

	if(pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		return relation_proxy_request(pstEnv, pstPlayer, pstCsPkg);
	}

	switch(pstRelationClt->OpType)
	{
	case RELATION_OP_TYPE_ADDRELATION_REQ:
		if (pstRelationClt->RelationCltData.AddRelationReq.Type == RELATION_TYPE_FRIEND)
		{
			if(0>player_check_same_world(pstEnv,pstAddRelationData->AddFriend.Name))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION26,
					pstAddRelationData->AddFriend.Name);
			}
			else
			{
				battle_rolename_remove_prefix(pstAddRelationData->AddFriend.Name);
				player_relation_addfriend_req(pstEnv, pstPlayer, &pstAddRelationData->AddFriend);
			}
		}
		else if(pstRelationClt->RelationCltData.AddRelationReq.Type == RELATION_TYPE_BLACK)
		{
			if(0>player_check_same_world(pstEnv,pstAddRelationData->AddBlack.Name))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION26,
					pstAddRelationData->AddBlack.Name);
			}
			else
			{
				battle_rolename_remove_prefix(pstAddRelationData->AddBlack.Name);
				player_relation_addblack_req(pstEnv, pstPlayer, &pstAddRelationData->AddBlack);
			}
		}
		else if (pstRelationClt->RelationCltData.AddRelationReq.Type == RELATION_TYPE_ENEMY)
		{
			if(0>player_check_same_world(pstEnv,pstAddRelationData->AddEnemy.Name))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION26,
					pstAddRelationData->AddEnemy.Name);
			}
			else
			{
				battle_rolename_remove_prefix(pstAddRelationData->AddEnemy.Name);
				player_relation_addenemy_req(pstEnv, pstPlayer, &pstAddRelationData->AddEnemy);
			}
		}
		else if (pstRelationClt->RelationCltData.AddRelationReq.Type == RELATION_TYPE_TMPFRIEND)
		{
			if(0>player_check_same_world(pstEnv,pstAddRelationData->AddTmpFriend.Name))
			{
				//z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "玩家 %s 不在相同的服!",
				//	pstAddRelationData->AddTmpFriend.Name);
			}
			else
			{
				battle_rolename_remove_prefix(pstAddRelationData->AddTmpFriend.Name);
				player_relation_addTmpfriend_req(pstEnv, pstPlayer, pstAddRelationData->AddTmpFriend.Name);
			}
		}
		else if (pstRelationClt->RelationCltData.AddRelationReq.Type == RELATION_TYPE_TMPENEMY)
		{
			//player_relation_addtmpenemy_req(pstEnv, pstPlayer, pstAddRelationData->AddTmpEnemy.Name);
		}
		
		break;
	case RELATION_OP_TYPE_ADDFRIEND_RES:
		if(0>player_check_same_world(pstEnv,pstRelationClt->RelationCltData.AddFriendRes.ReqName))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION26,
				pstRelationClt->RelationCltData.AddFriendRes.ReqName);
		}
		else
		{
			battle_rolename_remove_prefix(pstRelationClt->RelationCltData.AddFriendRes.ReqName);
			player_relation_addfriend_res(pstEnv, pstPlayer, &pstRelationClt->RelationCltData.AddFriendRes);
		}
		break;
	case RELATION_OP_TYPE_EMOTION_REQ:
		player_relation_emotion(pstEnv, pstPlayer, &pstRelationClt->RelationCltData.EmotionReq);
		break;
	case RELATION_OP_TYPE_MEMNAME_REQ:
		player_relation_memname_req(pstEnv, pstPlayer, &pstRelationClt->RelationCltData.MemNameReq);
		break;
	case RELATION_OP_TYPE_DELRELATION_REQ:
		player_relation_delrelation_req(pstEnv, pstPlayer, &pstRelationClt->RelationCltData.DelRelationReq);
		break;
	case RELATION_OP_TYPE_AVATAR_REQ:
		if(0>player_check_same_world(pstEnv,pstRelationClt->RelationCltData.AvatarReq.Name))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION26,
				pstRelationClt->RelationCltData.AvatarReq.Name);
		}
		else
		{
			battle_rolename_remove_prefix(pstRelationClt->RelationCltData.AvatarReq.Name);
			player_relation_avatar_req(pstEnv, pstPlayer, &pstRelationClt->RelationCltData.AvatarReq);
		}
		break;
	case RELATION_OP_TYPE_INFO_REQ:
		if(0>player_check_same_world(pstEnv,pstRelationClt->RelationCltData.BuddyInfoReq.ReqName))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_RELATION26,
				pstRelationClt->RelationCltData.BuddyInfoReq.ReqName);
		}
		else
		{
			battle_rolename_remove_prefix(pstRelationClt->RelationCltData.BuddyInfoReq.ReqName);
			player_relation_info_req(pstEnv, pstPlayer, pstRelationClt->RelationCltData.BuddyInfoReq.ReqName);
		}
		break;
	case RELATION_OP_TYPE_SET_GROUP:
		player_relation_set_group(pstEnv, pstPlayer,&pstRelationClt->RelationCltData.SetGroupReq);
		break;
	case RELATION_OP_TYPE_MSG_GROUP:
		 relation_friend_msg_group(pstEnv, pstPlayer,&pstRelationClt->RelationCltData.FriendMsgGroup);
		break;
	case RELATION_OP_TYPE_INVOLVE_MARRY_CLT:
		if (pstRelationClt->RelationCltData.InvolveMarryClt.Type == TYPE_PURSUE_RES_CLT)
		{
			relation_pursue_res_repose(pstEnv, pstPlayer,&pstRelationClt->RelationCltData.InvolveMarryClt.InvolveMarryCltData.PursueResClt);
		}
		else if(pstRelationClt->RelationCltData.InvolveMarryClt.Type == TYPE_REMOVE_RELATION_REQ_CLT)
		{
			relation_remove_relation_req_repose(pstEnv, pstPlayer,&pstRelationClt->RelationCltData.InvolveMarryClt.InvolveMarryCltData.RemoveRelationReqClt);
		}
		else if(pstRelationClt->RelationCltData.InvolveMarryClt.Type == TYPE_OATH_RES_CLT)
		{
			relation_oath_res_repose(pstEnv, pstPlayer,&pstRelationClt->RelationCltData.InvolveMarryClt.InvolveMarryCltData.OathResClt);
		}
		break;
	default:
		return -1;
		break;
	}
		
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstFrameHead);	//add by paraunused.pl
}


int relation_role_login(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSONLINESVR *pstOnline = &stPkg.Body.OnlineInfo;
		
	//上线通知
	pstOnline->RoleID = pstPlayer->stRoleData.RoleID;
	pstOnline->Online = 1;
	STRNCPY(pstOnline->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstOnline->RoleName));
	Z_CSHEAD_INIT(&stPkg.Head, ONLINE_INFO);
	z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);

	pstPlayer->stOnline.stRelation.GID = pstPlayer->stRoleData.RoleID;

	return 0;
}

int relation_role_logout(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSONLINESVR *pstOnline = &stPkg.Body.OnlineInfo;
	
	//下线通知
	pstOnline->RoleID = pstPlayer->stRoleData.RoleID;
	pstOnline->Online = 0;
	STRNCPY(pstOnline->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstOnline->RoleName));
	Z_CSHEAD_INIT(&stPkg.Head, ONLINE_INFO);
	z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);

	if (pstPlayer->stOnline.bRelationUpdateFlag)
	{
		relation_db_update(pstEnv,  &pstPlayer->stOnline.stRelation);
	}
	
	return 0;
}

inline int relation_buddy_have(Player *pstPlayer)
{
	ROLEBUDDY *pstRoleBuddy = &pstPlayer->stRoleData.Buddy;

	return (int)pstRoleBuddy->FriendNum+pstRoleBuddy->PrenticeNum+pstRoleBuddy->MasterNum
			+/*pstRoleBuddy->SpouseNum+*/pstRoleBuddy->BrotherNum+pstPlayer->stRoleData.MiscInfo.BlackNum
			+pstPlayer->stRoleData.MiscInfo.Enemy.EnemyNum;
}

int relation_db_get(ZONESVRENV* pstEnv, tdr_ulonglong ullRoleID, int iUin, DBAsyn *pstDBAsyn)
{
	RELATION stRelation;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
		
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, "Relation", iUin, pstDBAsyn);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 0;

	stRelation.GID = ullRoleID;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRelation;
	stMetaData.iBuff = sizeof(stRelation);
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

int relation_db_update(ZONESVRENV* pstEnv, RELATION *pstRelation)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
		
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, "Relation", -1, NULL);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 1;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pstRelation;
	stMetaData.iBuff = sizeof(*pstRelation);
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

int relation_self_emotion_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSEMOTIONRES *pstEmotionRes = &pstRelationSvr->RelationSvrData.EmotionRes;
	
	pstRelationSvr->OpType = RELATION_OP_TYPE_EMOTION_RES;
	memcpy(pstEmotionRes->Emotion, pstPlayer->stRoleData.Buddy.Emotion, sizeof(pstEmotionRes->Emotion));
	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int relation_online_claninfo_list(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	RELATIONCLANINFOLIST *pstRelationClanInfoList = &pstRelationSvr->RelationSvrData.RelationClanInfoList;
	//ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	ROLEBUDDY *pstBuddy = &pstPlayer->stRoleData.Buddy;
	
	
	for (i=0; i<pstBuddy->FriendNum; i++)
	{
		
		
	}
	
	pstRelationSvr->OpType = RELATION_OP_TYPE_CHGLIST_CLAN;
	pstRelationClanInfoList->Num = 0;
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int relation_online_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONCHGLIST *pstRelationChgList = &pstRelationSvr->RelationSvrData.RelationChgList;
	CSRELATIONCHG *pstRelationChg;
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	ROLEBUDDY *pstBuddy = &pstPlayer->stRoleData.Buddy;
	Player *pstOther;
	SSPKG stSSPkg;
	SSRELATIONONLINEREQ *pstRelationReq = &stSSPkg.Body.RelationOnlineReq;

	int iIsOnline;
	//通知一部分个人关系信息

	pstRelationReq->BuddyNum = 0;
	STRNCPY(pstRelationReq->RoleName, pstPlayer->stRoleData.RoleName, 
					sizeof(pstRelationReq->RoleName));
	
	pstRelationSvr->OpType = RELATION_OP_TYPE_CHGLIST;
	pstRelationChgList->Num = 0;

	for (i=0; i<pstMiscInfo->BlackNum; i++)
	{
		pstRelationChg = &pstRelationChgList->ChgList[pstRelationChgList->Num++];
		pstRelationChg->RoleID = pstMiscInfo->BlackMember[i].RoleID;
		pstRelationChg->RelationType = RELATION_TYPE_BLACK;
		pstRelationChg->RelationAttrNum = 2;
		pstRelationChg->RelationAttrs[0].RelationAttrID = RELATION_ATTR_NAME;
		STRNCPY(pstRelationChg->RelationAttrs[0].RelationAttrData.Name, pstMiscInfo->BlackMember[i].RoleName, 
					sizeof(pstRelationChg->RelationAttrs[0].RelationAttrData.Name));

		pstOther = z_name_player(pstEnv->pstAppCtx, pstEnv, pstMiscInfo->BlackMember[i].RoleName);
		if (pstOther)
			iIsOnline = 1;
		else
		{
			iIsOnline = 0;
			
			pstRelationReq->BuddyInfo[pstRelationReq->BuddyNum].RoleID = pstMiscInfo->BlackMember[i].RoleID;
			STRNCPY(pstRelationReq->BuddyInfo[pstRelationReq->BuddyNum].RoleName, 
						 pstMiscInfo->BlackMember[i].RoleName, sizeof(pstRelationReq->BuddyInfo[0].RoleName));
			pstRelationReq->BuddyInfo[pstRelationReq->BuddyNum].RelationType = RELATION_TYPE_BLACK;
			pstRelationReq->BuddyNum++;
		}
			
		pstRelationChg->RelationAttrs[1].RelationAttrID = RELATION_ATTR_ONLINE;
		pstRelationChg->RelationAttrs[1].RelationAttrData.IsOnline = iIsOnline;

	/*	pstRelationChg->RelationAttrs[2].RelationAttrID = RELATION_ATTR_HEAD;
		pstRelationChg->RelationAttrs[2].RelationAttrData.Head |= (pstMiscInfo->BlackMember[i].Head<<16);
		pstRelationChg->RelationAttrs[2].RelationAttrData.Head |= (pstMiscInfo->BlackMember[i].Hair<<8);
		pstRelationChg->RelationAttrs[2].RelationAttrData.Head |= pstMiscInfo->BlackMember[i].Face;

		pstRelationChg->RelationAttrs[3].RelationAttrID = RELATION_ATTR_CAREER;
		pstRelationChg->RelationAttrs[3].RelationAttrData.IsOnline = pstMiscInfo->BlackMember[i].Career;

		pstRelationChg->RelationAttrs[4].RelationAttrID = RELATION_ATTR_GENDER;
		pstRelationChg->RelationAttrs[4].RelationAttrData.IsOnline = pstMiscInfo->BlackMember[i].Gender;*/
		
				
	}

	for (i=0; i<pstBuddy->FriendNum; i++)
	{
		pstRelationChg = &pstRelationChgList->ChgList[pstRelationChgList->Num++];
		pstRelationChg->RoleID = pstBuddy->Friends[i].RoleID;
		pstRelationChg->RelationType = pstBuddy->Friends[i].RelationType;
		
		pstRelationChg->RelationAttrNum = 0;
		
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_NAME;
		STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Name, 
					pstBuddy->Friends[i].RoleName, sizeof(pstBuddy->Friends[i].RoleName));
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_BUDDYVAL;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.BuddyVal = pstBuddy->Friends[i].BuddyVal;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_GENDER;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Gender = pstBuddy->Friends[i].Gender;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_MEMNAME;
		STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.MemName, 
					pstBuddy->Friends[i].MemName, sizeof(pstBuddy->Friends[i].MemName));
		pstRelationChg->RelationAttrNum ++;

		pstOther = z_name_player(pstEnv->pstAppCtx, pstEnv, pstBuddy->Friends[i].RoleName);
		if (pstOther)
		{
			iIsOnline = 1;
		}
		else
		{
			iIsOnline = 0;

			pstRelationReq->BuddyInfo[pstRelationReq->BuddyNum].RoleID = pstBuddy->Friends[i].RoleID;
			STRNCPY(pstRelationReq->BuddyInfo[pstRelationReq->BuddyNum].RoleName, 
						pstBuddy->Friends[i].RoleName, sizeof(pstRelationReq->BuddyInfo[0].RoleName));

			pstRelationReq->BuddyInfo[pstRelationReq->BuddyNum].RelationType = pstBuddy->Friends[i].RelationType;
			
			pstRelationReq->BuddyNum++;
		}

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_ONLINE;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.IsOnline = iIsOnline;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_GROUP;
		if (pstBuddy->Friends[i].GroupID <= 0)
		{
			pstBuddy->Friends[i].GroupID = 1;
		}
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.GroupID = pstBuddy->Friends[i].GroupID;
		pstRelationChg->RelationAttrNum ++;
		
	}

	for (i=0; i<pstBuddy->BrotherNum; i++)
	{
		pstRelationChg = &pstRelationChgList->ChgList[pstRelationChgList->Num++];
		pstRelationChg->RoleID = pstBuddy->Brothers[i].RoleID;
		pstRelationChg->RelationType = pstBuddy->Brothers[i].RelationType;
		pstRelationChg->RelationAttrNum = 0;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_NAME;
		STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Name, 
					pstBuddy->Brothers[i].RoleName, sizeof(pstBuddy->Brothers[i].RoleName));
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_BUDDYVAL;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.BuddyVal = pstBuddy->Brothers[i].BuddyVal;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_GENDER;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Gender = pstBuddy->Brothers[i].Gender;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_MEMNAME;
		STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.MemName, 
					pstBuddy->Brothers[i].MemName, sizeof(pstBuddy->Brothers[i].MemName));
		pstRelationChg->RelationAttrNum ++;

		pstOther = z_name_player(pstEnv->pstAppCtx, pstEnv, pstBuddy->Brothers[i].RoleName);
		if (pstOther)
		{
			iIsOnline = 1;
		}
		else
		{
			iIsOnline = 0;
		}

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_ONLINE;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.IsOnline = iIsOnline;
				
		pstRelationChg->RelationAttrNum ++;
	}

	for (i=0; i<pstBuddy->MasterNum; i++)
	{
		pstRelationChg = &pstRelationChgList->ChgList[pstRelationChgList->Num++];
		pstRelationChg->RoleID = pstBuddy->Master[i].RoleID;
		pstRelationChg->RelationType = pstBuddy->Master[i].RelationType;
		pstRelationChg->RelationAttrNum = 0;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_NAME;
		STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Name, 
					pstBuddy->Master[i].RoleName, sizeof(pstBuddy->Master[i].RoleName));
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_BUDDYVAL;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.BuddyVal = pstBuddy->Master[i].BuddyVal;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_GENDER;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Gender = pstBuddy->Master[i].Gender;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_MEMNAME;
		STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.MemName, 
					pstBuddy->Master[i].MemName, sizeof(pstBuddy->Master[i].MemName));
		pstRelationChg->RelationAttrNum ++;

		pstOther = z_name_player(pstEnv->pstAppCtx, pstEnv, pstBuddy->Master[i].RoleName);
		if (pstOther)
		{
			iIsOnline = 1;
		}
		else
		{
			iIsOnline = 0;
		}
		
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_ONLINE;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.IsOnline = iIsOnline;
				
		pstRelationChg->RelationAttrNum ++;
	}

	for (i=0; i<pstBuddy->PrenticeNum; i++)
	{
		pstRelationChg = &pstRelationChgList->ChgList[pstRelationChgList->Num++];
		pstRelationChg->RoleID = pstBuddy->Prentices[i].RoleID;
		pstRelationChg->RelationType = pstBuddy->Prentices[i].RelationType;
		pstRelationChg->RelationAttrNum = 0;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_NAME;
		STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Name, 
					pstBuddy->Prentices[i].RoleName, sizeof(pstBuddy->Prentices[i].RoleName));
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_BUDDYVAL;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.BuddyVal = pstBuddy->Prentices[i].BuddyVal;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_GENDER;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Gender = pstBuddy->Prentices[i].Gender;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_MEMNAME;
		STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.MemName, 
					pstBuddy->Prentices[i].MemName, sizeof(pstBuddy->Prentices[i].MemName));
		pstRelationChg->RelationAttrNum ++;

		pstOther = z_name_player(pstEnv->pstAppCtx, pstEnv, pstBuddy->Prentices[i].RoleName);
		if (pstOther)
		{
			iIsOnline = 1;
		}
		else
		{
			iIsOnline = 0;
		}

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_ONLINE;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.IsOnline = iIsOnline;
		pstRelationChg->RelationAttrNum ++;
	}

/*
	for (i=0; i<pstBuddy->SpouseNum; i++)
	{
		pstRelationChg = &pstRelationChgList->ChgList[pstRelationChgList->Num++];
		pstRelationChg->RoleID = pstBuddy->Spouse[i].RoleID;
		pstRelationChg->RelationType = pstBuddy->Spouse[i].RelationType;
		pstRelationChg->RelationAttrNum = 0;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_NAME;
		STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Name, 
					pstBuddy->Spouse[i].RoleName, sizeof(pstBuddy->Spouse[i].RoleName));
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_BUDDYVAL;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.BuddyVal = pstBuddy->Spouse[i].BuddyVal;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_GENDER;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Gender = pstBuddy->Spouse[i].Gender;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_MEMNAME;
		STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.MemName, 
					pstBuddy->Spouse[i].MemName, sizeof(pstBuddy->Spouse[i].MemName));
		pstRelationChg->RelationAttrNum ++;

		pstOther = z_name_player(pstEnv->pstAppCtx, pstEnv, pstBuddy->Spouse[i].RoleName);
		if (pstOther)
		{
			iIsOnline = 1;
		}
		else
		{
			iIsOnline = 0;
		}

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_ONLINE;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.IsOnline = iIsOnline;
		pstRelationChg->RelationAttrNum ++;
	}
*/
	for (i=0; i<pstMiscInfo->Enemy.EnemyNum; i++)
	{
		pstRelationChg = &pstRelationChgList->ChgList[pstRelationChgList->Num++];
		pstRelationChg->RoleID = pstMiscInfo->Enemy.Enemys[i].RoleID;
		pstRelationChg->RelationAttrNum = 3;
		pstRelationChg->RelationType = RELATION_TYPE_ENEMY;

		pstRelationChg->RelationAttrs[0].RelationAttrID = RELATION_ATTR_NAME;
		STRNCPY(pstRelationChg->RelationAttrs[0].RelationAttrData.Name, 
					pstMiscInfo->Enemy.Enemys[i].RoleName, sizeof(pstMiscInfo->Enemy.Enemys[i].RoleName));

		pstRelationChg->RelationAttrs[1].RelationAttrID = RELATION_ATTR_ONLINE;
		pstRelationChg->RelationAttrs[1].RelationAttrData.IsOnline = 
				z_name_player(pstEnv->pstAppCtx, pstEnv, pstMiscInfo->Enemy.Enemys[i].RoleName)?1:0;

		pstRelationChg->RelationAttrs[2].RelationAttrID = RELATION_ATTR_GPRSEND;
		pstRelationChg->RelationAttrs[2].RelationAttrData.GprsEnd = pstMiscInfo->Enemy.Enemys[i].GPRSEnd;

	/*	pstRelationChg->RelationAttrs[3].RelationAttrID = RELATION_ATTR_CAREER;
		pstRelationChg->RelationAttrs[3].RelationAttrData.IsOnline = pstMiscInfo->Enemy.Enemys[i].Career;

		pstRelationChg->RelationAttrs[4].RelationAttrID = RELATION_ATTR_GENDER;
		pstRelationChg->RelationAttrs[4].RelationAttrData.IsOnline = pstMiscInfo->Enemy.Enemys[i].Gender;*/
		
	}

	for (i = 0; i < pstRelationChgList->Num; i ++)
	{
		pstRelationChg = &pstRelationChgList->ChgList[i];
		if (pstRelationChg->RelationAttrNum > RELATION_ATTR_MAX)
		{
			pstRelationChg->RelationAttrNum = RELATION_ATTR_MAX;
		}
	}

	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	if(pstRelationReq->BuddyNum)
	{
		Z_SSHEAD_INIT(&stSSPkg.Head, RELATION_ONLINE_REQ);
		z_ssmsg_send(pstEnv, &stSSPkg, pstEnv->iMudSvrID);
	}

	//通知玩家好友列表中情侣或者夫妻标记
	relation_involve_marry_svr(pstEnv,pstPlayer,NULL,TYPE_LOGIN_INTIMATE_COLOR);
	
	return 0;
}

int relation_player_tick(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	DBAsyn stDBAsyn;
	
	if (BUDDY_GET_INIT == pstPlayer->stOnline.bGetBuddyStat)
	{
		if (relation_buddy_have(pstPlayer))
		{
			//避免登录时下行包量过大
			//if (pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.tRoleLogin > 6)
			{
				relation_online_info(pstEnv, pstPlayer);				
				if (pstPlayer->stRoleData.Buddy.Emotion[0] != 0)
				{
					relation_self_emotion_to_clt(pstEnv, pstPlayer);
				}
				
				stDBAsyn.iCmd = DB_BUDDY_ASYN_INIT;
				if (0 == relation_db_get(pstEnv, pstPlayer->stRoleData.RoleID, pstPlayer->stRoleData.Uin, &stDBAsyn))
				{
					pstPlayer->stOnline.bGetBuddyStat = BUDDY_GET_GETTING;
					pstPlayer->stOnline.tRelationInitReq = pstEnv->pstAppCtx->stCurr.tv_sec;
				}
			}
		}
		else
		{
			pstPlayer->stOnline.bGetBuddyStat = BUDDY_GET_FINI;
			if (pstPlayer->stRoleData.Buddy.Emotion[0] != 0)
			{
				relation_self_emotion_to_clt(pstEnv, pstPlayer);
			}
		}

		// 写在这里会造成这个消息发几次
		/*if (pstPlayer->stRoleData.Buddy.Emotion[0] != 0)
		{
			relation_self_emotion_to_clt(pstEnv, pstPlayer);
		}*/
		
		return 0;
	}

	if (BUDDY_GET_GETTING == pstPlayer->stOnline.bGetBuddyStat)
	{
		if (pstPlayer->stOnline.bDbBuddyRetry > 5)
		{
			pstPlayer->stOnline.bGetBuddyStat = BUDDY_GET_FAIL;
			tlog_error(pstEnv->pstLogCat, 0, 0, "player %d get buddy fail", pstPlayer->stRoleData.Uin);
			return 0;
		}
		
		if (pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.tRelationInitReq > 5)
		{
			stDBAsyn.iCmd = DB_BUDDY_ASYN_INIT;
			relation_db_get(pstEnv, pstPlayer->stRoleData.RoleID, pstPlayer->stRoleData.Uin,  &stDBAsyn);
			pstPlayer->stOnline.tRelationInitReq = pstEnv->pstAppCtx->stCurr.tv_sec;
			pstPlayer->stOnline.bDbBuddyRetry++;
		}

		return 0;
	}

	if (pstPlayer->stOnline.bRelationUpdateFlag == 2) //立即更新
	{
		pstPlayer->stOnline.bRelationUpdateFlag = 1;

		relation_db_update(pstEnv, &pstPlayer->stOnline.stRelation);
	}

	return 0;
}

BUDDYINFO * relation_buddy_db_find(BUDDY *pstBuddy, tdr_ulonglong ullRoleID)
{
	int i;
	
	for (i=0; i<pstBuddy->BuddyNum; i++)
	{
		if (pstBuddy->BuddyInfos[i].RoleID == ullRoleID)
		{
			return &pstBuddy->BuddyInfos[i];
		}
	}

	return NULL;
}

// 查找仇人
ROLEENEMYMEMBER * relation_enemy_online_find(ROLEENEMY *pstEnemy , tdr_ulonglong ullRoleID)
{
	int i;
	
	for (i=0; i<pstEnemy->EnemyNum; i++)
	{
		if (ullRoleID == pstEnemy->Enemys[i].RoleID)
		{
			return &pstEnemy->Enemys[i];
		}	
	}

	return NULL;
}

ROLEENEMYMEMBER * relation_enemy_online_find_byname(ROLEENEMY *pstEnemy ,char* pszName)
{
	int i;
	
	for (i=0; i<pstEnemy->EnemyNum; i++)
	{
		if (0 == strcmp(pstEnemy->Enemys[i].RoleName,pszName))
		{
			return &pstEnemy->Enemys[i];
		}	
	}

	return NULL;
}

// 根据RoleID 查找同伴
ROLEBUDDYMEMBER * relation_buddy_online_find(ROLEBUDDY *pstRoleBuddy , tdr_ulonglong ullRoleID)
{
	int i;
	
	for (i=0; i<pstRoleBuddy->FriendNum; i++)
	{
		if (pstRoleBuddy->Friends[i].RoleID == ullRoleID)
		{
			return &pstRoleBuddy->Friends[i];
		}
	}

	for (i=0; i<pstRoleBuddy->PrenticeNum; i++)
	{
		if (pstRoleBuddy->Prentices[i].RoleID == ullRoleID)
		{
			return &pstRoleBuddy->Prentices[i];
		}
	}

	for (i=0; i<pstRoleBuddy->BrotherNum; i++)
	{
		if (pstRoleBuddy->Brothers[i].RoleID == ullRoleID)
		{
			return &pstRoleBuddy->Brothers[i];
		}
	}

	for (i=0; i<pstRoleBuddy->MasterNum; i++)
	{
		if (pstRoleBuddy->Master[i].RoleID == ullRoleID)
		{
			return &pstRoleBuddy->Master[i];
		}
	}
/*
	for (i=0; i<pstRoleBuddy->SpouseNum; i++)
	{
		if (pstRoleBuddy->Spouse[i].RoleID == ullRoleID)
		{
			return &pstRoleBuddy->Spouse[i];
		}
	}
*/
	return NULL;
}

int relation_check_one(Player *pstPlayer, BUDDYINFO *pstBuddyInfo)
{
	int iUpdate = 0;

	// RoleID 应该是可以不用写
	/*if (pstBuddyInfo->RoleID != pstPlayer->stRoleData.RoleID)
	{
		pstBuddyInfo->RoleID = pstPlayer->stRoleData.RoleID;
		iUpdate = 1;
	}*/

	if (pstBuddyInfo->Career != pstPlayer->stRoleData.Career)
	{
		pstBuddyInfo->Career = pstPlayer->stRoleData.Career;
		iUpdate = 1;
	}

	if (pstBuddyInfo->Head != pstPlayer->stRoleData.Head)
	{
		pstBuddyInfo->Head = pstPlayer->stRoleData.Head;
		iUpdate = 1;
	}

	if (pstBuddyInfo->Level != pstPlayer->stRoleData.Level)
	{
		pstBuddyInfo->Level = pstPlayer->stRoleData.Level;
		iUpdate = 1;
	}

	if (pstBuddyInfo->Hair!= pstPlayer->stRoleData.Hair)
	{
		pstBuddyInfo->Hair = pstPlayer->stRoleData.Hair;
		iUpdate = 1;
	}

	if (pstBuddyInfo->Face != pstPlayer->stRoleData.Face)
	{
		pstBuddyInfo->Face = pstPlayer->stRoleData.Face;
		iUpdate = 1;
	}

	if (strcmp(pstBuddyInfo->Emotion, pstPlayer->stRoleData.Buddy.Emotion))
	{
		STRNCPY(pstBuddyInfo->Emotion, pstPlayer->stRoleData.Buddy.Emotion,sizeof(pstBuddyInfo->Emotion));
		iUpdate = 1;
	}

	if (pstBuddyInfo->ClanID != pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id)
	{
		pstBuddyInfo->ClanID = pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id;
		STRNCPY(pstBuddyInfo->ClanName, pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Name,sizeof(pstBuddyInfo->ClanName));
		iUpdate = 1;
	}

	return iUpdate;
}

int relation_dbbuddy_chg_clt(ZONESVRENV* pstEnv, Player *pstPlayer, BUDDYINFO *pstBuddyInfo, int iType)
{
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONCHG *pstRelationChg = &pstRelationSvr->RelationSvrData.RelationChg;
	
	pstRelationSvr->OpType = RELATION_OP_TYPE_CHG;
	pstRelationChg->RoleID = pstBuddyInfo->RoleID;

	pstRelationChg->RelationAttrNum = 0;
	pstRelationChg->RelationType = iType;

	// 高16--head  接下来 8位---hair      低8位---face
	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_HEAD;
	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head = 0;
	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= (pstBuddyInfo->Head<<16);
	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= (pstBuddyInfo->Hair<<8);
	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= pstBuddyInfo->Face;
	pstRelationChg->RelationAttrNum ++;

	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_CAREER;
	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Career = pstBuddyInfo->Career;
	pstRelationChg->RelationAttrNum ++;

	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_LEVEL;
	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Level = pstBuddyInfo->Level;
	pstRelationChg->RelationAttrNum ++;

	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_EMOTION;
	STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Emotion,
		pstBuddyInfo->Emotion, sizeof(pstBuddyInfo->Emotion));
	pstRelationChg->RelationAttrNum ++;
	
	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
} 

int relation_claninfo_to_clt_in(Player *pstOther,CSRELATIONSVR *pstRelationSvr)
{

//	RELATIONCLANINFO *pstRelationClanInfo = &pstRelationSvr->RelationSvrData.RelationClanInfo;

	pstRelationSvr->OpType = RELATION_OP_TYPE_CHG_CLAN;
	pstRelationSvr->RelationSvrData.RelationClanInfo.RoleID = pstOther->stRoleData.RoleID;
	pstRelationSvr->RelationSvrData.RelationClanInfo.RelationClan.ClanID = pstOther->stRoleData.MiscInfo.ClanInfo.ClanId.Id;
	pstRelationSvr->RelationSvrData.RelationClanInfo.RelationClan.ClanName[0] = 0;
	if (pstOther->stRoleData.MiscInfo.ClanInfo.ClanId.Id > 0)
	{
		STRNCPY(pstRelationSvr->RelationSvrData.RelationClanInfo.RelationClan.ClanName, 
				  pstOther->stRoleData.MiscInfo.ClanInfo.ClanId.Name,
				  sizeof(pstRelationSvr->RelationSvrData.RelationClanInfo.RelationClan.ClanName));
	}

	return 0;
}

int relation_claninfo_to_clt_mud(ZONESVRENV* pstEnv,Player *pstOther,char* pszName)
{
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;

	relation_claninfo_to_clt_in(pstOther,pstRelationSvr);

	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_pkg_to_mud(pstEnv,&stPkg,pszName,0,PKG_TRANS_PLAYER);
	return 0;
}

int relation_claninfo_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstOther)
{
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;

	relation_claninfo_to_clt_in(pstOther,pstRelationSvr);
	
	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int relation_chg_clt_detail_in(Player *pstOther, int iType,CSRELATIONSVR *pstRelationSvr)
{
	CSRELATIONCHG *pstRelationChg = &pstRelationSvr->RelationSvrData.RelationChg;
	ROLEBUDDY *pstBuddy = &pstOther->stRoleData.Buddy; 

	memset(pstRelationChg, 0, sizeof(*pstRelationChg));
	//	 CLANID *pstClanId =   &pstOther->stRoleData.MiscInfo.ClanInfo.ClanId;
	
	pstRelationSvr->OpType = RELATION_OP_TYPE_CHG;
	pstRelationChg->RelationAttrNum = 0;
	pstRelationChg->RelationType = iType;
	pstRelationChg->RoleID = pstOther->stRoleData.RoleID;

	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_NAME;
	STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Name,
		pstOther->stRoleData.RoleName, sizeof(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Name));
	pstRelationChg->RelationAttrNum ++;
	
	//if (iType == RELATION_TYPE_FRIEND || iType == RELATION_TYPE_MASTER || iType == RELATION_TYPE_PRENTICE ||
		//iType == RELATION_TYPE_SPOUSE || iType == RELATION_TYPE_BROTHER || iType == RELATION_TYPE_TMPFRIEND)
	{
		// head字段 高16位表示head         接下来8位表示hair          最后8为表示face
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_HEAD;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= (pstOther->stRoleData.Head<<16);
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= (pstOther->stRoleData.Hair<<8);
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= pstOther->stRoleData.Face;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_CAREER;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Career = pstOther->stRoleData.Career;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_LEVEL;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Level = pstOther->stRoleData.Level;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_EMOTION;
		STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Emotion,
			pstBuddy->Emotion, sizeof(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Emotion));
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_GENDER;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Gender = pstOther->stRoleData.Gender;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_BUDDYVAL;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.BuddyVal = 0;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_MEMNAME;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.MemName[0] = 0;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_ONLINE;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.IsOnline = ROLE_GAME_ONLINE;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_GROUP;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.GroupID = 1;
		pstRelationChg->RelationAttrNum ++;
		//if (pstOther->stRoleData.MiscInfo.ClanInfo.ClanId.Id > 0)
		//{
		//	relation_claninfo_to_clt(pstEnv, pstPlayer, pstOther);
		//}

	}
	return 0;
}

//  详细好友信息    发送给pstPlayer
int relation_chg_clt_detail(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstOther, int iType)
{
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;

	relation_chg_clt_detail_in(pstOther,iType,pstRelationSvr);	
	
	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	
	//if (iType != RELATION_TYPE_BLACK && iType != RELATION_TYPE_ENEMY && 
	//    iType != RELATION_TYPE_TMPENEMY )
	{
		relation_claninfo_to_clt(pstEnv, pstPlayer, pstOther);
	}
	return 0;
} 

//  详细好友信息    发送给mud
int relation_chg_clt_detail_mud(ZONESVRENV* pstEnv, Player* pstOther,char* pszName, int iType)
{
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;

	relation_chg_clt_detail_in(pstOther,iType,pstRelationSvr);	
	
	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_pkg_to_mud(pstEnv,&stPkg,pszName,0,PKG_TRANS_PLAYER);

	relation_claninfo_to_clt_mud(pstEnv,pstOther,pszName);	

	return 0;
} 



int relation_del_one(ZONESVRENV* pstEnv,Player *pstPlayer1, Player *pstPlayer2, ROLEBUDDYMEMBER * pstDel)
{
	BUDDY *pstBuddy;
	BUDDYINFO *pstBuddyInfo;
	ROLEBUDDY *pstRoleBuddy;
	
	pstBuddy = &pstPlayer1->stOnline.stRelation.Buddy;
	pstBuddyInfo = relation_buddy_db_find(pstBuddy, pstPlayer2->stRoleData.RoleID);
	if (pstBuddyInfo == NULL)
	{
		return -1;
	}
	
	pstBuddy->BuddyNum --;
	if (pstBuddyInfo != &pstBuddy->BuddyInfos[pstBuddy->BuddyNum])
	{
		memcpy(pstBuddyInfo, &pstBuddy->BuddyInfos[pstBuddy->BuddyNum], sizeof(*pstBuddyInfo));
	}

	pstPlayer1->stOnline.bRelationUpdateFlag = 2;

	pstRoleBuddy = &pstPlayer1->stRoleData.Buddy;
	if (pstDel->RelationType == RELATION_TYPE_FRIEND)
	{
		pstRoleBuddy->FriendNum --;
		if (pstDel != &pstRoleBuddy->Friends[pstRoleBuddy->FriendNum])
		{
			memcpy(pstDel, &pstRoleBuddy->Friends[pstRoleBuddy->FriendNum], sizeof(pstRoleBuddy->Friends[0]));
		}
	}
	else if (pstDel->RelationType == RELATION_TYPE_MASTER)
	{

	}
	else if (pstDel->RelationType == RELATION_TYPE_PRENTICE)
	{

	}
	else if (pstDel->RelationType == RELATION_TYPE_SPOUSE)
	{

	}
	else if (pstDel->RelationType == RELATION_TYPE_BROTHER)
	{

	}
	else
	{
		return 0;
	}

	return 0;
	UNUSED(pstEnv);
	
}


// 
int relation_check_name(ZONESVRENV* pstEnv,Player *pstPlayer1, Player *pstPlayer2)
{
	ROLEBUDDYMEMBER * pstMember;

	if (pstPlayer1 == pstPlayer2)
	{
		return 0;
	}
	
	pstMember = relation_buddy_online_find_by_roleid(&pstPlayer1->stRoleData.Buddy,
													pstPlayer2->stRoleData.RoleID);
	if (!pstMember)
	{
		return 0;
	}

	if (strcmp(pstMember->RoleName, pstPlayer2->stRoleData.RoleName)!=0)
	{
		chg_name_to_clt(pstEnv, pstPlayer1,pstPlayer2->stRoleData.RoleName, 
								pstMember->RoleID, pstMember->RoleName,CHG_ROLE_NAME);
		STRNCPY(pstMember->RoleName, pstPlayer2->stRoleData.RoleName, sizeof(pstMember->RoleName));
	}

	pstMember = relation_buddy_online_find_by_name(&pstPlayer1->stRoleData.Buddy, 
												pstPlayer2->stRoleData.RoleName);
	if (!pstMember)
	{
		return 0;
	}
	
	// 名字相同   RoleID不同   删除
	if (pstMember->RoleID != pstPlayer2->stRoleData.RoleID)
	{
		CSPKG stPkg;
		CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
		CSRELATIONDELRES *pstRelationDelRes = &pstRelationSvr->RelationSvrData.RelationDelRes;  
	
		pstRelationSvr->OpType = RELATION_OP_TYPE_DELRELATION_RES;
		pstRelationDelRes->Type = pstMember->RelationType;
		pstRelationDelRes->RoleID = pstMember->RoleID;
			
		Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
		z_cltmsg_send(pstEnv, pstPlayer1, &stPkg);

		relation_del_one(pstEnv,pstPlayer1, pstPlayer2, pstMember);
	}
	return 0;
}


int relation_check_online(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	TMEMBLOCK *pstMemBlock;
	Player *pstPlayerOnline;
	BUDDYINFO *pstBuddyInfo;

	ROLEBUDDYMEMBER *pstMember;
	
	for (i=0; i<MAX_PLAYER; i++)
	{
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayerOnline = (Player *)pstMemBlock->szData;
		if (	PLAYER_STATUS_ROLE_LOGIN != pstPlayerOnline->eStatus || 
			BUDDY_GET_FINI != pstPlayerOnline->stOnline.bGetBuddyStat)
		{
			continue;
		}

		relation_check_name(pstEnv,pstPlayer, pstPlayerOnline);
		
		pstBuddyInfo = relation_buddy_db_find(&pstPlayerOnline->stOnline.stRelation.Buddy, pstPlayer->stRoleData.RoleID);
		if (NULL == pstBuddyInfo)
		{
			continue;
		}

		relation_check_name(pstEnv,pstPlayerOnline, pstPlayer);

		if (relation_check_one(pstPlayer, pstBuddyInfo))
		{
			pstPlayerOnline->stOnline.bRelationUpdateFlag = 1;

			//todo 通知一下客户端
			// 客户端自己根据RoleID判断
			//relation_dbbuddy_chg_clt(pstEnv, pstPlayerOnline, pstBuddyInfo, RELATION_TYPE_NONE);

			pstMember = relation_buddy_online_find( &pstPlayerOnline->stRoleData.Buddy, pstPlayer->stRoleData.RoleID);
			if (pstMember)
			{
				relation_dbbuddy_chg_clt(pstEnv, pstPlayerOnline, pstBuddyInfo, pstMember->RelationType);
				relation_claninfo_to_clt(pstEnv, pstPlayerOnline, pstPlayer);
			}
			
		}
	}
	
	return 0;
}

// 检查从数据库取回来的数据
int relation_check(ZONESVRENV* pstEnv, Player *pstPlayer, RELATION *pstRelation)
{
	int i;
	Player *pstPlayerTmp;
	BUDDYINFO *pstBuddyInfo;
	ROLEBUDDYMEMBER *pstRoleBuddyMember;
	BUDDY *pstBuddy = &pstRelation->Buddy;
	ROLEBUDDY *pstRoleBuddy = &pstPlayer->stRoleData.Buddy;
	int iUpdate = 0;

	//正向检查
	for (i=pstRoleBuddy->FriendNum-1; i>=0; i--)
	{
		pstBuddyInfo = relation_buddy_db_find(pstBuddy, pstRoleBuddy->Friends[i].RoleID);
		if (NULL == pstBuddyInfo)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "relation_buddy_db_find %s fail", pstRoleBuddy->Friends[i].RoleName);
			
			if (pstBuddy->BuddyNum < MAX_BUDDY)
			{
				pstBuddyInfo = &pstBuddy->BuddyInfos[pstBuddy->BuddyNum++];
				memset(pstBuddyInfo, 0, sizeof(*pstBuddyInfo));
				pstBuddyInfo->RoleID = pstRoleBuddy->Friends[i].RoleID;
				iUpdate = 1;
			}
		}

		if (pstBuddyInfo)
		{
			pstPlayerTmp = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleBuddy->Friends[i].RoleName);
			if (pstPlayerTmp)
			{

		//  如果玩家A从数据库取回来的数据 与玩家B自身数据不符,以玩家B为标准,更新至数据库
				iUpdate = relation_check_one(pstPlayerTmp, pstBuddyInfo);
			}
		}
	}

	for (i=pstRoleBuddy->BrotherNum-1; i>=0; i--)
	{
		pstBuddyInfo = relation_buddy_db_find(pstBuddy, pstRoleBuddy->Brothers[i].RoleID);
		if (NULL == pstBuddyInfo)
		{	
			tlog_error(pstEnv->pstLogCat, 0, 0, "relation_buddy_db_find %s fail", pstRoleBuddy->Brothers[i].RoleName);

			if (pstBuddy->BuddyNum < MAX_BUDDY)
			{
				pstBuddyInfo = &pstBuddy->BuddyInfos[pstBuddy->BuddyNum++];
				memset(pstBuddyInfo, 0, sizeof(*pstBuddyInfo));
				pstBuddyInfo->RoleID = pstRoleBuddy->Brothers[i].RoleID;
				iUpdate = 1;
			}
		}

		if (pstBuddyInfo)
		{
			pstPlayerTmp = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleBuddy->Brothers[i].RoleName);
			if (pstPlayerTmp)
			{
				iUpdate = relation_check_one(pstPlayerTmp, pstBuddyInfo);
			}
		}
	}

	for (i=pstRoleBuddy->PrenticeNum-1; i>=0; i--)
	{
		pstBuddyInfo = relation_buddy_db_find(pstBuddy, pstRoleBuddy->Prentices[i].RoleID);
		if (NULL == pstBuddyInfo)
		{	
			tlog_error(pstEnv->pstLogCat, 0, 0, "relation_buddy_db_find %s fail", pstRoleBuddy->Prentices[i].RoleName);

			if (pstBuddy->BuddyNum < MAX_BUDDY)
			{
				pstBuddyInfo = &pstBuddy->BuddyInfos[pstBuddy->BuddyNum++];
				memset(pstBuddyInfo, 0, sizeof(*pstBuddyInfo));
				pstBuddyInfo->RoleID = pstRoleBuddy->Prentices[i].RoleID;
			}
		}

		if (pstBuddyInfo)
		{
			pstPlayerTmp = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleBuddy->Prentices[i].RoleName);
			if (pstPlayerTmp)
			{
				iUpdate = relation_check_one(pstPlayerTmp, pstBuddyInfo);
			}
		}
	}

	for (i=pstRoleBuddy->MasterNum-1; i>=0; i--)
	{
		pstBuddyInfo = relation_buddy_db_find(pstBuddy, pstRoleBuddy->Master[i].RoleID);
		if (NULL == pstBuddyInfo)
		{	
			tlog_error(pstEnv->pstLogCat, 0, 0, "relation_buddy_db_find %s fail", pstRoleBuddy->Master[i].RoleName);

			if (pstBuddy->BuddyNum < MAX_BUDDY)
			{
				pstBuddyInfo = &pstBuddy->BuddyInfos[pstBuddy->BuddyNum++];
				memset(pstBuddyInfo, 0, sizeof(*pstBuddyInfo));
				pstBuddyInfo->RoleID = pstRoleBuddy->Master[i].RoleID;
			}

		}

		if (pstBuddyInfo)
		{
			pstPlayerTmp = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleBuddy->Master[i].RoleName);
			if (pstPlayerTmp)
			{
				iUpdate = relation_check_one(pstPlayerTmp, pstBuddyInfo);
			}
		}
	}
/*
	for (i=pstRoleBuddy->SpouseNum-1; i>=0; i--)
	{
		pstBuddyInfo = relation_buddy_db_find(pstBuddy, pstRoleBuddy->Spouse[i].RoleID);
		if (NULL == pstBuddyInfo)
		{	
			tlog_error(pstEnv->pstLogCat, 0, 0, "relation_buddy_db_find %s fail", pstRoleBuddy->Spouse[i].RoleName);

			if (pstBuddy->BuddyNum < MAX_BUDDY)
			{
				pstBuddyInfo = &pstBuddy->BuddyInfos[pstBuddy->BuddyNum++];
				memset(pstBuddyInfo, 0, sizeof(*pstBuddyInfo));
				pstBuddyInfo->RoleID = pstRoleBuddy->Spouse[i].RoleID;
			}
		}

		if (pstBuddyInfo)
		{
			pstPlayerTmp = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleBuddy->Spouse[i].RoleName);
			if (pstPlayerTmp)
			{
				iUpdate = relation_check_one(pstPlayerTmp, pstBuddyInfo);
			}
		}
	}
	
*/
	//反向检查
	for (i=pstBuddy->BuddyNum-1; i>=0; i--)
	{
		pstRoleBuddyMember = relation_buddy_online_find(pstRoleBuddy, pstBuddy->BuddyInfos[i].RoleID);
		if (NULL == pstRoleBuddyMember)
		{
			pstBuddy->BuddyNum--;
			if (i != pstBuddy->BuddyNum)
			{
				memcpy(&pstBuddy->BuddyInfos[i], &pstBuddy->BuddyInfos[pstBuddy->BuddyNum], sizeof(pstBuddy->BuddyInfos[0]));
			}
			
			iUpdate = 1;
		}
	}

	//遍历在线角色校正
	relation_check_online(pstEnv, pstPlayer);

	if (iUpdate)
	{
		pstPlayer->stOnline.bRelationUpdateFlag = 2;// 立即更新
	}
	
	return 0;
}

int relation_db_info(ZONESVRENV* pstEnv, Player *pstPlayer, RELATION *pstRelation)
{
	int i;
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONCHGLIST *pstRelationChgList = &pstRelationSvr->RelationSvrData.RelationChgList;
	CSRELATIONCHG *pstRelationChg;
	BUDDYINFO *pstBuddyInfo;
	ROLEBUDDYMEMBER *pstMenber;
	CSPKG stPkg1;
	CSRELATIONSVR *pstRelationSvr1 = &stPkg1.Body.RelationSvr;
	RELATIONCLANINFOLIST *pstRelationClanInfoList = &pstRelationSvr1->RelationSvrData.RelationClanInfoList;

	pstRelationSvr->OpType = RELATION_OP_TYPE_CHGLIST;
	pstRelationChgList->Num = 0;

	pstRelationSvr1->OpType = RELATION_OP_TYPE_CHGLIST_CLAN;
	memset(pstRelationClanInfoList, 0, sizeof(*pstRelationClanInfoList));
	
	for (i=0; i<pstRelation->Buddy.BuddyNum; i++)
	{
		pstBuddyInfo = &pstRelation->Buddy.BuddyInfos[i];
		pstRelationChg = &pstRelationChgList->ChgList[pstRelationChgList->Num++];
		
		pstRelationChg->RoleID = pstBuddyInfo->RoleID;
		pstMenber = relation_buddy_online_find(&pstPlayer->stRoleData.Buddy, pstBuddyInfo->RoleID);
		if (!pstMenber)
		{
			return -1;
		}
		pstRelationChg->RelationType = pstMenber->RelationType;
		
		pstRelationChg->RelationAttrNum = 0;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_HEAD;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head = 0;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= (pstBuddyInfo->Head << 16);
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= (pstBuddyInfo->Hair << 8);
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Head |= pstBuddyInfo->Face;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_CAREER;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Career = pstBuddyInfo->Career;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_LEVEL;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Level = pstBuddyInfo->Level;
		pstRelationChg->RelationAttrNum ++;

		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_EMOTION;
		STRNCPY(pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.Emotion,
				pstBuddyInfo->Emotion, sizeof(pstBuddyInfo->Emotion));
		pstRelationChg->RelationAttrNum ++;

		pstRelationClanInfoList->RelationClanInfo[pstRelationClanInfoList->Num].RoleID = pstBuddyInfo->RoleID;
		pstRelationClanInfoList->RelationClanInfo[pstRelationClanInfoList->Num].RelationClan.ClanID = pstBuddyInfo->ClanID;
		STRNCPY(pstRelationClanInfoList->RelationClanInfo[pstRelationClanInfoList->Num].RelationClan.ClanName,
				pstBuddyInfo->ClanName, sizeof(pstBuddyInfo->ClanName));
		pstRelationClanInfoList->Num ++;
		
	}

	for (i = 0; i < pstRelationChgList->Num; i ++)
	{
		pstRelationChg = &pstRelationChgList->ChgList[i];
		if (pstRelationChg->RelationAttrNum > RELATION_ATTR_MAX)
		{
			pstRelationChg->RelationAttrNum = RELATION_ATTR_MAX;
		}
	}
	
	//将关系表里的信息通知给客户端
	if (pstRelationChgList->Num > 0)
	{
		Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	if (pstRelationClanInfoList->Num > 0)
	{
		Z_CSHEAD_INIT(&stPkg1.Head, RELATION_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg1);
	}

	//通知玩家好友列表中情侣或者夫妻标记
	relation_involve_marry_svr(pstEnv,pstPlayer,NULL,TYPE_LOGIN_INTIMATE_COLOR);
	
	return 0;
}

int relation_init_res(ZONESVRENV* pstEnv, Player *pstPlayer, TORMSELECTRES *pstSelectRes)
{
	RELATION stRelation;
	RELATION *pstRelation = &pstPlayer->stOnline.stRelation;

	if (BUDDY_GET_GETTING != pstPlayer->stOnline.bGetBuddyStat)
	{
		return -1;
	}

	if (1 == pstSelectRes->SelectResult.ResultNum)
	{
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stRelation, sizeof(stRelation)))
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
						"uin=%d torm_fetch_one_meta fail", pstPlayer->stRoleData.Uin);
			return -1;
		}

		if (stRelation.GID != pstPlayer->stRoleData.RoleID)
		{
			return -1;
		}

		memcpy(pstRelation, &stRelation, sizeof(*pstRelation));
	}
	else
	{
		//没有记录，将由relation_check 校正
	}

	pstPlayer->stOnline.bGetBuddyStat = BUDDY_GET_FINI;

	relation_check(pstEnv, pstPlayer, pstRelation);
	relation_db_info(pstEnv, pstPlayer, pstRelation);
	
	return 0;
}

int relation_db_buddy_chg(ZONESVRENV* pstEnv,  TORMSELECTRES *pstSelectRes, DBBuddyChg *pstBuddyChg)
{
	RELATION stRelation;
	BUDDYINFO *pstBuddyInfo;
	Player *pstPlayer;
	Player *pstOther;
	ROLEBUDDYMEMBER *pstRoleBuddyMember; 


	if (1 != pstSelectRes->SelectResult.ResultNum)
	{
		return -1;
	}

	if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stRelation, sizeof(stRelation)))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"uin=%d torm_fetch_one_meta fail", 0);
		return -1;
	}

	pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstBuddyChg->szName);
	if (pstPlayer)
	{
		pstRoleBuddyMember = relation_buddy_online_find(&pstPlayer->stRoleData.Buddy, pstBuddyChg->stBuddyInfo.RoleID);
		if (NULL == pstRoleBuddyMember)
		{
			return -1;
		}

		if (pstPlayer->stOnline.bGetBuddyStat != BUDDY_GET_FINI)
		{
			return 0;
		}

		pstBuddyInfo = relation_buddy_db_find(&pstPlayer->stOnline.stRelation.Buddy, pstBuddyChg->stBuddyInfo.RoleID);
		if (NULL == pstBuddyInfo)
		{
			return -1;
		}

		pstOther = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleBuddyMember->RoleName);
		if (pstOther)
		{
			relation_buddyInfo_get(pstOther, pstBuddyInfo);
		}
		else
		{
			//这种情况很少出现，可以不通知客户端
			memcpy(pstBuddyInfo, &pstBuddyChg->stBuddyInfo, sizeof(*pstBuddyInfo));
		}

		pstPlayer->stOnline.bRelationUpdateFlag = 1;
	}
	else
	{
		pstBuddyInfo = relation_buddy_db_find(&stRelation.Buddy, pstBuddyChg->stBuddyInfo.RoleID);
		if (NULL == pstBuddyInfo)
		{
			return -1;
		}

		memcpy(pstBuddyInfo, &pstBuddyChg->stBuddyInfo, sizeof(*pstBuddyInfo));

		relation_db_update(pstEnv, &stRelation);
	}
	
	return 0;
}

int relation_buddyval_change(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEBUDDYMEMBER *pstBuddyMember, int iBuddyVal)
{
	int iVal;
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONCHGLIST *pstRelationChgList = &pstRelationSvr->RelationSvrData.RelationChgList;
	CSRELATIONCHG *pstRelationChg;

	pstRelationSvr->OpType = RELATION_OP_TYPE_CHGLIST;
	pstRelationChgList->Num = 0;

	iVal = pstBuddyMember->BuddyVal + iBuddyVal;

	if (iVal < 0)
	{
		iVal = 0;
	}
	
	if (iVal > MAX_BUDDY_VAL)
	{
		iVal = MAX_BUDDY_VAL;
	}

	if ( pstBuddyMember->BuddyVal == iVal)
	{
		return -1;
	}

	pstBuddyMember->BuddyVal = iVal;
	pstPlayer->stOnline.bRelationUpdateFlag = 1;

	pstRelationChg = &pstRelationChgList->ChgList[pstRelationChgList->Num++];
	pstRelationChg->RelationAttrNum = 0;
	pstRelationChg->RoleID = pstBuddyMember->RoleID;
	pstRelationChg->RelationType = pstBuddyMember->RelationType;
	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_BUDDYVAL;
	pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.BuddyVal = iVal;
	pstRelationChg->RelationAttrNum++;
	
	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
	UNUSED(pstEnv);
	UNUSED(pstPlayer);
	UNUSED(pstBuddyMember);
	UNUSED(&iBuddyVal);
	
}

int player_friends_buddyval_chg(ZONESVRENV* pstEnv, Player *pstPlayer, Player **apstPlayer, int iPlayerNum, int iBuddyVal)
{
	int i;
	int iIdx;
	int iIdx2;
	ROLEBUDDY *pstBuddy = &pstPlayer->stRoleData.Buddy;
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONCHGLIST *pstRelationChgList = &pstRelationSvr->RelationSvrData.RelationChgList;
	CSRELATIONCHG *pstRelationChg;
	ROLEBUDDY *pstBuddy2;
	unsigned short usOldBuddyVal = 0;

	pstRelationSvr->OpType = RELATION_OP_TYPE_CHGLIST;
	pstRelationChgList->Num = 0;
	
	for (i = 0; i<iPlayerNum; i++)
	{
		if (pstPlayer == apstPlayer[i])
		{
			continue;
		}
		
		iIdx = online_info_find_friend_byID(pstBuddy, apstPlayer[i]->stRoleData.RoleID);
		if(iIdx == -1)
		{
			continue;
		}

		//相互为好友才能加亲密度
		pstBuddy2 = &apstPlayer[i]->stRoleData.Buddy;
		iIdx2 = online_info_find_friend_byID(pstBuddy2, pstPlayer->stRoleData.RoleID);
		if(iIdx2 == -1)
		{
			continue;
		}

		usOldBuddyVal = pstBuddy->Friends[iIdx].BuddyVal;
		
		if ( (pstBuddy->Friends[iIdx].BuddyVal + iBuddyVal) > MAX_BUDDY_VAL)
		{
			pstBuddy->Friends[iIdx].BuddyVal  = MAX_BUDDY_VAL;
		}
		else
		{
			pstBuddy->Friends[iIdx].BuddyVal += iBuddyVal;
		}	

		if(pstBuddy->Friends[iIdx].BuddyVal > usOldBuddyVal)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_CHAT, ZONEERR_WEDDING58,
											apstPlayer[i]->stRoleData.RoleName,
											pstBuddy->Friends[iIdx].BuddyVal-usOldBuddyVal);
		}
		
		pstRelationChg = &pstRelationChgList->ChgList[pstRelationChgList->Num++];
		pstRelationChg->RoleID = pstBuddy->Friends[iIdx].RoleID;
		pstRelationChg->RelationAttrNum = 0;
		pstRelationChg->RelationType = pstBuddy->Friends[iIdx].RelationType;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_BUDDYVAL;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.BuddyVal = pstBuddy->Friends[iIdx].BuddyVal;
		pstRelationChg->RelationAttrNum++;
		
	}	

	if (pstRelationChgList->Num == 0)
	{
		return -1;
	}

	for (i = 0; i < pstRelationChgList->Num; i ++)
	{
		pstRelationChg = &pstRelationChgList->ChgList[i];
		if (pstRelationChg->RelationAttrNum > RELATION_ATTR_MAX)
		{
			pstRelationChg->RelationAttrNum = RELATION_ATTR_MAX;
		}
	}

	pstPlayer->stOnline.bRelationUpdateFlag = 1;
	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int team_relation_buddyval_change(ZONESVRENV* pstEnv, ZoneTeam *pstTeam, int iBuddyVal)
{
	int i;
	Player *pstPlayer;
	Player *apstPlayer[MAX_TEAM_MEMBER];
	int iOnlineNum = 0;
	TEAMMEMBER *pstTeamMember;

	for (i=pstTeam->stTeamInfo.MemberNum-1; i>=0; i--)
	{
		pstTeamMember = &pstTeam->stTeamInfo.TeamMember[i];
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeamMember->MemberName);
		if (pstPlayer)
		{
			apstPlayer[iOnlineNum++] = pstPlayer;
		}
	}

	for(i = 0; i< iOnlineNum; i++)
	{
		player_friends_buddyval_chg(pstEnv, apstPlayer[i], apstPlayer, iOnlineNum, iBuddyVal);
	}

	return 0;
}

int relation_online_query(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, SSRELATIONONLINEREQ *pstReq)
{
	Player *pstPlayerTmp;
	int i;
	SSPKG stPkg;
	SSRELATIONONLINERES *pstRes = &stPkg.Body.RelationOnlineRes;

	pstRes->BuddyNum = 0;
	STRNCPY(pstRes->RoleName, pstReq->RoleName, sizeof(pstRes->RoleName));
	
	for(i=0; i < pstReq->BuddyNum; i++)
	{
		pstPlayerTmp = z_name_player(pstAppCtx,  pstEnv, pstReq->BuddyInfo[i].RoleName);
		if(NULL == pstPlayerTmp || 
			(pstPlayerTmp->eStatus != PLAYER_STATUS_ROLE_LOGIN && 
			pstPlayerTmp->eStatus != PLAYER_STATUS_ROLE_NOTHING)
			|| pstPlayerTmp->stRoleData.RoleID != pstReq->BuddyInfo[i].RoleID)
		{
			continue;
		}

		pstRes->BuddyInfo[pstRes->BuddyNum].RoleID = pstPlayerTmp->stRoleData.RoleID;
		pstRes->BuddyInfo[pstRes->BuddyNum].RelationType = pstReq->BuddyInfo[i].RelationType;
		STRNCPY(pstRes->BuddyInfo[pstRes->BuddyNum].RoleName, 
			pstReq->BuddyInfo[i].RoleName, sizeof(pstRes->BuddyInfo[pstRes->BuddyNum].RoleName));
		
		pstRes->BuddyNum++;
	}

	if(pstRes->BuddyNum > 0)
	{
		Z_SSHEAD_INIT(&stPkg.Head, RELATION_ONLINE_RES);
		z_ssmsg_send(pstEnv, &stPkg, pstEnv->iMudSvrID);
	}

	return 0;
}

int ss_relation_online_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,  SSRELATIONONLINERES *pstRes)
{
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONCHGLIST *pstRelationChgList = &pstRelationSvr->RelationSvrData.RelationChgList;
	CSRELATIONCHG *pstRelationChg;
	Player *pstPlayer;
	int i;
	
	pstPlayer = z_name_player(pstAppCtx, pstEnv, pstRes->RoleName);
	if(NULL == pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		return -1;
	}

	pstRelationSvr->OpType = RELATION_OP_TYPE_CHGLIST;
	pstRelationChgList->Num = 0;

	for(i = 0; i<pstRes->BuddyNum; i++)
	{
		pstRelationChg = &pstRelationChgList->ChgList[pstRelationChgList->Num++];
		pstRelationChg->RoleID = pstRes->BuddyInfo[i].RoleID;
		pstRelationChg->RelationType = pstRes->BuddyInfo[i].RelationType;
		pstRelationChg->RelationAttrNum = 0;
		
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_ONLINE;
		pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.IsOnline = ROLE_MUD_ONLINE;
		pstRelationChg->RelationAttrNum++;
	}

	for (i = 0; i < pstRelationChgList->Num; i ++)
	{
		pstRelationChg = &pstRelationChgList->ChgList[i];
		if (pstRelationChg->RelationAttrNum > RELATION_ATTR_MAX)
		{
			pstRelationChg->RelationAttrNum = RELATION_ATTR_MAX;
		}
	}

	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

//相互为好友组队打副本加亲密度
int friend_team_in_pworld_chg_buddyval(ZONESVRENV* pstEnv, ZONEPWORLD *pstPworld,ZONEPWORLDINST *pstPworldInst, Player *pstPlayer)
{
	ZoneTeam *pstTeam = NULL;
	ZoneTeam *pstOtherTeam = NULL;
	int ret = 0;
	int ret2 = 0;
	int iBuddyVal = 0;
	int i;
	Player *pstTmpPlayer = NULL;
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSRELATIONCHGLIST *pstRelationChgList = &pstRelationSvr->RelationSvrData.RelationChgList;
	CSRELATIONCHG *pstRelationChg;
	int iVal = 0;
	unsigned short usOldBuddyVal = 0;
	
	if(0 == pstPworld->stPworldDef.BuddyVal)
	{
		return 0;
	}

	pstRelationSvr->OpType = RELATION_OP_TYPE_CHGLIST;
	pstRelationChgList->Num = 0;
	
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(NULL == pstTeam)
	{
		return 0;
	}
	
	for(i=0; i<pstPworldInst->iPlayer; i++)
	{
		if(pstPworldInst->aiPlayerID[i] == pstPlayer->iMemID)
		{
			continue;
		}
		
		pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[i]);
		if(NULL == pstTmpPlayer || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}

		pstOtherTeam = player_team_get(pstEnv, pstTmpPlayer);
		if(pstOtherTeam && pstOtherTeam == pstTeam)
		{
			ret = online_info_find_friend_byID(&pstPlayer->stRoleData.Buddy, pstTmpPlayer->stRoleData.RoleID);
			ret2 = online_info_find_friend_byID(&pstTmpPlayer->stRoleData.Buddy, pstPlayer->stRoleData.RoleID);
			
			if(ret >=0 && ret2 >=0)
			{		
				iBuddyVal = pstPworld->stPworldDef.BuddyVal;

				usOldBuddyVal = pstPlayer->stRoleData.Buddy.Friends[ret].BuddyVal;
				
				iVal = pstPlayer->stRoleData.Buddy.Friends[ret].BuddyVal + iBuddyVal;

				if (iVal < 0)
				{
					iVal = 0;
				}
				
				if (iVal > MAX_BUDDY_VAL)
				{
					iVal = MAX_BUDDY_VAL;
				}

				if(pstPlayer->stRoleData.Buddy.Friends[ret].BuddyVal == iVal)
				{
					continue;
				}

				if(iVal > usOldBuddyVal)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_CHAT, ZONEERR_WEDDING58,
													pstTmpPlayer->stRoleData.RoleName,
													iVal-usOldBuddyVal);
				}
				
				pstPlayer->stRoleData.Buddy.Friends[ret].BuddyVal = iVal;
		
				pstRelationChg = &pstRelationChgList->ChgList[pstRelationChgList->Num++];
				pstRelationChg->RoleID = pstPlayer->stRoleData.Buddy.Friends[ret].RoleID;
				pstRelationChg->RelationAttrNum = 0;
				pstRelationChg->RelationType = pstPlayer->stRoleData.Buddy.Friends[ret].RelationType;
				pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrID = RELATION_ATTR_BUDDYVAL;
				pstRelationChg->RelationAttrs[pstRelationChg->RelationAttrNum].RelationAttrData.BuddyVal = iVal;
				pstRelationChg->RelationAttrNum++;
			}
		}
	}
	
	if (pstRelationChgList->Num == 0)
	{
		return -1;
	}

	for (i = 0; i < pstRelationChgList->Num; i ++)
	{
		pstRelationChg = &pstRelationChgList->ChgList[i];
		if (pstRelationChg->RelationAttrNum > RELATION_ATTR_MAX)
		{
			pstRelationChg->RelationAttrNum = RELATION_ATTR_MAX;
		}
	}

	pstPlayer->stOnline.bRelationUpdateFlag = 1;
	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

//找情侣
int find_lover_byID(ROLEBUDDY *pstRoleBuddy, tdr_ulonglong ullID)
{
	int i;

	for (i=0; i<pstRoleBuddy->FriendNum; i++)
	{
		if (pstRoleBuddy->Friends[i].RoleID == ullID)
		{
			if(pstRoleBuddy->Friends[i].IntimateColorType == INTIMATE_COLOR_LOVERS)
			{
				return i;
			}
		}
	}
	
	return -1;
}

//找夫妻
int find_spouse_byID(ROLEBUDDY *pstRoleBuddy, tdr_ulonglong ullID)
{
	int i;

	for (i=0; i<pstRoleBuddy->FriendNum; i++)
	{
		if (pstRoleBuddy->Friends[i].RoleID == ullID)
		{
			if(pstRoleBuddy->Friends[i].IntimateColorType == INTIMATE_COLOR_SPOUSE)
			{
				return i;
			}
		}
	}
	
	return -1;
}

//从好友列表中找情侣
int find_lover_from_friend_list(ROLEBUDDY *pstRoleBuddy)
{
	int i;

	for (i=0; i<pstRoleBuddy->FriendNum; i++)
	{
		if(pstRoleBuddy->Friends[i].IntimateColorType == INTIMATE_COLOR_LOVERS)
		{
			return i;
		}
	}
	
	return -1;
}

//从好友列表中找夫妻
int find_spouse_from_friend_list(ROLEBUDDY *pstRoleBuddy)
{
	int i;

	for (i=0; i<pstRoleBuddy->FriendNum; i++)
	{
		if(pstRoleBuddy->Friends[i].IntimateColorType == INTIMATE_COLOR_SPOUSE)
		{
			return i;
		}
	}
	
	return -1;
}

//表白结果返回处理
int relation_pursue_res_repose(ZONESVRENV* pstEnv, Player *pstPlayer,CSPURSUERESCLT *pstPursueResClt)
{
	Player *pstPurusePlayer = NULL;
	char szSelf[ROLE_NAME_LEN];
	char szSelf2[ROLE_NAME_LEN];
	int ret = 0;
	int ret2 = 0;
	ITEMDEF *pstItemDef = NULL;
	char szText[512];
	char szText1[512];
	int ret3;
	int ret4;
	
	pstPurusePlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPursueResClt->PursueName);
	if(NULL == pstPurusePlayer)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING8);
		return -1;
	}
	
	STRNCPY(szSelf, pstPurusePlayer->stRoleData.RoleName,sizeof(szSelf));
	battle_rolename_remove_prefix(szSelf);
	
	STRNCPY(szSelf2, pstPlayer->stRoleData.RoleName,sizeof(szSelf2));
	battle_rolename_remove_prefix(szSelf2);

	pstItemDef = z_find_itemdef(pstEnv, PURSUE_ITEM_ID);
	if(!pstItemDef)
	{
		return -1;
	}

	//对方当前已建立情侣夫妻关系
	ret = find_lover_from_friend_list(&pstPurusePlayer->stRoleData.Buddy);
	if(ret >= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING68,szSelf);
		return -1;
	}
	
	ret2 = find_spouse_from_friend_list(&pstPurusePlayer->stRoleData.Buddy);
	if(ret2 >= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING69,szSelf);
		return -1;
	}

	ret = find_lover_from_friend_list(&pstPlayer->stRoleData.Buddy);
	if(ret >= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING70,szSelf);
		return -1;
	}
	
	ret2 = find_spouse_from_friend_list(&pstPlayer->stRoleData.Buddy);
	if(ret2 >= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING71,szSelf);
		return -1;
	}

	ret3 = online_info_find_friend_byID(&pstPlayer->stRoleData.Buddy, pstPurusePlayer->stRoleData.RoleID);
	if(ret3 < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING80,szSelf);
		return -1;
	}
	
	ret4 = online_info_find_friend_byID(&pstPurusePlayer->stRoleData.Buddy, pstPlayer->stRoleData.RoleID);
	if(ret4 < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING81,szSelf);
		return -1;
	}

	//达不到亲密度最小值，不能表白
	if(pstPlayer->stRoleData.Buddy.Friends[ret3].BuddyVal < PURSUE_MIN_BUDDY_VAL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING60,PURSUE_MIN_BUDDY_VAL);
		return -1;
	}

	if(pstPurusePlayer->stRoleData.Buddy.Friends[ret4].BuddyVal < PURSUE_MIN_BUDDY_VAL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING60,PURSUE_MIN_BUDDY_VAL);
		return -1;
	}

	if(pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	}
	
	if(pstPurusePlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPurusePlayer, 0);
	}
	
	if(z_distance(&pstPlayer->stRoleData.Pos, &pstPurusePlayer->stRoleData.Pos) > 2000)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING82,szSelf);
		return -1;
	}
		
	//拒绝
	if(!pstPursueResClt->Accept)
	{
		if(pstPurusePlayer->stOnline.ullLoveID == pstPlayer->stRoleData.RoleID)
		{
			pstPurusePlayer->stOnline.ullLoveID = 0;
			pstPurusePlayer->stOnline.cPursueName[0] = 0;
		}
		
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPurusePlayer, SYS_MIDDLE2, ZONEERR_WEDDING37,szSelf2);
		return 0;
	}
	else
	{		
		ret = online_info_find_friend_byID(&pstPlayer->stRoleData.Buddy, pstPurusePlayer->stRoleData.RoleID);
		if(ret < 0)
		{
			return -1;
		}

		ret2 = online_info_find_friend_byID(&pstPurusePlayer->stRoleData.Buddy, pstPlayer->stRoleData.RoleID);
		if(ret2 < 0)
		{
			return -1;
		}
		
		if (package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPurusePlayer, PURSUE_ITEM_ID, 1, -1, NULL,PACKAGE_DEC_NONE) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING30,pstItemDef->Name);
			return -1;
		}
		
		if (0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPurusePlayer, PURSUE_ITEM_ID, 1, -1, NULL, 
			PACKAGE_DEC_NONE, IMPITEM_OPTYPE_WEDDING))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING30,pstItemDef->Name);
			return -1;
		}
		 
		pstPlayer->stRoleData.Buddy.Friends[ret].IntimateColorType = INTIMATE_COLOR_LOVERS;
		pstPurusePlayer->stRoleData.Buddy.Friends[ret2].IntimateColorType = INTIMATE_COLOR_LOVERS;
		
		relation_involve_marry_svr(pstEnv,pstPlayer,&pstPurusePlayer->stRoleData.RoleID,TYPE_INTIMATE_COLOR_SVR);
		relation_involve_marry_svr(pstEnv,pstPurusePlayer,&pstPlayer->stRoleData.RoleID,TYPE_INTIMATE_COLOR_SVR);
		
		//为了进入视野时，快速索引
		pstPlayer->stRoleData.Buddy.SpouseNum = 1;
		pstPurusePlayer->stRoleData.Buddy.SpouseNum = 1;
		
		memcpy(&pstPlayer->stRoleData.Buddy.Spouse[0],&pstPlayer->stRoleData.Buddy.Friends[ret],
		sizeof(pstPlayer->stRoleData.Buddy.Spouse[0]));
		
		memcpy(&pstPurusePlayer->stRoleData.Buddy.Spouse[0],&pstPurusePlayer->stRoleData.Buddy.Friends[ret2],
		sizeof(pstPurusePlayer->stRoleData.Buddy.Spouse[0]));

		//双方播特效
		snprintf(szText, sizeof(szText),"playEffct(%d,%d,\"effect\\\\entity\\\\10542.ent\")",pstPlayer->stRoleData.Pos.X,pstPlayer->stRoleData.Pos.Y);
		play_effect(pstEnv, pstPlayer, 1, pstPlayer->iMemID, szText, NULL);
		snprintf(szText1, sizeof(szText1),"playEffct(%d,%d,\"effect\\\\entity\\\\10542.ent\")",pstPurusePlayer->stRoleData.Pos.X,pstPurusePlayer->stRoleData.Pos.Y);
		play_effect(pstEnv, pstPurusePlayer, 1, pstPurusePlayer->iMemID, szText1, NULL);
		
		//全服广播
		z_sys_strf_broadcast(pstEnv, SYS_SHOW, ZONEERR_WEDDING47,szSelf2,szSelf);
		
		wedding_dsn_action(pstEnv, pstPlayer);
		wedding_dsn_action(pstEnv, pstPurusePlayer);

		pstPlayer->stOnline.ullLoveID = 0;
		pstPurusePlayer->stOnline.ullLoveID = 0;

		pstPlayer->stOnline.cPursueName[0] = 0;
		pstPurusePlayer->stOnline.cPursueName[0] = 0;
		
	}
	
	return 0;
}

//解除当前情侣夫妻关系处理
int relation_remove_relation_req_repose(ZONESVRENV* pstEnv, Player *pstPlayer,CSREMOVERELATIONREQCLT *RemoveRelationReqClt)
{
	Player *pstOther = NULL;
	int ret2 = 0;
	ITEMDEF *pstItemDef = NULL;
	ROLEBUDDYMEMBER *pstBuddyMem = NULL;
	char szText[512];
	WEDDINGINFO *pstWeddingInfo1 = NULL;
	WEDDINGINFO *pstWeddingInfo2 = NULL;
	tdr_ulonglong ullTempRoleID;
	tdr_ulonglong ullTempRoleID2;
	int iTempRet = 0;
	int iTempRet2 = 0;

	if (pstPlayer->stRoleData.Buddy.SpouseNum <= 0 || pstPlayer->stRoleData.Buddy.SpouseNum > MAX_BUDDY_SPOUSE)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING33);
		return -1;		
	}

	pstBuddyMem = player_find_friend_by_name(pstPlayer, pstPlayer->stRoleData.Buddy.Spouse[0].RoleName);
	if(!pstBuddyMem)
	{
		//不在好友列表中
		return -1;
	}
	
	iTempRet = find_lover_byID(&pstPlayer->stRoleData.Buddy, pstBuddyMem->RoleID);
	iTempRet2 = find_spouse_byID(&pstPlayer->stRoleData.Buddy, pstBuddyMem->RoleID);
	if(iTempRet < 0 && iTempRet2 < 0)
	{
		//即不是情侣也不是夫妻
		return -1;
	}
	
	WEDDINGGLOBAL *pstWeddingGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.WeddingGlobal;
	if(pstWeddingGlobal->TravelNum > 0)
	{
		//双方当前并非举行 婚礼蜜游
		ullTempRoleID = pstWeddingGlobal->TravelInfos[pstWeddingGlobal->TravelNum-1].FirstTRoleID;
		ullTempRoleID2 = pstWeddingGlobal->TravelInfos[pstWeddingGlobal->TravelNum-1].SecondTRoleID;
		if( ullTempRoleID == pstPlayer->stRoleData.RoleID || ullTempRoleID2 == pstPlayer->stRoleData.RoleID)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING39);
			//z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING39,pstItemDef->Name);
			return -1;
		}
	}
	
	if(pstWeddingGlobal->CelebrationNum > 0)
	{
		//双方当前并非举行 婚礼庆典中
		ullTempRoleID = pstWeddingGlobal->CelebrationInfos[pstWeddingGlobal->CelebrationNum-1].FirstCRoleID;
		ullTempRoleID2 = pstWeddingGlobal->CelebrationInfos[pstWeddingGlobal->CelebrationNum-1].SecondCRoleID;
		if( ullTempRoleID == pstPlayer->stRoleData.RoleID || ullTempRoleID2 == pstPlayer->stRoleData.RoleID)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING40);
			//z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING40,pstItemDef->Name);
			return -1;
		}
	}

	if(iTempRet2 >= 0)
	{
		//解除情侣的现在不扣分手道具: 无情剪刀了
		pstItemDef = z_find_itemdef(pstEnv, SEPATATE_ITEM_ID);
		if(!pstItemDef)
		{
			return -1;
		}
		
		if (package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, SEPATATE_ITEM_ID, 1, -1, NULL,PACKAGE_DEC_NONE) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING28,pstItemDef->Name);
			return -1;
		}
		
		if (0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, SEPATATE_ITEM_ID, 1, -1, NULL, 
			PACKAGE_DEC_NONE, IMPITEM_OPTYPE_WEDDING))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING28,pstItemDef->Name);
			return -1;
		}
	}
	
	//清除身上记录的信息
	pstWeddingInfo1 = &pstPlayer->stRoleData.MiscInfo.WeddingInfo;
	memset(pstWeddingInfo1,0,sizeof(*pstWeddingInfo1));
	
	//头顶信息删除身份关系相关的显示
	pstPlayer->stRoleData.Buddy.SpouseNum = 0;
	
	//好友列表中，删除相应对象的关系标志
	pstBuddyMem->IntimateColorType = INTIMATE_COLOR_FRIEND;
	relation_involve_marry_svr(pstEnv,pstPlayer,&pstBuddyMem->RoleID,TYPE_INTIMATE_COLOR_SVR);

	z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, LIVE_SKILL_ID, 1);
	z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, STRIVE_SKILL_ID, 1);

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_WEDDING54,pstBuddyMem->RoleName);
	wedding_dsn_action(pstEnv, pstPlayer);

	wedding_item_recover(pstEnv, pstPlayer);
	
	pstOther = z_name_player(pstEnv->pstAppCtx,pstEnv,pstBuddyMem->RoleName);
	if(NULL == pstOther)
	{		
		snprintf(szText, sizeof(szText), LJY_MAIL72 ,pstPlayer->stRoleData.RoleName,pstBuddyMem->RoleName);	   
		player_sysmail_fast_send(pstEnv, pstBuddyMem->RoleName, LJY_MAIL73 ,szText, NULL, 0, 0,LJY_MAIL2, 0);
		
		//数据库操作！
		DBMiscInfo stDBMiscInfo;
		tdr_ulonglong ullRoleID = pstBuddyMem->RoleID;
		memset(&stDBMiscInfo,0,sizeof(stDBMiscInfo));
		
		//清除身上记录的信息
		stDBMiscInfo.iCmd = DB_MISC_INFO_WEDDINGINFO_UPDATE;
		stDBMiscInfo.AsynMiscInfo.stMiscInfoWedding.ullRoleID = ullRoleID;
		stDBMiscInfo.AsynMiscInfo.stMiscInfoWedding.type = DB_MISC_WEDDING_SET_NULL;
	
		z_get_role_misc_info(pstEnv,&stDBMiscInfo,ullRoleID);
		
		DBRoleBuddy stDBRoleBuddy;
		memset(&stDBRoleBuddy,0,sizeof(stDBRoleBuddy));
		
		//头顶信息删除身份关系相关的显示和好友列表中，删除相应对象的关系标志
		stDBRoleBuddy.ullRoleID = ullRoleID;
		stDBRoleBuddy.type = DB_WEDDING_REMOVE_RELATION;
		stDBRoleBuddy.ullOtherRoleID = pstPlayer->stRoleData.RoleID;
		
		z_get_role_buddy(pstEnv,&stDBRoleBuddy,ullRoleID);

		//还要删除当初加的技能!!!!
		DBRoleSkillInfo stDBRoleSkillInfo;
		memset(&stDBRoleSkillInfo,0,sizeof(stDBRoleSkillInfo));
		stDBRoleSkillInfo.iCmd = DB_SKILL_INFO_DELETE;
		stDBRoleSkillInfo.AsynRoleSkill.stSkillInfoDelete.iDeleteNum = 2;
		stDBRoleSkillInfo.AsynRoleSkill.stSkillInfoDelete.ullRoleID = ullRoleID;
		stDBRoleSkillInfo.AsynRoleSkill.stSkillInfoDelete.stSkillInfoDeleteData[0].SkillID = LIVE_SKILL_ID;
		stDBRoleSkillInfo.AsynRoleSkill.stSkillInfoDelete.stSkillInfoDeleteData[1].SkillID = STRIVE_SKILL_ID;
		z_get_role_skill_info(pstEnv,&stDBRoleSkillInfo,ullRoleID);
	}
	else
	{
		ret2 = online_info_find_friend_byID(&pstOther->stRoleData.Buddy, pstPlayer->stRoleData.RoleID);
		if(ret2 < 0)
		{
			return -1;
		}
		
		//好友列表中，删除相应对象的关系标志
		pstOther->stRoleData.Buddy.Friends[ret2].IntimateColorType = INTIMATE_COLOR_FRIEND;
		relation_involve_marry_svr(pstEnv,pstOther,&pstPlayer->stRoleData.RoleID,TYPE_INTIMATE_COLOR_SVR);
		
		//对方头顶信息删除身份关系相关的显示
		pstOther->stRoleData.Buddy.SpouseNum = 0;
		
		//清除对方身上记录的信息
		pstWeddingInfo2 = &pstOther->stRoleData.MiscInfo.WeddingInfo;
		memset(pstWeddingInfo2,0,sizeof(*pstWeddingInfo2));
		
		//消息通知对方解除伴侣关系
		relation_involve_marry_svr(pstEnv,pstOther,pstPlayer->stRoleData.RoleName,TYPE_REMOVE_RELATION_INFORM_SVR);

		z_del_skill(pstEnv->pstAppCtx, pstEnv, pstOther, LIVE_SKILL_ID, 1);
		z_del_skill(pstEnv->pstAppCtx, pstEnv, pstOther, STRIVE_SKILL_ID, 1);

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE2, ZONEERR_WEDDING29,pstPlayer->stRoleData.RoleName);

		wedding_dsn_action(pstEnv, pstOther);
		wedding_item_recover(pstEnv, pstOther);
	}

	return 0;
}

//判断要回收哪个套餐目录
int get_wedding_item_id_by_meal(int iMeal)
{
	int iItemID = 0;
	if(iMeal == 1)
	{
		iItemID = PRIMARY_WEDDING_ITEM_ID;
	}
	else if(iMeal == 2)
	{
		iItemID = MIDDLE_WEDDING_ITEM_ID;
	}
	else
	{
		iItemID = ADVANCED_WEDDING_ITEM_ID;
	}
		
	return iItemID;
}

//客户端誓言面板的操作：愿意成为夫妻不（要双方都愿意才可以)
int relation_oath_res_repose(ZONESVRENV* pstEnv, Player *pstPlayer,CSOATHRESCLT *pstOathResClt)
{
	Player *pstOther = NULL;
	char szSelf[ROLE_NAME_LEN];
	char szSelf2[ROLE_NAME_LEN];
	int ret = 0;
	int ret2 = 0;
	WEDDINGINFO *pstWeddingInfo1 = NULL;
	WEDDINGINFO *pstWeddingInfo2 = NULL;
	int iMeal = 0;
	int iItemID = 0;
	ITEMDEF *pstItemDef = NULL;
	int flag = 0;
	int num = 0;
	int num2 = 0;
	ITEMDEF *pstItemDef2 = NULL;
	char szText[512];
	char szText1[512];
	int num3 = 0;
	int ret3 = 0;
	
	pstOther = z_name_player(pstEnv->pstAppCtx,pstEnv,pstOathResClt->OtherName);
	if(NULL == pstOther)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING8);
		return -1;
	}

	ret = find_spouse_from_friend_list(&pstPlayer->stRoleData.Buddy);
	if(ret >= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING48);
		return -1;
	}
	
	ret = find_lover_byID(&pstPlayer->stRoleData.Buddy, pstOther->stRoleData.RoleID);
	if(ret < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING51);
		return -1;
	}
	
	ret = find_spouse_from_friend_list(&pstOther->stRoleData.Buddy);
	if(ret >= 0)
	{
		return -1;
	}
	
	ret = find_lover_byID(&pstOther->stRoleData.Buddy, pstPlayer->stRoleData.RoleID);
	if(ret < 0)
	{
		return -1;
	}

	STRNCPY(szSelf, pstOther->stRoleData.RoleName,sizeof(szSelf));
	battle_rolename_remove_prefix(szSelf);
	
	STRNCPY(szSelf2, pstPlayer->stRoleData.RoleName,sizeof(szSelf2));
	battle_rolename_remove_prefix(szSelf2);

	//拒绝
	if(!pstOathResClt->Accept)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE, ZONEERR_WEDDING38,szSelf2);
		pstOther->stOnline.iWillToSpouseStatus = 2;
		return 0;
	}
	else
	{		
		ret = online_info_find_friend_byID(&pstPlayer->stRoleData.Buddy, pstOther->stRoleData.RoleID);
		if(ret < 0)
		{
			return -1;
		}

		ret2 = online_info_find_friend_byID(&pstOther->stRoleData.Buddy, pstPlayer->stRoleData.RoleID);
		if(ret2 < 0)
		{
			return -1;
		}

		pstOther->stOnline.iWillToSpouseStatus = 1;
		
		if(pstPlayer->stOnline.iWillToSpouseStatus != 1)
		{
			return -1;
		}
		
		pstWeddingInfo1 = &pstPlayer->stRoleData.MiscInfo.WeddingInfo;
		pstWeddingInfo2 = &pstOther->stRoleData.MiscInfo.WeddingInfo;
	
		if(pstWeddingInfo1->WeddingMeal > 0)
		{
			iMeal = pstWeddingInfo1->WeddingMeal;
			flag = 1;
		}
		else
		{
			if(pstWeddingInfo2->WeddingMeal > 0)
			{
				iMeal = pstWeddingInfo2->WeddingMeal;
				flag = 2;
			}
			else
			{
				return -1;
			}	
		}
		
		iItemID = get_wedding_item_id_by_meal(iMeal);
		
		pstItemDef = z_find_itemdef(pstEnv, iItemID);
		if(!pstItemDef)
		{
			return -1;
		}

		pstItemDef2 = z_find_itemdef(pstEnv, PURSUE_RING_ITEM_ID);
		if(!pstItemDef2)
		{
			return -1;
		}

		//回收相应的套餐目录道具
		if(flag == 1)
		{
			num3 = z_get_pak_item_num(pstEnv, &pstPlayer->stRoleData.Package, iItemID, -2,0,PACKAGE_DEC_NONE);
			if(num3 <= 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING65,pstPlayer->stRoleData.RoleName,pstItemDef->Name);
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE, ZONEERR_WEDDING65,pstPlayer->stRoleData.RoleName,pstItemDef->Name);
				return -1;
			}

			package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, iItemID, num3, -2, NULL, PACKAGE_DEC_NONE, IMPITEM_OPTYPE_WEDDING);
		}
		else
		{
			num3 = z_get_pak_item_num(pstEnv, &pstOther->stRoleData.Package, iItemID, -2,0,PACKAGE_DEC_NONE);
			if(num3 <= 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_MIDDLE, ZONEERR_WEDDING65,pstOther->stRoleData.RoleName,pstItemDef->Name);
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING65,pstOther->stRoleData.RoleName,pstItemDef->Name);
				return -1;
			}

			package_dec(pstEnv->pstAppCtx, pstEnv, pstOther, iItemID, num3, -2, NULL, PACKAGE_DEC_NONE, IMPITEM_OPTYPE_WEDDING);
		}

		num = z_get_pak_item_num(pstEnv, &pstPlayer->stRoleData.Package, PURSUE_RING_ITEM_ID, -2,0,PACKAGE_DEC_NONE);
		if(num > 0)
		{
			ret3 = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, PURSUE_RING_ITEM_ID, num, -2, NULL, 
			PACKAGE_DEC_NONE, IMPITEM_OPTYPE_WEDDING);

			if(ret3 >= 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_CHAT, ZONEERR_WEDDING64,pstItemDef2->Name);
			}
		}

		num2 = z_get_pak_item_num(pstEnv, &pstOther->stRoleData.Package, PURSUE_RING_ITEM_ID, -2,0,PACKAGE_DEC_NONE);
		if(num2 > 0)
		{
			ret3 = package_dec(pstEnv->pstAppCtx, pstEnv, pstOther, PURSUE_RING_ITEM_ID, num2, -2, NULL, 
			PACKAGE_DEC_NONE, IMPITEM_OPTYPE_WEDDING);

			if(ret3 >= 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOther, SYS_CHAT, ZONEERR_WEDDING64,pstItemDef2->Name);
			}
		}
		
		pstPlayer->stRoleData.Buddy.Friends[ret].IntimateColorType = INTIMATE_COLOR_SPOUSE;
		pstOther->stRoleData.Buddy.Friends[ret2].IntimateColorType = INTIMATE_COLOR_SPOUSE;

		relation_involve_marry_svr(pstEnv,pstPlayer,&pstOther->stRoleData.RoleID,TYPE_INTIMATE_COLOR_SVR);
		relation_involve_marry_svr(pstEnv,pstOther,&pstPlayer->stRoleData.RoleID,TYPE_INTIMATE_COLOR_SVR);
		
		//加夫妻技能
		z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, LIVE_SKILL_ID, 1, 1);
		z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, STRIVE_SKILL_ID, 1, 1);
			
		z_add_skill(pstEnv->pstAppCtx, pstEnv, pstOther, NULL, LIVE_SKILL_ID, 1, 1);
		z_add_skill(pstEnv->pstAppCtx, pstEnv, pstOther, NULL, STRIVE_SKILL_ID, 1, 1);
		
		//为了进入视野时，快速索引
		pstPlayer->stRoleData.Buddy.SpouseNum = 1;
		pstOther->stRoleData.Buddy.SpouseNum = 1;
		
		memcpy(&pstPlayer->stRoleData.Buddy.Spouse[0],&pstPlayer->stRoleData.Buddy.Friends[ret],
		sizeof(pstPlayer->stRoleData.Buddy.Spouse[0]));
		memcpy(&pstOther->stRoleData.Buddy.Spouse[0],&pstOther->stRoleData.Buddy.Friends[ret2],
		sizeof(pstOther->stRoleData.Buddy.Spouse[0]));

		//双方记得播特效
		snprintf(szText, sizeof(szText),"playEffct(%d,%d,\"effect\\\\entity\\\\10254_5_2.ent\")",pstPlayer->stRoleData.Pos.X,pstPlayer->stRoleData.Pos.Y);
		play_effect(pstEnv, pstPlayer, 1, pstPlayer->iMemID, szText, NULL);
		snprintf(szText1, sizeof(szText1),"playEffct(%d,%d,\"effect\\\\entity\\\\10254_5_2.ent\")",pstOther->stRoleData.Pos.X,pstOther->stRoleData.Pos.Y);
		play_effect(pstEnv, pstOther, 1, pstOther->iMemID, szText1, NULL);

		//全服玫瑰花特效
		play_effect(pstEnv, pstPlayer, 1, 0, "playUiEffect(0,0,0,\"effect\\\\entity\\\\pm_hmg_bxh.ent\")", NULL);
		
		//全服广播
		z_sys_strf_broadcast(pstEnv, SYS_SHOW, ZONEERR_WEDDING41,szSelf2,szSelf);
		
		//可以领取永恒钻戒了
		pstWeddingInfo1->WeddingFlag |= IF_CAN_HAVE_RING;
		pstWeddingInfo2->WeddingFlag |= IF_CAN_HAVE_RING;

		wedding_dsn_action(pstEnv, pstPlayer);
		wedding_dsn_action(pstEnv, pstOther);
	}
	
	return 0;
}

int relation_involve_marry_svr(ZONESVRENV* pstEnv,Player *pstPlayer,const void* pvData,int Type)
{
	CSPKG stPkg;
	CSRELATIONSVR *pstRelationSvr = &stPkg.Body.RelationSvr;
	CSINVOLVEMARRYSVR *pstInvolveMarrySvr = &pstRelationSvr->RelationSvrData.InvolveMarrySvr;
	CSINVOLVEMARRYSVRDATA *pstInvolveMarrySvrData = &pstInvolveMarrySvr->InvolveMarrySvrData;
	memset(pstInvolveMarrySvrData,0,sizeof(*pstInvolveMarrySvrData));
	
	pstRelationSvr->OpType = RELATION_OP_TYPE_INVOLVE_MARRY_SVR;
	
	switch(Type)
	{
		case TYPE_PURSUE_INFO_INFORM_SVR:
			{
				char *pName = (char *)pvData;
				if(pName != NULL)
				{
					STRNCPY(pstInvolveMarrySvrData->PursueInfoInformSvr.PursueName,pName,
						sizeof(pstInvolveMarrySvrData->PursueInfoInformSvr.PursueName));
				}
			}
			break;
		case TYPE_LOGIN_INTIMATE_COLOR:
			{
				ROLEBUDDY *pstBuddy = &pstPlayer->stRoleData.Buddy;
				int i;
				for (i=0; i<pstBuddy->FriendNum; i++)
				{
					//现在都是一对一的关系，可以这样写
					if(pstBuddy->Friends[i].IntimateColorType == INTIMATE_COLOR_LOVERS || 
					pstBuddy->Friends[i].IntimateColorType == INTIMATE_COLOR_SPOUSE)
					{
						pstInvolveMarrySvrData->LoginIntimateColor.RoleID = pstBuddy->Friends[i].RoleID;
						pstInvolveMarrySvrData->LoginIntimateColor.IntimateColorType = pstBuddy->Friends[i].IntimateColorType;
						break;
					}
				}
				
				if(i>=pstBuddy->FriendNum)
				{
					return -1;
				}
			}
			break;
		case TYPE_INTIMATE_COLOR_SVR:
			{
				tdr_ulonglong *pRoleID = (tdr_ulonglong *)pvData;
				int ret;
				if(pRoleID == NULL)		
				{
					return -1;
				}
				
				ret = online_info_find_friend_byID(&pstPlayer->stRoleData.Buddy, *(tdr_ulonglong*)pRoleID);
				if(ret < 0)
				{
					return -1;
				}
				
				pstInvolveMarrySvrData->IntimateColorSvr.RoleID = pstPlayer->stRoleData.Buddy.Friends[ret].RoleID;
				pstInvolveMarrySvrData->IntimateColorSvr.IntimateColorType = pstPlayer->stRoleData.Buddy.Friends[ret].IntimateColorType;
			}
			break;
		case TYPE_OATH_UI_SVR:
			{
				Player *pstPlayerB = (Player *)pvData;
				if(pstPlayerB == NULL)
				{
					return -1;
				}
				
				WEDDINGINFO *pstWeddingInfo1 = NULL;
				WEDDINGINFO *pstWeddingInfo2 = NULL;
				pstWeddingInfo1 = &pstPlayer->stRoleData.MiscInfo.WeddingInfo;
				pstWeddingInfo2 = &pstPlayerB->stRoleData.MiscInfo.WeddingInfo;
				
				if(pstWeddingInfo1->WeddingMeal > 0)
				{
					pstInvolveMarrySvrData->OathUISvr.WeddingMeal = pstWeddingInfo1->WeddingMeal;
					STRNCPY(pstInvolveMarrySvrData->OathUISvr.OrderName,pstPlayer->stRoleData.RoleName,
						sizeof(pstInvolveMarrySvrData->OathUISvr.OrderName));
				}
				else
				{
					if(pstWeddingInfo2->WeddingMeal > 0)
					{
						pstInvolveMarrySvrData->OathUISvr.WeddingMeal = pstWeddingInfo2->WeddingMeal;
						STRNCPY(pstInvolveMarrySvrData->OathUISvr.OrderName,pstPlayerB->stRoleData.RoleName,
											sizeof(pstInvolveMarrySvrData->OathUISvr.OrderName));
					}
					else
					{
						return -1;
					}
				}
		
				pstInvolveMarrySvrData->OathUISvr.Gender = pstPlayerB->stRoleData.Gender;
				STRNCPY(pstInvolveMarrySvrData->OathUISvr.Name,pstPlayerB->stRoleData.RoleName,
				sizeof(pstInvolveMarrySvrData->OathUISvr.Name));
			}
			break;
		case TYPE_REMOVE_RELATION_INFORM_SVR:
			{
				char *pName = (char *)pvData;
				if(pName == NULL)
				{
					return -1;
				}
				
				STRNCPY(pstInvolveMarrySvrData->RemoveRelationSvr.InitiativeName,pName,
							sizeof(pstInvolveMarrySvrData->RemoveRelationSvr.InitiativeName));
			}
			break;
		case TYPE_REMOVE_RELATION_UI:
			{
				ROLEBUDDY *pstBuddy = &pstPlayer->stRoleData.Buddy;
				int i;
				for (i=0; i<pstBuddy->FriendNum; i++)
				{
					//现在都是一对一的关系，可以这样写
					if(pstBuddy->Friends[i].IntimateColorType == INTIMATE_COLOR_LOVERS || 
					pstBuddy->Friends[i].IntimateColorType == INTIMATE_COLOR_SPOUSE)
					{
						pstInvolveMarrySvrData->RemoveRelationUI.IntimateColorType = pstBuddy->Friends[i].IntimateColorType;
						break;
					}
				}
				
				if(i>=pstBuddy->FriendNum)
				{
					return -1;
				}
			}
			break;
		case TYPE_CELEBRATION_INFO:
			{
				CSCELEBRATIONINFO *pstCelebrationInfo = (CSCELEBRATIONINFO *)pvData;
				if(NULL == pstCelebrationInfo)
				{
					return -1;
				}
				
				memcpy(&pstInvolveMarrySvrData->CelebrationInfo,pstCelebrationInfo,sizeof(pstInvolveMarrySvrData->CelebrationInfo));
			}
			break;
		default:
			return -1;
			break;
	}
	
	pstInvolveMarrySvr->Type = Type;
	
	Z_CSHEAD_INIT(&stPkg.Head, RELATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int wedding_celebration_to_clt(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	if(is_fini_global(pstEnv) == 0)
	{
		return 0;
	}
	
	WEDDINGGLOBAL *pstWeddingGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.WeddingGlobal;
	CELEBRATIONINFOS *pstCelebrationInfos = NULL;
	CSCELEBRATIONINFO stCelebrationInfo;
	
	if(pstWeddingGlobal->CelebrationNum > 0)
	{
		pstCelebrationInfos = &pstWeddingGlobal->CelebrationInfos[pstWeddingGlobal->CelebrationNum-1];
		STRNCPY(stCelebrationInfo.FirstName,pstCelebrationInfos->FirstCRoleName,sizeof(stCelebrationInfo.FirstName));
		STRNCPY(stCelebrationInfo.SecondName,pstCelebrationInfos->SecondCRoleName,sizeof(stCelebrationInfo.SecondName));
		stCelebrationInfo.EndTime = pstCelebrationInfos->CelebrationEndTime;
		memcpy(&stCelebrationInfo.Pos, &pstCelebrationInfos->Pos,sizeof(stCelebrationInfo.Pos));
		relation_involve_marry_svr(pstEnv,pstPlayer,&stCelebrationInfo,TYPE_CELEBRATION_INFO);
	}
	
	return 0;
}

/*其他玩家发xxx的情侣伴侣，自己只发个伴侣的名字，给客户端自行处理*/
int wedding_dsn_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if(pstPlayer == NULL)
	{
		return 0;
	}
	
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	CSPKG stPkg2;
	CSACTIONRES *pstActionRes2 = &stPkg2.Body.ActionRes;
	
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;	
	ROLEBUDDY *pstBuddy = &pstPlayer->stRoleData.Buddy;

	pstActionRes->ActionID = ACTION_WEDDING_DSN;
	pstActionRes->Data.WeddingDsn.MiscName[0] = 0;
	pstActionRes->Data.WeddingDsn.ShowFlag = 0;
	pstActionRes->ID = pstPlayer->iMemID;

	pstActionRes2->ActionID = ACTION_WEDDING_DSN;
	pstActionRes2->Data.WeddingDsn.MiscName[0] = 0;
	pstActionRes2->Data.WeddingDsn.ShowFlag = 0;
	pstActionRes2->ID = pstPlayer->iMemID;

	if(pstMiscInfo->Flag & SHOW_WEDDING_DSN_FLAG)
	{
		pstActionRes->Data.WeddingDsn.ShowFlag = 1;
		pstActionRes2->Data.WeddingDsn.ShowFlag = 1;
	}
	
	if (pstPlayer->stOnline.State & CS_STAT_STALL)
	{
		STRNCPY(pstActionRes->Data.WeddingDsn.MiscName, pstPlayer->stOnline.stStallOnline.szStallName, sizeof(pstActionRes->Data.WeddingDsn.MiscName));
		STRNCPY(pstActionRes2->Data.WeddingDsn.MiscName, pstPlayer->stOnline.stStallOnline.szStallName, sizeof(pstActionRes2->Data.WeddingDsn.MiscName));
	}
	else
	{
		if(pstBuddy->SpouseNum > 0)
		{
			char szText[STALL_MAX_NAME_LEN] = {0};
			char szSpouseName[ROLE_NAME_LEN];
			int type = pstBuddy->Spouse[pstBuddy->SpouseNum-1].IntimateColorType;
			
			STRNCPY(szSpouseName, pstBuddy->Spouse[pstBuddy->SpouseNum-1].RoleName, CCH(szSpouseName));
			battle_rolename_remove_prefix(szSpouseName);

			STRNCPY(pstActionRes2->Data.WeddingDsn.MiscName, szSpouseName, sizeof(pstActionRes2->Data.WeddingDsn.MiscName));
		
			switch(type)
			{
				case INTIMATE_COLOR_SPOUSE:
					snprintf(szText, sizeof(szText),LJY_SYSMSG27,szSpouseName);
					break;
				case INTIMATE_COLOR_LOVERS:
					snprintf(szText, sizeof(szText),LJY_SYSMSG26,szSpouseName);
					break;
				default:
					szText[0] = 0;
					break;
			}
			
			if(szText[0])
			{
				STRNCPY(pstActionRes->Data.WeddingDsn.MiscName, szText, sizeof(pstActionRes->Data.WeddingDsn.MiscName));
			}
		}
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0, &stPkg, 0);

	Z_CSHEAD_INIT(&stPkg2.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg2);

	return 0;
}

int wedding_machine_skill_effect(ZONESVRENV* pstEnv, Player *pstPlayer,Monster *pstMon,unsigned short SkillID)
{
	if(SkillID != 11401)
	{
		return 0;
	}

	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_EFFECT;

	snprintf(pstActionRes->Data.Effect.EffectStr, sizeof(pstActionRes->Data.Effect.EffectStr),"playUiEffect(0,0,0,\"effect\\\\entity\\\\pm_hmg_bxh.ent\")");

	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	z_sendpkg_monview(pstEnv->pstAppCtx, pstEnv, pstMon, &stPkg, 0);
	
	return 0;
}

int wedding_login_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int ret = 0;

	ret = find_lover_from_friend_list(&pstPlayer->stRoleData.Buddy);
	if(ret < 0)
	{
		wedding_item_recover(pstEnv, pstPlayer);
	}

	return 0;
}

int wedding_item_recover(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int num3 = 0;
	int flag = 0;
	int ret = 0;
	if(pstPlayer == NULL)
		return 0;

	num3 = z_get_pak_item_num(pstEnv, &pstPlayer->stRoleData.Package, PRIMARY_WEDDING_ITEM_ID, -2,0,PACKAGE_DEC_NONE);
	if(num3 > 0)
	{
		ret = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, PRIMARY_WEDDING_ITEM_ID, num3, -2, NULL, PACKAGE_DEC_NONE, IMPITEM_OPTYPE_WEDDING);
		if(ret >= 0)
		{
			flag = 1;
		}
	}

	num3 = z_get_pak_item_num(pstEnv, &pstPlayer->stRoleData.Package, MIDDLE_WEDDING_ITEM_ID, -2,0,PACKAGE_DEC_NONE);
	if(num3 > 0)
	{
		ret = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, MIDDLE_WEDDING_ITEM_ID, num3, -2, NULL, PACKAGE_DEC_NONE, IMPITEM_OPTYPE_WEDDING);
		if(ret >= 0)
		{
			flag = 1;
		}
	}

	num3 = z_get_pak_item_num(pstEnv, &pstPlayer->stRoleData.Package, ADVANCED_WEDDING_ITEM_ID, -2,0,PACKAGE_DEC_NONE);
	if(num3 > 0)
	{
		ret = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, ADVANCED_WEDDING_ITEM_ID, num3, -2, NULL, PACKAGE_DEC_NONE, IMPITEM_OPTYPE_WEDDING);
		if(ret >= 0)
		{
			flag = 1;
		}
	}

	if(1 == flag)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING83);
	}
		
	return 0;
}

int wedding_pursue_logout(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	Player *pstPurusePlayer = NULL;

	pstPurusePlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPlayer->stOnline.cPursueName);
	if(NULL == pstPurusePlayer)
	{
		return -1;
	}

	if(pstPurusePlayer->stOnline.ullLoveID == pstPlayer->stRoleData.RoleID)
	{
		pstPurusePlayer->stOnline.ullLoveID = 0;
		pstPurusePlayer->stOnline.cPursueName[0] = 0;
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPurusePlayer, SYS_MIDDLE2, ZONEERR_WEDDING37,pstPlayer->stRoleData.RoleName);
	}
	
	return 0;
}

//老玩家第一次进入游戏，默认勾选显示情侣伴侣称号状态
int wedding_dsn_once_flag(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
			
	if (!(pstMiscInfo->Flag & WEDDING_DSN_FLAG_ONCE))
	{
		pstMiscInfo->Flag |= SHOW_WEDDING_DSN_FLAG;
		pstMiscInfo->Flag |= WEDDING_DSN_FLAG_ONCE;
	}
	
	return 0;
}
