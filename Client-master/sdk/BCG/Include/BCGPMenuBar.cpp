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

// BCGPMenuBar.cpp : implementation file
//

#include "stdafx.h"

#include "BCGGlobals.h"
#include "BCGPMenuBar.h"
#include "BCGPToolbarButton.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPToolbarSystemMenuButton.h"
#include "BCGPToolbarMenuButtonsButton.h"
#include "BCGPToolbarComboBoxButton.h"
#include "BCGPPopupMenu.h"
#include "BCGPTearOffManager.h"
#include "MenuHash.h"
#include "bcgprores.h"
#include "BCGPLocalResource.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPFrameWnd.h"
#include "BCGPOleIPFrameWnd.h"
#include "BCGPMultiDocTemplate.h"
#include "RegPath.h"
#include "BCGPMenuPage.h"
#include "BCGPWorkspace.h"
#include "BCGPDockBar.h"
#include "BCGPDockBarRow.h"
#include "BCGPMiniFrameWnd.h"
#include "CustomizeButton.h"

#ifdef _DEBUG 
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CHelpComboBoxButton : public CBCGPToolbarComboBoxButton
{
	DECLARE_SERIAL (CHelpComboBoxButton)

public:
	CHelpComboBoxButton (UINT uiId = 0, int iWidth = 0, LPCTSTR lpszPromt = NULL) :
		CBCGPToolbarComboBoxButton (uiId, -1, CBS_DROPDOWN, iWidth)
	{
		m_strPrompt = lpszPromt == NULL ? _T("") : lpszPromt;
	}

	virtual BOOL IsEditable () const
	{
		return FALSE;
	}

	virtual CString GetPrompt () const
	{	
		return m_strPrompt;
	}

	virtual void CopyFrom (const CBCGPToolbarButton& s)
	{
		CBCGPToolbarComboBoxButton::CopyFrom (s);
		const CHelpComboBoxButton& src = (const CHelpComboBoxButton&) s;

		m_strPrompt = src.m_strPrompt;
	}

	virtual void Serialize (CArchive& ar)
	{
		CBCGPToolbarComboBoxButton::Serialize (ar);

		if (ar.IsLoading ())
		{
			ar >> m_strPrompt;
		}
		else
		{
			ar << m_strPrompt;
		}
	}

protected:
	CString m_strPrompt;
};

IMPLEMENT_SERIAL(CHelpComboBoxButton, CBCGPToolbarComboBoxButton, VERSIONABLE_SCHEMA | 1)

IMPLEMENT_SERIAL(CBCGPMenuBar, CBCGPToolBar, VERSIONABLE_SCHEMA | 1)

BOOL CBCGPMenuBar::m_bShowAllCommands = FALSE;
BOOL CBCGPMenuBar::m_bRecentlyUsedMenus = TRUE;
BOOL CBCGPMenuBar::m_bShowAllMenusDelay = TRUE;
BOOL CBCGPMenuBar::m_bMenuShadows = TRUE;
BOOL CBCGPMenuBar::m_bHighlightDisabledItems = FALSE;

static const UINT uiShowAllItemsTimerId = 1;
static const int iShowAllItemsTimerFreq = 5000;	// 5 sec

static const CString strMenuProfile = _T("BCGMenuBar");

/////////////////////////////////////////////////////////////////////////////
// CBCGPMenuBar

CBCGPMenuBar::CBCGPMenuBar()
{
	m_bMaximizeMode = FALSE;
	m_hMenu = NULL;
	m_hDefaultMenu = NULL;
	m_hSysMenu = NULL;
	m_hSysIcon = NULL;
	m_uiDefMenuResId = 0;
	m_nSystemButtonsNum = 0;
	m_nSystemButtonsNumSaved = 0;
	m_bHaveButtons = FALSE;
	m_szSystemButton = CSize (0, 0);
	m_bAutoDocMenus = TRUE;
	m_pMenuPage = NULL;
    m_bForceDownArrows = FALSE;
	m_bExclusiveRow = TRUE;
	m_nHelpComboID = 0;
	m_nHelpComboWidth = 0;
}

CBCGPMenuBar::~CBCGPMenuBar()
{
	::DestroyMenu (m_hMenu);
}


BEGIN_MESSAGE_MAP(CBCGPMenuBar, CBCGPToolBar)
	//{{AFX_MSG_MAP(CBCGPMenuBar)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETTINGCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCGPMenuBar message handlers

