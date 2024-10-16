
#ifndef ZONE_ATTR_H
#define ZONE_ATTR_H

#include "zone_svr.h"

#define MSPD_DELAY 500000
#define MAX_EXP 1000000000000LL

enum tagLevelMulType
{
	 LEVEL_MUL_ATK = 0,
	 LEVEL_MUL_DEF,	
	 LEVEL_MUL_DEF_HEAVYHIT,  
};


enum tagBaseAttr2AttrType
{
	 BASE2ATTR_TYPE_HP = 0,
	 BASE2ATTR_TYPE_MP,
	 BASE2ATTR_TYPE_ICEATK,	
	 BASE2ATTR_TYPE_ICEDEF,  
	 BASE2ATTR_TYPE_PHSICATK,	
	 BASE2ATTR_TYPE_PHSICDEF,  
	 BASE2ATTR_TYPE_HIT,	
	 BASE2ATTR_TYPE_FLEE,
	 BASE2ATTR_TYPE_HEAVYHIT,
	 BASE2ATTR_TYPE_DEFHEAVYHIT,
};

int z_get_player_hpmax(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int z_get_player_mpmax(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int z_get_player_spmax(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int z_calc_all_attr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSPLAYERATTRVAL *pstPlayerAttrVal);
int z_attr_chg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, PlayerChagAttrVal *pstVal, int iNotify);
int z_update_all_attr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
void z_level_up_attrlist(ZONESVRENV *pstEnv, Player* pstPlayer, CSPLAYERATTRVAL *pstChg);
void z_level_up_attr( ZONESVRENV* pstEnv,Player* pstPlayer, LEVELUP *pstLevelUp);
int z_mon_chk_mspd(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster* pstMon, int iNotify);
int z_get_mon_spd(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster* pstMon, MONSTERDEF *pstMonDef);
int z_player_chk_mspd(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int z_get_player_mspd(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int z_is_weapon(ITEMDEF *pstItemDef, char cCareer);
char *z_get_money_name(int iMoneyType);
int player_update_cache_level(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_update_cache_career(ZONESVRENV* pstEnv, Player *pstPlayer);

int pet_attr_update(ZONESVRENV *pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, RolePetOnline *pstRolePetOnline);
int pet_role_fight_cacl(ZONESVRENV* pstEnv, Player *pstPlayer);
int pet_fight_attr(ZONESVRENV* pstEnv, Player *pstPlayer);

int z_player_chk_maxmphp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);

int z_attr_result(ZONESVRENV* pstEnv, DYNATTRLIST *pstDyn, ROLEARM *pstRoleArm, RESULT *pstResult);
int z_calc_player_attr_atk(DYNATTRLIST *pstDyn, int iAttrID);

int z_cacl_dyn_status_result(DYNATTRLIST *pstDyn, ZoneStatus *pstZoneStatus, STATUSRESULT *pstResult);

int z_get_career_basehp(ZONESVRENV* pstEnv, char cCareer, unsigned int ucLevel);

int z_get_career_basemp(ZONESVRENV* pstEnv, char cCareer, unsigned char ucLevel);

ARMADDATTRDEF *z_find_armaddattrdef(ZONESVRENV* pstEnv, int AddAttrID);

int z_get_watched_attr(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,
	                     CSPLAYERATTRVAL *pstAttrVal);
ARMLEVELHURTDEF *z_find_armlevelhurt(ZONESVRENV* pstEnv, unsigned short unArmType, unsigned short unLevel);

int z_create_arm_addattr(ZONESVRENV* pstEnv, ITEMDEF *pstItemDef, ROLEARM *pstRoleArm);

int player_add_exp(ZONESVRENV* pstEnv, Player *pstPlayer, int iExp, int iFlag);
int player_add_hp(ZONESVRENV* pstEnv, Player *pstPlayer, int iHp, int iFlag,int iNotify);
int player_add_xp(ZONESVRENV* pstEnv, Player *pstPlayer, int iXp, int iFlag, int iNotify);

int z_rand_create_addattr(ZONESVRENV* pstEnv, ITEMDEF *pstItemDef, ROLEARM *pstRoleArm);

int z_role_vimmax(ZONESVRENV* pstEnv, int iLevel);
int player_dec_exp(ZONESVRENV* pstEnv, Player *pstPlayer, int uiExp);
int get_player_dayexplimit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);

//�츳 ��������hp
int genius_add_hp_by_heavy_hit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
//�츳 �����ܼ���
int genius_add_status_by_flee(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
//�츳 ��������ʱ�����
int genius_reduce_pre_time_rate_by_atk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
//�츳  ���������ټ���CD
int genius_reduce_skill_cd_time_by_heavyhit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, struct timeval *pstCltTime,int iHeavyHit);
//�츳 ������������ --���
int genius_add_status_by_player_atk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstAtker, Player *pstAtkee);
//�츳 ������������ --����
int genius_add_status_by_pet_atk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstAtker, ROLEPET *pstRolePet, Player *pstAtkee);
//�츳 ������������ --����
int genius_add_status_by_monster_atk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstAtkee, Monster *pstMon);
//�츳 ��ͨ����������Ѫ
int genius_atk_random_add_hp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstAtker, char SkillType );
//�츳 ����ָ�����ܵ�cdʱ�� 
int genius_atk_reduce_skill_cd_time(Player *pstAtker,CSSKILLDATA *pstSkillData, SKILLRESULT *pstResult);
// �츳 ��ָ��������ǿ
int genius_atk_skill_add_hurt(Player *pstAtker,ZoneSkill *pstZoneSkill,SKILLRESULT *pstResult);
// �츳 �Լ���cd��Ӱ��
int z_skill_diathesis_attr(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneSkill *pstZoneSkill, SKILLDEF *pstSkillDef);
//�츳 ��ͨ������������
int genius_atk_random_add_mp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstAtker, char SkillType );
// �츳 ��ͨ�������������˺�
int genius_atk_random_add_hurt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstAtker, char SkillType,ATKCHG *pstAtkChg );
//�츳 ��ͨ���������������buff
int genius_atk_random_add_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,char SkillType);
// �츳 ���ܱ����ʵ���ǿ
int genius_atk_skill_add_heavy_hit_rate( Player *pstAtker,ZoneSkill *pstZoneSkill, SKILLRESULT *pstResult);

