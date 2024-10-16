#ifndef ZONE_ARM_H
#define ZONE_ARM_H

#include "zone_svr.h"

typedef struct  
{
	unsigned int ItemID;
	unsigned char Num;
}ConvergeItem;

typedef struct  
{
	int iItemID;              
	int iNum;
	int iListType;                        	/*    Bind Macrosgroup:LIST_TYPE,*/
	int iPos;
}StuffGridTmp;

typedef struct  
{
	int iNum;
	StuffGridTmp stGrid[8];
}TotalStuffGridTmp;


#define GEM_MAX_STAR 5  // 最大5星
#define GEM_HECHENG_LEVEL 3 // 大于等于4级宝石必须充能
#define GEM_MAX_LVL 7     	// 宝石最高等级

int player_arm(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

//装备品质,战斗力计算
int  z_arm_star_fight(ZONESVRENV* pstEnv, DYNATTRLIST *pstDyn, ROLEARM *pstRoleArm);

//装备宝石属性计算
int  z_arm_gem(ZONESVRENV* pstEnv, DYNATTRLIST *pstDyn, ROLEARM *pstRoleArm);

//装备加工UI
int open_arm_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID, int iUIType);

int items_total(ConvergeItem *pstGemItems, int *piNum,  unsigned int iItemID);

int z_get_arm_result_total(ITEMDEF *pstItemDef, RESULT *pstResult, int iResultID);

int arm_add_hole(ZONESVRENV* pstEnv,RNDHOLE *pstRndHole, ROLEARM *pstArm);

int arm_wear_update(ZONESVRENV* pstEnv,  Player *pstPlayer, ROLEARM  *pstRoleArm);

ARMHIDEATTRDEF *armhideattr_def_find(ZONESVRENV *pstEnv, short Career, short ArmStarLevel);

ARMRANDDEF *armrand_def_find(ZONESVRENV *pstEnv, unsigned short LevelType, unsigned short Level);

int z_arm_hideattr_calc(ZONESVRENV* pstEnv, Player *pstPlayer);

int arm_fengyin_in(ZONESVRENV* pstEnv, Player *pstPlayer, int iIdx,Player *pstKiller,int iListType);

int arm_fenjie_pre_brk(ZONESVRENV* pstEnv, Player* pstPlayer, int iArmFenJiePreBrk);
GEMNEWDEF* z_find_gemnewdef(ZONESVRENV *pstEnv, int iItemID);
GEMATTRSTARDEF* z_find_gemattrstardef(ZONESVRENV *pstEnv, short iAttrID, char cStar,char cLevel);
GEMLEVELSTARDEF* z_find_gemlevelstardef(ZONESVRENV *pstEnv, short iLevel, short iStar);

int gem_level_up(ZONESVRENV* pstEnv, Player *pstPlayer, GEMLEVELUPDATA *pstGemLevelUp);
int arm_reset_gem(ZONESVRENV* pstEnv, Player *pstPlayer, GEMRESET *pstGemReSet);
int gem_add_power(ZONESVRENV* pstEnv, Player *pstPlayer, GEMADDPOWER *pstGemAddPower);
int arm_fenjie(ZONESVRENV* pstEnv,Player* pstPlayer,ARMFENJIE* pstArmFenJie);
USEDEF* z_find_usedef(ZONESVRENV *pstEnv, int iID);

int arm_star_perfect_num_get(ROLEARM*pstRoleArm);

int arm_star_mul_idx_get(ZONESVRENV* pstEnv, ROLEARM *pstRoleArm, int iStar);

int get_arm_itemlevel(int RoleWearLevel);

ROLEGRID *get_pak_item_grid(Player *pstPlayer, char cListType, unsigned short unGridIdx);
int arm_one_grid_update(ZONESVRENV* pstEnv,  Player *pstPlayer, ROLEGRID *pstArmGrid);
int player_cacl_spar_attr(ZONESVRENV* pstEnv, Player *pstPlayer);

int is_fengyin_arm(ROLEARM *pstRoleArm);

int tmp_grid_total(TotalStuffGridTmp *pstGridTmp, int iItemID, int iItemNum, int iPos, int iListType);
int arm_attr_is_fengyin(ROLEARM *pstRoleArm, int iIdx, int iFlag);
int wear_has_one_fengyin(Player *pstPlayer);
int spar_hole_check(ZONESVRENV* pstEnv, Player *pstPlayer);
SPARDEF *z_find_spardef(ZONESVRENV *pstEnv, int iItemID);

int z_arm_hideattr_calc_one(ZONESVRENV* pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef, ROLEARM *pstRoleArm);

int z_wear_get_star_perfect_num(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_arm_attr_move(ZONESVRENV* pstEnv, Player *pstPlayer, ARMATTRMOVE*pstMove);
int player_arm_attr_reset(ZONESVRENV* pstEnv, Player *pstPlayer, ARMATTRRESET *pstReset);
int player_arm_attr_active(ZONESVRENV* pstEnv, Player *pstPlayer, ARMATTRACTIVE *pstActive);
int player_arm_check(ZONESVRENV* pstEnv, Player *pstPlayer, ARMCHECK*pstCheck);
int arm_is_uncheck(ROLEARM *pstArm);
int item_one_grid_update_ex(ZONESVRENV* pstEnv,  Player *pstPlayer, ROLEITEM*pstRoleItem, int iListType);
ARMSTEPUPDEF*z_find_arm_stepup_def(ZONESVRENV *pstEnv, int iLvl, int iWpos, int iImp);
int  z_arm_step_attr_calc(ZONESVRENV* pstEnv, DYNATTRLIST *pstDyn, ROLEARM *pstRoleArm, ITEMDEF *pstDef);
//lzk
int arm_is_badge(ITEMDEF *pstItemDef);
int player_arm_base_reset(ZONESVRENV* pstEnv, Player *pstPlayer, ARMBASERESET*pstReset);
int player_arm_base_reset_save(ZONESVRENV* pstEnv, Player *pstPlayer, ARMBASERESETSAVE*pstSave);
int arm_create_skill_num(ZONESVRENV* pstEnv,ROLEARM *pstArm, ITEMDEF *pstItemDef);
int arm_skill_wpos_get_mul(ARMSKILLATTRDEF *psAttrDef, ITEMDEF *pstItemDef);
ARMSKILLATTRDEF*z_find_arm_skillattr_def(ZONESVRENV *pstEnv, int iID);
int arm_skill_wpos_get_mul(ARMSKILLATTRDEF *psAttrDef, ITEMDEF *pstItemDef);
int arm_skill_fast_init(ZONESVRENV* pstEnv);
int gem_is_chongneng(ITEMDEF *pstItemDef, ROLEITEM *pstItem);
int gem_level_up_baoxian_val(short ResultVal1,int *piMoney);
int arm_calc_teshu_attr(ZONESVRENV* pstEnv,Player *pstPlayer, ROLEARM *pstRoleArm,ITEMDEF *pstItemDef);
int arm_calc_teshu_buff(ZONESVRENV* pstEnv,Player *pstPlayer);
int arm_back_unreal(ZONESVRENV* pstEnv, Player *pstPlayer, ARMBACKUNREAL *pstCSBackUnreal);
int back_unreal_result_check(ZONESVRENV* pstEnv, Player *pstPlayer,ROLEARM *pstRoleArm);


#endif
