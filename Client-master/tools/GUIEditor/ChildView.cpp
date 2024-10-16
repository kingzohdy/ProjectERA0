// ChildView.cpp : implementation file
//

#include "stdafx.h"
#include "ChildView.h"
#include "EnginAgent.h"
#include "OgreRenderSystem.h"
#include "OgreRenderWindow.h"

// CChildView

IMPLEMENT_DYNAMIC(CChildView, CWnd)

CChildView::CChildView()
{

}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_SIZE()
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

END_MESSAGE_MAP()

void CChildView::OnInit()
{
	// TODO:  Add your specialized creation code here
	CEnginAgent::CreateInstance();
	CEnginAgent* pEngineAgent = CEnginAgent::GetInstance();
	pEngineAgent->Initialize(m_hWnd);
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
}

void RenderWindowOnSizeMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(Ogre::RenderSystem::getSingletonPtr() == NULL) return;

	Ogre::RenderWindow *prwin = Ogre::RenderSystem::getSingleton().findRenderWindow(hWnd);
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
	
	CEnginAgent* pEnginAgent = CEnginAgent::GetInstance();
	if( pEnginAgent != NULL && pEnginAgent->m_bIsInitialized ) 
	{
		 pEnginAgent->OnWindowMsg(message, wParam, lParam);
	}
	
	if( message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN )
	{
		this->SetFocus();
	}

	if( message == WM_MOUSEWHEEL )
	{
		return 0;
	}
	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

BOOL CChildView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
// CChildView message handlers


