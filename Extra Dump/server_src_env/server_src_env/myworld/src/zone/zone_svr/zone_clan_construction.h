#ifndef ZONE_CLAN_CONSTRUCTION_H
#define ZONE_CLAN_CONSTRUCTION_H


// 公会提升等级
int clan_upgrade(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, Player *pstPlayer);

// 公会建设相关tick
int clan_construction_tick(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan);

// 公会城市提升等级
int clan_city_upgrade(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                      Player *pstPlayer, const char *pszName);

// 公会城市建筑提升等级
int clan_city_building_upgrade(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan,
	                           Player *pstPlayer, int iBuildingIdx);

// 玩家进入公会城市副本
int player_clan_enter_city(ZONESVRENV* pstEnv, Player *pstPlayer,
                           RESPOS *pstPos);


// 离开公会城市
int player_clan_leave_city(ZONESVRENV *pstEnv, Player *pstPlayer,
                           const ZoneClan *pstZoneClan);

// 查看本公会的公会城市信息
int player_clan_city_info(ZONESVRENV *pstEnv, Player *pstPlayer);


// 提升公会等级
int player_clan_upgrade(ZONESVRENV *pstEnv, Player *pstPlayer);


// 申请/提升城市等级
int player_clan_city_upgrade(ZONESVRENV *pstEnv, Player *pstPlayer, 
                             const char *pszClanCityName);

// 建造/提升建筑等级
int player_clan_city_building_upgrade(ZONESVRENV *pstEnv, Player *pstPlayer,
                                      int iBuildingID);

// 城市快照信息
int player_clan_info(ZONESVRENV *pstEnv, Player *pstPlayer);


// 移除建筑的障碍物
int clan_building_remove_barrier(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                 int iIdx, int iLevel);


// 获取某个等级公会科技所消耗的总科技点
int clan_tech_used_tech_point(ZONESVRENV *pstEnv, int iCategroy, int iLevel);


// 获取某个建筑对应的已经消耗的科技点数
int clan_building_used_tech_point(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                  int iBuildingIdx);


// 发送效果字符串给所有公会会员
int clan_effect_broadcast(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iID,
                          const char *pszEffect);


// 检查建造\升级建筑的已消耗科技点需求
int clan_building_upgrade_usedtech_check(
	ZONESVRENV *pstEnv,
    ZoneClan *pstZoneClan,
	Player *pstPlayer,
	int iBuildingIdx);

// 发送给城市中的玩家,公会城市信息更新
int player_clan_city_update(ZONESVRENV *pstEnv, Player *pstPlayer,ZoneClan *pstZoneClan);

// 公会等级或建筑物已经改变
int clan_city_chg(ZONESVRENV *pstEnv,ZoneClan *pstZoneClan);



extern const char *kClanBuildingName[];

#endif // ZONE_CLAN_CONSTRUCTION_H
