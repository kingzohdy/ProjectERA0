#ifndef ZONE_HOME_H
#define ZONE_HOME_H

#include "zone_svr.h"
#include "zone_db.h"

int home_db_count_res(ZONESVRENV *pstEnv, TORMSELECTCOUNTRES *pstSelectCountRes);
int z_active_home(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, int iCount);
int home_build(ZONESVRENV *pstEnv, HOME *pstHome);
int home_db_delete_res(ZONESVRENV* pstEnv, DBAsyn *pstDBAsyn, TORMDELETERES *pstRes);
HOME* home_one_find_by_id(ZONESVRENV* pstEnv,int iID);
ZoneHome* zonehome_one_find_by_id(ZONESVRENV* pstEnv,int iID);
int home_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
					TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int home_page(ZONESVRENV* pstEnv, Player *pstPlayer, int iPage);
int home_login_check(ZONESVRENV* pstEnv,Player *pstPlayer);
int home_farm_notify(ZONESVRENV* pstEnv, Player *pstPlayer, HOME *pstHome, ZONEMAPINST *pstMInst);
int home_player_is_in(ZONESVRENV* pstEnv, Player *pstPlayer, int *piIn);
int home_pet_notify(ZONESVRENV* pstEnv, Player *pstPlayer, HOME *pstHome, ZONEMAPINST *pstMInst);
int home_farm_tick(ZONESVRENV* pstEnv, HOME *pstHome);
void home_pet_init(HOME *pstHome, int iID);
int home_single_notify(ZONESVRENV* pstEnv, Player *pstPlayer, HOME *pstHome);
int home_init(ZONESVRENV *pstEnv);
int home_select_res(ZONESVRENV *pstEnv, TORMSELECTRES *pstSelectRes);
int home_update_res(ZONESVRENV *pstEnv, TORMPKG *pstTormPkg, DBAsyn *pstDBAsyn);
int home_destory(ZONESVRENV* pstEnv, Player *pstPlayer, HOMEDESTORY*pstDestory);
int home_new(ZONESVRENV* pstEnv, Player *pstPlayer, HOMENEW *pstNew);
int home_enter(ZONESVRENV* pstEnv, Player *pstPlayer, HOMEENTER*pstEnter);
int home_farm_add_exp(ZONESVRENV* pstEnv, Player *pstPlayer, int iExp);
int home_farm_kaiken(ZONESVRENV* pstEnv, Player *pstPlayer);
int home_init_all_pworld(ZONESVRENV *pstEnv);
int z_db_home_finish(ZONESVRENV *pstEnv);
int home_db_delete_req( ZONESVRENV* pstEnv, Player *pstPlayer,HOME *pstHome);
int home_db_insert_req( ZONESVRENV* pstEnv,Player *pstPlayer, HOME *pstHome);
int home_db_insert_res(ZONESVRENV* pstEnv, Player *pstPlayer, DBAsyn *pstDBAsyn, TORMINSERTRES *pstInsertRes);
int home_player_login_in(ZONESVRENV* pstEnv, Player *pstPlayer);
int home_get_soil_level(ZONESVRENV* pstEnv, Player *pstPlayer);
int home_player_get_kaiken_num(ZONESVRENV* pstEnv, Player *pstPlayer);
HOMESOILDEF *home_soil_def_find(ZONESVRENV* pstEnv, int iNum);
int home_player_get_next_kaiken_info(ZONESVRENV* pstEnv, Player *pstPlayer,int iType);
int home_adjust_jiaju(ZONESVRENV* pstEnv, Player *pstPlayer,  HOMEJIAJUADJUST *pstJiaJuAdjust);
HOMEFARMDEF *home_farm_def_find(ZONESVRENV* pstEnv, int iLvl);




#endif

