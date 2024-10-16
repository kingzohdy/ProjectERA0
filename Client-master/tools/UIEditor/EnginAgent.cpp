#include "StdAfx.h"
#include "EnginAgent.h"
#include "OgreCamera.h"
#include "OgreRoot.h"

#include "OgreRenderWindow.h"
#include "OgreSceneManager.h"
#include "OgreGameTerrainScene.h"
#include "OgreRay.h"
#include "OgreUIRenderer.h"
#include "OgreXMLData.h"
#include "OgreInputManager.h"
#include "GameCamera.h"
#include "ShowGameMap.h"
#include "MainFrm.h"

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
	m_pUIRenderer(NULL),
	m_pInputMgr(NULL)
{
	m_backColor   = Ogre::ColorQuad(0, 0, 0, 255);
}

CEnginAgent::~CEnginAgent(void)
{
	OGRE_RELEASE(m_pRoom);
	if(m_pUIRenderer) delete m_pUIRenderer;
	
	delete m_pInputMgr;
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

	m_pEngineRoot->Initlize();
	Root::getSingleton().initRenderSystem(hWnd);
	
	NormalSceneRenderer *scenerenderer = new NormalSceneRenderer();
	scenerenderer->setClearParams( CLEAR_TARGET|CLEAR_ZBUFFER, 0xff000000, 1.0f, 0 );
	scenerenderer->setRenderTarget(RenderSystem::getSingleton().getMainWindow());
	scenerenderer->setCamera(NULL);
	scenerenderer->setRenderScene(NULL);
	SceneManager::getSingleton().addSceneRenderer(0, scenerenderer);
	m_pSceneRenderer = scenerenderer;

	//m_pMainCamera = new FollowCamera;

	//int w = abs( rect.left - rect.right );
	//int h = abs( rect.top - rect.bottom );
	//m_pMainCamera->GetCameraNode()->setRatio(float(w)/float(h));
	//m_pMainCamera->GetCameraNode()->setViewport(0, 0, w, h);
	//
	//m_pSceneRenderer->setCamera(m_pMainCamera->GetCameraNode());

	Ogre::SceneManager::getSingleton().addSceneRenderer(0, m_pSceneRenderer);
	m_pUIRenderer = new UIRenderer;
	m_pUIRenderer->setPreRenderCallback( PreSceneRenderUI );
	SceneManager::getSingleton().addSceneRenderer( 1, m_pUIRenderer );
	
	m_pInputMgr		= new InputManager;
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
			//Ogre::Camera* pcamera = m_pMainCamera->GetCameraNode();
			//if( pcamera != NULL )
			//{
			//	m_nViewportWidth	= LOWORD(lparam);
			//	m_nViewportHeight	= HIWORD(lparam);
			//	pcamera->setViewport( 0, 0, m_nViewportWidth, m_nViewportHeight );
			//	pcamera->setRatio( float(m_nViewportWidth)/m_nViewportHeight );
			//}
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

	Ogre::SceneManager::getSingleton().doFrame();
}

CEnginAgent* CEnginAgent::ms_pInst = NULL;