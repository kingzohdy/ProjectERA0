#include <Windows.h>
#include "GameActor_Carry.h"
#include "GameActor.h"
#include "ShowActor.h"
#include "GameMainPlayer.h"
#include "OgreResourceManager.h"
#include "GameMap.h"
#include "ShowGameMap.h"

#include "OgreBackScene.h"
#include "OgreBorderScene.h"

using namespace Ogre;
GameActor_Carry::GameActor_Carry(GameActor* pOwner)
				:m_pOwner(pOwner),
				m_nBindID(-1),
				m_nCarryID(-1),
				m_nCarriedID(-1),
				m_pCarriedEntity(NULL),
				m_nCarryState(eNo_Carry)
{

}

GameActor_Carry::~GameActor_Carry()
{
	if( m_pCarriedEntity != NULL )
	{
// 		m_pCarriedEntity->detachFromScene();
		OGRE_RELEASE(m_pCarriedEntity);
	}
}

// 玩家背起一个模型
bool GameActor_Carry::carry(int nStandActionID,int nMoveActionID,int nBindID,int nResID,int nTargetAction)
{
	if( m_nCarryState != eNo_Carry )
	{
		return false;
	}

	if( m_pOwner == NULL )
	{
		return false;
	}

	if( m_pOwner->getActState().canChangeState(GSACT_IN_CARRY) != STATE_CHANGE )
	{
		return false;
	}	

	const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(nResID);
	//TODO HERE 判断 info 是否有效
	if( m_pCarriedEntity != NULL )
	{
		OGRE_RELEASE(m_pCarriedEntity);
	}

	if(info.model[0])
	{
		m_pCarriedEntity = new Entity;
		m_pCarriedEntity->load(info.model, false);
	}

	if( m_pCarriedEntity == NULL )
	{
		return false;
	}
	if( m_pOwner->getCurMap() && m_pOwner->getCurMap()->getShowGameMap() )
	{
		m_pCarriedEntity->attachToScene( m_pOwner->getCurMap()->getShowGameMap()->getGameScene() );
	}
	m_pOwner->getShowActor()->getCurEntity()->bindObject(nBindID,m_pCarriedEntity);

	if( nTargetAction > 0 )
	{
		if( m_pCarriedEntity->getMainModel()->hasAnim(nTargetAction) )
		{
			m_pCarriedEntity->playMotion(nTargetAction,NULL,0,0);
		}
	}
	m_pOwner->playAction( nStandActionID );

	m_nCarryID = m_pOwner->GetID();
	m_nCarriedID = -1;
	m_nCarryState = eCarrying;
	m_pOwner->m_Equip.hideWeapon();
	m_pOwner->setInState(GSACT_IN_CARRY,true);

	m_nBindID				= nBindID;
	m_nCarryStandActionID	= nStandActionID;
	m_nCarryMoveActionID	= nMoveActionID;
	m_nCarriedActionID		= nTargetAction;
	m_pOwner->ChangeActState( m_pOwner->getActState().GetState() );
	if( m_pOwner->getType() == GAT_MAINPLAYER )
	{
		((MainGamePlayer*)m_pOwner)->lockControl();
	}
	return true;
}

