
#ifndef ZONE_MAP_H
#define ZONE_MAP_H

#include "zone_svr.h"
#include "proto_cs.h"
#include "zone_task.h"
#include "mapmask.h"

#define PWORLD_LEVEL_NORMAL 0
#define PWORLD_LEVEL_JINYING 1		


extern int g_RanPosDirCurr;
extern int g_RanPosFaceCurr;
extern int g_RanPosFaceIdx;

extern int g_iMapPlayerNormalNum;
extern int g_iMapPlayerBusyNum;




static inline int SET_RANPOS_DIR(int iDir, int iFace, int iFaceIdx)
{
	g_RanPosDirCurr = iDir;
	g_RanPosFaceCurr = iFace%3;
	g_RanPosFaceIdx = iFaceIdx%6;

	return 0;
}

char *trim(char *str);
int z_init_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv);
int z_init_pworld(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv);
int z_check_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv);
MapIndex* z_mappos_index(MapObj *pstMap, int *piMapPos, int iMapIndex);
ZONEMAPINST*  z_find_map_inst(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEIDX *pstIdx);
int z_player_entermap(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iMapInstID, RESPOS *pstPos, int iChg);
int z_player_leave_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iChgMap);
int z_get_safe_mappos(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int *piMap, RESPOS *pstPos);
ZONEPWORLDINST* z_find_pworld_inst(MapObj* pstMap, int iPworldInstID);
ZONEPWORLD* z_find_pworld(MapObj* pstMap, ZONEIDX* pstIdx);
int z_valid_pos(MapObj *pstMap, int iMapIndex, int *piMapPos, RESPOS *pstPos);
int z_get_ran_pos(ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst,MapIndex *pstMapIdx, RESPOS *pstCurr, RESPOS* pstOrigin, int iMaxDist, int iMinDist,  RESPOS* pstPos);
MapIndex* z_map_inst_index(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEIDX *pstIdx);
int z_player_change_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, int iMapIndex, RESPOS *pstPos, unsigned char bDirection, int iCameraDir);
int z_monster_leave_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon);
int z_safe_monster_entermap(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster* pstMon, int iMap, RESPOS* pstPos);
MONSTERDEF * z_get_mon_def(ZONESVRENV* pstEnv, Monster* pstMon);
Monster* z_new_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster* pstMon, MONSTERDEF *pstMonDef);

ZONEPWORLDINST *z_find_id_pworld_inst_byteam(ZONESVRENV * pstEnv, ZoneTeam *pstZoneTeam, int iPworldID);
ZONEPWORLDINST *z_find_pworld_inst_bysingle(ZONESVRENV * pstEnv, Player *pstPlayer);
ZONEPWORLDINST *z_find_player_pworld_inst(ZONESVRENV * pstEnv, Player *pstPlayer);

int z_in_destroy_pworld_inst(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst);
int player_pworld_exit_chgmap(ZONESVRENV* pstEnv, Player *pstPlayer);
int z_active_pworld(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iCount);
int z_player_enter_pworld(ZONESVRENV * pstEnv, Player *pstPlayer, int iPworldID, PWORLDSELECT *pstPworldSelect, int iTrans);
int z_item_enter_map(ZONESVRENV * pstEnv, DropItem *pstDropItem);
int z_item_leave_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, DropItem *pstDItem);
int z_item_clean(ZONESVRENV* pstEnv, DropItem *pstDItem);
int z_in_new_pworld_npc(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, MapIndex* pstMapIndex,
								ZONEMAPINST* pstMapInst, MAPNPC *pstMapNpc,int iCampIdx);
int player_close_pworld(ZONESVRENV * pstEnv, Player *pstPlayer, int iPworldID);

int z_clear_map_mon(ZONESVRENV* pstEnv, ZONEIDX *pstMapInstIdx);

void pworld_team_together(ZONESVRENV * pstEnv, ZoneTeam *pstZoneTeam, ZONEPWORLDINST *pstZonePworldInst, int fCaptain) ;

ZONEMAPINST*  z_find_map_inst(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEIDX *pstIdx);
int z_valid_pos(MapObj *pstMap, int iMapIndex, int *piMapPos, RESPOS *pstPos);

MONDYNATTRDEF *z_find_mondyn(ZONESVRENV* pstEnv, unsigned int uiLevel);

