
#ifndef ZONE_MOVE_H
#define ZONE_MOVE_H

#include "zone_svr.h"

int z_init_search_func(SearchFunc *pstSearchFunc);
AREADYN *z_pos_area_dyn(MapIndex *pstMapIdx, TMEMPOOL *pstAreaPool, ZONEMAPINST* pstMapInst, RESPOS *pstResPos);
int z_player_search_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, RESPOS *pstPos,
							Player *pstPlayer);
int z_ani_enter_area(MapIndex *pstMapIndex, TMEMPOOL *pstAreaDynPool, int iID, RESPOS *pstPos, ZONEMAPINST* pstMapInst);
int z_ani_leave_area(MapIndex *pstMapIndex, TMEMPOOL *pstAreaDynPool, int iID, RESPOS *pstPos, ZONEMAPINST* pstMapInst);
int z_ani_leave_area_test(MapIndex *pstMapIndex, TMEMPOOL *pstAreaDynPool, int iID, RESPOS *pstPos, ZONEMAPINST* pstMapInst);

RESPOS* z_pos_search_pos(MapIndex *pstMapIdx, RESPOS *pstPos, RESPOS *pstSearchPos);
int z_obj_leave_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, Player *pstPlayer, int iAnimate, int iID, int iObjType, int iChgMapFlag);
int z_pos_search_area(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, RESPOS *pstPos,
							SearchArea *pstSArea);

inline long long z_compare_time(struct timeval *pstTime1, struct timeval *pstTime2);
int z_adjust_time(struct timeval *pstCltTime, struct timeval *pstConnectTime, Player *pstPlayer);
int z_bench_time(Player *pstPlayer);

int z_player_stop(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iBench, struct timeval *pstCltTime);

int z_player_check_stop(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSMOVEC *pstMove, struct timeval *pstCltTime);

int z_add_path(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSMOVEC *pstMove, char cPassC);

int z_player_move_notify(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,char cMoveType);
int z_monster_move_notify(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster  *pstMon, char cMoveType);
int z_player_move_curr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iSearch);
int z_player_move_stop(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int z_point_on_recordpath(int iX, int iY, char cStart, char cPoints, RESPOS *pstTurnPoints, unsigned char bFuturePoint);
int z_monster_move_curr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, int iSearch);

int z_monster_move_bytime(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, struct timeval *pstCltTime);

int z_sendpkg_playerview(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iSelf, CSPKG *pstPkg, int iImportant);
int z_sendpkg_monview(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, CSPKG *pstPkg, int iImportant);

int z_pos_in_view(RESPOS *pstPos1, RESPOS *pstPos2);
double z_distance(RESPOS* pstPos1, RESPOS* pstPos2);
int z_in_same_area(MapIndex *pstMapIdx, TMEMPOOL *pstAreaDynPool, ZONEMAPINST* pstMapInst, RESPOS *pstPos1, RESPOS *pstPos2);

inline int z_same_pos(RESPOS *pstPos1, RESPOS *pstPos2);
int z_adjust_time(struct timeval *pstCltTime, struct timeval *pstConnectTime, Player *pstPlayer);
int z_bench_time(Player *pstPlayer);
int z_pos_in_view_half(RESPOS *pstPos1, RESPOS *pstPos2);
int z_pos_in_view_half1(RESPOS *pstPos1, RESPOS *pstPos2);
int z_pos_in_view_half2(RESPOS *pstPos1, RESPOS *pstPos2);
int z_is_point_inseg( int x, int y, int x1, int y1, int x2, int y2 );
int z_reach_curr_turnpoint(Player *pstPlayer);
double z_move_on_path(Player *pstPlayer, double bMoveDist);
int z_turn_point_dist(RESPOS *pstTurnPoints, char cStart, char cEnd);

int z_mon_search_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, RESPOS *pstPos, Monster* pstMon);

