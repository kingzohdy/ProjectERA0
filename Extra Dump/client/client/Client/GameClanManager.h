#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameDefineTolua.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "GameClanManager_City.h"
#include "OgreEntity.h"
#include "GameMaxMap.h"
#include "ClientInterface.h"
#include <map>

//tolua_begin

#define MAX_CLAN_RESOURCE_TYPE MAX_CLAN_RESOURCE
#define CLAN_OPEN_GROUP_ITEM_ID_TOLUA CLAN_OPEN_GROUP_ITEM_ID

// ��Ա��������
enum
{
	QUICKSORT_BY_POSITION	= 1,	
	QUICKSORT_BY_LEVEL		= 2,
	QUICKSORT_BY_CAREER		= 4,
	QUICKSORT_BY_ONLINE		= 8,
	QUICKSORT_BY_CONTRIBUTE = 16,
	QUICKSORT_BY_NAME		= 32,
	QUICKSORT_BY_NICKNAME	= 64,
	QUICKSORT_BY_GROUP		= 128,
};

enum
{
	OTHER_CLAN_SORT_ID		= 1,	//����ID��ѯ
	OTHER_CLAN_SORT_NAME	= 2,	//��������ѯ
	OTHER_CLAN_SORT_NUM		= 3,	//����������ѯ
	OTHER_CLAN_SORT_LEVEL	= 4,	//����ȼ���ѯ
	OTHER_CLAN_SORT_COLOR	= 5,	//������ɫ
};

enum
{
	TIME_SORT	= 1,//����ʱ��
	MONEY_SORT	= 2,//������Ŀ
	CLAN_SORT	= 3,//�����ʽ�
	CONTRI_SORT	= 4,//��ù���
};

#define MAX_CAREER_NUM	4	//ְҵ����

#define CLAN_WAR_COST_TOLUA CLAN_WAR_COST
#define MIN_BUUL_CREATE_CLAN TMPCLAN_CREATE_MIN_MONEY

#pragma pack(1)
struct tagClanCityProf_ToLua
{
	unsigned char				Level;                           	/*   ���еȼ� > 0 ˵�������ѱ����� */
	tagClanCityBuilding_ToLua	Buildings[MAX_CLAN_BUILDING]; 	/*   ���Ὠ�� */
};

// ��Ա��Ϣ
struct tagMemberInfo
{
	tagLong_ToLua	m_RoleLongId;
	char			m_szRoleName[ROLE_NAME_LEN_SAVE];   
};

struct tagClanMem_ToLua
{
	tagMemberInfo	RoleMember;                     
	char			NickName[NAME_LEN];              	/*   �ǳ� */
	unsigned char	Position;                        	/*   ����ְλ Bind Macrosgroup:CLAN_POSITION,*/
	unsigned char	Level;                           	/*   �ȼ� */
	unsigned char	Career;                          	/*   ְҵ */
	unsigned char	Online;                          	/*   �Ƿ����� */
	unsigned int	Contribute;                      	/*   ����� */
	int				LastOnline;                      	/*   ��������ʱ�� */
	int				LastLogin;                       	/*  Ver.124 ���ĵ�½ʱ�� */
	int				TotalOnline;                     	/*  Ver.242 �ܼ�����ʱ�� */
	char			Title[CLAN_MEM_TITLE_LEN];       	/*  Ver.497 ��� */
	int				JoinTime;                        	/*  Ver.1048 ���ʱ�� */
	unsigned int	TotalContribute;                 	/*  Ver.1349 ���׶����� */
	int DonateNum;                       	/*  Ver.1567 �������� */
	int DonateClanMoney;                 	/*  Ver.1567 ���׹����ʽ����� */
	int DonateContri;                    	/*  Ver.1567 ���׻�ù��׶����� */
	int DonateLastTime;                  	/*  Ver.1567 ������ʱ�� */
    int GroupID;                         	/*  Ver.1957 ��������ս�� */
};

// ������Ϣ����

