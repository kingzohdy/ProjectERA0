#include "GameActor_Rider.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "ShowActor.h"
#include "GameMap.h"
#include "ShowGameMap.h"
#include "NetMessage.h"
#include "ServerTimer.h"
#include "OgreScriptLuaVM.h"
#include "GameMap.h"
#include "OgreVector3.h"
#include "OgreGameScene.h"
#include "GameMsgPrint.h"
#include "GameSkill_Mgr.h"
#include "WorldCity.h"
#include "OgreResourceManager.h"
#include <algorithm>
#include <time.h>

const tagRideDef_ToLua*	getRideDef( int nId, int nLv )
{
	static_assert( sizeof( tagRideDef_ToLua ) == sizeof( tagRideDef ) );
	return ( (const tagRideDef_ToLua*)(DBDataManager::getSingleton().getRideDef( nId, nLv ) ) );
}

// 是否是飞行道具
bool isFlyItem( const tagRideInfo& rideInfo )
{
	const tagRideDef* pRideDef = DBDataManager::getSingleton().getRideDef( rideInfo.RideID, rideInfo.nLevel );
	if ( pRideDef == NULL )
	{
		return false;
	}
	return pRideDef->RideType == RIDE_TYPE_FLY;
}

bool tagRideInfo::isValidEnd()
{	
	if( Endtime > 0 )
	{
		int sec, usec;
		ServerTimer::getSingleton().getServerTime(sec,usec);
		if( sec >= Endtime )
		{
			return true;
		}
	}
	return false;
}

Rider::Rider(void) : m_pShowActor( NULL ), 
					 m_pOwner( NULL ), 
					 m_nMaxRideNum( 0 ), 
					 m_nRideNum( 0 ), 
					 m_nHorseId( 0 ),
					 m_curMap( NULL ),
					 m_bBind( false ),
					 m_bIsContinueAutoFind( false )

{
	memset( &m_UsingRider, 0, sizeof( m_UsingRider ) );
}

Rider::~Rider(void)
{
	if( m_pShowActor != NULL )
	{	
		OGRE_RELEASE(m_pShowActor);
		m_pShowActor = NULL;
	}
}

void Rider::setOwner( GameActor* pActor )
{
	m_pOwner = pActor;
}

// 注册网络消息
void Rider::initRiderNetMessage()
{
	INetworkManager::getSingleton().RegisterMsgHandler( RIDE_SVR, this );
}

void Rider::releaseRiderNetMessage()
{
	INetworkManager::getSingleton().unRegisterMsgHander( RIDE_SVR );
}

int Rider::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if ( msgid == RIDE_SVR )
	{
		const CSRIDESVR& rideSvr = msgbody.RideSvr;
		switch ( rideSvr.Type )
		{
		case RIDE_SVR_ACCEPT:	/* 申请上坐骑信息转发 */
			{
				const RIDEAPPLYINFO& applyRoleInfo = rideSvr.Data.ApplyInfo;
				m_applyRole.m_nMemID = applyRoleInfo.MemID;
				strncpy( m_applyRole.m_szRoleName, applyRoleInfo.RoleName, NAME_LEN );
				GameMsgPrint::getSingleton().SendUIEvent( "GE_RIDE_APPLY" );
			}
			break;

		case RIDE_SVR_INVITE:	/* 邀请上坐骑信息转发 */
			{
				ActorManager& actorManager = ActorManager::getSingleton();
				const RIDEAPPLYINFO& inviteRoleInfo = rideSvr.Data.InviteInfo;
				if( actorManager.getGameOpt( GAME_OPT_REFUSE_RIDE_INVITE ) )
				{
					refuseInviteRide( inviteRoleInfo.MemID, RIDE_REFUSE_CANCEL );
				}
				else
				{
					m_inviteRole.m_nMemID = inviteRoleInfo.MemID;
					strncpy( m_inviteRole.m_szRoleName, inviteRoleInfo.RoleName, NAME_LEN );
					char buffer[256];
					sprintf( buffer, "GetInviteRide( %d )",  m_inviteRole.m_nMemID );
					//GameMsgPrint::getSingleton().SendUIEvent( "GE_RIDE_INVITE" );
					ActorManager::getSingleton().getScriptVM()->callString(buffer);
				}
			}
			break;

		default:
			assert( false );
			break;
		}
	}

	return 0 ;
}

