/*
**  @file $RCSfile: zone_clan.c,v $
**  general description of this module
**  $Id: zone_clan.c,v 1.562 2014/07/02 02:37:06 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/07/02 02:37:06 $
**  @version $Revision: 1.562 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "proto_comm.h"
#include "proto_cs.h"
#include "role_db_meta.h"
#include "zone_clan.h"
#include "zone_clan_city.h"
#include "zone_clan_advance.h"
#include "zone_clan_construction.h"
#include "zone_db.h"
#include "zone_clt.h"
#include "zone_svr.h"
#include "zone_package.h"
#include "zone_move.h"
#include "libdirty.h"
#include "zone_relation.h"
#include "zone_oplog.h"
#include "zone_task.h"
#include "zone_attr.h"
#include "zone_task.h"
#include "zone_map.h"
#include "zone_range.h"
#include "zone_player.h"
#include "libdirty.h"
#include <stdarg.h>
#include <time.h>
#include <limits.h>
#include "zone_spansvr.h"
#include "zone_safelock.h"
#include "zone_city.h"
#include "zone_machine.h"
#include "zone_strongpoint.h"
#include "zone_status.h"
#include "zone_span.h"
#include "zone_combat.h"
#include "zone_city.h"
#include "zone_misc.h"
#include "zone_roll.h"
#include "zone_err.h"
#include "zone_zh_name.h"
#include "zone_clanboon.h"
#include "zone_mail.h"
#include "zone_coroutine.h"
#include "zone_clan_pet.h"
#include "zone_dianfeng.h"
#include "zone_span_strong.h"

#define CLAN_ACTIVE_TIMER 5000
#define CLAN_DESTROY_MEM_MIN 10
#define CLAN_DESTROY_MEM_MIN_TIME  7*24*3600
#define CLAN_DESTROY_ACTIVE_MIN_TIME  60*24*3600
#define CLAN_DB_PAGE_COUNT 10

#define CLAN_EXIT_CD 24*3600 // 在退会CD时间中，无法再加入/创建公会。
#define CLAN_APPLY_MAX_TIME 24*3600 // 申请最长处理时间。
#define CLAN_ONLINE_ACTION_TIME 5*60 // 在线动作处理的最长时间。

#define CLAN_SET_BULLITEN_CD 10 // 修改公告的冷却时间
#define CLAN_EXIT_ENEMY_CD 24*3600

#define CLAN_MAX_OP  (CLAN_OP_VIEW_PAGE+1)
#define CLAN_MAX_POS (CLAN_POS_PRESIDENT+1)
#define CLAN_MAX_LV  6 // 最大等级

#define CLAN_CREATE_MIN_ROLE_LV 30 // 能创建公会的最小等级


#define CLAN_STOP_PATCH 25

#define CLAN_SELECT_TASK_GROUP_CD_TIME 24*3600 // 切换任务组的冷却时间
#define CLAN_PAY_CD_TIME 24*3600 // 领工资的冷却时间

#define MAX_CLAN_MONEY           0x7fffffff
#define MAX_CLAN_STABLE_VALUE         10000
#define MAX_CLAN_ACTIVE_VALUE    0x7fffffff
#define MAX_CLAN_TECH_POINT          250000
#define MAX_CLAN_USED_TECH_POINT 0x7fffffff

// 自动回复为默认任务组的时间
#define CLAN_AUTO_RECOVER_DEFAULT_TASK_GROUP_TIME 3*24*3600

// 公会城市降级第一次警告时间
#define CLAN_CITY_FIRST_DOWNGRADE_WARNING_TIME 6*3600

// 公会城市降级最后一次警告时间
#define CLAN_CITY_LAST_DOWNGRADE_WARNING_TIME  3600

// 宣战持续时间
#define CLAN_WAR_PERSISTED_TIME 7200

// 宣战通知间隔时间
#define CLAN_WAR_NOTIFY_INTERVAL 600

#define COND_DO_(cond, expr) (void)((cond) ? (expr) : 0)

#define CLAN_DART_MIN 20
//
// 配置信息
//
int g_iClanDestroyMemMinSecs = CLAN_DESTROY_MEM_MIN_TIME;
int g_iClanDestroyActiveMinSecs = CLAN_DESTROY_ACTIVE_MIN_TIME;
int g_iClanExitCDSecs = CLAN_EXIT_CD;
int g_iClanApplyMaxSecs = CLAN_APPLY_MAX_TIME;
int g_iClanOnlineActiveSecs = CLAN_ONLINE_ACTION_TIME;
int g_iClanSetBullitenCDSecs = CLAN_SET_BULLITEN_CD;
int g_iClanMaxApplyNum = CLAN_MAX_APPLY;
int g_iClanSelectTaskGroupCDTime = CLAN_SELECT_TASK_GROUP_CD_TIME;
int g_iClanAutoRecoverDefaultTaskGroupTime = CLAN_AUTO_RECOVER_DEFAULT_TASK_GROUP_TIME;
int g_iClanPayCDTime = CLAN_PAY_CD_TIME;
int g_iClanCityFirstDowngradeWarningTime = CLAN_CITY_FIRST_DOWNGRADE_WARNING_TIME;
int g_iClanCityLastDowngradeWarningTime = CLAN_CITY_LAST_DOWNGRADE_WARNING_TIME;

int g_iOptype = 0;
int g_iOpsubtype = 0;

#define _PUT(pos) (1U << (CLAN_POS_##pos))
#define _ALL_ALLOW 0xFFFFFFFFU
#define _ALL_DENY  0x00000000U
#define _ALL_OFFICER \
	(_PUT(GROUP_1) | \
	 _PUT(GROUP_2) | \
	 _PUT(GROUP_3) | \
	 _PUT(GROUP_4) | \
	 _PUT(GROUP_5) | \
	 _PUT(GROUP_6) | \
	 _PUT(VICE_PRESIDENT) | \
	 _PUT(PRESIDENT))
#define _ALL_MEMBER (_PUT(NORMAL) | _ALL_OFFICER)
#define _ALL_LEADER (_PUT(VICE_PRESIDENT) | _PUT(PRESIDENT))

// 公会所有操作的权限定义
static const unsigned int kClanPermission[MAX_CLAN_OP] =
{
	/*创建*/ [CLAN_OP_CREATE]                   = _PUT(NOT_MEMBER),
	/*加人*/ [CLAN_OP_INVITE_JOIN_CLAN]         = _ALL_OFFICER,
	/*踢人*/ [CLAN_OP_DEL_MEMBER]               = _ALL_OFFICER,
	/*退会*/ [CLAN_OP_EXIT_CLAN]                = _ALL_MEMBER,
	/*提升*/ [CLAN_OP_UP_POSITION]              = _ALL_DENY,
	/*降级*/ [CLAN_OP_DOWN_POSITION]            = _ALL_DENY,
	/*解散*/ [CLAN_OP_DISBAND]                  = _PUT(PRESIDENT),
	/*公告*/ [CLAN_OP_EDIT_BULLETIN]            = _ALL_LEADER,
	/*宣告*/ [CLAN_OP_EDIT_MANIFESTO]           = _PUT(PRESIDENT),
	/*让位*/ [CLAN_OP_DEMISE_JOB]               = _PUT(PRESIDENT),
	/*申请*/ [CLAN_OP_APPLY_JOIN_CLAN]          = _PUT(NOT_MEMBER),
	/*审核*/ [CLAN_OP_CENSOR_JOIN_CLAN]         = _ALL_OFFICER,
	/*昵称*/ [CLAN_OP_SET_MEMBER_NICKNAME]      = _ALL_OFFICER,
	/*列表*/ [CLAN_OP_APPLY_JOIN_LIST]          = _ALL_MEMBER,
	/*同盟*/ [CLAN_OP_INVITE_JOIN_UNION]        = _ALL_LEADER,
	/*同盟*/ [CLAN_OP_APPLY_JOIN_UNION]         = _ALL_LEADER,
	/*同盟*/ [CLAN_OP_EXIT_UNION]               = _ALL_LEADER,
	/*敌对*/ [CLAN_OP_APPLY_JOIN_ENEMY]         = _ALL_OFFICER,
	/*敌对*/ [CLAN_OP_EXIT_ENEMY]               = _ALL_DENY,
	/*外交*/ [CLAN_OP_DIPLOMATISM_STATUS]       = _ALL_MEMBER,
	/*应答*/ [CLAN_OP_ANSWER_JOIN_CLAN]         = _PUT(NOT_MEMBER),
	/*应答*/ [CLAN_OP_ANSWER_JOIN_UNION]        = _ALL_LEADER,
	/*应答*/ [CLAN_OP_ANSWER_JOIN_ENEMY]        = _ALL_DENY,
	/*搜索*/ [CLAN_OP_SEARCH]                   = _ALL_DENY,
	/*审核*/ [CLAN_OP_CENSOR_JOIN_UNION]        = _ALL_LEADER,
	/*审核*/ [CLAN_OP_CENSOR_JOIN_ENEMY]        = _ALL_LEADER,
	/*快照*/ [CLAN_OP_MEMBER_SNAPSHOT]          = _ALL_DENY,
	/*信息*/ [CLAN_OP_INFO]                     = _ALL_DENY,
	/*宣告*/ [CLAN_OP_MANIFESTO]                = _ALL_ALLOW,
	/*升级*/ [CLAN_OP_UPGRADE]                  = _ALL_LEADER,
	/*激活*/ [CLAN_OP_ENABLE_CITY]              = _ALL_LEADER,
	/*升级*/ [CLAN_OP_UPGRADE_CITY]             = _ALL_LEADER,
	/*升级*/ [CLAN_OP_UPGRADE_CITY_BUILDING]    = _ALL_OFFICER,
	/*切换*/ [CLAN_OP_SELECT_TASK_GROUP]        = _ALL_DENY,
	/*切换*/ [CLAN_OP_SELECT_CITY_TASK_GROUP]   = _ALL_DENY,
	/*分页*/ [CLAN_OP_VIEW_PAGE]                = _ALL_DENY,
	/*城市*/ [CLAN_OP_CITY_INFO]                = _ALL_DENY,
	/*工资*/ [CLAN_OP_PAY]                      = _ALL_DENY,
	/*查询*/ [CLAN_OP_FIND_REQ]                 = _ALL_ALLOW,
	/*分页*/ [CLAN_OP_VIEW_PAGE_BY_ID]          = _ALL_ALLOW,
	/*研发*/ [CLAN_OP_RESEARCH]                 = _ALL_OFFICER,
	/*取消*/ [CLAN_OP_CANCEL_RESEARCH]          = _ALL_OFFICER,
	/*制造*/ [CLAN_OP_MAKE]                     = _ALL_OFFICER,
	/*制造*/ [CLAN_OP_CANCEL_MAKING]            = _ALL_OFFICER,
	/*提取*/ [CLAN_OP_GRAB_MACHINE]             = _ALL_MEMBER,
	/*召回*/ [CLAN_OP_CALLBACK_MACHINE]         = _ALL_MEMBER,
	/*补给*/ [CLAN_OP_MACHINE_SUPPLY]           = _ALL_MEMBER,
	/*切换*/ [CLAN_OP_STORAGE_GRID_LOCK_SWITCH] = _PUT(PRESIDENT),
	/*摧毁*/ [CLAN_OP_STORAGE_GRID_DESTROY]     = _PUT(PRESIDENT),
	/*敌对*/ [CLAN_OP_ANSWER_EXIT_ENEMY]        = _ALL_LEADER,
	/*封官*/ [CLAN_OP_SET_JOB_AND_TITLE]        = _ALL_LEADER,
	/*加速*/ [CLAN_OP_RESEARCH_SPEEDUP]         = _ALL_OFFICER,
	/*加速*/ [CLAN_OP_MAKING_SPEEDUP]           = _ALL_OFFICER,
	/*未用*/ [CLAN_OP_TMP]						= _ALL_MEMBER,
	/*兑换*/ [CLAN_OP_RESOURCE_GRAB_ITEM]		= _ALL_MEMBER,
	/*切换*/ [CLAN_OP_RESOURCE_LOCK_SWITCH]		= _PUT(PRESIDENT),
	
	/*头顶*/ [CLAN_OP_SET_COLOR]	= _ALL_LEADER,
	/*会标*/ [CLAN_OP_SET_MAP_POS]	= _ALL_LEADER,
	/*----*/ [CLAN_OP_SKILL_RESEARCH]	= _ALL_LEADER,	//技能研发
	/*----*/ [CLAN_OP_SKILL_LEARN]	= _ALL_MEMBER,	//技能学习
	/*----*/ [CLAN_OP_VIEW_GETRES_INFO]	= _ALL_MEMBER,	//技能学习
	
	/*----*/ [CLAN_OP_CLAN_SUMMON_ANSWER]	= _ALL_MEMBER,	
	/*----*/ [CLAN_OP_SET_GROUP]	= _ALL_OFFICER,	
	/*----*/ [CLAN_OP_SET_GROUP_STRONG_MAP]	= _ALL_LEADER,
	/*----*/ [CLAN_OP_OPEN_GROUP]	= _ALL_MEMBER,
	/*----*/ [CLAN_OP_KICK_CITY]	= _ALL_LEADER,
};

// 公会那些操作需要安全锁认证
static const unsigned int kClanSafeLock[MAX_CLAN_OP] =
{
	/*创建*/ [CLAN_OP_CREATE]                   = 0,
	/*加人*/ [CLAN_OP_INVITE_JOIN_CLAN]         = 0,
	/*踢人*/ [CLAN_OP_DEL_MEMBER]               = 1,
	/*退会*/ [CLAN_OP_EXIT_CLAN]                = 1,
	/*提升*/ [CLAN_OP_UP_POSITION]              = 0,
	/*降级*/ [CLAN_OP_DOWN_POSITION]            = 0,
	/*解散*/ [CLAN_OP_DISBAND]                  = 1,
	/*公告*/ [CLAN_OP_EDIT_BULLETIN]            = 0,
	/*宣告*/ [CLAN_OP_EDIT_MANIFESTO]           = 0,
	/*让位*/ [CLAN_OP_DEMISE_JOB]               = 1,
	/*申请*/ [CLAN_OP_APPLY_JOIN_CLAN]          = 0,
	/*审核*/ [CLAN_OP_CENSOR_JOIN_CLAN]         = 0,
	/*昵称*/ [CLAN_OP_SET_MEMBER_NICKNAME]      = 1,
	/*列表*/ [CLAN_OP_APPLY_JOIN_LIST]          = 0,
	/*同盟*/ [CLAN_OP_INVITE_JOIN_UNION]        = 1,
	/*同盟*/ [CLAN_OP_APPLY_JOIN_UNION]         = 1,
	/*同盟*/ [CLAN_OP_EXIT_UNION]               = 1,
	/*敌对*/ [CLAN_OP_APPLY_JOIN_ENEMY]         = 1,
	/*敌对*/ [CLAN_OP_EXIT_ENEMY]               = 1,
	/*外交*/ [CLAN_OP_DIPLOMATISM_STATUS]       = 0,
	/*应答*/ [CLAN_OP_ANSWER_JOIN_CLAN]         = 0,
	/*应答*/ [CLAN_OP_ANSWER_JOIN_UNION]        = 1,
	/*应答*/ [CLAN_OP_ANSWER_JOIN_ENEMY]        = 1,
	/*搜索*/ [CLAN_OP_SEARCH]                   = 0,
	/*审核*/ [CLAN_OP_CENSOR_JOIN_UNION]        = 1,
	/*审核*/ [CLAN_OP_CENSOR_JOIN_ENEMY]        = 1,
	/*快照*/ [CLAN_OP_MEMBER_SNAPSHOT]          = 0,
	/*信息*/ [CLAN_OP_INFO]                     = 0,
	/*宣告*/ [CLAN_OP_MANIFESTO]                = 0,
	/*升级*/ [CLAN_OP_UPGRADE]                  = 1,
	/*激活*/ [CLAN_OP_ENABLE_CITY]              = 1,
	/*升级*/ [CLAN_OP_UPGRADE_CITY]             = 1,
	/*升级*/ [CLAN_OP_UPGRADE_CITY_BUILDING]    = 1,
	/*切换*/ [CLAN_OP_SELECT_TASK_GROUP]        = 0,
	/*切换*/ [CLAN_OP_SELECT_CITY_TASK_GROUP]   = 0,
	/*分页*/ [CLAN_OP_VIEW_PAGE]                = 0,
	/*城市*/ [CLAN_OP_CITY_INFO]                = 0,
	/*工资*/ [CLAN_OP_PAY]                      = 0,
	/*查询*/ [CLAN_OP_FIND_REQ]                 = 0,
	/*分页*/ [CLAN_OP_VIEW_PAGE_BY_ID]          = 0,
	/*研发*/ [CLAN_OP_RESEARCH]                 = 1,
	/*取消*/ [CLAN_OP_CANCEL_RESEARCH]          = 1,
	/*制造*/ [CLAN_OP_MAKE]                     = 1,
	/*制造*/ [CLAN_OP_CANCEL_MAKING]            = 1,
	/*提取*/ [CLAN_OP_GRAB_MACHINE]             = 0,
	/*召回*/ [CLAN_OP_CALLBACK_MACHINE]         = 0,
	/*补给*/ [CLAN_OP_MACHINE_SUPPLY]           = 0,
	/*切换*/ [CLAN_OP_STORAGE_GRID_LOCK_SWITCH] = 1,
	/*摧毁*/ [CLAN_OP_STORAGE_GRID_DESTROY]     = 1,
	/*敌对*/ [CLAN_OP_ANSWER_EXIT_ENEMY]        = 1,
	/*封官*/ [CLAN_OP_SET_JOB_AND_TITLE]        = 1,
	/*加速*/ [CLAN_OP_RESEARCH_SPEEDUP]         = 1,
	/*加速*/ [CLAN_OP_MAKING_SPEEDUP]           = 1,
	/*----*/     [CLAN_OP_RESOURCE_LOCK_SWITCH]     = 1, /* 资源中心锁切换 */
	/*----*/     [CLAN_OP_RESOURCE_GRAB_ITEM]       = 1, /* 资源中心兑换物品 */
	 		     [CLAN_OP_SET_COLOR]			= 1,	 /* 设置头顶颜色 */
	/*会标*/ [CLAN_OP_SET_MAP_POS]			= 0,	 
	/*----*/ [CLAN_OP_SKILL_RESEARCH]			= 0,	//技能研发
	/*----*/ [CLAN_OP_SKILL_LEARN]			= 0,	//技能学习

	/*----*/ [CLAN_OP_VIEW_GETRES_INFO]			= 0,	
	/*----*/ [CLAN_OP_CLAN_SUMMON_ANSWER]			= 0,	
	/*----*/ [CLAN_OP_SET_GROUP]			= 0,	
	/*----*/ [CLAN_OP_SET_GROUP_STRONG_MAP]			= 0,	
	/*----*/ [CLAN_OP_OPEN_GROUP]			= 0,	
	/*----*/ [CLAN_OP_KICK_CITY]			= 0,	
	
};


#undef _PUT
#undef _ALL_ALLOW
#undef _ALL_DENY
#undef _ALL_OFFICER
#undef _ALL_MEMBER
#undef _ALL_LEADER

const char *kClanPosName[] = {
	[CLAN_POS_NOT_MEMBER]         = LJY_CLAN_POS_NOT_MEMBER,
	[CLAN_POS_NORMAL]             = LJY_CLAN_POS_NORMAL,
	[CLAN_POS_TECH_OFFICER]       = LJY_CLAN_POS_TECH_OFFICER,
	[CLAN_POS_COLLECTION_OFFICER] = LJY_CLAN_POS_COLLECTION_OFFICER,
	[CLAN_POS_FACTORY_OFFICER]    = LJY_CLAN_POS_FACTORY_OFFICER,
	[CLAN_POS_PET_OFFICER]        = LJY_CLAN_POS_PET_OFFICER,
	[CLAN_POS_WALL_OFFICER]       = LJY_CLAN_POS_WALL_OFFICER,
	[CLAN_POS_VICE_PRESIDENT]     = LJY_CLAN_POS_VICE_PRESIDENT,
	[CLAN_POS_PRESIDENT]          = LJY_CLAN_POS_PRESIDENT,
	[CLAN_POS_GROUP_1]          = LJY_CLAN_POS_GROUP_1,
	[CLAN_POS_GROUP_2]          = LJY_CLAN_POS_GROUP_2,
	[CLAN_POS_GROUP_3]          = LJY_CLAN_POS_GROUP_3,
	[CLAN_POS_GROUP_4]          = LJY_CLAN_POS_GROUP_4,
	[CLAN_POS_GROUP_5]          = LJY_CLAN_POS_GROUP_5,
	[CLAN_POS_GROUP_6]          = LJY_CLAN_POS_GROUP_6,
};

static const int kClanPosCmp[][CLAN_POS_GROUP_6 + 1] =
{
	     /* 非  民  科  采  工  宠  郊  副  会 1团 2团 3团 4团 5团 6团*/
	/*非*/ { 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	/*民*/ { 1,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	/*科*/ { 1,  1,  0,  0,  0,  0,  0, -1, -1, -1, -1, -1, -1, -1, -1},
	/*采*/ { 1,  1,  0,  0,  0,  0,  0, -1, -1, -1, -1, -1, -1, -1, -1},
	/*工*/ { 1,  1,  0,  0,  0,  0,  0, -1, -1, -1, -1, -1, -1, -1, -1},
	/*宠*/ { 1,  1,  0,  0,  0,  0,  0, -1, -1, -1, -1, -1, -1, -1, -1},
	/*郊*/ { 1,  1,  0,  0,  0,  0,  0, -1, -1, -1, -1, -1, -1, -1, -1},
	/*副*/ { 1,  1,  1,  1,  1,  1,  1,  0, -1, 1, 1, 1, 1, 1, 1},
	/*会*/ { 1,  1,  1,  1,  1,  1,  1,  1,  0, 1, 1, 1, 1, 1, 1},
	/*1团*/ { 1,  1,  1,  1,  1,  1,  1,  -1,  -1, 0, 0, 0, 0, 0, 0},
	/*2团*/ { 1,  1,  1,  1,  1,  1,  1,  -1,  -1, 0, 0, 0, 0, 0, 0},
	/*3团*/ { 1,  1,  1,  1,  1,  1,  1,  -1,  -1, 0, 0, 0, 0, 0, 0},
	/*4团*/ { 1,  1,  1,  1,  1,  1,  1,  -1,  -1, 0, 0, 0, 0, 0, 0},
	/*5团*/ { 1,  1,  1,  1,  1,  1,  1,  -1,  -1, 0, 0, 0, 0, 0, 0},
	/*6团*/ { 1,  1,  1,  1,  1,  1,  1,  -1,  -1, 0, 0, 0, 0, 0, 0},
	     /* 非  民  科  采  工  宠  郊  副  会1团 2团 3团 4团 5团 6团 */
};


// 在什么地方查找公会
typedef enum tagClanInWhere
{
	CLAN_IN_ANY_WHERE, // 任何地方。
	CLAN_IN_FRIENDS, // 公会的盟友中。
	CLAN_IN_APPLY_FRIEND, // 公会的盟友申请队列中。
	CLAN_IN_ENEMYS, // 公会的敌人中。
	CLAN_IN_APPLY_ENEMY, // 公会的敌人申请队列中。
}ClanInWhere;

// 公会加入申请列表的上下文信息
typedef struct tagMemberApplyContext
{
	ZONESVRENV *pstEnv;
	Player *pstPlayer;
	const char *pszHint;
}MemberApplyContext;

// 查看入会请求列表
int player_clan_apply_join_clan_list(ZONESVRENV *pstEnv, Player *pstPlayer);

// 根据Id查找公会信息。
ZoneClan* z_id_clan(ZONESVRENV *pstEnv, unsigned int nID);

// 获取公会中某个成员的信息
CLANMEM* clan_id_mem(ZoneClan *pstClan, tdr_ulonglong ullRoleID, int *piIdx);

	
// 获取公会中某个申请者的信息。
static CLANAPPLY* clan_id_apply(ZoneClan *pstClan, tdr_ulonglong ullRoleID,
                                int *piIdx);
	
// 更新所有官员信息，包括会长，副会长。
static void clan_update_officers(ZONESVRENV *pstEnv, ZoneClan * pstClan);

// 有公会成员信息快照的人
static Player* ClanBodyHasMemberSnapshot(ZONESVRENV *pstEnv,
                                         const ROLEMEMBER *pstMember);

// 能够审查加入公会的人
static Player* ClanBodyCanCensorJoinClan(ZONESVRENV *pstEnv,
                                         const ROLEMEMBER *pstMember);




// 更新公会入会申请表，去除所有不在线或长期未被处理的玩家
static int IsRoleApplyTimeOutOrOffine(void *pvCtx, void *pvItem);
static void clan_update_apply(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                              Player *pstPlayer);

// 1. 更新公会的同盟申请列表、敌对申请列表。
// 去除不存在的公会或长期未被处理的公会
// 2. 更新公会的同盟列表，敌对列表。
// 去除不存在的公会。
static int IsClanInexist(void *pvCtx, void *pvItem);
//static int IsClanApplyTimeOutOrInexist(void *pvCtx, void *pvItem);

static void clan_update_diplomatism(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                    Player *pstPlayer);

// 更新在线数据，就是公会邀请公会加入同盟的数据。
static void clan_update_online_action(ZONESVRENV *pstEnv, ZoneClan *pstClan);
	
// 把某个玩家加入公会
// pstMem 可以为NULL
static int clan_add_mem(ZONESVRENV *pstEnv, ZoneClan *pstClan,
                        Player *pstCustom, unsigned char bPosition,
                        CLANMEM *pstMem);
	
// 在公会的指定位置查找公会
static void* clan_clanid_diplomatism(ZONESVRENV *pstEnv, ZoneClan *pstClan,
	                                 ClanInWhere nInWhere,
	                                 const CLANID *pstClanID,
	                                 unsigned int *punIdx);
	
// 检查一个公会是否能与另一个公会建立外交关系。
// 能建立外交关系返回非0，否则返回0。
static int clan_check_can_relations(ZONESVRENV *pstEnv, Player *pstPlayer,
                                    ZoneClan *pstMyClan,
                                    ZoneClan *pstOtherClan, int fNo2Side);
	
// 获取能活动(在线)的管理人员:会长，副会长。
static Player* clan_get_active_manager(ZONESVRENV *pstEnv, ZoneClan *pstClan);

// 填充一列公会视图
// 版本变化数据被填充返回1，否则返回0
static int clan_fill_view_row(CSCLANVIEWPAGE *pstPage, int iRow, int iVersion,
                              const ZoneClan *pstClan);
	
// 填充范围内的公会视图列
static void clan_fill_view_row_range(CSCLANVIEWPAGE *pstPage, int iBegin,
                                     int iEnd,
                                     const CSCLANVIEWPAGECOOKIE *pstCookie,
                                     const ClanObj *pstClanObj);

// 公会动作通知
static int clan_action(ZONESVRENV *pstEnv, const ZoneClan *pstZoneClan, 
                       const CLANMEM *pstClanMem );


static int clan_action_exit(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES * pstActionRes = &stPkg.Body.ActionRes;
	
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_CLAN_CHG;
	pstActionRes->Data.ClanChg.Position = 0;
	pstActionRes->Data.ClanChg.Name[0] = 0;

	return player_action_res_ex(pstEnv, pstPlayer, &stPkg);
}

// 处理公会物资消耗
static int clan_process_cost(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                             int *pfHasCost);

// 检查公会操作许可
static int clan_check_permission(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 int iOperation);

// 检查公会人数限制
static int clan_check_mem_limit(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	//const CLANLEVELDEF *pstLvDef = z_clan_level_def(pstEnv,
	//                                       pstZoneClan->stClan.ClanProf.Level);
//	if (!pstLvDef)
	//{
	//	return -1;
	//}
	//return (pstZoneClan->stClan.ClanCore.MemNum >=
	 //      (unsigned)pstLvDef->MaxMemberNumLimit) ? -1 : 0;

	int i;
	unsigned iMemNumTop = pstZoneClan->stClan.ClanCore.MemNumTop;
	if (iMemNumTop <= 0)
	{
		//刚创建公会的时候和数据库表中ResultVal1 没填写的时候调用
		const CLANSKILLDEF *pstDef = z_find_clan_skill_def(pstEnv,
											MEM_NUM_CLAN_SKILL_ID, 0);
	
		if (!pstDef)
		{
			return -1;
		}

		for(i = 0; i < MAX_CLAN_SKILL_RESULT;i++)
		{
			if (!pstDef->Result[i].ResultID)
			{
				break;
			}
			if (pstDef->Result[i].ResultID == RESULT_CLAN_MEM_NUM_TOP)
			{
				iMemNumTop = pstDef->Result[i].ResultVal1;
			}
		}
	}
	return (pstZoneClan->stClan.ClanCore.MemNum >= iMemNumTop) ? -1 : 0;
}

// 公会解散时间通知
static int clan_destroy_warning(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                Player *pstPlayer);

// 填充外交快照数据
static void clan_fill_diplomatism_snapshot(ZONESVRENV* pstEnv,
                                           const ZoneClan *pstZoneClan, 
                                           CSCLANDIPLOMATISMSNAPSHOT *pstRes);
	
// 玩家的公会加入另一公会同盟
static int player_clan_join_union(ZONESVRENV *pstEnv, Player *pstPlayer, 
                                  ZoneClan *pstHostClan,
                                  ZoneClan *pstCustomClan);

// 玩家的公会与另一公会敌对
static int player_clan_join_enemy(ZONESVRENV *pstEnv, Player *pstPlayer, 
                                  ZoneClan *pstHostClan,
                                  ZoneClan *pstCustomClan);

// 检查公会名是否合法
// 1 不能为空。
// 2 不能重复。
// 3 不能出现老大哥不喜欢的词。
// 不合法返回非0，否则返回0。
static int player_clan_check_name(ZONESVRENV *pstEnv, Player *pstPlayer, 
                                  const char *psClanName);

// 广播公会公告信息
static int player_clan_bulliten_broadcast(ZONESVRENV *pstEnv,
                                          ZoneClan *pstClan);
// 发送公会无效信息给玩家
static inline int player_clan_disable(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;

	stPkg.Body.ClanSvr.Operation = CLAN_SVR_DISABLE;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	return z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
}

// 广播公会外交的变化
static inline int player_clan_diplomatism_chg_broadcast(ZONESVRENV *pstEnv,
                                                    const ZoneClan *pstZoneClan)
{
	CSPKG stPkg;
	clan_fill_diplomatism_snapshot(pstEnv, pstZoneClan,
	                   &stPkg.Body.ClanSvr.ClanSvrData.ClanDiplomatismSnapshot);
	stPkg.Body.ClanSvr.Operation = CLAN_SVR_DIPLOMATISM_CHG;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	return clan_broadcast_if(pstEnv, pstZoneClan, &stPkg, ClanEveryBody);
}

// 检查某个角色在公会中的能力
static inline int player_clan_capability_check(ZONESVRENV *pstEnv,
                                               Player *pstPlayer, int iOpt)
{
	int iPos = 0;
	if (iOpt < 0 || iOpt >= MAX_CLAN_OP)
	{
		return -1;
	}
	iPos = player_clan_position(pstEnv, pstPlayer);
	return (kClanPermission[iOpt] & (1U << iPos)) ? 0 : -1;
}

/*
// 如果battle_svr收到了公会操作,它不可能自己处理,将会转发给zone_svr处理
static int clan_proxy_request(ZONESVRENV* pstEnv, Player *pstPlayer,
                              CSPKG *pstCsPkg);
                              

*/// 通过名字找到公会、这个操作较慢
static ZoneClan* clan_find_by_name(ZONESVRENV *pstEnv, const char *pszClanName);

// 通过名字删除公会成员
static int clan_del_mem_by_name(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                const char *pszRoleName);

// 通过RoleID删除公会成员
static int clan_del_mem_by_id(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                              tdr_ulonglong ullRoleID,int iTip);

// 删除公会外交关系
static int clan_del_diplomatism(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                const char *pszTargetName);

// 将公会会长职位转让给指定的成员
static int clan_exchange_leader(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                const char *pszNewLeaderRoleName);

// 通知公会属性值
static int clan_property_do_notify(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                   int iValue, int iWhich);

static inline void clan_set_city_prof(const CLANCITY *pstFull,
                                      CLANCITYPROF *pstProf)
{
	pstProf->Level = pstFull->Level;
	memcpy(pstProf->Buildings, pstFull->Buildings, sizeof(pstProf->Buildings));
}

static inline void clan_fill_view(const ZoneClan *pstZoneClan,
                                  CSCLANVIEW *pstView)
{
	pstView->ProfInfo  = pstZoneClan->stClan.ClanProf;
	pstView->BriefInfo = pstZoneClan->stClan.ClanBrief;

	pstView->CityInfo.Level = pstZoneClan->stClan.ClanCity.Level;
	memcpy(pstView->CityInfo.Buildings, pstZoneClan->stClan.ClanCity.Buildings,
	       sizeof(pstView->CityInfo.Buildings));
}

// 检查老数据,使其兼容新数据
static int clan_check_old_data(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan);

// 公会成员退出公会的回调
static int clan_mem_on_exit(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                            Player *pstPlayer, int fActive);

// 部分的销毁公会
static int clan_partial_destroy(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan);


// 一个玩家是否能创建公会
static int clan_can_create(ZONESVRENV *pstEnv, Player *pstPlayer,
                           const char *psClanName,CLANLEVELDEF *pstInitialDef);
                           
// 对一个新公会进行简单初始化
static int clan_low_init(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                         Player *pstPlayer, CLANMEM *pstNewMem,
                         const char *psClanName);

// 将一个完成了初始化的公会安装到公会列表中
static int clan_install(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan);

// 设置现场信息
static inline void clan_set_live(ZONESVRENV *pstEnv, ZoneClanLive *pstClanLive,
                                 ZoneClan *pstOtherClan)
{
	pstClanLive->tActionTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstClanLive->stOtherSideClan = *clan_id_warp(&pstOtherClan->stClan);
}

//更新宣战cd时间
int clan_update_enemy_cd(ZONESVRENV *pstEnv,ZoneClan *pstClan);

// 检查退会冷却时间
static inline int player_clan_check_exit_punish_time(ZONESVRENV* pstEnv,
                                                     Player *pstPlayer)
{
	int iExitTm = 0;
	if (!pstPlayer)
	{
		return -1;
	}
	iExitTm = pstPlayer->stRoleData.MiscInfo.ClanInfo.ExitPunishTime;
	return (iExitTm != 0 &&
	       (pstEnv->pstAppCtx->stCurr.tv_sec - iExitTm < g_iClanExitCDSecs)) ?
	       -1 : 0;
}

// 公会的全部快照信息
static inline void player_clan_snapshot(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if (player_get_clan(pstEnv, pstPlayer))
	{
		player_clan_info(pstEnv, pstPlayer);
		player_clan_diplomatism_snapshot(pstEnv, pstPlayer);
		player_clan_city_info(pstEnv, pstPlayer);
		player_clan_member_snapshot(pstEnv, pstPlayer);
		player_clan_advance(pstEnv, pstPlayer);
		player_clan_group_info(pstEnv, pstPlayer);
	}
}


#define _OFFSET_OF(type, member) (uintptr_t)(&((type *)0)->member)

#define _SIZE_OF(type, member)   (int)sizeof(((type *)0)->member)
	
typedef struct _ClanPropertyEntry
{
	uintptr_t nOffset;
	int iBytes;
	int iMin;
	int iMax;
	int fNotify;
	int iInvalid;	//无效的属性,0表示有效
}ClanPropertyEntry;

#define CLAN_OFFSET_OF(member, low, high, notify,invalid) { \
	.nOffset = _OFFSET_OF(ZoneClan, member),\
	.iBytes = _SIZE_OF(ZoneClan, member),\
	.iMin = low,\
	.iMax = high,\
	.fNotify = notify, \
	.iInvalid = invalid, \
}

static const ClanPropertyEntry astClanProperties[] = {
	// CLAN_PROPERTY_MONEY
	CLAN_OFFSET_OF(stClan.ClanProf.Money, 0, MAX_CLAN_MONEY, 2,0),

	// CLAN_PROPERTY_STABLE_VALUE
	CLAN_OFFSET_OF(stClan.ClanProf.StableValue, 0, MAX_CLAN_STABLE_VALUE, 1,0),

	// CLAN_PROPERTY_ACTIVE_VALUE
	CLAN_OFFSET_OF(stClan.ClanProf.ActiveValue, 0, MAX_CLAN_ACTIVE_VALUE, 1,0),

	// CLAN_PROPERTY_TECH_POINT
	CLAN_OFFSET_OF(stClan.ClanProf.TechPoint, 0, MAX_CLAN_TECH_POINT, 1,0),

	// CLAN_PROPERTY_USED_TECH_POINT
	CLAN_OFFSET_OF(stClan.ClanAdvance.UsedTechPoint, 0,
	               MAX_CLAN_USED_TECH_POINT, 0,0),

	// CLAN_PROPERTY_MAX_COLLECTION_MAKING_NUM
	CLAN_OFFSET_OF(stClan.ClanAdvance.Produce.MaxCollectionMakingNum, 1,
	               MAX_COLLECTION_MAKING_NUM, 0,0),

	// CLAN_PROPERTY_MAX_ARM_MAKING_NUM
	CLAN_OFFSET_OF(stClan.ClanAdvance.Produce.MaxArmMakingNum, 1,
	               MAX_ARM_MAKING_NUM, 0,0),

	// CLAN_PROPERTY_MAX_MACHINE_GRID_NUM
	CLAN_OFFSET_OF(stClan.ClanAdvance, 1,
	               MAX_CLAN_MACHINE_GRID_NUM, 0,0),

	// CLAN_PROPERTY_MAX_RESOURCE_GRID_NUM
	CLAN_OFFSET_OF(stClan.ClanAdvance, 1,
	               MAX_CLAN_RESOURCE_GRID_NUM, 0,1),

	// CLAN_PROPERTY_LEVEL
	CLAN_OFFSET_OF(stClan.ClanProf.Level, 1, CLAN_MAX_LEVEL, 3,0),

	// CLAN_PROPERTY_CITY_LEVEL
	CLAN_OFFSET_OF(stClan.ClanCity.Level, 1, CLAN_MAX_CITY_LEVEL, 3,0),

	//CLAN_PROPERTY_FLAG
	CLAN_OFFSET_OF(stClan.ClanProf.Flag, 0, INT_MAX, 3,0),
};

#undef CLAN_OFFSET_OF


const char* time_text_zh(int iSec, char *psRev)
{
#define DAY_S   (24 * HOUR_S)
#define HOUR_S  (60 * MINUTE_S)
#define MINUTE_S 60

	char szBuf[128];
	int n = iSec / DAY_S;
	if (n > 0)
	{
		snprintf(szBuf, CCH(szBuf), "%d%s", n, LJY_DAY);
		iSec -= (n * DAY_S);
		strcat(psRev, szBuf);
	}
	
	if ((n = iSec / HOUR_S) > 0)
	{
		snprintf(szBuf, CCH(szBuf), "%d%s", n, LJY_HOUR);
		iSec -= (n * HOUR_S);
		strcat(psRev, szBuf);
	}
	
	if ((n = iSec / MINUTE_S) > 0)
	{
		snprintf(szBuf, CCH(szBuf), "%d%s", n, LJY_MINUTE);
		iSec -= (n * MINUTE_S);
		strcat(psRev, szBuf);
	}

	if (iSec > 0)
	{
		snprintf(szBuf, CCH(szBuf), "%d%s", iSec, LJY_SECOND);
		strcat(psRev, szBuf);
	}

	return psRev;
	
#undef MINUTE_S
#undef HOUR_S
#undef DAY_S
}

static const char* clan_exit_remain_time_zh(ZONESVRENV *pstEnv,
                                            Player *pstPlayer)
{
	static char szBuf[CLT_MSG_LEN] = "未知";
	const int kExitTime = pstPlayer->stRoleData.MiscInfo.ClanInfo.ExitPunishTime;
	const int kCurr = pstEnv->pstAppCtx->stCurr.tv_sec;

	if (kExitTime > 0 && kCurr > kExitTime)
	{
		const int kDelta = g_iClanExitCDSecs - (kCurr - kExitTime);
		if (kDelta >= 0)
		{
			szBuf[0] = 0;
			time_text_zh(kDelta, szBuf);
		}
	}

	szBuf[CLT_MSG_LEN - 1] = 0;
	return szBuf;
}


void* array_find_if(void *pvBase, size_t nBlock, int nNum, void *pvCtx,
                    int (*pfnPred)(void *, void *))
{
	int i;
	char *pcBase = pvBase;

	for(i = 0; i < nNum; ++i)
	{
		if ((*pfnPred)(pvCtx, pcBase + nBlock * i))
		{
			return pcBase + nBlock * i;
		}
	}

	return NULL;
}


size_t array_remove_if(void *pvBase, size_t nBlock, int nNum, void *pvCtx,
                       int (*pfnPred)(void *, void *))
{
	//size_t i, nFirst, nDest;
	int i = 0;
	char * pcBase = pvBase;

	if (!pvBase || !nBlock || !nNum || !pfnPred)
	{
		return nNum;
	}

	i = nNum;
	while(i--)
	{
		if ((*pfnPred)(pvCtx, pcBase + nBlock * i))
		{
			if (i < nNum - 1)
			{
				memcpy(pcBase + nBlock * i,
				       pcBase + nBlock * (nNum - 1), nBlock);
			}
			--nNum;
		}
	}
	return nNum;
}


size_t array_circle_push_back(void* pvBase, size_t nBlock, int nNum,
                              int nMax, const void* pvPushed)
{
	char *pcBase = pvBase;

	if (!pvBase || !nBlock || nNum > nMax || !pvPushed)
	{
		return 0;
	}

	if (nNum < nMax)
	{
		memcpy(pcBase + nNum * nBlock, pvPushed, nBlock);
		++nNum;
	}
	else
	{
		memmove(pcBase, pcBase + nBlock, nBlock * (nNum - 1));
		memcpy(pcBase + (nNum - 1) * nBlock, pvPushed, nBlock);
	}
	return nNum;
}


size_t array_circle_push_back_overflow(void *pvBase, size_t nBlock, int nNum,
                                       int nMax, const void *pvPushed,
                                       void *pvOverflowed, int *piIsOverflowed)
{
	char *pcBase = pvBase;

	if (!pvBase || !nBlock || nNum > nMax || !pvPushed)
	{
		return 0;
	}

	if (nNum < nMax)
	{
		if (piIsOverflowed)
		{
			*piIsOverflowed = 0;
		}
		memcpy(pcBase + nNum * nBlock, pvPushed, nBlock);
		++nNum;
	}
	else
	{
		if (pvOverflowed)
		{
			memcpy(pvOverflowed, pcBase, nBlock);
		}
		if (piIsOverflowed)
		{
			*piIsOverflowed = 1;
		}
		memmove(pcBase, pcBase + nBlock, nBlock * (nNum - 1));
		memcpy(pcBase + (nNum - 1) * nBlock, pvPushed, nBlock);
	}
	return nNum;
}


ZoneClan* z_clan_leader_id(ZONESVRENV *pstEnv, tdr_ulonglong ullRoleID)
{
	ZoneClan *pstZoneClan = NULL;
	ZoneClan *astZoneClans = NULL;
	int i = 0;
	
	if (pstEnv->pstClanObj->stClanObjInfo.iClanStat != CLAN_STAT_FINI)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "when player_get_clan clan stat %d", 
			         pstEnv->pstClanObj->stClanObjInfo.iClanStat);
		goto out;
	}

	astZoneClans = pstEnv->pstClanObj->astZoneClan;
	i = pstEnv->pstClanObj->iClanNum;
	while(i--)
	{
		if (astZoneClans[i].stClan.ClanBrief.Leader.RoleID == ullRoleID)
		{
			pstZoneClan = &astZoneClans[i];
			break;
		}
	}

out:
	return pstZoneClan;
}

int clan_open_generic_ui(ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID,
                         int iUI)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);

	if (!pstZoneClan && iUI != UI_TYPE_CLAN_CREATE &&
		iUI != UI_TYPE_CLAN_SEARCH)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
		               ZONEERR_CLAN1);
		return -1;
	}

	switch(iUI)
	{
	case UI_TYPE_CLAN_CREATE:
		if (pstZoneClan)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN201);
			return -1;
		}

		if (player_clan_check_exit_punish_time(pstEnv, pstPlayer) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_SVR,
			           ZONEERR_CLAN202,
			           clan_exit_remain_time_zh(pstEnv, pstPlayer));
			return -1;
		}
		break;

	case UI_TYPE_CLAN_DISBAND:
		if (clan_check_permission(pstEnv, pstPlayer, CLAN_OP_DISBAND) < 0)
		{
			return -1;
		}
		break;

	case UI_TYPE_CLAN_MANIFESTO:
		if (clan_check_permission(pstEnv, pstPlayer,
		                          CLAN_OP_EDIT_MANIFESTO) < 0)
		{
			return -1;
		}
		break;

	case UI_TYPE_UPGRADE_CLAN:
		if (clan_check_permission(pstEnv, pstPlayer, CLAN_OP_UPGRADE) < 0)
		{
			return -1;
		}
		break;

	case UI_TYPE_UPGRADE_CLAN_CITY:
		if (clan_check_permission(pstEnv, pstPlayer, CLAN_OP_UPGRADE_CITY) < 0)
		{
			return -1;
		}
		break;

	case UI_TYPE_UPGRADE_WALL:
	case UI_TYPE_UPGRADE_TECH_CENTER:
		if (clan_check_permission(pstEnv, pstPlayer,
		                          CLAN_OP_UPGRADE_CITY_BUILDING) < 0)
		{
			return -1;
		}
		break;

	case UI_TYPE_UPGRADE_COLLECTION_CENTER:
	case UI_TYPE_UPGRADE_FACTORY:
	case UI_TYPE_UPGRADE_FARM:
		if (clan_building_upgrade_usedtech_check(pstEnv, pstZoneClan, pstPlayer,
		                              iUI - UI_TYPE_UPGRADE_TECH_CENTER) < 0 ||
			clan_check_permission(pstEnv, pstPlayer,
			                      CLAN_OP_UPGRADE_CITY_BUILDING) < 0)
		{
			return -1;
		}
		break;

	case UI_TYPE_CLAN_SEARCH:
	case UI_TYPE_CLAN_RESEARCH:
	case UI_TYPE_CLAN_MAKING:
	case UI_TYPE_CLAN_STORAGE:
	case UI_TYPE_CLAN_FACTORY:
		break;

	default:
		return -1;
		break;
	}

	pstActionRes->ActionID = ACTION_UI;
	pstActionRes->Data.UI.UIType = iUI;
	pstActionRes->ID = iNpcID;
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


static int ClanIdEquals(void* pvLhs, void* pvRhs)
{
	const CLANID *pstLhs = pvLhs, *pstRhs = pvRhs;
	return pstLhs->GID == pstRhs->GID;
}

static int RoleMemberEquals(void* pvLhs, void* pvRhs)
{
	const ROLEMEMBER *pstLhs = pvLhs, *pstRhs = pvRhs;
	return pstLhs->RoleID == pstRhs->RoleID;
}

static int CmpClanMem(const void *pvLhs, const void *pvRhs)
{
	const CLANMEM *pstLhs = pvLhs, *pstRhs = pvRhs;

	if (pstLhs->RoleMember.RoleID < pstRhs->RoleMember.RoleID)
	{
		return -1;
	}
	if (pstLhs->RoleMember.RoleID > pstRhs->RoleMember.RoleID)
	{
		return 1;
	}
	return 0;
}

int ClanHashCmp(const void* pv1 , const void* pv2)
{
	ClanHash *pstClanHash1 = (ClanHash *)pv1;
	ClanHash *pstClanHash2 = (ClanHash *)pv2;

	return pstClanHash1->uiClanId - pstClanHash2->uiClanId;
}

unsigned int ClanHashCode(const void* pvData)
{
	ClanHash *pstClanHash1 = (ClanHash *)pvData;
	
	return pstClanHash1->uiClanId;
}

int clan_get_id(ZONESVRENV *pstEnv)
{
	++pstEnv->pstGlobalObj->stGlobal.ClanLastIdx;
	pstEnv->pstGlobalObj->stGlobalObjInfo.iDirty = 2;
	
	return pstEnv->pstGlobalObj->stGlobal.ClanLastIdx;
}

ZoneClan* player_get_clan(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	ClanHash stClanHash;
	ClanHash *pstClanHash;
	ZoneClan *pstZoneClan;
	CLANINFO *pstClanInfo = &pstPlayer->stRoleData.MiscInfo.ClanInfo;

	if (pstClanInfo->ClanId.GID == 0)
	{
		return NULL;
	}

	// 战场现在没有公会数据,只能如此处理
	if (pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		return NULL;
	}

	if (pstEnv->pstClanObj->stClanObjInfo.iClanStat != CLAN_STAT_FINI)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "when player_get_clan clan stat %d",
		           pstEnv->pstClanObj->stClanObjInfo.iClanStat);
	}

	if (0 <= pstClanInfo->Idx &&
		pstClanInfo->Idx < pstEnv->pstClanObj->iClanNum)
	{
		if (pstEnv->pstClanObj->astZoneClan[
			pstClanInfo->Idx].stClan.ClanProf.GID == pstClanInfo->ClanId.GID)
		{
			return &pstEnv->pstClanObj->astZoneClan[pstClanInfo->Idx];
		}
	}

	stClanHash.uiClanId = pstClanInfo->ClanId.Id;
	pstClanHash = sht_find(pstEnv->pstShtClan, &stClanHash, ClanHashCmp,
	                       ClanHashCode);
	if (NULL == pstClanHash ||
		pstClanHash->ullClanGID != pstClanInfo->ClanId.GID)
	{
		//memset(pstClanInfo, 0, sizeof(*pstClanInfo));
		return NULL;
	}

	if (0 > pstClanHash->iIdx ||
		pstClanHash->iIdx >= pstEnv->pstClanObj->iClanNum)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "hash error,iIdx=%d,iClanNum=%d",pstClanHash->iIdx,pstEnv->pstClanObj->iClanNum);
		return NULL;
	}

	pstZoneClan = &pstEnv->pstClanObj->astZoneClan[pstClanHash->iIdx];
	if (pstZoneClan->stClan.ClanProf.GID != pstClanHash->ullClanGID)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "hash error");
		return NULL;
	}

	pstClanInfo->Idx = pstClanHash->iIdx;

	if (pstZoneClan->stZoneClanInfo.iDirty == CACHE_FLAG_INEXIST)
	{
		//memset(pstClanInfo, 0, sizeof(*pstClanInfo));
		return NULL;
	}

	return pstZoneClan;
}

int clan_role_login(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	ZoneClan *pstClan = NULL;
	CLANMEM * pstMem = NULL;
	CLANINFO *pstClanInfo = &pstPlayer->stRoleData.MiscInfo.ClanInfo;
	int iOldPostion = pstClanInfo->Position;


	if (pstClanInfo->ClanId.GID == 0)
	{
		return 0;
	}

	if (CLAN_STAT_FAIL == pstEnv->pstClanObj->stClanObjInfo.iClanStat)
	{
		return 0;
	}

	// Lazy Update
	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
	{
		if (CLAN_STAT_FINI == pstEnv->pstClanObj->stClanObjInfo.iClanStat)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, 
			              ZONEERR_CLAN2);
			/*
			bzero(pstClanInfo, sizeof(*pstClanInfo));
			player_clan_disable(pstEnv, pstPlayer);
			del_task_clan(pstEnv, pstPlayer);*/

			clan_mem_on_exit(pstEnv, pstClan, pstPlayer, 0);
		}
		return 0;
	}

	pstMem = clan_id_mem(pstClan, pstPlayer->stRoleData.RoleID,
			                  &pstPlayer->stOnline.iClanMemIdx);
	if (pstMem == NULL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, 
		               ZONEERR_CLAN3);
		/*
		bzero(pstClanInfo, sizeof(*pstClanInfo));
		player_clan_disable(pstEnv, pstPlayer);
		del_task_clan(pstEnv, pstPlayer);*/

		clan_mem_on_exit(pstEnv, pstClan, pstPlayer, 0);
		return 0;
	}

	// 更新成员信息
	pstMem->Online = ROLE_GAME_ONLINE;
	pstMem->LastLogin = pstEnv->pstAppCtx->stCurr.tv_sec;

	pstClanInfo->Position = pstMem->Position;
	if(iOldPostion != pstClanInfo->Position)
	{
		clan_action(pstEnv, pstClan, pstMem);
	}
	
	++pstClan->stZoneClanInfo.iOnlineCount;
	if (0 == pstMem->JoinTime)
	{
		pstMem->JoinTime = pstEnv->pstAppCtx->stCurr.tv_sec - 7*24*3600;
	}

	if (pstMem->Level != pstPlayer->stRoleData.Level ||
		pstMem->Contribute != pstClanInfo->Contribute)
	{
		pstMem->Level = pstPlayer->stRoleData.Level;
		pstMem->Contribute = pstClanInfo->Contribute;
		
	}

	//pstMem->TotalContribute = max(pstMem->TotalContribute,pstClanInfo->Contribute);

	// 公会解散时间通知
	clan_destroy_warning(pstEnv, pstClan, pstPlayer);

	// 通知上线
	player_clan_mem_chg_broadcast(pstEnv, pstClan, pstMem, CLAN_MEMBER_CHG_MOD);

	// Lazy Update
	if (player_clan_capability_check(pstEnv, pstPlayer,
	                                 CLAN_OP_CENSOR_JOIN_CLAN) >= 0 ||
		player_clan_capability_check(pstEnv, pstPlayer,
	                                 CLAN_OP_CENSOR_JOIN_UNION) >= 0)
	{
		clan_update_diplomatism(pstEnv, pstClan, pstPlayer);
	}

	// 申请加入公会列表通知
	if (player_clan_capability_check(pstEnv, pstPlayer,
	                                 CLAN_OP_CENSOR_JOIN_CLAN) >= 0)
	{
		clan_update_apply(pstEnv, pstClan, pstPlayer);
		if (pstClan->stClan.ClanInner.ClanApplyNum > 0)
		{
			CSPKG stPkg;

			// 使其图标闪烁
			stPkg.Body.ClanSvr.Operation = CLAN_SVR_NOTIFY;
			stPkg.Body.ClanSvr.ClanSvrData.ClanNotify.Code = CLAN_NOTIFY_MEMBER;

			Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
			              ZONEERR_CLAN4);
		}
	}

	// 申请结盟列表通知
	if (player_clan_capability_check(pstEnv, pstPlayer,
	                                 CLAN_OP_CENSOR_JOIN_UNION) >= 0)
	{
		if (pstClan->stClan.ClanInner.ClanApplyFriendNum > 0)
		{
			CSPKG stPkg;

			// 使其图标闪烁
			stPkg.Body.ClanSvr.Operation = CLAN_SVR_NOTIFY;
			stPkg.Body.ClanSvr.ClanSvrData.ClanNotify.Code = 
				CLAN_NOTIFY_DIPLOMATISM;

			Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
			               ZONEERR_CLAN5);
		}
	}

	// 申请敌对列表通知
	if (player_clan_capability_check(pstEnv, pstPlayer,
	                                 CLAN_OP_CENSOR_JOIN_ENEMY) >= 0)
	{
		if (pstClan->stClan.ClanInner.ClanApplyEnemyNum > 0)
		{
			CSPKG stPkg;

			// 使其图标闪烁
			stPkg.Body.ClanSvr.Operation = CLAN_SVR_NOTIFY;
			stPkg.Body.ClanSvr.ClanSvrData.ClanNotify.Code = 
				CLAN_NOTIFY_DIPLOMATISM;

			Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
			               ZONEERR_CLAN6);
		}
	}

	pstClan->stClan.ClanActiveTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	player_clan_yesterday_clan_touch_notify(pstEnv,pstPlayer);
	player_dyn_active_helper_info(pstEnv,pstPlayer,CLAN_TOUCH_ACTIVE_HELP_ID,pstClan->stZoneClanInfo.iClanTouchStatus);

		//成长之路
	{
		ACHIEVEMENTCOND stCond;

		memset(&stCond,0,sizeof(stCond));
		
		
		stCond.Type = COND_CLAN_LEVEL;
		stCond.Val1 = pstClan->stClan.ClanProf.Level;		

		z_player_achievement_cond(pstEnv,pstPlayer,&stCond);			
		
	}
	return 0;
}


int clan_role_logout(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CLANMEM *pstMem;
	ZoneClan *pstClan;
	CLANINFO *pstClanInfo = &pstPlayer->stRoleData.MiscInfo.ClanInfo;

	if (pstClanInfo->ClanId.GID == 0)
	{
		return 0;
	}

	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
	{
		return -1;
	}

	pstMem = clan_id_mem(pstClan, pstPlayer->stRoleData.RoleID,
	                     &pstPlayer->stOnline.iClanMemIdx);
	if (!pstMem)
	{
		return -1;
	}

	pstMem->Online = 0;
	pstMem->Contribute = pstClanInfo->Contribute;
	//pstMem->TotalContribute = max(pstMem->TotalContribute,pstClanInfo->Contribute);
	pstMem->LastOnline = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstMem->TotalOnline += (pstMem->LastOnline - pstMem->LastLogin);
	pstMem->Level = pstPlayer->stRoleData.Level;
	--pstClan->stZoneClanInfo.iOnlineCount;

	player_clan_mem_chg_broadcast(pstEnv, pstClan, pstMem, CLAN_MEMBER_CHG_MOD);
	return 0;
}

int clan_reidx_hash(ZONESVRENV* pstEnv, int iIdx)
{
	int i;
	ClanHash stClanHash;
	ClanHash *pstClanHash;
	ZoneClan *pstZoneClan;
	ClanObj *pstClanObj = pstEnv->pstClanObj;

	for(i = iIdx; i < pstClanObj->iClanNum; ++i)
	{
		pstZoneClan = &pstClanObj->astZoneClan[i];

		stClanHash.uiClanId = pstZoneClan->stClan.ClanProf.Id;
		pstClanHash = sht_find(pstEnv->pstShtClan, &stClanHash, ClanHashCmp,
		                       ClanHashCode);
		if (!pstClanHash ||
			pstClanHash->ullClanGID != pstZoneClan->stClan.ClanProf.GID)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "clan hash error");
			continue;
		}
		pstClanHash->iIdx = i;
	}
	
	return 0;
}

int clan_get_count(ZONESVRENV* pstEnv, ClanObj *pstClanObj)
{
	DBAsyn stDBAsyn;
	CLAN stClan;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTCOUNTREQ *pstSelectCountReq = &pstPkg->Body.SelectCountReq;

	stDBAsyn.iCmd = DB_CLAN_COUNT_SELECT;
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head,
	                 TORM_SELECT_COUNT_REQ, "Clan", -1, &stDBAsyn);
	strcpy(pstSelectCountReq->WhereDef, "where GID != 0");
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);

	memset(&stClan, 0, sizeof(stClan));
	stMetaData.pszBuff = (char *)&stClan;
	stMetaData.iBuff = sizeof(stClan);
	if (0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData,
	                           pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}

	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}

	pstClanObj->stClanObjInfo.iClanStat = CLAN_STAT_COUNT_REQ;
	pstClanObj->stClanObjInfo.tReq = pstEnv->pstAppCtx->stCurr.tv_sec;

	return 0;
}

int clan_get(ZONESVRENV* pstEnv, ClanObj *pstClanObj)
{
	CLAN stClan;
	DBAsyn stDBAsyn;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;

	stDBAsyn.iCmd = DB_CLAN_SELECT;
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ,
	                 "Clan", -1, &stDBAsyn);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	strcpy(pstSelectReq->OperInfo.WhereDef, "where GID != 0");
	pstSelectReq->OffSet = pstClanObj->iClanNum;
	pstSelectReq->Limit = CLAN_DB_PAGE_COUNT;

	memset(&stClan, 0, sizeof(stClan));

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stClan;
	stMetaData.iBuff = sizeof(stClan);
	if (0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData,
	                           pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}

	pstClanObj->stClanObjInfo.iClanStat = CLAN_STAT_REQ;
	pstClanObj->stClanObjInfo.tReq = pstEnv->pstAppCtx->stCurr.tv_sec;
	return 0;
}

int clan_count_res(ZONESVRENV *pstEnv, TORMSELECTCOUNTRES *pstSelectCountRes)
{
	ClanObj *pstClanObj = pstEnv->pstClanObj;

	if (pstClanObj->stClanObjInfo.iClanStat != CLAN_STAT_COUNT_REQ)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "clan stat %d",
		           pstClanObj->stClanObjInfo.iClanStat);
		return -1;
	}
	
	pstClanObj->stClanObjInfo.iClanDBCount = pstSelectCountRes->RowsCount;
	if (0 == pstClanObj->stClanObjInfo.iClanDBCount)
	{
		pstClanObj->stClanObjInfo.iClanStat = CLAN_STAT_FINI;
	}
	else
	{
		clan_get(pstEnv, pstClanObj);
	}
	
	return 0;
}

int ZoneClanCmp(const void *pv1, const void *pv2)
{
	ZoneClan *pstZoneClan1 = (ZoneClan *)pv1;
	ZoneClan *pstZoneClan2 = (ZoneClan *)pv2;

	return pstZoneClan1->stClan.ClanProf.Id - pstZoneClan2->stClan.ClanProf.Id;
}

//公会初始化的时候检查下宣战cd时间
void clan_init_enemy_cd(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	CLANENEMYCD* pstCD = &pstZoneClan->stClan.ClanInner.ClanEnemyCD;
	int i = 0;
	CLANLEVELDEF* pstClanLevel = NULL;
	int iDiff = 0;

	clan_update_enemy_cd(pstEnv,pstZoneClan);

	pstClanLevel = z_clan_level_def(pstEnv, pstZoneClan->stClan.ClanProf.Level);

	if (!pstClanLevel)
	{
		return;
	}

	//搜索
	for (i = 0 ; i < pstCD->CDNum - 1; i++)
	{
		iDiff = (int)pstCD->EnemyCD[i].EndTime - pstEnv->pstAppCtx->stCurr.tv_sec;
		if (iDiff > MAX_COOL_TIME_ERR_MUL * pstClanLevel->EnemyCDTime)
		{
			pstCD->EnemyCD[i].EndTime = pstClanLevel->EnemyCDTime;
		}
	}

	clan_update_enemy_cd(pstEnv,pstZoneClan);
	
	return;
	
}

int clan_init_fini(ZONESVRENV *pstEnv, ClanObj *pstClanObj)
{
	int i;
	unsigned int uiOldID = 0;
	TMEMBLOCK *pstMemBlock;
	Player *pstPlayerOnline;
	ZoneClan *pstZoneClan;
	ClanHash stClanHash;
	ClanHash *pstClanHash;
	
	pstEnv->pstClanObj->stClanObjInfo.iClanStat = CLAN_STAT_FINI;

	if (pstClanObj->iClanNum > 1)
	{
		qsort(pstClanObj->astZoneClan, pstClanObj->iClanNum,
		      sizeof(pstClanObj->astZoneClan[0]), ZoneClanCmp);
	}

	if (pstClanObj->iClanNum > 0)
	{
		if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat ==
			GLOBAL_STAT_FINI)
		{
			if (pstClanObj->astZoneClan[
				pstClanObj->iClanNum-1].stClan.ClanProf.Id >
				pstEnv->pstGlobalObj->stGlobal.ClanLastIdx)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0,
				           "max clan id %d, global last clan idx %d",
				           pstClanObj->astZoneClan[
				           pstClanObj->iClanNum-1].stClan.ClanProf.Id,
				           pstEnv->pstGlobalObj->stGlobal.ClanLastIdx);
				pstEnv->pstGlobalObj->stGlobal.ClanLastIdx = 
		        pstClanObj->astZoneClan[
		        	pstClanObj->iClanNum-1].stClan.ClanProf.Id + 1;
			}
		}
	}

	pstEnv->pstClanObj->stClanObjInfo.iListVersion =
		pstEnv->pstAppCtx->stCurr.tv_sec;

	//建立clan hash 
	for(i = 0; i < pstClanObj->iClanNum; ++i)
	{
		pstZoneClan = &pstClanObj->astZoneClan[i];
		if (pstZoneClan->stClan.ClanProf.Id <= uiOldID)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "clan id error");
			pstEnv->pstClanObj->stClanObjInfo.iClanStat = CLAN_STAT_FAIL;
			return -1;
		}

		pstZoneClan->stZoneClanInfo.iListVersion =
			pstEnv->pstAppCtx->stCurr.tv_sec;
		

		uiOldID = pstZoneClan->stClan.ClanProf.Id;

		stClanHash.uiClanId = pstZoneClan->stClan.ClanProf.Id;
		pstClanHash = sht_insert_unique(pstEnv->pstShtClan, &stClanHash, 
		                                ClanHashCmp, ClanHashCode);
		if (NULL == pstClanHash)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "clan hash error");

			pstEnv->pstClanObj->stClanObjInfo.iClanStat = CLAN_STAT_FAIL;
			return -1;
		}

		pstClanHash->iIdx = i;
		pstClanHash->uiClanId = pstZoneClan->stClan.ClanProf.Id;
		pstClanHash->ullClanGID = pstZoneClan->stClan.ClanProf.GID;

		clan_advance_init(pstEnv, pstZoneClan);
		
		clan_init_enemy_cd(pstEnv, pstZoneClan);

		clan_touch_check(pstEnv,pstZoneClan,0);

		clan_officers_check(pstEnv, pstZoneClan);
	}

	//如果有玩家在线
	for(i = 0; i < MAX_PLAYER; ++i)
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

		clan_role_login(pstEnv, pstPlayerOnline);
	}

	// 创建全部公会公会城市副本
	clan_init_all_city_pworld(pstEnv);
	clan_init_touch_status(pstEnv);
	return 0;
}

int clan_touch_check_all(ZONESVRENV *pstEnv)
{
	int i;

	ClanObj *pstClanObj = pstEnv->pstClanObj;
	
	if (pstClanObj->stClanObjInfo.iClanStat != CLAN_STAT_FINI)
	{
		return 0;
	}

	if (pstClanObj->iClanNum < 1)
	{
		return 0;
	}


	for(i = 0; i < pstClanObj->iClanNum; ++i)
	{
		ZoneClan* pstZoneClan = &pstClanObj->astZoneClan[i];
	
		clan_touch_check(pstEnv,pstZoneClan,1);
	}

	


	return 0;
}

int clan_spy_open_ui_all(ZONESVRENV *pstEnv)
{
	int i;

	ClanObj *pstClanObj = pstEnv->pstClanObj;
	
	if (pstClanObj->stClanObjInfo.iClanStat != CLAN_STAT_FINI)
	{
		return 0;
	}

	if (pstClanObj->iClanNum < 1)
	{
		return 0;
	}


	for(i = 0; i < pstClanObj->iClanNum; ++i)
	{
		ZoneClan* pstZoneClan = &pstClanObj->astZoneClan[i];
	
		clan_active_ui_info_clan_spy(pstEnv, pstZoneClan,ACTIVE_GAME_SVR_TYPE_UI_INFO);

		if (pstZoneClan)
		{
			pstZoneClan->stClan.ClanProf.Flag |= CLAN_FLAG_TASK_SPY;
			clan_property_modify(pstEnv, pstZoneClan,CLAN_PROPERTY_FLAG, 
									pstZoneClan->stClan.ClanProf.Flag ,-1);
		}
	}
	


	return 0;
}



int clan_select_res(ZONESVRENV *pstEnv, TORMSELECTRES *pstSelectRes)
{
	int i;
	ZoneClan *pstZoneClan;
	ClanObj *pstClanObj = pstEnv->pstClanObj;

	if (pstEnv->pstClanObj->stClanObjInfo.iClanStat  != CLAN_STAT_REQ)
	{
		return -1;
	}
	
	if (0 == pstSelectRes->SelectResult.ResultNum ||
		pstSelectRes->SelectResult.ResultNum !=
		(int)pstSelectRes->SelectResult.ResultTotal)
	{
		pstEnv->pstClanObj->stClanObjInfo.iClanStat = CLAN_STAT_INIT;
		return -1;
	}

	for(i = 0; i < pstSelectRes->SelectResult.ResultNum; ++i)
	{
		pstZoneClan = &pstClanObj->astZoneClan[pstClanObj->iClanNum];
		bzero( pstZoneClan, sizeof(*pstZoneClan));
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&pstZoneClan->stClan,
		                            sizeof(pstZoneClan->stClan)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,	"torm_fetch_one_meta fail");
			pstEnv->pstClanObj->stClanObjInfo.iClanStat = CLAN_STAT_INIT;
			return -1;
		}

		clan_check_old_data(pstEnv, pstZoneClan);

		pstZoneClan->stZoneClanInfo.iListVersion =
			pstEnv->pstAppCtx->stCurr.tv_sec;

		pstZoneClan->stZoneClanInfo.iTimerCount = rand();

		// 公会物资消耗时间重新计时
		pstZoneClan->stClan.ClanCore.NextCostTime = 0;
		++pstClanObj->iClanNum;
	}

	if (pstClanObj->iClanNum < pstClanObj->stClanObjInfo.iClanDBCount)
	{
		clan_get(pstEnv, pstClanObj);
	}
	else
	{
		clan_init_fini(pstEnv, pstClanObj);
	}
	return 0;
}


int clan_destroy(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan)
{
	//int i;	//comment by paraunused.pl
	int iIdx;
	ClanHash stClanHash;
	ClanHash *pstClanHash = NULL;
	ClanObj *pstClanObj = pstEnv->pstClanObj;

	tlog_info(pstEnv->pstLogCat, 0, 0, "clan %lld %d destroy",
	          pstZoneClan->stClan.ClanProf.GID,
	          pstZoneClan->stClan.ClanProf.Id);

	iIdx = pstZoneClan - &pstClanObj->astZoneClan[0];
	
	// 处理各种外交关系并通知对方
	// 外交关系无需主动更新了，因为有Lazy Updating.

	if (pstZoneClan->stZoneClanInfo.fWillDelayDestroy)
	{
		clan_partial_destroy(pstEnv, pstZoneClan);
	}

	player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD206);

	mon_clan_factory_del(pstEnv, pstZoneClan->stClan.ClanProf.GID);

	//删除clan hash
	stClanHash.uiClanId = pstZoneClan->stClan.ClanProf.Id;
	pstClanHash = sht_find(pstEnv->pstShtClan, &stClanHash, ClanHashCmp,
	                       ClanHashCode);
	if (NULL == pstClanHash ||
		pstClanHash->ullClanGID != pstZoneClan->stClan.ClanProf.GID)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "clan hash error");
	}
	else
	{
		sht_remove(pstEnv->pstShtClan, &stClanHash, ClanHashCmp, ClanHashCode);
	}

	--pstClanObj->iClanNum;
	if (iIdx != pstClanObj->iClanNum)
	{
		memmove(pstZoneClan, pstZoneClan + 1,
		        (pstClanObj->iClanNum - iIdx) * sizeof(*pstZoneClan));
	}

	//整理clan hash
	clan_reidx_hash(pstEnv, iIdx);
	++pstClanObj->stClanObjInfo.iListVersion;
	return 0;
}


int clan_delete_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstTormPkg, 
                    unsigned int nClanID)
{
	TORMDELETERES *pstDeleteRes = &pstTormPkg->Body.DeleteRes;
	ZoneClan *pstClan = NULL;

	UNUSED(pstAppCtx);

	if (TORM_ERR_IS_ERROR(pstDeleteRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "db delete clan failed.");
		return -1;
	}

	pstClan = z_id_clan(pstEnv, nClanID);
	if (!pstClan)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "can not find clan.");
		return -1;
	}
	
	return clan_destroy(pstEnv, pstClan);
}


int clan_update_res(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, TORMPKG *pstTormPkg, 
                    unsigned int nClanID)
{
	TORMUPDATERES *pstUpdateRes = &pstTormPkg->Body.UpdateRes;
	ZoneClan *pstClan = NULL;

	UNUSED(pstAppCtx);

	if (TORM_ERR_IS_ERROR(pstUpdateRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "db update clan failed." );
		return -1;
	}

	pstClan = z_id_clan(pstEnv, nClanID);
	if (!pstClan)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "can not find clan." );
		return -1;
	}

	clan_set_clean(pstClan);
	return 0;
}

int clan_init(ZONESVRENV *pstEnv, ClanObj *pstClanObj)
{
	static int iRetry = 0;
	
	switch(pstClanObj->stClanObjInfo.iClanStat)
	{
	case CLAN_STAT_INIT:
		if (iRetry > 6)
		{
			pstClanObj->stClanObjInfo.iClanStat = CLAN_STAT_FAIL;
		}
		else
		{
			pstEnv->pstClanObj->iClanNum = 0;
			memset(&pstEnv->pstClanObj->stClanObjInfo, 0, sizeof(pstEnv->pstClanObj->stClanObjInfo));
			clan_get_count(pstEnv, pstClanObj);
		}
		break;

	case CLAN_STAT_COUNT_REQ:
	case CLAN_STAT_REQ:
		if (pstClanObj->stClanObjInfo.tReq + 5 <
			pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			iRetry++;
			pstClanObj->stClanObjInfo.iClanStat = CLAN_STAT_INIT;
		}
		break;

	default:
		break;
	}
	
	return 0;
}

int clan_update(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
		
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, 
	                 "Clan", -1, NULL);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 1;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&pstZoneClan->stClan;
	stMetaData.iBuff = sizeof(pstZoneClan->stClan);
	if (0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
	                           pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}

	clan_set_clean(pstZoneClan);
	return 0;
}


int clan_update_req(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;

	stDBAsyn.iCmd = DB_CLAN_UPDATE;
	stDBAsyn.AsynCmd.nClanID = pstZoneClan->stClan.ClanProf.Id;
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, 
	                 "Clan", -1, &stDBAsyn );
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 1;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&pstZoneClan->stClan;
	stMetaData.iBuff = sizeof(pstZoneClan->stClan);
	if (0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
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


int clan_delete_req(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMDELETEREQ *pstDeleteReq = &pstPkg->Body.DeleteReq;

	stDBAsyn.iCmd = DB_CLAN_DELETE;
	stDBAsyn.AsynCmd.nClanID = pstZoneClan->stClan.ClanProf.Id;
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_DELETE_REQ, 
	                 "Clan", -1, &stDBAsyn);
	pstDeleteReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDeleteReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDeleteReq->OperInfo.WhereDef[0] = 0;
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&pstZoneClan->stClan;
	stMetaData.iBuff = sizeof(pstZoneClan->stClan);
	if (0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
	                           pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}

	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}

	tlog_info(pstEnv->pstLogCat, 0, 0, "clan %lld %d destroy req", 
	          pstZoneClan->stClan.ClanProf.GID,
	          pstZoneClan->stClan.ClanProf.Id);
	
	return 0;
}

int clan_city_buff_chg(ZONESVRENV *pstEnv,ZoneClan *pstZoneClan,int iStatusID,
	int iLevel,int iTime)
{
	int i;
	Player *pstPlayer = NULL;

	ZONEPWORLDINST *pstZonePworldInst;

	pstZoneClan->stClan.ClanCore.CityStatusID = iStatusID;
	pstZoneClan->stClan.ClanCore.CityStatusLevel = iLevel;
	pstZoneClan->stClan.ClanCore.CityStatusTime = iTime;

	pstZonePworldInst = z_find_clan_city_inst(pstEnv, pstZoneClan);
	if (NULL == pstZonePworldInst)
	{
		return -1;
	}

	for(i = 0; i < pstZonePworldInst->iPlayer;i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx,pstEnv,pstZonePworldInst->aiPlayerID[i]);
		if(!pstPlayer)
		{
			continue;
		}
		if ( pstPlayer->stOnline.State & CS_STAT_DEAD)
		{
			continue;
		}
		if ( pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID != 
			pstZoneClan->stClan.ClanProf.GID)
		{
			continue;
		}

		z_player_apply_status(pstEnv->pstAppCtx,pstEnv,pstPlayer, pstEnv->iClanCityInitStatusID,
								pstEnv->iClanCityInitStatusLevel, pstEnv->iClanCityInitStatusTime,1, 0);
		
	}
	return 0;
}

int clan_skill_valid_level_chg(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan,CLANSKILL *pstClanSkill)
{
	CLANSKILLDEF* pstDef = NULL;
	PLAYERCLANSKILLINFO* pstInfo = NULL;	
	int i = 0;
	int k = 0;
	int iNum = MAX_CLAN_SKILL_NUM;
	Player *pstPlayer = NULL;

	pstDef = z_find_clan_skill_def(pstEnv,pstClanSkill->SkillID,pstClanSkill->Level);
	if (!pstDef)
	{
		return -1;
	}

	for (i = 0; i < MAX_CLAN_SKILL_RESULT; i++)
	{
		if (!pstDef->Result[i].ResultID)
		{
			break;
		}
		
		if (pstDef->Result[i].ResultID != RESULT_CLAN_CITY_BUFF)
		{
			continue;
		}
		
		if (0 == pstClanSkill->ValidLevel)
		{
			clan_city_buff_chg(pstEnv,pstZoneClan,0,0,0);
		}
		else
		{
			CLANSKILLDEF* pstDefValid = NULL;
			pstDefValid = z_find_clan_skill_def(pstEnv,pstClanSkill->SkillID,pstClanSkill->ValidLevel);
			if (pstDefValid)
			{
				for(k = 0; k < MAX_CLAN_SKILL_RESULT;k++)
				{
					if (!pstDefValid->Result[k].ResultID)
					{
						break;
					}
					
					if (pstDefValid->Result[k].ResultID != RESULT_CLAN_CITY_BUFF)
					{
						continue;
					}

					clan_city_buff_chg(pstEnv,pstZoneClan,pstDefValid->Result[k].ResultVal1,
						pstDefValid->Result[k].ResultVal2,pstDefValid->Result[k].ResultVal3);

					break;
					
				}
			}
		}
		break;
	}

	if (pstDef->AutoLearn == 2)
	{
		return 0;
	}

	for(i = 0; i < pstZoneClan->stClan.ClanProf.MemNum;i++)
	{
		if (!pstZoneClan->stClan.ClanCore.ClanMem[i].Online)
		{
			continue;
		}

		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
						 pstZoneClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName);
		if (!pstPlayer)
		{
			continue;
		}

		if (pstDef->AutoLearn == 1)
		{
			//刷新角色属性
			z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}
		else
		{
			pstInfo = &pstPlayer->stRoleData.MiscInfo.ClanSkillInfo;

			iNum = min(iNum,pstInfo->Num);

			for (k = 0; k < iNum;k++)
			{
				if (pstInfo->ClanSkill[k].SkillID == pstClanSkill->SkillID)
				{
					//刷新角色属性
					z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
					break;
				}
			}
		}
	}
	return 0;
}

int clan_level_down(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan)
{
	int iNum = MAX_CLAN_SKILL_NUM;
	CLANSKILLINFO* pstInfo = NULL;
	int i = 0;
	int k = 0;
	CLANSKILLDEF* pstDef = NULL;
	int iChg = 0;
	OPCLANDATA stData;
	
	if(pstZoneClan->stClan.ClanProf.Level <= 1)
	{
		return -1;
	}

	stData.ClanLevelCh.ClanLevelPre = pstZoneClan->stClan.ClanProf.Level;

	clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_LEVEL, 1, 1);

	stData.ClanLevelCh.ClanLevel = pstZoneClan->stClan.ClanProf.Level;

	// 升级后的数值变化
	/*
	pstZoneClan->stClan.ClanAdvance.Storage.MaxMachineNum =
		pstNextDef->MaxMachineGrid;
	clan_property_notify(pstEnv, pstZoneClan,
						 CLAN_PROPERTY_MAX_MACHINE_GRID_NUM);

	pstZoneClan->stClan.ClanAdvance.Storage.MaxResourceNum =
		pstNextDef->MaxResourceGrid;
	clan_property_notify(pstEnv, pstZoneClan,
						 CLAN_PROPERTY_MAX_RESOURCE_GRID_NUM);
	*/
	++pstZoneClan->stZoneClanInfo.iListVersion;
	clan_set_dirty(pstZoneClan, CACHE_FLAG_IMM);

	player_clan_view_chg(pstEnv, pstZoneClan, CLAN_VIEW_CHG_MOD);

	z_role_clan_oplog(pstEnv, NULL, clan_id_warp(&pstZoneClan->stClan),
			  OP_CLAN_LEVEL_CH,&stData);

	clan_level_down_group_clear(pstEnv, pstZoneClan);

	
	pstInfo = &pstZoneClan->stClan.ClanAdvance.ClanSkillInfo;
	
	iNum = min(iNum,pstInfo->Num);


	for(i = 0; i < iNum; i++)
	{
		if(MEM_NUM_CLAN_SKILL_ID == pstInfo->ClanSkill[i].SkillID)
		{
			continue;
		}
		
		int iValidLevel = pstInfo->ClanSkill[i].ValidLevel;
		for( k = pstInfo->ClanSkill[i].ValidLevel;k >= 1;k--)
		{
			pstDef = z_find_clan_skill_def(pstEnv,pstInfo->ClanSkill[i].SkillID ,k);
			if (!pstDef)
			{
				continue;
			}
			if (pstDef->ResearchNeedClanLevel > pstZoneClan->stClan.ClanProf.Level)
			{
				iValidLevel = k - 1;
			}
			else
			{
				break;
			}
		}
		
		if (iValidLevel != pstInfo->ClanSkill[i].ValidLevel)
		{
			iChg = 1;
			pstInfo->ClanSkill[i].ValidLevel = iValidLevel;
			clan_skill_valid_level_chg(pstEnv,pstZoneClan,&pstInfo->ClanSkill[i]);

			if(BUSINESS_CLAN_SKILL_ID == pstInfo->ClanSkill[i].SkillID)
			{
				//公会商运技能有效级别一改。缓存中的ClanBusinessExpMul 也要修改
				clan_skill_business_exp_jian_yan(pstEnv,pstZoneClan);
			}
		}
	}

	if (iChg)
	{
		clan_action_clan_skill_info(pstEnv,pstZoneClan);
	}
	

	return 0;
}

int clan_spend_notify(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneClan *pstClan)
{	
	CSPKG stPkg;
	ZoneClan *pstZoneClan = NULL;

	if (pstPlayer)
	{
		pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	}

	if (pstClan)
	{
		pstZoneClan = pstClan;
	}

	if (!pstZoneClan)
		return 0;
	
	stPkg.Body.ClanSvr.Operation = CLAN_SVR_SPEND_INFO;
	stPkg.Body.ClanSvr.ClanSvrData.ClanSpend.LastTime = pstZoneClan->stClan.ClanCore.LastCostMoneyTime;
	stPkg.Body.ClanSvr.ClanSvrData.ClanSpend.NextTime = pstZoneClan->stClan.ClanCore.NextCostTime;

	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	if (pstPlayer)
	{
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	else
	{
		player_clan_broadcast(pstEnv, pstZoneClan, &stPkg, 0);
	}
	
	return 0;
}

static int clan_process_cost(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan,
                             int *pfHasCost)
{
	const CLANLEVELDEF *pstLvDef = NULL;
	const CLANWORKINGCOST *pstCostDef = NULL;
	int iCostMoney = 0;
	int iTime = 0;

	// 定时消耗公会物资
	pstLvDef = z_clan_level_def(pstEnv, pstZoneClan->stClan.ClanProf.Level);
	if (!pstLvDef)
	{
		return -1;
	}
	pstCostDef = &pstLvDef->WorkingCost;
	if (!pstZoneClan->stClan.ClanCore.NextCostTime)
	{
		pstZoneClan->stClan.ClanCore.NextCostTime =
			pstEnv->pstAppCtx->stCurr.tv_sec + pstCostDef->IntervalTime;
		pstZoneClan->stClan.ClanCore.LastCostMoneyTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		clan_spend_notify(pstEnv, NULL,pstZoneClan);
		return 0;
	}
	else
	{
		if (pstEnv->pstAppCtx->stCurr.tv_sec < 
			pstZoneClan->stClan.ClanCore.NextCostTime)
		{
			return 0;
		}
	}

	//顺便检查下会长
	clan_leader_check(pstEnv, pstZoneClan);

	if (0 == pstZoneClan->stClan.ClanCore.LastCostMoneyTime ||
		pstZoneClan->stClan.ClanCore.LastCostMoneyTime > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		pstZoneClan->stClan.ClanCore.LastCostMoneyTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	iCostMoney = pstCostDef->Money;
	iTime = pstEnv->pstAppCtx->stCurr.tv_sec - pstZoneClan->stClan.ClanCore.LastCostMoneyTime;
	
	if (pstCostDef->IntervalTime > 0 )
	{
		iCostMoney = (iTime/pstCostDef->IntervalTime) * pstCostDef->Money;
		if (iCostMoney < pstCostDef->Money)
		{
			iCostMoney = pstCostDef->Money;
		}
	}

	//维护消耗=每日消耗*（1+(100-当前安定度/100)^2/2000）
	{
		double a = (100 - pstZoneClan->stClan.ClanProf.StableValue/100.0);
		iCostMoney = iCostMoney * (1 + a * a /2000);
	}
	
	if ((unsigned int)iCostMoney > pstZoneClan->stClan.ClanProf.Money)
	{		
		if (iTime >= pstLvDef->LevelDownTime)
		{
			if (pstZoneClan->stClan.ClanProf.Level > 1)
			{
				if (0 == clan_level_down(pstEnv, pstZoneClan))
				{
					player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
						ZONEERR_CLANBROAD207,iCostMoney,  pstZoneClan->stClan.ClanProf.Money,
						pstZoneClan->stClan.ClanProf.Level);
					//旧账一笔勾销
					pstZoneClan->stClan.ClanCore.LastCostMoneyTime = pstEnv->pstAppCtx->stCurr.tv_sec;
				}
			}
			else
			{
				//解散公会
				clan_disband(pstEnv,pstZoneClan,"");				
			}
		}
		else
		{
			char szTime[256] = {0};
			iTime = pstLvDef->LevelDownTime - iTime;
			
			time_text_zh(iTime,szTime);
			


			if ( 1== pstZoneClan->stClan.ClanProf.Level)
			{
				player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
					ZONEERR_CLANBROAD208,
					iCostMoney,pstZoneClan->stClan.ClanProf.Money,szTime);

				//一级不累计
				//pstZoneClan->stClan.ClanCore.LastCostMoneyTime = pstEnv->pstAppCtx->stCurr.tv_sec;
			}
			else
			{
				player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
					ZONEERR_CLANBROAD209,
					iCostMoney,pstZoneClan->stClan.ClanProf.Money,szTime);				
			}
		}
	}

	// 消耗公会物资
	g_iOptype = OP_OPTYPE_TIMER;
	if (iCostMoney && (unsigned int)iCostMoney <= pstZoneClan->stClan.ClanProf.Money)
	{
		clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_MONEY,
		                     min(iCostMoney,
		                         (int)pstZoneClan->stClan.ClanProf.Money), 1);
		
		player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
			ZONEERR_CLANBROAD210,
			iCostMoney,pstZoneClan->stClan.ClanProf.Money);

		pstZoneClan->stClan.ClanCore.LastCostMoneyTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_STABLE_VALUE,
	                     min(pstCostDef->StableValue,
	                         (int)pstZoneClan->stClan.ClanProf.StableValue),
	                     1);
	clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_ACTIVE_VALUE,
	                     min(pstCostDef->ActiveValue,
	                         (int)pstZoneClan->stClan.ClanProf.ActiveValue), 1);
	g_iOptype = 0;

	pstZoneClan->stClan.ClanCore.NextCostTime =
		pstEnv->pstAppCtx->stCurr.tv_sec + pstCostDef->IntervalTime;

	clan_spend_notify( pstEnv,NULL, pstZoneClan);
		
	clan_set_dirty(pstZoneClan, CACHE_FLAG_DIRTY);
	*pfHasCost = 1;
	return 0;
}

static inline const char *mktimef(time_t tJiff, char *pszBuf, size_t n)
{
	struct tm stLocal;
	localtime_r(&tJiff, &stLocal);
	snprintf(pszBuf, n, "%02d%s%02d%s", stLocal.tm_hour, LJY_HOUR1, stLocal.tm_min, LJY_MINUTE1);
	return pszBuf;
}

int clan_diplomatism_tick(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan)
{
	int i = pstZoneClan->stClan.ClanInner.ClanEnemyNum;
	int fHasNotified = 0;

	while (i--)
	{
		char szBuf[64];
		CLANRELATION *pstEnemy =
			&pstZoneClan->stClan.ClanInner.ClanEnemys[i];
		ZoneClan *pstEnemyClan = z_clanid_clan(pstEnv, &pstEnemy->ClanId);
		if (!pstEnemyClan)
			return 0;

		if (pstEnv->pstAppCtx->stCurr.tv_sec >= pstEnemy->Time)
		{
			// 通知双方
			player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
			                          ZONEERR_CLANBROAD211, pstEnemy->ClanId.Name);
			player_clan_msg_broadcast(pstEnv, pstEnemyClan, SYS_CLAN,
			                          ZONEERR_CLANBROAD212,
			                          pstZoneClan->stClan.ClanProf.Name);
			
			// 战争状态结束了
			if (i != pstZoneClan->stClan.ClanInner.ClanEnemyNum - 1)
			{
				pstZoneClan->stClan.ClanInner.ClanEnemys[i] =
					pstZoneClan->stClan.ClanInner.ClanEnemys[
						pstZoneClan->stClan.ClanInner.ClanEnemyNum - 1];
			}
			--pstZoneClan->stClan.ClanInner.ClanEnemyNum;
			// 更新外交关系
			player_clan_diplomatism_chg_broadcast(pstEnv,pstZoneClan);
			
			continue;
		}

		if (!pstEnemyClan)
		{
			continue;
		}

		// 每十分钟提醒一次
		if (pstZoneClan->stZoneClanInfo.iWarNotify &&
			pstEnv->pstAppCtx->stCurr.tv_sec <
			pstZoneClan->stZoneClanInfo.iWarNotify)
		{
			continue;
		}
		mktimef(pstEnemy->Time - CLAN_WAR_PERSISTED_TIME, szBuf, CCH(szBuf));
		player_clan_msg_broadcast(pstEnv, pstEnemyClan, SYS_CLAN,
		                          ZONEERR_CLANBROAD213,
		                          pstZoneClan->stClan.ClanProf.Name, szBuf);
		fHasNotified = 1;
	}

	if (fHasNotified)
	{
		pstZoneClan->stZoneClanInfo.iWarNotify =
			pstEnv->pstAppCtx->stCurr.tv_sec + CLAN_WAR_NOTIFY_INTERVAL;
	}

	clan_update_enemy_cd(pstEnv, pstZoneClan);
	
	return 0;
}

int is_same_hour(time_t t1, time_t t2)
{
	struct tm curr1;
	struct tm curr2;
	
	localtime_r(&t1, &curr1);
	localtime_r(&t2, &curr2);

	if(!IsSameDay(t1, t2))
	{
		return 0;
	}

	if(curr1.tm_hour != curr2.tm_hour)
		return 0;

	return 1;
}

//奖励整点更新时间
int clan_map_resource_time_update(time_t t1, int *piTime)
{
	struct tm curr1;

	localtime_r(&t1, &curr1);

	curr1.tm_min = 0;
	curr1.tm_sec = 0; 
	*piTime = mktime(&curr1);
	
	return 0;
}

MAPAWARDOREDEF* z_map_award_ore_def(ZONESVRENV *pstEnv, int iMapID)
{
	MAPAWARDOREDEF  stDummy;
	int iIndex, iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDummy.MapID = iMapID;
	iIndex = bsearch_int(&stDummy, pstObj->sMapAwardOreDef, pstObj->iMapAwardOreDef,
		                 MAP_AWARD_ORE_DEF_DATA, &iEqu);
	return iEqu ? (MAPAWARDOREDEF *)pstObj->sMapAwardOreDef[iIndex] : NULL;
}

int clan_map_resource_add(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iMapID)
{
	int i;
	MAPAWARDOREDEF *pstMapAwardOreDef;

	pstMapAwardOreDef = z_map_award_ore_def(pstEnv, iMapID);
	if(!pstMapAwardOreDef)
	{
		return 0;
	}

	for(i=0; i<MAP_AWARD_ORE_LIST; i++)
	{
		if(pstMapAwardOreDef->Ore[i].OreID == 0)
		{
			continue;
		}

		clan_commit_item_byid(pstEnv, pstZoneClan, 
			pstMapAwardOreDef->Ore[i].OreID, pstMapAwardOreDef->Ore[i].Num);
	}
	
	return 0;
}

//占领奖励资源TICK
int clan_map_resource_tick(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	STRONGPOINTONE *pstStrongInfo;

	pstStrongInfo = strong_one_find_by_clan(pstEnv, pstZoneClan, 0);
	if(pstStrongInfo)
	{
		if(!is_same_hour(pstZoneClan->stClan.ClanCore.NextAddStrongPointResource, 
			pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			clan_map_resource_add(pstEnv, pstZoneClan, pstStrongInfo->MapID);

			//更新时间
			clan_map_resource_time_update(pstEnv->pstAppCtx->stCurr.tv_sec, 
					&pstZoneClan->stClan.ClanCore.NextAddStrongPointResource);
		}
	}
		
	if(is_world_city_clan(pstEnv, pstZoneClan))
	{
		if(!is_same_hour(pstZoneClan->stClan.ClanCore.NextAddCityResource, 
			pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			clan_map_resource_add(pstEnv, pstZoneClan, WORLD_CITY_MAPID);

			//更新时间
			clan_map_resource_time_update(pstEnv->pstAppCtx->stCurr.tv_sec, 
					&pstZoneClan->stClan.ClanCore.NextAddCityResource);
		}
	}
	
	return 0;
}

CLANMEM* clan_chg_leader_mem(ZONESVRENV* pstEnv, ZoneClan *pstClan)
{
	int i;
	int iTmp;
	int iContri = 0, iIdx = -1;

	for(i=0; i<(int)pstClan->stClan.ClanCore.MemNum; i++)
	{
		if(pstClan->stClan.ClanCore.ClanMem[i].Position == CLAN_POS_PRESIDENT)
		{
			continue;
		}
		
		iTmp = pstEnv->pstAppCtx->stCurr.tv_sec - 
			pstClan->stClan.ClanCore.ClanMem[i].LastOnline;
		iTmp = iTmp/86400;

		
		if((int)pstClan->stClan.ClanCore.ClanMem[i].TotalContribute > iContri &&
			iTmp < 15)
		{
			iIdx = i;
			iContri = pstClan->stClan.ClanCore.ClanMem[i].TotalContribute;
		}
	}
	
	if(iIdx >= 0)
	{
		return &pstClan->stClan.ClanCore.ClanMem[iIdx];
	}
	
	return NULL;
}

int clan_leader_check(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan)
{
	int iTmp;
	CLANMEM *pstMem, *pstNewLeader = NULL;

	pstMem = clan_id_mem(pstZoneClan,
	                     pstZoneClan->stClan.ClanBrief.Leader.RoleID, NULL);
	if (!pstMem)
	{
		return -1;
	}

	//TODO: 这里记录下更新时间，一天一次更新

	iTmp = pstEnv->pstAppCtx->stCurr.tv_sec - pstMem->LastOnline;
	iTmp = iTmp/86400;
	if(iTmp < 7)
	{
		return -1;
	}

	if(iTmp < 10)
	{
		player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD214,
	                          pstMem->RoleMember.RoleName,
	                          iTmp);
		return 0;
	}

	pstNewLeader = clan_chg_leader_mem(pstEnv, pstZoneClan);
	if(!pstNewLeader)
	{
		return -1;
	}

	pstNewLeader->Position = CLAN_POS_PRESIDENT;
	pstNewLeader->GroupID = 0;
	STRNCPY(pstNewLeader->Title, pstMem->Title, CCH(pstNewLeader->Title));
	
	pstMem->Position = CLAN_POS_NORMAL;
	pstMem->GroupID = 0;
	pstMem->Title[0] = 0;
	clan_update_officers(pstEnv, pstZoneClan);
	++pstZoneClan->stZoneClanInfo.iListVersion;

	// 更新显示
	{
		Player *pstPlayer;
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
		                          pstNewLeader->RoleMember.RoleName);
		if (pstPlayer)
		{
			clan_action(pstEnv, pstZoneClan, pstNewLeader);
		}

		player_clan_mem_chg_broadcast(pstEnv, pstZoneClan, pstMem,
	                                      CLAN_MEMBER_CHG_MOD);
		player_clan_mem_chg_broadcast(pstEnv, pstZoneClan, pstNewLeader,
	                                      CLAN_MEMBER_CHG_MOD);
	}

	player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD215,
	                          pstMem->RoleMember.RoleName,
	                          pstNewLeader->RoleMember.RoleName);
	
	return 0;
}




int clan_tick(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan)
{
	int fHasCost = 0;
	int i, iAvgTime = 0, iTotalLv = 0, iActiveValue = 0;

	// 处理公会物资消耗
	if (clan_process_cost(pstEnv, pstZoneClan, &fHasCost) < 0)
	{
		return -1;
	}

	// 计算公会繁荣度
	// 公会繁荣度 = 40*4^公会等级+40*4^城市等级+公会等级*平均在线时间*平均成员等级*公会人数/10
	iActiveValue =
		40 * pow(4, pstZoneClan->stClan.ClanProf.Level) +
		40 * pow(4, pstZoneClan->stClan.ClanCity.Level);
	i = pstZoneClan->stClan.ClanCore.MemNum;
	while(i--)
	{
		iTotalLv += pstZoneClan->stClan.ClanCore.ClanMem[i].Level;
		if (pstZoneClan->stClan.ClanCore.ClanMem[i].Online)
		{
			iAvgTime += (pstEnv->pstAppCtx->stCurr.tv_sec -
			            pstZoneClan->stClan.ClanCore.ClanMem[i].LastLogin);
		}
		else
		{
			iAvgTime += pstZoneClan->stClan.ClanCore.ClanMem[i].TotalOnline;
		}
	}

	if(!pstZoneClan->stClan.ClanCore.MemNum)
	{
		return -1;
	}
	iAvgTime /= (pstZoneClan->stClan.ClanCore.MemNum * 3600);
	iActiveValue += (pstZoneClan->stClan.ClanProf.Level * iAvgTime *
	                 iTotalLv / 10);

	if ((int)pstZoneClan->stClan.ClanProf.ActiveValue != iActiveValue)
	{
		pstZoneClan->stClan.ClanProf.ActiveValue = iActiveValue;
		clan_set_dirty_notify(pstZoneClan, CACHE_FLAG_DIRTY);
		clan_property_notify(pstEnv, pstZoneClan, CLAN_PROPERTY_ACTIVE_VALUE);
	}

	// 公会高级tick
	clan_advance_tick(pstEnv, pstZoneClan);

	// 公会建设tick
	clan_construction_tick(pstEnv, pstZoneClan);

	// 公会外交tick
	clan_diplomatism_tick(pstEnv, pstZoneClan);

	clan_map_resource_tick(pstEnv, pstZoneClan);

	//公会神兽TICK
	clan_baby_tick(pstEnv, pstZoneClan);

	//公会ROLL点tick
	clan_booty_active(pstEnv, pstZoneClan);

	// 福利tick
	clan_boon_get_online(pstEnv, pstZoneClan);
	clan_boon_clear(pstEnv, pstZoneClan);

	// 运镖tick
	clan_dart_check_end( pstEnv, pstZoneClan);

	// 公会寄养
	clan_pet_tick(pstEnv,pstZoneClan);
	
	// 公会变化的数据更新到公会查询列表
	if (pstZoneClan->stZoneClanInfo.fWillViewNotify &&
		(pstZoneClan->stZoneClanInfo.iTimerCount % 60 == 0))
	{
		player_clan_view_chg(pstEnv, pstZoneClan, CLAN_VIEW_CHG_MOD);
		pstZoneClan->stZoneClanInfo.fWillViewNotify = 0;
	}
	
	return 0;
}


int clan_timer(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	int iClanDestroyMinMem = CLAN_DESTROY_MEM_MIN;

	if (pstEnv->pstConf->CountryVer == COUNTRY_VER_KOREA)
	{
		iClanDestroyMinMem = 3;
	}
		
	++pstZoneClan->stZoneClanInfo.iTimerCount;

	clan_update_online_action(pstEnv, pstZoneClan);

	// 更新活跃时间
	if (pstZoneClan->stZoneClanInfo.iOnlineCount > 0)
	{
		pstZoneClan->stClan.ClanActiveTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		clan_set_dirty(pstZoneClan, CACHE_FLAG_DIRTY);
	}

	if (pstZoneClan->stClan.ClanActiveTime + g_iClanDestroyActiveMinSecs <
		 pstEnv->pstAppCtx->stCurr.tv_sec)	
	{
		pstZoneClan->stZoneClanInfo.fWillDelayDestroy = 1;
		clan_set_dirty(pstZoneClan, CACHE_FLAG_INEXIST);
		
		z_role_clan_oplog(pstEnv, NULL, clan_id_warp(&pstZoneClan->stClan),
					  OP_CLAN_DESTROY_ACTIVE,NULL);
	}

	if ((pstZoneClan->stClan.ClanDestroyTime != 0 &&
			 pstZoneClan->stClan.ClanDestroyTime <
			 pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstZoneClan->stZoneClanInfo.fWillDelayDestroy = 1;
		clan_set_dirty(pstZoneClan, CACHE_FLAG_INEXIST);
		
		z_role_clan_oplog(pstEnv, NULL, clan_id_warp(&pstZoneClan->stClan),
	                  OP_CLAN_DESTROY_MEMBER,NULL);
	}
	
	if (pstZoneClan->stClan.ClanProf.MemNum < iClanDestroyMinMem)
	{
		if (!pstZoneClan->stClan.ClanDestroyTime)
		{
			pstZoneClan->stClan.ClanDestroyTime =
				pstEnv->pstAppCtx->stCurr.tv_sec + g_iClanDestroyMemMinSecs;
				
			clan_set_dirty(pstZoneClan, CACHE_FLAG_DIRTY);
		}
	}
	else
	{
		int iOldTime = pstZoneClan->stClan.ClanDestroyTime;
		pstZoneClan->stClan.ClanDestroyTime = 0;
		
		if (iOldTime)
		{
			clan_set_dirty(pstZoneClan, CACHE_FLAG_DIRTY);
		}
	}

	switch(pstZoneClan->stZoneClanInfo.iDirty)
	{
	case CACHE_FLAG_IMM:
		clan_update(pstEnv, pstZoneClan);
		clan_tick(pstEnv, pstZoneClan);
		break;

	case CACHE_FLAG_INEXIST:
		clan_delete_req(pstEnv, pstZoneClan);
		break;

	case CACHE_FLAG_DIRTY:
		if (pstZoneClan->stZoneClanInfo.iTimerCount % 100 == 0)
		{
			clan_update(pstEnv, pstZoneClan);
		}

		clan_tick(pstEnv, pstZoneClan);
		break;

	default:
		clan_tick(pstEnv, pstZoneClan);
		break;
	}
	
	return 0;
}


int z_active_clan(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, int iCount)
{
	static struct timeval stRoundTv;
	static struct timeval stRoundStartTv;
	static int iActivePos = 0, iRound = 0;
	int i;
	struct timeval stSub;
	long long  llMs=0;
	ZoneClan *pstZoneClan = NULL;
	ClanObj *pstClanObj = pstEnv->pstClanObj;

	if (INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
	{
		return 0;
	}
	
	if (pstClanObj->stClanObjInfo.iClanStat != CLAN_STAT_FINI)
	{
		return clan_init(pstEnv, pstClanObj);
	}
	
	if (0 == stRoundTv.tv_sec)
	{
		llMs = CLAN_ACTIVE_TIMER;
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
		
	if (llMs < CLAN_ACTIVE_TIMER)
	{
		return 0;
	}

	if (0 == iActivePos)
	{
		TV_CLONE(stRoundStartTv, pstAppCtx->stCurr);
	}

	for(i = 0; i < iCount; ++i) 
	{
		if (iActivePos >= pstClanObj->iClanNum)
		{
			break;
		}

		pstZoneClan = &pstClanObj->astZoneClan[iActivePos];
		clan_timer(pstEnv, pstZoneClan);

		++iActivePos;
	}
	
	if (iActivePos < pstClanObj->iClanNum)
	{
		return 0;
	}

	iActivePos = 0;
	++iRound;
	TV_CLONE(stRoundTv, pstAppCtx->stCurr);
	TV_DIFF(stSub, pstAppCtx->stCurr, stRoundStartTv);
	TV_TO_MS(llMs, stSub);
	if (llMs > CLAN_ACTIVE_TIMER)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "active clan loop %lld ms", llMs);
	}

	return 0;
}


ZoneClan* z_clanid_clan(ZONESVRENV *pstEnv, const CLANID *pstClanID)
{
	ZoneClan *pstWantClan = z_id_clan(pstEnv, pstClanID->Id);
	
	if (!pstWantClan)
	{
		int i;

		for(i = 0; i < pstEnv->pstClanObj->iClanNum; ++i)
		{
			ZoneClan *pstZoneClan = pstEnv->pstClanObj->astZoneClan + i;

			if (pstZoneClan->stClan.ClanProf.GID == pstClanID->GID)
			{
				pstWantClan = pstZoneClan;
				break;
			}
		}
	}

	if (pstWantClan != NULL)
	{
		if (pstWantClan->stClan.ClanProf.GID != pstClanID->GID ||
			pstWantClan->stZoneClanInfo.iDirty == CACHE_FLAG_INEXIST)
		{
			pstWantClan = NULL;
		}
	}

	return pstWantClan;
}


ZoneClan* z_id_clan(ZONESVRENV *pstEnv, unsigned int nID)
{
	ZoneClan *pstZoneClan = NULL;
	ClanHash stClanHash, *pstClanHash = NULL;

	stClanHash.uiClanId = nID;
	pstClanHash = sht_find(pstEnv->pstShtClan, &stClanHash, ClanHashCmp,
	                       ClanHashCode);
	if (NULL == pstClanHash)
	{
		return NULL;
	}

	if (0 > pstClanHash->iIdx &&
		pstClanHash->iIdx >= pstEnv->pstClanObj->iClanNum)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "hash error");
		return NULL;
	}

	pstZoneClan = &pstEnv->pstClanObj->astZoneClan[pstClanHash->iIdx];
	if (pstZoneClan->stClan.ClanProf.GID != pstClanHash->ullClanGID)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "hash error");
		return NULL;
	}

	return pstZoneClan;
}

ZoneClan* clan_find_by_wid(ZONESVRENV *pstEnv, tdr_ulonglong ullWID)
{
	int i;
	ZoneClan *pstZoneClan;
	
	for(i = 0; i < pstEnv->pstClanObj->iClanNum; ++i)
	{
		pstZoneClan = &pstEnv->pstClanObj->astZoneClan[i];
		if (pstZoneClan->stClan.ClanProf.GID == ullWID)
		{
			return pstZoneClan;
		}
	}

	return NULL;
}


void z_db_clan_fin(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv)
{
	ZoneClan *astClans = pstEnv->pstClanObj->astZoneClan;
	int i, iCount = 0;

	UNUSED(pstAppCtx);

	if (pstEnv->pstClanObj->stClanObjInfo.iClanStat != CLAN_STAT_FINI)
	{
		return;
	}

	// 立即处理所有公会数据的事务
	for(i = 0; i < pstEnv->pstClanObj->iClanNum; ++i)
	{
		// Clean有可能不是真的Clean，但是在现在，将确保公会数据能Clean
		clan_set_dirty(astClans + i, CACHE_FLAG_IMM);

		switch(astClans[i].stZoneClanInfo.iDirty)
		{
		case CACHE_FLAG_IMM:
		case CACHE_FLAG_DIRTY:
			clan_update_req(pstEnv, astClans + i);
			break;

		case CACHE_FLAG_INEXIST:
			clan_delete_req(pstEnv, astClans + i);
			break;

		default:
			break;
		}

		//一次少搞一些，避免bus 通道拥塞和db瓶颈
		if (iCount++ > CLAN_STOP_PATCH)
		{
			usleep(CLAN_STOP_PATCH * 1000); // 2ms存一个Clan
			iCount = 0;
		}
	}
}


int z_db_clan_is_allclean(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv)
{
	const ZoneClan * astClans = pstEnv->pstClanObj->astZoneClan;
	int i, iCount = 0;

	UNUSED(pstAppCtx);

	if (pstEnv->pstClanObj->stClanObjInfo.iClanStat != CLAN_STAT_FINI)
	{
		return 1;
	}

	for(i = 0; i < pstEnv->pstClanObj->iClanNum; ++i)
	{
		if (astClans[i].stZoneClanInfo.iDirty == CACHE_FLAG_CLEAN)
		{
			++iCount;
		}
	}
	return (iCount == pstEnv->pstClanObj->iClanNum);
}

CLANLEVELDEF* z_clan_level_def(ZONESVRENV *pstEnv, int iLevel)
{
	CLANLEVELDEF  stDummy;
	int iIndex, iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDummy.Level = iLevel;
	iIndex = bsearch_int(&stDummy, pstObj->sClanLevelDef, pstObj->iClanLevelDef,
		                 CLAN_LEVEL_DEF_DATA, &iEqu);
	return iEqu ? (CLANLEVELDEF *)pstObj->sClanLevelDef[iIndex] : NULL;
}

CLANCITYLEVELDEF* z_clan_city_level_def(ZONESVRENV *pstEnv, int iLevel)
{
	CLANCITYLEVELDEF  stDummy;
	int iIndex, iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDummy.Level = iLevel;
	iIndex = bsearch_int(&stDummy, pstObj->sClanCityLevelDef, 
		                 pstObj->iClanCityLevelDef, CLAN_CITY_LEVEL_DEF_DATA,
		                 &iEqu);
	return iEqu ? (CLANCITYLEVELDEF *)pstObj->sClanCityLevelDef[iIndex] : NULL;
}

CLANCITYBUILDINGLEVELDEF* z_clan_city_building_level_def(ZONESVRENV *pstEnv,
	                                                       int iLevel, int iID)
{
	CLANCITYBUILDINGLEVELDEF  stDummy;
	int iIndex, iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDummy.Level = iLevel;
	stDummy.Type  = iID;
	iIndex = bsearch_int(&stDummy, pstObj->sClanCityBuildingLevelDef, 
		                 pstObj->iClanCityBuildingLevelDef, 
		                 CLAN_CITY_BUILDING_LEVEL_DEF_DATA, &iEqu);
	return iEqu ?
	       (CLANCITYBUILDINGLEVELDEF *)pstObj->sClanCityBuildingLevelDef[iIndex] 
		   : NULL;
}

CLANMEM* clan_id_mem(ZoneClan *pstClan, tdr_ulonglong ullRoleID, int *piIdx)
{
	int iIdx, fEquals = 0;
	CLANMEM stKey;
	
	if (!pstClan || !ullRoleID)
	{
		return NULL;
	}

	if (piIdx && (*piIdx) >=0 && (*piIdx) < pstClan->stClan.ClanProf.MemNum)
	{
		if (ullRoleID ==
			pstClan->stClan.ClanCore.ClanMem[(*piIdx)].RoleMember.RoleID)
		{
			return &pstClan->stClan.ClanCore.ClanMem[(*piIdx)];
		}
	}

	stKey.RoleMember.RoleID = ullRoleID;
	iIdx = my_bsearch(&stKey, pstClan->stClan.ClanCore.ClanMem,
	                  pstClan->stClan.ClanProf.MemNum,
	                  sizeof(pstClan->stClan.ClanCore.ClanMem[0]), &fEquals,
	                  CmpClanMem);
	if (!fEquals)
	{
		return NULL;
	}
	if (piIdx)
	{
		*piIdx = iIdx;
	}
	return &pstClan->stClan.ClanCore.ClanMem[iIdx];
}


static CLANAPPLY* clan_id_apply(ZoneClan *pstClan, tdr_ulonglong ullRoleID,
                                int *piIdx)
{
	int i;
	
	if (!pstClan || !ullRoleID)
	{
		return NULL;
	}

	for(i = 0; i < pstClan->stClan.ClanInner.ClanApplyNum; ++i)
	{
		if (pstClan->stClan.ClanInner.ClanApplys[i].RoleMember.RoleID ==
			ullRoleID)
		{
			if (piIdx)
			{
				*piIdx = i;
			}
			return &pstClan->stClan.ClanInner.ClanApplys[i];
		}
	}

	return NULL;
}


static void clan_update_officers(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	int i;
	unsigned int nAssistNum = 0;
	CLANMEM *pstMem = pstZoneClan->stClan.ClanCore.ClanMem;
	CLANBRIEF *pstBrief = &pstZoneClan->stClan.ClanBrief;

	for(i = 0; i < pstZoneClan->stClan.ClanProf.MemNum; ++i)
	{
		switch(pstMem->Position)
		{
		case CLAN_POS_PRESIDENT:
			pstBrief->Leader = pstMem->RoleMember;
			break;

		case CLAN_POS_VICE_PRESIDENT:
			if (nAssistNum < CCH(pstBrief->Assists))
			{
				pstBrief->Assists[nAssistNum++] = pstMem->RoleMember;
			}
			break;
		}
		++pstMem;
	}
	
	pstZoneClan->stClan.ClanBrief.AssistNum = nAssistNum;
	clan_set_dirty_notify(pstZoneClan, CACHE_FLAG_DIRTY);

	player_clan_view_chg(pstEnv, pstZoneClan, CLAN_VIEW_CHG_MOD);
}

CLANMEM* clan_group_captain_get(ZoneClan *pstZoneClan, int iPostion)
{
	int i;
	CLANMEM *pstMem;

	for(i = 0; i < pstZoneClan->stClan.ClanProf.MemNum; i++)
	{
		pstMem = &pstZoneClan->stClan.ClanCore.ClanMem[i];
		if(iPostion == pstMem->Position)
		{
			return pstMem;
		}
	}
	
	return NULL;
}

static int IsRoleApplyTimeOutOrOffine(void* pvCtx, void* pvItem)
{
	ZONESVRENV *pstEnv = ((MemberApplyContext*)pvCtx)->pstEnv;
	Player *pstChecker = ((MemberApplyContext*)pvCtx)->pstPlayer;
	const CLANAPPLY *pstItem = pvItem;
	Player *pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
	                                  pstItem->RoleMember.RoleName);
	if (!pstPlayer) // 清除不在线角色
	{
		COND_DO_(pstChecker,
		         z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstChecker, SYS_TIP, ZONEERR_CLAN203,
		                    pstItem->RoleMember.RoleName));
		return 1;
	}

	// 清除申请超时的角色
	if (pstItem->ApplyTime + g_iClanApplyMaxSecs <
		pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		if (!player_get_clan(pstEnv, pstPlayer)) // 还是通知一下
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN204);
		}
		COND_DO_(pstChecker,
		         z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstChecker, SYS_TIP,
		                    ZONEERR_CLAN205,
		                    pstItem->RoleMember.RoleName));
		return 1;
	}

	// 清除已经有公会的角色
	if (!!player_get_clan(pstEnv, pstPlayer))
	{
		COND_DO_(pstChecker,
		         z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstChecker, SYS_TIP,
		                    ZONEERR_CLAN206,
		                    pstItem->RoleMember.RoleName));
		return 1;
	}
	
	return 0;
}


static void clan_update_apply(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                              Player *pstPlayer)
{
	size_t nNum = pstZoneClan->stClan.ClanInner.ClanApplyNum;
	MemberApplyContext stContext = {
		.pstEnv = pstEnv,
		.pstPlayer = pstPlayer,
	};
	
	nNum = array_remove_if( 
		pstZoneClan->stClan.ClanInner.ClanApplys, 
		sizeof(pstZoneClan->stClan.ClanInner.ClanApplys[0]), 
		nNum, 
		&stContext, 
		IsRoleApplyTimeOutOrOffine);

	if (pstZoneClan->stClan.ClanInner.ClanApplyNum != nNum)
	{
		pstZoneClan->stClan.ClanInner.ClanApplyNum = nNum;
		clan_set_dirty(pstZoneClan, CACHE_FLAG_DIRTY);
	}
}



Player *ClanEveryBody(ZONESVRENV *pstEnv, const ROLEMEMBER *pstMember)
{
	return z_name_player(pstEnv->pstAppCtx, pstEnv,
	                     (char *)pstMember->RoleName);
}


static Player* ClanBodyHasMemberSnapshot(ZONESVRENV *pstEnv,
                                         const ROLEMEMBER *pstMember)
{
	Player *pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
	                                  (char *)pstMember->RoleName);
	if (!pstPlayer)
	{
		return NULL;
	}
	return pstPlayer->stOnline.cMiscFlag & MISC_FLAG_CLAN_SNAP ?
	       pstPlayer : NULL;
}


static Player* ClanBodyCanCensorJoinClan(ZONESVRENV *pstEnv,
                                         const ROLEMEMBER *pstMember)
{
	Player *pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
	                                  (char *)pstMember->RoleName);
	if (!pstPlayer)
	{
		return NULL;
	}
	return player_clan_capability_check(pstEnv, pstPlayer,
	                                    CLAN_OP_CENSOR_JOIN_CLAN) < 0
	      ? NULL : pstPlayer;
}


static Player* ClanBodyCanDiplomatism(ZONESVRENV *pstEnv,
                                      const ROLEMEMBER *pstMember)
{
	Player *pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
	                                  (char *)pstMember->RoleName);
	if (!pstPlayer)
	{
		return NULL;
	}
	if (player_clan_capability_check(pstEnv, pstPlayer,
	                                 CLAN_OP_CENSOR_JOIN_UNION) < 0 ||
		player_clan_capability_check(pstEnv, pstPlayer,
	                                 CLAN_OP_CENSOR_JOIN_ENEMY) < 0)
	{
		return NULL;
	}
	return pstPlayer;
}


int clan_broadcast_if(
	ZONESVRENV *pstEnv,
	const ZoneClan *pstZoneClan,
	CSPKG *pstPkg,
	Player* (*pfnPred)(ZONESVRENV *, const ROLEMEMBER *))
{
	int i, iCount = 0;
	Player *pstPlayer = NULL;
	Player *apstPlayers[CLAN_MAX_MEM];

	i = pstZoneClan->stClan.ClanProf.MemNum;
	while(i--)
	{
		if (!pstZoneClan->stClan.ClanCore.ClanMem[i].Online)
		{
			continue;
		}

		pstPlayer = (*pfnPred)(
			pstEnv,
			&pstZoneClan->stClan.ClanCore.ClanMem[i].RoleMember);
		if (!pstPlayer)
		{
			continue;
		}
		apstPlayers[iCount++] = pstPlayer;
	}
	
	if (iCount > 0)
	{		
		return z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
		                                iCount, pstPkg);
	}
	return 0;
}

int clan_action_clan_donate_log(
	ZONESVRENV *pstEnv,
	const ZoneClan *pstZoneClan,
	CLANDONATELOGENTRY *pstLog)
{
	int i;
	Player *pstPlayer = NULL;

	i = pstZoneClan->stClan.ClanProf.MemNum;
	while(i--)
	{
		if (!pstZoneClan->stClan.ClanCore.ClanMem[i].Online)
		{
			continue;
		}

		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
	                     pstZoneClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName);
		if (!pstPlayer)
		{
			continue;
		}
		
		player_action_clan_donate_log(pstEnv, pstPlayer,pstLog);
	}

	return 0;
}

int clan_battle_apply_info_action(ZONESVRENV * pstEnv,  ZoneClan* pstClan,int iOpType)
{
	CSPKG stPkg;
	CSACTIONRES *pstAction = &stPkg.Body.ActionRes;	
	ClanBattleApplyData *pstApplyData ;


	pstApplyData = &pstClan->stZoneClanInfo.stBattleApplyData;

	
	memset(pstAction,0,sizeof(*pstAction));
	
	pstAction->ID = 0;
	pstAction->ActionID = ACTION_CLAN_BATTLE_APPLY_INFO;
	
	pstAction->Data.ClanBattleApplyInfo.BattleID = pstApplyData->iBattleID;
	pstAction->Data.ClanBattleApplyInfo.OpType = iOpType;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	clan_broadcast_if(pstEnv,pstClan,&stPkg,ClanEveryBody);
	
	return 0;
}



static int IsClanInexist(void *pvCtx, void *pvItem)
{
	MemberApplyContext *pstContext = (MemberApplyContext *)pvCtx;
	const CLANRELATION *pstItem = pvItem;

	if (!z_clanid_clan(pstContext->pstEnv, &pstItem->ClanId))
	{
		COND_DO_(pstContext->pstPlayer,
		         z_send_sys_str_msg(pstContext->pstEnv->pstAppCtx, pstContext->pstEnv, pstContext->pstPlayer, SYS_TIP,
		                    ZONEERR_CLAN207,
		                    pstItem->ClanId.Name, pstContext->pszHint));
		return 1;
	}
	return 0;
}

static int IsClanApplyTimeOut(void *pvCtx, void *pvItem)
{
	MemberApplyContext *pstContext = (MemberApplyContext *)pvCtx;
	CLANAPPLYRELATION *pstItem = pvItem;

	if (pstItem->ApplyTime + g_iClanApplyMaxSecs <
		pstContext->pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		COND_DO_(pstContext->pstPlayer,
		         z_send_sys_str_msg(pstContext->pstEnv->pstAppCtx, pstContext->pstEnv, pstContext->pstPlayer, SYS_TIP,
		                    ZONEERR_CLAN208,
		                    pstItem->ClanId.Name, pstContext->pszHint));
		return 1;
	}
	return 0;
}

static void clan_update_diplomatism(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                    Player *pstPlayer)
{
	size_t nNum;
	MemberApplyContext stContext = {
		.pstEnv = pstEnv,
		.pstPlayer = pstPlayer,
	};

	// 清理同盟申请列表。
	stContext.pszHint = "同盟申请";
	nNum = array_remove_if(
		pstZoneClan->stClan.ClanInner.ClanApplyFriends,
		sizeof(pstZoneClan->stClan.ClanInner.ClanApplyFriends[0]),
		pstZoneClan->stClan.ClanInner.ClanApplyFriendNum,
		&stContext,
		IsClanApplyTimeOut);
	if (pstZoneClan->stClan.ClanInner.ClanApplyFriendNum != nNum)
	{
		pstZoneClan->stClan.ClanInner.ClanApplyFriendNum = (unsigned char)nNum;
		clan_set_dirty(pstZoneClan, CACHE_FLAG_DIRTY);
	}

	// 清理敌对申请列表。
	stContext.pszHint = "敌对申请";
	nNum = array_remove_if(
		pstZoneClan->stClan.ClanInner.ClanApplyEnemys,
		sizeof(pstZoneClan->stClan.ClanInner.ClanApplyEnemys[0]),
		pstZoneClan->stClan.ClanInner.ClanApplyEnemyNum,
		&stContext,
		IsClanApplyTimeOut);
	if (pstZoneClan->stClan.ClanInner.ClanApplyEnemyNum != nNum)
	{
		pstZoneClan->stClan.ClanInner.ClanApplyEnemyNum = (unsigned char)nNum;
		clan_set_dirty(pstZoneClan, CACHE_FLAG_DIRTY);
	}

	// 清理同盟列表。
	stContext.pszHint = "同盟";
	nNum = array_remove_if(
		pstZoneClan->stClan.ClanInner.ClanFriends,
		sizeof(pstZoneClan->stClan.ClanInner.ClanFriends[0]),
		pstZoneClan->stClan.ClanInner.ClanFriendNum,
		&stContext,
		IsClanInexist);
	if (pstZoneClan->stClan.ClanInner.ClanFriendNum != nNum)
	{
		pstZoneClan->stClan.ClanInner.ClanFriendNum = (unsigned char)nNum;
		clan_set_dirty(pstZoneClan, CACHE_FLAG_DIRTY);
	}

	// 清理敌对列表
	stContext.pszHint = "敌对";
	nNum = array_remove_if(
		pstZoneClan->stClan.ClanInner.ClanEnemys,
		sizeof(pstZoneClan->stClan.ClanInner.ClanEnemys[0]),
		pstZoneClan->stClan.ClanInner.ClanEnemyNum,
		&stContext,
		IsClanInexist);
	if (pstZoneClan->stClan.ClanInner.ClanEnemyNum != nNum)
	{
		pstZoneClan->stClan.ClanInner.ClanEnemyNum = (unsigned char)nNum;
		clan_set_dirty(pstZoneClan, CACHE_FLAG_DIRTY);
	}
}


static void clan_update_online_action(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	if (pstZoneClan->stZoneClanInfo.stInviteLive.stOtherSideClan.GID != 0)
	{
		// 超时了
		if (pstZoneClan->stZoneClanInfo.stInviteLive.tActionTime +
			g_iClanOnlineActiveSecs < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			bzero(&pstZoneClan->stZoneClanInfo.stInviteLive,
			      sizeof(pstZoneClan->stZoneClanInfo.stInviteLive));
		}
	}

	if (pstZoneClan->stZoneClanInfo.stExitEnemyLive.stOtherSideClan.GID != 0)
	{
		if (pstZoneClan->stZoneClanInfo.stExitEnemyLive.tActionTime +
			g_iClanOnlineActiveSecs < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			bzero(&pstZoneClan->stZoneClanInfo.stExitEnemyLive,
			      sizeof(pstZoneClan->stZoneClanInfo.stExitEnemyLive));
		}
	}
}


static int clan_add_mem(ZONESVRENV *pstEnv, ZoneClan *pstClan,
                        Player *pstCustom, unsigned char bPosition,
                        CLANMEM *pstMem)
{
	CLANMEM stNewMem;
	ROLEDATA * pstRoleData = &pstCustom->stRoleData;
	CLANINFO * pstClanInfo = &pstRoleData->MiscInfo.ClanInfo;
	size_t nNum = pstClan->stClan.ClanProf.MemNum;

	if (clan_check_mem_limit(pstEnv, pstClan) < 0)
	{
		return -1;
	}

	bzero(&pstRoleData->MiscInfo.ClanSkillInfo,sizeof(pstRoleData->MiscInfo.ClanSkillInfo));
	player_action_player_clan_skill_info(pstEnv,pstCustom,&pstRoleData->MiscInfo.ClanSkillInfo);

	bzero(&stNewMem, sizeof(stNewMem));
	stNewMem.Career = pstRoleData->Career;
	stNewMem.LastLogin = pstEnv->pstAppCtx->stCurr.tv_sec;
	stNewMem.LastOnline = pstEnv->pstAppCtx->stCurr.tv_sec;
	stNewMem.Level = pstRoleData->Level;
	stNewMem.Online = 1;
	stNewMem.Position = bPosition;
	stNewMem.RoleMember.RoleID = pstRoleData->RoleID;
	STRNCPY(stNewMem.RoleMember.RoleName, pstRoleData->RoleName,
	        CCH(stNewMem.RoleMember.RoleName));
	stNewMem.JoinTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	++pstClan->stZoneClanInfo.iOnlineCount;

	if (!my_binsert(&stNewMem, pstClan->stClan.ClanCore.ClanMem, &nNum,
	                sizeof(pstClan->stClan.ClanCore.ClanMem[0]), 1, CmpClanMem))
	{
		// 有重复的了。
		return -1;
	}
	pstClan->stClan.ClanProf.MemNum = (unsigned short)nNum;
	pstClan->stClan.ClanCore.MemNum = (unsigned short)nNum;

	memset(pstClanInfo, 0, sizeof(*pstClanInfo));
	pstClanInfo->ClanId = *clan_id_warp(&pstClan->stClan);
	pstClanInfo->ExitPunishTime = 0;
	pstClanInfo->Contribute = 0;
	pstClanInfo->Idx = (pstClan - pstEnv->pstClanObj->astZoneClan);
	pstClanInfo->Position = bPosition;

	range_up_clanname(pstEnv, pstCustom->stRoleData.RoleID, "", pstClan->stClan.ClanProf.Name, 1);

	if (pstMem)
	{
		memcpy(pstMem, &stNewMem, sizeof(*pstMem));
	}

	//成长之路
	{
		ACHIEVEMENTCOND stCond;

		memset(&stCond,0,sizeof(stCond));
		
		stCond.Type = COND_CLAN_MEMBER;

		z_player_achievement_cond(pstEnv,pstCustom,&stCond);

		
		stCond.Type = COND_CLAN_LEVEL;
		stCond.Val1 = pstClan->stClan.ClanProf.Level;		

		z_player_achievement_cond(pstEnv,pstCustom,&stCond);			
		
	}

	player_action_clan_battle_apply_info(pstEnv,pstCustom,1);
	return 0;
}


static void * clan_clanid_diplomatism(ZONESVRENV *pstEnv, ZoneClan *pstClan,
                                      ClanInWhere nInWhere,
                                      const CLANID *pstClanID,
                                      unsigned int *punIdx)
{
	CLANRELATION *astIDs = NULL;
	CLANAPPLYRELATION *astApplyIDs = NULL;
	unsigned int i, nNum;

	UNUSED(pstEnv);

	if (!pstClan || !pstClanID)
	{
		return NULL;
	}

	switch(nInWhere)
	{
	case CLAN_IN_APPLY_ENEMY:
		nNum = pstClan->stClan.ClanInner.ClanApplyEnemyNum;
		astApplyIDs = pstClan->stClan.ClanInner.ClanApplyEnemys;
		break;

	case CLAN_IN_APPLY_FRIEND:
		nNum = pstClan->stClan.ClanInner.ClanApplyFriendNum;
		astApplyIDs = pstClan->stClan.ClanInner.ClanApplyFriends;
		break;

	case CLAN_IN_ENEMYS:
		nNum = pstClan->stClan.ClanInner.ClanEnemyNum;
		astIDs = pstClan->stClan.ClanInner.ClanEnemys;
		break;

	case CLAN_IN_FRIENDS:
		nNum = pstClan->stClan.ClanInner.ClanFriendNum;
		astIDs = pstClan->stClan.ClanInner.ClanFriends;
		break;

	default:
		return NULL;
	}

	if (astIDs != NULL)
	{
		for(i = 0; i < nNum; ++i)
		{
			if (astIDs[i].ClanId.GID == pstClanID->GID)
			{
				if (punIdx)
				{
					*punIdx = i;
				}
				return astIDs + i;
			}
		}
	}
	else if (astApplyIDs != NULL)
	{
		for(i = 0; i < nNum; ++i)
		{
			if (astApplyIDs[i].ClanId.GID == pstClanID->GID)
			{
				if (punIdx)
				{
					*punIdx = i;
				}
				return astApplyIDs + i;
			}
		}
	}
	
	return NULL;
}


static int clan_check_can_relations(ZONESVRENV *pstEnv, Player *pstPlayer,
                                    ZoneClan *pstMyClan,
                                    ZoneClan *pstOtherClan, int fNo2Side)
{
	if (!pstMyClan)
	{
		return 0;
	}

	if (pstMyClan == pstOtherClan)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
		               ZONEERR_CLAN7);
		return 0;	
	}
	
	if (clan_clanid_diplomatism(pstEnv, pstOtherClan, CLAN_IN_APPLY_FRIEND,
	                            clan_id_warp(&pstMyClan->stClan), NULL))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
		               ZONEERR_CLAN8);
		return 0;
	}

	if (clan_clanid_diplomatism(pstEnv, pstOtherClan, CLAN_IN_FRIENDS,
	                            clan_id_warp(&pstMyClan->stClan), NULL))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
		               ZONEERR_CLAN9);
		return 0;
	}
	
	if (clan_clanid_diplomatism(pstEnv, pstMyClan, CLAN_IN_ENEMYS,
	                            clan_id_warp(&pstOtherClan->stClan), NULL))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
		               ZONEERR_CLAN10);
		return 0;		
	}

	if (!fNo2Side &&
		clan_clanid_diplomatism(pstEnv, pstOtherClan, CLAN_IN_ENEMYS,
	                            clan_id_warp(&pstMyClan->stClan), NULL))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
		              ZONEERR_CLAN11);
		return 0;		
	}
	return 1;
}


static Player* clan_get_active_manager(ZONESVRENV *pstEnv, ZoneClan *pstClan)
{
	Player *pstManager = NULL;
	int i;

	if (!pstClan)
	{
		return NULL;
	}

	pstManager = z_name_player(pstEnv->pstAppCtx, pstEnv,
	                           pstClan->stClan.ClanBrief.Leader.RoleName);
	if (pstManager)
	{
		return pstManager;
	}

	for(i = 0; i < pstClan->stClan.ClanBrief.AssistNum; ++i)
	{
		pstManager = z_name_player(pstEnv->pstAppCtx, pstEnv,
		                         pstClan->stClan.ClanBrief.Assists[i].RoleName);
		if (pstManager)
		{
			return pstManager;
		}
	}

	return NULL;
}


static int clan_fill_view_row(CSCLANVIEWPAGE *pstPage, int iRow, int iVersion,
                              const ZoneClan *pstClan)
{

	if (pstClan->stZoneClanInfo.iListVersion == iVersion)
	{
		return 0;
	}
	
	pstPage->Cookie.RowVersion[iRow] = pstClan->stZoneClanInfo.iListVersion;
	pstPage->ClanView[iRow].ProfInfo = pstClan->stClan.ClanProf;
	pstPage->ClanView[iRow].BriefInfo = pstClan->stClan.ClanBrief;
	clan_set_city_prof(&pstClan->stClan.ClanCity,
	                   &pstPage->ClanView[iRow].CityInfo);
	return 1;
}


static void clan_fill_view_row_range(CSCLANVIEWPAGE *pstPage, int iBegin,
                                     int iEnd,
                                     const CSCLANVIEWPAGECOOKIE *pstCookie,
                                     const ClanObj *pstClanObj)
{
	int i, iIdx, iRowVersion, iRowCount = 0;
	const ZoneClan *pstClanBase = pstClanObj->astZoneClan;
	
	pstPage->Cookie.PageNo = pstCookie->PageNo;
	pstPage->Cookie.ListVersion = pstClanObj->stClanObjInfo.iListVersion;
	for(i = iBegin; i < iEnd; ++i)
	{
		iIdx = i % CLAN_MAX_VIEW_PAGE_ROW;
		if (iIdx >= pstCookie->RowNum)
		{
			iRowVersion = -1;
		}
		else
		{
			iRowVersion = pstCookie->RowVersion[iIdx];
		}
		
		if (clan_fill_view_row(pstPage, iRowCount, iRowVersion,
		                       pstClanBase + i))
		{
			++iRowCount;
		}
	}
	pstPage->Cookie.RowNum = (unsigned char)iRowCount;
}


static int clan_action(ZONESVRENV *pstEnv, const ZoneClan *pstZoneClan,
                       const CLANMEM *pstClanMem)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	Player *pstPlayer = NULL;

	if (!pstClanMem || !pstClanMem->Online)
	{
		return -1;
	}

	pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
	                          pstClanMem->RoleMember.RoleName);
	if (!pstPlayer)
	{
		return -1;
	}

	
	pstPlayer->stRoleData.MiscInfo.ClanInfo.Position = pstClanMem->Position;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_CLAN_CHG;
	pstActionRes->Data.ClanChg.Position = pstClanMem->Position;
	pstActionRes->Data.ClanChg.Group= pstClanMem->GroupID;
	pstActionRes->Data.ClanChg.GID = pstZoneClan->stClan.ClanProf.GID;
	STRNCPY(pstActionRes->Data.ClanChg.Name, pstZoneClan->stClan.ClanProf.Name,
	        CCH(pstActionRes->Data.ClanChg.Name));

	return player_action_res_ex(pstEnv, pstPlayer, &stPkg);
}





int clan_init_all_city_pworld(ZONESVRENV* pstEnv)
{
	ZoneClan *astZoneClan = pstEnv->pstClanObj->astZoneClan;
	int i = pstEnv->pstClanObj->iClanNum;

	while(i--)
	{
		if (astZoneClan[i].stClan.ClanCity.Level)
		{
			if (z_new_clan_city(pstEnv, astZoneClan + i) < 0)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0,	
				   "create clan city pworld fail, clan_name = %s, clan_id = %d",
					       astZoneClan[i].stClan.ClanProf.Name,
					       astZoneClan[i].stClan.ClanProf.Id);
			}
		}
	}
	return 0;
}


static int clan_check_permission(ZONESVRENV* pstEnv, Player *pstPlayer,
                                 int iOperation)
{
	int iClanPosition = player_clan_position(pstEnv, pstPlayer);

	if (!(kClanPermission[iOperation] & (1U << iClanPosition)))
	{
		// 玩家无有权限，不能操作。
		if (iClanPosition >= CLAN_POS_NORMAL)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, 
				           ZONEERR_CLAN26);
		}
		return -1;
	}
	return 0;
}

static int clan_check_safelock(ZONESVRENV* pstEnv, Player *pstPlayer,
                                 int iOperation)
{
	if (kClanSafeLock[iOperation])
	{
		return player_check_safelock(pstEnv,pstPlayer);
	}
	return 0;
}


int clan_chat_transmit(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                       const CSCHATRES *pstChatRes)
{
	SSPKG stPkg;
	SSCHAT *pstChat = &stPkg.Body.ChatInfo;

	pstChat->ChatType = SS_CHAT_CLAN;
	pstChat->ToData.ClanInfo = *clan_id_warp(&pstZoneClan->stClan);
	STRNCPY(pstChat->From, pstChatRes->From, CCH(pstChat->From));
	STRNCPY(pstChat->Content, pstChatRes->Content, CCH(pstChat->Content));
	pstChat->LinkLen = pstChatRes->LinkLen;
	memcpy(pstChat->LinkContent, pstChatRes->LinkContent, pstChat->LinkLen);

	Z_SSHEAD_INIT(&stPkg.Head, SS_CHAT);
	z_ssmsg_send(pstEnv, &stPkg, pstEnv->iMudSvrID);
	return 0;
}



int clan_resource_inc(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan,
                      Player *pstPlayer, CLAN_RESOURCE iWhich,
                      unsigned int nValue)
{
	DAOBAINFO *pstDaoba = &pstPlayer->stRoleData.MiscInfo.DaoBaInfo;
	if (!nValue)
	{
		return -1;
	}

	switch(iWhich)
	{
	case CLAN_MONEY:
		if (clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_MONEY, 
			                     nValue, 0) < 0)
		{
			if(pstPlayer)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
				           ZONEERR_CLAN209);
			}
			return -1;
		}
		if (pstPlayer)
		{
			player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_MIDDLE,
			                          ZONEERR_CLANBROAD216,
			                          pstPlayer->stRoleData.RoleName, nValue);
		}
		else
		{
			player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_MIDDLE,
			                          ZONEERR_CLANBROAD217, nValue);
		}
		break;

	case CLAN_STABLE_VALUE:
		if (clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_STABLE_VALUE, 
			                     nValue, 0) < 0)
		{
			if (pstPlayer)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
				           ZONEERR_CLAN210);
			}
			return -1;
		}
		if (pstPlayer)
		{
			player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_MIDDLE,
			                          ZONEERR_CLANBROAD218,
			                          pstPlayer->stRoleData.RoleName, nValue);
		}
		else
		{
			player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_MIDDLE,
			                          ZONEERR_CLANBROAD219, nValue);
		}
		break;

	case CLAN_TECH_POINT:

		if (nValue > 0)
		{
			pstZoneClan->stClan.ClanBoon.Cond.Tech += nValue;
			// 公会广播todo:
			clan_boon_condition_chg(pstEnv, pstZoneClan);
		}
		
		if (clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_TECH_POINT, 
			                     nValue, 0) < 0)
		{
			if (pstPlayer)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,
				           ZONEERR_CLAN211);
			}
			return -1;
		}
		
		if (pstPlayer)
		{
			player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_MIDDLE,
			                          ZONEERR_CLANBROAD220,
			                          pstPlayer->stRoleData.RoleName, nValue);
		}
		else
		{
			player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_MIDDLE,
			                          ZONEERR_CLANBROAD221, nValue);
		}

		break;

	case CLAN_OWNED_CONTRIBUT:
		if (!pstPlayer)
		{
			return -1;
		}

		if (pstDaoba->End>0 && pstDaoba->End > (unsigned int)pstEnv->pstAppCtx->stCurr.tv_sec )
		{
			nValue = nValue*95/100;
		}

		if (pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute >= MAX_CLAN_OWNED_CONTRIBUT)
		{
			return -1;
		}
		else if ((long long)pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute + nValue >= MAX_CLAN_OWNED_CONTRIBUT)
		{
			nValue = MAX_CLAN_OWNED_CONTRIBUT - pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute;
		}
		
		{
			CLANMEM *pstMem = clan_id_mem(pstZoneClan,
			                              pstPlayer->stRoleData.RoleID, NULL);
			if (!pstMem)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN212,
				           pstPlayer->stRoleData.RoleName,
				           pstZoneClan->stClan.ClanProf.Name);
				return -1;
			}
			pstMem->Contribute =
				pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute + nValue;

			if(g_iOptype != OP_OPTYPE_CONTRI_ITEM)
			{
				pstMem->TotalContribute += nValue;
				//pstMem->TotalContribute = max(pstMem->TotalContribute,pstMem->Contribute);
			}
			
			player_clan_mem_chg_broadcast(pstEnv, pstZoneClan, pstMem,
			                              CLAN_MEMBER_CHG_MOD);

			range_single_set(pstEnv, pstPlayer, RANGE_MAIN_SINGLE_SUB_CLANCT,
		                 pstMem->TotalContribute);
		}
		pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute += nValue;
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN213,
		           nValue);

		z_contri_create_oplog(pstEnv, pstZoneClan, g_iOptype, g_iOpsubtype, nValue);

		if (nValue > 0)
		{
			pstZoneClan->stClan.ClanBoon.Cond.Contri += nValue;
			// 公会广播todo:
			clan_boon_condition_chg(pstEnv, pstZoneClan);
		}
		
		if ((unsigned long long)pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute >= (unsigned)MAX_CLAN_OWNED_CONTRIBUT)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
			           ZONEERR_CLAN214);
			return 0;
		}
		break;

	default:
		return -1;
		break;
	}

	return 0;
}


static int clan_destroy_warning(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                Player *pstPlayer)
{
	const int kWarningTime = g_iClanDestroyMemMinSecs / 2;

	if ((pstZoneClan->stClan.ClanDestroyTime != 0 &&
		pstZoneClan->stClan.ClanDestroyTime - kWarningTime <
		pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		int iLeft = pstZoneClan->stClan.ClanDestroyTime - pstEnv->pstAppCtx->stCurr.tv_sec;
		if (0 > iLeft)
		{
			iLeft = 0;
		}
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN215,
		           (iLeft / (24 * 3600)) + 1);
	}
	return 0;
}


static void clan_fill_diplomatism_snapshot(ZONESVRENV* pstEnv,
                                           const ZoneClan *pstZoneClan, 
                                           CSCLANDIPLOMATISMSNAPSHOT *pstRes)
{
	int i = 0;
	ZoneClan *pstOther = NULL;

	memset(pstRes,0,sizeof(*pstRes));

	i = pstZoneClan->stClan.ClanInner.ClanFriendNum;
	pstRes->FriendNum = i;
	while(i--)
	{
		pstRes->Friends[i].ClanID =
			pstZoneClan->stClan.ClanInner.ClanFriends[i].ClanId;
		pstRes->Friends[i].Time =
			pstZoneClan->stClan.ClanInner.ClanFriends[i].Time;
		pstOther = z_clanid_clan(pstEnv, &pstRes->Friends[i].ClanID);
		if (pstOther)
		{
			STRNCPY(pstRes->Friends[i].Leader,
			        pstOther->stClan.ClanBrief.Leader.RoleName,
			        CCH(pstRes->Friends[i].Leader));
		}
	}

	i = pstZoneClan->stClan.ClanInner.ClanEnemyNum;
	pstRes->EnemyNum = i;
	while(i--)
	{
		pstRes->Enemys[i].ClanID =
			pstZoneClan->stClan.ClanInner.ClanEnemys[i].ClanId;
		pstRes->Enemys[i].Time =
			pstZoneClan->stClan.ClanInner.ClanEnemys[i].Time;
		pstOther = z_clanid_clan(pstEnv, &pstRes->Enemys[i].ClanID);
		if (pstOther)
		{
			STRNCPY(pstRes->Enemys[i].Leader,
			        pstOther->stClan.ClanBrief.Leader.RoleName,
			        CCH(pstRes->Enemys[i].Leader));
		}
	}
}


/*
static int clan_proxy_request(ZONESVRENV* pstEnv, Player *pstPlayer,
                              CSPKG *pstCsPkg)
{
	CSCLANCLT *pstClanClt = &pstCsPkg->Body.ClanClt;
	CSCLANCLTDATA *pstCltData = &pstClanClt->ClanCltData;
	Player *pstOther = NULL;

	switch(pstClanClt->Operation)
	{
	case CLAN_OP_INVITE_JOIN_CLAN: 	// 添加一个成员 

		return -1;
		pstOther = z_name_player(pstEnv->pstAppCtx, pstEnv,
		                         pstCltData->ClanInviteName);
		if (pstOther  && !is_together_world(pstEnv, pstPlayer, pstOther))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN216);
			return -1;
		}

		battle_rolename_remove_prefix(pstCltData->ClanInviteName);
		break;

	case CLAN_OP_DEL_MEMBER: // 删除一个成员 
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_BATTLE_SVR_OP);
		return -1;
		//battle_rolename_remove_prefix(pstCltData->ClanDelMemberGID.RoleName);
		break;
		
	case CLAN_OP_DEMISE_JOB: // 让位 
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_BATTLE_SVR_OP);
		return -1;
		//battle_rolename_remove_prefix(pstCltData->ClanDemiseJobRoleID.RoleName);
		break;

	case CLAN_OP_CREATE: // 创建一个公会 
	case CLAN_OP_EXIT_CLAN: // 退出公会 
	case CLAN_OP_DISBAND: // 解散公会 
	case CLAN_OP_EDIT_BULLETIN: // 编辑公告 
	case CLAN_OP_EDIT_MANIFESTO: // 编辑宣言 
	case CLAN_OP_APPLY_JOIN_CLAN: // 申请入会 
	case CLAN_OP_CENSOR_JOIN_CLAN: // 入会审查 
	case CLAN_OP_SET_MEMBER_NICKNAME: // 设置昵称 
	case CLAN_OP_APPLY_JOIN_LIST: // 查看入会请求列表 
	case CLAN_OP_INVITE_JOIN_UNION: // 邀请加入同盟 
	case CLAN_OP_APPLY_JOIN_UNION: // 申请加入同盟 
	case CLAN_OP_EXIT_UNION: // 退出同盟 
	case CLAN_OP_APPLY_JOIN_ENEMY: // 添加敌对公会 
	case CLAN_OP_EXIT_ENEMY: // 删除敌对公会 
	case CLAN_OP_DIPLOMATISM_STATUS: // 查看公会的外交状态 
	case CLAN_OP_ANSWER_JOIN_CLAN: // 回复别人的入会邀请 
	case CLAN_OP_ANSWER_JOIN_UNION: // 回复别人的入盟邀请 
	case CLAN_OP_FIND_REQ: // 公会查找 
	case CLAN_OP_CENSOR_JOIN_UNION: // 审查申请入盟的公会 
	case CLAN_OP_CENSOR_JOIN_ENEMY: // 审查申请敌对的公会 
	case CLAN_OP_MEMBER_SNAPSHOT: // 要求查看所有成员信息的快照 
	case CLAN_OP_INFO: // 要求查看公会的一般信息 
	case CLAN_OP_MANIFESTO: // 要求查看公会宣言 
	case CLAN_OP_UPGRADE: // 升级公会 
	case CLAN_OP_ENABLE_CITY: // 申请公会城市 
	case CLAN_OP_UPGRADE_CITY: // 升级公会城市 
	case CLAN_OP_UPGRADE_CITY_BUILDING: // 升级公会城市建筑 
	//case CLAN_OP_VIEW_PAGE: // 查看服务器上的一页公会 
	case CLAN_OP_CITY_INFO: // 查看本公会的公会城市信息 
	//case CLAN_OP_PAY:
	case CLAN_OP_VIEW_PAGE_BY_ID: // 根据ID获取一页公会的基本信息 
	case CLAN_OP_SET_JOB_AND_TITLE:	// 设置公会成员的职位与封号 
	case CLAN_OP_RESEARCH: // 研发科技 
	case CLAN_OP_CANCEL_RESEARCH: // 取消研发科技 
	case CLAN_OP_MAKE: // 制造物品/机械 
	case CLAN_OP_CANCEL_MAKING: // 取消制造物品/机械 
	case CLAN_OP_GRAB_MACHINE: // 从仓库中提取机械 
	case CLAN_OP_CALLBACK_MACHINE: // 召回机械放回仓库中 
	case CLAN_OP_MACHINE_SUPPLY: // 对机械进行补给 
	case CLAN_OP_STORAGE_GRID_LOCK_SWITCH: // 公会仓库格子锁切换 
	case CLAN_OP_STORAGE_GRID_DESTROY: // 公会仓库摧毁物品 
	case CLAN_OP_ANSWER_EXIT_ENEMY: // 回复别人的解除敌对请求 
	case CLAN_OP_RESEARCH_SPEEDUP: // 研发加速 
	case CLAN_OP_MAKING_SPEEDUP: // 制造加速 
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP, ZONEERR_BATTLE_SVR_OP);
		return -1;
		break;

	default:
		return -1;
		break;
	}

	return battle_proxy_request(pstEnv, pstPlayer, pstCsPkg);
}

*/

int clan_develop_val(ZoneClan *pstClan)
{
	return pstClan->stClan.ClanProf.ActiveValue;
}


static ZoneClan* clan_find_by_name(ZONESVRENV *pstEnv, const char *pszClanName)
{
	ZoneClan *astZoneClan = pstEnv->pstClanObj->astZoneClan;
	int i = pstEnv->pstClanObj->iClanNum;
	if (pstEnv->cShutdown > 1)
	{
		return NULL;
	}

	if (pstEnv->pstClanObj->stClanObjInfo.iClanStat != CLAN_STAT_FINI)
	{
		return NULL;
	}

	while(i--)
	{
		if (strcmp(astZoneClan[i].stClan.ClanProf.Name, pszClanName) == 0)
		{
			return (astZoneClan + i);
		}
	}
	return NULL;
}


static int clan_del_mem_by_name(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                const char *pszRoleName)
{
	int i = 0;
	Player *pstMemPlayer = NULL;
	CLANMEM *pstKey = NULL, stKey;

	for(i = 0; i < (int)pstZoneClan->stClan.ClanCore.MemNum; ++i)
	{
		if (!strcmp(pszRoleName,
		           pstZoneClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName))
		{
			pstKey = pstZoneClan->stClan.ClanCore.ClanMem + i;
			break;
		}
		pstKey = NULL;
	}
	if (!pstKey)
	{
		return -1;
	}
	if (pstKey->Position == CLAN_POS_PRESIDENT)
	{
		return -1;
	}

	bzero(&stKey, sizeof(stKey));
	stKey = *pstKey;
	// 要保持成员有序
	if (i != (int)pstZoneClan->stClan.ClanCore.MemNum - 1)
	{
		memmove(pstKey, pstKey + 1, 
		        ((int)pstZoneClan->stClan.ClanCore.MemNum - i - 1) *
		        sizeof(*pstKey));
	}
	pstKey = NULL;
	--pstZoneClan->stClan.ClanProf.MemNum;
	--pstZoneClan->stClan.ClanCore.MemNum;
	
	clan_update_officers(pstEnv, pstZoneClan);
	++pstZoneClan->stZoneClanInfo.iListVersion;

	pstMemPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pszRoleName);
	if (pstMemPlayer)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstMemPlayer, SYS_TIP, 
		               ZONEERR_CLAN12);
		bzero(&pstMemPlayer->stRoleData.MiscInfo.ClanInfo,
		      sizeof(pstMemPlayer->stRoleData.MiscInfo.ClanInfo));
		
		pstMemPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_CLAN_SNAP;

		// 退出公会城市先
		player_clan_leave_city(pstEnv, pstMemPlayer, pstZoneClan);

		stKey.LastOnline = pstEnv->pstAppCtx->stCurr.tv_sec;
		//clan_account_active_value(pstEnv, pstZoneClan, &stKey);
		clan_action_exit(pstEnv, pstMemPlayer);
		player_clan_disable(pstEnv, pstMemPlayer);
		relation_dbbuddy_chg(pstEnv, pstMemPlayer, 1, 1);

		bzero(&pstMemPlayer->stRoleData.MiscInfo.ClanInfo,
		      sizeof(pstMemPlayer->stRoleData.MiscInfo.ClanInfo));


	}

	player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN, ZONEERR_CLANBROAD222,
	                          				pszRoleName);
	player_clan_mem_chg_broadcast(pstEnv, pstZoneClan, &stKey,
	                              CLAN_MEMBER_CHG_DEL);	

	z_role_clan_oplog(pstEnv, (char*)pszRoleName,
                  clan_id_warp(&pstZoneClan->stClan), OP_CLAN_EXIT,NULL);
	
	return 0;
}

static int clan_del_mem_by_id(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                              tdr_ulonglong ullRoleID,int iTip)
{
	int iPos = -1;
	Player *pstMemPlayer = NULL;
	CLANMEM *pstKey = NULL, stKey;

	pstKey = clan_id_mem(pstZoneClan, ullRoleID, &iPos);
	if (!pstKey)
	{
		return -1;
	}
	stKey = *pstKey;
	if (iPos < pstZoneClan->stClan.ClanProf.MemNum - 1)
	{
		memmove(pstKey, pstKey + 1,
		        (pstZoneClan->stClan.ClanProf.MemNum - 1 - iPos) *
		        sizeof(*pstKey));
	}

	--pstZoneClan->stClan.ClanProf.MemNum;
	--pstZoneClan->stClan.ClanCore.MemNum;

	clan_update_officers(pstEnv, pstZoneClan);
	++pstZoneClan->stZoneClanInfo.iListVersion;

	pstMemPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
	                             stKey.RoleMember.RoleName);
	if (pstMemPlayer)
	{
		/*
		bzero(&pstMemPlayer->stRoleData.MiscInfo.ClanInfo,
		      sizeof(pstMemPlayer->stRoleData.MiscInfo.ClanInfo));
		
		pstMemPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_CLAN_SNAP;

		// 退出公会城市先
		player_clan_leave_city(pstEnv, pstMemPlayer, pstZoneClan);

		stKey.LastOnline = pstEnv->pstAppCtx->stCurr.tv_sec;
		clan_action_exit(pstEnv, pstMemPlayer);
		player_clan_disable(pstEnv, pstMemPlayer);
		relation_dbbuddy_chg(pstEnv, pstMemPlayer, 1, 1);

		bzero(&pstMemPlayer->stRoleData.MiscInfo.ClanInfo,
		      sizeof(pstMemPlayer->stRoleData.MiscInfo.ClanInfo));*/
		clan_mem_on_exit(pstEnv, pstZoneClan, pstMemPlayer, 0);      

	}

	pstZoneClan->stZoneClanInfo.iDirty = CACHE_FLAG_IMM;

	if (iTip)
	{
		player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN, ZONEERR_CLANBROAD223, stKey.RoleMember.RoleName);
	}
	
	player_clan_mem_chg_broadcast(pstEnv, pstZoneClan, &stKey,
	                              CLAN_MEMBER_CHG_DEL);

	z_role_clan_oplog(pstEnv, stKey.RoleMember.RoleName,
              clan_id_warp(&pstZoneClan->stClan), OP_CLAN_EXIT,NULL);
	return 0;
}

static int clan_del_diplomatism(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                const char *pszTargetName)
{
	int i = 0;

	// Lazy updating : 
	clan_update_diplomatism(pstEnv, pstZoneClan, NULL);

	i = pstZoneClan->stClan.ClanInner.ClanFriendNum;
	while(i--)
	{
		CLANRELATION *pstIter = pstZoneClan->stClan.ClanInner.ClanFriends + i;
		if (strcmp(pstIter->ClanId.Name, pszTargetName) == 0)
		{
			if (i != pstZoneClan->stClan.ClanInner.ClanFriendNum - 1)
			{
				memcpy(pstIter,
				       pstZoneClan->stClan.ClanInner.ClanFriends +
				       pstZoneClan->stClan.ClanInner.ClanFriendNum - 1,
				       sizeof(*pstIter));
			}
			--pstZoneClan->stClan.ClanInner.ClanFriendNum;
			clan_set_dirty(pstZoneClan, CACHE_FLAG_IMM);

			player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
			                         ZONEERR_CLANBROAD224, pszTargetName);
		}
	}

	i = pstZoneClan->stClan.ClanInner.ClanEnemyNum;
	while(i--)
	{
		CLANRELATION *pstIter = pstZoneClan->stClan.ClanInner.ClanEnemys + i;
		if (strcmp(pstIter->ClanId.Name, pszTargetName) == 0)
		{
			if (i != pstZoneClan->stClan.ClanInner.ClanEnemyNum - 1)
			{
				memcpy(pstIter,
				       pstZoneClan->stClan.ClanInner.ClanEnemys +
				       pstZoneClan->stClan.ClanInner.ClanEnemyNum - 1,
				       sizeof(*pstIter));
			}
			--pstZoneClan->stClan.ClanInner.ClanEnemyNum;
			clan_set_dirty(pstZoneClan, CACHE_FLAG_IMM);

			player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
			                         ZONEERR_CLANBROAD225, pszTargetName);
		}
	}

	return 0;
}


int clan_clear_diplomatism(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	// Lazy updating : 
	clan_update_diplomatism(pstEnv, pstZoneClan, NULL);

	// 清除公会的盟友和敌人
	pstZoneClan->stClan.ClanInner.ClanEnemyNum = 0;
	bzero(pstZoneClan->stClan.ClanInner.ClanApplyFriends,
	      sizeof(pstZoneClan->stClan.ClanInner.ClanApplyFriends));
	pstZoneClan->stClan.ClanInner.ClanFriendNum = 0;
	bzero(pstZoneClan->stClan.ClanInner.ClanFriends,
	      sizeof(pstZoneClan->stClan.ClanInner.ClanFriends));
	clan_set_dirty(pstZoneClan, CACHE_FLAG_IMM); // 立即更新到数据库

	// 通知在线的公会成员
	player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD226);
	return 0;
}


CLANMEM* clan_name_mem(ZoneClan *pstZoneClan, const char *pszRoleName,
                              int *piIdx)
{
	int i = 0;
	for(i = 0; i < (int)pstZoneClan->stClan.ClanCore.MemNum; ++i)
	{
		CLANMEM *pstMem = pstZoneClan->stClan.ClanCore.ClanMem + i;
		if (strcmp(pstMem->RoleMember.RoleName, pszRoleName) == 0)
		{
			if (piIdx)
			{
				*piIdx = i;
			}
			return pstMem;
		}
	}
	return NULL;
}


static int clan_check_old_data(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
/*
	CLANADVANCE *pstAdvance = &pstZoneClan->stClan.ClanAdvance;
	const CLANLEVELDEF *pstDef = z_clan_level_def(pstEnv,
	                                        pstZoneClan->stClan.ClanProf.Level);
	if (!pstDef)
	{
		return -1;
	}

	if (!pstAdvance->Produce.MaxArmMakingNum ||
		!pstAdvance->Produce.MaxCollectionMakingNum ||
		!pstAdvance->Storage.MaxMachineNum ||
		!pstAdvance->Storage.MaxResourceNum)
	{
		pstAdvance->Produce.MaxArmMakingNum = 1;
		pstAdvance->Produce.MaxCollectionMakingNum = 1;
		pstAdvance->Storage.MaxMachineNum = pstDef->MaxMachineGrid;
		pstAdvance->Storage.MaxResourceNum = pstDef->MaxResourceGrid;
	}
*/

	clan_skill_reset_return_money(pstEnv,pstZoneClan);
	clan_mem_num_top_jian_yan(pstEnv,pstZoneClan);
	clan_skill_business_exp_jian_yan(pstEnv,pstZoneClan);


	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstZoneClan);	//add by paraunused.pl
}


static int clan_exchange_leader(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                const char *pszNewLeaderRoleName)
{
	CLANMEM *pstTarget = clan_name_mem(pstZoneClan, pszNewLeaderRoleName, NULL);
	CLANMEM *pstMem = NULL;

	if (!pstTarget|| pstTarget->Position == CLAN_POS_PRESIDENT)
	{
		return -1;
	}
	
	pstMem = clan_id_mem(pstZoneClan,
	                     pstZoneClan->stClan.ClanBrief.Leader.RoleID, NULL);
	if (!pstMem)
	{
		return -1;
	}

	pstTarget->Position = CLAN_POS_PRESIDENT;
	STRNCPY(pstTarget->Title, pstMem->Title, CCH(pstTarget->Title));
	
	pstMem->Position = CLAN_POS_NORMAL;
	pstMem->Title[0] = 0;
	clan_update_officers(pstEnv, pstZoneClan);
	++pstZoneClan->stZoneClanInfo.iListVersion;

	// 更新显示
	{
		Player *pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
		                                  pstMem->RoleMember.RoleName);
		if (pstPlayer)
		{
			clan_action(pstEnv, pstZoneClan, pstMem);
		}
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
		                          pszNewLeaderRoleName);
		if (pstPlayer)
		{
			clan_action(pstEnv, pstZoneClan, pstTarget);
		}

		player_clan_mem_chg_broadcast(pstEnv, pstZoneClan, pstMem,
	                                      CLAN_MEMBER_CHG_MOD);
		player_clan_mem_chg_broadcast(pstEnv, pstZoneClan, pstTarget,
	                                      CLAN_MEMBER_CHG_MOD);
	}

	player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD227,
	                          pstMem->RoleMember.RoleName,
	                          pszNewLeaderRoleName);
	return 0;
}


static int clan_property_do_notify(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                   int iValue, int iWhich)
{
	CSPKG stPkg;
	stPkg.Body.ClanSvr.Operation = CLAN_SVR_PROPERTY_NOTIFY;
	stPkg.Body.ClanSvr.ClanSvrData.ClanPropertyNotify.Which = iWhich;
	stPkg.Body.ClanSvr.ClanSvrData.ClanPropertyNotify.Value = iValue;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	player_clan_broadcast(pstEnv, pstZoneClan, &stPkg, 0);
	return 0;
}


int clan_property_modify(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iWhich,
                         int iValue, int fDec)
{
	int iNewValue = 0;
	void *pvAddr = NULL;
	const ClanPropertyEntry *pstEntry = NULL;

	if (iWhich < 0 || iWhich >= (int)CCH(astClanProperties) || iValue < 0)
	{
		return -1;
	}

	pstEntry = astClanProperties + iWhich;

	//属性废弃失效了
	if (pstEntry->iInvalid) 
	{
		return -1;
	}
	
	pvAddr = ((char *)pstZoneClan + pstEntry->nOffset);

	switch(pstEntry->iBytes)
	{
	case 1:
		iNewValue = *((unsigned char *)pvAddr);
		break;

	case 2:
		iNewValue = *((unsigned short *)pvAddr);
		break;

	case 4:
		iNewValue = *((unsigned int *)pvAddr);
		break;

	default:
		return -1;
		break;
	}

	if (iValue >= 0)
	{
		/*
		if (fDec)
		{
			if ((long long)iNewValue - iValue < pstEntry->iMin)
			{
				return -1;
			}
		}
		else
		{
			if ((long long)iNewValue + iValue > pstEntry->iMax)
			{
				return -1;
			}
		}*/

		if (fDec > 0)
		{
			if (iNewValue <= pstEntry->iMin && iValue > 0)
			{
				return -1;
			}
			else if ((long long)iNewValue - iValue < pstEntry->iMin)
			{
				iValue = iNewValue - pstEntry->iMin;
			}

			if (CLAN_PROPERTY_MONEY == iWhich && g_iOptype > 0)
			{
				z_clanprop_consume_oplog(pstEnv, pstZoneClan, iWhich, 
					g_iOptype, g_iOpsubtype, iValue);
			}
		}
		else if (fDec == 0)
		{
			if (iNewValue >= pstEntry->iMax && iValue > 0)
			{
				return -1;
			}
			else if ((long long)iNewValue + iValue >= pstEntry->iMax)
			{
				iValue = pstEntry->iMax - iNewValue;
			}

			if (CLAN_PROPERTY_MONEY == iWhich && g_iOptype > 0)
			{
				z_clanprop_create_oplog(pstEnv, pstZoneClan, iWhich, 
					g_iOptype, g_iOpsubtype, iValue);
			}
		}
		else if (fDec == -1)
		{
			iNewValue = iValue;

			if (iNewValue >= pstEntry->iMax)
			{
				iValue = pstEntry->iMax;
			}

			if (iNewValue <= pstEntry->iMin)
			{
				iValue = pstEntry->iMin;
			}
		}

		switch(pstEntry->iBytes)
		{
		case 1:
			if (-1 == fDec)
			{
				*((unsigned char *)pvAddr) = iValue;
			}
			else
			{
				*((unsigned char *)pvAddr) += (fDec ? -iValue : iValue);
			}
			iNewValue = *((unsigned char *)pvAddr);
			break;

		case 2:
			if (-1 == fDec)
			{
				*((unsigned short *)pvAddr) = iValue;
			}
			else
			{
				*((unsigned short *)pvAddr) += (fDec ? -iValue : iValue);
			}
			iNewValue = *((unsigned short *)pvAddr);
			break;

		case 4:
			if (-1 == fDec)
			{
				*((unsigned int *)pvAddr) = iValue;
			}
			else
			{
				*((unsigned int *)pvAddr) += (fDec ? -iValue : iValue);
			}
			iNewValue = *((unsigned int *)pvAddr);
			break;
		}

		switch(pstEntry->fNotify)
		{
			case 0:
				clan_set_dirty(pstZoneClan, CACHE_FLAG_DIRTY);
				break;
			case 1:
				clan_set_dirty_notify(pstZoneClan, CACHE_FLAG_DIRTY);
				break;
			case 2:
				clan_set_dirty_notify(pstZoneClan, CACHE_FLAG_IMM);
				break;
			case 3:
				clan_set_dirty(pstZoneClan, CACHE_FLAG_IMM);
				player_clan_view_chg(pstEnv, pstZoneClan, CLAN_VIEW_CHG_MOD);
				break;
			default:
				break;
		}
	
	}
	clan_property_do_notify(pstEnv, pstZoneClan, iNewValue, iWhich);
	return 0;
}

int clan_property_modify_test(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iWhich,
                         int iValue, int fDec)
{
	int iNewValue = 0;
	void *pvAddr = NULL;
	const ClanPropertyEntry *pstEntry = NULL;

	if (iWhich < 0 || iWhich >= (int)CCH(astClanProperties) || iValue < 0)
	{
		return -1;
	}

	pstEntry = astClanProperties + iWhich;

	//属性废弃失效了
	if (pstEntry->iInvalid) 
	{
		return -1;
	}
	
	pvAddr = ((char *)pstZoneClan + pstEntry->nOffset);

	switch(pstEntry->iBytes)
	{
	case 1:
		iNewValue = *((unsigned char *)pvAddr);
		break;

	case 2:
		iNewValue = *((unsigned short *)pvAddr);
		break;

	case 4:
		iNewValue = *((unsigned int *)pvAddr);
		break;

	default:
		return -1;
		break;
	}

	if (iValue >= 0)
	{

		if (fDec > 0)
		{
			if (iNewValue <= pstEntry->iMin && iValue > 0)
			{
				return -1;
			}
			else if ((long long)iNewValue - iValue < pstEntry->iMin)
			{
				return -1;
			}

			return 0;
		}
		else if (fDec == 0)
		{
			if (iNewValue >= pstEntry->iMax && iValue > 0)
			{
				return -1;
			}
			else if ((long long)iNewValue + iValue >= pstEntry->iMax)
			{
				return -1;
			}

			return 0;
		}
		else if (fDec == -1)
		{
			iNewValue = iValue;

			if (iNewValue >= pstEntry->iMax)
			{
				return -1;
			}

			if (iNewValue <= pstEntry->iMin)
			{
				return -1;
			}

			return 0;
		}	
	}
	
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}



//单人冲掉排名
int player_flush_battle_pworld_queue(ZONESVRENV* pstEnv, Player *pstPlayer)
{	

	if(INST_MODE_ZONE_SVR == pstEnv->iInstMode &&
		pstEnv->pstConf->BattleFlag)
	{
		if(is_player_apply_battle(pstEnv, pstPlayer))
		{
			SSPKG stPkg;
			SSFLUSHBATTLEQUEUE  *pstFlushBattleQueue = &stPkg.Body.FlushBattleQueue;
			
			pstFlushBattleQueue->Member.Uin = pstPlayer->stRoleData.Uin;
			pstFlushBattleQueue->Member.RoleID = pstPlayer->stRoleData.RoleID;

			pstFlushBattleQueue->Captain.Uin = 0;//作为区分依据
			pstFlushBattleQueue->Captain.RoleID = 0;

			Z_SSHEAD_INIT(&stPkg.Head, FLUSH_BATTLE_QUEUE);
			z_ssmsg_send(pstEnv, &stPkg, pstEnv->iBattlePveBattleID);
		}
	}
	else
	{
		
	}
	
	return 0;
}

static int clan_mem_on_exit(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                            Player *pstPlayer, int fActive)
{
	// 记录主动退会的时间。

	//注意，pstZoneClan 可能是NULL

	//todo:公会跨服战退出排队队列
	if (pstZoneClan && pstZoneClan->stZoneClanInfo.stBattleApplyData.iBattleID &&
		pstPlayer->stOnline.stSpanOnline.stApply.iBattleApply)
	{
		player_flush_battle_pworld_queue(pstEnv,pstPlayer);
		player_action_clan_battle_apply_info(pstEnv,pstPlayer,0);
	}

	//lzk  退出公会或被踢出公会时，发送公会贡献返还邮件
	clan_exit_contribute_mail(pstEnv,pstPlayer,pstZoneClan);
	
	bzero(&pstPlayer->stRoleData.MiscInfo.ClanInfo,
		  sizeof(pstPlayer->stRoleData.MiscInfo.ClanInfo));

	bzero(&pstPlayer->stRoleData.MiscInfo.ClanSkillInfo,
		  sizeof(pstPlayer->stRoleData.MiscInfo.ClanSkillInfo));

	player_action_player_clan_skill_info(pstEnv,pstPlayer, 
								&pstPlayer->stRoleData.MiscInfo.ClanSkillInfo);
		
	//末世巅峰冠军界面公会信息更新
	dianfeng_chamption_clan_info_update(pstEnv,pstPlayer,0);

	//刷新属性
	z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);

	if (fActive)
	{
		pstPlayer->stRoleData.MiscInfo.ClanInfo.ExitPunishTime 
			= pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_CLAN_SNAP;

	clan_action_exit(pstEnv, pstPlayer);

	strong_pworld_force_exit(pstEnv, pstPlayer,LJY_JUDIAN1);
	//strong_pworld_player_exit_clan(pstEnv,pstPlayer);
	player_clan_leave_city(pstEnv, pstPlayer, pstZoneClan);
	player_clan_disable(pstEnv, pstPlayer);

	relation_dbbuddy_chg(pstEnv, pstPlayer, 1, 1);
	del_task_clan(pstEnv, pstPlayer);

	player_exit_clan_clear_everydaypworld(pstEnv, pstZoneClan, pstPlayer);
	player_dyn_active_helper_info(pstEnv,pstPlayer,CLAN_TOUCH_ACTIVE_HELP_ID,ACTIVE_STATUS_INIT);
	player_dyn_active_helper_info(pstEnv,pstPlayer,CLAN_BANISH_ACTIVE_HELP_ID,ACTIVE_STATUS_INIT);
	player_clan_active_ui_info_clan_spy(pstEnv,pstPlayer,ACTIVE_GAME_SVR_TYPE_UI_CLOSE);
	player_clan_active_ui_info_clan_machine_pworld(pstEnv,pstPlayer,ACTIVE_GAME_SVR_TYPE_UI_CLOSE);

	clan_pet_player_leave(pstEnv, pstPlayer, pstZoneClan);
	
	if (pstZoneClan)
	{
		range_up_clanname( pstEnv, pstPlayer->stRoleData.RoleID, pstZoneClan->stClan.ClanProf.Name, "", 1);
	}
	
	return 0;
}


static int clan_partial_destroy(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan)
{
	CLANMEM *astMem = pstZoneClan->stClan.ClanCore.ClanMem;
	int i = pstZoneClan->stClan.ClanProf.MemNum;

	strong_glory_close_by_clan_destory(pstEnv, pstZoneClan);
	strong_clan_destory(pstEnv, pstZoneClan);
	span_strong_clan_destory(pstEnv, pstZoneClan->stClan.ClanProf.Name);
	
	while(i--)
	{
		Player *pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
		                                  astMem[i].RoleMember.RoleName);
		if (pstPlayer)
		{
			clan_mem_on_exit(pstEnv, pstZoneClan, pstPlayer, 0);
		}
	}
	clan_advance_cleanup(pstEnv, pstZoneClan);

	clan_destroy_city(pstEnv, pstZoneClan);

	player_clan_view_chg(pstEnv, pstZoneClan, CLAN_VIEW_CHG_DEL);
	return 0;
}


static int clan_can_create(ZONESVRENV *pstEnv, Player *pstPlayer,
                           const char *psClanName,CLANLEVELDEF *pstInitialDef)
{
	int i;
	ClanObj *pstClanObj = pstEnv->pstClanObj;
	CLANINFO *pstClanInfo = &pstPlayer->stRoleData.MiscInfo.ClanInfo;

	if (pstClanInfo->ClanId.GID != 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_SVR,
		           ZONEERR_CLAN217);
		return 0;
	}

	if (pstClanObj->iClanNum < 0 || pstClanObj->iClanNum >= CLAN_MAX_NUM_SOFT)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	
		   "number of clan >= CLAN_MAX_NUM_SOFT.can not create clan any more.");
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR,
		           ZONEERR_CLAN218);
		return 0;
	}

	if (player_clan_check_exit_punish_time(pstEnv, pstPlayer) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR,
		           ZONEERR_CLAN219,
		           clan_exit_remain_time_zh(pstEnv, pstPlayer));
		return 0;
	}

	if (pstPlayer->stRoleData.Level < CLAN_CREATE_MIN_ROLE_LV)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR,
		           ZONEERR_CLAN220,
		           CLAN_CREATE_MIN_ROLE_LV);
		return 0;
	}

	for (i=0; i<MAX_CLAN_MAKE_MATERIAL; i++)
	{
		if (pstInitialDef->Required.Material[i].ID == 0)
		{
			break;
		}

		if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstInitialDef->Required.Material[i].ID, 
								pstInitialDef->Required.Material[i].Num, -1, NULL, PACKAGE_DEC_NONE))
		{
			ITEMDEF *pstItemDef;

			pstItemDef = z_find_itemdef(pstEnv, pstInitialDef->Required.Material[i].ID);

			if (pstItemDef)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_CLAN221,
							pstInitialDef->Required.Material[i].Num, pstItemDef->Name);
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_CLAN222);
			}
			
			return 0;
		}

	}

	if (package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL,
	                           pstInitialDef->Required.Money) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_CLAN223,
		           MONEY_TYPE_BULL_NAME, pstInitialDef->Required.Money);
		return 0;
	}

	if (player_clan_check_name(pstEnv, pstPlayer, psClanName) < 0)
	{
		return 0;
	}

	return 1;
}


static int clan_low_init(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                         Player *pstPlayer, CLANMEM *pstNewMem,
                         const char *psClanName)
{
	const CLANLEVELDEF *pstInitialDef = z_clan_level_def(pstEnv, 0);

	CLANLEVELDEF *pstDef = z_clan_level_def(pstEnv, 1);

	if (!pstInitialDef || !pstDef)
	{
		return -1;
	}

	// 创建公会初始数据。
	bzero(pstZoneClan, sizeof(*pstZoneClan));
		
	pstZoneClan->stClan.ClanProf.GID = z_get_gid(pstEnv->pstAppCtx, pstEnv);
	pstZoneClan->stClan.ClanProf.Id = clan_get_id(pstEnv);
	pstZoneClan->stClan.ClanProf.Level = 1;
	pstZoneClan->stClan.ClanProf.MemNum = 0;
	pstZoneClan->stClan.ClanProf.ActiveValue = 0;
	pstZoneClan->stClan.ClanProf.StableValue =
		pstInitialDef->Required.StableValue;
	pstZoneClan->stClan.ClanCore.MemNum = 0;
	STRNCPY(pstZoneClan->stClan.ClanProf.Name, psClanName, 
		    CCH(pstZoneClan->stClan.ClanProf.Name));
	pstZoneClan->stClan.ClanProf.Money = pstDef->PresentClanMoney;

	pstZoneClan->stClan.ClanBrief.CreateTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstZoneClan->stClan.ClanBrief.Leader.RoleID = pstPlayer->stRoleData.RoleID;
	STRNCPY(pstZoneClan->stClan.ClanBrief.Leader.RoleName,
	        pstPlayer->stRoleData.RoleName,
	        CCH(pstZoneClan->stClan.ClanBrief.Leader.RoleName));

	// 最大同时制造数可能会改变
	pstZoneClan->stClan.ClanAdvance.Produce.MaxArmMakingNum = 1;
	pstZoneClan->stClan.ClanAdvance.Produce.MaxCollectionMakingNum = 1;

	pstZoneClan->stClan.ClanCore.ClanGroup.GroupNum = MAX_CLAN_GROUP_NUM;

	/*
	pstZoneClan->stClan.ClanAdvance.Storage.MaxMachineNum = 
		pstInitialDef->MaxMachineGrid;
	pstZoneClan->stClan.ClanAdvance.Storage.MaxResourceNum = 
		pstInitialDef->MaxResourceGrid;
	*/
	pstZoneClan->stClan.ClanDestroyTime = 0;
	pstZoneClan->stClan.ClanActiveTime
		= pstEnv->pstAppCtx->stCurr.tv_sec;
	pstZoneClan->stZoneClanInfo.iListVersion 
		= pstEnv->pstAppCtx->stCurr.tv_sec;

	return 0;
	UNUSED(pstNewMem);	//add by paraunused.pl
}


static int clan_install(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	ClanObj *pstClanObj = pstEnv->pstClanObj;
	ClanHash stClanHash, *pstClanHash;
	const CLANLEVELDEF *pstDef = z_clan_level_def(pstEnv, 1);

	if (!pstDef ||
		pstZoneClan != pstClanObj->astZoneClan + pstClanObj->iClanNum)
	{
		return -1;
	}

	// 插入哈希表
	stClanHash.uiClanId = pstZoneClan->stClan.ClanProf.Id;
	pstClanHash = sht_insert_unique(pstEnv->pstShtClan, &stClanHash,
	                                ClanHashCmp, ClanHashCode);
	if (!pstClanHash)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "clan hash error");
		pstEnv->pstClanObj->stClanObjInfo.iClanStat = CLAN_STAT_FAIL;
		return -1;
	}

	pstClanHash->iIdx = pstClanObj->iClanNum;
	pstClanHash->uiClanId = pstZoneClan->stClan.ClanProf.Id;
	pstClanHash->ullClanGID = pstZoneClan->stClan.ClanProf.GID;

	// 插入公会表
	++pstClanObj->iClanNum;
	pstClanObj->stClanObjInfo.iListVersion++; // ListVersion版本变化。
	player_clan_view_chg(pstEnv, pstZoneClan, CLAN_VIEW_CHG_ADD);
	clan_effect_broadcast(pstEnv, pstZoneClan, 0, pstDef->EffectString);

	//整理clan hash
	clan_reidx_hash(pstEnv, pstClanObj->iClanNum - 1);

	// 立即写入数据库
	clan_set_dirty(pstZoneClan, CACHE_FLAG_IMM);
	return 0;
}

int clan_pos_cmp(int iRhs, int iLhs)
{
	assert(iRhs >= CLAN_POS_NORMAL && iRhs <= CLAN_POS_GROUP_6);
	assert(iLhs >= CLAN_POS_NORMAL && iLhs <= CLAN_POS_GROUP_6);

	return kClanPosCmp[iRhs][iLhs];
}

int player_clan_update_mem(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	CLANMEM *pstMem = NULL;
	int iOffset = 0;

	if (!pstZoneClan)
	{
		return -1;
	}

	pstMem = clan_id_mem(pstZoneClan, pstPlayer->stRoleData.RoleID, &iOffset);
	if (!pstMem)
	{
		return -1;
	}

	pstMem->Level = pstPlayer->stRoleData.Level;
	pstMem->Contribute = pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute;
	pstMem->Career = pstPlayer->stRoleData.Career;
	//pstMem->TotalContribute = max(pstMem->TotalContribute,pstMem->Contribute);
	return player_clan_mem_chg_broadcast(pstEnv, pstZoneClan, pstMem,
	                                     CLAN_MEMBER_CHG_MOD);
}


int player_clan_join_clan(ZONESVRENV* pstEnv, Player *pstPlayer,
                          ZoneClan * pstClan)
{
	CLANMEM stChg;
	
	if (clan_add_mem(pstEnv, pstClan, pstPlayer, CLAN_POS_NORMAL, &stChg) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
		               ZONEERR_CLAN13);
		return -1;
	}

	clan_set_dirty_notify(pstClan, CACHE_FLAG_IMM);
	++pstClan->stZoneClanInfo.iListVersion;

	player_clan_mem_chg_broadcast(pstEnv, pstClan, &stChg, CLAN_MEMBER_CHG_ADD);
	player_clan_snapshot(pstEnv, pstPlayer);

	clan_action(pstEnv, pstClan, &stChg);

	player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN, ZONEERR_CLANBROAD228,
	                          pstPlayer->stRoleData.RoleName);
	clan_boon_all_notify(pstEnv, pstClan, pstPlayer);

	relation_dbbuddy_chg(pstEnv, pstPlayer, 1, 1);
	task_player_trace(pstEnv, pstPlayer, TASK_OBJ_JOIN_CLAN);
	z_role_clan_oplog(pstEnv, pstPlayer->stRoleData.RoleName, clan_id_warp(&pstClan->stClan),
		              OP_CLAN_JOIN,NULL);

	player_clan_color_info(pstEnv, pstPlayer);
	player_clan_everyday_pworld_action(pstEnv, pstPlayer);
	player_clan_map_pos_info(pstEnv, pstPlayer);

	player_clan_view_chg(pstEnv, pstClan, CLAN_VIEW_CHG_MOD);

	player_clan_yesterday_clan_touch_notify(pstEnv,pstPlayer);
	player_dyn_active_helper_info(pstEnv,pstPlayer,CLAN_TOUCH_ACTIVE_HELP_ID,pstClan->stZoneClanInfo.iClanTouchStatus);
	player_clan_active_ui_info_clan_spy(pstEnv,pstPlayer,ACTIVE_GAME_SVR_TYPE_UI_INFO);
	player_clan_active_ui_info_clan_machine_pworld(pstEnv,pstPlayer,ACTIVE_GAME_SVR_TYPE_UI_INFO);

	// 这个函数需要改个名0_0;
	strong_glory_player_login( pstEnv, pstPlayer);
	clan_dart_login(pstEnv, pstPlayer);

	//末世巅峰冠军公会信息更新
	dianfeng_chamption_clan_info_update(pstEnv,pstPlayer,pstClan->stClan.ClanProf.Id);

	//公会面板资金提示界面的内容也需要更新
	clan_spend_notify(pstEnv, pstPlayer,pstClan);
	
	return 0;
}


static int player_clan_join_union(ZONESVRENV *pstEnv, Player *pstPlayer, 
                                  ZoneClan *pstHostClan,
                                  ZoneClan *pstCustomClan)
{
	CLANRELATION *pstPos = NULL;

	UNUSED(pstPlayer);

	if (!pstCustomClan || !pstHostClan || pstCustomClan == pstHostClan)
	{
		return -1;
	}

	if (pstHostClan->stClan.ClanInner.ClanFriendNum >=
		CCH(pstHostClan->stClan.ClanInner.ClanFriends))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
	               ZONEERR_CLAN224,
	               pstHostClan->stClan.ClanProf.Name);
		return -1;
	}

	if (pstCustomClan->stClan.ClanInner.ClanFriendNum >=
		CCH(pstCustomClan->stClan.ClanInner.ClanFriends))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP,
	               ZONEERR_CLAN225,
	               pstCustomClan->stClan.ClanProf.Name);
		return -1;
	}

	// 各自加对方为盟友
	pstPos = pstHostClan->stClan.ClanInner.ClanFriends +
	         pstHostClan->stClan.ClanInner.ClanFriendNum++;
	pstPos->ClanId = *clan_id_warp(&pstCustomClan->stClan);
	pstPos->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
	clan_set_dirty(pstHostClan, CACHE_FLAG_DIRTY);

	pstPos = pstCustomClan->stClan.ClanInner.ClanFriends +
	         pstCustomClan->stClan.ClanInner.ClanFriendNum++;
	pstPos->ClanId = *clan_id_warp(&pstHostClan->stClan);
	pstPos->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
	clan_set_dirty(pstCustomClan, CACHE_FLAG_DIRTY);

	// 通知双方公会
	player_clan_diplomatism_chg_broadcast(pstEnv, pstCustomClan);
	player_clan_msg_broadcast(pstEnv, pstCustomClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD229,
	                          pstHostClan->stClan.ClanProf.Name);

	player_clan_diplomatism_chg_broadcast(pstEnv, pstHostClan);
	player_clan_msg_broadcast(pstEnv, pstHostClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD229,
	                          pstCustomClan->stClan.ClanProf.Name);
	return 0;
}


static int player_clan_join_enemy(ZONESVRENV *pstEnv, Player *pstPlayer,
                                  ZoneClan *pstMyClan,
                                  ZoneClan *pstOtherClan)
{
	CLANRELATION *pstPos = NULL;
	CLANLEVELDEF *pstLevel = NULL;
	int iCD = 0;

	UNUSED(pstPlayer);

	if (!pstOtherClan || !pstMyClan || pstMyClan == pstOtherClan)
	{
		return -1;
	}
	
	if (pstMyClan->stClan.ClanInner.ClanEnemyNum >= CLAN_MAX_ENEMY)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP,
	               ZONEERR_CLAN226);
		return -1;
	}

	/*
	for (i = 0; i < pstMyClan->stClan.ClanInner.ClanEnemyNum; ++i)
	{
		if (pstMyClan->stClan.ClanInner.ClanEnemys[i].ClanId.GID ==
			pstOtherClan->stClan.ClanProf.GID)
		{
			z_sys_strf(pstEnv, pstPlayer, SYS_TIP,
		               "本公会已于公会\"%s\"处于战争状态",
		               pstOtherClan->stClan.ClanProf.Name);
			return -1;
		}
	}
	*/

	// 将其放入敌对列表
	pstPos = pstMyClan->stClan.ClanInner.ClanEnemys +
	         pstMyClan->stClan.ClanInner.ClanEnemyNum++;
	pstPos->ClanId = *clan_id_warp(&pstOtherClan->stClan);
	pstPos->Time = pstEnv->pstAppCtx->stCurr.tv_sec + CLAN_WAR_PERSISTED_TIME;
	clan_set_dirty(pstMyClan, CACHE_FLAG_DIRTY);

	if (pstMyClan->stClan.ClanInner.ClanEnemyNum == 1)
	{
		pstMyClan->stZoneClanInfo.iWarNotify =
			pstEnv->pstAppCtx->stCurr.tv_sec + CLAN_WAR_NOTIFY_INTERVAL;
	}	

	

	// 通知双方公会
	player_clan_msg_broadcast(pstEnv, pstOtherClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD230,
	                          pstMyClan->stClan.ClanProf.Name);

	player_clan_diplomatism_chg_broadcast(pstEnv, pstMyClan);
	player_clan_msg_broadcast(pstEnv, pstMyClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD231,
	                          pstOtherClan->stClan.ClanProf.Name);

	//将自己放入对方的敌对列表,并通知对方公会
	pstPos = pstOtherClan->stClan.ClanInner.ClanEnemys +
	         pstOtherClan->stClan.ClanInner.ClanEnemyNum++;
	pstPos->ClanId = *clan_id_warp(&pstMyClan->stClan);
	pstPos->Time = pstEnv->pstAppCtx->stCurr.tv_sec + CLAN_WAR_PERSISTED_TIME;
	clan_set_dirty(pstOtherClan, CACHE_FLAG_DIRTY);

	if (pstOtherClan->stClan.ClanInner.ClanEnemyNum == 1)
	{
		pstOtherClan->stZoneClanInfo.iWarNotify =
			pstEnv->pstAppCtx->stCurr.tv_sec + CLAN_WAR_NOTIFY_INTERVAL;
	}	

	player_clan_diplomatism_chg_broadcast(pstEnv, pstOtherClan);

	//设置的宣战冷却时间
	{
		pstLevel = z_clan_level_def(pstEnv, pstMyClan->stClan.ClanProf.Level);
		if (pstLevel)
		{
			iCD = pstLevel->EnemyCDTime;
		}
		clan_set_enemy_cd(pstEnv,pstMyClan,pstOtherClan,iCD);
	}
	
	return 0;
}


unsigned int player_clan_position(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	const CLANMEM *pstClanMem = NULL;

	if (!pstClan || pstClan->stZoneClanInfo.iDirty == CACHE_FLAG_INEXIST)
	{
		return CLAN_POS_NOT_MEMBER;
	}

	pstClanMem = clan_id_mem(pstClan, pstPlayer->stRoleData.RoleID,
	                         &pstPlayer->stOnline.iClanMemIdx);
	if (!pstClanMem)
	{
		tlog_info(pstEnv->pstLogCat, 0, 0,
		           "Role has clan, but clan don't has role.");
		return CLAN_POS_NOT_MEMBER;
	}
	return pstClanMem->Position;
}


static int player_clan_check_name(ZONESVRENV *pstEnv, Player *pstPlayer,
                                  const char *psClanName)
{
	const ClanObj *pstClanObj = pstEnv->pstClanObj;
	int i;

	UNUSED(pstPlayer);

	if (!psClanName || !psClanName[0])
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN14);
		goto fatal;
	}

	// 老大哥正看者你。
	if ((0 == pstEnv->pstConf->CountryVer && CheckCharacter((char *)psClanName)) ||
		(CheckDirty(psClanName , 0x1f) & 0x1f))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN15);
		goto fatal;
	}

	for(i = 0; i < pstClanObj->iClanNum; ++i)
	{
		const CLAN *pstClan = &pstClanObj->astZoneClan[i].stClan;

		if (strcmp(pstClan->ClanProf.Name, psClanName) == 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN16);
			goto fatal;
		}
	}

	return 0;

fatal:
	return -1;
}

int player_clan_group_broadcast_except(ZONESVRENV *pstEnv, ZoneClan *pstClan,
                                 CSPKG *pstPkg, Player *pstPlayerExcept)
{
	int i, iCount = 0;
	Player *pstPlayer = NULL;
	Player * apstPlayers[CLAN_MAX_MEM];
	CLANMEM *pstMem;

	pstMem = clan_id_mem(pstClan, pstPlayerExcept->stRoleData.RoleID, 
	                     &pstPlayerExcept->stOnline.iClanMemIdx);
	if (!pstMem || pstMem->GroupID == 0 )
	{
		return -1;
	}
	
	for(i = 0; i < pstClan->stClan.ClanProf.MemNum; ++i)
	{
		if (!pstClan->stClan.ClanCore.ClanMem[i].Online)
		{
			continue;
		}

		pstPlayer = z_name_player(
			pstEnv->pstAppCtx,
			pstEnv,
			pstClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName);
		if (!pstPlayer || pstPlayerExcept == pstPlayer)
		{
			continue;
		}

		if(pstClan->stClan.ClanCore.ClanMem[i].GroupID != pstMem->GroupID)
		{
			continue;
		}
		
		if (pstPlayerExcept && 0 <= player_black_find_byid(&pstPlayer->stRoleData.MiscInfo, pstPlayerExcept->stRoleData.RoleID))
		{
			continue;
		}

		apstPlayers[iCount++] = pstPlayer;
	}

	if (iCount > 0)
	{
		return z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
		                                iCount, pstPkg);
	}
	return 0;
}

int player_clan_broadcast_except(ZONESVRENV *pstEnv, ZoneClan *pstClan,
                                 CSPKG *pstPkg, int fIsMemChg,
                                 Player *pstPlayerExcept)
{
	int i, iCount = 0;
	Player *pstPlayer = NULL;
	Player * apstPlayers[CLAN_MAX_MEM];
	
	for(i = 0; i < pstClan->stClan.ClanProf.MemNum; ++i)
	{
		if (!pstClan->stClan.ClanCore.ClanMem[i].Online)
		{
			continue;
		}

		pstPlayer = z_name_player(
			pstEnv->pstAppCtx,
			pstEnv,
			pstClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName);
		if (!pstPlayer || pstPlayerExcept == pstPlayer)
		{
			continue;
		}

		if (fIsMemChg && !(pstPlayer->stOnline.cMiscFlag & MISC_FLAG_CLAN_SNAP))
		{
			continue;
		}

		if (pstPlayerExcept && 0 <= player_black_find_byid(&pstPlayer->stRoleData.MiscInfo, pstPlayerExcept->stRoleData.RoleID))
		{
			continue;
		}

		apstPlayers[iCount++] = pstPlayer;
	}

	if (iCount > 0)
	{
		return z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
		                                iCount, pstPkg);
	}
	return 0;
}

int player_clan_broadcast(ZONESVRENV *pstEnv, ZoneClan *pstClan, CSPKG *pstPkg,
                          int fIsMemChg)
{
	return player_clan_broadcast_except(pstEnv, pstClan, pstPkg, fIsMemChg, NULL);
}

int player_clan_msg_broadcast_raw(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                              int iSysType, char * pszMsg)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;


	pstSysMsg->Type = iSysType;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;

	snprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), pszMsg);

	pstSysMsg->Msg[sizeof(pstSysMsg->Msg) - 1] = 0;

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	z_pkg_to_mud(pstEnv,pstPkg, NULL,pstZoneClan->stClan.ClanProf.Id,PKG_TRANS_CLAN);
	return clan_broadcast_if(pstEnv, pstZoneClan, pstPkg, ClanEveryBody);
}


int player_clan_msg_broadcast(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                              int iSysType, int iErrno, ...)
{
	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	const char* pszErr;
	va_list marker;

	va_start( marker, iErrno );
	pszErr	=	ZONE_GET_ERR(iErrno, 0);

	pstSysMsg->Type = iSysType;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
	
	if( !pszErr )
	{
		snprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), "SysNo : %d", iErrno );
	}
	else
	{
		vsnprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), pszErr, marker);
	}

	pstSysMsg->Msg[sizeof(pstSysMsg->Msg) - 1] = 0;
	va_end(marker);

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	z_pkg_to_mud(pstEnv,pstPkg, NULL,pstZoneClan->stClan.ClanProf.Id,PKG_TRANS_CLAN);
	return clan_broadcast_if(pstEnv, pstZoneClan, pstPkg, ClanEveryBody);
}


static int player_clan_bulliten_broadcast(ZONESVRENV *pstEnv,
                                          ZoneClan *pstZoneClan)
{
	CSPKG stPkg;
	CSCLANBULLETIN *pstBulletin = &stPkg.Body.ClanSvr.ClanSvrData.ClanBulletin;

	STRNCPY(pstBulletin->BulletinText, pstZoneClan->stClan.ClanInner.GongGao,
	        SHORT_DESC_LEN);
	pstBulletin->BulletinLastWrite =
		pstZoneClan->stClan.ClanInner.GongGaoLastWrite;

	stPkg.Body.ClanSvr.Operation = CLAN_SVR_BULLETIN;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	z_pkg_to_mud(pstEnv,&stPkg, NULL,pstZoneClan->stClan.ClanProf.Id,PKG_TRANS_CLAN);
	return clan_broadcast_if(pstEnv, pstZoneClan, &stPkg, ClanEveryBody);
}


int player_clan_mem_chg_broadcast(ZONESVRENV *pstEnv,
                                         ZoneClan *pstZoneClan,
                                         const CLANMEM *pstMem,
                                         int iChgCode)
{
	CSPKG stPkg;
	CSCLANMEMBERCHG *pstMemChg = &stPkg.Body.ClanSvr.ClanSvrData.ClanMemberChg;

	if (!pstZoneClan || !pstMem)
	{
		return -1;
	}

	pstMemChg->Code = (unsigned char)iChgCode;
	memcpy(&pstMemChg->MemInfo, pstMem, sizeof(pstMemChg->MemInfo));

	stPkg.Body.ClanSvr.Operation = CLAN_SVR_MEMBER_CHG;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	z_pkg_to_mud(pstEnv,&stPkg, NULL,pstZoneClan->stClan.ClanProf.Id,PKG_TRANS_CLAN);
	
	return clan_broadcast_if(pstEnv, pstZoneClan, &stPkg,
	                         ClanBodyHasMemberSnapshot);
}


int player_clan_notify_created(ZONESVRENV *pstEnv, Player *pstPlayer,
                                      const ZoneClan *pstZoneClan)
{
	CSPKG stPkg;
	
	if (!pstZoneClan)
	{
		return -1;
	}

	STRNCPY(stPkg.Body.ClanSvr.ClanSvrData.ClanName,
	        pstZoneClan->stClan.ClanProf.Name,
	        CCH(stPkg.Body.ClanSvr.ClanSvrData.ClanName));

	stPkg.Body.ClanSvr.Operation = CLAN_SVR_CREATE;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	player_clan_member_snapshot(pstEnv, pstPlayer);
	player_clan_advance(pstEnv, pstPlayer);
	return 0;
}


int player_clan_create(ZONESVRENV *pstEnv, Player *pstPlayer,
                       const char *psClanName,int iCheck)
{
	int iRet;
	int i;
	CLANMEM stNewMem;
	ClanObj *pstClanObj = pstEnv->pstClanObj;
	ZoneClan *pstZoneClan = pstClanObj->astZoneClan + pstClanObj->iClanNum;
	CLANLEVELDEF *pstInitialDef = z_clan_level_def(pstEnv, 0);
	
	if (!pstInitialDef)
	{
		return -1;
	}
	
	if (iCheck)
	{
		if (check_npc_ui(pstEnv, pstPlayer, UI_TYPE_CLAN_CREATE) < 0)
		{
			return -1;
		}

		if (!clan_can_create(pstEnv, pstPlayer, psClanName,pstInitialDef))
		{
			return -1;
		}
	}
	else
	{
		ClanObj *pstClanObj = pstEnv->pstClanObj;
		CLANINFO *pstClanInfo = &pstPlayer->stRoleData.MiscInfo.ClanInfo;

		if (pstClanInfo->ClanId.GID != 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_SVR,
			           ZONEERR_CLAN217);
			return -1;
		}

		if (pstClanObj->iClanNum < 0 || pstClanObj->iClanNum >= CLAN_MAX_NUM_SOFT)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,	
			   "number of clan >= CLAN_MAX_NUM_SOFT.can not create clan any more.");
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_SVR,
			           ZONEERR_CLAN218);
			return -1;
		}
		
		if (player_clan_check_name(pstEnv, pstPlayer, psClanName) < 0)
		{
			return -1;
		}
	}

	if (clan_low_init(pstEnv, pstZoneClan, pstPlayer, &stNewMem,
		psClanName) < 0)
	{
		return -1;
	}

	if (clan_install(pstEnv, pstZoneClan) < 0)
	{
		return -1;
	}

	// 填充新公会的数据
	if (clan_add_mem(pstEnv, pstZoneClan, pstPlayer, CLAN_POS_PRESIDENT,
	                 &stNewMem) < 0)
	{
		return -1;
	}

	// 更新官员信息
	clan_update_officers(pstEnv, pstZoneClan);

	iRet = package_dec_money(pstEnv, pstPlayer, ATTR_ID_BULL, pstInitialDef->Required.Money);
	for (i=0; i<MAX_CLAN_MAKE_MATERIAL; i++)
	{
		if (pstInitialDef->Required.Material[i].ID == 0)
		{
			break;
		}

		package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstInitialDef->Required.Material[i].ID, 
								pstInitialDef->Required.Material[i].Num, -1, NULL, PACKAGE_DEC_NONE, IMPITEM_OPTYPE_CLAN_RESOURCE);
	}


	z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_CLAN_CREATE, 0,  pstInitialDef->Required.Money-iRet, iRet, "创建公会");

	tlog_info(pstEnv->pstLogCat, 0, 0, "clan %lld %d create", 
		      pstZoneClan->stClan.ClanProf.GID,
		      pstZoneClan->stClan.ClanProf.Id);
	player_clan_snapshot(pstEnv, pstPlayer);

	clan_action(pstEnv, pstZoneClan, &stNewMem);
	relation_dbbuddy_chg(pstEnv, pstPlayer, 1, 1);
	z_role_clan_oplog(pstEnv, pstPlayer->stRoleData.RoleName, clan_id_warp(&pstZoneClan->stClan),
		              OP_CLAN_CREATE,NULL);

	player_clan_color_info(pstEnv, pstPlayer);

	//lzk  加上不返回公会资金的标志
	pstZoneClan->stClan.ClanProf.Flag |= CLAN_FLAG_SKILL_RESET_RETURN_CLAN_MONEY;
	
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN227);
	z_sys_strf_broadcast(pstEnv, SYS_CHAT,
		ZONEERR_BROAD324, 
		pstPlayer->stRoleData.RoleName,
		pstZoneClan->stClan.ClanProf.Name);
	
	clan_city_upgrade(pstEnv, pstZoneClan, pstPlayer, NULL);

	player_clan_active_ui_info_clan_spy(pstEnv,pstPlayer,ACTIVE_GAME_SVR_TYPE_UI_INFO);

	//末世巅峰冠军公会更新
	dianfeng_chamption_clan_info_update(pstEnv,pstPlayer,pstZoneClan->stClan.ClanProf.Id);
	
	return 0;
}


int player_clan_invite_role(ZONESVRENV *pstEnv, Player *pstPlayer,
                            const char *psRoleName )
{
	CSPKG stPkg;
	Player *pstCustom = z_name_player(pstEnv->pstAppCtx, pstEnv, psRoleName);
	CSCLANASK *pstAsk = &stPkg.Body.ClanSvr.ClanSvrData.ClanAskJoinClan;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);

	if (!pstCustom)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
		               ZONEERR_CLAN17);
		return -1;
	}

	if (player_clan_check_exit_punish_time(pstEnv, pstCustom) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
		           ZONEERR_CLAN18,
		           clan_exit_remain_time_zh(pstEnv, pstCustom));
		return -1;
	}

	if (clan_check_mem_limit(pstEnv, pstClan) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
		              ZONEERR_CLAN19);
		return -1;
	}

	if (pstCustom->stRoleData.MiscInfo.ClanInfo.ClanId.GID != 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
		               ZONEERR_CLAN20);
		return -1;
	}

	if (pstCustom->stOnline.stClanOnline.stClanID.GID != 0 &&
		pstCustom->stOnline.stClanOnline.tTime + 100 >
		pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
		                ZONEERR_CLAN21);
		return -1;
	}

	// 询问被邀请玩家同不同意加入公会。
 	pstAsk->ID = *clan_id_warp(&pstClan->stClan);
	STRNCPY(pstAsk->HostName, pstPlayer->stRoleData.RoleName, 
	        CCH(pstAsk->HostName));

	stPkg.Body.ClanSvr.Operation = CLAN_SVR_ASK_JOIN_CLAN;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	z_cltmsg_send(pstEnv, pstCustom, &stPkg);

	// 设置被邀请角色的信息。
	pstCustom->stOnline.stClanOnline.stClanID = *clan_id_warp(&pstClan->stClan);
	pstCustom->stOnline.stClanOnline.tTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstCustom->stOnline.stClanOnline.ullRoleID = pstPlayer->stRoleData.RoleID;

	return 0;
}


int player_clan_del_mem(ZONESVRENV *pstEnv, Player *pstPlayer, 
                        const ROLEMEMBER *pstRoleID)
{
	Player *pstMemPlayer = NULL;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	size_t nNum = pstClan->stClan.ClanProf.MemNum;
	CLANMEM stKey, *pstKey = NULL;

	if (!(pstKey = clan_id_mem(pstClan, pstRoleID->RoleID, NULL)))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN228,
		           pstRoleID->RoleName);
		return -1;
	}

	if (pstKey->RoleMember.RoleID == pstPlayer->stRoleData.RoleID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
		               ZONEERR_CLAN22);
		return -1;
	}

	//if (pstKey->Position >= player_clan_position(pstEnv, pstPlayer))
	if (player_clan_pos_cmp(pstEnv, pstPlayer, pstKey->Position) <= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
		               ZONEERR_CLAN23);
		return -1;
	}

	//贡献度邮件
	//clan_kick_contribute_mail(pstEnv, pstClan->stClan.ClanProf.Name, pstKey ->RoleMember.RoleName, pstKey->Contribute);

	// 确保角色不在公会机械上
	player_clan_machine_ensure_down(pstEnv, pstPlayer);

	bzero(&stKey, sizeof(stKey));
	stKey = *pstKey;
	pstKey = NULL;
	if (!my_bdelete(
		&stKey,
		pstClan->stClan.ClanCore.ClanMem, 
		&nNum, 
		sizeof(pstClan->stClan.ClanCore.ClanMem[0]), 
		CmpClanMem))
	{
		return -1;
	}
	pstClan->stClan.ClanProf.MemNum = (unsigned short)nNum;
	pstClan->stClan.ClanCore.MemNum = (unsigned short)nNum;
	
	clan_update_officers(pstEnv, pstClan);
	++pstClan->stZoneClanInfo.iListVersion;

	pstMemPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
	                             pstRoleID->RoleName);
	if (pstMemPlayer)
	{
		ZoneClan* pstCurr = player_get_clan(pstEnv,pstMemPlayer);
		if (pstCurr != pstClan)
		{
			return -1;
		}
		
		stKey.LastOnline = pstEnv->pstAppCtx->stCurr.tv_sec;

		clan_mem_on_exit(pstEnv, pstClan, pstMemPlayer, 0);
		--pstClan->stZoneClanInfo.iOnlineCount;

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstMemPlayer, SYS_TIP, 
		               ZONEERR_CLAN24);

		player_clan_color_info(pstEnv, pstMemPlayer);
	}

	player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD232,
	                          pstPlayer->stRoleData.RoleName,
	                          pstRoleID->RoleName);
	player_clan_mem_chg_broadcast(pstEnv, pstClan, &stKey, CLAN_MEMBER_CHG_DEL);

	z_role_clan_oplog(pstEnv, stKey.RoleMember.RoleName, clan_id_warp(&pstClan->stClan),
	                  OP_CLAN_EXIT,NULL);
	
	return 0;
}


int player_clan_exit(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	CLANMEM stKey;
	CLANMEM *pstMem = NULL;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	size_t nNum = !pstZoneClan ? 0 : pstZoneClan->stClan.ClanProf.MemNum;

	if (!pstZoneClan)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN229);
		return -1;
	}

	if (player_clan_position(pstEnv, pstPlayer) == CLAN_POS_PRESIDENT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
		               ZONEERR_CLAN25);
		return -1;
	}

	if (clan_check_permission(pstEnv, pstPlayer, CLAN_OP_EXIT_CLAN))
	{
		return -1;
	}

	//脚本中退公会
	if (clan_check_safelock(pstEnv, pstPlayer,CLAN_OP_EXIT_CLAN) < 0)
	{
		return -1;
	}

	pstMem = clan_id_mem(pstZoneClan, pstPlayer->stRoleData.RoleID, 
	                     &pstPlayer->stOnline.iClanMemIdx);
	if (!pstMem)
	{
		return -1;
	}

	// 确保角色不在公会机械上
	player_clan_machine_ensure_down(pstEnv, pstPlayer);

	memcpy(&stKey, pstMem, sizeof(stKey));
	if (!my_bdelete(
		&stKey,
		pstZoneClan->stClan.ClanCore.ClanMem, 
		&nNum, 
		sizeof(pstZoneClan->stClan.ClanCore.ClanMem[0]), 
		CmpClanMem))
	{
		return -1;
	}
	pstZoneClan->stClan.ClanProf.MemNum = (unsigned short)nNum;
	pstZoneClan->stClan.ClanCore.MemNum = (unsigned short)nNum;

	stKey.LastOnline = pstEnv->pstAppCtx->stCurr.tv_sec;

	clan_update_officers(pstEnv, pstZoneClan);
	++pstZoneClan->stZoneClanInfo.iListVersion;

	// 记录主动退会的时间。
	clan_mem_on_exit(pstEnv, pstZoneClan, pstPlayer, 1);
	--pstZoneClan->stZoneClanInfo.iOnlineCount;

	player_clan_mem_chg_broadcast(pstEnv, pstZoneClan, &stKey,
	                              CLAN_MEMBER_CHG_DEL);
	player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN, ZONEERR_CLANBROAD233,
	                          pstPlayer->stRoleData.RoleName);

	player_clan_color_info(pstEnv, pstPlayer);
	
	z_role_clan_oplog(pstEnv, pstPlayer->stRoleData.RoleName, clan_id_warp(&pstZoneClan->stClan),
	                  OP_CLAN_EXIT,NULL);
	
	return 0;
}

/*
int player_clan_up_pos(ZONESVRENV *pstEnv, Player *pstPlayer,
                       const ROLEMEMBER *pstRoleID)
{
	CLANMEM *pstHost = NULL, *pstCustom = NULL;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	Player *pstOther = NULL;

	pstHost = clan_id_mem(pstClan, pstPlayer->stRoleData.RoleID, 
	                      &pstPlayer->stOnline.iClanMemIdx);
	pstCustom = clan_id_mem(pstClan, pstRoleID->RoleID, NULL);
	if (!pstHost || !pstCustom)
	{
		return -1;
	}

	if (pstHost == pstCustom)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "你不能提高自己的职位");
		return -1;
	}

	if (pstHost->Position <= pstCustom->Position)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "你没有权限提高此人的职位");
		return -1;
	}

	if (pstHost->Position - pstCustom->Position < 2)
	{
		// 官员必须比被提拔的会员高2级。
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP,
		           "此人已达到你能提升的最高职位了");
		return -1;
	}

	switch(pstCustom->Position)
	{
	case CLAN_POS_NORMAL:
		if (pstClan->stClan.ClanBrief.OfficerNum >= CLAN_MAX_OFFICER)
		{
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
			               "官员数量已经满了" );
			return -1;
		}
		break;

	case CLAN_POS_OFFICER:
		if (pstClan->stClan.ClanBrief.AssistNum>= CLAN_MAX_ASSIST)
		{
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
			               "副会长数量已经满了");
			return -1;
		}
		break;

	default:
		return -1;
		break;
	}

	++pstCustom->Position;

	pstOther = z_name_player(pstEnv->pstAppCtx, pstEnv,
	                         pstCustom->RoleMember.RoleName);
	if (pstOther)
	{
		pstOther->stRoleData.MiscInfo.ClanInfo.Position = pstCustom->Position;
	}

	clan_update_officers(pstClan);
	++pstClan->stZoneClanInfo.iListVersion;

	clan_action(pstEnv, pstClan, pstCustom);

	player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN, "%s 将 %s 提升为 %s",
	                          pstHost->RoleMember.RoleName,
	                          pstCustom->RoleMember.RoleName,
	                          aszClanLevelName[pstCustom->Position]);
	player_clan_mem_chg_broadcast(pstEnv, pstClan, pstCustom,
	                              CLAN_MEMBER_CHG_MOD);
	return 0;
}
*/


/*
int player_clan_down_pos(ZONESVRENV *pstEnv, Player *pstPlayer,
                         const ROLEMEMBER *pstRoleID)
{
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	CLANMEM *pstHost = NULL, *pstCustom = NULL;
	Player *pstOther = NULL;

	pstHost = clan_id_mem(pstClan, pstPlayer->stRoleData.RoleID,
	                      &pstPlayer->stOnline.iClanMemIdx);
	pstCustom = clan_id_mem(pstClan, pstRoleID->RoleID, NULL);
	if (!pstHost || !pstCustom)
	{
		return -1;
	}

	if (pstHost == pstCustom)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "你不能降低自己的职位");
		return -1;
	}

	if (pstCustom->Position < CLAN_POS_OFFICER)
	{
		// 被降职的人，职位也不能太低
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP,
		           "此人已达到最低职位,不能再降低了");
		return -1;
	}

	if (pstHost->Position - pstCustom->Position < 1)
	{
		// 官员必须比被降职的会员高1级。
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "你没有权限降低此人的职位");
		return -1;		
	}

	switch(pstCustom->Position)
	{
	case CLAN_POS_VICE_PRESIDENT:
		if (pstClan->stClan.ClanBrief.OfficerNum>= CLAN_MAX_OFFICER)
		{
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
			               "官员数量已经满了");
			return -1;
		}
		break;

	case CLAN_POS_OFFICER:
		break;
		
	default:
		return -1;
		break;
	}

	--pstCustom->Position;

	pstOther = z_name_player(pstEnv->pstAppCtx, pstEnv,
	                         pstCustom->RoleMember.RoleName);
	if (pstOther)
	{
		pstOther->stRoleData.MiscInfo.ClanInfo.Position = pstCustom->Position;
	}
	clan_update_officers(pstClan);
	++pstClan->stZoneClanInfo.iListVersion;

	clan_action(pstEnv, pstClan, pstCustom);

	player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN, "%s 将 %s 降职为 %s",
	                          pstHost->RoleMember.RoleName,
		                      pstCustom->RoleMember.RoleName,
		                      aszClanLevelName[pstCustom->Position]);

	player_clan_mem_chg_broadcast(pstEnv, pstClan, pstCustom,
	                              CLAN_MEMBER_CHG_MOD);
	return 0;
}
*/

static int clan_destory_clean_color_info(ZONESVRENV *pstEnv, ZoneClan *pstDestoryClan)
{
	
	int i;
	int j;
	SETCLANCOLOR *pstSet;
	ZoneClan *pstZoneClan;

	for(i = 0; i < pstEnv->pstClanObj->iClanNum; ++i)
	{
		pstZoneClan = pstEnv->pstClanObj->astZoneClan + i;
		pstSet = &pstZoneClan->stClan.ClanInner.SetClanColor;
		for (j=pstSet->Num -1; j>=0; j--)
		{
			if (pstSet->ClanColor[j].ClanID == pstDestoryClan->stClan.ClanProf.Id)
			{
				pstSet->Num --;
				if (j != pstSet->Num)
				{
					pstSet->ClanColor[j] = pstSet->ClanColor[pstSet->Num];
				}

				/*
				CSPKG stPkg;
				CSCLANCOLORNOTIFY *pstNotify = &stPkg.Body.ClanSvr.ClanSvrData.ClanColorNotify;
				stPkg.Body.ClanSvr.Operation = CLAN_SVR_COLOR_NOTIFY;
				pstNotify->Set = *pstSet;
				Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
				player_clan_broadcast(pstEnv, pstZoneClan, &stPkg, 0);
				*/
				break;
			}
		}
	}

	return 0;
}

int player_clan_color_chg_broadcast(ZONESVRENV *pstEnv,ZoneClan *pstZoneClan)
{
	CSPKG stPkg;
	CSCLANCOLORNOTIFY *pstNotify;

	pstNotify = &stPkg.Body.ClanSvr.ClanSvrData.ClanColorNotify;
	stPkg.Body.ClanSvr.Operation = CLAN_SVR_COLOR_NOTIFY;
	pstNotify->Set = pstZoneClan->stClan.ClanInner.SetClanColor;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	player_clan_broadcast(pstEnv, pstZoneClan, &stPkg, 0);
	return 0;
}

int clan_disband(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,char* pszRoleName)
{

	
	z_role_clan_oplog(pstEnv, pszRoleName, clan_id_warp(&pstZoneClan->stClan),
	                  OP_CLAN_DESTROY,NULL);

	player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
					ZONEERR_CLANBROAD234);	

	clan_destory_clean_color_info(pstEnv, pstZoneClan);

	memset(&pstZoneClan->stClan.ClanInner.SetClanColor, 0, sizeof(pstZoneClan->stClan.ClanInner.SetClanColor));
	player_clan_color_chg_broadcast(pstEnv,pstZoneClan);

	// 为了提高响应速度, 先销毁再提交数据库删除
	clan_partial_destroy(pstEnv, pstZoneClan);

	// 非延迟摧毁,部分数据已经先回收了
	pstZoneClan->stZoneClanInfo.fWillDelayDestroy = 0;
	clan_set_dirty(pstZoneClan, CACHE_FLAG_INEXIST);
	
	return 0;
}


int player_clan_disband(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	time_t tTime;
	const CLANLEVELDEF *pstLvDef = NULL;
	char szTime[256] = {0};
	int iDelayTime = 0;
	ClanBattleApplyData *pstApplyData;
			
			

	// 定时消耗公会物资
	pstLvDef = z_clan_level_def(pstEnv, 0);
	if (!pstLvDef)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_clan_level_def by level 0 fail");
		return -1;
	}

	if (!pstZoneClan)
	{
		return -1;
	}

	if (check_npc_ui(pstEnv, pstPlayer, UI_TYPE_CLAN_DISBAND) < 0)
	{
		return -1;
	}

	pstApplyData = &pstZoneClan->stZoneClanInfo.stBattleApplyData;

	if (pstApplyData->iApplyTime + CLAN_BATTLE_HB_TIMEOUT > pstEnv->pstAppCtx->stCurr.tv_sec &&
		pstApplyData->iBattleID )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
		           ZONEERR_CLAN_IN_BATTLE, szTime);
		return -1;
	}

	
	iDelayTime = pstLvDef->LevelDownTime;

	if (iDelayTime < 10)
	{
		iDelayTime = 10;
	}
	
	tTime = pstEnv->pstAppCtx->stCurr.tv_sec - pstZoneClan->stClan.ClanCore.FirstDisbandTime;
	if (tTime > 2*iDelayTime)
	{
		pstZoneClan->stClan.ClanCore.FirstDisbandTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		
		time_text_zh(iDelayTime,szTime);
		
		player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
					ZONEERR_CLANBROAD235,					
					pstPlayer->stRoleData.RoleName,szTime);
		return 0;
	}
	else if (tTime < iDelayTime)
	{
		time_text_zh(iDelayTime - tTime,szTime);
		
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
		           ZONEERR_CLAN230, szTime);
		return 0;
	}
	else
	{
		//do nothing
	}

	// 先设置解散公会时间
	pstPlayer->stRoleData.MiscInfo.ClanInfo.ExitPunishTime = 
		pstEnv->pstAppCtx->stCurr.tv_sec;
	
	clan_disband(pstEnv, pstZoneClan,pstPlayer->stRoleData.RoleName);

	return 0;
}


int player_clan_set_bulletin(ZONESVRENV *pstEnv, Player *pstPlayer,
                             const char *pszText)
{
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);

	if (!pszText)
	{
		return -1;
	}

	// 冷却时间判断
	if (pstEnv->pstAppCtx->stCurr.tv_sec -
		pstClan->stZoneClanInfo.tLastSetBulliten < g_iClanSetBullitenCDSecs)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
		           ZONEERR_CLAN231,
		           g_iClanSetBullitenCDSecs -
		           (pstEnv->pstAppCtx->stCurr.tv_sec -
		            pstClan->stZoneClanInfo.tLastSetBulliten));
		return -1;
	}

	// 老大哥正看者你。
	if (CheckDirty(pszText , 0xf) & 0xf)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
			           ZONEERR_DIRTY);
		return -1;
	}

	STRNCPY(pstClan->stClan.ClanInner.GongGao, pszText, 
		    CCH(pstClan->stClan.ClanInner.GongGao));
	clan_set_dirty(pstClan, CACHE_FLAG_DIRTY);

	pstClan->stZoneClanInfo.tLastSetBulliten =
		pstEnv->pstAppCtx->stCurr.tv_sec;
	pstClan->stClan.ClanInner.GongGaoLastWrite = 
		pstEnv->pstAppCtx->stCurr.tv_sec;

	player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN, ZONEERR_CLANBROAD236,
	                          pstPlayer->stRoleData.RoleName);
	player_clan_bulliten_broadcast(pstEnv, pstClan);
	return 0;
}
			

int player_clan_set_manifesto(ZONESVRENV *pstEnv, Player *pstPlayer,
                              const char *pszText)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);

	if (check_npc_ui(pstEnv, pstPlayer, UI_TYPE_CLAN_MANIFESTO) < 0)
	{
		return -1;
	}

	if (!pszText)
	{
		return -1;
	}

	// 老大哥正看者你。
	if (CheckDirty(pszText , 0xf) & 0xf)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
		               ZONEERR_DIRTY);
		return -1;
	}

	STRNCPY(pstZoneClan->stClan.ClanBrief.XuanGao, pszText,
		    CCH(pstZoneClan->stClan.ClanBrief.XuanGao));
	++pstZoneClan->stZoneClanInfo.iListVersion;

	clan_set_dirty(pstZoneClan, CACHE_FLAG_DIRTY);
	player_clan_view_chg(pstEnv, pstZoneClan, CLAN_VIEW_CHG_MOD);

	player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN, ZONEERR_CLANBROAD237,
	                          pstPlayer->stRoleData.RoleName);
	return -1;
}


int player_clan_demise_job(ZONESVRENV *pstEnv, Player *pstPlayer,
                           const ROLEMEMBER *pstRoleID)
{
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	CLANMEM *pstCustom = NULL;
	CLANMEM *pstHost = clan_id_mem(pstClan, pstPlayer->stRoleData.RoleID,
		                           &pstPlayer->stOnline.iClanMemIdx);
	Player * pstCustomPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
		                                     pstRoleID->RoleName);
	if (!pstHost)
	{
		// 公会中找不到这个成员
		goto fatal;
	}

	if (!pstCustomPlayer)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_CLAN27);
		goto fatal;
	}

	if (!(pstCustom = clan_id_mem(pstClan, pstCustomPlayer->stRoleData.RoleID,
	                              &pstCustomPlayer->stOnline.iClanMemIdx)))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_CLAN28);
		goto fatal;
	}

	if (clan_pos_cmp(pstHost->Position, pstCustom->Position) <= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_CLAN29);
		goto fatal;
	}

	//会长或副会长转让职位给其他人
	pstCustom->Position = pstHost->Position;
	pstCustom->GroupID = 0;
	STRNCPY(pstCustom->Title, pstHost->Title, CCH(pstCustom->Title));
	pstHost->Position = CLAN_POS_NORMAL;
	pstHost->Title[0] = 0;
	pstHost->GroupID = 0;

	player_in_strong_check(pstEnv, pstPlayer);
	player_in_strong_check(pstEnv, pstCustomPlayer);

	clan_update_officers(pstEnv, pstClan );
	++pstClan->stZoneClanInfo.iListVersion;

	clan_action(pstEnv, pstClan, pstHost);
	clan_action(pstEnv, pstClan, pstCustom);

	player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN, ZONEERR_CLANBROAD238,
	                          pstHost->RoleMember.RoleName,
	                          kClanPosName[pstCustom->Position],
	                          pstCustom->RoleMember.RoleName);

	player_clan_mem_chg_broadcast(pstEnv, pstClan, pstHost,
	                              CLAN_MEMBER_CHG_MOD);
	player_clan_mem_chg_broadcast(pstEnv, pstClan, pstCustom,
	                              CLAN_MEMBER_CHG_MOD);
	return 0;

fatal:
	return -1;
}


int player_clan_apply_join_clan(ZONESVRENV* pstEnv, Player *pstPlayer,
                                const CLANID *pstClanID)
{
	ZoneClan *pstClan = z_clanid_clan(pstEnv, pstClanID);
	CLANAPPLY stNewApply, stOldApply;
	int fOverflowed = 0;

	if (pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID != 0)
	{
		return -1;
	}
	
	if (player_clan_check_exit_punish_time(pstEnv, pstPlayer) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
		           ZONEERR_CLAN232,
		           clan_exit_remain_time_zh(pstEnv, pstPlayer));
		return -1;
	}

	if (!pstClan)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN233);
		return -1;
	}

	if (clan_check_mem_limit(pstEnv, pstClan) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
		               ZONEERR_CLAN30);
		return -1;
	}

	if (clan_id_apply(pstClan, pstPlayer->stRoleData.RoleID, NULL))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN31);
		return -1;
	}

	stNewApply.ApplyTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	stNewApply.Career = pstPlayer->stRoleData.Career;
	stNewApply.Level = pstPlayer->stRoleData.Level;
	stNewApply.RoleMember.RoleID = pstPlayer->stRoleData.RoleID;
	STRNCPY(stNewApply.RoleMember.RoleName, pstPlayer->stRoleData.RoleName,
	        CCH(stNewApply.RoleMember.RoleName));

	pstClan->stClan.ClanInner.ClanApplyNum =
		array_circle_push_back_overflow(
			pstClan->stClan.ClanInner.ClanApplys,
			sizeof(pstClan->stClan.ClanInner.ClanApplys[0]),
			pstClan->stClan.ClanInner.ClanApplyNum,
			g_iClanMaxApplyNum,
			&stNewApply,
			&stOldApply,
			&fOverflowed);
	clan_set_dirty_notify(pstClan, CACHE_FLAG_DIRTY);

	// 通知最早加入申请的玩家
	if (fOverflowed)
	{
		Player *pstOldPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
		                                     stOldApply.RoleMember.RoleName);
		if (pstOldPlayer && !player_get_clan(pstEnv, pstOldPlayer))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOldPlayer, SYS_MIDDLE2,
			           ZONEERR_CLAN234,
			           pstClan->stClan.ClanProf.Name);
		}
	}

	// 通知一下能入会审查的公会人员
	if (pstClan->stClan.ClanInner.ClanApplyNum > 0)
	{
		CSPKG stPkg;
		CSPKG *pstPkg = &stPkg;
		CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;

		pstSysMsg->Type = SYS_TIP;
		pstSysMsg->Times = 0;
		pstSysMsg->Interval = 0;
		pstSysMsg->LinkLen = 0;
		STRNCPY(pstSysMsg->Msg, LJY_SYSMSG3, CCH(pstSysMsg->Msg));

		Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
		clan_broadcast_if(pstEnv, pstClan, pstPkg, ClanBodyCanCensorJoinClan);

		// 使其图标闪烁
		pstPkg->Body.ClanSvr.Operation = CLAN_SVR_NOTIFY;
		pstPkg->Body.ClanSvr.ClanSvrData.ClanNotify.Code = CLAN_NOTIFY_MEMBER;

		Z_CSHEAD_INIT(&pstPkg->Head, CLAN_SVR);
		clan_broadcast_if(pstEnv, pstClan, pstPkg, ClanBodyCanCensorJoinClan);
	}

	z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2,
	           ZONEERR_CLAN235,
	           pstClan->stClan.ClanProf.Name);
	return 0;
}


int player_clan_censor_join_clan(ZONESVRENV *pstEnv, Player *pstPlayer, 
                                 const CSCLANCENSORROLE *pstClanCensor)
{
	size_t nNum;
	Player *pstCustomPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, 
		                                    pstClanCensor->ID.RoleName);
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);

	if (!pstCustomPlayer)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN236,
		           pstClanCensor->ID.RoleName);
		goto fatal;
	}

	if (player_get_clan(pstEnv, pstCustomPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN237,
		           pstCustomPlayer->stRoleData.RoleName);
		goto fatal;
	}

	// 退会冷却时间检查
	if (player_clan_check_exit_punish_time(pstEnv, pstCustomPlayer) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstCustomPlayer, SYS_MIDDLE2,
		           ZONEERR_CLAN238,
		           clan_exit_remain_time_zh(pstEnv, pstPlayer));

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
		           ZONEERR_CLAN239,
		           pstCustomPlayer->stRoleData.RoleName,
		           clan_exit_remain_time_zh(pstEnv, pstCustomPlayer));
		goto fatal;
	}

	// Lazy Updating :
	clan_update_apply(pstEnv, pstClan, pstPlayer);

	nNum = array_remove_if(
		pstClan->stClan.ClanInner.ClanApplys,
		sizeof(pstClan->stClan.ClanInner.ClanApplys[0]),
		pstClan->stClan.ClanInner.ClanApplyNum,
		(void*)&pstClanCensor->ID,
		RoleMemberEquals);
	if (nNum == pstClan->stClan.ClanInner.ClanApplyNum)
	{
		// 申请列表中没有指定的玩家信息。
		return -1;
	}
	pstClan->stClan.ClanInner.ClanApplyNum = (unsigned char)nNum;

	player_clan_apply_join_clan_list(pstEnv, pstPlayer);

	if (pstClanCensor->Vote == 0)
	{
		// 玩家被否决。
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstCustomPlayer, SYS_TIP,
		           ZONEERR_CLAN32, pstPlayer->stRoleData.RoleName);
		return 0;
	}

	if (player_clan_join_clan(pstEnv, pstCustomPlayer, pstClan) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
		               ZONEERR_CLAN30);
		return -1;
	}

	return 0;

fatal:
	// Lazy Updating :
	clan_update_apply(pstEnv, pstClan, NULL);
	return -1;
}


int player_clan_set_nickname(ZONESVRENV *pstEnv, Player *pstPlayer, 
                             const CSCLANMEMBERNICKNAME *pstNickname)
{
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	CLANMEM *pstMem = NULL;

	if (!pstNickname->Nickname[0] || !pstNickname->RoleID)
	{
		return -1;
	}

	// 老大哥正看者你。
	if ((CheckDirty(pstNickname->Nickname, 0x1f) & 0x1f))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
		               ZONEERR_DIRTY);
		return -1;
	}

	pstMem = clan_id_mem(pstClan, pstNickname->RoleID, NULL);
	if (!pstMem)
	{
		return -1;
	}

	STRNCPY(pstMem->NickName, pstNickname->Nickname, CCH(pstMem->NickName));
	clan_set_dirty(pstClan, CACHE_FLAG_DIRTY);
	++pstClan->stZoneClanInfo.iListVersion;

	player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD239,
	                          pstPlayer->stRoleData.RoleName,
	                          pstMem->RoleMember.RoleName);
	player_clan_mem_chg_broadcast(pstEnv, pstClan, pstMem, CLAN_MEMBER_CHG_MOD);
	return 0;
}


int player_clan_apply_join_clan_list(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSCLANAPPLYJOINLIST *pstApplyList
		= &stPkg.Body.ClanSvr.ClanSvrData.ClanApplyJoinList;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	
	// Lazy Updating :
	clan_update_apply(pstEnv, pstClan, pstPlayer);

	pstApplyList->ClanID = *clan_id_warp(&pstClan->stClan);
	pstApplyList->RoleNum = pstClan->stClan.ClanInner.ClanApplyNum;
	memcpy(pstApplyList->ApplyRoles, pstClan->stClan.ClanInner.ClanApplys,
	       sizeof(pstApplyList->ApplyRoles[0]) * pstApplyList->RoleNum);

	// 发送请求表。
	stPkg.Body.ClanSvr.Operation = CLAN_SVR_APPLY_JOIN_LIST;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int player_clan_invite_join_union(ZONESVRENV *pstEnv, Player *pstPlayer,
                                  const CLANID *pstClanID)
{
	CSPKG stPkg;
	CSCLANASK *pstAsk = &stPkg.Body.ClanSvr.ClanSvrData.ClanAskJoinUnion;
	Player *pstReceiver = NULL;
	ZoneClan *pstMyClan = player_get_clan(pstEnv, pstPlayer);
	ZoneClan *pstOtherClan = z_clanid_clan(pstEnv, pstClanID);
	ZoneClanLive *pstClanLive = &pstOtherClan->stZoneClanInfo.stInviteLive;

	if (!pstOtherClan)
	{
		return -1;
	}

	// Lazy Updating :
	clan_update_diplomatism(pstEnv, pstMyClan, NULL);
	clan_update_diplomatism(pstEnv, pstOtherClan, NULL);

	if (!clan_check_can_relations(pstEnv, pstPlayer, pstMyClan, pstOtherClan,
	                              0))
	{
		return -1;
	}

	if (pstMyClan->stClan.ClanInner.ClanFriendNum >= CLAN_MAX_FRIEND ||
		pstOtherClan->stClan.ClanInner.ClanFriendNum >= CLAN_MAX_FRIEND)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN240);
		return -1;
	}

	pstReceiver = clan_get_active_manager(pstEnv, pstOtherClan);
	if (!pstReceiver)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN241);
		return -1;
	}

	if (pstClanLive->stOtherSideClan.GID != 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN242);
		return -1;
	}

	// 询问对方是否加入同盟。
	pstAsk->ID = *clan_id_warp(&pstOtherClan->stClan);
	STRNCPY(pstAsk->HostName, pstPlayer->stRoleData.RoleName,
	        CCH(pstAsk->HostName));

	stPkg.Body.ClanSvr.Operation = CLAN_SVR_ASK_JOIN_UNION;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	if (z_cltmsg_send(pstEnv, pstReceiver, &stPkg) < 0)
	{
		return -1;
	}

	// 设置被邀请公会的信息。
	clan_set_live(pstEnv, pstClanLive, pstMyClan);
	return 0;
}


int player_clan_apply_join_union(ZONESVRENV* pstEnv, Player *pstPlayer,
                                 const CLANID *pstClanID)
{
	ZoneClan *pstOtherClan = z_clanid_clan(pstEnv, pstClanID);
	ZoneClan *pstMyClan = player_get_clan(pstEnv, pstPlayer);
	CLANAPPLYRELATION stNewApply;

	if (!pstMyClan)
	{
		return -1;
	}

	if (!pstOtherClan)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN243);
		return -1;
	}

	if (!clan_check_can_relations(pstEnv, pstPlayer, pstMyClan, pstOtherClan,
	                              0))
	{
		return -1;
	}

	// Lazy Updating :
	clan_update_diplomatism(pstEnv, pstOtherClan, NULL);
	clan_update_diplomatism(pstEnv, pstMyClan, NULL);

	if (pstOtherClan->stClan.ClanInner.ClanFriendNum >= CLAN_MAX_FRIEND)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN244,
		           pstOtherClan->stClan.ClanProf.Name);
		return -1;
	}

	if (pstMyClan->stClan.ClanInner.ClanFriendNum >= CLAN_MAX_FRIEND)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN244,
		           pstMyClan->stClan.ClanProf.Name);
		return -1;
	}

	stNewApply.ClanId = *clan_id_warp(&pstMyClan->stClan);
	stNewApply.ApplyTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	stNewApply.Leader = pstMyClan->stClan.ClanBrief.Leader;

	pstOtherClan->stClan.ClanInner.ClanApplyFriendNum = 
		array_circle_push_back(
			pstOtherClan->stClan.ClanInner.ClanApplyFriends,
			sizeof(pstOtherClan->stClan.ClanInner.ClanApplyFriends[0]),
			pstOtherClan->stClan.ClanInner.ClanApplyFriendNum,
			CLAN_MAX_APPLY_FRIEND,
			&stNewApply);
	clan_set_dirty(pstOtherClan, CACHE_FLAG_DIRTY);

	// 通知一下能审查结盟的公会人员
	{
		CSPKG stPkg;
		CSPKG *pstPkg = &stPkg;
		CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;

		pstSysMsg->Type = SYS_TIP;
		pstSysMsg->Times = 0;
		pstSysMsg->Interval = 0;
		pstSysMsg->LinkLen = 0;
		STRNCPY(pstSysMsg->Msg, LJY_SYSMSG4, CCH(pstSysMsg->Msg));

		Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
		clan_broadcast_if(pstEnv, pstOtherClan, pstPkg, ClanBodyCanDiplomatism);

		// 使其图标闪烁
		pstPkg->Body.ClanSvr.Operation = CLAN_SVR_NOTIFY;
		pstPkg->Body.ClanSvr.ClanSvrData.ClanNotify.Code = 
			CLAN_NOTIFY_DIPLOMATISM;

		Z_CSHEAD_INIT(&pstPkg->Head, CLAN_SVR);
		clan_broadcast_if(pstEnv, pstOtherClan, pstPkg, ClanBodyCanDiplomatism);
	}

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN245,
	           pstOtherClan->stClan.ClanProf.Name);
	return 0;
}

int clan_check_enemy_cd(ZONESVRENV *pstEnv,ZoneClan *pstClan,ZoneClan *pstOtherClan, Player *pstPlayer)
{
	CLANENEMYCD* pstCD = &pstClan->stClan.ClanInner.ClanEnemyCD;
	int i = 0;

	//从前面搜索	
	for (i = 0; i < pstCD->CDNum; i++)
	{
		if ((int)pstCD->EnemyCD[i].EndTime < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			break;//后面的都过了cd时间,不用找了
		}
		if (pstCD->EnemyCD[i].ClanID == pstOtherClan->stClan.ClanProf.Id)
		{
			if (pstPlayer)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN246);
			}
			return -1;//cd 中
		}
	}
	
	return 0;
}

int clan_set_enemy_cd(ZONESVRENV *pstEnv,ZoneClan *pstClan,ZoneClan *pstOtherClan,int iCD)
{
	CLANENEMYCD* pstCD = &pstClan->stClan.ClanInner.ClanEnemyCD;
	int i = 0;
	int iPos = -1;
	int iNum = 0;

	//搜索	
	for (i = 0; i < pstCD->CDNum; i++)
	{
		if (pstCD->EnemyCD[i].ClanID == pstOtherClan->stClan.ClanProf.Id)
		{
			iPos = i;
		}
	}

	//挪一挪
	if (iPos > -1 && iPos < pstCD->CDNum)
	{
		if (iPos > 0 )
		{
			iNum = min(iPos,MAX_CLAN_ENEMY_CD_NUM - 1);
		}
	}	
	else
	{
		iNum = min(MAX_CLAN_ENEMY_CD_NUM - 1,pstCD->CDNum);
		pstCD->CDNum++;
	}

	if (iNum > 0)
	{
		memmove(&pstCD->EnemyCD[1],&pstCD->EnemyCD[0],iNum * sizeof(pstCD->EnemyCD[0]));		
	}	
	
	if (pstCD->CDNum > MAX_CLAN_ENEMY_CD_NUM)
	{
		pstCD->CDNum = MAX_CLAN_ENEMY_CD_NUM;
	}
		


	pstCD->EnemyCD[0].ClanID = pstOtherClan->stClan.ClanProf.Id;
	pstCD->EnemyCD[0].EndTime = pstEnv->pstAppCtx->stCurr.tv_sec + iCD;
	
	return 0;
}

int clan_update_enemy_cd(ZONESVRENV *pstEnv,ZoneClan *pstClan)
{
	CLANENEMYCD* pstCD = &pstClan->stClan.ClanInner.ClanEnemyCD;
	int i = 0;

	//搜索	
	for (i = pstCD->CDNum - 1; i >= 0; i--)
	{
		if ((int)pstCD->EnemyCD[i].EndTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			pstCD->EnemyCD[i].EndTime = 0;
			pstCD->EnemyCD[i].ClanID = 0;
			pstCD->CDNum--;
		}
		else
		{
			break;
		}
	}
	
	return 0;
}



int player_clan_apply_join_enemy(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 const CLANID *pstClanID)
{
	ZoneClan *pstOtherClan = z_clanid_clan(pstEnv, pstClanID);
	ZoneClan *pstMyClan = player_get_clan(pstEnv, pstPlayer);

	if (!pstMyClan)
	{
		return -1;
	}

	if (package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY,
	                           CLAN_WAR_COST) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_SVR,
		           ZONEERR_CLAN247,
		           CLAN_WAR_COST);
		return -1;
	}	

	if (!pstOtherClan)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN248);
		return -1;
	}

	if (!clan_check_can_relations(pstEnv, pstPlayer, pstMyClan, pstOtherClan,
	    1))
	{
		return -1;
	}

	//宣战cd检查
	if (clan_check_enemy_cd(pstEnv,pstMyClan,pstOtherClan,pstPlayer) < 0)
	{
		return -1;
	}
	
	// Lazy Updating :
	clan_update_diplomatism(pstEnv, pstMyClan, NULL);

	if (pstMyClan->stClan.ClanInner.ClanEnemyNum >= CLAN_MAX_ENEMY)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN249,
		           pstMyClan->stClan.ClanProf.Name);
		return -1;
	}

	clan_update_diplomatism(pstEnv, pstOtherClan, NULL);
	if (pstOtherClan->stClan.ClanInner.ClanEnemyNum >= CLAN_MAX_ENEMY)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN249,
		           pstOtherClan->stClan.ClanProf.Name);
		return -1;
	}

	package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, CLAN_WAR_COST);
	z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_CLAN_ENEMY, 0, 0, CLAN_WAR_COST, "公会宣战");
	
	return player_clan_join_enemy(pstEnv, pstPlayer, pstMyClan, pstOtherClan);
}


int player_clan_exit_union(ZONESVRENV *pstEnv, Player *pstPlayer,
                           const CLANID *pstClanID)
{
	ZoneClan *pstMyClan = player_get_clan(pstEnv, pstPlayer);
	ZoneClan *pstOtherSideClan = z_clanid_clan(pstEnv, pstClanID);
	unsigned int nNum;

	if (!pstOtherSideClan)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN248);
		return -1;
	}

	if (pstMyClan == pstOtherSideClan)
	{
		return -1;
	}

	if (!clan_clanid_diplomatism(pstEnv, pstMyClan, CLAN_IN_FRIENDS, pstClanID,
	                             NULL))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN250);
		return -1;
	}

	// Lazy updating : 
	clan_update_diplomatism(pstEnv, pstMyClan, NULL);
	clan_update_diplomatism(pstEnv, pstOtherSideClan, NULL);


	// 互相删除对方。
	nNum = array_remove_if(
		pstMyClan->stClan.ClanInner.ClanFriends,
		sizeof(pstMyClan->stClan.ClanInner.ClanFriends[0]),
		pstMyClan->stClan.ClanInner.ClanFriendNum,
		(void *)clan_id_warp(&pstOtherSideClan->stClan),
		ClanIdEquals);
	pstMyClan->stClan.ClanInner.ClanFriendNum = nNum;
	clan_set_dirty(pstMyClan, CACHE_FLAG_DIRTY);

	player_clan_diplomatism_chg_broadcast(pstEnv, pstMyClan);
	player_clan_msg_broadcast(pstEnv, pstMyClan, SYS_CLAN, ZONEERR_CLANBROAD240,
	                          pstOtherSideClan->stClan.ClanProf.Name);

	nNum = array_remove_if(
		pstOtherSideClan->stClan.ClanInner.ClanFriends,
		sizeof(pstOtherSideClan->stClan.ClanInner.ClanFriends[0]),
		pstOtherSideClan->stClan.ClanInner.ClanFriendNum,
		(void *)clan_id_warp( &pstMyClan->stClan ),
		ClanIdEquals );
	pstOtherSideClan->stClan.ClanInner.ClanFriendNum = nNum;
	clan_set_dirty(pstOtherSideClan, CACHE_FLAG_DIRTY);

	player_clan_diplomatism_chg_broadcast(pstEnv, pstOtherSideClan);
	player_clan_msg_broadcast(pstEnv, pstOtherSideClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD241,
	                          pstMyClan->stClan.ClanProf.Name);
	return 0;
}


/*
int player_clan_exit_enemy(ZONESVRENV *pstEnv, Player *pstPlayer,
                           const CLANID *pstClanID)
{
	CSPKG stPkg;
	CSCLANASK *pstAsk = &stPkg.Body.ClanSvr.ClanSvrData.ClanAskJoinUnion;
	Player *pstReceiver = NULL;
	CLANRELATION *pstOtherRelation = NULL;
	ZoneClan *pstMyClan = player_get_clan(pstEnv, pstPlayer);
	ZoneClan *pstOtherClan = z_clanid_clan(pstEnv, pstClanID);

	if (!pstOtherClan)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "指定的公会已经不存在");
		return -1;
	}

	if (pstMyClan == pstOtherClan)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "不能和自己的公会取消敌对");
		return -1;
	}

	// Lazy Updating :
	clan_update_diplomatism(pstEnv, pstMyClan, NULL);
	clan_update_diplomatism(pstEnv, pstOtherClan, NULL);

	pstOtherRelation = clan_clanid_diplomatism(pstEnv, pstMyClan,
	                                           CLAN_IN_ENEMYS, pstClanID, NULL);
	if (!pstOtherRelation)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "指定的公会不在敌对列表中");
		return -1;
	}

	if (pstEnv->pstAppCtx->stCurr.tv_sec - pstOtherRelation->Time <
		CLAN_EXIT_ENEMY_CD)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2,
		           "24小时内不能解除与此公会的敌对状态");
		return -1;
	}

	pstReceiver = clan_get_active_manager(pstEnv, pstOtherClan);
	if (!pstReceiver)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "对方公会无人处理此事务");
		return -1;
	}

	if (pstOtherClan->stZoneClanInfo.stExitEnemyLive.stOtherSideClan.GID != 0)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "对方公会忙");
		return -1;
	}

	// 询问对方是否解除敌对。
	pstAsk->ID = *clan_id_warp(&pstMyClan->stClan);
	STRNCPY(pstAsk->HostName, pstPlayer->stRoleData.RoleName,
	        CCH(pstAsk->HostName));

	stPkg.Body.ClanSvr.Operation = CLAN_SVR_ASK_EXIT_ENEMY;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	if (z_cltmsg_send(pstEnv, pstReceiver, &stPkg) < 0)
	{
		return -1;
	}

	// 设置被邀请公会的信息。
	clan_set_live(pstEnv, &pstOtherClan->stZoneClanInfo.stExitEnemyLive,
	              pstMyClan);
	return 0;
}
*/

/*
int player_clan_answer_exit_enemy(ZONESVRENV *pstEnv, Player *pstPlayer,
                                  const CSCLANANSWER *pstAnswer)
{
	ZoneClan *pstMyClan = player_get_clan(pstEnv, pstPlayer);
	ZoneClanLive *pstClanLive = &pstMyClan->stZoneClanInfo.stExitEnemyLive;
	ZoneClan *pstOtherClan = z_clanid_clan(pstEnv, &pstAnswer->ID);

	if (!pstOtherClan)
	{
		return -1;
	}

	if (pstClanLive->stOtherSideClan.GID == 0)
	{
		return -1;
	}

	if (pstClanLive->stOtherSideClan.GID != pstAnswer->ID.GID)
	{
		return -1;
	}

	switch(pstAnswer->ACK)
	{
	case 0: // 拒绝
	case 2: // 无响应
		{
			Player *pstHostPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
			                                      pstAnswer->HostName);
			if (pstHostPlayer)
			{
				z_sys_strf(pstEnv, pstHostPlayer, SYS_TIP,
				           pstAnswer->ACK == 0 ? "%s 拒绝了你的解除敌对要求"
				                               : "%s 无法响应你的解除敌对要求",
				           pstPlayer->stRoleData.RoleName);
			}
		}
		break;

	case 1: // 同意
		{
			size_t nNum = 0;

			// 互相删除对方。
			nNum = array_remove_if(
				pstMyClan->stClan.ClanInner.ClanEnemys,
				sizeof(pstMyClan->stClan.ClanInner.ClanEnemys[0]),
				pstMyClan->stClan.ClanInner.ClanEnemyNum,
				(void *)clan_id_warp(&pstOtherClan->stClan),
				ClanIdEquals);
			pstMyClan->stClan.ClanInner.ClanEnemyNum = nNum;
			clan_set_dirty(pstMyClan, CACHE_FLAG_DIRTY);

			player_clan_diplomatism_chg_broadcast(pstEnv, pstMyClan);
			player_clan_msg_broadcast(pstEnv, pstMyClan, SYS_CLAN,
			                          "公会 %s 撤销了敌对状态",
			                          pstOtherClan->stClan.ClanProf.Name);

			nNum = array_remove_if(
				pstOtherClan->stClan.ClanInner.ClanEnemys,
				sizeof(pstOtherClan->stClan.ClanInner.ClanEnemys[0]),
				pstOtherClan->stClan.ClanInner.ClanEnemyNum,
				(void *)clan_id_warp(&pstMyClan->stClan),
				ClanIdEquals);
			pstOtherClan->stClan.ClanInner.ClanEnemyNum = nNum;
			clan_set_dirty(pstOtherClan, CACHE_FLAG_DIRTY);

			player_clan_diplomatism_chg_broadcast(pstEnv, pstOtherClan);
			player_clan_msg_broadcast(pstEnv, pstOtherClan, SYS_CLAN,
			                          "公会 %s 撤销了敌对状态",
			                          pstMyClan->stClan.ClanProf.Name);
		}
		break;

	default:
		break;
	}

	bzero(pstClanLive, sizeof(*pstClanLive));
	return 0;
}
*/

int player_clan_diplomatism_status(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i = 0;
	CSPKG stPkg;
	CSCLANDIPLOMATISMSTATUS *pstStatus
		= &stPkg.Body.ClanSvr.ClanSvrData.ClanDiplomatismStatus;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer), *pstOther = NULL;
	CLANINNER *pstClanInner = &pstClan->stClan.ClanInner;

	memset(pstStatus,0,sizeof(*pstStatus));


	// Lazy updating : 
	clan_update_diplomatism(pstEnv, pstClan, pstPlayer);

 	pstStatus->ID = *clan_id_warp(&pstClan->stClan);

	pstStatus->EnemyNum = pstClanInner->ClanEnemyNum;
	i = pstStatus->EnemyNum;
	while(i--)
	{
		pstStatus->Enemys[i].ClanID = pstClanInner->ClanEnemys[i].ClanId;
		pstOther = z_clanid_clan(pstEnv, &pstStatus->Enemys[i].ClanID);
		if (pstOther)
		{
			STRNCPY(pstStatus->Enemys[i].Leader,
			        pstOther->stClan.ClanBrief.Leader.RoleName,
			        CCH(pstStatus->Enemys[i].Leader));
		}
	}

	pstStatus->FriendNum = pstClanInner->ClanFriendNum;
	i = pstStatus->FriendNum;
	while(i--)
	{
		pstStatus->Friends[i].ClanID = pstClanInner->ClanFriends[i].ClanId;
		pstOther = z_clanid_clan(pstEnv, &pstStatus->Friends[i].ClanID);
		if (pstOther)
		{
			STRNCPY(pstStatus->Friends[i].Leader,
			        pstOther->stClan.ClanBrief.Leader.RoleName,
			        CCH(pstStatus->Friends[i].Leader));
		}
	}

	pstStatus->ApplyFriendNum = pstClanInner->ClanApplyFriendNum;
	i = pstStatus->ApplyFriendNum;
	while(i--)
	{
		pstStatus->ApplyFriends[i].ClanID = 
			pstClanInner->ClanApplyFriends[i].ClanId;
		pstStatus->ApplyFriends[i].ApplyTime =
			pstClanInner->ClanApplyFriends[i].ApplyTime;
		STRNCPY(pstStatus->ApplyFriends[i].Leader,
		        pstClanInner->ClanApplyFriends[i].Leader.RoleName,
		        CCH(pstStatus->ApplyFriends[i].Leader));
	}

	pstStatus->ApplyEnemyNum = pstClanInner->ClanApplyEnemyNum;
	i = pstStatus->ApplyEnemyNum;
	while(i--)
	{
		pstStatus->ApplyEnemys[i].ClanID =
			pstClanInner->ClanApplyEnemys[i].ClanId;
		pstStatus->ApplyEnemys[i].ApplyTime =
			pstClanInner->ClanApplyEnemys[i].ApplyTime;
		STRNCPY(pstStatus->ApplyEnemys[i].Leader,
		        pstClanInner->ClanApplyEnemys[i].Leader.RoleName,
		        CCH(pstStatus->ApplyEnemys[i].Leader));
	}

	// 发送外交状况信息。
	stPkg.Body.ClanSvr.Operation = CLAN_SVR_DIPLOMATISM_STATUS;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int player_clan_answer_join_clan(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 const CSCLANANSWER *pstAnswer)
{
	ClanOnline *pstClanOnline = &pstPlayer->stOnline.stClanOnline;
	ZoneClan *pstZoneClan = z_clanid_clan(pstEnv, &pstAnswer->ID);

	if (!pstZoneClan)
	{
		return -1;
	}

	if (pstClanOnline->stClanID.GID == 0)
	{
		return -1;
	}

	if (pstClanOnline->stClanID.GID != pstAnswer->ID.GID)
	{
		return -1;
	}

	switch(pstAnswer->ACK)
	{
	case 0: // 拒绝
	case 2: // 无响应
		{
			Player *pstHostPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
			                                      pstAnswer->HostName);
			if (pstHostPlayer &&
				pstHostPlayer->stRoleData.RoleID == pstClanOnline->ullRoleID)
			{
				if (pstAnswer->ACK == 0)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstHostPlayer,
										SYS_TIP, ZONEERR_CLAN251, pstPlayer->stRoleData.RoleName);
				}
				else
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstHostPlayer,
										SYS_TIP, ZONEERR_CLAN252, pstPlayer->stRoleData.RoleName);
				}
			}
		}
		break;

	case 1: // 同意
		player_clan_join_clan(pstEnv, pstPlayer, pstZoneClan);
		break;

	default:
		break;
	}

	bzero(pstClanOnline, sizeof(*pstClanOnline));
	return 0;
}


int player_clan_answer_join_union(ZONESVRENV *pstEnv, Player *pstPlayer,
                                  const CSCLANANSWER *pstAnswer)
{
	ZoneClan *pstMyClan = player_get_clan(pstEnv, pstPlayer);
	ZoneClan *pstOtherClan = z_clanid_clan(pstEnv, &pstAnswer->ID);
	ZoneClanLive *pstClanLive = &pstMyClan->stZoneClanInfo.stInviteLive;

	if (!pstOtherClan || pstClanLive->stOtherSideClan.GID == 0)
	{
		// 根本没有接收到邀请
		return -1;
	}

	// Lazy Updating :
	clan_update_diplomatism(pstEnv, pstMyClan, NULL);
	clan_update_diplomatism(pstEnv, pstOtherClan, NULL);

	switch(pstAnswer->ACK)
	{
	case 0: // 拒绝
	case 2: // 无响应
		{
			Player *pstHostPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
			                                      pstAnswer->HostName);
			if (pstHostPlayer)
			{
				if (pstAnswer->ACK == 0)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstHostPlayer, 
										SYS_TIP, ZONEERR_CLAN253, pstHostPlayer->stRoleData.RoleName);
				}
				else
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstHostPlayer, 
										SYS_TIP, ZONEERR_CLAN254, pstHostPlayer->stRoleData.RoleName);
				}
			}
		}
		break;

	case 1: // 同意
		if (!clan_check_can_relations(pstEnv, pstPlayer, pstMyClan,
		                              pstOtherClan, 0))
		{
			return -1;
		}
		player_clan_join_union(pstEnv, pstPlayer, pstOtherClan, pstMyClan);
		break;

	default:
		break;
	}

	bzero(pstClanLive, sizeof(*pstClanLive));
	return 0;
}


int player_clan_search(ZONESVRENV *pstEnv, Player *pstPlayer,
                       unsigned int nKeyID)
{
	ClanObj *pstClanObj = pstEnv->pstClanObj;
	CSPKG stPkg;
	CSCLANVIEWPAGE *pstPage = &stPkg.Body.ClanSvr.ClanSvrData.ClanViewPage;
	ZoneClan *pstClan = NULL;

	if (check_npc_ui(pstEnv, pstPlayer, UI_TYPE_CLAN_SEARCH ) < 0)
	{
		return -1;
	}

	pstClan = z_id_clan(pstEnv, nKeyID);
	if (pstClan != NULL)
	{
		// 找到了公会
		pstPage->TotalClanNum = pstClanObj->iClanNum;
		pstPage->Cookie.PageNo = 0xFFFF; // 幻数,通知客户端,这是查询结果.
		pstPage->Cookie.ListVersion = pstClanObj->stClanObjInfo.iListVersion;
		pstPage->Cookie.RowNum = 1;
		
		clan_fill_view_row(pstPage, 0, 0, pstClan);
	}
	else
	{
		// 没有找到公会
		int iEnd = pstClanObj->iClanNum < CLAN_MAX_VIEW_PAGE_ROW ? 
			       pstClanObj->iClanNum : CLAN_MAX_VIEW_PAGE_ROW;
		CSCLANVIEWPAGECOOKIE stForceCookie;

		pstPage->TotalClanNum = pstClanObj->iClanNum;

		// cookie 设置成0，强制更新公会信息
		memset(&stForceCookie, 0, sizeof(stForceCookie));
		clan_fill_view_row_range(pstPage, 0, iEnd, &stForceCookie, pstClanObj);
	}

	stPkg.Body.ClanSvr.Operation = CLAN_SVR_CLAN_VIEW_PAGE;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int player_clan_find_req(ZONESVRENV *pstEnv, Player *pstPlayer,
                         const CSCLANFINDREQ *pstReq)
{
	CSPKG stPkg;
	CSCLANFINDRES *pstRes = &stPkg.Body.ClanSvr.ClanSvrData.ClanFindRes;
	const ClanObj *pstClanObj = pstEnv->pstClanObj;

	if (check_npc_ui(pstEnv, pstPlayer, UI_TYPE_CLAN_SEARCH ) < 0)
	{
		return -1;
	}

	switch(pstReq->ByWhat)
	{
	case FIND_CLAN_BY_ID:
		{
			const ZoneClan *pstFound = z_id_clan(pstEnv, pstReq->Key.ID);
			if (pstFound)
			{
				pstRes->Num = 1;
				pstRes->HasRemain = 0;
				pstRes->ClanView[0].BriefInfo = pstFound->stClan.ClanBrief;
				pstRes->ClanView[0].ProfInfo = pstFound->stClan.ClanProf;
				clan_set_city_prof(&pstFound->stClan.ClanCity,
				                   &pstRes->ClanView[0].CityInfo);
			}
			else
			{
				pstRes->Num = 0;
				pstRes->HasRemain = 0;
			}
		}
		break;

	case FIND_CLAN_BY_NAME:
		{
			int i;

			pstRes->HasRemain = 0;
			pstRes->Num = 0;

			if (!pstReq->Key.Name[0])
			{
				goto out;
			}

			for(i = 0; i < pstClanObj->iClanNum; ++i)
			{
				const ZoneClan *pstZoneClan = pstClanObj->astZoneClan + i;
				if(!!strcasestr(pstZoneClan->stClan.ClanProf.Name,
				                pstReq->Key.Name))
				{
					if (pstRes->Num > CLAN_MAX_VIEW_PAGE_ROW)
					{
						pstRes->Num = CLAN_MAX_VIEW_PAGE_ROW;
						pstRes->HasRemain = 1;
						goto out;
					}
					pstRes->ClanView[(int)pstRes->Num].BriefInfo =
						pstZoneClan->stClan.ClanBrief;
					pstRes->ClanView[(int)pstRes->Num].ProfInfo =
						pstZoneClan->stClan.ClanProf;
					clan_set_city_prof(&pstZoneClan->stClan.ClanCity,
					              &pstRes->ClanView[(int)pstRes->Num].CityInfo);
					++pstRes->Num;
				}
			}
		}
		break;

	default:
		return -1;
		break;
	}

out:
	if (!pstRes->Num)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN248);
	}
	if (pstRes->HasRemain)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2,
		           ZONEERR_CLAN255);
	}

	stPkg.Body.ClanSvr.Operation = CLAN_SVR_FIND_RES;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int player_clan_view_page(ZONESVRENV *pstEnv, Player *pstPlayer,
                          const CSCLANVIEWPAGEBYID *pstReq)
{
	CSPKG stPkg;
	CSCLANVIEWOFPAGE *pstRes = &stPkg.Body.ClanSvr.ClanSvrData.ClanViewOfPage;
	int i = pstReq->Num;

	pstRes->Num = 0;
	while(i--)
	{
		ZoneClan *pstZoneClan = z_id_clan(pstEnv, pstReq->IDs[i]);
		if (!pstZoneClan)
		{
			continue;
		}
		clan_fill_view(pstZoneClan, pstRes->View + pstRes->Num++);
	}
	if (pstRes->Num)
	{
		stPkg.Body.ClanSvr.Operation = CLAN_SVR_VIEW_PAGE;
		Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	return 0;
}


int player_clan_censor_join_union(ZONESVRENV *pstEnv, Player *pstPlayer,
	                              CSCLANCENSORCLAN *pstCensorVote)
{
	int iRet = 0;
	ZoneClan *pstHostClan = player_get_clan(pstEnv, pstPlayer);
	ZoneClan *pstCustomClan = z_clanid_clan(pstEnv, &pstCensorVote->ID);
	Player *pstReceiver = NULL;
	size_t nNum;
	
	// Lazy Updating :
	clan_update_diplomatism(pstEnv, pstHostClan, NULL);

	// 被审查公会不能与自己的公会:是盟友，申请敌对，敌对关系
	if (clan_clanid_diplomatism(pstEnv, pstHostClan, CLAN_IN_FRIENDS, 
	                            &pstCensorVote->ID, NULL) ||
		clan_clanid_diplomatism(pstEnv, pstHostClan, CLAN_IN_APPLY_ENEMY,
	                            &pstCensorVote->ID, NULL) ||
		clan_clanid_diplomatism(pstEnv, pstHostClan, CLAN_IN_ENEMYS,
	                            &pstCensorVote->ID, NULL))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP,
		           ZONEERR_CLAN256, pstCensorVote->ID.Name);
		goto disallow;
	}

	nNum = array_remove_if(
		pstHostClan->stClan.ClanInner.ClanApplyFriends,
		sizeof(pstHostClan->stClan.ClanInner.ClanApplyFriends[0]),
		pstHostClan->stClan.ClanInner.ClanApplyFriendNum,
		&pstCensorVote->ID,
		ClanIdEquals);
	if (pstHostClan->stClan.ClanInner.ClanApplyFriendNum == nNum)
	{
		// 申请入盟列表中根本没有指定的公会
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN257,
		           pstCensorVote->ID.Name);
		goto disallow;
	}
	pstHostClan->stClan.ClanInner.ClanApplyFriendNum = (unsigned char)nNum;

	if (pstCensorVote->Vote == 0)
	{
		// 申请被否决
		goto disallow;
	}

	if (!pstCustomClan)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN258, pstCensorVote->ID.Name);
		goto disallow;
	}

	if (player_clan_join_union(pstEnv, pstPlayer, pstHostClan,
	                           pstCustomClan) < 0)
	{
		goto disallow;
	}

final:
	player_clan_diplomatism_status(pstEnv, pstPlayer);
	return iRet;

disallow:
	iRet = -1;
	pstReceiver = clan_get_active_manager(pstEnv, pstCustomClan);
	if (pstReceiver)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstReceiver, SYS_TIP,
		           ZONEERR_CLAN259,
		           pstHostClan->stClan.ClanProf.Name,
		           pstPlayer->stRoleData.RoleName);
	}
	goto final;
	return 0;
}

/*
int player_clan_censor_join_enemy(ZONESVRENV *pstEnv, Player *pstPlayer,
                                  CSCLANCENSORCLAN *pstCensorVote)
{
	int iRet = 0;
	ZoneClan *pstHostClan = player_get_clan(pstEnv, pstPlayer);
	ZoneClan *pstCustomClan = z_clanid_clan(pstEnv, &pstCensorVote->ID);
	Player *pstReceiver = NULL;
	size_t nNum;
	
	// Lazy Updating :
	clan_update_diplomatism(pstEnv, pstHostClan, NULL);

	// 被审查公会不能与自己的公会:是盟友，申请结盟，敌对关系
	if (clan_clanid_diplomatism(pstEnv, pstHostClan, CLAN_IN_FRIENDS, 
		                        &pstCensorVote->ID, NULL) ||
		clan_clanid_diplomatism(pstEnv, pstHostClan, CLAN_IN_APPLY_FRIEND, 
		                        &pstCensorVote->ID, NULL) ||
		clan_clanid_diplomatism(pstEnv, pstHostClan, CLAN_IN_ENEMYS,
		                        &pstCensorVote->ID, NULL))
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "公会\"%s\"已经是盟友/正在"
		           "申请盟友/敌对,所以无法再加入敌对列表",
		           pstCensorVote->ID.Name);
		goto disallow;
	}

	nNum = array_remove_if(
		pstHostClan->stClan.ClanInner.ClanApplyEnemys,
		sizeof(pstHostClan->stClan.ClanInner.ClanApplyEnemys[0]),
		pstHostClan->stClan.ClanInner.ClanApplyEnemyNum,
		&pstCensorVote->ID,
		ClanIdEquals);
	if (pstHostClan->stClan.ClanInner.ClanApplyEnemyNum == nNum)
	{
		// 申请敌对列表中根本没有指定的公会
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "公会\"%s\"已经不不在申请列表中",
		           pstCensorVote->ID.Name);
		goto disallow;
	}
	pstHostClan->stClan.ClanInner.ClanApplyEnemyNum = (unsigned char)nNum;

	// 申请被否决
	if (pstCensorVote->Vote == 0)
	{
		goto disallow;
	}

	if (!pstCustomClan)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "公会\"%s\"已经不存在了, 从申请"
		           "列表中移除", pstCensorVote->ID.Name);
		goto disallow;
	}

	if (player_clan_join_enemy(pstEnv, pstPlayer, pstHostClan,
	                           pstCustomClan) < 0)
	{
		goto disallow;
	}

final:
	player_clan_diplomatism_status(pstEnv, pstPlayer);
	return iRet;

disallow:
	iRet = -1;
	pstReceiver = clan_get_active_manager(pstEnv, pstCustomClan);
	if (pstReceiver)
	{
		z_sys_strf(pstEnv, pstReceiver, SYS_TIP,
		           "贵公会的敌对请求被\"%s\"公会的%s拒绝了",
		           pstHostClan->stClan.ClanProf.Name,
		           pstPlayer->stRoleData.RoleName);
	}
	goto final;
	return 0;
}
*/

int player_clan_member_snapshot(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSCLANMEMBERS *pstMems = &stPkg.Body.ClanSvr.ClanSvrData.ClanMemberSnapshot;
	ZoneClan *pstClan = NULL;
	SSCMDDATA stData;
	CLANINFO *pstClanInfo = &pstPlayer->stRoleData.MiscInfo.ClanInfo;
	
	// 只发送一次成员快照
	if (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_CLAN_SNAP)
	{
		return -1;
	}

	pstClan = player_get_clan(pstEnv, pstPlayer);
	
	pstMems->ID = *clan_id_warp(&pstClan->stClan);
	pstMems->MemNum = pstClan->stClan.ClanProf.MemNum;
	memcpy(pstMems->Members, pstClan->stClan.ClanCore.ClanMem,
	       sizeof(pstMems->Members[0]) * pstMems->MemNum);

	stPkg.Body.ClanSvr.Operation = CLAN_SVR_MEMBER_SNAPSHOT;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	
	if (0 == z_cltmsg_send(pstEnv, pstPlayer, &stPkg))
	{
		pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_CLAN_SNAP;
		
		//让mud好公会成员
		stData.ClanID = pstClanInfo->ClanId;
		z_cmd_to_mud(pstEnv, pstPlayer->stRoleData.RoleName, NULL,SS_CMD_CLAN_ONLINE_MEMBER, &stData);
	}

	return 0;
}


int player_clan_diplomatism_snapshot(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	const ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);

	clan_fill_diplomatism_snapshot(pstEnv, pstZoneClan,
	                   &stPkg.Body.ClanSvr.ClanSvrData.ClanDiplomatismSnapshot);

	stPkg.Body.ClanSvr.Operation = CLAN_SVR_DIPLOMATISM_CHG;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int player_clan_manifesto(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	char *pszManifesto = stPkg.Body.ClanSvr.ClanSvrData.ClanManifestoText;

	if (check_npc_ui(pstEnv, pstPlayer, UI_TYPE_CLAN_SEARCH) < 0)
	{
		return -1;
	}

	STRNCPY(pszManifesto, pstClan->stClan.ClanBrief.XuanGao, SHORT_DESC_LEN);

	stPkg.Body.ClanSvr.Operation = CLAN_SVR_MANIFESTO;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int player_clan_pay(ZONESVRENV *pstEnv, Player *pstPlayer, int iPayWhat,int * piGet)
{
#if 0
	ZoneClan* pstZoneClan = player_get_clan(pstEnv,pstPlayer);
	int iMoney = 0;
 	//LEVELUP* pstLevel = NULL;
	int iKeepTimes = 0;
	CLANMEM* pstMem = NULL;
	int iMoneyType = ATTR_ID_BULL;
	CLANPAYDEF* pstPayDef = NULL;

	if (piGet)
	{
		*piGet = 0;

		iKeepTimes = world_city_fight_keep_num(pstEnv);
		iKeepTimes++;

		pstPayDef = z_find_clan_pay_def(pstEnv,iKeepTimes);
		if (!pstPayDef)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN260);
			return -1;
		}
		else
		{

			*piGet = pstPayDef->Money;
		}
		return 0;
	}
	
	if (!is_world_city_master(pstEnv,pstPlayer) || !pstZoneClan)
	{
		if (!piGet)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN261);
		}
		return -1;
	}
	
	pstMem = clan_id_mem(pstZoneClan, pstPlayer->stRoleData.RoleID,
	                         &pstPlayer->stOnline.iClanMemIdx);
	if (!pstMem )
	{
		if (!piGet)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN262);
		}
		return -1;
	}
	
	if (pstMem->JoinTime + 2 * 86400 >= pstEnv->pstAppCtx->stCurr.tv_sec )
	{
		if (!piGet)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN263);
		}
		return -1;
	}
	
	switch (iPayWhat)
	{
	case CLAN_PAY_MONEY:
		if (IsSameDay(pstPlayer->stRoleData.MiscInfo.ClanInfo.PayTime[CLAN_PAY_MONEY],
			pstEnv->pstAppCtx->stCurr.tv_sec) ||
			IsSameDay(pstZoneClan->stClan.ClanCore.LastClanPayTime,
			pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			if (!piGet)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN264);
			}
			return -1;
		}

		
		/*pstLevel = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
		if (pstLevel)
		{
			iMoney = pstLevel->ContributeMoney/10.0;
		}*/

		iKeepTimes = world_city_fight_keep_num(pstEnv);
		iKeepTimes++;

		pstPayDef = z_find_clan_pay_def(pstEnv,iKeepTimes);
		if (!pstPayDef)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN265);
			return -1;
		}
		
		iMoney = pstPayDef->Money;
		iMoneyType = pstPayDef->MoneyType;

		//只获取钱数
		if (piGet)
		{
			*piGet = iMoney;
			return 0;
		}

		if (0 >  package_add_money_test(pstEnv, pstPlayer, iMoneyType, iMoney))
		{			
			return -1;
		}

		package_add_money(pstEnv, pstPlayer, iMoneyType, iMoney);
		if (iMoneyType == ATTR_ID_BULL)
		{
			z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_CLANPAY, 0,  iMoney, 0, "公会占领资金");
		}
		else if (iMoneyType == ATTR_ID_MONEY)
		{
			z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_CLANPAY, 0,  0, iMoney, "公会占领资金");
		}

		pstZoneClan->stClan.ClanCore.LastClanPayTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstPlayer->stRoleData.MiscInfo.ClanInfo.PayTime[CLAN_PAY_MONEY] = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstPlayer->stRoleData.MiscInfo.ClanInfo.PayNum = max(MAX_CLAN_PAY_WHAT,pstPlayer->stRoleData.MiscInfo.ClanInfo.PayNum );
		
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_CHAT, ZONEERR_CLAN266,
			iMoney,z_get_money_name(iMoneyType));
		
		break;
		
	case CLAN_PAY_EXP:
		return -1;
		break;
	default:
		return -1;
		break;
	}

#endif 
	return 0;
}


int player_clan_id_snapshot(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSCLANIDSNAPSHOT *pstRes = &stPkg.Body.ClanSvr.ClanSvrData.ClanIDSnapshot;
	int i = pstEnv->pstClanObj->iClanNum;

	pstRes->Num = 0;
	while(i--)
	{
		pstRes->IDs[pstRes->Num++] =
			pstEnv->pstClanObj->astZoneClan[i].stClan.ClanProf.Id;
	}

	if (pstRes->Num)
	{
		stPkg.Body.ClanSvr.Operation = CLAN_SVR_ID_SNAPSHOT;
		Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_CLANID_SNAPSHOT;
	return 0;
}


int player_clan_view_chg(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                int iWhat)
{
	CSPKG stPkg;
	CSCLANVIEWCHG *pstRes = &stPkg.Body.ClanSvr.ClanSvrData.ClanViewChg;

	switch(iWhat)
	{
	case CLAN_VIEW_CHG_ADD:
		clan_fill_view(pstZoneClan, &pstRes->Data.Added);
		break;

	case CLAN_VIEW_CHG_DEL:
		pstRes->Data.Deleted = pstZoneClan->stClan.ClanProf.Id;
		break;

	case CLAN_VIEW_CHG_MOD:
		clan_fill_view(pstZoneClan, &pstRes->Data.Modified);
		break;

	default:
		return -1;
		break;
	}

	pstRes->What = iWhat;
	stPkg.Body.ClanSvr.Operation = CLAN_SVR_VIEW_CHG;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
	return 0;
}

int player_clan_set_job_and_title(ZONESVRENV *pstEnv, Player *pstPlayer,
                                  const CSCLANSETJOBANDTITLE *pstReq)
{
	Player *pstTmpPlayer;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	CLANMEM *pstMem = clan_id_mem(pstZoneClan, pstReq->RoleID, NULL);
	const int kOwnedPos = player_clan_position(pstEnv, pstPlayer);

	if (!pstMem)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN267);
		return -1;
	}

	/*
	if (pstReq->RoleID == pstPlayer->stRoleData.RoleID)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "你不能给自己设置官衔");
		return -1;
	}
	*/

	if (pstReq->Title[0] &&
		(CheckDirty(pstReq->Title , 0x1f) & 0x1f))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN268);
		return -1;
	}

	if (pstReq->Position < CLAN_POS_NORMAL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN269);
		return -1;
	}

	if (pstReq->Position == CLAN_POS_NORMAL && pstReq->Title[0])
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_STRONG218,
		           kClanPosName[CLAN_POS_NORMAL]);
		return -1;
	}

	if (pstReq->Position >= CLAN_POS_GROUP_1 && pstReq->Position <= CLAN_POS_GROUP_6)
	{
		if (strong_pworld_is_open_by_clan(pstEnv, pstZoneClan) )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_STRONG243);
			return -1;
		}

		if (span_strong_pworld_is_open_by_clan(pstEnv,pstZoneClan->stClan.ClanProf.Id, pstPlayer->stRoleData.WorldID))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_STRONG243);
			return -1;
		}
	}

	if (clan_pos_cmp(kOwnedPos, pstReq->Position) <= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN271);
		return -1;
	}

	if (clan_pos_cmp(kOwnedPos, pstMem->Position) <= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN270);
		return -1;
	}

	switch(pstReq->Position)
	{
	case CLAN_POS_PRESIDENT:
		if (pstPlayer->stRoleData.RoleID != pstReq->RoleID)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN272);
			return -1;
		}
		pstMem->GroupID = 0;
		break;
	case CLAN_POS_VICE_PRESIDENT:
		if(pstZoneClan->stClan.ClanBrief.AssistNum > 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN273);
			return -1;
		}
		pstMem->GroupID = 0;
		break;
	case CLAN_POS_GROUP_1:
	case CLAN_POS_GROUP_2:
	case CLAN_POS_GROUP_3:
	case CLAN_POS_GROUP_4:
	case CLAN_POS_GROUP_5:
	case CLAN_POS_GROUP_6:
		{
			CLANMEM *pstCaptain =  clan_group_captain_get(pstZoneClan, pstReq->Position);
			
			if(0 > clan_group_valid_check(pstZoneClan, pstReq->Position - CLAN_POS_GROUP_1 +1))
			{
				return -1;
			}
			
			if(pstCaptain)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN274);
				return -1;
			}

			//提前下把归属战团设置上
			pstMem->GroupID = pstReq->Position - CLAN_POS_GROUP_1 +1;
		}
		break;

	case CLAN_POS_TECH_OFFICER:
	case CLAN_POS_COLLECTION_OFFICER:
	case CLAN_POS_FACTORY_OFFICER:
	case CLAN_POS_PET_OFFICER:
	case CLAN_POS_WALL_OFFICER:
		return -1;
		break;

	default:
		if (clan_pos_cmp(kOwnedPos, pstMem->Position) <= 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN270);
			return -1;
		}
		pstMem->GroupID = 0;
		break;
	}
	STRNCPY(pstMem->Title, pstReq->Title, CCH(pstMem->Title));
	pstMem->Position = pstReq->Position;

	pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstMem->RoleMember.RoleName);
	if(pstTmpPlayer)
	{
		player_in_strong_check(pstEnv, pstTmpPlayer);
	}

	clan_update_officers(pstEnv, pstZoneClan);
	player_clan_mem_chg_broadcast(pstEnv, pstZoneClan, pstMem,
	                              CLAN_MEMBER_CHG_MOD);
	
	clan_action(pstEnv, pstZoneClan, pstMem);

	player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD242,
	                          pstPlayer->stRoleData.RoleName,
	                          pstMem->RoleMember.RoleName,
	                          pstMem->Title[0] ? pstMem->Title :
	                          kClanPosName[(int)pstMem->Position]);
	return 0;
}


int player_clan_color_info(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSCLANCOLORNOTIFY *pstNotify;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);

	pstNotify = &stPkg.Body.ClanSvr.ClanSvrData.ClanColorNotify;
	stPkg.Body.ClanSvr.Operation = CLAN_SVR_COLOR_NOTIFY;
	if (pstClan)
	{
		pstNotify->Set = pstClan->stClan.ClanInner.SetClanColor;
	}
	else
	{
		memset(&pstNotify->Set, 0, sizeof(pstNotify->Set));
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int player_clan_set_color(ZONESVRENV *pstEnv, Player *pstPlayer, const CSSETCLANCOLOR *pstReq)
{
	ZoneClan *pstSelfClan;
	ZoneClan *pstOtherClan;
	SETCLANCOLOR *pstSet ;
	int i;
	int iNotify = 0;

	pstSelfClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstSelfClan)
	{	
		tlog_error(pstEnv->pstLogCat, 0, 0, "player_clan_set_color fail:pstSelfClan is NULL");
		return -1;
	}

	pstOtherClan = z_id_clan(pstEnv, pstReq->Set.ClanID);
	if (!pstOtherClan)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "player_clan_set_color fail:pstOtherClan is NULL");
		return -1;
	}
		
	pstSet = &pstSelfClan->stClan.ClanInner.SetClanColor;

	if (pstReq->Set.Color <= 0)
	{
		// del
		for (i=pstSet->Num-1; i>=0; i--)
		{
			if (pstSet->ClanColor[i].ClanID == pstReq->Set.ClanID)
			{
				pstSet->Num --;
				if (i != pstSet->Num)
				{
					pstSet->ClanColor[i] = pstSet->ClanColor[pstSet->Num];
				}
				iNotify = 1;
				break;
			}
		}
	}
	else
	{
		// modify
		for (i=0; i<pstSet->Num; i++)
		{
			if (pstSet->ClanColor[i].ClanID == pstReq->Set.ClanID)
			{
				if (pstSet->ClanColor[i].Color != pstReq->Set.Color)
				{
					pstSet->ClanColor[i].Color= pstReq->Set.Color;
					iNotify = 1;
				}
				break;
			}
		}

		// add
		if (i>=pstSet->Num )
		{
			if (pstSet->Num < MAX_CLAN_COLOR)
			{
				pstSet->ClanColor[pstSet->Num].ClanID = pstReq->Set.ClanID;
				pstSet->ClanColor[pstSet->Num].Color	= pstReq->Set.Color;
				pstSet->ClanColor[pstSet->Num].ClanGID = pstOtherClan->stClan.ClanProf.GID;
				pstSet->Num ++;
				iNotify = 1;
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN275);
			}
		}
	}

	if (iNotify)
	{
		pstSelfClan->stZoneClanInfo.iDirty = CACHE_FLAG_IMM; // 立即更新
		player_clan_color_chg_broadcast(pstEnv,pstSelfClan);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN276);
	}
	return 0;
}

int player_clan_map_pos_broadcast(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	CSPKG stPkg;
	CSCLANSVR  *pstClanSvr = &stPkg.Body.ClanSvr;

	pstClanSvr->Operation = CLAN_SVR_MAP_POS_NOTIFY;
	pstClanSvr->ClanSvrData.MapPosInfo.Info = pstZoneClan->stZoneClanInfo.stClanMapPosInfo;

	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	return player_clan_broadcast(pstEnv, pstZoneClan, &stPkg, 0);
}

int player_clan_map_pos_info(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSCLANSVR  *pstClanSvr = &stPkg.Body.ClanSvr;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);

	if(!pstZoneClan)
	{
		return -1;
	}

	pstClanSvr->Operation = CLAN_SVR_MAP_POS_NOTIFY;
	pstClanSvr->ClanSvrData.MapPosInfo.Info = pstZoneClan->stZoneClanInfo.stClanMapPosInfo;

	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	return z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
}

int clan_skill_auto_learn(ZONESVRENV * pstEnv,  ZoneClan* pstZoneClan,
							CLANSKILLDEF* pstDef,int iPosIdx)
{
	int i;
	Player *pstPlayer = NULL;

	for(i = 0; i < pstZoneClan->stClan.ClanProf.MemNum;i++)
	{
		if (!pstZoneClan->stClan.ClanCore.ClanMem[i].Online)
		{
			continue;
		}

		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
	                     pstZoneClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName);
		if (!pstPlayer)
		{
			continue;
		}
		
		//player_clan_skill_learn_in(pstEnv, pstPlayer,pstDef,iPosIdx);
		//刷新属性
		z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);		
	}
	
	return 0;
	UNUSED(pstDef);	//add by paraunused.pl
	UNUSED(iPosIdx);	//add by paraunused.pl
}

int clan_action_clan_skill_info(ZONESVRENV * pstEnv,  ZoneClan* pstZoneClan)
{
	int i;
	Player *pstPlayer = NULL;

	for(i = 0; i < pstZoneClan->stClan.ClanProf.MemNum;i++)
	{
		if (!pstZoneClan->stClan.ClanCore.ClanMem[i].Online)
		{
			continue;
		}

		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
	                     pstZoneClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName);
		if (!pstPlayer)
		{
			continue;
		}
		
		player_action_clan_skill_info(pstEnv, pstPlayer,
			&pstZoneClan->stClan.ClanAdvance.ClanSkillInfo);
	}

	return 0;
	
}



int player_clan_skill_research(ZONESVRENV *pstEnv, Player *pstPlayer, CSCLANSKILLRESEARCH *pstCSClanSkillResearch)
{
	CSCLANSKILLRESEARCH * pstReq = pstCSClanSkillResearch;
	int iIdx = -1;
	int i = 0;
	int iNum = MAX_CLAN_SKILL_NUM;
	CLANSKILLINFO * pstInfo = NULL;
	CLANSKILLDEF* pstDef = NULL;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);


	if (player_check_safelock(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}

	if(!pstZoneClan)
	{
		return -1;
	}

	pstDef = z_find_clan_skill_def(pstEnv,pstReq->SkillID, pstReq->Level);

	if (!pstDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN277);
		return -1;
	}

	if ((unsigned short)pstDef->ResearchNeedClanLevel > pstZoneClan->stClan.ClanProf.Level)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN336);
		return -1;
	}

	if (pstDef->ResearchNeedClanMoney > 0 &&
		(unsigned int)pstDef->ResearchNeedClanMoney > pstZoneClan->stClan.ClanProf.Money)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN278);
		return -1;
	}
	
	pstInfo = &pstZoneClan->stClan.ClanAdvance.ClanSkillInfo;

	iNum = min(iNum,pstInfo->Num);

	if (iNum >= MAX_CLAN_SKILL_NUM && pstReq->Level == 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN279);
		return -1;
	}

	for(i = 0; i < iNum; i++)
	{
		if (pstInfo->ClanSkill[i].SkillID == pstReq->SkillID)
		{
			iIdx = i;
			break;
		}
	}

	if (iIdx < 0 )
	{
		if (pstReq->Level == 1)
		{
			iIdx = pstInfo->Num;
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN279);
			return -1;
		}
	}
	
	if (iIdx < 0 || iIdx >= MAX_CLAN_SKILL_NUM)
	{		
		return -1;
	}

	if (pstInfo->ClanSkill[iIdx].Level > pstInfo->ClanSkill[iIdx].ValidLevel)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN280);
		return -1;		
	}

	if (pstInfo->ClanSkill[iIdx].Level != pstReq->Level - 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN281);
		return -1;		
	}
	
	if (pstDef->ResearchNeedClanMoney > 0)
	{
		clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_MONEY, 
			                     pstDef->ResearchNeedClanMoney , 1);
	}

	pstInfo->ClanSkill[iIdx].SkillID = pstReq->SkillID;
	pstInfo->ClanSkill[iIdx].Level = pstReq->Level;
	pstInfo->ClanSkill[iIdx].ValidLevel = pstReq->Level;

	if(iIdx == pstInfo->Num && pstInfo->Num < MAX_CLAN_SKILL_NUM)
	{
		pstInfo->Num++;
	}

	clan_action_clan_skill_info(pstEnv, pstZoneClan);

	for(i = 0; i < MAX_CLAN_SKILL_RESULT;i++)
	{
		if (!pstDef->Result[i].ResultID)
		{
			break;
		}
		if (pstDef->Result[i].ResultID == RESULT_CLAN_CITY_BUFF)
		{
			clan_city_buff_chg(pstEnv,pstZoneClan,pstDef->Result[i].ResultVal1,
				pstDef->Result[i].ResultVal2,pstDef->Result[i].ResultVal3);
		}
	}
	
	clan_skill_mem_limit_chg(pstEnv,pstZoneClan,pstCSClanSkillResearch);
	clan_skill_business_exp_chg(pstEnv,pstZoneClan,pstCSClanSkillResearch);

	if (pstDef->AutoLearn == 1)
	{
		clan_skill_auto_learn(pstEnv,pstZoneClan,pstDef,iIdx);
	}
	
	player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,ZONEERR_CLANBROAD243,
				pstDef->Name,pstDef->Level,pstDef->ResearchNeedClanMoney);

	return 0;
}

//公会技能附加额外人物自身属性
int clan_skill_addattr_self(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i,k;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	PLAYERCLANSKILLINFO *pstPlayerInfo = NULL;
	CLANSKILLINFO *pstClanInfo = NULL;
	CLANSKILL *pstClanSkill = NULL;
	PLAYERCLANSKILL* pstPlayerClanSkill = NULL;
	RESULT* pstResult = NULL;
	ZoneClan* pstClan = NULL;
	int iNum = MAX_CLAN_SKILL_NUM;
	int iLevel = 0;
	CLANSKILLDEF *pstDef = NULL;


	pstClan = player_get_clan(pstEnv,pstPlayer);

	if (!pstClan)
	{
		return -1;
	}
	pstPlayerInfo = &pstPlayer->stRoleData.MiscInfo.ClanSkillInfo;
	pstClanInfo = &pstClan->stClan.ClanAdvance.ClanSkillInfo;

	iNum = min(iNum,pstPlayerInfo->Num);

	for(i = 0; i < iNum;i++)
	{
		pstPlayerClanSkill = &pstPlayerInfo->ClanSkill[i];
		if (pstPlayerClanSkill->ClanSkillIdx < 0 || 
			pstPlayerClanSkill->ClanSkillIdx >= pstClanInfo->Num ||
			pstPlayerClanSkill->ClanSkillIdx >= MAX_CLAN_SKILL_NUM)
		{
			continue;
		}
		pstClanSkill = &pstClanInfo->ClanSkill[pstPlayerClanSkill->ClanSkillIdx];

		if (pstClanSkill->SkillID != pstPlayerClanSkill->SkillID)
		{
			continue;
		}
		
		iLevel = min(pstPlayerClanSkill->Level,pstClanSkill->ValidLevel);
		if (iLevel < 1)
		{
			continue;
		}

		pstDef = z_find_clan_skill_def(pstEnv,pstPlayerClanSkill->SkillID,iLevel);
		if (!pstDef)
		{
			continue;
		}
		
		for(k = 0; k < MAX_CLAN_SKILL_RESULT;k++)
		{
			pstResult = &pstDef->Result[k];
			if (!pstResult->ResultID)
			{
				break;
			}
			z_attr_result(pstEnv, pstDyn, NULL, pstResult);
		}
	}

	
	iNum = min(MAX_CLAN_SKILL_NUM,pstClanInfo->Num);
	
	for(i = 0; i < iNum;i++)
	{	
		pstClanSkill = &pstClanInfo->ClanSkill[i];

		iLevel = pstClanSkill->ValidLevel;
		if (iLevel < 1)
		{
			continue;
		}

		pstDef = z_find_clan_skill_def(pstEnv,pstClanSkill->SkillID,iLevel);
		if (!pstDef)
		{
			continue;
		}

		if (pstDef->AutoLearn != 1)
		{
			continue;
		}
		
		for(k = 0; k < MAX_CLAN_SKILL_RESULT;k++)
		{
			pstResult = &pstDef->Result[k];
			if (!pstResult->ResultID)
			{
				break;
			}
			z_attr_result(pstEnv, pstDyn, NULL, pstResult);
		}
	}

	return 0;
}


int player_clan_skill_learn_in(ZONESVRENV *pstEnv, Player *pstPlayer, CLANSKILLDEF* pstDef,int iPosIdx)
{
	int iIdx = -1;
	int i = 0;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	PLAYERCLANSKILLINFO *pstPlayerInfo = NULL;

	if(!pstZoneClan)
	{
		return -1;
	}

	pstPlayerInfo = &pstPlayer->stRoleData.MiscInfo.ClanSkillInfo;

	for(i = 0; i < pstPlayerInfo->Num;i++)
	{
		if(pstPlayerInfo->ClanSkill[i].SkillID == pstDef->SkillID)
		{
			iIdx = i;
			break;
		}
	}

	if (iIdx < 0 && pstPlayerInfo->Num < MAX_CLAN_SKILL_NUM)
	{
		iIdx = pstPlayerInfo->Num;
	}

	if (iIdx >= MAX_CLAN_SKILL_NUM || iIdx < 0)
	{
		return -1;
	}

	pstPlayerInfo->ClanSkill[iIdx].Level = pstDef->Level;
	pstPlayerInfo->ClanSkill[iIdx].SkillID = pstDef->SkillID;
	pstPlayerInfo->ClanSkill[iIdx].ClanSkillIdx = iPosIdx;

	if (iIdx == pstPlayerInfo->Num && pstPlayerInfo->Num < MAX_CLAN_SKILL_NUM)
	{
		pstPlayerInfo->Num++;
	}

	player_action_player_clan_skill_info(pstEnv,pstPlayer, pstPlayerInfo);
		

	//刷新属性
	z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);

	return 0;
}


int player_clan_skill_learn(ZONESVRENV *pstEnv, Player *pstPlayer, CSCLANSKILLLEARN *pstCSClanSkillLearn)
{
	CSCLANSKILLLEARN *pstReq = pstCSClanSkillLearn;
	int iIdx = -1;
	int i = 0;
	int iNum = MAX_CLAN_SKILL_NUM;
	CLANSKILLINFO * pstInfo = NULL;
	CLANSKILLDEF* pstDef = NULL;
	CLANSKILL* pstClanSkill = NULL;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	PLAYERCLANSKILLINFO * pstPlayerClanSkill = NULL;
	int iClanIdx = 0;

	if (player_check_safelock(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}

	if(!pstZoneClan)
	{
		return -1;
	}

	pstInfo = &pstZoneClan->stClan.ClanAdvance.ClanSkillInfo;

	iNum = min(iNum,pstInfo->Num);

	for(i = 0; i < iNum; i++)
	{
		if (pstInfo->ClanSkill[i].SkillID == pstReq->SkillID)
		{
			pstClanSkill = &pstInfo->ClanSkill[i];
			iClanIdx = i;
			break;
		}
	}

	if (!pstClanSkill)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN282);
		return -1;
	}

	if (pstClanSkill->ValidLevel < pstReq->Level)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN283);
		return -1;		
	}
	

	pstDef = z_find_clan_skill_def(pstEnv,pstReq->SkillID, pstReq->Level);

	if (!pstDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN284);
		return -1;
	}

	if ((unsigned int)pstDef->LearnNeedContri > pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN285);
		return -1;
	}

	if (pstDef->AutoLearn != 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN286);
		return -1;		
	}

	
	pstPlayerClanSkill = &pstPlayer->stRoleData.MiscInfo.ClanSkillInfo;

	iNum = pstPlayerClanSkill->Num;

	if (iNum >= MAX_CLAN_SKILL_NUM && pstReq->Level == 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN287);
		return -1;
	}

	iIdx = -1;
	for(i = 0; i < iNum; i++)
	{
		if (pstPlayerClanSkill->ClanSkill[i].SkillID == pstReq->SkillID)
		{
			iIdx = i;
			break;
		}
	}

	if (iIdx < 0 )
	{
		if (pstReq->Level == 1)
		{
			iIdx = pstPlayerClanSkill->Num;
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN288);
			return -1;
		}
	}
	else if (pstPlayerClanSkill->ClanSkill[iIdx].Level != pstReq->Level - 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN289);
		return -1;		
	}
	
	if (iIdx < 0 || iIdx >= MAX_CLAN_SKILL_NUM)
	{		
		return -1;
	}

	/*
	if (pstPlayerClanSkill->ClanSkill[iIdx].Level != pstReq->Level - 1)
	{
		z_sys_strf(pstEnv,pstPlayer,SYS_MIDDLE2,"需要学习此技能的前置等级才能升级");
		return -1;		
	}*/

	if (pstDef->LearnNeedContri > 0)
	{
		player_contribute_dec(pstEnv,pstPlayer, pstDef->LearnNeedContri,0);
	}

	player_clan_skill_learn_in(pstEnv,pstPlayer,pstDef,iClanIdx);

	return 0;
}

int player_clan_set_map_pos(ZONESVRENV *pstEnv, Player *pstPlayer, CSSETCLANMAPPOS *pstCSClanMapPos)
{
	CSPKG stPkg;
	POSTIONLINK stPostionLink;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	CLANMAPPOS *pstClanMapPosInfo = &pstZoneClan->stZoneClanInfo.stClanMapPosInfo;

	if(!pstZoneClan)
	{
		return -1;
	}

	if (0 == z_valid_pos(pstEnv->pstMapObj, pstCSClanMapPos->MapID, NULL, &pstCSClanMapPos->Pos))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN290);
		return -1;
	}

	switch(pstCSClanMapPos->Type)
	{
		case CLAN_MAP_POS_TYPE_ATK:
			pstClanMapPosInfo->AtkInfo.MapID = pstCSClanMapPos->MapID;
			pstClanMapPosInfo->AtkInfo.Pos = pstCSClanMapPos->Pos;
			pstClanMapPosInfo->AtkInfo.EndTime = pstEnv->pstAppCtx->stCurr.tv_sec + 60;
			stPostionLink.MapID =  pstCSClanMapPos->MapID;
			stPostionLink.X =  pstCSClanMapPos->Pos.X;
			stPostionLink.Y = pstCSClanMapPos->Pos.Y;
	
			z_fill_link_msg(&stPkg,  SYS_CLAN, CHAT_LINK_POSTION, (void *)&stPostionLink, ZONEERR_LINK302);
			break;
		case CLAN_MAP_POS_TYPE_VENUE:
			pstClanMapPosInfo->VenueInfo.MapID = pstCSClanMapPos->MapID;
			pstClanMapPosInfo->VenueInfo.Pos = pstCSClanMapPos->Pos;
			pstClanMapPosInfo->VenueInfo.EndTime = pstEnv->pstAppCtx->stCurr.tv_sec + 60;
			stPostionLink.MapID =  pstCSClanMapPos->MapID;
			stPostionLink.X =  pstCSClanMapPos->Pos.X;
			stPostionLink.Y = pstCSClanMapPos->Pos.Y;
			
			z_fill_link_msg(&stPkg,  SYS_CLAN, CHAT_LINK_POSTION, (void *)&stPostionLink, ZONEERR_LINK303);
			break;
		default:
			return -1;
			break;
	}

	player_clan_broadcast(pstEnv, pstZoneClan, &stPkg, 0);

	return player_clan_map_pos_broadcast(pstEnv, pstZoneClan);
}

int clan_level_down_group_clear(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	int i;
	CLANMEM *pstMem;

	for(i = 0; i < pstZoneClan->stClan.ClanProf.MemNum; i++)
	{
		pstMem = &pstZoneClan->stClan.ClanCore.ClanMem[i];
		if(pstMem->GroupID <= 0)
		{
			continue;
		}
		
		if(0 == clan_group_valid_check(pstZoneClan, pstMem->GroupID))
		{
			continue;
		}

		pstMem->GroupID = 0;
		if(pstMem->Position != CLAN_POS_NORMAL && pstMem->Position >= CLAN_POS_GROUP_1)
		{
			pstMem->Position = CLAN_POS_NORMAL;
		}

		player_clan_mem_chg_broadcast(pstEnv, pstZoneClan, pstMem, CLAN_MEMBER_CHG_MOD);
		clan_action(pstEnv, pstZoneClan, pstMem);
	}
	
	return 0;
}

int clan_group_role_num_get(ZoneClan *pstZoneClan, int iGroupID, int fCaptain)
{
	int i;
	int iCount = 0;
	CLANMEM *pstMem;

	for(i = 0; i < pstZoneClan->stClan.ClanProf.MemNum; i++)
	{
		pstMem = &pstZoneClan->stClan.ClanCore.ClanMem[i];
		if(iGroupID != pstMem->GroupID)
		{
			continue;
		}

		if(fCaptain  == 0 && pstMem->Position >= CLAN_POS_GROUP_1)
		{
			continue;
		}
		
		iCount++;
	}
	
	return iCount;
}

int clan_group_valid_check(ZoneClan *pstZoneClan, int iGroupID)
{
	if(iGroupID <= 0 || iGroupID> MAX_CLAN_GROUP_NUM)
	{
		return -1;
	}

	if(iGroupID == 6)
	{
		if(pstZoneClan->stClan.ClanCore.ClanGroup.UseItemGroup == 0)
		{
			return -1;
		}
	}
	else
	{
		if(iGroupID > pstZoneClan->stClan.ClanProf.Level)
		{
			return -1;
		}
	}
	
	return 0;
}

int is_in_strong_group_player(ZoneClan *pstZoneClan, Player *pstPlayer, int iMapID)
{
	CLANMEM *pstMem;
	
	pstMem = clan_id_mem(pstZoneClan, pstPlayer->stRoleData.RoleID,
	                         &pstPlayer->stOnline.iClanMemIdx);
	if (!pstMem )
	{
		return 0;
	}

	//if(pstMem->Position == CLAN_POS_VICE_PRESIDENT || pstMem->Position == CLAN_POS_PRESIDENT)
	//{
	//	return 1;
	//}

	if(pstMem->GroupID <= 0 || pstMem->GroupID > MAX_CLAN_GROUP_NUM)
	{
		return 0;
	}

	if(iMapID != pstZoneClan->stClan.ClanCore.ClanGroup.GroupInfo[pstMem->GroupID-1].StrongMap)
	{
		return 0;
	}

	return 1;
}

//检查角色在据点地图是否合法
int player_in_strong_check(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	ZONEPWORLD *pstPworld;
	STRONGPOINTDEF *pstStrongPointDef;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	CLANMEM *pstMem;
	
	pstPworld = z_find_pworld2(pstEnv, pstPlayer->stRoleData.Map);
	if (!pstPworld ||pstPworld->stPworldDef.StrongMapID <= 0)
	{
		return -1;
	}

	pstStrongPointDef = strong_def_find( pstEnv, pstPworld->stPworldDef.StrongMapID);
	if (!pstStrongPointDef)
	{
		return -1;
	}

	pstMem = clan_id_mem(pstZoneClan, pstPlayer->stRoleData.RoleID,
	                         &pstPlayer->stOnline.iClanMemIdx);
	if (!pstMem )
	{
		goto _clear;
	}

	//if(pstMem->Position == CLAN_POS_VICE_PRESIDENT || pstMem->Position == CLAN_POS_PRESIDENT)
	//{
	//	return 0;
	//}

	if(pstMem->GroupID <= 0 || pstMem->GroupID > MAX_CLAN_GROUP_NUM)
	{
		goto _clear;
	}

	if(pstStrongPointDef->MapID != pstZoneClan->stClan.ClanCore.ClanGroup.GroupInfo[pstMem->GroupID-1].StrongMap)
	{
		goto _clear;
	}
		
	return 0;

_clear:
	
	strong_pworld_force_exit(pstEnv, pstPlayer, NULL);
	return 0;
}

int player_clan_group_broadcast(ZONESVRENV *pstEnv,ZoneClan *pstZoneClan)
{
	CSPKG stPkg;
	CLANGROUPINFO *pstClanGroupInfo;

	pstClanGroupInfo = &stPkg.Body.ClanSvr.ClanSvrData.ClanGroupInfo;
	stPkg.Body.ClanSvr.Operation = CLAN_SVR_GROUP_INFO;
	*pstClanGroupInfo = pstZoneClan->stClan.ClanCore.ClanGroup;
	
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	player_clan_broadcast(pstEnv, pstZoneClan, &stPkg, 0);
	return 0;
}

int player_clan_group_info(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CLANGROUPINFO *pstClanGroupInfo;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);

	pstClanGroupInfo = &stPkg.Body.ClanSvr.ClanSvrData.ClanGroupInfo;
	stPkg.Body.ClanSvr.Operation = CLAN_SVR_GROUP_INFO;
	if (pstClan)
	{
		*pstClanGroupInfo = pstClan->stClan.ClanCore.ClanGroup;
	}
	else
	{
		memset(pstClanGroupInfo, 0, sizeof(*pstClanGroupInfo));
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_clan_set_group(ZONESVRENV *pstEnv, Player *pstPlayer, CSCLANSETGROUP *pstCSSetGroup)
{
	Player *pstTmpPlayer;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	CLANMEM *pstMem;
	const int kOwnedPos = player_clan_position(pstEnv, pstPlayer);

	if (!pstZoneClan)
	{
		return -1;
	}
	
	if(0 > clan_group_valid_check(pstZoneClan, pstCSSetGroup->Group))
	{
		return -1;
	}

	if (strong_pworld_is_open_by_clan(pstEnv, pstZoneClan) )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_STRONG243);
		return -1;
	}

	if (span_strong_pworld_is_open_by_clan(pstEnv,pstZoneClan->stClan.ClanProf.Id, pstPlayer->stRoleData.WorldID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_STRONG243);
		return -1;
	}
	
	pstMem = clan_id_mem(pstZoneClan, pstCSSetGroup->RoleID, NULL);
	if (!pstMem)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN267);
		return -1;
	}

	if(pstCSSetGroup->Set)
	{
		if(kOwnedPos != CLAN_POS_PRESIDENT && kOwnedPos != CLAN_POS_VICE_PRESIDENT)
		{
			if(pstMem->Position != CLAN_POS_NORMAL)
			{
				return -1;
			}

			if (pstCSSetGroup->RoleID == pstPlayer->stRoleData.RoleID)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN291 );
				return -1;
			}
				
			if (clan_pos_cmp(kOwnedPos, pstMem->Position) <= 0)
			{
				return -1;
			}
			
			if(kOwnedPos < CLAN_POS_GROUP_1)
			{
				return -1;
			}

			if(kOwnedPos - CLAN_POS_GROUP_1+1 != pstCSSetGroup->Group)
			{
				return -1;
			}
		}
		else
		{
			if (pstCSSetGroup->RoleID != pstPlayer->stRoleData.RoleID)
			{
				if (clan_pos_cmp(kOwnedPos, pstMem->Position) <= 0)
				{
					return -1;
				}
			}
		}

		if(pstMem->GroupID > 0)
		{
			if(kOwnedPos != CLAN_POS_PRESIDENT && kOwnedPos != CLAN_POS_VICE_PRESIDENT)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN293);
				return -1;
			}
		}

		if(CLAN_GROUP_MAX_NUM <= clan_group_role_num_get(pstZoneClan, pstCSSetGroup->Group, 0))
		{
			return -1;
		}

		pstMem->GroupID = pstCSSetGroup->Group;
		if(pstMem->Position >=  CLAN_POS_GROUP_1)
		{
			pstMem->Position = CLAN_POS_NORMAL;
		}
	}
	else
	{
		if(kOwnedPos != CLAN_POS_PRESIDENT && kOwnedPos != CLAN_POS_VICE_PRESIDENT)
		{
			if(pstMem->Position != CLAN_POS_NORMAL)
			{
				return -1;
			}

			if (pstCSSetGroup->RoleID == pstPlayer->stRoleData.RoleID)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_CLAN291 );
				return -1;
			}
				
			if (clan_pos_cmp(kOwnedPos, pstMem->Position) <= 0)
			{
				return -1;
			}
			
			if(kOwnedPos < CLAN_POS_GROUP_1)
			{
				return -1;
			}

			if(kOwnedPos - CLAN_POS_GROUP_1+1 != pstMem->GroupID)
			{
				return -1;
			}
		}
		else
		{
			if (pstCSSetGroup->RoleID != pstPlayer->stRoleData.RoleID)
			{
				if (clan_pos_cmp(kOwnedPos, pstMem->Position) <= 0)
				{
					return -1;
				}
			}
		}
		
		pstMem->GroupID = 0;
		if(pstMem->Position >=  CLAN_POS_GROUP_1)
		{
			pstMem->Position = CLAN_POS_NORMAL;
		}
	}

	// 检查这个人在据点地图合法性
	pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstMem->RoleMember.RoleName);
	if(pstTmpPlayer)
	{
		player_in_strong_check(pstEnv, pstTmpPlayer);
	}

	player_clan_mem_chg_broadcast(pstEnv, pstZoneClan, pstMem, CLAN_MEMBER_CHG_MOD);
	clan_action(pstEnv, pstZoneClan, pstMem);
	
	return 0;
}

int player_clan_set_group_strong(ZONESVRENV *pstEnv, Player *pstPlayer, CSCLANSETGROUPSTRONGMAP *pstCSSetGroupStrong)
{
	int i;
	CLANMEM *pstMem;
	Player *pstTmpPlayer;
	CLANGROUPINFO *pstClanGroupInfo;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	if(!pstZoneClan)
	{
		return -1;
	}

	//if(0 > clan_group_valid_check(pstZoneClan, pstCSSetGroupStrong->Group))
	//{
	//	return -1;
	//}
	if (strong_pworld_is_open_by_clan(pstEnv, pstZoneClan) )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_STRONG243);
		return -1;
	}

	pstClanGroupInfo = &pstZoneClan->stClan.ClanCore.ClanGroup;

	if(pstCSSetGroupStrong->Map > 0)
	{
		for(i=0; i<pstClanGroupInfo->GroupNum; i++)
		{
			if(0 > clan_group_valid_check(pstZoneClan, i+1))
			{
				continue;
			}

			if(pstClanGroupInfo->GroupInfo[i].StrongMap == pstCSSetGroupStrong->Map)
			{
				return -1;
			}
		}
	}

	pstClanGroupInfo->GroupInfo[pstCSSetGroupStrong->Group-1].StrongMap = pstCSSetGroupStrong->Map;
	player_clan_group_broadcast(pstEnv, pstZoneClan);

	for(i = 0; i < pstZoneClan->stClan.ClanProf.MemNum; i++)
	{
		pstMem = &pstZoneClan->stClan.ClanCore.ClanMem[i];

		pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstMem->RoleMember.RoleName);
		if(!pstTmpPlayer)
		{
			continue;
		}

		player_in_strong_check(pstEnv, pstTmpPlayer);
	}
	
	return 0;
}

int player_clan_open_group(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int iRet;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	if(!pstClan)
	{
		return -1;
	}

	if(pstClan->stClan.ClanCore.ClanGroup.UseItemGroup)
	{
		return -1;
	}

	iRet = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
					CLAN_OPEN_GROUP_ITEM_ID, 1, 
					-1, NULL,PACKAGE_DEC_NONE, 0);
	if(iRet < 0)
	{
		return -1;
	}

	pstClan->stClan.ClanCore.ClanGroup.UseItemGroup = 1;
	player_clan_group_broadcast(pstEnv, pstClan);
	return 0;
}

int player_clan_kick_city(ZONESVRENV *pstEnv, Player *pstPlayer, CSCLANKICKCITY *pstCSClanKickCity)
{
	Player *pstKickPlayer;
	ZoneClan *pstClan;
	ZoneClan* pstKickClan;
	
	int iKickPosition;
	int iPosition = player_clan_position(pstEnv, pstPlayer);

	pstKickPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstCSClanKickCity->RoleName);
	if(!pstKickPlayer)
	{
		return -1;
	}

	pstClan = player_get_clan(pstEnv, pstPlayer);
	if(!pstClan)
	{
		return -1;
	}

	pstKickClan = player_get_clan(pstEnv, pstKickPlayer);
	if(!pstKickClan)
	{
		return -1;
	}

	if(pstClan != pstKickClan)
	{
		return -1;
	}

	iKickPosition = player_clan_position(pstEnv, pstKickPlayer);
	if (clan_pos_cmp(iPosition, iKickPosition) <= 0)
	{
		return -1;
	}

	if (CLAN_CITY_MAP_ID == GET_MAPID(pstKickPlayer->stRoleData.Map))
	{
		if (!is_in_my_clan_city(pstEnv, pstPlayer))
		{
			return 0;
		}
	}

	player_pworld_exit_chgmap(pstEnv, pstKickPlayer);

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstKickPlayer, SYS_SVR, ZONEERR_CLAN801,
					kClanPosName[iPosition],
					pstPlayer->stRoleData.RoleName,
					pstKickPlayer->stRoleData.RoleName);

	player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD277,
	                          kClanPosName[iPosition],
	                          pstPlayer->stRoleData.RoleName);
	
	return 0;
}

int player_clan_check(ZONESVRENV *pstEnv, Player *pstPlayer,CSCLANCLT *pstClanClt)
{
	if (pstEnv->pstClanObj->stClanObjInfo.iClanStat != CLAN_STAT_FINI ||
		pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat != GLOBAL_STAT_FINI)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SVR_ERR );
		return -1;
	}

	if (pstClanClt->Operation >= MAX_CLAN_OP)
	{
		return -1;
	}

	if (clan_check_permission(pstEnv, pstPlayer, pstClanClt->Operation) < 0)
	{
		return -1;
	}

	if (clan_check_safelock(pstEnv, pstPlayer,pstClanClt->Operation) < 0)
	{
		return -1;
	}
	
	return 0;
}

int player_clan(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,
                TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSCLANCLT *pstClanClt = &pstCsPkg->Body.ClanClt;
	CSCLANCLTDATA *pstCltData = &pstClanClt->ClanCltData;

	UNUSED(pstAppCtx);
	UNUSED(pstFrameHead);

	// server关闭过程中禁止所有公会操作，夜长梦多。
	// 如果有人在服务器关闭过程中进行公会操作，
	// 公会数据将不能ALL CLEAN，导致服务器无法关闭。
	// 但是确保保存公会数据在保存角色数据之前进行，
	// 将不会发生这种情况。
	if (pstEnv->cShutdown > 1)
	{
		return -1;
	}

	// battle_svr的公会操作都转发给zone_svr处理
	if (pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
		return -1;
		// 转发的信息需要特殊处理一下,比如名字
		//return clan_proxy_request(pstEnv, pstPlayer, pstCsPkg);
	}

	if(0 > player_clan_check(pstEnv, pstPlayer,pstClanClt))
	{
		return -1;
	}

	switch(pstClanClt->Operation)
	{
	case CLAN_OP_CREATE: 	/* 创建一个公会 */
		player_clan_create(pstEnv, pstPlayer, pstCltData->ClanCreateClanName,1);
		break;

	case CLAN_OP_INVITE_JOIN_CLAN: 	/* 添加一个成员 */
		player_clan_invite_role(pstEnv, pstPlayer, pstCltData->ClanInviteName);
		break;

	case CLAN_OP_DEL_MEMBER: 	/* 删除一个成员 */
		player_clan_del_mem(pstEnv, pstPlayer, &pstCltData->ClanDelMemberGID);
		break;

	case CLAN_OP_DISBAND: 	/* 解散公会 */
		player_clan_disband(pstEnv, pstPlayer);
		break;

	case CLAN_OP_EDIT_BULLETIN: 	/* 编辑公告 */
		player_clan_set_bulletin(pstEnv, pstPlayer,
		                         pstCltData->ClanBulletinText);
		break;

	case CLAN_OP_EDIT_MANIFESTO: 	/* 编辑宣言 */
		player_clan_set_manifesto(pstEnv, pstPlayer,
		                          pstCltData->ClanManifestoText);
		break;

	case CLAN_OP_DEMISE_JOB: 	/* 让位 */
		player_clan_demise_job(pstEnv, pstPlayer,
		                       &pstCltData->ClanDemiseJobRoleID);
		break;

	case CLAN_OP_APPLY_JOIN_CLAN: 	/* 申请入会 */
		player_clan_apply_join_clan(pstEnv, pstPlayer, &pstCltData->ClanJoinID);
		break;

	case CLAN_OP_CENSOR_JOIN_CLAN: 	/* 入会审查 */
		player_clan_censor_join_clan(pstEnv, pstPlayer,
		                             &pstCltData->ClanCensorJoinClan);
		break;

	case CLAN_OP_SET_MEMBER_NICKNAME: 	/* 设置昵称 */
		player_clan_set_nickname(pstEnv, pstPlayer,
		                         &pstCltData->ClanMemberNickname);
		break;

	case CLAN_OP_APPLY_JOIN_LIST: 	/* 查看入会请求列表 */
		player_clan_apply_join_clan_list(pstEnv, pstPlayer );
		break;

	case CLAN_OP_INVITE_JOIN_UNION: 	/* 邀请加入同盟 */
		return 0; // 关闭同盟
		player_clan_invite_join_union(pstEnv, pstPlayer,
		                              &pstCltData->ClanJoinUnionCustomID);
		break;

	case CLAN_OP_APPLY_JOIN_UNION: 	/* 申请加入同盟 */
		return 0; // 关闭同盟
		player_clan_apply_join_union(pstEnv, pstPlayer,
		                             &pstCltData->ClanJoinUnionHostID );
		break;

	case CLAN_OP_EXIT_UNION: 	/* 退出同盟 */
		player_clan_exit_union(pstEnv, pstPlayer, &pstCltData->ClanExitUnionID);
		break;

	case CLAN_OP_APPLY_JOIN_ENEMY: 	/* 添加敌对公会 */
		player_clan_apply_join_enemy(pstEnv, pstPlayer,
		                             &pstCltData->ClanJoinEnemyHostID);
		break;

	//case CLAN_OP_EXIT_ENEMY: 	/* 删除敌对公会 */
	//	player_clan_exit_enemy(pstEnv, pstPlayer, &pstCltData->ClanExitEnemyID);
	//	break;

	//case CLAN_OP_ANSWER_EXIT_ENEMY: /* 回复别人的解除敌对请求 */
	//	player_clan_answer_exit_enemy(pstEnv, pstPlayer,
	//	                              &pstCltData->ClanAnswerExitEnemy);
	//	break;

	case CLAN_OP_DIPLOMATISM_STATUS: 	/* 查看公会的外交状态 */
		player_clan_diplomatism_status(pstEnv, pstPlayer);
		break;

	case CLAN_OP_ANSWER_JOIN_CLAN: 	/* 回复别人的入会邀请 */
		player_clan_answer_join_clan(pstEnv, pstPlayer,
		                             &pstCltData->ClanAnswerJoinClan);
		break;

	case CLAN_OP_ANSWER_JOIN_UNION: 	/* 回复别人的入盟邀请 */
		player_clan_answer_join_union(pstEnv, pstPlayer,
		                              &pstCltData->ClanAnswerJoinUnio);
		break;

	case CLAN_OP_FIND_REQ:
		player_clan_find_req(pstEnv, pstPlayer, &pstCltData->ClanFindReq);
		break;

	case CLAN_OP_CENSOR_JOIN_UNION: /* 审查申请入盟的公会 */
		player_clan_censor_join_union(pstEnv, pstPlayer,
		                              &pstCltData->ClanCensorJoinUnion);
		break;

	//case CLAN_OP_CENSOR_JOIN_ENEMY: /* 审查申请敌对的公会 */
	//	player_clan_censor_join_enemy(pstEnv, pstPlayer,
	//	                              &pstCltData->ClanCensorJoinEnemy);
	//	break;

	case CLAN_OP_MEMBER_SNAPSHOT: /* 要求查看所有成员信息的快照 */
		player_clan_member_snapshot(pstEnv, pstPlayer);
		break;

	case CLAN_OP_INFO: /* 要求查看公会的一般信息 */
		player_clan_info(pstEnv, pstPlayer);
		break;

	case CLAN_OP_MANIFESTO: /* 要求查看公会宣言 */
		player_clan_manifesto(pstEnv, pstPlayer);
		break;

	case CLAN_OP_UPGRADE: /* 升级公会 */
		player_clan_upgrade(pstEnv, pstPlayer);
		break;

	case CLAN_OP_ENABLE_CITY: /* 申请公会城市 */
		player_clan_city_upgrade(pstEnv, pstPlayer, pstCltData->ClanCityName);
		break;

	case CLAN_OP_UPGRADE_CITY: /* 升级公会城市 */
		player_clan_city_upgrade(pstEnv, pstPlayer, NULL);
		break;

	case CLAN_OP_UPGRADE_CITY_BUILDING: /* 升级公会城市建筑 */
		player_clan_city_building_upgrade(pstEnv, pstPlayer,
			                              pstCltData->ClanCityBuildingID);
		break;

	case CLAN_OP_CITY_INFO: /* 查看本公会的公会城市信息 */
		player_clan_city_info(pstEnv, pstPlayer);
		break;

	case CLAN_OP_VIEW_PAGE_BY_ID: /* 根据ID获取一页公会的基本信息 */
		player_clan_view_page(pstEnv, pstPlayer,
		                      &pstCltData->ClanViewPageByID);
		break;

	case CLAN_OP_SET_JOB_AND_TITLE: /* 设置公会成员的职位与封号 */
		player_clan_set_job_and_title(pstEnv, pstPlayer,
		                              &pstCltData->ClanSetJobAndTitle);
		break;

	/*--------------------------------以下属于高级功能----------------------- */
	case CLAN_OP_RESEARCH: /* 研发科技 */
		player_clan_research(pstEnv, pstPlayer, &pstCltData->ClanResearch);
		break;

	case CLAN_OP_RESEARCH_SPEEDUP: /* 研发加速 */
		player_clan_research_speedup(pstEnv, pstPlayer,
		                             &pstCltData->ClanResearchSpeedup);
		break;

	case CLAN_OP_CANCEL_RESEARCH: /* 取消研发科技 */
		player_clan_cancel_research(pstEnv, pstPlayer,
		                            &pstCltData->ClanCancelResearch);
		break;

	case CLAN_OP_MAKE: /* 制造物品/机械 */
		player_clan_make(pstEnv, pstPlayer, &pstCltData->ClanMake);
		break;

	case CLAN_OP_MAKING_SPEEDUP: /* 制造加速 */
		player_clan_making_speedup(pstEnv, pstPlayer,
		                           &pstCltData->ClanMakingSpeedup);
		break;

	case CLAN_OP_CANCEL_MAKING: /* 取消制造物品/机械 */
		player_clan_cancel_making(pstEnv, pstPlayer,
		                          &pstCltData->ClanCancelMaking);
		break;

	case CLAN_OP_GRAB_MACHINE: /* 从仓库中提取机械 */
	case CLAN_OP_CALLBACK_MACHINE: /* 召回机械放回仓库中 */
	case CLAN_OP_MACHINE_SUPPLY: /* 对机械进行补给 */
	case CLAN_OP_STORAGE_GRID_LOCK_SWITCH: /* 公会仓库格子锁切换 */
	case CLAN_OP_STORAGE_GRID_DESTROY: /* 公会仓库摧毁物品 */
		return -1;
		break;
		
	case CLAN_OP_RESOURCE_LOCK_SWITCH: /* 公会中心锁切换 */
		player_clan_resource_lock_switch(pstEnv, pstPlayer,
		                                &pstCltData->ClanResourceLockSwitch);
		break;

	case CLAN_OP_RESOURCE_GRAB_ITEM: /* 公会资源中心锁物品兑换 */
		player_clan_resource_grab_item(pstEnv, pstPlayer,&pstCltData->ClanResourceGrabItem);
		break;

	case CLAN_OP_SET_COLOR:
		player_clan_set_color(pstEnv, pstPlayer,  &pstCltData->ClanSetColor);
		break;
	case CLAN_OP_SET_MAP_POS:
		player_clan_set_map_pos(pstEnv, pstPlayer, &pstCltData->ClanSetMapPos);
		break;
	case CLAN_OP_SKILL_RESEARCH:
		player_clan_skill_research(pstEnv, pstPlayer, &pstCltData->ClanSkillResearch);
		break;
	case CLAN_OP_SKILL_LEARN:
		player_clan_skill_learn(pstEnv, pstPlayer, &pstCltData->ClanSkillLearn);
		break;
	case CLAN_OP_VIEW_GETRES_INFO:
		player_clan_getres_info(pstEnv, pstPlayer, &pstCltData->ViewGetResClt);
		break;
	case CLAN_OP_CLAN_SUMMON_ANSWER:
		player_clan_summon_answer(pstEnv, pstPlayer, &pstCltData->SummonAnswer);
		break;
	case CLAN_OP_SET_GROUP:
		player_clan_set_group(pstEnv, pstPlayer, &pstCltData->ClanSetGroup);
		break;
	case CLAN_OP_SET_GROUP_STRONG_MAP:
		player_clan_set_group_strong(pstEnv, pstPlayer, &pstCltData->ClanSetGroupStrongMap);
		break;
	case CLAN_OP_OPEN_GROUP:
		player_clan_open_group(pstEnv, pstPlayer);
		break;
	case CLAN_OP_KICK_CITY:
		player_clan_kick_city(pstEnv, pstPlayer, &pstCltData->KickCity);
	default:
		return -1;
		break;
	}

	return 0;
}

int ss_clan_all_info(ZONESVRENV *pstEnv,
                     const ZONECTLGETCLANALLINFOREQ *pstReq)
{
	SSPKG stSSPkg;
	ZONECTLGETCLANALLINFORES *pstRes = 
		&stSSPkg.Body.CsCtrL.CsCmd.CtlGetClanAllInfoRes;
	const ZoneClan *pstZoneClan = clan_find_by_name(pstEnv,
	                                                pstReq->Core.ClanName);

	bzero(pstRes, sizeof(*pstRes));
	if (!pstZoneClan)
	{
		pstRes->Core.Succeeded = 0;
		goto final;
	}
	pstRes->Core.Succeeded = 1;
	pstRes->Core.ClanActiveTime  = pstZoneClan->stClan.ClanActiveTime;
	pstRes->Core.ClanBrief       = pstZoneClan->stClan.ClanBrief;
	pstRes->Core.ClanCity        = pstZoneClan->stClan.ClanCity;
	pstRes->Core.ClanCore        = pstZoneClan->stClan.ClanCore;
	pstRes->Core.ClanDestroyTime = pstZoneClan->stClan.ClanDestroyTime;
	pstRes->Core.ClanInner       = pstZoneClan->stClan.ClanInner;
	pstRes->Core.ClanProf        = pstZoneClan->stClan.ClanProf;

final:
	ss_init_head(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_GET_CLAN_ALL_INFO_RES;
	pstRes->Extra = pstReq->Extra;
	ss_sendto_cs_svr(pstEnv, &stSSPkg);
	return 0;
}

int ss_clan_del_member(ZONESVRENV *pstEnv,
                       const ZONECTLDELCLANMEMBERREQ *pstReq)
{
	SSPKG stSSPkg;
	ZONECTLDELCLANMEMBERRES *pstRes = 
		&stSSPkg.Body.CsCtrL.CsCmd.CtlDelClanMemberRes;
	ZoneClan *pstZoneClan = clan_find_by_name(pstEnv, pstReq->Core.ClanName);

	if (!pstZoneClan)
	{
		pstRes->Core.Succeeded = 0;
		goto final;
	}

	if (clan_del_mem_by_name(pstEnv, pstZoneClan, pstReq->Core.RoleName) < 0)
	{
		pstRes->Core.Succeeded = 0;
	}
	else
	{
		pstRes->Core.Succeeded = 1;
	}

final:
	ss_init_head(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_DEL_CLAN_MEMBER_RES;
	pstRes->Extra = pstReq->Extra;
	ss_sendto_cs_svr(pstEnv, &stSSPkg);
	return 0;
}

int ss_clan_disband(ZONESVRENV *pstEnv, const ZONECTLDISBANDCLANREQ *pstReq)
{
	SSPKG stSSPkg;
	ZONECTLDISBANDCLANRES *pstRes = 
		&stSSPkg.Body.CsCtrL.CsCmd.CtlDisbandClanRes;
	ZoneClan *pstZoneClan = clan_find_by_name(pstEnv, pstReq->Core.ClanName);

	if (!pstZoneClan)
	{
		pstRes->Core.Succeeded = 0;
		goto final;
	}

	// 通知在线的公会成员
	player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD244);

	// 为了提高响应速度, 先销毁再提交数据库删除
	clan_partial_destroy(pstEnv, pstZoneClan);

	// 非延迟摧毁,部分数据已经先回收了
	pstZoneClan->stZoneClanInfo.fWillDelayDestroy = 0;
	clan_set_dirty(pstZoneClan, CACHE_FLAG_INEXIST);

	pstRes->Core.Succeeded = 1;

	z_role_clan_oplog(pstEnv, NULL, clan_id_warp(&pstZoneClan->stClan),
			  OP_CLAN_DESTROY_GM,NULL);
final:
	ss_init_head(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_DISBAND_CLAN_RES;
	pstRes->Extra = pstReq->Extra;
	ss_sendto_cs_svr(pstEnv, &stSSPkg);
	return 0;
}

int ss_clan_del_diplomatism(ZONESVRENV *pstEnv,
                            const ZONECTLDELCLANDIPLOMATISMREQ *pstReq)
{
	SSPKG stSSPkg;
	ZONECTLDELCLANDIPLOMATISMRES *pstRes = 
		&stSSPkg.Body.CsCtrL.CsCmd.CltDelClanDiplomatismRes;
	ZoneClan *pstZoneClan = clan_find_by_name(pstEnv, pstReq->Core.ClanName);

	pstRes->Core.Succeeded = 0;
	if (!pstZoneClan ||
		clan_del_diplomatism(pstEnv, pstZoneClan, pstReq->Core.TargetName) < 0)
	{
		goto final;
	}

	pstZoneClan = clan_find_by_name(pstEnv, pstReq->Core.TargetName);
	if (!pstZoneClan ||
		clan_del_diplomatism(pstEnv, pstZoneClan, pstReq->Core.ClanName) < 0)
	{
		goto final;
	}

	pstRes->Core.Succeeded = 1;
final:
	ss_init_head(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_DEL_CLAN_DIPLOMATISM_RES;
	pstRes->Extra = pstReq->Extra;
	ss_sendto_cs_svr(pstEnv, &stSSPkg);
	return 0;
}

int ss_clan_clear_diplomatism(ZONESVRENV *pstEnv,
                              const ZONECTLCLEARCLANDIPLOMATISMREQ *pstReq)
{
	int i = 0;
	SSPKG stSSPkg;
	ZONECTLCLEARCLANDIPLOMATISMRES *pstRes = 
		&stSSPkg.Body.CsCtrL.CsCmd.CtlClearClanDiplomatismRes;
	ZoneClan *pstZoneClan = clan_find_by_name(pstEnv, pstReq->Core.ClanName);

	pstRes->Core.Succeeded = 0;
	if (!pstZoneClan)
	{
		goto final;
	}

	i = pstZoneClan->stClan.ClanInner.ClanFriendNum;
	while (i--)
	{
		ZoneClan *pstIter = z_clanid_clan(pstEnv,
		                  &pstZoneClan->stClan.ClanInner.ClanFriends[i].ClanId);
		clan_clear_diplomatism(pstEnv, pstIter);
	}
	i = pstZoneClan->stClan.ClanInner.ClanEnemyNum;
	while (i--)
	{
		ZoneClan *pstIter = z_clanid_clan(pstEnv,
		                   &pstZoneClan->stClan.ClanInner.ClanEnemys[i].ClanId);
		clan_clear_diplomatism(pstEnv, pstIter);
	}
	clan_clear_diplomatism(pstEnv, pstZoneClan);

	pstRes->Core.Succeeded = 1;
final:
	ss_init_head(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_CLEAR_CLAN_DIPLOMATISM_RES;
	pstRes->Extra = pstReq->Extra;
	ss_sendto_cs_svr(pstEnv, &stSSPkg);
	return 0;
}

static int set_clan_base_attr(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,const ZONECTLSETCLANBASEATTRREQ *pstReq)
{

	// 设置一个公会的基本资源,金,定,繁,科
	pstZoneClan->stClan.ClanProf.Money = pstReq->Core.Money;
	pstZoneClan->stClan.ClanProf.TechPoint = pstReq->Core.Tech;
	pstZoneClan->stClan.ClanProf.StableValue = pstReq->Core.Stable;
	pstZoneClan->stClan.ClanProf.ActiveValue = pstReq->Core.Active;

	clan_set_dirty_notify(pstZoneClan, CACHE_FLAG_IMM); // 立即更新到数据库

	clan_property_do_notify(pstEnv, pstZoneClan,  pstReq->Core.Money, CLAN_PROPERTY_MONEY);
	clan_property_do_notify(pstEnv, pstZoneClan,  pstReq->Core.Tech, CLAN_PROPERTY_TECH_POINT);
	clan_property_do_notify(pstEnv, pstZoneClan,  pstReq->Core.Stable, CLAN_PROPERTY_STABLE_VALUE);
	clan_property_do_notify(pstEnv, pstZoneClan,  pstReq->Core.Active, CLAN_PROPERTY_ACTIVE_VALUE);

	// 通知在线的公会成员
	player_clan_msg_broadcast(pstEnv, pstZoneClan, SYS_CLAN,
	                          ZONEERR_CLANBROAD245);
	return 0;
}

int ss_set_clan_base_attr(ZONESVRENV *pstEnv,
                              const ZONECTLSETCLANBASEATTRREQ *pstReq)
{
	SSPKG stSSPkg;
	ZONECTLSETCLANBASEATTRRES *pstRes = 
		&stSSPkg.Body.CsCtrL.CsCmd.CtlSetClanBaseAttrRes;
	ZoneClan *pstZoneClan = clan_find_by_name(pstEnv, pstReq->Core.ClanName);

	pstRes->Core.Succeeded = 0;
	if (!pstZoneClan)
	{
		goto final;
	}

	set_clan_base_attr(pstEnv, pstZoneClan,pstReq);

	pstRes->Core.Succeeded = 1;
final:
	ss_init_head(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_SET_CLAN_BASE_ATTR_RES;
	pstRes->Extra = pstReq->Extra;
	ss_sendto_cs_svr(pstEnv, &stSSPkg);
	return 0;
}

int ss_clan_exchange_leader(ZONESVRENV *pstEnv,
                            const ZONECTLEXCHANGECLANLEADERREQ *pstReq)
{
	SSPKG stSSPkg;
	ZONECTLEXCHANGECLANLEADERRES *pstRes = 
		&stSSPkg.Body.CsCtrL.CsCmd.CltExchangeClanLeaderRes;
	ZoneClan *pstZoneClan = clan_find_by_name(pstEnv, pstReq->Core.ClanName);

	if (!pstZoneClan)
	{
		pstRes->Core.Succeeded = 0;
		goto final;
	}

	pstRes->Core.Succeeded = clan_exchange_leader(pstEnv, pstZoneClan,
	                                pstReq->Core.NewLeaderRoleName) < 0 ? 0 : 1;
final:
	ss_init_head(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_EXCHANGE_CLAN_LEADER_RES;
	pstRes->Extra = pstReq->Extra;
	ss_sendto_cs_svr(pstEnv, &stSSPkg);
	return 0;
}

int clan_global_del_mem_slow(ZONESVRENV *pstEnv, tdr_ulonglong ullRoleID)
{
	ClanObj *pstClanObj = pstEnv->pstClanObj;
	
	int i = pstClanObj->iClanNum;

	while (i--)
	{
		int rv = clan_del_mem_by_id(pstEnv, pstClanObj->astZoneClan + i,
		                            ullRoleID,1);
		if (!rv)
		{			
			return 0;
		}
	}
	return -1;
}

int clan_online_member_to_mud(ZONESVRENV *pstEnv,char *pszName,CLANID *pstID)
{
	int i = 0;
	ZoneClan* pstZoneClan = clan_find_by_wid(pstEnv,pstID->GID);
	CLANMEM * pstMem = NULL;


	CSPKG stPkg;
	CSCLANMEMBERCHG *pstMemChg = &stPkg.Body.ClanSvr.ClanSvrData.ClanMemberChg;
	
	if (!pstZoneClan)
	{
		return -1;
	}

	for(i = pstZoneClan->stClan.ClanProf.MemNum - 1;i >= 0;i--)
	{
		pstMem = &pstZoneClan->stClan.ClanCore.ClanMem[i];
		if (ROLE_GAME_ONLINE != pstMem->Online)
		{
			continue;
		}
		i--;

		pstMemChg->Code = (unsigned char)CLAN_MEMBER_CHG_MOD;
		memcpy(&pstMemChg->MemInfo, pstMem, sizeof(pstMemChg->MemInfo));

		stPkg.Body.ClanSvr.Operation = CLAN_SVR_MEMBER_CHG;
		Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
		z_pkg_to_mud(pstEnv,&stPkg, pszName,0,PKG_TRANS_PLAYER);

	}
	
	return 0;
}

int player_clan_yesterday_clan_touch_notify_broadcast(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	CSPKG stPkg;
	CSCLANSVR  *pstClanSvr = &stPkg.Body.ClanSvr;

	pstClanSvr->Operation = CLAN_SVR_YESTERDAY_CLAN_TOUCH_NOTIFY;
	//todo:
	pstClanSvr->ClanSvrData.YesterdayClanTouchNotify.Num = pstZoneClan->stClan.ClanCore.YesterdayClanTouchNum;

	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	return player_clan_broadcast(pstEnv, pstZoneClan, &stPkg, 0);
}

int player_clan_yesterday_clan_touch_notify(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSCLANSVR  *pstClanSvr = &stPkg.Body.ClanSvr;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);

	if(!pstZoneClan)
	{
		return -1;
	}

	pstClanSvr->Operation = CLAN_SVR_YESTERDAY_CLAN_TOUCH_NOTIFY;

	//todo:
	pstClanSvr->ClanSvrData.YesterdayClanTouchNotify.Num = pstZoneClan->stClan.ClanCore.YesterdayClanTouchNum;

	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	return z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
}

int clan_dyn_active_helper_info(ZONESVRENV *pstEnv, ZoneClan *pstClan)
{
	int i;	
	Player *pstPlayer = NULL;

	
	for(i = 0; i < pstClan->stClan.ClanProf.MemNum; ++i)
	{
		if (!pstClan->stClan.ClanCore.ClanMem[i].Online)
		{
			continue;
		}

		pstPlayer = z_name_player(
			pstEnv->pstAppCtx,
			pstEnv,
			pstClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName);
		if (!pstPlayer )
		{
			continue;
		}

		player_dyn_active_helper_info(pstEnv,pstPlayer,CLAN_TOUCH_ACTIVE_HELP_ID,pstClan->stZoneClanInfo.iClanTouchStatus);
	}
	return 0;
}


int player_clan_active_ui_info_clan_spy(ZONESVRENV * pstEnv,Player* pstPlayer,int iType)
{
	CSPKG stPkg;
	CSACTIVEGAMESVR *pstActiveGameSvr = &stPkg.Body.ActiveGameSvr;
	CSACTIVEGAMEUIINFO *pstActiveUIInfo = &pstActiveGameSvr->Data.UIInfo;

	if (ACTIVE_GAME_SVR_TYPE_UI_INFO == iType)
	{
		GameEventInst *pstEventSpy = z_get_game_event(pstEnv,SPY_EVENT_ID);

		ZoneClan* pstClan = player_get_clan(pstEnv,pstPlayer);

		if (!pstClan)
		{
			return 0;
		}

		if (!pstEventSpy)
		{		
			return 0;
		}

		if (!(pstEventSpy->iFlag & GAME_EVENT_FLAG_SPY))
		{
			return 0;
		}
	}
	
	memset(pstActiveGameSvr,0,sizeof(*pstActiveGameSvr));
	
	pstActiveGameSvr->Type = iType;
	pstActiveUIInfo->UIType = ACTIVE_UI_TYPE_CLAN_SPY;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTIVE_GAME_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_clan_active_ui_info_clan_machine_pworld(ZONESVRENV * pstEnv,Player* pstPlayer,int iType)
{
	/*CSPKG stPkg;
	CSACTIVEGAMESVR *pstActiveGameSvr = &stPkg.Body.ActiveGameSvr;
	CSACTIVEGAMEUIINFO *pstActiveUIInfo = &pstActiveGameSvr->Data.UIInfo;
	
	memset(pstActiveGameSvr,0,sizeof(*pstActiveGameSvr));

	if (ACTIVE_GAME_SVR_TYPE_UI_INFO == iType)
	{
		ZoneClan* pstClan = player_get_clan(pstEnv,pstPlayer);
		ZONEPWORLDINST* pstInst = NULL;
		
		if (!pstClan)
		{
			return 0;
		}

		pstInst = z_find_clan_machine_pworld_inst(pstEnv,pstClan);

		if (!pstInst || pstInst->bOver)
		{
			return 0;
		}		
	}
	
	pstActiveGameSvr->Type = iType;
	pstActiveUIInfo->UIType = ACTIVE_UI_TYPE_CLAN_MACHINE_PWORLD;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTIVE_GAME_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);*/

	CSPKG stPkg;
	CSACTIVEGAMESVR *pstActiveGameSvr = &stPkg.Body.ActiveGameSvr;
	CSACTIVEGAMEUIINFO *pstActiveUIInfo = &pstActiveGameSvr->Data.UIInfo;
	ROLEMISCINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo;

	ZONEPWORLDINST* pstInst = NULL;
	ZoneClan *pstShareClan;

	ZoneClan* pstClan = player_get_clan(pstEnv,pstPlayer);
	if (!pstClan)
	{
		return 0;
	}

	memset(pstActiveGameSvr,0,sizeof(*pstActiveGameSvr));

	pstActiveUIInfo->Data.ClanMachine.ClanID = pstClan->stClan.ClanProf.Id;
	
	if (ACTIVE_GAME_SVR_TYPE_UI_INFO == iType)
	{
		pstInst = z_find_clan_machine_pworld_inst(pstEnv,pstClan);

		if (!pstInst || pstInst->bOver)
		{
			goto shareshuniu;
		}	

		
		if (!IsSameDay(pstInfo->ShuNiuInfo.Time, pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			pstActiveUIInfo->Data.ClanMachine.Wave = pstInst->stWaveRound.iCurrRound+1;
		}
		else
		{
			if (pstInfo->ShuNiuInfo.Wid == pstInst->ullPworldWID)
			{
				pstActiveUIInfo->Data.ClanMachine.Wave = pstInst->stWaveRound.iCurrRound+1;
			}
			else
			{
				goto shareshuniu;	
			}
			
		}
	}
	
	pstActiveGameSvr->Type = iType;
	pstActiveUIInfo->UIType = ACTIVE_UI_TYPE_CLAN_MACHINE_PWORLD;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTIVE_GAME_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	
	
shareshuniu:

	pstShareClan = z_id_clan(pstEnv, pstEnv->iShuNiuClanID);
	if (!pstShareClan)
	{
		return 0;
	}

	pstActiveUIInfo->Data.ClanMachine.ClanID = pstShareClan->stClan.ClanProf.Id;
	pstActiveUIInfo->Data.ClanMachine.Flag = 1;
	if (ACTIVE_GAME_SVR_TYPE_UI_INFO == iType)
	{
		pstInst =  z_find_clan_machine_pworld_inst(pstEnv,pstShareClan);
		if (!pstInst || pstInst->bOver)
		{
			return 0;
		}	

		if (!IsSameDay(pstInfo->ShuNiuInfo.Time, pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			pstActiveUIInfo->Data.ClanMachine.Wave = pstInst->stWaveRound.iCurrRound+1;
		}
		else
		{
			if (pstInfo->ShuNiuInfo.Wid == pstInst->ullPworldWID)
			{
				pstActiveUIInfo->Data.ClanMachine.Wave = pstInst->stWaveRound.iCurrRound+1;
			}
			else
			{
				return 0;
			}
		}
	}
	
	pstActiveGameSvr->Type = iType;
	pstActiveUIInfo->UIType = ACTIVE_UI_TYPE_CLAN_MACHINE_PWORLD;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTIVE_GAME_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int clan_init_touch_status(ZONESVRENV* pstEnv)
{
	ZoneClan *astZoneClan = pstEnv->pstClanObj->astZoneClan;
	int i = pstEnv->pstClanObj->iClanNum;

	while(i--)
	{
		astZoneClan[i].stZoneClanInfo.iClanTouchStatus = 0;
		
		clan_dyn_active_helper_info(pstEnv,&astZoneClan[i]);
		
	}
	return 0;
}

int player_clan_summon_ask_check(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult)
{
	int iPosition;
	ZONEIDX stIdx;
	MapIndex * pstMapIdx = NULL;
	ZONEMAPINST *pstMapInst = NULL; 

	if (pstResult->ResultVal1 == 0)
	{
		iPosition = player_clan_position(pstEnv, pstPlayer);
		if(iPosition  != CLAN_POS_PRESIDENT && iPosition != CLAN_POS_VICE_PRESIDENT)
		{
			return -1;
		}
	}

	if(pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN294);
		return -1;
	}

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

	if(pstMapIdx->stMapDef.CtrlFlag & MAP_CTRL_NO_USER_CLAN_SUMMON)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN294);
		return -1;
	}

	if (pstMapIdx->stMapDef.EnterTime > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN294);
		return -1;
	}
	
	return 0;
}

int player_clan_summon_ask_msg(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneClan *pstZoneClan)
{
	CSPKG stPkg;
	CSCLANSVR *pstClanSvr = &stPkg.Body.ClanSvr;

	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	pstClanSvr->Operation = CLAN_SVR_SUMMON_ASK;
	pstClanSvr->ClanSvrData.SummonAsk.SummonMemID = pstPlayer->iMemID;
	pstClanSvr->ClanSvrData.SummonAsk.Map = pstPlayer->stOnline.stClanSummonInfo.Map;
	pstClanSvr->ClanSvrData.SummonAsk.X= pstPlayer->stOnline.stClanSummonInfo.X;
	pstClanSvr->ClanSvrData.SummonAsk.Y= pstPlayer->stOnline.stClanSummonInfo.Y;

	STRNCPY(pstClanSvr->ClanSvrData.SummonAsk.RoleName, 
		pstPlayer->stRoleData.RoleName, CCH(pstClanSvr->ClanSvrData.SummonAsk.RoleName));
	pstClanSvr->ClanSvrData.SummonAsk.Postion = player_clan_position(pstEnv, pstPlayer);

	pstClanSvr->ClanSvrData.SummonAsk.OverTime = pstPlayer->stOnline.stClanSummonInfo.OverTime - 
													pstEnv->pstAppCtx->stCurr.tv_sec;
	player_clan_broadcast_except(pstEnv, pstZoneClan, &stPkg, 0, pstPlayer);
	
	return 0;
}

int player_clan_summon(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult)
{
	ZoneClan* pstClan = player_get_clan(pstEnv,pstPlayer);
	ClanSummonInfo* pstInfo = &pstPlayer->stOnline.stClanSummonInfo;

	if(player_clan_summon_ask_check(pstEnv, pstPlayer, pstResult))
	{
		return -1;
	}
	
	pstInfo->Map = pstPlayer->stRoleData.Map;
	pstInfo->X = pstPlayer->stRoleData.Pos.X;
	pstInfo->Y = pstPlayer->stRoleData.Pos.Y;
	pstInfo->MapPos = pstPlayer->stOnline.iMapPos;
	pstInfo->OverTime = pstEnv->pstAppCtx->stCurr.tv_sec + CLAN_SUMMON_OVER_TIME;
	
	player_clan_summon_ask_msg(pstEnv,pstPlayer,pstClan);
	
	return 0;
}

int player_clan_summon_close_ui_msg(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSCLANSVR *pstClanSvr = &stPkg.Body.ClanSvr;

	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	pstClanSvr->Operation = CLAN_SVR_SUMMON_UI_CLOSE;
	pstClanSvr->ClanSvrData.SummonCloseUi.Res = 0;
	
	z_cltmsg_send(pstEnv,pstPlayer,&stPkg);
	return 0;
}

int player_clan_summon_answer(ZONESVRENV* pstEnv, Player *pstPlayer, CSCLANSUMMONANSWER *pstSummonAnswer)
{
	ZoneClan* pstClan = player_get_clan(pstEnv,pstPlayer);
	Player* pstSummoner = z_id_player(pstEnv->pstAppCtx,pstEnv,pstSummonAnswer->SummonMemID);
	ZoneClan* pstSummonClan = NULL;
	ClanSummonInfo* pstInfo = NULL;
	RESPOS stPos;

	if (0 == pstSummonAnswer->Yes)
	{
		//close ui
		player_clan_summon_close_ui_msg(pstEnv,pstPlayer);
		return -1;
	}
	if (!pstSummoner)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN295);
		player_clan_summon_close_ui_msg(pstEnv,pstPlayer);
		return -1;
	}
	
	pstInfo = &pstSummoner->stOnline.stClanSummonInfo;

	//if(pstSummoner->stRoleData.Map >= PWORLD_MAPID_BASE)
	//{
	//	return -1;
	//}
	
	if (!pstClan)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN296);
		player_clan_summon_close_ui_msg(pstEnv,pstPlayer);
		return -1;
	}
	
	pstSummonClan = player_get_clan(pstEnv,pstSummoner);
	if (pstClan != pstSummonClan)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN297);
		player_clan_summon_close_ui_msg(pstEnv,pstPlayer);
		return -1;
	}

	if (pstEnv->pstAppCtx->stCurr.tv_sec > pstSummoner->stOnline.stClanSummonInfo.OverTime)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN298);
		//close ui
		player_clan_summon_close_ui_msg(pstEnv,pstPlayer);
		return -1;
	}

	//状态检查
	
	if((pstPlayer->stOnline.State & CS_STAT_ATK)||
		(pstPlayer->stOnline.State & CS_STAT_MOVE_ATK) ||
		(pstPlayer->stOnline.State & CS_STAT_PREPARE) ||
		(pstPlayer->stOnline.State & CS_STAT_MOVE_PREPARE))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN299);
		player_clan_summon_close_ui_msg(pstEnv,pstPlayer);
		return -1;
	}

	//任务加了定身等效果的时候也不让传

	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOITEM))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN299);
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
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_CLAN300);		
		return -1;
	}
		
	player_goto_ignore_refer_line(pstEnv,pstPlayer, pstInfo->Map,pstInfo->X,  pstInfo->Y,pstInfo->MapPos);

	//close ui
	player_clan_summon_close_ui_msg(pstEnv,pstPlayer);
	return 0;
}

//是否在可召唤时间
int is_in_clan_call_baby_time(ZONESVRENV *pstEnv)
{
	struct tm stNow;
	char sTmp[20];
	int iCurrDate;

	localtime_r(&pstEnv->pstAppCtx->stCurr.tv_sec, &stNow);

	//周日
	if(stNow.tm_wday != 0)
	{
		return 0;
	}

	//时间格式 xx小时xx分xx秒
	strftime(sTmp, sizeof(sTmp), "%H%M%S", &stNow);
	iCurrDate = atoi(sTmp);

	if(iCurrDate >= 60000 && 
		iCurrDate <= 235900)
	{
		return 1;
	}
	
	return 0;
}

//是否在自动召唤时间内
int is_in_auto_clan_call_baby_time(ZONESVRENV *pstEnv)
{
	struct tm stNow;
	char sTmp[20];
	int iCurrDate;

	localtime_r(&pstEnv->pstAppCtx->stCurr.tv_sec, &stNow);

	//周日
	if(stNow.tm_wday != 0)
	{
		return 0;
	}

	//时间格式 xx小时xx分xx秒
	strftime(sTmp, sizeof(sTmp), "%H%M%S", &stNow);
	iCurrDate = atoi(sTmp);

	if(iCurrDate > 235900 && 
		iCurrDate <= 235959)
	{
		return 1;
	}
	
	return 0;
}


//公会是否有神兽宝宝
int clan_has_baby(ZONESVRENV* pstEnv, ZoneClan *pstClan)
{
	if(pstClan->stClan.ClanProf.Level < 3)
	{
		return 0;
	}

	return 1;
	UNUSED(pstEnv);	//add by paraunused.pl
}

//神兽宝宝每周一更新
int clan_baby_feed_update(ZONESVRENV* pstEnv, ZoneClan *pstClan)
{
	if(!is_same_week(pstClan->stClan.ClanCore.ClanBaByInfo.FeedTime, 
		pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstClan->stClan.ClanCore.ClanBaByInfo.FeedTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstClan->stClan.ClanCore.ClanBaByInfo.BaByVal = 0;
		pstClan->stClan.ClanCore.ClanBaByInfo.CallBaby = CLAN_BABY_STAT_NEW;
	}

	return 0;
}

//神兽宝宝现在的状态
int clan_baby_stat_get(ZONESVRENV* pstEnv, ZoneClan *pstClan)
{
	clan_baby_feed_update(pstEnv, pstClan);

	return pstClan->stClan.ClanCore.ClanBaByInfo.CallBaby;
}

//神兽宝宝吃东西
int clan_baby_feed_chg(ZONESVRENV* pstEnv, ZoneClan *pstClan, int iChg)
{
	clan_baby_feed_update(pstEnv, pstClan);
	
	pstClan->stClan.ClanCore.ClanBaByInfo.BaByVal += iChg;
	return 0;
}

//神兽宝宝营养值
int clan_baby_feed_get(ZONESVRENV* pstEnv, ZoneClan *pstClan)
{
	clan_baby_feed_update(pstEnv, pstClan);
	
	return pstClan->stClan.ClanCore.ClanBaByInfo.BaByVal;
}

//神兽当前品质
int clan_baby_type_get(ZONESVRENV* pstEnv, ZoneClan *pstClan)
{
	clan_baby_feed_update(pstEnv, pstClan);

	if(pstClan->stClan.ClanCore.ClanBaByInfo.BaByVal <= 100)
	{
		return 0;
	}
	else if(pstClan->stClan.ClanCore.ClanBaByInfo.BaByVal <= 2000)
	{
		return 1;
	}
	else if(pstClan->stClan.ClanCore.ClanBaByInfo.BaByVal <= 5000)
	{
		return 2;
	}
	else
	{
		return 3;
	}
	
	return 0;
}

int clan_baby_mon_id_get(ZONESVRENV* pstEnv, ZoneClan *pstClan)
{
	int iType = clan_baby_type_get(pstEnv, pstClan);
	int iLevel = get_curr_max_level(pstEnv);

	if(iLevel <= 59)
	{
		switch(iType)
		{
			case 0:
				return 208100;
				break;
			case 1:
				return 208101;
				break;
			case 2:
				return 208102;
				break;
			case 3:
				return 208103;
				break;
		}
	}
	else if(iLevel <= 69)
	{
		switch(iType)
		{
			case 0:
				return 208104;
				break;
			case 1:
				return 208105;
				break;
			case 2:
				return 208106;
				break;
			case 3:
				return 208107;
				break;
		}
	}
	else if (iLevel <= 79)
	{
		switch(iType)
		{
			case 0:
				return 208108;
				break;
			case 1:
				return 208109;
				break;
			case 2:
				return 208110;
				break;
			case 3:
				return 208111;
				break;
		}
	}
	else
	{
		switch(iType)
		{
			case 0:
				return 208112;
				break;
			case 1:
				return 208113;
				break;
			case 2:
				return 208114;
				break;
			case 3:
				return 208115;
				break;
		}
	}
	
	return 0;
}

int clan_baby_new(ZONESVRENV* pstEnv, ZoneClan *pstClan)
{
	Monster *pstMon, stMonster;
	MONSTERDEF *pstMonDef;
	ZONEMAPINST* pstMapInst;
	ZONEPWORLDINST *pstZonePworldInst;
	int iMonID = clan_baby_mon_id_get(pstEnv, pstClan);
	
	pstZonePworldInst = z_find_clan_city_inst(pstEnv, pstClan);
	if (NULL == pstZonePworldInst)
	{
		return 0;
	}

	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstZonePworldInst->astMapInstIdx[0]);
	if( NULL == pstMapInst )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_find_map_inst %d map inst fail", pstZonePworldInst->astMapInstIdx[0].iID);
		return 0;
	}
	
	memset(&stMonster, 0, sizeof(stMonster));
	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.stInitPos.X = 29600;
	stMonster.stInitPos.Y = 9700;
	stMonster.bDirection = 0;
	stMonster.stCurrPos = stMonster.stInitPos;
	stMonster.ullBootyClanGID = pstClan->stClan.ClanProf.GID;
	stMonster.nOwnerFastID = pstClan->stClan.ClanProf.Id;

	pstMonDef = z_get_mon_def(pstEnv, &stMonster);
	if (NULL == pstMonDef || !pstMonDef->ClanBooty)
	{
		return 0;
	}

	pstMon =	z_new_monster(pstEnv->pstAppCtx, pstEnv, &stMonster, pstMonDef);
	if( !pstMon )
	{
		return 0;
	}

	pstMon->cFirstBorn = 1;
	pstMon->tLifeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + 10800;
	if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstMon, NULL))
	{
		tmempool_free(pstEnv->pstMonsterPool, pstMon->iMIndex);
		return 0;
	}
	pstMon->cFirstBorn = 0;

	return 0;
}

int clan_baby_die_award(ZONESVRENV* pstEnv, ZoneClan *pstClan, Monster *pstMon)
{
	int i, iClanMoney = 0;
	int iMul = 0;
	ZONEPWORLDINST *pstZonePworldInst;
	Player *pstPlayer;
	CLANMEM *pstMem;
	int iType = clan_baby_type_get(pstEnv, pstClan);
	
	pstZonePworldInst = z_find_clan_city_inst(pstEnv, pstClan);
	if (NULL == pstZonePworldInst)
	{
		return 0;
	}

	switch(iType)
	{
		case 0:
			iClanMoney = 10000;
			iMul = 1000;
			break;
		case 1:
			iClanMoney = 20000;
			iMul = 1100;
			break;
		case 2:
			iClanMoney = 30000;
			iMul = 1150;
			break;
		case 3:
			iClanMoney = 50000;
			iMul = 1200;
			break;
		default:
			iClanMoney = 10000;
			iMul = 1000;
			break;
	}
	
	if(iClanMoney > 0)
	{
		clan_resource_inc(pstEnv, pstClan, NULL, CLAN_MONEY,
		                  iClanMoney);
	}

	for (i=0; i<pstZonePworldInst->iPlayer; i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstZonePworldInst->aiPlayerID[i]);
		if (!pstPlayer || pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID != pstClan->stClan.ClanProf.GID)
		{
			continue;
		}
			
		pstMem = clan_id_mem(pstClan, pstPlayer->stRoleData.RoleID,
	                         &pstPlayer->stOnline.iClanMemIdx);
		if (!pstMem )
		{
			continue;
		}
	
		if (pstMem->JoinTime + 86400 < pstEnv->pstAppCtx->stCurr.tv_sec )
		{
			tdr_longlong llExp;
			LEVELUP *pstLevelUP = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
			if(pstLevelUP)
			{
				llExp = (tdr_ulonglong )iMul*pstLevelUP->ContributeExp/10000.0;
				player_world_level_exp(pstEnv, pstPlayer, &llExp);
				player_add_exp(pstEnv, pstPlayer, llExp, 0);
			}
		}
	}

	return 0;
	UNUSED(pstMon);	//add by paraunused.pl
}

//神兽宝宝现身
int clan_baby_call(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iStat;
	int iPostion = player_clan_position(pstEnv, pstPlayer);
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	if(!pstClan)
	{
		return -1;
	}

	if(iPostion != CLAN_POS_PRESIDENT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN301);
		return -1;
	}

	iStat = clan_baby_stat_get(pstEnv, pstClan);
	if(iStat != CLAN_BABY_STAT_NEW)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN302);
		return -1;
	}

	if(!is_in_clan_call_baby_time(pstEnv))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN303);
		return -1;
	}

	pstClan->stClan.ClanCore.ClanBaByInfo.CallBaby = CLAN_BABY_STAT_CALL;
	clan_baby_new(pstEnv, pstClan);

	return 0;
}

int clan_baby_tick(ZONESVRENV* pstEnv, ZoneClan *pstClan)
{
	int iStat = clan_baby_stat_get(pstEnv, pstClan);
	if(iStat != CLAN_BABY_STAT_NEW)
	{
		return 0;
	}

	if(!clan_has_baby(pstEnv, pstClan))
	{
		return 0;
	}
		
	if(!is_in_auto_clan_call_baby_time(pstEnv))
	{
		return 0;
	}

	pstClan->stClan.ClanCore.ClanBaByInfo.CallBaby = CLAN_BABY_STAT_CALL;
	clan_baby_new(pstEnv, pstClan);
	player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN,
					ZONEERR_CLANBROAD246);
	return 0;
}

int player_clan_baby_award(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iStat;
	ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
	if(!pstClan)
	{
		return -1;
	}

	if(is_same_week(pstPlayer->stRoleData.MiscInfo.ClanBaByInfo.AwardTime, 
		pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN304);
		return -1;
	}

	iStat = clan_baby_stat_get(pstEnv, pstClan);
	if(iStat != CLAN_BABY_STAT_DIE)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN305);
		return -1;
	}

	pstPlayer->stRoleData.MiscInfo.ClanBaByInfo.AwardTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	return 0;
}

int clan_baby_clear_all(ZoneClan *pstClan)
{
	memset(&pstClan->stClan.ClanCore.ClanBaByInfo, 0, sizeof(pstClan->stClan.ClanCore.ClanBaByInfo));
	return 0;
}

//调整职位
int clan_officers_check(ZONESVRENV *pstEnv, ZoneClan * pstClan)
{
	int i, iViceNum;
	CLANMEM *pstMem, *pstVicePresident = NULL;

	for(i = 0; i < pstClan->stClan.ClanProf.MemNum; i++)
	{
		pstMem = &pstClan->stClan.ClanCore.ClanMem[i];
		if(pstMem->Position >= CLAN_POS_TECH_OFFICER && pstMem->Position <= CLAN_POS_WALL_OFFICER)
		{
			pstMem->Position = CLAN_POS_NORMAL;
			player_clan_mem_chg_broadcast(pstEnv, pstClan, pstMem, CLAN_MEMBER_CHG_MOD);
			clan_action(pstEnv, pstClan, pstMem);
			continue;
		}

		if(pstMem->Position == CLAN_POS_VICE_PRESIDENT)
		{
			iViceNum++;

			if(!pstVicePresident || pstMem->LastOnline > pstVicePresident->LastOnline)
			{
				pstVicePresident = pstMem;
			}
		}
	}

	if(iViceNum > 1)
	{
		for(i = 0; i < pstClan->stClan.ClanProf.MemNum; i++)
		{
			pstMem = &pstClan->stClan.ClanCore.ClanMem[i];
			if(pstMem->Position == CLAN_POS_VICE_PRESIDENT && pstMem != pstVicePresident)
			{
				pstMem->Position = CLAN_POS_NORMAL;
				player_clan_mem_chg_broadcast(pstEnv, pstClan, pstMem, CLAN_MEMBER_CHG_MOD);
				clan_action(pstEnv, pstClan, pstMem);
				continue;
			}
		}
	}

	clan_update_officers(pstEnv, pstClan);

	if(pstClan->stClan.ClanCore.ClanGroup.GroupNum == 0)
	{
		memset(&pstClan->stClan.ClanCore.ClanGroup, 0, sizeof(pstClan->stClan.ClanCore.ClanGroup));
		pstClan->stClan.ClanCore.ClanGroup.GroupNum = MAX_CLAN_GROUP_NUM;
		
	}
	
	return 0;
}

int clan_contribute_item_create(ZONESVRENV *pstEnv, int iContri, ROLEGRID *pstGrid)
{
	if(0 > item_create(pstEnv->pstAppCtx, pstEnv, pstGrid, CLAN_CONTRI_ITEM))
	{
		return -1;
	}

	pstGrid->GridData.RoleItem.InstVal1 = iContri;
	
	return 0;
}

//int clan_kick_contribute_mail(ZONESVRENV *pstEnv, char *pszClanName, char *pszRoleName, int iContri)
//{
	/*
	char szText[512];
	ROLEGRID stRoleGrid;

	if(iContri <= 0)
	{
		return -1;
	}

	if(0 > clan_contribute_item_create(pstEnv, iContri, &stRoleGrid))
	{
		return -1;
	}

	snprintf(szText, sizeof(szText), 
		"尊敬的%s您好：\n\t\t\t\t您已经被踢出[%s]公会，系统已将您剩余的个人公会贡献度折换为公会徽章。当您再次加入新的公会后，使用公会徽章就可将公会徽章内的公会贡献度转到现在的公会贡献度上。", 
		pszRoleName, pszClanName);
	
	player_sysmail_fast_send(pstEnv, pszRoleName, 
				"请离通知书：", 
				szText, 
				&stRoleGrid, 1, 0,NULL, 0);
	
	return 0;
	*/
//	return 0;
//}

int clan_is_open_dart(ZONESVRENV *pstEnv, ZoneClan *pstClan)
{
	return pstClan->stClan.ClanAdvance.DartInfo.IsOpen;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int clan_is_open_dart_by_role(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	ZoneClan *pstClan;
	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
		return 0;

	return pstClan->stClan.ClanAdvance.DartInfo.IsOpen;
}

int clan_open_dart_notify(ZONESVRENV *pstEnv, Player *pstPlayer,  int iEnd,ZoneClan *pstClan)
{
	CSPKG stPkg;
	CSCLANDART*pstNotify;

	pstNotify = &stPkg.Body.ClanSvr.ClanSvrData.ClanDart;
	stPkg.Body.ClanSvr.Operation = CLAN_SVR_DART_INFO;
	pstNotify->EndTime = iEnd;
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_SVR);
	
	if (pstPlayer)
	{
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	else
	{
		player_clan_broadcast(pstEnv, pstClan, &stPkg, 0);
	}
	return 0;
}

int clan_dart_login(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	ZoneClan *pstClan;
	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
		return 0;

	if (!clan_is_open_dart(pstEnv, pstClan))
	{
		return 0;
	}

	clan_open_dart_notify(pstEnv, pstPlayer,pstClan->stClan.ClanAdvance.DartInfo.StartTime+CLAN_DART_MIN*60,pstClan);
	return 0;
}

void clan_dart_die(ZONESVRENV *pstEnv, Player *pstPlayer, ZoneAni * pstAtker, Monster* pstMon)
{
	ZoneClan *pstClan;
	Player *pstAtk;
	ZoneClan *pstClanAtk;
	MONSTERDEF *pstDef;
	
	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
		return ;

	if (!clan_is_open_dart(pstEnv,pstClan))
	{
		return; 
	}

	pstAtk = get_atker_player(pstEnv, pstAtker);
	if (!pstAtk)
	{
		return ;
	}

	pstClanAtk = player_get_clan(pstEnv, pstAtk);
	if (!pstClanAtk)
	{
		return;
	}

	pstDef = z_get_mon_def(pstEnv, pstMon);
	if (!pstDef)
	{
		return;
	}

	player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN,
	                          ZONEERR_CLAN_DART3,
	                          pstClan->stClan.ClanProf.Name,
				pstDef->Name, pstClanAtk->stClan.ClanProf.Name);
	return ;
}

int clan_open_dart(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	CLANDARTINFO *pstInfo;
	time_t iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	int iClanPosition;
	ZoneClan *pstClan;
	struct tm *pstTm;
	time_t t = iCurr;
	
	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
		return -1;

	pstInfo  = &pstClan->stClan.ClanAdvance.DartInfo;

	iClanPosition = player_clan_position(pstEnv, pstPlayer);
	if (	iClanPosition != CLAN_POS_VICE_PRESIDENT &&
		iClanPosition != CLAN_POS_PRESIDENT)
	{
		//z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "只有会长或副会长有权限开启公会商运活动!");
		//return -1;
	}
	
	if (IsSameDay(pstInfo->StartTime, iCurr))
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_301);
		return -1;
	}

	pstTm = localtime(&t);
	if (pstTm->tm_hour >= 0 && pstTm->tm_hour <= 5)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_302);
		return -1;
	}

	pstInfo->IsOpen = 1;
	pstInfo->StartTime = iCurr;

	// 通知客户端
	clan_open_dart_notify(pstEnv, NULL, iCurr+CLAN_DART_MIN*60, pstClan);

	z_sys_strf_broadcast(pstEnv, SYS_SVR, ZONEERR_CLAN_DART1,pstClan->stClan.ClanProf.Name);
	return 0;
}

int clan_dart_close(ZONESVRENV *pstEnv, ZoneClan *pstClan)
{
	CLANDARTINFO *pstInfo = &pstClan->stClan.ClanAdvance.DartInfo;

	pstInfo->IsOpen = 0;
	clan_open_dart_notify(pstEnv, NULL, 0, pstClan);
	z_sys_strf_broadcast(pstEnv, SYS_SVR, ZONEERR_CLAN_DART2,pstClan->stClan.ClanProf.Name);
	return 0;
}

int clan_battle_award_in(ZONESVRENV *pstEnv,CLANID* pstID,int iClanWarAwardDefID,int iWin,int iMul)
{
	CLANWARAWARDDEF* pstClanWarAwardDef;
	CLANAWARDDEF* pstAward;
	ClanAwardDefTail* pstTail;
	ZoneClan *pstClan = z_id_clan(pstEnv, pstID->Id);
	int i;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	ITEMDEF *pstItemDef = NULL;

	char szTip[512] = {0};
	
	char szMoneyTip1[64] = {0};
	char szMoneyTip2[64] = {0};
	
	char *pszMoneyTip = szMoneyTip1;
	char *pszMoneyTipTmp = szMoneyTip2;	

	
	//char szExpTip[64] = {0};
	char szItemIip1[64] = {0};
	char szItemIip2[64] = {0};
	
	char *pszItemTip = szItemIip1;
	char *pszItemTipTmp = szItemIip2;
	char * pszTmp = NULL;
	
	
	if (!pstClan)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,"no clan,id=%d,name=%s",pstID->Id,pstID->Name);
		return -1;
	}
	
	pstClanWarAwardDef = z_find_clan_war_award_def(pstEnv,iClanWarAwardDefID);
	if (!pstClanWarAwardDef)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,"z_find_clan_war_award_def fail,id=%d",iClanWarAwardDefID);
		return -1;
	}
	
	pstAward = z_find_clan_award_def(pstEnv,pstClanWarAwardDef->ClanAwardDefID);
	if (!pstAward)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,"z_find_clan_award_def fail,id=%d",pstClanWarAwardDef->ClanAwardDefID);
		return -1;
	}

	if (iWin)
	{		
		z_sys_strf_broadcast(pstEnv, SYS_MIDDLE, ZONEERR_CLAN_BATTLE_WIN,pstClan->stClan.ClanProf.Name);
	}
	else
	{
		z_sys_strf_broadcast(pstEnv, SYS_MIDDLE, ZONEERR_CLAN_BATTLE_FAIL,pstClan->stClan.ClanProf.Name);
	}

	

	szTip[0] = 0;

	pstTail = z_get_clan_award_tail(pstAward);

		
	for( i = 0; i < pstTail->iPosCount; i++)
	{
		int idx = i + pstTail->iPosStart;		
		int iRealNum;
		pstAward = (CLANAWARDDEF*)pstObj->sClanAwardDef[idx];

		switch (pstAward->Type)
		{
			case CLAN_AWARD_TYPE_MONEY:
				iRealNum =  pstAward->Value * (1 + iMul/100.0);
				
				clan_property_adapt(pstEnv,pstClan,CLAN_PROPERTY_MONEY, iRealNum);
				
				pszTmp = pszMoneyTip;
				pszMoneyTip = pszMoneyTipTmp;
				pszMoneyTipTmp = pszTmp;
				
				snprintf(pszMoneyTip,sizeof(szMoneyTip1),"%s %s:%d",pszMoneyTipTmp,MONEY_TYPE_CLAN_MONEY,iRealNum);

				break;
			case CLAN_AWARD_TYPE_RESOURCE:
				iRealNum =  pstAward->Value * (1 + iMul/100.0);
				clan_resource_add_entries(pstEnv,pstClan,pstAward->ItemID,iRealNum, &iRealNum);

				pstItemDef = z_find_itemdef(pstEnv,pstAward->ItemID);
				if (pstItemDef)
				{
					pszTmp = pszItemTip;
					pszItemTip = pszItemTipTmp;
					pszItemTipTmp = pszTmp;
					
					snprintf(pszItemTip,sizeof(szItemIip1),"%s %sx%d",pszItemTipTmp,
								pstItemDef->Name,iRealNum);
				}
				break;
		
			default:
				break;
		}
	}

	if(pstClanWarAwardDef->ClanAwardTip[0] )
	{
		snprintf(szTip,sizeof(szTip),"%s%s%s",(char *)pstClanWarAwardDef->ClanAwardTip,pszMoneyTip,pszItemTip);

		player_clan_msg_broadcast_raw(pstEnv, pstClan, pstClanWarAwardDef->ClanAwardTipType,szTip);
	}
	
	return 0;
}

//异步给公会奖励
int clan_battle_award_co(ZONESVRENV *pstEnv, Player *pstPlayer,COROUTINEDATA *pstData,int iDstID)
{
	COROUTINERPC *pstRpc = &pstEnv->stCoroutineRPC;
	COROUTINEDATA *pstCoData = &pstRpc->Data;

	//第一次发起传指针,后续的调用传NULL
	if (pstData)
	{
		co_init(pstRpc);
	}

	CO_BEGIN
	{
		//分线代码
		tlog_debug(pstEnv->pstLogCat, 0, 0,"coroutine rpc begin");
		
		if (INST_MODE_BATTLE_SVR != pstEnv->iInstMode ||
			NULL == pstData)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,"not battle svr");
			return -1;
		}
		
		pstRpc->DstID = iDstID;
		
		pstRpc->Type = CO_TYPE_CLAN_BATTLE_AWARD;

		if (!pstData)
		{
			return -1;
		}
		
		pstCoData->ClanBattleAward = pstData->ClanBattleAward;
	}
	CO_YIELD(1)
	{
		//主线代码	
		if (INST_MODE_ZONE_SVR != pstEnv->iInstMode )
		{
			tlog_debug(pstEnv->pstLogCat, 0, 0,"not zone svr");
			return -1;
		}

	
		tlog_debug(pstEnv->pstLogCat, 0, 0,"coroutine rpc proc");
		
		
		pstRpc->Ret = clan_battle_award_in(pstEnv, &pstCoData->ClanBattleAward.ClanID,
											pstCoData->ClanBattleAward.ClanWarAwardDefID,
											pstCoData->ClanBattleAward.Win,
											pstCoData->ClanBattleAward.Mul);
		
		
	}
	CO_YIELD(2)
	{
		//分线代码
		if (INST_MODE_BATTLE_SVR != pstEnv->iInstMode ||
			NULL == pstData)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,"not battle svr");
			return -1;
		}
		
		tlog_debug(pstEnv->pstLogCat, 0, 0,"coroutine rpc ret=%d",pstRpc->Ret);
		if (pstRpc->Ret == 0)
		{
			//todo:log
		}
		else
		{
			
		}
		
	}
	CO_END
		

	return 0;
}



int clan_award_by_def(ZONESVRENV *pstEnv,ZoneClan * pstClan,int iAwardDefID,
						char * pszTip,int iTipType,int iErrTip,int iTest)
{
	//CLANAWARDDEF* pstDef = NULL;	//comment by paraunused.pl
	//ClanAwardDefTail* pstTail = NULL;	//comment by paraunused.pl
	//int i = 0;		//comment by paraunused.pl
	//ZoneObj *pstObj = pstEnv->pstZoneObj;	//comment by paraunused.pl
	//CLANAWARDDEF* pstAward = NULL;	//comment by paraunused.pl
	//ROLEGRID stRoleGrid[PLAYER_AWARD_ITEM_MAX];	//comment by paraunused.pl
	//int iItemNum = 0;	//comment by paraunused.pl
	//char szTip[512] = {0};	//comment by paraunused.pl
	
	//char szMoneyTip1[64] = {0};	//comment by paraunused.pl
	//char szMoneyTip2[64] = {0};	//comment by paraunused.pl
	
	//char *pszMoneyTip = szMoneyTip1;	//comment by paraunused.pl
	//char *pszMoneyTipTmp = szMoneyTip2;		//comment by paraunused.pl

	
	//char szExpTip[64] = {0};	//comment by paraunused.pl
	//char szItemIip1[64] = {0};	//comment by paraunused.pl
	//char szItemIip2[64] = {0};	//comment by paraunused.pl
	
	//char *pszItemTip = szItemIip1;	//comment by paraunused.pl
	//char *pszItemTipTmp = szItemIip2;	//comment by paraunused.pl

	//todo:
/*
	pstDef = z_find_clan_award_def(pstEnv,iAwardDefID);
	if (!pstDef)
	{
		if (iErrTip)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE2, ZONEERR_PLAYER146);
		}
		return -1;
	}

	pstTail = z_get_clan_award_tail(pstDef);

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
			
			pstAward = (CLANAWARDDEF*)pstObj->sClanAwardDef[idx];
			
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
		
		pstAward = (CLANAWARDDEF*)pstObj->sClanAwardDef[idx];
		
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
*/
	return 0;
	UNUSED(pstClan);	//add by paraunused.pl
	UNUSED(iAwardDefID);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pszTip);	//add by paraunused.pl
	UNUSED(iTipType);	//add by paraunused.pl
	UNUSED(iErrTip);	//add by paraunused.pl
	UNUSED(iTest);	//add by paraunused.pl
}
int clan_dart_check_end(ZONESVRENV *pstEnv, ZoneClan *pstClan)
{
	time_t iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	CLANDARTINFO *pstInfo = &pstClan->stClan.ClanAdvance.DartInfo;
	
	if (!clan_is_open_dart(pstEnv, pstClan))
	{
		return 0;
	}

	if (pstInfo->StartTime + CLAN_DART_MIN*60 < iCurr)
	{
		clan_dart_close(pstEnv, pstClan);
	}
	return 0;
}


//lzk   技能返回贡献度函数
int clan_exit_return_contribute(ZONESVRENV *pstEnv, Player *pstPlayer)
{	
	if (NULL == pstPlayer)
	{
		return -1;
	}

	int iContri = 0;
	int iNum = 0;
	PLAYERCLANSKILLINFO * pstPlayerClanSkill = NULL;
	pstPlayerClanSkill = &pstPlayer->stRoleData.MiscInfo.ClanSkillInfo;
	int i;
	int iSkillID = 0;
	int Level = 0;
	CLANSKILLDEF* pstDef = NULL;
	iNum = pstPlayerClanSkill->Num;

	for(i = 0; i < iNum; i++)
	{
		iSkillID = pstPlayerClanSkill->ClanSkill[i].SkillID;
		Level = pstPlayerClanSkill->ClanSkill[i].Level;
		pstDef = z_find_clan_skill_def(pstEnv,iSkillID,Level);

		if (!pstDef)
		{
			continue;
		}
		
		iContri += pstDef->ReContri;
		
	}

	if(iContri < 0)
	{
		return -1;
	}

	return iContri;
}


//lzk  退出公会发给玩家的邮件
int clan_exit_contribute_mail(ZONESVRENV *pstEnv,Player *pstPlayer,ZoneClan *pstZoneClan)
{
	char szText[512];
	ROLEGRID stRoleGrid;
	int iSumContri = 0;
	int iContri2 =0;
	int iContri = 0;

	if(NULL == pstPlayer)
	{
		return -1;
	}

	iContri = pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute;
	if(iContri < 0)
	{
		iContri = 0;
	}

	iContri2  = clan_exit_return_contribute(pstEnv,pstPlayer);
	if(iContri2 < 0)
	{
		iContri2 = 0;
	}

	iSumContri = iContri2 + iContri;
	if(iSumContri <= 0)
	{
		return -1;
	}

	if(0 > clan_contribute_item_create(pstEnv, iSumContri , &stRoleGrid))
	{
		return -1;
	}

	if(pstZoneClan)
	{
		snprintf(szText, sizeof(szText), LJY_MAIL56 ,pstPlayer->stRoleData.RoleName, 
	               pstZoneClan->stClan.ClanProf.Name,iContri,iContri2,iSumContri);
	}
	else
	{
		//以防万一，没有公会名
		snprintf(szText, sizeof(szText), LJY_MAIL58 ,pstPlayer->stRoleData.RoleName, 
	               iContri,iContri2,iSumContri);
	}
	
	player_sysmail_fast_send(pstEnv, pstPlayer->stRoleData.RoleName, LJY_MAIL57 ,
		szText, &stRoleGrid, 1, 0,NULL, 0);

	return 0;
}



//lzk   公会技能为成员人数，修改公会人数上限
int clan_skill_mem_limit_chg(ZONESVRENV *pstEnv,ZoneClan *pstZoneClan,CSCLANSKILLRESEARCH *pstCSClanSkillResearch)
{
	CLANSKILLDEF* pstDef = NULL;
	CSCLANSKILLRESEARCH * pstReq = pstCSClanSkillResearch;
	CLANCORE *pstClanCore = &pstZoneClan->stClan.ClanCore;
	int i;
      
	pstDef = z_find_clan_skill_def(pstEnv,pstReq->SkillID, pstReq->Level);

	if (!pstDef)
	{
		return -1;		
	}
	
	for(i = 0; i < MAX_CLAN_SKILL_RESULT;i++)
	{
		if (!pstDef->Result[i].ResultID)
		{
			break;
		}
		if (pstDef->Result[i].ResultID == RESULT_CLAN_MEM_NUM_TOP)
		{
			//修改公会人数上限
			pstClanCore->MemNumTop = pstDef->Result[i].ResultVal1;
		}
	}

	return 0;
}


//lzk  公会技能为公会商运，最终获取经验加成修正
int clan_skill_business_exp_chg(ZONESVRENV *pstEnv,ZoneClan *pstZoneClan,CSCLANSKILLRESEARCH *pstCSClanSkillResearch)
{
	CLANSKILLDEF* pstDef = NULL;
	CSCLANSKILLRESEARCH * pstReq = pstCSClanSkillResearch;
       CLANDARTINFO *pstDartInfo = &pstZoneClan->stClan.ClanAdvance.DartInfo;
	int i;
      
	pstDef = z_find_clan_skill_def(pstEnv,pstReq->SkillID, pstReq->Level);

	if (!pstDef)
	{
		return -1;		
	}
	
	for(i = 0; i < MAX_CLAN_SKILL_RESULT;i++)
	{
		if (!pstDef->Result[i].ResultID)
		{
			break;
		}
		if (pstDef->Result[i].ResultID ==  RESULT_CLAN_BUSINESS_EXP)
		{
			pstDartInfo->ClanBusinessExpMul = pstDef->Result[i].ResultVal1;
		}
	}

	return 0;
}


//lzk   数据兼容，公会技能重置，玩家获得贡献度
int clan_player_skill_reset_return_contri(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	if (NULL == pstPlayer)
	{
		return -1;
	}
	
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;
	CLANMEM *pstKey = NULL;

	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	if(!pstZoneClan)
	{
		return -1;
	}

	if (!(pstKey = clan_id_mem(pstZoneClan ,pstPlayer->stRoleData.RoleID, NULL)))
	{
		return -1;
	}

	if (!(pstMiscInfo->Flag & PLAYER_CLAN_SKILL_RESET))
	{
		OPCLANDATA stData;
		unsigned int iContri =0;
		unsigned int iOldContri = 0;
		int iNum = 0;
		PLAYERCLANSKILLINFO * pstPlayerClanSkill = NULL;
		pstPlayerClanSkill = &pstPlayer->stRoleData.MiscInfo.ClanSkillInfo;
		int i;	
		int k;	
		iNum = pstPlayerClanSkill->Num;

		for(i = 0; i < iNum; i++)
		{
			if (1 == pstPlayerClanSkill->ClanSkill[i].Level)
			{
				iContri += 200;	
			}
			else if(2 == pstPlayerClanSkill->ClanSkill[i].Level)
			{
				iContri += 1500;	
			}
			else if(3 == pstPlayerClanSkill->ClanSkill[i].Level)
			{
				iContri += 2500;	
			}
			else if(4 == pstPlayerClanSkill->ClanSkill[i].Level)
			{
				iContri += 5000;	
			}
			else if(5 == pstPlayerClanSkill->ClanSkill[i].Level)
			{
				iContri += 15000;	
			}
			else
			{
				return -1;
			}
		}

		if(iContri <= 0)
		{
			//防止一开始所有技能为0级，后面又升级进入该函数
			pstMiscInfo->Flag |= PLAYER_CLAN_SKILL_RESET;
			return -1;
		}
		
		iOldContri = pstKey->Contribute;
		pstKey->Contribute += iContri;
		if(pstKey->Contribute >= MAX_CLAN_OWNED_CONTRIBUT)
		{
			pstKey->Contribute = MAX_CLAN_OWNED_CONTRIBUT;
		}

		pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute = pstKey->Contribute;
		player_clan_mem_chg_broadcast(pstEnv, pstZoneClan, pstKey ,
			                              CLAN_MEMBER_CHG_MOD);

		//外网玩家已经学会的公会技能需要将技能等级重置为0
		for(k = 0; k < iNum; k++)
		{
			stData.ClanSkillResetReturnContri.PlayerClanSkillInfo.ClanSkill[k].Level  = pstPlayerClanSkill->ClanSkill[k].Level ;
			stData.ClanSkillResetReturnContri.PlayerClanSkillInfo.ClanSkill[k].SkillID = pstPlayerClanSkill->ClanSkill[k].SkillID ;
			stData.ClanSkillResetReturnContri.PlayerClanSkillInfo.ClanSkill[k].ClanSkillIdx = pstPlayerClanSkill->ClanSkill[k].ClanSkillIdx ;

			memset(&pstPlayerClanSkill->ClanSkill[k],0,sizeof(pstPlayerClanSkill->ClanSkill[k]));
		}
		pstMiscInfo->Flag |= PLAYER_CLAN_SKILL_RESET;
		
		stData.ClanSkillResetReturnContri.PlayerClanSkillInfo.Num = pstPlayerClanSkill->Num;
		pstPlayerClanSkill->Num = 0;
		stData.ClanSkillResetReturnContri.PlayerPreContri = iOldContri;
		stData.ClanSkillResetReturnContri.ReturnContri = iContri;

		z_role_clan_oplog(pstEnv,pstPlayer->stRoleData.RoleName , clan_id_warp(&pstZoneClan->stClan),
		OP_CLAN_SKILL_RESET_RETURN_CONTRI,&stData);

		//原来就有的函数: 玩家身上公会技能变化后的通知
		player_action_player_clan_skill_info(pstEnv,pstPlayer, &pstPlayer->stRoleData.MiscInfo.ClanSkillInfo);
	}
		
	return 0;
}

//lzk   数据兼容，公会技能重置，公会获得资金
int clan_skill_reset_return_money(ZONESVRENV *pstEnv,ZoneClan *pstZoneClan)
{	
	unsigned int llmoney = 0;
	CLANSKILLINFO * pstInfo = NULL;
	CLANPROF *pstClanProf = NULL;    
	int i;
	int k;
	unsigned int uiOldMoney = 0;  
	OPCLANDATA stData;
	int iNum = 0;
 
	if(!pstZoneClan)
	{
			return -1;
	}

	pstClanProf = &pstZoneClan->stClan.ClanProf;

	if(!(pstClanProf->Flag & CLAN_FLAG_SKILL_RESET_RETURN_CLAN_MONEY))
	{
		pstInfo = &pstZoneClan->stClan.ClanAdvance.ClanSkillInfo;
		iNum = pstInfo->Num;
		for(i = 0; i < iNum; i++)
		{
			if (1 == pstInfo->ClanSkill[i].Level)
			{
				llmoney += 100000;
			}
			else if(2 == pstInfo->ClanSkill[i].Level)
			{
				llmoney += 750000;	
			}
			else if(3 == pstInfo->ClanSkill[i].Level)
			{
				llmoney += 1250000;	
			}
			else if(4 == pstInfo->ClanSkill[i].Level)
			{
				llmoney += 2500000;	
			}
			else if(5 == pstInfo->ClanSkill[i].Level)
			{
				llmoney += 7500000;	
			}
			else
			{
				return -1;
			}

		}

		if(llmoney == 0)
		{
			//所有公会技能都为0级的情况下
			pstInfo->Num = 0;

			//成员人数公会技能
			pstInfo->Num++;
			pstInfo->ClanSkill[0].Level = pstClanProf->Level*10;
			pstInfo->ClanSkill[0].SkillID = MEM_NUM_CLAN_SKILL_ID;
			pstInfo->ClanSkill[0].ValidLevel = pstClanProf->Level*10;
		
			pstClanProf->Flag |= CLAN_FLAG_SKILL_RESET_RETURN_CLAN_MONEY;
			clan_property_modify(pstEnv, pstZoneClan,CLAN_PROPERTY_FLAG, pstClanProf->Flag ,-1);

			clan_action_clan_skill_info(pstEnv, pstZoneClan);
			return 0;
		}
	
		uiOldMoney = pstClanProf->Money;
		pstClanProf->Money += llmoney;
		//溢出了
		if(pstClanProf->Money < uiOldMoney)
		{
			pstClanProf->Money = uiOldMoney;
		}

		//大于最大值，自然会处理
		clan_property_modify(pstEnv, pstZoneClan, CLAN_PROPERTY_MONEY, 
			                     pstClanProf->Money , -1);
		
		for(k = 0; k < iNum; k++)
		{
			stData.ClanSkillResetReturnMoney.ClanSkillInfo.ClanSkill[k].Level = pstInfo->ClanSkill[k].Level ;
			stData.ClanSkillResetReturnMoney.ClanSkillInfo.ClanSkill[k].SkillID = pstInfo->ClanSkill[k].SkillID;
			stData.ClanSkillResetReturnMoney.ClanSkillInfo.ClanSkill[k].ValidLevel = pstInfo->ClanSkill[k].ValidLevel;

			memset(&pstInfo->ClanSkill[k],0,sizeof(pstInfo->ClanSkill[k]));

		}
		
		pstClanProf->Flag |= CLAN_FLAG_SKILL_RESET_RETURN_CLAN_MONEY;
		clan_property_modify(pstEnv, pstZoneClan,CLAN_PROPERTY_FLAG, pstClanProf->Flag ,-1);

		stData.ClanSkillResetReturnMoney.ClanSkillInfo.Num = pstInfo->Num;
		pstInfo->Num = 0;

		//成员人数公会技能
		pstInfo->Num++;
		pstInfo->ClanSkill[0].Level = pstClanProf->Level*10;
		pstInfo->ClanSkill[0].SkillID = MEM_NUM_CLAN_SKILL_ID;
		pstInfo->ClanSkill[0].ValidLevel = pstClanProf->Level*10;
		
		stData.ClanSkillResetReturnMoney.ClanPreMoney = uiOldMoney;
		stData.ClanSkillResetReturnMoney.ClanNowMoney = pstClanProf->Money;

		z_role_clan_oplog(pstEnv,NULL, clan_id_warp(&pstZoneClan->stClan),
		OP_CLAN_SKILL_RESET_RETURN_MONEY ,&stData);

		clan_action_clan_skill_info(pstEnv, pstZoneClan);

	}
	
	return 0;
}

//lzk   拉回公会列表时，成员人数限制检验
int clan_mem_num_top_jian_yan(ZONESVRENV *pstEnv,ZoneClan *pstZoneClan)
{
	if(!pstZoneClan)
	{
		return -1;
	}
	
	CLANSKILLDEF* pstDef = NULL;
	CLANCORE *pstClanCore = &pstZoneClan->stClan.ClanCore;
	int i;
	CLANSKILLINFO * pstInfo = NULL;
	int iNum;
	int k;
	int Level;
	
	pstInfo = &pstZoneClan->stClan.ClanAdvance.ClanSkillInfo;
	iNum = pstInfo->Num;
	for(k=0;k<iNum;k++)
	{
		if(MEM_NUM_CLAN_SKILL_ID == pstInfo->ClanSkill[k].SkillID)
		{
			Level = pstInfo->ClanSkill[k].Level;
			break;
		}
	}
	
	if(k>=iNum)
	{
		return -1;
	}
			
	pstDef = z_find_clan_skill_def(pstEnv,MEM_NUM_CLAN_SKILL_ID, Level);

	if (!pstDef)
	{
		return -1;		
	}
	
	for(i = 0; i < MAX_CLAN_SKILL_RESULT;i++)
	{
		if (!pstDef->Result[i].ResultID)
		{
			break;
		}
		if (pstDef->Result[i].ResultID == RESULT_CLAN_MEM_NUM_TOP)
		{
			//修改公会人数上限
			pstClanCore->MemNumTop = pstDef->Result[i].ResultVal1;
		}
	}

	return 0;
}


int clan_skill_business_exp_jian_yan(ZONESVRENV *pstEnv,ZoneClan *pstZoneClan)
{
	if(!pstZoneClan)
	{
		return -1;
	}
	
	CLANSKILLDEF* pstDef = NULL;
    CLANDARTINFO *pstDartInfo = &pstZoneClan->stClan.ClanAdvance.DartInfo;
	int i;
	CLANSKILLINFO * pstInfo = NULL;
	int iNum;
	int k;
	int Level;
	
	pstInfo = &pstZoneClan->stClan.ClanAdvance.ClanSkillInfo;
	iNum = pstInfo->Num;
	for(k=0;k<iNum;k++)
	{
		if(BUSINESS_CLAN_SKILL_ID == pstInfo->ClanSkill[k].SkillID)
		{
			Level = pstInfo->ClanSkill[k].ValidLevel;
			break;
		}
	}
	
	if(k>=iNum)
	{
		return -1;
	}
			
	pstDef = z_find_clan_skill_def(pstEnv,BUSINESS_CLAN_SKILL_ID, Level);

	if (!pstDef)
	{
		return -1;		
	}
	
	for(i = 0; i < MAX_CLAN_SKILL_RESULT;i++)
	{
		if (!pstDef->Result[i].ResultID)
		{
			break;
		}
		if (pstDef->Result[i].ResultID ==  RESULT_CLAN_BUSINESS_EXP)
		{
			pstDartInfo->ClanBusinessExpMul = pstDef->Result[i].ResultVal1;
		}
	}

	return 0;
}

