
#ifndef ZONE_COMBAT_H
#define ZONE_COMBAT_H

#include "zone_svr.h"
#include "zone_team.h"
#include "zone_move.h"
#include "zone_npc.h"
#include "zone_ai.h"
#include "mapmask.h"

#define HEAVYHIT_BASE 1.5
#define DEADHIT_BASE 15
#define BREAK_BASE 15
#define MAX_ADD_ATK 3
#define SKILL_FLY 7004 
#define SKILL_QIANXIN 7003
#define SET_PLAYER_NOFIGHT(pstPlayer) ((pstPlayer)->stOnline.tFightStatEnd = 0, (pstPlayer)->stOnline.iHitTarget = 0, (pstPlayer)->stOnline.State &= ~CS_STAT_FIGHT)

#define REPLACE_SKILL_OP_CLANITEM    0 //使用公会蒸汽道具
#define REPLACE_SKILL_OP_MACHINE     1 //使用机械
#define REPLACE_SKILL_OP_TMP             2 //使用临时系统
#define REPLACE_SKILL_OP_WUSHUANG 3 //使用无双技能

typedef struct
{
	ZoneAni stAtker;
	ZoneAni stAtkee;
}ZoneOneDead;

typedef struct
{
	int iSelf;
	ZoneOneDead stSelfDead;
	int iCount;
	ZoneOneDead astDeads[MAX_ATKEE_NUM];
}ZoneDeadList;

typedef struct
{
	int iMPUse;
	int iHPUse;
	int iSpdUse;
	int iCollectNumUse;//临时道具的使用
	int iEnergeUse;//临时道具的使用
	int iAmmUse;
	int iAmmID;
	int iAmmPos;
	int iXpUse;
	int iCollectSkillUse;//积累数技能的消耗
}ResourceConsume;

#define ATKER_INFO_FLAG_THROUGH_ABSORB 1
typedef struct{
	ZoneAni stAtker;
	DYNATTRLIST stDyn;
	ZoneSkill *pstZoneSkill;
	//CSFITTINGSROW *pstFittings;
	SKILLDEF *pstSkillDef;
	SKILLDEF *pstOriginDef;
	ResourceConsume stResourceConsume;
	//CollectNumAdd stCollectNumAdd;
	int iHurtLimitAdd;
	RESPOS stAtkPos;
	RESPOS stRolePos;
	unsigned char ucAppendResult[MAX_TARGET_STATUS]; 	// 额外result 索引
	CSTIME SvrTime;//这个时间先记下来,用来算技能的cooldown时间
	Player *pstStatusSkillAtker;
	unsigned char SelfPosChgFlag; 
	char cJiqiNum;
       RESPOS SelfPosChg;
	int iFlag; //按位的标记   
	int iPilot;
	
}AtkerInfo;

typedef struct{
	ZoneAni stAtkee;
	DYNATTRLIST stDyn;
}AtkeeInfo;

typedef struct{
	int   iMainAttrHurt;
	int unAtkerMul;
	int unAtkerAdd;
	int unAtkeeMul;
	int unAtkeeAdd;
	int iContinueNum;
	short nAddAtkNum;
	short aiAddAtkMul[MAX_ADD_ATK];  //追招攻击力比例
}AddAtkInfo;

typedef struct
{
	int iCount;
	Monster *apstMon[MAX_SKILL_TARGET];
	AIEvt astMonAi[MAX_SKILL_TARGET];
}ZoneAiList;

typedef struct
{
	int iAddAtkMul;
	int  iHpRateHurt;	 	// 按当前血量计算出的伤害
}HpRateHurt;

typedef struct 
{
	Player *pstBeater;
	ZoneSkill *pstZoneSkill;
}BeatBackInfo;

typedef struct
{
	int iBeatBackNum;
	BeatBackInfo astBeatBackInfo[MAX_ATKEE_NUM];
}ZoneBeatBackList;

typedef struct
{
	int iNum; // 反射了伤害的目标总数
    struct {
		ZoneAni stFrom; // 从哪个目标反射
		//long long llTotalDamage;   // 反射的伤害
		CSONEHIT stHit; // 反射的伤害
    } astEntries[MAX_SKILL_TARGET];
} ZoneReflectedDamage;

