#ifndef ZONE_SYNCRELATION_H
#define ZONE_SYNCRELATION_H

#include "zone_svr.h"

int is_school_student(ZONESVRENV* pstEnv, Player *pstPlayer);

int school_role_login(ZONESVRENV* pstEnv, Player *pstPlayer);

int role_sync_relation_select_res(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes,DBAsyn *pstDbsyn);

int role_base_db_res(ZONESVRENV* pstEnv, Player *pstPlayer, TORMSELECTRES *pstSelectRes,DBAsyn *pstDbsyn);

int player_add_school_val(ZONESVRENV* pstEnv, Player *pstPlayer, int iAddVal);

int player_pworld_award_schoolval(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_school_fini(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_open_school_invite_ui(ZONESVRENV* pstEnv, Player *pstPlayer);

int school_role_logout(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_school_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_school_daylimit_check(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_school_daylimit_add(ZONESVRENV* pstEnv, Player *pstPlayer);

int school_student_levelup(ZONESVRENV* pstEnv, Player *pstPlayer);
ROLESTUDENTINFO* school_teacher_get_stuinfo(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong ullRoleID);
int role_sync_relation_select_req(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong ullRoleID, DBAsyn *pstDBAsyn);
int player_chuancheng_notify(ZONESVRENV* pstEnv, Player *pstPlayer);


int player_new_people_column_clt(ZONESVRENV *pstEnv,Player* pstPlayer,CSNEWPEOPLECLT *pReq);
int all_new_people(ZONESVRENV* pstEnv,int type,Player* pstPlayer);
int player_obtian_new_people_to_clt(ZONESVRENV* pstEnv, Player **apstPlayers, Player *pstPlayer,int iCount);
int player_new_people_column_svr(ZONESVRENV* pstEnv, Player *pstPlayer,CSNEWPEOPLEINFO *pstNewPeopleInfo);
int new_people_tick(ZONESVRENV* pstEnv);

int school_check_data(ZONESVRENV* pstEnv, Player *pstPlayer);
int chg_role_name_notify_player(ZONESVRENV* pstEnv, Player *pstPlayer,tdr_ulonglong ullRoleID, const char* pszNew, const char* pszOld);






#endif /* ZONE_SYNCRELATION_H */
