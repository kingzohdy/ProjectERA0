#ifndef ZONE_DIFFPWORLD_H
#define ZONE_DIFFPWORLD_H

#include "zone_svr.h"

int is_diff_pworld_grade(ZONESVRENV *pstEnv, int iDiffPworldID, int iPworldID);

DIFFPWORLDDEF *z_find_diff_pworld_def(ZONESVRENV *pstEnv, int iDiffPworldID);

int diff_pworld_team_tips_add(CSDIFFPWORLDTEAMTIPS *pstTeamTips, char *pszRoleName, int iErrno, ... );

int open_diff_pworld_ui(ZONESVRENV* pstEnv, Player *pstPlayer, int iDiffPworldID, int iNpcID);

int diff_pworld_enter(ZONESVRENV* pstEnv, Player *pstPlayer, CSDIFFPWORLDENTER *pstEnter);

int diff_pworld_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
			TFRAMEHEAD	 *pstFrameHead, CSPKG *pstCsPkg);

int diff_pworld_dyn_level_get(ZONESVRENV* pstEnv, Player *pstPlayer, int iDiffPworldID);

int team_pworld_level_get(ZONESVRENV* pstEnv, ZoneTeam *pstTeam, ZONEPWORLD *pstPworld);

int single_pworld_level_get(Player *pstPlayer, ZONEPWORLD *pstPworld);

int pworld_val_trace_mon_die(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, Monster *pstMon);

int pworld_val_trace_player_die(ZONEPWORLDINST *pstPworldInst);

int pworld_award_open_ui(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, ZONEPWORLD *pstPworld);

int pworld_award_tick(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst);

int pworld_player_add_exp(ZONESVRENV* pstEnv, Player *pstPlayer, int iExp);
int team_diff_pworld_get(ZONESVRENV* pstEnv, ZoneTeam *pstTeam, int iDiffPworldID);
int player_is_in_diffpworld(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_team_create_syn_diffpworld(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneTeam *pstTeam);
int diff_pworld_team_check_tips(ZONESVRENV* pstEnv,ZoneTeam *pstTeam, 
									Player *pstPlayer,ZONEPWORLD *pstPworld);

int pworld_box_start(ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst);

int pworld_box_award(ZONESVRENV* pstEnv, Player *pstPlayer, Npc *pstNpc, ZONEMAPINST *pstMapInst);

#endif //  ZONE_DIFFPWORLD_H

