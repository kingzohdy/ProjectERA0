
#ifndef ZONE_GM_H
#define ZONE_GM_H

#include "zone_svr.h"

#define UIN_FLAG_GM_ALL (UIN_FLAG_GM_LOW|UIN_FLAG_GM_HIGH|UIN_FLAG_GM_ADMIN)


int z_gm_pet_addattr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_pet_addskill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_pet_addexp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_pet_addskillpoint(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_pet_passive(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_grant_perm(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_shutdown(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_reloadres(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_addattr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_updateskill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_skillatk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_updatediathesis(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_adjustfittings(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_applystatus(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_delstatus(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_additem(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_additems(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_tmppaktime(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_paklarge(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_openstash(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_loadscript(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_debugscript(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_goto(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_near(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_pworld(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_tmpitem(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_silence(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_dur(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_addfriend(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_fini_task(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_fini_taskall(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_del_taskall(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_effect(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_pet_stat(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
void z_sort_gm_cmd(void);
int z_gm_cmd(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, const char* pszArg);
int z_gm_gem(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_pet_addpower(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_hatchpet(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_msgmiddle(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_skillcool_clean(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_clan(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_conf(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_aas_cmd(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_del_pck(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_chg_rolename(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_chg_clanname(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_sharetask(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_bulletin(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_watch(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, 
	             const char* pszArg);

int z_gm_stash(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_endride(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_enditem(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_add_ridegrid(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_mon_task_fini(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_designation(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_respact(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_del_event(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_time(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_closepworld(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_reloadall(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_dropfairy(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_addtask(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_pet_addloveval(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_settime(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_package_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_repair_all(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_revive(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_gn(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_up_all_skill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_add_pworld_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int redef_all_mon(ZONESVRENV* pstEnv);
int additem_append_check(ZONESVRENV* pstEnv, Player* pstPlayer, int iInp, int *iRand, int );
int z_gm_cont_cover_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_set_perm_limit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_block_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_player_addqiyue(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_add_pet(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_lvgift(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_onedaygift(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_addpatrol(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_upfairy(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_bus_trap(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,
                  const char *pszArg);

int z_gm_range(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_region_create_pworld(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,const char* pszArg);
int z_gm_where(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_join_queue(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_event(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_item_cool(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_secondaryskill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_career_chg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_add_collect_num(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_extend(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_getexplimit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_cleanpworldlimit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_hide_on(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_getmem(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_hide_off(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_call_role(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_ear_on(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_ear_off(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_enter_bct(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_fittings_inlay(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,const char* pszArg);

int z_gm_ride_machine(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_clean_all_item(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);

int z_gm_in_pworld(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_reset_cangbaotu(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_set_item_unvaild(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,const char* pszArg);
int z_gm_run_script(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,const char* pszArg);

int z_gm_clean_all_item(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);

int z_gm_roll(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_safelock(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_get_item_num(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_rolenum(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_spook(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_monsterai(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);

int z_gm_pkval(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_add_enemy(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_add_enemy_by_num(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_finicyc(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);

int z_gm_choose(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);

int z_gm_chgline(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_npc_script(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_settime_by_sec(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_up_contonline(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_reload_contonline(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_setconti(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_pworld_again(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_battle_queue(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_pos(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_pworld_over(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_jump(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_dropped_item_count(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
                            Player *pstPlayer, const char *pszArg);
int z_gm_armattr_reset(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_play_movie(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
                            Player *pstPlayer, const char *pszArg);
int z_gm_rekeyin(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_bub(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);

int z_gm_add_machine_energy(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_set_ride_unvaild(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,const char* pszArg);

int z_gm_accgift(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);

int z_gm_oneoffexp(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_propgift(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_setdesig(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_onlinesec(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_nativeskill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_armstarup(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_valfight(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);

int z_gm_nearmemid(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_ride(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_wear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_setpetcamp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_levelup(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_stall(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_clanenemycd(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_clancleardip(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_clanenemycdlist(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_auto_team_apply(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_auto_team_agree(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_treasurepos(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_clearnametimes(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_diffpworld(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_cdkey(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);

int z_gm_mon_event_clean(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_chgofflineexp(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_setbattleflag(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_player_get_perm(ZONESVRENV* pstEnv, Player *pstPlayer);
int z_gm_ridetmpitem(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_drop_item(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_drop_money(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_vip(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_snapshoot(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_world_ore_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_fengyin(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_player_check_vip(ZONESVRENV* pstEnv,Player *pstPlayer);

int z_gm_world_city(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_del_role(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_lucky(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_cleanbattlelimit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_delskill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_enter_clan_city(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_battle_num(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_forcemsg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_vipbarmul(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_setclanassign(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_clanboonclear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_setclanonlime(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_skill_misstime(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,
                     const char* pszArg);
int z_gm_heti(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_runeattr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_clantouch(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_enter_unreal(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_cond(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_achievementreset(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_clear_dayexp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_get_mon_num(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_get_npc_num(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_pet_add_skill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_show_sys_tip(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_goto_pvp_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_add_everyday_val(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_onedaygiftreset(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_team_follow(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_accord_item(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,
                  				 const char* pszArg);
int z_gm_aasflag(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_cleanbct(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_decbull(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_sham_dead(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_wavespeed(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_clearmachinepworldlimit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_waveset(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_cowrangelist(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_trade(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_get_strong_stat(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_strong_sale(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_strong_enter(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_strong_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_pet_genius(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_strong_add_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_strong_clear_all(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_strong_set_owner(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_dayexpwarning(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_dart_reset(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_show_fy(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_geniusclear(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_setsvrstart(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);

int z_gm_clear_goldrate(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_clear_goldbox_info(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_clear_levfeng(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_delglobalboss(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,
                   const char* pszArg);
int z_gm_boss_info(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,
                   const char* pszArg);

int z_gm_get_weather(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_set_weather(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_geniusup(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_itemoneday_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_effigy_worship_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_showequipscore(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_sparta(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_strong_open(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_strongglory(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_strongglory_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_clan_baby_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_vip_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_armattr_active(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_machine_break(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_dart_car_resume(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_arm_check(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_vip_set(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_clear_daily_flag(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_setonlinesec(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_rolesnapshot(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_fini_taskcycle(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_losttotemfull(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_store_open(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_store_close(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_store_up(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_store_info(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_store_set(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_yanghaibang_set(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_sys_mail(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_armbasereset(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_armbasereset_save(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_cleardayzhengyi(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_rongyan_clear(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);

int z_gm_cleardaylifebrand(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);

int z_gm_clanbattleaward(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_addgem(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_petaddgenius(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_clear_paoshang(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);

int z_gm_set_paoshang_yinpiao(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);

int z_gm_smscode(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_beachsec(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_add_sparta(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_clan_skill_reset_re_contri_flag_chg(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_clan_skill_reset_re_money_flag_chg(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_sparta_num(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,const char* pszArg);
int z_gm_clan_pet_jiyang(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, 
	                const char* pszArg);
int z_gm_clan_pet_jiyang_end(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, 
	                const char* pszArg);

int z_gm_dianfeng_baoming(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_dianfeng_pk_begin(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_dianfeng_clean(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_dianfeng_clean_chamption_and_effigy(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_dianfeng_clean_myself(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);

int z_gm_pet_add_impexp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_dianfeng_to_chamption(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);

int z_gm_player_match_chg_career(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_pet_skillup(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_tank_add(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_pet_learn_genius(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_package_use(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_machine_patrol(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_machine_chg_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_wedding(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_new_machine(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_set_mzt_layer(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_sj_additem(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_set_level_remain_tm(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_get_level_remain_tm(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);

int z_gm_clan_make_clean_by_id(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_clear_zj_rongyan(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,const char *pszArg);
int z_gm_span_sale(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_span_enter(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_span_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_span_tip(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_span_mail(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_boss_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,const char* pszArg);
int z_gm_clan_make_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_span_occ(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_name_addattr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_span_add_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_home_new(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_home_del(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_home_enter(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_get_role_var(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_set_role_var(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);

int z_gm_shikong_enter(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_get_shikong_fb_num(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_clear_career_alter_time(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);
int z_gm_set_home_soil_lv(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, const char* pszArg);


#endif /* ZONE_GM_H */
