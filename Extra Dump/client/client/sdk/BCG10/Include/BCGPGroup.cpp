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
// BCGPGroup.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPGroup.h"
#include "Bcgglobals.h"
#include "BCGPVisualManager.h"
#include "BCGPDlgImpl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPGroup

IMPLEMENT_DYNAMIC(CBCGPGroup, CButton)

CBCGPGroup::CBCGPGroup()
{
	m_bVisualManagerStyle = FALSE;
	m_bOnGlass = FALSE;
}

CBCGPGroup::~CBCGPGroup()
{
}


BEGIN_MESSAGE_MAP(CBCGPGroup, CButton)
	//{{AFX_MSG_MAP(CBCGPGroup)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(BCGM_ONSETCONTROLVMMODE, OnBCGSetControlVMMode)
	ON_REGISTERED_MESSAGE(BCGM_ONSETCONTROLAERO, OnBCGSetControlAero)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPGroup message handlers

BOOL CBCGPGroup::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}
//**************************************************************************
void CBCGPGroup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBCGPMemDC* pMemDC = NULL;
	CDC* pDC = &dc;

	if (m_bOnGlass)
	{
		pMemDC = new CBCGPMemDC (dc, this);
		pDC = &pMemDC->GetDC ();
		globalData.DrawParentBackground (this, pDC);
	}

	CRect rectClient;
	GetClientRect (rectClient);

	CString strName;
	GetWindowText (strName);

	CFont* pOldFont = (CFont*) pDC->SelectStockObject (DEFAULT_GUI_FONT);
	ASSERT(pOldFont != NULL);

	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextColor (globalData.clrBarText);

	CBCGPVisualManager::GetInstance ()->OnDrawGroup (pDC, this, rectClient, strName);

	pDC->SelectObject (pOldFont);

	if (pMemDC != NULL)
	{
		delete pMemDC;
	}
}
//**************************************************************************
LRESULT CBCGPGroup::OnBCGSetControlVMMode (WPARAM wp, LPARAM)
{
	m_bVisualManagerStyle = (BOOL) wp;
	return 0;
}
//**************************************************************************
LRESULT CBCGPGroup::OnBCGSetControlAero (WPARAM wp, LPARAM)
{
	m_bOnGlass = (BOOL) wp;
	return 0;
}
