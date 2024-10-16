
#include "stdafx.h"
#include "RenderStaticView.h"
#include "EditorManager.h"

#include "OgreRenderWindow.h"
#include "OgreSceneManager.h"
#include "OgreLight.h"
#include "OgreCamera.h"
#include "OgreGameScene.h"
#include "OgreRenderSystem.h"
#include "OgreModel.h"

#include <Windows.h>
#include <MMSystem.h>

using namespace Ogre;

void CRenderStaticHandler::Start()
{

}

void CRenderStaticHandler::Shut()
{
}

void CRenderStaticHandler::OnMouseMove(UINT nFlags, CPoint point, HWND hViewport)
{
	if(!CEnginAgent::GetInstance())
		return ;
	CCameraCtrl* pCameraCtrl = &m_pRenderStatic->m_CameraCtrl;
	if(nFlags & MK_RBUTTON)
	{
		RECT rect;
		GetClientRect(hViewport, &rect);
		int cx = (rect.left + rect.right)/2;
		int cy = (rect.top + rect.bottom)/2;
		int w = (rect.right - rect.left)/2;
		int h = (rect.bottom - rect.top)/2;

		float x = 180.0f*float(point.x - cx)/w;
		float y = 180.0f*float(point.y - cy)/h;

		pCameraCtrl->MouseMoveTrans(x, y);

		if(x !=0 || y !=0)
		{
			POINT pt;
			pt.x = cx;
			pt.y = cy;

			::ClientToScreen(hViewport, &pt);
			::SetCursorPos(pt.x, pt.y);
		}
	}
	else if(nFlags & MK_LBUTTON)
	{
		RECT rect;
		GetClientRect(hViewport, &rect);
		int cx = (rect.left + rect.right)/2;
		int cy = (rect.top + rect.bottom)/2;
		int w = (rect.right - rect.left)/2;
		int h = (rect.bottom - rect.top)/2;

		POINT tmp;
		GetCursorPos( &tmp);

		float x = 180.0f*float(tmp.x - m_FixedPos.x)/w;
		float y = 180.0f*float(tmp.y - m_FixedPos.y)/h;

		if((GetAsyncKeyState(VK_CONTROL  )&0xff00)!=0)
		{
			pCameraCtrl->MouseRollTrans(x, y);
		}
		if((GetAsyncKeyState(VK_SHIFT  )&0xff00)!=0)
		{
			pCameraCtrl->ScaleTrans(y);
		}
		else
		{
			pCameraCtrl->MouseShiftTrans(x, y);
		}

		if(x !=0 || y !=0)
		{
			POINT pt;
			pt.x = cx;
			pt.y = cy;
			::ClientToScreen(hViewport, &pt);
			::SetCursorPos(m_FixedPos.x, m_FixedPos.y);
		}
	}
}

void CRenderStaticHandler::OnLButtonDown(UINT nFlags, CPoint point, HWND hViewport)
{
	POINT tmp;
	GetCursorPos(&tmp);
	m_FixedPos = tmp;


	//if(pNode)
	//{
	//	CMoveActorHandler* pHandler = new CMoveActorHandler(pNode);
	//	CActionInst::GetInstance()->m_input.SetCurrHandler(pHandler);
	//}
}

void CRenderStaticHandler::OnRButtonDown(UINT nFlags, CPoint point, HWND hViewport)
{
	m_pRenderStatic->m_CameraCtrl.SetKeyTrans(true);
	RECT rect;
	GetClientRect(hViewport, &rect);
	int cx = (rect.left + rect.right)/2;
	int cy = (rect.top + rect.bottom)/2;
	POINT pt;
	pt.x = cx;
	pt.y = cy;
	ClientToScreen(hViewport, &pt);
	::SetCursorPos(pt.x, pt.y);
}

void CRenderStaticHandler::OnLButtonUp(UINT nFlags, CPoint point, HWND hViewprot)
{
}

void CRenderStaticHandler::OnRButtonUp(UINT nFlags, CPoint point, HWND hViewprot)
{
	m_pRenderStatic->m_CameraCtrl.SetKeyTrans(false);
}




// CRenderStatic

IMPLEMENT_DYNAMIC(CRenderStatic, CWnd)
CRenderStatic::CRenderStatic()
{
	m_pCamera = NULL;
	m_pPreviewRoom = NULL;
	m_pPreviewObject = NULL;
	m_pSceneRenderer = NULL;
	m_pRenderTarget = NULL;
	m_fRatio = 1.0f;
	m_pCurrHandler = NULL;
	m_bSelfDraw = false;
	m_bAutoCamera = true;
	m_pCallBackHandler = NULL;
	m_bDraw = true;
	m_bDrawOnce = false;
}

