
#ifndef ZONE_SCRIPT_H
#define ZONE_SCRIPT_H

#include "zone_svr.h"
#include "zone_map.h"

#define ITEM_BAT_NUMMAX 8

#define SCRIPT_PROC_INIT	0
#define SCRIPT_PROC_DIALOG	1

#define NPC_SCRIPT_PROC_ACCEPT_TASK	2
#define NPC_SCRIPT_PROC_AWARD_TASK	3
#define NPC_SCRIPT_PROC_ACCEPT_TASK_MAP 0x01
#define NPC_SCRIPT_PROC_AWARD_TASK_MAP 0x02

#define MON_SCRIPT_PROC_NEW 1
#define MON_SCRIPT_PROC_DIE 2
#define MON_SCRIPT_PROC_PATROL 3
#define MON_SCRIPT_PROC_TIMER 4
#define MON_SCRIPT_PROC_PATROL_MIDDLE 5
#define MON_SCRIPT_PROC_COLLECT_BEGIN 6
#define MON_SCRIPT_PROC_COLLECT_END 7
#define MON_SCRIPT_PROC_FORCE_MOVE_END 8
#define MON_SCRIPT_PROC_AI_RESTORE_HP 9
#define MON_SCRIPT_PROC_ATK_BY_PLAYER 10
#define MON_SCRIPT_PROC_TIMER2 11
#define MON_SCRIPT_PROC_PRE_DIE 12

#define MON_SCRIPT_PROC_NEW_MAP 0x01
#define MON_SCRIPT_PROC_DIE_MAP 0x02
#define MON_SCRIPT_PROC_PATROL_MAP 0x04
#define MON_SCRIPT_PROC_TIMER_MAP 0x08
#define MON_SCRIPT_PROC_COLLECT_BEGIN_MAP 0x10
#define MON_SCRIPT_PROC_COLLECT_END_MAP 0x20
#define MON_SCRIPT_PROC_FORCE_MOVE_END_MAP 0x40
#define MON_SCRIPT_PROC_AI_RESTORE_HP_MAP 0x80
#define MON_SCRIPT_PROC_ATK_BY_PLAYER_MAP 0x100
#define MON_SCRIPT_PROC_TIMER2_MAP 0x200
#define MON_SCRIPT_PROC_PRE_DIE_MAP 0x400

#define PWORLD_SCRIPT_PROC_START 1
#define PWORLD_SCRIPT_PROC_END 2
#define PWORLD_SCRIPT_PROC_TIMER 3
#define PWORLD_SCRIPT_PROC_MON_DIE 4
#define PWORLD_SCRIPT_PROC_ENTER 5
#define PWORLD_SCRIPT_PROC_ROLE_DIE 6
#define PWORLD_SCRIPT_PROC_DOWN_TIME 7	// 倒计时结束
#define PWORLD_SCRIPT_PROC_REAL_START 8

#define PWORLD_SCRIPT_PROC_START_MAP 0x01
#define PWORLD_SCRIPT_PROC_END_MAP  0x02
#define PWORLD_SCRIPT_PROC_TIMER_MAP 0x04
#define PWORLD_SCRIPT_PROC_MON_DIE_MAP 0x08
#define PWORLD_SCRIPT_PROC_ENTER_MAP 0x10
#define PWORLD_SCRIPT_PROC_ROLE_DIE_EVENT 0x20
#define PWORLD_SCRIPT_PROC_DOWN_TIME_EVENT 0x40   // 倒计时结束事件
#define PWORLD_SCRIPT_PROC_REAL_START_MAP 0x80

#define GAME_EVENT_SCRIPT_PROC_START 1
#define GAME_EVENT_SCRIPT_PROC_END 2
#define GAME_EVENT_SCRIPT_PROC_TIMER 3
#define GAME_EVENT_SCRIPT_PROC_ROLE_LOGIN 4
#define GAME_EVENT_SCRIPT_PROC_MOTION 5
#define GAME_EVENT_SCRIPT_PROC_ENTER_MAP 6	//玩家进入地图事件
#define GAME_EVENT_SCRIPT_PROC_TIMER2 7

