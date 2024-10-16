#include "GameSkill_Effect.h"
#include "OgreEntity.h"
#include "GameMap.h"
#include "ShowGameMap.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "ShowActor.h"
#include "OgreEntity.h"
#include <Windows.h>

using namespace Ogre;

GameSpellEffect::GameSpellEffect(int type, const char* respath, GameActor* pOwner ) 
				:	m_Type( type ), 
					m_State( STATE_DEAD ), 
					m_bBindingObj( false ), 
					m_BindingObjId( 0 ), 
					m_nSpeed( 0.0f ), 
					m_uLinkTime( 0 ),
					m_pCurMap( NULL ) 
{
	m_pEntity = new Entity;
	if( ActorManager::getSingleton().getMainPlayer() != NULL )
	{
		if( pOwner != NULL && pOwner->getType() != GAT_PLAYER )
		{
			m_pEntity->setPostSceneRenderer(
				ActorManager::getSingleton().getPostSceneRenderer() );
		}
	}
	m_pEntity->load( respath, true );
}

// 创建链状特效
GameSpellEffect::GameSpellEffect( const char* szMontionName, int nMontionClass )
{
	m_pEntity = NULL;
}

GameSpellEffect::~GameSpellEffect()
{
	if( m_pEntity == NULL ) return;
	if( m_bBindingObj )		return;
	
	GameActor* pActor = ActorManager::getSingleton().getMainPlayer();
	if( pActor != NULL )
	{
		m_pEntity->detachFromScene();
		m_pEntity->release();
		m_pEntity = NULL;
	}
}

void GameSpellEffect::start()
{
	m_State		= STATE_DELAY;
	m_Life		= 0;
	m_nPerLift	= 0;
}

void GameSpellEffect::update(int dtick)
{
	if( m_State == STATE_DEAD )
	{
		return;
	}

	m_Life += dtick;
	if(m_State == STATE_DELAY)
	{
		if(m_Life >= m_DelayTick) m_State = STATE_IN;
	}

	if(m_State == STATE_IN)
	{
		if(m_Life >= m_DelayTick+m_InTick) m_State = STATE_LIVE;
	}

	if(m_State == STATE_LIVE)
	{
		if( m_Life >= m_DelayTick+m_InTick+m_LiveTick && m_LiveTick != -1 ) m_State = STATE_OUT;
	}

	if(m_State == STATE_OUT)
	{
		if( m_Life >= m_DelayTick+m_InTick+m_LiveTick+m_OutTick ) m_State = STATE_DEAD;
	}

	if( m_pEntity != NULL && !m_bBindingObj )
	{
		m_pEntity->update( dtick );
	}
}

bool GameSpellEffect::isDead()
{
	return m_State==STATE_DEAD;
}

void GameSpellEffect::EnterMap( GameMap *pmap )
{
	m_pCurMap = pmap;
	m_pEntity->update(0);
	m_pEntity->attachToScene( pmap->getShowGameMap()->getGameScene() );
}

void GameSpellEffect::setStartPos( const Ogre::WorldPos& pos )
{
	m_startPos = pos;
	m_pEntity->setPosition( pos );
}

void GameSpellEffect::setEndPos( const Ogre::WorldPos& pos )
{
	m_endPos = pos;
}

void GameSpellEffect::setLifeTick( int delaytick, int intick, int outtick, int livetick, int speed )
{
	m_DelayTick = delaytick;
	m_InTick	= intick;
	m_OutTick	= outtick;
	m_LiveTick	= livetick;
	m_nSpeed	= (float)speed/1000;
}

void GameSpellEffect::setLifeTick(int livetick)
{
	setLifeTick(0, 0, 0, livetick);
}

void GameSpellEffect::bindingObj( int ObjId, int pos )
{
	GameActor* pActor = ActorManager::getSingleton().FindActor( ObjId );
	if( pActor != NULL )
	{
		pActor->getShowActor()->getCurEntity()->bindObject( pos, m_pEntity );
		m_BindingObjId = ObjId;
		m_bBindingObj = true;
		m_State	= STATE_DELAY;
	}	
}

void GameSpellEffect::unBingdingObj()
{
	GameActor* pActor = ActorManager::getSingleton().FindActor( m_BindingObjId );
	if( pActor != NULL )
	{
		pActor->getShowActor()->getCurEntity()->unbindObject( m_pEntity );
		m_State	= STATE_DEAD;
	}
}

void GameSpellEffect::setFaceTo( int from_x, int from_y, int to_x, int to_y )
{
	if( m_pEntity != NULL )
	{
		int nAngle = FaceAngleBetween( from_x, from_y, to_x, to_y );
		m_pEntity->setRotation( nAngle - 90.0f, 0, 0 );
	}
}

void GameSpellEffect::setRotation( int nAngle )
{
	if( m_pEntity != NULL )
	{
		m_pEntity->setRotation( nAngle - 90.0f, 0, 0 );
	}
}
