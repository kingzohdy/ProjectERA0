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

// BCGPContextMenuManager.cpp: implementation of the CBCGPContextMenuManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPContextMenuManager.h"
#include "BCGPTearOffManager.h"
#include "BCGPPopupMenu.h"
#include "MenuHash.h"
#include "BCGGlobals.h"
#include "RegPath.h"
#include "BCGPDialog.h"
#include "BCGPPropertyPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static const CString strMenusProfile = _T("BCGContextMenuManager");

BCGCBPRODLLEXPORT CBCGPContextMenuManager*	g_pContextMenuManager = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPContextMenuManager::CBCGPContextMenuManager()
{
	ASSERT (g_pContextMenuManager == NULL);
	g_pContextMenuManager = this;
	m_nLastCommandID = 0;
	m_bTrackMode = FALSE;
}
//***********************************************************************************************
CBCGPContextMenuManager::~CBCGPContextMenuManager()
{
	for (POSITION pos = m_Menus.GetStartPosition (); pos != NULL;)
	{
		UINT uiResId;
		HMENU hMenu;

		m_Menus.GetNextAssoc (pos, uiResId, hMenu);
		::DestroyMenu (hMenu);
	}

	g_pContextMenuManager = NULL;
}
//**********************************************************************************
BOOL CBCGPContextMenuManager::AddMenu(UINT uiMenuNameResId, UINT uiMenuResId)
{
	CString strMenuName;
	strMenuName.LoadString (uiMenuNameResId);

	return AddMenu (strMenuName, uiMenuResId);
}
//***********************************************************************************************
BOOL CBCGPContextMenuManager::AddMenu(LPCTSTR lpszName, UINT uiMenuResId)
{
	ASSERT (lpszName != NULL);

	CMenu menu;
	if (!menu.LoadMenu (uiMenuResId))
	{
		ASSERT (FALSE);
		return FALSE;
	}

	HMENU hExMenu;
	if (m_Menus.Lookup (uiMenuResId, hExMenu))
	{
		//------------------------------------------
		// Menu with the same name is already exist!
		//------------------------------------------
		return FALSE;
	}

	HMENU hMenu = menu.Detach ();

	if (g_pTearOffMenuManager != NULL)
	{
		g_pTearOffMenuManager->SetupTearOffMenus (hMenu);
	}

	m_Menus.SetAt (uiMenuResId, hMenu);
	m_MenuNames.SetAt (lpszName, hMenu);

	return TRUE;
}
//***********************************************************************************************
BOOL CBCGPContextMenuManager::ShowPopupMenu (UINT uiMenuResId, int x, int y, 
											CWnd* pWndOwner, BOOL bOwnMessage,
											BOOL bRightAlign)
{
	HMENU hMenu;
	if (!m_Menus.Lookup (uiMenuResId, hMenu) || hMenu == NULL)
	{
		return FALSE;
	}

	if (x == -1 && y == -1 &&	// Undefined position
		pWndOwner != NULL)
	{
		CRect rectParent;
		pWndOwner->GetClientRect (&rectParent);
		pWndOwner->ClientToScreen (&rectParent);

		x = rectParent.left + 5;
		y = rectParent.top + 5;
	}

	HMENU hmenuPopup = ::GetSubMenu (hMenu, 0);
	if (hmenuPopup == NULL)
	{
		#ifdef _DEBUG

		MENUITEMINFO info;
		memset (&info, 0, sizeof (MENUITEMINFO));

		if (!::GetMenuItemInfo (hMenu, 0, TRUE, &info))
		{
			TRACE (_T ("Invalid menu: %d\n"), uiMenuResId);
		}
		else
		{
			ASSERT (info.hSubMenu == NULL);
			TRACE (_T ("Menu %d, first option '%s' doesn't contain popup menu!\n"), 
					uiMenuResId, info.dwTypeData);
		}

		#endif // _DEBUG
		return FALSE;
	}

	return ShowPopupMenu (hmenuPopup, x, y, pWndOwner, bOwnMessage,
		TRUE, bRightAlign) != NULL;
}
//***********************************************************************************************
CBCGPPopupMenu* CBCGPContextMenuManager::ShowPopupMenu (HMENU hmenuPopup, int x, int y,
											CWnd* pWndOwner, BOOL bOwnMessage,
											BOOL bAutoDestroy,
											BOOL bRightAlign)
{
	if (pWndOwner != NULL &&
		pWndOwner->IsKindOf (RUNTIME_CLASS (CBCGPDialog)) && !bOwnMessage)
	{
		// BCGDialog should own menu messages
		ASSERT (FALSE);
		return NULL;
	}

	if (pWndOwner != NULL &&
		pWndOwner->IsKindOf (RUNTIME_CLASS (CBCGPPropertyPage)) && !bOwnMessage)
	{
		// CBCGPropertyPage should own menu messages
		ASSERT (FALSE);
		return NULL;
	}

	ASSERT (hmenuPopup != NULL);
	if (g_pTearOffMenuManager != NULL)
	{
		g_pTearOffMenuManager->SetupTearOffMenus (hmenuPopup);
	}

	if (m_bTrackMode)
	{
		bOwnMessage = TRUE;
	}

	if (!bOwnMessage)
	{
		while (pWndOwner != NULL && pWndOwner->GetStyle() & WS_CHILD)
		{
			pWndOwner = pWndOwner->GetParent ();
		}
	}

	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	if (!globalData.bIsWindowsNT4 || 
		bAutoDestroy)
	{
		pPopupMenu->SetAutoDestroy (FALSE);
	}

	pPopupMenu->m_bTrackMode = m_bTrackMode;
	pPopupMenu->SetRightAlign (bRightAlign);

	CBCGPPopupMenu* pMenuActive = CBCGPPopupMenu::GetActiveMenu ();
	if (pMenuActive != NULL)
	{
		pMenuActive->SendMessage (WM_CLOSE);
	}

	if (!pPopupMenu->Create (pWndOwner, x, y, hmenuPopup, FALSE, bOwnMessage))
	{
		return NULL;
	}

	return pPopupMenu;
}
//***********************************************************************************************
UINT CBCGPContextMenuManager::TrackPopupMenu (HMENU hmenuPopup, int x, int y,
											 CWnd* pWndOwner, BOOL bRightAlign)
{
	m_nLastCommandID = 0;

	CWinApp* pApp = AfxGetApp ();
	if (pApp == NULL)
	{
		ASSERT (FALSE);
		return 0;
	}

	m_bTrackMode = TRUE;

	CBCGPPopupMenu* pMenu = ShowPopupMenu (hmenuPopup, x, y, pWndOwner,
		FALSE, TRUE, bRightAlign);

	if (pMenu != NULL)
	{
		CRect rect;
		pMenu->GetWindowRect (&rect);
		pMenu->UpdateShadow (&rect);
	}

	CBCGPDialog* pParentDlg = NULL;
	if (pWndOwner != NULL && pWndOwner->GetParent () != NULL)
	{
		pParentDlg = DYNAMIC_DOWNCAST (CBCGPDialog, pWndOwner->GetParent ());
		if (pParentDlg != NULL)
		{
			pParentDlg->SetActiveMenu (pMenu);
		}
	}

	CBCGPPropertyPage* pParentPropPage = NULL;
	if (pWndOwner != NULL && pWndOwner->GetParent () != NULL)
	{
		pParentPropPage = DYNAMIC_DOWNCAST (CBCGPPropertyPage, pWndOwner->GetParent ());
		if (pParentPropPage != NULL)
		{
			pParentPropPage->SetActiveMenu (pMenu);
		}
	}

	m_bTrackMode = FALSE;

	if (pMenu != NULL && pApp != NULL)
	{
		ASSERT_VALID (pMenu);

		HWND hwndMenu = pMenu->GetSafeHwnd ();
		while (::IsWindow (hwndMenu))
		{
			MSG msg;
			while (::PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (!::IsWindow (hwndMenu))
				{
					break;
				}

				if (::IsWindow (hwndMenu) &&
					!pApp->PreTranslateMessage (&msg))
				{
					::TranslateMessage (&msg);
					::DispatchMessage (&msg);
				}

				pApp->OnIdle (0);
			}

			if (!::IsWindow (hwndMenu))
			{
				break;
			}

			WaitMessage ();
		}
	}

	if (pParentDlg != NULL)
	{
		pParentDlg->SetActiveMenu (NULL);
	}

	if (pParentPropPage != NULL)
	{
		pParentPropPage->SetActiveMenu (NULL);
	}

	return m_nLastCommandID;
}
//***********************************************************************************************
void CBCGPContextMenuManager::GetMenuNames (CStringList& listOfNames) const
{
	listOfNames.RemoveAll ();

	for (POSITION pos = m_MenuNames.GetStartPosition (); pos != NULL;)
	{
		CString strName;
		HMENU hMenu;

		m_MenuNames.GetNextAssoc (pos, strName, hMenu);
		listOfNames.AddTail (strName);
	}
}
//***********************************************************************************************
HMENU CBCGPContextMenuManager::GetMenuByName (LPCTSTR lpszName, UINT* puiOrigResID) const
{
	HMENU hMenu;
	if (!m_MenuNames.Lookup (lpszName, hMenu))
	{
		return NULL;
	}

	if (puiOrigResID != NULL)
	{
		*puiOrigResID = 0;

		for (POSITION pos = m_Menus.GetStartPosition (); pos != NULL;)
		{
			UINT uiResId;
			HMENU hMenuMap;

			m_Menus.GetNextAssoc (pos, uiResId, hMenuMap);
			if (hMenuMap == hMenu)
			{
				*puiOrigResID = uiResId;
				break;
			}
		}
	}

	return hMenu;
}
//***********************************************************************************************
BOOL CBCGPContextMenuManager::LoadState (LPCTSTR lpszProfileName)
{
	CString strProfileName = ::BCGPGetRegPath (strMenusProfile, lpszProfileName);

	for (POSITION pos = m_Menus.GetStartPosition (); pos != NULL;)
	{
		UINT uiResId;
		HMENU hMenu;

		m_Menus.GetNextAssoc (pos, uiResId, hMenu);
		ASSERT (hMenu != NULL);

		HMENU hPopupMenu = ::GetSubMenu (hMenu, 0);
		ASSERT (hPopupMenu != NULL);

		CBCGPPopupMenuBar* pBar = new CBCGPPopupMenuBar;
		if (pBar->Create (CWnd::FromHandle (GetDesktopWindow ())))
		{
			if (!pBar->ImportFromMenu (hPopupMenu))
			{
				pBar->DestroyWindow ();
				delete pBar;
				return FALSE;
			}

			if (pBar->LoadState (strProfileName, 0, uiResId))
			{
				g_menuHash.SaveMenuBar (hPopupMenu, pBar);
			}

			pBar->DestroyWindow ();
		}
		delete pBar;
	}

	return TRUE;
}
//***********************************************************************************************
BOOL CBCGPContextMenuManager::SaveState (LPCTSTR lpszProfileName)
{
	CString strProfileName = ::BCGPGetRegPath (strMenusProfile, lpszProfileName);

	for (POSITION pos = m_Menus.GetStartPosition (); pos != NULL;)
	{
		UINT uiResId;
		HMENU hMenu;

		m_Menus.GetNextAssoc (pos, uiResId, hMenu);
		ASSERT (hMenu != NULL);

		HMENU hPopupMenu = ::GetSubMenu (hMenu, 0);
		ASSERT (hPopupMenu != NULL);

		CBCGPPopupMenuBar* pBar = new CBCGPPopupMenuBar;
		if (pBar->Create (CWnd::FromHandle (GetDesktopWindow ())))
		{
			if (g_menuHash.LoadMenuBar (hPopupMenu, pBar))
			{
				if (!pBar->SaveState (strProfileName, 0, uiResId))
				{
					pBar->DestroyWindow ();
					delete pBar;
					return FALSE;
				}
			}
		
			pBar->DestroyWindow ();
		}
		delete pBar;
	}

	return TRUE;
}
//***********************************************************************************************
BOOL CBCGPContextMenuManager::ResetState ()
{
	for (POSITION pos = m_Menus.GetStartPosition (); pos != NULL;)
	{
		UINT uiResId;
		HMENU hMenu;

		m_Menus.GetNextAssoc (pos, uiResId, hMenu);
		ASSERT (hMenu != NULL);

		HMENU hPopupMenu = ::GetSubMenu (hMenu, 0);
		ASSERT (hPopupMenu != NULL);

		g_menuHash.RemoveMenu (hPopupMenu);
	}

	return TRUE;
}
//********************************************************************************
HMENU CBCGPContextMenuManager::GetMenuById (UINT nMenuResId) const
{
   HMENU hMenu = NULL ;
   return m_Menus.Lookup (nMenuResId, hMenu) ? hMenu : NULL;
}
