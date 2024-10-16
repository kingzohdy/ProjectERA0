/*
**  @file $RCSfile: zone_team.c,v $
**  general description of this module
**  $Id: zone_team.c,v 1.394 2014/04/28 11:48:05 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/04/28 11:48:05 $
**  @version $Revision: 1.394 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_team.h"
#include "zone_clt.h"
#include "zone_err.h"
#include "zone_db.h"
#include "libdirty.h"
#include "zone_move.h"
#include "zone_map.h"
#include "zone_relation.h"
#include "zone_package.h"
#include "zone_status.h"
#include "zone_combat.h"
#include "zone_oplog.h"
#include "zone_pet.h"
#include "zone_player.h"
#include "zone_autoteam.h"
#include "zone_roll.h"
#include "zone_battle.h"
#include "zone_diffpworld.h"
#include "zone_spansvr.h"
#include "zone_clan.h"
#include "zone_booty.h"
#include "zone_unreal.h"
#include "zone_trans.h"
#include "zone_span.h"
#include "zone_autopworld.h"
#include "zone_sparta.h"

#define TEAM_OFFLINE_VALID 180
#define MIN_TEAM_MEMBER 2

int g_team_follow_flag = 0;

#define TEAM_RESONANCE_STATUS_TIME 4 // 队员共鸣Status 持续时间(秒)

// shadow team info的存活时间
#define SHADOW_TEAM_ALIVE_TIME (TEAM_ACTIVE_TIMER*5)

// 设置队伍的契约
static int player_team_set_qiyue(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 const char *psSelectedName);

int player_team_follow_check_one(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstCap);

int flush_battle_pworld_queue(ZONESVRENV* pstEnv, Player *pstPlayer,ZoneTeam *pstZoneTeam);

int flush_battle_queue_team_info(ZONESVRENV* pstEnv, Player *pstPlayer,ZoneTeam *pstZoneTeam, int iFlag);

int player_team_del_apply(TeamOnline *pstTeamOnline, tdr_ulonglong ullRoleID);

// 更新队伍契约的有效状态
// 状态说明 : 0 契约无效， 1 契约有效未选中， 2 契约有效被选中
static int player_team_update_qiyue_valid(ZONESVRENV *pstEnv,
                                          ZoneTeam *pstZoneTeam);

// 计算队伍中成员的职业数
static int player_team_career_count(ZoneTeam *pstZoneTeam,
                                    unsigned int *pnCounters, int iNumCounter);

// 获取队伍中成员的等级范围
/* 所有人 */
static int AllMan(Player *pstCaptain, const TEAMMEMBER *pstMember);
/* 除了队长 */
static int ExceptLeader(Player *pstCaptain, const TEAMMEMBER *pstMember);
/* 队长的弟子们 */
static int OnlyPrentice(Player *pstCaptain, const TEAMMEMBER *pstMember);
static int player_team_member_level_range(ZoneTeam *pstZoneTeam,
                                          Player *pstCaptain,
                                          unsigned int *pnMinLevel,
                                          unsigned int *pnMaxLevel,
                                  int (*pfnFilter)(Player*, const TEAMMEMBER*));

// 计算队伍中某一职业数
static unsigned int player_team_one_career_count(ZoneTeam *pstZoneTeam,
                                                 int iGareer);

// 计算队伍中与队长为某关系的人数
static unsigned int player_team_one_relation_count(ZoneTeam *pstZoneTeam,
                                                   Player *pstLeader,
                                                   int iRelationType);

// 检查契约是否有效
// 无效返回0，否则返回非0
static int player_team_is_qiyue_valid(ZONESVRENV *pstEnv, Player *pstPlayer,
                                      ZoneTeam *pstZoneTeam, int iQiYueID);

// 判断此玩家是否是契约受益人
// 是受益人，返回非0， 否则返回0
static int player_team_is_beneficiary(ZONESVRENV *pstEnv, Player *pstPlayer,
                                      Player *pstCaptain,
                                      const QIYUERESULTTARGET *pstTarget);

// 判断两个guys的关系
static int relation_is(Player *pstHost, const char *psCustomName,
                       int iRelationType);

int player_team_sync(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam);

// 队伍成员之间共鸣
static int player_team_resonance(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam);




// 通过契约ID查找契约定义
QIYUEDEF* z_find_qiyuedef(ZONESVRENV *pstEnv, int iQiYueID , int *piIdx)
{
	return z_find_def(pstEnv->pstZoneObj->sQiYueDef, iQiYueID, QIYUE_DEF_DATA, 
	                  pstEnv->pstZoneObj->iQiYueDef, piIdx);
}

// 获取队伍的队长的Player结构体
Player* player_team_get_captain_player(ZONESVRENV *pstEnv,
                                              ZoneTeam *pstZoneTeam)
{
	const TEAMMEMBER *pstCaptain = player_team_get_captain(pstZoneTeam);
	return pstCaptain ? z_name_player(pstEnv->pstAppCtx, pstEnv,
	                                  pstCaptain->MemberName)
	                  : NULL;
}

// 计算队伍中成员的职业数
static int player_team_career_count(ZoneTeam *pstZoneTeam,
                                    unsigned int *pnCounters, int iNumCounter)
{
	const TEAMMEMBER *pstMembers = NULL;
	int i;
	
	if(!pnCounters || iNumCounter <= CAREER_CHURCH)
	{
		return -1;
	}

	memset(pnCounters, 0, sizeof(*pnCounters) * iNumCounter);

	pstMembers = pstZoneTeam->stTeamInfo.TeamMember;
	for(i = 0; i < pstZoneTeam->stTeamInfo.MemberNum; ++i)
	{
		if( (pstMembers[i].Career&0x0F) <= CAREER_CHURCH)
		{
			pnCounters[pstMembers[i].Career&0x0F]++;
		}
	}
	return 0;
}

// 获取队伍中成员的等级范围
/* 所有人 */
static int AllMan(Player *pstCaptain, const TEAMMEMBER *pstMember)
{
	UNUSED(pstCaptain);
	UNUSED(pstMember);

	return 1;
}

/* 除了队长 */
static int ExceptLeader(Player *pstCaptain, const TEAMMEMBER *pstMember)
{
	UNUSED(pstCaptain);
	
	return (pstMember->Position == TEAM_POSITION_MEMBER);
}

/* 队长的弟子们 */
static int OnlyPrentice(Player *pstCaptain, const TEAMMEMBER *pstMember)
{
	return relation_is(pstCaptain, (const char*)pstMember->MemberName,
	                   RELATION_TYPE_PRENTICE);
}

static int player_team_member_level_range(ZoneTeam *pstZoneTeam,
                                          Player *pstCaptain,
                                          unsigned int *pnMinLevel,
                                          unsigned int *pnMaxLevel,
                                   int (*pfnFilter)(Player*, const TEAMMEMBER*))
{
	unsigned int nMinLevel = MAX_LEVEL + 1, nMaxLevel = 0;
	const TEAMMEMBER *pstMembers = NULL;
	int i;

	if(!pfnFilter)
	{
		return -1;
	}

	if(!pnMinLevel && !pnMaxLevel)
	{
		return -1;
	}

	pstMembers = pstZoneTeam->stTeamInfo.TeamMember;
	for(i = 0; i < pstZoneTeam->stTeamInfo.MemberNum; ++i)
	{
		if((*pfnFilter)(pstCaptain, &pstMembers[i])) // 过滤器
		{
			if(pstMembers[i].Level < nMinLevel)
			{
				nMinLevel = pstMembers[i].Level;
			}

			if(pstMembers[i].Level > nMaxLevel)
			{
				nMaxLevel = pstMembers[i].Level;
			}
		}
	}

	if(pnMinLevel)
	{
		*pnMinLevel = nMinLevel;
	}
	if(pnMaxLevel)
	{
		*pnMaxLevel = nMaxLevel;
	}
	return 0;	
}

// 计算队伍中某一职业人数
static unsigned int player_team_one_career_count(ZoneTeam *pstZoneTeam,
                                                 int iCareer)
{
	unsigned int anCounter[CAREER_CHURCH + 1];

	iCareer = iCareer&0x0F;
	if(iCareer < CAREER_NON || iCareer > CAREER_CHURCH)
	{
		return 0;
	}

	//memset( anCounter, 0, sizeof(anCounter) );
	if(player_team_career_count(pstZoneTeam, anCounter,
	                            sizeof(anCounter) / sizeof(anCounter[0])) == -1)
	{
		return 0;
	}

	return anCounter[iCareer];
}

// 计算队伍中与队长为某关系的人数
static unsigned int player_team_one_relation_count(ZoneTeam *pstZoneTeam,
                                                   Player *pstLeader,
                                                   int iRelationType)
{
	const TEAMMEMBER *pstMembers = pstZoneTeam->stTeamInfo.TeamMember;
	unsigned int i = 0, nCount = 0;

	for(i = 0; i < pstZoneTeam->stTeamInfo.MemberNum; ++i)
	{
		if(relation_is(pstLeader, pstMembers[i].MemberName, iRelationType))
		{
			++nCount;
		}
	}
	return nCount;
}

// 判断两个guys的关系
static int relation_is(Player *pstHost, const char *psCustomName,
                       int iRelationType)
{
	const ROLEBUDDYMEMBER *pstBuddy = NULL;
	unsigned int i, nNumBuddy = 0;

	if(!psCustomName || !psCustomName[0])
	{
		return 0;
	}
		
	switch(iRelationType)
	{
	case RELATION_TYPE_PRENTICE: // 徒弟
		pstBuddy = pstHost->stRoleData.Buddy.Prentices;
		nNumBuddy = pstHost->stRoleData.Buddy.PrenticeNum;
		break;

	case RELATION_TYPE_SPOUSE: // 夫妻
		//pstBuddy = pstHost->stRoleData.Buddy.Spouse;
		//nNumBuddy = pstHost->stRoleData.Buddy.SpouseNum;
		break;

	default:
		// TODO : 其他关系判断未实现
		return 0;
		break;
	}

	for(i = 0; i < nNumBuddy; ++i)
	{
		if(strcmp(pstBuddy[i].MemName, psCustomName) == 0)
		{
			return 1;
		}
	}

	return 0;
}

// 检查契约是否有效
// 无效返回0，否则返回非0
static int player_team_is_qiyue_valid(ZONESVRENV *pstEnv, Player *pstPlayer,
                                      ZoneTeam *pstZoneTeam, int iQiYueID)
{
	int i;
	QIYUEDEF *pstQiYueDef = NULL;
	unsigned int nMaxLevel;
	unsigned int nMinLevel;

	if(!iQiYueID || !pstPlayer)
	{
		return 0;
	}

	pstQiYueDef = z_find_qiyuedef(pstEnv, iQiYueID, NULL);
	if( !pstQiYueDef )
	{
		return 0;
	}
	
	for(i = 0; i < MAX_QIYUE_LIMIT; ++i)
	{
		const QIYUELIMIT *pstQiYueLimit = &pstQiYueDef->QiYueLimit[i];

		/* 无 */
		if(pstQiYueLimit->QiYueLimitID == QIYUE_LIMIT_NO)
		{
			break;
		}

		switch(pstQiYueLimit->QiYueLimitID)
		{
			case QIYUE_LIMIT_LEADER_CAREER:/* 队长职业 */
				if((unsigned int)(pstPlayer->stRoleData.Career & 0x0f) != pstQiYueLimit->Val)
				{
					return 0;
				}	
				break;
			case QIYUE_LIMIT_LEADER_LEVEL_MIN:/* 队长最小等级 */
				if( pstPlayer->stRoleData.Level < pstQiYueLimit->Val )
				{
					return 0;
				}
				break;
			case QIYUE_LIMIT_LEADER_LEVEL_MAX:	/* 队长最大等级 */
				if( pstPlayer->stRoleData.Level > pstQiYueLimit->Val )
				{
					return 0;
				}
				break;
			case QIYUE_LIMIT_LEADER_SPOUSE: /* 有队友是夫妻 */
				if( player_team_one_relation_count( pstZoneTeam, pstPlayer, 
					RELATION_TYPE_SPOUSE ) == 0 )
				{
					return 0;
				}	
				break;
			case QIYUE_LIMIT_LEADER_PRENTICE_MIN: /* 徒弟最少人数 */
				if( player_team_one_relation_count( pstZoneTeam, pstPlayer, 
					RELATION_TYPE_PRENTICE ) < pstQiYueLimit->Val )
				{
					return 0;
				}
				break;
			case QIYUE_LIMIT_LEADER_PRENTICE_MAX: /* 徒弟最大人数 */
				if( player_team_one_relation_count( pstZoneTeam, pstPlayer, 
					RELATION_TYPE_PRENTICE ) > pstQiYueLimit->Val )
				{
					return 0;
				}
				break;
			case QIYUE_LIMIT_NEED_CAREER_WARRIOR_MIN: /* 最少战士数量 */
				if( player_team_one_career_count( pstZoneTeam, CAREER_WARRIOR ) 
					< pstQiYueLimit->Val )
				{
					return 0;
				}
				break;
			case QIYUE_LIMIT_NEED_CAREER_WARRIOR_MAX: /* 最多战士数量 */
				if( player_team_one_career_count( pstZoneTeam, CAREER_WARRIOR ) 
					> pstQiYueLimit->Val )
				{
					return 0;
				}
				break;
			case QIYUE_LIMIT_NEED_CAREER_MAGICOR_MIN: /* 最少法师数量 */
				if( player_team_one_career_count( pstZoneTeam, CAREER_MAGICOR ) 
					< pstQiYueLimit->Val )
				{
					return 0;
				}
				break;
			case QIYUE_LIMIT_NEED_CAREER_MAGICOR_MAX: /* 最多法师数量 */
				if( player_team_one_career_count( pstZoneTeam, CAREER_MAGICOR )
					> pstQiYueLimit->Val )
				{
					return 0;
				}
				break;
			case QIYUE_LIMIT_NEED_CAREER_GUNER_MIN: /* 最少枪手数量 */
				if( player_team_one_career_count( pstZoneTeam, CAREER_GUNER )
					< pstQiYueLimit->Val )
				{
					return 0;
				}
				break;
			case QIYUE_LIMIT_NEED_CAREER_GUNER_MAX:/* 最多枪手数量 */
				if( player_team_one_career_count( pstZoneTeam, CAREER_GUNER )
					> pstQiYueLimit->Val )
				{
					return 0;
				}
				break;
			case QIYUE_LIMIT_NEED_CAREER_CHURCH_MIN: /* 最少牧师数量 */
				if( player_team_one_career_count( pstZoneTeam, CAREER_CHURCH )
					< pstQiYueLimit->Val )
				{
					return 0;
				}
				break;
			case QIYUE_LIMIT_NEED_CAREER_CHURCH_MAX: /* 最多牧师数量 */
				if( player_team_one_career_count( pstZoneTeam, CAREER_CHURCH )
					> pstQiYueLimit->Val )
				{
					return 0;
				}
				break;
			case QIYUE_LIMIT_ALL_LEVEL_MAX: /* 所有成员等级最大值 */
				if( player_team_member_level_range( pstZoneTeam, pstPlayer, NULL, &nMaxLevel,
					AllMan ) == -1 )
				{
					return 0;
				}

				if( nMaxLevel > pstQiYueLimit->Val )
				{
					return 0;
				}
				break;
			case QIYUE_LIMIT_ALL_LEVEL_MIN: /* 所有成员等级最小值 */
				if( player_team_member_level_range( pstZoneTeam, pstPlayer, &nMinLevel, NULL, 
					AllMan ) == -1 )
				{
					return 0;
				}

				if( nMinLevel < pstQiYueLimit->Val )
				{
					return 0;
				}
				break;
			case QIYUE_LIMIT_ALL_PRENTICE_LEVEL_MAX: /* 所有徒弟等级最大值 */
				if( player_team_member_level_range( pstZoneTeam, pstPlayer, NULL, &nMaxLevel,
					OnlyPrentice ) == -1 )
				{
					return 0;
				}

				if( nMaxLevel > pstQiYueLimit->Val )
				{
					return 0;
				}
				break;
			case QIYUE_LIMIT_ALL_PRENTICE_LEVEL_MIN: /* 所有徒弟等级最小值 */
				if( player_team_member_level_range( pstZoneTeam, pstPlayer, &nMinLevel, NULL, 
					OnlyPrentice ) == -1 )
				{
					return 0;
				}

				if( nMinLevel < pstQiYueLimit->Val )
				{
					return 0;
				}
				break;
			case QIYUE_LIMIT_TEAMMEM_MAX: /* 队伍人数最大 */
				if( pstZoneTeam->stTeamInfo.MemberNum > pstQiYueLimit->Val )
				{
					return 0;
				}
				break;
			case QIYUE_LIMIT_TEAMMEM_MIN:
				if( pstZoneTeam->stTeamInfo.MemberNum < pstQiYueLimit->Val )
				{
					return 0;
				}
				break;
			case QIYUE_LIMIT_EXCEPT_LEADER_LEVEL_MAX: /* 除队长外所有成员等级最大值 */
				if( player_team_member_level_range( pstZoneTeam, pstPlayer, NULL, &nMaxLevel,
					ExceptLeader ) == -1 )
				{
					return 0;
				}

				if( nMaxLevel > pstQiYueLimit->Val )
				{
					return 0;
				}
				break;
			case QIYUE_LIMIT_EXCEPT_LEADER_LEVEL_MIN: /* 除队长外所有成员等级最小值 */
				if( player_team_member_level_range( pstZoneTeam, pstPlayer, &nMinLevel, NULL, 
					ExceptLeader ) == -1 )
				{
					return 0;
				}

				if( nMinLevel < pstQiYueLimit->Val )
				{
					return 0;
				}
				break;
			default:
				return 0;
				break;
		}
	}

	return 1;
}


// 判断此玩家是否是契约受益人
// 是受益人，返回非0， 否则返回0
static int player_team_is_beneficiary( ZONESVRENV * pstEnv, Player * pstPlayer, Player * pstCaptain, const QIYUERESULTTARGET * pstTarget )
{
	switch( pstTarget->QiYueResultTargetID )
	{
	case QIYUE_RESULT_TARGET_ALL: /* 全队 */
		{
			return 1;
		}
		break;

	case QIYUE_RESULT_TARGET_LEADERSPOUSE: /* 队长夫妻 */
		{
			if( pstPlayer ==  pstCaptain)
			{
				return player_team_one_relation_count( player_team_get( pstEnv, pstCaptain ), 
							pstCaptain, RELATION_TYPE_SPOUSE ) > 0;
			}
			else
			{
				return relation_is( pstCaptain, pstPlayer->stRoleData.RoleName, 
							RELATION_TYPE_SPOUSE );
			}
		}
		break;

	case QIYUE_RESULT_TARGET_MASTER: /* 师傅 */
		{
			if( pstPlayer ==  pstCaptain)
			{
				return player_team_one_relation_count( player_team_get( pstEnv, pstCaptain ), 
						pstCaptain, RELATION_TYPE_PRENTICE ) > 0;
			}
			else
			{
				return 0;
			}
		}
		break;

	case QIYUE_RESULT_TARGET_PRENTICE: /* 徒弟 */
		{
			if( pstPlayer ==  pstCaptain)
			{
				return 0;
			}
			else
			{
				return relation_is( pstCaptain, pstPlayer->stRoleData.RoleName,
						RELATION_TYPE_PRENTICE );
			}
		}
		break;

	case QIYUE_RESULT_TARGET_LEADER: /* 队长 */
		{
			return (pstPlayer ==  pstCaptain);
		}
		break;

	case QIYUE_RESULT_TARGET_OTHER: /* 除队长外 */
		{
			return (pstPlayer !=  pstCaptain);
		}
		break;

	default:
		{
			return 0;
		}
		break;
	}
	
	return 0;
}

int player_team_qiyue_action(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneTeam *pstTeam, unsigned int iQiYueID)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	UNUSED(pstTeam);

	pstActionRes->ActionID = ACTION_QIYUE;
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->Data.QiYue.QiYueID = iQiYueID;

	player_action_res_ex(pstEnv, pstPlayer, &stPkg);
	//Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	//z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);

	return 0;
}

// 设置队伍的契约
static int player_team_set_qiyue(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 const char *psSelectedName)
{
	Player *pstPlayerMember = NULL;
	ZoneTeam *pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	Player *pstSelectedPlayer = NULL;
	TEAMMEMBER *pstSelectedMember = NULL, *pstCaptain = NULL;
	int iQiYueID;
	long long llMs;
	struct timeval stDeltaTv;
	int i = 0;

	memset(&stDeltaTv,0,sizeof(stDeltaTv));
	
	if(!pstZoneTeam)
	{
		return -1;
	}

	// 队长才可以设置/取消契约
	pstCaptain = player_team_get_captain(pstZoneTeam);
	if(!pstCaptain ||
	   strcmp(pstCaptain->MemberName, pstPlayer->stRoleData.RoleName) != 0)
	{
		return -1;
	}

	// 空字符串就是取消契约
	if(!psSelectedName[0])
	{
		for(i = 0; i < pstZoneTeam->stTeamInfo.MemberNum; ++i)
		{
			if(QIYUE_STATE_SELECTED ==
			   pstZoneTeam->stTeamInfo.TeamMember[i].QiYueValid)
			{
				pstZoneTeam->stTeamInfo.TeamMember[i].QiYueValid =
					QIYUE_STATE_VALID;
			}
		}
		pstZoneTeam->stTeamInfo.QiYue = 0;

		for(i = 0; i < pstZoneTeam->stTeamInfo.MemberNum; ++i)
		{
			pstPlayerMember = z_name_player(pstEnv->pstAppCtx, pstEnv,
			                  pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
			if(!pstPlayerMember)
			{
				continue;
			}

			player_team_qiyue_action(pstEnv, pstPlayerMember, pstZoneTeam,
			                         pstZoneTeam->stTeamInfo.QiYue);
		}

		player_team_sync(pstEnv, pstZoneTeam);
		player_team_broadcast_tip(pstEnv, pstZoneTeam, LJY_TEAM19);
		return 0;
	}

	pstSelectedPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
	                                  psSelectedName);
	if(!pstSelectedPlayer)
	{
		return -1;
	}
	
	iQiYueID = z_qiyue_id_from_player(pstEnv, pstSelectedPlayer);
	if(!iQiYueID)
	{
		// 无契约状态
		pstZoneTeam->stTeamInfo.QiYue = 0;
		return 0;		
	}

	pstSelectedMember = player_team_get_member(pstZoneTeam,
	                                     pstSelectedPlayer->stRoleData.RoleName,
	                                           NULL);
	if(!pstSelectedMember)
	{
		// 不是同一个队的玩家
		return -1;		
	}

	if(!player_team_is_qiyue_valid(pstEnv, pstPlayer, pstZoneTeam, iQiYueID))
	{
		return -1;		
	}

	// 判断冷却时间是否已过
	if(pstZoneTeam->stQiYueLastTv.tv_sec != 0)
	{
		TV_DIFF(stDeltaTv, pstEnv->pstAppCtx->stCurr, pstZoneTeam->stQiYueLastTv);
		TV_TO_MS(llMs, stDeltaTv);
		if(llMs <= QIYUE_SWITCHING_CD + 1000)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
			               ZONEERR_TEAM101);
			return -1;
		}
	}

	for(i = 0; i < pstZoneTeam->stTeamInfo.MemberNum; ++i)
	{
		if(QIYUE_STATE_SELECTED ==
		   pstZoneTeam->stTeamInfo.TeamMember[i].QiYueValid)
		{
			if (pstSelectedMember == &pstZoneTeam->stTeamInfo.TeamMember[i])
			{
				return 0;
			}
			else
			{
				pstZoneTeam->stTeamInfo.TeamMember[i].QiYueValid =
					QIYUE_STATE_VALID;
			}
		}
	}

	// 设置契约生效
	pstSelectedMember->QiYueValid = QIYUE_STATE_SELECTED;
	pstZoneTeam->stTeamInfo.QiYue = iQiYueID;

	// 更新最后设置契约的时间。
	pstZoneTeam->stQiYueLastTv = pstEnv->pstAppCtx->stCurr;

	for(i = 0; i < pstZoneTeam->stTeamInfo.MemberNum; ++i)
	{
		pstPlayerMember = z_name_player(pstEnv->pstAppCtx, pstEnv,
		                      pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (!pstPlayerMember)
		{
			continue;
		}

		player_team_qiyue_action(pstEnv, pstPlayerMember, pstZoneTeam, iQiYueID);
	}	

	player_team_sync(pstEnv, pstZoneTeam);
	return 0;
}


