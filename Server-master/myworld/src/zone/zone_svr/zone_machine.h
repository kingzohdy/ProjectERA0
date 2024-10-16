#ifndef ZONE_MACHINE_H
#define ZONE_MACHINE_H

#include "zone_svr.h"
#include "zone_npc.h"


#define RESET_DART_CAR_ITEM 5060023

enum tagMachineDynAttrType
{
	 MACHINE_DYNATTR_TYPE_ATK = 0,
	 MACHINE_DYNATTR_TYPE_HP,
	 MACHINE_DYNATTR_TYPE_ICEDEF,
	 MACHINE_DYNATTR_TYPE_PHSICDEF,
};

static inline MachineData* this_machine(ZONESVRENV *pstEnv,Monster *pstMon) 
{
	z_refresh_mon_comm_union_data(pstEnv, pstMon, MON_UNION_TYPE_MACHINE);
	return &pstMon->stCommUnion.stMachine; 
}

MONMACHINEDEF *z_find_mon_machine_def(ZONESVRENV *pstEnv, int  iMonID);

int z_mon_machine_move(ZONESVRENV* pstEnv, Player *pstPlayer);

int z_machine_attr_chg(ZONESVRENV * pstEnv, Monster *pstMon, int iType, int iVal);

int z_ride_machine_clear(ZONESVRENV * pstEnv, Monster *pstMachineMon);

int z_ride_on_machine(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMachineMon, int iSeatIdx, tdr_ulonglong ullItemWid);

int z_machine_move_notify_one(ZONESVRENV* pstEnv, Monster *pstMonMachine, Player *pstVPlayer);

int z_passenger_move_notify_one(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstVPlayer);

int get_machine_attr_val(ZONESVRENV* pstEnv,Monster *pstMon, int iType);

int mon_machine_back(ZONESVRENV * pstEnv, Monster *pstMachine);

int mon_machine_data(ZONESVRENV * pstEnv, Monster *pstMachine);

int mon_machine_back(ZONESVRENV * pstEnv, Monster *pstMachine);

int mon_machine_die(ZONESVRENV * pstEnv, Monster *pstMachine, ZoneAni * pstAtker);

Player * machine_get_driver(ZONESVRENV* pstEnv, Monster *pstMonMachine);

int machine_chg_map(ZONESVRENV * pstEnv, Monster *pstMon, int iMapID, RESPOS *pstPos);

int machine_cacl_dynattr(ZONESVRENV* pstEnv, Monster *pstMachineMon);

int machine_get_free_seatidx(ZONESVRENV* pstEnv,Monster *pstMachineMon);


int  is_clan_machine(ZONESVRENV* pstEnv,Monster *pstMachineMon);
	
int is_mon_machine(Monster *pstMachineMon);

int is_empty_machine(ZONESVRENV* pstEnv,Monster *pstMachineMon);

int is_player_ride_on_machine(Player *pstPlayer);

int is_player_driver_machine(ZONESVRENV* pstEnv, Player *pstPlayer);

int is_player_normal_passenger_machine(ZONESVRENV* pstEnv, Player *pstPlayer);

int is_collect_machine(ZONESVRENV* pstEnv,Monster *pstMachineMon);

int is_together_ride_machine(Player *pstPlayer1, Player *pstPlayer2);

int is_single_mon_machine(ZONESVRENV* pstEnv, Monster *pstMachineMon);

int is_collect_machine(ZONESVRENV* pstEnv,Monster *pstMachineMon);

int player_machine_atk(ZONESVRENV * pstEnv, Player * pstPlayer, CSSKILLATK *pstSkillAtk);

int player_ride_machine_brk(ZONESVRENV * pstEnv, Player * pstPlayer, int iMust);

int player_ride_machine(ZONESVRENV* pstEnv, Player *pstPlayer,  int iMachineID, int iSeatIdx);

int player_machine_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
			TFRAMEHEAD	 *pstFrameHead, CSPKG *pstCsPkg);

int player_new_machine_and_ride(ZONESVRENV *pstEnv, Player *pstPlayer,
                                int iMonMachineID, int iEnergy, RESPOS *pstPos);

Monster* machine_get_by_player(ZONESVRENV* pstEnv, Player *pstPlayer);
Monster* new_mon_machine(ZONESVRENV * pstEnv, ZONEMAPINST *pstMapInst, 
	int iMonID, RESPOS *pstPos, int iOwnerType, const void *pstOwnerData, tdr_ulonglong ullWid);
Monster* new_mon_machine_dir(ZONESVRENV * pstEnv, ZONEMAPINST *pstMapInst, 
	int iMonID, RESPOS *pstPos, int iOwnerType, const void *pstOwnerData, unsigned char bDir, tdr_ulonglong ullWid,
	int iBuildDefID, int iLeftTime);
	
int machine_set_fight(ZONESVRENV* pstEnv, Monster *pstMachineMon);

int machine_item_update(ZONESVRENV* pstEnv, Player *pstPlayer);

int machine_ore_idx2type(int iIdx);

int machine_get_ore_idx_byid(MONMACHINEDEF *pstMachineDef, int iID);

