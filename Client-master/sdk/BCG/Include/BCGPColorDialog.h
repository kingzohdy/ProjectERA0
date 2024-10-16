#if !defined(AFX_BCGPCOLORDIALOG_H__2C7F710C_9879_49AC_B4F5_3A6699211BE3__INCLUDED_)
#define AFX_BCGPCOLORDIALOG_H__2C7F710C_9879_49AC_B4F5_3A6699211BE3__INCLUDED_

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
// BCGColorDialog.h : header file
//

#ifndef BCG_NO_COLOR

class CPropertySheetCtrl;

#include "BCGCBPro.h"
#include "bcgprores.h"
#include "BCGPPowerColorPicker.h"
#include "ColorPage1.h"
#include "ColorPage2.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPColorDialog dialog

class BCGCBPRODLLEXPORT CBCGPColorDialog : public CDialog
{
// Construction

public:
	CBCGPColorDialog (COLORREF clrInit = 0, DWORD dwFlags = 0 /* reserved */, 
					CWnd* pParentWnd = NULL,
					HPALETTE hPal = NULL);

	virtual ~CBCGPColorDialog ();

	void SetCurrentColor(COLORREF rgb);
	void SetNewColor(COLORREF rgb);

	COLORREF GetColor () const
	{
		return m_NewColor;
	}

	void SetPageTwo(BYTE R, BYTE G, BYTE B);
	void SetPageOne(BYTE R, BYTE G, BYTE B);

	CPalette* GetPalette () const
	{
		return m_pPalette;
	}

// Dialog Data
	//{{AFX_DATA(CBCGPColorDialog)
	enum { IDD = IDD_BCGBARRES_COLOR_DLG };
	CStatic	m_wndStaticPlaceHolder;
	CBCGPColorPickerCtrl m_wndColors;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPColorDialog)
	public:
	virtual INT_PTR DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CPropertySheetCtrl* m_pPropSheet;
	CColorPage1* m_pColourSheetOne;
	CColorPage2* m_pColourSheetTwo;

	COLORREF	m_CurrentColor;
	COLORREF	m_NewColor;
	CPalette*	m_pPalette;
	BOOL		m_bIsMyPalette;

	// Generated message map functions
	//{{AFX_MSG(CBCGPColorDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void RebuildPalette ();
};

#endif // BCG_NO_COLOR

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPCOLORDIALOG_H__2C7F710C_9879_49AC_B4F5_3A6699211BE3__INCLUDED_)