typedef struct{
	CSHITS stHits; //攻击者作用与被攻击者的数值结果
	ZoneDeadList stDeadList; //死亡列表
	ZoneAiList stAiList;
	AtkerInfo stAtkerInfo; //攻击者信息
	int   iHurt;
	AddAtkInfo stAddAtkInfo; //追加信息
	HpRateHurt stHpRateHurt;
	int iAtkChgIdx;
	SKILLARMCHG *pstSkillArmChg;
	int iContinueCount;
	unsigned char bContinueNum;
	char cQieCuoEnd;
	ZoneBeatBackList stBeatBackList;
	char cDeadHit;
	char cHeavyHit;
	char cSordHpRate;
	char cSordMpRate;
	unsigned short ushKillAddMpNum;
	unsigned short ushKillAddCollectNum;
	int iSordHp;
 	ZoneReflectedDamage stReflectedDamage; // 伤害反射信息
 }AtkProcess;

extern int g_iMissTime;

int player_prepare(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
int player_skill_brk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_atk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_ride_prepare(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_ride_on(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_ride_brk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int z_ride_break(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);

int z_player_goto(ZONESVRENV* pstEnv, Player *pstPlayer, int iMap, RESPOS *pstPos);

int player_do_ride_brk( ZONESVRENV* pstEnv, Player *pstPlayer, int iRideItemID );

int player_atk_by_svr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iSkillID, int iAtkerID);
int z_mon_skill_break(ZONESVRENV* pstEnv, Monster *pstMon);

int z_mon_get_atk_list(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef, SKILLDEF *pstSkillDef, 
							ZoneAni *pstZoneAni, RESPOS *pstHitPos, ZoneObjList *pstZoneObjList, int iSearch);

int z_mon_hit_notify(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef, SKILLDEF *pstSkillDef,
						ZoneObjList *pstList);

int z_in_range(RESPOS *pstAtkeePos, RESPOS *pstAtkerPos, RESPOS *pstHitPos, RESRANGE *pstRange, int iSkillDist, int iRadius, int iDir);

void z_skill_clear(Player *pstPlayer);

int z_del_totem(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneSkill *pstZoneSkill, 
				Monster *pstTotem);

int z_ride_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);

PetFightShiOnline *pet_get_shipet_by_xiedaipos(Player *pstPlayer, int iXiedaiPos);

int pet_shi_hit(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, SKILLDEF *pstSkillDef, 
					PetZoneSkill *pstPetZoneSkill, ZoneAni *pstAniAtkee, PetFightShiOnline *pstPetFightShiOnline);

SKILLRESULT *z_skill_get_result(SKILLDEF *pstSkillDef, int iResultID);

int z_atkee_prepare_break(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iHurt, int iFlag);

int mon_fight_speech(ZONESVRENV* pstEnv, Monster *pstMon, int iFightType);

int z_del_unique_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iTagAniID, int iStatusID);

int z_player_ride_action(ZONESVRENV* pstEnv, Player *pstPlayer, int fRide);

int z_player_filter_atkobj(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ZoneAni *pstAni,  SKILLDEF *pstSkillDef,int *piRelation);

int z_player_hit_notify(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, /*CSFITTINGSROW *pstFittings,*/ ZoneSkill *pstZoneSkill, SKILLDEF *pstSkillDef, 
						SKILLDEF *pstOriginDef, ZoneObjList *pstList,  CSSKILLATK *pstHit, int iAtkChgIdx/*, unsigned int FittingsEffectID*/);

int z_get_package_ride(Player *pstPlayer, unsigned int iItemID);

int z_pet_campaign_break(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);

int z_player_goto_check(ZONESVRENV* pstEnv, Player *pstPlayer, int iMap, RESPOS *pstPos);

int z_send_player_fight(ZONESVRENV* pstEnv, Player *pstPlayer, int iFightType);

int z_player_fight_update(ZONESVRENV* pstEnv, Monster* pstMon, ZoneAni *pstAtker);

int z_mon_filter_atkobj(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, ZoneAni *pstAni, SKILLDEF *pstSkillDef,int *piRelation);

int z_hitch_ride_clear(ZONESVRENV* pstEnv, Player *pstPlayer);

