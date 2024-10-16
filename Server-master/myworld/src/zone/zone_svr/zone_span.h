#ifndef ZONE_SPAN_H
#define ZONE_SPAN_H

#include "zone_svr.h"

enum tagROLE_UPDATE_OPERATE
{
    SPAN_SVR_ROLE_UPDATE = 0, 	/* 正常更新 */
    CHG_SVR_ROLE_UPDATE = 1,	/* 切换线路回写更新 */
    SPAN_SVR_ROLE_KICK = 2,		/* 分线KICK流程 */
};

int ss_zone_msg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iMsgSrc, char *pszMsg, int iMsgLen);

ZoneTeam* team_shadow_by_id(ZONESVRENV *pstEnv, tdr_ulonglong ullID);

int team_shadow_insert(ZONESVRENV *pstEnv, const ZoneTeam *pstZoneTeam);

int team_shadow_delete(const ZONESVRENV *pstEnv, tdr_ulonglong ullID);

//根据Role的分配规则取出worldid
int role_world_id_get(Player *pstPlayer);

//是不是角色所属主线服务器
int is_my_master_svr(int iID, Player *pstPlayer);

//master: 是否报名参加跨服战场
int is_player_apply_battle(ZONESVRENV *pstEnv, Player *pstPlayer);

//master: 是否真身在跨服野外
int is_player_in_battle_pvp_svr(ZONESVRENV *pstEnv, Player *pstPlayer);

//是不是同一服的玩家。不支持跨区
int is_together_world(ZONESVRENV *pstEnv, Player *pstPlayer1, Player *pstPlayer2);

//master->other T用户下线
int player_span_role_kick(ZONESVRENV *pstEnv, Player *pstPlayer);

//更新Roldata 同时也是hello 包
int player_span_role_update_req(ZONESVRENV *pstEnv, Player *pstPlayer, int iOpType);

/*
	报名(自动组队，战场)等跨服系统前的虚假角色，用来处理一些排队逻辑
*/
int player_span_apply_req(ZONESVRENV *pstEnv, Player *pstPlayer, int iDstType);

/*
	跨服步骤 : Step1
	预先站位，对于报名类的跨服也必须经过此步骤，
	不可直接跨服，便于一些逻辑能顺序执行完
*/
int player_span_hold_req(ZONESVRENV *pstEnv, Player *pstPlayer, SSSPANDSTDATA *pstSpanDstInfo);

// 将战场中的角色名字 "服%d$%s",的$前缀去掉
int battle_rolename_remove_prefix(char *pszFullName);

//切地图是否跨服检查
int player_span_map_check(ZONESVRENV* pstEnv, Player *pstPlayer, MapIndex *pstMapIdx, RESPOS *pstPos);

//角色的zone_conn的BusID
int player_conn_busid_get(ZONESVRENV *pstEnv, Player *pstPlayer);

//角色的zone_svr的BusID
int player_master_busid_get(ZONESVRENV *pstEnv, Player *pstPlayer);

int player_check_same_world_name(char *pszName1,char *pszName2);

int player_check_same_world(ZONESVRENV* pstEnv,char *pszFullName);

int player_shadow_action_request(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 CSACTIONRES *pstActionRes);

int player_shadow_action_res(ZONESVRENV *pstEnv, Player *pstPlayer,
                             char *pszRaw, int iLen);

int player_pvp_svr_member_info_sync_req(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_pvp_svr_member_info_sync(ZONESVRENV* pstEnv, SSBATTLEMEMBERINFO *pstSSMemberInfo);

//SS 线路心跳发起
int player_span_heartbeat_req(ZONESVRENV *pstEnv, Player *pstPlayer);

//SS 线路报名状态心跳发起
int player_span_apply_heartbeat_req(ZONESVRENV *pstEnv, Player *pstPlayer);

//SS 线路心跳
int player_span_heartbeat(ZONESVRENV *pstEnv, SSHEARTBEATREQ *pstSSHeartBeatReq, int iSrcID);

//同步队伍信息到角色所在进程
int player_team_span_sync_req(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneTeam *pstZoneTeam);

//通知分线队伍解散
int z_team_span_destroy_req(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam);

//同步保护数据到角色所在分线
int player_protect_data_sync_req(ZONESVRENV* pstEnv, Player *pstPlayer);

//请求同步全队队伍信息
int z_team_all_span_sync_req(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam);

//清理排队报名占用的位置
int player_span_apply_queue_clean(ZONESVRENV* pstEnv, Player *pstPlayer);

//通知master 清理排队状态
int player_span_exit_pworld_queue_req(ZONESVRENV* pstEnv, Player *pstPlayer);

//跨服是否允许检查
int player_span_state_check(ZONESVRENV* pstEnv,Player *pstPlayer);

//切换线路前清理下七七八八的状态
int player_span_state_clean(ZONESVRENV* pstEnv,Player *pstPlayer);

int is_player_in_span_svr(ZONESVRENV *pstEnv, Player *pstPlayer);

//当前线路类型
int map_span_type_get(ZONESVRENV* pstEnv);

//跨服环境不存在角色时，干脆T掉本服人下线
int player_span_clean_logout_req(ZONESVRENV *pstEnv, int iUin, tdr_ulonglong ullRoleID, int iDstID);

int player_span_role_clean(ZONESVRENV *pstEnv, int iUin, tdr_ulonglong ullRoleID, int iDstID);
int clan_span_exit_pworld_queue_req(ZONESVRENV* pstEnv, CLANID *pstClanID,int iBusID);

int span_map_camp_init(ZONESVRENV* pstEnv);
int span_map_camp_ss_res(ZONESVRENV * pstEnv,SSPKG *pstPkg);
int span_map_camp_ss_req(ZONESVRENV * pstEnv,SSPKG *pstPkg,int iMsgSrc);
int get_span_map_camp_by_worldId(ZONESVRENV * pstEnv,int iMsgSrc,int *pCampIdx);
int get_world_by_camp(ZONESVRENV * pstEnv,int idx,int *iCampWorldID);
int z_ss_world_boss_data_sync(ZONESVRENV * pstEnv, SSWORLDBOSSDATASYNC *pstWorldBossDataSync);






#endif /* ZONE_SPAN_H */
