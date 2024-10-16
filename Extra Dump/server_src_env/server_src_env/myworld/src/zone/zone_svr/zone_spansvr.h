/*
**  @file $RCSfile: zone_spansvr.h,v $
**  general description of this module
**  $Id: zone_spansvr.h,v 1.21 2012/12/13 03:21:37 eric Exp $
**  @author $Author: eric $
**  @date $Date: 2012/12/13 03:21:37 $
**  @version $Revision: 1.21 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef ZONE_SPANSVR_H
#define ZONE_SPANSVR_H

#if 0
#include "zone_svr.h"

typedef enum tagDoStatusassitant
{
	DO_STATUSASSITANT_SYNC = 0,
	DO_STATUSASSITANT_GET = 1,
}DoStatusassitant;

int player_battle_acc_login(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, SSBATTLEPLAYERENTERPWORLDREQ *pstBattlePlayerEnterPworldReq);

int player_battle_now_kickoff(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, char *pszMsg);

int player_battle_enter_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,TFRAMEHEAD *pstFrameHead,Player *pstPlayer,
		int eStatus,int iBattleBusID);

int player_battle_enter_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,SSBATTLEPLAYERENTERPWORLDRES *pstBattlePlayerEnterPworldRes);

int player_battle_enter_map_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,Player *pstPlayer);

int player_battle_enter(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, SSBATTLEPLAYERENTERPWORLDREQ *pstBattlePlayerEnterPworldReq);

int player_battle_update_roledate_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,Player *pstPlayer,int OperType);

int player_battle_update_roledate_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,SSBATTLEUPDATEROLEDATARES *pstBattleUpdateRoleDataRes );

int player_battle_notify_clean_player(ZONESVRENV* pstEnv,int Uin,unsigned long long ullRoleID, 
					SSBATTLECAMPINFO *pstBattleCampInfo,int OperType,int BattleBusID);

int player_battle_now_kickoff(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, char *pszMsg);

int player_battle_logout(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);

int player_battle_enter_map(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,SSBATTLEENTERPWORLDMAPREQ *pstBattleEnterPworldMapReq);

int player_battle_update_roledate(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,SSBATTLEUPDATEROLEDATAREQ *pstBattleUpdateRoleDataReq );

int player_battle_hello2zone_req(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);

int player_battle_hello2zone(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, SSBATTLEPLAYERHELLOREQ *pstBattlePlayerHelloReq);

int player_battle_uin_role_cache_info(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int player_shadow_action_res(ZONESVRENV *pstEnv, Player *pstPlayer,
                             char *pszRaw, int iLen);

int player_shadow_action_request(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 CSACTIONRES *pstActionRes);

int team_shadow_sync_zone(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam,int eStatus,int iBusID);

int team_shadow_exit_req(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam);

int team_shadow_exit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,
                      int iMsgSrc, SSTEAMSHADOWEXITREQ *pstTeamShadowExitReq);
int team_shadow_exit_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,SSTEAMSHADOWEXITRES *pstTeamShadowExitRes);

int team_shadow_sync_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,SSTEAMSHADOWSYNCRES *pstTeamShadowSyncRes);

int team_shadow_sync(ZONESVRENV *pstEnv, int iMsgSrc,
                             tdr_ulonglong ullTeamID, char *pszRaw, int iLen);

int team_shadow_destroy_respond(ZONESVRENV *pstEnv, int iMsgSrc,
                                tdr_ulonglong ullID);

ZoneTeam* team_shadow_by_id(ZONESVRENV *pstEnv, tdr_ulonglong ullID);
int team_shadow_insert(ZONESVRENV *pstEnv, const ZoneTeam *pstZoneTeam);
int team_shadow_delete(const ZONESVRENV *pstEnv, tdr_ulonglong ullID);
int team_shadow_find_or_new(ZONESVRENV *pstEnv, tdr_ulonglong ullID,ZoneTeam **ppstZoneTeam);
int team_shadow_sync_req(ZONESVRENV *pstEnv, int iBusID,ZoneTeam *pstZoneTeam);
int team_shadow_statusassitant_req(ZONESVRENV* pstEnv,ZoneTeam *pstZoneTeam);

int battle_rolename_remove_prefix(char *pszFullName);
int player_check_same_world(ZONESVRENV* pstEnv,char *pszFullName);
int player_battle_get_memid(ZONESVRENV* pstEnv,Player *pstPlayer);
int team_shadow_chg_true_team(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam);
int player_check_same_world_name(char *pszName1,char *pszName2);

int player_do_statusassitant(ZONESVRENV* pstEnv,SSPLAYERSTATUSASSITANTSYNC *pstPlayerStatusassitantSync);
int player_do_battle_statusassitant(ZONESVRENV* pstEnv,Player *pstPlayer,int OperType);
int player_do_zone_statusassitant(ZONESVRENV* pstEnv,Player *pstPlayer,int OperType);
int player_do_statusassitant_sync(ZONESVRENV* pstEnv,Player *pstPlayer);
int player_do_battle_check_offline(ZONESVRENV* pstEnv,TEAMMEMBER *pstTeamMember,int eStatusAssistant);
int team_shadow_chg_true_team(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam);
int team_create_succ(ZONESVRENV* pstEnv,SSBATTLETEAMSUCCREQ *pstBattleTeamSuccReq);
int team_create_succ_req(ZONESVRENV* pstEnv,ZoneTeam *pstZoneTeam,Player *pstPlayer);

int is_player_in_battle_pve_svr(ZONESVRENV* pstEnv, Player *pstPlayer);
int is_player_in_battle_pvp_svr(ZONESVRENV* pstEnv, Player *pstPlayer);
int is_player_in_battle_autoteam_svr(ZONESVRENV * pstEnv, Player * pstPlayer);

int player_enter_battle_pvp_map_req(ZONESVRENV* pstEnv, Player *pstPlayer, int iMapID);
int player_enter_battle_pvp_map(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_pvp_svr_member_info_sync_req(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_pvp_svr_member_info_sync(ZONESVRENV* pstEnv, SSBATTLEMEMBERINFO *pstSSMemberInfo);

int player_battle2zone_action_request(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 CSACTIONRES *pstActionRes);
#endif

#endif /* ZONE_SPANSVR_H */

