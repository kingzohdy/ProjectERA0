#include <Windows.h>
#include "GameManager.h"
#include "GamePlayManager.h"
#include "GameCamera.h"
#include "GameActor.h"
#include "ShowActor.h"
#include "GameMap.h"
#include "ShowGameMap.h"
#include "GameSkill_Mgr.h"
#include "GameSkill_Common.h"
#include "GameShortCut.h"
#include "GameMaxMap.h"
#include "GameWizard.h"
#include "GameMainPlayer.h"
#include "GameBattle.h"
#include "BulletManager.h"
#include "WorldCity.h"
#include "OgreResourceManager.h"
#include "OgreBorderScene.h"
#include "OgreSoundSystem.h"
#include "GameStateManager.h"
#include "OgreLog.h"
#include "OgreInputHandler.h"
#include "OgreRay.h"
#include "OgreCamera.h"
#include "OgreScriptLuaVM.h"
#include "GameActor_PVP.h"
#include "GameClientOpt.h"
#include "GameKeyMoveOperate.h"
#include "GodSchema.h"
#include "GameQuest.h"
#include "OgreMaterialTemplate.h"
#include "GameBCT.h"
#include "GameFriendManager.h"
#include "ogrebloom.h"
#include "GameClanManager.h"
#include "GameLoginManager.h"
#include "OgreSceneManager.h"
#include "OgreGameScene.h"
#include "OgreGameTerrainScene.h"
#include "OgreTerrainData.h"
#include "OgreTerrainBlock.h"
#include "GameWarfogManager.h"
#include "AvatarManager.h"
#include "GameTopCareer.h"
#include "OgreRoot.h"
#include "GameExActorMgr.h"
#include "GameHomelandManager.h"

#include "../UILib/ui_framemgr.h"
#define INWarMap 1
#define OFFWarMap -1

using namespace Ogre;

extern FrameManager *g_pFrameMgr;

const int MAPID_NONE = -1;
static bool bOnCamerRotate = false;
static bool isFuniturnMove	= false;

GamePlayManager::GamePlayManager() : m_pMainCamera(NULL), m_pCurMap(NULL), m_CurMapID( MAPID_NONE ), m_isInWorld(false), m_isLoading(false)
{
	m_pMainCamera = new FollowCamera;

	int w, h;
	GameManager::getSingleton().GetViewport(w, h);
	Ogre::Camera *camera = m_pMainCamera->GetCameraNode();
	camera->setRatio(float(w)/float(h));
	camera->setViewport(0, 0, float(w), float(h));
	camera = m_pMainCamera->GetCutActorCameraNode();
	if (camera != NULL)
	{
		camera->setRatio(float(w)/float(h));
		camera->setViewport(0, 0, float(w), float(h));
	}
	//m_pShowEngine->SetMainCam(m_pMainCamera->GetCameraNode()); 
	memset( &m_PlayerCameraDir, 0, sizeof(tagPlayerCameraDir) );
	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_CAMERADIR, sizeof( tagPlayerCameraDir ) );
}
GamePlayManager::~GamePlayManager()
{
	SAFE_DELETE( m_pMainCamera );
	SAFE_DELETE( m_pCurMap );
}

