#ifndef ZONE_BATTLE_H
#define ZONE_BATTLE_H

#include "zone_svr.h"

extern int g_ApplyIng;

BATTLEDEF *z_find_battledef(ZONESVRENV* pstEnv, int iBattleID);

//����TICK
int z_pworld_queue_tick(ZONESVRENV* pstEnv);

//����BUFF
int z_add_runaway_buff(ZONESVRENV* pstEnv, Player* pstPlayer);

//�Ƿ���һ���ս��(BattleDef�Ķ���)
int is_normal_battle_pworld(ZONEPWORLD *pstZonePworld);

//�Ƿ���ս��
int is_battle_pworld(ZONEPWORLD *pstZonePworld);

//�Ƿ��Ƕ��鱨����ս��
int is_battle_team_queue(ZONESVRENV* pstEnv, ZONEPWORLD *pstZonePworld);

//�Ƿ���һ��������ս����
int is_in_over_battle_pworld(ZONESVRENV* pstEnv, Player *pstPlayer);


//���ָ�������ı�������
PworldApplyQueue *get_pworld_apply_queue(ZONESVRENV* pstEnv, int iPworldID);

//���ָ�������ĵȴ�����
PworldWaitQueue *get_pworld_wait_queue(ZONESVRENV* pstEnv, int iWaitQueueIdx, int iPworldInstID);


//�˳������Ŷӡ��ȴ�����
int z_exit_pworld_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID, int fNotify);

int z_get_free_wait_queue_idx(ZONESVRENV* pstEnv);

int z_get_free_apply_queue_idx(ZONESVRENV* pstEnv);
	
RolePworldQueue *z_get_player_online_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID);

int z_player_apply_pworld_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID);

int z_player_online_remove_queue(ZONESVRENV* pstEnv, Player *pstPlayer);

int z_player_remove_wait_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iWaitIdx);

//���ս��:�����������
int invite_player_enter_wait_queue(ZONESVRENV* pstEnv, ZONEPWORLD *pstPworld, 
			PworldApplyQueue* PworldApplyQueue, int iWaitIdx);

// �����Ŷ�
int z_player_pworld_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID, CSJOINQUEUE *pstJoin);

//�����ŶӶ���
int z_clear_pworld_apply_queue(ZONESVRENV* pstEnv, PworldApplyQueue *pstApplyQueue);

//�����������
int z_clear_pworld_wait_queue(ZONESVRENV* pstEnv, int iWaitIdx, int iPworldInstID);
	
//�����������м��
int z_pworld_apply_queue_check(ZONESVRENV * pstEnv, PworldApplyQueue * pstApplyQueue, int iImmeCreateSwitch);

//���ý���ս������Ϣ
int player_set_battle_info(ZONESVRENV * pstEnv, Player *pstPlayer, MapIndex *pstMapIndex,
	ZONEPWORLD *pstZonePworld, ZONEPWORLDINST *pstZonePworldInst, RESPOS *pstPos);

//ս��ÿ�ս������
int battle_limit_info_get(ZONESVRENV* pstEnv, Player* pstPlayer);

//����ս��ÿ�ս������
void player_insert_limit_battle(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLD * pstZonePworld);

//ս������������
int z_check_battle_enter_num(ZONESVRENV * pstEnv, Player *pstPlayer, ZONEPWORLD * pstZonePworld, int iNotify);

//���鱨���ั�����
int battle_check_apply_player(ZONESVRENV *pstEnv,Player *pstPlayer, Player** ppstPlayer, int *iPlayer,ZONEPWORLD *pstZonePworld);

//���鱨���ั�� ����
int z_player_apply_battle_pworld_queue(ZONESVRENV* pstEnv, Player *pstPlayer, 
											Player ** ppstPlayer, int iPlayer,
											ZONEPWORLD *pstZonePworld,
											int iTeamJoinType,
											int iSrcMaxLevel);


// ս������
int z_player_normal_battle_queue(ZONESVRENV* pstEnv, Player *pstPlayer, CSJOINQUEUE *pstJoin);

// iFlag==0 �Լ�    1--������δ������Ϣ    2--������������Ϣ
// Vcamp��ʾʤ������Ӫ
int normal_battle_svr_info(ZONESVRENV* pstEnv,Player* pstPlayer, ZONEPWORLD *pstPworld,
							ZONEPWORLDINST	 *pstPworldInst,	int iFlag, int iVCamp);


//����״̬
int leave_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

//����ACTION
int player_leave_action(ZONESVRENV* pstEnv, Player *pstPlayer);

//�������
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

