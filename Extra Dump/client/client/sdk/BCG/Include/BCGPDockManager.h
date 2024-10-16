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

// BCGPDockManager.h: interface for the CBCGPDockManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPDOCKMANAGER_H__FEF7A0D3_B9D4_4FF4_911F_E75B71B20FDF__INCLUDED_)
#define AFX_BCGPDOCKMANAGER_H__FEF7A0D3_B9D4_4FF4_911F_E75B71B20FDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGCBPro.h"
#include "BCGPControlBar.h"

class CBCGPDockBar;
class CBCGPAutoHideDockBar;
class CBCGPAutoHideToolBar;
class CBCGPMiniFrameWnd;
class CBCGPMultiMiniFrameWnd;
class CBCGPTabbedControlBar;
class CBCGPSlider;
class CBCGPDockingControlBar;
class CBCGPOleCntrFrameWnd;

struct BCGP_DOCKBAR_INFO
{
	DWORD			m_dwBarAlignment;
	CRuntimeClass*	pDockBarRTC;
};

struct BCGP_AUTOHIDEBAR_SAVE_INFO
{
	BCGP_AUTOHIDEBAR_SAVE_INFO ()
	{
		m_pSavedBar = NULL;
		m_rectBar.SetRectEmpty ();
		m_nBarID = (UINT)-1;
		m_bIsVisible = FALSE;
		m_dwBarAlignment = 0;
		m_bFirstInGroup = TRUE;
		m_bLastInGroup = TRUE;
		m_bActiveInGroup = TRUE;
	}

	BCGP_AUTOHIDEBAR_SAVE_INFO (BCGP_AUTOHIDEBAR_SAVE_INFO& src)
	{
		Copy (src);	
	}

	UINT	m_nBarID;
	DWORD	m_dwBarAlignment;
	BOOL	m_bIsVisible;
	CRect	m_rectBar;
	CList<UINT, UINT&> m_lstSiblingBars;

	BOOL	m_bFirstInGroup;
	BOOL	m_bLastInGroup;
	BOOL	m_bActiveInGroup;

	void Serilaize (CArchive& ar);
	BCGP_AUTOHIDEBAR_SAVE_INFO& operator= (BCGP_AUTOHIDEBAR_SAVE_INFO& src)
	{
		return Copy (src);	
	}

	BCGP_AUTOHIDEBAR_SAVE_INFO& Copy (BCGP_AUTOHIDEBAR_SAVE_INFO& src)
	{
		m_lstSiblingBars.RemoveAll ();
		m_nBarID = src.m_nBarID;
		m_dwBarAlignment = src.m_dwBarAlignment;
		m_bIsVisible = src.m_bIsVisible;
		m_rectBar = src.m_rectBar;
		m_lstSiblingBars.AddTail (&src.m_lstSiblingBars);
		m_bFirstInGroup = src.m_bFirstInGroup;
		m_bLastInGroup = src.m_bLastInGroup;
		m_bActiveInGroup = src.m_bActiveInGroup;
		return *this;
	}

	CBCGPDockingControlBar* m_pSavedBar; // must be set before serialize;
};

class BCGCBPRODLLEXPORT CBCGPDockManager : public CObject
{
	friend class CBCGPPrintPreviewView;
	friend class CBCGPMDIChildWnd;
	friend class CBCGPOleCntrFrameWnd;

public:
	CBCGPDockManager();
	virtual ~CBCGPDockManager();

	DWORD GetEnabledAutoHideAlignment () const {return m_dwEnabledSlideBars;}

	void DockControlBar (CBCGPBaseControlBar* pBar, UINT nDockBarID = 0, 
						 LPCRECT lpRect = NULL);
	BOOL DockControlBarLeftOf (CBCGPControlBar* pBarToDock, CBCGPControlBar* pTargetBar);

	BOOL IsPointNearDockBar (CPoint point, DWORD& dwBarAlignment, BOOL& bOuterEdge) const;

	BOOL Create (CFrameWnd* pParentWnd);
	BOOL EnableDocking (DWORD dwStyle);
	BOOL EnableAutoHideBars (DWORD dwStyle);

	BOOL AddDockBar (const BCGP_DOCKBAR_INFO& info, CBCGPDockBar** ppDockBar = NULL); 
	BOOL InsertDockBar (const BCGP_DOCKBAR_INFO& info, 
						 DWORD dwAlignToInsertAfter, 
						 CBCGPDockBar** ppDockBar = NULL);

