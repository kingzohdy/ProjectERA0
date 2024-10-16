#ifndef ZONE_CLAN_PET_H
#define ZONE_CLAN_PET_H

#include "zone_svr.h"

#define JIYANG_USE_CONTRI 100
#define JIYANG_USE_MONEY 100
#define PET_MAX_IMP 13
#define PET_WANSHUA_USE_MONEY 1000
#define PET_MAX_HAPPY 100
#define PET_USE_WANJU_ADD 10
#define PET_MAX_WENBAO 100
#define PET_WORK_HAPPY 25
#define PET_WORK_TIME 1.5*3600 

int clan_pet_pence_tick(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan);
int clan_pet_tick(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan);
int clan_pet_all_notify(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneClan *pstClan);
int clan_pet_player_enter_city(ZONESVRENV* pstEnv, Player *pstPlayer);
int clan_pet_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
					TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);
PETIMPUPATTRDEF *pet_imp_upattr_def_find(ZONESVRENV* pstEnv, int iImp, int iType);
PETIMPUPDEF *pet_imp_up_def_find(ZONESVRENV* pstEnv, int iImp);
int clan_pet_player_chg_name(ZONESVRENV* pstEnv, Player *pstPlayer, char *pszNewName);
int clan_pet_data_syn(ZONESVRENV* pstEnv, Player *pstPlayer, CLANPETONE *pstOne);
int clan_pet_player_login_check(ZONESVRENV* pstEnv, Player *pstPlayer, int iNotify);
int clan_pet_player_leave(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneClan *pstClan);
int clan_pet_jiyang_unlock(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, 
								CLANPETONE *pstOne, CLANPET *pstClanPet,Player *pstOwner, ZoneClan *pstClan);
CLANPETONE *clan_pet_by_player(CLANPET *pstClanPet, Player *pstPlayer);
int pet_is_jiyang(ROLEPET *pstRolePet);
int pet_jiyang_limit(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong PetWid);
int clan_pet_jiyang(ZONESVRENV* pstEnv, Player *pstPlayer, CSCLANPETJIYANG *pstJiYang);
int clan_pet_jiyang_end(ZONESVRENV* pstEnv, Player *pstPlayer, CSCLANPETJIYANGEND*pstEnd);
int clan_pet_info_notify(ZONESVRENV* pstEnv, Player *pstPlayer);
int clan_pet_clear_gaoji_sec(ZONESVRENV* pstEnv, Player *pstPlayer);
int clan_pet_gaoji_sec_notify(ZONESVRENV* pstEnv, Player *pstPlayer);
#endif 