#define GAME_EVENT_SCRIPT_PROC_START_MAP 0x01
#define GAME_EVENT_SCRIPT_PROC_END_MAP  0x02
#define GAME_EVENT_SCRIPT_PROC_TIMER_MAP 0x04
#define GAME_EVENT_SCRIPT_PROC_ROLE_LOGIN_MAP 0x08
#define GAME_EVENT_SCRIPT_PROC_MOTION_MAP 0x10
#define GAME_EVENT_SCRIPT_PROC_ENTER_MAP_MAP 0x20
#define GAME_EVENT_SCRIPT_PROC_TIMER2_MAP 0x40

#define PROCEVT_SCRIPT_PROC_START 1
#define PROCEVT_SCRIPT_PROC_TIMER 2
#define PROCEVT_SCRIPT_PROC_LEAVE 3

#define MAP_INST_SCRIPT_PROC_TIME 3			//mapinst tick  proc 3
#define MAP_INST_SCRIPT_PROC_MON_DIE 4	

#define MAP_INST_SCRIPT_PROC_TIME_FLAG 0x01	// mapinst tick flag
#define MAP_INST_SCRIPT_PROC_MON_DIE_MAP 0x02

#define ITEM_SCRIPT_USE 1
#define ITEM_SCRIPT_USE_TEST 2


#define ITEM_SCRIPT_USE_MAP 0x01
#define ITEM_SCRIPT_USE_TEST_MAP 0x02


#define TASK_SCRIPT_PROC_ACCEPT	1
#define TASK_SCRIPT_PROC_FINI	2
#define TASK_SCRIPT_PROC_AWARD	3
#define TASK_SCRIPT_PROC_FAIL	4
#define TASK_SCRIPT_PROC_DROP	5
#define TASK_SCRIPT_PROC_TIMER	6
#define TASK_SCRIPT_PROC_TIMER2	7
#define TASK_SCRIPT_PROC_MOTION	8

#define TASK_SCRIPT_PROC_ACCEPT_MAP 0x01
#define TASK_SCRIPT_PROC_FINI_MAP 0x02
#define TASK_SCRIPT_PROC_AWARD_MAP 0x04
#define TASK_SCRIPT_PROC_FAIL_MAP 0x08
#define TASK_SCRIPT_PROC_DROP_MAP 0x10
#define TASK_SCRIPT_PROC_TIMER_MAP 0x20
#define TASK_SCRIPT_PROC_TIMER2_MAP 0x40
#define TASK_SCRIPT_PROC_MOTION_MAP 0x80

#define PLAYER_EVENT_SCRIPT_NAME "player.mac"	//玩家事件脚本名
#define PLAYER_EVENT_SCRIPT_PROC_LEVEL	1			//玩家升级事件

#define PLAYER_EVENT_SCRIPT_PROC_LEVEL_MAP	0x01	//玩家升级事件
#define SCRIPT_ADD_ATTR_NUM	4	//脚本可以添加的属性总数


#define WORLD_MON_EVENT_SCRIPT_PROC_DIE	1


typedef struct {
	Player *pstPlayer;
	Monster *pstMonster ;
	Npc *pstNpc;
	ZONEPWORLDINST *pstPworldInst;
	GameEventInst *pstEvent;
	ZONEMAPINST* pstMapInst;
	int iMapInstFist; //环境变量中的pstMapInst优先取
	char szScript[RES_NAME_LEN];
}ZoneScriptCallStack;

#define RETURN_FLAG_INT 1	//脚本返回int

enum EX_PARA_TYPE
{
	EX_PARA_TYPE_NONE = 0,//非扩展参数
	EX_PARA_TYPE_ADD_STATUS_DYN,//带动态参数的加状态指令
};

typedef struct {
	ZONESVRENV* pstEnv;
	Player *pstPlayer;
	Monster *pstMonster ;
	Npc *pstNpc;
	ZONEMAPINST *pstMapinst;
	GameEventInst *pstEvent;
	ZONEPWORLDINST *pstPworldInst;
	int iLen;
	char szScript[RES_NAME_LEN];
	CSPKG* pstSPkg;
	CSPKG stDlgPkg;
	CSDLGC *pstDlgC;
	int iStackTop;
	ZoneScriptCallStack astCallStack[SCRIPT_CALLSTACK_SIZE];
	int iTaskCurr;
	int iMemID;			// 使用道具时,被动方MemID. 主动方是 pstPlayer.
	ROLEITEM *pstRoleItem; //记录下要使用物品的句柄
	CSPACKAGEUSE stCsPackageUse; //把用户请求记录下来
	int iItemDefID;//使用物品时物品的defid
	Player *pstKiller;		//1击杀者
	
	int iMotionID;		//2动作表情
	int iReturnFlag;	//脚本返回标记,目前支持int
	int iReturnValue;	//脚本返回值
	int iExParaType;	//扩展参数类型对应EX_PARA_TYPE
	int iExParaData;	//扩展参数值
	int iMoneyType;		//货币类型
	int iMapInstFist; //环境变量中的pstMapInst优先取
} ScriptEnv;

