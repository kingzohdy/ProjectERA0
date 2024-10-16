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

#if !defined(AFX_BCGPPOPUPMENU_H__A6E1AD75_D511_11D1_A64D_00A0C93A70EC__INCLUDED_)
#define AFX_BCGPPOPUPMENU_H__A6E1AD75_D511_11D1_A64D_00A0C93A70EC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// BCGPPopupMenu.h : header file
//

#include "BCGCBPro.h"
#include "BCGPPopupMenuBar.h"

class CBCGPToolbarMenuButton;
class CBCGPMenuBar;
class CBCGPMenuPage;

/////////////////////////////////////////////////////////////////////////////
// CBCGPPopupMenu frame

class BCGCBPRODLLEXPORT CBCGPPopupMenu : public CMiniFrameWnd
{
	friend class CCustomizeButton;
	friend class CBCGPToolbarMenuButton;
	friend class CBCGPMenuPage;
	friend class CBCGPFrameWnd;
	friend class CBCGPMDIFrameWnd;
	friend class CBCGPOleIPFrameWnd;
	friend class CBCGPOleDocIPFrameWnd;
	friend class CBCGPFrameImpl;
	friend class CBCGPDlgImpl;
	friend class CBCGPContextMenuManager;

	DECLARE_SERIAL(CBCGPPopupMenu)

public:
	CBCGPPopupMenu();
	virtual ~CBCGPPopupMenu();

protected:
	CBCGPPopupMenu(CBCGPMenuPage* pCustPage, LPCTSTR lpszTitle);	// For customization only

// Attributes
public:
	enum ANIMATION_TYPE
	{
		NO_ANIMATION,
		UNFOLD,
		SLIDE,
		FADE
	};

	enum MENUAREA_TYPE
	{
		MENU,
		SHADOW_RIGHT,
		SHADOW_BOTTOM,
		LOGO,
		OUTSIDE,
		TEAROFF_CAPTION
	};

	enum DROP_DIRECTION
	{
		DROP_DIRECTION_NONE,
		DROP_DIRECTION_BOTTOM,
		DROP_DIRECTION_TOP,
		DROP_DIRECTION_RIGHT,
		DROP_DIRECTION_LEFT,
	};

	enum LOGO_LOCATION
	{
		MENU_LOGO_LEFT,
		MENU_LOGO_RIGHT,
		MENU_LOGO_TOP,
		MENU_LOGO_BOTTOM,
	};

	enum QUICK_CUSTOMIZE_TYPE
	{
		QUICK_CUSTOMIZE_PANE,
		QUICK_CUSTOMIZE_ADDREMOVE,
		QUICK_CUSTOMIZE_NONE,

	};

	BOOL m_bAutoDestroyParent;
	HMENU GetMenu () const
	{
		return m_hMenu;
	}

	virtual CBCGPPopupMenuBar* GetMenuBar ()
	{
		return &m_wndMenuBar;
	}

	void SetAutoDestroy (BOOL bAutoDestroy = TRUE)
	{
		m_bAutoDestroy = bAutoDestroy;
	}

	CBCGPToolbarMenuButton* GetParentButton () const
	{
		return m_pParentBtn;
	}

    static void SetForceShadow(BOOL bValue)
    {
        m_bForceShadow = bValue;
    }

    static BOOL GetForceShadow()
    {
        return m_bForceShadow;
    }

    static void SetForceMenuFocus (BOOL bValue)
    {
        m_bForceMenuFocus = bValue;
    }

    static BOOL GetForceMenuFocus ()
    {
        return m_bForceMenuFocus;
    }

	static CBCGPPopupMenu::ANIMATION_TYPE GetAnimationType ()
	{
		return m_AnimationType;
	}

	static void SetAnimationType (CBCGPPopupMenu::ANIMATION_TYPE type)
	{
		m_AnimationType = type;
	}

	static void SetAnimationSpeed (UINT nElapse);

	static UINT GetAnimationSpeed ()
	{
		return m_AnimationSpeed;
	}

