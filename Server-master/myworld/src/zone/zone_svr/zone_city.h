#ifndef ZONE_CITY_H
#define ZONE_CITY_H

#include "zone_svr.h"

typedef struct  
{
	ZoneClan *pstClan;
	int iNum;
}TmpClanBuild;

//城池是否有名花有主了(被占领)
int is_has_master_world_city(ZONESVRENV *pstEnv);

//是否开启了攻城战功能
int is_open_fight_world_city(ZONESVRENV *pstEnv);

//是否在攻城战时间内
int is_in_siege_city(ZONESVRENV *pstEnv);

//快速检查是否在攻城期间，只看状态不看时间
int is_in_siege_city_fast(ZONESVRENV *pstEnv);

//城池建筑怪
int is_city_build_mon(Monster *pstMon);

//是否是城池所有的公会人员
int is_world_city_owner(ZONESVRENV *pstEnv, Player *pstPlayer);

//是否是城池所有的公会
int is_world_city_clan(ZONESVRENV *pstEnv, ZoneClan *pstClan);

//是否是城主
int is_world_city_master(ZONESVRENV *pstEnv, Player *pstPlayer);

//城市当前坚持防守的次数0 (刚占领/ 未被占领也是0)，1 防守住1次。。。。
int world_city_fight_keep_num(ZONESVRENV *pstEnv);

//设置城主雕像
int world_city_effigy_set(ZONESVRENV *pstEnv, Player *pstPlayer, int iMap, RESPOS *pstPos);

//城市创建
int world_city_refresh(ZONESVRENV *pstEnv);

//建筑死亡
int world_city_build_die(ZONESVRENV *pstEnv, Monster *pstMon, Player *pstKiller);

//更新建筑实例信息
int world_city_build_mon_update(ZONESVRENV *pstEnv, Monster *pstMon);

// 2秒TICK,  mapinst_active
int world_city_tick(ZONESVRENV *pstEnv, ZONEMAPINST *pstMapInst);

//10天后激活可进入地图
int world_city_set_enabletime(ZONESVRENV *pstEnv);

//下周开启城战
int world_city_set_nextweek_open(ZONESVRENV *pstEnv);

//是否可以进入攻城地图
int world_city_enter_check(ZONESVRENV *pstEnv);

//城池建筑信息广播
int map_world_city_info_broadcast(ZONESVRENV *pstEnv);

//进入攻城地图信息
int player_enter_world_city_info(ZONESVRENV *pstEnv, Player *pstPlayer);

//攻击告警
int world_city_build_warning(ZONESVRENV *pstEnv, Monster *pstMon);


//打开建造UI
int player_open_world_city_ui_info(ZONESVRENV *pstEnv, Player *pstPlayer, int iNpcID);

//重置攻城战城池
int world_city_reset(ZONESVRENV *pstEnv);

//CS协议
int world_city_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
			TFRAMEHEAD	 *pstFrameHead, CSPKG *pstCsPkg);

//城主上下线通知
int world_city_master_notify(ZONESVRENV* pstEnv, Player *pstPlayer, int fIn);

//保存退出地图坐标信息时防止是城池地图
void player_exit_info_check(ZONESVRENV *pstEnv, Player *pstPlayer);

//登录告知世界城池相关信息
int player_world_city_info(ZONESVRENV* pstEnv, Player *pstPlayer);

//设置攻城强制开启、关闭
int world_city_siege_set(int fSiege);

//开放城池攻城
int world_city_open(ZONESVRENV *pstEnv);

//获得地图上某个公会占有的控制中心数量
int city_build_control_num_get_byclan(ZONESVRENV *pstEnv, tdr_ulonglong ullGid);

//获得地图上能源增幅器存活的数量
int city_build_buff_num_get_byclan(ZONESVRENV * pstEnv, tdr_ulonglong ullGid);

//系统公告
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

//膜拜城主雕像
int player_worship_effigy(ZONESVRENV* pstEnv, Player *pstPlayer, int fFlower);

int player_effigy_award(ZONESVRENV* pstEnv, Player *pstPlayer);

int world_city_effigy_flower_update(ZONESVRENV* pstEnv);

//当前存在的能源增幅器数量
int world_city_buff_build_alive_num_get(ZONESVRENV* pstEnv);

//获得建筑的global信息
CITYBUILDINFO*  world_city_build_info_get(ZONESVRENV* pstEnv, int iID);

 int is_same_week(time_t t1, time_t t2);
 
#endif /* ZONE_CITY_H */

