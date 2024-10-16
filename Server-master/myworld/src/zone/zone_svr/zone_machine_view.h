
#ifndef ZONE_MACHINE_VIEW_H
#define ZONE_MACHINE_VIEW_H

#include "zone_svr.h"

int machine_msg_broadcast_except(ZONESVRENV* pstEnv, Monster *pstMachine, CSPKG *pstPkg, int iID);

int machine_msg_broadcast(ZONESVRENV* pstEnv, Monster *pstMachine, CSPKG *pstPkg);

int player_view_machine(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMachine, ZONEMAPINST* pstMapInst);

int mon_view_machine(ZONESVRENV* pstEnv, Monster *pstMon, Monster *pstMachine, ZONEMAPINST* pstMapInst);

int machine_count_view(ZONESVRENV* pstEnv, Monster *pstMachine);

int machine_view_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMachine, Monster *pstVMon);

int mon_norm_view_machine(ZONESVRENV* pstEnv, Monster *pstMon, Monster *pstMachine, ZONEMAPINST* pstMapInst);

int obj_leave_machine_view_send(ZONESVRENV* pstEnv, Monster *pstMachine, ZONEMAPINST* pstMapInst, int iID);

int obj_leave_machine_view(ZONESVRENV* pstEnv, Monster *pstMachine, ZONEMAPINST* pstMapInst, int iID, char cObjType);

int machine_search_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMachine, ZONEMAPINST* pstMapInst, RESPOS *pstPos);

int machine_player_rideon_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMachine);

int machine_player_rideoff_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMachine); 

int machine_view_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMachine, Player *pstVPlayer);
int npc_view_machine_check(ZONESVRENV* pstEnv, Monster *pstNpcMon, Monster *pstMachine);

							
#endif /* ZONE_MACHINE_VIEW_H */

