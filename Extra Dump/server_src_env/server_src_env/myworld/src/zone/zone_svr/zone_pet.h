
#ifndef ZONE_PET_H
#define ZONE_PET_H

#include "zone_svr.h"

PETDEF *z_find_pet_def(ZONESVRENV *pstEnv, int iPetID, int *pIidx);

PETLEVELDEF *z_find_pet_level_def(ZONESVRENV *pstEnv, int iLevel, int *pIidx);

PETTRAITDEF *z_find_pet_trait_def(ZONESVRENV *pstEnv, int iPetTraitID, int iStar);
ROLEPET *z_find_role_pet(Player *pstPlayer, tdr_ulonglong ullWID, int *piIdx);

PETSTRENGTHENDEF *z_find_pet_strengthen_def(ZONESVRENV *pstEnv, int iLevel);

RolePetOnline *pet_find_online(Player *pstPlayer, ROLEPET *pstRolePet);

PetExpOnline *pet_find_exponline(Player *pstPlayer, ROLEPET *pstRolePet);
PetExpOnline *pet_find_exponline_byonline(Player *pstPlayer, ROLEPET *pstRolePet, RolePetOnline *pstRolePetOnline);
PetPowerOnline *pet_find_poweronline(Player *pstPlayer, ROLEPET *pstRolePet);
PetPowerOnline *pet_find_poweronline_byonline(Player *pstPlayer, ROLEPET *pstRolePet, RolePetOnline *pstRolePetOnline);
PetFightAttrOnline *pet_find_fightattronline(Player *pstPlayer, ROLEPET *pstRolePet);
PetFightAttrOnline *pet_find_fightattronline_byonline(Player *pstPlayer, ROLEPET *pstRolePet, RolePetOnline *pstRolePetOnline);


int  get_pet_shi_xiedaipos(ZONESVRENV *pstEnv, Player * pstPlayer, ROLEPET* pstRolePet);
int pet_hp_action(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, int iOldHP);

int package_pet_use_hatch(ZONESVRENV * pstEnv, Player * pstPlayer, RESULT * pstResult);

int pet_hatch_check(ZONESVRENV* pstEnv, Player *pstPlayer,  RESULT *pstResult, ITEMDEF *pstItemDef);

int pet_add_power(ZONESVRENV* pstEnv, Player *pstPlayer,ROLEPET *pstRolePet,  int iPower);

ROLEPET* pet_hatch(ZONESVRENV* pstEnv, Player *pstPlayer,  int iPetID, int iTrait);

int pet_update_to_clt(ZONESVRENV *pstEnv,Player *pstPlayer, ROLEPET *pstRolePet);

int pet_love_val_chg(ZONESVRENV *pstEnv,Player *pstPlayer, ROLEPET *pstRolePet, int iLoveVal);

int pet_set_stat(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, int iStat);
int pet_genius_levelup_check(ZONESVRENV * pstEnv,ROLEPET *pstRolePet, PETDEF *pstPetDef, PETWASHGENIUSDEF *pstPetWashGeniusDef);
int pet_update_online(ZONESVRENV *pstEnv, Player *pstPlayer, ROLEPET *pstRolePet);

int get_ran_skillgrid_type(ROLEPET *pstRolePet);

int mon_booty_assign_pet_resource(ZONESVRENV* pstEnv, MONSTERDEF *pstMonDef , 
										 Player *pstPlayer, unsigned int  uiExp, int TeamExpMul);

int pet_view(ZONESVRENV* pstEnv, Player *pstPlayer, CSROLEPETVIEW *pstRolePetView);

int z_add_pet_exp(ZONESVRENV *pstEnv,Player *pstPlayer, ROLEPET *pstRolePet, unsigned int uiExp);

int pet_fight_attr_to_clt(ZONESVRENV *pstEnv,Player *pstPlayer, ROLEPET *pstRolePet);

int pet_package_info(ZONESVRENV* pstEnv, Player *pstPlayer);

int open_pet_clt_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID);

int pet_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int pet_shi_active(ZONESVRENV* pstEnv, Player *pstPlayer);

