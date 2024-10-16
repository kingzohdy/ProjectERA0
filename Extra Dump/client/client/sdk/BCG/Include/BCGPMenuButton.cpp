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

// BCGPMenuButton.cpp : implementation file
//

#include "stdafx.h"
#include "MenuImages.h"
#include "BCGCBPro.h"
#include "BCGPMenuButton.h"
#include "BCGPContextMenuManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int nImageHorzMargin = 10;

/////////////////////////////////////////////////////////////////////////////
// CBCGPMenuButton

IMPLEMENT_DYNAMIC(CBCGPMenuButton, CBCGPButton)

CBCGPMenuButton::CBCGPMenuButton()
{
	m_bRightArrow = FALSE;
	m_hMenu = NULL;
	m_nMenuResult = 0;
	m_bMenuIsActive = FALSE;
	m_bStayPressed = FALSE;
	m_bOSMenu = TRUE;
}
//*****************************************************************************************
CBCGPMenuButton::~CBCGPMenuButton()
{
}


BEGIN_MESSAGE_MAP(CBCGPMenuButton, CBCGPButton)
	//{{AFX_MSG_MAP(CBCGPMenuButton)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_LBUTTONUP()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCGPMenuButton message handlers

CSize CBCGPMenuButton::SizeToContent (BOOL bCalcOnly)
{
	CSize size = CBCGPButton::SizeToContent (FALSE);
	size.cx += CMenuImages::Size ().cx;

	if (!bCalcOnly)
	{
		SetWindowPos (NULL, -1, -1, size.cx, size.cy,
			SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	}

	return size;
}
//*****************************************************************************************
void CBCGPMenuButton::OnDraw (CDC* pDC, const CRect& rect, UINT uiState)
{
	ASSERT_VALID (pDC);

	CSize sizeArrow = CMenuImages::Size ();

	CRect rectParent = rect;
	rectParent.right -= sizeArrow.cx + nImageHorzMargin;

	CBCGPButton::OnDraw (pDC, rectParent, uiState);

	CRect rectArrow = rect;
	rectArrow.left = rectParent.right;

	CPoint pointImage (
		rectArrow.left + (rectArrow.Width () - sizeArrow.cx) / 2,
		rectArrow.top + (rectArrow.Height () - sizeArrow.cy) / 2);

	CMenuImages::Draw (pDC, 
		m_bRightArrow ? CMenuImages::IdArowRightLarge : CMenuImages::IdArowDownLarge, 
		pointImage,
		(uiState & ODS_DISABLED) ? CMenuImages::ImageGray : CMenuImages::ImageBlack);
}
//*****************************************************************************************
void CBCGPMenuButton::OnShowMenu () 
{
	if (m_hMenu == NULL || m_bMenuIsActive)
	{
		return;
	}

	CRect rectWindow;
	GetWindowRect (rectWindow);

	int x, y;

	if (m_bRightArrow)
	{
		x = rectWindow.right;
		y = rectWindow.top;
	}
	else
	{
		x = rectWindow.left;
		y = rectWindow.bottom;
	}

	if (m_bStayPressed)
	{
		m_bPushed = TRUE;
		m_bHighlighted = TRUE;
	}

	m_bMenuIsActive = TRUE;
	Invalidate ();

	if (!m_bOSMenu && g_pContextMenuManager != NULL)
	{
		m_nMenuResult = g_pContextMenuManager->TrackPopupMenu (
			m_hMenu, x, y, this);
		SetFocus ();
	}
	else
	{
		m_nMenuResult = ::TrackPopupMenu (m_hMenu, 
			TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, 
			x, y, 0, GetSafeHwnd (), NULL);
	}

	if (m_nMenuResult != 0)
	{
		//-------------------------------------------------------
		// Trigger mouse up event (to button click notification):
		//-------------------------------------------------------
		CWnd* pParent = GetParent ();
		if (pParent != NULL)
		{
			pParent->SendMessage (	WM_COMMAND,
									MAKEWPARAM (GetDlgCtrlID (), BN_CLICKED),
									(LPARAM) m_hWnd);
		}
	}

	// -- By Thomas Freudenberg:
	m_bPushed = FALSE;
	m_bHighlighted = FALSE;
	m_bMenuIsActive = FALSE;
	
	Invalidate ();
	UpdateWindow ();

	if (m_bCaptured)
	{
		ReleaseCapture ();
		m_bCaptured = FALSE;
	}
	//--
}
//*****************************************************************************************
void CBCGPMenuButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_SPACE || nChar == VK_DOWN)
	{
		OnShowMenu ();
		return;
	}
	
	CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}
//*****************************************************************************************
void CBCGPMenuButton::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/) 
{
	SetFocus ();
	OnShowMenu ();
}
//*****************************************************************************************
UINT CBCGPMenuButton::OnGetDlgCode() 
{
	return DLGC_WANTARROWS;
}
//****************************************************************************************
void CBCGPMenuButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bStayPressed && m_bMenuIsActive && m_bPushed)
	{
		m_bClickiedInside = FALSE;

		CButton::OnLButtonUp(nFlags, point);

		if (m_bCaptured)
		{
			ReleaseCapture ();
			m_bCaptured = FALSE;
		}
	}
	else
	{
		CBCGPButton::OnLButtonUp(nFlags, point);
	}
}
//***************************************************************************************
void CBCGPMenuButton::OnKillFocus(CWnd* pNewWnd) 
{
	if (m_bStayPressed && m_bMenuIsActive && m_bPushed)
	{
		CButton::OnKillFocus(pNewWnd);
		
		if (m_bCaptured)
		{
			ReleaseCapture ();
			m_bCaptured = FALSE;
		}
		
		m_bClickiedInside = FALSE;
		m_bHover = FALSE;
	}
	else
	{
		CBCGPButton::OnKillFocus(pNewWnd);
	}
}