// 更新队伍契约的有效状态
static int player_team_update_qiyue_valid(ZONESVRENV *pstEnv,
                                          ZoneTeam *pstZoneTeam)
{
	Player *pstCaptainPlayer = player_team_get_captain_player(pstEnv,
	                                                          pstZoneTeam);
	TEAMMEMBER *pstMembers = pstZoneTeam->stTeamInfo.TeamMember;
	int i, iSelecedQiYue = 0;

	for(i = 0; i < pstZoneTeam->stTeamInfo.MemberNum; ++i)
	{
		unsigned char bLastValid = pstMembers[i].QiYueValid;
		
		if(pstMembers[i].OffLineTime == 0)
		{
			// 队员在线
			Player *pstMemberPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, 
			                                        pstMembers[i].MemberName);
			int iQiYueID = 0;

			if(!pstMemberPlayer)
			{
				continue;
			}

			// 获取队员装备的契约ID。
			iQiYueID = z_qiyue_id_from_player(pstEnv, pstMemberPlayer);

			if(bLastValid == QIYUE_STATE_SELECTED)
			{
				// 有被选中的契约，标记一下。
				iSelecedQiYue = iQiYueID;
			}

			if(player_team_is_qiyue_valid(pstEnv, pstCaptainPlayer, pstZoneTeam,
			                              iQiYueID))
			{
				if(bLastValid != QIYUE_STATE_SELECTED)      
				{
					// 防止 "QIYUE_STATE_SELECTED"
					// 这个状态被洗成 "QIYUE_STATE_VALID"
					pstMembers[i].QiYueValid = QIYUE_STATE_VALID;
				}
			}
			else
			{
				if(bLastValid == QIYUE_STATE_SELECTED)
				{
					iSelecedQiYue = 0;
				}
				pstMembers[i].QiYueValid = QIYUE_STATE_INVALID;
			}
		}
		else
		{
			if(bLastValid == QIYUE_STATE_SELECTED)
			{
				iSelecedQiYue = 0;
			}
			pstMembers[i].QiYueValid = QIYUE_STATE_INVALID;
		}
	}

	if((int)pstZoneTeam->stTeamInfo.QiYue != iSelecedQiYue)
	{
		QIYUEDEF *pstQiYueDef = NULL;
		char szMsg[CLT_MSG_LEN];
			
		if (0 == iSelecedQiYue)
		{
			pstQiYueDef = z_find_qiyuedef(pstEnv, pstZoneTeam->stTeamInfo.QiYue,
			                              &pstZoneTeam->iQiYueIdx);
			if(pstQiYueDef)
			{
				sprintf(szMsg, LJY_TEAM13, pstQiYueDef->Name);
				player_team_broadcast_tip(pstEnv, pstZoneTeam, szMsg);
			}
		}
		else
		{
			pstQiYueDef = z_find_qiyuedef(pstEnv, iSelecedQiYue, NULL);
			if (pstQiYueDef)
			{
				sprintf(szMsg, LJY_TEAM14, pstQiYueDef->Name);
				player_team_broadcast_tip(pstEnv, pstZoneTeam, szMsg);
			}
		}
		
	}

	pstZoneTeam->stTeamInfo.QiYue = iSelecedQiYue;
	return 0;
}

// 获取契约效果
// 返回 0 : 契约被选定有效且受益人正确
// 否则返回 < 0
int player_team_checked_qiyue_result(ZONESVRENV *pstEnv, Player *pstPlayer,
                                     ZoneTeam *pstZoneTeam,
                                     QIYUERESULT *pstQiYueResult)
{
	const QIYUEDEF *pstQiYueDef = NULL;
	Player *pstCaptain = player_team_get_captain_player(pstEnv, pstZoneTeam);
	int i = 0;
	
	if(!pstQiYueResult || !pstCaptain)
	{
		return -1;
	}
	
	if(pstZoneTeam->stTeamInfo.QiYue == 0 || pstQiYueResult->QiYueResultID == 0)
	{
		// 无契约无效果
		memset(pstQiYueResult, 0, sizeof(*pstQiYueResult));
		return 0;
	}

	pstQiYueDef = z_find_qiyuedef(pstEnv, pstZoneTeam->stTeamInfo.QiYue,
	                              &pstZoneTeam->iQiYueIdx);
	if(!pstQiYueDef)
	{
		return -1;
	}

	for(i = 0; i < MAX_QIYUE_RESULT; ++i)
	{
		const QIYUERESULT *pstResult = &pstQiYueDef->QiYueResults[i];

		if(pstResult->QiYueResultID == 0)
		{
			break;
		}
		
		if(pstResult->QiYueResultID == pstQiYueResult->QiYueResultID &&
		   player_team_is_beneficiary(pstEnv, pstPlayer, pstCaptain,
		                              &pstResult->Target))
		{
			*pstQiYueResult = *pstResult;
			return 0;
		}
	}

	return -1;
}

ZoneTeam *z_idx_team(ZONESVRENV* pstEnv, int iTeamIdx, tdr_ulonglong ullTeamID)
{
	ZoneTeam *pstTeam = NULL;
	TMEMBLOCK *pstMemBlock = NULL;
	
	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, iTeamIdx);
	if (0 == pstMemBlock->fValid)
	{
		return NULL;
	}

	pstTeam= (ZoneTeam *)pstMemBlock->szData;
	if (pstTeam->stTeamInfo.TeamID != ullTeamID)
	{
		return NULL;
	}

	return pstTeam;
}

ZoneTeam *player_team_get(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZoneTeam *pstTeam = NULL;
	TMEMBLOCK *pstMemBlock = NULL;
	ROLEMISCINFO *pstRoleMiscInfo = NULL;
	
	pstRoleMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	if (pstRoleMiscInfo->TeamID == 0)
	{
		return NULL;
	}

	if (!is_my_master_svr(pstEnv->pstAppCtx->iId, pstPlayer))
	{
		pstTeam = team_shadow_by_id(pstEnv, pstRoleMiscInfo->TeamID);
		if(!pstTeam)
		{
			return NULL;
		}
	}
	else
	{
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, pstRoleMiscInfo->TeamIdx);
		if (0 == pstMemBlock->fValid)
		{
			pstRoleMiscInfo->TeamID = 0;
			return NULL;
		}
		pstTeam= (ZoneTeam *)pstMemBlock->szData;
	}

	if (pstTeam->stTeamInfo.TeamID != pstRoleMiscInfo->TeamID || NULL == player_team_get_member(pstTeam, pstPlayer->stRoleData.RoleName, NULL))
	{
		pstRoleMiscInfo->TeamID = 0;
		return NULL;
	}

	return pstTeam;
}

int player_team_insert_apply(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstPlayerOther)
{
	int i = 0;
	int iIdx = 0;
	TeamHandleEntry *pstTeamHandleEntry = NULL;
	TeamOnline *pstTeamOnline = &pstPlayer->stOnline.stTeamOnline;

	iIdx = -1;
	for (i=0; i<pstTeamOnline->unApplyNum; i++)
	{
		pstTeamHandleEntry = pstTeamOnline->astApplyEntry+i;
		if (pstTeamHandleEntry->ullRoleID == pstPlayerOther->stRoleData.RoleID && pstEnv->pstAppCtx->stCurr.tv_sec < pstTeamHandleEntry->tTime)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayerOther, SYS_MIDDLE2,ZONEERR_TEAM123);
			return -1;
		}

		if (0 == pstTeamHandleEntry->ullRoleID || pstEnv->pstAppCtx->stCurr.tv_sec >= pstTeamHandleEntry->tTime || 
			NULL == z_id_player(pstEnv->pstAppCtx, pstEnv, pstTeamHandleEntry->iMemID))
		{
			iIdx = i;
		}
	}

	if (iIdx >= 0)
	{
		pstTeamOnline->astApplyEntry[iIdx].ullRoleID = pstPlayerOther->stRoleData.RoleID;
		pstTeamOnline->astApplyEntry[iIdx].iMemID = pstPlayerOther->iMemID;
		pstTeamOnline->astApplyEntry[iIdx].tTime = pstEnv->pstAppCtx->stCurr.tv_sec + TEAM_OP_TIME;
		
		return iIdx;	
	}

	if (pstTeamOnline->unApplyNum < MAX_APPLY_NUM)
	{
		iIdx = pstTeamOnline->unApplyNum;
		pstTeamOnline->astApplyEntry[iIdx].ullRoleID = pstPlayerOther->stRoleData.RoleID;
		pstTeamOnline->astApplyEntry[iIdx].iMemID = pstPlayerOther->iMemID;
		pstTeamOnline->astApplyEntry[iIdx].tTime = pstEnv->pstAppCtx->stCurr.tv_sec + TEAM_OP_TIME;

		pstTeamOnline->unApplyNum++;

		return iIdx;
	}
		
	return -1;
}


// pstPlayerOther邀请pstPlayer
int player_team_insert_invite(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstPlayerOther)
{
	int i = 0;
	int iIdx = 0;
	TeamHandleEntry *pstTeamHandleEntry = NULL;
	TeamOnline *pstTeamOnline = &pstPlayer->stOnline.stTeamOnline;

	iIdx = -1;
	for (i=0; i<pstTeamOnline->unInviteNum; i++)
	{
		pstTeamHandleEntry = pstTeamOnline->astInviteEntry+i;
		if (pstTeamHandleEntry->ullRoleID == pstPlayerOther->stRoleData.RoleID && pstEnv->pstAppCtx->stCurr.tv_sec < pstTeamHandleEntry->tTime)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayerOther, SYS_MIDDLE2, ZONEERR_TEAM124);
			return -1;
		}

		if (0 == pstTeamHandleEntry->ullRoleID || pstEnv->pstAppCtx->stCurr.tv_sec >= pstTeamHandleEntry->tTime || 
			NULL == z_id_player(pstEnv->pstAppCtx, pstEnv, pstTeamHandleEntry->iMemID))
		{
			iIdx = i;
		}
	}

	if (iIdx >= 0)
	{
		pstTeamOnline->astInviteEntry[iIdx].ullRoleID = pstPlayerOther->stRoleData.RoleID;
		pstTeamOnline->astInviteEntry[iIdx].iMemID = pstPlayerOther->iMemID;
		pstTeamOnline->astInviteEntry[iIdx].tTime = pstEnv->pstAppCtx->stCurr.tv_sec + TEAM_OP_TIME;
		
		return iIdx;	
	}

	if (pstTeamOnline->unInviteNum < MAX_INVITE_NUM)
	{
		iIdx = pstTeamOnline->unInviteNum;
		pstTeamOnline->astInviteEntry[iIdx].ullRoleID = pstPlayerOther->stRoleData.RoleID;
		pstTeamOnline->astInviteEntry[iIdx].iMemID = pstPlayerOther->iMemID;
		pstTeamOnline->astInviteEntry[iIdx].tTime = pstEnv->pstAppCtx->stCurr.tv_sec + TEAM_OP_TIME;

		pstTeamOnline->unInviteNum++;

		return iIdx;
	}
		
	return -1;
}

TEAMMEMBER *player_team_get_captain(ZoneTeam *pstTeam)
{
	int i = 0;
	TEAMINFO *pstTeamInfo = &pstTeam->stTeamInfo;

	for (i=0; i<pstTeamInfo->MemberNum; i++)
	{
		if (TEAM_POSITION_CAPTAIN == pstTeamInfo->TeamMember[i].Position)
		{
			return &pstTeamInfo->TeamMember[i];
		}
	}

	return NULL;
}
//使用了这个函数后,相关member指针要考虑
int player_team_adjust_captain_pos(ZONESVRENV* pstEnv, ZoneTeam *pstTeam)
{
	int i = 0;
	TEAMINFO *pstTeamInfo = &pstTeam->stTeamInfo;
	TEAMMEMBER TeamMemberTmp;

	for (i=0; i<pstTeamInfo->MemberNum; i++)
	{
		if (TEAM_POSITION_CAPTAIN == pstTeamInfo->TeamMember[i].Position)
		{
			if(pstTeam->iRongYanMonId > 0)
			{
				Monster *pstMon;
				Player *pstCap;

				pstCap = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeamInfo->TeamMember[i].MemberName);
				if (pstCap)
				{
					pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstTeam->iRongYanMonId);
					if(NULL == pstMon)
					{
						return -1;
					}
					pstMon->iRongYanTeamCapId = pstCap->iMemID;
				}
			}
		}
		
		if ( (i !=0) &&
			 (TEAM_POSITION_CAPTAIN == pstTeamInfo->TeamMember[i].Position))
		{
			memset(&TeamMemberTmp,0,sizeof(TeamMemberTmp));
			memcpy(&TeamMemberTmp,&pstTeamInfo->TeamMember[i],sizeof(TEAMMEMBER));
			memcpy(&pstTeamInfo->TeamMember[i],&pstTeamInfo->TeamMember[0],sizeof(TEAMMEMBER));
			memcpy(&pstTeamInfo->TeamMember[0],&TeamMemberTmp,sizeof(TEAMMEMBER));
		}
	}

	return 0;
}

int player_team_apply_check(ZONESVRENV* pstEnv, Player *pstPlayer,Player *pstPlayerOther, Player **pstCap)
{
	ZoneTeam *pstTeam = NULL;
	TEAMMEMBER *pstTeamMember = NULL;
	
	if (pstPlayerOther->stRoleData.RoleID == pstPlayer->stRoleData.RoleID)
	{
		return -1;
	}

	if (pstPlayer->stOnline.State & CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM102);
		return -1;
	}

	if (pstPlayerOther->stOnline.State & CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM103);
		return -1;
	}

	if (player_team_get(pstEnv, pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM104);
		return -1;
	}

	if (0 <= player_black_find_byid(&pstPlayer->stRoleData.MiscInfo, pstPlayerOther->stRoleData.RoleID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM104);
		return -1;
	}

	if (player_enemy_find_byname(&pstPlayer->stRoleData.MiscInfo,
									pstPlayerOther->stRoleData.RoleName) >= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv,pstPlayer,SYS_MIDDLE2,ZONEERR_TEAM105);
		return -1;
	}

	pstTeam = player_team_get(pstEnv, pstPlayerOther);

	//lzk  融焰怪,	所有玩家组队锁定，只可离开队伍，不可进入队伍
	if(pstTeam && 1 == pstTeam->iRongYanBegin)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM800);
		return -1;
	}
	
	if (pstTeam)
	{
		if (pstTeam->stTeamInfo.MemberNum >= MAX_TEAM_MEMBER)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM106);
			return -1;
		}

		pstTeamMember = player_team_get_captain(pstTeam);

		// 找不到队长
		if (NULL == pstTeamMember )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM107);
			return -1;
		}

		// 申请的人不是队长
		if (0 != strcmp(pstTeamMember->MemberName, pstPlayerOther->stRoleData.RoleName))
		{
			Player *pstTmp = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeamMember->MemberName);
			if (!pstTmp)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM107 );
				return -1;
			}
			else
			{
				*pstCap = pstTmp;
			}
		}
		else
		{
			*pstCap = pstPlayerOther;
		}
	}
	else
	{
		*pstCap = pstPlayerOther;
	}

	return 0;
}


int player_team_apply(ZONESVRENV* pstEnv, Player *pstPlayer, CSTEAMAPPLY *pstTeamApply)
{
	Player *pstPlayerOther = NULL;
	CSPKG stPkg;
	CSTEAMSVR *pstTeamSvr = &stPkg.Body.TeamSvr;
	ZoneClan* pstZoneClan = NULL;
	Player *pstCap = NULL;

	
	pstPlayerOther = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeamApply->CaptainName);
	if (NULL == pstPlayerOther )
	{
		if(1 == is_player_in_battle_pvp_svr(pstEnv, pstPlayer))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPAN_MAP2,pstTeamApply->CaptainName);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM108);
		}
		return -1;
	}

	if(0 == is_player_in_battle_pvp_svr(pstEnv, pstPlayerOther))
	{
		if(is_player_in_span_svr(pstEnv, pstPlayerOther))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM109);
			return -1;
		}
	}
	
	if (player_team_apply_check(pstEnv, pstPlayer,pstPlayerOther,&pstCap) < 0)
		return -1;

	if (!pstCap)
	{
		return -1;
	}
		
	if (0 > player_team_insert_apply(pstEnv, pstCap, pstPlayer))
		return -1;

	//通知处理方
	memset(&pstTeamSvr->TeamSvrData.ApplyInfo,0,sizeof(pstTeamSvr->TeamSvrData.ApplyInfo));
	pstTeamSvr->OpType = TEAM_SVR_APPLY;
	pstTeamSvr->TeamSvrData.ApplyInfo.RoleID = pstPlayer->stRoleData.RoleID;
	STRNCPY(pstTeamSvr->TeamSvrData.ApplyInfo.RoleName, pstPlayer->stRoleData.RoleName, 
				sizeof(pstTeamSvr->TeamSvrData.ApplyInfo.RoleName));
	pstTeamSvr->TeamSvrData.ApplyInfo.Level = pstPlayer->stRoleData.Level;
	pstTeamSvr->TeamSvrData.ApplyInfo.Career = pstPlayer->stRoleData.Career;
	pstTeamSvr->TeamSvrData.ApplyInfo.Gender = pstPlayer->stRoleData.Gender;

	pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	if(pstZoneClan)
	{
		STRNCPY(pstTeamSvr->TeamSvrData.ApplyInfo.ClanName, pstZoneClan->stClan.ClanProf.Name,
				sizeof(pstTeamSvr->TeamSvrData.ApplyInfo.ClanName));
	}

	Z_CSHEAD_INIT(&stPkg.Head, TEAM_SVR);
	z_cltmsg_send(pstEnv, pstCap, &stPkg);
	
	return 0;
}

int player_team_add(ZONESVRENV* pstEnv, Player *pstPlayer, CSTEAMADD *pstTeamAdd)
{
	Player *pstPlayerOther = NULL;
	CSPKG stPkg;
	CSTEAMSVR *pstTeamSvr = &stPkg.Body.TeamSvr;
	ZoneTeam *pstZoneTeam;
	ZoneClan* pstZoneClan = NULL;
	Player *pstCap = NULL;
	TEAMMEMBER *pstTeamMember;
	ZoneTeam *pstTeam = NULL;
	
	pstPlayerOther = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeamAdd->MemberName);
	if (NULL == pstPlayerOther )
	{
		if(1 == is_player_in_battle_pvp_svr(pstEnv, pstPlayer))
		{
			//跨服野外单独提示，怕影响其他的
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPAN_MAP2,pstTeamAdd->MemberName);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM108);
		}
	
		return -1;
	}

	if(0 == is_player_in_battle_pvp_svr(pstEnv, pstPlayerOther))
	{
		if(is_player_in_span_svr(pstEnv, pstPlayerOther))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM109);
			return -1;
		}
	}

	if(player_team_get(pstEnv, pstPlayerOther))
	{
		if (player_team_apply_check(pstEnv, pstPlayer,pstPlayerOther,&pstCap) < 0)
			return -1;

		if (!pstCap)
		{
			return -1;
		}

		//lzk  融焰怪,	所有玩家组队锁定，只可离开队伍，不可进入队伍
		pstTeam = player_team_get(pstEnv, pstPlayerOther);
		if(pstTeam && 1 == pstTeam->iRongYanBegin)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM800);
			return -1;
		}
			
		if (0 > player_team_insert_apply(pstEnv, pstCap, pstPlayer))
			return -1;

		//通知处理方
		memset(&pstTeamSvr->TeamSvrData.ApplyInfo,0,sizeof(pstTeamSvr->TeamSvrData.ApplyInfo));
		pstTeamSvr->OpType = TEAM_SVR_APPLY;
		pstTeamSvr->TeamSvrData.ApplyInfo.RoleID = pstPlayer->stRoleData.RoleID;
		STRNCPY(pstTeamSvr->TeamSvrData.ApplyInfo.RoleName, pstPlayer->stRoleData.RoleName, 
					sizeof(pstTeamSvr->TeamSvrData.ApplyInfo.RoleName));
		pstTeamSvr->TeamSvrData.ApplyInfo.Level = pstPlayer->stRoleData.Level;
		pstTeamSvr->TeamSvrData.ApplyInfo.Career = pstPlayer->stRoleData.Career;
		pstTeamSvr->TeamSvrData.ApplyInfo.Gender = pstPlayer->stRoleData.Gender;

		pstZoneClan = player_get_clan(pstEnv, pstPlayer);
		if(pstZoneClan)
		{
			STRNCPY(pstTeamSvr->TeamSvrData.ApplyInfo.ClanName, pstZoneClan->stClan.ClanProf.Name,
					sizeof(pstTeamSvr->TeamSvrData.ApplyInfo.ClanName));
		}

		Z_CSHEAD_INIT(&stPkg.Head, TEAM_SVR);
		z_cltmsg_send(pstEnv, pstCap, &stPkg);
	}
	else
	{
		if (pstPlayer->stOnline.State & CS_STAT_DEAD)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM102);
			return -1;
		}

		if (pstPlayerOther->stOnline.State & CS_STAT_DEAD)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM103);
			return -1;
		}

		pstZoneTeam = player_team_get(pstEnv, pstPlayer);
		if (pstZoneTeam)
		{
			if (pstZoneTeam->stTeamInfo.MemberNum >= MAX_TEAM_MEMBER)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM106);
				return -1;
			}

			pstTeamMember = player_team_get_captain(pstZoneTeam);
			if (NULL == pstTeamMember || 0 != strcmp(pstTeamMember->MemberName, pstPlayer->stRoleData.RoleName))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM110, pstTeamAdd->MemberName);
				return -1;
			}
		}

		if (0 <= player_black_find_byid(&pstPlayer->stRoleData.MiscInfo, pstPlayerOther->stRoleData.RoleID))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,ZONEERR_TEAM111);
			return -1;
		}

		if (player_enemy_find_byname(&pstPlayer->stRoleData.MiscInfo,pstPlayerOther->stRoleData.RoleName) >= 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv,pstPlayer,SYS_MIDDLE2,ZONEERR_TEAM112);
			return -1;
		}

        //lzk  融焰怪,	所有玩家组队锁定，只可离开队伍，不可进入队伍
		if(pstZoneTeam && 1 == pstZoneTeam->iRongYanBegin)
		{
			z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM801);
			return -1;
		}
		
		if (0 > player_team_insert_invite(pstEnv, pstPlayerOther, pstPlayer))
		{
			return -1;
		}

		pstTeamSvr->OpType = TEAM_SVR_INVITE;

		if (pstZoneTeam)
		{
			memcpy(&pstTeamSvr->TeamSvrData.InviteInfo, &pstZoneTeam->stTeamInfo, sizeof(pstTeamSvr->TeamSvrData.InviteInfo));
		}
		else
		{
			memset(&pstTeamSvr->TeamSvrData.InviteInfo, 0, sizeof(pstTeamSvr->TeamSvrData.InviteInfo));
			pstTeamMember = &pstTeamSvr->TeamSvrData.InviteInfo.TeamMember[0];
			player_team_sync_member_info(pstEnv, pstTeamMember, pstPlayer);
			pstTeamMember->Position = TEAM_POSITION_CAPTAIN;
			pstTeamSvr->TeamSvrData.InviteInfo.MemberNum = 1;
		}

		Z_CSHEAD_INIT(&stPkg.Head, TEAM_SVR);
		z_cltmsg_send(pstEnv, pstPlayerOther, &stPkg);
	}
	
	return 0;
}

