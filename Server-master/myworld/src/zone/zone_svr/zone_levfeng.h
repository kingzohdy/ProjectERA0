#ifndef ZONE_LEVFENG_H
#define ZONE_LEVFENG_H

#include "zone_svr.h"

int level_feng_levelup(ZONESVRENV *pstEnv, Player *pstPlayer);
int level_feng_update(ZONESVRENV *pstEnv);
int level_feng_clear(ZONESVRENV *pstEnv);
int level_feng_get_expdec(ZONESVRENV *pstEnv, Player *pstPlayer);
int level_get_curr_fy(ZONESVRENV *pstEnv);
#endif	/*ZONE_LEVFENG_H */

