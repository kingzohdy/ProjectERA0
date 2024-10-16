//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2000 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************

// BCGPFrameWnd.cpp : implementation file
//

#include "stdafx.h"
#include "afxpriv.h"
#include "BCGPFrameWnd.h"
#include "BCGPMenuBar.h"
#include "BCGPPopupMenu.h"
#include "BCGPMiniFrameWnd.h"
#include "BCGPUserToolsManager.h"
#include "BCGPPrintPreviewView.h"

#ifdef _BCGP_USE_OLEIMPL_
#include "oleimpl2.h"
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPFrameWnd

IMPLEMENT_DYNCREATE(CBCGPFrameWnd, CFrameWnd)

#pragma warning (disable : 4355)

CBCGPFrameWnd::CBCGPFrameWnd() :
	m_Impl (this),
	m_bContextHelp (FALSE),
	m_bWasMaximized (FALSE),
	m_bIsMinimized (FALSE)
{
}

#pragma warning (default : 4355)

CBCGPFrameWnd::~CBCGPFrameWnd()
{
}

BEGIN_MESSAGE_MAP(CBCGPFrameWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CBCGPFrameWnd)
	ON_WM_MENUCHAR()
	ON_WM_ACTIVATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
	ON_WM_ACTIVATEAPP()
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_REGISTERED_MESSAGE(BCGM_CREATETOOLBAR, OnToolbarCreateNew)
	ON_REGISTERED_MESSAGE(BCGM_DELETETOOLBAR, OnToolbarDelete)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_STATUS_BAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_TOOLBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REBAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_REBAR, OnBarCheck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPFrameWnd message handlers