TEAMMEMBER *player_team_get_member(ZoneTeam *pstTeam, char *pszName, int *piIdx)
{
	int i = 0;
	TEAMINFO *pstTeamInfo = &pstTeam->stTeamInfo;

	for (i=0; i<pstTeamInfo->MemberNum; i++)
	{
		if (0 == strcmp(pstTeamInfo->TeamMember[i].MemberName, pszName))
		{
			if (piIdx)
			{
				*piIdx = i;
			}
			
			return &pstTeamInfo->TeamMember[i];
		}
	}

	return NULL;
}

static void pet_fill_diathesis_view(const ROLEPET *pstPet, TEAMPETVIEW *pstView)
{
	int i = 0;
	const PETSKILL *pstSkill = &pstPet->PetSkills;

	pstView->DiathesisNum = 0;
	for(i = 0; i < pstSkill->SkillGridNum; ++i)
	{
		if(pstSkill->SkillGrid[i].SkillID)
		{
			pstView->Diathesis[(int)pstView->DiathesisNum].ID = 
				pstSkill->SkillGrid[i].SkillID;
			pstView->Diathesis[(int)pstView->DiathesisNum].Level =
				pstSkill->SkillGrid[i].SkillLevel;
			++pstView->DiathesisNum;
		}
	}
}

void player_team_sync_member_info(ZONESVRENV* pstEnv, TEAMMEMBER *pstTeamMember, Player *pstPlayer)
{
	int i = 0;
	CSROLESTATUS *pstCSStatus = NULL;
	ZoneStatus *pstZoneStatus = NULL;
	ZoneStatusList *pstZoneStatusList = &pstPlayer->stOnline.stStatusList;
	FAIRYPACKAGE *pstFairyPkg = &pstPlayer->stRoleData.Package.FairyPackage;
	
	STRNCPY(pstTeamMember->MemberName, pstPlayer->stRoleData.RoleName, sizeof(pstTeamMember->MemberName));
	pstTeamMember->Level = pstPlayer->stRoleData.Level;
	pstTeamMember->Career = pstPlayer->stRoleData.Career;
	pstTeamMember->Gender = pstPlayer->stRoleData.Gender;
	pstTeamMember->HP = pstPlayer->stRoleData.RoleDetail.HPCurr;
	pstTeamMember->MaxHP = pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax;
	pstTeamMember->MP = pstPlayer->stRoleData.RoleDetail.MPCurr;
	pstTeamMember->MaxMP = pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax;
	pstTeamMember->MapID = pstPlayer->stRoleData.Map;
	pstTeamMember->Pos = pstPlayer->stRoleData.Pos;
	pstTeamMember->Head = pstPlayer->stRoleData.Head;
	pstTeamMember->OffLineTime = 0;
	pstTeamMember->Hair = pstPlayer->stRoleData.Hair;
	pstTeamMember->Face = pstPlayer->stRoleData.Face;
	pstTeamMember->MemID = pstPlayer->iMemID;
	pstTeamMember->Uin = pstPlayer->stRoleData.Uin;
	pstTeamMember->RoleID = pstPlayer->stRoleData.RoleID;
	pstTeamMember->Follow = pstPlayer->stOnline.stTeamFollow.cTeamFollow;
	pstTeamMember->BattlePworldID = 0;
	pstTeamMember->ZoneBusID = 0;

	if( ( CS_STAT_RIDE_ON | CS_STAT_FLY | CS_STAT_HITCH_RIDE  ) & pstPlayer->stOnline.State)
	{
		pstTeamMember->RideID = pstPlayer->stOnline.RideItemID;
		pstTeamMember->RideLevel = pstPlayer->stOnline.RideLevel;
	}
	else
	{
		pstTeamMember->RideID = 0;
		pstTeamMember->RideLevel = 0;
	}

	pstTeamMember->FairyID = 0;
	pstTeamMember->FairyLevel = 0;
	for (i=0; i<pstFairyPkg->FairyNum; i++)
	{
		if (pstFairyPkg->RoleFairys[i].Stat & FAIRY_STAT_CAMP)
		{
			pstTeamMember->FairyID = pstFairyPkg->RoleFairys[i].FairyID;
			pstTeamMember->FairyLevel = pstFairyPkg->RoleFairys[i].FairyLevel;
			break;
		}
	}

	
	
	pstTeamMember->StatusInfo.GoodStatusNum = pstZoneStatusList->bGoodStatusNum;
	for (i=0; i<pstZoneStatusList->bGoodStatusNum; i++)
	{
		pstCSStatus = &pstTeamMember->StatusInfo.GoodStatusList[i];
		memset(pstCSStatus,0,sizeof(CSROLESTATUS));
		pstZoneStatus = &pstZoneStatusList->astGoodStatus[i];

		pstCSStatus->StatusID = pstZoneStatus->unStatusID;
		pstCSStatus->Level = pstZoneStatus->bLevel;
		pstCSStatus->Overlay = pstZoneStatus->unOverlay;
		pstCSStatus->ApplyID = pstZoneStatus->iApplyID;
		pstCSStatus->ApplyRelation = pstZoneStatus->cApplyRelation;
		SET_STATUS_TIME(pstCSStatus->Time, pstZoneStatus->stEnd, pstEnv->pstAppCtx->stCurr);
	}
	
	pstTeamMember->StatusInfo.BadStatusNum = pstZoneStatusList->bBadStatusNum;
	for (i=0; i<pstZoneStatusList->bBadStatusNum; i++)
	{
		pstCSStatus = &pstTeamMember->StatusInfo.BadStatusList[i];
		memset(pstCSStatus,0,sizeof(CSROLESTATUS));
		pstZoneStatus = &pstZoneStatusList->astBadStatus[i];

		pstCSStatus->StatusID = pstZoneStatus->unStatusID;
		pstCSStatus->Level = pstZoneStatus->bLevel;
		pstCSStatus->Overlay = pstZoneStatus->unOverlay;
		pstCSStatus->ApplyID = pstZoneStatus->iApplyID;
		pstCSStatus->ApplyRelation = pstZoneStatus->cApplyRelation;
		SET_STATUS_TIME(pstCSStatus->Time, pstZoneStatus->stEnd, pstEnv->pstAppCtx->stCurr);
	}

	pstTeamMember->StatusInfo.NoLimitStatusNum = pstZoneStatusList->bNoLimitStatusNum;
	for (i=0; i<pstZoneStatusList->bNoLimitStatusNum; i++)
	{
		pstCSStatus = &pstTeamMember->StatusInfo.NolimitStatusList[i];
		memset(pstCSStatus,0,sizeof(CSROLESTATUS));
		pstZoneStatus = &pstZoneStatusList->astNoLimitStatus[i];

		pstCSStatus->StatusID = pstZoneStatus->unStatusID;
		pstCSStatus->Level = pstZoneStatus->bLevel;
		pstCSStatus->Overlay = pstZoneStatus->unOverlay;
		pstCSStatus->ApplyID = pstZoneStatus->iApplyID;
		pstCSStatus->ApplyRelation = pstZoneStatus->cApplyRelation;
		SET_STATUS_TIME(pstCSStatus->Time, pstZoneStatus->stEnd, pstEnv->pstAppCtx->stCurr);
	}
	/* 设置下玩家当前使用的契约 */
	pstPlayer->stRoleData.AvatarProf.QiYue = pstPlayer->stRoleData.Package.QiYueInfo.CurrQiYueID;
	memcpy(&pstTeamMember->Avatar, &pstPlayer->stRoleData.AvatarProf, sizeof(pstTeamMember->Avatar));

	// 填充宠物信息
	{
		const PetOnline *pstPetOnline = &pstPlayer->stOnline.stPetOnline;

		pstTeamMember->PetView.PetNum = 0;
		for(i = 0; i < pstPetOnline->bFightShiNum; i++)
		{
			const ROLEPET *pstRolePet = z_find_role_pet(
				pstPlayer,
				pstPetOnline->astFightShi[i].stRolePetOnline.ullWID, NULL);
			if(!pstRolePet)
			{
				continue;
			}

			pstTeamMember->PetView.PetView[i].Passive = pstRolePet->Passive;
			pet_fill_diathesis_view(pstRolePet,
			                        &pstTeamMember->PetView.PetView[i]);
			++pstTeamMember->PetView.PetNum;
		}
	}

	return;
}

int player_team_action(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneTeam *pstTeam)
{
	int iIdx;
	TEAMMEMBER *pstTeamMember;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_TEAM;
	pstActionRes->Data.Team.Type = 0;
	pstActionRes->Data.Team.TeamNum = pstTeam->stTeamInfo.MemberNum;
	pstActionRes->Data.Team.TeamID = pstTeam->stTeamInfo.TeamID;

	pstTeamMember = player_team_get_member(pstTeam, pstPlayer->stRoleData.RoleName, &iIdx);
	if (NULL == pstTeamMember)
	{
		return -1;
	}

/*
	//非野外跨服才检查，野外跨服需要转发action
	if(!is_player_in_battle_pvp_svr(pstEnv, pstPlayer))
	{
		if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			return -1;
		}
	}
*/
	
	if (TEAM_POSITION_CAPTAIN == pstTeamMember->Position)
	{
		pstActionRes->Data.Team.Captain = 1;
	}
	else
	{
		pstActionRes->Data.Team.Captain = 0;
	}
		
	STRNCPY(pstActionRes->Data.Team.TeamName, pstTeam->stTeamInfo.TeamName, sizeof(pstActionRes->Data.Team.TeamName));
	player_action_res_ex(pstEnv, pstPlayer, &stPkg);
	return 0;
}

ZoneTeam *player_team_create(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iIndex;
	TMEMBLOCK *pstMemBlock;
	ZoneTeam *pstTeam;
	TEAMMEMBER *pstTeamMember;
	//ROLEMISCINFO *pstRoleMisc = &pstPlayer->stRoleData.MiscInfo;
	
	iIndex = tmempool_alloc(pstEnv->pstTeamPool);
	if (0 > iIndex)
	{
		return NULL;
	}

	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, iIndex);
	pstTeam = (ZoneTeam *)pstMemBlock->szData;
	memset(pstTeam, 0, sizeof(*pstTeam ));
	pstTeam->stTeamInfo.TeamID = z_get_gid(pstEnv->pstAppCtx, pstEnv);
	pstTeam->iTeamIdx = iIndex;
	snprintf(pstTeam->stTeamInfo.TeamName, sizeof(pstTeam->stTeamInfo.TeamName), LJY_TEAM3, pstPlayer->stRoleData.RoleName);
	pstTeam->stTeamInfo.TeamName[sizeof(pstTeam->stTeamInfo.TeamName) - 1] = 0;
	pstTeam->stTeamInfo.MemberNum = 1;
	pstTeamMember = &pstTeam->stTeamInfo.TeamMember[0];
	player_team_sync_member_info(pstEnv, pstTeamMember, pstPlayer);
	pstTeamMember->Position = TEAM_POSITION_CAPTAIN;
	pstPlayer->stOnline.stTeamOnline.unInviteNum = 0;
	
	player_team_info_set(pstEnv, pstPlayer, pstTeam);

	//队长的副本加入队伍副本
	team_member_pworld_update(pstEnv, pstTeam, pstPlayer, 1);

	//队伍采用队长的默认地图线路
	pstTeam->iTeamMapLine = pstPlayer->stRoleData.MiscInfo.MapLine;

	player_team_adjust_captain_pos(pstEnv, pstTeam);
	team_shadow_insert(pstEnv, pstTeam);

	player_team_create_syn_diffpworld(pstEnv, pstPlayer, pstTeam);
	return pstTeam;
}

int player_team_broadcast_except(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, CSPKG *pstPkg, Player *pstPlayerExcept)
{
	int i;
	Player *pstPlayer;
	Player *apstPlayer[MAX_TEAM_MEMBER];
	int iOnlineNum = 0;
	TEAMMEMBER *pstTeamMember;
	TEAMINFO *pstTeam = &pstZoneTeam->stTeamInfo;
	
	for (i=pstTeam->MemberNum-1; i>=0; i--)
	{
		pstTeamMember = &pstTeam->TeamMember[i];
		if (pstTeamMember->OffLineTime > 0)
		{
			continue;
		}
		
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeamMember->MemberName);
		if (pstPlayer && pstPlayer != pstPlayerExcept)
		{
			apstPlayer[iOnlineNum++] = pstPlayer;
		}
	}

	if (iOnlineNum > 0)
	{
		z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iOnlineNum, pstPkg);
	}
	
	return 0;
}


int player_team_broadcast(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, CSPKG *pstPkg)
{
	return player_team_broadcast_except(pstEnv, pstZoneTeam, pstPkg, NULL);
}

int player_team_broadcast_tip(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, char *pszMsg)
{
	CSPKG stPkg;
	CSTEAMSVR *pstTeamSvr = &stPkg.Body.TeamSvr;

	Z_CSHEAD_INIT(&stPkg.Head, TEAM_SVR);
	pstTeamSvr->OpType = TEAM_SVR_MSG;
	STRNCPY(pstTeamSvr->TeamSvrData.TeamMsg.Msg, pszMsg, sizeof(pstTeamSvr->TeamSvrData.TeamMsg.Msg));
	player_team_broadcast(pstEnv, pstZoneTeam, &stPkg);
	
	return 0;
}

int player_team_broadcast_tip1(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, 
		char *pszMsg1, Player *pstPlayer, char *pszMsg2)
{

	CSPKG stPkg;
	CSTEAMSVR *pstTeamSvr = &stPkg.Body.TeamSvr;
	int i;
	Player *pstPlayerTmp;
	Player *apstPlayer[MAX_TEAM_MEMBER];
	int iOnlineNum = 0;
	TEAMMEMBER *pstTeamMember;
	TEAMINFO *pstTeam = &pstZoneTeam->stTeamInfo;

	if (!pstPlayer || !pszMsg2)
	{
		return player_team_broadcast_tip(pstEnv, pstZoneTeam, pszMsg1);
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, TEAM_SVR);
	pstTeamSvr->OpType = TEAM_SVR_MSG;
	STRNCPY(pstTeamSvr->TeamSvrData.TeamMsg.Msg, pszMsg2, sizeof(pstTeamSvr->TeamSvrData.TeamMsg.Msg));
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	
	STRNCPY(pstTeamSvr->TeamSvrData.TeamMsg.Msg, pszMsg1, sizeof(pstTeamSvr->TeamSvrData.TeamMsg.Msg));
	for (i=pstTeam->MemberNum-1; i>=0; i--)
	{
		pstTeamMember = &pstTeam->TeamMember[i];
		if (pstTeamMember->OffLineTime > 0)
		{
			continue;
		}
		
		pstPlayerTmp = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeamMember->MemberName);
		if (pstPlayerTmp && pstPlayerTmp->stRoleData.MiscInfo.TeamID && pstPlayerTmp != pstPlayer)
		{
			apstPlayer[iOnlineNum++] = pstPlayerTmp;
		}
	}

	if (iOnlineNum > 0)
	{
		z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iOnlineNum, &stPkg);
	}
	return 0;
}

int player_team_npc(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneTeam *pstZoneTeam, int iFlag)
{
	int i, j;
	SearchArea stSArea;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	AREADYN *pstArea;
	Npc *pstNpc;
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSLEAVEVIEW *pstLeaveView = &pstPkg->Body.LeaveView;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	if (0 > z_pos_search_area(pstEnv->pstAppCtx, pstEnv, pstMapInst, &pstPlayer->stRoleData.Pos, &stSArea))
		return -1;

	for(i = 0; i < stSArea.iNum; i++)
	{
		pstArea = stSArea.pastArea[i];

		for (j = 0; j < pstArea->iAnimate; j++)
		{
			pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstArea->aiAnimate[j]);
			if (pstNpc && pstNpc->stMon.cOwnerIDType && pstNpc->stMon.iOwnerID != pstPlayer->iMemID &&
				pstZoneTeam->stTeamInfo.TeamID == pstNpc->stMon.ullOwnerApplyID)
			{
				if (0 == z_pos_in_view(&pstNpc->stMon.stCurrPos, &pstPlayer->stRoleData.Pos))
				{
					continue;
				}
				
				if (iFlag)
				{
					z_player_view_monster(pstEnv->pstAppCtx, pstEnv, pstPlayer, &pstNpc->stMon);
				}
				else
				{
					pstLeaveView->ID = pstNpc->stMon.iID;
					pstLeaveView->HomeFlag = 0;

					Z_CSHEAD_INIT(&pstPkg->Head, LEAVE_VIEW);
					
					z_cltmsg_send(pstEnv, pstPlayer, pstPkg);		
				}
			}
		}
	}
	
	return 0;
}

int team_memchg_search_view(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	RESPOS stPos;
	MapIndex* pstMapIdx;
	
	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if(!pstMapIdx)
		return -1;

	if(z_pos_search_pos(pstMapIdx, &pstPlayer->stRoleData.Pos, &stPos))
	{
            	z_player_search_view(pstEnv->pstAppCtx, pstEnv, pstMapInst, &stPos, pstPlayer);
	}

	return 0;
}

int player_team_memchg(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneTeam *pstZoneTeam, int iFlag)
{
	int i;
	int fInPvP = 0;
	Player *pstPlayerOther;
	CSPKG stPkg;
	CSLEAVEVIEW *pstLeaveView = &stPkg.Body.LeaveView;

	player_team_npc(pstEnv, pstPlayer, pstZoneTeam, iFlag);

	//入队。搜索视野
	if(iFlag)
	{
		team_memchg_search_view(pstEnv, pstPlayer);
	}
	else
	{
		team_booty_player_exit(pstEnv, pstZoneTeam, pstPlayer);
	}
	
	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstPlayerOther = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (NULL == pstPlayerOther)
		{
			continue;
		}

		if(is_player_in_battle_pvp_svr(pstEnv, pstPlayer))
		{
			fInPvP = 1;
		}

		if (pstPlayerOther != pstPlayer)
		{
			if (pstPlayerOther->stRoleData.Map != pstPlayer->stRoleData.Map || 
				0 == z_pos_in_view(&pstPlayerOther->stRoleData.Pos, &pstPlayer->stRoleData.Pos))
			{
				continue;
			}

			if (iFlag)
			{
				if (pstPlayer->stOnline.State & CS_STAT_QIANXIN)
				{
					z_player_view_player(pstEnv->pstAppCtx, pstEnv, pstPlayerOther, pstPlayer);	
				}

				if (pstPlayerOther->stOnline.State & CS_STAT_QIANXIN)
				{
					z_player_view_player(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstPlayerOther);
				}
			}
			else
			{
				if (pstPlayer->stOnline.State & CS_STAT_QIANXIN)
				{
					pstLeaveView->ID = pstPlayer->iMemID;
					pstLeaveView->HomeFlag = 0;

					Z_CSHEAD_INIT(&stPkg.Head, LEAVE_VIEW);
					
					z_cltmsg_send(pstEnv, pstPlayerOther, &stPkg);
				}

				if (pstPlayerOther->stOnline.State & CS_STAT_QIANXIN)
				{
					pstLeaveView->ID = pstPlayerOther->iMemID;
					pstLeaveView->HomeFlag = 0;

					Z_CSHEAD_INIT(&stPkg.Head, LEAVE_VIEW);
					
					z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
				}	
			}
		}
	}

	/*
	if(fInPvP)
	{		
		team_shadow_sync_zone(pstEnv, pstZoneTeam, PLAYER_STATUS_BATTLE_LOGIN_READY, pstEnv->iBattlePvpID);
	}
	*/

	return 0;
}

