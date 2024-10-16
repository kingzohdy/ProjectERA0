#pragma once
#include "GameEngineTypes.h"
#include "NetMessage.h"
#include "OgreSingleton.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "GameMsgPrint.h"
#include <map>

//tolua_begin
enum  
{
	CLIENT_TASK_STAT_NO		= -1,	
	CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT	= 0, 
	CLIENT_TASK_STAT_FINI	= 1, 
	CLIENT_TASK_STAT_FAIL	= 2, 
	CLIENT_RECYCLE_TASK_STAT_CAN_ACCEPT = 3, 
	// CLIENT_TASK_STAT_CANNOTACCEPT = 4,
};

enum
{
	CLAN_GIFT_NPC_ID = 304059,
	CLAN_INTELLINGENCE_NPC_ID = 284300,
};

enum
{
	CLIENT_TASK_TYPE_NEW = 0, 		/* 新手任务 */
	CLIENT_TASK_TYPE_MAIN, 			/* 主线任务 */
	CLIENT_TASK_TYPE_BRANCH, 		/* 支线任务 */
	CLIENT_TASK_TYPE_BUSSINESS, 	/* 经商任务 */
	CLIENT_TASK_TYPE_ACT, 			/* 活动任务 */
	CLIENT_TASK_TYPE_REP, 			/* 重复任务 */
	CLIENT_TASK_TYPE_GOLD, 			/* 黄金任务 */
	CLIENT_TASK_TYPE_PWORLD, 		/* 副本任务 */
	CLIENT_TASK_TYPE_DISCOVER, 		/* 探索任务 */
	CLIENT_TASK_TYPE_SINGLE_CONVOY, /* 单人护送任务 */
	CLIENT_TASK_TYPE_TEAM_CONVOY, 	/* 队伍护送任务 */
	CLIENT_TASK_TYPE_CLAN,          /* 公会任务 */
	CLIENT_TASK_TYPE_TRUST,			/* 委托任务 */
	CLIENT_TASK_TYPE_CHG_CAREER,	/* 转职任务 */
	CLIENT_TASK_TYPE_RIDE,			/* 坐骑任务 */
	CLIENT_TASK_TYPE_FAIRY,			/* 精灵任务 */
	CLIENT_TASK_TYPE_LIFE,			/* 生活任务 */
	CLIENT_TASK_TYPE_CHG_RECYCLE,	/* 环组任务 */
	CLIENT_TASK_TYPE_DAILY_GOAL,	/* 今日目标任务 */
	CLIENT_TASK_TYPE_SJ_MON, 	/* 赏金剿灭令 */
	CLIENT_TASK_TYPE_SJ_ITEM, 	/* 赏金收集令令 */
	CLIENT_TASK_TYPE_SJ_BOSS, 	/* 赏金击杀令 */
	CLIENT_TASK_TYPE_DART_CAR, 	/* 商运镖车任务 */
	CLIENT_TASK_TYPE_DESIGNATION,		/* 称号任务 */
	CLIENT_TASK_TYPE_RONGLU, 	/* 熔炉任务 */
	CLIENT_TASK_TYPE_XIEZHU,	/* 协助任务 */		
	CLIENT_TASK_TYPE_BATTLE,	/* 战场任务 */
	CLIENT_TASK_TYPE_PAOSHANG,	/* 跑商任务 */

	CLIENT_TASK_TYPE_END,	
};

enum  
{
	CLIENT_TASK_DIFFICULT_NORM		= 0, 	/* 简单 */
	CLIENT_TASK_DIFFICULT_NOEASY	= 1, 	/* 复杂 */
	CLIENT_TASK_DIFFICULT_HARD		= 2, 	/* 困难 */
};

enum
{
	MAP_TYPE_COPY		= 1,	// 副本
	MAP_TYPE_OUTSIDE	= 2,	// 野外
	MAP_TYPE_ALL		= 3,	// 全部
};

