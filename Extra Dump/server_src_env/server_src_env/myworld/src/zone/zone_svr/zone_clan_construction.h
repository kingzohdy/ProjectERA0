#ifndef ZONE_CLAN_CONSTRUCTION_H
#define ZONE_CLAN_CONSTRUCTION_H


// ���������ȼ�
int clan_upgrade(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, Player *pstPlayer);

// ���Ὠ�����tick
int clan_construction_tick(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan);

// ������������ȼ�
int clan_city_upgrade(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                      Player *pstPlayer, const char *pszName);

// ������н��������ȼ�
int clan_city_building_upgrade(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan,
	                           Player *pstPlayer, int iBuildingIdx);

// ��ҽ��빫����и���
int player_clan_enter_city(ZONESVRENV* pstEnv, Player *pstPlayer,
                           RESPOS *pstPos);


// �뿪�������
int player_clan_leave_city(ZONESVRENV *pstEnv, Player *pstPlayer,
                           const ZoneClan *pstZoneClan);

// �鿴������Ĺ��������Ϣ
int player_clan_city_info(ZONESVRENV *pstEnv, Player *pstPlayer);


// ��������ȼ�
int player_clan_upgrade(ZONESVRENV *pstEnv, Player *pstPlayer);


// ����/�������еȼ�
int player_clan_city_upgrade(ZONESVRENV *pstEnv, Player *pstPlayer, 
                             const char *pszClanCityName);

// ����/���������ȼ�
int player_clan_city_building_upgrade(ZONESVRENV *pstEnv, Player *pstPlayer,
                                      int iBuildingID);

// ���п�����Ϣ
int player_clan_info(ZONESVRENV *pstEnv, Player *pstPlayer);


// �Ƴ��������ϰ���
int clan_building_remove_barrier(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                 int iIdx, int iLevel);


// ��ȡĳ���ȼ�����Ƽ������ĵ��ܿƼ���
int clan_tech_used_tech_point(ZONESVRENV *pstEnv, int iCategroy, int iLevel);


// ��ȡĳ��������Ӧ���Ѿ����ĵĿƼ�����
int clan_building_used_tech_point(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                  int iBuildingIdx);


// ����Ч���ַ��������й����Ա
int clan_effect_broadcast(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iID,
                          const char *pszEffect);


// ��齨��\���������������ĿƼ�������
int clan_building_upgrade_usedtech_check(
	ZONESVRENV *pstEnv,
    ZoneClan *pstZoneClan,
	Player *pstPlayer,
	int iBuildingIdx);

// ���͸������е����,���������Ϣ����
int player_clan_city_update(ZONESVRENV *pstEnv, Player *pstPlayer,ZoneClan *pstZoneClan);

// ����ȼ��������Ѿ��ı�
int clan_city_chg(ZONESVRENV *pstEnv,ZoneClan *pstZoneClan);



extern const char *kClanBuildingName[];

#endif // ZONE_CLAN_CONSTRUCTION_H