int z_item_leave_area(MapIndex *pstMapIndex, TMEMPOOL *pstAreaDynPool, int iID, RESPOS *pstPos, ZONEMAPINST* pstMapInst);
int z_item_enter_area(MapIndex *pstMapIndex, TMEMPOOL *pstAreaDynPool, int iID, RESPOS *pstPos, ZONEMAPINST* pstMapInst);
int z_item_search_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, RESPOS *pstPos, DropItem *pstDropItem);

int z_trust_clt_pos(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						int x, int y, struct timeval *pstCltTime, int iNoChgPos);

int z_check_proc(ZONESVRENV* pstEnv, Player *pstPlayer, MapIndex *pstMapIdx, RESPOS *pstPos, int fStartProc);

int z_player_move_curr_bytime(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int  iSearch, struct timeval *pstTime, int iTimeIgnore);
int z_mon_stop(ZONESVRENV* pstEnv, Monster *pstMon);
Area *z_pos_area(MapObj *pstMap, MapIndex* pstMapIdx, RESPOS *pstPos);

int pworld_map_broadcast(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, ZONEMAPINST *pstMapInst, int iImportant, CSPKG *pstPkg);
int pworld_map_broadcast_except(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, ZONEMAPINST *pstMapInst, int iImportant, 
									CSPKG *pstPkg, Player *pstPlayerExcept);

int z_make_mon_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, CSMONSTER *pstCSMon, Monster *pstMon);
int z_player_view_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstVPlayer);

int z_player_chgpos(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						RESPOS *pstPos);

int z_svr_player_move( ZONESVRENV* pstEnv, Player *pstPlayer);

int z_svr_mon_move( ZONESVRENV* pstEnv, Monster*pstMon);

int z_mon_run_away(ZONESVRENV* pstEnv, Monster*pstMon, RESPOS *pstAtkPos, int iDist, int iAiStat);

int z_player_move_notify_one(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstVPlayer);

int z_obj_area_move(ZONESVRENV* pstEnv, MapIndex* pstMapIdx, ZONEMAPINST* pstMapInst, int iMemID, RESPOS *pstLastPos, int fStartProc);

int z_svr_player_move_by_patroldef( ZONESVRENV* pstEnv, Player *pstPlayer,RESPOS *pstPatrolPos, unsigned int uiNum);

int z_player_stop_by_patrol_end( ZONESVRENV* pstEnv, Player *pstPlayer);

int z_player_store_remain_patrol(ZONESVRENV* pstEnv, Player *pstPlayer);

int z_ani_enter_area_test(MapIndex *pstMapIndex, TMEMPOOL *pstAreaDynPool, int iAniNum, RESPOS *pstPos, ZONEMAPINST* pstMapInst);

int z_make_player_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstVPlayer, CSENTERVIEW *pstSvr);
int logic_area_action(ZONESVRENV* pstEnv, Player *pstPlayer, int iFlag, int iType);

int role_leave_logic_area_test(ZONESVRENV* pstEnv, Player *pstPlayer);

int z_player_view_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstVMon);

int z_move_get_v_spd(int v0,int a,float t);

float z_move_get_t_time(int v0,int a,int s);

int z_move_get_x_distance(int v0,int a,int v);

int z_make_npc_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, CSNPC *pstCSNpc, Monster *pstVMon);

int player_view_mon_must(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstVMon);

int obj_leave_view_send(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, Player *pstPlayer, int iID, int iObjType, int iNotifyFlag);

int z_sendpkg_monview_except(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, 
		CSPKG *pstPkg, int iImportant, Player *pstPlayer);

int z_mon_chgpos(ZONESVRENV* pstEnv, Monster *pstMon, RESPOS *pstPos);

int z_sendpkg_atkposview(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst, 
							RESPOS *pstAtkPos,CSPKG *pstPkg,Player **apstPlayer, int iNum);
/*
int z_player_area_view_imp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, Player *pstPlayer,
							RESPOS *pstCurrPos,RESPOS *pstNextPos);

int z_monster_area_view_imp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, Monster *pstMon,
							RESPOS *pstCurrPos,RESPOS *pstNextPos);*/
							
#endif /* ZONE_MOVE_H */

