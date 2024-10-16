#include "GameTeamManager.h"
#include "OgreLog.h"
#include "cs_net.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "GameUIMsg.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "GameFriendManager.h"
#include "GameActorManager.h"
#include "GameQiYue.h"
#include "GameActor_PVP.h"
#include "ServerTimer.h"
#include "PworldDifficulty.h"
#include "GameAutoAssist.h"
#include "GameExchange.h"
#include "GameCollect.h"
#include "GameFollowManager.h"
#include "GameBattle.h"
#include "AutoFindLineManager.h"

const tagQiYueDef_ToLua* getQiYueDef( int id )
{
	const tagQiYueDef* pDef = &DBDataManager::getSingleton().getQiYueDef( id );
	return (tagQiYueDef_ToLua*)pDef;
}

const char* getQiYueDes( int id )
{
	return DBDataManager::getSingleton().getQiYueDef( id ).QiYueDesc;
}

const char* getQiYueDescName( int id )
{
	return DBDataManager::getSingleton().getQiYueDef( id ).Name;
}

const char* getQiYueType( int id )
{
	return DBDataManager::getSingleton().getQiYueDef( id ).TypeName;
}

GameTeamManager::GameTeamManager(void)
{
	releaseTeamInfo();
	INetworkManager::getSingleton().RegisterMsgHandler( TEAM_SVR, this );
	GameMsgPrint::getSingleton().registGameMsgHandler( this );
}

GameTeamManager::~GameTeamManager(void)
{
}

// 是否能够申请入队
bool GameTeamManager::canApplyTeam( const char* pszName )
{
	GameFriendManager& friendMgr = GameFriendManager::getSingleton();
	/*if ( friendMgr.isPlayerRelation( CLIENT_RELATION_TYPE_XIONG_SHOU, pszName ) )
	{
	GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_PLAYER_IS_ENEMY );
	return false;
	}*/

	if ( friendMgr.isPlayerRelation( CLIENT_RELATION_TYPE_SU_DI, pszName ) )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_PLAYER_IS_TMPENEMY );
		return false;
	}

	// 若为死亡状态，则不能接受申请
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer->isDead() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_SELF_DEAD );
		return false;
	}

	GameActor* pActor = ActorManager::getSingleton().findActorByName( pszName );
	if (  pActor != NULL && pActor->isDead() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_OTHER_DEAD );
		return false;
	}

	/*if ( GameFriendManager::getSingleton().isPlayerRelation( CLIENT_RELATION_TYPE_BLACK, pszName ) )
	{
	GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_OTHER_IN_BLACK );
	return false;
	}*/

	/*if ( GameFriendManager::getSingleton().isPlayerRelation( CLIENT_RELATION_TYPE_XIONG_SHOU, pszName ) )
	{
	GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_OTHER_IN_XIONGSHOU );
	return false;
	}*/

	if ( GameFriendManager::getSingleton().isPlayerRelation( CLIENT_RELATION_TYPE_SU_DI, pszName ) )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_OTHER_IN_ZHUISHA );
		return false;
	}

	return true;
}

// 主动请求申请入队
void GameTeamManager::Team_OP_Apply( const char* szName )
{
	if ( !canApplyTeam( szName ) )
	{
		return;
	}

	tagCSPkgBody msgbody;
	msgbody.TeamClt.OpType = TEAM_OP_APPLY;
	strncpy( msgbody.TeamClt.TeamCltData.TeamApply.CaptainName, szName, NAME_LEN );

	SendGameServerMsg( TEAM_CLT, msgbody );
}

// 是否能够申请入队
bool GameTeamManager::canAcceptApplyTeam( int nHeightId, int nLowId )
{
	GameFriendManager& friendMgr = GameFriendManager::getSingleton();
	/*if ( friendMgr.isPlayerRelation( CLIENT_RELATION_TYPE_XIONG_SHOU, nHeightId, nLowId ) )
	{
	GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_PLAYER_IS_ENEMY );
	return false;
	}*/

	// 若为死亡状态，则不能接受申请
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer->isDead() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_SELF_DEAD );
		return false;
	}

	/*if ( GameFriendManager::getSingleton().isPlayerRelation( CLIENT_RELATION_TYPE_BLACK, nHeightId, nLowId ) )
	{
	GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_OTHER_IN_BLACK );
	return false;
	}*/

	/*if ( pMainPlayer->isInTeam() )
	{
	GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_OTHER_IN_TEAM );
	return false;
	}*/

	return true;
}

// 接受申请入队
void GameTeamManager::Team_OP_ApplyAccept( int height, int low )
{
	if ( !canAcceptApplyTeam( height, low ) )
	{
		Team_OP_ApplyRefuse( height, low );
		return;
	}


	tagLong_ToLua LongId;
	LongId.nHeight	= height;
	LongId.nLow		= low;	
	tagCSPkgBody msgbody;
	msgbody.TeamClt.OpType = TEAM_OP_APPLY_ACCEPT;
	msgbody.TeamClt.TeamCltData.TeamApplyAccept.RoleID = *(tdr_ulonglong*)(&LongId);

	SendGameServerMsg( TEAM_CLT, msgbody );

	delTeamApplyInfo( height, low );
}

// 拒绝所有申请入队
void GameTeamManager::Team_OP_ApplyRefuseAll( )
{
	for ( std::vector<tagTeamApplyInfo_ToLua>::iterator iter = m_vectTeamApplyInfo.begin(), 
		end =m_vectTeamApplyInfo.end(); iter != end; ++iter )
	{
		const tagTeamApplyInfo_ToLua& applyInfo = *iter;
		tagLong_ToLua LongId;
		LongId.nHeight	= applyInfo.RoleID.nHeight;
		LongId.nLow		= applyInfo.RoleID.nLow;	
		tagCSPkgBody msgbody;
		msgbody.TeamClt.OpType = TEAM_OP_APPLY_REFUSE;
		msgbody.TeamClt.TeamCltData.TeamApplyRefuse.RoleID = *(tdr_ulonglong*)(&LongId);

		SendGameServerMsg( TEAM_CLT, msgbody );
	}
	m_vectTeamApplyInfo.clear();
}

// 拒绝申请入队
void GameTeamManager::Team_OP_ApplyRefuse( int height, int low )
{
	tagLong_ToLua LongId;
	LongId.nHeight	= height;
	LongId.nLow		= low;	
	tagCSPkgBody msgbody;
	msgbody.TeamClt.OpType = TEAM_OP_APPLY_REFUSE;
	msgbody.TeamClt.TeamCltData.TeamApplyRefuse.RoleID = *(tdr_ulonglong*)(&LongId);

	SendGameServerMsg( TEAM_CLT, msgbody );

	delTeamApplyInfo( height, low );
}