// 骑乘
void Rider::onHorse( int nHorseId, int nBindId, bool bPlayRideEffect )
{
	m_pOwner->m_Equip.hideWeapon();

	if( m_pShowActor == NULL )
	{
		m_pShowActor = new Ogre::ShowActor(nHorseId, false);
		if( m_pOwner->isInFly() )
		{
			m_pShowActor->setSnapToMap( false );
		}
		m_pShowActor->update(0);
	}
	if( m_bBind )
	{
		return;
	}
	m_bBind = true;
	
	ActorManager& actorMgr		= ActorManager::getSingleton(); 
	MainGamePlayer* pMainplayer = actorMgr.getMainPlayer();
	int nHideOtherPlayerOption	= actorMgr.getShowOtherPlayerModelOption();

	if( m_pOwner->getCurMap() != NULL )
	{
		m_pShowActor->setFaceTo( m_pOwner->getAngle() );
		m_pShowActor->setPosition( m_pOwner->GetPosition().x, m_pOwner->GetPosition().y, m_pOwner->GetPosition().z );
		Ogre::ShowGameMap* pshowmap = m_pOwner->getCurMap()->getShowGameMap();
		if (m_pOwner->getShowActor()->isInMap())
		{
			m_pOwner->getShowActor()->onLeaveMap(pshowmap);
			m_pShowActor->onEnterMap(pshowmap);
		}
		if ( nHideOtherPlayerOption != HIDE_MODEL_NONE && m_pOwner->getType() == GAT_PLAYER &&
				!pMainplayer->isInSameTeam( m_pOwner->getTeamWID() ) )
		{
			m_pShowActor->onLeaveMap(pshowmap);
		}
		m_curMap = m_pOwner->getCurMap();
	}
	
	int nAct = DBDataManager::getSingleton().GetNpcDisplayInfo( nHorseId ).rideact;
	int resid = m_pOwner->getResId();
	const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resid);
	m_pShowActor->getCurEntity()->setTransparent(m_pOwner->getShowActor()->getCurEntity()->getTransparent() / info.alpha);
	m_pOwner->getShowActor()->getCurEntity()->setTransparent(info.alpha);
	m_pShowActor->bindActor( nBindId, m_pOwner->getShowActor() );
	if ( bPlayRideEffect )
	{
		const char*	szEffName[1];
		szEffName[0] = "9512_16";
		m_pShowActor->playAction( 10123, szEffName, 1, 10123 );
		//m_pOwner->playMotion("9512_16", 10123);
	}	
	if( nAct == 0 )
	{
		m_pOwner->playAction( 10111, NULL, 0, 0 );
	}else{
		m_pOwner->playAction( nAct, NULL, 0, 0 );
	}
	if( !m_pOwner->isInFly() )
	{
		horsePlayAct( m_pOwner->getActState().getActionId(true), NULL, 0, 0 );
	}

	m_nCurBindingId	= nBindId;
	m_nHorseId		= nHorseId;

	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( m_pOwner == pMainPlayer )
	{
		GameMsgPrint::getSingleton().SendUIEvent( "GE_MAIN_RIDE_CHG" );
		GameMsgPrint::getSingleton().SendUIEvent( "GE_MAIN_RIDE_ON" );
	}

	if ( m_pOwner->GetID() == pMainPlayer->GetSelectTarget() )
	{
		GameMsgPrint::getSingleton().SendUIEvent( "GE_TARGET_RIDE_CHG" );
	}

	if ( m_nHorseId == 5500005 )
	{
		m_pOwner->getShowActor()->getCurEntity()->setTransparent( 0.0f );
		m_pOwner->getRoleText().setEnableDrawTitle(false);
	}
	if (m_pOwner->GetID() == pMainPlayer->m_DriverId)
	{
		m_pOwner->m_Rider.onHorseSide( pMainPlayer->GetID() );
	}
	m_pShowActor->setFootprint( m_pShowActor->getCurEntity(),m_pOwner,true);
}

void Rider::onHorseWithoutBind(int nHorseId,int nBindId/* =200 */)
{
	if( m_pShowActor == NULL )
	{
		m_pShowActor = new Ogre::ShowActor(nHorseId, false);
		m_pShowActor->update(0);
	}
	const char*	szEffName[1];
	szEffName[0] = "9512_16";
	m_pShowActor->playAction( 10123, szEffName, 1, 10123 );
	Ogre::ShowGameMap* pshowmap = m_pOwner->getCurMap()->getShowGameMap();
	m_pShowActor->onEnterMap(pshowmap);
	
	ActorManager& actorMgr		= ActorManager::getSingleton(); 
	MainGamePlayer* pMainplayer = actorMgr.getMainPlayer();
	int nHideOtherPlayerOption	= actorMgr.getShowOtherPlayerModelOption();

	if ( nHideOtherPlayerOption != HIDE_MODEL_NONE && m_pOwner->getType() == GAT_PLAYER &&
			!pMainplayer->isInSameTeam( m_pOwner->getTeamWID() ) )
	{
		m_pShowActor->onLeaveMap(pshowmap);
	}

	m_curMap = m_pOwner->getCurMap();

	m_nCurBindingId	= nBindId;
	m_nHorseId		= nHorseId;

	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( m_pOwner == pMainPlayer )
	{
		GameMsgPrint::getSingleton().SendUIEvent( "GE_MAIN_RIDE_CHG" );
	}

	if ( m_pOwner->GetID() == pMainPlayer->GetSelectTarget() )
	{
		GameMsgPrint::getSingleton().SendUIEvent( "GE_TARGET_RIDE_CHG" );
	}

	if (m_nHorseId == 5500005  )
	{
		m_pOwner->getShowActor()->getCurEntity()->setTransparent( 0.0f );
		m_pOwner->getRoleText().setEnableDrawTitle(false);
	}
	m_bBind = false;
}

void Rider::bindOwner()
{
	if( m_bBind )
	{
		return;
	}
	const char*	szEffName[1];
	szEffName[0] = "9512_16";
	m_bBind = true;
	Ogre::ShowGameMap* pshowmap = m_pOwner->getCurMap()->getShowGameMap();
	m_pOwner->getShowActor()->onLeaveMap(pshowmap);
	GameXYZ pos = m_pShowActor->getCurEntity()->getPosition();
	m_pOwner->m_Equip.hideWeapon();
	m_pOwner->SetPositionTM( pos.x,pos.y,pos.z );
	int resid = m_pOwner->getResId();
	const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resid);
	m_pShowActor->getCurEntity()->setTransparent(m_pOwner->getShowActor()->getCurEntity()->getTransparent() / info.alpha);
	m_pOwner->getShowActor()->getCurEntity()->setTransparent(info.alpha);
	m_pShowActor->bindActor( m_nCurBindingId, m_pOwner->getShowActor() );
	m_pShowActor->setFaceTo( m_pOwner->getAngle() );
	m_pShowActor->setPosition( m_pOwner->GetPosition().x, m_pOwner->GetPosition().y, m_pOwner->GetPosition().z );
	
	int nAct = DBDataManager::getSingleton().GetNpcDisplayInfo( m_nHorseId ).rideact;
	if( nAct == 0 )
	{
		m_pOwner->playAction( 10111, NULL, 0, 0 );
	}else{
		m_pOwner->playAction( nAct, NULL, 0, 0 );
	}
}