//iFlag 0 人数< 2解散， 1  队长解散
int player_team_destroy_in(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, int iFlag)
{
	int i;
	Player *pstPlayer;
	CSPKG stPkg;
	char szMsg[CLT_MSG_LEN];
	CSTEAMSVR *pstTeamSvr = &stPkg.Body.TeamSvr;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	Player *pstOwner = NULL;
	unsigned int ullTeamID = 0;
 
	if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
	{
		//tlog_info(pstEnv->pstLogCat, 0, 0, "battle player_team_destroy_in");
	}

	//融焰怪也要清理
	if(pstZoneTeam->iRongYanMonId > 0)
	{
		Monster *pstMon;
		MONSTERDEF *pstMonDef;

		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->iRongYanMonId);
		if(pstMon)
		{
			pstMonDef = z_get_mon_def(pstEnv, pstMon);
			if (pstMonDef)
			{
				ZoneAni stAni;
		
				pstMon->cFirstAtkerType = 0;
				pstMon->ullFirstAtkerWid = 0;
				pstMon->iFirstAtkerID = 0; //没有战利品
				pstMon->szOwner[0] = 0;
				stAni.iType = OBJ_UNKNOW;

				//为了在调用mon_rongyan_boss_die(pstEnv,pstMon);时，进行一些数据清0
				pstZoneTeam->iRongYanCurLevel = -1;
				
				z_mon_die(pstEnv->pstAppCtx, pstEnv, pstMon, pstMonDef, &stAni, 2, 0);
			}
			else
			{
				z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);
			}
		}
	}
	
	autopworld_invite_enter(pstEnv, NULL,pstZoneTeam->iAutoPworldID, 1, pstZoneTeam,1);
	
	ullTeamID = pstZoneTeam->stTeamInfo.TeamID;
	//处理队伍副本
	for(i = 0; i < pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstPlayer)
		{
			ZONEPWORLDINST *pstTmp;
			ZONEMAPINST *pstMapInst;
			ZONEIDX stIdx;

			pstActionRes->ActionID = ACTION_TEAM;
			pstActionRes->ID = pstPlayer->iMemID;
			pstPlayer->stOnline.stTeamFollow.cTeamFollow = 0;
			pstActionRes->Data.Team.Type = 1;
			pstActionRes->Data.Team.TeamNum = 0;
			pstActionRes->Data.Team.Captain = 0;
			pstActionRes->Data.Team.TeamID = pstZoneTeam->stTeamInfo.TeamID;
			STRNCPY(pstActionRes->Data.Team.TeamName, pstZoneTeam->stTeamInfo.TeamName,
			        CCH(pstActionRes->Data.Team.TeamName));
			
			player_action_res_ex(pstEnv, pstPlayer, &stPkg);

			player_team_info_set(pstEnv, pstPlayer, NULL);
			player_team_memchg(pstEnv, pstPlayer, pstZoneTeam, 0);

			//竞技场清理
			player_sparta_team_chg_clean(pstEnv, pstPlayer);
			
			//搭乘
			if (pstPlayer->stOnline.State & CS_STAT_HITCH_RIDE)
			{
				z_ride_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
			}

			flush_battle_queue_team_info(pstEnv, pstPlayer,pstZoneTeam, 0);

			player_leave_team_clean_buff(pstEnv, pstPlayer, STATUS_RESULT_QIXINXIELI);

			z_team_convoy_task_fail(pstEnv, pstPlayer);

			if (TEAM_POSITION_CAPTAIN == pstZoneTeam->stTeamInfo.TeamMember[i].Position)
			{
				pstOwner = pstPlayer;
			}

			stIdx.iID = pstPlayer->stRoleData.Map;
			stIdx.iPos = pstPlayer->stOnline.iMapPos;
			pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);

			if (NULL == pstMapInst)
			{
				continue;
			}

			pstTmp = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
			if(NULL == pstTmp)
			{
				continue;
			}
			
			pstPlayer->stRoleData.MiscInfo.TeamID = 0;
			if (pstTmp->iType == PWORLD_TYPE_TEAM && 
				pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN )
			{
				player_pworld_exit_chgmap(pstEnv, pstPlayer);
				pstPlayer->stRoleData.MiscInfo.PworldInstID = 0;
			}
		}
	}

	if(INST_MODE_ZONE_SVR == pstEnv->iInstMode)
	{
		Z_CSHEAD_INIT(&stPkg.Head, TEAM_SVR);
		pstTeamSvr->OpType = TEAM_SVR_MSG;
		if (iFlag == 1)
		{
			strcpy(szMsg, LJY_TEAM4);
			if(pstOwner)
			{
				player_team_broadcast_tip1(pstEnv, pstZoneTeam, szMsg,pstOwner,LJY_TEAM5);
			}
			else
			{
				player_team_broadcast_tip1(pstEnv, pstZoneTeam, szMsg,NULL,LJY_TEAM5);
			}
		}
		else
		{
			strcpy(szMsg, LJY_TEAM6);
			player_team_broadcast_tip(pstEnv, pstZoneTeam, szMsg);
		}
	}
	
	team_booty_destroy( pstEnv, pstZoneTeam);
	
	if(INST_MODE_ZONE_SVR == pstEnv->iInstMode)
	{
		z_team_span_destroy_req(pstEnv, pstZoneTeam);
		
		if (0 > tmempool_free(pstEnv->pstTeamPool, pstZoneTeam->iTeamIdx))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "team %s %d destroy fail", pstZoneTeam->stTeamInfo.TeamName, pstZoneTeam->iTeamIdx);
		}

		//tlog_debug(pstEnv->pstLogCat, 0, 0, "队伍%s 被解散", pstZoneTeam->stTeamInfo.TeamName);
	}
	else
	{
		team_shadow_delete(pstEnv, ullTeamID);
	}

	return 0;
}

int player_team_sync(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam)
{
	int i;
	CSPKG stPkg;
	Player *pstPlayer;
	char szMsg[CLT_MSG_LEN];
	Player *apstPlayer[MAX_TEAM_MEMBER];
	//Player *apstPlayer_battle[MAX_TEAM_MEMBER];
	//Player *apstPlayer_invalid[MAX_TEAM_MEMBER];
	//Player *apstPlayer_ready_login[MAX_TEAM_MEMBER];
	//int iOnlineNum_ready_login = 0;
	//int iOnlineNum_invalid = 0;
	//int iOnlineNum_battle = 0;
	int iOnlineNum = 0;
	TEAMMEMBER *pstTeamMember;
	CSTEAMSVR *pstTeamSvr = &stPkg.Body.TeamSvr;
	Player *pstCaptain;
	int  iAdjustCaptain = 0;
	unsigned long long RoleIDCaption = 0;

	pstCaptain = player_team_get_captain_player(pstEnv,pstZoneTeam);
	if(!pstCaptain)
	{
		iAdjustCaptain = 1;
	}
	
	for(i = pstZoneTeam->stTeamInfo.MemberNum - 1; i >= 0; --i)
	{
		pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[i];
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeamMember->MemberName);		
		if (pstPlayer && pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.tRoleLogin >= 0)
		{
			if(pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN ||
				pstPlayer->eStatus == PLAYER_STATUS_ROLE_NOTHING)
			{
				apstPlayer[iOnlineNum++] = pstPlayer;
				if(iAdjustCaptain==1)
				{
					pstTeamMember->Position = TEAM_POSITION_CAPTAIN;
					iAdjustCaptain++;
					RoleIDCaption = pstTeamMember->RoleID;
				}
			}
			else if (	pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGOUT ||
					pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGOUT_REQ)
			{
				pstTeamMember->Follow = 0;
			}
			else
			{
				continue;
			}
			
			pstTeamMember->OffLineTime = 0;
			
			if(pstPlayer->eStatus == PLAYER_STATUS_ROLE_NOTHING)
			{
				continue;
			}
		
			player_team_sync_member_info(pstEnv, pstTeamMember, pstPlayer);

			//tlog_info(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
			//	"player_team_sync member: %s,eStatusAssistant:%d",pstTeamMember->MemberName, pstPlayer->stOnline.eStatusAssistant);
		}
		else
		{
			//离线的标记还有用
			if (0 == pstTeamMember->OffLineTime)
			{
				pstTeamMember->OffLineTime = pstEnv->pstAppCtx->stCurr.tv_sec;
			}	
		}
	}

	if(iOnlineNum <= 0)
	{
		player_team_destroy_in(pstEnv, pstZoneTeam, 0);
		return 0;
	}

	if(pstZoneTeam->stTeamInfo.MemberNum < MIN_TEAM_MEMBER)
	{
		if(is_battle_pvp_svr(pstEnv) || is_battle_pve_battle_svr(pstEnv) ||
			is_battle_sparta_svr(pstEnv))
		{
			//存在只有一个队员跨到跨服野外的情况
		}
		else
		{
			player_team_destroy_in(pstEnv, pstZoneTeam, 0);
			return 0;
		}
	}

	if(is_battle_pvp_svr(pstEnv) || is_battle_pve_battle_svr(pstEnv) || is_battle_sparta_svr(pstEnv))
	{
		return 0;
	}

	if(iAdjustCaptain>1)
	{
		for(i = pstZoneTeam->stTeamInfo.MemberNum - 1; i >= 0; --i)
		{
			pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[i];
			if(RoleIDCaption && pstTeamMember->RoleID != RoleIDCaption)
			{
				pstTeamMember->Position = TEAM_POSITION_MEMBER;
			}
		}
		player_team_adjust_captain_pos(pstEnv, pstZoneTeam);
	}

	if(NULL == player_team_get_captain(pstZoneTeam) ) 
	{
		pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[0];
		pstTeamMember->Position = TEAM_POSITION_CAPTAIN;
		
		sprintf(szMsg, LJY_TEAM9, pstTeamMember->MemberName);
		player_team_broadcast_tip(pstEnv, pstZoneTeam, szMsg);

		autopworld_invite_enter(pstEnv, NULL,pstZoneTeam->iAutoPworldID, 1, pstZoneTeam,1);

		snprintf(pstZoneTeam->stTeamInfo.TeamName, sizeof(pstZoneTeam->stTeamInfo.TeamName),
				LJY_TEAM3, pstTeamMember->MemberName);
		player_team_all_action(pstEnv, pstZoneTeam);

		pstCaptain = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeamMember->MemberName);
		if(pstCaptain)
		{
			//队长的副本加入队伍副本
			team_member_pworld_update(pstEnv, pstZoneTeam, pstCaptain, 1);
		}

		player_team_adjust_captain_pos(pstEnv, pstZoneTeam);
	}
	
	// 更新契约状态
	player_team_update_qiyue_valid(pstEnv, pstZoneTeam);

	pstTeamSvr->OpType = TEAM_SVR_TEAMINFO;
	memcpy(&pstTeamSvr->TeamSvrData.TeamInfo, &pstZoneTeam->stTeamInfo, sizeof(pstTeamSvr->TeamSvrData.TeamInfo));

	Z_CSHEAD_INIT(&stPkg.Head, TEAM_SVR);
	if( iOnlineNum > 0 )
	{
		z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iOnlineNum, &stPkg);
	}
	
	return 0;
}

// 队伍成员之间共鸣处理
static int player_team_resonance(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam)
{
	int i = 0, iNumPlayer = 0, fFirstApplyed = 0;
	Player *apstPlayer[MAX_TEAM_MEMBER];
	Player *pstPlayer = NULL;
	ZoneStatus *pstZoneStatus = NULL;
	STATUSRESULT stStatusResult;

	i = pstZoneTeam->stTeamInfo.MemberNum;
	while(i--)
	{
		TEAMMEMBER *pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[i];
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
		                          pstTeamMember->MemberName);
		if(pstPlayer)
		{
			apstPlayer[iNumPlayer++] = pstPlayer;
		}
	}
	if(iNumPlayer <= 0)
	{
		return 0;
	}

	for(i = 0; i < iNumPlayer; ++i)
	{
		pstZoneStatus = z_is_player_status_result(pstEnv, apstPlayer[i],
		                                          STATUS_RESULT_RESONANCE);
		if(pstZoneStatus)
		{
			pstPlayer = apstPlayer[i];
			break;
		}
	}
	if(!pstZoneStatus || !pstPlayer)
	{
		return 0;
	}

	z_get_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_RESONANCE,
	                           &stStatusResult, 1);
	while(++i < iNumPlayer)
	{
		if(z_player_find_status(apstPlayer[i], pstZoneStatus->unStatusID) &&
		   z_distance(&pstPlayer->stRoleData.Pos, &apstPlayer[i]->stRoleData.Pos)
		   < stStatusResult.ResultVal3 * 100)
		{
			if(z_player_apply_status(
				pstEnv->pstAppCtx,
				pstEnv,
				apstPlayer[i],
				stStatusResult.ResultVal1,
				stStatusResult.ResultVal2,
				TEAM_RESONANCE_STATUS_TIME,
				1,
				0) < 0)
			{
				return -1;
			}

			if(!fFirstApplyed)
			{
				if(z_player_apply_status(
					pstEnv->pstAppCtx,
					pstEnv,
					pstPlayer,
					stStatusResult.ResultVal1,
					stStatusResult.ResultVal2,
					TEAM_RESONANCE_STATUS_TIME,
					1,
					0) < 0)
				{
					return -1;
				}
				fFirstApplyed = 1;
			}

			pstPlayer = apstPlayer[i];
		}
	}

	return 0;
}

int flush_battle_queue_team_info2(ZONESVRENV* pstEnv, Player *pstPlayer, int iFlag)
{
	int i;
	PworldApplyQueue *pstApplyQueue;
	PworldWaitQueue *pstWaitQueue;
	ZONEPWORLD *pstPworld;
	ZONEIDX stIdx;
	int iFirstTeamIdx = -1;

	stIdx.iID = pstPlayer->stOnline.stPworldQueue.iPworldID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (NULL == pstPworld)
		return 0;

	if (!is_normal_battle_pworld(pstPworld))
	{
		return 0;
	}
	
	if (pstPlayer->stOnline.stPworldQueue.iQueueType == PWORLD_WAIT_QUEUE)
	{
		pstWaitQueue = get_pworld_wait_queue(pstEnv, pstPlayer->stOnline.stPworldQueue.iQueueIdx, 
												pstPlayer->stOnline.stPworldQueue.iPworldInstID);
		if (!pstWaitQueue)
		{
			return 0;
		}

		for (i=0; pstWaitQueue->iQueuerNum; i++)
		{
			if (pstWaitQueue->stQueuerInfo[i].stRoleInfo.iQueuerID == pstPlayer->iMemID)
			{
				if (iFlag)
				{
					pstWaitQueue->stQueuerInfo[i].stRoleInfo.iTeamIdx 		= pstPlayer->stRoleData.MiscInfo.TeamIdx;
					pstWaitQueue->stQueuerInfo[i].stRoleInfo.ullTeamWID 	=  pstPlayer->stRoleData.MiscInfo.TeamID;
				}
				else
				{
					pstWaitQueue->stQueuerInfo[i].stRoleInfo.iTeamIdx 		= 0;
					pstWaitQueue->stQueuerInfo[i].stRoleInfo.ullTeamWID 	= 0;
				}
				break;
			}
		}
	}
	else if(pstPlayer->stOnline.stPworldQueue.iQueueType == PWORLD_APPLY_QUEUE)
	{
		pstApplyQueue = get_pworld_apply_queue(pstEnv, pstPlayer->stOnline.stPworldQueue.iPworldID);
		if (!pstApplyQueue)
			return 0;

		for (i=0; i<pstApplyQueue->iQueuerNum; i++)
		{
			// 退出队列是找到队伍在排队队列中的第一个人
			if (	iFlag == 0 && 
				iFirstTeamIdx < 0 &&
				pstApplyQueue->stQueuerInfo[i].ullTeamWID == pstPlayer->stRoleData.MiscInfo.TeamID)
			{
				iFirstTeamIdx = i;
			}
			
			if (pstApplyQueue->stQueuerInfo[i].iQueuerID == pstPlayer->iMemID)
			{
				if (iFlag)
				{
					pstApplyQueue->stQueuerInfo[i].iTeamIdx 	= pstPlayer->stRoleData.MiscInfo.TeamIdx;
					pstApplyQueue->stQueuerInfo[i].ullTeamWID 	= pstPlayer->stRoleData.MiscInfo.TeamID;
				}
				else
				{
					
					pstApplyQueue->stQueuerInfo[i].iTeamIdx 	= 0;
					pstApplyQueue->stQueuerInfo[i].ullTeamWID 	= 0;
					
					// 退出队伍的人与第一个人 交换位置
					if (iFirstTeamIdx >=0 && iFirstTeamIdx != i)
					{
						int iMemID = pstApplyQueue->stQueuerInfo[iFirstTeamIdx].iQueuerID;
						pstApplyQueue->stQueuerInfo[iFirstTeamIdx].iQueuerID 		= pstPlayer->iMemID;
						pstApplyQueue->stQueuerInfo[iFirstTeamIdx].iTeamIdx 		= 0;
						pstApplyQueue->stQueuerInfo[iFirstTeamIdx].ullTeamWID 	= 0;

						pstApplyQueue->stQueuerInfo[i].iQueuerID 	= iMemID;
						pstApplyQueue->stQueuerInfo[i].iTeamIdx 	= pstPlayer->stRoleData.MiscInfo.TeamIdx;
						pstApplyQueue->stQueuerInfo[i].ullTeamWID 	=  pstPlayer->stRoleData.MiscInfo.TeamID;
					}
				}
				break;
			}
		}
	}
	else if(pstPlayer->stOnline.stPworldQueue.iQueueType == PWORLD_CLAN_QUEUE)
	{
		//todo:
		return 0;
	}
	else
	{
		return 0;	
	}

	return 0;
}

int ss_flush_battle_queue_team_info(ZONESVRENV* pstEnv, SSFLUSHBATTLEQUEUETEAMINFO *pstFlushBattleQueueTeamInfo)
{
	Player *pstPlayer;

	pstPlayer = player_get_by_uin(pstEnv, pstFlushBattleQueueTeamInfo->Member.Uin);
	if(!pstPlayer || pstPlayer->stRoleData.RoleID != pstFlushBattleQueueTeamInfo->Member.RoleID)
	{
		return -1;
	}
	
	pstPlayer->stRoleData.MiscInfo.TeamID = pstFlushBattleQueueTeamInfo->TeamID;
	pstPlayer->stRoleData.MiscInfo.TeamIdx = pstFlushBattleQueueTeamInfo->TeamIdx;
	flush_battle_queue_team_info2(pstEnv, pstPlayer, pstFlushBattleQueueTeamInfo->Flag);
	return 0;
}

// 更新队列中队伍信息
// iFlag=1  入队  0-出队
int flush_battle_queue_team_info(ZONESVRENV* pstEnv, Player *pstPlayer,ZoneTeam *pstZoneTeam, int iFlag)
{
	if (!pstZoneTeam)
		return 0;

	if(INST_MODE_ZONE_SVR == pstEnv->iInstMode &&
		pstEnv->pstConf->BattleFlag)
	{
		if(is_player_apply_battle(pstEnv, pstPlayer))
		{
			SSPKG stPkg;
			SSFLUSHBATTLEQUEUETEAMINFO *pstFlushBattleQueueTeamInfo = &stPkg.Body.FlushBattleQueueTeamInfo;
			
			pstFlushBattleQueueTeamInfo->Member.Uin = pstPlayer->stRoleData.Uin;
			pstFlushBattleQueueTeamInfo->Member.RoleID = pstPlayer->stRoleData.RoleID;
			pstFlushBattleQueueTeamInfo->Flag = iFlag;

			Z_SSHEAD_INIT(&stPkg.Head, FLUSH_BATTLE_QUEUE_TEAM_INFO);
			z_ssmsg_send(pstEnv, &stPkg, pstEnv->iBattlePveBattleID);
		}
	}
	else
	{
		if (pstPlayer->stOnline.stPworldQueue.iPworldID <= 0)
			return 0;
		
		flush_battle_queue_team_info2(pstEnv, pstPlayer, iFlag);
	}

	return 0;
}

int flush_battle_pworld_queue2(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstCapPlayer)
{
	int i;
	PworldApplyQueue *pstApplyQueue;
	int iApplyIdx = -1,iAppCapIdx = -1;
	QueueInfo stQueueInfo;
	
	// 队长没有报名
	if (pstCapPlayer->stOnline.stPworldQueue.iPworldID <= 0)
		return 0;

	// 报的不是同一副本
	if (pstPlayer->stOnline.stPworldQueue.iPworldID != pstCapPlayer->stOnline.stPworldQueue.iPworldID)
		return 0;

	// 队列不一样  不管了 只能拆散队伍了
	if (pstPlayer->stOnline.stPworldQueue.iQueueType != pstCapPlayer->stOnline.stPworldQueue.iQueueType)
		return 0;

	// 都在等待队列 不管了
	if (PWORLD_WAIT_QUEUE == pstPlayer->stOnline.stPworldQueue.iQueueType)
		return 0;

	// 都在报名队列
	// 
	pstApplyQueue = get_pworld_apply_queue(pstEnv, pstPlayer->stOnline.stPworldQueue.iPworldID);
	if(NULL == pstApplyQueue)
	{
		return 0;
	}

	for (i=0; i<pstApplyQueue->iQueuerNum; i++)
	{
		if (pstApplyQueue->stQueuerInfo[i].iQueuerID == pstPlayer->iMemID)
		{
			iApplyIdx = i;
		}

		if (pstApplyQueue->stQueuerInfo[i].iQueuerID == pstCapPlayer->iMemID)
		{
			iAppCapIdx = i;
		}
	}

	if (iApplyIdx < 0 || iAppCapIdx < 0)
		return 0;

	stQueueInfo = pstApplyQueue->stQueuerInfo[iApplyIdx];

	if (iAppCapIdx - iApplyIdx  > 1)
	{
		memmove(&pstApplyQueue->stQueuerInfo[iApplyIdx], &pstApplyQueue->stQueuerInfo[iApplyIdx+1],
				sizeof(pstApplyQueue->stQueuerInfo[0]) * (iAppCapIdx-iApplyIdx-1));

		pstApplyQueue->stQueuerInfo[iAppCapIdx-1] = stQueueInfo;
	}
	else if(iApplyIdx - iAppCapIdx > 1)
	{
		memmove(&pstApplyQueue->stQueuerInfo[iAppCapIdx+2], &pstApplyQueue->stQueuerInfo[iAppCapIdx+1],
				sizeof(pstApplyQueue->stQueuerInfo[0]) * (iApplyIdx-iAppCapIdx-1));
		
		pstApplyQueue->stQueuerInfo[iAppCapIdx+1] = stQueueInfo;
	}
	else
	{
		return 0;
	}
	
	return 0;
}

int ss_flush_battle_pworld_queue(ZONESVRENV* pstEnv, SSFLUSHBATTLEQUEUE  *pstFlushBattleQueue)
{
	Player *pstPlayer, *pstCapPlayer;

	if (pstFlushBattleQueue->Member.Uin && 
		pstFlushBattleQueue->Captain.Uin)
	{
		pstPlayer = player_get_by_uin(pstEnv, pstFlushBattleQueue->Member.Uin);
		if(!pstPlayer || pstPlayer->stRoleData.RoleID != pstFlushBattleQueue->Member.RoleID)
		{
			return -1;
		}

		pstCapPlayer = player_get_by_uin(pstEnv, pstFlushBattleQueue->Captain.Uin);
		if(!pstCapPlayer || pstCapPlayer->stRoleData.RoleID != pstFlushBattleQueue->Captain.RoleID)
		{
			return -1;
		}
		return flush_battle_pworld_queue2(pstEnv, pstPlayer, pstCapPlayer);
	}
	else if (pstFlushBattleQueue->Member.Uin)//单人清队列
	{
		pstPlayer = player_get_by_uin(pstEnv, pstFlushBattleQueue->Member.Uin);
		if(!pstPlayer || pstPlayer->stRoleData.RoleID != pstFlushBattleQueue->Member.RoleID)
		{
			return -1;
		}
		//目前只清掉公会的
		if (pstPlayer->stOnline.stPworldQueue.iQueueType == PWORLD_CLAN_QUEUE)
		{
			z_exit_pworld_queue(pstEnv,pstPlayer,pstPlayer->stOnline.stPworldQueue.iPworldID,1);
			
		}
	}
	else
	{
	}
	
	return 0;
}



int flush_battle_pworld_queue(ZONESVRENV* pstEnv, Player *pstPlayer,ZoneTeam *pstZoneTeam)
{
	Player *pstCapPlayer;
	TEAMMEMBER *pstCap = NULL;

	pstCap = player_team_get_captain(pstZoneTeam);
	if (!pstCap)
		return 0;
	
	pstCapPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstCap->MemberName);
	if (!pstCapPlayer)
		return 0;

	if(INST_MODE_ZONE_SVR == pstEnv->iInstMode &&
		pstEnv->pstConf->BattleFlag)
	{
		if(is_player_apply_battle(pstEnv, pstPlayer))
		{
			SSPKG stPkg;
			SSFLUSHBATTLEQUEUE  *pstFlushBattleQueue = &stPkg.Body.FlushBattleQueue;
			
			pstFlushBattleQueue->Member.Uin = pstPlayer->stRoleData.Uin;
			pstFlushBattleQueue->Member.RoleID = pstPlayer->stRoleData.RoleID;

			pstFlushBattleQueue->Captain.Uin = pstCapPlayer->stRoleData.Uin;
			pstFlushBattleQueue->Captain.RoleID = pstCapPlayer->stRoleData.RoleID;

			Z_SSHEAD_INIT(&stPkg.Head, FLUSH_BATTLE_QUEUE);
			z_ssmsg_send(pstEnv, &stPkg, pstEnv->iBattlePveBattleID);
		}
	}
	else
	{

		// 新加入的队员没有报名
		if (pstPlayer->stOnline.stPworldQueue.iPworldID <= 0)
		{
			return 0;
		}
		
		flush_battle_pworld_queue2(pstEnv, pstPlayer, pstCapPlayer);
	}
	
	return 0;
}

