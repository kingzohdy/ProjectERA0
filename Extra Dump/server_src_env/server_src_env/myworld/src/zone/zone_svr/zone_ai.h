
#ifndef ZONE_AI_H
#define ZONE_AI_H

#include "zone_svr.h"

typedef struct
{
	ZoneAni stAtker;
	int iHurt;
	int iSkillPressRate;
}AIBeAtk;

typedef struct
{
	ZoneAni stAni;
}AIPress;

typedef struct
{
	Player *pstPlayer;
	int iTime;
}AIBeDefy;

typedef struct
{
	ZoneAni stAtkee;
	int iTime;
}AIMark;

typedef struct
{
	Player *pstPlayer;
}AICopyPress;

typedef union
{
	AIBeAtk stBeAtk; //AI_EVT_BEATK
	AIPress stInPress; //AI_EVT_INPRESS
	AIPress stOutPress; //AI_EVT_OUTPRESS
	AIBeDefy stBeDefy; //AI_EVT_BEDEFY
	AIMark stMark; // AI_EVT_MARK
	AICopyPress stCopyPress; // AI_EVT_COPY_PRESS
}AIEvtDetail;

typedef enum
{
	AI_EVT_SYSOUT = 0, //系统刷出
	AI_EVT_BEATK ,
	AI_EVT_INPRESS , // 进压力表
	AI_EVT_OUTPRESS, //出压力表
	AI_EVT_BEDEFY, //被嘲讽
	AI_EVT_MARK, // 标记
	AI_EVT_UNMARK, // 标记清除
	AI_EVT_COPY_PRESS , // 复制压力值
}AIEVTID;

typedef struct
{
	int iEvtID;
	AIEvtDetail stEvtDetail;
}AIEvt;

int  z_mon_ai_data(ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef);

int mon_prepare_end(ZONESVRENV* pstEnv, Monster *pstMon, SKILLDEF *pstSkillDef);
	
int mon_ai_stat(ZONESVRENV* pstEnv, Monster *pstMon, AIEvt *pstAIEvt);

int mon_press_insert(ZONESVRENV* pstEnv, Monster *pstMon, int iTargetID, int iPress);
int mon_press_delete(Monster *pstMon, int iTargetID);
int mon_press_max(TAPPCTX *pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, int *piIdx);
int mon_press_min_idx(Monster *pstMon);
int mon_press_find(Monster *pstMon, int iTargetID);

int mon_press_insert_player(ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef, Player *pstPlayer);
int mon_ai_tick(ZONESVRENV* pstEnv, Monster *pstMon);
int mon_press_delete_player(ZONESVRENV* pstEnv, Monster *pstMon, Player *pstPlayer);
int mon_ai_atk_continue(ZONESVRENV* pstEnv, Monster *pstMon);

int mon_ai_addpress(ZONESVRENV* pstEnv, Monster *pstMon, ZoneAni *pstZoneAni, int iPress);

int mon_ai_atk(ZONESVRENV* pstEnv, Monster *pstMon, int iDelay);
int mon_ai_trace(ZONESVRENV* pstEnv, Monster *pstMon, int iDelay);
int mon_ai_home(ZONESVRENV* pstEnv, Monster *pstMon);

int mon_have_movespd(Monster *pstMon);

int mon_press_update_out(ZONESVRENV* pstEnv, Monster *pstMon);
int mon_press_update_mon_in(ZONESVRENV* pstEnv, Monster *pstMon, Monster *pstVMon);
int mon_press_update_player_in(ZONESVRENV* pstEnv, Monster *pstMon, Player *pstPlayer);
int mon_ai_move(ZONESVRENV* pstEnv,  Monster *pstMon);
int mon_ai_run_away(ZONESVRENV* pstEnv,  Monster *pstMon, Player *pstTarget);

int z_mon_skill_break(ZONESVRENV* pstEnv, Monster *pstMon);

int mon_svr_atk(ZONESVRENV* pstEnv, Monster *pstMon, int iSkillID, int iSkillLevel, int iAtkerID);
int mon_ai_evt_outpress(ZONESVRENV* pstEnv, Monster *pstMon);

int mon_press_add_by_career(ZONESVRENV* pstEnv, Monster *pstMon, int iCareer,
                            int iValue);

int mon_press_add_by_max(ZONESVRENV* pstEnv, Monster *pstMon, int iValue);

int mon_press_order(Monster *pstMon, int iOrder);