int z_ploy_info_chg(ZONESVRENV* pstEnv, Player *pstPlayer, int iCmd);

int z_add_arm_addattr(ITEMDEF *pstItemDef, ROLEARM *pstRoleArm, int iAddAttrID,
                      int iAddAttrIdx, ARMRESULT *pstResult);

int get_fairy_base_xp(ZONESVRENV* pstEnv, Player *pstPlayer);
int get_fairy_xp_upper(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_career_chg_action(ZONESVRENV* pstEnv,Player *pstPlayer);

int player_collectskill_add_slot(ZONESVRENV* pstEnv, Player *pstPlayer,int iVal,int iAddType);

int player_collectskill_dec_slot( ZONESVRENV* pstEnv, Player *pstPlayer);

int z_get_player_vimmax(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);

// �츳 ��ʱ��е�ٶ�
int genius_add_tmpmachine_spd(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,ITEMDEF *pstItemDef,int *iMspd);

// �츳 ��ʱ���ߵĻ�������
int genius_add_tmpitem_collectnum(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,ITEMDEF *pstItemDef);

// �츳 ��ʱ��е��������
int genius_add_tmpitem_energy(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,ITEMDEF *pstItemDef);

// ��ɱ����״̬
int genius_add_status_by_player_kill(ZONESVRENV* pstEnv, Player *pstAtker);

// �츳 �������Ļ���������
int genius_collectskill_dec(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,unsigned short SkillID);

// �츳 Ӱ���������ܵĵȼ�
int genius_atkskill_addlevel(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,unsigned short SkillID);

int get_player_level_mul(ZONESVRENV* pstEnv, Player *pstPlayer, int iType);

int get_mon_level_mul(ZONESVRENV* pstEnv, Monster *pstMon, int iType);

int  z_dyn_attr_translate(ZONESVRENV* pstEnv, Player *pstPlayer, int iType);

int get_player_level_attr(ZONESVRENV* pstEnv, Player *pstPlayer, int iType);

int z_player_script_event(ZONESVRENV* pstEnv, Player *pstPlayer,int iProc);

int role_login_attr_chg(ZONESVRENV* pstEnv, Player *pstPlayer, CSPLAYERATTRVAL *pstPlayerAttrVal);

int pet_attr_diathesis(ZONESVRENV *pstEnv, Player *pstPlayer,PETSKILLGRID *pstDiaThesisOne, PetPowerOnline *pstPetPowerOnline, PetExpOnline *pstPetExpOnline, PetFightAttrOnline *pstPetFightAttrOnline, PetTiShenOnline *pstPetTiShenOnline);

int calc_addattr(ZONESVRENV *pstEnv, DYNATTRLIST *pstDyn,int iSuitAttrID);

int z_get_attr_list(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSPLAYERATTRVAL *pstPlayerAttrVal, int fBaseAttr);

//����װ����������ID���ӱ���
int arm_base_attr_mul_set(ROLEARM *pstRoleArm, int iResultID, int iVal);

//������һ��������ֵ
int arm_base_attr_rand_get(ZONESVRENV* pstEnv, int iResultID);

//����һ���������Եĸ�����Ϣ
BASEATTRINFO* arm_base_attr_info_get(ROLEARM *pstRoleArm, int iResultID);

//װ�����������ֵ����
int arm_base_attr_create(ZONESVRENV* pstEnv, ITEMDEF *pstItemDef, ROLEARM *pstRoleArm);

//���װ��ָ��Ч�����ӵ���ֵ
int arm_base_attr_val_get(ROLEARM *pstRoleArm , ITEMDEF *pstItemDef, int iResultID);

int z_calc_diathesis(ZONESVRENV* pstEnv, Player *pstPlayer, SKILLRESULT *pstResult);

int z_arm_skill_levelup(DYNATTRLIST *pstDyn, unsigned short unSkillID, unsigned char ucLevel);

int z_arm_add_skill(ZONESVRENV* pstEnv, DYNATTRLIST *pstDyn, unsigned short unSkillID, unsigned char ucLevel);

// ��ɫ����ֵ����
unsigned int z_get_max_vig(ZONESVRENV* pstEnv,int  iLvl);

int player_add_vig(ZONESVRENV* pstEnv, Player *pstPlayer, int iVig, int iFlag,int iNotify);
int player_add_vig_check(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_zhengyi_limit_chg(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_add_zhengyi_test(ZONESVRENV* pstEnv, Player *pstPlayer, unsigned int *piAdd);
int player_add_zhengyi_limit(ZONESVRENV* pstEnv, Player *pstPlayer, int iZhengyiLimit);

int z_add_attr_lifebrand(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, AttrChgVal *pstVal, CSPLAYERATTRVAL *pstChg);
int player_add_lifebrand_test(ZONESVRENV* pstEnv, Player *pstPlayer, unsigned short *piAdd);
int player_life_brand_limit_chg(ZONESVRENV* pstEnv, Player *pstPlayer);

int z_status_chg_skill_level(ZONESVRENV* pstEnv,Player* pstPlayer);



#endif /* ZONE_ATTR_H */