enum
{
	CLINT_CAN_NOT_ACCEPT_TASK	= 0,
	CLINT_CAN_ACCEPT_TASK		= 1,
	CLINT_CAN_NOT_ACCEPT_TASK_BY_LITTLE_LV	= 2,
};

struct tagTaskMoney_toLua
{
	unsigned int Type;                            	/*   货币种类 */
	unsigned int Val;                             	/*   多少 */
};
struct tagSJTaskMoney_tolua
{
	int Type;                           
	int Val;                            
};

struct tagSJTaskStar_tolua
{
	int Rate;                           
	int ExpMul;                         
	int MoneyMul;                       
};

struct tagSJTaskDef_tolua
{
	unsigned int ID;                              	/*   赏金类ID */
	tagSJTaskMoney_tolua RefreshTaskMoney;                	/*   赏金刷新任务消耗 */
	tagSJTaskMoney_tolua RefreshStarMoney;                	/*   赏金刷新星级消耗 */
	tagSJTaskStar_tolua StarRate[MAX_SJ_TASK_STAR];      	/*   星级信息 */
	int MaxCostNum;                      	/*  Ver.1971  */
};
struct tagDiscoverInfo 
{
	bool	m_bFiniTask; 
	int		m_nInAreaTime;
};

struct tagTimeAccumQuestInfo
{
	int		m_nMaxTime;		// 以秒为单位
	int		m_nTaskID;
	int		m_nTimeMode;
	int		m_nPassTime;	// 以秒为单位
};

struct tagOneTaskInfo
{
	int		m_nTaskID;
	int		m_nTaskState;
};

enum
{
	CLIENT_MAX_TASK_AWARD_ITEM			= MAX_TASK_AWARD_ITEM,
	CLIENT_MAX_RECYCLE_TASK_AWARD_ITEM	= MAX_RECYCLE_TASK_AWARD_ITEM,
};

enum
{
	CLIENT_TASK_RECYCLE_NORMAL	= 0,
	CLIENT_TASK_RECYCLE_CLAN	= CYC_TYPE_CLAN,
	CLIENT_TASK_RECYCLE_NO_SHOW	= 2,
};

#define SJ_ITEM_ID_TOLUA SJ_ITEM_ID
//tolua_end

typedef std::map<int, TASKONEDAYINFO>::iterator TASKONE_ITER;

class GameQuest//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GameQuest>, public IGameMsgPrint
{//tolua_export
public:
	GameQuest(void);
	~GameQuest(void);
	
public:
	// c->s
	
	//tolua_begin
	// 获取任务
	void getQuest();
	// 接受任务
	void acceptQuest( unsigned int npcId, int questId );
	// 角色领取奖励
	void awardQuest( unsigned int npcId, int questId, int selectId = 0 );
	// 角色放弃任务
	void dropQuest( int questId );
	// 杀怪或者收集完成后结束任务
	void finishQuest( int questId );
	//活动助手完成任务
	void finishActiveQuest( int questId );
	// 共享任务
	void shareQuest( int questId );
	// 拒绝任务
	void refuseQuest( int questId );
	void AutoQuest();
	//tolua_end

	// s->c
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

public:
	//tolua_begin
	// 获取任务名称
	const char* getQuestName( int questId );
	// 获取道具奖励类型
	int getAwardType( int questId, int nRecycleID = 0, int nMaxIdx = 1 );
	// 获取可接任务所在场景
	const char* getQusetScene( int questId );
	// 获取任务类型
	int getQuestType( int questId );
	//获取已接受赏金任务个数
	int getSJTaskNum();
	// 获取任务难度
	int getQuestDifficult( int questId );
	// 获取任务星级
	int getQuestStar( int questId );
	// 获取任务最低等级要求
	int getQuestLevelMin( int questId );
	// 任务描述
	const char* getQuestDes( int questId );
	// 任务对话
	const char* getQuestDialog( int questId );
	const char* getQuestFiniDialog( int questId );
	// 获取当前任务的环数
	int getTaskRecycleIdx( int questId, int nRecycleID );
	int getTaskRecycleIdx( int nRecycleID );
	// 是否显示环组次数文字
	bool isShowRecycleDesc( int questId, int nRecycleID );
	// 获取环组任务的描述
	const std::string getRecycleDes( int questId, int nRecycleID );
	//获取任务地图id
	unsigned int getQuestMapID( int questId );