int player_team_apply_accept(ZONESVRENV* pstEnv, Player *pstPlayer, CSTEAMAPPLYACCEPT *pstApplyAccept)
{
	int i, fIsCreated = 0;
	Player *pstPlayerOther = NULL;
	ZoneTeam *pstZoneTeam = NULL;
	char szMsg[256];
	char szBuff[32];
	TEAMMEMBER *pstTeamMember;
	TeamOnline *pstTeamOnline = &pstPlayer->stOnline.stTeamOnline;

	for (i=0; i<pstTeamOnline->unApplyNum; i++)
	{
		if (pstTeamOnline->astApplyEntry[i].ullRoleID == pstApplyAccept->RoleID)
			break;
	}

	if (i == pstTeamOnline->unApplyNum || pstEnv->pstAppCtx->stCurr.tv_sec > pstTeamOnline->astApplyEntry[i].tTime)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM_OP);
		goto fail;
	}

	pstPlayerOther = z_id_player(pstEnv->pstAppCtx, pstEnv, pstTeamOnline->astApplyEntry[i].iMemID);
	if(!pstPlayerOther ||
	   (pstPlayerOther->eStatus != PLAYER_STATUS_ROLE_LOGIN &&
	   pstPlayerOther->eStatus != PLAYER_STATUS_ROLE_NOTHING) || 
	   pstTeamOnline->astApplyEntry[i].ullRoleID !=
	   pstPlayerOther->stRoleData.RoleID)
	{
		pstTeamOnline->astApplyEntry[i].ullRoleID = 0;
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM_OP);
		goto fail;
	}
	
	if (pstPlayerOther->stRoleData.RoleID == pstPlayer->stRoleData.RoleID)
	{
		goto fail;
	}

	if (0 <= player_black_find_byid(&pstPlayerOther->stRoleData.MiscInfo, pstPlayer->stRoleData.RoleID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM125);
		goto fail;
	}

	if (player_enemy_find_byname(&pstPlayerOther->stRoleData.MiscInfo,pstPlayer->stRoleData.RoleName) >= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM126);
		goto fail;
	}

	pstTeamOnline->astApplyEntry[i].ullRoleID = 0;

	if (player_team_get(pstEnv, pstPlayerOther))
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM_OP);
		goto fail;
	}

	if (0 <= player_black_find_byid(&pstPlayerOther->stRoleData.MiscInfo, pstPlayer->stRoleData.RoleID))
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM_OP);
		goto fail;
	}

	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	
	if (pstZoneTeam && pstZoneTeam->stTeamInfo.MemberNum >= MAX_TEAM_MEMBER)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM_OP);
		goto fail;
	}

	if (pstZoneTeam) //加入成员
	{
		if(1 == pstZoneTeam->iRongYanBegin)
		{
			z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM800);
			goto fail;
		}
		
		pstTeamMember = player_team_get_captain(pstZoneTeam);
		if (NULL == pstTeamMember)
		{
			goto fail;
		}

		if (0 != strcmp(pstTeamMember->MemberName, pstPlayer->stRoleData.RoleName))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_TEAM127);
			goto fail;
		}

		snprintf(szMsg, sizeof(szMsg), LJY_TEAM15, pstPlayerOther->stRoleData.RoleName);
		szMsg[sizeof(szMsg) - 1] = 0;
		player_team_broadcast_tip(pstEnv, pstZoneTeam, szMsg);

		pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[pstZoneTeam->stTeamInfo.MemberNum];
		player_team_sync_member_info(pstEnv, pstTeamMember, pstPlayerOther);
		pstZoneTeam->stTeamInfo.MemberNum++;

		flush_battle_queue_team_info(pstEnv, pstPlayerOther,pstZoneTeam, 1);

		if(0 > booty_mode_to_string(pstZoneTeam->stTeamInfo.TeamBootyMode, szBuff, sizeof(szBuff)))
		{
			szBuff[0] = 0;
		}
	}
	else //创建队伍
	{
		pstZoneTeam = player_team_create(pstEnv, pstPlayer);
		if (NULL == pstZoneTeam)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "create team fail");
			goto fail;
		}

		pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[pstZoneTeam->stTeamInfo.MemberNum];
		player_team_sync_member_info(pstEnv, pstTeamMember, pstPlayerOther);
		pstZoneTeam->stTeamInfo.MemberNum++;

		if(0 > booty_mode_to_string(pstZoneTeam->stTeamInfo.TeamBootyMode, szBuff, sizeof(szBuff)))
		{
			szBuff[0] = 0;
		}
		
		//提示
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM113, szBuff);

		fIsCreated = 1;
		
		flush_battle_queue_team_info(pstEnv, pstPlayerOther,pstZoneTeam, 1);
		flush_battle_queue_team_info(pstEnv, pstPlayer,pstZoneTeam, 1);
	}

	if(!pstZoneTeam)
	{
		goto fail;
	}
	pstPlayerOther->stOnline.stTeamOnline.unInviteNum = 0;

	

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayerOther, SYS_MIDDLE2, ZONEERR_TEAM114, 
					      pstPlayer->stRoleData.RoleName, szBuff);

	//队伍发生变化同步分线
	z_team_all_span_sync_req(pstEnv, pstZoneTeam);

	//竞技场清理
	player_sparta_team_chg_clean(pstEnv, pstPlayer);
	
	player_team_info_set(pstEnv, pstPlayerOther, pstZoneTeam);
	player_team_memchg(pstEnv, pstPlayerOther, pstZoneTeam, 1);

	flush_battle_pworld_queue(pstEnv, pstPlayerOther,pstZoneTeam);

	//同步team info
	player_team_sync(pstEnv, pstZoneTeam);
	player_team_all_action(pstEnv, pstZoneTeam);

	if( fIsCreated )
	{
		// 使队长契约成为默认契约
		pstZoneTeam->stTeamInfo.QiYue = 0;
		memset( &pstZoneTeam->stQiYueLastTv, 0, sizeof(pstZoneTeam->stQiYueLastTv) );
	 	player_team_set_qiyue(pstEnv, pstPlayer, pstPlayer->stRoleData.RoleName );

		// 设置默认契约不计算冷却时间
		memset( &pstZoneTeam->stQiYueLastTv, 0, sizeof(pstZoneTeam->stQiYueLastTv) );
	}

	z_team_sys_strf(pstEnv, pstZoneTeam, SYS_TEAM, ZONEERR_TEAM316);

	autopworld_invite_enter(pstEnv, NULL,pstZoneTeam->iAutoPworldID, 1, pstZoneTeam,1);
	return 0;

fail:
	player_team_del_apply(pstTeamOnline, pstApplyAccept->RoleID);
	return -1;
}

int player_team_del_apply(TeamOnline *pstTeamOnline, tdr_ulonglong ullRoleID)
{
	int i;

	for (i=pstTeamOnline->unApplyNum-1; i>=0; i--)
	{
		if (pstTeamOnline->astApplyEntry[i].ullRoleID == ullRoleID)
		{
			pstTeamOnline->unApplyNum--;

			if (i != pstTeamOnline->unApplyNum)
			{
				memmove(&pstTeamOnline->astApplyEntry[i], &pstTeamOnline->astApplyEntry[i+1], 
							(pstTeamOnline->unApplyNum-i)*sizeof(pstTeamOnline->astApplyEntry[0]));
			}
			
			return 0;
		}
	}
	
	return -1;
}

int player_team_apply_refuse(ZONESVRENV* pstEnv, Player *pstPlayer, CSTEAMAPPLYREFUSE *pstApplyRefuse)
{
	int i;
	TeamOnline * pstTeamOnline = &pstPlayer->stOnline.stTeamOnline;

	i = pstPlayer->stOnline.stTeamOnline.unApplyNum;
	while( i-- )
	{
		if( pstTeamOnline->astApplyEntry[i].ullRoleID == pstApplyRefuse->RoleID )
		{
			Player * pstOtherPlayer = z_id_player( pstEnv->pstAppCtx, pstEnv, 
				pstTeamOnline->astApplyEntry[i].iMemID );

			player_team_del_apply(&pstPlayer->stOnline.stTeamOnline, pstApplyRefuse->RoleID);

			if( pstOtherPlayer )
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOtherPlayer, SYS_MIDDLE2, ZONEERR_TEAM115, pstPlayer->stRoleData.RoleName );
			}
		}
	}

	return 0;
}

int player_team_apply_clear(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	TeamOnline * pstTeamOnline = &pstPlayer->stOnline.stTeamOnline;
	UNUSED(pstEnv);

	i = pstPlayer->stOnline.stTeamOnline.unApplyNum;
	while( i-- )
	{
		Player * pstOtherPlayer = z_id_player( pstEnv->pstAppCtx, pstEnv, 
			pstTeamOnline->astApplyEntry[i].iMemID );

		//player_team_del_apply(&pstPlayer->stOnline.stTeamOnline, pstApplyRefuse->RoleID);

		if( pstOtherPlayer )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOtherPlayer, SYS_MIDDLE2, 
								ZONEERR_TEAM115, pstPlayer->stRoleData.RoleName);
		}
		
	}
	pstPlayer->stOnline.stTeamOnline.unApplyNum = 0;
	return 0;
}

int player_team_invite(ZONESVRENV* pstEnv, Player *pstPlayer, CSTEAMINVITE *pstTeamInvite)
{
	CSPKG stPkg;
	CSTEAMSVR *pstTeamSvr = &stPkg.Body.TeamSvr;
	Player *pstPlayerOther;
	ZoneTeam *pstZoneTeam;
	TEAMMEMBER *pstTeamMember;

	pstPlayerOther = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeamInvite->MemberName);
	if (NULL == pstPlayerOther )
	{
		if(1 == is_player_in_battle_pvp_svr(pstEnv, pstPlayer))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SPAN_MAP2,pstTeamInvite->MemberName);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM108);
		}
		return -1;
	}

	if(0 == is_player_in_battle_pvp_svr(pstEnv, pstPlayerOther))
	{
		if(is_player_in_span_svr(pstEnv, pstPlayerOther))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM109);
			return -1;
		}
	}

	if (pstPlayerOther->stRoleData.RoleID == pstPlayer->stRoleData.RoleID)
	{
		return -1;
	}

	if (pstPlayer->stOnline.State & CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM102);
		return -1;
	}

	if (pstPlayerOther->stOnline.State & CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM103);
		return -1;
	}

	if (player_team_get(pstEnv, pstPlayerOther))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM104);
		return -1;
	}

	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	
	if (pstZoneTeam)
	{
		if (pstZoneTeam->stTeamInfo.MemberNum >= MAX_TEAM_MEMBER)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM106);
			return -1;
		}

		pstTeamMember = player_team_get_captain(pstZoneTeam);
		if (NULL == pstTeamMember || 0 != strcmp(pstTeamMember->MemberName, pstPlayer->stRoleData.RoleName))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM110, pstTeamInvite->MemberName);
			return -1;
		}
	}

	if (0 <= player_black_find_byid(&pstPlayer->stRoleData.MiscInfo, pstPlayerOther->stRoleData.RoleID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM111);
		return -1;
	}

	if (player_enemy_find_byname(&pstPlayer->stRoleData.MiscInfo,pstPlayerOther->stRoleData.RoleName) >= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_TEAM112);
		return -1;
	}

	//lzk  融焰怪,	所有玩家组队锁定，只可离开队伍，不可进入队伍
	if(pstZoneTeam && 1 == pstZoneTeam->iRongYanBegin)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM801);
		return -1;
	}
	
	if (0 > player_team_insert_invite(pstEnv, pstPlayerOther, pstPlayer))
	{
		return -1;
	}

	pstTeamSvr->OpType = TEAM_SVR_INVITE;

	if (pstZoneTeam)
	{
		memcpy(&pstTeamSvr->TeamSvrData.InviteInfo, &pstZoneTeam->stTeamInfo, sizeof(pstTeamSvr->TeamSvrData.InviteInfo));
	}
	else
	{
		memset(&pstTeamSvr->TeamSvrData.InviteInfo, 0, sizeof(pstTeamSvr->TeamSvrData.InviteInfo));
		pstTeamMember = &pstTeamSvr->TeamSvrData.InviteInfo.TeamMember[0];
		player_team_sync_member_info(pstEnv, pstTeamMember, pstPlayer);
		pstTeamMember->Position = TEAM_POSITION_CAPTAIN;
		pstTeamSvr->TeamSvrData.InviteInfo.MemberNum = 1;
	}

	Z_CSHEAD_INIT(&stPkg.Head, TEAM_SVR);
	z_cltmsg_send(pstEnv, pstPlayerOther, &stPkg);
	
	return 0;
}

int player_team_invite_accept(ZONESVRENV* pstEnv, Player *pstPlayer, CSTEAMINVITEACCEPT *pstInviteAccept)
{
	int i, fIsCreated = 0;
	Player *pstPlayerOther;
	ZoneTeam *pstZoneTeam;
	char szBuff[32];
	TEAMMEMBER *pstTeamMember;
	TeamOnline *pstTeamOnline = &pstPlayer->stOnline.stTeamOnline;
	
	if (player_team_get(pstEnv, pstPlayer))
	{
		goto fail;
	}

	pstPlayerOther = z_name_player(pstEnv->pstAppCtx, pstEnv, pstInviteAccept->CaptainName);
	if (NULL == pstPlayerOther)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM_OP);
		goto fail;
	}
	
	if (0 <= player_black_find_byid(&pstPlayerOther->stRoleData.MiscInfo, pstPlayer->stRoleData.RoleID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM125);
		goto fail;
	}

	if (player_enemy_find_byname(&pstPlayerOther->stRoleData.MiscInfo,pstPlayer->stRoleData.RoleName) >= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM126);
		goto fail;
	}

	if (pstPlayerOther->stRoleData.RoleID == pstPlayer->stRoleData.RoleID)
	{
		goto fail;
	}

	pstZoneTeam = player_team_get(pstEnv, pstPlayerOther);
	if (pstZoneTeam && pstZoneTeam->stTeamInfo.MemberNum >= MAX_TEAM_MEMBER)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_TEAM106);
		goto fail;
	}

	for (i=0; i<pstTeamOnline->unInviteNum; i++)
	{
		if (pstTeamOnline->astInviteEntry[i].ullRoleID == pstPlayerOther->stRoleData.RoleID)
			break;
	}

	if (i == pstTeamOnline->unInviteNum || pstEnv->pstAppCtx->stCurr.tv_sec > pstTeamOnline->astInviteEntry[i].tTime)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM_OP);
		goto fail;
	}

	pstTeamOnline->astInviteEntry[i].ullRoleID = 0;
	pstTeamOnline->astInviteEntry[i].iMemID = 0;

	if (0 <= player_black_find_byid(&pstPlayerOther->stRoleData.MiscInfo, pstPlayer->stRoleData.RoleID))
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM_OP);
		goto fail;
	}

	if (pstZoneTeam) //加入成员
	{
		if(1 == pstZoneTeam->iRongYanBegin)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM801);
			goto fail;
		}
		
		pstTeamMember = player_team_get_captain(pstZoneTeam);
		if (NULL == pstTeamMember || 0 != strcmp(pstTeamMember->MemberName, pstPlayerOther->stRoleData.RoleName))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM116,pstPlayerOther->stRoleData.RoleName);
			goto fail;
		}

		flush_battle_queue_team_info(pstEnv, pstPlayer,pstZoneTeam, 1);

		if(0 > booty_mode_to_string(pstZoneTeam->stTeamInfo.TeamBootyMode, szBuff, sizeof(szBuff)))
		{
			szBuff[0] = 0;
		}
	}
	else //创建队伍
	{
		pstZoneTeam = player_team_create(pstEnv, pstPlayerOther);
		if (NULL == pstZoneTeam)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "create team fail");
			goto fail;
		}

		if(0 > booty_mode_to_string(pstZoneTeam->stTeamInfo.TeamBootyMode, szBuff, sizeof(szBuff)))
		{
			szBuff[0] = 0;
		}

		//提示
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayerOther, SYS_MIDDLE2, 
							ZONEERR_TEAM113, szBuff);

		fIsCreated = 1;

		flush_battle_queue_team_info(pstEnv, pstPlayerOther,pstZoneTeam, 1);
		flush_battle_queue_team_info(pstEnv, pstPlayer,pstZoneTeam, 1);
	}

	pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[pstZoneTeam->stTeamInfo.MemberNum];
	player_team_sync_member_info(pstEnv, pstTeamMember, pstPlayer);
	pstZoneTeam->stTeamInfo.MemberNum++;

	for (i=0; i<pstTeamOnline->unInviteNum; i++)
	{
		Player *pstTmp;
		pstTmp = z_id_player(pstEnv->pstAppCtx, pstEnv, pstTeamOnline->astInviteEntry[i].iMemID);
		if (pstTmp &&pstTmp != pstPlayerOther)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTmp, SYS_MIDDLE2, ZONEERR_TEAM116,pstPlayer->stRoleData.RoleName);
		}		
	}

	pstPlayer->stOnline.stTeamOnline.unInviteNum = 0;
	
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
						ZONEERR_TEAM114, pstPlayerOther->stRoleData.RoleName, szBuff);

	//队伍发生变化同步分线
	z_team_all_span_sync_req(pstEnv, pstZoneTeam);
	//竞技场清理
	player_sparta_team_chg_clean(pstEnv, pstPlayerOther);
	
	player_team_info_set(pstEnv, pstPlayer, pstZoneTeam);
	
	player_team_memchg(pstEnv, pstPlayer, pstZoneTeam, 1);
	flush_battle_pworld_queue(pstEnv, pstPlayer,pstZoneTeam);

	player_team_sync(pstEnv, pstZoneTeam);
	player_team_all_action(pstEnv, pstZoneTeam);
	z_team_sys_strf(pstEnv, pstZoneTeam, SYS_TEAM, ZONEERR_TEAM316);

	if( fIsCreated )
	{
		// 使队长契约成为默认契约
		memset( &pstZoneTeam->stQiYueLastTv, 0, sizeof(pstZoneTeam->stQiYueLastTv) );
		pstZoneTeam->stTeamInfo.QiYue = 0;
	 	player_team_set_qiyue(pstEnv, pstPlayerOther, pstPlayerOther->stRoleData.RoleName );

		// 默认设置的契约不计算冷却时间
		memset( &pstZoneTeam->stQiYueLastTv, 0, sizeof(pstZoneTeam->stQiYueLastTv) );
	}

	autopworld_invite_enter(pstEnv, NULL,pstZoneTeam->iAutoPworldID, 1, pstZoneTeam,1);
	return 0;

fail:
	for (i=pstTeamOnline->unInviteNum-1; i>=0; i--)
	{
		pstPlayerOther = z_id_player(pstEnv->pstAppCtx, pstEnv, pstTeamOnline->astInviteEntry[i].iMemID);
		if (pstPlayerOther &&0 == strcmp(pstPlayerOther->stRoleData.RoleName, pstInviteAccept->CaptainName))
		{
			pstTeamOnline->unInviteNum--;
			if (i != pstTeamOnline->unInviteNum)
			{
				memmove(&pstTeamOnline->astInviteEntry[i], &pstTeamOnline->astInviteEntry[i+1], 
							(pstTeamOnline->unInviteNum-i) * sizeof(pstTeamOnline->astInviteEntry[0]));
			}
		}
	}
	return -1;
}

int player_team_invite_refuse(ZONESVRENV* pstEnv, Player *pstPlayer, CSTEAMINVITEREFUSE *pstInviteRefuse)
{
	int i;
	Player *pstPlayerOther;
	TeamOnline * pstTeamOnline = &pstPlayer->stOnline.stTeamOnline;

	for (i=pstTeamOnline->unInviteNum-1; i>=0; i--)
	{
		pstPlayerOther = z_id_player(pstEnv->pstAppCtx, pstEnv, pstTeamOnline->astInviteEntry[i].iMemID);
		if (NULL == pstPlayerOther || 
		    (PLAYER_STATUS_ROLE_LOGIN != pstPlayerOther->eStatus &&
		    PLAYER_STATUS_ROLE_NOTHING != pstPlayerOther->eStatus))
		{
			pstTeamOnline->astInviteEntry[i].ullRoleID = 0;
		}
		else if (0 == strcmp(pstPlayerOther->stRoleData.RoleName, pstInviteRefuse->CaptainName))
		{
			pstTeamOnline->unInviteNum--;
			if (i != pstTeamOnline->unInviteNum)
			{
				memmove(&pstTeamOnline->astInviteEntry[i], &pstTeamOnline->astInviteEntry[i+1], 
							(pstTeamOnline->unInviteNum-i) * sizeof(pstTeamOnline->astInviteEntry[0]));
			}

			if (pstInviteRefuse->Reason == 1)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayerOther, SYS_MIDDLE2, ZONEERR_TEAM117,
									pstPlayer->stRoleData.RoleName);
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayerOther, SYS_MIDDLE2, ZONEERR_TEAM115, 
									pstPlayer->stRoleData.RoleName);
			}
			return 0;
		}
	}
	
	return -1;
}

int player_team_invite_clear(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	UNUSED(pstEnv);
	
	pstPlayer->stOnline.stTeamOnline.unInviteNum = 0;
	return 0;
}

