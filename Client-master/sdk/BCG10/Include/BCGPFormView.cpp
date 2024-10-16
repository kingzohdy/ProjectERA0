//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2008 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPFormView.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPFormView.h"
#include "BCGPVisualManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPFormView

IMPLEMENT_DYNAMIC(CBCGPFormView, CFormView)

#pragma warning (disable : 4355)

CBCGPFormView::CBCGPFormView(LPCTSTR lpszTemplateName)
	: CFormView(lpszTemplateName),
	m_Impl (*this)
{
}

CBCGPFormView::CBCGPFormView(UINT nIDTemplate)
	: CFormView(nIDTemplate),
	m_Impl (*this)
{
}

#pragma warning (default : 4355)

CBCGPFormView::~CBCGPFormView()
{
}

BEGIN_MESSAGE_MAP(CBCGPFormView, CFormView)
	//{{AFX_MSG_MAP(CBCGPFormView)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPFormView diagnostics

#ifdef _DEBUG
void CBCGPFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CBCGPFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBCGPFormView message handlers

HBRUSH CBCGPFormView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if (IsVisualManagerStyle ())
	{
		HBRUSH hbr = m_Impl.OnCtlColor (pDC, pWnd, nCtlColor);
		if (hbr != NULL)
		{
			return hbr;
		}
	}	

	return CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
}
//************************************************************************
BOOL CBCGPFormView::OnEraseBkgnd(CDC* pDC) 
{
	CRect rectClient;
	GetClientRect (rectClient);

	if (IsVisualManagerStyle () &&
		CBCGPVisualManager::GetInstance ()->OnFillDialog (pDC, this, rectClient))
	{
		return TRUE;
	}

	return CFormView::OnEraseBkgnd(pDC);
}
//************************************************************************
void CBCGPFormView::EnableVisualManagerStyle (BOOL bEnable)
{
	ASSERT_VALID (this);

	m_Impl.EnableVisualManagerStyle (bEnable);
	m_Impl.m_bTransparentStaticCtrls = FALSE;
}
//************************************************************************
void CBCGPFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	if (IsVisualManagerStyle ())
	{
		m_Impl.EnableVisualManagerStyle (TRUE);
	}
}
//************************************************************************
void CBCGPFormView::OnDestroy() 
{
	m_Impl.OnDestroy ();
	CFormView::OnDestroy();
}
