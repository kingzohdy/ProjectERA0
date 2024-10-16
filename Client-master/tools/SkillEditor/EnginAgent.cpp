#include "StdAfx.h"
#include "EnginAgent.h"
#include "OgreCamera.h"
#include "OgreGameTerrainScene.h"

#include "OgreRoot.h"
#include "OgreLight.h"
#include "OgreLines.h"

#include "OgreRenderWindow.h"
#include "OgreSceneManager.h"
#include "OgreEntity.h"
#include "OgreModel.h"
#include "OgreSkeleton.h"
#include "OgreGameTerrainScene.h"
#include "OgreRay.h"
#include "ogrebloom.h"
#include "OgreUIRenderer.h"
#include "OgreXMLData.h"
#include "OgreInputManager.h"

#include "GameDBData.h"
#include "GameActor.h"
#include "GameCamera.h"
#include "ShowActor.h"
#include "GameMap.h"
#include "ShowGameMap.h"
#include "ActorCtrl.h"
#include "GameMsgPrint.h"
#include "GameSkill_Mgr.h"
#include "MainFrm.h"
#include "SkillEditor.h"
#include "Item.h"

using namespace Ogre;

static void PreSceneRenderUI()
{

}

CEnginAgent::CEnginAgent(void)
:	m_bIsInitialized( false ), 
	m_hWnd(NULL),
	m_pRoom(NULL),
	m_FrameLimit(30),
	m_fSpeed(1.0f),
	m_fRatio(1.5f),
	m_pMainPlayer(NULL),
	m_CurMapID(-1),
	m_pActorMgr(NULL),
	m_pUIRenderer(NULL),
	m_pNetMgr(NULL),
	m_pCurMap(NULL),
	m_pInputMgr(NULL),
	m_pGameSkillMgr(NULL)
{
	m_backColor   = Ogre::ColorQuad(0, 0, 0, 255);
}

CEnginAgent::~CEnginAgent(void)
{
	OGRE_RELEASE(m_pRoom);
	
	if(m_pDBMgr) delete m_pDBMgr;
	if(m_pUIRenderer) delete m_pUIRenderer;
	
	//if(m_pActorMgr) delete m_pActorMgr;
	
	delete m_pGameMsgPrint;
	delete m_pActionCtrl;
	delete m_pInputMgr;
	delete m_pNetMgr;
	delete m_pMainCamera;
	delete m_pGameSkillMgr;

	delete m_pEngineRoot;
	delete m_pSceneRenderer;	
}

void CEnginAgent::CreateInstance()
{
	if(ms_pInst != NULL)
		delete ms_pInst;
	ms_pInst = new CEnginAgent();
}

CEnginAgent* CEnginAgent::GetInstance() 
{ 
	return ms_pInst; 
}

void CEnginAgent::ReleaseInstance() 
{
	if( ms_pInst != NULL ) 
		delete ms_pInst;
	ms_pInst = NULL;
}