// 清空申请入队
void GameTeamManager::Team_OP_ApplyClear()
{
	tagCSPkgBody msgbody;
	msgbody.TeamClt.OpType = TEAM_OP_APPLY_CLEAR;
	msgbody.TeamClt.TeamCltData.TeamApplyClear = 0;

	SendGameServerMsg( TEAM_CLT, msgbody );

	m_vectTeamApplyInfo.clear();
}

// 是否能够邀请入队
bool GameTeamManager::canInviteTeam( const char* pszName )
{
	GameFriendManager& friendMgr = GameFriendManager::getSingleton();
	/*if ( friendMgr.isPlayerRelation( CLIENT_RELATION_TYPE_XIONG_SHOU, pszName ) )
	{
	GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_PLAYER_IS_ENEMY );
	return false;
	}*/

	if ( friendMgr.isPlayerRelation( CLIENT_RELATION_TYPE_SU_DI, pszName ) )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_INVITE_TEAM_PLAYER_IS_TMPENEMY );
		return false;
	}

	// 若为死亡状态，则不能接受申请
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer->isDead() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_SELF_DEAD );
		return false;
	}

	GameActor* pActor = ActorManager::getSingleton().findActorByName( pszName );
	if ( pActor != NULL && pActor->isDead() )
	{
		//GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_OTHER_DEAD );
		GameManager::getSingleton().SendUIEvent( "GE_INVITER_DEAD" );
		return false;
	}

	/*if ( GameFriendManager::getSingleton().isPlayerRelation( CLIENT_RELATION_TYPE_BLACK, pszName ) )
	{
	GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_OTHER_IN_BLACK );
	return false;
	}*/

	if ( GameFriendManager::getSingleton().isPlayerRelation( CLIENT_RELATION_TYPE_SU_DI, pszName ) )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_OTHER_IN_ZHUISHA );
		return false;
	}

	/*if ( GameFriendManager::getSingleton().isPlayerRelation( CLIENT_RELATION_TYPE_XIONG_SHOU, pszName ) )
	{
	GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_OTHER_IN_XIONGSHOU );
	return false;
	}*/

	// 由于有可能邀请不在视野范围内的玩家，故客户端无法判断
	return true;
}

// 是否能够邀请入队
bool GameTeamManager::canInviteTeam( int nHeightId, int nLowId )
{
	GameFriendManager& friendMgr = GameFriendManager::getSingleton();
	/*if ( friendMgr.isPlayerRelation( CLIENT_RELATION_TYPE_XIONG_SHOU, nHeightId, nLowId ) )
	{
	GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_PLAYER_IS_ENEMY );
	return false;
	}*/

	// 若为死亡状态，则不能接受申请
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer->isDead() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_SELF_DEAD );
		return false;
	}

	return true;
}

// 主动邀请入队
void GameTeamManager::Team_OP_Invite( const char* szName )
{
	if ( !canInviteTeam( szName ) )
	{
		return;
	}

	tagCSPkgBody msgbody;
	msgbody.TeamClt.OpType = TEAM_OP_INVITE;
	strncpy( msgbody.TeamClt.TeamCltData.TeamInvite.MemberName, szName, NAME_LEN );

	SendGameServerMsg( TEAM_CLT, msgbody );
}

// 是否能够被动接受邀请入队
bool GameTeamManager::canAcceptInviteTeam( const char* pszName )
{
	GameFriendManager& friendMgr = GameFriendManager::getSingleton();
	/*if ( friendMgr.isPlayerRelation( CLIENT_RELATION_TYPE_XIONG_SHOU, pszName ) )
	{
	GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_PLAYER_IS_ENEMY );
	return false;
	}*/

	// 若为死亡状态，则不能接受申请
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer->isDead() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_SELF_DEAD );
		return false;
	}

	GameActor* pActor = ActorManager::getSingleton().findActorByName( pszName );
	if ( pActor != NULL && pActor->isDead() )
	{
		//GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_OTHER_DEAD );
		GameManager::getSingleton().SendUIEvent( "GE_INVITER_DEAD" );
		return false;
	}

	/*if ( GameFriendManager::getSingleton().isPlayerRelation( CLIENT_RELATION_TYPE_BLACK, pszName ) )
	{
	GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_OTHER_IN_BLACK );
	return false;
	}*/

	if ( pMainPlayer->isInTeam() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_APPLY_TEAM_OTHER_IN_TEAM );
		return false;
	}

	// 由于有可能邀请不在视野范围内的玩家，故客户端无法判断
	return true;
}

// 接受邀请入队
void GameTeamManager::Team_OP_InviteAccept( const char* szName )
{
	if ( !canAcceptInviteTeam( szName ) )
	{
		Team_OP_InviteRefuse( szName );
		return;
	}

	tagCSPkgBody msgbody;
	msgbody.TeamClt.OpType = TEAM_OP_INVITE_ACCEPT;
	strncpy( msgbody.TeamClt.TeamCltData.TeamInviteAccept.CaptainName, szName, NAME_LEN );

	SendGameServerMsg( TEAM_CLT, msgbody );
}

// 拒绝邀请入队
void GameTeamManager::Team_OP_InviteRefuse( const char* szName, bool bTeamOut )
{
	tagCSPkgBody msgbody;
	memset( &msgbody, 0, sizeof(tagCSPkgBody) );
	msgbody.TeamClt.OpType = TEAM_OP_INVITE_REFUSE;
	strncpy( msgbody.TeamClt.TeamCltData.TeamInviteRefuse.CaptainName, szName, NAME_LEN );
	if( bTeamOut )
	{
		msgbody.TeamClt.TeamCltData.TeamInviteRefuse.Reason = 1;
	}

	SendGameServerMsg( TEAM_CLT, msgbody );
	for( std::vector<TEAMINFO>::iterator iter = m_vectTeamInviteInfo.begin(); iter != m_vectTeamInviteInfo.end(); ++iter )
	{
		const TEAMINFO& teamInfo = *iter;
		for( int i = 0; i < teamInfo.MemberNum; i++ )
		{
			if( CompareRoleName( szName, teamInfo.TeamMember[i].MemberName ) )
			{
				m_vectTeamInviteInfo.erase( iter );
				return;
			}
		}
	}
}

// 组队，根据情况邀请或者申请
void GameTeamManager::Team_OP_Add( const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.TeamClt.OpType = TEAM_OP_ADD;
	strncpy( msgbody.TeamClt.TeamCltData.TeamAdd.MemberName, szName, NAME_LEN );

	SendGameServerMsg( TEAM_CLT, msgbody );
}

