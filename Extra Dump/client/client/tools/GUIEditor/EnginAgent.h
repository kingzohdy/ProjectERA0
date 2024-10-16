#pragma once
#include "OgreWorldPos.h"
#include "OgreColourValue.h"
#include "OgreSceneRendererPostEffect.h"

class GameUI;
class EditUI;

class CEnginAgent
{
public:
	CEnginAgent(void);
	~CEnginAgent(void);

	//singleton
public:
	static CEnginAgent* GetInstance();
	static void CreateInstance();
	static void ReleaseInstance();
private:
	static CEnginAgent* ms_pInst;

public:
	bool LoadConfigFile(Ogre::XMLData *pxmldata);
	bool Initialize(HWND hWnd);
	void DestroyScene();
	
	int	 OnWindowMsg(unsigned int msgid, unsigned int wparam, unsigned int lparam);
	void Update(long lTick);
	void Draw(long lTick);
	void SetRatio(float fRatio) { m_fRatio = fRatio; };
public:
	bool m_bIsInitialized;	// 是否加载完毕

public:
	HWND					m_hWnd;
	Ogre::GameScene*		m_pRoom;
	Ogre::SceneRenderer*	m_pSceneRenderer;
	Ogre::Root*				m_pEngineRoot;
	Ogre::Camera*			m_pCamera;
	
	float					m_fRatio;
	float					m_fSpeed;			// 帧率
	int						m_FrameLimit;		// 帧速限制
	Ogre::ColorQuad			m_backColor;		// 背景颜色
	
	int						m_nViewportWidth;
	int						m_nViewportHeight;

public:
	Ogre::UIRenderer*		m_pUIRenderer;
	Ogre::InputManager*		m_pInputMgr;		// 输入控制

	Ogre::ScriptVM*			m_pScriptVM;		// 只为兼容UILIB实际用不上
	GameUI*					m_pGameUI;
	EditUI*					m_pEditUI;
};
