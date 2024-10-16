#pragma once
#include "GameEngineTypes.h"
#include "OgreSingleton.h"
#include <map>
#include <set>
#include <string>
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameOpt.h"
#include "resdb_meta.h"

const int LOOSEITEM_DELAYTIME	= 300;
const int LOOSEITEM_DEATHTIME	= 500;

bool CompareRoleName(	const char* pszLeftOriginalName,	int nLeftWorldID, 
					 const char* pszRightOriginalName,	int nRightWorldID );

struct BaseWarMapOwnerInfo
{
	int nBaseWarMapID;
	int nMapID;
	tdr_ulonglong nOwnerClanWID;
};
#define MAX_BASE_WAR_MAP_CLAN_INFO 6

//tolua_begin

bool CompareRoleName( const char* pszLeftName, const char* pszRightName );

enum GameActorType
{
	GAT_NPC = 0,
	GAT_MONSTER,
	GAT_ITEM,
	GAT_PLAYER,
	GAT_MAINPLAYER,
	GAT_PET,
	GAT_FAIRY,
	GAT_EXTRA_NPC,

	GAT_OBJ_END,
};

enum
{
	PICK_TYPE_LBUTTON_DOWN = 0,
	PICK_TYPE_RBUTTON_DOWN = 1,
};

enum
{
	HIDE_MODEL_OPTION_MIN		= 1,
	HIDE_OTHER_PLAYER_AND_PET	= HIDE_MODEL_OPTION_MIN,
	HIDE_OTHER_PLAYER_AND_PET_AND_STALL,
	HIDE_MODEL_NONE,
	HIDE_MODEL_OPTION_MAX		= HIDE_MODEL_NONE,
};

enum tagMON_SUB_TYPE_TOLUA 
{
	MON_SUB_TYPE_MINI_TOLUA = MON_SUB_TYPE_MINI, 	/* 微 */
	MON_SUB_TYPE_WEAK_TOLUA = MON_SUB_TYPE_WEAK, 	/* 弱 */
	MON_SUB_TYPE_SOFT_TOLUA = MON_SUB_TYPE_SOFT, 	/* 软 */
	MON_SUB_TYPE_NORM_TOLUA = MON_SUB_TYPE_NORM, 	/* 普 */
	MON_SUB_TYPE_HARD_TOLUA = MON_SUB_TYPE_HARD, 	/* 硬 */
	MON_SUB_TYPE_XI_TOLUA = MON_SUB_TYPE_XI, 	/* 稀 */
	MON_SUB_TYPE_JIN_TOLUA = MON_SUB_TYPE_JIN, 	/* 精 */
	MON_SUB_TYPE_BOSS_TOLUA = MON_SUB_TYPE_BOSS, 	/* BOSS */
	MON_SUB_TYPE_JI_TOLUA = MON_SUB_TYPE_JI, 	/* 极 */
	MON_SUB_TYPE_SINGLE_CAR_TOLUA = MON_SUB_TYPE_SINGLE_CAR, 	/* 单人镖车 */
	MON_SUB_TYPE_TEAM_CAR_TOLUA = MON_SUB_TYPE_TEAM_CAR, 	/* 队伍镖车 */
	MON_SUB_TYPE_HUMAN_FRIEND_TOLUA = MON_SUB_TYPE_HUMAN_FRIEND, 	/* 帮助人类的怪物。与其他怪物敌对 */
	MON_SUB_TYPE_HUMAN_BROTHER_TOLUA = MON_SUB_TYPE_HUMAN_BROTHER, 	/* 帮助人类的怪物兄弟，享受人类的待遇。与其他怪物敌对 */
	MON_SUB_TYPE_MACHINE_TOLUA = MON_SUB_TYPE_MACHINE, 	/* 可以骑乘的怪物。有技能等各种属性 */
	MON_SUB_TYPE_CLAN_FACTORY_TOLUA = MON_SUB_TYPE_CLAN_FACTORY, 	/* 公会工厂 */
	MON_SUB_TYPE_TOTEM_TOLUA = MON_SUB_TYPE_TOTEM, 	/* 图腾 */
	MON_SUB_TYPE_TMP_TOLUA = MON_SUB_TYPE_TMP, 	/* 临时怪物 */
	MON_SUB_TYPE_TRAP_TOLUA = MON_SUB_TYPE_TRAP, 	/* 地雷 */
	MON_SUB_TYPE_COLLECT_TOLUA = MON_SUB_TYPE_COLLECT, 	/* 采集 */
	MON_SUB_TYPE_SINGLE_SAFECAR_TOLUA = MON_SUB_TYPE_SINGLE_SAFECAR, 	/* 不可打劫的单人镖车 */
	MON_SUB_TYPE_TEAM_SAFECAR_TOLUA = MON_SUB_TYPE_TEAM_SAFECAR, 	/*  不可打劫的队伍镖车 */
	MON_SUB_TYPE_BUILD_TOLUA = MON_SUB_TYPE_BUILD, 	/* 建筑类怪物，不可选中，不会被攻击，头顶无名字 */
	MON_SUB_TYPE_XUNLUO_TOLUA = MON_SUB_TYPE_XUNLUO, 	/* 巡逻怪,只会移动，不会追敌，不会主动攻击,不会追敌,没有压力表 */
	MON_SUB_TYPE_SKILL_DESTORY_TOLUA = MON_SUB_TYPE_SKILL_DESTORY, 	/* 技能怪,使用技能后消失 只会移动，不会追敌，不会主动攻击,不会追敌,没有压力表 */
	MON_SUB_TYPE_SKILL_NODESTORY_TOLUA = MON_SUB_TYPE_SKILL_NODESTORY, 	/* 技能怪,使用技能后不消失(到时间消失) 只会移动，不会追敌，不会主动攻击,不会追敌,没有压力表 */
};

