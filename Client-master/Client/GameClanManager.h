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

// 成员排序类型
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
	OTHER_CLAN_SORT_ID		= 1,	//公会ID查询
	OTHER_CLAN_SORT_NAME	= 2,	//公会名查询
	OTHER_CLAN_SORT_NUM		= 3,	//公会人数查询
	OTHER_CLAN_SORT_LEVEL	= 4,	//公会等级查询
	OTHER_CLAN_SORT_COLOR	= 5,	//公会颜色
};

enum
{
	TIME_SORT	= 1,//捐献时间
	MONEY_SORT	= 2,//捐献数目
	CLAN_SORT	= 3,//公会资金
	CONTRI_SORT	= 4,//获得贡献
};

#define MAX_CAREER_NUM	4	//职业数量

#define CLAN_WAR_COST_TOLUA CLAN_WAR_COST
#define MIN_BUUL_CREATE_CLAN TMPCLAN_CREATE_MIN_MONEY

#pragma pack(1)
struct tagClanCityProf_ToLua
{
	unsigned char				Level;                           	/*   城市等级 > 0 说明城市已被激活 */
	tagClanCityBuilding_ToLua	Buildings[MAX_CLAN_BUILDING]; 	/*   公会建筑 */
};

// 成员信息
struct tagMemberInfo
{
	tagLong_ToLua	m_RoleLongId;
	char			m_szRoleName[ROLE_NAME_LEN_SAVE];   
};

struct tagClanMem_ToLua
{
	tagMemberInfo	RoleMember;                     
	char			NickName[NAME_LEN];              	/*   昵称 */
	unsigned char	Position;                        	/*   公会职位 Bind Macrosgroup:CLAN_POSITION,*/
	unsigned char	Level;                           	/*   等级 */
	unsigned char	Career;                          	/*   职业 */
	unsigned char	Online;                          	/*   是否在线 */
	unsigned int	Contribute;                      	/*   共享度 */
	int				LastOnline;                      	/*   最后的在线时间 */
	int				LastLogin;                       	/*  Ver.124 最后的登陆时间 */
	int				TotalOnline;                     	/*  Ver.242 总计在线时间 */
	char			Title[CLAN_MEM_TITLE_LEN];       	/*  Ver.497 封号 */
	int				JoinTime;                        	/*  Ver.1048 入会时间 */
	unsigned int	TotalContribute;                 	/*  Ver.1349 贡献度总数 */
	int DonateNum;                       	/*  Ver.1567 捐献总数 */
	int DonateClanMoney;                 	/*  Ver.1567 捐献公会资金总数 */
	int DonateContri;                    	/*  Ver.1567 捐献获得贡献度总数 */
	int DonateLastTime;                  	/*  Ver.1567 最后捐献时间 */
    int GroupID;                         	/*  Ver.1957 所属公会战团 */
};

// 公会信息快照

struct tagClanInfo_ToLua
{
	tagLong_ToLua		m_clanLongId;				
	unsigned int		m_uClanId;
	int					m_nClanLevel;						// 公会等级
	int					m_nClanMemNum;						// 成员数
	char				m_szClanName[NAME_LEN];				// 公会名称
	char				m_szBulletinText[SHORT_DESC_LEN];   // 公告信息
	unsigned int		m_nMoney;							// 公会资金
	unsigned int		m_nBuildingValue;                   // 公会建设度	安定度
	unsigned int		m_nActiveValue;                     // 公会繁荣度	繁荣度
	unsigned int		m_nSkillPoint;                      // 公会技能点
	char				m_szCaptionName[NAME_LEN];			// 公会会长名
	int					m_BulletinTextLastEditTime;			//最后一次修改公告的时间
	int					m_nTime;							// 敌对公会用
	int					m_Flag;								//公会标记CLAN_FLAG,是否可刺探等标记
};

// 本公会信息
struct tagClanViewer
{
	tagClanInfo_ToLua	m_clanInfo;
	tagMemberInfo		m_PresidentInfo;			// 会长信息		
	tagClanMem_ToLua	m_Members[CLAN_MAX_MEM];	// 公会成员快照信息
};

// 其他公会信息
struct tagClanBrief_ToLua
{
	tagMemberInfo Leader;                         
	int CreateTime;                      	/*   公会创建时间(UTC) */
	unsigned short AssistNum;                       	/*   副会长数量 */
	tagMemberInfo Assists[CLAN_MAX_ASSIST];        	/*   副会长名字与ID */
	char XuanGao[SHORT_DESC_LEN];         	/*   宣告:公会外人员可见 */
};

