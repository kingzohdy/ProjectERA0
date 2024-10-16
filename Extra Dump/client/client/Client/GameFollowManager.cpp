#include <algorithm>
#include <windows.h>
#include "GameFollowManager.h"
#include "GameManager.h"
#include "GameMainPlayer.h"
#include "GameStateAction.h"
#include "OgreScriptLuaVM.h"
#include "GameActorManager.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameExchange.h"
#include "GameCamera.h"
#include "GamePlayManager.h"
#include "ShowActor.h"
#include "OgreCamera.h"
#include "GameMaxMap.h"
using namespace Ogre;

static int s_breakFollowBasicStates[] = { GSACT_DEAD, GSACT_AUTOFLY, GSACT_FLY, GSACT_STALL };

static int s_breakFollowSecondStates = GSACT_IN_TETANIC;

static int s_unStartFollowStates[] = { GSACT_SIT, GSACT_DEAD, GSACT_STALL };

GameFollowManager::GameFollowManager() : m_nPreMapID( 0 )
{	
	INetworkManager::getSingleton().RegisterMsgHandler(WEI_XING_SVR, this);
}

GameFollowManager::~GameFollowManager()
{}

void GameFollowManager::beginWeiXing( const char* pszName, int nMemId, int nFollowType )
{
	if ( !canStartFollow() )
	{
		return;
	}
	
	tagCSPkgBody msgbody;
	memset( &msgbody, 0, sizeof(tagCSPkgBody) );
	CSWEIXINGCLT& weiXingRequireData	= msgbody.WeiXingClt;
	weiXingRequireData.Flag				= PLAYER_WEIXING;
	strncpy( weiXingRequireData.RoleName, pszName, NAME_LEN );
	//weiXingRequireData.CapInvite		= nFollowType;
	weiXingRequireData.Type				= nFollowType;
	weiXingRequireData.MemID			= nMemId;
	m_strPlayerName						= pszName;
	//m_uLastTime						= timeGetTime();
	m_nMemID							= nMemId;
	SendGameServerMsg( WEI_XING_CLT, msgbody );
}

void GameFollowManager::cancelWeiXing( )
{
	tagCSPkgBody msgbody;
	memset( &msgbody, 0, sizeof(tagCSPkgBody) );
	CSWEIXINGCLT& weiXingRequireData	= msgbody.WeiXingClt;
	weiXingRequireData.Flag				= PLAYER_UNWEIXING;
	if ( !m_strPlayerName.empty() )
	{
		strncpy( weiXingRequireData.RoleName, m_strPlayerName.c_str(), NAME_LEN );
		SendGameServerMsg( WEI_XING_CLT, msgbody );
	}
	else
	{
		if ( m_nMemID != 0 )
		{
			weiXingRequireData.Type		= 1;
			weiXingRequireData.MemID	= m_nMemID;
			SendGameServerMsg( WEI_XING_CLT, msgbody );
		}		
	}
	m_strPlayerName.clear();
	m_nPreMapID	= 0;
	m_nMemID = 0;
	//m_uLastTime	= 0;	

	char buff[256];
	sprintf( buff, "CancelWeiXing()" );
	GameManager::getSingleton().getScriptVM()->callString( buff );
}

void GameFollowManager::appealWeiXing()
{
	tagCSPkgBody msgbody;
	CSWEIXINGCLT& weiXingRequireData = msgbody.WeiXingClt;
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	//weiXingRequireData.MemID	= pMainPlayer->GetID();
	strncpy( weiXingRequireData.RoleName, pMainPlayer->getName(), NAME_LEN );
	weiXingRequireData.Flag		= PLAYER_APPEAL_WEIXING;
	SendGameServerMsg( WEI_XING_CLT, msgbody );
}

bool GameFollowManager::isWeiXing()
{
	return !m_strPlayerName.empty() || m_nMemID != 0;
}

bool GameFollowManager::isWeiXingTarget( const char* pszName, int nMemId )
{
	if ( sizeof( pszName ) != 0 && nMemId == 0 )
	{
		return pszName == m_strPlayerName;
	}
	if ( sizeof( pszName ) == 0 && nMemId != 0 )
	{
		return nMemId == m_nMemID;
	}
	return false;
}

const char*	GameFollowManager::weiXingTarget()
{
	if ( sizeof( m_strPlayerName ) != 0 && m_nMemID == 0 )
	{
		return m_strPlayerName.c_str();
	}
	return "";
}

bool GameFollowManager::canStartFollow()
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	int size = sizeof( s_unStartFollowStates ) / sizeof( int );
	int* unStartFollowEnd = s_unStartFollowStates + sizeof( s_unStartFollowStates ) / sizeof( int );
	if ( std::find( s_unStartFollowStates, unStartFollowEnd, pMainPlayer->getActState().GetState() ) != unStartFollowEnd )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_FOLLOW_START_FAIL );
		return false;
	}

	if ( GameExchange::getSingleton().isInExchange() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_FOLLOW_START_FAIL );
		return false;
	}

	return true;
}