// 清空邀请入队
void GameTeamManager::Team_OP_InviteClear()
{
	tagCSPkgBody msgbody;
	msgbody.TeamClt.OpType = TEAM_OP_INVITE_CLEAR;
	msgbody.TeamClt.TeamCltData.TeamInviteClear = 0;

	SendGameServerMsg( TEAM_CLT, msgbody );
}

// 队长转让
void GameTeamManager::Team_OP_CaptainChange( const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.TeamClt.OpType = TEAM_OP_CAPTAIN_CHG;
	strncpy( msgbody.TeamClt.TeamCltData.TeamCaptainChg.MemberName, szName, NAME_LEN );

	SendGameServerMsg( TEAM_CLT, msgbody );
}

// 退出队伍
void GameTeamManager::Team_OP_Exit()
{
	tagCSPkgBody msgbody;
	msgbody.TeamClt.OpType = TEAM_OP_EXIT;
	msgbody.TeamClt.TeamCltData.TeamExit.Res = 0;
	GameManager::getSingleton().getScriptVM()->callString( "CancelTeamFollowState()" );
	SendGameServerMsg( TEAM_CLT, msgbody );
}

// 开除出队伍
void GameTeamManager::Team_OP_Kick( const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.TeamClt.OpType = TEAM_OP_KICK;
	strncpy( msgbody.TeamClt.TeamCltData.TeamKick.MemberName, szName, NAME_LEN );

	SendGameServerMsg( TEAM_CLT, msgbody );
}

// 解散队伍
void GameTeamManager::Team_OP_Destroy()
{
	tagCSPkgBody msgbody;
	msgbody.TeamClt.OpType = TEAM_OP_DESTROY;
	msgbody.TeamClt.TeamCltData.TeamDestroy = 0;

	SendGameServerMsg( TEAM_CLT, msgbody );
}

// 队伍改名
void GameTeamManager::Team_OP_ChangeName( const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.TeamClt.OpType = TEAM_OP_CHGNAME;
	strncpy( msgbody.TeamClt.TeamCltData.TeamChgName.TeamName, szName, NAME_LEN );

	SendGameServerMsg( TEAM_CLT, msgbody );
}

//请求组队跟随
void GameTeamManager::Team_OP_AppealTeamFollow()
{
	tagCSPkgBody msgbody;
	msgbody.TeamClt.OpType = TEAM_OP_FOLLOW;
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer == NULL || !pMainPlayer->isInTeam() )
	{
		return;
	}
	if ( pMainPlayer->isCaptain() )
	{
		msgbody.TeamClt.TeamCltData.TeamFollow.Flag = TEAM_FOLLOW_CAPTAIN;
	}
	else
	{
		msgbody.TeamClt.TeamCltData.TeamFollow.Flag = TEAM_FOLLOW_MEMBER;
	}

	SendGameServerMsg( TEAM_CLT, msgbody );
}

//取消组队跟随
void GameTeamManager::Team_OP_CancelTeamFollow()
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer == NULL || !pMainPlayer->isInTeam())
	{		
		return;
	}
	tagCSPkgBody msgbody;
	msgbody.TeamClt.OpType = TEAM_OP_FOLLOW;
	msgbody.TeamClt.TeamCltData.TeamFollow.Flag = TEAM_FOLLOW_CANCEL;
	GameManager::getSingleton().getScriptVM()->callString( "CancelTeamFollow()" );
	SendGameServerMsg( TEAM_CLT, msgbody );	
}

// 刷新周围玩家
void GameTeamManager::updateNearPlayer()
{
	m_NearPlayerId.clear();
	ACTOR_ITER iter;
	for( iter  = ActorManager::getSingleton().getActorBegin(); 
		iter != ActorManager::getSingleton().getActorEnd(); ++iter )
	{
		if( iter->second->getType() == GAT_PLAYER )
		{
			m_NearPlayerId.push_back( iter->second->GetID() );
		}
	}
}

// 周围玩家数量
int GameTeamManager::getNearPlayerCount()
{
	return (int)m_NearPlayerId.size();
}

// 通过index获取周围玩家
GameActor* GameTeamManager::getNearPlayer( int index )
{
	assert( index < (int)m_NearPlayerId.size() );
	return ActorManager::getSingleton().FindActor( m_NearPlayerId[index] );
}

// 获取队伍信息
const tagTeamInfo_ToLua* GameTeamManager::getTeamInfo()
{
	static_assert( sizeof( tagTeamInfo_ToLua ) == sizeof( tagTeamInfo ) );	
	return (const tagTeamInfo_ToLua*)&m_TeamInfo;
}

// 申请列表数量
int GameTeamManager::getTeamApplyInfoSize()
{
	return (int)m_vectTeamApplyInfo.size();
}

// 获取申请列表玩家信息
tagTeamApplyInfo_ToLua& GameTeamManager::getTeamApplyInfoByIndex( int nIndex )
{
	assert( nIndex < (int)m_vectTeamApplyInfo.size() );
	return m_vectTeamApplyInfo[nIndex];
}

// 获取申请列表玩家信息
tagTeamApplyInfo_ToLua* GameTeamManager::getTeamApplyInfo( const char* pszName )
{
	for ( std::vector<tagTeamApplyInfo_ToLua>::iterator iter = m_vectTeamApplyInfo.begin(), 
		end =m_vectTeamApplyInfo.end(); iter != end; ++iter )
	{
		if ( CompareRoleName( iter->RoleName, pszName ) )
		{
			return &( *iter );
		}
	}

	return NULL;
}

void GameTeamManager::setAvatarAttrib(int i)
{
	assert( 0 <= i && i < m_TeamInfo.MemberNum );

	clearEquipIdList();

	const TEAMMEMBER& teamMember = m_TeamInfo.TeamMember[i];
	for( int i = 0; i <teamMember.Avatar.Num; i++ )
	{
		setArmEquipIdList( teamMember.Avatar.Arms[i].GridIdx, teamMember.Avatar.Arms[i].ArmDefID, teamMember.Avatar.Arms[i].StarLv );
	}
	setAvatarSelectSuit( teamMember.Avatar.SelectFashion );
	g_AvatarChange(); 
	::setAvatarAttrib(0, teamMember.Career);
	::setAvatarAttrib(1, teamMember.Gender-1 );
	::setAvatarAttrib(POS_ID_HAIR,teamMember.Hair + 1);
	if( teamMember.MapID == BEACH_MAP_ID )
	{
		::setAvatarAttrib( POS_ID_FASHION_HAIR,1 );
	}
	g_ConfigFashionFair();
}

// 获取组队的定义ID
int GameTeamManager::getMapDefID( int nRealMapID )
{
	return nRealMapID & 0xffff;
}

// 获取队伍当前契约
int GameTeamManager::getTeamQiYue()
{
	return m_TeamInfo.QiYue;
}	