int player_team_captain_chg(ZONESVRENV* pstEnv, Player *pstPlayer, CSTEAMCAPTAINCHG *pstCaptainChg)
{
	ZoneTeam *pstZoneTeam;
	TEAMMEMBER *pstTeamMember;
	TEAMMEMBER *pstTeamMemberCap;
	TEAMMEMBER stTmp;
	Player *pstPlayerOther;
	int iIdx;
	char szMsg[CLT_MSG_LEN];
	char TeamName[32];
	CSPKG stPkg;
	CSTEAMSVR *pstTeamSvr = &stPkg.Body.TeamSvr;

	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if (NULL == pstZoneTeam)
	{
		return -1;
	}

	pstTeamMemberCap = player_team_get_captain(pstZoneTeam);
	if (NULL == pstTeamMemberCap || 0 != strcmp(pstTeamMemberCap->MemberName, pstPlayer->stRoleData.RoleName))
	{
		return -1;
	}
	
	pstPlayerOther = z_name_player(pstEnv->pstAppCtx, pstEnv, pstCaptainChg->MemberName);
	if (NULL == pstPlayerOther)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM118);
		return -1;
	}

	if (pstPlayerOther == pstPlayer)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM119);
		return -1;
	}

	pstTeamMember = player_team_get_member(pstZoneTeam, pstCaptainChg->MemberName, &iIdx);
	if (NULL == pstTeamMember)
	{
		return -1;
	}

	pstTeamMemberCap->Position = TEAM_POSITION_MEMBER;
	pstTeamMember->Position = TEAM_POSITION_CAPTAIN;

	memcpy(&stTmp, pstTeamMember, sizeof(stTmp));
	if (iIdx != pstZoneTeam->stTeamInfo.MemberNum - 1)
	{
		memmove(pstTeamMember, pstTeamMember+1, (pstZoneTeam->stTeamInfo.MemberNum - 1 - iIdx) * sizeof(*pstTeamMember));
	}

	memmove(&pstZoneTeam->stTeamInfo.TeamMember[1], &pstZoneTeam->stTeamInfo.TeamMember[0], (pstZoneTeam->stTeamInfo.MemberNum - 1) * sizeof(*pstTeamMember));
	memcpy(&pstZoneTeam->stTeamInfo.TeamMember[0], &stTmp, sizeof(pstZoneTeam->stTeamInfo.TeamMember[0]));

	sprintf(szMsg,  LJY_TEAM2, pstCaptainChg->MemberName);
	snprintf(TeamName, sizeof(TeamName), LJY_TEAM3, pstPlayer->stRoleData.RoleName);
	if (0 == strcmp(TeamName, pstZoneTeam->stTeamInfo.TeamName))
	{
		snprintf(pstZoneTeam->stTeamInfo.TeamName, sizeof(pstZoneTeam->stTeamInfo.TeamName),
			     LJY_TEAM3, pstPlayerOther->stRoleData.RoleName);
	}

	//队长的副本加入队伍副本
	team_member_pworld_update(pstEnv, pstZoneTeam, pstPlayerOther, 1);

	Z_CSHEAD_INIT(&stPkg.Head, TEAM_SVR);
	pstTeamSvr->OpType = TEAM_SVR_MSG;
	STRNCPY(pstTeamSvr->TeamSvrData.TeamMsg.Msg, szMsg, sizeof(pstTeamSvr->TeamSvrData.TeamMsg.Msg));
	//给队友其他人发
	player_team_broadcast_except(pstEnv, pstZoneTeam, &stPkg,pstPlayerOther);
	//给自己发
	memset(szMsg,0,sizeof(szMsg));
	memset(pstTeamSvr->TeamSvrData.TeamMsg.Msg, 0,sizeof(pstTeamSvr->TeamSvrData.TeamMsg.Msg));
	snprintf(szMsg,sizeof(szMsg),  LJY_TEAM7);
	STRNCPY(pstTeamSvr->TeamSvrData.TeamMsg.Msg, szMsg, sizeof(pstTeamSvr->TeamSvrData.TeamMsg.Msg));
	z_cltmsg_send(pstEnv, pstPlayerOther,  &stPkg);
	
	player_team_adjust_captain_pos(pstEnv, pstZoneTeam);
	player_team_all_action(pstEnv, pstZoneTeam);

	autopworld_invite_enter(pstEnv, NULL,pstZoneTeam->iAutoPworldID, 1, pstZoneTeam,1);

	//队伍发生变化同步分线
	z_team_all_span_sync_req(pstEnv, pstZoneTeam);

	return 0;
}

//设置用户队伍信息
int player_team_info_set(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneTeam *pstTeam)
{
	ROLEMISCINFO *pstRoleMiscInfo = &pstPlayer->stRoleData.MiscInfo;

	if(pstTeam)
	{
		pstRoleMiscInfo->TeamID = pstTeam->stTeamInfo.TeamID;
		pstRoleMiscInfo->TeamIdx = pstTeam->iTeamIdx;
	}
	else
	{
		pstRoleMiscInfo->TeamID = 0;
		pstRoleMiscInfo->TeamIdx = 0;
	}

	player_protect_data_sync_req(pstEnv, pstPlayer);
	return 0;
}

int player_team_exit(ZONESVRENV* pstEnv, Player *pstPlayer,ZoneTeam *pstZoneTeamTmp)
{
	ZoneTeam *pstZoneTeam;
	TEAMMEMBER *pstTeamMember;
	int iIdx;
	CSPKG stPkg;
	ZONEPWORLDINST *pstPworldInst;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	char szMsg[CLT_MSG_LEN];
	char TeamName[32];
	char cIsCap = 0;
	int i;

	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if (NULL == pstZoneTeam)
	{
		pstActionRes->ID = pstPlayer->iMemID;
		pstActionRes->ActionID = ACTION_TEAM;
		pstActionRes->Data.Team.Type = 1;
		pstActionRes->Data.Team.TeamID = 0;
		pstActionRes->Data.Team.TeamNum = 0;
		STRNCPY(pstActionRes->Data.Team.TeamName, pstPlayer->stRoleData.RoleName,
				CCH(pstActionRes->Data.Team.TeamName));
		pstActionRes->Data.Team.Captain = 0;
		player_action_res_ex(pstEnv, pstPlayer, &stPkg);

		player_team_info_set(pstEnv, pstPlayer, NULL);
		return -1;
	}
		
	pstTeamMember = player_team_get_member(pstZoneTeam, pstPlayer->stRoleData.RoleName, &iIdx);
	if (NULL == pstTeamMember)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
					"player_team_exit faile roleName %s ", pstPlayer->stRoleData.RoleName);
		pstPlayer->stRoleData.MiscInfo.TeamID = 0;
		return -1;
	}

	if (TEAM_POSITION_CAPTAIN == pstTeamMember->Position)
	{
		cIsCap = 1;	
	}
	
	if (pstZoneTeam->stTeamInfo.MemberNum <= MIN_TEAM_MEMBER)
	{
		player_team_destroy_in(pstEnv, pstZoneTeam, 0);
		return 0;
	}

	autopworld_invite_enter(pstEnv, NULL,pstZoneTeam->iAutoPworldID, 1, pstZoneTeam,1);
	
	pstZoneTeam->stTeamInfo.MemberNum--;
	if (iIdx != pstZoneTeam->stTeamInfo.MemberNum)
	{
		memmove(pstTeamMember, pstTeamMember+1,  (pstZoneTeam->stTeamInfo.MemberNum - iIdx) * sizeof(*pstTeamMember));
	}

	player_team_info_set(pstEnv, pstPlayer, NULL);
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM128);
	
	pstPlayer->stOnline.stTeamFollow.cTeamFollow = 0;
	sprintf(szMsg, LJY_TEAM16, pstPlayer->stRoleData.RoleName);
	player_team_broadcast_tip(pstEnv, pstZoneTeam, szMsg);

	player_leave_team_clean_buff(pstEnv, pstPlayer, STATUS_RESULT_QIXINXIELI);

	if (cIsCap)
	{
		Player *pstCaptain = NULL;
		int iMemIdx = 0;

		for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
		{
			pstCaptain =  z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
			if (pstCaptain)
			{
				iMemIdx = i;
				break;
			}
			pstCaptain = NULL;
		}

		pstZoneTeam->stTeamInfo.TeamMember[iMemIdx].Position = TEAM_POSITION_CAPTAIN;
		sprintf(szMsg, LJY_TEAM9, pstZoneTeam->stTeamInfo.TeamMember[iMemIdx].MemberName);
		if(pstCaptain)
		{
			player_team_broadcast_tip1(pstEnv,pstZoneTeam, szMsg, pstCaptain, LJY_TEAM7);
		}
		else
		{
			player_team_broadcast_tip1(pstEnv,pstZoneTeam, szMsg, NULL, LJY_TEAM7);
		}
		
		snprintf(TeamName, sizeof(TeamName), LJY_TEAM3, pstPlayer->stRoleData.RoleName);
		if (0 == strcmp(TeamName, pstZoneTeam->stTeamInfo.TeamName))
		{
			snprintf(pstZoneTeam->stTeamInfo.TeamName, sizeof(pstZoneTeam->stTeamInfo.TeamName),
				     LJY_TEAM3, pstZoneTeam->stTeamInfo.TeamMember[iMemIdx].MemberName);
		}

		if (pstCaptain)
		{
			team_member_pworld_update(pstEnv, pstZoneTeam, pstCaptain, 1);
		}

		player_team_adjust_captain_pos(pstEnv, pstZoneTeam);
		player_team_all_action(pstEnv, pstZoneTeam);
	}

	flush_battle_queue_team_info(pstEnv, pstPlayer,pstZoneTeam, 0);

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_TEAM;
	pstActionRes->Data.Team.Type = 1;
	pstActionRes->Data.Team.TeamID = pstZoneTeam->stTeamInfo.TeamID;
	pstActionRes->Data.Team.TeamNum = pstZoneTeam->stTeamInfo.MemberNum;
	STRNCPY(pstActionRes->Data.Team.TeamName, pstZoneTeam->stTeamInfo.TeamName,
			CCH(pstActionRes->Data.Team.TeamName));
	pstActionRes->Data.Team.Captain = 0;
	player_action_res_ex(pstEnv, pstPlayer, &stPkg);
		
	player_team_sync(pstEnv, pstZoneTeam);

	//竞技场清理
	player_sparta_team_chg_clean(pstEnv, pstPlayer);
	
	//队伍发生变化同步分线
	z_team_all_span_sync_req(pstEnv, pstZoneTeam);

	//清理马上得乘客
	if (pstPlayer->stOnline.stRideOnInfo.iPassengerNum && 
		(pstPlayer->stOnline.State & (CS_STAT_RIDE_ON | CS_STAT_FLY)))
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
	//搭乘的下马
	else if (pstPlayer->stOnline.State & CS_STAT_HITCH_RIDE)
	{
		z_ride_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	//自己在队伍的副本里就把自己T出去
	if (pstPlayer->stRoleData.Map > PWORLD_MAPID_BASE)
	{
		ZONEMAPINST *pstMapInst; 
		ZONEIDX stIdx;
			
		stIdx.iID = pstPlayer->stRoleData.Map;
		stIdx.iPos = pstPlayer->stOnline.iMapPos;
		pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
		if (pstMapInst)
		{
			pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
			if (pstPworldInst && pstPworldInst->iType == PWORLD_TYPE_TEAM)
			{
				player_pworld_exit_chgmap(pstEnv, pstPlayer);
				pstPlayer->stRoleData.MiscInfo.PworldInstID = 0;
			}
		}
	}

	player_team_memchg(pstEnv, pstPlayer, pstZoneTeam, 0);

	z_team_convoy_task_fail(pstEnv, pstPlayer);
	return 0;
	UNUSED(pstZoneTeamTmp);	//add by paraunused.pl
}

//投票
int player_team_kick_vote_svr(ZONESVRENV* pstEnv, Player *pstPlayer, TEAMMEMBER *pstTeamMemberKick)
{
	ZoneTeam *pstZoneTeam = NULL;
	TEAMMEMBER *pstTeamMember;
	int iCurrTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if (NULL == pstZoneTeam)
	{
		return -1;
	}

	if(pstZoneTeam->stTeamInfo.KickVoteStartTime>0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM120);
		return -1;
	}
	
	pstTeamMember = player_team_get_member(pstZoneTeam, pstPlayer->stRoleData.RoleName, NULL);
	if (NULL == pstTeamMember)
	{
		return -1;
	}
	
	if(pstTeamMember->KickStartTime+TEAM_KICK_INTERVAL_TIME > iCurrTime)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM121);
		return -1;
	}

	pstZoneTeam->stTeamInfo.KickMemberUin = pstTeamMemberKick->Uin;
	pstTeamMember->KickStartTime = iCurrTime;
	pstZoneTeam->stTeamInfo.KickVoteStartTime = iCurrTime;
	pstTeamMember->VoteFlag = 1;
		
	player_team_sync(pstEnv,pstZoneTeam);
	
	return 0;
}

int player_team_kick_vote_clear(ZONESVRENV* pstEnv,ZoneTeam *pstZoneTeam)
{
	int i = 0;
	TEAMMEMBER *pstTeamMember = NULL;
	
	pstZoneTeam->stTeamInfo.KickVoteStartTime = 0;
	pstZoneTeam->stTeamInfo.KickMemberUin = 0;
	for(i = pstZoneTeam->stTeamInfo.MemberNum - 1; i >= 0; --i)
	{
		pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[i];
		pstTeamMember->VoteFlag = 0;
	}

	return 0;
	UNUSED(pstEnv);
}
int player_team_kick_vote_tick(ZONESVRENV* pstEnv,ZoneTeam *pstZoneTeam)
{
	int i = 0;
	int VoteSucc = 0;
	int iCurrTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	TEAMMEMBER *pstTeamMember = NULL;
	Player *pstPlayer = NULL;
	int iDo = 0; // 投过票的数量
	TEAMMEMBER *pstKick = NULL;
	float f = 0.0;

	if( pstZoneTeam->stTeamInfo.KickVoteStartTime <= 0 ||
		iCurrTime <= pstZoneTeam->stTeamInfo.KickVoteStartTime + TEAM_KICK_VOTE_VALID_TIME)
	{
		return -1;
	}
	
	for(i = pstZoneTeam->stTeamInfo.MemberNum - 1; i >= 0; --i)
	{
		pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[i];
		if (pstTeamMember->VoteFlag > 0)
		{
			iDo ++;
			if(pstTeamMember->VoteFlag == 1)
			{
				VoteSucc++;
			}
		}

		if(pstZoneTeam->stTeamInfo.KickMemberUin == pstTeamMember->Uin)
		{
			pstKick = pstTeamMember;
		}
	}

	if (pstKick == NULL)
	{
		return -1;
	}

	f = 100.0*VoteSucc/iDo;
	
	if(f >= 60)
	{
		for(i = pstZoneTeam->stTeamInfo.MemberNum - 1; i >= 0; --i)
		{
			pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[i];
			pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeamMember->MemberName);
			if(pstKick != pstTeamMember && pstPlayer)
			{
				CSTEAMKICK stTeamKick;
				memset(&stTeamKick,0,sizeof(stTeamKick));
				STRNCPY(stTeamKick.MemberName,pstKick->MemberName,CCH(stTeamKick.MemberName));

				player_team_kick_vote_clear(pstEnv,pstZoneTeam);
				player_team_kick(pstEnv, pstPlayer,&stTeamKick,1);
				return 0;
			}	
		}
		
	}
	else
	{
		for(i = pstZoneTeam->stTeamInfo.MemberNum - 1; i >= 0; --i)
		{
			pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[i];
			if(pstKick == pstTeamMember)
			{
				continue;
			}
			pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeamMember->MemberName);
			if(pstPlayer)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM129,pstKick->MemberName);
			}
		}
	}

	player_team_kick_vote_clear(pstEnv,pstZoneTeam);
	player_team_sync(pstEnv,pstZoneTeam);
	return 0;
}

int player_team_kick_vote_clt(ZONESVRENV* pstEnv, Player *pstPlayer, CSTEAMVOTEKICK *pstTeamVoteKick)
{
	ZoneTeam *pstZoneTeam = NULL;
	TEAMMEMBER *pstTeamMember = NULL;
	TEAMMEMBER *pstKickTeamMember = NULL;
	int i=0;
	int VoteSucc = 0;
	
	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if (NULL == pstZoneTeam)
	{
		return -1;
	}

	if(pstZoneTeam->stTeamInfo.KickVoteStartTime<=0)
	{
		//无效了 不处理
		return -1;
	}
	
	pstTeamMember = player_team_get_member(pstZoneTeam, pstPlayer->stRoleData.RoleName, NULL);
	if (NULL == pstTeamMember)
	{
		return -1;
	}

	if(pstTeamMember->VoteFlag > 0)
	{
		//已经投过票了
		return -1;
	}
	pstTeamMember->VoteFlag = pstTeamVoteKick->VoteFlag;

	for(i = pstZoneTeam->stTeamInfo.MemberNum - 1; i >= 0; --i)
	{
		pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[i];

		if(pstTeamMember->VoteFlag == 1)
		{
			VoteSucc++;
		}
		if(pstZoneTeam->stTeamInfo.KickMemberUin == pstTeamMember->Uin)
		{
			pstKickTeamMember = pstTeamMember;
		}
	} 

	if (pstKickTeamMember == NULL)
	{
		player_team_kick_vote_clear(pstEnv,pstZoneTeam);
		return -1;
	}

	if( 	(pstZoneTeam->stTeamInfo.MemberNum == 6 && VoteSucc>=3) ||
		(pstZoneTeam->stTeamInfo.MemberNum == 5 && VoteSucc>=3) ||
		(pstZoneTeam->stTeamInfo.MemberNum == 4 && VoteSucc>=2) ||
		(pstZoneTeam->stTeamInfo.MemberNum == 3 && VoteSucc>=2) )
	{
		CSTEAMKICK stTeamKick;
		memset(&stTeamKick,0,sizeof(stTeamKick));
		STRNCPY(stTeamKick.MemberName,pstKickTeamMember->MemberName,CCH(stTeamKick.MemberName));

		player_team_kick_vote_clear(pstEnv,pstZoneTeam);
		player_team_kick(pstEnv, pstPlayer,&stTeamKick,1);
		return 0;
	}
	
	player_team_sync(pstEnv,pstZoneTeam);
	
	return 0;
}

// 0表示正常流程 1表示投票踢人
int player_team_kick(ZONESVRENV* pstEnv, Player *pstPlayer, CSTEAMKICK *pstTeamKick,int OperType)
{
	ZoneTeam *pstZoneTeam;
	TEAMMEMBER *pstTeamMember;
	int iIdx;
	Player *pstPlayerOther;
	TEAMMEMBER *pstTeamMemberOther;
	char szMsg[CLT_MSG_LEN];

	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if (NULL == pstZoneTeam)
	{
		return -1;
	}
	
	pstPlayerOther = z_name_player(pstEnv->pstAppCtx,  pstEnv, pstTeamKick->MemberName);
	pstTeamMemberOther = player_team_get_member(pstZoneTeam, pstTeamKick->MemberName, NULL);
	if(!pstTeamMemberOther)
	{
		return -1;
	}

	if(pstPlayerOther)
	{
		if(check_team_booty_roll(pstEnv, pstPlayerOther,pstZoneTeam))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_TEAM130);
			return -1;
		}
	}
	
	if(OperType==0 )
	{
		pstTeamMember = player_team_get_captain(pstZoneTeam);
		if (0 == strcmp(pstPlayer->stRoleData.RoleName, pstTeamKick->MemberName))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_TEAM131);
			return -1;
		}
		
		if (0 != strcmp(pstPlayer->stRoleData.RoleName, pstTeamMember->MemberName))
		{
			if(pstZoneTeam->stTeamInfo.MemberNum > 2 )
			{
				//走投票流程
				player_team_kick_vote_svr(pstEnv, pstPlayer, pstTeamMemberOther);
				return 0;
			}
		}
		else
		{
			if(pstTeamMemberOther->MapID >= PWORLD_MAPID_BASE &&
			   pstZoneTeam->stTeamInfo.MemberNum > 2 )
			{
				//走投票流程
				player_team_kick_vote_svr(pstEnv, pstPlayer, pstTeamMemberOther);
				return 0;
			}
		}
	
	}

	pstTeamMember = player_team_get_member(pstZoneTeam, pstTeamKick->MemberName, &iIdx);
	if (NULL == pstTeamMember)
	{
		return -1;
	}

	

	sprintf(szMsg, LJY_TEAM17, pstTeamKick->MemberName);
	if(pstPlayerOther)
	{
		pstPlayerOther->stOnline.stTeamFollow.cTeamFollow = 0;
		player_leave_team_clean_buff(pstEnv, pstPlayerOther, STATUS_RESULT_QIXINXIELI);
		player_team_broadcast_tip1(pstEnv, pstZoneTeam, szMsg, pstPlayerOther, LJY_TEAM18);
	}
	else
	{
		player_team_broadcast_tip1(pstEnv, pstZoneTeam, szMsg, NULL, LJY_TEAM18);
	}
	
	if (pstZoneTeam->stTeamInfo.MemberNum <= MIN_TEAM_MEMBER)
	{
		player_team_destroy_in(pstEnv, pstZoneTeam, 0);
		return 0;
	}

	autopworld_invite_enter(pstEnv, NULL,pstZoneTeam->iAutoPworldID, 1, pstZoneTeam,1);

	pstZoneTeam->stTeamInfo.MemberNum--;
	if (iIdx != pstZoneTeam->stTeamInfo.MemberNum)
	{
		memmove(pstTeamMember, pstTeamMember+1,  (pstZoneTeam->stTeamInfo.MemberNum - iIdx) * sizeof(*pstTeamMember));
	}

	player_team_sync(pstEnv, pstZoneTeam);

	//竞技场清理
	player_sparta_team_chg_clean(pstEnv, pstPlayer);
	
	//队伍发生变化同步分线
	z_team_all_span_sync_req(pstEnv, pstZoneTeam);
	
	if (pstPlayerOther)
	{
		CSPKG stPkg;
		CSACTIONRES *pstActionRes;
		ZONEPWORLDINST *pstPworldInst;

		player_team_info_set(pstEnv, pstPlayerOther, NULL);

		pstActionRes = &stPkg.Body.ActionRes;
		pstActionRes->ID = pstPlayerOther->iMemID;
		pstActionRes->ActionID = ACTION_TEAM;
		pstActionRes->Data.Team.Type = 1;
		pstActionRes->Data.Team.TeamID = pstZoneTeam->stTeamInfo.TeamID;
		pstActionRes->Data.Team.TeamNum = pstZoneTeam->stTeamInfo.MemberNum;;
		STRNCPY(pstActionRes->Data.Team.TeamName, pstZoneTeam->stTeamInfo.TeamName,
		        CCH(pstActionRes->Data.Team.TeamName));
		pstActionRes->Data.Team.Captain = 0;
		player_action_res_ex(pstEnv, pstPlayerOther, &stPkg);

		//清理马上得乘客
		if (pstPlayerOther->stOnline.stRideOnInfo.iPassengerNum && 
			(pstPlayerOther->stOnline.State & (CS_STAT_RIDE_ON | CS_STAT_FLY)))
		{
			int i;
			Player *pstTmpPlayer;

			for(i = 0; i<pstPlayerOther->stOnline.stRideOnInfo.iPassengerNum; i++)
			{
				pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayerOther->stOnline.stRideOnInfo.PassengerID[i]);
				{
					continue;
				}

				//客人下马
				z_hitch_ride_clear(pstEnv, pstTmpPlayer);
			}
		}
		//搭乘的下马
		else if (pstPlayerOther->stOnline.State & CS_STAT_HITCH_RIDE)
		{
			z_ride_break(pstEnv->pstAppCtx, pstEnv, pstPlayerOther);
		}

		//自己在队伍的副本里就把自己T出去
		if (pstPlayerOther->stRoleData.Map > PWORLD_MAPID_BASE)
		{
			ZONEMAPINST *pstMapInst; 
			ZONEIDX stIdx;
				
			stIdx.iID = pstPlayerOther->stRoleData.Map;
			stIdx.iPos = pstPlayerOther->stOnline.iMapPos;
			pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
			if (pstMapInst)
			{
				pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
				if (pstPworldInst && pstPworldInst->iType == PWORLD_TYPE_TEAM)
				{
					player_pworld_exit_chgmap(pstEnv, pstPlayerOther);
					pstPlayerOther->stRoleData.MiscInfo.PworldInstID = 0;
				}
			}
		}

		player_team_memchg(pstEnv, pstPlayerOther, pstZoneTeam, 0);

		flush_battle_queue_team_info(pstEnv, pstPlayer,pstZoneTeam, 0);

		z_team_convoy_task_fail(pstEnv, pstPlayerOther);
	}

	return 0;
}

