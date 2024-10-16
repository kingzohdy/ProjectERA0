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
#include "MainFrm.h"
#include "OgreScriptLuaVM.h"
#include "ui_gameui.h"
#include "ui_framemgr.h"
#include "ui_editui.h"

using namespace Ogre;

static void PreSceneRenderUI()
{
	CEnginAgent::GetInstance()->m_pGameUI->Render();
	CEnginAgent::GetInstance()->m_pEditUI->Render();
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
	return true;
}

bool CEnginAgent::Initialize(HWND hWnd)
{
	m_pEngineRoot =  new Ogre::Root("ogre_fxeditor.cfg");
	m_pEngineRoot->Initlize();

	m_hWnd = hWnd;
	RECT rect;
	rect.left	= 0;
	rect.top	= 0;
	rect.bottom	= DEFAULT_UI_HEIGHT;
	rect.right	= DEFAULT_UI_WIDTH;
	//::GetClientRect(hWnd, &rect);
	Root::getSingleton().initRenderSystem(hWnd);
	
	// scene
	NormalSceneRenderer *scenerenderer = new NormalSceneRenderer();
	m_pRoom						= new Ogre::SimpleGameScene;
	m_pRoom->m_RenderDummyNode	= true;
	ColorQuad colorQ(118, 65, 26);
	scenerenderer->setClearParams(CLEAR_TARGET|CLEAR_ZBUFFER, colorQ.c, 1.0f, 0);
	scenerenderer->setRenderTarget(RenderSystem::getSingleton().getMainWindow());
	scenerenderer->setCamera(NULL);
	scenerenderer->setRenderScene(m_pRoom);
	m_pSceneRenderer = scenerenderer;
	SceneManager::getSingleton().addSceneRenderer(0, scenerenderer);
	// camera
	m_pCamera = new Camera;
	int w = abs( rect.left - rect.right );
	int h = abs( rect.top - rect.bottom );
	m_pCamera->setRatio( float(w)/float(h) );
	m_pCamera->setViewport( 0, 0, w, h );
	m_pSceneRenderer->setCamera( m_pCamera );
	Ogre::SceneManager::getSingleton().addSceneRenderer( 0, m_pSceneRenderer );
	// uicallback
	m_pUIRenderer = new UIRenderer;
	m_pUIRenderer->setPreRenderCallback( PreSceneRenderUI );
	SceneManager::getSingleton().addSceneRenderer( 1, m_pUIRenderer );
	
	// init UIsystem
	m_pInputMgr		= new InputManager;
	
	m_pScriptVM		= new Ogre::ScriptVM;
	m_pEditUI		= new EditUI;
	m_pGameUI		= new GameUI;
	if(!m_pGameUI->Create( "uires\\editorDef.toc", rect.right, rect.bottom, m_pUIRenderer, m_pScriptVM ) )
	{
		return false;
	}
	/*if(!m_pGameUI->Create( "uires\\default.toc", rect.right, rect.bottom, m_pUIRenderer, m_pScriptVM ) )
	{
		return false;
	}*/
	//g_pFrameMgr->m_bEditor = true;  
	//g_pFrameMgr->SetEditMode( UI_MODE_DEBUG );
	g_pFrameMgr->SetEditMode( UI_MODE_DEFAULT );
	m_pInputMgr->RegisterInputHandler(m_pEditUI);
	m_pInputMgr->RegisterInputHandler(m_pGameUI);

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
			if( m_pCamera != NULL )
			{
				m_nViewportWidth	= DEFAULT_UI_WIDTH; //LOWORD(lparam);
				m_nViewportHeight	= DEFAULT_UI_HEIGHT; //HIWORD(lparam);
				m_pCamera->setViewport( 0, 0, m_nViewportWidth, m_nViewportHeight );
				m_pCamera->setRatio( float(m_nViewportWidth)/m_nViewportHeight );
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
	if( m_pInputMgr != NULL )
	{
		m_pInputMgr->Dispatch();
	}

	m_pGameUI->Update(lTick);
}

void CEnginAgent::Draw(long lTick)
{	
	
	Ogre::SceneManager::getSingleton().doFrame();
}

CEnginAgent* CEnginAgent::ms_pInst = NULL;