LRESULT CBCGPFrameWnd::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) 
{
	if (m_Impl.OnMenuChar (nChar))
	{
		return MAKELPARAM (MNC_EXECUTE, -1);
	}
		
	return CFrameWnd::OnMenuChar(nChar, nFlags, pMenu);
}
//*******************************************************************************************
afx_msg LRESULT CBCGPFrameWnd::OnSetMenu (WPARAM wp, LPARAM lp)
{
	OnSetMenu ((HMENU) wp);
	return DefWindowProc (WM_MDISETMENU, NULL, lp);
}
//*******************************************************************************************
BOOL CBCGPFrameWnd::OnSetMenu (HMENU hmenu)
{
	if (m_Impl.m_pMenuBar != NULL)
	{
		m_Impl.m_pMenuBar->CreateFromMenu 
			(hmenu == NULL ? m_Impl.m_hDefaultMenu : hmenu);
		return TRUE;
	}

	return FALSE;
}
//*******************************************************************************************
BOOL CBCGPFrameWnd::PreTranslateMessage(MSG* pMsg) 
{
	BOOL bProcessAccel = TRUE;

	switch (pMsg->message)
	{
	case WM_SYSKEYDOWN:
	case WM_CONTEXTMENU:
		if (CBCGPPopupMenu::m_pActivePopupMenu != NULL &&
			::IsWindow (CBCGPPopupMenu::m_pActivePopupMenu->m_hWnd) &&
			pMsg->wParam == VK_MENU)
		{
			CBCGPPopupMenu::m_pActivePopupMenu->SendMessage (WM_CLOSE);
			return TRUE;
		}
		else if (m_Impl.ProcessKeyboard ((int) pMsg->wParam))
		{
			return TRUE;
		}
		break;

	case WM_SYSKEYUP:
		if (m_Impl.m_pMenuBar != NULL && (VK_MENU == pMsg->wParam))
		{
			if (m_Impl.m_pMenuBar == GetFocus ())
			{
				SetFocus ();
			}
			else
			{
				m_Impl.m_pMenuBar->SetFocus ();
			}
			return TRUE;
		}
		if (CBCGPPopupMenu::m_pActivePopupMenu != NULL &&
			::IsWindow (CBCGPPopupMenu::m_pActivePopupMenu->m_hWnd))
		{
			return TRUE;	// To prevent system menu opening
		}
		break;

	case WM_KEYDOWN:
		//-----------------------------------------
		// Pass keyboard action to the active menu:
		//-----------------------------------------
		if (!CBCGPFrameImpl::IsHelpKey (pMsg) && 
			m_Impl.ProcessKeyboard ((int) pMsg->wParam, &bProcessAccel))
		{
			return TRUE;
		}

		if (pMsg->wParam == VK_ESCAPE)
		{
			if (IsFullScreen())
			{
				m_Impl.m_FullScreenMgr.RestoreState (this);
			}
		}

		if (!bProcessAccel)
		{
			return FALSE;
		}
		break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
		{
			CPoint pt (BCG_GET_X_LPARAM(pMsg->lParam), BCG_GET_Y_LPARAM(pMsg->lParam));
			CWnd* pWnd = CWnd::FromHandle (pMsg->hwnd);

			if (pWnd != NULL)
			{
				pWnd->ClientToScreen (&pt);
			}

			if (m_Impl.ProcessMouseClick (pMsg->message, pt, pMsg->hwnd))
			{
				return TRUE;
			}

			if (!::IsWindow (pMsg->hwnd))
			{
				return TRUE;
			}

			if (pMsg->message == WM_RBUTTONUP &&
				!CBCGPToolBar::IsCustomizeMode ())
			{
				//---------------------------------------
				// Activate the control bar context menu:
				//---------------------------------------
				CDockBar* pBar = DYNAMIC_DOWNCAST(CDockBar, pWnd);
				if (pBar != NULL)
				{
					CPoint pt;

					pt.x = BCG_GET_X_LPARAM(pMsg->lParam);
					pt.y = BCG_GET_Y_LPARAM(pMsg->lParam);
					pBar->ClientToScreen(&pt);

					SendMessage (BCGM_TOOLBARMENU,
								(WPARAM) GetSafeHwnd (),
								MAKELPARAM (pt.x, pt.y));
				}
			}
		}
		break;

	case WM_NCLBUTTONDOWN:
	case WM_NCLBUTTONUP:
	case WM_NCRBUTTONDOWN:
	case WM_NCRBUTTONUP:
	case WM_NCMBUTTONDOWN:
	case WM_NCMBUTTONUP:
		if (m_Impl.ProcessMouseClick (pMsg->message,
			CPoint (BCG_GET_X_LPARAM(pMsg->lParam), BCG_GET_Y_LPARAM(pMsg->lParam)),
			pMsg->hwnd))
		{
			return TRUE;
		}
		break;

	case WM_MOUSEMOVE:
		{
			CPoint pt (BCG_GET_X_LPARAM(pMsg->lParam), BCG_GET_Y_LPARAM(pMsg->lParam));
			CWnd* pWnd = CWnd::FromHandle (pMsg->hwnd);

			if (pWnd != NULL)
			{
				pWnd->ClientToScreen (&pt);
			}

			if (m_Impl.ProcessMouseMove (pt))
			{
				return TRUE;
			}
		}
	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}
//*******************************************************************************************
BOOL CBCGPFrameWnd::ShowPopupMenu (CBCGPPopupMenu* pMenuPopup)
{
	if (!m_Impl.OnShowPopupMenu (pMenuPopup, this))
	{
		return FALSE;
	}

	if (pMenuPopup != NULL && pMenuPopup->m_bShown)
	{
		return TRUE;
	}

	return OnShowPopupMenu (pMenuPopup);
}
//*******************************************************************************************
void CBCGPFrameWnd::OnClosePopupMenu (CBCGPPopupMenu* pMenuPopup)
{
	if (CBCGPPopupMenu::m_pActivePopupMenu == pMenuPopup)
	{
		CBCGPPopupMenu::m_pActivePopupMenu = NULL;
	}
}
//*******************************************************************************************
BOOL CBCGPFrameWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (HIWORD (wParam) == 1)
	{
		UINT uiCmd = LOWORD (wParam);

		CBCGPToolBar::AddCommandUsage (uiCmd);

		//---------------------------
		// Simmulate ESC keystroke...
		//---------------------------
		if (m_Impl.ProcessKeyboard (VK_ESCAPE))
		{
			return TRUE;
		}

		if (g_pUserToolsManager != NULL &&
			g_pUserToolsManager->InvokeTool (uiCmd))
		{
			return TRUE;
		}
	}

	if (!CBCGPToolBar::IsCustomizeMode ())
	{
		return CFrameWnd::OnCommand(wParam, lParam);
	}

	return FALSE;
}
//******************************************************************
BOOL CBCGPFrameWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	m_Impl.m_nIDDefaultResource = nIDResource;
	m_Impl.LoadLargeIconsState ();

	if (!CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	m_Impl.OnLoadFrame ();
	return TRUE;
}
//***************************************************************************************
void CBCGPFrameWnd::OnClose() 
{
	m_Impl.OnCloseFrame();	
	CFrameWnd::OnClose();
}
//***************************************************************************************
BOOL CBCGPFrameWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	m_dockManager.Create (this);
	m_Impl.SetDockManager (&m_dockManager);

	m_Impl.RestorePosition(cs);
	return CFrameWnd::PreCreateWindow(cs);
}
//***************************************************************************************
void CBCGPFrameWnd::WinHelp(DWORD dwData, UINT nCmd) 
{
	if (dwData > 0 || !m_bContextHelp)
	{
		CFrameWnd::WinHelp(dwData, nCmd);
	}
	else
	{
		OnContextHelp ();
	}
}
//***************************************************************************************
void CBCGPFrameWnd::OnContextHelp ()
{
	m_bContextHelp = TRUE;

	if (!m_bHelpMode && CanEnterHelpMode())
	{
		CBCGPToolBar::SetHelpMode ();
	}

	CFrameWnd::OnContextHelp ();

	if (!m_bHelpMode)
	{
		CBCGPToolBar::SetHelpMode (FALSE);
	}

	m_bContextHelp = FALSE;
}
//*******************************************************************************************
LRESULT CBCGPFrameWnd::OnToolbarCreateNew(WPARAM,LPARAM lp)
{
	ASSERT (lp != NULL);
	return (LRESULT) m_Impl.CreateNewToolBar ((LPCTSTR) lp);
}
//***************************************************************************************
LRESULT CBCGPFrameWnd::OnToolbarDelete(WPARAM,LPARAM lp)
{
	CBCGPToolBar* pToolbar = (CBCGPToolBar*) lp;
	ASSERT_VALID (pToolbar);

	return (LRESULT) m_Impl.DeleteToolBar (pToolbar);
}
//***************************************************************************************
BOOL CBCGPFrameWnd::DockControlBarLeftOf (CBCGPControlBar* pBar, CBCGPControlBar* pLeftOf)
{
	return m_dockManager.DockControlBarLeftOf (pBar, pLeftOf);
}
//************************************************************************************
void CBCGPFrameWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	switch (nState)
	{
	case WA_CLICKACTIVE:
		UpdateWindow ();
		break;

	case WA_INACTIVE:
		if (!CBCGPToolBar::IsCustomizeMode ())
		{
			m_Impl.DeactivateMenu ();
		}
		break;
	}
}
//****************************************************************************
#if _MSC_VER >= 1300
void CBCGPFrameWnd::OnActivateApp(BOOL bActive, DWORD /*dwThreadID*/)
#else
void CBCGPFrameWnd::OnActivateApp(BOOL bActive, HTASK /*hTask*/) 
#endif
{
	m_dockManager.OnActivateFrame (bActive);	
}
//****************************************************************************
void CBCGPFrameWnd::DelayUpdateFrameMenu(HMENU hMenuAlt)
{
	OnUpdateFrameMenu (hMenuAlt);
	CFrameWnd::DelayUpdateFrameMenu (hMenuAlt);
}
//****************************************************************************
COleClientItem*	CBCGPFrameWnd::GetInPlaceActiveItem ()
{
	CFrameWnd* pActiveFrame = GetActiveFrame ();
	if (pActiveFrame == NULL)
	{
		return NULL;
	}

	ASSERT_VALID (pActiveFrame);

	CView* pView = pActiveFrame->GetActiveView ();
	if (pView == NULL)
	{
		return NULL;
	}

	ASSERT_VALID (pView);

	COleDocument* pDoc = DYNAMIC_DOWNCAST (COleDocument, pView->GetDocument ());
	if (pDoc == NULL)
	{
		return NULL;
	}

	ASSERT_VALID (pDoc);
	return pDoc->GetInPlaceActiveItem (pView);
}
//****************************************************************************
void CBCGPFrameWnd::OnUpdateFrameMenu (HMENU hMenuAlt)
{
	CFrameWnd::OnUpdateFrameMenu (hMenuAlt);

	if (m_Impl.m_pMenuBar != NULL &&
		(m_Impl.m_pMenuBar->GetStyle () & WS_VISIBLE))
	{
		COleClientItem*	pActiveItem = GetInPlaceActiveItem ();
		if (pActiveItem == NULL ||
			pActiveItem->GetInPlaceWindow () == NULL)
		{
			SetMenu (NULL);
		}
		else
		{
			SetMenu (CMenu::FromHandle (hMenuAlt));
		}
	}
}
//*****************************************************************************
void CBCGPFrameWnd::OnDestroy() 
{
    if (m_hAccelTable != NULL)
    {
        ::DestroyAcceleratorTable (m_hAccelTable);
        m_hAccelTable = NULL;
    }

	m_dockManager.m_bEnableAdjustLayout = FALSE;

	CList<HWND, HWND> lstChildren;
	CWnd* pNextWnd = GetTopWindow ();
	while (pNextWnd != NULL)
	{
		lstChildren.AddTail (pNextWnd->m_hWnd);
		pNextWnd = pNextWnd->GetNextWindow ();
	}

	for (POSITION pos = lstChildren.GetHeadPosition (); pos != NULL;)
	{
		HWND hwndNext = lstChildren.GetNext (pos);
		if (IsWindow (hwndNext) && ::GetParent (hwndNext) == m_hWnd)
		{
			::DestroyWindow (hwndNext);
		}
	}

	CFrameWnd::OnDestroy();
}

