#include "GameFriendManager.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "GameChatManager.h"
#include "GameActorManager.h"
#include "GameLoginManager.h"
#include "GameActor.h"
#include "GameClanManager.h"
#include "ServerTimer.h"
#include <algorithm>
#include "OgreLog.h"
#include "GameFollowManager.h"
#include "GameMainPlayer.h"
#include "GameTeamManager.h"
#include "GameSchool.h"

using namespace std;

const int MAX_XION_SHOU_NUM = 100;
const int MAX_SU_DI_NUM		= 16;
extern bool CompareRoleName( const char* pszLeftName, const char* pszRightName );
int getServeSecondTime()
{
	int sec, usec;
	ServerTimer::getSingleton().getServerTime( sec, usec );
	return sec;
}

bool operator< ( const tagFriendInfo& lhs, const tagFriendInfo& rhs )
{
	// 在线角色>不在线角色>首字母排列
	if ( lhs.m_nIsOnline != rhs.m_nIsOnline )
	{
		return lhs.m_nIsOnline == CLIENT_ONLINE;
	}

	return std::string( lhs.m_szName ) < rhs.m_szName;
}

bool operator==( const tagFriendInfo& lhs, const tagFriendInfo& rhs )
{
	return  ( lhs.m_RoleID.nHeight == rhs.m_RoleID.nHeight &&
				lhs.m_RoleID.nLow == rhs.m_RoleID.nLow ) || 
					( CompareRoleName( lhs.m_szName, rhs.m_szName ) );
}

bool operator== ( const tagFriendInfo& friendInfo, const char* pszName )
{
	return CompareRoleName( friendInfo.m_szName, pszName );
}

bool operator== ( const tagFriendInfo& friendInfo, const tagLong_ToLua& roleID )
{
	return friendInfo.m_RoleID.nHeight == roleID.nHeight && friendInfo.m_RoleID.nLow == roleID.nLow;
}

bool operator== ( const tagFriendInfo& friendInfo, const tdr_ulonglong roleWID )
{
	return *( tdr_ulonglong* )( &friendInfo.m_RoleID ) == roleWID;
}

GameFriendManager::GameFriendManager(void)
{
	INetworkManager::getSingleton().RegisterMsgHandler( RELATION_SVR, this );
	INetworkManager::getSingleton().RegisterMsgHandler( ONLINE_INFO, this );
	INetworkManager::getSingleton().RegisterMsgHandler( GPRS_SVR, this );
}

GameFriendManager::~GameFriendManager(void)
{
}

void GameFriendManager::objEnterView(GameActor *actor)
{
	if (actor->getType() != GAT_PLAYER)
	{
		return;
	}
	GameFriendManager& friendMgr = GameFriendManager::getSingleton();
	actor->m_nRelation = friendMgr.getPlayerRelation(actor->getName());
}

// 重置好友列表
void GameFriendManager::reSetFriendList( const char* pszCallStack )
{
	m_FriendMap.clear();

	FRI_VECTOR frivect;
	m_FriendMap[CLIENT_RELATION_TYPE_FRIEND]	= frivect;
	m_FriendMap[CLIENT_RELATION_TYPE_MASTER]	= frivect;
	m_FriendMap[CLIENT_RELATION_TYPE_PRENTICE]	= frivect;
	m_FriendMap[CLIENT_RELATION_TYPE_SPOUSE]	= frivect;
	m_FriendMap[CLIENT_RELATION_TYPE_BROTHER]	= frivect;
	m_FriendMap[CLIENT_RELATION_TYPE_XIONG_SHOU]= frivect;
	m_FriendMap[CLIENT_RELATION_TYPE_BLACK]		= frivect;
	m_FriendMap[CLIENT_RELATION_TYPE_TMPFRIEND] = frivect;
	m_FriendMap[CLIENT_RELATION_TYPE_SU_DI]		= frivect;

	memset( m_szEmotion, 0, EMOTION_LEN );
	m_increaseWid.nLow		= 0;
	m_increaseWid.nHeight	= 0;
	m_nMaxFriendNum			= 0;
	m_OnlinePlayer.clear();
}

// 追踪某人
void GameFriendManager::track( int nFlag, const char* pszName )
{
	tagCSPkgBody msgbody;

	GPRSCLT& gprsRequireData = msgbody.GPRSClt;
	gprsRequireData.Flag = nFlag;
	strncpy( gprsRequireData.RoleName, pszName, ROLE_NAME_LEN );

	SendGameServerMsg( GPRS_CLT, msgbody );
}

// 1.若已经是好友；2.若是仇人则不能加为好友
bool GameFriendManager::canBeFriend( const char* pszRoleName )
{
	if ( getPlayerRelation( pszRoleName ) == CLIENT_RELATION_TYPE_FRIEND )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_FRIEND_REPEAT_REQ );
		return false;
	}

	/*if ( getPlayerRelation( pszRoleName ) == CLIENT_RELATION_TYPE_XIONG_SHOU )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_FRIEND_IS_ENEMY );
		return false;
	}*/

	if ( getPlayerRelation( pszRoleName ) == CLIENT_RELATION_TYPE_SU_DI )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_FRIEND_IS_SU_DI );
		return false;
	}

	if ( getPlayerRelation( pszRoleName ) == CLIENT_RELATION_TYPE_BLACK )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_FRIEND_IS_BLACK );
		return false;
	}
	
	return true;
}

// 加好友(注意：界面需要限定名字和附加信息大小)
void GameFriendManager::addFriend( const char* szRoleName, const char* szReqInfo )
{
	if ( !canBeFriend( szRoleName ) )
	{
		return;
	}

	tagCSPkgBody msgbody;
	msgbody.RelationClt.OpType = RELATION_OP_TYPE_ADDRELATION_REQ;
	CSADDRELATION& addReq = msgbody.RelationClt.RelationCltData.AddRelationReq;
	addReq.Type = RELATION_TYPE_FRIEND;
	ADDFRIENDREQ& addFriendInfo = addReq.AddRelationData.AddFriend;
	strncpy( addFriendInfo.Name, szRoleName, strlen( szRoleName ) + 1 );
	strncpy( addFriendInfo.ReqInfo, szReqInfo, strlen( szReqInfo ) + 1 );
	addFriendInfo.OnlineType = getPlayerOnlineTypeByName( addFriendInfo.Name );
	SendGameServerMsg( RELATION_CLT, msgbody );
}

// 请求某人信息
void GameFriendManager::requirePlayerInfo( const char* szRoleName, int nType )
{
	tagCSPkgBody msgbody;
	msgbody.RelationClt.OpType	= RELATION_OP_TYPE_INFO_REQ;
	CSBUDDYINFOREQ& requireInfo = msgbody.RelationClt.RelationCltData.BuddyInfoReq;
	strncpy( requireInfo.ReqName, szRoleName, NAME_LEN );
	requireInfo.OnlineType = nType;
	SendGameServerMsg( RELATION_CLT, msgbody );
}

