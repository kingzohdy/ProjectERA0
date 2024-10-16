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
#define WORLD_CITY_STAT_CLEAR_TOLUA WORLD_CITY_STAT_CLEAR	/* 19点到20点清场阶段*/
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
	resPos_ToLua Pos;                  		/*   位置 */
};

struct tagCSCityMapPosInfo_ToLua
{
	int MemNum;                          	/*   人数 */
	tagCSCityMemInfo_ToLua Members[CLAN_MAX_MEM];           	/*   个人地图位置信息 */
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
	int		getCityBuildResID(const CityBuildInfo *info);// 获取资源ID
	int		getCityBuildMapIconId(int id);// 获取图片ID
	bool	CityBuildCanDisplay(const CityBuildInfo *info);// 是否显示
	int		CityBuildUpgradeId(const CityBuildInfo *info);// 是否可以升级
	bool	CityBuildCanRepair(const CityBuildInfo *info);// 是否可以修理
	bool	CityBuildCanSupply(const CityBuildInfo *info);// 是否可以充能
	bool	CityBuildCanRemove(const CityBuildInfo *info);// 是否可以拆除
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
	
	int				GetWorldCityClanNum();							//获取参与时间城争霸战的公会数目
	const char*		GetWorldCityClanName( int index );				//获取参与时间城争霸战的公会名字
	int				GetWorldCityClanGetReviveNum( int index );		//获取时间城争霸战公会复活点数目
	int				GetWorldCityClanGetControlNum( int index );		//获取时间城争霸战公会终端控制器数目
	int				GetWorldCityClanBuffNum( int index );			//获取时间城争霸战守护/摧毁增幅器数目
	int				GetWorldCityClanDoorNum( int index );			//获取时间城争霸战守护/摧毁城门数目
	int				GetWorldCityClanDefNum( int index );			//获取时间城争霸战守护/摧毁防御塔数目
	int				GetWorldCityClanTotal( int index );				//获取时间城争霸战总积分
	int				GetWorldCityClanTime( int index );				//获得时间城争霸战的Time

	int				GetWorldCitySelClanTotal();						//获取战争中自己公会的总积分

	int				GetWorldCityClanAwardMoneyType( int index );	//获取时间城争霸战获得的金钱类型
	int				GetWorldCityClanAwardMoney( int index );		//获取时间城争霸战获得的金钱数量
	int				GetWorldCityClanAwardItemID( int index );		//获取时间城争霸赛获得的奖励物品ID
	int				GetWorldCityClanAwardExp( int index );			//获取时间城争霸赛获得的奖励exp

	int				GetWorldCityClanEnterTime();					//获取时间城争霸战进入时间

	int				GetWorldCitySelfClanRank();						//获取时间城战争结束时自己公会排名
	int				GetWorldCitySelfClanMoneyType();				//获取时间城战争结束时自己公会获得的奖励金钱类型
	int				GetWorldCitySelfClanMoney();					//获取时间城战争结束时自己公会获得的奖励金钱数量

	int				GetWorldCitySelfContineKill();					//获取时间城争霸战自己最大连杀
	void			ClearWorldCitySelfContineKill();				//clear时间城争霸战自己最大连杀
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
	tagCSCityClanFightRange				m_CityClanFightInfo;	//攻城战公会排行榜信息
	tagCSCityKillInfo						m_KillInfo;				//攻城战连杀信息
};//tolua_export

#endif