struct tagOtherClanInfo
{
	tagClanInfo_ToLua			m_ClanInfo;
	tagClanBrief_ToLua			m_ClanBrief;
	tagClanCityProf_ToLua		m_cityInfo;
};

// 公会查询页
struct tagOnePageClan
{
	CSCLANVIEWPAGECOOKIE		m_cookie;			// 当前页cook
	std::list<tagOtherClanInfo>	m_clanInfoList;		// 当前页公会列表
};

struct tagClanApply_ToLua
{
	tagMemberInfo RoleMember;                     
	unsigned char Level;                          
	unsigned char Career;                        
	unsigned short Res3;                           
	int ApplyTime;                      
};

// 申请列表
struct tagApplyJoinList
{
	int  m_nRoleNum;                              
	tagClanApply_ToLua m_ApplyRoles[CLAN_MAX_APPLY];
};

// 申请公会的信息
struct tagApplyClanInfo
{       
	tagClanInfo_ToLua	m_ClanInfo;			// 公会信息	
	int					m_nApplyTime;		// 申请时间
};

struct tagClanDiplomatism
{
	int					m_nFriendNum;			// 同盟公会数
	int					m_nEnemyNum;			// 敌对公会数
	int					m_nApplyFriendNum;		// 申请同盟公会数
	int					m_nApplyEnemyNum;		// 申请敌对公会数
	tagClanInfo_ToLua	m_Friends[CLAN_MAX_FRIEND];       
	tagClanInfo_ToLua	m_Enemys[CLAN_MAX_ENEMY];       
	tagApplyClanInfo	m_ApplyFriends[CLAN_MAX_APPLY_FRIEND];
	tagApplyClanInfo	m_ApplyEnemys[CLAN_MAX_APPLY_ENEMY];
};

struct tagClanWorkingCost_ToLua
{
	unsigned int IntervalTime;                    	/*   消耗间隔时间（秒） */
	unsigned int Memory;                          	/*   公会资金 */
	unsigned int BuildingValue;                   	/*   建设度 */
	unsigned int ActiveValue;                     	/*   活跃度 */
};

struct tagResourceLimit_ToLua
{
	int ResourceID;                      	/*   资源ID */
	int Limit;                           	/*   资源限制 */
};

