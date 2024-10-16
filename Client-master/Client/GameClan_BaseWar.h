#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"

#pragma pack(1)
//tolua_begin
#define STRONG_STAT_NO_TOLUA		STRONG_STAT_NO				/*  */
#define STRONG_STAT_IDLE_TOLUA		STRONG_STAT_IDLE			/* 空闲时间 */
#define STRONG_STAT_SALE_TOLUA		STRONG_STAT_SALE			/* 竞拍时间 */
#define STRONG_STAT_TONGJI_TOLUA	STRONG_STAT_TONGJI			/* 统计时间 */
#define STRONG_STAT_SELECT_TOLUA	STRONG_STAT_SELECT			/* 玩家查询时间 */
#define STRONG_STAT_FIGHT_TOLUA		STRONG_STAT_FIGHT			/* 开搞时间 */

#define CLAN_TECH_CATEGORY_SHAPE_TOLUA CLAN_TECH_CATEGORY_SHAPE	/* 形状科技*/
#define CLAN_TECH_CATEGORY_SHADING_TOLUA CLAN_TECH_CATEGORY_SHADING	/* 底纹科技*/
#define CLAN_TECH_CATEGORY_MARK_TOLUA CLAN_TECH_CATEGORY_MARK/* 标记科技*/


struct tagBannerDef_ToLua
{
	short Type;                           
	short SubType;                        
	char TechLimit;                      
	int ClanMoney;                      
	int IconID;                         
	char Name[RES_NAME_LEN];              	/*  Ver.1593 xxx形状,xxx底纹等 */
};
struct tagStrongMon_ToLua
{
	int Flag;                            	/*  Ver.1616 1-表示是最大的资源怪 */
	int MemID;                          
	unsigned int ClanID;                         
	tdr_ulonglong ClanWID;                        
	int X;                               	/*  Ver.1598  */
	int Y;                               	/*  Ver.1598  */
};
struct tagStrongSaleInfo_ToLua
{
	int NpcID;                          
	int MapID;                           	/*  Ver.1563  */
	int SaleStat;                       
	int ClanJoin;                       
	int ClanPostion;                    
	int ClanMoney;                      
	int AtkClanMoney;                   
	tdr_ulonglong AtkClanWid;                     
};

struct tagStrongInfoOne_ToLua
{
	int MapID;                          
	int IsOpen;                          	/*  Ver.1613 0-未开启 1-开启 */
	tdr_ulonglong ClanWid;                        
};
struct tagAwardOre_ToLua
{
	int OreID;                          
	int Num;                            
}; 
struct tagMapAwardOreDef_ToLua
{
	int MapID;                          
	tagAwardOre_ToLua Ore[MAP_AWARD_ORE_LIST];        
}; 
struct tagGloryOne_ToLua
{
	int MapID;                          
	int Open;                           
	int EndTime;                        
	int StartTime;                      
	int x;                              
	int y;                              
};

struct tagStrongGlory_ToLua
{
	int Num;                             	/*  Ver.2226  */
	tagGloryOne_ToLua Glory[MAX_STRONG_POINT];         	/*  Ver.2226  */
};

//tolua_end
#pragma pack()

struct BaseWarMapInfo
{
	int nBaseWarMapID;
	int nMapID;
};

#define  MAX_BASE_WAR_MAP 6
#define  MAX_STRONG_POINT_LV 3

const BaseWarMapInfo BaseWarMapList[] = {
	{ 21300, 2202 },
	{ 21400, 2200 },
	{ 21500, 2300 },
	{ 21600, 2400 },
	{ 21700, 2500 },
	{ 21800, 3404 },
};