void GameFriendManager::addTmpFriend( const char* szName )
{	
	// 若所要添加的对象已经在我的关系列表中,则不添加为临时好友
	tagFriendInfo* pFriend = getFriendInfoByName( szName );
	if ( pFriend != NULL ) 
	{
		return;
	}

	tagCSPkgBody msgbody;
	msgbody.RelationClt.OpType = RELATION_OP_TYPE_ADDRELATION_REQ;
	CSADDRELATION& addRelationReq = msgbody.RelationClt.RelationCltData.AddRelationReq;
	addRelationReq.Type = RELATION_TYPE_TMPFRIEND;
	ADDTMPFRIENDREQ& tmpFriendReqInfo = addRelationReq.AddRelationData.AddTmpFriend;
	strncpy( tmpFriendReqInfo.Name, szName, strlen( szName ) + 1 );
	SendGameServerMsg( RELATION_CLT, msgbody );
}

// 添加临时仇人
void GameFriendManager::addTmpEnemy( const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.RelationClt.OpType = RELATION_OP_TYPE_ADDRELATION_REQ;
	CSADDRELATION& addRelationReq = msgbody.RelationClt.RelationCltData.AddRelationReq;
	addRelationReq.Type = RELATION_TYPE_TMPENEMY;
	ADDTMPENMEYREQ& tmpEnemyReqInfo = addRelationReq.AddRelationData.AddTmpEnemy;
	strncpy( tmpEnemyReqInfo.Name, szName, strlen( szName ) + 1 );
	SendGameServerMsg( RELATION_CLT, msgbody );
}

// 加黑名单
void GameFriendManager::addBlack( const char* szRoleName )
{
	if( LoginManager::getSingleton().isInBattleWorld() )
	{
		GameActor* pActor = ActorManager::getSingleton().findActorByName(szRoleName);
		if( pActor != NULL && pActor->isCamp(SKILL_TARGET_ENEMY_PLAYER) )
		{
			FRI_VECTOR& fri_vector	= m_FriendMap[CLIENT_RELATION_TYPE_BLACK];
			if ( std::find( fri_vector.begin(), fri_vector.end(), pActor->getName() ) == fri_vector.end() )
			{
				tagFriendInfo friendInfo;
				memset( &friendInfo, 0, sizeof( tagFriendInfo ) );
				m_increaseWid.nLow ++;
				memcpy( &friendInfo.m_RoleID,&m_increaseWid, sizeof(tdr_ulonglong) );
				friendInfo.m_nRelationType		= CLIENT_RELATION_TYPE_BLACK;
				friendInfo.m_bOutSide			= true;
				friendInfo.m_nIsOnline			= true;

				friendInfo.m_nRelationType = CLIENT_RELATION_TYPE_BLACK;
				strcpy( friendInfo.m_szName, pActor->getName() );
				friendInfo.m_nGender = pActor->getGender();
				friendInfo.m_nLevel = pActor->getLv();
				friendInfo.m_nHead = pActor->getHead();
				friendInfo.m_nHair = pActor->getHair();
				friendInfo.m_nFace = pActor->getFace();
				friendInfo.m_nCareer = pActor->getRace();
				fri_vector.push_back( friendInfo );
				UpdateScenePlayer( friendInfo.m_szName );
				GameManager::getSingleton().SendUIEvent( "GE_FRIEND_GETLIST" );
				return;
			}
		}
	}

	tagCSPkgBody msgbody;
	msgbody.RelationClt.OpType = RELATION_OP_TYPE_ADDRELATION_REQ;
	CSADDRELATION& addReq = msgbody.RelationClt.RelationCltData.AddRelationReq;
	addReq.Type = RELATION_TYPE_BLACK;
	ADDBLACKREQ& addBlackInfo = addReq.AddRelationData.AddBlack;
	strncpy( addBlackInfo.Name, szRoleName, strlen( szRoleName ) + 1 );
	SendGameServerMsg( RELATION_CLT, msgbody );
}

// 加追杀名单
void GameFriendManager::addEnem( const char* szRoleName, int nRoleHightId, int nRoleLowId )
{
	tagCSPkgBody msgbody;
	msgbody.RelationClt.OpType = RELATION_OP_TYPE_ADDRELATION_REQ;
	CSADDRELATION& addReq = msgbody.RelationClt.RelationCltData.AddRelationReq;
	addReq.Type = RELATION_TYPE_ENEMY;
	ADDENEMYREQ& addEnemInfo = addReq.AddRelationData.AddEnemy;
	strncpy( addEnemInfo.Name, szRoleName, strlen( szRoleName ) + 1 );
	tagLong_ToLua LongId;
	LongId.nHeight	= nRoleHightId;
	LongId.nLow		= nRoleLowId;	
	addEnemInfo.RoleID = *(tdr_ulonglong*)(&LongId);
	SendGameServerMsg( RELATION_CLT, msgbody );
}

// 可以接受好友邀请
bool GameFriendManager::canAcceptBeFriend( const char* pszRoleName )
{
	/*if ( getPlayerRelation( pszRoleName ) == CLIENT_RELATION_TYPE_XIONG_SHOU )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_FRIEND_IS_ENEMY );
		return false;
	}*/

	/*if ( getPlayerRelation( pszRoleName ) == CLIENT_RELATION_TYPE_BLACK )
	{
	GameMsgPrint::getSingleton().showMidTip( MID_FRIEND_IS_BLACK );
	return false;
	}*/

	return true;
}

// 好友添加消息回复
// 说明：Accept=0 表示拒绝, AddOther=0 表示不添加对方为好友,客户端收到RELATION_OP_TYPE_CHG消息后,添加好友
void GameFriendManager::addFriendRes( const char* szRoleName, int nAccept, int nAddOther, bool bTimeOut )
{
	if ( nAccept == 1 && !canAcceptBeFriend( szRoleName ) )
	{
		nAccept = 0;
		//return;
	}

	tagCSPkgBody msgbody;
	memset( &msgbody, 0 ,sizeof(tagCSPkgBody) );
	msgbody.RelationClt.OpType = RELATION_OP_TYPE_ADDFRIEND_RES;
	CSADDFRIENDRES& addFriendResInfo = msgbody.RelationClt.RelationCltData.AddFriendRes;
	strncpy( addFriendResInfo.ReqName, szRoleName, strlen( szRoleName ) + 1 );
	addFriendResInfo.Accept		= nAccept;
	addFriendResInfo.AddOther	= nAddOther;
	addFriendResInfo.OnlineType = getPlayerOnlineTypeByName( szRoleName );
	if( bTimeOut )
	{
		addFriendResInfo.Reason = 1;
	}
	SendGameServerMsg( RELATION_CLT, msgbody );
}

