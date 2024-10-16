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

#if !defined(AFX_BCGPMAINCLIENTAREAWND_H__08B9EC05_DCE3_11D1_A64F_00A0C93A70EC__INCLUDED_)
#define AFX_BCGPMAINCLIENTAREAWND_H__08B9EC05_DCE3_11D1_A64F_00A0C93A70EC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// BCGMainClientAreaWnd.h : header file
//

#ifndef __AFXTEMPL_H__
	#include "afxtempl.h"
#endif

#include "BCGCBPro.h"
#include "BCGPTabWnd.h"

class CBCGPMDIFrameWnd;

/////////////////////////////////////////////////////////////////////////////
// CBCGPMainClientAreaWnd window

class BCGCBPRODLLEXPORT CBCGPMainClientAreaWnd : public CWnd
{
	DECLARE_DYNAMIC(CBCGPMainClientAreaWnd)
	friend class CBCGPMDIFrameWnd;

// Construction
public:
	CBCGPMainClientAreaWnd();

// Attributes
public:
	CBCGPTabWnd& GetMDITabs ()
	{
		return m_wndTab;
	}

protected:
	CBCGPTabWnd					m_wndTab;
	CBCGPTabWnd::Location		m_tabLocation;
	BOOL						m_bTabIsVisible;
	BOOL						m_bTabIsEnabled;
	CImageList					m_TabIcons;
	BOOL						m_bTabIcons;
	BOOL						m_bTabCloseButton;
	CMap<HICON,HICON,int,int>	m_mapIcons;	// Icons already loaded into the image list

// Operations
public:
	void EnableMDITabs (BOOL bEnable = TRUE,
						BOOL bIcons = TRUE,
						CBCGPTabWnd::Location tabLocation = CBCGPTabWnd::LOCATION_BOTTOM,
						BOOL bCloseButton = FALSE,
						CBCGPTabWnd::Style style = CBCGPTabWnd::STYLE_3D_SCROLLED);
	BOOL DoesMDITabExist () const
	{
		return m_bTabIsEnabled;
	}

	void SetActiveTab (HWND hwnd);
	void UpdateTabs (BOOL bSetActiveTabVisible = FALSE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPMainClientAreaWnd)
	protected:
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPMainClientAreaWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPMainClientAreaWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg LRESULT OnSetMenu (WPARAM wp, LPARAM);
	afx_msg LRESULT OnUpdateTabs (WPARAM, LPARAM);
	afx_msg LRESULT OnMDIRefreshMenu (WPARAM wp, LPARAM);
	afx_msg LRESULT OnMDIDestroy(WPARAM wp, LPARAM);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPMAINCLIENTAREAWND_H__08B9EC05_DCE3_11D1_A64F_00A0C93A70EC__INCLUDED_)
