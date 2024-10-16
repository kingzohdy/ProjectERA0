#include "GameExActorMgr.h"
#include "GameMainPlayer.h"

#include "GameActor.h"
#include "GameNpc.h"
#include "GameMsgPrint.h"
#include "ShowActor.h"
#include "OgreEntity.h"
#include "GameActor_RoleText.h"
#include "OgreScriptLuaVM.h"
#include "OgreRay.h"

#include <Windows.h>
#include "GameMap.h"
#include "OgreRoot.h"

#include "GameEffect_Mgr.h"
#include "GameCamera.h"
#include "OgreCamera.h"
#include "WorldCity.h"

#include "../UILib/ui_framemgr.h"

#include "OgreBorderScene.h"
#include "BulletManager.h"

#include "OgreLog.h"
using namespace Ogre;
using namespace std;

ExtraActorMgr::ExtraActorMgr(void) : m_nLastNpcID(0),m_nNpcMemId(0)
{

}

ExtraActorMgr::~ExtraActorMgr(void)
{
	DestroyAllActor();
}

GameActor*	ExtraActorMgr::mounseOnActor( const Ogre::WorldRay& worldray )
{
	Ogre::Ray ray;
	worldray.getRelativeRay(ray, WorldPos(0,0,0));
	GameActor* pSelectActor = NULL;

	if(BorderGameScene::getSingletonPtr())
	{
		BorderGameScene::getSingleton().clear();
	}
	std::map<int, GameActor*>::iterator it = m_ExtraActors.begin();
	for ( it = m_ExtraActors.begin();it != m_ExtraActors.end(); it++ )
	{
		it->second->getShowActor()->getCurEntity()->setInstanceAmbient(ColourValue(0.45f, 0.45f, 0.45f, 0));
		GameActor* pActor = it->second;
		bool b = pActor->getShowActor()->intersectRay(ray);
		if(b)
		{
			int resid = pActor->getResId();
			const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resid);
			if (info.MiscType & NPCDISPLAYINFO_MISC_TYPE_NO_SELECT)
			{
				continue;
			}
			pSelectActor = pActor;
		}
	}
	if( pSelectActor != NULL )
	{
		Ogre::ShowActor* pSelectShowActor = pSelectActor->getShowActor();
		if ( pSelectShowActor != NULL && !pSelectShowActor->isInMap() && !pSelectActor->isInStall() )
		{
			return pSelectActor;
		}

		Vector3 color = Vector3(0.16f,0.99f,0.99f);
		
		pSelectActor->getShowActor()->getCurEntity()->setInstanceAmbient(ColourValue(0.8f, 0.8f, 0.8f, 0));
		if (BorderGameScene::getSingletonPtr() && pSelectActor->getShowActor())
		{
			Entity *entity = 0;
			entity = pSelectActor->getShowActor()->getCurEntity();
			if (entity)
			{
				entity->setBorderColor(color);
				BorderGameScene::getSingleton().onAttachObject(entity);
			}
		}
	}
	return pSelectActor;
}
void ExtraActorMgr::setNpcTalkID( int nNpcID )
{
	m_nNpcMemId = nNpcID;
}
int	ExtraActorMgr::getNpcTalkID()
{
	return m_nNpcMemId;
}
void ExtraActorMgr::setLastNpcTalkID( int nNpcID )
{
	m_nLastNpcID = nNpcID;
}
int	ExtraActorMgr::getLastNpcTalkID()
{
	return m_nLastNpcID;
}

void ExtraActorMgr::drawActorTitles(FollowCamera *pcamera)
{
	for( std::map<int, GameActor *>::iterator iter = m_ExtraActors.begin(); iter != m_ExtraActors.end(); iter++)
	{
		GameActor* pOneActor = iter->second;
		RoleText& drawRoleTextMgr = pOneActor->getRoleText();
		if( pOneActor->getActState().GetState() != GSACT_DEAD )
		{
			drawRoleTextMgr.drawTitles(pcamera);	
		}
	}
}

void ExtraActorMgr::Update( unsigned int dtick )
{
	if ( ActorManager::getSingleton().getMainPlayer() == NULL )
	{
		return;
	}

	FollowCamera *camera = FollowCamera::getSingletonPtr();
	Ogre::Camera *cutcamera = NULL;
	if (camera != NULL)
	{
		cutcamera = camera->GetCutActorCameraNode();
	}
	for(std::map<int, GameActor *>::iterator iter = m_ExtraActors.begin(); iter != m_ExtraActors.end(); iter++)
	{
		if (cutcamera && !cutcamera->canSeePointInWorld(iter->second->GetPosition()))
		{
			iter->second->optimizeClear(false);
		}
		else
		{
			iter->second->optimizeClear(true);
		}
		iter->second->Update(dtick);
	}
}

GameActor* ExtraActorMgr::CreateActor( int id, GameActorType type, int resid, bool background/* = false*/ )
{
	assert(FindActor(id) == NULL);
	
	GameActor *pactor = NULL;
	switch(type)
	{
	case GAT_EXTRA_NPC:
		pactor = new GameNPC;
		break;
	default:
		assert(0);
		break;
	}

	pactor->setType(type);

	pactor->LoadRes(resid);
	pactor->SetID(id);
	m_ExtraActors[id] = pactor;
	return pactor;
}

GameActor* ExtraActorMgr::FindActor( int id )
{
	std::map<int, GameActor *>::iterator it = m_ExtraActors.find(id);
	if(it != m_ExtraActors.end())
	{
		return it->second;
	}
	return NULL;
}

void ExtraActorMgr::DestroyAllActor()
{
	if ( m_ExtraActors.empty() )
	{
		return;
	}
	for( std::map<int, GameActor*>::iterator iter = m_ExtraActors.begin(); iter != m_ExtraActors.end();iter++ )
	{
		if( iter->second )
		{
			/*Ogre::Entity*  pActorEntity = iter->second->getShowActor()->getCurEntity();
			pActorEntity->detachFromScene();*/
			char szPopName[256];
			sprintf( szPopName, "%d%s", iter->second->GetID(),iter->second->getName() );
			g_pFrameMgr->hidePopWin( szPopName );

			ActorManager::getSingleton().updateMonsters( iter->second );
			iter->second->OnLeaveMap();
			delete iter->second;
		}	
	}
	m_ExtraActors.clear();
}