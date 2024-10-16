
#ifndef ZONE_TASK_H
#define ZONE_TASK_H

#include "zone_svr.h"

#define TASK_SHARING_ACCEPT_TIME 60 // 共享任务的最长时间
#define TASK_SHARING_ACCEPT_DIST 2000 // 共享任务的最长距离
#define TASK_CYCLE_DROP_COOL 5


#define SJ_MAX_SUIPIAN_WEEK 50

TASKRUNONE *task_run_find(Player *pstPlayer, int iTaskID);
int task_opt_str(ZONESVRENV *pstEnv, Npc *pstNpc, Player *pstPlayer, char *pszOptStr, int iOptStr, int iTaskID);
TASKDEF *task_def_find(ZONESVRENV *pstEnv, int iTaskID, int *piTaskIdx);

TASKRECYCLEDEF *taskcycle_def_find(ZONESVRENV *pstEnv, int iTaskCycleID, int *piTaskCycleIdx);

int player_task(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int task_chg(ZONESVRENV *pstEnv, Player *pstPlayer, TASKRUNONE *pstTaskRunOne);
int task_tick(ZONESVRENV *pstEnv, Player *pstPlayer);
//int task_trace_mon(ZONESVRENV *pstEnv, Player *pstPlayer, int iMonID);
int task_trace_mon(ZONESVRENV *pstEnv, Player *pstPlayer, Monster *pstMon);
int task_check_trace(ZONESVRENV *pstEnv, Player *pstPlayer);
int task_check_fini(ZONESVRENV *pstEnv, Player *pstPlayer, TASKRUNONE *pstTaskRunOne, TASKDEF *pstTaskDef);
int task_option(ZONESVRENV *pstEnv, Player *pstPlayer, int iNPCID, int iTaskID);
int task_dialog(ZONESVRENV *pstEnv, Player *pstPlayer, int iNPCID, int iTaskID, int iDialogType);
int taskcycle_option(ZONESVRENV *pstEnv, Player *pstPlayer, int iNPCID, int iTaskCycleID);
int task_accept(ZONESVRENV *pstEnv, Player *pstPlayer, int iTaskID);
int task_accept_check(ZONESVRENV *pstEnv, Player *pstPlayer, TASKDEF *pstTaskDef, int fDoNotNotify, int fTeamNotify);
int task_fini(ZONESVRENV *pstEnv, Player *pstPlayer, int iTaskID, int iNotify);
int task_find_fini(Player *pstPlayer, int iTaskID);
int gm_task_commit(ZONESVRENV* pstEnv, Player *pstPlayer, TASKRUNONE *pstTaskRunOne, TASKDEF *pstTaskDef);
int gm_task_accept(ZONESVRENV *pstEnv, Player *pstPlayer, int iTaskID);
int task_info_get(ZONESVRENV* pstEnv, Player *pstPlayer);

int TrustHashCmp( const void * pv1 , const void *pv2 );
unsigned int TrustHashCode(const void* pvData);

int player_trust(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_trust_order_list( ZONESVRENV* pstEnv, Player *pstPlayer );

int trust_buddyval_check(unsigned short unBuddyVal, unsigned char ucBuddyGrade);

int task_special_fini(ZONESVRENV *pstEnv, Player *pstPlayer, TASKDEF *pstTaskDef, TASKRUNONE *pstTaskRunOne);

int task_player_trace(ZONESVRENV *pstEnv, Player *pstPlayer, int iTraceType);

int task_pet_trace(ZONESVRENV *pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, int iTraceType);

TASKRUNONE *task_add_taskrun(ZONESVRENV *pstEnv, Player *pstPlayer, TASKDEF *pstTaskDef);

int task_arm_trace(ZONESVRENV *pstEnv, Player *pstPlayer, ROLEGRID *pstArmGrid, int iTraceType);

int task_fini_trace(ZONESVRENV *pstEnv, Player *pstPlayer, unsigned int iTaskID);

int taskcycle_fini_trace(ZONESVRENV *pstEnv, Player *pstPlayer, unsigned int iTaskCycleID);

int taskcycle_opt_str(ZONESVRENV *pstEnv, Npc *pstNpc, Player *pstPlayer, char *pszOptStr, int iOptStr, int iTaskCycleID);

int task_hit_monster_handler(ZONESVRENV* pstEnv, Player *pstPlayer,
                             ZoneSkill *pstZoneSkill, Monster *pstMonster);
int gm_task_add(ZONESVRENV *pstEnv, Player *pstPlayer, int iTaskID);

int get_taskiteam_idx(TASKDEF *pstTaskDef, unsigned int uiItemID);

int task_share_check( ZONESVRENV* pstEnv, TASKDEF * pstTaskDef, Player *pstPlayer, Player * pstAcceptor, int fCheckDist );

int is_convoy_task(TASKDEF *pstTaskDef);

int z_team_convoy_task_fail(ZONESVRENV *pstEnv, Player *pstPlayer);

GROUPONEDAYINFO *group_find_oneday(Player *pstPlayer, int iGroupID);

int group_add_oneday(ZONESVRENV *pstEnv, Player *pstPlayer, int iGroupID);

int z_convoy_task_check(ZONESVRENV *pstEnv, Player *pstPlayer);

int task_award_mail(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEGRID *pstGrid, int iGridNum);

void z_clear_convoy_info(Player *pstPlayer);

int z_check_task_script(ZONESVRENV* pstEnv, TASKDEF  *pstTaskDef);

int z_convoy_task_fail(ZONESVRENV *pstEnv, Player *pstPlayer);

int task_fairy_trace(ZONESVRENV *pstEnv, Player *pstPlayer, ROLEFAIRY *pstRoleFairy, int iTraceType);

int task_hit_player_trace(ZONESVRENV *pstEnv, Player *pstPlayer, Player *pstAtkee);

int task_info_all(ZONESVRENV* pstEnv, Player *pstPlayer);

// 任务检查返回代码
typedef enum tagTaskAcceptError
{
	TAE_OK = 0,             // 检查成功成功
	TAE_DEAD,               // 已死
	TAE_NO_CYCLE,           // 无环组任务数据
	TAE_MUTEX,              // 互斥任务
	TAE_PACKAGE_FULL,       // 背包已满
	TAE_MONEY_FULL,         // 钱包已满
	TAE_EXP_LIMIT,          // 经验限制
	TAE_NEED_TEAM,          // 需要组队
	TAE_REPEATED,           // 重复接接取
	TAE_TASK_FULL,          // 任务已满
	TAE_NOT_ENOUGH_MONEY,   // 无足够金钱
	TAE_CYCLE_ERROR,        // 环组任务接取错误
	TAE_CAREER_ERROR,       // 职业错误
	TAE_GENDER_ERROR,       // 性别错误
	TAE_NOT_ENOUGH_RESPACT, // 无足够声望值
	TAE_NEED_PRETASK,       // 需要完成前置任务
	TAE_TASK_REDO,          // 任务不能重复做
	TAE_FINI_PER_DAY,       // 每日完成任务限制
	TAE_LIMIT_GROUP,        // 组限制
	TAE_LEVEL_LIMIT,        // 任务等级限制
	TAE_LEVEL_ONE,          // 任务等级限制且只相差一级
	TAE_INTERNAL_ERROR,     // 内部错误
	TAE_EVERYDAY_EXP_LIMIT, // 每日经验限制
	TAE_SECONDARYSKILL_LIMIT,
	TAE_CLAN_LIMIT,         // 公会限制
	TAE_BIANSHEN_LIMIT,		// 变身限制
	TAE_CLAN_LEVEL_LIMIT,		// 公会等级限制
}TaskAcceptError;


int task_generic_progress_run(ZONESVRENV *pstEnv, Player *pstPlayer, int iTaskID,
                              int iValueAdd);

int task_beatk_player_trace(ZONESVRENV *pstEnv, Player *pstPlayer);

int task_add_fini(Player *pstPlayer, int iTaskID);

int task_add_oneday(ZONESVRENV *pstEnv, Player *pstPlayer, int iTaskID);
int group_add_oneday(ZONESVRENV *pstEnv, Player *pstPlayer, int iGroupID);

int task_run_del(ZONESVRENV* pstEnv, Player *pstPlayer, int iTaskID);
//int del_clan_task(ZONESVRENV *pstEnv, Player *pstPlayer);
int sec_trans_char(int iSec, char *pszTime, int iLen);
int del_secondaryskill_task(ZONESVRENV *pstEnv, Player *pstPlayer, int iSkillType);

int del_task_clan(ZONESVRENV *pstEnv, Player *pstPlayer);

int taskcycle_run_del(Player *pstPlayer, int iTaskCycleID);
int del_taskrecycle_lvlup(ZONESVRENV *pstEnv, Player *pstPlayer);
int player_check_clan(ZONESVRENV *pstEnv, Player *pstPlayer);

int task_login_check_trace(ZONESVRENV *pstEnv,Player *pstPlayer);
int task_drop_check(ZONESVRENV *pstEnv, Player *pstPlayer,TASKDEF *pstTaskDef);

int task_player_repair_trace(ZONESVRENV *pstEnv, Player *pstPlayer);

int task_commit(ZONESVRENV* pstEnv, Player *pstPlayer, TASKRUNONE *pstTaskRunOne, TASKDEF *pstTaskDef);

int task_pworld_fini_trace(ZONESVRENV *pstEnv, Player *pstPlayer, unsigned int iPworldID);

int task_use_item_fini_trace(ZONESVRENV *pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef);

int task_bat_fini_trace(ZONESVRENV *pstEnv, Player *pstPlayer, unsigned int iBattleID);

int task_online_time_trace(ZONESVRENV *pstEnv, Player *pstPlayer,unsigned int iOnlineTime);

int task_fairy_star_trace(ZONESVRENV *pstEnv, Player *pstPlayer,unsigned int iStar);

int task_bct_layer_trace(ZONESVRENV *pstEnv, Player *pstPlayer,unsigned int iLayer);

int task_clan_gift_trace(ZONESVRENV *pstEnv, Player *pstPlayer);

int task_cangbaotu_fini_trace(ZONESVRENV *pstEnv, Player *pstPlayer, unsigned int iItemID);

int task_clan_everyday_trace(ZONESVRENV *pstEnv, Player *pstPlayer);
int task_award_spar(ROLEITEM *pstItem);
int task_fail_by_dartcar_die(ZONESVRENV *pstEnv, Monster *pstMon);
int task_login_check_dart_car(ZONESVRENV *pstEnv, Player *pstPlayer);
TASKRUNONE *player_get_dart_car_task(ZONESVRENV *pstEnv, Player *pstPlayer);

TASKRECYCLEONE *taskcycle_run_find(Player *pstPlayer, int iTaskCycleID);
TASKCYCLEONEDAYINFO *taskcycle_find_oneday(Player *pstPlayer, int iTaskCycleID);
int taskcycle_add_oneday(ZONESVRENV* pstEnv, Player *pstPlayer, int iTaskCycleID);
int taskcycle_accept_check(ZONESVRENV *pstEnv, Player *pstPlayer, TASKRECYCLEDEF *pstTaskCycleDef,
								int fDoNotNotify);
int task_liuyi_fini_trace(ZONESVRENV *pstEnv, Player *pstPlayer);

int task_player_sparta_fini_trace(ZONESVRENV *pstEnv, Player *pstPlayer);

int task_paoshang_fini(ZONESVRENV *pstEnv, Player *pstPlayer,int YinPiao);

int task_za_charm_fini(ZONESVRENV *pstEnv, Player *pstPlayer,CHARMREQ *pstCharmReq);

int task_sjstar_trace(ZONESVRENV *pstEnv, Player *pstPlayer);

int task_npcbuy_trace(ZONESVRENV *pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef);

#endif /* ZONE_TASK_H */