int player_team_destroy(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZoneTeam *pstZoneTeam;
	TEAMMEMBER *pstTeamMember;
		
	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if (NULL == pstZoneTeam)
	{
		return -1;
	}

	pstTeamMember = player_team_get_captain(pstZoneTeam);
	if (0 != strcmp(pstPlayer->stRoleData.RoleName, pstTeamMember->MemberName))
	{
		return -1;
	}

	player_team_destroy_in(pstEnv, pstZoneTeam, 1);

	return 0;
}

int player_team_all_action(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam)
{
	int i;
	Player *pstPlayerMember;
	
	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstPlayerMember = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (!pstPlayerMember)
		{
			continue;
		}

		player_team_action(pstEnv, pstPlayerMember, pstZoneTeam);
	}
	
	return 0;
}

int player_team_chgname(ZONESVRENV* pstEnv, Player *pstPlayer, CSTEAMCHGNAME *pstTeamChgName)
{
	CSPKG stPkg;
	CSTEAMSVR *pstTeamSvr = &stPkg.Body.TeamSvr;
	ZoneTeam *pstZoneTeam;
	TEAMMEMBER *pstTeamMember;
	char szMsg[CLT_MSG_LEN];
	
	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if (NULL == pstZoneTeam)
	{
		return -1;
	}

	if (strcmp(pstZoneTeam->stTeamInfo.TeamName, pstTeamChgName->TeamName) == 0)
	{
		return 0;
	}

	pstTeamMember = player_team_get_captain(pstZoneTeam);
	if (0 != strcmp(pstPlayer->stRoleData.RoleName, pstTeamMember->MemberName))
	{
		return -1;
	}

	if ((0 == pstEnv->pstConf->CountryVer && CheckCharacter(pstTeamChgName->TeamName)) || (CheckDirty(pstTeamChgName->TeamName , 0x1f) & 0x1f ) )
	{
		Z_CSHEAD_INIT(&stPkg.Head, TEAM_SVR);
		pstTeamSvr->OpType = TEAM_SVR_MSG;
		sprintf(pstTeamSvr->TeamSvrData.TeamMsg.Msg, LJY_TEAM1);

		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		
		return -1;
	}

	STRNCPY(pstZoneTeam->stTeamInfo.TeamName, pstTeamChgName->TeamName, sizeof(pstZoneTeam->stTeamInfo.TeamName));

	sprintf(szMsg, LJY_TEAM8, pstTeamChgName->TeamName);
	player_team_broadcast_tip(pstEnv, pstZoneTeam, szMsg);

	player_team_sync(pstEnv, pstZoneTeam);
	player_team_all_action(pstEnv, pstZoneTeam);
	
	return 0;
}

int player_team_same(ZONESVRENV* pstEnv, Player *pstPlayer1, Player *pstPlayer2)
{
	ROLEMISCINFO *pstMisc = &pstPlayer1->stRoleData.MiscInfo;
	UNUSED(pstEnv);

	if (pstMisc->TeamID == 0|| pstMisc->TeamID != pstPlayer2->stRoleData.MiscInfo.TeamID)
	{
		return 0;
	}
	
	return 1;
}

int player_team_send_user_qiyuelist(ZONESVRENV * pstEnv, Player * pstPlayer)
{
	CSPKG stPkg;
	CSTEAMSVR *pstTeamSvr = NULL;

	pstTeamSvr = &stPkg.Body.TeamSvr;
	
	pstTeamSvr->TeamSvrData.UserQiYueList = pstPlayer->stRoleData.Package.QiYueInfo;
	
	pstTeamSvr->OpType = TEAM_USER_QIYUELIST;
	Z_CSHEAD_INIT(&stPkg.Head, TEAM_SVR);
	z_cltmsg_send( pstEnv, pstPlayer,  &stPkg);

	return 0;
}
/* 用户设置自己的当前需要使用的契约 */
static int player_team_user_set_qiyue( ZONESVRENV * pstEnv, Player * pstPlayer, const unsigned int  iQiYue )
{
	QIYUEINFO *pstQiYueInfo = NULL;
	int iEqual = 0;
	int iIndex = 0;
	
	pstQiYueInfo = &pstPlayer->stRoleData.Package.QiYueInfo;

	iIndex = bsearch_int(&iQiYue, pstQiYueInfo->QiYueList, (int)pstQiYueInfo->QiYueNum, sizeof(int), &iEqual);
	if( !iEqual )
	{
		/* 没找到自己有这个契约*/
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM122);
		return -1;
	}
	/* 找到了就设置下 */
	pstQiYueInfo->CurrQiYueID = iQiYue;

	player_team_send_user_qiyuelist(pstEnv,pstPlayer);
	/* 上报下日志 */
	z_role_qiyue_oplog(pstEnv,pstPlayer,iQiYue,0);
	return 0;
}
static int player_team_user_del_qiyue( ZONESVRENV * pstEnv, Player * pstPlayer, const unsigned int  iQiYue )
{
	QIYUEINFO *pstQiYueInfo = NULL;
	
	pstQiYueInfo = &pstPlayer->stRoleData.Package.QiYueInfo;

	if (!bdelete_int(&iQiYue, pstQiYueInfo->QiYueList,(int *)&pstQiYueInfo->QiYueNum, sizeof(int)))
	{
		return -1;
	}

	if( iQiYue == pstQiYueInfo->CurrQiYueID )
	{
		pstQiYueInfo->CurrQiYueID = 0;
	}

	player_team_send_user_qiyuelist(pstEnv,pstPlayer);

	/* 上报下日志 */
	z_role_qiyue_oplog(pstEnv,pstPlayer,iQiYue,2);
	return 0;
}

int team_proxy_request(ZONESVRENV* pstEnv, Player *pstPlayer,
                              CSPKG *pstCsPkg)
{
	CSTEAMCLT *pstTeamClt = &pstCsPkg->Body.TeamClt;
	Player *pstOther = NULL;

	switch(pstTeamClt->OpType)
	{
	case TEAM_OP_APPLY:
		pstOther = z_name_player(pstEnv->pstAppCtx, pstEnv,
		                         pstTeamClt->TeamCltData.TeamApply.CaptainName);
		
		if(pstOther &&  
			!is_together_world(pstEnv, pstPlayer, pstOther))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM132);
			return -1;
		}
		battle_rolename_remove_prefix(
			                     pstTeamClt->TeamCltData.TeamApply.CaptainName);
		break;

	case TEAM_OP_INVITE:
		pstOther = z_name_player(pstEnv->pstAppCtx, pstEnv,
		                         pstTeamClt->TeamCltData.TeamInvite.MemberName);

		if(pstOther &&  
			!is_together_world(pstEnv, pstPlayer, pstOther))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM132);
			return -1;
		}
		
		battle_rolename_remove_prefix(
			                     pstTeamClt->TeamCltData.TeamInvite.MemberName);
		break;

	case TEAM_OP_INVITE_ACCEPT:
		battle_rolename_remove_prefix(
		                  pstTeamClt->TeamCltData.TeamInviteAccept.CaptainName);
		break;

	case TEAM_OP_INVITE_REFUSE:
		battle_rolename_remove_prefix(
		                  pstTeamClt->TeamCltData.TeamInviteRefuse.CaptainName);
		break;

	case TEAM_OP_CAPTAIN_CHG:
		battle_rolename_remove_prefix(
		                     pstTeamClt->TeamCltData.TeamCaptainChg.MemberName);
		break;

	case TEAM_OP_KICK:
		battle_rolename_remove_prefix(
		                           pstTeamClt->TeamCltData.TeamKick.MemberName);
		break;

	case TEAM_OP_CHGNAME:
		battle_rolename_remove_prefix(
		                          pstTeamClt->TeamCltData.TeamChgName.TeamName);
		break;

	case TEAM_OP_SWITCH_QIYUE:
		battle_rolename_remove_prefix(
		                           pstTeamClt->TeamCltData.TeamSwitchQiYueName);
		break;

	case TEAM_OP_APPLY_ACCEPT:
	case TEAM_OP_APPLY_REFUSE:
	case TEAM_OP_APPLY_CLEAR:
	case TEAM_OP_INVITE_CLEAR:
	case TEAM_OP_DESTROY:
	case TEAM_OP_EXIT:
	case TEAM_OP_USER_SET_QIYUE:
	case TEAM_OP_USER_DEL_QIYUE:
	case TEAM_OP_CHG_BOOTY_MODE:
	case TEAM_OP_VOTE_KICK:
    	break;
    case TEAM_OP_ADD:		
		pstOther = z_name_player(pstEnv->pstAppCtx, pstEnv,
		                         pstTeamClt->TeamCltData.TeamAdd.MemberName);
		
		if(pstOther &&  
			!is_together_world(pstEnv, pstPlayer, pstOther))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM132);
			return -1;
		}
		battle_rolename_remove_prefix(
			                     pstTeamClt->TeamCltData.TeamAdd.MemberName);
		break;
	default:
		return -1;
		break;
	}

	return battle_proxy_request(pstEnv, pstPlayer, pstCsPkg);
}


int player_team_follow_trans(ZONESVRENV* pstEnv,ZoneTeam *pstTeam)
{
	int i;
	Player *apstPlayer[MAX_TEAM_MEMBER-1];
	int iPlayer = 0;
	CSPKG stPkg;
	CSTEAMSVR *pstTeamSvr = &stPkg.Body.TeamSvr;
	Player *pstPlayer;
	Player *pstCap = player_team_get_captain_player(pstEnv,pstTeam);

	if (!pstCap)
	{
		return -1;
	}
	
	for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		if (pstTeam->stTeamInfo.TeamMember[i].Position == TEAM_POSITION_MEMBER)
		{
			pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, 
									pstTeam->stTeamInfo.TeamMember[i].MemberName);
			if (pstPlayer && iPlayer < MAX_TEAM_MEMBER-1)
			{
				if (player_team_follow_check_one(pstEnv,pstPlayer, pstCap) < 0)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstCap,SYS_MIDDLE, 
								ZONEERR_TEAM133,
								pstPlayer->stRoleData.RoleName);
					continue;	
				}

				apstPlayer[iPlayer ++] = pstPlayer;
			}
		}
	}
	
	if (iPlayer > 0)
	{
		Z_CSHEAD_INIT(&stPkg.Head, TEAM_SVR);
		pstTeamSvr->OpType = TEAM_SVR_FOLLOW;
		pstTeamSvr->TeamSvrData.TeamSvrFollow = 0;
		z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iPlayer, &stPkg);
	}
		
	return 0;
}

int player_team_follow_cap(ZONESVRENV* pstEnv,ZoneTeam *pstTeam, Player *pstPlayer)
{
	pstPlayer->stOnline.stTeamFollow.cTeamFollow = 1;
	player_team_sync(pstEnv,pstTeam);

	// 发一下队长信息
	player_team_follow_tick(pstEnv,pstTeam);
	return 0;
}

// iCap为1是队长取消跟随
int player_team_follow_cancel(ZONESVRENV* pstEnv,ZoneTeam *pstTeam, Player *pstPlayer, int iCap)
{
	int i;
	Player *pstTmp;

	if (iCap)
	{
		for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
		{
			pstTmp = z_name_player(pstEnv->pstAppCtx,pstEnv, 
					pstTeam->stTeamInfo.TeamMember[i].MemberName);
			if (pstTmp)
			{
				pstTmp->stOnline.stTeamFollow.cTeamFollow  = 0;
			}
		}
	}
	else
	{
		pstPlayer->stOnline.stTeamFollow.cTeamFollow  = 0;	
	}
	player_team_sync(pstEnv,pstTeam);
	return 0;
}

int player_logout_cancel_follow(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	Player *pstCap;
	ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);

	if (pstTeam)
	{
		pstCap  =  player_team_get_captain_player(pstEnv,pstTeam);
		if (pstCap )
		{
			player_team_follow_cancel(pstEnv,pstTeam, pstPlayer, (pstCap == pstPlayer)?1:0);
		}
	}
	
	return 0;
}

// 蛋疼的函数,各种硬编码
int player_team_follow_check_cap(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iMapID = GET_MAPID(pstPlayer->stRoleData.Map);
	int iPworldID;
	ZONEPWORLD *pstZPworld;
	
	if (	pstPlayer->stOnline.State & (CS_STAT_DEAD|CS_STAT_AUTOFLY))
	{
		return -1;
	}

	if (player_is_zuolao(pstPlayer))
	{
		return -1;
	}

	// 海滩
	if (4403 == iMapID)
	{
		return -1;
	}

	if (iMapID >= PWORLD_MAPID_BASE)
	{
		iPworldID = iMapID/100;
		// 公会城市 ,公会冶炼岛,欢乐嘉年华
		if (60000 == iMapID || 60100 == iMapID || iMapID == 20100)
		{
			return -1;
		}

		pstZPworld  = z_find_pworld2(pstEnv, iMapID);
		if (pstZPworld)
		{
			if (is_battle_pworld(pstZPworld))
			{
				return -1;
			}
		}
	}
	
	return 0;
}

int player_team_follow_check_one(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstCap)
{
	
	if (	pstPlayer->stOnline.State & 
		(CS_STAT_DIALOG|CS_STAT_DEAD|CS_STAT_AUTOFLY|
		CS_STAT_COLLECT|CS_STAT_STALL|CS_STAT_RECIPE_PREPARE |
		CS_STAT_GEM_FENJIE_PRE|CS_STAT_ARM_FENJIE_PRE))
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

	if (player_is_zuolao(pstPlayer))
	{
		return -1;
	}

	if (pstCap)
	{
		if (pstCap->stRoleData.Map != pstPlayer->stRoleData.Map)
		{
			return -1;
		}
	}
	
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}


int player_team_follow(ZONESVRENV* pstEnv, Player *pstPlayer, CSTEAMFOLLOW *pstFollow)
{
	ZoneTeam *pstTeam;
	TEAMMEMBER *pstCap;
	Player *pstCapPlayer;


	pstTeam = player_team_get(pstEnv, pstPlayer);
	if (!pstTeam)
	{
		return -1;
	}

	pstCap =  player_team_get_captain(pstTeam);
	if (!pstCap)
	{
		return -1;
	}

	pstCapPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstCap->MemberName);
	if(!pstCapPlayer)
	{
		return -1;
	}

	// 队长要求跟随
	if (pstFollow->Flag == 0)
	{
		if (pstPlayer == pstCapPlayer)
		{
			if (player_team_follow_check_cap(pstEnv, pstPlayer) < 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM134);
				return -1;
			}
			player_team_follow_trans( pstEnv,pstTeam);	
		}	
	}
	// 队员跟随
	else if (pstFollow->Flag == 1)
	{
		if (	pstPlayer != pstCapPlayer )
		{
			if (player_team_follow_check_one(pstEnv,pstPlayer, pstCapPlayer) < 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM135);
				return -1;	
			}

			if (player_team_follow_check_cap(pstEnv,pstCapPlayer) < 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM136);
				return -1;	
			}
			
			player_team_follow_cap(pstEnv,pstTeam, pstPlayer);
		}
	}
	// 取消跟随
	else if (pstFollow->Flag == 2)
	{
		player_team_follow_cancel(pstEnv,pstTeam,pstPlayer,(pstPlayer == pstCapPlayer?1:0));
	}
	else
	{
		return -1;
	}
	
	return 0;
}

int player_team_follow_tick_msg(ZONESVRENV* pstEnv, Player *pstCap, int iPlayer, Player **apstPlayer)
{
	MapIndex *pstMapIdx;
	CSPKG stPkg;
	CSTEAMSVR *pstTeamSvr = &stPkg.Body.TeamSvr;

	Z_CSHEAD_INIT(&stPkg.Head, TEAM_SVR);
	pstTeamSvr->OpType = TEAM_SVR_FOLLOW_TICK;
	//pstTeamSvr->TeamSvrData.TeamFollowTick.Map = pstCap->stRoleData.Map;
	pstTeamSvr->TeamSvrData.TeamFollowTick.Map = GET_MAPID(pstCap->stRoleData.Map);
	pstTeamSvr->TeamSvrData.TeamFollowTick.MapLine = 0;
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, pstCap->stRoleData.Map);
	if (pstMapIdx  && pstMapIdx->stMapDef.MapLine > 0)
	{
		pstTeamSvr->TeamSvrData.TeamFollowTick.MapLine = pstCap->stRoleData.MiscInfo.MapLine;
	}
	pstTeamSvr->TeamSvrData.TeamFollowTick.X = pstCap->stRoleData.Pos.X;
	pstTeamSvr->TeamSvrData.TeamFollowTick.Y = pstCap->stRoleData.Pos.Y;
	z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iPlayer, &stPkg);
	return 0;
}


int player_team_follow_sec(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	Player *pstCap;
	ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);

	if (pstTeam)
	{
		pstCap = player_team_get_captain_player(pstEnv,pstTeam);
		if (pstCap && pstCap == pstPlayer && pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			player_team_follow_tick(pstEnv,pstTeam);
		}
	}
	return 0;
}

int player_team_summon_ask_msg(ZONESVRENV* pstEnv, Player *pstPlayer,ZoneTeam *pstTeam)
{
	CSPKG stPkg;
	CSTEAMSVR *pstTeamSvr = &stPkg.Body.TeamSvr;

	Z_CSHEAD_INIT(&stPkg.Head, TEAM_SVR);
	pstTeamSvr->OpType = TEAM_SVR_SUMMON_ASK;
	pstTeamSvr->TeamSvrData.TeamSummonAsk.SummonMemID = pstPlayer->iMemID;
	pstTeamSvr->TeamSvrData.TeamSummonAsk.Map = GET_MAPID(pstPlayer->stOnline.stTeamSummonInfo.Map);
	pstTeamSvr->TeamSvrData.TeamSummonAsk.X= pstPlayer->stOnline.stTeamSummonInfo.X;
	pstTeamSvr->TeamSvrData.TeamSummonAsk.Y= pstPlayer->stOnline.stTeamSummonInfo.Y;

	pstTeamSvr->TeamSvrData.TeamSummonAsk.OverTime = pstPlayer->stOnline.stTeamSummonInfo.OverTime - 
													pstEnv->pstAppCtx->stCurr.tv_sec;
	
	if (pstTeamSvr->TeamSvrData.TeamSummonAsk.OverTime > 0)
	{
		int i;
		Player *psTmptPlayer;
		Player *apstPlayer[MAX_TEAM_MEMBER];
		int iOnlineNum = 0;
		TEAMMEMBER *pstTeamMember;
		TEAMINFO *pstTeamInfo = &pstTeam->stTeamInfo;
		
		for (i=pstTeamInfo->MemberNum-1; i>=0; i--)
		{
			pstTeamMember = &pstTeamInfo->TeamMember[i];
			if (pstTeamMember->OffLineTime > 0)
			{
				continue;
			}
			
			psTmptPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeamMember->MemberName);
			if (!psTmptPlayer || psTmptPlayer == pstPlayer ||psTmptPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				continue;
			}

			if(pstTeamSvr->TeamSvrData.TeamSummonAsk.Map >= PWORLD_MAPID_BASE && 
				psTmptPlayer->stOnline.stWarOL.cCampIdx != pstPlayer->stOnline.stWarOL.cCampIdx)
			{
				continue;
			}

			if(pstTeamSvr->TeamSvrData.TeamSummonAsk.Map >= PWORLD_MAPID_BASE && 
				psTmptPlayer->stRoleData.Map != pstPlayer->stRoleData.Map)
			{
				continue;
			}

			apstPlayer[iOnlineNum++] = psTmptPlayer;
		}

		if (iOnlineNum > 0)
		{
			z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer, iOnlineNum, &stPkg);
		}
		
		//player_team_broadcast_except(pstEnv, pstTeam, &stPkg,pstPlayer);
	}
	
	return 0;
}


int player_team_summon_close_ui_msg(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSTEAMSVR *pstTeamSvr = &stPkg.Body.TeamSvr;

	Z_CSHEAD_INIT(&stPkg.Head, TEAM_SVR);
	pstTeamSvr->OpType = TEAM_SVR_SUMMON_CLOSE_UI;
	pstTeamSvr->TeamSvrData.TeamSummonCloseUi.Res = 0;
	
	z_cltmsg_send(pstEnv,pstPlayer,&stPkg);
	return 0;
}

int player_team_follow_tick(ZONESVRENV* pstEnv,ZoneTeam *pstTeam)
{
	int i;
	Player *apstPlayer[MAX_TEAM_MEMBER-1];
	int iPlayer = 0;
	Player *pstTmp;
	Player *pstCap = player_team_get_captain_player(pstEnv,pstTeam);
	
	if (!pstCap)
		return -1;
	
	for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		if (pstTeam->stTeamInfo.TeamMember[i].Follow <= 0)
			continue;
		
		pstTmp = z_name_player(pstEnv->pstAppCtx,pstEnv, 
							pstTeam->stTeamInfo.TeamMember[i].MemberName);
		if (!pstTmp || pstTmp == pstCap)
			continue;

		if (iPlayer < MAX_TEAM_MEMBER-1)
		{
			apstPlayer[iPlayer++] = pstTmp;
		}
	}

	if (iPlayer > 0)
	{
		player_team_follow_tick_msg(pstEnv, pstCap, iPlayer, apstPlayer);
	}
	return 0;
}

int player_team_follow_logout(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSTEAMFOLLOW stFollow;

	stFollow.Flag = 2;
	player_team_follow(pstEnv, pstPlayer, &stFollow);
	return 0;
}

int player_team_follow_cancel_svr(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);
	Player *pstCap;
	
	if (pstTeam)
	{
		pstCap = player_team_get_captain_player(pstEnv,pstTeam);
		if (pstCap)
		{
			player_team_follow_cancel(pstEnv,pstTeam,pstPlayer,pstPlayer==pstCap?1:0);
		}
	}
	
	return 0;
}

int player_follow_enter_map_check(ZONESVRENV* pstEnv, Player *pstPlayer,int iMap)
{
	int i;
	int j;
	TASKINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.TaskInfo;

	MapIndex*pstIdx	= z_mappos_index(pstEnv->pstMapObj, NULL, iMap);
	if( !pstIdx )
		return -1;

	for (i=0; i<MAX_MAP_ENTER_LIMIT; i++)
	{
		if (pstIdx->stMapDef.EnterLimit[i].Type <= 0)
		{
			break;
		}

		switch(pstIdx->stMapDef.EnterLimit[i].Type)
		{
		case MAP_ENTER_LIMIT_TYPE_LVEL_MIN:
			if (pstPlayer->stRoleData.Level < pstIdx->stMapDef.EnterLimit[i].Val)
			{
				return -1;
			}
			break;
		case MAP_ENTER_LIMIT_TYPE_LVEL_MAX:
			if (pstPlayer->stRoleData.Level > pstIdx->stMapDef.EnterLimit[i].Val)
			{
				return -1;
			}

			break;
		case MAP_ENTER_LIMIT_TYPE_TASK:
			for (j=0; j<pstInfo->TaskFini.TaskNum; j++)
			{
				if (pstInfo->TaskFini.TaskIDs[j] == pstIdx->stMapDef.EnterLimit[i].Val)
				{
					break;
				}
			}	

			if (j >= pstInfo->TaskFini.TaskNum)
			{
				return -1;
			}
			break;
		}
	}
	return 0;
}

