#ifndef ZONE_GENIUS_H
#define ZONE_GENIUS_H
#define GENIUS_OPEN_LVL 60

#include "zone_svr.h"
#include "zone_combat.h"

int genius_player_check(ZONESVRENV *pstEnv, Player *pstPlayer);
int genius_player_open(ZONESVRENV *pstEnv, Player *pstPlayer, int iNotify);
int genius_clt_op_up( ZONESVRENV* pstEnv, Player *pstPlayer, GUNIUSUPCLT  *pstUp);
int genius_check_level_up(ZONESVRENV* pstEnv, Player *pstPlayer,  LEVELUP *pstLvlUp);
int genius_cacl_attr(ZONESVRENV* pstEnv, Player *pstPlayer);
int genius_clt_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int genius_list_notify(ZONESVRENV* pstEnv, Player *pstPlayer);
int genius_gm_clear(ZONESVRENV* pstEnv, Player *pstPlayer);
int genius_get_mod(ZONESVRENV* pstEnv, Player *pstPlayer);
GENIUSATTRDEF *genius_attr_def_find(ZONESVRENV* pstEnv, int iCareer, int iAttrID, int iLvl);
int genius_get_mod1(ROLEDATA *pstRoleData);
int genius_hurt_mul_get(ZONESVRENV *pstEnv, AtkerInfo *pstAtkerInfo, AtkeeInfo *pstAtkeeInfo);
int genius_get_min_lvl(Player *pstPlayer, int iType);
int genius_hurt_dec_get(ZONESVRENV *pstEnv, AtkerInfo *pstAtkerInfo, AtkeeInfo *pstAtkeeInfo);
int genius_get_mod_by_type(ZONESVRENV* pstEnv, Player *pstPlayer, int iType);
GENIUSCAREERDEF *genius_career_def_find(ZONESVRENV* pstEnv, int iCareer);

#endif

