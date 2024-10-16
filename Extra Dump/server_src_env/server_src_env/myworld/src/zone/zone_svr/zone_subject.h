#ifndef ZONE_SUBJECT_H
#define ZONE_SUBJECT_H

#include "zone_svr.h"


#define SUBJECT_LOCK_END 10
#define SUBJECT_RES_END 25
#define SUBJECT_EVENT_ID 49



int subject_check_stat(ZONESVRENV* pstEnv,GameEventInst *pstEvent);
int subject_clt_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
SUBJECTDEF* subject_def_find(ZONESVRENV* pstEnv, int iID);

int subject_player_login(ZONESVRENV* pstEnv, Player *pstPlayer);
int subject_player_logout(ZONESVRENV* pstEnv, Player *pstPlayer);
int subject_player_info_find(ZONESVRENV* pstEnv, Player *pstPlayer);
int subject_db_pre(ZONESVRENV* pstEnv);
SUBJECTTYPEDEF* subject_type_def_find(ZONESVRENV* pstEnv, int iIdx);
int subject_type_db_pre(ZONESVRENV* pstEnv);
SUBJECTDEF* subject_get_theme(ZONESVRENV* pstEnv);
int subject_player_is_join(ZONESVRENV* pstEnv, Player *pstPlayer);
int subject_get_top_award_libao(ZONESVRENV* pstEnv);

#endif