int machine_ores_sum(ZONESVRENV * pstEnv, Monster *pstMon);

int package_use_machine_item(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, CSPACKAGEUSE *pstUse);

int package_use_machine_item_check(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEITEM *pstRoleItem,
	ITEMDEF *pstItemDef, RESULT *pstResult);

int player_machine_commit_ore(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEGRID *pastRoleGrid, int iNum);

int player_machine_commit_ore_check(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEGRID *pastRoleGrid, int *piNum);

int machine_item_create(ZONESVRENV* pstEnv, RESULT *pstResult, ROLEITEM *pstRoleItem);

int is_mon_big_ore(int iOreDefID);

int is_mon_big_oil(int iiOreDefID);

int is_mon_factory(Monster *pstMon);

//矿石更新
int mon_ore_update(ZONESVRENV* pstEnv, Monster *pstMon);

//工厂更新
int mon_factory_update(ZONESVRENV* pstEnv, Monster *pstMon);

int mon_clan_factory_del(ZONESVRENV* pstEnv, tdr_ulonglong ullClanGID);

int mon_factory_tick(ZONESVRENV* pstEnv, Monster *pstMon);

WORLDOREINFO *global_world_ore_get_byid(ZONESVRENV* pstEnv, int iDefID);

int global_world_ore_add(ZONESVRENV * pstEnv, Monster *pstMon);

int global_world_ore_refresh(ZONESVRENV* pstEnv);

int global_world_ore_new_check(ZONESVRENV * pstEnv, int iOreDefID);

Monster* new_mon_factory(ZONESVRENV * pstEnv, Player *pstPlayer,
	Monster *pstOreMon, int iFactoryID, int iRadius);

int package_use_factory_item_check(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, int iOreMemID);

int package_use_factory_item(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult, int iOreMemID);

int global_world_ore_del(ZONESVRENV* pstEnv, Monster *pstMon);

int package_use_machine_energy_check(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult);

int package_use_machine_energy(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult);

int package_use_machine_hp_check(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult);

int package_use_machine_hp(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult);

int machine_pkval_update(ZONESVRENV* pstEnv, Monster *pstMachineMon);

int machine_pk_time_action(ZONESVRENV* pstEnv, Monster *pstMachineMon);

int player_machine_leave_pre_brk(ZONESVRENV * pstEnv, Player * pstPlayer);

int machine_item_action(ZONESVRENV* pstEnv, Player *pstPlayer);

int machine_passenger_clear_skill(ZONESVRENV * pstEnv, Player * pstPlayer, Monster *pstMachineMon);

int machine_passenger_update_skill(ZONESVRENV * pstEnv, Player * pstPlayer, Monster *pstMachineMon);

int player_ride_off_machine_target_update(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMachine);

int player_ride_on_machine_target_update(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMachine);

int machine_ride_on_pktime_update(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMachine);

int mon_factory_warning_byhit(ZONESVRENV *pstEnv, Monster *pstMon);

// 刷一个商运镖车
Monster* new_mon_dart_car(ZONESVRENV * pstEnv, ZONEMAPINST *pstMapInst, 
								int iMonID, RESPOS *pstPos, Player *pstPlayer,int iLeftTime);

DARTCARDEF * z_find_dartcar_def(ZONESVRENV *pstEnv, int  iLvl);
int new_mon_dart_car_by_task(ZONESVRENV * pstEnv, Monster *pstMachine, Player *pstPlayer, int iType, int iLeftTime);
int player_reset_dart_car(ZONESVRENV * pstEnv, Player *pstPlayer);
int player_is_in_dart_car(ZONESVRENV * pstEnv,Player *pstPlayer);
int player_ride_machine_clear(ZONESVRENV * pstEnv, Player * pstPlayer);
int machine_dart_car_login(ZONESVRENV *pstEnv, Player *pstPlayer);

int package_use_machine_status_check(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult);
int package_use_machine_status(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult);

//是不是镖车
int is_mon_biaoche(Monster *pstMon);
Monster* mon_biaoche_get_by_player(ZONESVRENV * pstEnv,Player *pstPlayer);
CARONE *z_find_carone1(DARTCARDEF *pstDef);
int player_resume_dart_car(ZONESVRENV * pstEnv, Player *pstPlayer, int iUseItemNum);

int player_machine_ballistic_hit(ZONESVRENV * pstEnv, Player * pstPlayer, CSSKILLATK *pstSkillAtk);

int z_passenger_update_pos(ZONESVRENV* pstEnv, MapIndex *pstMapIdx, ZONEMAPINST* pstMapInst, 
	Player *pstPlayer, RESPOS *pstPos);

Monster* new_mon_machine_dir_patrol(ZONESVRENV * pstEnv, ZONEMAPINST *pstMapInst, 
	int iMonID, RESPOS *pstPos, int iOwnerType, const void *pstOwnerData, unsigned char bDir, tdr_ulonglong ullWid,
	int iBuildDefID, int iLeftTime, int iPatrol);
int machine_passenger_info(ZONESVRENV* pstEnv, Monster *pstMachineMon);
#endif /* ZONE_MACHINE_H */

