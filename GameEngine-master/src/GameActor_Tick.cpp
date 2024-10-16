#include "GameActor_Tick.h"
#include "GameActor.h"
#include "ShowActor.h"
#include "GameSkill_Mgr.h"
#include "GameMsgPrint.h"
#include "OgreResourceManager.h"
#include "ogredistort.h"
#include "GameMainPlayer.h"
#include "GameMsgPrint.h"
#include "OgreScriptLuaVM.h"
#include "GameEffect_Mgr.h"
#include "EntityEffect.h"
#include "OgreDeathScene.h"
#include "TMPL.h"
#include "WorldCity.h"
#include <Windows.h>
using namespace Ogre;

GameActor_Tick::GameActor_Tick( void* pOwner ) : m_nState(TICK_BEGIN)
{
	m_pOwner = pOwner;
}

GameActor_Tick::GameActor_Tick( void* pOwner, unsigned int uStartTime/*开始时间*/, int	nPassTime/*所需时间*/ )
{
	m_pOwner = pOwner;
	setBeginTime( uStartTime, nPassTime );
}

GameActor_Tick::~GameActor_Tick()
{

}

// 设置起始时间
void GameActor_Tick::setBeginTime( unsigned int uStartTime/*开始时间*/, int	nPassTime/*所需时间*/ )
{
	m_uBeginTime = uStartTime;
	m_uTime		 = nPassTime;	 	
	m_uEndTime	 = uStartTime + nPassTime;
	m_nState	 = TICK_BEGIN;
	if( nPassTime == LOOP_TICK )
	{
		m_uEndTime = LOOP_TICK;
	}
}

// 设置时间
void GameActor_Tick::setTime( unsigned int uStartTime/*开始时间*/, int	nPassTime/*所需时间*/ )
{
	m_uBeginTime = uStartTime;
	m_uTime		 = nPassTime;	 	
	m_uEndTime	 = uStartTime + nPassTime;
	if( nPassTime == LOOP_TICK )
	{
		m_uEndTime = LOOP_TICK;
	}
}

int GameActor_Tick::updateTick( unsigned int uCurTime )
{
	// 开始
	if( uCurTime >= m_uBeginTime && m_nState == TICK_BEGIN )
	{
		onBeginEvent();
		m_nState = TICK_PROC;
	}
	
	// 循环
	if( m_nState == TICK_PROC && m_uEndTime == LOOP_TICK )
	{
		onPassEvent();
		return m_nState;
	}

	// 过程
	if( uCurTime >= m_uBeginTime && uCurTime <= m_uEndTime && m_nState == TICK_PROC  )
	{
		onPassEvent();
	}

	// 结束
	if( uCurTime >= m_uEndTime && m_nState == TICK_PROC )
	{
		onEndEvent();
		m_nState = TICK_END;
	}

	return m_nState;
}

//////////////////////////////////////////////////////////////////////////
void GameActor_Tick_MonsterBirth::onBeginEvent()
{
	GameActor* pActor = (GameActor*)m_pOwner;
	pActor->getActState().setInState( GSACT_IN_BIRTH, true );
	pActor->playAction( 10001, NULL, 0, -1 );
	if ( pActor->getType() == GAT_MONSTER )
	{
		char szBuf[256];
		const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo( pActor->getResId() );
		int id = info.birth;
		if (id == 0)
		{
			id = pActor->getResId();
		}
		if( info.MiscType & NPCDISPLAYINFO_MISC_TYPE_BIRTH_HIDE )
		{
			pActor->hide(true);
			sprintf( szBuf, "effect\\entity\\%d_18.ent", id );
			GameXYZ pos = pActor->GetPosition();
			EffectManage::getSingleton().playEffect( pos.x/10,pos.z/10,pActor->getAngle(),szBuf );
		}else{
			sprintf( szBuf, "%d_18", id );
			pActor->playMotion( szBuf );
		}
	}
}