// 获取当前成员所装备契约
int GameTeamManager::getTeamMemberQiYue( int nIndex )
{
	assert( 0 <= nIndex && nIndex < m_TeamInfo.MemberNum );
	return m_TeamInfo.TeamMember[nIndex].Avatar.QiYue;
}

// 获取成员契约状态
int GameTeamManager::getTeamQiYueState( int nIndex )
{
	assert( 0 <= nIndex && nIndex < m_TeamInfo.MemberNum );
	return m_TeamInfo.TeamMember[nIndex].QiYueValid;
}

// 契约选择( 参数：角色名字 )
void GameTeamManager::switchQiYue( const char* szTeamPlayerName )
{
	tagCSPkgBody msgbody;
	msgbody.TeamClt.OpType = TEAM_OP_SWITCH_QIYUE;
	strncpy( msgbody.TeamClt.TeamCltData.TeamSwitchQiYueName , szTeamPlayerName, NAME_LEN );

	SendGameServerMsg( TEAM_CLT, msgbody );
}

// 是否在队伍里
bool GameTeamManager::isInTeamById( int id )
{
	for( int i = 0 ; i < m_TeamInfo.MemberNum; i++ )
	{
		return m_TeamInfo.TeamMember[i].MemID == id;
	}
	return false;
}

// 查询某位玩家是否在队伍里
bool GameTeamManager::isInTeam( const char* pszName )
{
	for ( int i = 0; i < m_TeamInfo.MemberNum; i++ )
	{
		if ( CompareRoleName( m_TeamInfo.TeamMember[i].MemberName, pszName ) )
		{
			return true;
		}
	}
	return false;
}

// 更新队伍成员的状态
void GameTeamManager::updateActorStatus( GameActor* pActor )
{
	for ( int i = 0; i < m_TeamInfo.MemberNum; i++ )
	{
		TEAMMEMBER& member = m_TeamInfo.TeamMember[i];
		if ( !CompareRoleName( m_TeamInfo.TeamMember[i].MemberName, pActor->getName() ) )
		{
			continue;
		}

		CSSTATUSINFO& statusInfo	= member.StatusInfo;
		GameBufStatus* pActorStatus = pActor->getBufStatus();
		statusInfo.GoodStatusNum = pActorStatus->getGoodStatusCount();
		for ( int i = 0, size = pActorStatus->getGoodStatusCount(); i < size; ++i )
		{
			CSROLESTATUS& goodStatus	= statusInfo.GoodStatusList[i];
			GameBuf* pActorBuf			= pActorStatus->getGoodStatus( i );
			goodStatus.StatusID = pActorBuf->getBufId();
			goodStatus.Level	= pActorBuf->getBufLv();
			goodStatus.Overlay	= pActorBuf->getOverlay();
			goodStatus.Time		= unsigned int(pActorBuf->getLife());
		}

		statusInfo.BadStatusNum = pActorStatus->getBadStatusCount();
		for ( int i = 0, size = pActorStatus->getBadStatusCount(); i < size; ++i )
		{
			CSROLESTATUS& badStatus	= statusInfo.BadStatusList[i];
			GameBuf* pActorBuf			= pActorStatus->getBadStatus( i );
			badStatus.StatusID	= pActorBuf->getBufId();
			badStatus.Level		= pActorBuf->getBufLv();
			badStatus.Overlay	= pActorBuf->getOverlay();
			badStatus.Time		= unsigned int(pActorBuf->getLife());
		}

		statusInfo.NoLimitStatusNum = pActorStatus->getNolimitStatusCount();
		for ( int i = 0, size = pActorStatus->getNolimitStatusCount(); i < size; ++i )
		{
			CSROLESTATUS& badStatus	= statusInfo.BadStatusList[i];
			GameBuf* pActorBuf			= pActorStatus->getNolimitStatus( i );
			badStatus.StatusID	= pActorBuf->getBufId();
			badStatus.Level		= pActorBuf->getBufLv();
			badStatus.Overlay	= pActorBuf->getOverlay();
			badStatus.Time		= unsigned int(pActorBuf->getLife());
		}
	}
}

void GameTeamManager::updateActorRideStatus( GameActor* pActor )
{
	for ( int i = 0; i < m_TeamInfo.MemberNum; i++ )
	{
		TEAMMEMBER& member = m_TeamInfo.TeamMember[i];
		if ( !CompareRoleName( m_TeamInfo.TeamMember[i].MemberName, pActor->getName() ) )
		{
			continue;
		}
		Rider* pRider = pActor->getRider();
		if ( pRider->isOnHorse() )
		{
			const tagRideInfo& riderInfo = pRider->getRideItem();
			member.RideID		= riderInfo.RideID;
			member.RideLevel	= riderInfo.nLevel;
		}
		else
		{
			member.RideID		= 0;
			member.RideLevel	= 0;
		}
	}
}

// 使用契约
void GameTeamManager::useQiYueItem( unsigned int nQiYueID )
{
	tagCSPkgBody msgbody;
	msgbody.TeamClt.OpType = TEAM_OP_USER_SET_QIYUE;
	msgbody.TeamClt.TeamCltData.TeamUserSetQieYue = nQiYueID;

	SendGameServerMsg( TEAM_CLT, msgbody );
}

// 删除契约
void GameTeamManager::delQiYueItem( unsigned int nQiYueID )
{
	tagCSPkgBody msgbody;
	msgbody.TeamClt.OpType = TEAM_OP_USER_DEL_QIYUE;
	msgbody.TeamClt.TeamCltData.TeamUserDelQieYue = nQiYueID;

	SendGameServerMsg( TEAM_CLT, msgbody );
}
// 获取队员信息
TEAMMEMBER* GameTeamManager::getTeamMemberInfo( GameActor* pActor )
{
	static_assert( sizeof( TEAMMEMBER ) == sizeof( tagTeamMember_ToLua ) );
	return (TEAMMEMBER*)getTeamMemberInfo( pActor->getName() );
}

// 获取队伍成员信息
tagTeamMember_ToLua* GameTeamManager::getTeamMemberInfo( const char* pszName )
{
	for ( int i = 0; i < m_TeamInfo.MemberNum; ++i )
	{
		TEAMMEMBER& teamMember = m_TeamInfo.TeamMember[i];
		if ( !CompareRoleName( teamMember.MemberName, pszName ) )
		{
			continue;
		}

		return (tagTeamMember_ToLua*)&teamMember;
	}

	return NULL;
}

tagTeamMember_ToLua* GameTeamManager::getTeamMemberInfoById( int id )
{
	for ( int i = 0; i < m_TeamInfo.MemberNum; ++i )
	{
		TEAMMEMBER& teamMember = m_TeamInfo.TeamMember[i];
		if ( teamMember.MemID == id )
		{
			return (tagTeamMember_ToLua*)&teamMember;
		}
	}
	return NULL;
}