	// 获取任务一天可以完成的次数
	int getQuestMaxCanAcceptNumOneDay( int questId );
	// 获取任务完成次数
	int getQusetHaveFinishNumOneDay( int questId );
	// 获取环组任务完成次数
	int getReCycleQusetOneDayNum( int nCycleId );
	// 获取组任务完成次数
	int getGroupQusetOneDayNum( int nGroupId );
	// 添加任务完成次数
	void addQusetNumOneDay( int questId );
	// 获取组任务每周完成次数
	int getGroupQuestWeekNum( int nGroupId );
	// 添加组任务每周完成次数
	void AddGroupQuestWeekNum( int questId );
	// 是否是环组任务
	bool isRecycleTask( int questId );
	bool isRecycleTask( const tagTaskDef& taskDef );

	// 当放弃任务的时候更新环组任务当前的环数
	void updateRecycleIdxByDrop( const tagTaskDef& taskDef, int nRecycleIdx, int nRecycleID );
	// 当放弃任务的时候更新环组任务一天已经做的轮数
	void updateRecycleOneDayInfoByDrop( const tagTaskDef& taskDef, int nRecycleID );

	// 当提交任务的时候更新环组任务当前的环数
	void updateRecycleIdxByCommit( const tagTaskDef& taskDef, int nRecycleIdx, int nRecycleID );
	// 当提交任务的时候更新环组任务一天已经做的轮数
	void updateRecycleOneDayInfoByCommit( const tagTaskDef& taskDef, int nRecycleIdx, int nRecycleID );
	
	// 获取任务完成时间
	int getQusetTime( int questId ); 
	// 00:00后重置当所有委托完成次数
	void resetQusetNumOneDay(); 
	// 任务是否达到完成次数限制
	bool isQuestHaveFinishOneDayMaxNum( int questId );

	// 重新组织委托任务数据
	void reSetTrust( int nMapId = 0 );
	// 设置委托任务区域
	void setTrustArea( int nArea );
	// 选择所有的委托任务区域
	void clearTrustArea( );
	// 设置委托任务区域类型 1为副本 2 为野外
	void setTrustAreaType( int nType );
	// 是否是委托任务区域
	bool IsTrustArea( int nArea );
	// 是否是委托任务区域类型
	bool IsTrustAreaType( int nType );
	// 获取指定地图的委托任务数量
	int	getMapTustNum( int nMapId );
	// 是否是委托任务
	bool IsTrustQuest( int questId );

	// 获取委托任务数据
	int getTrustPageInfo( int nPage );
	// 获取委托任务等级
	int getQuestLvLimit( int questId );
	// 获取委托任务需求总物品
	int getQuestNeedItemNum( int questId );
	// 获取委托任务需求物品
	int getQuestNeedItem( int questId, int index );
	// 获取委托任务需求物品单个数量
	int getQuestNeedItemCount( int questId, int index );
	// 获取委托任务数量
	int getNumTrust();
	// 选定所有的委托任务
	void selectAllTrustTask( bool bSelect );
	// 是否所有的委托任务都被选定
	bool isAllTrustSelect();

	// 获取任务奖励物品总数量
    int getQuestItemNum( int questId, int nRecycleID, bool bFinalAward = false );
	// 获取任务奖励物品
    int getQuestItem( int questId, int nRecycleID, int index, bool bFinalAward = false );
    // 获取任务奖励单个物品数量
    int getQuestItemCount( int questId, int nRecycleID, int index, bool bFinalAward = false );
    
    // 获取环组任务的总次数
    int getTaskCycleNum( int questId, int nRecycleID );