void GameActor_Tick_MonsterBirth::onEndEvent()
{
	GameActor* pActor = (GameActor*)m_pOwner;
	pActor->setInState( GSACT_IN_BIRTH, false );
	// 当在播放出场动画的时候切换到站立状态
	if( pActor->getShowActor()->getCurAction() == 10001 )
	{
		pActor->ChangeActState( GSACT_STAND );
	}
	const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo( pActor->getResId() );
	if( info.MiscType & NPCDISPLAYINFO_MISC_TYPE_BIRTH_HIDE )
	{
		pActor->hide(false);
	}
}

//////////////////////////////////////////////////////////////////////////
void GameActor_Tick_TmpEquip::onBeginEvent()
{

}

void GameActor_Tick_TmpEquip::onEndEvent()
{

}

//////////////////////////////////////////////////////////////////////////
void GameActor_Tick_OnDead::onBeginEvent()
{
	GameActor* pActor = (GameActor*)m_pOwner;

	pActor->onStop( pActor->GetPosition().x/10, pActor->GetPosition().z/10, GSACT_STAND );
	int actionId = pActor->getActState().getActionId();
	
	char szBuf[256];
	char szBuf2[256];
	if ( pActor->getType() == GAT_MONSTER )
	{
		const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo( pActor->getResId() );
		int id = info.dieeffect;
		if (id == 0)
		{
			const tagMonsterDef& MonsterDef = DBDataManager::getSingleton().getMonsterDef( pActor->getResId() );
			id = MonsterDef.IconID;
		}
		sprintf( szBuf, "effect\\entity\\%d_20.ent", id );
		if (Ogre::ResourceManager::getSingleton().getFileAttrib( szBuf ) != 0)
		{
			EntityEffect *effect = new EntityEffect(szBuf, pActor->GetID());
			effect->start();
			effect->setAngle(pActor->getAngle());
			effect->setpos(pActor->GetPosition());
		}
		else
		{
			sprintf( szBuf2, "%d_19", id ); 
			sprintf( szBuf, "effect\\public\\%d_19.emo", id );
			if (Ogre::ResourceManager::getSingleton().getFileAttrib( szBuf ) != 0)
			{
				pActor->playMotion( szBuf2 );
			}
// 			else
// 			{
// 				pActor->playMotion( "9503_16" );
// 			}
		}
	}
	else if ( pActor->getType() == GAT_MAINPLAYER )
	{
		SkillManager::getSingleton().clearIntentSpell();
		MainGamePlayer* pMainPlayer = ( MainGamePlayer* )pActor;
		pMainPlayer->setAutoFind( false );
		pMainPlayer->breakPreAction();
		GameMsgPrint::getSingleton().SendUIEvent( "GE_MAIN_DEAD" );
	}
	else if ( pActor->getType() == GAT_PET )
	{
        pActor->playMotion( "9553_16" );
		MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
		if ( pMainPlayer->GetID() == pActor->getOwnerId() )
		{
			 GameMsgPrint::getSingleton().SendUIEvent( "GE_PET_DEAD" );
		}
	}
	
	if ( !pActor->getShowActor()->hasAnim( actionId ) )
	{
		pActor->getShowActor()->playAction( pActor->getActState().getActionId(true), NULL, 0, 0 );
	}
	else
	{
		pActor->getShowActor()->playAction( actionId, NULL, 0, 0 );
	}

	/*if( pActor->getType() == GAT_MONSTER && pActor->getSubType() == MON_SUB_TYPE_COLLECT )
	{
		char szScript[256];
		sprintf( szScript,  "EndWizardCollect(%d)", pActor->GetID() );
		ActorManager::getSingleton().getScriptVM()->callString( szScript );
	}*/
}

