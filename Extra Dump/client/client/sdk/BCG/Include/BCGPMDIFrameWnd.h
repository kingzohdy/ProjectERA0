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

#if !defined(AFX_BCGPMDIFRAMEWND_H__08B9EC04_DCE3_11D1_A64F_00A0C93A70EC__INCLUDED_)
#define AFX_BCGPMDIFRAMEWND_H__08B9EC04_DCE3_11D1_A64F_00A0C93A70EC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// BCGPMDIFrameWnd.h : header file
//

#include "BCGCBPro.h"
#include "BCGPFrameImpl.h"
#include "BCGPMainClientAreaWnd.h"
#include "BCGPPopupMenu.h"
#include "BCGPDockManager.h"

class COleClientItem;
class CBCGPPopupMenu;
class CBCGPToolBar;
class CBCGPToolbarMenuButton;
class CBCGPToolbarButton;

/////////////////////////////////////////////////////////////////////////////
// CBCGPMDIFrameWnd frame

class BCGCBPRODLLEXPORT CBCGPMDIFrameWnd : public CMDIFrameWnd
{
	friend class CBCGPMainClientAreaWnd;
	friend class CBCGPToolBar;
	friend class CBCGPMenuBar;
	friend class CBCGPToolbarsPage;
	friend class CBCGPWorkspace;
	friend class CBCGPPopupMenu;
	friend class CBCGPToolbarCustomize;
	friend class CBCGPMiniFrameWnd;
	friend class CBCGPMDIChildWnd;
	friend class CBCGPGlobalUtils;
	friend class CBCGPTabWnd;

	DECLARE_DYNCREATE(CBCGPMDIFrameWnd)

protected:
	CBCGPMDIFrameWnd();           // protected constructor used by dynamic creation

// Attributes
public:
	BOOL IsMenuBarAvailable () const
	{
		return m_Impl.GetMenuBar () != NULL;
	}

	const CBCGPMenuBar* GetMenuBar () const
	{
		return m_Impl.GetMenuBar ();
	}

	UINT GetDefaultResId () const
	{
		return m_Impl.m_nIDDefaultResource;
	}

	CBCGPPopupMenu*	GetActivePopup () const
	{
		return CBCGPPopupMenu::m_pActivePopupMenu;
	}

	const CObList& GetTearOffBars () const
	{
		return m_Impl.m_listTearOffToolbars;
	}

protected:
	BOOL					m_bIsTlbCustMode;
	CBCGPMainClientAreaWnd	m_wndClientArea;
	HMENU					m_hmenuWindow;
	CBCGPFrameImpl			m_Impl;
	CBCGPDockManager		m_dockManager; // THE DockManager
	BOOL					m_bContextHelp;
	BOOL					m_bDoSubclass;
	UINT					m_uiWindowsDlgMenuId;
	CString					m_strWindowsDlgMenuText;
	BOOL					m_bShowWindowsDlgAlways;
	BOOL					m_bShowWindowsDlgHelpButton;

	BOOL					m_bWasMaximized;
	BOOL					m_bIsMinimized;

	BOOL					m_bClosing;

// Operations
public:
	//------------------ dockmanager layer ------------
	void AddDockBar			();
	BOOL AddControlBar		(CBCGPBaseControlBar* pControlBar, BOOL bTail = TRUE);
	BOOL InsertControlBar	(CBCGPBaseControlBar* pControlBar, 
							 CBCGPBaseControlBar* pTarget, BOOL bAfter = TRUE);
	void RemoveControlBarFromDockManager (CBCGPBaseControlBar* pControlBar, BOOL bDestroy,
							 BOOL bAdjustLayout, BOOL bAutoHide);
	void DockControlBar		(CBCGPBaseControlBar* pBar, UINT nDockBarID = 0, 
							 LPCRECT lpRect = NULL);
	CBCGPBaseControlBar* ControlBarFromPoint (CPoint point, int nSensitivity, 
											  bool bExactBar, 
											   CRuntimeClass* pRTCBarType) const;
	CBCGPBaseControlBar* ControlBarFromPoint (CPoint point, int nSensitivity, 
											  DWORD& dwAlignment, 
											  CRuntimeClass* pRTCBarType) const;
	BOOL IsPointNearDockBar (CPoint point, DWORD& dwBarAlignment, 
							BOOL& bOuterEdge) const;
	virtual void AdjustDockingLayout (HDWP hdwp = NULL);
	BOOL EnableDocking (DWORD dwDockStyle);	
	BOOL EnableAutoHideBars (DWORD dwDockStyle);