int mon_press_specific_career(ZONESVRENV* pstEnv, Monster *pstMon,
                              int iWantCareer);

int mon_press_hp_order(ZONESVRENV* pstEnv, Monster *pstMon, int iOrder);

int mon_press_hp_percent_order(ZONESVRENV* pstEnv, Monster *pstMon, int iOrder);

int mon_press_forced_rand(Monster *pstMon);

int mon_ai_atk_specific_target_by_id(ZONESVRENV* pstEnv, Monster *pstMon,
                                     int iSkillID, int iTargetID);

int mon_press_insert_mon(ZONESVRENV* pstEnv, Monster *pstMon, Monster *pstVMon);

PATROLDEF *z_find_patroldef(ZONESVRENV* pstEnv, int iPatrolID, int *piPatrolIdx);

#define GET_MON_IS_FIGHTING(mon) \
	(((mon)->stAtkTargets.PressNum > 0 || \
	  (mon)->stAtkTargets.iDefyID > 0 || \
	  (mon)->stAtkTargets.iMarkedID > 0)? 1 : 0)

int mon_ai_refreshed_skill(ZONESVRENV* pstEnv, Monster *pstMon,
                           AIREFRESHEDSKILL *pstProfile);

int mon_skill_begin_cool(ZONESVRENV* pstEnv, Monster *pstMon);
int mon_atk_end(ZONESVRENV* pstEnv, Monster *pstMon, SKILLDEF *pstSkillDef, ZoneObjList *pstObjList);

int mon_ai_check_skill_conditionid(ZONESVRENV* pstEnv, Monster *pstMon, 
		SKILLDEF *pstSkillDef, int ConditionID, int ConditionVal, int iBeAtkHurt);

int mon_select_atkskill(ZONESVRENV* pstEnv, Monster *pstMon, ZoneAni *pstAtkee, int *piHaveSkill);

int mon_ai_get_dynmskmon_distance(ZONESVRENV* pstEnv, Monster* pstDynMskMon, MONSTERDEF *pstDynMskMonDef, RESPOS *pstFrom, RESPOS *pstTo,RESPOS *pstInter);

int mon_press_target_dynmsk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, Monster *pstMon,RESPOS *pstTracePos);

int is_mon_bub(Monster *pstMon);

Player* mon_bub_get_owner(ZONESVRENV *pstEnv, Monster *pstMon);

int mon_bub_get_mode(ZONESVRENV *pstEnv, Monster *pstMon);

int mon_bub_ai_home(ZONESVRENV* pstEnv, Monster *pstMon);

int mon_bub_del_owner_info(ZONESVRENV* pstEnv, Monster *pstMon);

int mon_bub_inpress_check(ZONESVRENV* pstEnv, Monster *pstMon, ZoneAni *pstAni);

int mon_ai_totem_addpress(ZONESVRENV* pstEnv, Monster *pstMon, ZoneAni *pstZoneAni, int iPress);

int z_mon_skill_atk_dist(SKILLDEF *pstSkillDef);

int mon_press_delete_mon(ZONESVRENV* pstEnv, Monster *pstMon, Monster *pstTar);

//刷状态技能,CoolFlag 0表示刷出来就要冷却，1表示刷出来不要冷却
int mon_refresh_state_skill( ZONESVRENV* pstEnv, Monster* pstMon,
								MONSTERSTATESKILLDEF *pstMonStateSkillDef,int CoolFlag);


int mon_refresh_skill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster* pstMon);
int mon_ai_press_modify(ZONESVRENV* pstEnv, Monster *pstMon, int iID, int iVal);

int mon_bub_get_idx(ZONESVRENV* pstEnv, Monster *pstMon);

int is_single_convoy_car(Monster *pstMon);

//此函数会恢复AI状态，不可在MON_AI_HOME中调用
int mon_ai_home_process(ZONESVRENV* pstEnv, Monster *pstMon);

int mon_press_target_dynmsk_pos(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, 
									ZoneAni *pstAni,RESPOS *pstForm, RESPOS *pstTo, RESPOS *pstInter,int *pinsterCount,int iCheckPathFlag);

SKILLDEF * mon_find_one_skilldef(ZONESVRENV* pstEnv, Monster *pstMon);

int mon_press_add_by_memid(ZONESVRENV* pstEnv, Monster *pstMon, int iTargetID, int iPress);
int npc_in_press_check_camp(ZONESVRENV* pstEnv, Monster *pstMon, Player *pstPlayer);

#endif /* ZONE_AI_H */
