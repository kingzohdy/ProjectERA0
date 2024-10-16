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
	CLIENT_TASK_TYPE_NEW = 0, 		/* �������� */
	CLIENT_TASK_TYPE_MAIN, 			/* �������� */
	CLIENT_TASK_TYPE_BRANCH, 		/* ֧������ */
	CLIENT_TASK_TYPE_BUSSINESS, 	/* �������� */
	CLIENT_TASK_TYPE_ACT, 			/* ����� */
	CLIENT_TASK_TYPE_REP, 			/* �ظ����� */
	CLIENT_TASK_TYPE_GOLD, 			/* �ƽ����� */
	CLIENT_TASK_TYPE_PWORLD, 		/* �������� */
	CLIENT_TASK_TYPE_DISCOVER, 		/* ̽������ */
	CLIENT_TASK_TYPE_SINGLE_CONVOY, /* ���˻������� */
	CLIENT_TASK_TYPE_TEAM_CONVOY, 	/* ���黤������ */
	CLIENT_TASK_TYPE_CLAN,          /* �������� */
	CLIENT_TASK_TYPE_TRUST,			/* ί������ */
	CLIENT_TASK_TYPE_CHG_CAREER,	/* תְ���� */
	CLIENT_TASK_TYPE_RIDE,			/* �������� */
	CLIENT_TASK_TYPE_FAIRY,			/* �������� */
	CLIENT_TASK_TYPE_LIFE,			/* �������� */
	CLIENT_TASK_TYPE_CHG_RECYCLE,	/* �������� */
	CLIENT_TASK_TYPE_DAILY_GOAL,	/* ����Ŀ������ */
	CLIENT_TASK_TYPE_SJ_MON, 	/* �ͽ������ */
	CLIENT_TASK_TYPE_SJ_ITEM, 	/* �ͽ��ռ����� */
	CLIENT_TASK_TYPE_SJ_BOSS, 	/* �ͽ��ɱ�� */
	CLIENT_TASK_TYPE_DART_CAR, 	/* �����ڳ����� */
	CLIENT_TASK_TYPE_DESIGNATION,		/* �ƺ����� */
	CLIENT_TASK_TYPE_RONGLU, 	/* ��¯���� */
	CLIENT_TASK_TYPE_XIEZHU,	/* Э������ */		
	CLIENT_TASK_TYPE_BATTLE,	/* ս������ */
	CLIENT_TASK_TYPE_PAOSHANG,	/* �������� */

	CLIENT_TASK_TYPE_END,	
};

enum  
{
	CLIENT_TASK_DIFFICULT_NORM		= 0, 	/* �� */
	CLIENT_TASK_DIFFICULT_NOEASY	= 1, 	/* ���� */
	CLIENT_TASK_DIFFICULT_HARD		= 2, 	/* ���� */
};

enum
{
	MAP_TYPE_COPY		= 1,	// ����
	MAP_TYPE_OUTSIDE	= 2,	// Ұ��
	MAP_TYPE_ALL		= 3,	// ȫ��
};

enum
{
	CLINT_CAN_NOT_ACCEPT_TASK	= 0,
	CLINT_CAN_ACCEPT_TASK		= 1,
	CLINT_CAN_NOT_ACCEPT_TASK_BY_LITTLE_LV	= 2,
};

struct tagTaskMoney_toLua
{
	unsigned int Type;                            	/*   �������� */
	unsigned int Val;                             	/*   ���� */
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
	unsigned int ID;                              	/*   �ͽ���ID */
	tagSJTaskMoney_tolua RefreshTaskMoney;                	/*   �ͽ�ˢ���������� */
	tagSJTaskMoney_tolua RefreshStarMoney;                	/*   �ͽ�ˢ���Ǽ����� */
	tagSJTaskStar_tolua StarRate[MAX_SJ_TASK_STAR];      	/*   �Ǽ���Ϣ */
	int MaxCostNum;                      	/*  Ver.1971  */
};
struct tagDiscoverInfo 
{
	bool	m_bFiniTask; 
	int		m_nInAreaTime;
};