//*****************************************************************************
//******************* dockmanager layer ***************************************
//*****************************************************************************
void CBCGPFrameWnd::AddDockBar ()
{
	ASSERT_VALID (this);
}
//*****************************************************************************
BOOL CBCGPFrameWnd::AddControlBar (CBCGPBaseControlBar* pControlBar, BOOL bTail)
{
	ASSERT_VALID (this);
	return m_dockManager.AddControlBar (pControlBar, bTail);
}
//*****************************************************************************
BOOL CBCGPFrameWnd::InsertControlBar (CBCGPBaseControlBar* pControlBar, 
									  CBCGPBaseControlBar* pTarget, BOOL bAfter)
{
	ASSERT_VALID (this);
	return m_dockManager.InsertControlBar (pControlBar, pTarget, bAfter);
}
//*****************************************************************************
void CBCGPFrameWnd::RemoveControlBarFromDockManager (CBCGPBaseControlBar* pControlBar, BOOL bDestroy,
									  BOOL bAdjustLayout, BOOL bAutoHide)
{
	ASSERT_VALID (this);
	m_dockManager.RemoveControlBarFromDockManager (pControlBar, bDestroy, bAdjustLayout, bAutoHide);
}
//*****************************************************************************
void CBCGPFrameWnd::DockControlBar (CBCGPBaseControlBar* pBar, UINT nDockBarID, 
									LPCRECT lpRect)
{
	ASSERT_VALID (this);
	m_dockManager.DockControlBar (pBar, nDockBarID, lpRect);
}
//*****************************************************************************
CBCGPBaseControlBar* CBCGPFrameWnd::ControlBarFromPoint (CPoint point, 
							int nSensitivity, bool bExactBar, 
							CRuntimeClass* pRTCBarType) const
{
	ASSERT_VALID (this);
	return m_dockManager.ControlBarFromPoint (point, nSensitivity, bExactBar, 
												pRTCBarType);
}
//*****************************************************************************
CBCGPBaseControlBar* CBCGPFrameWnd::ControlBarFromPoint (CPoint point, 
								int nSensitivity, DWORD& dwAlignment, 
								CRuntimeClass* pRTCBarType) const
{
	ASSERT_VALID (this);
	return m_dockManager.ControlBarFromPoint (point, nSensitivity, dwAlignment, 
												pRTCBarType);
}
//*****************************************************************************
BOOL CBCGPFrameWnd::IsPointNearDockBar (CPoint point, DWORD& dwBarAlignment, 
										BOOL& bOuterEdge) const
{
	ASSERT_VALID (this);
	return m_dockManager.IsPointNearDockBar (point, dwBarAlignment, bOuterEdge);
}
//*****************************************************************************
void CBCGPFrameWnd::AdjustDockingLayout (HDWP hdwp)
{
	ASSERT_VALID (this);
	
	if (m_dockManager.IsInAdjustLayout ())
	{
		return;
	}

	m_dockManager.AdjustDockingLayout (hdwp);

	AdjustClientArea ();
	if (m_dockManager.IsOLEContainerMode ())
	{
		RecalcLayout ();
	}
}
//*****************************************************************************
void CBCGPFrameWnd::AdjustClientArea ()
{
	CWnd* pChildWnd = GetDlgItem (AFX_IDW_PANE_FIRST);
	if (pChildWnd != NULL)
	{
		CRect rectClientAreaBounds = m_dockManager.GetClientAreaBounds ();

		rectClientAreaBounds.left += m_rectBorder.left;
		rectClientAreaBounds.top  += m_rectBorder.top;
		rectClientAreaBounds.right -= m_rectBorder.right;
		rectClientAreaBounds.bottom -= m_rectBorder.bottom;

		pChildWnd->CalcWindowRect (rectClientAreaBounds);

		if (!pChildWnd->IsKindOf (RUNTIME_CLASS (CSplitterWnd)))
		{
			pChildWnd->ModifyStyle (0, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
		}
		else
		{
			pChildWnd->ModifyStyle (0, WS_CLIPSIBLINGS);
		}

		
		pChildWnd->SetWindowPos (&wndBottom, rectClientAreaBounds.left, 
										rectClientAreaBounds.top, 
										rectClientAreaBounds.Width (), 
										rectClientAreaBounds.Height (),
										SWP_NOACTIVATE);
	}
}
//*****************************************************************************
BOOL CBCGPFrameWnd::OnMoveMiniFrame	(CWnd* pFrame)
{
	ASSERT_VALID (this);
	return m_dockManager.OnMoveMiniFrame (pFrame);
}
//*****************************************************************************
BOOL CBCGPFrameWnd::EnableDocking (DWORD dwDockStyle)
{
	return m_dockManager.EnableDocking (dwDockStyle);
}
//****************************************************************************************
BOOL CBCGPFrameWnd::EnableAutoHideBars (DWORD dwDockStyle)
{
	return m_dockManager.EnableAutoHideBars (dwDockStyle);
}
//****************************************************************************************
CBCGPBaseControlBar* CBCGPFrameWnd::GetControlBar (UINT nID)
{
	ASSERT_VALID (this);

	CBCGPBaseControlBar* pBar = m_dockManager.FindBarByID (nID, TRUE);
	return pBar;
}
//****************************************************************************************
void CBCGPFrameWnd::ShowControlBar (CBCGPBaseControlBar* pBar, BOOL bShow, BOOL bDelay, BOOL bActivate)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pBar);

	pBar->ShowControlBar (bShow, bDelay, bActivate);
}
//*************************************************************************************
void CBCGPFrameWnd::OnUpdateControlBarMenu(CCmdUI* pCmdUI)
{
	CBCGPBaseControlBar* pBar = GetControlBar(pCmdUI->m_nID);
	if (pBar != NULL)
	{
		pCmdUI->SetCheck((pBar->GetStyle() & WS_VISIBLE) != 0);
		return;
	}

	pCmdUI->ContinueRouting();
}
//*************************************************************************************
BOOL CBCGPFrameWnd::OnBarCheck(UINT nID)
{
	ASSERT_VALID (this);

	CBCGPBaseControlBar* pBar = GetControlBar(nID);
	if (pBar != NULL)
	{
		ShowControlBar(pBar, (pBar->GetStyle() & WS_VISIBLE) == 0, FALSE, FALSE);
		return TRUE;
	}

	return FALSE;
}
//*************************************************************************************
void CBCGPFrameWnd::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CFrameWnd::OnSizing(fwSide, pRect);
	
	AdjustDockingLayout ();	
}
//*************************************************************************************
void CBCGPFrameWnd::RecalcLayout (BOOL bNotify)
{
	if (m_bInRecalcLayout)
		return;

	m_bInRecalcLayout = TRUE;

	if (!m_bIsMinimized)
	{
		CView* pView = GetActiveView ();

		if (m_dockManager.IsPrintPreviewValid () ||
			m_dockManager.IsOLEContainerMode ())
		{
			if (pView != NULL && pView->IsKindOf (RUNTIME_CLASS (CBCGPPrintPreviewView)))
			{
	
				m_dockManager.RecalcLayout (bNotify);
				CRect rectClient = m_dockManager.GetClientAreaBounds ();
				pView->SetWindowPos (NULL, rectClient.left, rectClient.top, 
										rectClient.Width (), rectClient.Height (),
										SWP_NOZORDER  | SWP_NOACTIVATE);
			}
			else
			{
#ifdef _BCGP_USE_OLEIMPL_				
				COleClientItem*	pActiveItem = GetInPlaceActiveItem ();
				if (bNotify && pActiveItem != NULL && pActiveItem->m_pInPlaceFrame != NULL)
				{
					pActiveItem->m_pInPlaceFrame->OnRecalcLayout ();
				}
#else			
				m_bInRecalcLayout = FALSE;
				CFrameWnd::RecalcLayout (bNotify);
#endif
				AdjustClientArea ();	
			}
		}
		else
		{
			m_dockManager.RecalcLayout (bNotify);
			AdjustClientArea ();
		}
	}

	m_bInRecalcLayout = FALSE;
}
//****************************************************************************************
BOOL CBCGPFrameWnd::NegotiateBorderSpace( UINT nBorderCmd, LPRECT lpRectBorder )
{
	CRect border, request;

	switch (nBorderCmd)
	{
	case borderGet:
	{
		CFrameWnd::NegotiateBorderSpace (nBorderCmd, lpRectBorder);
		CRect rectBounds = m_dockManager.GetClientAreaBounds ();
		ASSERT(lpRectBorder != NULL);

		*lpRectBorder = rectBounds;
		break;
	}
	case borderRequest:
		return TRUE;

	case borderSet:
		return CFrameWnd::NegotiateBorderSpace (nBorderCmd, lpRectBorder);

	default:
		ASSERT(FALSE);  // invalid CFrameWnd::BorderCmd
	}

	return TRUE;
}
//*************************************************************************************
void CBCGPFrameWnd::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
{
	ASSERT_VALID (this);

	m_dockManager.SetPrintPreviewMode (bPreview, pState);
	DWORD dwSavedState = pState->dwStates;
	CFrameWnd::OnSetPreviewMode (bPreview, pState);
	pState->dwStates = dwSavedState;

	AdjustDockingLayout ();
	RecalcLayout ();
}
//*************************************************************************************
BOOL CBCGPFrameWnd::OnShowControlBars (BOOL bShow)
{
	ASSERT_VALID (this);
	BOOL bResult = m_dockManager.ShowControlBars (bShow);
	AdjustDockingLayout ();
	
	return bResult;
}

