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

// BCGPMDIChildWnd.cpp : implementation file
//

#include "stdafx.h"
#include "BCGCBPro.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPMDIChildWnd.h"
#include "BCGPMiniFrameWnd.h"
#include "BCGPPrintPreviewView.h"
#include "BCGPMainClientAreaWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CBCGPMDIChildWnd::m_bEnableFloatingBars = FALSE;

/////////////////////////////////////////////////////////////////////////////
// CBCGPMDIChildWnd

IMPLEMENT_DYNCREATE(CBCGPMDIChildWnd, CMDIChildWnd)

CBCGPMDIChildWnd::CBCGPMDIChildWnd()
{
	m_pMDIFrame = NULL;
	m_bToBeDestroyed = FALSE;
	m_bWasMaximized = FALSE;
	m_bIsMinimized = FALSE;
}

CBCGPMDIChildWnd::~CBCGPMDIChildWnd()
{
}


BEGIN_MESSAGE_MAP(CBCGPMDIChildWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CBCGPMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_MDIACTIVATE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_DESTROY()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETTEXT,OnSetText)
	ON_MESSAGE(WM_SETICON,OnSetIcon)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPMDIChildWnd message handlers

BOOL CBCGPMDIChildWnd::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST &&
		m_pMDIFrame != NULL &&
		m_pMDIFrame->GetActivePopup () != NULL)
	{
		// Don't process accelerators if popup window is active
		return FALSE;
	}

	return CMDIChildWnd::PreTranslateMessage(pMsg);
}
//*******************************************************************************
int CBCGPMDIChildWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pMDIFrame = DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, GetMDIFrame ());
	ASSERT_VALID (m_pMDIFrame);
	
	return 0;
}
//*************************************************************************************
BOOL CBCGPMDIChildWnd::DockControlBarLeftOf(CBCGPControlBar* pBar, CBCGPControlBar* pLeftOf)
{
	m_dockManager.DockControlBarLeftOf (pBar, pLeftOf);
	return TRUE;
}
//*************************************************************************************
void CBCGPMDIChildWnd::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	static BOOL bActivating = FALSE;

	m_dockManager.OnActivateFrame (bActivate);
	if (!bActivating)
	{
		bActivating = TRUE;

		CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

		if (bActivate && m_pMDIFrame != NULL)
		{
			ASSERT_VALID (m_pMDIFrame);
			m_pMDIFrame->m_wndClientArea.SetActiveTab (pActivateWnd->GetSafeHwnd ());
		}
		bActivating = FALSE;
	}

}
//*************************************************************************************
void CBCGPMDIChildWnd::ActivateFrame(int nCmdShow) 
{
	GetParent ()->SetRedraw (FALSE);
	CMDIChildWnd::ActivateFrame(nCmdShow);

	GetParent ()->SetRedraw (TRUE);
	GetParent ()->RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
}
//*************************************************************************************
LRESULT CBCGPMDIChildWnd::OnSetText(WPARAM,LPARAM)
{
	LRESULT lRes = Default();

	if (m_pMDIFrame != NULL)
	{
		ASSERT_VALID (m_pMDIFrame);
		m_pMDIFrame->m_wndClientArea.UpdateTabs (TRUE);
	}

	return lRes;
}
//*************************************************************************************
LRESULT CBCGPMDIChildWnd::OnSetIcon(WPARAM,LPARAM)
{
	LRESULT lRes = Default();

	if (m_pMDIFrame != NULL)
	{
		ASSERT_VALID (m_pMDIFrame);
		m_pMDIFrame->m_wndClientArea.UpdateTabs ();
	}

	return lRes;
}
//*************************************************************************************
CString CBCGPMDIChildWnd::GetFrameText () const
{
	ASSERT_VALID (this);

	CString strText;
	GetWindowText (strText);

	return strText;
}
//*************************************************************************************
HICON CBCGPMDIChildWnd::GetFrameIcon () const
{
	ASSERT_VALID (this);

	HICON hIcon = GetIcon (FALSE);
	if (hIcon == NULL)
	{
		hIcon = (HICON)(LONG_PTR) GetClassLongPtr (GetSafeHwnd (), GCLP_HICONSM);
	}

	return hIcon;
}
//*************************************************************************************
void CBCGPMDIChildWnd::OnUpdateFrameTitle (BOOL bAddToTitle)
{
	CMDIChildWnd::OnUpdateFrameTitle (bAddToTitle);

	if (m_pMDIFrame != NULL)
	{
		ASSERT_VALID (m_pMDIFrame);
		m_pMDIFrame->m_wndClientArea.UpdateTabs ();
	}
}
//*****************************************************************************
void CBCGPMDIChildWnd::OnSize(UINT nType, int cx, int cy) 
{
	if (m_bToBeDestroyed)
	{
		// prevents main menu flickering when the last dockument is being closed
		return;
	}
	
	m_bIsMinimized = (nType == SIZE_MINIMIZED);


	if (!m_bIsMinimized && nType != SIZE_MAXIMIZED && !m_bWasMaximized)
	{
		m_dockManager.m_bSizeFrame = TRUE;
		CMDIChildWnd::OnSize(nType, cx, cy);
		AdjustDockingLayout ();			
		m_dockManager.m_bSizeFrame = FALSE;
		return;
	}

	CMDIChildWnd::OnSize(nType, cx, cy);

	if (nType == SIZE_MAXIMIZED || (nType == SIZE_RESTORED && m_bWasMaximized))
	{
		RecalcLayout ();
	}

	m_bWasMaximized = (nType == SIZE_MAXIMIZED);
}
//*****************************************************************************
BOOL CBCGPMDIChildWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	m_dockManager.Create (this);
	
	return CMDIChildWnd::PreCreateWindow(cs);
}