void CBCGPMenuBar::CreateFromMenu (HMENU hMenu, BOOL bDefaultMenu, BOOL bForceUpdate)
{
	if (GetFocus () == this)
	{
		GetParentFrame()->SetFocus ();
	}

	if (m_hMenu != hMenu || IsCustomizeMode () || bForceUpdate)
	{
		if (g_pTearOffMenuManager != NULL && m_hMenu != NULL)
		{
			g_pTearOffMenuManager->Reset (m_hMenu);
		}

		g_menuHash.SaveMenuBar (m_hMenu, this);

		BOOL bMaximizeMode = m_bMaximizeMode;
		m_bMaximizeMode = FALSE;

		m_hMenu = hMenu;
		if (bDefaultMenu)
		{
			m_hDefaultMenu = hMenu;
		}

		DWORD dwOldAlignment = GetCurrentAlignment (); 

		if (!g_menuHash.LoadMenuBar (hMenu, this) || bForceUpdate)
		{
			CMenu* pMenu = CMenu::FromHandle (hMenu);
			if (pMenu == NULL)
			{
				return;
			}

			if (g_pTearOffMenuManager != NULL)
			{
				g_pTearOffMenuManager->SetupTearOffMenus (hMenu);
			}

			RemoveAllButtons ();

			int iCount = (int) pMenu->GetMenuItemCount ();
			for (int i = 0; i < iCount; i ++)
			{
				UINT uiID = pMenu->GetMenuItemID (i);
				
				CString strText;
				pMenu->GetMenuString (i, strText, MF_BYPOSITION);
				
				switch (uiID)
				{
				case -1:	// Pop-up menu
					{
						CMenu* pPopupMenu = pMenu->GetSubMenu (i);
						ASSERT (pPopupMenu != NULL);

						UINT uiTearOffId = 0;
						if (g_pTearOffMenuManager != NULL)
						{
							uiTearOffId = g_pTearOffMenuManager->Parse (strText);
						}

						CBCGPToolbarMenuButton button (
							0, pPopupMenu->GetSafeHmenu (), -1, strText);
						button.m_bText = TRUE;
						button.m_bImage = FALSE;
						button.SetTearOff (uiTearOffId);

						InsertButton (button);
					}
					break;

				case 0:		// Separator
					InsertSeparator ();
					break;

				default:	// Regular command
					{
						CBCGPToolbarButton button (uiID, -1, strText);
						button.m_bText = TRUE;
						button.m_bImage = FALSE;

						InsertButton (button);
					}
					break;
				}
			}

			if (m_nHelpComboID != 0 && CommandToIndex (m_nHelpComboID) < 0)
			{
				CHelpComboBoxButton combobox (m_nHelpComboID, m_nHelpComboWidth, m_strHelpComboPrompt);
				InsertButton (combobox);
			}
		}
		else
		{
			SetBarAlignment (dwOldAlignment);
		}

		if (bMaximizeMode)
		{
			CMDIFrameWnd* pParentFrame = DYNAMIC_DOWNCAST (CMDIFrameWnd, m_pParentWnd);
			if (pParentFrame != NULL)
			{
				SetMaximizeMode (TRUE, pParentFrame->MDIGetActive ());
			}
		}

		if (GetSafeHwnd () != NULL)
		{
			AdjustLayout ();
		}

		RebuildAccelerationKeys ();
	}
	else if (m_bMaximizeMode && !IsCustomizeMode ())
	{
		//----------------------------------
		// System menu should be re-checked:
		//----------------------------------
		SetMaximizeMode (FALSE, NULL, FALSE /* Don't recalculate layout */);
		SetMaximizeMode (TRUE, NULL, FALSE /* Don't recalculate layout */);

		//------------------------
		// Repaint system buttons:
		//------------------------
		InvalidateButton (0);
		for (int i = 0; i < m_nSystemButtonsNum; i++)
		{
			InvalidateButton (GetCount () - 1 - i);
		}
	}

	if (!m_bExclusiveRow)
	{
		AdjustSizeImmediate ();
	}
}
//***************************************************************************************
CSize CBCGPMenuBar::CalcLayout(DWORD dwMode, int nLength)
{
	OnChangeHot (-1);

	//------------------------------------------
	// Is menu bar have the buttons with images?
	//------------------------------------------
	m_bHaveButtons = FALSE;

	for (POSITION pos = m_Buttons.GetHeadPosition (); pos != NULL;)
	{
		CBCGPToolbarButton* pButton = (CBCGPToolbarButton*) m_Buttons.GetNext (pos);
		ASSERT_VALID (pButton);

		if (!pButton->IsKindOf (RUNTIME_CLASS (CBCGPToolbarMenuButtonsButton)) &&
			!pButton->IsKindOf (RUNTIME_CLASS (CBCGPToolbarSystemMenuButton)) &&
			pButton->m_bImage && pButton->IsDrawImage ())
		{
			m_bHaveButtons = TRUE;
			break;
		}
	}
	BOOL bVert = (dwMode & LM_VERTDOCK) || ((dwMode & LM_HORZ) == 0);

	if (IsFloating () || !m_bExclusiveRow)
	{
		return CBCGPToolBar::CalcLayout (dwMode, nLength);
	}

	CRect rect; rect.SetRectEmpty ();
	
	if (m_pDockBarRow != NULL)
	{
		m_pDockBarRow->GetClientRect (rect);
	}


	if (rect.IsRectEmpty ())
	{
		CWnd* pFrame = GetOwner ();
		pFrame->GetClientRect(&rect);
	}

	CSize size;

	CRect rectClient;
	rectClient.SetRectEmpty();
	CalcInsideRect (rectClient, (dwMode & LM_HORZ));

	if (!bVert)
	{
		WrapToolBar (rect.Width() + rectClient.Width() - 1);

		//-------------------------------------
		// Calculate size again after wrapping:
		//-------------------------------------
		size = CalcSize (FALSE);
		size.cx = rect.Width () - rectClient.Width() / 2;
		size.cy -= rectClient.Height();
	}
	else
	{
		//-------------------------------------
		// Calculate size again after wrapping:
		//-------------------------------------
		size = CalcSize (TRUE);

		size.cy = rect.Height () - rectClient.Height() / 2;
		size.cx -= rectClient.Width();
	}

	//--------------------------------------------------
	// Something may changed, rebuild acceleration keys:
	//--------------------------------------------------
	RebuildAccelerationKeys ();
	
	return size;
}
//***************************************************************************************
void CBCGPMenuBar::SetMaximizeMode (BOOL bMax, CWnd* pWnd, BOOL bRecalcLayout)
{
	if (m_bMaximizeMode == bMax)
	{
		return;
	}

	if (bMax)
	{
		if (pWnd == NULL)
		{
			bMax = FALSE;
		}
		else
		{
			ASSERT_VALID (pWnd);

            // get the sys menu.
			m_hSysMenu = pWnd->GetSystemMenu (FALSE)->GetSafeHmenu ();
			if (!::IsMenu (m_hSysMenu))
			{
				m_hSysMenu = NULL;
			}

            // If we have a system menu, then add a system menu button.
            if (m_hSysMenu != NULL)
            {
				m_hSysIcon = pWnd->GetIcon (FALSE);
				if (m_hSysIcon == NULL)
				{
					m_hSysIcon = (HICON)(LONG_PTR) GetClassLongPtr (*pWnd, GCLP_HICONSM);
				}

                InsertButton (CBCGPToolbarSystemMenuButton (m_hSysMenu, m_hSysIcon), 0);
            }

            LONG style = ::GetWindowLong(*pWnd, GWL_STYLE);

            // Assume no buttons.
            m_nSystemButtonsNum = 0;

			if (m_hSysMenu != NULL)
			{
				// Add a minimize box if required.
				if (style & WS_MINIMIZEBOX)
				{
					InsertButton (CBCGPToolbarMenuButtonsButton (SC_MINIMIZE));
					m_nSystemButtonsNum++;
				}

				// Add a restore box if required.
				if (style & WS_MAXIMIZEBOX)
				{
    				InsertButton (CBCGPToolbarMenuButtonsButton (SC_RESTORE));
					m_nSystemButtonsNum++;
				}

				// Add a close box if required.
				CBCGPToolbarMenuButtonsButton closeButton (SC_CLOSE);
				if (m_hSysMenu != NULL)
				{
					//--------------------------------------------------------------
					// Jan Vasina: check if the maximized window has its system menu 
					// with the close button enabled:
					//--------------------------------------------------------------
					MENUITEMINFO menuInfo;
					ZeroMemory(&menuInfo,sizeof(MENUITEMINFO));
					menuInfo.cbSize = sizeof(MENUITEMINFO);
					menuInfo.fMask = MIIM_STATE;

					if (!::GetMenuItemInfo(m_hSysMenu, SC_CLOSE, FALSE, &menuInfo) ||
						(menuInfo.fState & MFS_GRAYED) || 
						(menuInfo.fState & MFS_DISABLED))
					{
						closeButton.m_nStyle |= TBBS_DISABLED;
					}
				}

				InsertButton (closeButton);
				m_nSystemButtonsNum++;
			}
		}
	}
	else
	{
		m_nSystemButtonsNumSaved = m_nSystemButtonsNum;

        // Remove first button if a system menu was added.
        if (m_hSysMenu != NULL)
        {
		    RemoveButton (0);
        }

		int iSysIndex = (int) m_Buttons.GetCount () - 1;
        if (m_pCustomizeBtn != NULL)
        {
            iSysIndex--;
        }

        for (int i = 0; i < m_nSystemButtonsNum; i ++)
        {
			ASSERT_KINDOF (CBCGPToolbarMenuButtonsButton, GetButton (iSysIndex - i));
			RemoveButton (iSysIndex - i);
		}

        // Now we have no system buttons on the menu.
        m_nSystemButtonsNum = 0;
	}

	m_bMaximizeMode = bMax;

	if (bRecalcLayout)
	{
		AdjustLayout ();
	}

	if (!m_bExclusiveRow && bRecalcLayout)
	{
		AdjustSizeImmediate (bRecalcLayout);
	}
}
//***************************************************************************************
void CBCGPMenuBar::RestoreMaximizeMode (BOOL bRecalcLayout)
{
	if (m_bMaximizeMode)
	{
		return;
	}

	int nSystemButtonsNum = 0;

	if (m_hSysMenu != NULL)
	{
		CMDIFrameWnd* pParentFrame = DYNAMIC_DOWNCAST (CMDIFrameWnd, m_pParentWnd);
		if (pParentFrame != NULL && pParentFrame->MDIGetActive () != NULL)
		{
			LONG style = ::GetWindowLong(*pParentFrame->MDIGetActive (), GWL_STYLE);

			CBCGPToolbarSystemMenuButton button (m_hSysMenu, m_hSysIcon);
			InsertButton (button, 0);
		
			if (style & WS_MINIMIZEBOX)
			{
				InsertButton (CBCGPToolbarMenuButtonsButton (SC_MINIMIZE));
				nSystemButtonsNum++;
			}

			if (style & WS_MAXIMIZEBOX)
			{
				InsertButton (CBCGPToolbarMenuButtonsButton (SC_RESTORE));
				nSystemButtonsNum++;
			}

			CBCGPToolbarMenuButtonsButton closeButton (SC_CLOSE);

			MENUITEMINFO menuInfo;
			ZeroMemory(&menuInfo,sizeof(MENUITEMINFO));
			menuInfo.cbSize = sizeof(MENUITEMINFO);
			menuInfo.fMask = MIIM_STATE;

			if (!::GetMenuItemInfo(m_hSysMenu, SC_CLOSE, FALSE, &menuInfo) ||
				(menuInfo.fState & MFS_GRAYED) || 
				(menuInfo.fState & MFS_DISABLED))
			{
				closeButton.m_nStyle |= TBBS_DISABLED;
			}

			InsertButton (closeButton);
			nSystemButtonsNum++;
		}
	}

	m_bMaximizeMode = TRUE;
	m_nSystemButtonsNum = m_nSystemButtonsNumSaved;

	ASSERT (m_nSystemButtonsNum == nSystemButtonsNum);

	if (bRecalcLayout)
	{
		GetParentFrame ()->RecalcLayout ();

		Invalidate ();
		UpdateWindow ();
	}
}
//***************************************************************************************
void CBCGPMenuBar::AdjustLocations ()
{
	CBCGPToolbarComboBoxButton* pHelpCombobox = GetHelpCombobox ();
	if (pHelpCombobox != NULL)
	{
		pHelpCombobox->Show (FALSE);
	}

	CBCGPToolBar::AdjustLocations ();

	CRect rectClient;
	GetClientRect (&rectClient);

	int xRight = rectClient.right;
	BOOL bHorz = GetCurrentAlignment () & CBRS_ORIENT_HORZ ? TRUE : FALSE;
		
	if (m_bMaximizeMode)
	{
		int iButtonWidth = m_szSystemButton.cx;
		int iButtonHeight = m_szSystemButton.cy;

		POSITION pos = m_Buttons.GetTailPosition ();
		CRect rect = rectClient;
		
		rectClient.SetRectEmpty ();
		CalcInsideRect (rectClient, bHorz);

		if (!bHorz)
		{
			rect.bottom += rectClient.Height ();
		}

		rect.left = rect.right - iButtonWidth;
		rect.top = rect.bottom - iButtonHeight;

		if (m_pCustomizeBtn != NULL)
		{
			m_Buttons.GetPrev(pos);
		}

		for (int i = 0; i < m_nSystemButtonsNum; i ++)
		{
			ASSERT (pos != NULL);

			CBCGPToolbarMenuButtonsButton* pButton = 
				(CBCGPToolbarMenuButtonsButton*) m_Buttons.GetPrev (pos);
			ASSERT_KINDOF (CBCGPToolbarMenuButtonsButton, pButton);

			pButton->SetRect (rect);

			xRight = rect.left;

			if (bHorz)
			{
				rect.OffsetRect (-iButtonWidth - 1, 0);
			}
			else
			{
				rect.OffsetRect (0, -iButtonHeight - 1);
			}
		}
	}

	//----------------------------
	// Adjust help combo location:
	//----------------------------
	if (pHelpCombobox != NULL)
	{
		pHelpCombobox->Show (TRUE);

		CRect rectCombo = pHelpCombobox->Rect ();

		if (bHorz)
		{
			rectCombo.right = xRight;
			rectCombo.left = rectCombo.right - m_nHelpComboWidth;
		}
		else
		{
			rectCombo.SetRectEmpty ();
		}

		pHelpCombobox->SetRect (rectCombo);
	}
}
//***************************************************************************************
BOOL CBCGPMenuBar::OnSendCommand (const CBCGPToolbarButton* pButton)
{
	CBCGPToolbarMenuButtonsButton* pSysButton =
		DYNAMIC_DOWNCAST (CBCGPToolbarMenuButtonsButton, pButton);
	if (pSysButton == NULL)
	{
		return FALSE;
	}

	if (pSysButton->m_uiSystemCommand != SC_CLOSE &&
		pSysButton->m_uiSystemCommand != SC_MINIMIZE &&
		pSysButton->m_uiSystemCommand != SC_RESTORE)
	{
		ASSERT (FALSE);
		return TRUE;
	}

	CMDIFrameWnd* pParentFrame = 
		DYNAMIC_DOWNCAST (CMDIFrameWnd, m_pParentWnd);

	if (pParentFrame == NULL)
	{
		MessageBeep ((UINT) -1);
		return TRUE;
	}
		
	CMDIChildWnd* pChild = pParentFrame->MDIGetActive ();
	ASSERT_VALID (pChild);

	pChild->SendMessage (WM_SYSCOMMAND, pSysButton->m_uiSystemCommand);
	return TRUE;
}
//*************************************************************************************
int CBCGPMenuBar::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT_VALID(this);

	int nHit = ((CBCGPMenuBar*)this)->HitTest(point);
	if (nHit != -1)
	{
		CBCGPToolbarButton* pButton = GetButton (nHit);
		if (pButton == NULL ||
			pButton->IsKindOf (RUNTIME_CLASS (CBCGPToolbarMenuButton)))
		{
			//-----------------------------------	
			// Don't show tooltips on menu items!
			//-----------------------------------
			return -1;
		}
	}

	return CBCGPToolBar::OnToolHitTest (point, pTI);
}
//*************************************************************************************
int CBCGPMenuBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	//------------------------------------
	// Attach menubar to the parent frame:
	//------------------------------------

	//----------------------
	// First, try MDI frame:
	//----------------------
	CBCGPMDIFrameWnd* pWndParentMDIFrame = 
		DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, m_pParentWnd);
	if (pWndParentMDIFrame != NULL)
	{
		pWndParentMDIFrame->m_Impl.SetMenuBar (this);
	}
	else
	{
		CBCGPFrameWnd* pWndParentFrame = 
			DYNAMIC_DOWNCAST (CBCGPFrameWnd, m_pParentWnd);
		if (pWndParentFrame != NULL)
		{
			pWndParentFrame->m_Impl.SetMenuBar (this);
		}
		else
		{
			CBCGPOleIPFrameWnd* pOleFrame = 
				DYNAMIC_DOWNCAST (CBCGPOleIPFrameWnd, GetParentFrame ());
			if (pOleFrame != NULL)
			{
				pOleFrame->m_Impl.SetMenuBar (this);
			}
		}
	}

	//----------------------------
	// Set default menu bar title:
	//----------------------------
	CBCGPLocalResource locaRes;

	CString strTitle;
	strTitle.LoadString (IDS_BCGBARRES_MENU_BAR_TITLE);
		
	SetWindowText (strTitle);

	//-------------------------------------------------------------
	// Force the menu bar to be hiden whren the in-place editing is
	// is activated (server application shows its own menu):
	//-------------------------------------------------------------
	SetBarStyle (GetBarStyle() | CBRS_HIDE_INPLACE);

	//------------------------------
	// Calculate system button size:
	//------------------------------
	CalcSysButtonSize ();
	return 0;
}
//*************************************************************************************
BOOL CBCGPMenuBar::LoadState (LPCTSTR lpszProfileName, int nIndex, UINT /*uiID*/)
{
	ASSERT (m_hDefaultMenu != NULL);

	CString strProfileName = ::BCGPGetRegPath (strMenuProfile, lpszProfileName);

	//------------------------------------------------------------
	// Save current maximize mode (system buttons are not saved!):
	//------------------------------------------------------------
	BOOL bMaximizeMode = m_bMaximizeMode;
	SetMaximizeMode (FALSE, NULL, FALSE);

	CDocManager* pDocManager = AfxGetApp ()->m_pDocManager;
	if (m_bAutoDocMenus && pDocManager != NULL)
	{
		//---------------------------------------
		// Walk all templates in the application:
		//---------------------------------------
		for (POSITION pos = pDocManager->GetFirstDocTemplatePosition (); pos != NULL;)
		{
			CBCGPMultiDocTemplate* pTemplate = 
				(CBCGPMultiDocTemplate*) pDocManager->GetNextDocTemplate (pos);
			ASSERT_VALID (pTemplate);
			ASSERT_KINDOF (CDocTemplate, pTemplate);

			//-----------------------------------------------------
			// We are interessing CMultiDocTemplate objects with
			// the sahred menu only....
			//-----------------------------------------------------
			if (!pTemplate->IsKindOf (RUNTIME_CLASS (CMultiDocTemplate)) ||
				pTemplate->m_hMenuShared == NULL)
			{
				continue;
			}

			UINT uiMenuResId = pTemplate->GetResId ();
			ASSERT (uiMenuResId != 0);

			//---------------------------------------------------------------
			// Load menubar from registry and associate it with the
			// template shared menu:
			//---------------------------------------------------------------
			BuildOrigItems (uiMenuResId);
			if (CBCGPToolBar::LoadState (strProfileName, nIndex, uiMenuResId) &&
				!m_bResourceWasChanged)
			{
				g_menuHash.SaveMenuBar (pTemplate->m_hMenuShared, this);
			}
			else if (GetOwner()->GetSafeHwnd() != NULL) 
			{
				//-----------------------------------------------------
				// Addition by Oz Solomonovich: 
				// The following code was added to ensure that a
				// BCGM_RESETMENU message will be sent to the frame the 
				// first time the application is loaded
				//-----------------------------------------------------
				m_hMenu = NULL;
				CreateFromMenu (pTemplate->m_hMenuShared, FALSE);
        		GetOwner()->SendMessage (BCGM_RESETMENU, uiMenuResId);
    			g_menuHash.SaveMenuBar (pTemplate->m_hMenuShared, this);
				m_hMenu = pTemplate->m_hMenuShared;
			}
		}
	}

	//----------------------
	// Load defualt menubar:
	//----------------------
	BuildOrigItems (m_uiDefMenuResId);

	if (CBCGPToolBar::LoadState (strProfileName, nIndex, 0) &&
		!m_bResourceWasChanged)
	{
		g_menuHash.SaveMenuBar (m_hDefaultMenu, this);
	}
    else if (GetOwner()->GetSafeHwnd() != NULL)
    {
        // The following code was added to ensure that a BCGM_RESETMENU 
        // message will be sent to the frame the first time the application 
        // is loaded
        
		m_hMenu = NULL;
		CreateFromMenu (m_hDefaultMenu, TRUE);

		UINT uiResID = m_uiDefMenuResId;
		if (uiResID == 0)
		{
			// Obtain main window resource ID:
			UINT uiResID = (UINT) GetOwner()->SendMessage (WM_HELPHITTEST);
			if (uiResID != 0)
			{
				uiResID -= HID_BASE_RESOURCE;
			}
		}
        GetOwner()->SendMessage (BCGM_RESETMENU, uiResID);
    	g_menuHash.SaveMenuBar (m_hDefaultMenu, this);
                m_hMenu = m_hDefaultMenu;
	}

	//----------------------
	// Restore current menu:
	//----------------------
	BOOL bLoaded = (m_hMenu != NULL && g_menuHash.LoadMenuBar (m_hMenu, this));
	
	if (bMaximizeMode)
	{
		RestoreMaximizeMode (!bLoaded); // do not recalc layout if the menu was loaded
	}

	if (bLoaded) 
	{
		GetParentFrame ()->RecalcLayout ();
		Invalidate ();
		UpdateWindow ();
	}

	AdjustLayout ();
	RebuildAccelerationKeys ();

	return TRUE;
}
//*************************************************************************************
BOOL CBCGPMenuBar::SaveState (LPCTSTR lpszProfileName, int nIndex, UINT /*uiID*/)
{
	ASSERT (m_hDefaultMenu != NULL);

	CString strProfileName = ::BCGPGetRegPath (strMenuProfile, lpszProfileName);

	g_menuHash.SaveMenuBar (m_hMenu, this);

	//------------------------------------------------------------
	// Save current maximize mode (system buttons are not saved!):
	//------------------------------------------------------------
	BOOL bMaximizeMode = m_bMaximizeMode;
	SetMaximizeMode (FALSE, NULL, FALSE);

	CDocManager* pDocManager = AfxGetApp ()->m_pDocManager;
	if (m_bAutoDocMenus && pDocManager != NULL)
	{
		//---------------------------------------
		// Walk all templates in the application:
		//---------------------------------------
		for (POSITION pos = pDocManager->GetFirstDocTemplatePosition (); pos != NULL;)
		{
			CBCGPMultiDocTemplate* pTemplate = 
				(CBCGPMultiDocTemplate*) pDocManager->GetNextDocTemplate (pos);
			ASSERT_VALID (pTemplate);
			ASSERT_KINDOF (CDocTemplate, pTemplate);

			//-----------------------------------------------------
			// We are interessing CMultiDocTemplate objects with
			// the sahred menu only....
			//-----------------------------------------------------
			if (!pTemplate->IsKindOf (RUNTIME_CLASS (CMultiDocTemplate)) ||
				pTemplate->m_hMenuShared == NULL)
			{
				continue;
			}

			UINT uiMenuResId = pTemplate->GetResId ();
			ASSERT (uiMenuResId != 0);

			//----------------------------------------------------------
			// Load menubar associated with the template shared menu and
			// save it in the registry:
			//----------------------------------------------------------
			if (g_menuHash.LoadMenuBar (pTemplate->m_hMenuShared, this))
			{
				BuildOrigItems (uiMenuResId);
				CBCGPToolBar::SaveState (strProfileName, nIndex, uiMenuResId);
			}
		}
	}

	//-------------------
	// Save default menu:
	//-------------------
	if (g_menuHash.LoadMenuBar (m_hDefaultMenu, this))
	{
		BuildOrigItems (m_uiDefMenuResId);
		CBCGPToolBar::SaveState (strProfileName, nIndex, 0);
	}

	//----------------------
	// Restore current menu:
	//----------------------
	BOOL bRestored = (m_hMenu != NULL && g_menuHash.LoadMenuBar (m_hMenu, this));

	if (bMaximizeMode)
	{
		RestoreMaximizeMode (!bRestored);
	}

	AdjustSizeImmediate ();
	
	if (bRestored)
	{
		GetParentFrame ()->RecalcLayout ();
		Invalidate ();
		UpdateWindow ();
	}
	
	AdjustLayout ();

	return TRUE;
}
//*****************************************************************************************
void CBCGPMenuBar::ResetImages ()
{
	ASSERT (m_hDefaultMenu != NULL);

	g_menuHash.SaveMenuBar (m_hMenu, this);

	CDocManager* pDocManager = AfxGetApp ()->m_pDocManager;
	if (pDocManager != NULL)
	{
		//---------------------------------------
		// Walk all templates in the application:
		//---------------------------------------
		for (POSITION pos = pDocManager->GetFirstDocTemplatePosition (); pos != NULL;)
		{
			CBCGPMultiDocTemplate* pTemplate = 
				(CBCGPMultiDocTemplate*) pDocManager->GetNextDocTemplate (pos);
			ASSERT_VALID (pTemplate);
			ASSERT_KINDOF (CDocTemplate, pTemplate);

			//-----------------------------------------------------
			// We are interessing CMultiDocTemplate objects with
			// the sahred menu only....
			//-----------------------------------------------------
			if (!pTemplate->IsKindOf (RUNTIME_CLASS (CMultiDocTemplate)) ||
				pTemplate->m_hMenuShared == NULL)
			{
				continue;
			}

			if (g_menuHash.LoadMenuBar (pTemplate->m_hMenuShared, this))
			{
				CBCGPToolBar::ResetImages ();
				g_menuHash.SaveMenuBar (pTemplate->m_hMenuShared, this);
			}
		}
	}

	//--------------------
	// Reset default menu:
	//--------------------
	if (g_menuHash.LoadMenuBar (m_hDefaultMenu, this))
	{
		CBCGPToolBar::ResetImages ();
		g_menuHash.SaveMenuBar (m_hDefaultMenu, this);
	}

	//----------------------
	// Restore current menu:
	//----------------------
	if (m_hMenu != NULL && g_menuHash.LoadMenuBar (m_hMenu, this))
	{
		GetParentFrame ()->RecalcLayout ();
		Invalidate ();
		UpdateWindow ();
	}
}
//*****************************************************************************************
void CBCGPMenuBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int iHit = HitTest (point);

	//--------------------------------------------------------
	// Disable control bar dragging if any menues are dropped!
	//--------------------------------------------------------
	if (iHit < 0 &&	// Click outside of buttons
		GetDroppedDownMenu () != NULL)
	{
//		Do nothing
		CBCGPToolBar::OnLButtonDown(nFlags, point);
	}
	else
	{		
		CBCGPToolBar::OnLButtonDown(nFlags, point);
	}
}
//****************************************************************************************
BOOL CBCGPMenuBar::RestoreOriginalstate ()
{
	HMENU hMenuCurr = m_hMenu;
	
	if (m_hMenu != NULL)
	{
		g_menuHash.SaveMenuBar (m_hMenu, this);
	}


	//-----------------------
	// Save customize button:
	//-----------------------
	CCustomizeButton* pCustomizeBtn = NULL;
	if (m_pCustomizeBtn != NULL)
	{
		ASSERT_VALID (m_pCustomizeBtn);
		ASSERT (m_pCustomizeBtn == m_Buttons.GetTail ());	// Should be last

		pCustomizeBtn = new CCustomizeButton;
		pCustomizeBtn->CopyFrom (*m_pCustomizeBtn);
	}



	CBCGPMDIFrameWnd* pWndParentMDIFrame = 
		DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, m_pParentWnd);

	if (g_pTearOffMenuManager != NULL)
	{
		g_pTearOffMenuManager->Reset (NULL);
	}

	BOOL bCurrMenuIsRestored = FALSE;
	CDocTemplate* pActiveTemplate = NULL;

	//------------------------------------------------------------
	// Save current maximize mode (system buttons are not saved!):
	//------------------------------------------------------------
	BOOL bMaximizeMode = m_bMaximizeMode;
	SetMaximizeMode (FALSE);

	CDocManager* pDocManager = AfxGetApp ()->m_pDocManager;
	if (pDocManager != NULL)
	{
		//------------------------------------
		// Find an active document's template:
		//------------------------------------
		CMDIFrameWnd* pParentFrame = DYNAMIC_DOWNCAST (CMDIFrameWnd, m_pParentWnd);
		if (pParentFrame != NULL && pParentFrame->MDIGetActive () != NULL)
		{
			CDocument* pActiveDoc = pParentFrame->MDIGetActive ()->GetActiveDocument ();
			if (pActiveDoc != NULL)
			{
				pActiveTemplate = pActiveDoc->GetDocTemplate ();
			}
		}

		//---------------------------------------
		// Walk all templates in the application:
		//---------------------------------------
		for (POSITION pos = pDocManager->GetFirstDocTemplatePosition (); pos != NULL;)
		{
			CBCGPMultiDocTemplate* pTemplate = 
				(CBCGPMultiDocTemplate*) pDocManager->GetNextDocTemplate (pos);
			ASSERT_VALID (pTemplate);
			ASSERT_KINDOF (CDocTemplate, pTemplate);

			//-----------------------------------------------------
			// We are interessing CMultiDocTemplate objects with
			// the shared menu only....
			//-----------------------------------------------------
			if (!pTemplate->IsKindOf (RUNTIME_CLASS (CMultiDocTemplate)) ||
				pTemplate->m_hMenuShared == NULL)
			{
				continue;
			}

			UINT uiMenuResId = pTemplate->GetResId ();
			ASSERT (uiMenuResId != 0);

			//-------------------------------------
			// Restore original menu from resource:
			//-------------------------------------
			HINSTANCE hInst = AfxFindResourceHandle (
				MAKEINTRESOURCE (uiMenuResId), RT_MENU);

			BOOL bCurr = (pActiveTemplate == pTemplate);

			HMENU hmenuSharedOld = pTemplate->m_hMenuShared;
			pTemplate->m_hMenuShared = ::LoadMenu (hInst, MAKEINTRESOURCE (uiMenuResId));

			CreateFromMenu (pTemplate->m_hMenuShared, FALSE);
			g_menuHash.SaveMenuBar (pTemplate->m_hMenuShared, this);

			if (bCurr)
			{
				hMenuCurr = pTemplate->m_hMenuShared;
				bCurrMenuIsRestored = TRUE;
			}

			//----------------------------------------------
			// Update shared menus in all MDI child windows:
			//----------------------------------------------
			UpdateMDIChildrenMenus (pTemplate);

			if (hmenuSharedOld != NULL)
			{
				ASSERT (::IsMenu (hmenuSharedOld));
				g_menuHash.RemoveMenu (hmenuSharedOld);
				::DestroyMenu (hmenuSharedOld);
			}
		}
	}

	//----------------------
	// Load defualt menubar:
	//----------------------
	if (m_uiDefMenuResId != 0)
	{
		HINSTANCE hInst = AfxFindResourceHandle (
			MAKEINTRESOURCE (m_uiDefMenuResId), RT_MENU);

		HMENU hOldDefaultMenu = m_hDefaultMenu;

		m_hDefaultMenu = ::LoadMenu (hInst, MAKEINTRESOURCE (m_uiDefMenuResId));

		OnDefaultMenuLoaded(m_hDefaultMenu);

		CreateFromMenu (m_hDefaultMenu, TRUE);
		g_menuHash.SaveMenuBar (m_hDefaultMenu, this);

		if (!bCurrMenuIsRestored)
		{
			hMenuCurr = m_hDefaultMenu;
		}

		if (pWndParentMDIFrame != NULL)
		{
			pWndParentMDIFrame->m_hMenuDefault = m_hDefaultMenu;
			pWndParentMDIFrame->m_Impl.m_hDefaultMenu = m_hDefaultMenu;
		}

		CFrameWnd* pWndParentFrame = DYNAMIC_DOWNCAST (CFrameWnd, m_pParentWnd);
		if (pWndParentFrame != NULL)
		{
			pWndParentFrame->m_hMenuDefault = m_hDefaultMenu;
		}

		if (hOldDefaultMenu != NULL)
		{
			ASSERT (::IsMenu (hOldDefaultMenu));

			g_menuHash.RemoveMenu (hOldDefaultMenu);
			::DestroyMenu (hOldDefaultMenu);
		}
	}

	//----------------------
	// Restore current menu:
	//----------------------
	if (g_menuHash.LoadMenuBar (hMenuCurr, this))
	{
		m_hMenu = hMenuCurr;

		if (!bMaximizeMode)
		{
			GetParentFrame ()->RecalcLayout ();
			Invalidate ();
			UpdateWindow ();
		}
	}

	if (pWndParentMDIFrame != NULL)
	{
		pWndParentMDIFrame->OnUpdateFrameMenu (m_hMenu);
	}

	if (bMaximizeMode)
	{
		RestoreMaximizeMode ();
	}