// 多人骑乘上副座
void Rider::onHorseSide( int nActorId )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( m_pShowActor == NULL || pMainPlayer == NULL )
	{
		assert(0);
		return;
	}
	
	// 此为主人让客人上驾，故m_pOwner一定是坐骑的主人，但是未必为客人
	GameActor* pActor = ActorManager::getSingleton().FindActor( nActorId );
	if ( pActor == NULL )
	{
		return;
	}

	if( pActor != NULL )
	{
		pActor->m_Equip.hideWeapon();
		Ogre::ShowGameMap* pshowmap = m_pOwner->getCurMap()->getShowGameMap();
		pActor->getShowActor()->onLeaveMap(pshowmap);
		m_pShowActor->bindActor( ++m_nCurBindingId, pActor->getShowActor() );
		const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo( m_nHorseId );
		int nAct = info.passengeract;
		if (nAct == 0)
		{
			nAct = info.rideact;
		}
		if( nAct == 0 )
		{
			pActor->playAction( 10111, NULL, 0, 0 );
		}else{
			pActor->playAction( nAct, NULL, 0, 0 );
		}
		pActor->setInState( GSACT_IN_RIDE, true );
	}

	
	if ( pActor == pMainPlayer )
	{
		GameMsgPrint::getSingleton().SendUIEvent( "GE_MAIN_RIDE_CHG" );
		pMainPlayer->m_DriverId = m_pOwner->GetID();
	}
	else
	{
		GameMsgPrint::getSingleton().SendUIEvent( "GE_ACTOR_RIDE_SIDE" );
	}
	
	if ( pActor->GetID() == pMainPlayer->GetSelectTarget() )
	{
		GameMsgPrint::getSingleton().SendUIEvent( "GE_TARGET_RIDE_CHG" );
	}
}

// 取消骑乘
void Rider::downHorse()
{
	m_pOwner->m_Equip.showWeapon();

	if (m_nHorseId == 5500005  )
	{
		m_pOwner->getShowActor()->getCurEntity()->setTransparent( 1.0f );
		m_pOwner->getRoleText().setEnableDrawTitle(true);
	}

	memset( &m_UsingRider, 0, sizeof( tagRideInfo ) );
	m_nHorseId = 0;
	// 下骑乘的时候，若骑乘的实体存在于场景中，则进行一些清空操作
	if ( m_pShowActor == NULL )
	{
		return;
	}	
	Ogre::ShowGameMap *pshowmap = m_pOwner->getCurMap()->getShowGameMap();

	// 将骑乘的实体与拥有者解除绑定
	m_pShowActor->unbindActor(m_pOwner->getShowActor());
	int resid = m_pOwner->getResId();
	const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resid);
	m_pOwner->getShowActor()->getCurEntity()->setTransparent(m_pShowActor->getCurEntity()->getTransparent() * info.alpha);
	m_pShowActor->getCurEntity()->setTransparent(1.0f);
	m_pOwner->getShowActor()->setPosition(m_pOwner->GetPosition().x, m_pOwner->GetPosition().y, m_pOwner->GetPosition().z);
	m_pOwner->getShowActor()->setFaceTo(m_pOwner->getAngle());

	// 让骑乘实体离开当前地图
	if (m_pShowActor->isInMap())
	{
		m_pShowActor->onLeaveMap(m_pOwner->getCurMap()->getShowGameMap());
		m_pOwner->getShowActor()->onEnterMap(pshowmap);
	}

	OGRE_RELEASE(m_pShowActor);
	m_pShowActor	= NULL;

	ActorManager& actorMgr		= ActorManager::getSingleton(); 
	MainGamePlayer* pMainplayer = actorMgr.getMainPlayer();
	int nHideOtherPlayerOption	= actorMgr.getShowOtherPlayerModelOption();

	if ( nHideOtherPlayerOption != HIDE_MODEL_NONE && m_pOwner->getType() == GAT_PLAYER &&
			!pMainplayer->isInSameTeam( m_pOwner->getTeamWID() ) )
	{
		if ( !( nHideOtherPlayerOption == HIDE_OTHER_PLAYER_AND_PET && m_pOwner->isInStall()  ) )
		{
			m_pOwner->getShowActor()->onLeaveMap(pshowmap);
		}
	}	

	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( m_pOwner == pMainPlayer )
	{
		GameMsgPrint::getSingleton().SendUIEvent( "GE_MAIN_RIDE_CHG" );
	}
	
	if ( m_pOwner->GetID() == pMainPlayer->GetSelectTarget() )
	{
		GameMsgPrint::getSingleton().SendUIEvent( "GE_TARGET_RIDE_CHG" );
	}
	m_bBind = false;
}

// 乘客下马
void Rider::downHorseSide()
{
	m_pOwner->m_Equip.showWeapon();
	m_pOwner->setInState( GSACT_IN_RIDE, false );

	GameActor* pOwner = ActorManager::getSingleton().FindActor( m_UsingRider.m_nOwnerID );
	Ogre::ShowGameMap *pshowmap = m_pOwner->getCurMap()->getShowGameMap();
	if ( pOwner != NULL )
	{
		if( !pOwner->m_Rider.isOnHorse() )
		{
			return;
		}
		pOwner->m_Rider.m_pShowActor->unbindActor( m_pOwner->getShowActor() );
		--pOwner->m_Rider.m_nCurBindingId;
		if (m_pOwner == ActorManager::getSingleton().getMainPlayer())
		{
			((MainGamePlayer *)m_pOwner)->m_DriverId = 0;
		}
	}

	m_pShowActor = NULL;
	memset( &m_UsingRider, 0, sizeof( tagRideInfo ) );
	//m_pOwner->getShowActor()->setPosition( m_pOwner->GetPosition().x, m_pOwner->GetPosition().y, m_pOwner->GetPosition().z );
	m_pOwner->getShowActor()->setFaceTo( m_pOwner->getAngle() );
	m_pOwner->getShowActor()->onEnterMap( pshowmap );

	ActorManager& actorMgr		= ActorManager::getSingleton(); 
	MainGamePlayer* pMainplayer = actorMgr.getMainPlayer();
	int nHideOtherPlayerOption	= actorMgr.getShowOtherPlayerModelOption();

	if ( nHideOtherPlayerOption != HIDE_MODEL_NONE && m_pOwner->getType() == GAT_PLAYER &&
			!pMainplayer->isInSameTeam( m_pOwner->getTeamWID() ) )
	{
		if ( !( nHideOtherPlayerOption == HIDE_OTHER_PLAYER_AND_PET && m_pOwner->isInStall() ) )
		{
			m_pOwner->getShowActor()->onLeaveMap(pshowmap);
		}
	}	

	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( m_pOwner == pMainPlayer )
	{
		GameMsgPrint::getSingleton().SendUIEvent( "GE_MAIN_RIDE_CHG" );
	}
	
	if ( m_pOwner->GetID() == pMainPlayer->GetSelectTarget() )
	{
		GameMsgPrint::getSingleton().SendUIEvent( "GE_TARGET_RIDE_CHG" );
	}
}