struct tagTimeAccumQuestInfo
{
	int		m_nMaxTime;		// ����Ϊ��λ
	int		m_nTaskID;
	int		m_nTimeMode;
	int		m_nPassTime;	// ����Ϊ��λ
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
	// ��ȡ����
	void getQuest();
	// ��������
	void acceptQuest( unsigned int npcId, int questId );
	// ��ɫ��ȡ����
	void awardQuest( unsigned int npcId, int questId, int selectId = 0 );
	// ��ɫ��������
	void dropQuest( int questId );
	// ɱ�ֻ����ռ���ɺ��������
	void finishQuest( int questId );
	//������������
	void finishActiveQuest( int questId );
	// ��������
	void shareQuest( int questId );
	// �ܾ�����
	void refuseQuest( int questId );
	void AutoQuest();
	//tolua_end

	// s->c
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

public:
	//tolua_begin
	// ��ȡ��������
	const char* getQuestName( int questId );
	// ��ȡ���߽�������
	int getAwardType( int questId, int nRecycleID = 0, int nMaxIdx = 1 );
	// ��ȡ�ɽ��������ڳ���
	const char* getQusetScene( int questId );
	// ��ȡ��������
	int getQuestType( int questId );
	//��ȡ�ѽ����ͽ��������
	int getSJTaskNum();
	// ��ȡ�����Ѷ�
	int getQuestDifficult( int questId );
	// ��ȡ�����Ǽ�
	int getQuestStar( int questId );
	// ��ȡ������͵ȼ�Ҫ��
	int getQuestLevelMin( int questId );
	// ��������
	const char* getQuestDes( int questId );
	// ����Ի�
	const char* getQuestDialog( int questId );
	const char* getQuestFiniDialog( int questId );
	// ��ȡ��ǰ����Ļ���
	int getTaskRecycleIdx( int questId, int nRecycleID );
	int getTaskRecycleIdx( int nRecycleID );
	// �Ƿ���ʾ�����������
	bool isShowRecycleDesc( int questId, int nRecycleID );
	// ��ȡ�������������
	const std::string getRecycleDes( int questId, int nRecycleID );
	//��ȡ�����ͼid
	unsigned int getQuestMapID( int questId );

	// ��ȡ����һ�������ɵĴ���
	int getQuestMaxCanAcceptNumOneDay( int questId );
	// ��ȡ������ɴ���
	int getQusetHaveFinishNumOneDay( int questId );
	// ��ȡ����������ɴ���
	int getReCycleQusetOneDayNum( int nCycleId );
	// ��ȡ��������ɴ���
	int getGroupQusetOneDayNum( int nGroupId );
	// ���������ɴ���
	void addQusetNumOneDay( int questId );
	// ��ȡ������ÿ����ɴ���
	int getGroupQuestWeekNum( int nGroupId );
	// ���������ÿ����ɴ���
	void AddGroupQuestWeekNum( int questId );
	// �Ƿ��ǻ�������
	bool isRecycleTask( int questId );
	bool isRecycleTask( const tagTaskDef& taskDef );

	// �����������ʱ����»�������ǰ�Ļ���
	void updateRecycleIdxByDrop( const tagTaskDef& taskDef, int nRecycleIdx, int nRecycleID );
	// �����������ʱ����»�������һ���Ѿ���������
	void updateRecycleOneDayInfoByDrop( const tagTaskDef& taskDef, int nRecycleID );

	// ���ύ�����ʱ����»�������ǰ�Ļ���
	void updateRecycleIdxByCommit( const tagTaskDef& taskDef, int nRecycleIdx, int nRecycleID );
	// ���ύ�����ʱ����»�������һ���Ѿ���������
	void updateRecycleOneDayInfoByCommit( const tagTaskDef& taskDef, int nRecycleIdx, int nRecycleID );
	
	// ��ȡ�������ʱ��
	int getQusetTime( int questId ); 
	// 00:00�����õ�����ί����ɴ���
	void resetQusetNumOneDay(); 
	// �����Ƿ�ﵽ��ɴ�������
	bool isQuestHaveFinishOneDayMaxNum( int questId );

