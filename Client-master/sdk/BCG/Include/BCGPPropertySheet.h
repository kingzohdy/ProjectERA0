#if !defined(AFX_BCGPPROPERTYSHEET_H__54B75502_E449_41DD_AEE7_1BB6FBE72205__INCLUDED_)
#define AFX_BCGPPROPERTYSHEET_H__54B75502_E449_41DD_AEE7_1BB6FBE72205__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCGPPropertySheet.h : header file
//

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
// BCGPropertySheet.h : header file
//

#ifndef __AFXTEMPL_H__
	#include "afxtempl.h"
#endif

#include "BCGCBPro.h"
#include "BCGPOutlookBar.h"
#include "BCGPOutlookBarPane.h"
#include "BCGPTabWnd.h"
#include "BCGPDlgImpl.h"

class CBCGPPropertySheet;
class CBCGPPropertyPage;

/////////////////////////////////////////////////////////////////////////////
// CBCGPPropSheetPane

class CBCGPPropSheetPane : public CBCGPOutlookBarPane
{
	friend class CBCGPPropertySheet;

	virtual BOOL OnSendCommand (const CBCGPToolbarButton* pButton);
	void EnsureVisible (int iButton);

	CBCGPPropertySheet* m_pParent;
};

class CBCGPPropSheetTab : public CBCGPTabWnd
{
	friend class CBCGPPropertySheet;

	CBCGPPropSheetTab();

	virtual BOOL SetActiveTab (int iTab);

	CBCGPPropertySheet* m_pParent;
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPPropSheetCategory

class BCGCBPRODLLEXPORT CBCGPPropSheetCategory : public CObject
{
	friend class CBCGPPropertySheet;

	DECLARE_DYNAMIC(CBCGPPropSheetCategory)

	CBCGPPropSheetCategory(LPCTSTR lpszName, int nIcon, int nSelectedItem,
		const CBCGPPropSheetCategory* pParentCategory,
		CBCGPPropertySheet& propSheet);
	virtual ~CBCGPPropSheetCategory();

	const CString					m_strName;
	const int						m_nIcon;
	const int						m_nSelectedIcon;
	CBCGPPropSheetCategory*			m_pParentCategory;
	HTREEITEM						m_hTreeItem;

	CList<CBCGPPropSheetCategory*,CBCGPPropSheetCategory*>		m_lstSubCategories;
	CList<CBCGPPropertyPage*, CBCGPPropertyPage*>				m_lstPages;

	HTREEITEM						m_hLastSelectedItem;
	CBCGPPropertySheet&				m_propSheet;
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPPropertySheet

class BCGCBPRODLLEXPORT CBCGPPropertySheet : public CPropertySheet
{
	friend class CBCGPPropSheetPane;
	friend class CBCGPPropSheetCategory;

	DECLARE_DYNAMIC(CBCGPPropertySheet)

// Construction
public:
	CBCGPPropertySheet();
	CBCGPPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CBCGPPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

	enum PropSheetLook
	{
		PropSheetLook_Tabs,
		PropSheetLook_OutlookBar,
		PropSheetLook_Tree,
		PropSheetLook_OneNoteTabs
	};

	// Should be called BEFORE DoModal or Create!
	void SetLook (PropSheetLook look, int nNavControlWidth = 100);

// Attributes
public:
	CBCGPTabWnd& GetTab () const;	// for PropSheetLook_OneNoteTabs only

protected:
	PropSheetLook		m_look;
	CBCGPOutlookBar		m_wndOutlookBar;
	CBCGPPropSheetPane	m_wndPane1;
	CBCGPPropSheetTab	m_wndTab;
	CTreeCtrl			m_wndTree;
	int					m_nBarWidth;
	int					m_nActivePage;
	CImageList			m_Icons;

	CList<CBCGPPropSheetCategory*,CBCGPPropSheetCategory*>	m_lstTreeCategories;

	BOOL				m_bIsInSelectTree;
	CBCGPDlgImpl		m_Impl;

// Operations
public:
	BOOL SetIconsList (UINT uiImageListResID, int cx, COLORREF clrTransparent = RGB (255, 0, 255));
	void SetIconsList (HIMAGELIST hIcons);

	void AddPage(CPropertyPage* pPage);

	void RemovePage(CPropertyPage* pPage);
	void RemovePage(int nPage);
	void RemoveCategory (CBCGPPropSheetCategory* pCategory);

	// PropSheetLook_Tree methods:
	CBCGPPropSheetCategory* AddTreeCategory (LPCTSTR lpszLabel, 
		int nIconNum = -1, int nSelectedIconNum = -1,
		const CBCGPPropSheetCategory* pParentCategory = NULL);
	void AddPageToTree (CBCGPPropSheetCategory* pCategory, 
		CBCGPPropertyPage* pPage, int nIconNum = -1, int nSelIconNum = -1);

// Overrides
	virtual void OnActivatePage (CPropertyPage* pPage);
	virtual CWnd* InitNavigationControl ();
	virtual BOOL OnRemoveTreePage(CPropertyPage* pPage);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPPropertySheet)
	public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPPropertySheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPPropertySheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg LRESULT OnAfterActivatePage(WPARAM,LPARAM);
	afx_msg void OnSelectTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

	void InternalAddPage (int nTab);
	void AddCategoryToTree (CBCGPPropSheetCategory* pCategory);
	void CommonInit ();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPPROPERTYSHEET_H__54B75502_E449_41DD_AEE7_1BB6FBE72205__INCLUDED_)