// 在乘客被销毁的时候下马
void Rider::downHorseSideWhenDestroy()
{
	GameActor* pOwner = ActorManager::getSingleton().FindActor( m_UsingRider.m_nOwnerID );
	GameMap* pCurMap = m_pOwner->getCurMap();
	Ogre::ShowGameMap *pshowmap = pCurMap->getShowGameMap();
	if ( pOwner != NULL )
	{
		if( !pOwner->m_Rider.isOnHorse() )
		{
			return;
		}
		--pOwner->m_Rider.m_nCurBindingId;
		if (m_pOwner == ActorManager::getSingleton().getMainPlayer())
		{
			((MainGamePlayer *)m_pOwner)->m_DriverId = 0;
		}
		if ( pCurMap == NULL )
		{
			return;
		}
		pOwner->m_Rider.m_pShowActor->unbindActor( m_pOwner->getShowActor() );		
	}

	m_pShowActor = NULL;
	memset( &m_UsingRider, 0, sizeof( tagRideInfo ) );
}

// 骑乘播放动作
void Rider::horsePlayAct( int seq, const char **pmotions, int num_motion, int motion_class )
{
	if ( m_pShowActor == NULL )
	{
		return;
	}
	m_pShowActor->playAction( seq, pmotions, num_motion, motion_class );
	if( m_pOwner->isInStand() )
	{
		char szBuf[256];
		sprintf(szBuf, "effect\\public\\%d_s.emo", getHorseId());
		if( Ogre::ResourceManager::getSingleton().getFileAttrib(szBuf) != 0 )
		{
			sprintf( szBuf,"%d_s",getHorseId() );
			m_pShowActor->getCurEntity()->playMotion( szBuf,true, 9999);
		}
	}else if( m_pOwner->isInMove() )
	{
		char szBuf[256];
		sprintf(szBuf, "effect\\public\\%d_m.emo", getHorseId());
		if( Ogre::ResourceManager::getSingleton().getFileAttrib(szBuf) != 0 )
		{
			sprintf( szBuf,"%d_m",getHorseId() );
			m_pShowActor->getCurEntity()->playMotion( szBuf,true,9999);
		}
	}
}

// 骑术吟唱
void Rider::MainPlayerRidePre( unsigned int uRideId, int nLevel )
{
	for( int i = 0 ; i < m_nMaxRideNum ; i++ )
	{
		if( m_RideGrids[i].RideID == uRideId && m_RideGrids[i].nLevel == nLevel )
		{
			if( m_RideGrids[i].isValidEnd() )
			{
				const tagRideDef_ToLua* pDef = getRideDef( uRideId,nLevel );
				if( pDef != NULL )
				{
					char buf[256];
					sprintf( buf,"ItemValidEnd(\"%s\")",pDef->RideName );
					ActorManager::getSingleton().getScriptVM()->callString( buf );
				}
				return;
			}
		}
	}
	if ( !canRidePre( uRideId, nLevel ) )
	{
		return;
	}
// 	// 若玩家拿着临时道具，则丢弃临时道具
// 	if ( m_pOwner->m_Equip.isUsedTmp() )
// 	{
// 		m_pOwner->m_Equip.looseTmpArm();
// 	}

	const tagRideDef* pRideDef = DBDataManager::getSingleton().getRideDef( uRideId, nLevel );
	m_nHorseId = pRideDef->ModelID;
	if( m_pOwner->getType() == GAT_MAINPLAYER )
	{
		MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
		int nCurState = GSACT_IN_RIDE;
		tagRideInfo rideInfo(uRideId,0,nLevel);
		if( isFlyItem( rideInfo ) )
		{
			nCurState = GSACT_FLY;
		}
// 		int nChangeResult = m_pOwner->getActState().canChangeState( nCurState );
// 		if( nChangeResult == STATE_UNCHANGE )
// 		{
// 			return;
// 		}

		if ( pMainplayer->getAutoFind() )
		{
			m_bIsContinueAutoFind = true;
		}
		else
		{
			m_bIsContinueAutoFind = false;
		}

// 		if( nChangeResult == STATE_STAND_CHANGE || m_pOwner->getActState().GetState() == GSACT_MOVE )
// 		{
// 			m_pOwner->CmdMoveStop();
// 		}
		
		if( m_pOwner->getInState( GSACT_IN_RIDE ) || m_pOwner->getActState().GetState() == GSACT_FLY )
		{
			if( uRideId == m_UsingRider.RideID )
			{
				requestRideDown();
				return;
			}
			requestRideDown();
		}

		//这里改为直接骑乘，原来代码保留备用
		tagCSPkgBody msgbody;
		msgbody.RideOn.RideItemID = uRideId;
		msgbody.RideOn.RidePos.X	= m_pOwner->GetPosition().x/10;
		msgbody.RideOn.RidePos.Y	= m_pOwner->GetPosition().z/10;
		ServerTimer::getSingleton().getServerTime(msgbody.RideOn.SvrTime.BenchTimeSec, msgbody.RideOn.SvrTime.BenchTimeUSec);
		SendGameServerMsg( RIDE_ON, msgbody ); 
		//tagCSPkgBody msgbody;
		//msgbody.RidePre.RideItemID	= uRideId;
		//msgbody.RidePre.RidePos.X	= m_pOwner->GetPosition().x/10;
		//msgbody.RidePre.RidePos.Y	= m_pOwner->GetPosition().z/10;
		//ServerTimer::getSingleton().getServerTime(msgbody.RidePre.SvrTime.BenchTimeSec, msgbody.RidePre.SvrTime.BenchTimeUSec);
		//SendGameServerMsg( RIDE_PRE, msgbody );
		//pMainplayer->startRideChant( uRideId,nLevel );
	}
}

