#pragma once
#include "OgreSingleton.h"
#include "GameEngineTypes.h"
#include "GameMsgPrint.h"
#include "NetMessage.h"
#include "cs_net.h"
#include "GameDefineTolua.h"
#include <map>

// 契约状态
//tolua_begin
enum 
{
	CLIENT_QIYUE_STATE_INVALID  = 0, 	/* 无效 */
	CLIENT_QIYUE_STATE_VALID	= 1, 	/* 有效未选中 */
	CLIENT_QIYUE_STATE_SELECTED = 2, 	/* 有效被选中 */
};

enum
{
	CLIENT_NO_TEAM	= 0,
	CLIENT_TEAM_MEMBER,
	CLIENT_TEAM_CAPTAIN,
};

enum
{
	CLIENT_JOIN_TEAM	= 0,	// 加入队伍
	CLIENT_LEAVE_TEAM	= 1,	// 离开队伍
};

enum
{
	CLIENT_MEMBER_ONLINE = 0,		// 在线
};

enum tagTEAM_BOOTY_MODE_TOLUA 
{
	TEAM_BOOTY_ROLL_TOLUA = TEAM_BOOTY_ROLL, 	/* 蓝色品质及以上ROLL点 */
	TEAM_BOOTY_RAND_TOLUA = TEAM_BOOTY_RAND, 	/* 随机选择一个 */
	TEAM_BOOTY_ALL_TOLUA = TEAM_BOOTY_ALL, 	/* 所有物品，队伍成员都可以拾取 */
};

enum 
{
	TEAM_FOLLOW_CAPTAIN = 0, 	/* 队长请求组队跟随 */
	TEAM_FOLLOW_MEMBER = 1, 	/* 队员请求组队跟随 */
	TEAM_FOLLOW_CANCEL = 2, 	/* 取消组队跟随 */
};

struct tagQiYueLimit_ToLua
{
	unsigned int QiYueLimitID;                   
	unsigned int Val;                            
};
struct tagQiYueResultTarget_ToLua
{
	unsigned int QiYueResultTargetID;            
	int ResultTargetVal1;               
	int ResultTargetVal2;               
	int ResultTargetVal3;               
	int ResultTargetVal4;               
};

struct tagQiYueResult_ToLua
{
	unsigned int QiYueResultID;                  
	int ResultVal1;                     
	int ResultVal2;                     
	int ResultVal3;                     
	int ResultVal4;                     
	tagQiYueResultTarget_ToLua Target;                         
};

struct tagQiYueDef_ToLua
{
	unsigned int QiYueID;                        
	char Name[NAME_LEN];                 
	char TypeName[NAME_LEN];              	/*  Ver.140 契约类型的名字 */
	tagQiYueLimit_ToLua QiYueLimit[MAX_QIYUE_LIMIT];     	/*   契约条件限制 */
	tagQiYueResult_ToLua QiYueResults[MAX_QIYUE_RESULT];  	/*   契约奖励 */
	char QiYueDesc[SHORT_DESC_LEN];      
};

const tagQiYueDef_ToLua* getQiYueDef( int id );
const char* getQiYueDes( int id );
const char* getQiYueDescName( int id );
const char* getQiYueType( int id );
//tolua_end
struct tagPetDiathesis_ToLua;

class GameTeamManager//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GameTeamManager>, public IGameMsgPrint
{//tolua_export
public:
	GameTeamManager(void);
	~GameTeamManager(void);

public:

	//tolua_begin 
	// 是否能够申请入队
	bool canApplyTeam( const char* pszName );
	
