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

// BCGFrameImpl.cpp: implementation of the CBCGPFrameImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "multimon.h"

#include "BCGPFrameImpl.h"
#include "BCGPToolBar.h"
#include "BCGPMenuBar.h"
#include "BCGPLocalResource.h"
#include "bcgprores.h"
#include "BCGPPopupMenu.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPWorkspace.h"
#include "RegPath.h"
#include "BCGPRegistry.h"
#include "BCGPTearOffManager.h"
#include "BCGPVisualManager.h"
#include "BCGPDockBar.h"
#include "BCGPKeyboardManager.h"
#include "BCGPMiniFrameWnd.h"
#include "BCGPPrintPreviewView.h"
#include "BCGPCustomizeMenuButton.h"
#include "CustomizeButton.h"
#include "BCGPToolbarCustomize.h"
#include "BCGPVisualManager.h"
#include "BCGPVisualManager.h"
#include "BCGPDropDown.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPFrameWnd.h"
#include "BCGPOleIPFrameWnd.h"
#include "BCGPOleDocIPFrameWnd.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CObList	gAllToolbars;
extern CBCGPWorkspace* g_pWorkspace;

class CCustomizeButton;

static const CString strTearOffBarsRegEntry = _T("ControlBars-TearOff");

BOOL CBCGPFrameImpl::m_bControlBarExtraPixel = TRUE;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#pragma warning (disable : 4355)

CBCGPFrameImpl::CBCGPFrameImpl(CFrameWnd* pFrame) :
	m_pFrame (pFrame),
	m_pDockManager (NULL),
	m_uiUserToolbarFirst ((UINT)-1),
	m_uiUserToolbarLast ((UINT)-1),
	m_pMenuBar (NULL),
	m_hDefaultMenu (NULL),
	m_nIDDefaultResource (0),
	m_FullScreenMgr(this),
	m_bLoadDockState(TRUE)
{
	ASSERT_VALID (m_pFrame);
}

#pragma warning (default : 4355)

