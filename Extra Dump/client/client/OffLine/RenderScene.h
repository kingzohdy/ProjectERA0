#pragma once

#include <Windows.h>
#include <OgreRenderSystem.h>
#include <OgreSceneRenderer.h>
#include <OgreRenderWindow.h>
#include <OgreGameScene.h>
#include <OgreCamera.h>
#include <OgreLight.h>
#include <OgreSceneManager.h>
#include <OgreUIRenderer.h>
#include "EditorModel.h"

#ifdef _DEBUG
#pragma comment(lib, "OgreMain_d.lib")
#pragma comment(lib, "GameEngine_d.lib")
#pragma comment(lib, "UILib_d.lib")
#pragma comment(lib, "luadll_d.lib")
#pragma comment(lib, "tolua++_d.lib")
#else
#pragma comment(lib, "OgreMain.lib")
#pragma comment(lib, "GameEngine.lib")
#pragma comment(lib, "UILib.lib")
#pragma comment(lib, "luadll.lib")
#pragma comment(lib, "tolua++.lib")
#endif

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Imm32.lib")
#pragma comment(lib, "atl")


class RenderUnit
{
public:
	RenderUnit(void *hwnd, bool bMain = false);
	~RenderUnit();
	
	void SetModel(const std::string strFilename);
	void update(unsigned int dtick);
	void setClearColor(unsigned int color);

public:
	int m_Type;
	Ogre::SceneRenderer *m_pRenderer;
	Ogre::UIRenderer *m_pUIRenderer;
	Ogre::GameScene *m_pScene;
	Ogre::Camera *m_pCamera;
	Ogre::RenderWindow *m_pTarget;

	std::string m_strFilename;
	EditorModel *m_pEditorModel;

protected:
	static int unitproperty;

};