// 过程事件
void GameActor_Tick_OnDead::onPassEvent()
{
	GameActor* pActor = (GameActor*)m_pOwner;

	if( pActor->getType() == GAT_MONSTER )
	{
		const tagNpcDisplayInfo& npcDisplayInfo = DBDataManager::getSingleton().GetNpcDisplayInfo( pActor->getResId() ); 
		if (npcDisplayInfo.deadstate == 0)
		{
			m_nState = TICK_END;
			pActor->getShowActor()->getCurEntity()->setTransparent( 0.0f );
		}
		else if (npcDisplayInfo.deadstate & POWER<2, DEAD_STATE_FADE>::value)
		{
			if( timeGetTime() - m_uBeginTime >= 1000 )
			{
				float f = (float)( timeGetTime() - m_uBeginTime - 1000 ) / ( m_uTime - 1000 ) ;
				if (f > 1)
				{
					f = 1;
				}
				pActor->getShowActor()->getCurEntity()->setTransparent( 1.0f - f ); 
			}
		}
	}
	else if ( pActor->getType() == GAT_PET )
	{
		if( timeGetTime() - m_uBeginTime >= 8000 )
		{
			float f = (float)(timeGetTime() - m_uBeginTime - 8000) / (m_uTime - 8000);
			if (f > 1)
			{
				f = 1;
			}
			pActor->getShowActor()->getCurEntity()->setTransparent( 1.0f - f ); 
		}
	}
	else
	{
		m_nState = TICK_END;
	}
}

// 结束事件
void GameActor_Tick_OnDead::onEndEvent()
{
	GameActor* pActor = (GameActor*)m_pOwner;

	ActorManager& actorMgr = ActorManager::getSingleton();
	if ( pActor->getType() == GAT_PET )
	{
		GameActor* pPetOwner = actorMgr.FindActor( pActor->getOwnerId() );
		if ( pPetOwner == NULL )
		{
			return;
		}

		GamePet* pPetPackage = pPetOwner->getPet();
		for ( int i = 0; i < PET_MAX_FIGHT_SHI; ++i )
		{
			GameActor* pFightPet = pPetPackage->getPetActor( i );
			if ( pFightPet != pActor )
			{
				continue;
			}
			
			// 能播放到这边一定是宠物未出征情况，若已出征，这个事件一定已经被停止
			pPetPackage->setPetActor( i, NULL );
		}

		actorMgr.addClientSelfDestroy( pActor );
	}
}

void GameActor_Tick_OnWink::onBeginEvent()
{
	m_uEndTime = LOOP_TICK;
}

const unsigned int MAX_FADE_OUT = 1000;
const unsigned int MAX_FADE_IN  = 700;
const float	MIN_ALPHA = 0.3f;
// 过程事件
void GameActor_Tick_OnWink::onPassEvent()
{
	unsigned int uCurTime	= timeGetTime();
	unsigned int nPassTime = uCurTime - m_uBeginTime;

	GameActor* pActor = (GameActor*)m_pOwner;
	if ( pActor == NULL )
	{
		return;
	}
	pActor->setMonsterLiveTime( m_uLifeTime - nPassTime );

	// 若没到闪烁时间
	if ( nPassTime < ( m_uLifeTime - m_uWinkTime ) )
	{
		return;
	}
	
	// 解除循环状态
	if ( m_uEndTime == LOOP_TICK )
	{
		m_uEndTime = m_uBeginTime + m_uLifeTime;
		m_keepState.m_bFadeOut			= true;
		m_keepState.m_uKeepStartTime	= uCurTime;
		return;
	}
	
	Entity* pEntity = pActor->getShowActor()->getCurEntity();
	unsigned int uKeepTime = uCurTime - m_keepState.m_uKeepStartTime; 
	// 淡出
	if ( m_keepState.m_bFadeOut )
	{
		if ( uKeepTime >= MAX_FADE_OUT )
		{
			m_keepState.m_bFadeOut = false;
			m_keepState.m_uKeepStartTime = uCurTime;
		}
		else
		{
			float fChgAlpha = (float)uKeepTime / MAX_FADE_OUT * ( 1.0f - MIN_ALPHA );
			float fAlpha	= 1.0f - fChgAlpha;
			fAlpha = fAlpha < MIN_ALPHA ? MIN_ALPHA : fAlpha;
			pEntity->setTransparent( fAlpha );
		}
	}
	// 淡入
	else
	{
		if ( uKeepTime >= MAX_FADE_IN )
		{
			m_keepState.m_bFadeOut = true;
			m_keepState.m_uKeepStartTime = uCurTime;
		}
		else
		{
			float fChgAlpha = (float)uKeepTime / MAX_FADE_IN * ( 1.0f - MIN_ALPHA );
			float fAlpha	= MIN_ALPHA + fChgAlpha;
			fAlpha = fAlpha > 1.0f ? 1.0f : fAlpha;
			pEntity->setTransparent( fAlpha );
		}
	}
}