// 修改心情签名
void GameFriendManager::changeEmotion( const char* szEmotion )
{
	tagCSPkgBody msgbody;
	msgbody.RelationClt.OpType = RELATION_OP_TYPE_EMOTION_REQ;
	CSEMOTIONREQ& emotionInfo = msgbody.RelationClt.RelationCltData.EmotionReq;
	strncpy( emotionInfo.Emotion, szEmotion, strlen( szEmotion ) + 1 );
	SendGameServerMsg( RELATION_CLT, msgbody );
}

// 修改备注姓名
// 注意:服务端不回复，客户端直接改
void GameFriendManager::changeMemName( const char* szRoleName, const char* szMemName )
{
	tagCSPkgBody msgbody;
	msgbody.RelationClt.OpType = RELATION_OP_TYPE_MEMNAME_REQ;
	CSMEMNAMEREQ& changeMemNameInfo = msgbody.RelationClt.RelationCltData.MemNameReq;
	strncpy( changeMemNameInfo.Name,	szRoleName, strlen( szRoleName ) + 1 );
	strncpy( changeMemNameInfo.MEMNAME, szMemName,	strlen( szMemName ) + 1 );
	SendGameServerMsg( RELATION_CLT, msgbody );

	tagFriendInfo* pfriendName = getFriendInfoByName( szRoleName );
	strncpy( pfriendName->m_szMemName, szMemName, strlen( szMemName ) + 1 );
}

//设置好友分组
void GameFriendManager::setFriendGroup( const char* szRoleName, int nGroupID )
{
	tagCSPkgBody msgbody;
	msgbody.RelationClt.OpType = RELATION_OP_TYPE_SET_GROUP;
	CSSETGROUPREQ& groupInfo = msgbody.RelationClt.RelationCltData.SetGroupReq;
	strncpy( groupInfo.RoleName, szRoleName, strlen( szRoleName ) + 1 );
	groupInfo.GroupID = nGroupID;
	SendGameServerMsg( RELATION_CLT, msgbody );
}

//获取最大好友数
int GameFriendManager::getMaxFriendNum()
{
	if ( m_nMaxFriendNum > 0 )
	{
		return m_nMaxFriendNum;
	}
	return 0;
}

// 删除关系请求
// nType对应CLIENT_RELATION_TYPE_FRIEND...
void GameFriendManager::delRelation( int nType, int nRoleHightId, int nRoleLowId )
{
	if( nType == CLIENT_RELATION_TYPE_BLACK )
	{
		FRI_VECTOR& fri_vector	= m_FriendMap[nType];
		tagLong_ToLua uid;
		uid.nHeight	= nRoleHightId;
		uid.nLow	= nRoleLowId;
		FRI_VECTOR_ITER iter = std::find( fri_vector.begin(), fri_vector.end(), uid );
		if( iter != fri_vector.end() && iter->m_bOutSide == true )
		{
			const tagFriendInfo& oneFriendInfo = *iter;
			fri_vector.erase(iter);
			GameManager::getSingleton().SendUIEvent( "GE_FRIEND_GETLIST" );
			return;
		}
	}

	tagCSPkgBody msgbody;
	msgbody.RelationClt.OpType = RELATION_OP_TYPE_DELRELATION_REQ;
	CSDELRELATIONREQ& delRelationInfo = msgbody.RelationClt.RelationCltData.DelRelationReq;
	delRelationInfo.Type = nType;
	tagLong_ToLua LongId;
	LongId.nHeight	= nRoleHightId;
	LongId.nLow		= nRoleLowId;	
	memcpy( &delRelationInfo.RoleID, &LongId, sizeof( tdr_ulonglong ) );
	SendGameServerMsg( RELATION_CLT, msgbody );
}

// 查看玩家装备
void GameFriendManager::seeAvatarReq( const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.RelationClt.OpType = RELATION_OP_TYPE_AVATAR_REQ;
	CSAVATARREQ& avatarReqInfo = msgbody.RelationClt.RelationCltData.AvatarReq;
	strncpy( avatarReqInfo.Name, szName, strlen( szName ) + 1 );
	SendGameServerMsg( RELATION_CLT, msgbody );
}

bool otherFriendCompare(const tagFriendInfo& lhs,const tagFriendInfo& rhs )
{
	return (int)( lhs.m_nIntimateColorType - rhs.m_nIntimateColorType ) > 0?true:false;
}

// 重新排序
void GameFriendManager::sortFriendInfo( int nRelationType )
{
	FRI_VECTOR& fri_vector = m_FriendMap[nRelationType];
	std::stable_sort( fri_vector.begin(), fri_vector.end() );
	std::stable_sort( fri_vector.begin(), fri_vector.end(), otherFriendCompare );
}