	CBCGPBaseControlBar* GetControlBar (UINT nID);
	void ShowControlBar (CBCGPBaseControlBar* pBar, BOOL bShow, BOOL bDelay, BOOL bActivate);

	virtual BOOL OnMoveMiniFrame	(CWnd* pFrame);
	virtual void RecalcLayout (BOOL bNotify = TRUE);
	//-------------------------------------------------
	

	BOOL DockControlBarLeftOf (CBCGPControlBar* pBar, CBCGPControlBar* pLeftOf);

	void SetupToolbarMenu (	CMenu& menu,
							const UINT uiViewUserToolbarCmdFirst,
							const UINT uiViewUserToolbarCmdLast)
	{
		m_Impl.SetupToolbarMenu (menu, uiViewUserToolbarCmdFirst, uiViewUserToolbarCmdLast);
	}

	void EnableWindowsDialog (UINT uiMenuId, 
		LPCTSTR lpszMenuText,
		BOOL bShowAllways = FALSE,
		BOOL bShowHelpButton = FALSE);
	void EnableWindowsDialog (UINT uiMenuId, 
		UINT uiMenuTextResId,
		BOOL bShowAllways = FALSE,
		BOOL bShowHelpButton = FALSE);
		// Enable built-in CBCGPWindowsManagerDlg dialog. When bShowAllways
		// is TRUE, show windows dialog everytime; otherwise only instead of
		// the statndard "Windows..." dialog (appear if more than 9 windows 
		// are open)
	void ShowWindowsDialog ();
	
	// Enable/Disable MDI child tabs:
	void EnableMDITabs (BOOL bEnable = TRUE, BOOL bIcons = TRUE,
						CBCGPTabWnd::Location tabLocation = CBCGPTabWnd::LOCATION_BOTTOM,
						BOOL bTabCloseButton = FALSE,
						CBCGPTabWnd::Style style = CBCGPTabWnd::STYLE_3D_SCROLLED);
	BOOL AreMDITabs () const
	{
		return m_wndClientArea.m_bTabIsEnabled;
	}

	CBCGPTabWnd& GetMDITabs ()
	{
		return m_wndClientArea.GetMDITabs ();
	}

	BOOL IsFullScreen() const
	{
		return m_Impl.m_FullScreenMgr.IsFullScreen();
	}

	void ShowFullScreen()
	{
		m_Impl.m_FullScreenMgr.ShowFullScreen();
	}

	void EnableFullScreenMainMenu(BOOL bEnableMenu)
	{
		m_Impl.m_FullScreenMgr.EnableMainMenu(bEnableMenu);
	}

	void EnableFullScreenMode(UINT uiFullScreenCmd)
	{
		m_Impl.m_FullScreenMgr.SetFullScreenID(uiFullScreenCmd);
	}

	void EnableLoadDockState(BOOL bEnable = TRUE)
	{
		m_Impl.m_bLoadDockState = bEnable;
	}

	void EnableMDITabsLastActiveActivation(BOOL bLastActiveTab = TRUE)
	{
		if (AreMDITabs ())
		{
			m_wndClientArea.m_wndTab.EnableActivateLastActive(bLastActiveTab);
		}	
	}

protected:
	virtual BOOL OnSetMenu (HMENU hmenu);

// Implementation:
	void InitUserToolbars (LPCTSTR lpszRegEntry, UINT uiUserToolbarFirst, UINT uiUserToolbarLast)
	{
		m_Impl.InitUserToolbars (lpszRegEntry, uiUserToolbarFirst, uiUserToolbarLast);
	}

	void LoadUserToolbars ()
	{
		m_Impl.LoadUserToolbars ();
	}

