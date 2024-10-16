
#ifndef ZONE_UNREAL_H
#define ZONE_UNREAL_H

#include "zone_svr.h"

#define UNREAL_ENTER_NPC 0
#define UNREAL_ENTER_NEXT 1
#define UNREAL_ENTER_BOSS 2
#define UNREAL_ENTER_GM 3
#define ENTER_NEXT_KILL 80

#define ENTER_UNREAL_MIN 3

UNREALMAPDEF* z_find_unrealmapdef(ZONESVRENV* pstEnv, int iMapID);
UNREALDEF* z_find_unrealdef(ZONESVRENV* pstEnv, int iLayer);
int unreal_get_rand_map(UNREALDEF *pstDef);
int unreal_new_map_inst(ZONESVRENV* pstEnv, PWORLDSELECT *pstSelect, int iPworldInstID);
int unreal_info_up(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, int iLayer,int iType);
int unreal_refresh_map_inst(ZONESVRENV* pstEnv,ZONEPWORLDINST *pstZInst,ZONEMAPINST* pstMInst);
int unreal_layer_info_notify(ZONESVRENV* pstEnv, Player* pstPlayer, ZONEPWORLDINST *pstPworldInst);
int unreal_cacl_down_time(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst);
int  unreal_up_boss_info(ZONESVRENV* pstEnv, Monster *pstMon,int iEnterMap);
int unreal_watch_boss_info(ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID);
int unreal_save_info(ZONESVRENV* pstEnv, Player *pstPlayer);
int unreal_save(ZONESVRENV* pstEnv, Player *pstPlayer);
int unreal_team_info(ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID);
int unreal_enter(ZONESVRENV* pstEnv, Player *pstPlayer,int iLayer,int iType);
int unreal_info_delete( ZONESVRENV* pstEnv,int iLayer,int iDeleteID, int);
int unreal_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
Player *is_captain_in_unreal(ZONESVRENV* pstEnv, Player *pstPlayer);
int get_player_unreal_layer(ZONESVRENV *pstEnv,Player *pstPlayer);
int unreal_up_kill(ZONESVRENV* pstEnv,  Player *pstPlayer,int iKillID);
int unreal_get_enter_pos(ZONESVRENV* pstEnv,ZONEPWORLDINST *pstPInst, int *piMapIdx, RESPOS *pstPos);
int unreal_enter_check(ZONESVRENV* pstEnv, Player *pstPlayer,int iLayer,int iType);
int unreal_enter_next_check_kill(ZONEPWORLDINST *pstPworldInst);
int unreal_team_enter_pworld(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLD *pstPworld, 
									ZONEPWORLDINST *pstPInst,ZoneTeam *pstTeam);
//int unreal_enter_check_team(ZONESVRENV* pstEnv, Player *pstPlayer);
int unreal_enter_check_team(ZONESVRENV* pstEnv, Player *pstPlayer,int iType, int iTip);
int is_unreal_boss_map(ZONESVRENV* pstEnv, int iMapID);
int team_trans_check_one(ZONESVRENV* pstEnv, Player *pstPlayer);
int team_trans_check_tip(ZONESVRENV* pstEnv, Player *pstPlayer, int iCap, int iRet, Player *pstTeamPlayer);
int unreal_enter_boss_check_level(ZONESVRENV *pstEnv,Player *pstPlayer, ZONEPWORLDINST *pstPInst);
int unreal_check_exp_layer(ZONESVRENV *pstEnv,Player *pstPlayer);
int unreal_exp_layer_chg(ZONESVRENV *pstEnv,Player *pstPlayer, int iSingle);
int unreal_add_award_exp(ZONESVRENV *pstEnv,Player *pstPlayer);
#endif