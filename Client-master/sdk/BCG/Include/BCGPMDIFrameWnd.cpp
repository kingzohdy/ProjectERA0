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

// BCGPMDIFrameWnd.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPToolbar.h"
#include "BCGPMenuBar.h"
#include "BCGPPopupMenu.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPMiniFrameWnd.h"
#include "BCGPLocalResource.h"
#include "bcgprores.h"
#include "BCGPWindowsManagerDlg.h"
#include "BCGPUserToolsManager.h"

#ifdef _BCGP_USE_OLEIMPL_
#include "oleimpl2.h"
#endif

#include "BCGPDockingControlBar.h"
#include "BCGPTabbedControlBar.h"
#include "BCGPPrintPreviewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPMDIFrameWnd

IMPLEMENT_DYNCREATE(CBCGPMDIFrameWnd, CMDIFrameWnd)

#pragma warning (disable : 4355)

CBCGPMDIFrameWnd::CBCGPMDIFrameWnd() :
	m_Impl (this),
	m_hmenuWindow (NULL),
	m_bContextHelp (FALSE),
	m_bDoSubclass (TRUE),
	m_uiWindowsDlgMenuId (0),
	m_bShowWindowsDlgAlways (FALSE),
	m_bShowWindowsDlgHelpButton (FALSE),
	m_bWasMaximized (FALSE),
	m_bIsMinimized (FALSE),
	m_bClosing (FALSE)
{
	
}

#pragma warning (default : 4355)

CBCGPMDIFrameWnd::~CBCGPMDIFrameWnd()
{
}

//BEGIN_MESSAGE_MAP(CBCGPMDIFrameWnd, CMDIFrameWnd)
BEGIN_MESSAGE_MAP(CBCGPMDIFrameWnd, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CBCGPMDIFrameWnd)
	ON_WM_MENUCHAR()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ACTIVATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
	ON_WM_ACTIVATEAPP()
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_REGISTERED_MESSAGE(BCGM_CREATETOOLBAR, OnToolbarCreateNew)
	ON_REGISTERED_MESSAGE(BCGM_DELETETOOLBAR, OnToolbarDelete)
	ON_COMMAND( ID_CONTEXT_HELP, OnContextHelp)
	ON_MESSAGE( WM_EXITSIZEMOVE, OnExitSizeMove )
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_STATUS_BAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_TOOLBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REBAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_REBAR, OnBarCheck)
	ON_COMMAND(ID_WINDOW_NEW, OnWindowNew)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPMDIFrameWnd message handlers

BOOL CBCGPMDIFrameWnd::OnSetMenu (HMENU hmenu)
{
	COleClientItem*	pActiveItem = GetInPlaceActiveItem ();
	if (pActiveItem != NULL && 
		pActiveItem->GetInPlaceWindow () != NULL)
	{
		return FALSE;
	}

	if (m_Impl.m_pMenuBar != NULL)
	{
		SetMenu (NULL);
		m_Impl.m_pMenuBar->CreateFromMenu (hmenu == NULL ? m_Impl.m_hDefaultMenu : hmenu);
		return TRUE;
	}

	return FALSE;
}
//*******************************************************************************************
BOOL CBCGPMDIFrameWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if (!CMDIFrameWnd::OnCreateClient(lpcs, pContext))
	{
		return FALSE;
	}

	if (m_bDoSubclass)
	{
		m_wndClientArea.SubclassWindow (m_hWndMDIClient);
	}

	return TRUE;
}
//*******************************************************************************************
LRESULT CBCGPMDIFrameWnd::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) 
{
	if (m_Impl.OnMenuChar (nChar))
	{
		return MAKELPARAM (MNC_EXECUTE, -1);
	}
		
	return CMDIFrameWnd::OnMenuChar(nChar, nFlags, pMenu);
}
//*******************************************************************************************
void CBCGPMDIFrameWnd::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CMDIFrameWnd::OnWindowPosChanged(lpwndpos);
	
	if (m_Impl.m_pMenuBar != NULL)
	{
		BOOL bMaximized;
		CMDIChildWnd* pChild = MDIGetActive (&bMaximized);

		if (pChild == NULL || !bMaximized)
		{
			m_Impl.m_pMenuBar->SetMaximizeMode (FALSE);
		}
		else
		{
			m_Impl.m_pMenuBar->SetMaximizeMode (TRUE, pChild);
		}
	}
}
//*******************************************************************************************
BOOL CBCGPMDIFrameWnd::PreTranslateMessage(MSG* pMsg) 
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
			m_Impl.ProcessKeyboard ((int) pMsg->wParam))
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
			CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);

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
			CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);
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

	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}