// 结束事件
void GameActor_Tick_OnWink::onEndEvent()
{
	GameActor* pActor = (GameActor*)m_pOwner;
	if ( pActor == NULL )
	{
		return;
	}
	pActor->setMonsterLiveTime( 0 );
}

//////////////////////////////////////////////////////////////////////////
void GameActor_Tick_InFightTime::onBeginEvent()
{
	GameActor* pActor			= (GameActor*)m_pOwner;
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	
	int nCanChangeFight = pActor->getActState().canChangeState( GSACT_IN_FIGHT );
	if( nCanChangeFight == STATE_UNCHANGE )
	{
		m_nState = TICK_END;
		return;
	}
	
	pActor->getActState().setInState( GSACT_IN_FIGHT, true );
	if( pActor == (GameActor*)pMainPlayer )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_FIGHT_IN );
	}

	if( nCanChangeFight != STATE_UNACT_CHANGE )
	{
		pActor->getShowActor()->playAction( pActor->getActState().getActionId() );
	}	
	m_nState = TICK_PROC;
}

void GameActor_Tick_InFightTime::onEndEvent()
{
	GameActor* pActor	= (GameActor*)m_pOwner;
	int nCanChangeFight = pActor->getActState().canChangeState( GSACT_IN_LEAVEFIGHT );
	
	pActor->getActState().setInState( GSACT_IN_FIGHT, false );
	if( pActor == (GameActor*)ActorManager::getSingleton().getMainPlayer() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_FIGHT_OUT );
	}

	if( nCanChangeFight != STATE_UNACT_CHANGE )
	{
		int nType = WorldCityManager::getSingleton().GetCityBuildType( pActor->GetID() );
		if( nType == CITY_BUILD_TYPE_DOOR_BUILD || nType == CITY_BUILD_TYPE_DOOR2 || nType == CITY_BUILD_TYPE_DOOR3 )
		{
			return;
		}

		int actionId = pActor->getActState().getActionId(true);
		const tagMonsterDef &monsterDef = DBDataManager::getSingleton().getMonsterDef(pActor->getResId());
		if (pActor->getSpeed() <= monsterDef.MoveSpd)
		{
			actionId = pActor->getActState().getWalkActId();
		}

		if (!pActor->getShowActor()->hasAnim( actionId ))
		{
			actionId = pActor->getActState().getActionId(true);
		}
		pActor->getShowActor()->playAction( actionId );
		return;
	}
}