// 查看是否有客人等待上坐骑
void Rider::checkWaiter()
{
	// 查看自己的客人是否在等待自己出现然后上线
	std::vector<int> waitGuestList;
	ActorManager::getSingleton().popWaitRideList( m_pOwner->GetID(), waitGuestList );
	for ( std::vector<int>::iterator iter = waitGuestList.begin(), end = waitGuestList.end(); iter != end; ++iter )
	{
		onHorseSide( *iter );
	}
}
void Rider::rideOn( unsigned int uRideId, int nRideLevel, bool already/* = false*/ )
{
	if( uRideId == 0 )
	{
		return;
	}

	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( m_pOwner ==  pMainPlayer )
	{
		if ( m_bIsContinueAutoFind )
		{
			ActorManager::getSingleton().getScriptVM()->callString( "ContinueLastAutoFindAfterRide()" );
		}
	}

	bool bPlayRideEffect			= !already;
	if ( m_pOwner != pMainPlayer && m_UsingRider.RideID != 0 && m_UsingRider.RideID == uRideId && 
		m_pOwner->isInRide() )
	{
		bPlayRideEffect = false;
	}

	m_UsingRider.RideID				= uRideId;
	m_UsingRider.nLevel				= nRideLevel;
	m_UsingRider.m_nOwnerID			= m_pOwner->GetID();
	m_UsingRider.m_nPassengerNum	= 0;

	const tagRideDef& rideDef = *(DBDataManager::getSingleton().getRideDef( uRideId, nRideLevel ) );
	if( &rideDef == NULL )
	{
		return;
	}

	m_nHorseId = rideDef.ModelID;
	int rideModleId = rideDef.ModelID;

	if( rideModleId != 0 )
	{
		if ( isFlyItem( m_UsingRider ) )
		{
			onFly( rideModleId );
		}
		else
		{
			m_pOwner->setInState( GSACT_IN_RIDE, true );
			onHorse( rideModleId, 200, bPlayRideEffect );			
		}
	}
	if( m_pOwner == pMainPlayer )
	{
		ActorManager::getSingleton().getScriptVM()->callString( "UpdateRideFrame()" );
	}
	checkWaiter();
}

// 上别人
void Rider::rideOnSide( const tagCSRideOnInfo& rideInfo )
{
	m_UsingRider.m_nOwnerID = rideInfo.OwnerID;
	m_UsingRider.nLevel		= rideInfo.RideLevel;
	m_UsingRider.RideID		= rideInfo.RideID;
	const tagRideDef* pRideDef = DBDataManager::getSingleton().getRideDef( m_UsingRider.RideID, m_UsingRider.nLevel );
	m_nHorseId = pRideDef->ModelID;
	setPassengersInfo( rideInfo.PassengerNum, rideInfo.PassengerID );
	GameActor* pOwner = ActorManager::getSingleton().FindActor( m_UsingRider.m_nOwnerID );
	if( pOwner == NULL )
	{
		ActorManager::getSingleton().pushWaitRideList( rideInfo.OwnerID, m_pOwner->GetID() );
		return;
	}
	
	pOwner->m_Rider.setPassengersInfo( rideInfo.PassengerNum, rideInfo.PassengerID );
	pOwner->m_Rider.onHorseSide( m_pOwner->GetID() );
	/*MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( m_pOwner == pMainPlayer )
	{
		GameMsgPrint::getSingleton().SendUIEvent( "GE_MAIN_RIDE_CHG" );
	}*/
}

// 将所有的乘客T下马
void Rider::kickOffAllPassenger()
{
	for ( unsigned int i = 0; i < m_UsingRider.m_nPassengerNum; ++i )
	{
		GameActor* pGuest = ActorManager::getSingleton().FindActor( m_UsingRider.m_nPassengerID[i] );
		if ( pGuest != NULL )
		{
			pGuest->m_Rider.downHorseSide();
		}
	}
}

// 请求下马
void Rider::requestRideDown()
{
	// 若没有处于骑乘状态，则不能做骑马操作
	if ( m_UsingRider.RideID == 0 )
	{
		return;
	}
	if( m_pOwner->getType() == GAT_MAINPLAYER )
	{
		tagCSPkgBody msgbody;
		msgbody.RideBrk.RideItemID = m_UsingRider.RideID;
		SendGameServerMsg( RIDE_BRK, msgbody );

		if( isFlyItem( m_UsingRider ) )
		{
			downFly();
			return;
		}
		if( isOwnRide() )
		{
			kickOffAllPassenger();
			m_pOwner->setInState( GSACT_IN_RIDE, false );
			downHorse();
		}
		
		ActorManager::getSingleton().getScriptVM()->callString( "UpdateRideFrame()" );
	}
}

void Rider::rideDown()
{
	if ( m_UsingRider.RideID == 0 )
	{
		return;
	}

// 	if ( m_pOwner == ActorManager::getSingleton().getMainPlayer() )
// 	{
// 		if ( m_pOwner->getActState().GetState() == GSACT_MOVE )
// 		{
// 			m_pOwner->CmdMoveStop();
// 		}
// 	}

	if( isFlyItem( m_UsingRider ) )
	{
		downFly();
	}else{
		if( m_UsingRider.m_nOwnerID == m_pOwner->GetID() )
		{
			// 先将所有的乘客T下马
			kickOffAllPassenger();
			downHorse();
		}else{
			downHorseSide();
		}

		m_pOwner->setInState( GSACT_IN_RIDE, false );
	}
	if( m_pOwner->getType() == GAT_MAINPLAYER )
	{
		ActorManager::getSingleton().getScriptVM()->callString( "UpdateRideFrame()" );
	}
}

// 飞行
void Rider::onFly( int nFlyModelId )
{
	m_pOwner->m_Equip.showWeapon();
	if (m_pOwner->getAvatar())
	{
		m_pOwner->getAvatar()->equipItem( POS_ID_BACK, nFlyModelId, 0 );
		m_pOwner->getAvatar()->updateAttrib();
	}
	m_pOwner->getActState().SetState( GSACT_FLY );
	m_pOwner->playAction( m_pOwner->getActState().getActionId(true) );
	m_pOwner->playAction( 10122, NULL, 0, 0 );
}

