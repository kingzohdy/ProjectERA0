

#ifndef ZONE_AUTOPWORLD_H
#define ZONE_AUTOPWORLD_H

#include "zone_svr.h"

int autopworld_clt_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int  autopworld_effect(ZONESVRENV* pstEnv, Player *pstPlayer);
int autopworld_clt_enter(ZONESVRENV* pstEnv, Player *pstPlayer, AUTOPWORLDENTERREQ *pstReq, int iEnterFlag);
int autopworld_team_tick(ZONESVRENV* pstEnv, ZoneTeam *pstTeam);
int autopworld_invite_enter(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID, int iCancel, ZoneTeam *pstTeam, int iTip);

#endif