void *get_script_location(char *sName);
int init_script_sys(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv);
int z_exec_script(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, char *pszScript, ScriptIndex *pstIndex, int iProc);
int get_script_input(char *sInput, char *sName, char *sVal, int iSize);
int load_one_script(ZONESVRENV* pstEnv, char *pszScriptName);
void *get_script_location_byind(ScriptIndex *pstIndex, char *pszScriptName);
ScriptIndex *get_script_ind(char *sName);
int load_script_dir(ZONESVRENV* pstEnv);
int play_effect(ZONESVRENV* pstEnv, Player *pstPlayer, int iEffectType, int iMemID, char* p, ZONEMAPINST *pstMapInst);


int   npc_dialog();
int   npc_talk();
int   npc_addtalk();
int   npc_option();
int   npc_next();
int   npc_stash();	
int   s_get_player_base_att();
int   s_enter_pworld();
int s_get_pworld_info();
int s_get_item_num_inpak();
int s_dec_item_num_inpak();
int s_add_exp();
int s_new_mon();
int s_get_mon_num();
int s_transmit();
int s_msg_info();
int s_map_msg_info();
int s_svr_msg_info();
int s_get_pworld_var();
int s_set_pworld_var();
int s_del_mon();
int s_mon_skill();
int s_add_money();
int s_is_captain();
int s_team_trans();
int s_get_ran_pos();
int s_new_tmp_item();
int s_get_player_id();
int s_get_team_player_id();
int s_add_sys_msg_start();
int s_add_sys_msg();
int s_add_sys_msg_pos();
int s_set_sys_msg_mode();
int s_open_npc_shop();
int s_reset_pworld();
int s_new_npc();
int s_del_npc();
int s_get_npc_num();
int s_pworld_over();
int s_get_task_var();
int s_set_task_var();
int s_task_add_option();
int s_task_do_option();
int s_taskcycle_do_option();
int s_task_fini();
int s_task_stat();
int s_effect();
int s_autofly();
int s_get_trig_task();
int s_add_item_num_inpak();
int s_add_item_bat_inpak();
int s_get_point_distance();
int s_get_player_mapinfo();
int s_get_map_randpoint();
int s_get_now_time();
int s_out_pworld_by_single();
int s_out_pworld_by_team();

int npc_ui();
int s_get_player_diathesisinfo();
int s_get_player_skillinfo();

int s_get_time_sec();

int s_get_tmp_num();

int s_get_role_var();
int s_set_role_var();

int s_set_mon_var();
int s_get_mon_var();

int z_check_mon_script(ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef);
int z_check_pworld_script(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, ZONEPWORLD *pstZonePworld);
int z_check_item_script(ZONESVRENV* pstEnv, ITEMDEF  *pstItemDef);

int npc_pworld_ui();

int s_get_mon_info();
int s_set_mon_buff();

int s_get_item_obj_info();
int s_get_pet_info();

int s_transmit_camera();
int s_player_transmit_camera();

int s_compel_close_pworld(); // 强制关闭副本 
int s_call_mon();

int s_add_pet_fight();
int s_is_in_pworld();
int s_new_mon_by_dir();

int s_add_xp();
int s_get_map_npc_id();
int s_effect_id();

int s_enter_clan_city();
int npc_dynamic_option_list(); // 生成动态选项
int npc_dynamic_do_task_option(); // 完成动态选项的任务动作

int s_clan_pay();

int s_get_target_info();
//int s_target_add_status();
int s_player_has_status();
int s_target_clean_by_memid();
int s_target_relation_by_memid();
int s_target_add_status_by_memid();

int s_taskcycle_add_option();