	BOOL AddControlBar (CBCGPBaseControlBar* pWnd, BOOL bTail = TRUE, 
						BOOL bAutoHide = FALSE, BOOL bInsertForOuterEdge = FALSE); 
	BOOL InsertControlBar (CBCGPBaseControlBar* pControlBar, 
						   CBCGPBaseControlBar* pTarget, BOOL bAfter = TRUE);

	void CalcExpectedDockedRect (CWnd* pWnd, CPoint ptMouse, CRect& rectResult, 
									BOOL& bDrawTab, CBCGPDockingControlBar** ppTargetBar);

	
	void RemoveControlBarFromDockManager (CBCGPBaseControlBar* pWnd, BOOL bDestroy, BOOL bAdjustLayout, 
											BOOL bAutoHide = FALSE);

	virtual CBCGPBaseControlBar* ControlBarFromPoint (CPoint point, int nSensitivity, 
													 bool bExactBar = false, 
													 CRuntimeClass* pRTCBarType = NULL,
													 BOOL bCheckVisibility = FALSE, 
													 const CBCGPBaseControlBar* pBarToIgnore = NULL) const;
	virtual CBCGPBaseControlBar* ControlBarFromPoint (CPoint point, int nSensitivity, 
													 DWORD& dwAlignment, 
													 CRuntimeClass* pRTCBarType = NULL, 
													 const CBCGPBaseControlBar* pBarToIgnore = NULL) const;
	virtual CBCGPMiniFrameWnd* FrameFromPoint (CPoint pt, CBCGPMiniFrameWnd* pFrameToExclude, 
												BOOL bFloatMultiOnly) const;

	virtual BCGP_CS_STATUS DetermineControlBarAndStatus (CPoint pt, int nSencitivity, 
														 DWORD dwEnabledAlignment,
														 CBCGPBaseControlBar** ppTargetBar, 
														 const CBCGPBaseControlBar* pBarToIgnore,
														 const CBCGPBaseControlBar* pBarToDock);

	virtual BOOL AddMiniFrame (CBCGPMiniFrameWnd* pWnd);
	virtual BOOL RemoveMiniFrame (CBCGPMiniFrameWnd* pWnd);

	virtual CBCGPBaseControlBar* FindBarByID (UINT uBarID, BOOL bSearchMiniFrames = FALSE);
	virtual CBCGPDockBar* FindDockBar (DWORD dwAlignment, BOOL bOuter);	

	virtual CBCGPDockBar* FindDockBarByControlBar (CBCGPControlBar* pTargetBar);

	virtual void FixupVirtualRects ();
	virtual void RecalcLayout (BOOL bNotify = TRUE);
	virtual void AdjustDockingLayout (HDWP hdwp = NULL);
	virtual BOOL OnMoveMiniFrame (CWnd* pFrame);

	virtual BOOL SaveState (LPCTSTR lpszProfileName = NULL, UINT uiID = (UINT) -1);
	virtual BOOL LoadState (LPCTSTR lpszProfileName = NULL, UINT uiID = (UINT) -1);
	virtual void SetDockState ();

	virtual BOOL ShowControlBars (BOOL bShow);
	virtual void OnActivateFrame (BOOL bActivate);

	CRect GetClientAreaBounds () const {return m_rectClientAreaBounds;}

	CBCGPAutoHideToolBar*  AutoHideBar (CBCGPDockingControlBar* pBar, 
										CBCGPAutoHideToolBar* pCurrAutoHideToolBar = NULL);
	void AlignAutoHideBar (CBCGPSlider* pDefaultSlider, BOOL bIsVisible = TRUE);
	void BringBarsToTop (DWORD dwAlignment = 0, BOOL bExcludeDockedBars = TRUE);
	void SetAutohideZOrder (CBCGPDockingControlBar* pAHDockingBar);
	void HideAutoHideBars (CBCGPDockingControlBar* pBarToExclude = NULL, BOOL bImmediately = FALSE);

	virtual BOOL AdjustRectToClientArea (CRect& rectResult, DWORD dwAlignment);

	void Serialize (CArchive& ar);

