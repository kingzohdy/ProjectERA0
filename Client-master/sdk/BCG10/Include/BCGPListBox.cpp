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
// BCGPListBox.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPListBox.h"
#include "BCGPDlgImpl.h"
#include "BCGPVisualManager.h"
#include "TrackMouse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPListBox

CBCGPListBox::CBCGPListBox()
{
	m_bVisualManagerStyle = FALSE;
	m_bOnGlass = FALSE;
	m_nHighlightedItem = -1;
	m_bTracked = FALSE;
}

CBCGPListBox::~CBCGPListBox()
{
}


BEGIN_MESSAGE_MAP(CBCGPListBox, CListBox)
	//{{AFX_MSG_MAP(CBCGPListBox)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(BCGM_ONSETCONTROLVMMODE, OnBCGSetControlVMMode)
	ON_REGISTERED_MESSAGE(BCGM_ONSETCONTROLAERO, OnBCGSetControlAero)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPListBox message handlers

LRESULT CBCGPListBox::OnBCGSetControlVMMode (WPARAM wp, LPARAM)
{
	m_bVisualManagerStyle = (BOOL) wp;
	return 0;
}
//**************************************************************************
LRESULT CBCGPListBox::OnBCGSetControlAero (WPARAM wp, LPARAM)
{
	m_bOnGlass = (BOOL) wp;
	return 0;
}
//**************************************************************************
void CBCGPListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	CListBox::OnMouseMove (nFlags, point);

	ASSERT (this->IsWindowEnabled ());

	CRect rectItem;

	int nHighlightedItem = -1;

	for (int i = 0; i < GetCount (); i++)
	{
		GetItemRect (i, rectItem);

		if (rectItem.PtInRect (point))
		{
			nHighlightedItem = i;
			break;
		}
	}

	if (!m_bTracked)
	{
		m_bTracked = TRUE;
		
		TRACKMOUSEEVENT trackmouseevent;
		trackmouseevent.cbSize = sizeof(trackmouseevent);
		trackmouseevent.dwFlags = TME_LEAVE;
		trackmouseevent.hwndTrack = GetSafeHwnd();
		trackmouseevent.dwHoverTime = HOVER_DEFAULT;
		BCGPTrackMouse (&trackmouseevent);
	}

	if (nHighlightedItem != m_nHighlightedItem)
	{
		if (m_nHighlightedItem >= 0)
		{
			GetItemRect (m_nHighlightedItem, rectItem);
			InvalidateRect (rectItem);
		}

		m_nHighlightedItem = nHighlightedItem;

		if (m_nHighlightedItem >= 0)
		{
			GetItemRect (m_nHighlightedItem, rectItem);
			InvalidateRect (rectItem);
		}

		UpdateWindow ();
	}
}
//***********************************************************************************************	
LRESULT CBCGPListBox::OnMouseLeave(WPARAM,LPARAM)
{
	m_bTracked = FALSE;

	if (m_nHighlightedItem >= 0)
	{
		CRect rectItem;
		GetItemRect (m_nHighlightedItem, rectItem);

		m_nHighlightedItem = -1;
		RedrawWindow (rectItem);
	}

	return 0;
}
//***********************************************************************************************	
void CBCGPListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	int nIndex = lpDIS->itemID;

	if (nIndex < 0)
	{
		return;
	}

	CRect rect = lpDIS->rcItem;

	const BOOL bIsSelected = (lpDIS->itemState & ODS_SELECTED);
	const BOOL bIsHighlihted = nIndex == m_nHighlightedItem;

	CDC* pDC = CDC::FromHandle (lpDIS->hDC);
	ASSERT_VALID (pDC);

	pDC->SetBkMode (TRANSPARENT);

	CFont* pOldFont = pDC->SelectObject (&globalData.fontRegular);
	ASSERT_VALID (pOldFont);

	pDC->FillRect (rect, &globalData.brWindow);

	COLORREF clrText = (COLORREF)-1;

	if (bIsHighlihted || bIsSelected)
	{
		if (m_bVisualManagerStyle)
		{
			clrText = CBCGPVisualManager::GetInstance ()->OnFillListBoxItem (
				pDC, this, nIndex, rect, bIsHighlihted, bIsSelected);
		}
		else
		{
			pDC->FillRect (rect, &globalData.brHilite);
			
			if (bIsHighlihted)
			{
				pDC->DrawFocusRect (rect);
			}

			clrText = globalData.clrTextHilite;
		}
	}

	if (clrText == (COLORREF)-1)
	{
		pDC->SetTextColor (globalData.clrWindowText);
	}
	else
	{
		pDC->SetTextColor (clrText);
	}

	CRect rectText = rect;
	rectText.DeflateRect (10, 0);

	CString strText;
	GetText (nIndex, strText);

	pDC->DrawText (strText, rectText, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
	pDC->SelectObject (pOldFont);
}
//***********************************************************************************************	
void CBCGPListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight = globalData.GetTextHeight () * 9 / 5;
}

