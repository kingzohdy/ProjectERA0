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

// BCGPToolBox.h: interface for the CBCGPToolBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPTOOLBOX_H__58F892DC_E5CC_4A7F_84A5_DCE55D58509B__INCLUDED_)
#define AFX_BCGPTOOLBOX_H__58F892DC_E5CC_4A7F_84A5_DCE55D58509B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCGPToolBox.h : header file
//

#include "BCGCBPro.h"
#include "BCGPDockingControlBar.h"
#include "BCGPOutlookBar.h"
#include "BCGPButton.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPToolBoxPage window

class CBCGPToolBox;

class BCGCBPRODLLEXPORT CBCGPToolBoxPage : public CBCGPControlBar  
{
	friend class CBCGPToolBox;

	DECLARE_DYNCREATE(CBCGPToolBoxPage)

// Construction
public:
	CBCGPToolBoxPage();

// Attributes
protected:
	CArray<CBCGPButton*, CBCGPButton*>	m_arButtons;
	CSize								m_sizeImage;
	int									m_nCheckedButton;
	CBCGPToolBox*						m_pToolBox;
	int									m_nVertScrollOffset;
	int									m_nVertScrollSize;

protected:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPToolBoxPage)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPToolBoxPage();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPToolBoxPage)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int FindButton (HWND hwndButton) const;
	void ReposButtons ();

	BOOL InitPage (UINT uiBmpResID, int nImageWidth, const CStringList& lstLabels,
		CRuntimeClass* pButtonClass);
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPToolBoxBar window

class CBCGPToolBoxBar : public CBCGPOutlookBar
{
	virtual void OnActivateTab (int iTabNum);
	virtual void OnScroll (BOOL bDown);
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPToolBox window

class BCGCBPRODLLEXPORT CBCGPToolBox : public CBCGPDockingControlBar
{
	DECLARE_DYNCREATE(CBCGPToolBox)

// Construction
public:
	CBCGPToolBox();

// Attributes
public:
	int GetActivePage () const;
	int GetLastClickedTool (int nPage) const;
	int GetPageNumber (CBCGPToolBoxPage* pPage) const;

	CBCGPToolBoxPage* GetPage (int nPage) const;

	CBCGPBaseTabWnd* GetTabWnd () const;

protected:
	CBCGPToolBoxBar m_wndOutlook;

// Operations
public:
	BOOL AddToolsPage (LPCTSTR lpszPageName, UINT uiBmpResID, int nImageWidth,
		const CStringList& lstLabels,
		CRuntimeClass* pPageClass = RUNTIME_CLASS (CBCGPToolBoxPage),
		CRuntimeClass* pButtonClass = RUNTIME_CLASS (CBCGPButton));

	BOOL AddToolsPage (LPCTSTR lpszPageName, UINT uiBmpResID, int nImageWidth,
		LPCTSTR lpszLabels,	// Devided by '\n'
		CRuntimeClass* pPageClass = RUNTIME_CLASS (CBCGPToolBoxPage),
		CRuntimeClass* pButtonClass = RUNTIME_CLASS (CBCGPButton));

// Overrides
public:
	virtual void OnClickTool (int nPage, int nIndex);
	virtual void OnActivatePage (int nPage);

	virtual void OnScroll (BOOL bDown);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPToolBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPToolBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPToolBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPTOOLBOX_H__58F892DC_E5CC_4A7F_84A5_DCE55D58509B__INCLUDED_)