	// ������֯ί����������
	void reSetTrust( int nMapId = 0 );
	// ����ί����������
	void setTrustArea( int nArea );
	// ѡ�����е�ί����������
	void clearTrustArea( );
	// ����ί�������������� 1Ϊ���� 2 ΪҰ��
	void setTrustAreaType( int nType );
	// �Ƿ���ί����������
	bool IsTrustArea( int nArea );
	// �Ƿ���ί��������������
	bool IsTrustAreaType( int nType );
	// ��ȡָ����ͼ��ί����������
	int	getMapTustNum( int nMapId );
	// �Ƿ���ί������
	bool IsTrustQuest( int questId );

	// ��ȡί����������
	int getTrustPageInfo( int nPage );
	// ��ȡί������ȼ�
	int getQuestLvLimit( int questId );
	// ��ȡί��������������Ʒ
	int getQuestNeedItemNum( int questId );
	// ��ȡί������������Ʒ
	int getQuestNeedItem( int questId, int index );
	// ��ȡί������������Ʒ��������
	int getQuestNeedItemCount( int questId, int index );
	// ��ȡί����������
	int getNumTrust();
	// ѡ�����е�ί������
	void selectAllTrustTask( bool bSelect );
	// �Ƿ����е�ί�����񶼱�ѡ��
	bool isAllTrustSelect();

	// ��ȡ��������Ʒ������
    int getQuestItemNum( int questId, int nRecycleID, bool bFinalAward = false );
	// ��ȡ��������Ʒ
    int getQuestItem( int questId, int nRecycleID, int index, bool bFinalAward = false );
    // ��ȡ������������Ʒ����
    int getQuestItemCount( int questId, int nRecycleID, int index, bool bFinalAward = false );
    
    // ��ȡ����������ܴ���
    int getTaskCycleNum( int questId, int nRecycleID );

	// ��ȡ��������Ʒ����Ҫְҵ
	int getQuestItemCareer( int questId, int nRecycleID, int index );
	// ����ְҵ�Ƿ������������Ʒ����
	bool isQuestItemCareer(int questId, int nRecycleID, int index, int race);
	
	
	// ��ȡ����������ҵĹ�����˹��׶�
	int getQuestClanContribute( int questId, int nRecycleID, bool bFinalAward = false );
	// ��ȡ���ᰲ���Ƚ���
	int getQuestClanAnDing( int questId, bool bFinalAward = false );
	// ��ȡ������Դ����
	int getQuestClanResourceId(int questId, unsigned int index);
	int getQuestClanResourceNum(int questId, unsigned int index);
	// ��õ�ǰ�����ٽ�ȡ����������
	int getRecycleLeastTaskNum( int nRecycleID );
	// �������ĵ�ǰ����
	int getQuestRecycleIndex( int questId, int nRecycleID );
	const CYCAWDRATE* getRecycleAwardInfo( int questId, int nRecycleID );
	// ��ȡĳ���Ľ���
	int getRecycleTaskAwardRate( int questId, int nRecycleID );
	// ��ȡ���Ὠ��Ƚ���
	int getQuestClanBuildingValue( int questId, int nRecycleID, bool bFinalAward = false );
	// ��ȡ�����ʽ���
	int getQuestClanMoney( int questId, int nRecycleID, bool bFinalAward = false );
	// ��ȡ����Ƽ��㽱��
	int getQuestClanTechPoint( int questId, bool bFinalAward = false );
	// �Ƿ��Ƕ�̬����
	bool isDynamicAwardMoneyAndExp( int questId, int nRecycleID );
	// ��ȡ��������Ǯ
	tagTaskMoney_toLua* getQuestMoney( int questId, int nRecycleID, bool bFinalAward = false );
	// ��ȡ����������
	unsigned int getQuestExp( int questId, int nRecycleID, bool bFinalAward = false );
	// ��ȡ�������ƺ�
    const char* getAwardDesignation( int questId, int nRecycleID, bool bFinalAward = false );
    // ��ȡ������������
    const char* getAwardRespactName( int questId, int nRecycleID, bool bFinalAward = false );
    // ��ȡ����������ֵ
    unsigned int getAwardRespactValue( int questId, int nRecycleID, bool bFinalAward = false );
	// ����������ؼ�������
	int getQuestFixedAttr( int questId, int nRecycleID, int index );
	// �����������װ���󶨼�������
	int getQuestBindingAndSlot( int questId, int nRecycleID, int index );
	// ��ý����������
	const char* getAwardSecondarySkillName(int questId);
	int getAwardSecondarySkillVal( int questId );
	void PrintQuestState( int nQuestID, int nQuestState, const char* pszSrcServeInfo );
	// ��ȡ�ѽ�ȡ��������
	int getNumQusetRun();
	