int GameFriendManager::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if( msgid == RELATION_SVR ) // 这是用于从服务器中查询好友面板中的相关信息
	{
		const CSRELATIONSVR& msg = msgbody.RelationSvr;
		switch( msg.OpType )
		{
		case RELATION_OP_TYPE_CHG: // 这是对已有的玩家的信息进行更新
			{
				const CSRELATIONCHG& relationChgInfo = msg.RelationSvrData.RelationChg;
				OnRelationTypeChange( relationChgInfo );
			}
			break;

		case RELATION_OP_TYPE_CHGLIST://这是用于更新客户端某个类型的玩家列表中所有玩家信息
			{
				const CSRELATIONCHGLIST& relationChgList = msg.RelationSvrData.RelationChgList;
				OnRelationTypeListChange( relationChgList );
			}
			break;

		case RELATION_OP_TYPE_ADDFRIEND_INFO:
			{
				// 若设置了拒绝好友申请
				SystemSettingConfig& systemConfig = GameManager::getSingleton().getUserSystemConfig();
				const CSRELATIONADDFRIENDINFO& addFriendInfo = msg.RelationSvrData.RelationAddFriendInfo;
				setOnlinePlayer( addFriendInfo.ReqName, addFriendInfo.OnlineType );
				if ( systemConfig.getGameOpt( GAME_OPT_REFUSE_ADD_FRIEND_REQ ) )
				{
					// 若设置拒绝被加，直接回复拒绝
					addFriendRes( addFriendInfo.ReqName, 0, 0 );
				}
				else
				{
					tagUIMsgBody UI_msg;
					strncpy( UI_msg.AddFriendInfo.m_szReqName, addFriendInfo.ReqName, 
								strlen( addFriendInfo.ReqName ) + 1 );
					GameUIMsg::getSingleton().setUIMsg( UI_msg );
					GameManager::getSingleton().SendUIEvent( "GE_FRIEND_ADDFRIENDINFO" );
					GameManager::getSingleton().FlashWindow();
				}
			}
			break;

		case RELATION_OP_TYPE_AVATAR_RES:
			{
				// TODO::获取玩家装备信息

			}
			break;

		case RELATION_OP_TYPE_DELRELATION_RES: //这是用于从某个类型的玩家列表中删除某个玩家
			{
				const CSRELATIONDELRES& delInfo = msg.RelationSvrData.RelationDelRes;
				deleteRole( delInfo.Type, delInfo.RoleID );
				GameManager::getSingleton().SendUIEvent( "GE_FRIEND_GETLIST" );
			}
			break;

		case RELATION_OP_TYPE_EMOTION_RES: //这是在收到服务器的相应消息后更新当前我控制的玩家的心情签名
			{
				const CSEMOTIONRES& emotionRes = msg.RelationSvrData.EmotionRes;
				strncpy( m_szEmotion, emotionRes.Emotion, strlen( emotionRes.Emotion ) + 1 );
				GameManager::getSingleton().SendUIEvent( "GE_FRIEND_UPDATAEMOTION" );
			}
			break;

		case RELATION_OP_TYPE_CHG_CLAN:
			{
				const RELATIONCLANINFO& clanInfo = msg.RelationSvrData.RelationClanInfo;
				updateClanInfo( clanInfo.RoleID, clanInfo.RelationClan );
			}
			break;

		case RELATION_OP_TYPE_CHGLIST_CLAN:
			{
				const RELATIONCLANINFOLIST& clanList = msg.RelationSvrData.RelationClanInfoList;
				for ( int i = 0; i < clanList.Num; ++i )
				{
					const RELATIONCLANINFO& oneRoleClanInfo = clanList.RelationClanInfo[i];
					updateClanInfo( oneRoleClanInfo.RoleID, oneRoleClanInfo.RelationClan );
				}
			}
			break;

		case RELATION_OP_TYPE_INFO_RES: /* 服务器关系信息回复 */
			{
				const CSBUDDYINFORES& buddyInfSvrRes = msg.RelationSvrData.BuddyInfoRes;
				tagCSBuddyInfoRes_ToLua& buddyInfoResData = m_CurRequireRoleInfo;
				
				buddyInfoResData.Career	= buddyInfSvrRes.Career;
				strncpy( buddyInfoResData.ClanName, buddyInfSvrRes.ClanName, NAME_LEN );
				buddyInfoResData.Face	= buddyInfSvrRes.Face;
				buddyInfoResData.Gender	= buddyInfSvrRes.Gender;
				buddyInfoResData.Hair	= buddyInfSvrRes.Hair;
				buddyInfoResData.Head	= buddyInfSvrRes.Head;
				buddyInfoResData.Level	= buddyInfSvrRes.Level;
				buddyInfoResData.ClanWid = buddyInfSvrRes.ClanWid;
				buddyInfoResData.FriendVal = buddyInfSvrRes.FriendVal;
				buddyInfoResData.Map	= buddyInfSvrRes.Map;
				buddyInfoResData.Pos	= buddyInfSvrRes.Pos;
				buddyInfoResData.TeamRoleNum = buddyInfSvrRes.TeamRoleNum;
				buddyInfoResData.Span = buddyInfSvrRes.Span;
				buddyInfoResData.Charm = buddyInfSvrRes.Charm;
				strncpy( buddyInfoResData.Emotion, buddyInfSvrRes.Emotion, NAME_LEN );
				strncpy( buddyInfoResData.RoleName, buddyInfSvrRes.RoleName, NAME_LEN );
		
				GameManager::getSingleton().SendUIEvent( "GE_FRIEND_GET_REQUIRE_PLAYER_INFO" );
			}
			break;

		case RELATION_OP_TYPE_MAX_FRIEND:
			{
				const CSRELATIONMAXRES& relationMaxInfo = msg.RelationSvrData.BuddyMax;
				m_nMaxFriendNum = relationMaxInfo.FriendNum;
			}
			break;

		case RELATION_OP_TYPE_INVOLVE_MARRY_SVR:
			{
				HandleMarryInfo( msg.RelationSvrData.InvolveMarrySvr );
			}
			break;

		default:
			break;
		}
	}
	else if ( msgid == ONLINE_INFO )//这是用于查询某个玩家是否在线
	{
		const CSONLINESVR& onlineInfo = msgbody.OnlineInfo;
		setOnlinePlayer( onlineInfo.RoleName, onlineInfo.Online);
		//检测这个人是我的好友,若不是我的好友则不提示他上线
		FRI_VECTOR& fri_vector	= m_FriendMap[CLIENT_RELATION_TYPE_FRIEND];
		FRI_VECTOR_ITER iter = std::find( fri_vector.begin(), fri_vector.end(), onlineInfo.RoleID );
		if ( iter != fri_vector.end() )
		{
			roleOnLineInfo( onlineInfo.Online, iter->m_szName );
		}

		tagFriendInfo* pFriendInfo = getFriendInfoByRoleId( onlineInfo.RoleID );
		if ( pFriendInfo != NULL )
		{
			pFriendInfo->m_nIsOnline = onlineInfo.Online;
			GameManager::getSingleton().getScriptVM()->callString( "RoleOnLine()" );
			if ( onlineInfo.Online == CLIENT_OFFLINE )
			{
				char buffer[256];
				sprintf( buffer, "OffLine( \"%s\" )", onlineInfo.RoleName );
				GameManager::getSingleton().getScriptVM()->callString( buffer );
			}
		}
		if( GameClanManager::getSingleton().isClanOfficer( onlineInfo.RoleName ) 
			&& onlineInfo.Online == CLIENT_ONLINE )
		{
			char buff[256];
			sprintf( buff,"ClanMemeberOnLine(\"%s\",%d)",onlineInfo.RoleName,onlineInfo.Online );
			GameManager::getSingleton().getScriptVM()->callString( buff );
		}
		if ( GameSchool::getSingleton().IsSelfSchool( onlineInfo.RoleName ) )
		{
			GameManager::getSingleton().getScriptVM()->callString( "UpdateSchoolRelationShip()" );
		}
		//下线
		if( onlineInfo.Online == CLIENT_OFFLINE )
		{
			//如果这个人在公会的申请列表的话，清除掉
			tagApplyJoinList& applyList = GameClanManager::getSingleton().getApplyJoinList();
			for( int i = 0 ; i < applyList.m_nRoleNum ; i++ )
			{
				if( CompareRoleName( applyList.m_ApplyRoles[i].RoleMember.m_szRoleName,onlineInfo.RoleName ) )
				{
					for( int j = i; j < applyList.m_nRoleNum - 1; j ++ )
					{
						std::swap( applyList.m_ApplyRoles[j],applyList.m_ApplyRoles[j+1] );
					}
					applyList.m_nRoleNum --;
					break;
				}
			}

			// 刷新申请列表
			GameManager::getSingleton().SendUIEvent( "GE_CLAN_JOINLIST" );	

			//如果玩家正在跟随的对象下线则取消跟随
			MainGamePlayer* pMainPlayer	= ActorManager::getSingleton().getMainPlayer();
			if ( pMainPlayer != NULL )
			{
				if ( GameFollowManager::getSingleton().isWeiXing() && GameFollowManager::getSingleton().isWeiXingTarget( onlineInfo.RoleName, 0 ) )
				{
					GameFollowManager::getSingleton().cancelWeiXing();
				}				
			}							
		}		
	}
	else if ( msgid == GPRS_SVR )
	{
		const GPRSSVR& gprsSvrData = msgbody.GPRSSvr;
		tagUIMsgBody UI_Msg;
		tagGPRSMsg& gprsMsgData = UI_Msg.GPRSData;
		/*gprsMsgData.GPRSEnd	= gprsSvrData.GPRSEnd;
		gprsMsgData.Map		= gprsSvrData.Map;
		strncpy( gprsMsgData.RoleName, gprsSvrData.RoleName, ROLE_NAME_LEN );
		gprsMsgData.Team	= gprsSvrData.Team;
		gprsMsgData.X		= gprsSvrData.X;
		gprsMsgData.Y		= gprsSvrData.Y;*/
		memcpy( &gprsMsgData, &gprsSvrData, sizeof( tagGPRSMsg ) );
		static_assert( sizeof( tagGPRSMsg ) == sizeof( GPRSSVR ) );
		GameUIMsg::getSingleton().setUIMsg( UI_Msg );
		GameManager::getSingleton().SendUIEvent( "GE_GET_TRACKPLAYER_INFO" );
	}
	return 0;
}

