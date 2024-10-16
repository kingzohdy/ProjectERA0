#ifndef ZONE_SPARTA_H
#define ZONE_SPARTA_H

#include "zone_svr.h"

int player_sparta_apply_req(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_sparta_apply(ZONESVRENV * pstEnv, SSSPANSPARTAAPPLYREQ * pstReq);

int span_sparta_invite(ZONESVRENV* pstEnv, SSSPANSPARTAINVITE *pstReq);

int sparta_player_die(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstAtkPlayer);

int sparta_role_logout_clean(ZONESVRENV* pstEnv, Player *pstPlayer);

int sparta_player_pos_get(ZONEPWORLDINST *pstZonePworldInst, MapIndex *pstMapIdx, Player *pstPlayer, RESPOS *pstPos);

int sparta_tick(ZONESVRENV* pstEnv);

int sparta_pworld_tick(ZONESVRENV * pstEnv, ZONEPWORLD *pstPworld, ZONEPWORLDINST *pstPworldInst, int iSec);

int sparta_fight_info_update(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, Player *pstPlayer, int fEnd, int iVCamp);

int player_set_sparta_info(ZONESVRENV * pstEnv, Player *pstPlayer, MapIndex *pstMapIndex,
	ZONEPWORLD *pstZonePworld, ZONEPWORLDINST *pstZonePworldInst, RESPOS *pstPos);

int player_leave_sparta(ZONESVRENV* pstEnv, ZONEPWORLDINST* pstPworldInst, Player *pstPlayer);

int player_sparta_team_chg_clean(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_sparta_victory_rate(Player *pstPlayer);

int player_sparta_info(ZONESVRENV* pstEnv, Player *pstPlayer);

int role_login_sparta(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_sparta_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_sparta_runaway_clear(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_sparta_all_clear(ZONESVRENV* pstEnv, Player *pstPlayer);

int sparta_range_check(ZONESVRENV* pstEnv);
 int is_same_real_week(time_t t1, time_t t2);
 int sparta_login_check(ZONESVRENV* pstEnv, Player *pstPlayer);
 
 SPARTAAWARDDEF* z_find_sparta_award_def(ZONESVRENV *pstEnv, int iRange);

 int player_sparta_exit_sparta_apply(ZONESVRENV* pstEnv, Player *pstPlayer);
#endif /* ZONE_SPARTA_H */