	// 获取任务奖励物品所需要职业
	int getQuestItemCareer( int questId, int nRecycleID, int index );
	// 测试职业是否符合任务奖励物品需求
	bool isQuestItemCareer(int questId, int nRecycleID, int index, int race);
	
	
	// 获取任务奖励给玩家的公会个人贡献度
	int getQuestClanContribute( int questId, int nRecycleID, bool bFinalAward = false );
	// 获取公会安定度奖励
	int getQuestClanAnDing( int questId, bool bFinalAward = false );
	// 获取公会资源奖励
	int getQuestClanResourceId(int questId, unsigned int index);
	int getQuestClanResourceNum(int questId, unsigned int index);
	// 获得当前环至少接取的任务数量
	int getRecycleLeastTaskNum( int nRecycleID );
	// 获得任务的当前环数
	int getQuestRecycleIndex( int questId, int nRecycleID );
	const CYCAWDRATE* getRecycleAwardInfo( int questId, int nRecycleID );
	// 获取某环的奖励
	int getRecycleTaskAwardRate( int questId, int nRecycleID );
	// 获取公会建设度奖励
	int getQuestClanBuildingValue( int questId, int nRecycleID, bool bFinalAward = false );
	// 获取公会资金奖励
	int getQuestClanMoney( int questId, int nRecycleID, bool bFinalAward = false );
	// 获取公会科技点奖励
	int getQuestClanTechPoint( int questId, bool bFinalAward = false );
	// 是否是动态奖励
	bool isDynamicAwardMoneyAndExp( int questId, int nRecycleID );
	// 获取任务奖励金钱
	tagTaskMoney_toLua* getQuestMoney( int questId, int nRecycleID, bool bFinalAward = false );
	// 获取任务奖励经验
	unsigned int getQuestExp( int questId, int nRecycleID, bool bFinalAward = false );
	// 获取任务奖励称号
    const char* getAwardDesignation( int questId, int nRecycleID, bool bFinalAward = false );
    // 获取任务奖励声望名
    const char* getAwardRespactName( int questId, int nRecycleID, bool bFinalAward = false );
    // 获取任务奖励声望值
    unsigned int getAwardRespactValue( int questId, int nRecycleID, bool bFinalAward = false );
	// 获得任务奖励关键字属性
	int getQuestFixedAttr( int questId, int nRecycleID, int index );
	// 获得任务奖励的装备绑定及孔属性
	int getQuestBindingAndSlot( int questId, int nRecycleID, int index );
	// 获得奖励生活技能名
	const char* getAwardSecondarySkillName(int questId);
	int getAwardSecondarySkillVal( int questId );
	void PrintQuestState( int nQuestID, int nQuestState, const char* pszSrcServeInfo );
	// 获取已接取任务数量
	int getNumQusetRun();
	
	// 获取已接任务
	int getQuestRunId( int index );
	// 该任务是否已经接取
	bool isQuestRun( int id );
	// 删除累积计时任务
	void delTimeAccumQuest( int questId );
	// 删除已接取任务
	void delQuestRun( int questId );
	// 获取已接取任务所属环组ID
	int getQuestRunRecycleID( int questId );
	// 获取任务完成情况
	int getQuestRunState( int questId );
	// 获取已接任务要求描述
	const char* getQuestRunReqDesc( int questId );
	// 获取任务完成后追踪描述
	const char* getQuestFiniTraceDes( int questId );
	// 获取任务完成后追踪描述位置
	std::string getQuestFiniTraceDespos( int questId );
	//获取赏金未接任务目标描述
	const char* getSJQuestTraceNoFiniTip( int questId );
	// 获取任务未完成任务追踪提示
	// (注意：在任务查询面板中就算完成也是用该描述信息)
	const char* getQuestNoFiniTraceDes( int questId );
	// 获取任务未完成任务追踪提示
	// (注意：在任务查询面板中就算完成也是用该描述信息)
	std::string getQuestNoFiniTraceDespos( int questId );
	// 获得任务总通用进度
	int getQuestMaxGenericProgress( int questId);
	// 获得任务当前通用进度
	int getQuestGenericProgress( int questId);
	