void Rider::downFly()
{
    int nEquipBackId = m_pOwner->m_Equip.getItem( EQUIP_POS_BACK ).getItemId(); 
	if (m_pOwner->getAvatar())
	{
		m_pOwner->getAvatar()->equipItem( POS_ID_BACK, nEquipBackId, 0 );
		m_pOwner->getAvatar()->updateAttrib();
	}
	m_pOwner->getActState().SetState( GSACT_STAND );
	m_pOwner->playAction( m_pOwner->getActState().getActionId(true) );
	m_pOwner->playAction( 10125, NULL, 0, 0 );
	memset( &m_UsingRider, 0, sizeof(tagRideInfo) ); 
}

// 是否在吟唱
bool Rider::isPre()
{
	return m_pOwner->isInPre( eRide_Chant );
}

// 是否骑马上
bool Rider::isOnHorse()
{
// 	if( m_pShowActor == NULL )
	{
		return m_pOwner->getInState( GSACT_IN_RIDE );
	}
	
	return true;
}

// 是否飞行状态
bool Rider::isOnFly()
{
	if( m_pOwner->getActState().GetState() == GSACT_FLY )
	{
		return true;
	}
	return false;
}

bool Rider::isContinueAutoFind()
{
	return m_bIsContinueAutoFind;
}

// 获取骑乘的骑宠
const tagRideInfo& Rider::getRideItem()
{
	return m_UsingRider;
}

// 获取骑乘最大格子位
int Rider::getMaxRideNum()
{
	return m_nMaxRideNum;
}

// 掌握的骑乘数量
int Rider::getRidNum()
{
	return m_nRideNum;
}


// 丢弃坐骑
void Rider::dropRide( int nRideId )
{
	tagCSPkgBody msgbody;
	msgbody.RideClt.Type = RIDE_DROP;
	msgbody.RideClt.Data.RideDrop.RideItemID = nRideId;
	SendGameServerMsg( RIDE_CLT, msgbody );
}

// 升级坐骑
void Rider::levelUpRide( int nRideId, int nListType, int nGrid )
{
	tagCSPkgBody msgbody;
	CSRIDELEVELUP& rideLevelUp = msgbody.RideClt.Data.RideLevelUp;
	msgbody.RideClt.Type	= RIDE_LEVEL_UP;
	rideLevelUp.RideID		= nRideId;
	rideLevelUp.ListType	= nListType;
	rideLevelUp.GridIdx		= nGrid;
	SendGameServerMsg( RIDE_CLT, msgbody );
}

// 申请乘坐他人坐骑
void Rider::applyRide( int nOwerId )
{
	tagCSPkgBody msgbody;
	msgbody.RideClt.Type = RIDE_APPLY;
	msgbody.RideClt.Data.RideApply.RideOwnerID = nOwerId;
	SendGameServerMsg( RIDE_CLT, msgbody );
}

// 同意他人申请做自己的坐骑
void Rider::acceptApplyRide( int nRideApplyId )
{
	tagCSPkgBody msgbody;
	msgbody.RideClt.Type = RIDE_APPLY_ACCEPT;
	msgbody.RideClt.Data.RideApplyAccpet.RideApplyID = nRideApplyId;
	SendGameServerMsg( RIDE_CLT, msgbody );
	memset( &m_applyRole, 0, sizeof( tagRideRoleInfo ) );
}

// 拒绝他人申请做自己的坐骑
void Rider::refuseApplyRide( int nApplyId )
{
	tagCSPkgBody msgbody;
	msgbody.RideClt.Type = RIDE_APPLY_REFUSE;
	msgbody.RideClt.Data.RideApplyRefuse.RideApplyID = nApplyId;
	SendGameServerMsg( RIDE_CLT, msgbody );
	memset( &m_applyRole, 0, sizeof( tagRideRoleInfo ) );
}

// 邀请乘坐他人共同骑乘
void Rider::inviteRide( int nInvitedId )
{
	tagCSPkgBody msgbody;
	msgbody.RideClt.Type = RIDE_INVITE;
	msgbody.RideClt.Data.RideInvite.RideInviteID = nInvitedId;
	SendGameServerMsg( RIDE_CLT, msgbody );
}

// 同意他人邀请共同骑乘
void Rider::acceptInviteRide( int nRideOwerId )
{
	tagCSPkgBody msgbody;
	msgbody.RideClt.Type = RIDE_INVITE_ACCEPT;
	msgbody.RideClt.Data.RideInviteAccpet.RideOwnerID = nRideOwerId;
	SendGameServerMsg( RIDE_CLT, msgbody );
	memset( &m_inviteRole, 0, sizeof( tagRideRoleInfo ) );
}

// 踢某个乘客下马
void Rider::kickOffPassenger( int nMemId )
{
	tagCSPkgBody msgbody;
	msgbody.RideClt.Type = RIDE_KICK_PASSENGER;
	msgbody.RideClt.Data.RideKick.MemID = nMemId;
	SendGameServerMsg( RIDE_CLT, msgbody );
}

//坐骑等级转移
void Rider::rideMoveLevel( int srcRideID, int desRideID )
{
	tagCSPkgBody msgbody;
	msgbody.RideClt.Type = RIDE_MOVE_LEVEL;
	msgbody.RideClt.Data.MoveLevel.ScrRideID = srcRideID;
	msgbody.RideClt.Data.MoveLevel.DstRideID = desRideID;
	SendGameServerMsg( RIDE_CLT, msgbody );
}

// 拒绝他人邀请共同骑乘
void Rider::refuseInviteRide( int nRideOwerId, int nRefuseType )
{
	tagCSPkgBody msgbody;
	msgbody.RideClt.Type = RIDE_INVITE_REFUSE;
	msgbody.RideClt.Data.RideInviteRefuse.RideOwnerID	= nRideOwerId;
	msgbody.RideClt.Data.RideInviteRefuse.RefuseType	= nRefuseType;
	SendGameServerMsg( RIDE_CLT, msgbody );
	memset( &m_inviteRole, 0, sizeof( tagRideRoleInfo ) );
}

