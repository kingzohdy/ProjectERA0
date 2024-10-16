#ifndef ZONE_VIP_H
#define ZONE_VIP_H

#include "zone_svr.h"

STARSTONEDEF *z_find_stonevip_def(ZONESVRENV *pstEnv, int ID);
STARFUNCDEF *z_find_starfunc_def(ZONESVRENV *pstEnv, int ID);

int star_stone_check_endtime(ZONESVRENV *pstEnv, Player *pstPlayer);
int star_get_vipflag(ZONESVRENV *pstEnv, Player *pstPlayer);
int star_stardata_chg(ZONESVRENV *pstEnv, Player *pstPlayer, int iAction);

int star_stone_open_req(ZONESVRENV *pstEnv, Player *pstPlayer,CSSTARSTONEOPENREQ *pstStarStoneOpenReq);

int star_check_item_result(ZONESVRENV *pstEnv, Player *pstPlayer, RESULT *pstResult);
int star_item_result(ZONESVRENV *pstEnv, Player *pstPlayer, RESULT *pstResult);

RESULT *star_get_result(ZONESVRENV *pstEnv, Player *pstPlayer, int iResultID);
int star_stardata_action(ZONESVRENV *pstEnv, Player *pstPlayer);

int player_star_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

#endif //  ZONE_VIP_H