CRenderStatic::~CRenderStatic()
{
	OGRE_RELEASE(m_pPreviewObject);
	OGRE_RELEASE(m_pPreviewRoom);
}


BEGIN_MESSAGE_MAP(CRenderStatic, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()



// CRenderStatic message handlers


void CRenderStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (m_bSelfDraw)
	{
		long lTick = ::GetTickCount();
		CEnginAgent::GetInstance()->Update(lTick);
		DoFrame();
		Invalidate(FALSE);

	}
}

BOOL CRenderStatic::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{

	return CWnd::Create(AfxRegisterWndClass(0), NULL, WS_CHILD, rect, pParentWnd, nID);
}

int CRenderStatic::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	Init();
	return 0;
}
void CRenderStatic::DoJobFrame(int dtick)
{
	DoFrame(false);
}
void CRenderStatic::DoFrame(bool bDebugMsgBox)
{
	static long lLastTime = 0;
	long lCurr = ::GetTickCount();

	long dtime = lCurr - lLastTime;


	Update(dtime);
	//if(bDebugMsgBox == true)
	//MessageBox("Update(dtime);成功");

	Draw(bDebugMsgBox);
	//if(bDebugMsgBox == true)
	//MessageBox("Draw();成功");
	lLastTime = lCurr;
}

void CRenderStatic::Update(Ogre::uint ldtick)
{
	if(m_pCamera)
	{
		if (m_bAutoCamera) 
		{
			m_CameraCtrl.MouseRollTrans(1.7f, 0);
		}
		if(m_pCallBackHandler)
		{
			m_pCallBackHandler->OnCallbackUpdate(ldtick / 1000.0f);
		}

		m_CameraCtrl.Update((float)ldtick / 1000.0f);

		Ogre::Vector3 dir;
		dir.y = Ogre::Sin(m_CameraCtrl.m_ViewYaw);
		dir.x = Ogre::Sin(m_CameraCtrl.m_ViewPitch)*Ogre::Cos(m_CameraCtrl.m_ViewYaw );
		dir.z = Ogre::Cos(m_CameraCtrl.m_ViewPitch)*Ogre::Cos(m_CameraCtrl.m_ViewYaw);

		m_pCamera->setLookDirect(m_CameraCtrl.m_Viewpoint, dir, Ogre::Vector3(0, 1.0f, 0));
		m_pCamera->setFov(60.0f);
		m_pCamera->setDepth(1.0f, 2000.0f);
		m_pCamera->setRatio(m_fRatio);

		m_pCamera->update(ldtick);
	}
	if(m_pPreviewRoom != NULL)
	{
		m_pPreviewRoom->update(ldtick);
	}

	if(m_pPreviewObject)
	{
		m_pPreviewObject->update(ldtick);
	}
}


void CRenderStatic::Draw(bool bDebugMsgBox)
{
	if(m_bDraw == false)
		return;

	if(m_bDrawOnce == true)
		m_bDraw = false;

	static int iFrameCount;

	if(m_pPreviewRoom && m_pRenderTarget && m_pSceneRenderer)
	{
		if(m_pCallBackHandler)
			m_pCallBackHandler->OnCallbackDraw();
	}
}

void CRenderStatic::Init()
{
	m_defHandler.m_pRenderStatic = this;
	SetInputHandler(NULL);
	CRect rect;
	GetClientRect(rect);
	m_fRatio = (float)rect.Width() / (float)rect.Height();
}