//**************************************************************************************
CBCGPFrameImpl::~CBCGPFrameImpl()
{
	//-----------------------------
	// Clear user-defined toolbars:
	//-----------------------------
	while (!m_listUserDefinedToolbars.IsEmpty ())
	{
		delete m_listUserDefinedToolbars.RemoveHead ();
	}

	//-------------------------
	// Clear tear-off toolbars:
	//-------------------------
	while (!m_listTearOffToolbars.IsEmpty ())
	{
		delete m_listTearOffToolbars.RemoveHead ();
	}
}
//**************************************************************************************
void CBCGPFrameImpl::OnCloseFrame()
{
	ASSERT_VALID (m_pFrame);

	//----------------------------------------------------------------------
	// Automatically load state and frame position if CBCGPWorkspace is used:
	//----------------------------------------------------------------------
	if (g_pWorkspace != NULL)
	{
		
		if (m_FullScreenMgr.IsFullScreen())
		{
			if(::IsWindow (m_pFrame->GetSafeHwnd ()))
			{
				m_FullScreenMgr.RestoreState(m_pFrame);
			}
		}

		g_pWorkspace->OnClosingMainFrame (this);

		//---------------------------
		// Store the Windowplacement:
		//---------------------------
		if (::IsWindow (m_pFrame->GetSafeHwnd ()))
		{
			WINDOWPLACEMENT wp;
			wp.length = sizeof (WINDOWPLACEMENT);

			if (m_pFrame->GetWindowPlacement (&wp))
			{
				//---------------------------
				// Make sure we don't pop up 
				// minimized the next time
				//---------------------------
				if (wp.showCmd != SW_SHOWMAXIMIZED)
				{
					wp.showCmd = SW_SHOWNORMAL;
				}

				RECT rectDesktop;
				SystemParametersInfo(SPI_GETWORKAREA,0,(PVOID)&rectDesktop,0);
				OffsetRect(&wp.rcNormalPosition, rectDesktop.left, rectDesktop.top);
      
    			g_pWorkspace->StoreWindowPlacement (
					wp.rcNormalPosition, wp.flags, wp.showCmd);
			}
		}
	}
}
//**************************************************************************************
void CBCGPFrameImpl::RestorePosition(CREATESTRUCT& cs)
{
	if (g_pWorkspace != NULL &&
		cs.hInstance != NULL)	
	{
		CRect rectNormal (CPoint (cs.x, cs.y), CSize (cs.cx, cs.cy));
		int nFlags = 0;
		int nShowCmd = SW_SHOWNORMAL;

		if (!g_pWorkspace->LoadWindowPlacement (rectNormal, nFlags, nShowCmd))
		{
			return;
		}

		if (nShowCmd != SW_MAXIMIZE)
		{
			nShowCmd = SW_SHOWNORMAL;
		}

		switch (AfxGetApp()->m_nCmdShow)
		{
		case SW_MAXIMIZE:
		case SW_MINIMIZE:
		case SW_SHOWMINIMIZED:
		case SW_SHOWMINNOACTIVE:
			break;	// don't change!

		default:
			AfxGetApp()->m_nCmdShow = nShowCmd;
		}

		CRect rectDesktop;
		CRect rectInter;

		MONITORINFO mi;
		mi.cbSize = sizeof (MONITORINFO);
		if (GetMonitorInfo (MonitorFromPoint (rectNormal.TopLeft (), 
			MONITOR_DEFAULTTONEAREST), &mi))
		{
			rectDesktop = mi.rcWork;
		}
		else
		{
			::SystemParametersInfo (SPI_GETWORKAREA, 0, &rectDesktop, 0);
		}

		if (nShowCmd == SW_MAXIMIZE)
		{
			cs.x = rectDesktop.left;
			cs.y = rectDesktop.top;
			cs.cx = rectDesktop.Width ();
			cs.cy = rectDesktop.Height ();

			return;
		}

		if (rectInter.IntersectRect (&rectDesktop, &rectNormal))
		{
			cs.x = rectInter.left;
			cs.y = rectInter.top;
			cs.cx = rectNormal.Width ();
			cs.cy = rectNormal.Height ();
		}
	}
}
//**************************************************************************************
void CBCGPFrameImpl::OnLoadFrame()
{
	//---------------------------------------------------
	// Automatically load state if CBCGPWorkspace is used:
	//---------------------------------------------------
	if (g_pWorkspace != NULL)
	{
		g_pWorkspace->LoadState (0, this);
	}
}
//**************************************************************************************
void CBCGPFrameImpl::LoadUserToolbars ()
{
	ASSERT_VALID (m_pFrame);

	if (m_uiUserToolbarFirst == (UINT) -1 ||
		m_uiUserToolbarLast == (UINT) -1)
	{
		return;
	}

	for (UINT uiNewToolbarID = m_uiUserToolbarFirst;
		uiNewToolbarID <= m_uiUserToolbarLast;
		uiNewToolbarID ++)
	{
		CBCGPToolBar* pNewToolbar = new CBCGPToolBar;
		if (!pNewToolbar->Create (m_pFrame, 
			dwDefaultToolbarStyle,
			uiNewToolbarID))
		{
			TRACE0 ("Failed to create a new toolbar!\n");
			delete pNewToolbar;
			continue;
		}

		if (!pNewToolbar->LoadState (m_strControlBarRegEntry))
		{
			pNewToolbar->DestroyWindow ();
			delete pNewToolbar;
		}
		else
		{
			pNewToolbar->SetBarStyle (pNewToolbar->GetBarStyle () |
				CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
			pNewToolbar->EnableDocking (CBRS_ALIGN_ANY);

			ASSERT_VALID (m_pDockManager);
			m_pDockManager->DockControlBar (pNewToolbar);
			m_listUserDefinedToolbars.AddTail (pNewToolbar);
		}
	}
}
//**********************************************************************************************
void CBCGPFrameImpl::SaveUserToolbars (BOOL bFrameBarsOnly)
{
	for (POSITION pos = m_listUserDefinedToolbars.GetHeadPosition (); pos != NULL;)
	{
		CBCGPToolBar* pUserToolBar = 
			(CBCGPToolBar*) m_listUserDefinedToolbars.GetNext (pos);
		ASSERT_VALID(pUserToolBar);

		if (!bFrameBarsOnly || pUserToolBar->GetTopLevelFrame () == m_pFrame)
		{
			pUserToolBar->SaveState (m_strControlBarRegEntry);
		}
	}
}
//**********************************************************************************************
CBCGPToolBar* CBCGPFrameImpl::GetUserBarByIndex (int iIndex) const
{
	POSITION pos = m_listUserDefinedToolbars.FindIndex (iIndex);
	if (pos == NULL)
	{
		return NULL;
	}

	CBCGPToolBar* pUserToolBar = 
		(CBCGPToolBar*) m_listUserDefinedToolbars.GetAt (pos);
	ASSERT_VALID (pUserToolBar);

	return pUserToolBar;
}
//**********************************************************************************************
BOOL CBCGPFrameImpl::IsUserDefinedToolbar (const CBCGPToolBar* pToolBar) const
{
	ASSERT_VALID (pToolBar);

	UINT uiCtrlId = pToolBar->GetDlgCtrlID ();
	return	uiCtrlId >= m_uiUserToolbarFirst &&
			uiCtrlId <= m_uiUserToolbarLast;
}
//*******************************************************************************************
BOOL CBCGPFrameImpl::IsDockStateValid (const CDockState& /*state*/)
{
	ASSERT_VALID (m_pFrame);
	return TRUE;
}
//**********************************************************************************
void CBCGPFrameImpl::InitUserToolbars (	LPCTSTR lpszRegEntry,
										UINT uiUserToolbarFirst, 
										UINT uiUserToolbarLast)
{
	ASSERT (uiUserToolbarLast >= uiUserToolbarFirst);

	if (uiUserToolbarFirst == (UINT) -1 ||
		uiUserToolbarLast == (UINT) -1)
	{
		ASSERT (FALSE);
		return;
	}

	m_uiUserToolbarFirst = uiUserToolbarFirst;
	m_uiUserToolbarLast = uiUserToolbarLast;

	// ET: get Path automatically from workspace if needed
	m_strControlBarRegEntry = (lpszRegEntry == NULL) ? 
		( g_pWorkspace ? g_pWorkspace->GetRegSectionPath() : _T("") )
		: lpszRegEntry;
}
//**************************************************************************************
UINT CBCGPFrameImpl::GetFreeCtrlBarID (UINT uiFirstID, UINT uiLastID, const CObList& lstCtrlBars)
{
	if (uiFirstID == (UINT)-1 || uiLastID == (UINT)-1)
	{
		return 0;
	}

	int iMaxToolbars = uiLastID - uiFirstID + 1;
	if (lstCtrlBars.GetCount () == iMaxToolbars)
	{
		return 0;
	}

	for (UINT uiNewToolbarID = uiFirstID; uiNewToolbarID <= uiLastID;
		uiNewToolbarID ++)
	{
		BOOL bUsed = FALSE;

		for (POSITION pos = lstCtrlBars.GetHeadPosition (); 
			!bUsed && pos != NULL;)
		{
			CBCGPToolBar* pToolBar = (CBCGPToolBar*) lstCtrlBars.GetNext (pos);
			ASSERT_VALID (pToolBar);

			bUsed = (pToolBar->GetDlgCtrlID () == (int) uiNewToolbarID);
		}

		if (!bUsed)
		{
			return uiNewToolbarID;
		}
	}

	return 0;
}
//**************************************************************************************
const CBCGPToolBar* CBCGPFrameImpl::CreateNewToolBar (LPCTSTR lpszName)
{
	ASSERT_VALID (m_pFrame);
	ASSERT (lpszName != NULL);

	UINT uiNewToolbarID = 
		GetFreeCtrlBarID (m_uiUserToolbarFirst, m_uiUserToolbarLast, m_listUserDefinedToolbars);

	if (uiNewToolbarID == 0)
	{
		CBCGPLocalResource locaRes;

		CString strError;
		strError.Format (IDS_BCGBARRES_TOO_MANY_TOOLBARS_FMT, 
			m_uiUserToolbarLast - m_uiUserToolbarFirst + 1);

		AfxMessageBox (strError, MB_OK | MB_ICONASTERISK);
		return NULL;
	}

	CBCGPToolBar* pNewToolbar = new CBCGPToolBar;
	if (!pNewToolbar->Create (m_pFrame,
		dwDefaultToolbarStyle,
		uiNewToolbarID))
	{
		TRACE0 ("Failed to create a new toolbar!\n");
		delete pNewToolbar;
		return NULL;
	}

	pNewToolbar->SetWindowText (lpszName);

	pNewToolbar->SetBarStyle (pNewToolbar->GetBarStyle () |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	pNewToolbar->EnableDocking (CBRS_ALIGN_ANY);

	CRect rectBar;
	pNewToolbar->GetWindowRect (rectBar);
	int nLeft = ::GetSystemMetrics (SM_CXFULLSCREEN) / 2;
	int nTop  = ::GetSystemMetrics (SM_CYFULLSCREEN) / 2;

	CRect rectFloat (nLeft, nTop, nLeft + rectBar.Width (), nTop + rectBar.Height ());
	pNewToolbar->FloatControlBar (rectFloat, DM_UNKNOWN);
	pNewToolbar->m_nMRUWidth = 32767;
	m_pFrame->RecalcLayout ();

	m_listUserDefinedToolbars.AddTail (pNewToolbar);
	return pNewToolbar;
}
//**************************************************************************************
void CBCGPFrameImpl::AddTearOffToolbar (CBCGPBaseControlBar* pToolBar)
{
	ASSERT_VALID (pToolBar);
	m_listTearOffToolbars.AddTail (pToolBar);
}
//**************************************************************************************
void CBCGPFrameImpl::RemoveTearOffToolbar (CBCGPBaseControlBar* pToolBar)
{
	ASSERT_VALID (pToolBar);

	POSITION pos = m_listTearOffToolbars.Find (pToolBar);
	if (pos != NULL)
	{
		m_listTearOffToolbars.RemoveAt (pos);
	}
}
//**************************************************************************************
void CBCGPFrameImpl::LoadTearOffMenus ()
{
	ASSERT_VALID (m_pFrame);

	//------------------------------
	// Remove current tear-off bars:
	//------------------------------
	for (POSITION pos = m_listTearOffToolbars.GetHeadPosition (); pos != NULL;)
	{
		CBCGPBaseControlBar* pBar = (CBCGPBaseControlBar*) m_listTearOffToolbars.GetNext (pos);
		ASSERT_VALID (pBar);

		if (pBar->IsDocked ())
		{
			pBar->UnDockControlBar (TRUE);
		}
		
		pBar->DestroyWindow ();
		delete pBar;
	}
 
	m_listTearOffToolbars.RemoveAll ();

	CString strProfileName = g_pWorkspace != NULL ?
		g_pWorkspace->GetRegSectionPath() : _T("");

	strProfileName += strTearOffBarsRegEntry;

	for (int iIndex = 0;; iIndex++)
	{
		CString strKey;
		strKey.Format (_T("%s-%d"), strProfileName, iIndex);

		int iId = 0;
		CBCGPToolBar* pToolBar = NULL;
		CString strName;

		CBCGPRegistrySP regSP;
		CBCGPRegistry& reg = regSP.Create (FALSE, TRUE);

		if (!reg.Open (strKey) ||
			!reg.Read (_T("ID"), iId) ||
			!reg.Read (_T("Name"), strName) ||
			!reg.Read (_T("State"), (CObject*&) pToolBar))
		{
			break;
		}

		ASSERT_VALID (pToolBar);

		if (!pToolBar->Create (m_pFrame,
			dwDefaultToolbarStyle,
			(UINT) iId))
		{
			TRACE0 ("Failed to create a new toolbar!\n");
			delete pToolBar;
			break;
		}

		pToolBar->SetWindowText (strName);

		pToolBar->SetBarStyle (pToolBar->GetBarStyle () |
			CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		pToolBar->EnableDocking (CBRS_ALIGN_ANY);

		ASSERT_VALID (m_pDockManager);
		m_listTearOffToolbars.AddTail (pToolBar);
		pToolBar->LoadState (strProfileName, iIndex);
		m_pDockManager->DockControlBar (pToolBar);
	}
}
//**************************************************************************************
void CBCGPFrameImpl::SaveTearOffMenus (BOOL bFrameBarsOnly)
{
	CString strProfileName = g_pWorkspace != NULL ?
		g_pWorkspace->GetRegSectionPath() : _T("");
	strProfileName += strTearOffBarsRegEntry;

	//------------------------------------------------
	// First, clear old tear-off toolbars in registry:
	//------------------------------------------------
	for (int iIndex = 0;; iIndex++)
	{
		CString strKey;
		strKey.Format (_T("%s-%d"), strProfileName, iIndex);

		CBCGPRegistrySP regSP;
		CBCGPRegistry& reg = regSP.Create (FALSE, FALSE);

		if (!reg.DeleteKey (strKey))
		{
			break;
		}
	}

	iIndex = 0;

	for (POSITION pos = m_listTearOffToolbars.GetHeadPosition (); pos != NULL; iIndex ++)
	{
		CBCGPBaseControlBar* pBar = (CBCGPBaseControlBar*) m_listTearOffToolbars.GetNext (pos);
		ASSERT_VALID (pBar);

		if ((!bFrameBarsOnly || pBar->GetTopLevelFrame () == m_pFrame) &&
			pBar->IsBarVisible ())
		{
			CString strName;
			pBar->GetWindowText (strName);

			CString strKey;
			strKey.Format (_T("%s-%d"), strProfileName, iIndex);

			CBCGPRegistrySP regSP;
			CBCGPRegistry& reg = regSP.Create (FALSE, FALSE);

			reg.CreateKey (strKey);

			reg.Write (_T("ID"), (int) pBar->GetDlgCtrlID ());
			reg.Write (_T("Name"), strName);
			reg.Write (_T("State"), pBar);
			pBar->SaveState (strProfileName, iIndex);
		}
	}
}
//**************************************************************************************
BOOL CBCGPFrameImpl::DeleteToolBar (CBCGPToolBar* pToolBar)
{
	ASSERT_VALID (m_pFrame);
	ASSERT_VALID (pToolBar);

	POSITION pos = m_listUserDefinedToolbars.Find (pToolBar);
	if (pos == NULL)
	{
		return FALSE;
	}

	m_listUserDefinedToolbars.RemoveAt (pos);
	pToolBar->RemoveStateFromRegistry (m_strControlBarRegEntry);

	CBCGPDockBar* pParentDockBar = pToolBar->GetParentDockBar ();
	CBCGPMiniFrameWnd* pParentMiniFrame = pToolBar->GetParentMiniFrame ();
	if (pParentDockBar != NULL)
	{
		ASSERT_VALID (pParentDockBar);
		pParentDockBar->RemoveControlBar (pToolBar, DM_UNKNOWN);
	}
	else if (pParentMiniFrame != NULL)
	{
		ASSERT_VALID (pParentMiniFrame);
		pParentMiniFrame->RemoveControlBar (pToolBar);
	}

	pToolBar->DestroyWindow ();
	delete pToolBar;

	m_pFrame->RecalcLayout ();
	return TRUE;
}
//*******************************************************************************************
void CBCGPFrameImpl::SetMenuBar (CBCGPMenuBar* pMenuBar)
{
	ASSERT_VALID (m_pFrame);
	ASSERT_VALID (pMenuBar);
	ASSERT (m_pMenuBar == NULL);	// Method should be called once!

	m_pMenuBar = pMenuBar;

	m_hDefaultMenu=*m_pFrame->GetMenu();

	// ET: Better support for dynamic menu
	m_pMenuBar->OnDefaultMenuLoaded (m_hDefaultMenu);
	m_pMenuBar->CreateFromMenu (m_hDefaultMenu, TRUE /* Default menu */);

	m_pFrame->SetMenu (NULL);
	
	m_pMenuBar->SetDefaultMenuResId (m_nIDDefaultResource);
}
//*******************************************************************************************
BOOL CBCGPFrameImpl::ProcessKeyboard (int nKey, BOOL* pbProcessAccel)
{
	ASSERT_VALID (m_pFrame);


	if (pbProcessAccel != NULL)
	{
		*pbProcessAccel = TRUE;
	}

	//--------------------------------------------------------
	// If popup menu is active, pass keyboard control to menu:
	//--------------------------------------------------------
	if (CBCGPPopupMenu::m_pActivePopupMenu != NULL &&
		::IsWindow (CBCGPPopupMenu::m_pActivePopupMenu->m_hWnd))
	{
		CBCGPPopupMenu::m_pActivePopupMenu->SendMessage (WM_KEYDOWN, nKey);
		return TRUE;
	}

	//------------------------------------------
	// If appication is minimized, don't handle
	// any keyboard accelerators:
	//------------------------------------------
	if (m_pFrame->IsIconic ())
	{
		return TRUE;
	}

	//----------------------------------------------------------
	// Don't handle keybaord accererators in customization mode:
	//----------------------------------------------------------
	if (CBCGPToolBar::IsCustomizeMode ())
	{
		return FALSE;
	}

	//-----------------------------------------------------
	// Is any toolbar control (such as combobox) has focus?
	//-----------------------------------------------------
	BOOL bIsToolbarCtrlFocus = FALSE;
	for (POSITION posTlb = gAllToolbars.GetHeadPosition (); 
		!bIsToolbarCtrlFocus && posTlb != NULL;)
	{
		CBCGPToolBar* pToolBar = (CBCGPToolBar*) gAllToolbars.GetNext (posTlb);
		ASSERT (pToolBar != NULL);

		if (CWnd::FromHandlePermanent (pToolBar->m_hWnd) != NULL)
		{
			ASSERT_VALID(pToolBar);

			for (int i = 0; i < pToolBar->GetCount (); i++)
			{
				CBCGPToolbarButton* pButton = pToolBar->GetButton (i);
				ASSERT_VALID (pButton);

				if (pButton->HasFocus ())
				{
					bIsToolbarCtrlFocus = TRUE;
					break;
				}
			}
		}
	}

	//-------------------------------------
	// Check for the keyboard accelerators:
	//-------------------------------------
	BYTE fVirt = 0;

	if (::GetAsyncKeyState (VK_CONTROL) & 0x8000)
	{
		fVirt |= FCONTROL;
	}

	if (::GetAsyncKeyState (VK_MENU) & 0x8000)
	{
		fVirt |= FALT;
	}

	if (::GetAsyncKeyState (VK_SHIFT) & 0x8000)
	{
		fVirt |= FSHIFT;
	}

	if (!bIsToolbarCtrlFocus)
	{
		if (CBCGPKeyboardManager::IsKeyHandled ((WORD) nKey, (BYTE)(fVirt | FVIRTKEY), 
												m_pFrame, TRUE) ||
			CBCGPKeyboardManager::IsKeyHandled ((WORD) nKey, (BYTE)(fVirt | FVIRTKEY), 
												m_pFrame->GetActiveFrame (), FALSE))
		{
			return FALSE;
		}
	}

	if (fVirt == FALT)
	{
		//--------------------------------------------
		// Handle menu accelerators (such as "Alt-F"):
		//--------------------------------------------
		if (OnMenuChar (nKey))
		{
			return TRUE;
		}
	}

	if (bIsToolbarCtrlFocus && pbProcessAccel != NULL)
	{
		//---------------------------------------------
		// Don't process default keyboard accelerators:
		//---------------------------------------------
		*pbProcessAccel = FALSE;
	}

	return FALSE;
}
//*******************************************************************************************
BOOL CBCGPFrameImpl::ProcessMouseClick (UINT uiMsg, POINT pt, HWND hwnd)
{
	ASSERT_VALID (m_pFrame);

	//------------------------------------------------
	// Maybe user start drag the button with control?
	//------------------------------------------------
	if (uiMsg == WM_LBUTTONDOWN &&
		(CBCGPToolBar::IsCustomizeMode () ||
		(::GetAsyncKeyState (VK_MENU) & 0x8000)))	// ALT is pressed
	{
		for (POSITION posTlb = gAllToolbars.GetHeadPosition (); posTlb != NULL;)
		{
			CBCGPToolBar* pToolBar = (CBCGPToolBar*) gAllToolbars.GetNext (posTlb);
			ASSERT (pToolBar != NULL);

			if (CWnd::FromHandlePermanent (pToolBar->m_hWnd) != NULL)
			{
				ASSERT_VALID(pToolBar);

				CPoint ptToolBar = pt;
				pToolBar->ScreenToClient (&ptToolBar);

				int iHit = pToolBar->HitTest (ptToolBar);
				if (iHit >= 0)
				{
					CBCGPToolbarButton* pButton = pToolBar->GetButton (iHit);
					ASSERT_VALID (pButton);

					if (pButton->GetHwnd () != NULL &&
						pButton->GetHwnd () == hwnd &&
						pButton->Rect ().PtInRect (ptToolBar))
					{
						pToolBar->SendMessage (WM_LBUTTONDOWN, 
							0, MAKELPARAM (ptToolBar.x, ptToolBar.y));
						return TRUE;
					}

					break;
				}
			}
		}
	}

	if (!CBCGPToolBar::IsCustomizeMode () &&
		CBCGPPopupMenu::m_pActivePopupMenu != NULL &&
		::IsWindow (CBCGPPopupMenu::m_pActivePopupMenu->m_hWnd))
	{
		CBCGPPopupMenu::MENUAREA_TYPE clickArea = CBCGPPopupMenu::m_pActivePopupMenu->CheckArea (pt);

		if (clickArea == CBCGPPopupMenu::OUTSIDE)
		{
			// Click outside of menu

			//--------------------------------------------
			// Maybe secondary click on the parent button?
			//--------------------------------------------
			CBCGPToolbarMenuButton* pParentButton = 
				CBCGPPopupMenu::m_pActivePopupMenu->GetParentButton ();
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
							!CBCGPPopupMenu::m_pActivePopupMenu->InCommand ())
						{
							//----------------------------------------
							// Toolbar/menu bar: close an active menu!
							//----------------------------------------
							CBCGPPopupMenu::m_pActivePopupMenu->SendMessage (WM_CLOSE);
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
								m_pFrame->SetFocus ();

								return TRUE;
							}
						}
					}
				}
			}

			if (!CBCGPPopupMenu::m_pActivePopupMenu->InCommand ())
			{
				CBCGPPopupMenu::m_pActivePopupMenu->SendMessage (WM_CLOSE);

				CWnd* pWndFocus = CWnd::GetFocus ();
				if (pWndFocus != NULL && pWndFocus->IsKindOf (RUNTIME_CLASS (CBCGPToolBar)))
				{
					m_pFrame->SetFocus ();
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
			CBCGPPopupMenu::m_pActivePopupMenu->SendMessage (WM_CLOSE);
			m_pFrame->SetFocus ();

			return TRUE;
		}
	}

	return FALSE;
}
//*******************************************************************************************
BOOL CBCGPFrameImpl::ProcessMouseMove (POINT pt)
{
	if (!CBCGPToolBar::IsCustomizeMode () &&
		CBCGPPopupMenu::m_pActivePopupMenu != NULL)
	{
		CRect rectMenu;
		CBCGPPopupMenu::m_pActivePopupMenu->GetWindowRect (rectMenu);

		if (rectMenu.PtInRect (pt) ||
			CBCGPPopupMenu::m_pActivePopupMenu->GetMenuBar ()->FindDestBar (pt) != NULL)
		{
			return FALSE;	// Default processing
		}

		return TRUE;		// Active menu "capturing"
	}

	return FALSE;	// Default processing
}
//*******************************************************************************************
BOOL CBCGPFrameImpl::OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup, CFrameWnd* /*pWndFrame*/)
{
	CBCGPPopupMenu::m_pActivePopupMenu = pMenuPopup;

	if (pMenuPopup != NULL && IsCustomizePane(pMenuPopup))
    {
		ShowQuickCustomizePane(pMenuPopup);
	}

	if (pMenuPopup != NULL && !CBCGPToolBar::IsCustomizeMode ())
	{
		ASSERT_VALID (pMenuPopup);

		CBCGPBaseControlBar* pTopLevelBar = NULL;

		for (CBCGPPopupMenu* pMenu = pMenuPopup; pMenu != NULL;
			pMenu = pMenu->GetParentPopupMenu ())
		{
			CBCGPToolbarMenuButton* pParentButton = pMenu->GetParentButton ();
			if (pParentButton == NULL)
			{
				break;
			}
		
			pTopLevelBar = 
				DYNAMIC_DOWNCAST (CBCGPBaseControlBar, pParentButton->GetParentWnd ());
		}

		if (pTopLevelBar != NULL && 
			!pTopLevelBar->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar)))
		{
			ASSERT_VALID (pTopLevelBar);

			if (pTopLevelBar->IsDocked () &&
				::GetFocus () != pTopLevelBar->GetSafeHwnd () &&
				CBCGPPopupMenu::GetForceMenuFocus ())
			{
				pTopLevelBar->SetFocus ();
			}
		}
	}

	return TRUE;
}
//****************************************************************************************
void CBCGPFrameImpl::SetupToolbarMenu (CMenu& menu, 
									  const UINT uiViewUserToolbarCmdFirst,
									  const UINT uiViewUserToolbarCmdLast)
{
	//---------------------------------------------------------------
	// Replace toolbar dummy items to the user-defined toolbar names:
	//---------------------------------------------------------------
	for (int i = 0; i < (int) menu.GetMenuItemCount ();)
	{
		UINT uiCmd = menu.GetMenuItemID (i);

		if (uiCmd >= uiViewUserToolbarCmdFirst && 
			uiCmd <= uiViewUserToolbarCmdLast)
		{
			//-------------------------------------------------------------------
			// "User toolbar" item. First check that toolbar number 'x' is exist:
			//-------------------------------------------------------------------
			CBCGPToolBar* pToolBar = GetUserBarByIndex (uiCmd - uiViewUserToolbarCmdFirst);
			if (pToolBar != NULL)
			{
				ASSERT_VALID (pToolBar);

				//-----------------------------------------------------------
				// Modify the current menu item text to the toolbar title and
				// move next:
				//-----------------------------------------------------------
				CString strToolbarName;
				pToolBar->GetWindowText (strToolbarName);

				menu.ModifyMenu (i ++, MF_BYPOSITION | MF_STRING, uiCmd, strToolbarName);
			}
			else
			{
				menu.DeleteMenu (i, MF_BYPOSITION);
			}
		}
		else	// Not "user toolbar" item, move next
		{
			i ++;
		}
	}
}
//********************************************************************************
BOOL CBCGPFrameImpl::OnMenuChar (UINT nChar)
{
	ASSERT_VALID (m_pFrame);

	BOOL bInPrintPreview = 
		m_pDockManager != NULL && m_pDockManager->IsPrintPreviewValid ();

	if (!bInPrintPreview)
	{
		if (m_pMenuBar != NULL &&
			(m_pMenuBar->GetStyle () & WS_VISIBLE) &&
			m_pMenuBar->TranslateChar (nChar))
		{
			return TRUE;
		}
	}

	for (POSITION posTlb = gAllToolbars.GetHeadPosition (); posTlb != NULL;)
	{
		CBCGPToolBar* pToolBar = (CBCGPToolBar*) gAllToolbars.GetNext (posTlb);
		ASSERT (pToolBar != NULL);

		if (bInPrintPreview && !pToolBar->IsKindOf (RUNTIME_CLASS (CBCGPPrintPreviewToolBar)))
		{
			continue;
		}

		if (CWnd::FromHandlePermanent (pToolBar->m_hWnd) != NULL &&
			pToolBar != m_pMenuBar &&
			(pToolBar->GetStyle () & WS_VISIBLE) &&
			pToolBar->GetTopLevelFrame () == m_pFrame &&
			pToolBar->TranslateChar (nChar))
		{
			return TRUE;
		}
	}

	return FALSE;
}
//************************************************************************************
void CBCGPFrameImpl::SaveDockState (LPCTSTR lpszSectionName)
{
	if (m_pDockManager != NULL)
	{
		m_pDockManager->SaveState (lpszSectionName, m_nIDDefaultResource);
	}
}
//************************************************************************************
void CBCGPFrameImpl::LoadDockState (LPCTSTR lpszSectionName)
{
	if (m_pDockManager != NULL && m_bLoadDockState)
	{
		m_pDockManager->LoadState (lpszSectionName, m_nIDDefaultResource);
	}
}