	// 刷新可接任务
	void updateCanAcceptQuest();
	// 获取可接任务数量
	int getNumCanAcceptQuest();
	// 获取可接任务任务
	int getCanAcceptQuestId( int index );
	// 获取可接任务要求描述
	const char* getCanAcceptQuestReqDesc( int questId );
	// 获取可接任务要求描述位置
	std::string getCanAcceptQuestReqDescpos( int questId );
	// 获取任务描述信息
	const char* getQuestReqDesc( int questId );

	// 获取任务追踪数量
	int getNumQuestTrace();
	// 获取任务追踪
	int getQuestTrace( int index );
	// 获取任务追踪Tips提示
	const char* getQuestRunTip( int questId );
	// 获取任务追踪对象的坐标
	std::string getQuestRunTippos( int questId );
	// 添加任务追踪
	void addQuestTrace( int questId );
	// 删除任务追踪
	void delQuestTrace( int questId );
	// 检测任务是否被追踪
	bool isQuestTrace( int questId  );
	
	// TODO: REFA 任务是否可以接取 0 不可接 1 可接 2 下一等级可接
	int canAcceptQuest( int nQuestId );
	// 环组任务是否可接取
	bool canAcceptRecycleQuest( int nPayerLv, int nRecycleId, int npcId );
	// TODO: REFA 任务采集物是否可以采集
	bool canCollectQuest( int nMonResId );
	// 任务是否是共享任务
	bool isShareQuest( int questId );
	
	// 显示怪物提示
	void showKillMonsterInfo( const CSTASKRUNONE& newOneTaskData, const CSTASKRUNONE& oldOneTaskData );
	// 显示物品提示
	void showCollectItemInfo( bool bShow = true );
	
	// 获取任务的超链接
	bool getMissionLink( int nQuestId, int nRecycleID ); 
	
	// 检查是否是需要累积计时的任务
	bool isTimeAccumQuest( int nTaskID );
	// 任务是否限时任务
	bool isQuestTimeLimit( int nQuestId ); 

	// 获取任务的剩余时间
	unsigned int getTaskEndTime( int nQuestId ); 
	
	// 是否已经接取了任务
	bool isHaveAcceptQuest( int questId );
	// 是否曾经完成过任务
	bool isQuestHaveFiniEver( int questId );
	// 任务是否已经做过
	bool isQuestFini( int questId );
	
	// 
	// 设置任务是否全部追踪
	void setQuestTraceAll( bool bTraceAll );
	
	// 任务是否全部追踪
	bool isQuestTraceAll(){ return m_bTraceAll; }
	
	// 获得任务类型名称
	const char* getQuestTypeName( int nType );

	// 获取运镖任务所需要的压金
	tagTaskMoney_toLua* getConvoyQuestMoney( int questId );

	// 获取任务一天完成的次数
	const int getQuestOneDayFiniNum( int questId, int nRecycleID );
	
	// 判断该任务是否是不限制次数的
	bool isQuestLimitNumOneDay( int questId, int nRecycleID );
	// 获得任务一天完成的限制次数
	const int getQuestOneDayLimitNum( int questId, int nRecycleID );
	
	// 保存追踪信息到服务器
	void saveTaskTraceOpt();

	// 清空任务列表
	void clearQuest();

    // 更新当前地图NPC头顶任务标志
	void            updateNpcQuestTitle( int nMapID, int nRealMapID );
    void            updateNpcQuestTitle();
	
