#pragma once
#include "OgreSingleton.h"
#include <time.h>
#include <map>
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "GameEngineTypes.h"
#include "GameDefineTolua.h"
#include "GameDBData.h"

//tolua_begin

enum
{
	PRMOTION_DAILY_TASK,	//每日目标
	PROMOTION_ALL,			//全部
	PRMOTION_ACTIVE,		//副本
	PRMOTION_CLAN,			//公会
	PRMOTION_DAYACT,		//每日活动
	PRMOTION_BOSS,			//世界boss
	PRMOTION_BUSY,			//我很忙
};

enum
{
	PROMITION_CMP_NAME,		//名称
	PROMITION_CMP_TIME,		//开放时间
	PROMITION_CMP_LEVEL,	//等级
	PROMITION_CMP_STAR,		//收益类型
	PROMITION_CMP_COUNT,	//完成次数
	PROMITION_CMP_TEAM,		//是否组队
	PROMITION_CMP_STATE,	//状态
};

enum
{
	PROMOTION_RUNING,	//正在进行中
	PROMOTION_PASSED,	//已经结束
	PROMOTION_COMING,	//今天还没到
	PROMOTION_NOTTODAY,	//不在今天
};

#define MAX_ACTIVE_DAY_TIME                      	10
#define MAX_ACTIVE_AWARD_ITEM                    	8
#define PROMOTION_BEACH_ID							18
#define PROMOTION_DARK_COMING_ID					23

#define MAX_REWARD_ITEM_COUNT MAX_ACTIVE_AWARD_ITEM

enum
{
	PROMOTION_TYPE_DAILY = 0,
	PROMOTION_TYPE_WEEKLY,
};
enum
{
	PROMOTION_SINGLE = 0,	//单人
	PROMOTION_TEAM,			//组队
};

enum
{
	PROMOTION_NOT_NEED_AUTOTEAM = 0,		//不需要在寻求组队里面显示
	PROMOTION_SINGLE_LIMIT,					//是单人活动，不需要在需求组队里面显示
	PROMOTION_NOT_START,					//需要显示，不能参加，活动没有在进行中
	PROMOTION_LEVEL_LIMIT,					//需要显示，不能参加，等级不够，
	PROMOTION_HAVE_FINISH,					//需要显示，不能参加，已经完成
	PROMOTION_OK,							//可以参加
	PROMOTION_NOT_TODAY,					// 不在今天
};

enum
{
	PROMOTION_REWARD_EXP = 1,	//奖励经验
	PROMOTION_REWARD_MONEY,		//钱
	PROMOTION_REWARD_EQUIP,		//装备
	PROMOTION_REWARD_GEM,		//宝石
	PROMOTION_REWARD_PET,		//宠物
	PROMOTION_REWARD_RIDE,		//坐骑
	PROMOTION_REWARD_STUFF,		//材料
};

#define WORLD_CITY_ACTIVE_HELP_ID_TOLUA WORLD_CITY_ACTIVE_HELP_ID

//活动是否开始数据
struct PromotionStartInfo
{
	int		startFlag;
	int		timeIdx;
};

struct tagPromotionTime 
{
	char	m_szStartTime[9];
	char	m_szEndTime[9];
};

struct tagPromotionDate 
{
	int					m_nID;
	int					m_nFlag;						// 0任务，1活动,2公会活动 Bind Macrosgroup:ACTIVE_HELPER_TYPE
	int					m_nIconID;						//图标ID
	char				m_szName[NAME_LEN];				//活动名称
	int					m_nType;						//日常|周常
	int					m_nWeekFlag;					//一周内开放标记
	char				m_szStartDate[9];
	char				m_szEndDate[9];

	int					m_nTimeCount;					//时间范围的个数
	tagPromotionTime	m_Time[MAX_ACTIVE_DAY_TIME];

