
#ifndef ZONE_AUTOTEAM_H
#define ZONE_AUTOTEAM_H

#if 0
#include "zone_svr.h"


#define LIMIT_LOOSE_TIME 60
#define LIMIT_LEVEL_DIFF 5
#define LIMIT_LEVEL_LOOSE_DIFF 120

//活动副本
#define LIMIT_LEVEL_DIFF_PLOY 5
#define LIMIT_LEVEL_LOOSE_DIFF_PLOY 10

int autoteam_init(ZONESVRENV* pstEnv);

int player_auto_team_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int autoteam_created_mem_online_info(ZONESVRENV* pstEnv, Player *pstPlayer);

int autoteam_tick_player(ZONESVRENV *pstEnv, Player *pstPlayer);

int autoteam_refresh_team(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam,int OperType);

int autoteam_player_offline(ZONESVRENV *pstEnv, Player *pstPlayer);
//队伍改变不允许继续自动组队
int autoteam_team_chg_reject(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam, int iCancelFlag);
//允许队伍改变,自动组队继续
int autoteam_team_chg_allow(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam);

//iContinueFlag 0表示告知zonesvr自动组队退出  1表示继续自动组队
int autoteam_reject(ZONESVRENV* pstEnv, Player *pstPlayer,int iContinueFlag);

int autoteam_team_reject(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneTeam *pstZoneTeam, int iCancelFlag);

int autoteam_check_pworld(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, 
								Player *pstPlayer, int iPworldID);

int autoteam_check_active_running(ZONESVRENV* pstEnv, ACTIVEHELPER *pstActiveHelper);

AutoTeamCreated *autoteam_find_created_by_teamid(AutoTeamOne *pstAutoTeamOne, int iTeamID);

int autoteam_created_succ(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam,
								AutoTeamCreated *pstAutoTeamCreated, int iAutoTeamID);

int autoteam_player_cancel_send(ZONESVRENV* pstEnv, Player *pstPlayer, int iCancelFlag);

int autoteam_chg_pworld_mappos_byteam(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,ZONEPWORLDINST* pstPworldInst,RESPOS *pstPos);

int autoteam_team_info_sync(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam,int OperType);

int autoteam_team_chg_allow_check(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam);

int autoteam_in_pworld_add_safe_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,ZONEPWORLDINST* pstPworldInst, Player* pstPlayer);

int autoteam_in_pworld_del_saft_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer);

//新加入的人看看能否继续自动组队
int autoteam_team_add_player_check(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, Player* pstPlayer);

int autoteam_team_successed_add_member(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, Player *pstOne);

int autoteam_apply(ZONESVRENV* pstEnv, Player *pstPlayer,AUTOTEAMAPPLYINFO *pstApply);
int autoteam_agree(ZONESVRENV* pstEnv, Player *pstPlayer);

int autoteam_op_trans(ZONESVRENV* pstEnv, Player *pstPlayer, int PworldID);

int autoteam_notify_clt_clean(ZONESVRENV* pstEnv, Player *pstPlayer);

ACTIVEHELPER *find_active_by_autoteam(ZONESVRENV* pstEnv, int iAutoTeamID);

int autoteam_team_successed_chg_member(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam);

int autoteam_createdteam_add_createdteam(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeamDec);

short autoteam_find_apply_limit(AUTOTEAMAPPLYINFO *pstApply, int iAutoTeamID);

int autoteam_create_agree(ZONESVRENV* pstEnv, Player *pstPlayer);

int autoteam_exit_trans(ZONESVRENV* pstEnv, Player *pstPlayer);

int autoteam_reject_send_to_battle(ZONESVRENV* pstEnv, Player *pstPlayer);

int autoteam_team_chat(ZONESVRENV* pstEnv, Player *pstPlayer, CSPKG *pstPkg,int iSelf);

#endif

#endif /* ZONE_AUTOTEAM_H */
