#pragma once
#ifndef __WORLDCITY__
#define __WORLDCITY__

#include "GameEngineTypes.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "GameMsgPrint.h"
#include "ClientInterface.h"
#include <vector>
#include <map>
#include <set>

//tolua_begin
/* WORLD_CITY_STAT_TOLUA*/
#define WORLD_CITY_STAT_NORMAL_TOLUA WORLD_CITY_STAT_NORMAL 
#define WORLD_CITY_STAT_CLEAR_TOLUA WORLD_CITY_STAT_CLEAR	/* 19�㵽20���峡�׶�*/
#define WORLD_CITY_STAT_FIGHT_TOLUA WORLD_CITY_STAT_FIGHT 

enum
{
	WorldCityMapID = WORLD_CITY_MAPID
};

struct CityBuildInfo
{
	int		DefID;
	int		MemID;
	int		State;
	int		Type;
	int		BuildIdx;
	int		BuildLevel;
	int		HP;
	int		MaxHP;
	int		Energy;
	int		MaxEnergy;
	char	ClanName[NAME_LEN];
	int		WarnEndTime;
};

struct resPos_ToLua
{
	int X;                              
	int Y;                              
};

struct tagCSCityMemInfo_ToLua
{
	int MemID;                           	/*   MemID */
	resPos_ToLua Pos;                  		/*   λ�� */
};

struct tagCSCityMapPosInfo_ToLua
{
	int MemNum;                          	/*   ���� */
	tagCSCityMemInfo_ToLua Members[CLAN_MAX_MEM];           	/*   ���˵�ͼλ����Ϣ */
};

//tolua_end

