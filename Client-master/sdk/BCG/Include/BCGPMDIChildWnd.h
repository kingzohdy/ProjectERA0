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

#if !defined(AFX_BCGPMDICHILDWND_H__3BE44BE6_C83D_11D3_A723_009027900694__INCLUDED_)
#define AFX_BCGPMDICHILDWND_H__3BE44BE6_C83D_11D3_A723_009027900694__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCGMDIChildWnd.h : header file
//

#include "BCGCBPro.h"

class CBCGPMDIFrameWnd;

class CBCGPDisableMDIAnimation
{
public:
	CBCGPDisableMDIAnimation()
	{
		// Inquire animation flag
		m_AnimInfo.cbSize = sizeof (ANIMATIONINFO);
		SystemParametersInfo (SPI_GETANIMATION, 0, &m_AnimInfo, 0);
		
		m_bPrevAnimation = (m_AnimInfo.iMinAnimate != 0);
		
		// Disable animation
		m_AnimInfo.iMinAnimate = FALSE;
		SystemParametersInfo (SPI_SETANIMATION, 0, &m_AnimInfo, 0);
	}
	
	~CBCGPDisableMDIAnimation()
	{
		// Set back previous animation
		m_AnimInfo.iMinAnimate = m_bPrevAnimation;
		SystemParametersInfo (SPI_SETANIMATION, 0, &m_AnimInfo, 0);
	}
	
private:
	ANIMATIONINFO	m_AnimInfo;
	BOOL			m_bPrevAnimation;
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPMDIChildWnd frame

class BCGCBPRODLLEXPORT CBCGPMDIChildWnd : public CMDIChildWnd
{
	friend class CBCGPMainClientAreaWnd;
	friend class CBCGPGlobalUtils;

	DECLARE_DYNCREATE(CBCGPMDIChildWnd)
protected:
	CBCGPMDIChildWnd();           // protected constructor used by dynamic creation

// Attributes
public:
	static BOOL			m_bEnableFloatingBars;
protected:
	CBCGPMDIFrameWnd*	m_pMDIFrame;
	BOOL				m_bToBeDestroyed;
	CBCGPDockManager	m_dockManager;

	BOOL				m_bWasMaximized;
	BOOL				m_bIsMinimized;

// Operations
public:
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

	virtual BOOL GetToolbarButtonToolTipText (CBCGPToolbarButton* /*pButton*/, CString& /*strTTText*/)
	{
		return FALSE;
	}


	BOOL DockControlBarLeftOf(CBCGPControlBar* pBar, CBCGPControlBar* pLeftOf);

// Overrides

	// Next methods used by MDI tabs:
	virtual CString GetFrameText () const;
	virtual HICON GetFrameIcon () const;

	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);

	virtual BOOL CanShowOnMDITabs () {	return TRUE;	}

	CBCGPDockManager* GetDockManager () {	return &m_dockManager;	}

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPMDIChildWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CBCGPMDIChildWnd();

	// Generated message map functions
	//{{AFX_MSG(CBCGPMDIChildWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnDestroy();
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	afx_msg LRESULT OnSetText(WPARAM,LPARAM);
	afx_msg LRESULT OnSetIcon(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()

	void AdjustClientArea ();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPMDICHILDWND_H__3BE44BE6_C83D_11D3_A723_009027900694__INCLUDED_)