void GameFriendManager::resetFriendList()
{
	for ( FRI_MAP_ITER map_iter = m_FriendMap.begin(), map_end = m_FriendMap.end(); map_iter != map_end; ++map_iter )
	{
		map_iter->second.clear();
	}
}

void GameFriendManager::deleteRole( int nRelationType, tdr_ulonglong roleWID )
{
	FRI_VECTOR& fri_vector = m_FriendMap[nRelationType];
	FRI_VECTOR_ITER iter = std::find( fri_vector.begin(), fri_vector.end(), roleWID );
	if ( iter != fri_vector.end())
	{
		const tagFriendInfo& oneFriendInfo	= *iter;
		std::string strEraseRoleName		= oneFriendInfo.m_szName;
		fri_vector.erase( iter );
		UpdateScenePlayer( strEraseRoleName.c_str() );
		GameManager::getSingleton().SendUIEvent( "GE_FRIEND_GETLIST" );
	}
}

// 根据WID删除玩家
void GameFriendManager::deleteRole( tdr_ulonglong roleWID )
{
	for ( FRI_MAP_ITER iter = m_FriendMap.begin(); iter != m_FriendMap.end(); ++iter )
	{
		FRI_VECTOR& fri_vector		= iter->second;
		FRI_VECTOR_ITER vec_iter	= std::find( fri_vector.begin(), fri_vector.end(), roleWID );
		if ( vec_iter != fri_vector.end() )
		{
			const tagFriendInfo& oneFriendInfo	= *vec_iter;
			std::string strEraseRoleName		= oneFriendInfo.m_szName;
			fri_vector.erase( vec_iter );
			UpdateScenePlayer( strEraseRoleName.c_str() );
		}
	}
	GameManager::getSingleton().SendUIEvent( "GE_FRIEND_GETLIST" );
}

void GameFriendManager::deleteRole( int nRelationType, const char* pszName )
{
	FRI_VECTOR& fri_vector = m_FriendMap[nRelationType];
	FRI_VECTOR_ITER iter = std::find( fri_vector.begin(), fri_vector.end(), pszName );
	if ( iter != fri_vector.end() )
	{
		fri_vector.erase( iter );
		UpdateScenePlayer( pszName );
	}
	GameManager::getSingleton().SendUIEvent( "GE_FRIEND_GETLIST" );
}

tagFriendInfo* GameFriendManager::getFriendInfoByName( int nRelationType, const char* pszName )
{
	FRI_VECTOR& fri_vector = m_FriendMap[nRelationType];
	FRI_VECTOR_ITER iter_vector = std::find( fri_vector.begin(), fri_vector.end(), pszName );
	if ( iter_vector != fri_vector.end() )
	{
		return &( *iter_vector );
	}

	return NULL;
}

// 根据姓名获取好友信息
tagFriendInfo* GameFriendManager::getFriendInfoByName( const char* pszName )
{
	for( FRI_MAP_ITER iter = m_FriendMap.begin(); iter != m_FriendMap.end(); ++iter )
	{
		tagFriendInfo* pFriendInfo = getFriendInfoByName( iter->first, pszName );
		if ( pFriendInfo != NULL )
		{
			return pFriendInfo;
		}
	}

	return NULL;
}	

// 根据好友Id获取好友信息
tagFriendInfo* GameFriendManager::getFriendInfoByRoleId( int nRoleHightId, int nRoleLowId )
{
	tagLong_ToLua LongId;
	LongId.nHeight	= nRoleHightId;
	LongId.nLow		= nRoleLowId;	
	return getFriendInfoByRoleId( *(tdr_ulonglong*)( & LongId ) );
}

tagFriendInfo* GameFriendManager::getFriendInfoByRoleId( tdr_ulonglong roleWID )
{
	for ( FRI_MAP_ITER iter = m_FriendMap.begin(); iter != m_FriendMap.end(); ++iter )
	{
		FRI_VECTOR& fri_vector = iter->second;
		for ( FRI_VECTOR_ITER iter_vector = fri_vector.begin(); iter_vector != fri_vector.end(); ++iter_vector )
		{
			if ( *(tdr_ulonglong*)(&iter_vector->m_RoleID) == roleWID )
			{
				return &(*iter_vector);
			}
		}
	}
	return NULL;
}

// 获取关系玩家数量
int GameFriendManager::getRelationPlayerCount( int nType )
{
	FRI_MAP_ITER iter = m_FriendMap.find( nType );
	if( iter != m_FriendMap.end() )
	{
		FRI_VECTOR::size_type	length = iter->second.size();//DEL
		return (int)iter->second.size();
	}

	assert(false );
	return 0;
}

// 根据index获取好友信息
tagFriendInfo* GameFriendManager::getFriendInfoByIndex( int nType, int nIndex )
{
	FRI_MAP_ITER iter = m_FriendMap.find( nType );
	if( iter != m_FriendMap.end() )
	{
		assert( nIndex < (int)iter->second.size() );
		return &( (iter->second)[nIndex] );
	}

	assert(0);
	return NULL;
}

const char* GameFriendManager::getEmotion() const
{
	return m_szEmotion;
}

// 查询该玩家是否是指定关系
bool GameFriendManager::isPlayerRelation( int nRelationType, const char* pszName )
{
	FRI_VECTOR& fri_vector = m_FriendMap[nRelationType];
	return std::find( fri_vector.begin(), fri_vector.end(), pszName ) != fri_vector.end();
}

