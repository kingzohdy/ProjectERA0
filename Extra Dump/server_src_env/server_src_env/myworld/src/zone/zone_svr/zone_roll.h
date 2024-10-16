#ifndef ZONE_ROLL_H
#define ZONE_ROLL_H

#include "zone_svr.h"

#define TEAM_ROLL_BOOTY_TIME                           	60
#define CLAN_ROLL_BOOTY_TIME                           	120

int is_wait_roll_drop_item(DropItem *pstDropItem);

int is_rolling_drop_item(DropItem *pstDropItem);

int is_collect_drop_item(DropItem *pstDropItem);

int team_booty_active(ZONESVRENV * pstEnv, ZoneTeam * pstTeam);

int team_roll_booty_start(ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstRollItem);

int player_roll_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_team_chg_booty_mode(ZONESVRENV* pstEnv, Player *pstPlayer, CSTEAMCHGBOOTYMODE *pstChg);

int team_booty_mode_chg(ZONESVRENV* pstEnv, ZoneTeam *pstTeam, int iMode);

int team_booty_destroy(ZONESVRENV* pstEnv, ZoneTeam *pstTeam);

int team_booty_player_exit(ZONESVRENV* pstEnv, ZoneTeam *pstTeam, Player *pstPlayer);

int booty_mode_to_string(int iTeamBootyMode, char* pszBuff, int iSize);

int player_roll_booty_start(ZONESVRENV* pstEnv, Player *pstPlayer, DropItem *pstRollItem);

int clan_booty_active(ZONESVRENV* pstEnv, ZoneClan *pstClan);

#endif /* ZONE_ROLL_H */