//玩家背另一个玩家，或者npc
bool GameActor_Carry::carry( int nCarryID,int nStandActionID,int nMoveActionID,int nBindID,int nTargetID,int nTargetAction )
{
	if( m_nCarryState != eNo_Carry )
	{
		return false;
	}

	if( m_pOwner == NULL )
	{
		return false;
	}

	//玩家背人
	if( m_pOwner->GetID() == nCarryID )
	{
		if( m_pOwner->getActState().canChangeState(GSACT_IN_CARRY) != STATE_CHANGE )
		{
			return false;
		}
		GameActor* pTarget = ActorManager::getSingleton().FindActor(nTargetID);
		if( pTarget == NULL )
		{
			return false;
		}
		//判断距离
		GameXYZ myPos = m_pOwner->GetPosition();
		GameXYZ targetPos = pTarget->GetPosition();
		if( targetPos.subtract(myPos).length() >=1000 )
		{
			return false;
		}
		//转向
		m_pOwner->SetFaceTo( targetPos.x,targetPos.z,myPos.x,myPos.z );
		
		if( pTarget->getActState().canChangeState(GSACT_IN_CARRY) != STATE_CHANGE )
		{
			return false;
		}
		m_pOwner->playAction(nStandActionID);
		m_pOwner->m_Equip.hideWeapon();

		if(! pTarget->m_pCarry->carry(nCarryID,nStandActionID,nMoveActionID,nBindID,nTargetID,nTargetAction) )
		{
			return false;
		}
	}else{
		//玩家被别人背起来
		if( m_pOwner->GetID() != nTargetID )
		{
			return false;
		}
		if( nTargetAction > 0  )
		{
			m_pOwner->playAction(nTargetAction);
		}
		if( m_pOwner->getType() == GAT_PLAYER )
		{
			m_pOwner->m_Equip.hideWeapon();
		}
	}
	
	m_pOwner->setInState(GSACT_IN_CARRY,true);
	m_nCarryState			= eCarrying;
	m_nCarryID				= nCarryID;
	m_nCarriedID			= nTargetID;
	m_nBindID				= nBindID;
	m_nCarryStandActionID	= nStandActionID;
	m_nCarryMoveActionID	= nMoveActionID;
	m_pOwner->ChangeActState(GSACT_STAND);
	if( m_pOwner->getType() == GAT_MAINPLAYER )
	{
		((MainGamePlayer*)m_pOwner)->lockControl();
	}
	return true;
}

bool GameActor_Carry::unCarry(unsigned int costTime)
{

	//目前先直接放下
	if( m_nCarryState == eNo_Carry || m_nCarryID == -1 )
	{
		return false;
	}
	if( m_nCarryID == m_pOwner->GetID() )
	{
		if( m_nCarriedID < 0 && m_pCarriedEntity != NULL )
		{
			m_pOwner->getShowActor()->getCurEntity()->unbindObject(m_pCarriedEntity);
			m_pCarriedEntity->detachFromScene();
			OGRE_RELEASE(m_pCarriedEntity);
		}else if( m_nCarriedID > 0 )
		{
			GameActor *pActor = ActorManager::getSingleton().FindActor(m_nCarriedID);
			if( pActor != NULL )
			{
				pActor->m_pCarry->unCarry(0);
			}
		}
	}

	m_pOwner->m_Equip.showWeapon();
	m_pOwner->setInState(GSACT_IN_CARRY,false);
	m_nCarriedID = -1;
	m_nCarryID = -1;
	m_nCarryState = eNo_Carry;
	if( m_pOwner->getActState().GetState() != GSACT_DEAD )
	{
		m_pOwner->ChangeActState( m_pOwner->getActState().GetState() );
	}
	if( m_pOwner->getType() == GAT_MAINPLAYER )
	{
		((MainGamePlayer*)m_pOwner)->releaseControl();
	}

// 	//是否有放下的动作的时间消耗
// 	if( m_pOwner != NULL )
// 	{
// 		m_pOwner->m_Equip.showWeapon();
// 	}
// 	if( m_nCarriedID > 0 )
// 	{
// 		GameActor* pTarget = ActorManager::getSingleton().FindActor(m_nCarriedID);
// 		if( pTarget != NULL && pTarget->getType() == GAT_PLAYER )
// 		{
// 			pTarget->m_Equip.showWeapon();
// 		}
// 	}
// 	//播放放下动画，
// 	m_pOwner->playAction(10130);
// 	m_uStartTime	= timeGetTime();
// 	m_uCostTime		= costTime;
// 
// 	//开始计时

	return true;
}