int genius_add_hp_by_heavyhit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstAtkee,CSONEHIT *pstOneHit,int  iHeavyHit);

int z_mon_destroy(ZONESVRENV* pstEnv, Monster *pstMon);

int z_get_hit_player_relation(ZONESVRENV* pstEnv, Player *pstAtker, Player *pstAtkee);

int z_cacl_pet_status_result(ZONESVRENV* pstEnv, Player *pstPlayer, DYNATTRLIST *pstDyn, ROLEPET *pstRolePet);

int  player_set_skill_cool_time(ZONESVRENV* pstEnv, Player *pstPlayer, unsigned short unSkillID,unsigned short  SkillCDTime);

int is_friend_mon(Monster *pstMon);

int z_prepareatk_mon_skill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);


inline void  SET_PLAYER_FIGHT(ZONESVRENV* pstEnv, Player *pstPlayer, time_t  tCurr, int iFightDur);

inline int z_find_dup_target(int *piTargets, int iNum, int iID);

int z_cacl_attr_atk(ATKCHG *pstAtkChg, int iSkillAttrType);
int z_cacl_attr_def(DEFCHG *pstDefChg, int iSkillAttrType);

int z_player_hurt_status_absorb(ZONESVRENV* pstEnv,Player *pstPlayer, CSONEHIT *pstOneHit, int *piHurt);

int z_mon_hurt_status_absorb(ZONESVRENV* pstEnv,Monster *pstMon, CSONEHIT *pstOneHit, int *piHurt);

int z_obj_hurt_status_absorb(ZONESVRENV* pstEnv,ZoneAni *pstAni, CSONEHIT *pstOneHit, int *piHurt);

int is_player_same_camp_mon(Player *pstPlayer, Monster *pstMon);

int is_camp_mon(Monster *pstMon);

//判断队伍成员有多少个这个正方型里
int z_check_teammember_in_rect(ZoneTeam *pstZoneTeam,int MapID,int ox, int oy, int xlen, int ylen);

int z_do_skill_result_by_flee(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,SKILLDEF *pstSkillDef,Player *pstPlayer,Monster *pstMon);

int z_do_call_pos_xunluo_mon(ZONESVRENV* pstEnv, Monster *pstFind, SKILLDEF *pstSkillDef);

int player_get_hit_mon_relation(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMon);

int is_player_enemy_mon(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMon);

int is_player_enemy_player(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstOtherPlayer);

int z_mon_filter_relation(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, ZoneAni *pstAni, SKILLDEF *pstSkillDef);

int z_player_filter_relation(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ZoneAni *pstAni, SKILLDEF *pstSkillDef);

int z_player_hit_player_filter(ZONESVRENV* pstEnv, Player *pstAtker, Player *pstAtkee);

int is_mon_enemy_mon(ZONESVRENV* pstEnv, Monster *pstMon, Monster *pstAtkee);

int is_mon_enemy_player(ZONESVRENV* pstEnv, Monster *pstMon, Player *pstAtkee);

int z_get_dec_def(ATKCHG *pstAtkChg, int iSkillAttrType,int *piDecDef,int *piDecDefMul);

//存储闪电链的相关数据
LightningAtkInfo *z_storage_lightning_link_info(Player *pstPlayer);

int player_atk_by_lightning(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);

int z_get_in_range_team_member_list(ZONESVRENV* pstEnv, Player *pstPlayer, SKILLDEF *pstSkillDef,  ZoneObjList *pstZoneObjList, RESPOS *pstHitPos);

int z_player_atklist(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,  SKILLDEF *pstSkillDef, 
						CSSKILLATK *pstHit, ZoneObjList *pstList, struct timeval *pstCltTime);

int z_check_skill_arm(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneSkill *pstZoneSkill, SKILLDEF *pstSkillDef, int *piAtkChgIdx);

int z_atker_status_result(ZONESVRENV* pstEnv, SKILLDEF *pstSkillDef,AtkerInfo *pstAtkerInfo, ATKCHG *pstAtkChg, ZoneStatus *pstStatus,AtkProcess *pstAtkProcess);

int z_atkee_status_result(ZONESVRENV* pstEnv, SKILLDEF *pstSkillDef,AtkerInfo *pstAtkerInfo, ATKCHG *pstAtkChg, ZoneStatus *pstStatus);