	// 获取需要累积计时的任务
	tagTimeAccumQuestInfo*	getTimeAccumQuest( int questId );
	tagOneTaskInfo&			getChgTaskInfo();
	const tagTaskDef*		getTaskDef( int questId );
	bool			IsNeedCollectItem( int questId ,int index );
	bool			IsNeedMonster(int questId);
	//获得任务所需要的怪物ID
	int				getQuestMonsterId(int questId);
	//获得任务所需要的道具ID
	int				getQuestItemId(int questId);
	//获得任务特殊标记处的OBJ
	int				getQuestSpecialFiniObjVal(int questId,int index);
	//获得任务特殊标记处的OBJConditionVal
	int				getQuestSpecialFiniConditionVal(int questId,int index);
	int				getFirstCanAcceptRecycleTaskID( int questId );

	//使用赏金任务物品
	void UseBountyHunterItem( int itemID );
	//刷新赏金任务
	void RefurbishBountyHunterTask( int sjID );
	//刷新赏金任务星级
	void RefurbishBountyHunterStar( int sjID, int nflag = 0 );
	//接受赏金任务
	void AcceptBountyHunterTask( int itemID, int SJID, int TaskID );
	//获取赏金任务类定义
	const tagSJTaskDef_tolua* getSJTaskDef( int SJID );
	//判断是否必做任务
	bool isQuestMust( int questId );
	// 通过环组ID获取已接取任务
	CSTASKRUNONE*	getQuestRunByRecycle( int recycleId );
	// 获取可接接取任务 
	bool			getQuestCanAccept( int questId );
	//获得npc任务id
	int				GetNpcQuestID( int npcID );
	bool checkQuestMonsterKilled( int questId, int index );
	// 使用物品打开环组任务面板
	void RequestRecycleTaskByItem( int nListType, int nGridIdx );
	// 接受物品环组
	void AcceptRecycleTaskByItem( int nListType, int nGridIdx, int nTaskID );
	//tolua_end
	void	SetConvoyInfo( bool bGetNewTask, int nQuestId );
	
	bool canGetDynamicTask( int nQuestID );
	void addDynamicGetTask( const int* pQuest, int nNum );
	void setDynamicGetTask( const int* pQuest, int nNum );
public:
	
	void			updateOneDayQuest();
	void			updateCollectActors();
	virtual void	update( unsigned int dtick );
    // 对象进入视野
    virtual void    npcEnterView( int id );
	// 到达指定区域自动完成任务
	void			updateAutoFiniQuest( CSTASKRUNONE& questRun, const tagTaskDef& taskDef );
	// 获取已接取任务
	CSTASKRUNONE*	getQuestRun( int questId );
	
	bool			canAddFiniQuest( int questId );
	// 添加一个已完成任务
	void			addQuestFini( int questId );
	void			loadTaskTraceOpt();
	
	unsigned long*	getTaskTraceData()
	{	
		return &m_ulTraceData; 
	}

	bool			IsCollectHasItem( int nMonsterID );
	bool			IsCollectNeedItem( int nMonsterID );
	int				getFirstNotLvLimitRecycle( int questId );

	TASKONE_ITER	getTaskOnedayInfoBegin();
	TASKONE_ITER	getTaskOnedayInfoEnd();
private:
	// TODO: REFA 显示头顶任务提示
	void			showQuestTitle( int npcId );
	// 是否在指定区域
	bool			isInArea( tagDiscoverInfo& discoverInfo, const tagTaskDef& taskDef, int nTaskId, int nTaskState );
	// 检查是否满足累积计时的任务
	bool			isSatisfyTimeAccumQuest( CSTASKRUNONE& runQuest, const tagTaskDef& taskDef );
	
