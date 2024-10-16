#ifndef ZONE_PWORLD_H
#define ZONE_PWORLD_H

#include "zone_svr.h"

int player_set_poly_info(ZONESVRENV * pstEnv, Player *pstPlayer, MapIndex *pstMapIndex,
	ZONEPWORLD *pstZonePworld, ZONEPWORLDINST *pstZonePworldInst, RESPOS *pstPos);

int z_player_leave_poly_pworld(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLDINST* pstPworldInst);

int player_enter_poly_pworld_info(ZONESVRENV * pstEnv, ZONEPWORLDINST *pstZonePworldInst, Player *pstPlayer);

int poly_pworld_end(ZONESVRENV * pstEnv, ZONEPWORLDINST *pstZonePworldInst);

int player_enter_map_add_machine(ZONESVRENV * pstEnv, Player *pstPlayer, MapIndex* pstMapIdx);

int player_leave_map_machine_clear(ZONESVRENV * pstEnv, MapIndex *pstMapIndex, Player *pstPlayer);

int poly_pworld_tick(ZONESVRENV * pstEnv, ZONEPWORLDINST *pstPworldInst, int iSec);

int poly_pworld_player_die(ZONESVRENV * pstEnv, Player *pstPlayer, ZoneAni *pstAtker);

int poly_pworld_mon_die(ZONESVRENV * pstEnv, Monster *pstMon, Player *pstPlayer);

int z_sys_strf_camp_broadcast(ZONESVRENV *pstEnv, ZONEMAPINST * pstMapInst, int iCampIdx, int iSysType, int iErrno, ...);

long long pworld_liuyi_player_exp(ZONESVRENV * pstEnv, ZONEPWORLDINST *pstPworldInst,int type, int fEnd);
 int is_valid_money_award_player(int iIdx, int iMaxRole);
#endif /* ZONE_PWORLD_H */