class GameClan_BaseWar//tolua_export
	:public NetMsgHandler, public Ogre::Singleton<GameClan_BaseWar>
{//tolua_export
public:
	GameClan_BaseWar(void);
	~GameClan_BaseWar(void);
public:
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

	//tolua_begin
	const char* getStrongPointName();				//获取当前地图据点名称
	int			getStrongPointLV();					//获取当前地图据点等级
	int			getStrongPointBaseUse();			//获取当前地图竞拍底价
	const char*	getStrongPointOwnerName();			//获取当前地图据点占领公会名称
	int			getStrongPointStat();				//获取当前据点竞拍状态
	int			getStrongPworldResLimit();			//获取当前据点战地图资源上限
	bool		IsJoinStrongSale();					//获取自己公会是否参与了竞拍
	int			getMyClanSaleMoney();				//获取自己公会暗拍资金
	int			getSaleWinerPrice();				//获取竞拍成功公会出价
	const char*	getSaleWinerName();					//获取竞拍成功公会名字

	bool		IsInBaseWarPworld();				//获取当前是否在据点战中
	int			getPworldAtkResNum();				//获取当前攻方资源数
	int			getPworldDefResNum();				//获取当前守方资源数
	tdr_ulonglong getAtkClanWID();					//获取当前攻方WID
	tdr_ulonglong getDefClanWID();					//获取当前守方WID

	tdr_ulonglong getCurMapStrongOwnerWID( int mapID );	//获取当前地图占领公会WID
	const char*	getCurMapStrongOWnerName( int mapID );				//获取当前地图占领公会Name
	bool		IsCurMapHasStrongPoint( int mapID );	//当前地图是否是据点地图

	int			getBannerNumByType( int nType );	//根据索引获取某一类型纹理的数量
	tagBannerDef_ToLua* getBannerDef( int index );	//获取旗帜定义

	tagStrongMon_ToLua getBaseWarResMonInfo( int index );	//获取据点战地图资源怪Info
	tdr_ulonglong	getBaseWarResMonClanWID( int index );	//获取据点战地图资源怪ClanID
	int			getBaseWarResMonMemID( int index );	//获取据点战地图资源怪memID

	//c->s
	void		EnterStrongSale( int nMoney );		//确认参加竞拍
	void		SetBanner( int mapID, int npcID, int nStyle, int nBack, int nSign );	//设置旗子
	void		SetBannerXuanYan( int mapID, const char* pszXuanYan );	//设置旗子宣言
	void		RequestAllStrongInfo();							//请求战争势力图信息
	void		RequestAward( int strongID, int type );			//领取奖励请求
	void		EnterStrongWar( int mapID );					//进入据点

	int			 getAllStrongInfoNum();					//获取战争势力图信息Num
	tagStrongSaleInfo_ToLua* getStrongSaleInfoByIndex( int index );		//根据索引获取战争势力图竞拍信息
	tagStrongInfoOne_ToLua* getStrongOccupInfoByIndex( int index );		//根据索引获取战争势力图占领信息

	unsigned int	getStrongOwnClanFengHaoColor( const tdr_ulonglong ClanWid );		//获取据点拥有者封号颜色

	int			getStrongAwardMoney( int mapID );			//获取据点奖励金钱
	int			getStrongAwardBuffID( int mapID );			//获取据点增益Buff名称
	int			getStrongAwardBuffLV( int mapID );			//获取据点占领奖励buffLV
	int			getStrongAwardClanContri( int mapID );		//获取据点奖励公会贡献度

	void		setStrongBaseWarSaleInfoByIndex( int index );	//据点势力图点击竞拍的时候，根据索引设置竞拍据点信息
	int			getStrongPointLVByMapID( int mapID );					//获取当前地图据点等级
	int			getStrongPointBaseUseByMapID( int mapID );			//获取当前地图竞拍底价
	int			getCityWarFightStat();								//获取时间城争霸赛状态

	bool		hasStrongOpen();
	bool		hasStrongPoint(GameActor *pActor);						//玩家公会当前是否占有据点
	tagMapAwardOreDef_ToLua*	getMapAwardOreDef( int nMapID );		//获取地图占领奖励

	//据点战玩家战绩信息
	int			getStrongWarSelfResGetNum();				//获取自己资源点占领次数
	int			getStrongWarSelfKill();						//获取自己杀人数
	int			getStrongWarSelfMaxContineKill();			//获取自己最大连杀数
	int			getStrongWarSelfAssist();					//获取自己助攻数
	int			getStrongWarSelfEnterTime();				//获取自己进入战场时间
	int			getStrongWarSelfCurContineKill();			//获取自己连杀数
	int			getStrongWarSelfDie();						//获取自己死亡数
	void		clearStrongWarSelfMaxContineKill();			//clear自己连杀数

	int			getStrongWarPlayerNum();							//获取据点战参与的玩家数目

	const char* getStrongWarPlayerName( int index );				//获取玩家名字
	int			getStrongWarPlayerRaceId( int index );				//获取玩家职业id
	int			getStrongWarPlayerResGetNum( int index );			//获取玩家资源点占领次数
	int			getStrongWarPlayerKill( int index );				//获取玩家杀人数
	int			getStrongWarPlayerMaxContineKill( int index );		//获取玩家最大连杀数
	int			getStrongWarPlayerAssist( int index );				//获取玩家助攻数
	int			getStrongWarPlayerEnterTime( int index );			//获取玩家进入战场时间
	const char* getStrongWarPlayerClanName( int index );			//获取玩家公会名

	int			getStrongWarWinnerMoneyType();						//获取据点胜利方奖励金钱类型
	int			getStrongWarWinnerMoney();							//获取据点胜利方奖励金钱数量
	int			getStrongWarFailMoneyType();						//获取失败方奖励金钱类型
	int			getStrongWarFailMoney();							//获取失败方奖励金钱数量

	int			getStrongWarWinnerItemID();							//获取据点战获胜方的奖励物品ID
	int			getStrongWarWinnerItemNum();						//获取据点战获胜方奖励物品数量
	int			getStrongWarFailItemID();							//获取据点战失败方奖励物品ID
	int			getStrongWarFailItemNum();							//获取据点战失败方奖励物品数量

	int			getStrongWarWinnereExp();							//获取失胜利奖励exp
	int			getStrongWarFailExp();								//获取失败方奖励exp

	void		LeaveStrongWar();									//离开据点战场

	int			getStrongWarTodayEnteredID();						//获取玩家今日进入的据点战记录ID
	const char*	getStrongWarTodayEnteredStrongName();				//获取玩家今日进入的据点战记录据点Name

	tagStrongGlory_ToLua	getStrongBannerGloryInfo();		//获取对应地图的战旗荣耀信息

	//tolua_end
	void		SetBaseWarResMonInfo();					//设置据点战资源怪的信息
private:
	std::vector<tagStrongInfoOne> m_StrongPointsVec;	//全服据点信息列表	
	tagStrongPworldInfo m_StrongPworldInfo;				//据点战过程中资源信息记录
	tagStrongSaleInfo m_CurStrongSaleInfo;				//当前竞拍据点信息
	std::vector<tagBannerDef*> m_BannerDefVec;			//公会旗帜列表
	tagStrongAllInfo m_StrongAllInfo;				//战争势力图信息
	tagStrongWarOne m_StrongWarOne;					//单人据点战斗信息
	tagStrongWarAll m_StrongWarAll;					//所有人的据点战斗信息
	bool m_StrongWarIsInFight;						//是否在据点战战争中

	int m_StrongWarTodayEnterID;					//玩家今日进入的据点战记录ID

	tagStrongGlory_ToLua	m_StrongGlory;					//战旗荣耀信息
};//tolua_export