// 查询玩家关系
bool GameFriendManager::isPlayerRelation( int nRelationType, int nHeightId, int nLowId )
{
	FRI_VECTOR& fri_vector = m_FriendMap[nRelationType];
	tagLong_ToLua roleID;
	roleID.nHeight	= nHeightId;
	roleID.nLow		=  nLowId;
	return std::find( fri_vector.begin(), fri_vector.end(), roleID ) != fri_vector.end();
}

// 查询玩家关系
int GameFriendManager::getPlayerRelation( const char* pszName )
{
	for ( FRI_MAP_ITER iter = m_FriendMap.begin(), end = m_FriendMap.end(); iter != end; ++iter )
	{
		if ( isPlayerRelation( iter->first, pszName ) )
		{
			return iter->first;
		}
	}

	return CLIENT_RELATION_TYPE_NONE;
}

// 查询当前请求的玩家
tagCSBuddyInfoRes_ToLua& GameFriendManager::getCurRequireRoleInfo()
{
	return m_CurRequireRoleInfo;
}

void GameFriendManager::setRoleAttr( tagFriendInfo& info, int nType, const CSRELATIONATTRDATA* pdata )
{
	switch( nType )
	{
	case RELATION_ATTR_GPRSEND:
		{
			info.m_nGprsEnd = pdata->GprsEnd;
		}
		break;

	case RELATION_ATTR_NAME:
		{
			strncpy( info.m_szName, pdata->Name, strlen( pdata->Name ) + 1 );
		}
		break;
	case RELATION_ATTR_GENDER:
		{
			info.m_nGender = (int)pdata->Gender;
		}
		break;
	case RELATION_ATTR_LEVEL:
		{
			info.m_nLevel = (int)pdata->Level;
		}
		break;
	case RELATION_ATTR_BUDDYVAL:
		{
			info.m_nBuddyVal = (int)pdata->BuddyVal;
		}
		break;
	case RELATION_ATTR_HEAD:
		{
			info.m_nHead	= pdata->Head >> 16;
			info.m_nHair	= ( pdata->Head >> 8 ) & 0xff;
			info.m_nFace	= pdata->Head & 0X000000FF;
		}
		break;
	case RELATION_ATTR_CAREER:
		{
			info.m_nCareer = (int)pdata->Career;
		}
		break;
	case RELATION_ATTR_EMOTION:
		{
			strncpy( info.m_szEmotion, pdata->Emotion, strlen( pdata->Emotion ) + 1 );
		}
		break;
	case RELATION_ATTR_MEMNAME:
		{
			strncpy( info.m_szMemName, pdata->MemName, strlen( pdata->MemName ) + 1 );
		}
		break;
	case RELATION_ATTR_ONLINE:
		{
			info.m_nIsOnline = pdata->IsOnline;
		}
		break;
	case RELATION_ATTR_GROUP:
		{
			info.m_nGroupID = (int)pdata->GroupID;
		}
		break;
	
	default:
		break;
	}
}

// 对象上下线通知
void GameFriendManager::roleOnLineInfo( int nOnline, const char* pszName )
{
	char buf[256];
	memset( buf, 0, sizeof(buf) );
	if ( nOnline == 1 )
	{
		sprintf( buf, "您的好友:%s 上线", pszName );
	}else if( nOnline == 2 )
	{
		sprintf( buf ,"您的好友:%s 暂离游戏，进入引力空间",pszName );
	}else
	{
		sprintf( buf, "您的好友:%s 下线", pszName );
	}
	ChatManager::getSingleton().ShowSysMsg( buf );
}

void GameFriendManager::onLeavePWorld( int nMapid )
{
	const tagPworldDef *pworldDef = DBDataManager::getSingleton().getPworldDef( nMapid / 100 );
	if( pworldDef != NULL && pworldDef->PolyType == PWORLD_POLY_SPAN_WORLD )
	{
		FRI_VECTOR& fri_vector	= m_FriendMap[CLIENT_RELATION_TYPE_BLACK];
		FRI_VECTOR_ITER iter = fri_vector.begin();
		while(iter != fri_vector.end() )
		{
			if( iter->m_bOutSide == true )
			{
				const tagFriendInfo& oneFriendInfo = *iter;
				std::string strEraseRoleName = oneFriendInfo.m_szName;
				iter = fri_vector.erase(iter);
				UpdateScenePlayer( strEraseRoleName.c_str() );
			}else{
				iter ++ ;
			}
		}
	}
	GameManager::getSingleton().SendUIEvent( "GE_FRIEND_GETLIST" );
}

bool GameFriendManager::isInBattleBlack(const char* szName)
{
	if( LoginManager::getSingleton().isInBattleWorld() )
	{
		return isPlayerRelation( CLIENT_RELATION_TYPE_BLACK,szName );
	}
	return false;
}

void GameFriendManager::changeFriendName( const char* pszOldName, const char* pszNewName )
{
	for ( FRI_MAP_ITER mapIter = m_FriendMap.begin(), mapEnd = m_FriendMap.end(); mapIter != mapEnd; ++mapIter )
	{
		FRI_VECTOR& fri_vector = mapIter->second;
		for ( FRI_VECTOR_ITER vecIter = fri_vector.begin(), vecEnd = fri_vector.end(); vecIter != vecEnd; ++vecIter )
		{
			tagFriendInfo& oneRoleInfo = *vecIter;
			if ( strncmp( pszOldName, oneRoleInfo.m_szName, NAME_LEN ) == 0 )
			{
				strncpy( oneRoleInfo.m_szName, pszNewName, NAME_LEN );
				
				tagUIMsgBody UI_Msg;
				strncpy( UI_Msg.ChangeNameData.szOldName, pszOldName, NAME_LEN );
				strncpy( UI_Msg.ChangeNameData.szNewName, pszNewName, NAME_LEN );
				GameUIMsg::getSingleton().setUIMsg( UI_Msg );
				GameManager::getSingleton().SendUIEvent( "GE_FRIEND_CHGNAME" );
				break;
			}
		}
	}
}

// 更新公会信息
void GameFriendManager::updateClanInfo( tdr_ulonglong roleWID, const RELATIONCLAN& clanInfo )
{
	for ( FRI_MAP_ITER iter = m_FriendMap.begin(); iter != m_FriendMap.end(); ++iter )
	{
		FRI_VECTOR& fri_vector = iter->second;
		for ( FRI_VECTOR_ITER iter_vector = fri_vector.begin(); iter_vector != fri_vector.end(); ++iter_vector )
		{
			tagFriendInfo& oneRoleInfo = *iter_vector;
			if ( *(tdr_ulonglong*)(&oneRoleInfo.m_RoleID) == roleWID )
			{
				strncpy( oneRoleInfo.m_szClanName, clanInfo.ClanName, NAME_LEN );
				oneRoleInfo.m_nClanID = clanInfo.ClanID;
				break;
			}
		}
	}
}

