

#ifndef ZONE_SPAN_STRONG_H
#define ZONE_SPAN_STRONG_H

#include "zone_svr.h"

int span_global_init(ZONESVRENV* pstEnv);
RSTRONGONE *span_strong_one_find(ZONESVRENV* pstEnv, int iStrongID);
int span_strong_get_map_pos(ZONESVRENV* pstEnv, int iStrongID, MapIndex *pstMapIdx, 
								RESPOS *pstPos, Player *pstPlayer, int iRevive);
SPANSTRONGDEF * span_strong_def_find(ZONESVRENV* pstEnv, int iStrongID);
int span_strong_all_world_notify(ZONESVRENV* pstEnv);
int strong_get_enter_pos(ZONESVRENV* pstEnv, ZONEPWORLD *pstPworld, MapIndex *pstMapIdx, 
								RESPOS *pstPos, Player *pstPlayer, int iRevive);
int span_strong_pworld_info(ZONESVRENV* pstEnv, Player *pstPlayer, int iSingle, ZONEMAPINST *pstMInst, int iStrongID);
int span_strong_ss_req(ZONESVRENV * pstEnv,SSPKG *pstPkg);
int span_strong_ss_res(ZONESVRENV * pstEnv,SSPKG *pstPkg);
int span_global_res(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes);
int span_global_tick(ZONESVRENV* pstEnv);
int span_global_info_req(ZONESVRENV* pstEnv);
int span_strong_resmon_die(ZONESVRENV* pstEnv, Monster* pstMon, MONSTERDEF *pstMonDef, Player *pstKiller);
int span_strong_get_camp(ZONESVRENV* pstEnv, Player *pstPlayer, int iStrongID);
int span_strong_refresh_map_inst(ZONESVRENV* pstEnv,ZONEPWORLDINST *pstZInst,ZONEMAPINST* pstMInst, int StrongMapID);
int span_strong_clt_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int span_strong_clt_sale(ZONESVRENV* pstEnv, Player *pstPlayer, SPANSTRONGSALE *pstSale);
int span_strong_clt_enter(ZONESVRENV* pstEnv, Player *pstPlayer, SPANSTRONGENTER*pstEnter);
int span_strong_player_die(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstKiller);
int span_strong_war_one_notify(ZONESVRENV* pstEnv, Player *pstPlayer, int iStrongID);
int strong_is_span(ZONEPWORLD *pstPworld);
int span_strong_war_all_notify(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPInst, int iEnd, int iStrongID);
int span_strong_pworld_tick(ZONESVRENV* pstEnv, ZONEPWORLD *pstZonePworld, 
								ZONEPWORLDINST *pstPInst, int iFlag);
int span_strong_one_world_notify(ZONESVRENV* pstEnv, RSTRONGONE *pstOne);
int span_strong_pworld_res_add(ZONESVRENV* pstEnv, SPANSTRONGDEF *pstDef,RSTRONGONE* pstOne);
int span_strong_enter_init(ZONESVRENV * pstEnv, Player *pstPlayer,ZONEPWORLD *pstPworld);
int span_strong_player_money_award(ZONESVRENV* pstEnv, Player *pstPlayer);
void span_strong_occ_info_notify(ZONESVRENV* pstEnv, Player *pstPlayer);
int span_strong_chg_name(ZONESVRENV* pstEnv, int iType, const char* pszOld, const char *pszNew);
int span_strong_clan_destory(ZONESVRENV* pstEnv, const char* pszClanName);
int span_strong_gm_occ_ss_req(ZONESVRENV* pstEnv, ZoneClan*pstClan, int iStrongID);
int span_strong_pworld_is_open_by_clan(ZONESVRENV* pstEnv, int iClanID, int iSvrID);
int span_strong_exit_clear_buff(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLD *pstPworld);
int span_msg_world_notify(ZONESVRENV* pstEnv, SSPKG *pstPkg, unsigned int iSrcID);
#endif