int GamePlayManager::HandleInput(const Ogre::InputEvent &event)
{
	if(!m_isInWorld) return 0;

	switch(event.msg)
	{
	case GIE_MOUSEMOVE:
		{
			m_SelectMgr.onMouseMove(event);
			static int PreMouseX = -1;
			static int PreMouseY = -1;
			static POINT point; 
			POINT curPoint; 
			::GetCursorPos(&curPoint);
			
			int CurMouseX = event.mouse.x;
			int CurMouseY = event.mouse.y;
			//float MouseDeltaX	= 100.0f*float( CurMouseX - PreMouseX );
			float MouseDeltaX	= float( curPoint.x - point.x );
			float MouseDeltaY	= float( curPoint.y - point.y );
			
			PreMouseX = CurMouseX;
			PreMouseY = CurMouseY;
			
			if( GameHomelandManager::getSingleton().isMovingMode() ) {
				if( !(event.mouse.flags & MK_LBUTTON ) ) {
					if( isFuniturnMove ) {
						isFuniturnMove = false;
						GameHomelandManager::getSingleton().endMoveMode();
					}
				} else {
					if( !isFuniturnMove ) {
						::GetCursorPos( &point );
						MouseDeltaX = MouseDeltaY = 0;
						isFuniturnMove = true;
					}
					if( MouseDeltaX != 0 || MouseDeltaY != 0 ) {
						GameHomelandManager::getSingleton().stepFurnitureLocation( MouseDeltaX, MouseDeltaY );
					}
					if( Ogre::Abs(MouseDeltaX) > 5 || Ogre::Abs(MouseDeltaY)>5 ) {
						::SetCursorPos( point.x, point.y );
					}
				}
			}

			if( !(event.mouse.flags&MK_RBUTTON) )
			{
				if( bOnCamerRotate )
				{
					while( ::ShowCursor(true) < 0);
					bOnCamerRotate = false;
				}
			}

			if( (event.mouse.flags&MK_RBUTTON) )
			{
				if( !bOnCamerRotate )
				{
					while(::ShowCursor(false) >= 0);
					::GetCursorPos(&point);
					MouseDeltaX = 0;
					MouseDeltaY = 0;
					bOnCamerRotate = true;
				}

				int w, h;
				GameManager::getSingleton().GetViewport(w, h);
				float angle = MouseDeltaX/w * 160.0f;
				float hAngel= MouseDeltaY/h * 160.0f;
				if(m_pMainCamera) 
				{
					if ( abs(MouseDeltaY) > abs(MouseDeltaX) && ActorManager::getSingleton().getGameOpt( GAME_OPT_CAMERA_3D ) )
					{
						m_pMainCamera->ChangeToward(hAngel);
					} 
					else
					{
						m_pMainCamera->Rotate(angle);
					}
					if( abs(MouseDeltaX) > 5 || abs(MouseDeltaY)>5 )
					{
						::SetCursorPos( point.x, point.y );
					}
				}
			}
			
			/*if( getMainPlayer() && (event.mouse.flags&MK_RBUTTON) ) 
			{
				int w, h;
				GameManager::getSingleton().GetViewport(w, h);
				float angle = MouseDeltaX/w * 360.0f;
				int nAngleLR = getMainPlayer()->getAngle() + angle;
				if(nAngleLR < 0) nAngleLR += 360.0f;
				if(nAngleLR >= 360.0f) nAngleLR -= 360.0f;

				getMainPlayer()->SetFaceTo( nAngleLR );
			}*/
		}
		break;
	case GIE_RBTNDOWN:
		{
			m_SelectMgr.onRButtonDown(event);
			::SetCapture((HWND)g_pFrameMgr->getWndHandle());
		}
		break;
	case GIE_LBTNDOWN:
		{
			m_SelectMgr.onLButtonDown(event);
			GameManager::getSingleton().getScriptVM()->callString( "GameSelectLeftButtonDown()" );
		}
		break;

	case GIE_RBTNUP:
		{
			if( !(event.mouse.flags&MK_RBUTTON) )
			{
				if( bOnCamerRotate )
				{
					while( ::ShowCursor(true) < 0);
					bOnCamerRotate = false;
				}
			}
			::ReleaseCapture();
		}
		break;

	case GIE_MOUSEWHEEL:
		{
			if( m_pMainCamera ) 
			{
				m_pMainCamera->PullInOut(-event.wheel.dist*400.0f);
			}
		}
		break;
	}

	return 0;
}

void GamePlayManager::doFrame(unsigned int dtick)
{
	ActorManager::getSingleton().Update(dtick);
	ExtraActorMgr::getSingleton().Update(dtick);
	WorldCityManager::getSingleton().update(dtick);
	GameTopCareer::getSingleton().Update(dtick);
	AvatarManager::getSingleton().update(dtick);
	god_update(dtick);
	if( getMainPlayer() != NULL )
	{
		if( m_pCurMap != NULL && m_pMainCamera != NULL && !m_isLoading ) 
		{
			m_pMainCamera->Update( dtick );
			m_pCurMap->update( dtick, m_pMainCamera->GetCameraNode(), m_pMainCamera->getCameraCenter() );
		}

		m_SelectMgr.update(dtick);
		GameShortCut::getSingleton().update( dtick );
		GameKeyMoveOperate::getSingleton().update();
	}
}