	int					m_nLevel;		//等级限制
	int					m_nNeedTeam;	//是否需要组队
	int					m_nAutoTeamID;	//自动组队标记
	int					m_nRelateId;	//关联任务，或者副本ID
	int					m_nMainRewardType;
	int					m_nMainRewardLevel;
	char				m_szNpcName[MAX_MSG_DESC_LEN];
	char				m_szDesc[DESC_LEN];
	int					m_nReCommand;							//推荐1推荐0 不推荐	
	int					m_RewardItem[MAX_ACTIVE_AWARD_ITEM];	//奖励物品
	int					m_nRewardItemCount;						//奖励的数量
	bool				m_bShowInAutoTeam;
	int					m_nTimeShowType;						/*  Ver.1169 时间显示类型 0为显示表中的时间 1为显示会长开启 */
	int					TransMapID;								/*  Ver.1653 传送地图ID */
	tagResPos_ToLua		TransPos;								/*  Ver.1653 传送的坐标 */
	int					m_nBusyID;								/*  Ver.1865 我很忙ID */
	int					m_nHuoDongShow;							// 每日活动
};

struct AutoTeamData
{
	int npcID;
	int mapID;
	int proType;	//0 任务|1 活动
	int relateID;
	int nRow;
};

struct tagDailyGoalAward_ToLua
{
	int AwardID;                         	/*   奖励阶段ID */
	int Gold;                            	/*   金币 */
	int BindGold;                        	/*   绑定金卷 */
	int ItemID[MAX_DAILY_GOAL_AWARD_ITEM]; 	/*   奖励的物品ID */
	int ItemNum[MAX_DAILY_GOAL_AWARD_ITEM]; 	/*   奖励的物品数量 */
	int Condi;                           	/*  Ver.1669 条件|位操作 */
};

#pragma pack( 1 )
/* 世界boss定义表 */
struct tagWorldBossDef_ToLua
{
	int MonID;                           	/*   怪物ID */
	char FreshType;                       	/*   刷新类型 */
	int AwardIcon[MAX_WORLD_BOSS_ICON];  	/*   奖励图标最多8个 */
	int MapID;                           	/*   刷新地图 */
	int X;                               	/*   刷新坐标 */
	int Y;                               	/*   刷新坐标 */
	int LifeTime;                        	/*   存活时间:秒 */
	int PatrolID;                        	/*   巡逻ID */
	int DieFreshTime;                    	/*   死亡后过多久再次刷出:秒 */
	int DieFreshMonRate;                 	/*   死亡后刷怪概率:万分率 */
	int DieFreshMonID;                   	/*   死亡后刷那个怪 */
	int DieFreshMonTime;                 	/*   死亡后隔多久刷那个怪 */
	char DieFreshNum;                     	/*   死亡后刷怪次数:每天最多x次并且最少刷1次 */
	int Week;                            	/*   星期几,0表示不刷, 1000001--表示周1周日刷 */
	char FreshTime[9];                    	/*    hh:mm  */
	int TipTime;                         	/*   刷新前多少秒提示 */
	int TipChannel;                      	/*   提示频道 */
	char TipMsg[128];                     	/*   提示内容 */
	char FreshMsg[128];                   	/*   刷出提示 */
};
#pragma pack()

//tolua_end

enum
{
	PROMITION_FLAG_QUEST = 0,	//任务
	PROMITION_FLAG_PROMITION,	//活动
	PROMITION_FLAG_CLAN,		//公会
	PROMITION_FLAG_TASKGROUP, 	/* 任务组 */
};

enum
{
	PROMITION_TYPE_DAILY = 0,	//每日
	PROMITION_TYPE_WEEKLY,	//每周
};

