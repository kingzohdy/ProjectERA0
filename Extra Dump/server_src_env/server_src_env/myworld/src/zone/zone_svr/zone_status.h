
#ifndef ZONE_STATUS_H
#define ZONE_STATUS_H

#include "zone_svr.h"

#define SET_STATUS_RESULTMAP(RESULTID, Map) ((Map)[(RESULTID)/8] |= (1<<((RESULTID)%8)))

#define IS_STATUS_RESULTMAP_SET(RESULTID, Map) ((Map)[(RESULTID)/8]&(1<<((RESULTID)%8)))

#define SET_STATUS_TIME(time,end,now)		do											\
											{											\
												time = 0;								\
												if ((end).tv_sec > (now).tv_sec)			\
												{										\
													time = (end).tv_sec -(now).tv_sec;		\
												} 										\
											} while(0)

int z_status_online(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer , int iPosFlag);
int z_status_fill( TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSSTATUSINFO *pstCSStatusInfo);
STATUSDEF *z_find_status_zonestatus(ZONESVRENV* pstEnv, ZoneStatus *pstZoneStatus);
STATUSDEF *z_find_status(ZONESVRENV* pstEnv, int iStatusID, int iLevel);
int z_status_roledata(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iFlag);
int z_player_check_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int z_check_mon_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon);
int player_die_status_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int mon_die_status_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon);
ZoneStatus  * z_find_bindskill_status(ZONESVRENV* pstEnv, Player *pstPlayer, unsigned short unSkillID);
int z_get_player_status_result(ZONESVRENV* pstEnv, Player *pstPlayer, int iResultID, STATUSRESULT *pstResult, int iUnique);
int z_get_player_status_result_min(ZONESVRENV* pstEnv, Player *pstPlayer, int iResultID, int *piVal);
int z_get_mon_status_result(ZONESVRENV* pstEnv, Monster *pstMon, int iResultID, STATUSRESULT *pstResult);
int z_get_mon_status_result_min(ZONESVRENV* pstEnv, Monster *pstMon, int iResultID, int *piVal);
int z_player_apply_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iStatus, int iStatusLevel, int iLast, int iNotify,  int iApplyID);
int z_mon_apply_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstPlayer, int iStatus, int iStatusLevel, int iLast, int iNotify,  int iApplyID);
ZoneStatus* z_is_player_status_result(ZONESVRENV* pstEnv, Player *pstPlayer, int iResultID);
ZoneStatus* z_is_mon_status_result(ZONESVRENV* pstEnv, Monster *pstMon, int iResultID);
void z_get_player_csstatusinfo(TAPPCTX* pstAppCtx, CSSTATUSINFO *pstCSStatusInfo, ZoneStatusList *pstStatusList);
void z_get_mon_csstatusinfo(TAPPCTX* pstAppCtx, CSSTATUSINFO *pstCSStatusInfo, ZoneMonStatusList *pstStatusList);
int z_player_del_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iStatusID, int iNotify,  int iApplyID);
int z_player_send_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int player_status_clean(ZONESVRENV* pstEnv, Player *pstPlayer, SKILLRESULT *pstResult, unsigned char ucSubType);
int mon_status_clean(ZONESVRENV* pstEnv, Monster *pstMon, SKILLRESULT *pstResult, unsigned char ucSubType);
ZoneStatus * z_player_find_status(Player *pstPlayer, int iStatusID);
ZoneStatus *z_mon_find_status(Monster *pstMon, int iStatusID);
int z_mon_send_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon);
int player_status_qianxin(ZONESVRENV* pstEnv, Player *pstPlayer, int iFlag);
int player_status_consume( ZONESVRENV* pstEnv,PlayerChagAttrVal *pstChgVal,Player *pstPlayer);
int z_player_del_wushuang_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);

int z_mon_del_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, int iStatusID, int iApplyID);

int z_del_apply_unique_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);

// 处理Status相关的客户端消息
int player_cancel_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						             TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

STATUSRESULT *z_get_status_result(STATUSDEF *pstStatusDef, int iResultID);

int player_status_beatk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSONEHIT *pstOneHit);
int mon_status_beatk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon);
int player_status_skill_clean(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
//int player_status_heavyhit_clean(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);

// 指定位置取消指定Status
int player_del_status_by_idx(ZONESVRENV* pstEnv, Player *pstPlayer,
                             int iStatusID, int iIdx);

int z_pet_apply_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, int iStatus, int iStatusLevel, int iLast, int iNotify);

void z_get_pet_csstatusinfo(TAPPCTX* pstAppCtx, CSSTATUSINFO *pstCSStatusInfo, PetStatusInfo *pstStatusList);

int z_pet_check_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet);

STATUSDEF *z_find_status_petstatus(ZONESVRENV* pstEnv, ZoneStatus *pstZoneStatus);