	// 更新需要累积计时的任务
	void			updateTimeAccumQuest( CSTASKRUNONE& runQuest, const tagTaskDef& taskDef );
	// 更新倒计时任务
	void			updateTimeCountDownQuest( CSTASKRUNONE& runQuest );
	// 检查是否满足要收集物品或打怪
	bool			isSatisfyItemOrMonsterQuest( CSTASKRUNONE& runQuest, const tagTaskDef& taskDef );
	// 更新需要道具或者打怪的任务
	void			updateItemOrMonsterQuest( CSTASKRUNONE& runQuest, const tagTaskDef& taskDef, bool bShowMsg );
	// 登陆时设置任务状态
    void			updateAllTaskState( bool bShowMsg );
    // 环组任务接取等级限制
    bool            isLimitByLv( const tagTaskRecycleDef& taskRecycleDef, int nPlayerLv );
    // 环组任务组限制
    bool            isLimitByGroup( const tagTaskRecycleDef& taskRecycleDef );
    // 环组任务类型限制
    bool            isLimitByMutex( const tagTaskRecycleDef& taskRecycleDef );
    // 环组任务每天完成次数的限制
    bool            isLimitByOneDay( const tagTaskRecycleDef& taskRecycleDef );
    // 环组任务当前进行环的限制
	bool            isLimitByRecycleIdx( int nRecycleId, int nNpcId );
	// 环组任务是否受公会限制
	bool			isLimitByClan( const tagTaskRecycleDef& taskRecycleDef );
	// 环组任务是否受前置任务限制
	bool			isLimitByPreTaskID( const tagTaskRecycleDef& taskRecycleDef );
	// 是否是固定顺序接取的环组任务
	bool			isFixedSequenceRecycleTask( int questId );
	bool			isFixedSequenceRecycleTask( const tagTaskDef& taskDef );
	bool			canAcceptRecycleTask( int nRecycleID );
	int				getFirstCanAcceptRecycleTaskID( const tagTaskDef& taskDef );
	bool			canAcceptRecycleTask( const tagTaskDef& taskDef );
	bool			isBelongToRecycleTask( const tagTaskDef& taskDef, int nRecycleID );
	
	void			loadRecycleTaskPreTaskFixIndex();

	void			OnTaskChg( const CSTASKRUNONE& oneChgTaskData );
	void			OnCommitTask( const CSTASKRUNONE& oneChgTaskData );
	void			OnDropTask( const CSTASKRUNONE& oneChgTaskData );
	void			OnUpdateTaskState( const CSTASKRUNONE& oneChgTaskData );
	void			OnUpdateCollectTaskState( CSTASKRUNONE* pOneRunQuest );

private:
	std::vector<CSTASKRUNONE>			m_QuestRuns;			// 已经接取的任务
	std::vector<int>					m_CanAcceptQuest;		// 可接取的任务列表		
	std::vector<int>					m_QuestTrace;			// 任务追踪列表
	std::vector<int>					m_FiniQuest;			// 已完成任务列表
	std::vector<tagTaskDef*>			m_TaskDef;				// 所有任务列表
	std::map<int,CSTASKRECYCLEINFO>		m_TaskRecycleInfo;		// 环组任务列表
	std::multimap<int, int>				m_TrustMap;				// 可接取委托列表	
	std::map<int, TASKONEDAYINFO>		m_TrustOneDayInfo;		// 一天可完成次数
	std::map<int, TASKCYCLEONEDAYINFO>	m_RecycleOneDayInfo;	// 一天可完成的环组次数
	std::map<int, GROUPONEDAYINFO>		m_GroupOneDayInfo;		// 组任务每天完成次数
	std::map<int, TASKGROUPWEEKINFO>	m_GroupWeekInfo;		// 组任务每周完成次数
	std::map<int, int>					m_PreTime;				// 限时任务的开始时间
	std::map<int, int>					m_QuestRecycleIdx;		// 环组任务当前进行的环
	std::map<int, tagDiscoverInfo>		m_mapDiscoverInfo;		// 探索任务的开始时间
	std::vector<tagTimeAccumQuestInfo>	m_timeAccumQuestRuns;	// 已经接取的需要计算累计时间的任务
	std::vector<int>					m_FinishedDynamicGetTasks;
	int									m_nTrustArea;
	bool								m_bSelectAll;
	bool								m_bTraceAll;			// 任务是否全部追踪

	std::map<int, int>					m_PreItemCount;
	unsigned long						m_ulTraceData;

	tagOneTaskInfo						m_chgTaskInfo;
};//tolua_export