struct tagClanInfo_ToLua
{
	tagLong_ToLua		m_clanLongId;				
	unsigned int		m_uClanId;
	int					m_nClanLevel;						// ����ȼ�
	int					m_nClanMemNum;						// ��Ա��
	char				m_szClanName[NAME_LEN];				// ��������
	char				m_szBulletinText[SHORT_DESC_LEN];   // ������Ϣ
	unsigned int		m_nMoney;							// �����ʽ�
	unsigned int		m_nBuildingValue;                   // ���Ὠ���	������
	unsigned int		m_nActiveValue;                     // ���ᷱ�ٶ�	���ٶ�
	unsigned int		m_nSkillPoint;                      // ���Ἴ�ܵ�
	char				m_szCaptionName[NAME_LEN];			// ����᳤��
	int					m_BulletinTextLastEditTime;			//���һ���޸Ĺ����ʱ��
	int					m_nTime;							// �жԹ�����
	int					m_Flag;								//������CLAN_FLAG,�Ƿ�ɴ�̽�ȱ��
};

// ��������Ϣ
struct tagClanViewer
{
	tagClanInfo_ToLua	m_clanInfo;
	tagMemberInfo		m_PresidentInfo;			// �᳤��Ϣ		
	tagClanMem_ToLua	m_Members[CLAN_MAX_MEM];	// �����Ա������Ϣ
};

// ����������Ϣ
struct tagClanBrief_ToLua
{
	tagMemberInfo Leader;                         
	int CreateTime;                      	/*   ���ᴴ��ʱ��(UTC) */
	unsigned short AssistNum;                       	/*   ���᳤���� */
	tagMemberInfo Assists[CLAN_MAX_ASSIST];        	/*   ���᳤������ID */
	char XuanGao[SHORT_DESC_LEN];         	/*   ����:��������Ա�ɼ� */
};

struct tagOtherClanInfo
{
	tagClanInfo_ToLua			m_ClanInfo;
	tagClanBrief_ToLua			m_ClanBrief;
	tagClanCityProf_ToLua		m_cityInfo;
};

// �����ѯҳ
struct tagOnePageClan
{
	CSCLANVIEWPAGECOOKIE		m_cookie;			// ��ǰҳcook
	std::list<tagOtherClanInfo>	m_clanInfoList;		// ��ǰҳ�����б�
};

struct tagClanApply_ToLua
{
	tagMemberInfo RoleMember;                     
	unsigned char Level;                          
	unsigned char Career;                        
	unsigned short Res3;                           
	int ApplyTime;                      
};

// �����б�
struct tagApplyJoinList
{
	int  m_nRoleNum;                              
	tagClanApply_ToLua m_ApplyRoles[CLAN_MAX_APPLY];
};

// ���빫�����Ϣ
struct tagApplyClanInfo
{       
	tagClanInfo_ToLua	m_ClanInfo;			// ������Ϣ	
	int					m_nApplyTime;		// ����ʱ��
};

struct tagClanDiplomatism
{
	int					m_nFriendNum;			// ͬ�˹�����
	int					m_nEnemyNum;			// �жԹ�����
	int					m_nApplyFriendNum;		// ����ͬ�˹�����
	int					m_nApplyEnemyNum;		// ����жԹ�����
	tagClanInfo_ToLua	m_Friends[CLAN_MAX_FRIEND];       
	tagClanInfo_ToLua	m_Enemys[CLAN_MAX_ENEMY];       
	tagApplyClanInfo	m_ApplyFriends[CLAN_MAX_APPLY_FRIEND];
	tagApplyClanInfo	m_ApplyEnemys[CLAN_MAX_APPLY_ENEMY];
};

struct tagClanWorkingCost_ToLua
{
	unsigned int IntervalTime;                    	/*   ���ļ��ʱ�䣨�룩 */
	unsigned int Memory;                          	/*   �����ʽ� */
	unsigned int BuildingValue;                   	/*   ����� */
	unsigned int ActiveValue;                     	/*   ��Ծ�� */
};

struct tagResourceLimit_ToLua
{
	int ResourceID;                      	/*   ��ԴID */
	int Limit;                           	/*   ��Դ���� */
};

