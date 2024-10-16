

#ifndef ZONE_MISC_H
#define ZONE_MISC_H

#include "zone_svr.h"
#include "zone_combat.h"

enum tagBrkPreActType
{
	 BRK_PRE_ACT_BEATK = 0,
	 BRK_PRE_ACT_MOVE,
	 BRK_PRE_ACT_SELF_NOPRE_ATK,	
	 BRK_PRE_ACT_BEATK_CTRLSKILL,  
	 BRK_PRE_ACT_BEATK_CTRLSTATUS,	
	 BRK_PRE_ACT_STALL,  
	 BRK_PRE_ACT_DIE,	
	 BRK_PRE_ACT_LOGOUT,
	 BRK_PRE_ACT_CHG_MAP,
	 BRK_PRE_ACT_SHAM_DEAD,
	 BRK_PRE_ACT_SPAN_SVR,
	 BRK_PRE_ACT_SPE_MON,
};

int role_test_action(ZONESVRENV* pstEnv, Player *pstPlayer, int iFlag,int iVictory);
int player_qiecuo(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						     TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int z_player_tick_qiecuo(ZONESVRENV* pstEnv, Player *pstPlayer);
int z_player_qiecuo_end(ZONESVRENV* pstEnv, Player *pstPlayer, int iVictory);
int z_qiecuo_invite_cancel(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_logout_qiecuo(ZONESVRENV* pstEnv, Player *pstPlay);

int z_is_qiecuo(ZONESVRENV* pstEnv, Player *pstAtker, Player *pstAtkee);
int player_qiecuo_check(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_accgift_res(ZONESVRENV* pstEnv, SSPKG *pstSSPkg);
int player_accgift_insert_res(ZONESVRENV* pstEnv, SSPKG *pstSSPkg);
int player_propsgift_res(ZONESVRENV* pstEnv, SSPKG *pstSSPkg);
int player_propsgift_bill_res(ZONESVRENV* pstEnv, SSPKG *pstSSPkg);
int player_update_online_sec(ZONESVRENV* pstEnv,Player *pstPlayer,int iLogin);
int player_login_cmp_onlinegroup(ZONESVRENV* pstEnv,Player *pstPlayer);
int player_update_onlinegiftgroup(ZONESVRENV* pstEnv,Player *pstPlayer);
int player_onlinegiftgroup_info(ZONESVRENV* pstEnv,Player *pstPlayer, GIFTGROUP *pstGiftGroup);
int giftgroup_add_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,GIFTONE *pstGiftOne,int iID);
int giftgroup_res_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,GIFTONE *pstGiftOne,int iID);
ONLINEGIFTGROUP *z_find_onlinegroupgift(ZONESVRENV* pstEnv, int iID);
int player_cdkey_req(ZONESVRENV* pstEnv, Player *pstPlayer, const char* pszCDKey);
int player_ss_cdkey_res(ZONESVRENV* pstEnv, SSPKG *pstSSPkg);


int player_trace_act_break_prepare(ZONESVRENV* pstEnv, Player *pstPlayer, int iActType);

int player_pworld_off_exp(ZONESVRENV* pstEnv, Player *pstPlayer, int iNotify);

int player_pworld_award_offexp(ZONESVRENV* pstEnv, Player *pstPlayer, int iAwardExp);

int player_pworld_offmul_chg(ZONESVRENV* pstEnv, Player *pstPlayer, int iMul);

int atker_hurt_mul_get(ZONESVRENV *pstEnv, AtkerInfo *pstAtkerInfo, AtkeeInfo *pstAtkeeInfo);

int player_raffle(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						     TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int atker_type_get(ZoneAni *pstAni, SKILLDEF *pstSkillDef);

int is_in_qiecuo(ZONESVRENV* pstEnv, Player *pstPlayer);

int z_player_clean_badbuff(ZONESVRENV* pstEnv, Player *pstPlayer);
int active_clt_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

//等级差经验补偿
int player_level_exp_mul_get(ZONESVRENV* pstEnv, Player *pstPlayer);

//经验值补偿
int player_world_level_exp(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_longlong *piExp);

//经验BUFF 每跳的经验
int player_level_buff_exp_add(ZONESVRENV* pstEnv, Player *pstPlayer);

//离线经验一次性使用检查
int player_use_all_offline_exp_check(ZONESVRENV *pstEnv,Player *pstPlayer);

//离线经验一次性使用
int player_use_all_offline_exp(ZONESVRENV *pstEnv,Player *pstPlayer, int iAddMul);

//活动补偿经验一次性使用检查
int player_use_all_pworld_offexp_check(ZONESVRENV* pstEnv, Player *pstPlayer);

//活动补偿经验一次性使用
int player_use_all_pworld_offexp(ZONESVRENV* pstEnv, Player *pstPlayer, int iAddMul);

//角色登录等级补偿BUFF信息
int player_login_level_buff_info(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_level_buff_award(ZONESVRENV* pstEnv, Player *pstPlayer);

//广播世界最高等级
int world_max_level_broadcast(ZONESVRENV* pstEnv);

int player_mt_gift_info(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_mt_gift_add_check(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_mt_gift_add(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_mt_gift_award(ZONESVRENV* pstEnv, Player *pstPlayer);

int is_mt_gift_item(int iItemID);

YANGHAIBANGDEF *z_find_yanghaibang_def(ZONESVRENV* pstEnv, int iLevel, int iDayNum);

int player_yanghaibang_day_update(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_yanghaibang_open_ui(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_yanghaibang_award(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_yanghaibang_start(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_yanghaibang_start_check(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_yanghaibang_add(ZONESVRENV* pstEnv, Player *pstPlayer,CSYANGHAIBANGADD *pReq);

int player_yanghaibang_val(ZONESVRENV* pstEnv, Player *pstPlayer,int iNum);

int player_yanghaibang_login_check(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_pinjiu_screen_vague(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_daqiangdao_fenshu(ZONESVRENV* pstEnv, Player *pstPlayer,int iFenShu);

int player_career_alter_check(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_career_alter(ZONESVRENV* pstEnv, Player *pstPlayer,int iCareer);

int player_clear_shortcut_skill_clt(ZONESVRENV* pstEnv, Player *pstPlayer,CSCLEARSHORTCUTSKILL *pstReq);

int player_add_all_skill_by_now_level(ZONESVRENV* pstEnv, Player *pstPlayer);



#endif 



