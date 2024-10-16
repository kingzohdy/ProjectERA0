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
	PRMOTION_DAILY_TASK,	//ÿ��Ŀ��
	PROMOTION_ALL,			//ȫ��
	PRMOTION_ACTIVE,		//����
	PRMOTION_CLAN,			//����
	PRMOTION_DAYACT,		//ÿ�ջ
	PRMOTION_BOSS,			//����boss
	PRMOTION_BUSY,			//�Һ�æ
};

enum
{
	PROMITION_CMP_NAME,		//����
	PROMITION_CMP_TIME,		//����ʱ��
	PROMITION_CMP_LEVEL,	//�ȼ�
	PROMITION_CMP_STAR,		//��������
	PROMITION_CMP_COUNT,	//��ɴ���
	PROMITION_CMP_TEAM,		//�Ƿ����
	PROMITION_CMP_STATE,	//״̬
};

enum
{
	PROMOTION_RUNING,	//���ڽ�����
	PROMOTION_PASSED,	//�Ѿ�����
	PROMOTION_COMING,	//���컹û��
	PROMOTION_NOTTODAY,	//���ڽ���
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
	PROMOTION_SINGLE = 0,	//����
	PROMOTION_TEAM,			//���
};

enum
{
	PROMOTION_NOT_NEED_AUTOTEAM = 0,		//����Ҫ��Ѱ�����������ʾ
	PROMOTION_SINGLE_LIMIT,					//�ǵ��˻������Ҫ���������������ʾ
	PROMOTION_NOT_START,					//��Ҫ��ʾ�����ܲμӣ��û���ڽ�����
	PROMOTION_LEVEL_LIMIT,					//��Ҫ��ʾ�����ܲμӣ��ȼ�������
	PROMOTION_HAVE_FINISH,					//��Ҫ��ʾ�����ܲμӣ��Ѿ����
	PROMOTION_OK,							//���Բμ�
	PROMOTION_NOT_TODAY,					// ���ڽ���
};

enum
{
	PROMOTION_REWARD_EXP = 1,	//��������
	PROMOTION_REWARD_MONEY,		//Ǯ
	PROMOTION_REWARD_EQUIP,		//װ��
	PROMOTION_REWARD_GEM,		//��ʯ
	PROMOTION_REWARD_PET,		//����
	PROMOTION_REWARD_RIDE,		//����
	PROMOTION_REWARD_STUFF,		//����
};

#define WORLD_CITY_ACTIVE_HELP_ID_TOLUA WORLD_CITY_ACTIVE_HELP_ID

//��Ƿ�ʼ����
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
	int					m_nFlag;						// 0����1�,2���� Bind Macrosgroup:ACTIVE_HELPER_TYPE
	int					m_nIconID;						//ͼ��ID
	char				m_szName[NAME_LEN];				//�����
	int					m_nType;						//�ճ�|�ܳ�
	int					m_nWeekFlag;					//һ���ڿ��ű��
	char				m_szStartDate[9];
	char				m_szEndDate[9];

	int					m_nTimeCount;					//ʱ�䷶Χ�ĸ���
	tagPromotionTime	m_Time[MAX_ACTIVE_DAY_TIME];

	int					m_nLevel;		//�ȼ�����
	int					m_nNeedTeam;	//�Ƿ���Ҫ���
	int					m_nAutoTeamID;	//�Զ���ӱ��
	int					m_nRelateId;	//�������񣬻��߸���ID
	int					m_nMainRewardType;
	int					m_nMainRewardLevel;
	char				m_szNpcName[MAX_MSG_DESC_LEN];
	char				m_szDesc[DESC_LEN];
	int					m_nReCommand;							//�Ƽ�1�Ƽ�0 ���Ƽ�	
	int					m_RewardItem[MAX_ACTIVE_AWARD_ITEM];	//������Ʒ
	int					m_nRewardItemCount;						//����������
	bool				m_bShowInAutoTeam;
	int					m_nTimeShowType;						/*  Ver.1169 ʱ����ʾ���� 0Ϊ��ʾ���е�ʱ�� 1Ϊ��ʾ�᳤���� */
	int					TransMapID;								/*  Ver.1653 ���͵�ͼID */
	tagResPos_ToLua		TransPos;								/*  Ver.1653 ���͵����� */
	int					m_nBusyID;								/*  Ver.1865 �Һ�æID */
	int					m_nHuoDongShow;							// ÿ�ջ
};

struct AutoTeamData
{
	int npcID;
	int mapID;
	int proType;	//0 ����|1 �
	int relateID;
	int nRow;
};

struct tagDailyGoalAward_ToLua
{
	int AwardID;                         	/*   �����׶�ID */
	int Gold;                            	/*   ��� */
	int BindGold;                        	/*   �󶨽�� */
	int ItemID[MAX_DAILY_GOAL_AWARD_ITEM]; 	/*   ��������ƷID */
	int ItemNum[MAX_DAILY_GOAL_AWARD_ITEM]; 	/*   ��������Ʒ���� */
	int Condi;                           	/*  Ver.1669 ����|λ���� */
};

