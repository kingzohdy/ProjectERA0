
#include "OgreRoot.h"
#include "OgreRenderSystem.h"
#include "OgreRenderWindow.h"
#include <Windows.h>
#include "OgreBillboard.h"
#include "OgreBillboardData.h"
#include "OgreTexture.h"
#include "OgreCamera.h"
#include "OgreSceneRenderer.h"
#include "OgreHardwareBuffer.h"
#include "OgreGroundGrid.h"
#include "OgreGameScene.h"
#include "OgreArchive.h"
#include "OgreMesh.h"
#include "OgreModel.h"
#include "OgreModelData.h"
#include "OgreResourceManager.h"
#include "OgreLight.h"
#include "OgreSceneManager.h"
#include "OgreRenderWindow.h"
#include "OgreRenderSystem.h"
#include "OgreEntity.h"
#include "OgreUIRenderer.h"
#include "OgreSoundSystem.h"
#include "OgreGameTerrainScene.h"
#include "OgreTerrainData.h"
#include "OgreEntityMotionData.h"
#include "OgreEntityMotion.h"
#include "OgreShadowmap.h"
#include "ogrereflect.h"
#include "OgreDecal.h"
#include "ogreshadowcubemap.h"

#pragma comment(lib, "winmm.lib")

using namespace Ogre;

Camera *g_pCamera;
GroundGrid *g_pGrid;
GameScene *g_pScene;
Entity *g_pEntity;
Model*	g_model;
UIRenderer *g_pUIRenderer;
ReflectEffect *g_pRef;
HUIRES hres;
Ogre::HFONT hfont1, hfont2;
Shadowcubemap* g_pSCM = NULL;
void RenderUI()
{
	/*
	g_pUIRenderer->BeginDraw(hres);
	g_pUIRenderer->DrawRect(100, 100, 256, 256, 0xffffffff);
	g_pUIRenderer->EndDraw();

	g_pUIRenderer->renderText(hfont2, 0, "和气生财", 0, 100, ColorQuad(255,255,255));
	g_pUIRenderer->renderText(hfont1, 0, "4567773", 0, 50, ColorQuad(255,255,255));
	*/
	
}
Light* g_pLight;
Light* g_ppLight;

