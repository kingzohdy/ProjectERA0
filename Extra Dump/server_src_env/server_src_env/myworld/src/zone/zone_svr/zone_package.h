
#ifndef ZONE_PACKAGE_H
#define ZONE_PACKAGE_H

#include "zone_svr.h"

#include "zone_zh_name.h"

#define ITEM_CHGNAME_ID 5009999
#define PURSUE_MIN_BUDDY_VAL 300   


// 返回一个玩家的契约ID，如没有返回0.
unsigned int z_qiyue_id_from_player( ZONESVRENV * pstEnv, Player * pstPlayer );

int player_package_req(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int package_add_test(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, const ROLEGRID *pstGrid, int iGridNum,int iShoppingFlag);

int package_add(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,  ROLEGRID *pstGrid, int iGridNum,int iShoppingFlag, int iWhatWay);

ITEMDEF *z_find_itemdef(ZONESVRENV* pstEnv, int iItemID);

ITEMDEF *z_get_itemdef_roleitem(ZONESVRENV* pstEnv, ROLEITEM *pstRoleItem);

//打开仓库
int open_stash(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iID);

int close_stash(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);

int item_create(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ROLEGRID *pstRoleGrid, int iIndex);

//增加零时仓库有效时间, iTime 单位秒
int package_tmp_time(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iTime);

//背包扩容， iListType 指示哪个背包栏， iNum 表示扩容几格
int package_enlarge(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iListType, int iNum);

int player_wear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSWEAR *pstWear);
int player_unwear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSWEAR *pstUnWear);
int z_update_avatar(ZONESVRENV* pstEnv, Player *pstPlayer);
int z_check_player_package(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int player_die_wear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iDurRate);

int package_dec_test(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID, int iNum, int iListType, int *piPos,int iDecType);
int package_dec(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID, int iNum, int iListType, int *piPos,int iDecType, int iWhatWay);

int find_grid_of_package(ROLEGRID*pstPackageRoleGrid, int iNum,ITEMDEF *pstItemDef, int fBind, int iGemStat);
int find_item_of_package(ROLEITEM *pstItem, int iNum,ITEMDEF *pstItemDef, int fBind);
int package_grid_cmp(const void *temp1, const void *temp2);
int package_item_cmp(const void *temp1, const void *temp2);
//int package_item_arrange(ZONESVRENV* pstEnv,ROLEITEM *pstRoleItem, unsigned short *iNum,ROLEPACKAGE *pstPak,char cListType);
int package_grid_arrange(ZONESVRENV* pstEnv,ROLEGRID *pstRoleGrid, unsigned short *punNum, 
				unsigned short unMax, ROLEPACKAGE *pstPak,char cListType);

int package_item_arrange(ZONESVRENV* pstEnv,ROLEITEM *pstRoleItem, unsigned short *punNum, 
				unsigned short unMax, ROLEPACKAGE *pstPak,char cListType);


//int package_grid_arrange(ZONESVRENV* pstEnv,ROLEGRID *pstRoleGrid, unsigned short *iNum,ROLEPACKAGE *pstPak,char cListType);

int z_get_pak_item_num(ZONESVRENV* pstEnv, ROLEPACKAGE *pstPak, int iItemID, int iListType,int iGnore,int iDecType);
int package_add_in(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, DYNATTRLIST *pstDyn,  ROLEPACKAGE *pstPlayerPackage, ROLEGRID *pstGrid, ROLEPACKAGECHG *pstPackageChg);

int package_dec_money_test(ZONESVRENV* pstEnv, Player *pstPlayer, int iMoneyType, unsigned int uiMoney);
int package_dec_money(ZONESVRENV* pstEnv, Player *pstPlayer, int iMoneyType, unsigned int uiMoney);
int package_add_money(ZONESVRENV* pstEnv, Player *pstPlayer, int iMoneyType, unsigned int uiMoney);

int package_itemgrid_pos(ROLEITEM*pstPackageRoleItem, int iNum, int iGridIdx);
int package_grid_pos(ROLEGRID*pstPackageRoleGrid, int iNum, int iGridIdx);

int z_arm_repair_price(ZONESVRENV* pstEnv, ROLEARM *pstRoleArm, ITEMDEF *pstItemDef,Player* pstPlayer);
int z_arm_sell_price(ZONESVRENV* pstEnv, ROLEARM *pstRoleArm, ITEMDEF *pstItemDef);