int s_set_svr_status();
int s_set_event_var();
int s_get_event_var();
int s_set_svr_bulletin();
int s_set_event_pworld();
int s_range_add_exp();
int s_range_add_status();
int s_get_event_stat();

int s_new_life_mon();
int get_pworld_role_num();
int s_add_tmp_item();
int s_add_fairy_fight();

int s_get_autofly_info();

int s_get_item_oneday_info();
int s_item_add_oneday();

int s_get_map_mon_id();
//老的藏宝图脚本
int s_use_treasure_pic();
//新的藏宝图脚本
int s_use_cangbaotu_pic();

int s_task_share_test();

int s_team_task_share();

int s_mon_task_area_check();

int s_mon_chg_map();

int s_mon_call_enemy_mon();

int s_set_mon_task_stat();

int s_team_msg_info();

int  s_id_player_base_att();

int s_new_convoy_mon();

int s_get_group_oneday_info();

int s_group_add_oneday();

int s_new_map_mon();

int s_enter_pworld_num_test();

int s_team_pworld_info();

int s_get_mon_patrol_id();

int s_get_map_mon_num();

int s_get_item_use_dur();

int s_add_item_on_map();

int s_is_my_convoy_mon();

int s_get_id_player_status_info();

int s_get_id_mon_status_info();

int s_reset_all_genius();

int s_get_mon_fight_time_count();

int s_modify_mon_press();

int s_mon_atk_specific_target();

int s_mon_speech();

int s_new_mon_and_forced_move();

int s_get_mon_current_patrol_pos();

int s_set_mon_current_patrol_time();

int s_add_pworld_resource();

int s_id_player_group_oneday_info();

int s_id_player_group_add_oneday();

int s_set_event_pworld_queue();

int s_get_distance_between_2mon();

int s_get_mon_num_by_defid();

int s_get_player_num_in_mon_range_by_career();

int s_get_friend_num_in_mon_range();

int s_get_mon_hp_percent();

int s_get_friend_player_num_in_mon_press_order_player();

int s_delete_mon_by_defid();

int s_apply_pworld_queue();

int s_destroy_mon();

int s_mon_owner_msg_info();

int s_get_id_player_info();

int s_add_fini_task();

int s_del_id_mon();

int s_get_pack_space();

int s_new_life_npc();

int s_is_same_week();

int s_new_owner_tmp_item();

int s_add_player_hp();

int s_add_player_mp();

int s_add_fight_pet_hp();

int s_call_mon_by_patrol();

int s_pworld_revive_player();

int s_get_role_tmp_item();

int s_del_role_tmp_item();

int s_del_role_unused_tmp_item();

int s_task_run();

int s_dec_money();

int s_npc_die();

int s_has_id_player_designation();

int s_task_fail();

int s_player_ride_on();

int s_del_player_ride();

int s_range_set();

int s_run_task_progress();

int s_bct_call_mon();
int s_bct_pass();
int s_bct_get_curr_layer();
int s_bct_shest_award();
int s_bct_ui();

int s_player_collect_break();

int s_set_pworld_map_curr();

int s_career_level_up();
int  s_get_player_explimit();

int s_set_mon_curr();
int s_get_mon_patrolIdx();
int s_mon_set_hp();

int s_set_player_curr();
int s_exit_clan();

int s_commit_task();

int s_dec_money_test();
int s_get_id_mon_info();

int s_new_linked_mon_slot();
int s_link_mon();

int s_get_player_secondary_skill();
int s_enter_logic_area();
int s_leave_logic_area();

int s_set_mon_group();
int s_pworld_linked_mon_info();
int s_player_new_linked_mon();
int s_role_item_num();
int s_has_fairy_by_id();

int s_new_npc_private();
int s_pworld_again();
int s_procevt_player_num();
int s_call_mon_team_all();

int npc_commit_opt();
int s_get_commit_item_info();
int s_dec_item_num_byldx();
int s_get_role_tmp_machine();
int   set_dialog_style();

int s_circle_new_mon();
int s_apply_tmp_item();

int s_giveback_clan_machine();
int s_laydown_clan_ore();

int s_get_player_pkval_type();
int s_set_mapinst_var();
int s_get_mapinst_var();