tagCSRoleStatus_ToLua* GameTeamManager::getOneBuf( const char* pszName, int nId, int nLv )
{
	tagTeamMember_ToLua* pMember = getTeamMemberInfo( pszName );
	/*const tagStatusDef_ToLua* pStatusDef = getStatusDef( nId, nLv );
	if ( pStatusDef == NULL )
	{
	return NULL;
	}*/

	tagCSStatusInfo_ToLua& oneMemberAllStatus = pMember->StatusInfo;
	for ( int i = 0; i < oneMemberAllStatus.GoodStatusNum; ++i )
	{
		tagCSRoleStatus_ToLua& oneStatus = oneMemberAllStatus.GoodStatusList[i];
		if ( oneStatus.StatusID == nId && oneStatus.Level == nLv )
		{
			return &oneStatus;
		}
	}

	for ( int i = 0; i < oneMemberAllStatus.BadStatusNum; ++i )
	{
		tagCSRoleStatus_ToLua& oneStatus = oneMemberAllStatus.BadStatusList[i];
		if ( oneStatus.StatusID == nId && oneStatus.Level == nLv )
		{
			return &oneStatus;
		}
	}

	for ( int i = 0; i < oneMemberAllStatus.NoLimitStatusNum; ++i )
	{
		tagCSRoleStatus_ToLua& oneStatus = oneMemberAllStatus.NolimitStatusList[i];
		if ( oneStatus.StatusID == nId && oneStatus.Level == nLv )
		{
			return &oneStatus;
		}
	}

	return NULL;
}

// 更新视野范围内玩家的信息的同时更新队伍成员的数据
void GameTeamManager::updateActorAttr( GameActor* pActor )
{
	assert( pActor != NULL );
	if ( pActor == NULL || ( pActor->getType() != GAT_PLAYER && pActor->getType() != GAT_MAINPLAYER )  )
	{
		return;
	}

	const char* pszName = pActor->getName();

	for ( int i = 0; i < m_TeamInfo.MemberNum; ++i )
	{
		TEAMMEMBER& member = m_TeamInfo.TeamMember[i];
		if ( CompareRoleName( member.MemberName, pszName ) )
		{
			member.HP		= pActor->getHP();
			member.MaxHP	= pActor->getMaxHP();
			member.MP		= pActor->getMP();
			member.MaxMP	= pActor->getMaxMP();
			GameManager::getSingleton().SendUIEvent( "GE_TEAM_GET_TEAMINFO" );
			break;
		}
	}
}

// 宠物出战模式更改对队员的状态影响
void GameTeamManager::updateActorPetStatus( GameActor* pActor, bool bPetFight )
{
	TEAMMEMBER* pTeamMember = getTeamMemberInfo( pActor );
	if ( pTeamMember == NULL )
	{
		return;
	}

	bool bNeedUpdate[PET_MAX_FIGHT_SHI] = { false };
	// 若是出征宠物
	if ( bPetFight )
	{
		// 先判定是否需要进行更新操作
		int nPetCount = 0;
		GamePet* pPetPackage = pActor->getPet();
		for ( int i = 0; i < PET_MAX_FIGHT_SHI; ++i )
		{
			GameActor* pPetActor = pPetPackage->getFightPet( i );
			if ( pPetActor == NULL )
			{
				continue;
			}

			const tagPetDiathesis_ToLua& newPetDiathesisData = pPetActor->getPetDiathesis();
			TEAMPETVIEW& memberPetData = pTeamMember->PetView.PetView[nPetCount];
			// 检测是否要更新数据，若不需要则不更新，以避免不必要的开销
			bNeedUpdate[i] = !isPetDiathesisEqual( newPetDiathesisData, memberPetData );
			if ( !bNeedUpdate[i] )
			{
				continue;
			}

			for ( int j = 0; j < newPetDiathesisData.m_nNum; ++j )
			{
				CSPETDIATHESISVIEW& oldMemberData = memberPetData.Diathesis[j];
				const tagPetDiathesisData_ToTua& newMemberData = newPetDiathesisData.m_diathesisData[j];

				oldMemberData.ID		= newMemberData.m_nID;
				oldMemberData.Level		= newMemberData.m_nLevel;
			}
			memberPetData.DiathesisNum	= newPetDiathesisData.m_nNum;
			memberPetData.Passive		= newPetDiathesisData.m_nFightMode;
			++nPetCount;
		}

		pTeamMember->PetView.PetNum = nPetCount;
	}
	else
	{
		// 若是收回宠物
		// 查看之前是否就是0个了
		// TODO: 目前只能处理1个宠物，若出征多只宠物要更改逻辑
		for ( int i = 0; i < pTeamMember->PetView.PetNum; ++i )
		{
			const TEAMPETVIEW& petViewData = pTeamMember->PetView.PetView[i];
			if ( petViewData.DiathesisNum > 0 )
			{
				bNeedUpdate[i] = true;
				break;
			}
		}

		memset( &( pTeamMember->PetView ), 0, sizeof( pTeamMember->PetView ) );
	}

	// 只要部分变化，就全局更新，无需多次调用脚本，造成不必要的开销
	for ( int i = 0; i < ARRAY_ELEMENTS( bNeedUpdate ) ; ++i )
	{
		if ( bNeedUpdate[i] )
		{
			// TODO: 这最好精确到只更新某个成员，要不然会有效率问题
			GameManager::getSingleton().SendUIEvent( "GE_TEAM_GET_TEAMINFO" );
			break;
		}
	}
}

bool GameTeamManager::isPetDiathesisEqual( const tagPetDiathesis_ToLua& lhs, const TEAMPETVIEW& rhs )
{
	if ( lhs.m_nNum != rhs.DiathesisNum || lhs.m_nFightMode != rhs.Passive )
	{
		return false;
	}

	for ( int i = 0; i < lhs.m_nNum; ++i )
	{
		bool bFound = false;
		for ( int j = 0; j < rhs.DiathesisNum; ++j )
		{
			const CSPETDIATHESISVIEW& rhsData					= rhs.Diathesis[j];
			const tagPetDiathesisData_ToTua& lhsData	= lhs.m_diathesisData[i];
			if ( rhsData.ID == lhsData.m_nID )
			{
				bFound = true;
				break;
			}
		}

		if ( !bFound )
		{
			return false;
		}
	}

	return true;
}

