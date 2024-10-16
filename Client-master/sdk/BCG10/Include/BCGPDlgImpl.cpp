//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of BCGControlBar Library Professional Edition
// Copyright (C) 1998-2008 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPDlgImpl.cpp: implementation of the CBCGPDlgImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "bcgcbpro.h"

#ifndef _BCGSUITE_
#include "BCGPPopupMenu.h"
#include "BCGPToolbarMenuButton.h"
#else
#define FindDestBar	FindDestintationToolBar
#endif

#include "BCGPDialog.h"
#include "BCGPButton.h"
#include "BCGPPropertyPage.h"
#include "BCGPDlgImpl.h"
#include "BCGPSliderCtrl.h"
#include "BCGPProgressCtrl.h"
#include "BCGPGroup.h"
#include "BCGPStatic.h"
#include "BCGPEdit.h"
#include "BCGPComboBox.h"
#include "BCGPVisualManager.h"
#include "BCGPScrollBar.h"
#include "BCGPSpinButtonCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

HHOOK CBCGPDlgImpl::m_hookMouse = NULL;
CBCGPDlgImpl* CBCGPDlgImpl::m_pMenuDlgImpl = NULL;

UINT BCGM_ONSETCONTROLAERO = ::RegisterWindowMessage (_T("BCGM_ONSETCONTROLAERO"));
UINT BCGM_ONSETCONTROLVMMODE = ::RegisterWindowMessage (_T("BCGM_ONSETCONTROLVMMODE"));

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPDlgImpl::CBCGPDlgImpl(CWnd& dlg) :
	m_Dlg (dlg),
	m_bVisualManagerStyle (FALSE),
	m_bTransparentStaticCtrls (TRUE)
{
	m_AeroMargins.cxLeftWidth = 0;
	m_AeroMargins.cxRightWidth = 0;
	m_AeroMargins.cyTopHeight = 0;
	m_AeroMargins.cyBottomHeight = 0;
}

