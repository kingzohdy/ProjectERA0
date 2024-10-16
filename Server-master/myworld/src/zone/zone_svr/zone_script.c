/*
**  @file $RCSfile: zone_script.c,v $
**  general description of this module
**  $Id: zone_script.c,v 1.1175 2014/06/30 03:49:09 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/06/30 03:49:09 $
**  @version $Revision: 1.1175 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_script.h"
#include "mcdef.h"
#include "og_script.h"
#include "zone_npc.h"
#include "zone_clt.h"
#include "zone_err.h"
#include "zone_package.h"
#include "zone_combat.h"
#include "zone_skill.h"
#include "zone_attr.h"
#include "zone_team.h"
#include "zone_tmp.h"
#include "zone_task.h"
#include "zone_player.h"
#include "zone_map.h"
#include "zone_pet.h"
#include <time.h>
#include "zone_status.h"
#include "zone_clan.h"
#include "zone_arm.h"
#include "zone_clan.h"
#include "zone_clan_city.h"
#include "zone_clan_construction.h"
#include "zone_clan_advance.h"
#include "zone_relation.h"
#include "zone_fairy.h"
#include "mapmask.h"
#include "zone_ai.h"
#include "zone_designation.h"
#include "zone_range.h"
#include "zone_secondaryskill.h"
#include "zone_oplog.h"
#include "zone_act_status.h"
#include "zone_mail.h"
#include "zone_machine.h"
#include "zone_ai.h"
#include "zone_battle.h"
#include "zone_itemlimit.h"
#include "zone_diffpworld.h"
#include "zone_safelock.h"
#include "zone_booty.h"
#include "zone_trans.h"
#include "zone_city.h"
#include "zone_task.h"
#include "zone_unreal.h"
#include "zone_booty.h"
#include "zone_gay.h"
#include "zone_strongpoint.h"
#include "zone_misc.h"
#include "zone_ronglu.h"
#include "zone_span.h"
#include "zone_shop.h"
#include "zone_weather.h"
#include "zone_busy.h"
#include "zone_vip.h"
#include "zone_pworld.h"
#include "zone_addattr.h"
#include "zone_syncrelation.h"
 #include <iconv.h>
 #include "zone_store.h"
 #include "zone_zh_name.h"
 #include "zone_ploy.h"
 #include "zone_clan_pet.h"
 #include "zone_dianfeng.h"
 #include "zone_home.h"
 #include "zone_shikong.h"
 #include "zone_sparta.h"

#define MAX_VAR_LEN 32

extern int g_team_follow_flag;
extern int g_invaild_dec;

typedef struct {
    int iFunctionID ;
    int (*UserScriptFunc)() ;
    int iExecNum;
    int iExecTime;
} ScriptFunc ;

#define SCRIPT_FUNC_SIZE sizeof(ScriptFunc)

typedef struct {
		char sName[RES_NAME_LEN];
		time_t tFileTime;
} ScriptHead;

#define SCRIPT_HEAD_SIZE	64

typedef struct {
		ScriptHead stHead;
		char sRes[SCRIPT_HEAD_SIZE - sizeof(ScriptHead)];
		char sData[SCRIPT_HUGE_SIZE];
} Script;

ScriptEnv stSEnv;

ScriptFunc ScriptList[] = { //为了效率考虑，脚本命令号请递增!!
	/*iFunctionID              FunctionPointer                */
	{CMD_DIALOG ,            npc_dialog, 0, 0 },
	{CMD_TALK   ,              npc_talk, 0, 0     },
	{CMD_ADD_TALK ,        npc_addtalk, 0, 0 },
	{CMD_OPTION   ,          npc_option, 0, 0  },	
	{CMD_NEXT   ,             npc_next, 0, 0    },	
	{CMD_OPEN_STASH,     npc_stash, 0, 0    },
	{CMD_GET_PLAYER_BASE_ATT,     s_get_player_base_att, 0, 0    },
	{CMD_ENTER_PWORLD,     s_enter_pworld, 0, 0    },
	{CMD_GET_PWORLD_INFO,     s_get_pworld_info, 0, 0    },
	{CMD_GET_ITEM_NUM_INPAK,     s_get_item_num_inpak, 0, 0    },
	{CMD_DEC_ITEM_NUM_INPAK,     s_dec_item_num_inpak, 0, 0    },
	{CMD_ADD_EXP,     s_add_exp, 0, 0    },
	{CMD_NEW_MON,     s_new_mon, 0, 0    },	
	{CMD_GET_MON_NUM,     s_get_mon_num, 0, 0    },	
	{CMD_TRANSMIT,     s_transmit, 0, 0    },		
	{CMD_MSG_INFO,     s_msg_info, 0, 0    },	
	{CMD_GET_PWORLD_VAR,     s_get_pworld_var, 0, 0    },	
	{CMD_SET_PWORLD_VAR,     s_set_pworld_var, 0, 0    },	
	{CMD_MAP_MSG_INFO,     s_map_msg_info, 0, 0    },
	{CMD_SVR_MSG_INFO,     s_svr_msg_info, 0, 0    },	
	{CMD_DEL_MON,     s_del_mon, 0, 0    },		
	{CMD_MON_SKILL,     s_mon_skill, 0, 0    },	
	{CMD_ADD_MONEY,     s_add_money, 0, 0    },	
	{CMD_IS_CAPTAIN,     s_is_captain, 0, 0    },	
	{CMD_TEAM_TRANS,     s_team_trans, 0, 0    },	
	{CMD_GET_RAN_POS,     s_get_ran_pos, 0, 0    },	
	{CMD_NEW_TMP_ITEM,     s_new_tmp_item, 0, 0    },	
	{CMD_GET_PLAYER_ID,     s_get_player_id, 0, 0    },	
	{CMD_GET_TEAM_PLAYER_ID,     s_get_team_player_id, 0, 0    },	
	{CMD_ADD_SYS_MSG_START,     s_add_sys_msg_start, 0, 0    },	
	{CMD_ADD_SYS_MSG,     s_add_sys_msg, 0, 0    },
	{CMD_ADD_SYS_MSG_POS,     s_add_sys_msg_pos, 0, 0    },	
	{CMD_SET_SYS_MSG_MODE,     s_set_sys_msg_mode, 0, 0    },	
	{CMD_OPEN_NPC_SHOP,     s_open_npc_shop, 0, 0    },	
	{CMD_RESET_PWORLD,     s_reset_pworld, 0, 0    },	
	{CMD_NEW_NPC,     s_new_npc, 0, 0    },
	{CMD_DEL_NPC,     s_del_npc, 0, 0    },
	{CMD_GET_NPC_NUM,     s_get_npc_num, 0, 0    },	
	{CMD_PWORLD_OVER,     s_pworld_over, 0, 0    },	
	{CMD_GET_TASK_VAR,     s_get_task_var, 0, 0    },	
	{CMD_SET_TASK_VAR,     s_set_task_var, 0, 0    },	
	{CMD_TASK_ADD_OPTION,      s_task_add_option, 0, 0    },	
	{CMD_TASK_DO_OPTION,      s_task_do_option, 0, 0    },
	{CMD_TASKCYCLE_DO_OPTION,      s_taskcycle_do_option, 0, 0    },	
	{CMD_TASK_FINI,          s_task_fini, 0, 0    },	
	{CMD_TASK_STAT,          s_task_stat, 0, 0    },	
	{CMD_EFFECT,              s_effect, 0, 0    },
	{CMD_AUTOFLY,           s_autofly, 0, 0    },	
	{CMD_GET_TRIG_TASK,           s_get_trig_task, 0, 0    },
	{CMD_OPEN_UI,           npc_ui, 0, 0    },
	{CMD_GET_SKILL_INFO,           s_get_player_skillinfo, 0, 0    },
	{CMD_GET_DIATHESIS_INFO,           s_get_player_diathesisinfo, 0, 0    },
	{CMD_ADD_ITEM_NUM_INPAK,           s_add_item_num_inpak, 0, 0    },
	{CMD_ADD_ITEM_BAT_INPAK,           s_add_item_bat_inpak, 0, 0    },
	{CMD_GET_NOW_TIME,           s_get_now_time, 0, 0    },
	{CMD_GET_POINT_DIST,           s_get_point_distance, 0, 0    },
	{CMD_GET_PLAYER_MAPINFO,           s_get_player_mapinfo, 0, 0    },
	{CMD_GET_MAP_RANDPOINT,           s_get_map_randpoint, 0, 0    },
	{CMD_OUT_PWORLD_BY_SINGLE,           s_out_pworld_by_single, 0, 0    },
	{CMD_OUT_PWORLD_BY_TEAM,           s_out_pworld_by_team, 0, 0    },
	{CMD_GET_TIME_SEC,           s_get_time_sec, 0, 0    },
	{CMD_GET_TMP_NUM,           s_get_tmp_num, 0, 0    },
	{CMD_GET_ROLE_VAR,           s_get_role_var, 0, 0    },
	{CMD_SET_ROLE_VAR,           s_set_role_var, 0, 0    },	
	{CMD_GET_MON_VAR,           s_get_mon_var, 0, 0    },
	{CMD_SET_MON_VAR,           s_set_mon_var, 0, 0    },	
	{CMD_OPEN_PWORLD_UI,           npc_pworld_ui, 0, 0    },
	{CMD_GET_MON_INFO,           s_get_mon_info, 0, 0    },
	{CMD_SET_MON_BUFF,           s_set_mon_buff, 0, 0    },
	{CMD_GET_ITEM_OBJ_INFO,           s_get_item_obj_info, 0, 0    },
	{CMD_GET_PET_INFO,           s_get_pet_info, 0, 0    },
	{CMD_TRANSMIT_CAMERA,           s_transmit_camera, 0, 0    },	
	{CMD_COMPEL_CLOSE_PWORLD,           s_compel_close_pworld, 0, 0    },	
	{CMD_CALL_MON,           s_call_mon, 0, 0    },	
	{CMD_ADD_PET_FIGHT,           s_add_pet_fight, 0, 0    },	
	{CMD_IS_IN_PWORLD,           s_is_in_pworld, 0, 0    },	
	{CMD_NEW_MON_BY_DIR,           s_new_mon_by_dir, 0, 0    },	
	{CMD_ADD_XP,           s_add_xp, 0, 0    },	
	{CMD_GET_MAP_NPC_ID,           s_get_map_npc_id, 0, 0    },	
	{CMD_EFFECT_ID,           s_effect_id, 0, 0    },
	{CMD_ENTER_CLAN_CITY,     s_enter_clan_city, 0, 0 },
	{CMD_DYNAMIC_OPTION_LIST,     npc_dynamic_option_list, 0, 0 },
	{CMD_DYNAMIC_DO_TASK_OPTION,     npc_dynamic_do_task_option, 0, 0 },
	{CMD_CLAN_PAY,     s_clan_pay, 0, 0 },
	
	{CMD_GET_TARGET_INFO,       s_get_target_info, 0, 0 },
	{CMD_TARGET_ADD_STATUS,     s_target_add_status_by_memid, 0, 0 },
	{CMD_HAS_STATUS,            s_player_has_status, 0, 0 },
	{CMD_CLEAN_TARGET,          s_target_clean_by_memid, 0, 0 },
	{CMD_TARGET_RELATION,       s_target_relation_by_memid, 0, 0 },
	{CMD_TASKCYCLE_ADD_OPTION,       s_taskcycle_add_option, 0, 0 },
	{CMD_SET_SVR_STATUS,       s_set_svr_status, 0, 0 },
	{CMD_SET_EVENT_VAR,       s_set_event_var, 0, 0 },
	{CMD_GET_EVENT_VAR,       s_get_event_var, 0, 0 },
	{CMD_SET_EVENT_BULLETIN,       s_set_svr_bulletin, 0, 0 },
	{CMD_SET_EVENT_PWORLD,       s_set_event_pworld, 0, 0 },
	{CMD_RANGE_ADD_EXP,       s_range_add_exp, 0, 0 },
	{CMD_RANGE_ADD_STATUS,       s_range_add_status, 0, 0 },
	{CMD_GET_EVENT_STAT,       s_get_event_stat, 0, 0 },
	{CMD_NEW_LIFE_MON,       s_new_life_mon, 0, 0 },
	{CMD_GET_PWORLD_ROLE_NUM,       get_pworld_role_num, 0, 0 },
	{CMD_ADD_TMP_ITEM,       s_add_tmp_item, 0, 0 },
	{CMD_ADD_TMP_FAIRY,       s_add_fairy_fight, 0, 0 },
	{CMD_GET_AUTO_FLY_INFO,       s_get_autofly_info, 0, 0 },
	{CMD_GET_ITEM_ONEDAY_INFO,       s_get_item_oneday_info, 0, 0 },
	{CMD_ITEM_ADD_ONEDAY,       s_item_add_oneday, 0, 0 },
	{CMD_GET_MAP_MON_ID,       s_get_map_mon_id, 0, 0 },
	{CMD_USE_TREASURE_PIC,       s_use_treasure_pic, 0, 0 },
	{CMD_TASK_SHARE_TEST,       s_task_share_test, 0, 0 },
	{CMD_TEAM_TASK_SHARE,       s_team_task_share, 0, 0 },
	{CMD_MON_TASK_AREA_CHECK,       s_mon_task_area_check, 0, 0 },
	{CMD_MON_CHG_MAP,       s_mon_chg_map, 0, 0 },
	{CMD_MON_CALL_ENEMY_MON,       s_mon_call_enemy_mon, 0, 0 },
	{CMD_SET_MON_TASK_STAT,       s_set_mon_task_stat, 0, 0 },
	{CMD_TEAM_MSG_INFO,       s_team_msg_info, 0, 0 },
	{CMD_ID_PLAYER_BASE_ATT,     s_id_player_base_att, 0, 0    },
	{CMD_NEW_CONVOY_MON,     s_new_convoy_mon, 0, 0    },
	{CMD_GROUP_ADD_ONEDAY,     s_group_add_oneday, 0, 0    },
	{CMD_GET_GROUP_ONEDAY_INFO,     s_get_group_oneday_info, 0, 0    },
	{CMD_NEW_MAP_MON,     s_new_map_mon, 0, 0    },
	{CMD_TEAM_PWORLD_INFO,     s_team_pworld_info, 0, 0    },
	{CMD_ENTER_PWORLD_NUM_TEST,     s_enter_pworld_num_test, 0, 0    },
	{CMD_GET_MON_PATROL_ID,     s_get_mon_patrol_id, 0, 0    },
	{CMD_GET_MAP_MON_NUM,     s_get_map_mon_num, 0, 0    },
	{CMD_GET_ITEM_USE_DUR,     s_get_item_use_dur, 0, 0    },
	{CMD_ADD_ITEM_ON_MAP,     s_add_item_on_map, 0, 0    },
	{CMD_IS_MY_CONVOY_MON,     s_is_my_convoy_mon, 0, 0    },
	{CMD_GET_ID_PLAYER_STATUS_INFO,     s_get_id_player_status_info, 0, 0    },
	{CMD_GET_ID_MON_STATUS_INFO,     s_get_id_mon_status_info, 0, 0    },
	{CMD_RESET_ALL_GENIUS,     s_reset_all_genius, 0, 0    },

	{CMD_GET_MON_FIGHT_TIME_COUNT, s_get_mon_fight_time_count, 0, 0 },
	{CMD_MODIFY_MON_PRESS, s_modify_mon_press, 0, 0 },
	{CMD_MON_ATK_SPECIFIC_TARGET, s_mon_atk_specific_target, 0, 0 },
	{CMD_MON_SPEECH, s_mon_speech, 0, 0 },
	{CMD_NEW_MON_AND_FORCED_MOVE, s_new_mon_and_forced_move, 0, 0 },
	{CMD_GET_MON_CURRENT_PATROL_POS, s_get_mon_current_patrol_pos, 0, 0 },
	{CMD_SET_MON_CURRENT_PATROL_TIME, s_set_mon_current_patrol_time, 0, 0 },
	{CMD_ADD_PWORLD_RESOURCE, s_add_pworld_resource, 0, 0 },
	{CMD_GET_DISTANCE_BETWEEN_2MON, s_get_distance_between_2mon, 0, 0 },
	{CMD_GET_MON_NUM_BY_DEFID, s_get_mon_num_by_defid, 0, 0 },
	{CMD_MON_GET_PLAYER_NUM_BY_CAREER, s_get_player_num_in_mon_range_by_career, 0, 0 },
	{CMD_MON_GET_FRIEND_NUM, s_get_friend_num_in_mon_range, 0, 0 },
	{CMD_GET_MON_HP_PERCENT, s_get_mon_hp_percent, 0, 0 },
	{CMD_MON_PRESS_GET_FRIEND_PLAYER_NUM, s_get_friend_player_num_in_mon_press_order_player, 0, 0 },
	{CMD_DELETE_MON_BY_DEFID, s_delete_mon_by_defid, 0, 0 },
	{CMD_ID_PLAYER_GROUP_ADD_ONEDAY, s_id_player_group_add_oneday, 0, 0 },
	{CMD_ID_PLAYER_GROUP_ONEDAY_INFO, s_id_player_group_oneday_info, 0, 0 },
	{CMD_SET_EVENT_PWORLD_QUEUE, s_set_event_pworld_queue, 0, 0 },
	{CMD_APPLY_PWORLD_QUEUE, s_apply_pworld_queue, 0, 0 },
	{CMD_DESTROY_MON, s_destroy_mon, 0, 0 },
	{CMD_MON_OWNER_MSG_INFO, s_mon_owner_msg_info, 0, 0 },
	{CMD_GET_ID_PLAYER_INFO, s_get_id_player_info, 0, 0 },
	{CMD_ADD_FINI_TASK, s_add_fini_task, 0, 0 },
	{CMD_DEL_ID_MON, s_del_id_mon, 0, 0 },
	{CMD_GET_PACK_SPACE, s_get_pack_space, 0, 0 },
	{CMD_NEW_LIFE_NPC, s_new_life_npc, 0, 0 },
	{CMD_IS_SAME_WEEK, s_is_same_week, 0, 0 },
	{CMD_NEW_OWNER_TMP_ITEM, s_new_owner_tmp_item, 0, 0 },
	{CMD_ADD_PLAYER_HP, s_add_player_hp, 0, 0 },
	{CMD_ADD_PLAYER_MP, s_add_player_mp, 0, 0 },
	{CMD_ADD_FIGHT_PET_HP, s_add_fight_pet_hp, 0, 0 },
	{CMD_PLAYER_TRANSMIT_CAMERA, s_player_transmit_camera, 0, 0 },
	{CMD_CALL_MON_BY_PATROL, s_call_mon_by_patrol, 0, 0 },
	{CMD_PWORLD_REVIVE_PLAYER, s_pworld_revive_player, 0, 0 },
	{CMD_GET_ROLE_TMP_ITEM, s_get_role_tmp_item, 0, 0 },
	{CMD_DEL_ROLE_TMP_ITEM, s_del_role_tmp_item, 0, 0 },
	{CMD_DEL_ROLE_UNUSED_TMP_ITEM, s_del_role_unused_tmp_item, 0, 0 },
	{CMD_TASK_RUN, s_task_run, 0, 0 },
	{CMD_DEC_MONEY, s_dec_money, 0, 0 },
	{CMD_NPC_DIE, s_npc_die, 0, 0 },
	{CMD_HAS_ID_PLAYER_DESIGNATION, s_has_id_player_designation, 0, 0 },
	{CMD_TASK_FAIL, s_task_fail, 0, 0 },
	{CMD_PLAYER_RIDE_ON, s_player_ride_on, 0, 0 },
	{CMD_RIDE_DROP, s_del_player_ride, 0, 0 },
	{CMD_RANGE_SET, s_range_set, 0, 0 },
	{CMD_RUN_TASK_PROGRESS, s_run_task_progress, 0, 0 },
	{CMD_BCT_GET_CURR_LAYER, s_bct_get_curr_layer, 0, 0 },
	{CMD_BCT_PASS, s_bct_pass, 0, 0 },
	{CMD_BCT_SHEST_AWARD, s_bct_shest_award, 0, 0 },
	{CMD_BCT_UI, s_bct_ui, 0, 0 },
	{CMD_BCT_CALL_MON, s_bct_call_mon, 0, 0 },
	{CMD_PLAYER_COLLECT_BREAK, s_player_collect_break, 0, 0 },
	{CMD_SET_PWORLD_MAP_CURR, s_set_pworld_map_curr, 0, 0 },
	{CMD_CAREER_LEVEL_UP, s_career_level_up, 0, 0 },
	{CMD_EXP_LIMIT, s_get_player_explimit, 0, 0 },
	{CMD_SET_MON_CURR, s_set_mon_curr, 0, 0 },
	{CMD_MON_SET_HP, s_mon_set_hp, 0, 0 },
	{CMD_GET_MON_PATROLIDX, s_get_mon_patrolIdx, 0, 0 },
	{CMD_SET_PLAYER_CURR, s_set_player_curr, 0, 0 },
	{CMD_EXIT_CLAN, s_exit_clan, 0, 0 },
	{CMD_USE_CANGBAOTU_PIC,       s_use_cangbaotu_pic, 0, 0 },
	{CMD_COMMIT_TASK,       s_commit_task, 0, 0 },
	{CMD_DEC_MONEY_TEST,       s_dec_money_test, 0, 0 },
	{CMD_GET_ID_MON_INFO,       s_get_id_mon_info, 0, 0 },
	{CMD_NEW_LINKED_MON_SLOT,	s_new_linked_mon_slot, 0, 0 },
	{CMD_LINK_MON,	s_link_mon, 0, 0 },
	{CMD_GET_PLAYER_SECONDARY_SKILL,	s_get_player_secondary_skill, 0, 0 },
	{CMD_ENTER_LOGIC_AREA,	s_enter_logic_area, 0, 0 },
	{CMD_LEAVE_LOGIC_AREA,	s_leave_logic_area, 0, 0 },
	{CMD_SET_MON_GROUP, s_set_mon_group, 0, 0 },
	{CMD_ROLE_ITEM_NUM, s_role_item_num, 0, 0 },
	{CMD_HAS_FAIRY, s_has_fairy_by_id, 0, 0 },
	{CMD_PWORLD_LINKED_MON_INFO, s_pworld_linked_mon_info, 0, 0 },
	{CMD_PLAYER_NEW_LINKED_MON, s_player_new_linked_mon, 0, 0 },
	{CMD_NEW_NPC_PRIVATE, s_new_npc_private, 0, 0 },
	{CMD_PWORLD_AGAIN, s_pworld_again, 0, 0 },
	{CMD_PROCEVT_PLAYER_NUM, s_procevt_player_num, 0, 0 },
	{CMD_CALL_MON_TEAM_ALL, s_call_mon_team_all, 0, 0 },

	{CMD_NPC_COMMIT_OPT, npc_commit_opt, 0, 0 },
	{CMD_GET_COMMIT_ITEM_INFO, s_get_commit_item_info, 0, 0 },
	{CMD_DEC_ITEM_NUM_BYIDX, s_dec_item_num_byldx, 0, 0 },
	{CMD_GET_ROLE_TMP_MACHINE, s_get_role_tmp_machine, 0, 0 },
	{CMD_SET_DIALOG_STYLE, set_dialog_style, 0, 0 },

	{CMD_CIRCLE_NEW_MON, s_circle_new_mon, 0, 0 },

	{CMD_APPLY_TMP_ITEM, s_apply_tmp_item, 0, 0 },
	{CMD_GIVEBACK_CLAN_MACHINE, s_giveback_clan_machine, 0, 0 },
	{CMD_LAYDOWN_CLAN_ORE, s_laydown_clan_ore, 0, 0 },
	{CMD_GET_PLAYER_PKVAL_TYPE, s_get_player_pkval_type, 0, 0 },

	{CMD_SET_MAP_VAL, s_set_mapinst_var, 0, 0 },
	{CMD_GET_MAP_VAL, s_get_mapinst_var, 0, 0 },

	{CMD_PLAYER_CLOSE_DIALOG, s_player_close_dialog, 0, 0 },

	{CMD_SYS_MAIL, s_sys_mail, 0, 0 },
	{CMD_MAIL_TEXT, s_mail_text, 0, 0 },
	{CMD_ADD_DESIGNATION, s_add_designation, 0, 0 },
	{CMD_CHOOSE, s_choose, 0, 0 },
	{CMD_ADD_ONLINE_ATTR, s_add_online_attr, 0, 0 },
	{CMD_LEARN_SEC_SKILL, s_learn_sec_skill, 0, 0 },
	{CMD_IS_MON_FIGHTING, s_is_mon_fighting, 0, 0 },
	{CMD_SET_BATTLE_WIN, s_set_battle_win, 0, 0 },
	{CMD_SET_MON_CAMP_IDX, s_set_mon_camp_idx, 0, 0 },
	{CMD_SET_REVIVE_POS_CAMP, s_set_revive_pos_camp, 0, 0 },
	{CMD_NEW_DYN_MON, s_new_dyn_mon, 0, 0 },
	{CMD_GET_ATTACKER_CAMP, s_get_attacker_camp, 0, 0 },	
	{CMD_PLAYER_JUMP, s_player_jump, 0, 0 },
	{CMD_NEW_CAMP_NPC, s_new_camp_npc, 0, 0 },
	{CMD_MON_GET_PLAYER_NUM_IN_RECT, s_mon_get_player_num_in_rect, 0, 0 },
	{CMD_PLAYER_MOVIE_START, s_player_movie_start, 0, 0 },
	{CMD_GET_CLAN_BUILDING_LV, s_get_clan_building_lv, 0, 0 },
	{CMD_GET_PLAYER_TASK_SPRITE_FLAG, s_get_player_task_sprite_flag, 0, 0 },
	{CMD_SET_PLAYER_TASK_SPRITE_FLAG, s_set_player_task_sprite_flag, 0, 0 },
	{CMD_GET_BATTLE_LEVEL_LIMIT, s_get_battle_level_limit, 0, 0 },
	{CMD_GOTO_CLAN_CITY, s_goto_clan_city, 0, 0 },
	{CMD_GET_MON_GROUP, s_get_mon_group, 0, 0 },
	{CMD_GET_RAND_PWORLD_PLAYER_ID, s_get_rand_pworld_player_id, 0, 0 },	
	{CMD_NEW_MON_MACHINE, s_new_mon_machine, 0, 0 },
	{CMD_MON_FORCED_MOVE, s_mon_forced_move, 0, 0 },
	{CMD_PLAYER_SET_HIDE_TASK, s_player_set_hide_task, 0, 0 },
	{CMD_MON_FORCED_MOVE_POS, s_mon_forced_move_pos, 0, 0 },
	{CMD_GET_RADIAN, s_get_radian, 0, 0 },
	{CMD_GET_ANGLE_POS, s_get_angle_pos, 0, 0 },
	{CMD_IS_PLAYER_OWNER_MON, s_is_player_owner_mon, 0, 0 },
	{CMD_COMMIT_CLAN_ITEM, s_commit_clan_item, 0, 0 },
	{CMD_PLAYER_CALL_BUB, s_player_call_bub, 0, 0 },
	{CMD_GET_PLAYER_MAP_LINE, s_get_player_map_line, 0, 0 },
	{CMD_SET_PLAYER_MAP_LINE, s_set_player_map_line, 0, 0 },
	{CMD_MON_PUSH_PRESS, s_mon_push_press, 0, 0 },
	{CMD_MON_OWNER_DIS, s_mon_owner_dis, 0, 0 },
	{CMD_GET_CLAN_USED_TECH_POINT, s_get_clan_used_tech_point, 0, 0 },
	{CMD_PLAYER_TRIGGER_MON_PRESS, s_player_trigger_mon_press, 0, 0 },
	{CMD_DESTROY_MON_BY_DEFID, s_destroy_mon_by_defid, 0, 0 },
	{CMD_IS_CAN_BIANSHEN, s_is_can_bianshen, 0, 0 },
	{CMD_MON_RANGE_ADD_PLAYER_HP, s_mon_range_add_player_hp, 0, 0 },
	{CMD_SET_DESIG, s_player_set_desig, 0, 0 },
	{CMD_MON_GET_KILLER_ID, s_mon_get_killer_id, 0, 0 },
	{CMD_GET_WORLD_FIRST_START_TIME, s_get_world_first_start_time, 0, 0 },

	{CMD_ADD_BIND_ITEM_NUM_INPAK, s_add_bind_item_num_inpak, 0, 0 },
	{CMD_ADD_BIND_ITEM_BAT_INPAK, s_add_bind_item_bat_inpak, 0, 0 },
	{CMD_BATTLE_MIN_CAMP_PLAYER_NUM, s_battle_min_camp_player_num, 0, 0 },
	{CMD_GET_CMP_FAIRYID, s_get_cmp_fairyid, 0, 0 },
	{CMD_NEW_MON_MACHINE_BY_DIR, s_new_mon_machine_by_dir, 0, 0 },
	{CMD_ID_PLAYER_MOTION, s_id_player_motion, 0, 0 },

	{CMD_SET_EVENT_BATTLE, s_set_event_battle, 0, 0 },

	{CMD_PLAYER_NEED_NEW_NAME, s_player_need_new_name, 0, 0 },
	{CMD_CLAN_NEED_NEW_NAME, s_clan_need_new_name, 0, 0 },
	{CMD_MAP_NEW_NPC, s_new_map_npc, 0, 0 },

	{CMD_GET_PWORLD_FIRST_PLAYER_ID, s_get_pworld_first_player_id, 0, 0 },
	{CMD_GET_MAP_INST, s_get_map_inst, 0, 0 },
		
	{CMD_IS_ONE_MON_FIGHTING_BY_DEFID, s_is_one_mon_fighting_by_defid, 0, 0 },
		
	{CMD_SET_SCRIPT_INT_RET, s_set_script_int_ret, 0, 0 },

	{CMD_GET_PLAYER_NUM_RANGE, s_get_player_num_range, 0, 0 },
	{CMD_OPEN_PWORLD_AWARD_UI, s_pworld_open_award_ui, 0, 0 },

	{CMD_PKG_IS_UNLOCK, s_get_player_safelocak_info, 0, 0 },
	{CMD_PLAYER_IS_GRABTIME, s_get_player_grab_info, 0, 0 },
		
	{CMD_GET_MUL_COMMIT_ITEM_INFO, s_get_mul_commit_item_info, 0, 0 },
	{CMD_SET_DLG_COMMIT_NUM, s_set_dlg_commit_num, 0, 0 },
	{CMD_WORLD_ORE_NEW_CHECK, s_world_ore_new_check, 0, 0 },
 
	{CMD_COMMIT_MUL_CLAN_ITEM, s_commit_mul_clan_item, 0, 0 },

	{CMD_MON_OWNER_TASK_CHECK, s_mon_owner_task_check, 0, 0 },
	
	{CMD_AUTO_MOVE, s_auto_move, 0, 0 },
	{CMD_PLAYER_CLAN_POS_INFO, s_player_clan_pos_info, 0, 0 },
	{CMD_GET_ROLE_RIDE_ON_MACHINE, s_get_role_ride_on_machine, 0, 0 },
	{CMD_MAP_TRANS, s_map_trans, 0, 0 },

	{CMD_ENTER_WORLD_CITY_CHECK, s_enter_world_city_check, 0, 0 },
	{CMD_IS_WORLD_CITY_OWNER, s_is_world_city_owner, 0, 0 },

	{CMD_SET_WORLD_CITY_OPEN_TIME, s_set_world_city_open_time, 0, 0 },
	{CMD_GET_WORLD_CITY_ENABLE_TIME, s_get_world_city_enable_time, 0, 0 },
	{CMD_SET_WORLD_CITY_ENABLE_TIME, s_set_world_city_enable_time, 0, 0 },
	{CMD_GET_WORLD_CITY_OPEN_TIME, s_get_world_city_open_time, 0, 0 },
	{CMD_SET_WORLD_BOSS_FIRST_KILL_TIME, s_set_world_boss_first_kill_time, 0, 0 },
	{CMD_GET_WORLD_BOSS_FIRST_KILL_TIME, s_get_world_boss_first_kill_time, 0, 0 },
	{CMD_WORLD_CITY_AREA, s_world_city_area, 0, 0 },
	{CMD_ADD_WORLD_CITY_BUILD_SERIAL, s_add_world_city_build_serial, 0, 0 },
	{CMD_GET_CRAYFISH_NUM_ONEDAY, s_get_crayfish_num_oneday, 0, 0 },
	{CMD_COMMIT_CRAYFISH_NUM_ONCE, s_commit_crayfish_num_once, 0, 0 },
	{CMD_GET_WORLD_CITY_KEEP_NUM, s_get_world_city_keep_num, 0, 0 },
	{CMD_GET_PLAYER_JOIN_CLAN_TIME, s_get_player_join_clan_time, 0, 0 },
	{CMD_SET_WORLD_CITY_EFFIGY, s_set_world_city_effigy, 0, 0 },
	{CMD_USE_ITEM_SHOW_MAP, s_use_item_show_map, 0, 0 },
	{CMD_HAS_MASTER_WORLD_CITY, s_has_master_world_city, 0, 0 },
	{CMD_CHOOSE_WITH_TIMER, s_choose_with_timer, 0, 0 },
	{CMD_GET_MAP_RAND_MON_POS, s_get_map_rand_mon_pos, 0, 0 },
	{CMD_TARGET_ADD_STATUS_DYN, s_target_add_status_dyn, 0, 0 },
	{CMD_SET_LUCKY_OPEN, s_set_lucky_open, 0, 0 },
	{CMD_IS_PLAYER_LUCKY_ATTENDEE, s_is_player_lucky_attendee, 0, 0 },
	{CMD_IS_LUCKY_CLOSED, s_is_lucky_closed, 0, 0 },
	{CMD_CLAN_EVERYDAY_PWORLD_STAT_GET, s_clan_everyday_pworld_stat_get, 0, 0 },
	{CMD_CLAN_EVERYDAY_PWORLD_CREATE, s_clan_everyday_pworld_create, 0, 0 },
	{CMD_CLAN_EVERYDAY_PWORLD_ENTER, s_clan_everyday_pworld_enter, 0, 0 },
	{CMD_CLAN_EVERYDAY_PWORLD_VAL_CHG, s_clan_everyday_pworld_val_chg, 0, 0 },
	{CMD_FOR_EACH_MAP_OBJ, s_for_each_map_obj, 0, 0 },
	{CMD_IS_IN_MY_CLAN_CITY, s_is_in_my_clan_city, 0, 0 },
	{CMD_NEW_CLAN_MON, s_new_clan_mon, 0, 0 },
	{CMD_GET_CLAN_TECH_LEVEL, s_get_clan_tech_level, 0, 0 },
	{CMD_IS_IN_SIEGE_CITY, s_is_in_siege_city, 0, 0 },
	{CMD_GET_CLAN_STABLE, s_get_clan_stable, 0, 0 },
	{CMD_ADD_CLAN_STABLE, s_add_clan_stable, 0, 0 },
	{CMD_PLAYER_ADD_STATUS_BY_CLANMON, s_player_add_status_by_clanmon, 0, 0 },
	{CMD_GET_PLAYER_DAILY_FLAG, s_get_player_daily_flag, 0, 0 },
	{CMD_SET_PLAYER_DAILY_FLAG, s_set_player_daily_flag, 0, 0 },
	{CMD_CLAN_MSG_INFO, s_clan_msg_info, 0, 0 },
	{CMD_DYN_ACTIVE_STATUS_SET, s_dyn_active_status_set, 0, 0 },
	{CMD_CLAN_TOUCH_ADD, s_clan_touch_add, 0, 0 },
	{CMD_DYN_ACTIVE_STATUS_GET, s_dyn_active_status_get, 0, 0 },
	{CMD_UNREAL_WATCH_BOSS_INFO, s_unreal_watch_boss_info, 0, 0 },
	{CMD_UNREAL_TEAM_INFO, s_unreal_team_info, 0, 0 },
	{CMD_GOTO_UNREAL_CAPTAIN_NEAR, s_goto_unreal_captain_near, 0, 0 },
	{CMD_GET_UNREAL_LAYER, s_get_unreal_layer, 0, 0 },
	{CMD_SAVE_UNREAL_LAYER, s_save_unreal_layer, 0, 0 },
	{CMD_UNREAL_ENTER, s_unreal_enter, 0, 0 },
	{CMD_IS_LAST_UNREAL_BOSS_MAP, s_is_last_unreal_boss_map, 0, 0 },
	{CMD_UNREAL_ENTER_BOSS_CHECK, s_unreal_enter_boss_check, 0, 0 },	
	{CMD_DARKMATTER_DROPMONEY, s_darkmatter_dropmoney, 0, 0 },	
	{CMD_DARKMATTER_FLEE_DROPMONEY, s_darkmatter_flee_dropmoney, 0, 0 },
	{CMD_GET_MON_MAPDEFID, s_get_mon_mapdefid, 0, 0 },
	{CMD_TEAM_TRANS_INSAMEMAPINST, s_team_trans_insamemapinst, 0, 0 },
	{CMD_ADD_PLAYER_HP2, s_add_player_hp2, 0, 0 },
	{CMD_MON_DROP_BOOTYGROUP, s_mon_drop_bootygroup, 0, 0 },
	{CMD_GET_MON_LEVEL, s_get_mon_level, 0, 0 },
	{CMD_MAP_DEL_TMP_ITEM, s_map_del_tmp_item, 0, 0 },
	{CMD_PLAYER_CLOSE_DIALOG1, s_player_close_dialog1, 0, 0 },
	{CMD_NEW_LIFE_DYN_MON, s_new_life_dyn_mon, 0, 0 },
	{CMD_COUNT_DOWN, s_count_down, 0, 0 },
	{CMD_FOLLOW_CANCEL, s_team_follow_cancel, 0, 0 },
	{CMD_MON_DIE_ADD_TEAM_BUFF, s_mon_die_team_add_buff, 0, 0 },
	{CMD_IS_CLAN_CAN_SPY, s_is_clan_can_spy, 0, 0 },
	{CMD_IS_CLAN_ANTI_SPY_TASK_OPEN, s_is_clan_anti_spy_task_open, 0, 0 },
	{CMD_CLAN_SPY_CLOSE, s_clan_spy_close, 0, 0 },
	{CMD_CLAN_ANTI_SPY_TASK_OPEN, s_clan_anti_spy_task_open, 0, 0 },
	{CMD_CLAN_SPY_TASK_OPEN, s_clan_spy_task_open, 0, 0 },
	{CMD_IS_IN_CLAN_SPY_TARGET_MAP, s_is_in_clan_spy_target_map, 0, 0 },
	{CMD_IS_CLAN_SPY_TASK_OPEN, s_is_clan_spy_task_open, 0, 0 },
	{CMD_IS_CLAN_SPY_OPEN, s_is_clan_spy_open, 0, 0 },
	{CMD_CLAN_SPY_OPEN, s_clan_spy_open, 0, 0 },
	{CMD_NEW_DART_CAR, s_new_dart_car, 0, 0 },
	{CMD_RESET_DART_CAR, s_reset_dart_car, 0, 0 },
	{CMD_RESET_DART_CAR_CHECK, s_reset_dart_car_check, 0, 0 },
	{CMD_GET_DART_CAR_TYPE, s_get_dart_car_type, 0, 0 },
	{CMD_SET_CALL_VAR, s_set_call_var, 0, 0 },
	{CMD_IN_DART_CAR, s_in_dart_car, 0, 0 },
	{CMD_IS_PLAYER_IN_JAIL, s_is_player_in_jail, 0, 0 },
	{CMD_GET_SPY_TARGET_CLAN_ID, s_get_spy_target_clan_id, 0, 0 },
	{CMD_ADD_TARGET_CLAN_STABLE, s_add_target_clan_stable, 0, 0 },
	{CMD_SET_PLAYER_SPY_TASK_STAGE_DONE, s_set_player_spy_task_stage_done, 0, 0 },
	{CMD_GET_PWORLD_TIMER_MON_ALIVE, s_get_pworld_timer_mon_alive, 0, 0 },
	{CMD_GET_PWORLD_TIMER, s_get_pworld_timer, 0, 0 },
	{CMD_GET_PWORLD_WAVE_MON_ALIVE, s_get_pworld_wave_mon_alive, 0, 0 },
	{CMD_GET_PWORLD_WAVE_MON_COUNT_DOWN, s_get_pworld_wave_mon_count_down, 0, 0 },
	{CMD_PWORLD_PUSH_TIMER_MON, s_pworld_push_timer_mon, 0, 0 },
	{CMD_GET_PWORLD_TIMER_MON_COUNT_DOWN, s_get_pworld_timer_mon_count_down, 0, 0 },
	{CMD_SET_PWORLD_WAVE_MON_COUNT_DOWN, s_set_pworld_wave_mon_count_down, 0, 0 },
	{CMD_GET_ROLE_RIDE_ON_MACHINE_MEM_ID, s_get_role_ride_on_machine_mem_id, 0, 0 },
	{CMD_SET_PWORLD_TIMER, s_set_pworld_timer, 0, 0 },
	{CMD_GET_PWORLD_TIMER_MON_ROUND, s_get_pworld_timer_mon_round, 0, 0 },
	{CMD_GET_PWORLD_WAVE_MON_ROUND, s_get_pworld_wave_mon_round, 0, 0 },
	{CMD_SET_PWORLD_TIMER_MON_COUNT_DOWN, s_set_pworld_timer_mon_count_down, 0, 0 },
	{CMD_PWORLD_PUSH_WAVE_MON, s_pworld_push_wave_mon, 0, 0 },
	{CMD_CLAN_MACHINE_PWORLD_CREATE, s_clan_machine_pworld_create, 0, 0 },
	{CMD_IS_PWORLD_OVER, s_is_pworld_over, 0, 0 },
	{CMD_GAY_INVITE, s_gay_invite, 0, 0 },
	{CMD_IS_GAY_GONG, s_is_gay_gong, 0, 0 },
	{CMD_ENTER_CLAN_MACHINE_PWORLD, s_enter_clan_machine_pworld, 0, 0 },
	{CMD_IS_CLAN_MACHINE_PWORLD_OPEN, s_is_clan_machine_pworld_open, 0, 0 },
	{CMD_GET_MACHINE_PWORLD_STATUS_INFO, s_get_machine_pworld_status_info, 0, 0 },	
	{CMD_SET_MONEY_TYPE, s_set_money_type, 0, 0 },
	{CMD_GET_MACHINE_PWORLD_STATUS, s_get_machine_pworld_status, 0, 0 },
	{CMD_NEW_MAP_LIFE_NPC, s_new_map_life_npc, 0, 0 },	
	{CMD_COW_PWORLD_CREATE, s_cow_pworld_create, 0, 0 },
	{CMD_COW_PWORLD_STAUS, s_cow_pworld_staus, 0, 0 },
	{CMD_COW_FINAL_RANGE, s_cow_final_range, 0, 0 },
	{CMD_COW_FINAL_END, s_cow_final_end, 0, 0 },
	{CMD_COW_PWORLD_ENTER, s_cow_pworld_enter, 0, 0 },
	{CMD_OPEN_COW_FINAL_RANGE_UI, s_open_cow_final_range_ui, 0, 0 },
	{CMD_COW_FINAL_BEGIN, s_cow_final_begin, 0, 0 },
	{CMD_GET_VAILD_ITEM_NUM_INPAK, s_get_vaild_item_num_inpak, 0, 0 },
	{CMD_DEC_VAILD_ITEM_NUM_INPAK, s_dec_vaild_item_num_inpak, 0, 0 },
	{CMD_STRONG_SALE_INFO, s_get_strong_sale_info, 0, 0 },
	{CMD_STRONG_PLAYER_AWARD, s_strong_player_award, 0, 0 },
	{CMD_STRONG_PWORLD_ADD_RES, s_strong_add_res, 0, 0 },

	{CMD_ADD_ID_PLAYER_CONTRI_TEST, s_add_id_player_contri_test, 0, 0 },
	{CMD_ADD_ID_CLAN_MONEY, s_add_id_clan_money, 0, 0 },
	{CMD_ADD_ID_CLAN_MONEY_TEST, s_add_id_clan_money_test, 0, 0 },
	{CMD_GET_ID_PLAYER_CONTRI, s_get_id_player_contri, 0, 0 },
	{CMD_RECORD_ID_PLAYER_CLAN_DONATE, s_record_id_player_clan_donate, 0, 0 },
	{CMD_ADD_ID_PLAYER_CONTRI, s_add_id_player_contri, 0, 0 },
	{CMD_STRONG_ENTER, s_strong_enter_pworld, 0, 0 },
	{CMD_STRONG_IS_OPEN, s_strong_is_open, 0, 0 },
	{CMD_STRONG_CANCEL, s_strong_cancel, 0, 0 },
	{CMD_STRONG_IS_MINE, s_strong_is_mine, 0, 0 },
	{CMD_GET_TOP_BY_LEVEL, s_get_top_by_level, 0, 0 },
	{CMD_ADD_BIND_ITEM_NUM_INPAK_MONEY, s_add_bind_item_num_inpak_money, 0, 0 },
	{CMD_ADD_ITEM_NUM_INPAK_MONEY, s_add_item_num_inpak_money, 0, 0 },
	{CMD_GET_YUEYU_NUM, s_get_yueyu_num, 0, 0 },
	{CMD_ADD_YUEYU_NUM, s_add_yueyu_num, 0, 0 },

	{CMD_SET_EVENT_VAR_BY_ID, s_set_event_var_by_id, 0, 0 },
	{CMD_GET_EVENT_VAR_BY_ID, s_get_event_var_by_id, 0, 0 },
	{CMD_RIDE_OFF, s_ride_off, 0, 0 },
	
	{CMD_RONGLU_TASK_GET, s_ronglu_task_get, 0, 0 },
	{CMD_RONGLU_CALL_MON, s_call_ronglu_mon, 0, 0 },
	{CMD_RONGLU_IS_FULL, s_ronglu_is_full, 0, 0 },
	{CMD_RONGLU_CALL_COLLECT, s_ronglu_new_collect_mon, 0, 0 },
	{CMD_RONGLU_GET_TASKID, s_ronglu_get_taskid, 0, 0 },
	{CMD_RONGLU_GET_ITEMID, s_ronglu_get_itemid, 0, 0 },	
	{CMD_GET_RONGLU_TASK_NUM, s_get_ronglu_task_num, 0, 0 },

	{CMD_GOLD_RATE_GET_MONEY, s_gold_rate_get_money, 0, 0 },
	{CMD_GOLD_RATE_DEC_MONEY, s_gold_rate_dec_money, 0, 0 },
	{CMD_GOLD_RATE_GET_DAYNUM, s_gold_rate_get_daynum, 0, 0 },	
	{CMD_GET_RAND_LUKY_MAP, s_get_rand_luky_map, 0, 0 },
	{CMD_SET_CLAN_EVERYPWORLD_BOSS_DIE, s_set_clan_everydaypworld_boss_die, 0, 0 },
	{CMD_ADD_ITEM_NUM_INPAK_NOLIMIT, s_add_item_num_inpak_nolimit, 0, 0 },
	{CMD_ADD_ITEM_BAT_INPAK_NOLIMIT, s_add_item_bat_inpak_nolimit, 0, 0 },
	{CMD_ADD_BIND_ITEM_NUM_INPAK_NOLIMIT, s_add_bind_item_num_inpak_nolimit, 0, 0 },
	{CMD_ADD_BIND_ITEM_BAT_INPAK_NOLIMIT, s_add_bind_item_bat_inpak_nolimit, 0, 0 },
	{CMD_IS_SAME_DAY, s_is_same_day, 0, 0 },
	{CMD_GET_CLAN_CREATE_TIME, s_get_clan_create_time, 0, 0 },
	{CMD_SET_WEATHER, s_set_weather, 0, 0 },
	{CMD_PLAYER_WORSHIP_EFFIGY_TEST, s_player_worship_effigy_test, 0, 0 },
	{CMD_PLAYER_WORSHIP_EFFIGY, s_player_worship_effigy, 0, 0 },
	{CMD_PLAYER_EFFIGY_AWARD, s_player_effigy_award, 0, 0 },
	{CMD_CITY_EFFIGY_INFO, s_city_effigy_info, 0, 0 },
	{CMD_IS_UIN_FLAG_SET, s_is_uin_flag_set, 0, 0 },
	{CMD_CLAN_JOIN_TIME_NO_SAMEDAY, s_clan_join_time_no_sameday, 0, 0 },
	{CMD_GET_CUR_MAX_LEVEL, s_get_curr_max_level, 0, 0 },	
	{CMD_PLAYER_BUSY_UI, s_player_busy_ui, 0, 0 },
	{CMD_IS_FIRST_STRONG, s_is_first_enter_strong, 0, 0 },
	{CMD_HAS_STRONG_GLORY, s_clan_has_strong_glory, 0, 0 },
	{CMD_IS_STRONG_MEMBER, s_player_is_strong_member, 0, 0 },
	{CMD_PLAYER_CHG_MACHINE, s_player_chg_machine, 0, 0 },
	{CMD_PLAYER_MACHINE_ADD_HP, s_player_machine_add_hp, 0, 0 },
	{CMD_PLAYER_CAMP_ADD_EXP, s_pworld_camp_add_exp, 0, 0 },
	{CMD_GET_MON_NUM_RADIUS, s_get_mon_num_radius, 0, 0 },
	{CMD_STRONG_OPEN_GLORY, s_strong_open_glory, 0, 0 },
	{CMD_GET_STAR_INFO, s_get_star_info, 0, 0 },	
	{CMD_RONGLU_CHG_GOLD, s_ronglu_chg_gold, 0, 0 },
	{CMD_CLAN_BABY_FEED, s_clan_baby_feed, 0, 0 },
	{CMD_GET_CLAN_BABY_FEED, s_get_clan_baby_feed, 0, 0 },
	{CMD_GET_CLAN_BABY_STAT, s_get_clan_baby_stat, 0, 0 },
	{CMD_CLAN_BABY_CALL, s_clan_baby_call, 0, 0 },
	{CMD_PLAYER_CLAN_BABY_AWARD, s_player_clan_baby_award, 0, 0 },
	{CMD_CLAN_BABY_DIE, s_clan_baby_die, 0, 0 },
	{CMD_TASKCYCLE_STAT, s_taskcycle_stat, 0, 0 },
	{CMD_GET_DAYONLINE, s_get_dayonline, 0, 0 },
	{CMD_ARM_IS_FASHION, s_arm_is_fashion, 0, 0 },
	{CMD_PLAYER_TRANSMIT_RAND, s_player_transmit_rand, 0, 0 },
	{CMD_RESUME_DART_CAR, s_resume_dart_car, 0, 0 },
	{CMD_GET_PRE_DART_CAR, s_get_pre_car, 0, 0 },
	{CMD_SET_PWORLD_TIMER_W_COUNT_DOWN, s_set_pworld_timer_mon_count_down_wave, 0, 0 },
	{CMD_GET_NPC_DEFID, s_get_npc_defid, 0, 0 },	
	{CMD_NEW_MAP_LIFE_NPC_EX, s_new_map_life_npc_ex, 0, 0 },
	{CMD_EXCHG_ITEM, s_exchg_item, 0, 0 },
	{CMD_EXCHG_ITEM_GET, s_exchg_item_get, 0, 0 },
	{CMD_PWORLD_PLAYER_EXP, s_pworld_player_exp, 0, 0 },
	{CMD_COMMIT_ARM_WASH_ADDATTR, s_commit_arm_wash_addattr, 0, 0 },
	{CMD_SCHOOL_INVITE_UI, s_open_school_invite_ui, 0, 0 },
	{CMD_SCHOOL_FINI, s_school_fini, 0, 0 }, 
	{CMD_GET_REG_INFO, s_get_reg_info, 0, 0 },
	{CMD_COMMIT_XUKONG_ENERGY, s_commit_xukong_energy, 0, 0 }, 
	{CMD_BUY_HETONG, s_buy_hetong, 0, 0 },
	{CMD_PWORLD_BOX_START, s_pworld_box_start, 0, 0 },
	{CMD_PWORLD_BOX_AWARD, s_pworld_box_award, 0, 0 },
	{CMD_HANG_HAI_BEGIN,s_hang_hai_begin, 0, 0 },
	{CMD_HANG_HAI_END,s_hang_hai_end, 0, 0 },
	{CMD_HANG_HAI_RANGE,s_hang_hai_range, 0, 0 },
	{CMD_OPEN_HANG_HAI_RANGE_UI,s_open_hang_hai_range_ui, 0, 0 },
	{CMD_OPEN_YANG_HAI_BANG_UI,s_open_yanghaibang_ui, 0, 0 },
	{CMD_YANG_HAI_BANG_START_CHECK,s_player_yanghaibang_start_check, 0, 0 },
	{CMD_YANG_HAI_BANG_START,s_player_yanghaibang_start, 0, 0 },
	{CMD_GET_PINGJIU_VAR,s_get_pingjiu_var, 0, 0 },
	{CMD_SET_PINGJIU_VAR,s_set_pingjiu_var, 0, 0 },
	{CMD_OPEN_STORE_UI,s_store_open_ui, 0, 0 },
	{CMD_CLAN_OPEN_DART,s_clan_open_dart, 0, 0 },
	{CMD_PINJIU_SCREEN_VAGUE,s_pinjiu_screen_vague, 0, 0 },
	{CMD_ITEM_IS_CHENGHAO,s_item_is_chenghao, 0, 0 },	
	{CMD_ITEM_IS_FUMO,s_item_is_fumo, 0, 0 },	
	{CMD_ITEM_IS_ZHUZHAO,s_item_is_zhuzhao, 0, 0 },
	{CMD_ITEM_IS_LIANJIN,s_item_is_lianjin, 0, 0 },	
	{CMD_OPEN_SHOP_BY_ID,s_open_npc_shop_by_id, 0, 0 },
	{CMD_PLAYER_CLAN_DEVELOP_RANKING,s_player_clan_develop_ranking, 0, 0 },	
	{CMD_CLAN_IS_OPEN_DART,s_clan_is_open_dart, 0, 0 },	
	{CMD_USE_ITEM_IS_BIND,s_use_item_is_bind, 0, 0 },
	{CMD_CAMP_MON_MAP_SHOW_ACTION, s_camp_mon_map_show_action, 0, 0 },
	{CMD_SET_CAMP_MON_MAP_SHOW, s_set_camp_mon_map_show, 0, 0 },
	{CMD_SET_PAOSHANG_INFO, s_set_paoshang_info, 0, 0 },
	{CMD_GET_PAOSHANG_INFO, s_get_paoshang_info, 0, 0 },	
	{CMD_OPEN_PAOSHANG_UI, s_open_paoshang_ui, 0, 0 },	
	{CMD_GET_POSITEM_INFO, s_get_positem_info, 0, 0 },
	{CMD_SET_POSITEM_INFO, s_set_positem_info, 0, 0 },
	{CMD_POSITEM_DIR, s_positem_dir, 0, 0 },	
	{CMD_PAOSHANG_DUIHUAN_REFRESH, s_paoshang_duihuan_refresh, 0, 0 },	
	{CMD_DAQIANGDAO_FENSHU, s_daqiangdao_fenshu, 0, 0 },	
	{CMD_SET_MON_OWNER, s_mon_set_owner, 0, 0 },
	{CMD_SET_DLG_OLD_STAT, s_npc_set_dialog_old_stat, 0, 0 },
	{CMD_GOLD_RATE_DAYNUM_ADD, s_gold_rate_daynum_add, 0, 0 },
	{CMD_STRONG_ACTIVE_LEVEL, s_strong_active_level, 0, 0 },
	{CMD_RANGE_CLAN_FANRONG, s_range_clan_fanrong, 0, 0 },
	{CMD_ENTER_CLAN_MACHINE_PWORLD_UNITE, s_enter_clan_machine_pworld_unite, 0, 0 },
	{CMD_IS_EXIST_MACHINE_PWORLD_UINTE, s_is_exist_machine_pworld_unite, 0, 0 },
	{CMD_PLOY_GC_GET_AWARD, s_ploy_gc_get_award, 0, 0 },
	{CMD_PLOY_GC_SET_AWARDE, s_ploy_gc_set_award, 0, 0 },
	{CMD_PLOY_GC_UI_INFO, s_ploy_gc_ui_info, 0, 0 },
	{CMD_PLOY_GC_PLAYER_CLEAR, s_ploy_gc_end_player_clear, 0, 0 },
	{CMD_DEL_NPC_BY_MAPID, s_del_npc_by_mapid, 0, 0 }, 
	{CMD_GET_EVENT_EXIST_TIME, s_get_event_exist_time, 0, 0 },
	{CMD_NEW_MAP_LIFE_NPC1, s_new_map_life_npc1, 0, 0 },
	{CMD_PLAYER_IS_JIYANG, s_player_is_jiyang, 0, 0 },
	{CMD_PLAYER_PET_UNLOCK, s_player_clan_pet_unlock, 0, 0 },
	{CMD_DIANFENG_FIRST_INFO, s_player_dianfeng_first_info, 0, 0 },
	{CMD_DIANFENG_AWARD, s_player_dianfeng_award, 0, 0 },
	{CMD_DIANFENG_FRONT_CHAMPION, s_player_dianfeng_front_champion, 0, 0 },
	{CMD_DIANFENG_SET_EFFIGY, s_player_dianfeng_set_effigy, 0, 0 },
	{CMD_GET_GLOBAL_VAR, s_get_global_var, 0, 0 },
	{CMD_SET_GLOBAL_VAR, s_set_global_var, 0, 0 },
	{CMD_APPLY_PWORLD, s_apply_pworld, 0, 0 },
	{CMD_CANCEL_SPAN_QUEUE, s_cancel_span_queue, 0, 0 },
	{CMD_CANCEL_SPAN_QUEUE_CHECK, s_cancel_span_queue_check, 0, 0 },
	{CMD_DIANFENG_IF_OPEN, s_dianfeng_if_open, 0, 0 },
	{CMD_DIANFENG_ADD_STATUS, s_dianfeng_add_status, 0, 0 },
	{CMD_DIANFENG_CLEAR_BAOMING_INFO, s_dianfeng_clear_baoming_info, 0, 0 },
	{CMD_DIANFENG_AWARD_CHECK, s_player_dianfeng_award_check, 0, 0 },
	{CMD_GET_MZT_LAYER, s_get_mzt_layer, 0, 0 },
	{CMD_UNREAL_GET_EXP_LAYER, s_unreal_get_exp_layer, 0, 0 },
	{CMD_SUB_PLAYER_HP, s_sub_player_hp, 0, 0 },
	{CMD_REMOVE_RELATION_UI, s_remove_relation_ui, 0, 0 },
	{CMD_BROADCAST_WEDDING_TIP, s_broadcast_wedding_tip, 0, 0 },
	{CMD_OATH_UI, s_oath_ui, 0, 0 },
	{CMD_GET_WORLD_CAMP_IN_SPAN_MAP, s_get_world_camp_in_span_map, 0, 0}, 
	{CMD_GET_WEAR_POS1, s_get_wear_pos1, 0, 0}, 
	{CMD_GET_BAOXIANG_TOUCH_LIMIT, s_get_baoxiang_touch_limit, 0, 0},	
	{CMD_CAREER_ALTER_CHECK, s_career_alter_check, 0, 0},	
	{CMD_CAREER_ALTER, s_career_alter, 0, 0},	
	{CMD_BEGIN_HONEYMOON_TRAVEL_CHECK, s_begin_honeymoon_travel_check, 0, 0 },
	{CMD_BEGIN_WEDDING_CELEBRATION_CHECK, s_begin_wedding_celebration_check, 0, 0 },
	{CMD_BEGIN_WEDDING_CHECK, s_begin_wedding_check, 0, 0 },
	{CMD_GET_WEDDING_STH, s_get_wedding_sth, 0, 0 },
	{CMD_SET_WEDDING_STH, s_set_wedding_sth, 0, 0 },
	{CMD_ORDER_WEDDING_MEAL_CHECK, s_order_wedding_meal_check, 0, 0 },
	{CMD_NEW_MACHINE_PATROL_SPOUSE, s_new_machine_patrol_spouse, 0, 0 },
	{CMD_MACHINE_SPOUSE_CHG_MAP, s_machine_spouse_chg_map, 0, 0 },
	{CMD_MACHINE_CLEAR, s_machine_clear, 0, 0 },
	{CMD_CHECK_WORLD_LIMIT, s_check_world_limit, 0, 0 },
	{CMD_SYS_MAIL_BIND, s_sys_mail_bind, 0, 0 },
	{CMD_BOSS_BAOXIANG_TOUCH_SET, s_boss_baoxiang_touch_set, 0, 0 },
	{CMD_BOSS_BAOXIANG_TOUCH_GET, s_boss_baoxiang_touch_get, 0, 0 },
	{CMD_BCT_BAOXIANG_IF_OPEN, s_bct_baoxiang_if_open, 0, 0},	
	{CMD_WORD_PWD_UI, s_word_pwd_ui, 0, 0 },
	{CMD_WORD_PWD_RAND_NEW_MON, s_word_pwd_rand_new_mon, 0, 0 },
	{CMD_GET_WORD_PWD_MON_INFO, s_get_word_pwd_mon_info, 0, 0},	
	{CMD_SPAN_MAP_TRANSMIT, s_span_map_transmit, 0, 0},	
	{CMD_HOME_ADD_EXP, s_home_add_exp, 0, 0},	
	{CMD_HOME_KAIKEN, s_home_kaiken, 0, 0},	
	{CMD_HOME_UI, s_home_open_ui, 0, 0},	
	{CMD_HOME_ENTER, s_home_enter, 0, 0},	
	{CMD_HOME_INFO, s_home_info, 0, 0},	
		
	
};

int clean_pworld_mon(ZONESVRENV* pstEnv);

ScriptFunc *find_script_func( int iFunctionCode )
{
	static int iTotalFunc = sizeof(	ScriptList ) / SCRIPT_FUNC_SIZE ;
	int iEqual = 0 ;
	int iIdx;

	iIdx = iFunctionCode - ScriptList[0].iFunctionID;
	if (iIdx >= 0 && iIdx < iTotalFunc)
	{
		if (ScriptList[iIdx].iFunctionID == iFunctionCode)
		{
			return ScriptList+iIdx;
		}
	}

	tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "script function code = %d not sort", iFunctionCode);

	iIdx =	bsearch_int(&iFunctionCode, &ScriptList[0], iTotalFunc, SCRIPT_FUNC_SIZE, &iEqual);

	if ( iEqual ) return ScriptList+iIdx ;
	else  return NULL ;
}


ZONEMAPINST* s_get_mapinst()
{
	ZONEIDX stIdx;
	ZONEPWORLD *pstPworld;
	ZONEMAPINST *pstMapInst; 

	if (stSEnv.iMapInstFist && stSEnv.pstMapinst)
	{
		return stSEnv.pstMapinst;
	}

	if (stSEnv.pstMonster)
	{
		pstMapInst = z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stSEnv.pstMonster->stMap);
	}
	else if (stSEnv.pstNpc)
	{
		pstMapInst = z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stSEnv.pstNpc->stMon.stMap);
	}
	else if (stSEnv.pstPlayer)
	{
		stIdx.iID = stSEnv.pstPlayer->stRoleData.Map;
		stIdx.iPos = stSEnv.pstPlayer->stOnline.iMapPos;
		pstMapInst	=	z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stIdx);
	}
	else if (stSEnv.pstPworldInst)
	{
		int iMapID;
		
		pstPworld	=	z_find_pworld(stSEnv.pstEnv->pstMapObj, &stSEnv.pstPworldInst->stPworldIdx);
		if (NULL == pstPworld)
		{
			return NULL;
		}

		if(stSEnv.pstPworldInst->iMapCurr > 0)
		{
			iMapID = stSEnv.pstPworldInst->iMapCurr;
		}
		else
		{
			iMapID = pstPworld->stPworldDef.InitMapID;
		}

		stIdx.iID = ((stSEnv.pstPworldInst->iPworldInstID & 0xffff) << 16) | (iMapID & 0xffff); 
		stIdx.iPos = -1;
		pstMapInst	=	z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stIdx);
	}
	else if (stSEnv.pstMapinst)
	{
		pstMapInst = stSEnv.pstMapinst;
	}
	else
	{
		return NULL;
	}

	return pstMapInst;

}

int my_script_decoder(unsigned int code)
{
	//static struct timeval stTv1, stTv2;
	//int iTimeUse;
	//time_t lCur;
	int result = -1 ;
	ScriptFunc *pstScript = NULL ;

	if ((pstScript = find_script_func(code) ) == NULL )
	{
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "Undifined script function code = %d", code);
		return result;
	}

	
	{
		//tlog_debug(stSEnv.pstEnv->pstLogCat, 0, 0, "Function code=%0x" , code);
		//gettimeofday(&stTv1, NULL);
		result = pstScript->UserScriptFunc() ;
		/*gettimeofday(&stTv2, NULL);
		pstScript->iExecNum++;
		iTimeUse = (stTv2.tv_sec - stTv1.tv_sec)*1000000 + (stTv2.tv_usec - stTv1.tv_usec);
		pstScript->iExecTime += iTimeUse;
		if (iTimeUse > stSEnv.pstConfig->pstZoneConfig->stServer.iScriptTimeUse)
		{
			time(&lCur);
			script_printf("%s:\ncommand_code\texec_num\texec_time(us)\tAvg\t\tthis_time\t\t\n0x%04X\t%12d\t%12d\t%12d\t%12d\t\n",
					ctime(&lCur),
					pstScript->iFunctionID, pstScript->iExecNum, pstScript->iExecTime,
					pstScript->iExecTime/pstScript->iExecNum, iTimeUse);
		}*/
	}
	
	return result ;
}

TMEMPOOL *script_type_block(ZONESVRENV* pstEnv, char cScriptType)
{
	if (cScriptType == SCRIPT_MINI) return pstEnv->pstScriptMiniPool;
	else if (cScriptType == SCRIPT_MIDDLE) return pstEnv->pstScriptMiddlePool;
	else if (cScriptType == SCRIPT_LARGE) return pstEnv->pstScriptLargePool;
	else if (cScriptType == SCRIPT_HUGE) return pstEnv->pstScriptHugePool;
	else return NULL;
}

int ZoneScriptIndexCompare(const void *p1, const void *p2)
{
	ZoneScriptIndex *pstIndex1, *pstIndex2;

	pstIndex1 = (ZoneScriptIndex *)p1;
	pstIndex2 = (ZoneScriptIndex *)p2;
	return strcasecmp(pstIndex1->szScriptName, pstIndex2->szScriptName);
}

Script* find_script(ZONESVRENV* pstEnv, ZoneScriptIndex *pstIndex)
{
	Script *pstScp;
	TMEMPOOL *pstPool;
	int iPos, iEqu;
	size_t tSize;
	TMEMBLOCK *pstTmemBlock;
	ZoneObj *pstZoneObj = pstEnv->pstZoneObj;

	if (pstIndex->stScriptIdx.cScriptType) {
		if (!(pstPool = script_type_block(pstEnv, pstIndex->stScriptIdx.cScriptType))) return NULL;// 根据脚本类型查找内存池
		pstTmemBlock = TMEMPOOL_GET_PTR(pstPool, pstIndex->stScriptIdx.iIndex);
		if (pstTmemBlock->fValid)
		{
			pstScp = (Script *)pstTmemBlock->szData;
			if (0 == strcasecmp(pstScp->stHead.sName, pstIndex->szScriptName)) return pstScp;
		}
	}

	iPos = my_bsearch(pstIndex, pstZoneObj->astScriptIndex, pstZoneObj->iSIdxNum, sizeof(ZoneScriptIndex), &iEqu, ZoneScriptIndexCompare);
	if (!iEqu) return NULL;
	pstIndex->stScriptIdx.iIndex = pstZoneObj->astScriptIndex[iPos].stScriptIdx.iIndex;
	pstIndex->stScriptIdx.cScriptType = pstZoneObj->astScriptIndex[iPos].stScriptIdx.cScriptType;
	if (!(pstPool = script_type_block(pstEnv, pstIndex->stScriptIdx.cScriptType))) return NULL;
	pstTmemBlock = TMEMPOOL_GET_PTR(pstPool, pstIndex->stScriptIdx.iIndex);
	if (pstTmemBlock->fValid)
	{
		pstScp = (Script *)pstTmemBlock->szData;
		if (0 == strcasecmp(pstScp->stHead.sName, pstIndex->szScriptName)) return pstScp;
	}

	tSize = pstZoneObj->iSIdxNum;
	my_bdelete(pstIndex, pstZoneObj->astScriptIndex, &tSize, sizeof(ZoneScriptIndex), ZoneScriptIndexCompare);
	pstZoneObj->iSIdxNum = tSize;

	return NULL;
}

TMEMPOOL *script_size_block(ZONESVRENV* pstEnv, int iSize, char *pType)
{
	if (iSize > SCRIPT_HUGE_SIZE) {
		return NULL;
	}
	else if (iSize > SCRIPT_LARGE_SIZE) {
		*pType = SCRIPT_HUGE;
		return pstEnv->pstScriptHugePool;
	} else if (iSize > SCRIPT_MIDDLE_SIZE) {
		*pType = SCRIPT_LARGE;
		return pstEnv->pstScriptLargePool;
	} else if (iSize > SCRIPT_MINI_SIZE) {
		*pType = SCRIPT_MIDDLE;
		return pstEnv->pstScriptMiddlePool;
	} else {
		*pType = SCRIPT_MINI;
		return pstEnv->pstScriptMiniPool;
	}
}

Script* free_one_script(ZONESVRENV* pstEnv, ZoneScriptIndex *pstIndex)
{
	Script *pstScp;
	size_t tSize;
	TMEMPOOL *pstPool;
	ZoneObj *pstZoneObj = pstEnv->pstZoneObj;

	pstScp = find_script(pstEnv, pstIndex);
	if (!pstScp) return NULL;
	pstPool = script_type_block(pstEnv, pstIndex->stScriptIdx.cScriptType);
	if (!pstPool) return NULL;

	tmempool_free(pstPool, pstIndex->stScriptIdx.iIndex);

	tSize = pstZoneObj->iSIdxNum;
	my_bdelete(pstIndex, pstZoneObj->astScriptIndex, &tSize, sizeof(ZoneScriptIndex), ZoneScriptIndexCompare);
	pstZoneObj->iSIdxNum = tSize;

	return 0;
}

int load_one_script(ZONESVRENV* pstEnv, char *pszScriptName)
{
	int iSize=0;
	Script stScp, *pstScp=NULL;
	ZoneScriptIndex stIndex;
	char cType;
	TMEMPOOL *pstPool;
	struct stat stStat;
	int iAddSize;
	int iIndex = 0;
	char sName[32];
	size_t tSize;
	ZoneObj *pstZoneObj = pstEnv->pstZoneObj;

	if	(stat(pszScriptName, &stStat) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "stat file %s fail", pszScriptName);
		return -1;
	}

	STRNCPY(sName, basename(pszScriptName), sizeof(sName));

	memset(&stIndex, 0, sizeof(stIndex));
	STRNCPY(stIndex.szScriptName, sName, sizeof(stIndex.szScriptName));
	pstScp = find_script(pstEnv, &stIndex);
	
	if (pstScp && stStat.st_mtime == pstScp->stHead.tFileTime)
	{
		return 0;
	}

	iSize = load_script(pszScriptName, stScp.sData, sizeof(stScp.sData));
	if (iSize <= 0) {
		tlog_error(pstEnv->pstLogCat, 0, 0, "Can not load script file %s ret=%d", pszScriptName, iSize);
		return -1;
	}
	iSize	+=	SCRIPT_HEAD_SIZE;
	STRNCPY(stScp.stHead.sName, sName, sizeof(stScp.stHead.sName));
	stScp.stHead.tFileTime = stStat.st_mtime;

	if (pstScp)
	{
		if (NULL == (pstPool = script_size_block(pstEnv, iSize, &cType)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "script_size_block fail, size=%d", iSize);
			return -1;
		}

		if (stIndex.stScriptIdx.cScriptType == cType)
		{
			memcpy(pstScp, &stScp, iSize);
			return 0;
		}
		else
		{
			free_one_script(pstEnv, &stIndex);
		}
	}

	//重新分配空间
	iAddSize = iSize;
	while(1)
	{
		pstPool = script_size_block(pstEnv, iAddSize, &stIndex.stScriptIdx.cScriptType);
		if (NULL == pstPool)
		{
			return -1;
		}
		
		iIndex = tmempool_alloc(pstPool);
		if (0 <= iIndex)
		{
			pstScp = tmempool_get(pstPool, iIndex);
			if (!pstScp)
			{
				return -1;
			}
			break;
		}

		if (stIndex.stScriptIdx.cScriptType == SCRIPT_MINI) iAddSize = SCRIPT_MIDDLE_SIZE;
		else if (stIndex.stScriptIdx.cScriptType == SCRIPT_MIDDLE) iAddSize = SCRIPT_LARGE_SIZE;
		else if (stIndex.stScriptIdx.cScriptType == SCRIPT_LARGE) iAddSize = SCRIPT_HUGE_SIZE;
		else 
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "load %s script no room, size=%d", pszScriptName, iSize);
			return -1;
		}
	}

	stIndex.stScriptIdx.iIndex = iIndex;
	if(pstScp)
	{
		memcpy(pstScp, &stScp, iSize);
	}

	tSize = pstZoneObj->iSIdxNum;
	my_binsert(&stIndex, pstZoneObj->astScriptIndex, &tSize, sizeof(ZoneScriptIndex), 1, ZoneScriptIndexCompare);
	pstZoneObj->iSIdxNum = tSize;
		
	return 0;
}

int load_script_dir(ZONESVRENV* pstEnv)
{
	char *pCh;
	char  szScriptPath[256];
	DIR		*pstDir = NULL;
	struct  dirent  *pstDirent = NULL;

	pstDir = opendir("../cfg/script");
	if (NULL == pstDir)
	{
		printf("opens directory script dir fail\n");
		return -1;
	}

	while ((pstDirent = readdir(pstDir)) != NULL)
	{
		
		if ((strcmp((const char*)pstDirent->d_name, ".")  ==0) ||
					(strcmp((const char*)pstDirent->d_name, "..") == 0)) 
		{
			continue;
		}
		
		/*查找以.mac结尾的文件*/
		pCh = strrchr((const char*)pstDirent->d_name, '.');
		if (NULL == pCh)
		{
			continue;
		}
		pCh++;
		if (0 != strcmp(pCh, "mac"))
		{
			continue;
		}

		snprintf(szScriptPath, sizeof(szScriptPath), "../cfg/script/%s", pstDirent->d_name);
		szScriptPath[sizeof(szScriptPath) - 1] = 0;
		if (0 > load_one_script(pstEnv, szScriptPath))
		{
			printf("load_one_script %s fail\n", pstDirent->d_name);
			return -1;
		}
	}

	closedir(pstDir);
	
	return 0;
}

void init_script_conter()
{
	int i;
	ScriptFunc *pstSF;

	for (i=0; i<(int)(sizeof(ScriptList)/sizeof(ScriptList[0])); i++)
	{
		pstSF	=	ScriptList + i;
		pstSF->iExecNum 	=	0;
		pstSF->iExecTime	=	0;
	}

	return;
}

void *get_script_location_byind(ScriptIndex *pstIndex, char *pszScriptName)
{
	Script *pstScp;
	TMEMPOOL *pstPool;
	TMEMBLOCK *pstMemBlock;

	pstPool = script_type_block(stSEnv.pstEnv, pstIndex->cScriptType);
	if (!pstPool)
	{
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "get script %s location fail, ScriptType %d", pszScriptName, pstIndex->cScriptType);
		return NULL;
	}

	pstMemBlock = TMEMPOOL_GET_PTR(pstPool, pstIndex->iIndex);
	if (!pstMemBlock->fValid)
	{
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "get script %s location fail memblock null", pszScriptName);
		return NULL;
	}
	
	pstScp = (Script *)pstMemBlock->szData;
	if (strcasecmp(pstScp->stHead.sName, pszScriptName))
	{
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "get script %s location fail head name %s", pszScriptName, pstScp->stHead.sName);
		return NULL;
	}

	return pstScp->sData;
}

ScriptIndex *get_script_ind(char *sName)
{
	ZoneScriptIndex  stZoneIndex, *pstZoneIndex;
	ZoneObj *pstZoneObj;
	
	pstZoneObj = stSEnv.pstEnv->pstZoneObj;
	STRNCPY(stZoneIndex.szScriptName, sName, sizeof(stZoneIndex.szScriptName));
	pstZoneIndex = bsearch(&stZoneIndex, pstZoneObj->astScriptIndex, pstZoneObj->iSIdxNum, sizeof(ZoneScriptIndex), ZoneScriptIndexCompare);

	if (NULL == pstZoneIndex) return NULL;

	return &pstZoneIndex->stScriptIdx;
}

void *get_script_location(char *sName)
{
	ScriptIndex *pstIndex; 
		
	pstIndex = get_script_ind(sName);
	if (!pstIndex) {
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "get script %s location fail", sName);
		return NULL;
	}

	return get_script_location_byind(pstIndex, sName);
}

int init_script_sys(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	UNUSED(pstAppCtx);
	
	memset(&stSEnv, 0, sizeof(stSEnv));
	
	stSEnv.pstEnv = pstEnv;
		
	if (init_script(pstEnv->pstZoneObj->sScriptSysVar, SCRIPT_SYSVAR_SIZE) <= 0) 
	{
		return -1;
	}

	set_script_decoder(my_script_decoder);
	load_script_dir(pstEnv);
	init_script_conter();
	
	return 0;
}

static int z_script_pushstack()
{
    int idx = stSEnv.iStackTop;
    int is_success = 0;

    if(idx < SCRIPT_CALLSTACK_SIZE)
    {
        stSEnv.astCallStack[idx].pstMonster = stSEnv.pstMonster;
	 stSEnv.astCallStack[idx].pstNpc = stSEnv.pstNpc;
	 stSEnv.astCallStack[idx].pstPlayer = stSEnv.pstPlayer;
	 stSEnv.astCallStack[idx].pstPworldInst= stSEnv.pstPworldInst;
	 stSEnv.astCallStack[idx].pstEvent = stSEnv.pstEvent;
		 stSEnv.astCallStack[idx].pstMapInst= stSEnv.pstMapinst;
		 stSEnv.astCallStack[idx].iMapInstFist = stSEnv.iMapInstFist;
		 memcpy(stSEnv.astCallStack[idx].szScript,stSEnv.szScript,sizeof(stSEnv.astCallStack[idx].szScript));
        stSEnv.iStackTop++;

        is_success = 1;
    }

    return is_success;
}


static int z_script_popstack()
{
    int idx = stSEnv.iStackTop;
    int is_success = 0;

    if(idx > 0)
    {
        idx = --stSEnv.iStackTop;

	 if (idx > 0)
	 {
	        stSEnv.pstMonster = stSEnv.astCallStack[idx-1].pstMonster;
		 stSEnv.pstNpc = stSEnv.astCallStack[idx-1].pstNpc;
		 stSEnv.pstPlayer = stSEnv.astCallStack[idx-1].pstPlayer;
		 stSEnv.pstPworldInst = stSEnv.astCallStack[idx-1].pstPworldInst;
		 stSEnv.pstEvent = stSEnv.astCallStack[idx-1].pstEvent;
		 stSEnv.pstMapinst = stSEnv.astCallStack[idx-1].pstMapInst;		 
		 stSEnv.iMapInstFist = stSEnv.astCallStack[idx-1].iMapInstFist;
		 //szScript是在z_exec_script中push之后赋的值，所以这里pop idx,而不是idx-1
		 memcpy(stSEnv.szScript,stSEnv.astCallStack[idx].szScript,sizeof(stSEnv.szScript));
	
	 }

        is_success = 1;
    }

	if (stSEnv.iStackTop <= 0)
	{
		//栈底pop后重置
		stSEnv.iMapInstFist = 0;
	}

    return is_success;
}

int z_exec_script(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, char *pszScript, ScriptIndex *pstIndex,  int iProc)
{
	int iRet;
	char szDebug[DLG_STR_LEN];

	if (!z_script_pushstack())
	{
		return -1;
	}
	
	STRNCPY(stSEnv.szScript, pszScript, sizeof(stSEnv.szScript));

	stSEnv.iReturnFlag = 0;
	stSEnv.iReturnValue = 0;
	stSEnv.iExParaType = EX_PARA_TYPE_NONE;
	stSEnv.iExParaData = 0;
	stSEnv.iMoneyType = 0;

	
	if (pstPlayer)
	{
		if (FindDebugUin(pstPlayer->stRoleData.Uin) >= 0)
		{
			szDebug[0] = '\0';
			iRet = exec_script(pszScript, pstIndex, iProc, szDebug, &pstPlayer->stOnline.stTestEnv);
			z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SCRIPT_DEBUG, szDebug);
		}
		else
			iRet = exec_script(pszScript, pstIndex, iProc, NULL, &pstPlayer->stOnline.stTestEnv);
	}
	else
		iRet = exec_script(pszScript, pstIndex, iProc, NULL, NULL);

	z_script_popstack();

	return iRet;
}

int  npc_dialog()
{
	int iType;
	CSDLGS *pstDlgS = &stSEnv.stDlgPkg.Body.DlgS;

	if (NULL == stSEnv.pstPlayer)
	{
		return -1;
	}

	if (z_is_player_status_result(stSEnv.pstEnv, stSEnv.pstPlayer, STATUS_RESULT_NODIALOG))
	{
		return -1;
	}

	stSEnv.pstPlayer->stOnline.State |= CS_STAT_DIALOG;

	stSEnv.iLen = 0;
	iType = get_script_number(NULL);
	pstDlgS->Type = iType;

	switch (iType) {
		case DLG_TYPE_ANSWER:
			pstDlgS->Dlg.DlgAnswer.Next = 0;
			pstDlgS->Dlg.DlgAnswer.Answer[0] = 0;
			stSEnv.pstPlayer->stOnline.cDialogStat = DIALOG_STAT_ANSWER;
			break;
		case DLG_TYPE_OPT:
			pstDlgS->Dlg.DlgOpt.Sentence[0] = 0;
			pstDlgS->Dlg.DlgOpt.Total = 0;
			stSEnv.pstPlayer->stOnline.cDialogStat = DIALOG_STAT_OPT;
			break;
		case DLG_TYPE_LAST:
			pstDlgS->Dlg.DlgLast[0] = 0;
			break;
		case DLG_TYPE_COMMIT_OPT:
			pstDlgS->Dlg.DlgCommit.Answer[0] = 0;
			stSEnv.pstPlayer->stOnline.cDialogStat = DIALOG_STAT_COMMIT_OPT;
			stSEnv.pstPlayer->stOnline.stSData.stCommitOpt.iNum = 0;
			pstDlgS->Dlg.DlgCommit.CommitNum = 1;
			memset(stSEnv.pstPlayer->stOnline.stSData.stCommitOpt.astGrids,0,
					sizeof(stSEnv.pstPlayer->stOnline.stSData.stCommitOpt.astGrids));
			
			stSEnv.pstPlayer->stOnline.stSData.stCommitOpt.shProc = 0;
			break;
		default:
			return -1;
	}
	return 0;
}

int   npc_talk()
{
	CSDLGS *pstDlgS = &stSEnv.stDlgPkg.Body.DlgS;
		
	if (NULL == stSEnv.pstPlayer)
	{
		return -1;
	}
	
	if ( stSEnv.iLen > 0)
	{
		if (stSEnv.pstNpc)
			pstDlgS->NpcID = stSEnv.pstNpc->stMon.iID;
		else
			pstDlgS->NpcID = 0;

		if (DLG_TYPE_ANSWER == pstDlgS->Type && DIALOG_STAT_NEXT == stSEnv.pstPlayer->stOnline.cDialogStat)
		{
			pstDlgS->Dlg.DlgAnswer.Next = 1;
		}
		
		if(!stSEnv.pstNpc)
		{
			return -1;
		}

		if (stSEnv.pstNpc->stMon.cMoving)
		{
			z_mon_stop(stSEnv.pstEnv, &stSEnv.pstNpc->stMon);
		}
		stSEnv.pstNpc->stMon.tFreezeEnd = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec + 10;
		
		Z_CSHEAD_INIT(&stSEnv.stDlgPkg.Head, DLG_SVR);
		z_cltmsg_send(stSEnv.pstEnv, stSEnv.pstPlayer, &stSEnv.stDlgPkg);
	}

	stSEnv.iLen = 0;

	if (pstDlgS->Type == DLG_TYPE_LAST)
	{
		stSEnv.pstPlayer->stOnline.cDialogStat = DIALOG_STAT_BEGIN;
		stSEnv.pstPlayer->stOnline.State &= ~CS_STAT_DIALOG;
	}
	
	return 0;
}

int   set_dialog_style()
{
	int iType;
	CSDLGS *pstDlgS = &stSEnv.stDlgPkg.Body.DlgS;

	iType = get_script_number(NULL);

	if (iType < DLG_STYLE_NORMAL || iType > DLG_STYLE_COMMIT)
	{
		return -1;
	}
	pstDlgS->DlgStyleType = iType;

	return 0;
}

int IsSpecialChar(char c)
{
	switch (c) {
		case '=':
		case '|':
		case '\\':
		case '&':
			return 1;
		default:
			return 0;
	}
}

int get_script_input(char *sInput, char *sName, char *sVal, int iSize)
{
	char *p, *p0;
	int i, iNameLen;

	p0 = sInput;
	iNameLen = strlen(sName);
	if (iNameLen <= 0) {
		sVal[0] = 0;
		return 0;
	}
	while (1) {
		p = cistrstr(p0, sName);
		if (!p) return -1;
		p0 = p + strlen(sName);
		if ((p == sInput || IsSpecialChar(*(p - 1))) && *p0 == '=') break;
	}
	p = p0 + 1;
	for (i = 0; i < iSize - 1; i++, p++) {
		if (*p == '\\') p++;
		else if (IsSpecialChar(*p)) break;
		if (*p == 0) break;
		sVal[i] = *p;
	}
	sVal[i] = 0;
	return 0;
}

int get_career_str(int iCareer, char *pszCareerStr, int iLen)
{
	char *pszCareer;
	
	switch(iCareer)
	{
		case CAREER_WARRIOR:
			pszCareer = CAREER_NAME_WARRIOR;
			break;

		case CAREER_MAGICOR:
			pszCareer = CAREER_NAME_MAGICOR;
			break;

		case CAREER_GUNER:
			pszCareer = CAREER_NAME_GUNER;
			break;

		case CAREER_CHURCH:
			pszCareer = CAREER_NAME_CHURCH;
			break;

		case CAREER_WARRIOR_SINGLEHAND:
			pszCareer = CAREER_NAME_WARRIOR_SINGLEHAND;
			break;

		case CAREER_WARRIOR_DOUBLEHAND:
			pszCareer = CAREER_NAME_WARRIOR_DOUBLEHAND;
			break;

		case CAREER_MAGICOR_SINGLEHAND:
			pszCareer = CAREER_NAME_MAGICOR_SINGLEHAND;
			break;

		case CAREER_MAGICOR_DOUBLEHAND:
			pszCareer = CAREER_NAME_MAGICOR_DOUBLEHAND;
			break;

		case CAREER_GUNER_SINGLEHAND:
			pszCareer = CAREER_NAME_GUNER_SINGLEHAND;
			break;

		case CAREER_GUNER_DOUBLEHAND:
			pszCareer = CAREER_NAME_GUNER_DOUBLEHAND;
			break;

		case CAREER_CHURCH_SINGLEHAND:
			pszCareer = CAREER_NAME_CHURCH_SINGLEHAND;
			break;

		case CAREER_CHURCH_DOUBLEHAND:
			pszCareer = CAREER_NAME_CHURCH_DOUBLEHAND;
			break;

		default:
			pszCareer = NULL;
			break;
	}
	
	if (pszCareer)
	{
		STRNCPY(pszCareerStr, pszCareer, iLen);
	}
	else
	{
		pszCareerStr[0] = 0;
	}

	return 0;
}

void str2utf8(char *pVar, int iVarLen)
{
	char szCode[64];
	char szTmpVar[DLG_STR_LEN];
	size_t iTmpVar;
	char *pTmpVar;
	char szTmpUtf8[DLG_STR_LEN];
	size_t iTmpUtf8;
	char *pTmpUtf8;
	iconv_t cd;

	switch(stSEnv.pstEnv->pstConf->CountryVer)
	{
		case COUNTRY_VER_CHINA:
			return;
			break;
		case COUNTRY_VER_KOREA:
			strcpy(szCode, "EUC-KR");
			break;
		case COUNTRY_VER_CHINA_TAIWAN:
			strcpy(szCode, "BIG5");
			break;
		default:	
			return;
			break;
	}

	STRNCPY(szTmpVar, pVar, sizeof(szTmpVar));
	iTmpVar = strlen(szTmpVar);
	pTmpVar = &szTmpVar[0];
	
	memset(szTmpUtf8, 0, sizeof(szTmpUtf8));
	iTmpUtf8 = sizeof(szTmpUtf8);
	pTmpUtf8 = &szTmpUtf8[0];

	cd = iconv_open("UTF-8", szCode);
	if ((iconv_t)-1 == cd)
	{
		return;
	}

	if ( (size_t)-1 == iconv(cd, &pTmpVar, &iTmpVar, &pTmpUtf8, &iTmpUtf8))
	{
		return;
	}

	szTmpVar[sizeof(szTmpVar) - 1] = 0;

	STRNCPY(pVar, szTmpVar, iVarLen);
	
	return ;
}

int add_str_replace(char *sDst, int iSize, char *sSrc,int iNoNewLine)
{
	int i, iLen, iDst, iSrc;
	char *p, *p0, sVarName[MAX_VAR_LEN], sVar[DLG_STR_LEN];
	MONSTERDEF *pstMDef;

	iSrc = iDst = 0;
	while (iDst < iSize && sSrc[iSrc]) {
		if (sSrc[iSrc] == '$') {
			p0 = sSrc + iSrc + 1;
			p = strchr(p0, '$');
			if (p) {
				for (i = 0; i < MAX_VAR_LEN - 1 && p != p0 + i; i++) {
					sVarName[i] = p0[i];
				}
				sVarName[i] = 0;

				if (!cistrcmp(sVarName, "_Player"))
				{
					if (stSEnv.pstPlayer)
					{
						STRNCPY(sVar, stSEnv.pstPlayer->stRoleData.RoleName, sizeof(sVar));
						battle_rolename_remove_prefix(sVar);
						str2utf8(sVar, sizeof(sVar));
					}
					else
						sVar[0] = 0;
				}
				else if (!cistrcmp(sVarName, "_Npc")) {
					pstMDef = NULL;

					if (stSEnv.pstNpc)
					{
						pstMDef = z_get_mon_def(stSEnv.pstEnv, &stSEnv.pstNpc->stMon);
					}
					
					if (pstMDef) STRNCPY(sVar, pstMDef->Name, sizeof(sVar));
					else sVar[0] = 0;
				} else if (!cistrcmp(sVarName, "_Monster")) {
					pstMDef = NULL;

					if (stSEnv.pstMonster)
					{
						pstMDef = z_get_mon_def(stSEnv.pstEnv, stSEnv.pstMonster);
					}
					
					if (pstMDef) STRNCPY(sVar, pstMDef->Name, sizeof(sVar));
					else sVar[0] = 0;
				} else if (sVarName[0] == '@') {
					i = 0;
					get_script_variable(atoi(sVarName + 1), &i);
					sprintf(sVar, "%d", i);
				}
				else if (!cistrcmp(sVarName, "_MoneyType")) 
				{
					int iRet = money_type_to_string(stSEnv.iMoneyType,sVar,sizeof(sVar));
					if (iRet < 0)
					{
						sVar[0] = 0;
					}
				}
				else if(!cistrcmp(sVarName, "_ClanName")) 
				{
					ZoneClan *pstZoneClan = NULL;
					Player* pstPlayer = stSEnv.pstPlayer;

					if (stSEnv.pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
					{
						if (pstPlayer)
						{
							STRNCPY(sVar, pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Name, sizeof(sVar));
							str2utf8(sVar, sizeof(sVar));
						}
						else
						{
							sVar[0] = 0;
						}
					}
					else
					{

						if (pstPlayer)
						{
							pstZoneClan = player_get_clan(stSEnv.pstEnv,pstPlayer);
						}
						
						
						if (pstZoneClan)
						{
							STRNCPY(sVar, pstZoneClan->stClan.ClanProf.Name, sizeof(sVar));
							str2utf8(sVar, sizeof(sVar));
						}
						else
							sVar[0] = 0;
					}
				}
				else if(!cistrcmp(sVarName, "_WorldCityClanName")) 
				{
					WORLDCITY *pstWorldCity = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
					
					if (is_has_master_world_city(stSEnv.pstEnv))
					{
						STRNCPY(sVar, pstWorldCity->CityCore.ClanName, sizeof(sVar));
						str2utf8(sVar, sizeof(sVar));
					}
					else
						sVar[0] = 0;
				}
				else if(!cistrcmp(sVarName, "_LuckyMapName")) 
				{
					if (stSEnv.pstEvent)
					{
						LUCKYMAPDEF * pstDef = z_find_lucky_map_def(stSEnv.pstEnv,stSEnv.pstEvent->iLuckyRandID);
						
						if (pstDef)
						{
							STRNCPY(sVar, pstDef->MapName, sizeof(sVar));
						}
						else
							sVar[0] = 0;
					}
					else
					{
						sVar[0] = 0;
					}
				}
				else if(!cistrcmp(sVarName, "_SpyTargetClanName")) 
				{
					Player* pstPlayer = stSEnv.pstPlayer;
					ZoneClan *pstZoneClan = NULL;
					
					if (pstPlayer)
					{
						pstZoneClan = z_id_clan(stSEnv.pstEnv,pstPlayer->stRoleData.MiscInfo.DailyInfo.SpyInfo.TargetClanID);
					}					
					
					if (pstZoneClan)
					{
						STRNCPY(sVar, pstZoneClan->stClan.ClanProf.Name, sizeof(sVar));
						str2utf8(sVar, sizeof(sVar));
					}
					else
						sVar[0] = 0;
					
				}
				else if(!cistrcmp(sVarName, "_CityOwnerClanName")) 
				{
					ZoneClan *pstZoneClan = NULL;
					ZONEPWORLDINST *pstPworldInst = NULL;
					ZONEMAPINST *pstMapInst = NULL;
					ZONEIDX stMapIdx;

					Player* pstPlayer = stSEnv.pstPlayer;

					if (pstPlayer)
					{
						stMapIdx.iID = pstPlayer->stRoleData.Map;
						stMapIdx.iPos = pstPlayer->stOnline.iMapPos;
						pstMapInst = z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stMapIdx);
						if(pstMapInst && pstMapInst->iPworldInstID)
						{
							pstPworldInst =	z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID);
							if (pstPworldInst && pstPworldInst->iType == PWORLD_TYPE_CLAN)
							{
								pstZoneClan = z_id_clan(stSEnv.pstEnv,  pstPworldInst->stPworldTypeData.stClan.iID);
							}
						}		
						
					}
					
					if (pstZoneClan)
					{
						STRNCPY(sVar, pstZoneClan->stClan.ClanProf.Name, sizeof(sVar));
						str2utf8(sVar, sizeof(sVar));
					}
					else
						sVar[0] = 0;
				}
				else if (!cistrcmp(sVarName, "_GayShou"))
				{
					if (stSEnv.pstPlayer && is_gay_gong(stSEnv.pstEnv, stSEnv.pstPlayer))
					{
						STRNCPY(sVar, stSEnv.pstPlayer->stRoleData.MiscInfo.GayInfo.GayRoleName, sizeof(sVar));
						str2utf8(sVar, sizeof(sVar));
					}
					else
						sVar[0] = 0;
				}
				else if(!cistrcmp(sVarName, "_Career"))
				{
					Player* pstPlayer = stSEnv.pstPlayer;

					if (pstPlayer)
					{
						get_career_str(pstPlayer->stRoleData.Career, sVar, sizeof(sVar));
					}
					else
					{
						sVar[0] = 0;
					}
				}
				else get_script_input(stSEnv.pstDlgC->Dlg.Input, sVarName, sVar, sizeof(sVar));
				
				iLen = strlen(sVar);
				if (iLen + iDst >= iSize) iLen = iSize - iDst - 1;
				strncpy(sDst + iDst, sVar, iLen);
				iDst += iLen;
				iSrc += p - p0 + 2;
				continue;
			}
		}
		sDst[iDst] = sSrc[iSrc];
		iSrc++;
		iDst++;
	}
	if (iDst < iSize) {
		if (iNoNewLine)
		{
			sDst[iDst] = 0;
		}
		else
		{
			sDst[iDst] = '\n';
		}
		iDst++;
	}
	sDst[iDst] = 0;
	return iDst;
}

int add_talk_str( char * szStr )
{
	int iLen;
	CSDLGS *pstDlgS = &stSEnv.stDlgPkg.Body.DlgS;
	
	switch(pstDlgS->Type )
	{
		case DLG_TYPE_ANSWER:
			iLen = add_str_replace( pstDlgS->Dlg.DlgAnswer.Answer + stSEnv.iLen,
					                          sizeof(pstDlgS->Dlg.DlgAnswer.Answer) - stSEnv.iLen - 1, szStr,0 );
			break;
		case DLG_TYPE_OPT:
			iLen = add_str_replace( pstDlgS->Dlg.DlgOpt.Sentence + stSEnv.iLen,
					                          sizeof(pstDlgS->Dlg.DlgOpt.Sentence) - stSEnv.iLen - 1, szStr,0 );
			break;

		case DLG_TYPE_LAST:
			iLen = add_str_replace( pstDlgS->Dlg.DlgLast + stSEnv.iLen,
					                          sizeof(pstDlgS->Dlg.DlgLast) - stSEnv.iLen - 1, szStr,0 );
			break;

		case DLG_TYPE_COMMIT_OPT:
			iLen = add_str_replace(pstDlgS->Dlg.DlgCommit.Answer + stSEnv.iLen,
					                          sizeof(pstDlgS->Dlg.DlgCommit.Answer) - stSEnv.iLen - 1, szStr,0 );
			break;

		default:
			return -1;
	}

	stSEnv.iLen += iLen;
	return 0;
}	

int   npc_addtalk()
{
	char *p;
	p = get_script_string();
	
	return add_talk_str( p );
}

int   npc_option()
{
	int iProc;
	char *p;
	DLGOPTS *pstCSDlgOpt = &stSEnv.stDlgPkg.Body.DlgS.Dlg.DlgOpt;
	DialogOpt *pstDlgOpt;

	if (!stSEnv.pstPlayer)
	{
		return -1;
	}
	
	if (pstCSDlgOpt->Total >= MAX_DLG_OPT) return 0;
	pstDlgOpt = &stSEnv.pstPlayer->stOnline.stSData.stOpt;
	
	iProc = get_script_number(NULL);
	p = get_script_string();

	add_str_replace(pstCSDlgOpt->OneOpt[pstCSDlgOpt->Total].Opt, sizeof(pstCSDlgOpt->OneOpt[pstCSDlgOpt->Total].Opt), p,0);

	//STRNCPY(pstCSDlgOpt->OneOpt[pstCSDlgOpt->Total].Opt, p, sizeof(pstCSDlgOpt->OneOpt[0].Opt));
	
	pstDlgOpt->ashProc[pstCSDlgOpt->Total] = iProc;
	pstCSDlgOpt->Total++;
	pstDlgOpt->shNum =  pstCSDlgOpt->Total;

	return 0;
}

int   npc_next()
{
	int iNext;

	if (!stSEnv.pstPlayer)
	{
		return -1;
	}

	iNext = get_script_number(NULL);
	stSEnv.pstPlayer->stOnline.stSData.shNext = iNext;
	stSEnv.pstPlayer->stOnline.cDialogStat = DIALOG_STAT_NEXT;
	
	return 0;
}

int   npc_commit_opt()
{
	int iProc;

	if (!stSEnv.pstPlayer)
	{
		return -1;
	}

	iProc = get_script_number(NULL);
	stSEnv.pstPlayer->stOnline.stSData.stCommitOpt.shProc = iProc;
	stSEnv.pstPlayer->stOnline.cDialogStat = DIALOG_STAT_COMMIT_OPT;
	
	return 0;
}

int   npc_stash()
{
	if (!stSEnv.pstPlayer || !stSEnv.pstNpc)
	{
		return -1;
	}

	stSEnv.pstPlayer->stOnline.cUIType = UI_TYPE_STASH;

	return open_stash(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, stSEnv.pstNpc->stMon.iID);
}



// 打开ui面板 
int npc_ui()
{
	int iType;
	
	if (!stSEnv.pstPlayer || !stSEnv.pstNpc)
	{
		return -1;
	}

	iType = get_script_number(NULL);
	
	return player_ui(stSEnv.pstEnv, stSEnv.pstPlayer, stSEnv.pstNpc,iType);
}


int npc_pworld_ui()
{
	int iDiffPworldID;
	
	if (!stSEnv.pstPlayer || !stSEnv.pstNpc)
	{
		return -1;
	}

	iDiffPworldID = get_script_number(NULL);

	return open_diff_pworld_ui(stSEnv.pstEnv, stSEnv.pstPlayer, iDiffPworldID, stSEnv.pstNpc->stMon.iID);
}

// 脚本加一个称号,并且应用该称号
int s_player_set_desig()
{
	int iID;
	int iVarIdx;
	int iRet = 0;

	iVarIdx = get_script_number(NULL);
	iID = get_script_number(NULL);
	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	iRet = player_designation_add_in(stSEnv.pstEnv, stSEnv.pstPlayer, iID);
	if (	iRet == ADD_DESIG_NO_DEF || 
		iRet == ADD_DESIG_NUM_LIMIT)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	if (player_designation_set(stSEnv.pstEnv, stSEnv.pstPlayer, iID) < 0)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}
	set_script_variable(iVarIdx, 0);
	return 0;
}

// 计算点与点之间的距离
int s_get_point_distance()
{
	int x1,x2,y1,y2,iRet,iDist;

	x1 = get_script_number(NULL);
	y1 = get_script_number(NULL);
	
	x2 = get_script_number(NULL);
	y2 = get_script_number(NULL);
	
	iRet = get_script_number(NULL);

	iDist = hypot((abs(x1 - x2)), abs(y1 - y2));

	set_script_variable(iRet, iDist);	
	return 0;
}


// 获取角色地图（Defid和Instid）和坐标
int s_get_player_mapinfo()
{
	int iVarIdx;			// 返回值
	int iDefID;			// 地图定义ID
	int iInstID;			// 地图实例ID
	RESPOS stPos;		// 坐标
	


	iVarIdx = get_script_number(NULL);
	iDefID = get_script_number(NULL);
	iInstID = get_script_number(NULL);
	stPos.X = get_script_number(NULL);
	stPos.Y = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	set_script_variable(iVarIdx, 0);
	set_script_variable(iDefID, GET_MAPID(stSEnv.pstPlayer->stRoleData.Map));
	set_script_variable(iInstID, stSEnv.pstPlayer->stRoleData.Map);
	set_script_variable(stPos.X, stSEnv.pstPlayer->stRoleData.Pos.X);
	set_script_variable(stPos.Y, stSEnv.pstPlayer->stRoleData.Pos.Y);

	if (stSEnv.pstPlayer->stOnline.cMoving)
		z_player_move_curr(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, 0);
	
	if (stSEnv.pstPlayer)
	{
		set_script_variable(iVarIdx, 0);
		set_script_variable(iDefID, GET_MAPID(stSEnv.pstPlayer->stRoleData.Map));
		set_script_variable(iInstID, stSEnv.pstPlayer->stRoleData.Map);
		set_script_variable(stPos.X, stSEnv.pstPlayer->stRoleData.Pos.X);
		set_script_variable(stPos.Y, stSEnv.pstPlayer->stRoleData.Pos.Y);
	}
	return 0;
}

// 获取角色安全锁信息
int s_get_player_safelocak_info()
{
	int iRet;
	int iUnLock; // 是否解锁

	iRet = get_script_number(NULL);
	iUnLock = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	if (player_check_safelock(stSEnv.pstEnv, stSEnv.pstPlayer) < 0)
	{
		//z_sys_strf(stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE2, "请先认证背包安全锁密码");	
		set_script_variable(iUnLock, 0);
	}
	else
	{
		set_script_variable(iUnLock, 1);
	}

	set_script_variable(iRet, 0);
	return 0;
}

// 角色是否在顶号保护时间内
int s_get_player_grab_info()
{
	int iRet;
	int iGrab; 

	iRet = get_script_number(NULL);
	iGrab = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	if ( player_check_grabsafe(stSEnv.pstEnv, stSEnv.pstPlayer) < 0)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE2, 
				ZONEERR_SCRIPT302,
				GRAB_SAFE_TIME);
		set_script_variable(iGrab, 1);  // 顶号状态
	}
	else
	{
		set_script_variable(iGrab, 0);	
	}
	
	set_script_variable(iRet, 0);
	return 0;
}


// 获取某个地图坐标的某个半径范围内的随机坐标
int s_get_map_randpoint()
{
	MapIndex *pstMapIdx;
	ZONEMAPINST* pstMapInst;
	RESPOS stPos;
	RESPOS stPosIn;
	int iRadius;			// 半径
	int iVarIdx;			// 返回值
	RESPOS stPosOut;
	
	stPosIn.X = get_script_number(NULL);
	stPosIn.Y = get_script_number(NULL);
	iRadius = get_script_number(NULL);
	
	iVarIdx = get_script_number(NULL);
	stPosOut.X = get_script_number(NULL);
	stPosOut.Y = get_script_number(NULL);
	
	
	pstMapInst = s_get_mapinst();
	if (!pstMapInst)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}
	
	z_get_ran_pos(stSEnv.pstEnv, pstMapInst, pstMapIdx, &stPosIn, &stPosIn, iRadius, 0, &stPos) ;

	set_script_variable(iVarIdx, 0);
	set_script_variable(stPosOut.X, stPos.X);
	set_script_variable(stPosOut.Y, stPos.Y);
	
	return 0;
}

// 获取角色技能信息
int s_get_player_skillinfo()
{
	int iVarIdx;
	int iSkillID;
	int iLevel;
	int i;
	int iRet = -1;

	ROLEDATA *pstRoleData = &stSEnv.pstPlayer->stRoleData;


	iVarIdx = get_script_number(NULL);
	iSkillID = get_script_number(NULL);
	iLevel =  get_script_number(NULL);
	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	for (i=0; i<pstRoleData->SkillInfo.SkillNum; i++)
	{
		if (iSkillID == pstRoleData->SkillInfo.SkillList[i].SkillID)
		{
			iRet = pstRoleData->SkillInfo.SkillList[i].SkillLevel;
			break;
		}
	}

	if (iRet <= 0)
	{
		set_script_variable(iVarIdx, -1);
	}
	else
	{
		set_script_variable(iVarIdx, 0);
		set_script_variable(iLevel, iRet);
	}
	return 0;
}


// 获取角色数质信息
int s_get_player_diathesisinfo()
{
	int iVarIdx;
	int iDiathesis;
	int iLevel;
	int i;
	int iRet = -1;

	ROLEDATA *pstRoleData = &stSEnv.pstPlayer->stRoleData;


	iVarIdx = get_script_number(NULL);
	iDiathesis = get_script_number(NULL);
	iLevel =  get_script_number(NULL);
	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	for (i=0; i<(int)pstRoleData->DiathesisInfo.DiathesisNum; i++)
	{
		if (iDiathesis == pstRoleData->DiathesisInfo.DiathesisList[i].DiathesisID)
		{
			iRet = pstRoleData->DiathesisInfo.DiathesisList[i].DiathesisLevel;
			break;
		}
	}

	if (iRet <= 0)
	{
		set_script_variable(iVarIdx, -1);
	}
	else
	{
		set_script_variable(iVarIdx, 0);
		set_script_variable(iLevel, iRet);
	}
	return 0;
}

int  s_get_player_base_att()
{
	char cGender;
	unsigned char cLevel;
	char cCareer;   
	int iVarIdx;
	
	if (!stSEnv.pstPlayer)
	{
		iVarIdx = get_script_number(NULL);
		set_script_variable(iVarIdx, -1);
		return 0;
	}
	
	cGender = stSEnv.pstPlayer->stRoleData.Gender;
	cLevel = stSEnv.pstPlayer->stRoleData.Level;
	cCareer = stSEnv.pstPlayer->stRoleData.Career;

	iVarIdx = get_script_number(NULL);
	set_script_variable(iVarIdx, 0);

	iVarIdx = get_script_number(NULL);
	set_script_variable(iVarIdx, cGender);

	iVarIdx = get_script_number(NULL);
	set_script_variable(iVarIdx, cLevel);

	iVarIdx = get_script_number(NULL);
	set_script_variable(iVarIdx, cCareer);

	return 0;
}

int  s_get_player_explimit()
{
	Player *pstPlayer = NULL;
	//unsigned int uiExpLimit;
	tdr_longlong iExp;
	int iVarIdx;
	int iMemID;

	
	iVarIdx = get_script_number(NULL);
	iMemID = get_script_number(NULL);

	if (iMemID > 0)
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	}
	else
	{
		pstPlayer = stSEnv.pstPlayer;
	}
	
	if (!pstPlayer)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	iExp = get_script_number(NULL);
	if (iExp <=0)
	{
		set_script_variable(iVarIdx, -1);
		return 0;	
	}
	else
	{
		set_script_variable(iVarIdx, 0);
		iVarIdx = get_script_number(NULL);
		if (z_player_addexp_test(stSEnv.pstEnv,stSEnv.pstPlayer, &iExp) == 0)
		{
			set_script_variable(iVarIdx, 1);
		}
		else
		{
			set_script_variable(iVarIdx, 0);
		}
	}
	return 0;
}


int   s_enter_pworld()
{
	int iVarIdx;
	int iRet;
	int iPworldID;
	PWORLDSELECT stPworldSelect;

	memset(&stPworldSelect, 0, sizeof(stPworldSelect));

	iPworldID = get_script_number(NULL);
	stPworldSelect.PworldGrade = get_script_number(NULL);
	iVarIdx = get_script_number(NULL);

	stPworldSelect.LevelType = PWORLD_LEVEL_AVG;
	stPworldSelect.PworldDifficulty = 0;
	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	iRet = z_player_enter_pworld(stSEnv.pstEnv, stSEnv.pstPlayer, iPworldID, &stPworldSelect, 0);
	set_script_variable(iVarIdx, iRet);

	return 0;
}

int   s_set_weather()
{
	int iType;
	int iVarIdx;
	int iRet;
	
	iType = get_script_number(NULL);
	iVarIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iVarIdx, -1);
	}

	
	iRet = weather_set(stSEnv.pstEnv, stSEnv.pstPlayer, iType);
	set_script_variable(iVarIdx, iRet);
	return 0;
}

int s_reset_pworld()
{
	int iPworldID;

	iPworldID = get_script_number(NULL);
	
	if (stSEnv.pstPlayer)
	{
		player_close_pworld(stSEnv.pstEnv, stSEnv.pstPlayer, iPworldID);
	}
	
	return 0;
}

int s_get_pworld_info()
{
	ZONEMAPINST* pstMapInst;
	int iVarIdx;
	int iLevel;
	int iTime;
	ZONEPWORLDINST *pstPworldInst= NULL;

	iVarIdx = get_script_number(NULL);
	iLevel = get_script_number(NULL);
	iTime = get_script_number(NULL);

	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );

	if (NULL == pstPworldInst)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	set_script_variable(iVarIdx, 0);
	set_script_variable(iLevel, pstPworldInst->stPworldselect.PworldGrade);

	if (pstPworldInst->tEnd > 0)
	{
		if (pstPworldInst->tEnd > stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			set_script_variable(iTime, pstPworldInst->tEnd-stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec);
		}
		else
		{
			set_script_variable(iTime, 0);
		}
	}
	else
	{
		set_script_variable(iTime, -1);
	}

	return 0;
}


int s_mon_set_hp()
{
	int iHp;
	int iMemID;
	int iVarIdx;
	Monster *pstMon = NULL;
	int iOldHp;
	MONSTERDEF *pstMonDef;
	ZoneAni stAtker;
				
		

	iHp = get_script_number(NULL);
	iMemID = get_script_number(NULL);
	iVarIdx = get_script_number(NULL);

	if (iHp==0 || iMemID==0)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if (!pstMon)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	pstMonDef = z_get_mon_def(stSEnv.pstEnv, pstMon);
	if (!pstMonDef)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	if(iHp < 0 && stSEnv.pstPlayer)
	{
		mon_booty_first_atker(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMon, stSEnv.pstPlayer);
	}
	
	iOldHp = pstMon->iHP;
	
	pstMon->iHP += iHp;
	if (pstMon->iHP > pstMon->iMaxHP)
	{
		pstMon->iHP = pstMon->iMaxHP;
	}

	if (pstMon->iHP < 0)
	{
		pstMon->iHP = 0;
	}

	if (iOldHp != pstMon->iHP)
	{
		mon_hp_action(stSEnv.pstEnv, pstMon, iOldHp, 0);
	}
	
	if (pstMon->iHP == 0)
	{
		memset(&stAtker, 0, sizeof(stAtker));
		stAtker.iType = OBJ_UNKNOW;
		z_mon_die(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMon, pstMonDef, &stAtker, 1, 1);
		set_script_variable(iVarIdx, 0);
		return 0;
	}

	set_script_variable(iVarIdx, 0);
	return 0;
}

int s_get_mon_patrolIdx()
{
	int iMemID;
	int iPatrolIdx;
	int iVarIdx;
	Monster *pstMon = NULL;
	PATROLDEF *pstPatrolDef;
	int i;
	
	iMemID = get_script_number(NULL);
	iPatrolIdx = get_script_number(NULL);
	iVarIdx = get_script_number(NULL);

	if (iMemID==0)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if (!pstMon || pstMon->iPatrolID <= 0)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	pstPatrolDef = z_find_patroldef(stSEnv.pstEnv,pstMon->iPatrolID,&pstMon->iPatrolIdx);
	if (!pstPatrolDef)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	for (i=0; i<MAX_PATROL_POINT; i++)
	{
		if (pstPatrolDef->PatrolPos[i].X == 0 || pstPatrolDef->PatrolPos[i].Y == 0)
		{
			break;
		}

		if (pstPatrolDef->PatrolPos[i].X == pstMon->stCurrPos.X &&
			pstPatrolDef->PatrolPos[i].Y== pstMon->stCurrPos.Y)
		{
			set_script_variable(iVarIdx, 0);
			set_script_variable(iPatrolIdx, i+1);
			return 0;
		}
	}
	
	set_script_variable(iVarIdx, -1);
	return 0;
}

int s_call_mon()
{
	int iMonNum;
	int iMonID;
	int iRadius;
	int iLifeTime;
	int iX,iY;
	int iVarIdx;
	int iRet;
	ZONEMAPINST *pstMapInst ;
	ZONEIDX stIdx;

	iMonNum = get_script_number(NULL);
	iMonID = get_script_number(NULL);
	iRadius =   get_script_number(NULL);
	iLifeTime = get_script_number(NULL);
	iX  = get_script_number(NULL);
	iY =   get_script_number(NULL);
	iVarIdx =   get_script_number(NULL);

	if (NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	stIdx.iID = stSEnv.pstPlayer->stRoleData.Map;
	stIdx.iPos = stSEnv.pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	iRet = item_call_mon(stSEnv.pstEnv,stSEnv.pstPlayer, pstMapInst, iMonNum,iMonID, iRadius, iLifeTime, iX, iY,0, 0, 0);
	
	set_script_variable(iVarIdx, iRet);
	
	return 0;
}

int s_compel_close_pworld()
{
	ZONEPWORLDINST *pstPworldInst= NULL;
	ZONEMAPINST* pstMapInst;
	int iVarIdx;

	iVarIdx = get_script_number(NULL);

	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );

	if (NULL == pstPworldInst)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	if (z_in_destroy_pworld_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPworldInst) < 0)
	{
		set_script_variable(iVarIdx, -1);
	}
	else
	{
		set_script_variable(iVarIdx, 0);
	}
	
	return 0;
}

int s_add_item_bat_inpak()
{
	int iItemID[ITEM_BAT_NUMMAX];
	int iNumIdx[ITEM_BAT_NUMMAX];
	int iNum ;
	int iFlag;
	int iRes;
	int iRetIdx;
	int i;
	ROLEGRID stRoleGrid[ITEM_BAT_NUMMAX], stDummy;

	iNum = get_script_number(NULL);

	if ( iNum <= 0 && iNum > ITEM_BAT_NUMMAX ) 
	{
		return -1;
	}
	
	for (i =0 ; i< ITEM_BAT_NUMMAX; i++)
	{
		iItemID[i] = get_script_number(NULL);
		iNumIdx[i] = get_script_number(NULL);
	}
	
	iFlag = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	for (i =0 ; i< iNum; i++)
	{
		//超过全局限制就不掉落了
		if(0 > world_item_limit_check(stSEnv.pstEnv, iItemID[i]))
		{
			set_script_variable(iRetIdx, -2);
			return 0;
		}
		
		if (0 == item_create(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stRoleGrid[i], iItemID[i] ) )
		{
				stRoleGrid[i].GridData.RoleItem.Num = iNumIdx[i];
		}
		else 
		{
				set_script_variable(iRetIdx, -1);
				return 0;
		}
	}

	//0 能加就加 1必须全部加进去   2 仅仅测试是否能全部放进去
	if(iFlag > 0)
	{
		iRes=package_add_test(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, &stRoleGrid[0], iNum,0);

		if (iRes == -1)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		//只测试是否可以加进去
		if(iFlag == 2)
		{
			set_script_variable(iRetIdx, 0);
			return 0;
		}
	}

	for (i =0 ; i< iNum; i++)
	{
		//op_log
		memcpy(&stDummy, &stRoleGrid[i], sizeof(stDummy));
		
		package_add(stSEnv.pstEnv->pstAppCtx , stSEnv.pstEnv , stSEnv.pstPlayer, &stRoleGrid[i], 1,0, IMPITEM_OPTYPE_SCRIPT);
		
		//记录全局限制的物品产出
		if(iNumIdx[i] - stRoleGrid[i].GridData.RoleItem.Num > 0)
		{
			world_item_limit_add(stSEnv.pstEnv, 
				stRoleGrid[i].GridData.RoleItem.DefID, 
				iNumIdx[i] - stRoleGrid[i].GridData.RoleItem.Num);

			stDummy.GridData.RoleItem.Num = iNumIdx[i]-stRoleGrid[i].GridData.RoleItem.Num;
			z_role_pickitem_oplog(stSEnv.pstEnv,stSEnv.pstPlayer, &stDummy, 1, OP_SCRIPT);
		}
	}
	
	set_script_variable(iRetIdx, 0);
	
	return 0;
	
	
}

int s_add_item_num_inpak_money()
{
	int iItemID;
	int iNumIdx;
	int iFlag;
	int iRes;
	int iRetIdx;
	ROLEGRID stRoleGrid, stDummy;
	int iMoney;
	
	iItemID = get_script_number(NULL);
	iNumIdx = get_script_number(NULL);
	iFlag = get_script_number(NULL);
	iMoney = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	//超过全局限制就不掉落了
	if(0 > world_item_limit_check(stSEnv.pstEnv, iItemID))
	{
		if (iMoney > 0)
		{
			package_add_money(stSEnv.pstEnv, stSEnv.pstPlayer, ATTR_ID_BULL,iMoney);
			z_money_create_oplog(stSEnv.pstEnv, stSEnv.pstPlayer, MONEY_OPTYPE_SCRIPT_ADD, iItemID,  iMoney, 0, "世界道具限制补偿");
		}
		
		set_script_variable(iRetIdx, -2);
		return 0;
	}
	
	if (0 == item_create(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stRoleGrid, iItemID))
	{
		stRoleGrid.GridData.RoleItem.Num = iNumIdx;

		//0 能加就加 1必须全部加进去   2 仅仅测试是否能全部放进去
		if(iFlag > 0)
		{
			iRes=package_add_test(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, &stRoleGrid,  1,0);
			if (iRes == -1)
			{
				z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,stSEnv.pstPlayer,SYS_MIDDLE2, ZONEERR_NOSPACE);
				set_script_variable(iRetIdx, iRes);
				return 0;
			}

			//只测试是否可以加进去
			if(iFlag == 2)
			{
				set_script_variable(iRetIdx, 0);
				return 0;
			}
		}	

		//op_log
		memcpy(&stDummy, &stRoleGrid, sizeof(stDummy));
		
		if( 0 > package_add(stSEnv.pstEnv->pstAppCtx , stSEnv.pstEnv , stSEnv.pstPlayer, &stRoleGrid, 1,0, IMPITEM_OPTYPE_SCRIPT))
		{
			z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,stSEnv.pstPlayer,SYS_MIDDLE2, ZONEERR_NOSPACE);
		}

		//记录全局限制的物品产出
		if(iNumIdx - stRoleGrid.GridData.RoleItem.Num > 0)
		{
			world_item_limit_add(stSEnv.pstEnv, 
				stRoleGrid.GridData.RoleItem.DefID, 
				iNumIdx-stRoleGrid.GridData.RoleItem.Num);

			stDummy.GridData.RoleItem.Num = iNumIdx-stRoleGrid.GridData.RoleItem.Num;
			z_role_pickitem_oplog(stSEnv.pstEnv,stSEnv.pstPlayer, &stDummy, 1, OP_SCRIPT);
		}

		set_script_variable(iRetIdx, 0);
		
		return 0;

	}
	
	set_script_variable(iRetIdx, -1);
	return 0;
}


int s_add_item_num_inpak()
{
	int iItemID;
	int iNumIdx;
	int iFlag;
	int iRes;
	int iRetIdx;
	ROLEGRID stRoleGrid, stDummy;
	
	iItemID = get_script_number(NULL);
	iNumIdx = get_script_number(NULL);
	iFlag = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	//超过全局限制就不掉落了
	if(0 > world_item_limit_check(stSEnv.pstEnv, iItemID))
	{
		set_script_variable(iRetIdx, -2);
		return 0;
	}
	
	if (0 == item_create(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stRoleGrid, iItemID))
	{
		stRoleGrid.GridData.RoleItem.Num = iNumIdx;

		//0 能加就加 1必须全部加进去   2 仅仅测试是否能全部放进去
		if(iFlag > 0)
		{
			iRes=package_add_test(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, &stRoleGrid,  1,0);
			if (iRes == -1)
			{
				z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,stSEnv.pstPlayer,SYS_MIDDLE2, ZONEERR_NOSPACE);
				set_script_variable(iRetIdx, iRes);
				return 0;
			}

			//只测试是否可以加进去
			if(iFlag == 2)
			{
				set_script_variable(iRetIdx, 0);
				return 0;
			}
		}	

		//op_log
		memcpy(&stDummy, &stRoleGrid, sizeof(stDummy));
		
		if( 0 > package_add(stSEnv.pstEnv->pstAppCtx , stSEnv.pstEnv , stSEnv.pstPlayer, &stRoleGrid, 1,0, IMPITEM_OPTYPE_SCRIPT))
		{
			z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,stSEnv.pstPlayer,SYS_MIDDLE2, ZONEERR_NOSPACE);
		}

		//记录全局限制的物品产出
		if(iNumIdx - stRoleGrid.GridData.RoleItem.Num > 0)
		{
			world_item_limit_add(stSEnv.pstEnv, 
				stRoleGrid.GridData.RoleItem.DefID, 
				iNumIdx-stRoleGrid.GridData.RoleItem.Num);

			stDummy.GridData.RoleItem.Num = iNumIdx-stRoleGrid.GridData.RoleItem.Num;
			z_role_pickitem_oplog(stSEnv.pstEnv,stSEnv.pstPlayer, &stDummy, 1, OP_SCRIPT);
		}

		set_script_variable(iRetIdx, 0);
		
		return 0;

	}
	
	set_script_variable(iRetIdx, -1);
	return 0;
}

int s_get_item_num_inpak()
{
	int iItemID;
	int iRetIdx;
	int iNumIdx;
	int iNum;

	iItemID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iNumIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iNum = z_get_pak_item_num(stSEnv.pstEnv, &stSEnv.pstPlayer->stRoleData.Package, iItemID, -1,0,PACKAGE_DEC_NONE);

	set_script_variable(iRetIdx, 0);
	set_script_variable(iNumIdx, iNum);

	return 0;
}

int s_dec_item_num_inpak()
{
	int iItemID;
	int iNum;
	int iRetIdx;
	int iRet = -1;
	ITEMDEC stDec;

	iItemID = get_script_number(NULL);
	iNum = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (iNum > z_get_pak_item_num(stSEnv.pstEnv, &stSEnv.pstPlayer->stRoleData.Package, iItemID, -1,0,PACKAGE_DEC_NONE))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	stDec.ID = iItemID;
	stDec.Num = iNum;
	stDec.BindNum = 0;
	
	if( stSEnv.pstRoleItem != NULL && stSEnv.pstRoleItem->DefID == iItemID )
	{
		// 严格判断下,如果是藏宝图,要扣指定格子的物品
		ITEMDEF *pstItemDef;
		ROLEITEM *pstRoleItem;
		int iPos;
		if (0 > package_get_item(stSEnv.pstEnv, stSEnv.pstPlayer, stSEnv.stCsPackageUse.SrcListType, 
							stSEnv.stCsPackageUse.SrcGridIdx, &pstItemDef, &pstRoleItem, &iPos))
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
		iRet = package_dec(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, iItemID, iNum, 
			stSEnv.stCsPackageUse.SrcListType, &iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_SCRIPT);
		if (iRet > 0)
			stDec.BindNum = iRet;
	}
	else 
	{
		iRet = package_dec(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, iItemID, iNum, -1,
				NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_SCRIPT);
		if (iRet > 0)
			stDec.BindNum = iRet;
	}

	if (iRet >= 0)
	{
		z_role_item_dec_oplog(stSEnv.pstEnv,stSEnv.pstPlayer, &stDec,OP_ITEM_DEC_SCRIPT);
	}

	set_script_variable(iRetIdx, 0);

	return 0;
}

int s_add_exp()
{
	int iPlayerID;
	tdr_longlong iExp;
	int iRetIdx;
	PlayerChagAttrVal stAttr;
	Player *pstPlayer;

	iPlayerID = get_script_number(NULL);
	iExp = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (0 == iPlayerID)
	{
		pstPlayer = stSEnv.pstPlayer;
	}
	else
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iPlayerID);
	}
	
	if (NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	z_player_addexp_test(stSEnv.pstEnv, pstPlayer, &iExp);

	if (iExp <= 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	stAttr.AttrList[0].AttrID = ATTR_ID_EXP;
	stAttr.AttrList[0].Value.Exp= iExp;
	stAttr.AttrList[0].nFlag = 0;
	stAttr.AttrNum = 1;
	
	z_attr_chg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, &stAttr, 1);

	set_script_variable(iRetIdx, 0);

	z_script_add_exp_oplog(stSEnv.pstEnv,pstPlayer,iExp, stSEnv.szScript);
	return 0;
}

int s_add_money()
{
	int iPlayerID;
	int iMoney;
	int iRetIdx;
	int iMoneyType;
	int iRes;
	Player *pstPlayer;
	char szMsg[512];

	iPlayerID = get_script_number(NULL);
	iMoney = get_script_number(NULL);
	iMoneyType = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (0 == iPlayerID)
	{
		pstPlayer = stSEnv.pstPlayer;
	}
	else
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iPlayerID);
	}
	
	if (NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (0 >  package_add_money_test(stSEnv.pstEnv, pstPlayer, iMoneyType, iMoney))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	snprintf(szMsg, sizeof(szMsg), "脚本加:%s", stSEnv.szScript);
	szMsg[sizeof(szMsg) - 1] = 0;

	iRes = package_add_money(stSEnv.pstEnv, pstPlayer, iMoneyType, iMoney);

	if (iMoneyType == ATTR_ID_BINDGODCOIN)
	{
		z_bindgold_create_oplog(stSEnv.pstEnv, pstPlayer, BINDGOLD_OPTYPE_SCRIPT_ADD, 0, iMoney, szMsg);
	}
	else if (iMoneyType == ATTR_ID_MONEY)
	{
		z_money_create_oplog(stSEnv.pstEnv, pstPlayer, MONEY_OPTYPE_SCRIPT_ADD, 0, 0, iMoney, szMsg);
	}
	else if (iMoneyType == ATTR_ID_BULL)
	{
		z_money_create_oplog(stSEnv.pstEnv, pstPlayer, MONEY_OPTYPE_SCRIPT_ADD, 0, iMoney,0,  szMsg);
	}
		
	set_script_variable(iRetIdx, iRes);
	
	return 0;
}

int s_is_captain()
{
	int iIsCaptain;
	int iRetIdx;
	ZoneTeam *pstTeam;
	TEAMMEMBER * pstCaptain;

	iIsCaptain = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iIsCaptain, -1);
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstTeam = player_team_get(stSEnv.pstEnv, stSEnv.pstPlayer);
	if(pstTeam == NULL)
	{
		set_script_variable(iIsCaptain, -1);
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstCaptain = player_team_get_captain(pstTeam);
	if(pstCaptain == NULL)
	{
		set_script_variable(iIsCaptain, -1);
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(strcmp(pstCaptain->MemberName, stSEnv.pstPlayer->stRoleData.RoleName) != 0)
	{
		set_script_variable(iIsCaptain, -1);
		set_script_variable(iRetIdx, -1);
	}
	else
	{
		set_script_variable(iIsCaptain, 0);
		set_script_variable(iRetIdx, 0);
	}
	return 0;
}


int s_bct_get_curr_layer()
{
	int iRetIdx;
	int iLayer;

	iLayer = get_script_number(NULL); 
	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		goto error;
	}

	if (stSEnv.pstPlayer->stRoleData.MiscInfo.BCTInfo.CurrLayer < 1 ||
		stSEnv.pstPlayer->stRoleData.MiscInfo.BCTInfo.CurrLayer > BAICENG_CENG_NUM)
	{
		goto error;
	}

	set_script_variable(iLayer, stSEnv.pstPlayer->stRoleData.MiscInfo.BCTInfo.CurrLayer);
	set_script_variable(iRetIdx, 0);
	return 0;
error:			
	set_script_variable(iRetIdx, -1);
	return 0;	
}


static int bct_clear_map_obj(ZONESVRENV* pstEnv, ZONEIDX *pstMapInstIdx)
{
	int i, k, iID;
	MapObj* pstMap;
	ZONEMAPINST* pstMapInst;
	AREADYN* pstAreaDyn;
	Monster *pstMon;
	Npc *pstNpc;
	//DropItem* pstDropItem;
	char cType;
	int iIndex;
	
	pstMap	=	pstEnv->pstMapObj;

	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, pstMapInstIdx);
	if( !pstMapInst )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_find_map_inst %d map inst fail", pstMapInstIdx->iID);
		return -1;
	}

	for (i=0; i<pstMapInst->iAreaDyn; i++)
	{
		if (0 > pstMapInst->aiAreaDyn[i])
		{
			continue;
		}
		
		pstAreaDyn	=	(AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, pstMapInst->aiAreaDyn[i]);
		if (NULL == pstAreaDyn)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_get %d area dyn fail", pstMapInst->aiAreaDyn[i]);
			continue;
		}

		for(k=pstAreaDyn->iAnimate-1; k>=0; k--)
		{
			iID	=	pstAreaDyn->aiAnimate[k];
			iIndex	=	z_id_indextype(pstEnv->pstAppCtx, pstEnv, iID, &cType);

			if( -1 == iIndex )
			{
				continue;
			}

			switch(cType)
			{
			case OBJ_MONSTER:
				pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, iID);
				if (NULL == pstMon)
				{
					continue;
				}

				if (pstMon->cStat == MONSTER_DIE)
				{
					continue;
				}

				if(is_mon_machine(pstMon))
				{
					mon_machine_back(pstEnv, pstMon);
					continue;
				}
				
				z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
				z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);
				break;
			case OBJ_NPC:
				pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, iID);
				if (!pstNpc)
				{		
					continue;
				}

				z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, &pstNpc->stMon);
				z_free_monster(pstEnv->pstAppCtx, pstEnv, &pstNpc->stMon);
				break;
			}
			
			
			
		}

		/*while(pstAreaDyn->iInanimate>0)
		{
			iID	=	pstAreaDyn->aiInanimate[pstAreaDyn->iInanimate-1];

			iIndex	=	z_id_indextype(pstEnv->pstAppCtx, pstEnv, iID, &cType);

			if( iIndex<0 )
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "inanimal id=%d iIndex=%d", iID, iIndex);
				pstAreaDyn->iInanimate--;
				continue;
			}

			switch(cType)
			{
			case OBJ_ITEM:
				pstDropItem = (DropItem *)tmempool_get(pstEnv->pstItemPool, iIndex);
				if (pstDropItem)
				{
					//这里面会pstAreaDyn->iInanimate--
					z_item_clean(pstEnv, pstDropItem);
					continue;
				}
				else
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "item iIndex=%d", iIndex);	
				}
				break;
			default:
				break;
			}

			pstAreaDyn->iInanimate--;
		}*/
	}

	return 0;
}

static int get_pworld_mon_hp(ZONESVRENV* pstEnv,ZONEIDX *pstMapInstIdx)
{
	int i, k, iID;
	MapObj* pstMap;
	ZONEMAPINST* pstMapInst;
	AREADYN* pstAreaDyn;
	Monster *pstMon;
	int iHp = 0;
	
	pstMap	=	pstEnv->pstMapObj;

	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, pstMapInstIdx);
	if( !pstMapInst )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_find_map_inst %d map inst fail", pstMapInstIdx->iID);
		return 0;
	}

	for (i=0; i<pstMapInst->iAreaDyn; i++)
	{
		if (0 > pstMapInst->aiAreaDyn[i])
		{
			continue;
		}
		
		pstAreaDyn	=	(AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, pstMapInst->aiAreaDyn[i]);
		if (NULL == pstAreaDyn)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tmempool_get %d area dyn fail", pstMapInst->aiAreaDyn[i]);
			continue;
		}

		for(k=pstAreaDyn->iAnimate-1; k>=0; k--)
		{
			iID	=	pstAreaDyn->aiAnimate[k];
			pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, iID);
			if (NULL == pstMon)
			{
				continue;
			}

			if (pstMon->cStat == MONSTER_DIE)
			{
				continue;
			}

			if (MON_SUB_TYPE_TMP > pstMon->bSubType)
			{
				iHp += pstMon->iHP;
			}
		}
	}

	return iHp;
}

int get_pworld_mon_total_hp(ZONESVRENV* pstEnv)
{
	int iHpTotal = 0;
	int i;
	ZONEMAPINST *pstMapInst;
	ZONEPWORLDINST *pstPworldInst;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		return 0;
	}
	
	for (i=0; i<pstPworldInst->iMapInst; i++)
	{
		iHpTotal += get_pworld_mon_hp(pstEnv, &pstPworldInst->astMapInstIdx[i]);
	}


	return iHpTotal;
}

int clean_pworld_mon(ZONESVRENV* pstEnv)
{
	int i;
	ZONEMAPINST *pstMapInst;
	ZONEPWORLDINST *pstPworldInst;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		return 0;
	}
	
	for (i=0; i<pstPworldInst->iMapInst; i++)
	{
		bct_clear_map_obj(pstEnv, &pstPworldInst->astMapInstIdx[i]);
	}

	// pass 之后不倒计时了

	// iDownTm=0  	不会进入计时结束脚本了
	pstPworldInst->iDownTm = 0;
	pstPworldInst->iMonHpTotal = 0;
	return 0;
}

int s_bct_pass()
{
	int iRetIdx;
	ROLEBCTINFO *pstBctInfo;
	BCTDEF *pstBctDef;
	int iCurr;
	int iUseTime;
	GLOBALMISC *pstMisc;
	CSPKG stPak;
	CSBCTRES*pstBctRes = &stPak.Body.BctRes;
	int iIsUp = 0;
	SINGLELAYER *pstSinglayers;
	ZONEPWORLDINST *pstPworldInst = stSEnv.pstPworldInst;
	Player *pstPlayer;
	int iExp;
	int iUseTimeMs;
	SINGLELAYER *pstSingleLayer;
	BAICENGHISTORYENTRY *pstHistory;
	//short iCurrPass;
	//short iCountMoney;
	
	iRetIdx = get_script_number(NULL);
	if (!stSEnv.pstPlayer)
	{
		if (pstPworldInst && pstPworldInst->iPlayer > 0)
		{
			stSEnv.pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,pstPworldInst->aiPlayerID[0]);
		}
		
	}

	if (!stSEnv.pstPlayer)
	{
		goto error;
	}

	pstPlayer = stSEnv.pstPlayer;

	pstBctInfo = &stSEnv.pstPlayer->stRoleData.MiscInfo.BCTInfo;
	if (pstBctInfo->CurrLayer <= 0 || pstBctInfo->CurrLayer > BAICENG_CENG_NUM)
	{
		goto error;
	}
	
	iCurr = pstBctInfo->CurrLayer;
	pstBctDef = z_find_bctdef(stSEnv.pstEnv, pstBctInfo->CurrLayer);
	if (!pstBctDef)
	{
		goto error;
	}

	if (package_add_money_test(stSEnv.pstEnv, stSEnv.pstPlayer, ATTR_ID_BULL,pstBctDef->AwardMoney) == 0 )
	{
		package_add_money(stSEnv.pstEnv, stSEnv.pstPlayer, ATTR_ID_BULL,pstBctDef->AwardMoney);
		z_money_create_oplog(stSEnv.pstEnv, stSEnv.pstPlayer, MONEY_OPTYPE_BCT, 0, pstBctDef->AwardMoney, 0, "大地之心");
	}

	iExp = pstBctDef->AwardExp;
	if (pstPlayer->stRoleData.Level < (iCurr+15))
	{
		iExp = pstBctDef->AwardExp*2;
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, SYS_MIDDLE2,ZONEERR_SCRIPT306);
	}
	player_add_exp(stSEnv.pstEnv, stSEnv.pstPlayer, iExp, 0);

	// 通过的层数加1
	stSEnv.pstPlayer->stRoleData.MiscInfo.BCTInfo.PassLayer ++;
	stSEnv.pstPlayer->stRoleData.MiscInfo.BCTInfo.PassExp += iExp;
	stSEnv.pstPlayer->stRoleData.MiscInfo.BCTInfo.PassMoney += pstBctDef->AwardMoney;
	stSEnv.pstPlayer->stRoleData.MiscInfo.BCTInfo.MaxPassLayer = iCurr;

	// 记录自己最好成绩
	iUseTime = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec - pstBctInfo->LayerStartTime;
	iUseTime -= 2 ;
	if (iUseTime < 0)
		iUseTime = 0;
	
	iUseTimeMs = RAND1(100); 			// 欺骗消费者

	if (iCurr > pstPlayer->stRoleData.MiscInfo.BCTHistoryTop )
	{
		if (iCurr - pstPlayer->stRoleData.MiscInfo.BCTHistoryTop > 1)
		{
			goto error;
		}
		iIsUp = 1;
		pstSingleLayer = &stSEnv.pstPlayer->stRoleData.MiscInfo.BCTSingleLayers[pstPlayer->stRoleData.MiscInfo.BCTHistoryTop];
	}
	else
	{
		pstSingleLayer = &stSEnv.pstPlayer->stRoleData.MiscInfo.BCTSingleLayers[iCurr-1];
		if (iUseTime < pstSingleLayer->Top)
		{
			iIsUp = 1;
		}
		else if (iUseTime == pstSingleLayer->Top &&
				iUseTimeMs < pstSingleLayer->TopMs)
		{
			iIsUp = 1;
		}
	}

	// 记录历史最高层 ,只要是当前层过了, 历史记录指向下一层
	if (pstPlayer->stRoleData.MiscInfo.BCTHistoryTop < pstPlayer->stRoleData.MiscInfo.BCTInfo.CurrLayer)
	{
		pstPlayer->stRoleData.MiscInfo.BCTHistoryTop = pstPlayer->stRoleData.MiscInfo.BCTInfo.CurrLayer;

		//更新排行榜个人信息
		range_combat_script_set(stSEnv.pstEnv, pstPlayer, RANGE_MAIN_COMBAT, RANGE_MAIN_COMBAT_SUB_BCT, pstPworldInst);
	}
	
	/*pstSingleLayer = &stSEnv.pstPlayer->stRoleData.MiscInfo.BCTSingleLayers[iCurr-1];
	if (pstSingleLayer->Top <= 0) 		// 从来没有记录过的肯定更新
	{
		iIsUp = 1;
	}
	else
	{
		if (iUseTime < pstSingleLayer->Top)
		{
			iIsUp = 1;
		}
		else if (iUseTime == pstSingleLayer->Top &&
				iUseTimeMs < pstSingleLayer->TopMs)
		{
			iIsUp = 1;
		}
	}*/

	if (iIsUp)
	{
		pstSingleLayer->Top 		= iUseTime;
		pstSingleLayer->TopMs 	= iUseTimeMs;
		pstSingleLayer->RecordTm 	= stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	
	iIsUp = 0;
	// 记录全服最好成绩
	pstMisc = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc;
	pstHistory = &pstMisc->BaiCengHistory.BaiCengHistoryEntrys[iCurr-1];
	if (	pstHistory->Time <= 0) 
	{
		iIsUp = 1;
		z_sys_strf_broadcast(stSEnv.pstEnv,SYS_SHOW, 
			ZONEERR_BROAD341,
			stSEnv.pstPlayer->stRoleData.RoleName, iCurr );
	}
	else
	{
		if (iUseTime < pstHistory->Time)
		{
			iIsUp = 1;
		}
		else if (iUseTime == pstHistory->Time &&
			iUseTimeMs < pstHistory->TimeMs)
		{
			iIsUp = 1;
		}

		if (iIsUp)
		{
			z_sys_strf_broadcast(stSEnv.pstEnv,SYS_SHOW, 
			ZONEERR_BROAD342,
			stSEnv.pstPlayer->stRoleData.RoleName, iUseTime/60,iUseTime%60,iUseTimeMs, iCurr );
		}
	}

	if (iIsUp)
	{
		pstHistory->Time 			= iUseTime;
		pstHistory->TimeMs		= iUseTimeMs;
		pstHistory->CengIdx 		= iCurr;
		strncpy(pstHistory->Name, stSEnv.pstPlayer->stRoleData.RoleName,sizeof(pstHistory->Name));

		// 更新数据库
		stSEnv.pstEnv->pstGlobalObj->stGlobalObjInfo.iDirty = 2;
	}

	// 没必要广播给客户端
	//bct_layer_history_notify(stSEnv.pstEnv, stSEnv.pstPlayer, 
	//						stSEnv.pstPlayer->stRoleData.MiscInfo.BCTInfo.CurrLayer);
	

	
	
	set_script_variable(iRetIdx, 0);

	tmp_item_del(stSEnv.pstEnv, stSEnv.pstPlayer);

	// 先这么写着, 以后再清除本地图的
	clean_pworld_mon(stSEnv.pstEnv);

	pstSinglayers = stSEnv.pstPlayer->stRoleData.MiscInfo.BCTSingleLayers;
	pstSinglayers[iCurr-1].MonHpTotal = 100;

	// 通知客户端 pass了  不要倒计时了
	pstBctRes->Cmd = BCT_CMD_SVR_PASS_STOP_TIME;
	pstBctRes->SvrData.BctStopTm.Layer = iCurr;

	//完成大地之心通过XX层任务
	task_bct_layer_trace(stSEnv.pstEnv, stSEnv.pstPlayer, iCurr);
	
	Z_CSHEAD_INIT(&stPak.Head, BCT_SVR);
	z_cltmsg_send(stSEnv.pstEnv, stSEnv.pstPlayer, &stPak);
	return 0;
error:			
	set_script_variable(iRetIdx, -1);
	return 0;	
}

static int get_bct_shest_award_idx(BCTDEF *pstBctDef, int *pIdx)
{
	int i;
	int iRate = 0;
	int iRnd;

	for(i=0; i<pstBctDef->Num; i++)
	{
		iRate += pstBctDef->BCTItems[i].Rate;
	}

	if (iRate <=0 )return 0;
	iRnd = (int)(RAND1(iRate));

	iRate = 0;
	for (i=0; i<pstBctDef->Num; i++)
	{
		iRate += pstBctDef->BCTItems[i].Rate;
		if (iRnd < iRate)
		{
			*pIdx = i;
			break;
		}
	}
	return 0;
}

// 白层塔箱子奖励
 int s_bct_shest_award()
 {
	int iRetIdx;
	ROLEBCTINFO *pstBctInfo;
	BCTDEF *pstBctDef;
	//int iRnd = -1;
	ROLEGRID stRoleGrid;
	ITEMDEF *pstItemDef;
	int iRet;
	int iIdx = -1;
	int iDouble = 0;
	char szItemName[32];


	iRetIdx = get_script_number(NULL);
	if (!stSEnv.pstPlayer)
	{
		goto error;
	}

	pstBctInfo = &stSEnv.pstPlayer->stRoleData.MiscInfo.BCTInfo;
	if (pstBctInfo->CurrLayer <= 0 || pstBctInfo->CurrLayer > BAICENG_CENG_NUM)
	{
		goto error;
	}

	pstBctDef = z_find_bctdef(stSEnv.pstEnv, pstBctInfo->CurrLayer);
	if (!pstBctDef || pstBctDef->Num <= 0 || pstBctDef->Num >BCT_ITEM_NUM)
	{
		goto error;
	}

	if (stSEnv.pstPlayer->stRoleData.Level < (stSEnv.pstPlayer->stRoleData.MiscInfo.BCTInfo.CurrLayer+15))
	{
		iDouble = 1; // 双倍奖励
	}
	
	get_bct_shest_award_idx(pstBctDef, &iIdx);
	if (iIdx == -1)
		goto error;

	iRet = accord_player_item(stSEnv.pstEnv, stSEnv.pstPlayer, 
						pstBctDef->BCTItems[iIdx].ItemID, pstBctDef->BCTItems[iIdx].Num, 1,OP_BCT_CHEST,
						LJY_ACCORD_ITEM3,LJY_ACCORD_ITEM4,1);	
	if (iRet >= 0)
	{
		if (pstBctInfo->PassItemNum < MAX_PASS_ITEM_NUM)
		{
			pstBctInfo->PassItems[(int)pstBctInfo->PassItemNum].ItemID = pstBctDef->BCTItems[iIdx].ItemID;
			pstBctInfo->PassItems[(int)pstBctInfo->PassItemNum].Layer = pstBctInfo->CurrLayer;
			if (GRID_ITEM_ITEM == stRoleGrid.Type)
			{
				pstBctInfo->PassItems[(int)pstBctInfo->PassItemNum].Num = pstBctDef->BCTItems[iIdx].Num;
			}
			else
			{
				pstBctInfo->PassItems[(int)pstBctInfo->PassItemNum].Num = 1;
			}

			pstBctInfo->PassItemNum ++;
		}

		pstItemDef = z_find_itemdef(stSEnv.pstEnv, pstBctDef->BCTItems[iIdx].ItemID);
		if (pstItemDef && iRet == 0 && !iDouble)
		{
			z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE2,ZONEERR_SCRIPT301,pstItemDef->Name);
			
		}

		if (pstItemDef && iDouble)
		{
			strncpy(szItemName, pstItemDef->Name, 32);
		}
	}

	if (iDouble)
	{
		iIdx = -1;
		get_bct_shest_award_idx(pstBctDef, &iIdx);
		if (iIdx >= 0)
		{
			iRet = accord_player_item(stSEnv.pstEnv, stSEnv.pstPlayer, 
						pstBctDef->BCTItems[iIdx].ItemID, pstBctDef->BCTItems[iIdx].Num, 1,OP_BCT_CHEST,
						LJY_ACCORD_ITEM3, LJY_ACCORD_ITEM4,1);
			if (iRet >= 0)
			{
				if (pstBctInfo->PassItemNum < MAX_PASS_ITEM_NUM)
				{
					pstBctInfo->PassItems[(int)pstBctInfo->PassItemNum].ItemID = pstBctDef->BCTItems[iIdx].ItemID;
					pstBctInfo->PassItems[(int)pstBctInfo->PassItemNum].Layer = pstBctInfo->CurrLayer;
					if (GRID_ITEM_ITEM == stRoleGrid.Type)
					{
						pstBctInfo->PassItems[(int)pstBctInfo->PassItemNum].Num = pstBctDef->BCTItems[iIdx].Num;
					}
					else
					{
						pstBctInfo->PassItems[(int)pstBctInfo->PassItemNum].Num = 1;
					}

					pstBctInfo->PassItemNum ++;
				}

				pstItemDef = z_find_itemdef(stSEnv.pstEnv, pstBctDef->BCTItems[iIdx].ItemID);
				if (pstItemDef && iRet == 0 )
				{
					z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE2,
							ZONEERR_SCRIPT307,
							pstItemDef->Name,
							szItemName);
				}
			}
		}
	}

	clean_pworld_mon(stSEnv.pstEnv);
	set_script_variable(iRetIdx, 0);
	return 0;

error:			
	set_script_variable(iRetIdx, -1);
	return 0;	
 }

// 通知客户端continue或者exit
int s_bct_ui()
{
	int iRetIdx;
	int iType;
	CSPKG stPak;
	CSBCTRES*pstBctRes = &stPak.Body.BctRes;
	int iCurrHpTotal = 0;
	int iCurrLayer;
	ZONEMAPINST *pstMapInst;
	ZONEPWORLDINST *pstPworldInst;
	SINGLELAYER *pstSinglayers;
	int iHPRate;
	int i;
	Player *pstPlayer;
	
	iType = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		goto error;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if(NULL == pstPworldInst)
	{
		goto error;
	}

	for (i=pstPworldInst->iPlayer-1; i>=0; i--)
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPworldInst->aiPlayerID[i]);
		if (pstPlayer)
		{
			pstSinglayers = pstPlayer->stRoleData.MiscInfo.BCTSingleLayers;
			iCurrLayer = pstPlayer->stRoleData.MiscInfo.BCTInfo.CurrLayer;
			// 继续面板
			if (iType == 0)
			{
				pstBctRes->Cmd = BCT_CMD_SVR_PASS;
				pstBctRes->SvrData.BctPass.Layer = iCurrLayer;
				bct_layer_history_notify(stSEnv.pstEnv, stSEnv.pstPlayer, iCurrLayer);
			}
			// 结束面板
			else if (iType == 1)
			{
				iCurrHpTotal = get_pworld_mon_total_hp(stSEnv.pstEnv);
				pstBctRes->Cmd = BCT_CMD_SVR_FAIL;
				pstBctRes->SvrData.BctFail.Layer = iCurrLayer;
				pstBctRes->SvrData.BctFail.CurrMonHp = iCurrHpTotal;

				// 曾经闯关过了 怪物血量发0
				if (pstPlayer->stRoleData.MiscInfo.BCTHistoryTop >= iCurrLayer)
				{
					pstBctRes->SvrData.BctFail.HistoryMonHp = 100;
				}
				else
				{
					pstBctRes->SvrData.BctFail.HistoryMonHp = pstSinglayers[iCurrLayer-1].MonHpTotal;
				}
				
				pstBctRes->SvrData.BctFail.LayerMonHp = pstPworldInst->iMonHpTotal;

				
				if (iCurrHpTotal > 0 && pstPworldInst->iMonHpTotal > 0)
				{
					iHPRate = 100 -(int)(iCurrHpTotal*100.0/pstPworldInst->iMonHpTotal);
					
					if (pstSinglayers[iCurrLayer-1].MonHpTotal == 0 )
					{
						pstSinglayers[iCurrLayer-1].MonHpTotal = iHPRate;
					}
					else
					{
						if (iHPRate > pstSinglayers[iCurrLayer-1].MonHpTotal)
						{
							pstSinglayers[iCurrLayer-1].MonHpTotal = iHPRate;
						}
					}
				}
			}
			Z_CSHEAD_INIT(&stPak.Head, BCT_SVR);
			z_cltmsg_send(stSEnv.pstEnv, pstPlayer, &stPak);
		}
	}

	set_script_variable(iRetIdx, 0);
	return 0;

error:			
	set_script_variable(iRetIdx, -1);
	return 0;
}

// 白层塔刷怪
int s_bct_call_mon()
{
	BCTDEF *pstBctDef;
	int iRetIdx;
	ROLEBCTINFO *pstBctInfo;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;

	int iRadius;
	int iX,iY;
	int iMonNum;
	int iMonID;
	int iLifeTm;
	int iRet = -1;
	int i;
	
	iMonNum = get_script_number(NULL);
	iMonID = get_script_number(NULL);
	iLifeTm = get_script_number(NULL);
	iRadius =   get_script_number(NULL);
	iX  = get_script_number(NULL);
	iY =   get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	
	
	if (!stSEnv.pstPlayer)
	{
		goto _error;
	}

	if (iMonID <= 0)
	{
		goto _error;
	}
	
	pstBctInfo = &stSEnv.pstPlayer->stRoleData.MiscInfo.BCTInfo;
	if (pstBctInfo->CurrLayer <= 0 || pstBctInfo->CurrLayer > BAICENG_CENG_NUM)
	{
		goto _error;
	}
	
	 pstBctDef = z_find_bctdef(stSEnv.pstEnv, pstBctInfo->CurrLayer);
	 if (!pstBctDef)
	 {
	 	goto _error;
	 }

	 for (i=0; i<BCT_MON_TYPE_NUM; i++)
	{
		if (pstBctDef->BCTMon[i].MonID == iMonID)
			break;
	}

	if (i >= BCT_MON_TYPE_NUM)
	{
		goto _error;
	}

	stIdx.iID = stSEnv.pstPlayer->stRoleData.Map;
	stIdx.iPos = stSEnv.pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		goto _error;
	}

	iRet = item_call_mon(stSEnv.pstEnv, stSEnv.pstPlayer, pstMapInst, iMonNum, iMonID, iRadius, iLifeTm,iX,iY,0, 0, pstBctInfo->CurrLayer);

_final:
	
	set_script_variable(iRetIdx, iRet);
	return 0;
	
_error:			
	
	iRet = -1;
	goto _final;
	
	return 0;
}

int s_team_trans()
{
	int iMapID;
	RESPOS stPos;
	int iRetIdx;
	int iMapInstID;
	ZoneTeam *pstTeam;
	int i;
	ZONEMAPINST *pstZoneMapInst;
	ZONEPWORLDINST *pstZonePworldInst = NULL;
	Player *pstPlayer = NULL;

	iMapID = get_script_number(NULL);
	stPos.X = get_script_number(NULL);
	stPos.Y = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		TMEMBLOCK *pstMemBlock;
	
		pstZoneMapInst = s_get_mapinst();
		if (NULL == pstZoneMapInst)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
		if (0 != pstZoneMapInst->iPworldInstID)
		{
			pstZonePworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstZoneMapInst->iPworldInstID);
		}
		if (NULL == pstZonePworldInst)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		if (iMapID >= PWORLD_MAPID_BASE)
		{
			iMapInstID = (pstZoneMapInst->iMapInstID & 0xffff0000) | iMapID;
		}
		else
		{
			iMapInstID = iMapID;
		}
		
		if (pstZonePworldInst->iType != PWORLD_TYPE_TEAM)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		pstMemBlock = TMEMPOOL_GET_PTR(stSEnv.pstEnv->pstTeamPool, pstZonePworldInst->stPworldTypeData.stTeam.iTeamIdx);
		if (0 == pstMemBlock->fValid)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		pstTeam= (ZoneTeam *)pstMemBlock->szData;
		if (pstTeam->ullPworldWID != pstZonePworldInst->ullPworldWID)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
	}
	else
	{
		pstPlayer = stSEnv.pstPlayer;
		if (iMapID >= PWORLD_MAPID_BASE)
		{
			iMapInstID = (pstPlayer->stRoleData.Map & 0xffff0000) | iMapID;
		}
		else
		{
			iMapInstID = iMapID;
		}

		pstTeam = player_team_get(stSEnv.pstEnv, pstPlayer);
		if(pstTeam == NULL)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
	}

	for(i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		pstPlayer = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
		if(NULL != pstPlayer)
		{
			z_player_change_map(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, iMapInstID, &stPos, RAND1(180), 0);
		}
	}
	set_script_variable(iRetIdx, 0);
		
	return 0;
}

int s_new_mon()
{
	int iMonID;
	int iMonNum;
	RESPOS stPos, stCurrPos;
	int iRandRadius;
	int iPatrolId;
	char cRefresh;
	int iRetIdx;
	int iMonInstIDIdx;
	int i;
	Monster stMonster;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	Monster *pstFind;
	MONSTERDEF *pstMonDef;
	char szMsg[256];

	iMonID = get_script_number(NULL);
	iMonNum = get_script_number(NULL);
	stPos.X = get_script_number(NULL);
	stPos.Y = get_script_number(NULL);
	iRandRadius = get_script_number(NULL);
	iPatrolId = get_script_number(NULL);
	cRefresh = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iMonInstIDIdx = get_script_number(NULL);

	pstMapInst = s_get_mapinst();

	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0,  "MapInst NULL");
		return 0;
	}

	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		set_script_variable(iRetIdx, -1);
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0,  "pstMapIdx NULL");
		return 0;
	}

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.stInitPos = stPos;
	stMonster.cRefresh	= cRefresh;

	pstMonDef = z_get_mon_def(stSEnv.pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		set_script_variable(iRetIdx, -1);
		sprintf(szMsg, "pstMonDef NULL  %d", iMonID);
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0,  szMsg);
		return 0;
	}
	
	for (i=0; i<iMonNum; i++)
	{
		stMonster.bDirection = RAND1(180);
		stMonster.stInitPos = stCurrPos = stPos;

		if (iRandRadius > 0)
		{
			z_get_ran_pos(stSEnv.pstEnv, pstMapInst, pstMapIdx, &stMonster.stInitPos, &stMonster.stInitPos, iRandRadius, 0, &stCurrPos);
		}
		
		stMonster.stInitPos = stCurrPos;
		stMonster.stCurrPos = stCurrPos;

		pstFind =	z_new_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stMonster, pstMonDef);
	
		if( !pstFind )
		{
			set_script_variable(iRetIdx, -1);
			sprintf(szMsg, "NEW MON FAIL  %d", iMonID);
			tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0,  szMsg);
			return 0;
		}

		pstFind->iPatrolID = iPatrolId;

		pstFind->cFirstBorn = 1;
		if (0 > z_refresh_mon(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstFind,NULL))
		{
			tmempool_free(stSEnv.pstEnv->pstMonsterPool, pstFind->iMIndex);
			set_script_variable(iRetIdx, -1);
			sprintf(szMsg, "z_refresh_mon FAIL  %d", iMonID);
			tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0,  szMsg);
			return 0;
		}
		pstFind->cFirstBorn = 0;

		if (iPatrolId > 0)
		{
			pstFind->bMoveType = MOVE_TYPE_PATROL;
		}

		if (0 == i)
		{
			stSEnv.iMemID = pstFind->iID;
			set_script_variable(iMonInstIDIdx, pstFind->iID);
		}	
	}

	set_script_variable(iRetIdx, 0);
		
	return 0;
}


int s_new_mon_by_dir()
{
	int iMonID;
	int iMonNum;
	RESPOS stPos, stCurrPos;
	int iRandRadius;
	int iPatrolId;
	char cRefresh;
	int iRetIdx;
	int iMonInstIDIdx;
	int i;
	Monster stMonster;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	Monster *pstFind;
	MONSTERDEF *pstMonDef;
	int iDir;

	iMonID = get_script_number(NULL);
	iMonNum = get_script_number(NULL);
	stPos.X = get_script_number(NULL);
	stPos.Y = get_script_number(NULL);
	iRandRadius = get_script_number(NULL);
	iPatrolId = get_script_number(NULL);
	cRefresh = get_script_number(NULL);
	iDir = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iMonInstIDIdx = get_script_number(NULL);

	pstMapInst = s_get_mapinst();

	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.stInitPos = stPos;
	stMonster.cRefresh	= cRefresh;

	pstMonDef = z_get_mon_def(stSEnv.pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	for (i=0; i<iMonNum; i++)
	{
		if (iDir < 0)
		{
			stMonster.bDirection = RAND1(180);
		}
		else
		{
			stMonster.bDirection = iDir%180;
		}
		stMonster.stInitPos = stCurrPos = stPos;
		
		if (iRandRadius > 0)
		{
			z_get_ran_pos(stSEnv.pstEnv, pstMapInst, pstMapIdx, &stMonster.stInitPos, &stMonster.stInitPos, iRandRadius, 0, &stCurrPos);
		}
		
		stMonster.stInitPos = stCurrPos;
		stMonster.stCurrPos = stCurrPos;

		pstFind =	z_new_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stMonster, pstMonDef);
	
		if( !pstFind )
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		pstFind->iPatrolID = iPatrolId;

		pstFind->cFirstBorn = 1;
		if (0 > z_refresh_mon(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstFind,NULL))
		{
			tmempool_free(stSEnv.pstEnv->pstMonsterPool, pstFind->iMIndex);
			set_script_variable(iRetIdx, -1);
			return 0;
		}
		pstFind->cFirstBorn = 0;

		if (iPatrolId > 0)
		{
			pstFind->bMoveType = MOVE_TYPE_PATROL;
		}

		if (0 == i)
		{
			set_script_variable(iMonInstIDIdx, pstFind->iID);
		}
	}

	set_script_variable(iRetIdx, 0);
		
	return 0;
}

int s_get_mon_num()
{
	int iRetIdx;
	int iMonNumIdx;
	ZONEMAPINST *pstMapInst;
	
	iRetIdx = get_script_number(NULL);
	iMonNumIdx = get_script_number(NULL);

       pstMapInst = s_get_mapinst();

	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iMonNumIdx, pstMapInst->iMonsterCount);

	return 0;
}

int s_transmit()
{
	int iMapID;
	RESPOS stPos;
	int iRetIdx;
	int iRet;
	int iMapInstID;

	iMapID = get_script_number(NULL);
	stPos.X = get_script_number(NULL);
	stPos.Y = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (iMapID >= PWORLD_MAPID_BASE)
	{
		iMapInstID = (stSEnv.pstPlayer->stRoleData.Map & 0xffff0000) | iMapID;
	}
	else
	{
		iMapInstID = iMapID;
	}

	iRet = team_trans_check_one(stSEnv.pstEnv,stSEnv.pstPlayer);
	team_trans_check_tip(stSEnv.pstEnv, stSEnv.pstPlayer,1, iRet, NULL);
	if(0 > iRet)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	g_team_follow_flag = 4;
	iRet = z_player_change_map(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, iMapInstID, &stPos, RAND1(180), 0);
	g_team_follow_flag = 0;
	if (0 > iRet)
	{
		set_script_variable(iRetIdx, -1);
	}
	else
	{
		set_script_variable(iRetIdx, 0);
		//player_team_follow_chg_map(stSEnv.pstEnv,stSEnv.pstPlayer,iMapInstID, &stPos);
	}

	return 0;
}

static int transmit_camera_in(Player *pstPlayer, int iDirection)
{
	int iMapID;
	RESPOS stPos;
	int iCameraDir;
	int iRetIdx;
	int iRet;
	int iMapInstID;
	int iDir;

	iMapID = get_script_number(NULL);
	stPos.X = get_script_number(NULL);
	stPos.Y = get_script_number(NULL);
	iCameraDir = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (iMapID >= PWORLD_MAPID_BASE)
	{
		iMapInstID = (pstPlayer->stRoleData.Map & 0xffff0000) | iMapID;
	}
	else
	{
		iMapInstID = iMapID;
	}

	if(iDirection > 0)
	{
		iDir = iDirection;
	}
	else
	{
		iDir = RAND1(180);
	}

	iRet = z_player_change_map(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,
	                           pstPlayer, iMapInstID, &stPos, iDir,
	                           iCameraDir);
	if (0 > iRet)
	{
		set_script_variable(iRetIdx, -1);
	}
	else
	{
		set_script_variable(iRetIdx, 0);
	}

	return 0;
}

int s_player_transmit_camera()
{
	int iMemID = get_script_number(NULL);
	int iRet = 0;

	if(!iMemID)
	{
		iRet = transmit_camera_in(stSEnv.pstPlayer, 0);
	}
	else
	{
		iRet = transmit_camera_in(z_id_player(stSEnv.pstEnv->pstAppCtx,
			                                  stSEnv.pstEnv,
                                              iMemID), 0);
	}
	return iRet;
}

int s_transmit_camera()
{
	int iMemID = get_script_number(NULL);
	int iDir = get_script_number(NULL);
	int iRet = 0;

	if(!iMemID)
	{
		iRet = transmit_camera_in(stSEnv.pstPlayer, iDir);
	}
	else
	{
		iRet = transmit_camera_in(z_id_player(stSEnv.pstEnv->pstAppCtx,
			                                  stSEnv.pstEnv,
                                              iMemID), iDir);
	}
	
	return iRet;
	
	/*
	int iMapID;
	RESPOS stPos;
	int iCameraDir;
	int iRetIdx;
	int iRet;
	int iMapInstID;

	iMapID = get_script_number(NULL);
	stPos.X = get_script_number(NULL);
	stPos.Y = get_script_number(NULL);
	iCameraDir = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (iMapID >= PWORLD_MAPID_BASE)
	{
		iMapInstID = (stSEnv.pstPlayer->stRoleData.Map & 0xffff0000) | iMapID;
	}
	else
	{
		iMapInstID = iMapID;
	}

	iRet = z_player_change_map(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, iMapInstID, &stPos, RAND()%180, iCameraDir);
	if (0 > iRet)
	{
		set_script_variable(iRetIdx, -1);
	}
	else
	{
		set_script_variable(iRetIdx, 0);
	}

	return 0;
	*/
}

int s_player_transmit_rand()
{
	int iMapID;
	RESPOS stPos, stChgPos;
	int iRetIdx;
	int iRadius;
	int iRet;
	int iMapInstID;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;

	iMapID = get_script_number(NULL);
	stPos.X = get_script_number(NULL);
	stPos.Y = get_script_number(NULL);
	iRadius = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (iMapID >= PWORLD_MAPID_BASE)
	{
		iMapInstID = (stSEnv.pstPlayer->stRoleData.Map & 0xffff0000) | iMapID;
	}
	else
	{
		iMapInstID = iMapID;
	}

	pstMapInst = s_get_mapinst();
	
	if (NULL ==pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (0 == z_valid_pos(stSEnv.pstEnv->pstMapObj, iMapInstID, &pstMapInst->iMapPos, &stPos))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	z_get_ran_pos(stSEnv.pstEnv, pstMapInst, pstMapIdx, &stPos, &stPos, iRadius, 0, &stChgPos);

	iRet = z_player_change_map(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,
	                           stSEnv.pstPlayer, iMapInstID, &stChgPos, RAND1(180),
	                           0);
	if (0 > iRet)
	{
		set_script_variable(iRetIdx, -1);
	}
	else
	{
		set_script_variable(iRetIdx, 0);
	}

	return 0;
}

int s_msg_info()
{
	CSPKG *pstPkg = &stSEnv.stDlgPkg;
		
	if (!stSEnv.pstPlayer)
	{
		return 0;
	}

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	z_cltmsg_send(stSEnv.pstEnv, stSEnv.pstPlayer, pstPkg);
	
	return 0;
}

int s_map_msg_info()
{
	ZONEMAPINST *pstMapInst;
	CSPKG *pstPkg = &stSEnv.stDlgPkg;
	
	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);

	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		return 0;
	}

	map_broadcast(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMapInst, pstPkg);

	return 0;
}

int s_svr_msg_info()
{
	CSPKG *pstPkg = &stSEnv.stDlgPkg;
	
	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);

	z_cltmsg_broadcast(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPkg);

	return 0;
}


int s_get_pworld_var()
{
	int iIdx;
	int iBit;
	int iRetIdx;
	int iRetValIdx;
	ZONEPWORLDINST *pstPworldInst = NULL;
	ZONEMAPINST* pstMapInst;
	char cFlag;
	int iRet;

	iIdx = get_script_number(NULL);
	iBit = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iRetValIdx = get_script_number(NULL);

	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );

	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if ((iBit>=0 && iBit<=7) || 8==iBit || 16==iBit || 32==iBit)
	{
		
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if( iIdx<0 || iIdx>=PWORLD_VAR || (16==iBit && iIdx>=PWORLD_VAR-1) || (32==iBit && iIdx>=PWORLD_VAR-3))
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if( 8 == iBit )
	{
		iRet =	pstPworldInst->sScriptVar[iIdx];
	}
	else if (16 == iBit)
	{
		iRet		= *(short*)(pstPworldInst->sScriptVar+iIdx);
	}
	else if (32 == iBit)
	{
		iRet		= *(int*)(pstPworldInst->sScriptVar+iIdx);
	}
	else
	{
		cFlag	=	1 << iBit;

		if( pstPworldInst->sScriptVar[iIdx] & cFlag )
			iRet	=	1;
		else
			iRet	=	0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iRetValIdx, iRet);

	return 0;
}

int s_get_event_var()
{
	int iIdx;
	int iBit;
	int iRetIdx;
	int iRetValIdx;
	char cFlag;
	int iRet;

	iIdx = get_script_number(NULL);
	iBit = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iRetValIdx = get_script_number(NULL);

	if (stSEnv.pstEvent == NULL)
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if ((iBit>=0 && iBit<=7) || 8==iBit || 16==iBit || 32==iBit)
	{
		
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if( iIdx<0 || iIdx>=MAX_EVENT_SCRIPT_VAR || (16==iBit && iIdx>=MAX_EVENT_SCRIPT_VAR-1) || (32==iBit && iIdx>=MAX_EVENT_SCRIPT_VAR-3))
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if( 8 == iBit )
	{
		iRet =	stSEnv.pstEvent->EventScriptVar[iIdx];
	}
	else if (16 == iBit)
	{
		iRet		= *(short*)(stSEnv.pstEvent->EventScriptVar+iIdx);
	}
	else if (32 == iBit)
	{
		iRet		= *(int*)(stSEnv.pstEvent->EventScriptVar+iIdx);
	}
	else
	{
		cFlag	=	1 << iBit;

		if( stSEnv.pstEvent->EventScriptVar[iIdx] & cFlag )
			iRet	=	1;
		else
			iRet	=	0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iRetValIdx, iRet);

	return 0;
}


int s_get_event_var_by_id()
{
	int iID;
	int iIdx;
	int iBit;
	int iRetIdx;
	int iRetValIdx;
	char cFlag;
	int iRet;
	GameEventInst stEventInst, *pstEvent;

	iID = get_script_number(NULL);
	iIdx = get_script_number(NULL);
	iBit = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iRetValIdx = get_script_number(NULL);

	stEventInst.iEventID = iID;
	pstEvent = sht_find(stSEnv.pstEnv->pstShtEvent, &stEventInst, EventHashCmp, EventHashCode);
	if (NULL == pstEvent)
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if ((iBit>=0 && iBit<=7) || 8==iBit || 16==iBit || 32==iBit)
	{
		
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if( 	iIdx<0 || iIdx>=MAX_EVENT_SCRIPT_VAR || 
		(16==iBit && iIdx>=MAX_EVENT_SCRIPT_VAR-1) || 
		(32==iBit && iIdx>=MAX_EVENT_SCRIPT_VAR-3))
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if( 8 == iBit )
	{
		iRet =	pstEvent->EventScriptVar[iIdx];
	}
	else if (16 == iBit)
	{
		iRet		= *(short*)(pstEvent->EventScriptVar+iIdx);
	}
	else if (32 == iBit)
	{
		iRet		= *(int*)(pstEvent->EventScriptVar+iIdx);
	}
	else
	{
		cFlag	=	1 << iBit;

		if( pstEvent->EventScriptVar[iIdx] & cFlag )
			iRet	=	1;
		else
			iRet	=	0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iRetValIdx, iRet);
	return 0;
}

int s_set_event_var_by_id()
{
	int iID;
	int iIdx;
	int iBit;
	int iVal;
	int iRetIdx;
	GameEventInst stEventInst, *pstEvent;
	static char sTemp[4];
	char cFlag;

	iID = get_script_number(NULL);
	iIdx = get_script_number(NULL);
	iBit = get_script_number(NULL);
	iVal = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);


	stEventInst.iEventID = iID;
	pstEvent = sht_find(stSEnv.pstEnv->pstShtEvent, &stEventInst, EventHashCmp, EventHashCode);
	if (NULL == pstEvent)
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if ((iBit>=0 && iBit<=7) || 8==iBit || 16==iBit || 32==iBit)
	{
		
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if( 	iIdx<0 || iIdx>=MAX_EVENT_SCRIPT_VAR|| 
		(16==iBit && iIdx>=MAX_EVENT_SCRIPT_VAR-1) || 
		(32==iBit && iIdx>=MAX_EVENT_SCRIPT_VAR-3))
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	memcpy(sTemp, &iVal, sizeof(int));

	if( 8 == iBit )
	{
		pstEvent->EventScriptVar[iIdx] = (char) iVal;
	}
	else if (16 == iBit)
	{
		pstEvent->EventScriptVar[iIdx]		=	sTemp[0];
		pstEvent->EventScriptVar[iIdx + 1]	=     sTemp[1];
	}
	else if (32 == iBit)
	{
		pstEvent->EventScriptVar[iIdx]		=	sTemp[0];
		pstEvent->EventScriptVar[iIdx + 1]	=	sTemp[1];
		pstEvent->EventScriptVar[iIdx + 2]	=	sTemp[2];
		pstEvent->EventScriptVar[iIdx + 3]	=	sTemp[3];
	}
	else
	{
		cFlag	=	1 << iBit;

		if( iVal )
			pstEvent->EventScriptVar[iIdx]		|=	cFlag;
		else
			pstEvent->EventScriptVar[iIdx]		&=	~cFlag;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_set_event_var()
{
	int iIdx;
	int iBit;
	int iVal;
	int iRetIdx;
	char cFlag;
	static char sTemp[4];

	iIdx = get_script_number(NULL);
	iBit = get_script_number(NULL);
	iVal = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (stSEnv.pstEvent == NULL)
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if ((iBit>=0 && iBit<=7) || 8==iBit || 16==iBit || 32==iBit)
	{
		
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if( iIdx<0 || iIdx>=MAX_EVENT_SCRIPT_VAR|| (16==iBit && iIdx>=MAX_EVENT_SCRIPT_VAR-1) || (32==iBit && iIdx>=MAX_EVENT_SCRIPT_VAR-3))
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	memcpy(sTemp, &iVal, sizeof(int));

	if( 8 == iBit )
	{
		stSEnv.pstEvent->EventScriptVar[iIdx] = (char) iVal;
	}
	else if (16 == iBit)
	{
		stSEnv.pstEvent->EventScriptVar[iIdx]		=	sTemp[0];
		stSEnv.pstEvent->EventScriptVar[iIdx + 1]	   	=     sTemp[1];
	}
	else if (32 == iBit)
	{
		stSEnv.pstEvent->EventScriptVar[iIdx]		=	sTemp[0];
		stSEnv.pstEvent->EventScriptVar[iIdx + 1]	=	sTemp[1];
		stSEnv.pstEvent->EventScriptVar[iIdx + 2]	=	sTemp[2];
		stSEnv.pstEvent->EventScriptVar[iIdx + 3]	=	sTemp[3];
	}
	else
	{
		cFlag	=	1 << iBit;

		if( iVal )
			stSEnv.pstEvent->EventScriptVar[iIdx]		|=	cFlag;
		else
			stSEnv.pstEvent->EventScriptVar[iIdx]		&=	~cFlag;
	}

	set_script_variable(iRetIdx, 0);

	return 0;
	
}

int s_set_pworld_var()
{
	int iIdx;
	int iBit;
	int iVal;
	int iRetIdx;
	ZONEPWORLDINST *pstPworldInst = NULL;
	ZONEMAPINST* pstMapInst;
	char cFlag;
	static char sTemp[4];
	
	iIdx = get_script_number(NULL);
	iBit = get_script_number(NULL);
	iVal = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );

	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if ((iBit>=0 && iBit<=7) || 8==iBit || 16==iBit || 32==iBit)
	{
		
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if( iIdx<0 || iIdx>=PWORLD_VAR || (16==iBit && iIdx>=PWORLD_VAR-1) || (32==iBit && iIdx>=PWORLD_VAR-3))
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	memcpy(sTemp, &iVal, sizeof(int));

	if( 8 == iBit )
	{
		pstPworldInst->sScriptVar[iIdx]	=	(char) iVal;
	}
	else if (16 == iBit)
	{
		pstPworldInst->sScriptVar[iIdx]		=	sTemp[0];
		pstPworldInst->sScriptVar[iIdx + 1]	=	sTemp[1];
	}
	else if (32 == iBit)
	{
		pstPworldInst->sScriptVar[iIdx]		=	sTemp[0];
		pstPworldInst->sScriptVar[iIdx + 1]	=	sTemp[1];
		pstPworldInst->sScriptVar[iIdx + 2]	=	sTemp[2];
		pstPworldInst->sScriptVar[iIdx + 3]	=	sTemp[3];
	}
	else
	{
		cFlag	=	1 << iBit;

		if( iVal )
			pstPworldInst->sScriptVar[iIdx]		|=	cFlag;
		else
			pstPworldInst->sScriptVar[iIdx]		&=	~cFlag;
	}

	set_script_variable(iRetIdx, 0);

	return 0;
}

int s_get_task_var()
{
	int iTaskID;
	int iIdx;
	int iBit;
	int iRetIdx;
	int iRetValIdx;
	char cFlag;
	int iRet;
	Player *pstPlayer = stSEnv.pstPlayer;
	TASKRUNONE *pstTaskRun;

	iTaskID = get_script_number(NULL);
	iIdx = get_script_number(NULL);
	iBit = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iRetValIdx = get_script_number(NULL);

	if (stSEnv.pstPlayer)
	{
		pstPlayer = stSEnv.pstPlayer;
	}
	else
	{
		if (!stSEnv.pstMonster)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		if (stSEnv.pstMonster->szOwner[0] == 0)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		pstPlayer = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstMonster->szOwner);
	}

	if (NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstTaskRun = task_run_find(pstPlayer, iTaskID);
	if (NULL == pstTaskRun)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if ((iBit>=0 && iBit<=7) || 8==iBit || 16==iBit || 32==iBit)
	{
		
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if( iIdx<0 || iIdx>=MAX_ONE_TASK_VAR || (16==iBit && iIdx>=MAX_ONE_TASK_VAR-1) || (32==iBit && iIdx>=MAX_ONE_TASK_VAR-3))
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if( 8 == iBit )
	{
		iRet =	pstTaskRun->TaskVars[iIdx];
	}
	else if (16 == iBit)
	{
		iRet		= *(short*)(pstTaskRun->TaskVars+iIdx);
	}
	else if (32 == iBit)
	{
		iRet		= *(int*)(pstTaskRun->TaskVars+iIdx);
	}
	else
	{
		cFlag	=	1 << iBit;

		if( pstTaskRun->TaskVars[iIdx] & cFlag )
			iRet	=	1;
		else
			iRet	=	0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iRetValIdx, iRet);

	return 0;
}

int s_set_task_var()
{
	int iTaskID;
	int iIdx;
	int iBit;
	int iVal;
	int iRetIdx;
	char cFlag;
	static char sTemp[4];
	Player *pstPlayer = stSEnv.pstPlayer;
	TASKRUNONE *pstTaskRun;

	iTaskID = get_script_number(NULL);
	iIdx = get_script_number(NULL);
	iBit = get_script_number(NULL);
	iVal = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstTaskRun = task_run_find(pstPlayer, iTaskID);
	if (NULL == pstTaskRun)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if ((iBit>=0 && iBit<=7) || 8==iBit || 16==iBit || 32==iBit)
	{
		
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if( iIdx<0 || iIdx>=MAX_ONE_TASK_VAR || (16==iBit && iIdx>=MAX_ONE_TASK_VAR-1) || (32==iBit && iIdx>=MAX_ONE_TASK_VAR-3))
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	memcpy(sTemp, &iVal, sizeof(int));

	if( 8 == iBit )
	{
		pstTaskRun->TaskVars[iIdx]	=	(char) iVal;
	}
	else if (16 == iBit)
	{
		pstTaskRun->TaskVars[iIdx]		=	sTemp[0];
		pstTaskRun->TaskVars[iIdx + 1]	=	sTemp[1];
	}
	else if (32 == iBit)
	{
		pstTaskRun->TaskVars[iIdx]		=	sTemp[0];
		pstTaskRun->TaskVars[iIdx + 1]	=	sTemp[1];
		pstTaskRun->TaskVars[iIdx + 2]	=	sTemp[2];
		pstTaskRun->TaskVars[iIdx + 3]	=	sTemp[3];
	}
	else
	{
		cFlag	=	1 << iBit;

		if( iVal )
			pstTaskRun->TaskVars[iIdx]		|=	cFlag;
		else
			pstTaskRun->TaskVars[iIdx]		&=	~cFlag;
	}

	set_script_variable(iRetIdx, 0);

	return 0;
	
}


int s_set_mapinst_var()
{
	int iIdx;
	int iBit;
	int iVal;
	int iRetIdx;
	char cFlag;
	static char sTemp[4];
	ZONEMAPINST *pstMapInst = NULL;

	iIdx = get_script_number(NULL);
	iBit = get_script_number(NULL);
	iVal = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	pstMapInst = stSEnv.pstMapinst;
	if (!pstMapInst)
	{
		pstMapInst =  s_get_mapinst();
		if (!pstMapInst)
			goto final;
	}
	
	if ((iBit>=0 && iBit<=7) || 8==iBit || 16==iBit || 32==iBit)
	{
		
	}
	else
	{
		goto final;
	}

	if( 	iIdx<0 || iIdx>=MAX_MAPINST_SCRI_VAR|| 
		(16==iBit && iIdx>=MAX_MAPINST_SCRI_VAR-1) || 
		(32==iBit && iIdx>=MAX_MAPINST_SCRI_VAR-3))
	{
		goto final;
	}

	memcpy(sTemp, &iVal, sizeof(int));

	if( 8 == iBit )
	{
		pstMapInst->szScriptVar[iIdx] = (char) iVal;
	}
	else if (16 == iBit)
	{
		pstMapInst->szScriptVar[iIdx]  		= sTemp[0];
		pstMapInst->szScriptVar[iIdx+1]  	= sTemp[1];
	}
	else if (32 == iBit)
	{
		pstMapInst->szScriptVar[iIdx]  		= sTemp[0];
		pstMapInst->szScriptVar[iIdx+1]  	= sTemp[1];
		pstMapInst->szScriptVar[iIdx+2]  	= sTemp[2];
		pstMapInst->szScriptVar[iIdx+3]  	= sTemp[3];
	}
	else
	{
		cFlag	=	1 << iBit;

		if( iVal )
			pstMapInst->szScriptVar[iIdx]		|=	cFlag;
		else
			pstMapInst->szScriptVar[iIdx]		&=	~cFlag;
	}

	set_script_variable(iRetIdx, 0);
	return 0;

final:
	set_script_variable(iRetIdx, -1);
	return 0;
}

int s_get_mapinst_var()
{
	int iIdx;
	int iBit;
	int iRetIdx;
	int iRetValIdx;
	char cFlag;
	int iRet;
	ZONEMAPINST *pstMapInst = NULL;

	iIdx = get_script_number(NULL);
	iBit = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iRetValIdx = get_script_number(NULL);

	pstMapInst = stSEnv.pstMapinst;
	if (!pstMapInst)
	{
		pstMapInst =  s_get_mapinst();
		if (!pstMapInst)
			goto final;
	}
	
	if ((iBit>=0 && iBit<=7) || 8==iBit || 16==iBit || 32==iBit)
	{
		
	}
	else
	{
		goto final;
		
	}

	if( 	iIdx<0 || iIdx>=MAX_MAPINST_SCRI_VAR || 
		(16==iBit && iIdx>=MAX_MAPINST_SCRI_VAR-1) || 
		(32==iBit && iIdx>=MAX_MAPINST_SCRI_VAR-3))
	{
		goto final;
	}

	if( 8 == iBit )
	{
		iRet =	pstMapInst->szScriptVar[iIdx];
	}
	else if (16 == iBit)
	{
		iRet		= *(short*)(pstMapInst->szScriptVar+iIdx);
	}
	else if (32 == iBit)
	{
		iRet		= *(int*)(pstMapInst->szScriptVar+iIdx);
	}
	else
	{
		cFlag	=	1 << iBit;

		if( pstMapInst->szScriptVar[iIdx] & cFlag )
			iRet	=	1;
		else
			iRet	=	0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iRetValIdx, iRet);
	return 0;
	
final:
	set_script_variable(iRetIdx, -1);
	return 0;
}



int s_set_mon_var()
{
	int iIdx;
	int iBit;
	int iVal;
	int iRetIdx;
	char cFlag;
	static char sTemp[4];
	Monster *pstMon =  stSEnv.pstMonster;

	iIdx = get_script_number(NULL);
	iBit = get_script_number(NULL);
	iVal = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (NULL == pstMon && stSEnv.pstNpc) 
	{
		pstMon = &stSEnv.pstNpc->stMon;
	}

	if (NULL == pstMon)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if ((iBit>=0 && iBit<=7) || 8==iBit || 16==iBit || 32==iBit)
	{
		
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if( iIdx<0 || iIdx>=MAX_MON_SCRIPT_VAR|| (16==iBit && iIdx>=MAX_MON_SCRIPT_VAR-1) || (32==iBit && iIdx>=MAX_MON_SCRIPT_VAR-3))
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	memcpy(sTemp, &iVal, sizeof(int));

	if( 8 == iBit )
	{
		pstMon->cMonScriptVar[iIdx] = (char) iVal;
		//pstTaskRun->TaskVars[iIdx]	=	(char) iVal;
	}
	else if (16 == iBit)
	{
		pstMon->cMonScriptVar[iIdx]		=	sTemp[0];
		pstMon->cMonScriptVar[iIdx + 1]	   	=     sTemp[1];
	}
	else if (32 == iBit)
	{
		pstMon->cMonScriptVar[iIdx]		=	sTemp[0];
		pstMon->cMonScriptVar[iIdx + 1]	=	sTemp[1];
		pstMon->cMonScriptVar[iIdx + 2]	=	sTemp[2];
		pstMon->cMonScriptVar[iIdx + 3]	=	sTemp[3];
	}
	else
	{
		cFlag	=	1 << iBit;

		if( iVal )
			pstMon->cMonScriptVar[iIdx]		|=	cFlag;
		else
			pstMon->cMonScriptVar[iIdx]		&=	~cFlag;
	}

	set_script_variable(iRetIdx, 0);

	return 0;
	
}


int s_get_mon_var()
{
	int iIdx;
	int iBit;
	int iRetIdx;
	int iRetValIdx;
	char cFlag;
	int iRet;
	Monster *pstMon =  stSEnv.pstMonster;

	iIdx = get_script_number(NULL);
	iBit = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iRetValIdx = get_script_number(NULL);

	if (NULL == pstMon && stSEnv.pstNpc) 
	{
		pstMon = &stSEnv.pstNpc->stMon;
	}

	if (NULL == pstMon)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if ((iBit>=0 && iBit<=7) || 8==iBit || 16==iBit || 32==iBit)
	{
		
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if( iIdx<0 || iIdx>=MAX_MON_SCRIPT_VAR || (16==iBit && iIdx>=MAX_MON_SCRIPT_VAR-1) || (32==iBit && iIdx>=MAX_MON_SCRIPT_VAR-3))
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if( 8 == iBit )
	{
		iRet =	pstMon->cMonScriptVar[iIdx];
	}
	else if (16 == iBit)
	{
		iRet		= *(short*)(pstMon->cMonScriptVar+iIdx);
	}
	else if (32 == iBit)
	{
		iRet		= *(int*)(pstMon->cMonScriptVar+iIdx);
	}
	else
	{
		cFlag	=	1 << iBit;

		if( pstMon->cMonScriptVar[iIdx] & cFlag )
			iRet	=	1;
		else
			iRet	=	0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iRetValIdx, iRet);

	return 0;
}

int s_get_role_var()
{
	int iIdx;
	int iBit;
	int iRetIdx;
	int iRetValIdx;
	char cFlag;
	int iRet;
	Player *pstPlayer = stSEnv.pstPlayer;
	
	iIdx = get_script_number(NULL);
	iBit = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iRetValIdx = get_script_number(NULL);

	if (NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if ((iBit>=0 && iBit<=7) || 8==iBit || 16==iBit || 32==iBit)
	{
		
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if( iIdx<0 || iIdx>=MAX_ROLE_SCRIPT_VAR || (16==iBit && iIdx>=MAX_ROLE_SCRIPT_VAR-1) || (32==iBit && iIdx>=MAX_ROLE_SCRIPT_VAR-3))
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if( 8 == iBit )
	{
		iRet =	pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx];
	}
	else if (16 == iBit)
	{
		iRet		= *(short*)(pstPlayer->stRoleData.MiscInfo.GlobalVar+iIdx);
	}
	else if (32 == iBit)
	{
		iRet		= *(int*)(pstPlayer->stRoleData.MiscInfo.GlobalVar+iIdx);
	}
	else
	{
		cFlag	=	1 << iBit;

		if( pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx] & cFlag )
			iRet	=	1;
		else
			iRet	=	0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iRetValIdx, iRet);

	return 0;
}

int s_set_role_var()
{
	int iIdx;
	int iBit;
	int iVal;
	int iRetIdx;
	char cFlag;
	static char sTemp[4];
	Player *pstPlayer = stSEnv.pstPlayer;

	iIdx = get_script_number(NULL);
	iBit = get_script_number(NULL);
	iVal = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if ((iBit>=0 && iBit<=7) || 8==iBit || 16==iBit || 32==iBit)
	{
		
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if( iIdx<0 || iIdx>=MAX_ROLE_SCRIPT_VAR || (16==iBit && iIdx>=MAX_ROLE_SCRIPT_VAR-1) || (32==iBit && iIdx>=MAX_ROLE_SCRIPT_VAR-3))
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	memcpy(sTemp, &iVal, sizeof(int));

	if( 8 == iBit )
	{
		pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx]	=	(char) iVal;
	}
	else if (16 == iBit)
	{
		pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx]		=	sTemp[0];
		pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx + 1]	=	sTemp[1];
	}
	else if (32 == iBit)
	{
		pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx]		=	sTemp[0];
		pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx + 1]	=	sTemp[1];
		pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx + 2]	=	sTemp[2];
		pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx + 3]	=	sTemp[3];
	}
	else
	{
		cFlag	=	1 << iBit;

		if( iVal )
			pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx]		|=	cFlag;
		else
			pstPlayer->stRoleData.MiscInfo.GlobalVar[iIdx]		&=	~cFlag;
	}

	set_script_variable(iRetIdx, 0);

	return 0;
	
}


int s_task_add_option()
{
	int iTaskID;
	int iRetIdx;
	int iProc;
	DialogOpt *pstDlgOpt;
	DLGOPTS *pstCSDlgOpt = &stSEnv.stDlgPkg.Body.DlgS.Dlg.DlgOpt;
	Player *pstPlayer = stSEnv.pstPlayer;

	iTaskID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iProc = get_script_number(NULL);

	if (NULL == pstPlayer || NULL == stSEnv.pstNpc)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (pstCSDlgOpt->Total >= MAX_DLG_OPT) return 0;

	pstDlgOpt = &stSEnv.pstPlayer->stOnline.stSData.stOpt;
		
	if (0 == task_opt_str(stSEnv.pstEnv, stSEnv.pstNpc, pstPlayer, pstCSDlgOpt->OneOpt[pstCSDlgOpt->Total].Opt, 
						sizeof(pstCSDlgOpt->OneOpt[pstCSDlgOpt->Total].Opt), iTaskID))
	{
		pstDlgOpt->ashProc[pstCSDlgOpt->Total] = iProc;
		pstCSDlgOpt->Total++;
		pstDlgOpt->shNum =  pstCSDlgOpt->Total;

		set_script_variable(iRetIdx, 0);
	}
	else
	{
		set_script_variable(iRetIdx, -1);
	}
	
	return 0;
}

int s_taskcycle_add_option()
{
	int iTaskCycleID;
	int iRetIdx;
	int iProc;
	DialogOpt *pstDlgOpt;
	DLGOPTS *pstCSDlgOpt = &stSEnv.stDlgPkg.Body.DlgS.Dlg.DlgOpt;
	Player *pstPlayer = stSEnv.pstPlayer;

	iTaskCycleID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iProc = get_script_number(NULL);

	if (NULL == pstPlayer || NULL == stSEnv.pstNpc)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (pstCSDlgOpt->Total >= MAX_DLG_OPT) return 0;

	pstDlgOpt = &stSEnv.pstPlayer->stOnline.stSData.stOpt;
		
	if (0 == taskcycle_opt_str(stSEnv.pstEnv, stSEnv.pstNpc, pstPlayer, pstCSDlgOpt->OneOpt[pstCSDlgOpt->Total].Opt, 
						sizeof(pstCSDlgOpt->OneOpt[pstCSDlgOpt->Total].Opt), iTaskCycleID))
	{
		pstDlgOpt->ashProc[pstCSDlgOpt->Total] = iProc;
		pstCSDlgOpt->Total++;
		pstDlgOpt->shNum =  pstCSDlgOpt->Total;

		set_script_variable(iRetIdx, 0);
	}
	else
	{
		set_script_variable(iRetIdx, -1);
	}
	
	return 0;
}

int s_task_do_option()
{
	int iTaskID;
	int iRetIdx;
	Player *pstPlayer = stSEnv.pstPlayer;

	iTaskID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (NULL == pstPlayer || NULL == stSEnv.pstNpc)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	task_option(stSEnv.pstEnv, pstPlayer, stSEnv.pstNpc->stMon.iID, iTaskID);
	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_taskcycle_do_option()
{
	int iTaskCycleID;
	int iRetIdx;
	Player *pstPlayer = stSEnv.pstPlayer;

	iTaskCycleID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (NULL == pstPlayer || NULL == stSEnv.pstNpc)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	taskcycle_option(stSEnv.pstEnv, pstPlayer, stSEnv.pstNpc->stMon.iID, iTaskCycleID);
	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_task_fini()
{
	int iTaskID;
	int iRetIdx;
	TASKRUNONE *pstTaskRunOne;
	Player *pstPlayer = stSEnv.pstPlayer;

	iTaskID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstTaskRunOne = task_run_find(pstPlayer, iTaskID);
	if (NULL == pstTaskRunOne)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(pstTaskRunOne->TaskStat  == TASK_STAT_FAIL)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstTaskRunOne->TaskStat = TASK_STAT_FINI;
	task_fini(stSEnv.pstEnv, pstPlayer, iTaskID, 1);

	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_task_stat()
{
	int iTaskID;
	int iRetIdx;
	int iTaskStat;
	TASKRUNONE *pstTaskRunOne;
	Player *pstPlayer = stSEnv.pstPlayer;

	iTaskID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iTaskStat = get_script_number(NULL);

	if (NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		set_script_variable(iTaskStat, 0);
		return 0;
	}

	pstTaskRunOne = task_run_find(pstPlayer, iTaskID);
	if (NULL == pstTaskRunOne)
	{
		if (0 == task_find_fini(pstPlayer, iTaskID))
		{
			set_script_variable(iTaskStat, 3);
		}
		else
		{
			set_script_variable(iTaskStat, 0);
		}
	}
	else
	{
		TASKDEF * pstTaskDef;
		
		pstTaskDef = task_def_find(stSEnv.pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			set_script_variable(iRetIdx, -1);
			set_script_variable(iTaskStat, 0);
			return 0;
		}
		
		if (task_check_fini(stSEnv.pstEnv, pstPlayer, pstTaskRunOne, pstTaskDef))
		{
			set_script_variable(iTaskStat, 1);
		}
		else
		{
			if(pstTaskRunOne->TaskStat == TASK_STAT_FAIL)
			{
				set_script_variable(iTaskStat, 4);
			}
			else
			{
				set_script_variable(iTaskStat, 2);
			}
		}
	}
	
	set_script_variable(iRetIdx, 0);
	
	return 0;
}


int play_effect(ZONESVRENV* pstEnv, Player *pstPlayer, int iEffectType, int iMemID, char* p, ZONEMAPINST *pstMapInst)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);

	pstActionRes->ID = iMemID;
	pstActionRes->ActionID = ACTION_EFFECT;
	
	add_str_replace(pstActionRes->Data.Effect.EffectStr, sizeof(pstActionRes->Data.Effect.EffectStr) - 1, p,0);
	pstActionRes->Data.Effect.EffectStr[strlen(pstActionRes->Data.Effect.EffectStr) - 1] = 0;

	switch (iEffectType)
	{
	case 0:
		map_broadcast(pstEnv->pstAppCtx, pstEnv, pstMapInst, &stPkg);
		break;

	case 1:
		z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
		break;

	case 2:
		if (pstPlayer)
		{
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		}
		break;

	case 3:
		if (pstPlayer)
		{
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		}
		break;

	case 4:
		if (pstPlayer)
		{
			ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
			if (!pstZoneClan)
			{
				return -1;
			}
			player_clan_broadcast(pstEnv, pstZoneClan, &stPkg, 0);
		}
		break;
	}
	
	return 0;
}

int s_effect()
{
	int iEffectType;
	int iObj;
	char *p;
	int iID = 0;
	ZONEMAPINST *pstMapInst = NULL;
	Player *pstPlayer = stSEnv.pstPlayer;
			
	iEffectType = get_script_number(NULL);
	iObj = get_script_number(NULL);
	p = get_script_string();

	if (0 == iEffectType)
	{
		pstMapInst = s_get_mapinst();
	}

	//发给monster属主
	if (3 == iEffectType && stSEnv.pstMonster)
	{
		if (stSEnv.pstMonster->szOwner[0] != 0)
		{
			pstPlayer = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstMonster->szOwner);
			if (!pstPlayer)
			{
				return 0;
			}
		}
	}

	if (1 ==  iObj)
	{
		if (stSEnv.pstPlayer)
		{
			iID = stSEnv.pstPlayer->iMemID;
		}
	}
	else if (2 == iObj)
	{
		if (stSEnv.pstNpc)
		{
			iID = stSEnv.pstNpc->stMon.iID;
		}
	}
	else if (3 == iObj)
	{
		if (stSEnv.pstMonster)
		{
			iID = stSEnv.pstMonster->iID;
		}
	}
	if(pstMapInst)
	{
		play_effect(stSEnv.pstEnv, pstPlayer, iEffectType, iID, p, pstMapInst);
	}
	else
	{
		play_effect(stSEnv.pstEnv, pstPlayer, iEffectType, iID, p, NULL);
	}

	return 0;
}

int s_destroy_mon()
{
	int iRetIdx;
	MONSTERDEF *pstMonDef = NULL;

	iRetIdx = get_script_number(NULL);

	if (NULL == stSEnv.pstMonster)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstMonDef = z_get_mon_def(stSEnv.pstEnv, stSEnv.pstMonster);
	if (pstMonDef)
	{
		ZoneAni stAni;

		stSEnv.pstMonster->cFirstAtkerType = 0;
		stSEnv.pstMonster->ullFirstAtkerWid = 0;
		stSEnv.pstMonster->iFirstAtkerID = 0; //没有战利品
		stSEnv.pstMonster->szOwner[0] = 0;
		stAni.iType = OBJ_UNKNOW;
		
		z_mon_die(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstMonster, pstMonDef, &stAni, 2, 0);
	}
	else
	{
		z_free_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstMonster);
	}

	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_del_mon()
{
	int iRetIdx;
	MONSTERDEF *pstMonDef = NULL;

	iRetIdx = get_script_number(NULL);

	if (NULL == stSEnv.pstMonster)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstMonDef = z_get_mon_def(stSEnv.pstEnv, stSEnv.pstMonster);
	if (pstMonDef)
	{
		ZoneAni stAni;

		if(stSEnv.pstPlayer)
		{
			stSEnv.pstMonster->cFirstAtkerType = MON_BOOTY_OWNER_PLAYER;
			stSEnv.pstMonster->ullFirstAtkerWid = stSEnv.pstPlayer->stRoleData.RoleID;
			stSEnv.pstMonster->iFirstAtkerID = stSEnv.pstPlayer->iMemID;

			stAni.iType = OBJ_PLAYER;
			stAni.stObj.pstPlayer = stSEnv.pstPlayer;
		}
		else
		{
			stAni.iType = OBJ_UNKNOW;
		}
		
		z_mon_die(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstMonster, pstMonDef, &stAni, 1, 0);
	}
	else
	{
		z_free_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstMonster);
	}

	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_mon_skill()
{
	int iSkillID;
	int iSkillLeve;
	ZoneObjList stObjList;
	MONSTERDEF *pstMonDef;
	SKILLDEF *pstSkillDef;
	Monster *pstMon = stSEnv.pstMonster;

	iSkillID = get_script_number(NULL);
	iSkillLeve = get_script_number(NULL);

	if (NULL == pstMon)
	{
		return 0;
	}

	pstMonDef = z_get_mon_def(stSEnv.pstEnv, pstMon);
	if (NULL == pstMonDef)
	{
		return 0;
	}

	pstSkillDef = z_find_skill(stSEnv.pstEnv, iSkillID, iSkillLeve, NULL);
	if (NULL == pstSkillDef)
	{
		return 0;
	}

	if (pstMon->cMoving)
	{
		z_monster_move_curr(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,  pstMon, 0);
	}

	z_mon_get_atk_list(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMon, pstMonDef, pstSkillDef, NULL, 
						&pstMon->stCurrPos, &stObjList, 1);
	pstMon->stMonAtkInfo.stAtkPos = pstMon->stCurrPos;

	if (stObjList.iObj > 0)
	{
		z_mon_hit_notify(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMon, pstMonDef, pstSkillDef, &stObjList);
	}
	
	return 0;
}

int s_get_ran_pos()
{
	int iInputX;
	int iInputY;
	int iRadius;
	int iRetIdx;
	int iOutputX;
	int iOutputY;
	RESPOS stPos1;
	RESPOS stPos2;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
		
	iInputX = get_script_number(NULL);
	iInputY = get_script_number(NULL);
	iRadius = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iOutputX = get_script_number(NULL);
	iOutputY = get_script_number(NULL);

	pstMapInst = s_get_mapinst();
	
	if (NULL ==pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	stPos1.X = iInputX;
	stPos1.Y = iInputY;
	z_get_ran_pos(stSEnv.pstEnv, pstMapInst, pstMapIdx, &stPos1, &stPos1, iRadius, 0, &stPos2);

	set_script_variable(iRetIdx, 0);
	set_script_variable(iOutputX, stPos2.X);
	set_script_variable(iOutputY, stPos2.Y);
	
	return 0;
}

int s_add_tmp_item()
{
	int iItemID;
	int iItemNum;
	int iInputX;
	int iInputY;
	int iRandRadius;
	int iRefresh;
	int iStay;
	int iRetIdx;
	int iDir;
	int i;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	ZONEIDX stIdx;
	RESPOS stInputPos, stPos;
	ITEMDEF *pstItemDef;

	iItemID = get_script_number(NULL);
	iItemNum = get_script_number(NULL);
	iInputX = get_script_number(NULL);
	iInputY = get_script_number(NULL);
	iRandRadius = get_script_number(NULL);
	iDir = get_script_number(NULL);
	iRefresh = get_script_number(NULL);
	iStay = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	
	pstMapInst = s_get_mapinst();

	if (NULL ==pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	stInputPos.X = iInputX;
	stInputPos.Y = iInputY;
	stIdx.iID = pstMapInst->iMapInstID;
	stIdx.iPos = pstMapInst->iMapPos;

	pstItemDef = z_find_itemdef(stSEnv.pstEnv, iItemID);
	if (NULL == pstItemDef || pstItemDef->ItemTmp.Type == ITEM_TMP_NO)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	for(i=0; i<iItemNum; i++)
	{
		if (iRandRadius > 0)
		{
			z_get_ran_pos(stSEnv.pstEnv, pstMapInst, pstMapIdx, &stInputPos, &stInputPos, iRandRadius, 0, &stPos);
		}

		if (0 > tmp_item_create(stSEnv.pstEnv,NULL, iItemID, &stIdx, &stPos, pstItemDef->ItemTmp.MachineEnerge, iRefresh, iStay, iDir,0,0,NULL))
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
	}
	
	set_script_variable(iRetIdx, 0);
	
	return 0;
}


int s_new_tmp_item()
{
	int iItemID;
	int iInputX;
	int iInputY;
	int iRefresh;
	int iStay;
	int iRetIdx;
	int iDir;
	ZONEMAPINST *pstMapInst;
	ZONEIDX stIdx;
	RESPOS stPos;
	ITEMDEF *pstItemDef;
	char szMsg[256];

	iItemID = get_script_number(NULL);
	iInputX = get_script_number(NULL);
	iInputY = get_script_number(NULL);
	iDir = get_script_number(NULL);
	iRefresh = get_script_number(NULL);
	iStay = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	
	pstMapInst = s_get_mapinst();

	if (NULL ==pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	stPos.X = iInputX;
	stPos.Y = iInputY;
	stIdx.iID = pstMapInst->iMapInstID;
	stIdx.iPos = pstMapInst->iMapPos;

	pstItemDef = z_find_itemdef(stSEnv.pstEnv, iItemID);
	if (NULL == pstItemDef || pstItemDef->ItemTmp.Type == ITEM_TMP_NO)
	{
		set_script_variable(iRetIdx, 0);
		sprintf(szMsg, "ItemID %d not find def", iItemID);
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, szMsg);
		return 0;
	}
		
	if (0 > tmp_item_create(stSEnv.pstEnv, NULL,iItemID, &stIdx, &stPos, pstItemDef->ItemTmp.MachineEnerge, iRefresh, iStay, iDir,0,0,NULL))
	{
		sprintf(szMsg, "ItemID %d create fail", iItemID);
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, szMsg);
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_get_player_id()
{
	int iRetIdx;
	int iPlayerId;

	iRetIdx = get_script_number(NULL);
	iPlayerId = get_script_number(NULL);

	if (NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iPlayerId, stSEnv.pstPlayer->iMemID);
	
	return 0;
}

int s_get_team_player_id()
{
	int iRetIdx;
	int iCaptainid;
	int iPlayernum;
	ZoneTeam *pstZoneTeam;
	int i;
	Player *pstPlayer;
	int aiTeamPlayerID[MAX_TEAM_MEMBER];
	int iTeamPlayerNum = 0;
	Player *pstCaptain = NULL;

	iRetIdx = get_script_number(NULL);
	iCaptainid = get_script_number(NULL);
	iPlayernum = get_script_number(NULL);
	aiTeamPlayerID[0] = get_script_number(NULL);
	aiTeamPlayerID[1] = get_script_number(NULL);
	aiTeamPlayerID[2] = get_script_number(NULL);
	aiTeamPlayerID[3] = get_script_number(NULL);
	aiTeamPlayerID[4] = get_script_number(NULL);
	aiTeamPlayerID[5] = get_script_number(NULL);

	if (NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstZoneTeam = player_team_get(stSEnv.pstEnv, stSEnv.pstPlayer);
	if (NULL == pstZoneTeam)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstPlayer = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (NULL == pstPlayer)
		{
			continue;
		}

		if (TEAM_POSITION_CAPTAIN == pstZoneTeam->stTeamInfo.TeamMember[i].Position)
		{
			pstCaptain = pstPlayer;
		}

		set_script_variable(aiTeamPlayerID[iTeamPlayerNum], pstPlayer->iMemID);
		iTeamPlayerNum++;
	}

	set_script_variable(iRetIdx, 0);
	if (pstCaptain)
	{
		set_script_variable(iCaptainid, pstCaptain->iMemID);
	}
	else
	{
		set_script_variable(iCaptainid, -1);
	}

	set_script_variable(iPlayernum, iTeamPlayerNum);
			
	return 0;
}

int s_add_sys_msg_start()
{
	CSSYSMSG *pstSysMsg = &stSEnv.stDlgPkg.Body.SysMsg;

	pstSysMsg->LinkLen = 0;
	pstSysMsg->Msg[0] = 0;
	return 0;
}

int s_add_sys_msg()
{
	char *p;
	char szMsg[1024];
	int iLen;
	CSSYSMSG *pstSysMsg = &stSEnv.stDlgPkg.Body.SysMsg;
	
	p = get_script_string();
	add_str_replace(szMsg, sizeof(szMsg), p,0);

	iLen = strlen(pstSysMsg->Msg);
	if (iLen > 0)
	{
		if (pstSysMsg->Msg[iLen-1] == '\n')
		{
			pstSysMsg->Msg[iLen-1] = 0;
			iLen--;
		}
	}
		
	if (iLen + strlen(szMsg) + 1> sizeof(pstSysMsg->Msg))	
	{
		return -1;
	}

	strcat(pstSysMsg->Msg, szMsg);

	return 0;
}

int s_add_sys_msg_pos()
{
	int iMap;
	int iX;
	int iY;
	int iLen;
	SYSMSGLINKPOS stLinkPos;
	char szTmp[256];
	MapIndex *pstMapIdx;
	CSSYSMSG *pstSysMsg = &stSEnv.stDlgPkg.Body.SysMsg;

	iMap = get_script_number(NULL);
	iX = get_script_number(NULL);
	iY = get_script_number(NULL);

	if (pstSysMsg->LinkLen > 0)
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, NULL, iMap);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	iLen = strlen(pstSysMsg->Msg);
	if (iLen > 0)
	{
		if (pstSysMsg->Msg[iLen-1] == '\n')
		{
			pstSysMsg->Msg[iLen-1] = 0;
			iLen--;
		}
	}

	sprintf(szTmp, "#cFF0000#L<%s(%d,%d)>@@5#n", pstMapIdx->stMapDef.Name, iX , iY);
	if (iLen + strlen(szTmp) + 1> sizeof(pstSysMsg->Msg))
	{
		return -1;
	}

	strcat(pstSysMsg->Msg, szTmp);
	pstSysMsg->LinkLen = sizeof(SYSMSGLINKPOS);
	stLinkPos.LinkFlag = 1;
	stLinkPos.MapID = iMap;
	stLinkPos.X = iX;
	stLinkPos.Y = iY;
	memcpy(pstSysMsg->LinkContent, &stLinkPos, sizeof(stLinkPos));

	return 0;
}

int s_set_sys_msg_mode()
{
	CSSYSMSG *pstSysMsg = &stSEnv.stDlgPkg.Body.SysMsg;

	pstSysMsg->Type = get_script_number(NULL);
	pstSysMsg->Times = get_script_number(NULL);
	pstSysMsg->Interval = get_script_number(NULL);

	return 0;
}


int s_open_npc_shop_by_id()
{
//	int i;
	int iID;
	NPCSELLLIST *pstSellList;
//	SHOPLIST *pstShopList;
/*	CSPKG stPkg;
	CSSHOPSVR *pstShopSvr = &stPkg.Body.ShopSvr;*/

	if (NULL == stSEnv.pstNpc || NULL == stSEnv.pstPlayer)
	{
		return 0;
	}

	iID = get_script_number(NULL);

	pstSellList = z_get_npc_selllist_ex(stSEnv.pstEnv, iID);
	if (!pstSellList)
	{
		return 0;
	}

//	if (stSEnv.pstNpc->nSellListID == 0)
	{	
		stSEnv.pstNpc->nSellListID = iID;
	}
	s_open_npc_shop();

	/*memset(pstShopSvr, 0, sizeof(*pstShopSvr));

	pstShopSvr->OpType = SHOP_LIST;
	pstShopList = &pstShopSvr->ShopSvrData.ShopList;
	pstShopList->ConditionNum = 0;
	
	for (i=0; i<(int)pstSellList->Count; i++)
	{
		pstShopList->ShopSellItems[i].GoodsID = pstSellList->Goods[i].GoodsID;
		pstShopList->ShopSellItems[i].GoodsType = pstSellList->Goods[i].GoodsType;
		
		if (pstSellList->Goods[i].Condition.Type > 0)
		{
			pstShopList->Conditions[pstShopList->ConditionNum].GridIdx = i;
			pstShopList->Conditions[pstShopList->ConditionNum].Condition = pstSellList->Goods[i].Condition;
			
			pstShopList->ConditionNum++;
		}

		if (0 != pstSellList->Goods[i].SpecialMoneyType)
		{
			pstShopList->ShopSellItems[i].Price = pstSellList->Goods[i].SpecialMoneyNum;
			pstShopList->ShopSellItems[i].SpecialMoneyType = pstSellList->Goods[i].SpecialMoneyType;
		}
		else
		{
			switch(pstShopList->ShopSellItems[i].GoodsType)
			{
			case SHOP_ITEM_ITEM:
				{
					const ITEMDEF *pstDef = z_find_itemdef(stSEnv.pstEnv,
					                         pstShopList->ShopSellItems[i].GoodsID);
					if (!pstDef)
					{
						return -1;
					}

					if (pstDef->ClanContri <= 0)
					{
						pstShopList->ShopSellItems[i].Price = pstDef->BuyPrice;
					}
					else
					{
						pstShopList->ShopSellItems[i].Price = pstDef->ClanContri;
					}
				}
				break;

			case SHOP_ITEM_CLAN_ITEM:
				{
					const CLANPRODUCTDEF *pstDef = clan_product_def(stSEnv.pstEnv,
					                         pstShopList->ShopSellItems[i].GoodsID);
					if (!pstDef)
					{
						return -1;
					}
					pstShopList->ShopSellItems[i].Price = pstDef->BuyPrice;
				}
				break;

			default:
				return -1;
				break;
			}
		}
	}

	player_cacl_rebate( stSEnv.pstEnv, stSEnv.pstPlayer, &pstShopList->RebateCount,
						&pstShopList->RebateTypeNum, pstShopList->RebateTypes);

	pstShopList->NpcID = stSEnv.pstNpc->stMon.iID;
	pstShopList->SellListID = pstSellList->SellListID;
	pstShopList->ShopItemNum = pstSellList->Count;
	selled_del(stSEnv.pstEnv,&stSEnv.pstPlayer->stRoleData.MiscInfo.ItemSellSave);
	memcpy(&pstShopList->ItemSellSave, &stSEnv.pstPlayer->stRoleData.MiscInfo.ItemSellSave, sizeof(pstShopList->ItemSellSave));

	Z_CSHEAD_INIT(&stPkg.Head, SHOP_SVR);
	z_cltmsg_send(stSEnv.pstEnv, stSEnv.pstPlayer, &stPkg);

	stSEnv.pstNpc->stMon.tFreezeEnd = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec + 20;
	if (stSEnv.pstNpc->stMon.cMoving)
	{
		z_mon_stop(stSEnv.pstEnv, &stSEnv.pstNpc->stMon); 
	}*/
	return 0;
}

int s_open_npc_shop()
{
	int i;
	SHOPLIST *pstShopList;
	NPCSELLLIST *pstSellList;
	CSPKG stPkg;
	CSSHOPSVR *pstShopSvr = &stPkg.Body.ShopSvr;

	if (NULL == stSEnv.pstNpc || NULL == stSEnv.pstPlayer)
	{
		return -1;
	}

	pstSellList = z_get_npc_selllist(stSEnv.pstEnv, stSEnv.pstNpc);
	if (NULL == pstSellList)
	{
		return -1;
	}
	memset(pstShopSvr, 0, sizeof(*pstShopSvr));

	pstShopSvr->OpType = SHOP_LIST;
	pstShopList = &pstShopSvr->ShopSvrData.ShopList;
	pstShopList->ConditionNum = 0;
	
	for (i=0; i<(int)pstSellList->Count; i++)
	{
		pstShopList->ShopSellItems[i].GoodsID = pstSellList->Goods[i].GoodsID;
		pstShopList->ShopSellItems[i].GoodsType = pstSellList->Goods[i].GoodsType;
		
		if (pstSellList->Goods[i].Condition.Type > 0)
		{
			pstShopList->Conditions[pstShopList->ConditionNum].GridIdx = i;
			pstShopList->Conditions[pstShopList->ConditionNum].Condition = pstSellList->Goods[i].Condition;
			
			pstShopList->ConditionNum++;
		}

		if (0 != pstSellList->Goods[i].SpecialMoneyType)
		{
			pstShopList->ShopSellItems[i].Price = pstSellList->Goods[i].SpecialMoneyNum;
			pstShopList->ShopSellItems[i].SpecialMoneyType = pstSellList->Goods[i].SpecialMoneyType;
		}
		else
		{
			switch(pstShopList->ShopSellItems[i].GoodsType)
			{
			case SHOP_ITEM_ITEM:
				{
					const ITEMDEF *pstDef = z_find_itemdef(stSEnv.pstEnv,
					                         pstShopList->ShopSellItems[i].GoodsID);
					if (!pstDef)
					{
						return -1;
					}

					if (pstDef->ClanContri <= 0)
					{
						pstShopList->ShopSellItems[i].Price = pstDef->BuyPrice;
					}
					else
					{
						pstShopList->ShopSellItems[i].Price = pstDef->ClanContri;
					}
				}
				break;

			case SHOP_ITEM_CLAN_ITEM:
				{
					const CLANPRODUCTDEF *pstDef = clan_product_def(stSEnv.pstEnv,
					                         pstShopList->ShopSellItems[i].GoodsID);
					if (!pstDef)
					{
						return -1;
					}
					pstShopList->ShopSellItems[i].Price = pstDef->BuyPrice;
				}
				break;

			default:
				return -1;
				break;
			}
		}
	}

	player_cacl_rebate( stSEnv.pstEnv, stSEnv.pstPlayer, &pstShopList->RebateCount,
						&pstShopList->RebateTypeNum, pstShopList->RebateTypes);

	pstShopList->NpcID = stSEnv.pstNpc->stMon.iID;
	pstShopList->SellListID = pstSellList->SellListID;
	pstShopList->ShopItemNum = pstSellList->Count;
	selled_del(stSEnv.pstEnv,&stSEnv.pstPlayer->stRoleData.MiscInfo.ItemSellSave);
	memcpy(&pstShopList->ItemSellSave, &stSEnv.pstPlayer->stRoleData.MiscInfo.ItemSellSave, sizeof(pstShopList->ItemSellSave));

	Z_CSHEAD_INIT(&stPkg.Head, SHOP_SVR);
	z_cltmsg_send(stSEnv.pstEnv, stSEnv.pstPlayer, &stPkg);

	stSEnv.pstNpc->stMon.tFreezeEnd = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec + 20;
	if (stSEnv.pstNpc->stMon.cMoving)
	{
		z_mon_stop(stSEnv.pstEnv, &stSEnv.pstNpc->stMon); 
	}
	
	return 0;
}


int s_new_map_npc()
{
	int iNpcID;
	int iX;
	int iY;
	int iDir;
	char *pszScript;
	int iRetIdx;
	MAPNPC stMapNpc;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIndex;
	int iRet;
	Npc *pstNpc;
	int iMap;
	ZONEIDX stIdx;

	iRetIdx = get_script_number(NULL);
	iNpcID = get_script_number(NULL);
	iX = get_script_number(NULL);
	iY = get_script_number(NULL);
	iDir = get_script_number(NULL);
	iMap = get_script_number(NULL);
	pszScript = get_script_string();

	stIdx.iID = iMap;
	stIdx.iPos = -1;
	
	pstMapInst = z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapIndex = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIndex)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	memset(&stMapNpc, 0, sizeof(stMapNpc));
	stMapNpc.MonsterID = iNpcID;
	stMapNpc.Pos.X = iX;
	stMapNpc.Pos.Y = iY;
	stMapNpc.Dir = iDir;
	STRNCPY(stMapNpc.Script, pszScript, sizeof(stMapNpc.Script));

	iRet = z_in_new_pworld_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMapIndex, pstMapInst, &stMapNpc,0);
	if (0 > iRet)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, iRet);

	pstNpc = z_id_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iRet);
	if(pstNpc)
	{
		newnpc_add(stSEnv.pstEnv, pstMapInst, pstNpc);
	}
	
	return 0;
	
}

int s_new_npc()
{
	int iNpcID;
	int iX;
	int iY;
	int iDir;
	char *pszScript;
	int iRetIdx;
	MAPNPC stMapNpc;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIndex;
	int iRet;
	Npc *pstNpc;

	iRetIdx = get_script_number(NULL);
	iNpcID = get_script_number(NULL);
	iX = get_script_number(NULL);
	iY = get_script_number(NULL);
	iDir = get_script_number(NULL);
	pszScript = get_script_string();
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapIndex = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIndex)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	memset(&stMapNpc, 0, sizeof(stMapNpc));
	stMapNpc.MonsterID = iNpcID;
	stMapNpc.Pos.X = iX;
	stMapNpc.Pos.Y = iY;
	stMapNpc.Dir = iDir;
	STRNCPY(stMapNpc.Script, pszScript, sizeof(stMapNpc.Script));

	iRet = z_in_new_pworld_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMapIndex, pstMapInst, &stMapNpc,0);
	if (0 > iRet)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, iRet);

	pstNpc = z_id_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iRet);
	if(pstNpc)
	{
		newnpc_add(stSEnv.pstEnv, pstMapInst, pstNpc);
	}
	
	return 0;
}


int s_get_map_inst()
{
	int iMap;
	int iRetIdx;
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst = NULL;

	iMap = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (iMap == 0)
	{
		stSEnv.iMapInstFist = 0;

		if (stSEnv.iStackTop > 0)
		{
			stSEnv.astCallStack[stSEnv.iStackTop - 1].iMapInstFist = stSEnv.iMapInstFist;
		}
		set_script_variable(iRetIdx, 0);

		return 0;
	}
	

	stIdx.iID = iMap;
	stIdx.iPos = -1;

	pstMapInst = z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stIdx);
	if (!pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	stSEnv.pstMapinst = pstMapInst;
	stSEnv.iMapInstFist = 1;

	if (stSEnv.iStackTop > 0)
	{
		stSEnv.astCallStack[stSEnv.iStackTop - 1].pstMapInst = stSEnv.pstMapinst;
		stSEnv.astCallStack[stSEnv.iStackTop - 1].iMapInstFist = stSEnv.iMapInstFist;
	}
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_del_npc_by_mapid()
{
	int i, k, iDyn;
	int iID, iIndex;
	char cType;
	int iNpcID;
	int iMapID;
	int iRetIdx;
	ZONEMAPINST *pstMapInst;
	AREADYN *pstAreaDyn;
	TMEMBLOCK *pstMemBlock;
	Npc *pstNpc;
	int iMemID = 0;
	MONSTERDEF *pstMonDef;
	ZONEIDX stIdx;

	iNpcID = get_script_number(NULL);
	iMapID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	stIdx.iID = iMapID;
	stIdx.iPos = -1;
	pstMapInst = z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	for (i=pstMapInst->iAreaDyn-1; i>=0; i--)
	{
		iDyn	=	pstMapInst->aiAreaDyn[i];

		if( iDyn<0)
		{
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(stSEnv.pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			continue;
		}

		for(k=pstAreaDyn->iAnimate-1; k>=0; k--)
		{
			iID	=	pstAreaDyn->aiAnimate[k];
			iIndex	=	z_id_indextype(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iID, &cType);

			if( -1==iIndex )
			{
				continue;
			}

			switch(cType)
			{
				case OBJ_NPC:
					pstMemBlock = TMEMPOOL_GET_PTR(stSEnv.pstEnv->pstNpcPool, iIndex);
					if (!pstMemBlock->fValid)
					{
						break;
					}
					pstNpc	=	(Npc *)pstMemBlock->szData;

					if (pstNpc->stMon.iDefIdx == iNpcID)
					{
						//要删除定义中不刷新的npc
						pstMonDef = z_get_mon_def(stSEnv.pstEnv, &pstNpc->stMon);
						if (pstMonDef && pstMonDef->NotRefresh)
						{
							pstNpc->stMon.cRefresh = 0;
						}
							
						iMemID = pstNpc->stMon.iID;
						z_npc_die(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstNpc);
					}
					
					break;
				default:
					break;
			}
		}
	}

	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_del_npc()
{
	int i, k, iDyn;
	int iID, iIndex;
	char cType;
	int iNpcID;
	int iRetIdx;
	ZONEMAPINST *pstMapInst;
	AREADYN *pstAreaDyn;
	TMEMBLOCK *pstMemBlock;
	Npc *pstNpc;
	int iMemID = 0;
	MONSTERDEF *pstMonDef;

	iNpcID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (0 == iNpcID)
	{
		if (NULL == stSEnv.pstNpc)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
		
		z_npc_die(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstNpc);
		set_script_variable(iRetIdx, 0);
		
		return 0;
	}

	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	for (i=pstMapInst->iAreaDyn-1; i>=0; i--)
	{
		iDyn	=	pstMapInst->aiAreaDyn[i];

		if( iDyn<0)
		{
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(stSEnv.pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			continue;
		}

		for(k=pstAreaDyn->iAnimate-1; k>=0; k--)
		{
			iID	=	pstAreaDyn->aiAnimate[k];
			iIndex	=	z_id_indextype(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iID, &cType);

			if( -1==iIndex )
			{
				continue;
			}

			switch(cType)
			{
				case OBJ_NPC:
					pstMemBlock = TMEMPOOL_GET_PTR(stSEnv.pstEnv->pstNpcPool, iIndex);
					if (!pstMemBlock->fValid)
					{
						break;
					}
					pstNpc	=	(Npc *)pstMemBlock->szData;

					if (pstNpc->stMon.iDefIdx == iNpcID)
					{
						//要删除定义中不刷新的npc
						pstMonDef = z_get_mon_def(stSEnv.pstEnv, &pstNpc->stMon);
						if (pstMonDef && pstMonDef->NotRefresh)
						{
							pstNpc->stMon.cRefresh = 0;
						}
							
						iMemID = pstNpc->stMon.iID;
						z_npc_die(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstNpc);
					}
					
					break;
				default:
					break;
			}
		}
	}

	set_script_variable(iRetIdx, 0);
					
	return 0;
}


int s_ploy_gc_end_player_clear()
{
	int iRetIdx;

	iRetIdx = get_script_number(NULL);

	if (stSEnv.pstEnv == NULL )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	ploy_gc_end_player_clear(stSEnv.pstEnv);
	set_script_variable(iRetIdx, 0);
	return 0;
}

// iType=0--弹出ui
// iType=1--关闭ui
int s_ploy_gc_ui_info()
{
	int iRetIdx;
	int iType;

	iRetIdx = get_script_number(NULL);
	iType = get_script_number(NULL);

	if (stSEnv.pstEnv == NULL )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	GameEventInst stEventInst, *pstEvent;
	
	stEventInst.iEventID = GC_EVENT_ID;
	pstEvent = sht_find(stSEnv.pstEnv->pstShtEvent, &stEventInst, EventHashCmp, EventHashCode);
	if (!pstEvent)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	ploy_gc_ui_info(stSEnv.pstEnv,NULL, iType, pstEvent->tEventStart);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_ploy_gc_set_award()
{
	int iRetIdx;
	int iLayer;
	int iRet;
	
	iRetIdx = get_script_number(NULL);
	iLayer = get_script_number(NULL);

	if (stSEnv.pstEnv == NULL || stSEnv.pstPlayer == NULL)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iRet = ploy_gc_set_award(stSEnv.pstEnv, stSEnv.pstPlayer, iLayer);
	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_ploy_gc_get_award()
{
	int iRetIdx;
	int iLayer;
	int iRet;
	
	iRetIdx = get_script_number(NULL);
	iLayer = get_script_number(NULL);

	if (stSEnv.pstEnv == NULL || stSEnv.pstPlayer == NULL)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iRet = ploy_gc_get_award(stSEnv.pstEnv, stSEnv.pstPlayer, iLayer);
	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_get_npc_num()
{
	int iNpcID, iRetIdx, iRetNum;
	int i, k, iDyn;
	int iID, iIndex;
	char cType;
	ZONEMAPINST *pstMapInst;
	AREADYN *pstAreaDyn;
	TMEMBLOCK *pstMemBlock;
	Npc *pstNpc;
	int iNum=0;
	
	iNpcID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iRetNum = get_script_number(NULL);

	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	for (i=pstMapInst->iAreaDyn-1; i>=0; i--)
	{
		iDyn	=	pstMapInst->aiAreaDyn[i];

		if( iDyn<0)
		{
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(stSEnv.pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			continue;
		}

		for(k=pstAreaDyn->iAnimate-1; k>=0; k--)
		{
			iID	=	pstAreaDyn->aiAnimate[k];
			iIndex	=	z_id_indextype(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iID, &cType);

			if( -1==iIndex )
			{
				continue;
			}

			switch(cType)
			{
				case OBJ_NPC:
					pstMemBlock = TMEMPOOL_GET_PTR(stSEnv.pstEnv->pstNpcPool, iIndex);
					if (!pstMemBlock->fValid)
					{
						break;
					}
					pstNpc	=	(Npc *)pstMemBlock->szData;

					if (iNpcID == 0 || pstNpc->stMon.iDefIdx == iNpcID)
					{
						iNum++;
					}
					
					break;
				default:
					break;
			}
		}
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iRetNum, iNum);
	
	return 0;
}

int s_pworld_over()
{
	ZONEMAPINST *pstMapInst;
	ZONEPWORLDINST *pstPworldInst;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		return 0;
	}

	z_pworld_over(stSEnv.pstEnv,pstPworldInst);

	//clean_pworld_mon(stSEnv.pstEnv,1);
	
	return 0;
}

int z_check_mon_script(ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef)
{	
	char szScript[32];
	ScriptIndex *pstScriptInd;
	UNUSED(pstEnv);
	
	pstMon->cMonScriptFlag = 0;
	pstMon->stScriptIdx.cScriptType = -1;

	snprintf(szScript, sizeof(szScript), "monster%d.mac", pstMonDef->MonsterID);
	szScript[sizeof(szScript) - 1] = 0;
	
	pstScriptInd = get_script_ind(szScript);
	if (NULL == pstScriptInd)
	{
		return 0;
	}
	
	pstMon->stScriptIdx = *pstScriptInd;

	if (0 == check_script_proc(szScript, pstScriptInd, MON_SCRIPT_PROC_NEW))
	{
		pstMon->cMonScriptFlag |= MON_SCRIPT_PROC_NEW_MAP;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, MON_SCRIPT_PROC_DIE))
	{
		pstMon->cMonScriptFlag |= MON_SCRIPT_PROC_DIE_MAP;
	}
	
	if (0 == check_script_proc(szScript, pstScriptInd, MON_SCRIPT_PROC_PATROL))
	{
		pstMon->cMonScriptFlag |= MON_SCRIPT_PROC_PATROL_MAP;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, MON_SCRIPT_PROC_TIMER))
	{
		pstMon->cMonScriptFlag |= MON_SCRIPT_PROC_TIMER_MAP;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, MON_SCRIPT_PROC_COLLECT_BEGIN))
	{
		pstMon->cMonScriptFlag |= MON_SCRIPT_PROC_COLLECT_BEGIN_MAP;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, MON_SCRIPT_PROC_COLLECT_END))
	{
		pstMon->cMonScriptFlag |= MON_SCRIPT_PROC_COLLECT_END_MAP;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, MON_SCRIPT_PROC_FORCE_MOVE_END))
	{
		pstMon->cMonScriptFlag |= MON_SCRIPT_PROC_FORCE_MOVE_END_MAP;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, MON_SCRIPT_PROC_AI_RESTORE_HP))
	{
		pstMon->cMonScriptFlag |= MON_SCRIPT_PROC_AI_RESTORE_HP_MAP;
	}

	if( 0 == check_script_proc(szScript, pstScriptInd, MON_SCRIPT_PROC_ATK_BY_PLAYER))
	{
		pstMon->cMonScriptFlag |= MON_SCRIPT_PROC_ATK_BY_PLAYER_MAP;
	}
	if( 0 == check_script_proc(szScript, pstScriptInd, MON_SCRIPT_PROC_TIMER2))
	{
		pstMon->cMonScriptFlag |= MON_SCRIPT_PROC_TIMER2_MAP;
	}

	if( 0 == check_script_proc(szScript, pstScriptInd, MON_SCRIPT_PROC_PRE_DIE))
	{
		pstMon->cMonScriptFlag |= MON_SCRIPT_PROC_PRE_DIE_MAP;
	}

	return 0;
}

int z_check_pworld_script(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPworldInst, ZONEPWORLD *pstZonePworld)
{
	char szScript[32];
	ScriptIndex *pstScriptInd;
	UNUSED(pstEnv);
	
	pstPworldInst->cPworldScriptFlag = 0;
	pstPworldInst->stScriptIdx.cScriptType = -1;

	snprintf(szScript, sizeof(szScript),"pworld%d.mac", pstZonePworld->stPworldDef.PworldID);
	szScript[sizeof(szScript) - 1] = 0;
	
	pstScriptInd = get_script_ind(szScript);
	if (NULL == pstScriptInd)
	{
		return 0;
	}
	
	pstPworldInst->stScriptIdx = *pstScriptInd;

	if (0 == check_script_proc(szScript, pstScriptInd, PWORLD_SCRIPT_PROC_START))
	{
		pstPworldInst->cPworldScriptFlag |= PWORLD_SCRIPT_PROC_START_MAP;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, PWORLD_SCRIPT_PROC_END))
	{
		pstPworldInst->cPworldScriptFlag |= PWORLD_SCRIPT_PROC_END_MAP;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, PWORLD_SCRIPT_PROC_TIMER))
	{
		pstPworldInst->cPworldScriptFlag |= PWORLD_SCRIPT_PROC_TIMER_MAP;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, PWORLD_SCRIPT_PROC_MON_DIE))
	{
		pstPworldInst->cPworldScriptFlag |= PWORLD_SCRIPT_PROC_MON_DIE_MAP;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, PWORLD_SCRIPT_PROC_ENTER))
	{
		pstPworldInst->cPworldScriptFlag |= PWORLD_SCRIPT_PROC_ENTER_MAP;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, PWORLD_SCRIPT_PROC_ROLE_DIE))
	{
		pstPworldInst->cPworldScriptFlag |= PWORLD_SCRIPT_PROC_ROLE_DIE_EVENT;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, PWORLD_SCRIPT_PROC_DOWN_TIME))
	{
		pstPworldInst->cPworldScriptFlag |= PWORLD_SCRIPT_PROC_DOWN_TIME_EVENT;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, PWORLD_SCRIPT_PROC_REAL_START))
	{
		pstPworldInst->cPworldScriptFlag |= PWORLD_SCRIPT_PROC_REAL_START_MAP;
	}

	
	
	return 0;
}

int s_autofly()
{
	int iAutoFlyPath;
	int iRetIdx;
	Player *pstPlayer = stSEnv.pstPlayer;

	iAutoFlyPath = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	
	if (NULL == pstPlayer || iAutoFlyPath <= 0 || iAutoFlyPath > 255)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (0 > player_auto_fly(stSEnv.pstEnv, pstPlayer, iAutoFlyPath))
	{
		set_script_variable(iRetIdx, -1);
	}
	else
	{
		set_script_variable(iRetIdx, 0);
	}
	
	return 0;
}

int s_get_trig_task()
{
	int iRetIdx;
	int iTask;
	
	iRetIdx = get_script_number(NULL);
	iTask = get_script_number(NULL);

	set_script_variable(iRetIdx, 0);
	set_script_variable(iTask, stSEnv.iTaskCurr);
	
	return 0;
}

int s_out_pworld_by_single()
{
	Player *pstPlayer = stSEnv.pstPlayer;
	if (!pstPlayer)
	{
		return 0;
	}

	player_pworld_exit_chgmap(stSEnv.pstEnv, pstPlayer);
	return 0;
}

int s_out_pworld_by_team()
{
	TEAMMEMBER * pstCaptain;
	ZoneTeam * pstTeam;
	int i;
	Player *pstPlayerTeam;
	
	if (!stSEnv.pstPlayer)
	{
		return 0;
	}
	
	pstTeam = player_team_get(stSEnv.pstEnv, stSEnv.pstPlayer);
	if(pstTeam == NULL)
	{
		return 0;
	}

	pstCaptain = player_team_get_captain(pstTeam);
	if(pstCaptain == NULL)
	{
		return 0;
	}

	if(strcmp(pstCaptain->MemberName, stSEnv.pstPlayer->stRoleData.RoleName) != 0)
	{
		return 0;
	}
	
	for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{	
		pstPlayerTeam = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstPlayerTeam)
		{
			player_pworld_exit_chgmap(stSEnv.pstEnv, pstPlayerTeam);
		}
	}
	
	return 0;
}

int z_check_item_script(ZONESVRENV* pstEnv, ITEMDEF  *pstItemDef)
{
	char szScript[32];
	ScriptIndex *pstScriptInd;
	UNUSED(pstEnv);
	
	snprintf(szScript, sizeof(szScript),"item%d.mac", pstItemDef->ItemID);
	szScript[sizeof(szScript) - 1] = 0;
	
	pstScriptInd = get_script_ind(szScript);
	if (NULL == pstScriptInd)
	{
		return 0;
	}
	

	if (0 == check_script_proc(szScript, pstScriptInd, ITEM_SCRIPT_USE))
	{
		pstItemDef->ItemScript |= ITEM_SCRIPT_USE_MAP;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, ITEM_SCRIPT_USE_TEST))
	{
		pstItemDef->ItemScript |= ITEM_SCRIPT_USE_TEST_MAP;
	}
	
	return 0;
}

int s_add_pworld_resource()
{
	int iPlayerID;
	int iType;
	int iRes;
	int iRetIdx;
	Player *pstPlayer;
	ZONEPWORLD *pstZonePworld;
	ZONEPWORLDINST * pstPworldInst;
	

	iPlayerID = get_script_number(NULL);
	iType = get_script_number(NULL);
	iRes = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (0 == iPlayerID)
	{
		pstPlayer = stSEnv.pstPlayer;
	}
	else
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iPlayerID);
	}
	
	if (NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (iRes == 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (iType == POLY_RES_TYPE_CYTSTAL)
	{
		pstPlayer->stOnline.stWarOL.iCrystal += iRes;
	}
	else if (iType == POLY_RES_TYPE_ORE)
	{
		pstPlayer->stOnline.stWarOL.iOre += iRes;
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworldinst2(stSEnv.pstEnv, pstPlayer->stRoleData.Map);
	if (!pstPworldInst )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
		
	}

	pstZonePworld = z_find_pworld(stSEnv.pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if (!pstZonePworld)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	//  通知客户端自己资源变化
	normal_battle_svr_info(stSEnv.pstEnv, pstPlayer, pstZonePworld, pstPworldInst,0, 0);
	//z_ploy_info_chg(stSEnv.pstEnv, pstPlayer, SELF_RES_CHG);

	set_script_variable(iRetIdx, 0);

	return 0;
}

int s_get_now_time()
{
	int imonth;
	int iday;
	int iweeks;
	int iweekday;
	int hours;
	int minute;
	int sec;
	int i,w,n,wd;
	time_t now;
	struct tm *pstnow;

	now = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;
	pstnow = localtime(&now);

	imonth = get_script_number(NULL);
	iday = get_script_number(NULL);
	iweeks = get_script_number(NULL);
	iweekday = get_script_number(NULL);
	hours = get_script_number(NULL);
	minute = get_script_number(NULL);
	sec = get_script_number(NULL);
	
	if ( pstnow->tm_wday == 0 )
	{
		wd = 7;
	}
	else
	{
		wd = pstnow->tm_wday;
	}
	
	i = pstnow->tm_mday;     // (wday%7+day) / 7   如果有余在+1       wday {0~ 6} 
	n = 7 - wd  ;
	i += n;
	w = i%7;
	
	if ( w == 0 )
	{
		w = i /7;
	}
	else
	{
		w = i /7 +1;
	}

	
	set_script_variable(imonth ,pstnow->tm_mon+1);
	set_script_variable(iday ,pstnow->tm_mday);
	set_script_variable(iweeks ,w);
	set_script_variable(iweekday ,wd);
	set_script_variable(hours ,pstnow->tm_hour);
	set_script_variable(minute ,pstnow->tm_min);
	set_script_variable(sec, pstnow->tm_sec);

	return 0;
	
}

int s_get_time_sec()
{
	int iSec;

	iSec = get_script_number(NULL);

	set_script_variable(iSec, stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec);
	
	return 0;
}

int s_get_tmp_num()
{
	int iTmpID;
	int iRet;
	int iTmpNum;
	int i, j, iCount;
	ZONEMAPINST *pstMapInst;
	AREADYN* pstAreaDyn;
	DropItem *pstDropItem;
	ZONESVRENV* pstEnv = stSEnv.pstEnv;

	iTmpID = get_script_number(NULL);
	iRet = get_script_number(NULL);
	iTmpNum = get_script_number(NULL);

	 pstMapInst = s_get_mapinst();
	 if (NULL == pstMapInst)
	 {
	 	set_script_variable(iRet, -1);
	 	return 0;
	 }

	 iCount = 0;
	 for (i=0; i<pstMapInst->iAreaDyn;i++)
	 {
	 	if (0 > pstMapInst->aiAreaDyn[i])
	 	{
			continue;
	 	}
		
	 	pstAreaDyn	=	(AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, pstMapInst->aiAreaDyn[i]);
		if (NULL == pstAreaDyn)
		{
			continue;
		}

		for (j=0; j<pstAreaDyn->iInanimate; j++)
		{
			pstDropItem = z_id_dropitem(pstEnv->pstAppCtx, pstEnv, pstAreaDyn->aiInanimate[j]);
			if (pstDropItem && (DROP_ITEM_TMP == pstDropItem->cType))
			{
				if (0 == iTmpID)
				{
					iCount++;
				}
				else if (pstDropItem->iDefIdx == iTmpID)
				{
					iCount++;
				}
			}
		}
	 }

	 set_script_variable(iRet, 0);
	 set_script_variable(iTmpNum, iCount);

	 return 0;
}

int s_get_id_mon_info()
{
	Monster *pstMon;
	int iInstID = get_script_number(NULL);
	
	int iRet = get_script_number(NULL);
	int iDefID = get_script_number(NULL);
	int iHP = get_script_number(NULL);
	int iMapID = get_script_number(NULL);
	int iMapInst = get_script_number(NULL);
	int iX = get_script_number(NULL);
	int iY = get_script_number(NULL);
	
	pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iInstID);
	if (pstMon == NULL)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	set_script_variable(iRet, 0);
	set_script_variable(iDefID, pstMon->iDefIdx);
	set_script_variable(iHP, pstMon->iHP);
	set_script_variable(iMapID, GET_MAPID(pstMon->stMap.iID));
	set_script_variable(iMapInst, pstMon->stMap.iID);
	set_script_variable(iX, pstMon->stCurrPos.X);
	set_script_variable(iY, pstMon->stCurrPos.Y);
	
	return 0;
}

int s_get_mon_info()
{
	Monster *pstMonster;
	
	int iRet = get_script_number(NULL);
	int iDefID = get_script_number(NULL);
	int iInstID = get_script_number(NULL);
	int iHP = get_script_number(NULL);
	int iMapID = get_script_number(NULL);
	int iMapInst = get_script_number(NULL);
	int iX = get_script_number(NULL);
	int iY = get_script_number(NULL);

	if (stSEnv.pstMonster == NULL && stSEnv.pstNpc == NULL)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	if (stSEnv.pstMonster)
	{
		pstMonster = stSEnv.pstMonster;
	}
	else if (stSEnv.pstNpc)
	{
		pstMonster = &stSEnv.pstNpc->stMon;
	}
	else
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	if (pstMonster->cMoving)
	{
		z_monster_move_curr(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMonster, 1);
	}

	set_script_variable(iRet, 0);
	set_script_variable(iDefID, pstMonster->iDefIdx);
	set_script_variable(iInstID, pstMonster->iID);
	set_script_variable(iHP, pstMonster->iHP);
	set_script_variable(iMapID, GET_MAPID(pstMonster->stMap.iID));
	set_script_variable(iMapInst, pstMonster->stMap.iID);
	set_script_variable(iX, pstMonster->stCurrPos.X);
	set_script_variable(iY, pstMonster->stCurrPos.Y);
	
	return 0;
}

int s_set_mon_buff()
{
	int iBuffID = get_script_number(NULL);
	int iBuffLevel = get_script_number(NULL);
	int iTime = get_script_number(NULL);
	int iRet = get_script_number(NULL);

	if (stSEnv.pstMonster == NULL)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	set_script_variable(iRet, 
		z_mon_apply_status(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstMonster, iBuffID, iBuffLevel, iTime, 1, 0));	
	
	return 0;
}

int s_get_item_obj_info()
{
	int iObjType;
	int iDefID;
	int iMapID;
	int iX, iY;
	int iRet;

	iObjType = get_script_number(NULL);
	iRet = get_script_number(NULL);
	iDefID = get_script_number(NULL);
	iMapID = get_script_number(NULL);
	iX = get_script_number(NULL);
	iY = get_script_number(NULL);
	

	if (iObjType == SCRIPT_TARGET_NPC)
	{
		if (!stSEnv.pstNpc)
		{
			set_script_variable(iRet, -1);
			return 0;
		}
		
		set_script_variable(iRet, 0);
		set_script_variable(iDefID, stSEnv.pstNpc->stMon.iDefIdx);
		set_script_variable(iMapID, GET_MAPID(stSEnv.pstNpc->stMon.stMap.iID));
		set_script_variable(iX, stSEnv.pstNpc->stMon.stCurrPos.X);
		set_script_variable(iY, stSEnv.pstNpc->stMon.stCurrPos.Y);
	}
	else if (iObjType == SCRIPT_TARGET_MONSTER)
	{
		if (!stSEnv.pstMonster)
		{
			set_script_variable(iRet, -1);
			return 0;
		}

		set_script_variable(iRet, 0);
		set_script_variable(iDefID, stSEnv.pstMonster->iDefIdx);
		set_script_variable(iMapID, GET_MAPID(stSEnv.pstMonster->stMap.iID));
		set_script_variable(iX, stSEnv.pstMonster->stCurrPos.X);
		set_script_variable(iX, stSEnv.pstMonster->stCurrPos.Y);
	}
	
	return 0;
}

int s_get_pet_info()
{
	/*
	int iPetID = get_script_number(NULL);
	int iLevel = get_script_number(NULL);
	ROLEPET *pstRolepet = NULL;

	if (stSEnv.pstPlayer == NULL)
	{
		set_script_variable(iLevel, 0);
		return 0;
	}

	pstRolepet = find_rolepet_by_defid(stSEnv.pstEnv, stSEnv.pstPlayer, iPetID);
	if (!pstRolepet)
	{
		set_script_variable(iLevel, 0);
		return 0;
	}
	
	set_script_variable(iLevel, pstRolepet->Level);
	*/
	
	return 0;
}

int s_add_pet_fight()
{
	/*
	int iItemID = get_script_number(NULL);
	int iRet = get_script_number(NULL);
	ITEMDEF *pstItemDef = NULL;
	RESULT *pstResult = NULL;
	ROLEPET *pstRolePet = NULL;

	if (NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	pstItemDef =  z_find_itemdef(stSEnv.pstEnv, iItemID);
	if (!pstItemDef)
	{
		goto error;
	}

	pstResult = z_item_get_result( pstItemDef,  RESULT_PET_HATCH);
	if(!pstResult)
	{
		goto error;
	}

	if ( 0 > pet_hatch_check( stSEnv.pstEnv, stSEnv.pstPlayer, pstResult, pstItemDef) )
	{
		goto error;
	}

	pstRolePet = pet_hatch(stSEnv.pstEnv, stSEnv.pstPlayer, pstResult->ResultVal1, pstResult->ResultVal2, pstResult->ResultVal3, 0);
	if (!pstRolePet)
	{
		goto error;
	}

	//临时宠物
	pstRolePet->StatFlag |= PET_STAT_TEMP;
	
	if ( 0 > pet_set_stat(stSEnv.pstEnv, stSEnv.pstPlayer, pstRolePet, PET_STAT_FIGHT) )
	{
		goto error;
	}
	
	set_script_variable(iRet, 0);
	return 0;
error:
	set_script_variable(iRet, -1);
	return 0;
	*/
	return 0;
}

int s_add_fairy_fight()
{
	int iItemID = get_script_number(NULL);
	int iRet = get_script_number(NULL);
	ITEMDEF *pstItemDef = NULL;
	RESULT *pstResult = NULL;
	ROLEFAIRY *pstRoleFairy;
	int i;
	FAIRYDEF *pstFairyDef;
	FAIRYPACKAGE *pstFairyPkg;

	if (NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	pstItemDef =  z_find_itemdef(stSEnv.pstEnv, iItemID);
	if (!pstItemDef)
	{
		goto error;
	}

	pstResult = z_item_get_result( pstItemDef,  RESULT_FAIRY_BORN);
	if(!pstResult)
	{
		goto error;
	}

	pstFairyPkg = &stSEnv.pstPlayer->stRoleData.Package.FairyPackage;

	pstFairyDef = faity_find(stSEnv.pstEnv, pstResult->ResultVal1, 1);
	if (!pstFairyDef)
	{
		return -1;
	}

	if (pstFairyPkg->FairyNum >= pstFairyPkg->FairyMaxNum)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_SCRIPT201);
		return -1;
	}

	for (i=0; i<pstFairyPkg->FairyNum; i++)
	{
		if (pstFairyPkg->RoleFairys[i].FairyID == pstResult->ResultVal1)
		{
			z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_SCRIPT202);
			return -1;
		}
	}

	pstRoleFairy = fairy_born(stSEnv.pstEnv, stSEnv.pstPlayer, pstResult->ResultVal1);
	if (!pstRoleFairy)
	{
		goto error;
	}

	pstRoleFairy->Flag |= FAIRY_CTL_FLAG_TMP;

	set_fairy_camp(stSEnv.pstEnv, stSEnv.pstPlayer, pstRoleFairy);

	set_script_variable(iRet, 0);
	return 0;
error:
	set_script_variable(iRet, -1);
	return 0;		
}

int s_is_in_pworld()
{
	ZONEPWORLDINST * pstZonePworldInst;
		
	int iPworldID = get_script_number(NULL);
	int iRet = get_script_number(NULL);
	int iSuc = get_script_number(NULL);
	int iDifficulty = get_script_number(NULL);

	if (NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	pstZonePworldInst = z_find_player_pworld_inst(stSEnv.pstEnv, stSEnv.pstPlayer);
	if ( (NULL == pstZonePworldInst)||(iPworldID != pstZonePworldInst->stPworldIdx.iID) )
	{
		set_script_variable(iRet, 0);
		set_script_variable(iSuc, 0);
		set_script_variable(iDifficulty, 0);
	}
	else
	{
		set_script_variable(iRet, 0);
		set_script_variable(iSuc, 1);
		set_script_variable(iDifficulty, pstZonePworldInst->stPworldselect.PworldGrade);
	}

	return 0;	
}

int s_add_xp()
{
	PlayerChagAttrVal stAttr;
	int iXP = get_script_number(NULL);
	int iRet = get_script_number(NULL);

	if (NULL == stSEnv.pstPlayer)
	{
		goto error;
	}

	//stSEnv.pstPlayer->stRoleData.MiscInfo.XpPrecise += iXP *100;

	stAttr.AttrList[0].nFlag = 0;
	stAttr.AttrList[0].AttrID = ATTR_ID_XP;
	stAttr.AttrList[0].Value.XP = iXP;
	stAttr.AttrNum = 1;
	z_attr_chg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, &stAttr, 1);

	/*if (stSEnv.pstPlayer->stRoleData.Map > PWORLD_MAPID_BASE)
	{
		stSEnv.pstPlayer->stRoleData.MiscInfo.Flag |= PWORLD_ITEM_ADD_XP;
	}*/

	set_script_variable(iRet, 0);
	return 0;
error:
	set_script_variable(iRet, -1);
	return 0;
}

int s_get_map_npc_id()
{
	int i, j, iDyn;
	Npc *pstNpc;
	AREADYN *pstAreaDyn; 
	ZONEMAPINST *pstMapInst = NULL;
	int iDefID = get_script_number(NULL);
	int iID = get_script_number(NULL);
	int iRet = get_script_number(NULL);

	pstMapInst = s_get_mapinst();
	if (pstMapInst == NULL)
	{
		goto error;
	}
	
	for (i=0; i<pstMapInst->iAreaDyn; i++)
	{
		iDyn	 =	pstMapInst->aiAreaDyn[i];
		if( iDyn<0)
		{
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(stSEnv.pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		for(j=0; j<pstAreaDyn->iAnimate; j++)
		{
			if( j>=MAX_AREA_ANIMATE )
			{
				tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d", pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
				break;
			}

			pstNpc = z_id_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstAreaDyn->aiAnimate[j]);
			if (pstNpc &&  pstNpc->stMon.iDefIdx == iDefID)
			{
				set_script_variable(iID, pstNpc->stMon.iID);
				goto fini;
			}
		}
	}
	
fini:
	set_script_variable(iRet, 0);
	return 0;
error:
	set_script_variable(iRet, -1);
	return 0;
}

int s_effect_id()
{
	int iEffectType;
	int iID;
	char *p;
	ZONEMAPINST *pstMapInst = NULL;
	Player *pstPlayer = stSEnv.pstPlayer;
			
	iEffectType = get_script_number(NULL);
	iID = get_script_number(NULL);
	p = get_script_string();

	if (0 == iEffectType)
	{
		pstMapInst = s_get_mapinst();
	}

	if(pstMapInst)
	{
		play_effect(stSEnv.pstEnv, pstPlayer, iEffectType, iID, p, pstMapInst);
	}
	else
	{
		play_effect(stSEnv.pstEnv, pstPlayer, iEffectType, iID, p, NULL);
	}

	return 0;
}

int s_enter_clan_city()
{
	int iRetIdx = get_script_number(NULL);
	int iRet = 0;

	if (!stSEnv.pstPlayer)
	{
		goto fatal;
	}

	iRet = player_clan_enter_city(stSEnv.pstEnv, stSEnv.pstPlayer, NULL);
	set_script_variable(iRetIdx, iRet);
	return 0;

fatal:
	set_script_variable(iRetIdx, -1);
	return 0;
}

int s_mon_forced_move_pos()
{
	//参数1	返回值
	//参数2	怪物实例id
	//参数3	坐标x
	//参数4	坐标y

	int iRetIdx = get_script_number(NULL);//返回值
	int iMonserID = get_script_number(NULL);//怪物实例id
	int iX = get_script_number(NULL);//坐标x
	int iY = get_script_number(NULL);//坐标y

	Monster* pstMonster = stSEnv.pstMonster;
	ZONEMAPINST* pstMapInst = s_get_mapinst();
	MapIndex* pstMapIdx = NULL;
	TMAPMASK *pstMask = NULL;
	RESPOS stDestPos;

	stDestPos.X = iX;
	stDestPos.Y = iY;
	
	if (iMonserID)
	{
		pstMonster = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMonserID);	
	}

	if(!pstMonster)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (!pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstMask = (TMAPMASK *)(stSEnv.pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);
	if( !IsPixelLineThrough(pstMask, pstMonster->stCurrPos.X, pstMonster->stCurrPos.Y, stDestPos.X, stDestPos.Y, maskStop) ||
		!dyn_msk_check(stSEnv.pstEnv, pstMapInst, pstMask, pstMonster->stCurrPos.X, 
				pstMonster->stCurrPos.Y, stDestPos.X, stDestPos.Y, z_get_monsterdef_misctype(stSEnv.pstEnv,pstMonster),DYN_MSK_STOP))
	{
		set_script_variable(iRetIdx, -1);
		return 0;	
	}

	// 移动Monster到指定位置	
	pstMonster->stNextPos = stDestPos;
	pstMonster->stForcedPos = stDestPos;
	pstMonster->stInitPos = stDestPos;

	pstMonster->bMoveType = MOVE_TYPE_FORCED; // 强制移动
	pstMonster->cMoving = 1;
	z_monster_move_notify(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,pstMonster, CS_MOVE_WALK);
	TV_CLONE(pstMonster->stLastMoveTv, stSEnv.pstEnv->pstAppCtx->stCurr);		

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_is_player_owner_mon()
{
	//参数1	返回值
	//参数2	怪物id
	//参数3	属主关系

	int iRetIdx = get_script_number(NULL);//返回值
	int iMemID = get_script_number(NULL);//怪物id
	int iOwnerIdx = get_script_number(NULL);//属主关系

	int iOwner = 0;
	Monster* pstMonster = stSEnv.pstMonster;
	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);		
		return 0;
	}

	if (iMemID)
	{
		pstMonster = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);	
	}
	
	if (!pstMonster)
	{
		set_script_variable(iRetIdx, -1);	
		return 0;
	}

	if (0 == strcmp(stSEnv.pstMonster->szOwner,stSEnv.pstPlayer->stRoleData.RoleName))
	{
		iOwner = 1;
	}

	
	set_script_variable(iOwnerIdx, iOwner);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_angle_pos()
{
	//参数1	返回值
	//参数2	源坐标x
	//参数3	源坐标y
	//参数4	角度
	//参数5	长度
	//参数6	目标坐标x
	//参数7	目标坐标y

	int iRetIdx = get_script_number(NULL);//返回值
	int iX = get_script_number(NULL);//源坐标x
	int iY = get_script_number(NULL);//源坐标y
	int iAngle = get_script_number(NULL);//角度
	int iLength = get_script_number(NULL);//长度
	int iDesXIdx = get_script_number(NULL);//目标坐标x
	int iDesYIdx = get_script_number(NULL);//目标坐标y

	int iDesX = iX + iLength * mycos(iAngle);
	int iDesY = iY - iLength * mysin(iAngle);	

	set_script_variable(iDesXIdx, iDesX);
	set_script_variable(iDesYIdx, iDesY);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_radian()
{
	//参数1	返回值
	//参数2	源坐标x
	//参数3	源坐标y
	//参数4	目标坐标x
	//参数5	目标坐标y
	//参数6	弧度

	int iRetIdx = get_script_number(NULL);//返回值
	int iX = get_script_number(NULL);//源坐标x
	int iY = get_script_number(NULL);//源坐标y
	int iDesX = get_script_number(NULL);//目标坐标x
	int iDesY = get_script_number(NULL);//目标坐标y
	int iRadianIdx = get_script_number(NULL);//弧度

	int iFaceAngel = FaceAngleBetween(iX, iY,iDesX, iDesY);

	set_script_variable(iRadianIdx, iFaceAngel);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_mon_forced_move()
{
	//参数1	返回值
	//参数2	怪物实例id
	//参数3	方向原点坐标x
	//参数4	方向原点坐标y
	//参数5	随机角度
	//参数6	距离

	int iRetIdx = get_script_number(NULL);//返回值
	int iMonserID = get_script_number(NULL);//怪物实例id
	int iX = get_script_number(NULL);//方向原点坐标x
	int iY = get_script_number(NULL);//方向原点坐标y
	int iRandDir = get_script_number(NULL);//随机角度
	int iDis = get_script_number(NULL);//距离

	Monster* pstMonster = stSEnv.pstMonster;
	int iFaceAngel = 0;
	ZONEMAPINST* pstMapInst = s_get_mapinst();
	RESPOS stDestPos;
	MapIndex* pstMapIdx = NULL;
	TMAPMASK *pstMask = NULL;
	
	if (iMonserID)
	{
		pstMonster = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMonserID);	
	}

	if(!pstMonster)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (!pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (pstMonster->cMoving)
	{
		z_monster_move_curr(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMonster, 1);
	}
	
	iFaceAngel = FaceAngleBetween(pstMonster->stCurrPos.X, pstMonster->stCurrPos.Y,iX, iY);
	iFaceAngel += 90;
	iFaceAngel += RAND1(iRandDir*2);
	if (iFaceAngel >= 360)
	{
		iFaceAngel -= 360;
	}

	stDestPos.X = (int)(pstMonster->stCurrPos.X + cosf(iFaceAngel*PI/180.0f)* iDis);		
	stDestPos.Y = (int)(pstMonster->stCurrPos.Y - sinf(iFaceAngel*PI/180.0f)* iDis);
	
	pstMask = (TMAPMASK *)(stSEnv.pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);
	if( !IsPixelLineThrough(pstMask, pstMonster->stCurrPos.X, pstMonster->stCurrPos.Y, stDestPos.X, stDestPos.Y, maskStop) ||
		!dyn_msk_check(stSEnv.pstEnv, pstMapInst, pstMask, pstMonster->stCurrPos.X, 
			pstMonster->stCurrPos.Y, stDestPos.X, stDestPos.Y, z_get_monsterdef_misctype(stSEnv.pstEnv,pstMonster),DYN_MSK_STOP))
	{
		set_script_variable(iRetIdx, -1);
		return 0;	
	}

	// 移动Monster到指定位置	
	pstMonster->stNextPos = stDestPos;
	pstMonster->stForcedPos = stDestPos;
	pstMonster->stInitPos = stDestPos;

	pstMonster->bMoveType = MOVE_TYPE_FORCED; // 强制移动
	pstMonster->cMoving = 1;
	z_monster_move_notify(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,pstMonster, CS_MOVE_WALK);
	TV_CLONE(pstMonster->stLastMoveTv, stSEnv.pstEnv->pstAppCtx->stCurr);		

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_new_mon_machine()
{
	//参数1	返回值
	//参数2	怪物机械DefID
	//参数3	能量
	//参数4	数量
	//参数5	坐标x
	//参数6	坐标y
	//参数7	实例id

	int iRetIdx = get_script_number(NULL);//返回值
	int iDefID = get_script_number(NULL);//怪物机械DefID
	int iEnergy = get_script_number(NULL);//能量
	int iNum = get_script_number(NULL);//数量
	int iX = get_script_number(NULL);//坐标x
	int iY = get_script_number(NULL);//坐标y
	int iMemIdx = get_script_number(NULL);//实例id

	RESPOS pos;
	Monster* pstMachine = NULL;
	int i = 0;
	int iMemID = 0;

	pos.X = iX;
	pos.Y = iY;
	
	
	stSEnv.pstMapinst = s_get_mapinst();		
	

	if (NULL == stSEnv.pstMapinst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;	
	}
	for (i = 0; i < iNum; i++)
	{
		//产出一个空燃料空弹药的机械
		pstMachine = new_mon_machine(stSEnv.pstEnv, stSEnv.pstMapinst, iDefID, &pos, MACHINE_OWNER_NONE, NULL, 0);
		if(pstMachine )
		{
			if (0 == iEnergy)
			{
				iEnergy = pstMachine->stCommUnion.stMachine.stMachineInfo.MaxEnergy;
			}
			//燃料
			z_machine_attr_chg(stSEnv.pstEnv, pstMachine, MON_MACHINE_ATTR_ENERGY, iEnergy);
			if (0 == iMemID)
			{
				iMemID = pstMachine->iID;
			}

		}

		
	}
	if (0 == iMemID)
	{
		set_script_variable(iRetIdx, 1);
		return 0;
	}
	
	set_script_variable(iMemIdx, iMemID);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_rand_pworld_player_id()
{
	//参数1	返回值
	//参数2	实例id

	int iRetIdx = get_script_number(NULL);//返回值
	int iMemIdx = get_script_number(NULL);//实例id

	int iRand = 0;
	int i = 0;
	Player* pstPlayer = NULL;
	
	if (!stSEnv.pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if (stSEnv.pstPworldInst->iPlayer < 1)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	iRand = RAND1(stSEnv.pstPworldInst->iPlayer);
	pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,stSEnv.pstPworldInst->aiPlayerID[iRand]);
	if (pstPlayer && pstPlayer->stOnline.State & CS_STAT_DEAD)
	{
		pstPlayer = NULL;
	}
	
	if (NULL == pstPlayer)
	{
		for (i=0; i<stSEnv.pstPworldInst->iPlayer; i++)
		{
			iRand++;

			if (iRand >= stSEnv.pstPworldInst->iPlayer)
				iRand = 0;
				
			pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, 
							stSEnv.pstPworldInst->aiPlayerID[iRand]);
			if (!pstPlayer)
			{
				continue;
			}

			if ( !(pstPlayer->stOnline.State & CS_STAT_DEAD))
			{
				break;
			}
		}
	}

	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
		
	}

	set_script_variable(iMemIdx, pstPlayer->iMemID);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_mon_group()
{
	//参数1	返回值
	//参数2	组号

	int iRetIdx = get_script_number(NULL);//返回值
	int iGroupIdx = get_script_number(NULL);//组号

	int iGroup = 0;
	
	if(!stSEnv.pstMonster)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iGroup = stSEnv.pstMonster->iGroupID;
	set_script_variable(iGroupIdx, iGroup);
	
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_goto_clan_city()
{
	//参数1	返回值
	//参数2	坐标x
	//参数3	坐标y

	int iRetIdx = get_script_number(NULL);
	RESPOS stPos = {
		.X = get_script_number(NULL),
		.Y = get_script_number(NULL),
	};
	int iRet;

	if (!stSEnv.pstPlayer)
	{
		goto fatal;
	}

	iRet = player_clan_goto_city(stSEnv.pstEnv, stSEnv.pstPlayer, &stPos);
	if (iRet < 0)
	{
		iRet = player_clan_enter_city(stSEnv.pstEnv, stSEnv.pstPlayer, &stPos);
	}
	set_script_variable(iRetIdx, iRet);
	return 0;

fatal:
	set_script_variable(iRetIdx, -1);
	return 0;
}

int s_clan_pay()
{
	int iType, iRetIdx, iRet, iGetIdx = 0;
	int iGet = 0;

	if(!stSEnv.pstPlayer)
	{
		goto fatal;
	}

	iType = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iGetIdx = get_script_number(NULL);

	switch(iType)
	{
	case 0:
		if (iGetIdx)
		{
			iRet = player_clan_pay(stSEnv.pstEnv, stSEnv.pstPlayer, CLAN_PAY_MONEY,&iGet);
		}
		else
		{
			iRet = player_clan_pay(stSEnv.pstEnv, stSEnv.pstPlayer, CLAN_PAY_MONEY,NULL);
		}
		break;

	case 1:
		iRet = player_clan_pay(stSEnv.pstEnv, stSEnv.pstPlayer, CLAN_PAY_EXP,NULL);
		break;

	default:
		goto fatal;
	}
	set_script_variable(iGetIdx, iGet);
	set_script_variable(iRetIdx, iRet);
	return 0;

fatal:
	set_script_variable(iRetIdx, -1);
	return 0;
}

int s_get_pack_space()
{
	int iType, iRetIdx = 0;
	int iSpace = 0;
	ROLEPACKAGE *pstPack;

	if(!stSEnv.pstPlayer)
	{
		goto fatal;
	}

	pstPack = &stSEnv.pstPlayer->stRoleData.Package;

	iType = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	if (iType == 0)			// 杂物
	{
		iSpace = pstPack->SundriesPackage.MaxNum - pstPack->SundriesPackage.Num;
	}
	else if(iType == 1)		// 任务
	{
		iSpace = pstPack->TaskPackage.MaxNum - pstPack->TaskPackage.Num;
	}
	else if(iType == 2)		// 材料
	{
		iSpace = pstPack->StuffPackage.MaxNum - pstPack->StuffPackage.Num;
	}
	else
	{
		goto fatal;
	}

	//if (iSpace < 0)
		//goto fatal;

	set_script_variable(iRetIdx, iSpace);
	return 0;

fatal:
	set_script_variable(iRetIdx, -1);
	return 0;
}

int s_get_target_info()
{
	// 参数1: 目标类型 1--player 2--npc 3--monster
	// 参数2: 获取属性 
		/*
			1--MemID  2--Hp  3-- x  4-- y  5--map 6--sex 7--Career 8--level
			9--DefID
		*/
	// 参数3: 返回值 -1为失败

	Player *pstPlayer;
	Npc *pstNpc;
	Monster *pstMon;
	int iTargeType = get_script_number(NULL);
	int iAttrType = get_script_number(NULL);
	int iRet = get_script_number(NULL);
	MONSTERDEF * pstMonDef;

	if (stSEnv.iMemID <= 0)
	{
		goto error;
	}
	
	if (iTargeType == 1)
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.iMemID);
		if (!pstPlayer)
			goto error;

		if (iAttrType == 1)
		{
			set_script_variable(iRet, stSEnv.iMemID);
		}
		else if(iAttrType == 2)
		{
			set_script_variable(iRet, pstPlayer->stRoleData.RoleDetail.HPCurr);
		}
		else if (iAttrType == 3)
		{
			set_script_variable(iRet, pstPlayer->stRoleData.Pos.X);
		}
		else if (iAttrType == 4)
		{
			set_script_variable(iRet, pstPlayer->stRoleData.Pos.Y);
		}
		else if (iAttrType == 5)
		{
			set_script_variable(iRet, pstPlayer->stRoleData.Map);
		}
		else if (iAttrType == 6)
		{
			set_script_variable(iRet, pstPlayer->stRoleData.Gender);
		}
		else if(iAttrType == 7)
		{
			set_script_variable(iRet, pstPlayer->stRoleData.Career);
		}
		else if(iAttrType == 8)
		{
			set_script_variable(iRet, pstPlayer->stRoleData.Level);
		}
		else if(iAttrType == 9)
		{
			set_script_variable(iRet, 0);
		}
		else
		{
			goto error;
		}
		
	}
	else if(iTargeType == 2)
	{
		pstNpc = z_id_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.iMemID);
		if (!pstNpc)
			goto error;

		if (iAttrType == 1)
		{
			set_script_variable(iRet, stSEnv.iMemID);
		}
		else if(iAttrType == 2)
		{
			set_script_variable(iRet, pstNpc->stMon.iHP);
		}
		else if (iAttrType == 3)
		{
			set_script_variable(iRet, pstNpc->stMon.stCurrPos.X);
		}
		else if (iAttrType == 4)
		{
			set_script_variable(iRet, pstNpc->stMon.stCurrPos.Y);
		}
		else if (iAttrType == 5)
		{
			set_script_variable(iRet, pstNpc->stMon.stMap.iID);
		}
		else if(iAttrType == 6 || iAttrType == 7 )
		{
			goto error;
		}
		else if (iAttrType == 8)
		{
			pstMonDef = z_get_mon_def(stSEnv.pstEnv, &pstNpc->stMon);
			if (!pstMonDef)
				goto error;
			set_script_variable(iRet, pstMonDef->Level);
		}
		else if(iAttrType == 9)
		{
			set_script_variable(iRet, pstNpc->stMon.iDefIdx);
		}
		else
		{
			goto error;
		}
		
	}
	else if(iTargeType == 3)
	{
		pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.iMemID);
		if (!pstMon)
			goto error;

		if (iAttrType == 1)
		{
			set_script_variable(iRet, stSEnv.iMemID);
		}
		else if(iAttrType == 2)
		{
			set_script_variable(iRet, pstMon->iHP);
		}
		else if (iAttrType == 3)
		{
			set_script_variable(iRet, pstMon->stCurrPos.X);
		}
		else if (iAttrType == 4)
		{
			set_script_variable(iRet, pstMon->stCurrPos.Y);
		}
		else if (iAttrType == 5)
		{
			set_script_variable(iRet, pstMon->stMap.iID);
		}
		else if(iAttrType == 6 || iAttrType == 7 )
		{
			goto error;
		}
		else if(iAttrType == 8)
		{
			pstMonDef = z_get_mon_def(stSEnv.pstEnv, pstMon);
			if (!pstMonDef)
				goto error;
			set_script_variable(iRet, pstMonDef->Level);
		}
		else if(iAttrType == 9)
		{
			set_script_variable(iRet, pstMon->iDefIdx);
		}
		else
		{
			goto error;
		}
		
	}
	else
	{
		goto error;
	}
	
	return 0;

error:
	set_script_variable(iRet, -1);
	return 0;
}

int s_target_add_status_by_memid()
{
	// 参数1: 目标类型 1--player 2--npc 3--monster
	// 参数2: memid
	// 参数3: statusID ID
	// 参数4: statusLV 等级
	// 参数5: statusTM 时间
	// 参数6: 返回值 0--成功 -1失败

	Player *pstPlayer;
	Npc *pstNpc;
	Monster *pstMon;
	STATUSDEF *pstStatusDef; 
	int iApplyID = 0;
	
	int iTargeType = get_script_number(NULL);
	int iMemID = get_script_number(NULL);
	int iStatusID = get_script_number(NULL);
	int iStatusLV = get_script_number(NULL);
	int iStatusTM = get_script_number(NULL);
	int iRet = get_script_number(NULL);

	pstStatusDef = z_find_status(stSEnv.pstEnv, iStatusID, iStatusLV);
	if (!pstStatusDef || iMemID <= 0)
	{
		goto error;
	}

	if(stSEnv.pstPlayer)
	{
		iApplyID = stSEnv.pstPlayer->iMemID;
 	}

	if (iTargeType == 1)
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
		if (!pstPlayer || (pstPlayer->stOnline.State & CS_STAT_DEAD))
			goto error;

		z_player_del_status(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, STATUS_ID_AREAPROTECTED, 1 , 0);

		if(iStatusTM == 0)
		{
			z_player_del_status_level(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, iStatusID, iStatusLV, 1 , 0);
		}
		else
		{
			if (z_player_apply_status(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, 
							iStatusID, iStatusLV, iStatusTM, 1, 0) < 0)
			{
				goto error;
			}
			else
			{
				if (stSEnv.iExParaType == EX_PARA_TYPE_ADD_STATUS_DYN)
				{
					ZoneStatus * pstAdd = z_player_find_status(pstPlayer,iStatusID);
					if (pstAdd && iStatusLV == pstAdd->bLevel)
					{
						pstAdd->iDynPara = stSEnv.iExParaData;		
						pstAdd->MiscType |= ZONESTATUS_MISCTYPE_DYN;
					}
				}
			}
		}
		
	}
	else if(iTargeType == 2)
	{
		pstNpc = z_id_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
		if (!pstNpc)
			goto error;

		if (z_mon_apply_status(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &pstNpc->stMon, 
							iStatusID, iStatusLV, iStatusTM, 1, iApplyID) < 0)
		{
			goto error;
		}		

		if (stSEnv.iExParaType == EX_PARA_TYPE_ADD_STATUS_DYN)
		{
			ZoneStatus * pstAdd = z_mon_find_status(&pstNpc->stMon,iStatusID);
			if (pstAdd && iStatusLV == pstAdd->bLevel)
			{
				pstAdd->iDynPara = stSEnv.iExParaData;		
				pstAdd->MiscType |= ZONESTATUS_MISCTYPE_DYN;
			}
		}
	}
	else if(iTargeType == 3)
	{
		pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
		if (!pstMon)
			goto error;

		if (z_mon_apply_status(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMon, 
							iStatusID, iStatusLV, iStatusTM, 1, iApplyID) < 0)
		{
			goto error;
		}	

		if (stSEnv.iExParaType == EX_PARA_TYPE_ADD_STATUS_DYN)
		{
			ZoneStatus * pstAdd = z_mon_find_status_by_level(pstMon,iStatusID,iStatusLV);
			if (pstAdd && iStatusLV == pstAdd->bLevel)
			{
				pstAdd->iDynPara = stSEnv.iExParaData;		
				pstAdd->MiscType |= ZONESTATUS_MISCTYPE_DYN;
			}
		}
	}
	else
	{
		goto error;
	}

	set_script_variable(iRet, 0);
	return 0;
	
error:
	set_script_variable(iRet, -1);
	return 0;
}

// 玩家是否有某种debuff
int s_player_has_status()
{
	// 参数1-- statusID
	// 参数2-- 返回值 0-- 有 -1没有

	int iStatusID = get_script_number(NULL);
	int iRet = get_script_number(NULL);
	
	if (!(stSEnv.pstPlayer) || z_player_find_status(stSEnv.pstPlayer, iStatusID) == NULL)
	{
		set_script_variable(iRet, -1);
		return 0;
	}
	
	set_script_variable(iRet, 0);
	return 0;
}

int s_target_clean_by_memid()
{
	// 参数1-- 对象类型 1-- mon 2-- npc
	// 参数2-- 对象memid
	// 参数3-- 返回值0-- 成功  -1--失败

	Npc *pstNpc;
	Monster *pstMon;
	int iTgType= get_script_number(NULL);
	int iMemID = get_script_number(NULL);
	int iRet   = get_script_number(NULL);

	if (iTgType == 1)
	{
		pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
		if (!pstMon)
			goto error;

		z_monster_leave_map(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMon);
		z_free_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMon);
		
	}
	else if(iTgType == 2)
	{
		pstNpc = z_id_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
		if (!pstNpc)
			goto error;

		z_monster_leave_map(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &pstNpc->stMon);
		z_free_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &pstNpc->stMon);
		
	}
	else
	{
		goto error;
	}

	stSEnv.iMemID = 0;
	set_script_variable(iRet, 0);
	return 0;

	
error:
	set_script_variable(iRet, -1);
	return 0;
}

int s_target_relation_by_memid()
{

	// 参数1-- memid
	// 参数2-- 返回值 -1-- 失败 0-- 朋友 1-- 师父 2-- 徒弟 3-- 配偶 4-- 兄弟 5--黑名单 6-- 仇敌 7-- 无关系

	int i;
	ROLEBUDDY *pstBuddy;
	tdr_ulonglong ullRoleID;
	ROLEMISCINFO *pstMiscInfo;
	Player *pstPlayer = NULL;
	int iMemID = get_script_number(NULL);
	int iRet =   get_script_number(NULL);

	if (iMemID != stSEnv.iMemID)
	{
		goto error;
	}
	
	pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if (!pstPlayer || !stSEnv.pstPlayer)
	{
		goto error;
	}

	pstMiscInfo = &stSEnv.pstPlayer->stRoleData.MiscInfo;
	ullRoleID = pstPlayer->stRoleData.RoleID;
	pstBuddy = &stSEnv.pstPlayer->stRoleData.Buddy;

	if (player_enemy_find_byname(pstMiscInfo, pstPlayer->stRoleData.RoleName) >= 0)
	{
		set_script_variable(iRet, 5);
		return 0;	
	}

	if (relation_enemy_online_find(&pstMiscInfo->Enemy, ullRoleID))
	{
		set_script_variable(iRet, 6);
		return 0;	
	}

	for (i=0; i<pstBuddy->BrotherNum; i++)
	{
		if (pstBuddy->Brothers[i].RoleID == ullRoleID)
		{
			set_script_variable(iRet, 4);
			return 0;	
		}
	}

	for (i=0; i<pstBuddy->MasterNum; i++)
	{
		if (pstBuddy->Master[i].RoleID == ullRoleID)
		{
			set_script_variable(iRet, 1);
			return 0;	
		}
	}

	for (i=0; i<pstBuddy->FriendNum; i++)
	{
		if (pstBuddy->Friends[i].RoleID == ullRoleID)
		{
			set_script_variable(iRet, 0);
			return 0;	
		}
	}

	for (i=0; i<pstBuddy->PrenticeNum; i++)
	{
		if (pstBuddy->Prentices[i].RoleID == ullRoleID)
		{
			set_script_variable(iRet, 2);
			return 0;	
		}
	}

	for (i=0; i<pstBuddy->SpouseNum; i++)
	{
		if (pstBuddy->Spouse[i].RoleID == ullRoleID)
		{
			set_script_variable(iRet, 3);
			return 0;	
		}
	}

	set_script_variable(iRet, 7);
	return 0;	
	
error:
	set_script_variable(iRet, -1);
	return 0;	
	
}

int s_set_svr_status()
{
	int iStatusID;
	int iStatusLevel;
	int iStatusTime;
	int iRetValIdx;
	int i;
	Player *pstPlayer = NULL;
	TMEMBLOCK *pstMemBlock;

	iStatusID = get_script_number(NULL);
	iStatusLevel = get_script_number(NULL);
	iStatusTime = get_script_number(NULL);
	iRetValIdx = get_script_number(NULL);

	if (stSEnv.pstEvent == NULL)
	{
		set_script_variable(iRetValIdx, -1);
		return -1;
	}

	if (stSEnv.pstEnv->pstWorldEvent->iStatusNum >= MAX_EVENT_STATUS)
	{
		set_script_variable(iRetValIdx, -1);
		return -1;
	}

	if (z_find_status(stSEnv.pstEnv, iStatusID, iStatusLevel) == NULL)
	{
		set_script_variable(iRetValIdx, -1);
		return -1;
	}
	
	stSEnv.pstEnv->pstWorldEvent->stStatus[stSEnv.pstEnv->pstWorldEvent->iStatusNum].iStatusID = iStatusID;
	stSEnv.pstEnv->pstWorldEvent->stStatus[stSEnv.pstEnv->pstWorldEvent->iStatusNum].iLevel= iStatusLevel;
	stSEnv.pstEnv->pstWorldEvent->stStatus[stSEnv.pstEnv->pstWorldEvent->iStatusNum].iTime= iStatusTime;
	stSEnv.pstEnv->pstWorldEvent->stStatus[stSEnv.pstEnv->pstWorldEvent->iStatusNum].EventIdx = stSEnv.pstEvent->iEventID;
	stSEnv.pstEnv->pstWorldEvent->iStatusNum++;
		
	for (i=0; i<MAX_PLAYER; i++)
	{
		int iTime = iStatusTime - (stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec - stSEnv.pstEvent->tEventStart);
		
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(stSEnv.pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (Player *)pstMemBlock->szData;
		//if (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus || pstPlayer->stOnline.bConnLost || pstPlayer->stOnline.bWaitRoleLogout)
		if (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
		{
			continue;
		}

		if(iTime > 0)
		{
			z_player_apply_status( stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, iStatusID, iStatusLevel, iTime, 1, 0);
		}
	}

	set_script_variable(iRetValIdx, 0);
	return 0;
}

int s_set_svr_bulletin()
{
	int iBulletinID;
	int iRollInterval, iActiveTime;
	char *p;
	int iRetValIdx;
	SSBULLETIN stReq;

	memset(&stReq, 0, sizeof(stReq));
	iRetValIdx = get_script_number(NULL);
	iActiveTime = get_script_number(NULL);
	iRollInterval = get_script_number(NULL);
	p = get_script_string();

	if (stSEnv.pstEvent == NULL)
	{
		set_script_variable(iRetValIdx, -1);
		return -1;
	}

	iBulletinID = z_gen_bulletin_id();
	stReq.ActiveTime = iActiveTime;
	stReq.Core.ID = iBulletinID;
	stReq.Core.RollInterval = iRollInterval;
	STRNCPY(stReq.Core.Text, p, CCH(stReq.Core.Text));

	if(z_add_bulletin(stSEnv.pstEnv, &stReq) < 0)
	{
		set_script_variable(iRetValIdx, -1);
		return -1;
	}

	stSEnv.pstEnv->pstWorldEvent->stBulletin[stSEnv.pstEnv->pstWorldEvent->iBulletinNum].iBulletinID= iBulletinID;
	stSEnv.pstEnv->pstWorldEvent->stBulletin	[stSEnv.pstEnv->pstWorldEvent->iBulletinNum].EventIdx = stSEnv.pstEvent->iEventID;
	stSEnv.pstEnv->pstWorldEvent->iBulletinNum++;

	set_script_variable(iRetValIdx, 0);
	return 0;
}

int s_get_event_stat()
{
	int iEventID;
	int iRetValIdx;
	GameEventInst stEventInst, *pstEvent;
	
	iRetValIdx = get_script_number(NULL);
	iEventID = get_script_number(NULL);

	stEventInst.iEventID = iEventID;
	pstEvent = sht_find(stSEnv.pstEnv->pstShtEvent, &stEventInst, EventHashCmp, EventHashCode);
	if (!pstEvent)
	{
		set_script_variable(iRetValIdx, 0);
		return 0;
	}

	set_script_variable(iRetValIdx, 1);
	return 0;
}

int s_get_event_exist_time()
{
	int iEventID;
	int iTime;
	GameEventInst stEventInst, *pstEvent;
	
	iTime = get_script_number(NULL);
	iEventID = get_script_number(NULL);

	stEventInst.iEventID = iEventID;
	pstEvent = sht_find(stSEnv.pstEnv->pstShtEvent, &stEventInst, EventHashCmp, EventHashCode);
	if (!pstEvent)
	{
		set_script_variable(iTime, -1);
		return 0;
	}

	set_script_variable(iTime, stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec - pstEvent->tEventStart);
	return 0;
}

int s_set_event_pworld()
{
	int iPworldID;
	int iRetValIdx;
	ZONEIDX stIdx;
	ZONEPWORLD *pstPworld;
	int i;

	iRetValIdx = get_script_number(NULL);
	iPworldID = get_script_number(NULL);
	
	if (stSEnv.pstEvent == NULL)
	{
		set_script_variable(iRetValIdx, -1);
		return 0;
	}

	stIdx.iID = iPworldID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(stSEnv.pstEnv->pstMapObj, &stIdx);
	if (pstPworld == NULL || pstPworld->stPworldDef.Type != PWORLD_TYPE_PLOY)
	{
		set_script_variable(iRetValIdx, -1);
		return 0;
	}

	//有数量限制的副本，活动开始既创建完毕
	for (i = 0; i<pstPworld->stPworldDef.PworldNum; i++)
	{
		z_new_ploy_pworld_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPworld,  pstPworld->stPworldDef.Timeout*60,0);
	}

	if(stSEnv.pstEnv->pstWorldEvent->iPworldNum >= MAX_EVENT_PWORLD)
	{
		set_script_variable(iRetValIdx, -1);
		return 0;
	}
	
	stSEnv.pstEnv->pstWorldEvent->stPworld[stSEnv.pstEnv->pstWorldEvent->iPworldNum].iPworldID = iPworldID;
	stSEnv.pstEnv->pstWorldEvent->stPworld[stSEnv.pstEnv->pstWorldEvent->iPworldNum].EventIdx = stSEnv.pstEvent->iEventID;
	stSEnv.pstEnv->pstWorldEvent->iPworldNum++;

	set_script_variable(iRetValIdx, 0);
	return 0;
}

int s_range_add_exp()
{
	int i, j, iDyn;
	int iRadius;
	tdr_longlong iExp;
	int iRetIdx;
	PlayerChagAttrVal stAttr;
	Player *pstTmpPlayer;
	Monster *pstMon = NULL;
	ZONEMAPINST *pstMapInst;
	AREADYN *pstAreaDyn;

	iRadius = get_script_number(NULL);
	iExp = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (iRadius > 0)
	{
		pstMon = stSEnv.pstMonster;
		if (NULL == pstMon)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
	}
	else
	{
		pstMon = NULL;
	}

	//全地图找一遍
	for (i=0; i<pstMapInst->iAreaDyn; i++)
	{
		iDyn	 =	pstMapInst->aiAreaDyn[i];
		if( iDyn<0)
		{
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(stSEnv.pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		for(j=0; j<pstAreaDyn->iAnimate; j++)
		{
			if( j>=MAX_AREA_ANIMATE )
			{
				tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d", pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
				break;
			}

			pstTmpPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstAreaDyn->aiAnimate[j]);
			if (NULL == pstTmpPlayer || PLAYER_STATUS_ROLE_LOGIN != pstTmpPlayer->eStatus)
			{
				continue;
			}

			//iRadius == 0 全地图获得
			if(iRadius > 0)
			{
				if (NULL == pstMon)
				{
					continue;
				}
				
				if(z_distance(&pstMon->stCurrPos, &pstTmpPlayer->stRoleData.Pos) > iRadius)
				{
					continue;
				}
			}

			z_player_addexp_test(stSEnv.pstEnv, pstTmpPlayer, &iExp);

			if (iExp > 0)
			{
				stAttr.AttrList[0].AttrID = ATTR_ID_EXP;
				stAttr.AttrList[0].Value.Exp= iExp;
				stAttr.AttrList[0].nFlag = 0;
				stAttr.AttrNum = 1;
				
				z_attr_chg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstTmpPlayer, &stAttr, 1);
				z_script_add_exp_oplog(stSEnv.pstEnv,pstTmpPlayer,iExp, stSEnv.szScript);
			}
			
		}
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_range_add_status()
{
	int i, j, iDyn;
	int iRadius;
	int iStatusID, iStatusLevel, iStatusTime;
	int iRetIdx;
	Player *pstTmpPlayer;
	Monster *pstMon = NULL;
	ZONEMAPINST *pstMapInst;
	AREADYN *pstAreaDyn;

	iRadius = get_script_number(NULL);
	iStatusID = get_script_number(NULL);
	iStatusLevel = get_script_number(NULL);
	iStatusTime = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (iRadius != 0)
	{
		pstMon = stSEnv.pstMonster;
		if (NULL == pstMon)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
	}
	
	
	//全地图找一遍
	for (i=0; i<pstMapInst->iAreaDyn; i++)
	{
		iDyn	 =	pstMapInst->aiAreaDyn[i];
		if( iDyn<0)
		{
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(stSEnv.pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		for(j=0; j<pstAreaDyn->iAnimate; j++)
		{
			if( j>=MAX_AREA_ANIMATE )
			{
				tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d", pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
				break;
			}

			pstTmpPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstAreaDyn->aiAnimate[j]);
			if (NULL == pstTmpPlayer || PLAYER_STATUS_ROLE_LOGIN != pstTmpPlayer->eStatus)
			{
				continue;
			}

			//iRadius == 0 全地图获得
			if( pstMon && iRadius != 0 &&
				z_distance(&pstMon->stCurrPos, &pstTmpPlayer->stRoleData.Pos) > iRadius)
			{
				continue;
			}

			 z_player_apply_status(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstTmpPlayer, 
	 							iStatusID, iStatusLevel, iStatusTime, 1, 0);
		}
	}
	
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_new_life_mon()
{
	int iMonID;
	int iMonNum;
	RESPOS stPos, stCurrPos;
	int iRandRadius;
	int iPatrolId;
	char cRefresh;
	int iRetIdx;
	int iMonInstIDIdx;
	int i;
	int iLifeTime;
	Monster stMonster;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	Monster *pstFind;
	MONSTERDEF *pstMonDef;
	int iDir;
	

	iMonID = get_script_number(NULL);
	iMonNum = get_script_number(NULL);
	stPos.X = get_script_number(NULL);
	stPos.Y = get_script_number(NULL);
	iRandRadius = get_script_number(NULL);
	iPatrolId = get_script_number(NULL);
	cRefresh = get_script_number(NULL);
	iDir = get_script_number(NULL);
	iLifeTime = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iMonInstIDIdx = get_script_number(NULL);

	pstMapInst = s_get_mapinst();

	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.stInitPos = stPos;
	stMonster.cRefresh	= cRefresh;

	pstMonDef = z_get_mon_def(stSEnv.pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	for (i=0; i<iMonNum; i++)
	{
		if (iDir < 0)
		{
			stMonster.bDirection = RAND1(180);
		}
		else
		{
			stMonster.bDirection = iDir%180;
		}
		stMonster.stInitPos = stCurrPos = stPos;

		if (iRandRadius > 0)
		{
			z_get_ran_pos(stSEnv.pstEnv, pstMapInst, pstMapIdx, &stMonster.stInitPos, &stMonster.stInitPos, iRandRadius, 0, &stCurrPos);
		}
		
		stMonster.stInitPos = stCurrPos;
		stMonster.stCurrPos = stCurrPos;

		pstFind =	z_new_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stMonster, pstMonDef);
	
		if( !pstFind )
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		pstFind->iPatrolID = iPatrolId;

		pstFind->cFirstBorn = 1;
		pstFind->tLifeEnd = iLifeTime > 0?stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec+iLifeTime:0;
		if (0 > z_refresh_mon(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstFind,NULL))
		{
			tmempool_free(stSEnv.pstEnv->pstMonsterPool, pstFind->iMIndex);
			set_script_variable(iRetIdx, -1);
			return 0;
		}
		pstFind->cFirstBorn = 0;

		if (iPatrolId > 0)
		{
			pstFind->bMoveType = MOVE_TYPE_PATROL;
		}

		if (0 == i)
		{
			set_script_variable(iMonInstIDIdx, pstFind->iID);
		}
	}

	set_script_variable(iRetIdx, 0);
		
	return 0;
}

/*	-lxk
	功能:刷角色所属公会怪物
*/
int s_new_clan_mon()
{
	int iMonID;
	int iMonNum;
	RESPOS stPos, stCurrPos;
	int iRandRadius;
	int iPatrolId;
	char cRefresh;
	int iRetIdx;
	int iMonInstIDIdx;
	int i;
	int iLifeTime;
	Monster stMonster;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	Monster *pstFind;
	MONSTERDEF *pstMonDef;
	int iDir;
	Player * pstPlayer = stSEnv.pstPlayer;
	ZoneClan * pstClan;
	ZONEPWORLDINST *pstZonePworldInst;
	iMonID = get_script_number(NULL);
	iMonNum = get_script_number(NULL);
	stPos.X = get_script_number(NULL);
	stPos.Y = get_script_number(NULL);
	iRandRadius = get_script_number(NULL);
	iPatrolId = get_script_number(NULL);
	cRefresh = get_script_number(NULL);
	iDir = get_script_number(NULL);
	iLifeTime = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iMonInstIDIdx = get_script_number(NULL);

	if(NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.stInitPos = stPos;
	stMonster.cRefresh	= cRefresh;

	pstMonDef = z_get_mon_def(stSEnv.pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstClan = player_get_clan(stSEnv.pstEnv,pstPlayer);
	if(NULL == pstClan)
	{
		set_script_variable(iRetIdx,-1);
		return 0;
	}	
	pstZonePworldInst = z_find_clan_city_inst(stSEnv.pstEnv,pstClan);
	if(NULL == pstZonePworldInst)
	{
		set_script_variable(iRetIdx,-1);
		return 0;
	}
	
	mon_set_pkinfo(&stMonster, MON_PK_TYPE_CLAN, pstClan->stClan.ClanProf.GID);
	stMonster.nOwnerFastID = pstClan->stClan.ClanProf.Id;
	for (i=0; i<iMonNum; i++)
	{
		if (iDir < 0)
		{
			stMonster.bDirection = RAND1(180);
		}
		else
		{
			stMonster.bDirection = iDir%180;
		}
		stMonster.stInitPos = stCurrPos = stPos;

		if (iRandRadius > 0)
		{
			z_get_ran_pos(stSEnv.pstEnv, pstMapInst, pstMapIdx, &stMonster.stInitPos, &stMonster.stInitPos, iRandRadius, 0, &stCurrPos);
		}
		
		stMonster.stInitPos = stCurrPos;
		stMonster.stCurrPos = stCurrPos;

		pstFind =	z_new_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stMonster, pstMonDef);
	
		if( !pstFind )
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		pstFind->iPatrolID = iPatrolId;

		pstFind->cFirstBorn = 1;
		pstFind->tLifeEnd = iLifeTime > 0?stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec+iLifeTime:0;
		if (0 > z_refresh_mon(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstFind,NULL))
		{
			tmempool_free(stSEnv.pstEnv->pstMonsterPool, pstFind->iMIndex);
			set_script_variable(iRetIdx, -1);
			return 0;
		}
		pstFind->cFirstBorn = 0;

		if (iPatrolId > 0)
		{
			pstFind->bMoveType = MOVE_TYPE_PATROL;
		}

		if (0 == i)
		{
			set_script_variable(iMonInstIDIdx, pstFind->iID);
		}
	}

	set_script_variable(iRetIdx, 0);
		
	return 0;
}

int get_pworld_role_num()
{
	int iRetIdx;
	int iRoleNum;

	iRoleNum = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if(stSEnv.pstPworldInst == NULL)
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}
	
	set_script_variable(iRoleNum, stSEnv.pstPworldInst->iPlayer);
	set_script_variable(iRetIdx, 0);
	
	return 0;
}


int s_get_autofly_info()
{
	int iAutoFlyPath, iAutoFlyStatus;
	int iRetIdx;
	Player *pstPlayer = stSEnv.pstPlayer;

	iAutoFlyStatus = get_script_number(NULL);
	iAutoFlyPath = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	
	if (NULL == pstPlayer)
	{
		set_script_variable(iAutoFlyStatus, 0);
		set_script_variable(iAutoFlyPath, 0);
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (CS_STAT_AUTOFLY & pstPlayer->stOnline.State)
	{
		
		set_script_variable(iAutoFlyStatus, 1);
		set_script_variable(iAutoFlyPath, pstPlayer->stRoleData.MiscInfo.AutoFlyPath);
	}
	else
	{
		set_script_variable(iAutoFlyStatus, 0);
		set_script_variable(iAutoFlyPath, 0);
	}

	set_script_variable(iRetIdx, 0);
	
	return 0;
}


int s_get_item_oneday_info()
{
	int iRetIdx;
	int iItemID;
	int iOneDayNum;
	int iMaxOneDayNum;
	Player *pstPlayer = stSEnv.pstPlayer;
	ITEMDEF *pstItemDef;
	USEITEMONEDAYINFO *pstItemOneDayInfo;
	
	iItemID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iOneDayNum = get_script_number(NULL);
	iMaxOneDayNum = get_script_number(NULL);

	if(NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstItemDef = z_find_itemdef(stSEnv.pstEnv, iItemID);
	if(NULL == pstItemDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstItemOneDayInfo = z_get_item_oneday_info(stSEnv.pstEnv, pstPlayer, iItemID);
	if(NULL ==pstItemOneDayInfo)
	{
		set_script_variable(iOneDayNum, 0);
		set_script_variable(iMaxOneDayNum, pstItemDef->MaxUseOneDay);
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	set_script_variable(iOneDayNum, pstItemOneDayInfo->OneDayNum);
	set_script_variable(iMaxOneDayNum, pstItemDef->MaxUseOneDay);
	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_item_add_oneday()
{
	int iRetIdx;
	int iItemID;
	Player *pstPlayer = stSEnv.pstPlayer;
	ITEMDEF *pstItemDef;
	
	iItemID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if(NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstItemDef = z_find_itemdef(stSEnv.pstEnv, iItemID);
	if(NULL == pstItemDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	item_add_oneday(stSEnv.pstEnv, pstPlayer, iItemID);
	
	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_get_map_mon_id()
{
	int i, j, iDyn;
	Monster *pstMon;
	AREADYN *pstAreaDyn; 
	ZONEMAPINST *pstMapInst = NULL;
	int iDefID = get_script_number(NULL);
	int iID = get_script_number(NULL);
	int iRet = get_script_number(NULL);
	
	pstMapInst = s_get_mapinst();
	if (pstMapInst == NULL)
	{
		goto error;
	}
	
	for (i=0; i<pstMapInst->iAreaDyn; i++)
	{
		iDyn	 =	pstMapInst->aiAreaDyn[i];
		if( iDyn<0)
		{
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(stSEnv.pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		for(j=0; j<pstAreaDyn->iAnimate; j++)
		{
			if( j>=MAX_AREA_ANIMATE )
			{
				tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d", pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
				break;
			}

			pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstAreaDyn->aiAnimate[j]);
			if (pstMon &&  pstMon->iDefIdx == iDefID)
			{
				set_script_variable(iID, pstMon->iID);
				goto fini;
			}
		}
	}
	
fini:
	set_script_variable(iRet, 0);
	return 0;
error:
	set_script_variable(iRet, -1);
	return 0;
}

int s_get_group_oneday_info()
{
	int iRetIdx;
	int iGroupID;
	int iOneDayNum;
	Player *pstPlayer = stSEnv.pstPlayer;
	GROUPONEDAYINFO *pstGroupOneDayInfo;
	
	iGroupID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iOneDayNum = get_script_number(NULL);

	if(NULL == pstPlayer)
	{
		goto _error;
	}

	if(iGroupID <= 0)
	{
		goto _error;
	}
	
	pstGroupOneDayInfo = group_find_oneday(pstPlayer, iGroupID);
	if(NULL == pstGroupOneDayInfo)
	{
		set_script_variable(iRetIdx, 0);
		set_script_variable(iOneDayNum, 0);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iOneDayNum, pstGroupOneDayInfo->OneDayNum);
	
	return 0;
_error:

	set_script_variable(iRetIdx, -1);
	set_script_variable(iOneDayNum, 0);
	
	return 0;
}

int s_group_add_oneday()
{
	int iRetIdx;
	int iGroupID;
	Player *pstPlayer = stSEnv.pstPlayer;
	
	iGroupID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if(NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(iGroupID)
	{
		group_add_oneday(stSEnv.pstEnv, pstPlayer, iGroupID);
	}
	
	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_task_share_test()
{
	int iTaskID = get_script_number(NULL);
	int iMemID = get_script_number(NULL);
	int iRet = get_script_number(NULL);
	Player *pstPlayer = stSEnv.pstPlayer;
	Player *pstAcceptor;
	TASKDEF *pstTaskDef;

	if(NULL == pstPlayer)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	pstAcceptor = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if(NULL == pstAcceptor)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	pstTaskDef = task_def_find(stSEnv.pstEnv, iTaskID, NULL);
	if(NULL == pstTaskDef)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	if( z_check_op_dist( stSEnv.pstEnv, pstPlayer, pstAcceptor->stRoleData.Map, 
		                    &pstAcceptor->stRoleData.Pos,
		                    TASK_SHARING_ACCEPT_DIST ) < 0 )
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, SYS_MIDDLE2,  ZONEERR_SCRIPT308, pstAcceptor->stRoleData.RoleName);
		set_script_variable(iRet, -1);
		return 0;
	}

	if( pstAcceptor->stOnline.iSharingTaskID
		&& pstAcceptor->stOnline.tSharingTime - stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec 
		   < TASK_SHARING_ACCEPT_TIME )
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SCRIPT309, pstAcceptor->stRoleData.RoleName);
		set_script_variable(iRet, -1);
		return 0;
	}

	if( task_accept_check(stSEnv.pstEnv, pstAcceptor, pstTaskDef, 0, 1) < 0 )
	{
		set_script_variable(iRet, -1);
		return 0;
	}
	
	set_script_variable(iRet, 0);
	
	return 0;
}

int s_team_task_share()
{
	int i, iCount = 0;
	TEAMMEMBER * pstMember = NULL;
	Player *pstPlayer;
	Player * apstAcceptors[MAX_TEAM_MEMBER];
	TASKDEF * pstTaskDef;
	ZoneTeam * pstZoneTeam;
	int iTaskID = get_script_number(NULL);
	int iRet = get_script_number(NULL);

	pstPlayer = stSEnv.pstPlayer;
	if(NULL == pstPlayer)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	pstTaskDef = task_def_find( stSEnv.pstEnv, iTaskID, NULL );
	if(NULL == pstTaskDef)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	pstZoneTeam = player_team_get(stSEnv.pstEnv, pstPlayer);
	if(NULL == pstZoneTeam)
	{
		set_script_variable(iRet, -1);
		return 0;
	}
	
	pstMember = pstZoneTeam->stTeamInfo.TeamMember;
	i = pstZoneTeam->stTeamInfo.MemberNum;

	while( i-- )
	{
		Player * pstAcceptor = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, 
			pstMember->MemberName );

		pstMember++;

		if( task_share_check( stSEnv.pstEnv, pstTaskDef, pstPlayer, pstAcceptor, 1 ) == 0 )
		{
			apstAcceptors[iCount++] = pstAcceptor;
		}
	}

	if( iCount > 0 )
	{
		CSPKG stPkg;
		CSTASKSVR *pstTaskSvr = &stPkg.Body.TaskSvr;
	
		pstTaskSvr->OpType = TASK_DIALOG;
		pstTaskSvr->TaskSvrData.TaskDialog.NpcID = 0;
		pstTaskSvr->TaskSvrData.TaskDialog.TaskID = iTaskID;
		pstTaskSvr->TaskSvrData.TaskDialog.Type = TASK_DIALOG_ACCEPT;
		pstTaskSvr->TaskSvrData.TaskDialog.CycleID = 0;
	
		Z_CSHEAD_INIT(&stPkg.Head, TASK_SVR);
		z_cltmsg_limit_broadcast( stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, apstAcceptors, iCount, &stPkg);
	}

	set_script_variable(iRet, 0);
	return 0;
}

int s_mon_task_area_check()
{
	int iTaskID;
	int iDist;
	int iRet;
	int iPlayerNum;
	int iCount = 0;
	Monster *pstMon =  stSEnv.pstMonster;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	//Player *pstTmpPlayer;

	iTaskID = get_script_number(NULL);
	iDist = get_script_number(NULL);
	iRet = get_script_number(NULL);
	iPlayerNum = get_script_number(NULL);

	if (NULL == pstMon)
	{
		goto _error;
	}

	if (pstMon->cMoving)
	{
		z_monster_move_curr(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMon, 0);
	}
	
	pstTaskDef = task_def_find(stSEnv.pstEnv, iTaskID, NULL);
	if(NULL == pstTaskDef)
	{
		goto _error;
	}

	//单人 
	if(pstMon->bSubType == MON_SUB_TYPE_SINGLE_CAR ||
		pstMon->bSubType == MON_SUB_TYPE_SINGLE_SAFECAR)
	{
		//僵直时间 不检查
		if (stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec > pstMon->tFreezeEnd )
		{
			Player *pstOwner;
		
			pstOwner = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMon->szOwner);
			if(NULL == pstOwner)
			{	
				goto _error;
			}

			pstTaskRunOne = task_run_find(pstOwner, pstTaskDef->ID);
			if(NULL == pstTaskRunOne || pstTaskRunOne->TaskStat != TASK_STAT_RUN)
			{
				goto _error;
			}

			if(pstOwner->stRoleData.MiscInfo.ConvoyInfo.ConvoyMonID != pstMon->iID)
			{
				goto _error;
			}

			if(pstOwner->stOnline.tRoleChgMap + CONVOY_TASK_CHG_MAP_SAFETIME >= stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec )
			{
				iCount = 1;
				goto _final;
			}

			//僵直时间 不检查
			if (stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec <= pstMon->tFreezeEnd)
			{
				iCount = 1;
				goto _final;
			}

			if(pstOwner->stOnline.cMoving)
			{
				z_player_move_curr(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstOwner, 0);
			}

			if(pstMon->stMap.iID != pstOwner->stRoleData.Map 
				|| z_distance(&pstMon->stCurrPos, &pstOwner->stRoleData.Pos) > iDist)
			{
				z_clear_convoy_info(pstOwner);
				
				pstTaskRunOne->TaskStat = TASK_STAT_FAIL;
				task_chg(stSEnv.pstEnv, pstOwner, pstTaskRunOne);
				
				tlog_info(stSEnv.pstEnv->pstLogCat, 0, 0, 
						"single convoy fail:Uin=%d RoleName=%s TaskID=%d, RolePosx:%d RolePosy:%d, RoleMapID:%d, MonMapID =%d, MonX:%d MonY:%d", 
						pstOwner->stRoleData.Uin, pstOwner->stRoleData.RoleName, pstTaskDef->ID, 
						pstOwner->stRoleData.Pos.X, pstOwner->stRoleData.Pos.Y, pstOwner->stRoleData.Map,
						pstMon->stMap.iID, pstMon->stCurrPos.X, pstMon->stCurrPos.Y);
				goto _error;
			}
		}

		iCount = 1;
	}
	//组队
	else if(pstMon->bSubType == MON_SUB_TYPE_TEAM_CAR ||
			pstMon->bSubType == MON_SUB_TYPE_TEAM_SAFECAR)
	{
		int i;
		Player *pstPlayerTmp;
		ZoneTeam *pstTeam;
		TEAMMEMBER *pstMember;
		
		pstTeam = z_idx_team(stSEnv.pstEnv, pstMon->stTeamInfo.iTeamIdx, pstMon->stTeamInfo.ullTeamID);
		if(NULL == pstTeam)
		{
			goto _error;
		}

		for(i = 0; i<pstTeam->stTeamInfo.MemberNum; i++)
		{
			pstMember = &pstTeam->stTeamInfo.TeamMember[i];
			pstPlayerTmp = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMember->MemberName);
			if (!pstPlayerTmp || pstPlayerTmp->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				continue;
			}

			pstTaskRunOne = task_run_find(pstPlayerTmp, pstTaskDef->ID);
			if(NULL == pstTaskRunOne || pstTaskRunOne->TaskStat != TASK_STAT_RUN)
			{
				continue;
			}

			if(pstPlayerTmp->stRoleData.MiscInfo.ConvoyInfo.ConvoyMonID != pstMon->iID)
			{
				continue;
			}

			if(pstPlayerTmp->stOnline.tRoleChgMap + CONVOY_TASK_CHG_MAP_SAFETIME >= stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec )
			{
				iCount++;
				continue;
			}

			//僵直时间 不检查
			if (stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec <= pstMon->tFreezeEnd)
			{
				iCount++;
				continue;
			}

			if(pstPlayerTmp->stOnline.cMoving)
			{
				z_player_move_curr(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayerTmp, 0);
			}
			
			if(pstMon->stMap.iID != pstPlayerTmp->stRoleData.Map ||
				z_distance(&pstMon->stCurrPos, &pstPlayerTmp->stRoleData.Pos) > iDist)
			{
				z_clear_convoy_info(pstPlayerTmp);
				
				pstTaskRunOne->TaskStat = TASK_STAT_FAIL;
				task_chg(stSEnv.pstEnv, pstPlayerTmp, pstTaskRunOne);

				tlog_info(stSEnv.pstEnv->pstLogCat, 0, 0, 
						"team convoy fail:Uin=%d RoleName=%s TaskID=%d, RolePosx:%d RolePosy:%d, RoleMapID:%d, MonMapID =%d, MonX:%d MonY:%d", 
						pstPlayerTmp->stRoleData.Uin, pstPlayerTmp->stRoleData.RoleName, pstTaskDef->ID, 
						pstPlayerTmp->stRoleData.Pos.X, pstPlayerTmp->stRoleData.Pos.Y, pstPlayerTmp->stRoleData.Map,
						pstMon->stMap.iID, pstMon->stCurrPos.X, pstMon->stCurrPos.Y);
				
				continue;
			}	

			iCount++;
		}
	}
	else
	{
		goto _error;
	}

_final:
	
	set_script_variable(iRet, 0);
	set_script_variable(iPlayerNum, iCount);
	
	return 0;
	
_error:
	
	set_script_variable(iRet, -1);
	set_script_variable(iPlayerNum, 0);

	return 0;
}

int get_convoy_mon_map_id(ZONESVRENV *pstEnv, MapIndex *pstMapIdx, Monster *pstMon)
{
	int iLine = 0;
	int iLineMapID;
	int iMapID = pstMapIdx->stMapDef.MapID;
	
	if(pstMon->bSubType == MON_SUB_TYPE_SINGLE_CAR ||
		pstMon->bSubType == MON_SUB_TYPE_SINGLE_SAFECAR)
	{
		Player *pstOwner;
		ZoneTeam *pstTeam;
	
		pstOwner = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMon->szOwner);
		if(NULL== pstOwner)
		{	
			return iMapID;
		}

		//尽量人、车、队伍在一起
		pstTeam = player_team_get(pstEnv, pstOwner);
		if(pstTeam)
		{
			pstOwner->stRoleData.MiscInfo.MapLine = pstTeam->iTeamMapLine;
		}

		iLine = pstOwner->stRoleData.MiscInfo.MapLine;
	}
	else if(pstMon->bSubType == MON_SUB_TYPE_TEAM_CAR ||
			pstMon->bSubType == MON_SUB_TYPE_TEAM_SAFECAR)
	{
		ZoneTeam *pstTeam;
		
		pstTeam = z_idx_team(stSEnv.pstEnv, pstMon->stTeamInfo.iTeamIdx, pstMon->stTeamInfo.ullTeamID);
		if(NULL == pstTeam)
		{
			return iMapID;
		}

		iLine = pstTeam->iTeamMapLine;
	}

	iLineMapID = z_find_map_byline(pstMapIdx, iLine);
	if(iLineMapID > 0)
	{
		iMapID = iLineMapID;
	}

	return iMapID;
}

int s_mon_chg_map()
{
	int iFreezeTime;
	int iMapID;
	int iPatrolId;
	int iRet;
	RESPOS stPos;
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst = NULL;
	Monster *pstMon =  stSEnv.pstMonster;
	MapIndex *pstMapIdx;
	
	iMapID = get_script_number(NULL);
	stPos.X = get_script_number(NULL);
	stPos.Y = get_script_number(NULL);
	iPatrolId = get_script_number(NULL);
	iFreezeTime = get_script_number(NULL);
	iRet = get_script_number(NULL);

	if (NULL == pstMon)
	{
		goto _error;
	}

	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, NULL, iMapID);
	if(NULL == pstMapIdx)
	{
		goto _error;
	}

	//多线地图
	if(is_refer_line_map(pstMapIdx))
	{
		iMapID = get_convoy_mon_map_id(stSEnv.pstEnv, pstMapIdx, pstMon);
	}

	stIdx.iID	 =	iMapID;
	stIdx.iPos =	-1;
	pstMapInst = z_find_map_inst( stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		goto _error;
	}

	if (NULL == z_pos_area_dyn(pstMapIdx, stSEnv.pstEnv->pstAreaDynPool, pstMapInst, &stPos))
	{
		goto _error;
	}

	if (0 == z_valid_pos(stSEnv.pstEnv->pstMapObj, iMapID, &pstMapInst->iMapPos, &stPos))
	{
		goto _error;
	}

	iRet = z_monster_leave_map(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMon);

	//开始新的巡逻路径
	pstMon->iPatrolID = iPatrolId;
	pstMon->bPatrolPos = 0;
	pstMon->bPatrolTurn = 0;
	pstMon->cPatrolScript = 0;

	//僵直时间
	pstMon->tFreezeEnd =stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec + iFreezeTime;
	
	iRet = z_safe_monster_entermap(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMon, iMapID, &stPos);
	
	set_script_variable(iRet, 0);
	return 0;
_error:
	
	set_script_variable(iRet, -1);
	return 0;
}

//怪物找唤怪物 ，出生攻击目标是自己
int s_mon_call_enemy_mon()
{
	int iMonID;
	int iMonNum;
	int iDynLevel;
	int iDynGrade;
	RESPOS stPos;
	int iRandRadius;
	int iPatrolId;
	char cRefresh;
	int iRetIdx;
	int iMonInstIDIdx;
	int i;
	Monster stMonster;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	Monster *pstFind;
	MONSTERDEF *pstMonDef;
	int iDir, iLifeTime;
	MONDYNATTRDEF *pstMonDynAttr;

	iMonID = get_script_number(NULL);
	iMonNum = get_script_number(NULL);
	iDynLevel = get_script_number(NULL);
	iDynGrade = get_script_number(NULL);
	stPos.X = get_script_number(NULL);
	stPos.Y = get_script_number(NULL);
	iRandRadius = get_script_number(NULL);
	iPatrolId = get_script_number(NULL);
	cRefresh = get_script_number(NULL);
	iDir = get_script_number(NULL);
	iLifeTime = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iMonInstIDIdx = get_script_number(NULL);

	pstMapInst = s_get_mapinst();

	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.stInitPos = stPos;
	stMonster.cRefresh	= cRefresh;

	if(stSEnv.pstMonster)
	{
		stMonster.iCallMonID = stSEnv.pstMonster->iID;
	}

	pstMonDef = z_get_mon_def(stSEnv.pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	//动态属性
	pstMonDynAttr = z_find_mondyn(stSEnv.pstEnv, iDynLevel);
	
	for (i=0; i<iMonNum; i++)
	{
		if (iDir < 0)
		{
			stMonster.bDirection = RAND1(180);
		}
		else
		{
			stMonster.bDirection = iDir%180;
		}

		if (iRandRadius > 0)
		{
			z_get_ran_pos(stSEnv.pstEnv, pstMapInst, pstMapIdx, &stMonster.stInitPos, &stMonster.stInitPos, iRandRadius, 0, &stPos);
		}
		
		stMonster.stInitPos = stPos;
		stMonster.stCurrPos = stPos;

		pstFind =	z_new_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stMonster, pstMonDef);
	
		if( !pstFind )
		{
			set_script_variable(iRetIdx, -1);
			return -1;
		}

		if (pstMonDynAttr && iDynGrade < MAX_DYN_GRADE)
		{
			pstFind->stMonDynAttr.Level = pstMonDynAttr->Level;
			pstFind->stMonDynAttr.HPMul = pstMonDynAttr->DynHPMul[iDynGrade];
			pstFind->stMonDynAttr.DynHurtMul = pstMonDynAttr->DynHurtMul[iDynGrade];
			pstFind->stMonDynAttr.IceHurtMul = pstMonDynAttr->IceHurtMul;
			pstFind->stMonDynAttr.PhsicHurtMul = pstMonDynAttr->PhsicHurtMul;
		}

		pstFind->iPatrolID = iPatrolId;

		pstFind->cFirstBorn = 1;
		pstFind->tLifeEnd = iLifeTime > 0?stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec+iLifeTime:0;

		if (0 > z_refresh_mon(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstFind,NULL))
		{
			tmempool_free(stSEnv.pstEnv->pstMonsterPool, pstFind->iMIndex);
			set_script_variable(iRetIdx, -1);
			return -1;
		}
		
		pstFind->cFirstBorn = 0;
		
		if (iPatrolId > 0)
		{
			pstFind->bMoveType = MOVE_TYPE_PATROL;
		}

		if (0 == i)
		{
			set_script_variable(iMonInstIDIdx, pstFind->iID);
		}
	}

	set_script_variable(iRetIdx, 0);
		
	return 0;
}

int s_set_mon_task_stat()
{
	Player *pstOwner;
	Monster *pstMon = stSEnv.pstMonster;
	TASKRUNONE *pstTaskRunOne;
	int iTaskID = get_script_number(NULL);
	int iStat  = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	if(NULL == pstMon)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(iStat < TASK_STAT_RUN || iStat > TASK_STAT_COMMIT)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(pstMon->bSubType == MON_SUB_TYPE_SINGLE_CAR || pstMon->bSubType == MON_SUB_TYPE_SINGLE_SAFECAR)
	{
		pstOwner = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMon->szOwner);
		if(NULL == pstOwner)
		{	
			set_script_variable(iRetIdx, 0);
			return 0;
		}

		pstTaskRunOne = task_run_find(pstOwner, iTaskID);
		if(NULL == pstTaskRunOne ||
			pstTaskRunOne->TaskStat == iStat  ||
			pstTaskRunOne->TaskStat != TASK_STAT_RUN)
		{
			set_script_variable(iRetIdx, 0);
			return 0;
		}

		pstTaskRunOne->TaskStat = iStat;
		task_chg(stSEnv.pstEnv, pstOwner, pstTaskRunOne);
	}
	else if(pstMon->bSubType == MON_SUB_TYPE_TEAM_CAR || pstMon->bSubType == MON_SUB_TYPE_TEAM_SAFECAR)
	{
		int i;
		Player *pstPlayerTmp;
		ZoneTeam *pstTeam;
		TEAMMEMBER *pstMember;
		
		pstTeam = z_idx_team(stSEnv.pstEnv, pstMon->stTeamInfo.iTeamIdx, pstMon->stTeamInfo.ullTeamID);
		if(NULL == pstTeam)
		{
			set_script_variable(iRetIdx, 0);
			return 0;
		}

		for(i = 0; i<pstTeam->stTeamInfo.MemberNum; i++)
		{
			pstMember = &pstTeam->stTeamInfo.TeamMember[i];
			pstPlayerTmp = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMember->MemberName);
			if (!pstPlayerTmp || pstPlayerTmp->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				continue;
			}

			pstTaskRunOne = task_run_find(pstPlayerTmp, iTaskID);
			if(NULL == pstTaskRunOne || 
				pstTaskRunOne->TaskStat == iStat ||
				pstTaskRunOne->TaskStat != TASK_STAT_RUN)
			{
				continue;
			}

			pstTaskRunOne->TaskStat = iStat;
			task_chg(stSEnv.pstEnv, pstPlayerTmp, pstTaskRunOne);
		}
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_team_msg_info()
{
	CSPKG *pstPkg = &stSEnv.stDlgPkg;
	ZoneTeam *pstTeam;
		
	if (!stSEnv.pstPlayer)
	{
		return 0;
	}

	pstTeam = player_team_get(stSEnv.pstEnv, stSEnv.pstPlayer);
	if(NULL == pstTeam)
	{
		return 0;
	}

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	player_team_broadcast(stSEnv.pstEnv, pstTeam, pstPkg);
	
	return 0;
}

int s_new_dart_car()
{
	int iRetIdx;
	int iRet;
	int iLeft;
	
	iRetIdx = get_script_number(NULL);
	iLeft = get_script_number(NULL);
	
	if (!stSEnv.pstPlayer || !stSEnv.pstEnv)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	iRet = new_mon_dart_car_by_task(stSEnv.pstEnv, NULL, stSEnv.pstPlayer, 
									DART_CAR_REFRESH_TYPE_GETTASK_RAND, iLeft);

	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_resume_dart_car()
{
	int iRetIdx;
	int iRet;
	int iUse;

	iRetIdx = get_script_number(NULL);
	iUse = get_script_number(NULL);
	
	if (!stSEnv.pstPlayer || !stSEnv.pstEnv)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iRet = player_resume_dart_car(stSEnv.pstEnv, stSEnv.pstPlayer, iUse);

	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_get_pre_car()
{
	int iRetIdx;
	int iPre;
	Monster stMon;
	MONSTERDEF *pstMonDef;

	iRetIdx = get_script_number(NULL);
	iPre = get_script_number(NULL);

	if (!stSEnv.pstPlayer || !stSEnv.pstEnv)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	stMon.iDefIdx = stSEnv.pstPlayer->stRoleData.MiscInfo.DartCarInfo.PreID;
	stMon.iDefPos = -1;

	pstMonDef = z_get_mon_def(stSEnv.pstEnv, &stMon);
	if (!pstMonDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iPre, pstMonDef->DartCarType);
	return 0;
}

int s_reset_dart_car()
{
	int iRetIdx;
	int iRet;
	
	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer || !stSEnv.pstEnv)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iRet = player_reset_dart_car(stSEnv.pstEnv, stSEnv.pstPlayer);
	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_use_item_is_bind()
{
	int iRet;
	int iBind;

	iRet = get_script_number(NULL);
	iBind = get_script_number(NULL);

	if (!stSEnv.pstRoleItem)
	{
		set_script_variable(iRet, -1);
		return 0;	
	}
	else
	{
		set_script_variable(iRet, 0);
		set_script_variable(iBind, stSEnv.pstRoleItem->InstFlag&INST_ITEM_BIND);
		return 0;	
	}
}

int s_clan_is_open_dart()
{
	int iRet;
	int iOpen;
	ZoneClan *pstClan;

	iRet = get_script_number(NULL);
	iOpen = get_script_number(NULL);

	if (!stSEnv.pstPlayer || !stSEnv.pstEnv)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	pstClan = player_get_clan(stSEnv.pstEnv, stSEnv.pstPlayer);
	if (!pstClan)
	{
		set_script_variable(iRet, -1);
		return 0;	
	}

	// 今天是否开启过
	set_script_variable(iOpen, IsSameDay(pstClan->stClan.ClanAdvance.DartInfo.StartTime,
										stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec));
	
	set_script_variable(iRet, 0);
	return 0;
}

int s_buy_hetong()
{
	int iID;
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstRes = &stPkg.Body.PlayerMiscSvr;
	CSBUYHETONG *pstBuy = &pstRes->PlayerMiscSvrData.BuyHetong;

	iID = get_script_number(NULL);


	if (!stSEnv.pstPlayer || !stSEnv.pstEnv || iID <= 0)
	{
		return 0;
	}


	pstRes->PlayerMiscType = PLAYER_SVR_BUY_HETONG;

	pstBuy->HetongID = iID;
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(stSEnv.pstEnv, stSEnv.pstPlayer, &stPkg);
	return 0;
}

int s_get_dart_car_type()
{
	int iRetIdx;
	int iType;
	//int iRet;
	Player *pstPlayer;
	Monster *pstMon;
	MONSTERDEF *pstMonDef;
	
	iRetIdx = get_script_number(NULL);
	iType = get_script_number(NULL);

	if (!stSEnv.pstPlayer || !stSEnv.pstEnv)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPlayer = stSEnv.pstPlayer;

	pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer->stRoleData.MiscInfo.DartCarInfo.MonMemID);
	if (pstMon)
	{
		pstMonDef = z_get_mon_def(stSEnv.pstEnv, pstMon);
		if (pstMonDef)
		{
			set_script_variable(iRetIdx, 0);
			set_script_variable(iType, pstMonDef->DartCarType);
			return 0;
		}
	}
	set_script_variable(iRetIdx, -1);
	return 0;
}


// 判断角色是不是在自己的镖车上，顺便返回镖车类型
int s_in_dart_car()
{
	int iRetIdx;
	int iType;
	int iIn;
	Player *pstPlayer;
	Monster *pstMon;
	MONSTERDEF *pstMonDef;
	
	iRetIdx = get_script_number(NULL);
	iIn = get_script_number(NULL);
	iType = get_script_number(NULL);

	if (!stSEnv.pstPlayer || !stSEnv.pstEnv)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPlayer = stSEnv.pstPlayer;

	pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer->stRoleData.MiscInfo.DartCarInfo.MonMemID);
	if (pstMon )
	{
		pstMonDef = z_get_mon_def(stSEnv.pstEnv, pstMon);
		if (	pstMonDef && 
			pstMon->stCommUnion.stMachine.stMachineInfo.PassengerID[0] == pstPlayer->iMemID)
		{
			set_script_variable(iRetIdx, 0);
			set_script_variable(iIn, 1);
			set_script_variable(iType, pstMonDef->DartCarType);
			return 0;
		}
	}

	set_script_variable(iRetIdx, -1);
	return 0;
}

int s_reset_dart_car_check()
{
	int iRetIdx;
	TASKRUNONE *pstOne;

	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer || !stSEnv.pstEnv)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstOne = player_get_dart_car_task(stSEnv.pstEnv, stSEnv.pstPlayer);
	if (!pstOne || pstOne->TaskStat != TASK_STAT_RUN)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	set_script_variable(iRetIdx, 0);
	return 0;
}

int  s_id_player_base_att()
{
	int iPlayerID;
	int iRetIdx;
	int iGender;
	int iLevel;
	int iCareer;
	Player *pstPlayer;

	iPlayerID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iGender = get_script_number(NULL);
	iLevel = get_script_number(NULL);
	iCareer = get_script_number(NULL);
	
	if (0 == iPlayerID)
	{
		pstPlayer = stSEnv.pstPlayer;
	}
	else
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iPlayerID);
	}
	
	if (NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	set_script_variable(iRetIdx, 0);
	set_script_variable(iGender, pstPlayer->stRoleData.Gender);
	set_script_variable(iLevel, pstPlayer->stRoleData.Level);
	set_script_variable(iCareer, pstPlayer->stRoleData.Career);

	return 0;
}

int s_new_convoy_mon()
{
	int iMonID;
	RESPOS stPos;
	int iRandRadius;
	int iPatrolId;
	char cRefresh;
	int iRetIdx;
	int iMonInstIDIdx;
	int iLifeTime;
	int iDir;
	int iTaskID;
	int i;
	Monster stMonster;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	Monster *pstFind;
	MONSTERDEF *pstMonDef;
	char szMsg[256];
	ZoneTeam *pstTeam = NULL;
	Player *pstPlayer = stSEnv.pstPlayer;

	iMonID = get_script_number(NULL);
	stPos.X = get_script_number(NULL);
	stPos.Y = get_script_number(NULL);
	iRandRadius = get_script_number(NULL);
	iPatrolId = get_script_number(NULL);
	cRefresh = get_script_number(NULL);
	iDir = get_script_number(NULL);
	iLifeTime = get_script_number(NULL);
	iTaskID  = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iMonInstIDIdx = get_script_number(NULL);

	pstMapInst = s_get_mapinst();

	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0,  "MapInst NULL");
		return 0;
	}

	if(NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		set_script_variable(iRetIdx, -1);
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0,  "pstMapIdx NULL");
		return 0;
	}

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.stInitPos = stPos;
	stMonster.cRefresh	= cRefresh;

	pstMonDef = z_get_mon_def(stSEnv.pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	//单人镖车
	if(pstMonDef->SubType == MON_SUB_TYPE_SINGLE_CAR ||
		pstMonDef->SubType == MON_SUB_TYPE_SINGLE_SAFECAR)
	{
		STRNCPY(stMonster.szOwner, pstPlayer->stRoleData.RoleName, sizeof(stMonster.szOwner));

		if(pstMonDef->SubType == MON_SUB_TYPE_SINGLE_CAR)
		{
			mon_set_pkinfo(&stMonster, MON_PK_TYPE_PLAYER, pstPlayer->iMemID);
		}
		else
		{
			mon_set_pkinfo(&stMonster, MON_PK_TYPE_PROTECT, 0);
		}
	}
	//队伍镖车
	else if(pstMonDef->SubType == MON_SUB_TYPE_TEAM_CAR || 
			pstMonDef->SubType == MON_SUB_TYPE_TEAM_SAFECAR)
	{
		pstTeam = player_team_get(stSEnv.pstEnv, pstPlayer);
		if(NULL == pstTeam)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		stMonster.stTeamInfo.iTeamIdx = pstTeam->iTeamIdx;
		stMonster.stTeamInfo.ullTeamID = pstTeam->stTeamInfo.TeamID;
		
		snprintf(szMsg, sizeof(szMsg), "%s的队伍", pstPlayer->stRoleData.RoleName);
		STRNCPY(stMonster.szOwner, szMsg, sizeof(stMonster.szOwner));

		if(pstMonDef->SubType == MON_SUB_TYPE_TEAM_CAR)
		{
			mon_set_pkinfo(&stMonster, MON_PK_TYPE_TEAM, pstTeam->stTeamInfo.TeamID);
		}
		else
		{
			mon_set_pkinfo(&stMonster, MON_PK_TYPE_PROTECT, 0);
		}
	}
	else
	{
		return -1;
	}

	if (iDir < 0)
	{
		iDir = RAND1(180);
	}
	
	stMonster.bDirection = iDir%180;

	if (iRandRadius > 0)
	{
		z_get_ran_pos(stSEnv.pstEnv, pstMapInst, pstMapIdx, &stMonster.stInitPos, &stMonster.stInitPos, iRandRadius, 0, &stPos);
	}
	
	stMonster.stInitPos = stPos;
	stMonster.stCurrPos = stPos;

	pstFind =	z_new_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stMonster, pstMonDef);

	if( !pstFind )
	{
		set_script_variable(iRetIdx, -1);
		sprintf(szMsg, "NEW MON FAIL  %d", iMonID);
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0,  szMsg);
		return 0;
	}

	pstFind->iPatrolID = iPatrolId;

	pstFind->cFirstBorn = 1;
	pstFind->tLifeEnd = iLifeTime > 0?stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec+iLifeTime:0;
	if (0 > z_refresh_mon(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstFind,NULL))
	{
		tmempool_free(stSEnv.pstEnv->pstMonsterPool, pstFind->iMIndex);
		set_script_variable(iRetIdx, -1);
		sprintf(szMsg, "z_refresh_mon FAIL  %d", iMonID);
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0,  szMsg);
		return 0;
	}
	
	pstFind->cFirstBorn = 0;
	
	//出生僵直时间利用出生时间
	pstFind->tFreezeEnd =stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec + pstMonDef->BirthTime;

	if(pstTeam && (pstMonDef->SubType == MON_SUB_TYPE_TEAM_CAR || pstMonDef->SubType == MON_SUB_TYPE_TEAM_SAFECAR))
	{
		Player *pstTmpPlayer;
		
		//队伍信息记录 本队的镖车
		pstTeam->stTeamInfo.TeamMonID = pstFind->iID;

		//个人身上记录镖车
		for(i = 0; i<pstTeam->stTeamInfo.MemberNum; i++)
		{
			pstTmpPlayer = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, 
				pstTeam->stTeamInfo.TeamMember[i].MemberName);
			
			if (!pstTmpPlayer || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				continue;
			}

			pstTmpPlayer->stRoleData.MiscInfo.ConvoyInfo.TaskID = iTaskID;
			pstTmpPlayer->stRoleData.MiscInfo.ConvoyInfo.ConvoyMonID = pstFind->iID;
		}
	}
	else if (pstMonDef->SubType == MON_SUB_TYPE_SINGLE_CAR || pstMonDef->SubType == MON_SUB_TYPE_SINGLE_SAFECAR)
	{
		pstPlayer->stRoleData.MiscInfo.ConvoyInfo.TaskID = iTaskID;
		pstPlayer->stRoleData.MiscInfo.ConvoyInfo.ConvoyMonID = pstFind->iID;
	}

	if (iPatrolId > 0)
	{
		pstFind->bMoveType = MOVE_TYPE_PATROL;
	}
	
	stSEnv.iMemID = pstFind->iID;
	set_script_variable(iMonInstIDIdx, pstFind->iID);
	

	set_script_variable(iRetIdx, 0);
		
	return 0;
}

int s_use_treasure_pic()
{
	Player *pstPlayer;
	int iRet = 0;
	int iItemID = 0;
	int iRadius = 0;
	int iDirection = 0;
	int iMapID = 0;
	int x = 0;
	int y = 0;
	ROLEITEM *pstRoleItem = NULL;
	ZONEMAPINST *pstMapInst = NULL;
	MapIndex *pstMapIdx = NULL;
	int iPointDist = 0;
	int iDir = 0;
	RESPOS ItemPos; 
	int iRealMapID = 0;
	/*
	  参数: 物品id,策划定义的半径,返回值,方向,地图id,物品实例的x坐标，y坐标
	  返回值：0表示在规定的范围内. 1表示不在这个地图里,2一屏内,3一屏~二屏,4二屏以上,
	  方向：1--8表示8个方向. 直接返回角度了
	*/
	iItemID = get_script_number(NULL);
	iRadius =  get_script_number(NULL);
	iRet = get_script_number(NULL);
	iDirection = get_script_number(NULL);
	iMapID = get_script_number(NULL);
	x = get_script_number(NULL);
	y = get_script_number(NULL);

	pstPlayer = stSEnv.pstPlayer;
	if(NULL == pstPlayer)
	{
		set_script_variable(iRet, -1);
		return 0;
	}
	pstRoleItem =  stSEnv.pstRoleItem;
	if( NULL == pstRoleItem )
	{
		set_script_variable(iRet, -1);
		return 0;
	}
	//使用的物品id 和 传进来的物品id不一样,脚本写错了
	if(iItemID != stSEnv.pstRoleItem->DefID)
	{
		set_script_variable(iRet, -1);
		return 0;
	}
	
	set_script_variable(x,  pstRoleItem->InstVal2);
	set_script_variable(y,  pstRoleItem->InstVal3);
	
	/* 判断下是否在这个地图里 */
	pstMapInst = s_get_mapinst();
	if (pstMapInst == NULL)
	{
		set_script_variable(iRet, -1);
		return 0;
	}
	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		set_script_variable(iRet, -1);
		return -1;
	}	

	//多线地图
	if(pstMapIdx->stMapDef.MapLine > 0 && pstMapIdx->stMapDef.ReferMapID > 0)
	{
		iRealMapID = pstMapIdx->stMapDef.ReferMapID;
	}
	else
	{
		iRealMapID = pstMapIdx->stMapDef.MapID;
	}

	set_script_variable(iMapID,  iRealMapID);
	
	// Flag 存储的是地图的ID
	if( pstRoleItem->Flag != iRealMapID)
	{
		set_script_variable(iRet, 1);
		return 0;
	}
	
	// 先移动下
	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, 0);
	}

	ItemPos.X = pstRoleItem->InstVal2;
	ItemPos.Y = pstRoleItem->InstVal3;
	iPointDist = z_distance(&pstPlayer->stRoleData.Pos,&ItemPos);
	// 宝藏在规定的范围内
	if( iPointDist <= iRadius )
	{
		set_script_variable(iRet, 0);
		return 0;
	}
	else if(iPointDist < SCREEN_XGRID)
	{
		// 2一屏内
		set_script_variable(iRet, 2);
	}
	else if(iPointDist < 2*SCREEN_XGRID)
	{
		// 3一屏~二屏
		set_script_variable(iRet, 3);
	}
	else
	{
		// 4二屏以上
		set_script_variable(iRet, 4);
	}
	
	// 来判断下距离 
	iDir	=	FaceAngleBetween(ItemPos.X,  ItemPos.Y,
					pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y);
	/* 弄出八个方向来 */
	if( iDir < 67.5 &&  iDir >= 22.5 )
	{
		iDir = 8;//西北
	}
	else if( iDir <112.5 &&  iDir >= 67.5 )
	{
		iDir = 1;//北
	}
	else if( iDir <157.5 &&  iDir >= 102.5 )
	{
		iDir = 2;//东北
	}
	else if( iDir <202.5 &&  iDir >= 157.5 )
	{
		iDir = 3;//东
	}
	else if( iDir <247.5 &&  iDir >= 202.5 )
	{
		iDir = 4;//东南
	}
	else if( iDir <292.5 &&  iDir >= 247.5 )
	{
		iDir = 5;//南
	}
	else if( iDir <337.5 &&  iDir >= 292.5 )
	{
		iDir = 6;//西南
	}
	else 
	{
		iDir = 7;//西
	}
	
	set_script_variable(iDirection, iDir);
	
	return 0;
}

int s_new_map_mon()
{
	int iMonID;
	int iMonNum;
	int iMapID;
	RESPOS stPos, stCurrPos;
	int iRandRadius;
	int iPatrolId;
	char cRefresh;
	int iRetIdx;
	int iMonInstIDIdx;
	int i;
	Monster stMonster;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	Monster *pstFind;
	MONSTERDEF *pstMonDef;
	int iDir, iLifeTime;
	ZONEIDX stIdx;

	iMonID = get_script_number(NULL);
	iMonNum = get_script_number(NULL);
	iMapID = get_script_number(NULL);
	stPos.X = get_script_number(NULL);
	stPos.Y = get_script_number(NULL);
	iRandRadius = get_script_number(NULL);
	iPatrolId = get_script_number(NULL);
	cRefresh = get_script_number(NULL);
	iDir = get_script_number(NULL);
	iLifeTime = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iMonInstIDIdx = get_script_number(NULL);

	stIdx.iID	=	iMapID;
	stIdx.iPos	=	-1;
	pstMapInst	=	z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stIdx);
	if(!pstMapInst)
		return 0;
	
	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.stInitPos = stPos;
	stMonster.cRefresh	= cRefresh;

	pstMonDef = z_get_mon_def(stSEnv.pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	
	
	for (i=0; i<iMonNum; i++)
	{
		if (iDir < 0)
		{
			stMonster.bDirection = RAND1(180);
		}
		else
		{
			stMonster.bDirection = iDir%180;
		}
		
		stMonster.stInitPos = stCurrPos = stPos;
		
		if (iRandRadius > 0)
		{
			z_get_ran_pos(stSEnv.pstEnv, pstMapInst, pstMapIdx, &stMonster.stInitPos, &stMonster.stInitPos, iRandRadius, 0, &stCurrPos);
		}
		
		stMonster.stInitPos = stCurrPos;
		stMonster.stCurrPos = stCurrPos;

		pstFind =	z_new_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stMonster, pstMonDef);
	
		if( !pstFind )
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		pstFind->iPatrolID = iPatrolId;

		pstFind->cFirstBorn = 1;
		pstFind->tLifeEnd = iLifeTime > 0?stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec+iLifeTime:0;
		if (0 > z_refresh_mon(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstFind,NULL))
		{
			tmempool_free(stSEnv.pstEnv->pstMonsterPool, pstFind->iMIndex);
			set_script_variable(iRetIdx, -1);
			return 0;
		}
		
		pstFind->cFirstBorn = 0;
		
		if (iPatrolId > 0)
		{
			pstFind->bMoveType = MOVE_TYPE_PATROL;
		}

		if (0 == i)
		{
			set_script_variable(iMonInstIDIdx, pstFind->iID);
		}
	}

	set_script_variable(iRetIdx, 0);
		
	return 0;
}

int s_team_pworld_info()
{
	ZoneTeam *pstTeam;
	int iRetIdx;
	int iPworldID;
	int iLevel;
	int iGrade;
	ZONEPWORLDINST *pstPworldInst;

	iRetIdx = get_script_number(NULL);
	iPworldID = get_script_number(NULL);
	iLevel = get_script_number(NULL);
	iGrade = get_script_number(NULL);
	
	if (NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstTeam = player_team_get(stSEnv.pstEnv, stSEnv.pstPlayer);
	if(NULL == pstTeam)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_id_pworld_inst_byteam(stSEnv.pstEnv, pstTeam, iPworldID);
	if(NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iLevel, pstPworldInst->bPlayerMeanLevel);
	set_script_variable(iGrade, pstPworldInst->stPworldselect.PworldGrade);
	
	return 0;
}



int s_enter_pworld_num_test()
{
	ZoneTeam *pstTeam;
	int iRetIdx;
	int iPworldID;
	int iDiff;
	ZONEPWORLD *pstZonePworld;
	ZONEIDX stIdx;
	ZONEPWORLDINST *pstPworldInst;
	int i;
	ROLEMISCINFO *pstMiscInfo;
	
	iPworldID = get_script_number(NULL);
	iDiff = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	stIdx.iID = iPworldID;
	stIdx.iPos = -1;
	pstZonePworld = z_find_pworld(stSEnv.pstEnv->pstMapObj, &stIdx);
	if (NULL == pstZonePworld)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	//已经创建了这个副本则可以进入
	pstPworldInst = z_find_pworld_inst_bysingle(stSEnv.pstEnv, stSEnv.pstPlayer);
	if(pstPworldInst && pstPworldInst->stPworldIdx.iID == iPworldID)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	pstTeam = player_team_get(stSEnv.pstEnv, stSEnv.pstPlayer);
	if(pstTeam)
	{
		pstPworldInst = z_find_id_pworld_inst_byteam(stSEnv.pstEnv, pstTeam, iPworldID);
		if(pstPworldInst && pstPworldInst->stPworldIdx.iID == iPworldID)
		{
			set_script_variable(iRetIdx, 0);
			return 0;
		}
	}
	
	pstMiscInfo = &stSEnv.pstPlayer->stRoleData.MiscInfo;
	if (0 < pstZonePworld->stPworldDef.NumEveryDay || 
		0 < pstZonePworld->stPworldDef.NumEveryWeek)
	{
		//检查每日副本次数
		for (i=pstMiscInfo->LimitPworldNum-1; i>=0; i--)
		{
			if (	0 == IsSameDay(stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].Time) &&
				0 == IsSameDay(stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].ItemTime) &&
				( 0 == pstZonePworld->stPworldDef.NumEveryWeek || 
				  0 == IsSameWeek(stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec, pstMiscInfo->LimitPworld[i].WeekTime)))
			{
				pstMiscInfo->LimitPworldNum--;
				if (i != pstMiscInfo->LimitPworldNum)
				{
					memmove(&pstMiscInfo->LimitPworld[i], &pstMiscInfo->LimitPworld[i+1],  (pstMiscInfo->LimitPworldNum - i) * sizeof(pstMiscInfo->LimitPworld[0]));
				}
				
				continue;
			}
			
			if (pstMiscInfo->LimitPworld[i].PworldID == pstZonePworld->stPworldDef.PworldID )
			{
				int iVip = 0;

				if (pstZonePworld->stPworldDef.PworldID == 300 ) //大地之心
				{
					RESULT *pstVipResult = star_get_result(stSEnv.pstEnv, stSEnv.pstPlayer, STAR_RESULT_DADI);
					if (pstVipResult)
					{
						iVip = pstVipResult->ResultVal1;
					}
				}
				
				if (pstMiscInfo->LimitPworld[i].Num >= pstZonePworld->stPworldDef.NumEveryDay+iVip)
				{
					set_script_variable(iRetIdx, -1);
					return 0;
				}

				if (pstMiscInfo->LimitPworld[i].WeekNum >= pstZonePworld->stPworldDef.NumEveryWeek)
				{
					set_script_variable(iRetIdx, -2);
					return 0;
				}
			}
		}
	}


	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_mon_patrol_id()
{
	int iMonID;
	int iRetIdx; 
	int iPatrolId;
	Monster *pstMon;

	iMonID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iPatrolId = get_script_number(NULL);

	pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMonID);
	if (NULL == pstMon)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iPatrolId, pstMon->iPatrolID);
	
	return 0;
}

int s_get_map_mon_num()
{
	int i, j, iDyn;
	Monster *pstMon;
	AREADYN *pstAreaDyn; 
	ZONEMAPINST *pstMapInst = NULL;
	ZONEIDX stIdx;
	int iMapID = get_script_number(NULL);
	int iDefID = get_script_number(NULL);
	int iRet = get_script_number(NULL);
	int iNum = get_script_number(NULL);
	int iInstID = get_script_number(NULL);
	int iCount = 0;

	if(iMapID == 0)
	{	
		pstMapInst = s_get_mapinst();
		if (pstMapInst == NULL)
		{
			goto _error;
		}
	}
	else
	{
		stIdx.iID	 =	iMapID;
		stIdx.iPos =	-1;
		pstMapInst = z_find_map_inst( stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stIdx);
		if (NULL == pstMapInst)
		{
			goto _error;
		}
	}
	
	for (i=0; i<pstMapInst->iAreaDyn; i++)
	{
		iDyn	 =	pstMapInst->aiAreaDyn[i];
		if( iDyn<0)
		{
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(stSEnv.pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		for(j=0; j<pstAreaDyn->iAnimate; j++)
		{
			if( j>=MAX_AREA_ANIMATE )
			{
				tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d", pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
				break;
			}

			pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstAreaDyn->aiAnimate[j]);
			if (pstMon &&  pstMon->iDefIdx == iDefID)
			{
				if(iCount == 0)
				{
					set_script_variable(iInstID, pstMon->iID);
				}
				
				iCount ++;
			}
		}
	}

	set_script_variable(iRet, 0);
	set_script_variable(iNum, iCount);
	return 0;
	
_error:
	set_script_variable(iRet, -1);
	return 0;
}

int s_get_item_use_dur()
{
	int iRetIdx;
	int iUseDur;
	int iMaxUseDur;
	ITEMDEF *pstItemDef;
	
	iRetIdx = get_script_number(NULL);
	iUseDur = get_script_number(NULL);
	iMaxUseDur = get_script_number(NULL);
	
	if(NULL == stSEnv.pstRoleItem)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstItemDef = z_find_itemdef(stSEnv.pstEnv, stSEnv.pstRoleItem->DefID);
	if(NULL == pstItemDef || pstItemDef->UseMaxDur == 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iUseDur, stSEnv.pstRoleItem->InstVal1);
	set_script_variable(iMaxUseDur, pstItemDef->UseMaxDur);
	set_script_variable(iRetIdx, 0);
	
	return 0;
}
int s_add_item_on_map()
{
	RESPOS stPos;
	Player *pstPlayer;
	TASKGETITEM stTaskGetItem[3];
	int iRet = 0;
	int iviewFlag = 0;

	memset(stTaskGetItem,0,sizeof(TASKGETITEM)*3);
	/*
	  参数:第一个参数为返回值，后面的一次 itemid_1,itemnum_1,
	   itemid_2,itemnum_2,
       itemid_3,itemnum_3,
	*/
	iRet = get_script_number(NULL);
	iviewFlag = get_script_number(NULL);
	stTaskGetItem[0].ItemID= get_script_number(NULL);
	stTaskGetItem[0].Num=  get_script_number(NULL);
	stTaskGetItem[1].ItemID= get_script_number(NULL);
	stTaskGetItem[1].Num=  get_script_number(NULL);
	stTaskGetItem[2].ItemID= get_script_number(NULL);
	stTaskGetItem[2].Num=  get_script_number(NULL);	

	pstPlayer = stSEnv.pstPlayer;
	if(NULL == pstPlayer)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	if (stSEnv.pstMonster)
	{
		stPos = stSEnv.pstMonster->stCurrPos;
	}
	else
	{
		stPos = pstPlayer->stRoleData.Pos;
	}

	player_add_booty(stSEnv.pstEnv, pstPlayer, stTaskGetItem,3,iviewFlag, &stPos);


	set_script_variable(iRet,0);
	
	return 0;
}


int s_is_my_convoy_mon()
{
	int iMonID;
	int iRetIdx;
	Monster *pstMon;
	MONSTERDEF *pstMonDef;
	ZoneTeam *pstTeam = NULL;
	Player *pstPlayer = stSEnv.pstPlayer;

	iMonID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if(NULL == pstPlayer)
	{
		goto _error;
	}
	
	pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMonID);
	if(NULL == pstMon)
	{
		goto _error;
	}

	pstMonDef = z_get_mon_def(stSEnv.pstEnv, pstMon);
	if (NULL == pstMonDef)
	{
		goto _error;
	}

	//单人镖车
	if(pstMonDef->SubType == MON_SUB_TYPE_SINGLE_CAR ||
		pstMonDef->SubType == MON_SUB_TYPE_SINGLE_SAFECAR)
	{
		if(strcmp(pstPlayer->stRoleData.RoleName, pstMon->szOwner) != 0)
		{
			goto _error;
		}
	}
	//队伍镖车
	else if(pstMonDef->SubType == MON_SUB_TYPE_TEAM_CAR || 
			pstMonDef->SubType == MON_SUB_TYPE_TEAM_SAFECAR)
	{
		pstTeam = player_team_get(stSEnv.pstEnv, pstPlayer);
		if(NULL == pstTeam)
		{
			goto _error;
		}


		if(pstMon->stTeamInfo.iTeamIdx !=  pstTeam->iTeamIdx || 
			pstMon->stTeamInfo.ullTeamID != pstTeam->stTeamInfo.TeamID)
		{
			goto _error;
		}
	}
	else
	{
		goto _error;
	}
	
	set_script_variable(iRetIdx, 0);
	return 0;
_error:

	set_script_variable(iRetIdx, -1);
	return 0;
}


int s_get_id_player_status_info()
{
	// 参数1-- iPlayerID  
	// 参数2-- statusID
	// 参数3-- 返回值 0-- 有 -1没有
	// 参数4-- 状态等级
	// 参数5-- 叠加层数 
	// 参数6-- 施加者ID
	// 参数7-- 特殊信息

	int iPlayerID = get_script_number(NULL);
	int iStatusID = get_script_number(NULL);
	int iRet = get_script_number(NULL);
	int iStatusLevel = get_script_number(NULL);
	int iOverlay = get_script_number(NULL);
	int iApplyID = get_script_number(NULL);
	int iVal = get_script_number(NULL);
	ZoneStatus *pstStatus;
	Player *pstPlayer;

	pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iPlayerID);
	if(NULL == pstPlayer)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	pstStatus = z_player_find_status(pstPlayer, iStatusID);
	if(NULL == pstStatus)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	set_script_variable(iStatusLevel, pstStatus->bLevel);
	set_script_variable(iOverlay, pstStatus->unOverlay);
	set_script_variable(iApplyID, pstStatus->iApplyID);
	set_script_variable(iVal, pstStatus->iVal);
	
	set_script_variable(iRet, 0);
	return 0;
}

int s_get_id_mon_status_info()
{
	// 参数1-- iMonID  
	// 参数2-- statusID
	// 参数3-- 返回值 0-- 有 -1没有
	// 参数4-- 状态等级
	// 参数5-- 叠加层数 
	// 参数6-- 施加者ID
	// 参数7-- 特殊信息

	int iMonID = get_script_number(NULL);
	int iStatusID = get_script_number(NULL);
	int iRet = get_script_number(NULL);
	int iStatusLevel = get_script_number(NULL);
	int iOverlay = get_script_number(NULL);
	int iApplyID = get_script_number(NULL);
	int iVal = get_script_number(NULL);
	ZoneStatus *pstStatus;
	Monster *pstMon;

	pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMonID);
	if(NULL == pstMon)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	pstStatus = z_mon_find_status(pstMon, iStatusID);
	if(NULL == pstStatus)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	set_script_variable(iStatusLevel, pstStatus->bLevel);
	set_script_variable(iOverlay, pstStatus->unOverlay);
	set_script_variable(iApplyID, pstStatus->iApplyID);
	set_script_variable(iVal, pstStatus->iVal);
	
	set_script_variable(iRet, 0);
	return 0;
}

int s_reset_all_genius()
{
	Player *pstPlayer;
	int iRet = 0;
	/*
	  参数1: 返回值
	*/
	iRet = get_script_number(NULL);
	
	pstPlayer = stSEnv.pstPlayer;
	if(NULL == pstPlayer)
	{
		set_script_variable(iRet, -1);
		return 0;
	}
	genius_all_reset(stSEnv.pstEnv, pstPlayer);

	set_script_variable(iRet,0);	
	return 0;
}

int s_get_mon_fight_time_count()
{
	// 参数1-- iMonID  
	// 参数2-- 返回值 -1失败,0没有战斗,>0战斗计时

	int iMonID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	Monster *pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,
	                               iMonID);
	if(NULL == pstMon)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, pstMon->iCombatTimeCount);
	return 0;
}

int s_modify_mon_press()
{
	// 参数1-- iMonID  
	// 参数2-- 返回值 0成功,<0失败
	// 参数3-- 如何增加 0: 全部清空; 1: 第一名减少百分比; 2: 特殊职业压力值增加
	// 参数4-- 变化哪一个
	// 参数5-- 变化值

	int iMonID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	int iHow = get_script_number(NULL);
	int iWhich = get_script_number(NULL);
	int iValue = get_script_number(NULL);
	int iRet = -1;

	Monster *pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,
	                               iMonID);
	if(!pstMon)
	{
		goto final;
	}

	switch(iHow)
	{
	case 0:
		{
			ZONEMAPINST *pstMapInst = z_find_map_inst(
				stSEnv.pstEnv->pstAppCtx,
				stSEnv.pstEnv,
				&pstMon->stMap);
			if(!pstMapInst)
			{
				goto final;
			}
			pstMon->stAtkTargets.PressNum = 0; // 清除压力表之后要search view:
			iRet = z_mon_search_view(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,
			                         pstMapInst, &pstMon->stLastSearchPos, pstMon);
		}
		break;

	case 1:
		{
			int i = 0, iPressValue = 0;
		 	if(mon_press_max(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMon, &i) < 0)
		 	{
		 		goto final;
		 	}

		 	iPressValue = pstMon->stAtkTargets.astPressEntrys[i].iPress;
			iRet = mon_press_add_by_max(stSEnv.pstEnv, pstMon,
			                            ((iPressValue * iValue) / 100));
		}
		break;

	case 2:
		iRet = mon_press_add_by_career(stSEnv.pstEnv, pstMon, iWhich, iValue);
		break;
	}

final:
	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_mon_atk_specific_target()
{
	// 参数1-- 怪物实例ID  
	// 参数2-- 返回值 0成功,<0失败
	// 参数3-- 怎样选目标
	// 参数4-- 哪个目标
	// 参数5-- 技能ID
	
	int iMonID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	int iHow = get_script_number(NULL);
	int iWhich = get_script_number(NULL);
	int iSkillID = get_script_number(NULL);

	int iRet = -1;
	int iTargetID = -1;

	Monster *pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,
	                               iMonID);
	if(!pstMon)
	{
		goto final;
	}

	// 选定目标
	switch(iHow)
	{
  	case SPECIFIC_TARGET_RANDOM:
  		iTargetID = mon_press_forced_rand(pstMon);
  		break;
  	case SPECIFIC_TARGET_PRESS_ORDER:
  		iTargetID = mon_press_order(pstMon, iWhich);
  		break;
  	case SPECIFIC_TARGET_CAREER:
  		iTargetID = mon_press_specific_career(stSEnv.pstEnv, pstMon, iWhich);
  		break;
  	case SPECIFIC_TARGET_HP_ORDER:
  		iTargetID = mon_press_hp_order(stSEnv.pstEnv, pstMon, iWhich);
  		break;
  	case SPECIFIC_TARGET_HP_PERCENT_ORDER:
  		iTargetID = mon_press_hp_percent_order(stSEnv.pstEnv, pstMon, iWhich);
  		break;
	default:
		goto final;
		break;
	}

	// 进行攻击
	iRet = mon_ai_atk_specific_target_by_id(stSEnv.pstEnv, pstMon, iSkillID,
	                                        iTargetID);

final:
	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_mon_speech()
{
	// 参数1-- 怪物/NPC实例ID  
	// 参数2-- 返回值 0成功,<0失败
	// 参数3-- 广播类型
	// 参数4-- 发言类型
	// 参数5-- 发言内容

	Monster *pstMon;
	int iMonOrNpcID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	int iBroadcastType = get_script_number(NULL);
	int iSpeechType = get_script_number(NULL);
	const char *pszMsg = get_script_string();

	int iRet = -1;

	if (0 == iMonOrNpcID)
	{
		if (stSEnv.pstMonster)
		{
			pstMon = stSEnv.pstMonster;
		}
		else if (stSEnv.pstNpc)
		{
			pstMon = &stSEnv.pstNpc->stMon;
		}
		else
		{
			goto final;
		}
	}
	else
	{
		pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,
		                               iMonOrNpcID);
		if(!pstMon)
		{
			pstMon = (Monster *)z_id_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,
			                             iMonOrNpcID);
			if(!pstMon)
			{
				goto final;
			}
		}
	}

	if(!pszMsg || !pszMsg[0])
	{
		goto final;
	}

	iRet = z_mon_speech_ex(stSEnv.pstEnv, pstMon, stSEnv.pstPlayer, pszMsg,
	                       iBroadcastType, iSpeechType);

final:
	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_new_mon_and_forced_move()
{
	// 参数1-- iMonDefID  
	// 参数2-- 返回值 非0成功,<0失败
	// 参数3-- 怪物数量
	// 参数4-- 出生范围半径
	// 参数5-- 出生地x坐标
	// 参数6-- 出生地y坐标
	// 参数7-- 刷新时间
	// 参数8-- 目的地x坐标
	// 参数9-- 目的地y坐标
	int iMonDefID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	int iNumMon = get_script_number(NULL);
	int iRandRadius = get_script_number(NULL);
	int iX = get_script_number(NULL);
	int iY = get_script_number(NULL);
	char cRefresh = get_script_number(NULL);
	int iDestX = get_script_number(NULL);
	int iDestY = get_script_number(NULL);

	int i = 0, iRet = -1, iDummyMapPos = -1;
	Monster stMon;
	RESPOS stPos, stDestPos, stCurrPos;
	MapIndex *pstMapIdx = NULL;
	Monster *pstMonInst = NULL;
	MONSTERDEF *pstMonDef = NULL;
	ZONEMAPINST *pstMapInst = s_get_mapinst();

	if(!pstMapInst)
	{
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "MapInst NULL");
		goto error;
	}

	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos,
	                           pstMapInst->iMapInstID);
	if(!pstMapIdx)
	{
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "pstMapIdx NULL");
		goto error;
	}

	// 出生点
	stPos.X = iX;
	stPos.Y = iY;

	// 检查目标点的有效性
	iDummyMapPos = pstMapInst->iMapPos;
	stDestPos.X = iDestX;
	stDestPos.Y = iDestY;
	if(!z_valid_pos(stSEnv.pstEnv->pstMapObj, pstMapInst->iMapInstID,
	                &iDummyMapPos, &stDestPos)) 
	{
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0,
		           "mon %d cannot move to invalid pos(%d,%d).",
		           iMonDefID, iDestX, iDestY);
		goto error;
	}

	memset(&stMon, 0, sizeof(stMon));
	stMon.cStat = MONSTER_DIE;
	stMon.iDefIdx = iMonDefID;
	stMon.iDefPos = -1;
	stMon.stMap.iID = pstMapInst->iMapInstID;
	stMon.iInitMap = pstMapInst->iMapInstID;
	stMon.stMap.iPos = pstMapInst->iIndex;
	stMon.iInitMapPos = pstMapInst->iMapPos;
	stMon.stInitPos = stPos;
	stMon.cRefresh = cRefresh;

	pstMonDef = z_get_mon_def(stSEnv.pstEnv, &stMon);
	if(!pstMonDef)
	{
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "MonDef not exist: id=%d",
		           iMonDefID);
		goto error;
	}

	for(i = 0; i < iNumMon; i++)
	{
		stMon.stInitPos = stCurrPos = stPos;
		
		// 在一个圆形范围内随机点新建怪物
		if(iRandRadius > 0)
		{
			z_get_ran_pos(stSEnv.pstEnv, pstMapInst, pstMapIdx, &stMon.stInitPos,
			              &stMon.stInitPos, iRandRadius, 0, &stCurrPos);
		}
		stMon.bDirection = RAND1(180);
		stMon.stInitPos = stCurrPos;
		stMon.stCurrPos = stCurrPos;

		pstMonInst = z_new_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,
		                           &stMon, pstMonDef);
		if(!pstMonInst)
		{
			tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "new monster failed %d",
			           iMonDefID);
			goto error;
		}

		pstMonInst->cFirstBorn = 1;
		if(z_refresh_mon(stSEnv.pstEnv->pstAppCtx,
		                 stSEnv.pstEnv, pstMonInst,NULL) < 0)
		{
			tmempool_free(stSEnv.pstEnv->pstMonsterPool, pstMonInst->iMIndex);
			tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0,
			           "z_refresh_mon failed %d", iMonDefID);
			goto error;
		}
		pstMonInst->cFirstBorn = 0;


		// 移动Monster到指定位置
		// 硬穿越到这个点
 		pstMonInst->stNextPos = stDestPos;
 		pstMonInst->stForcedPos = stDestPos;
 		pstMonInst->stInitPos = stDestPos;

		pstMonInst->bMoveType = MOVE_TYPE_FORCED; // 强制移动
		pstMonInst->cMoving = 1;
		z_monster_move_notify(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,
		                      pstMonInst, CS_MOVE_WALK);
		TV_CLONE(pstMonInst->stLastMoveTv, stSEnv.pstEnv->pstAppCtx->stCurr);

		if(i == 0)
		{
			stSEnv.iMemID = pstMonInst->iID;
			iRet = pstMonInst->iID; // 只返回第一个怪物的实例ID
		}
	}

final:
	set_script_variable(iRetIdx, iRet);
	return 0;

error:
	iRet = -1;
	goto final;
	return 0;
}

int s_get_mon_current_patrol_pos()
{
	// 参数1-- 返回值 <0失败,否则成功
	int iRetIdx = get_script_number(NULL);

	int iRet = -1;

	if(!stSEnv.pstMonster)
	{
		goto final;
	}

	iRet = stSEnv.pstMonster->bPatrolPos;

final:
	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_set_mon_current_patrol_time()
{
	// 参数1-- 在当前巡逻点停留的时间
	// 参数2-- 返回值 <0失败,否则成功
	int iStoppingTime = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	int iRet = -1;

	if(!stSEnv.pstMonster)
	{
		goto final;
	}

	stSEnv.pstMonster->stLastMoveTv.tv_sec += iStoppingTime;
	iRet = 0;

final:
	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_get_distance_between_2mon()
{
	// 参数1-- 怪物1的实例ID
	// 参数2-- 怪物2的实例ID
	// 参数3-- 返回值 <0失败，否则为怪物1与怪物2之间的距离
	int iMon1ID = get_script_number(NULL);
	int iMon2ID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	int iRet = -1;
	Monster *pstMon1 = NULL, *pstMon2 = NULL;

	pstMon1 = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMon1ID);
	pstMon2 = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMon2ID);
	if(!pstMon1 || !pstMon2 || pstMon1->iInitMap != pstMon2->iInitMap)
	{
		goto final;
	}

	iRet = z_distance(&pstMon1->stCurrPos, &pstMon2->stCurrPos);

final:
	set_script_variable(iRetIdx, iRet);
	return 0;
}

static int MonDefID(ZONESVRENV *pstEnv, int iID, void *pvCookie,
                    int *piCount)
{
	int iDefID = *((const int*)pvCookie);
	const Monster *pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, iID);
	
	if(pstMon &&  pstMon->iDefIdx == iDefID)
	{
		++(*piCount);
	}
	return 0;
}

int s_get_mon_num_by_defid()
{
	// 参数1-- 怪物的实例ID
	// 参数2-- 返回值 <0失败，否则为统计的怪物数量
	int iMonDefID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	int iRet = -1;
	const ZONEMAPINST *pstMapInst = s_get_mapinst();
	if(!pstMapInst)
	{
		goto final;
	}

	iRet = map_entity_count_if(stSEnv.pstEnv, pstMapInst, &iMonDefID, 
	                           MonDefID);
final:
	set_script_variable(iRetIdx, iRet);
	return 0;
}

static int PlayerCareerInRange(ZONESVRENV *pstEnv, int iID,
                               void *pvCookie, int *piCount)
{
	const CountCookie *pstCookie = pvCookie;
	const Player *pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, iID);
	if(pstPlayer)
	{
		int iDist = z_distance((RESPOS *)&pstCookie->stPoint, 
		                       (RESPOS *)&pstPlayer->stRoleData.Pos);
		if(iDist < pstCookie->iRadius )
		{
			if( CAREER_NON  != (pstCookie->iExtra&0xF0) && pstPlayer->stRoleData.Career == pstCookie->iExtra  )
			{
				++(*piCount);
			}
			else if( CAREER_NON  == (pstCookie->iExtra&0xF0) && (pstPlayer->stRoleData.Career&0x0F) == (pstCookie->iExtra&0x0F) )
			{
				++(*piCount);
			}
		}
	}

	return 0;
}

int s_get_player_num_in_mon_range_by_career()
{
	// 参数1-- 怪物的实例ID
	// 参数2-- 返回值 <0失败，否则为统计的玩家数量
	// 参数3-- 范围半径，单位:米
	// 参数4-- 职业
	int iMonID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	int iRadius = get_script_number(NULL);
	int iCareer = get_script_number(NULL);

	int iRet = -1;
	CountCookie stCountCookie;
	const ZONEMAPINST *pstMapInst = s_get_mapinst();

	bzero(&stCountCookie, sizeof(stCountCookie));
	stCountCookie.pstMon = z_id_monster(
		stSEnv.pstEnv->pstAppCtx,
		stSEnv.pstEnv,
		iMonID);
	if(!stCountCookie.pstMon || !pstMapInst)
	{
		goto final;
	}

	stCountCookie.stPoint = stCountCookie.pstMon->stCurrPos;
	stCountCookie.iRadius = iRadius * 100;
	stCountCookie.iExtra = iCareer;
	iRet = map_entity_count_if(stSEnv.pstEnv, pstMapInst, &stCountCookie,
	                           PlayerCareerInRange);

final:
	set_script_variable(iRetIdx, iRet);
	return 0;
}

static int FriendMonInRange(ZONESVRENV *pstEnv, int iID, void *pvCookie,
                            int *piCount)
{
	const CountCookie *pstCookie = pvCookie;
	const Monster *pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, iID);

	if(!pstMon)
	{
		return 0;
	}

	switch(pstMon->bSubType)
	{
	case MON_SUB_TYPE_TRAP: /* 地雷 */
	case MON_SUB_TYPE_TOTEM: /* 图腾 */
	case MON_SUB_TYPE_COLLECT: /* 采集 */
		return 0; // 排除此三类怪不统计
		break;
    }

    if(z_distance((RESPOS*)&pstCookie->stPoint, (RESPOS*)&pstMon->stCurrPos)
       >= pstCookie->iRadius)
    {
    	return 0;
    }

	if(is_friend_mon((Monster *) pstCookie->pstMon) ||
	   is_convoy_car((Monster *)pstCookie->pstMon))
	{
		// 帮助邪恶人类的怪或镖车
		if(is_friend_mon((Monster *) pstMon) ||
		   is_convoy_car((Monster *)pstMon))
		{
			++(*piCount);
		}
	}
	else
	{
		// 正义友怪
		if(!is_friend_mon((Monster *) pstMon) &&
		   !is_convoy_car((Monster *)pstMon))
		{
			++(*piCount);
		}
	}
	return 0;
}

int s_get_friend_num_in_mon_range()
{
	// 参数1-- 怪物的实例ID
	// 参数2-- 返回值 <0失败，否则为统计的怪物数量
	// 参数3-- 范围半径，单位:米
	int iMonID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	int iRadius = get_script_number(NULL);

	int iRet = -1;
	CountCookie stCountCookie;
	const ZONEMAPINST *pstMapInst = s_get_mapinst();

	bzero(&stCountCookie, sizeof(stCountCookie));
	stCountCookie.pstMon = z_id_monster(
		stSEnv.pstEnv->pstAppCtx,
		stSEnv.pstEnv,
		iMonID);
	if(!stCountCookie.pstMon || !pstMapInst)
	{
		goto final;
	}

	stCountCookie.stPoint = stCountCookie.pstMon->stCurrPos;
	stCountCookie.iRadius = iRadius * 100;
	iRet = map_entity_count_if(stSEnv.pstEnv, pstMapInst, &stCountCookie,
	                           FriendMonInRange);

final:
	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_get_mon_hp_percent()
{
	// 参数1-- 怪物的实例ID
	// 参数2-- 返回值 <0失败，怪物血量百分比
	int iMonID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	int iRet = -1;
	Monster *pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,
	                               iMonID);
	if(!pstMon)
	{
		goto final;
	}

	iRet = pstMon->iHP * 100 / (!pstMon->iMaxHP ? 1 : pstMon->iMaxHP);

final:
	set_script_variable(iRetIdx, iRet);
	return 0;
}

static int FriendPlayerInRange(ZONESVRENV *pstEnv, int iID,
                               void *pvCookie, int *piCount)
{
	CountCookie *pstCookie = (CountCookie *)pvCookie;
	Player *pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, iID);

	if(pstPlayer)
	{
		int iFlags = z_get_hit_player_relation(pstEnv, pstPlayer,
		                                       pstCookie->pstPlayer);
		if(iFlags & SKILL_TARGET_FRIEND_PLAYER)
		{
			int iDist = z_distance((RESPOS *)&pstCookie->stPoint, 
			                       (RESPOS *)&pstPlayer->stRoleData.Pos);
			if(iDist < pstCookie->iRadius)
			{
				++(*piCount);
			}
		}
	}
	return 0;
}

int s_get_friend_player_num_in_mon_press_order_player()
{
	// 参数1-- 怪物的实例ID
	// 参数2-- 返回值 <0失败，否则为统计的玩家数量
	// 参数3-- 压力表排位
	// 参数4-- 以玩家为圆心范围半径，单位:米
	int iMonID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	int iPressIdx = get_script_number(NULL);
	int iRadius = get_script_number(NULL);

	int iRet = -1;
	CountCookie stCountCookie;
	const ZONEMAPINST *pstMapInst = s_get_mapinst();
	if(!pstMapInst)
	{
		goto final;
	}

	bzero(&stCountCookie, sizeof(stCountCookie));
	stCountCookie.pstMon = z_id_monster(
		stSEnv.pstEnv->pstAppCtx,
		stSEnv.pstEnv,
		iMonID);
	if(!stCountCookie.pstMon)
	{
		goto final;
	}

	if(iPressIdx < 0 ||
	   iPressIdx >= stCountCookie.pstMon->stAtkTargets.PressNum)
	{
		goto final;
	}

	stCountCookie.pstPlayer = z_id_player(
		stSEnv.pstEnv->pstAppCtx,
		stSEnv.pstEnv,
		stCountCookie.pstMon->stAtkTargets.astPressEntrys[iPressIdx].iTargetID);
	if(!stCountCookie.pstPlayer)
	{
		goto final;
	}

	stCountCookie.stPoint = stCountCookie.pstPlayer->stRoleData.Pos;
	stCountCookie.iRadius = iRadius;
	iRet = map_entity_count_if(stSEnv.pstEnv, pstMapInst, &stCountCookie,
	                           FriendPlayerInRange);

final:
	set_script_variable(iRetIdx, iRet);
	return 0;
}

typedef struct tagDestroyCookie
{
	int *aiIDs;
	int iMax;
	int iDefID;
}DestroyCookie;

static int WillDestroyDefID(ZONESVRENV *pstEnv, int iID, void *pvCookie,
                            int *piCount)
{
	DestroyCookie *pstCookie = (DestroyCookie*)pvCookie;

	Monster *pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, iID);
	if(pstMon && pstMon->iDefIdx == pstCookie->iDefID)
	{
		pstCookie->aiIDs[(*piCount)] = pstMon->iID;
		++(*piCount);
	}
	return 0;
}

int s_delete_mon_by_defid()
{
	// 参数1-- 怪物的DefID
	// 参数2-- 返回值 <0失败，否则成功
	int iMonDefID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	static int aiIDs[MAX_AREA_ANIMATE]; // 太大了，静态分配吧
	int i = 0, iRet = -1;
	DestroyCookie stDestroyCookie;
	const ZONEMAPINST *pstMapInst = s_get_mapinst();

	bzero(&stDestroyCookie, sizeof(stDestroyCookie));
	if(!pstMapInst)
	{
		goto final;
	}

	stDestroyCookie.aiIDs = aiIDs;
	stDestroyCookie.iMax = CCH(aiIDs);
	stDestroyCookie.iDefID = iMonDefID;
	iRet = map_entity_count_if(stSEnv.pstEnv, pstMapInst, &stDestroyCookie,
	                           WillDestroyDefID);
	if(iRet < 0)
	{
		goto final;
	}

	for(i = 0; i < iRet; ++i)
	{
		Monster *pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,
		                               aiIDs[i]);
		if(!pstMon)
		{
			continue;
		}
		z_mon_destroy(stSEnv.pstEnv, pstMon);
	}

final:
	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_id_player_group_oneday_info()
{
	int iRetIdx;
	int iGroupID;
	int iOneDayNum;
	int iMemID;
	Player *pstPlayer;
	GROUPONEDAYINFO *pstGroupOneDayInfo;

	iMemID = get_script_number(NULL);
	iGroupID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iOneDayNum = get_script_number(NULL);

	pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if(NULL == pstPlayer)
	{
		goto _error;
	}

	if(iGroupID <= 0)
	{
		goto _error;
	}
	
	pstGroupOneDayInfo = group_find_oneday(pstPlayer, iGroupID);
	if(NULL == pstGroupOneDayInfo)
	{
		set_script_variable(iRetIdx, 0);
		set_script_variable(iOneDayNum, 0);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iOneDayNum, pstGroupOneDayInfo->OneDayNum);
	
	return 0;
_error:

	set_script_variable(iRetIdx, -1);
	set_script_variable(iOneDayNum, 0);
	
	return 0;
}

int s_id_player_group_add_oneday()
{
	int iRetIdx;
	int iGroupID;
	int iMemID;
	Player *pstPlayer;

	iMemID  = get_script_number(NULL);
	iGroupID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if(NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(iGroupID)
	{
		group_add_oneday(stSEnv.pstEnv, pstPlayer, iGroupID);
	}
	
	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_set_event_pworld_queue()
{
	int iPworldID;
	int iRetValIdx;
	ZONEIDX stIdx;
	ZONEPWORLD *pstPworld;
	int iQueueIdx;
	WorldPworldQueue *pstPworldQueue = stSEnv.pstEnv->pstPworldQueue;
	
	iPworldID = get_script_number(NULL);
	iRetValIdx = get_script_number(NULL);
	
	if (stSEnv.pstEvent == NULL)
	{
		set_script_variable(iRetValIdx, -1);
		return 0;
	}

	stIdx.iID = iPworldID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(stSEnv.pstEnv->pstMapObj, &stIdx);
	if (pstPworld == NULL || pstPworld->stPworldDef.Type != PWORLD_TYPE_PLOY)
	{
		set_script_variable(iRetValIdx, -1);
		return 0;
	}

	if(stSEnv.pstEnv->pstWorldEvent->iQueueNum >= MAX_EVENT_QUEUE)
	{
		set_script_variable(iRetValIdx, -1);
		return 0;
	}
	
	//找个队列
	iQueueIdx = z_get_free_apply_queue_idx(stSEnv.pstEnv);
	if(0 > iQueueIdx )
	{
		set_script_variable(iRetValIdx, -1);
		return 0;
	}

	pstPworldQueue->stApplyQueue[iQueueIdx].iPworldID = iPworldID;
	pstPworldQueue->stApplyQueue[iQueueIdx].iQueuerNum = 0;
	if (pstPworld->stPworldDef.PolyType == PWORLD_POLY_SPAN_WORLD)
	{
		pstPworldQueue->stApplyQueue[iQueueIdx].iPworldFlag = 1;
	}
	
	stSEnv.pstEnv->pstWorldEvent->stPworldQueue[stSEnv.pstEnv->pstWorldEvent->iQueueNum].iPworldID = iPworldID;
	stSEnv.pstEnv->pstWorldEvent->stPworldQueue[stSEnv.pstEnv->pstWorldEvent->iQueueNum].EventIdx = stSEnv.pstEvent->iEventID;
	stSEnv.pstEnv->pstWorldEvent->iQueueNum++;

	set_script_variable(iRetValIdx, 0);
	return 0;
}

int s_set_event_battle()
{
	int iBattleID;
	int iRetValIdx;
	BATTLEDEF *pstBattleDef;
	
	iBattleID = get_script_number(NULL);
	iRetValIdx = get_script_number(NULL);
	
	if(stSEnv.pstEvent == NULL)
	{
		set_script_variable(iRetValIdx, -1);
		return 0;
	}

	pstBattleDef = z_find_battledef(stSEnv.pstEnv, iBattleID);
	if(NULL == pstBattleDef)
	{
		set_script_variable(iRetValIdx, -1);
		return 0;
	}

	if(stSEnv.pstEnv->pstWorldEvent->iBattleNum >= MAX_EVENT_BATTLE)
	{
		set_script_variable(iRetValIdx, -1);
		return 0;
	}
	
	stSEnv.pstEnv->pstWorldEvent->stBattle[stSEnv.pstEnv->pstWorldEvent->iBattleNum].iBattleD = iBattleID;
	stSEnv.pstEnv->pstWorldEvent->stBattle[stSEnv.pstEnv->pstWorldEvent->iBattleNum].EventIdx = stSEnv.pstEvent->iEventID;
	stSEnv.pstEnv->pstWorldEvent->iBattleNum++;

	set_script_variable(iRetValIdx, 0);
	return 0;
}


//去掉脚本报名
int s_apply_pworld_queue()
{
	int iPworldID;
	int iRetValIdx;
	
	iPworldID = get_script_number(NULL);
	iRetValIdx = get_script_number(NULL);
	
	if (stSEnv.pstPlayer == NULL ||iPworldID <= 0)
	{
		goto error;
	}

	/*
	if (z_player_pworld_queue(stSEnv.pstEnv, stSEnv.pstPlayer, iPworldID) < 0)
	{
		goto error;
	}
	*/

	set_script_variable(iRetValIdx, 0);
	return 0;

error:
	set_script_variable(iRetValIdx, -1);
	return 0;
}

int s_store_open_ui()
{
	int iRetValIdx;
	ZoneStore *pstZoneStore;
	StoreObj *pstObj;
	int iCountPage;
	Player *pstPlayer;
	
	iRetValIdx = get_script_number(NULL);
	
	if (stSEnv.pstPlayer == NULL || !stSEnv.pstEnv ||!stSEnv.pstNpc)
	{
		goto error;
	}

	pstPlayer = stSEnv.pstPlayer ;
	pstObj = stSEnv.pstEnv->pstStoreObj;

	if (stroe_open_ui_check( stSEnv.pstEnv, pstPlayer) < 0)
	{
		goto error;
	}
	
	iCountPage = pstObj->iStoreNum/STORE_PAGE_MAX;
	if (pstObj->iStoreNum%STORE_PAGE_MAX)
	{
		iCountPage++;
	}

	pstZoneStore = store_player_get(stSEnv.pstEnv, stSEnv.pstPlayer);

	store_open_ui_notify(stSEnv.pstEnv,stSEnv.pstPlayer, 0, iCountPage,stSEnv.pstNpc->stMon.iID,
						pstZoneStore?(&pstZoneStore->stStore):NULL);
	set_script_variable(iRetValIdx, 0);
	return 0;

error:
	set_script_variable(iRetValIdx, -1);
	return 0;
}

int s_mon_owner_msg_info()
{
	CSPKG *pstPkg = &stSEnv.stDlgPkg;
	Player *pstPlayer;
		
	if (!stSEnv.pstMonster)
	{
		return 0;
	}

	if (stSEnv.pstMonster->szOwner[0] == 0)
	{
		return 0;
	}

	pstPlayer = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstMonster->szOwner);
	if(NULL == pstPlayer)
	{
		return 0;
	}

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
	z_cltmsg_send(stSEnv.pstEnv, pstPlayer, pstPkg);
	
	return 0;
}

static ROLEPET* get_fight_pet(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i = 0;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;

	UNUSED(pstEnv);
	for(i = 0; i < (int)pstPetPackage->Num; i++)
	{
		ROLEPET *pstPet = pstPetPackage->RolePets + i;
		if(pstPet->StatFlag & PET_STAT_FIGHT)
		{
			return pstPet;
		}
	}
	return NULL;
}

static inline int get_fight_pet_hp(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	const ROLEPET *pstPet = get_fight_pet(pstEnv, pstPlayer);
	return !pstPet ? -1 : (int)pstPet->HP;
}

static inline int get_fight_pet_maxhp(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	const ROLEPET *pstPet = get_fight_pet(pstEnv, pstPlayer);
	return !pstPet ? -1 : (int)pstPet->MaxHP;
}


int s_get_id_player_info()
{
	// 参数1: 目标的实例ID ，0 是自己
	// 参数2: 获取属性 
	// 1--MemID
	// 2--Hp
	// 3-- x
	// 4-- y
	// 5--map
	// 6--sex
	// 7--Career
	// 8--level
	// 9--MaxHP
	// 10--MaxMP
	// 11--Mp
	// 12--Pet.HP
	// 13--Pet.MaxHP
	// 14--银币
	// 15--等级榜的排名|没上榜用0表示
	// 16--财富榜的排名|没上榜用0表示
	// 17--PK模式
	// 18--公会id
	// 19--角色对应等级表中的ContributeExp值
	// 20--PworldInstID副本实例ID
	// 23 战场荣誉
	// 24 绿洲牌子(战魂徽章)
	// 25 狂沙牌子(生存奖章)
	// 参数3: 返回值 -1为失败

	Player *pstPlayer;
	int iMemID = get_script_number(NULL);
	int iAttrType = get_script_number(NULL);
	int iRet = get_script_number(NULL);
	LEVELUP *pstLevel = NULL;
	
	//0 是自己
	if(iMemID == 0)
	{
		pstPlayer = stSEnv.pstPlayer;
	}
	else
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
		
	}

	if (NULL == pstPlayer)
	{
		goto error;
	}

	if (pstPlayer->stOnline.cMoving)
		z_player_move_curr(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, 0);
	
	if (iAttrType == 1)
	{
		set_script_variable(iRet, pstPlayer->iMemID);
	}
	else if(iAttrType == 2)
	{
		set_script_variable(iRet, pstPlayer->stRoleData.RoleDetail.HPCurr);
	}
	else if (iAttrType == 3)
	{
		set_script_variable(iRet, pstPlayer->stRoleData.Pos.X);
	}
	else if (iAttrType == 4)
	{
		set_script_variable(iRet, pstPlayer->stRoleData.Pos.Y);
	}
	else if (iAttrType == 5)
	{
		set_script_variable(iRet, GET_MAPID(pstPlayer->stRoleData.Map));
	}
	else if (iAttrType == 6)
	{
		set_script_variable(iRet, pstPlayer->stRoleData.Gender);
	}
	else if(iAttrType == 7)
	{
		set_script_variable(iRet, pstPlayer->stRoleData.Career);
	}
	else if(iAttrType == 8)
	{
		set_script_variable(iRet, pstPlayer->stRoleData.Level);
	}
	else if(iAttrType == 9)
	{
		set_script_variable(iRet, pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax);
	}
	else if(iAttrType == 10)
	{
		set_script_variable(iRet, pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax);
	}
	else if(iAttrType == 11)
	{
		set_script_variable(iRet, pstPlayer->stRoleData.RoleDetail.MPCurr);
	}
	else if(iAttrType == 12)
	{
		set_script_variable(iRet, get_fight_pet_hp(stSEnv.pstEnv, pstPlayer));
	}
	else if(iAttrType == 13)
	{
		set_script_variable(iRet, get_fight_pet_maxhp(stSEnv.pstEnv, pstPlayer));
	}
	else if (iAttrType == 14)
	{
		set_script_variable(iRet, pstPlayer->stRoleData.RoleDetail.Money);
	}
	else if (iAttrType == 15)
	{
		set_script_variable(iRet, player_get_top_range(stSEnv.pstEnv,pstPlayer,
								RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_LEVEL));
	}
	else if (iAttrType == 16)
	{
		set_script_variable(iRet, player_get_top_range(stSEnv.pstEnv,pstPlayer,
								RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_MONEY));
	}
	else if (iAttrType == 17)
	{
		set_script_variable(iRet, pstPlayer->stOnline.ePkMode);
	}
	else if (iAttrType == 18)
	{
		set_script_variable(iRet, pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id);
	}
	else if (iAttrType == 19)
	{
		pstLevel = z_get_levelup(stSEnv.pstEnv,pstPlayer->stRoleData.Level);

		if (!pstLevel)
		{
			goto error;
		}

		set_script_variable(iRet,pstLevel->ContributeExp);
	}
	else if (iAttrType == 20)
	{
		ZONEPWORLDINST *pstZonePworldInst = NULL;
			
		pstZonePworldInst = z_find_player_pworld_inst(stSEnv.pstEnv,pstPlayer);

		if (!pstZonePworldInst)
		{
			goto error;
		}
		set_script_variable(iRet, pstZonePworldInst->iPworldInstID);
	}
	else if (iAttrType == 21)
	{
		set_script_variable(iRet, pstPlayer->stRoleData.RoleDetail.Bull);
	}
	else if (iAttrType == 22)
	{
		LEVELUP *pstLevelUp = z_get_levelup(stSEnv.pstEnv, pstPlayer->stRoleData.Level);
	
		if (!pstLevelUp)
		{
			goto error;
		}

		if(pstPlayer->stRoleData.MiscInfo.DailyUseBull >= pstLevelUp->DailyUseBull)
		{
			set_script_variable(iRet, 0);
		}
		else
		{
			set_script_variable(iRet, pstLevelUp->DailyUseBull - pstPlayer->stRoleData.MiscInfo.DailyUseBull);
		}
	}
	else if (iAttrType == 23)
	{
		set_script_variable(iRet, pstPlayer->stRoleData.RoleDetail.BlueJade);
	}
	else if (iAttrType == 24)
	{
		set_script_variable(iRet, pstPlayer->stRoleData.RoleDetail.BlueGem);
	}
	else if (iAttrType == 25)
	{
		set_script_variable(iRet, pstPlayer->stRoleData.RoleDetail.GreenGem);
	}
	else
	{
		goto error;
	}
	
	return 0;

error:
	set_script_variable(iRet, -1);
	return 0;
}

int s_add_fini_task()
{
	Player *pstPlayer;
	int iTaskID = get_script_number(NULL);
	int iRet = get_script_number(NULL);
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;

	pstPlayer = stSEnv.pstPlayer;
	if(NULL == pstPlayer)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	pstTaskRunOne = task_run_find(pstPlayer, iTaskID);
	if (NULL == pstTaskRunOne)
	{
		if (gm_task_accept(stSEnv.pstEnv, pstPlayer, iTaskID) < 0)
		{
			return 0;
		}
		
		pstTaskRunOne = task_run_find(pstPlayer, iTaskID);
		if(NULL == pstTaskRunOne)
		{		
			return 0;
		}
	}

	pstTaskDef = task_def_find(stSEnv.pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
	if (NULL == pstTaskDef)
	{
		return 0;
	}
	
	if ( gm_task_commit(stSEnv.pstEnv, pstPlayer, pstTaskRunOne, pstTaskDef) < 0)
	{
		return 0;
	}

	return 0;
}

int s_commit_task()
{
	Player *pstPlayer;
	int iTaskID = get_script_number(NULL);
	int iRet = get_script_number(NULL);
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;

	pstPlayer = stSEnv.pstPlayer;
	if(NULL == pstPlayer)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	pstTaskRunOne = task_run_find(pstPlayer, iTaskID);
	if (NULL == pstTaskRunOne)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	pstTaskDef = task_def_find(stSEnv.pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
	if (NULL == pstTaskDef)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	if (0 <= task_commit(stSEnv.pstEnv, pstPlayer, pstTaskRunOne, pstTaskDef))
	{
		set_script_variable(iRet, 0);
	}
	else
	{
		set_script_variable(iRet, -1);
	}
	
	return 0;
/*
	if (pstTaskRunOne->TaskStat < TASK_STAT_COMMIT)
	{
		pstTaskRunOne->TaskStat = TASK_STAT_COMMIT;
		task_chg(stSEnv.pstEnv, pstPlayer, pstTaskRunOne);
		task_run_del(stSEnv.pstEnv, pstPlayer, pstTaskDef->ID);
	}

	if (pstTaskDef->TaskGetLimit.NumOneDay > 0)
	{
		task_add_oneday(stSEnv.pstEnv, pstPlayer, pstTaskDef->ID);
	}

	if (pstTaskDef->LimitGroupID > 0)
	{
		group_add_oneday(stSEnv.pstEnv, pstPlayer, pstTaskDef->LimitGroupID);
	}

	if (0 == pstTaskDef->Redo || pstTaskDef->Flag)
	{
		task_add_fini(pstPlayer, pstTaskDef->ID);
	}
	
	return 0;*/
}

int s_del_id_mon()
{
	int iRetIdx;
	int iMemID;
	MONSTERDEF *pstMonDef = NULL;
	Monster *pstMon;

	iMemID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if(NULL == pstMon)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstMonDef = z_get_mon_def(stSEnv.pstEnv, pstMon);
	if (pstMonDef)
	{
		ZoneAni stAni;

		if(stSEnv.pstPlayer)
		{
			pstMon->cFirstAtkerType = MON_BOOTY_OWNER_PLAYER;
			pstMon->ullFirstAtkerWid = stSEnv.pstPlayer->stRoleData.RoleID;
			pstMon->iFirstAtkerID = stSEnv.pstPlayer->iMemID;

			stAni.iType = OBJ_PLAYER;
			stAni.stObj.pstPlayer = stSEnv.pstPlayer;
		}
		else
		{
			stAni.iType = OBJ_UNKNOW;
		}
		
		z_mon_die(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMon, pstMonDef, &stAni, 1, 0);
	}
	else
	{
		z_free_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMon);
	}

	set_script_variable(iRetIdx, 0);
	
	return 0;
}
 int s_new_life_npc()
{
	int iNpcID;
   	int iX;
   	int iY;
   	int iDir;
   	int iLifeTime;
  	char *pszScript;
   	int iRetIdx;
   	MAPNPC stMapNpc;
   	ZONEMAPINST *pstMapInst;
   	MapIndex *pstMapIndex;
	int iRet;
	Npc *pstNpc;
	Player *pstFirstPlayer = NULL;
		
	iRetIdx = get_script_number(NULL);
	iNpcID = get_script_number(NULL);
	iX = get_script_number(NULL);
	iY = get_script_number(NULL);
	iDir = get_script_number(NULL);
	iLifeTime = get_script_number(NULL);
	pszScript = get_script_string();

	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
	        set_script_variable(iRetIdx, -1);
	        return 0;
	}

	pstMapIndex = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIndex)
	{
	        set_script_variable(iRetIdx, -1);
	        return 0;
	}

	memset(&stMapNpc, 0, sizeof(stMapNpc));
	stMapNpc.MonsterID = iNpcID;
	stMapNpc.Pos.X = iX;
	stMapNpc.Pos.Y = iY;
	stMapNpc.Dir = iDir;
	STRNCPY(stMapNpc.Script, pszScript, sizeof(stMapNpc.Script));

	if(stSEnv.pstMonster)
	{
		if(stSEnv.pstMonster->iFirstAtkerMemID != 0)
		{
			pstFirstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstMonster->iFirstAtkerMemID);
			if (pstFirstPlayer && pstFirstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				pstFirstPlayer = NULL;
			}
			
			if(pstFirstPlayer && 0 == player_if_in_span_map(stSEnv.pstEnv,pstFirstPlayer)
				&& pstFirstPlayer->stOnline.stWarOL.cCampIdx > 0)
			{
				stMapNpc.SpanMapCamp = pstFirstPlayer->stOnline.stWarOL.cCampIdx;
			}
		}
	}

	iRet = z_in_new_pworld_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMapIndex, pstMapInst, &stMapNpc,0);
	if (0 > iRet)
	{
	        set_script_variable(iRetIdx, -1);
	        return 0;
	}

	pstNpc = z_id_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iRet);
	if(pstNpc)
	{
		newnpc_add(stSEnv.pstEnv, pstMapInst, pstNpc);
		
		if (iLifeTime > 0)
		{
			pstNpc->stMon.tLifeEnd = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec + iLifeTime;
		}

		if(stSEnv.pstMonster)
		{
			pstNpc->stMon.FormMonDropWID = stSEnv.pstMonster->WID;
			MONSTERDEF *pstMonDef;
			pstMonDef = z_get_mon_def(stSEnv.pstEnv,stSEnv.pstMonster);
			if(pstMonDef)
			{
				pstNpc->stMon.iFromMonDropLevel = pstMonDef->Level;
			}
		}
	}

	set_script_variable(iRetIdx, iRet);

	return 0;
}
 
 int s_is_same_week()
{
	time_t tNow;
	time_t iStartTime = get_script_number(NULL);
	int iRet = get_script_number(NULL);

	tNow = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;

	if(iStartTime > tNow || iStartTime <= 0)
	{
		set_script_variable(iRet, -1);
		return 0;
	}   

	
	if (IsSameWeek(iStartTime, tNow))
	{
		set_script_variable(iRet, 0);
		return 0;
	}
	else
	{	
		set_script_variable(iRet, -1);
		return 0;
	}

	return 0;
}

 int s_is_same_day()
 {
 	time_t tNow;
	time_t iTime = get_script_number(NULL);
	int iRet = get_script_number(NULL);

	tNow = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;

	if(iTime > tNow || iTime <= 0)
	{
		set_script_variable(iRet, -1);
		return 0;
	}   

	if (IsSameDay(tNow, iTime))
	{
		set_script_variable(iRet, 0);
		return 0;
	}
	else
	{
		set_script_variable(iRet, -1);
		return 0;	
	}
	
	return 0;
 }

 int s_new_owner_tmp_item()
{
	int iItemID;
   	int iInputX;
   	int iInputY;
   	int iDir;
   	int iRefresh;//刷新时间 
	int iStay;//存活时间
	int iPatrolID;//巡逻路径
	int iStatus; //拾取道具时给的状态
	int iStatusLevel;//拾取道具时给的等级
	int iLast;//拾取道具时给的持续的时间
	int iRetIdx;
	
	ITEMDEF *pstItemDef;
   	ZONEMAPINST *pstMapInst;
   	MapIndex *pstMapIndex;
	MapIndex *pstMapIndex1;
	RESPOS stPos;
	PATROLDEF *pstPatrolDef;
	int iPatrolIdx;
	DropItem *pstDropItem;
	ZONEIDX stIdx;
	
	iItemID = get_script_number(NULL);
	iInputX = get_script_number(NULL);
	iInputY = get_script_number(NULL);
	iDir = get_script_number(NULL);
	iRefresh = get_script_number(NULL);
	iStay = get_script_number(NULL);
	iPatrolID = get_script_number(NULL);
	iStatus = get_script_number(NULL);
	iStatusLevel = get_script_number(NULL);
	iLast = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

       pstItemDef = z_find_itemdef(stSEnv.pstEnv, iItemID);
	if (NULL == pstItemDef || ITEM_TMP_NO == pstItemDef->ItemTmp.Type)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if(!stSEnv.pstPlayer )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
	        set_script_variable(iRetIdx, -1);
	        return 0;
	}

	pstMapIndex = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIndex)
	{
	        set_script_variable(iRetIdx, -1);
	        return 0;
	}

	if( iPatrolID != 0 )
	{
		pstPatrolDef = z_find_patroldef(stSEnv.pstEnv, iPatrolID, &iPatrolIdx);
		if (NULL == pstPatrolDef)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		pstMapIndex1 = z_mappos_index(stSEnv.pstEnv->pstMapObj, NULL, pstPatrolDef->MapID);
		if (NULL == pstMapIndex1)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		if (!(pstMapIndex == pstMapIndex1 || 
			pstMapIndex1->stMapDef.ReferMapID == pstMapIndex->stMapDef.MapID || 
			is_map_valid_line(pstMapIndex1, pstMapIndex)))
		{
			// 不在同一地图不让使用
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		if( pstPatrolDef->PatrolPos[0].X != iInputX ||
			pstPatrolDef->PatrolPos[0].Y != iInputY )
		{
			// 传入点不是起点,直接返回,严格控制
			set_script_variable(iRetIdx, -1);
			return 0;
		}
	}
	
	stPos.X = iInputX;
	stPos.Y = iInputY;

	stIdx.iID = stSEnv.pstPlayer->stRoleData.Map;
	stIdx.iPos = stSEnv.pstPlayer->stOnline.iMapPos;
	if(0 == tmp_item_create(stSEnv.pstEnv, stSEnv.pstPlayer,iItemID,&stIdx, &stPos, pstItemDef->ItemTmp.MachineEnerge, iRefresh, 
							iStay, iDir,0,0,&pstDropItem) )
	{
		pstDropItem->stObj.stTmpItem.iPatrolID = iPatrolID;
		pstDropItem->stObj.stTmpItem.iStatus = iStatus;
		pstDropItem->stObj.stTmpItem.iStatusLevel = iStatusLevel;
		pstDropItem->stObj.stTmpItem.iLast = iLast;
	}
	else 
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (0 > tmp_item_add(stSEnv.pstEnv, stSEnv.pstPlayer, pstDropItem, pstItemDef))
	{
		set_script_variable(iRetIdx, -1);
	}
	else
	{
		z_item_leave_map(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstDropItem);
		set_script_variable(iRetIdx, 0);
	}
	
	set_script_variable(iRetIdx, 0);
	return 0;
}
/*
	iType:0 - 指定属性加减；1 - 指定属性的百分比去加减
*/
static void s_add_player_attr(PlayerChagAttrVal *pstAttrVal, void *pvOffset, void *pvFlag,int iType)
{
	int iOldHP = 0;
	int iOldMP = 0;
	int iPlayerID = get_script_number(NULL);
	int nValue = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	Player *pstPlayer = NULL;
	if(iPlayerID == 0)
	{
		pstPlayer = stSEnv.pstPlayer;
	}
	else
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,
		                        iPlayerID);
	}
	if(!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return ;
	}

	iOldHP = pstPlayer->stRoleData.RoleDetail.HPCurr;
	iOldMP = pstPlayer->stRoleData.RoleDetail.MPCurr;

	if(iType == 1 && nValue != 0)
	{
		//iType=1时，暂时写死，默认HP属性
		nValue = (nValue/abs(nValue))*(pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax * abs(nValue)/100);
	}
	if(nValue < 0)
	{
		*((short *)pvFlag) = 1;
		*((unsigned int*)pvOffset) = -nValue;
	}
	else
	{
		*((short *)pvFlag) = 0;
		*((unsigned int*)pvOffset) = nValue;
	}
	
 	z_attr_chg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, pstAttrVal, 0);

	player_hp_action(stSEnv.pstEnv, pstPlayer, iOldHP, iOldMP, 0);
	
	set_script_variable(iRetIdx, 0);
}

int s_add_player_hp()
{
	PlayerChagAttrVal stAttr;

	stAttr.AttrNum = 1;
	stAttr.AttrList[0].AttrID = ATTR_ID_HP;
	stAttr.AttrList[0].nFlag = 0;
	s_add_player_attr(&stAttr, &stAttr.AttrList[0].Value.HP, &stAttr.AttrList[0].nFlag,0);
	return 0;
}

int s_add_player_mp()
{
	PlayerChagAttrVal stAttr;

	stAttr.AttrNum = 1;
	stAttr.AttrList[0].AttrID = ATTR_ID_MP;
	stAttr.AttrList[0].nFlag = 0;
	s_add_player_attr(&stAttr, &stAttr.AttrList[0].Value.MP, &stAttr.AttrList[0].nFlag,0);
	return 0;
}

int s_add_fight_pet_hp()
{
	// 参数1: 目标的实例ID ，0 是自己
	// 参数2: 增加的血量
	// 参数3: 返回值0为成功,<0为失败

	int iMemID = get_script_number(NULL);
	int iHP = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	ROLEPET *pstPet = NULL;
	int iOldHP = 0;
	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
	               z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if(!pstPlayer)
	{
		goto error;
	}
	pstPet = get_fight_pet(stSEnv.pstEnv, pstPlayer);
	if(!pstPet)
	{
		goto error;
	}
	if(iHP < 0)
	{
		goto error;
	}

	iOldHP = pstPet->HP;
	pstPet->HP += iHP;
	if(pstPet->HP > pstPet->MaxHP)
	{
		pstPet->HP = pstPet->MaxHP; 
	}
	pet_hp_action(stSEnv.pstEnv, pstPlayer, pstPet, iOldHP);
	set_script_variable(iRetIdx, 0);
	return 0;
	
error:
	set_script_variable(iRetIdx, -1);
	return 0;
}




int s_pworld_revive_player()
{
	int iPlayerID;
	int iType;
	int iX,iY;
	int iRet;	
	Player *pstPlayer = NULL;

	//CSPKG *pstPkg = &stSEnv.pstEnv->stCSRes;
	//PlayerChagAttrVal stChgVal;
	//CSACTIONRES *pstActRes = &pstPkg->Body.ActionRes;
	//int iHPAdd = 0, iMPAdd = 0;

	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	RESPOS stPos;
	int iMapIndex;

	iPlayerID = get_script_number(NULL);
	iType =  get_script_number(NULL);
	iX = get_script_number(NULL);
	iY = get_script_number(NULL);
	iRet = get_script_number(NULL);
	
	if (0 == iPlayerID)
	{
		pstPlayer = stSEnv.pstPlayer;
	}
	else
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iPlayerID);
	}
	
	if (NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	//if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
	//
	//{
	//	return -1;
	//}
	
	//iHPAdd = (pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax * 20) / 100;
	//iMPAdd = (pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax * 20) / 100;

	pstPlayer->stOnline.iReviverID = 0;
	pstPlayer->stOnline.State &= ~CS_STAT_DEAD;
	player_revive_attr_chg(	stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer,
							(pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax * 20) / 100, 
							(pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax * 20) / 100);
	
	stIdx.iID		=	pstPlayer->stRoleData.Map;
	stIdx.iPos	=	pstPlayer->stOnline.iMapPos;
	pstMapInst	=	z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,  &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	if (iType == 0)		// 复活点
	{
		stPos = pstMapIdx->stMapDef.RevivePs[0];
		if (stPos.X <=0 || stPos.Y <= 0)
		{
			set_script_variable(iRet, -1);
			return 0;
		}
		
		if (pstMapIdx->stMapDef.ReviveMap > 0 && pstMapIdx->stMapDef.ReviveMap < PWORLD_MAPID_BASE)
		{
			z_player_change_map(stSEnv.pstEnv->pstAppCtx,  stSEnv.pstEnv, pstPlayer, pstMapIdx->stMapDef.ReviveMap, &stPos, RAND1(180), 0);
		}
		else if (pstMapIdx->stMapDef.ReviveMap >= PWORLD_MAPID_BASE)  // 副本内
		{
			iMapIndex = (pstPlayer->stRoleData.Map & 0xffff0000) | pstMapIdx->stMapDef.ReviveMap;
			z_player_change_map(stSEnv.pstEnv->pstAppCtx,  stSEnv.pstEnv, pstPlayer, iMapIndex, &stPos, RAND1(180), 0);
		}
		
	}
	else if (iType == 1)	// 原地复活
	{
		z_player_search_view(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,  pstMapInst, &pstPlayer->stOnline.stLastSearchPos, pstPlayer);
	}
	else if(iType == 2)	// 指定坐标复活
	{
		if (iX <=0 || iY <= 0)
		{
			set_script_variable(iRet, -1);
			return 0;
		}
		stPos.X = iX;
		stPos.Y = iY;
		
		iMapIndex = (pstPlayer->stRoleData.Map & 0xffff0000) | pstMapIdx->stMapDef.ReviveMap;
		z_player_change_map(stSEnv.pstEnv->pstAppCtx,  stSEnv.pstEnv, pstPlayer, iMapIndex, &stPos, RAND1(180), 0);
	}
	else
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	player_revive_action(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer);

	set_script_variable(iRet, 0);
	return 0;
}


int s_call_mon_by_patrol()
{
	int iMonNum;
	int iMonID;
	int iRadius;
	int iLifeTime;
	int iX,iY;
	int iVarIdx;
	int iPatrol;
	int iRet;
	int iDir;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;

	iMonNum = get_script_number(NULL);
	iMonID = get_script_number(NULL);
	iRadius =   get_script_number(NULL);
	iLifeTime = get_script_number(NULL);
	iX  = get_script_number(NULL);
	iY =   get_script_number(NULL);
	iPatrol = get_script_number(NULL);
	iDir = get_script_number(NULL);
	iVarIdx =   get_script_number(NULL);

	if (NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}
	
	stIdx.iID = stSEnv.pstPlayer->stRoleData.Map;
	stIdx.iPos = stSEnv.pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		set_script_variable(iVarIdx, -1);
		return 0;
	}

	iRet = item_call_mon(stSEnv.pstEnv,stSEnv.pstPlayer, pstMapInst, iMonNum,iMonID, iRadius, iLifeTime, iX, iY, iPatrol, iDir, 0);
	
	set_script_variable(iVarIdx, iRet);
	
	return 0;
}


int s_get_role_tmp_item()
{
	// 参数1: 角色的实例ID ，0 是自己
	// 参数2: 返回值>=0为成功,<0为失败

	int iMemID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
		z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if(!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, pstPlayer->stRoleData.MiscInfo.TmpSys.TmpItemID);
	return 0;
}

int s_del_role_tmp_item()
{
	// 参数1: 角色的实例ID ，0 是自己
	// 参数2: 返回值0为成功,<0为失败

	int iMemID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
		z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if(!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	
	set_script_variable(iRetIdx,
		                tmp_item_del(stSEnv.pstEnv, pstPlayer) >= 0 ? 0 : -1);
	return 0;
}

static int map_delete_item(ZONESVRENV *pstEnv, const ZONEMAPINST *pstMapInst,
                           int iMemID, int iItemID)
{
	int i = 0, iCount = 0;

	for(i = 0; i < pstMapInst->iAreaDyn; ++i)
	{
		const AREADYN *pstAreaDyn = NULL;
		int j = 0;
		int iDyn = pstMapInst->aiAreaDyn[i];
		if(iDyn < 0)
		{
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		for(j = 0; j < pstAreaDyn->iInanimate; ++j)
		{
			DropItem *pstDropItem = NULL;
			if(j >= MAX_AREA_INANIMATE)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0,
				           "map %d dyn %d iInanimate=%d",
				           pstMapInst->iMapInstID, iDyn,
				           pstAreaDyn->iInanimate);
				return -1;
			}

			pstDropItem = z_id_dropitem(pstEnv->pstAppCtx, pstEnv,
			                            pstAreaDyn->aiInanimate[j]);
			if(!pstDropItem)
			{
				continue;
			}
			if (iMemID)
			{
				if(pstDropItem->stObj.stTmpItem.iPlayerMemID == iMemID)
				{
					if(!iItemID || iItemID == pstDropItem->iDefIdx)
					{
						z_item_clean(pstEnv, pstDropItem);
						++iCount;
					}
				}
			}
			else
			{
				if(DROP_ITEM_TMP == pstDropItem->cType && (!iItemID || iItemID == pstDropItem->iDefIdx))
				{
					z_item_clean(pstEnv, pstDropItem);
					++iCount;
				}
			}
		}
	}
	return iCount;
}

int s_del_role_unused_tmp_item()
{
	// 参数1: 角色的实例ID，0 是自己
	// 参数2: 指定的物品ID，0 是全部
	// 参数3: 返回值>=0为成功,<0为失败

	int iMemID = get_script_number(NULL);
	int iItemID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	const ZONEMAPINST *pstMapInst = s_get_mapinst();
	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
		z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if(!pstPlayer || !pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx,
		map_delete_item(stSEnv.pstEnv, pstMapInst, pstPlayer->iMemID, iItemID));
	return 0;
}

int s_task_run()
{
	int iTaskID;
	int iRetIdx;
	TASKRUNONE *pstTaskRunOne;
	Player *pstPlayer = stSEnv.pstPlayer;

	iTaskID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if(!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstTaskRunOne = task_run_find(pstPlayer, iTaskID);
	if(!pstTaskRunOne)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstTaskRunOne->TaskStat = TASK_STAT_RUN;
	task_chg(stSEnv.pstEnv, pstPlayer, pstTaskRunOne);

	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_dec_money_test()
{
	// 参数1: 角色的实例ID，0 是自己
	// 参数2: 减少的钱
	// 参数3: 钱类型
	// 参数4: 返回值0为成功,<0为失败
	
	int iMemID = get_script_number(NULL);
	int iMoney = get_script_number(NULL);
	int iMoneyType = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
		z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if(!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if(!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(package_dec_money_test(stSEnv.pstEnv, pstPlayer, iMoneyType, iMoney) < 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	
	return 0;
}


int s_dec_money()
{
	// 参数1: 角色的实例ID，0 是自己
	// 参数2: 减少的钱
	// 参数3: 钱类型
	// 参数4: 返回值0为成功,<0为失败
	
	int iMemID = get_script_number(NULL);
	int iMoney = get_script_number(NULL);
	int iMoneyType = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	int iRet;
	char szMsg[512];

	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
		z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if(!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if(!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(package_dec_money_test(stSEnv.pstEnv, pstPlayer, iMoneyType, iMoney) < 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iRet = package_dec_money(stSEnv.pstEnv, pstPlayer, iMoneyType, iMoney);
	if(0 > iRet)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	snprintf(szMsg, sizeof(szMsg), "脚本减:%s", stSEnv.szScript);
	szMsg[sizeof(szMsg) - 1] = 0;

	if (iMoneyType == ATTR_ID_BINDGODCOIN)
	{
		z_bindgold_consume_oplog(stSEnv.pstEnv, pstPlayer, BINDGOLD_OPTYPE_SCRIPT_DEC, 0, iMoney,
							szMsg);
	}
	else if (iMoneyType == ATTR_ID_MONEY)
	{
		z_money_consume_oplog(stSEnv.pstEnv, pstPlayer, MONEY_OPTYPE_SCRIPT_DEC, 0, 0, iMoney, szMsg);
	}
	else if (iMoneyType == ATTR_ID_BULL)
	{
		z_money_consume_oplog(stSEnv.pstEnv, pstPlayer, MONEY_OPTYPE_SCRIPT_DEC, 0, iMoney-iRet,iRet,  szMsg);
	}
	else if (iMoneyType == ATTR_ID_GODCOIN)
	{
		godcoin_consume_log(stSEnv.pstEnv, pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
						pstPlayer->stRoleData.Level, GODCOIN_OPER_FLAG_SCRIPT,0,0,0,
						iMoney, szMsg);
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_npc_die()
{
	Npc *pstNpc;
	int iMemID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	if(iMemID == 0)
	{
		pstNpc = stSEnv.pstNpc;
	}
	else
	{
		pstNpc = z_id_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	}
	
	if(NULL == pstNpc)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	z_npc_die(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstNpc);

	set_script_variable(iRetIdx, 0);

	return 0;
}

int s_has_id_player_designation()
{

	int iMemID = get_script_number(NULL);
	int iDesignationID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	//ROLEDESIGNATIONS *pstInfo = NULL;
	DESIGNATIONENTRY *pstDsn = NULL;
	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
	               z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
				   
	if(NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstDsn = player_find_designation(stSEnv.pstEnv, pstPlayer, iDesignationID);

	if(pstDsn)
	{
		set_script_variable(iRetIdx, 1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_task_fail()
{
	int iTaskID;
	int iRetIdx;
	TASKRUNONE *pstTaskRunOne;
	Player *pstPlayer = stSEnv.pstPlayer;

	iTaskID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if(!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstTaskRunOne = task_run_find(pstPlayer, iTaskID);
	if(!pstTaskRunOne)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(pstTaskRunOne->TaskStat  == TASK_STAT_FINI)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(pstTaskRunOne->TaskStat != TASK_STAT_FAIL)
	{
		pstTaskRunOne->TaskStat = TASK_STAT_FAIL;
		task_chg(stSEnv.pstEnv, pstPlayer, pstTaskRunOne);
	}
	
	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_player_ride_on()
{
	int iMemID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	int iRideID = get_script_number(NULL);
	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
	               z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
				   
	if(NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(! (pstPlayer->stOnline.State & (CS_STAT_RIDE_ON | CS_STAT_FLY)))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRideID, pstPlayer->stOnline.RideItemID);
	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_ride_off()
{
	int iRetIdx = get_script_number(NULL);
	if (NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	z_ride_break(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer);

	set_script_variable(iRetIdx, 0);

	return 0;
}


int s_ronglu_task_get()
{
	int iRetIdx = get_script_number(NULL);
	int iTaskID = get_script_number(NULL);
	if (NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (ronglu_get_task(stSEnv.pstEnv,stSEnv.pstPlayer, iTaskID) < 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;	
	}
	
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_call_ronglu_mon()
{
	int iRetIdx = get_script_number(NULL);
	int iMonID = get_script_number(NULL);
	int iLeftTime = get_script_number(NULL);
	int iDynLvl = get_script_number(NULL);
	int iDynGrade = get_script_number(NULL);

	if ( stSEnv.pstPlayer == NULL || stSEnv.iItemDefID == 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;	
	}

	if (ronglu_player_new_mon(stSEnv.pstEnv, stSEnv.pstPlayer ,stSEnv.iItemDefID, 
	 							iMonID, iLeftTime,  iDynLvl, iDynGrade) < 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;	
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_ronglu_is_full()
{
	int iRetIdx = get_script_number(NULL);
	int iFull = get_script_number(NULL);

	if (stSEnv.pstMonster == NULL)
	{
		set_script_variable(iRetIdx, -1);
		return 0;	
	}

	if (stSEnv.pstMonster->iCommType != MON_UNION_TYPE_RONGLU)
	{
		set_script_variable(iRetIdx, -1);
		return 0;	
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iFull, ronglu_is_full(stSEnv.pstMonster ));
	return 0;
}

int s_ronglu_chg_gold()
{
	int iRetIdx = get_script_number(NULL);
	int iItemID = get_script_number(NULL);

	if (stSEnv.pstPlayer == NULL)
	{
		set_script_variable(iRetIdx, -1);
		return 0;	
	}

	set_script_variable(iRetIdx,  ronglu_item_chg_gold(stSEnv.pstEnv, stSEnv.pstPlayer, iItemID));
	return 0;
}

int s_ronglu_new_collect_mon()
{
	int iRetIdx = get_script_number(NULL);
	int iMonID = get_script_number(NULL);
	int iLeftTime = get_script_number(NULL);
	Player *pstPlayer;


	if (stSEnv.pstMonster == NULL || stSEnv.pstMonster->iCommType != MON_UNION_TYPE_RONGLU)
	{
		set_script_variable(iRetIdx, -1);
		return 0;	
	}

	pstPlayer = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,stSEnv.pstMonster->szOwner);
	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;		
	}

	if (ronglu_player_new_collect_mon(stSEnv.pstEnv, pstPlayer, iMonID, iLeftTime) < 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_ronglu_get_taskid()
{
	int iRetIdx = get_script_number(NULL);
	int iTaskID = get_script_number(NULL);
	
	if (stSEnv.pstPlayer == NULL)
	{
		set_script_variable(iRetIdx, -1);
		return 0;	
	}

	set_script_variable(iTaskID, ronglu_get_task_id(stSEnv.pstPlayer));
	return 0;
}

int s_ronglu_get_itemid()
{
	int iRetIdx = get_script_number(NULL);
	int iItemID = get_script_number(NULL);
	
	if (stSEnv.pstPlayer == NULL)
	{
		set_script_variable(iRetIdx, -1);
		return 0;	
	}

	set_script_variable(iItemID, ronglu_get_item_id(stSEnv.pstPlayer));
	return 0;
}

int s_del_player_ride()
{
	int iIdx;
	ROLERIDELIST   *pstRideList;
	int iMemID = get_script_number(NULL);
	int iRideID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
	               z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
				   
	if(NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstRideList = &pstPlayer->stRoleData.MiscInfo.RideList;

	iIdx = z_get_package_ride(pstPlayer,  iRideID);
	if (iIdx < 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if( ((CS_STAT_RIDE_ON & pstPlayer->stOnline.State) ||  (CS_STAT_FLY & pstPlayer->stOnline.State)) && 
		pstPlayer->stOnline.RideItemID == iRideID)
	{
		z_ride_break(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,  pstPlayer);
	}
	
	pstRideList->Num--;
	if (iIdx != pstRideList->Num)
	{
		memmove(&pstRideList->RideGrids[iIdx], &pstRideList->RideGrids[iIdx+1], (pstRideList->Num - iIdx)*sizeof(RIDEGRID));
	}

	ride_list_chg(stSEnv.pstEnv, pstPlayer);

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_range_set()
{
	int iMainType, iSubType;
	ZONEMAPINST* pstMapInst;
	ZONEPWORLDINST *pstPworldInst;
	Player *pstPlayer;

	iMainType = get_script_number(NULL);
	iSubType = get_script_number(NULL);

	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		return 0;
	}
	
	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );

	pstPlayer = stSEnv.pstPlayer;
	if (iMainType == RANGE_MAIN_BATTLE)
	{
		pstPlayer = stSEnv.pstKiller;
	}
	
	range_combat_script_set(stSEnv.pstEnv, pstPlayer, iMainType, iSubType, pstPworldInst);
	
	return 0;
}

int s_run_task_progress()
{
	// 参数1: 角色的实例ID，0 是自己
	// 参数2: 任务DefID
	// 参数3: 进度变化量
	// 参数4: 返回值0为成功,<0为失败

	int iMemID = get_script_number(NULL);
	int iTaskDefID = get_script_number(NULL);
	int iProgressAdd = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
		z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if(!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if(!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, task_generic_progress_run(
		stSEnv.pstEnv, pstPlayer, iTaskDefID, iProgressAdd));
	return 0;
}

int s_player_collect_break()
{
	Player *pstPlayer = stSEnv.pstPlayer;
	Monster *pstMon;

	if(!pstPlayer)
	{
		return 0;
	}

	if (0 == (CS_STAT_COLLECT & pstPlayer->stOnline.State))
	{
		return -1;
	}

	pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer->stOnline.iCollectTarget);

	player_collect_break(stSEnv.pstEnv, pstPlayer, pstMon);
	
	return 0;
}

//iMapCurr >0 则相关的刷新怪物，道具等操作均在副本中的iMapCurr地图上
int s_set_pworld_map_curr()
{
	Player *pstPlayer = stSEnv.pstPlayer;

	if(!pstPlayer || !stSEnv.pstPworldInst)
	{
		return 0;
	}

	stSEnv.pstPworldInst->iMapCurr = GET_MAPID(pstPlayer->stRoleData.Map);
	
	return 0;
}

int s_career_level_up()
{
	// 参数1: 转职的职业类型 请参考tagCAREER_TYPE
	// 参数2: 积累数技能槽的上限
	// 参数3: 积累数技能槽的增加方式
	// 参数4: 返回值0为成功,<0为失败 -2表示已经转职过了

	int iCareer= get_script_number(NULL);
	int iCollectSkill_SlotMax = get_script_number(NULL);
	int iCollectSkill_AddType = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	Player *pstPlayer = stSEnv.pstPlayer;
	if(!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if( pstPlayer->stRoleData.Career > CAREER_CHURCH )
	{
		set_script_variable(iRetIdx, -2);
		return 0;
	}
		
      if( CAREER_WARRIOR_SINGLEHAND != iCareer && CAREER_WARRIOR_DOUBLEHAND != iCareer &&
   	   CAREER_MAGICOR_SINGLEHAND != iCareer && CAREER_MAGICOR_DOUBLEHAND != iCareer &&
   	   CAREER_GUNER_SINGLEHAND     != iCareer && CAREER_GUNER_DOUBLEHAND     != iCareer &&
   	   CAREER_CHURCH_SINGLEHAND   != iCareer && CAREER_CHURCH_DOUBLEHAND   != iCareer )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
  
	if( pstPlayer->stRoleData.Career == CAREER_WARRIOR && 
		CAREER_WARRIOR_SINGLEHAND != iCareer && 
		CAREER_WARRIOR_DOUBLEHAND != iCareer  )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	if( pstPlayer->stRoleData.Career == CAREER_MAGICOR && 
		CAREER_MAGICOR_SINGLEHAND != iCareer && 
		CAREER_MAGICOR_DOUBLEHAND != iCareer  )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	if( pstPlayer->stRoleData.Career == CAREER_GUNER && 
		CAREER_GUNER_SINGLEHAND != iCareer && 
		CAREER_GUNER_DOUBLEHAND != iCareer )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	if(pstPlayer->stRoleData.Career == CAREER_CHURCH && 
		CAREER_CHURCH_SINGLEHAND != iCareer && 
		CAREER_CHURCH_DOUBLEHAND != iCareer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	//转职了
	pstPlayer->stRoleData.Career = iCareer;
	pstPlayer->stRoleData.MiscInfo.CollectSkillInfo.CollectSkill_SlotMax = iCollectSkill_SlotMax;
	pstPlayer->stRoleData.MiscInfo.CollectSkillInfo.CollectSkill_AddType = iCollectSkill_AddType;

	relation_dbbuddy_chg(stSEnv.pstEnv, pstPlayer, 0,1);

	player_update_cache_career(stSEnv.pstEnv, pstPlayer);
	
	//记录一条转职的日志
	z_role_lvup_snapshot_oplog(stSEnv.pstEnv, pstPlayer, pstPlayer->stRoleData.Level, pstPlayer->stRoleData.RoleDetail.Exp,1);

	//下发个ACTION
	player_career_chg_action(stSEnv.pstEnv, pstPlayer);
	return 0;
}

int s_set_mon_curr()
{
	int iMemID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	Npc* pstNpc = NULL;

	Monster *pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if(!pstMon)
	{
		pstNpc = z_id_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	}

	if (!pstMon && !pstNpc)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}


	if (pstMon)
	{
		stSEnv.pstMonster = pstMon;
		if (stSEnv.iStackTop > 0)
		{
			stSEnv.astCallStack[stSEnv.iStackTop - 1].pstMonster = pstMon;
		}
	}
	else if (pstNpc)
	{
		stSEnv.pstNpc = pstNpc;
		if (stSEnv.iStackTop > 0)
		{
			stSEnv.astCallStack[stSEnv.iStackTop - 1].pstNpc = pstNpc;
		}
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_set_player_curr()
{
	int iMemID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	Player *pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if(!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	stSEnv.pstPlayer = pstPlayer;
	if (stSEnv.iStackTop > 0)
	{
		stSEnv.astCallStack[stSEnv.iStackTop - 1].pstPlayer = pstPlayer;
	}
	
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_exit_clan()
{
	int iVarIdx, iRet = 0;

	iVarIdx = get_script_number(NULL);	
	if (!stSEnv.pstPlayer)
	{
		goto fatal;
	}

	iRet = player_clan_exit(stSEnv.pstEnv, stSEnv.pstPlayer);
	set_script_variable(iVarIdx, iRet);

	return 0;

fatal:
	set_script_variable(iVarIdx, -1);
	return 0;
}

int s_use_cangbaotu_pic()
{
	Player *pstPlayer = NULL;
	int iRet = 0;
	int iItemID = 0;
	int iRadius = 0;
	int iDirection = 0;
	ROLEITEM *pstRoleItem = NULL;
	ZONEMAPINST *pstMapInst = NULL;
	MapIndex *pstMapIdx = NULL;
	int iPointDist = 0;
	int iDir = 0;
	int iMapID;
	RESPOS ItemPos; 
	CANGBAOTUINFO *pstCangBaoTuInfo = NULL;
	int i = 0;
	CSPACKAGECANGBAOTURESET stReset;
	int iVip = 0;
	RESULT *pstVipResult;
	
	/*
	  参数: 物品id,策划定义的半径,返回值,方向
	  返回值：0表示在规定的范围内. 1表示不在这个地图里,2一屏内,3一屏~二屏,4二屏以上,
	  方向：1--8表示8个方向.
	*/
	iItemID = get_script_number(NULL);
	iRadius =  get_script_number(NULL);
	iRet = get_script_number(NULL);
	iDirection = get_script_number(NULL);

	pstPlayer = stSEnv.pstPlayer;
	if(NULL == pstPlayer)
	{
		set_script_variable(iRet, -1);
		return 0;
	}
	pstCangBaoTuInfo = &pstPlayer->stRoleData.MiscInfo.CangBaoTuInfo;

	pstVipResult = star_get_result(stSEnv.pstEnv, pstPlayer, STAR_RESULT_CANGBAOTU);
	if (pstVipResult)
	{
		iVip = pstVipResult->ResultVal1;
	}
	
	if(pstCangBaoTuInfo->DayTimes+iVip<= 0)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, SYS_TIP, ZONEERR_SCRIPT303);
		set_script_variable(iRet, -1);
                return 0;
	}
	
	pstRoleItem =  stSEnv.pstRoleItem;
	if( NULL == pstRoleItem )
	{
		set_script_variable(iRet, -1);
		return 0;
	}
	//使用的物品id 和 传进来的物品id不一样,脚本写错了
	if(iItemID != stSEnv.pstRoleItem->DefID)
	{
		set_script_variable(iRet, -1);
		return 0;
	}
	
	/* 判断下是否在这个地图里 */
	pstMapInst = s_get_mapinst();
	if (pstMapInst == NULL)
	{
		set_script_variable(iRet, -1);
		return 0;
	}
	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		set_script_variable(iRet, -1);
		return -1;
	}	

	for( i=0;i<CANGBAOTU_NUM;i++)
	{
		if(pstCangBaoTuInfo->CangBaoTuMapInfo[i].ItemID == (unsigned int)iItemID)
		{
			break;
		}
	}

	//多线地图
	if(pstMapIdx->stMapDef.MapLine > 0 && pstMapIdx->stMapDef.ReferMapID > 0)
	{
		iMapID = pstMapIdx->stMapDef.ReferMapID;
	}
	else
	{
		iMapID = pstMapIdx->stMapDef.MapID;
	}
	
	if( i>=CANGBAOTU_NUM || 
		pstCangBaoTuInfo->CangBaoTuMapInfo[i].MapID != (unsigned int)iMapID )
	{
		set_script_variable(iRet, 1);
		return 0;
	}
	
	// 先移动下
	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, 0);
	}

	ItemPos = pstCangBaoTuInfo->CangBaoTuMapInfo[i].Pos;
	iPointDist = z_distance(&pstPlayer->stRoleData.Pos,&ItemPos);
	// 宝藏在规定的范围内
	if( iPointDist <= iRadius )
	{
		//成功了要重置下数据
		memset(&stReset,0,sizeof(stReset));
		stReset.ItemID = iItemID;
		package_cangbaotu_reset(stSEnv.pstEnv,pstPlayer,&stReset);
		
		//检查完成藏宝图任务
		task_cangbaotu_fini_trace(stSEnv.pstEnv,pstPlayer,iItemID);
		set_script_variable(iRet, 0);
		return 0;
	}
	else if(iPointDist < SCREEN_XGRID)
	{
		// 2一屏内
		set_script_variable(iRet, 2);
	}
	else if(iPointDist < 2*SCREEN_XGRID)
	{
		// 3一屏~二屏
		set_script_variable(iRet, 3);
	}
	else
	{
		// 4二屏以上
		set_script_variable(iRet, 4);
	}
	
	// 来判断下距离 
	iDir	=	FaceAngleBetween(ItemPos.X,  ItemPos.Y,
					pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y);
	/* 弄出八个方向来 */
	if( iDir < 67.5 &&  iDir >= 22.5 )
	{
		iDir = 8;//西北
	}
	else if( iDir <112.5 &&  iDir >= 67.5 )
	{
		iDir = 1;//北
	}
	else if( iDir <157.5 &&  iDir >= 102.5 )
	{
		iDir = 2;//东北
	}
	else if( iDir <202.5 &&  iDir >= 157.5 )
	{
		iDir = 3;//东
	}
	else if( iDir <247.5 &&  iDir >= 202.5 )
	{
		iDir = 4;//东南
	}
	else if( iDir <292.5 &&  iDir >= 247.5 )
	{
		iDir = 5;//南
	}
	else if( iDir <337.5 &&  iDir >= 292.5 )
	{
		iDir = 6;//西南
	}
	else 
	{
		iDir = 7;//西
	}
	
	set_script_variable(iDirection, iDir);
	
	return 0;
}

int s_new_linked_mon_slot()
{
	/*
	 * 参数1: 返回位置( >= 0成功, 否则失败)
	 */
	int iRetIdx = get_script_number(NULL);
	/*
	if(!stSEnv.pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(stSEnv.pstPworldInst->iLinkedMonSlotNum >=
	   (int)CCH(stSEnv.pstPworldInst->aiLinkedMonSlots))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	// 返回一个新的slot
	set_script_variable(iRetIdx, stSEnv.pstPworldInst->iLinkedMonSlotNum++);
	*/

	set_script_variable(iRetIdx, 0);
	return 0;
}


static Monster* link_mon_in_map(ZONESVRENV *pstEnv,
                                const ZONEMAPINST *pstMapInst,
                                int iGroupID,
                                Monster *pstPrev,
                                Monster **ppstHead,
                                int *piCount)
{
	int i = 0;
	Monster *pstMon = NULL;

	for(i = 0; i < pstMapInst->iAreaDyn; ++i)
	{
		const AREADYN *pstAreaDyn = NULL;
		int j = 0;
		int iDyn = pstMapInst->aiAreaDyn[i];
		if(iDyn < 0)
		{
			continue;
		}

		pstAreaDyn = tmempool_get(pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		for(j = 0; j < pstAreaDyn->iAnimate; ++j)
		{
			if(j >= MAX_AREA_ANIMATE)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d",
				           pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
				return NULL;
			}

			pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv,
			                      pstAreaDyn->aiAnimate[j]);
			if(!pstMon)
			{
				continue;
			}

			if(pstMon->iLinkedPrev > 0 || pstMon->iLinkedPrev > 0)
			{
				continue;
			}
			if(pstMon->iGroupID != iGroupID)
			{
				continue;
			}
			if(!*ppstHead)
			{
				*ppstHead = pstMon;
			}
			if(pstPrev)
			{
				pstMon->iLinkedPrev = pstPrev->iMIndex;
				pstPrev->iLinkedNext = pstMon->iMIndex;
			}
			
			++(*piCount);
			pstPrev = pstMon;
		}
	}
	return pstPrev;
}

int s_link_mon()
{
	/*
	 * 参数1: 返回位置( >= 0被链接的怪物数量, 否则失败)
	 * 参数2: 组ID
	 */

	int iRetIdx = get_script_number(NULL);
	int iGroupID = get_script_number(NULL);
	int i, iCount = 0;
	ZONEPWORLDINST *pstPworldInst = stSEnv.pstPworldInst;

	if(!pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(iGroupID <= 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	for(i = 0; i < pstPworldInst->iMapInst; ++i)
	{
		Monster *pstHead = NULL, *pstLast = NULL;
		ZONEMAPINST *pstMapInst = z_find_map_inst(
			stSEnv.pstEnv->pstAppCtx,
			stSEnv.pstEnv,
			pstPworldInst->astMapInstIdx + i);
		if(!pstMapInst)
		{
			tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0,
			           "pworld inst %d map inst %d",
			           pstPworldInst->iPworldInstID,
			           (pstPworldInst->astMapInstIdx + i)->iID);
			continue;
		}

		pstHead = NULL;
		if(pstLast)
		{
			pstLast = link_mon_in_map(stSEnv.pstEnv, pstMapInst, iGroupID, pstLast,
		                          &pstHead, &iCount);
		}
		else
		{
			pstLast = link_mon_in_map(stSEnv.pstEnv, pstMapInst, iGroupID, NULL,
		                          &pstHead, &iCount);
		}
		if(!pstLast || !pstHead)
		{
			continue;
		}
		// 封闭链表
		pstHead->iLinkedPrev = pstLast->iMIndex;
		pstLast->iLinkedNext = pstHead->iMIndex;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_player_secondary_skill()
{
	SECONDARYSKILLINFO *pstSkillInfo;
	int iMemID = get_script_number(NULL);
	int iSkillType  = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	int iLevel  = get_script_number(NULL);
	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
	               z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
				   
	if(!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstSkillInfo = get_player_secondary_skill_info(pstPlayer, iSkillType);
	if(!pstSkillInfo)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	set_script_variable(iRetIdx, 0);
	set_script_variable(iLevel, pstSkillInfo->SkillLevel);
	
	return 0;
}

int s_enter_logic_area()
{
	
	int iLogicAreaType = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer || iLogicAreaType <= 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	if(iLogicAreaType == SCENEOVERLAY_SAFE)
	{
		if(player_is_in_dart_car(stSEnv.pstEnv, stSEnv.pstPlayer))
		{
			Monster *pstMon = mon_biaoche_get_by_player(stSEnv.pstEnv, stSEnv.pstPlayer);
			if(pstMon)
			{
				pstMon->cSecenOverLay |= iLogicAreaType;
			}
		}
		stSEnv.pstPlayer->stOnline.cSecenOverLay |= iLogicAreaType;
		z_status_safearea(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,stSEnv.pstPlayer);
	} 
	//PK擂台
	else if(iLogicAreaType == SCENEOVERLAY_PK)
	{
		stSEnv.pstPlayer->stOnline.cSecenOverLay |= iLogicAreaType;
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	logic_area_action(stSEnv.pstEnv, stSEnv.pstPlayer, 0, stSEnv.pstPlayer->stOnline.cSecenOverLay);
	
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_leave_logic_area()
{
	int iLogicAreaType = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer || iLogicAreaType <= 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	if (iLogicAreaType == SCENEOVERLAY_SAFE)
	{
		if(player_is_in_dart_car(stSEnv.pstEnv, stSEnv.pstPlayer))
		{
			Monster *pstMon = mon_biaoche_get_by_player(stSEnv.pstEnv, stSEnv.pstPlayer);
			if(pstMon)
			{
				pstMon->cSecenOverLay &= ~iLogicAreaType;
			}
		}
		stSEnv.pstPlayer->stOnline.cSecenOverLay &= ~iLogicAreaType;
	}
	//PK擂台
	else if(iLogicAreaType == SCENEOVERLAY_PK)
	{
		stSEnv.pstPlayer->stOnline.cSecenOverLay &= ~iLogicAreaType;
		z_player_clean_badbuff(stSEnv.pstEnv, stSEnv.pstPlayer);
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	logic_area_action(stSEnv.pstEnv, stSEnv.pstPlayer, 1, iLogicAreaType);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_set_mon_group()
{
	/*
	 * 参数1: 返回位置( >= 0被链接的怪物数量, 否则失败)
	 * 参数2: 怪物实例id
	 * 参数3: 组id(不能为0)
	 */
	int iRetIdx = get_script_number(NULL);
	int iMonID = get_script_number(NULL);
	int iGroupID = get_script_number(NULL);
	Monster *pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,
	                               iMonID);
	if(!pstMon || iGroupID <= 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMon->iGroupID = iGroupID;
	set_script_variable(iRetIdx, 0);
	return 0;
}

static int mon_group_count_in_map(ZONESVRENV *pstEnv,
                                  const ZONEMAPINST *pstMapInst, int iGroupID,
                                  int iFlag)
{
	int i = 0, iCount = 0;
	Monster *pstMon = NULL;

	for(i = 0; i < pstMapInst->iAreaDyn; ++i)
	{
		const AREADYN *pstAreaDyn = NULL;
		int j = 0;
		int iDyn = pstMapInst->aiAreaDyn[i];
		if(iDyn < 0)
		{
			continue;
		}

		pstAreaDyn = tmempool_get(pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		for(j = 0; j < pstAreaDyn->iAnimate; ++j)
		{
			if(j >= MAX_AREA_ANIMATE)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d",
				           pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
				return iCount;
			}

			pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv,
			                      pstAreaDyn->aiAnimate[j]);
			if(pstMon && pstMon->iGroupID == iGroupID)
			{
				switch(iFlag)
				{
				case 0:
					++iCount;
					break;

				case 1:
					if(pstMon->cStat != MONSTER_DIE)
					{
						++iCount;
					}
					break;

				default:
					return 0;
				}
			}
		}
	}
	return iCount;
}


int s_pworld_linked_mon_info()
{
	/*
	 * 参数1: 返回位置(返回指定组的怪物数量)
	 * 参数2: 组id(不能为0)
	 * 参数3: 标记.
	 *        0统计所有怪物;
	 *        1只统计存活的怪物
	 */
	int i, iCount = 0;
	int iRetIdx = get_script_number(NULL);
	int iGroupID = get_script_number(NULL);
	int iFlag = get_script_number(NULL);

	ZONEPWORLDINST *pstPworldInst = stSEnv.pstPworldInst;
	if(!pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(iGroupID <= 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	for(i = 0; i < pstPworldInst->iMapInst; ++i)
	{
		ZONEMAPINST *pstMapInst = z_find_map_inst(
			stSEnv.pstEnv->pstAppCtx,
			stSEnv.pstEnv,
			pstPworldInst->astMapInstIdx + i);
		if(!pstMapInst)
		{
			tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0,
			           "pworld inst %d map inst %d",
			           pstPworldInst->iPworldInstID,
			           (pstPworldInst->astMapInstIdx + i)->iID);
			continue;
		}

		iCount += mon_group_count_in_map(stSEnv.pstEnv, pstMapInst, iGroupID,
		                                 iFlag);
	}

	set_script_variable(iRetIdx, iCount);
	return 0;
}

int s_player_new_linked_mon()
{
	/*
	 * 参数1: 返回位置(0表示成功,否则失败)
	 * 参数2: Player ID, 0 为自己
	 * 参数3: 怪物DefID
	 * 参数4: 怪物数量
	 * 参数5: x
	 * 参数6: y
	 * 参数7: 半径
	 * 参数8: 巡逻ID
	 * 参数9: 是否刷新
	 */

	int iRetIdx = get_script_number(NULL);
	int iMemID = get_script_number(NULL);
	int iDefID = get_script_number(NULL);
	int iNum = get_script_number(NULL);
	RESPOS stPos = { get_script_number(NULL),
	                 get_script_number(NULL), };
	int iRadius = get_script_number(NULL);
	int iPatrolID = get_script_number(NULL);
	int fRefresh = get_script_number(NULL);

	int i = 0;
	Monster stMon, *pstNew = NULL, *pstHead = NULL, *pstPrev = NULL;
	MONSTERDEF *pstMonDef = NULL;
	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
	               z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	ZONEMAPINST *pstMapInst = s_get_mapinst();
	MapIndex *pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj,
	                                     &pstMapInst->iMapPos,
	                                     pstMapInst->iMapInstID);
	if(!pstPlayer || !pstMapInst || !pstMapIdx)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	memset(&stMon, 0, sizeof(stMon));
	stMon.cStat = MONSTER_DIE;
	stMon.iDefIdx = iDefID;
	stMon.iDefPos = -1;
	stMon.stMap.iID = stMon.iInitMap = pstMapInst->iMapInstID;
	stMon.stMap.iPos = pstMapInst->iIndex;
	stMon.iInitMapPos = pstMapInst->iMapPos;	
	stMon.stInitPos = stPos;
	stMon.cRefresh	= fRefresh;

	pstMonDef = z_get_mon_def(stSEnv.pstEnv, &stMon);
	if(!pstMonDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	for(i = 0; i < iNum; ++i)
	{
		stMon.bDirection = RAND1(180);
		if(iRadius > 0)
		{
			z_get_ran_pos(stSEnv.pstEnv, pstMapInst, pstMapIdx,
			              &stMon.stInitPos, &stMon.stInitPos, iRadius, 0,
			              &stPos);
		}
		
		stMon.stInitPos = stPos;
		stMon.stCurrPos = stPos;

		pstNew = z_new_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stMon,
		                        pstMonDef);
		if(!pstNew)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		// 组ID就是Player的Memory ID
		pstNew->iOwnerID = pstPlayer->iMemID;
		pstNew->iGroupID = pstPlayer->iMemID;
		pstNew->iPatrolID = iPatrolID;
		pstNew->cFirstBorn = 1;
		if(z_refresh_mon(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstNew,NULL) < 0)
		{
			tmempool_free(stSEnv.pstEnv->pstMonsterPool, pstNew->iMIndex);
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		pstNew->cFirstBorn = 0;
		if(iPatrolID > 0)
		{
			pstNew->bMoveType = MOVE_TYPE_PATROL;
		}

		// 把创建的怪物链接起来
		if(!pstHead)
		{
			pstHead = pstNew;
		}
		if(pstPrev)
		{
			pstPrev->iLinkedNext = pstNew->iMIndex;
			pstNew->iLinkedPrev = pstPrev->iMIndex;
		}
		pstPrev = pstNew;
	}

	if(pstHead && pstNew && iNum > 0)
	{
		// 封闭链表
		pstHead->iLinkedPrev = pstNew->iMIndex;
		pstNew->iLinkedNext = pstHead->iMIndex;
	}
	set_script_variable(iRetIdx, 0);
	return 0;
}


int s_role_item_num()
{
	int iNum;
	int iItemID = get_script_number(NULL);
	int iItemNum = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	
	if (!stSEnv.pstPlayer || iItemID <= 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iNum = z_get_pak_item_num(stSEnv.pstEnv, &stSEnv.pstPlayer->stRoleData.Package, iItemID, -2,0,PACKAGE_DEC_NONE);
	
	set_script_variable(iItemNum, iNum);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_has_fairy_by_id()
{
	int i;
	int iFairyID = get_script_number(NULL);
	int iHas = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer || iFairyID <= 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	for(i=0; i<stSEnv.pstPlayer->stRoleData.Package.FairyPackage.FairyNum; i++)
	{
		if (stSEnv.pstPlayer->stRoleData.Package.FairyPackage.RoleFairys[i].FairyID == iFairyID)
		{
			set_script_variable(iHas, 1);
			set_script_variable(iRetIdx,0);
			return 0;
		}
	}

	set_script_variable(iHas, 0);
	set_script_variable(iRetIdx,0);
	return 0;
}

int s_get_cmp_fairyid()
{
	ROLEFAIRY *pstRoleFairy;
	int iFairyID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstRoleFairy = fairy_get_camp(stSEnv.pstEnv, stSEnv.pstPlayer);
	if (pstRoleFairy)
	{
		set_script_variable(iRetIdx, 0);
		set_script_variable(iFairyID, pstRoleFairy->FairyID);
	}
	else
	{
		set_script_variable(iRetIdx, -1);
	}
	return 0;
}

int s_is_can_bianshen()
{
	int iCan = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	Player *pstPlayer = stSEnv.pstPlayer;

	if (!pstPlayer)
	{
		set_script_variable(iRetIdx,-1);
		return 0;
	}

	set_script_variable(iCan, is_can_bianshen(stSEnv.pstEnv, pstPlayer,1,0));
	set_script_variable(iRetIdx,0);
	return 0;
}

int s_new_npc_private()
{
	int iMode;
	int iNpcID;
	int iLifeTime;
	int iX;
	int iY;
	int iDir;
	char *pszScript;
	int iRetIdx;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIndex;
	Npc *pstFind, stNpc;
	Player *pstPlayer;
	RESPOS stCurr, stOrig, stOut;

	iRetIdx = get_script_number(NULL);
	iMode = get_script_number(NULL);
	iNpcID = get_script_number(NULL);
	iLifeTime = get_script_number(NULL);
	iX = get_script_number(NULL);
	iY = get_script_number(NULL);
	iDir = get_script_number(NULL);
	pszScript = get_script_string();

	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapIndex = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIndex)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPlayer = stSEnv.pstPlayer;
	if (NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	stOut.X = iX;
	stOut.Y = iY;
	if (iX == 0 || iY == 0)
	{
		stCurr = pstPlayer->stRoleData.Pos;
		stOrig = stCurr;
		z_get_ran_pos(stSEnv.pstEnv, pstMapInst, pstMapIndex, &stCurr, &stOrig,
						200, 100, &stOut);
	}

	memset(&stNpc, 0, sizeof(stNpc));

	stNpc.stMon.iDefIdx = iNpcID;
	stNpc.stMon.stMap.iID = stNpc.stMon.iInitMap = pstMapInst->iMapInstID;
	stNpc.stMon.iInitMapPos = pstMapInst->iMapPos;
	stNpc.stMon.stMap.iPos = pstMapInst->iIndex;
	stNpc.stMon.cType = OBJ_NPC;
	stNpc.stMon.cStat = MONSTER_DIE;
	stNpc.stMon.bDirection = iDir;
	stNpc.stMon.tRefresh = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;
	stNpc.stMon.stInitPos = stOut;
	stNpc.stMon.stCurrPos = stOut;
				
	STRNCPY(stNpc.szScriptName, pszScript, sizeof(stNpc.szScriptName));
	trim(stNpc.szScriptName);

	if (stNpc.szScriptName[0] != 0)
	{
		ScriptIndex *pstScriptInd;
		
		pstScriptInd = get_script_ind(stNpc.szScriptName);
		if (!pstScriptInd)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
		memcpy(&stNpc.stScriptIdx, pstScriptInd, sizeof(stNpc.stScriptIdx));
	}

	pstFind	=	z_new_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stNpc);
	if( !pstFind )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstFind->stMon.iOwnerID = pstPlayer->iMemID;
	pstFind->stMon.cOwnerIDType = 0;	

	if (iMode)
	{
		ZoneTeam *pstZoneTeam;

		pstZoneTeam = player_team_get(stSEnv.pstEnv, pstPlayer);

		if (pstZoneTeam)
		{
			pstFind->stMon.ullOwnerApplyID = pstZoneTeam->stTeamInfo.TeamID;
			pstFind->stMon.cOwnerIDType = 1;	
		}
	}
	
	
	pstFind->stMon.cFirstBorn = 1;
	if (iLifeTime > 0)
	{
		pstFind->stMon.tLifeEnd = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec + iLifeTime;
	}
	
	pstFind->stMon.cMonMiscFlag |= MON_MISC_FLAG_IGNORE_PWORLD_OVER;
	
	if (0 > z_refresh_mon(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, (Monster *)pstFind,NULL))
	{
		tmempool_free(stSEnv.pstEnv->pstNpcPool, pstFind->stMon.iMIndex);
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	pstFind->stMon.cFirstBorn = 0;

	z_check_npc_script(stSEnv.pstEnv, pstFind);

	set_script_variable(iRetIdx, pstFind->stMon.iID);

	//newnpc_add(stSEnv.pstEnv, pstMapInst, pstFind);
	
	return 0;
}

int s_pworld_again()
{
	ZONEPWORLDINST *pstPworldInst = NULL;
	ZONEMAPINST* pstMapInst;
	ZONEPWORLD *pstPworld;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		return 0;
	}

	pstPworld	= z_find_pworld(stSEnv.pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if (NULL == pstPworld)
	{
		return 0;
	}

	pworld_again(stSEnv.pstEnv, pstPworld, pstPworldInst, 
					pstPworld->stPworldDef.InitMapID, &pstPworld->stPworldDef.InitMapPos);

	return 0;
	/*for (i=pstPworldInst->iPlayer-1; i>=0; i--)
       {
	   	pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPworldInst->aiPlayerID[i]);
		if (pstPlayer)
		{
			aiPlayerID[iPlayer++] = pstPlayer->iMemID;

			SET_PLAYER_NOMOVE(pstPlayer);
			z_player_move_stop(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer);
			z_player_leave_map(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, 1);
		}
	}

	while(pstPworldInst->iMapInst>0)
	{
		z_in_destroy_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPworld, pstPworldInst, pstPworldInst->astMapInstIdx + pstPworldInst->iMapInst - 1);
		pstPworldInst->iMapInst--;
	}

	if ((pstPworldInst->cPworldScriptFlag & PWORLD_SCRIPT_PROC_START_MAP))
	{
		pstPworldInst->bDelayScript |= PWORLD_SCRIPT_PROC_START_MAP;
	}

	pstPworldInst->iNpcNum = 0;
	pstPworldInst->iPlayer = 0;
	memset(pstPworldInst->sScriptVar, 0, sizeof(pstPworldInst->sScriptVar));
	pstPworldInst->iMonHpTotal = 0;
	pstPworldInst->iMapCurr = 0;

	for(i=0; i<pstPworld->iMap; i++)
	{
		 z_in_new_map_inst(stSEnv.pstEnv->pstAppCtx,stSEnv.pstEnv, pstPworldInst->iPworldInstID, pstPworldInst->stPworldselect.PworldDifficulty, pstPworld->astMapIdx+i);
	}
	
	iMapIndex = (pstPworldInst->iPworldInstID & 0xffff)<<16 | pstPworld->stPworldDef.InitMapID;
	stPos = pstPworld->stPworldDef.InitMapPos;

	pstIdx	= z_mappos_index(stSEnv.pstEnv->pstMapObj, NULL, iMapIndex);
	if( NULL == pstIdx )
	{
		return 0;
	}

	for (i=0; i<iPlayer; i++)
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, aiPlayerID[i]);
		if (pstPlayer)
		{
			pstPlayer->stOnline.iReviverID = 0;
			pstPlayer->stOnline.State &= ~CS_STAT_DEAD;

			player_revive_attr_chg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, 
				pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax, pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax);

			if (pstPworld->stPworldDef.InitMapID != (pstPlayer->stRoleData.Map & 0xffff))
			{
				if (pstIdx->stMapDef.ReferMapID > 0)
				{
					pstEnterMap->Map = pstIdx->stMapDef.ReferMapID;
				}
				else
				{
					pstEnterMap->Map = pstIdx->stMapDef.MapID;
				}
				pstEnterMap->RealMap = pstIdx->stMapDef.MapID;

				pstEnterMap->x = stPos.X;
				pstEnterMap->y = stPos.Y;

				pstPlayer->stOnline.iPosSeq += 1000;
				pstEnterMap->InitMoveSeq = pstPlayer->stOnline.iPosSeq;
				pstEnterMap->Dir = pstPlayer->stOnline.bDir;
				pstEnterMap->Effect = pstIdx->stMapDef.EffectID;
				pstEnterMap->CameraDir = pstPworld->stPworldDef.InitCameraDir;
				
				pstEnterMap->PworldInfoFlag = 1;
				pstEnterMap->PworldInfo.LeftTime = pstPworldInst->tEnd - stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;
				pstEnterMap->PworldInfo.PworldInstID = pstPworldInst->iPworldInstID;

				Z_CSHEAD_INIT(&pstPkg->Head, ENTER_MAP);
				z_cltmsg_send(stSEnv.pstEnv, pstPlayer, pstPkg);
			}

			z_player_entermap(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, iMapIndex, &stPos, 1);
						
			player_revive_action(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer);
		}
	}

	return 0;*/
}

int s_procevt_player_num()
{
	int i;
	int iMapID;
	int iProcevtID;
	int iRetIdx = 0;
	int iPlayerLivingIdx;
	int iPlayerTotalIdx;
	int iPlayerLiving = 0;
	int iPlayerTotal = 0;
	PROCEVENT *pstProcEvt=NULL;
	MapObj *pstMapObj = stSEnv.pstEnv->pstMapObj;
	ZONEPWORLDINST *pstPworldInst = NULL;
	ZONEMAPINST* pstMapInst;
	Player *pstPlayer;
	int iMapIndex;
	MapIndex *pstMapIdx;

	iMapID = get_script_number(NULL);
	iProcevtID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iPlayerLivingIdx = get_script_number(NULL);
	iPlayerTotalIdx = get_script_number(NULL);
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );

	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	for (i=0; i<pstMapObj->iProcDef; i++)
	{
		pstProcEvt = (PROCEVENT *)pstMapObj->sProcDef[i];
		if (iMapID == (int)pstProcEvt->MapID && iProcevtID == (int)pstProcEvt->ProcID)
		{
			break;
		}
		pstProcEvt = NULL;
	}

	if (!pstProcEvt ||
		i >= pstMapObj->iProcDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	for (i=0; i<pstPworldInst->iPlayer; i++)
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPworldInst->aiPlayerID[i]);
		if (pstPlayer)
		{
			iMapIndex = pstPlayer->stRoleData.Map & 0x0000ffff;
			pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, NULL, iMapIndex);
			if (pstMapIdx->stMapDef.ReferMapID > 0)
			{
				iMapIndex = pstMapIdx->stMapDef.ReferMapID;
			}
			
			if (iMapIndex == iMapID)
			{
				if (IsPointInProcPoly(&pstProcEvt->ProcPoly, pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y))
				{
					if (!(pstPlayer->stOnline.State & CS_STAT_DEAD))
					{
						iPlayerLiving++;
					}

					iPlayerTotal++;
				}
			}
		}
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iPlayerLivingIdx, iPlayerLiving);
	set_script_variable(iPlayerTotalIdx, iPlayerTotal);
	
	return 0;
}

int s_call_mon_team_all()
{
	int i;
	int iMonID;
	int iRadius;
	int iLifeTime;
	int iX,iY;
	ZONEMAPINST* pstMapInst;
	Player *pstPlayer2, *pstPlayer1;
	ZoneTeam *pstZoneTeam;
	RESPOS stPos;

	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		return 0;
	}

	pstPlayer1 =  stSEnv.pstPlayer;
	if (NULL == pstPlayer1)
	{
		return 0;
	}

	iMonID = get_script_number(NULL);
	iRadius =   get_script_number(NULL);
	iLifeTime = get_script_number(NULL);
	iX  = get_script_number(NULL);
	iY =   get_script_number(NULL);

	pstZoneTeam = player_team_get(stSEnv.pstEnv, pstPlayer1);
	if (NULL == pstZoneTeam)
	{
		item_call_mon(stSEnv.pstEnv, pstPlayer1, pstMapInst, 1,iMonID, 0, iLifeTime, iX, iY,0, 0, 0);
		return 0;
	}

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstPlayer2 = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstPlayer2)
		{
			stPos.X		=	(int) (iX - iRadius * cos((360.0*i/pstZoneTeam->stTeamInfo.MemberNum)*PI/180.0f));
			stPos.Y		=	(int) (iY + iRadius * sin((360.0*i/pstZoneTeam->stTeamInfo.MemberNum)*PI/180.0f));
			item_call_mon(stSEnv.pstEnv, pstPlayer2, pstMapInst, 1,iMonID, 0, iLifeTime, stPos.X, stPos.Y,0, 0, 0);
		}
	}
	
	return 0;
}

int s_commit_arm_wash_addattr()
{
	ROLEGRID *pstGrid;
	ITEMDEF *pstItemDef = NULL;
	int iListType, iGridIdx;
	int iRetIdx  = get_script_number(NULL);
	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iListType = stSEnv.pstPlayer->stOnline.stSData.stCommitOpt.astGrids[0].iListType;
	iGridIdx = stSEnv.pstPlayer->stOnline.stSData.stCommitOpt.astGrids[0].iGridIdx;

	pstGrid = get_pak_item_grid(stSEnv.pstPlayer, iListType, iGridIdx);
	if (!pstGrid || pstGrid->Type != GRID_ITEM_ARM)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstItemDef = z_get_itemdef_roleitem(stSEnv.pstEnv, &pstGrid->GridData.RoleItem);
	if (!pstItemDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstGrid->GridData.RoleArm.AddAttrNum = 0;
	memset(&pstGrid->GridData.RoleArm.AddAttr[0], 0, sizeof(pstGrid->GridData.RoleArm.AddAttr[0])*MAX_ARM_ADD_ATTR);

	item_add_attr_create(stSEnv.pstEnv, &pstGrid->GridData.RoleArm, pstItemDef);
	arm_one_grid_update(stSEnv.pstEnv, stSEnv.pstPlayer, pstGrid);

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_commit_item_info()
{
	// 参数1: 获取类型 1--DefID  2--栏位  3-- 格子位置  4-- 重要程度(白绿蓝紫) 
	//                5--是否是宠物蛋
	// 参数2: 返回值 -1为失败

	int i;
	ITEMDEF *pstItemDef = NULL;
	ROLEITEM *pstRoleItem =NULL;
	int iListType, iGridIdx, iPos = -1;
	int iType = get_script_number(NULL);
	int iRetIdx  = get_script_number(NULL);
	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iListType = stSEnv.pstPlayer->stOnline.stSData.stCommitOpt.astGrids[0].iListType;
	iGridIdx = stSEnv.pstPlayer->stOnline.stSData.stCommitOpt.astGrids[0].iGridIdx;

	if(0 > package_get_item(stSEnv.pstEnv, stSEnv.pstPlayer, iListType, iGridIdx, &pstItemDef, &pstRoleItem, &iPos))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	if(!pstRoleItem || !pstItemDef )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	switch(iType)
	{
		case 1:
			set_script_variable(iRetIdx, pstRoleItem->DefID);
			break;
		case 2:
			set_script_variable(iRetIdx, iListType);
			break;
		case 3:
			set_script_variable(iRetIdx, iGridIdx);
			break;
		case 4:
			set_script_variable(iRetIdx, pstItemDef->Important);
			break;
		case 5:
			for (i=0; i<MAX_ITEM_RESULT; i++)
			{
				if (pstItemDef->Result[i].ResultID == RESULT_PET_HATCH)
				{
					set_script_variable(iRetIdx, 0);
					return 0;
				}
			}

			set_script_variable(iRetIdx, -1);
			break;
		default:
			set_script_variable(iRetIdx, -1);
			return 0;
			break;
	}

	return 0;
}

int s_dec_item_num_byldx()
{
	int iItemID;
	int iNum;
	int iRetIdx, iListType, iGridIdx;
	ITEMDEF *pstItemDef;
	ROLEITEM *pstRoleItem;
	int iPos;
	int iRet = -1;
	ITEMDEC stDec;

	iItemID = get_script_number(NULL);
	iNum = get_script_number(NULL);
	iListType = get_script_number(NULL);
	iGridIdx = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	stDec.ID = iItemID;
	stDec.Num = iNum;
	stDec.BindNum = 0;
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (iNum > z_get_pak_item_num(stSEnv.pstEnv, &stSEnv.pstPlayer->stRoleData.Package, iItemID, -1,0,PACKAGE_DEC_NONE))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
		
	if (0 > package_get_item(stSEnv.pstEnv, stSEnv.pstPlayer, iListType, 
						iGridIdx, &pstItemDef, &pstRoleItem, &iPos))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	iRet = package_dec(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, iItemID, iNum, 
		iListType, &iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_SCRIPT);

	if (iRet >= 0)
	{
		if (iRet > 0) stDec.BindNum = iRet;
		z_role_item_dec_oplog(stSEnv.pstEnv,stSEnv.pstPlayer,&stDec,OP_ITEM_DEC_SCRIPT);
	}

	set_script_variable(iRetIdx, 0);

	return 0;
}

int s_get_role_tmp_machine()
{
	// 参数1: 角色的实例ID ，0 是自己
	// 参数2: 返回值>=0为成功,<0为失败

	ITEMDEF *pstItemDef;
	int iMemID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
		z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if(!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstItemDef = z_find_itemdef(stSEnv.pstEnv, pstPlayer->stRoleData.MiscInfo.TmpSys.TmpItemID);
	if(pstItemDef && pstItemDef->ItemTmp.Type == ITEM_TMP_MACHINE)
	{
		set_script_variable(iRetIdx, pstPlayer->stRoleData.MiscInfo.TmpSys.TmpItemID);
		return 0;
	}

	set_script_variable(iRetIdx, -1);
	return 0;
}

static int get_circle_position(ZONEMAPINST *pstMapInst, MapIndex *pstMapIdx, int iPosition, int iStep, int iRadius, RESPOS *pstCurrPos)
{
	int iFaceAngle;
	int iRound, iDist;
	RESPOS stPos;
	TMAPMASK *pstMask;

	pstMask = (TMAPMASK *)(stSEnv.pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);
	
	WindingItemAxisDir(iPosition, iStep, &iFaceAngle, &iRound);

	iDist = iRound*iRadius;
	stPos = *pstCurrPos;

	stPos.X		=	(int) (pstCurrPos->X + iDist * cos((iFaceAngle)*PI/180.0f));
	stPos.Y		=	(int) (pstCurrPos->Y - iDist * sin((iFaceAngle)*PI/180.0f));
	
	if (dyn_msk_check(stSEnv.pstEnv, pstMapInst, pstMask, pstCurrPos->X, pstCurrPos->Y, stPos.X, stPos.Y, 0,DYN_MSK_STOP) &&
		z_valid_pos(stSEnv.pstEnv->pstMapObj, pstMapIdx->stMapDef.MapID, NULL, &stPos)) 
	{
		*pstCurrPos = stPos;
		return iFaceAngle;
	}

	return -1;
}

int s_circle_new_mon()
{
	int i, iDir;
	int iMonID, iMonNum;
	RESPOS stRandPos, stPos;
	char cRefresh;
	int iPatrolId, iRandRadius, iPosition, iStep;
	int iRetIdx, iMonInstIDIdx, iLifeTime;
	Monster stMonster;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	Monster *pstFind;
	MONSTERDEF *pstMonDef;

	//怪物ID\数量
	iMonID = get_script_number(NULL);
	iMonNum = get_script_number(NULL);

	//圆心坐标
	stRandPos.X = get_script_number(NULL);
	stRandPos.Y = get_script_number(NULL);

	//摆放的圆形半径
	iRandRadius = get_script_number(NULL);

	//摆放的在圆的位置
	iPosition = get_script_number(NULL);

	//第一圈圆形摆放的数量，成倍增长
	iStep = get_script_number(NULL);
	
	iPatrolId = get_script_number(NULL);
	cRefresh = get_script_number(NULL);
	iDir = get_script_number(NULL);
	iLifeTime = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iMonInstIDIdx = get_script_number(NULL);

	pstMapInst = s_get_mapinst();

	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.stInitPos = stPos;
	stMonster.cRefresh	= cRefresh;

	pstMonDef = z_get_mon_def(stSEnv.pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (iDir < 0)
	{
		iDir = abs(iDir);
	}
	
	for (i=0; i<iMonNum; i++)
	{
		stPos = stRandPos;
		if (iRandRadius > 0)
		{
			iDir = get_circle_position(pstMapInst, pstMapIdx, iPosition++, iStep, iRandRadius, &stPos);
			if(0 > iDir)
			{
				//放不下就随机找个点
				z_get_ran_pos(stSEnv.pstEnv, pstMapInst, pstMapIdx, 
					&stMonster.stInitPos, &stMonster.stInitPos, iRandRadius, 0, &stPos);

				iDir = abs(iDir);
			}

			if(iDir >= 270 )
			{
				iDir = iDir - 270;
			}
			else
			{
				iDir = iDir + 90;
			}
		}

		stMonster.bDirection = (iDir/2)%180;
		stMonster.stInitPos = stPos;
		stMonster.stCurrPos = stPos;

		pstFind =	z_new_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stMonster, pstMonDef);
	
		if( !pstFind )
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		pstFind->iPatrolID = iPatrolId;

		pstFind->cFirstBorn = 1;
		pstFind->tLifeEnd = iLifeTime > 0?stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec+iLifeTime:0;
		if (0 > z_refresh_mon(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstFind,NULL))
		{
			tmempool_free(stSEnv.pstEnv->pstMonsterPool, pstFind->iMIndex);
			set_script_variable(iRetIdx, -1);
			return 0;
		}
		pstFind->cFirstBorn = 0;

		if (iPatrolId > 0)
		{
			pstFind->bMoveType = MOVE_TYPE_PATROL;
		}

		if (0 == i)
		{
			set_script_variable(iMonInstIDIdx, pstFind->iID);
		}
	}

	set_script_variable(iRetIdx, 0);
		
	return 0;
}

int s_apply_tmp_item()
{
	int iItemID;
	int iStay;
	int iRetIdx;
	//DropItem *pstDropItem;
	ITEMDEF *pstItemDef;
	//ZONEMAPINST *pstMapInst;
	//ZONEIDX stMapIdx;
	//RESPOS stPos;
	Player *pstPlayer = stSEnv.pstPlayer;
	ZONESVRENV* pstEnv = stSEnv.pstEnv;
	int iRet;
	
	iItemID = get_script_number(NULL);
	iStay = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	pstItemDef = z_find_itemdef(pstEnv, iItemID);
	if (NULL == pstItemDef || NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (pstItemDef->ItemTmp.Type > ITEM_TMP_MACHINE_ENERGE)
	{
		if (!is_can_bianshen(pstEnv, pstPlayer, 1, 1))
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		if (pstPlayer->stOnline.State & (CS_STAT_RIDE_ON | CS_STAT_FLY ))
		{
			z_ride_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}
	}

	iRet = tmp_apply_item(pstEnv,pstPlayer, iItemID, iStay);
	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_giveback_clan_machine()
{
	// 参数1: 目标的实例ID ，0 是自己
	// 参数2: 返回值0为成功,<0为失败

	int iMemID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
	               z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx,
	                    player_clan_commit_machine(stSEnv.pstEnv, pstPlayer,
	                                               stSEnv.pstNpc));
	return 0;
}

int s_laydown_clan_ore()
{
	// 参数1: 目标的实例ID ，0 是自己
	// 参数2: 返回值0为成功,<0为失败

	int iNum = 0;
	ROLEGRID astRoleGrid[MAX_MON_MACHINE_ORE];
	int iMemID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
	               z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(0 > player_machine_commit_ore_check(stSEnv.pstEnv, stSEnv.pstPlayer, &astRoleGrid[0], &iNum))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	//公会贡献资源，有公会就加
	player_clan_commit_ore(stSEnv.pstEnv, pstPlayer, stSEnv.pstNpc,&astRoleGrid[0], iNum);

	//获得个人资源，清理机械携带资源
	player_machine_commit_ore(stSEnv.pstEnv, stSEnv.pstPlayer, &astRoleGrid[0], iNum);
	
	set_script_variable(iRetIdx, 0);
	return 0;
}

//获取玩家PK值对应的红白名信息
int s_get_player_pkval_type()
{
	// 参数1: 目标的实例ID ，0 是自己
	// 参数2: 返回值1为白名 2 红名 3黑名 ,<0为失败
	
	int iMemID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
	               z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx,
	                    get_player_pkval_type(pstPlayer));
	return 0;
}

//关闭角色与NPC对话时打开的物品提交对话框
int s_player_close_dialog()
{
	// 参数1: 目标的实例ID ，0 是自己
	// 参数2: 返回值<0为失败
	int iMemID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
	               z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstActionRes->ID =  pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_CLOSE_DIALOG;
	pstActionRes->Data.CloseDialog.Type = 0;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(stSEnv.pstEnv, pstPlayer, &stPkg);

	set_script_variable(iRetIdx, 0);
	
	return 0;
}

/*	-lxk
	功能:关闭NPC面板
	参数:
	[1]、角色实例ID : 0表示自身，>0表示一个指定ID的其他角色
	[2]、返回标识
*/
int s_player_close_dialog1()
{
	int iMemID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
	               z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstActionRes->ID =  pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_CLOSE_DIALOG;
	pstActionRes->Data.CloseDialog.Type = 1;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(stSEnv.pstEnv, pstPlayer, &stPkg);

	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_mail_text()
{
	//参数1	邮件正文
	
	stSEnv.stDlgPkg.Body.DlgS.Type  = DLG_TYPE_OPT;
	stSEnv.iLen = 0;
	npc_addtalk();

	return 0;
}

int s_sys_mail()
{
	//参数1	返回值,0成功,-1失败
	//参数2	物品1id
	//参数3	物品1数量
	//参数4	物品2id
	//参数5	物品2数量
	//参数6	邮件标题$发件人,以$分开

	int iRetIdx = get_script_number(NULL);
	int iItem1ID = get_script_number(NULL);
	int iItem1Num = get_script_number(NULL);
	int iItem2ID = get_script_number(NULL);
	int iItem2Num = get_script_number(NULL);
	char* pszMail = get_script_string();
	//char* pszMail2 = get_script_string();
	ITEMDEF *pstItemDef = NULL;

	char szTitle[NAME_LEN];
	char szText[SHORT_DESC_LEN];
	char szSender[ROLE_NAME_LEN_SAVE];
	ROLEGRID astRoleGrid[2];
	int iCount = 0;
	Player* pstPlayer = NULL;

	if (stSEnv.pstPlayer)
	{
		pstPlayer = stSEnv.pstPlayer;
	}
	else
	{
		if (!stSEnv.pstMonster)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		if (stSEnv.pstMonster->szOwner[0] == 0)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		pstPlayer = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstMonster->szOwner);
	}

	if (NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	memset(szTitle,0,sizeof(szTitle));
	while (*pszMail)
	{
		szTitle[iCount] = *pszMail;
		iCount++;
		pszMail++;

		if (iCount >= NAME_LEN)
		{
			break;
		}

		if ('$' == *pszMail)
		{
			pszMail++;
			break;
		}
	}

	if (0 == szTitle[0] || '$' ==  szTitle[0])
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iCount = 0;//reset
	memset(szSender,0,sizeof(szSender));
	while (*pszMail)
	{
		szSender[iCount] = *pszMail;
		iCount++;
		pszMail++;

		if (iCount >= ROLE_NAME_LEN_SAVE)
		{
			break;
		}

	}

	if (0 == szSender[0] )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iCount = 0;//reset
	memset(szText,0,sizeof(szText));
	pszMail = stSEnv.stDlgPkg.Body.DlgS.Dlg.DlgOpt.Sentence;
	while (*pszMail)
	{
		if ('#' == *pszMail)
		{
			char* pszNext = pszMail + 1;
			switch (*pszNext)
			{
			//player name
			case 'n':
			case 'N':
				{
					int nLen = min(ROLE_NAME_LEN,SHORT_DESC_LEN - iCount);
					nLen = snprintf(&szText[iCount],nLen,"%s",pstPlayer->stRoleData.RoleName);
					if (nLen < 0) //failed
					{
						szText[iCount] = *pszMail;
						iCount++;
						pszMail++;
					}
					else
					{
						iCount += nLen;
						pszMail += 2;
					}					
					break;
				}
			//table space
			case 't':
			case 'T':
				{
					int nLen = min(8,SHORT_DESC_LEN - iCount);
					nLen = snprintf(&szText[iCount],nLen,"%s","       ");
					if (nLen < 0) //failed
					{
						szText[iCount] = *pszMail;
						iCount++;
						pszMail++;
					}
					else
					{
						iCount += nLen;
						pszMail += 2;
					}			
					break;
				}
			//space
			case 's':
			case 'S':
				{
					szText[iCount] = ' ';//space
					iCount++;
					pszMail += 2;
					break;
				}
			//career
			case 'c':
			case 'C':
				{
					char* pszCareer = NULL;
					switch (pstPlayer->stRoleData.Career)
					{
					case CAREER_WARRIOR:
						pszCareer = CAREER_NAME_WARRIOR;
						break;

					case CAREER_MAGICOR:
						pszCareer = CAREER_NAME_MAGICOR;
						break;

					case CAREER_GUNER:
						pszCareer = CAREER_NAME_GUNER;
						break;

					case CAREER_CHURCH:
						pszCareer = CAREER_NAME_CHURCH;
						break;

					case CAREER_WARRIOR_SINGLEHAND:
						pszCareer = CAREER_NAME_WARRIOR_SINGLEHAND;
						break;

					case CAREER_WARRIOR_DOUBLEHAND:
						pszCareer = CAREER_NAME_WARRIOR_DOUBLEHAND;
						break;

					case CAREER_MAGICOR_SINGLEHAND:
						pszCareer = CAREER_NAME_MAGICOR_SINGLEHAND;
						break;

					case CAREER_MAGICOR_DOUBLEHAND:
						pszCareer = CAREER_NAME_MAGICOR_DOUBLEHAND;
						break;

					case CAREER_GUNER_SINGLEHAND:
						pszCareer = CAREER_NAME_GUNER_SINGLEHAND;
						break;

					case CAREER_GUNER_DOUBLEHAND:
						pszCareer = CAREER_NAME_GUNER_DOUBLEHAND;
						break;

					case CAREER_CHURCH_SINGLEHAND:
						pszCareer = CAREER_NAME_CHURCH_SINGLEHAND;
						break;

					case CAREER_CHURCH_DOUBLEHAND:
						pszCareer = CAREER_NAME_CHURCH_DOUBLEHAND;
						break;

					default:
						pszCareer = NULL;
						break;
					}

					if (pszCareer)
					{
						int nLen = min(ROLE_NAME_LEN,SHORT_DESC_LEN - iCount);
						nLen = snprintf(szText+iCount,nLen,"%s",pszCareer);
						if (nLen < 0) //failed
						{
							szText[iCount] = *pszMail;
							iCount++;
							pszMail++;
						}
						else
						{
							iCount += nLen;
							pszMail += 2;
						}	
					}
					break;
				}
			default:
				szText[iCount] = *pszMail;
				iCount++;
				pszMail++;
			}

		}
		else
		{
	
			szText[iCount] = *pszMail;
			iCount++;
			pszMail++;
		}
		if (iCount >= SHORT_DESC_LEN)
		{
			break;
		}

	}

	if (0 == szText[0])
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iCount = 0;//reset
	memset(astRoleGrid,0,sizeof(astRoleGrid));
	if( 0 != iItem1ID  && 0 != iItem1Num )
	{
		if (0 == item_create(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &astRoleGrid[iCount], iItem1ID ))
		{
			if (GRID_ITEM_ITEM == astRoleGrid[iCount].Type)
			{
				pstItemDef = z_find_itemdef(  stSEnv.pstEnv, iItem1ID);					
				if(pstItemDef && pstItemDef->MaxPile && iItem1Num > pstItemDef->MaxPile)
				{
					iItem1Num = pstItemDef->MaxPile;
				}
				astRoleGrid[iCount].GridData.RoleItem.Num = iItem1Num;
			}
			iCount++;
		}
	}

	if( 0 != iItem2ID  && 0 != iItem2Num )
	{
		if (0 == item_create(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &astRoleGrid[iCount], iItem2ID ))
		{
			if (GRID_ITEM_ITEM == astRoleGrid[iCount].Type)
			{
				pstItemDef = z_find_itemdef(  stSEnv.pstEnv, iItem2ID);					
				if(pstItemDef && pstItemDef->MaxPile && iItem2Num > pstItemDef->MaxPile)
				{
					iItem2Num = pstItemDef->MaxPile;
				}
				astRoleGrid[iCount].GridData.RoleItem.Num = iItem2Num;
			}
			iCount++;
		}
	}
	
	player_sysmail_fast_send(
		stSEnv.pstEnv,
		pstPlayer->stRoleData.RoleName,
		szTitle,
		szText,
		astRoleGrid,
		iCount,
		0,
		szSender,
		0);
	set_script_variable(iRetIdx, 0);	
	return 0;
}

int s_sys_mail_bind()
{
	//参数1	返回值,0成功,-1失败
	//参数2	物品1id
	//参数3	物品1数量
	//参数4    物品1绑定不，1表示绑定
	//参数5	物品2id
	//参数6	物品2数量
	//参数7    物品2绑定不，1表示绑定
	//参数8	邮件标题$发件人,以$分开

	int iRetIdx = get_script_number(NULL);
	int iItem1ID = get_script_number(NULL);
	int iItem1Num = get_script_number(NULL);
	int iBind1 = get_script_number(NULL);
	int iItem2ID = get_script_number(NULL);
	int iItem2Num = get_script_number(NULL);
	int iBind2 = get_script_number(NULL);
	char* pszMail = get_script_string();
	//char* pszMail2 = get_script_string();
	ITEMDEF *pstItemDef = NULL;

	char szTitle[NAME_LEN];
	char szText[SHORT_DESC_LEN];
	char szSender[ROLE_NAME_LEN_SAVE];
	ROLEGRID astRoleGrid[2];
	int iCount = 0;
	Player* pstPlayer = NULL;

	if (stSEnv.pstPlayer)
	{
		pstPlayer = stSEnv.pstPlayer;
	}
	else
	{
		if (!stSEnv.pstMonster)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		if (stSEnv.pstMonster->szOwner[0] == 0)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		pstPlayer = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstMonster->szOwner);
	}

	if (NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	memset(szTitle,0,sizeof(szTitle));
	while (*pszMail)
	{
		szTitle[iCount] = *pszMail;
		iCount++;
		pszMail++;

		if (iCount >= NAME_LEN)
		{
			break;
		}

		if ('$' == *pszMail)
		{
			pszMail++;
			break;
		}
	}

	if (0 == szTitle[0] || '$' ==  szTitle[0])
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iCount = 0;//reset
	memset(szSender,0,sizeof(szSender));
	while (*pszMail)
	{
		szSender[iCount] = *pszMail;
		iCount++;
		pszMail++;

		if (iCount >= ROLE_NAME_LEN_SAVE)
		{
			break;
		}

	}

	if (0 == szSender[0] )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iCount = 0;//reset
	memset(szText,0,sizeof(szText));
	pszMail = stSEnv.stDlgPkg.Body.DlgS.Dlg.DlgOpt.Sentence;
	while (*pszMail)
	{
		if ('#' == *pszMail)
		{
			char* pszNext = pszMail + 1;
			switch (*pszNext)
			{
			//player name
			case 'n':
			case 'N':
				{
					int nLen = min(ROLE_NAME_LEN,SHORT_DESC_LEN - iCount);
					nLen = snprintf(&szText[iCount],nLen,"%s",pstPlayer->stRoleData.RoleName);
					if (nLen < 0) //failed
					{
						szText[iCount] = *pszMail;
						iCount++;
						pszMail++;
					}
					else
					{
						iCount += nLen;
						pszMail += 2;
					}					
					break;
				}
			//table space
			case 't':
			case 'T':
				{
					int nLen = min(8,SHORT_DESC_LEN - iCount);
					nLen = snprintf(&szText[iCount],nLen,"%s","       ");
					if (nLen < 0) //failed
					{
						szText[iCount] = *pszMail;
						iCount++;
						pszMail++;
					}
					else
					{
						iCount += nLen;
						pszMail += 2;
					}			
					break;
				}
			//space
			case 's':
			case 'S':
				{
					szText[iCount] = ' ';//space
					iCount++;
					pszMail += 2;
					break;
				}
			//career
			case 'c':
			case 'C':
				{
					char* pszCareer = NULL;
					switch (pstPlayer->stRoleData.Career)
					{
					case CAREER_WARRIOR:
						pszCareer = CAREER_NAME_WARRIOR;
						break;

					case CAREER_MAGICOR:
						pszCareer = CAREER_NAME_MAGICOR;
						break;

					case CAREER_GUNER:
						pszCareer = CAREER_NAME_GUNER;
						break;

					case CAREER_CHURCH:
						pszCareer = CAREER_NAME_CHURCH;
						break;

					case CAREER_WARRIOR_SINGLEHAND:
						pszCareer = CAREER_NAME_WARRIOR_SINGLEHAND;
						break;

					case CAREER_WARRIOR_DOUBLEHAND:
						pszCareer = CAREER_NAME_WARRIOR_DOUBLEHAND;
						break;

					case CAREER_MAGICOR_SINGLEHAND:
						pszCareer = CAREER_NAME_MAGICOR_SINGLEHAND;
						break;

					case CAREER_MAGICOR_DOUBLEHAND:
						pszCareer = CAREER_NAME_MAGICOR_DOUBLEHAND;
						break;

					case CAREER_GUNER_SINGLEHAND:
						pszCareer = CAREER_NAME_GUNER_SINGLEHAND;
						break;

					case CAREER_GUNER_DOUBLEHAND:
						pszCareer = CAREER_NAME_GUNER_DOUBLEHAND;
						break;

					case CAREER_CHURCH_SINGLEHAND:
						pszCareer = CAREER_NAME_CHURCH_SINGLEHAND;
						break;

					case CAREER_CHURCH_DOUBLEHAND:
						pszCareer = CAREER_NAME_CHURCH_DOUBLEHAND;
						break;

					default:
						pszCareer = NULL;
						break;
					}

					if (pszCareer)
					{
						int nLen = min(ROLE_NAME_LEN,SHORT_DESC_LEN - iCount);
						nLen = snprintf(szText+iCount,nLen,"%s",pszCareer);
						if (nLen < 0) //failed
						{
							szText[iCount] = *pszMail;
							iCount++;
							pszMail++;
						}
						else
						{
							iCount += nLen;
							pszMail += 2;
						}	
					}
					break;
				}
			default:
				szText[iCount] = *pszMail;
				iCount++;
				pszMail++;
			}

		}
		else
		{
	
			szText[iCount] = *pszMail;
			iCount++;
			pszMail++;
		}
		if (iCount >= SHORT_DESC_LEN)
		{
			break;
		}

	}

	if (0 == szText[0])
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iCount = 0;//reset
	memset(astRoleGrid,0,sizeof(astRoleGrid));
	if( 0 != iItem1ID  && 0 != iItem1Num )
	{
		if (0 == item_create(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &astRoleGrid[iCount], iItem1ID ))
		{
			if (GRID_ITEM_ITEM == astRoleGrid[iCount].Type)
			{
				pstItemDef = z_find_itemdef(  stSEnv.pstEnv, iItem1ID);					
				if(pstItemDef && pstItemDef->MaxPile && iItem1Num > pstItemDef->MaxPile)
				{
					iItem1Num = pstItemDef->MaxPile;
				}
				astRoleGrid[iCount].GridData.RoleItem.Num = iItem1Num;
			}
			
			if(iBind1 == ITEM_GET_BIND)
			{
				astRoleGrid[iCount].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
			}
			
			iCount++;
		}
	}

	if( 0 != iItem2ID  && 0 != iItem2Num )
	{
		if (0 == item_create(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &astRoleGrid[iCount], iItem2ID ))
		{
			if (GRID_ITEM_ITEM == astRoleGrid[iCount].Type)
			{
				pstItemDef = z_find_itemdef(  stSEnv.pstEnv, iItem2ID);					
				if(pstItemDef && pstItemDef->MaxPile && iItem2Num > pstItemDef->MaxPile)
				{
					iItem2Num = pstItemDef->MaxPile;
				}
				astRoleGrid[iCount].GridData.RoleItem.Num = iItem2Num;
			}

			if(iBind2 == ITEM_GET_BIND)
			{
				astRoleGrid[iCount].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
			}
			
			iCount++;
		}
	}
	
	player_sysmail_fast_send(
		stSEnv.pstEnv,
		pstPlayer->stRoleData.RoleName,
		szTitle,
		szText,
		astRoleGrid,
		iCount,
		0,
		szSender,
		0);
	set_script_variable(iRetIdx, 0);	
	return 0;
}


int s_add_designation()
{
	//参数1	返回值
	//参数2	称号ID

	int iRetIdx = get_script_number(NULL);//返回值
	int iID = get_script_number(NULL);//称号ID
	int iRet = -1;

	if (! stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iRet = player_designation_add(stSEnv.pstEnv, stSEnv.pstPlayer, iID);
	if (-1 == iRet)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int choose(int iYes,int iNo,char* pszTips,int iTimer)
{
	CSCHOOSES *pstChooseS = NULL;

	if (NULL == stSEnv.pstPlayer || !pszTips)
	{
		return -1;
	}


	stSEnv.stDlgPkg.Body.PlayerMiscSvr.PlayerMiscType = PLAYER_CHOOSE_SVR;
	pstChooseS = &stSEnv.stDlgPkg.Body.PlayerMiscSvr.PlayerMiscSvrData.ChooseS;

	memset(pstChooseS,0,sizeof(*pstChooseS));

	pstChooseS->BindType = BIND_TYPE_NAME;
	pstChooseS->MemID = RAND1(10000);
	
	if (stSEnv.pstNpc)
	{
		pstChooseS->BindType = BIND_TYPE_NPC;
		pstChooseS->MemID = stSEnv.pstNpc->stMon.iID;
	}
	else if(stSEnv.iItemDefID > 0)
	{
		pstChooseS->BindType = BIND_TYPE_ITEM;
		pstChooseS->MemID = stSEnv.iItemDefID;
	}
	else if(stSEnv.pstEvent)
	{
		pstChooseS->BindType = BIND_TYPE_EVENT;
		pstChooseS->MemID = stSEnv.pstEvent->iEventID;
	}
	
	pstChooseS->Timer = iTimer;

	add_str_replace( pstChooseS->Tips,sizeof(pstChooseS->Tips)  - 1, pszTips,1 );

	//STRNCPY(pstChooseS->Tips,pszTips,sizeof(pstChooseS->Tips));

	STRNCPY(stSEnv.pstPlayer->stOnline.stChooseInfo.szScript,stSEnv.szScript,sizeof(stSEnv.pstPlayer->stOnline.stChooseInfo.szScript));
	
	stSEnv.pstPlayer->stOnline.stChooseInfo.cChooseBindType = pstChooseS->BindType;
	stSEnv.pstPlayer->stOnline.stChooseInfo.iChooseMemID = pstChooseS->MemID;
	stSEnv.pstPlayer->stOnline.stChooseInfo.aiChooseProc[0] = iYes;
	stSEnv.pstPlayer->stOnline.stChooseInfo.aiChooseProc[1] = iNo;
	stSEnv.pstPlayer->stOnline.stChooseInfo.iChooseLevel++;

	Z_CSHEAD_INIT(&stSEnv.stDlgPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(stSEnv.pstEnv, stSEnv.pstPlayer, &stSEnv.stDlgPkg);
	return 0;
}

int s_choose()
{
	//参数1	返回值
	//参数2	yes事件号
	//参数3	no事件号
	//参数4	提示信息

	int iRetIdx = get_script_number(NULL);//返回值
	int iYes = get_script_number(NULL);//yes事件号
	int iNo = get_script_number(NULL);//no事件号
	char* pszTips = get_script_string();//提示信息

	if (NULL == stSEnv.pstPlayer || !pszTips || 0 == stSEnv.szScript[0])
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
		
	set_script_variable(iRetIdx, choose(iYes,iNo,pszTips,0));
	return 0;
}

int s_add_online_attr()
{
	//参数1	返回值
	//参数2	属性类型
	//参数3	属性值
	//参数4 播放的光效

	int iRetIdx = get_script_number(NULL);//返回值
	int iType = get_script_number(NULL);//属性类型
	int iValue = get_script_number(NULL);//属性值
	char *pszEffectID = get_script_string();//播放的光效


	PlayerChagAttrVal stAttr;
	
	TMEMBLOCK* pstMemBlock = NULL;
	Player * pstPlayer = NULL;
	int i = 0;
	

	/*
        　　　属性0:	经验
        　　　属性1:	绑银
        　　　属性2:	银币
        　　　属性3:	金券
    */
	static int saiType[SCRIPT_ADD_ATTR_NUM] = { 
		ATTR_ID_EXP,
		ATTR_ID_BULL,
		ATTR_ID_MONEY,
		ATTR_ID_BINDGODCOIN,
		};	
	char szEffct[64] = {0}
	;
	memset(&stAttr,0,sizeof(stAttr));
	memset(&szEffct,0,sizeof(szEffct));

	if (iType < 0 || iType >= SCRIPT_ADD_ATTR_NUM)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	stAttr.AttrNum = 0;
	if (iValue >= 0)
	{
		stAttr.AttrList[stAttr.AttrNum].nFlag = 0;
	}
	else
	{
		stAttr.AttrList[stAttr.AttrNum].nFlag = 1;
		iValue = -iValue;
	}

	stAttr.AttrList[stAttr.AttrNum].AttrID = saiType[iType];
	stAttr.AttrList[stAttr.AttrNum].Value.HP = iValue;
	stAttr.AttrNum++;	

	for(i = 0; i < MAX_PLAYER; i++)
	{
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(stSEnv.pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (Player *)pstMemBlock->szData;
		if (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
		{
			continue;
		}

		if (ATTR_ID_MONEY == stAttr.AttrList[0].AttrID)
		{
			if( 0 != package_add_money_test(stSEnv.pstEnv, pstPlayer, ATTR_ID_MONEY, stAttr.AttrList[0].Value.Money) )
			{
				z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx,stSEnv.pstEnv, pstPlayer,  SYS_TIP, ZONEERR_SCRIPT304, MONEY_TYPE_MONEY_NAME);
				continue;
			}

			z_money_create_oplog(stSEnv.pstEnv, pstPlayer, MONEY_OPTYPE_SCRIPT_ADDONLINE, 0, 0, stAttr.AttrList[0].Value.Money, "脚本加在线属性");
		}

		if (ATTR_ID_BULL == stAttr.AttrList[0].AttrID)
		{
			if( 0 != package_add_money_test(stSEnv.pstEnv, pstPlayer, ATTR_ID_BULL, stAttr.AttrList[0].Value.Bull) )
			{
				z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx,stSEnv.pstEnv, pstPlayer,  SYS_TIP, ZONEERR_SCRIPT304, MONEY_TYPE_BULL_NAME);
				continue;
			}

			z_money_create_oplog(stSEnv.pstEnv, pstPlayer, MONEY_OPTYPE_SCRIPT_ADDONLINE, 0, stAttr.AttrList[0].Value.Bull,0,  "脚本加在线属性");
		}

		if (ATTR_ID_BINDGODCOIN == stAttr.AttrList[0].AttrID)
		{
			if( 0 != package_add_money_test(stSEnv.pstEnv, pstPlayer, ATTR_ID_BINDGODCOIN, stAttr.AttrList[0].Value.BindGodCoin) )
			{
				z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx,stSEnv.pstEnv, pstPlayer,  SYS_TIP, ZONEERR_SCRIPT304, MONEY_TYPE_BIND_GODCOIN_NAME);
				continue;
			}

			z_bindgold_create_oplog(stSEnv.pstEnv, pstPlayer, BINDGOLD_OPTYPE_SCRIPT_ADDONLINE, 0,
									stAttr.AttrList[0].Value.BindGodCoin, "脚本加在线属性");
		}
		
		z_attr_chg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, &stAttr, 1);
		//0表示不需要特效
		if ( '0' != *pszEffectID)
		{
			snprintf(szEffct,sizeof(szEffct),"playEffct(%d,%d,%s)",pstPlayer->stRoleData.Pos.X,	pstPlayer->stRoleData.Pos.Y,pszEffectID);	
			play_effect(stSEnv.pstEnv, pstPlayer, 2, pstPlayer->iMemID, szEffct, NULL);
		}
		

		if (ATTR_ID_MONEY == stAttr.AttrList[0].AttrID)
		{
			range_single_set(stSEnv.pstEnv, pstPlayer, RANGE_MAIN_SINGLE_SUB_MONEY, 
						pstPlayer->stRoleData.RoleDetail.Money + pstPlayer->stRoleData.Package.StashPackage.Money);
		}
	}

	
	
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_learn_sec_skill()
{
	//参数1	返回值
	//参数2	生活技能类型

	int iRetIdx = get_script_number(NULL);//返回值
	int iType = get_script_number(NULL);//生活技能类型
	int iRet = 0;

	if (NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	iRet = player_learn_secondary_skill(stSEnv.pstEnv, stSEnv.pstPlayer, iType, 1);

	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_is_mon_fighting()
{
	//参数1	返回值
	//参数2	是否在战斗

	int iRetIdx = get_script_number(NULL);//返回值
	int iFightingRetIdx = get_script_number(NULL);//是否在战斗	
	int iFighting = 0;

	if (NULL == stSEnv.pstMonster)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iFighting = GET_MON_IS_FIGHTING(stSEnv.pstMonster);
	
	set_script_variable(iRetIdx, 0);
	set_script_variable(iFightingRetIdx, iFighting);
	return 0;
}

int s_set_battle_win()
{
	//参数1	返回值
	//参数2	胜利方阵营

	int iRetIdx = get_script_number(NULL);//返回值
	int iCampIdx = get_script_number(NULL);//胜利方阵营	

	ZONEPWORLD *pstPworld = NULL;
	int iRet = 0;

	if (NULL == stSEnv.pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	pstPworld	= z_find_pworld(stSEnv.pstEnv->pstMapObj, &stSEnv.pstPworldInst->stPworldIdx);
	if (NULL == pstPworld)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	stSEnv.pstPworldInst->tEnd = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec + 60;

	iRet = normal_battle_svr_info(stSEnv.pstEnv,NULL,pstPworld,stSEnv.pstPworldInst,2,iCampIdx);

	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_set_mon_camp_idx()
{
	//参数1	返回值
	//参数2	实例ID
	//参数3	阵营索引

	int iRetIdx = get_script_number(NULL);//返回值
	int iMemID = get_script_number(NULL);//实例ID
	int iCampIdx = get_script_number(NULL);//阵营索引

	Monster *pstMonster = NULL;
	int iIndex = -1;
	char cType = 0;
	TMEMBLOCK *pstMemBlock = NULL;

	if (0 == iMemID)
	{
		pstMonster = stSEnv.pstMonster;
	}
	else
	{
		iIndex = z_id_indextype(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID, &cType);
		if (iIndex >= 0)
		{

			if (cType == OBJ_MONSTER)
			{
				pstMemBlock = TMEMPOOL_GET_PTR(stSEnv.pstEnv->pstMonsterPool, iIndex);
				if (pstMemBlock->fValid)
				{
					pstMonster = (Monster *)pstMemBlock->szData;
				}
			}			
		}
	}

	if (NULL == pstMonster)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	mon_set_camp(pstMonster, iCampIdx);

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_set_revive_pos_camp()
{
	//参数1	返回值
	//参数2	复活点索引
	//参数3	阵营索引

	int iRetIdx = get_script_number(NULL);//返回值
	int iReviveIndex = get_script_number(NULL);//复活点索引	
	int iCampIdx = get_script_number(NULL);//阵营索引

	ZONEMAPINST *pstMapInst = NULL;

	if (NULL == stSEnv.pstMonster)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapInst = z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stSEnv.pstMonster->stMap);

	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	if (iReviveIndex < 0 || iReviveIndex >= BATTLE_DYN_REVIVEPOS_NUM)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapInst->aiDynReviveCamp[iReviveIndex] = iCampIdx;

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_new_dyn_mon()
{
	//参数1	返回值
	//参数2	怪物DefID
	//参数3	怪物数量
	//参数4	坐标x
	//参数5	坐标y
	//参数6	随机半径
	//参数7	巡逻ID
	//参数8	是否可以重生
	//参数9	第一只怪物的实例ID
	//参数10	阵营索引
	//参数11	动态等级
	//参数12	动态难度

	int iRetIdx = get_script_number(NULL);//返回值
	int iMonsterDefID = get_script_number(NULL);//怪物DefID
	int iMonsterNum = get_script_number(NULL);//怪物数量
	int iPosX = get_script_number(NULL);//坐标x
	int iPosY = get_script_number(NULL);//坐标y
	int iRandRadius = get_script_number(NULL);//随机半径
	int iPatrolId  = get_script_number(NULL);//巡逻ID
	int iRefresh = get_script_number(NULL);//是否可以重生
	int iMemIDRet = get_script_number(NULL);//第一只怪物的实例ID
	int iCampIdx = get_script_number(NULL);//阵营索引
	int iDynLevel = get_script_number(NULL);//动态等级
	int iDynGrade = get_script_number(NULL);//动态难度

	ZONEMAPINST *pstMapInst = NULL;
	Monster stMonster;
	MapIndex* pstMapIdx = NULL;
	MONSTERDEF* pstMonDef = NULL;
	int i = 0;
	RESPOS stPos, stCurrPos;
	Monster* pstFind = NULL;
	MONDYNATTRDEF *pstMonDynAttr = NULL;

	stPos.X = iPosX;
	stPos.Y = iPosY;
	memset(&stMonster,0,sizeof(stMonster));
	
	pstMapInst = s_get_mapinst();

	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0,  "MapInst NULL");
		return 0;
	}

	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		set_script_variable(iRetIdx, -1);
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0,  "pstMapIdx NULL");
		return 0;
	}

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonsterDefID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.stInitPos = stPos;
	stMonster.cRefresh	= iRefresh;
	mon_set_camp(&stMonster, iCampIdx);

	pstMonDef = z_get_mon_def(stSEnv.pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	//动态属性
	pstMonDynAttr = z_find_mondyn(stSEnv.pstEnv, iDynLevel);
	
	for (i=0; i<iMonsterNum; i++)
	{
		stMonster.bDirection = RAND1(180);
		stMonster.stInitPos = stCurrPos = stPos;
		
		if (iRandRadius > 0)
		{
			z_get_ran_pos(stSEnv.pstEnv, pstMapInst, pstMapIdx, &stMonster.stInitPos, &stMonster.stInitPos, iRandRadius, 0, &stCurrPos);
		}
		
		stMonster.stInitPos = stCurrPos;
		stMonster.stCurrPos = stCurrPos;

		pstFind =	z_new_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stMonster, pstMonDef);
	
		if( !pstFind )
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		pstFind->iPatrolID = iPatrolId;

		if (pstMonDynAttr && iDynGrade < MAX_DYN_GRADE)
		{
			pstFind->stMonDynAttr.Level = pstMonDynAttr->Level;
			pstFind->stMonDynAttr.HPMul = pstMonDynAttr->DynHPMul[iDynGrade];
			pstFind->stMonDynAttr.DynHurtMul = pstMonDynAttr->DynHurtMul[iDynGrade];
			pstFind->stMonDynAttr.IceHurtMul = pstMonDynAttr->IceHurtMul;
			pstFind->stMonDynAttr.PhsicHurtMul = pstMonDynAttr->PhsicHurtMul;
		}

		pstFind->cFirstBorn = 1;
		if (0 > z_refresh_mon(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstFind,NULL))
		{
			tmempool_free(stSEnv.pstEnv->pstMonsterPool, pstFind->iMIndex);
			set_script_variable(iRetIdx, -1);
			return 0;
		}
		pstFind->cFirstBorn = 0;

		if (iPatrolId > 0)
		{
			pstFind->bMoveType = MOVE_TYPE_PATROL;
		}

		if (0 == i)
		{
			stSEnv.iMemID = pstFind->iID;
			set_script_variable(iMemIDRet, pstFind->iID);
		}	
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}


int s_mon_set_owner()
{
	int iRetIdx = get_script_number(NULL);//返回值
	int iMemID = get_script_number(NULL);//怪物memid
	Monster *pstMon;

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;	
	}
	
	pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if (!pstMon)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	strncpy(pstMon->szOwner, stSEnv.pstPlayer->stRoleData.RoleName, sizeof(pstMon->szOwner));
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_new_life_dyn_mon()
{
	//参数1	返回值
	//参数2	怪物DefID
	//参数3	怪物数量
	//参数4	坐标x
	//参数5	坐标y
	//参数6	随机半径
	//参数7	巡逻ID
	//参数8	是否可以重生
	//参数9	第一只怪物的实例ID
	//参数10	阵营索引
	//参数11	动态等级
	//参数12	动态难度
	//参数13	生存时间

	int iRetIdx = get_script_number(NULL);//返回值
	int iMonsterDefID = get_script_number(NULL);//怪物DefID
	int iMonsterNum = get_script_number(NULL);//怪物数量
	int iPosX = get_script_number(NULL);//坐标x
	int iPosY = get_script_number(NULL);//坐标y
	int iRandRadius = get_script_number(NULL);//随机半径
	int iPatrolId  = get_script_number(NULL);//巡逻ID
	int iRefresh = get_script_number(NULL);//是否可以重生
	int iMemIDRet = get_script_number(NULL);//第一只怪物的实例ID
	int iCampIdx = get_script_number(NULL);//阵营索引
	int iDynLevel = get_script_number(NULL);//动态等级
	int iDynGrade = get_script_number(NULL);//动态难度
	int iLifeTime = get_script_number(NULL);

	ZONEMAPINST *pstMapInst = NULL;
	Monster stMonster;
	MapIndex* pstMapIdx = NULL;
	MONSTERDEF* pstMonDef = NULL;
	int i = 0;
	RESPOS stPos, stCurrPos;
	Monster* pstFind = NULL;
	MONDYNATTRDEF *pstMonDynAttr = NULL;

	stPos.X = iPosX;
	stPos.Y = iPosY;
	memset(&stMonster,0,sizeof(stMonster));
	
	pstMapInst = s_get_mapinst();

	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0,  "MapInst NULL");
		return 0;
	}

	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		set_script_variable(iRetIdx, -1);
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0,  "pstMapIdx NULL");
		return 0;
	}

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonsterDefID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.stInitPos = stPos;
	stMonster.cRefresh	= iRefresh;
	mon_set_camp(&stMonster, iCampIdx);

	pstMonDef = z_get_mon_def(stSEnv.pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	//动态属性
	pstMonDynAttr = z_find_mondyn(stSEnv.pstEnv, iDynLevel);
	
	for (i=0; i<iMonsterNum; i++)
	{
		stMonster.bDirection = RAND1(180);
		stMonster.stInitPos = stCurrPos = stPos;
		
		if (iRandRadius > 0)
		{
			z_get_ran_pos(stSEnv.pstEnv, pstMapInst, pstMapIdx, &stMonster.stInitPos, &stMonster.stInitPos, iRandRadius, 0, &stCurrPos);
		}
		
		stMonster.stInitPos = stCurrPos;
		stMonster.stCurrPos = stCurrPos;

		pstFind =	z_new_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stMonster, pstMonDef);
	
		if( !pstFind )
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		pstFind->iPatrolID = iPatrolId;

		if (pstMonDynAttr && iDynGrade < MAX_DYN_GRADE)
		{
			pstFind->stMonDynAttr.Level = pstMonDynAttr->Level;
			pstFind->stMonDynAttr.HPMul = pstMonDynAttr->DynHPMul[iDynGrade];
			pstFind->stMonDynAttr.DynHurtMul = pstMonDynAttr->DynHurtMul[iDynGrade];
			pstFind->stMonDynAttr.IceHurtMul = pstMonDynAttr->IceHurtMul;
			pstFind->stMonDynAttr.PhsicHurtMul = pstMonDynAttr->PhsicHurtMul;
		}

		pstFind->cFirstBorn = 1;
		pstFind->tLifeEnd = iLifeTime > 0?stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec+iLifeTime:0;
		if (0 > z_refresh_mon(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstFind,NULL))
		{
			tmempool_free(stSEnv.pstEnv->pstMonsterPool, pstFind->iMIndex);
			set_script_variable(iRetIdx, -1);
			return 0;
		}
		pstFind->cFirstBorn = 0;

		if (iPatrolId > 0)
		{
			pstFind->bMoveType = MOVE_TYPE_PATROL;
		}

		if (0 == i)
		{
			stSEnv.iMemID = pstFind->iID;
			set_script_variable(iMemIDRet, pstFind->iID);
		}	
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_attacker_camp()
{
	//参数1	返回值
	//参数2	攻击者的阵营索引

	int iRetIdx = get_script_number(NULL);//返回值
	int iCampIdx = get_script_number(NULL);//攻击者的阵营索引

	int iCamp = 0;

	if (NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iCampIdx, 0);
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	iCamp = stSEnv.pstPlayer->stOnline.stWarOL.cCampIdx;
	set_script_variable(iCampIdx, iCamp);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_player_jump()
{
	//参数1	返回值
	//参数2	坐标x
	//参数3	坐标y

	int iRetIdx = get_script_number(NULL);//返回值
	int iX = get_script_number(NULL);//坐标x
	int iY = get_script_number(NULL);//坐标y

	RESPOS stPos;
	int iRet = 0;

	if (NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if (0 == z_player_act_status(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, RES_ACTION_WALK))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	stPos.X = iX; 
	stPos.Y = iY;

 
	iRet = player_goto(stSEnv.pstEnv,stSEnv.pstPlayer,
						stSEnv.pstPlayer->stRoleData.Map,stPos.X, stPos.Y,
						stSEnv.pstPlayer->stOnline.iMapPos);

	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_mon_get_player_num_in_rect()
{
	//参数1	返回值
	//参数2	长
	//参数3	宽
	//参数4	玩家数量

	int iRetIdx = get_script_number(NULL);//返回值
	int iLength = get_script_number(NULL);//长
	int iHigh = get_script_number(NULL);//宽
	int iNumIdx = get_script_number(NULL);//玩家数量

	int iNum = 0;
	int i = 0;
	Player* pstPlayer = NULL;
	int iXdis = 0;
	int iYdis = 0;
	
	if (NULL == stSEnv.pstMonster)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	for (i = 0; i < stSEnv.pstMonster->iVPlayer; i++)
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstMonster->aiVPlayer[i]);
		if (!pstPlayer)
		{
			continue;
		}
		iXdis = abs(pstPlayer->stRoleData.Pos.X - stSEnv.pstMonster->stCurrPos.X);
		iYdis = abs(pstPlayer->stRoleData.Pos.Y - stSEnv.pstMonster->stCurrPos.Y);
		if (iXdis <= iLength/2.0 && iYdis <= iHigh/2.0)
		{
			iNum++;
		}

	}
	

	set_script_variable(iNumIdx, iNum);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_new_camp_npc()
{
	//参数1	返回值
	//参数2	NpcID
	//参数3	坐标x
	//参数4	坐标y
	//参数5	方向
	//参数6	阵营索引
	//参数7	脚本

	int iRetIdx = get_script_number(NULL);//返回值
	int iNpcID = get_script_number(NULL);//NpcID
	int iX = get_script_number(NULL);//坐标x
	int iY = get_script_number(NULL);//坐标y
	int iDir = get_script_number(NULL);//方向
	int iCampIdx = get_script_number(NULL);//阵营索引
	char* pszScript = get_script_string();//脚本

	MAPNPC stMapNpc;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIndex;
	int iRet;
	Npc *pstNpc;
		
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapIndex = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIndex)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	memset(&stMapNpc, 0, sizeof(stMapNpc));
	stMapNpc.MonsterID = iNpcID;
	stMapNpc.Pos.X = iX;
	stMapNpc.Pos.Y = iY;
	stMapNpc.Dir = iDir;
	STRNCPY(stMapNpc.Script, pszScript, sizeof(stMapNpc.Script));

	iRet = z_in_new_pworld_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMapIndex, pstMapInst, &stMapNpc,iCampIdx);
	if (0 > iRet)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, iRet);

	pstNpc = z_id_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iRet);
	if(pstNpc)
	{
		newnpc_add(stSEnv.pstEnv, pstMapInst, pstNpc);
	}
	
	return 0;
}

int s_player_movie_start()
{
	//参数1	返回值
	//参数2	动画的相对目录

	int iRetIdx = get_script_number(NULL);//返回值
	char* pszFilePath = get_script_string();//脚本

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	player_movie_start_notify(stSEnv.pstEnv, stSEnv.pstPlayer, pszFilePath);
	
	set_script_variable(iRetIdx, 0);
	return 0;
}

int npc_dynamic_option_list()
{
	// 此函数已废弃; 占位
	return 0;
}

int npc_dynamic_do_task_option()
{
	// 此函数已废弃; 占位
	return 0;
}

int s_get_clan_building_lv()
{
	//参数1	返回值索引
	//参数2	角色ID
	//参数3	公会建筑ID

	int iRetIdx = get_script_number(NULL);
	int iMemID = get_script_number(NULL);
	int iBuildingID = get_script_number(NULL);
	int iLv = -1;
	ZoneClan *pstZoneClan = NULL;
	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
	               z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstZoneClan = player_get_clan(stSEnv.pstEnv, pstPlayer);
	
	if (!pstZoneClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	if (iBuildingID >= 0 && iBuildingID < MAX_CLAN_BUILDING)
	{
		iLv = pstZoneClan->stClan.ClanCity.Buildings[iBuildingID].Level;
		if (pstZoneClan->stClan.ClanCity.Buildings[iBuildingID].Pending)
		{
			iLv += 100;
		}
	}
	else if (iBuildingID == CLAN_IDX_CITY)
	{
		iLv = pstZoneClan->stClan.ClanCity.Level;
	}
	else if (iBuildingID == 9)
	{
		iLv = pstZoneClan->stClan.ClanProf.Level;
	}
	set_script_variable(iRetIdx, iLv);
	return 0;
}

int s_get_player_task_sprite_flag()
{
	//参数1	返回值
	//参数2	标记

	int iRetIdx = get_script_number(NULL);//返回值
	int iFlagIndex = get_script_number(NULL);//标记

	int iFlag = 0;

	if (NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (stSEnv.pstPlayer->stRoleData.MiscInfo.Flag & RAND_TASK_SPRITE )
	{
		iFlag = 1;
	}
	set_script_variable(iFlagIndex, iFlag);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_mon_die_team_add_buff()
{
	int iRetIdx = get_script_number(NULL);
	int iBuff = get_script_number(NULL);
	int iSec = get_script_number(NULL);

	if (!stSEnv.pstMonster ||!stSEnv.pstEnv)
	{
		set_script_variable(iRetIdx, -1);
		return 0;	
	}
	mon_die_team_add_buff(stSEnv.pstEnv, stSEnv.pstMonster,iBuff, iSec);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_set_player_task_sprite_flag()
{
	//参数1	返回值
	//参数2	标记

	int iRetIdx = get_script_number(NULL);//返回值
	int iFlag = get_script_number(NULL);//标记

	int iTaskFlag = RAND_TASK_SPRITE;

	if (NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (0 == iFlag)
	{
		iTaskFlag = ~RAND_TASK_SPRITE;
		stSEnv.pstPlayer->stRoleData.MiscInfo.Flag &= iTaskFlag;
	}
	else
	{
		stSEnv.pstPlayer->stRoleData.MiscInfo.Flag |= iTaskFlag;
	}
	
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_battle_level_limit()
{
	//参数1	返回值
	//参数2	最小等级
	//参数3	最大等级

	int iRetIdx = get_script_number(NULL);//返回值
	int iMinIdx = get_script_number(NULL);//最小等级
	int iMaxIdx = get_script_number(NULL);//最大等级

	int iMin = 0;
	int iMax = 0;
	ZONEPWORLD* pstZonePworld = NULL;
	BATTLEDEF *pstBattleDef = NULL;
	int i = 0;

	if (NULL == stSEnv.pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	pstZonePworld = z_find_pworld(stSEnv.pstEnv->pstMapObj, &stSEnv.pstPworldInst->stPworldIdx);
	if (!pstZonePworld)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (0 == is_normal_battle_pworld(pstZonePworld))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstBattleDef = z_find_battledef(stSEnv.pstEnv, pstZonePworld->stPworldDef.BattleID);
	if (!pstBattleDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	for (i=0; i<MAX_BATTLE_PWORLD_QUEUE; i++)
	{	
		if ((int)pstZonePworld->stPworldDef.PworldID == pstBattleDef->BattleQueue[i].PworldID)
		{
			iMin = pstBattleDef->BattleQueue[i].MinLevel;
			iMax = pstBattleDef->BattleQueue[i].MaxLevel;
		
			break;
		}
	}


	set_script_variable(iMinIdx, iMin);
	set_script_variable(iMaxIdx, iMax);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_player_set_hide_task()
{
	//参数1	返回值
	//参数2	任务id

	int iRetIdx = get_script_number(NULL);//返回值
	int iTaskID = get_script_number(NULL);//任务id
	
	TASKDEF* pstTaskDef = NULL;
	int i = 0;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);		
		return 0;
	}
	if (stSEnv.pstPlayer->stRoleData.MiscInfo.TaskInfo.HideTaskInfo.HideTaskNum >= MAX_HIDE_TASK)
	{
		set_script_variable(iRetIdx, -1);		
		return 0;
	}
	pstTaskDef = task_def_find(stSEnv.pstEnv, iTaskID, NULL);
	if (NULL == pstTaskDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	i = stSEnv.pstPlayer->stRoleData.MiscInfo.TaskInfo.HideTaskInfo.HideTaskNum;
	stSEnv.pstPlayer->stRoleData.MiscInfo.TaskInfo.HideTaskInfo.HideTaskID[i] = iTaskID;
	stSEnv.pstPlayer->stRoleData.MiscInfo.TaskInfo.HideTaskInfo.HideTaskNum++;

	pstActionRes->ID = stSEnv.pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_HIDE_TASK_INFO;
	memcpy(&pstActionRes->Data.HideTaskInfo,&stSEnv.pstPlayer->stRoleData.MiscInfo.TaskInfo.HideTaskInfo,sizeof(pstActionRes->Data.HideTaskInfo));
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(stSEnv.pstEnv, stSEnv.pstPlayer, &stPkg);


	set_script_variable(iRetIdx, 0);
	return 0;
}


int s_player_call_bub()
{
	//参数1	怪物ID
	//参数2	生存时间，0则不限制

	int iMonID = get_script_number(NULL);
	int iLifeTime = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	int iRet;

	if (NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);		
		return 0;
	}

	iRet = player_call_bub(stSEnv.pstEnv, stSEnv.pstPlayer, iMonID, iLifeTime);
	if(0 >= iRet)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	set_script_variable(iRetIdx, iRet);
	return 0;
}


int s_get_player_map_line()
{
	int iLine = 0;
	ZoneTeam *pstTeam;
	int iRetIdx = get_script_number(NULL);

	if (NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);		
		return 0;
	}
	
	pstTeam = player_team_get(stSEnv.pstEnv, stSEnv.pstPlayer);
	if(pstTeam)
	{
		iLine = pstTeam->iTeamMapLine;
	}
	else
	{
		iLine = stSEnv.pstPlayer->stRoleData.MiscInfo.MapLine;
	}
	
	set_script_variable(iRetIdx, iLine);
	
	return 0;
}

int s_set_player_map_line()
{
	int iLine =  get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	if (NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);		
		return 0;
	}

	if(iLine <= 0 || iLine >MAX_MAP_LINE)
	{
		set_script_variable(iRetIdx, -1);		
		return 0;
	}
	
	stSEnv.pstPlayer->stRoleData.MiscInfo.MapLine = iLine;
	
	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_mon_push_press()
{
	//参数1	返回值
	//参数2	目标怪物Memid

	int iRetIdx = get_script_number(NULL);//返回值
	int iTargetMemID = get_script_number(NULL);//目标怪物Memid	

	Monster* pstTargetMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iTargetMemID);
	AtkTargetList *pstSourceList = NULL;
	AtkTargetList *pstTargetList = NULL;
	
	if (!stSEnv.pstMonster || !pstTargetMon)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	
	pstSourceList = &stSEnv.pstMonster->stAtkTargets;
	pstTargetList = &pstTargetMon->stAtkTargets;
	
	if (0 >= pstSourceList->PressNum)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	//我有压力,你有压力
	pstTargetList->PressNum = pstSourceList->PressNum;
	memcpy(pstTargetList->astPressEntrys,pstSourceList->astPressEntrys,sizeof(pstSourceList->astPressEntrys[0]) * pstSourceList->PressNum);
	//检查
	mon_press_update_out(stSEnv.pstEnv,pstTargetMon);
	//mon_ai_evt_outpress(stSEnv.pstEnv,pstTargetList);

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_mon_owner_dis()
{
	//参数1	返回值
	//参数2	距离

	int iRetIdx = get_script_number(NULL);//返回值
	int iDisIdx = get_script_number(NULL);//距离

	Player* pstPlayer = NULL;
	int iDis = 0;

	if (!stSEnv.pstMonster)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (stSEnv.pstMonster->szOwner[0] == 0)
	{
		return 0;
	}

	pstPlayer = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstMonster->szOwner);
	if(NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iDis = z_distance(&pstPlayer->stRoleData.Pos, &stSEnv.pstMonster->stCurrPos);

	set_script_variable(iDisIdx, iDis);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_commit_clan_item()
{
	//参数1	返回值
	//参数2	角色ID
	//参数3 提交物品在杂物包中的索引
	int iRetIdx = get_script_number(NULL);
	int iMemID = get_script_number(NULL);
	int iGridIdx = get_script_number(NULL);
	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
	               z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx,
	                    player_clan_commit_item(stSEnv.pstEnv, pstPlayer,
	                                            iGridIdx));
	return 0;
}

int s_get_clan_used_tech_point()
{
	ZoneClan *pstZoneClan = NULL;

	//参数1	返回值
	//参数2	角色ID
	//参数3 获取哪个(些)科技消耗的科技点数
	int iRetIdx = get_script_number(NULL);
	int iMemID = get_script_number(NULL);
	int iTechID = get_script_number(NULL);
	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
	               z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstZoneClan = player_get_clan(stSEnv.pstEnv, pstPlayer);
	if (!pstZoneClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, clan_used_tech_point(stSEnv.pstEnv,
	                                                  pstZoneClan, iTechID));
	return 0;
}

int s_player_trigger_mon_press()
{
	//参数1	返回值
	//参数2	玩家实例id
	//参数3	距离
	//参数4	怪物defid
	//参数5	压力值

	int iRetIdx = get_script_number(NULL);//返回值
	int iMemID = get_script_number(NULL);//玩家实例id
	int iDis = get_script_number(NULL);//距离
	int iDefID = get_script_number(NULL);//怪物defid
	int iPress = get_script_number(NULL);//压力值

	int i = 0;
	Monster* pstMon = NULL;
	MONSTERDEF* pstMonDef = NULL;
	int iNum = 0;
	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
	               z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	for(i=0;i<pstPlayer->stOnline.iVAnimate;i++)
	{
		pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx,stSEnv.pstEnv,pstPlayer->stOnline.aiVAnimate[i]);
		if (pstMon && pstMon->cStat == MONSTER_ALIVE && iDis >= z_distance(&pstMon->stCurrPos, &pstMon->stCurrPos))
		{
			if (iDefID && iDefID != pstMon->iDefIdx)
			{
				continue;
			}
			pstMonDef = z_get_mon_def(stSEnv.pstEnv, pstMon);
			if (!pstMonDef)
			{
				continue;
			}


			if ( 0 <= mon_press_insert_player(stSEnv.pstEnv, pstMon, pstMonDef,pstPlayer))
			{
				iNum++;
				//修改压力值
				if (iPress > 0)
				{
					mon_ai_press_modify(stSEnv.pstEnv, pstMon, pstPlayer->iMemID, iPress);
				}
			}
	
		
		}
	}


	set_script_variable(iRetIdx, iNum);

	return 0;
}

int s_destroy_mon_by_defid()
{
	//参数1	返回值
	//参数2	怪物defid

	int iRetIdx = get_script_number(NULL);//返回值
	int iDefID = get_script_number(NULL);//怪物defid

	static int aiIDs[MAX_AREA_ANIMATE]; // 太大了，静态分配吧
	int i = 0, iRet = -1;
	DestroyCookie stDestroyCookie;
	const ZONEMAPINST *pstMapInst = s_get_mapinst();
	int iNum = 0;
	MONSTERDEF *pstMonDef = NULL;

	bzero(&stDestroyCookie, sizeof(stDestroyCookie));
	if(!pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	stDestroyCookie.aiIDs = aiIDs;
	stDestroyCookie.iMax = CCH(aiIDs);
	stDestroyCookie.iDefID = iDefID;
	iRet = map_entity_count_if(stSEnv.pstEnv, pstMapInst, &stDestroyCookie,
	                           WillDestroyDefID);
	if(iRet < 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	for(i = 0; i < iRet; ++i)
	{
		Monster *pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,
		                               aiIDs[i]);
		if(!pstMon)
		{
			continue;
		}

		pstMonDef = z_get_mon_def(stSEnv.pstEnv, pstMon);
		if (pstMonDef)
		{
			ZoneAni stAni;

			pstMon->cFirstAtkerType = 0;
			pstMon->ullFirstAtkerWid = 0;
			pstMon->iFirstAtkerID = 0; //没有战利品
			pstMon->szOwner[0] = 0;
			stAni.iType = OBJ_UNKNOW;
			
			z_mon_die(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMon, pstMonDef, &stAni, 2, 0);
			iNum++;
		}
		else
		{
			z_free_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMon);
		}
	}


	set_script_variable(iRetIdx, iNum);
	return 0;
}

int s_mon_range_add_player_hp()
{
	//参数1	返回值
	//参数2	范围半径
	//参数3	HP

	int iRetIdx = get_script_number(NULL);//返回值
	int iRange = get_script_number(NULL);//范围半径
	int iHP = get_script_number(NULL);//HP

	int i = 0;
	Player* pstPlayer = NULL;
	int iNum = 0;
	int iXdis = 0;
	int iYdis = 0;
	int iDis = 0;
	int iOldHP = 0;
	int iOldMP = 0;
	PlayerChagAttrVal stAttr;

	if (NULL == stSEnv.pstMonster)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	stAttr.AttrNum = 1;
	stAttr.AttrList[0].AttrID = ATTR_ID_HP;
	stAttr.AttrList[0].nFlag = 0;
	
	for (i = 0; i < stSEnv.pstMonster->iVPlayer; i++)
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstMonster->aiVPlayer[i]);
		if (!pstPlayer)
		{
			continue;
		}
		iXdis = abs(pstPlayer->stRoleData.Pos.X - stSEnv.pstMonster->stCurrPos.X);
		iYdis = abs(pstPlayer->stRoleData.Pos.Y - stSEnv.pstMonster->stCurrPos.Y);
		iDis = hypot(iXdis, iYdis);
		if (iDis > iRange)
		{
			continue;
		}
	
		iNum++;

		iOldHP = pstPlayer->stRoleData.RoleDetail.HPCurr;
		iOldMP = pstPlayer->stRoleData.RoleDetail.MPCurr;

		if(iHP < 0)
		{
			stAttr.AttrList[0].nFlag = 1;
			stAttr.AttrList[0].Value.HP = -iHP;
		}
		else
		{
			stAttr.AttrList[0].nFlag = 0;
			stAttr.AttrList[0].Value.HP = iHP;
		}
		
		z_attr_chg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, &stAttr,	0);

		player_hp_action(stSEnv.pstEnv, pstPlayer, iOldHP, iOldMP, 0);
	}

	set_script_variable(iRetIdx, iNum);
	return 0;
}

int s_mon_get_killer_id()
{
	int iRetIdx = get_script_number(NULL);
	int iID =  get_script_number(NULL);

	if (NULL == stSEnv.pstKiller)
	{
		set_script_variable(iRetIdx, -1);		
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iID, stSEnv.pstKiller->iMemID);
	
	return 0;
}

int s_get_world_first_start_time()
{
	//参数1	返回值
	//参数2	时间

	int iRetIdx = get_script_number(NULL);//返回值
	int iTime = get_script_number(NULL);//时间

	set_script_variable(iTime, stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.NewWorldFlags.FirstStartupTime);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_add_bind_item_bat_inpak()
{
	int iItemID[ITEM_BAT_NUMMAX];
	int iNumIdx[ITEM_BAT_NUMMAX];
	int iNum ;
	int iFlag;
	int iRes;
	int iRetIdx;
	int i;
	ROLEGRID stRoleGrid[ITEM_BAT_NUMMAX], stDummy;

	iNum = get_script_number(NULL);

	if ( iNum <= 0 && iNum > ITEM_BAT_NUMMAX ) 
	{
		return -1;
	}
	
	for (i =0 ; i< ITEM_BAT_NUMMAX; i++)
	{
		iItemID[i] = get_script_number(NULL);
		iNumIdx[i] = get_script_number(NULL);
	}
	
	iFlag = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	for (i =0 ; i< iNum; i++)
	{
		//超过全局限制就不掉落了
		if(0 > world_item_limit_check(stSEnv.pstEnv, iItemID[i]))
		{
			set_script_variable(iRetIdx, -2);
			return 0;
		}
		
		if (0 == item_create(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stRoleGrid[i], iItemID[i] ) )
		{
				stRoleGrid[i].GridData.RoleItem.Num = iNumIdx[i];

				stRoleGrid[i].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
		}
		else 
		{
				set_script_variable(iRetIdx, -1);
				return 0;
		}
	}

	//0 能加就加 1必须全部加进去   2 仅仅测试是否能全部放进去
	if(iFlag > 0)
	{
		iRes=package_add_test(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, &stRoleGrid[0], iNum,0);

		if (iRes == -1)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		if(iFlag == 2)
		{
			set_script_variable(iRetIdx, 0);
			return 0;
		}
	}

	for (i =0 ; i< iNum; i++)
	{
		//op_log
		memcpy(&stDummy, &stRoleGrid[i], sizeof(stDummy));
		
		package_add(stSEnv.pstEnv->pstAppCtx , stSEnv.pstEnv , stSEnv.pstPlayer, &stRoleGrid[i], 1,0, IMPITEM_OPTYPE_SCRIPT);
		
		//记录全局限制的物品产出
		if(iNumIdx[i] - stRoleGrid[i].GridData.RoleItem.Num > 0)
		{
			world_item_limit_add(stSEnv.pstEnv, 
				stRoleGrid[i].GridData.RoleItem.DefID, 
				iNumIdx[i] - stRoleGrid[i].GridData.RoleItem.Num);

			stDummy.GridData.RoleItem.Num = iNumIdx[i]-stRoleGrid[i].GridData.RoleItem.Num;
			z_role_pickitem_oplog(stSEnv.pstEnv,stSEnv.pstPlayer, &stDummy, 1, OP_SCRIPT);
		}
	}
	
	set_script_variable(iRetIdx, 0);
	
	return 0;
	
	
}

int s_add_bind_item_num_inpak()
{
	int iItemID;
	int iNumIdx;
	int iFlag;
	int iRes;
	int iRetIdx;
	ROLEGRID stRoleGrid, stDummy;
	
	iItemID = get_script_number(NULL);
	iNumIdx = get_script_number(NULL);
	iFlag = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	//超过全局限制就不掉落了
	if(0 > world_item_limit_check(stSEnv.pstEnv, iItemID))
	{
		set_script_variable(iRetIdx, -2);
		return 0;
	}
	
	if (0 == item_create(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stRoleGrid, iItemID))
	{
		stRoleGrid.GridData.RoleItem.Num = iNumIdx;
		stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
		
		//0 能加就加 1必须全部加进去   2 仅仅测试是否能全部放进去
		if(iFlag > 0)
		{
			iRes=package_add_test(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, &stRoleGrid,  1,0);
			if (iRes == -1)
			{
				set_script_variable(iRetIdx, iRes);
				return 0;
			}

			if(iFlag == 2)
			{
				set_script_variable(iRetIdx, 0);
				return 0;
			}
		}	

		//op_log
		memcpy(&stDummy, &stRoleGrid, sizeof(stDummy));
		
		if ( 0 > package_add(stSEnv.pstEnv->pstAppCtx , stSEnv.pstEnv , stSEnv.pstPlayer, &stRoleGrid, 1,0, IMPITEM_OPTYPE_SCRIPT))
		{
			z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,stSEnv.pstPlayer,SYS_MIDDLE2, ZONEERR_NOSPACE);
		}

		//记录全局限制的物品产出
		if(iNumIdx - stRoleGrid.GridData.RoleItem.Num > 0)
		{
			world_item_limit_add(stSEnv.pstEnv, 
				stRoleGrid.GridData.RoleItem.DefID, 
				iNumIdx-stRoleGrid.GridData.RoleItem.Num);

			stDummy.GridData.RoleItem.Num = iNumIdx-stRoleGrid.GridData.RoleItem.Num;
			z_role_pickitem_oplog(stSEnv.pstEnv,stSEnv.pstPlayer, &stDummy, 1, OP_SCRIPT);
		}

		set_script_variable(iRetIdx, 0);
		
		return 0;

	}
	
	set_script_variable(iRetIdx, -1);
	
	return 0;
}


int s_check_world_limit()
{
	int iItemID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	if(0 > world_item_limit_check(stSEnv.pstEnv, iItemID))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_add_bind_item_num_inpak_money()
{
	int iItemID;
	int iNumIdx;
	int iFlag;
	int iRes;
	int iRetIdx;
	ROLEGRID stRoleGrid, stDummy;
	int iMoney;
	
	iItemID = get_script_number(NULL);
	iNumIdx = get_script_number(NULL);
	iFlag = get_script_number(NULL);
	iMoney = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	//超过全局限制就不掉落了
	if(0 > world_item_limit_check(stSEnv.pstEnv, iItemID))
	{
		if (iMoney > 0)
		{
			package_add_money(stSEnv.pstEnv, stSEnv.pstPlayer, ATTR_ID_BULL,iMoney);
			z_money_create_oplog(stSEnv.pstEnv, stSEnv.pstPlayer, MONEY_OPTYPE_SCRIPT_ADD, iItemID,  iMoney, 0, "世界道具限制补偿");
		}
		set_script_variable(iRetIdx, -2);
		return 0;
	}
	
	if (0 == item_create(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stRoleGrid, iItemID))
	{
		stRoleGrid.GridData.RoleItem.Num = iNumIdx;
		stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
		
		//0 能加就加 1必须全部加进去   2 仅仅测试是否能全部放进去
		if(iFlag > 0)
		{
			iRes=package_add_test(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, &stRoleGrid,  1,0);
			if (iRes == -1)
			{
				set_script_variable(iRetIdx, iRes);
				return 0;
			}

			if(iFlag == 2)
			{
				set_script_variable(iRetIdx, 0);
				return 0;
			}
		}	

		//op_log
		memcpy(&stDummy, &stRoleGrid, sizeof(stDummy));
		
		if ( 0 > package_add(stSEnv.pstEnv->pstAppCtx , stSEnv.pstEnv , stSEnv.pstPlayer, &stRoleGrid, 1,0, IMPITEM_OPTYPE_SCRIPT))
		{
			z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,stSEnv.pstPlayer,SYS_MIDDLE2, ZONEERR_NOSPACE);
		}

		//记录全局限制的物品产出
		if(iNumIdx - stRoleGrid.GridData.RoleItem.Num > 0)
		{
			world_item_limit_add(stSEnv.pstEnv, 
				stRoleGrid.GridData.RoleItem.DefID, 
				iNumIdx-stRoleGrid.GridData.RoleItem.Num);

			stDummy.GridData.RoleItem.Num = iNumIdx-stRoleGrid.GridData.RoleItem.Num;
			z_role_pickitem_oplog(stSEnv.pstEnv,stSEnv.pstPlayer, &stDummy, 1, OP_SCRIPT);
		}

		set_script_variable(iRetIdx, 0);
		
		return 0;

	}
	
	set_script_variable(iRetIdx, -1);
	
	return 0;
}

int s_battle_min_camp_player_num()
{
	int i, MinPlayerNum;
	ZONEMAPINST *pstMapInst;
	ZONEPWORLDINST *pstPworldInst;
	int iRetIdx, iNum;
	int iCampID = 0;

	iRetIdx = get_script_number(NULL);
	iNum = get_script_number(NULL);;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst ||pstPworldInst->stPworldCamp.iCampNum <= 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	MinPlayerNum = pstPworldInst->stPworldCamp.stCampInfo[0].iPlayerNum;
	iCampID = 1;
	for(i = 0; i<pstPworldInst->stPworldCamp.iCampNum; i++)
	{
		if(pstPworldInst->stPworldCamp.stCampInfo[i].iPlayerNum <= MinPlayerNum)
		{
			if (pstPworldInst->stPworldCamp.stCampInfo[i].stMapShowMon.iDefID )
			{
				if (pstPworldInst->stPworldCamp.stCampInfo[i].stMapShowMon.iHP > 0)
				{
					MinPlayerNum = pstPworldInst->stPworldCamp.stCampInfo[i].iPlayerNum;
					iCampID = i+1;
				}
			}
			else
			{
				MinPlayerNum = pstPworldInst->stPworldCamp.stCampInfo[i].iPlayerNum;
				iCampID = i+1;
			}
		}
	}

	set_script_variable(iRetIdx, iCampID);
	set_script_variable(iNum, MinPlayerNum);
	
	return 0;
}

int s_new_mon_machine_by_dir()
{
	//参数1	返回值
	//参数2	怪物机械DefID
	//参数3	能量
	//参数4	数量
	//参数5	坐标x
	//参数6	坐标y
	//参数7	实例id
	//参数8	方向

	int iRetIdx = get_script_number(NULL);//返回值
	int iDefID = get_script_number(NULL);//怪物机械DefID
	int iEnergy = get_script_number(NULL);//能量
	int iNum = get_script_number(NULL);//数量
	int iX = get_script_number(NULL);//坐标x
	int iY = get_script_number(NULL);//坐标y
	int iMemIdx = get_script_number(NULL);//实例id
	int iDir = get_script_number(NULL);//方向

	RESPOS pos;
	Monster* pstMachine = NULL;
	int i = 0;
	int iMemID = 0;

	pos.X = iX;
	pos.Y = iY;
	
	stSEnv.pstMapinst = s_get_mapinst();		

	if (NULL == stSEnv.pstMapinst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;	
	}
	for (i = 0; i < iNum; i++)
	{
		//产出一个空燃料空弹药的机械
		pstMachine = new_mon_machine_dir(stSEnv.pstEnv, stSEnv.pstMapinst, iDefID, &pos, MACHINE_OWNER_NONE, NULL,iDir, 0, 0,0);
		if(pstMachine )
		{
			if (0 == iEnergy)
			{
				iEnergy = pstMachine->stCommUnion.stMachine.stMachineInfo.MaxEnergy;
			}
			//燃料
			z_machine_attr_chg(stSEnv.pstEnv, pstMachine, MON_MACHINE_ATTR_ENERGY, iEnergy);
			if (0 == iMemID)
			{
				iMemID = pstMachine->iID;
			}

		}

		
	}
	if (0 == iMemID)
	{
		set_script_variable(iRetIdx, 1);
		return 0;
	}
	
	set_script_variable(iMemIdx, iMemID);
	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int  s_id_player_motion()
{
	int iPlayerID;
	int iRetIdx;	
	int iMotionID;
	Player *pstPlayer;

	iPlayerID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iMotionID = get_script_number(NULL);
	
	if (0 == iPlayerID)
	{
		pstPlayer = stSEnv.pstPlayer;
	}
	else
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iPlayerID);
	}
	
	if (NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	set_script_variable(iRetIdx, 0);
	set_script_variable(iMotionID, pstPlayer->stOnline.stMotionInfo.MotionID);
	return 0;
}

int s_player_need_new_name()
{
	//参数1	返回值

	int iRetIdx = get_script_number(NULL);//返回值
	int iNeed = 0;

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, iNeed);
		return 0;
	}

	/*不再数星星
	// 名字有星星
	if (strstr(stSEnv.pstPlayer->stRoleData.RoleName, "*") != NULL) 
	{
	      iNeed = 1;
	}       
	*/

	//数次数
	if (stSEnv.pstPlayer->stRoleData.MiscInfo.NameChangeTimes < MAX_NAME_CHANGE)
	{
		iNeed = 1;
	}
	
	set_script_variable(iRetIdx, iNeed);
	return 0;
}

int s_clan_need_new_name()
{
	//参数1	返回值

	int iRetIdx = get_script_number(NULL);//返回值
	int iNeed = 0;
	ZoneClan *pstZoneClan = NULL;     
	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, iNeed);
		return 0;
	}

	pstZoneClan =  player_get_clan(stSEnv.pstEnv, stSEnv.pstPlayer); 
	if (!pstZoneClan)
	{
		set_script_variable(iRetIdx, iNeed);
		return 0;
	}
	
	// 不是会长
	if (strcmp(pstZoneClan->stClan.ClanBrief.Leader.RoleName, stSEnv.pstPlayer->stRoleData.RoleName) != 0)
	{
		set_script_variable(iRetIdx, iNeed);
		return 0;
	}
	
	// 公会名字有星星
	if (strstr(stSEnv.pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Name, "*") != NULL)
	{
		iNeed = 1;
	}     

	set_script_variable(iRetIdx, iNeed);
	return 0;
}

int s_get_pworld_first_player_id()
{
	int iPlayerMemID;
	int iRetIdx;
	ZONEPWORLDINST *pstPworldInst = NULL;
	ZONEMAPINST* pstMapInst;
	
	iRetIdx = get_script_number(NULL);
	iPlayerMemID = get_script_number(NULL);

	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(pstPworldInst->iPlayer <= 0)
	{
		set_script_variable(iRetIdx, -1);
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iPlayerMemID, pstPworldInst->aiPlayerID[0]);

	return 0;
}


int s_is_one_mon_fighting_by_defid()
{
	//参数1	返回值
	//参数2	怪物Defid

	int iRetIdx = get_script_number(NULL);//返回值
	int iDefid = get_script_number(NULL);//怪物Defid
	int iFighting = 0;


	static int aiIDs[MAX_AREA_ANIMATE]; // 太大了，静态分配吧
	int i = 0, iRet = -1;
	DestroyCookie stDestroyCookie;
	const ZONEMAPINST *pstMapInst = s_get_mapinst();

	bzero(&stDestroyCookie, sizeof(stDestroyCookie));
	if(!pstMapInst)
	{
		goto final;
	}

	stDestroyCookie.aiIDs = aiIDs;
	stDestroyCookie.iMax = CCH(aiIDs);
	stDestroyCookie.iDefID = iDefid;
	iRet = map_entity_count_if(stSEnv.pstEnv, pstMapInst, &stDestroyCookie,
	                           WillDestroyDefID);
	if(iRet < 0)
	{
		goto final;
	}

	for(i = 0; i < iRet; ++i)
	{
		Monster *pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,
		                               aiIDs[i]);
		if(!pstMon)
		{
			continue;
		}
		iFighting += GET_MON_IS_FIGHTING(pstMon);
	}
	
final:
	set_script_variable(iRetIdx, iFighting);
	return 0;
}

int s_set_item_use_test_ret()
{
	//参数1 返回值
	//参数2 测试结果

	int iRetIdx = get_script_number(NULL);//返回值
	int iTestRet = get_script_number(NULL);//测试结果

	stSEnv.iReturnFlag |= RETURN_FLAG_INT;
	stSEnv.iReturnValue = iTestRet;
	
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_set_script_int_ret()
{
	//参数1 返回结果

	int iRet = get_script_number(NULL);//返回结果

	stSEnv.iReturnFlag |= RETURN_FLAG_INT;
	stSEnv.iReturnValue = iRet;

	return 0;
}

int s_get_player_num_range()
{
	//参数1 返回结果
	//参数2 坐标x
	//参数3 坐标y
	//参数4 范围半径

	int iRetIdx = get_script_number(NULL);//返回结果
	int iX = get_script_number(NULL);//坐标x
	int iY = get_script_number(NULL);//坐标y
	int iRange = get_script_number(NULL);//范围半径

	ZONEIDX stIdx;
	ZONEPWORLDINST *pstPworldInst = NULL;
	ZONEMAPINST *pstMapInst = NULL; 
	ZONEMAPINST *pstPlayerMapInst = NULL; 
	int i = 0;
	Player* pstPlayer = NULL;
	int iRet = 0;
	RESPOS pos;

	pos.X = iX;
	pos.Y = iY;


	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );

	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	for (i=0; i<pstPworldInst->iPlayer; i++)
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPworldInst->aiPlayerID[i]);
		//只要活得
		if (pstPlayer && !(pstPlayer->stOnline.State & CS_STAT_DEAD))
		{
			stIdx.iID = pstPlayer->stRoleData.Map;
			stIdx.iPos = pstPlayer->stOnline.iMapPos;
			pstPlayerMapInst = z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stIdx);
			
			if (pstPlayerMapInst == pstMapInst)
			{				
				if (z_distance(&pos, &pstPlayer->stRoleData.Pos) < iRange)
				{
					iRet++;
				}
			}
		}
	}

	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_pworld_open_award_ui()
{
	ZONEPWORLD *pstPworld;
	
	if(!stSEnv.pstPworldInst)
	{
		return 0;
	}

	pstPworld = z_find_pworld(stSEnv.pstEnv->pstMapObj, &stSEnv.pstPworldInst->stPworldIdx);
	if(NULL == pstPworld)
	{
		return 0;
	}

	pworld_award_open_ui(stSEnv.pstEnv, stSEnv.pstPworldInst, pstPworld);
	
	return 0;
}

int s_get_mul_commit_item_info()
{
	//参数1	返回标志
	//参数2	提交格子位置
	//参数3	获取类型
	//参数4	获取值

	int i;
	int iRetIdx = get_script_number(NULL);//返回标志
	int iCommitGridIdx = get_script_number(NULL);//提交格子位置
	int iType = get_script_number(NULL);//获取类型
	int iValueIdx = get_script_number(NULL);//获取值
	DialogCommitOpt* pstOpt = NULL;
	int iValue = 0;
	DialogCommitInfo* pstInfo = NULL;
	int iPos = 0;
	ITEMDEF* pstItemDef = NULL;
	ROLEITEM* pstRoleItem = NULL;

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstOpt = &stSEnv.pstPlayer->stOnline.stSData.stCommitOpt;

	if (iCommitGridIdx >= pstOpt->iNum || iCommitGridIdx < 0 
		|| iCommitGridIdx >= MAX_DIALOG_COMMIT_NUM)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstInfo = &pstOpt->astGrids[iCommitGridIdx];


	if(0 > package_get_item(stSEnv.pstEnv, stSEnv.pstPlayer, pstInfo->iListType, pstInfo->iGridIdx, &pstItemDef, &pstRoleItem, &iPos))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if(!pstRoleItem || !pstItemDef )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	switch(iType)
	{
		case 1:
			iValue = pstRoleItem->DefID;
			break;
		case 2:
			iValue =  pstInfo->iListType;
			break;
		case 3:
			iValue = pstInfo->iGridIdx;
			break;
		case 4:
			iValue = pstItemDef->Important;
			break;
		case 5:	
			for (i=0; i<MAX_ITEM_RESULT; i++)
			{
				if (pstItemDef->Result[i].ResultID == RESULT_PET_HATCH)
				{
					set_script_variable(iValueIdx, 1);
					set_script_variable(iRetIdx, 0);
					return 0;
				}
			}

			iValueIdx = 0;
		default:
			set_script_variable(iRetIdx, -1);
			return 0;
			break;
	}

	
	set_script_variable(iValueIdx, iValue);
	set_script_variable(iRetIdx, 0);
	return 0;
}


int s_set_dlg_commit_num()
{
	//参数1 返回结果
	//参数2 数量

	int iRetIdx = get_script_number(NULL);//返回结果
	int iNum = get_script_number(NULL);//数量

	CSDLGS *pstDlgS = &stSEnv.stDlgPkg.Body.DlgS;

	if (iNum != 1 && iNum != 10)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
		

	pstDlgS->Dlg.DlgCommit.CommitNum = iNum;

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_world_ore_new_check()
{
	int iDefID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	if(0 > global_world_ore_new_check(stSEnv.pstEnv, iDefID))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}


int commit_item_sum(DialogCommitIDInfo* pstIDInfo,int iSize,int iID,int iNum)
{
	int i = 0;

	if (!iID || !iNum)
	{
		return -1;
	}
	
	for (i = 0 ; i < iSize; i++)
	{
		if ( 0 == pstIDInfo[i].iItemID )
		{
			break;
		}
		
		if (pstIDInfo[i].iItemID == iID)
		{
			pstIDInfo[i].iNum += iNum;
			return 0;
		}

	}

	if ( i >= iSize)
	{
		return -1;
	}


	pstIDInfo[i].iItemID = iID;
	pstIDInfo[i].iNum = iNum;

	return 0;
}

int s_commit_mul_clan_item()
{
	//参数1 返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	DialogCommitOpt* pstOpt = NULL;
	DialogCommitIDInfo astIDs[MAX_DIALOG_COMMIT_NUM];// 提交物品的id和数量信息
	int i = 0;
	ITEMDEF* pstItemDef = NULL;
	ROLEITEM* pstRoleItem = NULL;
	int iPos = 0;
	char szTip[128];
	char szTmp[128];
	int iTip = 0;

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstOpt = &stSEnv.pstPlayer->stOnline.stSData.stCommitOpt;

	for(i = 0; i < pstOpt->iNum; i++)
	{
		if(0 > package_get_item(stSEnv.pstEnv, stSEnv.pstPlayer, pstOpt->astGrids[i].iListType, pstOpt->astGrids[i].iGridIdx, &pstItemDef, &pstRoleItem, &iPos))
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
		
		if (NULL == pstRoleItem)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
		
		commit_item_sum(astIDs,MAX_DIALOG_COMMIT_NUM,pstRoleItem->DefID,pstRoleItem->Num);
	}
	
	szTip[0] = 0;
	snprintf(szTip,sizeof(szTip),LJY_SCRIPT1);
	for(i = 0; i < MAX_DIALOG_COMMIT_NUM; i++)
	{
		DialogCommitIDInfo* pstInfo = &astIDs[i];
		if (!pstInfo->iItemID )
			break;

		if (0 == player_clan_commit_item_byid(stSEnv.pstEnv, stSEnv.pstPlayer,pstInfo->iItemID,&pstInfo->iNum))
		{
			CLANPRODUCTDEF* pstProductDef = clan_product_def(stSEnv.pstEnv, pstInfo->iItemID);
			if (pstProductDef)
			{
				snprintf(szTmp,sizeof(szTmp),"%s",szTip);
				snprintf(szTip,sizeof(szTip),"%s [%s]x%d",szTmp,pstProductDef->Name,pstInfo->iNum);
				iTip++;
			}
		}
		
	}
	
	if (iTip > 0)
	{
		szTip[sizeof(szTip)-1] = 0;
		
		z_sys_strf(stSEnv.pstEnv, stSEnv.pstPlayer, SYS_TIP, szTip);
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}




int s_mon_owner_task_check()
{
	int i;
	int iPlayerNum;
	Player *apstPlayer[MAX_TEAM_MEMBER]; 
	Player *pstPlayerTmp;
	ZoneTeam *pstTeam;
	TASKRUNONE *pstTaskRunOne;
	int iTaskID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	if(!stSEnv.pstMonster)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iPlayerNum = 0;
	if(stSEnv.pstMonster->cFirstAtkerType == MON_BOOTY_OWNER_TEAM)
	{
		pstTeam = z_idx_team(stSEnv.pstEnv, stSEnv.pstMonster->iFirstAtkerID, stSEnv.pstMonster->ullFirstAtkerWid);
		if(NULL == pstTeam)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
			
		for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
		{
			pstPlayerTmp = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
			if (NULL == pstPlayerTmp || 0 > mon_booty_valid_player(stSEnv.pstMonster, pstPlayerTmp))
			{
				continue;
			}
			
			apstPlayer[iPlayerNum++] = pstPlayerTmp;
		}
	}
	else if(stSEnv.pstMonster->cFirstAtkerType == MON_BOOTY_OWNER_PLAYER)
	{
		pstPlayerTmp = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstMonster->iFirstAtkerID);
		if (pstPlayerTmp && pstPlayerTmp->stRoleData.RoleID == stSEnv.pstMonster->ullFirstAtkerWid)
		{
			apstPlayer[iPlayerNum++] = pstPlayerTmp;;
		}
	}

	for (i=0; i<iPlayerNum; i++)
	{
		pstTaskRunOne = task_run_find(apstPlayer[i], iTaskID);
		if(pstTaskRunOne && pstTaskRunOne->TaskStat == TASK_STAT_RUN)
		{
			set_script_variable(iRetIdx, 0);
			return 0;
		}
	}

	set_script_variable(iRetIdx, -1);
	return 0;
}

int s_auto_move()
{
	//参数1	返回标志
	//参数2	地图ID
	//参数3	坐标x
	//参数4	坐标y
	//参数5	临时道具ID
	//参数6	方向
	//参数7	状态id
	//参数8	状态等级
	//参数9	状态持续时间

	int iRetIdx = get_script_number(NULL);//返回标志
	int iMapID = get_script_number(NULL);//地图ID
	int iX = get_script_number(NULL);//坐标x
	int iY = get_script_number(NULL);//坐标y
	int iTmpItemID = get_script_number(NULL);//临时道具ID
	int iDir = get_script_number(NULL);//方向
	int iStatus = get_script_number(NULL);//状态id
	int iLevel = get_script_number(NULL);//状态等级
	int iTime = get_script_number(NULL);//状态持续时间


	RESPOS stPos;
	ZONEIDX stIdx;
	ITEMDEF *pstItemDef = NULL;
	DropItem *pstDropItem = NULL;





	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	if (iTmpItemID > 0)
	{
		pstItemDef = z_find_itemdef(stSEnv.pstEnv, iTmpItemID);
		if (NULL == pstItemDef || ITEM_TMP_NO == pstItemDef->ItemTmp.Type)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		stIdx.iID = stSEnv.pstPlayer->stRoleData.Map;
		stIdx.iPos = stSEnv.pstPlayer->stOnline.iMapPos;

		stPos.X = stSEnv.pstPlayer->stRoleData.Pos.X;
		stPos.Y = stSEnv.pstPlayer->stRoleData.Pos.Y;
	
		if(0 == tmp_item_create(stSEnv.pstEnv, stSEnv.pstPlayer,iTmpItemID,&stIdx, &stPos, pstItemDef->ItemTmp.MachineEnerge, 0, 
								0, iDir,0,0,&pstDropItem) )
		{
			if (pstDropItem)
			{
				
				pstDropItem->stObj.stTmpItem.iPatrolID = 0;
				pstDropItem->stObj.stTmpItem.iStatus = iStatus;
				pstDropItem->stObj.stTmpItem.iStatusLevel = iLevel;
				pstDropItem->stObj.stTmpItem.iLast = iTime;
			}

		}
		else 
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		if (!pstDropItem)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		if (0 > tmp_item_add(stSEnv.pstEnv, stSEnv.pstPlayer, pstDropItem, pstItemDef))
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
		else
		{
			z_item_leave_map(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstDropItem);
		}
	}

	stSEnv.pstPlayer->stOnline.stAutoMoveInfo.iMapID = iMapID;
	stSEnv.pstPlayer->stOnline.stAutoMoveInfo.iTmpItemID = iTmpItemID;
	stSEnv.pstPlayer->stOnline.stAutoMoveInfo.iX = iX;
	stSEnv.pstPlayer->stOnline.stAutoMoveInfo.iY = iY;
	stSEnv.pstPlayer->stOnline.stAutoMoveInfo.iStatusID = iStatus;
	

	{
		CSPKG stPkg;
		CSACTIONRES *pstAction = &stPkg.Body.ActionRes;

		memset(pstAction,0,sizeof(*pstAction));

		pstAction->ID = stSEnv.pstPlayer->iMemID;
		pstAction->ActionID = ACTION_CLIENT_CMD;
		
		pstAction->Data.ClientCmd.CmdType = CLIENT_CMD_AUTO_MOVE;
		pstAction->Data.ClientCmd.CmdData.AutoMove.MapID = iMapID;
		pstAction->Data.ClientCmd.CmdData.AutoMove.X = iX;
		pstAction->Data.ClientCmd.CmdData.AutoMove.Y = iY;
		pstAction->Data.ClientCmd.CmdData.AutoMove.Stop = 0;

		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		z_cltmsg_send(stSEnv.pstEnv, stSEnv.pstPlayer, &stPkg);
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_player_clan_pos_info()
{
	//参数1	返回标志
	//参数2	玩家MemID
	//参数3	公会全局ID低位
	//参数4	公会全局ID高位
	//参数5	官职

	int iRetIdx = get_script_number(NULL);//返回标志
	int iPlayerMemid = get_script_number(NULL);//玩家MemID
	int iIDLowIdx = get_script_number(NULL);//公会全局ID低位
	int iIDHighIdx = get_script_number(NULL);//公会全局ID高位
	int iPosIdx = get_script_number(NULL);//官职

	ZoneClan* pstClan = NULL;
	int iPos = 0;
	Player* pstPlayer = stSEnv.pstPlayer;
	int* piClanID = NULL;

	if (iPlayerMemid != 0)
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx,stSEnv.pstEnv, iPlayerMemid);
	}

	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(stSEnv.pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		piClanID = (int*)&pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID;
		set_script_variable(iIDLowIdx, *piClanID);
		piClanID++;
		set_script_variable(iIDHighIdx, *piClanID);
		set_script_variable(iPosIdx, pstPlayer->stRoleData.MiscInfo.ClanInfo.Position);
	}
	else
	{
		pstClan = player_get_clan(stSEnv.pstEnv,pstPlayer);
		if (!pstClan)
		{
			set_script_variable(iIDLowIdx, 0);
			set_script_variable(iIDLowIdx, 0);
			set_script_variable(iPosIdx, 0);
			set_script_variable(iRetIdx, 0);
			return 0;
		}
		
		piClanID = (int*)&pstClan->stClan.ClanProf.GID;
		set_script_variable(iIDLowIdx, *piClanID);
		piClanID++;
		set_script_variable(iIDHighIdx, *piClanID);
		
		iPos = player_clan_position(stSEnv.pstEnv,pstPlayer);
		set_script_variable(iPosIdx, iPos);
	}
	
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_role_ride_on_machine()
{
	// 参数1: 角色的实例ID ，0 是自己
	// 参数2: 返回值>0为乘坐机械ID, =0为未乘坐

	Monster *pstMachine;
	int iMemID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
		z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	if(!pstPlayer)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	if(!is_player_ride_on_machine(pstPlayer))
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	pstMachine = machine_get_by_player(stSEnv.pstEnv, pstPlayer);
	if(pstMachine)
	{
		set_script_variable(iRetIdx, pstMachine->iDefIdx);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_map_trans()
{
	//参数1	返回标志
	int iRetIdx = get_script_number(NULL);//返回标志
	int iRet = 0;

	if (!stSEnv.pstPlayer || !stSEnv.pstNpc)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iRet = player_trans_openlist(stSEnv.pstEnv,stSEnv.pstPlayer,stSEnv.pstNpc);

	set_script_variable(iRetIdx, iRet);
	return 0;
}


int s_enter_world_city_check()
{
	int iRetIdx = get_script_number(NULL);//返回标志

	if(0 > world_city_enter_check(stSEnv.pstEnv))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	set_script_variable(iRetIdx, 0);
	return 0;
}	

int s_is_world_city_owner()
{
	int iRetIdx = get_script_number(NULL);//返回标志

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}
	
	if(!is_world_city_owner(stSEnv.pstEnv, stSEnv.pstPlayer))
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}
	
	set_script_variable(iRetIdx, 1);
	return 0;
}	

int s_set_world_city_open_time()
{
	//参数1	返回标志
	//参数2	开放时间

	int iRetIdx = get_script_number(NULL);//返回标志
	int iTime = get_script_number(NULL);//开放时间

	
	world_city_set_nextweek_open(stSEnv.pstEnv);

	set_script_variable(iRetIdx, 0);
	return 0;
	UNUSED(iTime);
}

int s_get_world_city_enable_time()
{
	//参数1	返回标志
	//参数2	激活时间

	int iRetIdx = get_script_number(NULL);//返回标志
	int iTimeIdx = get_script_number(NULL);//激活时间

	int iTime = stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity.CityCore.FightEnableTime;

	if (stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity.CityCore.FightEnableStat != WORLD_CITY_UNOPEN)
	{
		iTime = 0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iTimeIdx, iTime);

	return 0;
}

int s_get_top_by_level()
{
	int iRetIdx = get_script_number(NULL);		//返回标志
	int iTop = get_script_number(NULL);		//第几名
	int iLevel = get_script_number(NULL);		//等级

	RangeOneMem * pstOne;
	pstOne = range_find_onemem(stSEnv.pstEnv,RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_LEVEL);
	if (!pstOne)
	{
		goto fail;
	}

	if (iTop < 1 || iTop > RANGE_ONE_NUM)
	{
		goto fail;
	}
	
	if (pstOne->stLastRange.iNum < iTop)
	{
		goto fail;	
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iLevel, pstOne->stLastRange.astRangeEntry[iTop-1].stRange.RangeData.Level);	
	return 0;
	

fail:
	set_script_variable(iRetIdx, -1);
	set_script_variable(iLevel, -1);	
	return 0;
}

int s_set_world_city_enable_time()
{
	//参数1	返回标志
	//参数2	激活时间

	int iRetIdx = get_script_number(NULL);//返回标志
	int iTime = get_script_number(NULL);//激活时间

	world_city_set_enabletime(stSEnv.pstEnv);

	set_script_variable(iRetIdx, 0);
	return 0;
	UNUSED(iTime);
}

int s_get_world_city_open_time()
{
	//参数1	返回标志
	//参数2	开放时间

	int iRetIdx = get_script_number(NULL);//返回标志
	int iTimeIdx = get_script_number(NULL);//开放时间

	int iTime = stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity.CityCore.FightEnableTime;

	if (stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity.CityCore.FightEnableStat < WORLD_CITY_NEXT_WEEK_OPEN)
	{
		iTime = 0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iTimeIdx, iTime);
	return 0;
}

int s_set_world_boss_first_kill_time()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.WorldBossFirstKill = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_world_boss_first_kill_time()
{
	//参数1	返回标志
	//参数2	首杀时间

	int iRetIdx = get_script_number(NULL);//返回标志
	int iKillTimeIdx = get_script_number(NULL);//首杀时间

	set_script_variable(iRetIdx, 0);
	set_script_variable(iKillTimeIdx,stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.WorldBossFirstKill);
	return 0;
}

int s_world_city_area()
{
	//参数1	返回标志
	//参数2	区域
	//参数3	类型

	int iRetIdx = get_script_number(NULL);//返回标志
	int iArea = get_script_number(NULL);//区域
	int iType = get_script_number(NULL);//类型

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	if (iType == 0)
	{
		if (iArea == 0)
		{
			stSEnv.pstPlayer->stOnline.iWorldCityProcEventFlag |= WORLD_CITY_PROC_EVENT_IN;
		}
		else
		{
			stSEnv.pstPlayer->stOnline.iWorldCityProcEventFlag |= WORLD_CITY_PROC_EVENT_OUT;
		}
	}
	else
	{
		if (iArea == 0)
		{
			stSEnv.pstPlayer->stOnline.iWorldCityProcEventFlag &= ~WORLD_CITY_PROC_EVENT_IN;
		}
		else
		{
			stSEnv.pstPlayer->stOnline.iWorldCityProcEventFlag &= ~WORLD_CITY_PROC_EVENT_OUT;
		}
	}

	stSEnv.pstPlayer->stOnline.iWorldCityBuildSerial--;

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_add_world_city_build_serial()
{
	//参数1 返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	stSEnv.pstEnv->iWorldCityBuildSerial++;

	set_script_variable(iRetIdx, 0);
	return 0;
}
/*	-lxk
	功能:获取一天中已提交的小龙虾数量
*/
int s_get_crayfish_num_oneday()
{
	int iNum2;
	int iRetIdx = get_script_number(NULL);
	int iNum = get_script_number(NULL);
	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	if(!IsSameDay(stSEnv.pstPlayer->stRoleData.MiscInfo.BeachInfo.CommitCrayfish.Time, stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		stSEnv.pstPlayer->stRoleData.MiscInfo.BeachInfo.CommitCrayfish.Num = 0;
	}
	iNum2 = stSEnv.pstPlayer->stRoleData.MiscInfo.BeachInfo.CommitCrayfish.Num;
	set_script_variable(iRetIdx, 0);
	set_script_variable(iNum, iNum2);

	return 0;
}

/*	-lxk
	功能:提交小龙虾
*/
int s_commit_crayfish_num_once()
{
	int iRetIdx;
	int iNum;
	int exp,exp2;
	int iCurrTime;
	LEVELUP *pstLevel;
	COMMITCRAYFISH *pstCommitCrayfish;
	
	iRetIdx = get_script_number(NULL);
	iNum = get_script_number(NULL);
	exp = get_script_number(NULL);
	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstLevel = z_get_levelup(stSEnv.pstEnv,stSEnv.pstPlayer->stRoleData.Level);

	if (!pstLevel)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstCommitCrayfish = &stSEnv.pstPlayer->stRoleData.MiscInfo.BeachInfo.CommitCrayfish;

	iCurrTime = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;
	if(!IsSameDay(pstCommitCrayfish->Time, iCurrTime))
	{
		//不是同一天数量清零
		pstCommitCrayfish->Num=0;
	}
	
	iNum = min(MAX_CRAYFISH_NUM - pstCommitCrayfish->Num,iNum);

	exp2 = ceil(pstLevel->ContributeExp/1000.0)*iNum;
	pstCommitCrayfish->Num += iNum;
	pstCommitCrayfish->Time = iCurrTime;
	set_script_variable(iRetIdx, iNum);
	set_script_variable(exp, exp2);

	return 0;
}

int s_get_world_city_keep_num()
{
	//参数1	返回标志
	//参数2	防守胜利次数

	int iRetIdx = get_script_number(NULL);//返回标志
	int iKeepNumIdx = get_script_number(NULL);//防守胜利次数

	set_script_variable(iRetIdx, 0);
	set_script_variable(iKeepNumIdx, world_city_fight_keep_num(stSEnv.pstEnv));
	return 0;
}

int s_has_master_world_city()
{
	//参数1	返回标志
	int iRetIdx = get_script_number(NULL);//返回标志

	if(is_has_master_world_city(stSEnv.pstEnv))
	{
		set_script_variable(iRetIdx, 1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_player_join_clan_time()
{
	//参数1	返回标志
	//参数2	时间

	int iRetIdx = get_script_number(NULL);//返回标志
	int iTime = get_script_number(NULL);//时间

	ZoneClan *pstZoneClan = NULL;
	CLANMEM* pstMem = NULL;

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstZoneClan = player_get_clan(stSEnv.pstEnv, stSEnv.pstPlayer);

	if (!pstZoneClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMem = clan_id_mem(pstZoneClan, stSEnv.pstPlayer->stRoleData.RoleID, 
	                     &stSEnv.pstPlayer->stOnline.iClanMemIdx);
	if (!pstMem)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iTime, pstMem->JoinTime);
	return 0;
}

int s_set_world_city_effigy()
{
	//参数1	返回标志
	//参数2	地图ID
	//参数3	坐标X
	//参数4	坐标Y

	int iRetIdx = get_script_number(NULL);//返回标志
	int iMapID = get_script_number(NULL);//地图ID
	int iX = get_script_number(NULL);//坐标X
	int iY = get_script_number(NULL);//坐标Y

	RESPOS stPos;
	stPos.X = iX;
	stPos.Y = iY;

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	
	if (!is_world_city_master(stSEnv.pstEnv,stSEnv.pstPlayer))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	world_city_effigy_set(stSEnv.pstEnv, stSEnv.pstPlayer, iMapID, &stPos);

	set_script_variable(iRetIdx, 0);
	return 0;
}
//使用物品告知物品在哪个位置
int s_use_item_show_map()
{
	//参数1	返回值
	//参数2	地图id
	//参数3	物品id
	//参数4	x坐标
	//参数4	y坐标

	int iRetIdx = get_script_number(NULL);
	int iMapID = get_script_number(NULL);
	int iItemID = get_script_number(NULL);
	int X = get_script_number(NULL);
	int Y = get_script_number(NULL);

	if (NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	stSEnv.stDlgPkg.Body.PlayerMiscSvr.PlayerMiscType = PLAYER_OPEN_ITEM_MAP;
	
		
	stSEnv.stDlgPkg.Body.PlayerMiscSvr.PlayerMiscSvrData.OpenItemMap.MapID = iMapID;
	stSEnv.stDlgPkg.Body.PlayerMiscSvr.PlayerMiscSvrData.OpenItemMap.ItemID = iItemID;
	stSEnv.stDlgPkg.Body.PlayerMiscSvr.PlayerMiscSvrData.OpenItemMap.Pos.X= X;
	stSEnv.stDlgPkg.Body.PlayerMiscSvr.PlayerMiscSvrData.OpenItemMap.Pos.Y = Y;	

	Z_CSHEAD_INIT(&stSEnv.stDlgPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(stSEnv.pstEnv, stSEnv.pstPlayer, &stSEnv.stDlgPkg);
	
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_choose_with_timer()
{
	//参数1	返回标志
	//参数2	yes事件号
	//参数3	no事件号
	//参数4	倒计时(单位秒)
	//参数5	提示内容

	int iRetIdx = get_script_number(NULL);//返回标志
	int iYes = get_script_number(NULL);//yes事件号
	int iNo = get_script_number(NULL);//no事件号
	int iTimer = get_script_number(NULL);//倒计时(单位秒)
	char* pszTip = get_script_string();//提示内容

	set_script_variable(iRetIdx, choose(iYes,iNo,pszTip,iTimer));
	return 0;
}

int s_get_map_rand_mon_pos()
{
	//参数1	返回标志
	//参数2	地图ID
	//参数3	坐标x
	//参数4	坐标y

	int iRetIdx = get_script_number(NULL);//返回标志
	int iMapID = get_script_number(NULL);//地图ID
	int iXIdx = get_script_number(NULL);//坐标x
	int iYIdx = get_script_number(NULL);//坐标y

	MAPMONSTER * pstMapMonDef = get_rand_map_mon_def(stSEnv.pstEnv,iMapID,1);
	if (!pstMapMonDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iXIdx, pstMapMonDef->Pos.X);
	set_script_variable(iYIdx, pstMapMonDef->Pos.Y);

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_target_add_status_dyn()
{
	//参数1	动态参数
	//参数2	目标类型
	//参数3	memid
	//参数4	statusID
	//参数5	等级
	//参数6	时间
	//参数7	返回标志

	int iDyn = get_script_number(NULL);//动态参数
	stSEnv.iExParaType = EX_PARA_TYPE_ADD_STATUS_DYN;
	stSEnv.iExParaData = iDyn;
	
	return s_target_add_status_by_memid();
}

int s_set_lucky_open()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	stSEnv.pstEnv->iMiscFlag |= GAME_FLAG_LUCK;

	lucky_open_ui_action(stSEnv.pstEnv,ACTIVE_GAME_SVR_TYPE_UI_INFO);

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_is_player_lucky_attendee()
{
	//参数1	返回标志
	//参数2	是否参加了

	int iRetIdx = get_script_number(NULL);//返回标志
	int iAttendee = get_script_number(NULL);//是否参加了
	int iIs = 0;

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if(stSEnv.pstPlayer->stOnline.cMiscFlag & MISC_FLAG_LUCK)
	{
		iIs = 1;
	}
	
	set_script_variable(iAttendee, iIs);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_is_lucky_closed()
{
	//参数1	返回标志
	//参数2	是否关闭

	int iRetIdx = get_script_number(NULL);//返回标志
	int iClosed = get_script_number(NULL);//是否关闭

	set_script_variable(iClosed, !(stSEnv.pstEnv->iMiscFlag & GAME_FLAG_LUCK));
	set_script_variable(iRetIdx, 0);
	return 0;
}
/*	-lxk
	功能:公会积分活动副本状态[驱逐入侵]
*/
int s_clan_everyday_pworld_stat_get()
{
	int iRetIdx,iRes;
	ZoneClan *pstZoneClan;
	iRetIdx = get_script_number(NULL);//返回标志
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	pstZoneClan = player_get_clan(stSEnv.pstEnv, stSEnv.pstPlayer);
	if(NULL == pstZoneClan)
	{
		set_script_variable(iRetIdx,-1);
		return 0;
	}
	
	iRes = clan_everyday_pworld_stat_get(stSEnv.pstEnv,pstZoneClan);
	set_script_variable(iRetIdx,iRes);
	return 0;
}

/*	-lxk
	功能:（会长OR副会长）创建公会积分活动副本[驱逐入侵]
*/
int s_clan_everyday_pworld_create()
{
	int iRetIdx,iPosIdx,iRes;
	char *pstPosName;
	ZoneClan *pstZoneClan;
	iRetIdx = get_script_number(NULL);//返回标志
	iPosIdx = get_script_number(NULL);//官职 (表示谁开启的活动 [会长or副会长])
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	pstZoneClan = player_get_clan(stSEnv.pstEnv, stSEnv.pstPlayer);
	if(NULL == pstZoneClan)
	{
		set_script_variable(iRetIdx,-1);
		return 0;
	}
	
	iRes = clan_everyday_pworld_create(stSEnv.pstEnv,pstZoneClan);
	if(0 > iRes)
	{
		set_script_variable(iRetIdx,-1);
		return 0;
	}
	if(iPosIdx == CLAN_POS_VICE_PRESIDENT)
	{
		pstPosName = "副会长";
	}
	else if(iPosIdx == CLAN_POS_PRESIDENT)
	{
		pstPosName = "会长";
	}
	else
	{
		pstPosName = "会员";
	}
	//公会频道消息 - 带自动寻路功能
	//#L斯蒂芬斯@@(1000,25400,39300)#n - 表示地图ID加坐标
	//#L斯蒂芬斯@@[303006,1000]#n - 表示NPC MonsterID加地图ID
	player_clan_msg_broadcast(stSEnv.pstEnv, pstZoneClan, SYS_CLAN, 
		ZONEERR_CLANBROAD267,
		pstPosName,stSEnv.pstPlayer->stRoleData.RoleName);

	z_clan_active_oplog(stSEnv.pstEnv,stSEnv.pstPlayer,CLAN_BANISH_ACTIVE_HELP_ID,1);
	return 0;
}

/*	-lxk
	功能:进入公会积分活动副本[驱逐入侵]
*/
int s_clan_everyday_pworld_enter()
{
	int iRetIdx,iRes;
	iRetIdx = get_script_number(NULL);//返回标志
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	iRes = clan_everyday_pworld_enter(stSEnv.pstEnv,stSEnv.pstPlayer);
	set_script_variable(iRetIdx,iRes);
	return 0;

}

int s_for_each_map_obj()
{
	//参数1	返回标志
	//参数2	地图ID
	//参数3	类型
	//参数4	迭代器1
	//参数5	迭代器2

	int iRetIdx = get_script_number(NULL);//返回标志
	int iMapID = get_script_number(NULL);//地图ID
	int iType = get_script_number(NULL);//类型
	int iIter1 = get_script_number(NULL);//迭代器1
	int iIter2 = get_script_number(NULL);//迭代器2

	int iRet = -1;
	ZONEMAPINST* pstMapInst = NULL;
	ZONEIDX stIdx;
	int i = 0;
	int k= 0;

	if (iIter1 == iIter2)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iRet = get_script_variable(iIter1,&i);
	if (iRet < 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iRet = get_script_variable(iIter2,&k);
	if (iRet < 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iRet = -1;
	
	stIdx.iID = iMapID;
	stIdx.iPos = 0;
	
	pstMapInst = z_find_map_inst(stSEnv.pstEnv->pstAppCtx,stSEnv.pstEnv, &stIdx);

	if (!pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (i < 0 || i > pstMapInst->iAreaDyn)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
		
	for(;i < MAX_MAP_AREA;i++)
	{
		int iIndex	=	pstMapInst->aiAreaDyn[i];
		AREADYN* pstArea;

		if( iIndex<0 )
		{
			continue;
		}

		pstArea	= (AREADYN*)tmempool_get(stSEnv.pstEnv->pstAreaDynPool, iIndex);
		if (NULL == pstArea)
		{
			continue;
		}

		if (k < 0 || k >= pstArea->iAnimate) 
		{
			k = 0;
			continue;
		}
		for(;k < pstArea->iAnimate;k++)
		{
			switch (iType)
			{
			case OBJ_PLAYER:
				{
					Player* pstMapPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstArea->aiAnimate[k]);

					if(pstMapPlayer && PLAYER_STATUS_ROLE_LOGIN == pstMapPlayer->eStatus &&
						!(pstMapPlayer->stOnline.State & CS_STAT_DEAD) )
					{
						stSEnv.pstPlayer = pstMapPlayer;
						if (stSEnv.iStackTop > 0)
						{
							stSEnv.astCallStack[stSEnv.iStackTop - 1].pstPlayer = pstMapPlayer;
						}
						
						iRet = 0;
					}			
				}
				break;
			case OBJ_MONSTER:
				{
					Monster* pstMon = z_id_monster( stSEnv.pstEnv->pstAppCtx,stSEnv.pstEnv,pstArea->aiAnimate[k]);
					if (pstMon && (MONSTER_DIE != pstMon->cStat))
					{
						stSEnv.pstMonster = pstMon;
						if (stSEnv.iStackTop > 0)
						{
							stSEnv.astCallStack[stSEnv.iStackTop - 1].pstMonster = pstMon;
						}
						iRet = 0;
					}
				}
				break;
			case OBJ_NPC:
				{
					Npc* pstNpc = z_id_npc( stSEnv.pstEnv->pstAppCtx,stSEnv.pstEnv,pstArea->aiAnimate[k]);
					if (pstNpc)
					{
						stSEnv.pstNpc = pstNpc;
						if (stSEnv.iStackTop > 0)
						{
							stSEnv.astCallStack[stSEnv.iStackTop - 1].pstNpc = pstNpc;
						}
						iRet = 0;
					}
				}
				break;
			default:
				break;
			}
			
			if (0 == iRet )
			{
				break;
			}
		}

		if (0 == iRet )
		{
			break;
		}
		else
		{
			if (k >= pstArea->iAnimate)
			{
				k = 0;
			}
		}
		
	}

	set_script_variable(iRetIdx, iRet);
	
	if (0 == iRet)
	{
		k++;
		
		set_script_variable(iIter1, i);
		set_script_variable(iIter2, k);
	}
	
	return 0;
}
		
/*	-lxk
	功能:改变角色公会积分活动积分[驱逐入侵]
*/
int s_clan_everyday_pworld_val_chg()
{
	int iRetIdx,iRes,iChg,iCurrJF,iRCurrJF;
	iRetIdx = get_script_number(NULL);//返回标志
	iChg = get_script_number(NULL);
	iCurrJF = get_script_number(NULL);
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	iRes = player_clan_everyday_pworld_val_chg(stSEnv.pstEnv,stSEnv.pstPlayer,iChg);
	iRCurrJF = stSEnv.pstPlayer->stRoleData.MiscInfo.ClanInfo.EveryDayPworldVal;
	set_script_variable(iRetIdx,iRes);
	set_script_variable(iCurrJF,iRCurrJF);
	return 0;
}

/*	-lxk
	功能:角色当前是否在所属公会城市
*/
int s_is_in_my_clan_city()
{
	int iRetIdx,iRes=1;
	iRetIdx = get_script_number(NULL);
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	iRes = is_in_my_clan_city(stSEnv.pstEnv,stSEnv.pstPlayer);
	set_script_variable(iRetIdx,iRes);
	return 0;
}

int s_get_clan_tech_level()
{
	//参数1	返回标志
	//参数2	玩家MemID
	//参数3	科技ID
	//参数4	科技等级

	int iRetIdx = get_script_number(NULL);//返回标志
	int iMemID = get_script_number(NULL);//玩家MemID
	int iTech = get_script_number(NULL);//科技ID
	int iLevelIdx = get_script_number(NULL);//科技等级

	ZoneClan* pstZoneClan = NULL;
	
	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
	               z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
				   
	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if (iTech < 0 || iTech >= MAX_CLAN_TECH_CATEGORY)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}


	
	pstZoneClan = player_get_clan(stSEnv.pstEnv, pstPlayer);
	if (!pstZoneClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
		
	set_script_variable(iLevelIdx, pstZoneClan->stClan.ClanAdvance.Tech.TechLevel[iTech]);

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_is_in_siege_city()
{
	//参数1 返回标志
	//参数2 是否正在进行

	int iRetIdx = get_script_number(NULL);//返回标志
	int iIsIdx = get_script_number(NULL);//是否正在进行

	int iIs = is_in_siege_city_fast(stSEnv.pstEnv);

	set_script_variable(iRetIdx, 0);
	set_script_variable(iIsIdx, iIs);
	return 0;
}

int s_get_clan_stable()
{
	//参数1 返回标志
	//参数2 玩家MemID
	//参数3 安定度

	int iRetIdx = get_script_number(NULL);//返回标志
	int iMemID = get_script_number(NULL);//玩家MemID
	int iValueIdx = get_script_number(NULL);//安定度

	ZoneClan* pstZoneClan = NULL;
	
	Player *pstPlayer = !iMemID ? stSEnv.pstPlayer : 
	               z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
				   
	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstZoneClan = player_get_clan(stSEnv.pstEnv, pstPlayer);
	if (!pstZoneClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iValueIdx, pstZoneClan->stClan.ClanProf.StableValue);
	
	return 0;
}

int s_add_clan_stable()
{
	//参数1 返回标志
	//参数2 玩家MemID
	//参数3 增加的安定度

	int iRetIdx = get_script_number(NULL);//返回标志
	int iMemID = get_script_number(NULL);//玩家MemID
	int iAdd = get_script_number(NULL);//增加的安定度

	ZoneClan* pstZoneClan = NULL;
	int iRet = 0;
	Player *pstPlayer;
	
	if (iMemID > 0)
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);
	}
	else
	{
		pstPlayer = stSEnv.pstPlayer;
	}
				   
	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (iMemID >= 0)
	{
		pstZoneClan = player_get_clan(stSEnv.pstEnv, pstPlayer);
		if (!pstZoneClan)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
	}
	else
	{
		ZONEIDX stIdx;
		ZONEMAPINST* pstMapInst;
		ZONEPWORLDINST *pstPworldInst;

		stIdx.iID = pstPlayer->stRoleData.Map;
		stIdx.iPos = pstPlayer->stOnline.iMapPos;
		pstMapInst = z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stIdx);
		if( !pstMapInst )
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
			
		pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID);
		if (pstPworldInst == NULL)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		pstZoneClan = z_id_clan(stSEnv.pstEnv, pstPworldInst->stPworldTypeData.stClan.iID);
		if (!pstZoneClan)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
		
	}
	
	

	pstZoneClan->stZoneClanInfo.iListVersion++;
	
	iRet = clan_property_adapt(stSEnv.pstEnv, pstZoneClan,
								CLAN_PROPERTY_STABLE_VALUE, iAdd);

	set_script_variable(iRetIdx, iRet);
	return 0;
}


/*	-lxk
	功能:功能：在公会怪物指定的半径范围内给非公会玩家加BUFF
*/
int s_player_add_status_by_clanmon()
{
	Player* pstPlayer = NULL;
	int i;
	STATUSDEF *pstStatusDef;
	
	int iRet = get_script_number(NULL);
	int iRadius = get_script_number(NULL);
	int iStatusID = get_script_number(NULL);
	int iStatusLV = get_script_number(NULL);
	int iStatusTM = get_script_number(NULL);
	
	
	Monster * pstMon = stSEnv.pstMonster;
	if(NULL == pstMon && pstMon->iPKType != MON_PK_TYPE_CLAN)
	{
		goto error;
	}
	pstStatusDef = z_find_status(stSEnv.pstEnv, iStatusID, iStatusLV);
	if (!pstStatusDef)
	{
		goto error;
	}

	for(i = 0;i < stSEnv.pstMonster->iVPlayer;i++)
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx,stSEnv.pstEnv,stSEnv.pstMonster->aiVPlayer[i]);
		if(!pstPlayer || (pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID == pstMon->ullPKID) || (z_distance(&pstMon->stCurrPos,&pstPlayer->stRoleData.Pos) > iRadius))
		{
			continue;
		}
		if (z_player_apply_status(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, 
							iStatusID, iStatusLV, iStatusTM, 1, 0) < 0)
		{
			goto error;
		}
	}
	
	set_script_variable(iRet, 0);
	return 0;
	
error:
	set_script_variable(iRet, -1);
	return 0;
}


int s_get_player_daily_flag()
{
	//参数1 返回标志
	//参数2 标志位
	//参数3 标志值

	int iRetIdx = get_script_number(NULL);//返回标志
	int iPos = get_script_number(NULL);//标志位
	int iSetIdx = get_script_number(NULL);//标志值

	int iSet = 0;
	int iIdx = 0;

	Player *pstPlayer = stSEnv.pstPlayer;

   	iIdx = iPos / 8;
	if (!pstPlayer || iPos < 0 || iIdx >= MAX_DAILY_FLAG_NUM)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	
	iPos %= 8;
	
	if (iIdx < pstPlayer->stRoleData.MiscInfo.DailyFlag.Num && 
		IsSameDay(pstPlayer->stRoleData.MiscInfo.DailyFlag.Time,stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		if (pstPlayer->stRoleData.MiscInfo.DailyFlag.Flags[iIdx] & ( 1 << iPos))
		{
			iSet = 1;
		}
	}

	

	set_script_variable(iRetIdx, 0);
	set_script_variable(iSetIdx, iSet);
	return 0;
}

int s_set_player_daily_flag()
{
	//参数1 返回标志
	//参数2 标志位
	//参数3 标志值

	int iRetIdx = get_script_number(NULL);//返回标志
	int iPos = get_script_number(NULL);//标志位
	int iSet = get_script_number(NULL);//标志值

	
	int iIdx = 0;
	
	Player *pstPlayer = stSEnv.pstPlayer;

	iIdx = iPos / 8;
	if (!pstPlayer || iPos < 0 || iIdx >= MAX_DAILY_FLAG_NUM)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	
	iPos %= 8;

	if (!IsSameDay(pstPlayer->stRoleData.MiscInfo.DailyFlag.Time,stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		memset(pstPlayer->stRoleData.MiscInfo.DailyFlag.Flags,0,
			sizeof(pstPlayer->stRoleData.MiscInfo.DailyFlag.Flags));

		pstPlayer->stRoleData.MiscInfo.DailyFlag.Time = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;
		pstPlayer->stRoleData.MiscInfo.DailyFlag.Num = 0;
	}

	pstPlayer->stRoleData.MiscInfo.DailyFlag.Num = max(iIdx + 1,pstPlayer->stRoleData.MiscInfo.DailyFlag.Num);
	
	if (iSet)
	{
		pstPlayer->stRoleData.MiscInfo.DailyFlag.Flags[iIdx] |= ( 1 << iPos);

		//成长之路
		if(iIdx == 0)
		{
			ACHIEVEMENTCOND stCond;

			memset(&stCond,0,sizeof(stCond));
			
			stCond.Type = COND_CLAN_TOUCH;

			z_player_achievement_cond(stSEnv.pstEnv,pstPlayer,&stCond);
		}	
	}
	else
	{
		pstPlayer->stRoleData.MiscInfo.DailyFlag.Flags[iIdx] &= ~( 1 << iPos);
	}
	

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_clan_msg_info()
{
	//参数1	返回标志
	//参数2	公会ID

	int iRetIdx = get_script_number(NULL);//返回标志
	int iClanID = get_script_number(NULL);//公会ID

	ZoneClan* pstZoneClan = NULL;
	CSPKG *pstPkg = &stSEnv.stDlgPkg;

	if (0 == iClanID)
	{
		if (!stSEnv.pstPlayer)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		iClanID = stSEnv.pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id;
	}
	
	pstZoneClan = z_id_clan(stSEnv.pstEnv, iClanID);
	if (!pstZoneClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);

	player_clan_broadcast(stSEnv.pstEnv,pstZoneClan, pstPkg,0);

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_dyn_active_status_set()
{
	//参数1 返回标志
	//参数2 活动助手ID
	//参数3 状态

	int iRetIdx = get_script_number(NULL);//返回标志
	int iActiveHelperID = get_script_number(NULL);//活动助手ID
	int iStatus = get_script_number(NULL);//状态

	int iRet = -1;


	if (iStatus < ACTIVE_STATUS_INIT || iStatus > ACTIVE_STATUS_END)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	if (CLAN_TOUCH_ACTIVE_HELP_ID == iActiveHelperID)
	{
		//todo:
		ZONEMAPINST* pstMapInst = s_get_mapinst();
		ZONEPWORLDINST* pstPworldInst = NULL;
		ZoneClan* pstZoneClan = NULL;
		
		if (pstMapInst)
		{
			pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj,pstMapInst->iPworldInstID);
		}
		
		if (pstPworldInst && pstPworldInst->iType == PWORLD_TYPE_CLAN)
		{
			pstZoneClan = z_id_clan(stSEnv.pstEnv,  pstPworldInst->stPworldTypeData.stClan.iID);

		}
		if (!pstZoneClan && stSEnv.pstPlayer)
		{
			pstZoneClan = player_get_clan(stSEnv.pstEnv,stSEnv.pstPlayer);
		}
		
		if (pstZoneClan)
		{
			pstZoneClan->stZoneClanInfo.iClanTouchStatus = iStatus;
			
			clan_dny_active_helper_info(stSEnv.pstEnv,pstZoneClan,CLAN_TOUCH_ACTIVE_HELP_ID,iStatus);
			
			iRet = 0;
			goto final;
		}
	}
	
	if (WORLD_CITY_ACTIVE_HELP_ID != iActiveHelperID)
	{
		iRet = 0;
		goto final;
	}
	iRet = dyn_active_status_ch(stSEnv.pstEnv,iActiveHelperID,iStatus);
	
final:
	if (iRet == 0 && 
		(CLAN_TOUCH_ACTIVE_HELP_ID == iActiveHelperID ||
		CLAN_BANISH_ACTIVE_HELP_ID == iActiveHelperID))
	{
		if (stSEnv.pstPlayer)
		{
			z_clan_active_oplog(stSEnv.pstEnv,stSEnv.pstPlayer,iActiveHelperID,iStatus);
		}
		else
		{
			z_clan_active_oplog(stSEnv.pstEnv,NULL,iActiveHelperID,iStatus);
		}
	}
	
	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_clan_touch_add()
{
	//参数1 返回标志
	//参数2 增加值

	int iRetIdx = get_script_number(NULL);//返回标志
	int iAdd = get_script_number(NULL);//增加值

	ZoneClan* pstZoneClan = NULL;
	int iClanID = 0;

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iClanID = stSEnv.pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id;
	
	pstZoneClan = z_id_clan(stSEnv.pstEnv, iClanID);
	if (!pstZoneClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (!IsSameDay(pstZoneClan->stClan.ClanCore.ClanTouchTime,stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstZoneClan->stClan.ClanCore.ClanTouchTime = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;
		pstZoneClan->stClan.ClanCore.ClanTouchNum = 0;
	}

	pstZoneClan->stClan.ClanCore.ClanTouchNum += iAdd;
		
	set_script_variable(iRetIdx, 0);
	return 0;
}


int s_dyn_active_status_get()
{
	//参数1	返回标志
	//参数2	活动助手ID
	//参数3	状态

	int iRetIdx = get_script_number(NULL);//返回标志
	int iActiveHelperID = get_script_number(NULL);//活动助手ID
	int iStatusIdx = get_script_number(NULL);//状态
	int iStatus= 0;
	int iRet = -1;

	if (CLAN_TOUCH_ACTIVE_HELP_ID == iActiveHelperID)
	{
		//todo:
		ZONEMAPINST* pstMapInst = s_get_mapinst();
		ZONEPWORLDINST* pstPworldInst = NULL;
		ZoneClan* pstZoneClan = NULL;
		
		if (pstMapInst)
		{
			pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj,pstMapInst->iPworldInstID);
		}
		
		if (pstPworldInst && pstPworldInst->iType == PWORLD_TYPE_CLAN)
		{
			pstZoneClan = z_id_clan(stSEnv.pstEnv,  pstPworldInst->stPworldTypeData.stClan.iID);

		}

		if (!pstZoneClan && stSEnv.pstPlayer)
		{
			pstZoneClan = player_get_clan(stSEnv.pstEnv,stSEnv.pstPlayer);
		}
		
		if (pstZoneClan)
		{
			iStatus = pstZoneClan->stZoneClanInfo.iClanTouchStatus;	
			iRet = 0;
		}
		
	}
	else
	{

		DYNACTIVESTATUS	*pstActive = z_find_dyn_active_status(stSEnv.pstEnv, iActiveHelperID);
		if (!pstActive)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
	}

	set_script_variable(iRetIdx, iRet);
	set_script_variable(iStatusIdx, iStatus);
	return 0;
}

/*	-lxk
	功能:[彩虹幻境]查看BOSS信息
	参数:
		[1]、返回标识
*/
int s_unreal_watch_boss_info()
{
	int iRetIdx = get_script_number(NULL);	//返回标志
	if (!stSEnv.pstPlayer || !stSEnv.pstNpc)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	unreal_watch_boss_info(stSEnv.pstEnv,stSEnv.pstPlayer, stSEnv.pstNpc->stMon.iID);
	set_script_variable(iRetIdx, 0);
	return 0;
}

/*	-lxk
	功能:[彩虹幻境]查看在彩虹幻境中的相关队伍，便于角色加入队伍
	参数:
		[1]、返回标识
*/
int s_unreal_team_info()
{
	int iRetIdx = get_script_number(NULL);	//返回标志
	if (!stSEnv.pstPlayer || !stSEnv.pstNpc)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	unreal_team_info(stSEnv.pstEnv,stSEnv.pstPlayer, stSEnv.pstNpc->stMon.iID);
	set_script_variable(iRetIdx, 0);
	return 0;
}

/*	-lxk
	功能:[彩虹幻境]传送到队长所在幻境
	参数:
		[1]、返回标识
*/
int s_goto_unreal_captain_near()
{
	int iMap;
	Player *pstCaptain;
	RESPOS stPos;
	MapIndex *pstMapIdx;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	int iRetIdx = get_script_number(NULL);	//返回标志
	if (!stSEnv.pstPlayer)
	{
		goto _error;
	}
	pstCaptain = is_captain_in_unreal(stSEnv.pstEnv,stSEnv.pstPlayer);
	if(pstCaptain == NULL)
	{
		goto _error;
	}

	stIdx.iID = pstCaptain->stRoleData.Map;
	stIdx.iPos = pstCaptain->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		goto _error;
	}
	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		goto _error;
	}

	iMap = pstCaptain->stRoleData.Map;
	z_get_ran_pos(stSEnv.pstEnv, pstMapInst, pstMapIdx, &pstCaptain->stRoleData.Pos, &pstCaptain->stRoleData.Pos, 400, 50,  &stPos);

	if(0 > z_player_change_map2(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, iMap, &stPos, RAND1(180), 0))
	{
		goto _error;
	}

	unreal_exp_layer_chg(stSEnv.pstEnv, stSEnv.pstPlayer, 1);
	set_script_variable(iRetIdx, 0);
	return 0;
	
	_error:
		set_script_variable(iRetIdx, -1);
		return 0;
}

/*	-lxk
	功能:[彩虹幻境]得到角色所在幻境所在层
	参数:
		[1]、返回参数
		[2]、返回角色所在层
*/
int s_get_unreal_layer()
{
	int iCurrLayer = 0;
	int iRet = 0;
	int iRetIdx = get_script_number(NULL);	//返回标志
	int iLayer = get_script_number(NULL);	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	iCurrLayer = get_player_unreal_layer(stSEnv.pstEnv, stSEnv.pstPlayer);
	if(0 > iCurrLayer)
	{
		iRet = -1;
	}
	set_script_variable(iLayer, iCurrLayer);
	set_script_variable(iRetIdx, iRet);
	return 0;
}


/*int s_get_unreal_save()
{
	int iRetIdx = get_script_number(NULL);	//返回标志
	int iSave = get_script_number(NULL);	
	int iRet = -1;

	if (stSEnv.pstPlayer)
	{
		set_script_variable(iSave, stSEnv.pstPlayer->stRoleData.MiscInfo.Unreal.Save);	
		iRet = 0;
	}
	set_script_variable(iRetIdx, iRet);
	return 0;
}*/

/*	-lxk
	功能:[彩虹幻境]保存角色当前所在层
	参数:
		[1]、返回标识
*/
int s_save_unreal_layer()
{
	int iRetIdx = get_script_number(NULL);	//返回标志	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	if(0 > unreal_save(stSEnv.pstEnv, stSEnv.pstPlayer))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	set_script_variable(iRetIdx, 0);
	return 0;
}

/*	-lxk
	功能:[彩虹幻境]队伍根据类型进入下一层
	参数:
		[1]、返回标识
		[2]、设置类型(1:在副本层里;2:从BOSS地图进入下一层)
*/
int s_unreal_enter()
{
	int iRetIdx = get_script_number(NULL);	//返回标志	
	int iType = get_script_number(NULL);
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	if(0 > unreal_enter(stSEnv.pstEnv,stSEnv.pstPlayer,0,iType))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_unreal_get_exp_layer()
{
	int iRetIdx = get_script_number(NULL);	//返回标志	
	int iLayer = get_script_number(NULL);
	int iLy ;

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iLy =  stSEnv.pstPlayer->stRoleData.MiscInfo.Unreal.ExpLayer;
	if (iLy < 0)
		iLy = 0;
	
	set_script_variable(iRetIdx, 0);
	set_script_variable(iLayer, iLy);
	return 0;
}

/*	-lxk
	功能:[彩虹幻境]判断是否是最后一个BOSS地图
	参数:
		[1]、返回标识
		[2]、返回结果(0、不是;1、是)
*/
int s_is_last_unreal_boss_map()
{
	int iRetIdx = get_script_number(NULL);	//返回标志
	int iIsLastMap = get_script_number(NULL);
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		set_script_variable(iIsLastMap, 0);
		return 0;
	}
	if( 0 > unreal_enter_check(stSEnv.pstEnv,stSEnv.pstPlayer,0,UNREAL_ENTER_BOSS))
	{
		set_script_variable(iRetIdx, 0);
		set_script_variable(iIsLastMap, 1);
		return 0;
	}
	set_script_variable(iRetIdx, 0);
	set_script_variable(iIsLastMap, 0);
	return 0;
}

/*	-lxk
	功能:[彩虹幻境]判断进入BOSS地图的条件
	参数:
		[1]、返回标识(0:不满足；1:满足)
*/
int s_unreal_enter_boss_check()
{
	int iRetIdx = get_script_number(NULL);	//返回标志
	ZONEPWORLDINST *pstZonePworldInst;
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	pstZonePworldInst = z_find_pworldinst2(stSEnv.pstEnv, stSEnv.pstPlayer->stRoleData.Map);
	if (NULL == pstZonePworldInst )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (pstZonePworldInst->stPworldIdx.iID != UNREAL_PWORLD_ID)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	if (unreal_enter_next_check_kill(pstZonePworldInst) < 0)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx,stSEnv.pstEnv, stSEnv.pstPlayer,SYS_MIDDLE2, ZONEERR_SCRIPT305, ENTER_NEXT_KILL );
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (unreal_enter_boss_check_level(stSEnv.pstEnv, stSEnv.pstPlayer, pstZonePworldInst) < 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;	
	}
	
	if (unreal_enter_check_team(stSEnv.pstEnv, stSEnv.pstPlayer, 10000, 1) < 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	set_script_variable(iRetIdx, 0);
	return 0;
}

/*	-lxk
	功能:黑暗物质受角色攻击掉绑定银币
	参数:
		[1]、返回标识
		[2]、掉落多少钱
*/
int s_darkmatter_dropmoney()
{
	int iRet,iRnd,iMoneyType;
	int iRetIdx = get_script_number(NULL);	//返回标志
	int iDropMoney = get_script_number(NULL);
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	if (!stSEnv.pstMonster)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	/*[80%机率掉绑银，20%机率掉流通银币]*/
	iRnd = RAND1(100);
	if(iRnd < 20)
	{
		iMoneyType = ATTR_ID_MONEY;
	}
	else
	{
		iMoneyType = ATTR_ID_BULL;
	}
	
	//iMoneyType = ATTR_ID_BULL;
	iRet = booty_money_drop_ex(stSEnv.pstEnv,stSEnv.pstPlayer,stSEnv.pstMonster,
						iMoneyType,iDropMoney, 1, 1, 1);
	set_script_variable(iRetIdx, iRet);
	return 0;
}

/*	-lxk
	功能:黑暗物质逃跑掉落绑定银币
	参数:
		[1]、返回标识
		[2]、掉落多少钱
*/
int s_darkmatter_flee_dropmoney()
{
	int iRet,iRnd,iMoneyType;
	int iRetIdx = get_script_number(NULL);	//返回标志
	int iDropMoney = get_script_number(NULL);
	//int iType = get_script_number(NULL);
	//int iNum = get_script_number(NULL);
	if (!stSEnv.pstMonster)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	//float gl = (8 + RAND1(5))/10.0;
	//iDropMoney = (iDropMoney/iNum)*gl;
	
	/*[80%机率掉绑银，20%机率掉流通银币]*/
	iRnd = RAND1(100);
	if(iRnd < 20)
	{
		iMoneyType = ATTR_ID_MONEY;
	}
	else
	{
		iMoneyType = ATTR_ID_BULL;
	}
	
	//iMoneyType = ATTR_ID_BULL;
	iRet = booty_money_drop_ex(stSEnv.pstEnv,stSEnv.pstPlayer,stSEnv.pstMonster,
						iMoneyType,iDropMoney, 0, 2, 1);
	set_script_variable(iRetIdx, iRet);
	return 0;
}

/*	-lxk
	功能:获取怪物所在地图表定义ID
	参数:
		[1]、返回标识
		[2]、返回的怪物所在地图ID
*/
int s_get_mon_mapdefid()
{
	int iRetMapID;
	int iRetIdx = get_script_number(NULL);	//返回标志
	int iMapDefID = get_script_number(NULL);
	if (!stSEnv.pstMonster)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	iRetMapID = GET_MAPID(stSEnv.pstMonster->stMap.iID);
	set_script_variable(iRetIdx, 0);
	set_script_variable(iMapDefID,iRetMapID);
	return 0;
}

/*	-lxk
	功能:队伍中队员在同一张地图实例中满足一定的条件下的一起传送
	条件:在同一张地图中的队员满足:没有死亡、摆摊、切磋、交易状态
	参数:
		[1]、地图ID
		[2]、地图坐标X
		[3]、地图坐标Y
		[4]、返回标识
*/
int s_team_trans_insamemapinst()
{
	int iMapID;
	RESPOS stPos;
	int iRetIdx;
	int iMapInstID;	//角色所在地图实例ID
	int iGotoMapInstID;//角色要去的地图实例ID
	ZoneTeam *pstTeam;
	int i,iRet;
	Player *pstPlayer = NULL;
	Player *pstOtherPlayer = NULL;

	int iVaildNum = 0;
	Player *apstPlayer[MAX_TEAM_MEMBER];
	iMapID = get_script_number(NULL);
	stPos.X = get_script_number(NULL);
	stPos.Y = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstPlayer = stSEnv.pstPlayer;
	iMapInstID = pstPlayer->stRoleData.Map;	
	if (iMapID >= PWORLD_MAPID_BASE)
	{
		iGotoMapInstID = (pstPlayer->stRoleData.Map & 0xffff0000) | iMapID;
	}
	else
	{
		iGotoMapInstID = iMapID;
	}

	pstTeam = player_team_get(stSEnv.pstEnv, pstPlayer);
	if(pstTeam == NULL)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	for(i = 0; i < pstTeam->stTeamInfo.MemberNum; i++)
	{
		pstOtherPlayer = z_name_player(stSEnv.pstEnv->pstAppCtx, 
			                           stSEnv.pstEnv, 
			                           pstTeam->stTeamInfo.TeamMember[i].MemberName
			                           );
		if(NULL != pstOtherPlayer)
		{
			//队友(包含角色-因角色也要检查自身所处状态)跟角色不在同一个地图实例上，不传送
			if( pstOtherPlayer->stRoleData.Map != iMapInstID)
			{
				continue;
			}
			iRet = team_trans_check_one(stSEnv.pstEnv,pstOtherPlayer);
			if (iRet < 0)
			{
				if(pstOtherPlayer == pstPlayer)
				{
					team_trans_check_tip(stSEnv.pstEnv, pstPlayer,1, iRet, NULL);
				}
				else
				{
					team_trans_check_tip(stSEnv.pstEnv, pstPlayer, 0, iRet,pstOtherPlayer);
				}
				set_script_variable(iRetIdx, -1);
				return 0;
			}
			apstPlayer[iVaildNum++] = pstOtherPlayer;	
		}
	}
	for(i = 0; i < iVaildNum; i++)
	{
		z_player_change_map(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, apstPlayer[i], iGotoMapInstID, &stPos, RAND1(180), 0);
	}
	set_script_variable(iRetIdx, 0);
	return 0;
}

/*	-lxk
	功能:按指定角色最大血量的百分比加减血
	参数:
	[1]、角色ID : 0表示自身（如果在当前脚本上下文能得到角色自身的情况下才有效），>0表示一个指定ID的其他角色
	[2]、HP增减百分比 ( 如:20/-20 表示 加/减 指定角色当前血量最大值的20%的血量)
	[3]、返回标识
*/
int s_add_player_hp2()
{
	PlayerChagAttrVal stAttr;

	stAttr.AttrNum = 1;
	stAttr.AttrList[0].AttrID = ATTR_ID_HP;
	stAttr.AttrList[0].nFlag = 0;
	s_add_player_attr(&stAttr, &stAttr.AttrList[0].Value.HP, &stAttr.AttrList[0].nFlag,1);
	return 0;

}

int s_map_del_tmp_item()
{
	//参数1 返回标志
	//参数2 临时道具DefID

	int iRetIdx = get_script_number(NULL);//返回标志
	int iItemID = get_script_number(NULL);//临时道具DefID

	const ZONEMAPINST *pstMapInst = s_get_mapinst();

	if(!pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
		

	set_script_variable(iRetIdx, map_delete_item(stSEnv.pstEnv, pstMapInst, 0, iItemID));
	return 0;
}



int s_get_mon_level()
{
	int iRetIdx = get_script_number(NULL);	//返回标志
	int iLevel = get_script_number(NULL);	
	
	if (!stSEnv.pstMonster)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iLevel, stSEnv.pstMonster->stMonDynAttr.Level);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_team_follow_cancel()
{
	ZoneTeam *pstTeam;
	Player *pstCap;
	
	if (!stSEnv.pstEnv || !stSEnv.pstPlayer)
	{
		return 0;
	}
	
	pstTeam = player_team_get(stSEnv.pstEnv, stSEnv.pstPlayer);
	if (pstTeam)
	{
		pstCap = player_team_get_captain_player(stSEnv.pstEnv,pstTeam);
		if (pstCap && pstCap != stSEnv.pstPlayer)
		{
			player_team_follow_cancel(stSEnv.pstEnv,pstTeam,stSEnv.pstPlayer,0);
		}
	}
	
	return 0;
}

int s_mon_drop_bootygroup()
{
	int i;
	int iRetIdx;
	int aiBootyGroupID[4];
	ZoneTeam *pstTeam = NULL;
	Player *pstPlayer = NULL;
	Monster *pstMon = stSEnv.pstMonster;
	ADDMONBOOTYGROUP stMonBooty;

	aiBootyGroupID[0] = get_script_number(NULL);
	aiBootyGroupID[1] = get_script_number(NULL);
	aiBootyGroupID[2] = get_script_number(NULL);
	aiBootyGroupID[3] = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);	//返回标志
	
	if (!pstMon)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (0 == pstMon->iFirstAtkerID && 0 == pstMon->szOwner[0])
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (0 == pstMon->szOwner[0] || is_convoy_car(pstMon))
	{
		if(pstMon->cFirstAtkerType == MON_BOOTY_OWNER_PLAYER)
		{
			pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMon->iFirstAtkerID);
		}
		else if(pstMon->cFirstAtkerType == MON_BOOTY_OWNER_TEAM)
		{
			pstTeam = z_idx_team(stSEnv.pstEnv, pstMon->iFirstAtkerID, pstMon->ullFirstAtkerWid);
		}
	}
	else
	{
		pstPlayer = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMon->szOwner);
	}

	if(pstPlayer && is_valid_booty_team(stSEnv.pstEnv, pstPlayer))
	{
		pstTeam = player_team_get(stSEnv.pstEnv, pstPlayer);
	}

	//初始化掉落位置
	rand_position_init();
	
	//掉落包单拿出来
	for (i=0; i<4; i++)
	{
		stMonBooty.GroupID = aiBootyGroupID[i];
		stMonBooty.CollectTime = 0;
		stMonBooty.Flag = 0;
		if(pstTeam )
		{
			if (pstMon->cRonglu)
			{
				mon_booty_group_assign(stSEnv.pstEnv, pstMon,
					&stMonBooty, NULL);	
			}
			else
			{
				mon_booty_group_assign(stSEnv.pstEnv, pstMon,
					&stMonBooty, pstTeam);
			}
		}
		else
		{
			mon_booty_group_assign(stSEnv.pstEnv, pstMon,
					&stMonBooty, NULL);
		}
	}
	
	return 0;
}

int s_get_ronglu_task_num()
{
	int iRetIdx = get_script_number(NULL);//返回标志
	int iGroup = get_script_number(NULL);//返回标志
	int iNum = get_script_number(NULL);//返回标志

	Player *pstPlayer = stSEnv.pstPlayer;
	TASKINFO *pstTaskInfo;
	int i;
	int iCount = 0;

	if (!pstPlayer || iGroup <= 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstTaskInfo = &pstPlayer->stRoleData.MiscInfo.TaskInfo;

	for (i=0; i<pstTaskInfo->GroupOneDay.GroupNum; i++)
	{
		if (pstTaskInfo->GroupOneDay.GroupOneDayInfo[i].GroupID == iGroup)
		{
			iCount = pstTaskInfo->GroupOneDay.GroupOneDayInfo[i].OneDayNum;
			break;
		}
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iNum, iCount);
	return 0;
}

int s_count_down()
{
	//参数1	返回标志
	//参数2	倒计时
	//参数3	标题

	int iRetIdx = get_script_number(NULL);//返回标志
	int iType = get_script_number(NULL);//类型
	int iSec = get_script_number(NULL);//倒计时
	char* pszTitle = get_script_string();//标题

	int i = 0;
	Player* pstPlayer = NULL;

	if (iType > 0)
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,iType);
	}

	
	if (!pstPlayer && 0 < iType)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	
	if (!stSEnv.pstPworldInst && 0 == iType)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (stSEnv.pstPworldInst && 0 == iType)
	{
		for (i=0; i<stSEnv.pstPworldInst->iPlayer; i++)
		{
			pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPworldInst->aiPlayerID[i]);
			if(pstPlayer)
			{
				player_action_count_down(stSEnv.pstEnv,pstPlayer,iSec,pszTitle);
			}
		}
	}

	if (pstPlayer && 0 < iType)
	{
		//pstPlayer = stSEnv.pstPlayer;
		player_action_count_down(stSEnv.pstEnv,pstPlayer,iSec,pszTitle);
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}
int s_is_clan_can_spy()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	ZoneClan* pstClan = NULL;

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	pstClan = player_get_clan(stSEnv.pstEnv,stSEnv.pstPlayer);
	if (!pstClan)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	if (pstClan->stClan.ClanProf.Flag & CLAN_FLAG_SPY_AVAILABLE)
	{
		set_script_variable(iRetIdx, 1);
		return 0;
	}
	else
	{

		set_script_variable(iRetIdx, 0);
		return 0;
	}

	return 0;
}

int s_is_clan_anti_spy_task_open()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	
	ZoneClan* pstClan = NULL;
	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	pstClan = player_get_clan(stSEnv.pstEnv,stSEnv.pstPlayer);
	if (!pstClan)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	if (pstClan->stClan.ClanProf.Flag & CLAN_FLAG_TASK_SPY_ANTI)
	{
		set_script_variable(iRetIdx, 1);
		return 0;
	}
	else
	{

		set_script_variable(iRetIdx, 0);
		return 0;
	}
	
	return 0;
}

int clan_spy_close(ZONESVRENV* pstEnv)
{
	GameEventInst *pstEventSpy = NULL;
	int i = 0;	
	ZoneClan *astZoneClan = pstEnv->pstClanObj->astZoneClan;

	pstEventSpy = z_get_game_event(pstEnv,SPY_EVENT_ID);

	if (!pstEventSpy)
	{		
		return -1;
	}

/*
	if (!(pstEventSpy->iFlag & GAME_EVENT_FLAG_SPY))
	{		
		return -1;
	}
*/
	for(i= 0 ; i < stSEnv.pstEnv->pstClanObj->iClanNum;i++)
	{
		ZONEPWORLDINST* pstZonePworldInst = NULL;
		ZoneClan* pstClan = &astZoneClan[i];
		int iFlag = pstClan->stClan.ClanProf.Flag;
		if (pstClan->stZoneClanInfo.iDirty == CACHE_FLAG_INEXIST)
		{
			continue;
		}


		{
			iFlag &= ~CLAN_FLAG_SPY_AVAILABLE;
			iFlag &= ~CLAN_FLAG_TASK_SPY;
			iFlag &= ~CLAN_FLAG_TASK_SPY_ANTI;
			
			clan_property_modify(stSEnv.pstEnv, pstClan,CLAN_PROPERTY_FLAG, iFlag ,-1);
			clan_active_ui_info_clan_spy(stSEnv.pstEnv, pstClan,ACTIVE_GAME_SVR_TYPE_UI_CLOSE);
		}

		pstZonePworldInst = z_find_clan_city_inst(stSEnv.pstEnv, pstClan);
		if (pstZonePworldInst)
		{
			pstZonePworldInst->iClanSpyFlag = 0;
		}
	}

	pstEventSpy->iFlag &= ~GAME_EVENT_FLAG_SPY;

	for(i = 0; i < MAX_PLAYER; i++)
	{
		TMEMBLOCK* pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(stSEnv.pstEnv->pstPlayerPool, i);
		Player *pstPlayer = NULL;
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (Player *)pstMemBlock->szData;
		if (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
		{
			continue;
		}
		
	}
	
	return 0;
}

int s_clan_spy_close()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志



	set_script_variable(iRetIdx, clan_spy_close(stSEnv.pstEnv));
	return 0;
}

int s_clan_anti_spy_task_open()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志
	
	ZoneClan* pstClan = NULL;
	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstClan = player_get_clan(stSEnv.pstEnv,stSEnv.pstPlayer);
	if (!pstClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (pstClan->stClan.ClanProf.Flag & CLAN_FLAG_TASK_SPY_ANTI)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	else
	{
		pstClan->stClan.ClanProf.Flag |= CLAN_FLAG_TASK_SPY_ANTI;

		clan_property_modify(stSEnv.pstEnv, pstClan,CLAN_PROPERTY_FLAG, pstClan->stClan.ClanProf.Flag ,-1);
		
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	return 0;
}

int s_clan_spy_task_open()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	ZoneClan* pstClan = NULL;
	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstClan = player_get_clan(stSEnv.pstEnv,stSEnv.pstPlayer);
	if (!pstClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (pstClan->stClan.ClanProf.Flag & CLAN_FLAG_TASK_SPY)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	else
	{
		pstClan->stClan.ClanProf.Flag |= CLAN_FLAG_TASK_SPY;

		clan_property_modify(stSEnv.pstEnv, pstClan,CLAN_PROPERTY_FLAG, pstClan->stClan.ClanProf.Flag ,-1);
		
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	return 0;
}

int s_is_in_clan_spy_target_map()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	ZoneClan* pstClan = NULL;
	ZONEPWORLDINST *pstZonePworldInstTarget = NULL;
	ZONEPWORLDINST* pstZonePworldInstSelf = NULL;
	ZONEMAPINST* pstMapInst = NULL;
	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}
	
	pstMapInst = s_get_mapinst();
	if (!pstMapInst)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}	

	pstClan = z_id_clan(stSEnv.pstEnv,stSEnv.pstPlayer->stRoleData.MiscInfo.DailyInfo.SpyInfo.TargetClanID);
	if (!pstClan)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}	

	if (!(pstClan->stClan.ClanProf.Flag & CLAN_FLAG_SPY_AVAILABLE))
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}
	

	pstZonePworldInstTarget = z_find_clan_city_inst(stSEnv.pstEnv, pstClan);
	pstZonePworldInstSelf = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj,pstMapInst->iPworldInstID);	
	if (pstZonePworldInstTarget != pstZonePworldInstSelf)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}
	else
	{
		set_script_variable(iRetIdx, 1);
		return 0;
	}
	
	return 0;
}

int s_is_clan_spy_task_open()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	ZoneClan* pstClan = NULL;
	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	pstClan = player_get_clan(stSEnv.pstEnv,stSEnv.pstPlayer);
	if (!pstClan)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	if (pstClan->stClan.ClanProf.Flag & CLAN_FLAG_TASK_SPY)
	{
		set_script_variable(iRetIdx, 1);
		return 0;
	}
	else
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}
	
	return 0;
}

int s_is_clan_spy_open()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	GameEventInst *pstEventSpy = NULL;

	pstEventSpy = z_get_game_event(stSEnv.pstEnv,SPY_EVENT_ID);

	if (!pstEventSpy)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	if (pstEventSpy->iFlag & GAME_EVENT_FLAG_SPY)
	{
		set_script_variable(iRetIdx, 1);
		return 0;
	}
	else
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}	

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_clan_spy_open()
{
	//参数1	返回标志
	//参数2	排名
	//参数3	需求公会城市数量

	int iRetIdx = get_script_number(NULL);//返回标志
	int iRange = get_script_number(NULL);//排名
	int iCityNum = get_script_number(NULL);//需求公会城市数量

	ZoneObj *pstObj = stSEnv.pstEnv->pstZoneObj;

	int i = 0;
	RangeOneMem *pstRangeClan = NULL;
	int iNum = 0;
	GameEventInst *pstEventSpy = NULL;

	clan_spy_close(stSEnv.pstEnv);

	if (iRange < 1)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstEventSpy = z_get_game_event(stSEnv.pstEnv,SPY_EVENT_ID);

	if (!pstEventSpy)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (pstEventSpy->iFlag & GAME_EVENT_FLAG_SPY)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (pstObj->stRange.iState != RANGE_STAT_DB_FINI)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (stSEnv.pstEnv->pstClanObj->stClanObjInfo.iClanStat != CLAN_STAT_FINI)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	for (i=0; i<pstObj->stRange.iRangeNum; i++)
	{
		RangeOneMem *pstRangeOneMem = &pstObj->stRange.astRange[i];

		if (pstRangeOneMem->iMainType == RANGE_MAIN_CLAN && 
			RANGE_MAIN_CLAN_SUB_DEVELOP == pstRangeOneMem->iSubType)
		{
			pstRangeClan = pstRangeOneMem;
			break;
		}
	}

	if (!pstRangeClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (pstRangeClan->stCurrRange.iNum < iCityNum)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iRange = min(iRange,pstRangeClan->stCurrRange.iNum);
	
	for(i = 0 ; i < pstRangeClan->stCurrRange.iNum; i++)
	{
		 ZoneClan* pstClan = clan_find_by_wid(stSEnv.pstEnv,pstRangeClan->stCurrRange.astRangeEntry[i].stRange.RangeData.GID);
		 if (pstClan && pstClan->stClan.ClanCity.Level > 0)
		 {	
		 	iNum++;
			if (iNum >= iCityNum)
			{
				break;
			}
		 }
	}
	
	if (iNum < iCityNum)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	{
		WORLDCITY *pstWorldCity = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
		if (pstWorldCity->CityCore.ClanGID)
		{
			ZoneClan* pstClan = clan_find_by_wid(stSEnv.pstEnv,pstWorldCity->CityCore.ClanGID);
			ZONEPWORLDINST* pstZonePworldInst = NULL;
			 if (pstClan && pstClan->stClan.ClanCity.Level > 0)
			 {
			 	int iVal = pstClan->stClan.ClanProf.Flag;
				iVal |= CLAN_FLAG_SPY_AVAILABLE;
				
			 	clan_property_modify(stSEnv.pstEnv, pstClan,CLAN_PROPERTY_FLAG, iVal,-1);
				pstZonePworldInst = z_find_clan_city_inst(stSEnv.pstEnv, pstClan);
				if (pstZonePworldInst)
				{
					pstZonePworldInst->iClanSpyFlag = 1;
				}
				iRange--;
			 }
		}
	}

	for(i = 0 ; i < iRange; i++)
	{
		ZONEPWORLDINST* pstZonePworldInst = NULL;
		 ZoneClan* pstClan = clan_find_by_wid(stSEnv.pstEnv,pstRangeClan->stCurrRange.astRangeEntry[i].stRange.RangeData.GID);
		 if (pstClan && pstClan->stClan.ClanCity.Level > 0)
		 {
		 	int iVal = pstClan->stClan.ClanProf.Flag;
			iVal |= CLAN_FLAG_SPY_AVAILABLE;
			
		 	clan_property_modify(stSEnv.pstEnv, pstClan,CLAN_PROPERTY_FLAG, iVal,-1);
			pstZonePworldInst = z_find_clan_city_inst(stSEnv.pstEnv, pstClan);
			if (pstZonePworldInst)
			{
				pstZonePworldInst->iClanSpyFlag = 1;
			}
		 }
	}

	pstEventSpy->iFlag |= GAME_EVENT_FLAG_SPY;
	clan_spy_open_ui_all(stSEnv.pstEnv);


	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_set_call_var()
{
	int var = get_script_number(NULL);
	int value = get_script_number(NULL);

	set_script_call_var(var, value);	
	
	return 0;
}

int s_is_player_in_jail()
{
	//参数1	返回标志
	//参数2	玩家MemID

	int iRetIdx = get_script_number(NULL);//返回标志
	int iPlayerID = get_script_number(NULL);//玩家MemID

	Player* pstPlayer = stSEnv.pstPlayer;

	if (iPlayerID)
	{
		pstPlayer = z_id_player( stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,iPlayerID);
	}

	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	if (player_is_zuolao( pstPlayer))
	{
		set_script_variable(iRetIdx, 1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_spy_target_clan_id()
{
	//参数1	返回标志
	//参数2	公会ID

	int iRetIdx = get_script_number(NULL);//返回标志
	int iClanIDIdx = get_script_number(NULL);//公会ID

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iClanIDIdx,
		stSEnv.pstPlayer->stRoleData.MiscInfo.DailyInfo.SpyInfo.TargetClanID);

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_add_target_clan_stable()
{
	//参数1	返回标志
	//参数2	公会ID
	//参数3	安定度
	//参数4	保护值

	int iRetIdx = get_script_number(NULL);//返回标志
	int iClanID = get_script_number(NULL);//公会ID
	int iStable = get_script_number(NULL);//安定度
	int iProtect = get_script_number(NULL);//保护值
	ZoneClan* pstZoneClan = NULL;
	int iRet = 0;

	pstZoneClan = z_id_clan(stSEnv.pstEnv, iClanID);
	if (!pstZoneClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(iStable < 0)
	{
		int iTmp = abs(iStable);
		if (pstZoneClan->stClan.ClanProf.StableValue <= (unsigned int)iProtect )
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		iTmp = min((unsigned int)iTmp,pstZoneClan->stClan.ClanProf.StableValue);

		iStable = iTmp * -1;
		
	}
	
	

	pstZoneClan->stZoneClanInfo.iListVersion++;
	
	iRet = clan_property_adapt(stSEnv.pstEnv, pstZoneClan,
								CLAN_PROPERTY_STABLE_VALUE, iStable);

	set_script_variable(iRetIdx, iRet);

	return 0;
}

int s_set_player_spy_task_stage_done()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	stSEnv.pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_SPY_FINI;

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_pworld_timer_mon_alive()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	int iRet = -1;
	int iFlag = MON_MISC_FLAG_PWORLD_TIMER;
	
	ZONEMAPINST *pstMapInst;
	ZONEPWORLDINST *pstPworldInst;
	PWORLDROUNDMON *pstRound = NULL;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iRet = map_entity_count_if(stSEnv.pstEnv, pstMapInst, &iFlag, 
	                           AliveFlagMon);

	pstRound = &pstPworldInst->stTimerRound;

	if (pstRound->iCurrRound >= 1 &&
		pstRound->iCurrRound < PWORLD_MON_ROUND_MAX &&
		pstRound->iCurrRound < pstRound->iTotalRound)
	{
		int i = pstRound->iCurrRound;
		
		iRet += pstRound->aRoundMonInfo[i].iTotalNum - 
				pstRound->aRoundMonInfo[i].iCurrNum;
	}

	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_get_pworld_timer()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	ZONEMAPINST *pstMapInst;
	ZONEPWORLDINST *pstPworldInst;
	int iTimer = 0;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iTimer = pstPworldInst->iDownTm - stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;

	set_script_variable(iRetIdx,iTimer);
	return 0;
}

int s_get_pworld_wave_mon_alive()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	int iRet = -1;
	int iFlag = MON_MISC_FLAG_PWORLD_WAVE;
	
	ZONEMAPINST *pstMapInst;
	ZONEPWORLDINST *pstPworldInst;
	PWORLDROUNDMON *pstRound = NULL;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iRet = map_entity_count_if(stSEnv.pstEnv, pstMapInst, &iFlag, 
	                           AliveFlagMon);

	pstRound = &pstPworldInst->stWaveRound;

	if (pstRound->iCurrRound >= 1 &&
		pstRound->iCurrRound < PWORLD_MON_ROUND_MAX &&
		pstRound->iCurrRound < pstRound->iTotalRound)
	{
		int i = pstRound->iCurrRound ;
		
		iRet += pstRound->aRoundMonInfo[i].iTotalNum - 
				pstRound->aRoundMonInfo[i].iCurrNum;
	}

	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_get_pworld_wave_mon_count_down()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	ZONEMAPINST *pstMapInst = NULL;
	ZONEPWORLDINST *pstPworldInst = NULL;
	int iCountDown = 0;
	PWORLDROUNDMON *pstRound = NULL;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstRound = &pstPworldInst->stWaveRound;

	if (pstRound->iCurrRound >= 0 &&
		pstRound->iCurrRound < PWORLD_MON_ROUND_MAX &&
		pstRound->iCurrRound < pstRound->iTotalRound)
	{
		int i = pstRound->iCurrRound;
		
		iCountDown = pstRound->aRoundMonInfo[i].iRefreshTime -
						stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;

		iCountDown = max(iCountDown,0);		
	}	

	set_script_variable(iRetIdx, iCountDown);
	return 0;
}

int s_pworld_push_timer_mon()
{
	//参数1	返回标志
	//参数2	波次定义ID

	int iRetIdx = get_script_number(NULL);//返回标志
	int iWaveMonDefID = get_script_number(NULL);//波次定义ID	

	ZONEMAPINST *pstMapInst = NULL;
	ZONEPWORLDINST *pstPworldInst = NULL;
	PWORLDROUNDMON *pstRound = NULL;
	PWORLDWAVEMONDEF *pstWaveDef = NULL;
	PworldWaveMonDefTail* pstWaveDefTail = NULL;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstWaveDef = z_find_pworld_wave_mon_def(stSEnv.pstEnv, iWaveMonDefID);

	if (!pstWaveDef)
	{
		tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "WaveDefID error :%d", iWaveMonDefID);
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstWaveDefTail = z_get_pworld_wave_mon_tail(pstWaveDef);

	if (!pstWaveDefTail)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstRound = &pstPworldInst->stTimerRound;

	if (pstRound->iTotalRound < PWORLD_MON_ROUND_MAX)
	{
		int i = pstRound->iTotalRound;
		PWORLDROUNDMONINFO *pstRoundInfo = &pstRound->aRoundMonInfo[i];
		
		memset(pstRoundInfo,0,sizeof(*pstRoundInfo));
		
		pstRoundInfo->iWaveDefID = iWaveMonDefID;		
		pstRoundInfo->iCountDown = pstWaveDef->RefreshCountDown;
		pstRoundInfo->iTotalNum = pstWaveDefTail->iTotalMonNum;

		pstRound->iTotalRound++;
	}	

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_pworld_timer_mon_count_down()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	ZONEMAPINST *pstMapInst = NULL;
	ZONEPWORLDINST *pstPworldInst = NULL;
	PWORLDROUNDMON *pstRound = NULL;
	int iCountDown = 0;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstRound = &pstPworldInst->stTimerRound;
	
	if (pstRound->iCurrRound >= 0 &&
		pstRound->iCurrRound < PWORLD_MON_ROUND_MAX &&
		pstRound->iCurrRound < pstRound->iTotalRound)
	{
		int i = pstRound->iCurrRound;
		
		iCountDown = pstRound->aRoundMonInfo[i].iRefreshTime -
						stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;

		iCountDown = max(iCountDown,0);		
	}	

	set_script_variable(iRetIdx, iCountDown);
	return 0;
}

int s_set_pworld_wave_mon_count_down()
{
	//参数1	返回标志
	//参数2	时间

	int iRetIdx = get_script_number(NULL);//返回标志
	int iTimer = get_script_number(NULL);//时间	

	ZONEMAPINST *pstMapInst = NULL;
	ZONEPWORLDINST *pstPworldInst = NULL;
	PWORLDROUNDMON *pstRound = NULL;
	int iCountDown = 0;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	
	pstRound = &pstPworldInst->stWaveRound;
		
	if (pstRound->iCurrRound >= 0 &&
		pstRound->iCurrRound < PWORLD_MON_ROUND_MAX &&
		pstRound->iCurrRound < pstRound->iTotalRound)
	{
		int i = pstRound->iCurrRound;
		
		iCountDown = pstRound->aRoundMonInfo[i].iRefreshTime -
						stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;

		pstRound->aRoundMonInfo[i].iRefreshTime = 
			stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec + iTimer;

		pstRound->aRoundMonInfo[i].iCountDown += iTimer - iCountDown;
		pworld_wave_countdown_action(stSEnv.pstEnv,pstPworldInst);
	}	

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_role_ride_on_machine_mem_id()
{
	//参数1	返回标志
	//参数2	玩家MemID

	int iRetIdx = get_script_number(NULL);//返回标志
	int iMemID = get_script_number(NULL);//玩家MemID
	Monster* pstMachine = NULL;

	Player *pstPlayer = NULL;

	if (0 == iMemID)
	{
		pstPlayer = stSEnv.pstPlayer;
	}
	else
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,iMemID);
	}

	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMachine = machine_get_by_player(stSEnv.pstEnv, pstPlayer);
	if(pstMachine)
	{
		set_script_variable(iRetIdx, pstMachine->iID);
		return 0;
	}	

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_set_pworld_timer()
{
	//参数1	返回标志
	//参数2	时间

	int iRetIdx = get_script_number(NULL);//返回标志
	int iTimer = get_script_number(NULL);//时间	

	ZONEMAPINST *pstMapInst = NULL;
	ZONEPWORLDINST *pstPworldInst = NULL;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst->iDownTm = iTimer + stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_pworld_timer_mon_round()
{
	//参数1	返回标志
	//参数2	波次定义ID

	int iRetIdx = get_script_number(NULL);//返回标志
	int iWaveMonDefIDIdx = get_script_number(NULL);//波次定义ID	

	ZONEMAPINST *pstMapInst = NULL;
	ZONEPWORLDINST *pstPworldInst = NULL;
	PWORLDROUNDMON *pstRound = NULL;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstRound = &pstPworldInst->stTimerRound;
	
	set_script_variable(iRetIdx, pstRound->iCurrRound);
		
	if (pstRound->iCurrRound >= 0 &&
		pstRound->iCurrRound < PWORLD_MON_ROUND_MAX &&
		pstRound->iCurrRound < pstRound->iTotalRound)
	{
		int i = pstRound->iCurrRound;
		
		set_script_variable(iWaveMonDefIDIdx,  pstRound->aRoundMonInfo[i].iWaveDefID);
	}	
	else
	{
		set_script_variable(iWaveMonDefIDIdx, 0);
	}

	return 0;
}

int s_get_pworld_wave_mon_round()
{
	//参数1	返回标志
	//参数2	波次定义ID

	int iRetIdx = get_script_number(NULL);//返回标志
	int iWaveMonDefIDIdx = get_script_number(NULL);//波次定义ID	

	ZONEMAPINST *pstMapInst = NULL;
	ZONEPWORLDINST *pstPworldInst = NULL;
	PWORLDROUNDMON *pstRound = NULL;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, 1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, 1);
		return 0;
	}

	pstRound = &pstPworldInst->stWaveRound;
	
	set_script_variable(iRetIdx, pstRound->iCurrRound);
		
	if (pstRound->iCurrRound >= 0 &&
		pstRound->iCurrRound < PWORLD_MON_ROUND_MAX &&
		pstRound->iCurrRound < pstRound->iTotalRound)
	{
		int i = pstRound->iCurrRound;
		
		set_script_variable(iWaveMonDefIDIdx,pstRound->aRoundMonInfo[i].iWaveDefID);
	}	
	else
	{
		set_script_variable(iWaveMonDefIDIdx, 0);
	}

	return 0;
}

int s_set_pworld_timer_mon_count_down()
{
	//参数1	返回标志
	//参数2	时间

	int iRetIdx = get_script_number(NULL);//返回标志
	int iTimer = get_script_number(NULL);//时间	

	ZONEMAPINST *pstMapInst = NULL;
	ZONEPWORLDINST *pstPworldInst = NULL;
	PWORLDROUNDMON *pstRound = NULL;
	int iCountDown = 0;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstRound = &pstPworldInst->stTimerRound;
		
	if (pstRound->iCurrRound >= 0 &&
		pstRound->iCurrRound < PWORLD_MON_ROUND_MAX &&
		pstRound->iCurrRound < pstRound->iTotalRound)
	{
		int i = pstRound->iCurrRound;
		
		iCountDown = pstRound->aRoundMonInfo[i].iRefreshTime -
						stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;

		pstRound->aRoundMonInfo[i].iRefreshTime = 
			stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec + iTimer;

		pstRound->aRoundMonInfo[i].iCountDown += iTimer - iCountDown;

		if (-1 == iTimer)
		{
			pstRound->aRoundMonInfo[i].iRefreshTime = 
				stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;
			
			pstRound->aRoundMonInfo[i].iCountDown = 0;
			pstRound->iTotalRound = 0;
	
		}
		
		pworld_wave_countdown_action(stSEnv.pstEnv,pstPworldInst);
	}	
	
	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_set_pworld_timer_mon_count_down_wave()
{
	//参数1	返回标志
	//参数2	时间

	int iRetIdx = get_script_number(NULL);//返回标志
	int iTimer = get_script_number(NULL);//时间	

	ZONEMAPINST *pstMapInst = NULL;
	ZONEPWORLDINST *pstPworldInst = NULL;
	PWORLDROUNDMON *pstRound = NULL;
	int iCountDown = 0;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstRound = &pstPworldInst->stWaveRound;
		
	if (pstRound->iCurrRound >= 0 &&
		pstRound->iCurrRound < PWORLD_MON_ROUND_MAX &&
		pstRound->iCurrRound < pstRound->iTotalRound)
	{
		int i = pstRound->iCurrRound;
		
		iCountDown = pstRound->aRoundMonInfo[i].iRefreshTime -
						stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;

		pstRound->aRoundMonInfo[i].iRefreshTime = 
			stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec + iTimer;

		pstRound->aRoundMonInfo[i].iCountDown += iTimer - iCountDown;

		if (-1 == iTimer)
		{
			pstRound->aRoundMonInfo[i].iRefreshTime = 
				stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;
			
			pstRound->aRoundMonInfo[i].iCountDown = 0;
			pstRound->iTotalRound = 0;
	
		}
		
		pworld_wave_countdown_action(stSEnv.pstEnv,pstPworldInst);
	}	
	
	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_pworld_push_wave_mon()
{
	//参数1	返回标志
	//参数2	波次定义ID

	int iRetIdx = get_script_number(NULL);//返回标志
	int iWaveMonDefID = get_script_number(NULL);//波次定义ID	

	ZONEMAPINST *pstMapInst = NULL;
	ZONEPWORLDINST *pstPworldInst = NULL;
	PWORLDROUNDMON *pstRound = NULL;
	PWORLDWAVEMONDEF *pstWaveDef = NULL;
	PworldWaveMonDefTail* pstWaveDefTail = NULL;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstWaveDef = z_find_pworld_wave_mon_def(stSEnv.pstEnv, iWaveMonDefID);

	if (!pstWaveDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstWaveDefTail = z_get_pworld_wave_mon_tail(pstWaveDef);

	if (!pstWaveDefTail)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstRound = &pstPworldInst->stWaveRound;

	if (pstRound->iTotalRound < PWORLD_MON_ROUND_MAX)
	{
		int i = pstRound->iTotalRound;
		PWORLDROUNDMONINFO *pstRoundInfo = &pstRound->aRoundMonInfo[i];
		
		memset(pstRoundInfo,0,sizeof(*pstRoundInfo));
		
		pstRoundInfo->iWaveDefID = iWaveMonDefID;		
		pstRoundInfo->iCountDown = pstWaveDef->RefreshCountDown;
		pstRoundInfo->iTotalNum = pstWaveDefTail->iTotalMonNum;

		pstRound->iTotalRound++;
	}	


	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_clan_machine_pworld_create()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志
	int iShare = get_script_number(NULL);
	ZoneClan* pstZoneClan = NULL;
	int iRet = 0;

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	pstZoneClan = player_get_clan(stSEnv.pstEnv, stSEnv.pstPlayer);
	if(NULL == pstZoneClan)
	{
		set_script_variable(iRetIdx,-1);
		return 0;
	}
	
	iRet = clan_machine_pworld_create(stSEnv.pstEnv,pstZoneClan, iShare);
	if(0 > iRet)
	{
		set_script_variable(iRetIdx,iRet);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_is_pworld_over()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	ZONEMAPINST *pstMapInst;
	ZONEPWORLDINST *pstPworldInst;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, 1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, 1);
		return 0;
	}

	if (pstPworldInst->bOver)
	{
		set_script_variable(iRetIdx, 1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

//基友配对邀请
int s_gay_invite()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	player_gay_invite(stSEnv.pstEnv, stSEnv.pstPlayer);
	set_script_variable(iRetIdx, 0);
	return 0;
}

//是不是有效的基友发起者
int s_is_gay_gong()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(is_gay_gong(stSEnv.pstEnv, stSEnv.pstPlayer))
	{
		set_script_variable(iRetIdx, 1);
	}
	else
	{
		set_script_variable(iRetIdx, 0);
	}
	
	return 0;
}


int s_strong_active_level()
{
	int iRetIdx = get_script_number(NULL);//返回标志

	STRONGPOINTINFO *pstInfo = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	int i;
	int iLevel = 0;
	STRONGPOINTDEF *pstDef;

	for (i=0; i<pstInfo->Num; i++)
	{
		if (pstInfo->StrongPoints[i].IsOpen)
		{
			pstDef = strong_def_find(stSEnv.pstEnv, pstInfo->StrongPoints[i].MapID);
			if (!pstDef)
				continue;

			if (iLevel < pstDef->Lvl)
			{
				iLevel =pstDef->Lvl;	
			}
		}
	}

	set_script_variable(iRetIdx, iLevel);
	return 0;	
}

int s_range_clan_fanrong()
{	
	int iRetIdx = get_script_number(NULL);//返回标志
	int iClanID = get_script_number(NULL);
	ZoneClan *pstClan;

	RangeOneMem *pstOneMem = NULL;
	int i;
	RangeOne *pstCurr;

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);	
		return 0;
	}
	
	pstOneMem = range_find_onemem(stSEnv.pstEnv, RANGE_MAIN_CLAN, RANGE_MAIN_CLAN_SUB_DEVELOP);
	if (!pstOneMem){
		set_script_variable(iRetIdx, -1);
		return 0;}

	if (iClanID == 0)
	{
		pstClan = player_get_clan(stSEnv.pstEnv, stSEnv.pstPlayer);
	}
	else
	{
		pstClan = z_id_clan(stSEnv.pstEnv, iClanID);
	}

	if (!pstClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstCurr = &pstOneMem->stCurrRange;

	for (i=pstCurr->iNum-1; i>=0; i--)
	{
		if ( pstCurr->astRangeEntry[i].stRange.RangeData.GID == pstClan->stClan.ClanProf.GID)
		{
			set_script_variable(iRetIdx, pstCurr->astRangeEntry[i].stRange.TopRange);		
			return 0;
		}
	}
	
	set_script_variable(iRetIdx, -1);	
	return 0;
}

int s_enter_clan_machine_pworld()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	clan_machine_pworld_enter(stSEnv.pstEnv, stSEnv.pstPlayer, stSEnv.pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id);

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_enter_clan_machine_pworld_unite()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志
	ZoneClan *pstZoneClan;
	ZONEPWORLDINST *pstPworldInst;

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstZoneClan = z_id_clan( stSEnv.pstEnv,stSEnv.pstEnv->iShuNiuClanID);
	if(NULL == pstZoneClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_clan_machine_pworld_inst(stSEnv.pstEnv, pstZoneClan);
	if(NULL == pstPworldInst || pstPworldInst->bOver)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (!pstPworldInst->cShareShuNiu)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if (clan_machine_pworld_enter(stSEnv.pstEnv, stSEnv.pstPlayer, stSEnv.pstEnv->iShuNiuClanID) < 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;	
	}
	
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_is_clan_machine_pworld_open()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	ZoneClan *pstZoneClan;
	ZONEPWORLDINST *pstPworldInst;

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}


	pstZoneClan = player_get_clan(stSEnv.pstEnv, stSEnv.pstPlayer);
	if(NULL == pstZoneClan)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	pstPworldInst = z_find_clan_machine_pworld_inst(stSEnv.pstEnv, pstZoneClan);
	if(NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	set_script_variable(iRetIdx, 1);
	return 0;
}

int s_get_machine_pworld_status_info()
{
	//参数1	返回标志
	//参数2	状态ID
	//参数3	状态等级
	//参数4	状态时间
	//参数5	金钱类型
	//参数6	价格
	//参数7	物品id
	//参数8	物品数量

	int iRetIdx = get_script_number(NULL);//返回标志
	int iStatusID = get_script_number(NULL);//状态ID
	int iLevel = get_script_number(NULL);//状态等级
	int iTimeIdx = get_script_number(NULL);//状态时间
	int iCostTypeIdx = get_script_number(NULL);//金钱类型
	int iCostIdx = get_script_number(NULL);//价格
	int iItemIdx = get_script_number(NULL);//物品id
	int iNumIdx = get_script_number(NULL);//物品数量
	int iRemoveItemIdx = get_script_number(NULL);//物品id
	int iRemoveNumIdx = get_script_number(NULL);//物品数量

	PWORLDMACHINEBUFFSHOPDEF* pstDef = NULL;

	pstDef = z_find_pworld_machine_buff_shop_def(stSEnv.pstEnv,iStatusID,iLevel);

	if (!pstDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iTimeIdx, pstDef->Time);
	set_script_variable(iCostTypeIdx, pstDef->CostType);
	set_script_variable(iCostIdx, pstDef->Cost);
	set_script_variable(iItemIdx, pstDef->ItemID);
	set_script_variable(iNumIdx, pstDef->ItemNum);
	set_script_variable(iRemoveItemIdx, pstDef->RemoveItemID);
	set_script_variable(iRemoveNumIdx, pstDef->RemoveItemNum);	
	
	return 0;
}

int s_set_money_type()
{
	//参数1	返回标志
	//参数2	货币类型

	int iRetIdx = get_script_number(NULL);//返回标志
	int iMoneyType = get_script_number(NULL);//货币类型

	stSEnv.iMoneyType = iMoneyType;

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_is_exist_machine_pworld_unite()
{
	int iRetIdx = get_script_number(NULL);//返回标志
	int iWave = get_script_number(NULL);//返回标志
	ZoneClan* pstZoneClan = NULL;
	ZONEPWORLDINST *pstPworldInst;
	int iID = stSEnv.pstEnv->iShuNiuClanID;

	pstZoneClan = z_id_clan(stSEnv.pstEnv, iID);
	if(NULL == pstZoneClan)
	{
		set_script_variable(iRetIdx,0);
		return 0;
	}

	pstPworldInst = z_find_clan_machine_pworld_inst(stSEnv.pstEnv, pstZoneClan);
	if(pstPworldInst && !pstPworldInst->bOver && pstPworldInst->cShareShuNiu)
	{
		set_script_variable(iRetIdx,1);
		set_script_variable(iWave,pstPworldInst->stWaveRound.iCurrRound);
		return 0;
	}

	set_script_variable(iRetIdx,0);
	return 0;
}

int s_get_machine_pworld_status()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志
	int iModel = get_script_number(NULL);

	ZoneClan* pstZoneClan = NULL;
	ZONEPWORLDINST *pstPworldInst;

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	pstZoneClan = player_get_clan(stSEnv.pstEnv, stSEnv.pstPlayer);
	if(NULL == pstZoneClan)
	{
		set_script_variable(iRetIdx,-1);
		return 0;
	}

	pstPworldInst = z_find_clan_machine_pworld_inst(stSEnv.pstEnv, pstZoneClan);
	if(NULL == pstPworldInst)
	{
		if(IsSameDay(stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec,
				pstZoneClan->stClan.ClanCore.MachinePworldLastOpenTime))
		{
			set_script_variable(iRetIdx,2);
			return 0;
		}

		if (IsSameWeek(stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec,
					pstZoneClan->stClan.ClanCore.MachinePworldLastOpenTime) &&
			pstZoneClan->stClan.ClanCore.MachinePworldWeekTimes >= CLAN_MACHINE_PWORLD_WEEK_MAX)
		{
			set_script_variable(iRetIdx,2);
			return 0;
		}
		
		set_script_variable(iRetIdx,0);
		return 0;
	}
	else
	{
		if (pstPworldInst->cShareShuNiu)
		{
			set_script_variable(iModel,1);	
		}
		else
		{
			set_script_variable(iModel,0);
		}
		
		if (pstPworldInst->bOver)
		{
			set_script_variable(iRetIdx,2);
			return 0;
		}
		else
		{
			set_script_variable(iRetIdx,1);
			return 0;
		}
	}

	return 0;
}

int s_cow_pworld_create()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	ZONEIDX stIdx;
	ZONEPWORLD * pstPworld;
	ZONEPWORLDINST*pstZonePworldInst;
	PWORLDSELECT stPworldSelect;
	

	stIdx.iID = COW_PWORLD_ID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(stSEnv.pstEnv->pstMapObj, &stIdx);
	if(NULL == pstPworld)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	pstZonePworldInst = z_in_new_pworld_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPworld, &stPworldSelect,  0, pstPworld->stPworldDef.Timeout*60);
	if (NULL == pstZonePworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstZonePworldInst->iType = PWORLD_TYPE_PLOY;

	if (stSEnv.pstEvent)
	{
		stSEnv.pstEvent->iPworldInstID = pstZonePworldInst->iPworldInstID;
		stSEnv.pstEvent->iPworldWID = pstZonePworldInst->ullPworldWID;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_cow_pworld_staus()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	GameEventInst *pstEvent = NULL;
	ZONEPWORLDINST *pstPworldInst = NULL;	

	pstEvent = z_get_game_event(stSEnv.pstEnv,COW_EVT_ID);

	if (!pstEvent)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstEvent->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	if (pstPworldInst->ullPworldWID != pstEvent->iPworldWID)
	{
		set_script_variable(iRetIdx, 0);
		return 0;		
	}

	if (pstPworldInst->bOver)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}
	
	set_script_variable(iRetIdx, 1);
	return 0;
}

int s_cow_final_range()
{
	//参数1	返回标志
	//参数2	比赛成绩

	int iRetIdx = get_script_number(NULL);//返回标志
	int iSec = get_script_number(NULL);//比赛成绩
	COWFINALRANGE *pstRangeDraft = NULL;
	int i = 0;
	int idx = 0;

	
	COWFINALRANGEMEMBER* pstMember = NULL;
	Player* pstPlayer = NULL;
	int iMinRangeLastRecordID = 0;//最末排名对应的最后流水号
	int iFound = 0;

	if (stSEnv.pstPlayer)
	{
		pstPlayer = stSEnv.pstPlayer;
	}
	else
	{
		
		if (!stSEnv.pstMonster)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		if (stSEnv.pstMonster->szOwner[0] == 0)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		pstPlayer = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstMonster->szOwner);
	}
	
	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstRangeDraft = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.CowFinalRangeDraft;

	if (pstRangeDraft->Num == 0)
	{
		pstRangeDraft->MinRangeSec = 0;
		pstRangeDraft->MinRangeIdx = 0;
	}

	for(i = 0; i < pstRangeDraft->Num; i++)
	{
		pstMember = &pstRangeDraft->Members[i];
		if (pstMember->RoleID == pstPlayer->stRoleData.RoleID)
		{

			idx = i;
			iFound = 1;
			
		}	
	}	

	if (!iFound)
	{
		if (pstRangeDraft->Num < MAX_COW_FINAL_RANGE)
		{
			idx = pstRangeDraft->Num;
			pstRangeDraft->Num ++;
		}
		else
		{
			if (iSec >= pstRangeDraft->MinRangeSec)
			{
				set_script_variable(iRetIdx, 0);
				return 0;
			}

			idx = pstRangeDraft->MinRangeIdx;
		}
	}

	if (idx < 0 ||  idx >= MAX_COW_FINAL_RANGE)
	{
		set_script_variable(iRetIdx, 0);
		return 0;		
	}

	stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.CowFinalRangeLastRecordID++;
	
	pstMember = &pstRangeDraft->Members[idx];

	if (iFound && pstMember->Time < iSec)
	{
		set_script_variable(iRetIdx, 0);
		return 0;	
	}

	pstMember->Career = pstPlayer->stRoleData.Career;
	pstMember->Level = pstPlayer->stRoleData.Level;
	STRNCPY(pstMember->Name,pstPlayer->stRoleData.RoleName,sizeof(pstMember->Name));
	pstMember->RoleID = pstPlayer->stRoleData.RoleID;
	pstMember->Time = iSec;
	pstMember->TakeAward = 0;
	pstMember->RecordID = stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.CowFinalRangeLastRecordID;

	pstRangeDraft->MinRangeSec = 0;
	iMinRangeLastRecordID = 0;
	for(i = 0; i < pstRangeDraft->Num; i++)
	{
		pstMember = &pstRangeDraft->Members[i];
		if (pstMember->Time >= pstRangeDraft->MinRangeSec)
		{

			pstRangeDraft->MinRangeSec = pstMember->Time;
			if (pstMember->RecordID > iMinRangeLastRecordID)
			{
				iMinRangeLastRecordID = pstMember->RecordID;
				pstRangeDraft->MinRangeIdx = i;
			}
			
		}	
	}	

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_cow_final_end()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志
	COWFINALRANGE *pstRangeDraft = NULL;
	COWFINALRANGE *pstRange = NULL;
	int i = 0;

	pstRangeDraft = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.CowFinalRangeDraft;
	pstRange = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.CowFinalRange;

	qsort(pstRangeDraft->Members, pstRangeDraft->Num, 
					sizeof(pstRangeDraft->Members[0]), trl_comp_func);

	memcpy(pstRange,pstRangeDraft,sizeof(*pstRange));

	for(i = 0; i < pstRange->Num; i++)
	{
		pstRange->Members[i].Range = i + 1;
	}

	pstRange->RangeTime = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;
	
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_cow_pworld_enter()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	GameEventInst *pstEvent = NULL;
	ZONEPWORLDINST *pstPworldInst = NULL;	
	ZONEPWORLD * pstPworld;

	pstEvent = z_get_game_event(stSEnv.pstEnv,COW_EVT_ID);

	if (!pstEvent)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstEvent->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (pstPworldInst->ullPworldWID != pstEvent->iPworldWID)
	{
		set_script_variable(iRetIdx, -1);
		return 0;		
	}

	if (pstPworldInst->bOver)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworld = z_find_pworld(stSEnv.pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if(NULL == pstPworld)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(0 == z_player_enter_pworld_by_inst(stSEnv.pstEnv, stSEnv.pstPlayer, pstPworld, pstPworldInst))
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	set_script_variable(iRetIdx, -1);
	return 0;
}

int s_open_cow_final_range_ui()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	player_action_cow_final_range(stSEnv.pstEnv,stSEnv.pstPlayer,0);

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_cow_final_begin()
{
	//参数1	返回标志

	int iRetIdx = get_script_number(NULL);//返回标志

	COWFINALRANGE *pstRange = NULL;

	pstRange = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.CowFinalRangeDraft;
	stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.CowFinalRangeLastRecordID = 0;

	memset(pstRange,0,sizeof(*pstRange));

	set_script_variable(iRetIdx, 0);
	return 0;
}


 //lzk  航海大亨活动开始，清零
 int s_hang_hai_begin()
 {
	 //参数1 返回标志
 
	 int iRetIdx = get_script_number(NULL);//返回标志
 
	 HANGHAIRANGE *pstRange = NULL;
 
	 pstRange = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.HangHaiRangeDraft;
 
	 memset(pstRange,0,sizeof(*pstRange));
 
	 set_script_variable(iRetIdx, 0);
	 return 0;
 }
 
//lzk   结束 
 int s_hang_hai_end()
 {
	 //参数1 返回标志
 
	 int iRetIdx = get_script_number(NULL);//返回标志
	 HANGHAIRANGE *pstRangeDraft = NULL;
	 HANGHAIRANGE *pstRange = NULL;
	 int i = 0;
 
	 pstRangeDraft = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.HangHaiRangeDraft;
	 pstRange = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.HangHaiRange;
 
	 qsort(pstRangeDraft->Members, pstRangeDraft->Num, 
					 sizeof(pstRangeDraft->Members[0]), trl_comp_func);
 
	 memcpy(pstRange,pstRangeDraft,sizeof(*pstRange));
 
	 for(i = 0; i < pstRange->Num; i++)
	 {
		 pstRange->Members[i].Range = i + 1;
	 }
 
	 pstRange->RangeTime = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;
	 
	 set_script_variable(iRetIdx, 0);
	 return 0;
 }
 
 
 //lzk   记录航海时间
 int s_hang_hai_range()
 {
	 //参数1 返回标志
	 //参数2 比赛成绩
 
	 int iRetIdx = get_script_number(NULL);//返回标志
	 int iSec = get_script_number(NULL);//比赛成绩
	 HANGHAIRANGE *pstRangeDraft = NULL;
	 int i = 0;
	 int idx = 0;
	 
	 HANGHAIRANGEMEMBER* pstMember = NULL;
	 Player* pstPlayer = NULL;
	 int iFound = 0;
 
	 if (stSEnv.pstPlayer)
	 {
		 pstPlayer = stSEnv.pstPlayer;
	 }

	 if (!pstPlayer)
	 {
		 set_script_variable(iRetIdx, -1);
		 return 0;
	 }
 
	 pstRangeDraft = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.HangHaiRangeDraft;
 
	 if (pstRangeDraft->Num == 0)
	 {
		 pstRangeDraft->MinRangeSec = 0;
		 pstRangeDraft->MinRangeIdx = 0;
	 }
 
	 for(i = 0; i < pstRangeDraft->Num; i++)
	 {
		 pstMember = &pstRangeDraft->Members[i];
		 if (pstMember->RoleID == pstPlayer->stRoleData.RoleID)
		 {
 
			 idx = i;
			 iFound = 1;
			 
		 }	 
	 }	 
 
	 if (!iFound)
	 {
		 if (pstRangeDraft->Num < MAX_HANG_HAI_RANGE)
		 {
			 idx = pstRangeDraft->Num;
			 pstRangeDraft->Num ++;
		 }
		 else
		 {
			 if (iSec >= pstRangeDraft->MinRangeSec)
			 {
				 set_script_variable(iRetIdx, 0);
				 return 0;
			 }
 
			 idx = pstRangeDraft->MinRangeIdx;
		 }
	 }
 
	 if (idx < 0 ||  idx >= MAX_HANG_HAI_RANGE)
	 {
		 set_script_variable(iRetIdx, 0);
		 return 0;		 
	 }
 	 
	 pstMember = &pstRangeDraft->Members[idx];
 
	 if (iFound && pstMember->UseTime < iSec)
	 {
		 set_script_variable(iRetIdx, 0);
		 return 0;	 
	 }
 
	 pstMember->Career = pstPlayer->stRoleData.Career;
	 pstMember->Level = pstPlayer->stRoleData.Level;
	 STRNCPY(pstMember->PlayerName,pstPlayer->stRoleData.RoleName,sizeof(pstMember->PlayerName));
	 pstMember->RoleID = pstPlayer->stRoleData.RoleID;
	 pstMember->UseTime= iSec;
	 pstMember->TakeAward = 0;
 
	 pstRangeDraft->MinRangeSec = 0;
 	 for(i = 0; i < pstRangeDraft->Num; i++)
	 {
		 pstMember = &pstRangeDraft->Members[i];
		 if (pstMember->UseTime >= pstRangeDraft->MinRangeSec)
		 {
			 pstRangeDraft->MinRangeSec = pstMember->UseTime; 
			 pstRangeDraft->MinRangeIdx = i;
		 }	 
	 }	 
 
	 set_script_variable(iRetIdx, 0);
	 return 0;
 }
 
 
 //lzk   打开航海奖励面板
 int s_open_hang_hai_range_ui()
 {
	 //参数1 返回标志
 
	 int iRetIdx = get_script_number(NULL);//返回标志
 
	 if (!stSEnv.pstPlayer)
	 {
		 set_script_variable(iRetIdx, -1);
		 return 0;
	 }
 
	 player_action_hang_hai_range(stSEnv.pstEnv,stSEnv.pstPlayer,0);
 
	 set_script_variable(iRetIdx, 0);
	 return 0;
 }
 

 //lzk   打开养海蚌面板
 int s_open_yanghaibang_ui()
 {
 	//参数1 返回标志
 
	 int iRetIdx = get_script_number(NULL);//返回标志
 
	 if (!stSEnv.pstPlayer)
	 {
		 set_script_variable(iRetIdx, -1);
		 return 0;
	 }
	 
	player_yanghaibang_open_ui(stSEnv.pstEnv,stSEnv.pstPlayer);
	
	set_script_variable(iRetIdx, 0);
	return 0;
 }


//拼酒活动玩家输了，通知客户端进行屏幕模糊
int s_pinjiu_screen_vague()
 {
 	//参数1 返回标志
 
	 int iRetIdx = get_script_number(NULL);//返回标志
 
	 if (!stSEnv.pstPlayer)
	 {
		 set_script_variable(iRetIdx, -1);
		 return 0;
	 }
	 
	player_pinjiu_screen_vague(stSEnv.pstEnv,stSEnv.pstPlayer);
	
	set_script_variable(iRetIdx, 0);
	return 0;
 }


//lzk    报废 ，没有用到
int s_get_pingjiu_var()
{
	//参数1  获取的值
	 //参数2 返回标志

	 int iVar = get_script_number(NULL);//获取的值
	 int iRetIdx = get_script_number(NULL);//返回标志

 	 int iGetVar = 0; 
	 iGetVar = stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.PingJiuVar;
	 
	 set_script_variable(iVar, iGetVar);
	 set_script_variable(iRetIdx, 0);
	 return 0;
}

//lzk   报废。没有用到
 int s_set_pingjiu_var()
 {
	 //参数1  设置值
	 //参数2 返回标志

	 int iVar = get_script_number(NULL);//设置的值
	 int iRetIdx = get_script_number(NULL);//返回标志
 
	 stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.PingJiuVar = iVar;
 
	 set_script_variable(iRetIdx, 0);
	 return 0;
 
 }


 int s_new_map_life_npc()
{
	int iNpcID;
	int iMap;
   	int iX;
   	int iY;
   	int iDir;
   	int iLifeTime;
  	char *pszScript;
   	int iRetIdx;
   	MAPNPC stMapNpc;
	ZONEIDX stIdx;
   	ZONEMAPINST *pstMapInst;
   	MapIndex *pstMapIndex;
	int iRet;
	Npc *pstNpc;
		
	iRetIdx = get_script_number(NULL);
	iMap = get_script_number(NULL);
	iNpcID = get_script_number(NULL);
	iX = get_script_number(NULL);
	iY = get_script_number(NULL);
	iDir = get_script_number(NULL);
	iLifeTime = get_script_number(NULL);
	pszScript = get_script_string();	

	stIdx.iID = iMap;
	stIdx.iPos = -1;

	pstMapInst = z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stIdx);
	if (!pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapIndex = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIndex)
	{
	        set_script_variable(iRetIdx, -1);
	        return 0;
	}

	memset(&stMapNpc, 0, sizeof(stMapNpc));
	stMapNpc.MonsterID = iNpcID;
	stMapNpc.Pos.X = iX;
	stMapNpc.Pos.Y = iY;
	stMapNpc.Dir = iDir;
	STRNCPY(stMapNpc.Script, pszScript, sizeof(stMapNpc.Script));

	iRet = z_in_new_pworld_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMapIndex, pstMapInst, &stMapNpc,0);
	if (0 > iRet)
	{
	        set_script_variable(iRetIdx, -1);
	        return 0;
	}

	pstNpc = z_id_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iRet);
	if(pstNpc)
	{
		newnpc_add(stSEnv.pstEnv, pstMapInst, pstNpc);
		
		if (iLifeTime > 0)
		{
			pstNpc->stMon.tLifeEnd = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec + iLifeTime;
		}
	}

	set_script_variable(iRetIdx, iRet);

	return 0;
}


int s_new_map_life_npc1()
{
	int iNpcID;
	int iMap;
   	int iX;
   	int iY;
   	int iDir;
   	int iLifeTime;
  	char *pszScript;
   	int iRetIdx;
   	MAPNPC stMapNpc;
	ZONEIDX stIdx;
   	ZONEMAPINST *pstMapInst;
   	MapIndex *pstMapIndex;
	int iRet;
	Npc *pstNpc;
	int iPatrolID;
		
	iRetIdx = get_script_number(NULL);
	iMap = get_script_number(NULL);
	iNpcID = get_script_number(NULL);
	iX = get_script_number(NULL);
	iY = get_script_number(NULL);
	iDir = get_script_number(NULL);
	iLifeTime = get_script_number(NULL);
	iPatrolID = get_script_number(NULL);
	pszScript = get_script_string();	

	stIdx.iID = iMap;
	stIdx.iPos = -1;

	pstMapInst = z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stIdx);
	if (!pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapIndex = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIndex)
	{
	        set_script_variable(iRetIdx, -1);
	        return 0;
	}

	memset(&stMapNpc, 0, sizeof(stMapNpc));
	stMapNpc.MonsterID = iNpcID;
	stMapNpc.Pos.X = iX;
	stMapNpc.Pos.Y = iY;
	stMapNpc.Dir = iDir;
	stMapNpc.PatrolID = iPatrolID;
	STRNCPY(stMapNpc.Script, pszScript, sizeof(stMapNpc.Script));

	iRet = z_in_new_pworld_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMapIndex, pstMapInst, &stMapNpc,0);
	if (0 > iRet)
	{
	        set_script_variable(iRetIdx, -1);
	        return 0;
	}

	pstNpc = z_id_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iRet);
	if(pstNpc)
	{
		newnpc_add(stSEnv.pstEnv, pstMapInst, pstNpc);
		
		if (iLifeTime > 0)
		{
			pstNpc->stMon.tLifeEnd = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec + iLifeTime;
		}
	}

	set_script_variable(iRetIdx, iRet);

	return 0;
}

  int s_new_map_life_npc_ex()
{
	int iNpcID;
	int iMap;
   	int iX;
   	int iY;
   	int iDir;
   	int iLifeTime;
  	char *pszScript;
   	int iRetIdx;
   	MAPNPC stMapNpc;
	ZONEIDX stIdx;
   	ZONEMAPINST *pstMapInst;
   	MapIndex *pstMapIndex;
	int iRet;
	Npc *pstNpc;
	int iSellListID;
		
	iRetIdx = get_script_number(NULL);
	iMap = get_script_number(NULL);
	iNpcID = get_script_number(NULL);
	iX = get_script_number(NULL);
	iY = get_script_number(NULL);
	iDir = get_script_number(NULL);
	iLifeTime = get_script_number(NULL);
	iSellListID = get_script_number(NULL);
	pszScript = get_script_string();	

	stIdx.iID = iMap;
	stIdx.iPos = -1;

	pstMapInst = z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stIdx);
	if (!pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapIndex = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIndex)
	{
	        set_script_variable(iRetIdx, -1);
	        return 0;
	}

	memset(&stMapNpc, 0, sizeof(stMapNpc));
	stMapNpc.MonsterID = iNpcID;
	stMapNpc.Pos.X = iX;
	stMapNpc.Pos.Y = iY;
	stMapNpc.Dir = iDir;
	stMapNpc.SellListID = iSellListID;
	STRNCPY(stMapNpc.Script, pszScript, sizeof(stMapNpc.Script));

	iRet = z_in_new_pworld_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstMapIndex, pstMapInst, &stMapNpc,0);
	if (0 > iRet)
	{
	        set_script_variable(iRetIdx, -1);
	        return 0;
	}

	pstNpc = z_id_npc(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iRet);
	if(pstNpc)
	{
		newnpc_add(stSEnv.pstEnv, pstMapInst, pstNpc);
		
		if (iLifeTime > 0)
		{
			pstNpc->stMon.tLifeEnd = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec + iLifeTime;
		}
	}

	set_script_variable(iRetIdx, iRet);
	return 0;
}

 int s_get_vaild_item_num_inpak()
{
	int iItemID;
	int iRetIdx;
	int iNumIdx;
	int iNum;

	iItemID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iNumIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	g_invaild_dec = 1;
	iNum = z_get_pak_item_num(stSEnv.pstEnv, &stSEnv.pstPlayer->stRoleData.Package, iItemID, -1,0,PACKAGE_DEC_NONE);
	g_invaild_dec = 0;
	
	set_script_variable(iRetIdx, 0);
	set_script_variable(iNumIdx, iNum);

	return 0;
}

int s_get_strong_sale_info()
{
	int iRetIdx;
	int iMapID;
	
	iRetIdx = get_script_number(NULL);
	iMapID = get_script_number(NULL);

	if (!stSEnv.pstPlayer || !stSEnv.pstNpc || !stSEnv.pstPlayer || iMapID <= 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	strong_svr_sale_info(stSEnv.pstEnv,stSEnv.pstPlayer, iMapID, stSEnv.pstNpc->stMon.iID);
	return 0;
}

int s_strong_player_award()
{
	int iRetIdx;
	int iMapID;
	int iType;
	
	iRetIdx = get_script_number(NULL);
	iMapID = get_script_number(NULL);
	iType = get_script_number(NULL);


	if (!stSEnv.pstPlayer || !stSEnv.pstNpc  || iMapID <= 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	strong_player_award(stSEnv.pstEnv,stSEnv.pstPlayer, iType,  iMapID);
	return 0;
}

int s_strong_add_res()
{
	int iRetIdx;
	int iMapID;

	iRetIdx = get_script_number(NULL);
	iMapID = get_script_number(NULL);

	if (iMapID <= 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	strong_pworld_res_add(stSEnv.pstEnv, iMapID);
	return 0;
}

int s_strong_is_open()
{
	int iRetIdx;
	int iMapID;	
	int iOpen;
	//STRONGPOINTONE *pstOne;

	iRetIdx = get_script_number(NULL);
	iMapID = get_script_number(NULL);
	iOpen = get_script_number(NULL);

	if (iMapID <= 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iOpen, strong_is_open(stSEnv.pstEnv,iMapID));
	return 0;
}


// 公会是否开启了据点荣耀
// 
int s_clan_has_strong_glory()
{
	int iRetIdx;
	int iMapID;	
	Player *pstPlayer = stSEnv.pstPlayer;

	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
	}

	iRetIdx = get_script_number(NULL);
	iMapID = get_script_number(NULL);

	set_script_variable(iRetIdx, 0);
	set_script_variable(iMapID, strong_glory_clan_is_open(stSEnv.pstEnv, pstPlayer));
	return 0;
}

// 判断某个玩家是不是某个据点的成员
int s_player_is_strong_member()
{
	int iRetIdx;
	int iMapID;	
	int iYes;
	Player *pstPlayer = stSEnv.pstPlayer;

	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
	}

	iRetIdx = get_script_number(NULL);
	iMapID = get_script_number(NULL);
	iYes = get_script_number(NULL);

	set_script_variable(iRetIdx, 0);
	set_script_variable(iYes, strong_player_is_owner(stSEnv.pstEnv, pstPlayer, iMapID));
	return 0;
}


// 开启战旗荣耀
int s_strong_open_glory()
{
	int iRetIdx;
	int iMapID;	
	int iRet;

	Player *pstPlayer = stSEnv.pstPlayer;

	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	iRetIdx = get_script_number(NULL);
	iMapID = get_script_number(NULL);

	iRet = strong_glory_open(stSEnv.pstEnv,pstPlayer, iMapID);

	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_clan_open_dart()
{
	int iRetIdx;
	int iRet;
	Player *pstPlayer = stSEnv.pstPlayer;

	if (!pstPlayer || !stSEnv.pstEnv)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	iRet = clan_open_dart(stSEnv.pstEnv, pstPlayer);

	set_script_variable(iRetIdx, iRet);
	return 0;
}

int s_strong_is_mine()
{
	int iRetIdx;
	int iMapID;	
	int iMy;
	STRONGPOINTONE *pstOne;
	ZoneClan *pstClan;

	iRetIdx = get_script_number(NULL);
	iMapID = get_script_number(NULL);
	iMy = get_script_number(NULL);

	if (iMapID <= 0 || !stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		set_script_variable(iMy, 0);
		return 0;
	}

	pstClan = player_get_clan(stSEnv.pstEnv, stSEnv.pstPlayer);
	if (!pstClan)
	{
		set_script_variable(iRetIdx, -1);
		set_script_variable(iMy, 0);
		return 0;
	}

	pstOne = strong_point_one_get(stSEnv.pstEnv, iMapID);
	if (!pstOne || pstClan->stClan.ClanProf.Id != pstOne->OwerID)
	{
		set_script_variable(iRetIdx, -1);
		set_script_variable(iMy, 0);
	}
	else
	{
		set_script_variable(iRetIdx, 0);
		set_script_variable(iMy, 1);
	}
	return 0;
}


// 放弃据点
int s_strong_cancel()
{
	int iRetIdx;
	int iMapID;	

	iRetIdx = get_script_number(NULL);
	iMapID = get_script_number(NULL);

	if (iMapID<=0 ||!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	strong_cancel(stSEnv.pstEnv,stSEnv.pstPlayer, iMapID);
	return 0;
}

int s_strong_enter_pworld()
{
	int iRetIdx;
	int iMapID;
	STRONGENTER stEnter;

	iRetIdx = get_script_number(NULL);
	iMapID = get_script_number(NULL);

	if (iMapID <= 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (!stSEnv.pstEnv || !stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	stEnter.MapID = iMapID;
	strong_enter_pworld(stSEnv.pstEnv,stSEnv.pstPlayer,  &stEnter);
	return 0;
}

int s_dec_vaild_item_num_inpak()
{
	int iItemID;
	int iNum;
	int iRetIdx;
	int iRet = -1;
	ITEMDEC stDec;

	iItemID = get_script_number(NULL);
	iNum = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	g_invaild_dec = 1;
	if (iNum > z_get_pak_item_num(stSEnv.pstEnv, &stSEnv.pstPlayer->stRoleData.Package, iItemID, -1,0,PACKAGE_DEC_NONE))
	{
		g_invaild_dec = 0;
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	stDec.ID = iItemID;
	stDec.Num = iNum;
	stDec.BindNum = 0;

	g_invaild_dec = 1;
	if(stSEnv.pstRoleItem != NULL && stSEnv.pstRoleItem->DefID == iItemID)
	{
		// 严格判断下,如果是藏宝图,要扣指定格子的物品
		ITEMDEF *pstItemDef;
		ROLEITEM *pstRoleItem;
		int iPos;
		if (0 > package_get_item(stSEnv.pstEnv, stSEnv.pstPlayer, stSEnv.stCsPackageUse.SrcListType, 
							stSEnv.stCsPackageUse.SrcGridIdx, &pstItemDef, &pstRoleItem, &iPos))
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
		iRet = package_dec(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, iItemID, iNum, 
			stSEnv.stCsPackageUse.SrcListType, &iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_SCRIPT);
		if (iRet > 0)
			stDec.BindNum = iRet;
	}
	else 
	{
		iRet = package_dec(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, iItemID, iNum, -1,
				NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_SCRIPT);
		if (iRet > 0)
			stDec.BindNum = iRet;
	}
	g_invaild_dec = 0;

	if (iRet >= 0)
	{
		z_role_item_dec_oplog(stSEnv.pstEnv,stSEnv.pstPlayer, &stDec,OP_ITEM_DEC_SCRIPT);
	}

	set_script_variable(iRetIdx, 0);

	return 0;
}

int s_add_id_player_contri_test()
{
	//参数1	返回标志
	//参数2	玩家MemID
	//参数3	贡献度
	//参数4	类型

	int iRetIdx = get_script_number(NULL);//返回标志
	int iPlayerID = get_script_number(NULL);//玩家MemID
	int iContri = get_script_number(NULL);//贡献度
	int iType = get_script_number(NULL);//类型

	int iRet = 0;

	Player* pstPlayer = NULL;
	ZoneClan* pstClan = NULL;

	if (iPlayerID)
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx,stSEnv.pstEnv,iPlayerID);
	}
	else
	{
		pstPlayer = stSEnv.pstPlayer;
	}
	
	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstClan = player_get_clan(stSEnv.pstEnv,pstPlayer);

	if (!pstClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;		
	}
	
	if (!iContri)
	{
		set_script_variable(iRetIdx, 0);
		return 0;		
	}
	
	if (iContri > 0)
	{
		if ((unsigned long long)pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute >= (unsigned)MAX_CLAN_OWNED_CONTRIBUT)
		{
			iRet = -1;
		}
		
		if (iType == 1 &&
			(unsigned long long)pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute + iContri > (unsigned)MAX_CLAN_OWNED_CONTRIBUT)
		{
			iRet = -1;
		}
	}
	else
	{		
		iRet = player_contribute_dec(stSEnv.pstEnv,pstPlayer, (unsigned int) -iContri,1);
	}	

	set_script_variable(iRetIdx, iRet);
	return 0;
}
int s_add_id_clan_money()
{
	//参数1	返回标志
	//参数2	公会ID
	//参数3	公会资金
	//参数4	类型
	//参数5	实际增加资金

	int iRetIdx = get_script_number(NULL);//返回标志
	int iClanID = get_script_number(NULL);//公会ID
	int iMoney = get_script_number(NULL);//公会资金
	int iType = get_script_number(NULL);//类型
	int iMoneyIdx = get_script_number(NULL);//实际增加资金

	int iRet = 0;

	ZoneClan* pstClan = NULL;
	
	if (iClanID)
	{
		pstClan = z_id_clan(stSEnv.pstEnv,(unsigned int)iClanID);
	}
	else
	{
		if (stSEnv.pstPlayer)
		{
			pstClan = player_get_clan(stSEnv.pstEnv,stSEnv.pstPlayer);
		}
	}

	if (!pstClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;		
	}

	if (iMoney > 0)
	{
		iRet = clan_property_modify(stSEnv.pstEnv, pstClan, CLAN_PROPERTY_MONEY, 
			                     iMoney, 0);
	}
	else
	{
		iRet = clan_property_modify(stSEnv.pstEnv, pstClan, CLAN_PROPERTY_MONEY, 
			                     -iMoney, 1);
	}
	
	if ( iRet < 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;		
	}

	
	set_script_variable(iRetIdx, 0);
	set_script_variable(iMoneyIdx, iMoney);
	return 0;
	UNUSED(iType);
}
int s_add_id_clan_money_test()
{
	//参数1	返回标志
	//参数2	公会ID
	//参数3	公会资金
	//参数4	类型

	int iRetIdx = get_script_number(NULL);//返回标志
	int iClanID = get_script_number(NULL);//公会ID
	int iMoney = get_script_number(NULL);//公会资金
	int iType = get_script_number(NULL);//类型

	int iRet = 0;

	ZoneClan* pstClan = NULL;
	
	if (iClanID)
	{
		pstClan = z_id_clan(stSEnv.pstEnv,(unsigned int)iClanID);
	}
	else
	{
		if (stSEnv.pstPlayer)
		{
			pstClan = player_get_clan(stSEnv.pstEnv,stSEnv.pstPlayer);
		}
	}

	if (!pstClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;		
	}

	if (iMoney > 0)
	{
		iRet = clan_property_modify_test(stSEnv.pstEnv, pstClan, CLAN_PROPERTY_MONEY, 
			                     iMoney, 0);
	}
	else
	{
		iRet = clan_property_modify_test(stSEnv.pstEnv, pstClan, CLAN_PROPERTY_MONEY, 
			                     -iMoney, 1);
	}
	
	if ( iRet < 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;		
	}

	set_script_variable(iRetIdx, 0);
	return 0;
	UNUSED(iType);
}

int s_get_id_player_contri()
{
	//参数1	返回标志
	//参数2	玩家MemID
	//参数3	贡献度

	int iRetIdx = get_script_number(NULL);//返回标志
	int iPlayerID = get_script_number(NULL);//玩家MemID
	int iContriIdx = get_script_number(NULL);//贡献度

	Player* pstPlayer = NULL;
	ZoneClan* pstClan = NULL;

	if (iPlayerID)
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx,stSEnv.pstEnv,iPlayerID);
	}
	else
	{
		pstPlayer = stSEnv.pstPlayer;
	}
	
	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstClan = player_get_clan(stSEnv.pstEnv,pstPlayer);

	if (!pstClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;		
	}	

	set_script_variable(iContriIdx,pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_record_id_player_clan_donate()
{
	//参数1	返回标志
	//参数2	玩家MemID
	//参数3	捐献数量
	//参数4	公会获得资金
	//参数5	个人获得贡献度

	int iRetIdx = get_script_number(NULL);//返回标志
	int iPlayerID = get_script_number(NULL);//玩家MemID
	int iDonateMoney = get_script_number(NULL);//捐献数量
	int iClanMoney = get_script_number(NULL);//公会获得资金
	int iContri = get_script_number(NULL);//个人获得贡献度

	//int iIdx = 0;
	//CLANDONATELOGINFO* pstLog;

	Player* pstPlayer = NULL;
	ZoneClan* pstClan = NULL;
	CLANMEM *pstMem = NULL;

	if (iPlayerID)
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx,stSEnv.pstEnv,iPlayerID);
	}
	else
	{
		pstPlayer = stSEnv.pstPlayer;
	}
	
	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstClan = player_get_clan(stSEnv.pstEnv,pstPlayer);

	if (!pstClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;		
	}

	pstMem = clan_id_mem(pstClan,
			                              pstPlayer->stRoleData.RoleID, NULL);
	if (!pstMem)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMem->DonateLastTime = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;
	pstMem->DonateClanMoney += iClanMoney;
	pstMem->DonateContri += iContri;
	pstMem->DonateNum += iDonateMoney;
	
	player_clan_mem_chg_broadcast(stSEnv.pstEnv, pstClan, pstMem,
								  CLAN_MEMBER_CHG_MOD);

	
/*
	pstLog = &pstClan->stClan.ClanAdvance.ClanDonateLogInfo;

	if (pstLog->Num < MAX_CLAN_DONATE)
	{
		iIdx = pstLog->Num;
	}
	else
	{
		iIdx = pstLog->BeginPos;
	}
	
	if (iIdx < 0 || iIdx >= MAX_CLAN_DONATE)
	{
		set_script_variable(iRetIdx, -1);		
		return 0;
	}
	
	pstLog->Log[iIdx].DonateMoney = iDonateMoney;
	pstLog->Log[iIdx].ClanMoney = iClanMoney;
	pstLog->Log[iIdx].MemberContri = iContri;
	pstLog->Log[iIdx].LogTime = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;
	STRNCPY(pstLog->Log[iIdx].MemberName,pstPlayer->stRoleData.RoleName,
			sizeof(pstLog->Log[iIdx].MemberName));

	if (pstLog->Num < MAX_CLAN_DONATE)
	{
		pstLog->Num++;
	}
	else
	{
		pstLog->BeginPos++;
	}
	
	
	clan_action_clan_donate_log(stSEnv.pstEnv, pstClan,&pstLog->Log[iIdx]);
*/

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_add_id_player_contri()
{
	//参数1	返回标志
	//参数2	玩家MemID
	//参数3	贡献度
	//参数4	类型
	//参数5	实际增加值

	int iRetIdx = get_script_number(NULL);//返回标志
	int iPlayerID = get_script_number(NULL);//玩家MemID
	int iContri = get_script_number(NULL);//贡献度
	int iType = get_script_number(NULL);//类型
	int iContriIdx = get_script_number(NULL);//实际增加值

	Player* pstPlayer = NULL;
	ZoneClan* pstClan = NULL;

	if (iPlayerID)
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx,stSEnv.pstEnv,iPlayerID);
	}
	else
	{
		pstPlayer = stSEnv.pstPlayer;
	}
	
	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstClan = player_get_clan(stSEnv.pstEnv,pstPlayer);

	if (!pstClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;		
	}
	if (!iContri)
	{
		set_script_variable(iRetIdx, 0);
		return 0;		
	}
	
	if (iContri > 0)
	{
		clan_resource_inc(stSEnv.pstEnv,pstClan,pstPlayer,
						CLAN_OWNED_CONTRIBUT,(unsigned int)iContri);
	}
	else
	{		
		player_contribute_dec(stSEnv.pstEnv,pstPlayer, (unsigned int) -iContri,0);
	}

	set_script_variable(iContriIdx, iContri);

	set_script_variable(iRetIdx, 0);
	return 0;
	UNUSED(iType);
}

int s_get_yueyu_num()
{
	int iRetIdx;
	int iNum;
	Player *pstPlayer = stSEnv.pstPlayer;

	iRetIdx = get_script_number(NULL);
	iNum = get_script_number(NULL);
	
	if(NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(pstPlayer->stRoleData.MiscInfo.YueYuInfo.DayTime <= 0 ||
		!IsSameDay(pstPlayer->stRoleData.MiscInfo.YueYuInfo.DayTime, stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstPlayer->stRoleData.MiscInfo.YueYuInfo.DayTime = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;
		pstPlayer->stRoleData.MiscInfo.YueYuInfo.Num = 0;
		set_script_variable(iRetIdx, 0);
		set_script_variable(iNum , pstPlayer->stRoleData.MiscInfo.YueYuInfo.Num);
		return 0;
	}
	
	set_script_variable(iRetIdx, 0);
	set_script_variable(iNum , pstPlayer->stRoleData.MiscInfo.YueYuInfo.Num);
	return 0;
}

int s_add_yueyu_num()
{
	int iRetIdx;
	Player *pstPlayer = stSEnv.pstPlayer;

	iRetIdx = get_script_number(NULL);

	if(NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(pstPlayer->stRoleData.MiscInfo.YueYuInfo.DayTime <= 0 ||
		!IsSameDay(pstPlayer->stRoleData.MiscInfo.YueYuInfo.DayTime, stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstPlayer->stRoleData.MiscInfo.YueYuInfo.DayTime = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;
		pstPlayer->stRoleData.MiscInfo.YueYuInfo.Num = 0;
	}

	pstPlayer->stRoleData.MiscInfo.YueYuInfo.Num++;
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_gold_rate_get_money()
{
	int iRetIdx;
	Player *pstPlayer = stSEnv.pstPlayer;

	iRetIdx = get_script_number(NULL);

	if(NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, gold_rate_get_money(stSEnv.pstEnv, pstPlayer));
	
	return 0;
}

int s_gold_rate_dec_money()
{
	int iRetIdx;
	Player *pstPlayer = stSEnv.pstPlayer;

	iRetIdx = get_script_number(NULL);

	if(NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, gold_rate_dec_money(stSEnv.pstEnv, pstPlayer));
	
	return 0;
}

int s_gold_rate_daynum_add()
{
	int iRetIdx;
	Player *pstPlayer = stSEnv.pstPlayer;

	iRetIdx = get_script_number(NULL);

	if(NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	ROLEMISCGOLDRATEBOX *pstGoldRateBox = &pstPlayer->stRoleData.MiscInfo.GoldRateBox;

	if (	0 != pstGoldRateBox->DecTime && 
		IsSameDay(pstGoldRateBox->DecTime, stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		if (pstGoldRateBox->Num < 255)
		{
			pstGoldRateBox->Num++;
		}
	}
	else
	{
		pstGoldRateBox->Num = 1;
	}

	pstGoldRateBox->DecTime = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec;
	pstGoldRateBox->GetTime = 0;
	pstGoldRateBox->GoldRateBoxMoney = 0;

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_gold_rate_get_daynum()
{
	int iRetIdx;
	Player *pstPlayer = stSEnv.pstPlayer;

	iRetIdx = get_script_number(NULL);

	if(NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, gold_rate_get_daynum(stSEnv.pstEnv, pstPlayer));
	
	return 0;
}

int s_get_rand_luky_map()
{
	//参数1	返回标志
	//参数2	地图ID
	//参数3	怪物ID
	//参数4	地图怪物等级

	int iRetIdx = get_script_number(NULL);//返回标志
	int iMapIDIdx = get_script_number(NULL);//地图ID
	int iMonsterIDIdx = get_script_number(NULL);//怪物ID
	int iMapMonsterLevelIdx = get_script_number(NULL);//地图怪物等级

	int iRand = 0;
	int i;
	LUCKYMAPDEF* pstDef = NULL;
	ZONESVRENV* pstEnv = stSEnv.pstEnv;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	int iSum = 0;
	GameEventInst *pstEvent = NULL;

	pstEvent = stSEnv.pstEvent;

	if (!pstEvent)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (pstEnv->iLuckyRandSum <= 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "pstEnv->iLuckyRandSum invalid");
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iRand = RAND1(pstEnv->iLuckyRandSum);



	for (i=0; i<pstObj->iLuckyMapDef; i++)
	{
		
		pstDef = (LUCKYMAPDEF*)pstObj->sLuckyMapDef[i];
		
		iSum += pstDef->RandProb;
		
		if (iRand < iSum)
		{			
			set_script_variable(iMapIDIdx, pstDef->MapID);
			set_script_variable(iMonsterIDIdx, pstDef->MonsterID);
			set_script_variable(iMapMonsterLevelIdx, pstDef->MapMonsterLevel);

			pstEvent->iLuckyRandID = pstDef->ID;
			
			break;	
		}
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_set_clan_everydaypworld_boss_die()
{
	int iRetIdx;
	ZONEPWORLDINST *pstPworldInst = NULL;
	ZONEMAPINST* pstMapInst;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (pstPworldInst->iType == PWORLD_TYPE_CLAN)
	{
		ZoneClan* pstZoneClan = z_id_clan(stSEnv.pstEnv,  pstPworldInst->stPworldTypeData.stClan.iID);
		if (pstZoneClan)
		{
			pstZoneClan->stZoneClanInfo.iBossLife = 0;
			clan_broadcast_do_pkg(stSEnv.pstEnv, pstZoneClan, player_everyday_pworld_ui_info);
		}
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_add_item_bat_inpak_nolimit()
{
	int iItemID[ITEM_BAT_NUMMAX];
	int iNumIdx[ITEM_BAT_NUMMAX];
	int iNum ;
	int iFlag;
	int iRes;
	int iRetIdx;
	int i;
	ROLEGRID stRoleGrid[ITEM_BAT_NUMMAX], stDummy;

	iNum = get_script_number(NULL);

	if ( iNum <= 0 && iNum > ITEM_BAT_NUMMAX ) 
	{
		return -1;
	}
	
	for (i =0 ; i< ITEM_BAT_NUMMAX; i++)
	{
		iItemID[i] = get_script_number(NULL);
		iNumIdx[i] = get_script_number(NULL);
	}
	
	iFlag = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	for (i =0 ; i< iNum; i++)
	{
		if (0 == item_create(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stRoleGrid[i], iItemID[i] ) )
		{
				stRoleGrid[i].GridData.RoleItem.Num = iNumIdx[i];
		}
		else 
		{
				set_script_variable(iRetIdx, -1);
				return 0;
		}
	}

	//0 能加就加 1必须全部加进去   2 仅仅测试是否能全部放进去
	if(iFlag > 0)
	{
		iRes=package_add_test(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, &stRoleGrid[0], iNum,0);

		if (iRes == -1)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		//只测试是否可以加进去
		if(iFlag == 2)
		{
			set_script_variable(iRetIdx, 0);
			return 0;
		}
	}

	for (i =0 ; i< iNum; i++)
	{
		//op_log
		memcpy(&stDummy, &stRoleGrid[i], sizeof(stDummy));
		
		package_add(stSEnv.pstEnv->pstAppCtx , stSEnv.pstEnv , stSEnv.pstPlayer, &stRoleGrid[i], 1,0, IMPITEM_OPTYPE_SCRIPT);

		//记录全局限制的物品产出
		if(iNumIdx[i] - stRoleGrid[i].GridData.RoleItem.Num > 0)
		{
			stDummy.GridData.RoleItem.Num = iNumIdx[i]-stRoleGrid[i].GridData.RoleItem.Num;
			z_role_pickitem_oplog(stSEnv.pstEnv,stSEnv.pstPlayer, &stDummy, 1, OP_SCRIPT);
		}
	}
	
	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_add_item_num_inpak_nolimit()
{
	int iItemID;
	int iNumIdx;
	int iFlag;
	int iRes;
	int iRetIdx;
	ROLEGRID stRoleGrid, stDummy;
	
	iItemID = get_script_number(NULL);
	iNumIdx = get_script_number(NULL);
	iFlag = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if (0 == item_create(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stRoleGrid, iItemID))
	{
		stRoleGrid.GridData.RoleItem.Num = iNumIdx;

		//0 能加就加 1必须全部加进去   2 仅仅测试是否能全部放进去
		if(iFlag > 0)
		{
			iRes=package_add_test(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, &stRoleGrid,  1,0);
			if (iRes == -1)
			{
				z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,stSEnv.pstPlayer,SYS_MIDDLE2, ZONEERR_NOSPACE);
				set_script_variable(iRetIdx, iRes);
				return 0;
			}

			//只测试是否可以加进去
			if(iFlag == 2)
			{
				set_script_variable(iRetIdx, 0);
				return 0;
			}
		}	

		//op_log
		memcpy(&stDummy, &stRoleGrid, sizeof(stDummy));
		
		if( 0 > package_add(stSEnv.pstEnv->pstAppCtx , stSEnv.pstEnv , stSEnv.pstPlayer, &stRoleGrid, 1,0, IMPITEM_OPTYPE_SCRIPT))
		{
			z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,stSEnv.pstPlayer,SYS_MIDDLE2, ZONEERR_NOSPACE);
		}

		//记录全局限制的物品产出
		if(iNumIdx - stRoleGrid.GridData.RoleItem.Num > 0)
		{
			stDummy.GridData.RoleItem.Num = iNumIdx-stRoleGrid.GridData.RoleItem.Num;
			z_role_pickitem_oplog(stSEnv.pstEnv,stSEnv.pstPlayer, &stDummy, 1, OP_SCRIPT);
		}

		set_script_variable(iRetIdx, 0);
		
		return 0;

	}
	
	set_script_variable(iRetIdx, -1);
	return 0;
}

int s_add_bind_item_bat_inpak_nolimit()
{
	int iItemID[ITEM_BAT_NUMMAX];
	int iNumIdx[ITEM_BAT_NUMMAX];
	int iNum ;
	int iFlag;
	int iRes;
	int iRetIdx;
	int i;
	ROLEGRID stRoleGrid[ITEM_BAT_NUMMAX], stDummy;

	iNum = get_script_number(NULL);

	if ( iNum <= 0 && iNum > ITEM_BAT_NUMMAX ) 
	{
		return -1;
	}
	
	for (i =0 ; i< ITEM_BAT_NUMMAX; i++)
	{
		iItemID[i] = get_script_number(NULL);
		iNumIdx[i] = get_script_number(NULL);
	}
	
	iFlag = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	for (i =0 ; i< iNum; i++)
	{
		if (0 == item_create(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stRoleGrid[i], iItemID[i] ) )
		{
				stRoleGrid[i].GridData.RoleItem.Num = iNumIdx[i];

				stRoleGrid[i].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
		}
		else 
		{
				set_script_variable(iRetIdx, -1);
				return 0;
		}
	}

	//0 能加就加 1必须全部加进去   2 仅仅测试是否能全部放进去
	if(iFlag > 0)
	{
		iRes=package_add_test(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, &stRoleGrid[0], iNum,0);

		if (iRes == -1)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		if(iFlag == 2)
		{
			set_script_variable(iRetIdx, 0);
			return 0;
		}
	}

	for (i =0 ; i< iNum; i++)
	{
		//op_log
		memcpy(&stDummy, &stRoleGrid[i], sizeof(stDummy));
		
		package_add(stSEnv.pstEnv->pstAppCtx , stSEnv.pstEnv , stSEnv.pstPlayer, &stRoleGrid[i], 1,0, IMPITEM_OPTYPE_SCRIPT);

		//记录全局限制的物品产出
		if(iNumIdx[i] - stRoleGrid[i].GridData.RoleItem.Num > 0)
		{
			stDummy.GridData.RoleItem.Num = iNumIdx[i]-stRoleGrid[i].GridData.RoleItem.Num;
			z_role_pickitem_oplog(stSEnv.pstEnv,stSEnv.pstPlayer, &stDummy, 1, OP_SCRIPT);
		}
	}
	
	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_add_bind_item_num_inpak_nolimit()
{
	int iItemID;
	int iNumIdx;
	int iFlag;
	int iRes;
	int iRetIdx;
	ROLEGRID stRoleGrid, stDummy;
	
	iItemID = get_script_number(NULL);
	iNumIdx = get_script_number(NULL);
	iFlag = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if (0 == item_create(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stRoleGrid, iItemID))
	{
		stRoleGrid.GridData.RoleItem.Num = iNumIdx;
		stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
		
		//0 能加就加 1必须全部加进去   2 仅仅测试是否能全部放进去
		if(iFlag > 0)
		{
			iRes=package_add_test(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, &stRoleGrid,  1,0);
			if (iRes == -1)
			{
				set_script_variable(iRetIdx, iRes);
				return 0;
			}

			if(iFlag == 2)
			{
				set_script_variable(iRetIdx, 0);
				return 0;
			}
		}	

		//op_log
		memcpy(&stDummy, &stRoleGrid, sizeof(stDummy));
		
		if ( 0 > package_add(stSEnv.pstEnv->pstAppCtx , stSEnv.pstEnv , stSEnv.pstPlayer, &stRoleGrid, 1,0, IMPITEM_OPTYPE_SCRIPT))
		{
			z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,stSEnv.pstPlayer,SYS_MIDDLE2, ZONEERR_NOSPACE);
		}

		//记录全局限制的物品产出
		if(iNumIdx - stRoleGrid.GridData.RoleItem.Num > 0)
		{
			stDummy.GridData.RoleItem.Num = iNumIdx-stRoleGrid.GridData.RoleItem.Num;
			z_role_pickitem_oplog(stSEnv.pstEnv,stSEnv.pstPlayer, &stDummy, 1, OP_SCRIPT);
		}
		
		set_script_variable(iRetIdx, 0);
		
		return 0;

	}
	
	set_script_variable(iRetIdx, -1);
	
	return 0;
}

int s_get_clan_create_time()
{
	//参数1	返回标志
	//参数2	公会ID
	//参数3	创建时间

	int iRetIdx = get_script_number(NULL);//返回标志
	int iClanID = get_script_number(NULL);//公会ID
	int iCreateTimeIdx = get_script_number(NULL);//创建时间

	ZoneClan *pstZoneClan = NULL;
	
	Player *pstPlayer = NULL;
	
	if (iClanID)
	{
		pstZoneClan = z_id_clan(stSEnv.pstEnv,iClanID) ;
	}
	else
	{
		pstPlayer = stSEnv.pstPlayer;
		if (pstPlayer)
		{
			pstZoneClan = player_get_clan(stSEnv.pstEnv, pstPlayer);
		}
	}

	if (!pstZoneClan)
	{
		set_script_variable(iRetIdx, -1);
		set_script_variable(iCreateTimeIdx, -1);
		return 0;
	}

	set_script_variable(iCreateTimeIdx, pstZoneClan->stClan.ClanBrief.CreateTime);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_player_worship_effigy_test()
{
	int iFlower = get_script_number(NULL);//返回标志
	int iRetIdx = get_script_number(NULL);//返回标志

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if(0 > player_worship_effigy_test(stSEnv.pstEnv, stSEnv.pstPlayer, iFlower))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_player_worship_effigy()
{
	int iFlower = get_script_number(NULL);//返回标志
	int iRetIdx = get_script_number(NULL);//返回标志

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if(0 > player_worship_effigy(stSEnv.pstEnv, stSEnv.pstPlayer, iFlower))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_player_effigy_award()
{
	int iRetIdx = get_script_number(NULL);//返回标志

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(0 > player_effigy_award(stSEnv.pstEnv, stSEnv.pstPlayer))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_city_effigy_info()
{
	WORLDCITY *pstWorldCity = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	WORLDCITYEFFIGY *pstEffigy = &pstWorldCity->CityCore.Effigy;
	int iFlower = get_script_number(NULL);//返回标志
	int iEgg = get_script_number(NULL);//返回标志

	world_city_effigy_flower_update(stSEnv.pstEnv);
	
	set_script_variable(iFlower, pstEffigy->YesterdayFlower);
	set_script_variable(iEgg, pstEffigy->YesterdayEgg);
	return 0;


}

int s_is_uin_flag_set()
{
	//参数1	返回标志
	//参数2	玩家MemID
	//参数3	标记值

	int iRetIdx = get_script_number(NULL);//返回标志
	int iMemID = get_script_number(NULL);//玩家MemID
	int iFlag = get_script_number(NULL);//标记值

	Player *pstPlayer = NULL;

	if (0 == iMemID)
	{
		pstPlayer = stSEnv.pstPlayer;
	}
	else
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,iMemID);
	}

	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (pstPlayer->UinFlag & iFlag)
	{
		set_script_variable(iRetIdx, 1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_clan_join_time_no_sameday()
{
	CLANMEM *pstMem;
	ZoneClan *pstClan;
	int iRetIdx = get_script_number(NULL);//返回标志

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	pstClan = player_get_clan(stSEnv.pstEnv, stSEnv.pstPlayer);
	if(!pstClan)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}
	
	pstMem = clan_id_mem(pstClan, stSEnv.pstPlayer->stRoleData.RoleID, NULL);
	if (!pstMem)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	if(IsSameDay(pstMem->JoinTime, stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}
	
	set_script_variable(iRetIdx, 1);
	return 0;
}

int s_get_curr_max_level()
{
	int iRetIdx = get_script_number(NULL);//返回标志
	int iLevel = get_script_number(NULL);//当前最大等级
	GLOBAL *pstGlobal = &stSEnv.pstEnv->pstGlobalObj->stGlobal;
	
	if (stSEnv.pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat != GLOBAL_FINI)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);

	if (pstGlobal->LevFeng.LevelFeng > 0)
	{
		set_script_variable(iLevel, pstGlobal->LevFeng.LevelFeng);
	}
	else if (stSEnv.pstEnv->pstConf->MaxLevel > 0)
	{
		set_script_variable(iLevel, stSEnv.pstEnv->pstConf->MaxLevel);
	}
	else
	{
		set_script_variable(iLevel, MAX_LEVEL);
	}
	
	return 0;
}


int s_player_busy_ui()
{
	int iID = get_script_number(NULL);//返回标志
	int iRetIdx = get_script_number(NULL);//返回标志

	if (!stSEnv.pstPlayer || !stSEnv.pstNpc)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	player_busy_ui(stSEnv.pstEnv, stSEnv.pstPlayer, iID, stSEnv.pstNpc->stMon.iID);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_is_first_enter_strong()
{
	int iFirst = get_script_number(NULL);//返回标志
	int iMap = get_script_number(NULL);//返回标志
	STRONGPOINTONE *pstOne;
	unsigned int iID;

	if (!stSEnv.pstPlayer )
	{
		set_script_variable(iFirst, -1);
		return 0;
	}

	iID = stSEnv.pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id;

	pstOne =  strong_point_one_get(stSEnv.pstEnv, iMap);
	if (!pstOne ||  pstOne->Stat != STRONG_STAT_FIGHT)
	{
		set_script_variable(iFirst, -1);
		return 0;
	}

	if (pstOne->OwerID != iID && pstOne->AtkClanID != iID)
	{
		set_script_variable(iFirst, -1);
		return 0;
	}

	set_script_variable(iFirst,stSEnv.pstPlayer->stRoleData.MiscInfo.RoleStrongInfo1.StrongID <=0?1:0);
	return 0;
}

int s_player_chg_machine()
{
	int iRetIdx, iMachine;
	Player *pstPlayer;
	Monster *pstMachine;
	ZONEMAPINST *pstMapInst;

	iMachine = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	pstPlayer = stSEnv.pstPlayer;
	if (!pstPlayer )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(is_player_ride_on_machine(pstPlayer))
	{
		player_ride_machine_brk(stSEnv.pstEnv, pstPlayer, 1);
	}

	//产出一个空燃料空弹药的机械
	pstMachine = new_mon_machine(stSEnv.pstEnv, pstMapInst, iMachine, &pstPlayer->stRoleData.Pos, MACHINE_OWNER_SELF, (void *)pstPlayer, 0);
	if(pstMachine == NULL)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	//乘坐
	z_ride_on_machine(stSEnv.pstEnv, pstPlayer, pstMachine, machine_get_free_seatidx(stSEnv.pstEnv, pstMachine), 0);
	return 0;
}

int s_player_machine_add_hp()
{
	int iRetIdx;
	Player *pstPlayer;
	Monster *pstMon;

	iRetIdx = get_script_number(NULL);

	pstPlayer = stSEnv.pstPlayer;
	if (!pstPlayer )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	
	if(!is_player_ride_on_machine(pstPlayer))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMon = machine_get_by_player(stSEnv.pstEnv, pstPlayer);
	if(!pstMon)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMon->iHP = pstMon->iMaxHP;
	mon_hp_action(stSEnv.pstEnv, pstMon, 0, 0);
	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_pworld_camp_add_exp()
{
	int i, j, iDyn;
	tdr_longlong iExp, iTotalExp;
	int iRetIdx, iCamp;
	PlayerChagAttrVal stAttr;
	Player *pstTmpPlayer;
	ZONEMAPINST *pstMapInst;
	AREADYN *pstAreaDyn;
	LEVELUP *pstLevelUP;

	iCamp = get_script_number(NULL);
	iExp = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	//全地图找一遍
	for (i=0; i<pstMapInst->iAreaDyn; i++)
	{
		iDyn	 =	pstMapInst->aiAreaDyn[i];
		if( iDyn<0)
		{
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(stSEnv.pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		for(j=0; j<pstAreaDyn->iAnimate; j++)
		{
			if( j>=MAX_AREA_ANIMATE )
			{
				tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d", pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
				break;
			}

			pstTmpPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstAreaDyn->aiAnimate[j]);
			if (NULL == pstTmpPlayer || PLAYER_STATUS_ROLE_LOGIN != pstTmpPlayer->eStatus)
			{
				continue;
			}

			if(iCamp != pstTmpPlayer->stOnline.stWarOL.cCampIdx)
			{
				continue;
			}

			pstLevelUP = z_get_levelup(stSEnv.pstEnv, pstTmpPlayer->stRoleData.Level);
			if(!pstLevelUP)
			{
				continue;
			}

			iTotalExp = pstLevelUP->ContributeExp/10000.0/iExp;
			z_player_addexp_test(stSEnv.pstEnv, pstTmpPlayer, &iTotalExp);

			if (iTotalExp > 0)
			{
				stAttr.AttrList[0].AttrID = ATTR_ID_EXP;
				stAttr.AttrList[0].Value.Exp= iTotalExp;
				stAttr.AttrList[0].nFlag = 0;
				stAttr.AttrNum = 1;
				
				z_attr_chg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstTmpPlayer, &stAttr, 1);
				z_script_add_exp_oplog(stSEnv.pstEnv,pstTmpPlayer,iTotalExp, stSEnv.szScript);
			}
		}
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_mon_num_radius()
{
	int i, j, iDyn;
	Monster *pstMon;
	AREADYN *pstAreaDyn; 
	ZONEIDX stIdx;
	RESPOS stPosOri;
	int iCount = 0;
	ZONEMAPINST *pstMapInst = NULL;
	int iMapID = get_script_number(NULL);
	int iX = get_script_number(NULL);
	int iY = get_script_number(NULL);
	int iRadius = get_script_number(NULL);
	int iMonDefID = get_script_number(NULL);
	int iRet = get_script_number(NULL);
	int iNum = get_script_number(NULL);

	stPosOri.X = iX;
	stPosOri.Y = iY;
		
	if (iMapID == 0)
	{
		pstMapInst = s_get_mapinst();
	}
	else
	{
		stIdx.iID	 =	iMapID;
		stIdx.iPos =	-1;
		pstMapInst = z_find_map_inst( stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stIdx);
	}

	if (NULL == pstMapInst)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	for (i=0; i<pstMapInst->iAreaDyn; i++)
	{
		iDyn	 =	pstMapInst->aiAreaDyn[i];
		if( iDyn<0)
		{
			continue;
		}

		pstAreaDyn = (AREADYN*)tmempool_get(stSEnv.pstEnv->pstAreaDynPool, iDyn);
		if (NULL == pstAreaDyn)
		{
			tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "Dyn=%d", iDyn);
			continue;
		}

		for(j=0; j<pstAreaDyn->iAnimate; j++)
		{
			if( j>=MAX_AREA_ANIMATE )
			{
				tlog_error(stSEnv.pstEnv->pstLogCat, 0, 0, "map %d dyn %d iAnimate=%d", pstMapInst->iMapInstID, iDyn, pstAreaDyn->iAnimate);
				break;
			}

			pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstAreaDyn->aiAnimate[j]);
			if (pstMon &&  pstMon->iDefIdx == iMonDefID && iRadius >= z_distance(&pstMon->stCurrPos, &stPosOri))
			{
				iCount ++;
			}
		}
	}

	set_script_variable(iRet, 0);
	set_script_variable(iNum, iCount);
	return 0;

	
	return 0;
}

int s_clan_baby_feed()
{
	int iRetIdx, iFeed;
	Player *pstPlayer;
	ZoneClan *pstZoneClan;

	iFeed = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	pstPlayer = stSEnv.pstPlayer;
	if (!pstPlayer )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstZoneClan = player_get_clan(stSEnv.pstEnv, pstPlayer);
	if(!pstZoneClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(!clan_has_baby(stSEnv.pstEnv, pstZoneClan))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	clan_baby_feed_chg(stSEnv.pstEnv, pstZoneClan, iFeed);
	set_script_variable(iRetIdx, 0);	
	return 0;
}

int s_player_is_jiyang()
{
	int iRetIdx, iHas;
	ZoneClan *pstClan;
	CLANPET *pstClanPet;

	iRetIdx = get_script_number(NULL);
	iHas = get_script_number(NULL);

	if (!stSEnv.pstEnv || !stSEnv.pstPlayer)
	{
		return 0;
	}
	
	pstClan = player_get_clan(stSEnv.pstEnv, stSEnv.pstPlayer);
	if (!pstClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstClanPet = &pstClan->stClan.ClanPet;
	
	if (clan_pet_by_player(pstClanPet, stSEnv.pstPlayer))
	{
		set_script_variable(iRetIdx, 0);
		set_script_variable(iHas, 1);
		return 0;
	}
	else
	{
		set_script_variable(iRetIdx, 0);
		set_script_variable(iHas, 0);
	}
	return 0;
}

int s_player_clan_pet_unlock()
{
	int iRetIdx;
	iRetIdx = get_script_number(NULL);
	ZoneClan *pstClan;
	CLANPET *pstClanPet;
	ROLEPET *pstRolePet;
	CLANPETONE *pstOne;

	if (!stSEnv.pstEnv || !stSEnv.pstPlayer)
	{
		goto error;
	}
	
	pstClan = player_get_clan(stSEnv.pstEnv, stSEnv.pstPlayer);
	if (!pstClan)
	{
		goto error;
	}
	
	if (!is_in_my_clan_city(stSEnv.pstEnv, stSEnv.pstPlayer))
	{
		goto error;
	}

	pstClanPet = &pstClan->stClan.ClanPet;
	
	pstOne = clan_pet_by_player(pstClanPet, stSEnv.pstPlayer);
	if (!pstOne)
	{
		goto error;
	}

	pstRolePet = z_find_role_pet(stSEnv.pstPlayer, pstOne->PetWid, NULL);
	if (!pstRolePet || !(pstRolePet->MiscFlag &PET_MISC_FLAG_JIYANG))
	{
		goto error;
	}
	

	clan_pet_jiyang_unlock(stSEnv.pstEnv,stSEnv.pstPlayer,pstRolePet,pstOne,pstClanPet,stSEnv.pstPlayer,pstClan);
	set_script_variable(iRetIdx, 0);
	return 0;

error:
	set_script_variable(iRetIdx, -1);
	return 0;
}


int s_get_clan_baby_feed()
{
	int iRetIdx;
	Player *pstPlayer;
	ZoneClan *pstZoneClan;

	iRetIdx = get_script_number(NULL);

	pstPlayer = stSEnv.pstPlayer;
	if (!pstPlayer )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstZoneClan = player_get_clan(stSEnv.pstEnv, pstPlayer);
	if(!pstZoneClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(!clan_has_baby(stSEnv.pstEnv, pstZoneClan))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, clan_baby_feed_get(stSEnv.pstEnv, pstZoneClan));	
	return 0;
}

int s_get_clan_baby_stat()
{
	int iRetIdx;
	Player *pstPlayer;
	ZoneClan *pstZoneClan;

	iRetIdx = get_script_number(NULL);

	pstPlayer = stSEnv.pstPlayer;
	if (!pstPlayer )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstZoneClan = player_get_clan(stSEnv.pstEnv, pstPlayer);
	if(!pstZoneClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(!clan_has_baby(stSEnv.pstEnv, pstZoneClan))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, clan_baby_stat_get(stSEnv.pstEnv, pstZoneClan));	
	return 0;
}

int s_clan_baby_call()
{
	int iRetIdx;
	Player *pstPlayer;
	
	iRetIdx = get_script_number(NULL);

	pstPlayer = stSEnv.pstPlayer;
	if (!pstPlayer )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(0 > clan_baby_call(stSEnv.pstEnv, pstPlayer))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);	
	return 0;

}

int s_clan_baby_die()
{
	int iRetIdx;
	Monster *pstMon;
	ZoneClan *pstZoneClan;

	iRetIdx = get_script_number(NULL);

	pstMon = stSEnv.pstMonster;
	if(!pstMon)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(pstMon->ullBootyClanGID <= 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstZoneClan = z_id_clan(stSEnv.pstEnv, pstMon->nOwnerFastID);
	if(!pstZoneClan || pstMon->ullBootyClanGID != pstZoneClan->stClan.ClanProf.GID)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(CLAN_BABY_STAT_CALL != clan_baby_stat_get(stSEnv.pstEnv, pstZoneClan))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	clan_baby_die_award(stSEnv.pstEnv, pstZoneClan, pstMon);

	pstZoneClan->stClan.ClanCore.ClanBaByInfo.CallBaby = CLAN_BABY_STAT_DIE;
	set_script_variable(iRetIdx, 0);	
	return 0;
}

int s_player_clan_baby_award()
{
	int iRetIdx;
	Player *pstPlayer;

	iRetIdx = get_script_number(NULL);

	pstPlayer = stSEnv.pstPlayer;
	if (!pstPlayer )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(0 > player_clan_baby_award(stSEnv.pstEnv, pstPlayer))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	set_script_variable(iRetIdx, 0);	
	return 0;
}

int s_get_star_info()
{
	int i;
	int iVipID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	Player *pstPlayer = stSEnv.pstPlayer;
	VIPDATA *pstVipData;

	if (NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	pstVipData = &pstPlayer->stRoleData.MiscInfo.VipData;

	for (i=0; i<pstVipData->StoneNum; i++)
	{
		if (pstVipData->StarStoneInfo[i].ID == iVipID)
		{
			set_script_variable(iRetIdx, 1);
			return 0;
		}
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_taskcycle_stat()
{
	int i;
	int iTaskCycleID;
	int iRetIdx;
	int iTaskStat;
	TASKRUNONE *pstTaskRunOne;
	Player *pstPlayer = stSEnv.pstPlayer;
	TASKRUN *pstTaskRun;
	TASKDEF * pstTaskDef;
	
	iTaskCycleID = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iTaskStat = get_script_number(NULL);

	if (NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		set_script_variable(iTaskStat, 0);
		return 0;
	}

	pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i=0; i<pstTaskRun->TaskRunNum; i++)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (pstTaskRunOne->CycleID != iTaskCycleID)
		{
			continue;
		}

		pstTaskDef = task_def_find(stSEnv.pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			set_script_variable(iRetIdx, -1);
			set_script_variable(iTaskStat, 0);
			return 0;
		}
		
		if (task_check_fini(stSEnv.pstEnv, pstPlayer, pstTaskRunOne, pstTaskDef))
		{
			set_script_variable(iTaskStat, 1);
		}
		else
		{
			if(pstTaskRunOne->TaskStat == TASK_STAT_FAIL)
			{
				set_script_variable(iTaskStat, 4);
			}
			else
			{
				set_script_variable(iTaskStat, 2);
			}
		}
			
		break;
	}

	if (i >= pstTaskRun->TaskRunNum)
	{
		set_script_variable(iTaskStat, 0);
	}

	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_get_dayonline()
{
	Player *pstPlayer = stSEnv.pstPlayer;
	int iRetIdx = get_script_number(NULL);
	int iOnline = get_script_number(NULL);

	if (NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		set_script_variable(iOnline, 0);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iOnline, pstPlayer->stRoleData.MiscInfo.ODTime.OndaySec);
	
	return 0;
}

int s_arm_is_fashion()
{
	Player *pstPlayer = stSEnv.pstPlayer;
	ITEMDEF *pstItemDef = NULL;
	int iRetIdx = get_script_number(NULL);
	int iDefId = get_script_number(NULL);

	if (NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstItemDef = z_find_itemdef(stSEnv.pstEnv, iDefId);
	if(	!pstItemDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (!arm_is_fashion(pstItemDef))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_get_npc_defid()
{
	Npc *pstNpc = stSEnv.pstNpc;
	int iRetIdx = get_script_number(NULL);
	int iDefId = get_script_number(NULL);

	if (NULL == pstNpc)
	{
		set_script_variable(iRetIdx, -1);
		set_script_variable(iDefId, 0);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iDefId, pstNpc->stMon.iDefIdx);
	
	return 0;
}

EXCHGITEMONEINFO* player_exchg_info_insert(ZONESVRENV *pstEnv, Player *pstPlayer, int iID)
{
	EXCHGITEMONEINFO *pstOneInfo;
	EXCHGITEMDEF *pstExchgItemDef;
	EXCHGITEMINFO *pstExchgItemInfo = &pstPlayer->stRoleData.MiscInfo.ExchgItemInfo;

	if(pstExchgItemInfo->Num >= MAX_EXCHG_ITEM_LIST)
	{
		return NULL;
	}

	pstExchgItemDef = z_find_exchg_item_def(pstEnv, iID);
	if(!pstExchgItemDef)
	{
		return NULL;
	}

	pstOneInfo = &pstExchgItemInfo->ListInfo[pstExchgItemInfo->Num];
	pstOneInfo->ID = pstExchgItemDef->ID;
	pstOneInfo->Num = pstExchgItemDef->EveryDayExchgNum;
	pstOneInfo->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstExchgItemInfo->Num++;
	
	return pstOneInfo;
}

EXCHGITEMONEINFO* player_exchg_info_get(ZONESVRENV *pstEnv, Player *pstPlayer, int iID)
{
	int i;
	int iSec, iDay;
	time_t tTime;
	EXCHGITEMONEINFO* pstOneInfo = NULL;
	EXCHGITEMDEF *pstExchgItemDef;
	EXCHGITEMINFO *pstExchgItemInfo = &pstPlayer->stRoleData.MiscInfo.ExchgItemInfo;

	for(i=0; i<pstExchgItemInfo->Num; i++)
	{
		if(pstExchgItemInfo->ListInfo[i].ID == iID)
		{
			pstOneInfo = &pstExchgItemInfo->ListInfo[i];
		}
	}

	if(!pstOneInfo)
	{
		return NULL;
	}

	pstExchgItemDef = z_find_exchg_item_def(pstEnv, iID);
	if(!pstExchgItemDef)
	{
		return NULL;
	}

	if(!IsSameDay(pstOneInfo->Time, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstOneInfo->Num += pstExchgItemDef->EveryDayExchgNum;
		
		iSec = pstEnv->pstAppCtx->stCurr.tv_sec - pstOneInfo->Time;
		if(iSec > 86400)
		{
			iDay = iSec/86400;
			if(iDay > 0)
			{
				tTime = pstOneInfo->Time + iDay*86400;
				if(IsSameDay(tTime, pstEnv->pstAppCtx->stCurr.tv_sec))
				{
					iDay = iDay -1;
				}

				pstOneInfo->Num += pstExchgItemDef->EveryDayExchgNum*iDay;
			}
		}
		pstOneInfo->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	if(pstOneInfo->Num > pstExchgItemDef->MaxExchgNum)
	{
		pstOneInfo->Num = pstExchgItemDef->MaxExchgNum;
	}

	return pstOneInfo;
}

int player_exchg_item(ZONESVRENV *pstEnv, Player *pstPlayer, int iID)
{
	int i;
	ROLEGRID stRoleGrid;
	EXCHGITEMDEF *pstExchgItemDef;
	EXCHGITEMONEINFO *pstExchgOneInfo;

	pstExchgItemDef = z_find_exchg_item_def(pstEnv, iID);
	if(!pstExchgItemDef)
	{
		return -1;
	}

	pstExchgOneInfo = player_exchg_info_get(pstEnv, pstPlayer, iID);
	if(!pstExchgOneInfo)
	{
		pstExchgOneInfo = player_exchg_info_insert(pstEnv, pstPlayer, iID);
		if(!pstExchgOneInfo)
		{
			return -1;
		}
	}

	if(pstExchgOneInfo->Num <= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SCRIPT310);
		return -1;
	}

	for(i=0; i<MAX_EXCHG_DEC_ITEM; i++)
	{
		if(pstExchgItemDef->DecItem[i].ItemID == 0)
		{
			break;
		}
		
		if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
			pstExchgItemDef->DecItem[i].ItemID, pstExchgItemDef->DecItem[i].Num, -1, NULL,PACKAGE_DEC_NONE))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SCRIPT311);
			return -1;
		}
	}

	if(0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstExchgItemDef->AddItemID))
	{
		return -1;
	}

	if(stRoleGrid.Type == GRID_ITEM_ITEM)
	{
		stRoleGrid.GridData.RoleItem.Num = pstExchgItemDef->AddNum;
	}
	stRoleGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;

	if(0> package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1, 0))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
		return -1;
	}

	for(i=0; i<MAX_EXCHG_DEC_ITEM; i++)
	{
		if(pstExchgItemDef->DecItem[i].ItemID == 0)
		{
			break;
		}
		
		if (0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
			pstExchgItemDef->DecItem[i].ItemID, pstExchgItemDef->DecItem[i].Num, -1, NULL, PACKAGE_DEC_NONE, IMPITEM_OPTYPE_SCRIPT))
		{
			return -1;
		}
	}
	
	pstExchgOneInfo->Num--;
	pstExchgOneInfo->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
	package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1, 0, IMPITEM_OPTYPE_SCRIPT);
	return 0;
}

int s_exchg_item()
{
	int iID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(0 > player_exchg_item(stSEnv.pstEnv, stSEnv.pstPlayer, iID))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);

	return 0;
}

int s_exchg_item_get()
{
	EXCHGITEMDEF *pstExchgItemDef;
	EXCHGITEMONEINFO *pstExchgOneInfo;
	int iID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstExchgItemDef = z_find_exchg_item_def(stSEnv.pstEnv, iID);
	if(!pstExchgItemDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstExchgOneInfo = player_exchg_info_get(stSEnv.pstEnv, stSEnv.pstPlayer, iID);
	if(!pstExchgOneInfo)
	{
		set_script_variable(iRetIdx, pstExchgItemDef->EveryDayExchgNum);
		return 0;
	}

	set_script_variable(iRetIdx, pstExchgOneInfo->Num);
	return 0;
}

int s_pworld_player_exp()
{
	int iRetIdx = get_script_number(NULL);//返回值
	int iType = get_script_number(NULL);// 1单人副本；2多人副本
	tdr_ulonglong uExp = 0;

	if (NULL == stSEnv.pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	uExp = pworld_liuyi_player_exp(stSEnv.pstEnv,stSEnv.pstPworldInst,iType, 0);
	
	set_script_variable(iRetIdx, 0);
	z_script_add_exp_oplog(stSEnv.pstEnv,stSEnv.pstPlayer,uExp, stSEnv.szScript);
	return 0;
	/*
	CSPKG *pstPkg = &stSEnv.stDlgPkg;
	CSSYSMSG *pstSysMsg = &pstPkg->Body.SysMsg;
	Player *pstPworldPlayer;
	LEVELUP *pstLevelUP;
	tdr_longlong llExp = 0;
	char szMsg[256];
	int i;
	
	int iRetIdx = get_script_number(NULL);//返回值
	int iType = get_script_number(NULL);// 1单人副本；2多人副本
	int isSucc = get_script_number(NULL);//是否成功
	
	if (NULL == stSEnv.pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	pstSysMsg->Type = SYS_POP_WINDOW;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;
	
	for (i=0; i< stSEnv.pstPworldInst->iPlayer; i++)
	{
		pstPworldPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPworldInst->aiPlayerID[i]);
		if (!pstPworldPlayer)
			continue;
		
		pstLevelUP = z_get_levelup(stSEnv.pstEnv, pstPworldPlayer->stRoleData.Level);
		if(pstLevelUP)
		{
			if(isSucc)
			{
				if(iType == 1)
				{
					llExp = (tdr_ulonglong)100*pstLevelUP->ContributeExp/10000.0;
				}
				else
				{
					llExp = (tdr_ulonglong)200*pstLevelUP->ContributeExp/10000.0;
				}
				snprintf(szMsg,sizeof(szMsg),"恭喜，由于您的机智果决，神奇的泡泡最终归于了乖乖归一了，以下是您的胜利奖励：%llu经验。",llExp);	
			}
			else
			{
				if(iType == 1)
				{
					llExp = (tdr_ulonglong)50*pstLevelUP->ContributeExp/10000.0;
				}
				else
				{
					llExp = (tdr_ulonglong)10*pstLevelUP->ContributeExp/10000.0;
				}
				snprintf(szMsg,sizeof(szMsg),"很遗憾，时间到了，泡泡小屋里还是一团糟，不过还是感谢您的到来，以下是您的参与奖励：%llu经验。",llExp);	
			}
			
			player_add_exp(stSEnv.pstEnv, pstPworldPlayer, llExp, 0);

			//弹出提示
			STRNCPY(pstSysMsg->Msg, szMsg, sizeof(szMsg));
			
			Z_CSHEAD_INIT(&pstPkg->Head, SYS_MSG);
			z_cltmsg_send(stSEnv.pstEnv, pstPworldPlayer, pstPkg);
			
			
		}
	}
	
	set_script_variable(iRetIdx, 0);
	return 0;
	*/
}

int s_open_school_invite_ui()
{
	int iRetIdx = get_script_number(NULL);
	
	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	player_open_school_invite_ui(stSEnv.pstEnv, stSEnv.pstPlayer);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_school_fini()
{
	int iRetIdx = get_script_number(NULL);

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	player_school_fini(stSEnv.pstEnv, stSEnv.pstPlayer);
	set_script_variable(iRetIdx, 0);
	return 0;
}


// 获取登记信息
int s_get_reg_info()
{
	int iRetIdx = get_script_number(NULL);
	int iNpcID = 0;

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (stSEnv.pstNpc)
	{
		iNpcID = stSEnv.pstNpc->stMon.iID;
	}

	player_reg_info_notify(stSEnv.pstEnv, stSEnv.pstPlayer, iNpcID);
	set_script_variable(iRetIdx, 0);
	
	return 0;
}

int s_commit_xukong_energy()
{
	int iRetIdx = get_script_number(NULL);

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(0 > player_commit_xukong_energy(stSEnv.pstEnv, stSEnv.pstPlayer))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_pworld_box_start()
{
	ZONEMAPINST *pstMapInst;
	int iRetIdx = get_script_number(NULL);

	pstMapInst = s_get_mapinst();
	if(!pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if(0 > pworld_box_start(stSEnv.pstEnv, pstMapInst))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_pworld_box_award()
{
	ZONEMAPINST *pstMapInst;
	int iRetIdx = get_script_number(NULL);

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(!stSEnv.pstNpc)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapInst = s_get_mapinst();
	if(!pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(0 > pworld_box_award(stSEnv.pstEnv, stSEnv.pstPlayer, stSEnv.pstNpc, pstMapInst))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_player_yanghaibang_start_check()
{
	int iRetIdx = get_script_number(NULL);

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	set_script_variable(iRetIdx, player_yanghaibang_start_check(stSEnv.pstEnv,stSEnv.pstPlayer));
	return 0;
}


int s_player_yanghaibang_start()
{
	int iRetIdx = get_script_number(NULL);

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if(0 > player_yanghaibang_start(stSEnv.pstEnv,stSEnv.pstPlayer))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_item_is_chenghao()
{
	int i;
	ITEMDEF *pstItemDef;
	int iRetIdx = get_script_number(NULL);
	int iDefId = get_script_number(NULL);

	pstItemDef = z_find_itemdef(stSEnv.pstEnv, iDefId);
	if(	!pstItemDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	for (i=0; i<MAX_ITEM_RESULT; i++)
	{
		if (pstItemDef->Result[i].ResultID == 0)
		{
			break;
		}

		if (pstItemDef->Result[i].ResultID == RESULT_ADD_DESIGNATION)
		{
			set_script_variable(iRetIdx, 0);
			return 0;
		}
	}

	set_script_variable(iRetIdx, -1);
		
	return 0;
}

int s_item_is_fumo()
{
	int i;
	ITEMDEF *pstItemDef;
	int iRetIdx = get_script_number(NULL);
	int iDefId = get_script_number(NULL);

	pstItemDef = z_find_itemdef(stSEnv.pstEnv, iDefId);
	if(	!pstItemDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	for (i=0; i<MAX_ITEM_RESULT; i++)
	{
		if (pstItemDef->Result[i].ResultID == 0)
		{
			break;
		}

		if (pstItemDef->Result[i].ResultID == RESULT_SECONDARY_SKILL_RECIPE &&
			pstItemDef->Result[i].ResultVal1 == SECONDARY_SKILL_TYPE_ENCHANT)
		{
			set_script_variable(iRetIdx, 0);
			return 0;
		}
	}

	set_script_variable(iRetIdx, -1);
		
	return 0;
}

int s_item_is_zhuzhao()
{
	int i;
	ITEMDEF *pstItemDef;
	int iRetIdx = get_script_number(NULL);
	int iDefId = get_script_number(NULL);

	pstItemDef = z_find_itemdef(stSEnv.pstEnv, iDefId);
	if(	!pstItemDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	for (i=0; i<MAX_ITEM_RESULT; i++)
	{
		if (pstItemDef->Result[i].ResultID == 0)
		{
			break;
		}

		if (pstItemDef->Result[i].ResultID == RESULT_SECONDARY_SKILL_RECIPE &&
			pstItemDef->Result[i].ResultVal1 == SECONDARY_SKILL_TYPE_BLACKSMITHING)
		{
			set_script_variable(iRetIdx, 0);
			return 0;
		}
	}

	set_script_variable(iRetIdx, -1);
		
	return 0;
}

int s_item_is_lianjin()
{
	int i;
	ITEMDEF *pstItemDef;
	int iRetIdx = get_script_number(NULL);
	int iDefId = get_script_number(NULL);

	pstItemDef = z_find_itemdef(stSEnv.pstEnv, iDefId);
	if(	!pstItemDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	for (i=0; i<MAX_ITEM_RESULT; i++)
	{
		if (pstItemDef->Result[i].ResultID == 0)
		{
			break;
		}

		if (pstItemDef->Result[i].ResultID == RESULT_SECONDARY_SKILL_RECIPE &&
			pstItemDef->Result[i].ResultVal1 == SECONDARY_SKILL_TYPE_ALCHEMY)
		{
			set_script_variable(iRetIdx, 0);
			return 0;
		}
	}

	set_script_variable(iRetIdx, -1);
		
	return 0;
}


//lzk  获取对话角色所在公会的繁荣度排名
int s_player_clan_develop_ranking()
{
//参数1     返回的繁荣度排名
//参数2     返回标志
	int iRetRanking = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	ZoneObj *pstObj = stSEnv.pstEnv->pstZoneObj;
	int i = 0;
	RangeOneMem *pstRangeClan = NULL;
	RangeOne *pstRange = NULL;
	int iRanking = 0;
	int k = 0;
	ZoneClan *pstZoneClan = NULL;
	tdr_ulonglong iGId;
	CLANPROF *pstClanProf;

	if (stSEnv.pstPlayer)
	{
		pstZoneClan = player_get_clan(stSEnv.pstEnv, stSEnv.pstPlayer);
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if (!pstZoneClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstClanProf = &pstZoneClan->stClan.ClanProf;

	iGId = pstClanProf->GID;
	
	for (i=0; i<pstObj->stRange.iRangeNum; i++)
	{
		RangeOneMem *pstRangeOneMem = &pstObj->stRange.astRange[i];

		if (pstRangeOneMem->iMainType == RANGE_MAIN_CLAN && 
			RANGE_MAIN_CLAN_SUB_DEVELOP == pstRangeOneMem->iSubType)
		{
			pstRangeClan = pstRangeOneMem;
			break;
		}
	}

	if (!pstRangeClan)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstRange = &pstRangeClan->stLastRange;
		
	//获取排名
	for (k=0; k<pstRange->iNum; k++)
	{
		if(pstRange->astRangeEntry[k].stRange.RangeData.GID == iGId)
		{
			iRanking = pstRange->astRangeEntry[k].stRange.TopRange;
			break;
		}
	}

	//未上榜   返回标志为-2
	if(k >= pstRange->iNum)
	{
		set_script_variable(iRetIdx, -2);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iRetRanking, iRanking);
		
	return 0;


}

int s_camp_mon_map_show_action()
{
	//参数1	返回标志
	//参数2	阵营
	//参数3	类型

	int iRetIdx = get_script_number(NULL);//返回标志
	int iCamp = get_script_number(NULL);//阵营
	int iType = get_script_number(NULL);//类型
	int iBroadcastType = get_script_number(NULL);//广播类型
	PworldCampPlayerInfo *pstCampInfo;

	ZONEMAPINST *pstMapInst;
	Player* pstPlayer = NULL;

	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if (!stSEnv.pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPlayer = stSEnv.pstPlayer;

	if (0 == iBroadcastType && NULL == pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (0 == iCamp)
	{
		int i ;
		for (i = 0; i < stSEnv.pstPworldInst->stPworldCamp.iCampNum;i++)
		{
			pstCampInfo = &stSEnv.pstPworldInst->stPworldCamp.stCampInfo[i];
			mon_action_map_show(stSEnv.pstEnv,&pstCampInfo->stMapShowMon, pstMapInst,
				iType,iBroadcastType,pstPlayer);
		}
	}
	else
	{
		iCamp --;
		if (iCamp < 0 ||
			iCamp >= stSEnv.pstPworldInst->stPworldCamp.iCampNum )
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		pstCampInfo = &stSEnv.pstPworldInst->stPworldCamp.stCampInfo[iCamp];
		mon_action_map_show(stSEnv.pstEnv,&pstCampInfo->stMapShowMon, pstMapInst, 
			iType,iBroadcastType,pstPlayer);
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_set_camp_mon_map_show()
{
	PworldCampPlayerInfo *pstCampInfo;
	Monster* pstMon;
	Npc* pstNpc;
	//参数1	返回标志
	//参数2	怪物MemID
	//参数3	阵营
	//参数4	类型

	int iRetIdx = get_script_number(NULL);//返回标志
	int iMemID = get_script_number(NULL);//怪物MemID
	int iCamp = get_script_number(NULL);//阵营
	int iType = get_script_number(NULL);//类型

	if (0 == iMemID)
	{
		if (stSEnv.pstMonster)
		{
			pstMon = stSEnv.pstMonster;
		}
		else if (stSEnv.pstNpc)
		{
			pstMon = &stSEnv.pstNpc->stMon;
		}
	}
	else
	{
		pstMon = z_id_monster(stSEnv.pstEnv->pstAppCtx,stSEnv.pstEnv,iMemID);
		if (!pstMon)
		{
			pstNpc = z_id_npc(stSEnv.pstEnv->pstAppCtx,stSEnv.pstEnv,iMemID);
			if (pstNpc)
			{
				pstMon = &pstNpc->stMon;
			}
		}
	}

	if (!pstMon)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (!stSEnv.pstPworldInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	iCamp --;
	if (iCamp < 0 ||
		iCamp >= stSEnv.pstPworldInst->stPworldCamp.iCampNum )
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstCampInfo = &stSEnv.pstPworldInst->stPworldCamp.stCampInfo[iCamp];
	memset(&pstCampInfo->stMapShowMon,0,sizeof(pstCampInfo->stMapShowMon));
	
	pstCampInfo->stMapShowMon.iMemID = iMemID;
	pstCampInfo->stMapShowMon.iDefID = pstMon->iDefIdx;
	pstCampInfo->stMapShowMon.stPos = pstMon->stCurrPos;
	pstCampInfo->stMapShowMon.iHP = pstMon->iHP;
	pstCampInfo->stMapShowMon.iCamp = iCamp + 1;

	pstMon->cMonMiscFlag |= MON_MISC_FLAG_MAP_SHOW;



	set_script_variable(iRetIdx, 0);
	UNUSED(iType);
	return 0;
}

//lzk   设置跑商的相关信息
int s_set_paoshang_info()
{
	//参数1	类型
	//参数2	设置值
	//参数3	返还标志

	//类型:
	//1：当前已兑换银票数量值
	//2:    银票额 
	//3:    npc的memid
	//4:    兑换银票开始结束标志  0表示可以出现兑换选项，1表示不可出现兑换选项
	
	int iType = get_script_number(NULL);//类型
	int iSetVal = get_script_number(NULL);//设置值
	int iRetIdx = get_script_number(NULL);//返回标志

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	PAOSHANGPLAYER *pstPaoShangPl = &stSEnv.pstPlayer->stRoleData.MiscInfo.PaoShang;
	if(NULL == pstPaoShangPl)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if(1 == iType)
	{
		pstPaoShangPl->YiExchangeYinPiao = iSetVal;
	}
	else if(2 == iType)
	{
		pstPaoShangPl->YinPiao = iSetVal;
	}
	else if(3 == iType)
	{
		stSEnv.pstPlayer->stOnline.iPaoShangNpcID = iSetVal;
	}
	else if(4 == iType)
	{
		pstPaoShangPl->ExchangeFlag = iSetVal;
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

//获取跑商的相关信息
int s_get_paoshang_info()
{
	//参数1	类型
	//参数2	返回值
	//参数3	返还标志

	//类型:
	//1：获取最多可兑换银票
	//2：当前已兑换银票数量值
	//3:    身上已有物品的数量 
	//4:    银票额 
	//5：交票额
    //6:    兑换银票的标志，0表示可以出现兑换选项，1表示不可出现兑换选项
	int iType = get_script_number(NULL);//类型
	int iGetVal = get_script_number(NULL);//返回值
	int iRetIdx = get_script_number(NULL);//返回标志

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	PAOSHANGPLAYER *pstPaoShangPl = &stSEnv.pstPlayer->stRoleData.MiscInfo.PaoShang;
	if(NULL == pstPaoShangPl)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if(1 == iType)
	{
		//每个任务最多可以兑换交票金额的30%
		set_script_variable(iGetVal, pstPaoShangPl->JiaoPiao/100*30);
	}
	else if(2 == iType)
	{
		set_script_variable(iGetVal, pstPaoShangPl->YiExchangeYinPiao);
	}
	else if(3 == iType)
	{
		set_script_variable(iGetVal, pstPaoShangPl->YiHaveItemNum);
	}
	else if(4 == iType)
	{
		set_script_variable(iGetVal, pstPaoShangPl->YinPiao);
	}
	else if(5 == iType)
	{
		set_script_variable(iGetVal, pstPaoShangPl->JiaoPiao);
	}
	else if(6 == iType)
	{
		set_script_variable(iGetVal, pstPaoShangPl->ExchangeFlag);
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

//lzk  打开跑商界面
int s_open_paoshang_ui()
{
	int iPaoShaId = get_script_number(NULL);//跑商id   跑商定义表的id
	int iPaoShaNpcId = get_script_number(NULL);//npc的memid
	int iRetIdx = get_script_number(NULL);//返回标志

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	ZoneObj *pstZoneObj = stSEnv.pstEnv->pstZoneObj;
	if (!pstZoneObj)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	PAOSHANGINFO *pstPaoShangInfo = &pstZoneObj->stPaoShangInfo;
	int iNpcNum = pstPaoShangInfo->iNpcNum;
	int k;
	PAOSHANGGLOBALDATA *pstPaoShangGlo = NULL;
	PAOSHANGPLAYER *pstPaoShangPl = &stSEnv.pstPlayer->stRoleData.MiscInfo.PaoShang;
	if(NULL == pstPaoShangPl)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	//找出跑商id在全局的结构体
	for(k=0;k<iNpcNum;k++)
	{
		pstPaoShangGlo = &pstPaoShangInfo->stPaoShangGloData[k];
		if(pstPaoShangGlo == NULL)
		{
			continue;
		}
		
		if(pstPaoShangGlo->iPaoShangID == iPaoShaId)
		{
			break;
		}
			
	}

	if(k>=iNpcNum)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	stSEnv.pstPlayer->stOnline.iPaoShangID = iPaoShaId;
	stSEnv.pstPlayer->stOnline.iPaoShangNpcID = iPaoShaNpcId;
	
	player_open_paoshang_ui(stSEnv.pstEnv, stSEnv.pstPlayer,pstPaoShangGlo,pstPaoShangPl);

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_positem_info()
{
	int iPos;
	ITEMDEF *pstItemDef;
	ROLEITEM *pstRoleItem;
	
	int iRet = get_script_number(NULL);
	int iMap = get_script_number(NULL);
	int iX = get_script_number(NULL);
	int iY = get_script_number(NULL);
	
	if (NULL == stSEnv.pstRoleItem || NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	if (0 > package_get_item(stSEnv.pstEnv, stSEnv.pstPlayer, stSEnv.stCsPackageUse.SrcListType, 
							stSEnv.stCsPackageUse.SrcGridIdx, &pstItemDef, &pstRoleItem, &iPos))
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	set_script_variable(iRet, 0);
	set_script_variable(iMap, pstRoleItem->InstVal1);
	set_script_variable(iX, pstRoleItem->InstVal2);
	set_script_variable(iY, pstRoleItem->InstVal3);
	
	return 0;
}

int s_set_positem_info()
{
	int iPos;
	ITEMDEF *pstItemDef;
	ROLEITEM *pstRoleItem;
	
	int iRet = get_script_number(NULL);
	int iMap = get_script_number(NULL);
	int iX = get_script_number(NULL);
	int iY = get_script_number(NULL);

	if (NULL == stSEnv.pstRoleItem || NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	if (0 > package_get_item(stSEnv.pstEnv, stSEnv.pstPlayer, stSEnv.stCsPackageUse.SrcListType, 
							stSEnv.stCsPackageUse.SrcGridIdx, &pstItemDef, &pstRoleItem, &iPos))
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	set_script_variable(iRet, 0);
	pstRoleItem->InstVal1 = iMap;
	pstRoleItem->InstVal2 = iX;
	pstRoleItem->InstVal3 = iY;
	
	return 0;
}

int s_positem_dir()
{
	int iPos;
	ITEMDEF *pstItemDef;
	ROLEITEM *pstRoleItem;
	
	RESPOS ItemPos; 
	int iRadius = get_script_number(NULL);
	int iRet = get_script_number(NULL);
	int iDirection = get_script_number(NULL);
	int iPointDist;
	int iDir;

	if (NULL == stSEnv.pstRoleItem || NULL == stSEnv.pstPlayer)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	if (0 > package_get_item(stSEnv.pstEnv, stSEnv.pstPlayer, stSEnv.stCsPackageUse.SrcListType, 
							stSEnv.stCsPackageUse.SrcGridIdx, &pstItemDef, &pstRoleItem, &iPos))
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	if (stSEnv.pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, 0);
	}

	if (0 == is_map_same_refer(stSEnv.pstEnv, pstRoleItem->InstVal1, stSEnv.pstPlayer->stRoleData.Map))
	{
		set_script_variable(iRet, 1);
		return 0;
	}

	ItemPos.X = pstRoleItem->InstVal2;
	ItemPos.Y = pstRoleItem->InstVal3;
	iPointDist = z_distance(&stSEnv.pstPlayer->stRoleData.Pos,&ItemPos);
	// 在规定的范围内
	if( iPointDist <= iRadius )
	{
		set_script_variable(iRet, 0);
		return 0;
	}
	else if(iPointDist < SCREEN_XGRID)
	{
		// 2一屏内
		set_script_variable(iRet, 2);
	}
	else if(iPointDist < 2*SCREEN_XGRID)
	{
		// 3一屏~二屏
		set_script_variable(iRet, 3);
	}
	else
	{
		// 4二屏以上
		set_script_variable(iRet, 4);
	}
	
	// 来判断下距离 
	iDir	=	FaceAngleBetween(ItemPos.X,  ItemPos.Y,
					stSEnv.pstPlayer->stRoleData.Pos.X, stSEnv.pstPlayer->stRoleData.Pos.Y);
	/* 弄出八个方向来 */
	if( iDir < 67.5 &&  iDir >= 22.5 )
	{
		iDir = 8;//西北
	}
	else if( iDir <112.5 &&  iDir >= 67.5 )
	{
		iDir = 1;//北
	}
	else if( iDir <157.5 &&  iDir >= 102.5 )
	{
		iDir = 2;//东北
	}
	else if( iDir <202.5 &&  iDir >= 157.5 )
	{
		iDir = 3;//东
	}
	else if( iDir <247.5 &&  iDir >= 202.5 )
	{
		iDir = 4;//东南
	}
	else if( iDir <292.5 &&  iDir >= 247.5 )
	{
		iDir = 5;//南
	}
	else if( iDir <337.5 &&  iDir >= 292.5 )
	{
		iDir = 6;//西南
	}
	else 
	{
		iDir = 7;//西
	}
	
	set_script_variable(iDirection, iDir);
	
	return 0;
}


int s_paoshang_duihuan_refresh()
{
	int iDuiHuan = get_script_number(NULL);//跑商兑换金额时，刷新界面   1表示刷新  0表示不刷新
	int iRetIdx = get_script_number(NULL);//返回标志

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	if(1 == iDuiHuan)
	{
		player_paoshang_login_check(stSEnv.pstEnv, stSEnv.pstPlayer);
		set_script_variable(iRetIdx, 0);
	}
	return 0;
}


int s_daqiangdao_fenshu()
{
	int iFenShu = get_script_number(NULL);//分数
	int iRetIdx = get_script_number(NULL);//返回标志

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	if(0 >= iFenShu)
	{
		iFenShu = 0;
	}

	player_daqiangdao_fenshu(stSEnv.pstEnv,stSEnv.pstPlayer,iFenShu);
	set_script_variable(iRetIdx, 0);
	return 0;
}

//lzk  设置对话框形式老状态(目前是为了在提交对话框后可以改变为其他dialog)
int  s_npc_set_dialog_old_stat()
{
	int iType;
	iType = get_script_number(NULL);
	
	if (!stSEnv.pstPlayer)
	{
		return -1;
	}

	switch (iType) {
		case 0:
			stSEnv.pstPlayer->stOnline.cDialogStatOld  = DIALOG_STAT_BEGIN;
			break;
		case 1:
			stSEnv.pstPlayer->stOnline.cDialogStatOld  = DIALOG_STAT_ANSWER;
			break;
		case 2:
			stSEnv.pstPlayer->stOnline.cDialogStatOld  = DIALOG_STAT_NEXT;
			break;
		case 3:
			stSEnv.pstPlayer->stOnline.cDialogStatOld = DIALOG_STAT_OPT;
			break;
		case 4:
			stSEnv.pstPlayer->stOnline.cDialogStatOld = DIALOG_STAT_COMMIT_OPT;
			break;
		default:
			return -1;
	}
	return 0;
}

//冠军信息界面 
int s_player_dianfeng_first_info()
{
	int iRetIdx = get_script_number(NULL);//返回标志

	player_first_or_effigy_info_to_clt(stSEnv.pstEnv, stSEnv.pstPlayer,FIRST_INFO);

	set_script_variable(iRetIdx, 0);
	return 0;
}

//末世巅峰领奖
int s_player_dianfeng_award()
{
	int iRetIdx = get_script_number(NULL);//返回标志
	int iFlag = get_script_number(NULL);//1表示可以领冠军奖励，2平常奖励，3表示已经领过，4不行

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	DIANFENGINFO *pstDianFengInfo = &stSEnv.pstPlayer->stRoleData.MiscInfo.DianFengInfo;
	DIANFENGGLOBAL *pstDianFengGlobal = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;
	 
	if(pstDianFengInfo->JoinTime != 0 && pstDianFengInfo->JoinTime != pstDianFengGlobal->ActiveTimeS)
	{
		pstDianFengInfo->DianFengFlag = 0;
		pstDianFengInfo->LoginOutNum = 0;
		pstDianFengInfo->PkOutNum = 0;
		pstDianFengInfo->JoinTime  = pstDianFengGlobal->ActiveTimeS;
	}

	if(!(pstDianFengInfo->DianFengFlag & IF_DIANFENG_ONE_PK))
	{
		set_script_variable(iFlag, 4);
		set_script_variable(iRetIdx, 1);
		return 0;
	}

	if(pstDianFengInfo->DianFengFlag & IF_DIANFENG_GET_AWARD)
	{
		set_script_variable(iFlag, 3);
		set_script_variable(iRetIdx, 1);
		return 0;
	}

	//是不是冠军
	if(1 == player_dianfeng_if_chamption(stSEnv.pstEnv,pstDianFengGlobal,stSEnv.pstPlayer))
	{
		set_script_variable(iFlag, 1);
		dianfeng_give_award(stSEnv.pstEnv,1,stSEnv.pstPlayer);
	}
	else
	{
		set_script_variable(iFlag, 2);
		dianfeng_give_award(stSEnv.pstEnv,2,stSEnv.pstPlayer);
	}

	pstDianFengInfo->DianFengFlag |= IF_DIANFENG_GET_AWARD;

	set_script_variable(iRetIdx, 1);
	return 0;
}

int s_player_dianfeng_front_champion()
{
	int iRetIdx = get_script_number(NULL);//返回标志
	int iFlag = get_script_number(NULL);//可以雕塑不，1表示可以，其他不行

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	DIANFENGGLOBAL *pstDianFengGlobal = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;

	//是不是冠军
	if(1 == player_dianfeng_if_chamption(stSEnv.pstEnv,pstDianFengGlobal,stSEnv.pstPlayer))
	{
		set_script_variable(iFlag, 1);
	}
	else
	{
		set_script_variable(iFlag, -1);
	}
	
	set_script_variable(iRetIdx, 1);
	
	return 0;
}

//设置雕像
int s_player_dianfeng_set_effigy()
{
	int iRetIdx = get_script_number(NULL);//返回标志
	int iMap = get_script_number(NULL);//地图id
	int iPosX = get_script_number(NULL);//x坐标
	int iPosY = get_script_number(NULL);//y坐标
	int iCareer = get_script_number(NULL);// 职业

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	RESPOS stPos;
	stPos.X = iPosX;
	stPos.Y = iPosY;
	int whichone = 0;

	switch(iCareer)
	{
		case CAREER_WARRIOR_SINGLEHAND:
			whichone = DIANFENG_JOB_SHZ;
			break;
		case CAREER_WARRIOR_DOUBLEHAND:	
			whichone = DIANFENG_JOB_ZFZ;
			break;
		case CAREER_MAGICOR_SINGLEHAND:
			whichone = DIANFENG_JOB_JNZ;
			break;
		case CAREER_MAGICOR_DOUBLEHAND:	
			whichone = DIANFENG_JOB_LHS;
			break;
		case CAREER_GUNER_SINGLEHAND:
			whichone = DIANFENG_JOB_LSZ;
			break;
		case CAREER_GUNER_DOUBLEHAND:
			whichone = DIANFENG_JOB_HMZ;
			break;
		case CAREER_CHURCH_SINGLEHAND:
			whichone = DIANFENG_JOB_SYZ;
			break;
		case CAREER_CHURCH_DOUBLEHAND:	
			whichone = DIANFENG_JOB_YLS;
			break;	
		default:
			set_script_variable(iRetIdx, -1);
			return 0;
			break;
	}
	
	if(0 > dianfeng_effigy_set(stSEnv.pstEnv, stSEnv.pstPlayer, iMap, &stPos,whichone))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	set_script_variable(iRetIdx, 0);
	
	return 0;
}

//末世巅峰是否开启
int s_dianfeng_if_open()
{
	int iRetIdx = get_script_number(NULL);//返回标志 , 1 表示开启 ，0表示无

	if(stSEnv.pstEnv->pstConf->DianFengFlag == 0 || (is_fini_global(stSEnv.pstEnv) == 0) || get_curr_max_level(stSEnv.pstEnv) < 65)
	{
		set_script_variable(iRetIdx, 0);
		return 0;
	}

	set_script_variable(iRetIdx, 1);
	return 0;
}

//给巅峰副本中所有在线的人加状态
int s_dianfeng_add_status()
{
	// 参数1: statusID ID
	// 参数2: statusLV 等级
	// 参数3: statusTM 时间
	// 参数4: 返回值 0--成功 -1失败
		
	int iStatusID = get_script_number(NULL);
	int iStatusLV = get_script_number(NULL);
	int iStatusTM = get_script_number(NULL);
	int iRet = get_script_number(NULL);

	ZONEPWORLDINST *pstPworldInst = NULL;
	ZONEMAPINST* pstMapInst;
	int i;
	Player *pstPlayer = NULL;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	pstPworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstMapInst->iPworldInstID );
	if (NULL == pstPworldInst)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	if(pstPworldInst->iPlayer <= 0)
	{
		set_script_variable(iRet, -1);
	}

	for (i=0; i<pstPworldInst->iPlayer; i++)
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPworldInst->aiPlayerID[i]);
		if (pstPlayer && !(pstPlayer->stOnline.State & CS_STAT_DEAD))
		{
			z_player_del_status(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, STATUS_ID_AREAPROTECTED, 1 , 0);

			if(iStatusTM == 0)
			{
				z_player_del_status_level(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, iStatusID, iStatusLV, 1 , 0);
			}
			else
			{
				if (z_player_apply_status(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, 
								iStatusID, iStatusLV, iStatusTM, 1, 0) < 0)
				{
					goto error;
				}
				else
				{
					if (stSEnv.iExParaType == EX_PARA_TYPE_ADD_STATUS_DYN)
					{
						ZoneStatus * pstAdd = z_player_find_status(pstPlayer,iStatusID);
						if (pstAdd && iStatusLV == pstAdd->bLevel)
						{
							pstAdd->iDynPara = stSEnv.iExParaData;		
							pstAdd->MiscType |= ZONESTATUS_MISCTYPE_DYN;
						}
					}
				}
			}
		
		}
	}
	
	set_script_variable(iRet, 0);
	return 0;
error:
	set_script_variable(iRet, -1);
	return 0;
}

//从传送门离开末世巅峰准备地图
int s_dianfeng_clear_baoming_info()
{
	int iRetIdx = get_script_number(NULL);//返回标志
	int iFlag = get_script_number(NULL);//返回标识

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	int MemNum;
	int i;
	int iMapIndex;
	RESPOS ResPos;
	int iPiPei = 0;
	DIANFENGPIPEI *pstDianFengPiPei = NULL;
	int k = 0;
	PIPEIINFOS *pstPiPei = NULL;
	int flag = 0;
	int ColorFlag = 0;
	int iEnd = 0;
	ZONEPWORLDINST *pstZonePworldInst = NULL;
	ZONEPWORLD *pstPworld = NULL;
	Player *pstTempPlayer = NULL;

	DIANFENGJOININFO *pstDianFengJoinInfo = NULL;
	DIANFENGGLOBAL *pstDianFengGlobal = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;

	DIANFENGINFO *pstDianFengInfo = NULL;
	pstDianFengInfo = &stSEnv.pstPlayer->stRoleData.MiscInfo.DianFengInfo;
	
	if(0 > dianfeng_get_joininfo_by_career(stSEnv.pstEnv,&pstDianFengJoinInfo,pstDianFengGlobal,stSEnv.pstPlayer->stRoleData.Career))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	MemNum = pstDianFengJoinInfo->MemNum;
	if(MemNum <= 0)
	{
		flag = 1;
	}
	
	for(i=MemNum-1;i>=0;i--)
	{
		if(0 == strcmp(pstDianFengJoinInfo->GroupInfo[i].RoleName,stSEnv.pstPlayer->stRoleData.RoleName))
		{
			//从报名队列去除
			pstDianFengJoinInfo->MemNum--;
			if(i != pstDianFengJoinInfo->MemNum)
			{
				memcpy(&pstDianFengJoinInfo->GroupInfo[i],&pstDianFengJoinInfo->GroupInfo[pstDianFengJoinInfo->MemNum],sizeof(pstDianFengJoinInfo->GroupInfo[i]));
			}
	
			goto end;
		}
	}

	//不在报名队列中
	if(i<0)
	{		
		flag = 1;
	}

	if(flag == 1)
	{
		//找匹配的
		pstDianFengPiPei = &pstDianFengJoinInfo->DianFengPiPei;
		if(pstDianFengPiPei == NULL)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
		iPiPei = pstDianFengPiPei->PiPeiNum;
		if(iPiPei <= 0)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
		
		for(k=iPiPei-1;k>=0;k--)
		{
			pstPiPei = &pstDianFengPiPei->PiPeiInfos[k];
			if(pstPiPei == NULL)
			{
				continue;
			}
			
			if(0 == strcmp(pstPiPei->RedRoleName,stSEnv.pstPlayer->stRoleData.RoleName))
			{
				ColorFlag = 1;
			}
		
			if(0 == strcmp(pstPiPei->BuleRoleName,stSEnv.pstPlayer->stRoleData.RoleName))
			{
				ColorFlag = 2;
			}

			if(ColorFlag == 1 || ColorFlag == 2)
			{
				//先找出来
				pstZonePworldInst = z_find_pworld_inst(stSEnv.pstEnv->pstMapObj, pstPiPei->PworldInstID);
				if(pstZonePworldInst != NULL)
				{
					pstPworld = z_find_pworld(stSEnv.pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
				}
			
				if(ColorFlag == 1)
				{
					pstTempPlayer = z_name_player(stSEnv.pstEnv->pstAppCtx,stSEnv.pstEnv,pstPiPei->BuleRoleName);
					//蓝方再添加到报名队列,并把匹配队列该双人去除
					player_career_pipei_del_baoming_add(stSEnv.pstEnv,pstPiPei->BuleRoleName,k,stSEnv.pstPlayer->stRoleData.Career);
				}
				else
				{
					pstTempPlayer = z_name_player(stSEnv.pstEnv->pstAppCtx,stSEnv.pstEnv,pstPiPei->RedRoleName);
					//红方再添加到报名队列,并把匹配队列该双人去除
					player_career_pipei_del_baoming_add(stSEnv.pstEnv,pstPiPei->RedRoleName,k,stSEnv.pstPlayer->stRoleData.Career);
				}
								
				if(NULL != pstTempPlayer)
				{
					if(0 > dianfeng_is_first_msg(stSEnv.pstEnv,pstTempPlayer->stRoleData.Career,pstDianFengGlobal))
					{
						z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstTempPlayer, SYS_MIDDLE2 , ZONEERR_DIANFENG34);
					}
					else
					{
						player_status_clean_all(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstTempPlayer);
						//为冠军时，还原状态
						dianfeng_restore_status(stSEnv.pstEnv,pstPworld,pstTempPlayer);
					}
				}

				if (NULL != pstZonePworldInst && NULL != pstPworld)
				{
					iEnd = 1;
				}
					
				goto end;
			}
		}

		if(k<0)
		{
			goto end;
		}
	}
	
end:
	if(pstDianFengInfo->MapId == 0)
	{
		if(iEnd == 1)
		{
			z_in_destroy_pworld_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstZonePworldInst);
		}
		
		set_script_variable(iFlag, 0);
		set_script_variable(iRetIdx, 0);
	}
	else
	{
		iMapIndex = pstDianFengInfo->MapId;
		ResPos.X = pstDianFengInfo->ResPos.X;
		ResPos.Y = pstDianFengInfo->ResPos.Y;

		//踢出去最开始的地方
		z_dianfeng_change_map(stSEnv.pstEnv, stSEnv.pstPlayer,iMapIndex, ResPos ,0);

		if(iEnd == 1)
		{
			z_in_destroy_pworld_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstZonePworldInst);
		}
		
		set_script_variable(iFlag, 1);
		set_script_variable(iRetIdx, 0);
	}

	pstDianFengInfo->DianFengFlag &= ~IF_DIANFENG_BAOMING;
	
	return 0;
}

int s_player_dianfeng_award_check()
{
	int iRetIdx = get_script_number(NULL);//返回标志
	int iFlag = get_script_number(NULL);//1表示可以领冠军奖励，2平常奖励，3表示已经领过，4不能领取

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	DIANFENGINFO *pstDianFengInfo = &stSEnv.pstPlayer->stRoleData.MiscInfo.DianFengInfo;
	DIANFENGGLOBAL *pstDianFengGlobal = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;
	 
	if(pstDianFengInfo->JoinTime != 0 && pstDianFengInfo->JoinTime != pstDianFengGlobal->ActiveTimeS)
	{
		pstDianFengInfo->DianFengFlag = 0;
		pstDianFengInfo->LoginOutNum = 0;
		pstDianFengInfo->PkOutNum = 0;
		pstDianFengInfo->JoinTime  = pstDianFengGlobal->ActiveTimeS;
	}

	if(!(pstDianFengInfo->DianFengFlag & IF_DIANFENG_ONE_PK))
	{
		set_script_variable(iFlag, 4);
		set_script_variable(iRetIdx, 1);
		return 0;
	}

	if(pstDianFengInfo->DianFengFlag & IF_DIANFENG_GET_AWARD)
	{
		set_script_variable(iFlag, 3);
		set_script_variable(iRetIdx, 1);
		return 0;
	}

	//是不是冠军
	if(1 == player_dianfeng_if_chamption(stSEnv.pstEnv,pstDianFengGlobal,stSEnv.pstPlayer))
	{
		set_script_variable(iFlag, 1);
	}
	else
	{
		set_script_variable(iFlag, 2);
	}
	
	set_script_variable(iRetIdx, 1);

	return 0;
}

//得到玩家在迷之塔的层数
int s_get_mzt_layer()
{
	int iRetIdx = get_script_number(NULL);//返回标志
	int iLayer = get_script_number(NULL);

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	if(stSEnv.pstPlayer->stRoleData.MiscInfo.MztInfo.Layer <= 0)
	{
		set_script_variable(iLayer,0);
		set_script_variable(iRetIdx, 0);
		return 0;
	}
	
	set_script_variable(iLayer, stSEnv.pstPlayer->stRoleData.MiscInfo.MztInfo.Layer);
	set_script_variable(iRetIdx, 0);
	
	return 0;
}

//主要功能是给玩家减血，不够血时，会死亡。加血也行
int s_sub_player_hp()
{
	int iOldHP = 0;
	int iOldMP = 0;
	int iPlayerID = get_script_number(NULL);
	int nValue = get_script_number(NULL);//正的表示加多少血，负的表示减多少血
	int iRetIdx = get_script_number(NULL);

	Player *pstPlayer = NULL;
	if(iPlayerID == 0)
	{
		pstPlayer = stSEnv.pstPlayer;
	}
	else
	{
		pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,
		                        iPlayerID);
	}
	
	if(!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	iOldHP = pstPlayer->stRoleData.RoleDetail.HPCurr;
	iOldMP = pstPlayer->stRoleData.RoleDetail.MPCurr;

	PlayerChagAttrVal stAttr;

	stAttr.AttrNum = 1;
	stAttr.AttrList[0].AttrID = ATTR_ID_HP;

	if(nValue < 0)
	{
		//减血
		stAttr.AttrList[0].nFlag = 1;
		stAttr.AttrList[0].Value.HP = -nValue;
	}
	else
	{
		//加血
		stAttr.AttrList[0].nFlag = 0;
		stAttr.AttrList[0].Value.HP = nValue;
	}
	
 	z_attr_chg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, &stAttr, 0);

	player_hp_action(stSEnv.pstEnv, pstPlayer, iOldHP, iOldMP, 0);

	if(pstPlayer->stRoleData.RoleDetail.HPCurr <= 0)
	{
		//目前只给采集怪物时，扣血扣到死亡时用
		ZoneAni  stAni;
		memset(&stAni,0,sizeof(stAni));

		stAni.iType = OBJ_UNKNOW;

		Monster *pstMonster = NULL;

		if (stSEnv.pstMonster)
		{
			pstMonster = stSEnv.pstMonster;
		}
		else if (stSEnv.pstNpc)
		{
			pstMonster = &stSEnv.pstNpc->stMon;
		}
		else
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		if(pstMonster)
		{
			//目前是采集怪用,为了获取复活框上显示被什么所杀的用
			stAni.iType = OBJ_MONSTER;
			stAni.stObj.pstMon = pstMonster;
		}
	
		player_die(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, &stAni, 1);
	}
	
	set_script_variable(iRetIdx, 0);
	return 0;
}

//订制婚礼套餐的限制检查
int s_order_wedding_meal_check()
{
	int iRetIdx = get_script_number(NULL);//返回1表示可以订制，-1表示不行

	Player *pstPlayer = stSEnv.pstPlayer;
	if(!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	ROLEBUDDY *pstBuddy = &pstPlayer->stRoleData.Buddy;
	Player *pstOther = NULL;
	int ret;
	int ret2;
	RESPOS stPos;
	WEDDINGINFO *pstWeddingInfoA = NULL;
	WEDDINGINFO *pstWeddingInfoB = NULL;
	
	ret2 = find_spouse_from_friend_list(pstBuddy);
	if(ret2 >= 0)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING31);
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	ret = find_lover_from_friend_list(pstBuddy);
	if(ret < 0)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING11);
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if(pstBuddy->Friends[ret].BuddyVal < TO_SPOUSE_INTIMATE_VAL)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING12,TO_SPOUSE_INTIMATE_VAL);
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstOther = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstBuddy->Friends[ret].RoleName);
	if (pstOther == NULL)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING13,pstBuddy->Friends[ret].RoleName);
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	//神父位置
	stPos.X = PRIEST_POS_X;
	stPos.Y = PRIEST_POS_Y;
	
	if(pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, 0);
	}
	
	if(pstOther->stOnline.cMoving)
	{
		z_player_move_curr(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstOther, 0);
	}

	if(z_distance(&stPos, &pstOther->stRoleData.Pos) > 1500)
	{
		z_send_sysmsg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING14,pstOther->stRoleData.RoleName);
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if(z_distance(&stPos, &pstPlayer->stRoleData.Pos) > 1500)
	{
		z_send_sysmsg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING14,stSEnv.pstPlayer->stRoleData.RoleName);
		set_script_variable(iRetIdx, -1);
		return -1;
	}
	
	pstWeddingInfoA = &pstPlayer->stRoleData.MiscInfo.WeddingInfo;
	pstWeddingInfoB = &pstOther->stRoleData.MiscInfo.WeddingInfo;
	//检查双方是否已经订制婚礼套餐了
	if(pstWeddingInfoB->WeddingMeal > 0 || pstWeddingInfoA->WeddingMeal > 0)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING15);
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	set_script_variable(iRetIdx, 1);
	return 0;
}

//记录婚礼相关的
int s_set_wedding_sth()
{
	int iType = get_script_number(NULL);//记录哪种类型
	int iRecordData = get_script_number(NULL);//记录的数据
	int iRetIdx = get_script_number(NULL);//返回1表示记录成功，-1表示没有
	/*
		iType 等于1的时候 ,  iRecordData 表示选择哪个婚礼套餐
		iType 等于2的时候 ,  记录领取永恒砖戒
		iType 等于3的时候 ,  记录领取浪漫烟花
		iType 等于4的时候 ,  记录婚礼庆典信息于服务器中
		iType 等于5的时候 ,  记录婚礼密游信息于服务器中
		iType 等于6的时候 ,  记录婚礼密游结束标识,iRecordData 为1时表示结束
	*/

	if(iType == 6)
	{
		WEDDINGGLOBAL *pstWeddingGlobal = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.WeddingGlobal;
		if(pstWeddingGlobal->TravelNum > 0)
		{
			//只有一对而已
			pstWeddingGlobal->TravelInfos[pstWeddingGlobal->TravelNum-1].TravelEndFlag = iRecordData;
		}
		
		set_script_variable(iRetIdx, 1);
		return 0;
	}

	Player *pstPlayer;
	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstPlayer = stSEnv.pstPlayer;
	WEDDINGINFO *pstWeddingInfo = &pstPlayer->stRoleData.MiscInfo.WeddingInfo;
	ROLEBUDDY *pstBuddy = &pstPlayer->stRoleData.Buddy;
	
	switch(iType)
	{
		case 1:
			{
				if(iRecordData > 0)
				{
					pstWeddingInfo->WeddingMeal = iRecordData;
				}
			}
			break;
		case 2:
			{
				pstWeddingInfo->WeddingFlag |= IF_GET_RING;
			}
			break;
		case 3:
			{
				pstWeddingInfo->WeddingFlag |= IF_GET_FIREWORKS;
			}
			break;
		case 4:
			{
				if (pstPlayer->stRoleData.Buddy.SpouseNum <= 0 ||
							pstPlayer->stRoleData.Buddy.SpouseNum > MAX_BUDDY_SPOUSE)
				{
					z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING33);
					set_script_variable(iRetIdx, -1);
					return 0;
				}
								
				WEDDINGGLOBAL *pstWeddingGlobal = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.WeddingGlobal;
				CELEBRATIONINFOS *pstCelebrationInfos = NULL;
				pstWeddingGlobal->CelebrationNum = 1;
				pstCelebrationInfos = &pstWeddingGlobal->CelebrationInfos[pstWeddingGlobal->CelebrationNum-1];
				memset(pstCelebrationInfos,0,sizeof(*pstCelebrationInfos));
				
				pstCelebrationInfos->FirstCRoleID = pstPlayer->stRoleData.RoleID;
				STRNCPY(pstCelebrationInfos->FirstCRoleName, pstPlayer->stRoleData.RoleName, sizeof(pstCelebrationInfos->FirstCRoleName));
				
				//没办法，在庆典结束时，需要名字广播
				pstCelebrationInfos->SecondCRoleID = pstBuddy->Spouse[0].RoleID;
				STRNCPY(pstCelebrationInfos->SecondCRoleName, pstBuddy->Spouse[0].RoleName, sizeof(pstCelebrationInfos->SecondCRoleName));
				//婚礼庆典30分钟
				pstCelebrationInfos->CelebrationEndTime = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec + 1800;
				pstCelebrationInfos->Pos.X = FIREWORK_POS_X;
				pstCelebrationInfos->Pos.Y = FIREWORK_POS_Y;
			}
			break;
		case 5:
			{
				if (pstPlayer->stRoleData.Buddy.SpouseNum <= 0 ||
							pstPlayer->stRoleData.Buddy.SpouseNum > MAX_BUDDY_SPOUSE)
				{
					z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING33);
					set_script_variable(iRetIdx, -1);
					return 0;
				}
						
				WEDDINGGLOBAL *pstWeddingGlobal = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.WeddingGlobal;
				TRAVELINFOS *pstTravelInfos = NULL;
				pstWeddingGlobal->TravelNum = 1;
				pstTravelInfos = &pstWeddingGlobal->TravelInfos[0];
				memset(pstTravelInfos,0,sizeof(*pstTravelInfos));
				
				pstTravelInfos->FirstTRoleID = pstPlayer->stRoleData.RoleID;
				STRNCPY(pstTravelInfos->FirstTRoleName, pstPlayer->stRoleData.RoleName, sizeof(pstTravelInfos->FirstTRoleName));
				//没办法，在密游结束时，需要名字广播
				pstTravelInfos->SecondTRoleID = pstPlayer->stRoleData.Buddy.Spouse[0].RoleID;
				STRNCPY(pstTravelInfos->SecondTRoleName, pstPlayer->stRoleData.Buddy.Spouse[0].RoleName, sizeof(pstTravelInfos->SecondTRoleName));
				//最多蜜游半小时
				pstTravelInfos->EndTime = stSEnv.pstEnv->pstAppCtx->stCurr.tv_sec + 1800;
			}
			break;
		default:
			set_script_variable(iRetIdx, -1);
			return 0;
			break;
	}
	
	set_script_variable(iRetIdx, 1);
	return 0;
}

//获取相关的结婚系统信息
int s_get_wedding_sth()
{
	int iType = get_script_number(NULL);//获取哪种类型
	int iGetData = get_script_number(NULL);//获取的数据
	int iRetIdx = get_script_number(NULL);//返回1表示获取成功，-1表示没有
	/*
		iType 等于1的时候 iGetData 表示选择哪个婚礼套餐
		iType 等于2的时候 表示判断是否领取了永恒砖戒，iGetData 1表示有，0表示没
		iType 等于3的时候 表示判断是否领取了浪漫烟花，iGetData 1表示有，0表示没
		iType 等于4的时候 表示是否可以开始领取永恒砖戒，iGetData 1表示可以，0表示不可以
		iType 等于5的时候 表示是否可以开始领取浪漫烟花，iGetData 1表示可以，0表示不可以
		iType 等于6的时候 表示是否有伴侣，iGetData 1表示有，0表示没有
		iType 等于7的时候 表示是否有情侣，iGetData 1表示有，0表示没有
		iType 等于8的时候 表示是否举行了婚礼庆典了，iGetData 1表示有，0表示没有
		iType 等于9的时候 表示是否举行了婚礼蜜游了，iGetData 1表示有，0表示没有
		iType 等于10的时候 表示判断夫妻双方是否都领取了永恒砖戒，iGetData 1表示有，0表示没
	*/

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	
	
	WEDDINGINFO *pstWeddingInfo = NULL;
	pstWeddingInfo = &stSEnv.pstPlayer->stRoleData.MiscInfo.WeddingInfo;
	int flag = 0;
	int ret;

	switch(iType)
	{
		case 1:
			{
				flag = pstWeddingInfo->WeddingMeal;
			}
			break;
		case 2:
			{
				if(pstWeddingInfo->WeddingFlag & IF_GET_RING)
				{
					flag = 1;
				}
			}
			break;
		case 3:
			{
				if(pstWeddingInfo->WeddingFlag & IF_GET_FIREWORKS)
				{
					flag = 1;
				}
			}
			break;
		case 4:
			{
				if(pstWeddingInfo->WeddingFlag & IF_CAN_HAVE_RING)
				{
					flag = 1;
				}
			}
			break;
		case 5:
			{
				if(pstWeddingInfo->WeddingFlag & IF_CAN_HAVE_FIREWORKS)
				{
					flag = 1;
				}
			}
			break;
		case 6:
			{
				ret = find_spouse_from_friend_list(&stSEnv.pstPlayer->stRoleData.Buddy);
				if(ret >= 0)
				{
					flag = 1;
				}
			}
			break;
		case 7:
			{
				ret = find_lover_from_friend_list(&stSEnv.pstPlayer->stRoleData.Buddy);
				if(ret >= 0)
				{
					flag = 1;
				}
			}
			break;
		case 8:
			{
				if(pstWeddingInfo->WeddingFlag & IF_START_CELEBRATION)
				{
					flag = 1;
				}
			}
			break;
		case 9:
			{
				if(pstWeddingInfo->WeddingFlag & IF_START_HONEYMOON_TRAVEL)
				{
					flag = 1;
				}
			}
			break;
		case 10:
			{
				ret = find_spouse_from_friend_list(&stSEnv.pstPlayer->stRoleData.Buddy);
				if(ret < 0)
				{
					set_script_variable(iRetIdx, -1);
					return 0;
				}

				Player *pstOther = NULL;
				pstOther = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer->stRoleData.Buddy.Friends[ret].RoleName);
				if (pstOther == NULL)
				{
					set_script_variable(iRetIdx, -1);
					return 0;
				}
	
				WEDDINGINFO *pstWeddingInfo2 = NULL;
				pstWeddingInfo2 = &pstOther->stRoleData.MiscInfo.WeddingInfo;
			
				if((pstWeddingInfo->WeddingFlag & IF_GET_RING) && (pstWeddingInfo2->WeddingFlag & IF_GET_RING))
				{
					flag = 1;
				}
			}
			break;
		default:
			set_script_variable(iRetIdx, -1);
			return 0;
			break;
	}
	
	set_script_variable(iGetData, flag);
	set_script_variable(iRetIdx, 1);
	return 0;
}

//开始婚礼限制检查
int s_begin_wedding_check()
{
	int iRetIdx = get_script_number(NULL);//返回1表示可以开始婚礼，-1表示不行

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	ROLEBUDDY *pstBuddy = &stSEnv.pstPlayer->stRoleData.Buddy;
	Player *pstOther = NULL;
	int ret;
	RESPOS stPos;
	WEDDINGINFO *pstWeddingInfo1 = NULL;
	WEDDINGINFO *pstWeddingInfo2 = NULL;
	int iMeal = 0;
	int flag = 0;
	int iItemID = 0;
	ITEMDEF *pstItemDef = NULL;
	int num = 0;
	ROLEWEAR *pstRoleWear = NULL;
	int i;
	ITEMDEF *pstRingItemDef=NULL;
	ROLEWEAR *pstRoleWear2 = NULL;
	ITEMDEF *pstItemDef2 = NULL;

	ret = find_spouse_from_friend_list(&stSEnv.pstPlayer->stRoleData.Buddy);
	//检查婚礼是否已经开始了
	if(ret >= 0)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING21);
		set_script_variable(iRetIdx, -1);
		return 0;
	}
		
	ret = find_lover_from_friend_list(pstBuddy);
	if(ret < 0)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING17);
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstOther = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstBuddy->Friends[ret].RoleName);
	if (pstOther == NULL)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING19,pstBuddy->Friends[ret].RoleName);
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstWeddingInfo1 = &stSEnv.pstPlayer->stRoleData.MiscInfo.WeddingInfo;
	pstWeddingInfo2 = &pstOther->stRoleData.MiscInfo.WeddingInfo;
	if(pstWeddingInfo1->WeddingMeal > 0)
	{
		iMeal = pstWeddingInfo1->WeddingMeal;
		flag = 1;
	}
	else
	{
		if(pstWeddingInfo2->WeddingMeal > 0)
		{
			iMeal = pstWeddingInfo2->WeddingMeal;
			flag = 2;
		}
		else
		{
			//没有订制婚礼套餐就别想开始婚礼
			z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING32);
			set_script_variable(iRetIdx, -1);
			return 0;
		}	
	}
	
	iItemID = get_wedding_item_id_by_meal(iMeal);

	pstItemDef = z_find_itemdef(stSEnv.pstEnv, iItemID);
	if(!pstItemDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if(flag == 1)
	{
		num = z_get_pak_item_num(stSEnv.pstEnv, &stSEnv.pstPlayer->stRoleData.Package, iItemID, -2,0,PACKAGE_DEC_NONE);
		if(num <= 0)
		{
			z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING65,stSEnv.pstPlayer->stRoleData.RoleName,pstItemDef->Name);
			set_script_variable(iRetIdx, -1);
			return 0;
		}
	}
	else
	{
		num = z_get_pak_item_num(stSEnv.pstEnv, &pstOther->stRoleData.Package, iItemID, -2,0,PACKAGE_DEC_NONE);
		if(num <= 0)
		{
			z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING65,pstOther->stRoleData.RoleName,pstItemDef->Name);
			set_script_variable(iRetIdx, -1);
			return 0;
		}
	}
	
	if(pstBuddy->Friends[ret].BuddyVal < TO_SPOUSE_INTIMATE_VAL)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING18,TO_SPOUSE_INTIMATE_VAL);
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	//神父位置
	stPos.X = PRIEST_POS_X;
	stPos.Y = PRIEST_POS_Y;
	
	if(stSEnv.pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, 0);
	}
	
	if(pstOther->stOnline.cMoving)
	{
		z_player_move_curr(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstOther, 0);
	}

	if(z_distance(&stPos, &pstOther->stRoleData.Pos) > 1500)
	{
		z_send_sysmsg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING20,pstOther->stRoleData.RoleName);
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if(z_distance(&stPos, &stSEnv.pstPlayer->stRoleData.Pos) > 1500)
	{
		z_send_sysmsg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING20,stSEnv.pstPlayer->stRoleData.RoleName);
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	pstItemDef2 = z_find_itemdef(stSEnv.pstEnv, PURSUE_RING_ITEM_ID);
	if(!pstItemDef2)
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}
	
	pstRoleWear = &stSEnv.pstPlayer->stRoleData.Wear;
	for (i=0; i<pstRoleWear->ArmNum; i++)
	{
		pstRingItemDef = z_get_itemdef_roleitem(stSEnv.pstEnv, (ROLEITEM *)&pstRoleWear->RoleArms[i]);
		if (pstRingItemDef)
		{
			if(pstRingItemDef->ItemID == PURSUE_RING_ITEM_ID)
			{
				z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING84,stSEnv.pstPlayer->stRoleData.RoleName,pstItemDef2->Name);
				z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstOther, SYS_MIDDLE, ZONEERR_WEDDING84,stSEnv.pstPlayer->stRoleData.RoleName,pstItemDef2->Name);
				set_script_variable(iRetIdx, -1);
				return -1;
			}
		}
	}

	pstRoleWear2 = &pstOther->stRoleData.Wear;
	for (i=0; i<pstRoleWear2->ArmNum; i++)
	{
		pstRingItemDef = z_get_itemdef_roleitem(stSEnv.pstEnv, (ROLEITEM *)&pstRoleWear2->RoleArms[i]);
		if (pstRingItemDef)
		{
			if(pstRingItemDef->ItemID == PURSUE_RING_ITEM_ID)
			{
				z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING84,pstOther->stRoleData.RoleName,pstItemDef2->Name);
				z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstOther, SYS_MIDDLE, ZONEERR_WEDDING84,pstOther->stRoleData.RoleName,pstItemDef2->Name);
				set_script_variable(iRetIdx, -1);
				return -1;
			}
		}
	}
	
	set_script_variable(iRetIdx, 1);
	return 0;
}

// 刷一个巡逻机械，夫妻上车
int s_new_machine_patrol_spouse()
{
	Monster *pstMachine;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	Player *pstPlayer;
	RESPOS stPos;
	int iMonID = get_script_number(NULL);
	int iPatrolID = get_script_number(NULL);
	stPos.X = get_script_number(NULL);
	stPos.Y = get_script_number(NULL);
	int iRetIdx =  get_script_number(NULL);
	Player *pstSpouse;
	
	if (!stSEnv.pstPlayer)
	{
		goto error;
	}

	pstPlayer = stSEnv.pstPlayer;

	if (	pstPlayer->stRoleData.Buddy.SpouseNum <= 0 ||
		pstPlayer->stRoleData.Buddy.SpouseNum > MAX_BUDDY_SPOUSE)
	{
		goto error;
	}

	pstSpouse = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer->stRoleData.Buddy.Spouse[0].RoleName);
	if (!pstSpouse)
	{
		goto error;
	}
	
	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		goto error;
	}

	pstMachine = new_mon_machine_dir_patrol(stSEnv.pstEnv, pstMapInst, iMonID, &stPos, 
											MACHINE_OWNER_SPOUSE, pstPlayer, RAND1(180), 0,0, 0,iPatrolID );
	if (!pstMachine)
	{
		goto error;
	}
	
	z_ride_on_machine(stSEnv.pstEnv, pstPlayer, pstMachine, machine_get_free_seatidx(stSEnv.pstEnv,pstMachine), 0);
	z_ride_on_machine(stSEnv.pstEnv, pstSpouse, pstMachine, machine_get_free_seatidx(stSEnv.pstEnv,pstMachine), 0);
	mon_ai_move( stSEnv.pstEnv,  pstMachine);

	set_script_variable(iRetIdx, 0);
	return 0;
error:

	set_script_variable(iRetIdx, -1);
	return 0;
}

int s_machine_spouse_chg_map()
{
	int iMap = get_script_number(NULL);
	RESPOS stPos;
	stPos.X = get_script_number(NULL);
	stPos.Y  = get_script_number(NULL);
	int iPatrolID = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);
	Player *pstPlayer;
	int i;
	Player *pstTmpPlayer;

	if (!stSEnv.pstMonster || !is_mon_machine(stSEnv.pstMonster) || stSEnv.pstMonster->iPatrolID <= 0)
	{
		goto error;
	}

	pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, 
							stSEnv.pstMonster->stCommUnion.stMachine.stMachineInfo.PassengerID[0]);
	if (pstPlayer && pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
	{
		player_goto( stSEnv.pstEnv, pstPlayer, iMap, stPos.X, stPos.Y,pstPlayer->stOnline.iMapPos);
	}
	else
	{
		for(i=1; i<MAX_MON_MACHINE_PASSENGER; i++)
		{
			if(i >= this_machine(stSEnv.pstEnv,stSEnv.pstMonster)->stMachineInfo.MaxPassenger)
			{
				break;
			}

			if(this_machine(stSEnv.pstEnv,stSEnv.pstMonster)->stMachineInfo.PassengerID[i] <= 0)
			{
				continue;
			}
			
			pstTmpPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, 
						this_machine(stSEnv.pstEnv,stSEnv.pstMonster)->stMachineInfo.PassengerID[i]);
			if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				continue;
			}

			player_goto( stSEnv.pstEnv, pstTmpPlayer, iMap, stPos.X, stPos.Y,pstTmpPlayer->stOnline.iMapPos);
		}

		machine_chg_map(stSEnv.pstEnv, stSEnv.pstMonster, iMap, &stPos);
	}
	
	stSEnv.pstMonster->iPatrolID = iPatrolID;
	stSEnv.pstMonster->cPatrolScript = 0;
	stSEnv.pstMonster->bPatrolPos = 0;
	//machine_passenger_info(stSEnv.pstEnv, stSEnv.pstMonster);
	mon_ai_move( stSEnv.pstEnv,  stSEnv.pstMonster);

	set_script_variable(iRetIdx, 0);
	return 0;

error:

	set_script_variable(iRetIdx, -1);
	return 0;
}

int s_machine_clear()
{
	int iRetIdx = get_script_number(NULL);
	
	if (!stSEnv.pstMonster || !is_mon_machine(stSEnv.pstMonster))
	{
		goto error;
	}

	//清理人员
	z_ride_machine_clear(stSEnv.pstEnv, stSEnv.pstMonster);
		
	//地图上拿掉
	z_monster_leave_map(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstMonster);
	z_free_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstMonster);	

	set_script_variable(iRetIdx, 0);
	return 0;
	
error:
	set_script_variable(iRetIdx, -1);
	return 0;

}

//开始庆典限制检查
int s_begin_wedding_celebration_check()
{
	int iRetIdx = get_script_number(NULL);//返回1表示可以开始婚礼庆典，-1表示不行

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	ROLEBUDDY *pstBuddy = &stSEnv.pstPlayer->stRoleData.Buddy;
	Player *pstOther = NULL;
	int ret;
	RESPOS stPos;
	WEDDINGINFO *pstWeddingInfo1 = NULL;
	WEDDINGINFO *pstWeddingInfo2 = NULL;
	pstWeddingInfo1 = &stSEnv.pstPlayer->stRoleData.MiscInfo.WeddingInfo;
	
	ret = find_spouse_from_friend_list(pstBuddy);
	if(ret < 0)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING23);
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(pstWeddingInfo1->WeddingFlag & IF_START_CELEBRATION)
	{
		//先判断自己的
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING52);
		set_script_variable(iRetIdx, -1);
		return -1;
	}
	
	pstOther = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstBuddy->Friends[ret].RoleName);
	if (pstOther == NULL)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING22,pstBuddy->Friends[ret].RoleName);
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstWeddingInfo2 = &pstOther->stRoleData.MiscInfo.WeddingInfo;
	if(pstWeddingInfo1->WeddingMeal <= 1 && pstWeddingInfo2->WeddingMeal <= 1)
	{
		//需要订制简约婚礼套餐或者豪华婚礼套餐才可以进行婚礼庆典
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING35);
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if(pstWeddingInfo2->WeddingFlag & IF_START_CELEBRATION)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING52);
		set_script_variable(iRetIdx, -1);
		return -1;
	}
	
	//沙滩神父位置
	stPos.X = PRIEST_POS_X;
	stPos.Y = PRIEST_POS_Y;
	
	if(stSEnv.pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, 0);
	}
	
	if(pstOther->stOnline.cMoving)
	{
		z_player_move_curr(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstOther, 0);
	}

	if(z_distance(&stPos, &pstOther->stRoleData.Pos) > 1500)
	{
		z_send_sysmsg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING22,pstOther->stRoleData.RoleName);
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if(z_distance(&stPos, &stSEnv.pstPlayer->stRoleData.Pos) > 1500)
	{
		z_send_sysmsg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING22,stSEnv.pstPlayer->stRoleData.RoleName);
		set_script_variable(iRetIdx, -1);
		return -1;
	}
	
	//服务器只能同时举行一场婚礼庆典否则无法举行
	WEDDINGGLOBAL *pstWeddingGlobal = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.WeddingGlobal;
	if(pstWeddingGlobal->CelebrationNum > 0)
	{
		z_send_sysmsg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING24);
		set_script_variable(iRetIdx, -1);
		return -1;
	}
	
	if(pstWeddingGlobal->TravelNum > 0)
	{
		//双方当前并非举行 婚礼蜜游中，否则无法举行，并广播系统提示
		tdr_ulonglong ullTempRoleID = pstWeddingGlobal->TravelInfos[pstWeddingGlobal->TravelNum-1].FirstTRoleID;
		if( ullTempRoleID == stSEnv.pstPlayer->stRoleData.RoleID || ullTempRoleID == pstOther->stRoleData.RoleID)
		{
			z_send_sysmsg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING25);
			set_script_variable(iRetIdx, -1);
			return -1;
		}
	}
	
	set_script_variable(iRetIdx, 1);
	return 0;
}

//开始婚礼密游限制检查
int s_begin_honeymoon_travel_check()
{
	int iRetIdx = get_script_number(NULL);//返回1表示可以开始婚礼密游，-1表示不行

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	ROLEBUDDY *pstBuddy = &stSEnv.pstPlayer->stRoleData.Buddy;
	Player *pstOther = NULL;
	int ret;
	RESPOS stPos;
	WEDDINGINFO *pstWeddingInfo1 = NULL;
	WEDDINGINFO *pstWeddingInfo2 = NULL;
	pstWeddingInfo1 = &stSEnv.pstPlayer->stRoleData.MiscInfo.WeddingInfo;
	
	ret = find_spouse_from_friend_list(pstBuddy);
	if(ret < 0)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING33);
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(pstWeddingInfo1->WeddingFlag & IF_START_HONEYMOON_TRAVEL)
	{
		//先判断自己的
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING53);
		set_script_variable(iRetIdx, -1);
		return -1;
	}
	
	pstOther = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstBuddy->Friends[ret].RoleName);
	if (pstOther == NULL)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING34,pstBuddy->Friends[ret].RoleName);
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstWeddingInfo2 = &pstOther->stRoleData.MiscInfo.WeddingInfo;
	if(pstWeddingInfo1->WeddingMeal <= 2 && pstWeddingInfo2->WeddingMeal <= 2)
	{
		//需要订制豪华婚礼套餐才可以进行婚礼蜜游
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING36);
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if(pstWeddingInfo2->WeddingFlag & IF_START_HONEYMOON_TRAVEL)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING53);
		set_script_variable(iRetIdx, -1);
		return -1;
	}
	
	//沙滩神父位置
	stPos.X = PRIEST_POS_X;
	stPos.Y = PRIEST_POS_Y;

	if(stSEnv.pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, 0);
	}
	
	if(pstOther->stOnline.cMoving)
	{
		z_player_move_curr(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstOther, 0);
	}
	
	if(z_distance(&stPos, &pstOther->stRoleData.Pos) > 1500)
	{
		z_send_sysmsg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING34,pstOther->stRoleData.RoleName);
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if(z_distance(&stPos, &stSEnv.pstPlayer->stRoleData.Pos) > 1500)
	{
		z_send_sysmsg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING34,stSEnv.pstPlayer->stRoleData.RoleName);
		set_script_variable(iRetIdx, -1);
		return -1;
	}
	
	WEDDINGGLOBAL *pstWeddingGlobal = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.WeddingGlobal;
	//服务器只能同时举行一场婚礼密游
	if(pstWeddingGlobal->TravelNum > 0)
	{
		z_send_sysmsg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING26);
		set_script_variable(iRetIdx, -1);
		return -1;
	}
	
	if(pstWeddingGlobal->CelebrationNum > 0)
	{
		//双方当前并非举行 婚礼庆典中，否则无法举行，并广播系统提示
		tdr_ulonglong ullTempRoleID = pstWeddingGlobal->CelebrationInfos[pstWeddingGlobal->CelebrationNum-1].FirstCRoleID;
		if( ullTempRoleID == stSEnv.pstPlayer->stRoleData.RoleID || ullTempRoleID == pstOther->stRoleData.RoleID)
		{
			z_send_sysmsg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING27);
			set_script_variable(iRetIdx, -1);
			return -1;
		}
	}
	
	set_script_variable(iRetIdx, 1);
	return 0;
}

//点击开始婚礼的用户与其情侣都弹出誓言面板
int s_oath_ui()
{
	int iRetIdx = get_script_number(NULL);

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	ROLEBUDDY *pstBuddy = &stSEnv.pstPlayer->stRoleData.Buddy;
	Player *pstOther = NULL;
	int ret;
	
	ret = find_lover_from_friend_list(pstBuddy);
	if(ret < 0)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING17);
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstOther = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstBuddy->Friends[ret].RoleName);
	if (pstOther == NULL)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING19,pstBuddy->Friends[ret].RoleName);
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	stSEnv.pstPlayer->stOnline.iWillToSpouseStatus = 0;
	pstOther->stOnline.iWillToSpouseStatus = 0;
	relation_involve_marry_svr(stSEnv.pstEnv,stSEnv.pstPlayer,pstOther,TYPE_OATH_UI_SVR);
	relation_involve_marry_svr(stSEnv.pstEnv,pstOther,stSEnv.pstPlayer,TYPE_OATH_UI_SVR);
	
	set_script_variable(iRetIdx, 1);
	return 0;
}

int s_broadcast_wedding_tip()
{
	int iType = get_script_number(NULL);
	int iRetIdx = get_script_number(NULL);

	Player *pstPlayer;
	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	

	pstPlayer = stSEnv.pstPlayer;
	ROLEBUDDY *pstBuddy = &pstPlayer->stRoleData.Buddy;
	Player *pstOther = NULL;
	WEDDINGINFO *pstWeddingInfo1 = NULL;
	WEDDINGINFO *pstWeddingInfo2 = NULL;

	if (pstPlayer->stRoleData.Buddy.SpouseNum <= 0 ||
		pstPlayer->stRoleData.Buddy.SpouseNum > MAX_BUDDY_SPOUSE)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING33);
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstOther = z_name_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstBuddy->Spouse[0].RoleName);
	if (!pstOther)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING8);
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstWeddingInfo1 = &pstPlayer->stRoleData.MiscInfo.WeddingInfo;
	pstWeddingInfo2 = &pstOther->stRoleData.MiscInfo.WeddingInfo;
	
	if(iType == 1)
	{
		z_sys_strf_broadcast(stSEnv.pstEnv, SYS_MIDDLE2, ZONEERR_WEDDING42,pstPlayer->stRoleData.RoleName,pstBuddy->Spouse[0].RoleName);
	}
	else if(iType == 2)
	{
		z_sys_strf_broadcast(stSEnv.pstEnv, SYS_SHOW, ZONEERR_WEDDING43,pstPlayer->stRoleData.RoleName,pstBuddy->Spouse[0].RoleName);
		
		//激活npc神父领取浪漫烟花选项
		pstWeddingInfo1->WeddingFlag |= IF_CAN_HAVE_FIREWORKS;
		pstWeddingInfo2->WeddingFlag |= IF_CAN_HAVE_FIREWORKS;
		// 记录已举行婚礼庆典标识
		pstWeddingInfo1->WeddingFlag |= IF_START_CELEBRATION;
		pstWeddingInfo2->WeddingFlag |= IF_START_CELEBRATION;
	}
	else if(iType == 3)
	{
		z_sys_strf_broadcast(stSEnv.pstEnv, SYS_SHOW, ZONEERR_WEDDING44,pstPlayer->stRoleData.RoleName,pstBuddy->Spouse[0].RoleName);
		// 记录已举行婚礼蜜游标识
		pstWeddingInfo1->WeddingFlag |= IF_START_HONEYMOON_TRAVEL;
		pstWeddingInfo2->WeddingFlag |= IF_START_HONEYMOON_TRAVEL;
	}
	else if(iType == 4)
	{
		//订制婚礼套餐成功，双方都提示
		int iMeal = 0;
		if(pstWeddingInfo1->WeddingMeal > 0)
		{
			iMeal = pstWeddingInfo1->WeddingMeal;
		}
		else
		{
			if(pstWeddingInfo2->WeddingMeal > 0)
			{
				iMeal = pstWeddingInfo2->WeddingMeal;
			}
			else
			{
				set_script_variable(iRetIdx, -1);
				return 0;
			}	
		}

		if(iMeal == 3)
		{
			z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING63,pstPlayer->stRoleData.RoleName);
			z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstOther, SYS_MIDDLE, ZONEERR_WEDDING63,pstPlayer->stRoleData.RoleName);
		}
		else if(iMeal == 2)
		{
			z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING62,pstPlayer->stRoleData.RoleName);
			z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstOther, SYS_MIDDLE, ZONEERR_WEDDING62,pstPlayer->stRoleData.RoleName);
		}
		else
		{
			z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING61,pstPlayer->stRoleData.RoleName);
			z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstOther, SYS_MIDDLE, ZONEERR_WEDDING61,pstPlayer->stRoleData.RoleName);
		}
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	set_script_variable(iRetIdx, 1);
	return 0;
}

int s_remove_relation_ui()
{
	int iRetIdx = get_script_number(NULL);

	if(!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}	
	
	if(0 > relation_involve_marry_svr(stSEnv.pstEnv,stSEnv.pstPlayer,NULL,TYPE_REMOVE_RELATION_UI))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	set_script_variable(iRetIdx, 1);
	return 0;
}

int s_boss_baoxiang_touch_set()
{
	/*iRetIdx 返回1表示设置成功，-1表示失败*/
	Npc *pstNpc = stSEnv.pstNpc;
	int iRetIdx = get_script_number(NULL);
	int res = 0;
	Player *pstPlayer = NULL;

	if (NULL == pstNpc)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	pstPlayer = stSEnv.pstPlayer;
	res  = player_boss_baoxiang_touch_set(stSEnv.pstEnv, pstPlayer,pstNpc->stMon.WID);
	set_script_variable(iRetIdx, res);
	return 0;
}

int s_boss_baoxiang_touch_get()
{
	/*iRetIdx 返回0的话，表示可以摸，-1表示错误,1表示摸过了*/
	/*iType 值为1时表示进行公会成员摸宝箱检查
	* iType 值为2时表示进行世界boss掉落宝箱检查
	*/

	int iRes = 0;
	Player *pstPlayer = NULL;
	ZoneClan *pstZoneClan = NULL;
	CLANMEM *pstMem = NULL;
	int iOffset = 0;
	
	int iRetIdx = get_script_number(NULL);
	int iType = get_script_number(NULL);

	Npc *pstNpc = stSEnv.pstNpc;
	if (NULL == pstNpc)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPlayer = stSEnv.pstPlayer;

	switch(iType)
	{
		case 1: //公会BOSS
		{
			pstZoneClan = player_get_clan(stSEnv.pstEnv, pstPlayer);
			if (!pstZoneClan)
			{
				set_script_variable(iRetIdx, -1);
				return 0;
			}

			iRes = is_in_my_clan_city(stSEnv.pstEnv,pstPlayer);
			if(!iRes)
			{
				set_script_variable(iRetIdx, -1);
				return 0;
			}	

			pstMem = clan_id_mem(pstZoneClan, pstPlayer->stRoleData.RoleID, &iOffset);
			if (!pstMem)
			{
				set_script_variable(iRetIdx, -1);
				return 0;
			}

			if(pstMem->JoinTime >= pstNpc->stMon.tBirthEnd)
			{
				set_script_variable(iRetIdx, -1);
				return 0;
			}
			break;
		}
		default :
		{
			break;
		}		
	}

	iRes = player_boss_baoxiang_if_touch(stSEnv.pstEnv,pstPlayer,pstNpc->stMon.WID);
	set_script_variable(iRetIdx, iRes);	
	
	return 0;
}

int s_bct_baoxiang_if_open()
{
	int iRetIdx = get_script_number(NULL);

	if(stSEnv.pstEnv->pstConf->BCTBaoXiangFlag == 1)
	{
		set_script_variable(iRetIdx, 1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);

	return 0;
}

int s_word_pwd_ui()
{
	int iMemID = get_script_number(NULL);//玩家的id
	int iType = get_script_number(NULL);//类型 , 0表示词语准备中，1表示请答题,2表示停止倒数
	int iNum = get_script_number(NULL);//第几题
	int iDaojishi = get_script_number(NULL);//倒计时,单位s
	int iRetIdx = get_script_number(NULL);//返回标志
	CSWORDPWDUI stWordPwdUI;
	Player *pstPlayer = NULL;
	pstPlayer = z_id_player(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, iMemID);

	if (!pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	memset(&stWordPwdUI,0,sizeof(stWordPwdUI));
	stWordPwdUI.Type = iType;
	stWordPwdUI.DaoJiShi = iDaojishi;
	stWordPwdUI.Num = iNum;

	player_word_pwd_ui(stSEnv.pstEnv,pstPlayer,&stWordPwdUI);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_word_pwd_rand_new_mon()
{
	int iMonID = get_script_number(NULL);//怪物的定义id
	int iNum = get_script_number(NULL);//怪物的数量
	int PosX1 = get_script_number(NULL);//坐标1
	int PosY1 = get_script_number(NULL);//坐标1
	int PosX2 = get_script_number(NULL);//坐标2
	int PosY2 = get_script_number(NULL);//坐标2
	int PosX3 = get_script_number(NULL);//坐标3
	int PosY3 = get_script_number(NULL);//坐标3
	int PosX4 = get_script_number(NULL);//坐标4
	int PosY4 = get_script_number(NULL);//坐标4
	int Radius = get_script_number(NULL);//随机的半径
	int iRetIdx = get_script_number(NULL);//返回标志

	if(iNum > 4 || iNum <= 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	int i = 0;
	int irand = 0;
	int iRandID = 0;
	ZONEMAPINST *pstMapInst; 
	MapIndex *pstMapIdx;
	RESPOS stPos, stCurrPos;  
	Monster stMonster; 
	Monster *pstFind;
	MONSTERDEF *pstMonDef = NULL;
	WORDPWDDEF *pstDef = NULL;
	int aTempRand[4] = {0};
	int iTempNum = 0;
	int j;
	
	pstMapInst = s_get_mapinst();
	if (NULL == pstMapInst)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstMapIdx = z_mappos_index(stSEnv.pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		set_script_variable(iRetIdx, -1); 
		return 0;
	}

	memset(&stMonster, 0, sizeof(stMonster));
	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos =	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.cRefresh	= 0;

	pstMonDef = z_get_mon_def(stSEnv.pstEnv, &stMonster);
	
	if (NULL == pstMonDef || !(pstMonDef->MiscType & MON_MISC_TYPE_WORD_PWD))
	{
		set_script_variable(iRetIdx, -1); 
		return 0;
	}
	
	if(stSEnv.pstEnv->pstZoneObj->iWordPwdDef <= 0)
	{
		set_script_variable(iRetIdx, -1); 
		return 0;
	}	

	iRandID = RAND1(stSEnv.pstEnv->pstZoneObj->iWordPwdDef);
	pstDef = (WORDPWDDEF *)&stSEnv.pstEnv->pstZoneObj->sWordPwdDef[iRandID];
	if(NULL == pstDef)
	{
		set_script_variable(iRetIdx, -1); 
		return 0;
	}
	
	for(i=0;i<iNum;i++)
	{
		irand = RAND1(4);
		irand = irand + 1 ; // 1 ~ 4
		
		for(j=0;j<iTempNum;j++)
		{
			if(irand == aTempRand[j])
			{
				irand = irand + 1;
				if(irand > 4)
				{
					irand = 1;
				}
				j = -1;
			}
		}
		
		//以下是刷怪 ---------------- begin ----------------
		stMonster.bDirection = RAND1(180);

		if(i == 0)
		{
			stPos.X = PosX1;
			stPos.Y = PosY1;
		}
		else if(1 == i)
		{
			stPos.X = PosX2;
			stPos.Y = PosY2;
		}
		else if(2 == i)
		{
			stPos.X = PosX3;
			stPos.Y = PosY3;
		}
		else 
		{
			stPos.X = PosX4;
			stPos.Y = PosY4;
		}
		
		stMonster.stInitPos = stCurrPos = stPos;

		if (Radius > 0)
		{
			z_get_ran_pos(stSEnv.pstEnv, pstMapInst, pstMapIdx, &stMonster.stInitPos, &stMonster.stInitPos, Radius, 0, &stCurrPos);
		}
		
		stMonster.stInitPos = stCurrPos;
		stMonster.stCurrPos = stCurrPos;

		pstFind =	z_new_monster(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, &stMonster, pstMonDef);
		if( !pstFind )
		{
			set_script_variable(iRetIdx, -1); 
			return 0;
		} 

		pstFind->cFirstBorn = 1;
		pstFind->iCommType = MON_UNION_TYPE_WORD_PWD;
		pstFind->stCommUnion.stWordPwd.iLine = pstDef->ID;
		pstFind->stCommUnion.stWordPwd.iIndex = irand;
		if (0 > z_refresh_mon(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, pstFind,NULL))
		{
			tmempool_free(stSEnv.pstEnv->pstMonsterPool, pstFind->iMIndex);
			set_script_variable(iRetIdx, -1);  
			return 0;
		}
		aTempRand[i] = irand;
		iTempNum++;
		pstFind->cFirstBorn = 0; 
		//---------------- end ----------------
	}

	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_word_pwd_mon_info()
{
	Monster *pstMonster;
	
	int iRet = get_script_number(NULL); // 返回标志 0  表示成功
	int iIndex = get_script_number(NULL);//  顺序中的第几个怪

	if (stSEnv.pstMonster == NULL && stSEnv.pstNpc == NULL)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	if (stSEnv.pstMonster)
	{
		pstMonster = stSEnv.pstMonster;
	}
	else if (stSEnv.pstNpc)
	{
		pstMonster = &stSEnv.pstNpc->stMon;
	}
	else
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	if(pstMonster->iCommType != MON_UNION_TYPE_WORD_PWD)
	{
		set_script_variable(iRet, -1);
		return 0;
	}

	set_script_variable(iRet, 0);
	set_script_variable(iIndex, pstMonster->stCommUnion.stWordPwd.iIndex);
	
	return 0;
}

int s_span_map_transmit()
{
	int iMapID;
	RESPOS stPos;
	int iRetIdx;
	int iRet;
	int iMapInstID;
	MapIndex *pstMapIdx;

	iMapID = get_script_number(NULL);
	stPos.X = get_script_number(NULL);
	stPos.Y = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (iMapID >= PWORLD_MAPID_BASE)
	{
		iMapInstID = (stSEnv.pstPlayer->stRoleData.Map & 0xffff0000) | iMapID;
	}
	else
	{
		iMapInstID = iMapID;
	}

	iRet = team_trans_check_one(stSEnv.pstEnv,stSEnv.pstPlayer);
	team_trans_check_tip(stSEnv.pstEnv, stSEnv.pstPlayer,1, iRet, NULL);
	if(0 > iRet)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if(is_battle_pvp_svr(stSEnv.pstEnv))
	{
		//pvp_battle -> zone
		iRet = z_player_change_map(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv, stSEnv.pstPlayer, iMapInstID, &stPos, RAND1(180), 0);

		if(iRet < 0)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
	}
	else
	{
		//zone -> pvp_battle
		pstMapIdx = z_mappos_index( stSEnv.pstEnv->pstMapObj, NULL, iMapInstID);
		if(NULL == pstMapIdx)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		if(0 > player_chg_map_passenger_check( stSEnv.pstEnv,stSEnv.pstPlayer, pstMapIdx))
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}

		if(INST_MODE_BATTLE_SVR == stSEnv.pstEnv->iInstMode)
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
		
		if(0 == player_span_map_check(stSEnv.pstEnv,stSEnv.pstPlayer, pstMapIdx, &stPos))
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
	}
	
	set_script_variable(iRetIdx, 0);

	return 0;
}

//获得自己所在服在跨服野外的阵营索引
int s_get_world_camp_in_span_map()
{
	//参数1	返回标记
	//参数2	阵营索引

	int iRetIdx = get_script_number(NULL);//返回标记
	int iCampIdx = get_script_number(NULL);//阵营索引

	int iCamp = 0;

	if (NULL == stSEnv.pstPlayer || is_fini_global(stSEnv.pstEnv) == 0)
	{
		set_script_variable(iCampIdx, 0);
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	SPANMAPGLOBAL *pstSpanMapGlobal = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc.SpanMapGlobal;
	
	iCamp = pstSpanMapGlobal->iCampIdx;
	set_script_variable(iCampIdx, iCamp);
	set_script_variable(iRetIdx, 0);
	return 0;
}

int s_get_wear_pos1()
{
	ITEMDEF *pstItemDef = NULL;
	int iRetIdx = get_script_number(NULL);
	int iDefId = get_script_number(NULL);  //道具id
	int iWearPos = get_script_number(NULL);//穿戴位置

	pstItemDef = z_find_itemdef(stSEnv.pstEnv, iDefId);
	if(	!pstItemDef)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iWearPos, pstItemDef->WearPos1);
	
	return 0;
}

//判断玩家是否有权能摸取宝箱
int s_get_baoxiang_touch_limit()
{
	/*iRetIdx 返回1的话，表示有权摸，-1表示不能摸*/
	int iRes = 0;
	Player *pstPlayer = NULL;	
	int iRetIdx = get_script_number(NULL);

	Npc *pstNpc = stSEnv.pstNpc;
	if (NULL == pstNpc)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPlayer = stSEnv.pstPlayer;

	iRes = player_baoxiang_if_have_limit_touch(stSEnv.pstEnv,pstPlayer,pstNpc->stMon.FormMonDropWID,pstNpc->stMon.iFromMonDropLevel);
	set_script_variable(iRetIdx, iRes);	
	
	return 0;
}

int s_home_add_exp()
{
	int iRetIdx = get_script_number(NULL);
	int iExp = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	home_farm_add_exp(stSEnv.pstEnv, stSEnv.pstPlayer, iExp);

	set_script_variable(iRetIdx, 0);	
	return 0;
}

int s_home_kaiken()
{
	int iRetIdx = get_script_number(NULL);

	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	home_farm_kaiken(stSEnv.pstEnv, stSEnv.pstPlayer);
	set_script_variable(iRetIdx, 0);	
	return 0;
}

int s_home_open_ui()
{
	int iRetIdx = get_script_number(NULL);
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, home_page(stSEnv.pstEnv, stSEnv.pstPlayer, 0));
	return 0;
}

int s_home_enter()
{
	int iRetIdx = get_script_number(NULL);
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	HOMEENTER stEnter;

	stEnter.Id = stSEnv.pstPlayer->stRoleData.MiscInfo.Home.ID;
	set_script_variable(iRetIdx, home_enter(stSEnv.pstEnv, stSEnv.pstPlayer, &stEnter));
	return 0;
}

int s_home_info()
{
	int iRetIdx = get_script_number(NULL);  //返回的标志
	int iType = get_script_number(NULL);  // 查询的类型
	int iVal = get_script_number(NULL);  //返回的值

	Player *pstPlayer;
	int iNum = 0;
	int iIn=0;
	
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPlayer = stSEnv.pstPlayer;

	home_player_is_in(stSEnv.pstEnv, pstPlayer, &iIn);
	if (iIn != 2)
	{
		z_send_sys_str_msg(stSEnv.pstEnv->pstAppCtx, stSEnv.pstEnv,pstPlayer, 
									SYS_MIDDLE, ZONEERR_HOME_04);
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	switch(iType)
	{
		case 1:
			//菜地等级
			iNum = home_get_soil_level(stSEnv.pstEnv, pstPlayer);
			break;
		case 2:
			//已开垦土地数量
			iNum = home_player_get_kaiken_num(stSEnv.pstEnv, pstPlayer);
			break;
		case 3:
			//下次开垦菜地的钱数量
			iNum = home_player_get_next_kaiken_info(stSEnv.pstEnv, pstPlayer,1);
			break;
		case 4:
			//下次开垦菜地的钱类型
			iNum = home_player_get_next_kaiken_info(stSEnv.pstEnv, pstPlayer,2);
			break;
		case 5:
			//下次开垦菜地的等级
			iNum = home_player_get_next_kaiken_info(stSEnv.pstEnv, pstPlayer,3);
			break;
		default:
			set_script_variable(iRetIdx,-1);
			return 0;
	}

	set_script_variable(iVal,iNum);
	
	if(iNum >= 0)
	{
		set_script_variable(iRetIdx,0);
	}
	else
	{
		set_script_variable(iRetIdx,-1);
	}
	
	return 0;
}

int s_career_alter_check()
{
	int iRetIdx = get_script_number(NULL); //返回0 表示可以转换职业
	
	Player *pstPlayer = NULL;
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPlayer = stSEnv.pstPlayer;
	
	if(0 > player_career_alter_check(stSEnv.pstEnv,pstPlayer))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
		
	return 0;
}

int s_career_alter()
{
	int iRetIdx = get_script_number(NULL);//返回0表示转换成功
	int iCareer = get_script_number(NULL);//转换成什么职业

	Player *pstPlayer = NULL;
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPlayer = stSEnv.pstPlayer;

	if(0 > player_career_alter(stSEnv.pstEnv,pstPlayer,iCareer))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
		
	return 0;
}

int s_get_global_var()
{
	int iIdx;
	int iBit;
	int iRetIdx;
	int iRetValIdx;
	char cFlag;
	int iRet;
	GLOBALMISC *pstMisc = NULL;
	
	iIdx = get_script_number(NULL);
	iBit = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);
	iRetValIdx = get_script_number(NULL);

	if (NULL == stSEnv.pstPlayer || is_fini_global(stSEnv.pstEnv) == 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	if ((iBit>=0 && iBit<=7) || 8==iBit || 16==iBit || 32==iBit)
	{
		
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if( iIdx<0 || iIdx>=MAX_GLOBAL_SCRIPT_VAR || (16==iBit && iIdx>=MAX_GLOBAL_SCRIPT_VAR-1) || (32==iBit && iIdx>=MAX_GLOBAL_SCRIPT_VAR-3))
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}
	
	pstMisc = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc;

	if( 8 == iBit )
	{
		iRet =	pstMisc->GlobalVar[iIdx];
	}
	else if (16 == iBit)
	{
		iRet = *(short*)(pstMisc->GlobalVar+iIdx);
	}
	else if (32 == iBit)
	{
		iRet = *(int*)(pstMisc->GlobalVar+iIdx);
	}
	else
	{
		cFlag	=	1 << iBit;

		if( pstMisc->GlobalVar[iIdx] & cFlag )
			iRet	=	1;
		else
			iRet	=	0;
	}

	set_script_variable(iRetIdx, 0);
	set_script_variable(iRetValIdx, iRet);

	return 0;
}

int s_set_global_var()
{
	int iIdx;
	int iBit;
	int iVal;
	int iRetIdx;
	char cFlag;
	static char sTemp[4];
	GLOBALMISC *pstMisc = NULL;

	iIdx = get_script_number(NULL);
	iBit = get_script_number(NULL);
	iVal = get_script_number(NULL);
	iRetIdx = get_script_number(NULL);

	if (NULL == stSEnv.pstPlayer || is_fini_global(stSEnv.pstEnv) == 0)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	if ((iBit>=0 && iBit<=7) || 8==iBit || 16==iBit || 32==iBit)
	{
		
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	if( iIdx<0 || iIdx>=MAX_GLOBAL_SCRIPT_VAR || (16==iBit && iIdx>=MAX_GLOBAL_SCRIPT_VAR-1) || (32==iBit && iIdx>=MAX_GLOBAL_SCRIPT_VAR-3))
	{
		set_script_variable(iRetIdx, -1);
		return -1;
	}

	memcpy(sTemp, &iVal, sizeof(int));
	pstMisc = &stSEnv.pstEnv->pstGlobalObj->stGlobal.Misc;

	if( 8 == iBit )
	{
		pstMisc->GlobalVar[iIdx]	=	(char) iVal;
	}
	else if (16 == iBit)
	{
		pstMisc->GlobalVar[iIdx]		=	sTemp[0];
		pstMisc->GlobalVar[iIdx + 1]	=	sTemp[1];
	}
	else if (32 == iBit)
	{
		pstMisc->GlobalVar[iIdx]		=	sTemp[0];
		pstMisc->GlobalVar[iIdx + 1]	=	sTemp[1];
		pstMisc->GlobalVar[iIdx + 2]	=	sTemp[2];
		pstMisc->GlobalVar[iIdx + 3]	=	sTemp[3];
	}
	else
	{
		cFlag	=	1 << iBit;

		if( iVal )
			pstMisc->GlobalVar[iIdx]		|=	cFlag;
		else
			pstMisc->GlobalVar[iIdx]		&=	~cFlag;
	}

	set_script_variable(iRetIdx, 0);

	return 0;
	
}

int s_apply_pworld()
{
	int iPworldID = get_script_number(NULL); // 副本id
	int iMode = get_script_number(NULL);  // 0代表单人，1代表组队
	int iRetIdx = get_script_number(NULL); // 返回标志 0 表示成功，-1表示失败

	Player *pstPlayer = NULL;
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPlayer = stSEnv.pstPlayer;

	if(0 > player_apply_pworld_from_npc(stSEnv.pstEnv,pstPlayer,iPworldID,iMode))
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	set_script_variable(iRetIdx, 0);
		
	return 0;
}

//主要是取消跨服战场队列和末世荣耀殿堂队列等
int s_cancel_span_queue()
{
	int iType = get_script_number(NULL); // 取消队列的大类型,1代表战场，2代表末世荣耀殿堂
	int iRetIdx = get_script_number(NULL); // 返回标志 0 表示成功，-1表示失败

	Player *pstPlayer = NULL;
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}

	pstPlayer = stSEnv.pstPlayer;

	if(iType == 1)
	{
		if(pstPlayer->stOnline.stSpanOnline.stApply.iBattleApply == SPAN_APPLY_ING)
		{
			if(INST_MODE_ZONE_SVR == stSEnv.pstEnv->iInstMode &&
				stSEnv.pstEnv->pstConf->BattleFlag)
			{
				player_span_apply_queue_clean(stSEnv.pstEnv, pstPlayer);
			}
			else
			{
				z_exit_pworld_queue(stSEnv.pstEnv, pstPlayer, pstPlayer->stOnline.stPworldQueue.iPworldID, 1);
			}
		}
		else
		{
			set_script_variable(iRetIdx, -1);
			return 0;
		}
	}
	else if(iType == 2)
	{
		set_script_variable(iRetIdx, player_sparta_exit_sparta_apply(stSEnv.pstEnv, pstPlayer));
		return 0;
	}
	else
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	
	set_script_variable(iRetIdx, 0);
	return 0;
}

//主要是取消跨服战场队列和末世荣耀殿堂队列等前的检查
int s_cancel_span_queue_check()
{
	int iType = get_script_number(NULL); // 已经报名哪种队列了
	int iRetIdx = get_script_number(NULL); // 返回标志 0 表示成功，-1表示失败

	Player *pstPlayer = NULL;
	if (!stSEnv.pstPlayer)
	{
		set_script_variable(iRetIdx, -1);
		return 0;
	}
	int iTempType = 0;//0 表示都没有报

	pstPlayer = stSEnv.pstPlayer;

	if(pstPlayer->stOnline.stSpanOnline.stApply.iBattleApply == SPAN_APPLY_ING)
	{
		iTempType = 1;  // 报名了战场
	}
	else if(pstPlayer->stOnline.stSpanOnline.stApply.iSpartaApply == SPAN_APPLY_ING)
	{
		iTempType = 2; // 报名了末世殿堂
	}

	set_script_variable(iType, iTempType); 
	set_script_variable(iRetIdx, 0);
	return 0;
}