void z_get_mon_dyn(ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonsterDef, DYNATTRLIST *pstDyn);

int z_cacl_mon_status_result(ZONESVRENV* pstEnv, DYNATTRLIST *pstDyn, ATKCHG *pstAtkChg, Monster *pstMon);

int z_miss_check(AtkerInfo *pstAtkerInfo, AtkeeInfo *pstAtkeeInfo);

int cacl_hurt(ZONESVRENV* pstEnv, AtkProcess *pstAtkProcess, AtkeeInfo *pstAtkeeInfo, 
		SKILLDEF *pstSkillDef, int iHeavyHit);

int z_cacl_skill_attr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, AtkProcess *pstAtkProcess, AtkeeInfo *pstAtkeeInfo);

int z_mon_dec_one_hp_by_hit(ZONESVRENV* pstEnv,Monster* pstMon);

int z_skill_break(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int iType, int iSkillID);

int z_do_skill_result_hurt_chg_team_hp(ZONESVRENV * pstEnv, Player *pstPlayer,AtkProcess *pstAtkProcess, 
													const RESPOS *pstAtkPos, int iHurt);

int player_atk_by_add_skill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,
								int iSkillID,int iLevel,RESPOS *pstAtkPos);

int player_atk_svr_get_list(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ZoneSkill *pstZoneSkill, SKILLDEF *pstSkillDef, CSSKILLATK *pstSkillAtk);

int check_replace_skill(ZONESVRENV* pstEnv, Player *pstPlayer, int iReplaceOP);

int z_mon_hit_factory(ZONESVRENV* pstEnv, Monster *pstMon, Monster *pstFactory);

int z_player_hit_factory(Monster *pstFactory, Player *pstAtker);

int z_machine_hit_player(ZONESVRENV* pstEnv, Monster *pstMachine, Player *pstAtkee);

int z_machine_hit_machine(ZONESVRENV* pstEnv, Monster *pstAtker, Monster *pstAtkee);

int IsSkillThrough(ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst, SKILLDEF *pstSkillDef ,TMAPMASK* pstMask, int lPixelX1, int lPixelY1, int lPixelX2, int lPixelY2);

int z_mon_get_atk_list_by_atkpos(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon,SKILLDEF *pstSkillDef,RESPOS *pstHitPos,
									ZoneObjList *pstZoneObjList);

int player_ballistic_hit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int z_skill_result_poschg(ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst, MapIndex *pstMapIdx, SKILLDEF *pstSkillDef, int iDist,
								SKILLRESULT *pstSkillResult, Player *pstPlayer, RESPOS *pstTargetPos, RESPOS *pstRolePos, RESPOS *pstPos);

int is_in_miss_time_player(struct timeval *pstCltTime, Player *pstPlayer);

int z_do_skill_result(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, AtkProcess *pstAtkProcess, AtkeeInfo *pstAtkeeInfo, SKILLRESULT *pstResult);

int z_send_player_hit_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,
                          Player *pstPlayer, RESPOS *pstAtkPos, CSPKG *pstPkg);

int is_valid_map_ride(MapIndex *pstMapIdx, RIDEDEF *pstRideDef);

int player_hit_team_call_rongyan_mon(ZONESVRENV*pstEnv,Player *pstAtker, Monster *pstAtkee);
int z_skill_transmit_spouse_pos_check(TAPPCTX* pstAppCtx,ZONESVRENV*pstEnv,Player *pstPlayer);
int z_skill_recover_spouse_hp_check(TAPPCTX* pstAppCtx,ZONESVRENV*pstEnv,Player *pstPlayer,SKILLDEF *pstSkillDef);

int z_span_map_if_player_hit_mon(ZONESVRENV*pstEnv,Player *pstAtker, Monster *pstAtkee,MONSTERDEF *pstMonsterDef);
int z_is_guard(MONSTERDEF *pstMonsterDef);
int z_span_map_if_mon_hit_mon(ZONESVRENV*pstEnv,Monster *pstMon,Monster *pstAtkee,MONSTERDEF *pstAtkeeMonsterDef);
int mon_if_in_span_map(ZONESVRENV *pstEnv,Monster *pstMon);



#endif /* ZONE_COMBAT_H */
