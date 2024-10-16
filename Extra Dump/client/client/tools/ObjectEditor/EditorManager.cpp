
#include "stdafx.h"
#include "EditorManager.h"
#include "CameraCtrl.h"
#include "EditorGround.h"
#include "EffectManager.h"
#include "GameDBData.h"

#include "OgreScriptLuaVM.h"
#include "OgreRoot.h"
#include "OgreGameScene.h"
#include "OgreCamera.h"
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
#include "OgreSoundSystem.h"

using namespace Ogre;

extern int  tolua_GameEngineTolua_open (lua_State* tolua_S);

Ogre::SceneRenderer *g_MainSceneRenderer;

CEnginAgent::CEnginAgent(void)
: m_bIsInitialized(false), m_pCamera(NULL), m_hWnd(NULL),m_pLightNode(NULL)
, m_fSpeed(1.0f), m_FrameLimit(30), m_pRoom(NULL), m_fRatio(1.5f)
, m_bShowBone(false), m_bIsKeyTrans(false)
, m_pGround(NULL), m_pAttachedNode(NULL)
, m_pCameraCtrl(NULL), m_pBackNode(NULL), m_pPERenderer(NULL),
m_bIsBone(true), m_bEnableGlow(true), m_bIsEditMode(false)
{
	m_iHightLightPos = -1;
	m_pEngineRoot = new Ogre::Root("ogre_fxeditor.cfg");
	m_pEngineRoot->Initlize();

	GameConfig cfg;
	LoadGameConfigFromXML(cfg, &m_pEngineRoot->m_Config);

	DBDataManager *pdbmgr = new DBDataManager;
	if(!pdbmgr->LoadData(cfg))
	{
		delete pdbmgr;
	}
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

bool CEnginAgent::Initialize(HWND hWnd)
{
	m_pScriptVM = new Ogre::ScriptVM;
	tolua_GameEngineTolua_open(m_pScriptVM->getLuaState());

	m_hWnd = hWnd;
	RECT rect;
	::GetClientRect(hWnd, &rect);

	m_pEngineRoot->initRenderSystem(hWnd);

	m_pRoom = new Ogre::SimpleGameScene;
	m_pRoom->m_RenderDummyNode = true;

	BloomEffect *pbloom = SceneManager::getSingleton().m_pBloom;
	pbloom->m_fHighScene = 0;
	pbloom->m_fBlur = 0;
	pbloom->m_fGrayValue = 0;

	m_bDistort = false;
	m_backColor = Ogre::ColorQuad(0, 0, 0, 255);
	m_pCameraCtrl = new CCameraCtrl();
	m_pCamera = new Ogre::Camera;

	Light *plight = new Light(LT_POINT);
	plight->setPosition(Vector3(0, 200.0f, -100.0f));
	plight->setColor(ColourValue(0.6f, 0.6f, 0.6f));
	plight->enableAmbient();
	plight->m_AmbientColor = ColourValue(0.4f, 0.4f, 0.4f);
	plight->m_DoSpecular = true;
	plight->m_SpecularColor = ColourValue(1.0f, 1.0f, 1.0f);
	plight->m_SpecularPower = 4.0f;
	plight->update(0);
	plight->attachToScene(m_pRoom);
	m_pLightNode = plight;

	m_pLineRenderer = new RenderLines;
	m_pLineRenderer->attachToScene(m_pRoom);

	m_pSceneRenderer = new Ogre::NormalSceneRenderer();
	m_pSceneRenderer->setCamera(m_pCamera);
	m_pSceneRenderer->setRenderScene(m_pRoom);
	m_pSceneRenderer->setRenderTarget(Ogre::RenderSystem::getSingleton().getMainWindow());
	g_MainSceneRenderer = m_pSceneRenderer;

	Ogre::SceneManager::getSingleton().addSceneRenderer(0, m_pSceneRenderer);

	m_hMutex = CreateMutex(NULL, FALSE, NULL);
	m_bIsInitialized = true;
	return true; 
}

CEnginAgent::~CEnginAgent(void)
{ 
	OGRE_RELEASE(m_pCamera);
	OGRE_RELEASE(m_pLightNode);
	OGRE_RELEASE(m_pLineRenderer);
	OGRE_RELEASE(m_pRoom);

	CloseHandle(m_hMutex);
	delete(m_pCameraCtrl);

	delete DBDataManager::getSingletonPtr();
	delete m_pEngineRoot;
	delete m_pSceneRenderer;
}

void CEnginAgent::SetDistort(bool bDistort)
{
	m_bDistort = bDistort;
}

bool CEnginAgent::GetDistort()
{
	return m_bDistort;
}

void CEnginAgent::OnResetDev()
{
	if(m_pGround) m_pGround->setGroundColor(Ogre::ColorQuad(255, 255, 255, 100));
}

void CEnginAgent::SetAmbientLight(Ogre::ColourValue light)
{
	if(m_pLightNode)
	{
		m_pLightNode->m_AmbientColor = light;
	}
}

void CEnginAgent::CreateScene()
{
	m_pGround = new CSceneGround(true);
	m_pGround->setLayer(RL_BACKGROUND);
	m_pGround->setStepColor(MakeRGBA(140, 80, 80), 10);
	//m_pGround->setStepColor(MakeRGBA(180, 180, 180), 10);
	m_pGround->setGridGround(5000.0f, 5000.0f, 100, 100);
	m_pGround->setPosition(Ogre::WorldPos((Ogre::WPOS_T)0.0f, (Ogre::WPOS_T)0.0f, (Ogre::WPOS_T)0.0f));
	m_pGround->attachToScene(m_pRoom);
	m_bShowGround = true;
}

void CEnginAgent::DestroyScene()
{
	OGRE_RELEASE(m_pGround);
	OGRE_RELEASE(m_pBackNode);
}

void CEnginAgent::RenderHightLight(long tick)
{
	static Ogre::ColorQuad colorList[] = { Ogre::ColorQuad(255, 0, 0), Ogre::ColorQuad(255, 255, 0), Ogre::ColorQuad(0, 0, 255), Ogre::ColorQuad(0, 255, 0)};
	static int iMaxColor = 4;

	Ogre::SceneNode* pNode = CEffectMgr::GetInstance()->m_pModelActor->getMainModel();
	if(!IS_KIND_OF(Ogre::Model, pNode))
		return;

	if(m_iHightLightPos >= 0 && m_bShowBone)
	{
		Ogre::Matrix4 boneTM;
		if(m_bIsBone == true && IS_KIND_OF(Ogre::Model, pNode))
		{
			Ogre::Model* pActorNode = (Ogre::Model*)CEffectMgr::GetInstance()->m_pModelActor->getMainModel();
			if(m_iHightLightPos >= (int)pActorNode->getSkeletonInstance()->m_Bones.size())
				return;
			boneTM = pActorNode->getSkeletonInstance()->getBoneTM(m_iHightLightPos);
		}
		else if(m_bIsBone == false)
		{
			assert(0);
		}
		else
		{
			return;
		}

		Ogre::RenderLines* pDebugger = m_pLineRenderer;
		Ogre::Vector3 pos0, pos1, pos2, pos3, pos4;

		pos0 = boneTM.getTranslate();
		Ogre::Vector3 facing = m_pCameraCtrl->m_Viewpoint - pos0;
		float length = facing.length();
		Ogre::Vector3 tmp = pos0 * m_pCamera->getViewMatrix();

		float dx = 0.01f * length;
		float dy = 0.01f * length;
		pos1 = tmp + Ogre::Vector3(-dx, -dy, 0);
		pos2 = tmp + Ogre::Vector3(dx, -dy, 0);
		pos3 = tmp + Ogre::Vector3(-dx, dy, 0);
		pos4 = tmp + Ogre::Vector3(dx, dy, 0);

		Ogre::Matrix4 invview = m_pCamera->getViewMatrix();
		invview.inverse();
		pos1 = pos1 * invview;
		pos2 = pos2 * invview;
		pos3 = pos3 * invview;
		pos4 = pos4 * invview;

		Ogre::ColorQuad lineColor = colorList[(tick / 100) % iMaxColor];
		pDebugger->addLine(pos0, pos1, lineColor);
		pDebugger->addLine(pos0, pos2, lineColor);
		pDebugger->addLine(pos0, pos3, lineColor);
		pDebugger->addLine(pos0, pos4, lineColor);

	}
}
void CEnginAgent::Update(long lTick)
{
	static long lLastTick = 0;
	long ldtime = lTick - lLastTick;
	long lMinDtime = 0;
	ldtime = lTick;
	//FrameLimit
	if(m_FrameLimit > 0)
	{
		Sleep(m_FrameLimit);
	}

	//if(ldtime > 100)
	//ldtime = 100;
	float dtime = ldtime / 1000.0f;	

	float speed = 80.0f;

	RenderHightLight(lTick);
	if(m_pBackNode != NULL)
	{
		Ogre::Vector3 offset = m_pBackNode->getWorldPosition();
		if(IsKeyDown(VK_SHIFT) && IsKeyDown('W'))
			offset.z += speed * dtime;
		if(IsKeyDown(VK_SHIFT) && IsKeyDown('S'))
			offset.z -= speed * dtime;
		if(IsKeyDown(VK_SHIFT) && IsKeyDown('A'))
			offset.x -= speed * dtime;
		if(IsKeyDown(VK_SHIFT) && IsKeyDown('D'))
			offset.x += speed * dtime;
		if(IsKeyDown(VK_SHIFT) && IsKeyDown('Q'))
			offset.y += speed * dtime;
		if(IsKeyDown(VK_SHIFT) && IsKeyDown('E'))
			offset.y -= speed * dtime;

		m_pBackNode->setPosition(Ogre::WorldPos(offset));
	}

	m_pCameraCtrl->Update(dtime);
	m_pCamera->update(ldtime);

	static float ttt = 0.0f;
	//tu211
	//m_pEngine->GetSceneMgr()->Update(Ogre::uint((ldtime) * m_fSpeed));
	if(IS_KIND_OF(Ogre::SimpleGameScene, m_pRoom))
	{
		m_pRoom->update((unsigned int)(ldtime*m_fSpeed));
	}


	//设置SetViewAreaCenter，让地图可以动态的加载
	if(IS_KIND_OF(Ogre::GameTerrainScene, m_pRoom))
	{
		Ogre::Vector3 vEyePos(m_pCameraCtrl->m_Viewpoint.x, 0, m_pCameraCtrl->m_Viewpoint.z);
		static_cast<Ogre::GameTerrainScene *>(m_pRoom)->updateFocusArea(vEyePos, 100.0f);
	}

	if (m_pGround != NULL) 
	{
		m_pGround->setPosition(Ogre::WorldPos((Ogre::WPOS_T)0.0f, (Ogre::WPOS_T)0.0f, (Ogre::WPOS_T)0.0f));
		m_pGround->update(ldtime);
	}

	SoundSystem::getSingleton().update();

	lLastTick = lTick;
}

void CEnginAgent::RenderBone(Ogre::Model* pnode)
{
	//if(m_bShowBone && pnode)
	//m_pEngine->GetSceneMgr()->GetDebugger()->RenderNode(pnode, Ogre::DBG_R_SKELETON);
}

void CEnginAgent::AddContainerBox(Ogre::Vector3 pos, float range,  Ogre::ColorQuad color)
{
	static unsigned short ind[] = { 0,1,  0,2,  0,4,  1,3,  1,5,  2,3,  2,6, 3,7, 4,5,  4,6, 5,7, 6,7};
	static Ogre::Vector3 dir[] = {	Ogre::Vector3 (1, 1, 1),   Ogre::Vector3 (-1, 1, 1),   Ogre::Vector3 (1,-1, 1),  Ogre::Vector3 (-1,-1, 1),
		Ogre::Vector3 (1, 1,-1),   Ogre::Vector3 (-1, 1,-1),   Ogre::Vector3 (1,-1,-1), Ogre::Vector3  (-1,-1,-1)};

	Ogre::RenderLines* pDbg = m_pLineRenderer;
	Ogre::Matrix4 tm;
	tm.makeTranslateMatrix(pos);
	for(int i = 0; i < 12; i++)
	{
		Ogre::Vector3 p1 = dir[ind[i * 2]] * range;
		Ogre::Vector3 p2 = dir[ind[i * 2 + 1]] * range;

		p1 = p1 * tm;
		p2 = p2 * tm;

		pDbg->addLine(p1, p2, color);
	}
}

void CEnginAgent::RequestEngineDevice()
{
	WaitForSingleObject(m_hMutex, INFINITE);
}

void CEnginAgent::ReleaseEngineDevice()
{
	ReleaseMutex(m_hMutex);
}

void CEnginAgent::Draw(long lTick)
{
	Ogre::Vector3 dir;
	dir.y = Ogre::Sin(m_pCameraCtrl->m_ViewYaw);
	dir.x = Ogre::Sin(m_pCameraCtrl->m_ViewPitch)*Ogre::Cos(m_pCameraCtrl->m_ViewYaw);
	dir.z = Ogre::Cos(m_pCameraCtrl->m_ViewPitch)*Ogre::Cos(m_pCameraCtrl->m_ViewYaw);

	lTick =(long)(lTick * m_fSpeed);
	//m_pCamera->SetLookDirect(m_pCameraCtrl->m_Viewpoint, dir, Ogre::Vector3(0, 1.0f, 0));

	//m_pCamera->SetFov(45.0f);

	//m_pCamera->SetDepth(1.0f, 200000.0f);

	m_pSceneRenderer->setClearParams(CLEAR_TARGET|CLEAR_ZBUFFER, m_backColor.c, 1.0f, 0);
	m_pCameraCtrl->UpdateCameraNode(m_pCamera);
	m_pCamera->setRatio(m_fRatio);

	float len = m_pCameraCtrl->m_Viewpoint.length();
	if (m_pGround != NULL) 
	{
		//if(len < 1000)
		m_pGround->SetGridSize(CSceneGround::SIZE_SMALL);
		//if(1000 < len && len < 10000)
		//	m_pGround->SetGridSize(CSceneGround::SIZE_MIDDLE);
		//if(len > 10000)
		//	m_pGround->SetGridSize(CSceneGround::SIZE_BIG);

	}

	RequestEngineDevice();

	//tu221
	/*
	if(IS_KIND_OF(Ogre::SimpleGameScene, m_pRoom))
	{

	Ogre::IRenderer *prenderer = m_pEngine->GetRenderer();
	Ogre::IRenderTarget *ptarget = prenderer->GetMainTarget();
	ptarget->SetClearParam(Ogre::CLEAR_TARGET|Ogre::CLEAR_ZBUFFER, m_backColor, 1.0f, 0);
	m_pEngine->GetSceneMgr()->SetClearColor(m_backColor);
	m_pEngine->GetSceneMgr()->Render(Ogre::CULL_VIEWFRUSTUM, m_pCamera);

	Ogre::Vector3 vVel(0.0f, 0.0f, 0.0f);
	Ogre::Vector3 vUp(0.0f, 1.0f, 0.0f);

	m_pEngine->GetAudioSys()->Set3DListenerAttributes(&m_pCameraCtrl->m_Viewpoint, &vVel, &dir, &vUp);
	m_pEngine->GetAudioSys()->Update();
	}
	else
	{
	m_GCEditor->DoFrame(lTick); 
	}*/

	ReleaseEngineDevice();


	Ogre::SceneManager::getSingleton().doFrame();
}

void CEnginAgent::SetBGColor(Ogre::ColorQuad color)
{
	m_backColor = color;
}

void CEnginAgent::SetSpeed(float fValue)
{
	m_fSpeed = fValue;
}

float CEnginAgent::GetSpeed()
{
	return m_fSpeed;
}

void CEnginAgent::SetFrameLimit(int iLimit)
{
	m_FrameLimit = iLimit;
}

//根据照相机的参数计算相机与地面的焦点
Ogre::Vector3 CEnginAgent::ComputeintersectionPos()
{
	Ogre::Vector3 result;

	float dis = 0.0f;
	Ogre::WorldRay ray;
	ray.m_Origin = m_pCamera->getPosition();
	ray.m_Dir = m_pCameraCtrl->ComputeDirection();

	bool bIsSucc = m_pRoom->pickGround(ray, &dis);
	if(!bIsSucc)
	{
		result.x = m_pCameraCtrl->m_Viewpoint.x;
		result.y = 10.0f;
		result.z = m_pCameraCtrl->m_Viewpoint.z + 500.0f;
	}
	else
		result = m_pCamera->getPosition().toVector3() + m_pCameraCtrl->ComputeDirection() * dis;

	return result;
}

void CEnginAgent::LoadGameMap( int MapID, Ogre::WorldPos& pos)
{/*
 if( m_pRoom )
 OGRE_RELEASE( m_pRoom );
 m_pRoom	= m_GCEditor->LoadGameMap( MapID, pos);
 m_pRoom->release();
 */
}

void CEnginAgent::SetBackNode(char* strpath)
{
	/*
	CString strPath(strpath);

	if(IS_KIND_OF(Ogre::SimpleRoom, m_pRoom))
	{
	//无背景
	if(strPath.IsEmpty() && m_pBackNode != NULL)
	{
	Ogre::Vector3 pos(0.0f, 0.0f, 0.0f);
	CEffectMgr::GetInstance()->SetModelActorPos(pos);
	m_pCameraCtrl->SetMainNodePos(pos);

	static_cast<Ogre::SimpleRoom*>(m_pRoom)->DetachNode(m_pBackNode);
	OGRE_RELEASE(m_pBackNode);
	}
	//街?尘?
	else if(strPath.Find("background1") != -1 && m_pBackNode == NULL)
	{
	Ogre::Vector3 pos(0.0f, 0.0f, 0.0f);
	CEffectMgr::GetInstance()->SetModelActorPos(pos);
	m_pCameraCtrl->SetMainNodePos(pos);

	m_pEngine->GetResLoader()->RemoveResourceFromCache(strpath);
	Ogre::SceneNode* pNode = Ogre::BuildNodeFromChunk(strpath);
	if(pNode != NULL)
	{
	static_cast<Ogre::SimpleRoom*>(m_pRoom)->AttachNode(pNode);
	m_pBackNode = pNode;
	}
	}
	//地图
	else if(strPath.Find("map.") != -1)
	{
	int nResId = 1002;
	if(strPath.Find("map.1002") != -1)
	nResId = 1002;
	else if(strPath.Find("map.1003") != -1)
	nResId = 1003;

	m_pCameraCtrl->ResetInScene(nResId);
	m_pCameraCtrl->UpdateCameraNode(m_pCamera);
	Ogre::WorldPos vAreaCenter((int)(m_pCameraCtrl->m_Viewpoint.x)*10, 20, (int)(m_pCameraCtrl->m_Viewpoint.z)*10);

	if(m_pBackNode != NULL)
	{
	static_cast<Ogre::SimpleRoom*>(m_pRoom)->DetachNode(m_pBackNode);
	OGRE_RELEASE(m_pBackNode);
	}
	static_cast<Ogre::SimpleRoom*>(m_pRoom)->DetachNode(m_pGround);
	CEffectMgr::GetInstance()->m_pModelActor->DetachFromRoom(m_pRoom);

	Ogre::GameScene* pRoom = m_GCEditor->LoadGameMap(nResId, vAreaCenter);
	pRoom->SetAmbientLight(0.5f, 0.5f, 0.5f);
	CEffectMgr::GetInstance()->m_pModelActor->AttachToRoom(pRoom);
	OGRE_RELEASE(m_pRoom);
	m_pRoom = pRoom;
	MAY_ADDREF(m_pRoom);

	//计算角色的位置
	Ogre::Vector3 pos;
	pos = ComputeintersectionPos(); 

	CEffectMgr::GetInstance()->SetModelActorPos(pos);
	m_pCameraCtrl->SetMainNodePos(pos);
	}
	}
	else if(IS_KIND_OF(Ogre::GameTerrainScene, m_pRoom))
	{
	if(strPath.IsEmpty() || strPath.Find("background1") != -1)
	{
	m_pCameraCtrl->Reset();
	Ogre::Vector3 pos(0.0f, 0.0f, 0.0f);
	CEffectMgr::GetInstance()->SetModelActorPos(pos);
	m_pCameraCtrl->SetMainNodePos(pos);

	CEffectMgr::GetInstance()->m_pModelActor->DetachFromRoom(m_pRoom);
	Ogre::SimpleRoom* pSimpleRoom = static_cast<Ogre::SimpleRoom*>(CEnginAgent::GetInstance()->m_GCEditor->UnloadGameMap());
	OGRE_RELEASE(m_pRoom);
	m_pRoom = pSimpleRoom;
	MAY_ADDREF(m_pRoom);
	m_pRoom->AttachNode(m_pGround);
	CEffectMgr::GetInstance()->m_pModelActor->AttachToRoom(m_pRoom);
	m_pRoom->SetAmbientLight(0.5f, 0.5f, 0.5f);

	if(!strPath.IsEmpty())
	{
	Ogre::SceneNode* pNode = Ogre::BuildNodeFromChunk(strpath);
	m_pRoom->AttachNode(pNode);
	m_pBackNode = pNode;
	}
	}
	else if(strPath.Find("map.") != -1)
	{
	int nResId = 1002;
	if(strPath.Find("map.1002") != -1)
	nResId = 1002;
	else if(strPath.Find("map.1003") != -1)
	nResId = 1003;

	m_pCameraCtrl->ResetInScene(nResId);
	m_pCameraCtrl->UpdateCameraNode(m_pCamera);
	Ogre::WorldPos vAreaCenter((int)(m_pCameraCtrl->m_Viewpoint.x)*10, 20, (int)(m_pCameraCtrl->m_Viewpoint.z)*10);
	CEffectMgr::GetInstance()->m_pModelActor->DetachFromRoom(m_pRoom);
	Ogre::GameTerrainScene* pBigRoom = static_cast<Ogre::GameTerrainScene*>(CEnginAgent::GetInstance()->m_GCEditor->LoadGameMap(nResId, vAreaCenter));

	OGRE_RELEASE(m_pRoom);
	m_pRoom = pBigRoom;
	MAY_ADDREF(m_pRoom);
	Ogre::Vector3 pos;
	pos = ComputeintersectionPos();
	CEffectMgr::GetInstance()->SetModelActorPos(pos);
	m_pCameraCtrl->SetMainNodePos(pos);

	pBigRoom->SetViewAreaCenter(Ogre::Vector3(vAreaCenter.x/10, vAreaCenter.y/10, vAreaCenter.z/10), Ogre::Vector3(0, 1, 0));
	pBigRoom->SetAmbientLight(0.5f, 0.5f, 0.5f);

	CEffectMgr::GetInstance()->m_pModelActor->AttachToRoom(m_pRoom);
	}
	}
	*/
}

void CEnginAgent::SetEffectPos(CPoint pt, int flag)
{
	if(IS_KIND_OF(Ogre::GameTerrainScene, m_pRoom) && m_bIsEditMode)
	{
		Ogre::WorldRay viewRay;
		m_pCamera->getViewRayByScreenPt(&viewRay, pt.x, pt.y);
		float dis = 0.0f;
		bool bSucc = static_cast<Ogre::GameTerrainScene*>(m_pRoom)->pickGround(viewRay, &dis);
		if(!bSucc)
			return;

		Ogre::Vector3 vPickPoint;
		Ogre::Vector3 cpos = m_pCamera->getPosition().toVector3();
		vPickPoint = viewRay.m_Origin.toVector3() + viewRay.m_Dir * dis;

		if(flag == 0)
		{

			CEffectMgr::GetInstance()->SetModelActorPos(vPickPoint);
			float fDis = m_pCameraCtrl->GetMainNodeToCameraDis();
			Ogre::Vector3 vDirection = m_pCameraCtrl->ComputeDirection();
			Ogre::Vector3 vCameraPos = vPickPoint - vDirection * fDis;

			m_pCameraCtrl->m_Viewpoint = vCameraPos;
			m_pCameraCtrl->SetMainNodePos(vPickPoint);
		}
		else if(flag == 1)
		{
			CEffectMgr::GetInstance()->SetModelTargetPos(vPickPoint);
		}
	}
}

void CEnginAgent::FortyFiveView()
{
	m_pCameraCtrl->SeeFortyFive();
	/*
	m_pCameraCtrl->UpdateCameraNode(m_pCamera);
	Ogre::Vector3 vPos;
	vPos = ComputeintersectionPos();

	CEffectMgr::GetInstance()->SetModelActorPos(vPos);
	m_pCameraCtrl->SetMainNodePos(vPos);
	*/
	float fDis = m_pCameraCtrl->GetMainNodeToCameraDis();
	Ogre::Vector3 vDirection;
	vDirection = m_pCameraCtrl->ComputeDirection();
	Ogre::Vector3 vMainNodePos = m_pCameraCtrl->GetMainNodePos();
	Ogre::Vector3 vNewViewPoint = vMainNodePos - vDirection * m_pCameraCtrl->GetMainNodeToCameraDis();
	m_pCameraCtrl->SetViewPoint(vNewViewPoint);
	m_pCameraCtrl->UpdateCameraNode(m_pCamera);
}

void CEnginAgent::SetEditModel(bool bMode)
{
	m_bIsEditMode = bMode;
	if(!bMode)
	{
		m_pCameraCtrl->ResetInScene();
		m_pCameraCtrl->UpdateCameraNode(m_pCamera);
		Ogre::Vector3 vPos ;
		vPos = ComputeintersectionPos();

		CEffectMgr::GetInstance()->SetModelActorPos(vPos);
		m_pCameraCtrl->SetMainNodePos(vPos);
	}
}

void CEnginAgent::AttachActor(Ogre::Model* pnode)
{
	if(m_pAttachedNode == NULL)
	{
		m_pAttachedNode = pnode;
		//m_pAttachedNode->AddRef();
		m_pAttachedNode->attachToScene(m_pRoom);
	}
}

void CEnginAgent::DetachActor(Ogre::Model* pnode)
{
	if(m_pAttachedNode != NULL )
	{
		m_pAttachedNode->detachFromScene();
		//OGRE_RELEASE(m_pAttachedNode);
		m_pAttachedNode = NULL;
	}
}

bool CEnginAgent::IsKeyDown(int key)
{
	return (GetAsyncKeyState(key)&0xff00)!=0; 
}

void CEnginAgent::ShowGround(bool bShow)
{
	if(bShow == m_bShowGround)
		return;

	m_bShowGround = bShow;
	if(m_bShowGround)
	{
		m_pGround->attachToScene(m_pRoom);
	}
	else
	{
		m_pGround->detachFromScene();
	}
}

bool CEnginAgent::IsShowGround()
{
	return m_bShowGround;
}

void CEnginAgent::SetPostEffect(std::vector<Ogre::PostproPhaseType>& effectlist)
{
	/*
	m_pPERenderer->ClearPostEffect();
	for(int i = 0; i < (int) effectlist.size(); i++)
	{
	m_pPERenderer->AddPostEffect(effectlist[i]);
	}*/
}


void CEnginAgent::ClearPostEffect()
{
	//	m_pPERenderer->ClearPostEffect();
}

void CEnginAgent::EnableGlow(bool bEnable)
{
	m_bEnableGlow = bEnable;
	//m_pEngine->GetSceneMgr()->SetGlow(m_bEnableGlow);
}

bool CEnginAgent::IsGlowEnable()
{
	return m_bEnableGlow;
}


CEnginAgent* CEnginAgent::ms_pInst = NULL;