// 骑乘主人NPC操作
void Rider::selectRideNpcOpton( int nSelectType )
{
	tagCSPkgBody msgbody;
	msgbody.RideClt.Type = RIDE_NPC_SELECT;
	msgbody.RideClt.Data.NpcSelect.SelectType = nSelectType;
	SendGameServerMsg( RIDE_CLT, msgbody );
}

// 获取邀请人的信息
const tagRideRoleInfo& Rider::getInviteRoleInfo()
{
	return m_inviteRole;
}

//  获取申请人的信息
const tagRideRoleInfo& Rider::getApplyRoleInfo()
{
	return m_applyRole;
}

// 判断自己是否是坐骑的主人
bool Rider::isOwnRide()
{
	return m_pOwner->GetID() == m_UsingRider.m_nOwnerID;
}

// 判断是否能够使用骑乘
bool Rider::canRidePre( unsigned int uRideId, int nLevel )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	// 若是非主角则无需判断，主角只需看到
	if ( m_pOwner != pMainPlayer )
	{
		return true;
	}

	if ( !pMainPlayer->canUsedSpell() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_RIDE_PRE_FAIL_SOME_STATE );
		return false;
	}

	if( SkillManager::getSingleton().getSpellControler()->getCurSpell() != NULL)
	{
		GameMsgPrint::getSingleton().showMidTip( MID_RIDE_PRE_FAIL_SKILLPRE );
		return false;
	}

	if( pMainPlayer->getActState().GetState() == GSACT_COLLECTITEM )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_RIDE_PRE_FAIL_COLLECT );
		return false;
	}

	// 对公会的flag 处理下
	const RIDEDEF* pRideDef = DBDataManager::getSingleton().getRideDef( uRideId,nLevel );
	if( pRideDef == NULL )
	{
		return false;
	}
	if( pRideDef->ClanLimit == 1 )
	{
		if( !WorldCityManager::getSingleton().IsWorldCityKeeper(m_pOwner) 
			|| m_pOwner->getClanPosition() != CLAN_POS_PRESIDENT )
		{
			GameMsgPrint::getSingleton().showMidTip( MID_RIDE_PRE_FAILED_CLAN_LIMIT_CAPTION );
			return false;
		}
	}else if( pRideDef->ClanLimit == 2 )
	{
		if( !WorldCityManager::getSingleton().IsWorldCityKeeper(m_pOwner) )
		{
			GameMsgPrint::getSingleton().showMidTip( MID_RIDE_PRE_FAILED_CLAN_LIMIT_MEMBER );
			return false;
		}		
	}

	return pMainPlayer->canStartChant( eRide_Chant );
}

// 根据骑乘id获取骑乘的信息
const tagRideInfo* Rider::getRideInfoById( int nRideId )
{
	for ( int i = 0; i < m_nRideNum; ++i )
	{
		if ( m_RideGrids[i].RideID == nRideId )
		{
			return &( m_RideGrids[i] );
		}
	}

	return NULL;
}

tagRideInfo& Rider::getRideInfoByRideId( int nRideId )
{
	for ( int i = 0; i < m_nRideNum; ++i )
	{
		if ( m_RideGrids[i].RideID == nRideId )
		{
			return m_RideGrids[i];
		}
	}
	tagRideInfo nonRideInfo;
	memset( &nonRideInfo, 0, sizeof(tagRideInfo) );
	return nonRideInfo;
}

//获取骑乘品质定义
const tagRidePinZhiDef_ToLua* Rider::getRidePinZhiDef( int nPinzhi )
{
	const tagRidePinZhiDef* pRidePinZhiDef = DBDataManager::getSingleton().getRidePinZhiDef(nPinzhi);
	return (const tagRidePinZhiDef_ToLua*)pRidePinZhiDef;
}

// 获取对应格子的骑乘信息
tagRideInfo& Rider::getRideGridInfo( int nIndex )
{
	return m_RideGrids[nIndex]; 
}

// 是否有对应骑乘
bool Rider::isHaveRide( int nRideId )
{
	if ( nRideId == 0 )
	{
		return false;
	}

	for ( int i = 0; i < MAX_RIDE_GRID; ++i )
	{
		const tagRideInfo& oneRideInfo = m_RideGrids[i];
		if ( oneRideInfo.RideID == nRideId )
		{
			return true;
		}
	}

	return false;
}

// 获取骑乘模型ID
const int Rider::getHorseId() const
{
	/*if ( m_UsingRider.RideID == 0 )
	{
		return 0;
	}

	const tagRideDef* pRideDef = DBDataManager::getSingleton().getRideDef( m_UsingRider.RideID, m_UsingRider.nLevel );
	return pRideDef->ModelID;*/
	return m_nHorseId;
}

// 设置骑乘主人
void Rider::setRideOwner( int nOwnerID )
{
	m_UsingRider.m_nOwnerID = nOwnerID;
}

// 设置骑乘信息
void Rider::setRideInfo( int nNum, int nMaxNum, tagRideInfo* pRiderGridInfo, bool bNewInGame )
{
	bool bGetNewRider = nNum > m_nRideNum;
	tagRideInfo newRideInfo;
	if ( !bNewInGame && bGetNewRider )
	{
		for ( int i = 0; i < nNum; ++i )
		{
			bool bFound = false;
			for ( int j = 0; j < m_nRideNum; ++j )
			{
				if ( pRiderGridInfo[i].RideID == m_RideGrids[j].RideID && 
						pRiderGridInfo[i].nLevel == m_RideGrids[j].nLevel )
				{
					bFound = true;
					break;
				}
			}

			if ( !bFound )
			{
				newRideInfo = pRiderGridInfo[i];
				break;
			}
		}
	}

	m_nRideNum		= nNum;
	m_nMaxRideNum	= nMaxNum;
	memset( m_RideGrids, 0, sizeof(m_RideGrids) );
	memcpy( m_RideGrids, pRiderGridInfo, sizeof(tagRideInfo)*nNum );
	for ( int i = 0; i < MAX_RIDE_GRID; ++i )
	{
		tagRideInfo& oneRider = m_RideGrids[i];
		if ( oneRider.Endtime != 0 )
		{
			time_t sert			= (time_t)oneRider.Endtime;
			struct tm *serTM	=  localtime( &sert );
			strftime( oneRider.m_szFormatEndTime, sizeof( oneRider.m_szFormatEndTime ), "%c", serTM );
		}
		else
		{
			memset( oneRider.m_szFormatEndTime, 0 , sizeof( oneRider.m_szFormatEndTime ) );
		}
	}
	if ( !bNewInGame && bGetNewRider )
	{
		char buffer[256];
		sprintf( buffer, "GetNewRider( %d, %d )", newRideInfo.RideID, newRideInfo.nLevel );
		ActorManager::getSingleton().getScriptVM()->callString( buffer );
	}
	GameMsgPrint::getSingleton().SendUIEvent("GE_GET_RIDE_INFO");
}