Entity* g_pEntities[100];
static void CreateData()
{
	/*
	g_pUIRenderer = new UIRenderer;
	g_pUIRenderer->setPreRenderCallback(RenderUI);
	hres = g_pUIRenderer->CreateTexture("uires\\texture\\hammer_premul.dds", BLEND_ALPHABLEND);
	hfont1 = g_pUIRenderer->CreateBitmapFont("uires\\fonts\\BloodNum.xml", CCODING_GBK);
	hfont2 = g_pUIRenderer->CreateTrueTypeFont(16, 16, "uires\\fonts\\FZHTJW.TTF", CCODING_GBK);

	g_pGrid = new GroundGrid;
	g_pGrid->setGridGround(200.0f, 200.0f, 10, 10);
	g_pGrid->update(0);
	*/

	//g_pScene = new SimpleGameScene;
	//SimpleGameScene *pterrscene = new SimpleGameScene();
	GameTerrainScene *pterrscene = new GameTerrainScene("test", 1, 1);
	TerrainTileSource *ptileres = static_cast<TerrainTileSource *>(ResourceManager::getSingleton().blockLoad("maps\\1002\\1002.tile"));
	pterrscene->addTerrainTile(0, 0, ptileres);
	g_pScene = pterrscene;

	//g_pRef = new ReflectEffect();
	//g_pRef->setGlobal(true);
	//g_pScene->onAttachObject(g_pRef);
	//g_pRef->release();

	//if(g_pGrid) g_pGrid->attachToScene(g_pScene);


	/*
	//Model *pmodel = loadModelFromFile("models\\aaa.omod");
	Model *pmodel = loadModelFromFile("models\\girl\\girl.omod");
	pmodel->setScale(Vector3(0.5f, 0.5f, 0.5f));
	pmodel->playAnim(0);

	pmodel->attachToScene(g_pScene);
	pmodel->release();
	*/

	Material* picemtl = new Material("zblend_stdmtl");
	Texture* pTexture = static_cast<Texture*>(ResourceManager::getSingletonPtr()->blockLoad("textures\\ice.dds"));
	picemtl->setParamTexture("g_DiffuseTex", pTexture);
	picemtl->setParamTexture("g_SpecSelfTex", pTexture);
	picemtl->setParamValue("g_SelfPower",&Vector3(0.5f,0.5f,0.5f));
	/*
	for(size_t i=0;i<1;i++)
	{
		g_pEntities[i] = new Entity;
		g_pEntities[i]->setScale(Vector3(1.0f, 1.0f, 1.0f));
		g_pEntities[i]->setRotation(0,0,0);	
		
		g_pEntities[i]->load("character\\player\\male.ent", true);

		g_pEntities[i]->playAnim(10100);

		WorldPos pos(i/10*1000.0f  ,0.0f,i%10*1000.0f );
		g_pEntities[i]->setPosition(pos);

		g_pEntities[i]->attachToScene(g_pScene);

		g_pEntities[i]->getMainModel()->SetRenderMtl(picemtl);
	}
	*/
	g_pEntities[0] = new Entity;
	g_pEntities[0]->setScale(Vector3(1.0f, 1.0f, 1.0f));
	g_pEntities[0]->setRotation(0,0,0);	

	g_pEntities[0]->load("character\\player\\male.ent", true);

	g_pEntities[0]->playAnim(10100);

	WorldPos pos(8000.0f,6000.0f,8000.0f);
	g_pEntities[0]->setPosition(pos);

	g_pEntities[0]->attachToScene(g_pScene);

	Light *plightp = new Light(LT_POINT);
	plightp->setColor(ColourValue(0.8f, 0.0f, 0.0f));
	plightp->m_Attenuation = 0.01f;
	plightp->m_Range = 800.0f;
	plightp->update(0);
	plightp->setPosition(Vector3(800.0f,900.0f,800.0f));
	plightp->attachToScene(g_pScene);
	plightp->enableShadow();
	g_ppLight = plightp;

	


	

	Light *plight = new Light(LT_DIRECT);
	plight->setRotation(0,45,0);
	plight->setColor(ColourValue(0.4f, 0.4f, 0.4f));
	plight->enableAmbient();
	plight->m_AmbientColor = ColourValue(0.0f, 0.0f, 0.0f);
	plight->m_DoSpecular = true;
	plight->m_SpecularColor = ColourValue(1.0f, 1.0f, 1.0f);
	plight->m_SpecularPower = 4.0f;
	plight->setGlobal(true);
	plight->update(0);
	plight->attachToScene(g_pScene);
	g_pLight = plight;

	g_pCamera = new Camera;
	g_pCamera->setRatio(800.0f/600.0f);
	g_pCamera->setLookAt(Vector3(800.0f, 1200.0f, 800.0f), Vector3(800.0f, 200.0f, 800.0f), Vector3(0.0f, 0.0f, 1.0f));
	g_pCamera->setDepth(1.0f,60000.0f);

	//DecalNode* pDecal = new DecalNode;
	//pDecal->setPosition( Vector3(1000.0f,0.0f,1000.0f) );
	//pDecal->SetRadius(100.0f);

	//g_pScene->onAttachObject(pDecal);




	//g_pCamera->setLookAt(Vector3(0.0f, 100.0f, -200.0f), Vector3(0,0,0), Vector3(0, 1.0f, 0));

	SoundSystem::getSingleton().setListener(&Vector3(0, 0, 0), &Vector3(0, 0, 0), &Vector3(0, 0, 1.0f), &Vector3(0, 1.0f, 0));
}

