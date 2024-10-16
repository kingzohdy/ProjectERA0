#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameDefineTolua.h"
#include "GameMsgPrint.h"
#include "ClientInterface.h"
#include <map>
#include <vector>

//tolua_begin
enum  
{
	CLIENT_RELATION_TYPE_NONE = 0, 
	CLIENT_RELATION_TYPE_FRIEND = 1,		// 好友
	CLIENT_RELATION_TYPE_MASTER = 2,		// 师傅
	CLIENT_RELATION_TYPE_PRENTICE = 4,		// 徒弟
	CLIENT_RELATION_TYPE_SPOUSE = 8,		// 夫妻
	CLIENT_RELATION_TYPE_BROTHER = 16,		// 结拜
	CLIENT_RELATION_TYPE_SU_DI = 32,	// 追杀
	CLIENT_RELATION_TYPE_BLACK = 64,		// 黑名单
	CLIENT_RELATION_TYPE_TMPFRIEND = 128,	// 临时
	CLIENT_RELATION_TYPE_XIONG_SHOU = 256,		// 仇人	
	CLIENT_RELATION_TYPE_NEWCOMER = 512	//新人
};

enum  
{
	CLIENT_OFFLINE	= 0,		// 离线
	CLIENT_ONLINE	= 1,		// 在线
	MUD_ONLINE		= 2,
};

enum tagINTIMATE_COLOR_TYPE_TOLUA
{
	INTIMATE_COLOR_FRIEND_TOLUA = INTIMATE_COLOR_FRIEND, 	/* 普通朋友关系 */
	INTIMATE_COLOR_LOVERS_TOLUA = INTIMATE_COLOR_LOVERS, 	/* 情侣关系，粉红色 */
	INTIMATE_COLOR_SPOUSE_TOLUA = INTIMATE_COLOR_SPOUSE, 	/* 夫妻关系，紫红色 */
};

// 好友信息
struct tagFriendInfo 
{
	tagLong_ToLua	m_RoleID;
	int				m_nRelationType;
	char			m_szName[NAME_LEN];			/* RELATION_ATTR_NAME,   */
	int				m_nGender;					/* RELATION_ATTR_GENDER,   */
	int				m_nLevel;					/* RELATION_ATTR_LEVEL,   */
	int				m_nBuddyVal;				/* RELATION_ATTR_BUDDYVAL,   */		
	int				m_nHead;					/* RELATION_ATTR_HEAD,   */
	int				m_nHair;
	int				m_nFace;
	int				m_nCareer;					/* RELATION_ATTR_CAREER,   */
	int				m_nIsOnline;				/* RELATION_ATTR_ONLINE,  0不在线 1在线 */
	char			m_szEmotion[EMOTION_LEN];	/* RELATION_ATTR_EMOTION,   */
	char			m_szMemName[NAME_LEN];		/* RELATION_ATTR_MEMNAME,   */
	char			m_szClanName[NAME_LEN];
	unsigned int	m_nClanID;
	bool			m_bOutSide;				//是否是别的服务器的
	int				m_nGprsEnd;				// RELATION_ATTR_GPRSEND
	int				m_nGroupID;
	int				m_nIntimateColorType;		//亲密度关系：0普通朋友，1情侣关系，2夫妻关系
};

struct tagCSBuddyInfoRes_ToLua
{
	int Level;                          
	int Career;                         
	int Head;                           
	int Hair;                           
	int Face;                           
	int Gender;                         
	char RoleName[NAME_LEN];             
	char ClanName[NAME_LEN];
	tdr_ulonglong ClanWid;
	RESPOS Pos;
	int Map;
	int FriendVal;
	unsigned char TeamRoleNum;
	char Emotion[EMOTION_LEN];
	int Span;                            	/*  Ver.1776  */
	unsigned int Charm;                     /*  Ver.2055  */
};

int getServeSecondTime();
//对好友列表排序规则
bool otherFriendCompare(const tagFriendInfo& lhs,const tagFriendInfo& rhs );
//tolua_end

bool operator< ( const tagFriendInfo& lhs, const tagFriendInfo& rhs );
bool operator==( const tagFriendInfo& lhs, const tagFriendInfo& rhs );
bool operator== ( const tagFriendInfo& friendInfo, const char* pszName );
bool operator== ( const tagFriendInfo& friendInfo, const tagLong_ToLua& roleID );
bool operator== ( const tagFriendInfo& friendInfo, const tdr_ulonglong roleWID );


typedef std::vector<tagFriendInfo>				FRI_VECTOR;
typedef std::vector<tagFriendInfo>::iterator	FRI_VECTOR_ITER;
typedef std::map<int, FRI_VECTOR>				FRI_MAP;
typedef std::map<int, FRI_VECTOR>::iterator		FRI_MAP_ITER;

class GameFriendManager//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GameFriendManager>, public IGameFriendManager
{//tolua_export
public:
	GameFriendManager(void);
	~GameFriendManager(void);
	virtual void objEnterView(GameActor *actor);
	
	// c->s
public:
	//tolua_begin
	
	void reSetFriendList( const char* pszCallStack );
	
