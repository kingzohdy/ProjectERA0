#ifndef ZONE_RONGLU_H
#define ZONE_RONGLU_H

#include "zone_svr.h"

RONGLUDEF* ronglu_def_find(ZONESVRENV* pstEnv, int iItemID);
int ronglu_kill_mon_booty(ZONESVRENV* pstEnv, Player *pstPlayer,Monster *pstKillMon);
int ronglu_is_full(Monster *pstMon);
int ronglu_killnum_chg(ZONESVRENV* pstEnv, Player *pstPlayer, int iNum, Monster *pstRonglu,Monster *pstMon, int );
int ronglu_clt_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int ronglu_one_chg(ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID, int iTaskID);
int ronglu_player_new_mon(ZONESVRENV* pstEnv, Player *pstPlayer ,int iItemID, 
								int iMonID, int iLeftTime, int iDynLvl, int iDynGrade);
int ronglu_player_new_collect_mon(ZONESVRENV* pstEnv, Player *pstPlayer, int iMonID, int iLeftTime);
int ronglu_get_item_id(Player *pstPlayer);
int ronglu_get_task_id(Player *pstPlayer);
int ronglu_get_task(ZONESVRENV* pstEnv, Player *pstPlayer, int iTaskID);
int ronglu_task_fini(ZONESVRENV* pstEnv, Player *pstPlayer, TASKDEF *pstTaskDef);
int ronglu_player_login(ZONESVRENV* pstEnv, Player *pstPlayer);
int ronglu_player_login_check(ZONESVRENV *pstEnv, Player *pstPlayer);
int ronglu_check_pre(ZONESVRENV* pstEnv);
int ronglu_item_chg_gold(ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID);

#endif 