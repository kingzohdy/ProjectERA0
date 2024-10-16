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

// BCGPMainClientAreaWnd.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPMainClientAreaWnd.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPMDIChildWnd.h"
#include "BCGPMenuBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CBCGPMainClientAreaWnd, CWnd)

#define UM_UPDATE_TABS	(WM_USER + 101)

/////////////////////////////////////////////////////////////////////////////
// CBCGPMainClientAreaWnd

CBCGPMainClientAreaWnd::CBCGPMainClientAreaWnd()
{
	m_bTabIsVisible = FALSE;
	m_bTabIsEnabled = FALSE;
	m_bTabIcons = TRUE;
	m_bTabCloseButton = FALSE;
	m_tabLocation = CBCGPTabWnd::LOCATION_BOTTOM;
}
//*************************************************************************************
CBCGPMainClientAreaWnd::~CBCGPMainClientAreaWnd()
{
}
//*************************************************************************************
void CBCGPMainClientAreaWnd::EnableMDITabs (BOOL bEnable/* = TRUE*/,
		BOOL bIcons/* = TRUE*/,
		CBCGPTabWnd::Location tabLocation /* = CBCGPTabWnd::LOCATION_BOTTOM*/,
		BOOL bCloseButton/* = FALSE*/,
		CBCGPTabWnd::Style style/* = CBCGPTabWnd::STYLE_3D_SCROLLED*/)
{
	m_bTabIsEnabled = bEnable;
	m_bTabIsVisible = bEnable;
	m_bTabIcons = bIcons;
	m_wndTab.m_location = m_tabLocation = tabLocation;
	m_wndTab.m_bCloseBtn = m_bTabCloseButton = bCloseButton;
	m_wndTab.m_bActivateLastVisibleTab = TRUE;

	m_wndTab.ModifyTabStyle (style);
	
	if (bEnable)
	{
		UpdateTabs ();
	}

	m_wndTab.RecalcLayout ();

	if (GetSafeHwnd () != NULL && GetParentFrame () != NULL)
	{
		GetParentFrame ()->RecalcLayout ();

		UINT uiRedrawFlags =	RDW_ALLCHILDREN | RDW_FRAME | RDW_INVALIDATE | 
								RDW_UPDATENOW | RDW_ERASE;

		if (m_wndTab.GetSafeHwnd () != NULL)
		{
			m_wndTab.RedrawWindow (NULL, NULL, uiRedrawFlags);
		}

		RedrawWindow (NULL, NULL, uiRedrawFlags);
	}
}

BEGIN_MESSAGE_MAP(CBCGPMainClientAreaWnd, CWnd)
	//{{AFX_MSG_MAP(CBCGPMainClientAreaWnd)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MDISETMENU,OnSetMenu)
	ON_MESSAGE(WM_MDIREFRESHMENU, OnMDIRefreshMenu)
	ON_MESSAGE(WM_MDIDESTROY, OnMDIDestroy)
	ON_MESSAGE(UM_UPDATE_TABS, OnUpdateTabs)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCGPMainClientAreaWnd message handlers

afx_msg LRESULT CBCGPMainClientAreaWnd::OnSetMenu (WPARAM wp, LPARAM lp)
{
	CBCGPMDIFrameWnd* pMainFrame = DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, GetParentFrame ());
	if (pMainFrame != NULL && ::IsWindow (pMainFrame->GetSafeHwnd ()))
	{
		if (pMainFrame->OnSetMenu ((HMENU) wp))
		{
			wp = NULL;
		}
	}
	else
	{
		wp = NULL;
	}

	return DefWindowProc (WM_MDISETMENU, wp, lp);
}
//*********************************************************************************
LRESULT CBCGPMainClientAreaWnd::OnMDIRefreshMenu (WPARAM /*wp*/, LPARAM /*lp*/)
{
	LRESULT lRes = Default ();

	CBCGPMDIFrameWnd* pMainFrame = DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, GetParentFrame ());
	if (pMainFrame != NULL && pMainFrame->GetMenuBar () != NULL)
	{
		pMainFrame->m_hmenuWindow = 
			pMainFrame->GetWindowMenuPopup (pMainFrame->GetMenuBar ()->GetMenu ());
	}

	return lRes;
}
//*********************************************************************************
BOOL CBCGPMainClientAreaWnd::OnEraseBkgnd(CDC* pDC) 
{
	CBCGPMDIFrameWnd* pMainFrame = DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, GetParentFrame ());
	if (pMainFrame != NULL && pMainFrame->OnEraseMDIClientBackground (pDC))
	{
		return TRUE;
	}

	return CWnd::OnEraseBkgnd(pDC);
}
//***********************************************************************************
LRESULT CBCGPMainClientAreaWnd::OnMDIDestroy(WPARAM wParam, LPARAM)
{
	int nTabsHeight = m_wndTab.GetTabsHeight ();

	CBCGPMDIFrameWnd* pParentFrame = DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, GetParentFrame ());
	
	if (!pParentFrame->m_bClosing)
	{
		SetRedraw (FALSE);
	}

	int iTab = m_wndTab.GetTabFromHwnd ((HWND)wParam);
	if (iTab >= 0)
	{
		CBCGPMDIChildWnd* pMDIChild = DYNAMIC_DOWNCAST(CBCGPMDIChildWnd, m_wndTab.GetTabWnd (iTab));
		if (pMDIChild != NULL)
		{
			pMDIChild->m_bToBeDestroyed = TRUE;
		}

		m_wndTab.RemoveTab (iTab);
	}

	LRESULT lRes = Default ();

	if (nTabsHeight != m_wndTab.GetTabsHeight () && pParentFrame != NULL)
	{
		pParentFrame->RecalcLayout ();
	}

	if (!pParentFrame->m_bClosing)
	{
		SetRedraw (TRUE);
		GetParent ()->RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
	}

	return lRes;
}
//***********************************************************************************
void CBCGPMainClientAreaWnd::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	if (m_wndTab.GetSafeHwnd () != NULL)
	{

		BOOL bRedraw = FALSE;

		if (m_bTabIsVisible)
		{
			CRect rectOld;
			m_wndTab.GetWindowRect (rectOld);

			m_wndTab.SetWindowPos (NULL, 
				lpClientRect->left, lpClientRect->top,
				lpClientRect->right - lpClientRect->left,
				lpClientRect->bottom - lpClientRect->top,
				SWP_NOZORDER | SWP_NOACTIVATE);

			CRect rectTabClient;
			m_wndTab.GetClientRect (rectTabClient);

			CRect rectTabWnd;
			m_wndTab.GetWndArea (rectTabWnd);

			lpClientRect->top += (rectTabWnd.top - rectTabClient.top);
			lpClientRect->bottom += (rectTabWnd.bottom - rectTabClient.bottom);
			lpClientRect->left += (rectTabWnd.left - rectTabClient.left);
			lpClientRect->right += (rectTabWnd.right - rectTabClient.right);

			m_wndTab.ShowWindow (SW_SHOWNA);

			CRect rectNew;
			m_wndTab.GetWindowRect (rectNew);

			bRedraw = (rectOld != rectNew);
		}
		else
		{
			m_wndTab.ShowWindow (SW_HIDE);
		}

		CRect rectOld;
		GetWindowRect (rectOld);
		int nHeightDelta = lpClientRect->bottom - lpClientRect->top - 
							rectOld.Height ();

		SetWindowPos (NULL, 
				lpClientRect->left, lpClientRect->top,
				lpClientRect->right - lpClientRect->left,
				lpClientRect->bottom - lpClientRect->top,
				SWP_NOZORDER | SWP_NOACTIVATE);

		if (!m_bTabIsVisible)
		{
			CRect rectClient;
			GetClientRect (&rectClient);
			CBCGPMDIFrameWnd* pFrame = (CBCGPMDIFrameWnd*) GetParentFrame ();
			ASSERT_VALID (pFrame);
			HWND hwndT = ::GetWindow(pFrame->m_hWndMDIClient, GW_CHILD);
			
			while (hwndT != NULL)
			{
				DWORD dwStyle = ::GetWindowLong (hwndT, GWL_STYLE);
				if (dwStyle & WS_MAXIMIZE)
				{
					break; // nothing to move;
				}
				if (dwStyle & WS_MINIMIZE)
				{
					CRect rectWnd;
					::GetWindowRect (hwndT, rectWnd);
					ScreenToClient (&rectWnd);

					rectWnd.OffsetRect (0, nHeightDelta);

					if (rectWnd.top < rectClient.top)
					{
						rectWnd.top = rectClient.top;
					}

					::SetWindowPos (hwndT, NULL, 
									rectWnd.left, rectWnd.top, 0, 0, 
									SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
				}

				hwndT=::GetWindow(hwndT,GW_HWNDNEXT);
			}
		}

		if (bRedraw)
		{
			RedrawWindow (NULL, NULL, 
				RDW_ALLCHILDREN | RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW |
				RDW_ERASE | RDW_INTERNALPAINT);
		}
	}

	CWnd::CalcWindowRect(lpClientRect, nAdjustType);
}
//***********************************************************************************
void CBCGPMainClientAreaWnd::SetActiveTab (HWND hwnd)
{
	if (m_bTabIsVisible)
	{
		int iTab = m_wndTab.GetTabFromHwnd (hwnd);
		if (iTab >= 0)
		{
			m_wndTab.SetActiveTab (iTab);
		}
	}
}
//************************************************************************************
LRESULT CBCGPMainClientAreaWnd::OnUpdateTabs (WPARAM, LPARAM)
{
	UpdateTabs ();
	return 0;
}
//**************************************************************************************
void CBCGPMainClientAreaWnd::PreSubclassWindow() 
{
	CWnd::PreSubclassWindow();

	//-------------------------
	// Create MDI tabs control:
	//-------------------------
    if (!m_wndTab.Create (CBCGPTabWnd::STYLE_3D_SCROLLED, CRect (0, 0, 0, 0), 
		GetParentFrame (), (UINT)-1, m_tabLocation, m_bTabCloseButton))
	{
		TRACE(_T("CBCGPMainClientAreaWnd::OnCreate: can't create tabs window\n"));
		return;
	}

	m_wndTab.HideInactiveWindow (FALSE);
	m_wndTab.HideNoTabs ();
	m_wndTab.AutoSizeWindow (FALSE);
	m_wndTab.AutoDestroyWindow (FALSE);
	m_wndTab.SetFlatFrame ();
	m_wndTab.m_bTransparent = TRUE;
	m_wndTab.m_bTopEdge = TRUE;
	m_wndTab.SetDrawNoPrefix (TRUE, FALSE);
	m_wndTab.SetActiveTabBoldFont ();

	if (!m_bTabIsVisible)
	{
		m_wndTab.ShowWindow (SW_HIDE);
	}

	//------------------
	// Create tab icons:
	//------------------
	m_TabIcons.Create (
		globalData.m_sizeSmallIcon.cx, globalData.m_sizeSmallIcon.cy, 
		ILC_COLOR8 | ILC_MASK, 0, 1);
}
//*************************************************************************************
void CBCGPMainClientAreaWnd::UpdateTabs (BOOL bSetActiveTabVisible/* = FALSE*/)
{
	if (m_wndTab.GetSafeHwnd () == NULL || !m_bTabIsVisible)
	{
		return;
	}

	BOOL bRecalcLayout = FALSE;

	CWnd* pWndChild = GetWindow (GW_CHILD);
	while (pWndChild != NULL)
	{
		ASSERT_VALID (pWndChild);

		CBCGPMDIChildWnd* pMDIChild = DYNAMIC_DOWNCAST(CBCGPMDIChildWnd, pWndChild);

		BOOL bIsShowTab = TRUE;
		if (pMDIChild != NULL)
		{
			bIsShowTab = pMDIChild->CanShowOnMDITabs ();
		}

		//--------------
		// Get tab icon:
		//--------------
		int iIcon = -1;
		if (m_bTabIcons)
		{
			HICON hIcon = NULL;
			if (pMDIChild != NULL)
			{
				hIcon = pMDIChild->GetFrameIcon ();
			}
			else
			{
				if ((hIcon = pWndChild->GetIcon (FALSE)) == NULL)
				{
					hIcon = (HICON)(LONG_PTR) GetClassLongPtr (*pWndChild, GCLP_HICONSM);
				}
			}

			if (hIcon != NULL)
			{
				if (!m_mapIcons.Lookup (hIcon, iIcon))
				{
					iIcon = m_TabIcons.Add (hIcon);
					m_mapIcons.SetAt (hIcon, iIcon);

					if (m_TabIcons.GetImageCount () == 1)
					{
						m_wndTab.SetImageList (m_TabIcons.GetSafeHandle ());
					}
				}
			}
		}

		//--------------------------------
		// Get tab label (window caption):
		//--------------------------------
		CString strTabLabel;
		if (pMDIChild != NULL)
		{
			strTabLabel = pMDIChild->GetFrameText ();
		}
		else
		{
			pWndChild->GetWindowText (strTabLabel);
		}

		int iTabIndex = m_wndTab.GetTabFromHwnd (pWndChild->GetSafeHwnd ());
		if (iTabIndex >= 0)
		{
			//---------------------------------
			// Tab is already exist, update it:
			//---------------------------------
			if (pWndChild->GetStyle () & WS_VISIBLE)
			{
				CString strCurTabLabel;
				m_wndTab.GetTabLabel (iTabIndex, strCurTabLabel);

				if (strCurTabLabel != strTabLabel)
				{
					//-----------------------------
					// Text was changed, update it:
					//-----------------------------
					m_wndTab.SetTabLabel (iTabIndex, strTabLabel);
					bRecalcLayout = TRUE;
				}

				if (m_wndTab.GetTabIcon (iTabIndex) != (UINT) iIcon)
				{
					//-----------------------------
					// Icon was changed, update it:
					//-----------------------------
					m_wndTab.SetTabIcon (iTabIndex, iIcon);
					bRecalcLayout = TRUE;
				}
			}
			else
			{
				//----------------------------------
				// Window is hidden now, remove tab:
				//----------------------------------
				m_wndTab.RemoveTab (iTabIndex);
				bRecalcLayout = TRUE;
			}
		}
		else if ((pMDIChild == NULL || !pMDIChild->m_bToBeDestroyed) && bIsShowTab)
		{
			//----------------------
			// New item, add it now:
			//----------------------
			m_wndTab.AddTab (pWndChild, strTabLabel, iIcon);
			m_wndTab.SetActiveTab (m_wndTab.GetTabsNum () - 1);

			bRecalcLayout = TRUE;
		}

		pWndChild = pWndChild->GetNextWindow ();
    }

	if (bRecalcLayout && GetParentFrame () != NULL)
	{
		GetParentFrame ()->RecalcLayout ();
	}

	if (bSetActiveTabVisible)
	{
		m_wndTab.EnsureVisible (m_wndTab.GetActiveTab ());
	}
}

