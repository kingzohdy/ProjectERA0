#ifndef ZONE_PLOY_H
#define ZONE_PLOY_H

#define LOST_MAP_ID_LAYER7 3217
#define GC_EVENT_ID 102
#define GC_PLOY_TIME 90*60
#define GC_PLOY_LVL 70
#define TANK_WIN_NUM 200

#define MZT_MAX_LAYER 9

//迷之塔第三层加的坏buff
#define MZT_THREE_LAYER_BAD_STATUS 2115

#include "zone_svr.h"

int ploy_gc_end_player_clear(ZONESVRENV* pstEnv);
int ploy_gc_ui_info(ZONESVRENV* pstEnv, Player *pstPlayer, int iType, int);
int ploy_gc_set_award(ZONESVRENV* pstEnv, Player *pstPlayer, int iLayer);
int ploy_gc_get_award(ZONESVRENV* pstEnv, Player *pstPlayer, int iLayer);
int ploy_gc_player_login(ZONESVRENV* pstEnv, Player *pstPlayer);


int ploy_tank_pworld_info(ZONESVRENV * pstEnv, ZONEPWORLD *pstPworld,
	ZONEPWORLDINST *pstPworldInst, Player *pstPlayer, int fEnd);
int ploy_tank_pworld_tick(ZONESVRENV * pstEnv, ZONEPWORLD *pstPworld, ZONEPWORLDINST *pstZonePworldInst, int iSec);
int poly_tank_pworld_mon_die(ZONESVRENV * pstEnv,  ZONEPWORLDINST *pstZonePworldInst,  Player *pstKiller);
int poly_tank_pworld_player_die(ZONESVRENV * pstEnv, Player *pstPlayer, ZoneAni *pstAtker);
int poly_tank_pworld_tip1(ZONESVRENV * pstEnv, Player *pstPlayer, int iDie);
int player_is_in_tank_pworld(ZONESVRENV * pstEnv, Player *pstPlayer);
int poly_mzt_get_layer(int iPworldID);
int poly_is_mzt_pworld(int iPworldID);
int ploy_mzt_get_pworldid(ZONESVRENV * pstEnv, ZoneTeam *pstTeam);
int poly_mzt_team_layer_notify(ZONESVRENV * pstEnv,  ZoneTeam *pstTeam);
int poly_mzt_del_status(ZONESVRENV* pstEnv, Player *pstPlayer,int iOldMap,int iStatusID);
int poly_mzt_get_team_layer(ZONESVRENV* pstEnv,ZoneTeam *pstTeam);

#endif 