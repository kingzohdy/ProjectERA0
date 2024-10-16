
#pragma once
#include "OgreWorldPos.h"
#include "OgreColourValue.h"
#include "OgreSceneRendererPostEffect.h"
#include "GameEngineTypes.h"

class CEnginAgent
{
public:
	//
	void LoadGameMap( INT MapID, Ogre::WorldPos& pos);
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

private:
	bool IsKeyDown(int key);

public:

	bool Initialize(HWND hWnd);
	void Draw(long lTick);
	void Update(long lTick);
	void CreateScene();
	void DestroyScene();

	void OnResetDev();

	void SetBGColor(Ogre::ColorQuad color);
	void SetSpeed(float fValue);
	float GetSpeed();
	void SetAmbientLight(Ogre::ColourValue light);

	void SetDistort(bool bDistort);
	bool GetDistort();
	bool m_bDistort;
	//void AttachEffect(Ogre::SceneNode* pEffect);
	//void DetachAllEffect();

	//void AttachSelectBox(Ogre::SceneNode* pEffect);
	//void DetachAllSelectBox();

	void SetRatio(float fRatio) { m_fRatio = fRatio; };

	void ShowBone(bool bShowBone) { m_bShowBone = bShowBone; };
	bool IsShowBone() { return m_bShowBone; };

	void AddContainerBox(Ogre::Vector3 pos, float range, Ogre::ColorQuad color);

	void RenderBone(Ogre::Model* pnode);

	void AttachActor(Ogre::Model* pnode);
	void DetachActor(Ogre::Model* pnode);
	void SetFrameLimit(int iLimit);
	int GetFrameLimit() { return m_FrameLimit;};

	void SetHightLightPos(int iPos, bool bIsBone) { m_iHightLightPos = iPos; m_bIsBone = bIsBone; };
	int GetHightLightBone() { return m_iHightLightPos; };
	void RenderHightLight(long tick);
	CCameraCtrl* GetCameraCtrl() {  return m_pCameraCtrl; };

	void SetBackNode(char* strpath);

	//在场景中设置特效的位置
	void FortyFiveView(); //场景中45度视角
	void SetEffectPos(CPoint pt, int flag = 0); //0: Actor  1:Target
	Ogre::Vector3 ComputeintersectionPos();

	void ShowGround(bool bShow);
	bool IsShowGround();

	void SetPostEffect(std::vector<Ogre::PostproPhaseType>& effectlist);
	void ClearPostEffect();

	void RequestEngineDevice();
	void ReleaseEngineDevice();

	void EnableGlow(bool bEnable);
	bool IsGlowEnable();
	bool IsEditModel() { return m_bIsEditMode; }
	void SetEditModel(bool bMode);

	Ogre::Root *m_pEngineRoot;
	Ogre::ScriptVM *m_pScriptVM;

	Ogre::PostEffectRenderer*	m_pPERenderer;

	bool					m_bShowGround;
	bool					m_bIsBone;

	bool					m_bEnableGlow;

	int						m_iHightLightPos;
	bool					m_bShowBone;
	bool					m_bIsKeyTrans;
	bool					m_bIsInitialized;
	HWND					m_hWnd;
	Ogre::Camera*		m_pCamera;
	Ogre::Light*			m_pLightNode;
	CSceneGround*			m_pGround;
	Ogre::SceneNode*			m_pBackNode;

	Ogre::GameScene*				m_pRoom;
	Ogre::RenderLines    *m_pLineRenderer;
	Ogre::SceneRenderer *m_pSceneRenderer;

	float					m_fRoomYaw;
	float					m_fRoomPitch;

	HANDLE					m_hMutex;

private:
	CCameraCtrl*			m_pCameraCtrl;
	Ogre::Model*		m_pAttachedNode;
	float					m_ViewPitch;
	Ogre::Vector3			m_Viewpoint;
	float					m_ViewYaw;

	float					m_fSpeed;

	int						m_FrameLimit;
	float					m_fRatio;
	Ogre::ColorQuad			m_backColor;

	bool					m_bIsEditMode;
public:
	//GC::IGCEditor*				m_GCEditor;
};