// 设置主人乘客列表信息
void Rider::setPassengersInfo( int nPassengerNum, const unsigned int* pPassengers )
{
	m_UsingRider.m_nPassengerNum = nPassengerNum;
	memcpy( m_UsingRider.m_nPassengerID, pPassengers, sizeof( unsigned int ) * MAX_RIDE_PASSENGER );
}

void Rider::Update(unsigned int dtick)
{

	int sec, usec;
	ServerTimer::getSingleton().getServerTime(sec,usec);

	if( m_pOwner->getType() == GAT_MAINPLAYER )
	{
		for( int i = 0 ; i < m_nMaxRideNum ; i++ )
		{
			if( m_RideGrids[i].Endtime > 0 )
			{
				int dt = m_RideGrids[i].Endtime - sec;
				if( dt <= 0 )
				{
					if( m_RideGrids[i].m_nEndLeft > 0 )
					{
						//过期
						char buf[256];
						const tagRideDef_ToLua* pDef = getRideDef( m_RideGrids[i].RideID,m_RideGrids[i].nLevel );
						if( pDef != NULL )
						{
							sprintf(buf,"ItemValidNotify(\"%s\",%d)",pDef->RideName,ITEM_VALID_END);
							ActorManager::getSingleton().getScriptVM()->callString( buf );
						}
						GameMsgPrint::getSingleton().SendUIEvent("GE_RIDELIST_CHG");
						//下坐骑
						if( m_RideGrids[i].RideID == m_UsingRider.RideID && m_RideGrids[i].nLevel == m_UsingRider.nLevel )
						{
							requestRideDown();
						}
					}
				}else if( dt < 10*60  ){
					if(  m_RideGrids[i].m_nEndLeft >= 10*60 )
					{
						//10分钟
						char buf[256];
						const tagRideDef_ToLua* pDef = getRideDef( m_RideGrids[i].RideID,m_RideGrids[i].nLevel );
						if( pDef != NULL )
						{
							sprintf(buf,"ItemValidNotify(\"%s\",%d)",pDef->RideName,ITEM_VALID_10M);
							ActorManager::getSingleton().getScriptVM()->callString( buf );
						}
					}
				}else if( dt < 3*60*60 ){
					if( m_RideGrids[i].m_nEndLeft >= 3*60*60 )
					{
						//剩3小时提示
						char buf[256];
						const tagRideDef_ToLua* pDef = getRideDef( m_RideGrids[i].RideID,m_RideGrids[i].nLevel );
						if( pDef != NULL )
						{
							sprintf(buf,"ItemValidNotify(\"%s\",%d)",pDef->RideName,ITEM_VALID_3H);
							ActorManager::getSingleton().getScriptVM()->callString( buf );
						}
					}
				}else if( dt < 24*60*60 ){
					if( m_RideGrids[i].m_nEndLeft >= 24*60*60 )
					{
						//24小时提示
						char buf[256];
						const tagRideDef_ToLua* pDef = getRideDef( m_RideGrids[i].RideID,m_RideGrids[i].nLevel );
						if( pDef != NULL )
						{
							sprintf(buf,"ItemValidNotify(\"%s\",%d)",pDef->RideName,ITEM_VALID_24H);
							ActorManager::getSingleton().getScriptVM()->callString( buf );
						}
					}
				}
				m_RideGrids[i].m_nEndLeft = dt;
			}		
		}
	}

	GameActor* pRideOwner = ActorManager::getSingleton().FindActor( m_UsingRider.m_nOwnerID );
	// 若我当前处于骑乘，且不是骑乘的主人，则将我自己的位置同主人同步
	if ( pRideOwner != NULL && m_UsingRider.m_nOwnerID != m_pOwner->GetID() )
	{
		m_pOwner->SetPosition(  pRideOwner->GetPosition().x / 10, pRideOwner->GetPosition().z / 10 );
	}

	if ( m_pShowActor == NULL || !m_pShowActor->isInMap() )
	{
		return;
	}
	
	if( m_curMap != m_pOwner->getCurMap() )
	{
		m_pShowActor->onEnterMap( m_pOwner->getCurMap()->getShowGameMap() );
		m_curMap = m_pOwner->getCurMap();
	}

	if( m_pOwner->getActState().GetState() != GSACT_AUTOFLY )
	{
		m_pShowActor->setFaceTo( m_pOwner->getAngle() );
		m_pShowActor->setPosition( m_pOwner->GetPosition().x, m_pOwner->GetPosition().y, m_pOwner->GetPosition().z );
	}else{
		if( m_pOwner->getSpecialMove()->getType() == MOVETYPE_FLY )
		{
			m_pShowActor->setFaceTo( m_pOwner->getAngle() );
		}
		if( m_bBind )
		{
			m_pShowActor->setPosition( m_pOwner->GetPosition().x, m_pOwner->GetPosition().y, m_pOwner->GetPosition().z );
		}
	}
	m_pShowActor->update( dtick );
}

void Rider::leaveMap()
{
	if( m_pShowActor != NULL )
	{
		m_pShowActor->onLeaveMap( m_pOwner->getCurMap()->getShowGameMap() );
		m_curMap = NULL;
	}
}

int Rider::getPassengerNum()
{
	return m_UsingRider.m_nPassengerNum;
}