//tolua_end


struct tagLoosItemPosInfo
{
	int m_nsX;
	int m_nsY;
	int m_nX;
	int m_nY;
	int m_nID;
	int m_nFromID;
	int m_droptype;
};

typedef std::map<int, GameActor*>::iterator ACTOR_ITER;
typedef std::vector<int>					PASSENGER_LIST;
typedef std::vector<int>::iterator			PASSENGER_LIST_ITER;
typedef std::map<int, PASSENGER_LIST>::iterator		OWNER_ITER;

class ActorManager : public NetMsgHandler, public Ogre::Singleton<ActorManager>//tolua_export
{//tolua_export
public:
	ActorManager();
	~ActorManager();
	//tolua_begin
	GameActor*			FindActor(int id);
	int					getActorIdByResId( int id );

	MainGamePlayer*		getMainPlayer() 
	{ 
		return m_pMainPlayer; 
	}

	GameActor*	findActorByName(const char* pszName);
	GameActor*  findActorByName( const char* pszName, int nType );
	GameActor*	findActorByWId( tdr_longlong wId );
	void		canPickTmpItem();
	void		tryPickMonsterMachine();
	void		pickTmpItem( int id );
	bool		needConvertName( const char* pszName );
	std::string	replaceSpecialCode( const std::string& strNeedConvertString  );
	// 获取周围玩家
	GameActor*	getNearestActor( int src, int radius = -1, bool bTask = false, int resid = 0, int actorType = -1 );

	bool isShowOtherPlayerTitle();
	// 获取是否显示玩家
	int getShowOtherPlayerModelOption();
	// 设置完美星特效
	void		SetPerfectStarNineActor();
	//tolua_end
	float		getActorDistance(int id);
	GameActor*	CreateActor( int id, GameActorType type, int resid, bool background = false );
	void		DestroyActor( GameActor *pactor, bool bDeletePet = false );
	void		DestroyAllActor( bool bDelMainPlayer = true, bool detach = false );
	void		Update( unsigned int dtick );
	bool		isActorIgnoreF12(GameActor* pOneActor);
	//bool		canShowActorModel(GameActor* pOneActor);
	
	// 添加骑乘等待列表
	void		pushWaitRideList( int nOwnerId, int nGuestId );
	// 获取骑乘列表
	void		popWaitRideList( int nOwnerId, std::vector<int>& passengerList );

	// 对象头顶信息绘制
	void		drawActorTitles( FollowCamera *pcamera );
	// 获取选择优先级
	GameActor*	getSelectActor( GameActor* pPreSelect, GameActor* pCurSelect );
	// 鼠标所在的对象
	GameActor*	mounseOnActor( const Ogre::WorldRay& worldray );
	// 对象拾取
	GameActor*	pickActor( const Ogre::WorldRay &worldray, int nPickType );
	// 物品抛出
	void		setItemChangeTime(unsigned int delay, int id);
	void		looseItemShow();
	void		addLooseItemShow(const tagLoosItemPosInfo &info);

	virtual int HandleNetMsg( int msgid, const tagCSPkgBody &msgbody );
	
	ACTOR_ITER getActorBegin() 
	{ 
		return m_Actors.begin(); 
	}

	ACTOR_ITER getActorEnd() 
	{ 
		return m_Actors.end();   
	}
	
	ACTOR_ITER getActorFind(int id)
	{
		return m_Actors.find(id);
	}

	void setPostSceneRenderer( Ogre::SceneRenderer* p ) 
	{ 
		m_pPostSceneRenderer = p; 
	}
	Ogre::SceneRenderer* getPostSceneRenderer() 
	{ 
		return m_pPostSceneRenderer; 
	}
	
