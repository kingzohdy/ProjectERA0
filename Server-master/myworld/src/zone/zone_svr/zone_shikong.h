
#ifndef ZONE_SHIKONG_H
#define ZONE_SHIKONG_H

#include "zone_svr.h"

extern int g_EnterShiKong;

int shikong_pworld_apply_queue_check(ZONESVRENV * pstEnv,ShiKongApplyQueue *pstApplyQueue);
int shikong_exit_queue_notify(ZONESVRENV * pstEnv,Player *pstPlayer,CSEXITSHIKONGQUEUE *pstExitQueue);
int shikong_pworld_wait_queue_check(ZONESVRENV * pstEnv,ShiKongWaitQueue *pstWaitQueue);
int shikong_tick(ZONESVRENV * pstEnv);
int shikong_clear_wait_queue(ZONESVRENV * pstEnv,int iWaitQueueIdx,int iPworldInstID);
int shikong_get_wait_queue_use_num(ZONESVRENV * pstEnv);
int shikong_get_wait_queue_free_idx(ZONESVRENV * pstEnv);
ShiKongRoleQueueInfo *shikong_get_player_online_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID);
int shikong_player2wait_queue(ZONESVRENV *pstEnv, ZONEPWORLD *pstPworld, int iPworldInstID, 
									int iWaitQueueIdx, Player *pstPlayer);
int shikong_player_online_remove_queue(ZONESVRENV* pstEnv, Player *pstPlayer);
ShiKongWaitQueue *get_shikong_pworld_wait_queue(ZONESVRENV* pstEnv, int iWaitQueueIdx, int iPworldInstID);
int shikong_player_remove_wait_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iWaitIdx,int iPworldInstID);
int shikong_player_remove_apply_queue(ZONESVRENV* pstEnv, Player *pstPlayer,ShiKongApplyQueue *pstApplyQueue);
int shikong_exit_pworld_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID, int fNotify);
int shikong_queuers_remove_applyqueue(ShiKongApplyQueue *pstApplyQueue, int iQueuerNum, int *paiQueuer);
int shikong_get_enter_pworld_idx(ZONESVRENV * pstEnv,int iMemNum);
int player_apply_pworld_from_npc(ZONESVRENV* pstEnv, Player *pstPlayer,int iPworldID,int iMode);
int shikong_enter_apply_queue_notify(ZONESVRENV* pstEnv, Player *pstPlayer,CSENTERAPPLYQUEUE *pstEnterQueue);
int shikong_players2apply_queue(ZONESVRENV* pstEnv, Player *pstPlayer, 
											Player ** ppstPlayer, int iPlayer,
											ZONEPWORLD *pstZonePworld);
int shikong_check_apply_single(ZONESVRENV *pstEnv,Player *pstPlayer, ZONEPWORLD *pstZonePworld);
int shikong_check_apply_team(ZONESVRENV *pstEnv,Player *pstPlayer, ZONEPWORLD *pstZonePworld,
				Player** apstPlayer, int *piPlayer);
ShiKongApplyQueue *get_shikong_pworld_apply_queue(ZONESVRENV* pstEnv, int iPworldID);
int shikong_get_apply_queue_free_idx(ZONESVRENV * pstEnv);
ShiKongApplyQueue *new_apply_queue(ZONESVRENV * pstEnv,int iPworldID);
int player_exit_apply_queue(ZONESVRENV* pstEnv, Player *pstPlayer, CSEXITAPPLYQUEUE *pstExitApplyQueue);
int shikong_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
					TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int shikong_invite_enter_notify(ZONESVRENV* pstEnv,Player *pstPlayer,CSINVITEENTER *pstInvite);
int shikong_queuers_inset_waitqueue(ZONESVRENV* pstEnv , ZONEPWORLD *pstPworld, 
									ZONEPWORLDINST *pstPworldInst,int iQueuerNum, int *paiQueuer);
int player_enter_shikong_pworld(ZONESVRENV *pstEnv,Player *pstPlayer,int iPworldID);
int is_shikong_pworld(ZONEPWORLD *pstZonePworld);



#endif