int GameTeamManager::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{
	if( msgid == TEAM_SVR)
	{
		const CSTEAMSVR& TeamSvr = msgbody.TeamSvr;
		switch( TeamSvr.OpType )
		{
		case TEAM_USER_QIYUELIST:	// 用于更新队伍契约列表
			{
				MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				if ( pMainPlayer != NULL )
				{
					pMainPlayer->m_QiYue.setQiYueList( TeamSvr.TeamSvrData.UserQiYueList );
					GameManager::getSingleton().SendUIEvent( "GE_GET_QIYUE_INFO" );
				}
			}
			break;

		case TEAM_SVR_TEAMINFO:// 队伍列表信息
			{
				const TEAMINFO& TeamInfo = TeamSvr.TeamSvrData.TeamInfo;				
				MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				if ( pMainPlayer == NULL )
				{
					return 0;
				}				

				const TEAMMEMBER* member = NULL;
				for ( int i = 0; i < TeamInfo.MemberNum; ++i )
				{
					if ( CompareRoleName( TeamInfo.TeamMember[i].MemberName, pMainPlayer->getName() ) )
					{
						member = &TeamInfo.TeamMember[i];
						break;
					}
				}
				bool bNeedShowVoteFrame = false;
				if( member && TeamInfo.KickMemberUin != 0 && member->Uin != TeamInfo.KickMemberUin && member->VoteFlag == 0 )
				{
					if( m_TeamInfo.KickVoteStartTime == 0 && TeamInfo.KickVoteStartTime != 0 )
					{
						bNeedShowVoteFrame = true;
					}					
				}else{
					GameManager::getSingleton().SendUIEvent("GE_HIDE_VOTE_FRAME");
				}

				bool bMemberNumChg = false;
				if ( m_TeamInfo.TeamID != 0 )
				{					
					if ( m_TeamInfo.MemberNum > 0 && TeamInfo.MemberNum > 0 && m_TeamInfo.MemberNum != TeamInfo.MemberNum )
					{
						bMemberNumChg = true;						
					}

					if ( member && member->Follow != 0 && strcmp( m_TeamInfo.TeamName, TeamInfo.TeamName ) != 0 )
					{
						GameManager::getSingleton().getScriptVM()->callString( "TroopInviteTeamFollowFrameRufuseButton_OnClick()" );
						Team_OP_CancelTeamFollow();
					}

					if ( !pMainPlayer->isCaptain() )
					{
						const TEAMMEMBER* tmpMember = NULL;
						for ( int i = 0; i < m_TeamInfo.MemberNum; ++i )
						{
							if ( CompareRoleName( m_TeamInfo.TeamMember[i].MemberName, pMainPlayer->getName() ) )
							{
								tmpMember = &m_TeamInfo.TeamMember[i];
								break;
							}
						}

						if ( member && tmpMember && tmpMember->Follow != 0 && member->Follow == 0 )
						{
							GameManager::getSingleton().getScriptVM()->callString( "CancelTeamFollow()" );
						}
					}					
				}

				m_TeamInfo = TeamInfo;
				for ( int i = 0; i < m_TeamInfo.MemberNum; ++i )
				{
					const int pworldId = m_TeamInfo.TeamMember[i].BattlePworldID;
					if ( pworldId != NULL && pworldId > 0 )
					{
						int referMapId = PworldDifficulty::getSingleton().getPworldReferMapId( pworldId );
						m_TeamInfo.TeamMember[i].MapID = referMapId;
					}
				}
				if( bNeedShowVoteFrame )
				{
					GameManager::getSingleton().SendUIEvent("GE_SHOW_VOTE_FRAME");
				}
				addTeamTmpFriend( TeamInfo );		

				pMainPlayer->setTeamWID( m_TeamInfo.TeamID );
				GameActor_PVP::getSingleton().resetTeamMember();
				
				for ( int i = 0; i < m_TeamInfo.MemberNum; i++ )
				{
					const TEAMMEMBER& member = m_TeamInfo.TeamMember[i];
					if ( CompareRoleName( member.MemberName, pMainPlayer->getName() ) )
					{
						pMainPlayer->setTeamName( m_TeamInfo.TeamName );
						pMainPlayer->setCaptain( member.Position==0?false:true );
						if (pMainPlayer->isCaptain())
						{
							pMainPlayer->m_ViewFlag |= VIEW_FLAG_CAPTAIN;
							//pMainPlayer->setViewFlg( VIEW_FLAG_CAPTAIN );
						}
						else
						{
							pMainPlayer->m_ViewFlag &= ~VIEW_FLAG_CAPTAIN;
						}
					}
					GameActor* pActor = ActorManager::getSingleton().FindActor( member.MemID );
					if( pActor != NULL )
					{
						if( (pActor->getTeamWID() != m_TeamInfo.TeamID) 
							|| (pActor->isCaptain() != (member.Position == TEAM_POSITION_CAPTAIN)) )
						{
							pActor->setTeamWID( m_TeamInfo.TeamID );
							pActor->setCaptain( member.Position == TEAM_POSITION_CAPTAIN );
							pActor->BuildTitle();
						}
						pActor->setRoleWId( member.RoleID );
					}
					GameActor_PVP::getSingleton().setTeamMember(member.MemID, member.MemberName, member.RoleID);
				}

				if ( pMainPlayer->isCaptain() )
				{
					GameManager::getSingleton().getScriptVM()->callString( "CalTeamMemberDistance()" );
				}

				GameManager::getSingleton().SendUIEvent( "GE_TEAM_GET_TEAMINFO" );

				if ( bMemberNumChg )
				{
					GameManager::getSingleton().SendUIEvent("GE_TEAM_MEMBER_NUM_CHG");
				}
			}
			break;

		case TEAM_SVR_APPLY:// 队伍申请转发，收到他人申请入队请求
			{
				const TEAMAPPLYINFO& svrApplyInfo = TeamSvr.TeamSvrData.ApplyInfo;
				if ( getTeamApplyInfo( svrApplyInfo.RoleName ) == NULL )
				{
					tagTeamApplyInfo_ToLua applyInfo;
					applyInfo.RoleID = *((tagLong_ToLua*)&svrApplyInfo.RoleID);
					applyInfo.Career = svrApplyInfo.Career;
					applyInfo.Gender = svrApplyInfo.Gender;
					applyInfo.Level	 = svrApplyInfo.Level;
					strncpy( applyInfo.ClanName, svrApplyInfo.ClanName, NAME_LEN );
					strncpy( applyInfo.RoleName, svrApplyInfo.RoleName, NAME_LEN );
					applyInfo.startTime = timeGetTime();
					m_vectTeamApplyInfo.push_back( applyInfo );
					
					GameManager::getSingleton().SendUIEvent( "GE_TEAM_GET_APPLY" );
					if ( !(AutoAssist::getSingleton().getAutoAssistBegin() && AutoAssist::getSingleton().getAutoAssistOpt()->m_bChkBtn[7]))
					{
						GameManager::getSingleton().FlashWindow();
					}
				}			
			}
			break;

		case TEAM_SVR_INVITE:// 队伍邀请转发，收到他人入队邀请
			{
				const TEAMINFO& teamInviteInfo = TeamSvr.TeamSvrData.InviteInfo;
				SystemSettingConfig& systemConfig = GameManager::getSingleton().getUserSystemConfig();
				if ( systemConfig.getGameOpt( GAME_OPT_REFUSE_TEAM_INVITE ) )
				{
					// 若设置拒绝组队邀请，直接回复拒绝
					for( int i = 0; i < TeamSvr.TeamSvrData.InviteInfo.MemberNum; i++ )
					{
						if( teamInviteInfo.TeamMember[i].Position == TEAM_POSITION_CAPTAIN )
						{
							Team_OP_InviteRefuse( teamInviteInfo.TeamMember[i].MemberName );
						}
					}

				}
				else if(AutoAssist::getSingleton().m_assistData.m_bChkBtn[7] == true &&
					AutoAssist::getSingleton().getAutoAssistBegin() &&
					!systemConfig.getGameOpt( GAME_OPT_REFUSE_TEAM_INVITE ) )
				{
					// 若自动辅助开启接受组队邀请，系统设置没有设置拒绝组队邀请
					for( int i = 0; i < TeamSvr.TeamSvrData.InviteInfo.MemberNum; i++ )
					{
						if( teamInviteInfo.TeamMember[i].Position == TEAM_POSITION_CAPTAIN )
						{
							Team_OP_InviteAccept(teamInviteInfo.TeamMember[i].MemberName );
						}
					}
				}
				else
				{
					m_vectTeamInviteInfo.push_back( TeamSvr.TeamSvrData.InviteInfo );

					for( int i = 0; i < TeamSvr.TeamSvrData.InviteInfo.MemberNum; i++ )
					{
						if( teamInviteInfo.TeamMember[i].Position == TEAM_POSITION_CAPTAIN )
						{
							char szScript[256];
							sprintf( szScript, "TeamInviteShow(\"%s\")", teamInviteInfo.TeamMember[i].MemberName );
							GameManager::getSingleton().getScriptVM()->callString( szScript );
							GameManager::getSingleton().FlashWindow();
						}
					}
				}
			}
			break;

		case TEAM_SVR_MSG:// 队伍提示
			{
				tagUIMsgBody UI_Msg;
				strncpy( UI_Msg.SysMsg.Msg, TeamSvr.TeamSvrData.TeamMsg.Msg, CLT_MSG_LEN );
				UI_Msg.SysMsg.Type = SYS_SVR;
				GameUIMsg::getSingleton().setUIMsg( UI_Msg );
				GameManager::getSingleton().SendUIEvent( "GE_UPDATE_SYSMSG" );
			}
			break;
		case TEAM_SVR_BOOTY_MODE:
			{
				m_TeamInfo.TeamBootyMode = TeamSvr.TeamSvrData.BootyModeInfo.TeamBootyMode;
				GameManager::getSingleton().SendUIEvent("GE_TEAM_CHANGE_BOOTY");
			}
			break;
		case TEAM_SVR_FOLLOW://收到他人组队跟随邀请
			{	
				MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				if ( pMainPlayer == NULL )
				{
					break;
				}	

				const TEAMMEMBER* member = NULL;
				for ( int i = 0; i < m_TeamInfo.MemberNum; ++i )
				{
					if ( CompareRoleName( m_TeamInfo.TeamMember[i].MemberName, pMainPlayer->getName() ) )
					{
						member = &m_TeamInfo.TeamMember[i];
						break;
					}
				}
				if ( member->Follow > 0 )
				{
					break;
				}
				GameManager::getSingleton().getScriptVM()->callString( "TeamFollowAppeal()" );
				GameManager::getSingleton().FlashWindow();
			}
			break;
		case TEAM_SVR_FOLLOW_TICK://组队跟随中接收到队长坐标
			{
				MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				if ( pMainPlayer == NULL || pMainPlayer->isCaptain() || pMainPlayer->isDead() )
				{
					break;
				}
				
				const TEAMMEMBER* captainInfo = NULL;
				for ( int i = 0; i < m_TeamInfo.MemberNum; ++i )
				{
					if ( m_TeamInfo.TeamMember[i].Position == TEAM_POSITION_CAPTAIN )
					{
						captainInfo = &m_TeamInfo.TeamMember[i];
						break;
					}
				}

				if ( captainInfo == NULL )
				{
					break;
				}

				GameActor* pActor = ActorManager::getSingleton().findActorByWId(captainInfo->RoleID);// captainInfo->MemberName );
				const CSTEAMSVRDATA &followDest  = msgbody.TeamSvr.TeamSvrData;
				char buff[256];
				// 若被跟随的目标玩家处于停止状态，则将停止距离靠远点
				if ( pActor != NULL && pActor->getActState().GetState() == GSACT_STAND )
				{
					sprintf( buff, "TeamFollow( %d, %d, %d, %d, %d, %d )", followDest.TeamFollowTick.Map, captainInfo->MapID, 
						followDest.TeamFollowTick.X, followDest.TeamFollowTick.Y, followDest.TeamFollowTick.MapLine, 150 );
				}
				else
				{
					sprintf( buff, "TeamFollow( %d, %d, %d, %d, %d, %d )", followDest.TeamFollowTick.Map, captainInfo->MapID,  
						followDest.TeamFollowTick.X, followDest.TeamFollowTick.Y, followDest.TeamFollowTick.MapLine, 0 );
				}
				GameManager::getSingleton().getScriptVM()->callString( buff );
				GameFollowManager::getSingleton().drawFollowFrame();
			}
			break;
		case TEAM_SVR_SUMMON_ASK:
			{
				MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				if ( pMainPlayer == NULL || pMainPlayer->isCaptain() || pMainPlayer->isDead() )
				{
					break;
				}
				m_TeamSummonMemId	=	msgbody.TeamSvr.TeamSvrData.TeamSummonAsk.SummonMemID;
				int m_OverTime		=	msgbody.TeamSvr.TeamSvrData.TeamSummonAsk.OverTime;
				int m_MapId			=	msgbody.TeamSvr.TeamSvrData.TeamSummonAsk.Map;
				char buff[256];
				sprintf( buff, "TeamMemberSummon(%d,%d)",m_OverTime,m_MapId );
				GameManager::getSingleton().getScriptVM()->callString( buff );
			}
			break;
		case TEAM_SVR_SUMMON_CLOSE_UI:
			{
				MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				if ( pMainPlayer == NULL || pMainPlayer->isCaptain())
				{
					break;
				}
				char buff[256];
				sprintf( buff, "TeamMemberSummonFrameClose()");
				GameManager::getSingleton().getScriptVM()->callString( buff );
			}
			break;
		}
	}
	return 0;
}

