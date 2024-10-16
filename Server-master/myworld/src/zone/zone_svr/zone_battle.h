#ifndef ZONE_BATTLE_H
#define ZONE_BATTLE_H

#include "zone_svr.h"

extern int g_ApplyIng;

BATTLEDEF *z_find_battledef(ZONESVRENV* pstEnv, int iBattleID);

//队列TICK
int z_pworld_queue_tick(ZONESVRENV* pstEnv);

//逃跑BUFF
int z_add_runaway_buff(ZONESVRENV* pstEnv, Player* pstPlayer);

//是否是一般的战场(BattleDef的都是)
int is_normal_battle_pworld(ZONEPWORLD *pstZonePworld);

//是否是战场
int is_battle_pworld(ZONEPWORLD *pstZonePworld);

//是否是队伍报名的战场
int is_battle_team_queue(ZONESVRENV* pstEnv, ZONEPWORLD *pstZonePworld);

//是否在一个结束的战场里
int is_in_over_battle_pworld(ZONESVRENV* pstEnv, Player *pstPlayer);


//获得指定副本的报名队列
PworldApplyQueue *get_pworld_apply_queue(ZONESVRENV* pstEnv, int iPworldID);

//获得指定副本的等待队列
PworldWaitQueue *get_pworld_wait_queue(ZONESVRENV* pstEnv, int iWaitQueueIdx, int iPworldInstID);


//退出副本排队、等待队列
int z_exit_pworld_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID, int fNotify);

int z_get_free_wait_queue_idx(ZONESVRENV* pstEnv);

int z_get_free_apply_queue_idx(ZONESVRENV* pstEnv);
	
RolePworldQueue *z_get_player_online_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID);

int z_player_apply_pworld_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID);

int z_player_online_remove_queue(ZONESVRENV* pstEnv, Player *pstPlayer);

int z_player_remove_wait_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iWaitIdx);

//跨服战场:进入邀请队列
int invite_player_enter_wait_queue(ZONESVRENV* pstEnv, ZONEPWORLD *pstPworld, 
			PworldApplyQueue* PworldApplyQueue, int iWaitIdx);

// 副本排队
int z_player_pworld_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID, CSJOINQUEUE *pstJoin);

//清理排队队列
int z_clear_pworld_apply_queue(ZONESVRENV* pstEnv, PworldApplyQueue *pstApplyQueue);

//清理邀请队列
int z_clear_pworld_wait_queue(ZONESVRENV* pstEnv, int iWaitIdx, int iPworldInstID);
	
//副本报名队列检查
int z_pworld_apply_queue_check(ZONESVRENV * pstEnv, PworldApplyQueue * pstApplyQueue, int iImmeCreateSwitch);

//设置进入战场的信息
int player_set_battle_info(ZONESVRENV * pstEnv, Player *pstPlayer, MapIndex *pstMapIndex,
	ZONEPWORLD *pstZonePworld, ZONEPWORLDINST *pstZonePworldInst, RESPOS *pstPos);

//战场每日进入次数
int battle_limit_info_get(ZONESVRENV* pstEnv, Player* pstPlayer);

//增加战场每日进入次数
void player_insert_limit_battle(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLD * pstZonePworld);

//战场进入次数检查
int z_check_battle_enter_num(ZONESVRENV * pstEnv, Player *pstPlayer, ZONEPWORLD * pstZonePworld, int iNotify);

//队伍报名类副本检查
int battle_check_apply_player(ZONESVRENV *pstEnv,Player *pstPlayer, Player** ppstPlayer, int *iPlayer,ZONEPWORLD *pstZonePworld);

//队伍报名类副本 报名
int z_player_apply_battle_pworld_queue(ZONESVRENV* pstEnv, Player *pstPlayer, 
											Player ** ppstPlayer, int iPlayer,
											ZONEPWORLD *pstZonePworld,
											int iTeamJoinType,
											int iSrcMaxLevel);


// 战场报名
int z_player_normal_battle_queue(ZONESVRENV* pstEnv, Player *pstPlayer, CSJOINQUEUE *pstJoin);

// iFlag==0 自己    1--所有人未排序信息    2--所有人排序信息
// Vcamp表示胜利方正营
int normal_battle_svr_info(ZONESVRENV* pstEnv,Player* pstPlayer, ZONEPWORLD *pstPworld,
							ZONEPWORLDINST	 *pstPworldInst,	int iFlag, int iVCamp);


//暂离状态
int leave_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

//暂离ACTION
int player_leave_action(ZONESVRENV* pstEnv, Player *pstPlayer);

//分配积分
int player_aoshan_battle_assign_fraction(ZONESVRENV* pstEnv, ZONEPWORLD *pstZonePworld, 
	ZONEPWORLDINST *pstPworldInst, Player *pstPlayer);

int battle_limit_win_add(Player *pstPlayer, int iBattleID);
int battle_limit_win_get(Player *pstPlayer, int iBattleID);

int is_same_camp_player(Player *pstPlayer1, Player *pstPlayer2);

int battle_enter_up_time_limit(ZONESVRENV* pstEnv,Player *pstPlayer, int fNotify);

int player_battle_limit_time_action(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_spansvr_battle_apply(ZONESVRENV* pstEnv, Player *pstPlayer,  CSJOINQUEUE *pstJoin, ZONEPWORLD *pstZonePworld);

int player_spansvr_battle_join_queue(ZONESVRENV * pstEnv, SSBATTLEAPPLYQUEUE *pstJoin);

int is_valid_aoshan_battle_award_player(ZONESVRENV* pstEnv, CSBATTLEAOSHANINFO *pstRoleInfo, int iIdx, int iMaxRole);

int is_valid_lift_battle_award_player(ZONESVRENV* pstEnv, CSBATTLESURVIVALINFO *pstRoleInfo, int iIdx, int iMaxRole);

int battle_pworld_tick(ZONESVRENV * pstEnv, ZONEPWORLD *pstPworld, ZONEPWORLDINST *pstPworldInst, int iSec);

int player_commit_xukong_energy(ZONESVRENV * pstEnv, Player *pstPlayer);

int is_has_xukong_energy(Player *pstPlayer);

int player_collect_xukong_energy(ZONESVRENV * pstEnv, Player *pstPlayer);

int player_xukong_energy_clear(ZONESVRENV * pstEnv, Player *pstPlayer);
int player_clan_war_battle_add_fraction(ZONEPWORLDINST *pstPworldInst, Player *pstPlayer, int iFraction);
PworldClanQueue *get_pworld_clan_queue_by_id(ZONESVRENV* pstEnv, int iClanQueueIdx, int iPworldID);
int player_pworld_clan_queue_all_match_stat(ZONESVRENV *pstEnv, ZONEPWORLD *pstPworld,							
							PworldClanQueue* pstClanQueue);
int z_battle_heart_beat(ZONESVRENV *pstEnv,BATLLEHEARTBEAT *pstBatlleHeartBeat);
int z_player_exit_pworld_queue(ZONESVRENV* pstEnv, Player *pstPlayer, CSEXITPWORLDQUEUE *pstCSExit);

int z_player_level_remain_tm_check(ZONESVRENV* pstEnv, Player *pstPlayer,BATTLEDEF *pstBattleDef);


#endif	/*ZONE_BATTLE_H */

