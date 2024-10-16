#if !defined(AFX_CALENDARPOPUP_H__6ABB7DC6_CCA2_4B65_9269_3F41D3F21D3D__INCLUDED_)
#define AFX_CALENDARPOPUP_H__6ABB7DC6_CCA2_4B65_9269_3F41D3F21D3D__INCLUDED_

// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2000 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalendarPopup.h : header file
//

#ifndef BCG_NO_CALENDAR

#include "BCGPPopupMenu.h"
#include "BCGPCalendarBar.h"

class CBCGPControlBar;
class CBCGPPropList;

/////////////////////////////////////////////////////////////////////////////
// CCalendarPopup window

class CCalendarPopup : public CBCGPPopupMenu
{
	friend class CBCGPCalendarMenuButton;
	friend class CBCGPDateTimeCtrl;
	friend class CBCGPCalendarProp;

	DECLARE_DYNAMIC(CCalendarPopup)

// Construction
protected:
    CCalendarPopup (
					const COleDateTime& month, 
					UINT uiCommandID)
		: m_wndCalendarBar ( month, uiCommandID, NULL)
	{
		m_bEnabledInCustomizeMode = FALSE;
	}

    CCalendarPopup (	CBCGPDateTimeCtrl* pParentBtn,
					const COleDateTime& month)
		: m_wndCalendarBar (month, (UINT)-1,
						pParentBtn)
	{
		m_bEnabledInCustomizeMode = FALSE;
	}

	virtual ~CCalendarPopup();

// Attributes
protected:
	CBCGPCalendarBar	m_wndCalendarBar;
	BOOL			m_bEnabledInCustomizeMode;

protected:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCalendarPopup)
	//}}AFX_VIRTUAL

	virtual CBCGPPopupMenuBar* GetMenuBar ()
	{
		return &m_wndCalendarBar;
	}

	virtual CBCGPControlBar* CreateTearOffBar (CFrameWnd* pWndMain, UINT uiID, LPCTSTR lpszName);

    // Generated message map functions
protected:
    //{{AFX_MSG(CCalendarPopup)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // BCG_NO_CALENDAR

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALENDARPOPUP_H__6ABB7DC6_CCA2_4B65_9269_3F41D3F21D3D__INCLUDED_)