int z_get_pet_status_result(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, int iResultID, STATUSRESULT *pstResult);

STATUSRESULT* player_status_result(ZONESVRENV* pstEnv, Player *pstPlayer,
                                   int iResultID);
STATUSRESULT* mon_status_result(ZONESVRENV* pstEnv, Monster *pstMon,
                                   int iResultID);

// 退出战斗时清除状态
int mon_out_fight_status_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,
                               Monster *pstMon);

int z_get_status_info_result(ZONESVRENV * pstEnv, ZoneStatus * pstZoneStatus, int iResultID, STATUSRESULT * pstResult);

// 清理匹配的状态
int Player_clean_matching_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int StatusFlag,long long MiscFlag);
int player_status_clean_all(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);

ZoneStatus* z_pet_find_status(ZONESVRENV* pstEnv,  Player *pstPlayer, ROLEPET *pstRolePet,int iStatusID);

int  z_player_has_wushuang_status(ZONESVRENV* pstEnv, Player *pstPlayer);

ZoneStatus *z_get_mon_zonestatus_by_resultid(ZONESVRENV* pstEnv, Monster *pstMon, int iResultID);

ZoneStatus *z_get_player_zonestatus_by_resultid(ZONESVRENV* pstEnv, Player *pstPlayer, int iResultID);
int z_player_status_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ZoneStatus *pstStatus, int iPos, unsigned char *pbNum);

int z_mon_status_ai_home_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon);

int z_del_status_by_memid(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, STATUSRESULT *pstResult,int iApplyID );

//暴击删除指定的状态
int z_del_result_by_heavyhit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,Player *pstPlayer,Monster *pstMon,int iHeavyHit);

// iFlag 1是加状态的时候，0是减状态的时候
int z_mon_chg_status_skill(ZONESVRENV* pstEnv, Monster* pstMon,STATUSDEF *pstStatusDef,int iFlag);

int z_mon_del_random_one_debuff(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon,SKILLRESULT *pstResult,CSONEHIT *pstOneHit);

int z_player_del_random_one_debuff(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,SKILLRESULT *pstResult,CSONEHIT *pstOneHit);

int z_status_megre_debuff(ZONESVRENV* pstEnv,ZoneAni *pstZoneAni,SKILLRESULT *pstResult,CSONEHIT *pstOneHit);

ZoneStatus * z_player_find_status_by_level(Player *pstPlayer, int iStatusID,char cLevel);

ZoneStatus *z_mon_find_status_by_level(Monster *pstMon, int iStatusID,char cLevel);

int z_status_mul_skill_hurt( ZONESVRENV* pstEnv, ZoneStatus *pstZoneStatus,int *iHurt,int *iHurtMul);

int z_mon_status_chuanran_by_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon,ZoneStatus *pstZoneStatus);

int z_player_status_chuanran_by_team(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,ZoneStatus *pstZoneStatus);

int z_player_status_chuanran_by_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,ZoneStatus *pstZoneStatus);

int is_ctrl_status(STATUSDEF *pstStatusDef);

int z_status_player_refresh_time_by_hit(ZONESVRENV* pstEnv, SKILLDEF *pstSkillDef,Player *pstPlayer);

int z_status_monster_refresh_time_by_hit(ZONESVRENV* pstEnv, SKILLDEF *pstSkillDef,Monster *pstMon);

int z_status_pet_refresh_time_by_hit(ZONESVRENV* pstEnv, SKILLDEF *pstSkillDef,Player *pstPlayer, ROLEPET *pstRolePet);

int z_status_bind_skill_trig_end(ZONESVRENV* pstEnv,ZoneStatus *pstZoneStatus,RESPOS *pstAtkPos);

int mon_status_clean_debuff(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon);

int player_status_clean_debuff(ZONESVRENV* pstEnv, Player *pstPlayer);

int z_player_check_hp_chunnel_status( ZONESVRENV* pstEnv,Player *pstPlayer,ZoneStatus stZoneStatus);

int z_mon_check_hp_chunnel_status( ZONESVRENV* pstEnv,Monster *pstMon,ZoneStatus stZoneStatus);

int z_player_apply_status_test(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iStatus, int iStatusLevel);

int z_status_offline(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer );

int z_status_safearea(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer );

int z_status_grab(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer );

int z_player_del_status_seq(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iNotify, int iSeq);
int z_mon_del_status_seq(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, int iSeq);

int z_player_del_status_level(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iStatusID, int iLevel, int iNotify, int iApplyID);

int z_mon_status_chg_hp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, int iHPChg,int iApplyID);

int z_mon_apply_status2(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, int iStatus, int iStatusLevel, int iLast, int iNotify, int  iApplyID);

int player_has_bianshen_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int z_player_byatk_clear_status(ZONESVRENV* pstEnv, Player *pstPlayer);
#endif /* ZONE_STATUS_H */