int open_pet_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID, int iUIType);

int z_player_backup_pet(ZONESVRENV* pstEnv, Player *pstPlayer);
int z_player_restore_pet(ZONESVRENV* pstEnv, Player *pstPlayer);
int z_player_restore_pet_nowmap(ZONESVRENV* pstEnv, Player *pstPlayer);

PetFightShiOnline *pet_find_fight_online(Player *pstPlayer, ROLEPET *pstRolePet);

int  get_pet_tishen(ZONESVRENV *pstEnv, Player *pstPlayer, ROLEPET *pstRolePet);

PetTiShenOnline *pet_find_tishenonline_byonline(Player *pstPlayer, ROLEPET *pstRolePet, RolePetOnline *pstRolePetOnline);

int pet_set_passive(ZONESVRENV *pstEnv, Player *pstPlayer, PETCHGMODE *pstChgMode);

int pet_svr_act(ZONESVRENV* pstEnv, Player *pstPlayer, int iType);

ROLEPET *pet_get_camp(ZONESVRENV* pstEnv, Player *pstPlayer);

int pet_fuse_cancel(ZONESVRENV* pstEnv, Player *pstPlayer);

int pet_del(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong PetWid, int fOplog, ITEMDEC *pstGetItem);

int pet_native_skill_result(ZONESVRENV* pstEnv, Player *pstPlayer,int iEvent, int* pValue,ROLEPET *pstRolePet);

int pet_strengthen_val_get(ROLEPET *pstRolePet);

int pet_wash_genius_money_get(ROLEPET *pstRolePet);
int pet_heti(ZONESVRENV* pstEnv, Player *pstPlayer);
int pet_heti_attr_move(ZONESVRENV* pstEnv, Player *pstPlayer);
int pei_heti_tick(ZONESVRENV* pstEnv, Player *pstPlayer);
int is_pet_heti(Player *pstPlayer);
tdr_ulonglong is_login_pet_heti(ZONESVRENV* pstEnv,Player *pstPlayer);
int pet_heti_in(ZONESVRENV* pstEnv, Player *pstPlayer,ROLEPET *pstRolePet, int iStart);
int pet_heti_check(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET **ppstRolePet);

int pet_wash_genius(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETWASHGENIUS *pstCSWashInfo);
int pet_wash_genius_save(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETWASHGENIUSSAVE *pstCSSave);
int pet_fight_addskill(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, PETSKILLGRID *pstPetSkillOne);
int pet_fight_val(ZONESVRENV* pstEnv, ROLEPET *pstRolePet, PETDEF *pstPetDef);
int pet_love_level_up_in(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet,
	int iLevel,PETDEF *pstPetDef,int iNotice );
int pet_love_level_up(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETLOVELEVELUP *pstCSSave);

int pet_eat(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETEAT *pstCSEat);

int pet_move_star(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETMOVESTAR *pstCSMove);

int get_max_pet_wash_genius(PETWASHGENIUSDEF *pstPetWashGeniusDef, int iDefVal);

PETWASHGENIUSDEF *z_find_pet_wash_genius_def(ZONESVRENV *pstEnv, int iImportant, int iPinZhi);

int player_revive_pet_update(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_die_pet_add_power(ZONESVRENV* pstEnv, Player *pstPlayer);
int pet_role_login_check(ZONESVRENV* pstEnv, Player *pstPlayer);
int pet_genius_learn(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETGENIUSLEARN *pstLearn);
int pet_genius_up_high(ZONESVRENV * pstEnv, Player *pstPlayer, CSPACKAGEUSE *pstUse);
int pet_genius_up_high_check(ZONESVRENV * pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef,CSPACKAGEUSE *pstUse);
ROLEPET * pet_find_fight(Player *pstPlayer);
int pet_native_add_money(ZONESVRENV* pstEnv, Player *pstPlayer);

int pet_unbind(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETUNBIND *pstPetUnbind);
int pet_unbind_money_val_by_genius(int PetGenius,int *piMoney);


#endif /* ZONE_PET_H */