int open_pworld_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID);

int player_enter_dyn_pworld(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int make_tmp_item(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, int iItemID, int iNum);

int z_do_call_npc(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,MapIndex* pstMapIndex,
								ZONEMAPINST* pstMapInst, MAPNPC *pstMapNpc, int iLifeTime);

int z_player_call_npc(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, int iNpcDefID, int iNpcLifeTime);

Npc* z_new_npc(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Npc* pstNpc);

int z_enter_map_check_pet(ZONESVRENV* pstEnv, Player *pstPlayer,MapIndex* pstMapIdx,int iTip);
int z_enter_pworld_check_pet(ZONESVRENV* pstEnv, Player *pstPlayer,ZONEPWORLD *pstZonePworld,int iTip);

int z_player_enter_pworld_map(ZONESVRENV * pstEnv, Player *pstPlayer, ZONEPWORLDINST *pstZonePworldInst, 
								ZONEPWORLD *pstZonePworld);

ZONEPWORLDINST* z_in_new_pworld_inst(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEPWORLD* pstPworld, PWORLDSELECT *pstPworldSelect, int iPlayerMeanLevel, int iTimeout);

ZONEPWORLDINST* z_new_ploy_pworld_inst(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, 
				ZONEPWORLD * pstPworld, int iTimeout,int iLevel);

 //int z_player_restore_fairy(ZONESVRENV* pstEnv, Player* pstPlayer);

int z_clear_load_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv);

int pworld_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int z_poly_pworld_role_info(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, ZONEPWORLD *pstZonePworld, int iSort);

int accord_player_item(ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID, int iNum, int iMail,int iLogType,	
						char *pszTitle,char *pszText , int iBind);

int player_find_notenteragain(ZONESVRENV * pstEnv, Player *pstPlayer, ZONEPWORLDINST *pstZonePworldInst);

void pworld_change_single(ZONEPWORLDINST *pstZonePworldInst, Player *pstPlayer);

//战场上报副本信息
int z_battle_report_pworld_info(ZONESVRENV* pstEnv,TAPPCTX* pstAppCtx);

// 玩家进入战场请求
int z_battle_player_enter_pworld(ZONESVRENV* pstEnv,TAPPCTX* pstAppCtx,Player *pstPlayer);

// 销毁副本时调用 删除副本信息
int z_del_battle_pworld_info(ZONESVRENV* pstEnv, int iBattleBusID, tdr_ulonglong ullPworldWID);
// 推出副本时调用 维护副本人数
int z_del_battle_player_num(ZONESVRENV* pstEnv, int iBattleBusID, tdr_ulonglong ullPworldWID);

int z_sendpkg_mon_area_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster* pstMon, CSPKG *pstPkg);

int z_change_battle_map_check(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer);

int pworld_limit_info_get(ZONESVRENV* pstEnv, Player* pstPlayer);

int z_add_battle_player_num(ZONESVRENV* pstEnv, int iBattleBusID, tdr_ulonglong ullPworldWID);

int bct_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
RNDAWDDEF *z_find_rndawddef(ZONESVRENV* pstEnv, int iIdx);
BCTDEF *z_find_bctdef(ZONESVRENV* pstEnv, int iLayer);
int bct_layer_history_notify(ZONESVRENV* pstEnv, Player *pstPlayer, int iLayer);
int open_bct_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int iNpcID);

int bct_self_top(ZONESVRENV* pstEnv, Player *pstPlayer);
int bct_exit_info(ZONESVRENV* pstEnv, Player *pstPlayer);
int bct_info(ZONESVRENV* pstEnv, Player *pstPlayer);
int bct_layer_history_notify(ZONESVRENV* pstEnv, Player *pstPlayer, int iLayer);
BCTMON* get_mon_bct_dyn(ZONESVRENV* pstEnv, Monster *pstMon);

int z_role_login_entermap_test(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer);

//获取顺时针圆上的某个点,iAngle相对于pstOrigin这个点的角度
int z_get_clockwise_pos(ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst, MapIndex *pstMapIdx, SKILLDEF *pstSkillDef,
							RESPOS	* pstCurr, RESPOS* pstOrigin, int iMaxDist, int iMinDist,int iAngle, RESPOS* pstPos);