int player_team_follow_chg_map(ZONESVRENV* pstEnv, Player *pstPlayer, int iMapID, 
										RESPOS *pstPos,ZONEPWORLDINST *pstPworldInst)
{
	int i;
	ZoneTeam *pstTeam;
	Player *pstCap;
	Player *pstTmp;
	PWORLDSELECT stSelect;
//	int iToMap;

	if (0 == g_team_follow_flag && NULL == pstPworldInst)
	{
		return 0;
	}

	if (pstPworldInst)
	{
		ZONEPWORLD *pstZonePworld;

		pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
		if (NULL == pstZonePworld)
		{
			return 0;
		}
		if ((pstZonePworld->stPworldDef.Type != PWORLD_TYPE_TEAM) &&
			(pstZonePworld->stPworldDef.Type != PWORLD_TYPE_ALL))
		{
			player_team_follow_cancel_svr(pstEnv, pstPlayer);
			return 0;
		}
	}

	// 跟随的跟着传送
	pstTeam = player_team_get(pstEnv,pstPlayer);
	if (pstTeam)
	{
		pstCap = player_team_get_captain_player(pstEnv,pstTeam);
		if (pstCap && pstCap == pstPlayer)
		{
			memset(&stSelect, 0, sizeof(stSelect));
			for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
			{
				if (pstTeam->stTeamInfo.TeamMember[i].Follow)
				{
					pstTmp = z_name_player(pstEnv->pstAppCtx, pstEnv,
										pstTeam->stTeamInfo.TeamMember[i].MemberName);
					if ( !pstTmp ||
						pstTmp == pstPlayer ||
						pstTmp->eStatus != PLAYER_STATUS_ROLE_LOGIN ||
						pstTmp->stRoleData.Map == pstPlayer->stRoleData.Map)
					{
						continue;
					}

					if (g_team_follow_flag == 2) //切线
					{
						if (0 == is_map_same_line(pstEnv, pstTmp->stRoleData.Map,
													pstPlayer->stRoleData.Map))
						{
							continue;
						}
					}
					else if (g_team_follow_flag == 1) // 坐飞机检查
					{
						if ( player_trans_select_check(pstEnv, pstTmp,&pstCap->stOnline.stTransSelect,NULL,NULL, NULL, 0) < 0)
						{
							z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv,pstCap,SYS_MIDDLE, ZONEERR_TEAM137,
										pstTmp->stRoleData.RoleName);
							player_team_follow_cancel(pstEnv,pstTeam,pstTmp,0);
							continue;
						}	
					}

					if (team_trans_check_one(pstEnv,pstTmp) < 0)
					{
						player_team_follow_cancel(pstEnv,pstTeam,pstTmp,0);
						continue;
					}

					if (pstPworldInst)
					{	
						if (pstTmp->stRoleData.Map >= PWORLD_MAPID_BASE)
						{
							continue;
						}
						
						if (z_player_enter_pworld(pstEnv, pstTmp, pstPworldInst->stPworldIdx.iID, 
												&stSelect, 1) < 0)
						{
							player_team_follow_cancel(pstEnv,pstTeam,pstTmp,0);
							continue;
						}
					}
					else
					{
						
						
						if ( player_follow_enter_map_check(pstEnv, pstTmp,iMapID) < 0)
						{
							z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstCap,SYS_MIDDLE, ZONEERR_TEAM138,
										pstTmp->stRoleData.RoleName);
							player_team_follow_cancel(pstEnv,pstTeam,pstTmp,0);
							continue;	
						}

						if (pstTmp->stRoleData.Map >= PWORLD_MAPID_BASE && iMapID < PWORLD_MAPID_BASE)
						{
							RESPOS stPos;
							
							stPos.X = pstTmp->stRoleData.ExitPos_x;
							stPos.Y = pstTmp->stRoleData.ExitPos_y;
						
							if (z_player_change_map2(pstEnv->pstAppCtx, pstEnv, pstTmp,
												pstTmp->stRoleData.ExitMap, &stPos, RAND1(180), 0) < 0)
							{
								player_team_follow_cancel(pstEnv,pstTeam,pstTmp,0);
								continue;
							}
						}
						else
						{
							if (z_player_change_map2(pstEnv->pstAppCtx, pstEnv, pstTmp,
												iMapID, pstPos, RAND1(180), 0) < 0)
							{
								player_team_follow_cancel(pstEnv,pstTeam,pstTmp,0);
								continue;
							}
						}

						if(pstTmp->stRoleData.Map != pstPlayer->stRoleData.Map)
						{
							player_team_follow_cancel(pstEnv,pstTeam,pstTmp,0);
							continue;
						}
					}
				}	
			}
		}
	}
	return 0;
}

int player_leave_team_clean_buff(ZONESVRENV* pstEnv, Player *pstPlayer, int iResultID)
{
	ZoneStatus* pstStat;
	pstStat = z_is_player_status_result(pstEnv, pstPlayer,iResultID);

	if (pstStat)
	{
		z_player_del_status_level(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstStat->unStatusID, pstStat->bLevel, 1, 0);
	}
	
	return 0;
}

int player_team(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{	
	CSTEAMCLT *pstTeamClt = &pstCsPkg->Body.TeamClt;

	
	if(pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		if(is_battle_pve_battle_svr(pstEnv))
		{
			if(pstTeamClt->OpType == TEAM_OP_APPLY_ACCEPT ||
				pstTeamClt->OpType == TEAM_OP_APPLY_REFUSE ||
				pstTeamClt->OpType == TEAM_OP_APPLY_CLEAR ||
				pstTeamClt->OpType == TEAM_OP_INVITE_ACCEPT ||
				pstTeamClt->OpType == TEAM_OP_INVITE_REFUSE ||
				pstTeamClt->OpType == TEAM_OP_INVITE_CLEAR ||
				pstTeamClt->OpType == TEAM_OP_VOTE_KICK)
			{
				return team_proxy_request(pstEnv, pstPlayer, pstCsPkg);
			}
			else
			{
				if(pstTeamClt->OpType != TEAM_OP_SUMMON_ANSWER)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
					return 0;
				}
			}
		}
		else if(is_battle_pvp_svr(pstEnv))
		{
			if(pstTeamClt->OpType != TEAM_OP_FOLLOW)
			{
				return team_proxy_request(pstEnv, pstPlayer, pstCsPkg);
			}
		}
		else if(is_battle_sparta_svr(pstEnv))
		{
			return 0;
		}
		else
		{
			if( pstTeamClt->OpType == TEAM_OP_EXIT &&
				player_team_get(pstEnv, pstPlayer) )
			{
				battle_proxy_request(pstEnv, pstPlayer, pstCsPkg);
			}
		}
	}

	switch(pstTeamClt->OpType)
	{
	case TEAM_OP_APPLY:
		player_team_apply(pstEnv, pstPlayer, &pstTeamClt->TeamCltData.TeamApply);
		break;
	case TEAM_OP_APPLY_ACCEPT:
		player_team_apply_accept(pstEnv, pstPlayer, &pstTeamClt->TeamCltData.TeamApplyAccept);
		break;
	case TEAM_OP_APPLY_REFUSE:
		player_team_apply_refuse(pstEnv, pstPlayer, &pstTeamClt->TeamCltData.TeamApplyRefuse);
		break;
	case TEAM_OP_APPLY_CLEAR:
		player_team_apply_clear(pstEnv, pstPlayer);
		break;
	case TEAM_OP_INVITE:
		player_team_invite(pstEnv, pstPlayer, &pstTeamClt->TeamCltData.TeamInvite);
		break;
	case TEAM_OP_INVITE_ACCEPT:
		player_team_invite_accept(pstEnv, pstPlayer, &pstTeamClt->TeamCltData.TeamInviteAccept);
		break;
	case TEAM_OP_INVITE_REFUSE:
		player_team_invite_refuse(pstEnv, pstPlayer, &pstTeamClt->TeamCltData.TeamInviteRefuse);
		break;
	case TEAM_OP_INVITE_CLEAR:
		player_team_invite_clear(pstEnv, pstPlayer);
		break;
	case TEAM_OP_CAPTAIN_CHG:
		player_team_captain_chg(pstEnv, pstPlayer, &pstTeamClt->TeamCltData.TeamCaptainChg);
		break;
	case TEAM_OP_EXIT:
		player_team_exit(pstEnv, pstPlayer,NULL);
		break;
	case TEAM_OP_KICK:
		player_team_kick(pstEnv, pstPlayer, &pstTeamClt->TeamCltData.TeamKick,0);
		break;
	case TEAM_OP_DESTROY:
		 player_team_destroy(pstEnv, pstPlayer);
		break;
	case TEAM_OP_CHGNAME:
		player_team_chgname(pstEnv, pstPlayer, &pstTeamClt->TeamCltData.TeamChgName);
		break;
	case TEAM_OP_SWITCH_QIYUE:
		player_team_set_qiyue(pstEnv, pstPlayer, pstTeamClt->TeamCltData.TeamSwitchQiYueName );
		break;
	case TEAM_OP_USER_SET_QIYUE:
		player_team_user_set_qiyue(pstEnv, pstPlayer,pstTeamClt->TeamCltData.TeamUserSetQieYue);
		break;
	case TEAM_OP_USER_DEL_QIYUE:
		player_team_user_del_qiyue(pstEnv, pstPlayer,pstTeamClt->TeamCltData.TeamUserDelQieYue);
		break;
	case TEAM_OP_CHG_BOOTY_MODE:
		return player_team_chg_booty_mode(pstEnv, pstPlayer, &pstTeamClt->TeamCltData.ChgBootyMode);
		break;
	case TEAM_OP_VOTE_KICK:
		player_team_kick_vote_clt(pstEnv, pstPlayer, &pstTeamClt->TeamCltData.TeamVoteKick);
		break;
	case TEAM_OP_FOLLOW:
		player_team_follow(pstEnv, pstPlayer, &pstTeamClt->TeamCltData.TeamFollow);
		break;
		
	case TEAM_OP_SUMMON_ANSWER:
		player_team_summon_answer(pstEnv, pstPlayer, &pstTeamClt->TeamCltData.TeamSummonAnswer);
		break;
	case TEAM_OP_ADD:
		player_team_add(pstEnv, pstPlayer, &pstTeamClt->TeamCltData.TeamAdd);
		break;
	default:
		return -1;
		break;
	}
		
	return 0;
	UNUSED(pstAppCtx);
	UNUSED(pstFrameHead);
}

int z_active_team(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iCount)
{
	static struct timeval stRoundTv;
	static struct timeval stRoundStartTv;
	static unsigned int iActivePos = 0, iRound = 0;
	int i;
	struct timeval stSub;
	long long  llMs;
	ZoneTeam *pstTeam;
	TMEMBLOCK *pstMemBlock;

	if (0 == stRoundTv.tv_sec)
	{
		llMs = TEAM_ACTIVE_TIMER;
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
		
	if (llMs < TEAM_ACTIVE_TIMER)
	{
		return 0;
	}

	if (0 == iActivePos)
	{
		TV_CLONE(stRoundStartTv, pstAppCtx->stCurr);
	}

	for (i = 0; i < iCount; i++) 
	{
		if (iActivePos >= MAX_TEAM) 
		{
			break;
		}
		
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstTeamPool, iActivePos);
		iActivePos++;

		if (!pstMemBlock->fValid)
		{
			continue;
		}
		
		pstTeam = (ZoneTeam *)pstMemBlock->szData;

		// 队伍同步信息都由zone_svr发送, battle_svr只持shadow team info
		// 用于共鸣和契约,基本所有的队伍操作都由zone_svr处理, battle_svr仅仅处理
		// 契约与共鸣

		//放到SYNC前面
		player_team_kick_vote_tick(pstEnv,pstTeam);
		
		player_team_sync(pstEnv, pstTeam); // player_team_sync内部会处理契约

		//player_team_follow_tick(pstEnv,pstTeam);

		team_booty_active(pstEnv, pstTeam);

		player_team_resonance(pstEnv, pstTeam); // 队伍成员之间共鸣

		autopworld_team_tick(pstEnv,pstTeam);

		rongyan_team_tick(pstEnv,pstTeam);
	}
	if (iActivePos < MAX_TEAM)
	{
		return 0;
	}
	
	iActivePos = 0;
	iRound++;
	TV_CLONE(stRoundTv, pstAppCtx->stCurr);

	TV_DIFF(stSub, pstAppCtx->stCurr, stRoundStartTv);
	TV_TO_MS(llMs, stSub);
	if (llMs > TEAM_ACTIVE_TIMER)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "active team loop %lld ms", llMs);
	}
	
	return 0;
}

int team_member_pworld_update(ZONESVRENV* pstEnv, ZoneTeam *pstTeam, Player *pstPlayer, int iCaptain)
{	
	ZONEPWORLDINST *pstZonePworldInst;

	UNUSED(pstEnv);
	
	//相关副本的处理
	pstZonePworldInst = z_find_player_pworld_inst(pstEnv, pstPlayer);
	if (pstZonePworldInst)
	{
		ZONEPWORLD* pstPworld;

		pstPworld	=	z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
		if (pstPworld && (PWORLD_TYPE_TEAM == pstPworld->stPworldDef.Type || PWORLD_TYPE_ALL== pstPworld->stPworldDef.Type))
		{
			pworld_team_together(pstEnv, pstTeam, pstZonePworldInst, iCaptain);
		}
	}
	
	return 0;
}

int team_insert_pworld_inst(ZONESVRENV * pstEnv, ZoneTeam *pstZoneTeam, ZONEPWORLDINST *pstZonePworldInst, int fCaptain)
{
	int i;

	UNUSED(pstEnv);
	
	if(pstZoneTeam->iPworldNum >= MAX_TEAM_PWORLD_INST)
	{
		return -1;
	}

	for(i=0; i<pstZoneTeam->iPworldNum; i++)
	{
		if(pstZoneTeam->stPworldInfo[i].iPworldID == pstZonePworldInst->stPworldIdx.iID)
		{
			//非队长不可顶替已有的队伍副本 
			if(fCaptain == 0)
			{
				return -1;
			}

			pstZoneTeam->stPworldInfo[i].iPworldInstID = pstZonePworldInst->iPworldInstID;
			pstZoneTeam->stPworldInfo[i].ullPworldWID = pstZonePworldInst->ullPworldWID;

			return 0;
		}
	}

	pstZoneTeam->stPworldInfo[pstZoneTeam->iPworldNum].iPworldID = pstZonePworldInst->stPworldIdx.iID;
	pstZoneTeam->stPworldInfo[pstZoneTeam->iPworldNum].iPworldInstID = pstZonePworldInst->iPworldInstID;
	pstZoneTeam->stPworldInfo[pstZoneTeam->iPworldNum].ullPworldWID = pstZonePworldInst->ullPworldWID;
	pstZoneTeam->iPworldNum++;
		
	return 0;
}

int team_del_pworld_inst(ZONESVRENV * pstEnv, ZoneTeam *pstZoneTeam, int iPworldID)
{
	int i;
	UNUSED(pstEnv);
	
	for(i=pstZoneTeam->iPworldNum-1; i>=0; i--)
	{
		if(pstZoneTeam->stPworldInfo[i].iPworldID != iPworldID)
		{
			continue;
		}

		pstZoneTeam->iPworldNum--;

		if(i != pstZoneTeam->iPworldNum)
		{
			memcpy(&pstZoneTeam->stPworldInfo[i], 
				&pstZoneTeam->stPworldInfo[pstZoneTeam->iPworldNum], 
				sizeof(pstZoneTeam->stPworldInfo[0]));
		}
	}
	
	return 0;
}


// 获取队伍的队长的Player结构体
Player* team_get_captain_by_player(ZONESVRENV *pstEnv,Player* pstPlayer)
{
	ZoneTeam *pstZoneTeam;
	TEAMMEMBER *pstCaptain;

	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if(!pstZoneTeam)
	{
		return NULL;
	}
	
	pstCaptain = player_team_get_captain(pstZoneTeam);
	return pstCaptain ? z_name_player(pstEnv->pstAppCtx, pstEnv,
	                                  pstCaptain->MemberName)
	                  : NULL;
}

int team_is_on_zonesvr_by_teamid(unsigned long long ullTeamID)
{
	unsigned short unTmp;

	if(ullTeamID == 0)
	{
		return 1;
	}
	memcpy(&unTmp,((char *)(&ullTeamID)+sizeof(int)), sizeof(unTmp)); 
	unTmp = unTmp&0x03FF;

	if(unTmp > 0 )
	{
		return 0;
	}
	return unTmp;
}

int player_team_summon_answer(ZONESVRENV* pstEnv, Player *pstPlayer, CSTEAMSUMMONANSWER *pstSummonAnswer)
{
	ZoneTeam* pstTeam = player_team_get(pstEnv,pstPlayer);
	Player* pstSummoner = z_id_player(pstEnv->pstAppCtx,pstEnv,pstSummonAnswer->SummonMemID);
	ZoneTeam* pstSummonTeam = NULL;
	TeamSummonInfo* pstInfo = NULL;
	RESPOS stPos;

	if (0 == pstSummonAnswer->Yes)
	{
		//close ui
		player_team_summon_close_ui_msg(pstEnv,pstPlayer);
		return -1;
	}
	if (!pstSummoner)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_TEAM139);
		player_team_summon_close_ui_msg(pstEnv,pstPlayer);
		return -1;
	}
	
	pstInfo = &pstSummoner->stOnline.stTeamSummonInfo;

	/*
	if(pstInfo->Map != pstSummoner->stRoleData.Map)
	{
		return -1;
	}
	*/

	if(pstSummoner->stRoleData.Map >= PWORLD_MAPID_BASE)
	{
		if(pstInfo->Map != pstPlayer->stRoleData.Map)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_TEAM140);
			player_team_summon_close_ui_msg(pstEnv,pstPlayer);
			return -1;
		}

		if(pstSummoner->stOnline.stWarOL.cCampIdx != pstPlayer->stOnline.stWarOL.cCampIdx)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_TEAM139);
			player_team_summon_close_ui_msg(pstEnv,pstPlayer);
			return -1;
		}
	}
	
	//队伍检查
	if (!pstTeam)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_TEAM141);
		player_team_summon_close_ui_msg(pstEnv,pstPlayer);
		return -1;
	}
	pstSummonTeam = player_team_get(pstEnv,pstSummoner);

	if (pstTeam != pstSummonTeam)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_TEAM142);
		player_team_summon_close_ui_msg(pstEnv,pstPlayer);
		return -1;
	}

	if (pstEnv->pstAppCtx->stCurr.tv_sec > pstSummoner->stOnline.stTeamSummonInfo.OverTime)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_TEAM143);
		//close ui
		player_team_summon_close_ui_msg(pstEnv,pstPlayer);
		return -1;
	}

	//状态检查
	
	if((pstPlayer->stOnline.State & CS_STAT_ATK)||
		(pstPlayer->stOnline.State & CS_STAT_MOVE_ATK) ||
		(pstPlayer->stOnline.State & CS_STAT_PREPARE) ||
		(pstPlayer->stOnline.State & CS_STAT_MOVE_PREPARE))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_TEAM144);
		return -1;
	}

	//任务加了定身等效果的时候也不让传

	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOITEM))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TEAM144);
		return -1;
	}
	
	//传送检查
	stPos.X = pstInfo->X;
	stPos.Y = pstInfo->Y;
	
	if(0 > z_player_goto_check(pstEnv, pstPlayer, pstInfo->Map, &stPos))
	{
		return -1;
	}

	if ( player_follow_enter_map_check(pstEnv, pstPlayer,pstInfo->Map) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_TEAM145);		
		return -1;
	}
		
	player_goto_ignore_refer_line(pstEnv,pstPlayer, pstInfo->Map,pstInfo->X,  pstInfo->Y,pstInfo->MapPos);

	//close ui
	player_team_summon_close_ui_msg(pstEnv,pstPlayer);
	
	return 0;
}


int player_team_summon(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZoneTeam* pstTeam = player_team_get(pstEnv,pstPlayer);
	TeamSummonInfo* pstInfo = &pstPlayer->stOnline.stTeamSummonInfo;

	pstInfo->Map = pstPlayer->stRoleData.Map;
	pstInfo->X = pstPlayer->stRoleData.Pos.X;
	pstInfo->Y = pstPlayer->stRoleData.Pos.Y;
	pstInfo->MapPos = pstPlayer->stOnline.iMapPos;
	pstInfo->OverTime = pstEnv->pstAppCtx->stCurr.tv_sec + TEAM_SUMMON_OVER_TIME;
	

	player_team_summon_ask_msg(pstEnv,pstPlayer,pstTeam);
	
	return 0;
}



int player_team_rongyan_login_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZoneTeam *pstTeam;
	CSRONGYANUIINFO stRongYanUiInfo;
	CSRONGYANBOSSINFO stRongYanBossInfo;
		
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(pstTeam == NULL)
	{
		return -1;
	}

	if(0 == pstTeam->iRongYanCurLevel)
	{
		return 0;
	}
	
	if(1 == pstTeam->iRongYanStatus)
	{
		//通知客户端弹出融焰移动信息框
		stRongYanUiInfo.BeginSelectMul = pstTeam->iRongYanSelectMul;
		stRongYanUiInfo.CurLayer = pstTeam->iRongYanCurLevel;
		player_rongyan_move_ui(pstEnv,pstPlayer,&stRongYanUiInfo);	
	}
	else
	{
		//处于第一层到第九层时。才能弹出boss选择
		if(pstTeam->iRongYanCurLevel > 0 && pstTeam->iRongYanCurLevel < 10)
		{
			
				//通知客户端弹出boss 真假选择框
				stRongYanBossInfo.BeginSelectMul = pstTeam->iRongYanSelectMul;
				stRongYanBossInfo.CurLayer = pstTeam->iRongYanCurLevel;
				player_rongyan_boss_ui(pstEnv, pstPlayer,&stRongYanBossInfo);
		
		}
		else
		{
			return -1;
		}
	}
	return 0;
}


int rongyan_team_tick(ZONESVRENV*pstEnv,ZoneTeam *pstTeam)
{
	CSRONGYANBOSSSELECT stRongYanBoss;
	TEAMMEMBER *pstCaptain;
	Player *pstCaptainPlayer = NULL;
	
	if(pstTeam == NULL)
	{
		return -1;
	}

	if(0 == pstTeam->tRongYanBossSelectTimeEnd)
	{
		return 0;
	}

	pstCaptain = player_team_get_captain(pstTeam);
	if(pstCaptain == NULL)
	{
		return -1;
	}
	
	pstCaptainPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,pstCaptain->MemberName);
	if(pstCaptainPlayer == NULL)
	{
		return -1;
	}

	if (pstEnv->pstAppCtx->stCurr.tv_sec > pstTeam->tRongYanBossSelectTimeEnd)
	{
		stRongYanBoss.iSelect = 0;
	    player_rongyan_boss(pstEnv,pstCaptainPlayer,&stRongYanBoss);
	}
	
	return 0;
}

