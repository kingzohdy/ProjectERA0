#ifndef ZONE_STRONGPOINT_H
#define ZONE_STRONGPOINT_H

#include "zone_svr.h"
int strong_global_init(ZONESVRENV* pstEnv);
int strong_refresh_map_inst(ZONESVRENV* pstEnv,ZONEPWORLDINST *pstZInst,ZONEMAPINST* pstMInst, int StrongMapID);
int strong_del_pworld_info(ZONESVRENV* pstEnv, int iMapID);
int strong_resmon_die(ZONESVRENV* pstEnv, Monster* pstMon, MONSTERDEF *pstMonDef, Player *pstKiller);
int strong_clt_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int strong_svr_strong_info(ZONESVRENV* pstEnv, Player *pstPlayer, int iSingle);
int strong_svr_sale_info(ZONESVRENV* pstEnv, Player *pstPlayer, int iMapID, int iNpcID);
int strong_svr_pworld_info(ZONESVRENV* pstEnv, Player *pstPlayer, int iSingle, ZONEMAPINST *pstMInst, int iStrongMapID);
int strong_get_map_pos(ZONESVRENV* pstEnv, int iMapID, MapIndex *pstMapIdx,RESPOS *pstPos, Player *pstPlayer, int iRevive);
int strong_pworld_res_add(ZONESVRENV* pstEnv, int iMapID);
int strong_pworld_tick(ZONESVRENV* pstEnv, ZONEPWORLD *pstZonePworld, ZONEPWORLDINST *pstPInst, int iFlag);
int strong_player_award(ZONESVRENV* pstEnv,Player *pstPlayer, int iType, int iMapID);
int strong_player_die(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstKiller, int *piNixi);
int strong_tick(ZONESVRENV* pstEnv);
int strong_get_stat(ZONESVRENV* pstEnv, int iMapID);
int strong_player_is_in_pworld(ZONESVRENV* pstEnv, Player *pstPlayer);
int strong_clt_sale( ZONESVRENV* pstEnv, Player *pstPlayer, STRONGSALE *pstSale);
int strong_enter_pworld(ZONESVRENV* pstEnv, Player *pstPlayer,  STRONGENTER *pstEnter);
//int strong_pworld_player_exit_clan(ZONESVRENV* pstEnv, Player *pstPlayer);
int strong_pworld_force_exit(ZONESVRENV* pstEnv, Player *pstPlayer, char *pstTip);
int strong_clear(ZONESVRENV* pstEnv, Player *pstPlayer, int iMapID);
int strong_make_banner_view( ZONESVRENV* pstEnv, CSMONSTER *pstCSMon, Monster *pstVMon);
int strong_open_set_banner_ui(ZONESVRENV* pstEnv,Player *pstPlayer, Npc* pstNpc);
int strong_open_set_banner_xuanyan_ui(ZONESVRENV* pstEnv,Player *pstPlayer, Npc* pstNpc);
int strong_new_banner(ZONESVRENV* pstEnv);
STRONGPOINTONE* strong_point_one_get(ZONESVRENV* pstEnv, int iMapID);
STRONGPOINTONE* strong_get_by_player(ZONESVRENV* pstEnv, Player *pstPlayer);
int strong_check_open(ZONESVRENV* pstEnv);
int strong_is_open(ZONESVRENV* pstEnv, int iMapID);
int strong_cancel(ZONESVRENV* pstEnv, Player * pstPlayer, int iMapID);
int strong_clear_all(ZONESVRENV* pstEnv);
int strong_set_owner(ZONESVRENV* pstEnv,Player *pstPlayer, int iMapID);
int strong_clan_destory(ZONESVRENV* pstEnv, ZoneClan *pstClan);
STRONGPOINTONE* strong_one_find_by_clan(ZONESVRENV* pstEnv, ZoneClan *pstClan, int iAtk);
int strong_war_all_notify(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPInst, int iEnd);
int strong_war_award(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPInst, int iWinID);
int strong_exit_clear_buff(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLD *pstPworld);
int strong_enter_id_chg(ZONESVRENV* pstEnv, Player *pstPlayer, int iID);
int strong_glory_player_tick(ZONESVRENV* pstEnv, Player *pstPlayer);
int strong_glory_player_login(ZONESVRENV* pstEnv, Player *pstPlayer);
int strong_glory_open(ZONESVRENV* pstEnv, Player *pstPlayer, int iMap);
int strong_glory_clan_is_open(ZONESVRENV* pstEnv, Player *pstPlayer);
int strong_player_login(ZONESVRENV* pstEnv, Player *pstPlayer);
int strong_player_is_owner(ZONESVRENV* pstEnv, Player *pstPlayer, int iMap);
int strong_glory_close_by_clan_destory(ZONESVRENV* pstEnv, ZoneClan *pstClan);
STRONGPOINTDEF* strong_def_find(ZONESVRENV* pstEnv, int iMapID);
int strong_pworld_is_open_by_clan(ZONESVRENV* pstEnv, ZoneClan *pstClan);
int strong_is_normal(ZONEPWORLD *pstPworld);
#endif