ROLEARM *z_wear_get(ROLEWEAR *pstRoleWear, int iItemDefID, char cWearPos);
int wear_arm_pos(ROLEARM*pstRoleArms, int iNum, int iGridIdx);

int z_arm_dur_dec(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEARM *pstRoleArm, int iDurDec);

int player_package_check_mapitem(ZONESVRENV* pstEnv, Player *pstPlayer);

RESULT *z_item_get_result(ITEMDEF *pstItemDef, int iResultID);

int package_get_item(ZONESVRENV* pstEnv, Player *pstPlayer, int iListType,
						int iGridIdx, ITEMDEF **ppstItemDef, ROLEITEM **ppstRoleItem,
						int *piPos);

//void z_check_ride_use( ZONESVRENV* pstEnv,Player *pstPlayer);
int item_result(ZONESVRENV* pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef,  
ZoneAni *pstTarAni, CSPACKAGEUSE *pstUse,ROLEITEM *pstRoleItem);

void ride_list_chg( ZONESVRENV* pstEnv,Player *pstPlayer);

int pet_wear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSWEAR *pstCSWear);
int pet_unwear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSWEAR *pstCSUnWear);

int check_npc_ui(ZONESVRENV* pstEnv, Player *pstPlayer, int iUIType);
int package_dec_in_grid(int iItemID, ROLEGRID *pstRoleGrids, unsigned short *punNum, int iListType, unsigned short *piItemNum, int *piPos, ROLEPACKAGECHG *pstRolePakChg,int iDecType);
int package_dec_in_item(int iItemID, ROLEITEM *pstRoleItems, unsigned short *punNum, int iListType, unsigned short *piItemNum, int *piPos, ROLEPACKAGECHG *pstRolePakChg,int iDecType);
int package_add_money_test(ZONESVRENV* pstEnv, Player *pstPlayer, int iMoneyType, unsigned int uiMoney);

//物品流通检查
int package_current_item_check(ZONESVRENV *pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef, ROLEITEM *pstItem);

int  player_skill_cool_clean(ZONESVRENV* pstEnv, Player *pstPlayer, unsigned short unSkillID);

int z_find_arm_dyn_skill(Player *pstPlayer, unsigned short unSkillID);
int z_arm_skill_update(ZONESVRENV* pstEnv, Player *pstPlayer);

int z_birth_avatar(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,ROLEAVATARPROF *pstAvatar, 
					unsigned char ucCareer, unsigned char ucGender,int iApperalIdx);
int z_player_add_gift(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int item_call_mon(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEMAPINST *pstMapInst,int iMonNum, int iMonID, int iRadius, int iLifeTime,int iX,int iY, int iPatrol, int iDir, int iBctDefID);

int z_arm_extra_dur(ITEMDEF *pstItemDef);

int z_get_arm_max_dur(ZONESVRENV* pstEnv, ITEMDEF *pstItemDef);

int player_ride_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

RIDEDEF *z_find_ride_def(ZONESVRENV *pstEnv, unsigned short iRideID, unsigned short iRideLevel);

RIDENPCSELECT *z_get_ride_npc(RIDEDEF *pstRideDef, int iSelectType);

int is_hatch_player_ride(Player *pstPlayer, Player *pstRidePlayer);

int z_leave_ride(ZONESVRENV* pstEnv, Player *pstRidePlayer, Player *pstPlayer);

int player_item_do_qiyue(ZONESVRENV* pstEnv, Player *pstPlayer,  RESULT *pstResult);

int player_item_do_qiyue_check(ZONESVRENV* pstEnv, Player *pstPlayer,  RESULT *pstResult);

int z_player_same_ride(ZONESVRENV* pstEnv, Player *pstPlayer1, Player *pstPlayer2);

//int package_fittings_inlay_check(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,CSPACKAGEUSE *pstUse, RESULT *pstResult);

int package_grid_update_to_clt(TAPPCTX * pstAppCtx, ZONESVRENV * pstEnv, Player * pstPlayer, int iListType, ROLEITEM * pstRoleItem, ROLEARM * pstRoleArm);

int package_add_grid_in(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ROLEGRID *pstRoleGrids, unsigned short *punNum, 
							int iMaxNum, int iListType, ROLEGRID *pstGrid, ITEMDEF *pstItemDef, 
							ROLEPACKAGECHG *pstPackageChg, ROLEPACKAGE *pstPkg);

int z_get_skill_atklist(ZONESVRENV* pstEnv, Player *pstPlayer, SKILLDEF *pstSkillDef, ZoneAni *pstTarAni, ZoneObjList *pstZoneObjList, RESPOS *pstHitPos);

USEITEMONEDAYINFO* z_get_item_oneday_info(ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID);

int item_add_oneday(ZONESVRENV *pstEnv, Player *pstPlayer, int iItemID);

//通知客户端自动打开拾取的对话框 
int package_booty_view( ZONESVRENV* pstEnv, Player *pstPlayer, unsigned int  DropBootyID);

int open_coin_to_bull_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iID);