class GamePromotion//tolua_export
	:public Ogre::Singleton<GamePromotion>
{//tolua_export
public:
	~GamePromotion( void );
	static GamePromotion* create( const char* szFileName );

	//tolua_begin
	
	int getAllNum();
	// 获取活动数量
	int		getPromotionNum();
	// 获取公会活动数量
	int		getClanProNum();
	// 排序活动
	int		sortPromition( int nProType );

	int		getAllRow(int nIndex);

	int		getPromotionRow( int nIndex );

	int		getClanProRow( int nIndex );

	// 重置活动相关数据
	void	resetProData();

	bool	isInDatePeriod( int nIndex );
	PromotionStartInfo* getCurrPromitionStartInfo( int nIndex );

	tagPromotionDate*	getPromotionData( int nRow );
	
	//判断指定分钟范围内
	bool	isInTime( const char* szStartTime,int min );

	int		getRepeatCount( int nRow );

	int		getFinishCount( int nRow );

	int		getSerHour();

	int		getSerMin();

	bool	isWeekOpen(int nRow,int nIndex);
	
	AutoTeamData*	getAutoTeamData( int teamID );

	int		getAutoTeamState( int nRowID );
	int		getAutoTeamStateByNoCountLimit( int nRowID );

	bool	isWeekToday(int nIndex);

	void	setPworldLimit(int pworldid,int num);

	void	promotionClearLimit();

	//是否是战场活动
	bool	isBattleActive( int nIndex );

	//获取活动显示名
	const char* getDisPlayName( int nRow );

	int		getDailyTaskNum();
	
	int		getDailyTaskID( int nIndex );

	int		GetDailyFiniNum();

	int		GetDailyAwardFlag();

	void	RequestGetDailyAward( int nIndex );

	int		GetDynActiveInfo( int nRow );

	int		getFirstOpenWeek( int nRow );

	tagDailyGoalAward_ToLua* getDailyGoalAward( int id );

	void	 RequestTransportToNPC( int nActiveID );

	int		getWorldBossNum(){ return (int)m_vecWorldBossData.size(); };
	const tagWorldBossDef_ToLua* getWorldBossDefByIndex( int nIndex );
	const tagWorldBossDef_ToLua* getWorldBossDef( int id );
	void	requestWorldBossInfo();
	int		getDieFreshMonsterID( int id );
	int		GetWeekDay( int nTime );
	bool	isBeachPromotion( int nProId );

	//tolua_end

	void	addDailyGoalTask( int taskID );

	void	SetDailyInfo( const CSACTIONDAILYGOALINFO& info );

	void	SetDynActiveInfo( const CSACTIONDYNACTIVEINFO& info );

	void	ResetCanApplyTask();

	void	Update();

	void	resetActivityGuidInfo();

	void	setPinJiuBlurTime( unsigned int tickTime );  // 设置拼酒屏幕模糊时间
private:
	GamePromotion( void );
	// 获取当前服务器标准时间
	struct tm*		getServerTimeTm();
	// 活动信息数据排序 nType 排序列 nFlag 1正序 -1 反序
	void		sortPromitionList( std::vector<int>& vectProData, int nType, int nFlag, int nProType);
	// 比较成员的大小
	int			cmpMember( const int src, const int des, const int nType,const int nProType );
	// 比较函数
	int			comp_func(const void * v1, const void *v2);
	// 设置当天活动时间
	void		setPromotionTime();
	// 设置活动数据
	void		setPromotion();
	// 设置公会数据
	void		setClanPro();

	void		sortDailyGoalList();

	bool		isFinishActive( int nRow );

private:
	std::vector<int>					m_vecAll;		//全部数据
	std::vector<int>					m_vectPromotion;// 副本活动 数据
	std::vector<int>					m_vectClanPro;	// 每周热点 数据
	std::map<int, tagPromotionDate>		m_mapPromotionTime;	// 活动 时间表

	std::map<int,int>					m_pWorldmap;	//副本完成次数

	std::vector<int>					m_vecDailyTask;
	std::vector<int>					m_vecNeedShowDailyTask;

	CSACTIONDAILYGOALINFO				m_DailyInfo;

	std::map<int,CSACTIONDYNACTIVEINFO> m_mapDynActiveInfo;
	std::map<int,bool>					m_mapActivityGuid; //活动引导中的各活动id及对应开始标记
	std::vector<tagWorldBossDef*>		m_vecWorldBossData;
	unsigned int						m_nPinJiuBlurStartTime;   // 拼酒--屏幕模糊时间
};//tolua_export