	void SaveUserToolbars ()
	{
		m_Impl.SaveUserToolbars ();
	}

	void LoadTearOffMenus ()
	{
		m_Impl.LoadTearOffMenus ();
	}

	void SaveTearOffMenus ()
	{
		m_Impl.SaveTearOffMenus ();
	}

	BOOL IsDockStateValid (const CDockState& state)
	{
		return m_Impl.IsDockStateValid (state);
	}

	CBCGPToolBar* GetUserBarByIndex (int iIndex) const
	{
		return m_Impl.GetUserBarByIndex (iIndex);
	}

	void SetDockState(const CDockState& state)
	{
		m_Impl.SetDockState (state);
	}

	CBCGPDockManager* GetDockManager () {return &m_dockManager;}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPMDIFrameWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	virtual HMENU GetWindowMenuPopup (HMENU hMenuBar);
	BOOL ShowPopupMenu (CBCGPPopupMenu* pMenuPopup);

public:
	virtual BOOL OnShowPopupMenu (CBCGPPopupMenu* /*pMenuPopup*/)
	{
		return TRUE;
	}

	virtual BOOL OnShowCustomizePane(CBCGPPopupMenu* pMenuPane, UINT uiToolbarID)
	{
		 m_Impl.AddDefaultButtonsToCustomizePane(pMenuPane, uiToolbarID);

		 return TRUE;	
	}

	virtual void OnClosePopupMenu (CBCGPPopupMenu* pMenuPopup);
	virtual BOOL OnDrawMenuImage (	CDC* /*pDC*/, 
									const CBCGPToolbarMenuButton* /*pMenuButton*/, 
									const CRect& /*rectImage*/)
	{
		return FALSE;
	}

	virtual BOOL OnMenuButtonToolHitTest (CBCGPToolbarButton* /*pButton*/, TOOLINFO* /*pTI*/)
	{
		return FALSE;
	}

	virtual BOOL GetToolbarButtonToolTipText (CBCGPToolbarButton* /*pButton*/, CString& /*strTTText*/)
	{
		return FALSE;
	}

	virtual BOOL OnEraseMDIClientBackground (CDC* /*pDC*/)
	{
		return FALSE;
	}

	virtual void OnDrawMenuLogo (CDC* /*pDC*/, CBCGPPopupMenu* /*pMenu*/, const CRect& /*rectLogo*/)	{}
	virtual void OnUpdateFrameMenu (HMENU hMenuAlt);

	virtual BOOL OnTearOffMenu (CBCGPPopupMenu* /*pMenuPopup*/, CBCGPControlBar* /*pBar*/)
	{
		return TRUE;
	}

	virtual void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);
	virtual BOOL OnShowControlBars (BOOL bShow);
	virtual BOOL NegotiateBorderSpace( UINT nBorderCmd, LPRECT lpRectBorder );

	virtual void AdjustClientArea ();
	virtual BOOL OnCloseMiniFrame (CBCGPMiniFrameWnd* /*pWnd*/) 
	{
		return TRUE;
	}
	virtual BOOL OnCloseDockingBar (CBCGPDockingControlBar* /*pWnd*/)
	{
		return TRUE;
	}

// Implementation
protected:
	virtual ~CBCGPMDIFrameWnd();
	virtual COleClientItem*	GetInPlaceActiveItem ();

	// Generated message map functions
	//{{AFX_MSG(CBCGPMDIFrameWnd)
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	#if _MSC_VER >= 1300
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
	#else
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	#endif

	afx_msg LRESULT OnToolbarCreateNew(WPARAM,LPARAM);
	afx_msg LRESULT OnToolbarDelete(WPARAM,LPARAM);
	afx_msg void OnContextHelp ();
	afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnBarCheck(UINT nID);
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM);
	LRESULT OnExitSizeMove (WPARAM, LPARAM);
	afx_msg void OnWindowNew();
	DECLARE_MESSAGE_MAP()
};

#ifndef InitUserToobars
#define InitUserToobars	InitUserToolbars
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPMDIFRAMEWND_H__08B9EC04_DCE3_11D1_A64F_00A0C93A70EC__INCLUDED_)
