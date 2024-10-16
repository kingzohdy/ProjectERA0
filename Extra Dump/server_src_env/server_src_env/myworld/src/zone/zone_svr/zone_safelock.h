

#ifndef ZONE_SAFELOCK_H
#define ZONE_SAFELOCK_H
#include "zone_svr.h"


int player_safelock_login(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_safelock(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int player_check_grabsafe(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_check_safelock_in(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_lock_safe_info(ZONESVRENV* pstEnv, Player *pstPlayer, int iFlag);
int player_check_safelock(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_span_safelock(ZONESVRENV* pstEnv, SSSPANSAFELOCK *pstSpanSafeLock);

#endif