//********************************************************************************
LRESULT CBCGPFrameWnd::OnIdleUpdateCmdUI(WPARAM, LPARAM)
{
	m_dockManager.SendMessageToMiniFrames (WM_IDLEUPDATECMDUI);
	return 0L;
}
//****************************************************************************************
void CBCGPFrameWnd::OnSize(UINT nType, int cx, int cy) 
{
	m_bIsMinimized = (nType == SIZE_MINIMIZED);

	m_dockManager.OnActivateFrame (!m_bIsMinimized);
	

	if (!m_bIsMinimized && nType != SIZE_MAXIMIZED && !m_bWasMaximized)
	{
		m_dockManager.m_bSizeFrame = TRUE;
		CFrameWnd::OnSize(nType, cx, cy);
		AdjustDockingLayout ();			
		m_dockManager.m_bSizeFrame = FALSE;
		return;
	}

	CFrameWnd::OnSize(nType, cx, cy);

	if (nType == SIZE_MAXIMIZED || (nType == SIZE_RESTORED && m_bWasMaximized))
	{
		RecalcLayout ();
	}

	m_bWasMaximized = (nType == SIZE_MAXIMIZED);
}
//****************************************************************************************
LRESULT CBCGPFrameWnd::OnExitSizeMove (WPARAM, LPARAM)
{
	m_dockManager.FixupVirtualRects ();
	return 0;
}
//****************************************************************************************
void CBCGPFrameWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	if (IsFullScreen ())
	{
		m_Impl.m_FullScreenMgr.OnGetMinMaxInfo (lpMMI);

	}
	else
	{
		CFrameWnd::OnGetMinMaxInfo(lpMMI);
	}
}