	// ��ȡ�ѽ�����
	int getQuestRunId( int index );
	// �������Ƿ��Ѿ���ȡ
	bool isQuestRun( int id );
	// ɾ���ۻ���ʱ����
	void delTimeAccumQuest( int questId );
	// ɾ���ѽ�ȡ����
	void delQuestRun( int questId );
	// ��ȡ�ѽ�ȡ������������ID
	int getQuestRunRecycleID( int questId );
	// ��ȡ����������
	int getQuestRunState( int questId );
	// ��ȡ�ѽ�����Ҫ������
	const char* getQuestRunReqDesc( int questId );
	// ��ȡ������ɺ�׷������
	const char* getQuestFiniTraceDes( int questId );
	// ��ȡ������ɺ�׷������λ��
	std::string getQuestFiniTraceDespos( int questId );
	//��ȡ�ͽ�δ������Ŀ������
	const char* getSJQuestTraceNoFiniTip( int questId );
	// ��ȡ����δ�������׷����ʾ
	// (ע�⣺�������ѯ����о������Ҳ���ø�������Ϣ)
	const char* getQuestNoFiniTraceDes( int questId );
	// ��ȡ����δ�������׷����ʾ
	// (ע�⣺�������ѯ����о������Ҳ���ø�������Ϣ)
	std::string getQuestNoFiniTraceDespos( int questId );
	// ���������ͨ�ý���
	int getQuestMaxGenericProgress( int questId);
	// �������ǰͨ�ý���
	int getQuestGenericProgress( int questId);
	
	// ˢ�¿ɽ�����
	void updateCanAcceptQuest();
	// ��ȡ�ɽ���������
	int getNumCanAcceptQuest();
	// ��ȡ�ɽ���������
	int getCanAcceptQuestId( int index );
	// ��ȡ�ɽ�����Ҫ������
	const char* getCanAcceptQuestReqDesc( int questId );
	// ��ȡ�ɽ�����Ҫ������λ��
	std::string getCanAcceptQuestReqDescpos( int questId );
	// ��ȡ����������Ϣ
	const char* getQuestReqDesc( int questId );

	// ��ȡ����׷������
	int getNumQuestTrace();
	// ��ȡ����׷��
	int getQuestTrace( int index );
	// ��ȡ����׷��Tips��ʾ
	const char* getQuestRunTip( int questId );
	// ��ȡ����׷�ٶ��������
	std::string getQuestRunTippos( int questId );
	// �������׷��
	void addQuestTrace( int questId );
	// ɾ������׷��
	void delQuestTrace( int questId );
	// ��������Ƿ�׷��
	bool isQuestTrace( int questId  );
	
	// TODO: REFA �����Ƿ���Խ�ȡ 0 ���ɽ� 1 �ɽ� 2 ��һ�ȼ��ɽ�
	int canAcceptQuest( int nQuestId );
	// ���������Ƿ�ɽ�ȡ
	bool canAcceptRecycleQuest( int nPayerLv, int nRecycleId, int npcId );
	// TODO: REFA ����ɼ����Ƿ���Բɼ�
	bool canCollectQuest( int nMonResId );
	// �����Ƿ��ǹ�������
	bool isShareQuest( int questId );
	
	// ��ʾ������ʾ
	void showKillMonsterInfo( const CSTASKRUNONE& newOneTaskData, const CSTASKRUNONE& oldOneTaskData );
	// ��ʾ��Ʒ��ʾ
	void showCollectItemInfo( bool bShow = true );
	
	// ��ȡ����ĳ�����
	bool getMissionLink( int nQuestId, int nRecycleID ); 
	
	// ����Ƿ�����Ҫ�ۻ���ʱ������
	bool isTimeAccumQuest( int nTaskID );
	// �����Ƿ���ʱ����
	bool isQuestTimeLimit( int nQuestId ); 