//*****************************************************************************
//******************* dockmanager layer ***************************************
//*****************************************************************************
void CBCGPMDIChildWnd::AddDockBar ()
{
	ASSERT_VALID (this);
}
//*****************************************************************************
BOOL CBCGPMDIChildWnd::AddControlBar (CBCGPBaseControlBar* pControlBar, BOOL bTail)
{
	ASSERT_VALID (this);
	return m_dockManager.AddControlBar (pControlBar, bTail);
}
//*****************************************************************************
BOOL CBCGPMDIChildWnd::InsertControlBar (CBCGPBaseControlBar* pControlBar, 
									  CBCGPBaseControlBar* pTarget, BOOL bAfter)
{
	ASSERT_VALID (this);
	return m_dockManager.InsertControlBar (pControlBar, pTarget, bAfter);
}
//*****************************************************************************
void CBCGPMDIChildWnd::RemoveControlBarFromDockManager (CBCGPBaseControlBar* pControlBar, BOOL bDestroy,
										 BOOL bAdjustLayout, BOOL bAutoHide)
{
	ASSERT_VALID (this);
	m_dockManager.RemoveControlBarFromDockManager (pControlBar, bDestroy, bAdjustLayout, bAutoHide);
}
//*****************************************************************************
void CBCGPMDIChildWnd::DockControlBar (CBCGPBaseControlBar* pBar, UINT nDockBarID, 
									LPCRECT /*lpRect*/)
{
	ASSERT_VALID (this);

	if (pBar->CanFloat () && !CBCGPMDIChildWnd::m_bEnableFloatingBars)
	{
		// bar can't be floating
		pBar->m_dwBCGStyle &= ~CBRS_BCGP_FLOAT;
	}


	if (pBar->CanBeResized () || pBar->CanFloat ())
	{
		pBar->EnableDocking (CBRS_ALIGN_ANY);
		m_dockManager.DockControlBar (pBar, nDockBarID);
	}
	else
	{
		AddControlBar (pBar, TRUE);
	}
}
//*****************************************************************************
CBCGPBaseControlBar* CBCGPMDIChildWnd::GetControlBar (UINT nID)
{
	ASSERT_VALID (this);
	
	CBCGPBaseControlBar* pBar = m_dockManager.FindBarByID (nID, TRUE);

	return pBar;
}
//*****************************************************************************
void CBCGPMDIChildWnd::ShowControlBar (CBCGPBaseControlBar* pBar, BOOL bShow, 
									   BOOL bDelay, BOOL bActivate)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pBar);

	pBar->ShowControlBar (bShow, bDelay, bActivate);
}
//*****************************************************************************
CBCGPBaseControlBar* CBCGPMDIChildWnd::ControlBarFromPoint (CPoint point, 
							int nSensitivity, bool bExactBar, 
							CRuntimeClass* pRTCBarType) const
{
	ASSERT_VALID (this);
	return m_dockManager.ControlBarFromPoint (point, nSensitivity, bExactBar, 
												pRTCBarType);
}
//*****************************************************************************
CBCGPBaseControlBar* CBCGPMDIChildWnd::ControlBarFromPoint (CPoint point, 
								int nSensitivity, DWORD& dwAlignment, 
								CRuntimeClass* pRTCBarType) const
{
	ASSERT_VALID (this);
	return m_dockManager.ControlBarFromPoint (point, nSensitivity, dwAlignment, 
												pRTCBarType);
}
//*****************************************************************************
BOOL CBCGPMDIChildWnd::IsPointNearDockBar (CPoint point, DWORD& dwBarAlignment, 
										   BOOL& bOuterEdge) const
{
	ASSERT_VALID (this);
	return m_dockManager.IsPointNearDockBar (point, dwBarAlignment, bOuterEdge);
}
//*****************************************************************************
void CBCGPMDIChildWnd::AdjustDockingLayout (HDWP hdwp)
{
	ASSERT_VALID (this);
	
	if (m_dockManager.IsInAdjustLayout ())
	{	
		return;
	}

	m_dockManager.AdjustDockingLayout (hdwp);
	AdjustClientArea ();

}
//*****************************************************************************
void CBCGPMDIChildWnd::AdjustClientArea ()
{
	CWnd* pChildWnd = GetDlgItem (AFX_IDW_PANE_FIRST);
	if (pChildWnd != NULL)
	{
		if (!pChildWnd->IsKindOf (RUNTIME_CLASS (CSplitterWnd)))
		{
			pChildWnd->ModifyStyle (0, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
		}
		else
		{
			pChildWnd->ModifyStyle (0, WS_CLIPSIBLINGS);
		}

		if (!CBCGPDockManager::m_bFullScreenMode)
		{
			CRect rectClientAreaBounds = m_dockManager.GetClientAreaBounds ();

			rectClientAreaBounds.left += m_rectBorder.left;
			rectClientAreaBounds.top  += m_rectBorder.top;
			rectClientAreaBounds.right -= m_rectBorder.right;
			rectClientAreaBounds.bottom -= m_rectBorder.bottom;
			
			
			
			pChildWnd->SetWindowPos (&wndBottom, rectClientAreaBounds.left, 
											rectClientAreaBounds.top, 
											rectClientAreaBounds.Width (), 
											rectClientAreaBounds.Height (),
											SWP_NOACTIVATE);
		}
		else
		{
			/*
			ModifyStyle (WS_CAPTION | WS_THICKFRAME, 0);

			int nCXBorder = GetSystemMetrics (SM_CXBORDER);
			int nCYBorder = GetSystemMetrics (SM_CYBORDER);

			int nCXScreen = GetSystemMetrics (SM_CXSCREEN);
			int nCYScreen = GetSystemMetrics (SM_CYSCREEN);

			int nCYCaption = GetSystemMetrics (SM_CYCAPTION);

			pChildWnd->SetWindowPos (&wndBottom, 0, 0, nCXScreen, nCYScreen, SWP_NOACTIVATE);
			SetWindowPos (&wndBottom, 0, 0, nCXScreen, nCYScreen, SWP_NOACTIVATE);
			*/

/*			SetWindowPos (NULL, -nCXBorder, -nCYBorder - nCYCaption - 20, 
						nCXScreen + 2*nCXBorder , nCYScreen + 2*nCYBorder + nCYCaption + 20, 
						SWP_NOACTIVATE | SWP_NOZORDER);
*/
		}
		
	}
}
//*****************************************************************************
BOOL CBCGPMDIChildWnd::OnMoveMiniFrame	(CWnd* pFrame)
{
	ASSERT_VALID (this);
	return m_dockManager.OnMoveMiniFrame (pFrame);
}
//****************************************************************************************
BOOL CBCGPMDIChildWnd::EnableDocking (DWORD dwDockStyle)
{
	return m_dockManager.EnableDocking (dwDockStyle);
}
//****************************************************************************************
BOOL CBCGPMDIChildWnd::EnableAutoHideBars (DWORD dwDockStyle)
{
	return m_dockManager.EnableAutoHideBars (dwDockStyle);
}
//*************************************************************************************
void CBCGPMDIChildWnd::RecalcLayout (BOOL bNotify)
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
				CMDIChildWnd::RecalcLayout (bNotify);
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
//*************************************************************************************
void CBCGPMDIChildWnd::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CMDIChildWnd::OnSizing(fwSide, pRect);

	CRect rect;
	GetWindowRect (rect);

	if (rect.Size () != CRect (pRect).Size ())
	{
		AdjustDockingLayout ();	
	}
	
}
//*************************************************************************************
void CBCGPMDIChildWnd::OnDestroy() 
{
	for (POSITION pos = m_dockManager.m_lstMiniFrames.GetHeadPosition (); pos != NULL;)
	{
		CBCGPMiniFrameWnd* pNextFrame = DYNAMIC_DOWNCAST (CBCGPMiniFrameWnd,
					m_dockManager.m_lstMiniFrames.GetNext (pos));
		if (pNextFrame != NULL)
		{
			pNextFrame->DestroyWindow ();
		}
	}

	CList<HWND, HWND> lstChildren;
	CWnd* pNextWnd = GetTopWindow ();
	while (pNextWnd != NULL)
	{
		lstChildren.AddTail (pNextWnd->m_hWnd);
		pNextWnd = pNextWnd->GetNextWindow ();
	}

	for (pos = lstChildren.GetHeadPosition (); pos != NULL;)
	{
		HWND hwndNext = lstChildren.GetNext (pos);
		if (IsWindow (hwndNext) && ::GetParent (hwndNext) == m_hWnd)
		{
			::DestroyWindow (hwndNext);
		}
	}

	CMDIChildWnd::OnDestroy();
}
//*************************************************************************************
void CBCGPMDIChildWnd::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CMDIChildWnd::OnWindowPosChanging(lpwndpos);
}
//****************************************************************************************
void CBCGPMDIChildWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	if (m_pMDIFrame != NULL && m_pMDIFrame->IsFullScreen() && 
		!m_pMDIFrame->AreMDITabs())
	{
		m_pMDIFrame->m_Impl.GetFullScreenMinMaxInfo(lpMMI);
	}
	else
	{
		CMDIChildWnd::OnGetMinMaxInfo(lpMMI);
	}
}
