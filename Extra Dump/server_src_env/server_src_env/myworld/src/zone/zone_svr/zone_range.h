
#ifndef ZONE_RANGE_H
#define ZONE_RANGE_H

#include "zone_svr.h"
#include "zone_db.h"

#define RANGE_STAT_FAIL -1
#define RANGE_STAT_INIT 0
#define RANGE_STAT_GET_COUNT_REQ 1
#define RANGE_STAT_GET_REQ 2
#define RANGE_STAT_DB_FINI 3
#define RANGE_STAT_CHG 4
#define RANGE_STAT_DB_UPDATE 5

int range_init(ZONESVRENV* pstEnv);
char *range_get_name(TOPRANGE *pstRange);
int range_set(ZONESVRENV* pstEnv, TOPRANGE *pstData);
int range_single_set(ZONESVRENV* pstEnv, Player *pstPlayer, int iSubType, unsigned int  uiVal);
int range_career_set(ZONESVRENV* pstEnv, Player *pstPlayer, unsigned int  uiVal);
int range_clan_set(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan, int iSubType, unsigned int  uiVal);
int range_pet_set(ZONESVRENV* pstEnv, Player *pstOwner, ROLEPET *pstRolePet, PETDEF *pstPetDef, 
				int iSubType, unsigned int  uiVal);

int range_do_chg(ZONESVRENV* pstEnv);

int player_range(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						     TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int range_count_res(ZONESVRENV *pstEnv, TORMSELECTCOUNTRES *pstSelectCountRes);
int range_tick(ZONESVRENV* pstEnv);
int range_select_res(ZONESVRENV *pstEnv, TORMSELECTRES *pstSelectRes);
int range_delete_res(ZONESVRENV* pstEnv, TORMPKG *pstTormPkg); 
int range_update_res(ZONESVRENV* pstEnv, TORMPKG *pstTormPkg, DBAsyn *pstDBAsyn);

ROLERANGEINFOENTRY * range_add_range_info_val(ZONESVRENV* pstEnv, Player *pstPlayer, 
													int iMainType, int iSubType, unsigned int uiVal);

int range_combat_set(ZONESVRENV* pstEnv, Player *pstPlayer, int iSubType, 
							int iPworldLevel, char *pszPworldName, unsigned int  uiVal);

int range_battle_set(ZONESVRENV* pstEnv, Player *pstPlayer, int iSubType, 
							int iPworldLevel, char *pszPworldName, unsigned int  uiVal);

int range_combat_script_set(ZONESVRENV* pstEnv, Player *pstPlayer, int iMainType,int iSubType, 
								ZONEPWORLDINST *pstPworldInst);

int range_delete(ZONESVRENV* pstEnv, int iMainType, int iSubType, tdr_ulonglong ullGID);

int range_delete_role(ZONESVRENV* pstEnv, tdr_ulonglong ullRoleID, int iCareer);
int range_up_role_name(ZONESVRENV* pstEnv, tdr_ulonglong ullRoleID, const char* pszOldName, const char* pszNewName);
int range_up_clanname(ZONESVRENV* pstEnv, tdr_ulonglong RoleID, const char* pszOldName, const char* pszNewName, int iSingle);

int player_get_top_range(ZONESVRENV* pstEnv,Player *pstPlayer,  int iMainType, int iSubType);
int z_db_watch_select_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
								TORMSELECTRES *pstSelectRes, tdr_ulonglong ullPetWID);
int player_watch_offline_in(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong ullPetWID, 
								WATCHONE *pstOne);
int player_logout_save_watch_cache(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_watch_offline_player(ZONESVRENV* pstEnv, Player *pstPlayer, char *pszRoleName, 
									tdr_ulonglong ullPetWID);
RangeOneMem *range_find_onemem(ZONESVRENV* pstEnv, int iMainType, int iSubType);
int range_attr_set_in(ZONESVRENV* pstEnv, Player *pstPlayer, int iSubType, unsigned int  uiVal);
int range_attr_set(ZONESVRENV* pstEnv, Player *pstPlayer,DYNATTRLIST *pstOldDyn);
int range_sparta_set(ZONESVRENV* pstEnv, Player *pstPlayer, int iSubType);
int player_sparta_in_top_ten(ZONESVRENV* pstEnv, Player *pstPlayer);
int range_weekcharm_award_dsn(ZONESVRENV* pstEnv);
int range_find(ZONESVRENV* pstEnv, int iMainType, int iSubType, tdr_ulonglong ullGID);
int range_attr_replace(ZONESVRENV* pstEnv, Player *pstPlayer);
int range_single_replace(ZONESVRENV* pstEnv, Player *pstPlayer);





#endif /* ZONE_RANGE_H */