CBCGPDlgImpl::~CBCGPDlgImpl()
{

}
//*******************************************************************************************
BOOL CBCGPDlgImpl::ProcessMouseClick (POINT pt)
{
	if (!CBCGPToolBar::IsCustomizeMode () &&
		CBCGPPopupMenu::GetActiveMenu() != NULL &&
		::IsWindow (CBCGPPopupMenu::GetActiveMenu()->m_hWnd))
	{
		CBCGPPopupMenu::MENUAREA_TYPE clickArea = CBCGPPopupMenu::GetActiveMenu()->CheckArea (pt);

		if (clickArea == CBCGPPopupMenu::OUTSIDE)
		{
			// Click outside of menu

			//--------------------------------------------
			// Maybe secondary click on the parent button?
			//--------------------------------------------
			CBCGPToolbarMenuButton* pParentButton = 
				CBCGPPopupMenu::GetActiveMenu()->GetParentButton ();
			if (pParentButton != NULL)
			{
				CWnd* pWndParent = pParentButton->GetParentWnd ();
				if (pWndParent != NULL)
				{
					CBCGPPopupMenuBar* pWndParentPopupMenuBar = 
						DYNAMIC_DOWNCAST (CBCGPPopupMenuBar, pWndParent);

					CPoint ptClient = pt;
					pWndParent->ScreenToClient (&ptClient);

					if (pParentButton->Rect ().PtInRect (ptClient))
					{
						//-------------------------------------------------------
						// If user clicks second time on the parent button,
						// we should close an active menu on the toolbar/menubar
						// and leave it on the popup menu:
						//-------------------------------------------------------
						if (pWndParentPopupMenuBar == NULL &&
							!CBCGPPopupMenu::GetActiveMenu()->InCommand ())
						{
							//----------------------------------------
							// Toolbar/menu bar: close an active menu!
							//----------------------------------------
							CBCGPPopupMenu::GetActiveMenu()->SendMessage (WM_CLOSE);
						}

						return TRUE;
					}

					if (pWndParentPopupMenuBar != NULL)
					{
						pWndParentPopupMenuBar->CloseDelayedSubMenu ();
						
						CBCGPPopupMenu* pWndParentPopupMenu = 
							DYNAMIC_DOWNCAST (CBCGPPopupMenu, 
							pWndParentPopupMenuBar->GetParent ());

						if (pWndParentPopupMenu != NULL)
						{
							CBCGPPopupMenu::MENUAREA_TYPE clickAreaParent = 
								pWndParentPopupMenu->CheckArea (pt);

							switch (clickAreaParent)
							{
							case CBCGPPopupMenu::MENU:
							case CBCGPPopupMenu::TEAROFF_CAPTION:
							case CBCGPPopupMenu::LOGO:
								return FALSE;

							case CBCGPPopupMenu::SHADOW_RIGHT:
							case CBCGPPopupMenu::SHADOW_BOTTOM:
								pWndParentPopupMenu->SendMessage (WM_CLOSE);
								m_Dlg.SetFocus ();

								return TRUE;
							}
						}
					}
				}
			}

			if (!CBCGPPopupMenu::GetActiveMenu()->InCommand ())
			{
				CBCGPPopupMenu::GetActiveMenu()->SendMessage (WM_CLOSE);

				CWnd* pWndFocus = CWnd::GetFocus ();
				if (pWndFocus != NULL && pWndFocus->IsKindOf (RUNTIME_CLASS (CBCGPToolBar)))
				{
					m_Dlg.SetFocus ();
				}

				if (clickArea != CBCGPPopupMenu::OUTSIDE)	// Click on shadow
				{
					return TRUE;
				}
			}
		}
		else if (clickArea == CBCGPPopupMenu::SHADOW_RIGHT ||
				clickArea == CBCGPPopupMenu::SHADOW_BOTTOM)
		{
			CBCGPPopupMenu::GetActiveMenu()->SendMessage (WM_CLOSE);
			m_Dlg.SetFocus ();

			return TRUE;
		}
	}

	return FALSE;
}
//*******************************************************************************************
BOOL CBCGPDlgImpl::ProcessMouseMove (POINT pt)
{
	if (!CBCGPToolBar::IsCustomizeMode () &&
		CBCGPPopupMenu::GetActiveMenu() != NULL)
	{
		CRect rectMenu;
		CBCGPPopupMenu::GetActiveMenu()->GetWindowRect (rectMenu);

		if (rectMenu.PtInRect (pt) ||
			CBCGPPopupMenu::GetActiveMenu()->GetMenuBar ()->FindDestBar (pt) != NULL)
		{
			return FALSE;	// Default processing
		}

		return TRUE;		// Active menu "capturing"
	}

	return FALSE;	// Default processing
}
//**************************************************************************************
BOOL CBCGPDlgImpl::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case WM_SYSKEYDOWN:
	case WM_CONTEXTMENU:
		if (CBCGPPopupMenu::GetActiveMenu() != NULL &&
			::IsWindow (CBCGPPopupMenu::GetActiveMenu()->m_hWnd) &&
			pMsg->wParam == VK_MENU)
		{
			CBCGPPopupMenu::GetActiveMenu()->SendMessage (WM_CLOSE);
			return TRUE;
		}
		break;

	case WM_SYSKEYUP:
		if (CBCGPPopupMenu::GetActiveMenu() != NULL &&
			::IsWindow (CBCGPPopupMenu::GetActiveMenu()->m_hWnd))
		{
			return TRUE;	// To prevent system menu opening
		}
		break;

	case WM_KEYDOWN:
		//-----------------------------------------
		// Pass keyboard action to the active menu:
		//-----------------------------------------
		if (CBCGPPopupMenu::GetActiveMenu() != NULL &&
			::IsWindow (CBCGPPopupMenu::GetActiveMenu()->m_hWnd))
		{
			CBCGPPopupMenu::GetActiveMenu()->SendMessage (WM_KEYDOWN, (int) pMsg->wParam);
			return TRUE;
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

			if (pWnd != NULL && ::IsWindow (pMsg->hwnd))
			{
				pWnd->ClientToScreen (&pt);
			}

			if (ProcessMouseClick (pt))
			{
				return TRUE;
			}

			if (!::IsWindow (pMsg->hwnd))
			{
				return TRUE;
			}
		}
		break;

	case WM_NCLBUTTONDOWN:
	case WM_NCLBUTTONUP:
	case WM_NCRBUTTONDOWN:
	case WM_NCRBUTTONUP:
	case WM_NCMBUTTONDOWN:
	case WM_NCMBUTTONUP:
		if (ProcessMouseClick (CPoint (BCG_GET_X_LPARAM(pMsg->lParam), BCG_GET_Y_LPARAM(pMsg->lParam))))
		{
			return TRUE;
		}
		break;

	case WM_MOUSEWHEEL:
		if (CBCGPPopupMenu::GetActiveMenu() != NULL &&
			::IsWindow (CBCGPPopupMenu::GetActiveMenu()->m_hWnd) &&
			CBCGPPopupMenu::GetActiveMenu()->IsScrollable ())
		{
			CBCGPPopupMenu::GetActiveMenu()->SendMessage (WM_MOUSEWHEEL,
				pMsg->wParam, pMsg->lParam);

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

			if (ProcessMouseMove (pt))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}
