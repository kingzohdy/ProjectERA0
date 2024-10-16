// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "ChildView.h"
#include "EditorManager.h"
#include "DBManager.h"
#include "EffectManager.h"
#include "DigitHelpManager.h"
#include "CameraCtrl.h"
#include "MainFrm.h"
#include "ObjectEditor.h"

#include "OgreRenderSystem.h"
#include "OgreRenderWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Ogre;

// CChildView

CChildView::CChildView() : m_bIsTrans(false), m_bIsRoll(false), m_uiState(UI_IDLE)
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_MESSAGE(EEM_DROPRES, OnDropRes)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void RenderWindowOnSizeMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(Ogre::RenderSystem::getSingletonPtr() == NULL) return;
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

BOOL CChildView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	RenderWindowOnSizeMove(GetSafeHwnd(), message, wParam, lParam);

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

void CChildView::OnInit()
{
	// TODO:  Add your specialized creation code here
	CEnginAgent* pEngineAgent = CEnginAgent::GetInstance();
	pEngineAgent->Initialize(m_hWnd);	//m_wndView.GetSafeHwnd());
	pEngineAgent->CreateScene();		// 网格
	CRect clientRect;
	GetClientRect(clientRect);
	pEngineAgent->SetRatio(clientRect.Width() / (float)clientRect.Height());

	CDataBase::CreateInstance();
	CDataBase::GetInstance()->Initialize();

	CEffectMgr::CreateInstance();
	CEffectMgr::GetInstance()->Initialze();
	CEffectMgr::GetInstance()->CreateDefaultModel();
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDigitHelpMgr::CreateInstance();

	m_dropTarget.Register(this);
	m_dropTarget.SetNotifyTarget(GetSafeHwnd());
	return 0;
}

