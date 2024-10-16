#include "StdAfx.h"
#include "OgreRay.h"
#include "OgreCamera.h"

#include "ActorCtrl.h"
#include "EnginAgent.h"
#include "OgreInputManager.h"
#include "ShowGameMap.h"
#include "GameMap.h"
#include "GameActor.h"
#include "GameCamera.h"
#include "OgreGameScene.h" 
#include "ShowActor.h"
#include "MainFrm.h"
#include "SkillEditor.h"
#include "GameSkill_Mgr.h"

using namespace Ogre;
CActorCtrl::CActorCtrl(void):m_PreSelId(0),m_pSelEntity(NULL),m_InUsedSkill(0)
{
}

CActorCtrl::~CActorCtrl(void)
{
	if( m_pSelEntity != NULL )
	{
		OGRE_RELEASE(m_pSelEntity);
	}
}

int CActorCtrl::HandleInput(const Ogre::InputEvent &event)
{
	switch(event.msg)
	{
	case GIE_MOUSEMOVE:
		{
			static int PreMouseX = -1;
			static int PreMouseY = -1;
			int CurMouseX = event.mouse.x;
			int CurMouseY = event.mouse.y;
			float MouseDeltaX	= float( CurMouseX - PreMouseX );
			float MouseDeltaY	= float( CurMouseY - PreMouseY );
			PreMouseX = CurMouseX;
			PreMouseY = CurMouseY;

			if((event.mouse.flags&MK_RBUTTON))
			{
				int w = CEnginAgent::GetInstance()->m_nViewportWidth;
				float angle = MouseDeltaX/w * 160.0f;
				CEnginAgent::GetInstance()->m_pMainCamera->Rotate(angle);
			}
		}
		break;
	case GIE_RBTNDOWN:
		{

		}
		break;
	case GIE_LBTNDOWN:
		{
			MainGamePlayer* pMainPlayer = CEnginAgent::GetInstance()->m_pMainPlayer;
			FollowCamera*	pMainCamera = CEnginAgent::GetInstance()->m_pMainCamera;
			GameMap*		pCurMap		= CEnginAgent::GetInstance()->m_pCurMap;	
			 
			Ogre::WorldRay ray;
			pMainCamera->GetCameraNode()->getViewRayByScreenPt(&ray, event.mouse.x, event.mouse.y);
			float dist;
			
			GameActor* pActor = ActorManager::getSingleton().pickActor( ray );
			
			if( pMainPlayer->m_AttackLock.m_nAttackLockType == ATTACK_LOCK_NONE )
			{
				if( pActor != NULL && pActor != pMainPlayer )
				{
					pMainPlayer->SetSelectTarget( pActor->GetID() );
					showSelectEffect(pActor->GetID());
					
					char tem[256];
					sprintf( tem, "选中的怪物ID为: %d", pActor->GetID() );
					((CMainFrame*)theApp.GetMainWnd())->m_wndFightWorkSpace.initMonsterAIPropValue( pActor->GetID() );
					((CMainFrame*)theApp.GetMainWnd())->m_wndOutput.printfDebugString( tem );
					return 0;
				}else{
					SkillManager::getSingleton().getSpellControler()->onStart( pMainPlayer->GetID(), 0, 0, 0 );
				}
			}

			if( pCurMap->getShowGameMap()->pickGround( ray, &dist ) )
			{
				Ogre::WorldPos pickpoint = ray.m_Origin + ray.m_Dir*dist;
				int x = (int)Ogre::WorldPos::Fix2Flt(pickpoint.x);
				int z = (int)Ogre::WorldPos::Fix2Flt(pickpoint.z);
				Ogre::Point2D start(x, z);

				pMainPlayer->CmdMoveTo( x, z );
			}
		}
		break;

	case GIE_MOUSEWHEEL:
		{
			FollowCamera*	pMainCamera = CEnginAgent::GetInstance()->m_pMainCamera;
			if(pMainCamera) pMainCamera->PullInOut(-event.wheel.dist*400.0f);
		}
		break;

	case GIE_KEYDOWN:
		{
			if( event.key.vkey >= VK_F1 && event.key.vkey <= VK_F12 )
			{
				int i = event.key.vkey - VK_F1;
				int skillId = ((CMainFrame*)theApp.GetMainWnd())->m_wndFightWorkSpace.m_skillId[i];
				if( skillId != 0 )
				{
					MainGamePlayer* pMainPlayer = CEnginAgent::GetInstance()->m_pMainPlayer;
					if ( pMainPlayer != NULL )
					{
						bool bUsed = SkillManager::getSingleton().getSpellControler()->onStart( pMainPlayer->GetID(), pMainPlayer->GetSelectTarget(),
							skillId, 0 );
						if( bUsed )
						{
							SkillManager::getSingleton().getSpellControler()->useSpell( skillId, 1 );
							m_InUsedSkill = skillId;
						}else{
							m_InUsedSkill = 0;
						}
					}	
				}
			}
		}
		break;
	case GIE_KEYUP:
		{
			if( m_InUsedSkill != 0 )
			{
				SkillManager::getSingleton().getSpellControler()->unUseSpell( m_InUsedSkill, 1 );
				m_InUsedSkill = 0;
			}
			m_InUsedSkill = 0;
		}
		break;
	}
	return 0;
}

void CActorCtrl::update(long lTick)
{

}

void CActorCtrl::showSelectEffect( int id )
{
	if( id != m_PreSelId )
	{
		GameActor* pSelObj = ActorManager::getSingleton().FindActor( m_PreSelId );
		if( pSelObj != NULL && m_pSelEntity != NULL )
		{
			pSelObj->getShowActor()->getCurEntity()->unbindObject(m_pSelEntity);
			OGRE_RELEASE(m_pSelEntity);
		}

		if( pSelObj != NULL && m_pSelEntity != NULL )
		{
			pSelObj->getShowActor()->getCurEntity()->unbindObject(m_pSelEntity);
			OGRE_RELEASE(m_pSelEntity);
		}	

		if( m_pSelEntity == NULL)
		{
			m_pSelEntity = new Ogre::Entity;
			m_pSelEntity->load( "effect\\entity\\9500_16.ent", true );
		}

		pSelObj = ActorManager::getSingleton().FindActor( id );
		m_PreSelId = id;
		if( pSelObj != NULL )
		{
			Ogre::WorldPos pos = Ogre::Vector3( 0, 20, 0 );
			m_pSelEntity->setPosition( pos );
			pSelObj->getShowActor()->getCurEntity()->bindObject( 0, m_pSelEntity );
		}
	}


}