int player_coin_to_bull(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int task_award_item_create(ZONESVRENV *pstEnv,
                           const TASKITEMAWARD *pstAwardItem,
                           ROLEGRID *pstRoleGrid);

int money_type_to_string(int iMoneyType, char* pszBuff, int iSize);

int z_fill_link_msg(CSPKG *pstPkg, int iSysType, int iLinkType, void *pstLinkData, int iErrno, ...);

int ride_clear_passenger(ZONESVRENV* pstEnv, Player *pstPlayer);

//ret -1表示没有装备枪手的左手武器,0表示装备了枪手的左手武器
int check_gunner_unwear_pos_left(ZONESVRENV* pstEnv, Player *pstPlayer,ITEMDEF *pstUnwearItemDef);
// 把枪手的左手装备卸下来
int package_gunner_unwear_pos_left(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int  iPos);
//获得枪手技能的左手频数
int package_get_guner_wear_left_continuenum(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,SKILLDEF *pstSkillDef);

int check_gunner_wear_pos_left(ZONESVRENV* pstEnv, Player *pstPlayer,ITEMDEF *pstWearItemDef);

ROLEARM *get_player_collect_arm(ZONESVRENV* pstEnv, ROLEPACKAGE *pstPak, MONCOLLECT *pstMonsterCollectDef);

//是否骑乘状态
int is_player_on_ride(Player *pstPlayer);

//角色骑乘携带的乘客数量
int player_get_ride_passenger_num(Player *pstPlayer);

int player_extend_item(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
                       Player *pstPlayer, TFRAMEHEAD *pstFrameHead,
                       CSPKG *pstCsPkg);

int z_wear_extended_bit_set(ZONESVRENV *pstEnv, Player *pstPlayer,
                            int iWearIdx);

int z_extend_item_property(ZONESVRENV *pstEnv, int iDefID, int iWhich);

int z_extend_item_from_item(Player *pstPlayer, const ITEMDEF *pstBeUsedDef,
                            ROLEGRID *pstBeExtended);

int package_maxnum_by_type(ROLEDATA *pstRoleData, int iListType);

int package_get_freegrid_num(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, char cListType);

int package_cangbaotu_get_pos(ZONESVRENV *pstEnv,Player *pstPlayer, unsigned int ItemID,
										unsigned  int *piMapID,RESPOS *pstPos);
int package_cangbaotu_check(ZONESVRENV *pstEnv,Player *pstPlayer);
int package_cangbaotu_reset(ZONESVRENV *pstEnv,Player *pstPlayer, CSPACKAGECANGBAOTURESET *pstReset);
int package_cangbaotu_send( ZONESVRENV* pstEnv, Player *pstPlayer);
int package_cangbaotu_tick( ZONESVRENV* pstEnv, Player *pstPlayer);

int z_wear_del(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEARM *pstRoleArm, ROLEPACKAGECHG *pstPackageChg);

int package_item_info_by_type(ROLEDATA *pstRoleData, int iListType,  SSGETPACKAGEITEMINFO *pstGetPackageItemInfo);

int get_vaild_gird(ZONESVRENV * pstEnv, ROLEPACKAGE *pstPkg, int iPkgType);
int gridIdx_is_unvaild(ZONESVRENV* pstEnv,ROLEPACKAGE *pstPak,char cListType,int iGridIdx);

int is_money_booty_grid(ROLEGRID *pstRoleGrid);
int package_get_free_grid(ROLEGRID*pstPackageRoleGrid, int iNum, int iMaxNum);
int role_pkg_num_chg(ZONESVRENV* pstEnv, Player* pstPlayer, int iPkgType);

int role_pkg_add_grid(ZONESVRENV* pstEnv, Player* pstPlayer, int iPkgType, int iNum);

int get_pkg_basenum(ZONESVRENV* pstEnv, Player* pstPlayer, int iListType);
int player_keyin_item(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
                       Player *pstPlayer, TFRAMEHEAD *pstFrameHead,
                       CSPKG *pstCsPkg);

int is_same_roleitem(ZONESVRENV* pstEnv,ROLEITEM *pstRoleItem1, ROLEITEM *pstRoleItem2);

int is_bind_roleitem(ROLEITEM *pstRoleItem);
int player_login_check_pkgbug(ZONESVRENV* pstEnv, Player *pstPlayer);

int z_player_add_and_wear_arm(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, 
									Player *pstPlayer, ITEMDEF *pstItemDef,ROLEGRID *pstRoleGrid);

int package_use_pre_brk(ZONESVRENV * pstEnv, Player * pstPlayer);
int arm_is_avatar(ITEMDEF *pstDef);
int arm_is_fashion(ITEMDEF *pstDef);
int arm_is_head(ITEMDEF *pstDef);
int arm_is_face(ITEMDEF *pstDef);


int package_use_clan_item(ZONESVRENV* pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef);

int package_pick(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSPACKAGEPICK *pstPick);
void itemdec_fill(ITEMDEC *pstItemDec, int iID, int iNum, int iBind);
int z_pkg_check_clanlimit(ZONESVRENV* pstEnv, Player *pstPlayer,char cLimit);
int pkg_get_pos(Player *pstPlayer, int iListType, int iGridIdx);
int spar_item_create(ZONESVRENV* pstEnv, ROLEARM*pstArm, ROLEITEM *pstRoleItem);
int pkg_num_chg(ZONESVRENV* pstEnv, Player *pstPlayer, int iListType);
int arm_is_back(ITEMDEF *pstDef);

int player_daily_bull_action(ZONESVRENV* pstEnv, Player *pstPlayer);

//假死取消
int player_sham_dead_clear(ZONESVRENV* pstEnv, Player *pstPlayer);

//假死
int player_sham_dead(ZONESVRENV* pstEnv, Player *pstPlayer);

//彩色字体时效
int player_color_font_limit_action(ZONESVRENV* pstEnv, Player *pstPlayer);

//动作表情时效
int player_motion_limit_action(ZONESVRENV* pstEnv, Player *pstPlayer);

int package_use_color_font_limit(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult);

int package_use_motion_limit(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult);
int package_add_by_id(ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID, int iNum, int iBind);

int open_shop_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, int iSellListID, int iVip);