extern int g_iCreatePloyPworldPlayerNum;


int dyn_msk_check(ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst, TMAPMASK* pstMask, int lPixelX1, int lPixelY1, int lPixelX2, int lPixelY2,
						int iMiscType,int iBit);

int player_limit_time_pworld_check(ZONESVRENV* pstEnv, Player *pstPlayer, PWORLDDEF *pstPworldDef, tdr_ulonglong ullPworldWid);

int z_clear_all_item(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv);

int z_in_destroy_areadyn(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEPWORLD* pstPworld, 
							ZONEPWORLDINST *pstPworldInst, AREADYN* pstAreaDyn, int iCleanRoleFlag);

ZONEPWORLDINST* z_new_single_pworld_inst(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, Player *pstPlayer, ZONEPWORLD * pstPworld, PWORLDSELECT *pstPworldSelect, int iTimeout);

ZONEPWORLDINST* z_new_team_pworld_inst(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, ZoneTeam * pstZoneTeam, ZONEPWORLD * pstPworld, PWORLDSELECT * pstPwolrdSelect, int iTimeout);

int z_in_destroy_map_inst(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEPWORLD* pstPworld, ZONEPWORLDINST *pstPworldInst, ZONEIDX *pstMapInstIdx);

int z_in_new_map_inst(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iPworldInstID, PWORLDSELECT *pstSelect, ZONEIDX *pstIdx);

void player_insert_limit_pworld(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLDINST *pstPworldInst,int AddFlag);
int z_active_mapinst(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iCount);

int is_refer_line_map(MapIndex* pstMapIdx);

int is_map_valid_line(MapIndex* pstMapIdx, MapIndex* pstLine);

int z_mapdef_pre(ZONESVRENV* pstEnv);

int map_line_info(ZONESVRENV* pstEnv, Player *pstPlayer, MapIndex* pstMapIdx);

int z_find_map_byline(MapIndex* pstMapIdx, int iLine);

int map_change_line(ZONESVRENV* pstEnv, Player *pstPlayer, CSMAPCHGLINE *pstChg);

int map_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
	TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int map_pworld_chg_limitflag(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);

LIMITPWORLD *player_check_same_pworld_by_wid(ZONESVRENV* pstEnv, ZONEPWORLDINST* pstPworldInst,Player* pstPlayer);

int pworld_again(ZONESVRENV* pstEnv, ZONEPWORLD *pstPworld,ZONEPWORLDINST *pstPworldInst,
					int iMap, RESPOS *pstPos);

int is_die_exit_pworld_revive(ZONEPWORLD *pstZonePworld);

int get_battle_enter_validtime(ZONEPWORLD *pstPworld);
int is_in_bct(ZONESVRENV* pstEnv, Player *pstPlayer);

int z_check_area_dyn(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv);
int z_check_pworld_inst(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv);
int z_check_map_inst(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv);

int z_player_change_map2(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, int iMapIndex, RESPOS *pstPos, unsigned char bDirection, int iCameraDir);

int map_broadcast(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst, CSPKG *pstPkg);

int newnpc_add(ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, Npc *pstNpc);

int z_mon_dyn_mask_set(ZONESVRENV* pstEnv, MapIndex *pstMapIdx, ZONEMAPINST* pstMapInst, Monster* pstMon, MONSTERDEF *pstMonDef, int iFlag);

int z_check_pworld_enter_num(ZONESVRENV * pstEnv, Player *pstPlayer, PWORLDDEF *pstPworldDef, int iNotify);

int z_assign_player_item(ZONESVRENV* pstEnv, Player *pstPlayer, char *pszTitle,
							 char *pszText, int iItemID, int iNum, int iMail, ITEMDEC *pstOpGetItem);

int z_check_enter_again(ZONESVRENV *pstEnv,Player *pstPlayer, ZONEPWORLD *pstZonePworld);

int z_pworld_queue_notify_region(ZONESVRENV* pstEnv,PworldApplyQueue *pstQueue);

int map_dynmsk_mon_memid_add(ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, int iMemID);

int map_dynmsk_mon_memid_del(ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, int iMemID);

int z_check_single_pworld(ZONESVRENV * pstEnv, Player *pstPlayer, PWORLDDEF *pstPworldDef, tdr_ulonglong ullPworldWid, CSDIFFPWORLDTEAMTIPS *pstTeamTips);