//**********************************************************************************
LRESULT CALLBACK CBCGPDlgImpl::BCGDlgMouseProc (int nCode, WPARAM wParam, LPARAM lParam)
{
	MOUSEHOOKSTRUCT* lpMS = (MOUSEHOOKSTRUCT*) lParam;
	ASSERT (lpMS != NULL);

	if (m_pMenuDlgImpl != NULL)
	{
		switch (wParam)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_NCMBUTTONDOWN:
			{
				CPoint ptCursor;
				::GetCursorPos (&ptCursor);

				CRect rectWindow;
				m_pMenuDlgImpl->m_Dlg.GetWindowRect (rectWindow);

				if (!rectWindow.PtInRect (ptCursor))
				{
					m_pMenuDlgImpl->ProcessMouseClick (ptCursor);
				}
			}
		}
	}

	return CallNextHookEx (m_hookMouse, nCode, wParam, lParam);
}
//****************************************************************************************
void CBCGPDlgImpl::SetActiveMenu (CBCGPPopupMenu* pMenu)
{
#ifndef _BCGSUITE_
	CBCGPPopupMenu::m_pActivePopupMenu = pMenu;
#else
	class CBCGPPopupMenuDummy : public CBCGPPopupMenu
	{
		friend class CBCGPDlgImpl;
	};

	CBCGPPopupMenuDummy::m_pActivePopupMenu = pMenu;

#endif

	if (pMenu != NULL)
	{
		if (m_hookMouse == NULL)
		{
			m_hookMouse = ::SetWindowsHookEx (WH_MOUSE, BCGDlgMouseProc, 
				0, GetCurrentThreadId ());
		}

		m_pMenuDlgImpl = this;
	}
	else 
	{
		if (m_hookMouse != NULL)
		{
			::UnhookWindowsHookEx (m_hookMouse);
			m_hookMouse = NULL;
		}

		m_pMenuDlgImpl = NULL;
	}

}
//****************************************************************************************
void CBCGPDlgImpl::OnDestroy ()
{
	for (int i = 0; i < m_arSubclassedCtrls.GetSize (); i++)
	{
		delete m_arSubclassedCtrls [i];
	}

	m_arSubclassedCtrls.RemoveAll ();
	if (m_pMenuDlgImpl != NULL &&
		m_pMenuDlgImpl->m_Dlg.GetSafeHwnd () == m_Dlg.GetSafeHwnd ())
	{
		m_pMenuDlgImpl = NULL;
	}
}
//****************************************************************************************
BOOL CBCGPDlgImpl::OnCommand (WPARAM wParam, LPARAM /*lParam*/)
{
	if (HIWORD (wParam) == 1)
	{
		UINT uiCmd = LOWORD (wParam);

		CBCGPToolBar::AddCommandUsage (uiCmd);

		//---------------------------
		// Simmulate ESC keystroke...
		//---------------------------
		if (CBCGPPopupMenu::GetActiveMenu() != NULL &&
			::IsWindow (CBCGPPopupMenu::GetActiveMenu()->m_hWnd))
		{
			CBCGPPopupMenu::GetActiveMenu()->SendMessage (WM_KEYDOWN, VK_ESCAPE);
			return TRUE;
		}

		if (g_pUserToolsManager != NULL &&
			g_pUserToolsManager->InvokeTool (uiCmd))
		{
			return TRUE;
		}
	}

	return FALSE;
}
//***************************************************************************************
void CBCGPDlgImpl::OnNcActivate (BOOL& bActive)
{
	//----------------------------------------
	// Stay active if WF_STAYACTIVE bit is on:
	//----------------------------------------
	if (m_Dlg.m_nFlags & WF_STAYACTIVE)
	{
		bActive = TRUE;
	}

	//--------------------------------------------------
	// But do not stay active if the window is disabled:
	//--------------------------------------------------
	if (!m_Dlg.IsWindowEnabled ())
	{
		bActive = FALSE;
	}
}
//****************************************************************************************
void CBCGPDlgImpl::OnActivate(UINT nState, CWnd* pWndOther)
{
	m_Dlg.m_nFlags &= ~WF_STAYACTIVE;

	//--------------------------------------------------
	// Determine if this window should be active or not:
	//--------------------------------------------------
	CWnd* pWndActive = (nState == WA_INACTIVE) ? pWndOther : &m_Dlg;
	if (pWndActive != NULL)
	{
		BOOL bStayActive = (pWndActive->GetSafeHwnd () == m_Dlg.GetSafeHwnd () ||
			pWndActive->SendMessage (WM_FLOATSTATUS, FS_SYNCACTIVE));

		if (bStayActive)
		{
			m_Dlg.m_nFlags |= WF_STAYACTIVE;
		}
	}
	else 
	{
		//------------------------------------------
		// Force painting on our non-client area....
		//------------------------------------------
		m_Dlg.SendMessage (WM_NCPAINT, 1);
	}
}
//*************************************************************************************
void CBCGPDlgImpl::EnableVisualManagerStyle (BOOL bEnable)
{
	m_bVisualManagerStyle = bEnable;

	if (m_Dlg.GetSafeHwnd () == NULL)
	{
		return;
	}

	CWnd* pWndChild = m_Dlg.GetWindow (GW_CHILD);

	while (pWndChild != NULL)
	{
		ASSERT_VALID (pWndChild);

		CBCGPButton* pButton = DYNAMIC_DOWNCAST(CBCGPButton, pWndChild);
		if (pButton != NULL)
		{
			ASSERT_VALID (pButton);
			pButton->m_bVisualManagerStyle = m_bVisualManagerStyle;
		}

		if (m_bVisualManagerStyle &&
			CWnd::FromHandlePermanent (pWndChild->GetSafeHwnd ()) == NULL)
		{
			#define MAX_CLASS_NAME		255
			#define STATIC_CLASS		_T("Static")
			#define BUTTON_CLASS		_T("Button")
			#define EDIT_CLASS			_T("Edit")
			#define	COMBOBOX_CLASS		_T("ComboBox")
			#define SCROLLBAR_CLASS		_T("ScrollBar")

			TCHAR lpszClassName [MAX_CLASS_NAME + 1];

			::GetClassName (pWndChild->GetSafeHwnd (), lpszClassName, MAX_CLASS_NAME);
			CString strClass = lpszClassName;

			CWnd* pWndSubclassedCtrl = NULL;

			if (strClass == STATIC_CLASS)
			{
				pWndSubclassedCtrl = new CBCGPStatic;
			}
			else if (strClass == BUTTON_CLASS)
			{
				if ((pWndChild->GetStyle () & 0xF) == BS_GROUPBOX)
				{
					pWndSubclassedCtrl = new CBCGPGroup;
				}
				else
				{
					pWndSubclassedCtrl = new CBCGPButton;
				}
			}
			else if (strClass == PROGRESS_CLASS)
			{
				pWndSubclassedCtrl = new CBCGPProgressCtrl;
			}
			else if (strClass == TRACKBAR_CLASS)
			{
				pWndSubclassedCtrl = new CBCGPSliderCtrl;
			}
			else if (strClass == EDIT_CLASS)
			{
				pWndSubclassedCtrl = new CBCGPEdit;
			}
			else if (strClass == COMBOBOX_CLASS)
			{
				pWndSubclassedCtrl = new CBCGPComboBox;
			}
			else if (strClass == SCROLLBAR_CLASS)
			{
				pWndSubclassedCtrl = new CBCGPScrollBar;
			}
			else if (strClass == UPDOWN_CLASS)
			{
				pWndSubclassedCtrl = new CBCGPSpinButtonCtrl;
			}

			if (pWndSubclassedCtrl != NULL)
			{
				m_arSubclassedCtrls.Add (pWndSubclassedCtrl);
				pWndSubclassedCtrl->SubclassWindow (pWndChild->GetSafeHwnd ());
			}
		}

		pWndChild->SendMessage (BCGM_ONSETCONTROLVMMODE, (WPARAM) bEnable);
		pWndChild = pWndChild->GetNextWindow ();
	}

	if (m_Dlg.IsWindowVisible ())
	{
		m_Dlg.RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
	}
}
//********************************************************************************************************
BOOL CBCGPDlgImpl::EnableAero (BCGPMARGINS& margins)
{
	m_AeroMargins = margins;

	if (HasAeroMargins () && !m_bVisualManagerStyle)
	{
		EnableVisualManagerStyle (TRUE);
	}

	if (m_Dlg.GetSafeHwnd () == NULL)
	{
		return TRUE;
	}

	BOOL bRes = globalData.DwmExtendFrameIntoClientArea (m_Dlg.GetSafeHwnd (), &m_AeroMargins);
	BOOL bIsAeroEnabled = globalData.DwmIsCompositionEnabled ();

	CRect rectClient;
	m_Dlg.GetClientRect (rectClient);

	CWnd* pWndChild = m_Dlg.GetWindow (GW_CHILD);

	while (pWndChild != NULL)
	{
		ASSERT_VALID (pWndChild);

		BOOL bGlass = FALSE;

		if (bIsAeroEnabled && HasAeroMargins ())
		{
			CRect rectChild;
			pWndChild->GetWindowRect (&rectChild);
			m_Dlg.ScreenToClient (&rectChild);

			CRect rectInter;

			if (m_AeroMargins.cxLeftWidth != 0)
			{
				CRect rectAero = rectClient;
				rectAero.right = rectAero.left + m_AeroMargins.cxLeftWidth;

				if (rectInter.IntersectRect (rectAero, rectChild))
				{
					bGlass = TRUE;
				}
			}

			if (!bGlass && m_AeroMargins.cxRightWidth != 0)
			{
				CRect rectAero = rectClient;
				rectAero.left = rectAero.right - m_AeroMargins.cxRightWidth;

				if (rectInter.IntersectRect (rectAero, rectChild))
				{
					bGlass = TRUE;
				}
			}

			if (!bGlass && m_AeroMargins.cyTopHeight != 0)
			{
				CRect rectAero = rectClient;
				rectAero.bottom = rectAero.top + m_AeroMargins.cyTopHeight;

				if (rectInter.IntersectRect (rectAero, rectChild))
				{
					bGlass = TRUE;
				}
			}

			if (!bGlass && m_AeroMargins.cyBottomHeight != 0)
			{
				CRect rectAero = rectClient;
				rectAero.top = rectAero.bottom - m_AeroMargins.cyBottomHeight;

				if (rectInter.IntersectRect (rectAero, rectChild))
				{
					bGlass = TRUE;
				}
			}
		}

		pWndChild->SendMessage (BCGM_ONSETCONTROLAERO, (WPARAM) bGlass);
		pWndChild = pWndChild->GetNextWindow ();
	}

	if (m_Dlg.IsWindowVisible ())
	{
		m_Dlg.RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
	}

	return bRes;
}
//********************************************************************************************************
void CBCGPDlgImpl::GetAeroMargins (BCGPMARGINS& margins) const
{
	margins = m_AeroMargins;
}
//********************************************************************************************************
BOOL CBCGPDlgImpl::HasAeroMargins () const
{
	return 	m_AeroMargins.cxLeftWidth != 0 ||
			m_AeroMargins.cxRightWidth != 0 ||
			m_AeroMargins.cyTopHeight != 0 ||
			m_AeroMargins.cyBottomHeight != 0;
}
//********************************************************************************************************
void CBCGPDlgImpl::ClearAeroAreas (CDC* pDC)
{
	if (!HasAeroMargins () || m_Dlg.GetSafeHwnd () == NULL ||
		!globalData.DwmIsCompositionEnabled ())
	{
		return;
	}

	CRect rectClient;
	m_Dlg.GetClientRect (rectClient);

	if (m_AeroMargins.cxLeftWidth != 0)
	{
		CRect rectAero = rectClient;
		rectAero.right = rectAero.left + m_AeroMargins.cxLeftWidth;

		pDC->FillSolidRect (rectAero, RGB (0, 0, 0));
	}

	if (m_AeroMargins.cxRightWidth != 0)
	{
		CRect rectAero = rectClient;
		rectAero.left = rectAero.right - m_AeroMargins.cxRightWidth;

		pDC->FillSolidRect (rectAero, RGB (0, 0, 0));
	}

	if (m_AeroMargins.cyTopHeight != 0)
	{
		CRect rectAero = rectClient;
		rectAero.bottom = rectAero.top + m_AeroMargins.cyTopHeight;

		pDC->FillSolidRect (rectAero, RGB (0, 0, 0));
	}

	if (m_AeroMargins.cyBottomHeight != 0)
	{
		CRect rectAero = rectClient;
		rectAero.top = rectAero.bottom - m_AeroMargins.cyBottomHeight;

		pDC->FillSolidRect (rectAero, RGB (0, 0, 0));
	}
}
//********************************************************************************************************
void CBCGPDlgImpl::OnDWMCompositionChanged ()
{
	if (!HasAeroMargins () || m_Dlg.GetSafeHwnd () == NULL)
	{
		return;
	}

	EnableAero (m_AeroMargins);
	m_Dlg.RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
}
//********************************************************************************************************
HBRUSH CBCGPDlgImpl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	#define MAX_CLASS_NAME	255
	#define STATIC_CLASS	_T("Static")
	#define BUTTON_CLASS	_T("Button")

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		TCHAR lpszClassName [MAX_CLASS_NAME + 1];

		::GetClassName (pWnd->GetSafeHwnd (), lpszClassName, MAX_CLASS_NAME);
		CString strClass = lpszClassName;

		if (strClass == STATIC_CLASS)
		{
			pDC->SetBkMode(TRANSPARENT);

			if (m_bVisualManagerStyle)
			{
				pDC->SetTextColor (globalData.clrBarText);
			}

			if (m_bTransparentStaticCtrls && (pWnd->GetStyle () & SS_ICON) == 0)
			{
				return (HBRUSH) ::GetStockObject (HOLLOW_BRUSH);
			}

			return (HBRUSH) CBCGPVisualManager::GetInstance ()->GetDlgBackBrush (&m_Dlg).GetSafeHandle ();
		}

		if (strClass == BUTTON_CLASS)
		{
			DWORD dwStyle = pWnd->GetStyle ();

			if (dwStyle & BS_GROUPBOX)
			{
				if (m_bVisualManagerStyle)
				{
					pDC->SetTextColor (globalData.clrBarText);
					pDC->SetBkMode(TRANSPARENT);
					return (HBRUSH) CBCGPVisualManager::GetInstance ()->GetDlgBackBrush (&m_Dlg).GetSafeHandle ();
				}
			}

			if ((dwStyle & BS_CHECKBOX) == 0)
			{
				pDC->SetBkMode(TRANSPARENT);
			}

			return (HBRUSH) ::GetStockObject (HOLLOW_BRUSH);
		}
	}

	return NULL;
}
