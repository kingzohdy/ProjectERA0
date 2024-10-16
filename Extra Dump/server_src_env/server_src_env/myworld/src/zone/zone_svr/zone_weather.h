#ifndef ZONE_WEATHER_H
#define ZONE_WEATHER_H

#include "zone_svr.h"

int weather_tick(ZONESVRENV *pstEnv);

int weather_set(ZONESVRENV *pstEnv, Player *pstPlayer, int iType);
int weather_player_login(ZONESVRENV *pstEnv, Player *pstPlayer);
int weather_init(ZONESVRENV *pstEnv);

#endif 