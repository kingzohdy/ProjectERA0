
#ifndef ZONE_FAIRY_H
#define ZONE_FAIRY_H

#include "zone_svr.h"

int fairy_born_check(ZONESVRENV* pstEnv, Player *pstPlayer,int iFairyID);
ROLEFAIRY * fairy_born(ZONESVRENV* pstEnv, Player *pstPlayer,int iFairyID);
int fairy_view(ZONESVRENV* pstEnv, Player *pstPlayer, CSROLEFAIRYVIEW *pstRoleFairyView);
int player_fairy_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int fairy_up_online(ZONESVRENV* pstEnv,Player *pstPlayer,ROLEFAIRY *pstRoleFairy);
int fairy_up_online_by_login(ZONESVRENV* pstEnv,Player *pstPlayer);
int fairy_diath_tick(ZONESVRENV* pstEnv, Player *pstPlayer);
int fairy_evolve_tick(ZONESVRENV* pstEnv, Player *pstPlayer);
int fairy_evolve_stop_by_logout(ZONESVRENV* pstEnv,Player *pstPlayer);
int set_fairy_camp(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEFAIRY *pstRoleFairy);
int set_fairy_drop(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEFAIRY *pstRoleFairy);
FAIRYDEF *faity_find(ZONESVRENV* pstEnv, int iFairyID, int iLv);
int fairy_evolve_update_time(ZONESVRENV* pstEnv, Player *pstPlayer);
int fairy_evolve_logout(ZONESVRENV* pstEnv, Player *pstPlayer);
int fairy_camp_clean_xp(ZONESVRENV* pstEnv, Player *pstPlayer);
int fairy_chg_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,ROLEFAIRY *pstRoleFairy);
ROLEFAIRY *get_pack_fairy(Player *pstPlayer,tdr_ulonglong ullWID);
ROLEFAIRY * get_evolve_fairy(Player *pstPlayer);
int fairy_evolve_use_item(ZONESVRENV* pstEnv, Player *pstPlayer);
int fairy_back_in(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEFAIRY *pstRoleFairy);
ROLEFAIRY* get_pack_fairy_by_id(Player *pstPlayer, int iFairyID);
int fairy_level_up(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEFAIRY *pstRoleFairy);
int fairy_is_tongling(ZONESVRENV* pstEnv, Player *pstPlayer);
ROLEFAIRY *fairy_get_camp(ZONESVRENV* pstEnv, Player *pstPlayer);
//int fairy_tongling_up_attr(ZONESVRENV* pstEnv, Player *pstPlayer, PetFightAttrOnline *pstPetFightAttrOnline);
int fairy_clean_keyin_info(ZONESVRENV* pstEnv, Player *pstPlayer);
int fairy_tongling_up_attr(ZONESVRENV* pstEnv, Player *pstPlayer, PetFightAttrOnline *pstPetFightAttrOnline, PetExpOnline *pstPetExpOnline);
int fairy_evolve_update_lift_time(ZONESVRENV* pstEnv, Player *pstPlayer);
int fairy_camp_pre_brk(ZONESVRENV* pstEnv, Player *pstPlayer,tdr_ulonglong ullWID);
int fairy_add_xp_by_sec(ZONESVRENV* pstEnv,Player *pstPlayer);

#endif