int is_valid_normal_fashion(Player *pstPlayer, int iWearPos);

int is_valid_back(Player *pstPlayer, int iWearPos);

int arm_is_normal_fashion(ITEMDEF *pstDef);

EXCHGITEMDEF *z_find_exchg_item_def(ZONESVRENV *pstEnv, int iID);

int player_extend_item_save(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
                       Player *pstPlayer, TFRAMEHEAD *pstFrameHead,
                       CSPKG *pstCsPkg);
int package_add_msg_show(ZONESVRENV* pstEnv, char*pszName, ITEMDEF *pstItemDef,ROLEGRID *pstRoleGrid);
int package_use(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, CSPACKAGEUSE *pstUse);

int check_pursue_item(ZONESVRENV* pstEnv, Player *pstPlayer,Player *pstTempPlayer,ITEMDEF *pstItemDef);
int check_buddy_val(ZONESVRENV* pstEnv, Player *pstPlayer,Player *pstTempPlayer,ITEMDEF *pstItemDef);

GEMPAGDEF *gem_pag_def_find(ZONESVRENV* pstEnv, int Id);
int item_use_gem_pag_check(ZONESVRENV* pstEnv, Player *pstPlayer,CSPACKAGEUSE *pstUse,int ID);
int item_use_gem_pag(ZONESVRENV* pstEnv, Player *pstPlayer, CSPACKAGEUSE *pstUse,int ID);

int player_wear_skill_cool_left(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,ZoneSkill *pstZoneSkill, unsigned int *puiCoolLeft);
int player_wear_skill_cool_update(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int player_unwear_skill_cool_update(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,ZoneSkill *pstZoneSkill);




#endif /* ZONE_PACKAGE_H */