void GameActor_Tick_UnfixTmpArm::onPassEvent()
{
	GameActor* pActor = (GameActor*)m_pOwner;
	if ( pActor == NULL )
	{
		return;
	}

	Rider* pRider = pActor->getRider();
	if ( pRider->isPre() )
	{
		setTime( timeGetTime(),LOOP_TICK );
		m_nTmpDelayTime = 10;
	}
	else
	{
		// 若没有处于骑乘吟唱状态
		if ( m_nTmpDelayTime != 0 )
		{
			setTime( timeGetTime(), m_nTmpDelayTime );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void GameActor_Tick_UnfixTmpArm::onEndEvent()
{
	GameActor* pActor = (GameActor*)m_pOwner;
	if ( pActor == NULL )
	{
		return;
	}
	Equip &equip = pActor->m_Equip;
	equip.changEquip( EQUIP_POS_RIGHTHAND, equip.m_ArmEquipId[EQUIP_POS_RIGHTHAND],equip.m_ArmStar[EQUIP_POS_RIGHTHAND] );
	equip.changEquip( EQUIP_POS_LEFT, equip.m_ArmEquipId[EQUIP_POS_LEFT],equip.m_ArmStar[EQUIP_POS_LEFT] );
	if (pActor->getAvatar())
	{
		pActor->getAvatar()->updateAttrib();
	}
	m_nTmpDelayTime = 0;
}

//////////////////////////////////////////////////////////////////////////
void GameActor_Tick_ShowArm::onEndEvent()
{
	GameActor* pActor = (GameActor*)m_pOwner;
	if (!pActor)
	{
		return;
	}
	pActor->m_Equip.showWeapon();
}

//////////////////////////////////////////////////////////////////////////
void GameActor_Tick_MainPlayerFree::onBeginEvent()
{
	GameActor* pActor = (GameActor*)m_pOwner;
	if (pActor->getActState().GetState() != GSACT_STAND
		|| pActor->getActState().getInState(GSACT_IN_RIDE) 
		|| pActor->getActState().getInState(GSACT_IN_MORPH)
		|| pActor->getActState().getInState(GSACT_IN_USEDMACHINE)
		|| pActor->getActState().getInState(GSACT_IN_CAST))
	{
		m_nState = TICK_END;
		return;
	}
	int standAction = getActId(pActor->getActState().getPoseState(),ACTSTA_NORMAL,ACTSTA_NORMAL_STAND);
	int freeAction = pActor->getActState().getPoseState()*1000 + 103;
 	if( pActor->getShowActor()->getCurAction() == standAction || pActor->getShowActor()->getCurAction() == freeAction )
	{
		pActor->playAction( pActor->getActState().getPoseState()*1000 + 103, NULL, 0, -1 );
	}else{
		m_nState = TICK_END;
	}
}

// 过程事件
void GameActor_Tick_MainPlayerFree::onPassEvent()
{
	GameActor* pActor = (GameActor*)m_pOwner;
	int standAction = getActId(pActor->getActState().getPoseState(),ACTSTA_NORMAL,ACTSTA_NORMAL_STAND);
	if (
		standAction != pActor->getShowActor()->getCurAction()
		|| pActor->getActState().GetState() != GSACT_STAND 
		|| pActor->getActState().getInState(GSACT_IN_RIDE) 
		|| pActor->getActState().getInState(GSACT_IN_MORPH)
		|| pActor->getActState().getInState(GSACT_IN_USEDMACHINE)
		|| pActor->getActState().getInState(GSACT_IN_CAST))
	{
		m_nState = TICK_END;
		return;
	}
	//如果已经开始播放动作的话，10s 重新播放
	if (timeGetTime() - m_uBeginTime > 25000 )
	{
		
		int freeAction = pActor->getActState().getPoseState()*1000 + 103;
		if( pActor->getShowActor()->getCurAction() != freeAction )
		{
			m_nState = TICK_END;
			return;
		}
		int nrand = rand()%100;
		if( nrand >= 80 )
		{
			pActor->playAction( freeAction, NULL, 0, -1 );
			setTime(timeGetTime(),LOOP_TICK);
		}
	}
}

void GameActor_Tick_ChgSize::onPassEvent()
{
	if (m_uTime == 0)
	{
		return;
	}
	float scale = (timeGetTime() - m_uBeginTime) * (m_scale - 1) / m_uTime + 1;
	GameActor* pActor = (GameActor*)m_pOwner;
	if ( pActor != NULL )
	{
		pActor->getShowActor()->getCurEntity()->setScale( Ogre::Vector3(scale / m_lastScale) * 
															pActor->getShowActor()->getCurEntity()->getScale() );
	}

	m_lastScale = scale;
}

void GameActor_Tick_ChgSize::onEndEvent()
{
	GameActor* pActor = (GameActor*)m_pOwner;
	if ( pActor != NULL )
	{
		if( pActor->isInStall() )
		{
			return;
		}

		pActor->getShowActor()->getCurEntity()->setScale(Ogre::Vector3(m_scale / m_lastScale) * pActor->getShowActor()->getCurEntity()->getScale());
	}
	m_lastScale = m_scale;
}
