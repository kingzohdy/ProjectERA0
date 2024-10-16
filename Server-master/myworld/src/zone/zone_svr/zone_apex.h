#ifndef ZONE_APEX_H
#define ZONE_APEX_H

#include "zone_svr.h"

int apex_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int z_ss_apex_get_res(ZONESVRENV* pstEnv, SSPKG *pstPkg);

#endif	/*ZONE_APEX_H*/