	static void EnableMenuSound (BOOL bEnable = TRUE)
	{
		m_bMenuSound = bEnable;
	}

	static UINT IsMenuSound ()
	{
		return m_bMenuSound;
	}

	BOOL IsScrollable () const
	{
		return m_bScrollable;
	}

	void EnableScrolling (BOOL /*bEnable*/ = TRUE)
	{
		// This method is obsolete;
	}

	void EnableMenuLogo (int iLogoSize, LOGO_LOCATION nLogoLocation = MENU_LOGO_LEFT);
	void SetMaxWidth (int iMaxWidth);

	CWnd* GetMessageWnd () const
	{
		return m_pMessageWnd; 
	}

	void SetMessageWnd (CWnd* pMsgWnd)
	{
		m_pMessageWnd = pMsgWnd;
	}

	BOOL PostCommand (UINT uiCommandID);
	// ------------------------

   void MoveTo (const CPoint& pt)
   {
	   m_ptLocation = pt;
	   RecalcLayout ();
   }

   virtual BOOL InCommand ()
   {
	   CBCGPPopupMenuBar* pBar = GetMenuBar ();
	   return pBar == NULL ? FALSE : pBar->m_bInCommand;
   }

	static CBCGPPopupMenu* GetActiveMenu ()
	{
		return m_pActivePopupMenu;
	}

	DROP_DIRECTION GetDropDirection () const
	{
		return m_DropDirection;
	}

	void SetRightAlign (BOOL bRightAlign = TRUE)
	{
		m_bRightAlign = bRightAlign;
	}

	BOOL IsRightAlign () const
	{
		return m_bRightAlign;
	}

	BOOL IsQuickCustomize()
	{
		return m_bQuickCusomize;
	}

	void SetQuickCustomizeType(QUICK_CUSTOMIZE_TYPE Type)
	{
		m_QuickType = Type;
	}

	QUICK_CUSTOMIZE_TYPE GetQuickCustomizeType() const
	{
		return m_QuickType;
	}

	BOOL IsCustomizePane()
	{
		return m_QuickType == QUICK_CUSTOMIZE_PANE;
	}

	BOOL IsShown() const
	{
		return m_bShown;
	}

protected:
	static CBCGPPopupMenu*	m_pActivePopupMenu;
	static BOOL				m_bForceMenuFocus;	// Menu takes a focus when activated

	CWnd*					m_pMessageWnd;

	CBCGPToolbarMenuButton*	m_pParentBtn;
	DROP_DIRECTION			m_DropDirection;
	CBCGPPopupMenuBar		m_wndMenuBar;
	HMENU					m_hMenu;
	CPoint					m_ptLocation;
	CPoint					m_ptLocationInitial;
	CSize					m_FinalSize;
	int						m_nMenuBarHeight;
	BOOL					m_bAutoDestroy;
	BOOL					m_bTrackMode;
	BOOL					m_bRightAlign;

	BOOL					m_bShown;
	BOOL					m_bTobeDstroyed;

	int						m_iMaxWidth;
	int						m_iLogoWidth;
	LOGO_LOCATION			m_nLogoLocation;

	CBCGPMenuPage* const		m_pMenuCustomizationPage;
	CString					m_strCaption;

	static BOOL				m_bMenuSound;


	//----------------------
	// Animation attributes:
	//----------------------
	static ANIMATION_TYPE	m_AnimationType;
	static UINT				m_AnimationSpeed;
	CSize					m_AnimSize;
	BOOL					m_bIsAnimRight;
	BOOL					m_bIsAnimDown;
	BOOL					m_bAnimationIsDone;

	BOOL					m_bScrollable;
	CRect					m_rectScrollUp;
	CRect					m_rectScrollDn;
	int						m_iScrollMode;	// -1 - Up, 0 - None, 1 - Down
	int						m_iScrollBtnHeight;

