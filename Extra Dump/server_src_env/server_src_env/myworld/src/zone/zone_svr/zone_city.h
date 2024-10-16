#ifndef ZONE_CITY_H
#define ZONE_CITY_H

#include "zone_svr.h"

typedef struct  
{
	ZoneClan *pstClan;
	int iNum;
}TmpClanBuild;

//�ǳ��Ƿ�������������(��ռ��)
int is_has_master_world_city(ZONESVRENV *pstEnv);

//�Ƿ����˹���ս����
int is_open_fight_world_city(ZONESVRENV *pstEnv);

//�Ƿ��ڹ���սʱ����
int is_in_siege_city(ZONESVRENV *pstEnv);

//���ټ���Ƿ��ڹ����ڼ䣬ֻ��״̬����ʱ��
int is_in_siege_city_fast(ZONESVRENV *pstEnv);

//�ǳؽ�����
int is_city_build_mon(Monster *pstMon);

//�Ƿ��ǳǳ����еĹ�����Ա
int is_world_city_owner(ZONESVRENV *pstEnv, Player *pstPlayer);

//�Ƿ��ǳǳ����еĹ���
int is_world_city_clan(ZONESVRENV *pstEnv, ZoneClan *pstClan);

//�Ƿ��ǳ���
int is_world_city_master(ZONESVRENV *pstEnv, Player *pstPlayer);

//���е�ǰ��ַ��صĴ���0 (��ռ��/ δ��ռ��Ҳ��0)��1 ����ס1�Ρ�������
int world_city_fight_keep_num(ZONESVRENV *pstEnv);

//���ó�������
int world_city_effigy_set(ZONESVRENV *pstEnv, Player *pstPlayer, int iMap, RESPOS *pstPos);

//���д���
int world_city_refresh(ZONESVRENV *pstEnv);

//��������
int world_city_build_die(ZONESVRENV *pstEnv, Monster *pstMon, Player *pstKiller);

//���½���ʵ����Ϣ
int world_city_build_mon_update(ZONESVRENV *pstEnv, Monster *pstMon);

// 2��TICK,  mapinst_active
int world_city_tick(ZONESVRENV *pstEnv, ZONEMAPINST *pstMapInst);

//10��󼤻�ɽ����ͼ
int world_city_set_enabletime(ZONESVRENV *pstEnv);

//���ܿ�����ս
int world_city_set_nextweek_open(ZONESVRENV *pstEnv);

//�Ƿ���Խ��빥�ǵ�ͼ
int world_city_enter_check(ZONESVRENV *pstEnv);

//�ǳؽ�����Ϣ�㲥
int map_world_city_info_broadcast(ZONESVRENV *pstEnv);

//���빥�ǵ�ͼ��Ϣ
int player_enter_world_city_info(ZONESVRENV *pstEnv, Player *pstPlayer);

//�����澯
int world_city_build_warning(ZONESVRENV *pstEnv, Monster *pstMon);


//�򿪽���UI
int player_open_world_city_ui_info(ZONESVRENV *pstEnv, Player *pstPlayer, int iNpcID);

//���ù���ս�ǳ�
int world_city_reset(ZONESVRENV *pstEnv);

//CSЭ��
int world_city_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
			TFRAMEHEAD	 *pstFrameHead, CSPKG *pstCsPkg);

//����������֪ͨ
int world_city_master_notify(ZONESVRENV* pstEnv, Player *pstPlayer, int fIn);

//�����˳���ͼ������Ϣʱ��ֹ�ǳǳص�ͼ
void player_exit_info_check(ZONESVRENV *pstEnv, Player *pstPlayer);

//��¼��֪����ǳ������Ϣ
int player_world_city_info(ZONESVRENV* pstEnv, Player *pstPlayer);

//���ù���ǿ�ƿ������ر�
int world_city_siege_set(int fSiege);

//���ųǳع���
int world_city_open(ZONESVRENV *pstEnv);

//��õ�ͼ��ĳ������ռ�еĿ�����������
int city_build_control_num_get_byclan(ZONESVRENV *pstEnv, tdr_ulonglong ullGid);

//��õ�ͼ����Դ��������������
int city_build_buff_num_get_byclan(ZONESVRENV * pstEnv, tdr_ulonglong ullGid);

//ϵͳ����
int world_city_add_bulletin(ZONESVRENV* pstEnv,int iActiveTime,int iRollInterval, char *pszStr, ...);

int global_world_city_update(ZONESVRENV *pstEnv);

int global_world_city_init(ZONESVRENV * pstEnv);

int global_city_build_online_clear(ZONESVRENV *pstEnv);

int world_city_revive_pos_get(ZONESVRENV* pstEnv, Player *pstPlayer, MapIndex *pstMapIdx, int *piMapIdx, RESPOS *pstPos);

int world_city_open_ui_action(ZONESVRENV* pstEnv);

int world_city_player_die(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneAni *pstAtker);
int get_world_city_clan_pos(ZONESVRENV *pstEnv, Player *pstPlayer);
Monster *new_city_build_normal_mon(ZONESVRENV *pstEnv, ZONEMAPINST *pstMapInst, 
		int iMonID, int iDir, RESPOS *pstPos, ZoneClan *pstClan, int iBuildDefID);

int world_city_fight_range_init(ZONESVRENV *pstEnv);
int world_city_fight_range_svr_info(ZONESVRENV* pstEnv, Player *pstPlayer, int fEnd);
int world_city_fight_range_update(ZONESVRENV *pstEnv, ZoneClan *pstClan, int iBuildType);
int world_city_owner_fight_info_update(ZONESVRENV* pstEnv);
int world_city_kill_info(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_worship_effigy_test(ZONESVRENV * pstEnv, Player * pstPlayer, int fFlower);

//Ĥ�ݳ�������
int player_worship_effigy(ZONESVRENV* pstEnv, Player *pstPlayer, int fFlower);

int player_effigy_award(ZONESVRENV* pstEnv, Player *pstPlayer);

int world_city_effigy_flower_update(ZONESVRENV* pstEnv);

//��ǰ���ڵ���Դ����������
int world_city_buff_build_alive_num_get(ZONESVRENV* pstEnv);

//��ý�����global��Ϣ
CITYBUILDINFO*  world_city_build_info_get(ZONESVRENV* pstEnv, int iID);

 int is_same_week(time_t t1, time_t t2);
 
#endif /* ZONE_CITY_H */