void CRenderStatic::StartRender()
{
	m_pCamera = new Ogre::Camera();
	m_pPreviewRoom = new Ogre::SimpleGameScene();

	Light *plight = new Light(LT_POINT);
	plight->setPosition(Vector3(0, 200.0f, -100.0f));
	plight->setColor(ColourValue(0.8f, 0.8f, 0.8f));
	plight->enableAmbient();
	plight->m_AmbientColor = ColourValue(0.2f, 0.2f, 0.2f);
	plight->m_DoSpecular = true;
	plight->m_SpecularColor = ColourValue(1.0f, 1.0f, 1.0f);
	plight->m_SpecularPower = 4.0f;
	plight->setGlobal(true);
	plight->update(0);
	plight->attachToScene(m_pPreviewRoom);
	plight->release();

	RenderSystem::InitDesc desc;
	memset(&desc, 0, sizeof(desc));
	desc.hwnd = GetSafeHwnd();
	desc.depthbits = 16;
	desc.stencilbits = 8;
	desc.colorbits = 32;

	CRect rect;
	GetClientRect(rect);
	desc.width = rect.Width();
	desc.height = rect.Height();

	m_pRenderTarget = RenderSystem::getSingleton().createRenderWindow(desc);

	m_pSceneRenderer = new NormalSceneRenderer();
	m_pSceneRenderer->setClearParams(Ogre::CLEAR_TARGET|Ogre::CLEAR_ZBUFFER, 0xff000000, 1.0f, 0);
	m_pSceneRenderer->setCamera(m_pCamera);
	m_pSceneRenderer->setRenderTarget(m_pRenderTarget);
	m_pSceneRenderer->setRenderScene(m_pPreviewRoom);

	SceneManager::getSingleton().addSceneRenderer(3, m_pSceneRenderer);
}

void CRenderStatic::SetSelfDraw(bool bSelfDraw)
{
	m_bSelfDraw = bSelfDraw;
	Invalidate(FALSE);
}

void CRenderStatic::SetInputHandler(CInputHandler* pHandler)
{
	if(m_pCurrHandler)
		m_pCurrHandler->Shut();

	m_pCurrHandler = pHandler;
	if(!m_pCurrHandler)
	{
		m_pCurrHandler = &m_defHandler;
	}
	m_pCurrHandler->Start();

}

void CRenderStatic::SetModel(CString path)
{
	Ogre::MovableObject *pobj = Ogre::createObjectFromResource(path.GetBuffer());
	SetModel(pobj);
	OGRE_RELEASE(pobj);
	m_strPath = path;
}

void CRenderStatic::SetModel(Ogre::MovableObject *pobj)
{
	m_strPath = "";
	if(m_pPreviewObject)
	{
		m_pPreviewObject->detachFromScene();
		OGRE_RELEASE(m_pPreviewObject);
	}

	m_pPreviewObject = pobj;
	if(m_pPreviewObject)
	{
		m_pPreviewObject->addRef();
		m_pPreviewObject->attachToScene(m_pPreviewRoom);
		if(IS_KIND_OF(Ogre::Model, m_pPreviewObject))
		{
			static_cast<Ogre::Model *>(m_pPreviewObject)->playAnim(0);
		}
	}
}

void CRenderStatic::ShutRender()
{
	if(m_pPreviewRoom)
	{
		if(m_pPreviewObject)
			m_pPreviewObject->detachFromScene( );
	}


	SceneManager::getSingleton().addSceneRenderer(3, NULL);
	if (m_pSceneRenderer)

		delete (m_pSceneRenderer);
	OGRE_RELEASE(m_pRenderTarget);
	OGRE_RELEASE(m_pPreviewObject);
	OGRE_RELEASE(m_pPreviewRoom);
	OGRE_RELEASE(m_pCamera);
}

void CRenderStatic::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	m_fRatio = (float)cx / (float)cy;
}

void CRenderStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(rect);
	if(rect.PtInRect(point) == TRUE)
	{
		SetCapture();
		if(m_pCurrHandler)
			m_pCurrHandler->OnMouseMove(nFlags, point, GetSafeHwnd());
	}
	else
	{
		ReleaseCapture();
	}
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CRenderStatic::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	m_CameraCtrl.ScaleTrans(zDelta);
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CRenderStatic::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_CameraCtrl.Reset();
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CRenderStatic::OnDestroy()
{
	CWnd::OnDestroy();

	SetInputHandler(NULL);
}

void CRenderStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_pCurrHandler->OnLButtonDown(nFlags, point, GetSafeHwnd());

	CWnd::OnLButtonDown(nFlags, point);
}

void CRenderStatic::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_pCurrHandler->OnLButtonUp(nFlags, point, GetSafeHwnd());

	CWnd::OnLButtonUp(nFlags, point);
}

void CRenderStatic::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_pCurrHandler->OnRButtonDown(nFlags, point, GetSafeHwnd());

	CWnd::OnRButtonDown(nFlags, point);
}

void CRenderStatic::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_pCurrHandler->OnRButtonUp(nFlags, point, GetSafeHwnd());
	CWnd::OnRButtonUp(nFlags, point);
}