//更新在线人员信息
void GameFriendManager::setOnlinePlayer(string szName,int OnlineType)
{
	if ( OnlineType == 1 || OnlineType == 2 )
	{
		map<string,int>::iterator olpliter=m_OnlinePlayer.find(szName);
		if ( olpliter != m_OnlinePlayer.end() )
		{
			olpliter->second = OnlineType;
		}
		else
		{
			m_OnlinePlayer[szName] = OnlineType;
		}
	}

	if ( OnlineType == 0 )
	{
		map<string,int>::iterator olpliter=m_OnlinePlayer.find(szName);
		if ( olpliter != m_OnlinePlayer.end() )
		{
			m_OnlinePlayer.erase( olpliter );
		}
	}
}

int GameFriendManager::getPlayerOnlineTypeByName( string szName)
{
	map<string,int>::iterator iter = m_OnlinePlayer.find( szName );
	if ( iter != m_OnlinePlayer.end() )
	{
		return iter->second;
	}
	
	tagFriendInfo* Friendinfo=GameFriendManager::getSingleton().getFriendInfoByName(szName.c_str());
	if ( Friendinfo )
	{
		return Friendinfo->m_nIsOnline;
	}
	return  GameClanManager::getSingleton().CLanMenmberOnlineinfo(szName.c_str());
}

void GameFriendManager::OnRelationTypeChange( const CSRELATIONCHG& relationChgInfo )
{
	FRI_VECTOR& fri_vector	= m_FriendMap[relationChgInfo.RelationType];
	// 若是新加的人，则先将其加入成员存储结构中
	if ( std::find( fri_vector.begin(), fri_vector.end(), relationChgInfo.RoleID ) == fri_vector.end() )
	{
		tagFriendInfo friendInfo;
		memset( &friendInfo, 0, sizeof( tagFriendInfo ) );
		memcpy( &friendInfo.m_RoleID, &relationChgInfo.RoleID, sizeof(tdr_ulonglong) );
		friendInfo.m_nRelationType		= relationChgInfo.RelationType;
		friendInfo.m_bOutSide			= false;
		fri_vector.push_back( friendInfo );
	}

	FRI_VECTOR_ITER iter = std::find( fri_vector.begin(), fri_vector.end(), relationChgInfo.RoleID );
	assert( iter != fri_vector.end() );
	iter->m_nRelationType = relationChgInfo.RelationType;
	for ( int i = 0; i < relationChgInfo.RelationAttrNum; ++i )
	{
		const RELATIONATTR& relationAttr = relationChgInfo.RelationAttrs[i];
		setRoleAttr( *iter, relationAttr.RelationAttrID, &relationAttr.RelationAttrData );
	}

	// 确保当前加为好友的玩家不会重复出现在临时列表中
	// 检查具有互斥关系
	if ( relationChgInfo.RelationType != CLIENT_RELATION_TYPE_TMPFRIEND )
	{
		deleteRole(  CLIENT_RELATION_TYPE_TMPFRIEND, fri_vector.back().m_szName );
	}

	if ( relationChgInfo.RelationType == CLIENT_RELATION_TYPE_SU_DI )
	{
		const tagFriendInfo& friendInfo = fri_vector.back();
		FRI_VECTOR& xionShouVector = m_FriendMap[relationChgInfo.RelationType];
		if ( xionShouVector.size() > MAX_SU_DI_NUM )
		{
			delRelation(  CLIENT_RELATION_TYPE_SU_DI, friendInfo.m_RoleID.nHeight, friendInfo.m_RoleID.nLow );
		}

		tagFriendInfo* pFriendInfo = getFriendInfoByName( CLIENT_RELATION_TYPE_FRIEND, friendInfo.m_szName );
		if ( pFriendInfo != NULL )
		{
			const tagLong_ToLua& roleWID = pFriendInfo->m_RoleID;
			delRelation( CLIENT_RELATION_TYPE_FRIEND, roleWID.nHeight, roleWID.nLow );
		}

		deleteRole( CLIENT_RELATION_TYPE_TMPFRIEND, friendInfo.m_szName );
		deleteRole( CLIENT_RELATION_TYPE_XIONG_SHOU, friendInfo.m_szName );
	}

	// TODO: 策划确认凶手作为临时列表可以与好友临时好友共存的
	GameManager::getSingleton().SendUIEvent( "GE_FRIEND_GETLIST" );

	const tagFriendInfo& friendInfo = fri_vector.back();
	UpdateScenePlayer( friendInfo.m_szName );
}

void GameFriendManager::OnRelationTypeListChange( const CSRELATIONCHGLIST& relationChgList )
{
	for ( int i = 0; i < relationChgList.Num; i++ )
	{
		const CSRELATIONCHG& relationChgInfo = relationChgList.ChgList[i];
		FRI_VECTOR& fri_vector	= m_FriendMap[relationChgInfo.RelationType];
		if ( std::find( fri_vector.begin(), fri_vector.end(), relationChgInfo.RoleID ) == fri_vector.end() )
		{
			// 若没找到该人物信息，则将其信息存入内存
			tagFriendInfo friendInfo;
			memset( &friendInfo, 0, sizeof( tagFriendInfo ) );
			memcpy( &friendInfo.m_RoleID, &relationChgInfo.RoleID, sizeof(tdr_ulonglong) );
			friendInfo.m_bOutSide = false;
			fri_vector.push_back( friendInfo );
		}

		FRI_VECTOR_ITER fri_iter = std::find( fri_vector.begin(), fri_vector.end(), relationChgInfo.RoleID );
		assert( fri_iter != fri_vector.end() );
		fri_iter->m_nRelationType = relationChgInfo.RelationType;
		for ( int j = 0; j < relationChgInfo.RelationAttrNum; ++j )
		{
			const RELATIONATTR& relationAttr = relationChgInfo.RelationAttrs[j];
			setRoleAttr( *fri_iter, relationAttr.RelationAttrID, &relationAttr.RelationAttrData );
		}

		const tagFriendInfo& friendInfo = *fri_iter;
		UpdateScenePlayer( friendInfo.m_szName );
	}
	GameManager::getSingleton().SendUIEvent( "GE_FRIEND_GETLIST" );
}

void GameFriendManager::UpdateScenePlayer( const char* pszName )
{
	if ( pszName == NULL || pszName[0] == '\0' )
	{
		return;
	}

	ActorManager& actorMgr	= ActorManager::getSingleton();
	GameActor* pActor		= actorMgr.findActorByName( pszName ); 
	if ( pActor == NULL )
	{
		return;
	}

	int nRelationType = getPlayerRelation( pszName );
	pActor->setRelationType( nRelationType );
}

void GameFriendManager::RequestWatchFriend( const char* szName )
{
	printf( "request watch player %s\n",szName );
	tagCSPkgBody msgbody;
	strcpy( msgbody.WatchFriendClt.FriendName, szName );
	SendGameServerMsg( WATCH_FRIEND_CLT, msgbody );
}

