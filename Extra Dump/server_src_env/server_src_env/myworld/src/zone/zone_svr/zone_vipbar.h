#ifndef ZONE_VIPBAR_H
#define ZONE_VIPBAR_H

#include "zone_svr.h"

VIPBARDEF * z_find_vip_bar_def(ZONESVRENV *pstEnv,unsigned int uiKey);
int shopping_db_vip_bar_def_count_req(ZONESVRENV *pstEnv);
int shopping_db_vip_bar_def_count_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg);
int hash_insert_vip_bar_def(ZONESVRENV *pstEnv,unsigned int uiKey,char* pszData,unsigned int iSize);
int shopping_db_vip_bar_def_select_req( ZONESVRENV* pstEnv,unsigned int iBeginRecord);
int shopping_db_vip_bar_def_select_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg);

VIPBARLEVELDEF * z_find_vip_bar_level_def(ZONESVRENV *pstEnv,unsigned int uiKey);
int shopping_db_vip_bar_level_def_count_req(ZONESVRENV *pstEnv);
int shopping_db_vip_bar_level_def_count_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg);
int hash_insert_vip_bar_level_def(ZONESVRENV *pstEnv,unsigned int uiKey,char* pszData,unsigned int iSize);
int shopping_db_vip_bar_level_def_select_req( ZONESVRENV* pstEnv,unsigned int iBeginRecord);
int shopping_db_vip_bar_level_def_select_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg);

VIPBARNUMDEF * z_find_vip_bar_num_def(ZONESVRENV *pstEnv,unsigned int uiKey);
int shopping_db_vip_bar_num_def_count_req(ZONESVRENV *pstEnv);
int shopping_db_vip_bar_num_def_count_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg);
int hash_insert_vip_bar_num_def(ZONESVRENV *pstEnv,unsigned int uiKey,char* pszData,unsigned int iSize);
int shopping_db_vip_bar_num_def_select_req( ZONESVRENV* pstEnv,unsigned int iBeginRecord);
int shopping_db_vip_bar_num_def_select_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg);

int z_check_vip_bar_all_loaded(ZONESVRENV *pstEnv);
int z_load_vip_bar_data(ZONESVRENV *pstEnv);


int player_vip_bar_check(ZONESVRENV *pstEnv,Player *pstPlayer);
int player_vip_bar_buff(ZONESVRENV *pstEnv,Player *pstPlayer,int iOpType);
int player_vip_bar_buff_chg_num(ZONESVRENV *pstEnv,Player *pstPlayer,int iOpType,int iChg);

VIPBARONLINE * z_find_vip_bar_online(ZONESVRENV *pstEnv,unsigned int uiKey);
int hash_insert_vip_bar_online(ZONESVRENV *pstEnv,unsigned int uiKey,char* pszData,unsigned int iSize);

#endif /* ZONE_SHOP_H */
