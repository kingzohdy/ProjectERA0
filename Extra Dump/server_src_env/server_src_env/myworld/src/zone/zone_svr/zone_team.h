
#ifndef ZONE_TEAM_H
#define ZONE_TEAM_H

#include "zone_svr.h"

int player_black(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_team(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

ZoneTeam *player_team_get(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_team_all_action(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam);

int player_team_broadcast(ZONESVRENV* pstEnv, ZoneTeam *pstTeam, CSPKG *pstPkg);
int player_team_broadcast_except(ZONESVRENV* pstEnv, ZoneTeam *pstTeam, CSPKG *pstPkg, Player *pstPlayerExcept);

int player_team_broadcast_tip(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, char *pszMsg);

int z_active_team(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iCount);

TEAMMEMBER *player_team_get_captain(ZoneTeam *pstTeam);

TEAMMEMBER *player_team_get_member(ZoneTeam *pstTeam, char *pszName, int *piIdx);

int player_team_same(ZONESVRENV* pstEnv, Player *pstPlayer1, Player *pstPlayer2);

// 获取契约效果
// 返回 0 : 契约被选定有效且受益人正确
// 否则返回 < 0
int player_team_checked_qiyue_result(ZONESVRENV * pstEnv, Player * pstPlayer, ZoneTeam * pstZoneTeam, QIYUERESULT * pstQiYueResult);

// 通过契约ID查找契约定义
// 返回 NULL : 无此契约
// 否则返回指向契约定义的指针。
QIYUEDEF * z_find_qiyuedef( ZONESVRENV* pstEnv, int iQiYueID , int *piIdx);

int player_team_send_user_qiyuelist(ZONESVRENV * pstEnv, Player * pstPlayer);

ZoneTeam *z_idx_team(ZONESVRENV* pstEnv, int iTeamIdx, tdr_ulonglong ullTeamID);

int team_insert_pworld_inst(ZONESVRENV * pstEnv, ZoneTeam *pstZoneTeam, ZONEPWORLDINST *pstZonePworldInst, int fCaptain);

int team_member_pworld_update(ZONESVRENV* pstEnv, ZoneTeam *pstTeam, Player *pstPlayer, int iCaptain);

int team_del_pworld_inst(ZONESVRENV * pstEnv, ZoneTeam *pstZoneTeam, int iPworldID);

Player* player_team_get_captain_player(ZONESVRENV *pstEnv,
                                              ZoneTeam *pstZoneTeam);

void player_team_sync_member_info(ZONESVRENV* pstEnv, TEAMMEMBER *pstTeamMember, Player *pstPlayer);

ZoneTeam *player_team_create(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_team_sync(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam);

int flush_battle_queue_team_info(ZONESVRENV* pstEnv, Player *pstPlayer,ZoneTeam *pstZoneTeam, int iFlag);

int flush_battle_pworld_queue(ZONESVRENV* pstEnv, Player *pstPlayer,ZoneTeam *pstZoneTeam);

int player_team_exit(ZONESVRENV* pstEnv, Player *pstPlayer,ZoneTeam *pstZoneTeamTmp);

int player_team_adjust_captain_pos(ZONESVRENV* pstEnv, ZoneTeam *pstTeam);

int player_team_destroy_in(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, int iFlag);

Player* team_get_captain_by_player(ZONESVRENV *pstEnv,Player* pstPlayer);

int player_team_kick(ZONESVRENV* pstEnv, Player *pstPlayer, CSTEAMKICK *pstTeamKick,int OperType);

int player_team_exit_sync(ZONESVRENV* pstEnv, Player *pstPlayer);

int team_check_on_zonesvr_by_teamid(unsigned long long ullTeamID);

int ss_flush_battle_pworld_queue(ZONESVRENV* pstEnv, SSFLUSHBATTLEQUEUE  *pstFlushBattleQueue);

int ss_flush_battle_queue_team_info(ZONESVRENV* pstEnv, SSFLUSHBATTLEQUEUETEAMINFO *pstFlushBattleQueueTeamInfo);
int player_team_follow(ZONESVRENV* pstEnv, Player *pstPlayer, CSTEAMFOLLOW *pstFollow);
int player_team_follow_logout(ZONESVRENV* pstEnv, Player *pstPlayer);
//int player_team_follow_chg_map(ZONESVRENV* pstEnv, Player *pstPlayer, int iMapID, RESPOS *pstPos);
//int player_team_follow_enter_pworld(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID,
//										PWORLDSELECT *pstPwolrdSelect);

int player_team_follow_chg_map(ZONESVRENV* pstEnv, Player *pstPlayer, int iMapID, 
										RESPOS *pstPos,ZONEPWORLDINST *pstPworldInst);
int player_team_follow_sec(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_team_follow_tick(ZONESVRENV* pstEnv,ZoneTeam *pstTeam);
int player_team_summon_answer(ZONESVRENV* pstEnv, Player *pstPlayer, CSTEAMSUMMONANSWER *pstSummonAnswer);

int player_team_summon(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_team_summon_close_ui(ZONESVRENV* pstEnv,ZoneTeam *pstTeam);
int player_logout_cancel_follow(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_team_follow_cancel(ZONESVRENV* pstEnv,ZoneTeam *pstTeam, Player *pstPlayer, int iCap);
int player_team_follow_cancel_svr(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_leave_team_clean_buff(ZONESVRENV* pstEnv, Player *pstPlayer, int iResultID);
int player_follow_enter_map_check(ZONESVRENV* pstEnv, Player *pstPlayer,int iMap);

//设置用户队伍信息
int player_team_info_set(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneTeam *pstTeam);

int player_team_memchg(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneTeam *pstZoneTeam, int iFlag);

int player_team_rongyan_login_check(ZONESVRENV* pstEnv, Player *pstPlayer);

int rongyan_team_tick(ZONESVRENV*pstEnv,ZoneTeam *pstTeam);



#endif /* ZONE_TEAM_H */
