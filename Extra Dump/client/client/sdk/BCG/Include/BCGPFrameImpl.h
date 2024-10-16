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

// BCGFrameImpl.h: interface for the CBCGPFrameImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPFRAMEIMPL_H__829B77B5_FE0E_11D1_8BCB_00A0C9B05590__INCLUDED_)
#define AFX_BCGPFRAMEIMPL_H__829B77B5_FE0E_11D1_8BCB_00A0C9B05590__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CBCGPBaseControlBar;
class CBCGPToolBar;
class CBCGPMenuBar;
class CBCGPPopupMenu;



#include "BCGCBPro.h"
#include "BCGPDockManager.h"
#include "BCGPFullScreenImpl.h"

class BCGCBPRODLLEXPORT CBCGPFrameImpl
{
	friend class CBCGPToolBar;
	friend class CBCGPMDIFrameWnd;
	friend class CBCGPFrameWnd;
	friend class CBCGPOleIPFrameWnd;
	friend class CBCGPOleDocIPFrameWnd;
	friend class CBCGPMenuBar;
	friend class CBCGPToolbarsPage;
	friend class CBCGPWorkspace;
	friend class CBCGPPopupMenu;
	friend class CBCGPMiniFrameWnd;
	friend class CBCGPFullScreenImpl;

public:
	CBCGPFrameImpl(CFrameWnd* pFrame);
	virtual ~CBCGPFrameImpl();

	void SetDockManager (CBCGPDockManager* pManager) 
	{
		ASSERT_VALID (pManager); 
		m_pDockManager = pManager;
	}

	void GetFullScreenMinMaxInfo(MINMAXINFO FAR* lpMMI)
	{
		m_FullScreenMgr.OnGetMinMaxInfo(lpMMI);
	}

// Attributes:
public:
	const CBCGPMenuBar* GetMenuBar () const
	{
		return m_pMenuBar;
	}

	static BOOL			m_bControlBarExtraPixel;

protected:
	UINT				m_uiUserToolbarFirst;
	UINT				m_uiUserToolbarLast;

	CObList				m_listUserDefinedToolbars;
	CObList				m_listTearOffToolbars;

	CString				m_strControlBarRegEntry;

	CBCGPMenuBar*		m_pMenuBar;
	HMENU				m_hDefaultMenu;
	CFrameWnd*			m_pFrame;
	CBCGPDockManager*	m_pDockManager;
	UINT				m_nIDDefaultResource;
	CBCGPFullScreenImpl	m_FullScreenMgr;
	BOOL				m_bLoadDockState;

// Operations
protected:

	static UINT GetFreeCtrlBarID (UINT uiFirstID, UINT uiLastID,
							const CObList& lstCtrlBars);

	// User-defined toolbars:
	void InitUserToolbars (LPCTSTR lpszRegEntry, UINT uiUserToolbarFirst, UINT uiUserToolbarLast);
	void LoadUserToolbars ();
	void SaveUserToolbars (BOOL bFrameBarsOnly = FALSE);

	const CBCGPToolBar* CreateNewToolBar (LPCTSTR lpszName);
	BOOL DeleteToolBar (CBCGPToolBar* pToolBar);

	// Tear-off toolbars:
	void AddTearOffToolbar (CBCGPBaseControlBar* pToolBar);
	void RemoveTearOffToolbar (CBCGPBaseControlBar* pToolBar);

	void LoadTearOffMenus ();
	void SaveTearOffMenus (BOOL bFrameBarsOnly = FALSE);

	void OnCloseFrame();
	void OnLoadFrame();
	void RestorePosition(CREATESTRUCT& cs);

	BOOL IsDockStateValid (const CDockState& state);
	BOOL IsUserDefinedToolbar (const CBCGPToolBar* pToolBar) const;

	void SetMenuBar (CBCGPMenuBar* pMenuBar);
	CBCGPToolBar* GetUserBarByIndex (int iIndex) const;

	void SetupToolbarMenu (	CMenu& menu, 
							const UINT uiViewUserToolbarCmdFirst,
							const UINT uiViewUserToolbarCmdLast);

	BOOL ProcessKeyboard (int nKey, BOOL* pbProcessAccel = NULL);
	BOOL ProcessMouseClick (UINT uiMsg, POINT pt, HWND hwnd);
	BOOL ProcessMouseMove (POINT pt);

	void FrameEnableDocking(CFrameWnd * pFrame, DWORD dwDockStyle);

	BOOL OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup, CFrameWnd* pWndFrame);

	BOOL OnShowCustomizePane(CBCGPPopupMenu* /*pMenuPane*/, UINT uiToolbarID);

	BOOL OnMenuChar (UINT nChar);

	void DeactivateMenu ();

	void SetDockState (const CDockState& state);
	void SaveDockState (LPCTSTR lpszSectionName = NULL);
	void LoadDockState (LPCTSTR lpszSectionName = NULL);

	BOOL LoadLargeIconsState ();

	static BOOL IsHelpKey (LPMSG lpMsg);

	void ShowQuickCustomizePane(CBCGPPopupMenu* pMenuPopup);
	void AddDefaultButtonsToCustomizePane(CBCGPPopupMenu* pMenuPane, UINT uiToolbarID);
	BOOL IsCustomizePane(const CBCGPPopupMenu* pMenuPopup) const;

	CRect GetFullScreeen()
	{
		return m_FullScreenMgr.GetFullScreenRect();
	}

	BOOL IsFullScreeen() const
	{
		return m_FullScreenMgr.IsFullScreen();
	}

	void EnableFullScreenMainMenu(BOOL bShow = TRUE)
	{
		m_FullScreenMgr.EnableMainMenu(bShow);
	}
};

#define UserToobars	UserToolbars

#endif // !defined(AFX_BCGPFRAMEIMPL_H__829B77B5_FE0E_11D1_8BCB_00A0C9B05590__INCLUDED_)