void GamePlayManager::enterPworld( int nMapId )
{
	if( nMapId >= GAME_PWORLD_MAPID && m_pCurMap != NULL )
	{
		int nPworldId = nMapId/100;
		const tagPworldDef* pPworldDef = DBDataManager::getSingleton().getPworldDef( nPworldId );
		if( getMainPlayer() != NULL && pPworldDef != NULL )
		{
			if( pPworldDef->PlayerModeID != 0 )
			{
				getMainPlayer()->onMorph( pPworldDef->PlayerModeID );	
			}
		}
		GameBattle::getSingleton().onEnterPworld(nMapId);
		GameBCT::getSingleton().onEnterPworld(nMapId);
	}
}

void GamePlayManager::leavePwolrd( int nMapId )
{
	if( nMapId >= GAME_PWORLD_MAPID && m_pCurMap != NULL )
	{
		int nPworldId = nMapId/100;
		const tagPworldDef* pPworldDef = DBDataManager::getSingleton().getPworldDef( nPworldId );
		if( getMainPlayer() != NULL && pPworldDef != NULL )
		{
			if( pPworldDef->PlayerModeID != 0 )
			{
				getMainPlayer()->onRestore(pPworldDef->PlayerModeID);
			}
			GameMapShower* pMapShower = GameManager::getSingleton().getGameMapShower();
			pMapShower->clearMapNpc( nMapId );
		}
		GameBattle::getSingleton().onLeavePworld(nMapId);
		GameBCT::getSingleton().onLeavePworld(nMapId);
		GameFriendManager::getSingleton().onLeavePWorld(nMapId);
		GameManager::getSingleton().SendUIEvent("GE_LEAVE_PWORLD");
	}
}

void GamePlayManager::enterMap(GameMap *map, int x, int z)
{
	int mapid = map->getMapId();
	m_SelectMgr.breakAutoClick();
	//公会地图
	if( m_CurMapID == CLAN_CITY_MAP_ID )
	{
		GameClanManager::getSingleton().onLeaveClanMap();
	}
	Ogre::SceneRenderer *prenderer = GameManager::getSingleton().GetMainSceneRenderer();
	if( m_CurMapID >= 0 )
	{
		m_SelectMgr.onLeaveMap();
		BulletManager::getSingleton().ClearBullet(true);
		ActorManager::getSingleton().DestroyAllActor( false, true );
		getMainPlayer()->OnLeaveMap();
	}
	WorldCityManager::getSingleton().ClearEffigy();
	GameTopCareer::getSingleton().ClearCareerEffigy();
	bool isSameMap = true;
	if (m_CurMapID != mapid)
	{
		m_CurMapID = mapid;
		isSameMap = false;
	}
	if (m_pCurMap != map)
	{
		if( m_pCurMap )
		{
			Root::getSingleton().setCurrentTerrainScene(NULL);
			m_pCurMap->getShowGameMap()->detachFromRenderer(prenderer);
			delete m_pCurMap;
		}
		m_pCurMap = map;
		m_pCurMap->getShowGameMap()->attachToRenderer(prenderer);
		Root::getSingleton().setCurrentTerrainScene((GameTerrainScene*)m_pCurMap->getShowGameMap()->getGameScene());
	}
	getMainPlayer()->OnEnterMap(m_pCurMap);
	if( getMainPlayer()->getActState().GetState() != GSACT_DEAD )
	{
		getMainPlayer()->ChangeActState( GSACT_STAND );
	}
	if (!isSameMap)
	{
		SoundSystem::getSingleton().ReleaseRes();
		m_pCurMap->getShowGameMap()->PreloadFiles();
	}
	{
		GameMapShower*		pGameMapShower = GameManager::getSingleton().getGameMapShower();
		MultiMapAutoFind&	mutilMapFinder = pGameMapShower->getMultiMapFindControl();

		std::vector<int>	nearMaps;
		int num = mutilMapFinder.getNextMapNum(mapid);
		nearMaps.resize(num);
		for (size_t i = 0; i < num; ++i)
		{
			nearMaps[i] = mutilMapFinder.getNextMapInfo(mapid, i);
		}
		ResourceManager::getSingleton().SetCurPatchFileMapId(mapid, nearMaps);
	}
	// 第一次进入地图重置下包边厚度
	BorderGameScene::getSingleton().setBorderStrength(2.6f);

	float	gridSize  = map->getShowGameMap()->getGridSize();
	int		mapWidth  = (int)(map->getShowGameMap()->getMapWidthGrid() * gridSize);
	int		mapHeight = (int)(map->getShowGameMap()->getMapHeightGrid() * gridSize);
	int		mapId	  = map->getShowGameMap()->GetMapID();

	GameManager::getSingleton().getGameMapShower()->GetMaxMapContal()->setMapInfo( mapWidth, mapHeight, mapid );
	GameManager::getSingleton().getGameMapShower()->GetMinMapContal()->setMapInfo( mapWidth, mapHeight, mapid );
	GameManager::getSingleton().SendUIEvent("GE_END_ENTERING_MAP");	
	GameQuest::getSingleton().updateNpcQuestTitle();
	if( mapid == CLAN_CITY_MAP_ID )
	{
		GameClanManager::getSingleton().onEnterClanMap();
	}		
	GameWarfogManager::getSingleton().reset();
	if (getMainPlayer()->isAFK())
	{
		getMainPlayer()->setAFK(false);
	}
	WorldCityManager::getSingleton().OnEnterMap(m_pCurMap);
	WorldCityManager::getSingleton().UpdateEffigy();
	WorldCityManager::getSingleton().UpdateMusic();
	GameTopCareer::getSingleton().UpdateCareerEffigy();
	getMainPlayer()->SetPosition(x, z);
	getMainPlayer()->CmdMoveStop();
	getMainPlayer()->stopBack();
	GameWizard::getSingleton().springByEnterMap( mapid );

	/// 设置PK强制模式
	//必须根据realmapId来取mapdef，有可能地图重用，但信息不同，例如21300,同时是跨服和本服据点战场的id，但是pkmode不同
	int realMapID = map->getRealId();
	const tagMapDef &mapdef = DBDataManager::getSingleton().getMapDef(realMapID);
	if (mapdef.MapLimit.PK == 0)
	{
		GameActor_PVP::getSingleton().forcePkMode(PK_MODE_NORMAL);
	}
	else
	{
		if (mapdef.MapLimit.PKMode != 0)
		{
			GameActor_PVP::getSingleton().forcePkMode(mapdef.MapLimit.PKMode);
		}
		else
		{
			GameActor_PVP::getSingleton().forcePkMode(-1);
		}
	}
}