	static UINT g_nTimeOutBeforeToolBarDock;
	static UINT g_nTimeOutBeforeDockingBarDock;

	void SetPrintPreviewMode (BOOL bPreview, CPrintPreviewState* pState);
	BOOL IsPrintPreviewValid () const {return m_bIsPrintPreviewMode;}
	BOOL IsOLEContainerMode () const {return m_bIsOLEContainerMode;}

	BOOL SendMessageToMiniFrames (UINT uMessage, WPARAM wParam = 0, LPARAM lParam = 0);

	void LockUpdate (BOOL bLock);

	BOOL IsInAdjustLayout () const
	{
		return m_bAdjustingBarLayout;
	}

	void GetControlBarList (CObList& lstBars, BOOL bIncludeAutohide = FALSE, 
							CRuntimeClass* pRTCFilter = NULL);

	void EnableInPlaceActivation()
	{
		m_bIsOLEInPlaceActivate = TRUE;
	}

public:
	BOOL					m_bEnableAdjustLayout;
	BOOL					m_bLockUpdate;
	BOOL					m_bSizeFrame;

	static BCGP_DOCK_TYPE	m_dockModeGlobal;
	static UINT				m_ahSlideModeGlobal;
	static int				m_nDockSencitivity;
	static BOOL				m_bRestoringDockState; 
	static BOOL				m_bSavingState; 

	static BOOL				m_bHideDockingBarsInContainerMode;

	static BOOL				m_bDisableRecalcLayout;

	static BOOL				m_bFullScreenMode;

protected:
	CObList		m_lstControlBars;
	CObList		m_lstAutoHideBars;

	CObList		m_lstLoadedBars; // list of control bars restored from the archive
	CObList		m_lstNonFloatingBars; // list of non-floating control bars restored from the archive
	CObList		m_lstLoadedMiniFrames;

	// list of ahoto hide bar ids loaded from the archive
	CList<BCGP_AUTOHIDEBAR_SAVE_INFO, BCGP_AUTOHIDEBAR_SAVE_INFO&> 
													m_lstLoadedAutoHideBarIDs;

	CObList		m_lstMiniFrames; // list miniframes; these miniframes contain control bars
								 // that belong to this dock manager (and parent frame as well)

	CFrameWnd*	m_pParentWnd;
	CRect		m_rectDockBarBounds;
	CRect		m_rectClientAreaBounds; // it contains bounds of the client area and 
										// calculated as m_rectDockBarBounds minus
										// rectangles of all dock bars
	CRect		m_rectOuterEdgeBounds;  // it contains rectangle of "outer edges" and
										// calculated as m_rectDockBarBounds minus 
										// rectangles of first nonresizable dock bars

	DWORD		m_dwEnabledDockBars;
	DWORD		m_dwEnabledSlideBars;

	CBCGPBaseControlBar*	m_pLastTargetBar;
	CBCGPMultiMiniFrameWnd* m_pLastMultiMiniFrame;
	clock_t					m_clkLastTime;
	BCGP_CS_STATUS			m_statusLast;

	// Print preview memebers:
	BOOL		m_bIsPrintPreviewMode;
	CObList		m_lstBarsHiddenInPreview;

	// OLE container mode
	BOOL		m_bIsOLEContainerMode;

	// OLE server mode
	BOOL		m_bIsOLEInPlaceActivate;

	CBCGPMiniFrameWnd* m_pActiveSlidingWnd;

	BOOL		m_bAdjustingBarLayout;
	BOOL		m_bRecalcLayout;

	BOOL		m_bDisableSetDockState; // should be disabled if load from registry failed

	// Bars hidden on deactivation
	CList<HWND, HWND&>		m_lstBarsHiddenOnDeactivate;

protected:
	void AlignByRect (const CRect& rectToAlignBy, CRect& rectResult, 
						DWORD dwAlignment, BOOL bHorizontal, BOOL bResizable);
	void AlignContainer (CRect& rectCurrBounds, POSITION& posNextBar, DWORD dwAlignment, 
						 BOOL bHorizontal);
	void CalcBarOffset (CBCGPAutoHideDockBar* pBar);

	void HideForPrintPreview (const CObList& lstBars);
};

#endif // !defined(AFX_BCGPDOCKMANAGER_H__FEF7A0D3_B9D4_4FF4_911F_E75B71B20FDF__INCLUDED_)