struct tagClanLevelDef_ToLua
{
	unsigned int Level;                           	/*   公会等级 */
	int MaxMemberNumLimit;               	/*   当前等级的最大成员数显示 */
	int MaxResourceGrid;                 	/*   公会资源仓库最大可用格子数 */
	int MaxMachineGrid;                  	/*   公会机械仓库最大可用格子数 */
	int LvLimited[MAX_CLAN_CONSTRUCTION]; 	/*   对建筑和城市的等级限制 */
	tagClanWorkingCost_ToLua WorkingCost;                     	/*   公会的消耗 */
	tagClanUpgradeRequired_ToLua Required;                        	/*   升级条件 */
	int RequiredCityLv;                  	/*  Ver.534 需要的公会城市最小等级 */
	char EffectString[RES_NAME_LEN];      	/*  Ver.601 效果字符串 */
	int EnemyCDTime;                     	/*  Ver.750 宣战CD时间单位秒 */
	tagResourceLimit_ToLua ResourceLimit[MAX_CLAN_RESOURCE]; 	/*  Ver.1000 公会资源限制 */
	int EveryDayPworldClanMoney;         	/*  Ver.1083 个人积分奖励公会资金基准值 */
	int EveryDayPworldContri;            	/*  Ver.1083 个人积分奖励贡献度基准值 */
	int MaxEveryDayPworldVal;            	/*  Ver.1083 每日积分活动副本积分上限满足给奖励 */
	int ClanValContri;                   	/*  Ver.1083 积分达到上限奖励的贡献度 */
	int PresentClanMoney;                	/*  Ver.1562 赠送的公会资金 */
	int CostClanMoney;                   	/*  Ver.1562 废弃|请填写WorkingCost_Money字段 */
	int MinClanMoney;                    	/*  Ver.1562 最小的公会资金数(小于此值则降级) */
	int LevelDownTime;                   	/*  Ver.1564 公会降级缓冲时间(秒) */
	char OccuNum;                         	/*  Ver.2252 能占领据点数 */
	char FreeFence;                       	/*  Ver.2289 免费栏位 */
	char MoneyFence;                      	/*  Ver.2289 收费栏位 */
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
	int PageCount;                       	/*   一共有多少页*/
	int PageCurr;                        	/*   当前第几页*/
	int InfoNum;                        
	tagGetResInfoOne_ToLua GetResInfos[CLAN_VIEW_GETRES_NUM];
}; 
struct tagClanPetOp_ToLua
{
	char RoleName[ROLE_NAME_LEN];        
	char OpType;                          	/*   1-喂食 2-玩耍 3-偷 */
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
	int WorkFlag;                        	/*   1-未领取 */
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
// 	"非会员",
// 	"平民",
// 	"科技官员",
// 	"采集官员",
// 	"工厂官员",
// 	"宠物官员",
// 	"郊区官员",
// 	"副会长",
// 	"会长",
// };

#define MAXCLANLISTROW 15		//公会查询一页的数量
#define MAX_CLAN_TECH_CATEGORY_TOLUA MAX_CLAN_TECH_CATEGORY

#define MAX_CLAN_MAKE_MATERIAL_TOLUA MAX_CLAN_MAKE_MATERIAL

enum tagCLAN_MAKE_FOR_TOLUA 
{
	CLAN_MAKE_FOR_COLLECTION_TOLUA = 0, 	/* 采集制造 */
	CLAN_MAKE_FOR_ARM_TOLUA = 1, 	/* 军事制造 */
};

enum tagCLAN_MAP_POS_TYPE_TOLUA 
{
	CLAN_MAP_POS_TYPE_ATK_TOLUA = 0, 	/* 进攻会标 */
	CLAN_MAP_POS_TYPE_VENUE_TOLUA = 1, 	/* 集合会标 */
};

struct tagClanOneMapPosInfo_ToLua
{
	int EndTime;                         	/*   有效时间服务器时间 */
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
	int DefID;                           	/*   ID|99(SPY_TASK_NPC_TRANS_DEFID)固定表示刺探任务NPC的坐标 */
	char Name[32];                        	/*   位置名称 */
	int X;                               	/*   坐标X */
	int Y;                               	/*   坐标Y */
};

struct tagClanOneGroupInfo_ToLua
{
	int StrongMap;                      
	int SpanStrongID;                    	/*  Ver.2427 跨服战团分配 */            
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

//在添加了公会战团长的职务后，已经不能按照职位值的大小来排序了,所以
//在这里校正公会职位大小,每次添加了公会职位后，要在这里修改,数值的大小代表官阶的高低
static const int nRegulateClanPos[] = { 
	0,				//CLAN_POS_NOT_MEMBER = 0, 	/* 非会员 */
	1,				//CLAN_POS_NORMAL = 1, 	/* 会员 */
	2,				//CLAN_POS_TECH_OFFICER = 2, 	/* 科技官员 */
	3,				//CLAN_POS_COLLECTION_OFFICER = 3, 	/* 采集官员 */
	4,				//CLAN_POS_FACTORY_OFFICER = 4, 	/* 工厂官员 */
	5,				//CLAN_POS_PET_OFFICER = 5, 	/* 宠物官员 */
	6,				//CLAN_POS_WALL_OFFICER = 6, 	/* 郊区官员 */
	15,				//CLAN_POS_VICE_PRESIDENT = 7, 	/* 副会长 */
	16,				//CLAN_POS_PRESIDENT = 8, 	/* 会长 */
	9,				//CLAN_POS_GROUP_1 = 9, 	/* 1战团团长 */
	10,				//CLAN_POS_GROUP_2 = 10, 	/* 2战团团长 */
	11,				//CLAN_POS_GROUP_3 = 11, 	/* 3战团团长 */
	12,				//CLAN_POS_GROUP_4 = 12, 	/* 4战团团长 */
	13,				//CLAN_POS_GROUP_5 = 13, 	/* 5战团团长 */
	14				//CLAN_POS_GROUP_6 = 14, 	/* 6战团团长 */
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
	// 创建公会
	void clan_Opt_Create( const char* szClanName );
	// 添加一个成员
	void clan_Opt_Invite( const char* szName );
	// 升级公会
	void clan_Opt_Upgrade();
	// 查看当前公会城市
	void clan_Opt_SeeClanCity();
	// 删除一个成员
	void clan_Opt_DelMember( int nHeight, int nLow, const char* szName );
	// 退出公会
	void clan_Opt_Exit();
	// 提升成员阶级
	void clan_Opt_UpPostion( int nHeight, int nLow, const char* szName );
	// 降低成员阶级
	void clan_Opt_DownPostion( int nHeight, int nLow, const char* szName );
	// 解散公会
	void clan_Opt_Disban();
	// 编辑公会
	void clan_Opt_EditBulletin( const char* szText );
	// 编辑宣言
	void clan_Opt_EditManifesto( const char* szText );
	// 让位
	void clan_DemiseJob( int nHeight, int nLow, const char* szName );

	// 申请入会
	void clan_Opt_ApplyJoin( int nHeight, int nLow, int nId, const char* szName );
	// 入会审查 vote:0表示反对，1表示同意
	void clan_Opt_CensorJoin( int nHeight, int nLow, const char* szName, int nVote );
	// 设置昵称
	void clan_Opt_SetNickName( int nHeight, int nLow, const char* szName );
	// 查看入会请求列表
	void clan_Opt_LookJoinList();
	// 邀请公会加入同盟
	void clan_Opt_InviteJoinUnion( int nHeight, int nLow, int Id, const char* szClanName );
	// 公会申请加入同盟
	void clan_Opt_ApplyJoinUnion( int nHeight, int nLow, int Id, const char* szClanName );
	// 退出同盟
	void clan_Opt_ExitUnion( int nHeight, int nLow, int Id, const char* szClanName );
	// 添加敌对公会
	void clan_Opt_ApplyJoinEnemy( int nHeight, int nLow, int Id, const char* szClanName );
	// 删除敌对公会
	void clan_Opt_ExitEnemy( int nHeight, int nLow, int Id, const char* szClanName );
	// 查看公会的外交状态
	void clan_Opt_LookDiplomatism();
	// 回复入会邀请 szHostName 请求人的名字，ask 0 不同意 1同意
	void clan_Opt_AnswerJoinClan( int nHeight, int nLow, int Id, const char* szClanName, const char* szHostName, int nAsk );
	// 回复入盟邀请 szHostName 请求人的名字，ask 0 不同意 1同意
	void clan_Opt_AnserJoinUnion( int nHeight, int nLow, int Id, const char* szClanName, const char* szHostName, int nAsk );
	// 回复敌对请求
	void clan_Opt_AnswerJoinEnemy( int nHeight, int nLow, int Id, const char* szClanName, const char* szHostName, int nAsk );
	// 搜索公会
	void clan_Opt_SearchClan( int nKeyId );
	// 用ID来搜索公会
	void clan_Opt_SearchClanByID( int nKeyId );
	// 用名字来搜索公会
	void clan_Opt_SearchClanByName( const char* pszName );
	// 审查申请入盟的公会
	void clan_Opt_CensorJoinUnion( int nHeight, int nLow, int Id, const char* szName, int nVote );
	// 审查申请敌对的公会
	void clan_Opt_CensorJoinEnemy( int nHeight, int nLow, int Id, const char* szName, int nVote );
	// 查看所有成员信息快照
	void clan_Opt_GetMemberSnapshot();
	// 要求查看公会的一般信息
	void clan_Opt_GetClanInfo();
	// 查看公会的宣言
	void clan_Opt_GetManifesto();
	// 查看服务器一页的公会
	void clan_Opt_GetPageClan( int nPage );
	// 进入公会城市
	void clan_Opt_EnterCity(int clanId);
	//传送进驱逐入侵副本
	void clan_Opt_EnterClanPworldIsland();
	//是否在进驱逐入侵副本
	bool IsInClanPworldIsland();
	//传送到公会城市情报官处
	void Clan_Opt_GoToClanIntellingenceNPC();
	//传送到指定公会的指定位置
	void Clan_Opt_GoToClanCityPos( int ClanID, int PosID );
	//是否在守护枢纽副本
	bool IsInClanMachinePworld();
	//传送进守护枢纽副本
	void Clan_Opt_EnterClanMachinePworld( int ClanID );
	// 地图会标
	void clan_Opt_SetMapPos( int nMapId, int nX, int nY, int nType );
	// 获取当前公会城市信息
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

	//设置公会颜色
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
	// 设置工会专属城市信息
	void setClanCity( const tagClanCity& cityInfo );
	//tolua_begin
	// 获取公会快照
	const tagClanViewer& getClanViewer()const;
	// 获取申请入会列表
	tagApplyJoinList& getApplyJoinList();
	// 获取公会关系状况
	const tagClanDiplomatism& getClanDipLomatism()const;
	// 公会成员排序 nFlag  1正向-1反向
	void sortMemberList( int nType, int nFlag = 1 );
	// 按在线情况对会员进行排序
	void sortMemberByOnline();
	// 获取服务器公会数量
	int getClanTotalNum();
	// 获取服务器指定页公会信息
	const tagOtherClanInfo* getOneClanInfoList( int nPage, int nRow, bool isSearch )const;
	// 获取所查询到的公会数量
	int getQueryClanTotalNum();
	// 获取所查询到的公会信息
	const tagOtherClanInfo* getQueryClanInfoList( int nRow ) const;
	// 查询某个人是否是工会成员
	bool isClanMember( int nHeight, int nLow );
	// 查询某个人是否是工会成员
	bool isClanMember( const char* pszName );
	// 重置公会信息
	void resetClanInfo();
	//根据索引获取公会信息
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

	//判断同盟公会
	bool isFriendClan( tagLong_ToLua clanWid );
	//判断敌对公会
	bool isEnemyClan( tagLong_ToLua clanWid );
	// 获取公会默认颜色
	unsigned int	getDefaultClanColor(const tagLong_ToLua &clanWid);
	unsigned int	getDefaultClanColor(unsigned int id);
	unsigned int	getClanColor(const tagLong_ToLua &clanWid);
	unsigned int	getClanColor(unsigned int id);
	bool			hasClanColor(const tagLong_ToLua &clanWid);
	bool			hasClanColor(unsigned int id);
	// 公会仓库协议修改
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

	//请求第n页的公会资源领取记录
	void Clan_Opt_AskGetResInfo( int nPage );
	tagClanViewGetRes_ToLua getCalnViewGetRes(){return m_ClanViewGetRes;}
	void ClearClanViewGetRes();
	void Clan_Opt_SummonRequest( bool bAgree );

	//设置成员加入战团
	void Clan_Opt_SetGroup( int roleIdx, int groupIdx, int set );
	//设置战团参与据点
	void Clan_Opt_SetGroupStrongMap( int groupIdx, int map );
	//激活第六个战团
	void Clan_Opt_OpenGroup( int groupIdx );

	tagClanGroupInfo_ToLua getClanGroupInfo(){return m_ClanGroupInfo;}

	const char* getMemberNameByPos( int pos );
	// 公会 宠物寄养部分
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
	// 设置成员信息
	void setClanInfo( tagOtherClanInfo* pClanInfo, const void* pInfo );
	// 成员比较
	int cmpMember( const tagClanMem_ToLua& src, const tagClanMem_ToLua& des, int nType=QUICKSORT_BY_ONLINE,int nFlag=1 );


	void modifyOtherClanData(tagCSClanView view);

	void deleteOtherClanData( unsigned int clanId );	

private:
	tagClanViewer		m_ClanViewer;		// 公会快照信息
	int					m_nSortType;		// 当前公会成员排序
	tagApplyJoinList	m_ApplyJoinList;	// 申请入会列表
	tagClanDiplomatism	m_ClanDipomatism;	// 公会关系状况
	std::map<unsigned int, unsigned int>	m_ClanColor;
	std::map<tdr_ulonglong, unsigned int>	m_ClanIDMap;
	std::map<unsigned int, tdr_ulonglong>	m_ClanIDMapReverse;
	std::vector<tagOtherClanInfo>	m_QueryClanInfoList;	// 所查询到的公会列表

	int								m_nClanTotalNum;	// 服务器中所有的公会数量
	// 	std::map<int, tagOnePageClan>	m_clanMap;			// 服务器公会信息
	GameClanCityInfo				m_clanCity;			// 公会城市

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
	std::vector<tagClanMem_ToLua>		m_vecSortDonateInfo;//捐献条目信息	
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
	int									m_ClanSummonMemId;		//召唤人ID

	tagClanGroupInfo_ToLua				m_ClanGroupInfo;		//公会军团信息

	std::map<int, tagClanPetOne_ToLua>	m_ClanPetAll;			// 公会寄养宠物 栏位信息
	int									m_ClanPetMax;
	int									m_ClanPetFree;
	Ogre::AvatarEntity*					m_ClanPetEntities[CLAN_PET_MAX_JIYANG];			// 宠物
	int									m_ClanPetHighLvLastTime;		//公会高级寄养剩余时间

	int									m_ClanCostLastTime;		//公会上次扣费时间
	int									m_ClanCostNextTime;		//公会下次扣费时间
};//tolua_export