//*******************************************************************************************
BOOL CBCGPMDIFrameWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
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
		return CMDIFrameWnd::OnCommand(wParam, lParam);
	}

	return FALSE;
}
//*******************************************************************************************
HMENU CBCGPMDIFrameWnd::GetWindowMenuPopup (HMENU hMenuBar)
{
	m_hmenuWindow = CMDIFrameWnd::GetWindowMenuPopup (hMenuBar);
	return m_hmenuWindow;
}
//********************************************************************************************
BOOL CBCGPMDIFrameWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	m_Impl.m_nIDDefaultResource = nIDResource;
	m_Impl.LoadLargeIconsState ();
	
	if (!CMDIFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	m_Impl.OnLoadFrame ();

	if (GetMenuBar () != NULL)
	{
		m_hMenuDefault = m_Impl.m_hDefaultMenu;
	}

	return TRUE;
}
//*******************************************************************************************
void CBCGPMDIFrameWnd::OnClose() 
{
	m_bClosing = TRUE;
	// Deactivate OLE container first:
	COleClientItem*	pActiveItem = GetInPlaceActiveItem ();
	if (pActiveItem != NULL)
	{
		pActiveItem->Deactivate ();
	}

	m_Impl.OnCloseFrame();	
	CMDIFrameWnd::OnClose();
}
//*******************************************************************************************
BOOL CBCGPMDIFrameWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	m_Impl.SetDockManager (&m_dockManager);
	m_Impl.RestorePosition(cs);	
	return CMDIFrameWnd::PreCreateWindow(cs);
}
//*******************************************************************************************
BOOL CBCGPMDIFrameWnd::ShowPopupMenu (CBCGPPopupMenu* pMenuPopup)
{
	if (!m_Impl.OnShowPopupMenu (pMenuPopup, this))
	{
		return FALSE;
	}

	if (!CBCGPToolBar::IsCustomizeMode () && m_hmenuWindow != NULL &&
		pMenuPopup != NULL && pMenuPopup->GetMenu () != NULL)
	{
		//-----------------------------------------------------------
		// Check the popup menu for the "Windows..." menu maching...:
		//-----------------------------------------------------------
		HMENU hMenuPop = pMenuPopup->GetMenu ();
		BOOL bIsWindowMenu = FALSE;

		int iItemMax = ::GetMenuItemCount (hMenuPop);
		for (int iItemPop = 0; !bIsWindowMenu && iItemPop < iItemMax; iItemPop ++)
		{
			UINT nID = ::GetMenuItemID( hMenuPop, iItemPop);
			bIsWindowMenu =  (nID >= AFX_IDM_WINDOW_FIRST && nID <= AFX_IDM_WINDOW_LAST);
		}

		if (bIsWindowMenu)
		{
			CMenu* pMenu = CMenu::FromHandle (m_hmenuWindow);
			if (pMenu != NULL)
			{
				int iCount = (int) pMenu->GetMenuItemCount ();
				BOOL bIsFirstWindowItem = TRUE;
				BOOL bIsStandradWindowsDlg = FALSE;

				for (int i = 0; i < iCount; i ++)
				{
					UINT uiCmd = pMenu->GetMenuItemID (i);
					if (uiCmd < AFX_IDM_FIRST_MDICHILD || uiCmd == (UINT) -1)
					{
						continue;
					}

					if (m_uiWindowsDlgMenuId != 0 &&
						uiCmd == AFX_IDM_FIRST_MDICHILD + 9)
					{
						// Don't add standrd "Windows..." command
						bIsStandradWindowsDlg = TRUE;
						continue;
					}

					if (bIsFirstWindowItem)
					{
						pMenuPopup->InsertSeparator ();
						bIsFirstWindowItem = FALSE;

						::SendMessage (m_hWndMDIClient, WM_MDIREFRESHMENU, 0, 0);
     				}

					CString strText;
					pMenu->GetMenuString (i, strText, MF_BYPOSITION);

					CBCGPToolbarMenuButton button (uiCmd, NULL, 
													-1, strText);

					UINT uiState = pMenu->GetMenuState (i, MF_BYPOSITION);
					if (uiState & MF_CHECKED)
					{
						button.m_nStyle |= TBBS_CHECKED;
					}

					pMenuPopup->InsertItem (button);
				}

				if (m_uiWindowsDlgMenuId != 0 &&
					(bIsStandradWindowsDlg || m_bShowWindowsDlgAlways))
				{
					if (!CBCGPToolBar::GetBasicCommands ().IsEmpty ())
					{
						CBCGPToolBar::AddBasicCommand (m_uiWindowsDlgMenuId);
					}

					//-----------------------------
					// Add our "Windows..." dialog:
					//-----------------------------
					pMenuPopup->InsertItem (
						CBCGPToolbarMenuButton (m_uiWindowsDlgMenuId,
								NULL, -1, m_strWindowsDlgMenuText));
				}
			}
		}
	}

	if (pMenuPopup != NULL && pMenuPopup->m_bShown)
	{
		return TRUE;
	}

	return OnShowPopupMenu (pMenuPopup);
}
//**********************************************************************************
void CBCGPMDIFrameWnd::OnClosePopupMenu (CBCGPPopupMenu* pMenuPopup)
{
	if (CBCGPPopupMenu::m_pActivePopupMenu == pMenuPopup)
	{
		CBCGPPopupMenu::m_pActivePopupMenu = NULL;
	}
}
//*******************************************************************************************
LRESULT CBCGPMDIFrameWnd::OnToolbarCreateNew(WPARAM,LPARAM lp)
{
	ASSERT (lp != NULL);
	return (LRESULT) m_Impl.CreateNewToolBar ((LPCTSTR) lp);
}
//***************************************************************************************
LRESULT CBCGPMDIFrameWnd::OnToolbarDelete(WPARAM,LPARAM lp)
{
	CBCGPToolBar* pToolbar = (CBCGPToolBar*) lp;
	ASSERT_VALID (pToolbar);

	return (LRESULT) m_Impl.DeleteToolBar (pToolbar);
}
//***************************************************************************************
void CBCGPMDIFrameWnd::WinHelp(DWORD dwData, UINT nCmd) 
{
	if (dwData > 0 || !m_bContextHelp)
	{
		CMDIFrameWnd::WinHelp(dwData, nCmd);
	}
	else
	{
		OnContextHelp ();
	}
}
//***************************************************************************************
void CBCGPMDIFrameWnd::OnContextHelp ()
{
	m_bContextHelp = TRUE;

	if (!m_bHelpMode && CanEnterHelpMode())
	{
		CBCGPToolBar::SetHelpMode ();
	}

	CMDIFrameWnd::OnContextHelp ();

	if (!m_bHelpMode)
	{
		CBCGPToolBar::SetHelpMode (FALSE);
	}

	m_bContextHelp = FALSE;
}
//***************************************************************************************
BOOL CBCGPMDIFrameWnd::DockControlBarLeftOf (CBCGPControlBar* pBar, CBCGPControlBar* pLeftOf)
{
	return m_dockManager.DockControlBarLeftOf (pBar, pLeftOf);
}
//***************************************************************************************
void CBCGPMDIFrameWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);
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
//***************************************************************************************
#if _MSC_VER >= 1300
void CBCGPMDIFrameWnd::OnActivateApp(BOOL bActive, DWORD /*dwThreadID*/)
#else
void CBCGPMDIFrameWnd::OnActivateApp(BOOL bActive, HTASK /*hTask*/) 
#endif
{
	m_dockManager.OnActivateFrame (bActive);	
}
//***************************************************************************************
void CBCGPMDIFrameWnd::EnableWindowsDialog (UINT uiMenuId, 
										   LPCTSTR lpszMenuText,
										   BOOL bShowAllways,
										   BOOL bShowHelpButton)
{
	ASSERT (lpszMenuText != NULL);
	ASSERT (uiMenuId != 0);

	m_uiWindowsDlgMenuId = uiMenuId;
	m_strWindowsDlgMenuText = lpszMenuText;
	m_bShowWindowsDlgAlways = bShowAllways;
	m_bShowWindowsDlgHelpButton = bShowHelpButton;
}
//****************************************************************************
void CBCGPMDIFrameWnd::EnableWindowsDialog (UINT uiMenuId, 
										   UINT uiMenuTextResId,
										   BOOL bShowAllways,
										   BOOL bShowHelpButton)
{
	CString strMenuText;
	VERIFY (strMenuText.LoadString (uiMenuTextResId));

	EnableWindowsDialog (uiMenuId, strMenuText, bShowAllways, bShowHelpButton);
}
//****************************************************************************
void CBCGPMDIFrameWnd::ShowWindowsDialog ()
{
	CBCGPLocalResource locaRes;

	CBCGPWindowsManagerDlg dlg (this, m_bShowWindowsDlgHelpButton);
	dlg.DoModal ();
}
//****************************************************************************
COleClientItem*	CBCGPMDIFrameWnd::GetInPlaceActiveItem ()
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
void CBCGPMDIFrameWnd::OnUpdateFrameMenu (HMENU hMenuAlt)
{

	CMDIFrameWnd::OnUpdateFrameMenu (hMenuAlt);

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
//****************************************************************************************
void CBCGPMDIFrameWnd::OnDestroy() 
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
		if (m_wndClientArea.m_hWnd != pNextWnd->m_hWnd)
		{
			lstChildren.AddTail (pNextWnd->m_hWnd);
		}
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


	CMDIFrameWnd::OnDestroy();
}
//*************************************************************************************
void CBCGPMDIFrameWnd::EnableMDITabs (BOOL bEnable/* = TRUE*/,
				BOOL bIcons/* = TRUE*/,
				CBCGPTabWnd::Location tabLocation /* = CBCGPTabWnd::LOCATION_BOTTOM*/,
				BOOL bTabCloseButton/* = FALSE*/,
				CBCGPTabWnd::Style style/* = CBCGPTabWnd::STYLE_3D_SCROLLED*/)
{
	ASSERT (style == CBCGPTabWnd::STYLE_3D_SCROLLED ||
		style == CBCGPTabWnd::STYLE_3D_ONENOTE);

	m_wndClientArea.EnableMDITabs (bEnable, bIcons, tabLocation, bTabCloseButton, style);
}
//*************************************************************************************
void CBCGPMDIFrameWnd::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
{
	ASSERT_VALID (this);

	if (m_wndClientArea.DoesMDITabExist ())
	{
		m_wndClientArea.m_bTabIsVisible = !bPreview;
		((CWnd&)m_wndClientArea.GetMDITabs ()).ShowWindow (
			bPreview ? SW_HIDE : SW_SHOWNOACTIVATE);
	}

	m_dockManager.SetPrintPreviewMode (bPreview, pState);

	DWORD dwSavedState = pState->dwStates;
	CMDIFrameWnd::OnSetPreviewMode (bPreview, pState);
	pState->dwStates = dwSavedState;

	AdjustDockingLayout ();
	RecalcLayout ();
	
}
//*************************************************************************************
BOOL CBCGPMDIFrameWnd::OnShowControlBars (BOOL bShow)
{
	ASSERT_VALID (this);
	
	BOOL bResult = m_dockManager.ShowControlBars (bShow);
	AdjustDockingLayout ();
	
	return bResult;
}
//*****************************************************************************
//******************* dockmanager layer ***************************************
//*****************************************************************************
void CBCGPMDIFrameWnd::AddDockBar ()
{
	ASSERT_VALID (this);
}
//*****************************************************************************
BOOL CBCGPMDIFrameWnd::AddControlBar (CBCGPBaseControlBar* pControlBar, BOOL bTail)
{
	ASSERT_VALID (this);
	return m_dockManager.AddControlBar (pControlBar, bTail);
}
//*****************************************************************************
BOOL CBCGPMDIFrameWnd::InsertControlBar (CBCGPBaseControlBar* pControlBar, 
									  CBCGPBaseControlBar* pTarget, BOOL bAfter)
{
	ASSERT_VALID (this);
	return m_dockManager.InsertControlBar (pControlBar, pTarget, bAfter);
}
//*****************************************************************************
void CBCGPMDIFrameWnd::RemoveControlBarFromDockManager (CBCGPBaseControlBar* pControlBar, BOOL bDestroy,
										 BOOL bAdjustLayout, BOOL bAutoHide)
{
	ASSERT_VALID (this);
	m_dockManager.RemoveControlBarFromDockManager (pControlBar, bDestroy, bAdjustLayout, bAutoHide);
}
//*****************************************************************************
void CBCGPMDIFrameWnd::DockControlBar (CBCGPBaseControlBar* pBar, UINT nDockBarID, 
									LPCRECT lpRect)
{
	ASSERT_VALID (this);
	m_dockManager.DockControlBar (pBar, nDockBarID, lpRect);
}
//*****************************************************************************
CBCGPBaseControlBar* CBCGPMDIFrameWnd::GetControlBar (UINT nID)
{
	ASSERT_VALID (this);
	
	CBCGPBaseControlBar* pBar = m_dockManager.FindBarByID (nID, TRUE);

	return pBar;
}
//*****************************************************************************
void CBCGPMDIFrameWnd::ShowControlBar (CBCGPBaseControlBar* pBar, BOOL bShow, 
									   BOOL bDelay, BOOL bActivate)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pBar);

	pBar->ShowControlBar (bShow, bDelay, bActivate);
}
//*****************************************************************************
CBCGPBaseControlBar* CBCGPMDIFrameWnd::ControlBarFromPoint (CPoint point, 
							int nSensitivity, bool bExactBar, 
							CRuntimeClass* pRTCBarType) const
{
	ASSERT_VALID (this);
	return m_dockManager.ControlBarFromPoint (point, nSensitivity, bExactBar, 
												pRTCBarType);
}
//*****************************************************************************
CBCGPBaseControlBar* CBCGPMDIFrameWnd::ControlBarFromPoint (CPoint point, 
								int nSensitivity, DWORD& dwAlignment, 
								CRuntimeClass* pRTCBarType) const
{
	ASSERT_VALID (this);
	return m_dockManager.ControlBarFromPoint (point, nSensitivity, dwAlignment, 
												pRTCBarType);
}
//*****************************************************************************
BOOL CBCGPMDIFrameWnd::IsPointNearDockBar (CPoint point, DWORD& dwBarAlignment, 
										   BOOL& bOuterEdge) const
{
	ASSERT_VALID (this);
	return m_dockManager.IsPointNearDockBar (point, dwBarAlignment, bOuterEdge);
}
//*****************************************************************************
void CBCGPMDIFrameWnd::AdjustDockingLayout (HDWP hdwp)
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
void CBCGPMDIFrameWnd::AdjustClientArea ()
{
	
	CRect rectClientAreaBounds = m_dockManager.GetClientAreaBounds ();

	rectClientAreaBounds.left += m_rectBorder.left;
	rectClientAreaBounds.top  += m_rectBorder.top;
	rectClientAreaBounds.right -= m_rectBorder.right;
	rectClientAreaBounds.bottom -= m_rectBorder.bottom;
	
	m_wndClientArea.CalcWindowRect (rectClientAreaBounds, 0);
}
//*****************************************************************************
BOOL CBCGPMDIFrameWnd::OnMoveMiniFrame	(CWnd* pFrame)
{
	ASSERT_VALID (this);
	return m_dockManager.OnMoveMiniFrame (pFrame);
}
//****************************************************************************************
BOOL CBCGPMDIFrameWnd::EnableDocking (DWORD dwDockStyle)
{
	return m_dockManager.EnableDocking (dwDockStyle);
}
//****************************************************************************************
BOOL CBCGPMDIFrameWnd::EnableAutoHideBars (DWORD dwDockStyle)
{
	return m_dockManager.EnableAutoHideBars (dwDockStyle);
}
//****************************************************************************************
void CBCGPMDIFrameWnd::RecalcLayout (BOOL bNotify)
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
				CMDIFrameWnd::RecalcLayout (bNotify);
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
BOOL CBCGPMDIFrameWnd::NegotiateBorderSpace( UINT nBorderCmd, LPRECT lpRectBorder )
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
//****************************************************************************************
int CBCGPMDIFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_dockManager.Create (this);	

	return 0;
}
//****************************************************************************************
void CBCGPMDIFrameWnd::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CMDIFrameWnd::OnSizing(fwSide, pRect);
	
	CRect rect;
	GetWindowRect (rect);

	if (rect.Size () != CRect (pRect).Size ())
	{
		AdjustDockingLayout ();	
	}
}
//****************************************************************************************
LRESULT CBCGPMDIFrameWnd::OnExitSizeMove (WPARAM, LPARAM)
{
	m_dockManager.FixupVirtualRects ();
	return 0;
}
//*************************************************************************************
void CBCGPMDIFrameWnd::OnUpdateControlBarMenu(CCmdUI* pCmdUI)
{
	CBCGPBaseControlBar* pBar = GetControlBar(pCmdUI->m_nID);
	if (pBar != NULL)
	{
		pCmdUI->SetCheck(pBar->IsWindowVisible ());
		return;
	}

	pCmdUI->ContinueRouting();
}
//*************************************************************************************
BOOL CBCGPMDIFrameWnd::OnBarCheck(UINT nID)
{
	ASSERT_VALID (this);

	CBCGPBaseControlBar* pBar = GetControlBar(nID);
	if (pBar != NULL)
	{
		ShowControlBar(pBar, !pBar->IsWindowVisible (), FALSE, FALSE);
		return TRUE;
	}

	return FALSE;
}