void GamePlayManager::leaveMap()
{
	//公会地图
	if( m_CurMapID == CLAN_CITY_MAP_ID )
	{
		GameClanManager::getSingleton().onLeaveClanMap();
	}
	if( m_CurMapID == HOMELAND_MAP_ID ) {
		GameHomelandManager::getSingleton().resetHomelandStatus();
	}
	if( m_CurMapID >= 0 )
	{
		m_SelectMgr.onLeaveMap();
		BulletManager::getSingleton().ClearBullet(true);
		ActorManager::getSingleton().DestroyAllActor( false, true );
	}
	leavePwolrd( m_CurMapID );
	WorldCityManager::getSingleton().ClearEffigy();
	GameTopCareer::getSingleton().ClearCareerEffigy();

	if( getMainPlayer() )
	{
		getMainPlayer()->OnLeaveMap();
	}

	Ogre::SceneRenderer *prenderer = GameManager::getSingleton().GetMainSceneRenderer();
	if( m_pCurMap != NULL )
	{
		m_pCurMap->getShowGameMap()->detachFromRenderer(prenderer);
		delete m_pCurMap;
		m_pCurMap	= NULL;
	}
	SoundSystem::getSingleton().ReleaseRes();
	m_CurMapID  = MAPID_NONE;
}

MainGamePlayer* GamePlayManager::enterWorld(int mapid, int x, int z, int player_id, int player_resid)
{
	ActorManager *pactormgr = ActorManager::getSingletonPtr();

	if( getMainPlayer() != NULL )
	{
		pactormgr->DestroyActor( getMainPlayer(), false );
	}

	MainGamePlayer* pMainPlayer = static_cast<MainGamePlayer *>( pactormgr->CreateActor( player_id, GAT_MAINPLAYER, player_resid ) );

	m_isInWorld = true;
	return pMainPlayer;
}

MainGamePlayer* GamePlayManager::getMainPlayer()
{
	ActorManager* pactormgr = ActorManager::getSingletonPtr();
	return pactormgr->getMainPlayer();
}