bool CEnginAgent::LoadConfigFile(Ogre::XMLData *pxmldata)
{
	XMLNode root = pxmldata->getRootNode();

	XMLNode winnode = root.getChild("RenderSystem").getChild("MainWindow");
	m_Config.win_width = winnode.attribToInt("width");
	m_Config.win_height = winnode.attribToInt("height");
	m_Config.win_fullscreen = winnode.attribToBool("fullscreen");

	XMLNode pnode = root.getChild("ResDB");
	if(!pnode.isNull())
	{
		if(strcmp(pnode.attribToString("mode"), "db") == 0) m_Config.resdb_mode = RESDB_DB;
		else m_Config.resdb_mode = RESDB_FILE;

		XMLNode pfilenode = pnode.getChild("file");
		if(!pfilenode.isNull())
		{
			strcpy(m_Config.resdb_dir, pfilenode.attribToString("dir"));
		}
		XMLNode pdbnode = pnode.getChild("db");
		if(!pdbnode.isNull())
		{
			strcpy(m_Config.resdb_server, pdbnode.attribToString("server"));
			strcpy(m_Config.resdb_user, pdbnode.attribToString("user"));
			strcpy(m_Config.resdb_passwd, pdbnode.attribToString("passwd"));
			strcpy(m_Config.resdb_database, pdbnode.attribToString("database"));
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool CEnginAgent::Initialize(HWND hWnd)
{
	m_pEngineRoot = new Ogre::Root("client.cfg");
	if(!LoadConfigFile(&m_pEngineRoot->m_Config))
	{
		return false;
	}

	m_hWnd = hWnd;
	RECT rect;
	::GetClientRect(hWnd, &rect);

	Root::getSingleton().initRenderSystem(hWnd);
	
	NormalSceneRenderer *scenerenderer = new NormalSceneRenderer();
	scenerenderer->setClearParams(CLEAR_TARGET|CLEAR_ZBUFFER, 0xff000000, 1.0f, 0);
	scenerenderer->setRenderTarget(RenderSystem::getSingleton().getMainWindow());
	scenerenderer->setCamera(NULL);
	scenerenderer->setRenderScene(NULL);
	SceneManager::getSingleton().addSceneRenderer(0, scenerenderer);
	m_pSceneRenderer = scenerenderer;

	m_pMainCamera = new FollowCamera;

	int w = abs( rect.left - rect.right );
	int h = abs( rect.top - rect.bottom );
	m_pMainCamera->GetCameraNode()->setRatio(float(w)/float(h));
	m_pMainCamera->GetCameraNode()->setViewport(0, 0, w, h);
	
	m_pSceneRenderer->setCamera(m_pMainCamera->GetCameraNode());

	Ogre::SceneManager::getSingleton().addSceneRenderer(0, m_pSceneRenderer);
	m_pUIRenderer = new UIRenderer;
	m_pUIRenderer->setPreRenderCallback(PreSceneRenderUI);
	SceneManager::getSingleton().addSceneRenderer(1, m_pUIRenderer);
	
	m_pInputMgr		= new InputManager;
	m_pActionCtrl	= new CActorCtrl;
	m_pDBMgr		= new DBDataManager;
	m_pNetMgr		= INetworkManager::Create();
	m_pGameMsgPrint = new GameMsgPrint;
	m_pGameSkillMgr	= new SkillManager;

	m_pDBMgr->LoadData( m_Config );
	m_pInputMgr->RegisterInputHandler( m_pActionCtrl );

	m_pActorMgr = new ActorManager;
	
	enterWorld( FRIST_MAP_ID, 2284, 7405, MAINPALYER_ID, 100001 );

	m_pGameSkillMgr->getSpellControler()->setUsedInTool( true );

	initSkillDyAttrib();

	m_bIsInitialized = true;
	return true;
}

void CEnginAgent::DestroyScene()
{
	
}

int CEnginAgent::OnWindowMsg(unsigned int msgid, unsigned int wparam, unsigned int lparam)
{
	if(m_pInputMgr) 
	{
		m_pInputMgr->OnWindowMessage(m_hWnd, msgid, wparam, lparam);
		m_pInputMgr->ShowSystemCursor(true);
	}

	switch(msgid)
	{
	case WM_SIZE:
		{
			Ogre::Camera* pcamera = m_pMainCamera->GetCameraNode();
			if( pcamera != NULL )
			{
				m_nViewportWidth	= LOWORD(lparam);
				m_nViewportHeight	= HIWORD(lparam);
				pcamera->setViewport( 0, 0, m_nViewportWidth, m_nViewportHeight );
				pcamera->setRatio( float(m_nViewportWidth)/m_nViewportHeight );
			}
		}
		break;

	case WM_CLOSE:
		{
			PostQuitMessage(0);
		}
		break;

	default:
		break;
	}

	return 0;
}

void CEnginAgent::Update(long lTick)
{

}

void CEnginAgent::Draw(long lTick)
{	
	if( m_pInputMgr != NULL )
	{
		m_pInputMgr->Dispatch();
	}
	
	if( m_pActionCtrl != NULL )
	{
		m_pActionCtrl->update(lTick);
	}

	ActorManager::getSingleton().Update(lTick * m_fSpeed);
	SkillManager::getSingleton().update( lTick );

	if( m_pMainPlayer )
	{
		Ogre::WorldPos center = m_pMainPlayer->GetPosition();
		center.y += Ogre::WorldPos::Flt2Fix(100.0f);

		m_pMainCamera->Update(center, lTick);

		if(m_pCurMap) m_pCurMap->update(lTick, m_pMainCamera->GetCameraNode(), center);
	}

	Ogre::SceneManager::getSingleton().doFrame();
}

void CEnginAgent::enterWorld(int mapid, int x, int z, int player_id, int player_resid)
{
	// create mainPlayer
	ActorManager *pactormgr = ActorManager::getSingletonPtr();

	if( m_pMainPlayer != NULL)
	{
		pactormgr->DestroyActor( m_pMainPlayer );
	}

	m_pMainPlayer = static_cast<MainGamePlayer *>( pactormgr->CreateActor( player_id, GAT_MAINPLAYER, player_resid ) );

	enterMap( mapid, x, z );	
}

void CEnginAgent::enterMap( int mapid, int x, int z )
{
	if( mapid != m_CurMapID )
	{
		GameMap *pmap = new GameMap;
		if(!pmap->Load(mapid, mapid, x, z))
		{
			delete pmap;
			return;
		}

		if( m_CurMapID >= 0 ) m_pMainPlayer->OnLeaveMap();

		delete m_pCurMap;
		m_pCurMap = pmap;
		m_CurMapID = mapid;

		Ogre::SceneRenderer *prenderer = m_pSceneRenderer;
		m_pCurMap->getShowGameMap()->attachToRenderer(prenderer);
		ActorManager::getSingleton().DestroyAllActor( false, true );

		m_pMainPlayer->OnEnterMap(m_pCurMap);
	}

	m_pMainPlayer->SetPosition(x, z);
	m_pMainPlayer->setSpeed( 400 );

	m_pMainPlayer->ChangeActState(GSACT_MOVE);
	CEnginAgent::GetInstance()->m_pMainPlayer->m_Equip.m_Arm[EQUIP_POS_RIGHTHAND].m_ItemType = ITEM_SLOTTYPE_AMM;
	CEnginAgent::GetInstance()->m_pMainPlayer->m_Equip.m_Arm[EQUIP_POS_RIGHTHAND].m_bCanUsed = true;
	CEnginAgent::GetInstance()->m_pMainPlayer->m_Equip.m_Arm[EQUIP_POS_RIGHTHAND].m_ItemData.m_Arm.DefID = 1251601;
	m_pMainPlayer->m_Equip.changEquip( EQUIP_POS_RIGHTHAND, 1251601 );
	if (m_pMainPlayer->getAvatar())
	{
		m_pMainPlayer->getAvatar()->updateAttrib();
	}
}

void CEnginAgent::createActor( int monsterId )
{
	static int monster_id = MONSTER_START_ID;
	monster_id++;

	GameActor* pactor = ActorManager::getSingleton().CreateActor( monster_id, GAT_MONSTER, monsterId );
	pactor->setName( "L E O" );
	pactor->setMaxHP( 100 );
	pactor->setHP( 100 );
	pactor->OnEnterMap( m_pMainPlayer->getCurMap() );
	pactor->ChangeActState(GSACT_STAND);

	pactor->SetPosition( m_pMainPlayer->GetPosition().x/10, m_pMainPlayer->GetPosition().z/10 );
	float dir = (float)(rand()%3600)/10;
	pactor->getShowActor()->getCurEntity()->setRotation( dir, 0, 0 );

	pactor->doBack( 0, m_pMainPlayer->GetPosition().x/10, m_pMainPlayer->GetPosition().z/10, 200, BACK_CURPOST, 2000 );
}

void CEnginAgent::initSkillDyAttrib()
{
	std::vector<tagSkillDef> skillinfo;
	DBDataManager::getSingleton().loadSkillDef(skillinfo);
	for( std::vector<tagSkillDef>::iterator iter = skillinfo.begin(); iter != skillinfo.end(); ++iter )
	{
		tagSkillDyAttrib* pDyAttrib = new tagSkillDyAttrib;
		pDyAttrib->SkillLevel								= 1;
		pDyAttrib->CollectNum							= iter->CollectNumMax;
		pDyAttrib->CollectNumMax						= iter->CollectNumMax;
		m_pGameSkillMgr->setSkillDyAttrib( iter->SkillID, pDyAttrib );
	}
	((CMainFrame*)theApp.GetMainWnd())->m_wndFightWorkSpace.initSkillAttribPropValue();

	// 加载技能模板
	((CMainFrame*)theApp.GetMainWnd())->m_wndCreatSkillWorkSpace.initSkillTemplate();
}

CEnginAgent* CEnginAgent::ms_pInst = NULL;