	// ��ȡ�����ʣ��ʱ��
	unsigned int getTaskEndTime( int nQuestId ); 
	
	// �Ƿ��Ѿ���ȡ������
	bool isHaveAcceptQuest( int questId );
	// �Ƿ�������ɹ�����
	bool isQuestHaveFiniEver( int questId );
	// �����Ƿ��Ѿ�����
	bool isQuestFini( int questId );
	
	// 
	// ���������Ƿ�ȫ��׷��
	void setQuestTraceAll( bool bTraceAll );
	
	// �����Ƿ�ȫ��׷��
	bool isQuestTraceAll(){ return m_bTraceAll; }
	
	// ���������������
	const char* getQuestTypeName( int nType );

	// ��ȡ������������Ҫ��ѹ��
	tagTaskMoney_toLua* getConvoyQuestMoney( int questId );

	// ��ȡ����һ����ɵĴ���
	const int getQuestOneDayFiniNum( int questId, int nRecycleID );
	
	// �жϸ������Ƿ��ǲ����ƴ�����
	bool isQuestLimitNumOneDay( int questId, int nRecycleID );
	// �������һ����ɵ����ƴ���
	const int getQuestOneDayLimitNum( int questId, int nRecycleID );
	
	// ����׷����Ϣ��������
	void saveTaskTraceOpt();

	// ��������б�
	void clearQuest();

    // ���µ�ǰ��ͼNPCͷ�������־
	void            updateNpcQuestTitle( int nMapID, int nRealMapID );
    void            updateNpcQuestTitle();
	
	// ��ȡ��Ҫ�ۻ���ʱ������
	tagTimeAccumQuestInfo*	getTimeAccumQuest( int questId );
	tagOneTaskInfo&			getChgTaskInfo();
	const tagTaskDef*		getTaskDef( int questId );
	bool			IsNeedCollectItem( int questId ,int index );
	bool			IsNeedMonster(int questId);
	//�����������Ҫ�Ĺ���ID
	int				getQuestMonsterId(int questId);
	//�����������Ҫ�ĵ���ID
	int				getQuestItemId(int questId);
	//������������Ǵ���OBJ
	int				getQuestSpecialFiniObjVal(int questId,int index);
	//������������Ǵ���OBJConditionVal
	int				getQuestSpecialFiniConditionVal(int questId,int index);
	int				getFirstCanAcceptRecycleTaskID( int questId );

	//ʹ���ͽ�������Ʒ
	void UseBountyHunterItem( int itemID );
	//ˢ���ͽ�����
	void RefurbishBountyHunterTask( int sjID );
	//ˢ���ͽ������Ǽ�
	void RefurbishBountyHunterStar( int sjID, int nflag = 0 );
	//�����ͽ�����
	void AcceptBountyHunterTask( int itemID, int SJID, int TaskID );
	//��ȡ�ͽ������ඨ��
	const tagSJTaskDef_tolua* getSJTaskDef( int SJID );
	//�ж��Ƿ��������
	bool isQuestMust( int questId );
	// ͨ������ID��ȡ�ѽ�ȡ����
	CSTASKRUNONE*	getQuestRunByRecycle( int recycleId );
	// ��ȡ�ɽӽ�ȡ���� 
	bool			getQuestCanAccept( int questId );
	//���npc����id
	int				GetNpcQuestID( int npcID );
	bool checkQuestMonsterKilled( int questId, int index );
	// ʹ����Ʒ�򿪻����������
	void RequestRecycleTaskByItem( int nListType, int nGridIdx );
	// ������Ʒ����
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
    // ���������Ұ
    virtual void    npcEnterView( int id );
	// ����ָ�������Զ��������
	void			updateAutoFiniQuest( CSTASKRUNONE& questRun, const tagTaskDef& taskDef );
	// ��ȡ�ѽ�ȡ����
	CSTASKRUNONE*	getQuestRun( int questId );
	