void GameFollowManager::resetFollowInfo()
{
	m_nPreMapID	= 0;
	//m_uLastTime	= 0;
	m_strPlayerName.clear();
	m_nMemID = 0;
}

void GameFollowManager::drawFollowFrame( )
{
	/*FollowCamera *pcamera = GamePlayManager::getSingleton().getMainCamera();

	if( pcamera == NULL || pcamera->isPlayCameraAnim() )
	{
		return;
	}
	
	MainGamePlayer*		pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	Ogre::ShowActor*	pShowActor	= pMainPlayer->getShowActor();
	if ( pShowActor == NULL )
	{
		return;
	}

	Ogre::Vector3 pos = pShowActor->getCurEntity()->getWorldPosition();
	pos.y += pShowActor->getActorHeight() + 100;
	Ogre::Vector3 scrpos;
	pcamera->GetCameraNode()->pointWorldToWindow( scrpos, pos );

	if ( pMainPlayer->m_Rider.isOnHorse())
	{
		assert( pMainPlayer->getRider()->getHorseId() != 0 );

		pos = pShowActor->getCurEntity()->getAnchorWorldPos( 106 );
		pcamera->GetCameraNode()->pointWorldToWindow( scrpos, pos );
	}
	
	char buffer[60];
	sprintf( buffer, "UpdateFollowFramePos( %f, %f )", scrpos.x, scrpos.y );*/
	char buffer[60];
	sprintf( buffer, "UpdateFollowFramePos()" );
	GameManager::getSingleton().getScriptVM()->callString( buffer );
}

int GameFollowManager::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if ( msgid != WEI_XING_SVR )
	{
		return 0;
	}

	const CSWEIXINGSVR &followDest = msgbody.WeiXingSvr;
	char buff[256];
	if ( followDest.WeiXingOp == WEIXING_SVR_POS )
	{
		// 若已经取消尾行，则不再接受由于网络延迟造成而迟收到的尾行网络包
		if ( m_strPlayerName.empty() && m_nMemID == 0 )
		{
			return 0 ;
		}

		// 若被跟随的目标玩家处于停止状态，则将停止距离靠远点
		GameActor* pActor = ActorManager::getSingleton().findActorByName( m_strPlayerName.c_str() );
		const WEIXINGPOS& pos = followDest.WeiXingData.WeiXingPos;
		// 若是跟随目标位于无线的就可以跟随

		int nSelfMapLine = GameManager::getSingleton().getGameMapShower()->getLineNo();
		if ( nSelfMapLine != 0 && pos.MapLine != 0 && nSelfMapLine != pos.MapLine )
		{
			cancelWeiXing();
			return 0;
		}

		if ( pActor != NULL && pActor->getActState().GetState() == GSACT_STAND )
		{
			sprintf( buff, "Follow( %d, %d, %d, %d, %d )", pos.Map, m_nPreMapID, pos.X, pos.Y, 150 );
		}
		else
		{
			sprintf( buff, "Follow( %d, %d, %d, %d, %d )", pos.Map, m_nPreMapID, pos.X, pos.Y, 0 );
		}

		m_nPreMapID	= pos.Map;
		GameManager::getSingleton().getScriptVM()->callString( buff );
		drawFollowFrame();
		//m_uLastTime = timeGetTime();
	}
	else if ( followDest.WeiXingOp == WEIXING_SVR_APPEAL )
	{
		sprintf( buff, "Appeal( [===[%s]===] )", followDest.WeiXingData.WeiXingAppeal.RoleName );
		GameManager::getSingleton().getScriptVM()->callString( buff );
	}
	else if ( followDest.WeiXingOp == WEIXING_DESTROY )
	{
		if ( isWeiXing() )
		{
			GameMsgPrint::getSingleton().showMidTip( MID_FAIL_WEI_XING );
			cancelWeiXing();
		}
	}

	return 0;
}

void GameFollowManager::update(unsigned int dtick)
{
	MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer == NULL )
	{
		return;
	}

	if ( m_strPlayerName.empty() && m_nMemID == 0 )
	{
		return;
	}

	// 检查主角是否处在不可以进行跟随操作的状态
	if ( needCancelFollow() )
	{
		cancelWeiXing();
	}
}

bool GameFollowManager::needCancelFollow()
{
	MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	int* unFollowEnd = s_breakFollowBasicStates + sizeof( s_breakFollowBasicStates ) / sizeof( int );
	if ( std::find( s_breakFollowBasicStates, unFollowEnd, pMainPlayer->getActState().GetState() ) != unFollowEnd )
	{
		return true;
	}

	if ( pMainPlayer->getInState( s_breakFollowSecondStates ) )
	{
		return true;
	}

	/*if ( timeGetTime() - m_uLastTime >= CLIENT_MAX_FOLLOW_MICROSECOND_INTERVAL )
	{
		return true;
	}*/

	GameActor* pActor = ActorManager::getSingleton().findActorByName( m_strPlayerName.c_str() );
	if( pActor != NULL && pActor->getActState().GetState() == GSACT_AUTOFLY )
	{
		return true;
	}

	return false;
}