void DestroyData()
{
	
	//g_pRef->release();
	g_pLight->release();
	g_pScene->release();
	OGRE_RELEASE(g_pGrid);

	delete g_pUIRenderer;
}
int tick;
void UpdateData(uint dtick)
{
	
		
	g_pScene->update(dtick);
	
	tick += dtick;

	g_ppLight->setPosition(Vector3(800.0f+50.0f*Cos(0.05*tick),900.0f,800.0f+50.0f*Sin(0.05*tick)));
	g_pScene->m_pSCM->SetLightPosRange(g_ppLight->getWorldPosition(),400.0f);

	for(size_t i=0;i<1;i++)
	{
		g_pEntities[i]->update(dtick);;
	}

	//g_pGrid->update(dtick);
	g_pCamera->update(dtick);

	SoundSystem::getSingleton().update();
}

static void RenderWindowOnSizeMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(RenderSystem::getSingletonPtr() == NULL) return;
	RenderWindow *prwin = RenderSystem::getSingleton().findRenderWindow(hWnd);
	if(prwin == NULL) return;

	switch(uMsg)
	{
	case WM_ENTERSIZEMOVE:
		// Halt frame movement while the app is sizing or moving
		break;

	case WM_EXITSIZEMOVE:
		prwin->onSizeOrMove();
		break;

	case WM_MOVE:
		prwin->onSizeOrMove();

		break;

	case WM_DISPLAYCHANGE:
		prwin->onSizeOrMove();
		break;

	case WM_SIZE:
		if( SIZE_MINIMIZED == wParam )
		{
			prwin->setMaximized(false);
			prwin->setMinimized(true);
		}
		else if( SIZE_MAXIMIZED == wParam )
		{
			prwin->setMaximized(true);
			prwin->setMinimized(false);
			prwin->onSizeOrMove();
		}
		else if( SIZE_RESTORED == wParam )
		{
			if(prwin->getMaximized())
			{
				prwin->setMaximized(false);
				prwin->onSizeOrMove();
			}
			else if(prwin->getMinimized())
			{
				prwin->setMinimized(false);
				prwin->onSizeOrMove();
			}
			else
			{
				// If we're neither maximized nor minimized, the window size 
				// is changing by the user dragging the window edges.  In this 
				// case, we don't reset the device yet -- we wait until the 
				// user stops dragging, and a WM_EXITSIZEMOVE message comes.
			}
		}
		break;
	}
}