	bool			canAddFiniQuest( int questId );
	// ���һ�����������
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
	// TODO: REFA ��ʾͷ��������ʾ
	void			showQuestTitle( int npcId );
	// �Ƿ���ָ������
	bool			isInArea( tagDiscoverInfo& discoverInfo, const tagTaskDef& taskDef, int nTaskId, int nTaskState );
	// ����Ƿ������ۻ���ʱ������
	bool			isSatisfyTimeAccumQuest( CSTASKRUNONE& runQuest, const tagTaskDef& taskDef );
	
	// ������Ҫ�ۻ���ʱ������
	void			updateTimeAccumQuest( CSTASKRUNONE& runQuest, const tagTaskDef& taskDef );
	// ���µ���ʱ����
	void			updateTimeCountDownQuest( CSTASKRUNONE& runQuest );
	// ����Ƿ�����Ҫ�ռ���Ʒ����
	bool			isSatisfyItemOrMonsterQuest( CSTASKRUNONE& runQuest, const tagTaskDef& taskDef );
	// ������Ҫ���߻��ߴ�ֵ�����
	void			updateItemOrMonsterQuest( CSTASKRUNONE& runQuest, const tagTaskDef& taskDef, bool bShowMsg );
	// ��½ʱ��������״̬
    void			updateAllTaskState( bool bShowMsg );
    // ���������ȡ�ȼ�����
    bool            isLimitByLv( const tagTaskRecycleDef& taskRecycleDef, int nPlayerLv );
    // ��������������
    bool            isLimitByGroup( const tagTaskRecycleDef& taskRecycleDef );
    // ����������������
    bool            isLimitByMutex( const tagTaskRecycleDef& taskRecycleDef );
    // ��������ÿ����ɴ���������
    bool            isLimitByOneDay( const tagTaskRecycleDef& taskRecycleDef );
    // ��������ǰ���л�������
	bool            isLimitByRecycleIdx( int nRecycleId, int nNpcId );
	// ���������Ƿ��ܹ�������
	bool			isLimitByClan( const tagTaskRecycleDef& taskRecycleDef );
	// ���������Ƿ���ǰ����������
	bool			isLimitByPreTaskID( const tagTaskRecycleDef& taskRecycleDef );
	// �Ƿ��ǹ̶�˳���ȡ�Ļ�������
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
	std::vector<CSTASKRUNONE>			m_QuestRuns;			// �Ѿ���ȡ������
	std::vector<int>					m_CanAcceptQuest;		// �ɽ�ȡ�������б�		
	std::vector<int>					m_QuestTrace;			// ����׷���б�
	std::vector<int>					m_FiniQuest;			// ����������б�
	std::vector<tagTaskDef*>			m_TaskDef;				// ���������б�
	std::map<int,CSTASKRECYCLEINFO>		m_TaskRecycleInfo;		// ���������б�
	std::multimap<int, int>				m_TrustMap;				// �ɽ�ȡί���б�	
	std::map<int, TASKONEDAYINFO>		m_TrustOneDayInfo;		// һ�����ɴ���
	std::map<int, TASKCYCLEONEDAYINFO>	m_RecycleOneDayInfo;	// һ�����ɵĻ������
	std::map<int, GROUPONEDAYINFO>		m_GroupOneDayInfo;		// ������ÿ����ɴ���
	std::map<int, TASKGROUPWEEKINFO>	m_GroupWeekInfo;		// ������ÿ����ɴ���
	std::map<int, int>					m_PreTime;				// ��ʱ����Ŀ�ʼʱ��
	std::map<int, int>					m_QuestRecycleIdx;		// ��������ǰ���еĻ�
	std::map<int, tagDiscoverInfo>		m_mapDiscoverInfo;		// ̽������Ŀ�ʼʱ��
	std::vector<tagTimeAccumQuestInfo>	m_timeAccumQuestRuns;	// �Ѿ���ȡ����Ҫ�����ۼ�ʱ�������
	std::vector<int>					m_FinishedDynamicGetTasks;
	int									m_nTrustArea;
	bool								m_bSelectAll;
	bool								m_bTraceAll;			// �����Ƿ�ȫ��׷��

	std::map<int, int>					m_PreItemCount;
	unsigned long						m_ulTraceData;

	tagOneTaskInfo						m_chgTaskInfo;
};//tolua_export