//********************************************************************************
LRESULT CBCGPMDIFrameWnd::OnIdleUpdateCmdUI(WPARAM, LPARAM)
{
	m_dockManager.SendMessageToMiniFrames (WM_IDLEUPDATECMDUI);
	return 0L;
}
//********************************************************************************
void CBCGPMDIFrameWnd::OnSize(UINT nType, int cx, int cy) 
{
	if (m_bClosing)
	{
		CMDIFrameWnd::OnSize(nType, cx, cy);
		return;
	}

	m_bIsMinimized = (nType == SIZE_MINIMIZED);

	m_dockManager.OnActivateFrame (!m_bIsMinimized);

	if (!m_bIsMinimized && nType != SIZE_MAXIMIZED && !m_bWasMaximized)
	{
		m_dockManager.m_bSizeFrame = TRUE;
		AdjustDockingLayout ();			
		CMDIFrameWnd::OnSize(nType, cx, cy);
		m_dockManager.m_bSizeFrame = FALSE;
		BOOL bParam = FALSE;
		SystemParametersInfo (SPI_GETDRAGFULLWINDOWS, 0, &bParam, 0);
		if (!bParam)
		{
			RecalcLayout ();
		}
		return;
	} 

	CMDIFrameWnd::OnSize(nType, cx, cy);

	if (nType == SIZE_MAXIMIZED || (nType == SIZE_RESTORED && m_bWasMaximized))
	{
		RecalcLayout ();
	}

	m_bWasMaximized = (nType == SIZE_MAXIMIZED);
}
//**************************************************************************************
void CBCGPMDIFrameWnd::OnWindowNew()
{
	CMDIChildWnd* pActiveChild = MDIGetActive();
	if (pActiveChild == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	ASSERT_VALID (pActiveChild);

	BOOL bIsZoomed = FALSE;

	if (pActiveChild->IsZoomed ())
	{
		pActiveChild->ShowWindow (SW_RESTORE);
		bIsZoomed = TRUE;
	}

	CMDIFrameWnd::OnWindowNew();

	if (bIsZoomed)
	{
		pActiveChild = MDIGetActive();
		if (pActiveChild != NULL)
		{
			pActiveChild->ShowWindow (SW_MAXIMIZE);
		}
	}
}
//**************************************************************************************
void CBCGPMDIFrameWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
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