	CBitmap					m_bmpScreenSrc;
	CBitmap					m_bmpScreenDst;
	CBitmap					m_bmpScreenTmp;

	int						m_iFadePercent;
	
	COLORREF*				m_cFadeSrcBits;
	COLORREF*				m_cFadeDstBits;
	COLORREF*				m_cFadeTmpBits;

	//-------------------
	// Shadow attributes:
	//-------------------
	int						m_iShadowSize;
    static BOOL             m_bForceShadow;     // when TRUE paints shadows even outside main-frame
	CBitmap					m_bmpShadowRight;	// Saved shadow butmaps
	CBitmap					m_bmpShadowBottom;

	//---------------------
	// Tear-off attributes:
	//---------------------
	BOOL					m_bTearOffTracking;
	BOOL					m_bIsTearOffCaptionActive;
	CRect					m_rectTearOffCaption;
	CToolTipCtrl			m_wndToolTip;

	//------------------------
	// Quick Cusomization flags
	//------------------------
	BOOL					m_bQuickCusomize;
	QUICK_CUSTOMIZE_TYPE    m_QuickType;

// Operations
public:
	void SaveState ();

	CBCGPPopupMenu* GetParentPopupMenu () const;
	CBCGPToolBar* GetParentToolBar () const;

	CBCGPToolbarMenuButton* GetSelItem ();

	int InsertItem (const CBCGPToolbarMenuButton& button, int iInsertAt = -1);
	int InsertSeparator (int iInsertAt = -1);

	int GetMenuItemCount () const;
	CBCGPToolbarMenuButton* GetMenuItem (int iIndex) const;
	CBCGPToolbarMenuButton* FindSubItemByCommand (UINT uiCmd) const;
	
	BOOL RemoveItem (int iIndex);
	void RemoveAllItems ();

	void SetDefaultItem (UINT uiCmd)
	{
		GetMenuBar ()->m_uiDefaultMenuCmdId = uiCmd;
	}

	static BOOL ActivatePopupMenu (CFrameWnd* pTopFrame, CBCGPPopupMenu* pPopupMenu);
	void CloseMenu (BOOL bSetFocusToBar = FALSE);
	void ShowAllCommands ();

	BOOL HideRarelyUsedCommands () const;
	BOOL AreAllCommandsShown () const
	{
		return ((CBCGPPopupMenu*) this)->GetMenuBar ()->m_bAreAllCommandsShown;
	}

	MENUAREA_TYPE CheckArea (const CPoint& ptScreen) const;

	void UpdateShadow (LPRECT lprectScreen = NULL);
	static void UpdateAllShadows (LPRECT lprectScreen = NULL); // Repaint all opened menu shadows

	void SetQuickMode();

protected:
	void Initialize ();

	BOOL IsScrollUpAvailable ();
	BOOL IsScrollDnAvailable ();

	void CollapseSubmenus ();
	void DrawImage (CDC* pDC, const CRect& rect, int iImage, BOOL bDrawFrame = FALSE);

	void UpdateBottomWindows (BOOL bCheckOnly = FALSE);

	void DoPaint (CDC* pPaintDC);
	void DrawFade (CDC* pPaintDC);

	BOOL AdjustScroll (BOOL bForceMenuBarResize = FALSE);

	virtual BOOL InitMenuBar ();
	virtual BOOL TearOff (CPoint point);
	virtual CBCGPControlBar* CreateTearOffBar (CFrameWnd* pWndMain, UINT uiID, LPCTSTR lpszName);

	BOOL NotifyParentDlg (BOOL bActivate);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPPopupMenu)
	public:
    virtual BOOL Create(CWnd* pWndParent, int x, int y, HMENU hMenu, BOOL bLocked = FALSE, BOOL bOwnMessage=FALSE); // Add: Alex Corazzin (2)
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBCGPPopupMenu)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
#if _MSC_VER >= 1300
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
#else
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
#endif
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPPOPUPMENU_H__A6E1AD75_D511_11D1_A64D_00A0C93A70EC__INCLUDED_)
