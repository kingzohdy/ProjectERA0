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
	MON_SUB_TYPE_MINI_TOLUA = MON_SUB_TYPE_MINI, 	/* ΢ */
	MON_SUB_TYPE_WEAK_TOLUA = MON_SUB_TYPE_WEAK, 	/* �� */
	MON_SUB_TYPE_SOFT_TOLUA = MON_SUB_TYPE_SOFT, 	/* �� */
	MON_SUB_TYPE_NORM_TOLUA = MON_SUB_TYPE_NORM, 	/* �� */
	MON_SUB_TYPE_HARD_TOLUA = MON_SUB_TYPE_HARD, 	/* Ӳ */
	MON_SUB_TYPE_XI_TOLUA = MON_SUB_TYPE_XI, 	/* ϡ */
	MON_SUB_TYPE_JIN_TOLUA = MON_SUB_TYPE_JIN, 	/* �� */
	MON_SUB_TYPE_BOSS_TOLUA = MON_SUB_TYPE_BOSS, 	/* BOSS */
	MON_SUB_TYPE_JI_TOLUA = MON_SUB_TYPE_JI, 	/* �� */
	MON_SUB_TYPE_SINGLE_CAR_TOLUA = MON_SUB_TYPE_SINGLE_CAR, 	/* �����ڳ� */
	MON_SUB_TYPE_TEAM_CAR_TOLUA = MON_SUB_TYPE_TEAM_CAR, 	/* �����ڳ� */
	MON_SUB_TYPE_HUMAN_FRIEND_TOLUA = MON_SUB_TYPE_HUMAN_FRIEND, 	/* ��������Ĺ������������ж� */
	MON_SUB_TYPE_HUMAN_BROTHER_TOLUA = MON_SUB_TYPE_HUMAN_BROTHER, 	/* ��������Ĺ����ֵܣ���������Ĵ���������������ж� */
	MON_SUB_TYPE_MACHINE_TOLUA = MON_SUB_TYPE_MACHINE, 	/* ������˵Ĺ���м��ܵȸ������� */
	MON_SUB_TYPE_CLAN_FACTORY_TOLUA = MON_SUB_TYPE_CLAN_FACTORY, 	/* ���Ṥ�� */
	MON_SUB_TYPE_TOTEM_TOLUA = MON_SUB_TYPE_TOTEM, 	/* ͼ�� */
	MON_SUB_TYPE_TMP_TOLUA = MON_SUB_TYPE_TMP, 	/* ��ʱ���� */
	MON_SUB_TYPE_TRAP_TOLUA = MON_SUB_TYPE_TRAP, 	/* ���� */
	MON_SUB_TYPE_COLLECT_TOLUA = MON_SUB_TYPE_COLLECT, 	/* �ɼ� */
	MON_SUB_TYPE_SINGLE_SAFECAR_TOLUA = MON_SUB_TYPE_SINGLE_SAFECAR, 	/* ���ɴ�ٵĵ����ڳ� */
	MON_SUB_TYPE_TEAM_SAFECAR_TOLUA = MON_SUB_TYPE_TEAM_SAFECAR, 	/*  ���ɴ�ٵĶ����ڳ� */
	MON_SUB_TYPE_BUILD_TOLUA = MON_SUB_TYPE_BUILD, 	/* ������������ѡ�У����ᱻ������ͷ�������� */
	MON_SUB_TYPE_XUNLUO_TOLUA = MON_SUB_TYPE_XUNLUO, 	/* Ѳ�߹�,ֻ���ƶ�������׷�У�������������,����׷��,û��ѹ���� */
	MON_SUB_TYPE_SKILL_DESTORY_TOLUA = MON_SUB_TYPE_SKILL_DESTORY, 	/* ���ܹ�,ʹ�ü��ܺ���ʧ ֻ���ƶ�������׷�У�������������,����׷��,û��ѹ���� */
	MON_SUB_TYPE_SKILL_NODESTORY_TOLUA = MON_SUB_TYPE_SKILL_NODESTORY, 	/* ���ܹ�,ʹ�ü��ܺ���ʧ(��ʱ����ʧ) ֻ���ƶ�������׷�У�������������,����׷��,û��ѹ���� */
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
	// ��ȡ��Χ���
	GameActor*	getNearestActor( int src, int radius = -1, bool bTask = false, int resid = 0, int actorType = -1 );

	bool isShowOtherPlayerTitle();
	// ��ȡ�Ƿ���ʾ���
	int getShowOtherPlayerModelOption();
	// ������������Ч
	void		SetPerfectStarNineActor();
	//tolua_end
	float		getActorDistance(int id);
	GameActor*	CreateActor( int id, GameActorType type, int resid, bool background = false );
	void		DestroyActor( GameActor *pactor, bool bDeletePet = false );
	void		DestroyAllActor( bool bDelMainPlayer = true, bool detach = false );
	void		Update( unsigned int dtick );
	bool		isActorIgnoreF12(GameActor* pOneActor);
	//bool		canShowActorModel(GameActor* pOneActor);
	
	// �����˵ȴ��б�
	void		pushWaitRideList( int nOwnerId, int nGuestId );
	// ��ȡ����б�
	void		popWaitRideList( int nOwnerId, std::vector<int>& passengerList );

	// ����ͷ����Ϣ����
	void		drawActorTitles( FollowCamera *pcamera );
	// ��ȡѡ�����ȼ�
	GameActor*	getSelectActor( GameActor* pPreSelect, GameActor* pCurSelect );
	// ������ڵĶ���
	GameActor*	mounseOnActor( const Ogre::WorldRay& worldray );
	// ����ʰȡ
	GameActor*	pickActor( const Ogre::WorldRay &worldray, int nPickType );
	// ��Ʒ�׳�
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
	
	// ���ýű�
	void setScriptVM( Ogre::ScriptVM* p )
	{
		m_pScriptVM = p;
	}
	// ��ȡ�ű�
	Ogre::ScriptVM* getScriptVM()
	{
		return m_pScriptVM;
	}

	//tolua_begin
	void addClientSelfDestroy( GameActor* pActor );
	// ��ȡ�������
	int getPlayerCount();
	// ��ȡ��������
	int getMonsterCount();
	// ���������ҵ�monster
	GameActor* getMonster( int nIndex );
	// actors����
	int getActorNums();
	// ���������ҵ�Actor
	GameActor* getActor( int nIndex );
	void updateMonsters( GameActor* pActor );

	//tolua_end
	// ��ȡNPC����
	int getNpcCount();
	
	void resetShowModelAndTitleOption();
	// �����Ƿ���ʾ���
	void changeShowOtherPlayerModel();
	bool canShowActorModelInScene( GameActor* pActor );

	// �����Ƿ���ʾ����/�ƺ�����
	void setShowItherPlayerTitle();
	// ����������ҵ�ͷ����Ϣ
	void updateAllTeamPlayerTitleInfo();
	void updateAllPlayerTitleInfo();
	void updateAllPlayerTitleColor();
	// �Ƿ���ʾ��������
	
	// ������Щ��ϢҪ��ʾ
	void setGameOpt( int nIndex, bool bShow );
	void setGameOptValue( int nIndex, char value );
	//tolua_begin
	// ��ȡ��Ӧ��ʾ��Ϣ�б�
	bool getGameOpt( int nIndex );
	GameSettingConfig&	getGameConfig()
	{
		return m_GameConfig;
	}

	// ������ȡ��һ��Actor
	GameActor *getNextActorById(int id);
	GameActor *getNextActor(GameActor *pre = NULL);
	void changeShowPlayerID();
	int getNextPlayerEnemy(int id = 0);
	int getServerId(const char* szName );

	const char* getClanFengHao( const tdr_ulonglong ClanWid );
	void setClanFengHao();

	//��ȡ�������ȼ��ƺ�
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
	std::map<int, PASSENGER_LIST>		m_waitRideList;	// �Ⱥ�����б�
	std::map<GameActor *, std::pair<float, bool>>		m_fadeActors;
	GameSettingConfig					m_GameConfig;
// 	std::vector<MONMACHINEINFO>			m_monsterMachineWaiters;

	struct HeapCmp
	{
		GameXYZ basePos;
		bool operator()(GameActor *_Left, GameActor *_Right) const;
		
	}									m_heapCmp;/// ��С�ѵıȽϷº���

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

	BaseWarMapOwnerInfo						m_BaseWarMapOwnerList[MAX_BASE_WAR_MAP_CLAN_INFO];		//�ݵ���Ϣ
	std::map<tdr_ulonglong, std::string>	m_ClanFengHao;		//������
	std::vector<tagSpartaLevelDef*>			m_SpartaLevelData;	//�������ȼ�����
};//tolua_export