//************************************************************************************
void CBCGPFrameImpl::SetDockState(const CDockState& /*state*/)
{
	ASSERT_VALID (m_pFrame);
	ASSERT_VALID (m_pDockManager);

	if (m_pDockManager != NULL)
	{
		m_pDockManager->SetDockState ();
	}
}
//**************************************************************************************
BOOL CBCGPFrameImpl::IsHelpKey (LPMSG lpMsg)
{
	return lpMsg->message == WM_KEYDOWN &&
		   lpMsg->wParam == VK_F1 &&
		   !(HIWORD(lpMsg->lParam) & KF_REPEAT) &&
		   GetKeyState(VK_SHIFT) >= 0 &&
		   GetKeyState(VK_CONTROL) >= 0 &&
		   GetKeyState(VK_MENU) >= 0;
}
//***************************************************************************************
void CBCGPFrameImpl::DeactivateMenu ()
{
	if (!CBCGPToolBar::IsCustomizeMode () &&
		CBCGPPopupMenu::m_pActivePopupMenu != NULL)
	{
		if (m_pMenuBar != NULL)
		{
			m_pMenuBar->Deactivate ();
		}
	}
}
//***************************************************************************************
BOOL CBCGPFrameImpl::LoadLargeIconsState ()
{
	if (g_pWorkspace != NULL)
	{
		return CBCGPToolBar::LoadLargeIconsState (g_pWorkspace->GetRegSectionPath ());
	}
	else
	{
		return FALSE;
	}
}
//*********************************************************************************
void CBCGPFrameImpl::ShowQuickCustomizePane(CBCGPPopupMenu* pMenuPopup)
{
	//---------------------------
	// Get Actual toolbar pointer
	//---------------------------
	CBCGPToolBar* pWndParentToolbar = NULL;

	CBCGPPopupMenu* pPopupLevel2 = pMenuPopup->GetParentPopupMenu();
	if (pPopupLevel2 == NULL)
	{
		return;
	}
	
	CBCGPPopupMenu* pPopupLevel1 = pPopupLevel2->GetParentPopupMenu();
	if (pPopupLevel1 == NULL)
	{
		return;
	}

	CCustomizeButton* pCustom = (CCustomizeButton*)pPopupLevel1->GetParentButton();
	if (pCustom == NULL)
	{
		//May be MiniFrameWnd
		CWnd* pFrame = pPopupLevel1->GetOwner();
		if (pFrame == NULL)
		{
			return;
		}

		if (pFrame->IsKindOf(RUNTIME_CLASS(CBCGPMiniFrameWnd)))
		{
			CBCGPMiniFrameWnd* pMinFrm = (CBCGPMiniFrameWnd*)pFrame;

			pWndParentToolbar = (CBCGPToolBar*)pMinFrm->GetControlBar();

		}else
		{
			 return;
		}
	}
	else
	{
		if (!pCustom->IsKindOf(RUNTIME_CLASS(CCustomizeButton)))
		{
			return;
		}

		CBCGPToolBar* pCurrentToolBar = pCustom->GetParentToolbar ();

		CBCGPToolbarMenuButton* btnDummy = pMenuPopup->GetMenuItem (0);
		int nID = _ttoi (btnDummy->m_strText);

		const CObList& gAllToolbars = CBCGPToolBar::GetAllToolbars ();
	
		CBCGPToolBar* pRealToolBar = NULL;
		for (POSITION pos = gAllToolbars.GetHeadPosition (); pos != NULL;)
		{
			pRealToolBar = (CBCGPToolBar*) gAllToolbars.GetNext (pos);
			ASSERT (pRealToolBar != NULL);
			if (nID == pRealToolBar->GetDlgCtrlID ())
			{
				break;
			}
			
			pRealToolBar = NULL;
		}

		if (pRealToolBar == NULL)
		{
			pWndParentToolbar = pCurrentToolBar;
		}
		else
		{
			pWndParentToolbar = pRealToolBar;
		}
	}

	pMenuPopup->RemoveAllItems ();

	CBCGPToolbarCustomize* pStdCust = new CBCGPToolbarCustomize(
											m_pFrame,
											TRUE,
											BCGCUSTOMIZE_MENUAMPERS);

	CBCGPCustomizeMenuButton::SetParentToolbar(pWndParentToolbar);

	//--------------------------
	// Populate pop-up menu
	//-------------------------
	UINT uiRealCount = 0;
	CBCGPCustomizeMenuButton::m_mapPresentIDs.RemoveAll();

	UINT uiCount = pWndParentToolbar->GetCount();
	for (UINT i=0; i< uiCount; i++)
	{
		CBCGPToolbarButton* pBtn = pWndParentToolbar->GetButton(i);

		if (pBtn->IsKindOf(RUNTIME_CLASS(CCustomizeButton)) || (pBtn->m_nStyle & TBBS_SEPARATOR))
		{
			continue;
		}

		CBCGPCustomizeMenuButton::m_mapPresentIDs.SetAt(pBtn->m_nID, 0);

		//---------------------------
		//Find Command Text if empty
		//---------------------------
		CString strText = pBtn->m_strText;
		if (pBtn->m_strText.IsEmpty())
		{
			strText = pStdCust->GetCommandName(pBtn->m_nID);
		}

		UINT uiID = pBtn->m_nID;
		if ((pBtn->m_nID == 0) || (pBtn->m_nID == -1))
		{
			uiID = BCGPCUSTOMIZE_INTERNAL_ID;
		}

		CBCGPCustomizeMenuButton button(uiID, NULL, pBtn->GetImage(), strText, pBtn->m_bUserButton);
		button.SetItemIndex(i);
		pMenuPopup->InsertItem(button);

		uiRealCount++;
	}

	delete pStdCust;

	pMenuPopup->SetQuickCustomizeType(CBCGPPopupMenu::QUICK_CUSTOMIZE_PANE);

	//------------------------------------------
	//Give User ability to customize pane
	//-----------------------------------------
	OnShowCustomizePane(pMenuPopup, pWndParentToolbar->GetResourceID());

	if (uiRealCount > 0)
	{
		pMenuPopup->InsertSeparator();
	}

	//--------------------------
	// Add Reset Toolbar Button
	//--------------------------
	CString strCommand;

	{
		CBCGPLocalResource locaRes;
		strCommand.LoadString (IDS_BCGBARRES_RESET_TOOLBAR);
	}

	CBCGPCustomizeMenuButton btnReset (BCGPCUSTOMIZE_INTERNAL_ID, NULL, -1, strCommand, FALSE);
	btnReset.SetItemIndex(ID_BCGBARRES_TOOLBAR_RESET_PROMT);

	pMenuPopup->InsertItem(btnReset);
}
//********************************************************************************
BOOL CBCGPFrameImpl::OnShowCustomizePane(CBCGPPopupMenu* pMenuPane, UINT uiToolbarID)
{
	BOOL bResult = FALSE;

	CBCGPMDIFrameWnd* pMainFrame =
				DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, m_pFrame);

	if (pMainFrame != NULL)
	{
		bResult = pMainFrame->OnShowCustomizePane(pMenuPane, uiToolbarID);
	}
	else	// Maybe, SDI frame...
	{
		CBCGPFrameWnd* pFrame = DYNAMIC_DOWNCAST (CBCGPFrameWnd, m_pFrame);
		if (pFrame != NULL)
		{
			bResult = pFrame->OnShowCustomizePane(pMenuPane, uiToolbarID);

		}else	// Maybe, OLE frame
		{
			CBCGPOleIPFrameWnd* pOleFrame = 
					DYNAMIC_DOWNCAST (CBCGPOleIPFrameWnd, m_pFrame);
			if (pOleFrame != NULL)
			{
				bResult = pOleFrame->OnShowCustomizePane(pMenuPane, uiToolbarID);
			}
			else
			{
				CBCGPOleDocIPFrameWnd* pOleDocFrame = 
					DYNAMIC_DOWNCAST (CBCGPOleDocIPFrameWnd, m_pFrame);
				if (pOleDocFrame != NULL)
				{
					bResult = pOleDocFrame->OnShowCustomizePane(pMenuPane, uiToolbarID);
				}
			}
		}
	}

	return bResult;
}
//********************************************************************************
void CBCGPFrameImpl::AddDefaultButtonsToCustomizePane(
						CBCGPPopupMenu* pMenuPane, UINT /*uiToolbarID*/)
{
	CBCGPToolBar* pWndParentToolbar = CBCGPCustomizeMenuButton::GetParentToolbar();
	
	if (pWndParentToolbar == NULL)
	{
		return;
	}
	
	CBCGPToolbarCustomize* pStdCust = new CBCGPToolbarCustomize(m_pFrame, TRUE, 
		BCGCUSTOMIZE_MENUAMPERS);
	
	const CObList& lstOrigButtons = pWndParentToolbar->GetOrigResetButtons(); 

	int i = 0;
	int nTmp = 0;
	for (POSITION posCurr = lstOrigButtons.GetHeadPosition (); posCurr != NULL; i++)
	{
		CBCGPToolbarButton* pButtonCurr = (CBCGPToolbarButton*)lstOrigButtons.GetNext (posCurr);

		UINT uiID = pButtonCurr->m_nID;

		if ((pButtonCurr == NULL) || 
			(pButtonCurr->m_nStyle & TBBS_SEPARATOR) ||
			(pButtonCurr->IsKindOf(RUNTIME_CLASS(CCustomizeButton))) ||
			 CBCGPCustomizeMenuButton::m_mapPresentIDs.Lookup(uiID, nTmp))
		{
				continue;
		}

		if (pButtonCurr->IsKindOf (RUNTIME_CLASS (CBCGPDropDownToolbarButton)))
		{
			CBCGPDropDownToolbarButton* pDropButton = 
				DYNAMIC_DOWNCAST (CBCGPDropDownToolbarButton, pButtonCurr);

			CBCGPToolBar* pDropToolBar = pDropButton->GetDropDownToolBar ();
			if (pDropToolBar != NULL)
			{
				int nIndex = pDropToolBar->CommandToIndex (uiID);
				if (nIndex != -1)
				{
					continue;
				}
			}
		}

		if (pButtonCurr->IsKindOf (RUNTIME_CLASS (CBCGPToolbarMenuButton)))
		{
			CBCGPToolbarMenuButton* pMenuButton = 
				DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButtonCurr);

			if (pMenuButton->IsMenuPaletteMode ())
			{
				const CObList& lstMenuItems = pMenuButton->GetCommands ();
				BOOL bIsExist = FALSE;

				for (POSITION posCommand = lstMenuItems.GetHeadPosition (); 
					!bIsExist && posCommand != NULL;)
				{
					CBCGPToolbarMenuButton* pMenuItem = (CBCGPToolbarMenuButton*) lstMenuItems.GetNext (posCommand);
					ASSERT_VALID (pMenuItem);

					bIsExist = (pMenuItem->m_nID == uiID);
				}

				if (bIsExist)
				{
					continue;
				}
			}
		}

		if ((pButtonCurr->m_nID == 0) || (pButtonCurr->m_nID == -1))
		{
			uiID = BCGPCUSTOMIZE_INTERNAL_ID;
		}

		CBCGPCustomizeMenuButton button(uiID, NULL, pButtonCurr->GetImage(), 
			pStdCust->GetCommandName(pButtonCurr->m_nID), pButtonCurr->m_bUserButton); 

		button.SetItemIndex(i, FALSE);

		int nIndex = pMenuPane->InsertItem(button, i);
		if (nIndex == -1)
		{
			pMenuPane->InsertItem(button);
		}
	}

	delete pStdCust;
}
//********************************************************************************
BOOL CBCGPFrameImpl::IsCustomizePane(const CBCGPPopupMenu* pMenuPopup) const
{
	CBCGPPopupMenu* pPopupLevel2 = pMenuPopup->GetParentPopupMenu();

	if (pPopupLevel2 == NULL)
	{
		return FALSE;
	}

	CString strLabel;

	{
		CBCGPLocalResource locaRes;
		strLabel.LoadString (IDS_BCGBARRES_ADD_REMOVE_BTNS);
	}

	CBCGPToolbarMenuButton* pButton = pPopupLevel2->GetParentButton ();
	if (pButton != NULL && pButton->m_strText.Find (strLabel) == -1)
	{
		return FALSE;
	}

		
	CBCGPPopupMenu* pPopupLevel1 = pPopupLevel2->GetParentPopupMenu();

	if (pPopupLevel1 == NULL)
	{
		return FALSE;
	}

	if (pPopupLevel1->GetQuickCustomizeType() == CBCGPPopupMenu::QUICK_CUSTOMIZE_ADDREMOVE)
	{
		return TRUE;
	}

	return FALSE;
}