	// 追踪某人
	void track( int nFlag, const char* pszName );
	// 重置好友列表
	bool canBeFriend( const char* pszRoleName );
	// 加好友(注意：界 面需要限定名字和附加信息大小)
	void addFriend( const char* szRoleName, const char* szReqInfo );
	// 请求某人信息
	void requirePlayerInfo( const char* szRoleName, int nType );
	// 添加临时好友
	void addTmpFriend( const char* szName );
	// 添加临时仇人
	void addTmpEnemy( const char* szName );
	// 加黑名单
	void addBlack( const char* szRoleName );
	// 加追杀名单
	void addEnem( const char* szRoleName, int nRoleHightId, int nRoleLowId );
	// 可以接受好友邀请
	bool canAcceptBeFriend( const char* pszRoleName );
	// 好友添加消息回复
	// 说明：Accept=0 表示拒绝, AddOther=0 表示不添加对方为好友,客户端收到RELATION_OP_TYPE_CHG消息后,添加好友	AddOther=1表示忽略别人加好友的请求
	void addFriendRes( const char* szRoleName, int nAccept, int nAddOther, bool bTimeOut = false );
	// 修改心情签名
	void changeEmotion( const char* szEmotion );
	// 设置好友分组
	void setFriendGroup( const char* szRoleName, int nGroupID );
	// 获取最大好友数()
	int getMaxFriendNum();
	// 修改备注姓名
	// 注意:服务端不回复，客户端直接改
	void changeMemName( const char* szRoleName, const char* szMemName );
	// 删除关系请求
	// nType对应CLIENT_RELATION_TYPE_FRIEND...
	void delRelation( int nType, int nRoleHightId, int nRoleLowId );
	// 查看玩家装备
	void seeAvatarReq( const char* szName );
	void sortFriendInfo( int nRelationType );

	void RequestWatchFriend( const char* szName );

	//对玩家表白的回应
	void ActToPursue( const char* szPursueName, int nFlag );
	//对结婚誓言的回应
	void ActToMarryOath( const char* szName, int nFlag );
	//对解除关系花费的回应
	void ActToRemoveRelationCost( const char* szName);
	//tolua_end

public:
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	void	resetFriendList();
	//tolua_begin
	// 删除玩家
	void	deleteRole( int nRelationType, tdr_ulonglong roleWID );
	void	deleteRole( tdr_ulonglong roleWID );
	void	deleteRole( int nRelationType, const char* pszName );
	
	// 根据姓名获取好友信息
	tagFriendInfo* getFriendInfoByName( int nRelationType, const char* pszName );
	tagFriendInfo* getFriendInfoByName( const char* pszName );
	// 根据好友Id获取好友信息
	tagFriendInfo* getFriendInfoByRoleId( int nRoleHightId, int nRoleLowId );
	tagFriendInfo* getFriendInfoByRoleId( tdr_ulonglong roleWID );
	// 获取关系玩家数量
	int getRelationPlayerCount( int nType );
	// 根据index获取好友信息
	tagFriendInfo* getFriendInfoByIndex( int nType, int nIndex );
	// 获取玩家签名
	const char* getEmotion() const;
	
	// 查询玩家关系
	bool	isPlayerRelation( int nRelationType, const char* pszName );
	// 查询玩家关系
	bool	isPlayerRelation( int nRelationType, int nHeightId, int nLowId );
	// 查询该玩家是否是指定关系
	int		getPlayerRelation( const char* pszName );
	// 查询当前请求的玩家
	tagCSBuddyInfoRes_ToLua&	getCurRequireRoleInfo();
	//tolua_end

	void	onLeavePWorld( int nMapid );
	
	//判断是否在战场黑名单中
	bool	isInBattleBlack(const char* szName);
	void changeFriendName( const char* pszOldName, const char* pszNewName );
	// 更新公会信息
	void	updateClanInfo( tdr_ulonglong roleWID, const RELATIONCLAN& clanInfo );
	//查询玩家的在线状态
	int		getPlayerOnlineTypeByName( std::string szName);
	void	setOnlinePlayer(std::string szName,int OnlineType);
	std::map<std::string,int>		m_OnlinePlayer;	
private:
	// 设置对象属性
	void setRoleAttr( tagFriendInfo& info, int nType, const CSRELATIONATTRDATA* pdata );
	// 对象上下线通知
	void roleOnLineInfo( int nOnline, const char* pszName );

	void OnRelationTypeChange( const CSRELATIONCHG& relationChgInfo );
	void OnRelationTypeListChange( const CSRELATIONCHGLIST& relationChgList );
	void UpdateScenePlayer( const char* pszName );

	//结婚信息处理
	void HandleMarryInfo( const CSINVOLVEMARRYSVR& InvolveMarrySvr );
		
private:
	FRI_MAP		m_FriendMap;
	char		m_szEmotion[EMOTION_LEN];
	tagLong_ToLua m_increaseWid;
	tagCSBuddyInfoRes_ToLua	m_CurRequireRoleInfo;	
	int			m_nMaxFriendNum;
};//tolua_export
