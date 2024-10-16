#include "GameSCTDamageManager.h"
#include "GameActorManager.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "GameMsgPrint.h"
#include <algorithm>
using namespace std;

extern bool isNormalSkill( int nSkillID, int nSkillLevel );

SCTDamageManager::SCTDamageManager()
{	
	m_activeSCT[GAT_MAINPLAYER]		= new MainplayerAttack;
	m_activeSCT[GAT_PET]			= new MainPetAttack;

	m_passiveSCT[GAT_MAINPLAYER]	= new MainplayerHurted;
	m_passiveSCT[GAT_PET]			= new MainplayerHurted;
}

SCTDamageManager::~SCTDamageManager()
{
	for ( std::map<int, SCTDamageBase*>::iterator iter = m_activeSCT.begin(), end = m_activeSCT.end();
			iter != end; ++iter )
	{
		SAFE_DELETE( iter->second );
	}
	
	for ( std::map<int, SCTDamageBase*>::iterator iter = m_passiveSCT.begin(), end = m_passiveSCT.end();
		iter != end; ++iter )
	{
		SAFE_DELETE( iter->second );
	}
}

SCTDamageBase*	SCTDamageManager::getActiveSCT( int id )
{	
	GameActor* pActor = ActorManager::getSingleton().FindActor( id );
	if ( pActor == NULL )
	{
		return NULL;
	}

	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( pActor == pMainplayer )
	{
		return m_activeSCT[GAT_MAINPLAYER];
	}

	// 若是怪物且为策划要求特别处理的怪物子类型，则按主角攻击来处理
	if ( pActor->getType() == GAT_MONSTER )
	{
		const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef( pActor->getResId() );
		if ( monsterDef.SubType == MON_SUB_TYPE_TMP )
		{
			return m_activeSCT[GAT_MAINPLAYER];
		}
	}

	if ( pActor->getOwnerId() == pMainplayer->GetID() )
	{
		return m_activeSCT[GAT_PET];
	}

	return NULL;
}

SCTDamageBase*	SCTDamageManager::getPassiveSCT( int id )
{
	std::set<int>::iterator	iter = m_actorsShowDamage.find( id );
	if ( iter == m_actorsShowDamage.end() )
	{
		return NULL;
	}

	GameActor* pActor = ActorManager::getSingleton().FindActor( id );
	if ( pActor == NULL || m_passiveSCT.find( pActor->getType() ) == m_passiveSCT.end() )
	{
		return NULL;
	}

	return m_passiveSCT[pActor->getType()];
}

void SCTDamageManager::addDamageActor( int nActorID )
{
	m_actorsShowDamage.insert( nActorID );
}

void SCTDamageManager::clearDamageActor( )
{
	m_actorsShowDamage.clear( );
}

void SCTDamageManager::setEntityAttr( int nHurtType, int nChgValue, int nActorID )
{
	GameActor* pActor = ActorManager::getSingleton().FindActor( nActorID );
	if ( pActor == NULL )
	{
		return;
	}

	if ( nHurtType == HURT_TYPE_DMG || nHurtType == HURT_TYPE_HEAVY_DMG || nHurtType == HURT_TYPE_HITDEAD )
	{
		pActor->setHP( pActor->getHP() - nChgValue );
	}
	else if ( nHurtType == HURT_TYPE_HP )
	{
		pActor->setHP( pActor->getHP() + nChgValue );
	}

	GameMsgPrint::getSingleton().updateActorAttr( pActor );
}