struct tagClanLevelDef_ToLua
{
	unsigned int Level;                           	/*   ����ȼ� */
	int MaxMemberNumLimit;               	/*   ��ǰ�ȼ�������Ա����ʾ */
	int MaxResourceGrid;                 	/*   ������Դ�ֿ������ø����� */
	int MaxMachineGrid;                  	/*   �����е�ֿ������ø����� */
	int LvLimited[MAX_CLAN_CONSTRUCTION]; 	/*   �Խ����ͳ��еĵȼ����� */
	tagClanWorkingCost_ToLua WorkingCost;                     	/*   ��������� */
	tagClanUpgradeRequired_ToLua Required;                        	/*   �������� */
	int RequiredCityLv;                  	/*  Ver.534 ��Ҫ�Ĺ��������С�ȼ� */
	char EffectString[RES_NAME_LEN];      	/*  Ver.601 Ч���ַ��� */
	int EnemyCDTime;                     	/*  Ver.750 ��սCDʱ�䵥λ�� */
	tagResourceLimit_ToLua ResourceLimit[MAX_CLAN_RESOURCE]; 	/*  Ver.1000 ������Դ���� */
	int EveryDayPworldClanMoney;         	/*  Ver.1083 ���˻��ֽ��������ʽ��׼ֵ */
	int EveryDayPworldContri;            	/*  Ver.1083 ���˻��ֽ������׶Ȼ�׼ֵ */
	int MaxEveryDayPworldVal;            	/*  Ver.1083 ÿ�ջ��ֻ��������������������� */
	int ClanValContri;                   	/*  Ver.1083 ���ִﵽ���޽����Ĺ��׶� */
	int PresentClanMoney;                	/*  Ver.1562 ���͵Ĺ����ʽ� */
	int CostClanMoney;                   	/*  Ver.1562 ����|����дWorkingCost_Money�ֶ� */
	int MinClanMoney;                    	/*  Ver.1562 ��С�Ĺ����ʽ���(С�ڴ�ֵ�򽵼�) */
	int LevelDownTime;                   	/*  Ver.1564 ���ή������ʱ��(��) */
	char OccuNum;                         	/*  Ver.2252 ��ռ��ݵ��� */
	char FreeFence;                       	/*  Ver.2289 �����λ */
	char MoneyFence;                      	/*  Ver.2289 �շ���λ */
};

struct tagClanMachineProp_ToLua
{
	int			MaxHp;
	int			Amm;
	int			Fuel;
	int			Def;
	int			Atk;
	int			MoveSpd;
	int			CollectSpd;
	int			CollectCap;
};
struct tagClanViewGetRes_ToLua
{
	int PageCount;                       	/*   һ���ж���ҳ*/
	int PageCurr;                        	/*   ��ǰ�ڼ�ҳ*/
	int InfoNum;                        
	tagGetResInfoOne_ToLua GetResInfos[CLAN_VIEW_GETRES_NUM];
}; 
struct tagClanPetOp_ToLua
{
	char RoleName[ROLE_NAME_LEN];        
	char OpType;                          	/*   1-ιʳ 2-��ˣ 3-͵ */
	int Time;                           
	int Val;                            
};
struct tagClanPetTou_ToLua
{
	tagLong_ToLua RoleID;                         
};
struct tagClanPetOne_ToLua
{
	char Idx;                            
	int PetID;                          
	char RoleName[ROLE_NAME_LEN];        
	int JiYangStart;                    
	int JiYangEnd;                      
	int WorkEnd;                        
	int WorkFlag;                        	/*   1-δ��ȡ */
	short HappyVal;                       
	short WenBao;                         
	char Imp;                            
	int ImpExp;                         
	int OpNum;                          
	tagClanPetOp_ToLua OpInfo[CLAN_PET_MAX_OP];        
	int TouNum;                         
	tagClanPetTou_ToLua Tou[CLAN_PET_MAX_TOU];          
	int UseExp;                         
	int UseTime;                        
	unsigned int JiyangFlag;               
	tagLong_ToLua PetWid;                         
	tagLong_ToLua RoleID; 
};

#pragma pack()

// static const char *aszClanPosName[] = {
// 	"�ǻ�Ա",
// 	"ƽ��",
// 	"�Ƽ���Ա",
// 	"�ɼ���Ա",
// 	"������Ա",
// 	"�����Ա",
// 	"������Ա",
// 	"���᳤",
// 	"�᳤",
// };

#define MAXCLANLISTROW 15		//�����ѯһҳ������
#define MAX_CLAN_TECH_CATEGORY_TOLUA MAX_CLAN_TECH_CATEGORY

#define MAX_CLAN_MAKE_MATERIAL_TOLUA MAX_CLAN_MAKE_MATERIAL

enum tagCLAN_MAKE_FOR_TOLUA 
{
	CLAN_MAKE_FOR_COLLECTION_TOLUA = 0, 	/* �ɼ����� */
	CLAN_MAKE_FOR_ARM_TOLUA = 1, 	/* �������� */
};

