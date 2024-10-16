#ifndef ZONE_SPAN_H
#define ZONE_SPAN_H

#include "zone_svr.h"

enum tagROLE_UPDATE_OPERATE
{
    SPAN_SVR_ROLE_UPDATE = 0, 	/* �������� */
    CHG_SVR_ROLE_UPDATE = 1,	/* �л���·��д���� */
    SPAN_SVR_ROLE_KICK = 2,		/* ����KICK���� */
};

int ss_zone_msg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iMsgSrc, char *pszMsg, int iMsgLen);

ZoneTeam* team_shadow_by_id(ZONESVRENV *pstEnv, tdr_ulonglong ullID);

int team_shadow_insert(ZONESVRENV *pstEnv, const ZoneTeam *pstZoneTeam);

int team_shadow_delete(const ZONESVRENV *pstEnv, tdr_ulonglong ullID);

//����Role�ķ������ȡ��worldid
int role_world_id_get(Player *pstPlayer);

//�ǲ��ǽ�ɫ�������߷�����
int is_my_master_svr(int iID, Player *pstPlayer);

//master: �Ƿ����μӿ��ս��
int is_player_apply_battle(ZONESVRENV *pstEnv, Player *pstPlayer);

//master: �Ƿ������ڿ��Ұ��
int is_player_in_battle_pvp_svr(ZONESVRENV *pstEnv, Player *pstPlayer);

//�ǲ���ͬһ������ҡ���֧�ֿ���
int is_together_world(ZONESVRENV *pstEnv, Player *pstPlayer1, Player *pstPlayer2);

//master->other T�û�����
int player_span_role_kick(ZONESVRENV *pstEnv, Player *pstPlayer);

//����Roldata ͬʱҲ��hello ��
int player_span_role_update_req(ZONESVRENV *pstEnv, Player *pstPlayer, int iOpType);

/*
	����(�Զ���ӣ�ս��)�ȿ��ϵͳǰ����ٽ�ɫ����������һЩ�Ŷ��߼�
*/
int player_span_apply_req(ZONESVRENV *pstEnv, Player *pstPlayer, int iDstType);

/*
	������� : Step1
	Ԥ��վλ�����ڱ�����Ŀ��Ҳ���뾭���˲��裬
	����ֱ�ӿ��������һЩ�߼���˳��ִ����
*/
int player_span_hold_req(ZONESVRENV *pstEnv, Player *pstPlayer, SSSPANDSTDATA *pstSpanDstInfo);

// ��ս���еĽ�ɫ���� "��%d$%s",��$ǰ׺ȥ��
int battle_rolename_remove_prefix(char *pszFullName);

//�е�ͼ�Ƿ������
int player_span_map_check(ZONESVRENV* pstEnv, Player *pstPlayer, MapIndex *pstMapIdx, RESPOS *pstPos);

//��ɫ��zone_conn��BusID
int player_conn_busid_get(ZONESVRENV *pstEnv, Player *pstPlayer);

//��ɫ��zone_svr��BusID
int player_master_busid_get(ZONESVRENV *pstEnv, Player *pstPlayer);

int player_check_same_world_name(char *pszName1,char *pszName2);

int player_check_same_world(ZONESVRENV* pstEnv,char *pszFullName);

int player_shadow_action_request(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 CSACTIONRES *pstActionRes);

int player_shadow_action_res(ZONESVRENV *pstEnv, Player *pstPlayer,
                             char *pszRaw, int iLen);

int player_pvp_svr_member_info_sync_req(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_pvp_svr_member_info_sync(ZONESVRENV* pstEnv, SSBATTLEMEMBERINFO *pstSSMemberInfo);

//SS ��·��������
int player_span_heartbeat_req(ZONESVRENV *pstEnv, Player *pstPlayer);

//SS ��·����״̬��������
int player_span_apply_heartbeat_req(ZONESVRENV *pstEnv, Player *pstPlayer);

//SS ��·����
int player_span_heartbeat(ZONESVRENV *pstEnv, SSHEARTBEATREQ *pstSSHeartBeatReq, int iSrcID);

//ͬ��������Ϣ����ɫ���ڽ���
int player_team_span_sync_req(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneTeam *pstZoneTeam);

//֪ͨ���߶����ɢ
int z_team_span_destroy_req(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam);

//ͬ���������ݵ���ɫ���ڷ���
int player_protect_data_sync_req(ZONESVRENV* pstEnv, Player *pstPlayer);

//����ͬ��ȫ�Ӷ�����Ϣ
int z_team_all_span_sync_req(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam);

//�����Ŷӱ���ռ�õ�λ��
int player_span_apply_queue_clean(ZONESVRENV* pstEnv, Player *pstPlayer);

//֪ͨmaster �����Ŷ�״̬
int player_span_exit_pworld_queue_req(ZONESVRENV* pstEnv, Player *pstPlayer);

//����Ƿ�������
int player_span_state_check(ZONESVRENV* pstEnv,Player *pstPlayer);

//�л���·ǰ���������߰˰˵�״̬
int player_span_state_clean(ZONESVRENV* pstEnv,Player *pstPlayer);

int is_player_in_span_svr(ZONESVRENV *pstEnv, Player *pstPlayer);

//��ǰ��·����
int map_span_type_get(ZONESVRENV* pstEnv);

//������������ڽ�ɫʱ���ɴ�T������������
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
