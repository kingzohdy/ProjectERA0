
#ifndef ZONE_STORE_H
#define ZONE_STORE_H

#include "zone_svr.h"
#include "zone_db.h"
#include "role_db_meta.h"
#include "proto_comm.h"

void store_player_login(ZONESVRENV* pstEnv, Player *pstPlayer);
int z_db_store_finish(ZONESVRENV *pstEnv);
int z_db_store_is_allclean(ZONESVRENV *pstEnv);
int store_db_insert_res(ZONESVRENV* pstEnv, Player *pstPlayer, DBAsyn *pstDBAsyn, TORMINSERTRES *pstInsertRes);
int store_select_res(ZONESVRENV *pstEnv, TORMSELECTRES *pstSelectRes);
int store_update_res(ZONESVRENV *pstEnv, TORMPKG *pstTormPkg, DBAsyn *pstDBAsyn);
int store_db_count_res(ZONESVRENV *pstEnv, TORMSELECTCOUNTRES *pstSelectCountRes);
int store_db_delete_res(ZONESVRENV* pstEnv, DBAsyn *pstDBAsyn, TORMDELETERES *pstRes);
int z_active_store(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, int iCount);
int store_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
					TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int store_init(ZONESVRENV *pstEnv);

int store_open(ZONESVRENV* pstEnv, Player *pstPlayer, STOREOPEN *pstOpen);
int store_op_close(ZONESVRENV* pstEnv, Player *pstPlayer, STORECLOSE*pstClose);
int store_item_up(ZONESVRENV* pstEnv, Player *pstPlayer, STOREUP*pstUp);
int store_process_role_chg_name(ZONESVRENV *pstEnv, Player *pstPlayer);
int store_open_ui_notify(ZONESVRENV* pstEnv, Player *pstPlayer,  int iPage, int iCount, int iNpcID, STORE *pstStore);
ZoneStore *store_player_get(ZONESVRENV* pstEnv, Player *pstPlayer);
int stroe_open_ui_check(ZONESVRENV* pstEnv, Player *pstPlayer);
#endif