enum tagCLAN_MAP_POS_TYPE_TOLUA 
{
	CLAN_MAP_POS_TYPE_ATK_TOLUA = 0, 	/* ������� */
	CLAN_MAP_POS_TYPE_VENUE_TOLUA = 1, 	/* ���ϻ�� */
};

struct tagClanOneMapPosInfo_ToLua
{
	int EndTime;                         	/*   ��Чʱ�������ʱ�� */
	int MapID;                          
	tagResPos_ToLua Pos;                            
};

struct tagClanMapPos_ToLua
{
	tagClanOneMapPosInfo_ToLua AtkInfo;                        
	tagClanOneMapPosInfo_ToLua VenueInfo;                      
};

struct tagClanSpyTransDef_ToLua 
{
	int DefID;                           	/*   ID|99(SPY_TASK_NPC_TRANS_DEFID)�̶���ʾ��̽����NPC������ */
	char Name[32];                        	/*   λ������ */
	int X;                               	/*   ����X */
	int Y;                               	/*   ����Y */
};

struct tagClanOneGroupInfo_ToLua
{
	int StrongMap;                      
	int SpanStrongID;                    	/*  Ver.2427 ���ս�ŷ��� */            
};

struct tagClanGroupInfo_ToLua
{
	int UseItemGroup;                   
	int GroupNum;                       
	tagClanOneGroupInfo_ToLua GroupInfo[MAX_CLAN_GROUP_NUM];
};

#define MAX_CLAN_GROUP_NUM_TOLUA MAX_CLAN_GROUP_NUM
#define CLAN_GROUP_MEMBER_MAX_NUM CLAN_GROUP_MAX_NUM

tagClanLevelDef_ToLua*	getClanLevelDef( int nLevel );

#define MEM_NUM_CLAN_SKILL_ID_TOLUA MEM_NUM_CLAN_SKILL_ID
//tolua_end

//������˹���ս�ų���ְ����Ѿ����ܰ���ְλֵ�Ĵ�С��������,����
//������У������ְλ��С,ÿ������˹���ְλ��Ҫ�������޸�,��ֵ�Ĵ�С����ٽ׵ĸߵ�
static const int nRegulateClanPos[] = { 
	0,				//CLAN_POS_NOT_MEMBER = 0, 	/* �ǻ�Ա */
	1,				//CLAN_POS_NORMAL = 1, 	/* ��Ա */
	2,				//CLAN_POS_TECH_OFFICER = 2, 	/* �Ƽ���Ա */
	3,				//CLAN_POS_COLLECTION_OFFICER = 3, 	/* �ɼ���Ա */
	4,				//CLAN_POS_FACTORY_OFFICER = 4, 	/* ������Ա */
	5,				//CLAN_POS_PET_OFFICER = 5, 	/* �����Ա */
	6,				//CLAN_POS_WALL_OFFICER = 6, 	/* ������Ա */
	15,				//CLAN_POS_VICE_PRESIDENT = 7, 	/* ���᳤ */
	16,				//CLAN_POS_PRESIDENT = 8, 	/* �᳤ */
	9,				//CLAN_POS_GROUP_1 = 9, 	/* 1ս���ų� */
	10,				//CLAN_POS_GROUP_2 = 10, 	/* 2ս���ų� */
	11,				//CLAN_POS_GROUP_3 = 11, 	/* 3ս���ų� */
	12,				//CLAN_POS_GROUP_4 = 12, 	/* 4ս���ų� */
	13,				//CLAN_POS_GROUP_5 = 13, 	/* 5ս���ų� */
	14				//CLAN_POS_GROUP_6 = 14, 	/* 6ս���ų� */
};

bool operator< ( const tagClanMem_ToLua& lhs, const tagClanMem_ToLua& rhs );
bool operator== ( const tagOtherClanInfo& clanInfo,const unsigned int clanId );

bool otherClanCompare( const tagOtherClanInfo& lhs,const tagOtherClanInfo& rhs );

bool compareLogNum( const tagClanMem_ToLua& lhs, const tagClanMem_ToLua& rhs );