LRESULT CALLBACK D3DWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	RenderWindowOnSizeMove(hWnd, uMsg, wParam, lParam);

	switch( uMsg )
	{
	case WM_KEYUP:
		if(SoundSystem::getSingletonPtr())
		{
			SoundSystem *pss = SoundSystem::getSingletonPtr();
			if(wParam == 'A') 
			{
				pss->playSound2D("sounds\\sound\\Axe_hit_5_m120att.wav", 1.0f);
			}
			else if(wParam == 'S')
			{
				SoundCreateInfo3D sndinfo;
				sndinfo.isloop = false;
				sndinfo.mindistance = 1.0f;
				sndinfo.maxdistance = 1000.0f;
				sndinfo.volume = 1.0f;
				sndinfo.pos = Vector3(10.0f, 0, 0);
				sndinfo.velocity = Vector3(-10.0f, 0, 0);
				pss->playSound3D("sounds\\sound\\Axe_hit_5_m120att.wav", sndinfo);
			}
			else if(wParam == 'D')
			{
				pss->playMusic(0, "sounds\\music\\1.mp3", true, 6000);
			}
			else if(wParam == 'F')
			{
				pss->playMusic(0, "sounds\\music\\dm_DayBarrenDry02.mp3", true, 6000);
			}
			else if(wParam == VK_LEFT)
			{

			}
			else if(wParam == '1')
			{
				//g_pEntity->playAnim(1080);
				//g_pEntity->playMotion("hit");
			}
			else if(wParam == '2')
			{
				//g_pEntity->update(33, NULL);
			}
		}
		break;

	case WM_CLOSE:
		DestroyData();
		delete Root::getSingletonPtr();
		PostQuitMessage(0);
		break;

	case WM_POWERBROADCAST:
		switch( wParam )
		{
#ifndef PBT_APMQUERYSUSPEND
#define PBT_APMQUERYSUSPEND 0x0000
#endif
	case PBT_APMQUERYSUSPEND:
		// At this point, the app should save any data for open
		// network connections, files, etc., and prepare to go into
		// a suspended mode.
		return true;

#ifndef PBT_APMRESUMESUSPEND
#define PBT_APMRESUMESUSPEND 0x0007
#endif
	case PBT_APMRESUMESUSPEND:
		// At this point, the app should recover any data, network
		// connections, files, etc., and resume running from when
		// the app was suspended.
		return true;
		}
		break;

	case WM_SYSCOMMAND:
		// Prevent moving/sizing and power loss in fullscreen mode
		switch( wParam )
		{
		case SC_MOVE:
		case SC_SIZE:
		case SC_MAXIMIZE:
		case SC_MONITORPOWER:
			{
				if(RenderSystem::getSingletonPtr())
				{
					RenderWindow *prwin = RenderSystem::getSingleton().findRenderWindow(hWnd);
					if(prwin!=NULL && !prwin->getWindowed()) return true;
				}
			}
			break;

		case SC_SCREENSAVE:
		case SC_KEYMENU:
			return true;

		}
		break;
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

static const char *RENDERWINDOW_CLASSNAME = "RenderWindow_ClassName";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Root *root = new Root("ogre.cfg");

	WNDCLASS wndClass = { 0, D3DWndProc, 0, 256, (HINSTANCE)hInstance,
		NULL,
		NULL,
		NULL,
		NULL, RENDERWINDOW_CLASSNAME };
	RegisterClass(&wndClass);
	
	DWORD winstyle;
	XMLNode mainwinnode = root->m_Config.getRootNode().getChild("RenderSystem").getChild("MainWindow");
	if( mainwinnode.attribToBool("fullscreen") ) winstyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE;
	else winstyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE;

	// Set the window's initial width
	RECT rc;
	SetRect( &rc, 0, 0, mainwinnode.attribToInt("width"), mainwinnode.attribToInt("height") );
	AdjustWindowRect( &rc, winstyle, FALSE );

	// Create the render window
	HWND hwnd = CreateWindow( RENDERWINDOW_CLASSNAME, "EngineWindow", winstyle, CW_USEDEFAULT, CW_USEDEFAULT, (rc.right-rc.left), (rc.bottom-rc.top), 0, NULL, hInstance, 0 );

	if(!root->initRenderSystem(hwnd))
	{
		return -1;
	}

	CreateData();

	NormalSceneRenderer scenerenderer(RU_GENERAL);
	scenerenderer.setClearParams(CLEAR_TARGET|CLEAR_ZBUFFER, 0xff0000ff, 1.0f, 0);
	scenerenderer.setRenderTarget(RenderSystem::getSingleton().getMainWindow());
	scenerenderer.setCamera(g_pCamera);
	scenerenderer.setRenderScene(g_pScene);
	SceneManager::getSingleton().addSceneRenderer(0, &scenerenderer);
	//SceneManager::getSingleton().addSceneRenderer(1, g_pUIRenderer);

	unsigned int lasttick = timeGetTime();
	unsigned int accumtick = 0;
	unsigned int lastframe = SceneManager::getSingleton().getDrawFrame();
	while(true)
	{
		MSG kMsg;
		if(PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
		{
			if(kMsg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&kMsg);
			DispatchMessage(&kMsg);
		}
		else
		{
			unsigned int curtick = timeGetTime();

			UpdateData(curtick-lasttick);
			if(SceneManager::getSingletonPtr()) SceneManager::getSingleton().doFrame();

			accumtick += curtick-lasttick;
			if(accumtick >= 1000)
			{
				uint curframe = SceneManager::getSingleton().getDrawFrame();
				uint nframe = curframe - lastframe;

				float fps = nframe/(accumtick/1000.0f);
				char buffer[256];
				sprintf(buffer, "%.1f", fps);
				SetWindowText(hwnd, buffer);

				lastframe = curframe;
				accumtick = 0;
			}

			lasttick = curtick;
		}
	}
	
	return 0;
}