CPoint g_FixedPos;
void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	RECT rt;
	GetClientRect(&rt);
	int x = point.x;
	int y = rt.bottom - rt.top - point.y;
	int width = rt.right - rt.left;
	int height = rt.bottom - rt.top;
	float fx = ((float)x - (float)width / 2.0f) / (float)width * 2.0f;
	float fy = ((float)y - (float)height / 2) / (float)height * 2;

	if(CEnginAgent::GetInstance() == NULL)
		return;

	CCameraCtrl* pCameraCtrl = CEnginAgent::GetInstance()->GetCameraCtrl();

	if((GetAsyncKeyState(VK_MENU )&0xff00)!=0) 
	{
		RECT rect;
		GetClientRect(&rect);
		int cx = (rect.left + rect.right)/2;
		int cy = (rect.top + rect.bottom)/2;
		int w = (rect.right - rect.left)/2;
		int h = (rect.bottom - rect.top)/2;

		POINT tmp;
		GetCursorPos(&tmp);

		float x = 180.0f*float(tmp.x - g_FixedPos.x)/w;
		float y = 180.0f*float(tmp.y - g_FixedPos.y)/h;
		bool bResetMouse = false;

		if((nFlags & MK_LBUTTON) && m_uiState == UI_ROLL)
		{
			ShowMouse(FALSE);
			pCameraCtrl->MouseRollTrans(x, y);
			bResetMouse = true;
		}
		else if((nFlags & MK_MBUTTON) && m_uiState == UI_SHIFT)
		{
			ShowMouse(FALSE);
			pCameraCtrl->MouseShiftTrans(x, y);
			bResetMouse = true;
		}
		else if((nFlags & MK_RBUTTON) && m_uiState == UI_SCALE)
		{
			ShowMouse(FALSE);
			pCameraCtrl->ScaleTrans((int)(20 * (x + y)));
			bResetMouse = true;
		}
		else
		{
			m_uiState = UI_IDLE;
			ShowMouse(TRUE);
		}

		if((x !=0 || y !=0) && bResetMouse)
		{
			::SetCursorPos(g_FixedPos.x, g_FixedPos.y);
		}
	}
	else if((nFlags&MK_MBUTTON) != 0 && m_uiState == UI_DIGIT)
	{
		POINT pt;
		pt = point;
		ClientToScreen(&pt);
		int step = pt.y - g_FixedPos.y;
		//if(step < 0) //鼠标向上移动
		//	CDigitHelpMgr::GetInstance()->IncreastTarget(-step);
		//else if(step > 0)
		//	CDigitHelpMgr::GetInstance()->DecreaseTarget(step);
		TRACE("Digiting %d\n", step);
		if(g_FixedPos != pt)
			::SetCursorPos(g_FixedPos.x, g_FixedPos.y);

	}
	else 
	{
		ShowMouse(TRUE);
		m_uiState = UI_IDLE;
		if((nFlags & MK_RBUTTON) )
		{
			RECT rect;
			GetClientRect(&rect);
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
				ClientToScreen(&pt);
				::SetCursorPos(pt.x, pt.y);
			}
		}
		if(CEffectMgr::GetInstance()->OnMouseMove(fx, fy))
		{
			CWnd::OnMouseMove(nFlags, point);
			return ;
		}

	}

	CWnd::OnMouseMove(nFlags, point);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	RECT rt;
	GetClientRect(&rt);

	int x = point.x;
	int y = rt.bottom - rt.top - point.y;
	int width = rt.right - rt.left;
	int height = rt.bottom - rt.top;
	float fx = ((float)x - (float)width / 2.0f) / (float)width * 2.0f;
	float fy = ((float)y - (float)height / 2) / (float)height * 2;

	if((GetAsyncKeyState(VK_MENU )&0xff00)!=0 || !CEffectMgr::GetInstance()->OnLBtnDown(fx, fy))
	{
		if(m_bIsTrans != true)
		{
			m_bIsRoll = true;
			RECT rect;
			GetClientRect(&rect);
			POINT tmp;
			GetCursorPos(&tmp);
			g_FixedPos = tmp;

			int cx = (rect.left + rect.right)/2;
			int cy = (rect.top + rect.bottom)/2;
			POINT pt;
			pt.x = cx;
			pt.y = cy;
			ClientToScreen(&pt);
			m_bIsTrans = true;

			if((GetAsyncKeyState(VK_MENU )&0xff00)!=0) 
				m_uiState = UI_ROLL;
		}
		//ShowMouse(FALSE);

	}

	//设置角色位置
	if((GetAsyncKeyState(VK_CONTROL)&0xff00) != 0)
	{
		CEnginAgent::GetInstance()->SetEffectPos(point, 0); //Actor
	}
	else if((GetAsyncKeyState(VK_SHIFT)&0xff00) != 0)
	{
		CEnginAgent::GetInstance()->SetEffectPos(point, 1); //Target
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	RECT rt;
	GetClientRect(&rt);
	int x = point.x;
	int y = rt.bottom - rt.top - point.y;
	int width = rt.right - rt.left;
	int height = rt.bottom - rt.top;
	float fx = ((float)x - (float)width / 2.0f) / (float)width * 2.0f;
	float fy = ((float)y - (float)height / 2) / (float)height * 2;

	if(!CEffectMgr::GetInstance()->OnLBtnUp((float)x, (float)y))
	{
		m_bIsRoll = false;
		m_bIsTrans = false;
	}
	else
	{
		ShowMouse(TRUE);
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	CCameraCtrl* pCameraCtrl = CEnginAgent::GetInstance()->GetCameraCtrl();

	if(m_bIsRoll != true)
	{
		m_bIsTrans = true;
		pCameraCtrl->SetKeyTrans(true);
		RECT rect;
		GetClientRect(&rect);
		int cx = (rect.left + rect.right)/2;
		int cy = (rect.top + rect.bottom)/2;
		POINT pt;
		pt.x = cx;
		pt.y = cy;
		ClientToScreen(&pt);
		::SetCursorPos(pt.x, pt.y);
		::SetCursor(theApp.LoadStandardCursor(IDC_CROSS));

	}
	//ShowMouse(FALSE);
	POINT tmp;
	GetCursorPos(&tmp);
	g_FixedPos = tmp;
	if((GetAsyncKeyState(VK_MENU )&0xff00)!=0) 
		m_uiState = UI_SCALE;

	CWnd::OnRButtonDown(nFlags, point);
}

void CChildView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_bIsTrans = false;
	m_bIsRoll = false;
	CCameraCtrl* pCameraCtrl = CEnginAgent::GetInstance()->GetCameraCtrl();
	pCameraCtrl->SetKeyTrans(false);
	SetCursor(theApp.LoadStandardCursor(IDC_ARROW));
	ShowMouse(TRUE);
	CWnd::OnRButtonUp(nFlags, point);
}