	// 主动请求申请入队
	void Team_OP_Apply( const char* szName );
	// 是否能够被动接受申请入队
	bool canAcceptApplyTeam( int nHeightId, int nLowId );
	// 被动接受申请入队
	void Team_OP_ApplyAccept( int height, int low );
	// 拒绝所有申请入队
	void Team_OP_ApplyRefuseAll( );
	// 拒绝申请入队
	void Team_OP_ApplyRefuse( int height, int low );
	// 清空申请入队
	void Team_OP_ApplyClear();
	// 是否能够邀请入队
	bool canInviteTeam( const char* pszName );
	// 是否能够邀请入队
	bool canInviteTeam( int nHeightId, int nLowId );
	// 主动邀请入队
	void Team_OP_Invite( const char* szName );
	// 是否能够被动接受邀请入队
	bool canAcceptInviteTeam( const char* pszName );
	// 接受邀请入队
	void Team_OP_InviteAccept( const char* szName );
	// 拒绝邀请入队
	void Team_OP_InviteRefuse( const char* szName, bool bTeamOut = false );
	// 清空邀请入队
	void Team_OP_InviteClear();
	// 队长转让
	void Team_OP_CaptainChange( const char* szName );
	// 退出队伍
	void Team_OP_Exit();
	// 开除出队伍
	void Team_OP_Kick( const char* szName );
	// 解散队伍
	void Team_OP_Destroy();
	// 队伍改名
	void Team_OP_ChangeName( const char* szName );
	// 请求组队跟随
	void Team_OP_AppealTeamFollow();
	// 取消组队跟随
	void Team_OP_CancelTeamFollow();
	//接受队长传送
	void Team_OP_SummonRequest(bool bAgree);
	// 是否在进行组队跟随
	bool isInTeamFollow();
	// 组队，根据情况邀请或者申请
	void Team_OP_Add( const char* szName );
	
	// 刷新周围玩家
	void updateNearPlayer();
	// 周围玩家数量
	int  getNearPlayerCount();
	// 通过index获取周围玩家
	GameActor* getNearPlayer( int index );

	// 获取队伍信息
	const tagTeamInfo_ToLua* getTeamInfo();
	// 申请列表数量
	int getTeamApplyInfoSize();
	// 获取申请列表玩家信息
	tagTeamApplyInfo_ToLua& getTeamApplyInfoByIndex( int nIndex );
	// 获取申请列表玩家信息
	tagTeamApplyInfo_ToLua* getTeamApplyInfo( const char* pszName );
	// 组织模型装备显示数据
	void setAvatarAttrib( int i );
	
	// 获取组队的定义ID
	int getMapDefID( int nRealMapID );
	// 获取队伍当前契约
	int getTeamQiYue();
	// 获取当前成员所装备契约ID
	int getTeamMemberQiYue( int nIndex );
	// 获取成员契约状态
	int getTeamQiYueState( int nIndex );
	// 契约选择( 参数：角色名字 )
	void switchQiYue( const char* szTeamPlayerName );
	
	// 视野范围内玩家是否在队伍里
	bool isInTeamById( int id );
	// 查询某位玩家是否在队伍里
	bool isInTeam( const char* pszName );
	void updateActorStatus( GameActor* pActor );
	void updateActorRideStatus( GameActor* pActor );
	// 使用契约
	void useQiYueItem( unsigned int nQiYueID );
	// 删除契约
	void delQiYueItem( unsigned int nQiYueID );
	// 获取队员信息
	TEAMMEMBER* getTeamMemberInfo( GameActor* pActor );
	// 获取队伍成员信息
	tagTeamMember_ToLua* getTeamMemberInfo( const char* pszName );
	tagTeamMember_ToLua* getTeamMemberInfoById( int id );
	tagCSRoleStatus_ToLua* getOneBuf( const char* pszName, int nId, int nLv ); 

	void changeBootyMode( int nMode );

	// 投票T 人
	void requestVoteKick( bool bAgree );

	// 获取被投票的人的名字 
	const char* getVotePlayerName();
	
	//tolua_end
	
	// 更新视野范围内玩家的信息的同时更新队伍成员的数据
	void updateActorAttr( GameActor* pActor );
	// 宠物出战模式更改对队员的状态影响
	void updateActorPetStatus( GameActor* pActor, bool bPetFight = false );
	static bool isPetDiathesisEqual( const tagPetDiathesis_ToLua& lhs, const TEAMPETVIEW& rhs );
	
	void getTeamMemberPostion( float *postionlist, float *outeye );
	void showTeamer();
public:
	
	virtual int  HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	
	void releaseTeamInfo();
	void leaveTeam();
	void addTeamTmpFriend( const TEAMINFO& TeamInfo );
	bool isInHaveAddTeamTmpFriend( const char* pszName );
private:
	void delTeamApplyInfo( int height, int low );
	
private:
	std::vector<int>							m_NearPlayerId;			// 周围玩家ID列表
	TEAMINFO									m_TeamInfo;				// 队伍信息
	std::vector<tagTeamApplyInfo_ToLua>			m_vectTeamApplyInfo;	// 队伍申请列表
	std::vector<TEAMINFO>						m_vectTeamInviteInfo;	// 队伍邀请列表
	std::vector<std::string>					m_vecHaveAddTmpFriends;
	int											m_TeamSummonMemId;		//召唤人ID
};//tolua_export
