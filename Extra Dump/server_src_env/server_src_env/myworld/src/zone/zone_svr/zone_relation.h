
#ifndef ZONE_RELATION_H
#define ZONE_RELATION_H

#include "zone_svr.h"

extern int g_SpousePos;


int player_black_find_byname(ROLEMISCINFO *pstRoleMisc, char *pszName);
int player_black_find_byid(ROLEMISCINFO *pstRoleMisc, tdr_ulonglong ullID);

int player_enemy_find_byname( const ROLEMISCINFO * pstMiscInfo, 	const char * pszName );

int player_relation(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int relation_role_login(ZONESVRENV* pstEnv, Player *pstPlayer);

int relation_role_logout(ZONESVRENV* pstEnv, Player *pstPlayer);

int relation_player_tick(ZONESVRENV* pstEnv, Player *pstPlayer);

int relation_init_res(ZONESVRENV* pstEnv, Player *pstPlayer, TORMSELECTRES *pstSelectRes);

int relation_db_buddy_chg(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes, DBBuddyChg *pstBuddyChg);

int relation_check_one(Player *pstPlayer, BUDDYINFO *pstBuddyInfo);

ROLEBUDDYMEMBER * relation_buddy_online_find(ROLEBUDDY *pstRoleBuddy , tdr_ulonglong ullRoleID);

ROLEENEMYMEMBER * relation_enemy_online_find(ROLEENEMY *pstEnemy , tdr_ulonglong ullRoleID);
ROLEENEMYMEMBER * relation_enemy_online_find_byname(ROLEENEMY *pstEnemy ,char* pszName);

BUDDYINFO * relation_buddy_db_find(BUDDY *pstBuddy, tdr_ulonglong ullRoleID);

int relation_db_update(ZONESVRENV* pstEnv, RELATION *pstRelation);

int relation_chg_clt_detail(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstOther, int iType);

int relation_dbbuddy_chg_clt(ZONESVRENV* pstEnv, Player *pstPlayer, BUDDYINFO *pstBuddyInfo, int iType);

int relation_friend_add(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstOther);

int player_auto_mate(ZONESVRENV* pstEnv, Player *pstPlayer);

int relation_dbbuddy_chg(ZONESVRENV* pstEnv, Player *pstPlayer, int iFlag, int iModifyDB);

int relation_buddyval_change(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEBUDDYMEMBER *pstBuddyMember, int iBuddyVal);

ROLEBUDDYMEMBER *relation_buddy_online_find_by_name(ROLEBUDDY *pstRoleBuddy, char* szRoleName);

int player_friends_buddyval_chg(ZONESVRENV* pstEnv, Player *pstPlayer, Player **apstPlayer, int iPlayerNum, int iBuddyVal);

int team_relation_buddyval_change(ZONESVRENV* pstEnv, ZoneTeam *pstTeam, int iBuddyVal);

int relation_online_query(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, SSRELATIONONLINEREQ *pstReq);

int ss_relation_online_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,  SSRELATIONONLINERES *pstRes);
int player_relation_addtmpenemy_req(ZONESVRENV* pstEnv, Player *pstPlayer, const char* pszName);
int player_logout_up_emotion(ZONESVRENV* pstEnv, Player *pstPlayer);
int player_relation_addenemy_req(ZONESVRENV* pstEnv, Player *pstPlayer, ADDENEMYREQ *pstAddEnemyReq);
ROLEBUDDYMEMBER *relation_buddy_online_find_by_roleid(ROLEBUDDY *pstRoleBuddy, tdr_ulonglong ullRoleID);
int relation_chg_clt_detail_mud(ZONESVRENV* pstEnv, Player* pstOther,char* pszName, int iType);
void relation_buddyInfo_get(Player *pstPlayer, BUDDYINFO *pstBuddyInfo);
int relation_get_role_member( Player *pstOther,ROLEBUDDYMEMBER *pstRoleBuddyMember);
int player_relation_addfriend_res_mud(ZONESVRENV* pstEnv, Player *pstPlayer, CSADDFRIENDRES *pstAddFriendRes,ROLEBUDDYMEMBER *pstOtherBuddyMember,BUDDYINFO *pstOtherBuddyInfo);
int relation_friend_add_mud(ZONESVRENV* pstEnv, Player *pstPlayer,char *pszOther, ROLEBUDDYMEMBER *pstOtherBuddyMember,BUDDYINFO *pstOtherBuddyInfo);
int relation_db_info(ZONESVRENV* pstEnv, Player *pstPlayer, RELATION *pstRelation);
int relation_online_info(ZONESVRENV* pstEnv, Player *pstPlayer);

int online_info_find_friend_byID(ROLEBUDDY * pstRoleBuddy, tdr_ulonglong ullID);
int player_relation_max_chg(ZONESVRENV* pstEnv, Player *pstPlayer);

int wedding_celebration_to_clt(ZONESVRENV *pstEnv,Player *pstPlayer);
int relation_involve_marry_svr(ZONESVRENV* pstEnv,Player *pstPlayer,const void* pvData,int Type);
int relation_oath_res_repose(ZONESVRENV* pstEnv, Player *pstPlayer,CSOATHRESCLT *pstOathResClt);
int get_wedding_item_id_by_meal(int iMeal);
int relation_remove_relation_req_repose(ZONESVRENV* pstEnv, Player *pstPlayer,CSREMOVERELATIONREQCLT *RemoveRelationReqClt);
int relation_pursue_res_repose(ZONESVRENV* pstEnv, Player *pstPlayer,CSPURSUERESCLT *pstPursueResClt);
int find_spouse_from_friend_list(ROLEBUDDY *pstRoleBuddy);
int find_lover_from_friend_list(ROLEBUDDY *pstRoleBuddy);
int find_spouse_byID(ROLEBUDDY *pstRoleBuddy, tdr_ulonglong ullID);
int find_lover_byID(ROLEBUDDY *pstRoleBuddy, tdr_ulonglong ullID);
int friend_team_in_pworld_chg_buddyval(ZONESVRENV* pstEnv, ZONEPWORLD *pstPworld,ZONEPWORLDINST *pstPworldInst, Player *pstPlayer);
int wedding_dsn_action(ZONESVRENV* pstEnv, Player *pstPlayer);
int wedding_machine_skill_effect(ZONESVRENV* pstEnv, Player *pstPlayer,Monster *pstMon,unsigned short SkillID);
int wedding_login_check(ZONESVRENV* pstEnv, Player *pstPlayer);
int wedding_item_recover(ZONESVRENV* pstEnv, Player *pstPlayer);
int wedding_pursue_logout(ZONESVRENV* pstEnv, Player *pstPlayer);
int wedding_dsn_once_flag(ZONESVRENV* pstEnv, Player *pstPlayer);








	
#endif /* ZONE_RELATION_H */