int s_player_close_dialog();
int s_player_close_dialog1();
int s_sys_mail();//脚本发系统邮件
int s_mail_text();//邮件内容
int s_add_designation();
int s_choose();
int s_add_online_attr();
int s_learn_sec_skill();
int s_is_mon_fighting();
int s_set_battle_win();
int s_set_mon_camp_idx();
int s_set_revive_pos_camp();
int s_new_dyn_mon();
int s_get_attacker_camp();
int s_player_jump();
int s_new_camp_npc();
int s_mon_get_player_num_in_rect();
int s_player_movie_start();

int s_get_clan_building_lv();
int s_get_player_task_sprite_flag();
int s_set_player_task_sprite_flag();
int s_get_battle_level_limit();
int s_goto_clan_city();
int s_get_mon_group();
int s_get_rand_pworld_player_id();
int s_new_mon_machine();
int s_mon_forced_move();
int s_player_set_hide_task();
int s_mon_forced_move_pos();
int s_get_radian();
int s_get_angle_pos();
int s_is_player_owner_mon();
int s_player_call_bub();
int s_get_player_map_line();
int s_set_player_map_line();
int s_mon_push_press();
int s_mon_owner_dis();
int s_commit_clan_item();
int s_get_clan_used_tech_point();
int s_player_trigger_mon_press();
int s_destroy_mon_by_defid();
int s_is_can_bianshen();
int s_mon_range_add_player_hp();
int s_player_set_desig();
int s_mon_get_killer_id();
int s_get_world_first_start_time();

int s_add_bind_item_bat_inpak();
int s_add_bind_item_num_inpak();

int s_battle_min_camp_player_num();

int s_get_cmp_fairyid();
int s_new_mon_machine_by_dir();

int  s_id_player_motion();

int s_set_event_battle();
int s_player_need_new_name();
int s_clan_need_new_name();

int s_new_map_npc();

int s_get_pworld_first_player_id();
int s_get_map_inst();

int s_is_one_mon_fighting_by_defid();

int s_set_script_int_ret();
int s_get_player_num_range();

int s_pworld_open_award_ui();
int s_get_player_safelocak_info();
int s_get_player_grab_info();
int s_get_mul_commit_item_info();
int s_set_dlg_commit_num();

int s_world_ore_new_check();

int s_commit_mul_clan_item();

int s_mon_owner_task_check();
int s_auto_move();
int s_player_clan_pos_info();

int s_get_role_ride_on_machine();

int s_map_trans();

int s_enter_world_city_check();
int s_is_world_city_owner();

int s_set_world_city_open_time();
int s_get_world_city_enable_time();
int s_set_world_city_enable_time();
int s_get_world_city_open_time();
int s_set_world_boss_first_kill_time();
int s_get_world_boss_first_kill_time();
int s_world_city_area();
int s_add_world_city_build_serial();

int s_get_crayfish_num_oneday();
int s_commit_crayfish_num_once();

int s_get_world_city_keep_num();
int s_get_player_join_clan_time();
int s_set_world_city_effigy();
int s_use_item_show_map();

int s_has_master_world_city();