int GameActor_Carry::getCarryStandAction()
{
	if( m_pOwner->GetID() == m_nCarryID )
	{
		return m_nCarryStandActionID;
	}else{
		return m_nCarriedActionID;
	}
}

int GameActor_Carry::getCarryMoveAction()
{
	if( m_pOwner->GetID() == m_nCarryID )
	{
		return m_nCarryMoveActionID;
	}else{
		return m_nCarriedActionID;
	}
}

int GameActor_Carry::getCarryState()
{
	if( m_nCarryState == eCarrying )
	{
		if( m_nCarryID == m_pOwner->GetID() )
		{
			return eCarrying;
		}else{
			return eCarryed;
		}
	}
	return m_nCarryState;
}

void GameActor_Carry::update(unsigned int dtick)
{
	if( m_pOwner == NULL )
	{
		return;
	}
	if( m_nCarryState == eNo_Carry )
	{
		return;
	}
	if( m_nCarryState == eCarrying )
	{
		if( m_nCarriedID > 0 && m_nCarriedID == m_pOwner->GetID() )
		{
			GameActor* pActor = ActorManager::getSingleton().FindActor( m_nCarryID );
			if( pActor != NULL )
			{
				Vector3 pos = pActor->getShowActor()->getCurEntity()->getAnchorWorldPos(m_nBindID);
				m_pOwner->SetPosition((int)pos.x,(int)pos.z,(int)pos.y);
			}
		}else{
			if( m_pCarriedEntity == NULL )
			{
				return;
			}
			if( m_pOwner->getCurMap() && m_pOwner->getCurMap()->getShowGameMap() )
			{
				if( m_pCarriedEntity->getScene() != m_pOwner->getCurMap()->getShowGameMap()->getGameScene() )
				{
					m_pCarriedEntity->attachToScene( m_pOwner->getCurMap()->getShowGameMap()->getGameScene() );
				}
			}

			//Vector3 pos = m_pOwner->getShowActor()->getCurEntity()->getAnchorWorldPos(m_nBindID);
			//m_pCarriedEntity->setPosition(pos);
			//m_pCarriedEntity->setRotation( m_pOwner->getShowActor()->getCurEntity()->getRotation() );
			m_pCarriedEntity->update(dtick);
		}
	}
	else if( m_nCarryState == eEnd_Carry )
	{
		if( timeGetTime() - m_uStartTime >= m_uCostTime )
		{
			if( m_nCarriedID > 0 )
			{
			}else{
				m_pCarriedEntity->detachFromScene();
				OGRE_RELEASE(m_pCarriedEntity);
				m_pOwner->setInState(GSACT_IN_CARRY,false);
				m_pOwner->CanChangeActState(GSACT_STAND);
				m_pOwner->m_Equip.showWeapon();
			}
		}
	}

	Vector3 pos = m_pOwner->getShowActor()->getCurEntity()->getAnchorWorldPos(m_nBindID);

	if (m_pCarriedEntity)
	{
		m_pCarriedEntity->setDrawbackScene(true);
	}
}

void GameActor_Carry::onOwnerDead()
{
	if( m_nCarryState != eNo_Carry )
	{
		m_nCarryState = eNo_Carry;
		m_pOwner->setInState( GSACT_IN_CARRY,false );
		if( m_pOwner->GetID() == m_nCarryID )
		{
			GameActor* pActor = ActorManager::getSingleton().FindActor( m_nCarriedID );
			if( pActor != NULL )
			{
				pActor->m_pCarry->unCarry( 0 );
			}
		}
	}
}

void GameActor_Carry::onLeaveMap()
{
	if( m_nCarryState != eNo_Carry )
	{
		if( m_nCarriedID > 0 )
		{
			unCarry(0);
		}else
		{
			if( m_pCarriedEntity != NULL )
			{
				m_pCarriedEntity->detachFromScene();
			}
		}
	}
}