void GamePlayManager::setCameraAngle( int nRealMapId )
{
	m_PlayerCameraDir.m_nAngle = (unsigned int)(getMainCamera()->getAngleLR());
	m_PlayerCameraDir.m_nMapId = nRealMapId;
	GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_CAMERADIR, (char*)&m_PlayerCameraDir );
}

int GamePlayManager::getCameraAngle()
{
	int nCameraAngle = m_PlayerCameraDir.m_nAngle; 
	m_PlayerCameraDir.m_nAngle = 0;
	m_PlayerCameraDir.m_nMapId = 0;
	GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_CAMERADIR, (char*)&m_PlayerCameraDir );
	return nCameraAngle;
}

bool GamePlayManager::isMapLoadOK()
{
	if (!m_LoadThread.empty())
	{
		return false;
	}
	return true;
}

void GamePlayManager::trySwapMap()
{
	if (!m_isFaded)
	{
		return;
	}
	if (m_LoadThread.empty())
	{
		return;
	}
	HANDLE handle = (HANDLE)m_LoadThread.front().first;
	if (WaitForSingleObject(handle, 0) != WAIT_TIMEOUT)
	{
		DWORD result;
		GetExitCodeThread(handle, &result);
		CloseHandle(handle);
		GameMap *otherMap = (GameMap *)result;
		if (m_LoadThread.size() == 1)
		{
			LoginManager::getSingleton().onMapPreLoaded(otherMap, m_LoadThread.front().second);
			GameManager::getSingleton().GetNetworkMgr()->SetBusy(false);
			ResourceManager::getSingleton().SetLoadMap( false );
		}
		else
		{
			delete otherMap;
		}
		m_LoadThread.pop_front();
	}
}

void GamePlayManager::preLoadMap(const CSENTERMAP& entermap)
{
	m_isFaded = true;
	RoleText::SetLoadMap( !m_isFaded );
	GameManager::getSingleton().GetNetworkMgr()->SetBusy(true);
	if (!LoginManager::getSingleton().isFirstLoad())
	{
		//Ogre::BloomEffect::getSingleton().playRadialBloom(1500, true);
		//GameManager::getSingleton().getScriptVM()->callString("MapLoadingMask:Show()");
	}
	GameStateManager::changeGameState( GameStateManager::STATE_MAPLOADING );
	GamePlayManager::getSingleton().leaveMap();
	GameMap *map = new GameMap;
	ResourceManager::getSingleton().SetLoadMap( true );
	{
		GameMapShower*		pGameMapShower = GameManager::getSingleton().getGameMapShower();
		MultiMapAutoFind&	mutilMapFinder = pGameMapShower->getMultiMapFindControl();

		std::vector<int>	nearMaps;
		int num = mutilMapFinder.getNextMapNum(entermap.Map);
		nearMaps.resize(num);
		for (size_t i = 0; i < num; ++i)
		{
			nearMaps[i] = mutilMapFinder.getNextMapInfo(entermap.Map, i);
		}
		ResourceManager::getSingleton().SetCurPatchFileMapId(entermap.Map, nearMaps);
	}
	uintptr_t handle = map->asynLoad(entermap.Map, entermap.RealMap, entermap.x, entermap.y);
	m_LoadThread.push_back(std::pair<uintptr_t, CSENTERMAP>(handle, entermap));
}

void GamePlayManager::leveMapSoon()
{

}

void GamePlayManager::startFade()
{
	m_isFaded = false;
	RoleText::SetLoadMap( !m_isFaded );
	GameManager::getSingleton().SendUIEvent("GE_MAP_LOAD");
}

void GamePlayManager::finishFade()
{
	m_isFaded = true;
	RoleText::SetLoadMap( !m_isFaded );
}

void GamePlayManager::ShowLoading(bool show)
{
	m_isLoading = show;
}

void GamePlayManager::setFashionChangeAngle(float pullDist)
{
	if( m_pMainCamera ) 
	{
		m_pMainCamera->setClamp(true);
		m_prevTarget = m_pMainCamera->getCameraCenter();
		m_pMainCamera->PullInOut(pullDist);
	}
}

void GamePlayManager::resetPlayAngle()
{
	if(m_pMainCamera)
	{
		m_pMainCamera->Reset(m_prevTarget);
		m_pMainCamera->setClamp(false);
	}
}