int s_choose_with_timer();
int s_get_map_rand_mon_pos();
int s_target_add_status_dyn();
int s_set_lucky_open();
int s_is_player_lucky_attendee();
int s_is_lucky_closed();
int s_clan_everyday_pworld_stat_get();
int s_clan_everyday_pworld_create();
int s_clan_everyday_pworld_enter();
int s_clan_everyday_pworld_val_chg();
int s_for_each_map_obj();
int s_is_in_my_clan_city();
int s_new_clan_mon();
int s_get_clan_tech_level();
int s_is_in_siege_city();
int s_get_clan_stable();
int s_add_clan_stable();
int s_player_add_status_by_clanmon();
int s_get_player_daily_flag();
int s_set_player_daily_flag();
int s_clan_msg_info();
int s_dyn_active_status_set();
int s_clan_touch_add();
int s_dyn_active_status_get();
int s_unreal_watch_boss_info();
int s_unreal_team_info();
int s_goto_unreal_captain_near();
int s_get_unreal_layer();
int s_save_unreal_layer();
int s_unreal_enter();
int s_is_last_unreal_boss_map();
int s_unreal_enter_boss_check();
int s_darkmatter_dropmoney();
int s_darkmatter_flee_dropmoney();
int s_get_mon_mapdefid();
int s_team_trans_insamemapinst();
int s_add_player_hp2();
int s_mon_drop_bootygroup();
int s_get_mon_level();
int s_map_del_tmp_item();
int s_new_life_dyn_mon();
int s_count_down();
int s_team_follow_cancel();
int s_mon_die_team_add_buff();
//int s_get_unreal_save();
int s_is_clan_can_spy();
int s_is_clan_anti_spy_task_open();
int s_clan_spy_close();
int s_clan_anti_spy_task_open();
int s_clan_spy_task_open();
int s_is_in_clan_spy_target_map();
int s_is_clan_spy_task_open();
int s_is_clan_spy_open();
int s_clan_spy_open();
int s_new_dart_car();
int s_reset_dart_car();
int s_reset_dart_car_check();
int s_get_dart_car_type();
int s_set_call_var();
int s_in_dart_car();
int s_is_player_in_jail();
int s_get_spy_target_clan_id();
int s_add_target_clan_stable();
int s_set_player_spy_task_stage_done();
int s_get_pworld_timer_mon_alive();
int s_get_pworld_timer();
int s_get_pworld_wave_mon_alive();
int s_get_pworld_wave_mon_count_down();
int s_pworld_push_timer_mon();
int s_get_pworld_timer_mon_count_down();
int s_set_pworld_wave_mon_count_down();
int s_get_role_ride_on_machine_mem_id();
int s_set_pworld_timer();
int s_get_pworld_timer_mon_round();
int s_get_pworld_wave_mon_round();
int s_set_pworld_timer_mon_count_down();
int s_set_pworld_timer_mon_count_down_wave();
int s_pworld_push_wave_mon();
int s_clan_machine_pworld_create();
int s_is_pworld_over();

//基友配对邀请
int s_gay_invite();

//是不是有效的基友发起者
int s_is_gay_gong();
int s_enter_clan_machine_pworld();
int s_is_clan_machine_pworld_open();
int s_get_machine_pworld_status_info();

int s_new_map_life_npc();

int s_set_money_type();
int s_get_machine_pworld_status();
int s_cow_pworld_create();
int s_cow_pworld_staus();
int s_cow_final_range();
int s_cow_final_end();
int s_cow_pworld_enter();
int s_open_cow_final_range_ui();
int s_cow_final_begin();
int s_get_vaild_item_num_inpak();
int s_dec_vaild_item_num_inpak();
int s_get_strong_sale_info();
int s_strong_player_award();
int s_strong_add_res();
int s_add_id_player_contri_test();
int s_add_id_clan_money();
int s_add_id_clan_money_test();
int s_get_id_player_contri();
int s_record_id_player_clan_donate();
int s_add_id_player_contri();
int s_strong_enter_pworld();
int s_strong_is_open();
int s_strong_cancel();
int s_strong_is_mine();
int s_get_top_by_level();
int s_add_bind_item_num_inpak_money();
int s_add_item_num_inpak_money();
int s_get_yueyu_num();
int s_add_yueyu_num();
int s_set_event_var_by_id();
int s_get_event_var_by_id();
int s_ride_off();
int s_ronglu_task_get();
int s_call_ronglu_mon();
int s_ronglu_is_full();
int s_ronglu_new_collect_mon();
int s_ronglu_get_taskid();
int s_ronglu_get_itemid();
int s_get_ronglu_task_num();

int s_gold_rate_get_money();
int s_gold_rate_dec_money();
int s_gold_rate_get_daynum();
int s_get_rand_luky_map();
int s_set_clan_everydaypworld_boss_die();
int s_add_item_bat_inpak_nolimit();
int s_add_item_num_inpak_nolimit();
int s_add_bind_item_bat_inpak_nolimit();
int s_add_bind_item_num_inpak_nolimit();
int s_get_clan_create_time();
int s_is_same_day();
int   s_set_weather();
int s_player_worship_effigy_test();
int s_player_worship_effigy();
int s_player_effigy_award();
int s_city_effigy_info();
int s_is_uin_flag_set();
int s_clan_join_time_no_sameday();
int s_get_curr_max_level();
int s_player_busy_ui();
int s_is_first_enter_strong();
int s_clan_has_strong_glory();
int s_player_is_strong_member();
int s_player_machine_add_hp();
int s_pworld_camp_add_exp();
int s_player_chg_machine();
int s_get_mon_num_radius();
int s_strong_open_glory();
int s_get_star_info();
int s_clan_baby_feed();
int s_get_clan_baby_feed();
int s_get_clan_baby_stat();
int s_clan_baby_call();
int s_ronglu_chg_gold();
int s_player_clan_baby_award();
int s_clan_baby_die();
int s_taskcycle_stat();
int s_get_dayonline();
int s_arm_is_fashion();
int s_player_transmit_rand();
int s_get_pre_car();
int s_resume_dart_car();
  int s_new_map_life_npc_ex();