void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	RECT rt;
	GetClientRect(&rt);
	GetClientRect(&rt);
	int x = point.x;
	int y = rt.bottom - rt.top - point.y;
	int width = rt.right - rt.left;
	int height = rt.bottom - rt.top;
	float fx = ((float)x - (float)width / 2.0f) / (float)width * 2.0f;
	float fy = ((float)y - (float)height / 2) / (float)height * 2;

	CEffectMgr::GetInstance()->OnDblClick(fx, fy);

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if(cy > 10 && cx > 10 && CEnginAgent::GetInstance() != NULL)
		CEnginAgent::GetInstance()->SetRatio(cx / (float)cy);
	if(CEnginAgent::GetInstance() != NULL)
		CEnginAgent::GetInstance()->OnResetDev();
}

BOOL CChildView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	CCameraCtrl* pCameraCtrl = CEnginAgent::GetInstance()->GetCameraCtrl();
	pCameraCtrl->ScaleTrans(zDelta * 5);

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CChildView::OnMButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	//ShowMouse(FALSE);
	// TODO: Add your message handler code here and/or call default
	POINT tmp;
	GetCursorPos(&tmp);
	g_FixedPos = tmp;
	if((GetAsyncKeyState(VK_MENU )&0xff00)!=0) 
		m_uiState = UI_SHIFT;
	else
		m_uiState = UI_DIGIT;
	//CWnd::OnMButtonDown(nFlags, point);


}

void CChildView::OnMButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	// TODO: Add your message handler code here and/or call default
	ShowMouse(TRUE);
	CWnd::OnMButtonUp(nFlags, point);
}

void CChildView::ShowMouse(BOOL bShow)
{
	if(bShow)
	{
		while(ShowCursor(TRUE) < 0);
	}
	else
	{
		while (ShowCursor(FALSE) >= 0);
	}
}

LRESULT CChildView::OnDropRes(WPARAM wParam, LPARAM lParam)
{
	CResDropTarget::DROPNOTIFY* pdn = (CResDropTarget::DROPNOTIFY*)lParam;
	char* tmp = new char[256];
	strcpy(tmp, pdn->strChunk);
	PostMainFrameMsg(EEM_REQ_LOAD_ACTOR, 0, (LPARAM)tmp);
	return 0;
}

BOOL CChildView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{

	return DROPEFFECT_COPY;
}

DROPEFFECT CChildView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	// TODO: Add your specialized code here and/or call the base class

	return DROPEFFECT_COPY; //CWnd::OnDragOver(pDataObject, dwKeyState, point);
}

DROPEFFECT CChildView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	// TODO: Add your specialized code here and/or call the base class

	return DROPEFFECT_COPY; //CWnd::OnDragEnter(pDataObject, dwKeyState, point);
}
void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if(nChar == 'W')
		static_cast<CMainFrame*>(theApp.m_pMainWnd)->OnBtnCoodPos();
	else if(nChar == 'E')
		static_cast<CMainFrame*>(theApp.m_pMainWnd)->OnBtnCoodRot();
	else if(nChar == 'R')
		static_cast<CMainFrame*>(theApp.m_pMainWnd)->OnBtnCoodScale();

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}