#ifndef ZONE_ADDATTR_H
#define ZONE_ADDATTR_H

#include "zone_svr.h"
#include "zone_combat.h"

int add_attr_pre(ZONESVRENV *pstEnv);

//��Ʒ�ؼ�������
int item_add_attr_create(ZONESVRENV *pstEnv, ROLEARM *pstRoleArm, ITEMDEF *pstItemDef);

//�ؼ������Լ���
int cacl_arm_add_attr_result(ZONESVRENV* pstEnv, Player *pstPlayer, DYNATTRLIST *pstDyn,
	ITEMDEF *pstItemDef, int iAddAttrID, int iVal);

//װ��ϴ��
int player_arm_wash(ZONESVRENV* pstEnv, Player *pstPlayer, ARMWASHDATA *pstCSWash);

//װ��ϴ������
int player_arm_wash_save(ZONESVRENV* pstEnv, Player *pstPlayer, ARMWASHSAVEDATA *pstCSWash);
NEWADDATTRDEF* z_find_new_addattr_def(ZONESVRENV *pstEnv, int iID);

//�ؼ��ָ���Ч��ֵ
int arm_addattr_result_addval_get(ZONESVRENV* pstEnv, ITEMDEF *pstItemDef, int iAddAttrID, int iVal);
double arm_addattr_result_addval_get_double(ZONESVRENV* pstEnv, ITEMDEF *pstItemDef, int iAddAttrID, int iVal);

int arm_hurt_mul_get(ZONESVRENV *pstEnv, AtkerInfo *pstAtkerInfo, AtkeeInfo *pstAtkeeInfo);

int player_kill_restore_hp_mp(ZONESVRENV* pstEnv, AtkerInfo *pstAtkerInfo, ZoneAni *pstAtkee);

int player_addattr_dec_wearlevel(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEARM *pstRoleArm, ITEMDEF *pstItemDef);

int player_addattr_heavyhit_add(AtkerInfo *pstAtkerInfo, AtkeeInfo *pstAtkeeInfo);

//����Ч�� Ŀ��Ѫ�������������ӱ����ʰٷֱ�
int skill_result_add_heavyhit_rate(SKILLDEF *pstSkillDef, AtkeeInfo *pstAtkeeInfo);
ADDATTRRANDDEF* z_find_addattr_rand_def(ZONESVRENV *pstEnv, int iArmLevel, int iImportant);
int wash_addattr_online_insert(ZONESVRENV* pstEnv, Player *pstPlayer, 
	ADDATTRRANDDEF *pstAddAttrRandDef, int iAddAttrID, int iInsertOnline, ROLEARM *pstArm);
OneAddAttrList* add_attr_one_list_get(ZONESVRENV *pstEnv, ITEMDEF *pstItemDef, int fWash);
int add_attr_num_rand_get(ZONESVRENV *pstEnv, ITEMDEF *pstItemDef);
int item_add_attr_insert(ZONESVRENV *pstEnv, ROLEARM *pstRoleArm, int iAddAttrID);
int add_attr_career_idx_get(ITEMDEF *pstItemDef);
int add_attr_wearpos_idx_get(ITEMDEF *pstItemDef);
#endif /* ZONE_ADDATTR_H */