void GameFriendManager::HandleMarryInfo( const CSINVOLVEMARRYSVR& InvolveMarrySvr )
{
	switch( InvolveMarrySvr.Type )
	{
	case TYPE_INTIMATE_COLOR_SVR:
		{
			FRI_VECTOR& fri_vector	= m_FriendMap[CLIENT_RELATION_TYPE_FRIEND];
			FRI_VECTOR_ITER iter = std::find( fri_vector.begin(), fri_vector.end(), InvolveMarrySvr.InvolveMarrySvrData.IntimateColorSvr.RoleID );
			if ( iter != fri_vector.end() )
			{
				iter->m_nIntimateColorType = InvolveMarrySvr.InvolveMarrySvrData.IntimateColorSvr.IntimateColorType;
				MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
				if( InvolveMarrySvr.InvolveMarrySvrData.LoginIntimateColor.IntimateColorType > 0 )
				{
					pMain->m_nLoveColorType = InvolveMarrySvr.InvolveMarrySvrData.LoginIntimateColor.IntimateColorType;
					strcpy(pMain->m_szLoverName,iter->m_szName);
					GameManager::getSingleton().SendUIEvent( "GE_LOVE_INFO_CHANGE" );
				}else if( pMain->m_loverID == InvolveMarrySvr.InvolveMarrySvrData.LoginIntimateColor.RoleID || strcmp( pMain->m_szLoverName, iter->m_szName ) == 0 )
				{
					pMain->m_nLoveColorType = InvolveMarrySvr.InvolveMarrySvrData.LoginIntimateColor.IntimateColorType;
					strcpy(pMain->m_szLoverName,iter->m_szName);
					GameManager::getSingleton().SendUIEvent( "GE_LOVE_INFO_CHANGE" );
				}
			}
			GameManager::getSingleton().SendUIEvent( "GE_FRIEND_GETLIST" );
		}
		break;
	case TYPE_PURSUE_INFO_INFORM_SVR:
		{
			char buffer[256];
			sprintf( buffer, "AddMarryInfo( \"%s\",%d )", InvolveMarrySvr.InvolveMarrySvrData.PursueInfoInformSvr.PursueName, 1 );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
		}
		break;
	case TYPE_REMOVE_RELATION_INFORM_SVR:
		{
			char buffer[256];
			sprintf( buffer, "AddMarryInfo( \"%s\",%d )", InvolveMarrySvr.InvolveMarrySvrData.RemoveRelationSvr.InitiativeName, 2 );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
			MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
			pMain->resetLoveInfo();
		}
		break;
	case TYPE_OATH_UI_SVR:
		{
			InvolveMarrySvr.InvolveMarrySvrData.OathUISvr.Gender;
			char buffer[256];
			sprintf( buffer, "OpenOathUIFrame( %d,\"%s\",\"%s\",%d )", InvolveMarrySvr.InvolveMarrySvrData.OathUISvr.Gender, 
				InvolveMarrySvr.InvolveMarrySvrData.OathUISvr.Name, InvolveMarrySvr.InvolveMarrySvrData.OathUISvr.OrderName,
				InvolveMarrySvr.InvolveMarrySvrData.OathUISvr.WeddingMeal);
			GameManager::getSingleton().getScriptVM()->callString( buffer );
		}
		break;
	case TYPE_LOGIN_INTIMATE_COLOR:
		{
			FRI_VECTOR& fri_vector	= m_FriendMap[CLIENT_RELATION_TYPE_FRIEND];
			FRI_VECTOR_ITER iter = std::find( fri_vector.begin(), fri_vector.end(), InvolveMarrySvr.InvolveMarrySvrData.LoginIntimateColor.RoleID );
			if ( iter != fri_vector.end() )
			{
				iter->m_nIntimateColorType = InvolveMarrySvr.InvolveMarrySvrData.LoginIntimateColor.IntimateColorType;
				MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
				pMain->m_nLoveColorType = InvolveMarrySvr.InvolveMarrySvrData.LoginIntimateColor.IntimateColorType;
				strcpy(pMain->m_szLoverName,iter->m_szName);
				pMain->m_loverID = InvolveMarrySvr.InvolveMarrySvrData.LoginIntimateColor.RoleID;
			}
			GameManager::getSingleton().SendUIEvent( "GE_FRIEND_GETLIST" );
			GameManager::getSingleton().SendUIEvent( "GE_LOVE_INFO_CHANGE" );
		}
		break;
	case TYPE_REMOVE_RELATION_UI:
		{
			char buffer[256];
			sprintf( buffer, "OpenRemoveRelationCostFrame( %d )", InvolveMarrySvr.InvolveMarrySvrData.RemoveRelationUI.IntimateColorType );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
		}
		break;
	case TYPE_CELEBRATION_INFO:
		{
		}
		break;
	}
}

void GameFriendManager::ActToPursue( const char* szPursueName, int nFlag )
{
	tagCSPkgBody msgbody;
	msgbody.RelationClt.OpType = RELATION_OP_TYPE_INVOLVE_MARRY_CLT;
	msgbody.RelationClt.RelationCltData.InvolveMarryClt.Type = TYPE_PURSUE_RES_CLT;
	strcpy( msgbody.RelationClt.RelationCltData.InvolveMarryClt.InvolveMarryCltData.PursueResClt.PursueName, szPursueName );
	msgbody.RelationClt.RelationCltData.InvolveMarryClt.InvolveMarryCltData.PursueResClt.Accept = nFlag;
	SendGameServerMsg( RELATION_CLT, msgbody );
}

void GameFriendManager::ActToMarryOath( const char* szName, int nFlag )
{
	tagCSPkgBody msgbody;
	msgbody.RelationClt.OpType = RELATION_OP_TYPE_INVOLVE_MARRY_CLT;
	msgbody.RelationClt.RelationCltData.InvolveMarryClt.Type = TYPE_OATH_RES_CLT;
	strcpy( msgbody.RelationClt.RelationCltData.InvolveMarryClt.InvolveMarryCltData.OathResClt.OtherName, szName );
	msgbody.RelationClt.RelationCltData.InvolveMarryClt.InvolveMarryCltData.OathResClt.Accept = nFlag;
	SendGameServerMsg( RELATION_CLT, msgbody );
}

void GameFriendManager::ActToRemoveRelationCost( const char* szName)
{
	tagCSPkgBody msgbody;
	msgbody.RelationClt.OpType = RELATION_OP_TYPE_INVOLVE_MARRY_CLT;
	msgbody.RelationClt.RelationCltData.InvolveMarryClt.Type = TYPE_REMOVE_RELATION_REQ_CLT;
	strcpy( msgbody.RelationClt.RelationCltData.InvolveMarryClt.InvolveMarryCltData.RemoveRelationReqClt.ReceiveName, szName );
	SendGameServerMsg( RELATION_CLT, msgbody );
}