#pragma pack( 1 )
/* ����boss����� */
struct tagWorldBossDef_ToLua
{
	int MonID;                           	/*   ����ID */
	char FreshType;                       	/*   ˢ������ */
	int AwardIcon[MAX_WORLD_BOSS_ICON];  	/*   ����ͼ�����8�� */
	int MapID;                           	/*   ˢ�µ�ͼ */
	int X;                               	/*   ˢ������ */
	int Y;                               	/*   ˢ������ */
	int LifeTime;                        	/*   ���ʱ��:�� */
	int PatrolID;                        	/*   Ѳ��ID */
	int DieFreshTime;                    	/*   �����������ٴ�ˢ��:�� */
	int DieFreshMonRate;                 	/*   ������ˢ�ָ���:����� */
	int DieFreshMonID;                   	/*   ������ˢ�Ǹ��� */
	int DieFreshMonTime;                 	/*   ����������ˢ�Ǹ��� */
	char DieFreshNum;                     	/*   ������ˢ�ִ���:ÿ�����x�β�������ˢ1�� */
	int Week;                            	/*   ���ڼ�,0��ʾ��ˢ, 1000001--��ʾ��1����ˢ */
	char FreshTime[9];                    	/*    hh:mm  */
	int TipTime;                         	/*   ˢ��ǰ��������ʾ */
	int TipChannel;                      	/*   ��ʾƵ�� */
	char TipMsg[128];                     	/*   ��ʾ���� */
	char FreshMsg[128];                   	/*   ˢ����ʾ */
};
#pragma pack()

//tolua_end

enum
{
	PROMITION_FLAG_QUEST = 0,	//����
	PROMITION_FLAG_PROMITION,	//�
	PROMITION_FLAG_CLAN,		//����
	PROMITION_FLAG_TASKGROUP, 	/* ������ */
};

enum
{
	PROMITION_TYPE_DAILY = 0,	//ÿ��
	PROMITION_TYPE_WEEKLY,	//ÿ��
};

class GamePromotion//tolua_export
	:public Ogre::Singleton<GamePromotion>
{//tolua_export
public:
	~GamePromotion( void );
	static GamePromotion* create( const char* szFileName );

	//tolua_begin
	
	int getAllNum();
	// ��ȡ�����
	int		getPromotionNum();
	// ��ȡ��������
	int		getClanProNum();
	// ����
	int		sortPromition( int nProType );

	int		getAllRow(int nIndex);

	int		getPromotionRow( int nIndex );

	int		getClanProRow( int nIndex );

	// ���û�������
	void	resetProData();

	bool	isInDatePeriod( int nIndex );
	PromotionStartInfo* getCurrPromitionStartInfo( int nIndex );

	tagPromotionDate*	getPromotionData( int nRow );
	
	//�ж�ָ�����ӷ�Χ��
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

	//�Ƿ���ս���
	bool	isBattleActive( int nIndex );

	//��ȡ���ʾ��
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

	void	setPinJiuBlurTime( unsigned int tickTime );  // ����ƴ����Ļģ��ʱ��
private:
	GamePromotion( void );
	// ��ȡ��ǰ��������׼ʱ��
	struct tm*		getServerTimeTm();
	// ���Ϣ�������� nType ������ nFlag 1���� -1 ����
	void		sortPromitionList( std::vector<int>& vectProData, int nType, int nFlag, int nProType);
	// �Ƚϳ�Ա�Ĵ�С
	int			cmpMember( const int src, const int des, const int nType,const int nProType );
	// �ȽϺ���
	int			comp_func(const void * v1, const void *v2);
	// ���õ���ʱ��
	void		setPromotionTime();
	// ���û����
	void		setPromotion();
	// ���ù�������
	void		setClanPro();

	void		sortDailyGoalList();

	bool		isFinishActive( int nRow );

private:
	std::vector<int>					m_vecAll;		//ȫ������
	std::vector<int>					m_vectPromotion;// ����� ����
	std::vector<int>					m_vectClanPro;	// ÿ���ȵ� ����
	std::map<int, tagPromotionDate>		m_mapPromotionTime;	// � ʱ���

	std::map<int,int>					m_pWorldmap;	//������ɴ���

	std::vector<int>					m_vecDailyTask;
	std::vector<int>					m_vecNeedShowDailyTask;

	CSACTIONDAILYGOALINFO				m_DailyInfo;

	std::map<int,CSACTIONDYNACTIVEINFO> m_mapDynActiveInfo;
	std::map<int,bool>					m_mapActivityGuid; //������еĸ��id����Ӧ��ʼ���
	std::vector<tagWorldBossDef*>		m_vecWorldBossData;
	unsigned int						m_nPinJiuBlurStartTime;   // ƴ��--��Ļģ��ʱ��
};//tolua_export
