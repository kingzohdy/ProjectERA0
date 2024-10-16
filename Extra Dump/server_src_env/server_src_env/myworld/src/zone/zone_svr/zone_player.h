
#ifndef ZONE_PLAYER_H
#define ZONE_PLAYER_H

#include "zone_svr.h"
#include "zone_db.h"

#define PLAYER_DELAY_LOGOUT_TIME 10

//<!--黑名玩家死亡复活点-->
#define BLACK_NAME_REVIVE_MAP 1000
#define BLACK_NAME_REVIVE_POS_X 30200
#define BLACK_NAME_REVIVE_POS_Y 18600

#define BASE_CITY_REVIVE_POS_X 26506
#define BASE_CITY_REVIVE_POS_Y 28694

#define LUCKY_LEVEL 40 // 天降洪福等级限制

#define RONGYAN_LEVEL_SUB 5

#define LEVEL_REMAIN_TM 2*60*60


#if defined(ZONE_ID_TRACKED)
Player *player_alloc_tracked(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, int iUin,
                             char *pszAccount, UinCache *pstUinCache,
                             const char *pszFile, int iLine);

#define player_alloc(app, env, uin, account, cache) \
	player_alloc_tracked(app, env, uin, account, cache, __FILE__, __LINE__)
#else
Player *player_alloc(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, int iUin,
                     char *pszAccount, UinCache *pstUinCache);
#endif

int player_open_connection(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TFRAMEHEAD *pstFrameHead);
int player_close_connection(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TFRAMEHEAD *pstFrameHead);