#ifndef BCG_NO_CUSTOMIZATION

	if (m_pMenuPage != NULL)
	{
		ASSERT_VALID (m_pMenuPage);
		m_pMenuPage->SelectMenu (pActiveTemplate, FALSE /* Don't save cur. menu */);
	}

#endif // BCG_NO_CUSTOMIZATION

	//--------------------------
	// Restore customize button:
	//--------------------------
	if (pCustomizeBtn != NULL)
	{
		InsertButton (pCustomizeBtn);
		m_pCustomizeBtn = pCustomizeBtn;
		AdjustLayout();
		AdjustSizeImmediate ();
	}

	return TRUE;
}
//********************************************************************************************
void CBCGPMenuBar::SetDefaultMenuResId (UINT uiResId)
{
	m_uiDefMenuResId = uiResId;
}
//******************************************************************
BOOL CBCGPMenuBar::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		//-----------------------------------------------------------
		// Fisrt, try to move keyboard control to the drop-down menu:
		//-----------------------------------------------------------
		CBCGPToolbarMenuButton* pMenuButon = GetDroppedDownMenu ();
		if (pMenuButon != NULL)
		{
			return CBCGPBaseControlBar::PreTranslateMessage(pMsg);
		}

		int iTotalItems = GetCount ();
		if (m_bMaximizeMode)
		{
			iTotalItems -= m_nSystemButtonsNum;
		}

		if (m_iHighlighted >= 0 && m_iHighlighted < iTotalItems)
		{
			int iButton = m_iHighlighted;

			switch (pMsg->wParam)
			{
			case VK_ESCAPE:
				{
					Deactivate ();
					RestoreFocus ();
					m_bShowAllCommands = FALSE;
				}
				break;

			case VK_RIGHT:
				if (++ m_iHighlighted >= iTotalItems)
				{
					m_iHighlighted = 0;
				}

				InvalidateButton (iButton);
				InvalidateButton (m_iHighlighted);
				UpdateWindow ();
				break;

			case VK_LEFT:
				if (-- m_iHighlighted < 0)
				{
					m_iHighlighted = iTotalItems - 1;
				}

				InvalidateButton (iButton);
				InvalidateButton (m_iHighlighted);
				UpdateWindow ();
				break;

			case VK_DOWN:
				DropDownMenu (GetButton (m_iHighlighted));
				return TRUE;

			case VK_RETURN:
				if (!DropDownMenu (GetButton (m_iHighlighted)))
				{
					ProcessCommand (GetButton (m_iHighlighted));
				}
				return TRUE;

			default:
				if (TranslateChar ((int) pMsg->wParam))
				{
					return TRUE;
				}
			}
		}
	}

	return CBCGPToolBar::PreTranslateMessage(pMsg);
}
//**************************************************************************************
void CBCGPMenuBar::OnSetFocus(CWnd* pOldWnd) 
{
	CBCGPToolBar::OnSetFocus(pOldWnd);

	if (GetDroppedDownMenu () == NULL)
	{
		GetOwner()->SendMessage (WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE);

		int iFirstItem = (GetSystemMenu () != NULL) ? 1 : 0;
		if (m_iHighlighted < 0 && iFirstItem < GetCount ())
		{
			m_iHighlighted = iFirstItem;
			InvalidateButton (iFirstItem);
		}
	}
}
//**************************************************************************************
BOOL CBCGPMenuBar::Create(CWnd* pParentWnd,
			DWORD dwStyle,
			UINT nID)
{
	m_pParentWnd = pParentWnd;
	return CBCGPToolBar::Create (pParentWnd, dwStyle, nID);
}
//***************************************************************************************
BOOL CBCGPMenuBar::CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle,
		DWORD dwStyle,
		CRect rcBorders,
		UINT nID)
{
	m_pParentWnd = pParentWnd;
	return CBCGPToolBar::CreateEx (pParentWnd, dwCtrlStyle, dwStyle, rcBorders, nID);
}
//***************************************************************************************
CSize CBCGPMenuBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	if (m_Buttons.IsEmpty ())
	{
		return GetButtonSize ();
	}

	DWORD dwMode = bStretch ? LM_STRETCH : 0;
	dwMode |= bHorz ? LM_HORZ : 0;

	return CalcLayout (dwMode);
}
//*****************************************************************************************
BOOL CBCGPMenuBar::OnSetDefaultButtonText (CBCGPToolbarButton* pButton)
{
	ASSERT_VALID (pButton);

	CString strText;
	if (FindMenuItemText (m_hMenu, pButton->m_nID, strText))
	{
		pButton->m_strText = strText;
		return TRUE;
	}

	return CBCGPToolBar::OnSetDefaultButtonText (pButton);
}
//****************************************************************************************
BOOL CBCGPMenuBar::FindMenuItemText (HMENU hMenu, const UINT nItemID, CString& strOutText)
{
	if (hMenu == NULL || nItemID == 0 || nItemID == (UINT) -1)
	{
		return FALSE;
	}

	CMenu* pMenu = CMenu::FromHandle (hMenu);
	if (pMenu == NULL)
	{
		return FALSE;
	}

	int iCount = (int) pMenu->GetMenuItemCount ();
	for (int i = 0; i < iCount; i ++)
	{
		UINT uiID = pMenu->GetMenuItemID (i);
		if (uiID == nItemID)	// Found!
		{
			pMenu->GetMenuString (i, strOutText, MF_BYPOSITION);
			return TRUE;
		}
		else if (uiID == -1)	// Pop-up menu
		{
			CMenu* pPopupMenu = pMenu->GetSubMenu (i);
			ASSERT (pPopupMenu != NULL);

			if (FindMenuItemText (pPopupMenu->GetSafeHmenu (), nItemID, strOutText))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}
//****************************************************************************************
int CBCGPMenuBar::FindDropIndex (const CPoint point, CRect& rectDrag) const
{
	int iIndex = CBCGPToolBar::FindDropIndex (point, rectDrag);
	if (m_bMaximizeMode && iIndex >= 0)
	{
		//--------------------------------------
		// Maybe drag left from the system icon?
		//--------------------------------------
		if (iIndex == 0 && m_hSysMenu != NULL)
		{
			return -1;
		}

		//-----------------------------------------
		// Maybe drag right of the system buttons?
		//-----------------------------------------
		if (iIndex > GetCount () - m_nSystemButtonsNum)
		{
			iIndex = GetCount () - m_nSystemButtonsNum;

			if (m_nSystemButtonsNum > 0)
			{
				//----------------------------------------------------------
				// Put drag rectangle right of the last "non-system" button:
				//----------------------------------------------------------

				CBCGPToolbarButton* pLastButton = GetButton (iIndex - 1);
				ASSERT_VALID (pLastButton);

				CRect rectBtn = pLastButton->Rect ();
				CPoint ptDrag (rectBtn.right, rectBtn.top + rectBtn.Height () / 2);

				VERIFY (CBCGPToolBar::FindDropIndex (ptDrag, rectDrag) == iIndex);
			}
		}
	}

	if (m_nHelpComboID != 0)
	{
		int nHelpComboIndex = CommandToIndex (m_nHelpComboID);
		if (nHelpComboIndex >= 0 && iIndex > nHelpComboIndex)
		{
			iIndex = nHelpComboIndex;
		}
	}

	return iIndex;
}
//****************************************************************************************
void CBCGPMenuBar::OnChangeHot (int iHot)
{
	CBCGPToolBar::OnChangeHot (iHot);

	KillTimer (uiShowAllItemsTimerId);

	if (GetDroppedDownMenu () == NULL)
	{
		m_bShowAllCommands = FALSE;
	}
	else
	{
		SetTimer (uiShowAllItemsTimerId, iShowAllItemsTimerFreq, NULL);
	}
}
//****************************************************************************************
void CBCGPMenuBar::SetShowAllCommands (BOOL bShowAllCommands)
{
	m_bShowAllCommands = bShowAllCommands;
}
//***************************************************************************************
void CBCGPMenuBar::SetRecentlyUsedMenus (BOOL bOn)
{
	m_bRecentlyUsedMenus = bOn;
}
//***************************************************************************************
CBCGPToolbarButton* CBCGPMenuBar::GetMenuItem (int iItem) const
{
	if (m_bMaximizeMode)
	{
		iItem --;	// Ignore system-menu button
	}

	return GetButton (iItem);
}
//***************************************************************************************
CBCGPToolbarSystemMenuButton* CBCGPMenuBar::GetSystemMenu () const
{
	if (!m_bMaximizeMode)
	{
		return NULL;
	}

	if (m_Buttons.IsEmpty ())
	{
		return NULL;
	}
	return DYNAMIC_DOWNCAST (CBCGPToolbarSystemMenuButton, m_Buttons.GetHead ());
}
//***************************************************************************************
CBCGPToolbarMenuButtonsButton* CBCGPMenuBar::GetSystemButton (UINT uiBtn, BOOL bByCommand) const
{
	if (!m_bMaximizeMode)
	{
		return NULL;
	}

	if (bByCommand)
	{
		for (POSITION pos = m_Buttons.GetTailPosition (); pos != NULL;)
		{
			CBCGPToolbarMenuButtonsButton* pButton = 
				DYNAMIC_DOWNCAST (CBCGPToolbarMenuButtonsButton,
									m_Buttons.GetPrev (pos));

			if (pButton == NULL)
			{
				break;
			}

			if (pButton->m_nID == uiBtn)
			{
				return pButton;
			}
		}

		return NULL;
	}
	// else - by index:
	if ((int) uiBtn < 0 || (int) uiBtn >= m_nSystemButtonsNum)
	{
		ASSERT (FALSE);
		return NULL;
	}

	int iIndex = (int) m_Buttons.GetCount () - m_nSystemButtonsNum + uiBtn;

	CBCGPToolbarMenuButtonsButton* pButton = 
		DYNAMIC_DOWNCAST (CBCGPToolbarMenuButtonsButton, GetButton (iIndex));
	ASSERT_VALID (pButton);

	return pButton;
}
//************************************************************************************
BOOL CBCGPMenuBar::SetMenuFont (LPLOGFONT lpLogFont, BOOL bHorz)
{
	if (!globalData.SetMenuFont (lpLogFont, bHorz))
	{
		return FALSE;
	}
	
	//-------------------------------------------
	// Recalculate all toolbars and menus layout:
	//-------------------------------------------
	extern CObList gAllToolbars;

	for (POSITION pos = gAllToolbars.GetHeadPosition (); pos != NULL;)
	{
		CBCGPToolBar* pToolBar = (CBCGPToolBar*) gAllToolbars.GetNext (pos);
		ASSERT (pToolBar != NULL);

		if (CWnd::FromHandlePermanent (pToolBar->m_hWnd) != NULL)
		{
			ASSERT_VALID(pToolBar);
			pToolBar->AdjustLayout ();
		}
	}

	return TRUE;
}
//************************************************************************************
const CFont& CBCGPMenuBar::GetMenuFont (BOOL bHorz)
{
	return bHorz ? globalData.fontRegular : globalData.fontVert;
}
//************************************************************************************
void CBCGPMenuBar::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == uiShowAllItemsTimerId)
	{
		CPoint ptCursor;

		::GetCursorPos (&ptCursor);
		ScreenToClient (&ptCursor);

		//--------------------------------------------------------------
		// Check that the popup-menu is still exist and mouse cursor is
		// within the menu button:
		//--------------------------------------------------------------
		CBCGPToolbarMenuButton* pMenuButon = GetDroppedDownMenu ();
		if (pMenuButon != NULL && pMenuButon->m_pPopupMenu != NULL &&
			pMenuButon->m_rect.PtInRect (ptCursor) &&
			!pMenuButon->m_pPopupMenu->AreAllCommandsShown ())
		{
			pMenuButon->m_pPopupMenu->ShowAllCommands ();
		}

		KillTimer (uiShowAllItemsTimerId);
	}
	
	CBCGPToolBar::OnTimer(nIDEvent);
}
//*******************************************************************************************
void CBCGPMenuBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	int iButton = HitTest(point);
	BOOL bSysMenu = FALSE;

	if (iButton >= 0)
	{
		bSysMenu = 
			DYNAMIC_DOWNCAST (CBCGPToolbarSystemMenuButton, GetButton (iButton)) != NULL;
	}

	CBCGPToolBar::OnLButtonDblClk(nFlags, point);

	if (bSysMenu || IsShowAllCommands () || IsCustomizeMode ())
	{
		return;
	}

	if ((iButton = HitTest(point)) < 0)
	{
		return;
	}

	CBCGPToolbarMenuButton* pMenuButton = DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, GetButton(iButton));
	if (pMenuButton == NULL)
	{
		return;
	}

	//////////////////////////////////////////////
	// Special deal to system menu button 
	//--------------------------------------------
	if (pMenuButton->IsKindOf (RUNTIME_CLASS (CBCGPToolbarSystemMenuButton)))
	{
		return;
	}
	//--------------------------------------------
	//////////////////////////////////////////////

	m_bShowAllCommands = TRUE;
	pMenuButton->OnCancelMode ();

	if (!(pMenuButton->m_nStyle & TBBS_DISABLED) && 
		pMenuButton->OnClick (this, FALSE))
	{
		OnChangeHot (iButton);

		InvalidateButton (iButton);
		UpdateWindow(); // immediate feedback
	}
}
//***********************************************************************************
void CBCGPMenuBar::CalcSysButtonSize ()
{
	CWindowDC dc (NULL);

	CDC dcMem;
	dcMem.CreateCompatibleDC (NULL);	// Assume display!

	int iButtonWidth = ::GetSystemMetrics (SM_CXMENUSIZE);	
	int iButtonHeight = ::GetSystemMetrics (SM_CXMENUSIZE);

	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap (&dc, iButtonWidth, iButtonHeight);

	CBitmap* pBmpOriginal = dcMem.SelectObject (&bmpMem);

	CRect rectBtn (0, 0, iButtonWidth, iButtonHeight);
	dcMem.DrawFrameControl (rectBtn, DFC_CAPTION, DFCS_ADJUSTRECT);

	m_szSystemButton = rectBtn.Size ();
	dcMem.SelectObject (pBmpOriginal);
}
//*********************************************************************************
void CBCGPMenuBar::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CBCGPToolBar::OnSettingChange(uFlags, lpszSection);
	
	CalcSysButtonSize ();
	Invalidate ();
	UpdateWindow ();
}
//**********************************************************************************
int CBCGPMenuBar::CalcMaxButtonHeight ()
{
	m_bHaveButtons = FALSE;

	for (POSITION pos = m_Buttons.GetHeadPosition (); pos != NULL;)
	{
		CBCGPToolbarButton* pButton = (CBCGPToolbarButton*) m_Buttons.GetNext (pos);
		ASSERT_VALID (pButton);

		if (!pButton->IsKindOf (RUNTIME_CLASS (CBCGPToolbarMenuButtonsButton)) &&
			!pButton->IsKindOf (RUNTIME_CLASS (CBCGPToolbarSystemMenuButton)) &&
			pButton->m_bImage && pButton->IsDrawImage ())
		{
			m_bHaveButtons = TRUE;
			break;
		}
	}

	return GetRowHeight ();
}
//*********************************************************************************
BOOL CBCGPMenuBar::BuildOrigItems (UINT uiMenuResID)
{
	while (!m_OrigButtons.IsEmpty ())
	{
		delete m_OrigButtons.RemoveHead ();
	}

	if (GetWorkspace () == NULL ||
		!GetWorkspace ()->IsResourceSmartUpdate ())
	{
		return FALSE;
	}
	
	CMenu menu;
	if (!menu.LoadMenu (uiMenuResID))
	{
		ASSERT (FALSE);
		return FALSE;
	}

	int iCount = (int) menu.GetMenuItemCount ();
	for (int i = 0; i < iCount; i ++)
	{
		UINT uiID = menu.GetMenuItemID (i);

		CString strText;

#ifdef _DEBUG
		menu.GetMenuString (i, strText, MF_BYPOSITION);
#endif

		switch (uiID)
		{
		case -1:	// Pop-up menu
			{
				CMenu* pPopupMenu = menu.GetSubMenu (i);
				ASSERT (pPopupMenu != NULL);

				m_OrigButtons.AddTail (
					new CBCGPToolbarMenuButton (0, pPopupMenu->GetSafeHmenu (), -1, strText));
			}
			break;

		case 0:		// Separator
			{
				CBCGPToolbarButton* pButton = new CBCGPToolbarButton;
				ASSERT (pButton != NULL);

				pButton->m_nStyle = TBBS_SEPARATOR;
				m_OrigButtons.AddTail (pButton);
			}
			break;

		default:	// Regular command

			m_OrigButtons.AddTail (new CBCGPToolbarButton (uiID, -1, strText));
			break;
		}
	}

	return TRUE;
}
//*****************************************************************************************
void CBCGPMenuBar::UpdateMDIChildrenMenus (CMultiDocTemplate* pTemplate)
{
	ASSERT_VALID (pTemplate);

	for (POSITION pos = pTemplate->GetFirstDocPosition (); pos != NULL;)
	{
		CDocument* pDoc = pTemplate->GetNextDoc (pos);
		ASSERT_VALID (pDoc);

		// assumes 1 doc per frame
		for (POSITION posView = pDoc->GetFirstViewPosition (); posView != NULL;)
		{
			CView* pView = pDoc->GetNextView (posView);
			ASSERT_VALID (pView);

			CMDIChildWnd* pFrame = DYNAMIC_DOWNCAST (CMDIChildWnd, pView->GetParentFrame());
			if (pFrame != NULL)
			{
				pFrame->SetHandles (pTemplate->m_hMenuShared, pTemplate->m_hAccelTable);
			}
		}
	}
}
//****************************************************************************************
BOOL CBCGPMenuBar::IsPureMenuButton (CBCGPToolbarButton* pButton) const
{
	ASSERT_VALID (pButton);
	return m_bMenuMode || pButton->IsKindOf (RUNTIME_CLASS (CBCGPToolbarMenuButton));
}
//**********************************************************************************
void CBCGPMenuBar::EnableHelpCombobox (UINT uiID /* 0 - disable */, LPCTSTR lpszPrompt /* = NULL */, int nComboBoxWidth/* = 150*/)
{
	ASSERT_VALID (this);

	m_nHelpComboID = uiID;
	m_nHelpComboWidth = nComboBoxWidth;
	m_strHelpComboPrompt = lpszPrompt == NULL ? _T("") : lpszPrompt;

	if (GetSafeHwnd () != NULL)
	{
		AdjustLayout ();
		RedrawWindow ();
	}
}
//**********************************************************************************
CBCGPToolbarComboBoxButton* CBCGPMenuBar::GetHelpCombobox ()
{
	if (m_nHelpComboID == 0)
	{
		return NULL;
	}

	int nHelpComboIndex = CommandToIndex (m_nHelpComboID);
	if (nHelpComboIndex <= 0)
	{
		return NULL;
	}

	CBCGPToolbarComboBoxButton* pCombobox = DYNAMIC_DOWNCAST (CBCGPToolbarComboBoxButton,
		GetButton (nHelpComboIndex));
	return pCombobox;
}