void GameTeamManager::releaseTeamInfo()
{
	memset( &m_TeamInfo, 0, sizeof(TEAMINFO) );
	m_vectTeamApplyInfo.clear();
	m_vectTeamInviteInfo.clear();
	m_vecHaveAddTmpFriends.clear();
	GameActor_PVP::getSingleton().resetTeamMember();
}

bool GameTeamManager::isInTeamFollow()
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer == NULL )
	{
		return false;
	}
	if ( !pMainPlayer->isInTeam() )
	{
		return false;
	}
	if ( pMainPlayer->isCaptain() )
	{
		return false;
	}

	const TEAMMEMBER* member = NULL;
	for ( int i = 0; i < m_TeamInfo.MemberNum; ++i )
	{
		if ( CompareRoleName( m_TeamInfo.TeamMember[i].MemberName, pMainPlayer->getName() ) )
		{
			member = &m_TeamInfo.TeamMember[i];
			break;
		}
	}
	if ( member == NULL )
	{
		return false;
	}
	return member->Follow != 0;
}

void GameTeamManager::leaveTeam()
{
	memset( &m_TeamInfo, 0, sizeof(TEAMINFO) );
	GameActor_PVP::getSingleton().resetTeamMember();
	m_vecHaveAddTmpFriends.clear();
}