class GameClanManager//tolua_export
	: public NetMsgHandler, public IGameClanManager, public Ogre::Singleton<GameClanManager>
{//tolua_export
public:
	GameClanManager(void);
	~GameClanManager(void);

public:

	// c->s

	//tolua_begin
	// ��������
	void clan_Opt_Create( const char* szClanName );
	// ���һ����Ա
	void clan_Opt_Invite( const char* szName );
	// ��������
	void clan_Opt_Upgrade();
	// �鿴��ǰ�������
	void clan_Opt_SeeClanCity();
	// ɾ��һ����Ա
	void clan_Opt_DelMember( int nHeight, int nLow, const char* szName );
	// �˳�����
	void clan_Opt_Exit();
	// ������Ա�׼�
	void clan_Opt_UpPostion( int nHeight, int nLow, const char* szName );
	// ���ͳ�Ա�׼�
	void clan_Opt_DownPostion( int nHeight, int nLow, const char* szName );
	// ��ɢ����
	void clan_Opt_Disban();
	// �༭����
	void clan_Opt_EditBulletin( const char* szText );
	// �༭����
	void clan_Opt_EditManifesto( const char* szText );
	// ��λ
	void clan_DemiseJob( int nHeight, int nLow, const char* szName );

	// �������
	void clan_Opt_ApplyJoin( int nHeight, int nLow, int nId, const char* szName );
	// ������ vote:0��ʾ���ԣ�1��ʾͬ��
	void clan_Opt_CensorJoin( int nHeight, int nLow, const char* szName, int nVote );
	// �����ǳ�
	void clan_Opt_SetNickName( int nHeight, int nLow, const char* szName );
	// �鿴��������б�
	void clan_Opt_LookJoinList();
	// ���빫�����ͬ��
	void clan_Opt_InviteJoinUnion( int nHeight, int nLow, int Id, const char* szClanName );
	// �����������ͬ��
	void clan_Opt_ApplyJoinUnion( int nHeight, int nLow, int Id, const char* szClanName );
	// �˳�ͬ��
	void clan_Opt_ExitUnion( int nHeight, int nLow, int Id, const char* szClanName );
	// ��ӵжԹ���
	void clan_Opt_ApplyJoinEnemy( int nHeight, int nLow, int Id, const char* szClanName );
	// ɾ���жԹ���
	void clan_Opt_ExitEnemy( int nHeight, int nLow, int Id, const char* szClanName );
	// �鿴������⽻״̬
	void clan_Opt_LookDiplomatism();
	// �ظ�������� szHostName �����˵����֣�ask 0 ��ͬ�� 1ͬ��
	void clan_Opt_AnswerJoinClan( int nHeight, int nLow, int Id, const char* szClanName, const char* szHostName, int nAsk );
	// �ظ��������� szHostName �����˵����֣�ask 0 ��ͬ�� 1ͬ��
	void clan_Opt_AnserJoinUnion( int nHeight, int nLow, int Id, const char* szClanName, const char* szHostName, int nAsk );
	// �ظ��ж�����
	void clan_Opt_AnswerJoinEnemy( int nHeight, int nLow, int Id, const char* szClanName, const char* szHostName, int nAsk );
	// ��������
	void clan_Opt_SearchClan( int nKeyId );
	// ��ID����������
	void clan_Opt_SearchClanByID( int nKeyId );
	// ����������������
	void clan_Opt_SearchClanByName( const char* pszName );
	// ����������˵Ĺ���
	void clan_Opt_CensorJoinUnion( int nHeight, int nLow, int Id, const char* szName, int nVote );
	// �������жԵĹ���
	void clan_Opt_CensorJoinEnemy( int nHeight, int nLow, int Id, const char* szName, int nVote );
	// �鿴���г�Ա��Ϣ����
	void clan_Opt_GetMemberSnapshot();
	// Ҫ��鿴�����һ����Ϣ
	void clan_Opt_GetClanInfo();
	// �鿴���������
	void clan_Opt_GetManifesto();
	// �鿴������һҳ�Ĺ���
	void clan_Opt_GetPageClan( int nPage );
	// ���빫�����
	void clan_Opt_EnterCity(int clanId);
	//���ͽ��������ָ���
	void clan_Opt_EnterClanPworldIsland();
	//�Ƿ��ڽ��������ָ���
	bool IsInClanPworldIsland();
	//���͵���������鱨�ٴ�
	void Clan_Opt_GoToClanIntellingenceNPC();
	//���͵�ָ�������ָ��λ��
	void Clan_Opt_GoToClanCityPos( int ClanID, int PosID );
	//�Ƿ����ػ���Ŧ����
	bool IsInClanMachinePworld();
	//���ͽ��ػ���Ŧ����
	void Clan_Opt_EnterClanMachinePworld( int ClanID );
	// ��ͼ���
	void clan_Opt_SetMapPos( int nMapId, int nX, int nY, int nType );
	// ��ȡ��ǰ���������Ϣ
	GameClanCityInfo&	getClanCity()
	{
		return m_clanCity;
	}

	void requestClanViewInfo();

	void clan_Opt_Research(int nCat,int nLevel);

	void clan_Opt_CancleResearch(int nIndex);

	void clan_Opt_Make(int nItemID,int nNum );

	void clan_Opt_CancleMake(int nItemID );

	void clan_Opt_Machine_Grab(int nPos);

	void clan_Opt_MachineDestroy( int nPos,int nCat );

	void clan_Opt_Machine_Lock( int nPos ,int nCat, int nState);

	void clan_Opt_Answer_Exit_Enemy( bool bAgree );

	int getServerTime();

	void clan_Opt_Set_Job_And_Title(int pos,int nLowID,int nHightID,char* szTitle);

	void clan_Opt_Skill_Speed_Up(int nCat,int nTime);

	void clan_Opt_Product_Speed_Up(int nID,int nTime);

	//���ù�����ɫ
	void clan_Opt_SetClanColor(unsigned int nClanID,unsigned int nClanColor);

	void clan_Opt_PworldAward();

	void clan_Opt_SearchClanByWID( tdr_ulonglong wid );
	const char* clanGetClanNameByWid( tdr_ulonglong wid);
	const char* clanGetClanNameByID( int clanID );
	int GetClanMemNumByWid( tdr_ulonglong wid );
	int GetClanLVByWid( tdr_ulonglong wid );
	const char* GetClanCaptionNameByWid( tdr_ulonglong wid );

	bool isInClanGroup();
	//tolua_end


public:
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	virtual void objEnterView(GameActor *actor);
	void	setClanViewer( const tagClanProf& clanProf );
	// ���ù���ר��������Ϣ
	void setClanCity( const tagClanCity& cityInfo );
	//tolua_begin
	// ��ȡ�������
	const tagClanViewer& getClanViewer()const;
	// ��ȡ��������б�
	tagApplyJoinList& getApplyJoinList();
	// ��ȡ�����ϵ״��
	const tagClanDiplomatism& getClanDipLomatism()const;
	// �����Ա���� nFlag  1����-1����
	void sortMemberList( int nType, int nFlag = 1 );
	// ����������Ի�Ա��������
	void sortMemberByOnline();
	// ��ȡ��������������
	int getClanTotalNum();
	// ��ȡ������ָ��ҳ������Ϣ
	const tagOtherClanInfo* getOneClanInfoList( int nPage, int nRow, bool isSearch )const;
	// ��ȡ����ѯ���Ĺ�������
	int getQueryClanTotalNum();
	// ��ȡ����ѯ���Ĺ�����Ϣ
	const tagOtherClanInfo* getQueryClanInfoList( int nRow ) const;
	// ��ѯĳ�����Ƿ��ǹ����Ա
	bool isClanMember( int nHeight, int nLow );
	// ��ѯĳ�����Ƿ��ǹ����Ա
	bool isClanMember( const char* pszName );
	// ���ù�����Ϣ
	void resetClanInfo();
	//����������ȡ������Ϣ
	const tagOtherClanInfo* getOneClanInfoByIndex( int nIdx )const;

	void sortOtherClan( int nType, int nFlag, bool bSearch );

	tagClanTechDef_ToLua* getClanTechDef( int nCat,int nLevel );

	int getMaxSkillLevel( int nCat );

	tagClanRunningProgress_ToLua* getTechProgress( int nCat );
	tagClanRunningProgress_ToLua* getProductProgress(int nID);
	tagClanRunningProgress_ToLua* getProductProgressByIdx(int nIdx);

	tagClanAdvance_ToLua* getAdvanceInfo();
	int getProductNum();
	tagClanProductDef_ToLua* getProductDefByID( int nID );
	tagClanProductDef_ToLua* getProductDefByIndex(int nIndex);

	tagClanMachineProp_ToLua* getMachineProp( int nMonsterID );

	const tagClanMem_ToLua*	getMemberInfo( const char* szName );

	//�ж�ͬ�˹���
	bool isFriendClan( tagLong_ToLua clanWid );
	//�жϵжԹ���
	bool isEnemyClan( tagLong_ToLua clanWid );
	// ��ȡ����Ĭ����ɫ
	unsigned int	getDefaultClanColor(const tagLong_ToLua &clanWid);
	unsigned int	getDefaultClanColor(unsigned int id);
	unsigned int	getClanColor(const tagLong_ToLua &clanWid);
	unsigned int	getClanColor(unsigned int id);
	bool			hasClanColor(const tagLong_ToLua &clanWid);
	bool			hasClanColor(unsigned int id);
	// ����ֿ�Э���޸�
	int							getStorageItemNum();
	tagClanResourceInfo_ToLua *	getStorageItemInfo(int idx);
	int							getStorageResourceNum(int resouceId);
	void						lockSwitchStorage(int resourceId, int state);
	void						lockSwitchStorageByIdx(int idx, int state);
	void						exchangeStorageItem(int resourceId, int num);
	void						exchangeStorageItemByIdx(int idx, int num);

	tagClanMapPos_ToLua*		getClanMapPos();

	int GetClanYesterdayTouchNum(){ return m_nClanYesterdayTouchNum; }

	int getCurClanCityId() { return m_CurClanCityId; }
	int getClanNpcsCount();
	const tagMapActorInfo &getClanNpcInfo(int index);
	const tagMapActorInfo *getClanNpcInfoById(int id);
	const tagClanSpyTransDef_ToLua*	getClanSpyTransDef( int id );

	int GetClanDonateLogNum();

	const tagClanMem_ToLua* GetClanDonateLog( int nIndex );

	void SortDonateLog( int nType, bool bFlag );

	void Clan_Opt_SkillResearch( int id, int level );

	void Clan_Opt_SkillLearn( int id, int level );

	const tagClanSkillDef_ToLua* getClanSkillDef( int id, int lv );

	const tagClanSkillInfo_ToLua* getClanSkillInfo();

	const tagPlayerClanSkillInfo_ToLua* getPlayerClanSkillInfo();

	int getAllCLanSkillNum(){ return (int)m_vClanSkill.size(); }
	const tagClanSkillDef_ToLua* getClanSkillDefByIndex( int nIndex );
	int getPlayerClanSkillNum() { return (int)m_PlayerCLanSkill.Num; }

	//�����nҳ�Ĺ�����Դ��ȡ��¼
	void Clan_Opt_AskGetResInfo( int nPage );
	tagClanViewGetRes_ToLua getCalnViewGetRes(){return m_ClanViewGetRes;}
	void ClearClanViewGetRes();
	void Clan_Opt_SummonRequest( bool bAgree );

	//���ó�Ա����ս��
	void Clan_Opt_SetGroup( int roleIdx, int groupIdx, int set );
	//����ս�Ų���ݵ�
	void Clan_Opt_SetGroupStrongMap( int groupIdx, int map );
	//���������ս��
	void Clan_Opt_OpenGroup( int groupIdx );

	tagClanGroupInfo_ToLua getClanGroupInfo(){return m_ClanGroupInfo;}

	const char* getMemberNameByPos( int pos );
	// ���� �����������
	void	Clan_Pet_JiYang( int nIndex, int nHour, int nHeight, int nLow, int nFlag = 0 );
	void	Clan_Pet_JiYangEnd( int nIndex, int nHeight, int nLow );
	void	Clan_Pet_WanShua( int nIndex, int nGrid, int nType, int nHeight, int nLow );
	void	Clan_Pet_Seduce( int nIndex, int nHeight, int nLow );
	void	Clan_Pet_Eat( int nIndex, int nGrid, int nHeight, int nLow );
	void	Clan_Pet_Work( int nIndex, int nHeight, int nLow );
	void	Clan_Pet_WorkAward( int nIndex, int nHeight, int nLow );
	void	Clan_Pet_Clear( int nIndex, int nHeight, int nLow );
	tagClanPetOne_ToLua*	getClanPetData( int nIdx );
	int		getClanPetNum();
	void	updateClanPetRailing( int nPosX, int nPosZ, int nIdx );
	void	cleanClanPetInfo();
	int		getClanPetFreeNum();

	int		getClanCostLastTime(){ return m_ClanCostLastTime; }
	int		getClanCostNextTime(){ return m_ClanCostNextTime; }

	int		getClanPetHighLvLastTime(){ return m_ClanPetHighLvLastTime; }
	
	//tolua_end

	void	changeClanMemberName( const char* pszOldName, const char* pszNewName );
	int		getClanSortType(){return m_ClanSortType;}
	int		getClanSortFlag(){return m_ClanSortFlag;}

	void update();
	void UpdateClanPetNpcTalk();
	void updateAllClan();
	void updateClan(GameActor *pActor);

	void onEnterClanMap();
	void onLeaveClanMap();
	void checkClanBuildChange();

	bool isClanOfficer( const char* pszName );

	int CLanMenmberOnlineinfo( const char* pszName );

	void changeClanName( tdr_ulonglong wid, const char* szNewName );

	void SetClanCityInfo(int id, const tagClanCity &info);	

	void updateClanDonateInfo();
	int getDonateSortType(){return m_nDonateSortType;}
	bool getSortFlag(){return m_bFlag;}

	void updateClanSkillInfo( const tagClanSkillInfo &info );
	void updatePlayerClanSkillInfo( const tagPlayerClanSkillInfo &info );

private:
	// ���ó�Ա��Ϣ
	void setClanInfo( tagOtherClanInfo* pClanInfo, const void* pInfo );
	// ��Ա�Ƚ�
	int cmpMember( const tagClanMem_ToLua& src, const tagClanMem_ToLua& des, int nType=QUICKSORT_BY_ONLINE,int nFlag=1 );


	void modifyOtherClanData(tagCSClanView view);

	void deleteOtherClanData( unsigned int clanId );	

private:
	tagClanViewer		m_ClanViewer;		// ���������Ϣ
	int					m_nSortType;		// ��ǰ�����Ա����
	tagApplyJoinList	m_ApplyJoinList;	// ��������б�
	tagClanDiplomatism	m_ClanDipomatism;	// �����ϵ״��
	std::map<unsigned int, unsigned int>	m_ClanColor;
	std::map<tdr_ulonglong, unsigned int>	m_ClanIDMap;
	std::map<unsigned int, tdr_ulonglong>	m_ClanIDMapReverse;
	std::vector<tagOtherClanInfo>	m_QueryClanInfoList;	// ����ѯ���Ĺ����б�

	int								m_nClanTotalNum;	// �����������еĹ�������
	// 	std::map<int, tagOnePageClan>	m_clanMap;			// ������������Ϣ
	GameClanCityInfo				m_clanCity;			// �������

	std::vector<unsigned int>		m_otherClanIDVec;
	std::vector<tagOtherClanInfo>	m_otherClanVec;		//

	int m_ClanSortType;
	int m_ClanSortFlag;

	CLANID	m_EnemyID;
	char	m_szEnemyHostName[NAME_LEN];

	unsigned int					m_uLastRequestTick;
	tagClanAdvance_ToLua			m_ClanAdvace;
	tagClanSkillInfo_ToLua			m_ClanSkill;
	tagPlayerClanSkillInfo_ToLua	m_PlayerCLanSkill;
	std::vector<tagClanMem_ToLua>		m_vecSortDonateInfo;//������Ŀ��Ϣ	
	int					m_nDonateSortType;
	bool				m_bFlag;

	Ogre::Entity*		m_CityEntities[MAX_CLAN_BUILDING];
	Ogre::Entity*		m_ClanCityEntity;

	tagClanMapPos		m_ClanMapPos;

	int					m_CurClanCityId;
	tagClanCity			m_ClanCityInfo;

	int					m_nClanYesterdayTouchNum;
	std::vector<tagMapActorInfo>		m_vClanNpcs;
	std::vector<tagClanSkillDef*>		m_vClanSkill;

	tagClanViewGetRes_ToLua				m_ClanViewGetRes;
	int									m_ClanSummonMemId;		//�ٻ���ID

	tagClanGroupInfo_ToLua				m_ClanGroupInfo;		//���������Ϣ

	std::map<int, tagClanPetOne_ToLua>	m_ClanPetAll;			// ����������� ��λ��Ϣ
	int									m_ClanPetMax;
	int									m_ClanPetFree;
	Ogre::AvatarEntity*					m_ClanPetEntities[CLAN_PET_MAX_JIYANG];			// ����
	int									m_ClanPetHighLvLastTime;		//����߼�����ʣ��ʱ��

	int									m_ClanCostLastTime;		//�����ϴο۷�ʱ��
	int									m_ClanCostNextTime;		//�����´ο۷�ʱ��
};//tolua_export