int player_acc_login(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int open_chg_name_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID,int iType);
int player_role_new(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_role_login(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_role_logout(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_cancel_role_logout(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_acc_logout(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_broadcast(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_heartbeat(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_role_del(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_role_selbirth(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_move(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_action(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_chat(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_opt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_revive(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_dialog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_watch(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						     TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

Player *player_get_by_uin(ZONESVRENV* pstEnv, int iUin);

int player_kickoff(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iErrno, ...);

int z_active_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iCount);

int z_touch_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv);

int player_die(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ZoneAni *pstAtker, int iNotify);

int player_adjust(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);


int player_adjust_amm(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSAMM *pstCSAmm);

int z_check_op_dist(ZONESVRENV* pstEnv, Player *pstPlayer,  int iMap, RESPOS* pstPos, int iOpDist);

int player_del_totem(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_auto_fly(ZONESVRENV* pstEnv, Player *pstPlayer, int iAutoFlyPath);

int player_auto_fly_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int z_player_addexp_test(ZONESVRENV* pstEnv,Player *pstPlayer, tdr_longlong *piExp);

int chg_name_select_res(ZONESVRENV* pstEnv, Player *pstPlayer,TORMSELECTRES *pstSelectRes,char *pstNewNane);
int chg_name_update_res(ZONESVRENV* pstEnv, Player *pstPlayer, TORMUPDATERES *pstUpdateRes, char *pszNewName);
int chg_name_check_select_res(ZONESVRENV* pstEnv, Player *pstPlayer,TORMSELECTRES *pstSelectRes, char *pszName);
int chg_clanname_check_select_res(ZONESVRENV* pstEnv, Player *pstPlayer,TORMSELECTRES *pstSelectRes, char *pszName);
int login_check_chg_rolename(ZONESVRENV* pstEnv, Player *pstPlayer);
int login_check_chg_clanname(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_chg_name(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int chg_name_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,char *pszNewName,tdr_ulonglong ullRoleID,char *pszOldName,int iFlag);
int player_chg_rolename(ZONESVRENV* pstEnv, Player *pstPlayer, CSCHGNAMECLT *pstChgNameClt);
int player_chg_clanname(ZONESVRENV* pstEnv, Player *pstPlayer, CSCHGNAMECLT *pstChgNameClt);

int exit_pworld_clean_xp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);

int player_arm_bull_rate(ZONESVRENV *pstEnv, Player *pstPlayer);

int send_to_aas(ZONESVRENV* pstEnv, int iUin, unsigned char ucType);

int player_jump(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_hp_action(ZONESVRENV* pstEnv, Player *pstPlayer, int iOldHP, int iOldMP, int iApplyID);
int chg_name_update_req(ZONESVRENV* pstEnv,tdr_ulonglong ullRoleID, int iUin, ChgRoleName *pstChg);
int chg_name_up_sht(ZONESVRENV* pstEnv,int iUin,tdr_ulonglong RoleID, char *pszNewName,char *pszOldName,int iMemID);

int player_bulletin_snapshot(ZONESVRENV* pstEnv, Player *pstPlayer);

int z_bulletin_chg_notify(ZONESVRENV* pstEnv, const BULLETINENTRY *pstChgData,
	                        int fAddOrDel);

int player_watch_role(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstTarget);

ONLINEAWARDDEF* find_online_award_def(ZONESVRENV* pstEnv, int uiIdx, uint iUnit);
int online_award(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_online_award(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						     TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_weixing(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						     TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_goto(ZONESVRENV* pstEnv, Player *pstPlayer, int iMap,int X, int Y,int iMapPos);

int z_player_move(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int z_role_svr_status(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_watch_friend(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_pk_mode_action(ZONESVRENV* pstEnv, Player *pstPlayer, int fBroadcast);

int player_pk_val_action(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_pk_val_chg(ZONESVRENV* pstEnv, Player *pstPlayer, int iVal);

int player_pk_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int chg_name_insert_nameid(ZONESVRENV* pstEnv,tdr_ulonglong ullRoleID, int iUin, const char* pstOldName);

int player_exit_fb_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID);
int player_package_info(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_revive_svr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer);

int role_online_to_mud(ZONESVRENV* pstEnv, Player *pstPlayer, int iOnlineType);

int role_gift_info(ZONESVRENV* pstEnv, Player *pstPlayer,int iLogin);

int player_gift_lv(ZONESVRENV* pstEnv, Player *pstPlayer, int iLv);
int player_gift_oneday(ZONESVRENV* pstEnv, Player *pstPlayer, int iGiftID);

int player_gift(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						     TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_updata_oneday_online_time(ZONESVRENV* pstEnv, ROLEGIFTINFO *pstRoleGiftInfo, int iLogin);

int player_revive_attr_chg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, int iHpAdd, int iMpAdd);

int player_revive_action(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer);

int player_disconnect(ZONESVRENV *pstEnv, Player *pstPlayer);

int player_action_res_ex(ZONESVRENV *pstEnv, Player *pstPlayer, CSPKG *pstPkg);

int player_collect_break(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMonCollectee);

int vim_role_login(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_item_use_info(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_mon_pkmode_make_view(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, CSMONPKMODECHG *pstMonPkModeChg);

ROLEARM *z_wear_get_secondaryskill_arm(ZONESVRENV* pstEnv, ROLEWEAR *pstRoleWear, int iSecondarySkillType);

int player_career_chg(TAPPCTX*pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer, int  CarrerChgType,int CheckUIFlag);

int player_misc_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int z_check_gm_ip(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,Player *pstPlayer, char *pszMac);

int player_gprs(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

//int player_lock_safe_info(ZONESVRENV* pstEnv, Player *pstPlayer, int iFlag);

//int player_check_safelock(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_adjust_level(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int iCheckFlag);

//int player_die_drop_item(ZONESVRENV* pstEnv, Player *pstPlayer, DIEDROP *pstDrop, int iWearRate,int iItemRate);
int player_die_drop_item(ZONESVRENV* pstEnv, Player *pstPlayer, DIEDROP *pstDrop, 
						int iWearRate,int iItemRate,Player *pstKiller, FENGYINARM *pstFengYinArm);

int z_sendpkg_playerview_filtered(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, 
	                                Player *pstPlayer, int iSelf, CSPKG *pstPkg,
	                                int iImportant);

int get_player_pkval_type(Player *pstPlayer);

int player_auto_fly_move(ZONESVRENV* pstEnv, Player *pstPlayer, RESPOS *pstPos);
int player_login_die_info(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_choose(ZONESVRENV* pstEnv, Player *pstPlayer,CSCHOOSEC* pstCsChooseC);
CONTONLINEGIFTDEF *z_find_contonlinedef(ZONESVRENV* pstEnv, int iGiftID);
int update_cont_online_time(ZONESVRENV* pstEnv, Player *pstPlayer,int iLogin);
int player_comp_contonlineinfo(ZONESVRENV* pstEnv, Player *pstPlayer);
int role_cont_online_info(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_clean_diffsameday_info(ZONESVRENV* pstEnv, Player *pstPlayer);

int chg_rolename_update_relation(ZONESVRENV* pstEnv, tdr_ulonglong ullRoleID,  char *pszNewName);

int player_movie_start_notify(ZONESVRENV* pstEnv, Player *pstPlayer, char *szFilePath);

int player_view_mon_delpress(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_boss_info_insert(Player *pstPlayer, Monster *pstMon);

int player_boss_info_del(Player *pstPlayer, Monster *pstMon);

int is_in_player_boss_info(Player *pstPlayer, Monster *pstMon);

int player_call_bub(ZONESVRENV* pstEnv, Player *pstPlayer, int iMonID, int iLifeTime);

int z_player_awardexp_change(ZONESVRENV* pstEnv, Player *pstPlayer,int iChange, int iSend);
int z_player_awardexp_calculate(ZONESVRENV* pstEnv, Player *pstPlayer);
//int player_offline_exp(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_bub_insert_enemy_mon(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMon);
int player_bub_insert_enemy_player(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstOtherPlayer);
int player_totem_info_del(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstTotem);


int is_bad_palyer(Player *pstPlayer);

int is_in_pk_time_player(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_pk_time_action(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_set_pktime(ZONESVRENV* pstEnv, Player *pstAtker, ZoneAni *pstAtkee, int fNoHurt);
int is_can_bianshen(ZONESVRENV* pstEnv, Player *pstPlayer,int iSend,int iIgnoreRide);
int player_acc_gift_req(ZONESVRENV* pstEnv, Player *pstPlayer, int iType);
int player_acc_gift_insert_req(ZONESVRENV* pstEnv, Player *pstPlayer, int iType);
//int z_player_oneoffexp_send(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_prop_gift_req(ZONESVRENV* pstEnv, Player *pstPlayer, int iID,tdr_ulonglong llCashUp);

int player_totem_update_enemy(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_motion_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int chg_role_name_notify(ZONESVRENV* pstEnv, tdr_ulonglong ullRoleID, const char* pszNew, const char* pszOld);
int chg_role_name_up_db(ZONESVRENV* pstEnv,int iUin,tdr_ulonglong ullRoleID,
							const char* pszOldName,const char* pszNewName, ACCOUNTROLENAME *pstAccRoleName);


int player_battle_hello2battle_req(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, TFRAMEHEAD *pstFrameHead,Player * pstPlayer);
int player_battle_exit_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,Player *pstPlayer);

int player_battle_exit(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,int Uin,int iOperType);

int player_battle_set_routing_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,Player *pstPlayer,int iOperType);
int player_battle_set_routing(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,SSSETBATTLEROUTINREQ *pstSetBattleRoutinReq);
int player_battle_set_routing_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,SSSETBATTLEROUTINRES *pstSetBattleRoutinRes);
int player_battle_exit_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,SSBATTLEEXITRES *pstBattleExitRes );
void player_switch_role_delay_logout( ZONESVRENV* pstEnv, Player * pstPlayer,
	int iReason );
int player_kickoff_text_notify(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, 
	Player *pstPlayer, char *pszMsg);
int player_weixing_clear(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_is_check_dyn_mask(Player *pstPlayer);
int z_heartbeat_to_region(ZONESVRENV* pstEnv, int iUin);
int player_acc_logout_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int player_drop_clan_tem(ZONESVRENV *pstEnv,Player *pstPlayer);
int player_acc_share_final(ZONESVRENV *pstEnv,Player *pstPlayer);
int player_use_offline_exp(ZONESVRENV *pstEnv,Player *pstPlayer, tdr_longlong* piExp);
int player_acc_share_init(ZONESVRENV *pstEnv,Player *pstPlayer);

int player_leave_beach(ZONESVRENV *pstEnv,Player *pstPlayer);
int player_enter_beach(ZONESVRENV *pstEnv,Player *pstPlayer,int iOldMapID);
int player_beach_tick(ZONESVRENV *pstEnv,Player *pstPlayer);
int player_beach_check_chair(ZONESVRENV *pstEnv,Player *pstPlayer,int iRange);
int player_acc_giftinfo_req_broadcast(ZONESVRENV* pstEnv);
int player_gold_raffle_num_notify(ZONESVRENV *pstEnv,Player *pstPlayer);

int player_ui(ZONESVRENV* pstEnv,Player *pstPlayer,Npc* pstNpc,int iType);
int is_bad_mon(ZONESVRENV * pstEnv, Monster * pstMon);
int get_pkval_type(int iVal);

int is_in_pk_time_mon(ZONESVRENV * pstEnv, Monster * pstMon);

int player_world_city_tick(ZONESVRENV *pstEnv,Player *pstPlayer);
int player_enter_world_city(ZONESVRENV *pstEnv,Player *pstPlayer,int iOldMap,RESPOS* pstOldPos);
int player_leave_world_city(ZONESVRENV *pstEnv,Player *pstPlayer);
int player_world_city_buff_update(ZONESVRENV *pstEnv,Player *pstPlayer);

int player_totem_chg_map(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_world_city_debuff_clear(ZONESVRENV *pstEnv,Player *pstPlayer);

int player_role_del_level(ZONESVRENV* pstEnv,tdr_ulonglong ullDelRoleID,Player *pstPlayer);
int player_is_zuolao(Player *pstPlayer);

int player_luck_tick(ZONESVRENV *pstEnv,Player *pstPlayer);
int player_broadcast_map_pos(ZONESVRENV *pstEnv,Player* pstPlayer,int iShowType);

MEDIAGIFTDEF * find_mediagiftdef(ZONESVRENV* pstEnv, int iMediaGiftID);
int player_silence_check(ZONESVRENV* pstEnv, Player *pstPlayer);
int z_check_chat(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, char* pszChat, char cType);

LTRGIFTDEF* find_ltrgiftdef(ZONESVRENV* pstEnv, int iLtrID);
//媒体礼包使用检查
int player_mediagift_use_check(ZONESVRENV * pstEnv,Player * pstPlayer,int iMediaGiftID,int selectOneItemIndex,ROLEITEM *pstRoleItem);
int player_mediagift_use(ZONESVRENV * pstEnv,Player * pstPlayer,int iMediaGiftID,int selectOneItemIndex, ROLEITEM *pstRoleItem, char * szItemName);

LTRGIFTDEF * find_ltrgiftdef_bytimes(ZONESVRENV * pstEnv,Player * pstPlayer,int iLtrGiftID,int iItemID);
//媒体次数礼包使用检查
int player_ltrgift_use_check(ZONESVRENV * pstEnv,Player * pstPlayer,int iLtrGiftID,ROLEITEM *pstRoleItem);
int player_ltrgift_use(ZONESVRENV * pstEnv,Player * pstPlayer,int iLtrGiftID,ROLEITEM *pstRoleItem,char * szItemName);

int player_daily_goal_info(ZONESVRENV * pstEnv,Player * pstPlayer);
int player_dyn_active_helper_info(ZONESVRENV * pstEnv,Player * pstPlayer,int iID,int iStatus);
int all_player_dny_active_helper_info(ZONESVRENV * pstEnv,int iID,int iStatus);



ROLEARM * player_get_wear_rolearm_by_grididx(Player * pstPlayer,int gridIdx);
extern int g_iVimRecoverInterval;

int atker_set_pktime(ZONESVRENV* pstEnv, ZoneAni *pstAtker, ZoneAni *pstAtkee, int fNoHurt);
int player_is_magic_career(int iCareer);

int auth_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int z_ss_get_account_res(ZONESVRENV * pstEnv,
	                            const SSGETACCOUNTRES *pstGetAccountRes);

int z_player_achievement_online(ZONESVRENV* pstEnv, Player* pstPlayer);
int z_player_achievement_done(ZONESVRENV* pstEnv,Player* pstPlayer,int iID);

int achievement_addattr(ZONESVRENV *pstEnv, Player *pstPlayer);
int z_player_achievement_cond(ZONESVRENV* pstEnv, Player* pstPlayer,ACHIEVEMENTCOND* pstCond);

int player_mon_pk_mode_action(ZONESVRENV* pstEnv,Monster *pstMon);
int z_ss_req_get_account(TAPPCTX * pstAppCtx, ZONESVRENV* pstEnv,
                              SelectInfo * pstExtraInfo);

int player_yesterday_info(ZONESVRENV* pstEnv, Player *pstPlayer);
int all_player_yesterday_info(ZONESVRENV * pstEnv);

int player_get_vig(ZONESVRENV * pstEnv, Monster *pstDieMon, Player*pstKiller);
int clean_oneday_gift_info(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_login_up_vig(ZONESVRENV * pstEnv,  Player*pstPlayer);
int player_full_vig(ZONESVRENV * pstEnv,  Player*pstPlayer);
int mon_weixing_tick(ZONESVRENV* pstEnv, Monster *pstMon);
int mon_weixing_clear(ZONESVRENV* pstEnv, Monster *pstMon);
int player_action_count_down(ZONESVRENV * pstEnv,  Player*pstPlayer,int iSec,char* pszTitle);
int z_send_player_viewinfo(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int player_action_view_flag_chg(ZONESVRENV * pstEnv,  Player*pstPlayer);
int z_ss_fill_id_card_req(ZONESVRENV* pstEnv,Player* pstPlayer,
                              CSAASIDINFO * pstInfo);
int z_ss_fill_id_card_res(ZONESVRENV * pstEnv,
	                            const SSFILLIDCARDRES *pstFillIdCardRes);

int player_goto_ignore_refer_line(ZONESVRENV* pstEnv, Player *pstPlayer, int iMap,int X, int Y,int iMapPos);
int player_use_trans_item_map_check(ZONESVRENV * pstEnv,  Player*pstPlayer,int iTip);

int player_chat_to_mud(ZONESVRENV* pstEnv, Player *pstPlayer, CSCHATREQ *pstChat, const char* pstRoleName);
int player_is_dartcar_limit(Player*pstPlayer);
int player_pkval_chg_outpress(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_action_clan_machine_pworld(ZONESVRENV * pstEnv,  Player*pstPlayer,ZONEPWORLDINST *pstPworldInst);
int player_action_cow_final_range(ZONESVRENV * pstEnv,  Player*pstPlayer,int iOpType);
int player_action_clan_donate_log(ZONESVRENV * pstEnv,  Player*pstPlayer,CLANDONATELOGENTRY *pstLog);
int player_contribute_dec(ZONESVRENV* pstEnv, Player *pstPlayer,
                      unsigned int nValue,int iTest);
int player_action_clan_skill_info(ZONESVRENV * pstEnv,  Player* pstPlayer,CLANSKILLINFO* pstInfo);
int player_action_player_clan_skill_info(ZONESVRENV * pstEnv,  Player*pstPlayer,PLAYERCLANSKILLINFO* pstInfo);

int role_login_pkval_check(ZONESVRENV* pstEnv, Player *pstPlayer);
int lucky_open_ui_action(ZONESVRENV* pstEnv, int iType);
int player_login_lucky_ui(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_uin_flag_chg(ZONESVRENV* pstEnv, Player *pstPlayer,int iUinFlag,int iType);

int player_delay_kickoff(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, char *pszMsg);

int player_beach_info_action(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_beach_info_update(ZONESVRENV* pstEnv, Player *pstPlayer);

//获得当前可达到的最高等级
int get_curr_max_level(ZONESVRENV* pstEnv);
int player_die_up_daoba(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstKiller);
int player_make_daoba_view(ZONESVRENV* pstEnv,CSPLAYER *pstCSPlayer , Player *pstVPlayer);
int player_daoba_sec(ZONESVRENV* pstEnv ,Player *pstPlayer);
int player_daoba_chg(ZONESVRENV* pstEnv, Player *pstPlayer, int iSelf);

int player_action_clan_war_award_info(ZONESVRENV * pstEnv,  Player*pstPlayer);
int player_clan_war_award_mail(ZONESVRENV * pstEnv,  Player*pstPlayer);
int player_beach_buff_action(ZONESVRENV* pstEnv, Player *pstPlayer, int fIn);
int is_mine_map(ZONESVRENV* pstEnv, int iMapID);
int player_charm_chg(ZONESVRENV* pstEnv, Player* pstPlayer, int iFlag, int iNum);
int player_week_charm_notify(ZONESVRENV* pstEnv, Player *pstPlayer, int iFlag, int iNum);
int player_reg_delete(ZONESVRENV* pstEnv, Player* pstPlayer);
int player_reg_delete_by_time(ZONESVRENV* pstEnv);
int player_reg_info_notify(ZONESVRENV* pstEnv, Player* pstPlayer, int iNpcID);
int player_die_tip_by_saprte_range(ZONESVRENV* pstEnv ,Player *pstPlayer, Player *pstKiller);
int player_new_role_gift_init(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_logout_new_gift(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_cont_cover_gift_notify(ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID, int iNum);
int player_cont_cover_up(ZONESVRENV* pstEnv, Player *pstPlayer, int iNotify);
int player_login_verdict_huiliu(ZONESVRENV* pstEnv ,Player *pstPlayer);
int player_is_huiliu(Player *pstPlayer);
BUCHANGDEF *buchang_def_find(ZONESVRENV* pstEnv, int iDay);
ARMBUCHANGDEF *arm_buchang_def_find(ZONESVRENV* pstEnv, int iLvl, int iCareer);
int player_action_hang_hai_range(ZONESVRENV * pstEnv,  Player*pstPlayer,int iOpType);
int player_gold_rate_notify(ZONESVRENV* pstEnv, Player* pstPlayer, int iLogin);

//lzk  融焰巨兽相关
int player_rongyan_boss(ZONESVRENV* pstEnv,Player* pstPlayer,CSRONGYANBOSSSELECT *pReq);
int player_rongyan_begin(ZONESVRENV* pstEnv,Player* pstPlayer,CSRONGYANBEGINSELECT *pReq);
RONGYANJUSHOUDEF *z_find_rongyanjushou_def(ZONESVRENV* pstEnv, int iLevel);
int player_rongyan_level(ZONESVRENV * pstEnv,Player *pstPlayer,ZoneTeam *pstTeam);
RONGYANMONDYNATTRDEF *z_find_rongyanmondynattr_def(ZONESVRENV* pstEnv, int iLevel);
int player_rongyan_boss_ui(ZONESVRENV* pstEnv, Player *pstPlayer,CSRONGYANBOSSINFO *pstRongYanBossInfo);
int player_rongyan_move_ui(ZONESVRENV* pstEnv, Player *pstPlayer,CSRONGYANUIINFO *pstRongYanUiInfo);
int player_rongyan_new_mon(ZONESVRENV* pstEnv, Player *pstPlayer,int tLifeTime,int iMonID,RESPOS *pstPos,int iTeamCapId,int *MemID,int iPinJunLevel,ZONEIDX *pstIdx);
int player_rongyanmon_dynattr(ZONESVRENV * pstEnv, Monster* pstMon,int iPinJunLevel);
int player_action_clan_battle_apply_info(ZONESVRENV * pstEnv,  Player*pstPlayer,int iOpType);

//跑商相关
int player_open_paoshang_ui(ZONESVRENV* pstEnv, Player *pstPlayer,PAOSHANGGLOBALDATA *pstPaoShangGlo,PAOSHANGPLAYER *pstPaoShangPl);
PAOSHANGDEF *z_find_paoshang_def(ZONESVRENV* pstEnv, int ID);
PAOSHANGITEMDEF *z_find_paoshangitem_def(ZONESVRENV* pstEnv, int ItemID);
int player_paoshang_clt_select(ZONESVRENV *pstEnv,Player* pstPlayer,CSPAOSHANGUISELECT *pReq);
int player_paoshang_price_if_change(ZONESVRENV *pstEnv,Player* pstPlayer,CSPAOSHANGUISELECT *pReq,PAOSHANGGLOBALDATA *pstPaoShangGlo,PAOSHANGPLAYER *pstPaoShangPl);
int player_paoshang_user_all_sell(ZONESVRENV *pstEnv,Player* pstPlayer,CSPAOSHANGUISELECT *pReq,PAOSHANGGLOBALDATA *pstPaoShangGlo,PAOSHANGPLAYER *pstPaoShangPl);
int player_paoshang_user_sell(ZONESVRENV *pstEnv,Player* pstPlayer,CSPAOSHANGUISELECT *pReq,PAOSHANGGLOBALDATA *pstPaoShangGlo,PAOSHANGPLAYER *pstPaoShangPl);
int player_paoshang_user_buy(ZONESVRENV *pstEnv,Player* pstPlayer,CSPAOSHANGUISELECT *pReq,PAOSHANGGLOBALDATA *pstPaoShangGlo,PAOSHANGPLAYER *pstPaoShangPl);
int player_paoshang_login_check(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_take_clan_war_award_in(ZONESVRENV *pstEnv,Player *pstPlayer);

int player_paoshang_init(ZONESVRENV* pstEnv);
int player_paoshang_clean(ZONESVRENV* pstEnv, Player *pstPlayer);
int z_ss_sms_res(ZONESVRENV * pstEnv,
	                            const SSSMSRES *pstSmsRes);
int player_action_precreate_gift_verify(ZONESVRENV * pstEnv,  Player* pstPlayer,int iOpType,int iRet);


int player_show_face(ZONESVRENV* pstEnv, Player *pstPlayer, int iShowFlag);
int player_beach_low_exp_ui_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_match_chg_career(TAPPCTX*pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer, MATCHCHGCAREER *pstChg);
int z_player_level_remain_tm_update(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_boss_baoxiang_if_touch(ZONESVRENV* pstEnv,Player *pstPlayer,tdr_ulonglong WID);
int player_boss_baoxiang_touch_set(ZONESVRENV* pstEnv,Player *pstPlayer,tdr_ulonglong WID);

//终极熔岩巨兽相关
int zj_rongyan_update_call_name(ZONESVRENV* pstEnv, const char* pszOldName, const char* pszNewName);
int player_if_call_zj_rongyan(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_login_zj_rongyan_check(ZONESVRENV* pstEnv, Player *pstPlayer);
tdr_longlong player_get_exp_by_zj_rongyan(ZONESVRENV* pstEnv,int iPlayerLevel,int iZJRongYanLevel);
int mon_zj_rongyan_die_player_add_exp(ZONESVRENV* pstEnv,Monster* pstMon);
int mon_zj_rongyan_die(ZONESVRENV* pstEnv,Monster* pstMon);
int zj_rongyan_mon_if_have_by_wid(ZONESVRENV* pstEnv,tdr_ulonglong WID);
int player_zj_rongyan_new_mon(ZONESVRENV* pstEnv,int tLifeTime,int iMonID,RESPOS *pstPos,tdr_ulonglong *WID,ZONEIDX *pstIdx);
int zj_rongyan_tick(ZONESVRENV* pstEnv);
int player_call_zj_rongyan(ZONESVRENV* pstEnv,Player *pstPlayer, ZJRONGYANCALL *pstZJRongYanCall);
ZJRONGYANJUSHOUDEF *z_find_zjrongyanjushou_def(ZONESVRENV* pstEnv, int iLevel);
int player_zj_rongyan_ui(ZONESVRENV* pstEnv, Player *pstPlayer,ZJRONGYANINFO *pstZJRongYanInfo);
int zj_rongyan_close_dlg(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_show_wedding_dsn(ZONESVRENV* pstEnv, Player *pstPlayer, int iShowFlag);
int player_zj_rongyan_boss_select_ui(ZONESVRENV* pstEnv, Player *pstPlayer,CSZJRONGYANBOSSINFO *pstZJRongYanBossInfo);
int player_zj_rongyan_boss(ZONESVRENV* pstEnv,Player* pstPlayer,CSZJRONGYANBOSSSELECT *pReq);
int span_map_broadcast(ZONESVRENV *pstEnv, Player *pstPlayer, CSPKG *pstPkg,int iType);
int player_sculpt_chg_clt(ZONESVRENV* pstEnv,Player* pstPlayer,CSSCULPTCHGCLT *pReq);
int player_sculpt_chg_svr(ZONESVRENV* pstEnv, Player *pstPlayer,CSSCULPTCHGSVR *pstSculptChgSvr);
int sculpt_chg_action(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_word_pwd_ui(ZONESVRENV* pstEnv, Player *pstPlayer,CSWORDPWDUI *pstWordPwdUI);
WORDPWDDEF *z_find_word_pwd_def(ZONESVRENV* pstEnv, int iID);
int player_if_in_span_map(ZONESVRENV *pstEnv,Player *pstElement);
int player_enter_span_map(ZONESVRENV *pstEnv,Player *pstPlayer,int iOldMap,RESPOS* pstOldPos);
int player_pk_mode_chg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iMode);
int player_enter_span_map_notify(ZONESVRENV* pstEnv, Player *pstPlayer,CSENTERSPANMAPNOTIFY *pstSpanMapNotify);
int player_enter_span_map_notify_msg(ZONESVRENV *pstEnv,Player *pstPlayer);
int player_baoxiang_if_have_limit_touch(ZONESVRENV* pstEnv,Player *pstPlayer,tdr_ulonglong WID,int iLevel);
int player_baoxiang_touch_limit_set(ZONESVRENV* pstEnv,Player *pstPlayer,tdr_ulonglong WID);
int z_add_tmpenemy(ZONESVRENV* pstEnv, Player *pstAtkee, ZoneAni *pstAtker);

int chg_clan_name(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan, char *pszNewName);
int span_map_revive_pos(int iCamp,RESPOS *pstPos);



#endif /* ZONE_PLAYER_H */