void z_get_team_avg_level(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, ZoneTeam *pstZoneTeam, int *piLevelVal);

void z_get_team_min_level(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, ZoneTeam *pstZoneTeam, int *piLevelVal);

int z_sendpkg_mon_area_view_except(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster* pstMon, 
		CSPKG *pstPkg, Player *pstExceptPlayer);

int z_load_ore_position(ZONESVRENV* pstEnv);
int player_pworld_exit_ctrl(ZONESVRENV* pstEnv, Player *pstPlayer,ZONEPWORLD *pstExitZonePworld);
ZONEPWORLD *z_find_pworld2(ZONESVRENV* pstEnv, int iMap);
int player_home(ZONESVRENV* pstEnv, Player *pstPlayer, int iMap, RESPOS *pstPos);
int map_check_lineseg_endpos(TMAPMASK* pstMask,RESPOS *pstFrom,RESPOS *pstTo);
int map_check_safe_pos(TMAPMASK* pstMask,RESPOS *pstPos);
RESPOS z_rotate_pos(RESPOS o,int iAngle,RESPOS p);

int is_player_in_pworld(Player *pstPlayer);

int is_map_same_line(ZONESVRENV* pstEnv, int iMainLine, int iOtherLine);
int is_map_same_refer(ZONESVRENV* pstEnv, int iMainMap, int iOtherMap);

int z_player_enter_pworld_by_inst(ZONESVRENV * pstEnv, Player *pstPlayer, ZONEPWORLD *pstZonePworld, 
										ZONEPWORLDINST *pstZonePworldInst);

MAPMONSTER * get_rand_map_mon_def(ZONESVRENV* pstEnv, int iMapID,int iExceptSafeArea);
int get_map_rand_mon_memid(ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst);

ZONEPWORLDINST * z_find_pworldinst2(ZONESVRENV* pstEnv, int iMap);

int z_in_new_pworld_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, MapIndex* pstMapIndex,
									ZONEMAPINST* pstMapInst, MAPMONSTER *pstMapMonster);
int z_player_check_map_timelimit(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_chg_map_by_mapid(ZONESVRENV* pstEnv, Player *pstPlayer,int iMapID,RESPOS *pstPos);
int player_chg_map_timelimit(ZONESVRENV* pstEnv, Player* pstPlayer,MapIndex *pstIdx,int *piTime, int iLogin);
int map_mon_broadcast(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEMAPINST* pstMapInst);

int player_login_single_pworld_update(ZONESVRENV * pstEnv, Player *pstPlayer);


int bct_continue_clean_item(ZONESVRENV* pstEnv, ZONEIDX *pstMapInstIdx);

int z_player_enter_pworld_inst(ZONESVRENV* pstEnv, Player *pstPlayer, 
				int iPworldID);
int player_change_map_check(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_is_in_logout_to_revivepoint_map(ZONESVRENV* pstEnv, Player *pstPlayer);
/*
 * 怪物/玩家统计相关:
 */
typedef struct tagCountCookie
{
	const Monster *pstMon;
	Player *pstPlayer;
	RESPOS stPoint;
	int iRadius;
	int iExtra;
}CountCookie;

typedef int (*CountPred)(ZONESVRENV *, int, void *, int *);

int map_entity_count_if(ZONESVRENV *pstEnv,
                               const ZONEMAPINST *pstMapInst,
                               const void *pvCookie,
                               CountPred pfnPred);

int AliveFlagMon(ZONESVRENV *pstEnv, int iID, void *pvCookie,
                    int *piCount);

int pworld_wave_countdown_action(ZONESVRENV* pstEnv,ZONEPWORLDINST *pstPworldInst);
int z_pworld_over(ZONESVRENV* pstEnv,ZONEPWORLDINST *pstPworldInst);

int is_player_zhongli_area(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_pworld_apply_invite_in(ZONESVRENV* pstEnv,ZoneClan* pstClan,Player * pstPlayer,CSPWORLDAPPLYINVITE *pstInvite);

int player_is_in_clan_shuniu(Player *pstPlayer);
int player_chg_map_passenger_check(ZONESVRENV* pstEnv, Player* pstPlayer,MapIndex *pstIdx);

#endif /* ZONE_MAP_H */