int s_get_npc_defid();
int s_exchg_item();
int s_exchg_item_get();
int s_pworld_player_exp();
int s_commit_arm_wash_addattr();
int s_open_school_invite_ui();
int s_school_fini();
int s_get_reg_info();
int s_commit_xukong_energy();
int s_buy_hetong();
int s_pworld_box_start();
int s_pworld_box_award();

int s_hang_hai_begin();
int s_hang_hai_end();
int s_hang_hai_range();
int s_open_hang_hai_range_ui();

int s_open_yanghaibang_ui();

int s_player_yanghaibang_start();
int s_player_yanghaibang_start_check();

int s_pinjiu_screen_vague();

int s_get_pingjiu_var();
int s_set_pingjiu_var();
int s_store_open_ui();

int s_clan_open_dart();

int s_item_is_chenghao();
int s_item_is_fumo();
int s_item_is_zhuzhao();
int s_item_is_lianjin();
int s_open_npc_shop_by_id();

int s_player_clan_develop_ranking();
int s_clan_is_open_dart();
int s_camp_mon_map_show_action();
int s_set_camp_mon_map_show();
int s_use_item_is_bind();
int s_mon_set_owner();
int s_set_paoshang_info();
int s_get_paoshang_info();
int s_open_paoshang_ui();
int s_paoshang_duihuan_refresh();


int s_get_positem_info();
int s_set_positem_info();
int s_positem_dir();

int s_daqiangdao_fenshu();

int  s_npc_set_dialog_old_stat();
int s_gold_rate_daynum_add();
int s_strong_active_level();
int s_range_clan_fanrong();
int s_enter_clan_machine_pworld_unite();
int s_is_exist_machine_pworld_unite();
int s_ploy_gc_set_award();
int s_ploy_gc_get_award();
int s_ploy_gc_ui_info();
int s_ploy_gc_end_player_clear();
int s_del_npc_by_mapid();
int s_get_event_exist_time();
int s_new_map_life_npc1();
int s_player_is_jiyang();
int s_player_clan_pet_unlock();

int s_player_dianfeng_first_info();
int s_player_dianfeng_award();
int s_player_dianfeng_front_champion();
int s_player_dianfeng_set_effigy();
int s_dianfeng_if_open();
int s_dianfeng_add_status();
int s_dianfeng_clear_baoming_info();
int s_player_dianfeng_award_check();

int s_get_mzt_layer();
int s_unreal_get_exp_layer();
int s_sub_player_hp();

int s_remove_relation_ui();
int s_broadcast_wedding_tip();
int s_oath_ui();
int s_begin_honeymoon_travel_check();
int s_begin_wedding_celebration_check();
int s_begin_wedding_check();
int s_get_wedding_sth();
int s_set_wedding_sth();
int s_order_wedding_meal_check();

int s_new_machine_patrol_spouse();
int s_machine_spouse_chg_map();
int s_machine_clear();
int s_check_world_limit();
int s_sys_mail_bind();

int s_boss_baoxiang_touch_set();
int s_boss_baoxiang_touch_get();

int s_bct_baoxiang_if_open();

int s_word_pwd_ui();
int s_word_pwd_rand_new_mon();
int s_get_word_pwd_mon_info();
int s_span_map_transmit();
int s_get_world_camp_in_span_map();
int s_get_wear_pos1();
int s_get_baoxiang_touch_limit();

int s_home_add_exp();
int s_home_kaiken();

int s_career_alter_check();
int s_career_alter();

int s_get_global_var();
int s_set_global_var();

int s_home_open_ui();
int s_home_enter();

int s_apply_pworld();

int s_cancel_span_queue();
int s_cancel_span_queue_check();

int s_home_info();







#endif /* ZONE_SCRIPT_H */