	// 设置脚本
	void setScriptVM( Ogre::ScriptVM* p )
	{
		m_pScriptVM = p;
	}
	// 获取脚本
	Ogre::ScriptVM* getScriptVM()
	{
		return m_pScriptVM;
	}

	//tolua_begin
	void addClientSelfDestroy( GameActor* pActor );
	// 获取玩家数量
	int getPlayerCount();
	// 获取怪物数量
	int getMonsterCount();
	// 根据索引找到monster
	GameActor* getMonster( int nIndex );
	// actors总数
	int getActorNums();
	// 根据索引找到Actor
	GameActor* getActor( int nIndex );
	void updateMonsters( GameActor* pActor );

	//tolua_end
	// 获取NPC数量
	int getNpcCount();
	
	void resetShowModelAndTitleOption();
	// 设置是否显示玩家
	void changeShowOtherPlayerModel();
	bool canShowActorModelInScene( GameActor* pActor );

	// 设置是否显示队伍/称号名字
	void setShowItherPlayerTitle();
	// 更新所有玩家的头顶信息
	void updateAllTeamPlayerTitleInfo();
	void updateAllPlayerTitleInfo();
	void updateAllPlayerTitleColor();
	// 是否显示公会名字
	
	// 设置哪些信息要显示
	void setGameOpt( int nIndex, bool bShow );
	void setGameOptValue( int nIndex, char value );
	//tolua_begin
	// 获取对应显示信息列表
	bool getGameOpt( int nIndex );
	GameSettingConfig&	getGameConfig()
	{
		return m_GameConfig;
	}

	// 遍历获取下一个Actor
	GameActor *getNextActorById(int id);
	GameActor *getNextActor(GameActor *pre = NULL);
	void changeShowPlayerID();
	int getNextPlayerEnemy(int id = 0);
	int getServerId(const char* szName );

	const char* getClanFengHao( const tdr_ulonglong ClanWid );
	void setClanFengHao();

	//获取竞技场等级称号
	const char* GetSpartaLevelName( int nFightVal );
	//tolua_end

	void changeMainPlayerID(int newId);

	bool insertVirtualActor(GameActor *pActor);
	void UpdateAllActorClanName( tdr_ulonglong clanWID, const char* pszClanName );

	void WarFogUpdate(int wardistance, float x, float z);
	void updateActorstate(GameActor* pOneActor, bool bshow);
	void hideAllActors();

	void addActorFade(GameActor *who, bool isIn);
	void removeActorFade(GameActor *who);
	bool isInMainPlayerTeam(GameActor *who);
	void setMapOwnerWID( int mapID, tdr_ulonglong clanWID );

private:
	void OnChangeAllPlayerNameOpt();
	void InitBaseWarMapOwnerList();
private:
	Ogre::SceneRenderer*				m_pPostSceneRenderer;
	MainGamePlayer*						m_pMainPlayer;
	std::map<int, GameActor*>			m_Actors;
	std::map<int, int>					m_ActorsByResId;
	std::vector<GameActor *>			m_Monsters;
	std::set<GameActor *>				m_Players;
	std::vector<int>					m_ClientSelfDestoryActors;
	std::vector<std::pair<unsigned int, int>>	m_ItemsChangeOwner;

	std::map<int, int>					m_ItemDropCount;
	std::vector<std::pair<unsigned int, tagLoosItemPosInfo>>		m_ItemTimeArray;
	Ogre::ScriptVM*						m_pScriptVM;

	int									m_nShowModelOption;
	bool								m_bShowOtherPlayerTitle;

	std::map<int, int>					m_attrMapToHurt;
	std::map<int, PASSENGER_LIST>		m_waitRideList;	// 等候骑乘列表
	std::map<GameActor *, std::pair<float, bool>>		m_fadeActors;
	GameSettingConfig					m_GameConfig;
// 	std::vector<MONMACHINEINFO>			m_monsterMachineWaiters;

	struct HeapCmp
	{
		GameXYZ basePos;
		bool operator()(GameActor *_Left, GameActor *_Right) const;
		
	}									m_heapCmp;/// 最小堆的比较仿函数

	template<class T>
	struct TimeHeapCmp
	{
		bool operator()(const std::pair<unsigned, T> &_Left, const std::pair<unsigned, T> &_Right) const;
	};
public:
	std::map<int, int>	lastAttacker;
	std::set<int>		playerEnemy;
	bool				m_bShowPlayerID;
	int					m_RequestMachineId;

	BaseWarMapOwnerInfo						m_BaseWarMapOwnerList[MAX_BASE_WAR_MAP_CLAN_INFO];		//据点信息
	std::map<tdr_ulonglong, std::string>	m_ClanFengHao;		//公会封号
	std::vector<tagSpartaLevelDef*>			m_SpartaLevelData;	//竞技场等级数据
};//tolua_export