class WorldCityManager//tolua_export
	: public NetMsgHandler, public IWorldCityManager
{//tolua_export
protected:
	WorldCityManager();

public:
	static WorldCityManager & getSingleton();

public:
	~WorldCityManager();
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	virtual void	objEnterView(GameActor *actor);
	virtual void	objLeaveView(GameActor *actor);
	virtual bool	objCanAttack(GameActor *actor);

public:
	void	reset();
	void	ClearEffigy();
	void	UpdateEffigy();
	void	UpdateMusic();
	void	OnEnterMap(GameMap *map);
	void	UpdateWorldCityClanName();
	void	update(unsigned int dtick);
	void	draw(FollowCamera *pcamera);
	bool	HasBuffAlive();

	//tolua_begin
	const CityBuildInfo *	getCityBuildInfo(int id);
	int		getFirstCityBuildId();
	int		getNextCityBuildId(int id);
	int		getCityBuildPosX(int id);
	int		getCityBuildPosY(int id);
	int		getCityBuildResID(const CityBuildInfo *info);// ��ȡ��ԴID
	int		getCityBuildMapIconId(int id);// ��ȡͼƬID
	bool	CityBuildCanDisplay(const CityBuildInfo *info);// �Ƿ���ʾ
	int		CityBuildUpgradeId(const CityBuildInfo *info);// �Ƿ��������
	bool	CityBuildCanRepair(const CityBuildInfo *info);// �Ƿ��������
	bool	CityBuildCanSupply(const CityBuildInfo *info);// �Ƿ���Գ���
	bool	CityBuildCanRemove(const CityBuildInfo *info);// �Ƿ���Բ��
	int		getCityBuildFirstConstruct(const CityBuildInfo *info, bool state);
	int		getCityBuildNextConstruct(const CityBuildInfo *info, int id, bool state);

	int		getCityProductMapIconId(int id);
	int		getCityProductBuildIconId(int id);
	const char *	getCityProductBuildTips(int id);
	const char *	getCityProductBuildDesc(int id);

	void	UpgradeCityBuild(int id);
	void	ConstructCityBuild(int id, int idx);
	void	RepairCityBuild(int id);
	void	SupplyCityBuild(int id);
	void	RemoveCityBuild(int id);
	bool	IsWorldCityKeeper(unsigned int id);
	bool	IsWorldCityKeeper(GameActor *pActor);
	bool	IsCityBuildFight(GameActor *pActor);
	bool	IsCityBuildFight(int id);
	int		GetCityBuildDefID(GameActor *pActor);
	int		GetCityBuildDefID(int id);
	int		GetCityBuildRepairCost(int id);
	int		GetCityBuildSupplyCost(int id);
	int		GetCityBuildConstructCost(int id);
	int		GetCityBuildConstructNextResource(int id, int resourceId = 0);
	int		GetCityBuildConstructResourceCost(int id, int resourceId);
	int		GetCityFightEnableStat();
	void	RequestClanMemberPos();
	int		GetCityKeeperClanID();
	tdr_ulonglong GetCityKeepClanWID();
	int		GerCityFightEnableStat();
	int		GetCityOwnerKeepNum();
	int		GetCityOwnerAwardMoney();
	int		GetCityStat();
	
	int				GetWorldCityClanNum();							//��ȡ����ʱ�������ս�Ĺ�����Ŀ
	const char*		GetWorldCityClanName( int index );				//��ȡ����ʱ�������ս�Ĺ�������
	int				GetWorldCityClanGetReviveNum( int index );		//��ȡʱ�������ս���Ḵ�����Ŀ
	int				GetWorldCityClanGetControlNum( int index );		//��ȡʱ�������ս�����ն˿�������Ŀ
	int				GetWorldCityClanBuffNum( int index );			//��ȡʱ�������ս�ػ�/�ݻ���������Ŀ
	int				GetWorldCityClanDoorNum( int index );			//��ȡʱ�������ս�ػ�/�ݻٳ�����Ŀ
	int				GetWorldCityClanDefNum( int index );			//��ȡʱ�������ս�ػ�/�ݻٷ�������Ŀ
	int				GetWorldCityClanTotal( int index );				//��ȡʱ�������ս�ܻ���
	int				GetWorldCityClanTime( int index );				//���ʱ�������ս��Time

	int				GetWorldCitySelClanTotal();						//��ȡս�����Լ�������ܻ���

	int				GetWorldCityClanAwardMoneyType( int index );	//��ȡʱ�������ս��õĽ�Ǯ����
	int				GetWorldCityClanAwardMoney( int index );		//��ȡʱ�������ս��õĽ�Ǯ����
	int				GetWorldCityClanAwardItemID( int index );		//��ȡʱ�����������õĽ�����ƷID
	int				GetWorldCityClanAwardExp( int index );			//��ȡʱ�����������õĽ���exp

	int				GetWorldCityClanEnterTime();					//��ȡʱ�������ս����ʱ��

	int				GetWorldCitySelfClanRank();						//��ȡʱ���ս������ʱ�Լ���������
	int				GetWorldCitySelfClanMoneyType();				//��ȡʱ���ս������ʱ�Լ������õĽ�����Ǯ����
	int				GetWorldCitySelfClanMoney();					//��ȡʱ���ս������ʱ�Լ������õĽ�����Ǯ����

	int				GetWorldCitySelfContineKill();					//��ȡʱ�������ս�Լ������ɱ
	void			ClearWorldCitySelfContineKill();				//clearʱ�������ս�Լ������ɱ
	//tolua_end

	bool	IsWorldCityKeeper(const tdr_ulonglong &GID);
	bool	IsFighting();
	void	MarkCityBuildFight(GameActor *pActor, int flag);
	void	MarkCityBuildFight(int id, int flag);
	void	UnMarkCityBuildFight(GameActor *pActor);
	void	UnMarkCityBuildFight(int id);
	void	ClearCityBuildFight();

	int		GetCityBuildType( int nMemId );
	const char* getOwnerName();

private:
	void	setCityBuildInfo(size_t num, const tagCSCityBuildInfo *info);
	void	setCaptureInfo(int time, const char *clan);
	void	setFightStat(int stat);

private:
	std::vector<CityBuildInfo>			BuildInfo;
	std::map<int, size_t>				BuildInfoIdx;
	tagCSWorldCityInfo					WorldCityCoreInfo;
	Ogre::Entity						*Effigy;
	int									FightStat;
	std::map<int, int>					CityBuildFight;
	tagCSCityClanFightRange				m_CityClanFightInfo;	//����ս�������а���Ϣ
	tagCSCityKillInfo						m_KillInfo;				//����ս��ɱ��Ϣ
};//tolua_export

#endif