void GameTeamManager::addTeamTmpFriend( const TEAMINFO& TeamInfo )
{
	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainplayer == NULL )
	{
		return;
	}

	for ( int i = 0; i < TeamInfo.MemberNum; ++i )
	{
		const TEAMMEMBER& oneTeamMember = TeamInfo.TeamMember[i];
		if ( CompareRoleName( pMainplayer->getName(), oneTeamMember.MemberName ) )
		{
			continue;
		}

		if ( oneTeamMember.OffLineTime != CLIENT_MEMBER_ONLINE )
		{
			continue;
		}

		if ( !isInHaveAddTeamTmpFriend( oneTeamMember.MemberName ) )
		{
			GameFriendManager::getSingleton().addTmpFriend( oneTeamMember.MemberName );
			m_vecHaveAddTmpFriends.push_back( oneTeamMember.MemberName );
		}
	}
}

bool GameTeamManager::isInHaveAddTeamTmpFriend( const char* pszName )
{
	for ( std::vector<std::string>::iterator iter = m_vecHaveAddTmpFriends.begin(), end = m_vecHaveAddTmpFriends.end();
		iter != end; ++iter )
	{
		const std::string& oneHaveDelMemberName = *iter;
		if ( CompareRoleName( oneHaveDelMemberName.c_str(), pszName ) )
		{
			return true;
		}
	}

	return false;
}

void GameTeamManager::delTeamApplyInfo( int height, int low )
{
	for( std::vector<tagTeamApplyInfo_ToLua>::iterator iter = m_vectTeamApplyInfo.begin(); 
		iter != m_vectTeamApplyInfo.end(); ++iter )
	{
		if( iter->RoleID.nHeight == height && iter->RoleID.nLow == low )
		{
			m_vectTeamApplyInfo.erase( iter );
			return;
		}
	}
}
void GameTeamManager::getTeamMemberPostion( float *postionlist, float *outeye )
{
	ACTOR_ITER iter;
	int index = 0;
	for( iter  = ActorManager::getSingleton().getActorBegin(); 
		iter != ActorManager::getSingleton().getActorEnd(); ++iter )
	{
		if (isInTeamById(iter->first))
		{
			index++;postionlist[15]++;
			postionlist[index*2] = iter->second->GetPosition().x / 10;
			postionlist[index*2+1]   = iter->second->GetPosition().z / 10;
			ActorManager::getSingleton().updateActorstate(iter->second,true);
		}
	}
	outeye[15]=(int)m_TeamInfo.MemberNum;
	for ( int i = 0; i < outeye[15];i++ )
	{
		outeye[i*2]		= m_TeamInfo.TeamMember[i].Pos.X;
		outeye[i*2+1]	= m_TeamInfo.TeamMember[i].Pos.Y;
	}
}

void	GameTeamManager::showTeamer()
{	
	ACTOR_ITER iter;
	for( iter  = ActorManager::getSingleton().getActorBegin(); 
		iter != ActorManager::getSingleton().getActorEnd(); ++iter )
	{
		if (isInTeamById(iter->first))
		{
			ActorManager::getSingleton().updateActorstate(iter->second,true);
		}
	}
}

void GameTeamManager::changeBootyMode( int nMode )
{
	if( m_TeamInfo.MemberNum > 0 )
	{
		tagCSPkgBody msgbody;
		msgbody.TeamClt.OpType = TEAM_OP_CHG_BOOTY_MODE;
		msgbody.TeamClt.TeamCltData.ChgBootyMode.TeamBootyMode = nMode;
		SendGameServerMsg( TEAM_CLT,msgbody );
	}
}

// 投票T 人
void GameTeamManager::requestVoteKick( bool bAgree )
{
	tagCSPkgBody msgbody;
	msgbody.TeamClt.OpType = TEAM_OP_VOTE_KICK;
	if( bAgree )
	{
		msgbody.TeamClt.TeamCltData.TeamVoteKick.VoteFlag = 1;
	}else{
		msgbody.TeamClt.TeamCltData.TeamVoteKick.VoteFlag = 2;
	}
	SendGameServerMsg( TEAM_CLT,msgbody );
}

const char* GameTeamManager::getVotePlayerName()
{
	for( int i = 0 ; i < m_TeamInfo.MemberNum ; i++ )
	{
		if( m_TeamInfo.TeamMember[i].Uin == m_TeamInfo.KickMemberUin )
		{
			return m_TeamInfo.TeamMember[i].MemberName;
		}
	}
	return "";
}
void  GameTeamManager::Team_OP_SummonRequest(bool bAgree)
{
	tagCSPkgBody msgbody;
	msgbody.TeamClt.OpType = TEAM_OP_SUMMON_ANSWER;
	if (bAgree)
	{
		msgbody.TeamClt.TeamCltData.TeamSummonAnswer.SummonMemID	=	m_TeamSummonMemId;
		msgbody.TeamClt.TeamCltData.TeamSummonAnswer.Yes			=	1;
	}
	else
	{
		msgbody.TeamClt.TeamCltData.TeamSummonAnswer.SummonMemID	=	m_TeamSummonMemId;
		msgbody.TeamClt.TeamCltData.TeamSummonAnswer.Yes			=	0;
	}
	SendGameServerMsg( TEAM_CLT,msgbody );
}